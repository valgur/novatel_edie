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
// ! \file novatel_parser.cpp
// ===============================================================================

#include "decoders/c_api/api/novatel_parser.h"

#include <limits>

#include "decoders/novatel/api/common.hpp"
#include "decoders/novatel/api/filter.hpp"
#include "decoders/novatel/api/parser.hpp"

using namespace novatel;

namespace {
edie::oem::Parser* cast(NovatelParser* pclParser_) { return reinterpret_cast<edie::oem::Parser*>(pclParser_); }
} // namespace

NovatelParser* NovatelParserInit(CommonJsonReader* pclJsonDb_)
{
    return reinterpret_cast<NovatelParser*>(new edie::oem::Parser(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)));
}

void NovatelParserDelete(NovatelParser* pclParser_) { delete cast(pclParser_); }

void NovatelParserLoadJsonDb(NovatelParser* pclParser_, CommonJsonReader* pclJsonDb_)
{
    if (pclParser_ && pclJsonDb_) { cast(pclParser_)->LoadJsonDb(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)); }
}

void NovatelParserSetIgnoreAbbrevAsciiResponses(NovatelParser* pclParser_, bool bIgnoreAbbrevAsciiResponsesCmp_)
{
    if (pclParser_) { cast(pclParser_)->SetIgnoreAbbreviatedAsciiResponses(bIgnoreAbbrevAsciiResponsesCmp_); }
}

bool NovatelParserGetIgnoreAbbrevAsciiResponses(NovatelParser* pclParser_)
{
    return pclParser_ ? cast(pclParser_)->GetIgnoreAbbreviatedAsciiResponses() : false;
}

void NovatelParserSetDecompressRangeCmp(NovatelParser* pclParser_, bool bDecompressRangeCmp_)
{
    if (pclParser_) { cast(pclParser_)->SetDecompressRangeCmp(bDecompressRangeCmp_); }
}

bool NovatelParserGetDecompressRangeCmp(NovatelParser* pclParser_) { return pclParser_ ? cast(pclParser_)->GetDecompressRangeCmp() : false; }

void NovatelParserSetReturnUnknownBytes(NovatelParser* pclParser_, bool bReturnUnknownBytes_)
{
    if (pclParser_) { cast(pclParser_)->SetReturnUnknownBytes(bReturnUnknownBytes_); }
}

bool NovatelParserGetReturnUnknownBytes(NovatelParser* pclParser_) { return pclParser_ ? cast(pclParser_)->GetReturnUnknownBytes() : false; }

void NovatelParserSetEncodeFormat(NovatelParser* pclParser_, EDIE_ENCODE_FORMAT eEncodeFormat_)
{
    if (pclParser_) { cast(pclParser_)->SetEncodeFormat(static_cast<edie::ENCODE_FORMAT>(eEncodeFormat_)); }
}

EDIE_ENCODE_FORMAT NovatelParserGetEncodeFormat(NovatelParser* pclParser_)
{
    return pclParser_ ? static_cast<EDIE_ENCODE_FORMAT>(cast(pclParser_)->GetEncodeFormat()) : EDIE_ENCODE_FORMAT_UNSPECIFIED;
}

NovatelFilter* NovatelParserGetFilter(NovatelParser* pclParser_)
{
    return pclParser_ ? reinterpret_cast<NovatelFilter*>(cast(pclParser_)->GetFilter()) : nullptr;
}

void NovatelParserSetFilter(NovatelParser* pclParser_, NovatelFilter* pclFilter_)
{
    if (pclParser_ && pclFilter_) { cast(pclParser_)->SetFilter(reinterpret_cast<edie::oem::Filter*>(pclFilter_)); }
}

unsigned char* NovatelParserGetBuffer(NovatelParser* pclParser_) { return pclParser_ ? cast(pclParser_)->GetInternalBuffer() : nullptr; }

uint32_t NovatelParserWrite(NovatelParser* pclParser_, unsigned char* pucBytes_, uint32_t uiByteCount_)
{
    return pclParser_ && pucBytes_ ? cast(pclParser_)->Write(pucBytes_, uiByteCount_) : std::numeric_limits<uint32_t>::max();
}

EDIE_STATUS NovatelParserRead(NovatelParser* pclParser_, NovatelMessageDataStruct* pstMessageData_, NovatelMetaDataStruct* pstMetaData_)
{
    return pclParser_ && pstMessageData_ && pstMetaData_
               ? static_cast<EDIE_STATUS>(cast(pclParser_)
                                              ->Read(*reinterpret_cast<edie::MessageDataStruct*>(pstMessageData_),
                                                     *reinterpret_cast<edie::oem::MetaDataStruct*>(pstMetaData_)))
               : EDIE_STATUS_NULL_PROVIDED;
}

uint32_t NovatelParserFlush(NovatelParser* pclParser_, unsigned char* pucBuffer_, uint32_t uiBufferSize_)
{
    return pclParser_ && pucBuffer_ ? cast(pclParser_)->Flush(pucBuffer_, uiBufferSize_) : std::numeric_limits<uint32_t>::max();
}
