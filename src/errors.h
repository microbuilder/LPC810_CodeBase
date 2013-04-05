/**************************************************************************/
/*!
    @file     errors.h
    @author   K. Townsend
    @brief    Global error codes for centralised error handling
*/
/**************************************************************************/
#ifndef _ERRORS_H_
#define _ERRORS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/*!
    Common error messages used across the system
*/
/**************************************************************************/
typedef enum
{
  ERROR_NONE                    = 0x0,      /**< Indicates no error occurred */
  ERROR_OPERATIONTIMEDOUT       = 0x1,      /**< Operation timed out before completion */
  ERROR_INVALIDPARAMETER        = 0x2,      /**< An invalid parameter value was provided */
  ERROR_DEVICENOTINITIALISED    = 0x3       /**< Attemping to execute a function on an unitialised peripheral */
} error_t;

#ifdef __cplusplus
}
#endif

#endif
