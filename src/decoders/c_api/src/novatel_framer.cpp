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
// ! \file novatel_framer.cpp
// ===============================================================================

#include "decoders/c_api/api/novatel_framer.h"

#include <limits>

#include "decoders/novatel/api/framer.hpp"

using namespace novatel;

namespace {
edie::oem::Framer* cast(NovatelFramer* pclFramer_) { return reinterpret_cast<edie::oem::Framer*>(pclFramer_); }
} // namespace

bool NovatelFramerSetLoggerLevel(NovatelFramer* pclFramer_, uint32_t uiLogLevel_)
{
    return pclFramer_ && uiLogLevel_ >= spdlog::level::level_enum::trace && uiLogLevel_ < spdlog::level::level_enum::n_levels
           ? cast(pclFramer_)->SetLoggerLevel(static_cast<spdlog::level::level_enum>(uiLogLevel_)),
           true : false;
}

void NovatelFramerShutdownLogger(NovatelFramer* pclFramer_)
{
    if (pclFramer_) { cast(pclFramer_)->ShutdownLogger(); } // TODO: ShutdownLogger is static, this function signature should be changed
}

NovatelFramer* NovatelFramerInit() { return reinterpret_cast<NovatelFramer*>(new edie::oem::Framer()); }

void NovatelFramerDelete(NovatelFramer* pclFramer_) { delete cast(pclFramer_); }

void NovatelFramerFrameJson(NovatelFramer* pclFramer_, bool bFrameJson_)
{
    if (pclFramer_) { cast(pclFramer_)->SetFrameJson(bFrameJson_); }
}

void NovatelFramerPayloadOnly(NovatelFramer* pclFramer_, bool bPayloadOnly_)
{
    if (pclFramer_) { cast(pclFramer_)->SetPayloadOnly(bPayloadOnly_); }
}

void NovatelFramerReportUnknownBytes(NovatelFramer* pclFramer_, bool bReportUnknownBytes_)
{
    if (pclFramer_) { cast(pclFramer_)->SetReportUnknownBytes(bReportUnknownBytes_); }
}

uint32_t NovatelFramerGetAvailableBytes(NovatelFramer* pclFramer_)
{
    return pclFramer_ ? cast(pclFramer_)->GetBytesAvailableInBuffer() : std::numeric_limits<uint32_t>::max();
}

uint32_t NovatelFramerWrite(NovatelFramer* pclFramer_, unsigned char* pucBytes_, uint32_t uiByteCount_)
{
    return pclFramer_ ? cast(pclFramer_)->Write(pucBytes_, uiByteCount_) : std::numeric_limits<uint32_t>::max();
}

EDIE_STATUS NovatelFramerRead(NovatelFramer* pclFramer_, unsigned char* pucBuffer_, uint32_t uiBufferSize_, NovatelMetaDataStruct* pstMetaData_)
{
    return pclFramer_ && pstMetaData_
               ? static_cast<EDIE_STATUS>(
                     cast(pclFramer_)->GetFrame(pucBuffer_, uiBufferSize_, *reinterpret_cast<edie::oem::MetaDataStruct*>(pstMetaData_)))
               : EDIE_STATUS_NULL_PROVIDED;
}

uint32_t NovatelFramerFlush(NovatelFramer* pclFramer_, unsigned char* pucBuffer_, uint32_t uiBufferSize_)
{
    return pclFramer_ && pucBuffer_ ? cast(pclFramer_)->Flush(pucBuffer_, uiBufferSize_) : std::numeric_limits<uint32_t>::max();
}
