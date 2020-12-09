// PlateBridge.cpp: implementation of the CPlateBridge class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "APlateData.h"
#include "PlateBridge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
///////////////////////////////////////////////////////////////////////
// class CPlateBridge
//<summary>
//생성자
//</summary>
//<return></return>
CPlateBridge::CPlateBridge(CLineInfo* pLine)
{
	m_pLine = pLine;

	m_pCRGirder = new CPlateGirderApp(this,pLine,NULL);	
	
	m_PierInfo = new CPierInfo[JIJUMSUMAX];	

	m_sNameBridge					= _T("");
	m_sNameOwner					= _T("");
	m_sNameConstructor				= _T("");
	m_sNameAssembler				= _T("");
	m_sNameSite						= _T("");
	m_sNameEngineering				= _T("");
	m_NumHaiseokCombo				= 0;	//Issue Number: 27196 - 합성형으로 고정함.
	m_bUseLine			            = TRUE;
	m_dBridgeWidth					= 0;
	m_dDisLineToBridgeCen	        = 0;
	m_dGirderSpace			        = 0;
	m_nLineType                     = 0; 

	m_sDeungeub						= _T("");
	m_nTypeAbutOrPier[0]			= 0;
	m_nTypeAbutOrPier[1]			= 0;
	m_dStationBridgeStt				= 0;
	m_dLengthTotalSlab				= 0;
	m_nQtyJigan						= 0;
	m_dWidthSlabLeft				= 0;
	m_dWidthSlabRight				= 0;
	
	m_dRoundCrossUpper				= 0;
	m_dRoundChipCrossUpper			= 0;
	m_dRoundCrossLower				= 0;
	m_dRoundChipCrossLower			= 0;
	m_dRoundStringerUpper			= 0;
	m_dRoundChipStringerUpper		= 0;
	m_dRoundStringerLower			= 0;
	m_dRoundChipStringerLower		= 0;

	m_cBracketForm					= 0;
	m_cBracketType					= 0;
	m_bLeftBracket					= 0;
	m_bRightBracket					= 0;
	m_cBrBeamType					= 0;
	m_bBracketBeamSpliceApply		= 0;
	m_dRoundBracketUpper			= 0;
	m_dRoundChipBracketUpper		= 0;
	m_dRoundBracketLower			= 0;
	m_dRoundChipBracketLower		= 0;

	m_dLengthUganStt				= 0;
	m_dLengthSlabGirderStt			= 0;
	m_dLengthGirderShoeStt			= 0;
	m_dLengthGirderShoeEnd			= 0;
	m_dLengthSlabGirderEnd			= 0;
	m_dLengthUganEnd				= 0;

	m_cOffSetStt					= 1;
	m_cOffSetEnd					= 1;
	m_dDisExtLineSttLeft			= 0;
	m_dDisExtLineEndLeft			= 0;
	m_dDisExtLineSttRight			= 0;
	m_dDisExtLineEndRight			= 0;
	m_cDirShoe						= 0;
	m_cDirVStiffJijum				= 0;

	m_dLengthShoeToTaperStt			= 0;
	m_dLengthTaperToTaperStt		= 0;
	m_dHeightTaperStt				= 0;
	m_dLengthShoeToTaperEnd			= 0;
	m_dLengthTaperToTaperEnd		= 0;
	m_dHeightTaperEnd				= 0;
	m_HTaper_W						= 2.5;
	m_HTaper_H						= 1;

	m_dHeightInOutStt				= 0;
	m_dWidthInOutStt				= 0;
	m_dRadiusInOutStt				= 0;
	m_dHeightInOutLowerStt			= 0;
	m_dHeightInOutEnd				= 0;
	m_dWidthInOutEnd				= 0;
	m_dRadiusInOutEnd				= 0;
	m_dHeightInOutLowerEnd			= 0;

	m_cTaperAngleTypeStt			= 0;
	m_cTaperAngleTypeEnd			= 0;

	m_cFacePlateRoundType			= 0;
	m_cFacePlateUse					= 0;
	m_cFaceManhole					= 0;

	m_cByunDan						= 0;

	m_dLengthBaseCrossBeam			= 0;

	m_dDisCenterOnCrBeam			= 0;
	m_dDisCenterOnVst				= 0;
	m_dDisCenterOnJRib				= 0;
	m_dDisCenterOnSplice			= 0;
	m_dDisCenterOnFact				= 0;
	m_dDisCenterOnCamber			= 0;
	m_dDisCenterOnStud				= 0;
	m_dDisCenterOnHStiff			= 0;

	m_nLengthTypeOnCrBeam			= 1;
	m_nLengthTypeOnVst				= 1;
	m_nLengthTypeOnJRib				= 1;
	m_nLengthTypeOnSplice			= 1;
	m_nLengthTypeOnFact				= 1;
	m_nLengthTypeOnCamber			= 1;
	m_nLengthTypeOnStud				= 1;
	m_nLengthTypeOnHStiff			= 1;

	m_dMoveTermOnSplice				= 0;

	m_bInputAngleVStiff				= 0;
	m_nCrossArrangeType				= 0;
	m_dLengthBaseVStiff				= 0;
	m_nHStiffDefault				= 0;
	m_cHStiffDimRound				= 0;
	m_cHStiffDimRoundValue			= 0;
	m_dHStiffLengthMin				= 0;
	m_dHStiffCheckLength			= 1250.0;
	m_dHStiffTRibApply				= 0;
	m_cHStiffHeightOffset			= 0;
	m_dHStiffHeight1				= 0;
	m_dHStiffHeight2				= 0;
	m_dHStiffHeight3				= 0;
	m_dHStiffMinLenJackup			= 0;
	m_cHStiffJackupVStiff			= 0;
	m_cFactThickFix					= 0;
	m_dDisOnCenterFactJoin			= 0;
	m_cSpliceSimpleType				= 0;

	// [슬래브 제원]
	m_dThickSlabLeft				= 0;
	m_dThickSlabBase				= 250;
	m_dThickSlabRight				= 0;
	m_dThickSlabHunch				= 0;
	m_dThickPave					= 80;
	m_dWidthHunch					= 0;
	m_dHunchDisForBaseThick			= 0;
	m_cLevelRoadCenterToSlabEdge	= 1;
	m_dHeightDosang					= 0;
	m_dSlopHunch					= 0;
	m_dMortalProtected				= 0;
	m_dThickSlabLeftExt				= 300;
	m_dThickSlabRightExt			= 300;

	m_cCamberUnitToMM				= 0;

	m_cSameApplySameColCross		= 1;
	m_cSameApplySameColVStiff		= 1;
	m_cSameApplySameColJRib			= 1;
	m_cSameApplySameColDanmyun		= 1;
	m_cSameApplySameColSplice		= 1;
	m_cSameApplySameColFactJoin		= 1;
	m_cSameApplySameColOthers		= 1;
	m_cSameApplySameColHStiff		= 1;
	m_cSameApplySameColStud			= 1;

	m_cFactoryType					= 0;
	m_dMaxFactoryValue				= 15;
	m_dRateFactoryValue				= 30;
	long n = 0;
	for(n = 0; n < KYUNGGANSUMAX; n++)
	{
		m_dLengthJigan[n]	= 0;
		m_nStyleByun[n]		= 0;
		m_nByunCurveType[n]	= 0;
		m_dHeightByun[n]	= 0;
		m_dLenLeftByun[n]	= 0;
		m_dLenRightByun[n]	= 0;
		m_dLenLeftCurve[n]	= 0;
		m_dLenRightCurve[n]	= 0;
	}
	for(n = 0; n < JIJUMSUMAX; n++)
	{
		m_strJijumName[n] = _T("");
		m_vAngJijum[n]  = CDPoint(0,1);
		long m = 0;
		for(m = 0; m < JACKUPSUMAX; m++)
		{
			m_dJiJumTerm[n][m] = 0;
			m_dJackUpTerm[n][m] = 0;
		}
		for(m = 0; m < 2; m++)
		{	
			m_structJackupJewon[n][m].m_dL = 0;
			m_structJackupJewon[n][m].m_dD = 0;
			m_structJackupJewon[n][m].m_dH = 0;
			m_structJackupJewon[n][m].m_dW = 0;
			m_structJackupJewon[n][m].m_dT = 0;
		}
		m_nQtyJijumStiff[n] = 0;
	}
	m_BindConc.m_dWidth		= 0;
	m_BindConc.m_dHeight	= 0;
	m_BindConc.m_dDeep		= 0;
	m_BindConc.m_dDis		= 0;
	
	m_dSlotA[0] = 70;
	m_dSlotB[0] = 50;
	m_dSlotR[0] = 35;
	m_dSlotR2[0] = 20;
	m_dSlotA[1] = 80;
	m_dSlotB[1] = 55;
	m_dSlotR[1] = 35;
	m_dSlotR2[1] = 20;
	m_dSlotA[2] = 90;
	m_dSlotB[2] = 65;
	m_dSlotR[2] = 35;
	m_dSlotR2[2] = 20;
	m_bApplyDefaultSlot = FALSE;
	
	m_FieldScallop_BaseT			= 25.0;	//현장 용접 기준 t.......................070206...추가...KB...
	m_FieldScallop_R1				= 40.0;	//현장 용접 시 복부판t < 기준t
	m_FieldScallop_R2				= 50.0;	//현장 용접 시 복부판t >= 기준t

	for(n=0; n<3; n++)
	{
		for(int i=0; i<2; i++)
		{
			m_nGussetStiff_Gen[n][i] = 0;
			m_dGS_T[n][i] = 10;		
			m_dGS_S3[n][i] = 30;
			m_dGS_W[n][i] = 120;
			m_dGS_dA[n][i] = 90;
			m_dGS_Gen_dB[n][i] = 30;
			m_dGS_Gen_dC[n][i] = 30;
			m_dGS_Trap_dB[n][i] = 30;
			m_dGS_Trap_dC[n][i] = 25;
			m_dGS_uA[n][i] = 90;
			m_dGS_uB[n][i] = 30;
			m_dGS_uC[n][i] = 30;
			m_dGS_Sub_sT[n][i] = 10;
			m_dGS_Sub_uD[n][i] = 30;
			m_dGS_Sub_dD[n][i] = 30;
			m_dGS_Sub_uE[n][i] = 200;
			m_dGS_Sub_dE[n][i] = 200;
		}
		m_dGS_S1[n][0] = 10;
		m_dGS_S2[n][0] = 0;
		m_dGS_S1[n][1] = 30;
		m_dGS_S2[n][1] = 30;

		m_dGS_Bolt_B[n] = 20;
		m_dGS_Bolt_H[n] = 20;
		m_dGS_Bolt_R[n] = 20;
	}
	m_nQtyHDanNode					= 0;
	for(n = 0; n < HDANLINESU; n++)
	{
		m_dLengthHDan[n] = 0;  
		for(long m = 0; m < 3;m++)
			m_nValueTypeHDan[n][m] = 0;   
	}

	m_dA_p = 0; // 일반부
	m_dB_p = 0;
	m_dC_p = 0;
	m_dT_p = 0;
	m_dC_n = 0; // 부모멘트부
	m_dT_n = 0;
	m_nInstallVStiff            = 0;
	m_dHBuSL = 80;
	m_dHBuGw = 20;
	m_dHBuSA = 40; 
	m_dHBuSB = 2;
	m_dHBuSC = 80;
	m_dHBuSD = 40;
	m_dHBuSE = 1;
	m_dHBuW  = 350;
	m_dHBuW1 = 100;
	m_szHBrSpBoltDia = "M22";
	m_dHBuStep1 = 50;
	m_dHBuStep2 = 20;
	m_dHBuStep3 = 20;
	m_dHBuStep4 = 20;
	m_dHBuStep5 = 120;
	m_nInstallVStiffOnCrossBeam = 0;
	m_bTendonInputComplete      = FALSE;
	m_bUserSlabHunch            = FALSE;

	for(n=0; n<MAX_TENSION; n++)
	{
		m_SlabTension[n].m_dL = 0;
		m_SlabTension[n].m_dH = 0;
		m_SlabTension[n].m_dR = 0;
		m_SlabTension[n].m_nType = 0;
	}
	m_dHeightLeftTension  = 0;
	m_dHeightRightTension = 0;
	m_dLengthGinjangdan   = 0;
	m_dLengthJungchakdan  = 0;
	m_nSlabTensionSize    = 0;
//	m_nBridgeType         = 0;

	m_dLengthSecMove	  = 0;
	m_dLengthSecMoveEnd	  = 0;
	m_dLengthDuctOffset   = 0;

	m_bUseJackupSolePlate = TRUE;
	m_bApplyHunchAtCB	  = TRUE;	
	m_nTendonFix          = 0;
	m_nTendonDomFix		  = 2;	// 지그재그타입
	m_dSameFlangeWidth    = 400;
	m_cSlabThickAddTypeStt= 0;	// 0 : 없음 1 : 두번째 가로보까지 2 : 단부 이격거리
	m_cSlabThickAddTypeEnd= 0;	// 0 : 없음 1 : 두번째 가로보까지 2 : 단부 이격거리
	m_bQuickLineSearch	  = TRUE;

	m_dLengthTUBindConc_Stt	= 100;	// 구속콘크리트 시점부 돌출길이
	m_dLengthTUBindConc_End	= 100;	// 구속콘크리트 종점부 돌출길이

	m_dGirEndStndGuttingAngle	= 70;		// 거더단부 모따기 기준각도 

	m_nBaseHeightPosHBraceSplice = 1;
	m_dBaseHeightFixPosHBraceSplice = 270;
	m_dBaseHeightRatioPosHBraceSplice = 0;
}
//<summary>
//소멸자
//</summary>
//<return></return> 
CPlateBridge::~CPlateBridge()
{
	for(long nG=0;nG< GetGirdersu();nG++ ) delete m_pGirder[nG]; m_pGirder.RemoveAll();
	AhTPADelete(&m_pGuardWall, (CCentSeparation*)0);

	delete m_pCRGirder;	
	delete[] m_PierInfo;
}


//<summary>
//매개변수 저장
//</summary>
//<param name="ar"></param>
void CPlateBridge::Serialize(CArchive& ar)
{
//	long nFlag    = 9;	// 071212 lina, 1214
//	long nFlag    = 10; // 080617 lina - 슬래브헌치 길이변수 추가. 기존에 안쓰던 변수명 고쳐서 사용함..
//	long nFlag    = 11; // 080617 lina - 슬래브헌치 길이변수 -> 구배변수로 변경
//	long nFlag	  = 12;	// 081121 lina - 수평브레이싱 거세트위치 설정변수 추가
//	long nFlag	  = 13; // 090617 APlate 2009 작업
	long nFlag		= 14;	// 110502 HDANLINESU 30->50 #24997
	long Girdersu = 0;
	long vLong    = 0;
	BYTE vBYTE    = 0;
	double vDouble = 0;

	if(ar.IsStoring())
	{
		Girdersu = GetGirdersu();
		ar << nFlag;		
		ar << Girdersu;														
		for(long nG = -1; nG<Girdersu; nG++)														
			GetGirder(nG)->Serialize(ar);														

		AhTPASerial(ar,&m_pGuardWall,(CCentSeparation*)0);	
		long nGuardSu = GetGuardWallSu();
		ar << nGuardSu;
		for(long i=0; i<nGuardSu; i++)
		{
			CCentSeparation *pGW = m_pGuardWall.GetAt(i);
			ar << pGW->m_nDirType;
		}

		ar << m_sNameBridge;
		ar << m_sNameOwner;
		ar << m_sNameConstructor;
		ar << m_sNameAssembler;
		ar << m_sNameSite;
		ar << m_sNameEngineering;		
		ar << m_NumHaiseokCombo;			
		ar << m_bUseLine;
		ar << m_dBridgeWidth;
		ar << m_dDisLineToBridgeCen;
		ar << m_dGirderSpace;		
		ar << m_nLineType;

		ar << m_sDeungeub;				
		ar << m_nTypeAbutOrPier[0];		
		ar << m_nTypeAbutOrPier[1];		
		ar << m_dStationBridgeStt;		
		ar << m_dLengthTotalSlab;		
		ar << m_nQtyJigan;				
		ar << m_dWidthSlabLeft;			
		ar << m_dWidthSlabRight;			
		ar << m_dHunchArray;
		ar << m_arrTendon;
		ar << m_nTendonFix;
		ar << m_nTendonDomFix;
		ar << m_nSlabTensionSize;
		long n = 0;
		for(n=0; n<m_nSlabTensionSize; n++)
		{
			ar << m_SlabTension[n].m_dL;
			ar << m_SlabTension[n].m_dH;
			ar << m_SlabTension[n].m_dR;
			ar << m_SlabTension[n].m_nType;
		}
		ar << m_dHeightLeftTension;
		ar << m_dHeightRightTension;

		ar << m_dRoundCrossUpper;		
		ar << m_dRoundChipCrossUpper;	
		ar << m_dRoundCrossLower;		
		ar << m_dRoundChipCrossLower;	
		ar << m_dRoundStringerUpper;		
		ar << m_dRoundChipStringerUpper;	
		ar << m_dRoundStringerLower;		
		ar << m_dRoundChipStringerLower;	

		ar << m_cBracketForm;			
		ar << m_cBracketType;			
		ar << m_bLeftBracket;			
		ar << m_bRightBracket;			
		ar << m_cBrBeamType;			
		ar << m_bBracketBeamSpliceApply;	
		ar << m_dRoundBracketUpper;
		ar << m_dRoundChipBracketUpper;	
		ar << m_dRoundBracketLower;		
		ar << m_dRoundChipBracketLower;	

		ar << m_dLengthUganStt;			
		ar << m_dLengthSlabGirderStt;	
		ar << m_dLengthGirderShoeStt;	
		ar << m_dLengthGirderShoeEnd;	
		ar << m_dLengthSlabGirderEnd;	
		ar << m_dLengthUganEnd;			

		ar << m_cOffSetStt;				
		ar << m_cOffSetEnd;				
		ar << m_dDisExtLineSttLeft;		
		ar << m_dDisExtLineEndLeft;		
		ar << m_dDisExtLineSttRight;		
		ar << m_dDisExtLineEndRight;	
		ar << m_cDirShoe;				
		ar << m_cDirVStiffJijum;			

		ar << m_dLengthShoeToTaperStt;	
		ar << m_dLengthTaperToTaperStt;	
		ar << m_dHeightTaperStt;			
		ar << m_dLengthShoeToTaperEnd;	
		ar << m_dLengthTaperToTaperEnd;	
		ar << m_dHeightTaperEnd;			
		ar << m_HTaper_W;				
		ar << m_HTaper_H;				

		ar << m_dHeightInOutStt;			
		ar << m_dWidthInOutStt;			
		ar << m_dRadiusInOutStt;			
		ar << m_dHeightInOutLowerStt;	
		ar << m_dHeightInOutEnd;			
		ar << m_dWidthInOutEnd;			
		ar << m_dRadiusInOutEnd;			
		ar << m_dHeightInOutLowerEnd;	

		ar << m_cTaperAngleTypeStt;		
		ar << m_cTaperAngleTypeEnd;		

		ar << m_cFacePlateRoundType;		
		ar << m_cFacePlateUse;
		ar << m_cFaceManhole;
		
		ar << m_cByunDan;	
		ar << m_CrossAngleGrid;
		ar << m_VStiffGrid;

		ar << m_dLengthBaseCrossBeam;	

		ar << m_dDisCenterOnCrBeam;		
		ar << m_dDisCenterOnVst;			
		ar << m_dDisCenterOnJRib;		
		ar << m_dDisCenterOnSplice;		
		ar << m_dDisCenterOnFact;		
		ar << m_dDisCenterOnCamber;		
		ar << m_dDisCenterOnStud;		
		ar << m_dDisCenterOnHStiff;		

		ar << m_nLengthTypeOnCrBeam;		
		ar << m_nLengthTypeOnVst;		
		ar << m_nLengthTypeOnJRib;		
		ar << m_nLengthTypeOnSplice;		
		ar << m_nLengthTypeOnFact;		
		ar << m_nLengthTypeOnCamber;		
		ar << m_nLengthTypeOnStud;		
		ar << m_nLengthTypeOnHStiff;		

		ar << m_dMoveTermOnSplice;		

		ar << m_bInputAngleVStiff;			
		ar << m_nCrossArrangeType;			
		ar << m_dLengthBaseVStiff;			
		ar << m_nHStiffDefault;				
		ar << m_cHStiffDimRound;				
		ar << m_cHStiffDimRoundValue;		
		ar << m_dHStiffLengthMin;
		ar << m_dHStiffCheckLength; //07.01.03 수평보강재 교번구간 체크 길이 추가...KB..
		ar << m_dHStiffTRibApply;			
		ar << m_cHStiffHeightOffset;			
		ar << m_dHStiffHeight1;				
		ar << m_dHStiffHeight2;				
		ar << m_dHStiffHeight3;				
		ar << m_dHStiffMinLenJackup;			
		ar << m_cHStiffJackupVStiff;		
		ar << m_cFactThickFix;				
		ar << m_dDisOnCenterFactJoin;		
		ar << m_cSpliceSimpleType;			

		ar << m_dThickSlabLeft;				
		ar << m_dThickSlabBase;				
		ar << m_dThickSlabRight;				
		ar << m_dThickSlabHunch;			
		ar << m_dThickPave;					
		ar << m_dWidthHunch;				
		ar << m_dHunchDisForBaseThick;		
		ar << m_cLevelRoadCenterToSlabEdge;	
		ar << m_dHeightDosang;				
		ar << m_dSlopHunch;				
		ar << m_dMortalProtected;			
		ar << m_dThickSlabLeftExt;
		ar << m_dThickSlabRightExt;

		ar << m_cCamberUnitToMM;				

		ar << m_cSameApplySameColCross;		
		ar << m_cSameApplySameColVStiff;		
		ar << m_cSameApplySameColJRib;		
		ar << m_cSameApplySameColDanmyun;	
		ar << m_cSameApplySameColSplice;		
		ar << m_cSameApplySameColFactJoin;	
		ar << m_cSameApplySameColOthers;		
		ar << m_cSameApplySameColHStiff;		
		ar << m_cSameApplySameColStud;		

		ar << m_cFactoryType;				
		ar << m_dMaxFactoryValue;			
		ar << m_dRateFactoryValue;			

		for(n = 0; n < m_nQtyJigan; n++)
		{
			ar << m_dLengthJigan[n];	
			ar << m_nStyleByun[n];	
			ar << m_nByunCurveType[n];
			ar << m_dHeightByun[n];	
			ar << m_dLenLeftByun[n];	
			ar << m_dLenRightByun[n];
			ar << m_dLenLeftCurve[n];
			ar << m_dLenRightCurve[n];
		}
		for(n = 0; n < m_nQtyJigan+1; n++)
		{
			ar << m_strJijumName[n];
			ar << m_vAngJijum[n];
			long m = 0;
			for(m = 0; m < JACKUPSUMAX; m++)
			{
				ar << m_dJiJumTerm[n][m];
				ar << m_dJackUpTerm[n][m];
			}
			for(m = 0; m < 2; m++)
			{	
				ar << m_structJackupJewon[n][m].m_dL;
				ar << m_structJackupJewon[n][m].m_dD;
				ar << m_structJackupJewon[n][m].m_dH;
				ar << m_structJackupJewon[n][m].m_dW;
				ar << m_structJackupJewon[n][m].m_dT;
			}
		}
		for(n=0; n<JIJUMSUMAX; n++)
			ar << m_nQtyJijumStiff[n];
		ar << m_BindConc.m_dWidth;
		ar << m_BindConc.m_dHeight;
		ar << m_BindConc.m_dDeep;
		ar << m_BindConc.m_dDis;
		ar << m_nBaseHeightPosHBraceSplice;
		ar << m_dBaseHeightFixPosHBraceSplice;
		ar << m_dBaseHeightRatioPosHBraceSplice;

		ar << m_dSlotA[0];
		ar << m_dSlotB[0];
		ar << m_dSlotR[0];
		ar << m_dSlotA[1];
		ar << m_dSlotB[1];
		ar << m_dSlotR[1];
		ar << m_dSlotA[2];
		ar << m_dSlotB[2];
		ar << m_dSlotR[2];
		ar << m_dSlotR2[0];
		ar << m_dSlotR2[1];
		ar << m_dSlotR2[2];

		ar << m_bApplyDefaultSlot;

		ar << m_FieldScallop_BaseT				;
		ar << m_FieldScallop_R1					;
		ar << m_FieldScallop_R2					;

		for(n=0; n<3; n++)
		{
			for(int i=0; i<2; i++)
			{
				ar << m_nGussetStiff_Gen[n][i];
				ar << m_dGS_T[n][i];		
				ar << m_dGS_S3[n][i];
				ar << m_dGS_W[n][i];
				ar << m_dGS_dA[n][i];
				ar << m_dGS_Gen_dB[n][i];
				ar << m_dGS_Gen_dC[n][i];
				ar << m_dGS_Trap_dB[n][i];
				ar << m_dGS_Trap_dC[n][i];
				ar << m_dGS_uA[n][i];
				ar << m_dGS_uB[n][i];
				ar << m_dGS_uC[n][i];
			}
			ar << m_dGS_S1[n][0];
			ar << m_dGS_S2[n][0];
			ar << m_dGS_S1[n][1];
			ar << m_dGS_S2[n][1];

			ar << m_dGS_Bolt_B[n];
			ar << m_dGS_Bolt_H[n];
			ar << m_dGS_Bolt_R[n];


		}
		ar << m_nQtyHDanNode;
		for(n = 0; n < HDANLINESU; n++)
		{
			ar << m_dLengthHDan[n];
			for(long m = 0; m < 3;m++)
				ar << m_nValueTypeHDan[n][m];   
		}
		for(n = 0; n < m_nQtyJigan+1; n++)
			ar << m_PierInfo[n];		

		ar << m_dA_p; // 일반부
		ar << m_dB_p;
		ar << m_dC_p;
		ar << m_dT_p;
		ar << m_dC_n; // 부모멘트부
		ar << m_dT_n;
		ar << m_nInstallVStiff;

		ar << m_dHBuSL;
		ar << m_dHBuGw;
		ar << m_dHBuSA;
		ar << m_dHBuSB;
		ar << m_dHBuSC;
		ar << m_dHBuSD;
		ar << m_dHBuSE;
		ar << m_dHBuStep1;
		ar << m_dHBuStep2;
		ar << m_dHBuStep3;
		ar << m_dHBuStep4;
		ar << m_dHBuStep5;
		ar << m_dHBuW;
		ar << m_dHBuW1;
		ar << m_szHBrSpBoltDia;

		ar << m_nInstallVStiffOnCrossBeam;
		ar << m_bTendonInputComplete;

		ar << m_bUserSlabHunch;
		ar << vLong;//m_nBridgeType;
		ar << m_bUseJackupSolePlate;
		ar << m_bApplyHunchAtCB;
		ar << m_dSameFlangeWidth;
		ar << m_cSlabThickAddTypeStt;
		ar << m_cSlabThickAddTypeEnd;

		ar << m_dLengthGinjangdan;
		ar << m_dLengthJungchakdan;

		ar << m_dLengthSecMove;//강선 관련... 070208일자 추가..KB...
		ar << m_dLengthSecMoveEnd;
		ar << m_dLengthDuctOffset;

		for(n=0; n<3; n++)
		{
			for(long i=0; i<2; i++)
			{
				ar << m_dGS_Sub_sT[n][i];
				ar << m_dGS_Sub_uD[n][i];
				ar << m_dGS_Sub_dD[n][i];
				ar << m_dGS_Sub_uE[n][i];
				ar << m_dGS_Sub_dE[n][i];
			}
		}

		ar << m_bQuickLineSearch;

		ar << m_dLengthTUBindConc_Stt;
		ar << m_dLengthTUBindConc_End;
		ar << m_dGirEndStndGuttingAngle;
	}
	else
	{
		ar >> nFlag;
		ar >> Girdersu;
		MakeGirder(Girdersu);	// 거더 객체 생성
		for(long nG = -1; nG<Girdersu; nG++)
			GetGirder(nG)->Serialize(ar);

		AhTPASerial(ar,&m_pGuardWall,(CCentSeparation*)0);
		long nGuardSu;
		ar >> nGuardSu;
		for(long i=0; i<nGuardSu; i++)
		{
			CCentSeparation *pGW = m_pGuardWall.GetAt(i);
			ar >> pGW->m_nDirType;
		}		
		ar >> m_sNameBridge;
		ar >> m_sNameOwner;
		ar >> m_sNameConstructor;
		ar >> m_sNameAssembler;
		ar >> m_sNameSite;
		ar >> m_sNameEngineering;		
		ar >> m_NumHaiseokCombo;			
		ar >> m_bUseLine;
		ar >> m_dBridgeWidth;
		ar >> m_dDisLineToBridgeCen;
		ar >> m_dGirderSpace;		
		ar >> m_nLineType;

		ar >> m_sDeungeub;				
		ar >> m_nTypeAbutOrPier[0];		
		ar >> m_nTypeAbutOrPier[1];		
		ar >> m_dStationBridgeStt;		
		ar >> m_dLengthTotalSlab;		
		ar >> m_nQtyJigan;				
		ar >> m_dWidthSlabLeft;			
		ar >> m_dWidthSlabRight;	
		ar >> m_dHunchArray;		
		ar >> m_arrTendon;
		ar >> m_nTendonFix;
		
		if(nFlag > 12)
			ar >> m_nTendonDomFix;

		ar >> m_nSlabTensionSize;
		long n = 0;
		for(n=0; n<m_nSlabTensionSize; n++)
		{
			ar >> m_SlabTension[n].m_dL;
			ar >> m_SlabTension[n].m_dH;
			ar >> m_SlabTension[n].m_dR;
			ar >> m_SlabTension[n].m_nType;
		}
		ar >> m_dHeightLeftTension;
		ar >> m_dHeightRightTension;

		ar >> m_dRoundCrossUpper;		
		ar >> m_dRoundChipCrossUpper;	
		ar >> m_dRoundCrossLower;		
		ar >> m_dRoundChipCrossLower;	
		ar >> m_dRoundStringerUpper;		
		ar >> m_dRoundChipStringerUpper;	
		ar >> m_dRoundStringerLower;		
		ar >> m_dRoundChipStringerLower;	

		ar >> m_cBracketForm;			
		ar >> m_cBracketType;			
		ar >> m_bLeftBracket;			
		ar >> m_bRightBracket;			
		ar >> m_cBrBeamType;			
		ar >> m_bBracketBeamSpliceApply;	
		ar >> m_dRoundBracketUpper;
		ar >> m_dRoundChipBracketUpper;	
		ar >> m_dRoundBracketLower;		
		ar >> m_dRoundChipBracketLower;	

		ar >> m_dLengthUganStt;			
		ar >> m_dLengthSlabGirderStt;	
		ar >> m_dLengthGirderShoeStt;	
		ar >> m_dLengthGirderShoeEnd;	
		ar >> m_dLengthSlabGirderEnd;	
		ar >> m_dLengthUganEnd;			

		ar >> m_cOffSetStt;				
		ar >> m_cOffSetEnd;				
		ar >> m_dDisExtLineSttLeft;		
		ar >> m_dDisExtLineEndLeft;		
		ar >> m_dDisExtLineSttRight;		
		ar >> m_dDisExtLineEndRight;	
		ar >> m_cDirShoe;				
		ar >> m_cDirVStiffJijum;			

		ar >> m_dLengthShoeToTaperStt;	
		ar >> m_dLengthTaperToTaperStt;	
		ar >> m_dHeightTaperStt;			
		ar >> m_dLengthShoeToTaperEnd;	
		ar >> m_dLengthTaperToTaperEnd;	
		ar >> m_dHeightTaperEnd;			
		ar >> m_HTaper_W;				
		ar >> m_HTaper_H;				

		ar >> m_dHeightInOutStt;			
		ar >> m_dWidthInOutStt;			
		ar >> m_dRadiusInOutStt;			
		ar >> m_dHeightInOutLowerStt;	
		ar >> m_dHeightInOutEnd;			
		ar >> m_dWidthInOutEnd;			
		ar >> m_dRadiusInOutEnd;			
		ar >> m_dHeightInOutLowerEnd;	

		ar >> m_cTaperAngleTypeStt;		
		ar >> m_cTaperAngleTypeEnd;		

		ar >> m_cFacePlateRoundType;		
		ar >> m_cFacePlateUse;
		if(nFlag > 8)
			ar >> m_cFaceManhole;
		else
			m_cFaceManhole = m_cFacePlateUse;
		
		ar >> m_cByunDan;				
		ar >> m_CrossAngleGrid;
		ar >> m_VStiffGrid;

		ar >> m_dLengthBaseCrossBeam;	

		ar >> m_dDisCenterOnCrBeam;		
		ar >> m_dDisCenterOnVst;			
		ar >> m_dDisCenterOnJRib;		
		ar >> m_dDisCenterOnSplice;		
		ar >> m_dDisCenterOnFact;		
		ar >> m_dDisCenterOnCamber;		
		ar >> m_dDisCenterOnStud;		
		ar >> m_dDisCenterOnHStiff;		

		ar >> m_nLengthTypeOnCrBeam;		
		ar >> m_nLengthTypeOnVst;		
		ar >> m_nLengthTypeOnJRib;		
		ar >> m_nLengthTypeOnSplice;		
		ar >> m_nLengthTypeOnFact;		
		ar >> m_nLengthTypeOnCamber;		
		ar >> m_nLengthTypeOnStud;		
		ar >> m_nLengthTypeOnHStiff;		

		ar >> m_dMoveTermOnSplice;		

		ar >> m_bInputAngleVStiff;			
		ar >> m_nCrossArrangeType;			
		ar >> m_dLengthBaseVStiff;			
		ar >> m_nHStiffDefault;				
		ar >> m_cHStiffDimRound;				
		ar >> m_cHStiffDimRoundValue;		
		ar >> m_dHStiffLengthMin;
		if (nFlag > 4)
			ar >> m_dHStiffCheckLength;
		ar >> m_dHStiffTRibApply;			
		ar >> m_cHStiffHeightOffset;			
		ar >> m_dHStiffHeight1;				
		ar >> m_dHStiffHeight2;				
		ar >> m_dHStiffHeight3;				
		ar >> m_dHStiffMinLenJackup;			
		ar >> m_cHStiffJackupVStiff;		
		ar >> m_cFactThickFix;				
		ar >> m_dDisOnCenterFactJoin;		
		ar >> m_cSpliceSimpleType;			

		ar >> m_dThickSlabLeft;				
		ar >> m_dThickSlabBase;				
		ar >> m_dThickSlabRight;				
		ar >> m_dThickSlabHunch;			
		ar >> m_dThickPave;					
		ar >> m_dWidthHunch;				
		ar >> m_dHunchDisForBaseThick;		
		ar >> m_cLevelRoadCenterToSlabEdge;	
		ar >> m_dHeightDosang;				
		ar >> m_dSlopHunch;
		if(nFlag<11)
			m_dSlopHunch = 5;
		ar >> m_dMortalProtected;

		ar >> m_dThickSlabLeftExt;
		ar >> m_dThickSlabRightExt;

		ar >> m_cCamberUnitToMM;				

		ar >> m_cSameApplySameColCross;		
		ar >> m_cSameApplySameColVStiff;		
		ar >> m_cSameApplySameColJRib;		
		ar >> m_cSameApplySameColDanmyun;	
		ar >> m_cSameApplySameColSplice;		
		ar >> m_cSameApplySameColFactJoin;	
		ar >> m_cSameApplySameColOthers;		
		ar >> m_cSameApplySameColHStiff;		
		ar >> m_cSameApplySameColStud;		

		ar >> m_cFactoryType;				
		ar >> m_dMaxFactoryValue;			
		ar >> m_dRateFactoryValue;			

		if(nFlag<8)
		{
			for(n = 0; n < 20; n++)
			{
				ar >> m_dLengthJigan[n];	
				ar >> m_nStyleByun[n];	
				ar >> m_nByunCurveType[n];
				ar >> m_dHeightByun[n];	
				ar >> m_dLenLeftByun[n];	
				ar >> m_dLenRightByun[n];
				ar >> m_dLenLeftCurve[n];
				ar >> m_dLenRightCurve[n];
			}

			for(n = 0; n < 21; n++)
			{
				ar >> m_strJijumName[n];
				ar >> m_vAngJijum[n];
				long m = 0;
				for(m = 0; m < JACKUPSUMAX; m++)
				{
					ar >> m_dJiJumTerm[n][m];
					ar >> m_dJackUpTerm[n][m];
				}
				for(m = 0; m < 2; m++)
				{	
					ar >> m_structJackupJewon[n][m].m_dL;
					ar >> m_structJackupJewon[n][m].m_dD;
					ar >> m_structJackupJewon[n][m].m_dH;
					ar >> m_structJackupJewon[n][m].m_dW;
					ar >> m_structJackupJewon[n][m].m_dT;
				}
			}
		}
		else
		{
			for(n = 0; n < m_nQtyJigan; n++)
			{
				ar >> m_dLengthJigan[n];	
				ar >> m_nStyleByun[n];	
				ar >> m_nByunCurveType[n];
				ar >> m_dHeightByun[n];	
				ar >> m_dLenLeftByun[n];	
				ar >> m_dLenRightByun[n];
				ar >> m_dLenLeftCurve[n];
				ar >> m_dLenRightCurve[n];
			}

			for(n = 0; n < m_nQtyJigan+1; n++)
			{
				ar >> m_strJijumName[n];
				ar >> m_vAngJijum[n];
				long m = 0;
				for(m = 0; m < JACKUPSUMAX; m++)
				{
					ar >> m_dJiJumTerm[n][m];
					ar >> m_dJackUpTerm[n][m];
				}
				for(m = 0; m < 2; m++)
				{	
					ar >> m_structJackupJewon[n][m].m_dL;
					ar >> m_structJackupJewon[n][m].m_dD;
					ar >> m_structJackupJewon[n][m].m_dH;
					ar >> m_structJackupJewon[n][m].m_dW;
					ar >> m_structJackupJewon[n][m].m_dT;
				}
			}

		}

		for(n=0; n<JIJUMSUMAX; n++)
		{
			if(nFlag>8)
				ar >> m_nQtyJijumStiff[n];
			else
			{
				for(long m=0; m<JACKUPSUMAX; m++)
				{
					if(m_dJiJumTerm[n][m] == 0)
					{
						m_nQtyJijumStiff[n] = m;
						break;
					}
					m_nQtyJijumStiff[n] = 6;
				}				
			}
		}

		if(nFlag>8)
		{
			ar >> m_BindConc.m_dWidth;
			ar >> m_BindConc.m_dHeight;
			ar >> m_BindConc.m_dDeep;
			ar >> m_BindConc.m_dDis;
		}
		if(nFlag>11)
		{
			ar >> m_nBaseHeightPosHBraceSplice;
			ar >> m_dBaseHeightFixPosHBraceSplice;
			ar >> m_dBaseHeightRatioPosHBraceSplice;
		}
		
		ar >> m_dSlotA[0];
		ar >> m_dSlotB[0];
		ar >> m_dSlotR[0];
		ar >> m_dSlotA[1];
		ar >> m_dSlotB[1];
		ar >> m_dSlotR[1];
		ar >> m_dSlotA[2];
		ar >> m_dSlotB[2];
		ar >> m_dSlotR[2];
		ar >> m_dSlotR2[0];
		ar >> m_dSlotR2[1];
		ar >> m_dSlotR2[2];
		ar >> m_bApplyDefaultSlot;

		if(nFlag>5)
		{
			ar >> m_FieldScallop_BaseT			;
			ar >> m_FieldScallop_R1				;
			ar >> m_FieldScallop_R2				;
		}

		for(n=0; n<3; n++)
		{
			for(int i=0; i<2; i++)
			{
				ar >> m_nGussetStiff_Gen[n][i];
				ar >> m_dGS_T[n][i];		
				ar >> m_dGS_S3[n][i];
				ar >> m_dGS_W[n][i];
				ar >> m_dGS_dA[n][i];
				ar >> m_dGS_Gen_dB[n][i];
				ar >> m_dGS_Gen_dC[n][i];
				ar >> m_dGS_Trap_dB[n][i];
				ar >> m_dGS_Trap_dC[n][i];
				ar >> m_dGS_uA[n][i];
				ar >> m_dGS_uB[n][i];
				ar >> m_dGS_uC[n][i];
			}
			ar >> m_dGS_S1[n][0];
			ar >> m_dGS_S2[n][0];
			ar >> m_dGS_S1[n][1];
			ar >> m_dGS_S2[n][1];

			ar >> m_dGS_Bolt_B[n];
			ar >> m_dGS_Bolt_H[n];
			ar >> m_dGS_Bolt_R[n];
		}
		ar >> m_nQtyHDanNode;
		for(n = 0; n < HDANLINESU; n++)	// HDANLINESU : 30->50 확장됨
		{
			if(nFlag<14 && n>=30)
				break;

			ar >> m_dLengthHDan[n];
			for(long m = 0; m < 3;m++)
				ar >> m_nValueTypeHDan[n][m];   
		}
		if(nFlag<8)
		{
			for(n = 0; n < 21; n++)
				ar >> m_PierInfo[n];	
		}
		else
		{
			for(n = 0; n < m_nQtyJigan+1; n++)
				ar >> m_PierInfo[n];	
		}

		ar >> m_dA_p; // 일반부
		ar >> m_dB_p;
		ar >> m_dC_p;
		ar >> m_dT_p;
		ar >> m_dC_n; // 부모멘트부
		ar >> m_dT_n;
		ar >> m_nInstallVStiff;

		if(nFlag>1)
		{
			ar >> m_dHBuSL;
			ar >> m_dHBuGw;
			ar >> m_dHBuSA;
			ar >> m_dHBuSB;
			ar >> m_dHBuSC;
			ar >> m_dHBuSD;
			ar >> m_dHBuSE;
			ar >> m_dHBuStep1;
			ar >> m_dHBuStep2;
			ar >> m_dHBuStep3;
			ar >> m_dHBuStep4;
			ar >> m_dHBuStep5;
			ar >> m_dHBuW;
			ar >> m_dHBuW1;
			ar >> m_szHBrSpBoltDia;
		}
		
		ar >> m_nInstallVStiffOnCrossBeam;
		ar >> m_bTendonInputComplete;
		ar >> m_bUserSlabHunch;
		ar >> vLong;//m_nBridgeType;
		ar >> m_bUseJackupSolePlate;
		if(nFlag>3)
			ar >> m_bApplyHunchAtCB;
		ar >> m_dSameFlangeWidth;
		if(nFlag>2)
		{
			ar >> m_cSlabThickAddTypeStt;
			ar >> m_cSlabThickAddTypeEnd;
		}

		ar >> m_dLengthGinjangdan;
		ar >> m_dLengthJungchakdan;
		
		if(nFlag>6)
		{
			ar >> m_dLengthSecMove;
			ar >> m_dLengthSecMoveEnd;
			ar >> m_dLengthDuctOffset;
		}

		SetPOSAndEachPointer();
		for(n=0; n<3; n++)
		{
			for(long i=0; i<2; i++)
			{
				ar >> m_dGS_Sub_sT[n][i];
				ar >> m_dGS_Sub_uD[n][i];
				ar >> m_dGS_Sub_dD[n][i];
				ar >> m_dGS_Sub_uE[n][i];
				ar >> m_dGS_Sub_dE[n][i];
			}
		}

		if(nFlag>0) ar >> m_bQuickLineSearch;

		if(nFlag>8)
		{
			ar >> m_dLengthTUBindConc_Stt;
			ar >> m_dLengthTUBindConc_End;
			ar >> m_dGirEndStndGuttingAngle;
		}
	}
}

//<summary>
//
//</summary>
//<param name="CPlateBridge::SetPOSAndEachPointer("></param>
void CPlateBridge::SetPOSAndEachPointer()
{
	long nG  = 0;
	for(nG = 0;nG < GetGirdersu();nG++)
		GetGirder(nG)->SetBxPosMatchCross();
	
	for(nG = -1;nG < GetGirdersu();nG++)// 후에 선형의 이름으로 구별하는 방식으로 전환
	{
		CPlateGirderApp *pGir = GetGirder(nG);
		pGir->SetLine( ((CLineInfoApp*)m_pLine)->GetLine(pGir->m_nLineNum));
	}
}

CPlateBridge& CPlateBridge::operator=(const CPlateBridge& obj)
{
	AhTPACopy (&m_pGirder, &obj.m_pGirder, (CPlateGirderApp*) 0);

	m_pLine      = obj.m_pLine;
	*m_pCRGirder    = *obj.m_pCRGirder;		
	AhTPACopy (&m_pGuardWall, &obj.m_pGuardWall, (CCentSeparation*) 0);

	m_nQtyHDanNode = obj.m_nQtyHDanNode;
	long n = 0;
	for(n = 0; n < HDANLINESU; n++)
	{
		m_dLengthHDan[n] = obj.m_dLengthHDan[n];  
		for(long m = 0; m < 3;m++)
			m_nValueTypeHDan[n][m] = obj.m_nValueTypeHDan[n][m];   
	}

	m_sNameBridge					= obj.m_sNameBridge;					
	m_sNameOwner					= obj.m_sNameOwner;					
	m_sNameConstructor				= obj.m_sNameConstructor;				
	m_sNameAssembler				= obj.m_sNameAssembler;				
	m_sNameSite						= obj.m_sNameSite;						
	m_sNameEngineering				= obj.m_sNameEngineering;				
	m_NumHaiseokCombo				= obj.m_NumHaiseokCombo;				
	m_bUseLine			            = obj.m_bUseLine;
	m_dBridgeWidth                  = obj.m_dBridgeWidth;
	m_dDisLineToBridgeCen           = obj.m_dDisLineToBridgeCen;
	m_dGirderSpace                  = obj.m_dGirderSpace;              		

	m_sDeungeub						= obj.m_sDeungeub;						
	m_nTypeAbutOrPier[0]			= obj.m_nTypeAbutOrPier[0];			
	m_nTypeAbutOrPier[1]			= obj.m_nTypeAbutOrPier[1];			
	m_dStationBridgeStt				= obj.m_dStationBridgeStt;				
	m_dLengthTotalSlab				= obj.m_dLengthTotalSlab;				
	m_nQtyJigan						= obj.m_nQtyJigan;						
	m_dWidthSlabLeft				= obj.m_dWidthSlabLeft;				
	m_dWidthSlabRight				= obj.m_dWidthSlabRight;				

	m_dRoundCrossUpper				= obj.m_dRoundCrossUpper;				
	m_dRoundChipCrossUpper			= obj.m_dRoundChipCrossUpper;			
	m_dRoundCrossLower				= obj.m_dRoundCrossLower;				
	m_dRoundChipCrossLower			= obj.m_dRoundChipCrossLower;			
	m_dRoundStringerUpper			= obj.m_dRoundStringerUpper;			
	m_dRoundChipStringerUpper		= obj.m_dRoundChipStringerUpper;		
	m_dRoundStringerLower			= obj.m_dRoundStringerLower;			
	m_dRoundChipStringerLower		= obj.m_dRoundChipStringerLower;		

	m_cBracketForm					= obj.m_cBracketForm;					
	m_cBracketType					= obj.m_cBracketType;					
	m_bLeftBracket					= obj.m_bLeftBracket;					
	m_bRightBracket					= obj.m_bRightBracket;					
	m_cBrBeamType					= obj.m_cBrBeamType;					
	m_bBracketBeamSpliceApply		= obj.m_bBracketBeamSpliceApply;		
	m_dRoundBracketUpper			= obj.m_dRoundBracketUpper;			
	m_dRoundChipBracketUpper		= obj.m_dRoundChipBracketUpper;		
	m_dRoundBracketLower			= obj.m_dRoundBracketLower;			
	m_dRoundChipBracketLower		= obj.m_dRoundChipBracketLower;		

	m_dLengthUganStt				= obj.m_dLengthUganStt;				
	m_dLengthSlabGirderStt			= obj.m_dLengthSlabGirderStt;			
	m_dLengthGirderShoeStt			= obj.m_dLengthGirderShoeStt;			
	m_dLengthGirderShoeEnd			= obj.m_dLengthGirderShoeEnd;			
	m_dLengthSlabGirderEnd			= obj.m_dLengthSlabGirderEnd;			
	m_dLengthUganEnd				= obj.m_dLengthUganEnd;				

	m_cOffSetStt					= obj.m_cOffSetStt;					
	m_cOffSetEnd					= obj.m_cOffSetEnd;					
	m_dDisExtLineSttLeft			= obj.m_dDisExtLineSttLeft;			
	m_dDisExtLineEndLeft			= obj.m_dDisExtLineEndLeft;			
	m_dDisExtLineSttRight			= obj.m_dDisExtLineSttRight;			
	m_dDisExtLineEndRight			= obj.m_dDisExtLineEndRight;			
	m_cDirShoe						= obj.m_cDirShoe;						
	m_cDirVStiffJijum				= obj.m_cDirVStiffJijum;				

	m_dLengthShoeToTaperStt			= obj.m_dLengthShoeToTaperStt;			
	m_dLengthTaperToTaperStt		= obj.m_dLengthTaperToTaperStt;		
	m_dHeightTaperStt				= obj.m_dHeightTaperStt;				
	m_dLengthShoeToTaperEnd			= obj.m_dLengthShoeToTaperEnd;			
	m_dLengthTaperToTaperEnd		= obj.m_dLengthTaperToTaperEnd;		
	m_dHeightTaperEnd				= obj.m_dHeightTaperEnd;				
	m_HTaper_W						= obj.m_HTaper_W;						
	m_HTaper_H						= obj.m_HTaper_H;						

	m_dHeightInOutStt				= obj.m_dHeightInOutStt;				
	m_dWidthInOutStt				= obj.m_dWidthInOutStt;				
	m_dRadiusInOutStt				= obj.m_dRadiusInOutStt;				
	m_dHeightInOutLowerStt			= obj.m_dHeightInOutLowerStt;			
	m_dHeightInOutEnd				= obj.m_dHeightInOutEnd;				
	m_dWidthInOutEnd				= obj.m_dWidthInOutEnd;				
	m_dRadiusInOutEnd				= obj.m_dRadiusInOutEnd;				
	m_dHeightInOutLowerEnd			= obj.m_dHeightInOutLowerEnd;			

	m_cTaperAngleTypeStt			= obj.m_cTaperAngleTypeStt;			
	m_cTaperAngleTypeEnd			= obj.m_cTaperAngleTypeEnd;			

	m_cFacePlateRoundType			= obj.m_cFacePlateRoundType;
	m_cFacePlateUse					= obj.m_cFacePlateUse;
	m_cFaceManhole					= obj.m_cFaceManhole;
	
	m_cByunDan						= obj.m_cByunDan;						

	m_dLengthBaseCrossBeam			= obj.m_dLengthBaseCrossBeam;			

	m_dDisCenterOnCrBeam			= obj.m_dDisCenterOnCrBeam;			
	m_dDisCenterOnVst				= obj.m_dDisCenterOnVst;				
	m_dDisCenterOnJRib				= obj.m_dDisCenterOnJRib;				
	m_dDisCenterOnSplice			= obj.m_dDisCenterOnSplice;			
	m_dDisCenterOnFact				= obj.m_dDisCenterOnFact;				
	m_dDisCenterOnCamber			= obj.m_dDisCenterOnCamber;			
	m_dDisCenterOnStud				= obj.m_dDisCenterOnStud;				
	m_dDisCenterOnHStiff			= obj.m_dDisCenterOnHStiff;			

	m_nLengthTypeOnCrBeam			= obj.m_nLengthTypeOnCrBeam;			
	m_nLengthTypeOnVst				= obj.m_nLengthTypeOnVst;				
	m_nLengthTypeOnJRib				= obj.m_nLengthTypeOnJRib;				
	m_nLengthTypeOnSplice			= obj.m_nLengthTypeOnSplice;			
	m_nLengthTypeOnFact				= obj.m_nLengthTypeOnFact;				
	m_nLengthTypeOnCamber			= obj.m_nLengthTypeOnCamber;			
	m_nLengthTypeOnStud				= obj.m_nLengthTypeOnStud;				
	m_nLengthTypeOnHStiff			= obj.m_nLengthTypeOnHStiff;			

	m_dMoveTermOnSplice				= obj.m_dMoveTermOnSplice;				

	m_bInputAngleVStiff				= obj.m_bInputAngleVStiff;				
	m_nCrossArrangeType				= obj.m_nCrossArrangeType;				
	m_dLengthBaseVStiff				= obj.m_dLengthBaseVStiff;				
	m_nHStiffDefault				= obj.m_nHStiffDefault;				
	m_cHStiffDimRound				= obj.m_cHStiffDimRound;				
	m_cHStiffDimRoundValue			= obj.m_cHStiffDimRoundValue;			
	m_dHStiffLengthMin				= obj.m_dHStiffLengthMin;				
	m_dHStiffTRibApply				= obj.m_dHStiffTRibApply;				
	m_cHStiffHeightOffset			= obj.m_cHStiffHeightOffset;			
	m_dHStiffHeight1				= obj.m_dHStiffHeight1;				
	m_dHStiffHeight2				= obj.m_dHStiffHeight2;				
	m_dHStiffHeight3				= obj.m_dHStiffHeight3;				
	m_dHStiffMinLenJackup			= obj.m_dHStiffMinLenJackup;			
	m_cHStiffJackupVStiff			= obj.m_cHStiffJackupVStiff;			
	m_cFactThickFix					= obj.m_cFactThickFix;					
	m_dDisOnCenterFactJoin			= obj.m_dDisOnCenterFactJoin;			
	m_cSpliceSimpleType				= obj.m_cSpliceSimpleType;				

	// [슬래브 제원]// [슬래브 제원]
	m_dThickSlabLeft				= obj.m_dThickSlabLeft;				
	m_dThickSlabBase				= obj.m_dThickSlabBase;				
	m_dThickSlabRight				= obj.m_dThickSlabRight;				
	m_dThickSlabHunch				= obj.m_dThickSlabHunch;				
	m_dThickPave					= obj.m_dThickPave;					
	m_dWidthHunch					= obj.m_dWidthHunch;				
	m_dHunchDisForBaseThick			= obj.m_dHunchDisForBaseThick;			
	m_cLevelRoadCenterToSlabEdge	= obj.m_cLevelRoadCenterToSlabEdge;	
	m_dHeightDosang					= obj.m_dHeightDosang;					
	m_dSlopHunch					= obj.m_dSlopHunch;					
	m_dMortalProtected				= obj.m_dMortalProtected;				

	m_cCamberUnitToMM				= obj.m_cCamberUnitToMM;			

	m_cSameApplySameColCross		= obj.m_cSameApplySameColCross;		
	m_cSameApplySameColVStiff		= obj.m_cSameApplySameColVStiff;		
	m_cSameApplySameColJRib			= obj.m_cSameApplySameColJRib;			
	m_cSameApplySameColDanmyun		= obj.m_cSameApplySameColDanmyun;		
	m_cSameApplySameColSplice		= obj.m_cSameApplySameColSplice;		
	m_cSameApplySameColFactJoin		= obj.m_cSameApplySameColFactJoin;		
	m_cSameApplySameColOthers		= obj.m_cSameApplySameColOthers;		
	m_cSameApplySameColHStiff		= obj.m_cSameApplySameColHStiff;		
	m_cSameApplySameColStud			= obj.m_cSameApplySameColStud;			

	m_cFactoryType					= obj.m_cFactoryType;					
	m_dMaxFactoryValue				= obj.m_dMaxFactoryValue;				
	m_dRateFactoryValue				= obj.m_dRateFactoryValue;				

	for(n = 0; n < KYUNGGANSUMAX; n++)
	{
		m_dLengthJigan[n]	= obj.m_dLengthJigan[n]	;
		m_nStyleByun[n]		= obj.m_nStyleByun[n];
		m_nByunCurveType[n]	= obj.m_nByunCurveType[n];
		m_dHeightByun[n]	= obj.m_dHeightByun[n];
		m_dLenLeftByun[n]	= obj.m_dLenLeftByun[n];
		m_dLenRightByun[n]	= obj.m_dLenRightByun[n];
		m_dLenLeftCurve[n]	= obj.m_dLenLeftCurve[n];
		m_dLenRightCurve[n]	= obj.m_dLenRightCurve[n];
	}
	for(n = 0; n < JIJUMSUMAX; n++)
	{
		m_strJijumName[n] = obj.m_strJijumName[n];
		m_vAngJijum[n]  = obj.m_vAngJijum[n];
		long m = 0;
		for(m = 0; m < JACKUPSUMAX; m++)
		{
			m_dJiJumTerm[n][m] = obj.m_dJiJumTerm[n][m];
			m_dJackUpTerm[n][m] = obj.m_dJackUpTerm[n][m];
		}
		for(m = 0; m < 2; m++)
		{	
			_JACKUP_SOLEPLATE_ JackupJewon  = m_structJackupJewon[n][m];
			_JACKUP_SOLEPLATE_ JackupJewon1 = obj.m_structJackupJewon[n][m];
			JackupJewon.m_dL = JackupJewon1.m_dL;
			JackupJewon.m_dD = JackupJewon1.m_dD;
			JackupJewon.m_dH = JackupJewon1.m_dH;
			JackupJewon.m_dW = JackupJewon1.m_dW;
			JackupJewon.m_dT = JackupJewon1.m_dT;
		}
		m_nQtyJijumStiff[n] = obj.m_nQtyJijumStiff[n];
	}
	m_BindConc.m_dWidth = obj.m_BindConc.m_dWidth;
	m_BindConc.m_dHeight = obj.m_BindConc.m_dHeight;
	m_BindConc.m_dDeep = obj.m_BindConc.m_dDeep;
	m_BindConc.m_dDis = obj.m_BindConc.m_dDis;
	
	m_dSlotA[0] = obj.m_dSlotA[0];
	m_dSlotB[0] = obj.m_dSlotB[0];
	m_dSlotR[0] = obj.m_dSlotR[0];
	m_dSlotR2[0] = obj.m_dSlotR2[0];
	m_dSlotA[1] = obj.m_dSlotA[1];
	m_dSlotB[1] = obj.m_dSlotB[1];
	m_dSlotR[1] = obj.m_dSlotR[1];
	m_dSlotR2[1] = obj.m_dSlotR2[1];
	m_dSlotA[2] = obj.m_dSlotA[2];
	m_dSlotB[2] = obj.m_dSlotB[2];
	m_dSlotR[2] = obj.m_dSlotR[2];
	m_dSlotR2[2] = obj.m_dSlotR2[2];
	m_bApplyDefaultSlot = obj.m_bApplyDefaultSlot;
					
	m_FieldScallop_BaseT	= obj.m_FieldScallop_BaseT;
	m_FieldScallop_R1		= obj.m_FieldScallop_R1;
	m_FieldScallop_R2		= obj.m_FieldScallop_R2;

	for(n=0; n<3; n++)
	{
		for(int i=0; i<2; i++)
		{
			m_nGussetStiff_Gen[n][i] = obj.m_nGussetStiff_Gen[n][i];
			m_dGS_T[n][i] = obj.m_dGS_T[n][i];		
			m_dGS_S3[n][i]= obj.m_dGS_S3[n][i];
			m_dGS_W[n][i] = obj.m_dGS_W[n][i];
			m_dGS_dA[n][i]= obj.m_dGS_dA[n][i];
			m_dGS_Gen_dB[n][i]  = obj.m_dGS_Gen_dB[n][i];
			m_dGS_Gen_dC[n][i]  = obj.m_dGS_Gen_dC[n][i];
			m_dGS_Trap_dB[n][i] = obj.m_dGS_Trap_dB[n][i];
			m_dGS_Trap_dC[n][i] = obj.m_dGS_Trap_dC[n][i];
			m_dGS_uA[n][i] = obj.m_dGS_uA[n][i];
			m_dGS_uB[n][i] = obj.m_dGS_uB[n][i];
			m_dGS_uC[n][i] = obj.m_dGS_uC[n][i];
			m_dGS_Sub_sT[n][i] = obj.m_dGS_Sub_sT[n][i];
			m_dGS_Sub_uD[n][i] = obj.m_dGS_Sub_uD[n][i];
			m_dGS_Sub_dD[n][i] = obj.m_dGS_Sub_dD[n][i];
			m_dGS_Sub_uE[n][i] = obj.m_dGS_Sub_uE[n][i];
			m_dGS_Sub_dE[n][i] = obj.m_dGS_Sub_dE[n][i];
		}
		m_dGS_S1[n][0]  = obj.m_dGS_S1[n][0]; 
		m_dGS_S2[n][0]  = obj.m_dGS_S2[n][0]; 
		m_dGS_S1[n][1]  = obj.m_dGS_S1[n][1]; 
		m_dGS_S2[n][1]  = obj.m_dGS_S2[n][1]; 

		m_dGS_Bolt_B[n] = obj.m_dGS_Bolt_B[n];
		m_dGS_Bolt_H[n] = obj.m_dGS_Bolt_H[n];
		m_dGS_Bolt_R[n] = obj.m_dGS_Bolt_R[n];
	}

	m_bQuickLineSearch = obj.m_bQuickLineSearch;

	m_nBaseHeightPosHBraceSplice = obj.m_nBaseHeightPosHBraceSplice;
	m_dBaseHeightFixPosHBraceSplice = obj.m_dBaseHeightFixPosHBraceSplice;
	m_dBaseHeightRatioPosHBraceSplice = obj.m_dBaseHeightRatioPosHBraceSplice;
	
	return *this;
}

//<summary>
//거더생성
//</summary>
//<param name="nMake"></param>
//<param name="bCopy"></param>
void CPlateBridge::MakeGirder(long nMake,BOOL bCopy /*= TRUE*/)
{
	CPlateGirderApp	*pGir	= GetGirder(0);
	CPlateGirderApp	*pLeft	= NULL;
	
	long nCreate = nMake - GetGirdersu();
	long nG = 0;
	for(nG=0;nG<nCreate;nG++) 	
	{
		if(m_pGirder.GetSize()>0)
			pLeft = m_pGirder[m_pGirder.GetUpperBound()];
		CPlateGirderApp *pGir = new CPlateGirderApp(this,m_pLine,pLeft);
		m_pGirder.Add(pGir);

		if(m_pGirder.GetSize()>1)
			m_pGirder[m_pGirder.GetUpperBound()-1]->SetRightGirder(pGir);


		// bCopy routine 처리
	}
	for(nG=0;nG>nCreate;nG--)
	{
		delete m_pGirder[m_pGirder.GetUpperBound() ];
		m_pGirder.RemoveAt(m_pGirder.GetUpperBound());
	}
	if(m_pGirder.GetSize()>0)
		m_pGirder[m_pGirder.GetUpperBound()]->SetRightGirder(NULL);	
}

//<summary>
//방호벽 생성
//</summary>
//<param name="nMake"></param>
void CPlateBridge::MakeGuardWall(long nMake)
{
	AhTPAMake(nMake,&m_pGuardWall,(CCentSeparation*)0);
}

//<summary>
//방호벽 생성 - 타입별 초기화
//</summary>
//<param name="nMake"></param>
void CPlateBridge::MakeGuardFence(long nMake, CStringArray& strName)
{
	AhTPAMake(nMake,&m_pGuardWall,(CCentSeparation*)0);

	long nSize = m_pGuardWall.GetSize();
	for(long i=0; i<nSize; i++)
	{
		CCentSeparation *pGuard = GetGuardWall(i);
		pGuard->SetSeperationType(strName.GetAt(i));
	}
}

//<summary>
//
//</summary>
//<param name="nGuard"></param>
//<return></return>
CCentSeparation* CPlateBridge::GetGuardWall(long nGuard) const 
{
	if( nGuard < 0 || nGuard >= GetGuardWallSu() ) return NULL;
	return m_pGuardWall.GetAt(nGuard); 
}

CCentSeparation* CPlateBridge::GetCurGuardWall(long nPlace) const
{
	CCentSeparation	*pGuard	= NULL;

	long nGuardSize = m_nQtyHDanNode;

	if(nPlace==CCentSeparation::LEFT)
	{	
		for(long n=0; n<nGuardSize; ++n)
		{
			if(!IsTypeGuardFenceDom(n))	continue;
			long nFence = GetNumGuardFenceFrLeft(n);
			pGuard = GetGuardWall(nFence);		
			if(pGuard->GetPlace() != nPlace) continue;
			return pGuard;		
		}	
	}
	else
	{
		for(long n=nGuardSize-1; n>=0; --n)
		{
			if(!IsTypeGuardFenceDom(n))	continue;
			long nFence = GetNumGuardFenceFrLeft(n);
			pGuard = GetGuardWall(nFence);		
			if(pGuard->GetPlace() != nPlace) continue;			
			return pGuard;		
		}	
	}

	return NULL;
}

BOOL CPlateBridge::IsTypeGuardFenceDom(long nHDan) const
{
	if( GetGuardFenceType(nHDan) == HT_GUARDFENCE || 
		GetGuardFenceType(nHDan) == HT_CENTERGUARDFENCE)
			return TRUE;
	return FALSE;
	
}

long CPlateBridge::GetGuardFenceType(long nHDan)  const
{
	return GetValueTypeHDan(nHDan, 1);
}

long CPlateBridge::GetValueTypeHDan(long nHDan, long nTypeHDan)  const
{
	return m_nValueTypeHDan[nHDan][nTypeHDan];
}

long CPlateBridge::GetNumGuardFenceFrLeft(long nHDan) const
{
	if(!IsTypeGuardFenceDom(nHDan))	//방호벽 타입이 아닐 경우
		return -1;

	long nFence = -1;
	for(long n=0; n <= nHDan; n++)
	{
		if(IsTypeGuardFenceDom(n))	//방호벽 타입이면
			nFence++;
	}
	return nFence;
}

//<summary>
//
//</summary>
//<param name="bAssertUse"></param>
//<return></return>
BOOL CPlateBridge::CheckSelfData(BOOL bAssertUse)
{
	CPlateBasicIndex::m_sBxError.Empty();

	BOOL bResult = TRUE;
	for(long n = -1; n < GetGirdersu(); n++)
	{
		bResult = GetGirder(n)->CheckSelfData(bAssertUse);

		if(!bAssertUse && bResult == FALSE)
		{
			if(CPlateBasicIndex::GetErrorBxString() == "LK_OUTSIDE_ENDBEAM_LEFT In pBx\r\n" ||
			   CPlateBasicIndex::GetErrorBxString() == "LK_OUTSIDE_ENDBEAM_RIGHT In pBx\r\n") return TRUE;
			CString sError;
			sError.Format("G%ld 입력 데이터 오류\r\n",n+1);
			sError += CPlateBasicIndex::GetErrorBxString();			
			AfxMessageBox(sError,MB_ICONERROR|MB_OK);
			return bResult;
		}

	}
	return bResult;
}

//<summary>
//
//</summary>
//<return></return>
BOOL CPlateBridge::IsJustModified() const
{
	for(long nG = -1; nG < GetGirdersu(); nG++)
		if( GetGirder(nG)->IsJustModified() ) return TRUE;
	return FALSE;
}

//<summary>
//
//</summary>
//<return></return>
BOOL CPlateBridge::IsModify() const
{
	for(long nG = -1; nG < GetGirdersu(); nG++)
		if( GetGirder(nG)->IsModify() ) return TRUE;
	return FALSE;
}
//<summary>
//
//</summary>
//<return></return>
BOOL CPlateBridge::IsDataFilled() const
{
	for(long nG = -1; nG < GetGirdersu(); nG++)
		if( GetGirder(nG)->IsModify() ) return FALSE;
	return TRUE;
}
//<summary>
//bModify를 FALSE로 하여 setting후 IsJustModified()로 확인할수 있음
//</summary>
//<param name="bModify"></param>
void CPlateBridge::SetJustModified(BOOL bModify)
{
	for(long nG = -1; nG < GetGirdersu(); nG++)
		GetGirder(nG)->SetJustModified(bModify);
}

//<summary>
//
//</summary>
//<return></return>
long CPlateBridge::GetNumberGirderNearestToCenterLine()
{
	long nGirderNumber = 0;
	double cenDist = fabs(GetGirder(0)->m_dGirderCenter);

	for(long n=0; n<GetGirdersu(); n++) {
		if(IsExtLine(n)==FALSE) {
			if(cenDist>fabs(GetGirder(n)->m_dGirderCenter)) {
				cenDist = fabs(GetGirder(n)->m_dGirderCenter);
				nGirderNumber = n;
			}
		}
	}

	return nGirderNumber;
}

//<summary>
//이형 선형의 사용의 교량여부
//</summary>
//<param name="/*=-1*/"></param>
//<return></return>
BOOL CPlateBridge::IsExtLine(long nG /*=-1*/) const
{	
	CLineInfo* pLine = m_pCRGirder->GetLine();
	if(nG == -1)			//전체 교량이 확폭인지 아닌지를 점검
	{
		for(long nG = 0; nG < GetGirdersu(); nG++)
			if(pLine != GetGirder(nG)->GetLine())
				return TRUE;
	}
	else				//현재 거더(nG)가  이형 선형인지 아닌지를 점검
	{
		if( pLine != GetGirder(nG)->GetLine() )	return TRUE;
	}


	return FALSE;
}

void CPlateBridge::SetGirderSearchIP()
{	
	if(m_bQuickLineSearch)
	{	
		if(IsExtLine())
		{
			CLineInfo *pLine = GetGirder(-1)->GetLine();
			pLine->SetSearchIpBySta(m_dStationBridgeStt, m_dStationBridgeStt + GetLengthBridge());
			for(long nG=0; nG<GetGirdersu(); nG++)
			{
				GetGirder(nG)->GetLine()->SetSearchIpBySta(GetGirder(nG)->GetStationStt(), GetGirder(nG)->GetStationEnd());
			}
		}
		else
		{
			for(long nG=-1; nG<GetGirdersu(); nG++)
				GetGirder(nG)->GetLine()->SetSearchIpBySta(m_dStationBridgeStt, m_dStationBridgeStt + GetLengthBridge());
		}	
	}
	else
	{		
		if(IsExtLine())
		{
			CLineInfo *pLine = GetGirder(-1)->GetLine();
			pLine->ReSetSearchIp();
			for(long nG=0; nG<GetGirdersu(); nG++)
			{
				GetGirder(nG)->GetLine()->ReSetSearchIp();
			}
		}
		else
		{
			for(long nG=-1; nG<GetGirdersu(); nG++)
				GetGirder(nG)->GetLine()->ReSetSearchIp();
		}
	}
}

BOOL CPlateBridge::IsHapsung()
{
	if(m_NumHaiseokCombo==0)	return TRUE;
	else						return FALSE;
}

double CPlateBridge::GetStdMaxTendonHeight()
{
	double dHeight = 0;

	for (long nTs=0; nTs<m_nSlabTensionSize; nTs++)
	{
		_TENSION& Tension = m_SlabTension[nTs];

		if (dHeight<Tension.m_dH) dHeight = Tension.m_dH;
	}

	return dHeight;
}
