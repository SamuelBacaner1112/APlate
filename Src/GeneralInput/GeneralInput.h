// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#ifndef _GENERALINPUT_H_
#define _GENERALINPUT_H_

#include "resource.h"
#include "GeneralBaseDlg.h"
#include "SelGirderHeightRangeDlg.h"

#include "JointRestraintDlg.h"
#include "UserDesignCheckDlg.h"
#include "DesignPageDlg.h"			// 0
#include "SelectLineDlg.h"			// 1
#include "BaseInfoDlg.h"			// 2
#include "HDanDlg.h"				// 3
#include "JiganGusengDlg.h"			// 5
#include "JijumAngleDlg.h"			// 6
#include "AbutPierDlg.h"			// 7
#include "JackupPositionDlg.h"		// 8
#include "JijumStiffDlg.h"			// 9
#include "JackupSolePlateDlg.h"		// 10
#include "StartPointJewonDlg.h"		// 11
#include "ByunDanmyunJewonDlg.h"	// 12
#include "VBracingPositionDlg.h"	// 13
#include "VStiffPositionDlg.h"		// 14
#include "HBracingPositionDlg.h"	// 15
#include "JRibPosition.h"			// 16
#include "DanmyunTypeDlg.h"			// 17
#include "FieldJoinPositionDlg.h"	// 18
#include "FactJoinPositionDlg.h"	// 19
#include "SlabJewonDlg.h"			// 20

#include "DetSolePlateDlg.h"		// 21	// 상세입력
#include "DetHBracingScallop.h"	    // 24
#include "DetHStiffDlg.h"			// 25
#include "DetDanTaperDlg.h"			// 26
#include "DetPlateWShiftDlg.h"		// 27
#include "DetStagingRingDlg.h"		// 28
#include "DetSlabAnchor.h"          // 29
#include "DetBracketDlg.h"			// 30
#include "DetBracketStiffDlg.h"		// 31
#include "DetLiftLugDlg.h"			// 32
#include "DetEtcInputDlg.h"

#include "DetFieldJoinDlg.h"		// 33
#include "DetCrossBeamTypeDlg.h"		// 34
#include "DetVBracingTypeDlg.h"		// 36

#include "DetHBracing.h"			// 39
#include "DetVStiffJewonDlg.h"		// 40
#include "DetCamberJewonDlg.h"		// 41
#include "DetWebHStiffPosDlg.h"		// 42
#include "DetSHConnectorVPosDlg.h"	// 44

#include "CalcCantileverModeling.h"
#include "CalcCantileverLoadCombo.h"
#include "CalcCantileverModelResult.h"
#include "DesignBaseDlg.h"
#include "CalcCantileverSetup.h"
#include "CalcCantileverDeadLoad.h"	// 46
#include "CalcCantileverLoadDlg.h"	// 71
#include "CalcLossOfPrestress.h"
#include "CalcFloorSlabStress.h"
#include "CalcCantileverRebarDlg.h"	// 47
#include "CalcUltimateLoadDlg.h"
#include "CalcCantileverDroop.h"	// 48
#include "CalcSectionConceptDlg.h"
#include "CalcSecForceDivCalc.h"	// 52
#include "CalcSecForceDivResult.h"  // 53
#include "CalcWindForce.h"			// 54
#include "CalcModelingSetup.h"		// 55
#include "CalcFrameComboLoadDlg.h"
#include "CalcModelingFramePropty.h"// 56
#include "CalcModelingResult.h"		// 57 
#include "CalcInfluenceK.h"			// 58   
#include "CalcSecStress.h"			// 59  
#include "CalcSecOutline.h"			// 60
#include "CalcWarping.h"	
#include "CalcDisplacement.h"
#include "CalcFatigue.h"    
#include "CalcWeld.h"		
#include "BMOutSangPage.h"			// 재료표
#include "BMOutSheet.h"				// 재료표
#include "PaintOutSheet.h"			// 재료표
#include "PaintOutAssembly.h"		// 재료표
#include "WeldOutSheet.h"			// 재료표
#include "WeldOutAssembly.h"		// 재료표
#include "ShoeZapyoDlg.h"
#include "DeckBasicDataDlg.h"		// 1. 기초설정
#include "DeckRebarInfoDlg.h"		// 2. 배근정보
#include "DeckMainRebarDlg.h"		// 3. 주철근 배치
#include "DeckDistriRebarCTCDlg.h"	// 4. 배력철근 간격
#include "DeckDistriRebarDlg.h"		// 5. 배력철근 배치
#include "DeckMainRebarCTCDlg.h"	// 6.0 주철근 간격
#include "DeckMainSkewRebarCTCDlg.h"// 6.1 사각부주철근 간격
#include "DeckSabogangCTCDlg.h"		// 6.2 사보강철근 간격
#include "DeckBindConcRebarDlg.h"	// 6.3 구속콘크리트 배근
#include "DeckCrossStringerDlg.h"	// 6.5 가로보,세로보보강철근
#include "DeckGuardFenceDlg.h"
#include "DeckPlanRebarDlg.h"		// 7. 전체배근
#include "DeckEtcRebarDlg.h"		// 8. 기타철근배치
#include "DeckRebarJewonDlg.h"		// 9. 철근제원	
#include "DeckBMOptionDlg.h"		// 10. 수량 출력 옵션
#include "CalcSecStressPos.h"
#include "SpliceDesignResult.h"
#include "SpliceBoltPositionDlg.h"
#include "PlateStressOut.h"
#include "VStiffStdInputDlg.h"
#include "HBrSpStdInputDlg.h"
#include "EarthQUackGradeDlg.h"
#include "EarthQuackDlg.h"
#include "EarthQuackFactDlg.h"
#include "GirHeightUserDlg.h"

#include "BindConcJewonDlg.h"
#include "GeneralBaseStd.h"
#endif