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
// ! \file outputfilestream.cpp
// ===============================================================================

#include "outputfilestream.hpp"

// ---------------------------------------------------------
OutputFileStream::OutputFileStream(const std::u32string& s32FileName)
{
    pOutFileStream = new FileStream(s32FileName);
    pOutFileStream->OpenFile(FileStream::FILE_MODES::OUTPUT);
}

// ---------------------------------------------------------
OutputFileStream::OutputFileStream(const char* pcFileName)
{
    pOutFileStream = new FileStream(pcFileName);
    pOutFileStream->OpenFile(FileStream::FILE_MODES::OUTPUT);
}

// ---------------------------------------------------------
OutputFileStream::~OutputFileStream()
{
    pOutFileStream->CloseFile();
    delete pOutFileStream;
}

// ---------------------------------------------------------
uint32_t OutputFileStream::WriteData(char* cData, uint32_t uiSize) { return pOutFileStream->WriteFile(cData, uiSize); }
