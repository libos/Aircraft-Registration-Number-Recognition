# Microsoft Developer Studio Project File - Name="leiGUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=leiGUI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "leiGUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "leiGUI.mak" CFG="leiGUI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "leiGUI - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "leiGUI - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "leiGUI - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "leiGUI - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 cv.lib cxcore.lib highgui.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "leiGUI - Win32 Release"
# Name "leiGUI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\leiGUI.cpp
# End Source File
# Begin Source File

SOURCE=.\leiGUI.rc
# End Source File
# Begin Source File

SOURCE=.\leiGUIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizeControl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\leiGUI.h
# End Source File
# Begin Source File

SOURCE=.\leiGUIDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResizeControl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\leiGUI.ico
# End Source File
# Begin Source File

SOURCE=.\res\leiGUI.rc2
# End Source File
# End Group
# Begin Group "Console Source Files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\csrc\CharOCR.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\cvExt.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\Detect.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\FeaQuan.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\Feature.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\IntegralFea.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\Knn.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\mser.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\RectSizer.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\SeqRect.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\Sobel.cpp
# End Source File
# Begin Source File

SOURCE=.\csrc\SVMachine.cpp
# End Source File
# Begin Source File

SOURCE=".\csrc\TLGL-Detect.cpp"
# End Source File
# End Group
# Begin Group "Console Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\csrc\CharOCR.h
# End Source File
# Begin Source File

SOURCE=.\csrc\cvExt.h
# End Source File
# Begin Source File

SOURCE=.\csrc\Detect.h
# End Source File
# Begin Source File

SOURCE=.\csrc\FeaQuan.h
# End Source File
# Begin Source File

SOURCE=.\csrc\Feature.h
# End Source File
# Begin Source File

SOURCE=.\csrc\IntegralFea.h
# End Source File
# Begin Source File

SOURCE=.\csrc\Knn.h
# End Source File
# Begin Source File

SOURCE=.\csrc\mser.h
# End Source File
# Begin Source File

SOURCE=.\csrc\RectSizer.h
# End Source File
# Begin Source File

SOURCE=.\csrc\SeqRect.h
# End Source File
# Begin Source File

SOURCE=.\csrc\Sobel.h
# End Source File
# Begin Source File

SOURCE=.\csrc\SVMachine.h
# End Source File
# End Group
# Begin Group "svm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\svm\svm.cpp
# End Source File
# Begin Source File

SOURCE=.\svm\svm.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
