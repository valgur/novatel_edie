#include "decoders/novatel/common.hpp"
#include "bindings_core.hpp"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_novatel_common(nb::module_& m)
{
   nb::enum_<oem::HEADERFORMAT>(m, "HEADERFORMAT")
      .value("UNKNOWN", oem::HEADERFORMAT::UNKNOWN)
      .value("BINARY", oem::HEADERFORMAT::BINARY)
      .value("SHORT_BINARY", oem::HEADERFORMAT::SHORT_BINARY)
      .value("PROPRIETARY_BINARY", oem::HEADERFORMAT::PROPRIETARY_BINARY)
      .value("ASCII", oem::HEADERFORMAT::ASCII)
      .value("SHORT_ASCII", oem::HEADERFORMAT::SHORT_ASCII)
      .value("ABB_ASCII", oem::HEADERFORMAT::ABB_ASCII)
      .value("NMEA", oem::HEADERFORMAT::NMEA)
      .value("JSON", oem::HEADERFORMAT::JSON)
      .value("SHORT_ABB_ASCII", oem::HEADERFORMAT::SHORT_ABB_ASCII)
      .value("ALL", oem::HEADERFORMAT::ALL);

   nb::class_<oem::MetaDataStruct>(m, "MetaData")
      .def(nb::init<>())
      .def(nb::init<oem::HEADERFORMAT, uint32_t>(), "format"_a, "length"_a)
      .def_rw("format", &oem::MetaDataStruct::eFormat)
      .def_rw("measurement_source", &oem::MetaDataStruct::eMeasurementSource)
      .def_rw("time_status", &oem::MetaDataStruct::eTimeStatus)
      .def_rw("response", &oem::MetaDataStruct::bResponse)
      .def_rw("week", &oem::MetaDataStruct::usWeek)
      .def_rw("milliseconds", &oem::MetaDataStruct::dMilliseconds)
      .def_rw("binary_msg_length", &oem::MetaDataStruct::uiBinaryMsgLength,
              "Message length according to the binary header. If ASCII, this field is not used.")
      .def_rw("length", &oem::MetaDataStruct::uiLength, "Length of the entire log, including the header and CRC.")
      .def_rw("header_length", &oem::MetaDataStruct::uiHeaderLength,
              "The length of the message header. Used for NovAtel logs.")
      .def_rw("message_id", &oem::MetaDataStruct::usMessageID)
      .def_rw("message_crc", &oem::MetaDataStruct::uiMessageCRC)
      .def_prop_rw("message_name", [](oem::MetaDataStruct& self) {
         return nb::str(self.acMessageName);
      }, [](oem::MetaDataStruct& self, std::string message_name) {
         if (message_name.length() > oem::OEM4_ASCII_MESSAGE_NAME_MAX)
            throw std::runtime_error("Message name is too long");
         memcpy(self.acMessageName, message_name.c_str(), message_name.length());
         self.acMessageName[message_name.length()] = '\0';
      });

   nb::class_<oem::IntermediateHeader>(m, "IntermediateHeader")
      .def(nb::init<>())
      .def_rw("message_id", &oem::IntermediateHeader::usMessageID)
      .def_rw("message_type", &oem::IntermediateHeader::ucMessageType)
      .def_rw("port_address", &oem::IntermediateHeader::uiPortAddress)
      .def_rw("length", &oem::IntermediateHeader::usLength)
      .def_rw("sequence", &oem::IntermediateHeader::usSequence)
      .def_rw("idle_time", &oem::IntermediateHeader::ucIdleTime)
      .def_rw("time_status", &oem::IntermediateHeader::uiTimeStatus)
      .def_rw("week", &oem::IntermediateHeader::usWeek)
      .def_rw("milliseconds", &oem::IntermediateHeader::dMilliseconds)
      .def_rw("receiver_status", &oem::IntermediateHeader::uiReceiverStatus)
      .def_rw("message_definition_crc", &oem::IntermediateHeader::uiMessageDefinitionCRC)
      .def_rw("receiver_sw_version", &oem::IntermediateHeader::usReceiverSwVersion);
}
