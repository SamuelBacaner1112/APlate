# Microsoft Developer Studio Project File - Name="APlateUI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=APlateUI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "APlateUI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "APlateUI.mak" CFG="APlateUI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "APlateUI - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APlateUI - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "APlateUI - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "WNT" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ACommon.lib Domyun.lib LineApp.lib APlateData.lib ARoadConfig.lib ShGrid.lib APlateDBDraw.lib PlateGangje.lib AException.lib APlateDraw.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\APlateUI.def" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "APlateUI - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "WNT" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../../Lib/XTPToolkitPro/source" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ACommonD.lib DomyunD.lib LineAppD.lib APlateDataD.lib ARoadConfigD.lib ShGridD.lib APlateDBDrawD.lib PlateGangjeD.lib AExceptionD.lib  APlateDrawD.lib AExceptionD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\APlateUID.def" /out:"../../CommonCompile/OutputD/APlateUID.dll" /pdbtype:sept /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "APlateUI - Win32 Release"
# Name "APlateUI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\APlateUI.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateUI.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\APlateUI.rc
# End Source File
# Begin Source File

SOURCE=.\OptionBMPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionCamber.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionCrossBeam.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDeckRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDetailDim.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionEtcDetail.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionGen.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionGeneralDim.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionHBracing.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionPaint.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionPlanGen.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionShoe.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSlabGen.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSplice.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionStd.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionSummary.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionTendon.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionTUGirder.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionVBracing.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionWeldMap.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APlateUI.h
# End Source File
# Begin Source File

SOURCE=.\OptionBMPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionCamber.h
# End Source File
# Begin Source File

SOURCE=.\OptionCrossBeam.h
# End Source File
# Begin Source File

SOURCE=.\OptionDeckRebar.h
# End Source File
# Begin Source File

SOURCE=.\OptionDetail.h
# End Source File
# Begin Source File

SOURCE=.\OptionDetailDim.h
# End Source File
# Begin Source File

SOURCE=.\OptionEtcDetail.h
# End Source File
# Begin Source File

SOURCE=.\OptionGen.h
# End Source File
# Begin Source File

SOURCE=.\OptionGeneral.h
# End Source File
# Begin Source File

SOURCE=.\OptionGeneralDim.h
# End Source File
# Begin Source File

SOURCE=.\OptionHBracing.h
# End Source File
# Begin Source File

SOURCE=.\OptionLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\OptionPaint.h
# End Source File
# Begin Source File

SOURCE=.\OptionPlanGen.h
# End Source File
# Begin Source File

SOURCE=.\OptionSheet.h
# End Source File
# Begin Source File

SOURCE=.\OptionShoe.h
# End Source File
# Begin Source File

SOURCE=.\OptionSlabGen.h
# End Source File
# Begin Source File

SOURCE=.\OptionSplice.h
# End Source File
# Begin Source File

SOURCE=.\OptionStd.h
# End Source File
# Begin Source File

SOURCE=.\OptionSummary.h
# End Source File
# Begin Source File

SOURCE=.\OptionTendon.h
# End Source File
# Begin Source File

SOURCE=.\OptionTUGirder.h
# End Source File
# Begin Source File

SOURCE=.\OptionVBracing.h
# End Source File
# Begin Source File

SOURCE=.\OptionWeldMap.h
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

SOURCE=.\res\APlateUI.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
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
