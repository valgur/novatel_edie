////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2020 NovAtel Inc.
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
////////////////////////////////////////////////////////////////////////////////
//
//  DESCRIPTION:
//    Class to provide decoder functionality
//
////////////////////////////////////////////////////////////////////////////////
#include "stream_interface.hpp"

InputFileStream* ifs_init(const char* file)
{
   return new InputFileStream(file);
}

void is_read(InputStreamInterface* ifs, StreamReadStatus* srs, char* databuffer, int& iSize)
{
   ReadDataStructure data;
   data.cData = databuffer;
   data.uiDataSize = iSize;
   // read data from input stream
   *srs = ifs->ReadData(data);
   iSize = data.uiDataSize;
}

void ifs_read(InputFileStream* ifs, StreamReadStatus* srs, char* databuffer, int& iSize)
{
   is_read(ifs, srs, databuffer, iSize);
}

void is_del(InputStreamInterface* pStream)
{
   delete pStream;
}

class InputStreamWrapper final : public InputStreamInterface
{
public:
   explicit InputStreamWrapper(void* context, read_callback_t readCallback)
      : m_pContext(context), m_pfnReadCallback(readCallback)
   {
   }

   StreamReadStatus ReadData(ReadDataStructure& pReadDataStructure) override
   {
      UINT count = m_pfnReadCallback(m_pContext, pReadDataStructure.cData, pReadDataStructure.uiDataSize);
      pReadDataStructure.uiDataSize = count;
      m_stFileReadStatus.uiCurrentStreamRead = count;     // Current read byte count
      m_stFileReadStatus.uiPercentStreamRead = 100;       // Total read percentage
      m_stFileReadStatus.ullStreamLength += count;        // Total File Length (in Bytes)
      m_stFileReadStatus.bEOS = count == 0;               // End of Stream
      return m_stFileReadStatus;
   }

private:
   void* m_pContext = nullptr;
   read_callback_t m_pfnReadCallback = nullptr;
   StreamReadStatus m_stFileReadStatus{};
};

InputStreamInterface* is_wrapper(void* context, read_callback_t readCallback)
{
   return new InputStreamWrapper(context, readCallback);
}
