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
// ! \file novatel_file_parser.hpp
// ===============================================================================

#ifndef DYNAMIC_LIBRARY_NOVATEL_FILE_PARSER_HPP
#define DYNAMIC_LIBRARY_NOVATEL_FILE_PARSER_HPP

#include "decoders_export.h"
#include "edie/decoders/novatel/common.hpp"
#include "edie/decoders/novatel/file_parser.hpp"
#include "edie/decoders/novatel/filter.hpp"
#include "edie/hw_interface/stream_interface/inputfilestream.hpp"

extern "C"
{
    // Construct/Destruct
    DECODERS_EXPORT novatel::edie::oem::FileParser* NovatelFileParserInit(JsonReader* pclJsonDb_);
    DECODERS_EXPORT void NovatelFileParserDelete(novatel::edie::oem::FileParser* pclFileParser_);

    // Config
    DECODERS_EXPORT void NovatelFileParserLoadJsonDb(novatel::edie::oem::FileParser* pclFileParser_, JsonReader* pclJsonDb_);

    DECODERS_EXPORT void NovatelFileParserSetIgnoreAbbrevAsciiResponses(novatel::edie::oem::FileParser* pclFileParser_,
                                                                        bool bIgnoreAbbrevAsciiResponsesCmp_);
    DECODERS_EXPORT bool NovatelFileParserGetIgnoreAbbrevAsciiResponses(novatel::edie::oem::FileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetDecompressRangeCmp(novatel::edie::oem::FileParser* pclFileParser_, bool bDecompressRangeCmp_);
    DECODERS_EXPORT bool NovatelFileParserGetDecompressRangeCmp(novatel::edie::oem::FileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetReturnUnknownBytes(novatel::edie::oem::FileParser* pclFileParser_, bool bReturnUnknownBytes_);
    DECODERS_EXPORT bool NovatelFileParserGetReturnUnknownBytes(novatel::edie::oem::FileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetEncodeFormat(novatel::edie::oem::FileParser* pclFileParser_,
                                                          novatel::edie::ENCODE_FORMAT eEncodeFormat_);
    DECODERS_EXPORT novatel::edie::ENCODE_FORMAT NovatelFileParserGetEncodeFormat(novatel::edie::oem::FileParser* pclFileParser_);

    DECODERS_EXPORT novatel::edie::oem::Filter* NovatelFileParserGetFilter(novatel::edie::oem::FileParser* pclFileParser_);
    DECODERS_EXPORT void NovatelFileParserSetFilter(novatel::edie::oem::FileParser* pclFileParser_, novatel::edie::oem::Filter* pclFilter_);

    DECODERS_EXPORT unsigned char* NovatelFileParserGetBuffer(novatel::edie::oem::FileParser* pclFileParser_);

    // Stream, R/W
    DECODERS_EXPORT bool NovatelFileParserSetStream(novatel::edie::oem::FileParser* pclFileParser_, InputFileStream* pclIfs_);
    DECODERS_EXPORT uint32_t NovatelFileParserGetPercentRead(novatel::edie::oem::FileParser* pclFileParser_);

    // R/W
    DECODERS_EXPORT novatel::edie::STATUS NovatelFileParserRead(novatel::edie::oem::FileParser* pclFileParser_,
                                                                novatel::edie::MessageDataStruct* pstMessageData_,
                                                                novatel::edie::oem::MetaDataStruct* pstMetaData_);
    DECODERS_EXPORT bool NovatelFileParserReset(novatel::edie::oem::FileParser* pclFileParser_);

    DECODERS_EXPORT uint32_t NovatelFileParserFlush(novatel::edie::oem::FileParser* pclFileParser_, unsigned char* pucBuffer_,
                                                    uint32_t uiBufferSize_);
}

#endif // DYNAMIC_LIBRARY_NOVATEL_FILE_PARSER_HPP
