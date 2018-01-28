#ifndef UNI_PORTABLE_TYPES_H_
#define UNI_PORTABLE_TYPES_H_
/** \file UNI_types.h
 * \brief Definitions for the types used in the Universal API methods.
 *
 * This file describes all necessary type definitions for using
 * methods of AVT's Universal API. These type definitions are designed to be
 * portable in the sense of Microsoft, means: also usable in VB and C#.
 *
 * \par license
 * This code is the property of Allied Vision Technologies. Nevertheless, you
 * are absolutely free to use and modify it. The code is provided "as is" with
 * no expressed or implied warranty. The author and AVT accept no liability if
 * it causes any damage to your computer.
 *
 */

/** @name Basic types used to define the types really used
Don't use these typedefs directly!
*/
/*@{*/


#ifdef _MSC_VER

//! 16 bit signed integer on Microsoft systems
typedef __int16             int16_t;

//! 32 bit signed integer on Microsoft systems
typedef __int32             int32_t; //should be taken; right now, it must be "long"
//typedef long                int32_t;

//! 64 bit signed integer on Microsoft systems
typedef __int64             int64_t;

//! 16 bit unsigned integer on Microsoft systems
typedef unsigned __int16    uint16_t;

//! 32 bit unsigned integer on Microsoft systems
typedef unsigned __int32    uint32_t; //should be used; right now, it must be "unsigned long"
//typedef unsigned long       uint32_t;

//! 64 bit unsigned integer on Microsoft systems
typedef unsigned __int64    uint64_t;

#else // for non MS compilers without any warranty for the size
#pragma message("Compatibility Warning: the typedefs in " __FILE__ " may not have the right number of bits ")

//! 16 bit signed integer on non-Microsoft systems
typedef short               int16_t;

//    typedef int                 int32_t; should be taken; right now, it must be "long"
//! 32 bit signed integer on non-Microsoft systems
typedef int                 int32_t;

//! 64 bit signed integer on non-Microsoft systems
typedef long long           int64_t;

//! 16 bit unsigned integer on non-Microsoft systems
typedef unsigned short      uint16_t;

//    typedef unsigned int        uint32_t; should be used; right now, it must be "unsigned long"
//! 32 bit unsigned integer on non-Microsoft systems
typedef unsigned int        uint32_t;

//! 64 bit unsigned integer on non-Microsoft systems
typedef unsigned long long  uint64_t;

#endif

/*@}*/
////////////////////////////////////////////////////////////////////////////////////////////

/** @name Portable types.
Use these types.
*/
/*@{*/

typedef signed char         INT8_TYPE;                      //!< 8 bit signed integer
typedef signed char         INT8_IN_TYPE;                   //!< 8 bit signed integer type for input
typedef INT8_IN_TYPE      * INT8_INOUT_TYPE;                //!< 8 bit signed integer type for input and output
typedef INT8_IN_TYPE     *  INT8_OUT_TYPE;                  //!< 8 bit signed integer type for output

typedef int16_t             INT16_TYPE;                     //!< 16 bit signed integer
typedef int16_t             INT16_IN_TYPE;                  //!< 16 bit signed integer type for input
typedef INT16_IN_TYPE  *    INT16_INOUT_TYPE;               //!< 16 bit signed integer type for input and output
typedef INT16_IN_TYPE  *    INT16_OUT_TYPE;                 //!< 16 bit signed integer type for output

typedef int32_t             INT32_TYPE;                     //!< 32 bit signed integer
typedef int32_t             INT32_IN_TYPE;                  //!< 32 bit signed integer type for input
typedef INT32_IN_TYPE  *    INT32_INOUT_TYPE;               //!< 32 bit signed integer type for input and output
typedef INT32_IN_TYPE  *    INT32_OUT_TYPE;                 //!< 32 bit signed integer type for output

typedef int64_t             INT64_TYPE;                     //!< 64 bit signed integer
typedef int64_t             INT64_IN_TYPE;                  //!< 64 bit signed integer type for input
typedef INT64_IN_TYPE  *    INT64_INOUT_TYPE;               //!< 64 bit signed integer type for input and output
typedef INT64_IN_TYPE  *    INT64_OUT_TYPE;                 //!< 64 bit signed integer type for output

typedef unsigned char       UINT8_TYPE;                     //!< 8 bit unsigned integer
typedef unsigned char       UINT8_IN_TYPE;                  //!< 8 bit unsigned integer type for input
typedef UINT8_IN_TYPE  *    UINT8_INOUT_TYPE;               //!< 8 bit unsigned integer type for input and output
typedef UINT8_IN_TYPE  *    UINT8_OUT_TYPE;                 //!< 8 bit unsigned integer type for output

typedef uint16_t            UINT16_TYPE;                    //!< 16 bit unsigned integer
typedef uint16_t            UINT16_IN_TYPE;                 //!< 16 bit unsigned integer type for input
typedef UINT16_IN_TYPE *    UINT16_INOUT_TYPE;              //!< 16 bit unsigned integer type for input and output
typedef UINT16_IN_TYPE *    UINT16_OUT_TYPE;                //!< 16 bit unsigned integer type for output

typedef uint32_t            UINT32_TYPE;                    //!< 32 bit unsigned integer
typedef uint32_t            UINT32_IN_TYPE;                 //!< 32 bit unsigned integer type for input
typedef UINT32_IN_TYPE *    UINT32_INOUT_TYPE;              //!< 32 bit unsigned integer type for input and output
typedef UINT32_IN_TYPE *    UINT32_OUT_TYPE;                //!< 32 bit unsigned integer type for output

typedef uint64_t            UINT64_TYPE;                    //!< 64 bit unsigned integer
typedef uint64_t            UINT64_IN_TYPE;                 //!< 64 bit unsigned integer type for input
typedef UINT64_IN_TYPE *    UINT64_INOUT_TYPE;              //!< 64 bit unsigned integer type for input and output
typedef UINT64_IN_TYPE *    UINT64_OUT_TYPE;                //!< 64 bit unsigned integer type for output

typedef float               FLOAT_TYPE;                     //!< 4 byte floating point
typedef float               FLOAT_IN_TYPE;                  //!< 4 byte floating point type for input
typedef FLOAT_IN_TYPE  *    FLOAT_INOUT_TYPE;               //!< 4 byte floating point type for input and output
typedef FLOAT_IN_TYPE  *    FLOAT_OUT_TYPE;                 //!< 4 byte floating point type for  output

typedef double              DOUBLE_TYPE;                    //!< 8 byte floating point
typedef double              DOUBLE_IN_TYPE;                 //!< 8 byte floating point type for input
typedef DOUBLE_IN_TYPE *    DOUBLE_INOUT_TYPE;              //!< 8 byte floating point type for input and output
typedef DOUBLE_IN_TYPE *    DOUBLE_OUT_TYPE;                //!< 8 byte floating point type for output

#ifdef __cplusplus
typedef bool                BOOL_TYPE;                      //!< "boolean" type
#else
typedef int                 BOOL_TYPE;                      //!< "boolean" type
#endif

typedef UINT32_IN_TYPE      BOOL_IN_TYPE;                   //!< "boolean" type for input
typedef BOOL_IN_TYPE   *    BOOL_INOUT_TYPE;                //!< "boolean" type for input and output
typedef BOOL_IN_TYPE   *    BOOL_OUT_TYPE;                  //!< "boolean" type for output

typedef void                VOID_TYPE;                      //!< "void" type
typedef VOID_TYPE      *    VOID_POINTER;                   //!< pointer to "void" type

#ifdef _SIZE_T_DEFINED                                      // if we have size_t we use it for the size of a pointer
    typedef size_t          POINTER_SIZE;                   //!< size of a pointer; system dependent
#else
#   if defined(_X86_) ||defined(__X86__) || defined(_M_IX86)  // else we use 32 bit on X86
        typedef uint32_t    POINTER_SIZE;                   //!< size of a pointer system dependent
#   else                                                      // or 64 bit everywhere else
        typedef uint64_t    POINTER_SIZE;                   //!< size of a pointer system dependent
#   endif
#endif

/*@}*/
////////////////////////////////////////////////////////////////////////////////////////////

/** @name Character types.
Types for character handling.
*/
/*@{*/
#ifndef _WCHAR_T_DEFINED
        typedef unsigned short wchar_t;       //!< if not defined wchar_t define it
#endif

#ifdef _UNICODE
    typedef wchar_t UNICODE_CHAR;       //!< char type dependent on the UNICODE define multi byte or single byte
#else
    typedef char UNICODE_CHAR; //!< char type dependent on the UNICODE define multi byte or single byte
#endif

typedef UNICODE_CHAR    CHAR_TYPE;                          //!< string type will change with UNICODE builds from char to wchar_t
typedef UNICODE_CHAR    CHAR_IN_TYPE;                       //!< string type will change with UNICODE builds from char to wchar_t
typedef CHAR_TYPE *     CHAR_INOUT_TYPE;                    //!< a valid pointer to a mutable char/wchar_t as, will take values into a  function and return the result
typedef CHAR_TYPE *     CHAR_OUT_TYPE;                      //!< a valid pointer to a char/wchar_t, will return results from a function
typedef wchar_t         WCHAR_TYPE;                         //!< string with wide char
typedef wchar_t         WCHAR_IN_TYPE;                      //!< string with wide char
typedef WCHAR_TYPE *    WCHAR_INOUT_TYPE;                   //!< valid pointer to a mutable wchar_t, will take values into a function and return the ressult.
typedef WCHAR_TYPE *    WCHAR_OUT_TYPE;                     //!< valid pointer to a wchar_t, will return results from a function
typedef char            ACHAR_TYPE;                         //!< string of 8bit ASCII chars
typedef char            ACHAR_IN_TYPE;                      //!< string of 8bit ASCII chars
typedef ACHAR_TYPE *    ACHAR_INOUT_TYPE;                   //!< 8bit ASCII char string taking values int a function and returning a result
typedef ACHAR_TYPE *    ACHAR_OUT_TYPE;                     //!< 8bit ASCII string returning a result from a function
/*@}*/
////////////////////////////////////////////////////////////////////////////////////////////

/** @name Special types.
Types for special purposes.
*/
/*@{*/

#ifndef HRESULT
//#define _HRESULT_DEFINED
//! Definition just in case HRESULT is not defined (and winnt.h should not be included)
typedef long HRESULT;
#endif

//! Return value for all the methods of the API
typedef HRESULT         UNI_RETURN_TYPE;

//! Type for identifying a camera
typedef UINT32_IN_TYPE  ID_TYPE;

//! A handle for a color correction object to be used during debayering
typedef void          * COLOR_CORRECTION_HANDLE;

/*@}*/

/// \cond UNI_CONTROL 
#ifndef UNI_TRANSFORM

#ifndef __1394GUID
//! The definition of a 1394 GUID
typedef UINT64_TYPE     __1394GUID;
#endif

//! Type for an enumeration of presets
typedef void*           PRESETS_ENUMERATION;

//! Type for an enumeration of color codes
typedef void*           COLORCODE_ENUMERATION;

#endif // !UNI_TRANSFORM
/// \endcond

#endif//UNI_PORTABLE_TYPES_H_