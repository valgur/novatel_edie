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
// ! \file novatel_header_decoder.h
// ===============================================================================

#ifndef C_API_NOVATEL_HEADER_DECODER_H
#define C_API_NOVATEL_HEADER_DECODER_H

#include <stdint.h>

#include "decoders/c_api/api/common.h"
#include "decoders/c_api/api/common_json_reader.h"
#include "decoders/c_api/api/decoders_export.h"
#include "decoders/c_api/api/novatel_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct NovatelHeaderDecoder NovatelHeaderDecoder;

    // Logger
    DECODERS_EXPORT bool NovatelHeaderDecoderSetLoggerLevel(NovatelHeaderDecoder* pclHeaderDecoder_, uint32_t iLogLevel_);
    DECODERS_EXPORT void NovatelHeaderDecoderShutdownLogger(NovatelHeaderDecoder* pclHeaderDecoder_);

    // Construct/Destruct
    DECODERS_EXPORT NovatelHeaderDecoder* NovatelHeaderDecoderInit(CommonJsonReader* pclJsonDb_);
    DECODERS_EXPORT void NovatelHeaderDecoderDelete(NovatelHeaderDecoder* pclHeaderDecoder_);

    // Config
    DECODERS_EXPORT void NovatelHeaderDecoderLoadJson(NovatelHeaderDecoder* pclHeaderDecoder_, CommonJsonReader* pclJsonDb_);

    // R/W
    DECODERS_EXPORT EDIE_STATUS NovatelHeaderDecoderDecode(NovatelHeaderDecoder* pclHeaderDecoder_, unsigned char* pucLogBuf_,
                                                           NovatelIntermediateHeader* pstInterHeader_, NovatelMetaDataStruct* pstMetaData_);
#ifdef __cplusplus
}
#endif

#endif // C_API_NOVATEL_HEADER_DECODER_H
