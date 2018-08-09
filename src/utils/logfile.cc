#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(__linux__)
#include <linux/limits.h>
#else
#include <limits.h>
#endif

#include "logfile.h"

namespace Utils {

ConfigSection::ConfigSection(char * section, int32_t length) {
  m_AllItems.clear();
  m_Section.assign(section, length);
}

ConfigSection::~ConfigSection() {
  m_AllItems.clear();
}

bool ConfigSection::addItem(char * key, int32_t nkey, char * value, int32_t, int32_t nvalue) {
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

const char * ConfigSection::getValue(const caht * key) {
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

}