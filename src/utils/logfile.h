#ifndef __SRC_UTILS_LOGFILE_H__
#define __SRC_UTILS_LOGFILE_H__

#include <map>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

namespace Utils {

class Logger;
class CShmem;
class CSemlock;

struct Buffer {
  int32_t     date;       // 记录日志的日期
  int32_t     index;      // 记录当前日期下日志文件的索引
  uint8_t     minlevel;   //
  size_t      sizelimit;  // 文件大小限制
  int32_t     offsets;    // 日志缓存的偏移量
  char        buffer[0];  // 日志缓存

  Buffer() {
    date        = 0;
    index       = 0;
    minlevel    = 0;
    sizelimit   = 0;
    offsets     = 0;
  };

};

class ConfigSection {

  public:

    ConfigSection(char * section, int32_t length);
    ~ConfigSection();

  public:

    bool addItem(char * key, int32_t nkey, char * value, int32_t nvalue);

    const char * getSection();

    const char * getValue(const char * key);

    static char * trim(char * data, int32_t & len);
  
  private:

    std::string m_Section;

    std::map<std::string, std::string>  m_AllItems;
};

class ConfigFile {

  public:

    ConfigFile(const char * path);
    ~ConfigFile();
    
  public:

    bool open();
    void close();
    
  public:

    bool get(const char * section, const char * key, bool & value);
    bool get(const char * section, const char * key, float & value);
    bool get(const char * section, const char * key, int8_t & value);
    bool get(const char * section, const char * key, uint8_t & value);
    bool get(const char * section, const char * key, int16_t & value);
    bool get(const char * section, const char * key, uint16_t & value);
    bool get(const char * section, const char * key, int32_t & value);
    bool get(const char * section, const char * key, uint32_t & value);
    bool get(const char * section, const char * key, int64_t & value);
    bool get(const char * section, const char * key, uint64_t & value);
    bool get(const char * section, const char * key, std::string & value);

  public:

    void parse(char * filecontent, int32_t filesize);

    const char * getValue(const char * section, const char * key);

  private:

    typedef std::map<std::string, ConfigSection *> SectionMap;
    std::string m_File;
    SectionMap m_Sections;
    
};

class CShmem {

  public:

    CShmem(const char * keyfile);
    ~CShmem();

  public:

    bool alloc(size_t size);

    void free();

    void * link();

    void unlink(void * ptr);

    bool isOwner() const;
  
  private:

    bool m_IsOwner;

    int32_t m_ShmId;

    std::string m_Keyfile;
};

class CSemlock {

  public:

    CSemlock(const char * keyfile);
    ~CSemlock();

  public:

    bool init();

    void final();

    void lock();

    void unlock();

    bool isOwner() const;

  private:

    bool m_IsOwner;

    int32_t m_SemId;

    std::string m_Keyfile;
};

class LogFile {
  public:

    enum {
      eLogLevel_Fatal = 1,        // 严重
      eLogLevel_Error = 2,        // 错误
      eLogLevel_Warn  = 3,        // 警告
      eLogLevel_Info  = 4,        // 信息
      eLogLevel_Trace = 5,        // 跟踪
      eLogLevel_Debug = 6,        // 调试
    };

    LogFile(const char * path, const char * module);
    ~LogFile();

  public:

    bool open();

    void print(uint8_t level, const char * format, ...);

    void printp(uint8_t level, const char * prefix, const char * format, ...);

    void flush();

    void close();

    void setLevel(uint8_t level);

    void setMaxSize(size_t size);

  private:

    friend class Logger;

    void print(uint8_t level, int32_t today, const std::string & head, const std::string & body);

    void ensure_keyfiles_exist(char * file1, char * file2);

    const char * getPath() const { return m_Path.c_str(); }

    const char * getModule() const { return m_Module.c_str(); }

    CShmem * getBlock() const { return m_Block; }

  private:

    std::string m_Path;
    
    std::string m_Module;
    
    CShmem * m_Block; 
    
    CSemlock * m_Lock;
    
    Logger * m_Logger;
};

class Logger {

  public:

    Logger(LogFile * file);
    ~Logger();

    static int32_t getToday();

    static size_t getBlockSize();

  public:

    void initialize(int32_t today = 0, bool isinit = true);

    ssize_t getFileSize();

    void getLogPath(char * path);

    void getIndexPath(char * path);

    void setDate(int32_t today);

    int32_t getDate() const { return m_Buffer->date; }

    int8_t getLevel() const { return m_Buffer->minlevel; }

    void setLevel(uint8_t level);

    int32_t getOffset() const { return m_Buffer->offsets; }

    void setSizeLimit( size_t size) { m_Buffer->sizelimit = size; }

  public:

    void open();

    void flush(bool sync = false);

    void append(const std::string & logline);

    void rotate();

  private:

    LogFile * m_LogFile;

    int32_t m_Fd;

    size_t m_Size;

    Buffer * m_Buffer;
  
};

}

#endif