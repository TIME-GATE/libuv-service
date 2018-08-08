#ifndef __SRC_UTILS_FILE_H__
#define __SRC_UTILS_FILE_H__

#include <map>
#include <string>

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

namespace Utils {

class ConfigSection;
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

  private:
    void parse(char * filecontent, int32_t filesize);

    const char * getValue(const char * section, const char * key);

  private:
    typedef std::map<std::string, ConfigSection *> SectionMap;
    std::string m_File;
    SectionMap m_Sections;
    
};


class Logger;
class CShmem;
class CSemlock;

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

}

#endif