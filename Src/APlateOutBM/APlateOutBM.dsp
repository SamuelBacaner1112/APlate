# Microsoft Developer Studio Project File - Name="APlateOutBM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=APlateOutBM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "APlateOutBM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "APlateOutBM.mak" CFG="APlateOutBM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "APlateOutBM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APlateOutBM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "APlateOutBM - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../CommonCompile/Output"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ARoadConfig.lib APlateData.lib LineApp.lib ShGrid.lib PlateGangje.lib XLReport.lib APlateDBDraw.lib APlateCalc.lib AException.lib GeneralInput.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\APlateOutBM.def" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "APlateOutBM - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../CommonCompile/OutputD"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../../Lib/XTPToolkitPro/source" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ARoadConfigD.lib APlateDataD.lib LineAppD.lib ShGridD.lib PlateGangjeD.lib XLReportD.lib APlateDBDrawD.lib APlateCalcD.lib AExceptionD.lib GeneralInputD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\APlateOutBMD.def" /out:"../../CommonCompile/OutputD/APlateOutBMD.dll" /pdbtype:sept /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "APlateOutBM - Win32 Release"
# Name "APlateOutBM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\APlateBMXL.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateNonDestructionXL.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateOutBM.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateOutBM.rc
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMBrInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMStd.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateOutNonDestructionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateOutWeldChangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateWeldChangeXL.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APlateBMXL.h
# End Source File
# Begin Source File

SOURCE=.\APlateNonDestructionXL.h
# End Source File
# Begin Source File

SOURCE=.\APlateOutBM.h
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMBrInfo.h
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMDlg.h
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMFormat.h
# End Source File
# Begin Source File

SOURCE=.\APlateOutBMStd.h
# End Source File
# Begin Source File

SOURCE=.\APlateOutNonDestructionDlg.h
# End Source File
# Begin Source File

SOURCE=.\APlateOutWeldChangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\APlateWeldChangeXL.h
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

SOURCE=.\res\APlateOutBM.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
