# Microsoft Developer Studio Project File - Name="APlateDBDraw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=APlateDBDraw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "APlateDBDraw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "APlateDBDraw.mak" CFG="APlateDBDraw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "APlateDBDraw - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APlateDBDraw - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "APlateDBDraw - Win32 Release"

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
# ADD LINK32 APlateData.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\APlateDBDraw.def" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "APlateDBDraw - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../../Lib/XTPToolkitPro/source" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 APlateDataD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\APlateDBDrawD.def" /out:"../../CommonCompile/OutputD/APlateDBDrawD.dll" /pdbtype:sept /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "APlateDBDraw - Win32 Release"
# Name "APlateDBDraw - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\APlateDBDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDBDraw.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\APlateDBDraw.rc
# End Source File
# Begin Source File

SOURCE=.\APlateDrawAbut.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawCamber.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawCross.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawDanmyun.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawDimJong.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawDimPyung.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawEtc.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawHBracing.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawJong.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawPyung.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawSebu.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawShoeCoord.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawStandard.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateDrawSteel.cpp
# End Source File
# Begin Source File

SOURCE=.\DimDBDeckBaseJong.cpp
# End Source File
# Begin Source File

SOURCE=.\DimDBDeckBasePlan.cpp
# End Source File
# Begin Source File

SOURCE=.\DimDomyunEx.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDBBaseRebarJong.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDBBaseRebarPlan.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDBBaseRebarTU.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDBDeck.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDBFrontRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawGuardFenceRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawOpenRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawSpacer.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawSupportRFBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateSymbolDom.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopSymbol.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APlateDrawAbut.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawCalc.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawCamber.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawCross.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawDanmyun.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawDimJong.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawDimPyung.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawEtc.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawHBracing.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawJong.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawLineInfo.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawPyung.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawSebu.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawShoeCoord.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawStandard.h
# End Source File
# Begin Source File

SOURCE=.\APlateDrawSteel.h
# End Source File
# Begin Source File

SOURCE=.\DimDBDeckBaseJong.h
# End Source File
# Begin Source File

SOURCE=.\DimDBDeckBasePlan.h
# End Source File
# Begin Source File

SOURCE=.\DimDomyunEx.h
# End Source File
# Begin Source File

SOURCE=.\DrawDBBaseRebarJong.h
# End Source File
# Begin Source File

SOURCE=.\DrawDBBaseRebarPlan.h
# End Source File
# Begin Source File

SOURCE=.\DrawDBBaseRebarTU.h
# End Source File
# Begin Source File

SOURCE=.\DrawDBDeck.h
# End Source File
# Begin Source File

SOURCE=.\DrawDBFrontRebar.h
# End Source File
# Begin Source File

SOURCE=.\DrawGuardFenceRebar.h
# End Source File
# Begin Source File

SOURCE=.\DrawOpenRebar.h
# End Source File
# Begin Source File

SOURCE=.\DrawSpacer.h
# End Source File
# Begin Source File

SOURCE=.\DrawSupportRFBar.h
# End Source File
# Begin Source File

SOURCE=.\PlateSymbolDom.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShopSymbol.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\APlateDBDraw.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
