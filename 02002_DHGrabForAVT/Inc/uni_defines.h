#ifndef UNI_DEFINES_H_
#define UNI_DEFINES_H_
#pragma once
#pragma warning (disable: 4635)     // badly-formed XML

/** \file uni_defines.h
 * \brief Definitions for the Universal API methods.
 *
 * This file describes all necessary definitions for using the
 *  Universal API methods.
 *
 * \par license
 * This code is the property of Allied Vision Technologies. Nevertheless, you
 * are absolutely free to use and modify it. The code is provided "as is" with
 * no expressed or implied warranty. The author and AVT accept no liability if
 * it causes any damage to your computer.
 *
 */
#ifdef USE_STDAFX_H_
#include "stdafx.h"
#endif
#include "uni_types.h"
#ifndef S_OK
#define S_OK ((HRESULT)0x00000000L)
#endif

#define UNI_RESULT_API_ERROR                 0x88000000 //!< mask for API error
#define UNI_RESULT_UNKNOWN_ERROR             0x88000001 //!< unclassified error
#define UNI_RESULT_PARAMETER_INVALID_1       0x88000002 //!< parameter 1 is invalid
#define UNI_RESULT_PARAMETER_INVALID_2       0x88000003 //!< parameter 2 is invalid
#define UNI_RESULT_PARAMETER_INVALID_3       0x88000004 //!< parameter 3 is invalid
#define UNI_RESULT_PARAMETER_INVALID_4       0x88000005 //!< parameter 4 is invalid
#define UNI_RESULT_PARAMETER_INVALID_5       0x88000006 //!< parameter 5 is invalid
#define UNI_RESULT_PARAMETER_INVALID_6       0x88000007 //!< parameter 6 is invalid
#define UNI_RESULT_PARAMETER_INVALID_7       0x88000008 //!< parameter 7 is invalid
#define UNI_RESULT_PARAMETER_INVALID_8       0x88000009 //!< parameter 8 is invalid
#define UNI_RESULT_PARAMETER_INVALID_9       0x8800000A //!< parameter 9 is invalid
#define UNI_RESULT_PARAMETER_INVALID_10      0x8800000B //!< parameter 10 is invalid
#define UNI_RESULT_PARAMETER_INVALID_11      0x8800000C //!< parameter 11 is invalid
#define UNI_RESULT_PARAMETER_INVALID_12      0x8800000D //!< parameter 12 is invalid

#define UNI_RESULT_CAMERA_NOT_FOUND          0x88000010 //!< camera not found
#define UNI_RESULT_AVT_CAMERA_NOT_FOUND      0x88000011 //!< AVT camera not found
#define UNI_RESULT_FEATURE_NOT_IMPLEMENTED   0x88000012 //!< feature support is not implemented (yet?)
#define UNI_RESULT_FEATURE_NOT_SUPPORTED     0x88000013 //!< feature not supported by camera
#define UNI_RESULT_INVALID_FEATURECAPABILITY 0x88000014 //!< invalid operation for the selected feature
#define UNI_RESULT_API_NOT_INITIALIZED       0x88000015 //!< the API was not initialized - ::UCC_Init has to be called first
#define UNI_RESULT_GRAB_NOT_INITIALIZED      0x88000016 //!< the camera was not initialized before grabbing
#define UNI_RESULT_CAMERA_BUSY               0x88000017 //!< the camera is busy
#define UNI_RESULT_INVALID_SETTINGS          0x88000018 //!< invalid camera settings selected
#define UNI_RESULT_INSUFFICIENT_RESOURCES    0x88000019 //!< insufficient FireWire resources
#define UNI_RESULT_OUT_OF_MEMORY             0x8800001A //!< out of memory
#define UNI_RESULT_NOT_ALLOWED_IN_THIS_MODE  0x8800001B //!< operation not allowed in this mode
#define UNI_RESULT_CAMERA_TIMEOUT            0x8800001C //!< timeout period expired
#define UNI_RESULT_API_ALREADY_INITIALIZED   0x8800001D //!< second call to ::UCC_Init ; the API was already initialized

#define UNI_RESULT_COMMAND_ABORTED           0x8800001E //!< the current command was aborted

#define UNI_RESULT_NOTIFICATION_NO_TYPE      0x8800001F //!< notification is missing the ::E_UNI_NOTIFICATION_TYPE parameter
#define UNI_RESULT_MORE_DATA                 ERROR_MORE_DATA //!< timeout period expired

#define UNI_UNDEFINED                        0xFFFFFFFF //!< represents an undefined input parameter
#define UNI_ALL_CAMERAS                      0xFFffFFff //!< special camera id to address all connected cameras
#define UNI_RESULT_FIREGRAB_ERROR            0x8A000000 //!< Mask for a FireGrab error
#define MAKE_FGRAB_ERROR(fce)                (fce | UNI_RESULT_FIREGRAB_ERROR)

//--------------------------------- Enumerations ------------------------------

/** Enumeration for raster operation (used by re-sampling methods).*/
typedef enum E_UNI_ROPMODE
{
    E_ROP_SRCCPY,       //!< nearest neighbor raster operation method
    E_ROP_LININTERPOL,  //!< interpolating raster operation method
} E_UniRopMode,E_UNI_ROPMODE;

/** 
Enumeration for color codes / transfer formats.
These codes have their origin in standard IIDC color codes and AVT-specific color codes
used in format 7, but they are used for cameras on both interfaces.
*/
typedef enum E_UNI_COLOR_CODE
{
    E_CC_COLOR_CODE_BEGIN=0,
    E_CC_MONO8=0,    //!< one byte per pixel, sequential byte order
    E_CC_YUV411,     //!< four pixels in six bytes, byte order is UYYVYYUYYVYY
    E_CC_YUV422,     //!< two pixels in four bytes, byte order is UYVYUYVY
    E_CC_YUV444,     //!< three bytes per pixel, byte order is UYVUYV
    E_CC_RGB8,       //!< three bytes per pixel, byte order is RGBRGB
    E_CC_MONO16,     //!< two bytes per pixel, byte order is high_byte,low_byte
    E_CC_RGB16,      //!< six bytes per pixel, byte order is Rh,Rl,Gh,Gl,Bh,Bl,
    E_CC_SMONO16,    //!< two bytes per pixel, byte order is high_byte,low_byte
    E_CC_SRGB16,     //!< six bytes per pixel, byte order is Rh,Rl,Gh,Gl,Bh,Bl,
    E_CC_RAW8,       //!< one byte per pixel, byte order is RGGB, GBRG, GRBG or BGGR
    E_CC_RAW16,      //!< two bytes per pixel, byte order is RhRlGhGlGhGlBhBl, GhGlBhBlRhRlGhGl, GhGlRhRlBhBlGhGl or BhBlGhGlGhGlRhRl
    E_CC_MONOR=128,  //!< see ::E_CC_MONO8
    E_CC_MONOG,      //!< see ::E_CC_MONO8
    E_CC_MONOB,      //!< see ::E_CC_MONO8
    E_CC_MONO12=132, //!< two pixels in three bytes, see ::S_12BIT_2PACKED
    E_CC_RGB12,      //!< 12 bit RGB, unused
    E_CC_SMONO12,    //!< 12 bit SMONO12, unused
    E_CC_SRGB12,     //!< 12 SRGB, unused
    E_CC_RAW12,      //!< two pixels in three bytes, see ::S_12BIT_2PACKED
    E_CC_RGBA8=141,  //!< four bytes per pixel, byte order is RGBARGBA
    E_CC_LAST=255,
    E_CC_COLOR_CODE_END=E_CC_LAST
} E_UniColorCode, E_UNI_COLOR_CODE;

//! Enumeration for the Bayer pattern
typedef enum E_UNI_BAYER_PATTERN
{
    E_BP_NONE = -2,  //!< Image is taken from mono camera, no bayer pattern present.
    E_BP_AUTO = -1,  //!< Set pattern to automatic mode (must not be used in conversion methods!!!)
    E_BP_RGGB=0,     //!< RGGB pattern, red pixel comes first
    E_BP_GBRG,       //!< RGGB pattern, green pixel of blue row comes first
    E_BP_GRBG,       //!< RGGB pattern, green pixel of red row comes first
    E_BP_BGGR,       //!< RGGB pattern, blue pixel comes first
    E_BP_CYGM=128,   //!< CYGM pattern, cyan pixel comes first in the first row, green in the second row (of the sensor)
    E_BP_GMCY,       //!< CYGM pattern, green pixel comes first in the first row, cyan in the second row (of the sensor)
    E_BP_CYMG,       //!< CYGM pattern, cyan pixel comes first in the first row, magenta in the second row (of the sensor)
    E_BP_MGCY,       //!< CYGM pattern, magenta pixel comes first in the first row, cyan in the second row (of the sensor)
    E_BP_LAST=255
} E_UniBayerPattern, E_UNI_BAYER_PATTERN;

//! Enumeration for the interface type
typedef enum E_UNI_INTERFACE
{
    E_INTF_FIREWIRE=0, //!< FireWire
    E_INTF_GIGE,       //!< Gigabit Ethernet
    E_INTF_LAST=255
} E_UniInterface, E_UNI_INTERFACE;

//! Enumeration for the endianness
typedef enum E_UNI_ENDIANNESS
{
    E_ENDIANNESS_LITTLE=0, //!< Little endian data format
    E_ENDIANNESS_BIG,      //!< Big endian data format
    E_ENDIANNESS_LAST=255
} E_UniEndianness, E_UNI_ENDIANNESS;

//! Enumeration for the image alignment
typedef enum E_UNI_ALIGNMENT
{
    E_ALIGNMENT_MSB=0,    //!< Data is MSB aligned (pppp pppp pppp ....)
    E_ALIGNMENT_LSB,      //!< Data is LSB aligned (.... pppp pppp pppp)
    E_ALIGNMENT_LAST=255
} E_UniAlignment, E_UNI_ALIGNMENT;

//! Enumeration for the direction for flipping images
typedef enum E_UNI_FLIP_MODE
{
    E_FLIP_MODE_NONE        = 0,
    E_FLIP_MODE_HORIZONTAL  = 1,                        //!< flip in horizontal direction
    E_FLIP_MODE_LEFT_RIGHT  = E_FLIP_MODE_HORIZONTAL,   //!< same as E_FLIP_MODE_HORIZONTAL

    E_FLIP_MODE_VERTICAL    = 2,                        //!< flip in vertical direction
    E_FLIP_MODE_TOP_DOWN    = E_FLIP_MODE_VERTICAL,     //!< same as E_FLIP_MODE_VERTICAL
    E_FLIP_MODE_BOTH        = 3                         //!< flip in both directions
} E_UniFlipMode, E_UNI_FLIP_MODE;

//! Structure for accessing data in 12-bit transfer mode, two pixel are coded into 3 bytes
typedef struct S_12BIT_2PACKED
{
    UINT8_TYPE  m_nVal8_1       ;   //!< High byte of the first Pixel
    UINT8_TYPE  m_nVal8_1Low : 4;   //!< Low nibble of the first pixel
    UINT8_TYPE  m_nVal8_2Low : 4;   //!< Low nibble of the second pixel
    UINT8_TYPE  m_nVal8_2       ;   //!< High byte of the second pixel
} S_12BIT_2PACKED, *PS_12BIT_2PACKED;

/** states of the multi media technology support for operating system and processor.
*/
typedef struct S_SUPPORT_STATE
{
    BOOL_TYPE Processor;         //!< technology supported by the processor
    BOOL_TYPE OperatingSystem;   //!< technology supported by the OS
}S_SupportState, S_SUPPORT_STATE;

/** states of the support for different multimedia technologies*/
typedef struct S_TECHNO_INFO
{
    S_SUPPORT_STATE IntelMMX;       //!< INTEL first gen MultiMedia eXtension
    S_SUPPORT_STATE IntelSSE;       //!< INTEL Streaming SIMD Extension
    S_SUPPORT_STATE IntelSSE2;      //!< INTEL Streaming SIMD Extension 2
    S_SUPPORT_STATE IntelSSE3;      //!< INTEL Streaming SIMD Extension 3
    S_SUPPORT_STATE IntelSSSE3;     //!< INTEL Supplemental Streaming SIMD Extension 3
    S_SUPPORT_STATE AMD3DNow;       //!< AMD 3DNow
} S_TechInfo, S_TECHNO_INFO;

/** enum for the secure image signatures*/
typedef enum E_UNI_SIS_TYPE
{
    E_SIS_NOT_PRESENT,  //!< camera supports no SIS
    E_SIS_MARLIN,       //!< Marlin Cycle,Frame,Trigger count
    E_SIS_PIKE          //!< Pike 3.0 and Stingray SIS
} E_UniSISType, E_UNI_SIS_TYPE;

/**information about state of the fields of the ::S_SIS_DATA struct*/
typedef union S_SIS_ENABLE
{
    /** states of the SIS data*/
    struct S_SIS_ENABLE_FIELDS
    {
        UINT32_TYPE     m_CycleCounter      : 1;    //!< inq cycle counter
        UINT32_TYPE     m_FrameCounter      : 1;    //!< inq frame count since last reset
        UINT32_TYPE     m_TriggerCounter    : 1;    //!< inq triggers received by the camera
        UINT32_TYPE     m_AOILeft           : 1;    //!< inq left coordinate of the cameras AOI
        UINT32_TYPE     m_AOITop            : 1;    //!< inq top coordinate of  the cameras AOI
        UINT32_TYPE     m_AOIWidth          : 1;    //!< inq width of the AOI
        UINT32_TYPE     m_AOIHeight         : 1;    //!< inq height of the AOI
        UINT32_TYPE     m_Shutter           : 1;    //!< inq camera shutter for the image
        UINT32_TYPE     m_Gain              : 1;    //!< inq camera gain for the image
        UINT32_TYPE     Reserved0           : 1;
        UINT32_TYPE     m_OutputState       : 1;    //!< inq output pin states
        UINT32_TYPE     m_InputState        : 1;    //!< inq input pin states
        UINT32_TYPE     m_SequenceIndex     : 1;    //!< inq position in sequence
        UINT32_TYPE     Reserved2a          : 1;
        UINT32_TYPE     m_ColorCoding       : 1;    //!< inq IIDC color coding
        UINT32_TYPE     Reserved2b          : 1;
        UINT32_TYPE     m_SerialNumber      : 1;    //!< inq  serial number of the camera
        UINT32_TYPE     m_UserValue         : 1;    //!< inq user defined value
        UINT32_TYPE     Reserved2           :13;
        UINT32_TYPE     m_Valid             : 1;    //!< Data in m_Enable is valid
    }               m_Enable ;                      //!< inquiry structure for supported SIS data
    UINT32_TYPE     m_Data;                 //!< m_Enable as UINT32
} S_SISEnable,S_SIS_ENABLE;

/** SIS data structure*/
typedef struct S_SIS_DATA
{

    UINT32_TYPE     m_CycleCounter;         //!< bus cycle count
    UINT32_TYPE     m_FrameCounter;         //!< frame count since last reset
    UINT32_TYPE     m_TriggerCounter;       //!< from camera received triggers
    UINT16_TYPE     m_AOILeft;              //!< left coordinate of the AOI begin
    UINT16_TYPE     m_AOITop;               //!< top coordinate of the AOI begin
    UINT16_TYPE     m_AOIWidth;             //!< width of the AOI
    UINT16_TYPE     m_AOIHeight;            //!< height of the AOI
    UINT32_TYPE     m_Shutter;              //!< camera shutter for the image
    UINT16_TYPE     m_Gain;                 //!< camera gain for the image
    UINT16_TYPE     m_Reserved0;            //!< reserved for future use
    UINT8_TYPE      m_OutputState[4];       //!< state of the output pins
    UINT8_TYPE      m_InputState[2];        //!< state of the input pins
    UINT8_TYPE      m_Reservec1[2];         //!< reserved for future use
    UINT8_TYPE      m_SequenceIndex;        //!< position in the sequence
    UINT8_TYPE      m_Reserved2a;           //!< reserved for future use
    UINT8_TYPE      m_ColorCoding;          //!< IIDC color coding for the transport format
    UINT8_TYPE      m_Reserved2b;           //!< reserved for future use
    UINT32_TYPE     m_SerialNumber;         //!< camera serial number
    UINT32_TYPE     m_UserValue;            //!< user defined value
    S_SIS_ENABLE    m_SISEnable;            //!< state inq struct
} S_SISData,S_SIS_DATA;
//! \cond UNI_CONTROL
#ifndef UNI_TRANSFORM

//! Enumeration for features used with unified access
typedef enum E_UNI_FEATURE
{
    E_FEAT_NONE = 0x0,                          //!< default value
    
    // IIDC Standard Camera Features
    E_FEAT_BRIGHTNESS               = 0x0100,   //!< image brightness feature -     see \ref DESC_FEAT_BRIGHTNESS "E_FEAT_BRIGHTNESS"
    E_FEAT_AUTOEXPOSURE             = 0x0101,   //!< auto exposure feature -        see \ref DESC_FEAT_AUTOEXPOSURE "E_FEAT_AUTOEXPOSURE"
    E_FEAT_SHARPNESS                = 0x0102,   //!< sharpness feature -            see \ref DESC_FEAT_SHARPNESS "E_FEAT_SHARPNESS"
    E_FEAT_WHITEBALANCE_UB          = 0x0103,   //!< white balance (ub) feature -   see \ref DESC_FEAT_WHITEBALANCE_UB "E_FEAT_WHITEBALANCE_UB"
    E_FEAT_WHITEBALANCE_VR          = 0x0104,   //!< white balance (vr) feature -   see \ref DESC_FEAT_WHITEBALANCE_VR "E_FEAT_WHITEBALANCE_VR"
    E_FEAT_HUE                      = 0x0105,   //!< color hue feature -            see \ref DESC_FEAT_HUE "E_FEAT_HUE"
    E_FEAT_SATURATION               = 0x0106,   //!< color saturation feature -     see \ref DESC_FEAT_SATURATION "E_FEAT_SATURATION"
    E_FEAT_GAMMA                    = 0x0107,   //!< gamma correction feature -     see \ref DESC_FEAT_GAMMA "E_FEAT_GAMMA"
    E_FEAT_SHUTTER                  = 0x0108,   //!< camera shutter feature -       see \ref DESC_FEAT_SHUTTER "E_FEAT_SHUTTER"
    E_FEAT_GAIN                     = 0x0109,   //!< signal gain feature -          see \ref DESC_FEAT_GAIN "E_FEAT_GAIN"
    E_FEAT_TRIGGERDELAY             = 0x010a,   //!< trigger delay feature -        see \ref DESC_FEAT_TRIGGERDELAYADV "E_FEAT_TRIGGERDELAY"

    // Advanced (AVT) Camera Features

    E_FEAT_TIMEBASE                 = 0x0200,   //!< shutter timebase feature -                             see \ref DESC_FEAT_TIMEBASE "E_FEAT_TIMEBASE"
    E_FEAT_EXPOSUREOFFSET           = 0x0201,   //!< exposure offset feature -                              see \ref DESC_FEAT_EXPOSUREOFFSET "E_FEAT_EXPOSUREOFFSET"
    E_FEAT_EXTENDEDSHUTTER          = 0x0202,   //!< extended shutter feature -                             see \ref DESC_FEAT_EXTENDEDSHUTTER "E_FEAT_EXTENDEDSHUTTER"
    E_FEAT_LUT                      = 0x0203,   //!< lookup table (LUT) feature -                           see \ref DESC_FEAT_LUT "E_FEAT_LUT"
    E_FEAT_LUT_BITSPERVAL           = 0x0204,   //!< lookup table detail information: bits per value -      see \ref DESC_FEAT_LUT_BITSPERVAL "E_FEAT_LUT_BITSPERVAL"
    E_FEAT_LUT_MAXSIZE              = 0x0205,   //!< lookup table detail information: maximum table size -  see \ref DESC_FEAT_LUT_MAXSIZE "E_FEAT_LUT_MAXSIZE"
    E_FEAT_SHADING                  = 0x0206,   //!< shading correction feature -                           see \ref DESC_FEAT_SHADING "E_FEAT_SHADING"
    E_FEAT_FRAMECOUNTER             = 0x0208,   //!< frame counter feature -                                see \ref DESC_FEAT_FRAMECOUNTER "E_FEAT_FRAMECOUNTER"
    E_FEAT_COLORCORRECTION          = 0x0209,   //!< color correction feature -                             see \ref DESC_FEAT_COLORCORRECTION "E_FEAT_COLORCORRECTION"
    E_FEAT_SOFTRESET                = 0x020a,   //!< soft reset feature -                                   see \ref DESC_FEAT_SOFTRESET "E_FEAT_SOFTRESET"
    E_FEAT_XMIRRORIMAGE             = 0x020b,   //!< horizontal mirror feature -                            see \ref DESC_FEAT_XMIRRORIMAGE "E_FEAT_XMIRRORIMAGE"
    E_FEAT_TRIGGERDELAYADV          = 0x020c,   //!< trigger delay feature -                                see \ref DESC_FEAT_TRIGGERDELAYADV          "E_FEAT_TRIGGERDELAYADV"
    E_FEAT_DSNUCORRECTION           = 0x020e,   //!< dark signal non-uniformity correction feature -        see \ref DESC_FEAT_DSNUCORRECTION           "E_FEAT_DSNUCORRECTION"
    E_FEAT_BLEMISHCORRECTION        = 0x020f,   //!< blemish pixel correction feature -                     see \ref DESC_FEAT_BLEMISHCORRECTION        "E_FEAT_BLEMISHCORRECTION"
    E_FEAT_DELAYEDINTEGRATIONENABLE = 0x0210,   //!< delayed 'integration enable' feature -                 see \ref DESC_FEAT_DELAYEDINTEGRATIONENABLE "E_FEAT_DELAYEDINTEGRATIONENABLE"
    E_FEAT_AUTOSHUTTER_LOW          = 0x0211,   //!< auto shutter minimum feature -                         see \ref DESC_FEAT_AUTOSHUTTER_LOW          "E_FEAT_AUTOSHUTTER_LOW"
    E_FEAT_AUTOSHUTTER_HIGH         = 0x0212,   //!< auto shutter maximum feature -                         see \ref DESC_FEAT_AUTOSHUTTER_HIGH         "E_FEAT_AUTOSHUTTER_HIGH"
    E_FEAT_AUTOGAIN_LOW             = 0x0213,   //!< auto gain minimum feature -                            see \ref DESC_FEAT_AUTOGAIN_LOW             "E_FEAT_AUTOGAIN_LOW"
    E_FEAT_AUTOGAIN_HIGH            = 0x0214,   //!< auto gain maximum feature -                            see \ref DESC_FEAT_AUTOGAIN_HIGH            "E_FEAT_AUTOGAIN_HIGH"
    E_FEAT_AUTOFUNCTIONAOI_XUNITS   = 0x0215,   //!< auto function AOI detail information: x units -        see \ref DESC_FEAT_AUTOFUNCTIONAOI_XUNITS   "E_FEAT_AUTOFUNCTIONAOI_XUNITS"
    E_FEAT_AUTOFUNCTIONAOI_YUNITS   = 0x0216,   //!< auto function AOI detail information: y units -        see \ref DESC_FEAT_AUTOFUNCTIONAOI_YUNITS   "E_FEAT_AUTOFUNCTIONAOI_YUNITS"
    E_FEAT_AUTOFUNCTIONAOI_LEFT     = 0x0217,   //!< auto function AOI left offset -                        see \ref DESC_FEAT_AUTOFUNCTIONAOI_LEFT     "E_FEAT_AUTOFUNCTIONAOI_LEFT"
    E_FEAT_AUTOFUNCTIONAOI_TOP      = 0x0218,   //!< auto function AOI top offset -                         see \ref DESC_FEAT_AUTOFUNCTIONAOI_TOP      "E_FEAT_AUTOFUNCTIONAOI_TOP"
    E_FEAT_AUTOFUNCTIONAOI_WIDTH    = 0x0219,   //!< auto function AOI width -                              see \ref DESC_FEAT_AUTOFUNCTIONAOI_WIDTH    "E_FEAT_AUTOFUNCTIONAOI_WIDTH"
    E_FEAT_AUTOFUNCTIONAOI_HEIGHT   = 0x021a,   //!< auto function AOI height -                             see \ref DESC_FEAT_AUTOFUNCTIONAOI_HEIGHT   "E_FEAT_AUTOFUNCTIONAOI_HEIGHT"
    E_FEAT_HIGHSNR                  = 0x021b,   //!< high SNR mode control -                                see \ref DESC_FEAT_HIGHSNR                  "E_FEAT_HIGHSNR"
    E_FEAT_USERPROFILE              = 0x021c,   //!< user profile feature -                                 see \ref DESC_FEAT_USERPROFILE              "E_FEAT_USERPROFILE"
    E_FEAT_HDR_KNEEPOINTCOUNT       = 0x021d,   //!< HDR mode parameter: number of active knee points -     see \ref DESC_FEAT_HDR_KNEEPOINTCOUNT       "E_FEAT_HDR_KNEEPOINTCOUNT"
    E_FEAT_HDR_KNEEPOINT1           = 0x021e,   //!< HDR mode setting for first knee point -                see \ref DESC_FEAT_HDR_KNEEPOINT1           "E_FEAT_HDR_KNEEPOINT1"
    E_FEAT_HDR_KNEEPOINT2           = 0x021f,   //!< HDR mode setting for second knee point -               see \ref DESC_FEAT_HDR_KNEEPOINT2           "E_FEAT_HDR_KNEEPOINT2"
    E_FEAT_HDR_KNEEPOINT3           = 0x0220,   //!< HDR mode setting for third knee point -                see \ref DESC_FEAT_HDR_KNEEPOINT3           "E_FEAT_HDR_KNEEPOINT3"
    E_FEAT_WHITEBALANCE_GR          = 0x0221,   //!< advanced white balance for complementary sensors - GR[CY] setting - see \ref DESC_FEAT_WHITEBALANCE_GR "E_FEAT_WHITEBALANCE_GR"
    E_FEAT_WHITEBALANCE_MG          = 0x0222,   //!< advanced white balance for complementary sensors - MG[YE] setting - see \ref DESC_FEAT_WHITEBALANCE_MG "E_FEAT_WHITEBALANCE_MG"
    E_FEAT_WHITEBALANCE_CY          = 0x0223,   //!< advanced white balance for complementary sensors - CY[MG] setting - see \ref DESC_FEAT_WHITEBALANCE_CY "E_FEAT_WHITEBALANCE_CY"
    E_FEAT_WHITEBALANCE_YE          = 0x0224,   //!< advanced white balance for complementary sensors - YE[GR] setting - see \ref DESC_FEAT_WHITEBALANCE_YE "E_FEAT_WHITEBALANCE_YE"
    E_FEAT_REDUCEDSMEAR             = 0x0225,   //!< reduced smear mode control -                           see \ref DESC_FEAT_REDUCEDSMEAR "E_FEAT_REDUCEDSMEAR"
    E_FEAT_PARAMUPDATETIMING        = 0x0226,   //!< parameter update timing mode control -                 see \ref DESC_FEAT_PARAMUPDATETIMING "E_FEAT_PARAMUPDATETIMING"
    E_FEAT_SIS                      = 0x0227,   //!< secure image signature feature this feature is present if E_FEAT_SIS_MARLIN or E_FEAT_SIS_PIKE is present - see \ref DESC_FEAT_SIS "E_FEAT_SIS"
    E_FEAT_SIS_MARLIN               = 0x0228,   //!< this feature is used to distinguish between Marlin and Pike SIS Implementations -  see \ref DESC_FEAT_SIS_MARLIN "E_FEAT_SIS_MARLIN"
    E_FEAT_SIS_PIKE                 = 0x0229,   //!< this feature is used to distinguish between Marlin and Pike SIS Implementations -  see \ref DESC_FEAT_SIS_PIKE "E_FEAT_SIS_PIKE"
    E_FEAT_CHANNELGAINBALANCE       = 0x022a,   //!< the channel gain balance of cameras with two channels -  see \ref DESC_FEAT_CHANNELGAINBALANCE "E_FEAT_CHANNELGAINBALANCE"
    E_FEAT_NETWORKPACKETSIZE        = 0x022b,   //!< the size of packets used for image transfer -          see \ref DESC_FEAT_NETWORKPACKETSIZE "E_FEAT_NETWORKPACKETSIZE"
    E_FEAT_TRIGGERCOUNTER           = 0x022c,   //!< trigger counter feature -                              see \ref DESC_FEAT_TRIGGERCOUNTER "E_FEAT_TRIGGERCOUNTER"
    // camera independent API features
    E_FEAT_LOGGING                  = 0x0300,   //!< Parameter to control logging. See \ref DESC_FEAT_LOGGING "E_FEAT_LOGGING"
    E_FEAT_LIST_ALL_CAMERAS         = 0x0301,   //!< Controls if ::UCC_GetCameras() and ::UCC_GetCamerasEx() list cameras which are in use by a different application \ref DESC_FEAT_LIST_ALL_CAMERAS "E_FEAT_LIST_ALL_CAMERAS"
    E_FEAT_CAMERABUSY               = 0x032a,   //!< this feature is used to query if the camera is used by an application - see \ref DESC_FEAT_CAMERABUSY "E_FEAT_CAMERABUSY"
    E_FEAT_OPENREADONLY             = 0x0330,   //!< controls the open mode of a camera \ref DESC_FEAT_OPENREADONLY "E_FEAT_OPENREADONLY"
    E_FEAT_DETECT_EXT_CONTR_CAMERAS = 0x0331,   //!< enables or disables the detection of cameras controlled by external applications \ref DESC_FEAT_DETECT_EXT_CONTR_CAMERAS "E_FEAT_DETECT_EXT_CONTR_CAMERAS"

    // IEEE1394 Driver Specific Features
    E_FEAT_STARTIMMEDIATELY         = 0x0400,   //!< starting parameter for ::UCC_GrabStart(); if (default)TRUE the ISO stream will start immediately \ref DESC_FEAT_STARTIMMEDIATELY "E_FEAT_STARTIMMEDIATELY"
    E_FEAT_PHYSPEED                 = 0x0401,   //!< the physical transmission speed for a camera. See \ref DESC_FEAT_PHYSPEED "E_FEAT_PHYSPEED"
    E_FEAT_FRAMEBUFFERCOUNT         = 0x0402,   //!< the number of frame buffers to be used by the driver - See \ref DESC_FEAT_FRAMEBUFFERCOUNT "E_FEAT_FRAMEBUFFERCOUNT"
    E_FEAT_USEIRMFORBW              = 0x0403,   //!< a flag to signal if the driver-internal isochronous resource manager should manage the bandwidth - See \ref DESC_FEAT_USEIRMFORBW "E_FEAT_USEIRMFORBW"
    E_FEAT_FRAMEMEMORYSIZE          = 0x0404,   //!< the size of one complete image in bytes. Read-only. See \ref DESC_FEAT_FRAMEMEMORYSIZE "E_FEAT_FRAMEMEMORYSIZE"
    E_FEAT_IRMFREEBW                = 0x0405,   //!< the size of the free bandwidth that is reported by the enabled isochronous resource manager - See \ref DESC_FEAT_IRMFREEBW "E_FEAT_IRMFREEBW"
    E_FEAT_USEIRMFORCHN             = 0x0406,   //!< a flag to signal if the driver-internal isochronous resource manager should manage the isochronous channel. See \ref DESC_FEAT_USEIRMFORCHN "E_FEAT_USEIRMFORCHN"
    E_FEAT_CAMACCEPTDELAY           = 0x0407,   //!< the time-out length (in milliseconds) for the time the driver waits for a reaction to an asynchronous command - See \ref DESC_FEAT_CAMACCEPTDELAY "E_FEAT_CAMACCEPTDELAY"
    E_FEAT_ISOCHANNEL               = 0x0408,   //!< a flag to signal if the driver should provide an isochronous resource manager. Enabled by default. See \ref DESC_FEAT_ISOCHANNEL "E_FEAT_ISOCHANNEL"
    E_FEAT_CYCLETIME                = 0x0409,   //!< the current cycle time on the bus Read-only. See \ref DESC_FEAT_CYCLETIME "E_FEAT_CYCLETIME"
    E_FEAT_BURSTCOUNT               = 0x040a,   //!< number of images to grab in bus trigger mode\ref DESC_FEAT_BURSTCOUNT "E_FEAT_BURSTCOUNT"
    E_FEAT_DO_BUSTRIGGER            = 0x040b,   //!< broadcast ISO enable message \ref DESC_FEAT_DO_BUSTRIGGER "E_FEAT_DO_BUSTRIGGER"
    E_FEAT_DO_MULTIBUSTRIGGER       = 0x040c,   //!< broadcast ISO enable message on all selected cards \ref DESC_FEAT_DO_MULTIBUSTRIGGER "E_FEAT_DO_MULTIBUSTRIGGER"
    E_FEAT_DO_CARDRESET             = 0x040e    //!< soft reset the Firewire card the camera is connected to. See \ref DESC_FEAT_DO_CARDRESET "E_FEAT_DO_CARDRESET".
} E_UniFeature, E_UNI_FEATURE;

//! Enumeration for feature states
typedef enum E_UNI_FEATURE_STATE
{
    E_FEATSTATE_OFF                 =0,     //!< read: feature state is off;<br>write: disable feature
    E_FEATSTATE_ON                  =1,     //!< read: feature is enabled; <br>write: enable feature
    E_FEATSTATE_ONEPUSH             =2,     //!< read: feature is temporarily in "OnePush" state; <br>write: Do "OnePush" action for this feature
    E_FEATSTATE_AUTO                =3,     //!< read: feature is in "Auto" state until set to "Off" again; <br>write: enable automatic adjustment mode for this feature
    E_FEATSTATE_BUSY                =4,     //!< read: feature is in "Busy" state;
    E_FEATSTATE_SHOWIMAGE           =5,     //!< read: feature is in "ShowImage" state; <br>write: enable "ShowImage" state for this feature
    E_FEATSTATE_LOADDATA            =6,     //!< read: feature is temporarily in "LoadData" state; <br>write: enable "LoadData" state for this feature
    E_FEATSTATE_SAVEDATA            =7,     //!< read: feature is temporarily in "SaveData" state; <br>write: enable "SaveData" state for this feature
    E_FEATSTATE_CLEARDATA           =8,     //!< read: feature is temporarily in "ClearData" state; <br>write: enable "ClearData" state for this feature
    E_FEATSTATE_ERROR               =9,     //!< read: feature state could not be set
    E_FEATSTATE_UNDEFINED           =10     //!< read: feature state is undefined
} E_UniFeatureState, E_UNI_FEATURE_STATE;

//! Enumeration for retrieval of module information
typedef enum E_UNI_MODULE_INFO
{
    E_MI_VERSION_MAIN=0,            //!< version of the main UniControl DLL
    E_MI_VERSION_UNICONTROL_F,      //!< version of the Firewire UniControl DLL
    E_MI_VERSION_UNICONTROL_P,      //!< version of the GigE UniControl DLL
    E_MI_LAST
} E_UniModuleInfo, E_UNI_MODULE_INFO;

//! Enumeration for retrieval of camera information
typedef enum E_UNI_CAMERA_INFO
{
    E_CAMINFO_MODEL=0,                  //!< model info
    E_CAMINFO_VENDOR,                   //!< vendor info
    E_CAMINFO_IIDCVERSION,              //!< IIDC version info
    E_CAMINFO_SN,                       //!< serial number
    E_CAMINFO_ORDERID,                  //!< order ID
    E_CAMINFO_MCVERSION,                //!< Micro controller version info
    E_CAMINFO_FPGAVERSION,              //!< FPGA version info
    E_CAMINFO_CUSTOMERKEY,              //!< customer key info
    E_CAMINFO_MAXWIDTH=20,              //!< maximum width of the camera
    E_CAMINFO_MAXHEIGHT,                //!< maximum height of the camera
    E_CAMINFO_MAXDEPTH,                 //!< maximum bit depth of the camera
    E_CAMINFO_COLORCAMERA,              //!< Is this a color camera?
    E_CAMINFO_BAYERPATTERN,             //!< Current Bayer pattern for the current image settings (see ::E_UNI_BAYER_PATTERN)
    E_CAMINFO_INTERLACED,               //!< Is this an interlaced camera?
    E_CAMINFO_SISTYPE,                  //!< type of SIS supported by the camera
    E_CAMINFO_INTERFACETYPE,            //!< type of camera interface (see ::E_UNI_INTERFACE)
    E_CAMINFO_IMAGE_ENDIANNESS,         //!< endianness of image data (see ::E_UNI_ENDIANNESS)
    E_CAMINFO_IMAGE_ALIGNMENT,          //!< alignment of image data (see ::E_UNI_ALIGNMENT)
    E_CAMINFO_BUS_LOAD,                 //!< currently configured bus load (in 1/1000)(see ::UCC_PrepareFreeGrab)
    E_CAMINFO_HOST_IPADDRESS,           //!< IP address of host currently controlling the camera (GigE cameras only)
    E_CAMINFO_HOST_MACADDRESS,          //!< MAC address of host currently controlling the camera (GigE cameras only)
    E_CAMINFO_CONTROLLEDEXTERNALLY,     //!< Is this camera controlled by another application or host? (GigE cameras only)
    E_CAMINFO_READONLY,                 //!< Is this camera opened in read-only mode? (GigE cameras only)
    E_CAMINFO_CAMERA_IPADDRESS,         //!< camera's IP address (GigE cameras only)
    E_CAMINFO_CAMERA_MACADDRESS,        //!< camera's MAC address (GigE cameras only)
    E_CAMINFO_RECEIVED_PACKETS,         //!< number of received packets with image data (GigE cameras only)
    E_CAMINFO_PACKET_RESEND_REQUESTS,   //!< number of missing packets requested to the camera for resent (GigE cameras only)
    E_CAMINFO_PACKET_RESEND_RECEIVED,   //!< number of missing packets resent by the camera and received by the host (GigE cameras only)
    E_CAMINFO_OPEN,                     //!< Is this camera opened?
    E_CAMINFO_CARD_ID,                  //!< id of fire wire card
    E_CAMINFO_LAST
} E_UniCameraInfo, E_UNI_CAMERA_INFO;

//! Enumeration for Free Modes to be used by ::UCC_PrepareFreeGrab.
typedef enum E_UNI_FREE_MODES
{
    E_FM_IIDC_0              = 0,            //!< IIDC format 7 mode 0
    E_FM_IIDC_1              = 1,            //!< IIDC format 7 mode 1
    E_FM_IIDC_2              = 2,            //!< IIDC format 7 mode 2
    E_FM_IIDC_3              = 3,            //!< IIDC format 7 mode 3
    E_FM_IIDC_4              = 4,            //!< IIDC format 7 mode 4
    E_FM_IIDC_5              = 5,            //!< IIDC format 7 mode 5
    E_FM_IIDC_6              = 6,            //!< IIDC format 7 mode 6
    E_FM_IIDC_7              = 7,            //!< IIDC format 7 mode 7
    E_FM_VIRTUAL_BINNING     = 0x01000000,   //!< base for virtual binning modes
    E_FM_VIRTUAL_SUBSAMPLING = 0x02000000,   //!< base for virtual sub-sampling modes
    E_FM_VIRTUAL_MASK        = 0x03000000    //!< mask for virtual modes
} E_UniFreeModes, E_UNI_FREE_MODES;

//! Enumeration for Free Mode information used by ::UCC_GetFreeModeInfo.
typedef enum E_UNI_FREE_MODE_INFO
{
    E_FREE_MODE_WIDTH           = 0,    //!< max image width in pixels
    E_FREE_MODE_HEIGHT          = 1,    //!< max image height in pixels
    E_FREE_MODE_PIXELCOUNT      = 2,    //!< number of pixels of the image
    E_FREE_MODE_BYTE_PER_FRAME  = 3,    //!< bytes per frame
    E_FREE_MODE_UNIT_SIZE_H     = 4,    //!< AOI width values have to be multiples of this unit size
    E_FREE_MODE_UNIT_SIZE_V     = 5,    //!< AOI height values have to be multiples of this unit size
    E_FREE_MODE_UNIT_POS_H      = 6,    //!< AOI left values have to be multiples of this unit size
    E_FREE_MODE_UNIT_POS_V      = 7     //!< AOI top values have to be multiples of this unit size
} E_UniFreeModeInfo, E_UNI_FREE_MODE_INFO;

//! Enumeration for I/O speed (for configuration of serial I/O communication)
typedef enum E_UNI_SIO_MODE
{
    E_SIOMODE_DISABLED=0,       //!< no serial transfer
    E_SIOMODE_RECEIVE,          //!< only allow reception of serial input
    E_SIOMODE_TRANSMIT,         //!< only allow transmission of serial output
    E_SIOMODE_RECEIVETRANSMIT,  //!< allow both directions of the serial transfer
    E_SIOMODE_UNDEFINED
} E_UniSioMode, E_UNI_SIO_MODE;

//! Enumeration for I/O stop bits (for configuration of serial I/O communication)
typedef enum E_UNI_SIO_STOPBITS
{
    E_SIOSTOPBITS_1=0,      //!< 1 stop bit
    E_SIOSTOPBITS_1_5,      //!< 1.5 stop bits
    E_SIOSTOPBITS_2,        //!< 2 stop bits
    E_SIOSTOPBITS_UNDEFINED
} E_UniSioStopbits, E_UNI_SIO_STOPBITS;

//! Enumeration for I/O parity (for configuration of serial I/O communication)
typedef enum E_UNI_SIO_PARITY
{
    E_SIOPARITY_NONE=0,     //!< no parity info
    E_SIOPARITY_ODD,        //!< odd parity
    E_SIOPARITY_EVEN,       //!< even parity
    E_SIOPARITY_UNDEFINED
} E_UniSioParity, E_UNI_SIO_PARITY;

//! Enumeration for I/O speed (for configuration of serial I/O communication)
typedef enum E_UNI_SIO_BAUDRATE
{
    E_SIOBAUDRATE_300=0,        //!< 300 Baud
    E_SIOBAUDRATE_600,          //!< 600 Baud
    E_SIOBAUDRATE_1200,         //!< 1200 Baud
    E_SIOBAUDRATE_2400,         //!< 2400 Baud
    E_SIOBAUDRATE_4800,         //!< 4800 Baud
    E_SIOBAUDRATE_9600,         //!< 9600 Baud
    E_SIOBAUDRATE_19200,        //!< 19200 Baud
    E_SIOBAUDRATE_38400,        //!< 38400 Baud
    E_SIOBAUDRATE_57600,        //!< 57600 Baud
    E_SIOBAUDRATE_115200,       //!< 115200 Baud
    E_SIOBAUDRATE_230400,       //!< 230400 Baud
    E_SIOBAUDRATE_UNDEFINED
} E_UniSioBaudRate, E_UNI_SIO_BAUDRATE;

//! Enumeration for 1394 speed (for FireWire only, deprecated use ::E_UNI_DATABITRATE)
typedef enum E_UNI_1394_SPEED
{
    E_SPEED_AUTO= -1,   //!< automatic speed detection by the driver
    E_SPEED100=0,       //!< S100 (100 MBit/s)
    E_SPEED200,         //!< S200 (200 MBit/s)
    E_SPEED400,         //!< S400 (400 MBit/s)
    E_SPEED800,         //!< S800 (800 MBit/s)
    E_SPEED_LAST        //!< last value for this enumeration
} E_Uni1394Speed, E_UNI_1394_SPEED;

//! Enumeration for stream speeds (FireWire and GigE, needed for feature ::E_FEAT_PHYSPEED)
typedef enum E_UNI_DATABITRATE
{
    E_DATABITRATE_AUTO= -1,   //!< automatic speed detection by the driver

    E_DATABITRATE_F100=0,      //!< FireWire S100 (100 MBit/s)
    E_DATABITRATE_F200,        //!< FireWire S200 (200 MBit/s)
    E_DATABITRATE_F400,        //!< FireWire S400 (400 MBit/s)
    E_DATABITRATE_F800,        //!< FireWire S800 (800 MBit/s)

    E_DATABITRATE_G10=100,     //!< GigE 10 MBit/s
    E_DATABITRATE_G100,        //!< GigE 100 MBit/s
    E_DATABITRATE_G1000,       //!< GigE 1 GBit/s
    E_DATABITRATE_G2000,       //!< GigE 2 GBit/s (via Link Aggregate Group LAG)
    E_DATABITRATE_G10000,      //!< GigE 10 GBit/s

    E_DATABITRATE_LAST         //!< last value for this enumeration
} E_UniDataBitRate, E_UNI_DATABITRATE;

/** Enumeration for output image formats supported by ::UCC_GetImage, ::UCC_GetImageEx, ::UCC_GrabImage and ::UCC_GrabImageEx.
* \note Image formats ::E_IF_MONO8 and ::E_IF_BGR have been removed due to accuracy issues. Instead, ::E_IF_MONO_8BPP
* and ::E_IF_BGR_24BPP should be used. Unlike their outdated counterparts, ::E_IF_MONO_8BPP and ::E_IF_BGR_24BPP
* consistently result in the expected image format. As a consequence, existing applications may require additional
* changes when the new formats are adopted. Alternatively, the macro USE_LEGACY_IMAGE_FORMATS can be defined for 
* existing projects. This activates compatibility definitions, allowing recompilation of existing applications 
* without any changes.
*/
typedef enum E_UNI_IMAGE_FORMAT
{
    E_IF_LEGACY1=0,    //!< Outdated legacy format - do not use for new applications (previously ::E_IF_MONO8)
    E_IF_LEGACY2,      //!< Outdated legacy format - do not use for new applications (previously ::E_IF_BGR)
    E_IF_RGB_PLANES=3, //!< Three separate color plane bitmaps
    E_IF_RGBY_PLANES,  //!< Three separate color plane bitmaps plus intensity plane bitmap
    E_IF_NATIVE,       //!< Native transfer format as supported by 1394 DCAM cameras. For compatibility reasons, GigE-Vision image frames are converted to match DCAM image formats. Use ::E_IF_NATIVE_EX to obtain the actual, interface-dependent transfer format. Memory requirements depend on the color code. 
    E_IF_NATIVE_EX,    //!< Native transfer format. The output format may differ between 1394 DCAM and GigE-Vision cameras, even if the same color code is set. ::UCC_GetImageEx, ::UCC_GrabImageEx and ::UCC_GetNativeImageEx return necessary format details as ::S_UNI_TRANSPORT_FORMAT_INFO. Memory requirements depend on the color code. 
    E_IF_MONO_8BPP,    //!< 8 bit per pixel mono format
    E_IF_BGR_24BPP,    //!< 24 bit per pixel BGR format (windows bitmap format)
    E_IF_LAST=9
} E_UniImageFormat, E_UNI_IMAGE_FORMAT;

#ifdef USE_LEGACY_IMAGE_FORMATS
#define E_IF_MONO8 E_IF_LEGACY1 //!< define for legacy usage; for new applications, use ::E_IF_MONO_8BPP
#define E_IF_BGR E_IF_LEGACY2   //!< define for legacy usage; for new applications, use ::E_IF_BGR_24BPP
#endif

//! Enumeration for frame rates.
typedef enum E_UNI_FRAME_RATE
{
    E_FR_1_875=0,  //!< 1.875 FPS
    E_FR_3_75,     //!< 3.75 FPS
    E_FR_7_5,      //!< 7.5 FPS
    E_FR_15,       //!< 15 FPS
    E_FR_30,       //!< 30 FPS
    E_FR_60,       //!< 60 FPS
    E_FR_120,      //!< 120 FPS
    E_FR_240,      //!< 240 FPS
    E_FR_LAST
} E_UniFrameRate, E_UNI_FRAME_RATE;


//! Enumeration for controlling logging
typedef enum E_UNI_LOGGING
{
    E_LOG_FATAL=0,          //!< Log only fatal events
    E_LOG_ERROR,            //!< Log fatal events and errors
    E_LOG_WARNING,          //!< Log fatal events, errors and warnings
    E_LOG_INFO,             //!< Log fatal events, errors, warnings and info messages
    E_LOG_DEBUG,            //!< Log everything
    E_LOG_LEVELMASK=7,      //!< Mask for testing log levels
    //EL_FLUSH_AUTO=8,      // Send every line of logging directly to the log file
    //EL_FLUSH_BUFFERED=16, // Send logging messages to the log file every 16384 chars
    //EL_FLUSH_NOW=32,      // Flush all buffered logging messages to the log file
    E_LOG_LAST
} E_UniLogging, E_UNI_LOGGING;

//! Enumeration for resolution presets
typedef enum E_UNI_RESOLUTION_PRESETS
{
    E_PRESET_BEGIN   =0,
    E_PRESET_QVGA=0,        //!<   320 x  240,      aspect ratio 4:3
    E_PRESET_VGA,           //!<   640 x  480,      aspect ratio 4:3
    E_PRESET_PAL,           //!<   768 x  576,      aspect ratio 4:3
    E_PRESET_SVGA,          //!<   800 x  600,      aspect ratio 4:3
    E_PRESET_XGA,           //!<  1024 x  768,      aspect ratio 4:3
    E_PRESET_XVGA,          //!<  1280 x  960,      aspect ratio 4:3
    E_PRESET_SXGAP,         //!<  1400 x 1050,      aspect ratio 4:3
    E_PRESET_UXGA,          //!<  1600 x 1200,      aspect ratio 4:3
    E_PRESET_QXGA,          //!<  2048 x 1536,      aspect ratio 4:3
    E_PRESET_SXGA,          //!<  1280 x 1024,      aspect ratio 5:4
    E_PRESET_QSXGA,         //!<  2580 x 2048,      aspect ratio 5:4
    E_PRESET_WVGA,          //!<   854 x  480,      aspect ratio 16:9 
    E_PRESET_HD720,         //!<  1280 x  720,      HDTV, aspect ratio 16:9
    E_PRESET_HD1080,        //!<  1920 x 1080,      HDTV, aspect ratio 16:9
    E_PRESET_DC2K,          //!<  2048 x 1080,      Digital Cinema // Line binning 16/9
    E_PRESET_DC4K,          //!<  4096 x 2160,
    E_PRESET_END
} E_UniResolutionPresets, E_UNI_RESOLUTION_PRESETS;

//! Delay in ms for device enumeration after UCC_Init, only needed for GigE
#define UNI_GIGE_ENUMERATION_DELAY 2000  

#endif // UNI_TRANSFORM
//! \endcond

//! \cond UNI_TRANSFORM_NODOTNET
/*------------------------------------ Structs --------------------------------*/
/***/
/** Structure for accessing Windows RGB data */
typedef struct S_RGB8
{
    UINT8_TYPE R;   //!< red part
    UINT8_TYPE G;   //!< green part
    UINT8_TYPE B;   //!< blue part
}  S_RGB8;

/** Structure for accessing Windows RGB data */
typedef struct S_BGR8
{
    UINT8_TYPE B;   //!< blue part
    UINT8_TYPE G;   //!< green part
    UINT8_TYPE R;   //!< red part
} S_BGR8, S_BGR24;

/** Structure for accessing non-Windows RGB data */
typedef struct S_RGBA8
{
    UINT8_TYPE R;   //!< red part
    UINT8_TYPE G;   //!< green part
    UINT8_TYPE B;   //!< blue part
    UINT8_TYPE A;   //!< additional part
} S_RGBA8, S_RGBA32;

/** Structure for accessing Windows RGBA data.*/
typedef struct S_BGRA8
{
    UINT8_TYPE B;   //!< blue part
    UINT8_TYPE G;   //!< green part
    UINT8_TYPE R;   //!< red part
    UINT8_TYPE A;   //!< unused
} S_BGRA8, S_BGRA32;

//! Structure for accessing 16-bit Windows-type RGB data
typedef struct S_BGR16
{
    UINT16_TYPE B;  //!< blue part
    UINT16_TYPE G;  //!< green part
    UINT16_TYPE R;  //!< red part
} S_BGR16;

/**Structure for accessing data in the YUV 4:4:4 format (YUV)
    prosilica component order
*/
typedef struct S_YUV444
{
    UINT8_TYPE U;   //!< U
    UINT8_TYPE Y;   //!< Luma
    UINT8_TYPE V;   //!< V
} S_YUV444;

//! Structure for accessing data in the YUV 4:2:2 format (UYVY)
typedef struct S_YUV422
{
    UINT8_TYPE U;   //!< the U part for both pixels
    UINT8_TYPE Y0;  //!< the intensity of the first pixel
    UINT8_TYPE V;   //!< the V part for both pixels
    UINT8_TYPE Y1;  //!< the intensity of the second pixel
}  S_YUV422;

//! Structure for accessing data in the YUV 4:1:1 format (UYYVYY)
typedef struct S_YUV411
{
    UINT8_TYPE U;   //!< the U part for all four pixels
    UINT8_TYPE Y0;  //!< the intensity of the first pixel
    UINT8_TYPE Y1;  //!< the intensity of the second pixel
    UINT8_TYPE V;   //!< the V part for all four pixels
    UINT8_TYPE Y2;  //!< the intensity of the third pixel
    UINT8_TYPE Y3;  //!< the intensity of the fourth pixel
}  S_YUV411;

//! \endcond

/** Simple image type enum. 
*   Enumeration for simple image types with one image plane consisting of consecutive pixel (starting at the top of the image).
*/
typedef enum E_UNI_SIMPLE_IMAGE_FORMAT
{
    E_SIF_MONO8_REC601       ,   //!< the Y value is defined by the Rec. 601
    E_SIF_BGR24              ,   //!< windows 24 bit BGR
    E_SIF_BGRA32             ,   //!< windows 32 bit BGRA
    E_SIF_MONO8_REC709       ,   //!< the Y value is the Y from the CIE - XYZ color space defined in Rec. 709
    E_SIF_UYVY               ,   //!< IIDC YUV422 full color space
    E_SIF_MONO16_REC601      ,   //!< 16 bit mono format (Y defined by the Rec. 601)
    E_SIF_MONO16_REC709      ,   //!< 16 bit mono format (Y defined by the Rec. 709)
    E_SIF_BGR48              ,   //!< windows 48 bit BGR
    E_SIF_LAST
}E_UniSimpleImageFormat, E_UNI_SIMPLE_IMAGE_FORMAT;

/** Structure providing an exact description of the image transfer format. 
* This structure is used for unified processing of 16-bit mono/raw images from Firewire and GigE-Vision cameras.
*/
typedef struct S_UNI_TRANSPORT_FORMAT_INFO
{
    UINT32_TYPE         eColorCode;         //!< general color code, as configured by the user (see \ref E_UNI_COLOR_CODE)
    UINT32_TYPE         eAlignment;         //!< alignment of data words, relevant for 16 bit modes with bit depth < 16 (see \ref E_UNI_ALIGNMENT)
    UINT32_TYPE         nBitDepth;          //!< actual bit depth
    UINT32_TYPE         eEndianness;         //!< endianness of data words, relevant for 16 bit modes (see \ref E_UNI_ENDIANNESS)
    UINT32_TYPE         eBayerPattern;      //!< Bayer pattern, relevant for raw modes (see \ref E_UNI_BAYER_PATTERN)
    UINT32_TYPE         nReserved0;         //!< reserved, must not be used
    UINT32_TYPE         nReserved1;         //!< reserved, must not be used
    UINT32_TYPE         nReserved2;         //!< reserved, must not be used
    UINT32_TYPE         nReserved3;         //!< reserved, must not be used
} S_UNI_TRANSPORT_FORMAT_INFO;

//! \cond UNI_CONTROL
/***/

#ifndef UNI_TRANSFORM

/** Structure for image data access supporting different formats.
 *  
 *  Using this structure, it is possible to request any of the supported image
 *  formats with the same function, e.g. ::UCC_GrabImage(). An instance of this
 *  structure has to be created and initialized by the user before function call.
 *  For initialization \c uImageformat has to be set to the desired image format.
 *  Memory space used for image data has to be provided by the user. According to 
 *  the selected format, one or more memory segments are used. Therefore, one or
 *  more of \ref S_UNI_IMAGE "S_UniImage's" data pointers are needed to point to sufficiently sized
 *  memory segments. Image formats other than ::E_IF_RGB_PLANES and ::E_IF_RGBY_PLANES
 *  generally use \c pImageAsSinglePlane.<br>
 *  Example:
 \verbatim
        UINT8_TYPE * pImgBytes=NULL;
        if( !m_Img.IsNull() )
            pImgBytes = (BYTE *)m_Img.GetBits();

        UINT32_TYPE bColor=0;
        S_UNI_IMAGE MyImageStruct;

        MyImageStruct.uImageformat = E_IF_MONO8;
        MyImageStruct.sImageData.pImageAsSinglePlane = pImgBytes;

        UNI_RETURN_TYPE hr = UNI_GrabImage( m_CamId, &MyImageStruct , &bColor, INFINITE );
\endverbatim
 */
typedef struct S_UNI_IMAGE
{
    UINT32_TYPE uImageformat;       //!< significator of the image format - for valid values see \ref E_UNI_IMAGE_FORMAT 
    S_SIS_DATA  SISData;            //!< secure image signature for the current image
    BOOL_TYPE   SISEnabled;         //!< meta info about SISData, if true SISData holds the secure image signature
    union
    {
        UINT8_TYPE * pImageAsSinglePlane; //!< Bitmap
        struct S_ImageAsMultiPlanes
        {
            UINT8_TYPE *pImageRed;      //!< Pointer to red plane
            UINT8_TYPE *pImageGreen;    //!< Pointer to green plane
            UINT8_TYPE *pImageBlue;     //!< Pointer to blue plane
            UINT8_TYPE *pImageY;        //!< Pointer to intensity plane
        } ImageAsMultiPlanes;           //!< RGBY planes image
    } sImageData;
} S_UniImage, S_UNI_IMAGE;

/**Frame rates for IIDC video modes.
*/
typedef struct S_IIDC_VIDEOINFO_FPS
{
    UINT32_TYPE m_FpsInq[8];            //!< support for IIDC frame rate
} S_IIDCVideoInfoFps, S_IIDC_VIDEOINFO_FPS;

/**IIDC video modes.
*/
typedef struct S_IIDC_VIDEOINFO_MODE
{
    S_IIDC_VIDEOINFO_FPS m_ModeInq[8];  //!< frame rates for the modes
}S_IIDC_VideoInfoMode, S_IIDC_VIDEOINFO_MODE;

/**IIDC video formats.
*/
typedef struct S_IIDC_VIDEOINFO_FORMAT
{
    S_IIDC_VIDEOINFO_MODE m_FormatInq[3];   //!< modes for the formats
}S_IIDC_VideoInfoFormat, S_IIDC_VIDEOINFO_FORMAT;
#define UNI_MAX_SIO_TX_DATA 64


//! Defines for inputs
#define UNI_MAX_INPUT_PINS 4            //!< Maximal number of input Pins
#define UNI_MAX_OUTPUT_PINS 4           //!< Maximal number of output pins

/** Input Pin Modes.
*/
typedef enum E_INPUTPIN_MODES
{
    E_IPINMODE_OFF          = 0x00,         //!< input on this pin is ignored
    E_IPINMODE_TRIGGER      = 0x02,         //!< input on this pin is a trigger signal
// Sequence mode is not yet supported by UniAPI.
//     E_IPINMODE_SEQ_STEP     = 0x06,         //!< input on this pin increases the sequence index
//     E_IPINMODE_SEQ_RESET    = 0x07          //!< input on this pin resets the current sequence
}E_InputPinModes, E_INPUTPIN_MODES;

/** Output Pin Modes.
*/
typedef enum E_OUTPUTPIN_MODES
{
    E_OPINMODE_OFF                  = 0x00,     //!< pin disabled
    E_OPINMODE_PINSTATE             = 0x01,     //!< output pin state determined by software (register), don't combine with inverted polarity
    E_OPINMODE_INTENA               = 0x02,     //!< signaling while integrating image
    E_OPINMODE_FRAMEVALID           = 0x06,     //!< signaling after integration, when frame is ready
    E_OPINMODE_BUSY                 = 0x07,     //!< signaling while the camera is busy (during exposure, sensor read out and data transmission)
    E_OPINMODE_FOLLOW_IN            = 0x08,     //!< only for output pins with a corresponding input pin: the state of the input pin
    E_OPINMODE_PWM                  = 0x09,     //!< create a pulse width modulation signal on this pin
    E_OPINMODE_WAITINGFORTRIGGER    = 0x0A,     //!< signaling according to the "Waiting For Trigger" signal
    E_OPINMODE_UNKNOWN              = 0xFF      //!< unknown mode encountered
}E_OutputPinModes, E_OUTPUTPIN_MODES;

/** Trigger modes.
*/
typedef enum E_TRIGGER_MODES
{
    E_TRIGGERMODE_HW_FIXED_SHUTTER         = 0x00,         //!< a trigger signal starts integration with the preset shutter time
    E_TRIGGERMODE_HW_EDGE                  = E_TRIGGERMODE_HW_FIXED_SHUTTER,
    E_TRIGGERMODE_HW_INTEGRATION_ENABLE    = 0x01,         //!< the trigger signal determines the integration time; the preset shutter time is ignored
    E_TRIGGERMODE_HW_LEVEL                 = E_TRIGGERMODE_HW_INTEGRATION_ENABLE,
    E_TRIGGERMODE_HW_BULK                  = 0x0F,         //!< the trigger signal starts a series of integrations with the preset shutter time
// Software trigger is not yet supported by UniAPI.
//     E_TRIGGERMODE_SW_FIXED_SHUTTER         = 0xE0,         //!< a trigger signal starts integration with the preset shutter time
//     E_TRIGGERMODE_SW_EDGE                  = E_TRIGGERMODE_SW_FIXED_SHUTTER,
//     E_TRIGGERMODE_SW_INTEGRATION_ENABLE    = 0xE1,         //!< the trigger signal determines the integration time; the preset shutter time is ignored
//     E_TRIGGERMODE_SW_LEVEL                 = E_TRIGGERMODE_SW_INTEGRATION_ENABLE,
//     E_TRIGGERMODE_SW_BULK                  = 0xEF,         //!< the trigger signal starts a series of integrations with the preset shutter time
}E_TriggerModes, E_TRIGGER_MODES;

/** License modes of Firegrab.
*/
typedef enum E_UNI_LICENSE_TYPE
{
    E_LT_NOLICENSE      = 0,    //!< No license
    E_LT_CARDGUID       = 1,    //!< License is based on FireWire card
    E_LT_HARDDISK       = 2,    //!< License is based on hard disk
    E_LT_ETHERNET       = 3,    //!< License is based on ethernet adapter
    E_LT_VENDOR         = 4,    //!< Vendor global license
    E_LT_DATELIMITED    = 5,    //!< License expires with date
    E_LT_DEVICE         = 6,    //!< Device specific license
    E_LT_FRAMELIMITED   = 7,     //!< Frame-limited demo license
    E_LT_NOTLIMITED     = 8     //!< no license limit
}E_UniLicenseType, E_UNI_LICENSE_TYPE;

/** Mode for selecting the camera entry from a settings file.
*/
typedef enum E_UNI_SETTINGS_SELECT_MODE
{
    E_SSM_USE_TYPE              = 0,                    //!< Select settings by type
    E_SSM_USE_GUID              = 1,                    //!< Select settings by camera serial number
    E_SSM_USE_GUID_FIRMWARE     = 0x80|E_SSM_USE_GUID   //!< Select settings by firmware and camera serial number
}E_UniSettingsSelectMode, E_UNI_SETTINGS_SELECT_MODE;

/** IP configuration modes.
*
* \note This enumeration is for GigE only.
*/
typedef enum E_UNI_IP_CONFIGURATION_MODE
{
    E_IPCM_PERSISTENT   = 1,    //!< Use persistent (fixed) IP address stored in the camera.
    E_IPCM_DHCP         = 2,    //!< Ask a DHCP server for an IP address. Fall back to Auto IP in case of failure.
    E_IPCM_AUTO_IP      = 4     //!< Use an automatically assigned IP address (LLA = link-local address).
}E_UniIpConfigurationMode, E_UNI_IP_CONFIGURATION_MODE;

#endif // !UNI_TRANSFORM
//! \endcond

#endif // UNI_DEFINES_H_
