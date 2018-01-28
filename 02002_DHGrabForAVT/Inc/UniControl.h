#ifndef UNI_CONTROL_H
#define UNI_CONTROL_H
#pragma once
/** \file UniControl.h
* \brief This file describes all the available AVT Universal API Camera Control methods.
*
* All the AVT Universal API methods for controlling cameras that are publicly available are described
*  in this part of the help. For a description see the section "\link getstart Getting started
*  \endlink " of this help.
*
* \version 2.0
*
* \date 29-Sep-2009
*
* \author Philipp Beyer, Holger Eddelbuettel, Matthias Heidenreich, Olaf Reuter
*
* \par license
* This code is the property of Allied Vision Technologies. Nevertheless, you
* are absolutely free to use and modify it. The code is provided "as is" with
* no expressed or implied warranty. The author and AVT accept no liability if
* it causes any damage to your computer.
*
*/

#include "uni_defines.h"
#include "uni_notification.h"

#ifdef __cplusplus

extern "C" {
#endif

#ifndef UCC_NO_EXPORT
#   ifdef WIN32DLL_EXPORTS
#       ifndef WIN32DLL_API
#           ifndef _WIN64
#               define WIN32DLL_API __declspec (dllexport) __stdcall
#           else
#               define WIN32DLL_API __stdcall
#           endif
#       endif // WIN32DLL_API
#   else // !WIN32DLL_EXPORTS:
#       ifndef WIN32DLL_API
#           define WIN32DLL_API __declspec (dllimport) __stdcall
#       endif // !Win32DLL_API
#   endif // ifdef WIN32DLL_EXPORTS
#else // UCC_NO_EXPORT:
#   ifndef WIN32DLL_API
#       define WIN32DLL_API
#    endif
#endif

/** @name Non-camera methods. 
A set of methods to deal with all actions not directly related to cameras.
*/
/*@{*/

/**
* Initialization of the API.
* This operation must be executed before any other call in this API. To end your usage of this API,
*  use ::UCC_Release
* 
* \return   <dl>    
*           <dt>S_OK</dt>                               <dd>if the Initialization was successful</dd>
            <dt>::UNI_RESULT_API_NOT_INITIALIZED</dt>   <dd>if this module could not be initialized</dd>
            </dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Init (  );

/**
* Deinitialization of the API.
* All actions needed to clean up the API is done.
*  This operation must be executed after the usage of this API has ended.
*
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_API_NOT_INITIALIZED</dt><dd>if this module was not initialized before this call</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Release ( );

/** Get the version number of the API.
* The format is 0x01020304 for version 1.2.3.4
*
* \param[out]  pnValue  pointer to ::UINT32_TYPE, which returns the library version (Major,Minor,SubMinor,Build)
* \return      <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*              <dt>::UNI_RESULT_API_NOT_INITIALIZED</dt><dd>if this module was not initialized before this call</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetVersion ( UINT32_OUT_TYPE   pnValue );

/** Get information about a module of the API.
* Used for returning the version numbers of the sub-modules of the UniAPI.
* The format for versions is 0x01020304 for version 1.2.3.4
*
* \param[in]   nModule  value indicating the index of the module of the UniAPI information is wanted for
                        (see ::E_UNI_MODULE_INFO)
* \param[out]  pnValue  pointer to ::UINT32_TYPE, which returns the desired information number
* \return      <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*              <dt>::UNI_RESULT_API_NOT_INITIALIZED</dt><dd>if this module was not initialized before this call</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetModuleInfo ( UINT32_IN_TYPE nModule, UINT32_OUT_TYPE pnValue );

/**
* get the UniAPI global FireGrab license information.
* \param[out] pnUniLicenseType  active license type \see ::E_UNI_LICENSE_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetLicenseType_Global( UINT32_OUT_TYPE pnUniLicenseType);

/**
* get active FireGrab license information for the camera.
* \param[in]  CamId                id of the camera to query the information for
* \param[out] pnUniLicenseType     the license type for the camera \see ::E_UNI_LICENSE_TYPE
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetLicenseType( ID_TYPE CamId, UINT32_OUT_TYPE pnUniLicenseType);

/**
 * Get error information.
 * The error code you supply is evaluated and the given string is
 *  filled with an error description.
 *
 * \param[in]   nErrCode        error code to be described
 * \param[out]  pszInfoString   the error code description
 * \param[in]   nMaxStrLen      length of the provided string buffer
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 * \note The following code shows an example of a UniAPI function call with subsequent
 * error code evaluation:
 * \code
 * UNI_RETURN_TYPE ret = UCC_Init();
 * if FAILED( ret )
 * {
 *      char text[256];
 *      UCC_GetErrorInfo ( ret, text, 256 );
 *      printf( "UCC_Init() returned error code 0x%X (%s)", (unsigned long)ret, text );
 * }
 * \endcode
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_GetErrorInfo ( UNI_RETURN_TYPE     nErrCode,
                                                ACHAR_INOUT_TYPE    pszInfoString,
                                                UINT32_IN_TYPE      nMaxStrLen );


/**
* Count connected cameras and return an array of camera IDs, if desired.
* The given array is filled up to the number of entries specified by *pnSize except if NULL is given as array pointer.
*
* \param[in,out] pnSize         <ul><li>in: array size</li><li>out: actual number of cameras filled into the array</li></ul>
* \param[out]    vecIds         pointer to an Id-array of nSize elements
*               
* \return <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_1</dt><dd>if pnSize is NULL or *pnSize==0 or *pnSize&gt;64</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if not the full range inside the array vecIds was writable</dd>
*         <dt>::UNI_RESULT_MORE_DATA</dt><dd>if there was more data than space provided by the given array</dd>
*         <dt>a FireGrab error</dt><dd></dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetCameras ( UINT32_INOUT_TYPE     pnSize,
                                              UINT32_OUT_TYPE       vecIds);

/**
* Count connected cameras and return arrays of camera IDs and extra information, if desired.
* The given arrays are filled up to the number of entries specified by *pnSize except if NULL is given as array pointer.
*
* \param[in,out]    pnSize          <ul><li>in: array size</li><li>out: actual number of cameras filled into the array</li></ul>
* \param[out]       vecIds          Pointer to an Id-array of nSize elements.
* \param[out]       vecExtraInfo    1:1 mapping of additional info for identifying of the vecIds 
* \note   vecExtraInfo gives bus specific information to a given camera [CardID:8][NodeId:8][reserved:16]
* \return <dl><dt>S_OK</dt>                         <dd>if the operation was successful,</dd> 
*         <dt>::UNI_RESULT_PARAMETER_INVALID_1</dt> <dd>if pnSize is NULL or *pnSize==0 or *pnSize>64</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt> <dd>if not the full range inside the array vecIds was writable</dd>
*         <dt>::UNI_RESULT_MORE_DATA</dt>           <dd>if there was more data than space provided by the given array</dd>
*         <dt>a FireGrab error</dt>                 <dd></dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetCamerasEx(UINT32_INOUT_TYPE    pnSize,
                                              UINT32_OUT_TYPE      vecIds,
                                              UINT32_OUT_TYPE      vecExtraInfo);

/**
 *  Wait for a bus reset.
 *  After, additional data is returned, if desired
 *
 * \param[out] vecSNs    array of camera IDs
 * \param[out] pCount    count
 * \param[in]  lTimeOut  TimeOut
 * \param[out] pRetErr   Error code of WaitForSingleObject
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 * deprecated, may be replaced by the following code:
 \code
    S_UNI_NOTIFICATION Notification;
    Notification.m_NotificationType          = E_UNI_NOTIFICATION_EVENT;
    Notification.m_NotificationEvent.m_Event = CreateEvent(NULL, false, false, "BusReset");
    NOTIFICATION_HANDLE hNotf = RegisterNodeListChangedNotification( &Notification); 

    DWORD dwRet = WaitForSingleObject( Notification.m_NotificationEvent.m_Event, lTimeOut );
    if( dwRet == WAIT_OBJECT_0 )
    {
        UCC_GetCameras ( pCount, vecSNs );
    }
    *pRetErr = dwRet;
    UnregisterNotification(hNotf);
\endcode
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_WaitForBusReset ( UINT32_OUT_TYPE       vecSNs,
                                                   UINT32_INOUT_TYPE     pCount, 
                                                   UINT32_IN_TYPE        lTimeOut, 
                                                   UINT32_OUT_TYPE       pRetErr );

/**
* Close all cameras.
*
* \return <dl><dt>::UNI_RESULT_API_NOT_INITIALIZED</dt><dd>::UCC_Init was not called</dd>
   <dt>::UNI_RESULT_CAMERA_NOT_FOUND</dt><dd>meaning that the camera with the given Id
   was not found on the bus, either bacause it has not been opened or because
   it is not accessible</dd></dl>

*/ 
UNI_RETURN_TYPE WIN32DLL_API UCC_CloseAllCameras ( void );

/**
* Disconnect all cameras.
*
* \return<dl><dt>S_OK</dt><dd>if the operation was successful</dd>
         <dt>::UNI_RESULT_API_NOT_INITIALIZED</dt><dd>::UCC_Init was not called</dd>
   </dl>

*/
UNI_RETURN_TYPE WIN32DLL_API UCC_DisconnectAllCameras ( void );

/**
* Reconnect all cameras.
*
* \param[in]   bAutoReLoadSettings (a "boolean" ::UINT32_TYPE value)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
                <dt>::UNI_RESULT_API_NOT_INITIALIZED</dt><dd>if ::UCC_Init was not called</dd></dl>

*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ReconnectAllCameras ( UINT32_IN_TYPE bAutoReLoadSettings );

/**
* Create a user log file.
*
* \param[in]   nMaxLevel      The maximum logging level to log see: ::E_UNI_LOGGING
* \param[in]   pszFileName    The name of the file to create
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
            <dt>::UNI_RESULT_API_NOT_INITIALIZED</dt><dd>::UCC_Init was not called</dd></dl>
   For general information about logging see \ref otherLog 
*/ 
UNI_RETURN_TYPE WIN32DLL_API UCC_CreateUserLog ( UINT32_IN_TYPE     nMaxLevel,
                                                 const ACHAR_TYPE   *pszFileName );

/**
 * Log a message.
 *
 * \param[in]   nLevel   logging level
 * \param[in]   pszMsg   text message to be logged
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 * For general information about logging see \ref otherLog 
 */ 
UNI_RETURN_TYPE WIN32DLL_API UCC_WriteUserLog ( UINT32_IN_TYPE      nLevel, 
                                                const ACHAR_TYPE    *pszMsg );

/**
* Get the number of pixels required for the transport of an image with the given color coding.
*
* \param[in]    ColorCode     The color coding see: ::E_UNI_COLOR_CODE
* \param[out]   BitsPerPixel  The number of bits (8,12,16,24 or 48)
* \return <dl><dt>S_OK</dt><dd>if a known color coding was submitted</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetBitsPerPixel ( UINT32_IN_TYPE  ColorCode,
                                                   UINT32_OUT_TYPE BitsPerPixel);

/*@}*/
////////////////////////////////////////////////////////////////////////////////////////////

/** @name Notification methods. 
A set of methods to deal with user notifications.
*/
/*@{*/

/** 
Register a notification object for 'node list changed' events (bus reset end).
@param[out]  pHandle         returns a handle to the notification object registration 
@param[in]   pNotification   notification object to be registered for 'bus reset end' events

@return      <dl><dt>S_OK</dt><dd>if successful</dd></dl>
@note 
The notification object pointed to by \c pNotification must not be deleted as long as its registration persists.
Therefore notification object registrations should be canceled immediately when they aren't needed anymore (see ::UCC_UnRegisterNotification()).
Notification objects may be registered for a certain camera or for all available cameras(\ref UNI_ALL_CAMERAS).
\note For examples see \ref codenotif "code snippets"
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_RegisterNodeListChangedNotification    (UNI_NOTIFICATION_HANDLE *pHandle,  S_UNI_NOTIFICATION *pNotification);

/**
Register a notification object for 'frame start' events.
\param[out]  pHandle         returns a handle to the notification object registration 
\param[in]   pNotification   notification object to be registered for 'frame start' events
\return      <dl><dt>S_OK</dt><dd>if successful</dd></dl>
@note 
The notification object pointed to by \c pNotification must not be deleted as long as its registration persists.
Therefore notification object registrations should be canceled immediately when they aren't needed anymore (see ::UCC_UnRegisterNotification()).
Notification objects may be registered for a certain camera or for all available cameras(\ref UNI_ALL_CAMERAS).

This method is for FireWire only, GigE camera will not send frame start events.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_RegisterFrameStartNotification     (UNI_NOTIFICATION_HANDLE *pHandle, S_UNI_NOTIFICATION *pNotification);

/** 
Register a notification object for 'frame ready' events.
\param[out]  pHandle         returns a handle to the notification object registration 
\param[in]   pNotification   notification object to be registered for 'frame ready' events
\return      <dl><dt>S_OK</dt><dd>if successful</dd></dl>
@note 
The notification object pointed to by \c pNotification must not be deleted as long as its registration persists.
Therefore notification object registrations should be canceled immediately when they aren't needed anymore (see ::UCC_UnRegisterNotification()).
Notification objects may be registered for a certain camera or for all available cameras(\ref UNI_ALL_CAMERAS).
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_RegisterFrameReadyNotification       (UNI_NOTIFICATION_HANDLE *pHandle, S_UNI_NOTIFICATION *pNotification);

/** 
Register a notification object for 'frame dropped' events.
\param[out]  pHandle         returns a handle to the notification object registration 
\param[in]   pNotification   notification object to be registered for 'frame dropped' events
\return      <dl><dt>S_OK</dt><dd>if successful</dd></dl>
@note 
The notification object pointed to by \c pNotification must not be deleted as long as its registration persists.
Therefore notification object registrations should be canceled immediately when they aren't needed anymore (see ::UCC_UnRegisterNotification ).
Notification objects may be registered for a certain camera or for all available cameras(\ref UNI_ALL_CAMERAS).
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_RegisterFrameDroppedNotification   (UNI_NOTIFICATION_HANDLE *pHandle, S_UNI_NOTIFICATION *pNotification);

/**
Register a notification object for a specified event type.
\param[out]  pHandle             returns a handle to the notification object registration 
\param[in]   NotificationEvent   the type of event the notification object should be registered for
\param[in]   pNotification       notification object to be registered
\return      <dl><dt>S_OK</dt><dd>if successful</dd></dl>
@note 
The notification object pointed to by \c pNotification must not be deleted as long as its registration persists.
Therefore notification object registrations should be canceled immediately when they aren't needed anymore (see ::UCC_UnRegisterNotification()).
Notification objects may be registered for a certain camera or for all available cameras(\ref UNI_ALL_CAMERAS).

Although this method can be used to register frame start events (see ::E_UNI_NOTIFICATION_FRAME_START) for GigE cameras, 
it does not work as expected, because GigE camera will not send frame start events.

\note For examples see \ref codenotif "code snippets"
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_RegisterNotification               (UNI_NOTIFICATION_HANDLE *pHandle, E_UNI_NOTIFICATION_EVENTS NotificationEvent,S_UNI_NOTIFICATION *pNotification);

/** 
Unregister a single notification object registration.
\param[out]  Handle         handle to registration to be canceled
\return      <dl><dt>S_OK</dt><dd>if successful</dd></dl>
\note For examples see \ref codenotif "code snippets"
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_UnRegisterNotification             (UNI_NOTIFICATION_HANDLE Handle);


/*@}*/
////////////////////////////////////////////////////////////////////////////////////////////

/** @name Camera methods. 
A set of methods to deal with all actions
related to cameras in general.
*/
/*@{*/
/**
* Open a Camera.
* Updates the internal camera object to a physical camera object.
* \note causes a NodeListChange event.
* \note if ::E_FEAT_OPENREADONLY is activated when opening a camera,
*   registers can be only read, and writing registers will return an error.
*
* \param[in]    CamId       camera identifier (serial number)
* \param[in]    lSpeedLimit maximum communication bit rate to be used ::E_UNI_DATABITRATE
*
* \return <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_OpenCamera ( ID_TYPE        CamId,
                                              INT32_IN_TYPE lSpeedLimit=-1);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_OpenCamera ( ID_TYPE        CamId,
                                              INT32_IN_TYPE  lSpeedLimit);
#endif

/**
* Open a camera with extended functionality.
* Updates the internal camera object to a physical camera object.
* \note causes a NodeListChange event.
* \note if ::E_FEAT_OPENREADONLY is activated when opening a camera,
*   registers can be only read, and writing registers will return an error.
*
* \param[in]    CamId       camera identifier (serial number)
* \param[in]    CardNr      if Camera on more then one card choose this card
* \param[in]    lSpeedLimit maximum communication bit rate to be used ::E_UNI_DATABITRATE
*
* \return <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_OpenCameraEx(ID_TYPE           CamId,
                                              INT32_TYPE        CardNr,
                                              INT32_IN_TYPE    lSpeedLimit=-1);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_OpenCameraEx(ID_TYPE           CamId,
                                              INT32_TYPE        CardNr,
                                              INT32_IN_TYPE     lSpeedLimit);
#endif

/**
* Close a specific camera.
*
* \param[in]    CamId   camera identifier (serial number)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_CloseCamera ( ID_TYPE CamId);

/*@}*/
////////////////////////////////////////////////////////////////////////////////////////////

/** @name Camera information methods. 
A set of methods to get information about the camera.
*/
/*@{*/

/**
* Check if a camera is currently available.
*
* \param[in]    CamId       camera identifier (serial number)
* \param[out]   pbResult    pointer to a "boolean" ::UINT32_TYPE value for receiving the availability state
* \return   <dl><dt>S_OK</dt><dd>if the node is available</dd></dl>
* \note a camera is available if it is opened by the API or not in busy state
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_CheckAvailable ( ID_TYPE            CamId,
                                                  UINT32_OUT_TYPE    pbResult);

/**
* Get numerical information about the specified camera.
*
* \param[in]        CamId           camera identifier (serial number)
* \param[in]        nId             identifier to the camera info you'd like to receive (see ::E_UNI_CAMERA_INFO)
* \param[out]       pnValue         pointer to the parameter info value
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetCameraInfo ( ID_TYPE           CamId,
                                                 UINT32_IN_TYPE    nId,
                                                 UINT32_INOUT_TYPE pnValue);

/**
* Get textual information about the specified camera.
*
* \param[in]        CamId           camera identifier (serial number)
* \param[in]        nId             identifier to the camera info you'd like to receive (see ::E_UNI_CAMERA_INFO)
* \param[out]       pszInfoString   pointer to a zero terminated String of length (*pLength) max.
* \param[in,out]    pLength         <ul><li>[in]pointer to an integer holding the available space in pszInfoString</li>
                                    <li>[out]pointer to the length of the string that was filled</li></ul>
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetCameraInfoString ( ID_TYPE           CamId,
                                                       UINT32_IN_TYPE    nId,
                                                       ACHAR_INOUT_TYPE  pszInfoString,
                                                       UINT32_INOUT_TYPE pLength);

/**
* Tell fundamental camera capabilities.
*
* \param[in]    CamId   Camera identifier (serial number)
* \param[out]   pnMaxImageWidth     Pointer to an ::UINT32_TYPE variable receiving maximal possible image width
* \param[out]   pnMaxImageHeight    Pointer to an ::UINT32_TYPE variable receiving maximal possible image height
* \param[out]   pbColorInfo         Pointer to an ::UINT32_TYPE boolean indicating colour camera
* \param[out]   pnMaxImageDepth     Pointer to an ::UINT32_TYPE variable receiving the maximal possible bit depth
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetCameraCapabilities ( ID_TYPE         CamId,
                                                         UINT32_OUT_TYPE pnMaxImageWidth,
                                                         UINT32_OUT_TYPE pnMaxImageHeight,
                                                         UINT32_OUT_TYPE pbColorInfo,
                                                         UINT32_OUT_TYPE pnMaxImageDepth );

/**
* Get current IP configuration of the specified camera.
*
* \note This method is for GigE only.
*
* \param[in]        CamId                   camera identifier (serial number)
* \param[out]       pnConfigMode            current IP configuration mode (see ::E_UNI_IP_CONFIGURATION_MODE)
* \param[out]       pnSupportedConfigMode   supported IP configuration modes (see ::E_UNI_IP_CONFIGURATION_MODE)
* \param[out]       pnIpAddress             currently configured IP address
* \param[out]       pnSubnetMask            currently configured subnet mask
* \param[out]       pnDefaultGateway        currently configured default gateway
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*              <dt>::UNI_RESULT_CAMERA_BUSY</dt><dd>if the camera has been opened by an application on the local or any remote host</dd></dl>
* \note <ul>
          <li>The parameter nConfigMode tells which method was used to set the IP address information. If the camera was configured to use DHCP 
          and fell back to Auto IP this parameter will still be set to DHCP.</li>
          <li>The parameter nSupportedConfigMode is in general a combination of several individual modes (see ::E_UNI_IP_CONFIGURATION_MODE) 
          and tells which methods to set the IP address information are supported by the given camera.</li>
          <li>The parameter nIpAddress, nSubnetMask, and nDefaultGateway tell the current IP configuration of the camera. 
          These values are given in network byte order (i.e. big endian) and were set either by DHCP, Auto IP, or from the 
          persistent information stored inside the camera.</li>
          <li>Each out parameter may be NULL if the corresponding value is of no interest.</li>
        </ul>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_IpConfiguration_Get ( ID_TYPE           CamId,
                                                       UINT32_OUT_TYPE   pnConfigMode,
                                                       UINT32_OUT_TYPE   pnSupportedConfigMode,
                                                       UINT32_OUT_TYPE   pnIpAddress,
                                                       UINT32_OUT_TYPE   pnSubnetMask,
                                                       UINT32_OUT_TYPE   pnDefaultGateway);

/**
* Set current IP configuration of the specified camera.
*
* \note This method is for GigE only.
*
* \param[in]        CamId                   camera identifier (serial number)
* \param[in]        nConfigMode             IP configuration mode to set (see ::E_UNI_IP_CONFIGURATION_MODE)
* \param[in]        nIpAddress              IP address to set (if nConfigMode is set to persistent IP only)
* \param[in]        nSubnetMask             subnet mask to set (if nConfigMode is set to persistent IP only)
* \param[in]        nDefaultGateway         default gateway to set (if nConfigMode is set to persistent IP only)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*              <dt>::UNI_RESULT_CAMERA_BUSY</dt><dd>if the camera has been opened by an application on the local or any remote host</dd></dl>
* \note    The values for the parameter nIpAddress, nSubnetMask, and nDefaultGateway should be given in network byte order (i.e. big endian).
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_IpConfiguration_Set ( ID_TYPE           CamId,
                                                       UINT32_IN_TYPE   nConfigMode,
                                                       UINT32_IN_TYPE   nIpAddress,
                                                       UINT32_IN_TYPE   nSubnetMask,
                                                       UINT32_IN_TYPE   nDefaultGateway);

/**
* Get current image format properties.
*
* \param[in]    CamId            Camera identifier (serial number)
* \param[out]   pnImageWidth     Pointer to ::UINT32_TYPE receiving the current image width
* \param[out]   pnImageHeight    Pointer to ::UINT32_TYPE receiving the current image height
* \param[out]   pnColorCoding    Pointer to ::UINT32_TYPE receiving the current color coding id see: ::E_UNI_COLOR_CODE
* \param[out]   pnImageDepth     Pointer to ::UINT32_TYPE receiving the current image depth
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetCurrentImageFormat ( ID_TYPE         CamId,
                                                         UINT32_OUT_TYPE pnImageWidth,
                                                         UINT32_OUT_TYPE pnImageHeight,
                                                         UINT32_OUT_TYPE pnColorCoding,
                                                         UINT32_OUT_TYPE pnImageDepth );

/**
* Get current IIDC format properties.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[out]   pFormat     Pointer to ::UINT32_TYPE receiving the current IIDC format
* \param[out]   pMode       Pointer to ::UINT32_TYPE receiving the current IIDC mode
* \param[out]   pFpsColId   Pointer to ::UINT32_TYPE receiving the current FPS id or color coding id see: :::E_UNI_FRAME_RATE
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetCurrentFixedFormat ( ID_TYPE         CamId,
                                                         UINT32_OUT_TYPE  pFormat,
                                                         UINT32_OUT_TYPE  pMode,
                                                         UINT32_OUT_TYPE  pFpsColId );

/** Retrieve information about the free format (IIDC:format 7) parameters of a given mode.
* \param[in]    CamId       Camera identifier (serial number)
* \param[in]    nMode       Mode returned from ::UCC_EnumFreeModes()
* \param[in]    nParamId    ::E_UNI_FREE_MODE_INFO identifier of the parameter 
* \param[out]   pnValue     returns the requested value
*\return    <dl><dt>S_OK</dt><dd>if the operation was successful)</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_GetFreeModeInfo(   ID_TYPE             CamId
                                                ,   UINT32_TYPE         nMode
                                                ,   UINT32_TYPE         nParamId
                                                ,   UINT32_OUT_TYPE     pnValue);

/**
* Enumerate through available free/scalable image formats.
* Start the complete enumeration by setting the index of the format to 0. On return,
* this value is incremented. If nothing is found, -1 is returned in *pnPos
*
* \param[in]     CamId       Camera identifier (serial number)
* \param[in,out] pnPos       Pointer to ::UINT32_TYPE holding the index of the format. Is updated after the call.
* \param[out]    pnMode      Pointer to ::UINT32_TYPE receiving the mode
* \param[out]    pnColorId   Pointer to ::UINT32_TYPE receiving the color coding id, see: ::E_UNI_COLOR_CODE
* \param[out]    pnXMax      Pointer to ::UINT32_TYPE receiving the maximum width in this mode
* \param[out]    pnYMax      Pointer to ::UINT32_TYPE receiving the maximum width in this mode
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_EnumFreeModes( ID_TYPE CamId,
                                                UINT32_INOUT_TYPE pnPos,
                                                UINT32_OUT_TYPE pnMode,
                                                UINT32_OUT_TYPE pnColorId,
                                                UINT32_OUT_TYPE pnXMax,
                                                UINT32_OUT_TYPE pnYMax  );

/*@}*/
////////////////////////////////////////////////////////////////////////////////////////////

/** @name Image acquisition. 
A set of methods to deal with the acquisition of images.
*/
/*@{*/


/**
* Prepare camera for format-free image acquisition.
* May be used for setting grabbing parameters for preparing your
*  image transport in just one call.
*
* \param[in]     CamId           Camera identifier (serial number)
* \param[in,out] pMode           As a rule, use the format 7 mode
* \param[in,out] pColorFormatId  Pointer to the color id (see ::E_UNI_COLOR_CODE)
* \param[in,out] pWidth          Pointer to the height of the image
* \param[in,out] pHeight         Pointer to the width of the image
* \param[in,out] pBufferCount    Pointer to the number of buffer (if NULL, the pre-defined value is taken)
* \param[in,out] pXOffset        Pointer to the horizontal position of the image
                            (if NULL, 0 is assumed)
* \param[in,out] pYOffset        Pointer to the vertical position of the image
                            (if NULL, 0 is assumed)
* \param[in,out] pBusLoad        Pointer to the desired bus load (in 1/1000) (return: the real bus load)
                            (if NULL, 1000 is assumed)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
* \note     This method automatically sets the packet size via the \c pBusload parameter, which is representing
            the payload in per mille.<ul><li>
            For FireWire, the following calculation is used:
            \code PacketSize = ROUND(Busload) x (1024 x 2^PhySpeed)/4000) x 4 \endcode
            Where PhySpeed is
            -   0 for S100,
            -   1 for S200,
            -   2 for S400,
            -   3 for S800.</li><li>
            For GigE, the full bandwidth is assumed.</li></ul>
            For examples see \ref codeformat.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_PrepareFreeGrab (  ID_TYPE             CamId,
                                                    UINT32_INOUT_TYPE   pMode,
                                                    UINT32_INOUT_TYPE   pColorFormatId,
                                                    UINT32_INOUT_TYPE   pWidth,
                                                    UINT32_INOUT_TYPE   pHeight,
                                                    UINT32_INOUT_TYPE   pBufferCount,
                                                    UINT32_INOUT_TYPE   pXOffset,
                                                    UINT32_INOUT_TYPE   pYOffset, 
                                                    UINT32_INOUT_TYPE   pBusLoad );

/**Query the supported IIDC video modes of a camera.
* Used to identify the IIDC video modes implemented in the camera.
* Setting one of the modes can fail due to the current set bus speed.
* \param[in]  CamId       Camera identifier (serial number)
* \param[out] pFormats    pointer to a ::S_IIDC_VIDEOINFO_FORMAT struct
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetSupportedFixedFormats ( ID_TYPE                CamId,
                                                           S_IIDC_VIDEOINFO_FORMAT  *pFormats);

/**
* Prepare camera for image acquisition.
* May be used for setting grabbing parameters for preparing your
* image transport in just one call. This function is primarily intended for the configuration
* of the fixed IIDC formats 0-2. It may be also used to configure format 7 modes with certain restrictions.
* Format 7 modes require additional configuration parameters, none of which is adjusted by this function.
* If these have already been set ( e.g. by ::UCC_PrepareFreeGrab calls), ::UCC_PrepareFixedGrab may be used
* to switch between different modes quickly.<br>
* \note the \c nFpsId Parameter is ignored for Format 7 Modes.
*
* \param[in]    CamId           Camera identifier (serial number)
* \param[in]    nFormat         The IIDC format id (0<=nFormat<=7)
* \param[in]    nMode           The IIDC mode id (0<=nMode<=7)
* \param[in]    nFpsId          The IIDC framerate id  (see ::E_UNI_FRAME_RATE for valid values)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*               <dt>::UNI_RESULT_INVALID_SETTINGS</dt><dd>if the parameters are not supported by the camera</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_PrepareFixedGrab ( ID_TYPE         CamId,
                                                    UINT32_IN_TYPE  nFormat,
                                                    UINT32_IN_TYPE  nMode,
                                                    UINT32_IN_TYPE  nFpsId );

/**
* Get the AOI for the current free IIDC mode (Format 7).
* Every AOI parameter may be omitted.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[out]   pLeft       Pointer to ::UINT32_TYPE receiving the left position of the current AOI.
* \param[out]   pTop        Pointer to ::UINT32_TYPE receiving the top position of the current AOI.
* \param[out]   pWidth      Pointer to ::UINT32_TYPE receiving the width of the current AOI.
* \param[out]   pHeight     Pointer to ::UINT32_TYPE receiving the height of the current AOI.
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetAOI ( ID_TYPE          CamId,
                                          UINT32_OUT_TYPE  pLeft,
                                          UINT32_OUT_TYPE  pTop,
                                          UINT32_OUT_TYPE  pWidth,
                                          UINT32_OUT_TYPE  pHeight);

/**
* Set AOI position only - image size will not be changed. This function is most useful when called
* during image acquisition. For general AOI configuration see ::UCC_PrepareFreeGrab.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[in,out]   pLeft    Pointer to new horizontal position - invalid data will be adjusted during function call. NULL will leave position unchanged.
* \param[in,out]   pTop    Pointer to new vertical position - invalid data will be adjusted during function call. NULL will leave position unchanged.
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_SetAOIPosition ( ID_TYPE            CamId,
                                                  UINT32_INOUT_TYPE  pLeft,
                                                  UINT32_INOUT_TYPE  pTop);


/**Set AOI for the current scalable mode to the camera.
* \param[in,out]    CamId       Camera identifier
* \param[in,out]    pLeft       left corner of the AOI
* \param[in,out]    pTop        top corner of the AOI
* \param[in,out]    pWidth      width of the AOI
* \param[in,out]    pHeight     height of the AOI
* \returns
* \note This function only succeeds for free modes(scalebel modes).
* Setting the AOI to a grabbing camera will try to fast resize the DMA, this will only succeed if the new AOI will use less data for transport then the one set with UCC_PrepareFreeGrab.
* IMPORTAINT: due to unreliability of fast resize, this functionality will be deactivated and in case of running camera an error will be returned.
* Setting the AOIwhile the camera is not grabbing will set the AOI for the current scaleable mode.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_SetAOI         ( ID_TYPE            CamId,
                                                  UINT32_INOUT_TYPE  pLeft,
                                                  UINT32_INOUT_TYPE  pTop,
                                                  UINT32_INOUT_TYPE  pWidth,
                                                  UINT32_INOUT_TYPE  pHeight);

/** Get the free bandwidth.
*   The free bandwidth for the bus the camera is connected to is returned.
*   \param[in]  CamId        Camera identifier (serial number)
*   \param[out] pnBandwidth  Pointer to ::UINT32_TYPE receiving the free bandwith for the bus the camera is connected to
*   \return <dl><dt>S_OK</dt><dd>if all parameter are valid</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetFreeBandwidth( ID_TYPE          CamId, 
                                                   UINT32_OUT_TYPE  pnBandwidth);


/**
* Synchronously grab a single image.
* The image format has to be specified by the \ref S_UNI_IMAGE::uImageformat "uImageformat"
* member of the provided ::S_UNI_IMAGE struct. The corresponding data pointers of psImage
* have to point to sufficiently sized memory buffers.
* This function is provided for backwards compatibility. When using GigE-Vision 
* cameras and ::E_IF_NATIVE_EX output format, necessary format details are not returned.
* Instead, ::UCC_GrabImageEx should be used.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[out]   psImage     Pointer to image struct
* \param[out]   pbIsColor   returns "boolean" ::UINT32_TYPE value indicating if output data contains multiple values per pixel
* \param[in]    nTimeOut    Timeout in ms
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabImage ( ID_TYPE                    CamId, 
                                             S_UNI_IMAGE *              psImage,
                                             UINT32_OUT_TYPE            pbIsColor,
                                             UINT32_IN_TYPE             nTimeOut );


/**
* Synchronously grab a single image.
* The image format has to be specified by the \ref S_UNI_IMAGE::uImageformat "uImageformat"
* member of the provided ::S_UNI_IMAGE struct. The corresponding data pointers of psImage
* have to point to sufficiently sized memory buffers.
*
* \param[in]    CamId         Camera identifier (serial number)
* \param[in]    psImage       Pointer to image struct (see ::S_UNI_IMAGE)
* \param[out]   psFormatInfo  Will be filled with image format details, required for ::E_IF_NATIVE_EX output format (see ::S_UNI_TRANSPORT_FORMAT_INFO)
* \param[out]   pbIsColor     Returns "boolean" ::UINT32_TYPE value indicating if output data contains multiple values per pixel
* \param[in]    nTimeOut      Timeout in ms
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
* \note Alternatively one the following functions may be used to acquire
* image data in a certain format:
* <ul><li>::UCC_GrabBitmap8Image() - returns grayscale image data in bitmap format</li>
* <li>::UCC_GrabBitmap24Image() - returns color image data in bitmap format</li>
* <li>::UCC_GrabPlanesImage() - returns image data in planar format</li>
* <li>::UCC_GrabNativeImageEx() - returns image data in native format, as transfered 
* by the camera</li></ul>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabImageEx ( ID_TYPE                       CamId, 
                                               S_UNI_IMAGE *                 psImage,
                                               S_UNI_TRANSPORT_FORMAT_INFO * psFormatInfo,
                                               UINT32_OUT_TYPE               pbIsColor,
                                               UINT32_IN_TYPE                nTimeOut );


/**
* Grab a single image synchronously in Windows bitmap format.
* Uses a byte pointer for the target image.
* This function is provided for backwards compatibility. Raw formats are not debayered, but instead
* treated as mono formats. Output data is in BGR24 or Mono8 format, depending on the camera's color code.
* It is not recommended to use this function for new code. Instead, ::UCC_GrabBitmap8Image and 
* ::UCC_GrabBitmap24Image should be used.
*
* \param[in]   CamId       Camera identifier (serial number)
* \param[out]  pImage      The pointer to the image
* \param[out]  pbIsColor   Returns "boolean" ::UINT32_TYPE value indicating if output data contains multiple values per pixel
* \param[in]   nTimeOut    Timeout in ms
* \param[out]  pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabBitmapImage ( ID_TYPE          CamId,
                                                   UINT8_OUT_TYPE   pImage,
                                                   UINT32_OUT_TYPE  pbIsColor,
                                                   UINT32_IN_TYPE   nTimeOut,
                                                   S_SIS_DATA       *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabBitmapImage ( ID_TYPE          CamId,
                                                   UINT8_OUT_TYPE   pImage,
                                                   UINT32_OUT_TYPE  pbIsColor,
                                                   UINT32_IN_TYPE   nTimeOut,
                                                   S_SIS_DATA       *pSISData );
#endif

/**
* Grab a single image synchronously in 8 BPP grayscale format.
* Uses a byte pointer for the target image. Sufficient memory space has to be provided by the caller.
* Image data is always returned in 8 Bit Mono format. Where applicable, debayering and/or 
* color-to-grayscale conversions are performed.
*
* \param[in]   CamId       Camera identifier (serial number)
* \param[out]  pImage      The pointer to the image
* \param[in]   nTimeOut    Timeout in ms
* \param[out]  pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabBitmap8Image ( ID_TYPE          CamId,
                                                    UINT8_OUT_TYPE   pImage,
                                                    UINT32_IN_TYPE   nTimeOut,
                                                    S_SIS_DATA       *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabBitmap8Image ( ID_TYPE          CamId,
                                                    UINT8_OUT_TYPE   pImage,
                                                    UINT32_IN_TYPE   nTimeOut,
                                                    S_SIS_DATA       *pSISData );
#endif

/**
* Grab a single image synchronously in 24 BPP color bitmap format.
* Uses a byte pointer for the target image. Sufficient memory space has to be provided by the caller.
* Image data is always returned in 24 Bit BGR format. Where applicable, debayering is performed.
*
* \param[in]   CamId       Camera identifier (serial number)
* \param[out]  pImage      The pointer to the image
* \param[in]   nTimeOut    Timeout in ms
* \param[out]  pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabBitmap24Image ( ID_TYPE          CamId,
                                                     UINT8_OUT_TYPE   pImage,
                                                     UINT32_IN_TYPE   nTimeOut,
                                                     S_SIS_DATA       *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabBitmap24Image ( ID_TYPE          CamId,
                                                     UINT8_OUT_TYPE   pImage,
                                                     UINT32_IN_TYPE   nTimeOut,
                                                     S_SIS_DATA       *pSISData );
#endif

/**
* Grab a single image synchronously into a planes image.
* Uses byte pointers for all target planes.
*  
* \note Raw images are not debayered yet. Use debayering methods afterwards.
* \since 0.1.2
*
* \param[in]   CamId       Camera identifier (serial number)
* \param[out]  pR          The pointer to the red plane
* \param[out]  pG          The pointer to the green plane 
* \param[out]  pB          The pointer to the blue plane
* \param[out]  pY          The pointer to the intensity plane ( may be set to NULL )
* \param[out]  pbIsColor   Returns "boolean" ::UINT32_TYPE value indicating if output data contains multiple values per pixel
* \param[in]   nTimeOut    Timeout in ms
* \param[out]  pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabPlanesImage ( ID_TYPE          CamId,
                                                   UINT8_OUT_TYPE   pR,
                                                   UINT8_OUT_TYPE   pG,
                                                   UINT8_OUT_TYPE   pB,
                                                   UINT8_OUT_TYPE   pY,
                                                   UINT32_OUT_TYPE  pbIsColor,
                                                   UINT32_IN_TYPE   nTimeOut,
                                                   S_SIS_DATA       *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabPlanesImage ( ID_TYPE          CamId,
                                                   UINT8_OUT_TYPE   pR,
                                                   UINT8_OUT_TYPE   pG,
                                                   UINT8_OUT_TYPE   pB,
                                                   UINT8_OUT_TYPE   pY,
                                                   UINT32_OUT_TYPE  pbIsColor,
                                                   UINT32_IN_TYPE   nTimeOut,
                                                   S_SIS_DATA       *pSISData );
#endif

/**
* Grab a single image synchronously in transport format as supported by 1394 DCAM cameras. 
* Image format conversions may be required to retrieve displayable data.
* This function is provided for backwards compatibility. GigE-Vision image frames are converted to match 
* DCAM image formats, when necessary (introducing processing overhead). Use ::UCC_GetNativeImageEx to obtain 
* data in actual, interface-dependent transport format.
* A byte pointer for the target image is used. Sufficient memory space has to be provided by the caller.
*
* \param[in]   CamId       Camera identifier (serial number)
* \param[out]  pImage      The pointer to the image
* \param[out]  pbIsColor   returns "boolean" ::UINT32_TYPE value indicating if output data contains multiple values per pixel
* \param[in]   nTimeOut    Timeout in ms
* \param[out]  pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabNativeImage ( ID_TYPE          CamId,
                                                   UINT8_OUT_TYPE   pImage,
                                                   UINT32_OUT_TYPE  pbIsColor,
                                                   UINT32_IN_TYPE   nTimeOut,
                                                   S_SIS_DATA      *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabNativeImage ( ID_TYPE          CamId,
                                                   UINT8_OUT_TYPE   pImage,
                                                   UINT32_OUT_TYPE  pbIsColor,
                                                   UINT32_IN_TYPE   nTimeOut,
                                                   S_SIS_DATA      *pSISData );
#endif

/**
* Grab a single image synchronously in native transport format.
* Image format conversions may be required to retrieve displayable data.
* For certain color codes (mostly 'deep' image formats), the output format
* differs between 1394 DCAM and GigE-Vision cameras. Relevant format details are returned 
* in psFormatInfo.
* A byte pointer for the target image is used.
* Sufficient memory space has to be provided by the caller.
*
*
* \param[in]   CamId       Camera identifier (serial number)
* \param[out]  pImage      The pointer to the image
* \param[out]   psFormatInfo  Will be filled with image format details, required for ::E_IF_NATIVE_EX output format. \see ::S_UNI_TRANSPORT_FORMAT_INFO
* \param[out]  pbIsColor   returns "boolean" ::UINT32_TYPE value indicating if output data contains multiple values per pixel
* \param[in]   nTimeOut    Timeout in ms
* \param[out]  pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabNativeImageEx ( ID_TYPE                       CamId,
                                                     UINT8_OUT_TYPE                pImage,
                                                     S_UNI_TRANSPORT_FORMAT_INFO * psFormatInfo,
                                                     UINT32_OUT_TYPE               pbIsColor,
                                                     UINT32_IN_TYPE                nTimeOut,
                                                     S_SIS_DATA                   *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabNativeImageEx ( ID_TYPE                       CamId,
                                                     UINT8_OUT_TYPE                pImage,
                                                     S_UNI_TRANSPORT_FORMAT_INFO * psFormatInfo,
                                                     UINT32_OUT_TYPE               pbIsColor,
                                                     UINT32_IN_TYPE                nTimeOut,
                                                     S_SIS_DATA                   *pSISData );
#endif

/**
* Start grabbing images asynchronously.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[in]    nTimeOut    timeout to wait for image to be ready
* \note dependent on ::SetFeatureState with ::E_FEAT_STARTIMMEDIATELY the ISO stream is started or delayed.
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabStart ( ID_TYPE        CamId,
                                             UINT32_IN_TYPE nTimeOut );

/**
* Start multi shot image grabbing asynchronously.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[in]    nGrabCount  Image count to grab (has to be greater then 0)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_MultiShotStart (   ID_TYPE         CamId,
                                                    UINT32_IN_TYPE  nGrabCount);

/** Stopping a running multi shot.
* \param[in]    CamId       Camera identifier (serial number)
* \return <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
* \note Get[xxx]Image might afterwards return UNI_RESULT_COMMAND_ABORTED
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_MultiShotStop  (   ID_TYPE        CamId);

/**
* Stops the grabbing of images.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[in]    nStopOption Option for stopping: 0 just stops grabbing (delivering all pending images), 1 cancels
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GrabStop ( ID_TYPE         CamId,
                                            UINT32_IN_TYPE  nStopOption );

/**
* Request an image from the async receive buffer.
* The image format has to be specified by the \ref S_UNI_IMAGE::uImageformat "uImageformat"
* member of the provided ::S_UNI_IMAGE struct. The corresponding data pointers of psImage
* have to point to sufficiently sized memory buffers.
* This function is provided for backwards compatibility. When using GigE-Vision 
* cameras and ::E_IF_NATIVE_EX output format, necessary format details are not returned.
* Instead, ::UCC_GetImageEx should be used.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[in]    psImage     Pointer to image struct \see ::S_UNI_IMAGE
* \param[in]    nTimeOut    timeout in ms
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
* \note Alternatively one the following functions may be used to acquire
* image data in a certain format:
* <ul><li>::UCC_GetBitmapImage() - returns image data in bitmap format</li>
* <li>::UCC_GetPlanesImage() - returns image data in planar format</li>
* <li>::UCC_GetNativeImageEx() - returns image data in native format, as transfered 
* by the camera</li></ul>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetImage ( ID_TYPE         CamId,
                                            S_UNI_IMAGE *   psImage,
                                            UINT32_IN_TYPE  nTimeOut );


/**
* Request an image from the async receive buffer.
* The image format has to be specified by the \ref S_UNI_IMAGE::uImageformat "uImageformat"
* member of the provided ::S_UNI_IMAGE struct. The corresponding data pointers of psImage
* have to point to sufficiently sized memory buffers.
*
* \param[in]    CamId         Camera identifier (serial number)
* \param[in]    psImage       Pointer to image struct \see ::S_UNI_IMAGE
* \param[out]   psFormatInfo  Will be filled with image format details, required for ::E_IF_NATIVE_EX output format. \see ::S_UNI_TRANSPORT_FORMAT_INFO
* \param[out]   pnFrameCount  returns frame count - may be NULL
* \param[in]    nTimeOut      timeout in ms
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
* \note Alternatively one the following functions may be used to acquire
* image data in a certain format:
* <ul><li>::UCC_GetBitmapImage() - returns image data in bitmap format</li>
* <li>::UCC_GetPlanesImage() - returns image data in planar format</li>
* <li>::UCC_GetNativeImageEx() - returns image data in native format, as transfered 
* by the camera</li></ul>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetImageEx ( ID_TYPE                       CamId,
                                              S_UNI_IMAGE *                 psImage,
                                              S_UNI_TRANSPORT_FORMAT_INFO * psFormatInfo,
                                              UINT32_OUT_TYPE               pnFrameCount,
                                              UINT32_IN_TYPE                nTimeOut );


/**
* Request a bitmap from the async receive buffer.
* Uses a byte pointer for the target image. Sufficient memory space has to be provided by the caller.
* This function is provided for backwards compatibility. Raw formats are not debayered, but instead
* treated as mono formats. Output data is in BGR24 or Mono8 format, depending on the camera's color code.
* It is not recommended to use this function for new code. Instead, ::UCC_GetBitmap8ImageEx and 
* ::UCC_GetBitmap24ImageEx should be used.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[out]   pImage      Pointer to image bitmap
* \param[in]    nTimeOut    timeout
* \param[out]   pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GetBitmapImage ( ID_TYPE           CamId,
                                                  UINT8_OUT_TYPE    pImage,
                                                  UINT32_IN_TYPE    nTimeOut,
                                                  S_SIS_DATA        *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GetBitmapImage ( ID_TYPE           CamId,
                                                  UINT8_OUT_TYPE    pImage,
                                                  UINT32_IN_TYPE    nTimeOut,
                                                  S_SIS_DATA        *pSISData );
#endif


/**
* Request a bitmap from the async receive buffer.
* Uses a byte pointer for the target image. Sufficient memory space has to be provided by the caller.
* Image data is always returned in 8 Bit Mono format. Where applicable, debayering and/or 
* color-to-greyscale conversions are performed.
*
* \param[in]    CamId         Camera identifier (serial number)
* \param[out]   pImage        Pointer to image bitmap
* \param[out]   pnFrameCount  returns frame count - may be NULL
* \param[in]    nTimeOut      timeout
* \param[out]   pSISData      Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GetBitmap8ImageEx ( ID_TYPE           CamId,
                                                     UINT8_OUT_TYPE    pImage,
                                                     UINT32_OUT_TYPE   pnFrameCount,
                                                     UINT32_IN_TYPE    nTimeOut,
                                                     S_SIS_DATA        *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GetBitmap8ImageEx ( ID_TYPE           CamId,
                                                     UINT8_OUT_TYPE    pImage,
                                                     UINT32_OUT_TYPE   pnFrameCount,
                                                     UINT32_IN_TYPE    nTimeOut,
                                                     S_SIS_DATA        *pSISData );
#endif


/**
* Request a bitmap from the async receive buffer.
* Uses a byte pointer for the target image. Sufficient memory space has to be provided by the caller.
* Image data is always returned in 24 Bit BGR format. Where applicable, debayering is performed.
*
* \param[in]    CamId         Camera identifier (serial number)
* \param[out]   pImage        Pointer to image bitmap
* \param[out]   pnFrameCount  returns frame count - may be NULL
* \param[in]    nTimeOut      timeout
* \param[out]   pSISData      Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GetBitmap24ImageEx ( ID_TYPE           CamId,
                                                      UINT8_OUT_TYPE    pImage,
                                                      UINT32_OUT_TYPE   pnFrameCount,
                                                      UINT32_IN_TYPE    nTimeOut,
                                                      S_SIS_DATA        *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GetBitmap24ImageEx ( ID_TYPE           CamId,
                                                      UINT8_OUT_TYPE    pImage,
                                                      UINT32_OUT_TYPE   pnFrameCount,
                                                      UINT32_IN_TYPE    nTimeOut,
                                                      S_SIS_DATA        *pSISData );
#endif

/**
 * Request a planes image from async receive buffer.
 *  Uses byte pointers for all the target planes. Sufficient memory space has to be provided by the caller.
 *
 * \param[in]   CamId       Camera identifier (serial number)
 * \param[out]  pR          The pointer to the red plane
 * \param[out]  pG          The pointer to the green plane 
 * \param[out]  pB          The pointer to the blue plane
 * \param[out]  pY          The pointer to the intensity plane ( may be set to NULL )
 * \param[in]   nTimeOut    timeout
 * \param[out]  pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GetPlanesImage ( ID_TYPE           CamId,
                                                  UINT8_OUT_TYPE    pR,
                                                  UINT8_OUT_TYPE    pG,
                                                  UINT8_OUT_TYPE    pB,
                                                  UINT8_OUT_TYPE    pY,
                                                  UINT32_IN_TYPE    nTimeOut,
                                                  S_SIS_DATA        *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GetPlanesImage ( ID_TYPE           CamId,
                                                  UINT8_OUT_TYPE    pR,
                                                  UINT8_OUT_TYPE    pG,
                                                  UINT8_OUT_TYPE    pB,
                                                  UINT8_OUT_TYPE    pY,
                                                  UINT32_IN_TYPE    nTimeOut,
                                                  S_SIS_DATA        *pSISData );
#endif

/**
* Request image data from the async receive buffer in transport format as supported by 1394 DCAM cameras. 
* Image format conversions may be required to retrieve displayable data.
* This function is provided for backwards compatibility. GigE-Vision image frames are converted to match 
* DCAM image formats, when necessary (introducing processing overhead). Use ::UCC_GetNativeImageEx to obtain 
* data in actual, interface-dependent transport format.
* A byte pointer for the target image is used. Sufficient memory space has to be provided by the caller.
*
* \param[in]    CamId       Camera identifier (serial number)
* \param[out]   pImage      Pointer to image bitmap
* \param[in]    nTimeOut    timeout
* \param[out]   pSISData    Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GetNativeImage ( ID_TYPE           CamId,
                                                  UINT8_OUT_TYPE    pImage,
                                                  UINT32_IN_TYPE    nTimeOut,
                                                  S_SIS_DATA        *pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GetNativeImage ( ID_TYPE           CamId,
                                                  UINT8_OUT_TYPE    pImage,
                                                  UINT32_IN_TYPE    nTimeOut,
                                                  S_SIS_DATA        *pSISData );
#endif

/**
* Request image data from the async receive buffer in native transport format.
* Image format conversions may be required to retrieve displayable data.
* For certain color codes (mostly 'deep' image formats), the output format
* differs between 1394 DCAM and GigE-Vision cameras.
* Relevant format details are returned in psFormatInfo.
* A byte pointer for the target image is used. Sufficient memory space has to be provided by the caller.
*
* \param[in]    CamId          Camera identifier (serial number)
* \param[out]   pImage         Pointer to image bitmap
* \param[out]   psFormatInfo   returns detailed input format specifications
* \param[out]   pnFrameCount   returns frame count - may be NULL
* \param[in]    nTimeOut       timeout
* \param[out]   pSISData       Optional secure image signature data (set \ref DESC_FEAT_SIS "E_FEAT_SIS" to \ref E_FEATSTATE_ON to receive images with embedded signature)
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_GetNativeImageEx ( ID_TYPE                       CamId,
                                                    UINT8_OUT_TYPE                pImage,
                                                    S_UNI_TRANSPORT_FORMAT_INFO * psFormatInfo, 
                                                    UINT32_OUT_TYPE               pnFrameCount,
                                                    UINT32_IN_TYPE                nTimeOut,
                                                    S_SIS_DATA *                  pSISData = 0);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_GetNativeImageEx ( ID_TYPE                       CamId,
                                                    UINT8_OUT_TYPE                pImage,
                                                    S_UNI_TRANSPORT_FORMAT_INFO * psFormatInfo, 
                                                    UINT32_OUT_TYPE               pnFrameCount,
                                                    UINT32_IN_TYPE                nTimeOut,
                                                    S_SIS_DATA *                  pSISData );
#endif

/*@}*/

//////////////////////////////////////////////////////////////////////////////
///////////////////////////   Features   /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/** @name Camera configuration. 
A set of methods to deal with camera features and how to set them.
*/
/*@{*/

/**
* Load the camera settings from file.
 *
 * \param[in]   CamId               Camera identifier (serial number)
 * \param[in]   pszFileName         The name of the file to use for storing settings
 * \param[in]   nSelectMode         See ::E_UNI_SETTINGS_SELECT_MODE
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 */ 
UNI_RETURN_TYPE WIN32DLL_API UCC_LoadSettings ( ID_TYPE             CamId,
                                                const ACHAR_TYPE    *pszFileName,
                                                UINT32_IN_TYPE      nSelectMode );

/**
 * Save the camera settings to a file.
 *
 * \param[in]   CamId           Camera identifier (serial number)
 * \param[in]   pszFileName     The name of the file to use for storing settings
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 */ 
UNI_RETURN_TYPE WIN32DLL_API UCC_SaveSettings ( ID_TYPE             CamId, 
                                                const ACHAR_TYPE    *pszFileName );

/**
 * Save the camera settings as a template for the camera family to a file.
 *
 * \param[in]   CamId                   Camera identifier (serial number)
 * \param[in]   pszFileName             The name of the file to use for storing settings
 * \param[in]   nSelectMode             See ::E_UNI_SETTINGS_SELECT_MODE
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 */ 
UNI_RETURN_TYPE WIN32DLL_API UCC_SaveSettingsEx (           ID_TYPE          CamId, 
                                                    const   ACHAR_TYPE      *pszFileName,
                                                            UINT32_TYPE      nSelectMode );

/**
 * Set the given register value.
 *
 * \param[in]   CamId     The ID of the camera to use 
 * \param[in]   uAddress  The address of the register
 * \param[in]   uValue    The new value to set
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_SetRegister ( ID_TYPE          CamId,
                                               UINT32_IN_TYPE   uAddress,
                                               UINT32_IN_TYPE   uValue );

/**
 * Get the value of the given register.
 *
 * \param[in]   CamId     The ID of the camera to use 
 * \param[in]   uAddress  The address of the register
 * \param[out]  pnValue   Pointer to ::UINT32_TYPE that receives the registers value
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_GetRegister ( ID_TYPE          CamId,
                                               UINT32_IN_TYPE   uAddress,
                                               UINT32_OUT_TYPE  pnValue );



/**
* Write memory block to camera.
*
* \param[in]   CamId     The ID of the camera to use 
* \param[in]   uAddress  The start address of the register block
* \param[out]  pnValue   Pointer to ::UINT32_TYPE array that holds the registers values
* \param[in]   uLength   element count to write
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_WriteBlock ( ID_TYPE          CamId,
                                             UINT32_IN_TYPE    uAddress,
                                             UINT32_IN_TYPE   *pnValue,
                                             UINT32_IN_TYPE    uLength);

/**
* Read memory block from camera.
*
* \param[in]   CamId     The ID of the camera to use 
* \param[in]   uAddress  The start address of the register block
* \param[out]  pnValue   Pointer to ::UINT32_TYPE array that receives the registers values
* \param[in]   uLength   element count to read
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ReadBlock   ( ID_TYPE          CamId,
                                              UINT32_IN_TYPE    uAddress,
                                              UINT32_OUT_TYPE   pnValue,
                                              UINT32_IN_TYPE    uLength);


/**
 * Check whether a specific unified feature is supported by the camera.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )

 * \return  <dl><dt>S_OK</dt><dd>if feature nFeature is supported by the camera</dd>
 *           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature is not supported by the camera</dd></dl>
 * \note For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_HasFeature ( ID_TYPE             CamId,
                                              UINT32_IN_TYPE      nFeature);

/**
 * Check whether a specific state is supported for the mentioned feature.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )
 * \param[in]   nState      status of the feature to be checked ( see ::E_UNI_FEATURE_STATE for 
 *                          valid values )
 * \return  <dl><dt>S_OK</dt><dd>if feature nFeature supports the state nState</dd>
 *          <dt>::UNI_RESULT_INVALID_FEATURECAPABILITY</dt><dd>if feature nFeature does not support the state nState</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature is not supported by the camera</dd></dl>
 * For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_HasFeatureStatus ( ID_TYPE             CamId,
                                                    UINT32_IN_TYPE      nFeature,
                                                    UINT32_IN_TYPE      nState );

/**
 * Get the status of the mentioned feature.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )
 * \param[out]  pnState     A pointer to ::UINT32_TYPE that will receive the status of the feature.
 *                          ( see ::E_UNI_FEATURE_STATE for valid values )

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature isn't supported by the camera</dd></dl>
 * \note For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_GetFeatureStatus ( ID_TYPE                 CamId, 
                                                    UINT32_IN_TYPE          nFeature, 
                                                    UINT32_OUT_TYPE         pnState );

/**
 * Set the status of the mentioned feature.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )
 * \param[in]   nState      Status of the feature to set ( see ::E_UNI_FEATURE_STATE for valid values )

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_INVALID_FEATURECAPABILITY</dt><dd>if feature nFeature does not support the state nState</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature isn't supported by the camera</dd></dl>
 * \note For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_SetFeatureStatus ( ID_TYPE             CamId,
                                                    UINT32_IN_TYPE      nFeature,
                                                    UINT32_IN_TYPE      nState );


/**
 * Get the value of the mentioned feature.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )
 * \param[out]  pnValue     A pointer to ::UINT32_TYPE that will receive the features current value

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_INVALID_FEATURECAPABILITY</dt><dd>if feature nFeature does not imply a value</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature is not supported by the camera</dd></dl>
 * \note For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_GetFeatureValue ( ID_TYPE          CamId,
                                                   UINT32_IN_TYPE   nFeature,
                                                   UINT32_OUT_TYPE  pnValue );

/**
 * Set the value of the mentioned feature.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )
 * \param[in]   nValue      The value of the feature to set

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_INVALID_FEATURECAPABILITY</dt><dd>if feature nFeature does not imply a value or the value is read-only</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature is not supported by the camera</dd>
 *          <dt>::UNI_RESULT_PARAMETER_INVALID_3</dt><dd>if nValue exceeds the range allowed for the chosen feature</dd></dl>
 * \note For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_SetFeatureValue ( ID_TYPE          CamId,
                                                   UINT32_IN_TYPE   nFeature,
                                                   UINT32_IN_TYPE   nValue );

/**
 * Get the minimum value of the mentioned feature.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )
 * \param[out]  pnValue     A pointer to ::UINT32_TYPE that will receive the features minimum value

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_INVALID_FEATURECAPABILITY</dt><dd>if feature nFeature does not imply a value</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature is not supported by the camera</dd></dl>
 * \note For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_GetFeatureMin ( ID_TYPE            CamId,
                                                 UINT32_IN_TYPE     nFeature,
                                                 UINT32_OUT_TYPE    pnValue );

/**
 * Get the maximum value of the mentioned feature.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFeature    ID of the feature ( see ::E_UNI_FEATURE for valid values )
 * \param[out]  pnValue     A pointer to ::UINT32_TYPE that will receive the features maximum value
 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_INVALID_FEATURECAPABILITY</dt><dd>if feature nFeature does not imply a value</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if feature nFeature is not supported by the camera</dd></dl>
 * \note For general information about unified features see \ref unifeat.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_GetFeatureMax ( ID_TYPE            CamId,
                                                 UINT32_IN_TYPE     nFeature,
                                                 UINT32_OUT_TYPE    pnValue );

/**
* Get maximum speed for 1394 image streaming.
* \param[in]    CamId       The ID of the camera to use
* \param[out]   nMaxSpeed   A pointer to ::UINT32_TYPE that will receive the maximum supported streaming speed
*                           (see ::E_UNI_1394_SPEED for valid values)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera is connected to a GigE interface</dd></dl>
* \note This method is for FireWire only and therefore deprecated, use \ref UCC_GetDataBitRateMax for all 
* interface types.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetStreamSpeedMax( ID_TYPE CamId,
                                                    UINT32_OUT_TYPE nMaxSpeed);

/**
* Get current speed for 1394 image streaming.
* \param[in]    CamId       The ID of the camera to use
* \param[out]   pnSpeed     A pointer to ::UINT32_TYPE that will receive the currently used FireWire speed
*                           (see ::E_UNI_1394_SPEED for valid values)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera is connected to a GigE interface</dd></dl>
* \note This method is for FireWire only and therefore deprecated, use \ref UCC_GetDataBitRate for all 
* interface types.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetStreamSpeed ( ID_TYPE CamId,
                                                  UINT32_OUT_TYPE pnSpeed);

/**
* Set speed for 1394 image streaming.
* \param[in]    CamId       The ID of the camera to use
* \param[in]    nSpeed      FireWire speed (see ::E_UNI_1394_SPEED for valid values)
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera is connected to a GigE interface</dd></dl>
* \note This method is for FireWire only and therefore deprecated, use \ref UCC_SetDataBitRate for all 
* interface types.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_SetStreamSpeed ( ID_TYPE CamId,
                                                  UINT32_IN_TYPE nSpeed);

/**
* Get maximum data bit rate for image streaming.
* \param[in]    CamId       The ID of the camera to use
* \param[out]   nMaxSpeed   A pointer to ::UINT32_TYPE that will receive the maximum supported data bit rate
*                           (see ::E_UNI_DATABITRATE for valid values)
* \return       S_OK if successful
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetDataBitRateMax( ID_TYPE CamId,
                                                    UINT32_OUT_TYPE nMaxSpeed);

/**
* Get current data bit rate for image streaming.
* \param[in]    CamId       The ID of the camera to use
* \param[out]   pnSpeed     A pointer to ::UINT32_TYPE that will receive the currently used data bit rate
*                           (see ::E_UNI_DATABITRATE for valid values)
* \return       S_OK if successful
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetDataBitRate ( ID_TYPE CamId,
                                                  UINT32_OUT_TYPE pnSpeed);

/**
* Set data bit rate for image streaming.
* \param[in]    CamId       The ID of the camera to use
* \param[in]    nSpeed      Data bit rate
*                           (see ::E_UNI_DATABITRATE for valid values)
* \return       S_OK if successful
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_SetDataBitRate ( ID_TYPE CamId,
                                                  UINT32_IN_TYPE nSpeed);

/**
* Get the minimum and maximum value for a color correction matrix coefficient.
* \param[in]    CamId           The ID of the camera to use
* \param[out]   pnMin           A pointer to ::UINT32_TYPE that will receive the minimum value 
*                               (NULL means the minimum will not be returned)
* \param[out]   pnMax           A pointer to ::UINT32_TYPE that will receive the maximum value
*                               (NULL means the maximum will not be returned)
* \return       S_OK if successful
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ColorCorrectionMatrix_GetInfo ( ID_TYPE CamId,
                                                                 INT32_OUT_TYPE pnMin,
                                                                 INT32_OUT_TYPE pnMax);

/**
* Get the value of the color correction matrix coefficient with the given ID.
* \param[in]    CamId           The ID of the camera to use
* \param[in]    nSrcColorIx     The color index for the source part of the transformation
* \param[in]    nDstColorIx     The color index for the destination part of the transformation
* \param[out]   pnValue         A pointer to ::UINT32_TYPE that will receive the factor for the given coefficient
* \return       S_OK if successful
* \note The allowed values for the color indices are: 0 for red, 1 for green, and 2 for blue.
* The RG coefficient controlling the original color's red part in the resulting green value for example can be
* accessed by setting \c nSrcColorIx to 0 and \c nDstColorIx to 1.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ColorCorrectionMatrix_GetCoefficient ( ID_TYPE CamId,
                                                                        UINT32_IN_TYPE nSrcColorIx,
                                                                        UINT32_IN_TYPE nDstColorIx,
                                                                        INT32_OUT_TYPE pnValue);

/**
* Set the value of the color correction matrix coefficient with the given ID.
* \param[in]    CamId           The ID of the camera to use
* \param[in]    nSrcColorIx     The color index for the source part of the transformation
* \param[in]    nDstColorIx     The color index for the destination part of the transformation
* \param[in]    nValue          The factor that will be set for the given coefficient
* \return       S_OK if successful
* \note The allowed values for the color indices are: 0 for red, 1 for green, and 2 for blue.
* The RG coefficient controlling the original color's red part in the resulting green value for example can be
* accessed by setting \c nSrcColorIx to 0 and \c nDstColorIx to 1.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ColorCorrectionMatrix_SetCoefficient ( ID_TYPE CamId,
                                                                        UINT32_IN_TYPE nSrcColorIx,
                                                                        UINT32_IN_TYPE nDstColorIx,
                                                                        INT32_IN_TYPE nValue);

/**
* Get the color correction matrix.
* \param[in]    CamId           The ID of the camera to use
* \param[out]   pnCoefficients  A pointer to an array of at least 9 elements where the coefficients will be stored
* \return       S_OK if successful
* \note The coefficients will be stored in the following order: RR, GR, BR, RG, GG, BG, RB, GB, BB.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ColorCorrectionMatrix_Get ( ID_TYPE CamId,
                                                             INT32_TYPE * pnCoefficients);

/**
* Set the color correction matrix.
* \param[in]    CamId           The ID of the camera to use
* \param[in]    pnCoefficients  A pointer to an array of at least 9 elements holding the coefficients
* \return       S_OK if successful
* \note The coefficients are expected in the following order: RR, GR, BR, RG, GG, BG, RB, GB, BB.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ColorCorrectionMatrix_Set ( ID_TYPE CamId,
                                                             INT32_TYPE * pnCoefficients);



///////////////////////////////////////////////////////////////////////////////
//////////////// Methods for access to AVT features ///////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*@}*/

/** @name AVT-specific camera features. 
A set of methods to deal with AVT-specific camera features.
*/
/*@{*/


/**
 * Get a number of details about the look-up table feature.
 * Parameters the caller is not interested in may be set to
 * NULL. However, not all parameters may be set to NULL.
 * The size of single LUT entries is either 8 or 16 bit depending
 * on pnMaxValue: cameras with pnMaxValue>255 expect a LUT entry size
 * of 16 bit.
 *
 * \param[in]   CamId           The ID of the camera to configure
 * \param[out]  pnNumOfLuts     A pointer to ::UINT32_TYPE that will receive the
 *                              maximum number of look-up tables (may be NULL)
 * \param[out]  pnMaxValue      A pointer to ::UINT32_TYPE that will receive the
 *                              maximum value possible for a LUT entry (may be NULL)
 * \param[out]  pnNumOfValues   A pointer to ::UINT32_TYPE that will receive the
 *                              number of entries per lookup table (may be NULL)
 * \param[out]  pnMaxDataSize   A pointer to ::UINT32_TYPE that will receive the
 *                              maximum size of a LUT in bytes (may be NULL)

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the LUT feature</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Lut_GetInfo ( ID_TYPE             CamId,
                                                UINT32_OUT_TYPE    pnNumOfLuts, 
                                                UINT32_OUT_TYPE    pnMaxValue, 
                                                UINT32_OUT_TYPE    pnNumOfValues, 
                                                UINT32_OUT_TYPE    pnMaxDataSize );

/**
 * Get the current status of the LUT feature.
 *
 * \param[in]   CamId           The ID of the camera to get information about
 * \param[out]  *pbActive       A pointer to a "boolean" ::UINT32_TYPE value to receive the current activity status, optional can be NULL.
 * \param[out]  *pnLutIndex     A pointer to an ::UINT32_TYPE to receive the current LUT number, optional can be NULL.

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the LUT feature</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Lut_GetStatus ( ID_TYPE            CamId,
                                                 UINT32_OUT_TYPE    pbActive,
                                                 UINT32_OUT_TYPE    pnLutIndex );

/**
 * Use the given look-up table and set the status.
 *
 * \param[in]   CamId           The ID of the camera to configure
 * \param[in]   nLutNo          The number of the LUT to use or ::UNI_UNDEFINED
 * \param[in]   nActive         A "boolean" ::UINT32_TYPE value to determine the activity status or ::UNI_UNDEFINED

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the LUT feature</dd>
 *          <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if nLutNo exceeds the range of [0 ... (NumOfLuts-1)]</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Lut_Use ( ID_TYPE          CamId,
                                           UINT32_IN_TYPE   nLutNo,
                                           UINT32_IN_TYPE   nActive );

/**
 * Upload the given LUT.
 *  The data array may contain 8 bit or 16 bit values depending on the cameras capabilities.
 *  Use UCC_Lut_GetInfo to determine the appropriate data format. LUT data may only be uploaded when
 *  the camera is not grabbing.
 *
 * \param[in]   CamId           the ID of the camera to configure 
 * \param[in]   nLutNr          the number of the LUT to use for storing the given data
 * \param[out]  arrValue        pointer to the LUT data
 * \param[in]   nSize           size of LUT data in bytes
 * \return  <dl>
 *              <dt>S_OK</dt>                        <dd>if the operation was successful</dd>
 *              <dt>::UNI_RESULT_CAMERA_BUSY</dt>    <dd>if the camera is grabbing</dd>
 *          </dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Lut_Upload ( ID_TYPE           CamId,
                                              UINT32_IN_TYPE    nLutNr,
                                              UINT8_TYPE *      arrValue,
                                              UINT32_IN_TYPE    nSize );

/**
* Load LUT to camera from CSV file.
* \param[in] CamId    id of the camera to use
* \param[in] pszPath  full path to the CSV file
* \param[in] nLength  length of the path string
* \param[in] nLutNr   number of LUT to load to
* \param[in] nCvsPos  position of the data in the CSV file
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Lut_LoadFromCsv(   ID_TYPE             CamId,
                                                    const ACHAR_TYPE   *pszPath, 
                                                    UINT32_IN_TYPE      nLength,
                                                    UINT32_IN_TYPE      nLutNr, 
                                                    UINT32_IN_TYPE      nCvsPos);

/**
 * Get information about the capabilities of the shading correction feature.
 *
 * \param[in]   CamId           The ID of the camera to use 
 * \param[out]  puSizeMax      A pointer to an integer that will receive the
 *                          maximum size of a shading image

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the shading correction feature is not supported by the camera</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Shading_GetInfo ( ID_TYPE          CamId,
                                                   UINT32_OUT_TYPE  puSizeMax );

/**
 * Upload shading correction image data into the camera.
 *  The camera must not be grabbing for this function to succeed.
 *
 * \param[in]   CamId           The ID of the camera to use 
 * \param[in]   pImage          Pointer to image data to be uploaded
 * \param[in]   nImageSize      The size of the image in bytes

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the shading correction feature is not supported by the camera</dd>
 *          <dt>::UNI_RESULT_CAMERA_BUSY</dt><dd>if the camera is grabbing</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Shading_Upload ( ID_TYPE           CamId,
                                                  UINT8_TYPE *      pImage,
                                                  UINT32_IN_TYPE    nImageSize
                                                  );

/**
 * Download shading correction image data from the camera.
 * The camera must not be grabbing for this function to succeed.
 *
 * \param[in]   CamId           The ID of the camera to use 
 * \param[out]  pImage          Pointer to an image data buffer that will be filled with 
 *                          the downloaded shading image (MONO8)
 * \param   pnImageSize     Pointer to an UNIT32_TYPE that will receive the size of 
 *                          the downloaded image (in bytes) 

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the shading correction feature is not supported by the camera</dd>
 *          <dt>::UNI_RESULT_CAMERA_BUSY</dt><dd>if the camera is grabbing</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Shading_Download ( ID_TYPE             CamId,
                                                    UINT8_TYPE *        pImage,
                                                    UINT32_INOUT_TYPE   pnImageSize
                                                    );

/**
 * Invoke the build operation of a shading image inside the camera.
 * This function blocks until the build process finishes if nBlock is true.
 * Otherwise it returns without blocking. 
 * The camera must not be grabbing for this function to succeed.
 *
 * \param[in]   CamId       The ID of the camera to use 
 * \param[in]   nFrames     The number of images to use for the build operation
 * \param[in]   nBlock      A "boolean" long value to determine whether the function should block until the
 *                      build process finishes

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the shading correction feature is not supported by the camera</dd>
 *          <dt>::UNI_RESULT_CAMERA_BUSY</dt><dd>if the camera is grabbing</dd>
 *          <dt>::UNI_RESULT_CAMERA_TIMEOUT</dt><dd>if the camera is still busy building the shading image at the
 *          time the function returns</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Shading_Build ( ID_TYPE        CamId,
                                                 UINT32_IN_TYPE nFrames, 
                                                 UINT32_IN_TYPE nBlock);


/**
 * Get state of input pins.
 *
 * \param[in]  CamId         The ID of the camera to use
 * \param[in]  nWhich        The number of the input pin see UNI_MAX_INPUT_PINS
 * \param[out] pnMode        Pointer to ::UINT32_TYPE that will receive the input mode see E_INPUTPIN_MODES
 * \param[out] pnPolarity    Pointer to ::UINT32_TYPE that will receive the polarity
 * \param[out] pnState       Pointer to ::UINT32_TYPE that will receive the input state

 * \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the chosen input pin is not supported by the camera</dd></dl>
 * \note For general information see \ref digiotrig.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_InputPin_Get ( ID_TYPE         CamId,
                                                UINT32_IN_TYPE  nWhich,
                                                UINT32_OUT_TYPE pnMode,
                                                UINT32_OUT_TYPE pnPolarity,
                                                UINT32_OUT_TYPE pnState);

/**
 * Set state of input pins. The current states are returned.
 *
 * \param[in] CamId     The ID of the camera to use
 * \param[in] nWhich    The number of the input pin see UNI_MAX_INPUT_PINS
 * \param[in] nMode     The input mode see ::E_INPUTPIN_MODES
 * \param[in] nPolarity The polarity

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the chosen input pin is not supported by
 *          the camera</dd></dl>
 * \note For general information see \ref digiotrig.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_InputPin_Set ( ID_TYPE             CamId,
                                                UINT32_IN_TYPE   nWhich,
                                                UINT32_IN_TYPE   nMode,
                                                UINT32_IN_TYPE   nPolarity);

/**
 * Get state of output pins.
 *
 * \param[in]  CamId         The ID of the camera to use
 * \param[in]  nWhich        The number of the output pin see ::MAX_OUTPUT_PINS
 * \param[out] pnMode        Pointer to ::UINT32_TYPE that will receive the input mode see ::E_OUTPUTPIN_MODES
 * \param[out] pnPolarity    Pointer to ::UINT32_TYPE that will receive the polarity
 * \param[out] pnState       Pointer to ::UINT32_TYPE that will receive the output state

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the chosen output pin is not supported by
 *                                              the camera</dd></dl>
 * \note For general information see \ref digiotrig.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_OutputPin_Get ( ID_TYPE            CamId,
                                                 UINT32_IN_TYPE     nWhich,
                                                 UINT32_OUT_TYPE    pnMode,
                                                 UINT32_OUT_TYPE    pnPolarity,
                                                 UINT32_OUT_TYPE    pnState);

/**
 * Set state of output pins.
 *
 * \param[in] CamId     The ID of the camera to use
 * \param[in] nWhich    The number of the output pin see ::MAX_OUTPUT_PINS
 * \param[in] nMode     The output mode see ::E_OUTPUTPIN_MODES
 * \param[in] nPolarity The polarity
 * \param[in] nState    The output state

 * \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the chosen output pin is not supported by
 *                                              the camera</dd></dl>
 * \note For general information see \ref digiotrig.
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_OutputPin_Set ( ID_TYPE            CamId,
                                                 UINT32_IN_TYPE  nWhich,
                                                 UINT32_IN_TYPE  nMode,
                                                 UINT32_IN_TYPE  nPolarity,
                                                 UINT32_IN_TYPE  nState);


/**
 * Configure the serial communication.
 *
 * \param[in] CamId         the ID of the camera to use
 * \param[in] nBaudRate     the baud rate to be used  
 *                          see the definition of ::E_UNI_SIO_BAUDRATE for valid values
 * \param[in] nCharLength   character length setting, may be set to 7 or 8
 * \param[in] nParity       parity setting - see the definition of ::E_UNI_SIO_PARITY  
 *                          for valid values
 * \param[in] nStopBit      stop bits - see the definition of ::E_UNI_SIO_STOPBITS 
 *                          for valid values
 * \param[in] nMode         receive/transmit mode - see the definition of ::E_UNI_SIO_MODE 
 *                          for valid values

 * \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if serial I/O is not supported by the camera</dd>
 *           <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if nBaudRate is set to an undefined value</dd>
 *           <dt>::UNI_RESULT_PARAMETER_INVALID_3</dt><dd>if nCharLength is set to something other than 7 or 8</dd>
 *           <dt>::UNI_RESULT_PARAMETER_INVALID_4</dt><dd>if nParity is set to an undefined value</dd>
 *           <dt>::UNI_RESULT_PARAMETER_INVALID_5</dt><dd>if nStopBit is set to an undefined value</dd>
 *           <dt>::UNI_RESULT_PARAMETER_INVALID_6</dt><dd>if nMode is set to an undefined value</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_SerialIO_SetConfig ( ID_TYPE               CamId,
                                                      UINT32_IN_TYPE        nBaudRate,
                                                      UINT32_IN_TYPE        nCharLength,
                                                      UINT32_IN_TYPE        nParity,
                                                      UINT32_IN_TYPE        nStopBit,
                                                      UINT32_IN_TYPE        nMode);
/**
 * Get the configuration of the serial interface.
 *
 * \param[in]  CamId         the ID of the camera to use
 * \param[out] pnBaudRate    pointer to UINT32_TYPE that will receive the configured baud rate 
 *                           see the definition of ::E_UNI_SIO_BAUDRATE for valid values
 * \param[out] pnCharLength  pointer to UINT32_TYPE that will receive the character length setting
 *                           possible values are 7 and 8
 * \param[out] pnParity      pointer to UINT32_TYPE that will receive the parity setting 
 *                           see the definition of ::E_UNI_SIO_PARITY for valid values
 * \param[out] pnStopBit     pointer to UINT32_TYPE that will receive the current stop bit setting 
 *                           see the definition of ::E_UNI_SIO_STOPBITS for valid values
 * \param[out] pnMode        pointer to UINT32_TYPE that will receive the current Receive/Transmit Mode
 *                           see the definition of ::E_UNI_SIO_MODE for valid values
 * \param[out] pnBufferSize  pointer to UINT32_TYPE that will receive the maximum size of the receive/transmit 
 *                      data buffer

 * \return              <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *                      <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if serial I/O is not supported by the camera</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_SerialIO_GetConfig ( ID_TYPE            CamId,
                                                      UINT32_OUT_TYPE    pnBaudRate,
                                                      UINT32_OUT_TYPE    pnCharLength,
                                                      UINT32_OUT_TYPE    pnParity,
                                                      UINT32_OUT_TYPE    pnStopBit,
                                                      UINT32_OUT_TYPE    pnMode,
                                                      UINT32_OUT_TYPE    pnBufferSize);

/**
 * Transmit data via the camera's serial I/O interface.
 *
 * \param[in] CamId         the ID of the camera to use
 * \param[in] pnData        pointer to the data to be transmitted
 * \param pnLength          pointer to UINT32_TYPE that indicates the number of bytes to transmit
 *                          after function call this value will be set to the number of bytes actually transmitted
 * \param[in] nTimeout      the maximum period of time in milliseconds this function will wait for the data
 *                          to be transmitted

 * \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if serial I/O is not supported by the camera</dd>
 *           <dt>::UNI_RESULT_CAMERA_TIMEOUT</dt><dd>if the timeout condition occurred before all data was transmitted</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_SerialIO_Transmit ( ID_TYPE    CamId,
                                                     UINT8_TYPE         *pnData, 
                                                     UINT32_INOUT_TYPE  pnLength,
                                                     UINT32_IN_TYPE     nTimeout);

/**
* Get information of the internal state of the SIO receiver.
* \param[in]    CamId           the ID of the camera to use
* \param[out]   pbRxStatus      bool receiver state ready
* \param[out]   puBytesPending  number of bytes pending for delivery
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_SerialIO_ReceiveInfo(  ID_TYPE         CamId,
                                                        UINT32_OUT_TYPE pbRxStatus, 
                                                        UINT32_OUT_TYPE puBytesPending);
/**
 * Receive data via the cameras serial I/O interface.
 *
 * \param[in] CamId         the ID of the camera to use
 * \param[out] pnData       pointer to the data buffer to be filled
 * \param pnLength          pointer to UINT32_TYPE that indicates the number of bytes to be received
 *                          after function call this value will be set to the number of bytes actually received
 * \param[in] nTimeout      the maximum period of time in milliseconds this function will wait for the data
 *                          to be received

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if serial I/O is not supported by the camera</dd>
 *          <dt>::UNI_RESULT_CAMERA_TIMEOUT</dt><dd>if the timeout condition occurred before all data was received</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_SerialIO_Receive ( ID_TYPE         CamId,
                                                    UINT8_TYPE          *pnData, 
                                                    UINT32_INOUT_TYPE   pnLength,
                                                    UINT32_IN_TYPE      nTimeout);


/**
* Configure the AutoFunction AOI feature.
* Set the AOI that is used for controlling the "auto" mode of \ref ::E_FEAT_SHUTTER "Shutter"
* and \ref ::E_FEAT_GAIN "Gain" features.
*
* \param[in] CamId         The ID of the camera to use
* \param[in] nMode         Mode of operation see: ::E_UNI_FEATURE_STATE
* \param[in] nLeft         Left offset of the AutoFunction AOI (will be left untouched if set to ::UNI_UNDEFINED)
* \param[in] nTop          Top offset of the AutoFunction AOI (will be left untouched if set to ::UNI_UNDEFINED)
* \param[in] nWidth        Width of the AutoFunction AOI (will be left untouched if set to ::UNI_UNDEFINED)
* \param[in] nHeight       Height of the AutoFunction AOI (will be left untouched if set to ::UNI_UNDEFINED)

* \return <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*         <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if AutoFunction AOI is not supported by the camera</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if nMode is set to a value not supported</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_3</dt><dd>if nLeft is greater than the current image width</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_4</dt><dd>if nTop is geater than the current image height</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_5</dt><dd>if nWidth is greater than the current image width minus
*                                              the AutoFunction AOI left value</dd>
*         <dt>::UNI_RESULT_PARAMETER_INVALID_6</dt><dd>if nHeight is greater than the current image height minus
*                                              the AutoFunction AOI top value</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_AutoFunctionAOI_Set ( ID_TYPE         CamId,
                                                       UINT32_IN_TYPE  nMode,
                                                       UINT32_IN_TYPE  nLeft,
                                                       UINT32_IN_TYPE  nTop,
                                                       UINT32_IN_TYPE  nWidth,
                                                       UINT32_IN_TYPE  nHeight);

/**
* Get the current status of the Autofunction AOI feature.
* Parameters the caller is not interested in may be set to NULL.
*
* \param[in]  CamId         The ID of the camera to use
* \param[out] pnMode        Pointer to ::UINT32_TYPE that will receive the current
*                           mode of operation. For valid values see ::E_UNI_FEATURE_STATE.
* \param[out] pnTop         Pointer to ::UINT32_TYPE that will receive the top offset
* \param[out] pnLeft        Pointer to ::UINT32_TYPE that will receive the left offset
* \param[out] pnHeight      Pointer to ::UINT32_TYPE that will receive the height
* \param[out] pnWidth       Pointer to ::UINT32_TYPE that will receive the width

* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if autofunction AOI is not supported by the camera</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_AutoFunctionAOI_Get ( ID_TYPE          CamId,
                                                       UINT32_OUT_TYPE  pnMode,
                                                       UINT32_OUT_TYPE  pnLeft,
                                                       UINT32_OUT_TYPE  pnTop,
                                                       UINT32_OUT_TYPE  pnWidth,
                                                       UINT32_OUT_TYPE  pnHeight);

/**
 * Get info about the hue feature.
 * Parameters the caller is not interested in may be set to NULL.
 *
 * \param[in]   CamId           The ID of the camera
 * \param[out]  pfMinValue      A pointer to ::FLOAT_TYPE that will receive the
 *                              minimum value for the hue feature (may be NULL)
 * \param[out]  pfMaxValue      A pointer to ::FLOAT_TYPE that will receive the
 *                              maximum value for the hue feature (may be NULL)
 * \param[out]  pfUnitSize      A pointer to ::FLOAT_TYPE that will receive the
 *                              step size for changing the hue feature (may be NULL)

 * \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
 *          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the Hue feature</dd></dl>
 */
UNI_RETURN_TYPE WIN32DLL_API UCC_Hue_GetInfo ( ID_TYPE           CamId,
                                               FLOAT_OUT_TYPE    pfMinValue, 
                                               FLOAT_OUT_TYPE    pfMaxValue, 
                                               FLOAT_OUT_TYPE    pfUnitSize);

/**
* Set the hue value in degrees.
*
* \param[in]     CamId          The ID of the camera to use
* \param[in,out] pfValue        The hue value in degrees so set. On return this parameter contains 
*                               the actual hue value stored in the camera which might be different,
*                               if the unit size or allowed range is violated (see ::UCC_Hue_GetInfo)

* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the hue feature</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Hue_Set ( ID_TYPE          CamId,
                                           FLOAT_INOUT_TYPE pfValue);

/**
* Get the hue value in degrees.
*
* \param[in]     CamId          The ID of the camera to use
* \param[out]    pfValue        The current hue value in degrees

* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the hue feature</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Hue_Get ( ID_TYPE          CamId,
                                           FLOAT_OUT_TYPE   pfValue);

/**
* Get the status of the Whitebalance feature.
* Parameters the caller is not interested in may be set to NULL.
*
* \param[in]  CamId         The ID of the camera to use
* \param[out] pnMode        Pointer to ::UINT32_TYPE that will receive the current mode
*                           of operation. Possible values are:
                            - ::E_FEATSTATE_OFF
                            - ::E_FEATSTATE_ON
                            - ::E_FEATSTATE_AUTO
* \param[out] pnUBVal       Pointer to ::UINT32_TYPE that will receive the U or B value
* \param[out] pnVRVal       Pointer to ::UINT32_TYPE that will receive the V or R value

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the whitebalance feature</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Whitebalance_Get ( ID_TYPE          CamId,
                                                    UINT32_OUT_TYPE  pnMode,
                                                    UINT32_OUT_TYPE  pnUBVal,
                                                    UINT32_OUT_TYPE  pnVRVal);


/**
* Configure the Whitebalance feature.
*
* \param[in] CamId         The ID of the camera to use
* \param[in] nMode         Mode of operation<br>
*                          Possible values are:
                           - ::E_FEATSTATE_OFF
                           - ::E_FEATSTATE_ON
                           - ::E_FEATSTATE_AUTO
                           - ::E_FEATSTATE_ONEPUSH
                           - will be left untouched if set to ::E_FEATSTATE_UNDEFINED
* \param[in] nUBVal        U / B Value (will be left untouched if set to ::UNI_UNDEFINED)
* \param[in] nVRVal        V / R Value (will be left untouched if set to ::UNI_UNDEFINED)

* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the whitebalance feature</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if nMode is set to a value not supported</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_3</dt><dd>if nUBVal exceeds the allowed range for whitebalance values</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_4</dt><dd>if nVRVal exceeds the allowed range for whitebalance values</dd></dl>
* Parameters set to UNI_UNDEFINED will not be changed.
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Whitebalance_Set ( ID_TYPE         CamId,
                                                    UINT32_IN_TYPE  nMode,
                                                    UINT32_IN_TYPE  nUBVal,
                                                    UINT32_IN_TYPE  nVRVal);

/**
* Get the status of the Whitebalance feature.
* Parameters the caller is not interested in may be set to NULL.
*
* \param[in]  CamId         The ID of the camera to use
* \param[out] pnMode        Pointer to ::UINT32_TYPE that will receive the current mode
*                           of operation. Possible values are:
                            - ::E_FEATSTATE_OFF
                            - ::E_FEATSTATE_ON
                            - ::E_FEATSTATE_AUTO
* \param[out] pnUBVal       Pointer to ::UINT32_TYPE that will receive the U or B value
* \param[out] pnVRVal       Pointer to ::UINT32_TYPE that will receive the V or R value

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the whitebalance feature</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Whitebalance_Get ( ID_TYPE          CamId,
                                                    UINT32_OUT_TYPE  pnMode,
                                                    UINT32_OUT_TYPE  pnUBVal,
                                                    UINT32_OUT_TYPE  pnVRVal);


/**
* Configure the Whitebalance for GMCY sensors.
* Parameters set to ::UNI_UNDEFINED will not be changed
*
* \param[in] CamId         The ID of the camera to use
* \param[in] nMode         Mode of operation<br>
                           Possible values are:
                           - ::E_FEATSTATE_OFF
                           - ::E_FEATSTATE_ON
                           - ::E_FEATSTATE_AUTO
                           - will be left untouched if set to ::E_FEATSTATE_UNDEFINED
* \param[in] nGr_CyVal     mode 1:Gr Value; Mode 0:Gr/Cy Value
* \param[in] nMg_YeVal     mode 1:Mg Value; Mode 0:Mg/Ye Value
* \param[in] nCy_MgVal     mode 1:Cy Value; Mode 0:Cy/Mg Value
* \param[in] nYe_GrVal     mode 1:Ye Value; Mode 0:Ye/Gr Value

* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the whitebalance feature</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if nMode is set to a value not supported</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_3</dt><dd>if nGr_CyVal exceeds the allowed range for whitebalance values</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_4</dt><dd>if nMg_YeVal exceeds the allowed range for whitebalance values</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_5</dt><dd>if nCy_MgVal exceeds the allowed range for whitebalance values</dd>
*           <dt>::UNI_RESULT_PARAMETER_INVALID_6</dt><dd>if nYe_GrVal exceeds the allowed range for whitebalance values</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_Whitebalance_GMCY_Set ( ID_TYPE         CamId,
                                                    UINT32_IN_TYPE  nMode,
                                                    UINT32_IN_TYPE  nGr_CyVal,
                                                    UINT32_IN_TYPE  nMg_YeVal,
                                                    UINT32_IN_TYPE  nCy_MgVal,
                                                    UINT32_IN_TYPE  nYe_GrVal);

/**
* Get the status of the whitebalance feature for GMCY sensors.
* Parameters the caller is not interested in may be set to NULL
*
* \param[in]  CamId         The ID of the camera to use
* \param[out] pnMode        Pointer to UINT32_TYPE that will receive the current mode
*                           of operation. See ::UCC_Whitebalance_Set for possible values
* \param[out] pnGr_CyVal    Pointer to UINT32_TYPE that will receive the value: 
                            - Mode 0:Gr/Cy 
                            - Mode 1:Gr Value
* \param[out] pnMg_YeVal    Pointer to UINT32_TYPE that will receive the value:
                            - Mode 0:Mg/Ye
                            - Mode 1:Mg Value
* \param[out] pnCy_MgVal    Pointer to UINT32_TYPE that will receive the value:
                            - Mode 0:Cy/Mg
                            - Mode 1:Cy Value
* \param[out] pnYe_GrVal    Pointer to UINT32_TYPE that will receive the value: 
                            - Mode 0:Ye/Gr
                            - Mode 1:Ye Value

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the camera does not support the whitebalance feature for GMCY sensors</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_Whitebalance_GMCY_Get ( ID_TYPE          CamId,
                                                    UINT32_OUT_TYPE  pnMode,
                                                    UINT32_OUT_TYPE  pnGr_CyVal,
                                                    UINT32_OUT_TYPE  pnMg_YeVal,
                                                    UINT32_OUT_TYPE  pnCy_MgVal,
                                                    UINT32_OUT_TYPE  pnYe_GrVal);


/**
* Load a user profile from camera memory.
* The camera must not be grabbing for this function to succeed.
*
* \param[in] CamId         the ID of the camera to use
* \param[in] nProfileID    number of profile to be loaded

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the user profile feature is not supported by the camera</dd>
*          <dt>::UNI_RESULT_CAMERA_BUSY</dt><dd>if the camera is grabbing</dd></dl>

*/

UNI_RETURN_TYPE WIN32DLL_API UCC_UserProfile_Load ( ID_TYPE          CamId,
                                                    UINT32_IN_TYPE   nProfileID);

/**
* Save a user profile to camera memory. The camera must not be grabbing for this function to succeed.
* user profile 0 is not writable
*
* \param[in] CamId         the ID of the camera to use
* \param[in] nProfileID    number of profile to be saved
* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the user profile feature is not supported by the camera</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_UserProfile_Save ( ID_TYPE          CamId,
                                                    UINT32_IN_TYPE   nProfileID);

/**
* Set the default user profile ID.
* The default user profile will be loaded on startup.
*
* \param[in] CamId         the ID of the camera to use
* \param[in] nProfileID    new default profile ID

* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the user profile feature is not supported by the camera</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_UserProfile_SetDefault ( ID_TYPE          CamId,
                                                          UINT32_IN_TYPE    nProfileID);

/**
* Get the user profile error status.
*
* \param[in]  CamId         the ID of the camera to use
* \param[out] pnErrorCode   pointer to UINT32_TYPE that will receive the error code of
*                           the user profile feature<br>
*                           for interpretation of error codes see camera manual, 
*                           chapter 'User Profiles'

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the user profile feature is not supported by the camera</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_UserProfile_GetErrorStatus ( ID_TYPE          CamId,
                                                              UINT32_OUT_TYPE  pnErrorCode);

/**
* Get the currently selected user profile ID.
* This function may be used after camera startup to determine the
* loaded user set.
*
* \param[in]  CamId             the ID of the camera to use
* \param[out] pnProfileID       pointer to UINT32_TYPE that will receive the profile ID
* \param[out] pnProfileCount    pointer to UINT32_TYPE that will receive the number of 
*                               profiles supported by the camera

* \return   <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*           <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the user profile feature is not supported by the camera</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_UserProfile_GetInfo ( ID_TYPE           CamId,
                                                       UINT32_OUT_TYPE   pnProfileID,
                                                       UINT32_OUT_TYPE   pnProfileCount);
/**
* Get the current configuration of the trigger feature.
*
* \param[in]  CamId         the ID of the camera to use
* \param[out] pnMode        pointer to ::UINT32_TYPE that will receive the current
*                      trigger mode (see ::E_TRIGGER_MODES)
* \param[out] pnActive      a pointer to a "boolean" long value to receive the current activity status
* \param[out] pnPolarity    a pointer to a "boolean" long value to receive the status of the trigger
*                      signal inverter
* \param[out] pnValue       a pointer to a "boolean" long value to receive the status of the trigger
*                      signal

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the trigger feature is not supported by the camera</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_Trigger_Get( ID_TYPE         CamId, 
                                              UINT32_OUT_TYPE pnMode, 
                                              UINT32_OUT_TYPE pnActive, 
                                              UINT32_OUT_TYPE pnPolarity, 
                                              UINT32_OUT_TYPE pnValue );

/**
* Configure the trigger feature.
*
* \param[in] CamId         the ID of the camera to use
* \param[in] nMode         trigger mode (see ::E_TRIGGER_MODES)
* \param[in] nActive       "boolean" long value to set the current activity status
* \param[in] nPolarity     "boolean" long value to set the status of the trigger
*                      signal inverter

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the trigger feature is not supported by the camera</dd>
*          <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if the trigger mode uMode is not supported by the camera</dd></dl>
* \note For triggered image acquisition at least one input pin has to be chosen as trigger input 
* by using the UCC_InputPin functions. For general information see \ref digiotrig.
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_Trigger_Set( ID_TYPE        CamId,
                                              UINT32_IN_TYPE nMode, 
                                              UINT32_IN_TYPE nActive, 
                                              UINT32_IN_TYPE nPolarity);

/**
* Get information about the test image feature.
*
* \param[in] CamId          the ID of the camera to use
* \param[in] pnImageNr      pointer to ::UINT32_TYPE that will receive the number of the
*                           currently activated test image
*                           A value of 0 indicates that the test image feature is
*                           not active. (normal mode of operation)
* \param[in] pnImageCount   pointer to ::UINT32_TYPE that will receive the number of
*                           supported test images

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the test image feature is not supported by the camera</dd></dl>
* \note For general information see \ref digiotrig.
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_TestImage_GetInfo( ID_TYPE        CamId,
                                                    UINT32_OUT_TYPE pnImageNr,  
                                                    UINT32_OUT_TYPE pnImageCount); 


/**
* Set the active test image.
*
* \param[in] CamId          the ID of the camera to use
* \param[in] nImageNr       test image to be used
*                           A value of 0 deactivates the test image feature. In this case the
*                           actual camera image will be received. (normal mode of operation)

* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the test image feature is not supported by the camera</dd>
*          <dt>::UNI_RESULT_PARAMETER_INVALID_2</dt><dd>if the chosen test image is not supported by the camera</dd></dl>
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_TestImage_Set( ID_TYPE        CamId,
                                                UINT32_IN_TYPE nImageNr); 

/**
* Start a configuration update procedure that may consist of several configuration
* changes, which will take effect simultaneously when ::UCC_ParamUpdate_Apply is called.
*
* \param[in] CamId          the ID of the camera to use
*
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the feature is not supported by the camera</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ParamUpdate_Begin ( ID_TYPE CamId );

/**
* Finish a configuration update procedure that has been started by ::UCC_ParamUpdate_Begin 
* and apply all changes simultaneously.
*
* \param[in] CamId          the ID of the camera to use
*
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the feature is not supported by the camera</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_ParamUpdate_Apply ( ID_TYPE CamId );

/**
* Apply a list of feature configuration changes simultaneously. 
*
* Most of the camera-related features controllable by \ref unifeat "unified feature access" are supported.
* Feature configuration changes have to be provided as a ::UINT32_TYPE array. Each list entry is expected to
* span 3 ::UINT32_TYPE values, the first of which is interpreted as feature identifier according to ::E_UNI_FEATURE,
* followed by the desired value and state ( see ::E_UNI_FEATURE_STATE ). Value or state data set to ::UNI_UNDEFINED
* will be ignored. This allows to change only the feature state or the feature value while keeping the other
* unchanged.
*
* In case of failure a partial list may have been applied. Parameter \c pnErronousItem can be interpreted after 
* function call to determine erroneous items and failure cases which lead to partial applied lists.
*
* A simple example without any error handling is shown below:
* \code
* UINT32_TYPE arrFeatureList[] =
*    {
*        E_FEAT_SHUTTER,         UNI_UNDEFINED,  E_FEATSTATE_AUTO,
*        E_FEAT_GAIN,            5,              E_FEATSTATE_ON
*    };
*
*    UINT32_TYPE index;
*    UCC_SetFeatureList(m_CamId, arrFeatureList, 2);
* \endcode
* \param[in] CamId          the ID of the camera to use
* \param[in] pnListPtr      pointer to the first element of the feature configuration list
* \param[in] nNumOfEntries  Number of items contained by the feature configuration list
* \param[out]   pnErronousItem  reveals details in case of failure. The following values are possible:
*                               <dl><dt>0</dt><dd>An error occured which could not be traced to
*                                                               a certain list entry. Camera configuration remains unchanged.</dd>
*                                   <dt>1...nNumOfEntries</dt><dd>(one-based) index of the erronous item - 
*                                                               Camera configuration remains unchanged.</dd>
*                                   <dt>nNumOfEntries+1</dt><dd>function succeeded without errors</dd>
*                                   <dt>::UNI_UNDEFINED</dt><dd>An error occured which could not be traced to
*                                                               a certain list entry - the list has been applied partially.</dd></dl>
*
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the register list update feature is not supported by the camera</dd>
*          </dl>
* \note This method provides easy but limited access to the camera's 'register list update' feature. Alternatively, ::UCC_SetRegisterList
* may be used for a more flexible, low-level control.
*/
#ifdef __cplusplus
UNI_RETURN_TYPE WIN32DLL_API UCC_SetFeatureList ( ID_TYPE                    CamId,
                                                  UINT32_IN_TYPE*            pnListPtr,
                                                  UINT32_IN_TYPE             nNumOfEntries,
                                                  UINT32_OUT_TYPE            pnErronousItem = NULL);
#else
UNI_RETURN_TYPE WIN32DLL_API UCC_SetFeatureList ( ID_TYPE                    CamId,
                                                  UINT32_IN_TYPE*            pnListPtr,
                                                  UINT32_IN_TYPE             nNumOfEntries,
                                                  UINT32_OUT_TYPE            pnErronousItem);
#endif

/**
* Apply a list of direct register writes simultaneously. 
*
* This function utilizes the camera's 'register list update' feature to change the contents of several
* configuration registers.
* Register addresses and values have to be provided as a ::UINT32_TYPE array. Each list entry is expected to
* span 2 ::UINT32_TYPE values, the first of which is interpreted as register address followed by the value to be written.
*
* In case of failure a partial list may have been applied. Parameter \c pnNumOfEntries is adjusted to the number
* of applied register entries at the time of function call. 
*
* \note The maximum supported register list size may be determined via ::UCC_GetRegisterListInfo.
*
* \param[in]    CamId           the ID of the camera to use
* \param[in]    pnListPtr       pointer to the first element of the register configuration list
* \param        pnNumOfEntries  Number of items contained by the register configuration list - will be adjusted
*                               to the number of register writes actually performed at the time of function call.
*
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the register list update feature is not supported by the camera</dd>
*          </dl>
* \note This function provides a flexible, low-level access to the camera's 'register list update' feature. Alternatively, ::UCC_SetFeatureList
* may be used as an easier access method.
*/

UNI_RETURN_TYPE WIN32DLL_API UCC_SetRegisterList ( ID_TYPE                    CamId,
                                                   UINT32_IN_TYPE*            pnListPtr,
                                                   UINT32_INOUT_TYPE          pnNumOfEntries );

/**
* Provides information about the cameras 'register list update' feature. 
*
* \param[in] CamId                  the ID of the camera to use
* \param[out] pnMaxNumOfEntries     pointer to ::UINT32_TYPE that will receive the maximum possible size
*                                   of register update lists.
*
* \return  <dl><dt>S_OK</dt><dd>if the operation was successful</dd>
*          <dt>::UNI_RESULT_FEATURE_NOT_SUPPORTED</dt><dd>if the register list update feature is not supported by the camera</dd>
*          </dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_GetRegisterListInfo ( ID_TYPE                    CamId,
                                                       UINT32_OUT_TYPE            pnMaxNumOfEntries);

/*@}*/

/////////////////////////////////////////////////////////////////////////////
// Resolution selection methods
/** @name Resolution enumeration methods.
A group of methods for enumerating the resolutions of a camera.
*/
/*@{*/

/** 
Convert a preset to Width and Height.
    \param[in]      Preset  an ::UINT32_TYPE value defining a resolution preset
                            see the definition of ::E_UNI_RESOLUTION_PRESETS for valid values
    \param[out]     Width   return the width for this preset (may not be NULL)
    \param[out]     Height  return the Height for this preset (may not be NULL)
    \return         <dl><dt>S_OK</dt><dd>if all parameter are valid</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_PresetToResolution ( UINT32_IN_TYPE        Preset,
                                                      UINT32_OUT_TYPE           Width,
                                                      UINT32_OUT_TYPE           Height);

/** 
Create a screen mode resolution preset Enumeration for the Camera.
    \param[in]      CamId           Camera to test for supported presets
    \param[out]     PresetsEnum     Enumeration of supported presets (may not be NULL)
    \param[out]     PresetCount     Element count of returned presets (may not be NULL)
    \note           Enumerate all supported presets for your Camera, 
                    then use ::UCC_EnumPresets to get a individual preset for the camera.
                    A Preset may be converted to Width and Height with ::UCC_PresetToResolution.
                    you have to use ::UCC_DestroyPresets to clean up after use.
    \return         <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_CreatePresetsEnum ( ID_TYPE                CamId,
                                                     PRESETS_ENUMERATION *  PresetsEnum  , 
                                                     UINT32_OUT_TYPE        PresetCount );

/** 
Clean up the data needed for the preset enumeration.
    \param[in]  PresetsEnum the preset enumeration to destroy
    \return     <dl><dt>S_OK</dt><dd>if all parameter are valid</dd></dl>
    \note       after ::UCC_DestroyPresetsEnum, the enumeration is no longer valid. 
                But all acquired presets are still valid.
    @see        ::UCC_CreatePresetsEnum, ::UCC_EnumPresets
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_DestroyPresetsEnum ( PRESETS_ENUMERATION   PresetsEnum);

/** 
Select a preset from the enumeration.
    \param[out]     Preset      a ::UINT32_TYPE value representing a resolution preset
                                see the definition of ::E_UNI_RESOLUTION_PRESETS for valid values
    \param[in]      PresetsEnum Enumeration to select the preset from (may not be NULL)
    \param[in]      PresetNr    Position in the enumeration inside the range 
                                [0,value returned from ::UCC_CreatePresetsEnum]
    \return         <dl><dt>S_OK</dt><dd>if all parameter are valid</dd></dl>
    @see            ::UCC_CreatePresetEnum
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_EnumPresets (  UINT32_OUT_TYPE      Preset,
                                                PRESETS_ENUMERATION  PresetsEnum,
                                                UINT32_IN_TYPE       PresetNr);

/** 
Create an enumeration of all supported color codings for a camera with a given preset.
    \param[in]  CamId           Camera to enumerate
    \param[out] ColorCodeEnum   Returns the enumeration of the color modes (may not be NULL)
    \param[out] ColorCodeCount  Element count of the returned color codes (may not be NULL)
    \param[in]  Preset          a ::UINT32_TYPE value representing a resolution preset,
                                the valid color codings of which will be enumerated
                                see the definition of ::E_UNI_RESOLUTION_PRESETS for valid values
    \return         <dl><dt>S_OK</dt><dd>if the operation was successful</dd></dl>
    \note       For a given Camera and preset enumerate all supported color codings.
                use ::UCC_EnumColorCode to select a coding.
                destroy the enumeration after use with ::UCC_DestroyColorCodeEnum
    @see    UCC_EnumColorCode, UCC_DestroyColorCodeEnum
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_CreateColorCodeEnum ( ID_TYPE                  CamId,
                                                       COLORCODE_ENUMERATION *  ColorCodeEnum,
                                                       UINT32_OUT_TYPE          ColorCodeCount,
                                                       UINT32_IN_TYPE           Preset );

/** 
Clean up the data needed for the color code enumeration.
    \param[in]  ColorCodeEnum    the Enumeration to clean up
    \return     <dl><dt>S_OK</dt><dd>if the parameter is valid</dd></dl>
    \note       After destruction, the enumeration is not valid, but
                all selected color codings are still valid
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_DestroyColorCodeEnum ( COLORCODE_ENUMERATION ColorCodeEnum);

/** 
Select a color  code from an enumeration.
    \param[out]     ColorCode       return the selected color coding. See ::E_UNI_COLOR_CODE(may not be NULL) 
    \param[in]      ColorCodeEnum   the enumeration to select from (may not be NULL)
    \param[in]      CodeNr          position of the color coding to select
    \return         <dl><dt>S_OK</dt><dd>if all parameter are valid</dd></dl>
*/
UNI_RETURN_TYPE WIN32DLL_API UCC_EnumColorCode ( UINT32_TYPE            * ColorCode,
                                                 COLORCODE_ENUMERATION  ColorCodeEnum,
                                                 UINT32_IN_TYPE         CodeNr );

/*@}*/

#ifdef __cplusplus
 }
//}
#endif

#endif //#ifndef UNI_CONTROL_H