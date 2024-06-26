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
// ! \file common.h
// ===============================================================================

#ifndef HW_INTERFACE_C_API_COMMON_H
#define HW_INTERFACE_C_API_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EDIE_DONT_REDEFINE_ENUMS
    typedef enum
    {
        SPLIT_SIZE, /*!< Split based on Size of log */
        SPLIT_LOG,  /*!< Split based on Log name */
        SPLIT_TIME, /*!< Split based on time of log */
        SPLIT_NONE  /*!< Do not split */
    } FileSplitMethodEnum;
#endif

    typedef struct ReadDataStructure ReadDataStructure;
    typedef struct StreamReadStatus StreamReadStatus;

#ifdef __cplusplus
}
#endif

#endif // HW_INTERFACE_C_API_COMMON_H