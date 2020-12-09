# Microsoft Developer Studio Project File - Name="APlateData" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=APlateData - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "APlateData.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "APlateData.mak" CFG="APlateData - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "APlateData - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APlateData - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "APlateData - Win32 Release"

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
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\APlateData.def" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Dll/ASolverDLL" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "APlateData - Win32 Debug"

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
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\APlateDataD.def" /out:"../../CommonCompile/OutputD/APlateDataD.dll" /pdbtype:sept /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Dll/ASolverDLL" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "APlateData - Win32 Release"
# Name "APlateData - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddRate.cpp
# End Source File
# Begin Source File

SOURCE=.\ADeckData.cpp
# End Source File
# Begin Source File

SOURCE=.\ADeckDataTU.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateData.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateData.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\APlateData.rc
# End Source File
# Begin Source File

SOURCE=.\BMOption.cpp
# End Source File
# Begin Source File

SOURCE=.\BoxOpenBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Bracing.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcData.cpp
# End Source File
# Begin Source File

SOURCE=.\Camber.cpp
# End Source File
# Begin Source File

SOURCE=.\CentSeparation.cpp
# End Source File
# Begin Source File

SOURCE=.\DataManage.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobarOption.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardFence.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardFenceRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\HBracingGusset.cpp
# End Source File
# Begin Source File

SOURCE=.\JackupStiff.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PierInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateBaseStru.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateBasicIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateBridge.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateBridgeApp.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateBxFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateBxObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateCrossBeam.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateFactChain.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateGirder.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateGirderApp.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatePieceData.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatePieceObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatePiecePoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatePiecePointArray.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateSplice.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateSpliceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\RangePress.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundOpenBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Section.cpp
# End Source File
# Begin Source File

SOURCE=.\Shoe.cpp
# End Source File
# Begin Source File

SOURCE=.\SlabApp.cpp
# End Source File
# Begin Source File

SOURCE=.\Spacer.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Steel.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelBolt.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelConc.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelConcData.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelDeduct.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelGrade.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelMark.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelPrestress.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelRange.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelScallop.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelSection.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelStud.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelUWeight.cpp
# End Source File
# Begin Source File

SOURCE=.\SteelWeld.cpp
# End Source File
# Begin Source File

SOURCE=.\SupportRFBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SupportRFBarSet.cpp
# End Source File
# Begin Source File

SOURCE=.\VBracing.cpp
# End Source File
# Begin Source File

SOURCE=.\VStiff.cpp
# End Source File
# Begin Source File

SOURCE=.\WebHStiff.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddRate.h
# End Source File
# Begin Source File

SOURCE=.\ADeckData.h
# End Source File
# Begin Source File

SOURCE=.\ADeckDataTU.h
# End Source File
# Begin Source File

SOURCE=.\APlateData.h
# End Source File
# Begin Source File

SOURCE=.\BMOption.h
# End Source File
# Begin Source File

SOURCE=.\BoxOpenBar.h
# End Source File
# Begin Source File

SOURCE=.\Bracing.h
# End Source File
# Begin Source File

SOURCE=.\CalcData.h
# End Source File
# Begin Source File

SOURCE=.\Camber.h
# End Source File
# Begin Source File

SOURCE=.\CentSeparation.h
# End Source File
# Begin Source File

SOURCE=.\DataManage.h
# End Source File
# Begin Source File

SOURCE=.\GlobarOption.h
# End Source File
# Begin Source File

SOURCE=.\GuardFence.h
# End Source File
# Begin Source File

SOURCE=.\GuardFenceRebar.h
# End Source File
# Begin Source File

SOURCE=.\HBracingGusset.h
# End Source File
# Begin Source File

SOURCE=.\JackupStiff.h
# End Source File
# Begin Source File

SOURCE=.\OpenBar.h
# End Source File
# Begin Source File

SOURCE=.\PierInfo.h
# End Source File
# Begin Source File

SOURCE=.\PlateBaseStru.h
# End Source File
# Begin Source File

SOURCE=.\PlateBasicIndex.h
# End Source File
# Begin Source File

SOURCE=.\PlateBridge.h
# End Source File
# Begin Source File

SOURCE=.\PlateBridgeApp.h
# End Source File
# Begin Source File

SOURCE=.\PlateBxFinder.h
# End Source File
# Begin Source File

SOURCE=.\PlateBxObject.h
# End Source File
# Begin Source File

SOURCE=.\PlateCrossBeam.h
# End Source File
# Begin Source File

SOURCE=.\PlateFactChain.h
# End Source File
# Begin Source File

SOURCE=.\PlateGirder.h
# End Source File
# Begin Source File

SOURCE=.\PlateGirderApp.h
# End Source File
# Begin Source File

SOURCE=.\PlatePieceData.h
# End Source File
# Begin Source File

SOURCE=.\PlatePieceObject.h
# End Source File
# Begin Source File

SOURCE=.\PlatePiecePoint.h
# End Source File
# Begin Source File

SOURCE=.\PlateSplice.h
# End Source File
# Begin Source File

SOURCE=.\PlateSpliceBase.h
# End Source File
# Begin Source File

SOURCE=.\RangePress.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoundOpenBar.h
# End Source File
# Begin Source File

SOURCE=.\Section.h
# End Source File
# Begin Source File

SOURCE=.\Shoe.h
# End Source File
# Begin Source File

SOURCE=.\SlabApp.h
# End Source File
# Begin Source File

SOURCE=.\Spacer.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Steel.h
# End Source File
# Begin Source File

SOURCE=.\SteelBolt.h
# End Source File
# Begin Source File

SOURCE=.\SteelConc.h
# End Source File
# Begin Source File

SOURCE=.\SteelConcData.h
# End Source File
# Begin Source File

SOURCE=.\SteelDeduct.h
# End Source File
# Begin Source File

SOURCE=.\SteelDefine.h
# End Source File
# Begin Source File

SOURCE=.\SteelGrade.h
# End Source File
# Begin Source File

SOURCE=.\SteelMark.h
# End Source File
# Begin Source File

SOURCE=.\SteelMaterial.h
# End Source File
# Begin Source File

SOURCE=.\SteelPrestress.h
# End Source File
# Begin Source File

SOURCE=.\SteelRange.h
# End Source File
# Begin Source File

SOURCE=.\SteelScallop.h
# End Source File
# Begin Source File

SOURCE=.\SteelSection.h
# End Source File
# Begin Source File

SOURCE=.\SteelStud.h
# End Source File
# Begin Source File

SOURCE=.\SteelUWeight.h
# End Source File
# Begin Source File

SOURCE=.\SteelWeld.h
# End Source File
# Begin Source File

SOURCE=.\SupportRFBar.h
# End Source File
# Begin Source File

SOURCE=.\SupportRFBarSet.h
# End Source File
# Begin Source File

SOURCE=.\VBracing.h
# End Source File
# Begin Source File

SOURCE=.\VStiff.h
# End Source File
# Begin Source File

SOURCE=.\WebHStiff.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\APlateData.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
