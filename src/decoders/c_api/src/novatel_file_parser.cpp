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
// ! \file novatel_file_parser.cpp
// ===============================================================================

#define EDIE_DONT_REDEFINE_ENUMS
#include "decoders/c_api/api/novatel_file_parser.h"
#undef EDIE_DONT_REDEFINE_ENUMS

#include <limits>

#include "decoders/novatel/api/file_parser.hpp"

using namespace novatel;

namespace {
edie::oem::FileParser* cast(NovatelFileParser* pclFileParser_) { return reinterpret_cast<edie::oem::FileParser*>(pclFileParser_); }
} // namespace

NovatelFileParser* NovatelFileParserInit(CommonJsonReader* pclJsonDb_)
{
    return reinterpret_cast<NovatelFileParser*>(new edie::oem::FileParser(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)));
}

void NovatelFileParserDelete(NovatelFileParser* pclFileParser_) { delete cast(pclFileParser_); }

void NovatelFileParserLoadJsonDb(NovatelFileParser* pclFileParser_, CommonJsonReader* pclJsonDb_)
{
    if (pclFileParser_ && pclJsonDb_) { cast(pclFileParser_)->LoadJsonDb(reinterpret_cast<edie::JsonReader*>(pclJsonDb_)); }
}

void NovatelFileParserSetIgnoreAbbrevAsciiResponses(NovatelFileParser* pclFileParser_, bool bIgnoreAbbrevAsciiResponsesCmp_)
{
    if (pclFileParser_) { cast(pclFileParser_)->SetIgnoreAbbreviatedAsciiResponses(bIgnoreAbbrevAsciiResponsesCmp_); }
}

bool NovatelFileParserGetIgnoreAbbrevAsciiResponses(NovatelFileParser* pclFileParser_)
{
    return pclFileParser_ ? cast(pclFileParser_)->GetIgnoreAbbreviatedAsciiResponses() : false;
}

void NovatelFileParserSetDecompressRangeCmp(NovatelFileParser* pclFileParser_, bool bDecompressRangeCmp_)
{
    if (pclFileParser_) { cast(pclFileParser_)->SetDecompressRangeCmp(bDecompressRangeCmp_); }
}

bool NovatelFileParserGetDecompressRangeCmp(NovatelFileParser* pclFileParser_)
{
    return pclFileParser_ ? cast(pclFileParser_)->GetDecompressRangeCmp() : false;
}

void NovatelFileParserSetReturnUnknownBytes(NovatelFileParser* pclFileParser_, bool bReturnUnknownBytes_)
{
    if (pclFileParser_) { cast(pclFileParser_)->SetReturnUnknownBytes(bReturnUnknownBytes_); }
}

bool NovatelFileParserGetReturnUnknownBytes(NovatelFileParser* pclFileParser_)
{
    return pclFileParser_ ? cast(pclFileParser_)->GetReturnUnknownBytes() : false;
}

void NovatelFileParserSetEncodeFormat(NovatelFileParser* pclFileParser_, EDIE_ENCODE_FORMAT eEncodeFormat_)
{
    if (pclFileParser_) { cast(pclFileParser_)->SetEncodeFormat(static_cast<edie::ENCODE_FORMAT>(eEncodeFormat_)); }
}

EDIE_ENCODE_FORMAT NovatelFileParserGetEncodeFormat(NovatelFileParser* pclFileParser_)
{
    return pclFileParser_ ? static_cast<EDIE_ENCODE_FORMAT>(cast(pclFileParser_)->GetEncodeFormat()) : EDIE_ENCODE_FORMAT_UNSPECIFIED;
}

unsigned char* NovatelFileParserGetBuffer(NovatelFileParser* pclFileParser_)
{
    return pclFileParser_ ? cast(pclFileParser_)->GetInternalBuffer() : nullptr;
}

NovatelFilter* NovatelFileParserGetFilter(NovatelFileParser* pclFileParser_)
{
    return reinterpret_cast<edie::oem::FileParser*>(pclFileParser_) ? reinterpret_cast<NovatelFilter*>(cast(pclFileParser_)->GetFilter()) : nullptr;
}

void NovatelFileParserSetFilter(NovatelFileParser* pclFileParser_, NovatelFilter* pclFilter_)
{
    if (pclFileParser_ && pclFilter_) { cast(pclFileParser_)->SetFilter(reinterpret_cast<edie::oem::Filter*>(pclFilter_)); }
}

bool NovatelFileParserSetStream(NovatelFileParser* pclFileParser_, InputFileStream* pclIfs_)
{
    return pclFileParser_ && pclIfs_ ? cast(pclFileParser_)->SetStream(pclIfs_) : false;
}

uint32_t NovatelFileParserGetPercentRead(NovatelFileParser* pclFileParser_)
{
    return pclFileParser_ ? cast(pclFileParser_)->GetPercentRead() : std::numeric_limits<uint32_t>::max();
}

EDIE_STATUS NovatelFileParserRead(NovatelFileParser* pclFileParser_, NovatelMessageDataStruct* pstMessageData_, NovatelMetaDataStruct* pstMetaData_)
{
    return pclFileParser_ && pstMessageData_ && pstMetaData_
               ? static_cast<EDIE_STATUS>(cast(pclFileParser_)
                                              ->Read(*reinterpret_cast<edie::MessageDataStruct*>(pstMessageData_),
                                                     *reinterpret_cast<edie::oem::MetaDataStruct*>(pstMetaData_)))
               : EDIE_STATUS_NULL_PROVIDED;
}

bool NovatelFileParserReset(NovatelFileParser* pclFileParser_) { return pclFileParser_ ? cast(pclFileParser_)->Reset() : false; }

uint32_t NovatelFileParserFlush(NovatelFileParser* pclFileParser_, unsigned char* pucBuffer_, uint32_t uiBufferSize_)
{
    return pclFileParser_ && pucBuffer_ ? cast(pclFileParser_)->Flush(pucBuffer_, uiBufferSize_) : std::numeric_limits<uint32_t>::max();
}
