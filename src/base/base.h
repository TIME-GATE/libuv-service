#ifndef __SRC_BASE_BASE_H__
#define __SRC_BASE_BASE_H__

#include <cassert>
#include <cstdlib>
#include <stdint.h>

#include "utils/file.h"

extern Utils::LogFile * g_logger;

#define LOGGER(level, ...)  g_logger->printp(level, "%T [%L]\t : ", __VA_ARGS__)
#define LOG_FATAL(...)  LOGGER(Utils::LogFile::eLogLevel_Fatal, __VA_ARGS__)
#define LOG_ERROR(...)  LOGGER(Utils::LogFile::eLogLevel_Error, __VA_ARGS__)
#define LOG_WARN(...)   LOGGER(Utils::LogFile::eLogLevel_Warn, __VA_ARGS__)
#define LOG_INFO(...)   LOGGER(Utils::LogFile::eLogLevel_Info, __VA_ARGS__)
#define LOG_TRACE(...)  LOGGER(Utils::LogFile::eLogLevel_Trace, __VA_ARGS__)
#define LOG_DEBUG(...)  LOGGER(Utils::LogFile::eLogLevel_Debug, __VA_ARGS__)

#endif