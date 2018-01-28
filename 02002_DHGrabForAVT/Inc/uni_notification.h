#ifndef UNI_NOTIFICATION_H_
#define UNI_NOTIFICATION_H_
/** \file uni_notification.h
* \brief Defines for API notifications.
*
* \par license
* This code is the property of Allied Vision Technologies. Nevertheless, you
* are absolutely free to use and modify it. The code is provided "as is" with
* no expressed or implied warranty. The author and AVT accept no liability if
* it causes any damage to your computer.
*
*/

#include "uni_defines.h"
#include "windows.h"


#ifdef NOTIFICATION_STD_CALL
#define NOTIFICATION_CALLING_CONVENTION _stdcall
#else
#define NOTIFICATION_CALLING_CONVENTION __cdecl
#endif

/** Callback function argument.*/
typedef struct S_UNI_CALLBACK_ARGUMENT
{
    VOID_POINTER    m_Parameter; //!< User defined parameter will be returned as-is
    UINT32_TYPE     m_CamId;     //!< Will be filled with the sending camera id when called
    UINT32_TYPE     m_Reserved;  //!< reserved

#ifdef __cplusplus
    /** default constructor if c++.
    */
    S_UNI_CALLBACK_ARGUMENT() 
        : m_Parameter(NULL), m_CamId(UNI_ALL_CAMERAS), m_Reserved(0)
    {    }
    /**
    * constructor with init if c++.
    * \param[in] Prm    user defined parameter which is passed every time the callback is called
    * \param[in] CamId  Canera ID for which the notification is registered default: ::UNI_ALL_CAMERAS
    */
    S_UNI_CALLBACK_ARGUMENT( VOID_POINTER Prm, UINT32_TYPE CamId = UNI_ALL_CAMERAS ) 
        : m_Parameter(Prm), m_CamId(CamId), m_Reserved(0)    
    {    }
#endif
} S_UNI_CALLBACK_ARGUMENT;

//! Declaration of callback type
typedef VOID_TYPE   ( NOTIFICATION_CALLING_CONVENTION *UNI_NOTIFICATION_CALLBACK_FUNCTION ) ( S_UNI_CALLBACK_ARGUMENT );

//! Declaration of the notification handle
typedef UINT32_TYPE UNI_NOTIFICATION_HANDLE;

/**
 Notification Callback struct.
 Will hold the callback function and the function argument
*/
typedef struct S_UNI_NOTIFICATION_CALLBACK
{
    UNI_NOTIFICATION_CALLBACK_FUNCTION  m_Callback;     //!< Callbackfunction which will be called as notification
    S_UNI_CALLBACK_ARGUMENT             m_Arg;          //!< Function argument which is passed with the callback function
#ifdef __cplusplus
    /** default constructor if c++.
    */
    S_UNI_NOTIFICATION_CALLBACK()
        : m_Callback(NULL)
    {    }
    /**
    * constructor with init.
    * \param[in] callback callback function for the notification
    * \param[in] arg      callback argument
    */
    S_UNI_NOTIFICATION_CALLBACK( UNI_NOTIFICATION_CALLBACK_FUNCTION callback, S_UNI_CALLBACK_ARGUMENT arg )
        :m_Callback(callback), m_Arg(arg) 
    {    }
#endif
} S_UNI_NOTIFICATION_CALLBACK;

/** Win API event notification.*/
typedef struct S_UNI_NOTIFICATION_EVENT
{
    HANDLE          m_Event;    //!< the initialized Event to use for notification 
    UINT32_TYPE     m_CamId;    //!< non-syncronized serial number from camera which triggered the last event
    UINT32_TYPE     m_Reserved; //!< reserved for internal use
#ifdef __cplusplus
    /** default constructor if c++*/
    S_UNI_NOTIFICATION_EVENT()
        : m_Event(NULL),m_CamId(UNI_ALL_CAMERAS), m_Reserved(0)
    {}
    /**
    *  constructor with variable init if c++.
    * \param[in] Event  windows event ::HANDLE
    * \param[in] CamId    Camera ID for which this notification is triggered
    */
    S_UNI_NOTIFICATION_EVENT( HANDLE *Event, UINT32_TYPE CamId = UNI_ALL_CAMERAS )
        : m_Event(Event),m_CamId(CamId), m_Reserved(0)
    {}
#endif
} S_UNI_NOTIFICATION_EVENT;

#ifdef _WINDOWS_
/** Windows message to send on notification.
    lParam will hold the sending camera id
*/
typedef struct S_UNI_WINDOWS_MESSAGE
{
    HWND            m_hWnd;     //!< Handle to the window to post the message
    UINT            m_Msg;      //!< Message id
    WPARAM          m_wParam;   //!< Reserved
    LPARAM          m_lParam;   //!< ID of sending camera
#ifdef __cplusplus
    /**default constructor if c++*/
    S_UNI_WINDOWS_MESSAGE()
        :m_hWnd(NULL), m_Msg(0),m_wParam(0), m_lParam(0)
    {}
    /**constructor with args.
    \param[in]  hWnd        Handle to window
    \param[in]  Msg         Message to be send to window when triggered
    \param[in]  wParam      Reserved
    \param[in]  lParam      Camera id
    */
    S_UNI_WINDOWS_MESSAGE( HWND hWnd, UINT Msg = 0, WPARAM wParam = 0, LPARAM lParam = 0 )
        :m_hWnd(hWnd), m_Msg(Msg),m_wParam(wParam), m_lParam(lParam)
    {}
#endif
} S_UNI_WINDOWS_MESSAGE;
#endif

//! Enumeration to id the notification type.
typedef enum E_UNI_NOTIFICATION_TYPE
{
    E_UNI_NOTIFICATION_CALLBACK, //!< the notification will use a user defined callback
    E_UNI_NOTIFICATION_EVENT,    //!< the notification will use a user defined event 
#ifdef _WINDOWS_
    E_UNI_NOTIFICATION_MESSAGE,  //!< the notification will use a windows message (needs windows.h)
#endif
    E_UNI_NOTIFICATION_NO_TYPE   //!< undefined type
} E_UNI_NOTIFICATION_TYPE;

/** UniAPI notification structure.*/
typedef struct S_UNI_NOTIFICATION
{   
    UINT32_TYPE                     m_NotificationType;             //!< id of the notification type - see \ref E_UNI_NOTIFICATION_TYPE 
    UINT32_TYPE                     m_CamId;                        //!< camera to register the notification for */
    S_UNI_NOTIFICATION_CALLBACK     m_NotificationCallback;         //!< callback used if m_NotificationType == E_UNI_NOTIFICATION_CALLBACK - see \ref S_UNI_NOTIFICATION_CALLBACK 
    S_UNI_NOTIFICATION_EVENT        m_NotificationEvent;            //!< structure defining the event used if m_NotificationType == E_UNI_NOTIFICATION_EVENT - see \ref S_UNI_NOTIFICATION_EVENT 
#ifdef _WINDOWS_
    S_UNI_WINDOWS_MESSAGE           m_NotificationWindowsMessage;   //!< structure defining the windows message used if m_NotificationType == E_UNI_NOTIFICATION_MESSAGE - see \ref S_UNI_WINDOWS_MESSAGE 
#endif
#ifdef __cplusplus
    S_UNI_NOTIFICATION()
        : m_CamId(UNI_ALL_CAMERAS), m_NotificationType(E_UNI_NOTIFICATION_NO_TYPE)
    {}
#endif

}S_UNI_NOTIFICATION;

/** Events you can register a notification for.*/
typedef enum E_UNI_NOTIFICATION_EVENTS
{
    E_UNI_NOTIFICATION_NODELIST_CHANGED,    //!< triggered when the internal bus node list has changed
    E_UNI_NOTIFICATION_ERROR_BUS,           //!< triggered when there was an error on the bus
    E_UNI_NOTIFICATION_FRAME_READY,         //!< triggered at the end of a frame transfer
    E_UNI_NOTIFICATION_FRAME_START,         //!< triggered at the beginning of a frame transfer (for FireWire only)
    E_UNI_NOTIFICATION_FRAME_DROPPED,       //!< triggered when a frame was dropped
    E_UNI_NOTIFICATION_ERROR,               //!< triggered for any error that occures (if you register for specific error you will get 2 notifications)
    E_UNI_NOTIFICATION_ERROR_FRAME,         //!< triggered when the driver reports a corrupted frame (for future use)
    E_UNI_NOTIFICATION_ERROR_DMA,           //!< triggered when the DMA was halted (for future use)
    E_UNI_NOTIFICATION_ERROR_TIMEOUT,       //!< timeout image 
    E_UNI_NOTIFICATION_ERROR_MISC,          //!< triggered when an unknown event occurred
    E_UNI_NOTIFICATION_BUS_RESET
} E_UNI_NOTIFICATION_EVENTS;

#endif