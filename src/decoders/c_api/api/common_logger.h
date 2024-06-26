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
// ! \file common_logger.h
// ===============================================================================

#ifndef C_API_LOGGER_H
#define C_API_LOGGER_H

#include <cstdint>

#include "decoders/c_api/api/decoders_export.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef SPDLOG_LEVEL_TRACE
#define SPDLOG_LEVEL_TRACE 0
#define SPDLOG_LEVEL_DEBUG 1
#define SPDLOG_LEVEL_INFO 2
#define SPDLOG_LEVEL_WARN 3
#define SPDLOG_LEVEL_ERROR 4
#define SPDLOG_LEVEL_CRITICAL 5
#define SPDLOG_LEVEL_OFF 6
#endif

    DECODERS_EXPORT void CommonLoggerSetup();
    DECODERS_EXPORT void CommonLoggerSetupFromFile(const char* pcLoggerConfigPath_);
    DECODERS_EXPORT bool CommonLoggerSetLoggerLevel(int32_t iLogLevel_);
    DECODERS_EXPORT void CommonLoggerShutdownLogger();
    DECODERS_EXPORT bool CommonLoggerLogUnformatted(int32_t iLogLevel_, const char* pucMessage_);
    DECODERS_EXPORT bool CommonLoggerLog(int32_t iLogLevel_, const char* pucMessage_, ...);
    DECODERS_EXPORT bool CommonLoggerLogTrace(const char* pucMessage_, ...);
    DECODERS_EXPORT bool CommonLoggerLogDebug(const char* pucMessage_, ...);
    DECODERS_EXPORT bool CommonLoggerLogInfo(const char* pucMessage_, ...);
    DECODERS_EXPORT bool CommonLoggerLogWarn(const char* pucMessage_, ...);
    DECODERS_EXPORT bool CommonLoggerLogError(const char* pucMessage_, ...);
    DECODERS_EXPORT bool CommonLoggerLogCritical(const char* pucMessage_, ...);

#ifdef __cplusplus
}
#endif

#endif // C_API_LOGGER_H
