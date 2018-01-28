; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSetCameraParam
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "avtgrabber.h"
LastPage=0

ClassCount=4
Class1=CAVTGrabberApp
Class2=CSetCameraParam

ResourceCount=3
Resource1=IDD_SET_CAMERA_DIALOG
Class3=Login
Resource2=IDD_DIALOG_LOGIN
Class4=PSW
Resource3=IDD_DIALOG_PSW

[CLS:CAVTGrabberApp]
Type=0
BaseClass=CWinApp
HeaderFile=AVTGrabber.h
ImplementationFile=AVTGrabber.cpp
LastObject=CAVTGrabberApp

[CLS:CSetCameraParam]
Type=0
BaseClass=CDialog
HeaderFile=SetCameraParam.h
ImplementationFile=SetCameraParam.cpp
LastObject=IDC_COMBO_TRIGGER_MODE
Filter=D
VirtualFilter=dWC

[DLG:IDD_SET_CAMERA_DIALOG]
Type=1
Class=CSetCameraParam
ControlCount=35
Control1=IDC_STATIC_LIGHT,static,1342308352
Control2=IDC_STATIC_GAIN,static,1342308352
Control3=IDC_STATIC_SHUTTER,static,1342308352
Control4=IDC_SLIDER_BRIGHTNESS,msctls_trackbar32,1342242840
Control5=IDC_EDIT_BRIGHTNESS,edit,1350631552
Control6=IDC_SLIDER_GAIN,msctls_trackbar32,1342242840
Control7=IDC_SLIDER_SHUTTER,msctls_trackbar32,1342242840
Control8=IDC_EDIT_GAIN,edit,1350631552
Control9=IDC_EDIT_SHUTTER,edit,1350631552
Control10=IDOK,button,1342242816
Control11=IDC_STATIC_CAMERA_PARAM,button,1342177287
Control12=IDC_STATIC_TRIGGER_MODE,button,1342177287
Control13=IDC_COMBO_TRIGGER_MODE,combobox,1344339971
Control14=IDC_BTN_APPLY,button,1342242816
Control15=IDC_STATIC_AOI_PARAM,button,1342177287
Control16=IDC_STATIC_LEFT,static,1342308352
Control17=IDC_STATIC_WIDTH,static,1342308352
Control18=IDC_STATIC_TOP,static,1342308352
Control19=IDC_STATIC_HEIGHT,static,1342308352
Control20=IDC_EDIT_AOI_LEFT,edit,1350639745
Control21=IDC_EDIT_AOI_WIDTH,edit,1350639745
Control22=IDC_EDIT_AOI_TOP,edit,1350639745
Control23=IDC_EDIT_AOI_HEIGHT,edit,1350639745
Control24=IDC_BUTTON_AOI_APPLY,button,1342242816
Control25=IDC_STATIC_FPS_TEST,button,1342177287
Control26=IDC_STATIC_FRAME,static,1342312961
Control27=IDC_BUTTON_FRAMETEST,button,1342242816
Control28=IDC_STATIC_AOI_INFO,static,1342312961
Control29=IDC_BUTTON_LOGIN,button,1342242816
Control30=IDC_STATIC_WB,button,1342177287
Control31=IDC_STATIC,static,1342308352
Control32=IDC_EDIT_CB,edit,1350631553
Control33=IDC_STATIC,static,1342308352
Control34=IDC_EDIT_CR,edit,1350631553
Control35=IDC_BUTTON_AOI_APPLYWB,button,1342242816

[DLG:IDD_DIALOG_LOGIN]
Type=1
Class=Login
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_PSW,edit,1350631584
Control5=IDC_BUTTON_APPLY,button,1208025088
Control6=IDC_BUTTON_NEWPSW,button,1342242816

[CLS:Login]
Type=0
HeaderFile=Login.h
ImplementationFile=Login.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_BUTTON_APPLY
VirtualFilter=dWC

[DLG:IDD_DIALOG_PSW]
Type=1
Class=PSW
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_OLDPSW,edit,1350631584
Control6=IDC_EDIT_NEWPSW,edit,1350631584

[CLS:PSW]
Type=0
HeaderFile=PSW.h
ImplementationFile=PSW.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_OLDPSW

