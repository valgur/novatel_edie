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
// ! \file outputstreaminterface.hpp
// ===============================================================================

#ifndef OUTPUT_STREAM_INTERFACE_HPP
#define OUTPUT_STREAM_INTERFACE_HPP

#include <string>

//-----------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------
class MemoryStream;

/*! \class OutputStreamInterface
 *  \brief An Interface Class used by application to write data to the output.
 *  \details Output stream is an interface for writing data to file and port handles.
 *  Output Stream has three derived classes to support file and port access, OutputFileStream,
 * OutputPortStream and MultiOutputFileStream. The ideal usage of Output Stream is to support
 * Encoder to write the converted data to given stream object.
 */
class OutputStreamInterface
{
  public:
    /** A virtual member.
     *  \brief Write data to the buffer
     *  \param[in] pcData_ Buffer pointer.
     *  \param[in] uiDataLength_ size of the buffer.
     *  \return Number of bytes written to output file.
     *  \remark Set Split type and write data to output files. If split type was not set,
     *  Then writing can be done to only one file.
     */
    virtual uint32_t WriteData(const char* pcData_, uint32_t uiDataLength_) = 0;

    /*! a virtual destructor. */
    virtual ~OutputStreamInterface() = default;

    /** A virtual member.
     *  \brief Sets the output file in which to be decoded output will be written.
     *  \remark FileStream Object will be created and added to map.
     *  If already created, The file with name stFileName will be set for writing.
     */
    virtual void SelectFileStream(const std::string&) {}

    /** A virtual member.
     *  \brief Enable/Disable Splitting of logs into different output files.
     *  \remark Enable/Disable log Splitting. If enabled, split type will be set to SPLIT_LOG
     *  If disabled split type will be set to SPLIT_NONE
     */
    virtual void ConfigureSplitByLog(bool) {}

    /** A virtual member.
     *  \brief Gets base file name and extension of it.
     *  \remark Sets Base file name (before '.' in file name)
     *          Sets Extension of the file.
     */
    virtual void ConfigureBaseFileName(const std::string&) {}

    /** A virtual member.
     *  \brief Split file into different output file with defined size.
     *  \remark Output files with ullFileSplitSize size will be created while writing to the output.
     */
    virtual void ConfigureSplitBySize(uint64_t) {}

    /** A virtual member.
     *  \brief Sets the interval of time the file to be split.
     *  \remark Different output files will be created with the logs,
     *  in which will be captured in the time interval provided.
     */
    virtual void ConfigureSplitByTime(double) {}

    /** A virtual member.
     * \return MemoryStream Object which had buffer.
     */
    virtual MemoryStream* GetMemoryStream() { return nullptr; }

    // #ifdef WIDE_CHAR_SUPPORT
    /** A virtual member.
     *  \brief Sets the output file in which to be decoded output will be written.
     *  \remark FileStream Object will be created and added to map.
     *  If already created, The file with name stFileName will be set for writing.
     */
    virtual void SelectFileStream(const std::u32string&) {}

    /** A virtual member
     *  \brief Gets wide character base file name and extension of it.
     *  \remark Sets Base file name (before '.' in file name)
     *          Sets Extension of the file.
     */
    virtual void ConfigureBaseFileName(const std::u32string&) {}
    // #endif

  private:
};

#endif
