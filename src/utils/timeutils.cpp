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
}

}