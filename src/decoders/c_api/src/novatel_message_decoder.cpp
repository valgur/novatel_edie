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
// ! \file novatel_message_decoder.cpp
// ===============================================================================

#include "decoders/c_api/api/novatel_message_decoder.h"

#include "decoders/novatel/api/common.hpp"
#include "decoders/novatel/api/message_decoder.hpp"

using namespace novatel;

namespace {
edie::oem::MessageDecoder* cast(NovatelMessageDecoder* pclMessageDecoder_)
{
    return reinterpret_cast<edie::oem::MessageDecoder*>(pclMessageDecoder_);
}
} // namespace

bool NovatelMessageDecoderSetLoggerLevel(NovatelMessageDecoder* pclMessageDecoder_, uint32_t uiLogLevel_)
{
    return pclMessageDecoder_ && uiLogLevel_ >= spdlog::level::level_enum::trace && uiLogLevel_ < spdlog::level::level_enum::n_levels
           ? cast(pclMessageDecoder_)->SetLoggerLevel(static_cast<spdlog::level::level_enum>(uiLogLevel_)),
           true : false;
}

void NovatelMessageDecoderShutdownLogger(NovatelMessageDecoder* pclMessageDecoder_)
{
    if (pclMessageDecoder_) { cast(pclMessageDecoder_)->ShutdownLogger(); }
}

NovatelMessageDecoder* NovatelMessageDecoderInit(CommonJsonReader* pclJsonDb_)
{
    return reinterpret_cast<NovatelMessageDecoder*>(new edie::oem::MessageDecoder(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)));
}

void NovatelMessageDecoderDelete(NovatelMessageDecoder* pclMessageDecoder_) { delete cast(pclMessageDecoder_); }

void NovatelMessageDecoderLoadJson(NovatelMessageDecoder* pclMessageDecoder_, CommonJsonReader* pclJsonDb_)
{
    if (pclMessageDecoder_ && pclJsonDb_) { cast(pclMessageDecoder_)->LoadJsonDb(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)); }
}

EDIE_STATUS NovatelMessageDecoderDecode(NovatelMessageDecoder* pclMessageDecoder_, unsigned char* pucLogBuf_,
                                        NovatelFieldContainerVector* pstInterMessage_, NovatelMetaDataStruct* pstMetaData_)
{
    return pclMessageDecoder_ && pstInterMessage_ && pstMetaData_
               ? static_cast<EDIE_STATUS>(cast(pclMessageDecoder_)
                                              ->Decode(pucLogBuf_, *reinterpret_cast<std::vector<edie::FieldContainer>*>(pstInterMessage_),
                                                       *reinterpret_cast<edie::oem::MetaDataStruct*>(pstMetaData_)))
               : EDIE_STATUS_NULL_PROVIDED;
}

NovatelFieldContainerVector* NovatelIntermediateMessageInit()
{
    return reinterpret_cast<NovatelFieldContainerVector*>(new std::vector<edie::FieldContainer>);
}

void NovatelIntermediateMessageDelete(NovatelFieldContainerVector* pstInterMessage_)
{
    delete reinterpret_cast<std::vector<edie::FieldContainer>*>(pstInterMessage_);
}
