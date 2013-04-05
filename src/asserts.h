/**************************************************************************/
/*!
    @file     asserts.h
    @author   K. Townsend
    @brief    Various ASSERT macros to simplify error checking, and
              centralise error logging when debugging.
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
