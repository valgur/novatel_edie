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
// ! \file novatel_parser.h
// ===============================================================================

#ifndef C_API_NOVATEL_PARSER_H
#define C_API_NOVATEL_PARSER_H

#include "decoders/c_api/api/common.h"
#include "decoders/c_api/api/common_json_reader.h"
#include "decoders/c_api/api/decoders_export.h"
#include "decoders/c_api/api/novatel_common.h"
#include "decoders/c_api/api/novatel_filter.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct NovatelParser NovatelParser;

    // Construct/Destruct
    DECODERS_EXPORT NovatelParser* NovatelParserInit(CommonJsonReader* pclJsonDb_);
    DECODERS_EXPORT void NovatelParserDelete(NovatelParser* pclParser_);

    // Config
    DECODERS_EXPORT void NovatelParserLoadJsonDb(NovatelParser* pclParser_, CommonJsonReader* pclJsonDb_);

    DECODERS_EXPORT void NovatelParserSetIgnoreAbbrevAsciiResponses(NovatelParser* pclParser_, bool bIgnoreAbbrevAsciiResponsesCmp_);
    DECODERS_EXPORT bool NovatelParserGetIgnoreAbbrevAsciiResponses(NovatelParser* pclParser_);
    DECODERS_EXPORT void NovatelParserSetDecompressRangeCmp(NovatelParser* pclParser_, bool bDecompressRangeCmp_);
    DECODERS_EXPORT bool NovatelParserGetDecompressRangeCmp(NovatelParser* pclParser_);
    DECODERS_EXPORT void NovatelParserSetReturnUnknownBytes(NovatelParser* pclParser_, bool bReturnUnknownBytes_);
    DECODERS_EXPORT bool NovatelParserGetReturnUnknownBytes(NovatelParser* pclParser_);
    DECODERS_EXPORT void NovatelParserSetEncodeFormat(NovatelParser* pclParser_, EDIE_ENCODE_FORMAT eEncodeFormat_);
    DECODERS_EXPORT EDIE_ENCODE_FORMAT NovatelParserGetEncodeFormat(NovatelParser* pclParser_);

    DECODERS_EXPORT NovatelFilter* NovatelParserGetFilter(NovatelParser* pclParser_);
    DECODERS_EXPORT void NovatelParserSetFilter(NovatelParser* pclParser_, NovatelFilter* pclFilter_);

    DECODERS_EXPORT unsigned char* NovatelParserGetBuffer(NovatelParser* pclParser_);

    // R/W
    DECODERS_EXPORT uint32_t NovatelParserWrite(NovatelParser* pclParser_, unsigned char* pucBytes_, uint32_t uiByteCount_);
    DECODERS_EXPORT EDIE_STATUS NovatelParserRead(NovatelParser* pclParser_, NovatelMessageDataStruct* pstMessageData_,
                                                  NovatelMetaDataStruct* pstMetaData_);

    DECODERS_EXPORT uint32_t NovatelParserFlush(NovatelParser* pclParser_, unsigned char* pucBuffer_, uint32_t uiBufferSize_);

#ifdef __cplusplus
}
#endif

#endif // C_API_NOVATEL_PARSER_H
