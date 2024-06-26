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
// ! \file novatel_header_decoder.cpp
// ===============================================================================

#include "decoders/c_api/api/novatel_header_decoder.h"

#include "decoders/novatel/api/header_decoder.hpp"

using namespace novatel;

namespace {
edie::oem::HeaderDecoder* cast(NovatelHeaderDecoder* pclHeaderDecoder_) { return reinterpret_cast<edie::oem::HeaderDecoder*>(pclHeaderDecoder_); }
} // namespace

bool NovatelHeaderDecoderSetLoggerLevel(NovatelHeaderDecoder* pclHeaderDecoder_, uint32_t uiLogLevel_)
{
    return pclHeaderDecoder_ && uiLogLevel_ >= spdlog::level::level_enum::trace && uiLogLevel_ < spdlog::level::level_enum::n_levels
           ? cast(pclHeaderDecoder_)->SetLoggerLevel(static_cast<spdlog::level::level_enum>(uiLogLevel_)),
           true : false;
}

void NovatelHeaderDecoderShutdownLogger(NovatelHeaderDecoder* pclHeaderDecoder_)
{
    if (pclHeaderDecoder_) { cast(pclHeaderDecoder_)->ShutdownLogger(); } // TODO: ShutdownLogger is static, this function signature should be changed
}

NovatelHeaderDecoder* NovatelHeaderDecoderInit(CommonJsonReader* pclJsonDb_)
{
    return reinterpret_cast<NovatelHeaderDecoder*>(new edie::oem::HeaderDecoder(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)));
}

void NovatelHeaderDecoderDelete(NovatelHeaderDecoder* pclHeaderDecoder_) { delete cast(pclHeaderDecoder_); }

void NovatelHeaderDecoderLoadJson(NovatelHeaderDecoder* pclHeaderDecoder_, CommonJsonReader* pclJsonDb_)
{
    if (pclHeaderDecoder_ && pclJsonDb_) { cast(pclHeaderDecoder_)->LoadJsonDb(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)); }
}

EDIE_STATUS NovatelHeaderDecoderDecode(NovatelHeaderDecoder* pclHeaderDecoder_, unsigned char* pucLogBuf_, NovatelIntermediateHeader* pstInterHeader_,
                                       NovatelMetaDataStruct* pstMetaData_)
{
    return pclHeaderDecoder_ && pucLogBuf_ && pstInterHeader_ && pstMetaData_
               ? static_cast<EDIE_STATUS>(cast(pclHeaderDecoder_)
                                              ->Decode(pucLogBuf_, *reinterpret_cast<edie::oem::IntermediateHeader*>(pstInterHeader_),
                                                       *reinterpret_cast<edie::oem::MetaDataStruct*>(pstMetaData_)))
               : EDIE_STATUS_NULL_PROVIDED;
}
