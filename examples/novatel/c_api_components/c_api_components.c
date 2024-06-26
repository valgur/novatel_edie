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
// ! \file c_api_components.c
// ===============================================================================

#include <decoders/c_api/api/common_json_reader.h>
#include <decoders/c_api/api/common_logger.h>
#include <decoders/c_api/api/helpers.h>
#include <decoders/c_api/api/novatel_encoder.h>
#include <decoders/c_api/api/novatel_filter.h>
#include <decoders/c_api/api/novatel_framer.h>
#include <decoders/c_api/api/novatel_header_decoder.h>
#include <decoders/c_api/api/novatel_message_decoder.h>
#include <decoders/c_api/api/version.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // This example uses the default logger config, but you can also pass a config file to InitLogger()
    // Example config file: logger\example_logger_config.toml
    CommonLoggerSetup();
    CommonLoggerSetLoggerLevel(SPDLOG_LEVEL_DEBUG);
    // Logger::AddConsoleLogging(pclLogger);
    // Logger::AddRotatingFileLogger(pclLogger);

    // Get command line arguments
    CommonLoggerLogInfo("Decoder library information:\n{}", PrettyVersion());

    char* sEncodeFormat = "ASCII";
    if (argc == 2 && strcmp(argv[1], "-V") == 0) { return 0; }
    if (argc < 3)
    {
        CommonLoggerLogError("ERROR: Need to specify a JSON message definitions DB, an input file and an output format.");
        CommonLoggerLogError("Example: converter <path to Json DB> <path to input file> <output format>");
        return 1;
    }
    if (argc == 4) { sEncodeFormat = argv[3]; }

    // Check command line arguments
    const char* pathJsonDb = argv[1];
    if (!PathExists(pathJsonDb))
    {
        CommonLoggerLogError("File \"{}\" does not exist", pathJsonDb);
        return 1;
    }
    const char* pathInFilename = argv[2];
    if (!PathExists(pathInFilename))
    {
        CommonLoggerLogError("File \"{}\" does not exist", pathInFilename);
        return 1;
    }

    EDIE_ENCODE_FORMAT eEncodeFormat = StringToEncodeFormat(sEncodeFormat);
    if (eEncodeFormat == EDIE_ENCODE_FORMAT_UNSPECIFIED)
    {
        CommonLoggerLogError("Unspecified output format.\n\tASCII\n\tBINARY\n\tFLATTENED_BINARY");
        return 1;
    }

    CommonLoggerLogInfo("Loading Database...");
    CommonJsonReader* pclJsonDb = CommonJsonReaderInit();
    CommonJsonReaderLoadFile(pclJsonDb, pathJsonDb);

    // Set up the EDIE components
    NovatelFramer* pclFramer = NovatelFramerInit();
    NovatelFramerSetLoggerLevel(pclFramer, SPDLOG_LEVEL_DEBUG);
    NovatelFramerFrameJson(pclFramer, false);
    NovatelFramerPayloadOnly(pclFramer, false);
    NovatelFramerReportUnknownBytes(pclFramer, true);

    NovatelHeaderDecoder* pclHeaderDecoder = NovatelHeaderDecoderInit(pclJsonDb);
    NovatelHeaderDecoderSetLoggerLevel(pclHeaderDecoder, SPDLOG_LEVEL_DEBUG);

    NovatelMessageDecoder* pclMessageDecoder = NovatelMessageDecoderInit(pclJsonDb);
    NovatelMessageDecoderSetLoggerLevel(pclMessageDecoder, SPDLOG_LEVEL_DEBUG);

    NovatelEncoder* pclEncoder = NovatelEncoderInit(pclJsonDb);
    NovatelEncoderSetLoggerLevel(pclEncoder, SPDLOG_LEVEL_DEBUG);

    NovatelFilter* pclFilter = NovatelFilterInit();
    NovatelFilterSetLoggerLevel(pclFilter, SPDLOG_LEVEL_DEBUG);

    // Set up buffers
    unsigned char acFrameBuffer[MAX_ASCII_MESSAGE_LENGTH];
    unsigned char acEncodeBuffer[MAX_ASCII_MESSAGE_LENGTH];
    unsigned char* pucEncodedMessageBuffer = acEncodeBuffer;

    // Initialize structures and error codes
    auto eFramerStatus = EDIE_STATUS_UNKNOWN;
    auto eDecoderStatus = EDIE_STATUS_UNKNOWN;
    auto eEncoderStatus = EDIE_STATUS_UNKNOWN;

    NovatelIntermediateHeader stHeader;
    std::vector<FieldContainer> stMessage;

    NovatelMetaDataStruct stMetaData;
    NovatelMessageDataStruct stMessageData;

    // Initialize FS structures and buffers
    StreamReadStatus stReadStatus;
    ReadDataStructure stReadData;
    unsigned char* acIfsReadBuffer[MAX_ASCII_MESSAGE_LENGTH];
    stReadData.cData = reinterpret_cast<char*>(acIfsReadBuffer);
    stReadData.uiDataSize = sizeof(acIfsReadBuffer);

    // Set up file streams
    InputFileStream clIfs(pathInFilename.string().c_str());
    OutputFileStream clConvertedLogsOfs(pathInFilename.string().append(".").append(sEncodeFormat).c_str());
    OutputFileStream clUnknownBytesOfs(pathInFilename.string().append(".").append(sEncodeFormat).append(".UNKNOWN").c_str());

    while (!stReadStatus.bEOS)
    {
        stReadStatus = clIfs.ReadData(stReadData);
        NovatelFramerWrite(pclFramer, reinterpret_cast<unsigned char*>(stReadData.cData), stReadStatus.uiCurrentStreamRead);
        // Clearing INCOMPLETE status when internal buffer needs more bytes.
        eFramerStatus = EDIE_STATUS_INCOMPLETE_MORE_DATA;

        while (eFramerStatus != EDIE_STATUS_BUFFER_EMPTY && eFramerStatus != EDIE_STATUS_INCOMPLETE)
        {
            unsigned char* pucFrameBuffer = acFrameBuffer;
            eFramerStatus = NovatelFramerRead(pclFramer, pucFrameBuffer, MAX_ASCII_MESSAGE_LENGTH, &stMetaData);

            if (eFramerStatus == EDIE_STATUS_SUCCESS)
            {
                if (stMetaData.bResponse)
                {
                    clUnknownBytesOfs.WriteData(reinterpret_cast<char*>(pucFrameBuffer), stMetaData.uiLength);
                    continue;
                }

                pucFrameBuffer[stMetaData.uiLength] = '\0';
                CommonLoggerLogInfo("Framed: {}", pucFrameBuffer);

                // Decode the header.  Get metadata here and populate the Intermediate header.
                eDecoderStatus = NovatelHeaderDecoderDecode(pclHeaderDecoder, pucFrameBuffer, &stHeader, &stMetaData);

                if (eDecoderStatus == EDIE_STATUS_SUCCESS)
                {
                    // Filter the log, pass over this log if we don't want it.
                    if (!NovatelFilterDoFiltering(pclFilter, &stMetaData)) { continue; }

                    pucFrameBuffer += stMetaData.uiHeaderLength;
                    uint32_t uiBodyLength = stMetaData.uiLength - stMetaData.uiHeaderLength;
                    // Decode the Log, pass the metadata and populate the intermediate log.
                    eDecoderStatus = NovatelMessageDecoderDecode(pclMessageDecoder, pucFrameBuffer, &stMessage, &stMetaData);

                    if (eDecoderStatus == EDIE_STATUS_SUCCESS)
                    {
                        eEncoderStatus = NovatelEncoderEncode(pclEncoder, pucEncodedMessageBuffer, MAX_ASCII_MESSAGE_LENGTH, &stHeader, &stMessage,
                                                              &stMessageData, &stMetaData, eEncodeFormat);

                        if (eEncoderStatus == EDIE_STATUS_SUCCESS)
                        {
                            clConvertedLogsOfs.WriteData(reinterpret_cast<char*>(stMessageData.pucMessage), stMessageData.uiMessageLength);
                            stMessageData.pucMessage[stMessageData.uiMessageLength] = '\0';
                            CommonLoggerLogInfo("Encoded: ({}) {}\n", stMessageData.uiMessageLength,
                                                reinterpret_cast<char*>(stMessageData.pucMessage));
                        }
                        else
                        {
                            clUnknownBytesOfs.WriteData(reinterpret_cast<char*>(pucFrameBuffer), uiBodyLength);
                            CommonLoggerLogWarn("Encoder returned with status code {}", (int32_t)eEncoderStatus);
                        }
                    }
                    else
                    {
                        clUnknownBytesOfs.WriteData(reinterpret_cast<char*>(pucFrameBuffer), uiBodyLength);
                        CommonLoggerLogWarn("MessageDecoder returned with status code {}", (int32_t)eDecoderStatus);
                    }
                }
                else
                {
                    clUnknownBytesOfs.WriteData(reinterpret_cast<char*>(pucFrameBuffer), stMetaData.uiLength);
                    CommonLoggerLogWarn("HeaderDecoder returned with status code {}", (int32_t)eDecoderStatus);
                }
            }
            else if (eFramerStatus == EDIE_STATUS_UNKNOWN) { clUnknownBytesOfs.WriteData(reinterpret_cast<char*>(pucFrameBuffer), stMetaData.uiLength); }
            else { CommonLoggerLogWarn("Framer returned with status code {}", (int32_t)eFramerStatus); }
        }
    }

    // Clean up
    NovatelFramerShutdownLogger(pclFramer);
    NovatelHeaderDecoderShutdownLogger(pclHeaderDecoder);
    NovatelMessageDecoderShutdownLogger(pclMessageDecoder);
    NovatelEncoderShutdownLogger(pclEncoder);
    NovatelFilterShutdownLogger(pclFilter);
    CommonLoggerShutdownLogger();

    NovatelFramerDelete(pclFramer);
    NovatelHeaderDecoderDelete(pclHeaderDecoder);
    NovatelMessageDecoderDelete(pclMessageDecoder);
    NovatelEncoderDelete(pclEncoder);
    NovatelFilterDelete(pclFilter);
    return 0;
}
