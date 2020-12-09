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

#include "DetSolePlateDlg.h"		// 21	// ���Է�
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
#include "BMOutSangPage.h"			// ���ǥ
#include "BMOutSheet.h"				// ���ǥ
#include "PaintOutSheet.h"			// ���ǥ
#include "PaintOutAssembly.h"		// ���ǥ
#include "WeldOutSheet.h"			// ���ǥ
#include "WeldOutAssembly.h"		// ���ǥ
#include "ShoeZapyoDlg.h"
#include "DeckBasicDataDlg.h"		// 1. ���ʼ���
#include "DeckRebarInfoDlg.h"		// 2. �������
#include "DeckMainRebarDlg.h"		// 3. ��ö�� ��ġ
#include "DeckDistriRebarCTCDlg.h"	// 4. ���ö�� ����
#include "DeckDistriRebarDlg.h"		// 5. ���ö�� ��ġ
#include "DeckMainRebarCTCDlg.h"	// 6.0 ��ö�� ����
#include "DeckMainSkewRebarCTCDlg.h"// 6.1 �簢����ö�� ����
#include "DeckSabogangCTCDlg.h"		// 6.2 �纸��ö�� ����
#include "DeckBindConcRebarDlg.h"	// 6.3 ������ũ��Ʈ ���
#include "DeckCrossStringerDlg.h"	// 6.5 ���κ�,���κ�����ö��
#include "DeckGuardFenceDlg.h"
#include "DeckPlanRebarDlg.h"		// 7. ��ü���
#include "DeckEtcRebarDlg.h"		// 8. ��Ÿö�ٹ�ġ
#include "DeckRebarJewonDlg.h"		// 9. ö������	
#include "DeckBMOptionDlg.h"		// 10. ���� ��� �ɼ�
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