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
// ! \file outputfilestreamunittest.cpp
// ===============================================================================

#include <filesystem>
#include <string>

#include <gtest/gtest.h>

#include "edie/hw_interface/stream_interface/outputfilestream.hpp"

class OutputFileStreamTest : public ::testing::Test
{
  public:
    void SetUp() override {}

    void TearDown() override {}

  private:
  protected:
};

// Constructor1
TEST_F(OutputFileStreamTest, Constructor1)
{
    OutputFileStream* pMyTestCommand = NULL;
    pMyTestCommand = new OutputFileStream((std::filesystem::path(std::getenv("TEST_RESOURCE_PATH")) / "outputfilestream_file1.asc").string().c_str());
    ASSERT_TRUE(pMyTestCommand->pOutFileStream != NULL);
    delete pMyTestCommand;
}

// Constructor Wide Char
TEST_F(OutputFileStreamTest, ConstructorWideChar)
{
    std::cout << "In Output Stream Test, Constructor WC" << std::endl;
    OutputFileStream* pMyTestCommand = NULL;
    pMyTestCommand =
        new OutputFileStream(std::u32string((std::filesystem::path(std::getenv("TEST_RESOURCE_PATH")) / U"不同语言的文件.gps").generic_u32string()));
    ASSERT_TRUE(pMyTestCommand->pOutFileStream != NULL);
    delete pMyTestCommand;
    std::cout << "Made it past ASSERT and Delete. Output Stream Test, Constructor WC" << std::endl;
}
