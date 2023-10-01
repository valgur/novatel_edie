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
// ! \file dynamic_parser.cpp
// ===============================================================================

#include <chrono>

#include <edie/decoders/dynamic_library/common_json_reader.hpp>
#include <edie/decoders/dynamic_library/novatel_filter.hpp>
#include <edie/decoders/dynamic_library/novatel_parser.hpp>
#include <edie/hw_interface/stream_interface/inputfilestream.hpp>
#include <edie/hw_interface/stream_interface/outputfilestream.hpp>
#include <edie/version.h>

using namespace novatel::edie;
using namespace novatel::edie::oem;

inline bool FileExists(const std::string& strName_)
{
    struct stat buffer;
    return stat(strName_.c_str(), &buffer) == 0;
}

int main(int argc, char* argv[])
{
    // This example uses the default logger config, but you can also pass a config file to InitLogger()
    // Example config file: logger\example_logger_config.toml
    Logger::InitLogger();
    std::shared_ptr<spdlog::logger> pclLogger = Logger::RegisterLogger("dynamic_parser");
    pclLogger->set_level(spdlog::level::debug);
    Logger::AddConsoleLogging(pclLogger);
    Logger::AddRotatingFileLogger(pclLogger);

    // Get command line arguments
    pclLogger->info("Decoder library information:\n{}", caPrettyPrint);

    std::string sEncodeFormat = "ASCII";
    if (argc == 2 && strcmp(argv[1], "-V") == 0) { return 0; }
    if (argc < 3)
    {
        pclLogger->error("ERROR: Need to specify a JSON message definitions DB, an input file and an output format.");
        pclLogger->error("Example: converter <path to Json DB> <path to input file> <output format>");
        return 1;
    }
    if (argc == 4) { sEncodeFormat = argv[3]; }

    // Check command line arguments
    std::string sJsonDb = argv[1];
    if (!FileExists(sJsonDb))
    {
        pclLogger->error("File \"{}\" does not exist", sJsonDb);
        return 1;
    }

    std::string sInFilename = argv[2];
    if (!FileExists(sInFilename))
    {
        pclLogger->error("File \"{}\" does not exist", sInFilename);
        return 1;
    }

    ENCODE_FORMAT eEncodeFormat = StringToEncodeFormat(sEncodeFormat);
    if (eEncodeFormat == ENCODE_FORMAT::UNSPECIFIED)
    {
        pclLogger->error("Unspecified output format.\n\tASCII\n\tBINARY\n\tFLATTENED_BINARY");
        return 1;
    }

    pclLogger->info("Loading Database...");
    auto tStart = std::chrono::high_resolution_clock::now();
    JsonReader* pclJsonDb = CommonJsonReaderInit();
    CommonJsonReaderLoadFile(pclJsonDb, sJsonDb.c_str());
    pclLogger->info("Done in {}ms",
                    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tStart).count());

    // Set up timers
    auto tLoop = std::chrono::high_resolution_clock::now();
    uint32_t uiCounter = 0;

    // Set up the EDIE components
    Parser* pclParser = NovatelParserInit(pclJsonDb);
    pclParser->SetEncodeFormat(eEncodeFormat);

    Filter* pclFilter = NovatelFilterInit();
    NovatelFilterSetLoggerLevel(pclFilter, spdlog::level::debug);

    // Initialize structures and error codes
    auto eStatus = STATUS::UNKNOWN;

    MetaDataStruct stMetaData;
    MessageDataStruct stMessageData;

    NovatelParserSetFilter(pclParser, pclFilter);

    // Initialize FS structures and buffers
    StreamReadStatus stReadStatus;
    ReadDataStructure stReadData;
    unsigned char acIfsReadBuffer[MAX_ASCII_MESSAGE_LENGTH];
    stReadData.cData = reinterpret_cast<char*>(acIfsReadBuffer);
    stReadData.uiDataSize = sizeof(acIfsReadBuffer);

    // Set up file streams
    InputFileStream clIfs(sInFilename.c_str());
    OutputFileStream clConvertedLogsOfs(sInFilename.append(".").append(sEncodeFormat).c_str());
    OutputFileStream clUnknownBytesOfs(sInFilename.append(".UNKNOWN").c_str());

    uint32_t uiCompleteMessages = 0;
    tStart = std::chrono::high_resolution_clock::now();
    tLoop = std::chrono::high_resolution_clock::now();
    while (!stReadStatus.bEOS)
    {
        stReadData.cData = reinterpret_cast<char*>(acIfsReadBuffer);
        stReadStatus = clIfs.ReadData(stReadData);
        NovatelParserWrite(pclParser, reinterpret_cast<unsigned char*>(stReadData.cData), stReadStatus.uiCurrentStreamRead);

        do {
            eStatus = NovatelParserRead(pclParser, &stMessageData, &stMetaData);

            if (eStatus == STATUS::SUCCESS)
            {
                clConvertedLogsOfs.WriteData(reinterpret_cast<char*>(stMessageData.pucMessage), stMessageData.uiMessageLength);
                stMessageData.pucMessage[stMessageData.uiMessageLength] = '\0';
                pclLogger->info("Encoded: ({}) {}", stMessageData.uiMessageLength, reinterpret_cast<char*>(stMessageData.pucMessage));
                uiCompleteMessages++;
            }

            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tLoop).count() > 1000)
            {
                uiCounter++;
                pclLogger->info("{} logs/s", uiCompleteMessages / uiCounter);
                tLoop = std::chrono::high_resolution_clock::now();
            }
        } while (eStatus != STATUS::BUFFER_EMPTY);
    }

    pclLogger->info("Converted {} logs in {}ms from {}", uiCompleteMessages,
                    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - tStart).count(),
                    sInFilename.c_str());
    Logger::Shutdown();
    NovatelParserDelete(pclParser);
    return 0;
}
