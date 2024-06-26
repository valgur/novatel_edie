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
// ! \file novatel_file_parser.h
// ===============================================================================

#ifndef C_API_NOVATEL_FILE_PARSER_H
#define C_API_NOVATEL_FILE_PARSER_H

#include "decoders/c_api/api/common.h"
#include "decoders/c_api/api/common_json_reader.h"
#include "decoders/c_api/api/decoders_export.h"
#include "decoders/c_api/api/novatel_filter.h"
#include "hw_interface/c_api/api/stream_interface.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct NovatelFileParser NovatelFileParser;

    // Construct/Destruct
    DECODERS_EXPORT NovatelFileParser* NovatelFileParserInit(CommonJsonReader* pclJsonDb_);
    DECODERS_EXPORT void NovatelFileParserDelete(NovatelFileParser* pclFileParser_);

    // Config
    DECODERS_EXPORT void NovatelFileParserLoadJsonDb(NovatelFileParser* pclFileParser_, CommonJsonReader* pclJsonDb_);

    DECODERS_EXPORT void NovatelFileParserSetIgnoreAbbrevAsciiResponses(NovatelFileParser* pclFileParser_, bool bIgnoreAbbrevAsciiResponsesCmp_);
    DECODERS_EXPORT bool NovatelFileParserGetIgnoreAbbrevAsciiResponses(NovatelFileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetDecompressRangeCmp(NovatelFileParser* pclFileParser_, bool bDecompressRangeCmp_);
    DECODERS_EXPORT bool NovatelFileParserGetDecompressRangeCmp(NovatelFileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetReturnUnknownBytes(NovatelFileParser* pclFileParser_, bool bReturnUnknownBytes_);
    DECODERS_EXPORT bool NovatelFileParserGetReturnUnknownBytes(NovatelFileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetEncodeFormat(NovatelFileParser* pclFileParser_, EDIE_ENCODE_FORMAT eEncodeFormat_);
    DECODERS_EXPORT EDIE_ENCODE_FORMAT NovatelFileParserGetEncodeFormat(NovatelFileParser* pclFileParser_);

    DECODERS_EXPORT NovatelFilter* NovatelFileParserGetFilter(NovatelFileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetFilter(NovatelFileParser* pclFileParser_, NovatelFilter* pclFilter_);

    DECODERS_EXPORT unsigned char* NovatelFileParserGetBuffer(NovatelFileParser* pclFileParser_);

    // Stream, R/W
    DECODERS_EXPORT bool NovatelFileParserSetStream(NovatelFileParser* pclFileParser_, InputFileStream* pclIfs_);
    DECODERS_EXPORT uint32_t NovatelFileParserGetPercentRead(NovatelFileParser* pclFileParser_);

    // R/W
    DECODERS_EXPORT EDIE_STATUS NovatelFileParserRead(NovatelFileParser* pclFileParser_, NovatelMessageDataStruct* pstMessageData_,
                                                      NovatelMetaDataStruct* pstMetaData_);
    DECODERS_EXPORT bool NovatelFileParserReset(NovatelFileParser* pclFileParser_);

    DECODERS_EXPORT uint32_t NovatelFileParserFlush(NovatelFileParser* pclFileParser_, unsigned char* pucBuffer_, uint32_t uiBufferSize_);

#ifdef __cplusplus
}
#endif

#endif // C_API_NOVATEL_FILE_PARSER_H
