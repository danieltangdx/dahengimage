#ifndef UNI_TRANSFORM_H_
#define UNI_TRANSFORM_H_
#ifndef UNI_TRANSFORM
#define UNI_TRANSFORM
#endif
#pragma once
#pragma warning (disable: 4635)     // badly-formed XML

/** \file UniTransform.h
* \brief The available AVT Universal API Image Transformation methods.
*
* All the AVT Universal API methods for image transformation publicly available are described in this part of
*  the help. For a description see the section "\link intro Introduction
*  \endlink " of this help.
*
* \version 2.0.1
*
* \date 30-Oct-2009
*
* \author Holger Eddelbuettel, Matthias Heidenreich, Olaf Reuter
*
* \par license
* This code is the property of Allied Vision Technologies. Nevertheless, you
* are absolutely free to use and modify it. The code is provided "as is" with
* no expressed or implied warranty. The author and AVT accept no liability if
* it causes any damage to your computer.
*
*/
#include "uni_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UIT_NO_EXPORT
#   ifdef UIT_EXPORTS
#       ifndef WIN32DLL_API
#           ifndef _WIN64
#               define WIN32DLL_API __declspec(dllexport) __stdcall
#           else
#               define WIN32DLL_API __stdcall
#           endif
#       endif
#   else
#       ifndef WIN32DLL_API
#           define WIN32DLL_API __declspec(dllimport) __stdcall
#       endif
#   endif
#else
#   ifndef WIN32DLL_API
#       define WIN32DLL_API __stdcall
#   endif
#endif

/////////////////////////////////////////////////////////////////////////////
// Miscellaneous methods
/** @name Miscellaneous methods
*/
/*@{*/
/**
  Get library version.
* \param[out] pnValue  pointer, returns the library version (Major,Minor,SubMinor,Build)
* \return will always return S_OK
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_GetVersion ( UINT32_OUT_TYPE   pnValue );

/**
Get information about processor supported features.
* \param[out] pTechnoInfo returns the supported SIMD technologies
* \note This should be called before using any SIMD (MMX,SSE) optimmized functions
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_GetTechnoInfo( S_TECHNO_INFO *pTechnoInfo);

/**
Extract the Secure Image Signature from a transport image.
\param[in,out]  pSISData    pointer to S_SIS_DATA struct to be filled
\param[in]      pImage      pointer to transport image with embedded SIS
\param[in]      nXSize      width of the image in pixel
\param[in]      nYSize      height of the image in pixel
\param[in]      eColorCode  IIDC color code for transport format, use ::E_UNI_COLOR_CODE
\param[in]      nSISPos     Line number where the SIS is embedded into the image
\param[in]      eSISType    Type of SIS, use ::E_UNI_SIS_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_GetSIS(            S_SIS_DATA      *pSISData,
                                            const   UINT8_TYPE      *pImage,
                                                    UINT32_TYPE     nXSize,
                                                    UINT32_TYPE     nYSize,
                                                    UINT32_TYPE     eColorCode,
                                                    INT32_TYPE      nSISPos,
                                                    UINT32_TYPE     eSISType);
/**
* Get the bits per pixel for the transport image with the given color coding.
*
* \param[in]    ColorCode     The color coding ::E_UNI_COLOR_CODE (0..255)
* \param[out]   BitsPerPixel  The number of bits (8,12,16,24 or 48)
* \return <dl><dt>S_OK</dt><dd>if a known color coding was submitted</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_GetBitsPerPixel ( UINT32_IN_TYPE  ColorCode,
                                                   UINT32_OUT_TYPE BitsPerPixel);

/** Translate ::UNI_RETURN_TYPE into an human readable description.
\param[in]      nErrCode        return code to translate
\param[in,out]  pszInfoString   pointer to string for output
\param[in]      nMaxStrLen        length of the string
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_GetErrorInfo ( UNI_RETURN_TYPE     nErrCode,
                                                char*               pszInfoString,
                                                UINT32_IN_TYPE      nMaxStrLen );

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Format conversions to BGR with simple 2x2 debayering w/o color conversion
/** @name Format conversions to BGR with simple 2x2 debayering w/o color conversion
Simple format conversions from IIDC transfer formats to the Windows RGB format.
*/
/*@{*/
/**
* Convert YUV 4:4:4 data to the Windows RGB format.
*
* \param[out]   pDst        A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc        A pointer to the source image (U Y V U Y V ...)
* \param[in]    nPixelCount The number of pixels to be converted (n>1)
* \return   S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV444ToBGR ( void                *pDst,
                                              const void          *pSrc,
                                              UINT32_IN_TYPE       nPixelCount );

/**
* Convert YUV 4:2:2 data to the Windows RGB format.
*
* \param[out]   pDst        A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc        A pointer to the source image (U Y V Y U Y V Y ...)
* \param[in]    nPixelCount The number of pixels to be converted n>1
* \return   S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV422ToBGR ( void                *pDst,
                                              const void          *pSrc,
                                              UINT32_IN_TYPE       nPixelCount );

/**
* Convert YUV 4:1:1 data to the Windows RGB format.
*
* \param[out]   pDst        A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc        A pointer to the source image (U Y Y V Y Y U Y Y V Y Y ...)
* \param[in]    nPixelCount The number of pixels to be converted n>1
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV411ToBGR ( void                *pDst,
                                               const void          *pSrc,
                                               UINT32_IN_TYPE       nPixelCount );

/**
* Convert RGB8 data to the Windows RGB format.
* 
* In the IIDC RGB format, the R,G and B values appear in inverse order,
* so to be displayed in Windows systems, they have to be swapped.<br>
* Naturally, this method may also be used to convert Windows RGB images
* to RGB images with a different byte order.
*
* \param[out]   pDst        A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc        A pointer to the source image (R G B R G B ...)
* \param[in]    nPixelCount The number of pixels (not: bytes) to be converted
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_RGB8ToBGR ( void              *pDst,
                                             const void        *pSrc,
                                             UINT32_IN_TYPE     nPixelCount );
/**
* Convert a RAW8 image to the Windows RGB format
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]    nXSize           The width of the image in columns
* \param[in]    nYSize           The height of the image in rows
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToBGR ( void                  *pDst,
                                             const UINT8_TYPE      *pSrc,
                                             UINT32_IN_TYPE         nXSize,
                                             UINT32_IN_TYPE         nYSize,
                                             UINT32_IN_TYPE         nBayerPattern );

/** 
* Convert Raw16 (YH YL) to the Windows RGB format.
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* raw images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Raw16ImageTransformEx for all-purpose image conversions
* from Raw16 formats.  
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the raw source image (big endian:RH RL GH GL ...)
* \param[in]    nXSize           The width of the image in columns
* \param[in]    nYSize           The height of the image in rows
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return   S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw16ToBGR ( void                 *pDst,
                                              const void           *pSrc, 
                                              UINT32_IN_TYPE        nXSize,
                                              UINT32_IN_TYPE        nYSize,
                                              UINT32_IN_TYPE        nBayerPattern );

/**
* Convert MONO8 to the Windows RGB format. 
* The Mono8 value is copied to each RGB pixel
* \param[out]   pDst        A pointer to the destination image  ( B G R B G R ...)
* \param[in]    pSrc        A pointer to the source image 
* \param[in]    nPixelCount The number of pixels to be converted
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y8ToBGR ( void                *pDst,
                                            const UINT8_TYPE   *pSrc,
                                            UINT32_IN_TYPE      nPixelCount );

/**
* Convert big endian MONO16 to the Windows RGB format. 
*  The resulting mostly monochrome image format (the R,G and B values are
*  more or less equal to one another) lets you extract at least a 10 bit
*  monochrome image by applying the following formula:
*   I   = 4*G + 2*(R-G) + (B-G)  = 2*R + G + B
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* mono images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Y16ImageTransformEx for all-purpose image conversions
* from Y16 formats. 
*
* \param[out]   pDst        A pointer to the destination image  ( B G R B G R ...)
* \param[in]    pSrc        A pointer to the source image (big endian: YH YL YH YL ...)
* \param[in]    nPixelCount The number of pixels to be converted
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ToBGR ( void               *pDst,
                                            const void         *pSrc,
                                            UINT32_IN_TYPE      nPixelCount);

/**
* Convert big endian MONO16 to the Windows RGB format. 
*  The resulting mostly monochrome image format (the R,G and B values are
*  more or less equal to one another) lets you extract at least a 10 bit
*  monochrome image by applying the following formula:
*   I   = 4*G + 2*(R-G) + (B-G)  = 2*R + G + B
*
* \param[out]   pDst        A pointer to the destination image  ( B G R B G R ...)
* \param[in]    pSrc        A pointer to the source image
* \param[in]    nPixelCount The number of pixels to be converted
* \param[in]    nEndianness The endianness of the image data, use ::E_UNI_ENDIANNESS (big endian: YH YL YH YL ..., little endian: YL YH YL YH ...)
* \param[in]    nAlignment  The alignment of the image data, use ::E_UNI_ALIGNMENT (MSB: pppp pppp pppp ...., LSB: .... pppp pppp pppp)
* \param[in]    nPixelDepth Number of bits per pixel
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ToBGREx ( void               *pDst,
                                              const void         *pSrc,
                                              UINT32_IN_TYPE      nPixelCount,
                                              UINT32_IN_TYPE      nEndianness,
                                              UINT32_IN_TYPE      nAlignment,
                                              UINT32_IN_TYPE      nPixelDepth);

/**
* Convert Raw8 to the Windows RGB format with R=G=B
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]    nXSize           The width of the image in columns
* \param[in]    nYSize           The height of the image in rows
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return   S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToBGR_mono ( UINT8_OUT_TYPE    pDst,
                                                  const UINT8_TYPE *pSrc, 
                                                  UINT32_IN_TYPE    nXSize,
                                                  UINT32_IN_TYPE    nYSize,
                                                  UINT32_IN_TYPE    nBayerPattern );

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// BGR Format conversion methods with more sophisticated debayering w/o color conversion
/** @name Format conversions to BGR format with more sophisticated debayering w/o color conversion
Format conversions from raw color images resulting from a RGGB sensor to the Windows RGB format.
*/
/*@{*/
/**
* Convert Raw8 to the Windows RGB format using a 3x3 interpolation.
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]    nXSize           The width of the image in columns (must be a multiple of 2)
* \param[in]    nYSize           The height of the image in rows   n>1
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
* \note    Source and Destination have to point to different memory locations
* \see     \link debmethods Debayering methods\endlink 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToBGR_3x3 ( void                 * pDst,
                                                 const UINT8_TYPE     * pSrc, 
                                                 UINT32_IN_TYPE         nXSize,
                                                 UINT32_IN_TYPE         nYSize,
                                                 UINT32_IN_TYPE         nBayerPattern);

/**
* Convert a RAW8 image to the Windows RGB format via an interpolated YUV422 format.
*  As the U resp. V value, the average of the U/V values of
*  consecutive pixels is taken
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]    nXSize           The width of the image in columns n>1
* \param[in]    nYSize           The height of the image in rows   n>1
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
* \note    Source and Destination have to point to different memory locations
* \see     \link debmethods Debayering methods \endlink
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToBGR_IntYUV422 ( void            *pDst,
                                                       UINT8_TYPE      *pSrc, 
                                                       UINT32_IN_TYPE   nXSize,
                                                       UINT32_IN_TYPE   nYSize,
                                                       UINT32_IN_TYPE   nBayerPattern);

/**
* Convert a RAW8 image to the Windows RGB format via a stronglier interpolated YUV422 format.
*  As the U resp. V value, the average of the U/V values of four
*  consecutive pixels is taken (indexes -1, 0 1 and 2)
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]    nXSize           The width of the image in columns n>1
* \param[in]    nYSize           The height of the image in rows   n>1
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
* \note    Source and Destination have to point to different memory locations
* \see     \link debmethods Debayering methods\endlink 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToBGR_IntLCAA ( void              *pDst,
                                                     UINT8_TYPE        *pSrc, 
                                                     UINT32_IN_TYPE     nXSize,
                                                     UINT32_IN_TYPE     nYSize,
                                                     UINT32_IN_TYPE     nBayerPattern);


/**
* Convert a RAW8 image to the Windows RGB format via an interpolated
*  YUV422 format.
*  As the U resp. V value, the average of the U/V values of a four by two
*  pixel window is taken (indexes -1, 0 1 and 2 of each row)
*
* \param[out]  pDst             A pointer to the destination image (B G R B G R...)
* \param[in]   pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]   nXSize           The width of the image in columns n>1
* \param[in]   nYSize           The height of the image in rows   n>1
* \param[in]   nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
* \note    Source and Destination have to point to different memory locations
* \see     \link debmethods Debayering methods\endlink 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToBGR_IntLCAAV ( void             *pDst,
                                                      UINT8_TYPE       *pSrc, 
                                                      UINT32_IN_TYPE    nXSize,
                                                      UINT32_IN_TYPE    nYSize,
                                                      UINT32_IN_TYPE    nBayerPattern);

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Format color conversion methods with simple 2x2 debayering with color conversion
/** @name Format conversions to BGR format with color conversion
Format conversions to the Windows RGB with simple 2x2 debayering.
*/
/*@{*/
/**
* Convert a RAW8 image to the Windows RGB format
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]    nXSize           The width of the image in columns (must be a multiple of 2)
* \param[in]    nYSize           The height of the image in rows n>0
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \param[in]    ColCor           A handle to the color conversion object, e.g created 
                                 by ::UIT_CreateColorCorrectionMatrix_Int8. If NULL, no
                                color correction is performed (faster).
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToBGR_CC ( void                   *pDst,
                                                const UINT8_TYPE       *pSrc,
                                                UINT32_IN_TYPE          nXSize,
                                                UINT32_IN_TYPE          nYSize,
                                                UINT32_IN_TYPE          nBayerPattern,
                                                COLOR_CORRECTION_HANDLE ColCor );


/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Mono conversion methods from Mono16 taking the most significant 8 bits with MMX support
/** @name Format conversions
Format conversions to the Windows Mono8 format with and without color conversion.
*/
/*@{*/
/**
* Convert RGB8 data to a monochrome 8BPP image.
* 
*
* \param[out]   pDst        A pointer to the destination image (Y Y Y ...)
* \param[in]    pSrc        A pointer to the source image (R G B R G B ...)
* \param[in]    nPixelCount The number of pixels (not: bytes) to be converted
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_RGB8ToY8 (  UINT8_OUT_TYPE     pDst,
                                             const void        *pSrc,
                                             UINT32_IN_TYPE     nPixelCount );

/**
* Convert big endian MONO16 to a monochrome 8BPP image.
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* mono images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Y16ImageTransformEx for all-purpose image conversions
* from Y16 formats. 
*
* \param[out]   pDst        A pointer to the destination image (Y1 Y2 Y3 Y4 ...)
* \param[in]    pSrc        A pointer to the source image (big endian: YH YL YH YL ...)
* \param[in]    nPixelCount The number of pixels to be converted
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ToY8 ( UINT8_OUT_TYPE       pDst,
                                           const void          *pSrc,
                                           UINT32_IN_TYPE       nPixelCount);

/**
* Convert MONO16 to a monochrome 8BPP image.
*
* \param[out]   pDst        A pointer to the destination image (Y1 Y2 Y3 Y4 ...)
* \param[in]    pSrc        A pointer to the source image
* \param[in]    nPixelCount The number of pixels to be converted
* \param[in]    nEndianness The endianness of the image data, use ::E_UNI_ENDIANNESS (big endian: YH YL YH YL ..., little endian: YL YH YL YH ...)
* \param[in]    nAlignment  The alignment of the image data, use ::E_UNI_ALIGNMENT (MSB: pppp pppp pppp ...., LSB: .... pppp pppp pppp)
* \param[in]    nPixelDepth Number of bits per pixel
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ToY8Ex ( UINT8_OUT_TYPE       pDst,
                                             const void          *pSrc,
                                             UINT32_IN_TYPE       nPixelCount,
                                             UINT32_IN_TYPE       nEndianness,
                                             UINT32_IN_TYPE       nAlignment,
                                             UINT32_IN_TYPE       nPixelDepth);


/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Mono conversion methods from Mono16 taking the most significant 8 bits with MMX support
/** @name Format conversions
Format conversions to the Windows Mono8 format with and without color conversion.
*/
/*@{*/
/**
* Convert big endian MONO16 to a monochrome 8BPP image.
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* mono images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Y16ImageTransformEx for all-purpose image conversions
* from Y16 formats. 
*
* \param[out]   pDst        A pointer to the destination image (Y1 Y2 Y3 Y4 ...)
* \param[in]    pSrc        A pointer to the source image (big endian: YH YL YH YL ...)
* \param[in]    nPixelCount The number of pixels to be converted
* \return S_OK if the operation was successful \see GetTechnoInfo
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ToY8_MMX (  UINT8_OUT_TYPE       pDst,
                                                const void          *pSrc,
                                                UINT32_IN_TYPE       nPixelCount);
/**
* Convert Raw8 to a monochrome 8BPP image.
*
* \param[out]  pDst             A pointer to the destination image (Y1 Y2 Y3 ...)
* \param[in]   pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]   nXSize           The width of the image in columns
* \param[in]   nYSize           The height of the image in rows n>0
* \param[in]   nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
* \note    Source and Destination can point to the same memory locations
* \see     \link debmethods Debayering methods\endlink 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToY8 ( UINT8_OUT_TYPE      pDst,
                                            const UINT8_TYPE   *pSrc, 
                                            UINT32_IN_TYPE      nXSize,
                                            UINT32_IN_TYPE      nYSize,
                                            UINT32_IN_TYPE      nBayerPattern);

/**
* Convert Raw8 to a monochrome 8BPP image with additional color conversion
*
* \param[out]  pDst             A pointer to the destination image (Y1 Y2 Y3 ...)
* \param[in]   pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]   nXSize           The width of the image in columns
* \param[in]   nYSize           The height of the image in rows n>0
* \param[in]   nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \param[in]   ColCor           A handle to the color conversion object, e.g. created 
                               by ::UIT_CreateColorCorrectionMatrix_Int8. If zero, no
                               color correction is performed (faster).
* \return S_OK if the operation was successful 
* \note    Source and Destination can point to the same memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ToY8_CC ( UINT8_OUT_TYPE           pDst,
                                               const UINT8_TYPE         *pSrc, 
                                               UINT32_IN_TYPE           nXSize,
                                               UINT32_IN_TYPE           nYSize,
                                               UINT32_IN_TYPE           nBayerPattern,
                                               COLOR_CORRECTION_HANDLE  ColCor);

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Miscellaneous conversion methods between 12 Bit image with simple 2x2 debayering
/** @name 12 bit format conversions/depacking to 8 or 16 bit, with support of 2x2 debayering, color conversion
Format conversions between 12 bit formats with support of 2x2 debayering, color conversion
*/
/*@{*/
/** Convert a packed 12 bit format to 8 bit data.
    This method may be used to convert RAW12 to RAW8 or MONO12 to MONO8, for example
* \param[out]  pDst             A pointer to the destination image 
* \param[in]   pSrc             A pointer to the binned interlaced image
* \param[in]   nXSize           The width of the image in columns (must be a multiple of 2) 
* \param[in]   nYSize           The height of the image in rows   (must be a multiple of 4) 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Packed12BitTo8Bit ( UINT8_OUT_TYPE    pDst
                                                    , const UINT8_TYPE *pSrc
                                                    , UINT32_TYPE       nXSize
                                                    , UINT32_TYPE       nYSize);

/** Convert a packed 12 bit format to 16 bit data.
    This method may be used to convert RAW12 to RAW16 or MONO12 to MONO16, for example
* \param[out]  pDst             A pointer to the destination image (see ::E
* \param[in]   pSrc             A pointer to the binned interlaced image
* \param[in]   nXSize           The width of the image in columns (must be a multiple of 2) 
* \param[in]   nYSize           The height of the image in rows   (must be a multiple of 4) 
    \note   the 12bit packed data will be output as continuous 16 bit data stream without most significat 4 bit padding
            [p1_8bit][p1_4bit|p2_4bit][p2_8bit]...[p(n)_8bit][p(n)_4bit|p(n+1)_4bit][p(n+1)_8bit]
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Packed12BitTo16Bit ( UINT16_OUT_TYPE    pDst
                                                    , const UINT8_TYPE *pSrc
                                                    , UINT32_TYPE       nXSize
                                                    , UINT32_TYPE       nYSize);
/** 12Bit packed RAW mode to Windows BGR.
    \param[out] pDst            destination windows BGR image buffer
    \param[in]  pSrc            12bit packed source image
    \param[in]  nXSize          image width
    \param[in]  nYSize          image height
    \param[in]  BayerPattern    BayerPattern to debayer the image, use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw12ToBGR         (  void              *pDst
                                                    , const UINT8_TYPE  *pSrc
                                                    , UINT32_TYPE       nXSize
                                                    , UINT32_TYPE       nYSize
                                                    , UINT32_TYPE       BayerPattern);
/** 12Bit packed RAW mode to Windows BGR with color conversion.
    \param[out] pDst            destination windows BGR image buffer
    \param[in]  pSrc            12bit packed source image
    \param[in]  nXSize          image width
    \param[in]  nYSize          image height
    \param[in]  BayerPattern    BayerPattern to debayer the image, use ::E_UNI_BAYER_PATTERN
    \param[in]  ColCor          ColorCorrection used while debayering
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw12ToBGR_CC      ( void              *pDst
                                                    , const UINT8_TYPE  *pSrc
                                                    , UINT32_TYPE       nXSize
                                                    , UINT32_TYPE       nYSize
                                                    , UINT32_TYPE       BayerPattern
                                                    , COLOR_CORRECTION_HANDLE  ColCor);
/** Pike 12Bit Packed to windows 16 BGR.
    \param[out] pDst            destination 16bit BGR image buffer
    \param[in]  pSrc            12bit packed source image
    \param[in]  nXSize          image width
    \param[in]  nYSize          image height
    \param[in]  BayerPattern    BayerPattern to debayer the image, use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw12ToBGR16      (  void              *pDst
                                                    , const UINT8_TYPE  *pSrc
                                                    , UINT32_TYPE       nXSize
                                                    , UINT32_TYPE       nYSize
                                                    , UINT32_TYPE       BayerPattern);
/** Pike 12Bit Packed to windows 16 bit three channel BGR with color conversion.
    \param[out] pDst            destination 16bit BGR image buffer
    \param[in]  pSrc            12bit packed source image
    \param[in]  nXSize          image width
    \param[in]  nYSize          image height
    \param[in]  BayerPattern    BayerPattern to debayer the image, use ::E_UNI_BAYER_PATTERN
    \param[in]  ColCor          ColorCorrection used while debayering
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw12ToBGR16_CC      ( void              *pDst
                                                    , const UINT8_TYPE  *pSrc
                                                    , UINT32_TYPE       nXSize
                                                    , UINT32_TYPE       nYSize
                                                    , UINT32_TYPE       BayerPattern
                                                    , COLOR_CORRECTION_HANDLE  ColCor);
/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Miscellaneous conversion methods between 16 Bit image with simple 2x2 debayering
/** @name 16 bit format conversions with 2x2 debayering, some with color conversion
Format conversions between 16 bit formats with simple 2x2 debayering, but some with color conversion.
*/
/*@{*/
/**
* Convert Raw16 (YH YL) to a 16 bit RGB format (big endian).
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* raw images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Raw16ImageTransformEx for all-purpose image conversions
* from Raw16 formats. 
*
* \param[out]   pDst            A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc            A pointer to the raw source image (big endian:RH RL GH GL ...)
* \param[in]    nXSize          The width of the image in columns
* \param[in]    nYSize          The height of the image in rows n>0
* \param[in]    nBayerPattern   The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \param[in]    nShiftBits      The number of bits the result should be shifted to the right (0<=nShiftBits<=8)
* \return   S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw16ToBGR16 ( void               *pDst,
                                                const void         *pSrc, 
                                                UINT32_IN_TYPE      nXSize,
                                                UINT32_IN_TYPE      nYSize,
                                                UINT32_IN_TYPE      nBayerPattern,
                                                UINT32_IN_TYPE      nShiftBits );

/**
* Convert Raw16 (YH YL) to a 16 bit RGB format (big endian) added color correction.
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* raw images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Raw16ImageTransformEx for all-purpose image conversions
* from Raw16 formats. 
*
* \param[out]   pDst            A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc            A pointer to the raw source image (big endian:RH RL GH GL ...)
* \param[in]    nXSize          The width of the image in columns (must be a multiple of 2)
* \param[in]    nYSize          The height of the image in rows   n>0
* \param[in]    nBayerPattern   The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \param[in]    nShiftBits      The number of bits the result should be shifted to the right (0<=nShiftBits<=8)
* \param[in]    hColCor         A handle to the color conversion object, e.g created 
                                by ::UIT_CreateColorCorrectionMatrix_Int16. If zero,
                                no color correction is performed (faster).
* \return   S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw16ToBGR16_CC ( void                    *pDst,
                                                   const void              *pSrc, 
                                                   UINT32_IN_TYPE           nXSize,
                                                   UINT32_IN_TYPE           nYSize,
                                                   UINT32_IN_TYPE           nBayerPattern,
                                                   UINT32_IN_TYPE           nShiftBits,
                                                   COLOR_CORRECTION_HANDLE  hColCor );

/**
* Convert Raw16 (YH YL) to the MONO16 format (big endian).
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* raw images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Raw16ImageTransformEx for all-purpose image conversions
* from Raw16 formats. 
*
* \param[out]   pDst             A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc             A pointer to the raw source image (big endian:RH RL GH GL ...)
* \param[in]    nXSize           The width of the image in columns
* \param[in]    nYSize           The height of the image in rows
* \param[in]    nBayerPattern    The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \param[in]    nShiftBits      The number of bits the result should be shifted to the right (0<=nShiftBits<=8)
* \return   S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw16ToY16 ( void                 *pDst,
                                              const void           *pSrc, 
                                              UINT32_IN_TYPE        nXSize,
                                              UINT32_IN_TYPE        nYSize,
                                              UINT32_IN_TYPE        nBayerPattern,
                                              UINT32_IN_TYPE        nShiftBits );

/**
* Convert Raw16 (YH YL) to the MONO16 format (big endian) with added color conversion
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* raw images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Raw16ImageTransformEx for all-purpose image conversions
* from Raw16 formats. 
*
* \param[out]   pDst            A pointer to the destination image (B G R B G R ...)
* \param[in]    pSrc            A pointer to the raw source image (big endian:RH RL GH GL ...)
* \param[in]    nXSize          The width of the image in columns (must be a multiple of 2)
* \param[in]    nYSize          The height of the image in rows   n>0
* \param[in]    nBayerPattern   The code of the start pixel , use ::E_UNI_BAYER_PATTERN
* \param[in]    nShiftBits      The number of bits the result should be shifted to the right (0<=nShiftBits<=8)
* \param[in]    hColCor         A handle to the color conversion object, e.g created 
                                by ::UIT_CreateColorCorrectionMatrix_Int16. If zero, no color correction 
                                is performed (faster).
* \return S_OK if the operation was successful 
* \note      Source and Destination can point to the same memory location
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw16ToY16_CC ( void                      *pDst,
                                                 const void                *pSrc, 
                                                 UINT32_IN_TYPE             nXSize,
                                                 UINT32_IN_TYPE             nYSize,
                                                 UINT32_IN_TYPE             nBayerPattern,
                                                 UINT32_IN_TYPE             nShiftBits,
                                                 COLOR_CORRECTION_HANDLE    hColCor);

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Methods for controlling color conversion
/** @name Methods for controlling color conversion
Methods for use with format conversion methods ending with "_CC"
(::UIT_Raw8ToBGR_CC, ::UIT_Raw8ToY8_CC, ::UIT_Raw16ToBGR16_CC, ::UIT_Raw16ToY16_CC).
*/
/*@{*/

/**
* Create a color conversion object and return the handle to it. 
*  This handle may then be used in some 8 bit debayering methods.
*  This color conversion is then used as the last operation on the
*  image data (in RGB space). 
*  Every RGB pixel is virtually multiplied with a RGB2RGB conversion matrix
*  and a range check is performed. The
*  actual multiplications are implemented as integer multiplications.
*
* \param   *fArrayCorr    A float vector with nine elements containing the color conversion matrix (3 by 3) 
* \param   *phCC       The pointer to the conversion object handle
* \return  Always S_OK
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_CreateColorCorrectionMatrix_Int8 ( const FLOAT_TYPE *          fArrayCorr,
                                                                    COLOR_CORRECTION_HANDLE *   phCC );

/**
* Create a color conversion object and return the handle to it. 
*  This handle may then be used in some 16 bit debayering methods.
*  This color conversion is then used as the last operation on the
*  image data (in RGB space).
*  Every RGB pixel is virtually multiplied with a RGB2RGB conversion matrix
*  and a range check is performed. The
*  actual multiplications are implemented as integer multiplications.
*
* \param   fArrayCorr    A float vector with nine elements containing the color conversion matrix (3 by 3) 
* \param   phCC           The pointer to the conversion object handle
* \return  Always S_OK
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_CreateColorCorrectionMatrix_Int16 ( const FLOAT_TYPE *         fArrayCorr,
                                                                     COLOR_CORRECTION_HANDLE *  phCC );

/**
* Destroy the color correction object identified by the given handle.
*  After using this operation, the handle may not be used any more.
*
* \param   hCC   The handle of the conversion object to be destroyed
* \return  \li S_OK if successful, \li UNI_RESULT_PARAMETER_INVALID_1 if the handle is NULL
\li UNI_RESULT_COMMAND_ABORTED if the object could not be destroyed
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_DestroyColorCorrection ( COLOR_CORRECTION_HANDLE hCC);

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Methods for interlaced cameras
/** @name Methods for interlaced cameras
Methods for use with interlaced cameras which also have a CYGM sensor.
*/
/*@{*/
/**
* Convert an interlaced format to a progressive format.
*  This method may be used to convert raw or mono images consisting of
*  two half-images to progressive-data images. Only images that start
*  with the second half-image are supported, like those that are resulting
*  from the Guppy GF-0xx NIR.
*
* \param[out]  pDst            A pointer to the destination image
* \param[in]   pSrc            A pointer to the source image
* \param[in]   nXSize           The width of the image in columns
* \param[in]   nYSize           The height of the image in rows
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_InterlacedToProgressive ( UINT8_OUT_TYPE    pDst,
                                                          const UINT8_TYPE *pSrc, 
                                                          UINT32_IN_TYPE    nXSize,
                                                          UINT32_IN_TYPE    nYSize);

/**
* Convert a raw interlaced CYGM format like raw or mono to a progressive RGB format.
*  Only images that start
*  with the second half-image are supported, like those that are resulting
*  from the Guppy GF-038.
*
* \param[out]  pDst             A pointer to the destination image (B G R B ...)
* \param[in]   pSrc             A pointer to the raw source image (start pixel depends on parameter "nBayerPattern" )
* \param[in]   nXSize           The width of the image in columns (must be a multiple of 2) 
* \param[in]   nYSize           The height of the image in rows   (must be a multiple of 4) 
* \param[in]   nBayerPattern    The code of the start pixel use ::E_UNI_BAYER_PATTERN
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_IRaw8CYGMToBGR ( void                * pDst,
                                                  const UINT8_TYPE    * pSrc, 
                                                  UINT32_IN_TYPE        nXSize,
                                                  UINT32_IN_TYPE        nYSize,
                                                  UINT32_IN_TYPE        nBayerPattern );

/**
* Convert a raw interlaced CYGM format like raw or mono to a progressive RGB format with 3x3 matrix interpolation.
*  Only images that start
*  with the second half-image are supported, like those that are resulting
*  from the Guppy GF-038.
*
* \param[out]  pDst            A pointer to the destination image ( [B,G,R] ...)
* \param[in]   pSrc            A pointer to the raw source image
* \param[in]   nXSize           The width of the image in columns
* \param[in]   nYSize           The height of the image in rows  (must be a multiple of 4)
* \param[in]   nBayerPattern    The code of the start pixel -- UNUSED so far ( otherwise, see ::E_UNI_BAYER_PATTERN)
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_IRaw8CYGMToBGR_3x3 (   void             *pDst,
                                                        const UINT8_TYPE *pSrc, 
                                                        UINT32_IN_TYPE    nXSize,
                                                        UINT32_IN_TYPE    nYSize,
                                                        UINT32_IN_TYPE    nBayerPattern );

/**
* Convert a raw interlaced CYGM binning format into progressive RGB format.
*  Only images that start
*  with the second half-image are supported, like those that are resulting
*  from the Guppy GF-038.
*
* \param[out]  pDst             A pointer to the destination image (B G R B ...)
* \param[in]   pSrc             A pointer to the binned interlaced image
* \param[in]   nXSize           The width of the image in columns (must be a multiple of 2) 
* \param[in]   nYSize           The height of the image in rows   (must be a multiple of 4) 
* \param[in]   nBayerPattern    The code of the start pixel -- UNUSED so far ( otherwise, see ::E_UNI_BAYER_PATTERN)
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_IRaw8CYGMToBGR_Bin(    void               *pDst, 
                                                        const UINT8_TYPE   *pSrc, 
                                                        UINT32_IN_TYPE      nXSize, 
                                                        UINT32_IN_TYPE      nYSize, 
                                                        UINT32_IN_TYPE      nBayerPattern);

/**
* Convert a raw interlaced CYGM binning format into progressive RGB format with R=G=B.
*  Only images that start
*  with the second half-image are supported, like those that are resulting
*  from the Guppy GF-038.
*
* \param[out]  pDst             A pointer to the destination image (B G R B ...)
* \param[in]   pSrc             A pointer to the binned interlaced image
* \param[in]   nXSize           The width of the image in columns (must be a multiple of 2) 
* \param[in]   nYSize           The height of the image in rows   (must be a multiple of 4) 
* \param[in]   nBayerPattern    The code of the start pixel -- UNUSED so far ( otherwise, see ::E_UNI_BAYER_PATTERN)
* \return S_OK if the operation was successful 
* \note      Source and Destination have to point to different memory locations
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_IRaw8CYGMToMono(       UINT8_OUT_TYPE      pDst, 
                                                        const UINT8_TYPE   *pSrc, 
                                                        UINT32_IN_TYPE      nXSize, 
                                                        UINT32_IN_TYPE      nYSize, 
                                                        UINT32_IN_TYPE      nBayerPattern);

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Methods for planes
/** @name Format conversions to image planes
Format conversions from IIDC transfer formats to image planes

*/
/*@{*/
/**
* Convert an image in YUV444 format to R,G and B planes
*
* \param[out]	pR		A pointer to the red plane
* \param[out]	pG		A pointer to the green plane 
* \param[out]	pB		A pointer to the blue plane
* \param[in]	pSrc	A pointer to the source image
* \param[in]	nXSize	The width of the image in columns n>1
* \param[in]	nYSize	The height of the image in rows   n>1 
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV444ToRGBPlanes( UINT8_OUT_TYPE      pR,
                                                   UINT8_OUT_TYPE      pG,
                                                   UINT8_OUT_TYPE      pB,
                                                   const UINT8_TYPE   *pSrc,
                                                   UINT32_IN_TYPE      nXSize,
                                                   UINT32_IN_TYPE      nYSize  );

/**
* Convert an image in YUV422 format to R,G and B planes
*
* \param[out]	pR		A pointer to the red plane
* \param[out]	pG		A pointer to the green plane 
* \param[out]	pB		A pointer to the blue plane
* \param[in]	pSrc	A pointer to the source image
* \param[in]	nXSize	The width of the image in columns n>1
* \param[in]	nYSize	The height of the image in rows   n>1 
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV422ToRGBPlanes( UINT8_OUT_TYPE      pR,
                                                   UINT8_OUT_TYPE      pG,
                                                   UINT8_OUT_TYPE      pB,
                                                   const UINT8_TYPE   *pSrc,
                                                   UINT32_IN_TYPE      nXSize,
                                                   UINT32_IN_TYPE      nYSize  );

/**
* Convert an image in YUV411 format to R,G and B planes
*
* \param[out]	pR		A pointer to the red plane
* \param[out]	pG		A pointer to the green plane 
* \param[out]	pB		A pointer to the blue plane
* \param[in]	pSrc	A pointer to the source image
* \param[in]	nXSize	The width of the image in columns
* \param[in]	nYSize	The height of the image in rows
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV411ToRGBPlanes( UINT8_OUT_TYPE      pR,
                                                    UINT8_OUT_TYPE      pG,
                                                    UINT8_OUT_TYPE      pB,
                                                    const UINT8_TYPE   *pSrc,
                                                    UINT32_IN_TYPE      nXSize,
                                                    UINT32_IN_TYPE      nYSize  );

/**
* Convert an image from YUV444 format to R,G,B and additional Y planes
*
* \param[out] pR        A pointer to the red plane
* \param[out] pG        A pointer to the green plane 
* \param[out] pB        A pointer to the blue plane
* \param[out] pY        A pointer to the intensity plane
* \param[in]  pSrc      A pointer to the source image
* \param[in]  nXSize    The width of the image in columns
* \param[in]  nYSize    The height of the image in rows
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV444ToRGBYPlanes(UINT8_OUT_TYPE      pR,
                                                    UINT8_OUT_TYPE      pG,
                                                    UINT8_OUT_TYPE      pB,
                                                    UINT8_OUT_TYPE      pY,
                                                    const UINT8_TYPE   *pSrc,
                                                    UINT32_IN_TYPE      nXSize,
                                                    UINT32_IN_TYPE      nYSize  );

/**
* Convert an image from YUV422 format to R,G,B and additional Y planes
*
* \param[out] pR        A pointer to the red plane
* \param[out] pG        A pointer to the green plane 
* \param[out] pB        A pointer to the blue plane
* \param[out] pY        A pointer to the intensity plane
* \param[in]  pSrc      A pointer to the source image
* \param[in]  nXSize    The width of the image in columns
* \param[in]  nYSize    The height of the image in rows
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV422ToRGBYPlanes(UINT8_OUT_TYPE      pR,
                                                    UINT8_OUT_TYPE      pG,
                                                    UINT8_OUT_TYPE      pB,
                                                    UINT8_OUT_TYPE      pY,
                                                    const UINT8_TYPE   *pSrc,
                                                    UINT32_IN_TYPE      nXSize,
                                                    UINT32_IN_TYPE      nYSize  );

/**
* Convert an image from YUV411 format to R,G,B and additional Y planes
*
* \param[out] pR        A pointer to the red plane
* \param[out] pG        A pointer to the green plane 
* \param[out] pB        A pointer to the blue plane
* \param[out] pY        A pointer to the intensity plane
* \param[in] pSrc       A pointer to the source image
* \param[in] nXSize     The width of the image in columns
* \param[in] nYSize     The height of the image in rows
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV411ToRGBYPlanes(UINT8_OUT_TYPE      pR,
                                                    UINT8_OUT_TYPE      pG,
                                                    UINT8_OUT_TYPE      pB,
                                                    UINT8_OUT_TYPE      pY,
                                                    const UINT8_TYPE   *pSrc,
                                                    UINT32_IN_TYPE      nXSize,
                                                    UINT32_IN_TYPE      nYSize  );

/**
* Convert an image from RGB format to R,G,B and additional Y planes
*
* \param[out] pR        A pointer to the red plane
* \param[out] pG        A pointer to the green plane 
* \param[out] pB        A pointer to the blue plane
* \param[out] pY        A pointer to the intensity plane
* \param[in] pSrc       A pointer to the source image
* \param[in] nXSize     The width of the image in columns
* \param[in] nYSize     The height of the image in rows
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_RGB8ToRGBYPlanes ( UINT8_OUT_TYPE      pR,
                                                    UINT8_OUT_TYPE      pG,
                                                    UINT8_OUT_TYPE      pB,
                                                    UINT8_OUT_TYPE      pY,
                                                    const UINT8_TYPE   *pSrc,
                                                    UINT32_IN_TYPE      nXSize,
                                                    UINT32_IN_TYPE      nYSize  );
                                                    
/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Image transformation
/** @name Image transformation.
Methods for transforming images without image type change. Logically, ::UIT_RGB8ToBGR
and ::UIT_InterlacedToProgressive would also fall into this category, but since
they are already listed in different categories, they do not appear here.

*/
/*@{*/
/** 
 Calculate the needed destination image width for images with non-square pixels.
 This method is made for Mono8 images as well as for Raw8 images.

    \param[out]     pDstXSize       pointer, returns the needed destination image width
    \param[in]      nSrcXSize       source image width
    \param[in]      nDstPixelSize   destination image pixel size
    \param[in]      nSrcPixelSize   source image pixel size
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_GetResampledScanlineLength( INT32_OUT_TYPE pDstXSize,
                                                             INT32_TYPE     nSrcXSize, 
                                                             FLOAT_TYPE     nDstPixelSize,
                                                             FLOAT_TYPE     nSrcPixelSize );

/**
 Do resampling along lines.
    \param[out] pDst             valid pointer to destination image
    \param[in]  pSrc             valid const pointer to source image
    \param[in]  nDstXSize        destination image width
    \param[in]  nSrcXSize        source image width
    \param[in]  nSrcYSize        source image height
    \param[in]  nDstPixelSize    destination image pixel width
    \param[in]  nSrcPixelSize    source image pixel width
    \param[in]  nBytesPerPixel   the number of bytes per pixel (e.g. 1 for mono, 3 for RGB)
    \param[in]  nRopMode         use ::E_UNI_ROPMODE
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_ResampleScanlines( UINT8_OUT_TYPE      pDst,
                                                    const UINT8_TYPE  * pSrc, 
                                                    INT32_TYPE          nDstXSize,        
                                                    INT32_TYPE          nSrcXSize,
                                                    INT32_TYPE          nSrcYSize, 
                                                    FLOAT_TYPE          nDstPixelSize,
                                                    FLOAT_TYPE          nSrcPixelSize,
                                                    UINT32_TYPE         nBytesPerPixel,
                                                    UINT32_TYPE         nRopMode);

/**
 Do resampling along lines.
 This method is made for Mono8 images as well as for Raw8 images.
Extended version.
    \param[out] pDst             valid pointer to destination image
    \param[in]  pSrc             valid const pointer to source image
    \param[in]  nDstXSize        destination image width
    \param[in]  nDstPitch        distance between consecutive lines in the destination image
    \param[in]  nSrcXSize        source image width
    \param[in]  nSrcPitch        distance between consecutive line in the source image
    \param[in]  nSrcYSize        source image height
    \param[in]  nDstPixelSize    destination image pixel width
    \param[in]  nSrcPixelSize    source image pixel width
    \param[in]  nBytesPerPixel   The number of bytes per pixel (e.g. 1 for mono, 3 for RGB)
    \param[in]  nRopMode         use ::E_UNI_ROPMODE
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_ResampleScanlinesEx( UINT8_OUT_TYPE    pDst,
                                                      const UINT8_TYPE *pSrc, 
                                                      INT32_TYPE        nDstXSize,
                                                      INT32_TYPE        nDstPitch,
                                                      INT32_TYPE        nSrcXSize,
                                                      INT32_TYPE        nSrcPitch,
                                                      INT32_TYPE        nSrcYSize, 
                                                      FLOAT_TYPE        nDstPixelSize,
                                                      FLOAT_TYPE        nSrcPixelSize,
                                                      UINT32_TYPE       nBytesPerPixel,
                                                      UINT32_TYPE       nRopMode);

/**
 Flip image vertically.
 Create a top-down version of the provided image (in place). This method is made for all kinds of images since the pixel format is not relevant if
 the correct number of overall bytes per line is submitted.
    \param[in,out]  pImage          Valid pointer to source/destination image
    \param[in]      nBytesPerLine   The number of bytes (not pixels!) per line
    \param[in]      nLines          The number of lines
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_FlipVertical( UINT8_INOUT_TYPE pImage
                                              , UINT32_TYPE     nBytesPerLine
                                              , UINT32_TYPE     nLines);

/** Generic flip of image.
    \param[in,out] pImage       valid pointer to source/destination image
    \param[in]  nXSize          image width
    \param[in]  nYSize          Image height
    \param[in]  eFlipMode       Use valid values from ::E_UNI_FLIP_MODE
    \param[in]  eccColorCode    Use ::E_UNI_COLOR_CODE to allow for correct image flipping in horizontal and both modes
* \return S_OK if the operation was successful 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Flip( UINT8_INOUT_TYPE pImage
                                     , UINT32_TYPE      nXSize
                                     , UINT32_TYPE      nYSize
                                     , UINT32_TYPE      eFlipMode
                                     , UINT32_TYPE      eccColorCode);

/*@}*/
/////////////////////////////////////////////////////////////////////////////
// Methods for variable output images
/** @name Format conversions to variable image formats
Format conversions from IIDC transfer formats to BGR24, BGRA32 or YUV422 UYVY

*/
/*@{*/

/** Convert RGB format with 8 bytes per pixel to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    IIDC RGB8/ RGB24 source image
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_RGB8ImageTransform  (          VOID_TYPE          *pDst
                                                      ,   const   VOID_TYPE          *pSrc
                                                      ,           UINT32_IN_TYPE      nPixelCount
                                                      ,           UINT32_IN_TYPE      eImageFormat);

/** Convert RGBA format with 8 bytes per pixel to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    RGBA8 / RGBA32 source image
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_RGBA8ImageTransform (          VOID_TYPE          *pDst
                                                      ,   const   VOID_TYPE          *pSrc
                                                      ,           UINT32_IN_TYPE      nPixelCount
                                                      ,           UINT32_IN_TYPE      eImageFormat);

/** Convert YUV 4:4:4 to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    YUV444 source image
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \returns      UNI_RETURN_TYPE 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV444ImageTransform(          VOID_TYPE          *pDst
                                                     ,  const   VOID_TYPE          *pSrc
                                                     ,          UINT32_IN_TYPE      nPixelCount
                                                     ,          UINT32_IN_TYPE      eImageFormat);

/** Convert YUV 4:2:2 to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    YUV422 source image (byte order UYVY)
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \returns      UNI_RETURN_TYPE 
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV422ImageTransform(           VOID_TYPE          *pDst
                                                      ,  const   VOID_TYPE          *pSrc
                                                      ,          UINT32_IN_TYPE      nPixelCount
                                                      ,          UINT32_IN_TYPE      eImageFormat);

/** Convert YUV 4:1:1 to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    YUV411 source image (byte order UYYVYY)
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_YUV411ImageTransform(          VOID_TYPE          *pDst
                                                     ,  const   VOID_TYPE          *pSrc
                                                     ,          UINT32_IN_TYPE      nPixelCount
                                                     ,          UINT32_IN_TYPE      eImageFormat);

/** Convert Mono8 to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    Mono8 source image
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \note         also supports E_SIF_UYVY_MONO output
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y8ImageTransform(              VOID_TYPE          *pDst
                                                    ,   const   UINT8_TYPE         *pSrc
                                                    ,           UINT32_IN_TYPE      nPixelCount
                                                    ,           UINT32_IN_TYPE      eImageFormat);

/** Convert IIDC Mono16 to variable image format.
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* mono images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Y16ImageTransformEx for all-purpose image conversions
* from Y16 formats. 
* 
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    IIDC Mono16 source image
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ImageTransform(             VOID_TYPE          *pDst
                                                  ,     const   VOID_TYPE          *pSrc
                                                  ,             UINT32_IN_TYPE      nPixelCount
                                                  ,             UINT32_IN_TYPE      eImageFormat);

/** Convert several Mono16 format variations to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    IIDC Mono16 source image
* \param[in]    nPixelCount             number of pixels per image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    psFormatInfo            detailed input format specification - IIDC peculiarities are assumed when NULL
* \returns      UNI_RETURN_TYPE
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ImageTransformEx(             VOID_TYPE                    *pDst
                                                    ,     const   VOID_TYPE                    *pSrc
                                                    ,             UINT32_IN_TYPE               nPixelCount
                                                    ,             UINT32_IN_TYPE               eImageFormat
                                                    ,     const   S_UNI_TRANSPORT_FORMAT_INFO  *psFormatInfo = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UIT_Y16ImageTransformEx(             VOID_TYPE                    *pDst
                                                    ,     const   VOID_TYPE                    *pSrc
                                                    ,             UINT32_IN_TYPE               nPixelCount
                                                    ,             UINT32_IN_TYPE               eImageFormat
                                                    ,     const   S_UNI_TRANSPORT_FORMAT_INFO  *psFormatInfo );
#endif

/** Convert RAW8 to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    RAW8 source image
* \param[in]    XSize                   width of the image
* \param[in]    YSize                   height of the image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \note         also supports ::E_SIF_UYVY output
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ImageTransform(            VOID_TYPE          *pDst
                                                   ,    const   UINT8_TYPE         *pSrc
                                                   ,            UINT32_IN_TYPE      XSize
                                                   ,            UINT32_IN_TYPE      YSize
                                                   ,            UINT32_IN_TYPE      eImageFormat
                                                   ,            UINT32_IN_TYPE      bayerPattern);

/** Convert IIDC RAW16 to variable image format.
* \note 
* This function is provided for backwards compatibility. When using GigE-Vision cameras,
* raw images without any compatibility adjustments will be converted incorrectly.  
* It is recommended to use ::UIT_Raw16ImageTransformEx for all-purpose image conversions
* from Y16 formats. 
*
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    IIDC RAW16 image
* \param[in]    XSize                   width of the image
* \param[in]    YSize                   height of the image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw16ImageTransform(           VOID_TYPE          *pDst
                                                    ,   const   VOID_TYPE          *pSrc
                                                    ,           UINT32_IN_TYPE      XSize
                                                    ,           UINT32_IN_TYPE      YSize
                                                    ,           UINT32_IN_TYPE      eImageFormat
                                                    ,           UINT32_IN_TYPE      bayerPattern);

/** Convert several Raw16 format variations to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    IIDC RAW16 image
* \param[in]    XSize                   width of the image
* \param[in]    YSize                   height of the image
* \param[in]    eImageFormat            destination image format (see \ref E_UNI_SIMPLE_IMAGE_FORMAT)
* \param[in]    psFormatInfo            detailed input format specification
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw16ImageTransformEx(           VOID_TYPE                    *pDst
                                                      ,   const   VOID_TYPE                    *pSrc
                                                      ,           UINT32_IN_TYPE                XSize
                                                      ,           UINT32_IN_TYPE                YSize
                                                      ,           UINT32_IN_TYPE                eImageFormat
                                                      ,   const   S_UNI_TRANSPORT_FORMAT_INFO  *psFormatInfo);

/** Convert RAW8 to variable image format with 3x3 local filtering.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    RAW8 source image
* \param[in]    XSize                   width of the image
* \param[in]    YSize                   height of the image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ImageTransform_3x3(        VOID_TYPE          *pDst
                                                       ,const   UINT8_IN_TYPE      *pSrc
                                                       ,        UINT32_IN_TYPE      XSize
                                                       ,        UINT32_IN_TYPE      YSize
                                                       ,        UINT32_IN_TYPE      eImageFormat
                                                       ,        UINT32_IN_TYPE      bayerPattern);

/** Convert RAW8 to variable image format with U/V interpolation in the YUV color space.
* \param[out]       pDst                variable image format destination (according to eImageFormat)
* \param[in,out]    pSrc                RAW8 image, will be altered in processing
* \param[in]        XSize               width of the image
* \param[in]        YSize               height of the image
* \param[in]        eImageFormat        destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns          UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ImageTransform_IntYUV422(  VOID_TYPE          *pDst
                                                             ,  UINT8_IN_TYPE      *pSrc
                                                             ,  UINT32_IN_TYPE      XSize
                                                             ,  UINT32_IN_TYPE      YSize
                                                             ,  UINT32_IN_TYPE      eImageFormat
                                                             ,  UINT32_IN_TYPE      bayerPattern);

/** Convert RAW8 to variable image format with local color anti aliasing.
* \param[out]       pDst                variable image format destination (according to eImageFormat)
* \param[in,out]    pSrc                RAW8 source image, will be altered while processing
* \param[in]        XSize               width of the image
* \param[in]        YSize               height of the image
* \param[in]        eImageFormat        destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns          UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ImageTransform_IntLCAA(    VOID_TYPE          *pDst
                                                           ,    UINT8_TYPE         *pSrc
                                                           ,    UINT32_IN_TYPE      XSize
                                                           ,    UINT32_IN_TYPE      YSize
                                                           ,    UINT32_IN_TYPE      eImageFormat
                                                           ,    UINT32_IN_TYPE      bayerPattern);

/** Convert RAW8 to variable image format  with local color anti aliasing vertical interpolation.
* \param[out]       pDst                variable image format destination (according to eImageFormat)
* \param[in,out]    pSrc                RAW8 image, will be altered in processing
* \param[in]        XSize               width of image
* \param[in]        YSize               height of image
* \param[in]        eImageFormat        destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns          UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw8ImageTransform_IntLCAAV(   VOID_TYPE          *pDst
                                                            ,   UINT8_IN_TYPE      *pSrc
                                                            ,   UINT32_IN_TYPE      XSize
                                                            ,   UINT32_IN_TYPE      YSize
                                                            ,   UINT32_IN_TYPE      eImageFormat 
                                                            ,   UINT32_IN_TYPE      bayerPattern);

/** Convert interlaced RAW8 binning format to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    interlaced RAW8 image source
* \param[in]    XSize                   width of the image
* \param[in]    YSize                   height of the image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_IRaw8CYGMImageTransform_Bin(       VOID_TYPE      *pDst
                                                            ,const  UINT8_TYPE     *pSrc
                                                            ,       UINT32_IN_TYPE  XSize
                                                            ,       UINT32_IN_TYPE  YSize
                                                            ,       UINT32_IN_TYPE  eImageFormat
                                                            ,       UINT32_IN_TYPE  bayerPattern);

/** Convert interlaced RAW8 CYGM to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    interlaced RAW8 image source
* \param[in]    XSize                   width of the image
* \param[in]    YSize                   height off the image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_IRaw8CYGMImageTransform(       VOID_TYPE          *pDst
                                                        , const UINT8_IN_TYPE      *pSrc
                                                        ,       UINT32_IN_TYPE      XSize
                                                        ,       UINT32_IN_TYPE      YSize
                                                        ,       UINT32_IN_TYPE      eImageFormat
                                                        ,       UINT32_IN_TYPE      bayerPattern);

/** Convert interlaced RAW8 CYGM to variable image format with 3x3 local filtering.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    interlaced RAW8 CYGM image source
* \param[in]    XSize                   width of the image
* \param[in]    YSize                   height of the image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    bayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_IRaw8CYGMImageTransform_3x3(       VOID_TYPE      *pDst
                                                            ,const  UINT8_TYPE     *pSrc
                                                            ,       UINT32_IN_TYPE  XSize
                                                            ,       UINT32_IN_TYPE  YSize
                                                            ,       UINT32_IN_TYPE  eImageFormat
                                                            ,       UINT32_IN_TYPE  bayerPattern);

/** Convert AVT RAW12 to variable image format.
* \param[out]   pDst                    variable image format destination (according to eImageFormat)
* \param[in]    pSrc                    AVT RAW12 image source
* \param[in]    nXSize                  width of the image
* \param[in]    nYSize                  height of the image
* \param[in]    eImageFormat            destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
* \param[in]    BayerPattern            bayer pattern ::E_UNI_BAYER_PATTERN
* \returns      UNI_RETURN_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Raw12ImageTransform (          VOID_TYPE          *pDst
                                                    ,   const   UINT8_TYPE         *pSrc
                                                    ,           UINT32_IN_TYPE      nXSize
                                                    ,           UINT32_IN_TYPE      nYSize
                                                    ,           UINT32_IN_TYPE      eImageFormat
                                                    ,           UINT32_TYPE         BayerPattern);
/** Convert a packed 12 bit monochrome format to variable image format.
* \param[out]   pDst                a pointer to the destination image  (according to eImageFormat)
* \param[in]    pSrc                a pointer to the binned interlaced image
* \param[in]    nPixelCount         number of pixels of the image
* \param[in]    eImageFormat        destination \ref E_UNI_SIMPLE_IMAGE_FORMAT
*/
UNI_RETURN_TYPE WIN32DLL_API UIT_Y12ImageTransform (            VOID_TYPE      *pDst
                                                    ,   const   UINT8_TYPE     *pSrc
                                                    ,           UINT32_TYPE     nPixelCount
                                                    ,           UINT32_TYPE     eImageFormat);
/*@}*/

#ifdef __cplusplus
 }
#endif // #ifdef __cplusplus
#endif //#ifndef UNI_TRANSFORM_H_