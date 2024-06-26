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
// ! \file novatel_encoder.cpp
// ===============================================================================

#include "decoders/c_api/api/novatel_encoder.h"

#include "decoders/novatel/api/encoder.hpp"

using namespace novatel;

namespace {
edie::oem::Encoder* cast(NovatelEncoder* pclEncoder_) { return reinterpret_cast<edie::oem::Encoder*>(pclEncoder_); }
} // namespace

bool NovatelEncoderSetLoggerLevel(NovatelEncoder* pclEncoder_, uint32_t uiLogLevel_)
{
    return pclEncoder_ && uiLogLevel_ >= spdlog::level::level_enum::trace && uiLogLevel_ < spdlog::level::level_enum::n_levels
           ? cast(pclEncoder_)->SetLoggerLevel(static_cast<spdlog::level::level_enum>(uiLogLevel_)),
           true : false;
}

void NovatelEncoderShutdownLogger(NovatelEncoder* pclEncoder_)
{
    if (pclEncoder_) { cast(pclEncoder_)->ShutdownLogger(); } // TODO: ShutdownLogger is static, this function signature should be changed
}

NovatelEncoder* NovatelEncoderInit(CommonJsonReader* pclJsonDb_)
{
    return reinterpret_cast<NovatelEncoder*>(new edie::oem::Encoder(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)));
}

void NovatelEncoderDelete(NovatelEncoder* pclEncoder_) { delete cast(pclEncoder_); }

void NovatelEncoderLoadJson(NovatelEncoder* pclEncoder_, CommonJsonReader* pclJsonDb_)
{
    if (pclEncoder_ && pclJsonDb_) { cast(pclEncoder_)->LoadJsonDb(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)); }
}

EDIE_STATUS NovatelEncoderEncode(NovatelEncoder* pclEncoder_, unsigned char* pucEncodeBuffer_, uint32_t uiEncodeBufferSize_,
                                 NovatelIntermediateHeader* pstInterHeader_, NovatelFieldContainerVector* pstInterMessage_,
                                 NovatelMessageDataStruct* pstMessageData_, NovatelMetaDataStruct* pstMetaData_, EDIE_ENCODE_FORMAT eEncodeFormat_)
{
    return pclEncoder_ && pucEncodeBuffer_ && pstInterHeader_ && pstInterMessage_ && pstMessageData_ && pstMetaData_
               ? static_cast<EDIE_STATUS>(cast(pclEncoder_)
                                              ->Encode(&pucEncodeBuffer_,                                                       //
                                                       uiEncodeBufferSize_,                                                     //
                                                       *reinterpret_cast<edie::oem::IntermediateHeader*>(pstInterHeader_),      //
                                                       *reinterpret_cast<std::vector<edie::FieldContainer>*>(pstInterMessage_), //
                                                       *reinterpret_cast<edie::MessageDataStruct*>(pstMessageData_),            //
                                                       *reinterpret_cast<edie::oem::MetaDataStruct*>(pstMetaData_),             //
                                                       static_cast<edie::ENCODE_FORMAT>(eEncodeFormat_)))
               : EDIE_STATUS_NULL_PROVIDED;
}
