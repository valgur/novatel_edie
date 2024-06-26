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
// ! \file novatel_commander.cpp
// ===============================================================================

#include "decoders/c_api/api/novatel_commander.h"

#include "decoders/novatel/api/commander.hpp"

using namespace novatel;

bool NovatelCommanderSetLoggerLevel(NovatelCommander* pclCommander_, uint32_t uiLogLevel_)
{
    return pclCommander_ && uiLogLevel_ >= spdlog::level::level_enum::trace && uiLogLevel_ < spdlog::level::level_enum::n_levels
           ? reinterpret_cast<edie::oem::Commander*>(pclCommander_)->SetLoggerLevel(static_cast<spdlog::level::level_enum>(uiLogLevel_)),
           true : false;
}

void NovatelCommanderShutdownLogger(NovatelCommander* pclCommander_)
{
    if (pclCommander_)
    {
        reinterpret_cast<edie::oem::Commander*>(pclCommander_)->ShutdownLogger();
    } // TODO: ShutdownLogger is static, this function signature should be changed
}

NovatelCommander* NovatelCommanderInit(CommonJsonReader* pclJsonDb_)
{
    return reinterpret_cast<NovatelCommander*>(new edie::oem::Commander(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)));
}

void NovatelCommanderDelete(NovatelCommander* pclCommander_) { delete reinterpret_cast<edie::oem::Commander*>(pclCommander_); }

void NovatelCommanderLoadJson(NovatelCommander* pclCommander_, CommonJsonReader* pclJsonDb_)
{
    if (pclCommander_ && pclJsonDb_)
    {
        reinterpret_cast<edie::oem::Commander*>(pclCommander_)->LoadJsonDb(reinterpret_cast<edie::JsonReader*>(pclJsonDb_));
    }
}

EDIE_STATUS NovatelCommanderEncode(NovatelCommander* pclCommander_, char* pcAbbrevAsciiCommand_, uint32_t uicAbbrevAsciiCommandLength_,
                                   char* pcEncodeBuffer_, uint32_t* puiEncodeBufferSize_, EDIE_ENCODE_FORMAT eEncodeFormat_)
{
    return pclCommander_ && pcAbbrevAsciiCommand_ && pcEncodeBuffer_ && puiEncodeBufferSize_
               ? static_cast<EDIE_STATUS>(reinterpret_cast<edie::oem::Commander*>(pclCommander_)
                                              ->Encode(pcAbbrevAsciiCommand_, uicAbbrevAsciiCommandLength_, pcEncodeBuffer_, *puiEncodeBufferSize_,
                                                       static_cast<edie::ENCODE_FORMAT>(eEncodeFormat_)))
               : EDIE_STATUS_NULL_PROVIDED;
}
