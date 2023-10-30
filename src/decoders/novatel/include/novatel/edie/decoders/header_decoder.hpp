////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT NovAtel Inc, 2022. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////////////////
//                            DESCRIPTION
//
//! \file header_decoder.cpp
//! \brief Decoder OEM message headers.
////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
// Recursive Inclusion
//-----------------------------------------------------------------------
#ifndef NOVATEL_HEADER_DECODER_HPP
#define NOVATEL_HEADER_DECODER_HPP

//-----------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------
#include "novatel/edie/common/common.hpp"
#include "novatel/edie/common/crc32.hpp"
#include "novatel/edie/common/jsonreader.hpp"
#include "novatel/edie/common/logger/logger.hpp"
#include "novatel/edie/decoders/common.hpp"

#include <nlohmann/json.hpp>
#include <variant>
#include <string>
#include <sstream>
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdarg>

namespace novatel::edie::oem {

//============================================================================
//! \class HeaderDecoder
//! \brief Decode framed OEM message headers.
//============================================================================
class HeaderDecoder
{
private:
   std::shared_ptr<spdlog::logger> pclMyLogger;
   JsonReader* pclMyMsgDb{ nullptr };
   EnumDefinition* vMyRespDefns{ nullptr };
   EnumDefinition* vMyCommandDefns{ nullptr };
   EnumDefinition* vMyPortAddrDefns{ nullptr };
   EnumDefinition* vMyGPSTimeStatusDefns{ nullptr };

   char* pucTempBufConvert{ nullptr };
   char* pucBeginningConvert{ nullptr };
   uint32_t uiMyBufferBytesRemaining;
   uint32_t uiMyAbbrevAsciiIndentationLevel;
   MessageDefinition stMyRespDef;

   uint32_t MsgNameToMsgId(std::string sMsgName_);
   std::string MsgIdToMsgName(const uint32_t uiMessageID_);

   // Decode novatel headers
   template <ASCIIHEADER     eField> [[nodiscard]] bool DecodeAsciiHeaderField (IntermediateHeader& stIntermediateHeader_, char** ppcLogBuf_);
   template <ASCIIHEADER... eFields> [[nodiscard]] bool DecodeAsciiHeaderFields(IntermediateHeader& stIntermediateHeader_, char** ppcLogBuf_);
   void DecodeJsonHeader(json clJsonHeader_, IntermediateHeader& stIntermediateHeader_);

public:
   //----------------------------------------------------------------------------
   //! \brief A constructor for the HeaderDecoder class.
   //
   //! \param[in] pclJsonDb_ A pointer to a JsonReader object. Defaults to nullptr.
   //----------------------------------------------------------------------------
   HeaderDecoder(JsonReader* pclJsonDb_ = nullptr);

   //----------------------------------------------------------------------------
   //! \brief Load a JsonReader object.
   //
   //! \param[in] pclJsonDb_ A pointer to a JsonReader object.
   //----------------------------------------------------------------------------
   void
   LoadJsonDb(JsonReader* pclJsonDb_);

   //----------------------------------------------------------------------------
   //! \brief Get the internal logger.
   //
   //! \return A shared_ptr to the spdlog::logger.
   //----------------------------------------------------------------------------
   std::shared_ptr<spdlog::logger>
   GetLogger();

   //----------------------------------------------------------------------------
   //! \brief Set the level of detail produced by the internal logger.
   //
   //! \param[in] eLevel_ The logging level to enable.
   //----------------------------------------------------------------------------
   void
   SetLoggerLevel(spdlog::level::level_enum eLevel_);

   //----------------------------------------------------------------------------
   //! \brief Shutdown the internal logger.
   //----------------------------------------------------------------------------
   void
   ShutdownLogger();

   //----------------------------------------------------------------------------
   //! \brief Decode an OEM message header from the provided frame.
   //
   //! \param[in] pucHeader_ A pointer to an OEM message header.
   //! \param[out] stIntermediateHeader_ The IntermediateHeader to be populated.
   //! \param[in, out] stMetaData_ MetaDataStruct to provide information about
   //! the frame and be fully populated to help describe the decoded log.
   //
   //! \return An error code describing the result of decoding.
   //!   SUCCESS: The operation was successful.
   //!   NULL_PROVIDED: pucHeader_ is a null pointer.
   //!   NO_DATABASE: No database was ever loaded into this component.
   //!   FAILURE: Failed to decode a header field.
   //!   UNSUPPORTED: Attempted to decode an unsupported format.
   //!   UNKNOWN: The header format provided is not known.
   //----------------------------------------------------------------------------
   [[nodiscard]] STATUS
   Decode(unsigned char* pucHeader_, IntermediateHeader& stIntermediateHeader_, MetaDataStruct& stMetaData_);
};
}
#endif
