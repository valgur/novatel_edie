// ===============================================================================
// |                                                                             |
// |  COPYRIGHT NovAtel Inc, 2022. All rights reserved.                          |
// |                                                                             |
// |  Permission is hereby granted, free of charge, to any person obtaining a    |
// |  copy of this software and associated documentation files (the "Software"), |
// |  to deal in the Software without restriction, including without limitation  |
// |  the rights to use, copy, modify, merge, publish, distribute, sublicense,   |
// |  and/or sell copies of the Software, and to permit persons to whom the      |
// |  Software is furnished to do so, subject to the following conditions:       |
// |                                                                             |
// |  The above copyright notice and this permission notice shall be included    |
// |  in all copies or substantial portions of the Software.                     |
// |                                                                             |
// |  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR |
// |  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   |
// |  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    |
// |  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER |
// |  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    |
// |  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        |
// |  DEALINGS IN THE SOFTWARE.                                                  |
// |                                                                             |
// ===============================================================================
// ! \file common_logger.cpp
// ===============================================================================

#include "decoders/c_api/api/common_logger.h"

#include <array>
#include <cstdarg>
#include <string>

#include "decoders/common/api/logger.hpp"

#ifndef COMMON_LOGGER_BUF_SIZE
#define COMMON_LOGGER_BUF_SIZE 5000
#endif

void CommonLoggerSetup() { Logger::InitLogger(); }

void CommonLoggerSetupFromFile(const char* pcLoggerConfigPath_) { Logger::InitLogger(std::string(pcLoggerConfigPath_)); }

bool CommonLoggerSetLoggerLevel(int32_t iLogLevel_)
{
    return iLogLevel_ >= spdlog::level::level_enum::trace && iLogLevel_ < spdlog::level::level_enum::n_levels
           ? spdlog::set_level(static_cast<spdlog::level::level_enum>(iLogLevel_)),
           true : false;
}

void CommonLoggerShutdownLogger() { Logger::Shutdown(); }

bool CommonLoggerLogUnformatted(int32_t iLogLevel_, const char* pucMessage_)
{
    std::shared_ptr<spdlog::logger> pclLogger = spdlog::get("logger_");

    if (!pclLogger) { pclLogger = Logger::RegisterLogger("decoder_dll"); }

    if (iLogLevel_ >= spdlog::level::level_enum::trace && iLogLevel_ < spdlog::level::level_enum::n_levels)
    {
        pclLogger->log(static_cast<spdlog::level::level_enum>(iLogLevel_), pucMessage_);
        return true;
    }
    return false;
}

#define format_args()                                                                                                                                \
    std::array<char, COMMON_LOGGER_BUF_SIZE> formatted;                                                                                              \
    va_list args;                                                                                                                                    \
    va_start(args, pucMessage_);                                                                                                                     \
    vsnprintf(formatted.data(), COMMON_LOGGER_BUF_SIZE, pucMessage_, args);                                                                          \
    va_end(args);

bool CommonLoggerLog(int32_t iLogLevel_, const char* pucMessage_, ...)
{
    format_args();
    return CommonLoggerLogUnformatted(iLogLevel_, formatted.data());
}

bool CommonLoggerLogTrace(const char* pucMessage_, ...)
{
    format_args();
    return CommonLoggerLogUnformatted(spdlog::level::level_enum::trace, formatted.data());
}
bool CommonLoggerLogDebug(const char* pucMessage_, ...)
{
    format_args();
    return CommonLoggerLogUnformatted(spdlog::level::level_enum::debug, formatted.data());
}
bool CommonLoggerLogInfo(const char* pucMessage_, ...)
{
    format_args();
    return CommonLoggerLogUnformatted(spdlog::level::level_enum::info, formatted.data());
}
bool CommonLoggerLogWarn(const char* pucMessage_, ...)
{
    format_args();
    return CommonLoggerLogUnformatted(spdlog::level::level_enum::warn, formatted.data());
}
bool CommonLoggerLogError(const char* pucMessage_, ...)
{
    format_args();
    return CommonLoggerLogUnformatted(spdlog::level::level_enum::err, formatted.data());
}
bool CommonLoggerLogCritical(const char* pucMessage_, ...)
{
    format_args();
    return CommonLoggerLogUnformatted(spdlog::level::level_enum::critical, formatted.data());
}