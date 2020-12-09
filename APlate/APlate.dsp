# Microsoft Developer Studio Project File - Name="APlate" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=APlate - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "APlate.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "APlate.mak" CFG="APlate - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "APlate - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "APlate - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "APlate - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../CommonCompile/Output"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../../Engine/BaseEng/Include" /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WNT" /FR /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../Lib/XTPToolkitPro/source" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 sx32w.lib opengl32.lib glu32.lib ACommon.lib Domyun.lib LineApp.lib APlateOutCalc.lib APlateDraw.lib ARoadConfig.lib APlateData.lib APlateUI.lib PlateGangje.lib AException.lib GeneralInput.lib APlateCalc.lib htmlhelp.lib APlateDBDraw.lib APlateOutBM.lib Shlwapi.lib /nologo /subsystem:windows /machine:I386 /out:"../CommonCompile/Output/APlate2010.exe" /libpath:"../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../Lib/ASolver" /libpath:"../../../Dll/ASolverdll" /libpath:"../../../Lib" /libpath:"../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../CommonCompile/Output"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "APlate - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../CommonCompile/OutputD"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD F90 /browser
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../../Engine/BaseEng/Include" /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "WNT" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /i "../../../Lib/XTPToolkitPro/source" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 sx32w.lib opengl32.lib glu32.lib ACommonD.lib DomyunD.lib LineAppD.lib APlateOutCalcD.lib APlateDrawD.lib ARoadConfigD.lib APlateDataD.lib APlateUID.lib PlateGangjeD.lib AExceptionD.lib GeneralInputD.lib APlateCalcD.lib htmlhelp.lib APlateDBDrawD.lib APlateOutBMD.lib Shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../CommonCompile/OutputD/APlate2010.exe" /pdbtype:sept /libpath:"../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../../Lib/ASolver" /libpath:"../../../Dll/ASolverDLL" /libpath:"../../../Lib" /libpath:"../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "APlate - Win32 Release"
# Name "APlate - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\APlate.cpp
# End Source File
# Begin Source File

SOURCE=.\APlate.rc
# End Source File
# Begin Source File

SOURCE=.\APlateDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\APlateView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\LockDataInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneDrawing.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneInput.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneStart.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WaitForSubProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\WmfSizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XTPTabClientWndInPlate.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\APlate.h

!IF  "$(CFG)" == "APlate - Win32 Release"

# Begin Custom Build - Engine Copy... 신종화, 심동윤, 이리나, 이상현, 황현묵 화이팅...
WkspDir=.
InputPath=.\APlate.h

"..\CommonCompile\Output\Out" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo Run WriteBuildDate.exe For Date Version....... 
	"$(WkspDir)\WriteBuildDate.exe" $(WkspDir)\..\include\  $(WkspDir)\..\include\versionNumber.inc $(BUILD_NUMBER) 
	$(WkspDir)\PostBuild_Release.bat 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "APlate - Win32 Debug"

# Begin Custom Build - Engine Copy... 신종화, 심동윤, 이리나, 이상현, 황현묵 화이팅...
WkspDir=.
InputPath=.\APlate.h

"..\CommonCompile\OutputD\OutD" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo Run WriteBuildDate.exe For Date Version....... 
	"$(WkspDir)\WriteBuildDate.exe"    $(WkspDir)\..\include\     $(WkspDir)\..\include\versionNumber.inc    BUILD_NUMBER                                       $(WkspDir)\  
	$(WkspDir)\PostBuild_Debug.bat 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\APlateDoc.h
# End Source File
# Begin Source File

SOURCE=.\APlateView.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\LockDataInfo.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneDrawing.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneInput.h
# End Source File
# Begin Source File

SOURCE=.\ShortcutPaneStart.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\WaitForSubProgram.h
# End Source File
# Begin Source File

SOURCE=.\WmfSizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\XTPTabClientWndInPlate.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\APlate.ico
# End Source File
# Begin Source File

SOURCE=.\res\APlate.rc2
# End Source File
# Begin Source File

SOURCE=.\res\APlate2006.ico
# End Source File
# Begin Source File

SOURCE=.\res\APlateDoc.ico
# End Source File
# Begin Source File

SOURCE=.\APlateSplash.bmp
# End Source File
# Begin Source File

SOURCE=.\APlateSplash_TU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_l.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drawing.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_apla.ico
# End Source File
# Begin Source File

SOURCE=.\res\input_head.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\move.cur
# End Source File
# Begin Source File

SOURCE=.\res\shortcut_big.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shortcut_small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\support.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tabimage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3_GEN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3_TU.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
