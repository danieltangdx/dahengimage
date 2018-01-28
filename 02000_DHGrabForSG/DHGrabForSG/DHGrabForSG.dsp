# Microsoft Developer Studio Project File - Name="GrabberRegularForSG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GrabberRegularForSG - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GrabberRegularForSG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GrabberRegularForSG.mak" CFG="GrabberRegularForSG - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GrabberRegularForSG - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GrabberRegularForSG - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GrabberRegularForSG - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Output"
# PROP Intermediate_Dir "../IntermediateFiles"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../Inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"../Lib"

!ELSEIF  "$(CFG)" == "GrabberRegularForSG - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../Inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dh_ImgLibEx.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"D:\Run-²ÉÑª¹Ü\GrabberRegularForSG.dll" /pdbtype:sept /libpath:"../Lib"

!ENDIF 

# Begin Target

# Name "GrabberRegularForSG - Win32 Release"
# Name "GrabberRegularForSG - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FileOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\GrabberExportForSG.cpp
# End Source File
# Begin Source File

SOURCE=.\GrabberRegularForSG.cpp
# End Source File
# Begin Source File

SOURCE=.\GrabberRegularForSG.def
# End Source File
# Begin Source File

SOURCE=.\GrabberRegularForSG.rc
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\SGParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulateGrabber.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FileOperate.h
# End Source File
# Begin Source File

SOURCE=.\Grabber.h
# End Source File
# Begin Source File

SOURCE=.\GrabberExportForSG.h
# End Source File
# Begin Source File

SOURCE=.\GrabberRegularForSG.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SGParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\SimulateGrabber.h
# End Source File
# Begin Source File

SOURCE=.\SpendTime.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\GrabberRegularForSG.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ModifyLog.txt
# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
# Section GrabberRegularForSG : {BC5D5FFD-1D97-4E86-A6A9-0DD685EE695D}
# 	1:16:IDD_DIALOG_PARAM:16001
# 	2:16:Resource Include:resource.h
# 	2:18:CLASS: CSGParamDlg:CSGParamDlg
# 	2:16:IDD_DIALOG_PARAM:IDD_DIALOG_PARAM
# 	2:10:ENUM: enum:enum
# 	2:12:SGParamDlg.h:SGParamDlg.h
# 	2:19:Application Include:GrabberRegularForSG.h
# 	2:14:SGParamDlg.cpp:SGParamDlg.cpp
# End Section
# Section GrabberRegularForSG : {7F4C03D7-E68E-4A37-9CEA-E8EB3A75D87D}
# 	1:16:IDD_DIALOG_PARAM:16003
# 	2:16:Resource Include:resource.h
# 	2:18:CLASS: CSGParamDlg:CSGParamDlg
# 	2:16:IDD_DIALOG_PARAM:IDD_DIALOG_PARAM
# 	2:10:ENUM: enum:enum
# 	2:12:SGParamDlg.h:SGParamDlg.h
# 	2:19:Application Include:GrabberRegularForSG.h
# 	2:14:SGParamDlg.cpp:SGParamDlg.cpp
# End Section
# Section GrabberRegularForSG : {E1464B36-7656-40D1-A208-B301C76F4333}
# 	1:16:IDD_DIALOG_PARAM:16000
# 	2:16:Resource Include:resource.h
# 	2:18:CLASS: CSGParamDlg:CSGParamDlg
# 	2:16:IDD_DIALOG_PARAM:IDD_DIALOG_PARAM
# 	2:10:ENUM: enum:enum
# 	2:12:SGParamDlg.h:SGParamDlg.h
# 	2:19:Application Include:GrabberRegularForSG.h
# 	2:14:SGParamDlg.cpp:SGParamDlg.cpp
# End Section
# Section GrabberRegularForSG : {859C7553-1357-4FA1-9626-DBBC236B8366}
# 	1:17:IDD_DIALOG_PARAM1:16002
# 	2:16:Resource Include:resource.h
# 	2:18:CLASS: CSGParamDlg:CSGParamDlg
# 	2:16:IDD_DIALOG_PARAM:IDD_DIALOG_PARAM1
# 	2:10:ENUM: enum:enum
# 	2:12:SGParamDlg.h:SGParamDlg1.h
# 	2:19:Application Include:GrabberRegularForSG.h
# 	2:14:SGParamDlg.cpp:SGParamDlg1.cpp
# End Section
