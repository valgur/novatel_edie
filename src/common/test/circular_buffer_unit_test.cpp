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
// ! \file circular_buffer_unit_test.cpp
// ===============================================================================

#include <gtest/gtest.h>

#include "novatel_edie/common/circular_buffer.hpp"

// -------------------------------------------------------------------------------------------------------
// CircularBuffer Unit Tests
// -------------------------------------------------------------------------------------------------------
TEST(CircularBufferTest, SET_CAPACITY)
{
    uint32_t uiCapacity = 100;
    CircularBuffer cCircularBuffer;
    cCircularBuffer.SetCapacity(uiCapacity);
    ASSERT_DOUBLE_EQ(cCircularBuffer.GetCapacity(), uiCapacity);

    cCircularBuffer.SetCapacity(0);
    ASSERT_DOUBLE_EQ(cCircularBuffer.GetCapacity(), uiCapacity);

    try
    {
        // Allocate huge amount of memory
        cCircularBuffer.SetCapacity(INT32_MAX);
        ASSERT_EQ(0, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

TEST(CircularBufferTest, APPEND)
{
    const auto* pcData = "test data";
    auto uiBytes = static_cast<uint32_t>(strlen(pcData));
    CircularBuffer cCircularBuffer;
    ASSERT_DOUBLE_EQ(cCircularBuffer.Append(reinterpret_cast<const unsigned char*>(pcData), uiBytes), uiBytes);
}

TEST(CircularBufferTest, GET_BYTE)
{
    const auto* pcData = "test data";
    auto uiBytes = static_cast<uint32_t>(strlen(pcData));

    CircularBuffer cCircularBuffer;
    cCircularBuffer.Append(reinterpret_cast<const unsigned char*>(pcData), uiBytes);

    ASSERT_EQ(cCircularBuffer.GetByte(0), 't');
    ASSERT_EQ(cCircularBuffer.GetByte(uiBytes - 1), 'a');

    ASSERT_EQ(cCircularBuffer.GetByte(uiBytes + 1), '\0');
    ASSERT_EQ(cCircularBuffer.GetByte(-1), '\0');

    cCircularBuffer.Discard(4);
    ASSERT_EQ(cCircularBuffer.GetByte(uiBytes), '\0');
}

TEST(CircularBufferTest, DISCARD)
{
    const auto* pcData = "test data";
    auto uiBytes = static_cast<uint32_t>(strlen(pcData));
    CircularBuffer cCircularBuffer;
    cCircularBuffer.Append(reinterpret_cast<const unsigned char*>(pcData), uiBytes);
    cCircularBuffer.Discard(1);
    ASSERT_TRUE(cCircularBuffer.GetLength() == uiBytes - 1);
    cCircularBuffer.Discard(1);
    ASSERT_TRUE(cCircularBuffer.GetLength() == uiBytes - 2);
    cCircularBuffer.Discard(uiBytes - 2);
    ASSERT_TRUE(cCircularBuffer.GetLength() == 0);
    cCircularBuffer.Discard(1);
    ASSERT_TRUE(cCircularBuffer.GetLength() == 0);
    cCircularBuffer.Append(reinterpret_cast<const unsigned char*>(pcData), uiBytes);
    ASSERT_TRUE(cCircularBuffer.GetLength() == uiBytes);
    cCircularBuffer.Discard(uiBytes + 1);
}

TEST(CircularBufferTest, CLEAR)
{
    std::string pcData("test data");
    CircularBuffer cCircularBuffer;
    cCircularBuffer.Append(reinterpret_cast<const unsigned char*>(pcData.c_str()), static_cast<uint32_t>(pcData.length()));
    cCircularBuffer.Clear();
    ASSERT_TRUE(cCircularBuffer.GetLength() == 0);
}

TEST(CircularBufferTest, COPY)
{
    CircularBuffer cCircularBuffer;

    const auto* pcData = "test data";
    auto uiBytes = static_cast<uint32_t>(strlen(pcData));
    cCircularBuffer.Append(reinterpret_cast<const unsigned char*>(pcData), uiBytes);

    auto pcData_ = std::make_unique<char[]>(uiBytes + 1); // more than uiBytes
    auto uiBytes_ = cCircularBuffer.Copy(reinterpret_cast<unsigned char*>(pcData_.get()), uiBytes + 1);
    ASSERT_EQ(uiBytes_, uiBytes);
}
