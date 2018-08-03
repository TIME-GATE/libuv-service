#ifndef __SRC_UTLIS_TIME_H__
#define __SRC_UTLIS_TIME_H__

#include<time.h>
#include<stdint.h>

#include "define.h"

namespace Utils {

class Clock {

  public : 
    // 绝对时间
    Clock(const char * now);

    // 相对时间
    Clock(time_t now, uint32_t interval);
  
  public :
    // 转换为时间戳
    time_t convert() const { return m_Timestamp; }
  
  private :
    time_t m_Timestamp;
};

class TimeUtils {

  public : 
    TimeUtils();
    TimeUtils(time_t t);
    TimeUtils(struct tm * tm);
  
  public :
    time_t getTimestamp();
    struct tm * getTimeStruct();
  
  public : 

    // 当前秒数
    static time_t time();
    
    // 当前毫秒数
    static int64_t now();
    
    // 毫秒
    static int32_t sleep(uint64_t mseconds);

    // 时间戳
    static time_t getTimestamp(const char * str);

  private : 
    time_t m_Timestamp;
    struct tm m_TimeStruct;
};

}

#endif