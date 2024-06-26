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
// ! \file novatel_framer.hpp
// ===============================================================================

#ifndef DYNAMIC_LIBRARY_NOVATEL_FRAMER_HPP
#define DYNAMIC_LIBRARY_NOVATEL_FRAMER_HPP

#include "decoders_export.h"
#include "edie/decoders/novatel/common.hpp"
#include "edie/decoders/novatel/filter.hpp"
#include "edie/decoders/novatel/framer.hpp"

extern "C"
{
    // Logger
    DECODERS_EXPORT bool NovatelFramerSetLoggerLevel(novatel::edie::oem::Framer* pclFramer_, uint32_t uiLogLevel_);
    DECODERS_EXPORT void NovatelFramerShutdownLogger(novatel::edie::oem::Framer* pclFramer_);

    // Construct/Destruct
    DECODERS_EXPORT novatel::edie::oem::Framer* NovatelFramerInit();
    DECODERS_EXPORT void NovatelFramerDelete(novatel::edie::oem::Framer* pclFramer_);

    // Config
    DECODERS_EXPORT void NovatelFramerFrameJson(novatel::edie::oem::Framer* pclFramer_, bool bFrameJson_);
    DECODERS_EXPORT void NovatelFramerPayloadOnly(novatel::edie::oem::Framer* pclFramer_, bool bPayloadOnly_);
    DECODERS_EXPORT void NovatelFramerReportUnknownBytes(novatel::edie::oem::Framer* pclFramer_, bool bReportUnknownBytes_);

    // R/W
    DECODERS_EXPORT uint32_t NovatelFramerGetAvailableBytes(novatel::edie::oem::Framer* pclFramer_);
    DECODERS_EXPORT uint32_t NovatelFramerWrite(novatel::edie::oem::Framer* pclFramer_, unsigned char* pucBytes_, uint32_t uiByteCount_);
    DECODERS_EXPORT novatel::edie::STATUS NovatelFramerRead(novatel::edie::oem::Framer* pclFramer_, unsigned char* pucBuffer_, uint32_t uiBufferSize_,
                                                            novatel::edie::oem::MetaDataStruct* pstMetaData_);
    DECODERS_EXPORT uint32_t NovatelFramerFlush(novatel::edie::oem::Framer* pclFramer_, unsigned char* pucBuffer_, uint32_t uiBufferSize_);
}

#endif // DYNAMIC_LIBRARY_NOVATEL_FRAMER_HPP
