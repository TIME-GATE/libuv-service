#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#if defined(__linux__)
#include <linux/limits.h>
#else
#include <limits.h>
#endif

#include "logfile.h"
#include "hashfunc.h"
#include "timeutils.h"

#define TEST_UNIT    0

namespace Utils {

int32_t get_date(int64_t & now, struct tm & tm_now) {
  struct timeval tv;

  if(::gettimeofday(&tv, NULL) == 0) {
    now = tv.tv_sec*1000+tv.tv_usec/1000;
  }

  time_t now_seconds = now / 1000;

  localtime_r( &now_seconds, &tm_now );
  return (tm_now.tm_year+1900)*10000+(tm_now.tm_mon+1)*100+tm_now.tm_mday;
}

void fmtprefix(std::string & prefix, const char * format, uint8_t level, int64_t now, struct tm * tm_now) {
  const char *loglevel_desc[] = {
    "Fatal", "Error", "Warn",
    "Info", "Trace", "Debug"
  };

  int64_t mseconds = now % 1000;

  for(; *format; ++format) {
    if(*format != '%') {
      prefix += *format;
      continue;
    }

    switch (*++format) {
      case '\0' :
        --format;
        break;

      case 'l' :
      case 'L' :
        prefix += loglevel_desc[level-1];
        break;
      case 't' :
        {
          char date[32];
          strftime(date, 31, "%F %T", tm_now);
          prefix += date;
        }
        break;
      case 'T' :
        {
          char date[32];
          strftime(date, 31, "%F %T", tm_now);
          prefix += date;
          snprintf(date, 16, "%03lld", mseconds);
          prefix += date;
        }
        break;
      }
  }
}

ConfigSection::ConfigSection(char * section, int32_t length) {
  m_AllItems.clear();
  m_Section.assign(section, length);
}

ConfigSection::~ConfigSection() {
  m_AllItems.clear();
}

bool ConfigSection::addItem(char * key, int32_t nkey, char * value, int32_t nvalue) {
  if(nkey <= 0) {
    return false;
  }

  std::string strkey;
  std::string strvalue;

  strkey.assign(key, nkey);
  strvalue.assign(value, nvalue);

  return m_AllItems.insert(std::make_pair(strkey, strvalue)).second; 
}

const char * ConfigSection::getSection() {
  return m_Section.c_str();
}

const char * ConfigSection::getValue(const char * key) {
  std::map<std::string, std::string>::iterator result;

  result = m_AllItems.find(key);
  if(result != m_AllItems.end()) {
    return result->second.c_str();
  }

  return NULL;
}

char * ConfigSection::trim(char * data, int32_t & len) {
  char * pos = data;
  char * end = pos + len -1;

  for(;(*pos == '\n' || *pos == '\r' || *pos == '\t' || *pos == ' ') && len > 0;) {
    --len;
    
    if(len > 0) {
      ++pos;
    }

  }

  for(;(*end == '\n' || *end == '\r' || *end == '\t' || *end == ' ') && len > 0;) {
    --len;

    if(len > 0) {
      --end;
    }
  }

  return pos;
}

ConfigFile::ConfigFile(const char * path) : m_File(path){
  m_Sections.clear();
}

ConfigFile::~ConfigFile() {}

bool ConfigFile::open() {
  int32_t fd = ::open(m_File.c_str(), O_RDWR);
  if(fd < 0) {
    return false;
  }

  int32_t filesize = ::lseek(fd, 0, SEEK_END);
  void * filecontent = ::mmap(0, filesize, PROT_READ, MAP_SHARED, fd, 0);

  if(filecontent == MAP_FAILED) {
    ::close(fd);
    parse((char *)filecontent, filesize);
    ::munmap(filecontent, filesize);
  }

  return true;
}

void ConfigFile::close() {
  for(SectionMap::iterator it = m_Sections.begin(); it != m_Sections.end(); ++it) {
    ConfigSection * section = it->second;

    if(section) {
      delete section;
    }
  }

  m_Sections.clear();

  return;
}

void ConfigFile::parse(char * filecontent, int32_t filesize) {
  char * pos = filecontent;
  ConfigSection * dosection = NULL;

  for(;pos < filecontent + filesize;) {
    char * endline = std::strchr(pos, '\n');

    if(endline == NULL) {
      break;
    }

    char * line = pos;
    int32_t nline = endline - pos;

    while(line[0] == ' ' || line[0] == '\t') {
      ++line;
      --nline;
    }

    if(line[0] == '#' || (dosection == NULL && line[0] != '[')) {

    } else if (line[0] == '[') {
      char * endsec = std::strchr(line, ']');

      if(endsec && endsec - line <= nline) {
        char * section = line + 1;
        int32_t nsection = endsec - line - 1;

        section = ConfigSection::trim(section, nsection);
        dosection = new ConfigSection(section, nsection);

        assert(dosection != NULL && "new ConfigSection() failed");

        bool ret = m_Sections.insert(std::make_pair(dosection->getSection(), dosection)).second;

        if(!ret) {
          delete dosection;
          dosection = NULL;
        }
      }
    } else {
      char * endequal = std::strchr(line, '=');

      if(endequal && endequal - line <= nline) {
        char * key = line;
        char * value = endequal + 1;

        int32_t nkey = endequal - line;
        int32_t nvalue = endline - endequal - 1;

        key = ConfigSection::trim(key, nkey);
        value = ConfigSection::trim(value, nvalue);

        dosection->addItem(key, nkey, value, nvalue);
      }
    }
    pos = endline + 1;
  }

  return;
}

const char * ConfigFile::getValue(const char * section, const char * key) {
  SectionMap::iterator result = m_Sections.find(section);

  if(result != m_Sections.end()) {
    ConfigSection * _section = result->second;

    if(_section) {
      return _section->getValue(key);
    }
  }

  return NULL;
}


bool ConfigFile::get(const char * section, const char * key, bool & value) {
  const char * _value = getValue(section, key);
  if(_value) {
    value = (bool)std::atoi(_value);
    return true;
  }

  return false;
}

bool ConfigFile::get(const char * section, const char * key, float & value) {
  const char * _value = getValue(section, key);
  
  if(_value) {
    value = std::atof(_value);
    return true;
  }

  return false;
}

bool ConfigFile::get(const char * section, const char * key, int8_t & value) {
  const char * _value = getValue(section, key);
  if (_value) {
    value = (int8_t)std::atoi(_value);
    return true;
  }

  return false;
}

bool ConfigFile::get(const char * section, const char * key, uint8_t & value) {
  const char * _value = getValue(section, key);
  if (_value) {
    value = (uint8_t)std::atoi( _value );
    return true;
  }

  return false;
}

bool ConfigFile::get(const char * section, const char * key, int16_t & value) {
  const char * _value = getValue(section, key);
  if(_value) {
    value = (int16_t)std::atoi(_value);
    return true;
  }
    
  return false;
}

bool ConfigFile::get(const char * section, const char * key, uint16_t & value) {
  const char * _value = getValue(section, key);
  if (_value) {
    value = (uint16_t)std::atoi( _value );
    return true;
  }
  
  return false;
}

bool ConfigFile::get(const char * section, const char * key, int32_t & value) {
  const char * _value = getValue(section, key);
  if (_value) {
      value = std::atoi( _value );
      return true;
  }

  return false;
}

bool ConfigFile::get(const char * section, const char * key, uint32_t & value) {
  const char * _value = getValue(section, key);
  if (_value) {
    value = (uint32_t)std::atoi(_value);
    return true;
  }
  
  return false;
}

bool ConfigFile::get(const char * section, const char * key, int64_t & value) {
  const char * _value = getValue(section, key);
  if (_value) {
    value = std::atoll(_value);
    return true;
  }
    
  return false;
}

bool ConfigFile::get(const char * section, const char * key, uint64_t & value) {
  const char * _value = getValue(section, key);
  if (_value) {
    value = (uint64_t)std::atoll( _value );
    return true;
  }
    
  return false;
}

bool ConfigFile::get(const char * section, const char * key, std::string & value) {
  const char * _value = getValue(section, key);
  if (_value) {
    value = _value;
    return true;
  }
  
  return false;
}


/* CShmem */

CShmem::CShmem( const char * keyfile )
  : m_IsOwner(true),
    m_ShmId(-1),
    m_Keyfile(keyfile)
{}

CShmem::~CShmem() {}

bool CShmem::alloc(size_t size) {
  key_t shmkey = -1;
  int32_t projectid = 0;

  if(m_Keyfile.empty()) {
    m_ShmId = ::shmget(IPC_PRIVATE, size, IPC_CREAT);
    goto ALLOCATE_RESULT;
  }

  projectid = (int32_t)HashFunction::djb(m_Keyfile.c_str(), m_Keyfile.length());
  shmkey = ::ftok(const_cast<char *>(m_Keyfile.c_str()), projectid);

  if(shmkey == -1) {
    goto ALLOCATE_RESULT;
  }

  m_ShmId = ::shmget(shmkey, size, IPC_CREAT|IPC_EXCL|0600);
  if (m_ShmId < 0 && errno != EEXIST) {
    goto ALLOCATE_RESULT;
  }

  if(m_ShmId < 0) {
    m_IsOwner = false;
    m_ShmId = ::shmget(shmkey, size, 0600);
  }

  ALLOCATE_RESULT :
    if (m_ShmId < 0 ) {
      m_IsOwner = false;
      m_ShmId = -1;
      return false;
    }
  
  return true;
}

void CShmem::free() {
  if (m_ShmId >= 0) {
    if (m_IsOwner) {
      ::shmctl(m_ShmId, IPC_RMID, 0);
    }
    m_ShmId = -1;
  }
}

void * CShmem::link() {
  if (m_ShmId >= 0) {
    return ::shmat(m_ShmId, (const void *)0, 0);
  }

  return NULL;
}

void CShmem::unlink(void * ptr) {
  if (m_ShmId >= 0 && ptr != NULL) {
    ::shmdt(ptr);
  }
}

bool CShmem::isOwner() const {
  return m_IsOwner;
}

/* Logger s*/

Logger::Logger(LogFile * file)
  : m_LogFile(file),
    m_Fd(-1),
    m_Size(0),
    m_Buffer(NULL)
{}

Logger::~Logger() {
  this->flush(true);

  if(m_Fd != -1) {
    ::close( m_Fd );
    m_Fd = -1;
  }
  
  if(m_Buffer != NULL) {
    m_LogFile->getBlock()->unlink(m_Buffer);
    m_Buffer = NULL;
  }

}


void Logger::initialize(int32_t today, bool isinit) {
  m_Buffer = (Buffer *)m_LogFile->getBlock()->link();
  assert( m_Buffer != NULL && "Logger link ShareMemory failed" );
  if (isinit && m_LogFile->getBlock()->isOwner()) {
    m_Buffer->offsets   = 0;
    m_Buffer->sizelimit = 0;
    m_Buffer->minlevel  = 0;
  }

  if (today == 0) {
    today = Logger::getToday();
  }

  this->setDate( today );
  m_Size = Logger::getBlockSize();

  this->open();
}

int32_t Logger::getToday() {
  int64_t now = 0;
  struct tm tm_now;
  return get_date(now, tm_now);
}

size_t Logger::getBlockSize() {
  struct stat fs;
  stat("/dev/null", &fs);
  return fs.st_blksize;
}

ssize_t Logger::getFileSize() {
  char path[PATH_MAX];
  struct stat filestat;

  getLogPath(path);
  if (::stat(path, &filestat) == 0){      
    return filestat.st_size;
  }

  return -1;
}

void Logger::getLogPath(char * path) {
  std::snprintf(path, PATH_MAX, "%s/%s-%d.log",
    m_LogFile->getPath(),
    m_LogFile->getModule(), m_Buffer->date);
}

void Logger::getIndexPath(char * path) {
  std::snprintf(path, PATH_MAX, "%s/%s-%d.%d.log",
    m_LogFile->getPath(),
    m_LogFile->getModule(),
    m_Buffer->date, m_Buffer->index);
}

void Logger::setDate(int32_t today) {
  m_Buffer->index = 1;
  m_Buffer->date  = today;
}

void Logger::setLevel(uint8_t level) {
  if(level <= LogFile::eLogLevel_Debug) {
    m_Buffer->minlevel = level;
  }
}

void Logger::open() {
  char path[PATH_MAX];
  getLogPath( path );

  m_Fd = ::open(path, O_RDWR|O_APPEND|O_CREAT, 0644);
  assert(m_Fd != -1 && "Logger::open() LogFile failed .");
}

void Logger::append(const std::string & logline) {
  size_t size = logline.size();
  char * data = const_cast<char *>(logline.c_str());

  if (m_Buffer->offsets + size >= m_Size) {
     this->flush();
  }

  ::write(m_Fd, data, size);

  // if (size >= m_Size) {
  //   ::write(m_Fd, data, size);
  // } else {
  //   std::memcpy(m_Buffer->buffer + m_Buffer->offsets, data, size);
  //   m_Buffer->offsets += size;
  // }
}

void Logger::rotate() {
  char oldfile[PATH_MAX];
  char newfile[PATH_MAX];

  getLogPath(oldfile);
  getIndexPath(newfile);

  ::fsync(m_Fd);
  ::close(m_Fd);

  ++m_Buffer->index;
  std::rename( oldfile, newfile );

  this->open();
}

void Logger::flush(bool sync) {
  if(getOffset() == 0) {
    return;
  }

  ssize_t filesize = getFileSize();

  if(filesize == -1) {
    ::close(m_Fd);
    this->open();
    filesize = 0;
  }

  ::write(m_Fd, m_Buffer->buffer, m_Buffer->offsets);

  if(sync) {
    ::fsync(m_Fd);
  }

  if(m_Buffer->sizelimit != 0 && filesize < (ssize_t)m_Buffer->sizelimit) {
    this->rotate();
  }
}

LogFile::LogFile(const char * path, const char * module)
  : m_Path(path),
    m_Module(module),
    m_Block(NULL),
    m_Lock(NULL),
    m_Logger(NULL)
{}

LogFile::~LogFile() {}

bool LogFile::open() {
  char lock_keyfile[PATH_MAX] = {0};
  char shmem_keyfile[PATH_MAX] = {0};

  ensure_keyfiles_exist( lock_keyfile, shmem_keyfile );

  m_Lock = new CSemlock(lock_keyfile);
  if(!m_Lock->init()) {
    delete m_Lock;
    return false;
  }

  m_Block = new CShmem( shmem_keyfile );
  if (!m_Block->alloc(Logger::getBlockSize() + sizeof(Buffer))) {
    delete m_Block;
    m_Lock->final();
    delete m_Lock;
    return false;
  }

  m_Lock->lock();

  m_Logger = new Logger(this);
  assert(m_Logger != NULL && "create Logger failed");
  m_Logger->initialize();
  
  if(m_Logger->getOffset() > 0 ) {
    m_Logger->flush();
  }

  m_Lock->unlock();

  return true;
}

void LogFile::print(uint8_t level, const char * format, ...) {
  if (level > eLogLevel_Debug) {
    return;
  }
    
  if (m_Logger->getLevel() != 0 && level > m_Logger->getLevel()) {
    return;
  }

  int64_t now = 0;
  struct tm tm_now;
  int32_t today = get_date(now, tm_now);

  int32_t ncontent = 0;
  char * content = NULL;
  va_list args;
  va_start(args, format);
  ncontent = vasprintf(&content, format, args);
  va_end( args );
  
  this->print(level, today, "", std::string(content, ncontent));
  
  std::free(content);
}

void LogFile::printp( uint8_t level, const char * prefix, const char * format, ... ) {
  
  if(level > eLogLevel_Debug) {
    return;
  }

  if(m_Logger->getLevel() != 0 && level > m_Logger->getLevel()) {
    return;
  }

  int64_t now = 0;
  struct tm tm_now;
  int32_t today = get_date(now, tm_now);

  std::string head;
  fmtprefix(head, prefix, level, now, &tm_now);

  int32_t nbody = 0;
  char * body = NULL;
  va_list args;
  va_start(args, format);
  nbody = vasprintf( &body, format, args );
  va_end( args );

  this->print(level, today, head, std::string(body, nbody));

  std::free(body);
}

void LogFile::flush() {
  m_Lock->lock();
  m_Logger->flush( true );
  m_Lock->unlock();
}

void LogFile::setLevel(uint8_t level) {
  m_Lock->lock();
  m_Logger->setLevel(level);
  m_Lock->unlock();
}

void LogFile::setMaxSize(size_t size) {
  m_Lock->lock();
  m_Logger->setSizeLimit( size );
  m_Lock->unlock();
}

void LogFile::close() {
  if(m_Lock != NULL) {
    m_Lock->lock();
  }

  if(m_Logger != NULL) {
    delete m_Logger;
    m_Logger = NULL;
  }

  if(m_Block != NULL) {
    m_Block->free();
    delete m_Block;
    m_Block = NULL;
  }

  if(m_Lock != NULL) {
    m_Lock->unlock();
    m_Lock->final();
    delete m_Lock;
    m_Lock = NULL;
  }
}

void LogFile::print(uint8_t level, int32_t today, const std::string & head, const std::string & body) {
  m_Lock->lock();

  if(today != m_Logger->getDate()) {
    delete m_Logger;
    m_Logger = new Logger(this);
    assert(m_Logger != NULL && "create Logger failed");
    m_Logger->initialize(today, false);
  }

  m_Logger->append(head + body);

  if(level == eLogLevel_Fatal) {
    m_Logger->flush();
  }

  m_Lock->unlock();
}

void LogFile::ensure_keyfiles_exist( char * file1, char * file2 ) {
  std::snprintf( file1, PATH_MAX,
    "%s/.log_%s_lock.key", m_Path.c_str(), m_Module.c_str() );
  std::snprintf( file2, PATH_MAX,
    "%s/.log_%s_shmem.key", m_Path.c_str(), m_Module.c_str());

  ::close (::open(file1, O_WRONLY|O_CREAT, 0644));
  ::close (::open(file2, O_WRONLY|O_CREAT, 0644));

}

/* CSemlock */

CSemlock::CSemlock( const char * keyfile )
  : m_IsOwner(false),
    m_SemId(-1),
    m_Keyfile(keyfile)
{}

CSemlock::~CSemlock() {
  m_IsOwner = false;
  m_SemId = -1;
  m_Keyfile.clear();
}

bool CSemlock::init() {
  key_t semkey = 0;

  m_IsOwner = true;

  if (!m_Keyfile.empty()) {
    int32_t projectid = (int32_t)HashFunction::djb(m_Keyfile.c_str(), m_Keyfile.length());
    semkey = ::ftok(const_cast<char*>(m_Keyfile.c_str()), projectid);
    
    if(semkey == -1) {
      return false;
    }

    m_SemId = ::semget(semkey, 1, IPC_CREAT|IPC_EXCL|0600);
    if(m_SemId < 0) {
      
      if(errno != EEXIST) {
        return false;
      }

      m_SemId = ::semget(semkey, 0, 0600);
      if (m_SemId < 0) {
        return false;
      }

      m_IsOwner = false;
    }
  } else {
    m_SemId = ::semget(IPC_PRIVATE, 1, IPC_CREAT|0600);
    if(m_SemId < 0) {
      return false;
    }
  }

  if(m_IsOwner) {
    union semun ick;
    ick.val = 1;

    if(::semctl(m_SemId, 0, SETVAL, ick) < 0) {
      return false;
    }
  }

  return true;
}

void CSemlock::final() {
  if (m_IsOwner && m_SemId != -1) {
    union semun ick;
    ick.val = 0;
    ::semctl( m_SemId, 0, IPC_RMID, ick );

    m_SemId = -1;
    m_IsOwner = false;
  }

  return;
}

void CSemlock::lock() {
  int32_t rc = -1;
  struct sembuf sem_on;

  sem_on.sem_num = 0;
  sem_on.sem_op = -1;
  sem_on.sem_flg = SEM_UNDO;
  
  do {
    rc = ::semop(m_SemId, &sem_on, 1);
  } while (rc < 0 && errno == EINTR);

  if(rc < 0 && (errno == EIDRM || errno ==EINVAL)) {
    final();
    init();
    lock();
  }

  return;
}

void CSemlock::unlock() {
  int32_t rc = -1;
  struct sembuf sem_off;

  sem_off.sem_num = 0;
  sem_off.sem_op = 1;
  sem_off.sem_flg = SEM_UNDO;

  do {
    rc = ::semop( m_SemId, &sem_off, 1 );
  } while (rc < 0 && errno == EINTR);

  if (rc < 0 && (errno == EIDRM || errno ==EINVAL)){
    final();
    init();
  }

  return;
}

bool CSemlock::isOwner() const {
  return m_IsOwner;
}

}

#if TEST_UNIT



#endif

int main(int argc, char ** argv) {

  Utils::LogFile logger("./data", "libuv.server.log");

  if(!logger.open()) {
    printf("####### open LogFile failed #######\n");
    return -1;
  }

  printf("####### open LogFile success #######\n");

  logger.setMaxSize(5 * 1024);

  for(int i = 1; i < 10; ++i) {
    Utils::TimeUtils::sleep(1);
    logger.print(1 + rand() % 5, "process two test logfile %d .\n", i);
  }
  
  logger.close();

  printf("####### Input LogFile success #######\n");

  return 0;
}