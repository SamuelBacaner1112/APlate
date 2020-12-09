# Microsoft Developer Studio Project File - Name="GeneralInput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=GeneralInput - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GeneralInput.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GeneralInput.mak" CFG="GeneralInput - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GeneralInput - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GeneralInput - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GeneralInput - Win32 Release"

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
# ADD LINK32 ARoadConfig.lib APlateData.lib LineApp.lib ShGrid.lib PlateGangje.lib APlateCalc.lib XLReport.lib AException.lib APlateDBDraw.lib BearingInfo.lib /nologo /subsystem:windows /dll /pdb:"Release/GeneralInput.pdb" /machine:I386 /def:".\GeneralInput.def" /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/Output" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Dll/ASolverDLL" /libpath:"../../../../Engine/BaseEng/CommonCompile/Output" /libpath:"../../CommonCompile/Output"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "GeneralInput - Win32 Debug"

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
# ADD LINK32 XLReportD.lib ARoadConfigD.lib APlateDataD.lib LineAppD.lib ShGridD.lib PlateGangjeD.lib APlateCalcD.lib AExceptionD.lib APlateDBDrawD.lib BearingInfoD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\GeneralInputD.def" /out:"../../CommonCompile/OutputD/GeneralInputD.dll" /pdbtype:sept /libpath:"../../../../Lib/XTPToolkitPro/CommonCompile/OutputD" /libpath:"../../../../Lib/ASolver" /libpath:"../../../../Dll/ASolverDLL" /libpath:"../../../../Engine/BaseEng/CommonCompile/OutputD" /libpath:"../../CommonCompile/OutputD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "GeneralInput - Win32 Release"
# Name "GeneralInput - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AbutPierDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BindConcJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BMOutSangPage.cpp
# End Source File
# Begin Source File

SOURCE=.\BMOutSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ByunDanmyunJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverDeadLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverDroop.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverLoadCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverModeling.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverModelResult.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverRebarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcDisplacement.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcFatigue.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcFloorSlabStress.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcFrameComboLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcInfluenceK.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcLossOfPrestress.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcModelingFramePropty.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcModelingResult.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcModelingSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcSecForceDivCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcSecForceDivResult.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcSecOutline.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcSecStress.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcSecStressPos.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcSectionConceptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcUltimateLoadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcWarping.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcWeld.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcWindForce.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CrossBeamSubInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DanmyunTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckBasicDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckBindConcRebarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckBMOptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckCrossStringerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDetailCyclesuDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDetailDivideRebar.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDetailMainLowerCenterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDetailMainLowerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDetailMainUpperDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDetailSettlementDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDistriRebarCTCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDistriRebarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckEtcRebarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckGuardFenceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckMainRebarCTCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckMainRebarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckMainSkewRebarCTCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckPlanRebarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckRebarInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckRebarJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckSabogangCTCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignBaseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignBMPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignBoltPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignConcretePage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignCSteelPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignCTSteelPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignDeductPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignFrictionCoeffPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignGradePage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignHSteelPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignISteelPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignLSteelPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignLVarSteelPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignMaterialPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignPageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignPrestressPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignPrestressPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignScallopPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignSpecialMovingLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignSteelDimPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignSteelMaterialPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignStudPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignUWeightPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignWeldPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DetBracketDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetBracketStiffDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetCamberJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetCrossBeamTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetDanTaperDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetEtcInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetFieldJoinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetHBracing.cpp
# End Source File
# Begin Source File

SOURCE=.\DetHBracingScallop.cpp
# End Source File
# Begin Source File

SOURCE=.\DetHStiffDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetLiftLugDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetPlateWShiftDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetSHConnectorVPosDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetSlabAnchor.cpp
# End Source File
# Begin Source File

SOURCE=.\DetSolePlateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetStagingRingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetVBracingTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetVStiffJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetWebHStiffPosDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogInstallBeryukAtBlockout.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogInstallMainAtBlockout.cpp
# End Source File
# Begin Source File

SOURCE=.\EarthQuackDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EarthQuackFactDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EarthQUackGradeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpansionJointTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FactJoinPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldJoinPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralBaseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralBaseStd.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralInput.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralInput.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GeneralInput.rc
# End Source File
# Begin Source File

SOURCE=.\GirHeightUserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardFenceTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HBracingPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HBrSpStdInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HDanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\JackupPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JackupSolePlateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JiganGusengDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JijumAngleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JijumStiffDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JointRestraintDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JRibPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\labelcontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintOutAssembly.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintOutSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PlateStressOut.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarAddRateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RebarTable.cpp
# End Source File
# Begin Source File

SOURCE=.\ReDir.cpp
# End Source File
# Begin Source File

SOURCE=.\Redirect.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RistModule.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectLineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelGirderHeightRangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShCheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ShearBarTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellFileOp.cpp
# End Source File
# Begin Source File

SOURCE=.\ShoeSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShoeZapyoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SlabJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SlabStructureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpliceBoltPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpliceDesignResult.cpp
# End Source File
# Begin Source File

SOURCE=.\StackDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StartPointJewonDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SubCrossHeightRistDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubInputTensionBarDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubRistRefImgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubSlabHunchInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TotalViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserDesignCheckDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VBracingPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VStiffPositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VStiffStdInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WeldOutAssembly.cpp
# End Source File
# Begin Source File

SOURCE=.\WeldOutSheet.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AbutPierDlg.h
# End Source File
# Begin Source File

SOURCE=.\BaseInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\BindConcJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\BMOutSangPage.h
# End Source File
# Begin Source File

SOURCE=.\BMOutSheet.h
# End Source File
# Begin Source File

SOURCE=.\ByunDanmyunJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverDeadLoad.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverDroop.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverLoadCombo.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverModeling.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverModelResult.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverRebarDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcCantileverSetup.h
# End Source File
# Begin Source File

SOURCE=.\CalcDisplacement.h
# End Source File
# Begin Source File

SOURCE=.\CalcFatigue.h
# End Source File
# Begin Source File

SOURCE=.\CalcFloorSlabStress.h
# End Source File
# Begin Source File

SOURCE=.\CalcFrameComboLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcInfluenceK.h
# End Source File
# Begin Source File

SOURCE=.\CalcLossOfPrestress.h
# End Source File
# Begin Source File

SOURCE=.\CalcModelingFramePropty.h
# End Source File
# Begin Source File

SOURCE=.\CalcModelingResult.h
# End Source File
# Begin Source File

SOURCE=.\CalcModelingSetup.h
# End Source File
# Begin Source File

SOURCE=.\CalcSecForceDivCalc.h
# End Source File
# Begin Source File

SOURCE=.\CalcSecForceDivResult.h
# End Source File
# Begin Source File

SOURCE=.\CalcSecOutline.h
# End Source File
# Begin Source File

SOURCE=.\CalcSecStress.h
# End Source File
# Begin Source File

SOURCE=.\CalcSecStressPos.h
# End Source File
# Begin Source File

SOURCE=.\CalcSectionConceptDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcUltimateLoadDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcWarping.h
# End Source File
# Begin Source File

SOURCE=.\CalcWeld.h
# End Source File
# Begin Source File

SOURCE=.\CalcWindForce.h
# End Source File
# Begin Source File

SOURCE=.\ColorListBox.h
# End Source File
# Begin Source File

SOURCE=.\CrossBeamSubInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\DanmyunTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckBasicDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckBindConcRebarDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckBMOptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckCrossStringerDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckDetailCyclesuDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckDetailDivideRebar.h
# End Source File
# Begin Source File

SOURCE=.\DeckDetailMainLowerCenterDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckDetailMainLowerDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckDetailMainUpperDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckDetailSettlementDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckDistriRebarCTCDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckDistriRebarDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckEtcRebarDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckGuardFenceDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckMainRebarCTCDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckMainRebarDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckMainSkewRebarCTCDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckPlanRebarDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckRebarInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckRebarJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeckSabogangCTCDlg.h
# End Source File
# Begin Source File

SOURCE=.\DesignBaseDlg.h
# End Source File
# Begin Source File

SOURCE=.\DesignBMPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignBoltPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignConcretePage.h
# End Source File
# Begin Source File

SOURCE=.\DesignCSteelPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignCTSteelPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignDeductPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignFrictionCoeffPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignGradePage.h
# End Source File
# Begin Source File

SOURCE=.\DesignHSteelPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignISteelPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignLSteelPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignLVarSteelPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignMaterialPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignPageDlg.h
# End Source File
# Begin Source File

SOURCE=.\DesignPrestressPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignPrestressPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignScallopPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignSpecialMovingLoad.h
# End Source File
# Begin Source File

SOURCE=.\DesignSteelDimPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignSteelMaterialPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignStudPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignUWeightPage.h
# End Source File
# Begin Source File

SOURCE=.\DesignWeldPage.h
# End Source File
# Begin Source File

SOURCE=.\DetBracketDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetBracketStiffDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetCamberJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetCrossBeamTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetDanTaperDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetEtcInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetFieldJoinDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetHBracing.h
# End Source File
# Begin Source File

SOURCE=.\DetHBracingScallop.h
# End Source File
# Begin Source File

SOURCE=.\DetHStiffDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetLiftLugDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetPlateWShiftDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetSHConnectorVPosDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetSlabAnchor.h
# End Source File
# Begin Source File

SOURCE=.\DetSolePlateDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetStagingRingDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetVBracingTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetVStiffJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetWebHStiffPosDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialogInstallBeryukAtBlockout.h
# End Source File
# Begin Source File

SOURCE=.\DialogInstallMainAtBlockout.h
# End Source File
# Begin Source File

SOURCE=.\EarthQuackDlg.h
# End Source File
# Begin Source File

SOURCE=.\EarthQuackFactDlg.h
# End Source File
# Begin Source File

SOURCE=.\EarthQUackGradeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExpansionJointTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\FactJoinPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\FieldJoinPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\GeneralBaseDlg.h
# End Source File
# Begin Source File

SOURCE=.\GeneralBaseStd.h
# End Source File
# Begin Source File

SOURCE=.\GeneralInput.h
# End Source File
# Begin Source File

SOURCE=.\GirHeightUserDlg.h
# End Source File
# Begin Source File

SOURCE=.\GuardFenceTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\HBracingPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\HBrSpStdInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\HDanDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImageListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\JackupPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\JackupSolePlateDlg.h
# End Source File
# Begin Source File

SOURCE=.\JiganGusengDlg.h
# End Source File
# Begin Source File

SOURCE=.\JijumAngleDlg.h
# End Source File
# Begin Source File

SOURCE=.\JijumStiffDlg.h
# End Source File
# Begin Source File

SOURCE=.\JointRestraintDlg.h
# End Source File
# Begin Source File

SOURCE=.\JRibPosition.h
# End Source File
# Begin Source File

SOURCE=.\labelcontrol.h
# End Source File
# Begin Source File

SOURCE=.\PaintOutAssembly.h
# End Source File
# Begin Source File

SOURCE=.\PaintOutSheet.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\PlateStressOut.h
# End Source File
# Begin Source File

SOURCE=.\RebarAddRateDlg.h
# End Source File
# Begin Source File

SOURCE=.\RebarTable.h
# End Source File
# Begin Source File

SOURCE=.\ReDir.h
# End Source File
# Begin Source File

SOURCE=.\Redirect.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\RistModule.h
# End Source File
# Begin Source File

SOURCE=.\SelectLineDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelGirderHeightRangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShCheckButton.h
# End Source File
# Begin Source File

SOURCE=.\ShearBarTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShellFileOp.h
# End Source File
# Begin Source File

SOURCE=.\ShoeSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShoeZapyoDlg.h
# End Source File
# Begin Source File

SOURCE=.\SlabJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\SlabStructureDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpliceBoltPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpliceDesignResult.h
# End Source File
# Begin Source File

SOURCE=.\StackDialog.h
# End Source File
# Begin Source File

SOURCE=.\StartPointJewonDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SubCrossHeightRistDlg.h
# End Source File
# Begin Source File

SOURCE=.\SubInputTensionBarDlg.h
# End Source File
# Begin Source File

SOURCE=.\SubRistRefImgDlg.h
# End Source File
# Begin Source File

SOURCE=.\SubSlabHunchInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\TotalViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserDesignCheckDlg.h
# End Source File
# Begin Source File

SOURCE=.\VBracingPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\VStiffPositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\VStiffStdInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\WeldOutAssembly.h
# End Source File
# Begin Source File

SOURCE=.\WeldOutSheet.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_apply.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_bodo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_bracingtype.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_byuntype1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_byuntype2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_byuntype3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_byuntype4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_guardrail.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_guardrail1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_h.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_hBracing.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_hisNext.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_hisNextNo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_hisPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_hisPrevNo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_install_beryuk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_install_main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_j.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_k.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_next.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_nextall.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_pin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_pin1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_pos.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_prev.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_prevall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_r_big.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_recommand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_treebar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_uninstall_beryuk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_uninstall_main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_w.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7230.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7231.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7232.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7233.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7234.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7235.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7236.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7237.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7238.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7239.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7240.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7241.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7242.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7243.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_FactJoint.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_GirderHeight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_GirderRate.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_HBracing.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_JiJumAngle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_RIST.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_SttEndI1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_SttEndI2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp_SttEndI3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bodo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ByunType1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checkoff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checkon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CompressStress.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GeneralInput.rc2
# End Source File
# Begin Source File

SOURCE=.\res\girht1_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\girht2_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\girht3_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\girht4_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\girhtra1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\girhtra2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\girhtra3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\girhtra4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Guard1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Guard2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Guard3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Guard4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Guard5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Guard6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Guard7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\guardrai.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShearBar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShearBar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShearBar3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShearButtonD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShearButtonU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShearStress1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShearStress2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tank.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TensionStress.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\trailer.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TREE_MENU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treebar_.bmp
# End Source File
# Begin Source File

SOURCE=".\res\가까운헌치부.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\기역배치-dn.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\기역배치.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\먼헌치부.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\분리배치.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\상단부정착장.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\전체배치.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\중앙부배치.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\중앙부배치01.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\중앙부배치02.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\직선배치.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\하단부정착장.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
