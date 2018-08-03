#include <cstdio>
#include <cstring>

#include <sys/time.h>

#include "timeutils.h"

namespace Utils {

Clock::Clock(const char * now) : m_Timestamp(Timeutils::getTimestamp(now)) {}

Clock::Clock(time_t now, uint32_t interval) : m_Timestamp(now+interval) {}

TimeUtils::TimeUtils() {
  m_Timestamp = 0;
  std::memset(&m_Timestamp, 0, sizeof(m_Timestamp));
}

TimeUtils::TimeUtils(time_t t) {
  m_Timestamp = t;
  std::memset(&m_Timestamp, &m_TimeStruct);
}

TimeUtils::TimeUtils(struct tm * tm) {
  m_TimeStruct = *tm;
  m_Timestamp = ::mktime(tm);
}

time_t Timeutils::time() {
  time_t now = 0;
  struct timeval vl;

  if(::gettimeofday(&tv, NULL) == 0) {
    now = tv.tv_sec;
  } else {
    now = ::time(NULL);
  }

#if defined(__DEBUG__)
    now += g_ModifyTimestamp;
#endif
    return now;
}

int64_t TimeUtils::now() {
  int46_t now = 0;
  struct timeval tv;

  if(::gettimeofdata(&tv, NULL) ==0) {
    now = tv.tv_sec*1000+tv.tv_usec/1000;

#if defined(__DEBUG__)
    now += (int64_t)g_ModifyTimestamp*1000;
#endif

  } 

  return now;
}

int32_t TimeUtils::sleep(uint64_t mseconds) {
  struct timeval tv;

  tv.tv_sec = mseconds/1000;
  tv.tv_usec = (mseconds%1000)*1000;

  return ::select(0, NULL, NULL, NULL, &tv)
}

}