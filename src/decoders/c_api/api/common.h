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
// ! \file common.h
// ===============================================================================

#ifndef DECODERS_C_API_COMMON_H
#define DECODERS_C_API_COMMON_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum EDIE_STATUS
{
    EDIE_STATUS_SUCCESS,                //!< Successfully found a frame in the framer buffer.
    EDIE_STATUS_FAILURE,                //!< An unexpected failure occurred.
    EDIE_STATUS_UNKNOWN,                //!< Could not identify bytes as a protocol.
    EDIE_STATUS_INCOMPLETE,             //!< It is possible that a valid frame exists in the frame buffer, but more
                                        //!< information is needed.
    EDIE_STATUS_INCOMPLETE_MORE_DATA,   //!< The current frame buffer is incomplete but more data is expected.
    EDIE_STATUS_NULL_PROVIDED,          //!< A null pointer was provided.
    EDIE_STATUS_NO_DATABASE,            //!< No database has been provided to the component.
    EDIE_STATUS_NO_DEFINITION,          //!< No definition could be found in the database for the provided message.
    EDIE_STATUS_NO_DEFINITION_EMBEDDED, //!< No definition could be found in the database for the embedded
                                        //!< message in the RXCONFIG log.
    EDIE_STATUS_BUFFER_FULL,            //!< The provided destination buffer is not big enough to contain the frame.
    EDIE_STATUS_BUFFER_EMPTY,           //!< The internal circular buffer does not contain any unread bytes
    EDIE_STATUS_STREAM_EMPTY,           //!< The input stream is empty.
    EDIE_STATUS_UNSUPPORTED,            //!< An attempted operation is unsupported by this component.
    EDIE_STATUS_MALFORMED_INPUT,        //!< The input is recognizable, but has unexpected formatting.
    EDIE_STATUS_DECOMPRESSION_FAILURE   //!< The RANGECMP log could not be decompressed.
} EDIE_STATUS;


typedef enum EDIE_ENCODE_FORMAT
{
    EDIE_ENCODE_FORMAT_FLATTENED_BINARY, //!< NovAtel EDIE "Flattened" binary format.  All strings/arrays are padded to
                                         //!< maximum length to allow programmatic access.
    EDIE_ENCODE_FORMAT_ASCII,            //!< NovAtel ASCII.  If the log was decoded from a SHORT/compressed format, it will be
                                         //!< encoded to the respective SHORT/compressed format.
    EDIE_ENCODE_FORMAT_ABBREV_ASCII,     //!< NovAtel Abbreviated ASCII.
    EDIE_ENCODE_FORMAT_BINARY,           //!< NovAtel Binary.  If the log was decoded from a SHORT/compressed format, it will be
                                         //!< encoded to the respective SHORT/compressed format.
    EDIE_ENCODE_FORMAT_JSON,             //!< A JSON object. See HTML documentation for information on fields.
    EDIE_ENCODE_FORMAT_UNSPECIFIED       //!< No encode format was specified.
} EDIE_ENCODE_FORMAT;


typedef enum EDIE_TIME_STATUS
{
    EDIE_TIME_STATUS_UNKNOWN = 20,             //!< Time validity is unknown.
    EDIE_TIME_STATUS_APPROXIMATE = 60,         //!< Time is set approximately.
    EDIE_TIME_STATUS_COARSEADJUSTING = 80,     //!< Time is approaching coarse precision.
    EDIE_TIME_STATUS_COARSE = 100,             //!< This time is valid to coarse precision.
    EDIE_TIME_STATUS_COARSESTEERING = 120,     //!< Time is coarse set and is being steered.
    EDIE_TIME_STATUS_FREEWHEELING = 130,       //!< Position is lost and the range bias cannot be calculated.
    EDIE_TIME_STATUS_FINEADJUSTING = 140,      //!< Time is adjusting to fine precision.
    EDIE_TIME_STATUS_FINE = 160,               //!< Time has fine precision.
    EDIE_TIME_STATUS_FINEBACKUPSTEERING = 170, //!< Time is fine set and is being steered by the backup system.
    EDIE_TIME_STATUS_FINESTEERING = 180,       //!< Time is fine set and is being steered.
    EDIE_TIME_STATUS_SATTIME = 200,            //!< Time from satellite. Only used in logs containing satellite data such as ephemeris and almanac.
    EDIE_TIME_STATUS_EXTERN = 220,             //!< Time source is external to the Receiver.
    EDIE_TIME_STATUS_EXACT = 240               //!< Time is exact.
} EDIE_TIME_STATUS;

typedef enum EDIE_HEADER_FORMAT
{
    EDIE_HEADER_FORMAT_UNKNOWN = 1,
    EDIE_HEADER_FORMAT_BINARY,
    EDIE_HEADER_FORMAT_SHORT_BINARY,
    EDIE_HEADER_FORMAT_PROPRIETARY_BINARY,
    EDIE_HEADER_FORMAT_ASCII,
    EDIE_HEADER_FORMAT_SHORT_ASCII,
    EDIE_HEADER_FORMAT_ABB_ASCII,
    EDIE_HEADER_FORMAT_NMEA,
    EDIE_HEADER_FORMAT_JSON,
    EDIE_HEADER_FORMAT_SHORT_ABB_ASCII,
    EDIE_HEADER_FORMAT_ALL // Used in filters to indicate all filter types : all new enums should be added before this value
} EDIE_HEADER_FORMAT;


typedef enum EDIE_MESSAGE_FORMAT
{
    EDIE_MESSAGE_FORMAT_BINARY = 0,
    EDIE_MESSAGE_FORMAT_ASCII = 1,
    EDIE_MESSAGE_FORMAT_ABBREV = 2,
    EDIE_MESSAGE_FORMAT_RSRVD = 3
} EDIE_MESSAGE_FORMAT;

typedef enum EDIE_MESSAGE_TYPE_MASK
{
    EDIE_MESSAGE_TYPE_MASK_MEASSRC = 0x1F,
    EDIE_MESSAGE_TYPE_MASK_MSGFORMAT = 0x60,
    EDIE_MESSAGE_TYPE_MASK_RESPONSE = 0x80
} EDIE_MESSAGE_TYPE_MASK;

typedef enum EDIE_MESSAGE_ID_MASK
{
    EDIE_MESSAGE_ID_MASK_LOGID = 0x00FFFF,
    EDIE_MESSAGE_ID_MASK_MEASSRC = EDIE_MESSAGE_TYPE_MASK_MEASSRC << 16,
    EDIE_MESSAGE_ID_MASK_MSGFORMAT = EDIE_MESSAGE_TYPE_MASK_MSGFORMAT << 16,
    EDIE_MESSAGE_ID_MASK_RESPONSE = EDIE_MESSAGE_TYPE_MASK_RESPONSE << 16
} EDIE_MESSAGE_ID_MASK;

typedef enum EDIE_MEASUREMENT_SOURCE
{
    EDIE_MEASUREMENT_SOURCE_PRIMARY,
    EDIE_MEASUREMENT_SOURCE_SECONDARY,
    EDIE_MEASUREMENT_SOURCE_MAX
} EDIE_MEASUREMENT_SOURCE;

typedef struct NovatelMessageDataStruct NovatelMessageDataStruct;
typedef struct EdieSatelliteId EdieSatelliteId; 

#define EDIE_SEC_TO_MILLI_SEC 1000 //!< A Macro definition for number of milliseconds in a second.
#define EDIE_SECS_IN_WEEK 604800   //!< A Macro definition for number of milliseconds in a week.

//!< FW-defined maximum transmittable message length. (32kB)
#define EDIE_MESSAGE_SIZE_MAX 0x8000
//!< Undefined message length assumes that the max log size for this format is the maximum message length allowed to be transmitted by the FW.
#define EDIE_MAX_ASCII_MESSAGE_LENGTH MESSAGE_SIZE_MAX
//!< Undefined message length assumes that the max log size for this format is the maximum message length allowed to be transmitted by the FW.
#define EDIE_MAX_BINARY_MESSAGE_LENGTH MESSAGE_SIZE_MAX
//!< Undefined message length assumes that the max log size for this format is the maximum message length allowed to be transmitted by the FW.
#define EDIE_MAX_SHORT_ASCII_MESSAGE_LENGTH MESSAGE_SIZE_MAX
//!< Short Binary message length cannot exceed the log length max value representation defined by the header.
#define EDIE_MAX_SHORT_BINARY_MESSAGE_LENGTH (12 + 255 + 4)
//!< Undefined message length assumes that the max log size for this format is the maximum message length allowed to be transmitted by the FW.
#define EDIE_MAX_ABB_ASCII_RESPONSE_LENGTH MESSAGE_SIZE_MAX
//!< NovAtel Docs - NMEA Standard Logs: Explicitly states that the maximum allowable is 82 chars.
//!< Numerous internal logs break that standard, so we will use 256here as a safety measure.
#define EDIE_MAX_NMEA_MESSAGE_LENGTH 256 //(82)

#ifdef __cplusplus
}
#endif

#endif // DECODERS_C_API_COMMON_H
