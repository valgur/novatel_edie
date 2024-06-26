// ===============================================================================
// |                                                                             |
// |  COPYRIGHT NovAtel Inc, 2024. All rights reserved.                          |
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
// ! \file novatel_common.h
// ===============================================================================

#ifndef C_API_NOVATEL_COMMON_H
#define C_API_NOVATEL_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#define EDIE_NMEA_SYNC '$'
#define EDIE_NMEA_SYNC_LENGTH 1
#define EDIE_NMEA_CRC_LENGTH 2
#define EDIE_OEM4_ASCII_SYNC '#'
#define EDIE_OEM4_ASCII_FIELD_SEPARATOR ','
#define EDIE_OEM4_ASCII_HEADER_TERMINATOR ';'
#define EDIE_OEM4_ASCII_SYNC_LENGTH 1
#define EDIE_OEM4_ASCII_CRC_DELIMITER '*'
#define EDIE_OEM4_ASCII_CRC_LENGTH 8
#define EDIE_OEM4_SHORT_ASCII_SYNC '%'
#define EDIE_OEM4_ASCII_MESSAGE_NAME_MAX 40
#define EDIE_OEM4_SHORT_ASCII_SYNC_LENGTH 1
#define EDIE_OEM4_ABBREV_ASCII_SYNC '<'
#define EDIE_OEM4_ABBREV_ASCII_SEPARATOR ' '
#define EDIE_OEM4_ABBREV_ASCII_INDENTATION_LENGTH 5; //! Number of spaces per Abbrev ASCII indentation level.
#define EDIE_OEM4_ERROR_PREFIX_LENGTH 6
#define EDIE_OEM4_BINARY_SYNC1 0xAA
#define EDIE_OEM4_BINARY_SYNC2 0x44
#define EDIE_OEM4_BINARY_SYNC3 0x12
#define EDIE_OEM4_BINARY_SYNC_LENGTH 3
#define EDIE_OEM4_BINARY_HEADER_LENGTH 28
#define EDIE_OEM4_BINARY_CRC_LENGTH 4
#define EDIE_OEM4_SHORT_BINARY_SYNC3 0x13
#define EDIE_OEM4_SHORT_BINARY_SYNC_LENGTH 3
#define EDIE_OEM4_SHORT_BINARY_HEADER_LENGTH 12
#define EDIE_OEM4_PROPRIETARY_BINARY_SYNC2 0x45

    enum NovAtelFrameState
    {
        WAITING_FOR_SYNC,                  //!< First sync byte of Header.
        WAITING_FOR_BINARY_SYNC2,          //!< Second sync byte of Header.
        WAITING_FOR_BINARY_SYNC3,          //!< Third sync byte of Header.
        WAITING_FOR_ABB_ASCII_SYNC2,       //!< Second 'sync byte' of header.
        WAITING_FOR_BINARY_HEADER,         //!< Read complete Binary header.
        WAITING_FOR_SHORT_BINARY_HEADER,   //!< Read complete short Binary header.
        WAITING_FOR_ABB_ASCII_HEADER,      //!< Read complete Abbreviated ASCII header.
        WAITING_FOR_BINARY_BODY_AND_CRC,   //!< Read complete Binary log including CRC.
        WAITING_FOR_ASCII_HEADER_AND_BODY, //!< Read complete ASCII log.
        WAITING_FOR_ASCII_CRC,             //!< Read complete ASCII CRC.
        WAITING_FOR_NMEA_BODY,             //!< Read complete NMEA log.
        WAITING_FOR_NMEA_CRC,              //!< Read complete NMEA CRC.
        WAITING_FOR_ABB_ASCII_BODY,        //!< Read complete Abbreviated ASCII log.
        WAITING_FOR_JSON_OBJECT,           //!< Read complete JSON object.
        COMPLETE_MESSAGE                   //!< Completed decoding of one log.
    };

    enum EDIE_ASCII_HEADER
    {
        EDIE_ASCII_HEADER_MESSAGE_NAME,        //!< Ascii log Name.
        EDIE_ASCII_HEADER_PORT,                //!< Receiver logging port.
        EDIE_ASCII_HEADER_SEQUENCE,            //!< Embedded log sequence number.
        EDIE_ASCII_HEADER_IDLE_TIME,           //!< Receiver Idle time.
        EDIE_ASCII_HEADER_TIME_STATUS,         //!< GPS reference time status.
        EDIE_ASCII_HEADER_WEEK,                //!< GPS Week number.
        EDIE_ASCII_HEADER_SECONDS,             //!< GPS week seconds.
        EDIE_ASCII_HEADER_RECEIVER_STATUS,     //!< Receiver status.
        EDIE_ASCII_HEADER_MSG_DEF_CRC,         //!< Reserved Field.
        EDIE_ASCII_HEADER_RECEIVER_SW_VERSION, //!< Receiver software version.
    };

    typedef struct NovatelMetaDataStruct NovatelMetaDataStruct;
    typedef struct NovatelIntermediateHeader NovatelIntermediateHeader;
    typedef struct Oem4BinaryShortHeader Oem4BinaryShortHeader;
    typedef struct Oem4BinaryHeader Oem4BinaryHeader;

#ifdef __cplusplus
}
#endif

#endif // C_API_NOVATEL_COMMON_H
