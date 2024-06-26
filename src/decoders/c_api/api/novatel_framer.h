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
// ! \file novatel_framer.h
// ===============================================================================

#ifndef C_API_NOVATEL_FRAMER_H
#define C_API_NOVATEL_FRAMER_H

#include "decoders/c_api/api/decoders_export.h"
#include "decoders/c_api/api/novatel_filter.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct NovatelFramer NovatelFramer;

    // Logger
    DECODERS_EXPORT bool NovatelFramerSetLoggerLevel(NovatelFramer* pclFramer_, uint32_t uiLogLevel_);
    DECODERS_EXPORT void NovatelFramerShutdownLogger(NovatelFramer* pclFramer_);

    // Construct/Destruct
    DECODERS_EXPORT NovatelFramer* NovatelFramerInit();
    DECODERS_EXPORT void NovatelFramerDelete(NovatelFramer* pclFramer_);

    // Config
    DECODERS_EXPORT void NovatelFramerFrameJson(NovatelFramer* pclFramer_, bool bFrameJson_);
    DECODERS_EXPORT void NovatelFramerPayloadOnly(NovatelFramer* pclFramer_, bool bPayloadOnly_);
    DECODERS_EXPORT void NovatelFramerReportUnknownBytes(NovatelFramer* pclFramer_, bool bReportUnknownBytes_);

    // R/W
    DECODERS_EXPORT uint32_t NovatelFramerGetAvailableBytes(NovatelFramer* pclFramer_);
    DECODERS_EXPORT uint32_t NovatelFramerWrite(NovatelFramer* pclFramer_, unsigned char* pucBytes_, uint32_t uiByteCount_);
    DECODERS_EXPORT EDIE_STATUS NovatelFramerRead(NovatelFramer* pclFramer_, unsigned char* pucBuffer_, uint32_t uiBufferSize_,
                                                  NovatelMetaDataStruct* pstMetaData_);
    DECODERS_EXPORT uint32_t NovatelFramerFlush(NovatelFramer* pclFramer_, unsigned char* pucBuffer_, uint32_t uiBufferSize_);

#ifdef __cplusplus
}
#endif

#endif // C_API_NOVATEL_FRAMER_H
