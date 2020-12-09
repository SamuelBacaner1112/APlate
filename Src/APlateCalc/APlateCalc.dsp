# Microsoft Developer Studio Project File - Name="APlateCalc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=APlateCalc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "APlateCalc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "APlateCalc.mak" CFG="APlateCalc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "APlateCalc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "APlateCalc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "APlateCalc - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib glu32.lib LineApp.lib ARoadConfig.lib PlateGangje.lib AException.lib APlateDBDraw.lib APlateData.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\APlateCalc.def" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Dll/ASolverDLL" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "APlateCalc - Win32 Debug"

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
# ADD LINK32 opengl32.lib glu32.lib LineAppD.lib ARoadConfigD.lib PlateGangjeD.lib AExceptionD.lib APlateDBDrawD.lib APlateDataD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\APlateCalcD.def" /out:"../../CommonCompile/OutputD/APlateCalcD.dll" /pdbtype:sept /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Dll/ASolverDLL" /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "APlateCalc - Win32 Release"
# Name "APlateCalc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\APlateCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateCalc.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\APlateCalc.rc
# End Source File
# Begin Source File

SOURCE=.\APlateCalcStd.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcFloor.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\DanMyunProp.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignCrossBeam.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignStiff.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementResult.cpp
# End Source File
# Begin Source File

SOURCE=.\FemCantileverCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\FemHLLCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\FemModelingCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\FemStressCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\JointResult.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateDesigCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\SAP2000Disp.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APlateCalc.h
# End Source File
# Begin Source File

SOURCE=.\APlateCalcStd.h
# End Source File
# Begin Source File

SOURCE=.\CalcFloor.h
# End Source File
# Begin Source File

SOURCE=.\CalcGeneral.h
# End Source File
# Begin Source File

SOURCE=.\DanMyunProp.h
# End Source File
# Begin Source File

SOURCE=.\Define.h
# End Source File
# Begin Source File

SOURCE=.\DesignCrossBeam.h
# End Source File
# Begin Source File

SOURCE=.\DesignStiff.h
# End Source File
# Begin Source File

SOURCE=.\ElementResult.h
# End Source File
# Begin Source File

SOURCE=.\FemCantileverCalc.h
# End Source File
# Begin Source File

SOURCE=.\FemHLLCalc.h
# End Source File
# Begin Source File

SOURCE=.\FEMManage.h
# End Source File
# Begin Source File

SOURCE=.\FemModelingCalc.h
# End Source File
# Begin Source File

SOURCE=.\FemModelingDraw.h
# End Source File
# Begin Source File

SOURCE=.\FemStressCalc.h
# End Source File
# Begin Source File

SOURCE=.\JointResult.h
# End Source File
# Begin Source File

SOURCE=.\PlateDesigCalc.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SAP2000Disp.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\APlateCalc.rc2
# End Source File
# End Group
# End Target
# End Project
