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
// ! \file novatel_filter.cpp
// ===============================================================================

#include "decoders/c_api/api/novatel_filter.h"

#include "decoders/novatel/api/filter.hpp"

using namespace novatel;

namespace {
edie::oem::Filter* cast(NovatelFilter* pclFilter_) { return reinterpret_cast<edie::oem::Filter*>(pclFilter_); }
} // namespace

bool NovatelFilterSetLoggerLevel(NovatelFilter* pclFilter_, uint32_t uiLogLevel_)
{
    return pclFilter_ && uiLogLevel_ >= spdlog::level::level_enum::trace && uiLogLevel_ < spdlog::level::level_enum::n_levels
           ? cast(pclFilter_)->SetLoggerLevel(static_cast<spdlog::level::level_enum>(uiLogLevel_)),
           true : false;
}

void NovatelFilterShutdownLogger(NovatelFilter* pclFilter_)
{
    if (pclFilter_) { cast(pclFilter_)->ShutdownLogger(); } // TODO: ShutdownLogger is static, this function signature should be changed
}

NovatelFilter* NovatelFilterInit() { return reinterpret_cast<NovatelFilter*>(new edie::oem::Filter()); }

void NovatelFilterDelete(NovatelFilter* pclFilter_) { delete cast(pclFilter_); }

void NovatelFilterSetIncludeLowerTime(NovatelFilter* pclFilter_, uint32_t uiLowerTimeWeek_, double dLowerTimeSec_)
{
    if (pclFilter_) { cast(pclFilter_)->SetIncludeLowerTimeBound(uiLowerTimeWeek_, dLowerTimeSec_); }
}

void NovatelFilterSetIncludeUpperTime(NovatelFilter* pclFilter_, uint32_t uiUpperTime_, double dUpperTimeSec_)
{
    if (pclFilter_) { cast(pclFilter_)->SetIncludeUpperTimeBound(uiUpperTime_, dUpperTimeSec_); }
}

void NovatelFilterInvertTimeFilter(NovatelFilter* pclFilter_, bool bInvert_)
{
    if (pclFilter_) { cast(pclFilter_)->InvertTimeFilter(bInvert_); }
}

void NovatelFilterSetIncludeDecimation(NovatelFilter* pclFilter_, double dPeriodSec_)
{
    if (pclFilter_) { cast(pclFilter_)->SetIncludeDecimation(dPeriodSec_); }
}

void NovatelFilterInvertDecimationFilter(NovatelFilter* pclFilter_, bool bInvert_)
{
    if (pclFilter_) { cast(pclFilter_)->InvertDecimationFilter(bInvert_); }
}

void NovatelFilterIncludeTimeStatus(NovatelFilter* pclFilter_, EDIE_TIME_STATUS eTimeStatus_)
{
    if (pclFilter_) { cast(pclFilter_)->IncludeTimeStatus(static_cast<edie::TIME_STATUS>(eTimeStatus_)); }
}

void NovatelFilterInvertTimeStatusFilter(NovatelFilter* pclFilter_, bool bInvert_)
{
    if (pclFilter_) { cast(pclFilter_)->InvertTimeStatusFilter(bInvert_); }
}

void NovatelFilterIncludeMessageId(NovatelFilter* pclFilter_, uint32_t uiId_, EDIE_HEADER_FORMAT eFormat_, EDIE_MEASUREMENT_SOURCE eSource_)
{
    if (pclFilter_)
    {
        cast(pclFilter_)->IncludeMessageId(uiId_, static_cast<edie::HEADER_FORMAT>(eFormat_), static_cast<edie::MEASUREMENT_SOURCE>(eSource_));
    }
}

void NovatelFilterInvertMessageIdFilter(NovatelFilter* pclFilter_, bool bInvert_)
{
    if (pclFilter_) { cast(pclFilter_)->InvertMessageIdFilter(bInvert_); }
}

void NovatelFilterIncludeMessageName(NovatelFilter* pclFilter_, uint8_t* pucMessageName_, EDIE_HEADER_FORMAT eFormat_,
                                     EDIE_MEASUREMENT_SOURCE eSource_)
{
    if (pclFilter_ && pucMessageName_)
    {
        cast(pclFilter_)
            ->IncludeMessageName(std::string(reinterpret_cast<char*>(pucMessageName_)), static_cast<edie::HEADER_FORMAT>(eFormat_),
                                 static_cast<edie::MEASUREMENT_SOURCE>(eSource_));
    }
}

void NovatelFilterInvertMessageNameFilter(NovatelFilter* pclFilter_, bool bInvert_)
{
    if (pclFilter_) { cast(pclFilter_)->InvertMessageNameFilter(bInvert_); }
}

void NovatelFilterIncludeNmeaMessages(NovatelFilter* pclFilter_, bool bInvert_)
{
    if (pclFilter_) { cast(pclFilter_)->IncludeNmeaMessages(bInvert_); }
}

bool NovatelFilterDoFiltering(NovatelFilter* pclFilter_, NovatelMetaDataStruct* pstMetaData_)
{
    return pclFilter_ && pstMetaData_ ? cast(pclFilter_)->DoFiltering(*reinterpret_cast<edie::oem::MetaDataStruct*>(pstMetaData_)) : false;
}

void NovatelFilterClearFilters(NovatelFilter* pclFilter_)
{
    if (pclFilter_) { cast(pclFilter_)->ClearFilters(); }
}
