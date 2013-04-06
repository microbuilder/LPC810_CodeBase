/**************************************************************************/
/*!
    @file     asserts.h
    @author   K. Townsend
    @brief    Various ASSERT macros to simplify error checking, and
              centralise error logging when debugging.

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _ASSERTS_H_
#define _ASSERTS_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
  #define _PRINTF(...)      printf(__VA_ARGS__)
#else
  #define _PRINTF(...)
#endif

/*! Compiler specific macro returning a string containing the current line number */
#define ASSERT_LINE __LINE__
/*! Compiler specific macro returning a integer containing the current file name */
#define ASSERT_FILE __FILE__
/*! Compiler specific macro returning a string containing the current function */
#define ASSERT_FUNC __func__

/**************************************************************************/
/*!
    @brief  This macro will assert the test condition and return the
            specified returnValue, as well as the specified 'message'
            string in the \ref ASSERT output
*/
/**************************************************************************/
#define ASSERT_MESSAGE(condition, returnValue, message) \
        do{\
          if (!(condition)) {\
            _PRINTF("Assert: %s at line %d: %s\n", ASSERT_FUNC, ASSERT_LINE, message);\
            return (returnValue);\
          }\
        }while(0)

/**************************************************************************/
/*!
    @brief Checks the condition, and if the assert fails the supplied
           returnValue will be returned in the calling function.
*/
/**************************************************************************/
#define ASSERT(condition, returnValue)  ASSERT_MESSAGE(condition, returnValue, NULL)

/**************************************************************************/
/*!
    @brief  Checks the supplied \ref error_t value (sts), and if it is
            not equal to \ref ERROR_NONE the sts value will be returned
            and the supplied error message will be sent via _PRINTF.

    @details
    This macro is useful to check if a function returned an error without
    bloating your own code with endless "if (error) {...}".
*/
/**************************************************************************/
#define ASSERT_STATUS_MESSAGE(sts, message) \
        do{\
          error_t status = (sts);\
          if (ERROR_NONE != status) {\
            _PRINTF("Assert: %s at line %d: Error 0x%04X - %s\n", ASSERT_FUNC, ASSERT_LINE, (uint32_t) status, message);\
            /* return status;*/\
          }\
        } while(0)

/**************************************************************************/
/*!
    @brief  Checks the supplied \ref error_t value (sts), and if it is
            not equal to \ref ERROR_NONE the sts value will be returned.

    @details
    This macro is useful to check if a function returned an error without
    bloating your own code with endless "if (error) {...}".
*/
/**************************************************************************/
#define ASSERT_STATUS(sts)                ASSERT_STATUS_MESSAGE(sts, NULL)

#ifdef __cplusplus
}
#endif

#endif
