# Microsoft Developer Studio Project File - Name="Gangje" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Gangje - Win32 Accuracy
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PlateGangje.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PlateGangje.mak" CFG="Gangje - Win32 Accuracy"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Gangje - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Gangje - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Gangje - Win32 Accuracy" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Gangje - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 LineApp.lib APlateData.lib opengl32.lib glu32.lib XLReport.lib ARoadConfig.lib AException.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\PlateGangje.def" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Gangje - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "../../../../Engine/BaseEng/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Fr /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../../Lib/XTPToolkitPro/source" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 LineAppD.lib APlateDataD.lib opengl32.lib glu32.lib XLReportD.lib ARoadConfigD.lib AExceptionD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\PlateGangjeD.def" /out:"../../CommonCompile/OutputD/PlateGangjeD.dll" /pdbtype:sept /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Gangje - Win32 Accuracy"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Gangje___Win32_Accuracy"
# PROP BASE Intermediate_Dir "Gangje___Win32_Accuracy"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\vc\commonCompile\lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_ACCURACY" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ACommonD.lib DomyunD.lib LineAppD.lib opengl32.lib glu32.lib XLReportD.lib ARoadConfigD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\Gangje.def" /out:"../CommonCompile/OutputD/Gangje.dll" /implib:"../CommonCompile/OutputD/GangjeD.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ACommonD.lib DomyunD.lib LineAppD.lib opengl32.lib glu32.lib XLReportD.lib ARoadConfigD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\Gangje.def" /out:"../CommonCompile/Output/Gangje.dll" /implib:"../CommonCompile/OutputD/GangjeD.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Gangje - Win32 Release"
# Name "Gangje - Win32 Debug"
# Name "Gangje - Win32 Accuracy"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BMCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\BMRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\Gangje.cpp
# End Source File
# Begin Source File

SOURCE=.\Gangje.rc
# End Source File
# Begin Source File

SOURCE=.\GangjeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TStandard.cpp
# End Source File
# Begin Source File

SOURCE=.\VAbortPgsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WeldCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\WeldRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\WeldStandard.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BMCreator.h
# End Source File
# Begin Source File

SOURCE=.\BMRecord.h
# End Source File
# Begin Source File

SOURCE=.\Gangje.h
# End Source File
# Begin Source File

SOURCE=.\GangjeBase.h
# End Source File
# Begin Source File

SOURCE=.\PaintCreator.h
# End Source File
# Begin Source File

SOURCE=.\PaintCreator.h
# End Source File
# Begin Source File

SOURCE=.\PaintRecord.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TStandard.h
# End Source File
# Begin Source File

SOURCE=.\VAbortPgsDlg.h
# End Source File
# Begin Source File

SOURCE=.\WeldCreator.h
# End Source File
# Begin Source File

SOURCE=.\WeldRecord.h
# End Source File
# Begin Source File

SOURCE=.\WeldStandard.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
