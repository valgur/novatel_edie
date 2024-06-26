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
// ! \file novatel_filter.h
// ===============================================================================

#ifndef C_API_NOVATEL_FILTER_H
#define C_API_NOVATEL_FILTER_H

#include <stdint.h>

#include "decoders/c_api/api/common.h"
#include "decoders/c_api/api/decoders_export.h"
#include "decoders/c_api/api/novatel_encoder.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct NovatelFilter NovatelFilter;

    // Logger
    DECODERS_EXPORT bool NovatelFilterSetLoggerLevel(NovatelFilter* pclFilter_, uint32_t uiLogLevel_);
    DECODERS_EXPORT void NovatelFilterShutdownLogger(NovatelFilter* pclFilter_);

    // Construct/Destruct
    DECODERS_EXPORT NovatelFilter* NovatelFilterInit();
    DECODERS_EXPORT void NovatelFilterDelete(NovatelFilter* pclFilter_);

    // Config
    DECODERS_EXPORT void NovatelFilterSetIncludeLowerTime(NovatelFilter* pclFilter_, uint32_t uiLowerTimeWeek_, double dLowerTimeSec_);
    DECODERS_EXPORT void NovatelFilterSetIncludeUpperTime(NovatelFilter* pclFilter_, uint32_t uiUpperTime_, double dUpperTimeSec_);
    DECODERS_EXPORT void NovatelFilterInvertTimeFilter(NovatelFilter* pclFilter_, bool bInvert_);

    DECODERS_EXPORT void NovatelFilterSetIncludeDecimation(NovatelFilter* pclFilter_, double dPeriodSec_);
    DECODERS_EXPORT void NovatelFilterInvertDecimationFilter(NovatelFilter* pclFilter_, bool bInvert_);

    DECODERS_EXPORT void NovatelFilterIncludeTimeStatus(NovatelFilter* pclFilter_, EDIE_TIME_STATUS eTimeStatus_);
    DECODERS_EXPORT void NovatelFilterInvertTimeStatusFilter(NovatelFilter* pclFilter_, bool bInvert_);

    DECODERS_EXPORT void NovatelFilterIncludeMessageId(NovatelFilter* pclFilter_, uint32_t uiId_, EDIE_HEADER_FORMAT eFormat_,
                                                       EDIE_MEASUREMENT_SOURCE eSource_);
    DECODERS_EXPORT void NovatelFilterInvertMessageIdFilter(NovatelFilter* pclFilter_, bool bInvert_);

    DECODERS_EXPORT void NovatelFilterIncludeMessageName(NovatelFilter* pclFilter_, uint8_t* pucMessageName_, EDIE_HEADER_FORMAT eFormat_,
                                                         EDIE_MEASUREMENT_SOURCE eSource_);
    DECODERS_EXPORT void NovatelFilterInvertMessageNameFilter(NovatelFilter* pclFilter_, bool bInvert_);

    DECODERS_EXPORT void NovatelFilterIncludeNmeaMessages(NovatelFilter* pclFilter_, bool bInvert_);

    // NovatelFilter
    DECODERS_EXPORT bool NovatelFilterDoFiltering(NovatelFilter* pclFilter_, NovatelMetaDataStruct* pstMetaData_);
    DECODERS_EXPORT void NovatelFilterClearFilters(NovatelFilter* pclFilter_);

#ifdef __cplusplus
}
#endif

#endif // C_API_NOVATEL_FILTER_H
