// CADeckData.cpp: implementation of the CADeckData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__; 
#define new DEBUG_NEW
#endif


#define REBAR_MIN_SPACE 80
#define MAKE_RATE m_nSelectRebarAddRate==0 ? ((pRB->m_dDia>=22) ? 1.06 : 1.03) : (pRB->m_bMainRebar ? 1.06 : 1.03)
#define TRIM_ARC_LEN 900
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL LessThanMidPoint(const REBAR_SHAPE& vShape1, const REBAR_SHAPE& vShape2)
{
	if (vShape1.m_dArrShape.GetSize()<2 || vShape2.m_dArrShape.GetSize()<2)
		return FALSE;

	double dMidPos1 = (vShape1.m_dArrShape[0].x+vShape1.m_dArrShape[vShape1.m_dArrShape.GetSize()-1].x)/2;
	double dMidPos2 = (vShape2.m_dArrShape[0].x+vShape2.m_dArrShape[vShape2.m_dArrShape.GetSize()-1].x)/2;

	return (dMidPos1 < dMidPos2);
}

CADeckData::CADeckData(CDataManage* pManage)
{
	m_pDataManage = pManage;

	m_dCoverUp	= 60;				// 콘크리트피복두께(상부)
	m_dCoverDn	= 40;				// 콘크리트피복두께(하부)
	m_dCoverSd	= 50;				// 콘크리트피복두께(측면)
	m_nSelectRebarAddRate = 0;
	m_nTypeBMApply = 0;
	m_nRoundRebarConnect = 0;
	m_bIncludeSumZero = TRUE;
	m_dLengthRebarMax = 8000;
	m_bShowMark           = FALSE;			
	m_bRecalcJewon        = FALSE;		
	m_bCheckSinchukDetail = TRUE;
	m_bCheckSinchukSimple = TRUE;
	m_SigmaCK		 = 2.70;
	m_SigmaY		 = 40;
	m_SigmaSA		 = 16;

	m_nTypeWaterHole = 0;
	m_nWaterHolesu	 = 0;
	m_nWaterHoleDansu= 0;
	m_bInstallShearRebar = TRUE;
	m_bInstallWaterHole  = TRUE;

	for (long n=0; n<POS_SLAB_TOTAL; n++)
	{
		m_dDiaMainRebar_Cent[n] = 0;		
		m_dDiaMainRebar_Dan[n] = 0;					
		m_dDiaMainRebar_Cent2nd[n] = 0;			
		m_dDiaMainRebar_Dan2nd[n] = 0;				
		m_dDiaBeryukRebar_Cent[n] = 0;				
		m_dDiaBeryukRebar_Dan[n] = 0;				
		m_dDiaMainRebarLower_Cent[n] = 0;			
		m_dDiaMainRebarLower_Dan[n] = 0;			
		m_dDiaMainRebarLower_Cent2nd[n] = 0;		
		m_dDiaMainRebarLower_Dan2nd[n] = 0;		
		m_dDiaBeryukRebarLower_Cent[n] = 0;			
		m_dDiaBeryukRebarLower_Dan[n] = 0;			
		m_dDiaJijumRebarUp[n] = 0;					
		m_dDiaJijumRebarDn[n] = 0;					
		m_dGenMainRebarCTC[n] = 0;					
		m_dGenSupportRebarCTC[n] = 0;				
		m_dEndMainRebarCTC[n] = 0;					
		m_dEndSupportRebarCTC[n] = 0;				
		m_dGenMainRebarCTCLower[n] = 0;				
		m_dGenSupportRebarCTCLower[n] = 0;			
		m_dEndMainRebarCTCLower[n] = 0;				
		m_dEndSupportRebarCTCLower[n] = 0;			
		m_dJijumRebarCTCUp[n] = 0;					
		m_dJijumRebarCTCDn[n] = 0;					
	}
	m_dSabogangRebarCTCUp = 125;
	m_dSabogangRebarCTCDn = 125;
	
	m_dDiaCrossBeam1 = 0;									
	m_dDiaCrossBeam2 = 0;									
	m_dDiaCrossBeam3 = 0;									
	m_dDiaStringer1 = 0;										
	m_dDiaStringer2 = 0;										
	m_dDiaStringer3 = 0;										
	m_dDiaSabogangUp = 0;									
	m_dDiaSabogangDn = 0;									
	m_dDiaDanbuHunch = 0;									
	m_dDiaDMainRebarUpper_Cent = 0;							
	m_dDiaDMainRebarLower_Cent = 0;							
	m_dDiaDBeryukRebarUpper = 0;								
	m_dDiaDBeryukRebarLower = 0;								
	m_dDiaDMainRebar_Dan = 0;								
	m_dDiaDBeryukRebar_Dan = 0;								
	m_dDMainRebarCTCUpper = 0;							
	m_dDMainRebarCTCLower = 0;							
	m_dDBeryukRebarCTCUpper = 0;						
	m_dDBeryukRebarCTCLower = 0;					
	m_dDDanBogangMainCTC = 0;					
	m_dDDanBogangBeryukCTC = 0;					
	m_dErrorRangeCTC = 0;							    	
	m_dTrimRatio = 0;		
	m_dRangeDanbuStt = 0;
	m_dRangeDanbuEnd = 0;

	m_nTypeDanbuTaperBeryukRebar = 0;			
	m_bInstallDanbuTaperMainRebar = TRUE;			
	m_bInstallMainRebarAtBlockOut = 0;			
	m_bInstallBeryukRebarAtBlockOut = 0;		
	m_nJongCtcCalcPos = 0;						
	m_nJongDrawPos = 0;						
	m_nJijumBogangArrange = 0;					
	m_nOptMainRebarArrange = 0;				
	m_nOptSabogangRebarRange = 0;				
	m_nOptMainRebarAtSkewRange = 0;			
	m_dRangeSabogangRebarStt = 0;			
	m_dRangeMainRebarAtSkewStt = 0;			
	m_dRangeSabogangRebarEnd = 0;			
	m_dRangeMainRebarAtSkewEnd = 0;			
	m_nUseJujangRebar = -1;					
	m_nTypeGuardCalc = 0;						
	m_nDetailMainUpper = 0;					
	m_nDetailMainLowerCenter = 0;				
	m_bInstallCrossBeamUpperBogangRebar = FALSE;	
	m_bInstallStringerUpperBogangRebar = FALSE;	
	m_nDetailSettlement = 0;					
	m_nOptBeryukRebarArrange = 0;				
	m_nCTCOptionGen = 0;						
	m_nCTCOptionPier = 0;						
	m_bApplyBeryukCTC = 0;						
	m_nMainCyclesu = 2;
	m_bMainRebarLowerLevel = FALSE;
	m_bPlusToSlabRebar	   = FALSE;
	m_bDivideRebarAtCant   = FALSE;	
	m_dLengthMainDolchulUp[0] = 0;			
	m_dLengthMainDolchulLo[0] = 0;			
	m_dLengthMainDolchulUpDan[0] = 0;		
	m_dLengthMainDolchulLoDan[0] = 0;		
	m_dLengthMainDolchulUp[1] = 0;			
	m_dLengthMainDolchulLo[1] = 0;			
	m_dLengthMainDolchulUpDan[1] = 0;		
	m_dLengthMainDolchulLoDan[1] = 0;		
	m_dRangeDistribRebarOnCant = 0;			
	m_dWidthSlabCantLeftForBeryuk = 0;		
	m_dWidthSlabCantRighForBeryuk = 0;		
	m_dRangeDistribRebarOnGirder = 0;		
	m_dLengthCrossBeamBogang = 0;			
	m_dLengthStringerBogang = 0;				
	m_dLengthCrossBeamBogangLower = 0;		
	m_dRangeMainRebarFromFlangeStt = 0;		
	m_dLengthBeryukSttA = 0;					
	m_dLengthBeryukSttB = 0;					
	m_dLengthBeryukSttC = 0;					
	m_dLengthBeryukEndA = 0;					
	m_dLengthBeryukEndB = 0;					
	m_dLengthBeryukEndC = 0;					
	m_dRangeDistribRebarDup = 0;				
	m_nAddRebarsu = 0;
	m_nRebarNo = 1;

	m_dHeightLatticeBarDanbu = 70;		// 단부 Lattice Bar 높이
	m_dHeightLatticeBarGen	 = 135;			// 일반부 Lattice Bar 높이
	m_nOptionLBDeckTensionRebarCTC = 0;	// 인장철근 CTC 옵션
	m_nOptionLBDeckLatticeRebarCTC = 0;	// Lattice Bar CTC 옵션
	m_dLengthLBDeckDanbuSettle	   = 0;		// 단부 가로보상면과 패널이 겹치는 범위

}



CADeckData::~CADeckData()
{	
	AhTPADelete(&m_arrRebarJewon,(CRebar*)0);
	AhTPADelete(&m_arrRebarJewonGuard,(CRebar*)0);
}




void CADeckData::Serialize(CArchive& ar)
{
	long nFlag = 7;
	long nAddSize = 0;
	long nAddSize2 = 0;
	
	CBasicConcInfo::Serialize(ar);
	if(CBasicConcInfo::IsSI() == FALSE)
		CBasicConcInfo::SetUnit(TRUE);
	
	if(ar.IsStoring()) 
	{
		ar << nFlag;
		ar << m_dCoverUp;	
		ar << m_dCoverDn;	
		ar << m_dCoverSd;	
		ar << m_nSelectRebarAddRate;
		ar << m_nTypeBMApply ;
		ar << m_nRoundRebarConnect;
		ar << m_bIncludeSumZero;
		ar << m_dLengthRebarMax;
		ar << m_bShowMark;				
		ar << m_bRecalcJewon;			
		ar << m_bCheckSinchukDetail;
		ar << m_bCheckSinchukSimple;

		ar << m_SigmaCK;
		ar << m_SigmaY;
		ar << m_SigmaSA;
		ar << m_nTypeWaterHole;
		ar << m_nWaterHolesu;
		ar << m_nWaterHoleDansu;
		ar << m_bInstallShearRebar;
		ar << m_bInstallWaterHole;
		
		long n = 0;
		for (n=0; n<POS_SLAB_TOTAL; n++)
		{
			ar << m_dDiaMainRebar_Cent[n];		
			ar << m_dDiaMainRebar_Dan[n];					
			ar << m_dDiaMainRebar_Cent2nd[n];			
			ar << m_dDiaMainRebar_Dan2nd[n];				
			ar << m_dDiaBeryukRebar_Cent[n];				
			ar << m_dDiaBeryukRebar_Dan[n];				
			ar << m_dDiaMainRebarLower_Cent[n];			
			ar << m_dDiaMainRebarLower_Dan[n];			
			ar << m_dDiaMainRebarLower_Cent2nd[n];		
			ar << m_dDiaMainRebarLower_Dan2nd[n];		
			ar << m_dDiaBeryukRebarLower_Cent[n];			
			ar << m_dDiaBeryukRebarLower_Dan[n];			
			ar << m_dDiaJijumRebarUp[n];					
			ar << m_dDiaJijumRebarDn[n];					
			ar << m_dGenMainRebarCTC[n];					
			ar << m_dGenSupportRebarCTC[n];				
			ar << m_dEndMainRebarCTC[n];					
			ar << m_dEndSupportRebarCTC[n];				
			ar << m_dGenMainRebarCTCLower[n];				
			ar << m_dGenSupportRebarCTCLower[n];			
			ar << m_dEndMainRebarCTCLower[n];				
			ar << m_dEndSupportRebarCTCLower[n];			
			ar << m_dJijumRebarCTCUp[n];					
			ar << m_dJijumRebarCTCDn[n];					
		}
		ar << m_dSabogangRebarCTCUp;
		ar << m_dSabogangRebarCTCDn;

		ar << m_dDiaCrossBeam1;									
		ar << m_dDiaCrossBeam2;									
		ar << m_dDiaCrossBeam3;									
		ar << m_dDiaStringer1;										
		ar << m_dDiaStringer2;										
		ar << m_dDiaStringer3;										
		ar << m_dDiaOutBrk1;
		ar << m_dDiaOutBrk2;
		ar << m_dDiaOutBrk3;
		ar << m_dDiaSabogangUp;									
		ar << m_dDiaSabogangDn;									
		ar << m_dDiaDanbuHunch;									
		ar << m_dDiaDMainRebarUpper_Cent;							
		ar << m_dDiaDMainRebarLower_Cent;							
		ar << m_dDiaDBeryukRebarUpper;								
		ar << m_dDiaDBeryukRebarLower;								
		ar << m_dDiaDMainRebar_Dan;								
		ar << m_dDiaDBeryukRebar_Dan;								
		ar << m_dDMainRebarCTCUpper;							
		ar << m_dDMainRebarCTCLower;							
		ar << m_dDBeryukRebarCTCUpper;						
		ar << m_dDBeryukRebarCTCLower;					
		ar << m_dDDanBogangMainCTC;					
		ar << m_dDDanBogangBeryukCTC;					
		ar << m_dErrorRangeCTC;							    	
		ar << m_dTrimRatio;		
		for( n=0; n<CTC_BERYUK_SIZE; n++)
		{
			ar << m_arrCTCBeryukRebarDan[n];
			ar << m_arrCTCBeryukRebarCen[n];
			ar << m_arrCTCBeryukRebarJijum[n];
			ar << m_arrVariableBeryukRebar[n];
		}
		for(n=0; n<CTC_MAIN_SIZE; n++)
		{
			ar << m_arrCTCMainRebar[n];
			ar << m_arrCTCMainRebarForDim[n];
		}		
		for(n=0; n<CTC_SABOGANG_SIZE; n++)
			 ar << m_arrCTCSabogangRebar[n];	
		for(n=0; n<CTC_SKEWMAIN_SIZE; n++)
		{
			ar << m_arrCTCMainRebarAtSkew[n];
			ar << m_arrCTCMainRebarAtSkewForDim[n];
		}
		ar << m_arrCTCMainRebarUnderSinchuk;		
		ar << m_arrCTCBogangRebarAtCrossBeam;		
		ar << m_arrIndexBogangRebarAtCrossBeam;
		ar << m_arrCTCBogangRebarAtStringer;	
		ar << m_arrCTCBogangRebarAtOutBrk;
		ar << m_arrCTCMainRebarDanHunch;			
		ar << m_CTCBasicCrossBeam;
		ar << m_CTCBasicStringer;
		ar << m_CTCBasicOutBrk;
		ar << m_arrStringer;
		ar << m_dRangeDanbuStt;
		ar << m_dRangeDanbuEnd;
	
		//////////////////////////////////////////////////////////////////////////
		// 입력정보
		ar << m_stMainCycleDan.size();
		for(n=0; n<(long)m_stMainCycleDan.size(); n++)
		{
			ar << m_stMainCycleDan.at(n).m_dArrJointUp;
			ar << m_stMainCycleDan.at(n).m_dArrJointDn;
			ar << m_stMainCycleDan.at(n).m_dArrDiaUp;
			ar << m_stMainCycleDan.at(n).m_dArrDiaDn;
			ar << m_stMainCycleDan.at(n).m_dArrValidUp;
			ar << m_stMainCycleDan.at(n).m_dArrValidDn;
		}
		ar << m_stMainCycleCen.size();
		for(n=0; n<(long)m_stMainCycleCen.size(); n++)
		{
			ar << m_stMainCycleCen.at(n).m_dArrJointUp;
			ar << m_stMainCycleCen.at(n).m_dArrJointDn;
			ar << m_stMainCycleCen.at(n).m_dArrDiaUp;
			ar << m_stMainCycleCen.at(n).m_dArrDiaDn;
			ar << m_stMainCycleCen.at(n).m_dArrValidUp;
			ar << m_stMainCycleCen.at(n).m_dArrValidDn;
		}
		ar << m_stMainCyclePier.size();
		for(n=0; n<(long)m_stMainCyclePier.size(); n++)
		{
			ar << m_stMainCyclePier.at(n).m_dArrJointUp;
			ar << m_stMainCyclePier.at(n).m_dArrJointDn;
			ar << m_stMainCyclePier.at(n).m_dArrDiaUp;
			ar << m_stMainCyclePier.at(n).m_dArrDiaDn;
			ar << m_stMainCyclePier.at(n).m_dArrValidUp;
			ar << m_stMainCyclePier.at(n).m_dArrValidDn;

		}
		ar << m_stMainCycleSabogangStt.size();
		for(n=0; n<(long)m_stMainCycleSabogangStt.size(); n++)
		{
			ar << m_stMainCycleSabogangStt.at(n).m_dArrJointUp;
			ar << m_stMainCycleSabogangStt.at(n).m_dArrJointDn;
			ar << m_stMainCycleSabogangStt.at(n).m_dArrDiaUp;
			ar << m_stMainCycleSabogangStt.at(n).m_dArrDiaDn;
			ar << m_stMainCycleSabogangStt.at(n).m_dArrValidUp;
			ar << m_stMainCycleSabogangStt.at(n).m_dArrValidDn;
		}
		ar << m_stMainCycleSabogangEnd.size();
		for(n=0; n<(long)m_stMainCycleSabogangEnd.size(); n++)
		{
			ar << m_stMainCycleSabogangEnd.at(n).m_dArrJointUp;
			ar << m_stMainCycleSabogangEnd.at(n).m_dArrJointDn;
			ar << m_stMainCycleSabogangEnd.at(n).m_dArrDiaUp;
			ar << m_stMainCycleSabogangEnd.at(n).m_dArrDiaDn;
			ar << m_stMainCycleSabogangEnd.at(n).m_dArrValidUp;
			ar << m_stMainCycleSabogangEnd.at(n).m_dArrValidDn;
		}
		ar << m_stBeryukCycleLeft.size();
		for(n=0; n<(long)m_stBeryukCycleLeft.size(); n++)
		{
			ar << m_stBeryukCycleLeft.at(n).m_dArrJointUp;
			ar << m_stBeryukCycleLeft.at(n).m_dArrJointDn;
			ar << m_stBeryukCycleLeft.at(n).m_dArrDiaUp;
			ar << m_stBeryukCycleLeft.at(n).m_dArrDiaDn;
			ar << m_stBeryukCycleLeft.at(n).m_dArrValidUp;
			ar << m_stBeryukCycleLeft.at(n).m_dArrValidDn;
		}
		ar << m_stBeryukCycleRigh.size();
		for(n=0; n<(long)m_stBeryukCycleRigh.size(); n++)
		{
			ar << m_stBeryukCycleRigh.at(n).m_dArrJointUp;
			ar << m_stBeryukCycleRigh.at(n).m_dArrJointDn;
			ar << m_stBeryukCycleRigh.at(n).m_dArrDiaUp;
			ar << m_stBeryukCycleRigh.at(n).m_dArrDiaDn;
			ar << m_stBeryukCycleRigh.at(n).m_dArrValidUp;	
			ar << m_stBeryukCycleRigh.at(n).m_dArrValidDn;	
		}
		ar << m_stBeryukCycleCent.size();
		for(n=0; n<(long)m_stBeryukCycleCent.size(); n++)
		{
			ar << m_stBeryukCycleCent.at(n).m_dArrJointUp;
			ar << m_stBeryukCycleCent.at(n).m_dArrJointDn;
			ar << m_stBeryukCycleCent.at(n).m_dArrDiaUp;
			ar << m_stBeryukCycleCent.at(n).m_dArrDiaDn;
			ar << m_stBeryukCycleCent.at(n).m_dArrValidUp;	
			ar << m_stBeryukCycleCent.at(n).m_dArrValidDn;	
		}
		ar << m_stBeryukCycleLC.size();
		for(n=0; n<(long)m_stBeryukCycleLC.size(); n++)
		{
			ar << m_stBeryukCycleLC.at(n).m_dArrJointUp;
			ar << m_stBeryukCycleLC.at(n).m_dArrJointDn;
			ar << m_stBeryukCycleLC.at(n).m_dArrDiaUp;
			ar << m_stBeryukCycleLC.at(n).m_dArrDiaDn;
			ar << m_stBeryukCycleLC.at(n).m_dArrValidUp;	
			ar << m_stBeryukCycleLC.at(n).m_dArrValidDn;	
		}
		ar << m_stBeryukCycleRC.size();
		for(n=0; n<(long)m_stBeryukCycleRC.size(); n++)
		{
			ar << m_stBeryukCycleRC.at(n).m_dArrJointUp;
			ar << m_stBeryukCycleRC.at(n).m_dArrJointDn;
			ar << m_stBeryukCycleRC.at(n).m_dArrDiaUp;
			ar << m_stBeryukCycleRC.at(n).m_dArrDiaDn;
			ar << m_stBeryukCycleRC.at(n).m_dArrValidUp;	
			ar << m_stBeryukCycleRC.at(n).m_dArrValidDn;	
		}
		ar << m_stBeryukCycleOnCB.size();
		for(n=0; n<(long)m_stBeryukCycleOnCB.size(); n++)
		{
			ar << m_stBeryukCycleOnCB.at(n).m_dArrJointUp;
			ar << m_stBeryukCycleOnCB.at(n).m_dArrJointDn;
			ar << m_stBeryukCycleOnCB.at(n).m_dArrDiaUp;
			ar << m_stBeryukCycleOnCB.at(n).m_dArrDiaDn;
			ar << m_stBeryukCycleOnCB.at(n).m_dArrValidUp;	
			ar << m_stBeryukCycleOnCB.at(n).m_dArrValidDn;	
		}
		//////////////////////////////////////////////////////////////////////////
		// 형태정보
		ar << m_stMainShapeDan.size();
		for(n=0; n<(long)m_stMainShapeDan.size(); n++)
		{
			ar << m_stMainShapeDan.at(n).size();
			for (long m=0; m<(long)m_stMainShapeDan.at(n).size(); m++)
			{
				ar << m_stMainShapeDan[n][m].m_dArrShape;
				ar << m_stMainShapeDan[n][m].m_dArrInJoint;
				ar << m_stMainShapeDan[n][m].m_dDia;
				ar << m_stMainShapeDan[n][m].m_sName;
				ar << m_stMainShapeDan[n][m].m_bValid;
				ar << m_stMainShapeDan[n][m].m_nCycle;
				ar << m_stMainShapeDan[n][m].m_bUpper;
			}
		}

		ar << m_stMainShapeCen.size();
		for(n=0; n<(long)m_stMainShapeCen.size(); n++)
		{
			ar << m_stMainShapeCen.at(n).size();
			for (long m=0; m<(long)m_stMainShapeCen.at(n).size(); m++)
			{
				ar << m_stMainShapeCen[n][m].m_dArrShape;
				ar << m_stMainShapeCen[n][m].m_dArrInJoint;
				ar << m_stMainShapeCen[n][m].m_dDia;
				ar << m_stMainShapeCen[n][m].m_sName;
				ar << m_stMainShapeCen[n][m].m_bValid;
				ar << m_stMainShapeCen[n][m].m_nCycle;
				ar << m_stMainShapeCen[n][m].m_bUpper;
			}
		}
		ar << m_stMainShapePier.size();
		for(n=0; n<(long)m_stMainShapePier.size(); n++)
		{
			ar << m_stMainShapePier.at(n).size();
			for (long m=0; m<(long)m_stMainShapePier.at(n).size(); m++)
			{
				ar << m_stMainShapePier[n][m].m_dArrShape;
				ar << m_stMainShapePier[n][m].m_dArrInJoint;
				ar << m_stMainShapePier[n][m].m_dDia;
				ar << m_stMainShapePier[n][m].m_sName;
				ar << m_stMainShapePier[n][m].m_bValid;
				ar << m_stMainShapePier[n][m].m_nCycle;
				ar << m_stMainShapePier[n][m].m_bUpper;
			}
		}
		ar << m_stMainShapeSkewStt.size();
		for(n=0; n<(long)m_stMainShapeSkewStt.size(); n++)
		{
			ar << m_stMainShapeSkewStt.at(n).size();
			for (long m=0; m<(long)m_stMainShapeSkewStt.at(n).size(); m++)
			{
				ar << m_stMainShapeSkewStt[n][m].m_dArrShape;
				ar << m_stMainShapeSkewStt[n][m].m_dArrInJoint;
				ar << m_stMainShapeSkewStt[n][m].m_dDia;
				ar << m_stMainShapeSkewStt[n][m].m_sName;
				ar << m_stMainShapeSkewStt[n][m].m_bValid;
				ar << m_stMainShapeSkewStt[n][m].m_nCycle;
				ar << m_stMainShapeSkewStt[n][m].m_bUpper;
			}
		}
		ar << m_stMainShapeSkewEnd.size();
		for(n=0; n<(long)m_stMainShapeSkewEnd.size(); n++)
		{
			ar << m_stMainShapeSkewEnd.at(n).size();
			for (long m=0; m<(long)m_stMainShapeSkewEnd.at(n).size(); m++)
			{
				ar << m_stMainShapeSkewEnd[n][m].m_dArrShape;
				ar << m_stMainShapeSkewEnd[n][m].m_dArrInJoint;
				ar << m_stMainShapeSkewEnd[n][m].m_dDia;
				ar << m_stMainShapeSkewEnd[n][m].m_sName;
				ar << m_stMainShapeSkewEnd[n][m].m_bValid;
				ar << m_stMainShapeSkewEnd[n][m].m_nCycle;
				ar << m_stMainShapeSkewEnd[n][m].m_bUpper;
			}
		}
		ar << m_stBeryukShapeLeft.size();
		for(n=0; n<(long)m_stBeryukShapeLeft.size(); n++)
		{
			ar << m_stBeryukShapeLeft.at(n).size();
			for (long m=0; m<(long)m_stBeryukShapeLeft.at(n).size(); m++)
			{
				ar << m_stBeryukShapeLeft[n][m].m_dArrShape;
				ar << m_stBeryukShapeLeft[n][m].m_dArrInJoint;
				ar << m_stBeryukShapeLeft[n][m].m_dDia;
				ar << m_stBeryukShapeLeft[n][m].m_sName;
				ar << m_stBeryukShapeLeft[n][m].m_bValid;
				ar << m_stBeryukShapeLeft[n][m].m_nCycle;
				ar << m_stBeryukShapeLeft[n][m].m_bUpper;
			}
		}
		ar << m_stBeryukShapeRigh.size();
		for(n=0; n<(long)m_stBeryukShapeRigh.size(); n++)
		{
			ar << m_stBeryukShapeRigh.at(n).size();
			for (long m=0; m<(long)m_stBeryukShapeRigh.at(n).size(); m++)
			{
				ar << m_stBeryukShapeRigh[n][m].m_dArrShape;
				ar << m_stBeryukShapeRigh[n][m].m_dArrInJoint;
				ar << m_stBeryukShapeRigh[n][m].m_dDia;
				ar << m_stBeryukShapeRigh[n][m].m_sName;
				ar << m_stBeryukShapeRigh[n][m].m_bValid;
				ar << m_stBeryukShapeRigh[n][m].m_nCycle;
				ar << m_stBeryukShapeRigh[n][m].m_bUpper;
			}
		}
		ar << m_stBeryukShapeCent.size();
		for(n=0; n<(long)m_stBeryukShapeCent.size(); n++)
		{
			ar << m_stBeryukShapeCent.at(n).size();
			for (long m=0; m<(long)m_stBeryukShapeCent.at(n).size(); m++)
			{
				ar << m_stBeryukShapeCent[n][m].m_dArrShape;
				ar << m_stBeryukShapeCent[n][m].m_dArrInJoint;
				ar << m_stBeryukShapeCent[n][m].m_dDia;
				ar << m_stBeryukShapeCent[n][m].m_sName;
				ar << m_stBeryukShapeCent[n][m].m_bValid;
				ar << m_stBeryukShapeCent[n][m].m_nCycle;
				ar << m_stBeryukShapeCent[n][m].m_bUpper;
			}
		}

		ar << m_stBeryukShapeLC.size();
		for(n=0; n<(long)m_stBeryukShapeLC.size(); n++)
		{
			ar << m_stBeryukShapeLC.at(n).size();
			for (long m=0; m<(long)m_stBeryukShapeLC.at(n).size(); m++)
			{
				ar << m_stBeryukShapeLC[n][m].m_dArrShape;
				ar << m_stBeryukShapeLC[n][m].m_dArrInJoint;
				ar << m_stBeryukShapeLC[n][m].m_dDia;
				ar << m_stBeryukShapeLC[n][m].m_sName;
				ar << m_stBeryukShapeLC[n][m].m_bValid;
				ar << m_stBeryukShapeLC[n][m].m_nCycle;
				ar << m_stBeryukShapeLC[n][m].m_bUpper;
			}
		}

		ar << m_stBeryukShapeRC.size();
		for(n=0; n<(long)m_stBeryukShapeRC.size(); n++)
		{
			ar << m_stBeryukShapeRC.at(n).size();
			for (long m=0; m<(long)m_stBeryukShapeRC.at(n).size(); m++)
			{
				ar << m_stBeryukShapeRC[n][m].m_dArrShape;
				ar << m_stBeryukShapeRC[n][m].m_dArrInJoint;
				ar << m_stBeryukShapeRC[n][m].m_dDia;
				ar << m_stBeryukShapeRC[n][m].m_sName;
				ar << m_stBeryukShapeRC[n][m].m_bValid;
				ar << m_stBeryukShapeRC[n][m].m_nCycle;
				ar << m_stBeryukShapeRC[n][m].m_bUpper;
			}
		}

		ar << m_stBeryukShapeOnCB.size();
		for(n=0; n<(long)m_stBeryukShapeOnCB.size(); n++)
		{
			ar << m_stBeryukShapeOnCB.at(n).size();
			for (long m=0; m<(long)m_stBeryukShapeOnCB.at(n).size(); m++)
			{
				ar << m_stBeryukShapeOnCB[n][m].m_dArrShape;
				ar << m_stBeryukShapeOnCB[n][m].m_dArrInJoint;
				ar << m_stBeryukShapeOnCB[n][m].m_dDia;
				ar << m_stBeryukShapeOnCB[n][m].m_sName;
				ar << m_stBeryukShapeOnCB[n][m].m_bValid;
				ar << m_stBeryukShapeOnCB[n][m].m_nCycle;
				ar << m_stBeryukShapeOnCB[n][m].m_bUpper;
			}
		}
		ar << m_nTypeDanbuTaperBeryukRebar;				
		ar << m_bInstallDanbuTaperMainRebar;				
		ar << m_bInstallMainRebarAtBlockOut;				
		ar << m_bInstallBeryukRebarAtBlockOut;			
		ar << m_nJongCtcCalcPos;							
		ar << m_nJongDrawPos;							
		ar << m_nJijumBogangArrange;						
		ar << m_nOptMainRebarArrange;					
		ar << m_nOptSabogangRebarRange;					
		ar << m_nOptMainRebarAtSkewRange;				
		ar << m_dRangeSabogangRebarStt;				
		ar << m_dRangeMainRebarAtSkewStt;				
		ar << m_dRangeSabogangRebarEnd;				
		ar << m_dRangeMainRebarAtSkewEnd;				
		ar << m_nUseJujangRebar;						
		ar << m_nTypeGuardCalc;							
		ar << m_nDetailMainUpper;						
		ar << m_nDetailMainLowerCenter;					
		ar << m_bInstallCrossBeamUpperBogangRebar;		
		ar << m_bInstallStringerUpperBogangRebar;		
		ar << m_bInstallOutBrkUpperBogangRebar;
		ar << m_nDetailSettlement;						
		ar << m_nSpaceTrimTypeMain;						
		ar << m_nOptBeryukRebarArrange;					
		ar << m_nCTCOptionGen;							
		ar << m_nCTCOptionPier;							
		ar << m_bApplyBeryukCTC;		
		ar << m_nMainCyclesu;
		ar << m_bMainRebarLowerLevel;
		ar << m_bDivideRebarAtCant;
		ar << m_bPlusToSlabRebar;
		ar << m_dLengthMainDolchulUp[0];		
		ar << m_dLengthMainDolchulLo[0];		
		ar << m_dLengthMainDolchulUpDan[0];		
		ar << m_dLengthMainDolchulLoDan[0];		
		ar << m_dLengthMainDolchulUp[1];		
		ar << m_dLengthMainDolchulLo[1];		
		ar << m_dLengthMainDolchulUpDan[1];		
		ar << m_dLengthMainDolchulLoDan[1];		
		ar << m_dRangeDistribRebarOnCant;		
		ar << m_dWidthSlabCantLeftForBeryuk;	
		ar << m_dWidthSlabCantRighForBeryuk;	
		ar << m_dRangeDistribRebarOnGirder;		
		ar << m_dLengthCrossBeamBogang;			
		ar << m_dLengthStringerBogang;			
		ar << m_dLengthCrossBeamBogangLower;	
		ar << m_dLengthOutBrkBogang;
		ar << m_dLengthOutBrkBogangLower;
		ar << m_dRangeMainRebarFromFlangeStt;	
		ar << m_dLengthBeryukSttA;				
		ar << m_dLengthBeryukSttB;				
		ar << m_dLengthBeryukSttC;				
		ar << m_dLengthBeryukEndA;				
		ar << m_dLengthBeryukEndB;				
		ar << m_dLengthBeryukEndC;				
		ar << m_dRangeDistribRebarDup;			
		ar << m_nAddRebarsu;					
		ar << m_arrTrimRange;
		ar << m_arrSangsePosCB;
		ar << m_arrSangsePosST;
		ar << m_arrSangsePosOBK;
		ar << m_arrSangsePosDan;			
		ar << m_arrSangsePosCant;		
		ar << m_arrSangseIndexCB;		
		ar << m_arrSangseIndexST;		
		ar << m_arrSangseIndexOBK;		
		ar << m_dDiaSangsePosCB;			
		ar << m_dDiaSangsePosST;			
		ar << m_dDiaSangsePosOBK;		
		ar << m_dDiaSangsePosDan;		
		ar << m_dDiaSangsePosCant;		
	
		ar << m_dHeightLatticeBarDanbu;	
		ar << m_dHeightLatticeBarGen;		
		ar << m_nOptionLBDeckTensionRebarCTC;
		ar << m_nOptionLBDeckLatticeRebarCTC; 
		ar << m_dLengthLBDeckDanbuSettle;	   

		for(n=0; n<CTC_SKEWJONG_SIZE; n++)
		{
			ar << m_arrCTCMainRebarAtSkewJong[n];
			ar << m_arrCTCMainRebarAtSkewJongForDim[n];
		}
	}
	else
	{
		ar >> nFlag;
		ar >> m_dCoverUp;	
		ar >> m_dCoverDn;	
		ar >> m_dCoverSd;	
		ar >> m_nSelectRebarAddRate;
		ar >> m_nTypeBMApply ;
		ar >> m_nRoundRebarConnect;
		ar >> m_bIncludeSumZero;
		ar >> m_dLengthRebarMax;
		ar >> m_bShowMark;				
		ar >> m_bRecalcJewon;			
		ar >> m_bCheckSinchukDetail;
		ar >> m_bCheckSinchukSimple;
		ar >> m_SigmaCK;
		ar >> m_SigmaY;
		ar >> m_SigmaSA;

		ar >> m_nTypeWaterHole;
		ar >> m_nWaterHolesu;
		ar >> m_nWaterHoleDansu;
		ar >> m_bInstallShearRebar;
		if(nFlag>2)
			ar >> m_bInstallWaterHole;
		long n = 0;
		for (n=0; n<POS_SLAB_TOTAL; n++)
		{
			ar >> m_dDiaMainRebar_Cent[n];		
			ar >> m_dDiaMainRebar_Dan[n];					
			ar >> m_dDiaMainRebar_Cent2nd[n];			
			ar >> m_dDiaMainRebar_Dan2nd[n];				
			ar >> m_dDiaBeryukRebar_Cent[n];				
			ar >> m_dDiaBeryukRebar_Dan[n];				
			ar >> m_dDiaMainRebarLower_Cent[n];			
			ar >> m_dDiaMainRebarLower_Dan[n];			
			ar >> m_dDiaMainRebarLower_Cent2nd[n];		
			ar >> m_dDiaMainRebarLower_Dan2nd[n];		
			ar >> m_dDiaBeryukRebarLower_Cent[n];			
			ar >> m_dDiaBeryukRebarLower_Dan[n];			
			ar >> m_dDiaJijumRebarUp[n];					
			ar >> m_dDiaJijumRebarDn[n];					
			ar >> m_dGenMainRebarCTC[n];					
			ar >> m_dGenSupportRebarCTC[n];				
			ar >> m_dEndMainRebarCTC[n];					
			ar >> m_dEndSupportRebarCTC[n];				
			ar >> m_dGenMainRebarCTCLower[n];				
			ar >> m_dGenSupportRebarCTCLower[n];			
			ar >> m_dEndMainRebarCTCLower[n];				
			ar >> m_dEndSupportRebarCTCLower[n];			
			ar >> m_dJijumRebarCTCUp[n];					
			ar >> m_dJijumRebarCTCDn[n];					
		}
		ar >> m_dSabogangRebarCTCUp;
		ar >> m_dSabogangRebarCTCDn;
		
		ar >> m_dDiaCrossBeam1;									
		ar >> m_dDiaCrossBeam2;									
		ar >> m_dDiaCrossBeam3;									
		ar >> m_dDiaStringer1;										
		ar >> m_dDiaStringer2;										
		ar >> m_dDiaStringer3;										
		if(nFlag>0)
		{
			ar >> m_dDiaOutBrk1;
			ar >> m_dDiaOutBrk2;
			ar >> m_dDiaOutBrk3;
		}
		ar >> m_dDiaSabogangUp;									
		ar >> m_dDiaSabogangDn;									
		ar >> m_dDiaDanbuHunch;									
		ar >> m_dDiaDMainRebarUpper_Cent;							
		ar >> m_dDiaDMainRebarLower_Cent;							
		ar >> m_dDiaDBeryukRebarUpper;								
		ar >> m_dDiaDBeryukRebarLower;								
		ar >> m_dDiaDMainRebar_Dan;								
		ar >> m_dDiaDBeryukRebar_Dan;								
		ar >> m_dDMainRebarCTCUpper;							
		ar >> m_dDMainRebarCTCLower;							
		ar >> m_dDBeryukRebarCTCUpper;						
		ar >> m_dDBeryukRebarCTCLower;					
		ar >> m_dDDanBogangMainCTC;					
		ar >> m_dDDanBogangBeryukCTC;					
		ar >> m_dErrorRangeCTC;							    	
		ar >> m_dTrimRatio;		
		for(n=0; n<CTC_BERYUK_SIZE; n++)
		{
			ar >> m_arrCTCBeryukRebarDan[n];
			ar >> m_arrCTCBeryukRebarCen[n];
			ar >> m_arrCTCBeryukRebarJijum[n];
			ar >> m_arrVariableBeryukRebar[n];
		}
		for(n=0; n<CTC_MAIN_SIZE; n++)
		{
			ar >> m_arrCTCMainRebar[n];
			ar >> m_arrCTCMainRebarForDim[n];
		}		
		for(n=0; n<CTC_SABOGANG_SIZE; n++)
			 ar >> m_arrCTCSabogangRebar[n];	
		for(n=0; n<CTC_SKEWMAIN_SIZE; n++)
		{
			ar >> m_arrCTCMainRebarAtSkew[n];
			ar >> m_arrCTCMainRebarAtSkewForDim[n];
		}
		ar >> m_arrCTCMainRebarUnderSinchuk;		
		ar >> m_arrCTCBogangRebarAtCrossBeam;		
		if(nFlag>5)
			ar >> m_arrIndexBogangRebarAtCrossBeam;
		ar >> m_arrCTCBogangRebarAtStringer;		
		if(nFlag>0)
			ar >> m_arrCTCBogangRebarAtOutBrk;
		ar >> m_arrCTCMainRebarDanHunch;			
		ar >> m_CTCBasicCrossBeam;
		ar >> m_CTCBasicStringer;
		if(nFlag>0)
			ar >> m_CTCBasicOutBrk;
		ar >> m_arrStringer;
		ar >> m_dRangeDanbuStt;
		ar >> m_dRangeDanbuEnd;
		//////////////////////////////////////////////////////////////////////////
		// 입력정보
		nAddSize = 0;
		ar >> nAddSize;
		m_stMainCycleDan.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stMainCycleDan.push_back(stREBAR_CYCLE);
		}
		ar >> nAddSize;
		m_stMainCycleCen.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stMainCycleCen.push_back(stREBAR_CYCLE);
		}
		ar >> nAddSize;
		m_stMainCyclePier.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stMainCyclePier.push_back(stREBAR_CYCLE);
		}
		ar >> nAddSize;
		m_stMainCycleSabogangStt.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stMainCycleSabogangStt.push_back(stREBAR_CYCLE);
		}
		ar >> nAddSize;
		m_stMainCycleSabogangEnd.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stMainCycleSabogangEnd.push_back(stREBAR_CYCLE);
		}

		ar >> nAddSize;
		m_stBeryukCycleLeft.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stBeryukCycleLeft.push_back(stREBAR_CYCLE);
		}

		ar >> nAddSize;
		m_stBeryukCycleRigh.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stBeryukCycleRigh.push_back(stREBAR_CYCLE);
		}

		ar >> nAddSize;
		m_stBeryukCycleCent.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stBeryukCycleCent.push_back(stREBAR_CYCLE);
		}
		ar >> nAddSize;
		m_stBeryukCycleLC.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stBeryukCycleLC.push_back(stREBAR_CYCLE);
		}
		ar >> nAddSize;
		m_stBeryukCycleRC.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stBeryukCycleRC.push_back(stREBAR_CYCLE);
		}
		ar >> nAddSize;
		m_stBeryukCycleOnCB.empty();
		for( n=0; n<nAddSize; n++)
		{
			struct _REBAR_CYCLE stREBAR_CYCLE;
			ar >> stREBAR_CYCLE.m_dArrJointUp;
			ar >> stREBAR_CYCLE.m_dArrJointDn;
			ar >> stREBAR_CYCLE.m_dArrDiaUp;
			ar >> stREBAR_CYCLE.m_dArrDiaDn;
			ar >> stREBAR_CYCLE.m_dArrValidUp;
			ar >> stREBAR_CYCLE.m_dArrValidDn;

			m_stBeryukCycleOnCB.push_back(stREBAR_CYCLE);
		}
		//////////////////////////////////////////////////////////////////////////
		// 배근정보
		//////////////////////////////////////////////////////////////////////////
		// 형태정보
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stMainShapeDan.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stMainShapeCen.push_back(vRebarShape);
		}
		
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stMainShapePier.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stMainShapeSkewStt.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stMainShapeSkewEnd.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stBeryukShapeLeft.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stBeryukShapeRigh.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stBeryukShapeCent.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stBeryukShapeLC.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stBeryukShapeRC.push_back(vRebarShape);
		}
		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stBeryukShapeOnCB.push_back(vRebarShape);
		}
		ar >> m_nTypeDanbuTaperBeryukRebar;				
		ar >> m_bInstallDanbuTaperMainRebar;				
		ar >> m_bInstallMainRebarAtBlockOut;				
		ar >> m_bInstallBeryukRebarAtBlockOut;			
		ar >> m_nJongCtcCalcPos;							
		ar >> m_nJongDrawPos;							
		ar >> m_nJijumBogangArrange;						
		ar >> m_nOptMainRebarArrange;					
		ar >> m_nOptSabogangRebarRange;					
		ar >> m_nOptMainRebarAtSkewRange;				
		ar >> m_dRangeSabogangRebarStt;				
		ar >> m_dRangeMainRebarAtSkewStt;				
		ar >> m_dRangeSabogangRebarEnd;				
		ar >> m_dRangeMainRebarAtSkewEnd;				
		ar >> m_nUseJujangRebar;						
		ar >> m_nTypeGuardCalc;							
		ar >> m_nDetailMainUpper;						
		ar >> m_nDetailMainLowerCenter;					
		ar >> m_bInstallCrossBeamUpperBogangRebar;		
		ar >> m_bInstallStringerUpperBogangRebar;		
		if(nFlag>0)
			ar >> m_bInstallOutBrkUpperBogangRebar;
		ar >> m_nDetailSettlement;						
		ar >> m_nSpaceTrimTypeMain;						
		ar >> m_nOptBeryukRebarArrange;					
		ar >> m_nCTCOptionGen;							
		ar >> m_nCTCOptionPier;							
		ar >> m_bApplyBeryukCTC;			
		if(nFlag<=3)
		{
			BOOL bTemp;
			ar >> bTemp;
			ar >> bTemp;
		}
		if(nFlag>3)
			ar >> m_nMainCyclesu;
		ar >> m_bMainRebarLowerLevel;
		if(nFlag>4)
			ar >> m_bDivideRebarAtCant;
		if(nFlag>1)
			ar >> m_bPlusToSlabRebar;
		ar >> m_dLengthMainDolchulUp[0];		
		ar >> m_dLengthMainDolchulLo[0];		
		ar >> m_dLengthMainDolchulUpDan[0];		
		ar >> m_dLengthMainDolchulLoDan[0];		
		ar >> m_dLengthMainDolchulUp[1];		
		ar >> m_dLengthMainDolchulLo[1];		
		ar >> m_dLengthMainDolchulUpDan[1];		
		ar >> m_dLengthMainDolchulLoDan[1];		
		ar >> m_dRangeDistribRebarOnCant;		
		ar >> m_dWidthSlabCantLeftForBeryuk;	
		ar >> m_dWidthSlabCantRighForBeryuk;	
		ar >> m_dRangeDistribRebarOnGirder;		
		ar >> m_dLengthCrossBeamBogang;			
		ar >> m_dLengthStringerBogang;			
		ar >> m_dLengthCrossBeamBogangLower;	
		if(nFlag>0)
		{
			ar >> m_dLengthOutBrkBogang;
			ar >> m_dLengthOutBrkBogangLower;
		}
		ar >> m_dRangeMainRebarFromFlangeStt;	
		ar >> m_dLengthBeryukSttA;				
		ar >> m_dLengthBeryukSttB;				
		ar >> m_dLengthBeryukSttC;				
		ar >> m_dLengthBeryukEndA;				
		ar >> m_dLengthBeryukEndB;				
		ar >> m_dLengthBeryukEndC;
		ar >> m_dRangeDistribRebarDup;			
		ar >> m_nAddRebarsu;					
		ar >> m_arrTrimRange;
		ar >> m_arrSangsePosCB;
		ar >> m_arrSangsePosST;
		if(nFlag>0)
			ar >> m_arrSangsePosOBK;
		if(nFlag>1)
		{
			ar >> m_arrSangsePosDan;			
			ar >> m_arrSangsePosCant;		
			ar >> m_arrSangseIndexCB;		
			ar >> m_arrSangseIndexST;		
			ar >> m_arrSangseIndexOBK;		
			ar >> m_dDiaSangsePosCB;			
			ar >> m_dDiaSangsePosST;			
			ar >> m_dDiaSangsePosOBK;		
			ar >> m_dDiaSangsePosDan;		
			ar >> m_dDiaSangsePosCant;		
		}

		ar >> m_dHeightLatticeBarDanbu;	
		ar >> m_dHeightLatticeBarGen;		
		ar >> m_nOptionLBDeckTensionRebarCTC;
		ar >> m_nOptionLBDeckLatticeRebarCTC; 
		ar >> m_dLengthLBDeckDanbuSettle;	   

		if(nFlag>6)
		{
			for(n=0; n<CTC_SKEWJONG_SIZE; n++)
			{
				ar >> m_arrCTCMainRebarAtSkewJong[n];
				ar >> m_arrCTCMainRebarAtSkewJongForDim[n];
			}
		}

		SetDataDefaultRebarInfo();
	}

	m_AddRateRebar.Serialize(ar);				
	m_WaterHoleBoxType.Serialize(ar);			// Box형철근  개구부 Class
	m_WaterHoleRoundType.Serialize(ar);			// Round형철근  개구부 Class
	m_ShearRebarDanbu.Serialize(ar);
	m_ShearRebarJijum.Serialize(ar);
	m_SpacerRebar.Serialize(ar);
	m_BMOption.Serialize(ar);

	AhTPASerial(ar, &m_arrRebarJewon, (CRebar*)0);
	AhTPASerial(ar, &m_arrRebarJewonGuard, (CRebar*)0);

	SerializePlan(ar);
}



void CADeckData::SerializePlan(CArchive& ar)
{
	long nFlag		= 0;
	long nAddSize	= 0;
	long nUD		= 0;
	long nSize		= 0;
	long n			= 0;
	long nPlan		= 0;

	if(ar.IsStoring()) 
	{
		ar << nFlag;

		for(nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN	MainRebar;
			ar << m_vPlanMainRebar[nUD].size();
			for(n=0; n<(long)m_vPlanMainRebar[nUD].size(); n++)
			{
				MainRebar = m_vPlanMainRebar[nUD].at(n);
				ar << MainRebar.m_nPos;
				ar << MainRebar.m_nCycle;
				ar << MainRebar.m_nShape;
				ar << MainRebar.m_tVector;
				ar << MainRebar.m_sName;
			}	
		}
		for(nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN	MainRebarLeft;
			nSize = m_vPlanMainRebarLeft[nUD].size();
			ar << nSize;
			for(n=0; n < nSize; n++)
			{
				MainRebarLeft = m_vPlanMainRebarLeft[nUD].at(n);
				ar << MainRebarLeft.m_nPos;
				ar << MainRebarLeft.m_nCycle;
				ar << MainRebarLeft.m_nShape;
				ar << MainRebarLeft.m_tVector;
				ar << MainRebarLeft.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN	MainRebarRight;
			nSize = m_vPlanMainRebarRigh[nUD].size();
			ar << nSize;
			for(n=0; n < nSize; n++)
			{
				MainRebarRight = m_vPlanMainRebarRigh[nUD].at(n);
				ar << MainRebarRight.m_nPos;
				ar << MainRebarRight.m_nCycle;
				ar << MainRebarRight.m_nShape;
				ar << MainRebarRight.m_tVector;
				ar << MainRebarRight.m_sName;
			}	
		}
		//////////////////////////////////////////////////////////////////////////
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN	MainRebarSkewStt;
			nSize = m_vPlanMainRebarSkewStt[nUD].size();
			ar << nSize;
			for(n=0;n < nSize; n++)
			{
				MainRebarSkewStt = m_vPlanMainRebarSkewStt[nUD].at(n);
				ar << MainRebarSkewStt.m_nPos;
				ar << MainRebarSkewStt.m_nCycle;
				ar << MainRebarSkewStt.m_nShape;
				ar << MainRebarSkewStt.m_tVector;
				ar << MainRebarSkewStt.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN	MainRebarSkewLeft;
			nSize = m_vPlanMainRebarSkewLeftStt[nUD].size();
			ar << nSize;
			for(n=0; n < nSize; n++)
			{
				MainRebarSkewLeft = m_vPlanMainRebarSkewLeftStt[nUD].at(n);
				ar << MainRebarSkewLeft.m_nPos;
				ar << MainRebarSkewLeft.m_nCycle;
				ar << MainRebarSkewLeft.m_nShape;
				ar << MainRebarSkewLeft.m_tVector;
				ar << MainRebarSkewLeft.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN	MainRebarSkewRight;
			nSize = m_vPlanMainRebarSkewRighStt[nUD].size();
			ar << nSize; 
			for(n=0; n< nSize; n++)
			{
				MainRebarSkewRight = m_vPlanMainRebarSkewRighStt[nUD].at(n);
				ar << MainRebarSkewRight.m_nPos;
				ar << MainRebarSkewRight.m_nCycle;
				ar << MainRebarSkewRight.m_nShape;
				ar << MainRebarSkewRight.m_tVector;
				ar << MainRebarSkewRight.m_sName;
			}	
		}
		//////////////////////////////////////////////////////////////////////////
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN MainRebarSkewEnd;
			nSize = m_vPlanMainRebarSkewEnd[nUD].size();
			ar << nSize;
			for(n=0; n< nSize; n++)
			{
				MainRebarSkewEnd = m_vPlanMainRebarSkewEnd[nUD].at(n);
				ar << MainRebarSkewEnd.m_nPos;
				ar << MainRebarSkewEnd.m_nCycle;
				ar << MainRebarSkewEnd.m_nShape;
				ar << MainRebarSkewEnd.m_tVector;
				ar << MainRebarSkewEnd.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN MainRebarSkewLeftEnd;
			nSize = m_vPlanMainRebarSkewLeftEnd[nUD].size();
			ar << nSize;
			for(n=0; n < nSize; n++)
			{
				MainRebarSkewLeftEnd = m_vPlanMainRebarSkewLeftEnd[nUD].at(n);
				ar << MainRebarSkewLeftEnd.m_nPos;
				ar << MainRebarSkewLeftEnd.m_nCycle;
				ar << MainRebarSkewLeftEnd.m_nShape;
				ar << MainRebarSkewLeftEnd.m_tVector;
				ar << MainRebarSkewLeftEnd.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN MainRebarSkewRightEnd;
			nSize = m_vPlanMainRebarSkewRighEnd[nUD].size();
			ar << nSize;
			for(n=0; n<nSize; n++)
			{
				MainRebarSkewRightEnd = m_vPlanMainRebarSkewRighEnd[nUD].at(n);
				ar << MainRebarSkewRightEnd.m_nPos;
				ar << MainRebarSkewRightEnd.m_nCycle;
				ar << MainRebarSkewRightEnd.m_nShape;
				ar << MainRebarSkewRightEnd.m_tVector;
				ar << MainRebarSkewRightEnd.m_sName;
			}	
		}
		//////////////////////////////////////////////////////////////////////////
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN PlanSabogangStt;
			nSize = m_vPlanSabogangStt[nUD].size();
			ar << nSize;
			for(n=0; n < nSize; n++)
			{
				PlanSabogangStt = m_vPlanSabogangStt[nUD].at(n);
				ar << PlanSabogangStt.m_nPos;
				ar << PlanSabogangStt.m_nCycle;
				ar << PlanSabogangStt.m_nShape;
				ar << PlanSabogangStt.m_tVector;
				ar << PlanSabogangStt.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN PlanSabogangLeftStt;
			nSize = m_vPlanSabogangLeftStt[nUD].size();
			ar << nSize;
			for(n=0; n < nSize; n++)
			{
				PlanSabogangLeftStt = m_vPlanSabogangLeftStt[nUD].at(n);
				ar << PlanSabogangLeftStt.m_nPos;
				ar << PlanSabogangLeftStt.m_nCycle;
				ar << PlanSabogangLeftStt.m_nShape;
				ar << PlanSabogangLeftStt.m_tVector;
				ar << PlanSabogangLeftStt.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN PlanSabogangRighStt;
			nSize = m_vPlanSabogangRighStt[nUD].size();
			ar << nSize;
			for(long n=0; n< nSize; n++)
			{
				PlanSabogangRighStt = m_vPlanSabogangRighStt[nUD].at(n);
				ar << PlanSabogangRighStt.m_nPos;
				ar << PlanSabogangRighStt.m_nCycle;
				ar << PlanSabogangRighStt.m_nShape;
				ar << PlanSabogangRighStt.m_tVector;
				ar << PlanSabogangRighStt.m_sName;
			}	
		}
		//////////////////////////////////////////////////////////////////////////
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN PlanSaboganEnd;
			nSize = m_vPlanSabogangEnd[nUD].size();
			ar << nSize;
			for(n=0; n< nSize; n++)
			{
				PlanSaboganEnd = m_vPlanSabogangEnd[nUD].at(n);
				ar << PlanSaboganEnd.m_nPos;
				ar << PlanSaboganEnd.m_nCycle;
				ar << PlanSaboganEnd.m_nShape;
				ar << PlanSaboganEnd.m_tVector;
				ar << PlanSaboganEnd.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN PlanSaboganLeftEnd;
			nSize = m_vPlanSabogangLeftEnd[nUD].size();
			ar << nSize;
			for(n=0; n< nSize; n++)
			{
				PlanSaboganLeftEnd = m_vPlanSabogangLeftEnd[nUD].at(n);
				ar << PlanSaboganLeftEnd.m_nPos;
				ar << PlanSaboganLeftEnd.m_nCycle;
				ar << PlanSaboganLeftEnd.m_nShape;
				ar << PlanSaboganLeftEnd.m_tVector;
				ar << PlanSaboganLeftEnd.m_sName;
			}	
		}
		for ( nUD=0; nUD<2; nUD++)
		{
			REBAR_PLAN PlanSaboganRightEnd;
			nSize = m_vPlanSabogangRighEnd[nUD].size();;
			ar << nSize;
			for(n=0; n< nSize; n++)
			{
				PlanSaboganRightEnd = m_vPlanSabogangRighEnd[nUD].at(n);
				ar << PlanSaboganRightEnd.m_nPos;
				ar << PlanSaboganRightEnd.m_nCycle;
				ar << PlanSaboganRightEnd.m_nShape;
				ar << PlanSaboganRightEnd.m_tVector;
				ar << PlanSaboganRightEnd.m_sName;
			}	
		}
		//////////////////////////////////////////////////////////////////////////
		REBAR_PLAN PlanUnderSinchukStt;
		nSize = m_vPlanUnderSinchukStt.size();
		ar << nSize;
		for(n=0; n < nSize; n++)
		{
			PlanUnderSinchukStt = m_vPlanUnderSinchukStt.at(n);
			ar << PlanUnderSinchukStt.m_nPos;
			ar << PlanUnderSinchukStt.m_nCycle;
			ar << PlanUnderSinchukStt.m_nShape;
			ar << PlanUnderSinchukStt.m_tVector;
			ar << PlanUnderSinchukStt.m_sName;
		}	
		REBAR_PLAN PlanUnderSinchukLeftStt;
		nSize = m_vPlanUnderSinchukLeftStt.size();
		ar << nSize;
		for( n=0; n < nSize; n++)
		{
			PlanUnderSinchukLeftStt = m_vPlanUnderSinchukLeftStt.at(n);
			ar << PlanUnderSinchukLeftStt.m_nPos;
			ar << PlanUnderSinchukLeftStt.m_nCycle;
			ar << PlanUnderSinchukLeftStt.m_nShape;
			ar << PlanUnderSinchukLeftStt.m_tVector;
			ar << PlanUnderSinchukLeftStt.m_sName;
		}	
		REBAR_PLAN PlanUnderSinchukRightStt;
		nSize = m_vPlanUnderSinchukRighStt.size();
		ar << nSize;
		for( n=0; n < nSize; n++)
		{
			PlanUnderSinchukRightStt = m_vPlanUnderSinchukRighStt.at(n);
			ar << PlanUnderSinchukRightStt.m_nPos;
			ar << PlanUnderSinchukRightStt.m_nCycle;
			ar << PlanUnderSinchukRightStt.m_nShape;

			ar << PlanUnderSinchukRightStt.m_tVector;
			ar << PlanUnderSinchukRightStt.m_sName;
		}	
		//////////////////////////////////////////////////////////////////////////
		REBAR_PLAN PlanUnderSinchukEnd;
		nSize = m_vPlanUnderSinchukEnd.size();
		ar << nSize;
		for( n=0; n < nSize; n++)
		{
			PlanUnderSinchukEnd = m_vPlanUnderSinchukEnd.at(n);
			ar << PlanUnderSinchukEnd.m_nPos;
			ar << PlanUnderSinchukEnd.m_nCycle;
			ar << PlanUnderSinchukEnd.m_nShape;
			ar << PlanUnderSinchukEnd.m_tVector;
			ar << PlanUnderSinchukEnd.m_sName;
		}	
		REBAR_PLAN PlanUnderSinchukLeftEnd;
		nSize = m_vPlanUnderSinchukLeftEnd.size();
		ar << nSize;
		for( n=0; n < nSize; n++)
		{
			PlanUnderSinchukLeftEnd = m_vPlanUnderSinchukLeftEnd.at(n);
			ar << PlanUnderSinchukLeftEnd.m_nPos;
			ar << PlanUnderSinchukLeftEnd.m_nCycle;
			ar << PlanUnderSinchukLeftEnd.m_nShape;
			ar << PlanUnderSinchukLeftEnd.m_tVector;
			ar << PlanUnderSinchukLeftEnd.m_sName;
		}	
		REBAR_PLAN PlanUnderSinchukRightEnd;
		nSize = m_vPlanUnderSinchukRighEnd.size();
		ar << nSize;
		for( n=0; n<(long)m_vPlanUnderSinchukRighEnd.size(); n++)
		{
			PlanUnderSinchukRightEnd = m_vPlanUnderSinchukRighEnd.at(n);
			ar << PlanUnderSinchukRightEnd.m_nPos;
			ar << PlanUnderSinchukRightEnd.m_nCycle;
			ar << PlanUnderSinchukRightEnd.m_nShape;
			ar << PlanUnderSinchukRightEnd.m_tVector;
			ar << PlanUnderSinchukRightEnd.m_sName;
		}	
		//////////////////////////////////////////////////////////////////////////
		for (long nSpan=0; nSpan<20; nSpan++)
		{
			long nSize = m_arrMarkPosPlan[nSpan].size();
			ar << nSize;
			for (long n=0; n<nSize; n++)
			{
				ar << m_arrMarkPosPlan[nSpan].at(n).first;
				ar << m_arrMarkPosPlan[nSpan].at(n).second;
			}
		}

	}
	else
	{
		ar >> nFlag;
		for (nPlan=0; nPlan<=14; nPlan++)
		{
			for (long nUD=0; nUD<2; nUD++)
			{
				ar >> nAddSize;
				for(n=0; n<nAddSize; n++)
				{
					struct _REBAR_PLAN stREBAR_PLAN;
					ar >> stREBAR_PLAN.m_nPos;
					ar >> stREBAR_PLAN.m_nCycle;
					ar >> stREBAR_PLAN.m_nShape;
					ar >> stREBAR_PLAN.m_tVector;
					ar >> stREBAR_PLAN.m_sName;

					if(nPlan==0)		m_vPlanMainRebar[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==1)	m_vPlanMainRebarLeft[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==2)	m_vPlanMainRebarRigh[nUD].push_back(stREBAR_PLAN);

					else if(nPlan==3)	m_vPlanMainRebarSkewStt[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==4)	m_vPlanMainRebarSkewLeftStt[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==5)	m_vPlanMainRebarSkewRighStt[nUD].push_back(stREBAR_PLAN);

					else if(nPlan==6)	m_vPlanMainRebarSkewEnd[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==7)	m_vPlanMainRebarSkewLeftEnd[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==8)	m_vPlanMainRebarSkewRighEnd[nUD].push_back(stREBAR_PLAN);

					else if(nPlan==9)	m_vPlanSabogangStt[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==10)	m_vPlanSabogangLeftStt[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==11)	m_vPlanSabogangRighStt[nUD].push_back(stREBAR_PLAN);

					else if(nPlan==12)	m_vPlanSabogangEnd[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==13)	m_vPlanSabogangLeftEnd[nUD].push_back(stREBAR_PLAN);
					else if(nPlan==14)	m_vPlanSabogangRighEnd[nUD].push_back(stREBAR_PLAN);
				}
			}
		}

		for ( nPlan=0; nPlan<=5; nPlan++)
		{
			ar >> nAddSize;
			for(n=0; n<nAddSize; n++)
			{
				struct _REBAR_PLAN stREBAR_PLAN;
				ar >> stREBAR_PLAN.m_nPos;
				ar >> stREBAR_PLAN.m_nCycle;
				ar >> stREBAR_PLAN.m_nShape;
				ar >> stREBAR_PLAN.m_tVector;
				ar >> stREBAR_PLAN.m_sName;
				
				if(nPlan==0)		m_vPlanUnderSinchukStt.push_back(stREBAR_PLAN);
				else if(nPlan==1)	m_vPlanUnderSinchukLeftStt.push_back(stREBAR_PLAN);
				else if(nPlan==2)	m_vPlanUnderSinchukRighStt.push_back(stREBAR_PLAN);

				else if(nPlan==3)	m_vPlanUnderSinchukEnd.push_back(stREBAR_PLAN);
				else if(nPlan==4)	m_vPlanUnderSinchukLeftEnd.push_back(stREBAR_PLAN);
				else if(nPlan==5)	m_vPlanUnderSinchukRighEnd.push_back(stREBAR_PLAN);
			}
		}
		
		for (long nSpan=0; nSpan<20; nSpan++)
		{
			ar >> nAddSize;
			for (n=0; n<nAddSize; n++)
			{
				pair<CString, CDPointArray > vPair;
				ar >> vPair.first;
				ar >> vPair.second;

				m_arrMarkPosPlan[nSpan].push_back(vPair);
			}
		}
	}
}
 

//같은 직선상에 여러점이 있을 경우 제거한다.
void CADeckData::EraseSameVector(CDPointArray& xyArrRebar)
{
	//중복점 제거
	CDPointArray xyArrLine;
	//정점이 두개 이상인 경우에 동일 직선상에 여러점이 있는 경우는 정점을 제거한다.
	double dError = 0.001;
	if(xyArrRebar.GetSize()>2)
	{
		xyArrLine.Add(xyArrRebar.GetAt(0));
		for(long a=0; a<xyArrRebar.GetSize()-2; a++)
		{
			CDPoint xyFir = (xyArrRebar.GetAt(a+1)-xyArrRebar.GetAt(a)).Unit();
			CDPoint xySec = (xyArrRebar.GetAt(a+2)-xyArrRebar.GetAt(a)).Unit();
			if(xySec != xyFir || fabs(xySec.x-xyFir.x) > dError || fabs(xySec.y-xyFir.y) > dError)
				xyArrLine.Add(xyArrRebar.GetAt(a+1));

		}
		xyArrLine.Add(xyArrRebar.GetAt(xyArrRebar.GetUpperBound()));
	}
	else
	{
		for(long a=0; a<xyArrRebar.GetSize(); a++)
			xyArrLine.Add(xyArrRebar.GetAt(a));
	}
	xyArrRebar.RemoveAll();
	for(long n=0; n<xyArrLine.GetSize(); n++)
		xyArrRebar.Add(xyArrLine.GetAt(n));

}


//xyArrRebar로 이루어진 철근의 길이를 리턴
double CADeckData::GetRebarLengthByPointArray(CDPointArray xyArrRebar)
{
	double dRebarL = 0;
	for(long n=0; n<xyArrRebar.GetUpperBound(); n++)
		dRebarL += ~(xyArrRebar.GetAt(n)-xyArrRebar.GetAt(n+1));

	return dRebarL;
}


//xyArrOrigin : 쏠 배열 
//xyArrPoly   : 맞는 배열
CDPointArray CADeckData::GetCDPointArrByPolyLine(CDPointArray xyArrOrigin, CDPointArray xyArrPoly, BOOL bVert)
{
	CDPointArray xyArrResult;
	CDPoint xyResult;
	for(long n=0; n<xyArrOrigin.GetSize(); n++)
	{
		for(long a=0; a<xyArrPoly.GetUpperBound(); a++)
		{
			if(GetXyMatchSegAndLine(xyArrPoly.GetAt(a), xyArrPoly.GetAt(a+1), xyArrOrigin.GetAt(n), bVert?CDPoint(0,-1):CDPoint(1,0), xyResult)==1)
					break;
				else
					continue;
		}
		xyArrResult.Add(xyResult);
	}
	return xyArrResult;
}


//xyArrOrigin : 쏠 배열 
//xyArrPoly   : 맞는 배열
CDPointArray CADeckData::GetCDPointArrByDoubleLine(CDoubleArray dArrOrigin, CDPointArray xyArrPoly)
{
	CDPointArray xyArrResult;
	CDPoint xyResult;
	for(long n=0; n<dArrOrigin.GetSize(); n++)
	{
		for(long a=0; a<xyArrPoly.GetUpperBound(); a++)
		{
			if(GetXyMatchSegAndLine(xyArrPoly.GetAt(a), xyArrPoly.GetAt(a+1), CDPoint(dArrOrigin.GetAt(n), 0), CDPoint(0,1), xyResult)==1)
					break;
				else
					continue;
		}
		xyArrResult.Add(xyResult);
	}
	return xyArrResult;
}



void CADeckData::MakeRebarJewon()
{
	CTypedPtrArray <CObArray, CRebar*> arrLock; 
	long n = 0;
	for (n=0; n<m_arrRebarJewon.GetSize(); n++)
	{
		if(m_arrRebarJewon[n]->m_bLock)	// Lock					
			arrLock.Add(new CRebar(m_arrRebarJewon[n]));					
	}

	AhTPADelete(&m_arrRebarJewon, (CRebar*)0);	

	/////////////////////////////////////////////////////////////////////////////
	// 일반 철근
	m_nRebarNo = 1;
	double dFy = toKgPCm2(m_SigmaY);
	m_AddRateRebar.m_dFy = dFy;

	MakeRebarJewon_Main_Danbu(&m_arrRebarJewon);		// 주철근 단부
	MakeRebarJewon_Main_GenNoHunch(&m_arrRebarJewon);		// 주철근 일반부 상부/하부(헌치없을경우)
	MakeRebarJewon_Main_GenLowerAtHunch(&m_arrRebarJewon);			// 주철근 일반부 하부(헌치부)
	if(m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT].GetSize() > 0)
		MakeRebarJewon_Skew(&m_arrRebarJewon, TRUE);			// 사보강철근
	if(m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END].GetSize() > 0)
		MakeRebarJewon_Skew(&m_arrRebarJewon, FALSE);			// 사보강철근
	MakeRebarJewon_SkewMainRebar(&m_arrRebarJewon, TRUE);	// 사각부 주철근
	MakeRebarJewon_SkewMainRebar(&m_arrRebarJewon, FALSE);	// 사각부 주철근
	for(long nCycle=0; nCycle < (long)m_stBeryukCycleLeft.size(); nCycle++)
		MakeRebarJewon_Beryuk(&m_arrRebarJewon, nCycle);	// 배력철근
	if(IsBridgeHasCrossBeam() && IsBridgeHasSlabHunch() )
		MakeRebarJewon_CrossBeam(&m_arrRebarJewon);			// 가로보 보강철근(C1, C2)
	if(IsBridgeHasStringer() && IsBridgeHasSlabHunch() )
		MakeRebarJewon_Stringer(&m_arrRebarJewon);			// 세로보 보강철근(S1, S2)
	if(IsBridgeHasOutBracket() && IsBridgeHasSlabHunch() )
		MakeRebarJewon_OutBrk(&m_arrRebarJewon);			// 외측가로보 보강철근(OB1, OB2)
	MakeRebarJewon_Etc(&m_arrRebarJewon);				// 간격재, 전단보강철근, 배수구 보강철근

	long i = 0;
	for(i=0; i<arrLock.GetSize(); i++)
	{
		BOOL bSame = FALSE;
		for (long ii=0; ii<m_arrRebarJewon.GetSize(); ii++)
		{
			if(arrLock[i]->m_sMark == m_arrRebarJewon[ii]->m_sMark)
			{
				*m_arrRebarJewon[ii] = *arrLock[i];
				bSame = TRUE;
			}				
		}
		if(bSame==FALSE)
 			m_arrRebarJewon.Add(new CRebar(arrLock[i]));

	}
	AhTPADelete(&arrLock,(CRebar*)0);

	/////////////////////////////////////////////////////////////////////////////
	// 방호벽 철근
	BOOL bLock = FALSE;
	for (n=0; n<m_arrRebarJewonGuard.GetSize(); n++)
	{
		bLock = m_arrRebarJewonGuard[n]->m_bLock;
		if(bLock)	// Lock					
			arrLock.Add(new CRebar(m_arrRebarJewonGuard[n]));					
	}
	AhTPADelete(&m_arrRebarJewonGuard, (CRebar*)0);	

	if(m_nTypeGuardCalc == 0)			//전체길이로 계산 
		MakeRebarJewon_GuardFence(&m_arrRebarJewonGuard, FALSE);		//방호벽 철근 배근 
	else if(m_nTypeGuardCalc == 1)
		MakeRebarJewon_GuardFence(&m_arrRebarJewonGuard, TRUE);		//방호벽 철근 배근 

	for( i=0; i<arrLock.GetSize(); i++)
	{
		BOOL	bSame = FALSE;
		CString	strLockMark = _T("");
		CString	strRebarJewonMark = _T("");
		for (long ii=0; ii<m_arrRebarJewonGuard.GetSize(); ii++)
		{
			strLockMark			= arrLock[i]->m_sMark;
			strRebarJewonMark	= m_arrRebarJewonGuard[ii]->m_sMark;
			if(strLockMark == strRebarJewonMark)
			{
				*m_arrRebarJewonGuard[ii] = *arrLock[i];
				bSame = TRUE;
			}						
		}
		if(bSame==FALSE)
			m_arrRebarJewonGuard.Add(new CRebar(arrLock[i]));

	}
	AhTPADelete(&arrLock,(CRebar*)0);

}




// 주철근을 생성함.
// 단부 주철근 (좌측, 우측, 중앙부의 철근 직경과 간격이 다른 경우와 동일한 경우를 따로 생성함)
// 직경이나 간격이 다른 경우는 좌측, 우측, 중앙부 철근을 개별로 생성함
// 직경과 간격이 동일한 경우는 좌,우,중앙을 하나의 주철근으로 보고 생성함.
void CADeckData::MakeRebarJewon_Main_Danbu(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon)
{
	CString sText;
	BOOL bOneCycle = m_stMainCycleDan.size()==1 ? TRUE : FALSE;

	for (long nCycle=0; nCycle < (long)m_stMainCycleDan.size(); nCycle++)
	{
		m_nRebarNo = 1;
		for (long nUpDn=0; nUpDn<2; nUpDn++)
		{
			BOOL bUpper = nUpDn==0 ? TRUE : FALSE;
			vector<REBAR_SHAPE> vShapeDan = GetNewRebarShapeArrBySection(SECTION_DAN, nCycle, bUpper);
			
			long nShapesu = vShapeDan.size();
			for (long nShape=0; nShape<nShapesu; nShape++)
			{
				CDPointArray xyArrRebar = vShapeDan[nShape].m_dArrShape;

				CRebar *pRB = new CRebar;
				if (bUpper)
				{
					if(bOneCycle)		sText.Format("단부상면%d", nShape+1);
					else				sText.Format("단부상면%d-%dCycle", nShape+1, nCycle+1);
				}
				else
				{
					if(bOneCycle)		sText.Format("단부하면%d", nShape+1);
					else				sText.Format("단부하면%d-%dCycle", nShape+1, nCycle+1);
				}
				pRB->m_sDescription = sText;
				if(bOneCycle)		sText.Format("%d", m_nRebarNo);	
				else				sText.Format("%d-%d", m_nRebarNo, nCycle+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;

				if (bUpper)
				{
					if(nShapesu==1)
					{
						pRB->m_nTypeRebar   = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 3;
						pRB->m_dDia			= vShapeDan[nShape].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x), 0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_UPPER_B, m_dCoverUp, m_dEndMainRebarCTC[0]/2);
					}
					else if(nShapesu==2)
					{
						pRB->m_nTypeRebar = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 2;
						pRB->m_dDia		  = vShapeDan[nShape].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x), 0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_UPPER_B, m_dCoverUp, m_dEndMainRebarCTC[nShape==0 ? 0 : 1]/2);
					}
					else
					{
						if(nShape==0 || nShape==nShapesu-1)
						{
							pRB->m_nTypeRebar = 2;
							pRB->m_dDia		  = vShapeDan[nShape].m_dDia;
							pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x),0);
							pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
							SetLengthRebar(pRB, JOINT_TENS_UPPER_B, m_dCoverUp, m_dEndMainRebarCTC[nShape==0 ? 0 : 1]/2);
						}
						else
						{
							pRB->m_nTypeRebar = 1;
							pRB->m_dDia		  = vShapeDan[nShape].m_dDia;
							pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[0].x-xyArrRebar[xyArrRebar.GetUpperBound()].x), 0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
							SetLengthRebar(pRB, JOINT_TENS_UPPER_B, m_dCoverUp, m_dEndMainRebarCTC[2]/2);
						}
					}
				}
				else
				{
					if(nShapesu==1)
					{
						pRB->m_nTypeRebar   = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 3;
						pRB->m_dDia			= vShapeDan[nShape].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x), 0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]) ,0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[0]/2);
					}
					else if(nShapesu==2)
					{
						pRB->m_nTypeRebar = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 2;
						pRB->m_dDia		  = vShapeDan[nShape].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x),0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[nShape==0 ? 0 : 1]/2);
					}
					else
					{
						if(nShape==0 || nShape==nShapesu-1)
						{
							pRB->m_nTypeRebar = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 2;
							pRB->m_dDia		  = vShapeDan[nShape].m_dDia;
							pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x),0);
							pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
							SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[nShape==0 ? 0 : 1]/2);
						}
						else
						{
							pRB->m_nTypeRebar = 1;
							pRB->m_dDia		  = vShapeDan[nShape].m_dDia;
							pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[0].x-xyArrRebar[xyArrRebar.GetUpperBound()].x),0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
							SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[2]/2);
						}
					}
				}

				pRB->m_nEa = GetRebarsuAtPlanArr(&m_vPlanMainRebar[nUpDn], nCycle, nShape, -1)+GetRebarsuAtPlanArr(&m_vPlanMainRebar[nUpDn], nCycle, nShape, 1);
				pRB->m_nIdxNumber = 0; //주철근 
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewon.Add(pRB);
					m_nRebarNo++;
				}
			}
		}	
	}

	if(m_bInstallDanbuTaperMainRebar)
	{
		CRebar *pRB = new CRebar;
		pRB->m_sDescription = "단부헌치부주철근";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= m_dDiaDanbuHunch;
		pRB->m_dLengthMax_A	= Round(m_pDB->GetWidthAct(m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB))-m_dCoverSd*2, 0);
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		if (m_arrCTCMainRebarDanHunch.GetSize() > 0)
		{
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_arrCTCMainRebarDanHunch[0].y);
			pRB->m_nEa = GetSuRebarArray(&m_arrCTCMainRebarDanHunch, TRUE) * 2;
			pRB->m_nIdxNumber = 0; //주철근 
			
			m_nRebarNo++;
			m_arrRebarJewon.Add(pRB);
		}
	}

	if(m_pDB->m_dHeightTaperStt>0)
	{
		CRebar *pRB = new CRebar;
		pRB->m_sDescription = "신축이음하면주철근";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= m_dDiaMainRebarLower_Dan[POS_SLAB_CENTER];
		pRB->m_dLengthMax_A	= Round(m_pDB->GetWidthAct(m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB))-m_dCoverSd*2, 0);
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		if (m_arrCTCMainRebarUnderSinchuk.GetSize() > 0)
		{
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_arrCTCMainRebarUnderSinchuk[0].y);
			pRB->m_nEa = GetSuRebarArray(&m_arrCTCMainRebarUnderSinchuk, TRUE) * 2;
			pRB->m_nIdxNumber = 0; //주철근 
			
			m_nRebarNo++;
			m_arrRebarJewon.Add(pRB);
		}
	}
	

}



void CADeckData::MakeRebarJewon_Main_GenNoHunch(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon)
{
	if(m_stMainCycleCen.size()==0)	return;

	CString sText;
	BOOL bOneCycle = m_stMainCycleCen.size()==1 ? TRUE : FALSE;
	// 일반부 상면철근
	long nRebarPlus = 0;
	for (long nCycle=0; nCycle < (long)m_stMainCycleCen.size(); nCycle++)
	{
		vector<REBAR_SHAPE> vShapeCenUp = GetNewRebarShapeArrBySection(SECTION_CEN, nCycle, TRUE);

		long nShapesu = vShapeCenUp.size();
		long nTemp = m_nRebarNo;
		nRebarPlus = nShapesu;
		for (long nShape=0; nShape<nShapesu; nShape++)
		{
			CDPointArray xyArrRebar = vShapeCenUp[nShape].m_dArrShape;

			CRebar *pRB = new CRebar;
			if(bOneCycle)		sText.Format("일반부상면%d", nShape+1);
			else				sText.Format("일반부상면%d-%dCycle", nShape+1, nCycle+1);

			pRB->m_sDescription = sText;
			if(bOneCycle)		sText.Format("%d", nTemp);	
			else				sText.Format("%d-%d", nTemp, nCycle+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;

			if(nShapesu==1)
			{
				pRB->m_nTypeRebar   = m_nDetailMainUpper==0 ? 1 : 3;
				pRB->m_dDia			= vShapeCenUp[nShape].m_dDia;
				pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()-1].x), 0);
				pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==0 ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
				pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[0]/2);
			}
			else if(nShapesu==2)
			{
				pRB->m_nTypeRebar = m_nDetailMainUpper==0 ? 1 : 2;
				pRB->m_dDia		  = vShapeCenUp[nShape].m_dDia;
				pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x), 0);
				pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==0 ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
				pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[nShape==0 ? 0 : 1]/2);
			}
			else
			{
				if(nShape==0 || nShape==nShapesu-1)
				{
					pRB->m_nTypeRebar = m_nDetailMainUpper==0 ? 1 : 2;
					pRB->m_dDia		  = vShapeCenUp[nShape].m_dDia;
					pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x), 0);
					pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==0 ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
					pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
					SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[nShape==0 ? 0 : 1]/2);
				}
				else
				{
					pRB->m_nTypeRebar = 1;
					pRB->m_dDia		  = vShapeCenUp[nShape].m_dDia;
					pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[0].x-xyArrRebar[xyArrRebar.GetUpperBound()].x), 0);
					pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
					SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[2]/2);
				}
			}
			pRB->m_nEa = GetRebarsuAtPlanArr(&m_vPlanMainRebar[0], nCycle, nShape, 0);
			pRB->m_nIdxNumber = 0; //주철근 
			
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewon.Add(pRB);
				nTemp++;
			}
		}
	}	
	m_nRebarNo += nRebarPlus;

	//////////////////////////////////////////////////////////////////////////
	// 일반부 하면 철근
	BOOL bNoBrk   = !IsBridgeHasOutBracket();
	BOOL bNoHunch = IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&m_pDB->GetGirdersu()==1) ? TRUE : FALSE;
	BOOL bTaperDiv= m_pDB->m_dHeightTaperStt>0 || (m_pDB->m_dHeightTaperStt==0&&!IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;
	
	if( bNoHunch && bTaperDiv)	// (TYPE4)
	{
		long nRebarPlus = 0;
		for (long nCycle=0; nCycle < (long)m_stMainCycleCen.size(); nCycle++)
		{
			vector<REBAR_SHAPE> vShapeCenDn = GetNewRebarShapeArrBySection(SECTION_CEN, nCycle, FALSE);

			long nShapesu = vShapeCenDn.size();
			long nTemp = m_nRebarNo;
			nRebarPlus = nShapesu;
			for (long nShape=0; nShape<nShapesu; nShape++)
			{
				CDPointArray xyArrRebar = vShapeCenDn[nShape].m_dArrShape;

				CRebar *pRB = new CRebar;
				if(bOneCycle)		sText.Format("일반부하면%d", nShape+1);
				else				sText.Format("일반부하면%d-%dCycle", nShape+1, nCycle+1);

				pRB->m_sDescription = sText;
				if(bOneCycle)		sText.Format("%d", nTemp);	
				else				sText.Format("%d-%d", nTemp, nCycle+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;

				if(nShapesu==1)
				{
					pRB->m_nTypeRebar   = m_nDetailMainUpper==1 ? 1 : 3;
					pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
					pRB->m_dLengthMax_A	= Round(~(xyArrRebar[1]-xyArrRebar[xyArrRebar.GetUpperBound()-1]), 0);
					pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==1 ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
					pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
					SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dEndMainRebarCTCLower[0]/2);
				}
				else if(nShapesu==2)
				{
					pRB->m_nTypeRebar = m_nDetailMainUpper==1 ? 1 : 2;
					pRB->m_dDia		  = vShapeCenDn[nShape].m_dDia;
					pRB->m_dLengthMax_A	= Round(~(xyArrRebar[1]-xyArrRebar[xyArrRebar.GetUpperBound()]), 0);
					pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==1 ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
					pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
					SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dEndMainRebarCTCLower[nShape==0 ? 0 : 1]/2);
				}
				else
				{
					if(nShape==0 || nShape==nShapesu-1)
					{
						pRB->m_nTypeRebar = 2;
						pRB->m_dDia		  = vShapeCenDn[nShape].m_dDia;
						pRB->m_dLengthMax_A	= Round(~(xyArrRebar[1]-xyArrRebar[xyArrRebar.GetUpperBound()]), 0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==1 ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]), 0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dEndMainRebarCTCLower[nShape==0 ? 0 : 1]/2);
					}
					else
					{
						pRB->m_nTypeRebar = 1;
						pRB->m_dDia		  = vShapeCenDn[nShape].m_dDia;
						pRB->m_dLengthMax_A	= Round(~(xyArrRebar[0]-xyArrRebar[xyArrRebar.GetUpperBound()]), 0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dEndMainRebarCTCLower[2]/2);
					}
				}

				pRB->m_nEa = GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
				pRB->m_nIdxNumber = 0; //주철근 

				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewon.Add(pRB);
					nTemp++;
				}
			}
		}	
		m_nRebarNo += nRebarPlus;
	}
}


void CADeckData::MakeRebarJewon_Main_GenLowerAtHunch(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon)
{
	BOOL bNoBrk   = !IsBridgeHasOutBracket();
	BOOL bNoHunch = IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&m_pDB->GetGirdersu()==1) ? TRUE : FALSE;

	if(bNoHunch)	return;		// 헌치가 없는 경우는 아래철근을 생성할 필요가 없다.

	//////////////////////////////////////////////////////////////////////////
	// 외측가로보가 양측에 모두 있는 경우
	vector<REBAR_SHAPE> vShapeCenDn = GetNewRebarShapeArrBySection(SECTION_CEN, 0, FALSE);
	BOOL bLeftBrk = IsBridgeHasOutBracket(SECTION_CEN, TRUE);
	BOOL bRighBrk = IsBridgeHasOutBracket(SECTION_CEN, FALSE);
	long nShape   = 0;
	long nRebarsu = 0;
	CString sText,sDes;

	if(bLeftBrk && bRighBrk)	// 여기서 수정할것...........................................
	{
		CDPointArray xyArrL = GetXyArrPoint("외측빔헌치보강(좌)", vShapeCenDn);
		CDPointArray xyArrR = GetXyArrPoint("외측빔헌치보강(우)", vShapeCenDn);
		long nShapeL = GetNoShapeAtHDanShapeArr("외측빔헌치보강(좌)", vShapeCenDn);
		long nShapeR = GetNoShapeAtHDanShapeArr("외측빔헌치보강(우)", vShapeCenDn);

		CRebar *pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측빔헌치보강";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_dDia			= vShapeCenDn[nShapeL].m_dDia;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		if(~(xyArrL[0]-xyArrL[1]) > 10)	// 돌출부가 있을 경우
		{
			pRB->m_nTypeRebar   = 24;
			pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrR[0]-xyArrR[1])), 0);
			pRB->m_dLengthMax_B	= Round(max(~(xyArrL[1]-xyArrL[2]), ~(xyArrR[1]-xyArrR[2])), 0);
			pRB->m_dLengthMax_C	= Round(max(~(xyArrL[2]-xyArrL[3]), ~(xyArrR[2]-xyArrR[3])), 0);
			pRB->m_dLengthMax_D	= Round(max(~(xyArrL[3]-xyArrL[4]), ~(xyArrR[3]-xyArrR[4])), 0);
			pRB->m_dLengthMin_A	= Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrR[0]-xyArrR[1])), 0);
			pRB->m_dLengthMin_B	= Round(min(~(xyArrL[1]-xyArrL[2]), ~(xyArrR[1]-xyArrR[2])), 0);
			pRB->m_dLengthMin_C	= Round(min(~(xyArrL[2]-xyArrL[3]), ~(xyArrR[2]-xyArrR[3])), 0);
			pRB->m_dLengthMin_D	= Round( min(~(xyArrL[3]-xyArrL[4]), ~(xyArrR[3]-xyArrR[4])), 0);
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
			pRB->m_dAngleX1 = (xyArrL[1]-xyArrL[2]).Unit().GetAngleDegree();
			pRB->m_dAngleX2 = 180 - (xyArrL[4]-xyArrL[3]).Unit().GetAngleDegree();
		}
		else
		{
			pRB->m_nTypeRebar   = 84;
			pRB->m_dLengthMax_A	= Round(max(~(xyArrL[1]-xyArrL[2]), ~(xyArrR[1]-xyArrR[2])), 0);
			pRB->m_dLengthMax_B	= Round(max(~(xyArrL[2]-xyArrL[3]), ~(xyArrR[2]-xyArrR[3])), 0);
			pRB->m_dLengthMax_C	= Round(max(~(xyArrL[3]-xyArrL[4]), ~(xyArrR[3]-xyArrR[4])), 0);
			pRB->m_dLengthMin_A	= Round(min(~(xyArrL[1]-xyArrL[2]), ~(xyArrR[1]-xyArrR[2])), 0);
			pRB->m_dLengthMin_B	= Round(min(~(xyArrL[2]-xyArrL[3]), ~(xyArrR[2]-xyArrR[3])), 0);
			pRB->m_dLengthMin_C	= Round(min(~(xyArrL[3]-xyArrL[4]), ~(xyArrR[3]-xyArrR[4])), 0);
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
			pRB->m_dAngleX1 = (xyArrL[1]-xyArrL[2]).Unit().GetAngleDegree();
			pRB->m_dAngleX2 = 180 - (xyArrL[4]-xyArrL[3]).Unit().GetAngleDegree();
		}
		long nRebarsu	= 0;
		long nCycle		= 0;
		for (nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
		{
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShapeL, 0);
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShapeR, 0);
		}
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 

		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		else delete pRB;

		//
		xyArrL = GetXyArrPoint("외측가로보상부(좌)", vShapeCenDn);
		xyArrR = GetXyArrPoint("외측가로보상부(우)", vShapeCenDn);
		nShapeL = GetNoShapeAtHDanShapeArr("외측가로보상부(좌)", vShapeCenDn);
		nShapeR = GetNoShapeAtHDanShapeArr("외측가로보상부(우)", vShapeCenDn);
		
		pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측가로보상부";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= vShapeCenDn[nShapeL].m_dDia;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrR[0]-xyArrR[1])), 0);
		pRB->m_dLengthMin_A	= Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrR[0]-xyArrR[1])), 0);
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		nRebarsu = 0;
		for ( nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
		{
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShapeL, 0);
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShapeR, 0);
		}
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		else delete pRB;
		//
		xyArrL = GetXyArrPoint("좌측거더상면", vShapeCenDn);
		nShapeL = GetNoShapeAtHDanShapeArr("좌측거더상면", vShapeCenDn);

		pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측거더하면좌측";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText; 
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar	= 84;
		pRB->m_dDia			= vShapeCenDn[nShapeL].m_dDia;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(~(xyArrL[0]-xyArrL[1]), 0);
		pRB->m_dLengthMax_B	= Round(~(xyArrL[1]-xyArrL[2]), 0);
		pRB->m_dLengthMax_C	= Round(~(xyArrL[2]-xyArrL[3]), 0);
		pRB->m_dAngleX1 = (xyArrL[0]-xyArrL[1]).Unit().GetAngleDegree();
		pRB->m_dAngleX2 = 180 - (xyArrL[3]-xyArrL[2]).Unit().GetAngleDegree();
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		nRebarsu = 0;
		for ( nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShapeL, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);	
			m_nRebarNo++;
		}
		else delete pRB;

		if(m_pDB->GetGirdersu() > 1)
		{
			xyArrR = GetXyArrPoint("우측거더상면", vShapeCenDn);
			nShape = GetNoShapeAtHDanShapeArr("우측거더상면", vShapeCenDn);

			pRB = new CRebar();
			pRB->m_sDescription  = "일반부외측거더하면우측";
			sText.Format("%d", m_nRebarNo);	
			pRB->m_sMark		= sText; 
			pRB->m_Fy			= toKgPCm2(m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar	= 84;
			pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A	= Round(~(xyArrR[0]-xyArrR[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrR[1]-xyArrR[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrR[2]-xyArrR[3]), 0);
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[1]/2);
			pRB->m_dAngleX1 = (xyArrL[0]-xyArrL[1]).Unit().GetAngleDegree();
			pRB->m_dAngleX2 = 180 - (xyArrL[3]-xyArrL[2]).Unit().GetAngleDegree();
			nRebarsu = 0;
			for (long nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
				nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
			pRB->m_nEa = nRebarsu;
			pRB->m_nIdxNumber = 0; //주철근 

			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewon.Add(pRB);	
				m_nRebarNo++;
			}
			else delete pRB;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 외측가로보가 좌측에만 있는 경우
	else if(bLeftBrk && !bRighBrk)		
	{
		CDPointArray xyArrL = GetXyArrPoint("외측빔헌치보강(좌)", vShapeCenDn);
		nShape = GetNoShapeAtHDanShapeArr("외측빔헌치보강(좌)", vShapeCenDn);

		CRebar *pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측빔헌치보강(좌)";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		if(~(xyArrL[0]-xyArrL[1]) > 10)	// 돌출부가 있을 경우
		{
			pRB->m_nTypeRebar   = 24;
			pRB->m_dLengthMax_A	= Round(~(xyArrL[0]-xyArrL[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrL[1]-xyArrL[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrL[2]-xyArrL[3]), 0);
			pRB->m_dLengthMax_D	= Round(~(xyArrL[3]-xyArrL[4]), 0);
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		}
		else
		{
			pRB->m_nTypeRebar   = 84;
			pRB->m_dLengthMax_A	= Round(~(xyArrL[1]-xyArrL[2]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrL[2]-xyArrL[3]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrL[3]-xyArrL[4]), 0);
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		}
		long nRebarsu = 0;
		long nCycle	  = 0;
		for (nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		else delete pRB;

		//
		xyArrL = GetXyArrPoint("외측가로보상부(좌)", vShapeCenDn);
		nShape = GetNoShapeAtHDanShapeArr("외측가로보상부(좌)", vShapeCenDn);

		pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측가로보상부(좌)";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(~(xyArrL[0]-xyArrL[1]), 0);
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		nRebarsu = 0;
		for (nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		else delete pRB;

		//
		if(m_pDB->GetGirdersu() > 1)
		{
			xyArrL = GetXyArrPoint("좌측거더상면", vShapeCenDn);
			nShape = GetNoShapeAtHDanShapeArr("좌측거더상면", vShapeCenDn);

			pRB = new CRebar();
			pRB->m_sDescription  = "일반부외측거더하면좌측";
			sText.Format("%d", m_nRebarNo);	
			pRB->m_sMark		= sText; 
			pRB->m_Fy			= toKgPCm2(m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar	= 84;
			pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A	= Round(~(xyArrL[0]-xyArrL[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrL[1]-xyArrL[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrL[2]-xyArrL[3]), 0);

			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
			nRebarsu = 0;
			for (long nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
				nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
			pRB->m_nEa = nRebarsu;
			pRB->m_nIdxNumber = 0; //주철근 
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewon.Add(pRB);
				m_nRebarNo++;
			}
			else delete pRB;
		}
		
		//
		CDPointArray xyArrR = GetXyArrPoint("우측거더상면", vShapeCenDn);
		nShape = GetNoShapeAtHDanShapeArr("우측거더상면", vShapeCenDn);

		pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측거더하면우측";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText; 
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
		if(~(xyArrR[0]-xyArrR[1]) > 10)	// 돌출부가 있을 경우
		{
			pRB->m_nTypeRebar	= 24;
			pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A	= Round(~(xyArrR[0]-xyArrR[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrR[1]-xyArrR[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrR[2]-xyArrR[3]), 0);
			pRB->m_dLengthMax_D	= Round(~(xyArrR[3]-xyArrR[4]), 0);
		}
		else
		{
			pRB->m_nTypeRebar	= 84;
			pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A	= Round(~(xyArrR[0]-xyArrR[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrR[1]-xyArrR[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrR[2]-xyArrR[3]), 0);
		}

		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[2]/2);
		nRebarsu = 0;
		for ( nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		else delete pRB;
	}
	//////////////////////////////////////////////////////////////////////////
	// 외측가로보가 우측에만 있는 경우
	else if(!bLeftBrk && bRighBrk)	
	{
		CDPointArray xyArrR = GetXyArrPoint("외측빔헌치보강(우)", vShapeCenDn);
		nShape = GetNoShapeAtHDanShapeArr("외측빔헌치보강(우)", vShapeCenDn);

		CRebar *pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측빔헌치보강(우)";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		if(~(xyArrR[0]-xyArrR[1]) > 10)	// 돌출부가 있을 경우
		{
			pRB->m_nTypeRebar   = 24;
			pRB->m_dLengthMax_A	= Round(~(xyArrR[0]-xyArrR[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrR[1]-xyArrR[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrR[2]-xyArrR[3]), 0);
			pRB->m_dLengthMax_D	= Round(~(xyArrR[3]-xyArrR[4]), 0);
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		}
		else
		{
			pRB->m_nTypeRebar   = 84;
			pRB->m_dLengthMax_A	= Round(~(xyArrR[1]-xyArrR[2]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrR[2]-xyArrR[3]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrR[3]-xyArrR[4]), 0);
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		}
		long nRebarsu = 0;
		long nCycle	  = 0;
		for (nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		//
		xyArrR = GetXyArrPoint("외측가로보상부(우)", vShapeCenDn);
		nShape = GetNoShapeAtHDanShapeArr("외측가로보상부(우)", vShapeCenDn);

		pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측가로보상부(우)";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(~(xyArrR[0]-xyArrR[1]), 0);
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[2]/2);
		nRebarsu = 0;
		for ( nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		else delete pRB;
		
		//
		CDPointArray xyArrL = GetXyArrPoint("좌측거더상면", vShapeCenDn);
		nShape = GetNoShapeAtHDanShapeArr("좌측거더상면", vShapeCenDn);

		pRB = new CRebar();
		pRB->m_sDescription  = "일반부외측거더하면좌측";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText; 
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
		if(~(xyArrL[0]-xyArrL[1]) > 10)	// 돌출부가 있을 경우
		{
			pRB->m_nTypeRebar = 24;
			pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A	= Round(~(xyArrL[0]-xyArrL[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrL[1]-xyArrL[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrL[2]-xyArrL[3]), 0);
			pRB->m_dLengthMax_D	= Round(~(xyArrL[3]-xyArrL[4]), 0);
		}
		else
		{
			pRB->m_nTypeRebar = 84;
			pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A	= Round(~(xyArrL[0]-xyArrL[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrL[1]-xyArrL[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrL[2]-xyArrL[3]), 0);
		}

		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
		nRebarsu = 0;
		for ( nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			m_nRebarNo++;
		}
		else delete pRB;

		//
		if(m_pDB->GetGirdersu() > 1)
		{
			xyArrR = GetXyArrPoint("우측거더상면", vShapeCenDn);
			nShape = GetNoShapeAtHDanShapeArr("우측거더상면", vShapeCenDn);

			pRB = new CRebar();
			pRB->m_sDescription  = "일반부외측거더하면우측";
			sText.Format("%d", m_nRebarNo);	
			pRB->m_sMark		= sText; 
			pRB->m_Fy			= toKgPCm2(m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar	= 84;
			pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A	= Round(~(xyArrR[0]-xyArrR[1]), 0);
			pRB->m_dLengthMax_B	= Round(~(xyArrR[1]-xyArrR[2]), 0);
			pRB->m_dLengthMax_C	= Round(~(xyArrR[2]-xyArrR[3]), 0);

			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[2]/2);
			nRebarsu = 0;
			for (long nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
				nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
			pRB->m_nEa = nRebarsu;
			pRB->m_nIdxNumber = 0; //주철근 
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewon.Add(pRB);
				m_nRebarNo++;
			}
			else delete pRB;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 외측가로보가 없는 경우
	else
	{
		long nShape = 0;

		if (m_pDB->GetGirdersu() > 1)
		{
			if (m_bDivideRebarAtCant)
			{
				for (long nLR=0; nLR<2; nLR++)
				{
					CRebar *pRB = new CRebar();
					if (nLR==0)		pRB->m_sDescription  = "일반부외측거더하면좌측";
					else			pRB->m_sDescription  = "일반부외측거더하면우측";
					sText.Format("%d", m_nRebarNo);	

					pRB->m_sMark		= sText; 
					pRB->m_Fy			= toKgPCm2(m_SigmaY);	
					pRB->m_bMainRebar   = TRUE;

					CDPointArray xyArrRebar;
					if (nLR==0)		xyArrRebar = GetXyArrPoint("좌측거더상면", vShapeCenDn);
					else			xyArrRebar = GetXyArrPoint("우측거더상면", vShapeCenDn);
					nShape = GetNoShapeAtHDanShapeArr(nLR==0?"좌측거더상면":"우측거더상면", vShapeCenDn);

					pRB->m_nTypeRebar = 84;
					pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
					pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
					pRB->m_dLengthMax_A	=  Round(~(xyArrRebar[0]-xyArrRebar[1]), 0);
					pRB->m_dLengthMax_B	=  Round(~(xyArrRebar[1]-xyArrRebar[2]), 0);
					pRB->m_dLengthMax_C	=  Round(~(xyArrRebar[2]-xyArrRebar[3]), 0);
					if (nLR==0)  //좌측 
					{
						pRB->m_dAngleX1 = (xyArrRebar[0]-xyArrRebar[1]).Unit().GetAngleDegree();
						pRB->m_dAngleX2 = 180 - (xyArrRebar[3]-xyArrRebar[2]).Unit().GetAngleDegree();
					}
					else //우측 
					{
						pRB->m_dAngleX1 = 180 - (xyArrRebar[0]-xyArrRebar[1]).Unit().GetAngleDegree();
						pRB->m_dAngleX2 = (xyArrRebar[3]-xyArrRebar[2]).Unit().GetAngleDegree();
					}
					//////////////////////////////////////////////////////////////////////////
					CRebar *pRB2 = new CRebar();
					if (nLR==0)		pRB2->m_sDescription  = "일반부외측캔틸레버하면좌측";
					else			pRB2->m_sDescription  = "일반부외측캔틸레버하면우측";
					sText.Format("%d", m_nRebarNo);	

					pRB2->m_sMark		= sText; 
					pRB2->m_Fy			= toKgPCm2(m_SigmaY);	
					pRB2->m_bMainRebar   = TRUE;

					if (nLR==0)		xyArrRebar = GetXyArrPoint("좌측캔틸레버상면", vShapeCenDn);
					else			xyArrRebar = GetXyArrPoint("우측캔틸레버상면", vShapeCenDn);
					nShape = GetNoShapeAtHDanShapeArr(nLR==0?"좌측캔틸레버상면":"우측캔틸레버상면", vShapeCenDn);

					pRB2->m_nTypeRebar		= 2;
					pRB2->m_dDia			= vShapeCenDn[nShape].m_dDia;
					pRB2->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB2->m_dDia, pRB2->m_bMainRebar);
					pRB2->m_dLengthMax_A	=  Round(~(xyArrRebar[0]-xyArrRebar[1]), 0);
					pRB2->m_dLengthMax_B	=  Round(~(xyArrRebar[1]-xyArrRebar[2]), 0);

					SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
					SetLengthRebar(pRB2, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
					nRebarsu = 0;
					for (long nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
						nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
					pRB->m_nEa = nRebarsu;
					pRB->m_nIdxNumber = 0; //주철근 
					if(pRB->m_nEa > 0)
					{
						m_arrRebarJewon.Add(pRB);	
						m_nRebarNo++;
					}
					else delete pRB;

					pRB2->m_nEa = nRebarsu;
					pRB2->m_nIdxNumber = 0; //주철근 
					if(pRB2->m_nEa > 0)
					{
						m_arrRebarJewon.Add(pRB2);	
						m_nRebarNo++;
					}
					else delete pRB2;
				}
			}
			else
			{
				for (long nLR=0; nLR<2; nLR++)
				{
					CRebar *pRB = new CRebar();
					if (nLR==0)		pRB->m_sDescription  = "일반부외측거더하면좌측";
					else			pRB->m_sDescription  = "일반부외측거더하면우측";
					sText.Format("%d", m_nRebarNo);	

					pRB->m_sMark		= sText; 
					pRB->m_Fy			= toKgPCm2(m_SigmaY);	
					pRB->m_bMainRebar   = TRUE;

					CDPointArray xyArrRebar;
					if (nLR==0)		xyArrRebar = GetXyArrPoint("좌측거더상면", vShapeCenDn);
					else			xyArrRebar = GetXyArrPoint("우측거더상면", vShapeCenDn);
					nShape = GetNoShapeAtHDanShapeArr(nLR==0?"좌측거더상면":"우측거더상면", vShapeCenDn);

					if(~(xyArrRebar[0]-xyArrRebar[1]) > 10)	// 돌출부가 있을 경우
					{
						pRB->m_nTypeRebar = 24;
						pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						pRB->m_dLengthMax_A	= Round(~(xyArrRebar[0]-xyArrRebar[1]), 0);
						pRB->m_dLengthMax_B	= Round(~(xyArrRebar[1]-xyArrRebar[2]), 0);
						pRB->m_dLengthMax_C	= Round(~(xyArrRebar[2]-xyArrRebar[3]), 0);
						pRB->m_dLengthMax_D	= Round(~(xyArrRebar[3]-xyArrRebar[4]), 0);
						if (nLR==0)  //좌측 
						{
							pRB->m_dAngleX1 = (xyArrRebar[1]-xyArrRebar[2]).Unit().GetAngleDegree();
							pRB->m_dAngleX2 = 180 - (xyArrRebar[4]-xyArrRebar[3]).Unit().GetAngleDegree();
						}
						else //우측 
						{
							pRB->m_dAngleX1 = 180 - (xyArrRebar[1]-xyArrRebar[2]).Unit().GetAngleDegree();
							pRB->m_dAngleX2 = (xyArrRebar[4]-xyArrRebar[3]).Unit().GetAngleDegree();
						}
					}
					else
					{
						pRB->m_nTypeRebar = 84;
						pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						pRB->m_dLengthMax_A	=  Round(~(xyArrRebar[1]-xyArrRebar[2]), 0);
						pRB->m_dLengthMax_B	=  Round(~(xyArrRebar[2]-xyArrRebar[3]), 0);
						pRB->m_dLengthMax_C	=  Round(~(xyArrRebar[3]-xyArrRebar[4]), 0);
						if (nLR==0)  //좌측 
						{
							pRB->m_dAngleX1 = (xyArrRebar[1]-xyArrRebar[2]).Unit().GetAngleDegree();
							pRB->m_dAngleX2 = 180 - (xyArrRebar[4]-xyArrRebar[3]).Unit().GetAngleDegree();
						}
						else //우측 
						{
							pRB->m_dAngleX1 = 180 - (xyArrRebar[1]-xyArrRebar[2]).Unit().GetAngleDegree();
							pRB->m_dAngleX2 = (xyArrRebar[4]-xyArrRebar[3]).Unit().GetAngleDegree();
						}
					}
					SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[0]/2);
					nRebarsu = 0;
					for (long nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
						nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
					pRB->m_nEa = nRebarsu;
					pRB->m_nIdxNumber = 0; //주철근 
					if(pRB->m_nEa > 0)
					{
						m_arrRebarJewon.Add(pRB);	
						m_nRebarNo++;
					}
					else delete pRB;
				}
			}
			
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 박스와 박스 사이의 하면보강철근
	if(m_pDB->GetGirdersu() > 1)
	{
		CRebar *pRB = new CRebar();
		pRB->m_sDescription  = "일반부중앙가로보부하면";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText; 
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar	= 1;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

		CDPointArray xyArrRebar = GetXyArrPoint("가로보상부전체", vShapeCenDn);

		nShape = GetNoShapeAtHDanShapeArr("가로보상부전체", vShapeCenDn);
		pRB->m_dLengthMax_A	= Round(~(xyArrRebar[0]-xyArrRebar[xyArrRebar.GetUpperBound()]), 0);
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[2]/2);
		nRebarsu = 0;
		for (long nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);	
			m_nRebarNo++;
		}
		else delete pRB;
	}

	if(m_pDB->GetGirdersu() > 2)
	{
		double dMaxA=0, dMaxB=0, dMaxC=0;
		double dMinA=0, dMinB=0, dMinC=0;
		nRebarsu = 0;
		for(long nG=1; nG<=m_pDB->GetGirdersu()-2; nG++)
		{
			sText.Format("G%d상면", nG);
			CDPointArray xyArrRebar = GetXyArrPoint(sText, vShapeCenDn);
			nShape = GetNoShapeAtHDanShapeArr(sText, vShapeCenDn);
			for (long nCycle=0; nCycle<(long)m_stMainCycleCen.size(); nCycle++)
			{
				if(!vShapeCenDn[nShape].m_bValid)	continue;
				nRebarsu += GetRebarsuAtPlanArr(&m_vPlanMainRebar[1], nCycle, nShape, 0);
			}
			
			dMaxA = max(dMaxA, ~(xyArrRebar[0]-xyArrRebar[1]));
			dMaxB = max(dMaxB, ~(xyArrRebar[1]-xyArrRebar[2]));
			dMaxC = max(dMaxC, ~(xyArrRebar[2]-xyArrRebar[3]));
			dMinA = min(dMinA, ~(xyArrRebar[0]-xyArrRebar[1]));
			dMinB = min(dMinB, ~(xyArrRebar[1]-xyArrRebar[2]));
			dMinC = min(dMinC, ~(xyArrRebar[2]-xyArrRebar[3]));
		}

		CRebar *pRB = new CRebar();

		pRB->m_sDescription  = "일반부중앙거더하면";
		sText.Format("%d", m_nRebarNo);	
		pRB->m_sMark		= sText; 
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar	= 84;
		pRB->m_dDia			= vShapeCenDn[nShape].m_dDia;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(dMaxA, 0);
		pRB->m_dLengthMin_A	= Round(dMinA, 0);
		pRB->m_dLengthMax_B	= Round(dMaxB, 0);
		pRB->m_dLengthMin_B	= Round(dMinB, 0);
		pRB->m_dLengthMax_C	= Round(dMaxC, 0);
		pRB->m_dLengthMin_C	= Round(dMinC, 0);

		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[2]/2);
		pRB->m_nEa = nRebarsu;
		pRB->m_nIdxNumber = 0; //주철근 
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);	
			m_nRebarNo++;
		}
		else delete pRB;
	}	
}



void CADeckData::MakeRebarJewon_Beryuk(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, long nCycle)
{
	CGlobarOption   *pGlopt  = m_pDB->m_pDataManage->GetGlobalOption();

	double dSttBlockOutHeight = pGlopt->GetSttExpansionJointHeight();
	if(m_bInstallBeryukRebarAtBlockOut)
	{
		dSttBlockOutHeight  = 0;
	}

	BOOL bOneCycle = m_stBeryukCycleLeft.size()==1 ? TRUE : FALSE;
	CString sText;
	long nRebarMark = 1;
	// 배력단부하면 1 (전 사이클에서 형상이 동일함.                         
	CDPointArray xyArrLStt  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle,"시점부하부(1)");
	CDPointArray xyArrRStt  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle,"시점부하부(1)");
	CDPointArray xyArrLCStt = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle,"시점부하부(1)");
	CDPointArray xyArrRCStt = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle,"시점부하부(1)");
	CDPointArray xyArrLEnd  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle,"종점부하부(1)");
	CDPointArray xyArrREnd  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle,"종점부하부(1)");
	CDPointArray xyArrLCEnd = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle,"종점부하부(1)");
	CDPointArray xyArrRCEnd = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle,"종점부하부(1)");

	long nType = GetTypeBeryukAtCantilever(TRUE, FALSE);

	if(nCycle==0 && m_pDB->m_dHeightTaperStt>0)	
	{
		for (long n=0; n<3; n++)	// 0:좌측, 1 : 중앙 2: 우측
		{
			long nPos = n==0 ? POS_SLAB_LEFT : (n==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
			if (n==0 && (nType==1 || nType==3))	// 좌측이 없는 경우
				continue;
			if (n==2 && (nType==2 || nType==3))	// 우측이 없는 경우
				continue;

			CRebar *pRB = new CRebar();
			if(n==0)		pRB->m_sDescription	= "배력단부하면L1";
			else if(n==1)	pRB->m_sDescription	= "배력단부하면1";
			else if(n==2)	pRB->m_sDescription	= "배력단부하면R1";

			if(n==0)		sText.Format("BTL");
			else if(n==1)	sText.Format("BT");
			else if(n==2)	sText.Format("BTR");
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(m_SigmaY);	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= m_bInstallBeryukRebarAtBlockOut ? 13 : 79;
			pRB->m_dDia			= m_dDiaBeryukRebarLower_Dan[nPos];
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			if(!m_bInstallBeryukRebarAtBlockOut)
			{
				pRB->m_dLengthMax_A	= Round(max(max(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrRStt[0]-xyArrRStt[1])), max(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrREnd[0]-xyArrREnd[1]))), 0);
				pRB->m_dLengthMin_A = Round(min(min(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrRStt[0]-xyArrRStt[1])), min(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrREnd[0]-xyArrREnd[1]))), 0);
				pRB->m_dLengthMax_D	= Round(max(max(~(xyArrLStt[3]-xyArrLStt[4]), ~(xyArrRStt[3]-xyArrRStt[4])), max(~(xyArrLEnd[3]-xyArrLEnd[4]), ~(xyArrREnd[3]-xyArrREnd[4]))), 0);
				pRB->m_dLengthMin_D = Round(min(min(~(xyArrLStt[3]-xyArrLStt[4]), ~(xyArrRStt[3]-xyArrRStt[4])), min(~(xyArrLEnd[3]-xyArrLEnd[4]), ~(xyArrREnd[3]-xyArrREnd[4]))), 0);
			}
			else
			{
				pRB->m_dLengthMax_C	= Round(max(max(~(xyArrLStt[2]-xyArrLStt[3]), ~(xyArrRStt[2]-xyArrRStt[3])), max(~(xyArrLEnd[2]-xyArrLEnd[3]), ~(xyArrREnd[2]-xyArrREnd[3]))), 0);
				pRB->m_dLengthMin_C = Round(min(min(~(xyArrLStt[2]-xyArrLStt[3]), ~(xyArrRStt[2]-xyArrRStt[3])), min(~(xyArrLEnd[2]-xyArrLEnd[3]), ~(xyArrREnd[2]-xyArrREnd[3]))), 0);
			}

			double dMaxVal = 0;
			double dMinVal = 0;
			long nIDStt = m_bInstallBeryukRebarAtBlockOut ? 1 : 2;
			long nIDEnd = m_bInstallBeryukRebarAtBlockOut ? 2 : 3;
			if (nType == 0)
			{
				if(n==0)
				{
					dMaxVal = Round(max(max(~(xyArrLStt[nIDStt]-xyArrLStt[3]), ~(xyArrLCStt[nIDStt]-xyArrLCStt[3])), max(~(xyArrLEnd[nIDStt]-xyArrLEnd[3]), ~(xyArrLCEnd[nIDStt]-xyArrLCEnd[3]))), 0);
					dMinVal = Round(min(min(~(xyArrLStt[nIDStt]-xyArrLStt[3]), ~(xyArrLCStt[nIDStt]-xyArrLCStt[3])), min(~(xyArrLEnd[nIDStt]-xyArrLEnd[3]), ~(xyArrLCEnd[nIDStt]-xyArrLCEnd[3]))), 0);
				}
				else if(n==1)
				{
					dMaxVal = Round(max(max(~(xyArrLCStt[nIDStt]-xyArrLCStt[3]), ~(xyArrRCStt[nIDStt]-xyArrRCStt[3])), max(~(xyArrLCEnd[nIDStt]-xyArrLCEnd[3]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[3]))), 0);
					dMinVal = Round(min(min(~(xyArrLCStt[nIDStt]-xyArrLCStt[3]), ~(xyArrRCStt[nIDStt]-xyArrRCStt[3])), min(~(xyArrLCEnd[nIDStt]-xyArrLCEnd[3]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[3]))), 0);
				}
				else if(n==2)
				{
					dMaxVal = Round(max(max(~(xyArrRCStt[nIDStt]-xyArrRCStt[nIDEnd]), ~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd])), max(~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[nIDEnd]))), 0);
					dMinVal = Round(min(min(~(xyArrRCStt[nIDStt]-xyArrRCStt[nIDEnd]), ~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd])), min(~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[nIDEnd]))), 0);
				}
			}
			else if (nType == 1)
			{
				if(n==1)
				{
					dMaxVal = Round(max(max(~(xyArrLStt[nIDStt]-xyArrLStt[nIDEnd]), ~(xyArrRCStt[nIDStt]-xyArrRCStt[nIDEnd])), max(~(xyArrLEnd[nIDStt]-xyArrLEnd[nIDEnd]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[nIDEnd]))), 0);
					dMinVal = Round(min(min(~(xyArrLStt[nIDStt]-xyArrLStt[nIDEnd]), ~(xyArrRCStt[nIDStt]-xyArrRCStt[nIDEnd])), min(~(xyArrLEnd[nIDStt]-xyArrLEnd[nIDEnd]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[nIDEnd]))), 0);
				}
				else if(n==2)
				{
					dMaxVal = Round(max(max(~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd]), ~(xyArrRCStt[nIDStt]-xyArrRCStt[nIDEnd])), max(~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[nIDEnd]))), 0);
					dMinVal = Round(min(min(~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd]), ~(xyArrRCStt[nIDStt]-xyArrRCStt[nIDEnd])), min(~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]), ~(xyArrRCEnd[nIDStt]-xyArrRCEnd[nIDEnd]))), 0);
				}
			}
			else if (nType == 2)
			{
				if(n==0)
				{
					dMaxVal = Round(max(max(~(xyArrLCStt[nIDStt]-xyArrLCStt[nIDEnd]), ~(xyArrLStt[nIDStt]-xyArrLStt[nIDEnd])), max(~(xyArrLCEnd[nIDStt]-xyArrLCEnd[nIDEnd]), ~(xyArrLEnd[nIDStt]-xyArrLEnd[nIDEnd]))), 0);
					dMinVal = Round(min(min(~(xyArrLCStt[nIDStt]-xyArrLCStt[nIDEnd]), ~(xyArrLStt[nIDStt]-xyArrLStt[nIDEnd])), min(~(xyArrLCEnd[nIDStt]-xyArrLCEnd[nIDEnd]), ~(xyArrLEnd[nIDStt]-xyArrLEnd[nIDEnd]))), 0);
				}
				else if(n==1)
				{
					dMaxVal = Round(max(max(~(xyArrLCStt[nIDStt]-xyArrLCStt[nIDEnd]), ~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd])), max(~(xyArrLCEnd[nIDStt]-xyArrLCEnd[nIDEnd]), ~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]))), 0);
					dMinVal = Round(min(min(~(xyArrLCStt[nIDStt]-xyArrLCStt[nIDEnd]), ~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd])), min(~(xyArrLCEnd[nIDStt]-xyArrLCEnd[nIDEnd]), ~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]))), 0);
				}
			}
			else if (nType == 3)
			{
				if(n==1)
				{
					dMaxVal = Round(max(max(~(xyArrLStt[nIDStt]-xyArrLStt[nIDEnd]), ~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd])), max(~(xyArrLEnd[nIDStt]-xyArrLEnd[nIDEnd]), ~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]))), 0);
					dMinVal = Round(min(min(~(xyArrLStt[nIDStt]-xyArrLStt[nIDEnd]), ~(xyArrRStt[nIDStt]-xyArrRStt[nIDEnd])), min(~(xyArrLEnd[nIDStt]-xyArrLEnd[nIDEnd]), ~(xyArrREnd[nIDStt]-xyArrREnd[nIDEnd]))), 0);
				}
			}

			if(m_bInstallBeryukRebarAtBlockOut)
			{
				pRB->m_dLengthMax_B = dMaxVal;
				pRB->m_dLengthMin_B = dMinVal;
			}
			else
			{
				pRB->m_dLengthMax_C = dMaxVal;
				pRB->m_dLengthMin_C = dMinVal;
			}

			// 시점부(일단 시작부만 검사함..별 무리없음)
			double LDist  = GetSideBarDistFromCenterGir(TRUE, TRUE);
			double RDist  = GetSideBarDistFromCenterGir(TRUE, FALSE);
			double LCDist = LDist+m_dWidthSlabCantLeftForBeryuk;
			double RCDist = RDist-m_dWidthSlabCantRighForBeryuk;
			CPlateBasicIndex *pBx = m_pDB->GetGirder(0)->GetBxOnJijum(0);
			double dThickSlabL = m_pDB->GetThickSlabAndPaveByDis(pBx, LDist);
			double dThickSlabR = m_pDB->GetThickSlabAndPaveByDis(pBx, RDist);
			double dThickSlabC = m_pDB->GetThickSlabAndPaveByDis(pBx, (LDist+RDist)/2);
			double dThickSlabLC = m_pDB->GetThickSlabAndPaveByDis(pBx, LCDist);
			double dThickSlabRC = m_pDB->GetThickSlabAndPaveByDis(pBx, RCDist);
			double dThickSlabMax = 0;
			double dThickSlabMin = 0;
			if (nType == 0)
			{
				if(n==0)
				{
					dThickSlabMax = max(dThickSlabL, dThickSlabLC);   
					dThickSlabMin = min(dThickSlabL, dThickSlabLC);   
				}
				else if(n==1)
				{
					dThickSlabMax = max(dThickSlabLC, dThickSlabRC);   
					dThickSlabMin = min(dThickSlabLC, dThickSlabRC);   
				}
				else if(n==2)
				{
					dThickSlabMax = max(dThickSlabR, dThickSlabRC);   
					dThickSlabMin = min(dThickSlabR, dThickSlabRC);   
				}
			}
			else if (nType == 1)
			{
				if(n==1)
				{
					dThickSlabMax = max(dThickSlabL, dThickSlabRC);   
					dThickSlabMin = min(dThickSlabL, dThickSlabRC);   
				}
				else if(n==2)
				{
					dThickSlabMax = max(dThickSlabR, dThickSlabRC);   
					dThickSlabMin = min(dThickSlabR, dThickSlabRC);   
				}
			}
			else if (nType == 2)
			{
				if(n==0)
				{
					dThickSlabMax = max(dThickSlabL, dThickSlabLC);   
					dThickSlabMin = min(dThickSlabL, dThickSlabLC);   
				}
				else if(n==1)
				{
					dThickSlabMax = max(dThickSlabLC, dThickSlabR);   
					dThickSlabMin = min(dThickSlabLC, dThickSlabR);   
				}
			}
			else if (nType == 3)
			{
				if(n==1)
				{
					dThickSlabMax = max(max(dThickSlabL, dThickSlabC), max(dThickSlabC, dThickSlabR));   
					dThickSlabMin = min(min(dThickSlabL, dThickSlabC), min(dThickSlabC, dThickSlabR));   
				}
			}

			if(m_bInstallBeryukRebarAtBlockOut)
			{
				pRB->m_dLengthMax_A	= 100;
				pRB->m_dLengthMin_A	= 100;
			}
			else
			{
				pRB->m_dLengthMax_B	= Round(dThickSlabMax-dSttBlockOutHeight-m_dCoverUp-m_dCoverDn, 0);
				pRB->m_dLengthMin_B	= Round(dThickSlabMin-dSttBlockOutHeight-m_dCoverUp-m_dCoverDn, 0);
			}
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dEndSupportRebarCTCLower[nPos]);
			if(m_bInstallBeryukRebarAtBlockOut)
				pRB->m_dAngleX1 = 180 - (xyArrLStt[3]-xyArrLStt[2]).Unit().GetAngleDegree();	// 각은 어느위치나 동일하다고 봄
			else
				pRB->m_dAngleX1 = 180 - (xyArrLStt[4]-xyArrLStt[3]).Unit().GetAngleDegree();	// 각은 어느위치나 동일하다고 봄

			pRB->m_nEa = GetsuBeryukAtCantilever(nPos, TRUE, FALSE) * 2;			// 시, 종점 모두
			pRB->m_nIdxNumber = 1; //배력철근 	
			if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
		}
	}

	// 배력단부상면                                                         
	xyArrLStt  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle,"시점부상부");
	xyArrRStt  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle,"시점부상부");
	xyArrLCStt = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle,"시점부상부");
	xyArrRCStt = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle,"시점부상부");
	xyArrLEnd  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle,"종점부상부");
	xyArrREnd  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle,"종점부상부");
	xyArrLCEnd = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle,"종점부상부");
	xyArrRCEnd = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle,"종점부상부");
	
	nType = GetTypeBeryukAtCantilever(TRUE, TRUE);

	for (long n=0; n<3; n++)	// 0:좌측, 1 : 중앙 2: 우측
	{
		long nPos = n==0 ? POS_SLAB_LEFT : (n==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
		if (n==0 && (nType==1 || nType==3))	// 좌측이 없는 경우
			continue;
		if (n==2 && (nType==2 || nType==3))	// 우측이 없는 경우
			continue;

		CRebar *pRB = new CRebar();

		if(n==0)
		{
			if(bOneCycle)	sText.Format("배력단부상면L");
			else			sText.Format("배력단부상면L%dCycle", nCycle+1);
			pRB->m_sDescription	= sText;
			if(bOneCycle)	sText.Format("BL%d", nRebarMark);
			else			sText.Format("BL%d-%d", nRebarMark, nCycle+1);
			pRB->m_sMark		= sText;
		}
		else if(n==1)
		{
			if(bOneCycle)	sText.Format("배력단부상면");
			else			sText.Format("배력단부상면%dCycle", nCycle+1);
			pRB->m_sDescription	= sText;
			if(bOneCycle)	sText.Format("B%d", nRebarMark);
			else			sText.Format("B%d-%d", nRebarMark, nCycle+1);
			pRB->m_sMark		= sText;
		}
		else if(n==2)
		{
			if(bOneCycle)	sText.Format("배력단부상면R");
			else			sText.Format("배력단부상면R%dCycle", nCycle+1);
			pRB->m_sDescription	= sText;
			if(bOneCycle)	sText.Format("BR%d", nRebarMark);
			else			sText.Format("BR%d-%d", nRebarMark, nCycle+1);
			pRB->m_sMark		= sText;
		}
		
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = FALSE;
		pRB->m_nTypeRebar	= 2;	// ㄱ타입
		pRB->m_dDia			= m_dDiaBeryukRebar_Dan[nPos];
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(max(max(~(xyArrLStt[1]-xyArrLStt[2]), ~(xyArrRStt[1]-xyArrRStt[2])), max(~(xyArrLEnd[1]-xyArrLEnd[2]), ~(xyArrREnd[1]-xyArrREnd[2]))), 0);
		pRB->m_dLengthMin_A = Round(min(min(~(xyArrLStt[1]-xyArrLStt[2]), ~(xyArrRStt[1]-xyArrRStt[2])), min(~(xyArrLEnd[1]-xyArrLEnd[2]), ~(xyArrREnd[1]-xyArrREnd[2]))), 0);
		if (nType==0)
		{
			if(n==0)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), max(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), min(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
			}
			else if(n==1)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrLCStt[0]-xyArrLCStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrLCEnd[0]-xyArrLCEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrLCStt[0]-xyArrLCStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrLCEnd[0]-xyArrLCEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
			}
			else if(n==2)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
			}
		}
		else if (nType == 1)
		{
			if(n==1)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
			}
			else if(n==2)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
			}
		}
		else if (nType == 2)
		{
			if(n==0)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), max(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), min(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
			}
			else if(n==1)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
			}
		}
		else if (nType == 3)
		{
			if(n==1)
			{
				pRB->m_dLengthMax_B	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLStt[0]-xyArrLStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLEnd[0]-xyArrLEnd[1]))), 0);
				pRB->m_dLengthMin_B = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLStt[0]-xyArrLStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLEnd[0]-xyArrLEnd[1]))), 0);
			}
		}
		
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndSupportRebarCTC[nPos]);

		pRB->m_nEa = (long)GetRebarsuAtCycle((long)GetsuBeryukAtCantilever(nPos, TRUE, TRUE)*2, nCycle, m_stBeryukCycleLeft.size());
		pRB->m_nIdxNumber = 1; //배력철근 	
		if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
	}
	nRebarMark++;
	
	nType = GetTypeBeryukAtCantilever(TRUE, FALSE);
	// 배력단부하면 2                                                       
	if (IsInstallBeryukRebarAtDanHunch(JONGBASEPOS_SLABCENTER, TRUE, nCycle))
	{
		xyArrLStt  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle,"시점부하부(2)");
		xyArrRStt  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle,"시점부하부(2)");
		xyArrLCStt = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle,"시점부하부(2)");
		xyArrRCStt = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle,"시점부하부(2)");
		xyArrLEnd  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle,"종점부하부(2)");
		xyArrREnd  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle,"종점부하부(2)");
		xyArrLCEnd = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle,"종점부하부(2)");
		xyArrRCEnd = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle,"종점부하부(2)");

		for (long n=0; n<3; n++)	// 0:좌측, 1 : 중앙 2: 우측
		{
			long nPos = n==0 ? POS_SLAB_LEFT : (n==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
			if (n==0 && (nType==1 || nType==3))	// 좌측이 없는 경우
				continue;
			if (n==2 && (nType==2 || nType==3))	// 우측이 없는 경우
				continue;

			CRebar *pRB = new CRebar();
			if(n==0)
			{
				if(bOneCycle)	sText.Format("배력단부하면2L");
				else			sText.Format("배력단부하면2L-%dCycle", nCycle+1);
				pRB->m_sDescription	= sText;
				if(bOneCycle)	sText.Format("BL%d", nRebarMark);
				else			sText.Format("BL%d-%d", nRebarMark, nCycle+1);
				pRB->m_sMark		= sText;
			}
			else if(n==1)
			{
				if(bOneCycle)	sText.Format("배력단부하면2");
				else			sText.Format("배력단부하면2-%dCycle", nCycle+1);
				pRB->m_sDescription	= sText;
				if(bOneCycle)	sText.Format("B%d", nRebarMark);
				else			sText.Format("B%d-%d", nRebarMark, nCycle+1);
				pRB->m_sMark		= sText;
			}
			else if(n==2)
			{
				if(bOneCycle)	sText.Format("배력단부하면2R");
				else			sText.Format("배력단부하면2R-%dCycle", nCycle+1);
				pRB->m_sDescription	= sText;
				if(bOneCycle)	sText.Format("BR%d", nRebarMark);
				else			sText.Format("BR%d-%d", nRebarMark, nCycle+1);
				pRB->m_sMark		= sText;
			}
			pRB->m_Fy			= toKgPCm2(m_SigmaY);	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 1;	
			pRB->m_dDia			= m_dDiaBeryukRebarLower_Dan[nPos];
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			if (nType==0)
			{
				if(n==0)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), max(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), min(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
				}
				else if(n==1)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrLCStt[0]-xyArrLCStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrLCEnd[0]-xyArrLCEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrLCStt[0]-xyArrLCStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrLCEnd[0]-xyArrLCEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				}
				else if(n==2)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				}
			}
			else if (nType == 1)
			{
				if(n==1)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				}
				else if(n==2)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrRCStt[0]-xyArrRCStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrRCEnd[0]-xyArrRCEnd[1]))), 0);
				}
			}
			else if (nType == 2)
			{
				if(n==0)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), max(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrLStt[0]-xyArrLStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), min(~(xyArrLEnd[0]-xyArrLEnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
				}
				else if(n==1)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLCStt[0]-xyArrLCStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLCEnd[0]-xyArrLCEnd[1]))), 0);
				}
			}
			else if (nType == 3)
			{
				if(n==1)
				{
					pRB->m_dLengthMax_A	= Round(max(max(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLStt[0]-xyArrLStt[1])), max(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLEnd[0]-xyArrLEnd[1]))), 0);
					pRB->m_dLengthMin_A = Round(min(min(~(xyArrRStt[0]-xyArrRStt[1]), ~(xyArrLStt[0]-xyArrLStt[1])), min(~(xyArrREnd[0]-xyArrREnd[1]), ~(xyArrLEnd[0]-xyArrLEnd[1]))), 0);
				}
			}
			SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dEndSupportRebarCTCLower[nPos]);
			pRB->m_nEa = (long)GetRebarsuAtCycle((long)GetsuBeryukAtCantilever(nPos, TRUE, FALSE)*2, nCycle, m_stBeryukCycleLeft.size());
			pRB->m_nIdxNumber = 1; //배력철근 	
			if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
		}

		nRebarMark++;
	}
	// 일반부 상부
	CDPointArray xyArrL, xyArrLC;
	CDPointArray xyArrR, xyArrRC;
	BOOL bPehap = IsPehapCycle(JONGBASEPOS_SLABLEFT, nCycle);
	nType = GetTypeBeryukAtCantilever(FALSE, TRUE);
	
	long nJ = 0;
	if (bPehap)
	{
		for(nJ=0; nJ<m_pDB->m_nQtyJigan; nJ++)
		{
			sText.Format("일반부상부(%d)", nJ+1);
			xyArrL  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle, sText);	
			xyArrR  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle, sText);	
			xyArrLC = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle, sText);	
			xyArrRC = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle, sText);	

			if(xyArrL.GetSize()==0 || xyArrR.GetSize()==0)		continue;

			for (long n=0; n<3; n++)	// 0:좌측, 1 : 중앙 2: 우측
			{
				long nPos = n==0 ? POS_SLAB_LEFT : (n==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				if (n==0 && (nType==1 || nType==3))	// 좌측이 없는 경우
					continue;
				if (n==2 && (nType==2 || nType==3))	// 우측이 없는 경우
					continue;

				CRebar *pRB = new CRebar();
		
				if(n==0)
				{
					if(bOneCycle)	sText.Format("배력일반부상면L%d", nJ+1);
					else			sText.Format("배력일반부상면L%d-%dCycle", nJ+1, nCycle+1);
					pRB->m_sDescription	= sText;
					if(bOneCycle)	sText.Format("BL%d", nRebarMark);
					else			sText.Format("BL%d-%d", nRebarMark, nCycle+1);
					pRB->m_sMark		= sText;
				}
				else if(n==1)
				{
					if(bOneCycle)	sText.Format("배력일반부상면%d", nJ+1);
					else			sText.Format("배력일반부상면%d-%dCycle", nJ+1, nCycle+1);
					pRB->m_sDescription	= sText;
					if(bOneCycle)	sText.Format("B%d", nRebarMark);
					else			sText.Format("B%d-%d", nRebarMark, nCycle+1);
					pRB->m_sMark		= sText;
				}
				else if(n==2)
				{
					if(bOneCycle)	sText.Format("배력일반부상면R%d", nJ+1);
					else			sText.Format("배력일반부상면R%d-%dCycle", nJ+1, nCycle+1);
					pRB->m_sDescription	= sText;
					if(bOneCycle)	sText.Format("BR%d", nRebarMark);
					else			sText.Format("BR%d-%d", nRebarMark, nCycle+1);
					pRB->m_sMark		= sText;
				}
				
				pRB->m_Fy			= toKgPCm2(m_SigmaY);	
				pRB->m_bMainRebar   = FALSE;
				pRB->m_nTypeRebar	= 1;
				pRB->m_dDia			= m_dDiaBeryukRebar_Cent[nPos];
				pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				if (nType==0)
				{
					if(n==0)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0); 
						pRB->m_dLengthMin_A = Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0); 
					}
					else if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrLC[0]-xyArrLC[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrLC[0]-xyArrLC[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
					else if(n==2)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
				}
				else if (nType == 1)
				{
					if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
					else if(n==2)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
				}
				else if (nType == 2)
				{
					if(n==0)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
					}
					else if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
					}
				}
				else if (nType == 3)
				{
					if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrL[0]-xyArrL[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrL[0]-xyArrL[1])), 0);
					}
				}				
				SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dGenMainRebarCTC[nPos]/2);
				long nRealCyclesu = GetsuRealCycle(JONGBASEPOS_SLABCENTER, TRUE);
				// 사이클별로 빠지는 형태가 있어서 예외적으로 처리함.
				long nRebarsu = GetsuBeryukAtCantilever(nPos, FALSE, TRUE);
				long nRebarCur= 0;
				for (long nC=0; nC<nCycle+1; nC++)
				{
					nRebarCur = (long)Round(double(nRebarsu/(m_stBeryukCycleLeft.size()-nC)), 0);
					nRebarsu -= nRebarCur;
				}
				nRebarCur = nRebarCur*(m_stBeryukCycleLeft.size()/nRealCyclesu);
				
				pRB->m_nEa = nRebarCur;
				pRB->m_nIdxNumber = 1; //배력철근 	
				if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
			}
			nRebarMark++;
		}

		// 일반부 하부                                                          
		nType = GetTypeBeryukAtCantilever(FALSE, FALSE);
		for( nJ=0; nJ<m_pDB->m_nQtyJigan;nJ++)
		{
			sText.Format("일반부하부(%d)", nJ+1);
			xyArrL  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle, sText);	
			xyArrR  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle, sText);	
			xyArrLC = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLC,   nCycle, sText);	
			xyArrRC = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRC,   nCycle, sText);	

			if(xyArrL.GetSize()==0 || xyArrR.GetSize()==0)		continue;

			for (long n=0; n<3; n++)	// 0:좌측, 1 : 중앙 2: 우측
			{
				long nPos = n==0 ? POS_SLAB_LEFT : (n==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				if (n==0 && (nType==1 || nType==3))	// 좌측이 없는 경우
					continue;
				if (n==2 && (nType==2 || nType==3))	// 우측이 없는 경우
					continue;

				CRebar *pRB = new CRebar();
				
				if(n==0)
				{
					if(bOneCycle)	sText.Format("배력일반부하면L%d", nJ+1);
					else			sText.Format("배력일반부하면L%d-%dCycle", nJ+1, nCycle+1);
					pRB->m_sDescription	= sText;
					if(bOneCycle)	sText.Format("BL%d", nRebarMark);
					else			sText.Format("BL%d-%d", nRebarMark, nCycle+1);
					pRB->m_sMark		= sText;
				}
				else if(n==1)
				{
					if(bOneCycle)	sText.Format("배력일반부하면%d", nJ+1);
					else			sText.Format("배력일반부하면%d-%dCycle", nJ+1, nCycle+1);
					pRB->m_sDescription	= sText;
					if(bOneCycle)	sText.Format("B%d", nRebarMark);
					else			sText.Format("B%d-%d", nRebarMark, nCycle+1);
					pRB->m_sMark		= sText;
				}
				else if(n==2)
				{
					if(bOneCycle)	sText.Format("배력일반부하면R%d", nJ+1);
					else			sText.Format("배력일반부하면R%d-%dCycle", nJ+1, nCycle+1);
					pRB->m_sDescription	= sText;
					if(bOneCycle)	sText.Format("BR%d", nRebarMark);
					else			sText.Format("BR%d-%d", nRebarMark, nCycle+1);
					pRB->m_sMark		= sText;
				}
				pRB->m_Fy			= toKgPCm2(m_SigmaY);	
				pRB->m_bMainRebar   = FALSE;
				pRB->m_nTypeRebar	= 1;	
				pRB->m_dDia			= m_dDiaBeryukRebarLower_Cent[nPos];
				pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				if (nType==0)
				{
					if(n==0)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0); 
						pRB->m_dLengthMin_A = Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0); 
					}
					else if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrLC[0]-xyArrLC[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrLC[0]-xyArrLC[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
					else if(n==2)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
				}
				else if (nType == 1)
				{
					if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
					else if(n==2)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrRC[0]-xyArrRC[1])), 0);
					}
				}
				else if (nType == 2)
				{
					if(n==0)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrL[0]-xyArrL[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
					}
					else if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrLC[0]-xyArrLC[1])), 0);
					}
				}
				else if (nType == 3)
				{
					if(n==1)
					{
						pRB->m_dLengthMax_A	= Round(max(~(xyArrR[0]-xyArrR[1]), ~(xyArrL[0]-xyArrL[1])), 0);
						pRB->m_dLengthMin_A = Round(min(~(xyArrR[0]-xyArrR[1]), ~(xyArrL[0]-xyArrL[1])), 0);
					}
				}	
				SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dGenMainRebarCTCLower[nPos]/2);
				long nRealCyclesu = GetsuRealCycle(JONGBASEPOS_SLABCENTER, FALSE);
				// 사이클별로 빠지는 형태가 있어서 예외적으로 처리함.
				long nRebarsu = GetsuBeryukAtCantilever(nPos, FALSE, FALSE);
				long nRebarCur = 0;
				for (long nC=0; nC<nCycle+1; nC++)
				{
					nRebarCur = (long)Round(double(nRebarsu/(m_stBeryukCycleLeft.size()-nC)), 0);
					nRebarsu -= nRebarCur;
				}
				nRebarCur = nRebarCur*(m_stBeryukCycleLeft.size()/nRealCyclesu);

				pRB->m_nEa = nRebarCur;
				pRB->m_nIdxNumber = 1; //배력철근 	
				if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
			}
			nRebarMark++;
		}
	}
	else
		nRebarMark += m_pDB->m_nQtyJigan*2;
	// 지점부 상부                                                         
	for(nJ=0; nJ<m_pDB->m_nQtyJigan-1; nJ++)
	{
		sText.Format("지점부상부(%d)", nJ+1);
		xyArrL  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle, sText);	
		xyArrR  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle, sText);	

		if(xyArrL.GetSize()==0 || xyArrR.GetSize()==0)		continue;

		CRebar *pRB = new CRebar();
		if(bOneCycle)	sText.Format("배력지점부상면%d", nJ+1);
		else			sText.Format("배력지점부상면%d-%dCycle", nJ+1, nCycle+1);
		pRB->m_sDescription		= sText;

		if(bOneCycle)	sText.Format("B%d", nRebarMark);
		else			sText.Format("B%d-%d", nRebarMark, nCycle+1);
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	

		pRB->m_bMainRebar   = FALSE;
		pRB->m_nTypeRebar	= 1;	// - 타입
		pRB->m_dDia			= m_dDiaJijumRebarUp[0];
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[2]), ~(xyArrR[0]-xyArrR[2])), 0);
		pRB->m_dLengthMin_A	= Round(min(~(xyArrL[0]-xyArrL[2]), ~(xyArrR[0]-xyArrR[2])), 0);
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dJijumRebarCTCUp[0]);
		
		pRB->m_nEa = (long)GetRebarsuAtCycle(GetSuRebarArray(&m_arrCTCBeryukRebarJijum[CTC_BERYUK_UPPER], TRUE), nCycle, m_stBeryukCycleLeft.size());
		pRB->m_nIdxNumber = 1; //배력철근 	
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			nRebarMark++;
		}
	}

	// 지점부 하부                                                          
	for(nJ=0; nJ<m_pDB->m_nQtyJigan-1;nJ++)
	{
		sText.Format("지점부하부(%d)", nJ+1);
		xyArrL  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABLEFT, nCycle, sText);	
		xyArrR  = GetRebarShapeByJongBasePos(JONGBASEPOS_SLABRIGHT,nCycle, sText);	

		if(xyArrL.GetSize()==0 || xyArrR.GetSize()==0)		continue;

		CRebar *pRB = new CRebar();
		if(bOneCycle)	sText.Format("배력지점부하면%d", nJ+1);
		else			sText.Format("배력지점부하면%d-%dCycle", nJ+1, nCycle+1);
		pRB->m_sDescription		= sText;

		if(bOneCycle)	sText.Format("B%d", nRebarMark);
		else			sText.Format("B%d-%d", nRebarMark, nCycle+1);
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = FALSE;
		pRB->m_nTypeRebar	= 1;	// - 타입
		pRB->m_dDia			= m_dDiaJijumRebarDn[0];
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pRB->m_dLengthMax_A	= Round(max(~(xyArrL[0]-xyArrL[2]), ~(xyArrR[0]-xyArrR[2])), 0);
		pRB->m_dLengthMin_A	= Round(min(~(xyArrL[0]-xyArrL[2]), ~(xyArrR[0]-xyArrR[2])), 0);
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dJijumRebarCTCDn[0]);
		pRB->m_nEa = (long)GetRebarsuAtCycle(GetSuRebarArray(&m_arrCTCBeryukRebarJijum[CTC_BERYUK_LOWER], TRUE), nCycle, m_stBeryukCycleLeft.size());

		pRB->m_nIdxNumber = 1; //배력철근 	
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewon.Add(pRB);
			nRebarMark++;
		}
	}
}


// 철근제원생성 - 사보강철근 및 사각부 주철근
void CADeckData::MakeRebarJewon_Skew(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bSttSlab)			
{
	CString sRebar, sMark;
	CDPoint vAngStt = m_pDB->GetGirder(-1)->GetAngleStt();
	CDPoint vAngEnd = m_pDB->GetGirder(-1)->GetAngleEnd();
	double  dRatio  = bSttSlab ? fabs(vAngStt.y) : fabs(vAngEnd.y);
	BOOL bOneCycle  = m_stMainCycleSabogangStt.size()==1 ? TRUE : FALSE;
	long nCurRebar  = m_nRebarNo;

	for (long nCycle=0; nCycle < (long)m_stMainCycleSabogangStt.size(); nCycle++)
	{
		m_nRebarNo = nCurRebar;
		for (long nUpDn=0; nUpDn<2; nUpDn++)
		{
			BOOL bUpper = nUpDn==0 ? TRUE : FALSE;
			vector<REBAR_SHAPE> vShapeSkew = GetNewRebarShapeArrBySection(bSttSlab ? SECTION_SKEW_STT : SECTION_SKEW_END, nCycle, nUpDn==0?TRUE:FALSE);
			
			long nShapesu = vShapeSkew.size();
			for (long n=0; n<nShapesu; n++)
			{
				CDPointArray xyArrRebar = vShapeSkew[n].m_dArrShape;

				CRebar *pRB = new CRebar;
				if (nUpDn==0)
				{
					if(bSttSlab)
					{
						if(bOneCycle)		sRebar.Format("시점사보강상면%d", n+1);
						else				sRebar.Format("시점사보강상면%d-%dCycle", n+1, nCycle+1);
					}
					else
					{
						if(bOneCycle)		sRebar.Format("종점사보강상면%d", n+1);
						else				sRebar.Format("종점사보강상면%d-%dCycle", n+1, nCycle+1);
					}
				}
				else
				{
					if(bSttSlab)
					{
						if(bOneCycle)		sRebar.Format("시점사보강하면%d", n+1);
						else				sRebar.Format("시점사보강하면%d-%dCycle", n+1, nCycle+1);
					}
					else
					{
						if(bOneCycle)		sRebar.Format("종점사보강하면%d", n+1);
						else				sRebar.Format("종점사보강하면%d-%dCycle", n+1, nCycle+1);
					}
				}
				pRB->m_sDescription = sRebar;
				if(bOneCycle)		sMark.Format("%d", m_nRebarNo);	
				else				sMark.Format("%d-%d", m_nRebarNo, nCycle+1);	
				pRB->m_sMark		= sMark;
				pRB->m_Fy			= toKgPCm2(m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;

				if (nUpDn==0)
				{
					if(nShapesu==1)
					{
						pRB->m_nTypeRebar   = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 3;
						pRB->m_dDia			= vShapeSkew[n].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()-1].x)/dRatio,0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[0]/2);
					}
					else if(nShapesu==2)
					{
						pRB->m_nTypeRebar = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 2;
						pRB->m_dDia		  = vShapeSkew[n].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x)/dRatio,0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[n==0 ? 0 : 1]/2);
					}
					else
					{
						if(n==0 || n==nShapesu-1)
						{
							pRB->m_nTypeRebar = 2;
							pRB->m_dDia		  = vShapeSkew[n].m_dDia;
							pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x)/dRatio,0);
							pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
							SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[n==0 ? 0 : 1]/2);
						}
						else
						{
							pRB->m_nTypeRebar = 1;
							pRB->m_dDia		  = vShapeSkew[n].m_dDia;
							pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[0].x-xyArrRebar[xyArrRebar.GetUpperBound()].x),0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar)/dRatio;
							SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[2]/2);
						}
					}
					pRB->m_nEa = (long)GetRebarsuAtCycle(GetSuRebarArray(&m_arrCTCSabogangRebar[bSttSlab?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_UPPER_END], TRUE), nCycle, m_stMainCycleSabogangStt.size());
					pRB->m_nIdxNumber = 0; //주철근
				}
				else
				{
					if(nShapesu==1)
					{
						pRB->m_nTypeRebar   = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 3;
						pRB->m_dDia			= vShapeSkew[n].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()-1].x)/dRatio,0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[0]/2);
					}
					else if(nShapesu==2)
					{
						pRB->m_nTypeRebar = m_nDetailMainUpper==(bUpper?0:1) ? 1 : 2;
						pRB->m_dDia		  = vShapeSkew[n].m_dDia;
						pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x)/dRatio,0);
						pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
						pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
						SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[n==0 ? 0 : 1]/2);
					}
					else
					{
						if(n==0 || n==nShapesu-1)
						{
							pRB->m_nTypeRebar = 2;
							pRB->m_dDia		  = vShapeSkew[n].m_dDia;
							pRB->m_dLengthMax_A	= Round(fabs(xyArrRebar[1].x-xyArrRebar[xyArrRebar.GetUpperBound()].x)/dRatio,0);
							pRB->m_dLengthMax_B	= Round(m_nDetailMainUpper==(bUpper?0:1) ? 0 : ~(xyArrRebar[0]-xyArrRebar[1]),0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
							SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[n==0 ? 0 : 1]/2);
						}
						else
						{
							pRB->m_nTypeRebar = 1;
							pRB->m_dDia		  = vShapeSkew[n].m_dDia;
							pRB->m_dLengthMax_A	=Round(fabs(xyArrRebar[0].x-xyArrRebar[xyArrRebar.GetUpperBound()].x)/dRatio,0);
							pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
							SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTCLower[2]/2);
						}
					}
					pRB->m_nEa = (long)GetRebarsuAtCycle(GetSuRebarArray(&m_arrCTCSabogangRebar[bSttSlab?CTC_SABOGANG_LOWER_STT:CTC_SABOGANG_LOWER_END], TRUE), nCycle, m_stMainCycleSabogangStt.size());
					pRB->m_nIdxNumber = 0; //주철근
				}
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewon.Add(pRB);
					m_nRebarNo++;
				}
			}
		}	
	}
	
}




//////////////////////////////////////////////////////////////////////////
// 철근제원생성 - 사각부 주철근
// 상부철근은 직선
// 하부철근은 ㄱ형태
// 추후 옵션처리할것
void CADeckData::MakeRebarJewon_SkewMainRebar(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bSttSlab)			
{
	CString sText;
	CTwinVectorArray tvArrPlane;	
	//////////////////////////////////////////////////////////////////////////
	// 사각부 주철근 상면
	tvArrPlane.RemoveAll();

	long nRebarNo =1;
	for(long n=0; n<m_nRebarNo+1; n++)
		nRebarNo = max(nRebarNo, m_nRebarNo);


	vector <REBAR_PLAN> *vPlanUp = GetMainRebarArrAtSkew(bSttSlab, TRUE, DRAW_SIDEALL);
	long nSize = vPlanUp->size();	
	double dLengthMax = 0;
	double dLengthMin = 10000000;
	if(nSize > 1)
	{
		for(long nn=0; nn<nSize; nn++)
		{
			double dLen = ~(vPlanUp->at(nn).m_tVector.m_v1-vPlanUp->at(nn).m_tVector.m_v2);
			dLengthMax = max(dLengthMax, dLen);
			dLengthMin = min(dLengthMin, dLen);
		}

		//APLATE-704
		long nCntCycle = m_stMainCycleDan.size(); //앞뒤 사이클은 같음
		BOOL bOneCycle = nCntCycle ==1 ? TRUE : FALSE;

		for (long nCycle =0; nCycle < nCntCycle; nCycle++)
		{
			for(long nUpDn =0; nUpDn < 2; nUpDn++)
			{
				BOOL bUpper = nUpDn == 0? TRUE : FALSE;
				vector<REBAR_SHAPE> vShape = GetNewRebarShapeArrBySection(SECTION_DAN, nCycle, bUpper);

				long nRebarsu = vShape.size();
				for (long n=0; n<nRebarsu; n++)
				{
					REBAR_SHAPE ShapeRebar	= vShape[n];
					CDPointArray xyArr				= ShapeRebar.m_dArrShape;
					if(ShapeRebar.m_bValid==FALSE)	continue;

					vector <REBAR_PLAN> *vPlanM = GetMainRebarArrAtSkew(bSttSlab, bUpper, DRAW_SIDEALL );
					if(vPlanM == NULL || vPlanM->size() == 0) continue;

					CTwinVectorArray tvArrMainRebarAtSkew;
					GetTwinVectorArrByCycleAndShape(vPlanM, &tvArrMainRebarAtSkew, nCycle, n);
					if(tvArrMainRebarAtSkew.GetSize() == 0 ) continue;

					double dMaxA = 0, dMinA = 0, dCurrLen =0;
					for (long ix =0; ix < tvArrMainRebarAtSkew.GetSize(); ix++)
					{
						CTwinVector tvRebar = tvArrMainRebarAtSkew.GetAt(ix);
						dCurrLen = tvRebar.GetLength();

						if(ix == 0)
						{
							dMaxA = dMinA = dCurrLen;
							continue;
						}

						dMaxA = max(dCurrLen, dMaxA);
						dMinA = min(dCurrLen, dMinA);
					}
					if(Compare(dMaxA, dMinA, _T("="),0.1))
					{
						dMinA = 0;
					}

					double dLengthMax = dMaxA;
					double dLengthMin = dMinA;

					CRebar *pRB = new CRebar;
					if (bUpper)
					{
						if(bOneCycle)		bSttSlab? sText.Format("시점사각주철근상면%d", n+1) : sText.Format("종점사각주철근상면%d", n+1);
						else				bSttSlab? sText.Format("시점사각주철근상면%d-%dCycle", n+1, nCycle+1) : sText.Format("종점사각주철근상면%d-%dCycle", n+1, nCycle+1);
					}
					else
					{
						if(bOneCycle)		bSttSlab? sText.Format("시점사각주철근하면%d", n+1) : sText.Format("종점사각주철근하면%d", n+1); 
						else				bSttSlab? sText.Format("시점사각주철근하면%d-%dCycle", n+1, nCycle+1) : sText.Format("종점사각주철근하면%d-%dCycle", n+1, nCycle+1);
					}


					pRB->m_sDescription = sText;
					if(bOneCycle)		sText.Format("%d", nRebarNo);	
					else					sText.Format("%d-%d", nRebarNo, nCycle+1);	
					pRB->m_sMark				= sText;
					pRB->m_Fy					=  toKgPCm2(m_SigmaY);	
					pRB->m_bMainRebar		= TRUE;
					pRB->m_dDia				= ShapeRebar.m_dDia;
					pRB->m_nTypeRebar		= 1;
					pRB->m_dRateAdd			= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
					pRB->m_dLengthMax_A	= Round(dLengthMax,0);
					pRB->m_dLengthMin_A	= Round(dLengthMin,0);

					SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_dEndMainRebarCTC[0]/2);

					pRB->m_nEa = tvArrMainRebarAtSkew.GetSize();
					pRB->m_nIdxNumber = 0; //주철근

					if(pRB->m_nEa > 0)
					{
						pArrJewon->Add(pRB);
						m_nRebarNo = nRebarNo;
						nRebarNo++;
					}


				}
			}
		}
	}

}


// 철근제원생성 - 가로보 보강철근(C1, C2)	
void CADeckData::MakeRebarJewon_CrossBeam(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon)		
{
	if(m_pDB->GetGirdersu() == 1)	return;
	if(!IsBridgeHasCrossBeamHunch())	return;

	//////////////////////////////////////////////////////////////////////////
	// 가로보 헌치보강철근 C1 (일반부 첫번 가로보 철근으로 다른것도 동일적용함)
	CDPoint A[10];
	CDPoint vY(0,1);
	CDPoint xyResultFir(0,0), xyResultSec(0,0);// 슬래브 변화

	CPlateBasicIndex *pBx = m_pDB->GetGirder(0)->GetBxCrossBeamHasHunch();
	CPlateCrossBeam  *pC  = pBx ? pBx->GetCrossBeam() : NULL; 

	if(pC==NULL)	return;
	
	pBx->GetGirder()->GetHunchXyDom(pBx, A, FALSE);
	GetXyMatchSegAndLine(A[4], A[5], A[3], vY, xyResultFir);

	double dChgSlabFirH = pC ? xyResultFir.y-A[3].y : 0;
	double dChgSlabSecH = 0;
	GetXyMatchSegAndLine(A[4], A[5], A[6], vY, xyResultSec);
	dChgSlabSecH = xyResultSec.y-A[6].y;
	// 경사길이
	double dAngleLStt = sqrt(10.0)*(dChgSlabFirH+m_pDB->m_dThickSlabBase-m_pDB->m_dThickSlabHunch-m_dCoverUp-m_dCoverDn);
	double dAngleLEnd = sqrt(10.0)*(dChgSlabSecH+m_pDB->m_dThickSlabBase-m_pDB->m_dThickSlabHunch-m_dCoverUp-m_dCoverDn);

	long nSumCrRebar = 0;
	for(long n=0; n<m_arrCTCBogangRebarAtCrossBeam.GetSize(); n++)
	{
		long nGugansu = (long)m_arrCTCBogangRebarAtCrossBeam[n].x;
		nSumCrRebar += nGugansu>0 ? (nGugansu+1) : 0;
	}
	
	CRebar *pRB = new CRebar();

	pRB->m_sDescription  = "가로보헌치보강";
	pRB->m_sMark		= "C1";
	pRB->m_Fy			= toKgPCm2(m_SigmaY);	
	pRB->m_bMainRebar   = FALSE;
	pRB->m_nTypeRebar	= 9;	// V형
	pRB->m_dDia			= m_dDiaCrossBeam1;
	pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	
	pRB->m_dLengthMax_A	= Round(pC->m_uM_CR*2 + m_pDB->m_dWidthHunch*2, 0);
	pRB->m_dLengthMax_B	= Round(max(dAngleLStt, dAngleLEnd), 0);
	pRB->m_dLengthMin_B	= Round(min(dAngleLStt, dAngleLEnd), 0);
	pRB->m_nEa			= nSumCrRebar;
	pRB->m_nIdxNumber = 3; //보강철근 
	if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);	

	//////////////////////////////////////////////////////////////////////////
	// 가로보하면보강  C2
	long nSumCr = 0;	
	double dLengthC2 = 0;	// 브라켓 플랜지 끝단에서 끝단까지
	double dMaxC2=0, dMinC2=0;
	for (long nG=0; nG<m_pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{		
			if(pGir->IsCrossBeamBxHasHunch(pBx))
			{
				dLengthC2 = pGir->GetDisFrontPlateWebToWeb(pBx)-pGir->GetLengthSharpOnBx(pBx, TRUE)-pGir->m_pRight->GetLengthSharpOnBx(pBx, TRUE)
							  -pGir->GetThickFactByStation(pBx->GetStation(), G_W)-pGir->m_pRight->GetThickFactByStation(pBx->GetStation(), G_W);	
				dLengthC2 += m_dLengthCrossBeamBogangLower*2;
				dMaxC2 = max(dLengthC2, dMaxC2);
				dMinC2 = max(dLengthC2, dMinC2);

				nSumCr++;
			}
			pBx = Finder.GetBxNext();
		}
	}

	pRB = new CRebar();

	pRB->m_sDescription  = "가로보하면보강";
	pRB->m_sMark		= "C2";
	pRB->m_Fy			= toKgPCm2(m_SigmaY);	
	pRB->m_bMainRebar   = FALSE;
	pRB->m_nTypeRebar	= 1;	
	pRB->m_dDia			= m_dDiaCrossBeam2;
	pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	pRB->m_dLengthMax_A	= Round(dMaxC2, 0);
	pRB->m_dLengthMin_A	= Round(dMinC2, 0);
	SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, pC->m_uM_CR*2);
	pRB->m_nEa			= nSumCr*2;
	pRB->m_nIdxNumber = 3; //보강철근 
	if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);	
	
	//////////////////////////////////////////////////////////////////////////
	// 가로보상면보강 C3
	if(m_bInstallCrossBeamUpperBogangRebar)
	{
		pRB = new CRebar();

		pRB->m_sDescription  = "가로보상면보강";
		pRB->m_sMark		= "C3";
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = FALSE;
		pRB->m_nTypeRebar	= 1;	
		pRB->m_dDia			= m_dDiaCrossBeam3;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		
		pRB->m_dLengthMax_A	= Round(m_dLengthCrossBeamBogang, 0);
		pRB->m_nEa			= nSumCrRebar;
		pRB->m_nIdxNumber = 3; //보강철근 
		if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
	}
	
}


// 철근제원생성 - 세로보 보강철근(S1, S2)
void CADeckData::MakeRebarJewon_Stringer(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon)			
{
/*	if(m_pDB->GetGirdersu() == 1)	return;
	if(m_arrStringer.GetSize() < 1)	return;

	//////////////////////////////////////////////////////////////////////////
	//  세로보 헌치보강철근 (첫번째 세로보의 제원으로 길이 산출)            
	double dMaxA=0, dMaxB=0, dMaxC=0;
	double dMinA=10000, dMinB=10000, dMinC=10000;
	
	CString sText;
	CDoubleArray arrLengthStg;
	CStringer *pStg = GetStgByStgNo(0);

	// 슬래브 변화
	CDPoint A[10];
	CDPoint vY(0,1);
	CDPoint xyResultFir(0,0), xyResultSec(0,0);
	CPlateBasicIndex *pBx = pStg->GetBx();
	CPlateCrossBeam  *pC  = pBx->GetCrossBeam();

	if(pC)
	{
		pBx = pBx->GetBxMatchByCrossBeam(pBx->GetGirder());
		pBx->GetGirder()->GetHunchXyDom(pBx, A, FALSE);
		GetXyMatchSegAndLine(A[4], A[11], A[3], vY, xyResultFir);
	}

	double dChgSlabFirH = pC ? xyResultFir.y-A[3].y : 0;
	GetXyMatchSegAndLine(A[4], A[11], A[12], vY, xyResultSec);
	double dChgSlabSecH = xyResultSec.y-A[12].y;

	// 경사길이
	double dAngleLStt = sqrt(10)*(dChgSlabFirH+m_pDB->m_dThickSlabBase-m_pDB->m_dThickSlabHunch-m_dCoverUp-m_dCoverDn);
	double dAngleLEnd = sqrt(10)*(dChgSlabSecH+m_pDB->m_dThickSlabBase-m_pDB->m_dThickSlabHunch-m_dCoverUp-m_dCoverDn);
	long nSumStRebar = 0;
	for(long n=0; n<m_arrCTCBogangRebarAtStringer.GetSize(); n++)
		nSumStRebar += m_arrCTCBogangRebarAtStringer[n].x;
			
	CRebar *pRB = new CRebar();

	pRB->m_sDescription  = "세로보헌치보강";
	pRB->m_sMark		= "S1";
	pRB->m_Fy			= toKgPCm2(m_SigmaY);	
	pRB->m_bMainRebar   = FALSE;
	pRB->m_nTypeRebar	= 9;	// V형
	pRB->m_dDia			= m_dDiaStringer1;
	pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	
	pRB->m_dLengthMax_A	= Round(pStg->m_uM*2, 0);
	pRB->m_dLengthMax_B	= Round(max(dAngleLStt, dAngleLEnd), 0);
	pRB->m_dLengthMin_A	= Round(pStg->m_uM*2, 0);
	pRB->m_dLengthMin_B	= Round(min(dAngleLStt, dAngleLEnd), 0);
	pRB->m_nEa			= nSumStRebar;
	SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_arrCTCBogangRebarAtStringer[0].y);
	pRB->m_nIdxNumber = 3; //보강철근 
	m_arrRebarJewon.Add(pRB);	

	//////////////////////////////////////////////////////////////////////////
	// 세로보상면보강
	if(m_bInstallStringerUpperBogangRebar)
	{
		pRB = new CRebar();

		pRB->m_sDescription  = "세로보상면보강";
		pRB->m_sMark		= "S3";
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = FALSE;
		pRB->m_nTypeRebar	= 1;	// V형
		pRB->m_dDia			= m_dDiaStringer3;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		
		pRB->m_dLengthMax_A	= Round(m_dLengthStringerBogang, 0);
		pRB->m_nEa			= nSumStRebar;
		SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, m_arrCTCBogangRebarAtStringer[0].y);
		pRB->m_nIdxNumber = 3; //보강철근 
		m_arrRebarJewon.Add(pRB);
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 세로보하면보강 (교축방향)                                                
	pRB = new CRebar();

	pRB->m_sDescription  = "세로보하면보강";
	pRB->m_sMark		= "S2";
	pRB->m_Fy			= toKgPCm2(m_SigmaY);	
	pRB->m_bMainRebar   = FALSE;
	pRB->m_nTypeRebar	= 1;	// V형
	pRB->m_dDia			= m_dDiaStringer2;
	pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	pRB->m_nEa			= (m_pDB->GetGirdersu()-1)*2;//m_ArrStgBogang.GetSize()*2;
	//////////////////////////////////////////////////////////////////////////
	// 세로보 하면 보강철근의 길이계산
	double dDiaStg	= m_dDiaStringer2;
	double dSettle	= GetValueSettle(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaStg, JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(pC->m_uM_CR*2));
	CVectorArray vArr;	// x=nG, y=nStg, z=Length
	long nMaxStg = 0;
	for(long nG=0; nG < m_pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(m_pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CPlateCrossBeam *pC = pBx->GetCrossBeam();

			if(pC && pC->m_nQtyStringer_CR > 0)
			{
				double dLSumStg = 0;
				nMaxStg = max(nMaxStg, pC->m_nQtyStringer_CR);

				for(long nStg=0; nStg<pC->m_nQtyStringer_CR; nStg++)
				{
					CStringer *pStg = pC->GetBx()->GetStringer(nStg);
					if(pStg)	vArr.Add(CVector(nG, nStg, GetStgLengthAtSlab(pStg)));
				}
			}
			pBx = Finder.GetBxNext();
		}
	}
	
	double dMaxLength = 0;
	double dMinLength = 100000000;
	if(vArr.GetSize() > 0)
	{
		for(long nG=0; nG < m_pDB->GetGirdersu()-1; nG++)
		{
			for(long nStg=0; nStg < nMaxStg; nStg++)
			{
				double dLStg = 0;
				for(long n=0; n<vArr.GetSize(); n++)
				{
					if(vArr[n].x == nG && vArr[n].y == nStg)
					{
						dLStg += vArr[n].z;
					}
				}
				dMaxLength = max(dMaxLength, dLStg);
				dMinLength = min(dMinLength, dLStg);
			}
		}
	}

	pRB->m_dLengthMax_A = Round(dMaxLength+dSettle*2, 0);
	pRB->m_dLengthMin_A = Round(dMinLength+dSettle*2, 0);
	SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, pStg->m_uM*2);
	pRB->m_nIdxNumber = 3; //보강철근 
	m_arrRebarJewon.Add(pRB);	
	
*/
}




// 철근제원생성 - 외측가로보 보강철근(OB1, OB2, OB3)
void CADeckData::MakeRebarJewon_OutBrk(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon)		
{
/*	// 외측가로보 헌치보강철근 OB1 (일반부 첫번 외측가로보 철근으로 다른것도 동일적용함)
	CDPoint A[10];
	CDPoint vY(0,1);
	CDPoint xyResultFir(0,0), xyResultSec(0,0);// 슬래브 변화

	CPlateBasicIndex *pBx = GetBxBySection(SECTION_CEN, 0);
	BOOL bRightOnly = m_pDB->GetLBracketsu()==0 ? TRUE : FALSE;	// 우측캔틸레버부에만 외측가로보가 있는 경우.
	BOOL bLeft = bRightOnly ? FALSE : TRUE;	// 기본제원길이를 어느쪽을 사용할지 결정..
	long nG = bLeft ? 0 : m_pDB->GetGirdersu()-1;
	CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
	CBracket *pBrk = pGir->GetBxByStationDir(pBx->GetStation(), 0, bLeft?BX_OUTSIDE_CROSS_LEFT:BX_OUTSIDE_CROSS_RIGHT)->GetBracket(bLeft);

	if(pBrk)
	{
		pDB->GetXyFrontCantilever(pBx, A, bLeft);
		GetXyMatchSegAndLine(A[3], A[4], A[2], vY, xyResultFir);
	}

	double dChgSlabFirH = pBrk ? xyResultFir.y-A[2].y : 0;
	double dChgSlabSecH = 0;
	GetXyMatchSegAndLine(A[3], A[4], A[5], vY, xyResultSec);
	dChgSlabSecH = xyResultSec.y-A[5].y;
	// 경사길이
	double dAngleLStt = sqrt(10)*(dChgSlabFirH+m_pDB->m_dThickSlabBase-m_pDB->m_dThickSlabHunch-m_dCoverUp-m_dCoverDn);
	double dAngleLEnd = sqrt(10)*(dChgSlabSecH+m_pDB->m_dThickSlabBase-m_pDB->m_dThickSlabHunch-m_dCoverUp-m_dCoverDn);

	long nSumObkRebar = 0;
	for(long n=0; n<m_arrCTCBogangRebarAtOutBrk.GetSize(); n++)
	{
		long nGugansu = m_arrCTCBogangRebarAtOutBrk[n].x;
		nSumObkRebar += nGugansu>0 ? (nGugansu+1) : 0;
	}
	
	CRebar *pRB = new CRebar();

	pRB->m_sDescription = "외측가로보헌치보강";
	pRB->m_sMark		= "OB1";
	pRB->m_Fy			= toKgPCm2(m_SigmaY);	
	pRB->m_bMainRebar   = FALSE;
	pRB->m_nTypeRebar	= 9;	// V형
	pRB->m_dDia			= m_dDiaOutBrk1;
	pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	
	pRB->m_dLengthMax_A	= Round(pBrk->m_uM_EB*2 + m_pDB->m_dWidthHunch*2, 0);
	pRB->m_dLengthMax_B	= Round(max(dAngleLStt, dAngleLEnd), 0);
	pRB->m_dLengthMin_B	= Round(min(dAngleLStt, dAngleLEnd), 0);
	pRB->m_nEa			= nSumObkRebar;
	pRB->m_nIdxNumber = 3; //보강철근 
	m_arrRebarJewon.Add(pRB);	

	//////////////////////////////////////////////////////////////////////////
	// 외측가로보하면보강  OB2
	long nSumObk = 0;	
	double dLengthOB2 = 0;	// 브라켓 플랜지 끝단에서 끝단까지
	double dMaxOB2=0, dMinOB2=0;
	pGir = m_pDB->GetGirder(0);
	CPlateBxFinder Finder(pGir);
	pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_LEFT);
	while(pBx)
	{		
		if(pGir->GetHeightTaper(pBx->GetStation())==0)
		{
			CDPoint xyStt = pGir->GetXyPlanSlabOutBrkBase(pBx, TRUE, TRUE);
			CDPoint xyEnd = pGir->GetXyPlanSlabOutBrkBase(pBx, TRUE, FALSE);
			dLengthOB2 = ~(xyStt-xyEnd);
			dLengthOB2 += m_dLengthOutBrkBogangLower*2;
			dMaxOB2 = max(dLengthOB2, dMaxOB2);
			dMinOB2 = max(dLengthOB2, dMinOB2);

			nSumObk++;
		}
		pBx = Finder.GetBxNext();
	}
	CPlateGirderApp *pGirEnd = m_pDB->GetGirder(m_pDB->GetGirdersu()-1);
	CPlateBasicIndex FinderR(pGirEnd);
	pBx = FinderR.GetBxFirst(BX_OUTSIDE_CROSS_RIGHT);
	while(pBx)
	{		
		if(pGir->GetHeightTaper(pBx->GetStation())==0)
		{
			CDPoint xyStt = pGirEnd->GetXyPlanSlabOutBrkBase(pBx, FALSE, TRUE);
			CDPoint xyEnd = pGirEnd->GetXyPlanSlabOutBrkBase(pBx, FALSE, FALSE);
			dLengthOB2 = ~(xyStt-xyEnd);
			dLengthOB2 += m_dLengthOutBrkBogangLower*2;
			dMaxOB2 = max(dLengthOB2, dMaxOB2);
			dMinOB2 = max(dLengthOB2, dMinOB2);

			nSumObk++;
		}
		pBx = FinderR.GetBxNext();
	}

	pRB = new CRebar();

	pRB->m_sDescription  = "외측가로보하면보강";
	pRB->m_sMark		= "OB2";
	pRB->m_Fy			= toKgPCm2(m_SigmaY);	
	pRB->m_bMainRebar   = FALSE;
	pRB->m_nTypeRebar	= 1;	
	pRB->m_dDia			= m_dDiaOutBrk2;
	pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	pRB->m_dLengthMax_A	= Round(dMaxOB2, 0);
	pRB->m_dLengthMin_A	= Round(dMinOB2, 0);
	SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, pBrk->m_uM_EB*2);
	pRB->m_nEa			= nSumObk*2;
	pRB->m_nIdxNumber	= 3; //보강철근 
	m_arrRebarJewon.Add(pRB);	
	
	//////////////////////////////////////////////////////////////////////////
	// 외측가로보상면보강 OB3
	if(m_bInstallOutBrkUpperBogangRebar)
	{
		pRB = new CRebar();

		pRB->m_sDescription  = "외측가로보상면보강";
		pRB->m_sMark		= "OB3";
		pRB->m_Fy			= toKgPCm2(m_SigmaY);	
		pRB->m_bMainRebar   = FALSE;
		pRB->m_nTypeRebar	= 1;	
		pRB->m_dDia			= m_dDiaOutBrk3;
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		
		pRB->m_dLengthMax_A	= Round(m_dLengthOutBrkBogang, 0);
		pRB->m_nEa			= nSumObkRebar;
		pRB->m_nIdxNumber = 3; //보강철근 
		m_arrRebarJewon.Add(pRB);
	}*/
}


// 철근제원생성 - 간격재, 전단보강철근, 배수구 보강철근
void CADeckData::MakeRebarJewon_Etc(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon)				
{
	/***********************************************************************/
	/*  간격재                                                              */
	/************************************************************************/
	CRebar *pRB = new CRebar();

	pRB->m_sDescription  = "간격재";
	pRB->m_sMark		= "ST";
	pRB->m_Fy			= m_SpacerRebar.GetFy();	
	pRB->m_bMainRebar   = FALSE;
	pRB->m_nTypeRebar	= 85;	// V형
	pRB->m_dDia			= m_SpacerRebar.GetRebarDiameter();
	pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

	pRB->m_dLengthMax_A	= Round(m_SpacerRebar.GetRebarLength_A(), 0);
	pRB->m_dLengthMax_B	= Round(m_SpacerRebar.GetRebarLength_B(), 0);
	double dLenA = pow(pRB->m_dLengthMax_A, 2);
	double dLenC = pow(m_SpacerRebar.GetRebarLength_C(), 2);
	pRB->m_dLengthMax_C = Round(sqrt(dLenA + dLenC), 0);  //최대값
	dLenC = pow(m_SpacerRebar.GetRebarLength_E(), 2);
	pRB->m_dLengthMin_C = Round(sqrt(dLenA + dLenC), 0);  //최소값 
	pRB->m_dLengthMax_D	= Round(m_SpacerRebar.GetRebarLength_D(), 0);

	double dCnt = m_SpacerRebar.GetRebarCntInSquare();
	double dMain = m_dGenMainRebarCTC[0]/2*m_SpacerRebar.GetCntMainRebarSet();	
	double dDist = m_dGenSupportRebarCTC[0]*m_SpacerRebar.GetCntDistRebarSet();
	double dTotal = (GetAreaSlab()/(dMain*dDist))*dCnt;

	pRB->m_nEa	= (long)dTotal;
	pRB->m_nIdxNumber = 2; //기타철근 
	if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);

	/************************************************************************/
	/*  전단보강철근                                                        */
	/************************************************************************/
	if(m_bInstallShearRebar)
	{
		pRB = new CRebar();

		pRB->m_sDescription  = "전단보강철근";
		pRB->m_sMark		= "S";
		pRB->m_Fy			= m_ShearRebarDanbu.GetFy();	
		pRB->m_bMainRebar   = FALSE;
		pRB->m_nTypeRebar	= 80;	// V형
		pRB->m_dDia			= m_ShearRebarDanbu.GetDiaRFBar();  //직경은 두 케이스가 같다고 가정 
		pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

		int nEndSize = m_ShearRebarDanbu.GetSize();
		int nMidSize = m_ShearRebarJijum.GetSize();
		CSupportRFBar* pSptRFBar;
		if(nEndSize > 0)
			pSptRFBar = m_ShearRebarDanbu.GetAt(0);
		else if(nMidSize > 0)
			pSptRFBar = m_ShearRebarJijum.GetAt(0);

		if(nEndSize + nMidSize > 0)
		{
			pRB->m_dLengthMax_A	= Round(pSptRFBar->GetLineLength(), 0);
			pRB->m_dLengthMax_B = Round(pSptRFBar->GetArcLength(), 0);
			pRB->m_dAngleX1 = Round(2*(90 - pSptRFBar->GetArcAngle()/2), 0);
			pRB->m_R = Round(pSptRFBar->GetArcRadius(), 0);
			//단지점부개수 = 단지점부개수*2(양사이드)*거더당 4개 
			int nCntEnd = (m_ShearRebarDanbu.GetSize()*2)*(m_pDB->GetGirdersu()*2);
			//중간지점부개수 = 지간수*중간지점부개수*2(양사이드)*거더당 2개 
			int nCntMid = 0;
			if(m_pDB->m_nQtyJigan > 1)
				nCntMid = (m_pDB->m_nQtyJigan-1)*(m_ShearRebarJijum.GetSize()*2)*(m_pDB->GetGirdersu());  
			pRB->m_nEa	= nCntEnd + nCntMid;
			pRB->m_nIdxNumber = 2; //기타철근 
			if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
		}
	}

	/************************************************************************/
	/*  배수구 보강철근                                                     */
	/************************************************************************/
	if(m_bInstallWaterHole)
	{
		pRB = new CRebar();

		pRB->m_sDescription  = "배수구보강철근";
		pRB->m_sMark		= "B";
		pRB->m_Fy			= m_WaterHoleRoundType.GetFy();	
		pRB->m_bMainRebar   = FALSE;

		if(m_nTypeWaterHole == 0)
		{
			pRB->m_nTypeRebar	= 80;	// V형
			pRB->m_dDia			= m_WaterHoleRoundType.GetDiaRebar();
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			
			pRB->m_dLengthMax_A	= Round(m_WaterHoleRoundType.GetLengthLine(), 0);
			pRB->m_dLengthMax_B	= Round(m_WaterHoleRoundType.GetLengthArc(), 0);
			pRB->m_dAngleX1 = Round(2*(90 - m_WaterHoleRoundType.GetAngle()/2), 0);
			pRB->m_R = Round(m_WaterHoleRoundType.GetRadius(), 0);
			pRB->m_nEa			= (m_WaterHoleRoundType.GetCntRebar()*2*m_nWaterHolesu)*m_nWaterHoleDansu;
		}
		else
		{
			pRB->m_nTypeRebar	= 1;	// -형 
			pRB->m_dDia			= m_WaterHoleBoxType.GetDiaRebar();
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			
			pRB->m_dLengthMax_A	= Round(m_WaterHoleBoxType.GetLengthRebar(), 0);
			pRB->m_nEa			= (m_WaterHoleBoxType.GetCntRebar()*8*m_nWaterHolesu)*m_nWaterHoleDansu;
		}

		pRB->m_nIdxNumber = 2; //기타철근 
		if(pRB->m_nEa > 0)	m_arrRebarJewon.Add(pRB);
	}
}



// 일단은 Skew는 고려하지 않음. - 추후 보강할것
// 철근배치옵션도 아직 미고려됨
double CADeckData::GetSttStationMainRebar(long nPos, BOOL bUpper)
{
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);

	long nJongBasePos = 0;
	if(nPos==POS_SLAB_LEFT)				nJongBasePos = JONGBASEPOS_SLABLEFT;
	else if(nPos==POS_SLAB_RIGH)		nJongBasePos = JONGBASEPOS_SLABRIGHT;
	else if(nPos==POS_SLAB_CENTER)		nJongBasePos = JONGBASEPOS_SLABCENTER;
	
	double dCoverSdStt      = GetLengthCoverSd(TRUE, nJongBasePos); 
	double dSttBlockOutWidth= GetWidthBlockOut(TRUE, nJongBasePos);
	double dCoverSd			= bUpper ? dCoverSdStt + dSttBlockOutWidth : dCoverSdStt;
	double dStaSlabSttCen	= pGirCen->GetSSS(OFF_STT_SLAB);
	CDPoint vAngSlabStt     = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint vAngStt			= pGirCen->GetLine()->GetAngleByOffset(dStaSlabSttCen, dStaSlabSttCen+dCoverSdStt, vAngSlabStt);
	double dStaSlabStt		= 0;
	double dDis				= 0;


	if(nPos==POS_SLAB_LEFT)
	{
		dDis		= m_pDB->GetDisSlabLeftAct(dStaSlabSttCen);
		if(dDis > 0)	dDis -= dCoverSd;
		else			dDis += dCoverSd;

		dStaSlabStt = pGirCen->GetStationBySc(dStaSlabSttCen+dCoverSd, dDis, vAngStt);
	}
	else if(nPos == POS_SLAB_RIGH)
	{
		dDis		= m_pDB->GetDisSlabRighAct(dStaSlabSttCen);
		if(dDis > 0)	dDis += dCoverSd;
		else			dDis -= dCoverSd;

		dStaSlabStt = pGirCen->GetStationBySc(dStaSlabSttCen+dCoverSd, dDis, vAngStt);
	}
	else if(nPos == POS_SLAB_CENTER)
	{
		dDis		= (m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_pDB->GetDisSlabRighAct(dStaSlabSttCen))/2;
		dStaSlabStt = pGirCen->GetStationBySc(dStaSlabSttCen+dCoverSd, dDis, vAngStt);
	}

	return dStaSlabStt;
}


// 상부 : 슬래브좌측켄틸레버부 상면 단부 주철근과 슬래브좌측켄틸레버부 상면 일반부 주철근이 같으면 동일
// 하부 : 슬래브좌측켄틸레버부 하면 단부 주철근과 슬래브좌측켄틸레버부 하면 일반부 주철근이 같고 슬래브헌치가 없으면 동일
BOOL CADeckData::IsSameMainRebarDanGen(BOOL bUpper)
{
	long nRebarDanbuStt = GetRebarSuAtDanbu(bUpper, TRUE);
	long nRebarDanbuEnd = GetRebarSuAtDanbu(bUpper, FALSE);

	BOOL bSame1stCycle	= FALSE;
	BOOL bSame2ndCycle	= FALSE;
	BOOL bSameDia = TRUE;
	if(bUpper)
	{
		if ((m_dDiaMainRebar_Cent[0] == m_dDiaMainRebar_Dan[0])
		  &&(m_dDiaMainRebar_Cent[1] == m_dDiaMainRebar_Dan[1])
		  &&(m_dDiaMainRebar_Cent[2] == m_dDiaMainRebar_Dan[2]))
			bSame1stCycle	= TRUE;

		if ((m_dDiaMainRebar_Cent2nd[0] == m_dDiaMainRebar_Dan2nd[0])
		  &&(m_dDiaMainRebar_Cent2nd[1] == m_dDiaMainRebar_Dan2nd[1])
		  &&(m_dDiaMainRebar_Cent2nd[2] == m_dDiaMainRebar_Dan2nd[2]))
			bSame2ndCycle	= TRUE;

		if(bSame1stCycle && bSame2ndCycle)
			bSameDia = TRUE;
		else
			bSameDia = FALSE;

	}
	else 
	{
		if((m_dDiaMainRebarLower_Cent[0] == m_dDiaMainRebarLower_Dan[0])
		 &&(m_dDiaMainRebarLower_Cent[1] == m_dDiaMainRebarLower_Dan[1])
	     &&(m_dDiaMainRebarLower_Cent[2] == m_dDiaMainRebarLower_Dan[2]))
			bSame1stCycle	= TRUE;

		if((m_dDiaMainRebarLower_Cent2nd[0] == m_dDiaMainRebarLower_Dan2nd[0])
		 &&(m_dDiaMainRebarLower_Cent2nd[1] == m_dDiaMainRebarLower_Dan2nd[1])
	     &&(m_dDiaMainRebarLower_Cent2nd[2] == m_dDiaMainRebarLower_Dan2nd[2]))
			bSame2ndCycle	= TRUE;

		 if(bSame1stCycle && bSame2ndCycle)
			bSameDia = TRUE;
		else
			bSameDia = FALSE;
	}

	if(!bSameDia)	return FALSE;
	else if(bSameDia && (nRebarDanbuStt>0 || nRebarDanbuEnd>0))	return FALSE;
	else			return TRUE;
}



// 상하면의 직경이 모두 동일해야 단일구간으로 함.
BOOL CADeckData::IsSameMainRebarDanGen()
{
	BOOL bDivideDanGen = IsSameMainRebarDanGen(TRUE) && IsSameMainRebarDanGen(FALSE) ? FALSE : TRUE;

	return bDivideDanGen ? FALSE : TRUE;
}



BOOL CADeckData::IsSameMainRebarLRC(BOOL bUpper, BOOL bDanbu)
{
	BOOL bSameMainRebarUp	 = FALSE;
	BOOL bSameMainRebarUpDan = FALSE;
	BOOL bSameMainRebarDnDan = FALSE;

	if((m_dGenMainRebarCTC[0] == m_dGenMainRebarCTC[2])
	&& (m_dGenMainRebarCTC[1] == m_dGenMainRebarCTC[2])
	&& (m_dDiaMainRebar_Cent[0] == m_dDiaMainRebar_Cent[2])
	&& (m_dDiaMainRebar_Cent[1] == m_dDiaMainRebar_Cent[2]))
		bSameMainRebarUp = TRUE;

	if((m_dEndMainRebarCTC[0] == m_dEndMainRebarCTC[2])
	&& (m_dEndMainRebarCTC[1] == m_dEndMainRebarCTC[2])
	&& (m_dDiaMainRebar_Dan[0] == m_dDiaMainRebar_Dan[2])
	&& (m_dDiaMainRebar_Dan[1] == m_dDiaMainRebar_Dan[2]))
		bSameMainRebarUpDan = TRUE;

	if((m_dEndMainRebarCTCLower[0] == m_dEndMainRebarCTCLower[2])
	&& (m_dEndMainRebarCTCLower[1] == m_dEndMainRebarCTCLower[2])
	&& (m_dDiaMainRebarLower_Dan[0] == m_dDiaMainRebarLower_Dan[2])
	&& (m_dDiaMainRebarLower_Dan[1] == m_dDiaMainRebarLower_Dan[2]))
		bSameMainRebarDnDan = TRUE;

	if(bUpper)
	{
		if(bDanbu)
			return bSameMainRebarUpDan;
		else
			return bSameMainRebarUp;
	}
	else
	{
		if(bDanbu)
			return bSameMainRebarDnDan;
		else
			return FALSE;	// 하부 일반부는 무조건 FALSE(이유: 철근이 다 상이함.)
	}
}


// 시점부 하부(2), 종점부 하부(2) 철근 존재유무
BOOL CADeckData::IsInstallBeryukRebarAtDanHunch(long nJongBasePos, BOOL bSttSlab, long nCycle)
{
	REBAR_CYCLE* pRebarCycle = GetRebarCycleByJongBasePos(nJongBasePos, nCycle);
	if(bSttSlab)
	{
		if(pRebarCycle->m_dArrJointDn[0] > 0)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(pRebarCycle->m_dArrJointDn[pRebarCycle->m_dArrJointDn.GetUpperBound()] > 0)
			return TRUE;
		else
			return FALSE;
	}
}


// bCalcLeft = TRUE		슬래브 우측 철근 간격을 기준으로 좌측 철근간격을 계산한다.
// bCalcLeft = FALSE	슬래브 좌측 철근 간격을 기준으로 우측 철근간격을 계산한다.
double CADeckData::GetCTCBySlabLeftRighRatio(BOOL bCalcLeft, CDPointArray &xyArrSourceCTC, double dDataNew, long nGugansu, BOOL bIncludeSinchuk)
{
	CPlateGirderApp		*pGirCen = m_pDB->GetGirder(-1);

	double dSttBlockOutWidth = GetWidthBlockOut(TRUE, bCalcLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);
	if(m_bInstallMainRebarAtBlockOut || bIncludeSinchuk==FALSE)
		dSttBlockOutWidth  = 0;

	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	CDPoint	vAngStt		  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd		  = pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	double dCoverSdStt = GetLengthCoverSd(TRUE, bCalcLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);

	//////////////////////////////////////////////////////////////////////////
	double dSum = 0;
	if(dStaSlabSttLeft >= dStaSlabSttRigh)
	{
		if(bCalcLeft==FALSE)
			dSum += dSttBlockOutWidth+dCoverSdStt;
		else 
			dSum += dSttBlockOutWidth+dCoverSdStt+pGirCen->GetLengthDimType(dStaSlabSttRigh, dStaSlabSttLeft, dDisRStt);
	}
	else
	{
		if(bCalcLeft==FALSE)
			dSum += dSttBlockOutWidth+dCoverSdStt+pGirCen->GetLengthDimType(dStaSlabSttLeft, dStaSlabSttRigh, dDisLStt);
		else 
			dSum += dSttBlockOutWidth+dCoverSdStt;
	}
	long n = 0;
	for(n=0; n<=nGugansu; n++)
		dSum += xyArrSourceCTC[n].x*xyArrSourceCTC[n].y;
	//////////////////////////////////////////////////////////////////////////
	long nGuganTotal = dArrStaLeft.GetUpperBound();						// 곡선반경 구간수
	double dSumGugan = 0;
	long   nCurGugan = 0;
	for(n=0; n<nGuganTotal; n++)
	{
		if(bCalcLeft)	// 좌측을 계산하려면 오른쪽 구간을 가지고 계산함.
			dSumGugan += m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[n], dArrStaRigh[n+1], dDisRStt);
		else
			dSumGugan += m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[n], dArrStaLeft[n+1], dDisLStt);
		
		if(dSum <= dSumGugan)
		{
			nCurGugan = n;
			break;
		}
	}

	double dLengthLeft = m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[nCurGugan], dArrStaLeft[nCurGugan+1], dDisLStt);
	double dLengthRigh = m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[nCurGugan], dArrStaRigh[nCurGugan+1], dDisRStt);
	
	if(bCalcLeft)
		return dDataNew*(dLengthLeft/dLengthRigh);
	else
		return dDataNew*(dLengthRigh/dLengthLeft);
}


// 0 : 도로중심 기준
// 1 : 슬래브중심 기준
// 2 : 곡률반경이 큰쪽 기준
void CADeckData::CalcCTCMainRebar()
{
	if(m_nJongCtcCalcPos == 0 || m_nJongCtcCalcPos == 1)
		CalcCTCMainRebarBySlabHDanPos();
	else if(m_nJongCtcCalcPos == 2)
		CalcCTCMainRebarByMaxRadius();

	CalcCTCSinchukRebar();
	CalcCTCDanHunchRebar();
}



// 0 : 도로중심 기준
// 1 : 슬래브중심 기준
void CADeckData::CalcCTCMainRebarBySlabHDanPos()
{
	CPlateGirderApp	*pGirCen = m_pDB->GetGirder(-1);
	CGlobarOption   *pGlopt  = m_pDB->m_pDataManage->GetGlobalOption();
	CRebarPlacing RebarPlacing;
	
	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	double dSttBlockOutWidthLeft = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
	double dEndBlockOutWidthLeft = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT);
	double dSttBlockOutWidthRigh = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
	double dEndBlockOutWidthRigh = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT);
	double dSttBlockOutWidthCent = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABCENTER);
	double dEndBlockOutWidthCent = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABCENTER);
	double dCoverSdStt = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABCENTER);
	double dCoverSdEnd = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABCENTER);
	
	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidthLeft = 0;
		dEndBlockOutWidthLeft = 0;
		dSttBlockOutWidthRigh = 0;	
		dEndBlockOutWidthRigh = 0;
		dSttBlockOutWidthCent = 0;
		dEndBlockOutWidthCent = 0;
	}

	double dLengthLeft=0, dLengthRigh=0, dLengthGuganSlab=0, dLengthBase=0, dStaStt=0;	
	vector<CDPointArray> xyArrCTCUpLeft, xyArrCTCDnLeft, xyArrCTCUpRigh, xyArrCTCDnRigh, xyArrCTCUpCent, xyArrCTCDnCent;
	CDPointArray xyArrDim;
	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaCL   = GetStationArrForMainRebarGugan(2);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDPoint xyDim, xyDimStt, xyDimEnd;

	double dLenModifySttLeft=0, dLenModifySttRigh=0, dLenModifyEndLeft=0, dLenModifyEndRigh=0;
	// 구간별 철근간격계산(상부기준철근)
	long nGugansu = dArrStaLeft.GetUpperBound();	// 곡선반경 구간수
	for(long nGugan=0; nGugan<nGugansu; nGugan++)
	{
		if(m_nJongCtcCalcPos==0)	// 도로중심
		{
			dLengthGuganSlab = dArrStaCL[nGugan+1]-dArrStaCL[nGugan];
		}
		else
		{
			dLengthLeft = m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[nGugan], dArrStaLeft[nGugan+1], dDisLStt);
			dLengthRigh = m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[nGugan], dArrStaRigh[nGugan+1], dDisRStt);
			dLengthGuganSlab = (dLengthLeft+dLengthRigh)/2;
		}

		double dSttBlockOutW = m_nJongCtcCalcPos==0 ? pGlopt->GetSttExpansionJointWidth() : dSttBlockOutWidthCent;
		double dEndBlockOutW = m_nJongCtcCalcPos==0 ? pGlopt->GetEndExpansionJointWidth() : dEndBlockOutWidthCent;
		if (nGugan==0)
		{
			if(IsSkewStt())
			{
				double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).GetAngleDegree();
				if((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360))
				{
					dCoverSdStt = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT));
					dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, dSttBlockOutWidthRigh);
				}
				if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
				{
					dCoverSdStt = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT));
					dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, dSttBlockOutWidthLeft);
				}
			}

			if(IsSkewEnd())
			{
				double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
				if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, dEndBlockOutWidthRigh);
				}
				if((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen,JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, dEndBlockOutWidthLeft);
				}
			}

			if(nGugansu==1)	dLengthBase = dLengthGuganSlab - dCoverSdStt - dCoverSdEnd - dSttBlockOutW  - dEndBlockOutW;
			else			dLengthBase = dLengthGuganSlab - dCoverSdStt - dSttBlockOutW;
		}
		else if (nGugan==nGugansu-1)
		{
			if(IsSkewEnd())
			{
				double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
				if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, dEndBlockOutWidthRigh);
				}
				if((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen,JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, dEndBlockOutWidthLeft);
				}
			}
			dLengthBase = dLengthGuganSlab - dCoverSdEnd - dEndBlockOutW;
		}
		else
			dLengthBase = dLengthGuganSlab;
		
		CDPointArray xyArrDimBase = CalcCTCMainRebarBase(nGugansu, nGugan, dLengthBase);

		if(m_nJongCtcCalcPos==0)
		{
			dLenModifySttLeft = GetCoverSdToModify(TRUE, TRUE);
			dLenModifyEndLeft = GetCoverSdToModify(FALSE, TRUE);
			dLenModifySttRigh = GetCoverSdToModify(TRUE, FALSE);
			dLenModifyEndRigh = GetCoverSdToModify(FALSE, FALSE);
		}		
		else
		{
			dLenModifySttLeft = GetCoverSdToModify(TRUE, TRUE);
			dLenModifySttRigh = -GetCoverSdToModify(TRUE, FALSE);
			dLenModifyEndLeft = GetCoverSdToModify(FALSE, TRUE);
			dLenModifyEndRigh = -GetCoverSdToModify(FALSE, FALSE);
		}

		// 좌측슬래브 위치 
		dStaStt  = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[nGugan], nGugan==0?(dCoverSdStt+dSttBlockOutWidthLeft):0, dDisLStt);
		double dDistSource = m_nJongCtcCalcPos==0 ? 0 : (dDisLStt+dDisRStt)/2;
		xyArrDim = GetDimArrAtMultiCurve(dStaStt, xyArrDimBase, dDistSource, -1);
		if(IsBridgeHasCurveLine())	// 시종점의 피복위치 보정(오프셋영향)
		{
			CDPointArray xyArrDimTemp;
			if(nGugansu==1)
			{
				if(!IsSkewStt())
				{	
					xyDim = xyArrDim[0];
					xyDimStt.x = 1;
					xyDimStt.y = xyDim.y+dLenModifySttLeft;
					xyDimEnd.x = xyDim.x-1;
					xyDimEnd.y = xyDim.y;
					xyArrDimTemp.Add(xyDimStt);
					if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
					for (long a=1; a<xyArrDim.GetSize()-1; a++)
						xyArrDimTemp.Add(xyArrDim[a]);
				}
				else
				{
					for (long a=0; a<xyArrDim.GetSize()-1; a++)
						xyArrDimTemp.Add(xyArrDim[a]);
				}
				if(!IsSkewEnd())
				{
					xyDim = xyArrDim[xyArrDim.GetUpperBound()];
					xyDimStt.x = 1;
					xyDimStt.y = xyDim.y+dLenModifyEndLeft;
					xyDimEnd.x = xyDim.x-1;
					xyDimEnd.y = xyDim.y;
					if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
					xyArrDimTemp.Add(xyDimStt);
				}
				else
				{
					xyArrDimTemp.Add(xyArrDim[xyArrDim.GetUpperBound()]);
				}
				xyArrCTCUpLeft.push_back(xyArrDimTemp);
			}
			else
			{
				if(nGugan==0)
				{
					if(!IsSkewStt())
					{
						xyDim = xyArrDim[0];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y+dLenModifySttLeft;
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						xyArrDimTemp.Add(xyDimStt);
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						for (long a=1; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
					else
					{
						for (long a=0; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
				}
				else if(nGugan==nGugansu-1)
				{
					if(!IsSkewEnd())
					{
						xyDim = xyArrDim[xyArrDim.GetUpperBound()];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y+dLenModifyEndLeft;
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						for (long a=0; a<xyArrDim.GetSize()-1; a++)
							xyArrDimTemp.Add(xyArrDim[a]);
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						xyArrDimTemp.Add(xyDimStt);
					}
					else
					{
						for (long a=0; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
				}
				else
				{						
					for (long a=0; a<xyArrDim.GetSize(); a++)
						xyArrDimTemp.Add(xyArrDim[a]);
				}
				xyArrCTCUpLeft.push_back(xyArrDimTemp);
			}
		}
		else
			xyArrCTCUpLeft.push_back(xyArrDim);

		// 우측슬래브 위치                                                      
		dStaStt  = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[nGugan], nGugan==0?(dCoverSdStt+dSttBlockOutWidthRigh):0, dDisRStt);
		xyArrDim = GetDimArrAtMultiCurve(dStaStt, xyArrDimBase, dDistSource, 1);
		if(IsBridgeHasCurveLine())	// 시종점의 피복위치 보정(오프셋영향)
		{
			CDPointArray xyArrDimTemp;
			if(nGugansu==1)	
			{
				if(!IsSkewStt())
				{
					xyDim = xyArrDim[0];
					xyDimStt.x = 1;
					xyDimStt.y = xyDim.y+dLenModifySttRigh;
					xyDimEnd.x = xyDim.x-1;
					xyDimEnd.y = xyDim.y;
					xyArrDimTemp.Add(xyDimStt);
					if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
					for (long a=1; a<xyArrDim.GetSize()-1; a++)
						xyArrDimTemp.Add(xyArrDim[a]);
				}
				else
				{
					for (long a=0; a<xyArrDim.GetSize()-1; a++)
						xyArrDimTemp.Add(xyArrDim[a]);
				}
				
				if(!IsSkewEnd())
				{
					xyDim = xyArrDim[xyArrDim.GetUpperBound()];
					xyDimStt.x = 1;
					xyDimStt.y = xyDim.y+dLenModifyEndRigh;
					xyDimEnd.x = xyDim.x-1;
					xyDimEnd.y = xyDim.y;
					if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
					xyArrDimTemp.Add(xyDimStt);
				}
				else
				{
					xyArrDimTemp.Add(xyArrDim[xyArrDim.GetUpperBound()]);
				}
				xyArrCTCUpRigh.push_back(xyArrDimTemp);
			}
			else
			{
				if(nGugan==0)
				{
					if(!IsSkewStt())
					{
						xyDim = xyArrDim[0];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y+dLenModifySttRigh;
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						xyArrDimTemp.Add(xyDimStt);
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						for (long a=1; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
					else
					{
						for (long a=0; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
				}
				else if(nGugan==nGugansu-1)
				{
					if(!IsSkewEnd())
					{
						xyDim = xyArrDim[xyArrDim.GetUpperBound()];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y+dLenModifyEndRigh;
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						for (long a=0; a<xyArrDim.GetSize()-1; a++)
							xyArrDimTemp.Add(xyArrDim[a]);
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						xyArrDimTemp.Add(xyDimStt);
					}
					else
					{
						for (long a=0; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
				}
				else
				{
					for (long a=0; a<xyArrDim.GetSize(); a++)
						xyArrDimTemp.Add(xyArrDim[a]);
				}
				xyArrCTCUpRigh.push_back(xyArrDimTemp);
			}
		}
		else
			xyArrCTCUpRigh.push_back(xyArrDim);
		
		CDPointArray xyArrDimSttL, xyArrDimSttR, xyArrDimEndL, xyArrDimEndR;
		if(nGugan==0)
			CalcCTCMainRebarAtSinchuk(TRUE, xyArrDimSttL, xyArrDimSttR);
		if(nGugan==nGugansu-1 || nGugansu==1)
			CalcCTCMainRebarAtSinchuk(FALSE, xyArrDimEndL, xyArrDimEndR);

		if(nGugan==0)
		{
			xyArrCTCDnLeft.push_back(xyArrDimSttL);
			xyArrCTCDnRigh.push_back(xyArrDimSttR);
		}
		xyArrCTCDnLeft.push_back(xyArrCTCUpLeft[nGugan]);
		xyArrCTCDnRigh.push_back(xyArrCTCUpRigh[nGugan]);
		if(nGugan==nGugansu-1 || nGugansu==1)
		{
			xyArrCTCDnLeft.push_back(xyArrDimEndL);
			xyArrCTCDnRigh.push_back(xyArrDimEndR);
		}
	}

	m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER].RemoveAll();

	long n = 0;
	// 좌측-상부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCUpLeft.size(); n++)
	{
		xyArrDim = xyArrCTCUpLeft[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].Add(xyArrDim[a]);
	}
	// 좌측-하부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCDnLeft.size(); n++)
	{
		xyArrDim = xyArrCTCDnLeft[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(xyArrDim[a]);
	}
	// 우측-상부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCUpRigh.size(); n++)
	{
		xyArrDim = xyArrCTCUpRigh[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].Add(xyArrDim[a]);
	}
	// 우측-하부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCDnRigh.size(); n++)
	{
		xyArrDim = xyArrCTCDnRigh[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(xyArrDim[a]);
	}

	m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT], m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT]);
	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT], m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT]);

	if(m_bInstallMainRebarAtBlockOut)
	{
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT]		= m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT];
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT]		= m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT];
		m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER]	= m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER];
	}

	CalcCTCMainRebarForDim();
}

// 2 : 곡률반경이 큰쪽 기준
void CADeckData::CalcCTCMainRebarByMaxRadius()
{
	CPlateGirderApp	*pGirCen = m_pDB->GetGirder(-1);
	CRebarPlacing RebarPlacing;
	
	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	double dSttBlockOutWidthLeft = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
	double dEndBlockOutWidthLeft = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT);
	double dSttBlockOutWidthRigh = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
	double dEndBlockOutWidthRigh = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdStt = GetLengthCoverSd(TRUE);
	double dCoverSdEnd = GetLengthCoverSd(FALSE);

	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidthLeft = 0;
		dEndBlockOutWidthLeft = 0;
		dSttBlockOutWidthRigh = 0;	
		dEndBlockOutWidthRigh = 0;
	}

	double dLengthLeft=0, dLengthRigh=0, dLengthMax=0, dLengthBase=0, dStaStt=0;	
	vector<CDPointArray> xyArrCTCUpLeft, xyArrCTCDnLeft, xyArrCTCUpRigh, xyArrCTCDnRigh;
	CDPointArray xyArrDimUp, xyArrDim;
	CDPoint xyDim, xyDimStt, xyDimEnd;
	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);	// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);	// 시,종점 및 선형곡률변경위치의 중앙 스테이션

	// 구간별 철근간격계산(상부기준철근)
	long nGugansu = dArrStaLeft.GetUpperBound();	// 곡선반경 구간수
	for(long nGugan=0; nGugan<nGugansu; nGugan++)
	{
		dLengthLeft = m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[nGugan], dArrStaLeft[nGugan+1], dDisLStt);
		dLengthRigh = m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[nGugan], dArrStaRigh[nGugan+1], dDisRStt);
		BOOL bBaseLeft = dLengthLeft>dLengthRigh ? TRUE : FALSE;
		dLengthMax  = max(dLengthLeft, dLengthRigh);
		double dSttBlockOutW = bBaseLeft ? dSttBlockOutWidthLeft : dSttBlockOutWidthRigh;
		double dEndBlockOutW = bBaseLeft ? dEndBlockOutWidthLeft : dEndBlockOutWidthRigh;
		dCoverSdStt = GetLengthCoverSd(TRUE, bBaseLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);
		dCoverSdEnd = GetLengthCoverSd(FALSE, bBaseLeft?JONGBASEPOS_SLABLEFT:JONGBASEPOS_SLABRIGHT);

		if (nGugan==0)
		{
			if(IsSkewStt())
			{
				double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).GetAngleDegree();
				if(bBaseLeft && ((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360)))
				{
					dCoverSdStt = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT));
					dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, dSttBlockOutWidthRigh);
				}
				if(!bBaseLeft && ((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270)))
				{
					dCoverSdStt = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT));
					dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, dSttBlockOutWidthLeft);
				}
			}

			if(IsSkewEnd())
			{
				double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
				if(bBaseLeft && ((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270)))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, dEndBlockOutWidthRigh);
				}
				if(!bBaseLeft && ((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360)))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen,JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, dEndBlockOutWidthLeft);
				}
			}

			if(nGugansu==1)	dLengthBase = dLengthMax - dCoverSdStt - dCoverSdEnd - dSttBlockOutW  - dEndBlockOutW;
			else			dLengthBase = dLengthMax - dCoverSdStt - dSttBlockOutW;
		}
		else if (nGugan==nGugansu-1)
		{
			if(IsSkewEnd())
			{
				double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
				if(bBaseLeft && ((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270)))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, dEndBlockOutWidthRigh);
				}
				if(!bBaseLeft && ((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360)))
				{
					dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT));
					dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen,JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, dEndBlockOutWidthLeft);
				}
			}
			dLengthBase = dLengthMax - dCoverSdEnd - dEndBlockOutW;
		}
		else
			dLengthBase = dLengthMax;

		CDPointArray xyArrDimBase = CalcCTCMainRebarBase(nGugansu, nGugan, dLengthBase);

		// 좌측슬래브 위치 
		if(bBaseLeft)		
		{
			xyArrCTCUpLeft.push_back(xyArrDimBase);
		}
		else								
		{
			dStaStt  = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[nGugan], nGugan==0?(dCoverSdStt+dSttBlockOutWidthLeft):0, dDisLStt);
			xyArrDim = GetDimArrAtMultiCurve(dStaStt, xyArrDimBase, dDisRStt, -1);
			if(IsBridgeHasCurveLine())	// 시종점의 피복위치 보정(오프셋영향)
			{
				CDPointArray xyArrDimTemp;
				if(nGugansu==1)
				{
					if(!IsSkewStt())
					{
						xyDim = xyArrDim[0];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y-GetCoverSdToModify(TRUE, TRUE);
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						xyArrDimTemp.Add(xyDimStt);
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						for (long a=1; a<xyArrDim.GetSize()-1; a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
					else
					{
						for (long a=0; a<xyArrDim.GetSize()-1; a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
					if(!IsSkewEnd())
					{
						xyDim = xyArrDim[xyArrDim.GetUpperBound()];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, TRUE);
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						xyArrDimTemp.Add(xyDimStt);
					}
					else
					{
						xyArrDimTemp.Add(xyArrDim[xyArrDim.GetUpperBound()]);
					}
					
					xyArrCTCUpLeft.push_back(xyArrDimTemp);
				}
				else
				{
					if(nGugan==0)
					{
						if(!IsSkewStt())
						{
							xyDim = xyArrDim[0];
							xyDimStt.x = 1;
							xyDimStt.y = xyDim.y-GetCoverSdToModify(TRUE, TRUE);
							xyDimEnd.x = xyDim.x-1;
							xyDimEnd.y = xyDim.y;
							xyArrDimTemp.Add(xyDimStt);
							if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
							for (long a=1; a<xyArrDim.GetSize(); a++)
								xyArrDimTemp.Add(xyArrDim[a]);
						}
						else
						{
							for (long a=0; a<xyArrDim.GetSize(); a++)
								xyArrDimTemp.Add(xyArrDim[a]);
						}
					}
					else if(nGugan==nGugansu-1)
					{
						if(!IsSkewEnd())
						{
							xyDim = xyArrDim[xyArrDim.GetUpperBound()];
							xyDimStt.x = 1;
							xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, TRUE);
							xyDimEnd.x = xyDim.x-1;
							xyDimEnd.y = xyDim.y;
							for (long a=0; a<xyArrDim.GetSize()-1; a++)
								xyArrDimTemp.Add(xyArrDim[a]);
							if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
							xyArrDimTemp.Add(xyDimStt);
						}
						else
						{
							for (long a=0; a<xyArrDim.GetSize(); a++)
								xyArrDimTemp.Add(xyArrDim[a]);
						}
					}
					else
					{						
						for (long a=0; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
					xyArrCTCUpLeft.push_back(xyArrDimTemp);
				}
			}
			else
				xyArrCTCUpLeft.push_back(xyArrDim);
		}

		// 우측슬래브 위치                                                      
		if(!bBaseLeft)
		{
			if(dLengthLeft==dLengthRigh)
				xyArrCTCUpRigh.push_back(xyArrDim);
			else
				xyArrCTCUpRigh.push_back(xyArrDimBase);
		}
		else							
		{
			dStaStt  = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[nGugan], nGugan==0?(dCoverSdStt+dSttBlockOutWidthRigh):0, dDisRStt);
			xyArrDim = GetDimArrAtMultiCurve(dStaStt, xyArrDimBase, dDisLStt, 1);
			if(IsBridgeHasCurveLine())	// 시종점의 피복위치 보정(오프셋영향)
			{
				CDPointArray xyArrDimTemp;
				if(nGugansu==1)	
				{
					if(!IsSkewStt())
					{
						xyDim = xyArrDim[0];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y-GetCoverSdToModify(TRUE, FALSE);
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						xyArrDimTemp.Add(xyDimStt);
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						for (long a=1; a<xyArrDim.GetSize()-1; a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
					else
					{
						for (long a=0; a<xyArrDim.GetSize()-1; a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}

					if(!IsSkewEnd())
					{
						xyDim = xyArrDim[xyArrDim.GetUpperBound()];
						xyDimStt.x = 1;
						xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, FALSE);
						xyDimEnd.x = xyDim.x-1;
						xyDimEnd.y = xyDim.y;
						if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
						xyArrDimTemp.Add(xyDimStt);
					}
					else
					{
						xyArrDimTemp.Add(xyArrDim[xyArrDim.GetUpperBound()]);
					}
					xyArrCTCUpRigh.push_back(xyArrDimTemp);
				}
				else
				{
					if(nGugan==0)
					{
						if(!IsSkewStt())
						{
							xyDim = xyArrDim[0];
							xyDimStt.x = 1;
							xyDimStt.y = xyDim.y-GetCoverSdToModify(TRUE, FALSE);
							xyDimEnd.x = xyDim.x-1;
							xyDimEnd.y = xyDim.y;
							xyArrDimTemp.Add(xyDimStt);
							if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
							for (long a=1; a<xyArrDim.GetSize(); a++)
								xyArrDimTemp.Add(xyArrDim[a]);
						}
						else
						{
							for (long a=0; a<xyArrDim.GetSize(); a++)
								xyArrDimTemp.Add(xyArrDim[a]);
						}						
					}
					else if(nGugan==nGugansu-1)
					{
						if(!IsSkewEnd())
						{
							xyDim = xyArrDim[xyArrDim.GetUpperBound()];
							xyDimStt.x = 1;
							xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, FALSE);
							xyDimEnd.x = xyDim.x-1;
							xyDimEnd.y = xyDim.y;
							for (long a=0; a<xyArrDim.GetSize()-1; a++)
								xyArrDimTemp.Add(xyArrDim[a]);
							if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
							xyArrDimTemp.Add(xyDimStt);
						}
						else
						{
							for (long a=0; a<xyArrDim.GetSize(); a++)
								xyArrDimTemp.Add(xyArrDim[a]);
						}					
					}
					else
					{						
						for (long a=0; a<xyArrDim.GetSize(); a++)
							xyArrDimTemp.Add(xyArrDim[a]);
					}
					xyArrCTCUpRigh.push_back(xyArrDimTemp);
				}
			}
			else
				xyArrCTCUpRigh.push_back(xyArrDim);
		}


		// 하부주철근 간격계산
		CDPointArray xyArrDimSttL, xyArrDimSttR, xyArrDimEndL, xyArrDimEndR;
		if(nGugan==0)
			CalcCTCMainRebarAtSinchuk(TRUE, xyArrDimSttL, xyArrDimSttR);
		if(nGugan==nGugansu-1 || nGugansu==1)
			CalcCTCMainRebarAtSinchuk(FALSE, xyArrDimEndL, xyArrDimEndR);

		if(IsSkewStt())
		{
			xyArrDimSttL.RemoveAll();
			xyArrDimSttR.RemoveAll();
		}
		if(IsSkewEnd())
		{
			xyArrDimEndL.RemoveAll();
			xyArrDimEndR.RemoveAll();
		}

		if(nGugan==0)
		{
			xyArrCTCDnLeft.push_back(xyArrDimSttL);
			xyArrCTCDnRigh.push_back(xyArrDimSttR);
		}
		xyArrCTCDnLeft.push_back(xyArrCTCUpLeft[nGugan]);
		xyArrCTCDnRigh.push_back(xyArrCTCUpRigh[nGugan]);
		if(nGugan==nGugansu-1 || nGugansu==1)
		{
			xyArrCTCDnLeft.push_back(xyArrDimEndL);
			xyArrCTCDnRigh.push_back(xyArrDimEndR);
		}
	}

	m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER].RemoveAll();

	long n =0;
	// 좌측-상부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCUpLeft.size(); n++)
	{
		xyArrDim = xyArrCTCUpLeft[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].Add(xyArrDim[a]);
	}
	// 좌측-하부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCDnLeft.size(); n++)
	{
		xyArrDim = xyArrCTCDnLeft[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(xyArrDim[a]);
	}
	// 우측-상부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCUpRigh.size(); n++)
	{
		xyArrDim = xyArrCTCUpRigh[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].Add(xyArrDim[a]);
	}
	// 우측-하부 주철근 위치                                                       
	for(n=0; n<(long)xyArrCTCDnRigh.size(); n++)
	{
		xyArrDim = xyArrCTCDnRigh[n];
		for(long a=0; a<xyArrDim.GetSize(); a++)
			m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(xyArrDim[a]);
	}

	m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT], m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT]);
	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT], m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT]);

	if(m_bInstallMainRebarAtBlockOut)
	{
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT]		= m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT];
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT]		= m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT];
		m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER]	= m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER];
	}

	CalcCTCMainRebarForDim();
}


CDPointArray CADeckData::CalcCTCMainRebarBase(long nGugansu, long nGugan, double dBaseLen)
{
	CDPointArray xyArrDimBase;

	double dCTCMainCen = m_dGenMainRebarCTC[0]/2;	// 일반부 주철근 간격
	double dCTCMainDan = m_dEndMainRebarCTC[0]/2;	// 단부 주철근 간격
	long nRebarDanStt = long(m_dRangeDanbuStt/dCTCMainDan);		// 단부 시점철근수 
	long nRebarDanEnd = long(m_dRangeDanbuEnd/dCTCMainDan);		// 단부 종점철근수 
	long nRebarDan    = nRebarDanStt+nRebarDanEnd;
	long nRebarCen    = 0;
	double dExtL      = 0;
	double dFirstSpace= 0;

	if (nGugan==0)
	{
		if(nGugansu==1)
		{
			nRebarCen = long((dBaseLen-nRebarDan*dCTCMainDan)/dCTCMainCen);			// 철근수 
			dExtL     = dBaseLen - nRebarCen*dCTCMainCen - nRebarDan*dCTCMainDan;	// 짜투리	
		}
		else
		{
			nRebarCen = long((dBaseLen-nRebarDanStt*dCTCMainDan)/dCTCMainCen);		 
			dExtL     = dBaseLen - nRebarCen*dCTCMainCen - nRebarDanStt*dCTCMainDan;	
		}
	}
	else if (nGugan==nGugansu-1)
	{
		nRebarCen = long((dBaseLen-nRebarDanEnd*dCTCMainDan)/dCTCMainCen);			 
		dExtL     = dBaseLen - nRebarCen*dCTCMainCen - nRebarDanEnd*dCTCMainDan;		
	}
	else
	{
		nRebarCen = long(dBaseLen/dCTCMainCen);	
		dExtL  = dBaseLen - nRebarCen*dCTCMainCen;
	}

	if(dExtL/2 > REBAR_MIN_SPACE)					// 짜투리의 절반이 최소간격보다 크면 
		dFirstSpace = dExtL/2;
	else											// 짜투리의 절반이 최소간격보다 작으면 철근간격반을 더함
	{			
		if(nGugan==0)
		{
			if(nRebarDanStt>0)
			{
				nRebarDanStt--;
				dFirstSpace = (dExtL+dCTCMainDan)/2;	
			}
			else
			{
				nRebarCen--;
				dFirstSpace = (dExtL+dCTCMainCen)/2;	
			}
		}
		else if(nGugan==nGugansu-1)
		{
			if(nRebarDanEnd>0)
			{
				nRebarDanEnd--;
				dFirstSpace = (dExtL+dCTCMainDan)/2;	
			}
			else
			{
				nRebarCen--;
				dFirstSpace = (dExtL+dCTCMainCen)/2;	
			}
		}
		else
		{
			dFirstSpace = (dExtL+dCTCMainCen)/2;	
			nRebarCen--;
		}
	}

	if(nGugan==0)
	{
		if(m_nOptMainRebarArrange == 0)		// 교량중심
		{
			xyArrDimBase.Add(CDPoint(1, dFirstSpace));
			xyArrDimBase.Add(CDPoint(nRebarDanStt, dCTCMainDan));
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			if(nGugansu==1)
				xyArrDimBase.Add(CDPoint(nRebarDanEnd, dCTCMainDan));
			xyArrDimBase.Add(CDPoint(1, dFirstSpace));
		}
		else if(m_nOptMainRebarArrange == 1)	// 슬래브 시점부 기준
		{
			xyArrDimBase.Add(CDPoint(2, dFirstSpace));
			xyArrDimBase.Add(CDPoint(nRebarDanStt, dCTCMainDan));
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			if(nGugansu==1)
				xyArrDimBase.Add(CDPoint(nRebarDanEnd, dCTCMainDan));
		}
		else if(m_nOptMainRebarArrange == 2)	// 슬래브 종점부 기준
		{
			xyArrDimBase.Add(CDPoint(nRebarDanStt, dCTCMainDan));
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			if(nGugansu==1)
				xyArrDimBase.Add(CDPoint(nRebarDanEnd, dCTCMainDan));
			xyArrDimBase.Add(CDPoint(2, dFirstSpace));
		}
	}
	else if (nGugan==nGugansu-1)
	{
		if(m_nOptMainRebarArrange == 0)		// 교량중심
		{
			xyArrDimBase.Add(CDPoint(1, dFirstSpace));
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			xyArrDimBase.Add(CDPoint(nRebarDanEnd, dCTCMainDan));
			xyArrDimBase.Add(CDPoint(1, dFirstSpace));
		}
		else if(m_nOptMainRebarArrange == 1)	// 슬래브 시점부 기준
		{
			xyArrDimBase.Add(CDPoint(2, dFirstSpace));
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			xyArrDimBase.Add(CDPoint(nRebarDanEnd, dCTCMainDan));
		}
		else if(m_nOptMainRebarArrange == 2)	// 슬래브 종점부 기준
		{
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			xyArrDimBase.Add(CDPoint(nRebarDanEnd, dCTCMainDan));
			xyArrDimBase.Add(CDPoint(2, dFirstSpace));
		}

	}
	else
	{
		if(m_nOptMainRebarArrange == 0)		// 교량중심
		{
			xyArrDimBase.Add(CDPoint(1, dFirstSpace));
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			xyArrDimBase.Add(CDPoint(1, dFirstSpace));
		}
		else if(m_nOptMainRebarArrange == 1)	// 슬래브 시점부 기준
		{
			xyArrDimBase.Add(CDPoint(2, dFirstSpace));
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
		}
		else if(m_nOptMainRebarArrange == 2)	// 슬래브 종점부 기준
		{
			xyArrDimBase.Add(CDPoint(nRebarCen, dCTCMainCen));
			xyArrDimBase.Add(CDPoint(2, dFirstSpace));
		}

	}			

	return xyArrDimBase;
}





void CADeckData::CalcCTCMainRebarBySlabCentCTCBySlabUpper()
{
//	CPlateGirderApp		*pGirCen	= m_pDB->GetGirder(-1);
//
//	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
//	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
//	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
//	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
//	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
//	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
//	double dDisLEnd		  = m_pDB->GetDisSlabLeftAct(dStaSlabEndCen)+m_dCoverSd;
//	double dDisREnd		  = m_pDB->GetDisSlabRighAct(dStaSlabEndCen)-m_dCoverSd;
//	CDPoint	vAngStt		  = pGirCen->GetAAA(OFF_STT_SLAB);
//	CDPoint	vAngEnd		  = pGirCen->GetAAA(OFF_END_SLAB);
//	double dStaSlabSttLeft= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
//	double dStaSlabSttRigh= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
//	double dStaSlabEndLeft= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
//	double dStaSlabEndRigh= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
//
//	double dSttBlockOutWidthLeft = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
//	double dSttBlockOutWidthRigh = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
//	double dEndBlockOutWidthLeft = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT);
//	double dEndBlockOutWidthRigh = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT);
//
//	CDPointArray xyArrUp;						// 상면철근간격
//	CDPointArray xyArrUpR;						// 상면철근간격우측
//	CDPointArray xyArrDn;
//	CDPointArray xyArrDnR;
//	double dCoverSdStt = GetLengthCoverSd(TRUE); 
//	double dCoverSdEnd = GetLengthCoverSd(FALSE); 
//
//	double dSpaceFrStt = dCoverSdStt+dSttBlockOutWidthRigh;
//	double dStaStt	   = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[0], dSpaceFrStt, dDisRStt);
//	CDPointArray xyArrDimUpRigh = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER], (dDisLStt+dDisRStt)/2, 1);
//	// 상부 시종점의 피복위치 보정(오프셋영향)
//	if(IsBridgeHasCurveLine())
//	{
//		CDPointArray xyArrDimTemp;
//		CDPoint xyDim = xyArrDimUpRigh[0];
//		CDPoint xyDimStt, xyDimEnd;
//		xyDimStt.x = 1;
//		xyDimStt.y = GetCoverSdToModifyAtExp(TRUE, FALSE, m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetAt(0));
//		xyDimEnd.x = xyDim.x-1;
//		xyDimEnd.y = xyDim.y;
//		xyArrDimTemp.Add(xyDimStt);
//		if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//		for (long a=1; a<xyArrDimUpRigh.GetSize()-1; a++)
//			xyArrDimTemp.Add(xyArrDimUpRigh[a]);
//		
//		xyDim = xyArrDimUpRigh[xyArrDimUpRigh.GetUpperBound()];
//		xyDimStt.x = 1;
//		xyDimStt.y = GetCoverSdToModifyAtExp(FALSE, FALSE, m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetAt(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetUpperBound()));
//		xyDimEnd.x = xyDim.x-1;
//		xyDimEnd.y = xyDim.y;
//		if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//		xyArrDimTemp.Add(xyDimStt);
//
//		m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = xyArrDimTemp;
//	}
//	else
//		m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER];
//
//	//////////////////////////////////////////////////////////////////////////
//		
//	dSpaceFrStt = dCoverSdStt+dSttBlockOutWidthLeft;
//	dStaStt	    = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[0], dSpaceFrStt, dDisLStt);
//	CDPointArray xyArrDimUpLeft = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER], (dDisLStt+dDisRStt)/2, -1);
//
//	// 상부 시종점의 피복위치 보정(오프셋영향)
//	if(IsBridgeHasCurveLine())
//	{
//		CDPointArray xyArrDimTemp;
//		CDPoint xyDim = xyArrDimUpLeft[0];
//		CDPoint xyDimStt, xyDimEnd;
//		xyDimStt.x = 1;
//		xyDimStt.y = GetCoverSdToModifyAtExp(TRUE, TRUE, m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetAt(0));
//		xyDimEnd.x = xyDim.x-1;
//		xyDimEnd.y = xyDim.y;
//		xyArrDimTemp.Add(xyDimStt);
//		if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//		for (long a=1; a<xyArrDimUpLeft.GetSize()-1; a++)
//			xyArrDimTemp.Add(xyArrDimUpLeft[a]);
//		
//		xyDim = xyArrDimUpLeft[xyArrDimUpLeft.GetUpperBound()];
//		xyDimStt.x = 1;
//		xyDimStt.y = GetCoverSdToModifyAtExp(FALSE, TRUE, m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetAt(m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetUpperBound()));
//		xyDimEnd.x = xyDim.x-1;
//		xyDimEnd.y = xyDim.y;
//		if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//		xyArrDimTemp.Add(xyDimStt);
//
//		m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = xyArrDimTemp;
//	}
//	else
//		m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER];
	CPlateGirderApp		*pGirCen	= m_pDB->GetGirder(-1);

	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	double dSttBlockOutWidthLeft = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
	double dSttBlockOutWidthRigh = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdStt = GetLengthCoverSd(TRUE);

	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidthLeft = 0;
		dSttBlockOutWidthRigh = 0;
	}

	CDPoint xyDim, xyDimStt, xyDimEnd;
	CDPointArray xyArrDimTemp;
	double dStaStt = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[0], dCoverSdStt+dSttBlockOutWidthRigh, dDisRStt);
	CDPointArray xyArrDimUpRigh = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER], (dDisLStt+dDisRStt)/2, 1);
	// 상부 시종점의 피복위치 보정(오프셋영향)
	if(IsBridgeHasCurveLine())
	{
		if(!IsSkewStt())
		{
			xyDim = xyArrDimUpRigh[0];
			xyDimStt.x = 1;
			xyDimStt.y = GetCoverSdToModify(TRUE, FALSE);
			xyDimEnd.x = xyDim.x-1;
			xyDimEnd.y = xyDim.y;
			xyArrDimTemp.Add(xyDimStt);
			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
			for (long a=1; a<xyArrDimUpRigh.GetSize()-1; a++)
				xyArrDimTemp.Add(xyArrDimUpRigh[a]);
		}
		else
		{
			xyArrDimTemp.Add(xyArrDimUpRigh[0]);
			for (long a=1; a<xyArrDimUpRigh.GetSize()-1; a++)
				xyArrDimTemp.Add(xyArrDimUpRigh[a]);			
		}

		if(!IsSkewEnd())
		{		
			xyDim = xyArrDimUpRigh[xyArrDimUpRigh.GetUpperBound()];
			xyDimStt.x = 1;
			xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, FALSE);
			xyDimEnd.x = xyDim.x-1;
			xyDimEnd.y = xyDim.y;
			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
			xyArrDimTemp.Add(xyDimStt);
		}
		else
		{
			xyArrDimTemp.Add(xyArrDimUpRigh[xyArrDimUpRigh.GetUpperBound()]);
		}

		m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = xyArrDimTemp;
	}
	else
		m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER];

	//////////////////////////////////////////////////////////////////////////
		
	dStaStt = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[0], dCoverSdStt+dSttBlockOutWidthLeft, dDisLStt);
	CDPointArray xyArrDimUpLeft = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER], (dDisLStt+dDisRStt)/2, -1);

	// 상부 시종점의 피복위치 보정(오프셋영향)
	if(IsBridgeHasCurveLine())
	{
		if(!IsSkewStt())
		{
			xyDim = xyArrDimUpLeft[0];
			xyDimStt.x = 1;
			xyDimStt.y = xyDim.y-GetCoverSdToModify(TRUE, TRUE);
			xyDimEnd.x = xyDim.x-1;
			xyDimEnd.y = xyDim.y;
			xyArrDimTemp.Add(xyDimStt);
			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
			for (long a=1; a<xyArrDimUpLeft.GetSize()-1; a++)
				xyArrDimTemp.Add(xyArrDimUpLeft[a]);
		}
		else
		{
			xyArrDimTemp.Add(xyArrDimUpLeft[0]);
			for (long a=1; a<xyArrDimUpLeft.GetSize()-1; a++)
				xyArrDimTemp.Add(xyArrDimUpLeft[a]);
		}
		if(!IsSkewEnd())
		{
			xyDim = xyArrDimUpLeft[xyArrDimUpLeft.GetUpperBound()];
			xyDimStt.x = 1;
			xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, TRUE);
			xyDimEnd.x = xyDim.x-1;
			xyDimEnd.y = xyDim.y;
			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
			xyArrDimTemp.Add(xyDimStt);
		}
		else
		{
			xyArrDimTemp.Add(xyArrDimUpLeft[xyArrDimUpLeft.GetUpperBound()]);
		}

		m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = xyArrDimTemp;
	}
	else
		m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER];
	

	// 하부주철근 간격
	m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER].RemoveAll();

	CDPointArray xyArrDimSttL, xyArrDimSttR, xyArrDimEndL, xyArrDimEndR;
	CalcCTCMainRebarAtSinchuk(TRUE, xyArrDimSttL, xyArrDimSttR);
	CalcCTCMainRebarAtSinchuk(FALSE, xyArrDimEndL, xyArrDimEndR);
	long n = 0;
	// 좌측
	for (n=0; n<xyArrDimSttL.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(xyArrDimSttL[n]);
	for ( n=0; n<m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetAt(n));
	for ( n=0; n<xyArrDimEndL.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(xyArrDimEndL[n]);
	// 우측
	for ( n=0; n<xyArrDimSttR.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(xyArrDimSttR[n]);
	for ( n=0; n<m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetAt(n));
	for ( n=0; n<xyArrDimEndR.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(xyArrDimEndR[n]);

	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT], m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT]);	
}




void CADeckData::CalcCTCMainRebarBySlabLeftRighCTCBySlabUpper(BOOL bLeftBase)
{
//	CPlateGirderApp		*pGirCen	= m_pDB->GetGirder(-1);
//
//	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
//	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
//	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
//	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
//	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
//	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
//	double dDisLEnd		  = m_pDB->GetDisSlabLeftAct(dStaSlabEndCen)+m_dCoverSd;
//	double dDisREnd		  = m_pDB->GetDisSlabRighAct(dStaSlabEndCen)-m_dCoverSd;
//	CDPoint	vAngStt		  = pGirCen->GetAAA(OFF_STT_SLAB);
//	CDPoint	vAngEnd		  = pGirCen->GetAAA(OFF_END_SLAB);
//	double dStaSlabSttLeft= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
//	double dStaSlabSttRigh= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
//	double dStaSlabEndLeft= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
//	double dStaSlabEndRigh= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
//
//	double dSttBlockOutWidthLeft = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
//	double dSttBlockOutWidthRigh = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
//	double dEndBlockOutWidthLeft = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT);
//	double dEndBlockOutWidthRigh = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT);
//	double dCoverSdStt = GetLengthCoverSd(TRUE); 
//	double dCoverSdEnd = GetLengthCoverSd(FALSE); 
//
//	CDPointArray xyArrUp;						// 상면철근간격
//	CDPointArray xyArrUpR;						// 상면철근간격우측
//	CDPointArray xyArrDn;
//	CDPointArray xyArrDnR;
//
//	if(bLeftBase)		// 좌측슬래브 위치가 기준인 경우
//	{
//		double dSpaceFrStt = dCoverSdStt+dSttBlockOutWidthRigh;
//		double dStaStt	   = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[0], dSpaceFrStt, dDisRStt);
//		CDPointArray xyArrDimUpRigh = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT], dDisLStt, 1);
//		// 상부 시종점의 피복위치 보정(오프셋영향)
//		if(IsBridgeHasCurveLine())
//		{
//			CDPointArray xyArrDimTemp;
//			CDPoint xyDim = xyArrDimUpRigh[0];
//			CDPoint xyDimStt, xyDimEnd;
//			xyDimStt.x = 1;
//			xyDimStt.y = GetCoverSdToModifyAtExp(TRUE, FALSE, m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetAt(0));
//			xyDimEnd.x = xyDim.x-1;
//			xyDimEnd.y = xyDim.y;
//			xyArrDimTemp.Add(xyDimStt);
//			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//			for (long a=1; a<xyArrDimUpRigh.GetSize()-1; a++)
//				xyArrDimTemp.Add(xyArrDimUpRigh[a]);
//			
//			xyDim = xyArrDimUpRigh[xyArrDimUpRigh.GetUpperBound()];
//			xyDimStt.x = 1;
//			xyDimStt.y = GetCoverSdToModifyAtExp(FALSE, FALSE, m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetAt(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetUpperBound()));
//			xyDimEnd.x = xyDim.x-1;
//			xyDimEnd.y = xyDim.y;
//			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//			xyArrDimTemp.Add(xyDimStt);
//
//			m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = xyArrDimTemp;
//		}
//		else
//			m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = xyArrDimUpRigh;
//	}
//	else								// 우측슬래브 위치의 곡률반경이 큰 경우  
//	{
//		double dSpaceFrStt = dCoverSdStt+dSttBlockOutWidthLeft;
//		double dStaStt	   = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[0], dSpaceFrStt, dDisLStt);
//		CDPointArray xyArrDimUpLeft = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT], dDisRStt, -1);
//
//		// 상부 시종점의 피복위치 보정(오프셋영향)
//		if(IsBridgeHasCurveLine())
//		{
//			CDPointArray xyArrDimTemp;
//			CDPoint xyDim = xyArrDimUpLeft[0];
//			CDPoint xyDimStt, xyDimEnd;
//			xyDimStt.x = 1;
//			xyDimStt.y = GetCoverSdToModifyAtExp(TRUE, TRUE, m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetAt(0));
//			xyDimEnd.x = xyDim.x-1;
//			xyDimEnd.y = xyDim.y;
//			xyArrDimTemp.Add(xyDimStt);
//			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//			for (long a=1; a<xyArrDimUpLeft.GetSize()-1; a++)
//				xyArrDimTemp.Add(xyArrDimUpLeft[a]);
//			
//			xyDim = xyArrDimUpLeft[xyArrDimUpLeft.GetUpperBound()];
//			xyDimStt.x = 1;
//			xyDimStt.y = GetCoverSdToModifyAtExp(FALSE, TRUE, m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetAt(m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetUpperBound()));
//			xyDimEnd.x = xyDim.x-1;
//			xyDimEnd.y = xyDim.y;
//			if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
//			xyArrDimTemp.Add(xyDimStt);
//
//			m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = xyArrDimTemp;
//		}
//		else
//			m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = xyArrDimUpLeft;
//	}
//	
//	m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT], m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT]);

	CPlateGirderApp		*pGirCen	= m_pDB->GetGirder(-1);

	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	double dSttBlockOutWidthLeft = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
	double dSttBlockOutWidthRigh = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdStt = GetLengthCoverSd(TRUE);

	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidthLeft = 0;
		dSttBlockOutWidthRigh = 0;
	}
	
	CDPointArray xyArrUp, xyArrUpR, xyArrDn, xyArrDnR, xyArrDimTemp, xyArrDim;
	CDPoint xyDim, xyDimStt, xyDimEnd;
	double dStaStt = 0;

	if(bLeftBase)		// 좌측슬래브 위치가 기준인 경우
	{
		dStaStt  = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[0], dCoverSdStt+dSttBlockOutWidthRigh, dDisRStt);
		xyArrDim = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT], dDisLStt, 1);
		// 상부 시종점의 피복위치 보정(오프셋영향)
		if(IsBridgeHasCurveLine())
		{
			if(!IsSkewStt())
			{	
				xyDim = xyArrDim[0];
				xyDimStt.x = 1;
				xyDimStt.y = GetCoverSdToModify(TRUE, FALSE);
				xyDimEnd.x = xyDim.x-1;
				xyDimEnd.y = xyDim.y;
				xyArrDimTemp.Add(xyDimStt);
				if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
				for (long a=1; a<xyArrDim.GetSize()-1; a++)
					xyArrDimTemp.Add(xyArrDim[a]);
			}
			else
			{
				xyArrDimTemp.Add(xyArrDim[0]);
				for (long a=1; a<xyArrDim.GetSize()-1; a++)
					xyArrDimTemp.Add(xyArrDim[a]);
			}			

			if(!IsSkewEnd())
			{
				xyDim = xyArrDim[xyArrDim.GetUpperBound()];
				xyDimStt.x = 1;
				xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, FALSE);
				xyDimEnd.x = xyDim.x-1;
				xyDimEnd.y = xyDim.y;
				if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
				xyArrDimTemp.Add(xyDimStt);
			}
			else
			{
				xyArrDimTemp.Add(xyArrDim[xyArrDim.GetUpperBound()]);
			}

			m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = xyArrDimTemp;
		}
		else
			m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT] = xyArrDim;
	}
	else								// 우측슬래브 위치의 곡률반경이 큰 경우  
	{
		dStaStt	 = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[0], dCoverSdStt+dSttBlockOutWidthLeft, dDisLStt);
		xyArrDim = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT], dDisRStt, -1);

		// 상부 시종점의 피복위치 보정(오프셋영향)
		if(IsBridgeHasCurveLine())
		{
			if(!IsSkewStt())
			{
				xyDim = xyArrDim[0];
				xyDimStt.x = 1;
				xyDimStt.y = xyDim.y-GetCoverSdToModify(TRUE, TRUE);
				xyDimEnd.x = xyDim.x-1;
				xyDimEnd.y = xyDim.y;
				xyArrDimTemp.Add(xyDimStt);
				if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
				for (long a=1; a<xyArrDim.GetSize()-1; a++)
					xyArrDimTemp.Add(xyArrDim[a]);
			}
			else
			{
				xyArrDimTemp.Add(xyArrDim[0]);
				for (long a=1; a<xyArrDim.GetSize()-1; a++)
					xyArrDimTemp.Add(xyArrDim[a]);
			}
			if(!IsSkewEnd())
			{
				xyDim = xyArrDim[xyArrDim.GetUpperBound()];
				xyDimStt.x = 1;
				xyDimStt.y = xyDim.y-GetCoverSdToModify(FALSE, TRUE);
				xyDimEnd.x = xyDim.x-1;
				xyDimEnd.y = xyDim.y;
				if(xyDim.x>1)	xyArrDimTemp.Add(xyDimEnd);
				xyArrDimTemp.Add(xyDimStt);
			}
			else
			{
				xyArrDimTemp.Add(xyArrDim[xyArrDim.GetUpperBound()]);
			}
			m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = xyArrDimTemp;
		}
		else
			m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT] = xyArrDim;
	}
	
	m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT], m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT]);


	// 하부주철근 간격
	m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].RemoveAll();
	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER].RemoveAll();
	
	CDPointArray xyArrDimSttL, xyArrDimSttR, xyArrDimEndL, xyArrDimEndR;
	CalcCTCMainRebarAtSinchuk(TRUE, xyArrDimSttL, xyArrDimSttR);
	CalcCTCMainRebarAtSinchuk(FALSE, xyArrDimEndL, xyArrDimEndR);
	long n = 0;
	// 좌측
	for (n=0; n<xyArrDimSttL.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(xyArrDimSttL[n]);
	for ( n=0; n<m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT].GetAt(n));
	for ( n=0; n<xyArrDimEndL.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT].Add(xyArrDimEndL[n]);
	// 우측
	for ( n=0; n<xyArrDimSttR.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(xyArrDimSttR[n]);
	for ( n=0; n<m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT].GetAt(n));
	for ( n=0; n<xyArrDimEndR.GetSize(); n++)
		m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT].Add(xyArrDimEndR[n]);

	m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER] = GetDimArrCenByLeftRight(m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT], m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT]);	
}





// 주철근의간격을 계산함
void CADeckData::CalcCTCMainRebarForDim()
{
	m_arrCTCMainRebarForDim[CTC_MAIN_UPPER_LEFT].RemoveAll();
	m_arrCTCMainRebarForDim[CTC_MAIN_UPPER_RIGHT].RemoveAll();
	m_arrCTCMainRebarForDim[CTC_MAIN_UPPER_CENTER].RemoveAll();
	m_arrCTCMainRebarForDim[CTC_MAIN_LOWER_LEFT].RemoveAll();
	m_arrCTCMainRebarForDim[CTC_MAIN_LOWER_RIGHT].RemoveAll();
	m_arrCTCMainRebarForDim[CTC_MAIN_LOWER_CENTER].RemoveAll();

	CDoubleArray dArrDim;
	CRebarPlacing RebarPlace;
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[CTC_MAIN_UPPER_LEFT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarForDim[CTC_MAIN_UPPER_LEFT], m_dErrorRangeCTC);		// 크로소이드로 인한 오차.

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[CTC_MAIN_UPPER_RIGHT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarForDim[CTC_MAIN_UPPER_RIGHT], m_dErrorRangeCTC);		// 크로소이드로 인한 오차.

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[CTC_MAIN_UPPER_CENTER], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarForDim[CTC_MAIN_UPPER_CENTER], m_dErrorRangeCTC);		// 크로소이드로 인한 오차.

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[CTC_MAIN_LOWER_LEFT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarForDim[CTC_MAIN_LOWER_LEFT], m_dErrorRangeCTC);		// 크로소이드로 인한 오차.

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarForDim[CTC_MAIN_LOWER_RIGHT], m_dErrorRangeCTC);		// 크로소이드로 인한 오차.

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[CTC_MAIN_LOWER_CENTER], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarForDim[CTC_MAIN_LOWER_CENTER], m_dErrorRangeCTC);		// 크로소이드로 인한 오차.

}


// 주철근의간격을 계산함
void CADeckData::CalcCTCMainRebarAtSkewForDim()
{
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_LEFT_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_LEFT_END].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_LEFT_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_LEFT_END].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_RIGH_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_RIGH_END].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_RIGH_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_RIGH_END].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_CENTER_END].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_CENTER_END].RemoveAll();
	m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_UPPER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_UPPER_CENTER_END].RemoveAll();
	m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_LOWER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_LOWER_CENTER_END].RemoveAll();

	CDoubleArray dArrDim;
	CRebarPlacing RebarPlace;
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_LEFT_STT], m_dErrorRangeCTC);		

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_LEFT_END], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_LEFT_STT], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_LEFT_END], m_dErrorRangeCTC);		

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_RIGH_STT], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_RIGH_END], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_RIGH_STT], m_dErrorRangeCTC);		

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_RIGH_END], m_dErrorRangeCTC);	
	
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_CENTER_STT], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_UPPER_CENTER_END], m_dErrorRangeCTC);		

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_CENTER_STT], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_LOWER_CENTER_END], m_dErrorRangeCTC);	


	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_UPPER_CENTER_STT], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_UPPER_CENTER_END], m_dErrorRangeCTC);		

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_STT], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_LOWER_CENTER_STT], m_dErrorRangeCTC);	

	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_END], dArrDim, FALSE);	
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrDim, m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_LOWER_CENTER_END], m_dErrorRangeCTC);
}


// 슬래브 중심위치기준 Offset
void CADeckData::CalcCTCSabogangRebar()
{
	double dSttBlockOutWidth = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABCENTER);
	double dEndBlockOutWidth = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABCENTER);
	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth = 0;
		dEndBlockOutWidth = 0;
	}
	double dCoverSdStt = GetLengthCoverSd(TRUE); 
	double dCoverSdEnd = GetLengthCoverSd(FALSE); 

	m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT].RemoveAll();
	m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END].RemoveAll();
	m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT].RemoveAll();
	m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END].RemoveAll();

	double dCTC		  = m_dEndMainRebarCTC[0]/2;
	double dRangeSkew = m_dRangeSabogangRebarStt-dCoverSdStt; 
	long   nRebar	  = long(dRangeSkew/dCTC);

	CDPoint	xyDim;
	xyDim.x = nRebar;
	xyDim.y = dCTC;

	m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT].Add(xyDim);
	CalcCTCChildByCTCParent(m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT], m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT], dSttBlockOutWidth, 0);

	dCTC	   = m_dEndMainRebarCTC[0]/2;
	dRangeSkew = m_dRangeSabogangRebarEnd-dCoverSdEnd;
	nRebar	   = long(dRangeSkew/dCTC);

	xyDim.x = nRebar;
	xyDim.y = dCTC;
	m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END].Add(xyDim);
	CalcCTCChildByCTCParent(m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END], m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END], dEndBlockOutWidth, 0);
	CDPointArray xyArrUp = m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END];
	//뒤집기
	m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END].RemoveAll();
	for(long n=0; n<xyArrUp.GetSize(); n++)
		m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END].Add(xyArrUp[n]);

	if(m_bInstallMainRebarAtBlockOut)
	{
		m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT]	= m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT];
		m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END]	= m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END];
	}
}


// 최대 곡률반경 위치를 기준으로 ctc생성
void CADeckData::CalcCTCMainRebarAtSkew()
{
	CPlateGirderApp	*pGirCen = m_pDB->GetGirder(-1);
	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);
	double dDisLStt = GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDisRStt = GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDisLEnd = GetSideBarDistFromCenterGir(FALSE, TRUE);
	double dDisREnd = GetSideBarDistFromCenterGir(FALSE, FALSE);
	double dSttExpWLeft = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
	double dSttExpWRigh = GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
	double dEndExpWLeft = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT);
	double dEndExpWRigh = GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT);
	CDPoint		vAngStt = pGirCen->GetAngleStt();
	CDPoint		vAngEnd = pGirCen->GetAngleEnd();
	CDPoint xyDimFir, xyDimSec, xyDimBase;
	double dCoverSdSttL = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT); 
	double dCoverSdEndL = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT); 
	double dCoverSdSttR = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT); 
	double dCoverSdEndR = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT); 

	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_STT].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_STT].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_END].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END].RemoveAll();
	m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_END].RemoveAll();
	m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_STT].RemoveAll();
	m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_END].RemoveAll();

	//=========================================================================
	// 사각부 주철근 배근가능 전체범위
	double dStaSlabSttCen	= pGirCen->GetSSS(OFF_STT_SLAB);
	double dStaSlabEndCen	= pGirCen->GetSSS(OFF_END_SLAB);
	double dStaSlabSttLeft	= m_pDB->m_pLine->GetStationByScInLine(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh	= m_pDB->m_pLine->GetStationByScInLine(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndLeft	= m_pDB->m_pLine->GetStationByScInLine(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh	= m_pDB->m_pLine->GetStationByScInLine(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dLengthSkewCenStt= m_pDB->m_pLine->GetLengthByDis(dStaSlabSttLeft, dStaSlabSttRigh, (dDisLStt+dDisRStt)/2);
	double dLengthSkewCenEnd= m_pDB->m_pLine->GetLengthByDis(dStaSlabEndLeft, dStaSlabEndRigh, (dDisLEnd+dDisREnd)/2);

	//=========================================================================

	double dCTC = m_dEndMainRebarCTC[POS_SLAB_CENTER]/2;
	long nRebar = long(m_dRangeMainRebarAtSkewStt/dCTC);
	CDPoint	xyDimOrigin(nRebar, dCTC);
	
	//////////////////////////////////////////////////////////////////////////
	// 시점부 (반경이 큰 쪽이 CTC기준)
	double dLengthLeft = m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[0], dArrStaLeft[1], dDisLStt);
	double dLengthRigh = m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[0], dArrStaRigh[1], dDisRStt);
	double dStaStt = 0;
	CDPointArray xyArrDimUpLeft, xyArrDimUpRigh, xyArrDimLoLeft, xyArrDimLoRigh;
	
	xyArrDimUpLeft.Add(xyDimOrigin);
	xyArrDimUpRigh.Add(xyDimOrigin);

	if(dLengthLeft >= dLengthRigh)
	{
		// 상부
		dStaStt = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[0], dCoverSdSttL+dSttExpWLeft, dDisLStt);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT]   = xyArrDimUpLeft;	
		xyArrDimUpRigh = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT] = GetDimArrAtMultiCurve(dStaStt, xyArrDimUpLeft, dDisLStt, 1);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT] = GetDimArrCenByLeftRight(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT], m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT]);
		// 하부
		// 하부 ( APLATE-704로 인한 수정)
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_STT] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_STT] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_STT] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT];

		// 사각부주철근배치 범위가 슬래브중앙부를 초과할때 중앙부사각부주철근 처리
		if(dLengthSkewCenStt/2 < m_dRangeMainRebarAtSkewStt)
		{
			xyArrDimUpLeft.RemoveAll();
			xyArrDimUpRigh.RemoveAll();
			xyArrDimLoLeft.RemoveAll();
			xyArrDimLoRigh.RemoveAll();
			
			nRebar = long((dLengthSkewCenStt/2)/dCTC);
			xyArrDimUpLeft.Add(CDPoint(nRebar, dCTC));
			xyArrDimUpRigh	= GetDimArrAtMultiCurve(dStaStt, xyArrDimUpLeft, dDisLStt, 1);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_STT] = GetDimArrCenByLeftRight(xyArrDimUpLeft, xyArrDimUpRigh);

			CalcCTCChildByCTCParent(xyArrDimUpLeft, xyArrDimLoLeft, dSttExpWLeft, 0);
			xyArrDimLoRigh	= GetDimArrAtMultiCurve(dStaStt, xyArrDimLoLeft, dDisLStt, 1, FALSE);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_STT] = GetDimArrCenByLeftRight(xyArrDimLoLeft, xyArrDimLoRigh);
		}
		else
		{
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_STT]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT];
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_STT]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_STT];
		}
	}
	else
	{
		// 상부
		dStaStt  = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[0], dCoverSdSttR+dSttExpWRigh, dDisRStt);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT]   = xyArrDimUpRigh;	
		xyArrDimUpLeft = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT] = GetDimArrAtMultiCurve(dStaStt, xyArrDimUpRigh, dDisRStt, -1);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT] = GetDimArrCenByLeftRight(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT], m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT]);

		// 하부 ( APLATE-704로 인한 수정)
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_STT] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_STT] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_STT] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT];

		// 사각부주철근배치 범위가 슬래브중앙부를 초과할때 중앙부사각부주철근 처리
		if(dLengthSkewCenStt/2 < m_dRangeMainRebarAtSkewStt)
		{
			xyArrDimUpLeft.RemoveAll();
			xyArrDimUpRigh.RemoveAll();
			xyArrDimLoLeft.RemoveAll();
			xyArrDimLoRigh.RemoveAll();
			
			nRebar = long((dLengthSkewCenStt/2)/dCTC);
			xyArrDimUpRigh.Add(CDPoint(nRebar, dCTC));
			xyArrDimUpLeft	= GetDimArrAtMultiCurve(dStaStt, xyArrDimUpRigh, dDisRStt, -1);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_STT] = GetDimArrCenByLeftRight(xyArrDimUpLeft, xyArrDimUpRigh);

			CalcCTCChildByCTCParent(xyArrDimUpRigh, xyArrDimLoRigh, dSttExpWRigh, 0);
			xyArrDimLoLeft	= GetDimArrAtMultiCurve(dStaStt, xyArrDimLoRigh, dDisRStt, -1, FALSE);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_STT] = GetDimArrCenByLeftRight(xyArrDimLoLeft, xyArrDimLoRigh);
		}
		else
		{
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_STT]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT];
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_STT]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_STT];
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 종점부 (반경이 큰 쪽이 CTC기준)
	xyArrDimUpLeft.RemoveAll();
	xyArrDimUpRigh.RemoveAll();

	nRebar = long(m_dRangeMainRebarAtSkewEnd/dCTC);
	xyDimBase.x = nRebar;
	xyDimBase.y = dCTC;

	xyArrDimUpLeft.Add(xyDimBase);
	xyArrDimUpRigh.Add(xyDimBase);

	dLengthLeft = m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[dArrStaLeft.GetUpperBound()-1], dArrStaLeft[dArrStaLeft.GetUpperBound()], dDisLEnd);
	dLengthRigh = m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[dArrStaRigh.GetUpperBound()-1], dArrStaRigh[dArrStaRigh.GetUpperBound()], dDisREnd);
	
	if(dLengthLeft >= dLengthRigh)
	{
		// 상부
		dStaStt = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[dArrStaLeft.GetUpperBound()], -dEndExpWLeft-dCoverSdEndL, dDisLEnd);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END]   = xyArrDimUpLeft;	
		xyArrDimUpRigh = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END]   = GetDimArrAtMultiCurve(dStaStt, xyArrDimUpLeft, dDisLEnd, 1);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END] = GetDimArrCenByLeftRight(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END], m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END]);

		// 하부 ( APLATE-704로 인한 수정)
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_END] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END];
// 
// 		CalcCTCChildByCTCParent(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END], m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END], dEndExpWLeft, 0);
// 		dStaStt = pGirCen->GetLine()->GetStationByDisLength(dArrStaLeft[dArrStaLeft.GetUpperBound()], -dCoverSdEndL, dDisLEnd);
// 		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_END]   = GetDimArrAtMultiCurve(dStaStt, m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END], dDisLEnd, 1);
// 		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END] = GetDimArrCenByLeftRight(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END], m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_END]);

		// 사각부주철근배치 범위가 슬래브중앙부를 초과할때 중앙부사각부주철근 처리
		if(dLengthSkewCenEnd/2 < m_dRangeMainRebarAtSkewEnd)
		{
			xyArrDimUpLeft.RemoveAll();
			xyArrDimUpRigh.RemoveAll();
			xyArrDimLoLeft.RemoveAll();
			xyArrDimLoRigh.RemoveAll();
			
			nRebar = long((dLengthSkewCenEnd/2)/dCTC);
			xyArrDimUpLeft.Add(CDPoint(nRebar, dCTC));
			xyArrDimUpRigh	= GetDimArrAtMultiCurve(dStaStt, xyArrDimUpLeft, dDisLEnd, 1);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_END] = GetDimArrCenByLeftRight(xyArrDimUpLeft, xyArrDimUpRigh);

			CalcCTCChildByCTCParent(xyArrDimUpLeft, xyArrDimLoLeft, dEndExpWLeft, 0);
			xyArrDimLoRigh	= GetDimArrAtMultiCurve(dStaStt, xyArrDimLoLeft, dDisLEnd, 1);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_END] = GetDimArrCenByLeftRight(xyArrDimLoLeft, xyArrDimLoRigh);
		}
		else
		{
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_END]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END];
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_END]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END];
		}
	}
	else
	{
		// 상부
		dStaStt = pGirCen->GetLine()->GetStationByDisLength(dArrStaRigh[dArrStaRigh.GetUpperBound()], -dEndExpWRigh-dCoverSdEndR, dDisREnd);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END]   = xyArrDimUpRigh;	
		xyArrDimUpLeft = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END]   = GetDimArrAtMultiCurve(dStaStt, xyArrDimUpRigh, dDisREnd, -1);
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END] = GetDimArrCenByLeftRight(m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END], m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END]);
	
		// 하부 ( APLATE-704로 인한 수정)
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_END] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END] = m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END];
		
		// 사각부주철근배치 범위가 슬래브중앙부를 초과할때 중앙부사각부주철근 처리
		if(dLengthSkewCenEnd/2 < m_dRangeMainRebarAtSkewEnd)
		{
			xyArrDimUpLeft.RemoveAll();
			xyArrDimUpRigh.RemoveAll();
			xyArrDimLoLeft.RemoveAll();
			xyArrDimLoRigh.RemoveAll();
			
			nRebar = long((dLengthSkewCenEnd/2)/dCTC);
			xyArrDimUpRigh.Add(CDPoint(nRebar, dCTC));
			xyArrDimUpLeft	= GetDimArrAtMultiCurve(dStaStt, xyArrDimUpRigh, dDisREnd, -1);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_END] = GetDimArrCenByLeftRight(xyArrDimUpLeft, xyArrDimUpRigh);

			CalcCTCChildByCTCParent(xyArrDimUpRigh, xyArrDimLoRigh, dEndExpWRigh, 0);
			xyArrDimLoRigh	= GetDimArrAtMultiCurve(dStaStt, xyArrDimLoLeft, dDisREnd, -1);
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_END] = GetDimArrCenByLeftRight(xyArrDimLoLeft, xyArrDimLoRigh);
		}
		else
		{
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_UPPER_CENTER_END]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END];
			m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_LOWER_CENTER_END]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END];
		}
	}

	if(m_bInstallMainRebarAtBlockOut)
	{
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_STT]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_STT];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_STT]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_STT];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_STT]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_STT];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_LEFT_END]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_LEFT_END];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_RIGH_END]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_RIGH_END];
		m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_LOWER_CENTER_END]	= m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_UPPER_CENTER_END];
	}

	CalcCTCMainRebarAtSkewForDim();
}




void CADeckData::CalcCTCMainRebarAtSinchuk(BOOL bStt, CDPointArray &xyArrDimL, CDPointArray &xyArrDimR)
{
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();

	double dStaSlabSttCen = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = m_pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  =  GetSideBarDistFromCenterGir(SECTION_DAN, TRUE);	 //GetDistByJongBase(JONGBASEPOS_SLABLEFT);
	double dDisRStt		  = GetSideBarDistFromCenterGir(SECTION_DAN, FALSE);	//GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	long nGugansu = dArrStaLeft.GetUpperBound();	// 곡선반경 구간수
	double dLengthLeft = m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[bStt?0:nGugansu-1], dArrStaLeft[bStt?1:nGugansu], dDisLStt);
	double dLengthRigh = m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[bStt?0:nGugansu-1], dArrStaRigh[bStt?1:nGugansu], dDisRStt);
	BOOL   bBaseLeft     = dLengthLeft>dLengthRigh ? TRUE : FALSE;
	double dBlockOutWidthLeft = GetWidthBlockOut(bStt, JONGBASEPOS_SLABLEFT);
	double dBlockOutWidthRigh = GetWidthBlockOut(bStt, JONGBASEPOS_SLABRIGHT);
	double dBlockOutWidthCent = GetWidthBlockOut(bStt, JONGBASEPOS_SLABCENTER);
	double dSttBlockOutW = 0, dEndBlockOutW = 0;
	if(m_nJongCtcCalcPos==0)
	{
		dSttBlockOutW = pGlopt->GetSttExpansionJointWidth();
		dEndBlockOutW = pGlopt->GetEndExpansionJointWidth();
	}
	else if(m_nJongCtcCalcPos==1)
	{
		dSttBlockOutW = dBlockOutWidthCent;
		dEndBlockOutW = dBlockOutWidthCent;
	}
	else if(m_nJongCtcCalcPos==2)
	{
		dSttBlockOutW = bBaseLeft ? dBlockOutWidthLeft : dBlockOutWidthRigh;
		dEndBlockOutW = bBaseLeft ? dBlockOutWidthLeft : dBlockOutWidthRigh;
	}

	if(m_nJongCtcCalcPos==0 || m_nJongCtcCalcPos==1)
	{
		if(bStt && IsSkewStt())
		{
			double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).GetAngleDegree();
			if((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360))
				dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, dBlockOutWidthRigh);
			if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
				dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, dBlockOutWidthLeft);
		}
		else if(!bStt && IsSkewEnd())
		{
			double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
			if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
				dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABCENTER, dBlockOutWidthRigh);
			if((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360))
				dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen,JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABCENTER, dBlockOutWidthLeft);
		}
	}
	else
	{
		if(bStt && IsSkewStt())
		{
			double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).GetAngleDegree();
			if(bBaseLeft && (90<dAngle && dAngle<180) || (270<dAngle && dAngle<360))
				dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, dBlockOutWidthRigh);
			if(!bBaseLeft && (0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
				dSttBlockOutW = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, dBlockOutWidthLeft);
		}
		else if(!bStt && IsSkewEnd())
		{
			double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
			if(bBaseLeft && (0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
				dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, dBlockOutWidthRigh);
			if(!bBaseLeft && (90<dAngle && dAngle<180) || (270<dAngle && dAngle<360))
				dEndBlockOutW = GetLengthBySlabEndRadius(dStaSlabEndCen,JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, dBlockOutWidthLeft);
		}
	}

	// 하부주철근 간격계산
	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutW	= 0;
		dEndBlockOutW	= 0;
		dBlockOutWidthLeft	= 0;
		dBlockOutWidthRigh	= 0;
		dBlockOutWidthCent	= 0;
	}

	double dBase = 0;
	if(bStt)	dBase = !IsSkewStt() ? dBlockOutWidthCent : dSttBlockOutW;
	else		dBase = !IsSkewEnd() ? dBlockOutWidthCent : dEndBlockOutW;
	if(dBlockOutWidthCent==0)	return;


	double dCTCMainDan = m_dEndMainRebarCTCLower[POS_SLAB_CENTER]/2;	// 단부 주철근 간격
	long   nRebar      = long(Round(dBase,3)/dCTCMainDan);
	double dExt        = (dBase-nRebar*dCTCMainDan)/2;

	// 상부주철근과 사이클을 맞추기 위해서 신축이음부 주철근수가 홀수가 되어야함.
	CDPointArray xyArrDim;
	if(dExt > REBAR_MIN_SPACE)	
	{
		if(nRebar%2==0)
		{
			xyArrDim.Add(CDPoint(1, dExt+dCTCMainDan/2));
			if(nRebar-1>0)
				xyArrDim.Add(CDPoint(nRebar-1, dCTCMainDan));
			xyArrDim.Add(CDPoint(1, dExt+dCTCMainDan/2));
		}
		else
		{
			xyArrDim.Add(CDPoint(1, dExt));
			xyArrDim.Add(CDPoint(nRebar, dCTCMainDan));
			xyArrDim.Add(CDPoint(1, dExt));
		}
	}
	else
	{
		if(nRebar%2==0)
		{
			xyArrDim.Add(CDPoint(1, dExt+dCTCMainDan));
			if(nRebar-2>0)
				xyArrDim.Add(CDPoint(nRebar-2, dCTCMainDan));
			xyArrDim.Add(CDPoint(1, dExt+dCTCMainDan));
		}
		else
		{
			xyArrDim.Add(CDPoint(1, dExt+dCTCMainDan/2));
			if(nRebar-1>0)
				xyArrDim.Add(CDPoint(nRebar-1, dCTCMainDan));
			xyArrDim.Add(CDPoint(1, dExt+dCTCMainDan/2));
		}
	}
	

	if(!IsSkewStt() || !IsBridgeHasCurveLine())	// offset
	{
		xyArrDimL = xyArrDim;
		xyArrDimR = xyArrDim;
	}
	else		// 방사
	{
		// 시점
		if(m_nJongCtcCalcPos == 2)
		{
			double dLeftRad = m_pDB->m_pLine->GetRadius_LineInfo(m_pDB->GetGirder(-1)->GetSSS(bStt?OFF_STT_SLAB:OFF_END_SLAB)) - GetDistByJongBase(JONGBASEPOS_SLABLEFT);
			double dRighRad = m_pDB->m_pLine->GetRadius_LineInfo(m_pDB->GetGirder(-1)->GetSSS(bStt?OFF_STT_SLAB:OFF_END_SLAB)) - GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
			if(dLeftRad>dRighRad)
			{
				xyArrDimL = xyArrDim;
				for (long n=0; n<xyArrDim.GetSize(); n++)
				{
					long nRebar = (long)xyArrDim[n].x;
					double dCTC = (xyArrDim[n].y*dRighRad)/dLeftRad;
					xyArrDimR.Add(CDPoint(nRebar, dCTC));
				}
			}
			else
			{
				xyArrDimR = xyArrDim;
				for (long n=0; n<xyArrDim.GetSize(); n++)
				{
					long nRebar = (long)xyArrDim[n].x;
					double dCTC = (xyArrDim[n].y*dRighRad)/dLeftRad;
					xyArrDimL.Add(CDPoint(nRebar, dCTC));
				}
			}
		}
		else
		{
			// 시점
			double dLeftRad = m_pDB->m_pLine->GetRadius_LineInfo(m_pDB->GetGirder(-1)->GetSSS(bStt?OFF_STT_SLAB:OFF_END_SLAB)) - GetDistByJongBase(JONGBASEPOS_SLABLEFT);
			double dCentRad = m_pDB->m_pLine->GetRadius_LineInfo(m_pDB->GetGirder(-1)->GetSSS(bStt?OFF_STT_SLAB:OFF_END_SLAB)) - GetDistByJongBase(JONGBASEPOS_SLABCENTER);
			double dRighRad = m_pDB->m_pLine->GetRadius_LineInfo(m_pDB->GetGirder(-1)->GetSSS(bStt?OFF_STT_SLAB:OFF_END_SLAB)) - GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
			for (long n=0; n<xyArrDim.GetSize(); n++)
			{
				long nRebar = (long)xyArrDim[n].x;
				double dCTCLeft = (xyArrDim[n].y*dLeftRad)/dCentRad;
				double dCTCRigh = (xyArrDim[n].y*dRighRad)/dCentRad;
				xyArrDimL.Add(CDPoint(nRebar, dCTCLeft));
				xyArrDimR.Add(CDPoint(nRebar, dCTCRigh));
			}
		}
	}

}


void CADeckData::CalcCTCSinchukRebar()
{
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();

	m_arrCTCMainRebarUnderSinchuk.RemoveAll();

	if(m_bInstallBeryukRebarAtBlockOut)	return;
	
	double dCTC   = m_dEndMainRebarCTC[0]/2;
	double dRange = pGlopt->GetSttExpansionJointWidth()+m_dLengthBeryukSttA;
	long   nRebar = long(dRange/dCTC);

	CDPoint	xyDim;
	xyDim.x = nRebar;
	xyDim.y = dCTC;

	m_arrCTCMainRebarUnderSinchuk.Add(xyDim);
}



void CADeckData::CalcCTCBogangRebarAtCrossBeam()
{
	m_arrCTCBogangRebarAtCrossBeam.RemoveAll();
	m_arrIndexBogangRebarAtCrossBeam.RemoveAll();

	if(!m_pDB->m_bApplyHunchAtCB)	return;

	CDPoint xyDim;
	CDPoint A[10];

	for(long nG=0; nG < m_pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pGir->IsCrossBeamBxHasHunch(pBx))
			{
				pGir->GetHunchXyDom(pBx, A, FALSE);

				xyDim.x		= long(~(A[6]-A[3]) / m_CTCBasicCrossBeam.y);
				xyDim.y		= m_CTCBasicCrossBeam.y;

				m_arrCTCBogangRebarAtCrossBeam.Add(xyDim);
				m_arrIndexBogangRebarAtCrossBeam.Add(CDPoint(nG, pBx->GetNumberCrossBeam()));
			}
			pBx = Finder.GetBxNext();
		}
	}	
}



void CADeckData::CalcCTCBogangRebarAtStringer()
{
/*	m_arrCTCBogangRebarAtStringer.RemoveAll();
	m_arrStringer.RemoveAll();
	
	CDPoint xyDim;

	for(long nG=0; nG < m_pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(m_pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CPlateCrossBeam *pC = pBx->GetCrossBeam();
			if(pC && pC->m_nQtyStringer_CR > 0 )
			{
				for(long n=0; n<pC->m_nQtyStringer_CR; n++)
				{
					CStringer *pStg = pC->GetBx()->GetStringer(n);
					if(pStg==NULL)	continue;
					
					double dL = GetStgLengthAtSlab(pStg);

					if(dL > 0)
					{
						xyDim.x	= long(dL / m_CTCBasicStringer.y);
						xyDim.y	= m_CTCBasicStringer.y;
						m_arrStringer.Add(CVector(nG, pC->GetSelfCount(), n));
						m_arrCTCBogangRebarAtStringer.Add(xyDim);
					}
				}
			}
			
			pBx = Finder.GetBxNext();
		}
	}	*/
}

void CADeckData::CalcCTCBogangRebarAtOutBrk()
{
	m_arrCTCBogangRebarAtOutBrk.RemoveAll();
/*	CDPoint xyDim;
	CDPoint A[10];

	// 좌측
	CPlateGirderApp *pGir = m_pDB->GetGirder(0);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_LEFT);
	while(pBx)
	{
		CBracket *pBrk = pBx->GetBracketLeft();
		if(pBrk)
		{
			pDB->GetXyFrontCantilever(pBx, A, TRUE);

			if(pGir->GetHeightTaper(pBx->GetStation()) > 0)
				xyDim = CDPoint(0,0);
			else
			{
				xyDim.x		= long(~(A[2]-A[5]) / m_CTCBasicOutBrk.y);
				xyDim.y		= m_CTCBasicOutBrk.y;
			}

			m_arrCTCBogangRebarAtOutBrk.Add(xyDim);
		}
		pBx = Finder.GetBxNext();
	}	

	// 우측
	pGir = m_pDB->GetGirder(m_pDB->GetGirdersu()-1);
	CPlateBasicIndex FinderR(pGir);
	pBx = FinderR.GetBxFirst(BX_OUTSIDE_CROSS_RIGHT);
	while(pBx)
	{
		CBracket *pBrk = pBx->GetBracketRigh();
		if(pBrk)
		{
			pDB->GetXyFrontCantilever(pBx, A, FALSE);

			if(pGir->GetHeightTaper(pBx->GetStation()) > 0)
				xyDim = CDPoint(0,0);
			else
			{
				xyDim.x		= long(~(A[2]-A[5]) / m_CTCBasicOutBrk.y);
				xyDim.y		= m_CTCBasicOutBrk.y;
			}

			m_arrCTCBogangRebarAtOutBrk.Add(xyDim);
		}
		pBx = FinderR.GetBxNext();
	}*/
}


void CADeckData::CalcCTCDanHunchRebar()
{
	// 기본값은 일반부의 CTC로 함
	m_arrCTCMainRebarDanHunch.RemoveAll();
	
	long nRebarSu = long(m_dLengthBeryukSttB/m_dGenMainRebarCTCLower[0]/2);
	if(nRebarSu>0)
		m_arrCTCMainRebarDanHunch.Add(CDPoint(nRebarSu, m_dGenMainRebarCTCLower[0]/2));
}



// 단부 헌치 정착장 3~4개 정도 들어가는 주철근 위치
CDPointArray CADeckData::GetXyArrRebarJongPosAtDanHunch(BOOL bSttSlab, long nJongBasePos)
{
	double dGirSttToHunchStt  = GetLengthSlabToTaper(TRUE, nJongBasePos);
	double dGirEndToHunchEnd  = GetLengthSlabToTaper(FALSE, nJongBasePos);
	double dThickSlab	      = m_pDB->m_dThickSlabBase;

	CDPoint vX(1,0), vY(0,1);
	CDPointArray xySlabJong = GetShapeSlabJongBasePos(nJongBasePos);
	CDPointArray xyArrMatch;
	if(m_bInstallDanbuTaperMainRebar==FALSE)
		return xyArrMatch;

	// 헌치 정착장부 시점
	CDPoint xyStt; 

	if (bSttSlab)
	{
		xyStt = vX*(dGirSttToHunchStt + m_pDB->m_dLengthTaperToTaperStt) - vY*(dThickSlab - m_dCoverDn);
		xyArrMatch.Add(xyStt);
		for (long n=0; n<m_arrCTCMainRebarDanHunch.GetSize(); n++)
		{
			CDPoint xyDim = m_arrCTCMainRebarDanHunch[n]; 
			for (long nn=0; nn<xyDim.x; nn++)
			{
				xyStt -= vX*xyDim.y;
				xyArrMatch.Add(xyStt);
			}
		}
	}
	else
	{
		xyStt = xySlabJong[2] - vX*(dGirEndToHunchEnd+m_pDB->m_dLengthTaperToTaperEnd) + vY*(m_pDB->m_dHeightTaperEnd+m_dCoverDn);
		xyArrMatch.Add(xyStt);
		for (long n=0; n<m_arrCTCMainRebarDanHunch.GetSize(); n++)
		{
			CDPoint xyDim = m_arrCTCMainRebarDanHunch[n]; 
			for (long nn=0; nn<xyDim.x; nn++)
			{
				xyStt += vX*xyDim.y;
				xyArrMatch.Add(xyStt);
			}
		}
	}

	return xyArrMatch;
}



CDPointArray CADeckData::GetXyArrSlabEnd(long nG)
{
	CPlateBridgeApp *pDB	 = m_pDB;
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	double dDist = (pGir->m_dGirderCenter+pGir->m_pRight->m_dGirderCenter)/2;

	double dLengthSlab = GetLengthSlabTotalByDist(dDist);
	double dSlabT	   = pDB->m_dThickSlabBase;

	CDPoint vX(1,0), vY(0,1);
	CDPointArray xyArrSlab;
	CDPoint xyPoint;
	
	xyPoint = CDPoint(0,0);
	xyArrSlab.Add(xyPoint);

	xyPoint += vX*dLengthSlab;
	xyArrSlab.Add(xyPoint);

	xyPoint += -vY*(dSlabT+pDB->m_dHeightTaperEnd);
	xyArrSlab.Add(xyPoint);

	xyPoint = CDPoint(0,0) - vY*(dSlabT+pDB->m_dHeightTaperStt);
	xyArrSlab.Add(xyPoint);

	return xyArrSlab;
}



CDPointArray CADeckData::GetXyArrSlabHunch(long nG, CPlateBasicIndex *pBxCB)
{
	CPlateBridgeApp *pDB	 = m_pDB;
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	
	pBxCB = pGir->GetBxMatchByCrossBeam(pBxCB, pGir);
	CPlateCrossBeam *pC = pBxCB->GetCrossBeam();
	long nCB = pC->GetSelfCount();
	long nQtyCB = pGir->GetQtyCrossSectionTotal();

	CDPoint vX(1,0), vY(0,1);

	double dSlabT	   = pDB->m_dThickSlabBase;
	double dHunchT     = m_pDB->m_dThickSlabHunch; 

	CDPoint xyPoint(0,0);
	CDPointArray xyArrSlab;

	if(nCB==1)
	{
		double dWidth = (pC->m_uM_CR+m_pDB->m_dWidthHunch)*2;
		double dDisToCB = GetLengthSlabEndToCBCen(nG, pBxCB);
		if(m_pDB->m_cSlabThickAddTypeStt==0)
		{
			xyPoint.x = dDisToCB + dWidth/2;
			xyPoint.y = -dSlabT;
			xyArrSlab.Add(xyPoint);

			xyPoint += vX*dHunchT*3 + vY*dHunchT;
			xyArrSlab.Add(xyPoint);
		}
		else
		{
			xyPoint.x = dDisToCB - dWidth - dHunchT*3;
			xyPoint.y = -dSlabT+dHunchT;
			xyArrSlab.Add(xyPoint);

			xyPoint.x = dDisToCB - dWidth;
			xyPoint.y = -dSlabT;
			xyArrSlab.Add(xyPoint);

			xyPoint.x = dDisToCB + dWidth;
			xyPoint.y = -dSlabT;
			xyArrSlab.Add(xyPoint);

			xyPoint.x = dDisToCB + dWidth + dHunchT*3;
			xyPoint.y = -dSlabT+dHunchT;
			xyArrSlab.Add(xyPoint);
		}
	}
	else if(nCB==nQtyCB-2)
	{
		double dWidth = (pC->m_uM_CR+m_pDB->m_dWidthHunch)*2;
		double dDisToCB = GetLengthSlabEndToCBCen(nG, pBxCB);
		if(m_pDB->m_cSlabThickAddTypeEnd==1)
		{
			xyPoint.x = dDisToCB - dWidth/2;
			xyPoint.y = -dSlabT;
			xyArrSlab.Add(xyPoint);

			xyPoint += -vX*dHunchT*3 + vY*dHunchT;
			xyArrSlab.Add(xyPoint);
		}
		else
		{
			xyPoint.x = dDisToCB - dWidth - dHunchT*3;
			xyPoint.y = -dSlabT+dHunchT;
			xyArrSlab.Add(xyPoint);

			xyPoint.x = dDisToCB - dWidth;
			xyPoint.y = -dSlabT;
			xyArrSlab.Add(xyPoint);

			xyPoint.x = dDisToCB + dWidth;
			xyPoint.y = -dSlabT;
			xyArrSlab.Add(xyPoint);

			xyPoint.x = dDisToCB + dWidth + dHunchT*3;
			xyPoint.y = -dSlabT+dHunchT;
			xyArrSlab.Add(xyPoint);
		}
	}
	else 
	{
		double dWidth = (pC->m_uM_CR+m_pDB->m_dWidthHunch)*2;
		double dDisToCB = GetLengthSlabEndToCBCen(nG, pBxCB);

		xyPoint.x = dDisToCB - dWidth - dHunchT*3;
		xyPoint.y = -dSlabT+dHunchT;
		xyArrSlab.Add(xyPoint);

		xyPoint.x = dDisToCB - dWidth;
		xyPoint.y = -dSlabT;
		xyArrSlab.Add(xyPoint);

		xyPoint.x = dDisToCB + dWidth;
		xyPoint.y = -dSlabT;
		xyArrSlab.Add(xyPoint);

		xyPoint.x = dDisToCB + dWidth + dHunchT*3;
		xyPoint.y = -dSlabT+dHunchT;
		xyArrSlab.Add(xyPoint);
	}

	return xyArrSlab;

}



double CADeckData::GetLengthSlabEndToCBCen(long nG, CPlateBasicIndex *pBxCB)
{
	CPlateBridgeApp *pDB	 = m_pDB;
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	double dDist = (pGir->m_dGirderCenter+pGir->m_pRight->m_dGirderCenter)/2;

	pBxCB = pGir->GetBxMatchByCrossBeam(pBxCB, pGir);

	CDPoint xyLeft = pGir->GetXyPlanSlabCrossbeamBase(pBxCB, TRUE);
	CDPoint xyRigh = pGir->GetXyPlanSlabCrossbeamBase(pBxCB, FALSE);
	CDPoint xyCen  = (xyLeft+xyRigh)/2;

	CLineInfoApp *pLineApp = (CLineInfoApp*)pDB->m_pLine;
	double dStaCBCen = 0;
	pLineApp->GetStationMatchVerticalByXy(xyCen, dStaCBCen);

	CDPoint xySlabStt;

	double dStaSlabSttCen1	= pGir->GetSSS(OFF_STT_SLAB);
	double dStaSlabSttCen2	= pGir->m_pRight->GetSSS(OFF_STT_SLAB);
	double dStaSlabStt      = (dStaSlabSttCen1+dStaSlabSttCen2) / 2;

	double dLenSlabEndToCBCen = pLineApp->GetLengthByDis(dStaSlabStt, dStaCBCen, dDist);

	return dLenSlabEndToCBCen;
}



CDPointArray CADeckData::GetShapeSlabOnCB(long nG)
{
	CDPoint vX(1,0), vY(0,1);
	CPlateGirderApp *pGir = m_pDB->GetGirder(nG);

	CDPointArray xyArr, xyArrSlabUp, xyArrSlabDn;
	CDPointArray xyArrEnd = GetXyArrSlabEnd(nG);

	//////////////////////////////////////////////////////////////////////////
	// 슬래브 끝단
	xyArrSlabUp.Add(xyArrEnd[0]);
	xyArrSlabUp.Add(xyArrEnd[1]);

	xyArrSlabDn.Add(xyArrEnd[2]);
	xyArrSlabDn.Add(xyArrEnd[3]);

	//////////////////////////////////////////////////////////////////////////
	// 테이퍼 시/종점부 
	double dDist = (pGir->m_dGirderCenter+pGir->m_pRight->m_dGirderCenter)/2;
	double dLengthSlab = GetLengthSlabTotalByDist(dDist);
	double dSlabT	   = m_pDB->m_dThickSlabBase;
	double dLSlabToTaperStt = GetLengthSlabToTaperByDist(TRUE, dDist);
	double dLSlabToTaperEnd = GetLengthSlabToTaperByDist(FALSE, dDist);
	double dHunchT     = m_pDB->m_dThickSlabHunch; 
	CDPoint xyPoint = vX*dLSlabToTaperStt - vY*(dSlabT+m_pDB->m_dHeightTaperStt);
	// 시점
	xyArrSlabDn.Add(xyPoint);
	if(m_pDB->m_cSlabThickAddTypeStt==1)	xyPoint += vX*m_pDB->m_dHeightTaperStt*5 + vY*m_pDB->m_dHeightTaperStt;
	else									xyPoint += vX*(m_pDB->m_dHeightTaperStt+dHunchT)*5 + vY*(m_pDB->m_dHeightTaperStt+dHunchT);
	xyArrSlabDn.Add(xyPoint);
	// 종점
	xyPoint = vX*(dLengthSlab-dLSlabToTaperEnd) - vY*(dSlabT+m_pDB->m_dHeightTaperEnd);
	xyArrSlabDn.Add(xyPoint);
	if(m_pDB->m_cSlabThickAddTypeEnd==1)	xyPoint += -vX*m_pDB->m_dHeightTaperEnd*5 + vY*m_pDB->m_dHeightTaperEnd;
	else									xyPoint += -vX*(m_pDB->m_dHeightTaperEnd+dHunchT)*5 + vY*(m_pDB->m_dHeightTaperEnd+dHunchT);
	xyArrSlabDn.Add(xyPoint);
	
	//////////////////////////////////////////////////////////////////////////
	// 가로보 헌치부
	vector <CDPointArray> vArrSlabHunch; 

	CPlateBxFinder Finder(m_pDB->GetGirder(nG));
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBx)
	{
		CPlateCrossBeam *pC = pBx->GetCrossBeam();
		if (!m_pDB->GetGirder(nG)->IsCrossBeamBxHasHunch(pBx))
		{
			pBx = Finder.GetBxNext();
			continue;
		}

		vArrSlabHunch.push_back(GetXyArrSlabHunch(nG, pBx));

		pBx = Finder.GetBxNext();
	}

	long n = 0;
	for (n=0; n<(long)vArrSlabHunch.size(); n++)
	{
		CDPointArray xyArr = vArrSlabHunch[n];
		for (long m=0; m<xyArr.GetSize(); m++)
			xyArrSlabDn.Add(xyArr[m]);
	}
	xyArrSlabDn.Sort(TRUE, FALSE);

	for (n=0; n<xyArrSlabUp.GetSize(); n++)
		xyArr.Add(xyArrSlabUp[n]);

	for (n=0; n<xyArrSlabDn.GetSize(); n++)
		xyArr.Add(xyArrSlabDn[n]);

	return xyArr;
}




CDPointArray CADeckData::GetShapeSlabJongBasePos(long nJongBasePos)
{
	CPlateBridgeApp      *pDB	 = m_pDB;
	CPlateGirderApp		*pGirCen = pDB->GetGirder(-1);

	if(nJongBasePos==JONGBASEPOS_ONCB)
		return GetShapeSlabOnCB(0);

	double dLengthSlab = GetLengthSlabTotal(nJongBasePos);
	double dSlabT	   = pDB->m_dThickSlabBase;
	double dLSlabToTaperStt = GetLengthSlabToTaper(TRUE, nJongBasePos);
	double dLSlabToTaperEnd = GetLengthSlabToTaper(FALSE, nJongBasePos);

	CDPoint vX(1,0), vY(0,1);
	CDPointArray xyArrSlab;
	CDPoint xyPoint;
	
	xyPoint = CDPoint(0,0);
	xyArrSlab.Add(xyPoint);

	xyPoint += vX*dLengthSlab;
	xyArrSlab.Add(xyPoint);

	xyPoint += -vY*(dSlabT+pDB->m_dHeightTaperEnd);
	xyArrSlab.Add(xyPoint);

	xyPoint += -vX*dLSlabToTaperEnd;
	xyArrSlab.Add(xyPoint);

	xyPoint += -vX*pDB->m_dLengthTaperToTaperEnd + vY*pDB->m_dHeightTaperEnd;
	xyArrSlab.Add(xyPoint);

	xyPoint += -vX*(dLengthSlab-dLSlabToTaperStt-dLSlabToTaperEnd-pDB->m_dLengthTaperToTaperStt-pDB->m_dLengthTaperToTaperEnd);
	xyArrSlab.Add(xyPoint);

	xyPoint += -vX*pDB->m_dLengthTaperToTaperStt - vY*pDB->m_dHeightTaperStt;
	xyArrSlab.Add(xyPoint);

	xyPoint += -vX*dLSlabToTaperStt;
	xyArrSlab.Add(xyPoint);

	return xyArrSlab;
}

CDPointArray *CADeckData::GetCTCBeryukRebar(long nSection, BOOL bUpper)
{
	if(nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END)
		return &m_arrCTCBeryukRebarDan[bUpper?0:1];
	else if(nSection==SECTION_CEN)
		return &m_arrCTCBeryukRebarCen[bUpper?0:1];
	else if(nSection==SECTION_PIER)	
		return &m_arrCTCBeryukRebarJijum[bUpper?0:1];

	return NULL;
}





//////////////////////////////////////////////////////////////////////////
// 이음길이 결정
double CADeckData::GetCTCBeryukForJoint(BOOL bUpper, long nJongPos, long nJongBasePos)
{
	double dCTCForJoint = 0;
	long nHDanPos = 0;
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
		nHDanPos = POS_SLAB_LEFT;
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
		nHDanPos = POS_SLAB_RIGH;
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
		nHDanPos = POS_SLAB_CENTER;

	if(nJongPos==JONGPOS_SLABSTTEND)
		dCTCForJoint = bUpper ? m_dEndSupportRebarCTC[nHDanPos] : m_dEndSupportRebarCTCLower[nHDanPos];
	else if(nJongPos==JONGPOS_SLABCENTER)
		dCTCForJoint = bUpper ? m_dGenSupportRebarCTC[nHDanPos] : m_dGenSupportRebarCTCLower[nHDanPos];
	else if(nJongPos==JONGPOS_SLABJIJUM)
		dCTCForJoint = bUpper ? m_dJijumRebarCTCUp[nHDanPos] : m_dJijumRebarCTCDn[nHDanPos];

	return dCTCForJoint;
}



//////////////////////////////////////////////////////////////////////////
// 이음길이 결정
double CADeckData::GetCTCMainForJoint(BOOL bUpper, long nHDanPos, long nSection)
{
	double dCTCForJoint = 0;
	if(nSection==SECTION_DAN)
		dCTCForJoint = bUpper ? m_dEndMainRebarCTC[nHDanPos]/2 : m_dEndMainRebarCTCLower[nHDanPos]/2;
	else if(nSection==SECTION_CEN || nSection==SECTION_PIER)
		dCTCForJoint = bUpper ? m_dGenMainRebarCTC[nHDanPos]/2 : m_dGenMainRebarCTCLower[nHDanPos]/2;
	else if(nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END)
		dCTCForJoint = bUpper ? m_dSabogangRebarCTCUp : m_dSabogangRebarCTCDn;

	return dCTCForJoint;
}



CPlateBasicIndex* CADeckData::GetBxBySection(long nSection, long nG)
{
	CPlateBasicIndex *pBx = NULL;

	CPlateGirderApp *pGir = m_pDB->GetGirder(nG);

	CPlateBxFinder Finder(pGir);
	if(nSection==SECTION_DAN || nSection==SECTION_SKEW_STT)		// 단부
		pBx = Finder.GetBxFirst(BX_STT_SLAB/*BX_CROSSBEAM_VBRACING*/);
	else if(nSection==SECTION_SKEW_END)		// 단부
		pBx = Finder.GetBxLast(BX_END_SLAB/*BX_CROSSBEAM_VBRACING*/);
	else if(nSection==SECTION_CEN)	// 일반부
	{
		pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
		if(m_pDB->m_dHeightTaperStt==0)
			return Finder.GetBxNext();
		else
		{
			while(pBx)
			{		
				CPlateBasicIndex *pBxL = pBx->GetBxMatchByCrossBeam(m_pDB->GetGirder(0));
				CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(m_pDB->GetGirder(m_pDB->GetGirdersu()-1));

				double dHTaperL = pBxL->GetGirder()->GetHeightTaper(pBxL->GetStation());
				double dHTaperR = pBxR->GetGirder()->GetHeightTaper(pBxR->GetStation());

				if(dHTaperL==0 && dHTaperR==0)
					return pBx;
				
				pBx = Finder.GetBxNext();
			}
		}
	}
	else if(nSection==SECTION_PIER && m_pDB->m_nQtyJigan>1)	// 지점부
		pBx =  pGir->GetBxOnJijum(1);

	return pBx;
}


// 슬래브의시작부 ~ 테이퍼 시작부
// 확폭일 경우는 고려 안됨.
double CADeckData::GetLengthSlabToTaper(BOOL bSttSlab, long nJongBasePos)
{
	double dDis = GetDistByJongBase(nJongBasePos);			// 선형거더 중심에서 이격거리
		
	return GetLengthSlabToTaperByDist(bSttSlab, dDis);
}


// 슬래브의시작부 ~ 테이퍼 시작부
// 확폭일 경우는 고려 안됨.
double CADeckData::GetLengthSlabToTaperByDist(BOOL bSttSlab, double dDist)
{
	CPlateBridgeApp      *pDB	 = m_pDB;
	CPlateGirderApp		*pGirCen = pDB->GetGirder(-1);
	
	double dStaTaperStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_HUNCHSTT), dDist, pGirCen->GetAAA(OFF_STT_HUNCHSTT));
	double dStaSlabStt  = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB), dDist, pGirCen->GetAAA(OFF_STT_SLAB));
	double dStaTaperEnd = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_HUNCHSTT), dDist, pGirCen->GetAAA(OFF_END_HUNCHSTT));
	double dStaSlabEnd  = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB), dDist, pGirCen->GetAAA(OFF_END_SLAB));
	double dLSlabToTaperStt = pGirCen->GetLengthDimType(dStaSlabStt, dStaTaperStt, dDist);
	double dLSlabToTaperEnd = pGirCen->GetLengthDimType(dStaSlabEnd, dStaTaperEnd, dDist);

	if(bSttSlab)
		return dLSlabToTaperStt;
	else 
		return dLSlabToTaperEnd;
}


// nJigan까지의 길이
double CADeckData::GetLengthSlabAccuml(long nJiganTo, long nJongBasePos, BOOL bConsiderCoverSd)
{
	double dSum = 0;
	for(long nJ=0; nJ<nJiganTo+1; nJ++)	
	{
		dSum += GetLengthSlab(nJ, nJongBasePos, bConsiderCoverSd);
	}
	return dSum;

}


// 기준위치에서 슬래브의 실제길이
// 주의 : 좌측슬래브와 우측슬래브 기준은 철근의 피복을 고려한 값.
double CADeckData::GetLengthSlabTotal(long nJongBasePos)
{
	return GetLengthSlabAccuml(m_pDB->m_nQtyJigan-1, nJongBasePos, TRUE);
}


// nJigan의 슬래브 길이
double CADeckData::GetLengthSlab(long nJigan, long nJongBasePos, BOOL bConsiderCoverSd)
{
	double dDis = GetDistByJongBase(nJongBasePos);
		
	return GetLengthSlabByDist(nJigan, dDis, bConsiderCoverSd);
}




// nJigan까지의 길이
double CADeckData::GetLengthSlabAccumlByDist(long nJiganTo, double dDist, BOOL bConsiderCoverSd)
{
	double dSum = 0;
	for(long nJ=0; nJ<nJiganTo+1; nJ++)	
	{
		dSum += GetLengthSlabByDist(nJ, dDist, bConsiderCoverSd);
	}
	return dSum;

}


// 기준위치에서 슬래브의 실제길이
// 주의 : 좌측슬래브와 우측슬래브 기준은 철근의 피복을 고려한 값.
double CADeckData::GetLengthSlabTotalByDist(double dDist)
{
	return GetLengthSlabAccumlByDist(m_pDB->m_nQtyJigan-1, dDist, TRUE);
}


// nJigan의 슬래브 길이
double CADeckData::GetLengthSlabByDist(long nJigan, double dDist, BOOL bConsiderCoverSd)
{
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);

	double dStaStt = 0;
	double dStaEnd = 0;
	double dL	   = 0;	

	if(m_pDB->m_nQtyJigan==1)	// 단경간 일 경우
	{
		dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB), dDist, pGirCen->GetAAA(OFF_STT_SLAB));
		dStaEnd = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB), dDist, pGirCen->GetAAA(OFF_END_SLAB));
		dL = pGirCen->GetLengthDimType(dStaStt, dStaEnd, dDist);
		
	}
	else
	{
		if(nJigan==0)			// 시작지간
		{
			dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB), dDist, pGirCen->GetAAA(OFF_STT_SLAB));
			dStaEnd = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(1)->GetStation(), dDist, pGirCen->GetBxOnJijum(1)->GetAngle());
			dL = pGirCen->GetLengthDimType(dStaStt, dStaEnd, dDist);
		}
		else if(nJigan==m_pDB->m_nQtyJigan-1)	// 마지막 지간
		{
			long nJijum = m_pDB->m_nQtyJigan-1;
			dStaStt = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(nJijum)->GetStation(), dDist, pGirCen->GetBxOnJijum(nJijum)->GetAngle());
			dStaEnd = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB), dDist, pGirCen->GetAAA(OFF_END_SLAB));
			dL = pGirCen->GetLengthDimType(dStaStt, dStaEnd, dDist);
		}
		else		// 중간지간
		{
			long nJijumStt = nJigan;
			long nJijumEnd = nJigan+1;
			dStaStt = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(nJijumStt)->GetStation(), dDist, pGirCen->GetBxOnJijum(nJijumStt)->GetAngle());
			dStaEnd = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(nJijumEnd)->GetStation(), dDist, pGirCen->GetBxOnJijum(nJijumEnd)->GetAngle());
			dL = pGirCen->GetLengthDimType(dStaStt, dStaEnd, dDist);
		}
	}
	
	return dL;
}



// BOOL bColumnMain : 기둥 축방향 주철근인지?
void CADeckData::SetLengthRebar(CRebar *pRB, long nJointType, double dCover, double dRebarSpace, long nBeta, long nIp)
{
	double dLength = pRB->GetLength(FALSE);
	if(dLength > m_dLengthRebarMax)
	{
		pRB->m_dJointLength = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), pRB->m_dDia, JOINT_TENS_NORMAL_B, toCm(dCover), toCm(dRebarSpace));
		
		double dInitLen		= m_dLengthRebarMax;
		double dMaxLength	= 0;
		long	nJointEa	= 0;
		for(nJointEa = 2; nJointEa<21; nJointEa++)		//설마 이음수가 20개를 넘진 않겠지..
		{
			dMaxLength = m_dLengthRebarMax*nJointEa - pRB->m_dJointLength*(nJointEa-1);
			
			if(dLength>dInitLen && dLength<dMaxLength)
				break;
			dInitLen = dMaxLength;
		}
		pRB->m_nJoint = nJointEa-1;
		// 이음 길이는 pRB->GetLegnth() 내부에서 더해줌
	}
	else
	{
		pRB->m_dJointLength = 0;
		pRB->m_nJoint	    = 0;
	}
	if(pRB->m_dLengthMax_A==pRB->m_dLengthMin_A) pRB->m_dLengthMin_A = 0;
	if(pRB->m_dLengthMax_B==pRB->m_dLengthMin_B) pRB->m_dLengthMin_B = 0;
	if(pRB->m_dLengthMax_C==pRB->m_dLengthMin_C) pRB->m_dLengthMin_C = 0;
	if(pRB->m_dLengthMax_D==pRB->m_dLengthMin_D) pRB->m_dLengthMin_D = 0;
}



long CADeckData::GetSuRebarArray(CDPointArray *xyArrRebar, BOOL bIncludeCover)
{
	long nEa	= 1;
	long nSize	= xyArrRebar->GetSize();

	for(long i=0; i<nSize; i++)
	{
		if(bIncludeCover==FALSE && (i==0 || i==nSize-1)) continue;
		CDPoint xyR = xyArrRebar->GetAt(i);
		nEa += (long)xyR.x;
	}
	
	return nEa;
}

void CADeckData::ShellSortRebarList(CTypedPtrArray <CObArray, CRebar*> *pArr, int (CRebar::*fcmp)(const CRebar*) const) const
{
	CPtrArray  Arr;

	long i, j, k, h;
	long nSize = pArr->GetSize();
	for(i=0; i<nSize; i++)
		Arr.Add(pArr->GetAt(i));

	CRebar pRB;
	for(h = 1; h < nSize; h = 3*h+1);

	for(h /= 3; h > 0; h /=3)
		for(i = 0; i < h; i++)
			for(j = i+h; j < nSize; j += h)
			{
				pRB = *(CRebar*)Arr[j];
				k = j;
				while(k > h - 1 && (((CRebar*)Arr[k-h])->*fcmp)(&pRB) > 0)
				{
					*(CRebar*)Arr[k] = *(CRebar*)Arr[k-h];
					k -= h;
				}
				*(CRebar*)Arr[k] = pRB;
			}

	Arr.RemoveAll();
}

double CADeckData::GetValueBasic(long nDia, long nType)
{
	if(nType<0 || nType>BASIC_GIRTH) return 0;
	double dValue = 0;

	if(nDia==10) dValue = m_RebarWeight[0][nType];
	if(nDia==13) dValue = m_RebarWeight[1][nType];
	if(nDia==16) dValue = m_RebarWeight[2][nType];
	if(nDia==19) dValue = m_RebarWeight[3][nType];
	if(nDia==22) dValue = m_RebarWeight[4][nType];
	if(nDia==25) dValue = m_RebarWeight[5][nType];
	if(nDia==29) dValue = m_RebarWeight[6][nType];
	if(nDia==32) dValue = m_RebarWeight[7][nType];
	if(nDia==35) dValue = m_RebarWeight[8][nType];
	if(nDia==38) dValue = m_RebarWeight[9][nType];
	if(nDia==41) dValue = m_RebarWeight[10][nType];
	if(nDia==51) dValue = m_RebarWeight[11][nType];

	return dValue;
}

CRebar* CADeckData::GetRebarByName(CString szName)
{
	for(long n=0; n<m_arrRebarJewon.GetSize(); n++)
	{
		CString sJewon =  m_arrRebarJewon[n]->m_sDescription;
		if(szName == m_arrRebarJewon[n]->m_sDescription)
			return m_arrRebarJewon[n];
	}
	return NULL;
}


// tvArrPlane의 시종점이 지간범위안에 있으면 OK(주철근 마크에서만 사용)
CDPointArray CADeckData::ModifyMarkPosArrByJigan(long nJigan, CDPointArray xyArrMark, CTwinVectorArray *tvArrRebar)
{
	if(xyArrMark.GetSize() < 1)		return xyArrMark;

	CPlateGirderApp	 *pGirCen = m_pDB->GetGirder(-1);
	double dStaStt		= GetStationMainRebarPos(TRUE);
	double dStaEnd		= GetStationMainRebarPos(FALSE);		
	double dStaResult	= 0;
	double dDistL		= pGirCen->GetLine()->GetDisMatchVerticalByXy(xyArrMark.GetAt(0));	// 철근의 이격거리
	double dStaJijum	= 0;
	CDPointArray xyArrReturn;
	
	if(nJigan==0)
	{
		dStaJijum = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(1)->GetStation(), dDistL, pGirCen->GetBxOnJijum(1)->GetAngle());
		for(long n=0; n<xyArrMark.GetSize(); n++)
		{
			CDPoint vRebar = (tvArrRebar->GetAt(n).m_v1-tvArrRebar->GetAt(n).m_v2).Unit();
			pGirCen->GetLine()->GetStationMatchByXyAndAngle(xyArrMark.GetAt(n), vRebar, dStaResult);
			if(dStaStt <= dStaResult && dStaResult <= dStaJijum)
				xyArrReturn.Add(xyArrMark.GetAt(n));
		}
	}
	else if(nJigan==m_pDB->m_nQtyJigan-1)
	{
		dStaJijum = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(m_pDB->m_nQtyJigan-1)->GetStation(), dDistL, pGirCen->GetBxOnJijum(m_pDB->m_nQtyJigan-1)->GetAngle());
		for(long n=0; n<xyArrMark.GetSize(); n++)
		{
			CDPoint vRebar = (tvArrRebar->GetAt(n).m_v1-tvArrRebar->GetAt(n).m_v2).Unit();
			pGirCen->GetLine()->GetStationMatchByXyAndAngle(xyArrMark.GetAt(n), vRebar, dStaResult);
			if(dStaJijum <=  dStaResult && dStaResult <= dStaEnd)
				xyArrReturn.Add(xyArrMark.GetAt(n));
		}
	}
	else
	{
		double	dStaJijumStt = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(nJigan)->GetStation(), dDistL, pGirCen->GetBxOnJijum(nJigan)->GetAngle());
		double	dStaJijumEnd = pGirCen->GetStationBySc(pGirCen->GetBxOnJijum(nJigan+1)->GetStation(), dDistL, pGirCen->GetBxOnJijum(nJigan+1)->GetAngle());
		for(long n=0; n<xyArrMark.GetSize(); n++)
		{
			CDPoint vRebar = (tvArrRebar->GetAt(n).m_v1-tvArrRebar->GetAt(n).m_v2).Unit();
			pGirCen->GetLine()->GetStationMatchByXyAndAngle(xyArrMark.GetAt(n), vRebar, dStaResult);
			if(dStaJijumStt <=  dStaResult && dStaResult <= dStaJijumEnd)
				xyArrReturn.Add(xyArrMark.GetAt(n));
		}
	}
	
	return xyArrReturn;
}


CDPointArray CADeckData::GetMarkPosArrByCycle(CDPointArray xyArrMark, long nCycle, long nCyclesu)
{
	CDPointArray xyArrReturn;

	for(long n=nCycle; n<xyArrMark.GetSize(); n+=nCyclesu)
	{
		xyArrReturn.Add(xyArrMark[n]);
	}

	return xyArrReturn;
}


double CADeckData::GetMaxCTC(CDPointArray &xyArrDim)
{
	double dMaxCTC = 0;
	for (long n=0; n<xyArrDim.GetSize(); n++) 
	{
		dMaxCTC = max(dMaxCTC, xyArrDim[n].y);
	}

	return dMaxCTC;
}

void CADeckData::MakeRebarMarkPosPlan(long nJigan, double dScale)
{
	m_arrMarkPosPlan[nJigan].clear();

	vector < pair<CString, CDPointArray > >  vMarkPos;

	MakeRebarMarkPosPlanMain(nJigan, TRUE, vMarkPos);	 
	MakeRebarMarkPosPlanMain(nJigan, FALSE, vMarkPos);	
	if(IsSkew())
		MakeRebarMarkPosPlanSkew(nJigan, vMarkPos);	

	if (m_pDB->m_nQtyJigan==1)
	{
		if (m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT].GetSize() > 0)// 슬래브 시점 상부		
			MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, TRUE, TRUE, dScale);	
		else
			MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, TRUE, TRUE, dScale);	
		
		if (m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT].GetSize() > 0)// 슬래브 시점 하부
			MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, TRUE, FALSE, dScale);	
		else
			MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, TRUE, FALSE, dScale);

		if (m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END].GetSize() > 0)// 슬래브 종점 상부
			MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, FALSE, TRUE, dScale);	
		else
			MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, FALSE, TRUE, dScale);

		if (m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END].GetSize() > 0)// 슬래브 종점 하부
			MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, FALSE, FALSE, dScale);	
		else
			MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, FALSE, FALSE, dScale);	
	}
	else
	{
		if (nJigan==0)
		{
			if (m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_STT].GetSize() > 0)// 슬래브 시점 상부		
				MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, TRUE, TRUE, dScale);	
			else
				MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, TRUE, TRUE, dScale);	
			
			if (m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_STT].GetSize() > 0)// 슬래브 시점 하부
				MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, TRUE, FALSE, dScale);	
			else
				MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, TRUE, FALSE, dScale);
		}
		else if (nJigan==m_pDB->m_nQtyJigan-1)
		{
			if (m_arrCTCSabogangRebar[CTC_SABOGANG_UPPER_END].GetSize() > 0)// 슬래브 종점 상부
				MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, FALSE, TRUE, dScale);	
			else
				MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, FALSE, TRUE, dScale);

			if (m_arrCTCSabogangRebar[CTC_SABOGANG_LOWER_END].GetSize() > 0)// 슬래브 종점 하부
				MakeRebarMarkPosPlanBeryukDanAtSkew(vMarkPos, FALSE, FALSE, dScale);	
			else
				MakeRebarMarkPosPlanBeryukDanAtNoSkew(vMarkPos, FALSE, FALSE, dScale);	
		}
	}

	MakeRebarMarkPosPlanBeryukGen(nJigan, vMarkPos, TRUE, dScale);	
	MakeRebarMarkPosPlanBeryukGen(nJigan, vMarkPos, FALSE, dScale);	
	MakeRebarMarkPosPlanBeryukJijum(nJigan, vMarkPos, TRUE, dScale);	
	MakeRebarMarkPosPlanBeryukJijum(nJigan, vMarkPos, FALSE, dScale);	
	if(IsBridgeHasCrossBeam() && IsBridgeHasSlabHunch() )
		MakeRebarMarkPosPlanCross(nJigan, vMarkPos, dScale);	
	if(IsBridgeHasStringer() && IsBridgeHasSlabHunch() )
		MakeRebarMarkPosPlanStringer(nJigan, vMarkPos, dScale);	
	if(IsBridgeHasOutBracket())
		MakeRebarMarkPosPlanOutBrk(nJigan, vMarkPos, dScale);	

	MakeRebarMarkPosPlanSangseCant(nJigan, vMarkPos);
	MakeRebarMarkPosPlanSangseDanbu(nJigan, vMarkPos);

	m_arrMarkPosPlan[nJigan] = vMarkPos;
}


// vMarkPos는 전체교량을 포함하는 것이 아니라 지간별로 생성됨
// m_arrMarkPosPlan[nJigan] = vMarkPos
void CADeckData::MakeRebarMarkPosPlanMain(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos)	
{
	if(bUpper)
	{
		if(IsSameMainRebarDanGen(TRUE))
			MakeRebarMarkPosPlanMainNoDiv(nJigan, bUpper, vMarkPos);
		else
			MakeRebarMarkPosPlanMainDiv(nJigan, bUpper, vMarkPos);
	}
	else
	{
		BOOL bNoBrk   = !IsBridgeHasOutBracket();
		BOOL bNoHunch = IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&m_pDB->GetGirdersu()==1) ? TRUE : FALSE;
		BOOL bTaperDiv= m_pDB->m_dHeightTaperStt>0 || (m_pDB->m_dHeightTaperStt==0&&!IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;
	
		if( bNoHunch && !bTaperDiv)	
			MakeRebarMarkPosPlanMainNoDiv(nJigan, bUpper, vMarkPos);
		else
			MakeRebarMarkPosPlanMainDiv(nJigan, bUpper, vMarkPos);

		MakeRebarMarkPosPlanMainLowerEtc(nJigan, vMarkPos);
	}

	
}

// 단부와 일반부의 철근이 구분되지 않는 경우 주철근의 마크를 생성함.
// 단면은 [단부단면-SECTION_DAN] 으로 통일함.
void CADeckData::MakeRebarMarkPosPlanMainNoDiv(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos)	
{
	CRebarPlacing RebarPlacing;

	CDPointArray xyArrMarkPosStt, xyArrMarkPosEnd;
	CTwinVectorArray tvArr;
	vector <REBAR_PLAN> *vPlan = GetMainRebarArr(bUpper, DRAW_SIDEALL);
	long nSize = vPlan->size();
	if(nSize==0)	return;

	long nSection = SECTION_DAN;	// 철근유무만 판단.
	long nCyclesu = GetRebarCycleSuBySection(nSection);
	double dRatio = 0.5;

	for (long nCycle=0; nCycle<nCyclesu; nCycle++)
	{
		REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, nCycle);
		long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

		for (long nShape=0; nShape<nShapesu; nShape++)
		{
			BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
			if(bValid==FALSE)
				continue;

			GetTwinVectorArrByCycleAndShape(vPlan, &tvArr, nCycle, nShape);
			if(tvArr.GetSize() > 0)
			{
				CDPoint xyMark = (tvArr[0].m_v1+tvArr[0].m_v2)/2;
				// tvArr의 중심점이 슬래브 중심기준으로 좌측/ 우측인지 판별하여 상부마킹을 경우는 좌측만, 하부마킹일 경우는 우측만 사용함.
				long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
				BOOL bExist = (bUpper&&nPos==-1 || !bUpper&&nPos==1 || nPos==0) ? TRUE : FALSE;
				if(bExist)
				{
					CDPointArray xyArrMark = GetMarkPosArr(&tvArr, GetCTCBeryukRebar(SECTION_DAN, bUpper), nPos==0 ? dRatio/2 : dRatio, bUpper);
					xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArr);
					DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);
					
					pair<CString, CDPointArray> vPair;
					CString sText;

					if(bUpper)		sText.Format("전체상면STT%d-%dCycle", nShape+1, nCycle+1);
					else			sText.Format("전체하면STT%d-%dCycle", nShape+1, nCycle+1);
					vPair.first		= sText;
					vPair.second	= xyArrMarkPosStt;
					vMarkPos.push_back(vPair);		

					if(bUpper)		sText.Format("전체상면END%d-%dCycle", nShape+1, nCycle+1);
					else			sText.Format("전체하면END%d-%dCycle", nShape+1, nCycle+1);
					vPair.first		= sText;
					vPair.second	= xyArrMarkPosEnd;
					vMarkPos.push_back(vPair);		
				}
			}
		}
	}
}

// -1 : 슬래브 좌측
// 0 : 슬래브 중심
// 1 : 슬래브 우측
long CADeckData::GetXyPosIDWhereInSlabHDan(CDPoint xyPos)
{
	double dStaCen = 0;
	double dDistSlabCen = GetDistByJongBase(JONGBASEPOS_SLABCENTER);
	m_pDB->m_pLine->GetStationMatchVerticalByXy(xyPos, dStaCen);
	CDPoint xyCen = m_pDB->m_pLine->GetXyLineDis(dStaCen, dDistSlabCen);
	CDPoint vGir = m_pDB->m_pLine->GetAngleAzimuth(dStaCen);
	BOOL bRotateRight = IsDirectionRotateRight(xyCen, vGir, xyPos);

	if(~(xyCen-xyPos)<1)
		return 0;
	else if(bRotateRight)
		return 1;
	else if(!bRotateRight)
		return -1;

	return 0;
}



void CADeckData::MakeRebarMarkPosPlanMainDiv(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos)	
{
	CRebarPlacing RebarPlacing;
	pair<CString, CDPointArray> vPair;
	CString sText;
	double dRatio = 0.5;

	CDPointArray xyArrMarkPosStt, xyArrMarkPosEnd;
	CTwinVectorArray tvArr;
	vector <REBAR_PLAN>* vPlan = GetMainRebarArr(bUpper, DRAW_SIDEALL);		
	long nShapesu= vPlan->size();
	if(nShapesu == 0)		return;

	BOOL bOneJigan = m_pDB->m_nQtyJigan > 1 ? FALSE : TRUE;

	CDPointArray xyArrDimDan = GetCTCBeryukRebar(SECTION_DAN, bUpper);
	CDPointArray xyArrDimCen = GetCTCBeryukRebar(SECTION_CEN, bUpper);

	vector <REBAR_PLAN> vPlanStt, vPlanEnd, vPlanGen;
	for (long nShape=0; nShape<nShapesu; nShape++)
	{
		if(vPlan->at(nShape).m_nPos==-1)
			vPlanStt.push_back(vPlan->at(nShape));		// 단부 시점
		else if(vPlan->at(nShape).m_nPos==0)
			vPlanGen.push_back(vPlan->at(nShape));		// 일반부
		else if(vPlan->at(nShape).m_nPos==1)
			vPlanEnd.push_back(vPlan->at(nShape));		// 단부 종점
	}

	long nCyclesu = 0;
	//////////////////////////////////////////////////////////////////////////
	//  단부 주철근
	if(bOneJigan)	
	{
		nCyclesu = GetRebarCycleSuBySection(SECTION_DAN);
		for (long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(SECTION_DAN, nCycle);
			long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

			for (long nShape=0; nShape<nShapesu; nShape++)
			{
				BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
				if(bValid==FALSE)
					continue;

				for (long nSttEnd=0; nSttEnd<2; nSttEnd++)
				{
					BOOL bStt = nSttEnd==0 ? TRUE : FALSE;
					GetTwinVectorArrByCycleAndShape(bStt?&vPlanStt:&vPlanEnd, &tvArr, nCycle, nShape);
					if(tvArr.GetSize() > 0)
					{
						CDPoint xyMark = (tvArr[0].m_v1+tvArr[0].m_v2)/2;
						long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
						BOOL bExist = (bUpper&&nPos==-1 || !bUpper&&nPos==1 || nPos==0) ? TRUE : FALSE;
						if(bExist)
						{
							CDPointArray xyArrMark = GetMarkPosArr(&tvArr, xyArrDimDan, nPos==0 ? dRatio/2 : dRatio, bUpper);
							xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArr);

							pair<CString, CDPointArray> vPair;
							CString sText;

							if(bStt)
							{
								if(bUpper)		sText.Format("단부상면STT%d-%dCycle", nShape+1, nCycle+1);
								else			sText.Format("단부하면STT%d-%dCycle", nShape+1, nCycle+1);
							}
							else
							{
								if(bUpper)		sText.Format("단부상면END%d-%dCycle", nShape+1, nCycle+1);
								else			sText.Format("단부하면END%d-%dCycle", nShape+1, nCycle+1);
							}
							vPair.first		= sText;
							vPair.second	= xyArrMark;
							vMarkPos.push_back(vPair);
						}
					}
				}
			}
		}
	}
	else
	{
		if(nJigan==0 || nJigan==m_pDB->m_nQtyJigan-1)	// 시작, 끝지간에서만...
		{
			BOOL bStt = nJigan==0 ? TRUE : FALSE;
			nCyclesu = GetRebarCycleSuBySection(SECTION_DAN);
			for(long nCycle=0; nCycle<nCyclesu; nCycle++)
			{
				REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(SECTION_DAN, nCycle);
				long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

				for (long nShape=0; nShape<nShapesu; nShape++)
				{
					BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
					if(bValid==FALSE)
						continue;

					GetTwinVectorArrByCycleAndShape(bStt ?&vPlanStt:&vPlanEnd, &tvArr, nCycle, nShape);
					if(tvArr.GetSize() > 0)
					{
						CDPoint xyMark = (tvArr[0].m_v1+tvArr[0].m_v2)/2;
						long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
						BOOL bExist = (bUpper&&nPos==-1 || !bUpper&&nPos==1 || nPos==0) ? TRUE : FALSE;
						if(bExist)
						{
							CDPointArray xyArrMark = GetMarkPosArr(&tvArr, xyArrDimDan, nPos==0 ? dRatio/2 : dRatio, bUpper);
							xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArr);

							pair<CString, CDPointArray> vPair;
							CString sText;

							if(bUpper)			sText.Format("단부상면%d-%dCycle", nShape+1, nCycle+1);
							else				sText.Format("단부하면%d-%dCycle", nShape+1, nCycle+1);	
							vPair.first		= sText;
							vPair.second	= xyArrMark;
							vMarkPos.push_back(vPair);
						}
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//  일반부 주철근
	BOOL bNoBrk   = !IsBridgeHasOutBracket();
	BOOL bNoHunch = IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&m_pDB->GetGirdersu()==1) ? TRUE : FALSE;
	BOOL bTaperDiv= m_pDB->m_dHeightTaperStt>0 || (m_pDB->m_dHeightTaperStt==0&&!IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;
	nCyclesu = GetRebarCycleSuBySection(SECTION_CEN);
	
	if(bUpper || !bUpper&&(bNoHunch && bTaperDiv))
	{
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(SECTION_CEN, nCycle);
			long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

			for (long nShape=0; nShape<nShapesu; nShape++)
			{
				BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
				if(bValid==FALSE)
					continue;

				GetTwinVectorArrByCycleAndShape(&vPlanGen, &tvArr, nCycle, nShape);
				if(tvArr.GetSize() > 0)
				{
					CDPoint xyMark = (tvArr[0].m_v1+tvArr[0].m_v2)/2;
					long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
					BOOL bExist = (bUpper&&nPos==-1 || !bUpper&&nPos==1 || nPos==0) ? TRUE : FALSE;
					if(bExist)
					{
						CDPointArray xyArrMark = GetMarkPosArr(&tvArr, xyArrDimCen, nPos==0 ? dRatio/2 : dRatio, bUpper);
						xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArr);
						CDPointArray xyArrMarkPosStt, xyArrMarkPosEnd;
						DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);

						pair<CString, CDPointArray> vPair;
						CString sText;

						if(bUpper)			sText.Format("일반부상면STT%d-%dCycle", nShape+1, nCycle+1);
						else				sText.Format("일반부하면STT%d-%dCycle", nShape+1, nCycle+1);	
						vPair.first	= sText;
						vPair.second	= xyArrMarkPosStt;
						vMarkPos.push_back(vPair);

						if(bUpper)			sText.Format("일반부상면END%d-%dCycle", nShape+1, nCycle+1);
						else				sText.Format("일반부하면END%d-%dCycle", nShape+1, nCycle+1);	
						vPair.first	= sText;
						vPair.second	= xyArrMarkPosEnd;
						vMarkPos.push_back(vPair);
					}
				}
			}
		}
	}
	else
	{
		GetTwinVectorArrByMultiCycleAndShape(&vPlanGen, &tvArr, 0, nCyclesu-1, m_pDB->GetGirdersu()==1?"좌측거더상면":"우측거더상면");

		if(tvArr.GetSize() > 0)
		{
			CDPoint xyMark = (tvArr[0].m_v1+tvArr[0].m_v2)/2;
			long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
			BOOL bExist = (bUpper&&nPos==-1 || !bUpper&&nPos==1 || nPos==0) ? TRUE : FALSE;
			if(bExist)
			{
				CDPointArray xyArrMark = GetMarkPosArr(&tvArr, xyArrDimCen, nPos==0 ? dRatio/2 : dRatio, bUpper);
				xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArr);
				DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);

				sText.Format("일반부외측거더하면STT");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosStt;
				vMarkPos.push_back(vPair);

				sText.Format("일반부외측거더하면END");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosEnd;
				vMarkPos.push_back(vPair);
			}
		}
	}
}



void CADeckData::MakeRebarMarkPosPlanMainLowerEtc(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos)	
{
	CRebarPlacing RebarPlacing;
	CString sText;
	double dRatio  = 0.5;

	CDPointArray xyArrMarkPosStt, xyArrMarkPosEnd;
	CTwinVectorArray tvArrGen;
	vector <REBAR_PLAN>* pvPlan = GetMainRebarArr(FALSE, DRAW_SIDEALL);	// 하면은 시작점이 슬래브우측

	long nShapesu = pvPlan->size();
	if(nShapesu == 0)		return;

	CDPointArray xyArrDimRev = GetCTCBeryukRebar(SECTION_CEN, FALSE);
	RebarPlacing.ReverseRebar(xyArrDimRev);

	vector <REBAR_PLAN> vPlanGen;
	for (long nShape=0; nShape<nShapesu; nShape++)
	{
		if(pvPlan->at(nShape).m_nPos==0)
			vPlanGen.push_back(pvPlan->at(nShape));
	}

	//////////////////////////////////////////////////////////////////////////
	// 일반부중앙거더하면
	//////////////////////////////////////////////////////////////////////////
	long nCyclesu = GetRebarCycleSuBySection(SECTION_CEN);
	pair<CString, CDPointArray> vPair;

	if(m_pDB->GetGirdersu() > 2)
	{
		GetTwinVectorArrByMultiCycleAndShape(&vPlanGen, &tvArrGen, 0, nCyclesu-1, "중앙부거더상면");

		if(tvArrGen.GetSize() > 0)
		{
			CDPoint xyMark = (tvArrGen[0].m_v1+tvArrGen[0].m_v2)/2;
			long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
			BOOL bExist = (nPos==1 || nPos==0) ? TRUE : FALSE;
			if(bExist)
			{
				CDPointArray xyArrMark = GetMarkPosArr(&tvArrGen, xyArrDimRev, nPos==0 ? dRatio/2 : dRatio, FALSE);
				xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArrGen);
				DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);
			
				sText.Format("일반부중앙거더하면STT");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosStt;
				vMarkPos.push_back(vPair);

				sText.Format("일반부중앙거더하면END");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosEnd;
				vMarkPos.push_back(vPair);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 일반부중앙가로보부하면
	//////////////////////////////////////////////////////////////////////////
	if(m_pDB->GetGirdersu() > 1 && m_pDB->m_dThickSlabHunch > 0)
	{
		GetTwinVectorArrByMultiCycleAndShape(&vPlanGen, &tvArrGen, 0, nCyclesu-1, "가로보상부전체");

		if(tvArrGen.GetSize() > 0)
		{
			CDPoint xyMark = (tvArrGen[0].m_v1+tvArrGen[0].m_v2)/2;
			long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
			BOOL bExist = (nPos==1 || nPos==0) ? TRUE : FALSE;
			if(bExist)
			{
				CDPointArray xyArrMark = GetMarkPosArr(&tvArrGen, xyArrDimRev, nPos==0 ? dRatio/2 : dRatio, FALSE);
				xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArrGen);
				DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);
			
				sText.Format("일반부중앙가로보부하면STT");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosStt;
				vMarkPos.push_back(vPair);

				sText.Format("일반부중앙가로보부하면END");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosEnd;
				vMarkPos.push_back(vPair);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 우측거더 상면
	if(m_pDB->GetGirdersu() > 1 && m_pDB->m_dThickSlabHunch > 0)
	{
		GetTwinVectorArrByMultiCycleAndShape(&vPlanGen, &tvArrGen, 0, nCyclesu-1, "우측거더상면");

		if(tvArrGen.GetSize() > 0)
		{
			CDPoint xyMark = (tvArrGen[0].m_v1+tvArrGen[0].m_v2)/2;
			long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
			BOOL bExist = (nPos==1 || nPos==0) ? TRUE : FALSE;
			if(bExist)
			{
				CDPointArray xyArrMark = GetMarkPosArr(&tvArrGen, xyArrDimRev, nPos==0 ? dRatio/2 : dRatio, FALSE);
				xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArrGen);
				DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);
			
				sText.Format("일반부우측거더상면STT");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosStt;
				vMarkPos.push_back(vPair);

				sText.Format("일반부우측거더상면END");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosEnd;
				vMarkPos.push_back(vPair);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 외측가로보 상부, 외측빔 헌치보강
	BOOL bLeftBrk = IsBridgeHasOutBracket(SECTION_CEN, TRUE);
	BOOL bRighBrk = IsBridgeHasOutBracket(SECTION_CEN, FALSE);
	if ((bLeftBrk && bRighBrk) || (!bLeftBrk && bRighBrk))
	{
		GetTwinVectorArrByMultiCycleAndShape(&vPlanGen, &tvArrGen, 0, nCyclesu-1, "외측가로보상부(우)");

		if(tvArrGen.GetSize() > 0)
		{
			CDPoint xyMark = (tvArrGen[0].m_v1+tvArrGen[0].m_v2)/2;
			long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
			BOOL bExist = (nPos==1 || nPos==0) ? TRUE : FALSE;
			if(bExist)
			{
				CDPointArray xyArrMark = GetMarkPosArr(&tvArrGen, xyArrDimRev, nPos==0 ? dRatio/2 : dRatio, FALSE);
				xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArrGen);
				DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);

				if(bLeftBrk && bRighBrk)		sText.Format("외측가로보상부STT");
				else							sText.Format("외측가로보상부(우)STT");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosStt;
				vMarkPos.push_back(vPair);

				if(bLeftBrk && bRighBrk)		sText.Format("외측가로보상부END");
				else							sText.Format("외측가로보상부(우)END");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosEnd;
				vMarkPos.push_back(vPair);
			}
		}

		GetTwinVectorArrByMultiCycleAndShape(&vPlanGen, &tvArrGen, 0, nCyclesu-1, "외측빔헌치보강(우)");

		if(tvArrGen.GetSize() > 0)
		{
			CDPoint xyMark = (tvArrGen[0].m_v1+tvArrGen[0].m_v2)/2;
			long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
			BOOL bExist = (nPos==1 || nPos==0) ? TRUE : FALSE;
			if(bExist)
			{
				CDPointArray xyArrMark = GetMarkPosArr(&tvArrGen, xyArrDimRev, nPos==0 ? dRatio/2 : dRatio, FALSE);
				xyArrMark = ModifyMarkPosArrByJigan(nJigan, xyArrMark, &tvArrGen);
				DivideMarkPosAtJigan(&xyArrMark, &xyArrMarkPosStt, &xyArrMarkPosEnd, nJigan);

				if(bLeftBrk && bRighBrk)		sText.Format("외측빔헌치보강STT");
				else							sText.Format("외측빔헌치보강(우)STT");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosStt;
				vMarkPos.push_back(vPair);

				if(bLeftBrk && bRighBrk)		sText.Format("외측빔헌치보강END");
				else							sText.Format("외측빔헌치보강(우)END");
				vPair.first		= sText;
				vPair.second	= xyArrMarkPosEnd;
				vMarkPos.push_back(vPair);
			}
		}
	}
}


void CADeckData::MakeRebarMarkPosPlanSkew(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos)
{
	if(nJigan!=0 && nJigan!=m_pDB->m_nQtyJigan-1)		return;

	CRebarPlacing RebarPlacing;
	pair<CString, CDPointArray> vPair;
	CString sText;
	double dRatio = 0.5;

	long nSection = 0;
	if(nJigan==0)							nSection=SECTION_SKEW_STT;
	else if(nJigan==m_pDB->m_nQtyJigan-1)	nSection=SECTION_SKEW_END;

	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);
	CDPoint		vAngStt = pGirCen->GetAngleStt();
	CDPoint		vAngEnd = pGirCen->GetAngleEnd();
	BOOL		bOneCycle = m_stMainCycleSabogangStt.size()==1 ? TRUE : FALSE;
	CDPointArray xyArrMark; 
	CTwinVectorArray tvArr;

	//////////////////////////////////////////////////////////////////////////
	// 시점 사보강철근
	for (long nUpDn=0; nUpDn<2; nUpDn++)
	{
		BOOL bUpper = nUpDn==0 ? TRUE : FALSE;
		CDPointArray xyArrDim = GetCTCBeryukRebar(SECTION_DAN, bUpper);
		if(nJigan==0)
		{
			vector <REBAR_PLAN> *vPlan = GetSabogangArr(TRUE, bUpper, DRAW_SIDEALL);
			long nSize = vPlan->size();
			if(nSize==0)	return;

			long nCyclesu = GetRebarCycleSuBySection(nSection);
			for (long nCycle=0; nCycle<nCyclesu; nCycle++)
			{
				REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, nCycle);
				long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

				for (long nShape=0; nShape<nShapesu; nShape++)
				{
					BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
					if(bValid==FALSE)
						continue;

					GetTwinVectorArrByCycleAndShape(vPlan, &tvArr, nCycle, nShape);

					if(tvArr.GetSize() > 0)
					{
						CDPoint xyMark = (tvArr[0].m_v1+tvArr[0].m_v2)/2;
						long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
						BOOL bExist = (bUpper&&nPos==-1 || !bUpper&&nPos==1 || nPos==0) ? TRUE : FALSE;
						if(bExist)
						{ 
							CDPointArray xyArrMark = GetMarkPosArr(&tvArr, xyArrDim, nPos==0 ? dRatio/2 : dRatio, bUpper);
							
							pair<CString, CDPointArray> vPair;
							CString sText;

							if(bOneCycle)	sText.Format(bUpper ? "시점사보강상면%d" : "시점사보강하면%d", nShape+1);
							else			sText.Format(bUpper ? "시점사보강상면%d-%dCycle" : "시점사보강하면%d-%dCycle", nShape+1, nCycle+1);
							vPair.first		= sText;
							vPair.second	= xyArrMark;
							vMarkPos.push_back(vPair);		
						}
					}
				}				
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 종점 사보강철근
		if(m_pDB->m_nQtyJigan==1 || nJigan == m_pDB->m_nQtyJigan-1)
		{
			vector <REBAR_PLAN> *vPlan = GetSabogangArr(FALSE, bUpper, DRAW_SIDEALL);
			long nSize = vPlan->size();
			if(nSize==0)	return;

			long nCyclesu = GetRebarCycleSuBySection(nSection);
			for (long nCycle=0; nCycle<nCyclesu; nCycle++)
			{
				REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, nCycle);
				long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();

				for (long nShape=0; nShape<nShapesu; nShape++)
				{
					BOOL bValid = bUpper ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
					if(bValid==FALSE)
						continue;

					GetTwinVectorArrByCycleAndShape(vPlan, &tvArr, nCycle, nShape);
					if(tvArr.GetSize() > 0)
					{
						CDPoint xyMark = (tvArr[0].m_v1+tvArr[0].m_v2)/2;
						long nPos = GetXyPosIDWhereInSlabHDan(xyMark);
						BOOL bExist = (bUpper&&nPos==-1 || !bUpper&&nPos==1 || nPos==0) ? TRUE : FALSE;
						if(bExist)
						{
							CDPointArray xyArrMark = GetMarkPosArr(&tvArr, xyArrDim, nPos==0 ? dRatio/2 : dRatio, bUpper);
							
							pair<CString, CDPointArray> vPair;
							CString sText;

							if(bOneCycle)	sText.Format(bUpper ? "종점사보강상면%d" : "종점사보강하면%d", nShape+1);
							else			sText.Format(bUpper ? "종점사보강상면%d-%dCycle" : "종점사보강하면%d-%dCycle", nShape+1, nCycle+1);
							vPair.first		= sText;
							vPair.second	= xyArrMark;
							vMarkPos.push_back(vPair);	
						}
					}
				}				
			}

		}	
	}
		
	
	//////////////////////////////////////////////////////////////////////////
	// 사각부 주철근
	//////////////////////////////////////////////////////////////////////////
	for (long nUpDn=0; nUpDn<2; nUpDn++)
	{
		BOOL bUpper = nUpDn==0 ? TRUE : FALSE;

		for(long nStt =0; nStt <= iEND; nStt++)
		{
			BOOL bSttSlab = nStt == iSTT? TRUE : FALSE;

			vector <REBAR_PLAN> *vPlanM = GetMainRebarArrAtSkew(bSttSlab, bUpper, bUpper?DRAW_LEFTSIDE:DRAW_RIGHTSIDE);		
			if(vPlanM->size() == 0) continue;
			if(!IsSkewStt() && bSttSlab) continue;
			if(!IsSkewEnd() && !bSttSlab) continue;

			if(bSttSlab && nJigan != 0) continue;
			if(!bSttSlab && nJigan != m_pDB->m_nQtyJigan-1) continue;

			CTwinVectorArray tvArrRebar;
			long nCntCycle = GetRebarCycleSuBySection(SECTION_DAN);
			for (long nCycle=0; nCycle<nCntCycle; nCycle++)
			{
				// 단부시점
				REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(SECTION_DAN, nCycle);
				long nShapesu = bUpper ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();
				long nShape = 0;
				for (nShape=0; nShape<nShapesu; nShape++)
				{
					BOOL bValid = (BOOL)(bUpper ? pRebarCycle->m_dArrValidUp[nShape] : pRebarCycle->m_dArrValidDn[nShape]);
					if(bValid==FALSE)	continue;
					GetTwinVectorArrByCycleAndShape(vPlanM, &tvArrRebar, nCycle, nShape);
					if(tvArrRebar.GetSize() > 0)
					{
						dRatio = 0.5 + nCycle*0.04;

						CDPointArray xyArrDimDan, xyArrMarkTmp;
						xyArrMark.RemoveAll();
						xyArrDimDan.RemoveAll();
						xyArrMarkTmp.RemoveAll();

						xyArrDimDan = GetCTCBeryukRebar(SECTION_DAN, bUpper);
						xyArrMarkTmp = GetMarkPosArrSkewMain(&tvArrRebar, xyArrDimDan,  bUpper, dRatio);

						//가끔 짧은 것이면 마킹을 할수가 없음 제외 한다.
						if(xyArrMarkTmp.GetSize() > 1)
						{
							CTwinVectorArray tvArrMarkTmp;
							CTwinVector tvLine;

							for (long nIdx =1; nIdx < xyArrMarkTmp.GetSize(); nIdx++)
							{
								CDPoint xyPrev = xyArrMarkTmp.GetAt(nIdx-1);
								CDPoint xyCurr = xyArrMarkTmp.GetAt(nIdx);

								tvLine.SetFromPoint(xyPrev, xyCurr);
								tvArrMarkTmp.Add(tvLine);
							}							

							RebarPlacing.GetXyMatchTvArrAndTvArr(tvArrRebar, tvArrMarkTmp, xyArrMark, FALSE);
						}
						else
						{
							xyArrMark = xyArrMarkTmp;
						}						

						if(xyArrMark.GetSize() > 0)
						{
							if(bUpper)
								bSttSlab? sText.Format("시점사각주철근상면%d-%dCycle", nShape+1, nCycle+1) : sText.Format("종점사각주철근상면%d-%dCycle", nShape+1, nCycle+1);
							else
								bSttSlab? sText.Format("시점사각주철근하면%d-%dCycle", nShape+1, nCycle+1) : sText.Format("종점사각주철근하면%d-%dCycle", nShape+1, nCycle+1);
							vPair.first		= sText;
							vPair.second	= xyArrMark;
							vMarkPos.push_back(vPair);
						}						
					}
				}
			}
		}
	}
}



// 단부 배력철근 : 사보강 철근(offset방식) 존재시... 
// 배력단부상면STT마크는 슬래브시점각과 동일한게 찍음
void CADeckData::MakeRebarMarkPosPlanBeryukDanAtSkew(vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bSttSlab, BOOL bUpper, double dScale)
{
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();

	CRebarPlacing RebarPlacing;
	CDPointArray xyArrDim;	// 단부상면철근 마크의 종단위치 결정을 위한 CTC배열

	if(bUpper)	// 상부는 사보강철근 사용
	{
		if(bSttSlab)
		{
			if(m_arrCTCSabogangRebar[bUpper?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_LOWER_STT].GetSize() > 0)	
				xyArrDim = m_arrCTCSabogangRebar[bUpper?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_LOWER_STT];
			else
				return;
		}
		else
		{
			if(m_arrCTCSabogangRebar[bUpper?CTC_SABOGANG_UPPER_END:CTC_SABOGANG_LOWER_END].GetSize() > 0)	
				xyArrDim = m_arrCTCSabogangRebar[bUpper?CTC_SABOGANG_UPPER_END:CTC_SABOGANG_LOWER_END];
			else
				return;
		}
	}
	else		// 하부는 신축이음하면주철근 사용
	{
		xyArrDim = m_arrCTCMainRebar[CTC_MAIN_LOWER_RIGHT];
	}

	double dSttBlockOutWidth = pGlopt->GetSttExpansionJointWidth();
	double dEndBlockOutWidth = pGlopt->GetEndExpansionJointWidth();
	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth = 0;
		dEndBlockOutWidth = 0;
	}

	CPlateGirderApp *pGirCen		= m_pDB->GetGirder(-1);
	
	double dStaSlabSttCen	= pGirCen->GetSSS(OFF_STT_SLAB);
	double dStaSlabEndCen	= pGirCen->GetSSS(OFF_END_SLAB);
	double dDisLStt			= GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDisRStt			= GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDisLEnd			= GetSideBarDistFromCenterGir(FALSE, TRUE);
	double dDisREnd			= GetSideBarDistFromCenterGir(FALSE, FALSE);
	double dDisL			= bSttSlab ? dDisLStt : dDisLEnd;
	double dDisR			= bSttSlab ? dDisRStt : dDisREnd;
	CDPoint	vAngStt			= pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd			= pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft	= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh	= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndLeft	= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh	= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dWidthHalfSlab	= m_pDB->GetWidthAct(bUpper ? dStaSlabSttCen : dStaSlabEndCen)/2;
	double dCoverSd         = GetLengthCoverSd(bSttSlab); 

	CDPoint A[4];
	CDPoint vAngSlab;
	if(bSttSlab)
	{
		m_pDB->GetXySss(OFF_STT_SLAB, A);
		vAngSlab = (A[0]-A[3]).Unit();
	}
	else
	{
		m_pDB->GetXySss(OFF_END_SLAB, A);
		vAngSlab = (A[0]-A[3]).Unit();
	}

	CDoubleArray dArrDim;
	CDPoint		 xyMark;
	CDPointArray xyArrMark; 
	CString		 sText;

	if(bUpper)
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCBeryukRebarDan[CTC_BERYUK_UPPER], dArrDim, TRUE);	
	else
	{
		CDPointArray xyArrTemp = m_arrCTCBeryukRebarDan[CTC_BERYUK_LOWER];
		RebarPlacing.ReverseRebar(xyArrTemp);
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrTemp, dArrDim, TRUE);	
	}
	dArrDim.InsertAt(0, 0);

	long	nCyclesu	=  m_stBeryukCycleLeft.size();
	BOOL	bOneCycle	= nCyclesu==1 ? TRUE : FALSE;
	double	dStaRebarStt= 0;

	if(bUpper)
	{
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			long   nRebarStt        = 0;						// 좌,우,중앙부구역의 시작마크 인덱스
			double dOffset          = 300 + nCycle*(dScale*25);	// 배력철근의 사이클간격
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	
			if(bSttSlab)	dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabSttLeft,   dSttBlockOutWidth+dCoverSd+dNearstMidPoint,  dDisLStt);
			else			dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabEndLeft, -(dEndBlockOutWidth+dCoverSd+dNearstMidPoint), dDisLEnd);
			
			CDPoint vAngCur	  = pGirCen->GetAngleOffset(bSttSlab ? dStaSlabSttLeft : dStaSlabEndCen, bSttSlab ? vAngStt : vAngEnd, (bSttSlab ? 1 : -1)*pGirCen->GetLengthDimType(bSttSlab ? dStaSlabSttCen: dStaSlabEndCen, dStaRebarStt));
			CDPoint xySttRebar= pGirCen->GetXyGirderDis(dStaRebarStt, dDisL);	// 마크의 시작위치(좌측끝, 우측끝)

			for (long nLRC=0; nLRC<3; nLRC++) 
			{
				long   nPos             = nLRC==0 ? POS_SLAB_LEFT : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				long   nRebarSu         = GetsuBeryukAtCantilever(nPos, TRUE, TRUE);
				if(nRebarSu==0)		continue;

				for(long n=nRebarStt; n<nRebarStt+nRebarSu; n++)
				{
					if(dArrDim[n] < dWidthHalfSlab-m_dCoverSd)	// 슬래브중심기준 좌측일 경우만..
					{
						xyMark = xySttRebar-vAngSlab*dArrDim[n]/fabs(vAngCur.y);
						xyArrMark.Add(xyMark);
					}
				}
				nRebarStt = nRebarSu;
				
				pair<CString, CDPointArray> vPair;

				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";

				if (bSttSlab)
				{
					if(bOneCycle)		sText.Format("배력단부상면%sSTT", szPos);
					else				sText.Format("배력단부상면%sSTT%dCycle", szPos, nCycle+1);
				}
				else
				{
					if(bOneCycle)		sText.Format("배력단부상면%sEND", szPos);
					else				sText.Format("배력단부상면%sEND%dCycle", szPos, nCycle+1);
				}
				
				vPair.first  = sText;
				vPair.second = GetDimArrByCycle(xyArrMark, nCycle, nCyclesu);
				xyArrMark.RemoveAll();
				vMarkPos.push_back(vPair);
			}
		}
	}
	else
	{
		// 단부하면2 배력철근 
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			long   nRebarStt   = 0;						// 좌,우,중앙부구역의 시작마크 인덱스
			double dStaStt     = 0;
			double dLenToTaper = 0;
			if(bSttSlab)
			{
				dStaStt = max(dStaSlabSttLeft, dStaSlabSttRigh);
				if(dStaSlabSttLeft>dStaSlabSttRigh)
					dLenToTaper = GetLengthSlabToTaper(bSttSlab, JONGBASEPOS_SLABLEFT);
				else
					dLenToTaper = GetLengthSlabToTaper(bSttSlab, JONGBASEPOS_SLABRIGHT);
			}
			else
			{
				dStaStt = min(dStaSlabEndLeft, dStaSlabEndRigh);
				if(dStaSlabEndLeft<dStaSlabEndRigh)
					dLenToTaper = GetLengthSlabToTaper(bSttSlab, JONGBASEPOS_SLABLEFT);
				else
					dLenToTaper = GetLengthSlabToTaper(bSttSlab, JONGBASEPOS_SLABRIGHT);
			}

			double dOffset			= dLenToTaper + 200 + nCycle*dScale*25;	// Fix Data 
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	

			if(bSttSlab)	dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaStt,   dCoverSd+dNearstMidPoint,  dDisRStt);
			else			dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaStt, -(dCoverSd+dNearstMidPoint), dDisREnd);

			CDPoint xySttRebar	= pGirCen->GetXyGirderDis(dStaRebarStt, dDisR);
			CDPoint vGir = pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt);

			for (long nLRC=0; nLRC<3; nLRC++) 
			{
				long   nPos             = nLRC==0 ? POS_SLAB_RIGH : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_LEFT);
				long   nRebarSu         = GetsuBeryukAtCantilever(nPos, TRUE, FALSE);
				if(nRebarSu==0)		continue;

				for(long n=nRebarStt; n<nRebarStt+nRebarSu; n++)
				{
					if(dArrDim[n] < dWidthHalfSlab-m_dCoverSd)
					{
						xyMark = xySttRebar+vGir.Rotate90()*dArrDim[n];
						xyArrMark.Add(xyMark);
					}
				}
				nRebarStt = nRebarSu;
				
				pair<CString, CDPointArray> vPair;
				
				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";

				if (bSttSlab)
				{
					if(bOneCycle)		sText.Format("배력단부하면2%sSTT", szPos);
					else				sText.Format("배력단부하면2%sSTT-%dCycle", szPos, nCycle+1);
				}
				else
				{
					if(bOneCycle)		sText.Format("배력단부하면2%sEND", szPos);
					else				sText.Format("배력단부하면2%sEND-%dCycle", szPos, nCycle+1);
				}
				vPair.first  = sText;
				vPair.second = GetDimArrByCycle(xyArrMark, nCycle, nCyclesu);
				xyArrMark.RemoveAll();
				vMarkPos.push_back(vPair);
			}
		}
	}
}


// 단부 배력철근 : 사보강철근 없을경우
void CADeckData::MakeRebarMarkPosPlanBeryukDanAtNoSkew(vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bSttSlab, BOOL bUpper, double dScale)
{
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	CRebarPlacing RebarPlacing;
	CDPointArray xyArrDim;	// 단부상면철근 마크의 종단위치 결정을 위한 CTC배열
	double dOffsetCycle = dScale*25;		// Cycle당 이격거리	

	if(bSttSlab)
	{
		xyArrDim = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_RIGHT];
	}
	else
	{
		CDPointArray xyArrTemp = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_RIGHT];
		RebarPlacing.ReverseRebar(xyArrTemp);
		xyArrDim = xyArrTemp;
	}

	double dSttBlockOutWidth = pGlopt->GetSttExpansionJointWidth();
	double dEndBlockOutWidth = pGlopt->GetEndExpansionJointWidth();
	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth = 0;
		dEndBlockOutWidth = 0;
	}

	CPlateGirderApp *pGirCen		= m_pDB->GetGirder(-1);
	
	double dStaSlabSttCen	= pGirCen->GetSSS(OFF_STT_SLAB);
	double dStaSlabEndCen	= pGirCen->GetSSS(OFF_END_SLAB);
	double dDisLStt			= GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDisRStt			= GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDisLEnd			= GetSideBarDistFromCenterGir(FALSE, TRUE);
	double dDisREnd			= GetSideBarDistFromCenterGir(FALSE, FALSE);
	double dDisL			= bSttSlab ? dDisLStt : dDisLEnd;
	double dDisR			= bSttSlab ? dDisRStt : dDisREnd;
	CDPoint	vAngStt			= pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd			= pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft	= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh	= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndLeft	= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh	= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dWidthHalfSlab	= m_pDB->GetWidthAct(bUpper ? dStaSlabSttCen : dStaSlabEndCen)/2;
	double dCoverSd         = GetLengthCoverSd(bSttSlab); 

	CDoubleArray dArrDim;
	CDPoint		 xyMark;
	CDPointArray xyArrMark; 
	CString		 sText;

	if(bUpper)
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCBeryukRebarDan[CTC_BERYUK_UPPER], dArrDim, TRUE);	
	else
	{
		CDPointArray xyArrTemp = m_arrCTCBeryukRebarDan[CTC_BERYUK_LOWER];
		RebarPlacing.ReverseRebar(xyArrTemp);
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrTemp, dArrDim, TRUE);	
	}
	dArrDim.InsertAt(0, 0);

	long	nCyclesu	=  m_stBeryukCycleLeft.size();
	BOOL	bOneCycle	= nCyclesu==1 ? TRUE : FALSE;
	double	dStaRebarStt= 0;

	if(bUpper)
	{
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			long   nRebarStt        = 0;						// 좌,우,중앙부구역의 시작마크 인덱스
			double dOffset			= 500 + nCycle*dOffsetCycle;	// Fix Data 
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	
			if(bSttSlab)	dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabSttLeft,   dSttBlockOutWidth+dCoverSd+dNearstMidPoint,  dDisLStt);
			else			dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabEndLeft, -(dEndBlockOutWidth+dCoverSd+dNearstMidPoint), dDisLEnd);
			
			CDPoint vAngRebar	= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt);
			CDPoint xySttRebar	= pGirCen->GetXyGirderDis(dStaRebarStt, dDisL);

			for (long nLRC=0; nLRC<3; nLRC++) 
			{
				long   nPos             = nLRC==0 ? POS_SLAB_LEFT : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				long   nRebarSu         = GetsuBeryukAtCantilever(nPos, TRUE, TRUE);
				if(nRebarSu==0)		continue;

				for(long n=nRebarStt; n<nRebarStt+nRebarSu; n++)
				{
					if(dArrDim[n] < dWidthHalfSlab-m_dCoverSd)
					{
						xyMark = xySttRebar-vAngRebar.Rotate90()*dArrDim[n];
						xyArrMark.Add(xyMark);
					}
				}
				nRebarStt = nRebarSu;
				
				pair<CString, CDPointArray> vPair;

				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";
				
				if (bSttSlab)
				{
					if(bOneCycle)		sText.Format("배력단부상면%sSTT", szPos);
					else				sText.Format("배력단부상면%sSTT%dCycle", szPos, nCycle+1);
				}
				else
				{
					if(bOneCycle)		sText.Format("배력단부상면%sEND", szPos);
					else				sText.Format("배력단부상면%sEND%dCycle", szPos, nCycle+1);
				}
				
				vPair.first  = sText;
				vPair.second = GetDimArrByCycle(xyArrMark, nCycle, nCyclesu);
				xyArrMark.RemoveAll();
				vMarkPos.push_back(vPair);
			}
		}
	}
	else
	{
		// 단부하면2 배력철근 
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			double dOffset			= GetLengthSlabToTaper(bSttSlab?TRUE:FALSE, JONGBASEPOS_SLABRIGHT) + 200 + nCycle*dOffsetCycle;	// Fix Data 
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	
			if(bSttSlab)	dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabSttRigh,   dCoverSd+dNearstMidPoint,  dDisRStt);
			else			dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabEndRigh, -(dCoverSd+dNearstMidPoint), dDisREnd);
			
			CDPoint vAngRebar	= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt);
			CDPoint xySttRebar	= pGirCen->GetXyGirderDis(dStaRebarStt, dDisR);
			long   nRebarStt    = 0;						// 좌,우,중앙부구역의 시작마크 인덱스

			for (long nLRC=0; nLRC<3; nLRC++) 
			{
				long   nPos             = nLRC==0 ? POS_SLAB_RIGH : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_LEFT);
				long   nRebarSu         = GetsuBeryukAtCantilever(nPos, TRUE, FALSE);
				if(nRebarSu==0)		continue;

				for(long n=nRebarStt; n<nRebarStt+nRebarSu; n++)
				{
					if(dArrDim[n] < dWidthHalfSlab-m_dCoverSd)
					{
						xyMark = xySttRebar+vAngRebar.Rotate90()*dArrDim[n];
						xyArrMark.Add(xyMark);
					}
				}
				nRebarStt = nRebarSu;

				pair<CString, CDPointArray> vPair;

				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";
				if (bSttSlab)
				{
					if(bOneCycle)		sText.Format("배력단부하면2%sSTT", szPos);
					else				sText.Format("배력단부하면2%sSTT-%dCycle", szPos, nCycle+1);
				}
				else
				{
					if(bOneCycle)		sText.Format("배력단부하면2%sEND", szPos);
					else				sText.Format("배력단부하면2%sEND-%dCycle", szPos, nCycle+1);
				}
				vPair.first  = sText;
				vPair.second = GetDimArrByCycle(xyArrMark, nCycle, nCyclesu);
				xyArrMark.RemoveAll();
				vMarkPos.push_back(vPair);
			}
		}
	}
}



// 일반부 철근 : 시작지간과 마지막 지간에만 그림.
void CADeckData::MakeRebarMarkPosPlanBeryukGen(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bUpper, double dScale)
{
	if(nJigan!=0 && nJigan!=m_pDB->m_nQtyJigan-1)		return;

	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	CRebarPlacing RebarPlacing;
	// 철근 마크의 종단위치 결정을 위한 CTC배열
	CDPointArray xyArrDim;
	if(nJigan==0)
	{
		xyArrDim = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_RIGHT];
	}
	else
	{
		CDPointArray xyArrTemp = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_RIGHT];
		RebarPlacing.ReverseRebar(xyArrTemp);
		xyArrDim = xyArrTemp;
	}

	double dSttBlockOutWidth = pGlopt->GetSttExpansionJointWidth();
	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth = 0;
	}

	CPlateGirderApp *pGirCen		= m_pDB->GetGirder(-1);
	
	double dStaSlabSttCen	= pGirCen->GetSSS(OFF_STT_SLAB);
	double dDisLStt			= GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDisRStt			= GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDisLEnd			= GetSideBarDistFromCenterGir(FALSE, TRUE);
	double dDisREnd			= GetSideBarDistFromCenterGir(FALSE, FALSE);
	double dDisL			= nJigan==0 ? dDisLStt : dDisLEnd;
	double dDisR			= nJigan==0 ? dDisRStt : dDisREnd;
	CDPoint	vAngStt			= pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd			= pGirCen->GetAAA(OFF_END_SLAB);
	double dWidthHalfSlab	= m_pDB->GetWidthAct(dStaSlabSttCen)/2;
	double dCoverSdStt		= GetLengthCoverSd(TRUE); 
	double dCoverSdEnd		= GetLengthCoverSd(FALSE); 
	
	CDoubleArray dArrDim;
	CDPoint		 xyMark;
	CDPointArray xyArrMark; 
	CString		 sText;

	if(bUpper)
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCBeryukRebarCen[CTC_BERYUK_UPPER], dArrDim, TRUE);	
	else
	{
		CDPointArray xyArrTemp = m_arrCTCBeryukRebarCen[CTC_BERYUK_LOWER];
		RebarPlacing.ReverseRebar(xyArrTemp);
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrTemp, dArrDim, TRUE);	
	}
	dArrDim.InsertAt(0, 0);

	long	nCyclesu	 = m_stBeryukCycleLeft.size();
	BOOL	bOneCycle	 = nCyclesu==1 ? TRUE : FALSE;
	// Cycle생성시 상부CTC를 기준으로 생성함.
	long nRealCyclesu = GetsuRealCycle(JONGBASEPOS_SLABCENTER, TRUE);
	long nBesu = GetBesu(JONGBASEPOS_SLABCENTER, TRUE);
	double dExtL = 1000;

	if(bUpper)
	{
		for(long nCycle=0; nCycle<nRealCyclesu; nCycle++)
		{
			long   nRebarStt        = 0;						// 좌,우,중앙부구역의 시작마크 인덱스
			long   nCycleCur		= nCycle*nBesu;
			double dOffset			= m_stBeryukCycleLeft[nCycleCur].m_dArrJointUp.GetAt(0) + dExtL + dScale*25*nCycle;	// Fix Data 
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	
			double dStaMainRebarStt = GetStationMainRebarPos(nJigan==0 ? TRUE : FALSE);
			double dStaRebarStt		= 0;
			if(nJigan==0)	dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt,   dSttBlockOutWidth+dCoverSdStt+dNearstMidPoint,  dDisLStt);
			else			dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt, -(dSttBlockOutWidth+dCoverSdEnd+dNearstMidPoint), dDisLEnd);
			
			CDPoint vAngRebar	= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt);
			CDPoint xySttRebar	= pGirCen->GetXyGirderDis(dStaRebarStt, dDisL);

			BOOL bPehap = IsPehapCycle(JONGBASEPOS_SLABLEFT, nCycleCur);
			if(!bPehap)		continue;

			for (long nLRC=0; nLRC<3; nLRC++) 
			{
				long   nPos             = nLRC==0 ? POS_SLAB_LEFT : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_RIGH);
				long   nRebarSu         = GetsuBeryukAtCantilever(nPos, FALSE, TRUE);
				if(nRebarSu==0)		continue;

				for(long n=nRebarStt; n<nRebarStt+nRebarSu; n++)
				{
					if(dArrDim[n] < dWidthHalfSlab-m_dCoverSd)
					{
						xyMark = xySttRebar-vAngRebar.Rotate90()*dArrDim[n];
						xyArrMark.Add(xyMark);
					}
				}
				nRebarStt = nRebarSu;

				pair<CString, CDPointArray> vPair;
				
				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";

				if(bOneCycle)		sText.Format("배력일반부상면%s%d", szPos, nJigan+1);
				else				sText.Format("배력일반부상면%s%d-%dCycle", szPos, nJigan+1, nCycleCur+1);
				
				vPair.first  = sText;
				vPair.second = GetDimArrByCycle(xyArrMark, nCycle, nRealCyclesu);
				xyArrMark.RemoveAll();
				vMarkPos.push_back(vPair);
			}
		}
	}
	else
	{
		for(long nCycle=0; nCycle<nRealCyclesu; nCycle++)
		{
			long   nRebarStt        = 0;						// 좌,우,중앙부구역의 시작마크 인덱스
			long   nCycleCur		= nCycle*nBesu;
			double dOffset			= m_stBeryukCycleLeft[nCycleCur].m_dArrJointDn.GetAt(0) + dExtL + dScale*25*nCycle;
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	
			double dStaMainRebarStt = GetStationMainRebarPos(nJigan==0 ? TRUE : FALSE);
			double dStaRebarStt		= 0;
			if(nJigan==0)	dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt,   dCoverSdStt+dNearstMidPoint,  dDisRStt);
			else			dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt, -(dCoverSdStt+dNearstMidPoint), dDisREnd);
			
			BOOL bPehap = IsPehapCycle(JONGBASEPOS_SLABLEFT, nCycleCur);
			if(!bPehap)		continue;

			CDPoint vAngRebar	= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt);
			CDPoint xySttRebar	= pGirCen->GetXyGirderDis(dStaRebarStt, dDisR);

			for (long nLRC=0; nLRC<3; nLRC++) 
			{
				long   nPos             = nLRC==0 ? POS_SLAB_RIGH : (nLRC==1 ? POS_SLAB_CENTER : POS_SLAB_LEFT);
				long   nRebarSu         = GetsuBeryukAtCantilever(nPos, FALSE, FALSE);
				if(nRebarSu==0)		continue;

				for(long n=nRebarStt; n<nRebarStt+nRebarSu; n++)
				{
					if(dArrDim[n] < dWidthHalfSlab-m_dCoverSd)
					{
						xyMark = xySttRebar+vAngRebar.Rotate90()*dArrDim[n];
						xyArrMark.Add(xyMark);
					}
				}
				nRebarStt = nRebarSu;

				pair<CString, CDPointArray> vPair;

				CString szPos("");
				if(nPos==POS_SLAB_LEFT)			szPos = "L";
				else if(nPos==POS_SLAB_RIGH)	szPos = "R";
				else							szPos = "";
				
				if(bOneCycle)		sText.Format("배력일반부하면%s%d", szPos, nJigan+1);
				else				sText.Format("배력일반부하면%s%d-%dCycle", szPos, nJigan+1, nCycleCur+1);
				
				vPair.first  = sText;
				vPair.second = GetDimArrByCycle(xyArrMark, nCycle, nRealCyclesu);
				xyArrMark.RemoveAll();
				vMarkPos.push_back(vPair);
			}
		}
	}
}




void CADeckData::MakeRebarMarkPosPlanBeryukSub(vector < pair<CString, CDPointArray > > &vMarkPos, CDPoint xySttRebar, CDPoint vRebar, CDPointArray xyArrDimMark, CString szMark, long nCycle, BOOL bUpper, double dScale)
{
	CRebarPlacing RebarPlacing;
	CDoubleArray dArrDim;
	// 마크를 순차적으로 생성하기 위한 누적거리배열생성
	if(bUpper)
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimMark, dArrDim, TRUE);	
	else
	{
		CDPointArray xyArrTemp = xyArrDimMark;
		RebarPlacing.ReverseRebar(xyArrTemp);
		RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrTemp, dArrDim, TRUE);	
	}

	//
	double dWidthHalfSlab = m_pDB->GetWidthAct(m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB)) / 2;
	
	CDPointArray xyArrMark;
	CDPoint xyMark;
	xyArrMark.Add(xySttRebar);

	for(long n=0; n<dArrDim.GetSize(); n++)
	{
		if(dArrDim[n] < dWidthHalfSlab-m_dCoverSd)
		{
			xyMark = xySttRebar + vRebar*dArrDim[n];
			xyArrMark.Add(xyMark);
		}
	}
	
	pair<CString, CDPointArray> vPair;

	vPair.first  = szMark;
	vPair.second = GetDimArrByCycle(xyArrMark, nCycle, m_stBeryukCycleLeft.size());
	vMarkPos.push_back(vPair);
}



void CADeckData::MakeRebarMarkPosPlanBeryukJijum(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bUpper, double dScale)
{
	CPlateGirderApp	*pGirCen	= m_pDB->GetGirder(-1);

	double dDisLStt			= GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDisRStt			= GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDisLEnd			= GetSideBarDistFromCenterGir(FALSE, TRUE);
	double dDisREnd			= GetSideBarDistFromCenterGir(FALSE, FALSE);
	double dDisL			= nJigan==0 ? dDisLStt : dDisLEnd;
	double dDisR			= nJigan==0 ? dDisRStt : dDisREnd;
	double dCoverSd         = GetLengthCoverSd(TRUE); 
	

	CString		 sText;
	CRebarPlacing RebarPlacing;

	// 철근 마크의 종단위치 결정을 위한 CTC배열
	// 마지막 지간을 제외한 지간은 슬래브 시점을 기준으로 MidPoint를 생성함.
	CDPointArray xyArrDim = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_RIGHT];
	if(nJigan==m_pDB->m_nQtyJigan-1)
	{
		CDPointArray xyArrTemp = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_RIGHT];
		RebarPlacing.ReverseRebar(xyArrTemp);
		xyArrDim = xyArrTemp;
	}

	long	nCyclesu	=  m_stBeryukCycleLeft.size();		// 배력철근수
	BOOL	bOneCycle	= nCyclesu==1 ? TRUE : FALSE;	// 단사이클인가?

	// nJigan에서 시작지점스테이션과 끝지점스테이션(첫거더의 지점위치와 마지막거더지점위치의 중간스테이션 : 슬래브 중심이라고 가정)
	double dStaJijumStt = (m_pDB->GetGirder(0)->GetBxOnJijum(nJigan)->GetStation() + m_pDB->GetGirder(m_pDB->GetGirdersu()-1)->GetBxOnJijum(nJigan)->GetStation()) / 2;
	double dStaJijumEnd = (m_pDB->GetGirder(0)->GetBxOnJijum(nJigan+1)->GetStation() + m_pDB->GetGirder(m_pDB->GetGirdersu()-1)->GetBxOnJijum(nJigan+1)->GetStation()) / 2;

	//////////////////////////////////////////////////////////////////////////
	// 일반부 상부마크
	//////////////////////////////////////////////////////////////////////////
	double dOffsetJijum = 3000;		// 지점중심에서 지점배력철근 마크까지의 이격거리
	double dOffsetCycle = dScale*25;		// Cycle당 이격거리
	
	// 신축이음부 폭
	double dSttBlockOutWidth = GetWidthBlockOut(TRUE, bUpper ? JONGBASEPOS_SLABLEFT : JONGBASEPOS_SLABRIGHT);
	double dEndBlockOutWidth = GetWidthBlockOut(FALSE, bUpper ? JONGBASEPOS_SLABLEFT : JONGBASEPOS_SLABRIGHT);
	if(m_bInstallMainRebarAtBlockOut)
	{
		dSttBlockOutWidth = 0;
		dEndBlockOutWidth = 0;
	}		

	if(nJigan==0)
	{
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			double dStaMainRebarStt = GetStationMainRebarPos(TRUE);
			double dLengthToJijum   = m_pDB->m_pLine->GetLengthByDis(dStaMainRebarStt, dStaJijumEnd, bUpper?dDisLStt:dDisRStt);
			double dOffset			= dLengthToJijum - dOffsetJijum + nCycle*dOffsetCycle;	// Fix Data 
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	
			double dStaRebarStt		= m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt, (bUpper?dSttBlockOutWidth:0)+dCoverSd+dNearstMidPoint,  bUpper?dDisLStt:dDisRStt);
			
			CDPoint vAngRebar		= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt).RotateInv90();
			if(bUpper==FALSE)	vAngRebar= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt).Rotate90();

			CDPoint xySttRebar		= pGirCen->GetXyGirderDis(dStaRebarStt, bUpper?dDisL:dDisR);
			if(bUpper)
			{
				if(bOneCycle)		sText.Format("배력지점부상면%d", nJigan+1);
				else				sText.Format("배력지점부상면%d-%dCycle", nJigan+1, nCycle+1);
			}
			else
			{
				if(bOneCycle)		sText.Format("배력지점부하면%d", nJigan+1);
				else				sText.Format("배력지점부하면%d-%dCycle", nJigan+1, nCycle+1);
			}

			MakeRebarMarkPosPlanBeryukSub(vMarkPos, xySttRebar, vAngRebar, m_arrCTCBeryukRebarJijum[bUpper?CTC_BERYUK_UPPER:CTC_BERYUK_LOWER], sText, nCycle, bUpper, dScale);	
		}
	}
	else if(nJigan==m_pDB->m_nQtyJigan-1)
	{
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			double dStaMainRebarStt = GetStationMainRebarPos(FALSE);
			double dLengthToJijum   = m_pDB->m_pLine->GetLengthByDis(dStaJijumStt, dStaMainRebarStt, bUpper?dDisLEnd:dDisREnd);
			double dOffset			= dLengthToJijum - dOffsetJijum - nCycle*dOffsetCycle;	// Fix Data 
			double dNearstMidPoint	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset);	
			double dStaRebarStt		= m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt, -((bUpper?dEndBlockOutWidth:0)+dCoverSd+dNearstMidPoint), bUpper?dDisLEnd:dDisREnd);
			
			CDPoint vAngRebar		= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt).RotateInv90();
			if(bUpper==FALSE)	vAngRebar= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt).Rotate90();
			
			CDPoint xySttRebar		= pGirCen->GetXyGirderDis(dStaRebarStt, bUpper?dDisL:dDisR);
			if(bUpper)
			{
				if(bOneCycle)		sText.Format("배력지점부상면%d", nJigan);
				else				sText.Format("배력지점부상면%d-%dCycle", nJigan, nCycle+1);
			}
			else
			{
				if(bOneCycle)		sText.Format("배력지점부하면%d", nJigan);
				else				sText.Format("배력지점부하면%d-%dCycle", nJigan, nCycle+1);
			}
			
			MakeRebarMarkPosPlanBeryukSub(vMarkPos, xySttRebar, vAngRebar, m_arrCTCBeryukRebarJijum[bUpper?CTC_BERYUK_UPPER:CTC_BERYUK_LOWER], sText, nCycle, bUpper, dScale);	
		}
	}
	else
	{
		for(long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			double dStaMainRebarStt = GetStationMainRebarPos(TRUE);
			double dLengthToJijum1  = m_pDB->m_pLine->GetLengthByDis(dStaMainRebarStt, dStaJijumStt, bUpper?dDisLStt:dDisRStt);
			double dLengthToJijum2  = m_pDB->m_pLine->GetLengthByDis(dStaMainRebarStt, dStaJijumEnd, bUpper?dDisLStt:dDisRStt);
			double dOffset1			= dLengthToJijum1 + dOffsetJijum + nCycle*dOffsetCycle;	// Fix Data 
			double dOffset2			= dLengthToJijum2 - dOffsetJijum - nCycle*dOffsetCycle;	// Fix Data 
			double dNearstMidPoint1	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset1);	
			double dNearstMidPoint2	= GetNearstMidPointAtCTCArr(xyArrDim, dOffset2);	
			double dStaRebarStt1	= m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt, (bUpper?dSttBlockOutWidth:0)+dCoverSd+dNearstMidPoint1, bUpper?dDisLStt:dDisRStt);
			double dStaRebarStt2	= m_pDB->m_pLine->GetStationByDisLength(dStaMainRebarStt, (bUpper?dSttBlockOutWidth:0)+dCoverSd+dNearstMidPoint2, bUpper?dDisLStt:dDisRStt);
			CDPoint vAngRebar1		= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt1).RotateInv90();
			CDPoint vAngRebar2		= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt2).RotateInv90();
			if(bUpper==FALSE)
			{
				vAngRebar1	= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt1).Rotate90();
				vAngRebar2	= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt2).Rotate90();
			}
			CDPoint xySttRebar1		= pGirCen->GetXyGirderDis(dStaRebarStt1, bUpper?dDisL:dDisR);
			CDPoint xySttRebar2		= pGirCen->GetXyGirderDis(dStaRebarStt2, bUpper?dDisL:dDisR);
			
			if(bUpper)
			{
				if(bOneCycle)		sText.Format("배력지점부상면%d", nJigan);
				else				sText.Format("배력지점부상면%d-%dCycle", nJigan, nCycle+1);
				MakeRebarMarkPosPlanBeryukSub(vMarkPos, xySttRebar1, vAngRebar1, m_arrCTCBeryukRebarJijum[bUpper?CTC_BERYUK_UPPER:CTC_BERYUK_LOWER], sText, nCycle, bUpper, dScale);	

				if(bOneCycle)		sText.Format("배력지점부상면%d", nJigan+1);
				else				sText.Format("배력지점부상면%d-%dCycle", nJigan+1, nCycle+1);
				MakeRebarMarkPosPlanBeryukSub(vMarkPos, xySttRebar2, vAngRebar2, m_arrCTCBeryukRebarJijum[bUpper?CTC_BERYUK_UPPER:CTC_BERYUK_LOWER], sText, nCycle, bUpper, dScale);	
			}
			else
			{
				if(bOneCycle)		sText.Format("배력지점부하면%d", nJigan);
				else				sText.Format("배력지점부하면%d-%dCycle", nJigan, nCycle+1);
				MakeRebarMarkPosPlanBeryukSub(vMarkPos, xySttRebar1, vAngRebar1, m_arrCTCBeryukRebarJijum[bUpper?CTC_BERYUK_UPPER:CTC_BERYUK_LOWER], sText, nCycle, bUpper, dScale);	

				if(bOneCycle)		sText.Format("배력지점부하면%d", nJigan+1);
				else				sText.Format("배력지점부하면%d-%dCycle", nJigan+1, nCycle+1);
				MakeRebarMarkPosPlanBeryukSub(vMarkPos, xySttRebar2, vAngRebar2, m_arrCTCBeryukRebarJijum[bUpper?CTC_BERYUK_UPPER:CTC_BERYUK_LOWER], sText, nCycle, bUpper, dScale);	
			}
		}
	}
}



void CADeckData::MakeRebarMarkPosPlanCross(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, double dScale)
{
	CString szPos;
	CDPointArray xyArrMark;
	CDoubleArray dArr;	

	for(long n=0; n<m_arrSangseIndexCB.GetSize(); n++)
	{
		long nG = (long)m_arrSangseIndexCB.GetAt(n).x;
		long nCross = (long)m_arrSangseIndexCB.GetAt(n).y;

		CPlateGirderApp  *pGir	= m_pDB->GetGirder(nG);
		CSection *pSec = pGir->GetSectionCRandVBByNo(nCross);
		if(pSec==NULL)	continue;
		
		if(!pSec->IsenDetType(SECDET_CROSSBEAM))	continue;

		CPlateCrossBeam *pC = pSec->GetCrossBeam();

		CPlateBasicIndex *pBx	= pSec->GetBx();
		CPlateBasicIndex *pBxR	= pBx->GetBxRight();
		CPlateGirderApp  *pGirR	= pBx->GetGirder()->m_pRight;
		CDPoint xyCen = m_arrSangsePosCB[n];

		double dStaCrAtCenter = pGir->GetStationCenterLine(pBx->GetStation(), pGir->m_dGirderCenter, pBx->GetAngle());
		double dStaStt = m_pDB->GetGirder(-1)->GetBxOnJijum(nJigan)->GetStation();
		double dStaEnd = m_pDB->GetGirder(-1)->GetBxOnJijum(nJigan+1)->GetStation();

		if(dStaStt > dStaCrAtCenter || dStaEnd < dStaCrAtCenter)		continue;

		//////////////////////////////////////////////////////////////////////////
		// C2
		CDPoint xy1 = pGir->GetXyPlanSlabCrossbeamBase(pBx, TRUE);
		CDPoint xy2 = pGir->GetXyPlanSlabCrossbeamBase(pBx, FALSE);
		CDPoint xyc = (xy1 + xy2)/2 + (xy1-xy2).Unit()*m_dDiaSangsePosCB/4;	
		vector<REBAR_SHAPE> vArr = GetNewRebarShapeArrBySection(SECTION_CEN, 0, FALSE);	// 슬래브 좌측 배력철근
		CDPointArray xyArrRebar = GetXyArrPoint("가로보상부전체", vArr);
		CVector vVecLine = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		if(xyArrRebar.GetSize() > 0)	
		{
			double dWidth = pC->m_dM_CR + 50-m_dCoverSd;
			CDPoint vVec = (xy2-xy1).Unit();
			CDPoint vAngBetween = vVecLine.Rotate(pBx->GetAngle());
			CDPoint pt2 = xyc - vVecLine*dWidth/fabs(pBx->GetAngle().y);
			CDPoint pt4 = xyc + vVecLine*dWidth/fabs(pBx->GetAngle().y);
			xyArrMark.Add(pt2);
			xyArrMark.Add(pt4);
			
			pair<CString, CDPointArray> vPair;

			vPair.first  = "가로보하면보강";
			vPair.second = xyArrMark;
			xyArrMark.RemoveAll();
			vMarkPos.push_back(vPair);
		}		

		//////////////////////////////////////////////////////////////////////////
		// C1
		GetBoganRebarArrAtCrossBeam(dArr, pBx);
		CDPoint xyCur, ptStt, ptEnd;
		double dOffset = m_dDiaSangsePosCB/4;	// C1, C3구분을 위한 이격거리
		double dOffsetDist = 15;// C1, C3구분을 위한 이격거리
			
		for(n=0; n<dArr.GetSize(); n++)
		{
			double dDist = dArr.GetAt(n);
			double sta	 = pBx->GetStation();
			CDPoint vAngCross = pBx->GetAngle();
			xyCur	 = pGir->GetXyGirderDis(sta-dOffset, dDist-pGir->m_dGirderCenter, vAngCross);	
			if(IsPointInCircle(xyCur, xyCen, m_dDiaSangsePosCB/2))
				xyArrMark.Add(xyCur);
		}
			
		pair<CString, CDPointArray> vPair;

		vPair.first  = "가로보헌치보강";
		vPair.second = xyArrMark;
		xyArrMark.RemoveAll();
		vMarkPos.push_back(vPair);	

		//////////////////////////////////////////////////////////////////////////
		// C3
		for(n=0; n<dArr.GetSize(); n++)
		{
			double dDist = dArr.GetAt(n);
			double sta	 = pBx->GetStation();
			CDPoint vAngCross = pBx->GetAngle();
			xyCur	 = pGir->GetXyGirderDis(sta+dOffset, dDist-pGir->m_dGirderCenter + dOffsetDist, vAngCross);	
			if(IsPointInCircle(xyCur, xyCen, m_dDiaSangsePosCB/2))
				xyArrMark.Add(xyCur);
		}
			
		vPair.first  = "가로보상면보강";
		vPair.second = xyArrMark;
		xyArrMark.RemoveAll();
		vMarkPos.push_back(vPair);	
	}
}


// nStgAtTotal = m_arrStringer
void CADeckData::MakeRebarMarkPosPlanStringerSub(vector < pair<CString, CDPointArray > > &vMarkPos, long nSangseSt, long nStgAtTotal, double dScale)
{
/*	CString szPos;
	CDPointArray xyArrMark;
	CDoubleArray dArr;	

	CStringer	*pStg  = GetStgByStgNo(nStgAtTotal);
	CPlateBasicIndex *pBxCr = pStg->GetBx();
	CPlateGirderApp  *pGir  = pBxCr->GetGirder(); 

	BOOL bMatchStt = FALSE, bMatchEnd = FALSE;
	CDPoint xyMatchStt, xyMatchEnd;

	CDPoint xy1 = pGir->GetXyPlanSlabStgBase(pBxCr, pStg->m_nStg, TRUE);
	CPlateBasicIndex *pBxNext = pGir->GetBxByCrossNo(pGir->GetNumberCrossBeamByBx(pBxCr)+1);
	CDPoint xy2 = pGir->GetXyPlanSlabStgBase(pBxNext, pStg->m_nStg, FALSE);
	IsStringerMatchAtTaper(pStg, bMatchStt, bMatchEnd, xyMatchStt, xyMatchEnd);
	if(bMatchStt)	xy1 = xyMatchStt;
	if(bMatchEnd)	xy2 = xyMatchEnd;

	CDPoint vVec = (xy2 - xy1).Unit();	// 세로보방향 
	CDPoint xyc  = (xy1 + xy2) / 2;		// 세로보의 중심
	CDPoint xyCircleCen = m_arrSangsePosST[nSangseSt];
	
	//////////////////////////////////////////////////////////////////////////
	// 헌치 보강철근(S3)
	double dRebarLen = GetLengthStringerHunchRebar(pBxCr, pStg->m_nStg, FALSE);
	
	CDPoint xyDim = m_arrCTCBogangRebarAtStringer.GetAt(nStgAtTotal);
	CDPoint xyBase = xyc - vVec * (xyDim.x * xyDim.y)/2;	// 보강철근이 시작되는 위치
	CDPoint xyMark;
			
	for(long n=0; n<xyDim.x; n++)
	{
		xyMark = xyBase + vVec.Rotate90()*m_dDiaSangsePosST/4;
		if(IsPointInCircle(xyMark, xyCircleCen, m_dDiaSangsePosST/2))
			xyArrMark.Add(xyMark);
		xyBase += vVec * xyDim.y;
	}

	pair<CString, CDPointArray> vPair;

	vPair.first  = "세로보헌치보강";
	vPair.second = xyArrMark;
	xyArrMark.RemoveAll();
	vMarkPos.push_back(vPair);	

	//////////////////////////////////////////////////////////////////////////
	// 상면 보강철근(S1)
	if(m_bInstallStringerUpperBogangRebar)
	{
		double dOffsetDist = 15;// S1, S3구분을 위한 이격거리
		dRebarLen = GetLengthStringerHunchRebar(pBxCr, pStg->m_nStg, TRUE);
		xyBase    = xyc - vVec * ((xyDim.x * xyDim.y)/2 - dOffsetDist);	// 보강철근 시작위치 초기화
		for(long n=0; n<xyDim.x; n++)
		{
			xyMark = xyBase - vVec.Rotate90()*m_dDiaSangsePosST/4;			
			if(IsPointInCircle(xyMark, xyCircleCen, m_dDiaSangsePosST/2))
				xyArrMark.Add(xyMark);
			xyBase += vVec * xyDim.y;
		}

		vPair.first  = "세로보상면보강";
		vPair.second = xyArrMark;
		xyArrMark.RemoveAll();
		vMarkPos.push_back(vPair);	
	}

	//////////////////////////////////////////////////////////////////////////
	//교축방향 철근(S2)세로보 종점부
	double dWidthHalf = pStg->m_dM + 50 - m_dCoverSd;
	xyMark = xyc + vVec*m_dDiaSangsePosST/4 - vVec.Rotate90()*dWidthHalf;
	xyArrMark.Add(xyMark);

	xyMark = xyc + vVec*m_dDiaSangsePosST/4  + vVec.Rotate90()*dWidthHalf;
	xyArrMark.Add(xyMark);
	
	vPair.first  = "세로보하면보강";
	vPair.second = xyArrMark;
	xyArrMark.RemoveAll();
	vMarkPos.push_back(vPair);*/
}



void CADeckData::MakeRebarMarkPosPlanStringer(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, double dScale)
{
/*	for(long n=0; n<m_arrSangseIndexST.GetSize(); n++)
	{
		long nG = (long)m_arrSangseIndexST.GetAt(n).x;
		long nC = (long)m_arrSangseIndexST.GetAt(n).y;

		CPlateGirderApp  *pGir = m_pDB->GetGirder(nG);
		CPlateCrossBeam  *pC   = pGir->GetCrossBeamByCrossNo(nC);
		CPlateBasicIndex *pBx  = pC->GetBx();

		long nStgAtTotal = GetStgNoByBx(pBx, 0);

		double dStaStgAtCenter = pGir->GetStationCenterLine(pBx->GetStation(), pGir->m_dGirderCenter, pBx->GetAngle());
		double dStaStt = m_pDB->GetGirder(-1)->GetBxOnJijum(nJigan)->GetStation();
		double dStaEnd = m_pDB->GetGirder(-1)->GetBxOnJijum(nJigan+1)->GetStation();
		if(nJigan==0)
			dStaStt = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
		if(nJigan==m_pDB->m_nQtyJigan-1)
			dStaEnd = m_pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB);

		if(dStaStt > dStaStgAtCenter || dStaEnd < dStaStgAtCenter)		continue;

		MakeRebarMarkPosPlanStringerSub(vMarkPos, n, nStgAtTotal, dScale);	
	}	*/

}


void CADeckData::MakeRebarMarkPosPlanOutBrk(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, double dScale)
{
/*	CString szPos;
	CDPointArray xyArrMark;
	CDoubleArray dArr;	

	for(long n=0; n<m_arrSangseIndexOBK.GetSize(); n++)
	{
		BOOL bLeft  = (long)m_arrSangseIndexOBK.GetAt(n).x==0 ? TRUE : FALSE;
		long nObk   = (long)m_arrSangseIndexOBK.GetAt(n).y;

		CPlateGirderApp *pGir = m_pDB->GetGirder(bLeft?0:m_pDB->GetGirdersu()-1);
		CBracket   *pBrk = m_pDB->GetBracket(nObk, bLeft);
		CPlateBasicIndex *pBx = pBrk->GetBx();
		CDPoint xyCen = m_arrSangsePosOBK[n];
		
		if(pBrk==NULL)	continue;

		double dStaCrAtCenter = pGir->GetStationCenterLine(pBx->GetStation(), pGir->m_dGirderCenter, pBx->GetAngle());
		double dStaStt = m_pDB->GetGirder(-1)->GetBxOnJijum(nJigan)->GetStation();
		double dStaEnd = m_pDB->GetGirder(-1)->GetBxOnJijum(nJigan+1)->GetStation();

		if(dStaStt > dStaCrAtCenter || dStaEnd < dStaCrAtCenter)		continue;

		//////////////////////////////////////////////////////////////////////////
		// OB2
		CDPoint xy1 = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, TRUE);
		CDPoint xy2 = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, FALSE);
		CDPoint xyc = (xy1 + xy2)/2 + (xy2-xy1).Unit()*m_dDiaSangsePosCB/4;	
		CVector vVecLine = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		double dWidth = pBrk->m_uM_EB + m_pDB->m_dWidthHunch - m_dCoverSd;
		CDPoint vVec = (xy2-xy1).Unit();
		CDPoint vAngBetween = vVecLine.Rotate(pBx->GetAngle());
		CDPoint pt2 = xyc - vVecLine*dWidth/fabs(pBx->GetAngle().y);
		CDPoint pt4 = xyc + vVecLine*dWidth/fabs(pBx->GetAngle().y);
		xyArrMark.Add(pt2);
		xyArrMark.Add(pt4);
		
		pair<CString, CDPointArray> vPair;

		vPair.first  = "외측가로보하면보강";
		vPair.second = xyArrMark;
		xyArrMark.RemoveAll();
		vMarkPos.push_back(vPair);

		//////////////////////////////////////////////////////////////////////////
		// OB1
		GetBoganRebarArrAtOutBrk(dArr, pBx, bLeft);
		CDPoint xyCur, ptStt, ptEnd;
		double dStaCen = 0;
		double dOffset = m_dDiaSangsePosOBK/4;	// C1, C3구분을 위한 이격거리
		double dOffsetDist = 15;// C1, C3구분을 위한 이격거리
			
		for(long n=0; n<dArr.GetSize(); n++)
		{
			double dDist = dArr.GetAt(n);
			double sta	 = pBx->GetStation();
			CDPoint vAngCross = pBx->GetAngle(); 
			xyCur	 = pGir->GetXyGirderDis(sta-dOffset, dDist-pGir->m_dGirderCenter, vAngCross);	
			if(IsPointInCircle(xyCur, xyCen, m_dDiaSangsePosOBK/2))
				xyArrMark.Add(xyCur);
		}
			
		pair<CString, CDPointArray> vPair2;

		vPair2.first  = "외측가로보헌치보강";
		vPair2.second = xyArrMark;
		xyArrMark.RemoveAll();
		vMarkPos.push_back(vPair2);	

		//////////////////////////////////////////////////////////////////////////
		// OB3
		for(n=0; n<dArr.GetSize(); n++)
		{
			double dDist = dArr.GetAt(n);
			double sta	 = pBx->GetStation();
			CDPoint vAngCross = pBx->GetAngle(); 
			xyCur	 = pGir->GetXyGirderDis(sta+dOffset, dDist-pGir->m_dGirderCenter + dOffsetDist, vAngCross);	
			if(IsPointInCircle(xyCur, xyCen, m_dDiaSangsePosOBK/2))
				xyArrMark.Add(xyCur);
		}
			
		pair<CString, CDPointArray> vPair3;

		vPair3.first  = "외측가로보상면보강";
		vPair3.second = xyArrMark;
		xyArrMark.RemoveAll();
		vMarkPos.push_back(vPair3);	
	}*/
}


// 캔틸레버부 상세는 일반부 철근위치에 정한다고 가정함..
void CADeckData::MakeRebarMarkPosPlanSangseDanbu(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos)
{
	CPlateGirderApp		*pGirCen = m_pDB->GetGirder(-1);

	if(nJigan>0 && nJigan<m_pDB->m_nQtyJigan-1)		return;

	CRebarPlacing RebarPlacing;
	CDPointArray xyArrMark;
	CDPoint xyMark;
	CDoubleArray dArrDim;
	CDPointArray xyArrTemp = m_arrCTCBeryukRebarDan[CTC_BERYUK_LOWER];
	RebarPlacing.ReverseRebar(xyArrTemp);
	RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrTemp, dArrDim, TRUE);	
	dArrDim.InsertAt(0, 0);

	double dOffset = 100, dStaRebarStt = 0;;	
	double dStaSlabSttCen	= pGirCen->GetSSS(OFF_STT_SLAB);
	double dStaSlabEndCen	= pGirCen->GetSSS(OFF_END_SLAB);
	double dDisRStt			= GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDisREnd			= GetSideBarDistFromCenterGir(FALSE, FALSE);
	CDPoint	vAngStt			= pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd			= pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttRigh	= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndRigh	= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dNearstMidPoint  = GetNearstMidPointAtCTCArr(m_arrCTCMainRebarUnderSinchuk, dOffset);		// 사보강철근 간격
	double dCoverSd         = GetLengthCoverSd(nJigan==0?TRUE:FALSE); 
	

	CString sText;
	for (long nPos=0; nPos<m_arrSangsePosDan.GetSize(); nPos++)
	{
		CDPoint xyCen = CDPoint(m_arrSangsePosDan[nPos]);
		if(!IsPointInJigan(xyCen, nJigan))	continue;	// 현재 상세 위치가 현재 지간에 있는지 검사..

		BOOL bStt = m_arrSangsePosDan[nPos].z==-1 ? TRUE: FALSE;	// 상세위치가 슬래브 좌측이면 하면철근을 표현함/
		BOOL bExistSkew = m_arrCTCSabogangRebar[bStt ? CTC_SABOGANG_LOWER_STT : CTC_SABOGANG_LOWER_END].GetSize() > 0 ? TRUE : FALSE;
		if(bStt)	dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabSttRigh,   dCoverSd+dNearstMidPoint,  dDisRStt);
		else		dStaRebarStt = m_pDB->m_pLine->GetStationByDisLength(dStaSlabEndRigh, -(dCoverSd+dNearstMidPoint), dDisREnd);
		double dHeightTaper = bStt ? m_pDB->m_dHeightTaperStt : m_pDB->m_dHeightTaperEnd;

		double dDisR = bStt? dDisRStt : dDisREnd;
		CDPoint vAngSlab;
		CDPoint A[4];
		if(bStt)
		{
			m_pDB->GetXySss(OFF_STT_SLAB, A);
			vAngSlab = (A[0]-A[3]).Unit();
		}
		else
		{
			m_pDB->GetXySss(OFF_END_SLAB, A);
			vAngSlab = (A[0]-A[3]).Unit();
		}

		if(bExistSkew)
		{
			CDPoint xySttRebar = pGirCen->GetXyGirderDis(dStaRebarStt, dDisR);
			xyArrMark.Add(xySttRebar);

			if(dHeightTaper > 0)
			{
				for(long n=0; n<dArrDim.GetSize(); n++)
				{
					xyMark = xySttRebar+vAngSlab*dArrDim[n]/(bStt ? vAngStt.y : vAngEnd.y);
					xyArrMark.Add(xyMark);
				}
			}
		}
		else
		{
			CDPoint vAngRebar	= pGirCen->GetLine()->GetAngleAzimuth(dStaRebarStt);
			CDPoint xySttRebar	= pGirCen->GetXyGirderDis(dStaRebarStt, dDisR);

			if(dHeightTaper > 0)
			{
				for(long n=0; n<dArrDim.GetSize(); n++)
				{
					xyMark = xySttRebar+vAngRebar.Rotate90()*dArrDim[n];
					xyArrMark.Add(xyMark);
				}
			}
		}
		pair<CString, CDPointArray> vPair;

		sText.Format("배력단부하면1-%d", nPos);

		xyCen = m_arrSangsePosDan[nPos];
		GetPointArrInCircle(xyArrMark, xyCen, m_dDiaSangsePosDan/2);
		vPair.first  = sText;
		vPair.second = xyArrMark;

		xyArrMark.RemoveAll();
		vMarkPos.push_back(vPair);
	}
}




// 캔틸레버부 상세는 일반부 철근위치에 정한다고 가정함..
void CADeckData::MakeRebarMarkPosPlanSangseCant(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos)
{
	CRebarPlacing RebarPlacing;
	
	CDPointArray xyArrMarkPosStt, xyArrMarkPosEnd;
	CTwinVectorArray tvArr;
	vector <REBAR_PLAN> *vPlanUp = GetMainRebarArr(TRUE, DRAW_SIDEALL);
	vector <REBAR_PLAN> *vPlanDn = GetMainRebarArr(FALSE, DRAW_SIDEALL);
	double dWidthHDan = m_pDB->GetWidthAct(m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB));

	CDPointArray xyArrDim;
	CString sText;
	for (long n=0; n<m_arrSangsePosCant.GetSize(); n++)
	{
		CDPoint xyCen = CDPoint(m_arrSangsePosCant[n]);
		if(!IsPointInJigan(xyCen, nJigan))	continue;	// 현재 상세 위치가 현재 지간에 있는지 검사..

		BOOL bUpperRebar = m_arrSangsePosCant[n].z==-1 ? FALSE: TRUE;	// 상세위치가 슬래브 좌측이면 하면철근을 표현함/
		if(!bUpperRebar)
		{
			xyArrDim = m_arrCTCBeryukRebarCen[CTC_BERYUK_LOWER];
			CRebarPlacing RebarPlace;
			RebarPlace.ReverseRebar(xyArrDim);
		}			
		else
		{
			xyArrDim = m_arrCTCBeryukRebarCen[CTC_BERYUK_UPPER];
		}

		long nSection = 0;
		if(IsSameMainRebarDanGen())	nSection = SECTION_DAN;
		else						nSection = SECTION_CEN;				
			
		double dMaxCTC = GetMaxCTC(xyArrDim);
		long nCyclesu = GetRebarCycleSuBySection(nSection);
		for (long nCycle=0; nCycle<nCyclesu; nCycle++)		// 한 사이클만 표시해줘도 될듯함.
		{
			sText.Format("캔틸레버상세%d_주철근%dCycle", n+1, nCycle+1);
	
			REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, nCycle);
			long nShapesu = bUpperRebar ? pRebarCycle->m_dArrValidUp.GetSize() : pRebarCycle->m_dArrValidDn.GetSize();
			long nShape   = m_arrSangsePosCant[n].z==-1 ? 0 : nShapesu-1;
			BOOL bValid   = bUpperRebar ? (BOOL)pRebarCycle->m_dArrValidUp[nShape] : (BOOL)pRebarCycle->m_dArrValidDn[nShape];
			if(bValid==FALSE)
				continue;
			// 각 사이클과 형태별로 마크위치를 생성함
			GetTwinVectorArrByCycleAndShape(bUpperRebar?vPlanUp:vPlanDn, &tvArr, nCycle, nShape);

			if(tvArr.GetSize() > 0)
			{
				double dLenToNearCTC = GetNearstMidPointAtCTCArr(xyArrDim, dWidthHDan-m_dDiaSangsePosCant/4-nCycle*dMaxCTC);
				CDPointArray xyArrMark = GetMarkPosArrAtSangCant(&tvArr, dLenToNearCTC, bUpperRebar);
				CDPointArray xyArrTemp;
				for (long m=0; m<xyArrMark.GetSize(); m++)
				{
					if(IsPointInCircle(xyArrMark[m], xyCen, m_dDiaSangsePosCant/2))
						xyArrTemp.Add(xyArrMark[m]);
				}
									
				pair<CString, CDPointArray> vPair;

				vPair.first		= sText;
				vPair.second	= xyArrTemp;
				vMarkPos.push_back(vPair);		
			}
		}
	}
}



BOOL CADeckData::IsPointInCircle(CDPoint xyPos, CDPoint xyCen, double dRad)
{
	if(~(xyPos-xyCen)<dRad)
		return TRUE;
	else
		return FALSE;
}


// tVector의 연장선상에서 선형중심에서 dDist만큼 떨어진 위치
CDPoint CADeckData::GetXyPosByTwinVectorAndDist(CTwinVector tVector, double dDist)
{
	CDPoint vTVector = (tVector.m_v1-tVector.m_v2).Unit();
	double dStaCen = 0;
	m_pDB->m_pLine->GetStationMatchByXyAndAngle(tVector.m_v1, vTVector, dStaCen);
	CDPoint vGir = m_pDB->m_pLine->GetAngleAzimuth(dStaCen);
	CDPoint vAng = vGir.BetweenAngle(vTVector);
	CDPoint xyPos = m_pDB->m_pLine->GetXyLineDisByAng(dStaCen, dDist, vAng);

	return xyPos;
}


// xyPos가 선형에서 떨어진 이격거리
double CADeckData::GetDistByXyPos(CDPoint xyPos)
{
	double dStaCen = 0;
	m_pDB->m_pLine->GetStationMatchVerticalByXy(xyPos, dStaCen);
	CDPoint xyCen = m_pDB->m_pLine->GetXyLine(dStaCen);
	CDPoint vGir = m_pDB->m_pLine->GetAngleAzimuth(dStaCen);
	BOOL bRotateRight = IsDirectionRotateRight(xyCen, vGir, xyPos);
	double dLen = ~(xyCen-xyPos);
	if(dLen<1)
		return 0;
	else if(bRotateRight)
		return dLen;
	else if(!bRotateRight)
		return -dLen;
	
	return 0;
}


// tvArrPlane	= 마킹할 철근
// xyArrDim		= 마킹적정위치를 찾기 위한 CTC배열
// dRatioMarkPos= 철근시점으로부터의 길이비
CDPointArray CADeckData::GetMarkPosArr(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, double dRatio, BOOL bUpper)
{
	CDPointArray xyReturn;
	if(tvArrPlane->GetSize()==0)	return xyReturn;

	if(bUpper)
	{
		double dDistLeft = GetDistByJongBase(JONGBASEPOS_SLABLEFT);
		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			double dDistStt = GetDistByXyPos(tVector.m_v1);
			double dDistEnd = GetDistByXyPos(tVector.m_v2);
			double dDist = (dDistStt+(dDistEnd-dDistStt)*dRatio)-dDistLeft;
			double dLenToNearMidCTC = GetPointByOffsetAndCTC(xyArrDim, Round(dDist,0));
			xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistLeft+dLenToNearMidCTC));
		}
	}
	else
	{
		CRebarPlacing RebarPlace;
		RebarPlace.ReverseRebar(xyArrDim);

		double dDistRigh = GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			double dDistStt = GetDistByXyPos(tVector.m_v1);	// 슬래브 바깥쪽
			double dDistEnd = GetDistByXyPos(tVector.m_v2);	// 슬래브 안쪽
			double dDist = dDistRigh-(dDistStt-(dDistStt-dDistEnd)*dRatio);
			double dLenToNearMidCTC = GetPointByOffsetAndCTC(xyArrDim, Round(dDist,0));
			xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistRigh-dLenToNearMidCTC));
		}
	}

	return xyReturn;
}

//사각부 주철근에 대한 마킹 위치 구하기 
CDPointArray CADeckData::GetMarkPosArrSkewMain(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, BOOL bUpper, double dRatio)
{
	CDPointArray xyReturn, xyResult;
	if(tvArrPlane->GetSize()==0)	return xyReturn;

	CDoubleArray dArrDist;

	//철근의 마킹은 중앙 선형에 가깝도록 한다.
	double dDistCen = bUpper? GetDistByJongBase(JONGBASEPOS_SLABLEFT) : GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
	double dDir = bUpper? -1.0 : + 1.0;
	for(long n=0; n<tvArrPlane->GetSize(); n++)
	{
		CTwinVector tVector = tvArrPlane->GetAt(n);
		double dDistStt = GetDistByXyPos(tVector.m_v1);
		double dDistEnd = GetDistByXyPos(tVector.m_v2);
		double dDist = 0;
		if(bUpper)
		{
			dDist = (dDistStt+(dDistEnd-dDistStt)*dRatio)-dDistCen;
		}
		else
		{
			dDist = dDistCen-(dDistStt-(dDistStt-dDistEnd)*dRatio);
		}

		double dLenToNearMidCTC = GetPointByOffsetAndCTC(xyArrDim, Round(dDist,0));

		if(bUpper)
		{
			double dTotL = dDistCen+dLenToNearMidCTC;
			xyResult.Add(GetXyPosByTwinVectorAndDist(tVector, dTotL));
		}
		else
		{
			double dTotR = dDistStt+dLenToNearMidCTC;
			xyResult.Add(GetXyPosByTwinVectorAndDist(tVector, dTotR));
		}
		dArrDist.Add(dDist);		
	}

	//검사
	double dMaxDist =0, dMinDist = 0;
	for (long ix =0; ix < dArrDist.GetSize(); ix++)
	{
		double dDistTmp = dArrDist.GetAt(ix);
		if(ix ==0)
		{
			dMinDist = dMaxDist = dDistTmp;
			continue;
		}
		dMaxDist = max(dDistTmp, dMaxDist);
		dMinDist = min(dDistTmp, dMinDist);
	}

	if(Compare(dMaxDist , dMinDist , _T("="), 10) == FALSE)
	{
		//이격 거리가 좀 차이가 날 경우 가장 작은 값으로 통일
		double dDir = 1;
		if(bUpper && dMinDist > 0)
		{
			dMinDist *= -1;
		}
		else if(!bUpper && dMinDist < 0)
		{
			dMinDist *= -1;
		}

		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dMinDist));		
		}
	}
	else
	{
		for (long ix =0; ix < xyResult.GetSize(); ix++)
		{
			xyReturn.Add(xyResult.GetAt(ix));
		}		
	}


	return xyReturn;
}

// tvArrPlane	= 마킹할 철근
// xyArrDim		= 마킹적정위치를 찾기 위한 CTC배열
// dRatioMarkPos= 철근시점으로부터의 길이비
CDPointArray CADeckData::GetMarkPosArrAtSangCant(CTwinVectorArray *tvArrPlane, double dLenToNearMidCTC, BOOL bUpper)
{
	CDPointArray xyReturn;
	if(tvArrPlane->GetSize()==0)	return xyReturn;

	if(bUpper)
	{
		double dDistLeft = GetDistByJongBase(JONGBASEPOS_SLABLEFT);
		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistLeft+dLenToNearMidCTC));
		}
	}
	else
	{
		double dDistRigh = GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistRigh-dLenToNearMidCTC));
		}
	}

	return xyReturn;
}




// tvArrPlane	= 마킹할 철근
// xyArrDim		= 마킹적정위치를 찾기 위한 CTC배열
// dRatioMarkPos= 철근시점으로부터의 길이비
CDPointArray CADeckData::GetMarkPosArrAtMainSkew(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, BOOL bUpper, BOOL bFromStt)
{
	CDPointArray xyReturn;
	if(tvArrPlane->GetSize()==0)	return NULL;

	double dDistLeft = GetDistByJongBase(JONGBASEPOS_SLABLEFT);
	double dDistRigh = GetDistByJongBase(JONGBASEPOS_SLABRIGHT);
	double dExtLen   = 1000;
	double dLenToNearMidCTC = 0;

	if(bUpper)
	{
		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			double dDistStt = GetDistByXyPos(tVector.m_v1);	// 슬래브 바깥쪽
			double dDistEnd = GetDistByXyPos(tVector.m_v2);	// 슬래브 안쪽
			dLenToNearMidCTC = GetPointByOffsetAndCTC(xyArrDim, dExtLen);
			if(bFromStt)
			{
				if(dDistLeft+dLenToNearMidCTC<dDistEnd)
					xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistLeft+dLenToNearMidCTC));
			}
			else	
			{
				if(dDistLeft+dLenToNearMidCTC>dDistStt)
					xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistLeft+dLenToNearMidCTC));
			}
		}
	}
	else
	{
		CRebarPlacing RebarPlace;
		RebarPlace.ReverseRebar(xyArrDim);

		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			double dDistStt = GetDistByXyPos(tVector.m_v1);	// 슬래브 바깥쪽
			double dDistEnd = GetDistByXyPos(tVector.m_v2);	// 슬래브 안쪽
			dLenToNearMidCTC = GetPointByOffsetAndCTC(xyArrDim, dExtLen);
			if(bFromStt)
			{
				if(dDistRigh-dLenToNearMidCTC>dDistEnd)
					xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistRigh-dLenToNearMidCTC));
			}
			else
			{
				if(dDistRigh-dLenToNearMidCTC<dDistStt)
					xyReturn.Add(GetXyPosByTwinVectorAndDist(tVector, dDistRigh-dLenToNearMidCTC));
			}
		}
	}

	return xyReturn;
}




// tvArrPlane	= 마킹할 철근
// xyArrDim		= 마킹적정위치를 찾기 위한 CTC배열
// dRatioMarkPos= 철근시점으로부터의 길이비
CDPointArray CADeckData::GetMarkPosArrByLenFrStt(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, double dLenFrStt, BOOL bUpper)
{
	if(tvArrPlane->GetSize()==0)	return NULL;

	// 첫번 주철근은 분리하지 않고 한철근으로 처리하여 2번 철근값을 사용.
	double dNearstMidPoint	= GetPointByOffsetAndCTC(xyArrDim, dLenFrStt);// dLenFrStt 떨어진 위치에서 CTC중간점이 가장 근접한 위치

	CDPointArray xyReturn;
	for(long n=0; n<tvArrPlane->GetSize(); n++)
	{
		CTwinVector tVector = tvArrPlane->GetAt(n);
		double dLength = ~(tVector.m_v1-tVector.m_v2);
		if(n==0)
			dLength = ~(tvArrPlane->GetAt(0).m_v1-tvArrPlane->GetAt(0).m_v2);

		if(dLength >= dNearstMidPoint)
			xyReturn.Add(tVector.m_v1 + tVector.GetXyDir()*dNearstMidPoint);
	}

	return xyReturn;
}



CDPointArray CADeckData::MakePointRebarArr(CDPointArray *parrBase, CDPointArray *parrDot, BOOL bMove, BOOL bUpper)
{
	CDPointArray xyArrRebar;

	if(parrDot->GetSize()==0)	return xyArrRebar;
	CDPoint xyResult;
	CDPoint ptBarPos = parrBase->GetSize() > 0 ? parrBase->GetAt(0) : CDPoint(0,0);	

	if(parrBase->GetSize() > 0)
		xyArrRebar.Add(ptBarPos);		

	for(long nPos=0; nPos<parrDot->GetSize(); nPos++)
	{
		double dNum = Round(parrDot->GetAt(nPos).x,0);
		double dCTC = parrDot->GetAt(nPos).y;

		for(long nC=0; nC<dNum; nC++)
		{				
			ptBarPos.x += dCTC;

			for(long nD=0; nD<parrBase->GetUpperBound(); nD++)
			{
				if(ptBarPos.x >= parrBase->GetAt(nD).x && ptBarPos.x <= parrBase->GetAt(nD+1).x)
				{				
					CDPoint vVec = (parrBase->GetAt(nD+1) - parrBase->GetAt(nD)).Unit();
					if(GetXyMatchSegAndLine(parrBase->GetAt(nD), parrBase->GetAt(nD+1), ptBarPos, CDPoint(0,1), xyResult))
					{
						xyArrRebar.Add(xyResult);		
						break;
					}
				}
			}
		}
	}
	
	ptBarPos = parrBase->GetSize() > 0 ? parrBase->GetAt(parrBase->GetUpperBound()) : CDPoint(0,0);

	if(bMove)
	{
		if(bUpper)
		{
			for (long n=0; n<xyArrRebar.GetSize(); n++)
			{
				xyArrRebar[n].y -= REBAR_SYMBOL_RADIUS;

				if(n==0)
					xyArrRebar[n].x += REBAR_SYMBOL_RADIUS;
				if(n==xyArrRebar.GetSize()-1)
					xyArrRebar[n].x -= REBAR_SYMBOL_RADIUS;
			}
		}
		else
		{
			for (long n=0; n<xyArrRebar.GetSize(); n++)
			{
				xyArrRebar[n].y += REBAR_SYMBOL_RADIUS;

				if(n==0)
					xyArrRebar[n].x += REBAR_SYMBOL_RADIUS;
				if(n==xyArrRebar.GetSize()-1)
					xyArrRebar[n].x -= REBAR_SYMBOL_RADIUS;
			}
		}
	}

	return xyArrRebar;
}

CDPointArray CADeckData::GetArrMarkPosPlanByName(long nJigan, CString szName)
{
	CDPointArray xyArrRebar;
	for(long n=0; n<(long)m_arrMarkPosPlan[nJigan].size(); n++)
	{
		if(szName == m_arrMarkPosPlan[nJigan].at(n).first)
			xyArrRebar = m_arrMarkPosPlan[nJigan].at(n).second;
	}
	return xyArrRebar;
}


// bSttMainRebar
// 사교가 아니어도 곡선교에서는 신축이음부와 유간이 오프셋이기 때문에 좌우측 스테이션이 상이함.
// nPosType =-1 : 슬래브좌측 스테이션
// nPosType = 0 : 슬래브시점-좌우측중 큰 스테이션, 슬래브종점-좌우측중 작은 스테이션
// nPosType = 1 : 슬래브우측 스테이션
// nPosType = 2 : 선형
double CADeckData::GetStationMainRebarPos(BOOL bSttMainRebar, long nPosType) const
{
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);

	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	double dDisLEnd		  = m_pDB->GetDisSlabLeftAct(dStaSlabEndCen)+m_dCoverSd;
	double dDisREnd		  = m_pDB->GetDisSlabRighAct(dStaSlabEndCen)-m_dCoverSd;
	CDPoint	vAngStt		  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd		  = pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndLeft= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dStaSameRange  = 100;	// 슬래브끝단 좌우측은 사교가 아니어도 곡교에서는 스테이션이 다르다.(이유 : 유간은 오프셋됨)
	double dStaStt = 0;
	double dStaEnd = 0;
	if(nPosType==0)
	{
		dStaStt = max(dStaSlabSttLeft, dStaSlabSttRigh);
		dStaEnd = min(dStaSlabEndLeft, dStaSlabEndRigh);
	}
	else if(nPosType==-1)
	{
		if(fabs(dStaSlabSttLeft-dStaSlabSttRigh) > dStaSameRange)
			dStaStt = max(dStaSlabSttLeft, dStaSlabSttRigh);
		else
			dStaStt = dStaSlabSttLeft;

		if(fabs(dStaSlabEndLeft-dStaSlabEndRigh) > dStaSameRange)
			dStaEnd = min(dStaSlabEndLeft, dStaSlabEndRigh);
		else
			dStaEnd = dStaSlabEndLeft;
	}
	else if(nPosType==1)
	{
		if(fabs(dStaSlabSttLeft-dStaSlabSttRigh) > dStaSameRange)
			dStaStt = max(dStaSlabSttLeft, dStaSlabSttRigh);
		else
			dStaStt = dStaSlabSttRigh;

		if(fabs(dStaSlabEndLeft-dStaSlabEndRigh) > dStaSameRange)
			dStaEnd = min(dStaSlabEndLeft, dStaSlabEndRigh);
		else
			dStaEnd = dStaSlabEndRigh;
	}
	else if(nPosType==2)
	{
		dStaStt = dStaSlabSttCen;
		dStaEnd = dStaSlabEndCen;
	}
	return bSttMainRebar ? dStaStt : dStaEnd;
}

// xyArrCTCParent : 기준이 되는 철근간격
// xyArrCTCChild  : 재생성될 철근간격
// dOffsetStt : xyArrCTCParent[0]에서 xyArrCTCChild[0]까지의 거리(항상 양수)
// dOffsetEnd : xyArrCTCParent[last]에서 xyArrCTCChild[last]까지의 거리(항상 양수)
// REBAR_MIN_SPACE 를 기준으로 생성함.
// 주요사용부는 주철근상부 간격을 하부간격으로 결정시 사용함.
void CADeckData::CalcCTCChildByCTCParent(CDPointArray &xyArrCTCParent, CDPointArray &xyArrCTCChild, double dOffsetStt, double dOffsetEnd)
{
	CDoubleArray arrParentRebar;	// 기준이 되는 철근 위치(시작점이 0)
	CDoubleArray arrChildRebar;		// 재생성될 철근위치
	CDPoint xyDim;

	double	dSumPos = 0;
	long	n		= 0;
	arrParentRebar.Add(0);
	for(n=0; n < xyArrCTCParent.GetSize(); n++)
	{
		xyDim = xyArrCTCParent[n];
		for(long nn=0; nn < xyDim.x; nn++)
		{
			dSumPos += xyDim.y;
			arrParentRebar.Add(dSumPos);
		}
	}

	double dSttPos = dOffsetStt;				// 절대위치(시작)A
	double dEndPos = dSumPos - dOffsetEnd;		// 절대위치(종료)

	double dFirstSpace = 0;		// 초기 이격거리
	BOOL bAdd = FALSE;
	long nParentSize = arrParentRebar.GetSize();
	for(n=0; n < nParentSize; n++)
	{
		if(dSttPos < arrParentRebar[n] && bAdd==FALSE)
		{
			arrChildRebar.Add(dSttPos);
			if(arrParentRebar[n]-dSttPos > REBAR_MIN_SPACE)
			{
				arrChildRebar.Add(arrParentRebar[n]);
				if(n < nParentSize-1)
					arrChildRebar.Add(arrParentRebar[n+1]);
			}
			else
			{
				if(n < nParentSize-1)
					dFirstSpace = (arrParentRebar[n+1]-dSttPos)/2;
				else
					dFirstSpace = (arrParentRebar[n]-dSttPos)/2;

				arrChildRebar.Add(dSttPos+dFirstSpace);
				arrChildRebar.Add(dSttPos+dFirstSpace*2);
			}
			bAdd = TRUE;
			n += 2;	
		}
		if(n < nParentSize)
		{
			if(dEndPos < arrParentRebar[n])
			{
				if(arrParentRebar[n-1] - dEndPos > REBAR_MIN_SPACE)
				{
					arrChildRebar.RemoveAt(arrChildRebar.GetUpperBound());
					arrChildRebar.Add(dEndPos);
					arrChildRebar.Add(arrParentRebar[n-1]);
					arrChildRebar.Add(arrParentRebar[n-2]);
				}
				else
				{
					dFirstSpace = (dEndPos - arrParentRebar[n-2])/2;

					arrChildRebar.RemoveAt(arrChildRebar.GetUpperBound());
					arrChildRebar.Add(dEndPos);
					arrChildRebar.Add(dEndPos-dFirstSpace);
					arrChildRebar.Add(dEndPos-dFirstSpace*2);
				}
				break;
			}
		}
		
		if(bAdd && n < nParentSize)
			arrChildRebar.Add(arrParentRebar[n]);

	}
	arrChildRebar.Sort();

	CDPointArray xyArrTemp;		// arrChildRebar를 CDPointArray로 변환
	
	for(n=0; n<arrChildRebar.GetSize(); n++)
	{
		CDPoint xyDim = CDPoint(arrChildRebar[n], 0);
		xyArrTemp.Add(CDPoint(arrChildRebar[n], 0));
	}

	CRebarPlacing RebarPlacing;
	RebarPlacing.RemoveSamePoint(xyArrTemp);	// 중복위치 제거
	RebarPlacing.ConvertPointArrayToOffsetInfo(xyArrTemp, xyArrCTCChild, 0);

}


double CADeckData::GetSideBarDistFromCenterGir(long nSection, BOOL bLeft)
{
	double dSta = GetStationBySection(nSection);

	double dDisL = m_pDB->GetDisSlabLeftAct(dSta) + m_dCoverSd;
	double dDisR = m_pDB->GetDisSlabRighAct(dSta) - m_dCoverSd;

	if(bLeft)	return dDisL;
	else		return dDisR;
}


// 시,종점 및 선형곡률변경위치의 중앙 스테이션
// 사교가 아니어도 곡선교에서는 신축이음부와 유간이 오프셋이기 때문에 좌우측 스테이션이 상이함.
// nPosType =-1 : 슬래브좌측 스테이션
// nPosType = 0 : 슬래브시점-좌우측중 큰 스테이션, 슬래브종점-좌우측중 작은 스테이션
// nPosType = 1 : 슬래브우측 스테이션
CDoubleArray CADeckData::GetStationArrForMainRebarGugan(long nPosType)
{
	double dStaStt  = GetStationMainRebarPos(TRUE, nPosType);
	double dStaEnd  = GetStationMainRebarPos(FALSE, nPosType);

	CDoubleArray dArrSta;
	dArrSta.Add(dStaStt);

	CDoubleArray arrSta = m_pDB->m_pLine->GetStationArrAtChgCurve(dStaStt, dStaEnd);

	for(long nSta=0; nSta<arrSta.GetSize(); nSta++)
		dArrSta.Add(arrSta[nSta]);
	
	dArrSta.Add(dStaEnd);

	return dArrSta;
}


// 사각부 주철근의 끝부분에서 피복까지의 거리(치수선그릴때 사용함.)
double CADeckData::GetLengthMainRebarToCover(BOOL bStt, long nJongBasePos, BOOL bUpper, double &dLengthMainRebarAtSkew, BOOL bNotBlock )
{
	CPlateBridgeApp      *pDB	   = m_pDB;
	CLineInfo		*pLine     = pDB->m_pLine;
	CPlateGirderApp		*pGirCen   = m_pDB->GetGirder(-1);

	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	double dDisCStt		  = (dDisLStt+dDisRStt)/2;
	double dDisLEnd		  = m_pDB->GetDisSlabLeftAct(dStaSlabEndCen)+m_dCoverSd;
	double dDisREnd		  = m_pDB->GetDisSlabRighAct(dStaSlabEndCen)-m_dCoverSd;
	double dDisCEnd		  = (dDisLEnd+dDisREnd)/2;
	CDPoint	vAngStt		  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd		  = pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabSttCent= (dStaSlabSttLeft+dStaSlabSttRigh)/2;
	double dStaSlabEndLeft= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dStaSlabEndCent= (dStaSlabEndLeft+dStaSlabEndRigh)/2;
	double dSttBlockOutWidth = GetWidthBlockOut(TRUE, nJongBasePos);
	double dEndBlockOutWidth = GetWidthBlockOut(FALSE, nJongBasePos);
	if(m_bInstallMainRebarAtBlockOut || bUpper==FALSE)
	{
		if(bNotBlock == FALSE)
		{
			dSttBlockOutWidth  = 0;
			dEndBlockOutWidth  = 0;
		}		
	}

	double dStaMainRebarLeft = 0;
	double dStaMainRebarRigh = 0;
	double dReturn = 0;
	double dLengthMainToCover = 0;
	double dStaSameRange = 20;	// 슬래브끝단 좌우측은 사교가 아니어도 곡교에서는 스테이션이 다르다.(이유 : 유간은 오프셋됨)
	double dCoverSd      = GetLengthCoverSd(bStt); 
	
	
	if(bStt)
	{
		if(fabs(dStaSlabSttLeft-dStaSlabSttRigh) < dStaSameRange || dStaSlabSttLeft > dStaSlabSttRigh)
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT || fabs(dStaSlabSttLeft-dStaSlabSttRigh) < dStaSameRange)
			{
				dReturn = 0;
				dLengthMainRebarAtSkew = 0;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
			{
				dStaMainRebarLeft = pLine->GetStationByDisLength(dStaSlabSttLeft, dSttBlockOutWidth+dCoverSd, dDisLStt);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabSttRigh, dStaMainRebarLeft, dDisRStt)-dSttBlockOutWidth-dCoverSd;
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_STT]);
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
			{
				dStaMainRebarLeft = pLine->GetStationByDisLength(dStaSlabSttLeft, dSttBlockOutWidth+dCoverSd, dDisLStt);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabSttCent, dStaMainRebarLeft, dDisCStt)-dSttBlockOutWidth-dCoverSd;
//				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_STT]);
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkewJong[bUpper?CTC_SKEWJONG_UPPER_CENTER_STT:CTC_SKEWJONG_LOWER_CENTER_STT]);
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
		}
		else
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT)
			{
				dStaMainRebarRigh = pLine->GetStationByDisLength(dStaSlabSttRigh, dSttBlockOutWidth+dCoverSd, dDisRStt);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabSttLeft, dStaMainRebarRigh, dDisLStt)-dSttBlockOutWidth-dCoverSd;
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_STT]);
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
			{
				dReturn = 0;
				dLengthMainRebarAtSkew = 0;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
			{
				dStaMainRebarRigh = pLine->GetStationByDisLength(dStaSlabSttRigh, dSttBlockOutWidth+dCoverSd, dDisRStt);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabSttCent, dStaMainRebarRigh, dDisCStt)-dSttBlockOutWidth-dCoverSd;
//				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_CENTER_STT:CTC_SKEWMAIN_LOWER_CENTER_STT]);
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkewJong[bUpper?CTC_SKEWJONG_UPPER_CENTER_STT:CTC_SKEWJONG_LOWER_CENTER_STT]);
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
		}
	}
	else
	{
		if(fabs(dStaSlabEndRigh-dStaSlabEndLeft) < dStaSameRange || dStaSlabEndRigh > dStaSlabEndLeft)
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT || fabs(dStaSlabEndRigh-dStaSlabEndLeft) < dStaSameRange)
			{
				dReturn = 0;
				dLengthMainRebarAtSkew = 0;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
			{
				dStaMainRebarLeft = pLine->GetStationByDisLength(dStaSlabEndLeft, -dEndBlockOutWidth-dCoverSd, dDisLEnd);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabEndRigh, dStaMainRebarLeft, dDisREnd)-dEndBlockOutWidth-dCoverSd;
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_RIGH_END:CTC_SKEWMAIN_LOWER_RIGH_END]); 
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
			{
				dStaMainRebarLeft = pLine->GetStationByDisLength(dStaSlabEndLeft, -dEndBlockOutWidth-dCoverSd, dDisLEnd);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabEndCent, dStaMainRebarLeft, dDisCEnd)-dEndBlockOutWidth-dCoverSd;
//				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_CENTER_END:CTC_SKEWMAIN_LOWER_CENTER_END]); 
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkewJong[bUpper?CTC_SKEWJONG_UPPER_CENTER_END:CTC_SKEWJONG_LOWER_CENTER_END]); 
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
		}
		else
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT)
			{
				dStaMainRebarRigh = pLine->GetStationByDisLength(dStaSlabEndRigh, -dEndBlockOutWidth-dCoverSd, dDisRStt);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabEndLeft, dStaMainRebarRigh, dDisLEnd)-dEndBlockOutWidth-dCoverSd;
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_LEFT_END:CTC_SKEWMAIN_LOWER_LEFT_END]);
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
			{
				dReturn = 0;
				dLengthMainRebarAtSkew = 0;
			}
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
			{
				dStaMainRebarRigh = pLine->GetStationByDisLength(dStaSlabEndRigh, -dEndBlockOutWidth-dCoverSd, dDisRStt);
				dLengthMainToCover= pGirCen->GetLengthDimType(dStaSlabEndCent, dStaMainRebarRigh, dDisCEnd)-dEndBlockOutWidth-dCoverSd;
//				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_CENTER_END:CTC_SKEWMAIN_LOWER_CENTER_END]);
				dLengthMainRebarAtSkew = GetLengthByCTCAndRebarsu(m_arrCTCMainRebarAtSkewJong[bUpper?CTC_SKEWJONG_UPPER_CENTER_END:CTC_SKEWJONG_LOWER_CENTER_END]);
				dReturn = dLengthMainToCover - dLengthMainRebarAtSkew;
			}
		}

	}

	return fabs(dReturn);
}

double CADeckData::GetLengthByCTCAndRebarsu(CDPointArray xyArrDim)
{
	double dSum = 0;
	for(long n=0; n<xyArrDim.GetSize(); n++)
	{
		dSum += (xyArrDim[n].x*xyArrDim[n].y);
	}
	
	return dSum;
}

BOOL CADeckData::IsBridgeHasStringer()
{
	CPlateBridgeApp      *pDB	   = m_pDB;

	if(IsBridgeHasCrossBeam() == FALSE)		return FALSE;

	for(long nG=0; nG< pDB->GetGirdersu()-1; nG++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(nG));
		CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CPlateCrossBeam *pC = pBx->GetCrossBeam();
			if(pC && pC->m_nQtyStringer_CR > 0)
				return TRUE;
			pBx = Finder.GetBxNext();
		}
	}

	return FALSE;
}

BOOL CADeckData::IsSameCycleDanGen()
{
	BOOL bNoBrk   = !IsBridgeHasOutBracket();
	BOOL bNoHunch = IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&m_pDB->GetGirdersu()==1) ? TRUE : FALSE;
	BOOL bTaperDiv= m_pDB->m_dHeightTaperStt>0 || (m_pDB->m_dHeightTaperStt==0&&!IsSameMainRebarDanGen(FALSE)) ? TRUE : FALSE;

	if(bNoHunch && bTaperDiv==FALSE)
		return TRUE;
	else
		return FALSE;
}


BOOL CADeckData::IsSameMainRebarShapesuDanGen()
{
	BOOL bSame = TRUE;
	long nCyclesuDan = GetRebarCycleSuBySection(SECTION_DAN);
	long nCyclesuCen = GetRebarCycleSuBySection(SECTION_CEN);
	long nCyclesu = min(nCyclesuDan, nCyclesuCen);
	for (long nCycle=0; nCycle<nCyclesu; nCycle++)
	{
		long nShapesuDan = GetRebarShapeArrBySection(SECTION_DAN, nCycle)->size();
		long nShapesuCen = GetRebarShapeArrBySection(SECTION_CEN, nCycle)->size();
		if(nShapesuDan!=nShapesuCen)
			bSame = FALSE;
	}

	return bSame;
}



BOOL CADeckData::IsBridgeHasCrossBeamHunch()
{
	BOOL bHunch = FALSE;

	CPlateBridgeApp *pDB = m_pDB;
	if(pDB->m_bApplyHunchAtCB==FALSE)	return FALSE;

	for (long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec->IsenDetType(SECDET_CROSSBEAM))
			{	
				CPlateCrossBeam *pC = pSec->GetCrossBeam();
				if(pC->m_dLH1==0 || pC->m_dRH1==0)
					bHunch = TRUE;
			}

			pBx = Finder.GetBxNext();
		}
	}

	return bHunch;
}



BOOL CADeckData::IsBridgeHasCrossBeam()
{
	CPlateBridgeApp      *pDB	   = m_pDB;

	if(pDB->GetGirdersu() > 1)	
		return TRUE;
	else
		return FALSE;
}


BOOL CADeckData::IsBridgeHasCurveLine()
{
	CPlateBridgeApp      *pDB	   = m_pDB;
	return (pDB->GetRadiusMinTotalBridge()!=0);
}

BOOL CADeckData::IsSkew()
{
	CPlateGirderApp	*pGirCen  = m_pDB->GetGirder(-1);
	CDPoint		vAngStt	  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint		vAngEnd	  = pGirCen->GetAAA(OFF_END_SLAB);

	if(IsSkewStt() || IsSkewEnd())
		return TRUE;
	else
		return FALSE;
}

BOOL CADeckData::IsSkewStt()
{
	CPlateGirderApp	*pGirCen  = m_pDB->GetGirder(-1);
	CDPoint		vAngStt	  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint		vAngEnd	  = pGirCen->GetAAA(OFF_END_SLAB);

	if(fabs(vAngStt.GetAngleDegree()-90) < 1)
		return FALSE;
	else
		return TRUE;

}


BOOL CADeckData::IsSkewEnd()
{
	CPlateGirderApp	*pGirCen  = m_pDB->GetGirder(-1);
	CDPoint		vAngStt	  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint		vAngEnd	  = pGirCen->GetAAA(OFF_END_SLAB);

	if(fabs(vAngEnd.GetAngleDegree()-90) < 1)
		return FALSE;
	else
		return TRUE;

}


BOOL CADeckData::IsBridgeHasOutBracket(long nSection, BOOL bLeft)
{
/*	CPlateBasicIndex *pBxL	= GetBxBySection(nSection, 0);
	CPlateBasicIndex *pBxR	= GetBxBySection(nSection, m_pDB->GetGirdersu()-1);
	CBracket	*pBrkL	= pBxL==NULL ? NULL : pBxL->GetBracket(TRUE); 
	CBracket	*pBrkR	= pBxR==NULL ? NULL : pBxR->GetBracket(FALSE); 
	if(pBrkL && bLeft)
		return TRUE;
	else if(pBrkR && bLeft==FALSE)
		return TRUE;
*/
	return FALSE;
}

BOOL CADeckData::IsBridgeHasOutBracket()
{
/*	CPlateBasicIndex *pBxL	= GetBxBySection(SECTION_DAN, 0);
	CPlateBasicIndex *pBxR	= GetBxBySection(SECTION_DAN, m_pDB->GetGirdersu()-1);
	CBracket	*pBrkL	= pBxL==NULL ? NULL : pBxL->GetBracket(TRUE); 
	CBracket	*pBrkR	= pBxR==NULL ? NULL : pBxR->GetBracket(FALSE); 
	if(pBrkL || pBrkR)
		return TRUE;
	else */
		return FALSE;
}


BOOL CADeckData::IsBridgeHasSlabHunch()
{
	return m_pDB->m_dThickSlabHunch>0 ? TRUE : FALSE;
}



CDPointArray CADeckData::GetXyArrGuideLineForBeryukRebarOnCB(long nG, BOOL bUpper)
{
	CPlateGirderApp *pGir = m_pDB->GetGirder(nG);

	CDPointArray xyArr;
	CDPointArray xyArrEnd = GetXyArrSlabEnd(nG);
	
	double dExpWStt = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE, JONGBASEPOS_ONCB);
	double dExpWEnd = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(FALSE, JONGBASEPOS_ONCB);
	double dCoverSdStt = GetLengthCoverSd(TRUE) ;
	double dCoverSdEnd = GetLengthCoverSd(FALSE);
	CDPoint vX(1,0), vY(0,1);

	if(bUpper)
	{
		xyArr.Add(xyArrEnd[0] + vX*(dExpWStt+dCoverSdStt) - vY*m_dCoverUp);
		xyArr.Add(xyArrEnd[1] - vX*(dExpWEnd+dCoverSdEnd) - vY*m_dCoverUp);
	}
	else
	{
		xyArr.Add(xyArrEnd[3] + vX*dCoverSdStt + vY*m_dCoverDn);
		xyArr.Add(xyArrEnd[2] - vX*dCoverSdEnd + vY*m_dCoverDn);

		//////////////////////////////////////////////////////////////////////////
		// 테이퍼 시/종점부 
		double dDist = (pGir->m_dGirderCenter+pGir->m_pRight->m_dGirderCenter)/2;
		double dLengthSlab = GetLengthSlabTotalByDist(dDist);
		double dSlabT	   = m_pDB->m_dThickSlabBase;
		double dLSlabToTaperStt = GetLengthSlabToTaperByDist(TRUE, dDist);
		double dLSlabToTaperEnd = GetLengthSlabToTaperByDist(FALSE, dDist);
		double dHunchT     = m_pDB->m_dThickSlabHunch; 
		CDPoint xyPoint = vX*dLSlabToTaperStt - vY*(dSlabT+m_pDB->m_dHeightTaperStt-m_dCoverDn);
		// 시점
		xyArr.Add(xyPoint);
		if(m_pDB->m_cSlabThickAddTypeStt==1)	xyPoint += vX*m_pDB->m_dHeightTaperStt*5 + vY*m_pDB->m_dHeightTaperStt;
		else									xyPoint += vX*(m_pDB->m_dHeightTaperStt+dHunchT)*5 + vY*(m_pDB->m_dHeightTaperStt+dHunchT);
		xyArr.Add(xyPoint);
		if(m_pDB->m_cSlabThickAddTypeStt==1)
		{
			CDPointArray xyArrCBHunch;
			CPlateBxFinder Finder(m_pDB->GetGirder(nG));
			CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			while(pBx)
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if (pC->GetSelfCount()==1)
				{
					xyArrCBHunch = GetXyArrSlabHunch(nG, pBx);
					break;
				}
				pBx = Finder.GetBxNext();
			}
			xyArr.Add(xyArrCBHunch[0]+vY*m_dCoverDn);
			xyArr.Add(xyArrCBHunch[1]+vY*m_dCoverDn);
		}
		// 종점
		xyPoint = vX*(dLengthSlab-dLSlabToTaperEnd) - vY*(dSlabT+m_pDB->m_dHeightTaperEnd-m_dCoverDn);
		xyArr.Add(xyPoint);
		if(m_pDB->m_cSlabThickAddTypeEnd==1)	xyPoint += -vX*m_pDB->m_dHeightTaperEnd*5 + vY*m_pDB->m_dHeightTaperEnd;
		else									xyPoint += -vX*(m_pDB->m_dHeightTaperEnd+dHunchT)*5 + vY*(m_pDB->m_dHeightTaperEnd+dHunchT);
		xyArr.Add(xyPoint);
		if(m_pDB->m_cSlabThickAddTypeEnd==1)
		{
			CDPointArray xyArrCBHunch;
			CPlateBxFinder Finder(m_pDB->GetGirder(nG));
			CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			while(pBx)
			{
				CPlateCrossBeam *pC = pBx->GetCrossBeam();
				if (pC->GetSelfCount()==m_pDB->GetGirder(nG)->GetQtyCrossSectionTotal()-2)
				{
					xyArrCBHunch = GetXyArrSlabHunch(nG, pBx);
					break;
				}
				pBx = Finder.GetBxNext();
			}
			xyArr.Add(xyArrCBHunch[0]+vY*m_dCoverDn);
			xyArr.Add(xyArrCBHunch[1]+vY*m_dCoverDn);
		}
	}

	xyArr.Sort();

	return xyArr;
}


CDPointArray CADeckData::GetXyArrGuideLineForBeryukRebar(long nJongBasePos, BOOL bUpper)
{
	if(nJongBasePos==JONGBASEPOS_ONCB)
		return GetXyArrGuideLineForBeryukRebarOnCB(0, bUpper);


	CDPointArray xyArrReturn;
	CDPointArray xySlabJong = GetShapeSlabJongBasePos(nJongBasePos);
	
	double dExpWStt = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE, nJongBasePos);
	double dExpWEnd = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(FALSE, nJongBasePos);
	double dCoverSdStt = GetLengthCoverSd(TRUE) ;
	double dCoverSdEnd = GetLengthCoverSd(FALSE);
	//////////////////////////////////////////////////////////////////////////
	//
	CDPoint vX(1,0), vY(0,1);
	
	if(bUpper)
	{
		xyArrReturn.Add(xySlabJong[0] + vX*(dExpWStt+dCoverSdStt) - vY*m_dCoverUp);
		xyArrReturn.Add(xySlabJong[1] - vX*(dExpWEnd+dCoverSdEnd) - vY*m_dCoverUp);
	}
	else
	{
		xyArrReturn.Add(xySlabJong[7] + vX*dCoverSdStt+vY*m_dCoverDn);
		xyArrReturn.Add(xySlabJong[6] + vY*m_dCoverDn);
		xyArrReturn.Add(xySlabJong[5] + vY*m_dCoverDn);
		xyArrReturn.Add(xySlabJong[4] + vY*m_dCoverDn);
		xyArrReturn.Add(xySlabJong[3] + vY*m_dCoverDn);
		xyArrReturn.Add(xySlabJong[2] - vX*dCoverSdEnd+vY*m_dCoverDn);
	}
	
	return xyArrReturn;
}



CDPointArray CADeckData::GetXyArrGuideLineForMainRebar(long nSection, BOOL bUpper)
{
	CPlateBridgeApp	*pDB		= m_pDB;
	CLineInfo		*pLine		= pDB->m_pLine;
	double			dSta		= GetStationBySection(nSection);
	long			nQtyHdan	= pDB->GetQtyHDanAct(dSta); 
	
	CDPointArray xyArrReturn;
	
	if(bUpper)
	{
		long n = 0;

		CDPoint A[HDANLINESU]; // 포장상단의 포인트
		pDB->GetXySlabAct(dSta, A);	
		
		A[0].x += m_dCoverSd;			// 최좌측
		A[nQtyHdan].x -= m_dCoverSd;	// 최우측
		for(n = 0; n < nQtyHdan+1; n++)				
			A[n].y -= m_dCoverUp;	
		
		for(n=0; n < nQtyHdan+1; n++)
			xyArrReturn.Add(A[n]);
	}
	else
	{
		// 좌측부 배열 등록
		CPlateGirderApp  *pGir	  = pDB->GetGirder(0);
		CPlateBasicIndex *pBxMatch = GetBxBySection(nSection, 0);

		CDPoint A[10], C[4], vX(1,0), vY(0,1), xyResult;
		pDB->GetXyFrontCantilever(pBxMatch, C, TRUE);

		xyArrReturn.Add(C[0]+vX*m_dCoverSd+vY*m_dCoverDn);
		xyArrReturn.Add(C[1]+vY*m_dCoverDn);
		xyArrReturn.Add(C[2]+vY*m_dCoverDn);

		// 중앙부 배열등록
		CPlateBasicIndex *pBxSec = NULL;		
		for(long g = 0;g < pDB->GetGirdersu()-1;g++)
		{
			pGir   = pDB->GetGirder(g);
			pBxSec = GetBxBySection(nSection, g);
			pGir->GetHunchXyDom(pBxSec, A, FALSE, FALSE, pDB->IsTUGir());

			for(long n=0; n<10; n++)		
			{
				if(pDB->IsTUGir())
				{
					if(n%3!=0)	continue;	// 0,3,6,9
					if(g>0 && n<5) continue;
					xyArrReturn.Add(CDPoint(A[n].x, A[n].y+m_dCoverDn));
				}
				else
					xyArrReturn.Add(CDPoint(A[n].x, A[n].y+m_dCoverDn));
			}
		}
		
		// 우측부 배열등록
		pGir	 = pDB->GetGirder(pDB->GetGirdersu()-1);
		pBxMatch = GetBxBySection(nSection, pDB->GetGirdersu()-1);
		pDB->GetXyFrontCantilever(pBxMatch,C, FALSE);

		xyArrReturn.Add(C[2]+vY*m_dCoverDn);
		xyArrReturn.Add(C[1]+vY*m_dCoverDn);		
		xyArrReturn.Add(C[0]-vX*m_dCoverSd+vY*m_dCoverDn);
	}

//	EraseSameVector(xyArrReturn);

	return xyArrReturn;
}



// 주철근의 평면위치 배열을 구함
// bUpper : 상부, 하부
// nDrawSide : DRAW_LEFTSIDE  = 슬래브 중심기준 좌측
// nDrawSide : DRAW_RIGHTSIDE = 슬래브 중심기준 우측
// nDrawSide : DRAW_SIDEALL   = 슬래브 좌,우측 모두
// dCheckLength
// bDrawAlways: Trim을 적용안하고 항상 그리는지 ? 
// 슬래브의 양끝단이 시점, 슬래브 중심이 종점
vector<REBAR_PLAN>* CADeckData::GetMainRebarArr(BOOL bUpper, long nDrawSide)
{
	if(nDrawSide==DRAW_SIDEALL)			return &m_vPlanMainRebar[bUpper?0:1];
	else if(nDrawSide==DRAW_LEFTSIDE)	return &m_vPlanMainRebarLeft[bUpper?0:1];
	else if(nDrawSide==DRAW_RIGHTSIDE)	return &m_vPlanMainRebarRigh[bUpper?0:1];

	return NULL;
}


// 사보강부 주철근 배열 구하지
// 슬래브의 양끝단이 시점, 슬래브 중심이 종점
vector<REBAR_PLAN>* CADeckData::GetMainRebarArrAtSkew(BOOL bStt, BOOL bUpper, long nDrawSide)
{
	if(nDrawSide==DRAW_SIDEALL)			return bStt ? &m_vPlanMainRebarSkewStt[bUpper?0:1] : &m_vPlanMainRebarSkewEnd[bUpper?0:1];
	else if(nDrawSide==DRAW_LEFTSIDE)	return bStt ? &m_vPlanMainRebarSkewLeftStt[bUpper?0:1] : &m_vPlanMainRebarSkewLeftEnd[bUpper?0:1];
	else if(nDrawSide==DRAW_RIGHTSIDE)	return bStt ? &m_vPlanMainRebarSkewRighStt[bUpper?0:1] : &m_vPlanMainRebarSkewRighEnd[bUpper?0:1];

	return NULL;
}


// 사보강철근 배열 구하기
// 슬래브의 양끝단이 시점, 슬래브 중심이 종점
vector<REBAR_PLAN>* CADeckData::GetSabogangArr(BOOL bStt, BOOL bUpper, long nDrawSide)
{
	if(nDrawSide==DRAW_SIDEALL)			return bStt ? &m_vPlanSabogangStt[bUpper?0:1] : &m_vPlanSabogangEnd[bUpper?0:1];
	else if(nDrawSide==DRAW_LEFTSIDE)	return bStt ? &m_vPlanSabogangLeftStt[bUpper?0:1] : &m_vPlanSabogangLeftEnd[bUpper?0:1];
	else if(nDrawSide==DRAW_RIGHTSIDE)	return bStt ? &m_vPlanSabogangRighStt[bUpper?0:1] : &m_vPlanSabogangRighEnd[bUpper?0:1];

	return NULL;
}


// 신축이음부 하단 주철근 배열 구하기
vector<REBAR_PLAN>* CADeckData::GetMainRebarArrUnderSinChuk(BOOL bStt, long nDrawSide) 
{
	if(nDrawSide==DRAW_SIDEALL)			return bStt ? &m_vPlanUnderSinchukStt : &m_vPlanUnderSinchukEnd;
	else if(nDrawSide==DRAW_LEFTSIDE)	return bStt ? &m_vPlanUnderSinchukLeftStt : &m_vPlanUnderSinchukLeftEnd;
	else if(nDrawSide==DRAW_RIGHTSIDE)	return bStt ? &m_vPlanUnderSinchukRighStt : &m_vPlanUnderSinchukRighEnd;

	return NULL;
}



// 가로보 헌치하단 보강 철근 배열(V형)
// pBx = 가로보 Bx
vector<REBAR_PLAN>* CADeckData::GetBoganRebarArrAtCrossBeam(CDoubleArray &dDistArr, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB	   = m_pDB;
	CPlateGirderApp	*pGir	   = pBx->GetGirder();
	
	CPlateCrossBeam *pCr = pBx->GetCrossBeam();
	if(!pCr) 
		return NULL;

	long nCount = 0;
	CDPoint xyDim = GetCTCRebarAtCB(pBx, nCount);

	CDPoint xySttCross = pGir->GetXyPlanSlabCrossbeamBase(pBx, TRUE);
	CDPoint xyEndCross = pGir->GetXyPlanSlabCrossbeamBase(pBx, FALSE);

	double dLDist = pDB->m_pLine->GetDisMatchVerticalByXy(xySttCross);
	double dRDist = pDB->m_pLine->GetDisMatchVerticalByXy(xyEndCross);

	double dDist = (dLDist + dRDist)/2 - (xyDim.x * xyDim.y)/2;			
	for(long n=0; n<=xyDim.x; n++)
	{					
		dDistArr.Add(dDist);			
		dDist += xyDim.y;			
	}	
	
	return NULL;

}


CDPoint CADeckData::GetCTCRebarAtCB(CPlateBasicIndex *pBx, long &nCount)
{
	long nG = pBx->GetNumberGirder();
	long nCrIdx = pBx->GetNumberCrossBeam();

	CDPoint xyDim;

	for (long n=0; n<m_arrCTCBogangRebarAtCrossBeam.GetSize(); n++)
	{
		if(nG==m_arrIndexBogangRebarAtCrossBeam[n].x && nCrIdx==m_arrIndexBogangRebarAtCrossBeam[n].y)
		{
			xyDim = m_arrCTCBogangRebarAtCrossBeam[n];
			nCount = n;
			return xyDim; 
		}
	}

	return xyDim;

}

// x : Station 시작
// y : Station 종료
// z : Dist
// nPos  : CTC_BERYUK_UPPER,  
// nSize => 상 하 좌 우
void CADeckData::GetBeryukRebarDistOnCenter(CVectorArray *pArr, CPlateBasicIndex *pBxSttJijum, CPlateBasicIndex *pBxEndJijum, BOOL bUpper, long nDrawSide)
{
	CPlateBridgeApp	*pDB  = m_pDB;
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateGirderApp	*pLGir	= pDB->GetGirder(0);
	CPlateGirderApp	*pRGir	= pDB->GetGirder(pDB->GetGirdersu()-1);
	CLineInfo   *pLine  = pDB->m_pLine;

	CDPointArray *pArray = GetCTCBeryukRebar(SECTION_CEN, bUpper);
	double LDist = GetSideBarDistFromCenterGir(TRUE, TRUE);
	double RDist = GetSideBarDistFromCenterGir(TRUE, FALSE);
	long   nJStt = pBxSttJijum->GetGirder()->GetNumberJijumByBx(pBxSttJijum);
	long   nJEnd = pBxEndJijum->GetGirder()->GetNumberJijumByBx(pBxEndJijum);
	
	CVectorArray vArrRebarStt, vArrRebarEnd;
	GetBeryukRebarDistOnJijum(&vArrRebarStt, nJStt, bUpper ? 0 : 1, nDrawSide);
	GetBeryukRebarDistOnJijum(&vArrRebarEnd, nJEnd, bUpper ? 0 : 1, nDrawSide);
	
	if(vArrRebarStt.GetSize()==0 || vArrRebarEnd.GetSize()==0 )		return;

	CDoubleArray dArrDist;
	double dDist = LDist;
	dArrDist.Add(dDist);
	long n = 0;
	for(n=0; n<pArray->GetSize(); n++)
	{			
		CDPoint xyDim = pArray->GetAt(n);
		for(long nCount=0; nCount<xyDim.x; nCount++)
		{			
			dDist += xyDim.y;
			
			if	   (nDrawSide==DRAW_LEFTSIDE && dDist > (LDist + RDist) / 2)	continue;
			else if(nDrawSide==DRAW_RIGHTSIDE && dDist < (LDist + RDist) / 2)	continue;

			dArrDist.Add(dDist);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 이음장
	long nPos = 0;	// 좌측
	double dDiaDanbuBeryukUp= m_dDiaBeryukRebar_Dan[nPos];		// 단부 상면 배력철근 직경	
	double dDiaDanbuBeryukLo= m_dDiaBeryukRebarLower_Dan[nPos];	// 단부 하면 배력철근 직경	
	double dDiaGenBeryukUp	= m_dDiaBeryukRebar_Cent[nPos];		// 일반부 상면 배력철근 직경
	double dDiaGenBeryukLo	= m_dDiaBeryukRebarLower_Cent[nPos];	// 일반부 하면 배력철근 직경
	double dDiaJijumBeryukUp= m_dDiaJijumRebarUp[nPos];			// 지점부 상면 배력철근 직경
	double dDiaJijumBeryukLo= m_dDiaJijumRebarDn[nPos];			// 지점부 하면 배력철근 직경

	double dLJointDanUp		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuBeryukUp,	JOINT_TENS_NORMAL_B, toCm(m_dCoverUp), toCm(m_dEndSupportRebarCTC[nPos]));
	double dLJointDanLo		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuBeryukLo,	JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dEndSupportRebarCTCLower[nPos]));
	double dLJointGenUp		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenBeryukUp,	JOINT_TENS_NORMAL_B, toCm(m_dCoverUp), toCm(m_dGenSupportRebarCTC[nPos]));
	double dLJointGenLo		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenBeryukLo,	JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dGenSupportRebarCTCLower[nPos]));
	double dLJointJijumUp	= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaJijumBeryukUp,	JOINT_TENS_NORMAL_B, toCm(m_dCoverUp), toCm(m_dJijumRebarCTCUp[nPos]));
	double dLJointJijumLo	= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaJijumBeryukLo,	JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dJijumRebarCTCDn[nPos]));
	double dLJointDanGenUp	= max(dLJointDanUp, dLJointGenUp);
	double dLJointDanGenLo	= max(dLJointDanLo, dLJointGenLo);
	double dLJointGenJijumUp= max(dLJointJijumUp, dLJointGenUp);
	double dLJointGenJijumLo= max(dLJointJijumLo, dLJointGenLo);
	//////////////////////////////////////////////////////////////////////////
	
	CVector vRebar;

	double dMinDist = 10000000000;
	long   nMin		= 0;
	double dStaStt  = 0;
	double dStaEnd  = 0;
	double dLengthJointStt = 0;
	double dLengthJointEnd = 0;

	if(nJStt==0)
	{
		dLengthJointStt = bUpper ? dLJointDanGenUp : dLJointDanGenLo;
		dLengthJointEnd = bUpper ? dLJointGenJijumUp : dLJointGenJijumLo;
	}
	else if(nJEnd==m_pDB->m_nQtyJigan)
	{
		dLengthJointStt = bUpper ? dLJointGenJijumUp : dLJointGenJijumLo;
		dLengthJointEnd = bUpper ? dLJointDanGenUp : dLJointDanGenLo;
	}
	else
	{
		dLengthJointStt = bUpper ? dLJointGenJijumUp : dLJointGenJijumLo;
		dLengthJointEnd = bUpper ? dLJointGenJijumUp : dLJointGenJijumLo;
	}

	for (long nDist=0; nDist<dArrDist.GetSize(); nDist++)
	{
		double dDistCur = dArrDist[nDist];
		
		nMin = 0;
		dMinDist = 10000000000;
		for (n=0; n<vArrRebarStt.GetSize(); n++)
		{
			 if( fabs(vArrRebarStt[n].z-dDistCur) <= dMinDist)
			{
				dMinDist = fabs(vArrRebarStt[n].z-dDistCur);
				nMin = n;
			}
		}
		dStaStt  = pLine->GetStationByDisLength(vArrRebarStt[nMin].y, -dLengthJointStt, dDistCur);
		vRebar.x = dStaStt;

		nMin = 0;
		dMinDist = 10000000000;

		for ( n=0; n<vArrRebarEnd.GetSize(); n++)
		{
			if( fabs(vArrRebarEnd[n].z-dDistCur) <= dMinDist)
			{
				dMinDist = fabs(vArrRebarEnd[n].z-dDistCur);
				nMin = n;
			}
		}
		dStaEnd  = pLine->GetStationByDisLength(vArrRebarEnd[nMin].x, dLengthJointEnd, dDistCur);
		vRebar.y = dStaEnd;
		vRebar.z = dDistCur;
		pArr->Add(vRebar);
	}	
}




// x : Station 시작
// y : Station 종료
// z : Dist
// nRebarPos:0 상부, nRebarPos:1 하부, nRebarPos:2 신축이음부
void CADeckData::GetBeryukRebarDistOnJijum(CVectorArray *pArr, long nJijum, long nRebarPos, long nDrawSide, long nLRType)
{
	CPlateBridgeApp		*pDB	= m_pDB;
	CPlateGirderApp		*pGirCen= pDB->GetGirder(-1);
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	CPlateBasicIndex		*pBx    = pGirCen->GetBxOnJijum(nJijum); 

	double dLengthL = 0;
	double dLengthR = 0;
	double dLengthLStt = 0;
	double dLengthRStt = 0;
	double dLengthLEnd = 0;
	double dLengthREnd = 0;
	double dStaStt	= 0;
	double dStaEnd	= 0;
	double dSta		= pBx->GetStation();
	double dExpWStt = pGlopt->GetSttExpansionJointWidth();
	double dExpWEnd = pGlopt->GetEndExpansionJointWidth();
	double dCoverSdStt = GetLengthCoverSd(TRUE);
	double dCoverSdEnd = GetLengthCoverSd(FALSE);

	if(m_bInstallMainRebarAtBlockOut || nRebarPos==1 || nRebarPos==2/*하부, 신축이음부 아래*/)
	{
		dExpWStt  = 0;
		dExpWEnd  = 0;
	}

	CDPointArray *pArray = NULL;
	CDoubleArray dArrLeftLength, dArrRighLength;
	CDoubleArray dArrLeftLengthStt, dArrRighLengthStt, dArrLeftLengthEnd, dArrRighLengthEnd;

	long nCycleSu = m_stBeryukCycleLeft.size();
	if(nCycleSu==0)	return;
	long nCycle = 0;
	for (nCycle=0; nCycle<nCycleSu; nCycle++)
	{
		vector<REBAR_SHAPE> stShapeL = m_stBeryukShapeLeft[nCycle];	//좌측으로 함.
		vector<REBAR_SHAPE> stShapeR = m_stBeryukShapeRigh[nCycle];	//좌측으로 함.

		if(nJijum==0)
		{
			if(nRebarPos==0)
			{
				CDPointArray xyArrL = GetXyArrPoint("시점부상부", stShapeL);
				CDPointArray xyArrR = GetXyArrPoint("시점부상부", stShapeR);

				dLengthL = ~(xyArrL[1]-xyArrL[2]);
				dLengthR = ~(xyArrR[1]-xyArrR[2]);
			}
			else if(nRebarPos==1)
			{
				CDPointArray xyArrL = GetXyArrPoint("시점부하부(1)", stShapeL);
				CDPointArray xyArrR = GetXyArrPoint("시점부하부(1)", stShapeR);
				CDPointArray xyArrL2 = GetXyArrPoint("시점부하부(2)", stShapeL);
				CDPointArray xyArrR2 = GetXyArrPoint("시점부하부(2)", stShapeR);

				if(xyArrL.GetSize()==0)
				{
					if(xyArrL2.GetSize()>0)
						dLengthL = fabs(xyArrL2[1].x-xyArrL2[0].x);
					else 
						dLengthL = 0;
				}
				else
				{
					if(xyArrL2.GetSize()>0)
						dLengthL = fabs(xyArrL[2].x-xyArrL2[1].x);
					else 
						dLengthL = fabs(xyArrL[2].x-xyArrL[xyArrL.GetUpperBound()].x);
				}

				if(xyArrR.GetSize()==0)
				{
					if(xyArrR2.GetSize()>0)
						dLengthR = fabs(xyArrR2[1].x-xyArrR2[0].x);
					else 
						dLengthR = 0;
				}
				else
				{
					if(xyArrR2.GetSize()>0)
						dLengthR = fabs(xyArrR[2].x-xyArrR2[1].x);
					else
						dLengthR = fabs(xyArrR[2].x-xyArrR[xyArrR.GetUpperBound()].x);
				}
			}
			else
			{
				CDPointArray xyArrRebar = GetXyArrPoint("시점부하부(1)", stShapeL);

				if(xyArrRebar.GetSize() > 1)
				{
					dLengthL = ~(xyArrRebar[0]-xyArrRebar[1]);
					dLengthR = dLengthL; 	 
				}
				else
				{
					CDPointArray xyArrTemp = GetXyArrPoint("시점부하부(2)", stShapeL);
					dLengthL = ~(xyArrTemp[0]-xyArrTemp[1]);
					dLengthR = dLengthL; 	 
				}
			}
			pArray = GetCTCBeryukRebar(SECTION_DAN, nRebarPos==0 ? TRUE : FALSE);
			dArrLeftLength.Add(dLengthL);
			dArrRighLength.Add(dLengthR);
		}
		else if(nJijum==pDB->m_nQtyJigan)
		{
			if (nRebarPos==0)
			{
				CDPointArray xyArrL = GetXyArrPoint("종점부상부", stShapeL);
				CDPointArray xyArrR = GetXyArrPoint("종점부상부", stShapeR);

				dLengthL = ~(xyArrL[1]-xyArrL[2]);
				dLengthR = ~(xyArrR[1]-xyArrR[2]);
			}
			else if(nRebarPos==1)
			{
				CDPointArray xyArrL = GetXyArrPoint("종점부하부(1)", stShapeL);
				CDPointArray xyArrR = GetXyArrPoint("종점부하부(1)", stShapeR);
				CDPointArray xyArrL2 = GetXyArrPoint("종점부하부(2)", stShapeL);
				CDPointArray xyArrR2 = GetXyArrPoint("종점부하부(2)", stShapeR);
				
				if(xyArrL.GetSize()==0)
				{
					if(xyArrL2.GetSize()>0)
						dLengthL = fabs(xyArrL2[1].x-xyArrL2[0].x);
					else 
						dLengthL = 0;
				}
				else
				{
					if(xyArrL2.GetSize()>0)
						dLengthL = fabs(xyArrL[2].x-xyArrL2[0].x);
					else 
						dLengthL = fabs(xyArrL[2].x-xyArrL[xyArrL.GetUpperBound()].x);
				}


				if(xyArrR.GetSize()==0)
				{
					if(xyArrR2.GetSize()>0)
						dLengthR = fabs(xyArrR2[1].x-xyArrR2[0].x);
					else 
						dLengthR = 0;
				}
				else
				{
					
					if(xyArrR2.GetSize()>0)
						dLengthR = fabs(xyArrR[2].x-xyArrR2[0].x);
					else
						dLengthR = fabs(xyArrR[2].x-xyArrR[xyArrL.GetUpperBound()].x);
				}
			}
			else
			{
				CDPointArray xyArrRebar = GetXyArrPoint("종점부하부(1)", stShapeL);

				if(xyArrRebar.GetSize() > 1)
				{
					dLengthL = ~(xyArrRebar[0]-xyArrRebar[1]);
					dLengthR = dLengthL; 	 
				}
				else
				{
					CDPointArray xyArrTemp = GetXyArrPoint("종점부하부(2)", stShapeL);
					dLengthL = ~(xyArrTemp[0]-xyArrTemp[1]);
					dLengthR = dLengthL; 	 
				}
			}
			pArray = GetCTCBeryukRebar(SECTION_DAN, nRebarPos==0 ? TRUE : FALSE);
			dArrLeftLength.Add(dLengthL);
			dArrRighLength.Add(dLengthR);
		}
		else
		{
			CString szJijum;
		
			if(nRebarPos==0)
			{
				szJijum.Format("지점부상부(%d)",nJijum);
				CDPointArray xyArrL = GetXyArrPoint(szJijum, stShapeL);
				CDPointArray xyArrR = GetXyArrPoint(szJijum, stShapeR);

				dLengthLStt = ~(xyArrL[0]-xyArrL[1]);
				dLengthRStt = ~(xyArrR[0]-xyArrR[1]);
				dLengthLEnd = ~(xyArrL[1]-xyArrL[2]);
				dLengthREnd = ~(xyArrR[1]-xyArrR[2]);
				
				pArray = GetCTCBeryukRebar(SECTION_PIER, nRebarPos==0 ? TRUE : FALSE);		
			}
			else if(nRebarPos==1)
			{
				szJijum.Format("지점부하부(%d)",nJijum);
				CDPointArray xyArrL = GetXyArrPoint(szJijum, stShapeL);
				CDPointArray xyArrR = GetXyArrPoint(szJijum, stShapeR);

				dLengthLStt = ~(xyArrL[0]-xyArrL[1]);
				dLengthRStt = ~(xyArrR[0]-xyArrR[1]);
				dLengthLEnd = ~(xyArrL[1]-xyArrL[2]);
				dLengthREnd = ~(xyArrR[1]-xyArrR[2]);
				
				pArray = GetCTCBeryukRebar(SECTION_PIER, nRebarPos==0 ? TRUE : FALSE);		
			}
			else
			{

			}
			dArrLeftLengthStt.Add(dLengthLStt);
			dArrRighLengthStt.Add(dLengthRStt);
			dArrLeftLengthEnd.Add(dLengthLEnd);
			dArrRighLengthEnd.Add(dLengthREnd);
		}		
	}
	
	double dDistL = GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDistR = GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDistCur = dDistL;
	
	
	// 첫번째 지점 배력 철근(1Cycle) - 슬래브 좌측끝단
	nCycle = 0;
	if(nDrawSide==DRAW_LEFTSIDE || nDrawSide==DRAW_SIDEALL)
	{
		if(nJijum==0)
		{
			dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB, dExpWStt+dCoverSdStt), dDistCur, pGirCen->GetAAA(OFF_STT_SLAB, dExpWStt+dCoverSdStt));
			dStaEnd = pDB->m_pLine->GetStationByDisLength(dStaStt, dArrLeftLength[nCycle], dDistCur);
		}
		else if(nJijum==pDB->m_nQtyJigan)
		{
			dStaEnd = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd), dDistCur, pGirCen->GetAAA(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd));
			dStaStt = pDB->m_pLine->GetStationByDisLength(dStaEnd, -dArrLeftLength[nCycle], dDistCur);
		}
		else
		{
//			dStaStt = pGirCen->GetStationBySc(dSta, dDistCur, pBx->GetAngle()) - ((nLRType==0 || nLRType==-1) ? dArrLeftLengthStt[nCycle] : 0);
//			dStaEnd = pGirCen->GetStationBySc(dSta, dDistCur, pBx->GetAngle()) + ((nLRType==0 || nLRType== 1) ? dArrLeftLengthEnd[nCycle] : 0);
			dStaStt = (nLRType==0 || nLRType==-1) ? pDB->m_pLine->GetStationByDisLength(dSta, -dArrLeftLengthStt[nCycle], dDistCur) : dSta;
			dStaEnd = (nLRType==0 || nLRType== 1) ? pDB->m_pLine->GetStationByDisLength(dSta, dArrLeftLengthEnd[nCycle], dDistCur)	: dSta;
		}
		pArr->Add(CVector(dStaStt, dStaEnd, dDistCur));
	}

	// 다음.... 지점 배력 철근
	double dStaSttLeft  = 0, dStaSttRigh = 0, dStaEndLeft = 0, dStaEndRigh = 0;
	double dWSlabMain	= fabs(dDistL-dDistR);
	double dSumY		= 0;
	long   nCurRebar    = 1;

	for(long n=0; n<pArray->GetSize(); n++)
	{			
		CDPoint xyDim = pArray->GetAt(n);
		for(long nCount=0; nCount<xyDim.x; nCount++)
		{
			long nCurCycle = long(nCurRebar%nCycleSu);
			dDistCur += xyDim.y;
			dSumY    += xyDim.y;

			if	   (nDrawSide==DRAW_LEFTSIDE && dDistCur > (dDistL + dDistR) / 2)	continue;
			else if(nDrawSide==DRAW_RIGHTSIDE && dDistCur < (dDistL + dDistR) / 2)	continue;

			if(nJijum==0)
			{
				dStaSttLeft = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB, dExpWStt+dCoverSdStt), dDistL, pGirCen->GetAAA(OFF_STT_SLAB, dExpWStt+dCoverSdStt));
				dStaSttRigh = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB, dExpWStt+dCoverSdStt), dDistR, pGirCen->GetAAA(OFF_STT_SLAB, dExpWStt+dCoverSdStt));
				dStaEndLeft = pDB->m_pLine->GetStationByDisLength(dStaSttLeft, dArrLeftLength[nCurCycle], dDistL);
				dStaEndRigh = pDB->m_pLine->GetStationByDisLength(dStaSttRigh, dArrRighLength[nCurCycle], dDistR);
				dStaStt = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_STT_SLAB, dExpWStt+dCoverSdStt), dDistCur, pGirCen->GetAAA(OFF_STT_SLAB, dExpWStt+dCoverSdStt));
				dStaEnd = (dStaEndLeft*(dWSlabMain-dSumY) + dStaEndRigh*dSumY) / dWSlabMain; 
			}
			else if(nJijum==pDB->m_nQtyJigan)
			{
				dStaSttLeft = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd), dDistL, pGirCen->GetAAA(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd));
				dStaSttRigh = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd), dDistR, pGirCen->GetAAA(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd));
				dStaEndLeft = pDB->m_pLine->GetStationByDisLength(dStaSttLeft, -dArrLeftLength[nCurCycle], dDistL);
				dStaEndRigh = pDB->m_pLine->GetStationByDisLength(dStaSttRigh, -dArrRighLength[nCurCycle], dDistR);
				dStaStt = (dStaEndLeft*(dWSlabMain-dSumY) + dStaEndRigh*dSumY) / dWSlabMain; 
				dStaEnd = pGirCen->GetStationBySc(pGirCen->GetSSS(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd), dDistCur, pGirCen->GetAAA(OFF_END_SLAB, -dExpWEnd-dCoverSdEnd));
			}
			else
			{
				double dLengthStt = (dArrLeftLengthStt[nCurCycle]*(dWSlabMain-dSumY) + dArrRighLengthStt[nCurCycle]*dSumY) / dWSlabMain; 
				double dLengthEnd = (dArrLeftLengthEnd[nCurCycle]*(dWSlabMain-dSumY) + dArrRighLengthEnd[nCurCycle]*dSumY) / dWSlabMain; 
//				dStaStt = pGirCen->GetStationBySc(dSta,dDistCur,pBx->GetAngle()) - ((nLRType==0 || nLRType==-1) ? dLengthStt : 0);
//				dStaEnd = pGirCen->GetStationBySc(dSta,dDistCur,pBx->GetAngle()) + ((nLRType==0 || nLRType== 1) ? dLengthEnd : 0);
				dStaStt = (nLRType==0 || nLRType==-1) ? pDB->m_pLine->GetStationByDisLength(dSta, -dLengthStt, dDistCur)	: dSta;
				dStaEnd = (nLRType==0 || nLRType== 1) ? pDB->m_pLine->GetStationByDisLength(dSta, dLengthEnd, dDistCur)		: dSta;
			}

			nCurRebar++;
			pArr->Add(CVector(dStaStt, dStaEnd, dDistCur));
		}		
	}	
}


// pt1과 pt2로 이루어진 주철근이 그려지는 철근인가 ?
// TRUE  : 그려짐
// FALSE : Trim됨
BOOL CADeckData::IsDrawMainRebarArea(CDPoint pt1, CDPoint pt2)
{
	CPlateBridgeApp *pDB	= m_pDB;
	
	double dArcVLen = 900;
	double dStaMatch1, dStaMatch2;
	
	m_pDB->m_pLine->GetStationMatchVerticalByXy(pt1, dStaMatch1);
	m_pDB->m_pLine->GetStationMatchVerticalByXy(pt2, dStaMatch2);
	
	double dStaBridgeStt = pDB->m_dStationBridgeStt + pDB->m_dLengthUganStt;
	double dStaBridgeEnd = dStaBridgeStt + pDB->m_dLengthTotalSlab;
	double dStaSttCheck = 0 , dStaEndCheck = 0;
	double dStation = dStaBridgeStt;
	
	if(m_arrTrimRange.GetSize() < 2)	// Trim구간이 전체일 경우
		return TRUE;

	for(long n=0; n<m_arrTrimRange.GetSize(); n++)
	{		
		dStaSttCheck = dStation-dArcVLen;
		dStaEndCheck = (n==m_arrTrimRange.GetUpperBound()) ? 			 
						dStaBridgeEnd+dArcVLen :
						dStation+m_arrTrimRange.GetAt(n)+dArcVLen;
		if(n==0)
		{
			if(dStaMatch1 <= dStaEndCheck && 
			   dStaMatch2 <= dStaEndCheck) return TRUE;
		}
		else if(n==m_arrTrimRange.GetUpperBound())
		{
			if(dStaMatch1 >= dStaSttCheck && 
			   dStaMatch2 >= dStaSttCheck) return TRUE;
		}
		else if(n%2==0 &&
		   dStaMatch1 >= dStaSttCheck && 
		   dStaMatch2 >= dStaSttCheck && 
		   dStaMatch1 <= dStaEndCheck && 
		   dStaMatch2 <= dStaEndCheck) return TRUE;
		dStation += m_arrTrimRange.GetAt(n);					
	}			
	
	return FALSE;
}

double CADeckData::GetLengthCrossBeamHunchRebar(CPlateBasicIndex *pBx, BOOL bUpper)
{
	CPlateCrossBeam *pCross = pBx->GetCrossBeam();
	if(!pCross) return 0;		
	
	if(!bUpper)
	{	
		double dWHalf = pCross->m_uM_CR + m_pDB->m_dWidthHunch - m_dCoverSd;
		return (dWHalf + (m_pDB->m_dHunchDisForBaseThick + m_pDB->m_dThickSlabBase - m_dCoverUp - m_dCoverDn)) * 3 * 2/*2배*/;		
	}

	return m_dLengthCrossBeamBogang;
}

double CADeckData::GetLengthStringerHunchRebar(CPlateBasicIndex *pBx, long nStg, BOOL bUpper) const
{
/*	CPlateGirderApp *pGir = pBx->GetGirder();
	CStringer  *pStg = pBx->GetStringer(nStg);
	if(!pStg) return 0;		
	
	double dWHalf = pStg->m_uM + m_pDB->m_dWidthHunch - m_dCoverSd;
	if(!bUpper)
	{
		return (dWHalf + (m_pDB->m_dHunchDisForBaseThick + m_pDB->m_dThickSlabBase - m_dCoverUp - m_dCoverDn)) * 3 * 2;	
	}
	else*/
		return m_dLengthStringerBogang;
}


double CADeckData::GetLengthOutBrkHunchRebar(CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft)
{
/*	CPlateGirderApp  *pGir = pBx->GetGirder();
	CBracket	*pBrk = pBx->GetBracket(bLeft);
	if(!pBrk) return 0;		
	
	if(!bUpper)
	{	
		double dWidth = pBrk->m_uM_EB + m_pDB->m_dWidthHunch;
		return dWidth + (m_pDB->m_dThickSlabHunch + m_pDB->m_dThickSlabBase - m_dCoverUp - m_dCoverDn)*6;	// 1:3구배에 양측이므로 2배		
	}
*/
	return m_dLengthOutBrkBogang;
}


// nCalcPos=-1 : 슬래브 좌측
// nCalcPos= 0 : 슬래브 중앙
// nCalcPos= 1 : 슬래브 우측
CDPointArray CADeckData::GetDimArrAtMultiCurve(double dStaStt, CDPointArray xyArrDimSource, double dDistSource, long nCalcPos, BOOL bPositiveDir)
{
	CPlateGirderApp		*pGirCen = m_pDB->GetGirder(-1);
	CDPointArray xyArrCalc;

	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	// xyArrDimSource를 이용하여 실제 위치를 구함.
	CRebarPlacing RebarPlacing;
	CDoubleArray dArrDim, dArrSta, dArrOffset;
	RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimSource, dArrDim, FALSE);

	long n = 0;
	if(bPositiveDir==FALSE)
	{
		for (long n=0; n<dArrDim.GetSize(); n++)
			dArrDim[n] = -dArrDim[n];
	}

	dArrSta.Add(dStaStt);
	for(n=0; n<dArrDim.GetSize(); n++)
	{
		dStaStt = pGirCen->GetLine()->GetStationByDisLength(dStaStt, dArrDim[n], dDistSource);
		dArrSta.Add(dStaStt);
	}

	double dDistTarget = 0;
	if(nCalcPos==-1)	dDistTarget = dDisLStt;
	if(nCalcPos== 0)	dDistTarget = (dDisLStt+dDisRStt)/2;
	if(nCalcPos== 1)	dDistTarget = dDisRStt;

	for(n=0; n<dArrSta.GetUpperBound(); n++)
	{
		double dOffset = fabs(pGirCen->GetLine()->GetLengthByDis(dArrSta[n], dArrSta[n+1], dDistTarget));
		dArrOffset.Add(dOffset);
	}

	RebarPlacing.ConvertOffsetDoubleArrayToOffsetInfo(dArrOffset, xyArrCalc, 0);		// 크로소이드로 인한 오차.
	
	return xyArrCalc;
}


long CADeckData::GetRebarSuAtRange(CDPointArray xyArrDim, double dRange)
{
	if(dRange<=0)	return 0;
	
	CDoubleArray dArrDim;
	CRebarPlacing RebarPlacing;
	RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDim, dArrDim, TRUE);

	long nRebar = 1;
	for(long n=0; n<dArrDim.GetSize(); n++)
	{
		if(dArrDim[n] > dRange)
			break;

		nRebar++;
	}

	return nRebar;
}


long CADeckData::GetRebarSuAtDanbu(BOOL bUpper, BOOL bStt)
{
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	CPlateGirderApp		*pGirCen = m_pDB->GetGirder(-1);

	double dSttBlockOutWidth = pGlopt->GetSttExpansionJointWidth();
	double dEndBlockOutWidth = pGlopt->GetEndExpansionJointWidth();
	if(m_bInstallMainRebarAtBlockOut || bUpper==FALSE)
	{
		dSttBlockOutWidth  = 0;
		dEndBlockOutWidth  = 0;
	}

	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;
	double dDisLEnd		  = m_pDB->GetDisSlabLeftAct(dStaSlabEndCen)+m_dCoverSd;
	double dDisREnd		  = m_pDB->GetDisSlabRighAct(dStaSlabEndCen)-m_dCoverSd;
	CDPoint	vAngStt		  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd		  = pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndLeft= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);
	double dCoverSd       = GetLengthCoverSd(bStt); 
	
	long   nRebarDan = 0;
	
	if(bStt)
	{
		if(dStaSlabSttLeft < dStaSlabSttRigh)
			nRebarDan = GetRebarSuAtRange(m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_RIGHT:CTC_MAIN_LOWER_RIGHT], m_dRangeDanbuStt-dSttBlockOutWidth-dCoverSd);
		else
			nRebarDan = GetRebarSuAtRange(m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_LEFT], m_dRangeDanbuStt-dSttBlockOutWidth-dCoverSd);
	}
	else
	{
		CDPointArray arrCTCLeft, arrCTCRight;
		long n = 0;
		for(n=m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_LEFT].GetUpperBound(); n>=0; n--)
			arrCTCLeft.Add(m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_LEFT].GetAt(n));
		for(n=m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_RIGHT:CTC_MAIN_LOWER_RIGHT].GetUpperBound(); n>=0; n--)
			arrCTCRight.Add(m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_RIGHT:CTC_MAIN_LOWER_RIGHT].GetAt(n));

		if(dStaSlabEndLeft > dStaSlabEndRigh)
			nRebarDan = GetRebarSuAtRange(arrCTCRight, m_dRangeDanbuEnd-dEndBlockOutWidth-dCoverSd);
		else
			nRebarDan = GetRebarSuAtRange(arrCTCLeft, m_dRangeDanbuEnd-dEndBlockOutWidth-dCoverSd);
	}

	return nRebarDan;
}


/*
// 슬래브 평면에서 세로보의 길이를 구함
// 1. 세로보가 단부테이퍼에 있을 경우 0
// 2. 세로보가 단부테이퍼와 일반부에 동시 걸릴 경우 단부테이퍼  끝점에서 일반부 세로보 중심위치 까지
// 3. 세로보가 일반부 일경우 세로보 중심에서 중심까지
double CADeckData::GetStgLengthAtSlab(CStringer *pStg)
{
	pStg->SetXyzStringer();

	CPlateBridgeApp  *pDB = m_pDB;
	CPlateBasicIndex *pBx = pStg->GetBx();
	CPlateGirderApp	*pGir= pBx->GetGirder();

	CDPoint xy1 = pGir->GetXyPlanSlabStgBase(pBx, pStg->m_nStg, TRUE);
	CPlateBasicIndex *pBxNext = pGir->GetBxByCrossNo(pGir->GetNumberCrossBeamByBx(pBx)+1);
	CDPoint xy2 = pGir->GetXyPlanSlabStgBase(pBxNext, pStg->m_nStg, FALSE);
	CDPoint A[5], xyResult;

	double dHeightTaperStt = pGir->GetHeightTaper(pBx->GetStation());
	double dHeightTaperEnd = pBxNext ? pGir->GetHeightTaper(pBxNext->GetStation()) : 0;

	if(dHeightTaperStt > 0 && dHeightTaperEnd > 0)
	{
		return 0;
	}
	else if(dHeightTaperStt > 0)
	{
		pDB->GetXySss(OFF_STT_HUNCHEND, A);
		for(long n=1; n<5; n++)
		{
			if(GetXyMatchSegAndSeg(xy1, xy2,A[n],A[n-1],xyResult))
			{
				xy1 = xyResult;
				return ~(xy1-xy2);
			}
		}
	}
	else if(pBxNext && dHeightTaperEnd > 0)
	{
		pDB->GetXySss(OFF_END_HUNCHEND, A);
		for(long n=1; n<5; n++)
		{
			if(GetXyMatchSegAndSeg(xy1, xy2,A[n],A[n-1],xyResult))
			{
				xy2 = xyResult;
				return ~(xy1-xy2);
			}
		}
	}
	else
	{	
		return ~(xy1-xy2);
	}

	return 0;

}

*/


double CADeckData::GetAreaSlab()
{
	CPlateGirderApp		*pGirCen = m_pDB->GetGirder(-1);

	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen);
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen);
	double dDisLEnd		  = m_pDB->GetDisSlabLeftAct(dStaSlabEndCen);
	double dDisREnd		  = m_pDB->GetDisSlabRighAct(dStaSlabEndCen);
	CDPoint	vAngStt		  = pGirCen->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd		  = pGirCen->GetAAA(OFF_END_SLAB);
	double dStaSlabSttLeft= pGirCen->GetStationBySc(dStaSlabSttCen, dDisLStt, vAngStt);
	double dStaSlabSttRigh= pGirCen->GetStationBySc(dStaSlabSttCen, dDisRStt, vAngStt);
	double dStaSlabEndLeft= pGirCen->GetStationBySc(dStaSlabEndCen, dDisLEnd, vAngEnd);
	double dStaSlabEndRigh= pGirCen->GetStationBySc(dStaSlabEndCen, dDisREnd, vAngEnd);

	double dLengthSlabLeft = pGirCen->GetLengthDimType(dStaSlabSttLeft, dStaSlabEndLeft, dDisLStt);
	double dLengthSlabRigh = pGirCen->GetLengthDimType(dStaSlabSttRigh, dStaSlabEndRigh, dDisRStt);

	double dWidthAvg = 0.00;
	for(long i=0; i<m_pDB->m_nQtyJigan+1;i++)
		dWidthAvg += m_pDB->GetWidthAct(m_pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (m_pDB->m_nQtyJigan+1);

	double dAreaSlab = (dLengthSlabLeft+dLengthSlabRigh)/2 * dWidthAvg;

	return dAreaSlab;	// mm^2

}


// CTC배열을 기준으로 시점에서 dOffsetStt만큼 떨어진 위치에서 CTC중간점이 가장 근접한 위치를 찾음.
double CADeckData::GetNearstMidPointAtCTCArr(CDPointArray xyArrDim, double dOffsetStt)
{
	CRebarPlacing RebarPlacing;
	CDoubleArray dArrDim;
	RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDim, dArrDim, TRUE);	

	CDoubleArray dArrMid;		// 철근과 철근의 중간위치를 구함	
	double	dReturn = 0;
	long	n		= 0;
	for(n=0; n<dArrDim.GetUpperBound(); n++)
		dArrMid.Add((dArrDim[n]+dArrDim[n+1])/2);

	// 중간위치 배열중 dOffsetStt 가장 가까운 것(dReturn)을 찾는다.
	double dMin		= 1000000;
	for(n=0; n<dArrMid.GetSize(); n++)
	{
		if(fabs(dArrMid[n]-dOffsetStt) < dMin)
		{
			dMin	= fabs(dArrMid[n]-dOffsetStt);
			dReturn = dArrMid[n];
		}
	}

	return dReturn;
}



CDPointArray CADeckData::GetDimArrByCycle(CDPointArray xyArrMark, long nCycle, long nCyclesu)
{
	CDPointArray xyArrReturn;

	for(long n=nCycle; n<xyArrMark.GetSize(); n+=nCyclesu)
	{
		xyArrReturn.Add(xyArrMark[n]);
	}

	return xyArrReturn;
}


CDPointArray CADeckData::GetDimArrCenByLeftRight(CDPointArray &xyArrDimLeft, CDPointArray &xyArrDimRigh)
{
	CDPointArray xyArrReturn;
	CDPoint xyLeft, xyRight, xyCen;

	CRebarPlacing RebarPlace;
	CDoubleArray dArrLeftPos, dArrRighPos, dArrCenterPos;
	
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimLeft, dArrLeftPos, FALSE);
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimRigh, dArrRighPos, FALSE);
	
	long nSize = min(dArrLeftPos.GetSize(), dArrRighPos.GetSize());
	for (long n=0; n<nSize; n++)
		dArrCenterPos.Add((dArrLeftPos[n]+dArrRighPos[n])/2);

	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCenterPos, xyArrReturn, 0);

	return xyArrReturn;
}


// Trim되어서 중간이 비어있는 xyArrMarkPos를 분할함. 
void CADeckData::DivideMarkPosAtJigan(CDPointArray *xyArrMarkPos, CDPointArray *xyArrMarkPosStt, CDPointArray *xyArrMarkPosEnd, long nJigan)
{
	CDoubleArray dStaArr;
	if(m_arrTrimRange.GetSize()<1) return;

	double	dSta = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
	long	n	 = 0;

	dStaArr.Add(dSta);
	for(n=0; n<m_arrTrimRange.GetSize(); n++)
	{		
		dSta += m_arrTrimRange.GetAt(n);				
		dStaArr.Add(dSta);
	}
		
	double dStaStt = dStaArr.GetAt(nJigan*2+1);
	double dStaEnd = dStaArr.GetAt(nJigan*2+2);

	xyArrMarkPosStt->RemoveAll();
	xyArrMarkPosEnd->RemoveAll();

	CDPointArray xyArrTemp;
	for( n=0; n<xyArrMarkPos->GetSize(); n++)
	{
		if(IsDrawMainRebarArea(xyArrMarkPos->GetAt(n), xyArrMarkPos->GetAt(n)))
			xyArrTemp.Add(xyArrMarkPos->GetAt(n));
	}
	double dValueCheck = 1000;	// 좌측trim영역과 우측trim영역의 거리
	CDPointArray xyArrTemp1, xyArrTemp2;

	for(n=0; n<xyArrTemp.GetUpperBound(); n++)
	{
		if (~(xyArrTemp[n]-xyArrTemp[n+1]) < dValueCheck)
		{
			xyArrTemp1.Add(xyArrTemp[n]);	
		}
		else
		{
			xyArrTemp1.Add(xyArrTemp[n]);	
			break;
		}
	}

	for(long nn=n+1; nn<xyArrTemp.GetSize(); nn++)
	{
		xyArrTemp2.Add(xyArrTemp[nn]);	
	}

	
	CDPoint xyCheck1 = xyArrTemp1.GetSize()>2 ? xyArrTemp1[xyArrTemp1.GetUpperBound()/2] : CDPoint(0,0);
	CDPoint xyCheck2 = xyArrTemp2.GetSize()>2 ? xyArrTemp2[xyArrTemp2.GetUpperBound()/2] : CDPoint(0,0);

	double dStaCheck1 = 0;
	double dStaCheck2 = 0;
	m_pDB->m_pLine->GetStationMatchVerticalByXy(xyCheck1, dStaCheck1);
	m_pDB->m_pLine->GetStationMatchVerticalByXy(xyCheck2, dStaCheck2);

	if(dStaStt>dStaCheck1 && xyCheck1!=CDPoint(0,0))
		*xyArrMarkPosStt = xyArrTemp1;

	if(dStaStt>dStaCheck2 && xyCheck2!=CDPoint(0,0))
		*xyArrMarkPosStt = xyArrTemp2;

	if(dStaEnd<dStaCheck1 && xyCheck1!=CDPoint(0,0))
		*xyArrMarkPosEnd = xyArrTemp1;
	
	if(dStaEnd<dStaCheck2 && xyCheck2!=CDPoint(0,0))
		*xyArrMarkPosEnd = xyArrTemp2;

}


//기타철근에서 사용되는 제일처음 스터드의 위치를 리턴한다.
CDPointArray CADeckData::GetPosFirstStud()
{
	CDoubleArray dArrDis;
	CDPointArray ptArrStud;
	CDPoint ptStud;
	int i = 0;

	for(i = 0; i < STUD_HORI_SU; i++)
	{
		double dDis = m_pDB->GetGirder(0)->m_pSangse->m_StudHori_Dis[i];
		dArrDis.Add(dDis);
	}

	int nSize = dArrDis.GetSize();

	if(nSize > 1)
	{
		ptArrStud.Add(ptStud);  //0,0부터 시작한다.

		for(i = 1; i < nSize-1; i++)   //제일처음(0)은 피복두께 이므로 무시한다. 1부터 시작 
		{
			double dDis1 = dArrDis.GetAt(i);
			double dDis2 = dArrDis.GetAt(i+1);
			if(dDis1 == dDis2)
			{
				ptStud.y -= dDis1;
				ptArrStud.Add(ptStud);  //0,0부터 시작한다.
			}					
			else
			{
				ptStud.y -= dDis1;
				ptArrStud.Add(ptStud);  //마지막으로 추가하고, 빠져나간다.

				return ptArrStud;
			}
		}
	}

	return ptArrStud;
}



void CADeckData::GetArrJointDisAtJijum(long nPier, CDPoint vAngStt, CDPoint vAngEnd, double dDisStt, double dDisEnd, double *dArrJoint)
{
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);
	CLineInfoApp *pLineApp = (CLineInfoApp*)m_pDB->m_pLine;

	CPlateBasicIndex *pBxJijum = pGirCen->GetBxOnJijum(nPier);
	double dStaJijum = pBxJijum->GetStation();
	double dDisL = GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDisR = GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDisC = (dDisL+dDisR)/2;
	// 지점각도
	CDPoint vAngJijum = pBxJijum->GetAngle();
	// 횡단배근위치별 스테이션 
	double dStaJijumSlabLeft   = pLineApp->GetStationByScInLine(dStaJijum, dDisL, vAngJijum);
	double dStaJijumSlabRigh   = pLineApp->GetStationByScInLine(dStaJijum, dDisR, vAngJijum);
	// 슬래브 중심선상의 점
	CDPoint xyMatchCLStt  = pLineApp->GetXyLineDisByAng(dStaJijum-dDisStt, 0, vAngStt);
	CDPoint xyMatchCLEnd  = pLineApp->GetXyLineDisByAng(dStaJijum+dDisEnd, 0, vAngEnd);
	CDPoint xyMatchSlabCenStt  = pLineApp->GetXyLineDisByAng(dStaJijum-dDisStt, dDisC, vAngStt);
	CDPoint xyMatchSlabCenEnd  = pLineApp->GetXyLineDisByAng(dStaJijum+dDisEnd, dDisC, vAngEnd);
	// 방위각
	CDPoint xyTempStt = pLineApp->GetXyLineDisByAng(dStaJijum-dDisStt, 100000, vAngStt);
	CDPoint xyTempEnd = pLineApp->GetXyLineDisByAng(dStaJijum+dDisEnd, 100000, vAngEnd);
	CDPoint vAngSttGlobal = (xyMatchSlabCenStt-xyTempStt).Unit();
	CDPoint vAngEndGlobal = (xyMatchSlabCenEnd-xyTempEnd).Unit();

	double dStaMatchCLStt = 0;
	double dStaMatchCLEnd = 0;

	pLineApp->GetStationMatchByXyAndAngle(xyMatchSlabCenStt, vAngSttGlobal, dStaMatchCLStt);
	pLineApp->GetStationMatchByXyAndAngle(xyMatchSlabCenEnd, vAngEndGlobal, dStaMatchCLEnd);

	double dStaLeftStt = pLineApp->GetStationByScInLine(dStaMatchCLStt, dDisL, vAngStt);
	double dStaRighStt = pLineApp->GetStationByScInLine(dStaMatchCLStt, dDisR, vAngStt);
	double dStaLeftEnd = pLineApp->GetStationByScInLine(dStaMatchCLEnd, dDisL, vAngEnd);
	double dStaRighEnd = pLineApp->GetStationByScInLine(dStaMatchCLEnd, dDisR, vAngEnd);
	
	dArrJoint[0] = pLineApp->GetLengthByDis(dStaLeftStt, dStaJijumSlabLeft, dDisL);
	dArrJoint[1] = pLineApp->GetLengthByDis(dStaJijumSlabLeft, dStaLeftEnd, dDisL);
	dArrJoint[2] = pLineApp->GetLengthByDis(dStaRighStt, dStaJijumSlabRigh, dDisR);
	dArrJoint[3] = pLineApp->GetLengthByDis(dStaJijumSlabRigh, dStaRighEnd, dDisR);
}


int CADeckData::SearchRebar(CString strMark, CRebar* pRB)
{
	int nSize = m_arrRebarJewon.GetSize();
	CRebar* pRebar = NULL;

	long i = 0;
	for(i = 0; i < nSize ;i++)
	{
		pRebar = m_arrRebarJewon.GetAt(i);
		if(pRebar->m_sMark == strMark)
		{
			*pRB = *pRebar;
			return i;
		}
	}

	nSize = m_arrRebarJewonGuard.GetSize();
	for(i = 0; i < nSize ;i++)
	{
		pRebar = m_arrRebarJewonGuard.GetAt(i);
		if(pRebar->m_sMark == strMark)
		{
			*pRB = *pRebar;
			return i;
		}
	}

	pRB = NULL;
	return -1;
}


BOOL CADeckData::DeleteRebar(CString strMark)
{
	CRebar *pRB = new CRebar;
	int nIdx = SearchRebar(strMark, pRB);

	if(nIdx == -1)
		return FALSE;
	else
	{
		CString str = pRB->m_sMark.Left(3);
		if(str == "ADD")
			m_nAddRebarsu--;

		if(pRB->m_nIdxNumber == 4)	//방호벽 철근 
			m_arrRebarJewonGuard.RemoveAt(nIdx);
		else
			m_arrRebarJewon.RemoveAt(nIdx);

		delete pRB;
		return TRUE;
	}
}


BOOL CADeckData::AddRebar(int nType)
{
	m_nAddRebarsu++;

	 CRebar* pRB = new CRebar;

	pRB->m_Fy 		  = toKgPCm2(m_SigmaY);	
	pRB->m_bMainRebar = TRUE;
	pRB->m_nTypeRebar = 1;
	pRB->m_dDia = 16;
	pRB->m_dRateAdd = m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	pRB->m_nIdxNumber = nType;
	pRB->m_sDescription = "추가철근";
	pRB->m_sMark.Format("ADD%d", m_nAddRebarsu);

	if(nType == 4) //방호벽이면..
		m_arrRebarJewonGuard.Add(pRB);
	else
		m_arrRebarJewon.Add(pRB);

		return TRUE;
}


// 마크위치에서 슬래브 시점부나 종점부까지의거리를 계산하여 리턴함
double CADeckData::GetLengthExtendToSlabSttEndFrPoint(CDPointArray xyArrMark, double dExtPlus, BOOL bFromSttMark, BOOL bSttSlabLine)
{
	if(xyArrMark.GetSize() < 2)		return 0;

	CDPoint xyResult;
	double LDist = 0;
	double RDist = 0;
	CDPoint pt1, pt2; 
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);

	if(bSttSlabLine)
	{
		LDist = GetSideBarDistFromCenterGir(TRUE, TRUE);
		RDist = GetSideBarDistFromCenterGir(TRUE, FALSE);
		pt1   = pGirCen->GetXyGirderDis(pGirCen->GetSSS(OFF_STT_SLAB), LDist, pGirCen->GetAAA(OFF_STT_SLAB));
		pt2   = pGirCen->GetXyGirderDis(pGirCen->GetSSS(OFF_STT_SLAB), RDist, pGirCen->GetAAA(OFF_STT_SLAB));	
	}
	else
	{
		LDist = GetSideBarDistFromCenterGir(FALSE, TRUE);
		RDist = GetSideBarDistFromCenterGir(FALSE, FALSE);
		pt1   = pGirCen->GetXyGirderDis(pGirCen->GetSSS(OFF_END_SLAB), LDist, pGirCen->GetAAA(OFF_END_SLAB));
		pt2   = pGirCen->GetXyGirderDis(pGirCen->GetSSS(OFF_END_SLAB), RDist, pGirCen->GetAAA(OFF_END_SLAB));	
	}

	GetXyMatchLineAndLine(pt1, (pt1-pt2).Unit(), xyArrMark[0], (xyArrMark[0]-xyArrMark[xyArrMark.GetUpperBound()]).Unit(), xyResult);

	double dLengthExt = 0;
	
	if (bFromSttMark)
		dLengthExt = ~(xyResult-xyArrMark[0]) + dExtPlus;
	else
		dLengthExt = ~(xyResult-xyArrMark[xyArrMark.GetUpperBound()]) + dExtPlus;

	return dLengthExt;
}



// 마크위치에서 슬래브 시점부나 종점부까지의거리를 계산하여 리턴함
double CADeckData::GetLengthExtendToSlabLeftRighFrPoint(CDPoint xyPoint, double dExtPlus, BOOL bLeftSlab)
{
	double dStaCen  = 0;
	m_pDB->m_pLine->GetStationMatchVerticalByXy(xyPoint, dStaCen);
	double dDisLStt = m_pDB->GetDisSlabLeftAct(dStaCen);
	double dDisRStt = m_pDB->GetDisSlabRighAct(dStaCen);
	CDPoint xyMatch = m_pDB->m_pLine->GetXyLineDis(dStaCen, bLeftSlab ? dDisLStt : dDisRStt);

	double dLengthExt = ~(xyMatch-xyPoint) + dExtPlus;
	
	return dLengthExt;
}



//종단면상에서의 지간의 시작과 끝위치를 리턴한다.
BOOL CADeckData::GetPosJiganAtJong(int nJigan, double &dStt, double& dEnd, long nJongBasePos)
{
	dStt = 0;
	dEnd = 0;

	int nCntJigan = m_pDB->m_nQtyJigan;
	if(nJigan < 0 || nJigan >= nCntJigan)
		return FALSE;

	if(nJigan > 0)  //좌측 단지점부 
		dStt = GetLengthSlabAccuml(nJigan - 1, nJongBasePos);

	dEnd = dStt + GetLengthSlab(nJigan, nJongBasePos);

	return TRUE;
}


// 슬래브좌측, 우측, 중앙부의 주철근 CTC 동일한가?
BOOL CADeckData::IsSameMainRebarCTC(BOOL bDanbu, BOOL bUpper)
{
	if(bDanbu)
	{
		if(bUpper)
		{
			if ((m_dEndMainRebarCTC[0]==m_dEndMainRebarCTC[1]) && (m_dEndMainRebarCTC[1]==m_dEndMainRebarCTC[2]))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if ((m_dEndMainRebarCTCLower[0]==m_dEndMainRebarCTCLower[1]) && (m_dEndMainRebarCTCLower[1]==m_dEndMainRebarCTCLower[2]))
				return TRUE;
			else
				return FALSE;
		}
	}
	else
	{
		if(bUpper)
		{
			if ((m_dGenMainRebarCTC[0]==m_dGenMainRebarCTC[1]) && (m_dGenMainRebarCTC[1]==m_dGenMainRebarCTC[2]))
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			if ((m_dGenMainRebarCTCLower[0]==m_dGenMainRebarCTCLower[1]) && (m_dGenMainRebarCTCLower[1]==m_dGenMainRebarCTCLower[2]))
				return TRUE;
			else
				return FALSE;
		}
	}
}



CDPoint CADeckData::GetSlopeAtHDanByPos(CDPointArray xyArrHDan, CDPoint xyPos)
{
	CDPoint xyDir;

	for(long n=0; n<xyArrHDan.GetUpperBound(); n++)
	{
		if(xyArrHDan[n].x <=xyPos.x && xyPos.x <=xyArrHDan[n+1].x)
		{
			xyDir = (xyArrHDan[n+1]-xyArrHDan[n]).Unit();

			break;
		}
	}


	return xyDir;
}



// nPos = 0 좌측
// nPos = 1 우측
// nPos = 2 중앙
double CADeckData::GetJointLengthAtHDan(long nSection, BOOL bUpper, long nHDanPos)
{
	double dJointL = 0;		// 슬래브좌측과 중앙부의 이음길이
	double dJointR = 0;		// 슬래브우측과 중앙부의 이음길이
	double dJointC = 0;		// 슬래브 중앙부 철근의 이음길이

	if(nSection==SECTION_DAN)		// 시종점부의 철근 직경이 동일하다고 가정함
	{
		double dDiaDanbuL = bUpper ? m_dDiaMainRebar_Dan[0] : m_dDiaMainRebarLower_Dan[0];	// 단부 좌측 주철근 직경	
		double dDiaDanbuR = bUpper ? m_dDiaMainRebar_Dan[1] : m_dDiaMainRebarLower_Dan[1];	// 단부 우측 주철근 직경	
		double dDiaDanbuC = bUpper ? m_dDiaMainRebar_Dan[2] : m_dDiaMainRebarLower_Dan[2];	// 단부 중앙 주철근 직경	

		long nJointStyle = JOINT_TENS_NORMAL_B;
		if(bUpper && (nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END))
			nJointStyle = JOINT_TENS_UPPER_B;

		dJointL = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuL, nJointStyle, toCm(bUpper ? m_dCoverUp : m_dCoverDn), toCm(bUpper ? m_dEndMainRebarCTC[0]/2 : m_dEndMainRebarCTCLower[0]/2));
		dJointR = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuR, nJointStyle, toCm(bUpper ? m_dCoverUp : m_dCoverDn), toCm(bUpper ? m_dEndMainRebarCTC[1]/2 : m_dEndMainRebarCTCLower[1]/2));
		dJointC = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuC, nJointStyle, toCm(bUpper ? m_dCoverUp : m_dCoverDn), toCm(bUpper ? m_dEndMainRebarCTC[2]/2 : m_dEndMainRebarCTCLower[2]/2));
		dJointL = max(dJointL, dJointC);
		dJointR = max(dJointR, dJointC);
	}
	else if(nSection==SECTION_CEN || nSection==SECTION_PIER)
	{
		double dDiaGenL = bUpper ? m_dDiaMainRebar_Cent[0] : m_dDiaMainRebarLower_Cent[0];	// 일반부 좌측 주철근 직경	
		double dDiaGenR = bUpper ? m_dDiaMainRebar_Cent[1] : m_dDiaMainRebarLower_Cent[1];	// 일반부 우측 주철근 직경	
		double dDiaGenC = bUpper ? m_dDiaMainRebar_Cent[2] : m_dDiaMainRebarLower_Cent[2];	// 일반부 중앙 주철근 직경	
		
		long nJointStyle = JOINT_TENS_NORMAL_B;
		
		dJointL = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenL, nJointStyle, toCm(bUpper ? m_dCoverUp : m_dCoverDn), toCm(bUpper ? m_dGenMainRebarCTC[0]/2 : m_dGenMainRebarCTCLower[0]/2));
		dJointR = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenR, nJointStyle, toCm(bUpper ? m_dCoverUp : m_dCoverDn), toCm(bUpper ? m_dGenMainRebarCTC[1]/2 : m_dGenMainRebarCTCLower[1]/2));
		dJointC = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenC, nJointStyle, toCm(bUpper ? m_dCoverUp : m_dCoverDn), toCm(bUpper ? m_dGenMainRebarCTC[2]/2 : m_dGenMainRebarCTCLower[2]/2));
		dJointL = max(dJointL, dJointC);
		dJointR = max(dJointR, dJointC);
	}
	else if(nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END)// 시종점부의 철근 직경이 동일하다고 가정함
	{
		double dDiaSkew = bUpper ? m_dDiaSabogangUp : m_dDiaSabogangDn;	// 일반부 좌측 주철근 직경	
		long nJointStyle = JOINT_TENS_NORMAL_B;
		if(bUpper && (nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END))
			nJointStyle = JOINT_TENS_UPPER_B;
		
		dJointL = dJointR = dJointC = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaSkew, nJointStyle, toCm(bUpper ? m_dCoverUp : m_dCoverDn), toCm(bUpper ? m_dEndMainRebarCTC[1]/2 : m_dEndMainRebarCTCLower[1]/2));
	}

	if	   (nHDanPos==POS_SLAB_LEFT)
		return dJointL;
	else if(nHDanPos==POS_SLAB_RIGH)
		return dJointR;
	else
		return dJointC;

}


//nJongPos = 0 단부 이음길이
//nJongPos = 1 일반부 이음길이 
//nJongPos = 3 지점부 이음길이 
//nJongPos = 4 단부와 일반부중 큰 이음길이
//nJongPos = 5 지점부와 일반부중 큰 이음길이
double CADeckData::GetJointLengthAtJDan(long nJongPos, BOOL bUpper, long nJongBasePos)
{
	long nPos = 0;	// 0=좌측, 1=우측, 2=중앙부
	if(nJongBasePos==0 || nJongBasePos==1)		nPos = 2;
	else if(nJongBasePos == 2)					nPos = 0;
	else if(nJongBasePos == 3)					nPos = 1;

	double dDiaDanbuBeryuk= bUpper ? long(m_dDiaBeryukRebar_Dan[nPos]) : long(m_dDiaBeryukRebarLower_Dan[nPos]);	// 단부 배력철근 직경	
	double dDiaGenBeryuk  = bUpper ? long(m_dDiaBeryukRebar_Cent[nPos]) : long(m_dDiaBeryukRebarLower_Cent[nPos]);	// 일반부 배력철근 직경
	double dDiaJijumBeryuk= bUpper ? long(m_dDiaJijumRebarUp[nPos])		: long(m_dDiaJijumRebarDn[nPos]);			// 지점부 배력철근 직경

	double dCover = bUpper ? m_dCoverUp : m_dCoverDn;
	double dLJointDan	  = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuBeryuk,	JOINT_TENS_NORMAL_B, toCm(dCover), toCm(bUpper ? m_dEndSupportRebarCTC[nPos] : m_dEndSupportRebarCTCLower[nPos]));
	double dLJointGen	  = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenBeryuk,	JOINT_TENS_NORMAL_B, toCm(dCover), toCm(bUpper ? m_dGenSupportRebarCTC[nPos] : m_dGenSupportRebarCTCLower[nPos]));
	double dLJointJijum	  = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaJijumBeryuk,	JOINT_TENS_NORMAL_B, toCm(dCover), toCm(bUpper ? m_dJijumRebarCTCUp[nPos] : m_dJijumRebarCTCDn[nPos]));

	switch(nJongPos)
	{
	case 0:
		return dLJointDan;
	case 1:
		return dLJointGen;
	case 2:
		return dLJointJijum;
	case 3:
		return max(dLJointDan, dLJointGen);
	case 4:
		return max(dLJointJijum, dLJointGen);
	}
	return 0;
}



double CADeckData::GetRebarWeightTotal(BOOL bMainRebar, BOOL bGuardRebar)
{
	double dTotal = 0;
	double dWeightRebar = 0; 
	double dWeightUnit  = 0; 
	if(bMainRebar)
	{
		for( long n = 0; n < m_arrRebarJewon.GetSize(); n++)
		{
			CRebar* pRB = m_arrRebarJewon[n];
			dWeightUnit  = GetRebarInfo((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
			dWeightRebar = dWeightUnit * toM(pRB->GetLength()*pRB->m_nEa);
			
			dTotal += dWeightRebar;
		}
	}
	if(bGuardRebar)
	{
		for( long n = 0; n < m_arrRebarJewonGuard.GetSize(); n++)
		{
			CRebar* pRB = m_arrRebarJewonGuard[n];
			dWeightUnit  = GetRebarInfo((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
			dWeightRebar = dWeightUnit * toM(pRB->GetLength()*pRB->m_dEa);
			
			dTotal += dWeightRebar;
		}

	}
	
	return dTotal;
}



// X축을 기준으로 시점과 종점의 중간위치에서 xyArrPoly에 교차하는점(마크점 찾기위해 사용)
CDPoint CADeckData::GetMidPointAtPolyLine(CDPointArray xyArrPoly)
{
	CDPoint xyMid = CDPoint((xyArrPoly[0].x+xyArrPoly[xyArrPoly.GetUpperBound()].x)/2, 0);
	CDPoint xyResult;
	for (long n=0; n<xyArrPoly.GetUpperBound(); n++)
	{
		if(GetXyMatchSegAndLine(xyArrPoly[n], xyArrPoly[n+1], xyMid, CDPoint(0,1), xyResult))
			return xyResult;
	}

	return xyResult;
}



double CADeckData::GetLengthJiganByGirder(long nG, long nJigan)
{
	CPlateGirderApp	*pGir  = m_pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxStt= pGir->GetBxOnJijum(nJigan);	
	CPlateBasicIndex	*pBxEnd= pGir->GetBxOnJijum(nJigan+1);	
	double dLengthJigan = pGir->GetLengthDimType(pBxStt->GetStation(), pBxEnd->GetStation());
	
	return dLengthJigan;
}

void CADeckData::MakeRebarJewon_GuardFence(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bCalcMeter)
{
	int nSize = m_pDB->GetGuardFencesu();
	CRebar* pRB = NULL;
	
	for(int i = 0; i < nSize; i++)
	{
		int nNo = 1;
		CGuardFenceRebar* pGFBar = m_pDB->GetGuardFence(i)->GetRebar();
		CString strSym = pGFBar->GetTextMark();

		int nType = pGFBar->GetTypePlacingRebar();
		double dCTC = pGFBar->GetMainRebarCTC();
		double dLengthSlab = 0;
		if(bCalcMeter == TRUE)
			dLengthSlab = 1000;
		else 
			dLengthSlab = GetLengthSlabTotal(JONGBASEPOS_SLABLEFT);

		if(nType == 1)
		{
			CDPointArray ptArrBar = pGFBar->GetPosMainRebarType1();
			CDPoint ptDot[8];
			for(int j = 0; j < 8; j++)
				ptDot[j] = ptArrBar.GetAt(j);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 45;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(0);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[0] - ptDot[1]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[1] - ptDot[2]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[2] - ptDot[3]), 0);
			pRB->m_dLengthMax_D = Round(~(ptDot[3] - ptDot[4]), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);
			pRB->m_nIdxNumber = 4;  // 방호벽철근 
			pRB->m_nPos = i;		// 방호벽번호
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 3;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(1);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[4] - ptDot[5]), 0);
			pRB->m_dLengthMax_B = 100;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 2;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(2);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[5] - ptDot[6]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[6] - ptDot[7]), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}
		else if(nType == 2)
		{
			vector<CDPointArray> vecBar = pGFBar->GetPosMainRebarType2();
			CDPointArray ptArrBar[3];
			for(int j = 0; j < 3; j++)
				ptArrBar[j] = vecBar[j];

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 13;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(0);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pRB->m_dLengthMax_A = Round(~(ptArrBar[0].GetAt(0) - ptArrBar[0].GetAt(1)), 0);
			pRB->m_dLengthMax_B = Round(~(ptArrBar[0].GetAt(1) - ptArrBar[0].GetAt(2)), 0);
			pRB->m_dLengthMax_C = Round(~(ptArrBar[0].GetAt(2) - ptArrBar[0].GetAt(3)), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);


			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 1;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(1);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptArrBar[1].GetAt(0) - ptArrBar[1].GetAt(1)), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);
	
			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 2;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(2);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptArrBar[2].GetAt(0) - ptArrBar[2].GetAt(1)), 0);
			pRB->m_dLengthMax_B = Round(~(ptArrBar[2].GetAt(1) - ptArrBar[2].GetAt(2)), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}
		else if(nType == 3)
		{
			CDPointArray ptArrBar = pGFBar->GetPosMainRebarType3();

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 1;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(0);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptArrBar.GetAt(0) - ptArrBar.GetAt(1)), 0);
			pRB->m_dEa = dLengthSlab / dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}
		else if(nType == 4)
		{
			CDPointArray ptArrBar = pGFBar->GetPosMainRebarType4();

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 7;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(0);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptArrBar.GetAt(2) - ptArrBar.GetAt(3)), 0);
			pRB->m_dLengthMax_B = Round(~(ptArrBar.GetAt(1) - ptArrBar.GetAt(2)), 0);
			pRB->m_dLengthMax_C = Round(~(ptArrBar.GetAt(0) - ptArrBar.GetAt(1)), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}
		else if(nType == 5)
		{
			CDPointArray ptArrBar = pGFBar->GetPosMainRebarType5();
			CDPoint ptDot[9];
			for(int j = 0; j < 9; j++)
				ptDot[j] = ptArrBar.GetAt(j);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 45;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(0);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[0] - ptDot[1]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[1] - ptDot[2]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[2] - ptDot[3]), 0);
			pRB->m_dLengthMax_D = Round(~(ptDot[3] - ptDot[4]), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 3;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(1);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[4] - ptDot[5]), 0);
			pRB->m_dLengthMax_B = 100;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 2;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(2);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[5] - ptDot[6]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[6] - ptDot[7]), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 11;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(3);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[0] - ptDot[1]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[1] - ptDot[2]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[2] - ptDot[8]), 0);
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}
		else if(nType == 6)
		{
			CDPointArray ptArrBar = pGFBar->GetPosMainRebarType6();
			CDPoint ptDot[9];
			for(int j = 0; j < 9; j++)
				ptDot[j] = ptArrBar.GetAt(j);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 36;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(0);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[0] - ptDot[1]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[1] - ptDot[2]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[2] - ptDot[3]), 0);
			pRB->m_dLengthMax_D = Round(~(ptDot[3] - ptDot[4]), 0);
			pRB->m_E = 50;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 79;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(1);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[6] - ptDot[7]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[5] - ptDot[6]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[4] - ptDot[5]), 0);
			pRB->m_dLengthMax_D = 50;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 45;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(3);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[0] - ptDot[1]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[1] - ptDot[2]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[2] - ptDot[8]), 0);
			pRB->m_dLengthMax_D = 50;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}
		else if(nType == 7)
		{
			CDPointArray ptArrBar = pGFBar->GetPosMainRebarType7();
			CDPoint ptDot[10];
			for(int j = 0; j < 10; j++)
				ptDot[j] = ptArrBar.GetAt(j);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 2;	// V형
			pRB->m_dDia			= pGFBar->GetMainRebarDia(0);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = ~(ptDot[4] - ptDot[9]);
			pRB->m_dLengthMax_B = 50;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 79;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(1);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[6] - ptDot[7]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[5] - ptDot[6]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[4] - ptDot[5]), 0);
			pRB->m_dLengthMax_D = 50;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);

			pRB = new CRebar;
			pRB->m_sDescription.Format("주철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 45;	
			pRB->m_dDia			= pGFBar->GetMainRebarDia(3);
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);

			pRB->m_dLengthMax_A = Round(~(ptDot[0] - ptDot[1]), 0);
			pRB->m_dLengthMax_B = Round(~(ptDot[1] - ptDot[2]), 0);
			pRB->m_dLengthMax_C = Round(~(ptDot[2] - ptDot[8]), 0);
			pRB->m_dLengthMax_D = 50;
			pRB->m_dEa	= dLengthSlab/dCTC; //좌측 끝단 
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}
		else
			return;


		if(nType != 3)
		{
			pRB = new CRebar;
			pRB->m_sDescription.Format("배력철근(방호벽%d)", i+1);
			pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
			pRB->m_Fy			= pGFBar->GetFy();	
			pRB->m_bMainRebar   = FALSE;
			pRB->m_nTypeRebar	= 1;	
			pRB->m_dDia			= pGFBar->GetDistRebarDia();
			pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);		

			if(bCalcMeter == FALSE)  //전체 길이로 계산 
			{
				pRB->m_dLengthMax_A	= Round(dLengthSlab, 0);
				pRB->m_nJoint = (long)pRB->m_dLengthMax_A / 8000;
				pRB->m_dJointLength = GetValueJoint(toKgPCm2(m_SigmaCK), pRB->m_Fy, pRB->m_dDia, JOINT_TENS_NORMAL_B
					, pGFBar->GetCover(), pGFBar->GetMainRebarCTC());
			}
			else if(bCalcMeter == TRUE) //단위미터로 계산
			{
				double dLen = GetValueJoint(toKgPCm2(m_SigmaCK), pRB->m_Fy, pRB->m_dDia, JOINT_TENS_NORMAL_B
					, pGFBar->GetCover(), pGFBar->GetMainRebarCTC());

				pRB->m_dLengthMax_A	= Round(dLengthSlab + (dLen / 8000)*1000, 0);
			}

			pRB->m_dEa = pGFBar->GetCntDistRebar();
			if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
				pRB->m_dEa = Round(pRB->m_dEa, 0);

			pRB->m_nIdxNumber = 4;  //방호벽철근 
			pRB->m_nPos = i;
			m_arrRebarJewonGuard.Add(pRB);
		}

		if(nType != 3)
		{
			CDPointArray ptArrShear;
			if(pGFBar->GetCntShearRebar() > 0)  //전단철근이 존재 할때만...
			{
				if(nType == 1)
					ptArrShear = pGFBar->GetPosShearRebarType1();
				else if(nType == 2)
					ptArrShear = pGFBar->GetPosShearRebarType2();
				else if(nType == 5)
					ptArrShear = pGFBar->GetPosShearRebarType5();
				else if(nType == 6)
					ptArrShear = pGFBar->GetPosShearRebarType6();
				else if(nType == 7)
					ptArrShear = pGFBar->GetPosShearRebarType7();


				int nCnt = ptArrShear.GetSize();

				for(int j = 0; j < nCnt; j+=2)
				{
					CDPoint ptStt = ptArrShear.GetAt(j);
					CDPoint ptEnd = ptArrShear.GetAt(j+1);

					pRB = new CRebar;
					pRB->m_sDescription.Format("전단철근(방호벽%d)", i+1);
					pRB->m_sMark.Format("%s%d-%d", strSym, i+1, nNo++);
					pRB->m_Fy			= pGFBar->GetFy();	
					pRB->m_bMainRebar   = FALSE;
					pRB->m_dDia			= pGFBar->GetShearRebarDia();
					pRB->m_dRateAdd		= m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);	
					double dLen = ~(ptStt - ptEnd);
					
					int nShearType = pGFBar->GetTypeShearBar();  //전단철근 타입 
					if(nShearType == 1 || nShearType == 2)
					{
						if(nShearType == 1)
							pRB->m_nTypeRebar	= 16;	// V형
						else if(nShearType == 2)
							pRB->m_nTypeRebar	= 17;	// V형

						if(nType != 2)
						{
							pRB->m_dLengthMax_A	= Round(pGFBar->GetMainRebarCTC() + pGFBar->GetShearRebarDia()
								+ pGFBar->GetMainRebarDia(0), 0);
							pRB->m_dLengthMax_B	= Round(dLen + pGFBar->GetShearRebarDia()
								+ pGFBar->GetMainRebarDia(0), 0);
						}
						else if(nType == 2)
						{
							double dMaxDia = max(pGFBar->GetMainRebarDia(0), pGFBar->GetMainRebarDia(1));
							pRB->m_dLengthMax_A	= Round(pGFBar->GetMainRebarCTC() + pGFBar->GetShearRebarDia()
								+ dMaxDia, 0);
							pRB->m_dLengthMax_B	= Round(dLen + pGFBar->GetShearRebarDia()
								+ pGFBar->GetMainRebarDia(0)/2 + pGFBar->GetMainRebarDia(1)/2, 0);
						}

						pRB->m_dLengthMax_C	= 100;
					}
					else if(nShearType == 3)
					{
						pRB->m_nTypeRebar	= 18;	// V형

						if(nType != 2)
						{
							pRB->m_dLengthMax_A	= Round(pGFBar->GetMainRebarCTC() + pGFBar->GetShearRebarDia()
								+ pGFBar->GetMainRebarDia(0), 0);
						}
						else if(nType == 2)
						{
							pRB->m_dLengthMax_A	= Round(pGFBar->GetMainRebarCTC() + pGFBar->GetShearRebarDia()
								+ pGFBar->GetMainRebarDia(0)/2 + pGFBar->GetMainRebarDia(1)/2, 0);
						}

						pRB->m_dLengthMax_B = Round(dLen, 0);
						pRB->m_dLengthMax_C = Round(dLen, 0);
					}

					int nCntMainBarSpace = pGFBar->GetCntMainBarSet();	//주철근간격 묶음 개수 
					pRB->m_dEa	= dLengthSlab / (dCTC*nCntMainBarSpace*2);
					if(bCalcMeter == FALSE)  //전체길이 계산이면... 정수로 한다.
						pRB->m_dEa = Round(pRB->m_dEa, 0);

					pRB->m_nIdxNumber = 4;  //방호벽철근 
					pRB->m_nPos = i;
					m_arrRebarJewonGuard.Add(pRB);
				}
			}
		}
	}
}


vector<double> CADeckData::GetLenArrFromLineArr(CDPointArray ptArrLine)
{
	vector<double> vecLen;

	for(int i = 0; i  < ptArrLine.GetSize(); i+=2)
	{
		CDPoint ptStt = ptArrLine.GetAt(i);
		CDPoint ptEnd = ptArrLine.GetAt(i+1);
		double dLen = ~(ptStt - ptEnd);
		vecLen.push_back(dLen);
	}

	return vecLen;
}



double CADeckData::GetRebarsuAtCycle(long nRebarsu, long nCycle, long nTotalCycle)
{
	if(nRebarsu==0)
		return 0;
	
	long nRebarCur = 0;
	for (long n=0; n<nCycle+1; n++)
	{
		nRebarCur = (long)Round(double(nRebarsu/(nTotalCycle-n)), 0);
		nRebarsu -= nRebarCur;
	}

	return nRebarCur;

}


// nBesu : 단경간 = 일반부/단부
// nBesu : 다경간 = 일반부/지점부
long CADeckData::GetsuRealCycle(long nJongBasePos, BOOL bUpper)
{
	long nBesu = GetBesu(nJongBasePos, bUpper);
	long nRealCyclesu= 1;
	if(nBesu>0)
		nRealCyclesu = long(m_stBeryukCycleLeft.size()/nBesu);
	else
		nRealCyclesu = 1;

	if(nRealCyclesu < 1)
		return 1;
	else
		return nRealCyclesu;
}




// nBesu : 단경간 = 일반부/단부
// nBesu : 다경간 = 일반부/지점부
long CADeckData::GetBesu(long nJongBasePos, BOOL bUpper)
{
	long nBesu = 0;

	if(bUpper)
	{		
		double	dMinCTC	= min(m_dJijumRebarCTCUp[nJongBasePos], m_dEndSupportRebarCTC[nJongBasePos]);
		if(m_pDB->m_nQtyJigan==1)
			dMinCTC = m_dEndSupportRebarCTC[nJongBasePos];
		nBesu = (long)RoundDown(m_dGenSupportRebarCTC[nJongBasePos]/dMinCTC, 0);
	}
	else
	{
		double	dMinCTC	= min(m_dJijumRebarCTCDn[nJongBasePos], m_dEndSupportRebarCTCLower[nJongBasePos]);
		if(m_pDB->m_nQtyJigan==1)
			dMinCTC = m_dEndSupportRebarCTCLower[nJongBasePos];
		nBesu = (long)RoundDown(m_dGenSupportRebarCTCLower[nJongBasePos]/dMinCTC, 0);
	}

	nBesu = min(nBesu, 3);
	nBesu = max(nBesu, 1);
	

	return nBesu;
}


double CADeckData::GetRatioOfPlanMark(CDPointArray xyArrRebar, BOOL bUpper, double& dRatioPre)
{
	xyArrRebar.Sort();

	CDPoint xyStt = xyArrRebar[0];
	CDPoint xyEnd = xyArrRebar[xyArrRebar.GetUpperBound()];
	double dRatio = -1;		// 슬래브좌우끝: 0, 슬래브중앙: 1
	double dStaSlabStt		 = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
	double dWidthHalfSlab	 = m_pDB->GetWidthAct(dStaSlabStt)/2 - m_dCoverSd;
	double dSlabCen			 = m_pDB->GetDisSlabLeftAct(dStaSlabStt) + m_pDB->GetWidthAct(dStaSlabStt)/2;

	if (bUpper)
	{
		if(xyStt.x<dSlabCen && xyEnd.x<dSlabCen)			// 양끝이 슬래브중심좌측에 있는 경우
			dRatio = (dWidthHalfSlab-(dSlabCen-(xyStt.x+xyEnd.x)/2)) / dWidthHalfSlab;
		else if(xyStt.x<dSlabCen && xyEnd.x>=dSlabCen)		// 시점만 슬래브중심좌측에 있는 경우
			dRatio = (dWidthHalfSlab-(dSlabCen-(xyStt.x+/*xyEnd.x*/dSlabCen)/2)) / dWidthHalfSlab;
	}
	else
	{
		if(xyStt.x>dSlabCen && xyEnd.x>dSlabCen)			// 양끝이 슬래브중심우측에 있는 경우
			dRatio = 1 - (((xyStt.x+xyEnd.x)/2-dSlabCen) / dWidthHalfSlab);
		else if(xyStt.x<=dSlabCen && xyEnd.x>dSlabCen)		// 종점만 슬래브중심우측에 있는 경우
			dRatio = 1 - (((xyEnd.x+/*xyStt.x*/dSlabCen)/2-dSlabCen) / dWidthHalfSlab);
	}
	
	if(dRatioPre==dRatio)
		dRatio += 600/~(xyStt-xyEnd);
	
	return dRatio;
}



void CADeckData::GetPointArrInCircle(CDPointArray& xyArrRebar, CDPoint xyCen, double dRad)
{
	CDPointArray xyArrTemp;
	for(long n=0; n<xyArrRebar.GetSize(); n++)
	{
		if(~(xyCen-xyArrRebar[n])<dRad)
			xyArrTemp.Add(xyArrRebar[n]);
	}

	xyArrRebar = xyArrTemp;
}





// CTC배열을 기준으로 시점에서 dOffsetStt만큼 떨어진 위치에서 CTC중간점이 가장 근접한 위치를 찾음.
double CADeckData::GetPointByOffsetAndCTC(CDPointArray xyArrDim, double dOffsetStt)
{
	CRebarPlacing RebarPlacing;
	CDoubleArray dArrDim;
	RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDim, dArrDim, TRUE);	

	CDoubleArray dArrMid;		// 철근과 철근의 중간위치를 구함	
	double	dReturn = 0;
	long	n		= 0;
	for(n=0; n<dArrDim.GetUpperBound(); n++)
		dArrMid.Add((dArrDim[n]+dArrDim[n+1])/2);

	// 중간위치 배열중 dOffsetStt 가장 가까운 것(dReturn)을 찾는다.
	double dMin		= 1000000;
	for(n=0; n<dArrMid.GetSize(); n++)
	{
		if(fabs(dArrMid[n]-dOffsetStt) < dMin)
		{
			dMin	= fabs(dArrMid[n]-dOffsetStt);
			dReturn = dArrMid[n];
		}
	}

	return dReturn;
}




CDPointArray CADeckData::GetDimTotalAtJongBasePos(BOOL bUpper, long nJongBasePos, BOOL bRealDim)
{
	CDPointArray xyArrDim;
	CDoubleArray dArrPos = GetRebarPosAtJongBasePos(bUpper, nJongBasePos, bRealDim);
	
	double dSttBlockOutWidth  = GetWidthBlockOut(TRUE, nJongBasePos);
	double dEndBlockOutWidth  = GetWidthBlockOut(FALSE, nJongBasePos);
	if(m_bInstallMainRebarAtBlockOut || !bUpper)
	{
		dSttBlockOutWidth  = 0;
		dEndBlockOutWidth  = 0;
	}
	double dSlabTotalL = GetLengthSlabTotal(nJongBasePos);
	double dCoverSdStt = GetLengthCoverSd(TRUE); 
	double dCoverSdEnd = GetLengthCoverSd(FALSE); 
	

	if(bUpper)
	{
		dArrPos.Add(0);
		dArrPos.Add(dSttBlockOutWidth);
		dArrPos.Sort();

		if(bRealDim)
		{
			dArrPos.Add(dSlabTotalL-dEndBlockOutWidth);
			dArrPos.Add(dSlabTotalL);
		}
		else
		{
			double dPosLast = dArrPos[dArrPos.GetUpperBound()];

			dArrPos.Add(dPosLast+dCoverSdEnd);
			dArrPos.Add(dPosLast+dCoverSdEnd+dEndBlockOutWidth);
		}
	}
	else
	{
		dArrPos.Add(0);
		dArrPos.Add(dCoverSdStt);
		dArrPos.Sort();
	
		if(bRealDim)
			dArrPos.Add(dSlabTotalL);
		else
		{
			double dPosLast = dArrPos[dArrPos.GetUpperBound()];
			dArrPos.Add(dPosLast+dCoverSdEnd);
		}
	}
	dArrPos.Sort();
	
	CDoubleArray dArrPosOffset;
	for (long n=0; n<dArrPos.GetSize()-1; n++)
	{
		dArrPosOffset.Add(dArrPos[n+1]-dArrPos[n]);
	}

	for (long nn=0; nn<dArrPosOffset.GetSize(); nn++)
	{
		TRACE2("n=%d, %lf\n", nn, dArrPosOffset[nn]);
	}

	CRebarPlacing RebarPlace;
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrPosOffset, xyArrDim, bRealDim ? 0 :m_dErrorRangeCTC);

	return xyArrDim;
}




// 신축이음부의 폭
double CADeckData::GetWidthBlockOut(BOOL bSttSlab, long nJongBasePos)
{
	CDPoint A[4], AA[4];
	double dRatio = 1;
	if(bSttSlab)
	{
		m_pDB->GetXySss(OFF_STT_SLAB, A);
		m_pDB->GetXySss(OFF_STT_EXP, AA);
		dRatio = fabs(m_pDB->GetGirder(-1)->GetAngleStt().y);
	}
	else
	{
		m_pDB->GetXySss(OFF_END_SLAB, A);
		m_pDB->GetXySss(OFF_END_EXP, AA);
		dRatio = fabs(m_pDB->GetGirder(-1)->GetAngleEnd().y);
	}
	CDPoint vDir  = (A[3]-A[0]).Unit();
	
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		return ~(A[0] - AA[0]);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		return ~(A[3] - AA[3]);
	}	
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		CDPoint xyStt = (A[0] + vDir*~(A[0]-A[3])/2);
		CDPoint xyEnd = (AA[0] + vDir*~(AA[0]-AA[3])/2);
		return ~(xyStt - xyEnd);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABLC)
	{
		return ~((A[0]+vDir*m_dWidthSlabCantLeftForBeryuk/dRatio) - (AA[0]+vDir*m_dWidthSlabCantLeftForBeryuk/dRatio));

	}
	else if(nJongBasePos==JONGBASEPOS_SLABRC)
	{
		return ~((A[3]-vDir*m_dWidthSlabCantRighForBeryuk/dRatio) - (AA[3]-vDir*m_dWidthSlabCantRighForBeryuk/dRatio));
	}
	else if(nJongBasePos==JONGBASEPOS_ONCB)
	{
		double dSta  = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
		double dDist = GetDistByJongBase(nJongBasePos);
		double dDistL = m_pDB->GetDisSlabLeftAct(dSta);
		return ~((A[0]+vDir*(dDist-dDistL)/dRatio) - (AA[0]+vDir*(dDist-dDistL)/dRatio));
	}


	return 0;
}



// 신축이음부의 폭
double CADeckData::GetWidthUgan(BOOL bSttSlab, long nJongBasePos)
{
	CDPoint A[4], AA[4];
	double dRatio = 1;
	if(bSttSlab)
	{
		m_pDB->GetXySss(OFF_STT_STT, A);
		m_pDB->GetXySss(OFF_STT_SLAB, AA);
		dRatio = fabs(m_pDB->GetGirder(-1)->GetAngleStt().y);
	}
	else
	{
		m_pDB->GetXySss(OFF_END_END, A);
		m_pDB->GetXySss(OFF_END_SLAB, AA);
		dRatio = fabs(m_pDB->GetGirder(-1)->GetAngleEnd().y);
	}

	CDPoint vDir  = (A[3]-A[0]).Unit();
	
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		return ~((A[0]+vDir*m_dCoverSd/dRatio) - (AA[0]+vDir*m_dCoverSd/dRatio));
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		return ~((A[3]-vDir*m_dCoverSd/dRatio) - (AA[3]-vDir*m_dCoverSd/dRatio));
	}
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		return ( ~(A[0]-AA[0]) + ~(A[3]-AA[3])) / 2;
	}
	else if(nJongBasePos==JONGBASEPOS_SLABLC)
	{
		return ~((A[0]+vDir*m_dWidthSlabCantLeftForBeryuk/dRatio) - (AA[0]+vDir*m_dWidthSlabCantLeftForBeryuk/dRatio));

	}
	else if(nJongBasePos==JONGBASEPOS_SLABRC)
	{
		return ~((A[3]-vDir*m_dWidthSlabCantRighForBeryuk/dRatio) - (AA[3]-vDir*m_dWidthSlabCantRighForBeryuk/dRatio));
	}
	else if(nJongBasePos==JONGBASEPOS_ONCB)
	{
		double dSta  = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
		double dDist = GetDistByJongBase(nJongBasePos);
		double dDistL = m_pDB->GetDisSlabLeftAct(dSta);
		return ~((A[0]+vDir*(dDist-dDistL)/dRatio) - (AA[0]+vDir*(dDist-dDistL)/dRatio));
	}



	return 0;
}


// bSttSlab : 슬래브 시점
// bLeftCalcPos : 슬래브좌측피복위치
double CADeckData::GetCoverSdToModify(BOOL bSttSlab, BOOL bLeftCalcPos)
{
	double dStaMatch = 0;
	double dStaSttL = GetStationMainRebarPos(bSttSlab, -1);
	double dStaSttR = GetStationMainRebarPos(bSttSlab,  1);
	double dDistL   = GetSideBarDistFromCenterGir(bSttSlab, TRUE);
	double dDistR   = GetSideBarDistFromCenterGir(bSttSlab, FALSE);
	double dCoverSd = GetLengthCoverSd(bSttSlab); 
	
	dStaSttL = m_pDB->m_pLine->GetStationByDisLength(dStaSttL, (bSttSlab?1:-1)*(dCoverSd+GetWidthBlockOut(bSttSlab, JONGBASEPOS_SLABLEFT)), dDistL);	
	dStaSttR = m_pDB->m_pLine->GetStationByDisLength(dStaSttR, (bSttSlab?1:-1)*(dCoverSd+GetWidthBlockOut(bSttSlab, JONGBASEPOS_SLABRIGHT)), dDistR);	
	CDPoint xyLeft = m_pDB->m_pLine->GetXyLineDis(dStaSttL, dDistL);
	CDPoint xyRigh = m_pDB->m_pLine->GetXyLineDis(dStaSttR, dDistR);
	CDPoint xyCent = (xyLeft+xyRigh)/2;
	CDPoint xyVert;
	if(m_nJongCtcCalcPos==0)	// 도로 중심
	{
		m_pDB->m_pLine->GetStationMatchByXyAndAngle(bLeftCalcPos ? xyRigh : xyLeft, (xyLeft-xyRigh).Unit(), dStaMatch);
		xyVert = m_pDB->GetGirder(-1)->GetXyGirderDis(dStaMatch, bLeftCalcPos ? dDistL : dDistR);

		double dLenModify = bLeftCalcPos ? ~(xyLeft-xyVert) : ~(xyRigh-xyVert);

		if(bLeftCalcPos)
		{
			if(xyLeft.x>xyVert.x)
				return bSttSlab ? -dLenModify : dLenModify;
			else
				return bSttSlab ? dLenModify : -dLenModify;
		}
		else
		{
			if(xyRigh.x>xyVert.x)
				return bSttSlab ? -dLenModify : dLenModify;
			else
				return bSttSlab ? dLenModify : -dLenModify;
		}
	}
	else if(m_nJongCtcCalcPos==1)	// 슬래브 중심
	{
		m_pDB->m_pLine->GetStationMatchVerticalByXy(xyCent, dStaMatch);
		xyVert = m_pDB->GetGirder(-1)->GetXyGirderDis(dStaMatch, bLeftCalcPos ? dDistL : dDistR);
		
		return bLeftCalcPos ? ~(xyLeft-xyVert) : ~(xyRigh-xyVert);

	}
	else if(m_nJongCtcCalcPos==2)	// 최대곡률위치
	{
		m_pDB->m_pLine->GetStationMatchVerticalByXy(bLeftCalcPos ? xyRigh : xyLeft, dStaMatch);
		xyVert = m_pDB->GetGirder(-1)->GetXyGirderDis(dStaMatch, bLeftCalcPos ? dDistL : dDistR);

		return bLeftCalcPos ? ~(xyLeft-xyVert) : ~(xyRigh-xyVert);
	}

	return -1;
}


// bSttSlab : 슬래브 시점
// bLeftCalcPos : 슬래브좌측피복위치
double CADeckData::GetCoverSdToModifyAtExp(BOOL bSttSlab, BOOL bLeftCalcPos, CDPoint xyDimStt)
{
	double dStaMatch = 0;
	double dStaSttL = GetStationMainRebarPos(bSttSlab, -1);
	double dStaSttR = GetStationMainRebarPos(bSttSlab,  1);
	double dDistL   = GetSideBarDistFromCenterGir(bSttSlab, TRUE);
	double dDistR   = GetSideBarDistFromCenterGir(bSttSlab, FALSE);
	double dCoverSd = GetLengthCoverSd(bSttSlab); 

	if(bLeftCalcPos)
	{
		dStaSttL = m_pDB->m_pLine->GetStationByDisLength(dStaSttL, (bSttSlab?1:-1)*(dCoverSd+GetWidthBlockOut(bSttSlab, JONGBASEPOS_SLABLEFT)), dDistL);	
		dStaSttR = m_pDB->m_pLine->GetStationByDisLength(dStaSttR, (bSttSlab?1:-1)*(xyDimStt.y+dCoverSd+GetWidthBlockOut(bSttSlab, JONGBASEPOS_SLABRIGHT)), dDistR);	
	}
	else
	{
		dStaSttL = m_pDB->m_pLine->GetStationByDisLength(dStaSttL, (bSttSlab?1:-1)*(xyDimStt.y+dCoverSd+GetWidthBlockOut(bSttSlab, JONGBASEPOS_SLABLEFT)), dDistL);	
		dStaSttR = m_pDB->m_pLine->GetStationByDisLength(dStaSttR, (bSttSlab?1:-1)*(dCoverSd+GetWidthBlockOut(bSttSlab, JONGBASEPOS_SLABRIGHT)), dDistR);	
	}
	CDPoint xyLeft   = m_pDB->m_pLine->GetXyLineDis(dStaSttL, dDistL);
	CDPoint xyRigh   = m_pDB->m_pLine->GetXyLineDis(dStaSttR, dDistR);
	m_pDB->m_pLine->GetStationMatchVerticalByXy(bLeftCalcPos ? xyRigh : xyLeft, dStaMatch);
	CDPoint xyVert = m_pDB->GetGirder(-1)->GetXyGirderDis(dStaMatch, bLeftCalcPos ? dDistL : dDistR);

	return bLeftCalcPos ? ~(xyLeft-xyVert) : ~(xyRigh-xyVert);
}
/*
void CADeckData::IsStringerMatchAtTaper(CStringer *pStg, BOOL &bMatchStt, BOOL &bMatchEnd, CDPoint &xyMatchStt, CDPoint &xyMatchEnd)
{
	if(pStg==NULL)		return;
	
	CPlateBasicIndex	*pBx	 = pStg->GetBx();
	CPlateCrossBeam	*pC		 = pBx->GetCrossBeam();
	CPlateGirderApp	*pGir	 = pBx->GetGirder();
	
	CDPoint xy1 = pGir->GetXyPlanSlabStgBase(pBx, pStg->m_nStg, TRUE);
	CPlateBasicIndex *pBxNext = pGir->GetBxByCrossNo(pGir->GetNumberCrossBeamByBx(pBx)+1);
	CDPoint xy2 = pGir->GetXyPlanSlabStgBase(pBxNext, pStg->m_nStg, FALSE);

	CDPoint A[5], xyResult;
	if(pGir->GetHeightTaper(pBx->GetStation())!=0)
	{
		m_pDB->GetXySss(OFF_STT_HUNCHEND, A);
		for(long n=1; n<5; n++)
		{
			if(GetXyMatchSegAndSeg(xy1, xy2,A[n],A[n-1],xyResult))
			{
				xyMatchStt = xyResult;
				bMatchStt = TRUE;
				break;
			}
		}
	}
	else if(pBxNext && pGir->GetHeightTaper(pBxNext->GetStation())!=0)
	{
		m_pDB->GetXySss(OFF_END_HUNCHEND, A);
		for(long n=1; n<5; n++)
		{
			if(GetXyMatchSegAndSeg(xy1, xy2,A[n],A[n-1],xyResult))
			{
				xyMatchEnd = xyResult;
				bMatchEnd = TRUE;
				break;
			}
		}
	}
}

// pBx : 가로보의 Bx
// nStg : 가로보에서 세로보 위치

long CADeckData::GetStgNoByBx(CPlateBasicIndex *pBx, long nStg)
{
	CStringer *pStg = pBx->GetStringer(nStg);
	long nG = pBx->GetNumberGirder();
	long nC = pBx->GetCrossBeam()->GetSelfCount();
	
	for (long n=0; n<m_arrStringer.GetSize(); n++)
	{
		if(m_arrStringer[n] == CVector(nG, nC, nStg))
			return n;
	}

	return -1;
}


CStringer* CADeckData::GetStgByStgNo(long nStgAtTotal)
{
	CVector vStg = m_arrStringer[nStgAtTotal];
	CStringer *pStg = m_pDB->GetGirder(vStg.x)->GetCrossBeamByCrossNo(vStg.y)->GetBx()->GetStringer(vStg.z);

	return pStg;

}


*/
long CADeckData::GetsuBeryukAtCantilever(long nPos, BOOL bDanbu, BOOL bUpper)
{
	long nRebarsuLeft = 0;
	long nRebarsuRigh = 0;

	CRebarPlacing RebarPlace;
	CDoubleArray dArrDim;
	CDPointArray m_arrCTC = bDanbu ? m_arrCTCBeryukRebarDan[bUpper ? CTC_BERYUK_UPPER : CTC_BERYUK_LOWER] : m_arrCTCBeryukRebarCen[bUpper ? CTC_BERYUK_UPPER : CTC_BERYUK_LOWER];
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTC, dArrDim, TRUE);
	
	long n = 0;
	for (n=0; n<dArrDim.GetSize(); n++)
	{
		if(dArrDim[n] > m_dWidthSlabCantLeftForBeryuk)
			break;
	}
	nRebarsuLeft = n+1;
	if(m_dWidthSlabCantLeftForBeryuk <= 0)	nRebarsuLeft = 0;

	dArrDim.Sort(FALSE);
	double dStt = dArrDim[0];
	for ( n=0; n<dArrDim.GetSize(); n++)
		dArrDim[n] = dStt-dArrDim[n];
	for ( n=0; n<dArrDim.GetSize(); n++)
	{
		if(dArrDim[n] > m_dWidthSlabCantRighForBeryuk)
			break;
	}
	nRebarsuRigh = n;
	if(m_dWidthSlabCantRighForBeryuk <= 0)	nRebarsuRigh = 0;

	long nType = GetTypeBeryukAtCantilever(bDanbu, bUpper);

	if(nType==0)
	{
		if(nPos == POS_SLAB_LEFT)
			return nRebarsuLeft;
		else if(nPos == POS_SLAB_RIGH)
			return nRebarsuRigh;
		else if(nPos == POS_SLAB_CENTER)
			return GetSuRebarArray(&m_arrCTC, TRUE) - nRebarsuLeft - nRebarsuRigh;
	}
	else if(nType==1)
	{
		if(nPos == POS_SLAB_LEFT)
			return 0;
		else if(nPos == POS_SLAB_RIGH)
			return nRebarsuRigh;
		else if(nPos == POS_SLAB_CENTER)
			return GetSuRebarArray(&m_arrCTC, TRUE) - nRebarsuRigh;
	}
	else if(nType==2)
	{
		if(nPos == POS_SLAB_LEFT)
			return nRebarsuLeft;
		else if(nPos == POS_SLAB_RIGH)
			return 0;
		else if(nPos == POS_SLAB_CENTER)
			return GetSuRebarArray(&m_arrCTC, TRUE) - nRebarsuLeft;
	}
	else if(nType==3)
	{
		if(nPos == POS_SLAB_LEFT)
			return 0;
		else if(nPos == POS_SLAB_RIGH)
			return 0;
		else if(nPos == POS_SLAB_CENTER)
			return GetSuRebarArray(&m_arrCTC, TRUE);
	}

	return 0;

}


long CADeckData::GetTypeBeryukAtCantilever(BOOL bDanbu, BOOL bUpper)
{
	long nIdxLeftB = 0;
	long nIdxRighB = 0;
	long nIdxCentB = 0;

	if(bDanbu)
	{
		nIdxLeftB = bUpper ? (long)m_dDiaBeryukRebar_Dan[POS_SLAB_LEFT] : (long)m_dDiaBeryukRebarLower_Dan[POS_SLAB_LEFT];
		nIdxRighB = bUpper ? (long)m_dDiaBeryukRebar_Dan[POS_SLAB_RIGH] : (long)m_dDiaBeryukRebarLower_Dan[POS_SLAB_RIGH];	
		nIdxCentB = bUpper ? (long)m_dDiaBeryukRebar_Dan[POS_SLAB_CENTER] : (long)m_dDiaBeryukRebarLower_Dan[POS_SLAB_CENTER];
	}
	else
	{
		nIdxLeftB = bUpper ? (long)m_dDiaBeryukRebar_Cent[POS_SLAB_LEFT] : (long)m_dDiaBeryukRebarLower_Cent[POS_SLAB_LEFT];
		nIdxRighB = bUpper ? (long)m_dDiaBeryukRebar_Cent[POS_SLAB_RIGH] : (long)m_dDiaBeryukRebarLower_Cent[POS_SLAB_RIGH];	
		nIdxCentB = bUpper ? (long)m_dDiaBeryukRebar_Cent[POS_SLAB_CENTER] : (long)m_dDiaBeryukRebarLower_Cent[POS_SLAB_CENTER];
	}
	if ((nIdxLeftB != nIdxCentB) && (nIdxCentB != nIdxRighB))
		return 0;
	else if ((nIdxLeftB == nIdxCentB) && (nIdxCentB != nIdxRighB))
		return 1;
	else if ((nIdxLeftB != nIdxCentB) && (nIdxCentB == nIdxRighB))
		return 2;
	else
		return 3;		// 모두 동일..
}


double CADeckData::Bogan(double x1, double x2, double y1, double y2, double x)
{
	if(fabs(x1-x2) < 0.001)
		return (y1 + y2) / 2;
	if(fabs(x2-x) < 0.001)
		return y2;
	return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
}




double CADeckData::GetLengthRebarRangeAtJongPos(long nJongBasePos, BOOL bUpper)
{
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);
	
	double dStaSlabSttCen = pGirCen->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
	double dStaSlabEndCen = pGirCen->GetSSS(OFF_END_SLAB);// 기준슬래브의 종점 스테이션
	double dDisLStt		  = m_pDB->GetDisSlabLeftAct(dStaSlabSttCen)+m_dCoverSd;
	double dDisRStt		  = m_pDB->GetDisSlabRighAct(dStaSlabSttCen)-m_dCoverSd;

	CDoubleArray dArrStaLeft = GetStationArrForMainRebarGugan(-1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	CDoubleArray dArrStaRigh = GetStationArrForMainRebarGugan(1);// 시,종점 및 선형곡률변경위치의 중앙 스테이션
	double dSttBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetWidthBlockOut(TRUE, nJongBasePos) : 0;
	double dEndBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetWidthBlockOut(FALSE, nJongBasePos) : 0;
	double dCoverSdStt = GetLengthCoverSd(TRUE, nJongBasePos); 
	double dCoverSdEnd = GetLengthCoverSd(FALSE, nJongBasePos); 

	//////////////////////////////////////////////////////////////////////////
	// 구간별 철근간격계산(하부)
	// 상부는 하부를 이용하여 계산함.
	long nGugansu = dArrStaLeft.GetUpperBound();	// 곡선반경 구간수
	double dTotalLength = 0;
	double dLengthBase  = 0;

	for(long nGugan=0; nGugan<nGugansu; nGugan++)
	{
		double dLengthLeft = m_pDB->m_pLine->GetLengthByDis(dArrStaLeft[nGugan], dArrStaLeft[nGugan+1], dDisLStt);
		double dLengthRigh = m_pDB->m_pLine->GetLengthByDis(dArrStaRigh[nGugan], dArrStaRigh[nGugan+1], dDisRStt);
		
		if(nJongBasePos == JONGBASEPOS_SLABCENTER)
		{
			if (nGugan==0)
			{
				if(nGugansu==1)	dLengthBase = (dLengthLeft+dLengthRigh)/2 - dCoverSdStt - dCoverSdEnd - dSttBlockOutW - dEndBlockOutW;
				else			dLengthBase = (dLengthLeft+dLengthRigh)/2 - dCoverSdStt - dSttBlockOutW;
			}
			else if (nGugan==nGugansu-1)
				dLengthBase = (dLengthLeft+dLengthRigh)/2 - dCoverSdEnd - dEndBlockOutW;
			else
				dLengthBase = (dLengthLeft+dLengthRigh)/2;
		}
		else
		{
			if(nGugan==0)
			{
				if(IsSkewStt())
				{
					double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).GetAngleDegree();
					if(nJongBasePos==JONGBASEPOS_SLABLEFT && ((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360)))
					{
						dCoverSdStt = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT));
						dSttBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT)) : 0;
					}
					else if(nJongBasePos==JONGBASEPOS_SLABRIGHT && ((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270)))
					{
						dCoverSdStt = GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT));
						dSttBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetLengthBySlabEndRadius(dStaSlabSttCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT)) : 0;
					}
				}

				if(IsSkewEnd())
				{
					double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
					if(nJongBasePos==JONGBASEPOS_SLABLEFT && ((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270)))
					{
						dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT));
						dEndBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT)) : 0;
					}
					else if(nJongBasePos==JONGBASEPOS_SLABRIGHT && ((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360)))
					{
						dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT));
						dEndBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT)) : 0;
					}
				}

				if(nGugansu==1)	dLengthBase = (nJongBasePos==JONGBASEPOS_SLABLEFT ? dLengthLeft : dLengthRigh) - dCoverSdStt - dCoverSdEnd - dSttBlockOutW - dEndBlockOutW;
				else			dLengthBase = (nJongBasePos==JONGBASEPOS_SLABLEFT ? dLengthLeft : dLengthRigh) - dCoverSdStt - dSttBlockOutW;
			}
			else if(nGugan==nGugansu-1)
			{
				if(IsSkewEnd())
				{
					double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
					if(nJongBasePos==JONGBASEPOS_SLABLEFT && ((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270)))
					{
						dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT));
						dEndBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABRIGHT, JONGBASEPOS_SLABLEFT, GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT)) : 0;
					}
					else if(nJongBasePos==JONGBASEPOS_SLABRIGHT && ((90<dAngle && dAngle<180) || (270<dAngle && dAngle<360)))
					{
						dCoverSdEnd = GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT));
						dEndBlockOutW = (bUpper&&!m_bInstallMainRebarAtBlockOut) ? GetLengthBySlabEndRadius(dStaSlabEndCen, JONGBASEPOS_SLABLEFT, JONGBASEPOS_SLABRIGHT, GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT)) : 0;
					}
				}
				dLengthBase = (nJongBasePos==JONGBASEPOS_SLABLEFT ? dLengthLeft : dLengthRigh) - dCoverSdEnd - dEndBlockOutW;
			}
			else
				dLengthBase = (nJongBasePos==JONGBASEPOS_SLABLEFT ? dLengthLeft : dLengthRigh);
		}
		dTotalLength += dLengthBase;
	}

	return dTotalLength;
}





double CADeckData::GetStationBySection(long nSection)
{
	double dStaSlabStt = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
	double dStaSlabEnd = m_pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB);
	CDPoint vAngStt	   = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint vAngEnd	   = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);
	double dCoverSdStt = GetLengthCoverSd(TRUE); 
	double dCoverSdEnd = GetLengthCoverSd(FALSE); 
	
	if (nSection == SECTION_DAN || nSection == SECTION_SKEW_STT)
	{
		return dStaSlabStt+dCoverSdStt;
	}
	else if(nSection == SECTION_SKEW_END)
	{
		return dStaSlabEnd-dCoverSdEnd;
	}
	else if(nSection == SECTION_CEN)
	{
		double dStaStt = GetStationMainRebarPos(TRUE, 0) + dCoverSdStt;
		double dDist   = (m_pDB->GetDisSlabLeftAct(dStaStt) + m_pDB->GetDisSlabRighAct(dStaStt))/2;
		double dStaSttCen = m_pDB->m_pLine->GetStationByDisLength(dStaStt, m_dRangeDanbuStt, dDist);

		return dStaSttCen;
	}
	else if(nSection == SECTION_PIER)			return m_pDB->GetGirder(-1)->GetStationOnJijum(1);

	return 0;
}





void CADeckData::RemoveRebarCycleBySection(long nSection)
{
	if	   (nSection == SECTION_DAN)			m_stMainCycleDan.clear();
	else if(nSection == SECTION_CEN)			m_stMainCycleCen.clear();
	else if(nSection == SECTION_PIER)			m_stMainCyclePier.clear();
	else if(nSection == SECTION_SKEW_STT)		m_stMainCycleSabogangStt.clear();
	else if(nSection == SECTION_SKEW_END)		m_stMainCycleSabogangEnd.clear();
}


void CADeckData::AddRebarCycleBySection(long nSection, REBAR_CYCLE stRebarCycle)
{
	if	   (nSection == SECTION_DAN)			m_stMainCycleDan.push_back(stRebarCycle);
	else if(nSection == SECTION_CEN)			m_stMainCycleCen.push_back(stRebarCycle);
	else if(nSection == SECTION_PIER)			m_stMainCyclePier.push_back(stRebarCycle);
	else if(nSection == SECTION_SKEW_STT)		m_stMainCycleSabogangStt.push_back(stRebarCycle);
	else if(nSection == SECTION_SKEW_END)		m_stMainCycleSabogangEnd.push_back(stRebarCycle);
}




REBAR_CYCLE* CADeckData::GetRebarCycleBySection(long nSection, long nCycle)
{
	if	   (nSection == SECTION_DAN)			return &m_stMainCycleDan.at(nCycle);
	else if(nSection == SECTION_CEN)			return &m_stMainCycleCen.at(nCycle);
	else if(nSection == SECTION_PIER)			return &m_stMainCyclePier.at(nCycle);
	else if(nSection == SECTION_SKEW_STT)		return &m_stMainCycleSabogangStt.at(nCycle);
	else if(nSection == SECTION_SKEW_END)		return &m_stMainCycleSabogangEnd.at(nCycle);
	
	return NULL;
}




vector<REBAR_CYCLE>* CADeckData::GetRebarCycleBySection(long nSection)
{
	if	   (nSection == SECTION_DAN)			return &m_stMainCycleDan;
	else if(nSection == SECTION_CEN)			return &m_stMainCycleCen;
	else if(nSection == SECTION_PIER)			return &m_stMainCyclePier;
	else if(nSection == SECTION_SKEW_STT)		return &m_stMainCycleSabogangStt;
	else if(nSection == SECTION_SKEW_END)		return &m_stMainCycleSabogangEnd;
	
	return NULL;
}



long CADeckData::GetRebarCycleSuBySection(long nSection)
{
	if	   (nSection == SECTION_DAN)			return m_stMainCycleDan.size();
	else if(nSection == SECTION_CEN)			return m_stMainCycleCen.size();
	else if(nSection == SECTION_PIER)			return m_stMainCyclePier.size();
	else if(nSection == SECTION_SKEW_STT)		return m_stMainCycleSabogangStt.size();
	else if(nSection == SECTION_SKEW_END)		return m_stMainCycleSabogangEnd.size();

	return 0;
}


void CADeckData::RemoveRebarCycleByJongBasePos(long nJongBasePos)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		m_stBeryukCycleLeft.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		m_stBeryukCycleRigh.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		m_stBeryukCycleCent.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			m_stBeryukCycleLC.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			m_stBeryukCycleRC.clear();
	else if(nJongBasePos == JONGBASEPOS_ONCB)			m_stBeryukCycleOnCB.clear();
}


void CADeckData::AddRebarCycleByJongBasePos(long nJongBasePos, REBAR_CYCLE stRebarCycle)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		m_stBeryukCycleLeft.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		m_stBeryukCycleRigh.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		m_stBeryukCycleCent.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			m_stBeryukCycleLC.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			m_stBeryukCycleRC.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_ONCB)			m_stBeryukCycleOnCB.push_back(stRebarCycle);
}


void CADeckData::AddRebarCycleByJongBasePos(long nJongBasePos, CDoubleArray& arrUpLeftLong, CDoubleArray& arrDnLeftLong
											, CDoubleArray& dArrDiaUpLeft, CDoubleArray& dArrDiaDnLeft)
{
	REBAR_CYCLE stRebarCycle;

	stRebarCycle.m_dArrJointUp = arrUpLeftLong;
	stRebarCycle.m_dArrJointDn = arrDnLeftLong;
	stRebarCycle.m_dArrDiaUp   = dArrDiaUpLeft;
	stRebarCycle.m_dArrDiaDn   = dArrDiaDnLeft;
	
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		m_stBeryukCycleLeft.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		m_stBeryukCycleRigh.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		m_stBeryukCycleCent.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			m_stBeryukCycleLC.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			m_stBeryukCycleRC.push_back(stRebarCycle);
	else if(nJongBasePos == JONGBASEPOS_ONCB)			m_stBeryukCycleOnCB.push_back(stRebarCycle);
}



REBAR_CYCLE* CADeckData::GetRebarCycleByJongBasePos(long nJongBasePos, long nCycle)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		return &m_stBeryukCycleLeft.at(nCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		return &m_stBeryukCycleRigh.at(nCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		return &m_stBeryukCycleCent.at(nCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			return &m_stBeryukCycleLC.at(nCycle);
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			return &m_stBeryukCycleRC.at(nCycle);
	else if(nJongBasePos == JONGBASEPOS_ONCB)			return &m_stBeryukCycleOnCB.at(nCycle);
	
	return NULL;
}


vector<REBAR_CYCLE>* CADeckData::GetRebarCycleByJongBasePos(long nJongBasePos)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		return &m_stBeryukCycleLeft;
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		return &m_stBeryukCycleRigh;
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		return &m_stBeryukCycleCent;
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			return &m_stBeryukCycleLC;
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			return &m_stBeryukCycleRC;
	else if(nJongBasePos == JONGBASEPOS_ONCB)			return &m_stBeryukCycleOnCB;
	
	return NULL;
}


long CADeckData::GetRebarCycleSuByJongBasePos(long nJongBasePos)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		return m_stBeryukCycleLeft.size();
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		return m_stBeryukCycleRigh.size();
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		return m_stBeryukCycleCent.size();
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			return m_stBeryukCycleLC.size();
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			return m_stBeryukCycleRC.size();
	else if(nJongBasePos == JONGBASEPOS_ONCB)			return m_stBeryukCycleOnCB.size();

	return 0;
}


long CADeckData::GetSizeHDanJointBySection(long nSection, long nCycle, BOOL bUpper)
{
	long nReturn = 0;
	if	   (nSection == SECTION_DAN)
		nReturn = bUpper ? m_stMainCycleDan[nCycle].m_dArrJointUp.GetSize() : m_stMainCycleDan[nCycle].m_dArrJointDn.GetSize();
	else if(nSection == SECTION_CEN)	
		nReturn = bUpper ? m_stMainCycleCen[nCycle].m_dArrJointUp.GetSize() : m_stMainCycleCen[nCycle].m_dArrJointDn.GetSize();
	else if(nSection == SECTION_PIER)	
		nReturn = bUpper ? m_stMainCyclePier[nCycle].m_dArrJointUp.GetSize() : m_stMainCyclePier[nCycle].m_dArrJointDn.GetSize();
	else if(nSection == SECTION_SKEW_STT)
		nReturn = bUpper ? m_stMainCycleSabogangStt[nCycle].m_dArrJointUp.GetSize() : m_stMainCycleSabogangStt[nCycle].m_dArrJointDn.GetSize();
	else if(nSection == SECTION_SKEW_END)	
		nReturn = bUpper ? m_stMainCycleSabogangEnd[nCycle].m_dArrJointUp.GetSize() : m_stMainCycleSabogangEnd[nCycle].m_dArrJointDn.GetSize();


	return nReturn;
}



double CADeckData::GetRebarDiaByJongBasePos(long nJongPos, long nJongBasePos, BOOL bUpper)
{
	if(nJongPos==JONGPOS_SLABSTTEND)
	{
		if(nJongBasePos==JONGBASEPOS_SLABLEFT)
			return bUpper ? m_dDiaBeryukRebar_Dan[POS_SLAB_LEFT] : m_dDiaBeryukRebarLower_Dan[POS_SLAB_LEFT];
		if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
			return bUpper ? m_dDiaBeryukRebar_Dan[POS_SLAB_RIGH] : m_dDiaBeryukRebarLower_Dan[POS_SLAB_RIGH];
		if(nJongBasePos==JONGBASEPOS_SLABCENTER)
			return bUpper ? m_dDiaBeryukRebar_Dan[POS_SLAB_CENTER] :m_dDiaBeryukRebarLower_Dan[POS_SLAB_CENTER];
	}
	else if(nJongBasePos==JONGPOS_SLABCENTER)
	{
		if(nJongBasePos==JONGBASEPOS_SLABLEFT)
			return bUpper ? m_dDiaBeryukRebar_Cent[POS_SLAB_LEFT] : m_dDiaBeryukRebarLower_Cent[POS_SLAB_LEFT];
		if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
			return bUpper ? m_dDiaBeryukRebar_Cent[POS_SLAB_RIGH] : m_dDiaBeryukRebarLower_Cent[POS_SLAB_RIGH];
		if(nJongBasePos==JONGBASEPOS_SLABCENTER)
			return bUpper ? m_dDiaBeryukRebar_Cent[POS_SLAB_CENTER] : m_dDiaBeryukRebarLower_Cent[POS_SLAB_CENTER];
	}
	else if(nJongBasePos==JONGPOS_SLABJIJUM)
	{
		if(nJongBasePos==JONGBASEPOS_SLABLEFT)
			return bUpper ? m_dDiaJijumRebarUp[POS_SLAB_LEFT] : m_dDiaJijumRebarDn[POS_SLAB_LEFT];
		if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
			return bUpper ? m_dDiaJijumRebarUp[POS_SLAB_RIGH] : m_dDiaJijumRebarDn[POS_SLAB_RIGH];
		if(nJongBasePos==JONGBASEPOS_SLABCENTER)
			return bUpper ? m_dDiaJijumRebarUp[POS_SLAB_CENTER] : m_dDiaJijumRebarDn[POS_SLAB_CENTER];
	}

	return 0;
}



double CADeckData::GetRebarDiaBySection(long nSection, long nHDanPos, long nCycle, BOOL bUpper)
{
	BOOL bFirstCycle = nCycle%2==0 ? TRUE : FALSE;

	if(nSection==SECTION_DAN)
	{
		if(nHDanPos==POS_SLAB_LEFT)
		{
			if(bFirstCycle)
				return bUpper ? m_dDiaMainRebar_Dan[POS_SLAB_LEFT] : m_dDiaMainRebarLower_Dan[POS_SLAB_LEFT];
			else
				return bUpper ? m_dDiaMainRebar_Dan2nd[POS_SLAB_LEFT] : m_dDiaMainRebarLower_Dan2nd[POS_SLAB_LEFT];
		}
		if(nHDanPos==POS_SLAB_RIGH)
		{
			if(bFirstCycle)
				return bUpper ? m_dDiaMainRebar_Dan[POS_SLAB_RIGH] : m_dDiaMainRebarLower_Dan[POS_SLAB_RIGH];
			else
				return bUpper ? m_dDiaMainRebar_Dan2nd[POS_SLAB_RIGH] : m_dDiaMainRebarLower_Dan2nd[POS_SLAB_RIGH];
		}
		if(nHDanPos==POS_SLAB_CENTER)
		{
			if(bFirstCycle)
				return bUpper ? m_dDiaMainRebar_Dan[POS_SLAB_CENTER] :m_dDiaMainRebarLower_Dan[POS_SLAB_CENTER];
			else
				return bUpper ? m_dDiaMainRebar_Dan2nd[POS_SLAB_CENTER] :m_dDiaMainRebarLower_Dan2nd[POS_SLAB_CENTER];
		}
	}
	else if(nSection==SECTION_CEN || nSection==SECTION_PIER)
	{
		if(nHDanPos==POS_SLAB_LEFT)
		{
			if(bFirstCycle)
				return bUpper ? m_dDiaMainRebar_Cent[POS_SLAB_LEFT] : m_dDiaMainRebarLower_Cent[POS_SLAB_LEFT];
			else
				return bUpper ? m_dDiaMainRebar_Cent2nd[POS_SLAB_LEFT] : m_dDiaMainRebarLower_Cent2nd[POS_SLAB_LEFT];

		}
		if(nHDanPos==POS_SLAB_RIGH)
		{
			if(bFirstCycle)
				return bUpper ? m_dDiaMainRebar_Cent[POS_SLAB_RIGH] : m_dDiaMainRebarLower_Cent[POS_SLAB_RIGH];
			else
				return bUpper ? m_dDiaMainRebar_Cent2nd[POS_SLAB_RIGH] : m_dDiaMainRebarLower_Cent2nd[POS_SLAB_RIGH];
		}
		if(nHDanPos==POS_SLAB_CENTER)
		{
			if(bFirstCycle)
				return bUpper ? m_dDiaMainRebar_Cent[POS_SLAB_CENTER] : m_dDiaMainRebarLower_Cent[POS_SLAB_CENTER];
			else
				return bUpper ? m_dDiaMainRebar_Cent2nd[POS_SLAB_CENTER] : m_dDiaMainRebarLower_Cent2nd[POS_SLAB_CENTER];
		}
	}
	else if(nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END)
	{
		if(nHDanPos==POS_SLAB_LEFT)
		{
			return bUpper ? m_dDiaSabogangUp : m_dDiaSabogangDn;
		}
		if(nHDanPos==POS_SLAB_RIGH)
		{
			return bUpper ? m_dDiaSabogangUp : m_dDiaSabogangDn;
		}
		if(nHDanPos==POS_SLAB_CENTER)
		{
			return bUpper ? m_dDiaSabogangUp : m_dDiaSabogangDn;
		}
	}

	return 0;
}



void CADeckData::RemoveRebarShapeBySection(long nSection)
{
	if	   (nSection == SECTION_DAN)			m_stMainShapeDan.clear();
	else if(nSection == SECTION_CEN)			m_stMainShapeCen.clear();
	else if(nSection == SECTION_PIER)			m_stMainShapePier.clear();
	else if(nSection == SECTION_SKEW_STT)		m_stMainShapeSkewStt.clear();
	else if(nSection == SECTION_SKEW_END)		m_stMainShapeSkewEnd.clear();
}


void CADeckData::AddRebarShapeArrAtSection(long nSection, vector<REBAR_SHAPE> vRebarShape)
{
	if	   (nSection == SECTION_DAN)			m_stMainShapeDan.push_back(vRebarShape);
	else if(nSection == SECTION_CEN)			m_stMainShapeCen.push_back(vRebarShape);
	else if(nSection == SECTION_PIER)			m_stMainShapePier.push_back(vRebarShape);
	else if(nSection == SECTION_SKEW_STT)		m_stMainShapeSkewStt.push_back(vRebarShape);
	else if(nSection == SECTION_SKEW_END)		m_stMainShapeSkewEnd.push_back(vRebarShape);
}




vector<REBAR_SHAPE>* CADeckData::GetRebarShapeArrBySection(long nSection, long nCycle)
{
	if	   (nSection == SECTION_DAN)			return &m_stMainShapeDan.at(nCycle);
	else if(nSection == SECTION_CEN)			return &m_stMainShapeCen.at(nCycle);
	else if(nSection == SECTION_PIER)			return &m_stMainShapePier.at(nCycle);
	else if(nSection == SECTION_SKEW_STT)		return &m_stMainShapeSkewStt.at(nCycle);
	else if(nSection == SECTION_SKEW_END)		return &m_stMainShapeSkewEnd.at(nCycle);

	return NULL;
}


vector<REBAR_SHAPE> CADeckData::GetNewRebarShapeArrBySection(long nSection, long nCycle, BOOL bUpper)
{
	vector<REBAR_SHAPE>* pRebarShape =  GetRebarShapeArrBySection(nSection, nCycle);
	vector<REBAR_SHAPE> RebarShapeReturn;

	for (long n=0; n<(long)pRebarShape->size(); n++)
	{	
		REBAR_SHAPE RebarShape = pRebarShape->at(n);
		if(bUpper && RebarShape.m_bUpper)
			RebarShapeReturn.push_back(RebarShape);

		if(!bUpper && !RebarShape.m_bUpper)
			RebarShapeReturn.push_back(RebarShape);
	}

	return RebarShapeReturn;
}


double CADeckData::GetRatioOfMidPosRebarAtHDan (CTwinVector& tVector, double dSta, BOOL bUpper, double &dRatioPre)
{
	CDPoint xyStt = tVector.m_v1;
	CDPoint xyEnd = tVector.m_v2;

	double dRatio = -1;		// 슬래브좌우끝: 0, 슬래브중앙: 1
	double dStaSlabStt		 = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
	double dWidthHalfSlab	 = m_pDB->GetWidthAct(dStaSlabStt)/2 - m_dCoverSd;
	double dSlabCen			 = m_pDB->GetDisSlabLeftAct(dSta) + m_pDB->GetWidthAct(dStaSlabStt)/2;

	if (bUpper)
	{
		if(xyStt.x<dSlabCen && xyEnd.x<dSlabCen)			// 양끝이 슬래브중심좌측에 있는 경우
			dRatio = (dWidthHalfSlab-(dSlabCen-(xyStt.x+xyEnd.x)/2)) / dWidthHalfSlab;
		else if(xyStt.x<dSlabCen && xyEnd.x>=dSlabCen)		// 시점만 슬래브중심좌측에 있는 경우
			dRatio = (dWidthHalfSlab-(dSlabCen-(xyStt.x+/*xyEnd.x*/dSlabCen)/2)) / dWidthHalfSlab;
	}
	else
	{
		if(xyStt.x>dSlabCen && xyEnd.x>dSlabCen)			// 양끝이 슬래브중심우측에 있는 경우
			dRatio = 1 - (((xyStt.x+xyEnd.x)/2-dSlabCen) / dWidthHalfSlab);
		else if(xyStt.x<=dSlabCen && xyEnd.x>dSlabCen)		// 종점만 슬래브중심우측에 있는 경우
			dRatio = 1 - (((xyEnd.x+/*xyStt.x*/dSlabCen)/2-dSlabCen) / dWidthHalfSlab);
	}
	
	if(dRatioPre==dRatio)
		dRatio += 600/~(xyStt-xyEnd);

	return dRatio;
}




void CADeckData::RemoveRebarShapeByJongBasePos(long nJongBasePos)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		m_stBeryukShapeLeft.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		m_stBeryukShapeRigh.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		m_stBeryukShapeCent.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			m_stBeryukShapeLC.clear();
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			m_stBeryukShapeRC.clear();
	else if(nJongBasePos == JONGBASEPOS_ONCB)			m_stBeryukShapeOnCB.clear();
}


void CADeckData::AddRebarShapeArrAtJongBasePos(long nJongBasePos, vector<REBAR_SHAPE> vRebarShape)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)		m_stBeryukShapeLeft.push_back(vRebarShape);
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		m_stBeryukShapeRigh.push_back(vRebarShape);
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)		m_stBeryukShapeCent.push_back(vRebarShape);
	else if(nJongBasePos == JONGBASEPOS_SLABLC)			m_stBeryukShapeLC.push_back(vRebarShape);
	else if(nJongBasePos == JONGBASEPOS_SLABRC)			m_stBeryukShapeRC.push_back(vRebarShape);
	else if(nJongBasePos == JONGBASEPOS_ONCB)			m_stBeryukShapeOnCB.push_back(vRebarShape);
}


vector<REBAR_SHAPE>* CADeckData::GetRebarShapeArrByJongBasePos(long nJongBasePos, long nCycle)
{
	if	   (nJongBasePos == JONGBASEPOS_SLABLEFT)
	{
		if (m_stBeryukShapeLeft.size()>1)	return &m_stBeryukShapeLeft.at(nCycle);
		else								return NULL;
	}
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)
	{
		if (m_stBeryukShapeRigh.size()>1)	return &m_stBeryukShapeRigh.at(nCycle);
		else								return NULL;
	}
	else if(nJongBasePos == JONGBASEPOS_SLABCENTER)
	{
		if (m_stBeryukShapeCent.size()>1)	return &m_stBeryukShapeCent.at(nCycle);
		else								return NULL;
	}
	else if(nJongBasePos == JONGBASEPOS_SLABLC)	
	{
		if (m_stBeryukShapeLC.size()>1)		return &m_stBeryukShapeLC.at(nCycle);
		else								return NULL;
	}
	else if(nJongBasePos == JONGBASEPOS_SLABRC)	
	{
		if (m_stBeryukShapeRC.size()>1)		return &m_stBeryukShapeRC.at(nCycle);
		else								return NULL;
	}
	else if(nJongBasePos == JONGBASEPOS_ONCB)	
	{
		if (m_stBeryukShapeOnCB.size()>1)	return &m_stBeryukShapeOnCB.at(nCycle);
		else								return NULL;
	}

	return NULL;
}


vector<REBAR_SHAPE> CADeckData::GetNewRebarShapeArrByJongBasePos(long nJongBasePos, long nCycle, BOOL bUpper)
{
	vector<REBAR_SHAPE>* pRebarShape =  GetRebarShapeArrByJongBasePos(nJongBasePos, nCycle);
	vector<REBAR_SHAPE> RebarShapeReturn;
	if(!pRebarShape)	return RebarShapeReturn;

	for (long n=0; n<(long)pRebarShape->size(); n++)
	{	
		REBAR_SHAPE RebarShape = pRebarShape->at(n);
		if(bUpper && RebarShape.m_sName.Find("상") != -1)
			RebarShapeReturn.push_back(RebarShape);

		if(bUpper==FALSE && RebarShape.m_sName.Find("하") != -1)
			RebarShapeReturn.push_back(RebarShape);
	}

	return RebarShapeReturn;
}


CDPointArray CADeckData::GetRebarShapeByJongBasePos(long nJongBasePos, long nCycle, CString szName)		
{
	vector<REBAR_SHAPE>* pRebarShape =  GetRebarShapeArrByJongBasePos(nJongBasePos, nCycle);

	for (long n=0; n<(long)pRebarShape->size(); n++)
	{
		if(pRebarShape->at(n).m_sName == szName)
			return pRebarShape->at(n).m_dArrShape;
	}

	CDPointArray xyArrReturn;
	return xyArrReturn;
}



double CADeckData::GetLengthMainDolchul(long nSection, BOOL bUpper, BOOL bLeft)
{
	if (nSection==SECTION_DAN || nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END)
	{
		if (bUpper)
		{
			if (bLeft)	return m_dLengthMainDolchulUpDan[0];
			else		return m_dLengthMainDolchulUpDan[1];
		}
		else
		{
			if (bLeft)	return m_dLengthMainDolchulLoDan[0];
			else		return m_dLengthMainDolchulLoDan[1];
		}
	}	
	else
	{
		if (bUpper)
		{
			if (bLeft)	return m_dLengthMainDolchulUp[0];
			else		return m_dLengthMainDolchulUp[1];
		}
		else
		{
			if (bLeft)	return m_dLengthMainDolchulLo[0];
			else		return m_dLengthMainDolchulLo[1];
		}
	}
}




// nRangeRebar =-2 : 사보강철근(시점)
// nRangeRebar =-1 : 사각부주철근(시점)
// nRangeRebar = 0 : 전체 철근
// nRangeRebar = 1 : 사보강철근(종점)
// nRangeRebar = 2 : 사각부주철근(종점)
// nRangeRebar = 3 : 주철근
CDoubleArray CADeckData::GetRebarPosAtJongBasePos(BOOL bUpper, long nJongBasePos, BOOL bRealPos, long nRangeRebar)
{
	double	dAngStt		= m_pDB->GetGirder(-1)->GetAngleStt().GetAngleDegree();
	double	dAngEnd		= m_pDB->GetGirder(-1)->GetAngleEnd().GetAngleDegree();
	double	dAng90		= 90;
	BOOL bNotBlockStt		= TRUE;
	BOOL bNotBlockEnd	= TRUE;

	double dSttBlockOutWidth  = GetWidthBlockOut(TRUE, nJongBasePos);
	double dEndBlockOutWidth  = GetWidthBlockOut(FALSE, nJongBasePos);
	double dSttBlockThick = dSttBlockOutWidth;
	double dEndBlockThick = dEndBlockOutWidth;
	
	if(m_bInstallMainRebarAtBlockOut || !bUpper)
	{
		dSttBlockThick = 0;
		dEndBlockThick = 0;				

		if(nJongBasePos == JONGBASEPOS_SLABLEFT)
		{
			if(Compare(dAngStt, dAng90, _T("<"), 0.3))
				bNotBlockStt = FALSE;
			if(Compare(dAngStt, dAng90, _T(">"), 0.3))
				bNotBlockEnd = FALSE;
		}
		else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)
		{
			if(Compare(dAngStt, dAng90, _T(">"), 0.3))
				bNotBlockStt = FALSE;
			if(Compare(dAngStt, dAng90, _T("<"), 0.3))
				bNotBlockEnd = FALSE;
		}
	}



	double dSlabTotalL = GetLengthSlabTotal(nJongBasePos);
	double dL1Stt=0, dL2Stt=0, dL1End=0, dL2End=0;
	double dSttPos=0, dEndPos=0;

	double dCoverSdStt = GetLengthCoverSd(TRUE);
	double dCoverSdEnd = GetLengthCoverSd(FALSE);
	CDoubleArray dArrRebarPos;
	CDPointArray xyArrDim;
	
	dL2Stt = GetLengthMainRebarToCover(TRUE, nJongBasePos, bUpper, dL1Stt, TRUE);
	dL2End = GetLengthMainRebarToCover(FALSE, nJongBasePos, bUpper, dL1End, TRUE);

	dSttPos = dCoverSdStt+dSttBlockThick;
	dEndPos = dSlabTotalL-dCoverSdEnd-dEndBlockThick;

	double dLastSttPos = dSttPos;
	double dLastEndPos = dEndPos;

	if(nRangeRebar==0)
		dArrRebarPos.Add(dSttPos);
	
	if(dL2Stt > 0)
	{
		xyArrDim = m_arrCTCSabogangRebar[bUpper?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_LOWER_STT];
		for (long n=0; n<xyArrDim.GetSize(); n++)
		{
			for (long nn=0; nn<xyArrDim[n].x; nn++)
			{
				dSttPos += xyArrDim[n].y;
				if(dSttPos < dL2Stt+dCoverSdStt+dSttBlockOutWidth)
				{
					if(nRangeRebar==0 || nRangeRebar==-2)
						dArrRebarPos.Add(dSttPos);
				}
			}
		}
	}
	
	if(dL1Stt > 0 || bNotBlockStt == FALSE)
	{
		dSttPos = dCoverSdStt+dSttBlockOutWidth+dL1Stt+dL2Stt;
		
		if(nRangeRebar==0 || nRangeRebar==-1)
			dArrRebarPos.Add(dSttPos);

		if(bRealPos)
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT)			xyArrDim = m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_STT];
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)	xyArrDim = m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_STT];
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)	xyArrDim = m_arrCTCMainRebarAtSkewJong[bUpper?CTC_SKEWJONG_UPPER_CENTER_STT:CTC_SKEWJONG_LOWER_CENTER_STT];
		}
		else
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT)			xyArrDim = m_arrCTCMainRebarAtSkewForDim[bUpper?CTC_SKEWMAIN_UPPER_LEFT_STT:CTC_SKEWMAIN_LOWER_LEFT_STT];
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)	xyArrDim = m_arrCTCMainRebarAtSkewForDim[bUpper?CTC_SKEWMAIN_UPPER_RIGH_STT:CTC_SKEWMAIN_LOWER_RIGH_STT];
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)	xyArrDim = m_arrCTCMainRebarAtSkewJongForDim[bUpper?CTC_SKEWJONG_UPPER_CENTER_STT:CTC_SKEWJONG_LOWER_CENTER_STT];
		}

		

		for (long n=0; n<xyArrDim.GetSize(); n++)
		{
			for (long nn=0; nn<xyArrDim[n].x; nn++)
			{
				dSttPos -= xyArrDim[n].y;

				if(bNotBlockStt  == FALSE && dSttPos < dLastSttPos)
					continue;

				if(nRangeRebar==0 || nRangeRebar==-1)
					dArrRebarPos.Add(dSttPos);
			}
		}
	}


	dSttPos = dCoverSdStt+dSttBlockOutWidth+dL1Stt+dL2Stt;

	if(nJongBasePos==JONGBASEPOS_SLABLEFT)			xyArrDim = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_LEFT:CTC_MAIN_LOWER_LEFT];
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)	xyArrDim = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_RIGHT:CTC_MAIN_LOWER_RIGHT];
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)	xyArrDim = m_arrCTCMainRebar[bUpper?CTC_MAIN_UPPER_CENTER:CTC_MAIN_LOWER_CENTER];
	
	for (long n=0; n<xyArrDim.GetSize(); n++)
	{
		CDPoint xyDim = xyArrDim[n];
		for (long nn=0; nn<xyDim.x; nn++)
		{
			dSttPos += xyDim.y;
			if(nRangeRebar==0 || nRangeRebar==3)
				dArrRebarPos.Add(dSttPos);
		}
	}

	if(dL1End > 0 || bNotBlockEnd == FALSE)
	{
		if(bNotBlockEnd == FALSE)
			dEndPos = dSlabTotalL-dCoverSdEnd-dEndBlockOutWidth-dL1End-dL2End;
		else
			dEndPos = dSlabTotalL-dCoverSdEnd-dEndBlockThick-dL1End-dL2End;
		if(nRangeRebar==0 || nRangeRebar==1)
			dArrRebarPos.Add(dEndPos);

		if(bRealPos)
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT)			xyArrDim = m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_LEFT_END:CTC_SKEWMAIN_LOWER_LEFT_END];
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)	xyArrDim = m_arrCTCMainRebarAtSkew[bUpper?CTC_SKEWMAIN_UPPER_RIGH_END:CTC_SKEWMAIN_LOWER_RIGH_END];
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)	xyArrDim = m_arrCTCMainRebarAtSkewJong[bUpper?CTC_SKEWJONG_UPPER_CENTER_END:CTC_SKEWJONG_LOWER_CENTER_END];
		}
		else
		{
			if(nJongBasePos==JONGBASEPOS_SLABLEFT)			xyArrDim = m_arrCTCMainRebarAtSkewForDim[bUpper?CTC_SKEWMAIN_UPPER_LEFT_END:CTC_SKEWMAIN_LOWER_LEFT_END];
			else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)	xyArrDim = m_arrCTCMainRebarAtSkewForDim[bUpper?CTC_SKEWMAIN_UPPER_RIGH_END:CTC_SKEWMAIN_LOWER_RIGH_END];
			else if(nJongBasePos==JONGBASEPOS_SLABCENTER)	xyArrDim = m_arrCTCMainRebarAtSkewJongForDim[bUpper?CTC_SKEWJONG_UPPER_CENTER_END:CTC_SKEWJONG_LOWER_CENTER_END];
		}

		for (long n=0; n<xyArrDim.GetSize(); n++)
		{
			for (long nn=0; nn<xyArrDim[n].x; nn++)
			{
				dEndPos += xyArrDim[n].y;

				if(bNotBlockEnd == FALSE && dEndPos > dLastEndPos)
					continue;

				if(nRangeRebar==0 || nRangeRebar==1)
					dArrRebarPos.Add(dEndPos);
			}
		}
	}

	if(dL2End > 0)
	{
		dEndPos = dSlabTotalL-dCoverSdEnd-dEndBlockThick;
		if(nRangeRebar==0 || nRangeRebar==2)
			dArrRebarPos.Add(dEndPos);
		xyArrDim = m_arrCTCSabogangRebar[bUpper?CTC_SABOGANG_UPPER_END:CTC_SABOGANG_LOWER_END];
		
		for (long n=0; n<xyArrDim.GetSize(); n++)
		{
			CDPoint xyDim = xyArrDim[n];
			for (long nn=0; nn< xyDim.x; nn++)
			{
				dEndPos -= xyDim.y;
				if(dEndPos > dSlabTotalL-dL2End-dCoverSdEnd-dEndBlockThick)
				{
					if(nRangeRebar==0 || nRangeRebar==2)
						dArrRebarPos.Add(dEndPos);
				}
			}
		}
	}


	dArrRebarPos.Sort();

	return dArrRebarPos;
}



long CADeckData::GetRebarSu(CDPointArray xyArrDim)
{
	CDoubleArray dArrDim;
	CRebarPlacing RebarPlacing;
	RebarPlacing.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDim, dArrDim, TRUE);

	long nRebar = 1;
	for(long n=0; n<dArrDim.GetSize(); n++)
		nRebar++;

	return nRebar;
}



void CADeckData::GetTwinVectorArrByCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycle, long nShape)
{
	tvArrReturn->RemoveAll();
	for (long n=0; n<(long)vPlan->size(); n++)
	{
		if ( vPlan->at(n).m_nCycle==nCycle && vPlan->at(n).m_nShape==nShape)
			tvArrReturn->Add(vPlan->at(n).m_tVector);
	}
}




void CADeckData::GetTwinVectorArrAtRebarPlan(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn)
{
	tvArrReturn->RemoveAll();
	for (long n=0; n<(long)vPlan->size(); n++)
	{
		tvArrReturn->Add(vPlan->at(n).m_tVector);
	}

}


void CADeckData::GetTwinVectorArrByCycleAndName(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycle, CString sName)
{
	CString sNameCur;
	tvArrReturn->RemoveAll();
	for (long n=0; n<(long)vPlan->size(); n++)
	{
		if ( vPlan->at(n).m_nCycle==nCycle && vPlan->at(n).m_sName==sName)
			tvArrReturn->Add(vPlan->at(n).m_tVector);
	}

}


void CADeckData::GetTwinVectorArrByMultiCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycleStt, long nCycleEnd, long nShape)
{
	tvArrReturn->RemoveAll();
	for (long n=0; n<(long)vPlan->size(); n++)
	{
		for (long nCycle=nCycleStt; nCycle<=nCycleEnd; nCycle++)
		{
			if ( vPlan->at(n).m_nCycle==nCycle && vPlan->at(n).m_nShape==nShape)
				tvArrReturn->Add(vPlan->at(n).m_tVector);
		}
	}
}


void CADeckData::GetTwinVectorArrByMultiCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycleStt, long nCycleEnd, CString sName)
{
	CString sNameCur;
	tvArrReturn->RemoveAll();
	for (long n=0; n<(long)vPlan->size(); n++)
	{
		for (long nCycle=nCycleStt; nCycle<=nCycleEnd; nCycle++)
		{
			if ( vPlan->at(n).m_nCycle==nCycle && vPlan->at(n).m_sName==sName)
			tvArrReturn->Add(vPlan->at(n).m_tVector);
		}
	}
}



BOOL CADeckData::IsCycleValid(long nSection)
{
	if(nSection==SECTION_DAN	  && m_stMainCycleDan.size()>0)		return TRUE;		
	else if(nSection==SECTION_CEN     && m_stMainCycleCen.size()>0)		return TRUE;
	else if(nSection==SECTION_PIER	  && m_stMainCyclePier.size()>0)		return TRUE;	
	else if(nSection==SECTION_SKEW_STT&& m_stMainCycleSabogangStt.size()>0)		return TRUE;
	else if(nSection==SECTION_SKEW_END&& m_stMainCycleSabogangEnd.size()>0)		return TRUE;

	return FALSE;
}




BOOL CADeckData::IsShapeValid(long nSection)
{
	if(nSection==SECTION_DAN		&& m_stMainShapeDan.size()>0)		return TRUE;		
	else if(nSection==SECTION_PIER	  && m_stMainShapePier.size()>0)		return TRUE;
	else if(nSection==SECTION_SKEW_STT&& m_stMainShapeSkewStt.size()>0)		return TRUE;
	else if(nSection==SECTION_SKEW_END&& m_stMainShapeSkewEnd.size()>0)		return TRUE;

	return FALSE;
}



BOOL CADeckData::IsCycleValidJong(long nJongBasePos)
{
	if(nJongBasePos==JONGBASEPOS_SLABLEFT && m_stBeryukCycleLeft.size()>0)			return TRUE;		
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT&& m_stBeryukCycleRigh.size()>0)		return TRUE;
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER && m_stBeryukCycleCent.size()>0)	return TRUE;
	else if(nJongBasePos==JONGBASEPOS_SLABLC && m_stBeryukCycleLC.size()>0)			return TRUE;
	else if(nJongBasePos==JONGBASEPOS_SLABRC && m_stBeryukCycleRC.size()>0)			return TRUE;
	else if(nJongBasePos==JONGBASEPOS_ONCB && m_stBeryukCycleOnCB.size()>0)			return TRUE;

	return FALSE;
}




BOOL CADeckData::IsShapeValidJong(long nJongBasePos)
{
	if(nJongBasePos==JONGBASEPOS_SLABLEFT && m_stBeryukShapeLeft.size()>0)			return TRUE;		
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT && m_stBeryukShapeRigh.size()>0)	return TRUE;
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER && m_stBeryukShapeCent.size()>0)	return TRUE;
	else if(nJongBasePos==JONGBASEPOS_SLABLC && m_stBeryukShapeLC.size()>0)			return TRUE;
	else if(nJongBasePos==JONGBASEPOS_SLABRC && m_stBeryukShapeRC.size()>0)			return TRUE;
	else if(nJongBasePos==JONGBASEPOS_ONCB && m_stBeryukShapeOnCB.size()>0)			return TRUE;

	return FALSE;
}



// pt1
// TRUE  : 그려짐
// FALSE : Trim됨
BOOL CADeckData::IsDrawArea(CDPoint pt1)
{
	double dStaMatch = 0;
	
	m_pDB->m_pLine->GetStationMatchVerticalByXy(pt1, dStaMatch);
	
	double dStaBridgeStt = frM(m_pDB->m_dStationBridgeStt) + m_pDB->m_dLengthUganStt;
	double dStaBridgeEnd = dStaBridgeStt + frM(m_pDB->m_dLengthTotalSlab);
	double dStaSttCheck = 0 , dStaEndCheck = 0;
	double dStation = dStaBridgeStt;
	
	if(m_arrTrimRange.GetSize() < 2)	// Trim구간이 전체일 경우
		return TRUE;

	for(long n=0; n<m_arrTrimRange.GetSize(); n++)
	{		
		dStaSttCheck = dStation;
		dStaEndCheck = (n==m_arrTrimRange.GetUpperBound()) ? 			 
						dStaBridgeEnd:
						dStation+m_arrTrimRange.GetAt(n);
		if(n==0)
		{
			if(dStaMatch <= dStaEndCheck) return TRUE;
		}
		else if(n==m_arrTrimRange.GetUpperBound())
		{
			if(dStaMatch >= dStaSttCheck) return TRUE;
		}
		else if(n%2==0 &&
		   dStaMatch >= dStaSttCheck && 
		   dStaMatch <= dStaEndCheck) return TRUE;
		dStation += m_arrTrimRange.GetAt(n);					
	}			
	
	return FALSE;
}

// 거더별 지간길이를 구함
vector<CDoubleArray> CADeckData::GetJiganLenArr()
{
	vector<CDoubleArray> vLenJigan;
	for (long nG=0; nG < m_pDB->GetGirdersu(); nG++)
	{
		CDoubleArray dArrLen;
		for (long nJigan=0; nJigan < m_pDB->m_nQtyJigan; nJigan++)
		{
			CPlateGirderApp	*pGir  = m_pDB->GetGirder(nG);
			CPlateBasicIndex	*pBxStt= pGir->GetBxOnJijum(nJigan);	
			CPlateBasicIndex	*pBxEnd= pGir->GetBxOnJijum(nJigan+1);	
			double dLengthJigan = pGir->GetLengthDimType(pBxStt->GetStation(), pBxEnd->GetStation());
			dArrLen.Add(dLengthJigan);
		}
		vLenJigan.push_back(dArrLen);
	}

	return vLenJigan;
}


// 좌측[0], 우측[1], 중앙[2] 
// x: 시점, y:종점
CDPointArray CADeckData::GetDanbuJointPos()
{
	CDPointArray arrReturn;

	// 기본길이
	double dLengthPlus = 2000;	// 추가길이
	double dSttExpWLeft = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
	double dEndExpWLeft = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT);
	double dSttExpWRigh = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
	double dEndExpWRigh = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdStt  = GetLengthCoverSd(TRUE);
	double dCoverSdEnd  = GetLengthCoverSd(FALSE);
	double dLenTaperStt = m_pDB->m_dHeightTaperStt*5 + (m_pDB->m_dThickSlabBase-m_dCoverUp-m_dCoverDn)*5;
	double dLenTaperEnd = m_pDB->m_dHeightTaperEnd*5 + (m_pDB->m_dThickSlabBase-m_dCoverUp-m_dCoverDn)*5;
	double dLengthSttLeft = GetLengthSlabToTaper(TRUE,  JONGBASEPOS_SLABLEFT) - dCoverSdStt - dSttExpWLeft + dLenTaperStt + dLengthPlus;
	double dLengthEndLeft = GetLengthSlabToTaper(FALSE, JONGBASEPOS_SLABLEFT) - dCoverSdEnd - dEndExpWLeft + dLenTaperEnd + dLengthPlus;
	double dLengthSttRigh = GetLengthSlabToTaper(TRUE, JONGBASEPOS_SLABRIGHT) - dCoverSdStt - dSttExpWRigh + dLenTaperStt + dLengthPlus;
	double dLengthEndRigh = GetLengthSlabToTaper(FALSE,JONGBASEPOS_SLABRIGHT) - dCoverSdEnd - dEndExpWRigh + dLenTaperEnd + dLengthPlus;
	double dLengthSttCent = (dLengthSttLeft+dLengthSttRigh)/2;
	double dLengthEndCent = (dLengthEndLeft+dLengthEndRigh)/2;

	arrReturn.Add(CDPoint(dLengthSttLeft, dLengthEndLeft));
	arrReturn.Add(CDPoint(dLengthSttRigh, dLengthEndRigh));
	arrReturn.Add(CDPoint(dLengthSttCent, dLengthEndCent));
 
	return arrReturn;
}

void CADeckData::MakeCycleBeryukRebar(CDoubleArray arrMomentLeft, CDoubleArray arrMomentRigh)		
{
 	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);
	long nGirEnd = m_pDB->GetGirdersu()-1;
	BOOL bOpenOutput = arrMomentLeft.GetSize() > m_pDB->m_nQtyJigan ? TRUE : FALSE;
	if(bOpenOutput==FALSE)	
		AfxMessageBox("모델링결과 파일을 읽은 후 지점보강철근 이음위치를 계산할 수 있습니다.\n지간길이의 1/4로 이음위치를 설정합니다.");
	else
	{
		if(arrMomentLeft.GetSize() != arrMomentRigh.GetSize())
		{
			AfxMessageBox("모델링결과 파일이 비정상적입니다.");
			return;
		}
	}
	RemoveRebarCycleByJongBasePos(JONGBASEPOS_SLABLEFT);
	RemoveRebarCycleByJongBasePos(JONGBASEPOS_SLABRIGHT);
	RemoveRebarCycleByJongBasePos(JONGBASEPOS_SLABCENTER);

	// 신축이음폭	
	double dSttExpWLeft = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT);
	double dSttExpWRigh = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT);
	double dSttExpWCent = m_bInstallBeryukRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE, JONGBASEPOS_SLABCENTER);

	// 지간장
	vector<CDoubleArray> vLenJigan = GetJiganLenArr();

	// 이음장 POS_SLAB_LEFT(0), POS_SLAB_RIGH(1), POS_SLAB_CENTER(2)
	double dLJointDanGenUp[3], dLJointDanGenLo[3], dLJointGenJijumUp[3], dLJointGenJijumLo[3];
	double dSigmaCK = toKgPCm2(m_SigmaCK);
	double dSigmaY  = toKgPCm2(m_SigmaY);
	long   nJointPos = JOINT_TENS_NORMAL_B;
	for (long nPos=0; nPos<3; nPos++)	
	{
		double dLJointDanUp		= GetValueJoint(dSigmaCK, dSigmaY, m_dDiaBeryukRebar_Dan[nPos],		nJointPos, toCm(m_dCoverUp), toCm(m_dEndSupportRebarCTC[nPos]));
		double dLJointDanLo		= GetValueJoint(dSigmaCK, dSigmaY, m_dDiaBeryukRebarLower_Dan[nPos],nJointPos, toCm(m_dCoverDn), toCm(m_dEndSupportRebarCTCLower[nPos]));
		double dLJointGenUp		= GetValueJoint(dSigmaCK, dSigmaY, m_dDiaBeryukRebar_Cent[nPos],	nJointPos, toCm(m_dCoverUp), toCm(m_dGenSupportRebarCTC[nPos]));
		double dLJointGenLo		= GetValueJoint(dSigmaCK, dSigmaY, m_dDiaBeryukRebarLower_Cent[nPos],nJointPos,toCm(m_dCoverDn), toCm(m_dGenSupportRebarCTCLower[nPos]));
		double dLJointJijumUp	= GetValueJoint(dSigmaCK, dSigmaY, m_dDiaJijumRebarUp[nPos],		nJointPos, toCm(m_dCoverUp), toCm(m_dJijumRebarCTCUp[nPos]));
		double dLJointJijumLo	= GetValueJoint(dSigmaCK, dSigmaY, m_dDiaJijumRebarDn[nPos],		nJointPos, toCm(m_dCoverDn), toCm(m_dJijumRebarCTCDn[nPos]));
		
		dLJointDanGenUp[nPos]	= max(dLJointDanUp, dLJointGenUp);
		dLJointDanGenLo[nPos]	= max(dLJointDanLo, dLJointGenLo);
		dLJointGenJijumUp[nPos] = max(dLJointJijumUp, dLJointGenUp);
		dLJointGenJijumLo[nPos] = max(dLJointJijumLo, dLJointGenLo);
	}
	
	// 단부철근 시작점에서 끝점까지의 길이 구하기
	CDPointArray xyArrLen = GetDanbuJointPos();
	double dLengthSttJointLeft = xyArrLen[0].x;
	double dLengthEndJointLeft = xyArrLen[0].y;
	double dLengthSttJointRigh = xyArrLen[1].x;
	double dLengthEndJointRigh = xyArrLen[1].y;
	double dLengthSttJointCen  = xyArrLen[2].x;
	double dLengthEndJointCen  = xyArrLen[2].y;

	//  지점에서 이음위치까지의 거리 구하기(교량중심기준)
	CDoubleArray arrUpLeftLong,arrUpLeftSmal;		// 슬래브좌측에서 이음위치가 거리(x:겹이음구간, y:스테이션)
	CDoubleArray arrDnLeftLong,arrDnLeftSmal;
	CDoubleArray arrUpRighLong,arrUpRighSmal;		// 슬래브우측에서 이음위치가 거리
	CDoubleArray arrDnRighLong,arrDnRighSmal;
	CDoubleArray arrUpCenLong, arrUpCenSmal;		// 슬래브중심에서 이음위치가 거리
	CDoubleArray arrDnCenLong, arrDnCenSmal;
	CDoubleArray dArrDiaUpLeft, dArrDiaUpRigh, dArrDiaUpCent; // 철근직경
	CDoubleArray dArrDiaDnLeft, dArrDiaDnRigh, dArrDiaDnCent; 
	vector <CDoubleArray> vArrValid;				// 유/무(사이클별) -  권고안 계산시 상,하부  좌측, 우측, 중앙부 동일함.

	CDoubleArray dArrLarge, dArrSmall, dArrLargeUp, dArrSmallUp, dArrLargeDn, dArrSmallDn;
	double dSpaceJoint = 600;		// 이음장간 간격
	double dDisLeftJointSttGirLong = 0;	// 지점에서 좌측이음까지의 거리(긴것)
	double dDisRighJointSttGirLong = 0;	// 지점에서 우측이음까지의 거리(긴것)
	double dDisCentJointSttGirLong = 0;	// 지점에서 중앙이음까지의 거리(긴것)
	double dDisLeftJointEndGirLong = 0;	// 지점에서 좌측이음까지의 거리(긴것)
	double dDisRighJointEndGirLong = 0;	// 지점에서 우측이음까지의 거리(긴것)
	double dDisCentJointEndGirLong = 0;	// 지점에서 중앙이음까지의 거리(긴것)
	double dDisLeftJointSttGirSmal = 0;	// 지점에서 좌측이음까지의 거리(작은것)
	double dDisRighJointSttGirSmal = 0;	// 지점에서 우측이음까지의 거리(작은것)
	double dDisCentJointSttGirSmal = 0;	// 지점에서 중앙이음까지의 거리(작은것)
	double dDisLeftJointEndGirSmal = 0;	// 지점에서 좌측이음까지의 거리(작은것)
	double dDisRighJointEndGirSmal = 0;	// 지점에서 우측이음까지의 거리(작은것)
	double dDisCentJointEndGirSmal = 0;	// 지점에서 중앙이음까지의 거리(작은것)
	CDPointArray xyArrUp = GetXyArrGuideLineForBeryukRebar(JONGBASEPOS_SLABCENTER, TRUE);
	CDPointArray xyArrDn = GetXyArrGuideLineForBeryukRebar(JONGBASEPOS_SLABCENTER, FALSE);
	double dLengthRebarUp = fabs(xyArrUp[0].x-xyArrUp[xyArrUp.GetUpperBound()].x);
	double dLengthRebarDn = fabs(xyArrDn[0].x-xyArrDn[xyArrDn.GetUpperBound()].x);

	// 
	CDPointArray dArrStaLargeUp, dArrStaSmallUp, dArrStaLargeDn, dArrStaSmallDn;	// x:시점, y:종점	(지점기준)
	CDPoint xySta;
	double dDistLeft = GetSideBarDistFromCenterGir(SECTION_PIER, TRUE);
	double dDistRigh = GetSideBarDistFromCenterGir(SECTION_PIER, FALSE);
	double dDistCen = (dDistLeft + dDistRigh) /2;
	
	long n = 0;
	for (n=0; n<=m_pDB->m_nQtyJigan; n++)
	{
		if(n==0)
		{
			// 상부 슬래브 중앙
			dArrLargeUp.Add(0);
			dArrSmallUp.Add(0);
			dArrLargeUp.Add(dLengthSttJointCen);
			dArrSmallUp.Add(dLengthSttJointCen);

			// 하부 슬래브 중앙
			dArrLargeDn.Add(0);
			dArrSmallDn.Add(0);
			dArrLargeDn.Add(dLengthSttJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2);							
			dArrSmallDn.Add(dLengthSttJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2);			
		}
		else if(n==m_pDB->m_nQtyJigan)
		{
			// 상부 슬래브 중앙
			dArrLargeUp.Add(dLengthRebarUp - dLengthEndJointCen);
			dArrSmallUp.Add(dLengthRebarUp - dLengthEndJointCen);
			dArrLargeUp.Add(dLengthRebarUp); 
			dArrSmallUp.Add(dLengthRebarUp);

			// 하부 슬래브 중앙
			dArrLargeDn.Add(dLengthRebarDn - (dLengthEndJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2));		
			dArrSmallDn.Add(dLengthRebarDn - (dLengthEndJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2));		
			dArrLargeDn.Add(dLengthRebarDn); 
			dArrSmallDn.Add(dLengthRebarDn);
		}
		else
		{
			// 상부 슬래브 중앙
			double dDistJijumFrStt = GetLengthSlabAccuml(n-1, JONGBASEPOS_SLABCENTER, TRUE) - GetLengthCoverSd(TRUE) - dSttExpWCent;
			double dLenSpanPreL = vLenJigan[0].GetAt(n-1);
			double dLenSpanPstL = vLenJigan[0].GetAt(n);
			double dLenSpanPreR = vLenJigan[nGirEnd].GetAt(n-1);
			double dLenSpanPstR = vLenJigan[nGirEnd].GetAt(n);

			dDisLeftJointSttGirLong = bOpenOutput ? fabs(arrMomentLeft[(n-1)*3+2]-arrMomentLeft[(n-1)*3+1]) + dLenSpanPreL/16 - dLJointGenJijumUp[0]/2 : (dLenSpanPreL/4 + dLenSpanPreL/16 - dLJointGenJijumUp[0]/2);
			dDisRighJointSttGirLong = bOpenOutput ? fabs(arrMomentLeft[(n-1)*3+3]-arrMomentLeft[(n-1)*3+2]) + dLenSpanPstL/16 - dLJointGenJijumUp[1]/2 : (dLenSpanPstL/4 + dLenSpanPstL/16 - dLJointGenJijumUp[1]/2);
			
			dDisLeftJointEndGirLong = bOpenOutput ? fabs(arrMomentRigh[(n-1)*3+2]-arrMomentRigh[(n-1)*3+1]) + dLenSpanPreR/16 - dLJointGenJijumUp[0]/2 : (dLenSpanPreR/4 + dLenSpanPreR/16 - dLJointGenJijumUp[0]/2);
			dDisRighJointEndGirLong = bOpenOutput ? fabs(arrMomentRigh[(n-1)*3+3]-arrMomentRigh[(n-1)*3+2]) + dLenSpanPstR/16 - dLJointGenJijumUp[1]/2 : (dLenSpanPstR/4 + dLenSpanPstR/16 - dLJointGenJijumUp[1]/2);
			
			dDisLeftJointSttGirSmal = bOpenOutput ? fabs(arrMomentLeft[(n-1)*3+2]-arrMomentLeft[(n-1)*3+1])  				  + dLJointGenJijumUp[0]/2 : (dLenSpanPreL/4 + dLJointGenJijumUp[0]/2);
			dDisRighJointSttGirSmal = bOpenOutput ? fabs(arrMomentLeft[(n-1)*3+3]-arrMomentLeft[(n-1)*3+2])  				  + dLJointGenJijumUp[1]/2 : (dLenSpanPstL/4 + dLJointGenJijumUp[1]/2);
			
			dDisLeftJointEndGirSmal = bOpenOutput ? fabs(arrMomentRigh[(n-1)*3+2]-arrMomentRigh[(n-1)*3+1])  				  + dLJointGenJijumUp[0]/2 : (dLenSpanPreR/4 + dLJointGenJijumUp[0]/2);
			dDisRighJointEndGirSmal = bOpenOutput ? fabs(arrMomentRigh[(n-1)*3+3]-arrMomentRigh[(n-1)*3+2])  				  + dLJointGenJijumUp[1]/2 : (dLenSpanPstR/4 + dLJointGenJijumUp[1]/2);
			
			dDisCentJointSttGirLong = (dDisLeftJointSttGirLong+dDisLeftJointEndGirLong)/2;
			dDisCentJointEndGirLong = (dDisRighJointSttGirLong+dDisRighJointEndGirLong)/2;
			dDisCentJointSttGirSmal = (dDisLeftJointSttGirSmal+dDisLeftJointEndGirSmal)/2;
			dDisCentJointEndGirSmal = (dDisRighJointSttGirSmal+dDisRighJointEndGirSmal)/2;

			dArrLargeUp.Add(dDistJijumFrStt-dDisCentJointSttGirLong);
			dArrLargeUp.Add(dDistJijumFrStt);
			dArrLargeUp.Add(dDistJijumFrStt+dDisCentJointEndGirLong);
			
			dArrSmallUp.Add(dDistJijumFrStt-dDisCentJointSttGirSmal);
			dArrSmallUp.Add(dDistJijumFrStt);
			dArrSmallUp.Add(dDistJijumFrStt+dDisCentJointEndGirSmal);

			double dStaJijum = pGirCen->GetStationOnJijum(n);

			xySta.x = m_pDB->m_pLine->GetStationByDisLength(dStaJijum, -dDisCentJointSttGirLong, dDistCen);
			xySta.y = m_pDB->m_pLine->GetStationByDisLength(dStaJijum,  dDisCentJointEndGirLong, dDistCen);
			dArrStaLargeUp.Add(xySta);
			xySta.x = m_pDB->m_pLine->GetStationByDisLength(dStaJijum, -dDisCentJointSttGirSmal, dDistCen);
			xySta.y = m_pDB->m_pLine->GetStationByDisLength(dStaJijum,  dDisCentJointEndGirSmal, dDistCen);
			dArrStaSmallUp.Add(xySta);

			// 하부 슬래브 중앙
			double dLargeStt = (dDisCentJointSttGirLong + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);
			double dLargeEnd = (dDisCentJointEndGirLong + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);
			double dSmallStt = (dDisCentJointSttGirSmal + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);
			double dSmallEnd = (dDisCentJointEndGirSmal + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);

			dArrLargeDn.Add(dDistJijumFrStt-dLargeStt);
			dArrLargeDn.Add(dDistJijumFrStt);
			dArrLargeDn.Add(dDistJijumFrStt+dLargeEnd);

			dArrSmallDn.Add(dDistJijumFrStt-dSmallStt);
			dArrSmallDn.Add(dDistJijumFrStt);
			dArrSmallDn.Add(dDistJijumFrStt+dSmallEnd);

			xySta.x = m_pDB->m_pLine->GetStationByDisLength(dStaJijum, -dLargeStt, dDistCen);
			xySta.y = m_pDB->m_pLine->GetStationByDisLength(dStaJijum,  dLargeEnd, dDistCen);
			dArrStaLargeDn.Add(xySta);
			xySta.x = m_pDB->m_pLine->GetStationByDisLength(dStaJijum, -dSmallStt, dDistCen);
			xySta.y = m_pDB->m_pLine->GetStationByDisLength(dStaJijum,  dSmallEnd, dDistCen);
			dArrStaSmallDn.Add(xySta);
		}
	}

	dArrLargeUp.Sort();
	dArrSmallUp.Sort();

	for (n=0; n<dArrLargeUp.GetSize()-1; n++)
		arrUpCenLong.Add(dArrLargeUp[n+1]-dArrLargeUp[n]);
	for (n=0; n<dArrSmallUp.GetSize()-1; n++)
		arrUpCenSmal.Add(dArrSmallUp[n+1]-dArrSmallUp[n]);

	// 하부 슬래브 중앙
//	for ( n=0; n<=m_pDB->m_nQtyJigan; n++)
//	{
//		if(n==0)
//		{
//			dArrLargeDn.Add(0);
//			dArrSmallDn.Add(0);
//			dArrLargeDn.Add(dLengthSttJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2);							
//			dArrSmallDn.Add(dLengthSttJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2);							
//		}
//		else if(n==m_pDB->m_nQtyJigan)
//		{
//			dArrLargeDn.Add(dLengthRebarDn - (dLengthEndJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2));		
//			dArrSmallDn.Add(dLengthRebarDn - (dLengthEndJointCen + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2));		
//			dArrLargeDn.Add(dLengthRebarDn); 
//			dArrSmallDn.Add(dLengthRebarDn);
//		}
//		else
//		{
//			double dDistJijumFrStt = GetLengthSlabAccuml(n-1, JONGBASEPOS_SLABCENTER, TRUE) - GetLengthCoverSd(TRUE) - dSttExpWCent;
//
//			double dLargeStt = (dDisCentJointSttGirLong + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);
//			double dLargeEnd = (dDisCentJointEndGirLong + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);
//			double dSmallStt = (dDisCentJointSttGirSmal + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);
//			double dSmallEnd = (dDisCentJointEndGirSmal + dSpaceJoint + (dLJointGenJijumUp[POS_SLAB_CENTER]+dLJointGenJijumLo[POS_SLAB_CENTER])/2);
//
//			dArrLargeDn.Add(dDistJijumFrStt-dLargeStt);
//			dArrLargeDn.Add(dDistJijumFrStt);
//			dArrLargeDn.Add(dDistJijumFrStt+dLargeEnd);
//
//			dArrSmallDn.Add(dDistJijumFrStt-dSmallStt);
//			dArrSmallDn.Add(dDistJijumFrStt);
//			dArrSmallDn.Add(dDistJijumFrStt+dSmallEnd);
//
//			double dStaJijum = pGirCen->GetStationOnJijum(n);
//
//			xySta.x = m_pDB->m_pLine->GetStationByDisLength(dStaJijum, -dLargeStt, dDistCen);
//			xySta.y = m_pDB->m_pLine->GetStationByDisLength(dStaJijum,  dLargeEnd, dDistCen);
//			dArrStaLargeDn.Add(xySta);
//			xySta.x = m_pDB->m_pLine->GetStationByDisLength(dStaJijum, -dSmallStt, dDistCen);
//			xySta.y = m_pDB->m_pLine->GetStationByDisLength(dStaJijum,  dSmallEnd, dDistCen);
//			dArrStaSmallDn.Add(xySta);
//		}
//	}

	dArrLargeDn.Sort();
	dArrSmallDn.Sort();

	for (n=0; n<dArrLargeDn.GetSize()-1; n++)
		arrDnCenLong.Add(dArrLargeDn[n+1]-dArrLargeDn[n]);
	for (n=0; n<dArrSmallDn.GetSize()-1; n++)
		arrDnCenSmal.Add(dArrSmallDn[n+1]-dArrSmallDn[n]);

	// 상/하부슬래브 좌/우측 모두 계산
	CDPoint vAngSttSmall, vAngSttLong, vAngEndSmall, vAngEndLong;
	double dArrJoint[4];
	double dLargeL = 0;
	double dSmallL = 0;

	for (long nLeft=0; nLeft<2; nLeft++)
	{
		BOOL bLeft = nLeft==0 ? TRUE : FALSE;
		
		xyArrUp = GetXyArrGuideLineForBeryukRebar(nLeft==0 ? JONGBASEPOS_SLABLEFT : JONGBASEPOS_SLABRIGHT, TRUE);
		xyArrDn = GetXyArrGuideLineForBeryukRebar(nLeft==0 ? JONGBASEPOS_SLABLEFT : JONGBASEPOS_SLABRIGHT, FALSE);
		dLengthRebarUp = fabs(xyArrUp[0].x-xyArrUp[xyArrUp.GetUpperBound()].x);
		dLengthRebarDn = fabs(xyArrDn[0].x-xyArrDn[xyArrDn.GetUpperBound()].x);

		for (long nUpDn=0; nUpDn<2; nUpDn++)
		{
			BOOL bUpper   = nUpDn==0 ? TRUE : FALSE;

			dArrLarge.RemoveAll();
			dArrSmall.RemoveAll();
			
			for (n=0; n<=m_pDB->m_nQtyJigan; n++)
			{
				if(n==0)
				{
					dArrLarge.Add(0);
					dArrSmall.Add(0);
					if(bUpper)
					{
						dLargeL = bLeft ? dLengthSttJointLeft : dLengthSttJointRigh;
						dSmallL = dLargeL;
					}
					else
					{
						dLargeL = (bLeft ? dLengthSttJointLeft : dLengthSttJointRigh) + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2;
						dSmallL = dLargeL;
					}
					dArrLarge.Add(dLargeL);
					dArrSmall.Add(dSmallL);

				}
				else if(n==m_pDB->m_nQtyJigan)
				{
					if(bUpper)
					{
						dLargeL = dLengthRebarUp - (bLeft ? dLengthEndJointLeft : dLengthEndJointRigh);
						dSmallL = dLargeL;
					}
					else
					{
						dLargeL = dLengthRebarDn - ((bLeft ? dLengthEndJointLeft : dLengthEndJointRigh) + dSpaceJoint + (dLJointDanGenUp[2]+dLJointDanGenLo[2])/2);
						dSmallL = dLargeL;
					}
					dArrLarge.Add(dLargeL);
					dArrSmall.Add(dSmallL);
					dArrLarge.Add(bUpper?dLengthRebarUp:dLengthRebarDn); 
					dArrSmall.Add(bUpper?dLengthRebarUp:dLengthRebarDn);
				}
				else
				{
					double dStaJijum = 0, dStaSttSmall = 0, dStaEndSmall = 0, dStaSttLong = 0, dStaEndLong = 0;
					double dDistJijumFrStt = GetLengthSlabAccuml(n-1, bLeft ? JONGBASEPOS_SLABLEFT : JONGBASEPOS_SLABRIGHT, TRUE) - GetLengthCoverSd(TRUE) - (bUpper?(bLeft?dSttExpWLeft:dSttExpWRigh):0);
	
					CDPoint vAng = m_pDB->GetGirder(-1)->GetAngleOnJijum(n);
					dStaJijum    = m_pDB->GetGirder(-1)->GetStationOnJijum(n);
					dStaSttSmall = bUpper ? dArrStaSmallUp[n-1].x : dArrStaSmallDn[n-1].x;
					dStaEndSmall = bUpper ? dArrStaSmallUp[n-1].y : dArrStaSmallDn[n-1].y;
					dStaSttLong  = bUpper ? dArrStaLargeUp[n-1].x : dArrStaLargeDn[n-1].x;
					dStaEndLong  = bUpper ? dArrStaLargeUp[n-1].y : dArrStaLargeDn[n-1].y;
					
					if(m_nJijumBogangArrange==0)		//교축직각 방향
					{
						vAngSttSmall = CDPoint(0, 1);
						vAngEndSmall = CDPoint(0, 1);
						vAngSttLong  = CDPoint(0, 1);
						vAngEndLong  = CDPoint(0, 1);
					}
					else if(m_nJijumBogangArrange==1)//지점각과 동일배치
					{
						vAngSttSmall = m_pDB->GetGirder(-1)->GetAngleOnJijum(n);
						vAngEndSmall = m_pDB->GetGirder(-1)->GetAngleOnJijum(n);
						vAngSttLong  = m_pDB->GetGirder(-1)->GetAngleOnJijum(n);
						vAngEndLong  = m_pDB->GetGirder(-1)->GetAngleOnJijum(n);
					}
					else if(m_nJijumBogangArrange==2)//지점각과 평행배치
					{
						vAngSttSmall = m_pDB->m_pLine->GetAngleByOffset(dStaJijum, dStaSttSmall, vAng);
						vAngEndSmall = m_pDB->m_pLine->GetAngleByOffset(dStaJijum, dStaEndSmall, vAng);
						vAngSttLong  = m_pDB->m_pLine->GetAngleByOffset(dStaJijum, dStaSttLong, vAng);
						vAngEndLong  = m_pDB->m_pLine->GetAngleByOffset(dStaJijum, dStaEndLong, vAng);
					}

					double dSttLong = m_pDB->m_pLine->GetLengthByDis(dStaJijum, dStaSttLong,  0);
					double dEndLong = m_pDB->m_pLine->GetLengthByDis(dStaJijum, dStaEndLong,  0);
					double dSttSmal = m_pDB->m_pLine->GetLengthByDis(dStaJijum, dStaSttSmall, 0);
					double dEndSmal = m_pDB->m_pLine->GetLengthByDis(dStaJijum, dStaEndSmall, 0);

					GetArrJointDisAtJijum(n, vAngSttLong, vAngEndLong, dSttLong, dEndLong, dArrJoint);	// dArrJoint==> 0: 좌측시점, 1 : 좌측종점 2 : 우측시점 3 : 우측종점 
					dArrLarge.Add(dDistJijumFrStt-dArrJoint[bLeft==TRUE?0:2]);
					dArrLarge.Add(dDistJijumFrStt);
					dArrLarge.Add(dDistJijumFrStt+dArrJoint[bLeft==TRUE?1:3]);
					
					GetArrJointDisAtJijum(n, vAngSttSmall, vAngEndSmall, dSttSmal, dEndSmal, dArrJoint);	// dArrJoint==> 0: 좌측시점, 1 : 좌측종점 2 : 우측시점 3 : 우측종점 
					dArrSmall.Add(dDistJijumFrStt-dArrJoint[bLeft==TRUE?0:2]);
					dArrSmall.Add(dDistJijumFrStt);
					dArrSmall.Add(dDistJijumFrStt+dArrJoint[bLeft==TRUE?1:3]);
				}
			}

			for (n=0; n<dArrLarge.GetSize()-1; n++)
			{
				if(bLeft)
				{
					if(bUpper)
					{
						arrUpLeftLong.Add(dArrLarge[n+1]-dArrLarge[n]);
						arrUpLeftSmal.Add(dArrSmall[n+1]-dArrSmall[n]);
					}
					else
					{
						arrDnLeftLong.Add(dArrLarge[n+1]-dArrLarge[n]);
						arrDnLeftSmal.Add(dArrSmall[n+1]-dArrSmall[n]);
					}
				}
				else
				{
					if(bUpper)
					{
						arrUpRighLong.Add(dArrLarge[n+1]-dArrLarge[n]);
						arrUpRighSmal.Add(dArrSmall[n+1]-dArrSmall[n]);
					}
					else
					{
						arrDnRighLong.Add(dArrLarge[n+1]-dArrLarge[n]);
						arrDnRighSmal.Add(dArrSmall[n+1]-dArrSmall[n]);
					}
				}
			}
		}
	}

	// 철근직경생성(슬래브 좌측)
	for ( n=0; n<arrUpLeftLong.GetSize(); n++)	
	{
		if(n==0 || n==arrUpLeftLong.GetUpperBound())	// 단부
		{
			dArrDiaUpLeft.Add(m_dDiaBeryukRebar_Dan[POS_SLAB_LEFT]);
			dArrDiaDnLeft.Add(m_dDiaBeryukRebar_Dan[POS_SLAB_LEFT]);
		}
		else if( (n-1)%3==0)	// 일반부
		{
			dArrDiaUpLeft.Add(m_dDiaBeryukRebar_Cent[POS_SLAB_LEFT]);
			dArrDiaDnLeft.Add(m_dDiaBeryukRebar_Cent[POS_SLAB_LEFT]);
		}
		else					// 지점부 
		{
			dArrDiaUpLeft.Add(m_dDiaJijumRebarUp[POS_SLAB_LEFT]);
			dArrDiaDnLeft.Add(m_dDiaJijumRebarDn[POS_SLAB_LEFT]);
		}

	}

	// 철근직경생성(슬래브 우측)
	for ( n=0; n<arrUpRighLong.GetSize(); n++)	
	{
		if(n==0 || n==arrUpRighLong.GetUpperBound())	// 단부
		{
			dArrDiaUpRigh.Add(m_dDiaBeryukRebar_Dan[POS_SLAB_RIGH]);
			dArrDiaDnRigh.Add(m_dDiaBeryukRebar_Dan[POS_SLAB_RIGH]);
		}
		else if( (n-1)%3==0)	// 일반부
		{
			dArrDiaUpRigh.Add(m_dDiaBeryukRebar_Cent[POS_SLAB_RIGH]);
			dArrDiaDnRigh.Add(m_dDiaBeryukRebar_Cent[POS_SLAB_RIGH]);
		}
		else					// 지점부 
		{
			dArrDiaUpRigh.Add(m_dDiaJijumRebarUp[POS_SLAB_RIGH]);
			dArrDiaDnRigh.Add(m_dDiaJijumRebarDn[POS_SLAB_RIGH]);
		}
	}

	// 철근직경생성(슬래브 중심)
	for ( n=0; n<arrUpCenLong.GetSize(); n++)	
	{
		if(n==0 || n==arrUpCenLong.GetUpperBound())	// 단부
		{
			dArrDiaUpCent.Add(m_dDiaBeryukRebar_Dan[POS_SLAB_CENTER]);
			dArrDiaDnCent.Add(m_dDiaBeryukRebar_Dan[POS_SLAB_CENTER]);
		}
		else if( (n-1)%3==0)	// 일반부
		{
			dArrDiaUpCent.Add(m_dDiaBeryukRebar_Cent[POS_SLAB_CENTER]);
			dArrDiaDnCent.Add(m_dDiaBeryukRebar_Cent[POS_SLAB_CENTER]);
		}
		else					// 지점부 
		{
			dArrDiaUpCent.Add(m_dDiaJijumRebarUp[POS_SLAB_CENTER]);
			dArrDiaDnCent.Add(m_dDiaJijumRebarDn[POS_SLAB_CENTER]);
		}
	}	

	// 이음위치간 거리를 이용한 사이클생성
	long nBesu = max(GetBesu(JONGBASEPOS_SLABCENTER, TRUE), GetBesu(JONGBASEPOS_SLABCENTER, FALSE));
	long nCyclesu = 0;
	if(m_pDB->m_nQtyJigan > 1)	// 다경간
	{
		// m_nUseJujangRebar : 총철근량의 1/3을 순지간의 1/16까지 연장배근
		if(nBesu==1)		nCyclesu = m_nUseJujangRebar==0 ? 3 : 2;
		else if(nBesu==2)	nCyclesu = m_nUseJujangRebar==0 ? 6 : 2;
		else if(nBesu==3)	nCyclesu = m_nUseJujangRebar==0 ? 3 : 6;
		
		for (long n=0; n<nCyclesu; ++n)
		{
			BOOL bLong = n==0 ? TRUE : FALSE;
			if(nBesu==3 && nCyclesu==6 && n%2==0)	bLong = TRUE;
			if(nBesu==2 && nCyclesu==6 && n%3==0)	bLong = TRUE;
			
			if(bLong)	//long
			{
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABLEFT, arrUpLeftLong, arrDnLeftLong, dArrDiaUpLeft, dArrDiaDnLeft);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABRIGHT, arrUpRighLong, arrDnRighLong, dArrDiaUpRigh, dArrDiaDnRigh);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABCENTER, arrUpCenLong, arrDnCenLong, dArrDiaUpCent, dArrDiaDnCent);
			}
			else		// short
			{
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABLEFT, arrUpLeftSmal, arrDnLeftSmal, dArrDiaUpLeft, dArrDiaDnLeft);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABRIGHT, arrUpRighSmal, arrDnRighSmal, dArrDiaUpRigh, dArrDiaDnRigh);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABCENTER, arrUpCenSmal, arrDnCenSmal, dArrDiaUpCent, dArrDiaDnCent);
			}
		}
	}
	else	// 단경간일 경우
	{
		nCyclesu = 2;

		for (long n=0; n<nCyclesu; n++)
		{
			BOOL bLong = nBesu==2 ? TRUE : FALSE;
			if(nBesu!=2	&& n==0)	bLong = TRUE;

			if(bLong)	//long
			{
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABLEFT, arrUpLeftLong, arrDnLeftLong, dArrDiaUpLeft, dArrDiaDnLeft);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABRIGHT, arrUpRighLong, arrDnRighLong, dArrDiaUpRigh, dArrDiaDnRigh);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABCENTER, arrUpCenLong, arrDnCenLong, dArrDiaUpCent, dArrDiaDnCent);
			}
			else	// short
			{
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABLEFT, arrUpLeftSmal, arrDnLeftSmal, dArrDiaUpLeft, dArrDiaDnLeft);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABRIGHT, arrUpRighSmal, arrDnRighSmal, dArrDiaUpRigh, dArrDiaDnRigh);
				AddRebarCycleByJongBasePos(JONGBASEPOS_SLABCENTER, arrUpCenSmal, arrDnCenSmal, dArrDiaUpCent, dArrDiaDnCent);
			}
		}
	}

	MakeCycleBeryukRebar_LC_RC();
}

void CADeckData::MakeCycleBeryukRebar_LC_RC()		
{
	RemoveRebarCycleByJongBasePos(JONGBASEPOS_SLABLC);
	RemoveRebarCycleByJongBasePos(JONGBASEPOS_SLABRC);
	RemoveRebarCycleByJongBasePos(JONGBASEPOS_ONCB);

	// 기본적으로 사이클수는 좌측기준으로 모두 같음
	long nCyclesu = GetRebarCycleSuByJongBasePos(JONGBASEPOS_SLABLEFT);
	double dDistL = GetDistByJongBase(JONGBASEPOS_SLABLEFT);
	double dDistR = GetDistByJongBase(JONGBASEPOS_SLABRIGHT);

	long nJongBaseEnd = IsBridgeHasCrossBeam() ? JONGBASEPOS_ONCB : JONGBASEPOS_SLABRC;
	for (long nJongBase=JONGBASEPOS_SLABLC; nJongBase<=nJongBaseEnd; nJongBase++)
	{
		double dDist  = GetDistByJongBase(nJongBase);

		for (long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			REBAR_CYCLE stCycle;
			CDoubleArray dArrJointUp, dArrJointDn; 

			long nJointsu = m_stBeryukCycleLeft[nCycle].m_dArrJointUp.GetSize();
			for (long nJoint=0; nJoint<nJointsu; nJoint++)
			{
				double dJointL = m_stBeryukCycleLeft[nCycle].m_dArrJointUp.GetAt(nJoint);
				double dJointR = m_stBeryukCycleRigh[nCycle].m_dArrJointUp.GetAt(nJoint);
				dArrJointUp.Add(Bogan(dDistL, dDistR, dJointL, dJointR, dDist));

				dJointL = m_stBeryukCycleLeft[nCycle].m_dArrJointDn.GetAt(nJoint);
				dJointR = m_stBeryukCycleRigh[nCycle].m_dArrJointDn.GetAt(nJoint);
				dArrJointDn.Add(Bogan(dDistL, dDistR, dJointL, dJointR, dDist));
			}

			stCycle.m_dArrJointUp = dArrJointUp;
			stCycle.m_dArrJointDn = dArrJointDn;
			stCycle.m_dArrDiaUp	= m_stBeryukCycleLeft[nCycle].m_dArrDiaUp;
			stCycle.m_dArrDiaDn   = m_stBeryukCycleLeft[nCycle].m_dArrDiaDn;
			AddRebarCycleByJongBasePos(nJongBase, stCycle);
		}
	}
}




// 주철근의 이음장 위치 계산
// [권고안] 에서만 작동함.
// 기본으로 2Cycle만 생성됨
// 함수내부의 데이터는 사용자가 입력에서 변경함.
void CADeckData::MakeCycleMainRebar(long nSection)		
{
	double dStaSec  = GetStationBySection(nSection);
	CDPoint vAngSec = CDPoint(0,1);	// 횡단면에서 각
	if(nSection==SECTION_SKEW_STT)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	else if(nSection==SECTION_SKEW_END)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	CPlateBasicIndex *pBx = GetBxBySection(nSection);
	double dRatio    = fabs(vAngSec.y);
	double dSlabW    = m_pDB->GetWidthAct(dStaSec) - m_dCoverSd*2;				// 슬래브의 폭은 투영폭임
	double dJointL   = GetJointLengthAtHDan(nSection, TRUE, POS_SLAB_LEFT);		// 슬래브좌측과 중앙부의 이음길이
	double dJointR   = GetJointLengthAtHDan(nSection, TRUE, POS_SLAB_RIGH);		// 슬래브우측과 중앙부의 이음길이
	double dJointC   = GetJointLengthAtHDan(nSection, TRUE, POS_SLAB_CENTER);	// 슬래브 중앙부 철근의 이음길이
	CDPoint ALeft[4], ARigh[4];
	m_pDB->GetXyFrontCantilever(pBx, ALeft, TRUE);
	m_pDB->GetXyFrontCantilever(pBx, ARigh, FALSE);
	double dLengthCantilLeft  = (ALeft[3].x-ALeft[0].x)  + m_dRangeMainRebarFromFlangeStt - m_dCoverSd;
	double dLengthCantilRight = (ARigh[0].x-ARigh[3].x) + m_dRangeMainRebarFromFlangeStt - m_dCoverSd;
		
	RemoveRebarCycleBySection(nSection);
	
	CDoubleArray ArrJoint;

	double dOffSetRebar = 600;	// 겹이음간 이격거리
	double dJointUpPos  = 0;	// 슬래브좌측끝단에서 겹이음위치까지의 거리
	double dDisDolchulL	= GetLengthMainDolchul(nSection, TRUE, TRUE);
	double dDisDolchulR	= GetLengthMainDolchul(nSection, TRUE, FALSE);

	//////////////////////////////////////////////////////////////////////////
	// 상부 철근 이음위치 (좌측에서 우측으로 이동하며 겹이음 생성)
	long nCyclesuTot = 2;
	long nCycle = 0;
	for (nCycle=0; nCycle<nCyclesuTot; nCycle++)
	{
		ArrJoint.RemoveAll();

		double dSttPos = -dDisDolchulL;								// 주철근의 시점
		double dEndPos =  dDisDolchulL+dDisDolchulR+dSlabW;	// 주철근의 종점
		ArrJoint.Add(0);			// [원점] : 슬래브상면-주철근 시작위치 
		// 1Cycle -> 2Cycle -> 3Cycle 일수록 이음위치는 슬래브 좌측에서 중심쪽으로 대칭이동함
		// 사보강철근은 좌,우,중앙부 철근 직경이 동일.
		if(nSection == SECTION_DAN)
		{
			if(m_dDiaMainRebar_Dan[POS_SLAB_LEFT] != m_dDiaMainRebar_Dan[POS_SLAB_CENTER])
			{
				dJointUpPos = dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar);
				ArrJoint.Add (dJointUpPos);
				dSttPos += dDisDolchulL+dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar)-dJointL/2;	
			}
			if(m_dDiaMainRebar_Dan[POS_SLAB_RIGH] != m_dDiaMainRebar_Dan[POS_SLAB_CENTER])
			{
				dJointUpPos = dSlabW-dLengthCantilRight-nCycle*(dJointR+dOffSetRebar);
				if(dJointUpPos+dJointR > dSlabW)	// 이음이 슬래브를 벗어난 경우
					dJointUpPos = dSlabW-dLengthCantilRight;
				ArrJoint.Add(dJointUpPos);
				dEndPos -= dLengthCantilRight+dDisDolchulR+nCycle*(dJointR+dOffSetRebar)-dJointR/2;
			}
		}
		else if(nSection == SECTION_CEN || nSection == SECTION_PIER)
		{
			if(m_dDiaMainRebar_Cent[POS_SLAB_LEFT] != m_dDiaMainRebar_Cent[POS_SLAB_CENTER])
			{
				dJointUpPos = dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar);
				ArrJoint.Add (dJointUpPos);
				dSttPos += dDisDolchulL+dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar)-dJointL/2;	
			}
			if(m_dDiaMainRebar_Cent[POS_SLAB_RIGH] != m_dDiaMainRebar_Cent[POS_SLAB_CENTER])
			{
				dJointUpPos = dSlabW-dLengthCantilRight-nCycle*(dJointR+dOffSetRebar);
				if(dJointUpPos+dJointR > dSlabW)	// 이음이 슬래브를 벗어난 경우
					dJointUpPos = dSlabW-dLengthCantilRight;
				ArrJoint.Add(dJointUpPos);
				dEndPos -= dLengthCantilRight+dDisDolchulR+nCycle*(dJointR+dOffSetRebar)-dJointR/2;
			}
		}

		// 한 이음씩 전진하면서 생성..
		// 1Cycle -> 2Cycle -> 3Cycle 일수록 이음위치는 우측으로 이동함
		long n = 0;
		for(n=0; n<10; n++)
		{
			if(dSttPos > dSlabW)
				break;

			if(fabs(dEndPos-dSttPos) > m_dLengthRebarMax)
			{
				dJointUpPos = dSttPos + m_dLengthRebarMax*dRatio - nCycle*(dJointC+dOffSetRebar)*dRatio - dJointC/2*dRatio;
				if(dJointUpPos + (dJointC/2*dRatio) > dSlabW)	// 이음이 슬래브를 벗어난 경우
					dJointUpPos = dSttPos + m_dLengthRebarMax*dRatio - dJointC*dRatio;

				ArrJoint.Add(dJointUpPos);
				dSttPos += m_dLengthRebarMax*dRatio - dJointC*dRatio;
			}
			else
				break;
		}
		ArrJoint.Sort();

		CDoubleArray arrGuganUp;
		for( n=0; n<ArrJoint.GetSize()-1; n++)
			arrGuganUp.Add(ArrJoint[n+1]-ArrJoint[n]);

		// 철근의 직경, 존재유무, 내부겹이음위치 결정
		// REBAR_OBJECT는 이음위치수보다 1개 더 많다.
		CDoubleArray dArrDia;
		CDoubleArray dArrValid;
		for ( n=0; n<=arrGuganUp.GetSize(); n++)
		{
			if(n==0)					// 슬래브좌측
				dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_LEFT, nCycle, TRUE));
			else if(n==arrGuganUp.GetSize())									// 슬래브우측
				dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_RIGH,  nCycle, TRUE));
			else 
				dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_CENTER,  nCycle, TRUE));

			dArrValid.Add(TRUE);
		}
		REBAR_CYCLE stRebarCycle;
		stRebarCycle.m_dArrJointUp  = arrGuganUp;
		stRebarCycle.m_dArrDiaUp	= dArrDia;
		stRebarCycle.m_dArrValidUp	= dArrValid;
		AddRebarCycleBySection(nSection, stRebarCycle);
	}

	//////////////////////////////////////////////////////////////////////////
	// 하부 철근 이음위치 (우측에서 좌측으로 이동하며 겹이음 생성)
	if((nSection==SECTION_CEN||nSection==SECTION_PIER) && IsBridgeHasSlabHunch())
	{
		for (nCycle=0; nCycle<nCyclesuTot; nCycle++)
		{
			REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, nCycle);
			long nShapesu = GetsuMainRebarShapeAtCen();
			CDoubleArray dArrDia;
			CDoubleArray dArrValid;
			CDoubleArray arrGuganDn;
			for (long nShape=0; nShape<nShapesu; nShape++)
			{
				if(nShape==0)					// 슬래브좌측
					dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_LEFT,  nCycle, FALSE));
				else if(nShape==nShapesu-1)									// 슬래브우측
					dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_RIGH,  nCycle, FALSE));
				else 
					dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_CENTER,  nCycle, FALSE));

				if(nShape<nShapesu-1)	arrGuganDn.Add(0);
				dArrValid.Add(TRUE);
			}
			pRebarCycle->m_dArrDiaDn   = dArrDia;
			pRebarCycle->m_dArrValidDn = dArrValid;
			pRebarCycle->m_dArrJointDn = arrGuganDn;
		}
		
		return;
	}

	dDisDolchulL = GetLengthMainDolchul(nSection, FALSE, TRUE);
	dDisDolchulR = GetLengthMainDolchul(nSection, FALSE, FALSE);
	dJointL = GetJointLengthAtHDan(nSection, FALSE, POS_SLAB_LEFT);		// 슬래브좌측과 중앙부의 이음길이
	dJointR = GetJointLengthAtHDan(nSection, FALSE, POS_SLAB_RIGH);		// 슬래브우측과 중앙부의 이음길이
	dJointC = GetJointLengthAtHDan(nSection, FALSE, POS_SLAB_CENTER);	// 슬래브 중앙부 철근의 이음길이

//	BOOL bDanbu = FALSE;
//	if(!IsSkewStt() && nSection==SECTION_DAN)	bDanbu = TRUE;
//	if(!IsSkewEnd() && nSection==SECTION_DAN)	bDanbu = TRUE;
//	if(nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END ) TRUE;
	BOOL bDanbu = nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END ? TRUE : FALSE;
	if(m_pDB->IsTUGir() && bDanbu)
	{
		for ( nCycle=0; nCycle<nCyclesuTot; nCycle++)
		{
			REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, nCycle);
			CDoubleArray arrGuganDn;

			double dJointDnPos = (ALeft[3].x-ALeft[0].x) - m_dCoverSd*2;
			arrGuganDn.Add(dJointDnPos);

			for(long nG=0; nG<m_pDB->GetQtyGirder()-1; nG++)
			{
				arrGuganDn.Add(m_pDB->m_BindConc.m_dWidth+m_dCoverSd*2);
				arrGuganDn.Add(m_pDB->GetGirder(nG)->GetDisGirderToGirder(pBx)-m_pDB->m_BindConc.m_dWidth-m_dCoverSd*2);
			}
			arrGuganDn.Add(m_pDB->m_BindConc.m_dWidth+m_dCoverSd*2);

			CDoubleArray dArrDia;
			CDoubleArray dArrValid;
			for (long n=0; n<=arrGuganDn.GetSize(); n++)
			{
				if(n==0)					// 슬래브좌측
					dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_LEFT,  nCycle, FALSE));
				else if(n==arrGuganDn.GetSize())									// 슬래브우측
					dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_RIGH,  nCycle, FALSE));
				else 
					dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_CENTER,  nCycle, FALSE));

				dArrValid.Add((n+1)%2);
			}
			pRebarCycle->m_dArrDiaDn   = dArrDia;
			pRebarCycle->m_dArrValidDn = dArrValid;
			pRebarCycle->m_dArrJointDn = arrGuganDn;
		}
		return;
	}

	for ( nCycle=0; nCycle<nCyclesuTot; nCycle++)
	{
		REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, nCycle);

		double dSttPos = -dDisDolchulR;			// 주철근의 우측종점
		double dEndPos =  dDisDolchulR+ dSlabW;	// 주철근의 좌측상면종점(꺽이는 부분은 이음을 안둠)
	
		ArrJoint.RemoveAll();
		ArrJoint.Add(0);			// [원점] : 슬래브상면-주철근 시작위치 
		// 1Cycle -> 2Cycle -> 3Cycle 일수록 이음위치는 슬래브 중심쪽으로 대칭이동함
		// 사보강철근은 좌,우,중앙부 철근 직경이 동일.
		if(nSection == SECTION_DAN)
		{
			if(m_dDiaMainRebar_Dan[POS_SLAB_LEFT] != m_dDiaMainRebar_Dan[POS_SLAB_CENTER])
			{
				dJointUpPos = dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar);
				ArrJoint.Add (dJointUpPos);
				dSttPos += dDisDolchulL+dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar)-dJointL/2;	
			}
			if(m_dDiaMainRebar_Dan[POS_SLAB_RIGH] != m_dDiaMainRebar_Dan[POS_SLAB_CENTER])
			{
				dJointUpPos = dSlabW-dLengthCantilRight-nCycle*(dJointR+dOffSetRebar);
				if(dJointUpPos+dJointR > dSlabW)	// 이음이 슬래브를 벗어난 경우
					dJointUpPos = dSlabW-dLengthCantilRight;
				ArrJoint.Add(dJointUpPos);
				dEndPos -= dLengthCantilRight+dDisDolchulR+nCycle*(dJointR+dOffSetRebar)-dJointR/2;
			}
		}
		else if(nSection == SECTION_CEN || nSection == SECTION_PIER)
		{
			if(m_dDiaMainRebar_Cent[POS_SLAB_LEFT] != m_dDiaMainRebar_Cent[POS_SLAB_CENTER])
			{
				dJointUpPos = dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar);
				ArrJoint.Add (dJointUpPos);
				dSttPos += dDisDolchulL+dLengthCantilLeft+nCycle*(dJointL+dOffSetRebar)-dJointL/2;	
			}
			if(m_dDiaMainRebar_Cent[POS_SLAB_RIGH] != m_dDiaMainRebar_Cent[POS_SLAB_CENTER])
			{
				dJointUpPos = dSlabW-dLengthCantilRight-nCycle*(dJointR+dOffSetRebar);
				if(dJointUpPos+dJointR > dSlabW)	// 이음이 슬래브를 벗어난 경우
					dJointUpPos = dSlabW-dLengthCantilRight;
				ArrJoint.Add(dJointUpPos);
				dEndPos -= dLengthCantilRight+dDisDolchulR+nCycle*(dJointR+dOffSetRebar)-dJointR/2;
			}
		}

		long n = 0;
		// 한 이음씩 전진하면서 생성..
		// 1Cycle -> 2Cycle -> 3Cycle 일수록 이음위치는 좌측으로 이동함
		double dTotalL = dSlabW + dDisDolchulR;
		for(n=0; n<10; n++)
		{
			if(dSttPos > dTotalL)
				break;

			if(fabs(dEndPos-dSttPos) > m_dLengthRebarMax)
			{
				dJointUpPos = dSlabW - (dSttPos + m_dLengthRebarMax*dRatio - nCycle*(dJointC+dOffSetRebar)*dRatio - dJointC/2*dRatio);
				if(dJointUpPos*dRatio + dJointC/2*dRatio < 0)	// 이음이 슬래브를 벗어난 경우
					dJointUpPos = dSlabW-(dSttPos + m_dLengthRebarMax*dRatio - dJointC*dRatio);

				ArrJoint.Add(dJointUpPos);
				dSttPos += m_dLengthRebarMax*dRatio - dJointC*dRatio;
			}
			else
				break;
		}
		ArrJoint.Sort();

		CDoubleArray arrGuganDn;
		for( n=0; n<ArrJoint.GetSize()-1; n++)
			arrGuganDn.Add(ArrJoint[n+1]-ArrJoint[n]);

		// 철근의 직경, 존재유무, 내부겹이음위치 결정
		// REBAR_OBJECT는 이음위치수보다 1개 더 많다.
		CDoubleArray dArrDia;
		CDoubleArray dArrValid;
		for ( n=0; n<=arrGuganDn.GetSize(); n++)
		{
			if(n==0)					// 슬래브좌측
				dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_LEFT,  nCycle, FALSE));
			else if(n==arrGuganDn.GetSize())									// 슬래브우측
				dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_RIGH,  nCycle, FALSE));
			else 
				dArrDia.Add(GetRebarDiaBySection(nSection, POS_SLAB_CENTER,  nCycle, FALSE));

			dArrValid.Add(TRUE);
		}
		pRebarCycle->m_dArrDiaDn   = dArrDia;
		pRebarCycle->m_dArrValidDn = dArrValid;
		pRebarCycle->m_dArrJointDn = arrGuganDn;
	}
}




// 주철근의 이음장 위치 계산
// [권고안] 에서만 작동함
// 함수내부의 데이터는 사용자가 입력에서 변경함.
void CADeckData::MakeCycleMainRebar4Cy(long nSection)		
{
	if(GetRebarCycleSuBySection(nSection) > 2)
	{
		vector <REBAR_CYCLE> *vCycle = GetRebarCycleBySection(nSection);
		vCycle->erase(vCycle->begin()+2, vCycle->end());

		return;
	}
	
	CDoubleArray dArrDia, dArrJoint, dArrJointNew, dArrGugan;
	double dStaSec	   = GetStationBySection(nSection);
	CDPoint vAngSec = CDPoint(0,1);	// 횡단면에서 각
	if(nSection==SECTION_SKEW_STT)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	else if(nSection==SECTION_SKEW_END)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	double	dSlabW = m_pDB->GetWidthAct(dStaSec) - (m_dCoverSd/fabs(vAngSec.y))*2;

	// 상부 철근 이음위치 (좌측에서 우측으로 이동하며 겹이음 생성)
	REBAR_CYCLE *pRebarCycle = GetRebarCycleBySection(nSection, 0);
	CDoubleArray dArrJointUp = pRebarCycle->m_dArrJointUp;
	CDoubleArray dArrJointDn = pRebarCycle->m_dArrJointDn;
	double dSumGuganUp = 0;
	double dSumGuganDn = 0;
	long   n		   = 0;

	for (n=0; n<dArrJointUp.GetSize(); n++)
		dSumGuganUp += dArrJointUp[n];
	for ( n=0; n<dArrJointDn.GetSize(); n++)
		dSumGuganDn += dArrJointDn[n];
	dArrJointUp.Add(dSlabW - dSumGuganUp);
	dArrJointDn.Add(dSlabW - dSumGuganDn);

	CDoubleArray dArrJointNewUp, dArrJointNewDn;
	for (n=dArrJointUp.GetSize()-1; n>=0; n--)
		dArrJointNewUp.Add(dArrJointUp[n]);
	for (n=dArrJointDn.GetSize()-1; n>=0; n--)
		dArrJointNewDn.Add(dArrJointDn[n]);

	dArrJointNewUp.RemoveAt(dArrJointNewUp.GetUpperBound());
	dArrJointNewDn.RemoveAt(dArrJointNewDn.GetUpperBound());

	//////////////////////////////////////////////////////////////////////////
	
	REBAR_CYCLE stRebarCycle3;
	stRebarCycle3.m_dArrJointUp = dArrJointNewUp;
	stRebarCycle3.m_dArrJointDn = dArrJointNewDn;
	stRebarCycle3.m_dArrDiaUp = pRebarCycle->m_dArrDiaUp;
	stRebarCycle3.m_dArrDiaDn = pRebarCycle->m_dArrDiaDn;
	stRebarCycle3.m_dArrValidUp = pRebarCycle->m_dArrValidUp;
	stRebarCycle3.m_dArrValidDn = pRebarCycle->m_dArrValidDn;

	AddRebarCycleBySection(nSection, stRebarCycle3);
	
	//////////////////////////////////////////////////////////////////////////
	
	REBAR_CYCLE stRebarCycle4 = *GetRebarCycleBySection(nSection, 1);
	AddRebarCycleBySection(nSection, stRebarCycle4);
}


// 주철근의 이음장 위치 계산
// [권고안] 에서만 작동함
// 함수내부의 데이터는 사용자가 입력에서 변경함.
void CADeckData::MakeCycleMainRebar1Cy(long nSection)		
{
	if(GetRebarCycleSuBySection(nSection) > 1)
	{
		vector <REBAR_CYCLE> *vCycle = GetRebarCycleBySection(nSection);
		vCycle->erase(vCycle->begin()+1, vCycle->end());
	}
}


// 주철근의 이음장 위치 계산
// [겹이음위치 재계산] 에서만 작동함
// 횡단에서만 사용가능
void CADeckData::MakeCycleMainRebarReCalc(long nSection)		
{
	CDoubleArray dArrDia, dArrJoint, dArrJointNew, dArrGugan;
	double dStaSec	    = GetStationBySection(nSection);
	double dCTCForJoint = 0;	// 겹이음길이 계산을 위한 CTC - 첫번철근CTC는 좌측주철근 CTC, 마지막철근CTC는 우측주철근CTC, 나머지는 중앙주철근CTC

	CDPoint vAngSec = CDPoint(0,1);	// 횡단면에서 각
	if(nSection==SECTION_SKEW_STT)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	else if(nSection==SECTION_SKEW_END)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	double dSlabW = (m_pDB->GetWidthAct(dStaSec) - m_dCoverSd*2) / fabs(vAngSec.y);
	
	vector<REBAR_CYCLE>* pvArrCycle =  GetRebarCycleBySection(nSection);
	// 상하부별...
	for (long nUpDn=0; nUpDn<2; nUpDn++)
	{
		BOOL bUpper = nUpDn==0 ? TRUE : FALSE;

		double dDisDolchulL	= GetLengthMainDolchul(nSection, bUpper, TRUE);
		double dDisDolchulR	= GetLengthMainDolchul(nSection, bUpper, FALSE);

		// 사이클별...
		long nCyclesuTot = m_nMainCyclesu;

		if(nSection>=SECTION_SKEW_STT)	nCyclesuTot = 2;

		for (long nCycle=0; nCycle<nCyclesuTot; nCycle++)
		{
			dArrJoint.RemoveAll();
			dArrGugan.RemoveAll();
			dArrDia.RemoveAll();
			if(bUpper)
			{
				dArrGugan = pvArrCycle->at(nCycle).m_dArrJointUp;	// 구간별 길이 
				dArrDia   = pvArrCycle->at(nCycle).m_dArrDiaUp;
			}
			else
			{
				dArrGugan = pvArrCycle->at(nCycle).m_dArrJointDn;	// 구간별 길이 
				dArrDia   = pvArrCycle->at(nCycle).m_dArrDiaDn;
			}
		
			double dSumJoint = 0;
			for (long nGugansu=0; nGugansu<dArrGugan.GetSize(); nGugansu++)
			{
				dSumJoint += dArrGugan[nGugansu];
				dArrJoint.Add(dSumJoint);
			}

			for (long nJoint=0; nJoint<dArrJoint.GetSize(); nJoint++)
			{
				if(nJoint==0)							dCTCForJoint = GetCTCMainForJoint(bUpper, POS_SLAB_LEFT, nSection);
				else if(nJoint==dArrJoint.GetSize()-1)	dCTCForJoint = GetCTCMainForJoint(bUpper, POS_SLAB_RIGH, nSection);
				else									dCTCForJoint = GetCTCMainForJoint(bUpper, POS_SLAB_CENTER, nSection);
				
				double dDiaStt = dArrDia[nJoint];
				double dDiaEnd = dArrDia[nJoint+1];
				double dJointStt = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaStt, JOINT_TENS_NORMAL_B, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
				double dJointEnd = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaEnd, JOINT_TENS_NORMAL_B, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
				double dJoint    = max(dJointStt, dJointEnd);
				// 철근직경 변경 때문에 겹이음길이가 증가하여 최대철근길이를 초과할 경우만 이음위치를 재계산
				if(bUpper)	// 좌측에서 우측으로 최대길이 적용
				{
					if(nJoint==0)
					{
						double dLengthRebar = dArrJoint[nJoint] + dJoint/2 + dDisDolchulL;
						if(dLengthRebar > m_dLengthRebarMax)
							dArrJoint[nJoint] -= dLengthRebar-m_dLengthRebarMax;
					}
					else
					{
						double dLengthRebar = dArrJoint[nJoint] - dArrJoint[nJoint-1] + dJoint/2;
						if(dLengthRebar > m_dLengthRebarMax)
							dArrJoint[nJoint] -= dLengthRebar-m_dLengthRebarMax;
					}
				}
				else	// 우측에서 좌측으로 최대길이 적용
				{
					if(nJoint==dArrJoint.GetSize()-1)
					{
						double dLengthRebar = (dSlabW-dArrJoint[nJoint])+dJoint/2+dDisDolchulR;
						if(dLengthRebar> m_dLengthRebarMax)
							dArrJoint[nJoint] += dLengthRebar-m_dLengthRebarMax;
					}
					else
					{
						double dLengthRebar = dArrJoint[nJoint+1]-dArrJoint[nJoint]+dJoint/2;
						if(dLengthRebar > m_dLengthRebarMax)
							dArrJoint[nJoint] += dLengthRebar-m_dLengthRebarMax;
					}
				}
			}

			dArrJoint.Sort();

			dArrGugan.RemoveAll();
			for(long n=0; n<dArrJoint.GetSize(); n++)
			{
				if(n==0)
					dArrGugan.Add(dArrJoint[n]-0);
				else
					dArrGugan.Add(dArrJoint[n]-dArrJoint[n-1]);
			}
			if(bUpper)		pvArrCycle->at(nCycle).m_dArrJointUp = dArrGugan;
			else			pvArrCycle->at(nCycle).m_dArrJointDn = dArrGugan;
		}
	}
}




// 일반부 배력철근 간격을 계산함.
// 확폭 : 일반부의 시종점중 교폭이 작은 위치를 기준으로 간격계산후 나머지 위치는 m_dArrVariable옵션에 따라 재배분함.
// 일반 : 일반부의 시점위치
void CADeckData::MakeDimArrAtCenterStrength()
{
	CDPointArray xyArrDimUpStt, xyArrDimDnStt, xyArrDimUpEnd, xyArrDimDnEnd;
	double dExtLength = 0;

	dExtLength = MakeDimArrAtCenterStrengthUp(SECTION_CEN, xyArrDimUpStt);	
	MakeDimArrAtCenterStrengthDn(SECTION_CEN, xyArrDimDnStt, dExtLength);

	CDoubleArray dArrCTCUp, dArrCTCDn;
	CRebarPlacing RebarPlace;
	
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimUpStt, dArrCTCUp, FALSE);
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimDnStt, dArrCTCDn, FALSE);

	// 중복되는 CTC구간을 합침.
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCUp, xyArrDimUpStt);
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCDn, xyArrDimDnStt);
	// 저장
	SetCTCBeryukRebar(xyArrDimUpStt, SECTION_CEN, TRUE);
	SetCTCBeryukRebar(xyArrDimDnStt, SECTION_CEN, FALSE);
}



// 일반부 배력철근 간격을 계산함.
// 확폭 : 일반부의 시종점중 교폭이 작은 위치를 기준으로 간격계산후 나머지 위치는 m_dArrVariable옵션에 따라 재배분함.
// 일반 : 일반부의 시점위치
void CADeckData::MakeDimArrAtCenterExperience()
{
	CDPointArray xyArrDimUpStt, xyArrDimDnStt, xyArrDimUpEnd, xyArrDimDnEnd;	
	MakeDimArrAtSection(SECTION_CEN, xyArrDimUpStt, TRUE);	
	MakeDimArrAtSection(SECTION_CEN, xyArrDimDnStt, FALSE);

	CDoubleArray dArrCTCUp, dArrCTCDn;
	CRebarPlacing RebarPlace;
	
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimUpStt, dArrCTCUp, FALSE);
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimDnStt, dArrCTCDn, FALSE);

	// 중복되는 CTC구간을 합침.
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCUp, xyArrDimUpStt);
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCDn, xyArrDimDnStt);
	// 저장
	SetCTCBeryukRebar(xyArrDimUpStt, SECTION_CEN, TRUE);
	SetCTCBeryukRebar(xyArrDimDnStt, SECTION_CEN, FALSE);
}




double CADeckData::MakeDimArrAtCenterStrengthUp(long nSection, CDPointArray &xyArrDim)
{
	xyArrDim.RemoveAll();

	CPlateBasicIndex	*pBx = GetBxBySection(nSection);
	
	double dCTCLeft = m_dGenSupportRebarCTC[POS_SLAB_LEFT];
	double dCTCRigh = m_dGenSupportRebarCTC[POS_SLAB_RIGH];
	double dCTCCent = m_dGenSupportRebarCTC[POS_SLAB_CENTER];
	double dDistL	= GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDistR	= GetSideBarDistFromCenterGir(TRUE, FALSE);

	CDoubleArray dArrCutPoint;	
	double dRangeGir = m_dRangeDistribRebarOnGirder;
	double dDupL     = m_dRangeDistribRebarDup;	// 상하부 중복되는 배근범위

	CDPoint A[10], APre[10];
	for(long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);
		pGir->GetHunchXyDom(pBxMatch, A, FALSE);
		if(nG>0)
		{
			CPlateGirderApp *pGirPre = m_pDB->GetGirder(nG-1);
			CPlateBasicIndex *pBxMatchPre = pBx->GetBxMatchByCrossBeam(pGirPre);
			pGirPre->GetHunchXyDom(pBxMatchPre, APre, FALSE);
		}
		dArrCutPoint.Add(pGir->m_dGirderCenter-dRangeGir/2+dDupL);
		dArrCutPoint.Add(pGir->m_dGirderCenter+dRangeGir/2-dDupL);
		if(nG==0)
		{
			dArrCutPoint.Add(dDistL);
			if(m_pDB->GetGirdersu()==1)
				dArrCutPoint.Add(dDistR);
			else
				dArrCutPoint.Add(A[4].x+dDupL);
		}
		else if(nG==m_pDB->GetGirdersu()-1)
		{
			dArrCutPoint.Add(dDistR);
			dArrCutPoint.Add(APre[5].x-dDupL);
		}
		else
		{
			dArrCutPoint.Add(APre[5].x-dDupL);
			dArrCutPoint.Add(A[4].x+dDupL);
		}
	}
	dArrCutPoint.Sort();

	double dSttGugan = 0;
	double dEndGugan = 0;
	double dSumLength = dDistL;
	double dCTC = 0;	// CTC는 좁은 간격임.

	CDoubleArray dArrRebar;
	
	for (long n=0; n<dArrCutPoint.GetSize()-1; n++)
	{
		if(n==0)
			dSttGugan = dArrCutPoint[n];
		
		dEndGugan = dArrCutPoint[n+1];

		if(n==0)								dCTC = dCTCLeft;
		else if(n==dArrCutPoint.GetSize()-2)	dCTC = dCTCRigh;
		else		
		{
			if(n%2==0)	dCTC = dCTCCent;
			else		dCTC = dCTCCent*2;
		}

		for (long nn=0; nn<10000; nn++)
		{
			if(dSttGugan <= dSumLength && dSumLength <= dEndGugan)
			{
				dSumLength += dCTC;

				if(dSumLength > dEndGugan)
				{
					dSumLength -= dCTC;
					dSttGugan = dSumLength;
					break;
				}
				else if(dSumLength == dEndGugan)
				{
					dArrRebar.Add(dCTC);
					dSttGugan = dSumLength;
					break;
				}
				else
				{
					dArrRebar.Add(dCTC);
				}
			}
		}
	}
	CRebarPlacing RebarPlace;
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrRebar, xyArrDim);

	double dWidthExtra = dDistR - dSumLength;
	double dReturn = 0;
	if(m_nOptBeryukRebarArrange == CADeckData::SP_BOTH)
	{		
		if(dWidthExtra/2 > REBAR_MIN_SPACE)
		{
			xyArrDim.InsertAt(0, CDPoint(1,dWidthExtra/2));
			xyArrDim.Add(CDPoint(1,dWidthExtra/2));
			dReturn = dWidthExtra;
		}
		else
		{
			CDPoint xyDimStt = xyArrDim[0];
			CDPoint xyDimEnd = xyArrDim[xyArrDim.GetUpperBound()];
			xyArrDim.InsertAt(0, CDPoint(2.0, (dWidthExtra/2+xyDimStt.y)/2.0));	
			xyArrDim.SetAt	 (1, CDPoint(xyDimStt.x-1, xyDimStt.y));		
			
			xyArrDim.RemoveAt(xyArrDim.GetUpperBound());
			xyArrDim.Add(CDPoint(xyDimEnd.x-1, xyDimEnd.y));				
			xyArrDim.Add(CDPoint(2, (dWidthExtra/2+xyDimEnd.y)/2 ));
			dReturn = dWidthExtra/2+xyDimStt.y;
		}
	}
	else if(m_nOptBeryukRebarArrange == CADeckData::SP_LEFT)
	{
		if(dWidthExtra > REBAR_MIN_SPACE)
		{
			xyArrDim.InsertAt(0, CDPoint(1,dWidthExtra));
			dReturn = dWidthExtra;
		}
		else
		{
			CDPoint xyDimStt = xyArrDim[0];

			xyArrDim.InsertAt(0, CDPoint(2.0, (dWidthExtra+xyDimStt.y)/2.0));	
			xyArrDim.SetAt	 (1, CDPoint(xyDimStt.x-1, xyDimStt.y));		
			dReturn = dWidthExtra+xyDimStt.y;
		}
	}
	else if(m_nOptBeryukRebarArrange == CADeckData::SP_RIGHT)
	{
		if(dWidthExtra > REBAR_MIN_SPACE)
		{
			xyArrDim.Add(CDPoint(1, dWidthExtra));
			dReturn = dWidthExtra;
		}
		else
		{
			CDPoint xyDimEnd = xyArrDim[xyArrDim.GetUpperBound()];
			
			xyArrDim.RemoveAt(xyArrDim.GetUpperBound());
			xyArrDim.Add(CDPoint(xyDimEnd.x-1, xyDimEnd.y));				
			xyArrDim.Add(CDPoint(2, (dWidthExtra+xyDimEnd.y)/2 ));
			dReturn = dWidthExtra+xyDimEnd.y;
		}
	}

	return dReturn;
}



void CADeckData::MakeDimArrAtCenterStrengthDn(long nSection, CDPointArray &xyArrDim, double dExtLength)
{
	xyArrDim.RemoveAll();

	CPlateBasicIndex	*pBx = GetBxBySection(nSection);
	
	double dCTCLeft = m_dGenSupportRebarCTCLower[POS_SLAB_LEFT];
	double dCTCRigh = m_dGenSupportRebarCTCLower[POS_SLAB_RIGH];
	double dCTCCent = m_dGenSupportRebarCTCLower[POS_SLAB_CENTER];
	double dDistL	= GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDistR	= GetSideBarDistFromCenterGir(TRUE, FALSE);

	CDoubleArray dArrCutPoint;	
	double dRangeGir = m_dRangeDistribRebarOnGirder;
	double dDupL     = -m_dRangeDistribRebarDup;	// 상하부 중복되는 배근범위

	CDPoint A[10], APre[10];
	for(long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);
		pGir->GetHunchXyDom(pBxMatch, A, FALSE);
		if(nG>0)
		{
			CPlateGirderApp *pGirPre = m_pDB->GetGirder(nG-1);
			CPlateBasicIndex *pBxMatchPre = pBx->GetBxMatchByCrossBeam(pGirPre);
			pGirPre->GetHunchXyDom(pBxMatchPre, APre, FALSE);
		}
		dArrCutPoint.Add(pGir->m_dGirderCenter-dRangeGir/2+dDupL);
		dArrCutPoint.Add(pGir->m_dGirderCenter+dRangeGir/2-dDupL);
		if(nG==0)
		{
			dArrCutPoint.Add(dDistL);
			if(m_pDB->GetGirdersu()==1)
				dArrCutPoint.Add(dDistR);
			else
				dArrCutPoint.Add(A[4].x+dDupL);
		}
		else if(nG==m_pDB->GetGirdersu()-1)
		{
			dArrCutPoint.Add(dDistR);
			dArrCutPoint.Add(APre[5].x-dDupL);
		}
		else
		{
			dArrCutPoint.Add(APre[5].x-dDupL);
			dArrCutPoint.Add(A[4].x+dDupL);
		}

//		if(nG < m_pDB->GetGirdersu()-1)
//		{
//			long nHunchStt=11;
//			CPlateCrossBeam *pC = pBxMatch->GetCrossBeam();
//			for(long nStg=0; nStg<pC->m_nQtyStringer_CR; nStg++)
//			{
//				dArrCutPoint.Add(A[nHunchStt].x-dDupL);
//				dArrCutPoint.Add(A[nHunchStt+4].x+dDupL);			
//				nHunchStt+= 5;
//			}
//		}
	}

	dArrCutPoint.Sort();

	double dSttGugan = 0;
	double dEndGugan = 0;
	double dSumLength = 0;
	
	if(m_nOptBeryukRebarArrange == CADeckData::SP_BOTH)
		dSumLength = dDistL+dExtLength;
	else if(m_nOptBeryukRebarArrange == CADeckData::SP_LEFT)
		dSumLength = dDistL+dExtLength;
	else if(m_nOptBeryukRebarArrange == CADeckData::SP_RIGHT)
		dSumLength = dDistL;

	double dCTC = 0;	// CTC는 좁은 간격임.

	CDoubleArray dArrRebar;
	
	for (long n=0; n<dArrCutPoint.GetSize()-1; n++)
	{
		if(n==0)	dSttGugan = dSumLength;

		if(n==dArrCutPoint.GetSize()-2)
		{
			if(m_nOptBeryukRebarArrange == CADeckData::SP_BOTH)
				dEndGugan = dDistR - dExtLength;
			else if(m_nOptBeryukRebarArrange == CADeckData::SP_LEFT)
				dEndGugan = dDistR;
			else if(m_nOptBeryukRebarArrange == CADeckData::SP_RIGHT)
				dEndGugan = dDistR - dExtLength;
		}
		else
			dEndGugan = dArrCutPoint[n+1];

		if(n==0)								dCTC = dCTCLeft*2;
		else if(n==dArrCutPoint.GetSize()-2)	dCTC = dCTCRigh*2;
		else		
		{
			if(n%2==0)	dCTC = dCTCCent*2;
			else		dCTC = dCTCCent;
		}

		for (long nn=0; nn<10000; nn++)
		{
			if(dSttGugan <= dSumLength && dSumLength <= dEndGugan)
			{
				dSumLength += dCTC;

				if(n==dArrCutPoint.GetSize()-2 && dSumLength > dEndGugan)
				{
					dArrRebar.Add(dEndGugan-(dSumLength-dCTC));
					break;
				}
					
				if(dSumLength > dEndGugan)
				{
					dSumLength -= dCTC;
					dSttGugan = dSumLength;
					break;
				}
				else if(dSumLength == dEndGugan)
				{
					dArrRebar.Add(dCTC);
					dSttGugan = dSumLength;
					break;
				}
				else
				{
					dArrRebar.Add(dCTC);
				}
			}
		}
	}
	CRebarPlacing RebarPlace;
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrRebar, xyArrDim);

	if(m_nOptBeryukRebarArrange == CADeckData::SP_BOTH)
	{		
		xyArrDim.InsertAt(0, CDPoint(1,dExtLength));
		xyArrDim.Add(CDPoint(1,dExtLength));
	}
	else if(m_nOptBeryukRebarArrange == CADeckData::SP_LEFT)
		xyArrDim.InsertAt(0, CDPoint(1,dExtLength));
	else if(m_nOptBeryukRebarArrange == CADeckData::SP_RIGHT)
		xyArrDim.Add(CDPoint(1,dExtLength));

}


void CADeckData::MakeDimArrAtDanbu()
{
	CDPointArray xyArrDimUpStt, xyArrDimDnStt, xyArrDimUpEnd, xyArrDimDnEnd;
	CDoubleArray dArrVariableUpStt, dArrVariableDnStt, dArrVariableUpEnd, dArrVariableDnEnd;

	MakeDimArrAtSection(SECTION_DAN, xyArrDimUpStt, TRUE);

	MakeDimArrAtSection(SECTION_DAN, xyArrDimDnStt, FALSE);
	if(m_pDB->IsTUGir())
		MakeDimArrAtSectionTU(SECTION_DAN, xyArrDimDnStt, FALSE);		

	CDoubleArray dArrCTCUp, dArrCTCDn;
	CRebarPlacing RebarPlace;
	
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimUpStt, dArrCTCUp, FALSE);
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimDnStt, dArrCTCDn, FALSE);

	// 중복되는 CTC구간을 합침.
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCUp, xyArrDimUpStt);
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCDn, xyArrDimDnStt);
	// 저장
	SetCTCBeryukRebar(xyArrDimUpStt, SECTION_DAN, TRUE);
	SetCTCBeryukRebar(xyArrDimDnStt, SECTION_DAN, FALSE);
}


void CADeckData::MakeDimArrAtJijum()
{
	CDPointArray xyArrDimUpStt, xyArrDimDnStt, xyArrDimUpEnd, xyArrDimDnEnd;

	MakeDimArrAtSection(SECTION_PIER, xyArrDimUpStt, TRUE);	
	MakeDimArrAtSection(SECTION_PIER, xyArrDimDnStt, FALSE);

	CDoubleArray dArrCTCUp, dArrCTCDn;
	CRebarPlacing RebarPlace;
	
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimUpStt, dArrCTCUp, FALSE);
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDimDnStt, dArrCTCDn, FALSE);

	// 중복되는 CTC구간을 합침.
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCUp, xyArrDimUpStt);
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCDn, xyArrDimDnStt);
	// 저장
	SetCTCBeryukRebar(xyArrDimUpStt, SECTION_PIER, TRUE);
	SetCTCBeryukRebar(xyArrDimDnStt, SECTION_PIER, FALSE);
}



// 
void CADeckData::MakeDimArrAtSection(long nSection, CDPointArray &xyArrDim, BOOL bUpper)
{
	double dStaSec		= GetStationBySection(nSection);
	double dWidthSlab = m_pDB->GetWidthAct(dStaSec);
	double dCTCLeft		= 0;
	double dCTCRight	= 0;	
	double dCTCCenter	= 0;	
	if(nSection==SECTION_DAN)	
	{
		dCTCLeft	= bUpper ? m_dEndSupportRebarCTC[POS_SLAB_LEFT]   : m_dEndSupportRebarCTCLower[POS_SLAB_LEFT];	
		dCTCRight	= bUpper ? m_dEndSupportRebarCTC[POS_SLAB_RIGH]   : m_dEndSupportRebarCTCLower[POS_SLAB_RIGH];	
		dCTCCenter	= bUpper ? m_dEndSupportRebarCTC[POS_SLAB_CENTER] : m_dEndSupportRebarCTCLower[POS_SLAB_CENTER];	
	}
	else if(nSection==SECTION_PIER)
	{
		dCTCLeft	= bUpper ? m_dJijumRebarCTCUp[POS_SLAB_LEFT] : m_dJijumRebarCTCDn[POS_SLAB_LEFT];	
		dCTCRight	= bUpper ? m_dJijumRebarCTCUp[POS_SLAB_RIGH] : m_dJijumRebarCTCDn[POS_SLAB_RIGH];	
		dCTCCenter	= bUpper ? m_dJijumRebarCTCUp[POS_SLAB_CENTER] : m_dJijumRebarCTCDn[POS_SLAB_CENTER];	
	}
	else if(nSection==SECTION_CEN)	
	{
		dCTCLeft	= bUpper ? m_dGenSupportRebarCTC[POS_SLAB_LEFT] : m_dGenSupportRebarCTCLower[POS_SLAB_LEFT];	
		dCTCRight	= bUpper ? m_dGenSupportRebarCTC[POS_SLAB_RIGH] : m_dGenSupportRebarCTCLower[POS_SLAB_RIGH];	
		dCTCCenter	= bUpper ? m_dGenSupportRebarCTC[POS_SLAB_CENTER] : m_dGenSupportRebarCTCLower[POS_SLAB_CENTER];	
	}

	CPlateGirderApp  *pGirL = m_pDB->GetGirder(0);
	CPlateGirderApp  *pGirR = m_pDB->GetGirder(m_pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBx   = GetBxBySection(nSection);
	CPlateBasicIndex *pBxLeft = pBx->GetBxMatchByCrossBeam(pGirL);
	CPlateBasicIndex *pBxRigh = pBx->GetBxMatchByCrossBeam(pGirR);
	CPlateBasicIndex *pBxRighPre = pBx->GetBxMatchByCrossBeam(m_pDB->GetGirder(m_pDB->GetGirdersu()-2));
	CDPoint CLeft[4], CRigh[4];
	m_pDB->GetXyFrontCantilever(pBxLeft, CLeft, TRUE);
	m_pDB->GetXyFrontCantilever(pBxRighPre, CRigh, FALSE);

	long   nRebarLeftCant = (long)((CLeft[3].x - CLeft[0].x + m_dRangeDistribRebarOnCant ) / dCTCLeft);
	long   nRebarRighCant = (long)((CRigh[0].x - CRigh[3].x + m_dRangeDistribRebarOnCant ) / dCTCRight);
	double nRebarCenter   = (long)((dWidthSlab - (nRebarLeftCant*dCTCLeft + nRebarRighCant*dCTCRight)  - m_dCoverSd*2) / dCTCCenter);
	double dWidthExtra    = dWidthSlab - (nRebarLeftCant*dCTCLeft + nRebarRighCant*dCTCRight + nRebarCenter*dCTCCenter) - m_dCoverSd*2;

	xyArrDim.RemoveAll();
	if(dWidthExtra==0)
	{		
		xyArrDim.Add(CDPoint(nRebarLeftCant,dCTCLeft));		
		xyArrDim.Add(CDPoint(nRebarCenter,dCTCCenter));
		xyArrDim.Add(CDPoint(nRebarRighCant,dCTCRight));		
	}
	else
	{	
		if(m_nOptBeryukRebarArrange == SP_BOTH)
		{		
			if(dWidthExtra/2 > REBAR_MIN_SPACE)
			{
				xyArrDim.Add(CDPoint(1,dWidthExtra/2));
				xyArrDim.Add(CDPoint(nRebarLeftCant,dCTCLeft));		
				xyArrDim.Add(CDPoint(nRebarCenter,dCTCCenter));
				xyArrDim.Add(CDPoint(nRebarRighCant,dCTCRight));
				xyArrDim.Add(CDPoint(1,dWidthExtra/2));
			}
			else
			{
				xyArrDim.Add(CDPoint(2,(dWidthExtra/2+dCTCLeft)/2));				
				xyArrDim.Add(CDPoint(nRebarLeftCant-1,dCTCLeft));		
				xyArrDim.Add(CDPoint(nRebarCenter,dCTCCenter));
				xyArrDim.Add(CDPoint(nRebarRighCant-1,dCTCRight));				
				xyArrDim.Add(CDPoint(2,(dWidthExtra/2+dCTCLeft)/2));
			}
		}
		else if(m_nOptBeryukRebarArrange == SP_LEFT)
		{
			if(dWidthExtra > REBAR_MIN_SPACE)
			{
				xyArrDim.Add(CDPoint(1,dWidthExtra));
				xyArrDim.Add(CDPoint(nRebarLeftCant,dCTCLeft));		
				xyArrDim.Add(CDPoint(nRebarCenter,dCTCCenter));
				xyArrDim.Add(CDPoint(nRebarRighCant,dCTCRight));
			}
			else
			{
				xyArrDim.Add(CDPoint(2,(dWidthExtra+dCTCLeft)/2));				
				xyArrDim.Add(CDPoint(nRebarLeftCant-1,dCTCLeft));		
				xyArrDim.Add(CDPoint(nRebarCenter,dCTCCenter));
				xyArrDim.Add(CDPoint(nRebarRighCant,dCTCRight));
			}

		}
		else if(m_nOptBeryukRebarArrange == SP_RIGHT)
		{
			if(dWidthExtra > REBAR_MIN_SPACE)
			{
				xyArrDim.Add(CDPoint(nRebarLeftCant,dCTCLeft));		
				xyArrDim.Add(CDPoint(nRebarCenter,dCTCCenter));
				xyArrDim.Add(CDPoint(nRebarRighCant,dCTCRight));
				xyArrDim.Add(CDPoint(1,dWidthExtra));
			}
			else
			{
				xyArrDim.Add(CDPoint(nRebarLeftCant,dCTCLeft));		
				xyArrDim.Add(CDPoint(nRebarCenter,dCTCCenter));
				xyArrDim.Add(CDPoint(nRebarRighCant-1,dCTCRight));				
				xyArrDim.Add(CDPoint(2,(dWidthExtra+dCTCRight)/2));
			}
		}
	}

	CRebarPlacing RebarPlace;
	CDoubleArray dArrCTC;
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDim, dArrCTC, FALSE);
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTC, xyArrDim);
}

// TU거더의 배력철근 (현재 단부 하부만 생성함 081113)
void CADeckData::MakeDimArrAtSectionTU(long nSection, CDPointArray &xyArrDim, BOOL bUpper)
{
	CDoubleArray dArrCTC, dArrCTCTemp;
	CDPointArray xyArrGuide = GetXyArrGuideLineForMainRebar(nSection, bUpper);
	
	CRebarPlacing RebarPlace;	
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrDim, dArrCTC, FALSE);

	double dSum		= xyArrGuide.GetAt(0).x;
	double dSpace	= 0;

	for(long n=0; n<dArrCTC.GetSize(); n++)
	{
		dSum += dArrCTC.GetAt(n);
		BOOL bAdd = TRUE;

		for(long nG=0; nG<m_pDB->GetQtyGirder(); nG++)
		{
			double dStt = m_pDB->GetGirder(nG)->m_dGirderCenter-m_pDB->m_BindConc.m_dWidth/2;
			double dEnd = m_pDB->GetGirder(nG)->m_dGirderCenter+m_pDB->m_BindConc.m_dWidth/2;

			if(dSum>=dStt && dSum<=dEnd)
			{
				dSpace += dArrCTC.GetAt(n);
				bAdd = FALSE;
				break;
			}
		}

		if(bAdd && dSpace>0)
		{
			dArrCTCTemp.Add(dSpace+dArrCTC.GetAt(n));
			dSpace = 0;
		}
		else if(bAdd)
		{
			dArrCTCTemp.Add(dArrCTC.GetAt(n));
		}		
	}

	xyArrDim.RemoveAll();
	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrCTCTemp, xyArrDim);
}

// 주철근의 평면위치 배열을 구함
// bUpper : 상부, 하부
// nDrawSide : DRAW_LEFTSIDE  = 슬래브 중심기준 좌측
// nDrawSide : DRAW_RIGHTSIDE = 슬래브 중심기준 우측
// nDrawSide : DRAW_SIDEALL   = 슬래브 좌,우측 모두
// 횡단방향으로 슬래브의 양끝단이 시점, 슬래브 중심이 종점
void CADeckData::MakePlanMainRebar()
{
	CLineInfoApp *pLineApp = (CLineInfoApp*)m_pDB->m_pLine;

	m_vPlanMainRebar[0].clear();		// 전체 상부
	m_vPlanMainRebar[1].clear();		// 전체 하부
	m_vPlanMainRebarLeft[0].clear();	// 좌측 상부
	m_vPlanMainRebarLeft[1].clear();	// 좌측 하부
	m_vPlanMainRebarRigh[0].clear();	// 우측 상부
	m_vPlanMainRebarRigh[1].clear();	// 우측 하부

	// 슬래브 시종점면 각도
	CDPoint vAngSttSlab = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint vAngEndSlab = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);
	double  dStaSttLeft = GetStationMainRebarPos(TRUE,  -1);// 슬래브 좌측배력근과 슬래브시점면이 교차하는 위치의 스테이션
	double  dStaSttRigh = GetStationMainRebarPos(TRUE,   1);// 슬래브 우측배력근과 슬래브시점면이 교차하는 위치의 스테이션
	CDoubleArray dArrCtcL, dArrCtcR, dArrCtcC;	// 주철근간격
	CRebarPlacing RebarPlace;
	long nCyclesu	= 0;	// 해당단면의 사이클수
	long nRebarsu	= 0;	// 해당구간의 주철근수
	long nSectionCur= 0;	// 현재 단면
	double dDistStt = 0, dDistEnd = 0;	// 현재 주철근의 시종점의 선형에서 이격거리
	CVector xySlabLeft, xySlabRigh, xyRoadCen;				// 현재 주철근의 시종점 위치
	double dDistL		= 0;	// 선형중심에서 슬래브좌측배력철근까지의 이격거리
	double dDistR		= 0;	// 선형중심에서 슬래브우측배력철근까지의 이격거리
	double dDistLBySide = 0;	// 주철근이 그려지는 구간의 좌측이격거리
	double dDistRBySide = 0;	// 주철근이 그려지는 구간의 우측이격거리
	double dStaSttL  = 0;
	double dStaSttR  = 0;
	double dCoverSdSttL = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT);
	double dCoverSdSttR = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdSttC = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABCENTER);

	for (long nUpDn=0; nUpDn<2; nUpDn++)	// 상하부
	{
		BOOL bUpper = nUpDn==0 ? TRUE : FALSE;
		// 블럭아웃제원
		double dSttBlockWL= (m_bInstallMainRebarAtBlockOut==FALSE) ? GetWidthBlockOut(TRUE, JONGBASEPOS_SLABLEFT) : 0;
		double dSttBlockWR= (m_bInstallMainRebarAtBlockOut==FALSE) ? GetWidthBlockOut(TRUE, JONGBASEPOS_SLABRIGHT) : 0;
		double dSttBlockWC= (m_bInstallMainRebarAtBlockOut==FALSE) ? GetWidthBlockOut(TRUE, JONGBASEPOS_SLABCENTER) : 0;
		//
		RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_LEFT : CTC_MAIN_LOWER_LEFT], dArrCtcL, FALSE);
		RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_RIGHT : CTC_MAIN_LOWER_RIGHT], dArrCtcR, FALSE);
		RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebar[bUpper ? CTC_MAIN_UPPER_CENTER: CTC_MAIN_LOWER_CENTER], dArrCtcC, FALSE);

		if(dArrCtcL.GetSize()!=dArrCtcR.GetSize())	return;
		if(dArrCtcL.GetSize()!=dArrCtcC.GetSize())	return;
		if(dArrCtcR.GetSize()!=dArrCtcC.GetSize())	return;
		if(dArrCtcL.GetSize()==0 || dArrCtcR.GetSize()==0 || dArrCtcC.GetSize()==0)		return;

		//단부와 일반부를 구분함
		long nDanbuSttRebarsu = GetRebarSuAtDanbu(bUpper, TRUE);	// 시점단부 주철근수
		long nDanbuEndRebarsu = GetRebarSuAtDanbu(bUpper, FALSE);	// 종점단부 주철근수
		long nCenterRebarsu   = (dArrCtcL.GetSize()+1) - nDanbuSttRebarsu - nDanbuEndRebarsu;	// 중앙부 주철근수
		//////////////////////////////////////////////////////////////////////////
		// 0 : 단부시점~일반부시점   1: 일반부시점~일반부종점    2 : 일반부종점~단부종점 
		//////////////////////////////////////////////////////////////////////////
		for (long nDrawSide=DRAW_LEFTSIDE; nDrawSide<=DRAW_RIGHTSIDE; nDrawSide++)	// DrawSide별
		{
			long nRebarTotal = 0;		// 횡단주철근 한줄 
			for (long nSection=0; nSection<3; nSection++)	// 단면별
			{
				if(nSection==0)		// 단부시점~일반부시점
				{
					nRebarsu    = nDanbuSttRebarsu;
					nSectionCur = SECTION_DAN;
				}
				else if(nSection==1)	// 일반부시점~일반부종점
				{
					nRebarsu    = nCenterRebarsu;		
					nSectionCur = SECTION_CEN;	
				}
				else if(nSection==2)	// 일반부종점~단부종점
				{	
					nRebarsu    = nDanbuEndRebarsu;
					nSectionCur = SECTION_DAN;		
				}

				if(nRebarsu==0)		continue;
				
				nCyclesu = GetRebarCycleSuBySection(nSectionCur);
				if(nCyclesu==0)		continue;

				dDistL = GetSideBarDistFromCenterGir(nSectionCur, TRUE);
				dDistR = GetSideBarDistFromCenterGir(nSectionCur, FALSE);
				double dStaSttLBySide = 0;
				double dStaSttRBySide = 0;
				double dLenByDisL = 0;
				double dLenByDisR = 0;
				
				if(nDrawSide==DRAW_SIDEALL)
				{
					dDistLBySide = dDistL;
					dDistRBySide = dDistR;
					dStaSttLBySide = dStaSttLeft;
					dStaSttRBySide = dStaSttRigh;
					dLenByDisL = dCoverSdSttL+dSttBlockWL;
					dLenByDisR = dCoverSdSttR+dSttBlockWR;
				}
				else if(nDrawSide==DRAW_LEFTSIDE)
				{
					dDistLBySide = dDistL;
					dDistRBySide = (dDistL + dDistR)/2;
					dStaSttLBySide = dStaSttLeft;
					dStaSttRBySide = (dStaSttLeft+dStaSttRigh)/2;
					dLenByDisL = dCoverSdSttL+dSttBlockWL;
					dLenByDisR = dCoverSdSttC+dSttBlockWC;
				}
				else if(nDrawSide==DRAW_RIGHTSIDE)
				{
					dDistLBySide = (dDistL + dDistR)/2;
					dDistRBySide = dDistR;
					dStaSttLBySide = (dStaSttLeft+dStaSttRigh)/2;
					dStaSttRBySide = dStaSttRigh;
					dLenByDisL = dCoverSdSttC+dSttBlockWC;
					dLenByDisR = dCoverSdSttR+dSttBlockWR;
				}

				if(nRebarTotal==0)	// 첫번째 주철근일 경우
				{
					if(IsSkewStt())		// 사교 방사형
					{
						double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).GetAngleDegree();
						if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
						{
							dStaSttL = pLineApp->GetStationByDisLength(dStaSttLeft, dCoverSdSttL+dSttBlockWL, dDistL);	
							dStaSttR = dStaSttL;	
						}
						else
						{
							dStaSttR = pLineApp->GetStationByDisLength(dStaSttRigh, dCoverSdSttR+dSttBlockWR, dDistR);	
							dStaSttL = dStaSttR;	
						}
					}
					else
					{
						dStaSttL = pLineApp->GetStationByDisLength(dStaSttLBySide, dLenByDisL, dDistLBySide);	
						dStaSttR = pLineApp->GetStationByDisLength(dStaSttRBySide, dLenByDisR, dDistRBySide);	
					}
				}

				for (long nRebarAtSec=0; nRebarAtSec<nRebarsu;)		// 해당구간
				{
					for (long nCycle=0; nCycle<nCyclesu; nCycle++)
					{
						vector<REBAR_SHAPE> vRebarShape = GetNewRebarShapeArrBySection(nSectionCur, nCycle, bUpper);
						
						long nShapesu = vRebarShape.size();
						
						double dCtcL = 0;
						double dCtcR = 0;

						if (nDrawSide==DRAW_SIDEALL && nRebarTotal>0)
						{
							dCtcL = dArrCtcL[nRebarTotal-1];
							dCtcR = dArrCtcR[nRebarTotal-1];
						}
						else if (nDrawSide==DRAW_LEFTSIDE && nRebarTotal>0)
						{
							dCtcL = dArrCtcL[nRebarTotal-1];
							dCtcR = dArrCtcC[nRebarTotal-1];
						}
						else if (nDrawSide==DRAW_RIGHTSIDE && nRebarTotal>0)
						{
							dCtcL = dArrCtcC[nRebarTotal-1];
							dCtcR = dArrCtcR[nRebarTotal-1];
						}
						
						dStaSttL = pLineApp->GetStationByDisLength(dStaSttL, dCtcL, dDistLBySide);
						dStaSttR = pLineApp->GetStationByDisLength(dStaSttR, dCtcR, dDistRBySide);

						xySlabLeft = m_pDB->m_pLine->GetXyLineDis(dStaSttL, dDistLBySide);
						xySlabRigh = m_pDB->m_pLine->GetXyLineDis(dStaSttR, dDistRBySide);

						CDPoint vDir = (xySlabRigh-xySlabLeft).Unit();
						double dStaResult = 0;
						pLineApp->GetStationMatchByXyAndAngle(xySlabLeft, vDir, dStaResult);
						xyRoadCen  = pLineApp->GetXyLine(dStaResult);

						for (long nShape=0; nShape<nShapesu; nShape++)
						{
							//주철근존재유무 
							BOOL bValid = vRebarShape[nShape].m_bValid;

							dDistStt = vRebarShape[nShape].m_dArrShape[0].x;
							dDistEnd = vRebarShape[nShape].m_dArrShape[vRebarShape[nShape].m_dArrShape.GetUpperBound()].x;
							// dDistStt와 dDistEnd가 dDistLBySide ~ dDistRBySide 사이에서 투영되는 범위를 계산함.
							CDPoint xyResult1, xyResult2, xyResult3, xyResult4;
							BOOL bMatch1 = GetXyMatchSegAndLine(CDPoint(dDistLBySide, 0), CDPoint(dDistRBySide, 0), CDPoint(dDistStt, 0), CDPoint(0,1), xyResult1);
							BOOL bMatch2 = GetXyMatchSegAndLine(CDPoint(dDistLBySide, 0), CDPoint(dDistRBySide, 0), CDPoint(dDistEnd, 0), CDPoint(0,1), xyResult2);
							BOOL bMatch3 = GetXyMatchSegAndLine(CDPoint(dDistStt, 0), CDPoint(dDistEnd, 0), CDPoint(dDistLBySide, 0), CDPoint(0,1), xyResult3);
							BOOL bMatch4 = GetXyMatchSegAndLine(CDPoint(dDistStt, 0), CDPoint(dDistEnd, 0), CDPoint(dDistRBySide, 0), CDPoint(0,1), xyResult4);
							CDPointArray xyArrResult;
							if(bMatch1)	xyArrResult.Add(xyResult1);
							if(bMatch2)	xyArrResult.Add(xyResult2);
							if(bMatch3)	xyArrResult.Add(xyResult3);
							if(bMatch4)	xyArrResult.Add(xyResult4);

							xyArrResult.Sort();

							if (xyArrResult.GetSize()>=2)
							{
								dDistStt = xyArrResult[0].x;
								dDistEnd = xyArrResult[xyArrResult.GetUpperBound()].x;
							}
							else
								continue;

							CDPoint xyShapeStt = xyRoadCen + vDir*dDistStt;
							CDPoint xyShapeEnd = xyRoadCen + vDir*dDistEnd;
							
							if(bValid && ~(xyShapeStt-xyShapeEnd)>0)
							{
								REBAR_PLAN Rebar_Plan;
								if (nSection == 0)			Rebar_Plan.m_nPos = -1;
								else if (nSection == 1)		Rebar_Plan.m_nPos =  0;
								else if (nSection == 2)		Rebar_Plan.m_nPos =  1;
								
								CTwinVector tvShape;
								tvShape.m_v1 = bUpper ? xyShapeStt : xyShapeEnd;
								tvShape.m_v2 = bUpper ? xyShapeEnd : xyShapeStt;
								tvShape.m_v1.z = nRebarTotal;
								tvShape.m_v2.z = nRebarTotal;

								Rebar_Plan.m_nCycle = nCycle;
								Rebar_Plan.m_nShape = nShape;
								Rebar_Plan.m_tVector= tvShape;
								Rebar_Plan.m_sName  = vRebarShape[nShape].m_sName;

								if(nDrawSide==DRAW_SIDEALL)
									m_vPlanMainRebar[bUpper?0:1].push_back(Rebar_Plan);
								else if(nDrawSide==DRAW_LEFTSIDE)
									m_vPlanMainRebarLeft[bUpper?0:1].push_back(Rebar_Plan);
								else if(nDrawSide==DRAW_RIGHTSIDE)
									m_vPlanMainRebarRigh[bUpper?0:1].push_back(Rebar_Plan);
							}		
						}
						nRebarAtSec++;
						nRebarTotal++;
						
						if(nRebarAtSec >= nRebarsu)	break;
					}
				}
			}
		}
	}

	MakePlanUnderSinchuk();

}


// 사보강부 주철근 배열 구하지
// bUpper : 상부, 하부
// nDrawSide : DRAW_LEFTSIDE  = 슬래브 중심기준 좌측
// nDrawSide : DRAW_RIGHTSIDE = 슬래브 중심기준 우측
// nDrawSide : DRAW_SIDEALL   = 슬래브 좌,우측 모두
// 횡단방향으로 슬래브의 양끝단이 시점, 슬래브 중심이 종점
void CADeckData::MakePlanMainRebarSkew()
{
	m_vPlanMainRebarSkewStt[0].clear();
	m_vPlanMainRebarSkewEnd[0].clear();
	m_vPlanMainRebarSkewLeftStt[0].clear();
	m_vPlanMainRebarSkewLeftEnd[0].clear();
	m_vPlanMainRebarSkewRighStt[0].clear();
	m_vPlanMainRebarSkewRighEnd[0].clear();

	m_vPlanMainRebarSkewStt[1].clear();
	m_vPlanMainRebarSkewEnd[1].clear();
	m_vPlanMainRebarSkewLeftStt[1].clear();
	m_vPlanMainRebarSkewLeftEnd[1].clear();
	m_vPlanMainRebarSkewRighStt[1].clear();
	m_vPlanMainRebarSkewRighEnd[1].clear();

	CLineInfo *pLine = m_pDB->m_pLine;

	double dDistL  = GetSideBarDistFromCenterGir(SECTION_SKEW_STT, TRUE);
	double dDistR  = GetSideBarDistFromCenterGir(SECTION_SKEW_STT, FALSE);
	double dDistLBySide = 0;
	double dDistRBySide = 0;
	double dStaSttLeft = GetStationMainRebarPos(TRUE,  -1);
	double dStaEndLeft = GetStationMainRebarPos(FALSE, -1);		
	double dStaSttRigh = GetStationMainRebarPos(TRUE,   1);
	double dStaEndRigh = GetStationMainRebarPos(FALSE,  1);		
	double dStaSttCent = (dStaSttLeft+dStaSttRigh)/2;
	double dStaEndCent = (dStaEndLeft+dStaEndRigh)/2;
	double dSttBlockLeft = m_bInstallMainRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE,  JONGBASEPOS_SLABLEFT);
	double dEndBlockLeft = m_bInstallMainRebarAtBlockOut ? 0 : GetWidthBlockOut(FALSE, JONGBASEPOS_SLABLEFT);
	double dSttBlockRigh = m_bInstallMainRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE,  JONGBASEPOS_SLABRIGHT);
	double dEndBlockRigh = m_bInstallMainRebarAtBlockOut ? 0 : GetWidthBlockOut(FALSE, JONGBASEPOS_SLABRIGHT);
	double dSttBlockCent = m_bInstallMainRebarAtBlockOut ? 0 : GetWidthBlockOut(TRUE,  JONGBASEPOS_SLABCENTER);
	double dEndBlockCent = m_bInstallMainRebarAtBlockOut ? 0 : GetWidthBlockOut(FALSE, JONGBASEPOS_SLABCENTER);
	double dCoverSdSttL = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT);
	double dCoverSdEndL = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT);
	double dCoverSdSttR = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdEndR = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdSttC = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABCENTER);
	double dCoverSdEndC = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABCENTER);
	double dStaSttL=0, dStaSttR=0;
	CDPoint	vAngStt	 = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint	vAngEnd	 = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	CTwinVector tv;
	double dDistTotLine = GetDistByJongBase(JONGBASEPOS_SLABCENTER);

	CDPoint xyLeft, xyRigh, xyResult; 
	CDoubleArray dArrCtcL, dArrCtcR, dArrCtcC;
	CRebarPlacing RebarPlace;

	CTwinVectorArray tvArrPlane;
	for (long nUpDn=0; nUpDn<2; nUpDn++)		// 상하부
	{
		BOOL bUpper = nUpDn==0 ? TRUE : FALSE;
		for (long nSttEnd=0; nSttEnd<2; nSttEnd++)	// 시종점
		{
			BOOL bStt = nSttEnd==0 ? TRUE : FALSE; 

			for (long nDrawSide=DRAW_LEFTSIDE; nDrawSide<=DRAW_RIGHTSIDE; nDrawSide++)	// 횡단위치
			{
				tvArrPlane.RemoveAll();
	
				if(nDrawSide==DRAW_LEFTSIDE && bUpper==FALSE)	continue;
				if(nDrawSide==DRAW_RIGHTSIDE && bUpper==TRUE)	continue;

// 				if (bUpper==FALSE || m_bInstallMainRebarAtBlockOut)    dSttBlockLeft =  0;
// 				if (bUpper==FALSE || m_bInstallMainRebarAtBlockOut)    dEndBlockLeft =  0;
// 				if (bUpper==FALSE || m_bInstallMainRebarAtBlockOut)    dSttBlockRigh =  0;
// 				if (bUpper==FALSE || m_bInstallMainRebarAtBlockOut)    dEndBlockRigh =  0;
// 				if (bUpper==FALSE || m_bInstallMainRebarAtBlockOut)    dSttBlockCent =  0;
// 				if (bUpper==FALSE || m_bInstallMainRebarAtBlockOut)    dEndBlockCent =  0;
				if(bStt && IsSkewStt())		// 사교 방사형
				{
					double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB).GetAngleDegree();
					if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
					{
						dStaSttL = pLine->GetStationByDisLength(dStaSttLeft, dCoverSdSttL+dSttBlockLeft, dDistL);	
						dStaSttR = dStaSttL;	
					}
					else
					{
						dStaSttR = pLine->GetStationByDisLength(dStaSttRigh, dCoverSdSttR+dSttBlockRigh, dDistR);	
						dStaSttL = dStaSttR;	
					}
				}
				else if(!bStt && IsSkewEnd())		// 사교 방사형
				{
					double dAngle = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB).GetAngleDegree();
					if((0<dAngle && dAngle<90) || (180<dAngle && dAngle<270))
					{
						dStaSttR = pLine->GetStationByDisLength(dStaEndRigh, -dCoverSdEndR-dEndBlockRigh, dDistR);	
						dStaSttL = dStaSttR;	
					}
					else
					{
						dStaSttL = pLine->GetStationByDisLength(dStaEndLeft, -dCoverSdEndL-dEndBlockLeft, dDistL);	
						dStaSttR = dStaSttL;	
					}
				}
				else
				{
					if(nDrawSide==DRAW_SIDEALL)
					{
						dStaSttL = bStt ? pLine->GetStationByDisLength(dStaSttLeft, dCoverSdSttL+dSttBlockLeft, dDistLBySide) : pLine->GetStationByDisLength(dStaEndLeft, -dCoverSdEndL-dEndBlockLeft, dDistLBySide);	
						dStaSttR = bStt ? pLine->GetStationByDisLength(dStaSttRigh, dCoverSdSttR+dSttBlockRigh, dDistRBySide) : pLine->GetStationByDisLength(dStaEndRigh, -dCoverSdEndR-dEndBlockRigh, dDistRBySide);	
					}
					else if(nDrawSide==DRAW_LEFTSIDE)
					{
						dStaSttL = bStt ? pLine->GetStationByDisLength(dStaSttLeft, dCoverSdSttL+dSttBlockLeft, dDistLBySide) : pLine->GetStationByDisLength(dStaEndLeft, -dCoverSdEndL-dEndBlockLeft, dDistLBySide);	
						dStaSttR = bStt ? pLine->GetStationByDisLength(dStaSttCent, dCoverSdSttC+dSttBlockCent, dDistRBySide) : pLine->GetStationByDisLength(dStaEndCent, -dCoverSdEndC-dEndBlockCent, dDistRBySide);	
					}
					else if(nDrawSide==DRAW_RIGHTSIDE)
					{
						dStaSttL = bStt ? pLine->GetStationByDisLength(dStaSttCent, dCoverSdSttC+dSttBlockCent, dDistLBySide) : pLine->GetStationByDisLength(dStaEndCent, -dCoverSdEndC-dEndBlockCent, dDistLBySide);	
						dStaSttR = bStt ? pLine->GetStationByDisLength(dStaSttRigh, dCoverSdSttR+dSttBlockRigh, dDistRBySide) : pLine->GetStationByDisLength(dStaEndRigh, -dCoverSdEndR-dEndBlockRigh, dDistRBySide);	
					}
				}
				if(nDrawSide==DRAW_SIDEALL)
				{
					dDistLBySide = dDistL;
					dDistRBySide = dDistR;
				}
				else if(nDrawSide==DRAW_LEFTSIDE)
				{
					dDistLBySide = dDistL;
					dDistRBySide = (dDistL + dDistR)/2;
				}
				else if(nDrawSide==DRAW_RIGHTSIDE)
				{
					dDistLBySide = (dDistL + dDistR)/2;
					dDistRBySide = dDistR;
				}

				if(bStt)
				{
					RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[bUpper ? CTC_SKEWMAIN_UPPER_LEFT_STT : CTC_SKEWMAIN_LOWER_LEFT_STT], dArrCtcL, FALSE);
					RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[bUpper ? CTC_SKEWMAIN_UPPER_RIGH_STT: CTC_SKEWMAIN_LOWER_RIGH_STT], dArrCtcR, FALSE);
					RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[bUpper ? CTC_SKEWMAIN_UPPER_CENTER_STT: CTC_SKEWMAIN_LOWER_CENTER_STT], dArrCtcC, FALSE);
				}
				else
				{
					RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[bUpper ? CTC_SKEWMAIN_UPPER_LEFT_END : CTC_SKEWMAIN_LOWER_LEFT_END], dArrCtcL, FALSE);
					RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[bUpper ? CTC_SKEWMAIN_UPPER_RIGH_END : CTC_SKEWMAIN_LOWER_RIGH_END], dArrCtcR, FALSE);
					RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTCMainRebarAtSkew[bUpper ? CTC_SKEWMAIN_UPPER_CENTER_END: CTC_SKEWMAIN_LOWER_CENTER_END], dArrCtcC, FALSE);
				}

				if (dArrCtcL.GetSize() != dArrCtcR.GetSize())			return;
				if (dArrCtcL.GetSize() != dArrCtcC.GetSize())			return;
				if (dArrCtcR.GetSize() != dArrCtcC.GetSize())			return;
				
				long n = 0;
				for(n=0; n<dArrCtcL.GetSize(); n++)
				{
					double dCTCL = dArrCtcL[n];
					double dCTCR = dArrCtcR[n];
					double dCTCC = dArrCtcC[n];
					
					if(nDrawSide==DRAW_SIDEALL)
					{
						dStaSttL = pLine->GetStationByDisLength(dStaSttL, (bStt?-1:1)*dCTCL, dDistLBySide);
						dStaSttR = pLine->GetStationByDisLength(dStaSttR, (bStt?-1:1)*dCTCR, dDistRBySide);
					}
					else if(nDrawSide==DRAW_LEFTSIDE)
					{
						dStaSttL = pLine->GetStationByDisLength(dStaSttL, (bStt?-1:1)*dCTCL, dDistLBySide);
						dStaSttR = pLine->GetStationByDisLength(dStaSttR, (bStt?-1:1)*dCTCC, dDistRBySide);
					}
					else if(nDrawSide==DRAW_RIGHTSIDE)
					{
						dStaSttL = pLine->GetStationByDisLength(dStaSttL, (bStt?-1:1)*dCTCC, dDistLBySide);
						dStaSttR = pLine->GetStationByDisLength(dStaSttR, (bStt?-1:1)*dCTCR, dDistRBySide);
					}
					
					xyLeft = m_pDB->m_pLine->GetXyLineDis(dStaSttL, dDistLBySide);
					xyRigh = m_pDB->m_pLine->GetXyLineDis(dStaSttR, dDistRBySide);
					
					
					///////////////////////새로 작업 APLATE-704 사각부 주철근 평면도 배치를 위함 
					CTwinVectorArray tvArrBlock;
					CTwinVector tvBlock;
					CDPoint ABlockDan[4];
					if(bStt)
					{
						m_pDB->GetXySss((bUpper&&!m_bInstallMainRebarAtBlockOut)?OFF_STT_EXP:OFF_STT_SLAB, ABlockDan);
						Offset(ABlockDan[0], ABlockDan[3], -m_dCoverSd);
					}
					else
					{
						m_pDB->GetXySss((bUpper&&!m_bInstallMainRebarAtBlockOut)?OFF_END_EXP:OFF_END_SLAB, ABlockDan);
						Offset(ABlockDan[0], ABlockDan[3], m_dCoverSd);
					}

					tvBlock.m_v1 = ABlockDan[0];
					tvBlock.m_v2 = ABlockDan[3];
					tvArrBlock.Add(tvBlock);	

					//1. 철근 설치
					long nSectionCur	= SECTION_DAN; //시작단부쪽에만 모양이 세팅되어 있으므로 시작만 세팅 해주면 됨(단면상에서)
					long nCyclesu		= GetRebarCycleSuBySection(nSectionCur);

					CTwinVectorArray tvArrRebar;
					CTwinVector tvRebar;
					long nCycle = n % nCyclesu;
					vector<REBAR_SHAPE> vRebarShape = GetNewRebarShapeArrBySection(nSectionCur, nCycle, bUpper);
					long nShapesu = vRebarShape.size();
					for (long nShape =0; nShape < nShapesu; nShape++)
					{
						BOOL bValid = vRebarShape[nShape].m_bValid;
						if(bValid ==FALSE) continue;

						double dDistStt = vRebarShape[nShape].m_dArrShape[0].x;
						double dDistEnd = vRebarShape[nShape].m_dArrShape[vRebarShape[nShape].m_dArrShape.GetUpperBound()].x;

						CDPoint xyL = m_pDB->m_pLine->GetXyLineDis(dStaSttL, dDistStt);
						CDPoint xyR = m_pDB->m_pLine->GetXyLineDis(dStaSttR, dDistEnd);
						double dLen = ~(xyL - xyR);
						if(Compare(dLen, (double)0, _T("<="),0.1))
							continue;

						//추가 철근이 사각에 의해서 잘려지기 때문에 tv.m_sInfo= nCycle, tv.m_sInfoID= nShape 정보를 넣는다.

						tv.m_sInfo.Format(_T("%dCycle"), nCycle+1);
						tv.m_sInfoID.Format(_T("%dShape"), nShape+1);

						tv.m_v1 = xyL;
						tv.m_v2 = xyR;
						tvArrRebar.Add(tv);
					}

					//첫 철근의 방향대로 철근들을 구성해 준다
					if(tvArrRebar.GetSize() > 1)
					{
						CTwinVector tvFir, tvCurr;
						tvFir = tvArrRebar.GetAt(0);

						for (long ix = 1; ix < tvArrRebar.GetSize(); ix++)
						{
							tvCurr = tvArrRebar.GetAt(ix);
							if(ComparePt(tvFir.GetXyDir(), tvCurr.GetXyDir(), _T("="),0.1) == FALSE)
							{
								swap(tvCurr.m_v1, tvCurr.m_v2);
								tvArrRebar.SetAt(ix, tvCurr);
							}
						}
					}

					//2. 사보강 빗면부 Trim
					RebarPlace.TrimTvArrayByTv(tvArrRebar, &tvBlock, bStt? FALSE : TRUE);

					//3. 모든 구간  철근 저장(확정 되고 주석 제거)
					//tvArrPlaneAll.AddFromTvArray(tvArrRebar);

					//4. 좌측 => 상면, 우측=> 하면을 보여주므로..........좌측+하면 or 우측+상면 -> 철근 구한 것 삭제
					if((nDrawSide == DRAW_LEFTSIDE && !bUpper) || (nDrawSide == DRAW_RIGHTSIDE && bUpper))
						tvArrRebar.RemoveAll();
					if(tvArrRebar.GetSize() == 0)
						continue;



					//5. 중심선을 기점으로 넘어가는 선은 제거 (4번 반드시 실행)
					if(nDrawSide == DRAW_LEFTSIDE || nDrawSide == DRAW_RIGHTSIDE)
					{
						CDPoint xyCen	  = nDrawSide == DRAW_LEFTSIDE? xyRigh : xyLeft;

						for (long nIdx =0; nIdx < tvArrRebar.GetSize(); nIdx++)
						{
							tvRebar = tvArrRebar.GetAt(nIdx);
							double dDistCen1 = pLine->GetDisMatchVerticalByXy(tvRebar.m_v1);
							double dDistCen2 = pLine->GetDisMatchVerticalByXy(tvRebar.m_v2);

							dDistCen1 -= dDistTotLine;
							dDistCen2 -= dDistTotLine;

							double dDistFirst  = nDrawSide == DRAW_LEFTSIDE? dDistCen2 : dDistCen1;
							double dDistLast  = nDrawSide == DRAW_LEFTSIDE? dDistCen1 : dDistCen2;
							CString sSign	  = nDrawSide == DRAW_LEFTSIDE? _T(">=") : _T("<=");


							if(Compare(dDistFirst, (double)0, sSign,0.1))
							{
								if(Compare(dDistLast, (double)0, sSign,0.1))
								{
									tvArrRebar.RemoveAt(nIdx);
									nIdx--;
								}
								else
								{
									nDrawSide == DRAW_LEFTSIDE? tvRebar.m_v2 = xyCen : tvRebar.m_v1 = xyCen;
									tvArrRebar.SetAt(nIdx,tvRebar);
								}
							}	
						}
					}

					if(tvArrRebar.GetSize() ==0)
						continue;

					//(확정 되고 주석 제거)
					tvArrPlane.AddFromTvArray(tvArrRebar);
				}
				
			
				for ( n=0; n<tvArrPlane.GetSize(); n++)
				{
					REBAR_PLAN Rebar_Plan;
					Rebar_Plan.m_nPos   = bStt ? -1 : 1;
// 					Rebar_Plan.m_nCycle = 0;
// 					Rebar_Plan.m_nShape = 0;
					Rebar_Plan.m_tVector= tvArrPlane[n];
					Rebar_Plan.m_sName  = bUpper?"상부1":"하부1";

					CTwinVector tvRebar = tvArrPlane[n];
					long nCycleVal	= hgatol(tvRebar.m_sInfo);
					long nShapeVal = hgatol(tvRebar.m_sInfoID);
					Rebar_Plan.m_nCycle = nCycleVal-1;
					Rebar_Plan.m_nShape = nShapeVal-1;


					if(nDrawSide==DRAW_SIDEALL)
					{
						if(bStt)	m_vPlanMainRebarSkewStt[bUpper?0:1].push_back(Rebar_Plan);
						else		m_vPlanMainRebarSkewEnd[bUpper?0:1].push_back(Rebar_Plan);
					}
					else if(nDrawSide==DRAW_LEFTSIDE)
					{
						if(bStt)	m_vPlanMainRebarSkewLeftStt[bUpper?0:1].push_back(Rebar_Plan);
						else		m_vPlanMainRebarSkewLeftEnd[bUpper?0:1].push_back(Rebar_Plan);
					}
					else if(nDrawSide==DRAW_RIGHTSIDE)
					{
						if(bStt)	m_vPlanMainRebarSkewRighStt[bUpper?0:1].push_back(Rebar_Plan);
						else		m_vPlanMainRebarSkewRighEnd[bUpper?0:1].push_back(Rebar_Plan);
					}
				}
			}
		}
	}

}



// 사보강철근 배열 구하기
// 슬래브의 양끝단이 시점, 슬래브 중심이 종점
void CADeckData::MakePlanSabogang()
{
	m_vPlanSabogangStt[0].clear();
	m_vPlanSabogangStt[1].clear();
	m_vPlanSabogangEnd[0].clear();
	m_vPlanSabogangEnd[1].clear();
	m_vPlanSabogangLeftStt[0].clear();
	m_vPlanSabogangLeftStt[1].clear();
	m_vPlanSabogangLeftEnd[0].clear();
	m_vPlanSabogangLeftEnd[1].clear();
	m_vPlanSabogangRighStt[0].clear();
	m_vPlanSabogangRighStt[1].clear();
	m_vPlanSabogangRighEnd[0].clear();
	m_vPlanSabogangRighEnd[1].clear();

	double dCoverSdSttL = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT);
	double dCoverSdEndL = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT);
	double dCoverSdSttR = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdEndR = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT);
	double dDistLBySide= 0;	double dDistRBySide = 0;
	double dAddLenLeftBySide = 0;
	double dAddLenRighBySide = 0;

	CDPoint xySlabLeft, xySlabRigh, xyRoadCen;	// 슬래브좌측/우측/중앙
	CDoubleArray dArrCtc;	// 사보강철근간격
	CRebarPlacing RebarPlace;

	for (long nUpDn=0; nUpDn<2; nUpDn++)	// 상하부
	{
		BOOL bUpper = nUpDn==0 ? TRUE : FALSE;
		for (long nSttEnd=0; nSttEnd<2; nSttEnd++)	// 시종점
		{
			BOOL bStt = nSttEnd==0 ? TRUE : FALSE; 
			long nSection = bStt?SECTION_SKEW_STT:SECTION_SKEW_END;

			double dDistL = GetSideBarDistFromCenterGir(nSection, TRUE) ;	// 실거리로 계산
			double dDistR = GetSideBarDistFromCenterGir(nSection, FALSE);
			double dBlockLenL = bUpper&&!m_bInstallMainRebarAtBlockOut ? GetWidthBlockOut(bStt, JONGBASEPOS_SLABLEFT) : 0;
			double dBlockLenR = bUpper&&!m_bInstallMainRebarAtBlockOut ? GetWidthBlockOut(bStt, JONGBASEPOS_SLABRIGHT) : 0;
			double dBlockLenC = bUpper&&!m_bInstallMainRebarAtBlockOut ? GetWidthBlockOut(bStt, JONGBASEPOS_SLABCENTER) : 0;

			for (long nDrawSide=DRAW_LEFTSIDE; nDrawSide<=DRAW_RIGHTSIDE; nDrawSide++)	// 횡단위치
			{
				if(nDrawSide==DRAW_SIDEALL)
				{
					dDistLBySide = dDistL;
					dDistRBySide = dDistR;
					dAddLenLeftBySide = dBlockLenL;
					dAddLenRighBySide = dBlockLenR;
				}
				else if(nDrawSide==DRAW_LEFTSIDE)
				{
					dDistLBySide = dDistL;
					dDistRBySide = (dDistL + dDistR)/2;
					dAddLenLeftBySide = dBlockLenL;
					dAddLenRighBySide = dBlockLenC;
				}
				else if(nDrawSide==DRAW_RIGHTSIDE)
				{
					dDistLBySide = (dDistL + dDistR)/2;
					dDistRBySide = dDistR;
					dAddLenLeftBySide = dBlockLenC;
					dAddLenRighBySide = dBlockLenR;
				}

				RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(bUpper? m_arrCTCSabogangRebar[bStt?CTC_SABOGANG_UPPER_STT:CTC_SABOGANG_UPPER_END]:m_arrCTCSabogangRebar[bStt?CTC_SABOGANG_LOWER_STT:CTC_SABOGANG_LOWER_END], dArrCtc, FALSE);

				if(dArrCtc.GetSize()==0)	continue;
				long nCyclesu = GetRebarCycleSuBySection(nSection);		if(nCyclesu==0)		continue;
				long nRebarTotalsu = dArrCtc.GetSize()+1;

				for(long nRebar=0; nRebar<nRebarTotalsu; )		// 철근수별
				{
					CDPoint vAng  = m_pDB->GetGirder(-1)->GetAAA(bStt?OFF_STT_SLAB:OFF_END_SLAB, (dAddLenLeftBySide+dAddLenRighBySide)/2);
					for (long nCycle=0; nCycle<nCyclesu; nCycle++)	// 사이클별
					{
						vector<REBAR_SHAPE> vRebarShape = GetNewRebarShapeArrBySection(nSection, nCycle, bUpper);

						long nShapesu = vRebarShape.size();
						
						if(nRebar >= nRebarTotalsu)		break;
						
						if(nRebar > 0)
						{
							dAddLenLeftBySide += dArrCtc[nRebar-1];
							dAddLenRighBySide += dArrCtc[nRebar-1];
						}

						if(bStt)
						{		
							double dStaSlabSttCen = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);// 기준슬래브의 시점 스테이션
							CDPoint vAngSlabSttCen= m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);

							double dStaSlabSttLeft= m_pDB->m_pLine->GetStationByScInLine(dStaSlabSttCen, dDistLBySide, vAngSlabSttCen);
							double dStaSlabSttLeftOffset = m_pDB->m_pLine->GetStationByDisLength(dStaSlabSttLeft, dAddLenLeftBySide+dCoverSdSttL, dDistLBySide);

							double dStaSlabSttRigh= m_pDB->m_pLine->GetStationByScInLine(dStaSlabSttCen, dDistRBySide, vAngSlabSttCen);
							double dStaSlabSttRighOffset = m_pDB->m_pLine->GetStationByDisLength(dStaSlabSttRigh, dAddLenRighBySide+dCoverSdSttR, dDistRBySide);
				
							xySlabLeft = m_pDB->m_pLine->GetXyLineDis(dStaSlabSttLeftOffset, dDistLBySide);
							xySlabRigh = m_pDB->m_pLine->GetXyLineDis(dStaSlabSttRighOffset, dDistRBySide);

							double dStaCenOffset = 0;
							((CLineInfoApp*)m_pDB->m_pLine)->GetStationMatchByXyAndAngle(xySlabLeft, (xySlabLeft-xySlabRigh).Unit(), dStaCenOffset);
							xyRoadCen = m_pDB->m_pLine->GetXyLine(dStaCenOffset);
						}
						else
						{
							double dStaSlabEndCen = m_pDB->GetGirder(-1)->GetSSS(OFF_END_SLAB);// 기준슬래브의 시점 스테이션
							CDPoint vAngSlabEndCen= m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);
							
							double dStaSlabEndLeft= m_pDB->m_pLine->GetStationByScInLine(dStaSlabEndCen, dDistLBySide, vAngSlabEndCen);
							double dStaSlabEndLeftOffset = m_pDB->m_pLine->GetStationByDisLength(dStaSlabEndLeft, -dAddLenLeftBySide-dCoverSdEndL, dDistLBySide);
							
							double dStaSlabEndRigh= m_pDB->m_pLine->GetStationByScInLine(dStaSlabEndCen, dDistRBySide, vAngSlabEndCen);
							double dStaSlabEndRighOffset = m_pDB->m_pLine->GetStationByDisLength(dStaSlabEndRigh, -dAddLenRighBySide-dCoverSdEndR, dDistRBySide);

							xySlabLeft = m_pDB->m_pLine->GetXyLineDis(dStaSlabEndLeftOffset, dDistLBySide);
							xySlabRigh = m_pDB->m_pLine->GetXyLineDis(dStaSlabEndRighOffset, dDistRBySide);

							double dStaCenOffset = 0;
							((CLineInfoApp*)m_pDB->m_pLine)->GetStationMatchByXyAndAngle(xySlabLeft, (xySlabLeft-xySlabRigh).Unit(), dStaCenOffset);
							xyRoadCen = m_pDB->m_pLine->GetXyLine(dStaCenOffset);
						}

						for (long nShape=0; nShape<nShapesu; nShape++)		// Shape별
						{
							// 사보강철근 존재유무 
							BOOL bValid = vRebarShape[nShape].m_bValid;

							double dDistStt = vRebarShape[nShape].m_dArrShape[0].x;
							double dDistEnd = vRebarShape[nShape].m_dArrShape[vRebarShape[nShape].m_dArrShape.GetUpperBound()].x;
							// dDistStt와 dDistEnd가 dDistLBySide ~ dDistRBySide 사이에서 투영되는 범위를 계산함.
							CDPoint xyResult1, xyResult2, xyResult3, xyResult4;
							BOOL bMatch1 = GetXyMatchSegAndLine(CDPoint(dDistLBySide, 0), CDPoint(dDistRBySide, 0), CDPoint(dDistStt, 0), CDPoint(0,1), xyResult1);
							BOOL bMatch2 = GetXyMatchSegAndLine(CDPoint(dDistLBySide, 0), CDPoint(dDistRBySide, 0), CDPoint(dDistEnd, 0), CDPoint(0,1), xyResult2);
							BOOL bMatch3 = GetXyMatchSegAndLine(CDPoint(dDistStt, 0), CDPoint(dDistEnd, 0), CDPoint(dDistLBySide, 0), CDPoint(0,1), xyResult3);
							BOOL bMatch4 = GetXyMatchSegAndLine(CDPoint(dDistStt, 0), CDPoint(dDistEnd, 0), CDPoint(dDistRBySide, 0), CDPoint(0,1), xyResult4);
							CDPointArray xyArrResult;
							if(bMatch1)	xyArrResult.Add(xyResult1);
							if(bMatch2)	xyArrResult.Add(xyResult2);
							if(bMatch3)	xyArrResult.Add(xyResult3);
							if(bMatch4)	xyArrResult.Add(xyResult4);

							xyArrResult.Sort();

							if (xyArrResult.GetSize()>=2)
							{
								dDistStt = xyArrResult[0].x;
								dDistEnd = xyArrResult[xyArrResult.GetUpperBound()].x;
							}
							else
								continue;

							CDPoint vDir = (xySlabRigh-xySlabLeft).Unit();
							double  dLen = ~(xySlabLeft-xySlabRigh);
							double  dDistRatioStt = Bogan(dDistLBySide, dDistRBySide, 0, 1, dDistStt);
							double  dDistRatioEnd = Bogan(dDistLBySide, dDistRBySide, 0, 1, dDistEnd);
							CDPoint xyShapeStt = xySlabLeft + vDir*dDistRatioStt*dLen;
							CDPoint xyShapeEnd = xySlabLeft + vDir*dDistRatioEnd*dLen;
							
							if(bValid && ~(xyShapeStt-xyShapeEnd)>0)
							{
								REBAR_PLAN Rebar_Plan;
								Rebar_Plan.m_nPos   = bStt ? -1 : 1;
								Rebar_Plan.m_nCycle = nCycle;
								Rebar_Plan.m_nShape = nShape;
								Rebar_Plan.m_tVector= bUpper ? CTwinVector(xyShapeStt, xyShapeEnd) : CTwinVector(xyShapeEnd, xyShapeStt); 
								Rebar_Plan.m_sName  = vRebarShape[nShape].m_sName;

								if(nDrawSide==DRAW_SIDEALL)
								{
									if(bStt)	m_vPlanSabogangStt[bUpper?0:1].push_back(Rebar_Plan);
									else		m_vPlanSabogangEnd[bUpper?0:1].push_back(Rebar_Plan);
								}
								else if(nDrawSide==DRAW_LEFTSIDE)
								{
									if(bStt)	m_vPlanSabogangLeftStt[bUpper?0:1].push_back(Rebar_Plan);
									else		m_vPlanSabogangLeftEnd[bUpper?0:1].push_back(Rebar_Plan);
								}
								else if(nDrawSide==DRAW_RIGHTSIDE)
								{
									if(bStt)	m_vPlanSabogangRighStt[bUpper?0:1].push_back(Rebar_Plan);
									else		m_vPlanSabogangRighEnd[bUpper?0:1].push_back(Rebar_Plan);
								}

							}	
						}
						nRebar++;
					}
				}
			}
		}
	}
}



void CADeckData::MakePlanUnderSinchuk()
{
	m_vPlanUnderSinchukStt.clear();
	m_vPlanUnderSinchukEnd.clear();
	m_vPlanUnderSinchukLeftStt.clear();
	m_vPlanUnderSinchukLeftEnd.clear();
	m_vPlanUnderSinchukRighStt.clear();
	m_vPlanUnderSinchukRighEnd.clear();

	CPlateBridgeApp	*pDB	= m_pDB;
	CLineInfo   *pLine  = pDB->m_pLine;
	CPlateGirderApp	*pGir	= pDB->GetGirder(-1);
	CPlateGirderApp	*pLGir	= pDB->GetGirder(0);
	CPlateGirderApp	*pRGir	= pDB->GetGirder(pDB->GetGirdersu()-1);

	double dDistL = GetSideBarDistFromCenterGir(TRUE, TRUE);
	double dDistR = GetSideBarDistFromCenterGir(TRUE, FALSE);
	double dDistC = 0;
	double dDistLBySide= 0;	double dDistRBySide = 0;

	CPlateBasicIndex *pBxStt = pGir->GetBxOnJijum(0);
	CPlateBasicIndex *pBxEnd = pGir->GetBxOnJijum(pDB->m_nQtyJigan-1);

	double dStaStt = pBxStt->GetGirder()->GetSSS(OFF_STT_SLAB);
	double dCoverSdSttL = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABLEFT);
	double dCoverSdEndL = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABLEFT);
	double dCoverSdSttR = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdEndR = GetLengthCoverSd(FALSE, JONGBASEPOS_SLABRIGHT);
	double dCoverSdSttC = GetLengthCoverSd(TRUE, JONGBASEPOS_SLABCENTER);

	for (long nDrawSide=DRAW_LEFTSIDE; nDrawSide<=DRAW_RIGHTSIDE; nDrawSide++)	// 횡단위치
	{
		if(nDrawSide==DRAW_SIDEALL)
		{
			dDistLBySide = dDistL;
			dDistRBySide = dDistR;
		}
		else if(nDrawSide==DRAW_LEFTSIDE)
		{
			dDistLBySide = dDistL;
			dDistRBySide = (dDistL + dDistR)/2;
		}
		else if(nDrawSide==DRAW_RIGHTSIDE)
		{
			dDistLBySide = (dDistL + dDistR)/2;
			dDistRBySide = dDistR;
		}
		dDistC = (dDistL+dDistR)/2;
		
		CDPoint xyShapeStt, xyShapeEnd;
		double dBaseDist = dDistC;
		double dSta = pLine->GetStationByDisLength(dStaStt, dCoverSdSttC, dBaseDist);	
		double dStaL=0, dStaR=0;

		for (long nSttEnd=0; nSttEnd<2; nSttEnd++)
		{
			double dAddLen=0;
			BOOL bStt = nSttEnd==0 ? TRUE : FALSE;

			if(bStt)
			{
				xyShapeStt = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB,dCoverSdSttL),dDistLBySide,pGir->GetAAA(OFF_STT_SLAB,dCoverSdSttL));
				xyShapeEnd = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SLAB,dCoverSdSttR),dDistRBySide,pGir->GetAAA(OFF_STT_SLAB,dCoverSdSttR));

				REBAR_PLAN Rebar_PlanStt;
				Rebar_PlanStt.m_nPos   = bStt ? -1 : 1;
				Rebar_PlanStt.m_nCycle = 0;
				Rebar_PlanStt.m_nShape = 0;
				Rebar_PlanStt.m_sName  = "";
				Rebar_PlanStt.m_tVector= CTwinVector(xyShapeStt, xyShapeEnd); 

				if(nDrawSide==DRAW_SIDEALL)
				{
					if(bStt)	m_vPlanUnderSinchukStt.push_back(Rebar_PlanStt);
					else		m_vPlanUnderSinchukEnd.push_back(Rebar_PlanStt);
				}
				else if(nDrawSide==DRAW_LEFTSIDE)
				{
					if(bStt)	m_vPlanUnderSinchukLeftStt.push_back(Rebar_PlanStt);
					else		m_vPlanUnderSinchukLeftEnd.push_back(Rebar_PlanStt);
				}
				else if(nDrawSide==DRAW_RIGHTSIDE)
				{
					if(bStt)	m_vPlanUnderSinchukRighStt.push_back(Rebar_PlanStt);
					else		m_vPlanUnderSinchukRighEnd.push_back(Rebar_PlanStt);
				}

				for(long n=0; n<m_arrCTCMainRebarUnderSinchuk.GetSize(); n++)
				{		
					CDPoint xyDim = m_arrCTCMainRebarUnderSinchuk.GetAt(n);		
					for(long i=0; i<xyDim.x; i++)
					{
						dSta = pLine->GetStationByDisLength(dSta, xyDim.y, dBaseDist);
						dAddLen += xyDim.y;
						
						dStaL = pLine->GetStationByScInLine(pGir->GetSSS(OFF_STT_SLAB,dCoverSdSttL+dAddLen),dDistLBySide,pGir->GetAAA(OFF_STT_SLAB, dCoverSdSttL+dAddLen));
						dStaR = pLine->GetStationByScInLine(pGir->GetSSS(OFF_STT_SLAB,dCoverSdSttR+dAddLen),dDistRBySide,pGir->GetAAA(OFF_STT_SLAB, dCoverSdSttR+dAddLen));
						xyShapeStt = pLine->GetXyLineDis(dStaL, dDistLBySide);
						xyShapeEnd = pLine->GetXyLineDis(dStaR, dDistRBySide);

						REBAR_PLAN Rebar_Plan;
						Rebar_Plan.m_nPos   = bStt ? -1 : 1;
						Rebar_Plan.m_nCycle = 0;
						Rebar_Plan.m_nShape = 0;
						Rebar_Plan.m_sName  = "";
						Rebar_Plan.m_tVector= CTwinVector(xyShapeStt, xyShapeEnd); 
						
						if(nDrawSide==DRAW_SIDEALL)
						{
							if(bStt)	m_vPlanUnderSinchukStt.push_back(Rebar_Plan);
							else		m_vPlanUnderSinchukEnd.push_back(Rebar_Plan);
						}
						else if(nDrawSide==DRAW_LEFTSIDE)
						{
							if(bStt)	m_vPlanUnderSinchukLeftStt.push_back(Rebar_Plan);
							else		m_vPlanUnderSinchukLeftEnd.push_back(Rebar_Plan);
						}
						else if(nDrawSide==DRAW_RIGHTSIDE)
						{
							if(bStt)	m_vPlanUnderSinchukRighStt.push_back(Rebar_Plan);
							else		m_vPlanUnderSinchukRighEnd.push_back(Rebar_Plan);
						}				
					}		
				}
			}
			else
			{
				xyShapeStt = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB,-dCoverSdEndL),dDistLBySide,pGir->GetAAA(OFF_END_SLAB,-dCoverSdEndL));
				xyShapeEnd = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SLAB,-dCoverSdEndR),dDistRBySide,pGir->GetAAA(OFF_END_SLAB,-dCoverSdEndR));

				REBAR_PLAN Rebar_PlanStt;
				Rebar_PlanStt.m_nPos   = bStt ? -1 : 1;
				Rebar_PlanStt.m_nCycle = 0;
				Rebar_PlanStt.m_nShape = 0;
				Rebar_PlanStt.m_sName  = "";
				Rebar_PlanStt.m_tVector= CTwinVector(xyShapeStt, xyShapeEnd); 
				if(nDrawSide==DRAW_SIDEALL)
				{
					if(bStt)	m_vPlanUnderSinchukStt.push_back(Rebar_PlanStt);
					else		m_vPlanUnderSinchukEnd.push_back(Rebar_PlanStt);
				}
				else if(nDrawSide==DRAW_LEFTSIDE)
				{
					if(bStt)	m_vPlanUnderSinchukLeftStt.push_back(Rebar_PlanStt);
					else		m_vPlanUnderSinchukLeftEnd.push_back(Rebar_PlanStt);
				}
				else if(nDrawSide==DRAW_RIGHTSIDE)
				{
					if(bStt)	m_vPlanUnderSinchukRighStt.push_back(Rebar_PlanStt);
					else		m_vPlanUnderSinchukRighEnd.push_back(Rebar_PlanStt);
				}
				
				for(long n=0; n<m_arrCTCMainRebarUnderSinchuk.GetSize(); n++)
				{		
					CDPoint xyDim = m_arrCTCMainRebarUnderSinchuk.GetAt(n);		
					for(long i=0; i<xyDim.x; i++)
					{
						if(m_nJongCtcCalcPos==0)
							dSta -= xyDim.y;
						else
							dSta = pLine->GetStationByDisLength(dSta, -xyDim.y, dBaseDist);
						dAddLen -= xyDim.y;
						dStaL = pLine->GetStationByScInLine(pGir->GetSSS(OFF_END_SLAB,-dCoverSdEndL+dAddLen),dDistLBySide,pGir->GetAAA(OFF_END_SLAB, -dCoverSdEndL+dAddLen));
						dStaR = pLine->GetStationByScInLine(pGir->GetSSS(OFF_END_SLAB,-dCoverSdEndR+dAddLen),dDistRBySide,pGir->GetAAA(OFF_END_SLAB, -dCoverSdEndR+dAddLen));
						xyShapeStt = pLine->GetXyLineDis(dStaL, dDistLBySide);
						xyShapeEnd = pLine->GetXyLineDis(dStaR, dDistRBySide);

						REBAR_PLAN Rebar_Plan;
						Rebar_Plan.m_nPos   = bStt ? -1 : 1;
						Rebar_Plan.m_nCycle = 0;
						Rebar_Plan.m_nShape = 0;
						Rebar_Plan.m_sName  = "";
						Rebar_Plan.m_tVector= CTwinVector(xyShapeStt, xyShapeEnd); 		

						if(nDrawSide==DRAW_SIDEALL)
						{
							if(bStt)	m_vPlanUnderSinchukStt.push_back(Rebar_Plan);
							else		m_vPlanUnderSinchukEnd.push_back(Rebar_Plan);
						}
						else if(nDrawSide==DRAW_LEFTSIDE)
						{
							if(bStt)	m_vPlanUnderSinchukLeftStt.push_back(Rebar_Plan);
							else		m_vPlanUnderSinchukLeftEnd.push_back(Rebar_Plan);
						}
						else if(nDrawSide==DRAW_RIGHTSIDE)
						{
							if(bStt)	m_vPlanUnderSinchukRighStt.push_back(Rebar_Plan);
							else		m_vPlanUnderSinchukRighEnd.push_back(Rebar_Plan);
						}	
					}		
				}		
			}
		}
	}
}


BOOL CADeckData::IsInstallBeryukRebarAtCenter(long nCycle)
{
	long nBesu    = GetBesu(JONGBASEPOS_SLABCENTER, TRUE);
	BOOL bInstall = FALSE;

	if(m_pDB->m_nQtyJigan > 1)	// 다경간
	{
		if(nBesu==2)
		{
			if(m_nUseJujangRebar==0)		// 총철근량의 1/3을 순지간의 1/16까지 연장배근
			{
				if(nCycle==0 || nCycle==2 || nCycle==4)		bInstall = TRUE;
				else										bInstall = FALSE;
			}
			else
			{
				if(nCycle==0)	bInstall = TRUE;
				else			bInstall = FALSE;
			}
		}
		else if(nBesu==3)
		{
			if(m_nUseJujangRebar==0)
			{
				if(nCycle==0)	bInstall = TRUE;
				else			bInstall = FALSE;
			}
			else
			{
				if(nCycle==0 || nCycle==3)	bInstall = TRUE;
				else						bInstall = FALSE;
			}
		}
		else if(nBesu==1)
		{
			if(nCycle==0)	bInstall = TRUE;
			else			bInstall = FALSE;
		}
	}
	else	// 단경간일 경우
	{
		if(nBesu==2)
		{
			if(nCycle==0 )	bInstall = TRUE;
			else			bInstall = FALSE;
		}
		else
		{
			bInstall = TRUE;
		}
	}
	
	return bInstall;
}

void CADeckData::MakeShapeBeryukRebar()
{
	long nJongBaseEnd = IsBridgeHasCrossBeam() ? JONGBASEPOS_ONCB : JONGBASEPOS_SLABRC;
	for (long nJongBasePos=0; nJongBasePos<=nJongBaseEnd; nJongBasePos++)
	{
		if(IsCycleValidJong(nJongBasePos)==FALSE)	return;

		RemoveRebarShapeByJongBasePos(nJongBasePos);

		long nCycleSu = GetRebarCycleSuByJongBasePos(nJongBasePos);
		for (long nCycle=0; nCycle<nCycleSu; nCycle++)
		{
			vector<REBAR_SHAPE> vRebarShapeUp, vRebarShapeDn; 
			MakeShapeBeryukRebarUp(nJongBasePos, nCycle, vRebarShapeUp);
			MakeShapeBeryukRebarDn(nJongBasePos, nCycle, vRebarShapeDn);
	
			vector<REBAR_SHAPE> vRebarShapeTot; 
			long n = 0;
			for (n=0; n<(long)vRebarShapeUp.size(); n++)
				vRebarShapeTot.push_back(vRebarShapeUp[n]);
			for ( n=0; n<(long)vRebarShapeDn.size(); n++)
				vRebarShapeTot.push_back(vRebarShapeDn[n]);

			AddRebarShapeArrAtJongBasePos(nJongBasePos, vRebarShapeTot);
		}
	}
}

void CADeckData::MakeShapeBeryukRebarUp(long nJongBasePos, long nCycle, vector <REBAR_SHAPE>& vRebarShape)
{
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	
	//////////////////////////////////////////////////////////////////////////
	// 종단이음위치 계산
	//////////////////////////////////////////////////////////////////////////
	REBAR_SHAPE stRebarShape;

	CDPointArray xyArrGuide = GetXyArrGuideLineForBeryukRebar(nJongBasePos, TRUE);
	CDPointArray xyArrJoint = GetXyArrJointPosAtJongBasePos(nJongBasePos, nCycle, TRUE);	// 이음중심 위치

	//////////////////////////////////////////////////////////////////////////
	// SHAPE 계산
	//////////////////////////////////////////////////////////////////////////
	CDPoint vX(1,0), vY(0,1);
	CString sRebar;

	double dSlabT		    = m_pDB->m_dThickSlabBase;
	double dThickPave		= m_pDB->GetThickPave();
	double dEndExpW			= GetWidthBlockOut(FALSE, nJongBasePos);
	double dEndExpH			= pGlopt->GetEndExpansionJointHeight()-dThickPave;
	if(m_bInstallMainRebarAtBlockOut)
	{
		dEndExpW = 0, dEndExpH = 0;
	}
	long nPos = 0;	// 0=좌측, 1=우측, 2=중앙부
	if     (nJongBasePos==JONGBASEPOS_SLABCENTER || nJongBasePos==JONGBASEPOS_SLABLC || nJongBasePos==JONGBASEPOS_SLABRC)	nPos = POS_SLAB_CENTER;
	else if(nJongBasePos == JONGBASEPOS_SLABLEFT)		nPos = POS_SLAB_LEFT;
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		nPos = POS_SLAB_RIGH;
	else if(nJongBasePos == JONGBASEPOS_ONCB)			nPos = POS_SLAB_CENTER;

	// 직경
	double dDiaDanbuBeryuk= m_dDiaBeryukRebar_Dan[nPos];		// 단부 상면 배력철근 직경	
	double dDiaGenBeryuk  = m_dDiaBeryukRebar_Cent[nPos];		// 일반부 상면 배력철근 직경
	double dDiaJijumBeryuk= m_dDiaJijumRebarUp[nPos];			// 지점부 상면 배력철근 직경

	// 이음장길이
	double dLJointDan		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuBeryuk,	JOINT_TENS_NORMAL_B, toCm(m_dCoverUp), toCm(m_dEndSupportRebarCTC[nPos]));
	double dLJointGen		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenBeryuk,	JOINT_TENS_NORMAL_B, toCm(m_dCoverUp), toCm(m_dGenSupportRebarCTC[nPos]));
	double dLJointJijum  	= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaJijumBeryuk,	JOINT_TENS_NORMAL_B, toCm(m_dCoverUp), toCm(m_dJijumRebarCTCUp[nPos]));
	double dLJointDanGen	= max(dLJointDan, dLJointGen);
	double dLJointGenJijum  = max(dLJointJijum, dLJointGen);

	// 중앙부 철근 설치 유무
	BOOL bInstallRebarCen = IsInstallBeryukRebarAtCenter(nCycle);
	CDPointArray xyArrRebar;
	CDPoint xyPoint;
	CString	szText = _T("");

	// 시점부 상부 배력철근
	xyPoint = xyArrGuide[0]-vY*(dSlabT+m_pDB->m_dHeightTaperStt-m_dCoverUp-m_dCoverDn);
	xyArrRebar.Add(xyPoint);

	xyPoint = xyArrGuide[0];
	xyArrRebar.Add(xyPoint);

	xyPoint.x = xyArrJoint[1].x+dLJointDanGen/2;
	xyArrRebar.Add(xyPoint);

	sRebar.Format("시점부상부");
	stRebarShape.m_sName     = sRebar;
	stRebarShape.m_dArrShape = xyArrRebar;
	stRebarShape.m_bValid    = TRUE;
	vRebarShape.push_back(stRebarShape);
	xyArrRebar.RemoveAll();

	// 지점부 상부 배력철근
	for(long nP=0; nP<m_pDB->m_nQtyJigan-1; nP++)
	{
		xyPoint = CDPoint(xyArrJoint[nP*3+2].x - dLJointGenJijum/2, -m_dCoverUp);
		xyArrRebar.Add(xyPoint);		

		xyPoint = CDPoint(xyArrJoint[nP*3+3].x, -m_dCoverUp);
		xyArrRebar.Add(xyPoint);		

		xyPoint = CDPoint(xyArrJoint[nP*3+4].x + dLJointGenJijum/2, -m_dCoverUp);
		xyArrRebar.Add(xyPoint);		

		sRebar.Format("지점부상부(%d)", nP+1);
		stRebarShape.m_sName     = sRebar;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bValid    = TRUE;
		vRebarShape.push_back(stRebarShape);
		xyArrRebar.RemoveAll();
	}

	// 종점부 상부 배력철근
	xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()]-vY*(dSlabT+m_pDB->m_dHeightTaperEnd-m_dCoverUp-m_dCoverDn);
	xyArrRebar.Add(xyPoint);

	xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()];
	xyArrRebar.Add(xyPoint);

	xyPoint.x = xyArrJoint[xyArrJoint.GetUpperBound()-1].x-dLJointDanGen/2;
	xyArrRebar.Add(xyPoint);

	sRebar.Format("종점부상부");
	stRebarShape.m_sName     = sRebar;
	stRebarShape.m_dArrShape = xyArrRebar;
	stRebarShape.m_bValid    = TRUE;
	vRebarShape.push_back(stRebarShape);
	xyArrRebar.RemoveAll();

	// 일반부 상부 배력철근
	long nSize = vRebarShape.size();
	for(long n=0; n<nSize-1; n++)
	{
		CDPointArray arrStt = vRebarShape[n].m_dArrShape;
		CDPointArray arrEnd = vRebarShape[n+1].m_dArrShape;
		CDPoint xyStt = arrStt.GetAt(arrStt.GetUpperBound());
		CDPoint xyEnd = arrEnd.GetAt(0);
		if(n==nSize-2)	// arrEnd->종점부상부철근(시점부상부와 Mirror형태임)
			xyEnd = arrEnd.GetAt(arrEnd.GetUpperBound());
		xyArrRebar.Add(xyStt - vX*(n==0		 ? dLJointDanGen : dLJointGenJijum));		
		xyArrRebar.Add(xyEnd + vX*(n==nSize-2 ? dLJointDanGen : dLJointGenJijum));

		sRebar.Format("일반부상부(%d)", n+1);
		stRebarShape.m_sName     = sRebar;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bValid    = bInstallRebarCen;
		vRebarShape.push_back(stRebarShape);
		xyArrRebar.RemoveAll();
	}
	// SHAPE저장 
	sort(vRebarShape.begin(), vRebarShape.end(), LessThanMidPoint);	
}


void CADeckData::MakeShapeBeryukRebarDn(long nJongBasePos, long nCycle, vector <REBAR_SHAPE>& vRebarShape)
{
	CGlobarOption *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	
	//////////////////////////////////////////////////////////////////////////
	// 종단이음위치 계산
	//////////////////////////////////////////////////////////////////////////
	REBAR_SHAPE stRebarShape;

	CDPointArray xyArrGuide = GetXyArrGuideLineForBeryukRebar(nJongBasePos, FALSE);
	CDPointArray xyArrJoint = GetXyArrJointPosAtJongBasePos(nJongBasePos, nCycle, FALSE);	

	//////////////////////////////////////////////////////////////////////////
	// SHAPE 계산
	//////////////////////////////////////////////////////////////////////////
	CDPoint vX(1,0), vY(0,1);
	CString sRebar;

	double dSlabT		    = m_pDB->m_dThickSlabBase;
	double dThickPave		= m_pDB->GetThickPave();
	double dSttExpW			= GetWidthBlockOut(TRUE, nJongBasePos);
	double dSttExpH			= pGlopt->GetSttExpansionJointHeight()-dThickPave;
	double dEndExpW			= GetWidthBlockOut(FALSE, nJongBasePos);
	double dEndExpH			= pGlopt->GetEndExpansionJointHeight()-dThickPave;
	if(m_bInstallMainRebarAtBlockOut)
	{	dSttExpW = 0, dSttExpH = 0, dEndExpW = 0, dEndExpH = 0;	}
	long nPos = 0;	// 0=좌측, 1=우측, 2=중앙부
	if     (nJongBasePos==JONGBASEPOS_SLABCENTER || nJongBasePos==JONGBASEPOS_SLABLC || nJongBasePos==JONGBASEPOS_SLABRC)	nPos = POS_SLAB_CENTER;
	else if(nJongBasePos == JONGBASEPOS_SLABLEFT)		nPos = POS_SLAB_LEFT;
	else if(nJongBasePos == JONGBASEPOS_SLABRIGHT)		nPos = POS_SLAB_RIGH;
	else if(nJongBasePos == JONGBASEPOS_ONCB)			nPos = POS_SLAB_CENTER;

	// 직경
	double dDiaDanbuBeryukLo= m_dDiaBeryukRebarLower_Dan[nPos];	// 단부 하면 배력철근 직경	
	double dDiaGenBeryukLo	= m_dDiaBeryukRebarLower_Cent[nPos];	// 일반부 하면 배력철근 직경
	double dDiaJijumBeryukLo= m_dDiaJijumRebarDn[nPos];			// 지점부 하면 배력철근 직경

	// 이음장길이
	double dLJointDanLo		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaDanbuBeryukLo,	JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dEndSupportRebarCTCLower[nPos]));
	double dLJointGenLo		= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaGenBeryukLo,	JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dGenSupportRebarCTCLower[nPos]));
	double dLJointJijumLo	= GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDiaJijumBeryukLo,	JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dJijumRebarCTCDn[nPos]));
	double dLJointDanGenLo	= max(dLJointDanLo, dLJointGenLo);
	double dLJointGenJijumLo= max(dLJointJijumLo, dLJointGenLo);

	// 중앙부 철근 설치 유무
	BOOL bInstallRebarCen = IsInstallBeryukRebarAtCenter(nCycle);
	CDPointArray xyArrRebar;
	CDPoint xyPoint;
	CString	szText = _T("");

	//////////////////////////////////////////////////////////////////////////
	// 하부철근
	//////////////////////////////////////////////////////////////////////////
	// 시점부 하부 배력철근 (1)
	BOOL bLower2Stt = IsInstallBeryukRebarAtDanHunch(nJongBasePos, TRUE, nCycle);
	BOOL bLower2End = IsInstallBeryukRebarAtDanHunch(nJongBasePos, FALSE, nCycle);
	BOOL bFillStt = m_pDB->m_cSlabThickAddTypeStt==1 && nJongBasePos==JONGBASEPOS_ONCB ? TRUE : FALSE;
	BOOL bFillEnd = m_pDB->m_cSlabThickAddTypeEnd==1 && nJongBasePos==JONGBASEPOS_ONCB ? TRUE : FALSE;
	BOOL bExtToSlabEnd = m_nTypeDanbuTaperBeryukRebar==1 ? TRUE : FALSE;	// 슬래브 끝단까지 배력철근연장
	
	if(m_pDB->m_dHeightTaperStt > 0)
	{
		if(m_bInstallBeryukRebarAtBlockOut)
		{
			double dExt = 100;
			xyPoint = CDPoint(xyArrGuide[0].x, -dSlabT-m_pDB->m_dHeightTaperStt+dExt);
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[0];
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[1];
			xyArrRebar.Add(xyPoint);

			CDPoint xyMatch;
			CDPoint vAngTaper = (xyArrGuide[2]-xyArrGuide[1]).Unit();
			GetXyMatchLineAndLine(xyArrGuide[1], vAngTaper, CDPoint(0, -m_dCoverUp), vX, xyMatch);
			xyArrRebar.Add(xyMatch);
		}
		else
		{
			xyPoint = CDPoint(xyArrGuide[0].x+dSttExpW+m_dLengthBeryukSttA, -dSttExpH-m_dCoverUp);
			xyArrRebar.Add(xyPoint);

			xyPoint += -vX*(m_dLengthBeryukSttA+dSttExpW);
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[0];
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[1];
			xyArrRebar.Add(xyPoint);

			CDPoint xyMatch;
			CDPoint vAngTaper = (xyArrGuide[2]-xyArrGuide[1]).Unit();
			GetXyMatchLineAndLine(xyArrGuide[1], vAngTaper, CDPoint(0, -m_dCoverUp), vX, xyMatch);
			xyArrRebar.Add(xyMatch);
		}
	}
	sRebar.Format("시점부하부(1)");
	stRebarShape.m_sName     = sRebar;
	stRebarShape.m_dArrShape = xyArrRebar;
	stRebarShape.m_bValid    = TRUE;/*m_pDB->m_dHeightTaperStt>0 ? TRUE : FALSE*/;
	vRebarShape.push_back(stRebarShape);
	xyArrRebar.RemoveAll();

	double dHunchT = nJongBasePos==JONGBASEPOS_ONCB ? m_pDB->m_dThickSlabHunch : 0;
	// 시점부 하부 배력철근 (2)
	if(!bExtToSlabEnd && m_pDB->m_dHeightTaperStt>0)	// 연장 안된경우
		xyPoint = xyArrGuide[2] - vX*m_dLengthBeryukSttB;
	else
		xyPoint = CDPoint(xyArrGuide[0].x, xyArrGuide[2].y);
	xyArrRebar.Add(xyPoint);
	
	if (bFillStt)
	{
		xyPoint = xyArrGuide[3];
		xyArrRebar.Add(xyPoint);

		CDPoint xyMatch;
		CDPoint vAngTaper = (xyArrGuide[4]-xyArrGuide[3]).Unit();
		GetXyMatchLineAndLine(xyArrGuide[3], vAngTaper, CDPoint(0, -m_dCoverUp), vX, xyMatch);
		xyArrRebar.Add(xyMatch);		
	}
	else
	{
		xyPoint = xyArrJoint[1] + vX*dLJointDanGenLo/2;
		xyArrRebar.Add(xyPoint);
	}

	sRebar.Format("시점부하부(2)");
	stRebarShape.m_sName     = sRebar;
	stRebarShape.m_dArrShape = xyArrRebar;
	stRebarShape.m_bValid    = FALSE;
	if(bFillStt || bLower2Stt)
		stRebarShape.m_bValid = TRUE;

	vRebarShape.push_back(stRebarShape);
	xyArrRebar.RemoveAll();

	// 지점부 하부 배력철근 
	for(long nP=0; nP<m_pDB->m_nQtyJigan-1; nP++)
	{
		xyPoint = CDPoint(xyArrJoint[nP*3+2].x-dLJointGenJijumLo/2, -(dSlabT-dHunchT)+m_dCoverDn);
		xyArrRebar.Add(xyPoint);		

		xyPoint = CDPoint(xyArrJoint[nP*3+3].x, -(dSlabT-dHunchT)+m_dCoverDn);
		xyArrRebar.Add(xyPoint);

		xyPoint = CDPoint(xyArrJoint[nP*3+4].x+dLJointGenJijumLo/2, -(dSlabT-dHunchT)+m_dCoverDn);
		xyArrRebar.Add(xyPoint);
		
		sRebar.Format("지점부하부(%d)", nP+1);
		stRebarShape.m_sName     = sRebar;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bValid    = TRUE;
		vRebarShape.push_back(stRebarShape);
		xyArrRebar.RemoveAll();
	}

	// 종점부 하부 배력철근 (2)
	if (bFillEnd)
	{
		CDPoint xyMatch;
		CDPoint vAngTaper = (xyArrGuide[xyArrGuide.GetUpperBound()-4]-xyArrGuide[xyArrGuide.GetUpperBound()-3]).Unit();
		GetXyMatchLineAndLine(xyArrGuide[xyArrGuide.GetUpperBound()-3], vAngTaper, CDPoint(0, -m_dCoverUp), vX, xyMatch);
		xyArrRebar.Add(xyMatch);		

		xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()-3];
		xyArrRebar.Add(xyPoint);
	}
	else
	{
		xyPoint = xyArrJoint[xyArrJoint.GetUpperBound()-1] - vX*dLJointDanGenLo/2;
		xyArrRebar.Add(xyPoint);
	}

	if(!bExtToSlabEnd  && m_pDB->m_dHeightTaperEnd > 0)
		xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()-2] + vX*m_dLengthBeryukEndB;
	else
		xyPoint = CDPoint(xyArrGuide[xyArrGuide.GetUpperBound()].x, xyArrGuide[xyArrGuide.GetUpperBound()-2].y);
	xyArrRebar.Add(xyPoint);

	sRebar.Format("종점부하부(2)");
	stRebarShape.m_sName     = sRebar;
	stRebarShape.m_dArrShape = xyArrRebar;
	stRebarShape.m_bValid    = FALSE;
	if((bFillEnd) || bLower2End)
		stRebarShape.m_bValid = TRUE;
	vRebarShape.push_back(stRebarShape);
	xyArrRebar.RemoveAll();

	// 종점부 하부 배력철근 (1)
	if(m_pDB->m_dHeightTaperEnd > 0)
	{
		if(m_bInstallBeryukRebarAtBlockOut)
		{
			double dExt = 100;

			xyPoint = CDPoint(xyArrGuide[xyArrGuide.GetUpperBound()].x,  -dSlabT-m_pDB->m_dHeightTaperEnd+dExt);
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()];
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()-1];
			xyArrRebar.Add(xyPoint);

			CDPoint xyMatch;
			CDPoint vAngTaper = (xyArrGuide[xyArrGuide.GetUpperBound()-2]-xyArrGuide[xyArrGuide.GetUpperBound()-1]).Unit();
			GetXyMatchLineAndLine(xyArrGuide[xyArrGuide.GetUpperBound()-1], vAngTaper, CDPoint(0, -m_dCoverUp), vX, xyMatch);
			xyArrRebar.Add(xyMatch);
		}
		else
		{
			xyPoint = CDPoint(xyArrGuide[xyArrGuide.GetUpperBound()].x-dEndExpW-m_dLengthBeryukEndA, -dEndExpH-m_dCoverUp);
			xyArrRebar.Add(xyPoint);

			xyPoint = CDPoint(xyArrGuide[xyArrGuide.GetUpperBound()].x, -dEndExpH-m_dCoverUp);
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()];
			xyArrRebar.Add(xyPoint);

			xyPoint = xyArrGuide[xyArrGuide.GetUpperBound()-1];
			xyArrRebar.Add(xyPoint);

			CDPoint xyMatch;
			CDPoint vAngTaper = (xyArrGuide[xyArrGuide.GetUpperBound()-2]-xyArrGuide[xyArrGuide.GetUpperBound()-1]).Unit();
			GetXyMatchLineAndLine(xyArrGuide[xyArrGuide.GetUpperBound()-1], vAngTaper, CDPoint(0, -m_dCoverUp), vX, xyMatch);
			xyArrRebar.Add(xyMatch);
		}
	}
	sRebar.Format("종점부하부(1)");
	stRebarShape.m_sName     = sRebar;
	stRebarShape.m_dArrShape = xyArrRebar;
	stRebarShape.m_bValid    = TRUE;/*m_pDB->m_dHeightTaperEnd>0 ? TRUE : FALSE;*/
	vRebarShape.push_back(stRebarShape);
	xyArrRebar.RemoveAll();

	// 일반부 하부 배력철근
	long nStt = 1;
	long nEnd = vRebarShape.size() - 2;
	CDPoint xyStt, xyEnd;

	for(long n=nStt; n<nEnd; n++)
	{
		CDPointArray arrStt = vRebarShape[n].m_dArrShape;
		CDPointArray arrEnd = vRebarShape[n+1].m_dArrShape;

		if (n==nStt)
		{
			if(m_pDB->m_dHeightTaperStt>0)
			{
				if (bLower2Stt)	// 시점부하부(2)가 있을 경우
				{
					if(bFillStt)
						xyStt = xyArrGuide[4] - vX*m_dLengthBeryukSttC;
					else
						xyStt = arrStt.GetAt(arrStt.GetUpperBound()) - vX* dLJointDanGenLo;
				}
				else			// 시점부하부(2)가 없고 일반부 철근이 연장됨
				{
					if(!bExtToSlabEnd)
						xyStt = xyArrGuide[bFillStt?4:2] - vX*m_dLengthBeryukSttC;
					else
					{
						if(bFillStt)
							xyStt = xyArrGuide[4] - vX*m_dLengthBeryukSttC;
						else
						{
							xyStt.x = xyArrGuide[0].x;
							xyStt.y = xyArrGuide[2].y;
						}
					}
				}

				if (m_pDB->m_nQtyJigan==1 && nJongBasePos==JONGBASEPOS_ONCB)
				{
					if(!bExtToSlabEnd)
					{
						if(bLower2End)
							xyEnd = arrEnd.GetAt(0) + vX*dLJointGenJijumLo;
						else
							xyEnd = xyArrGuide[xyArrGuide.GetUpperBound()-(bFillEnd?4:2)] + vX*m_dLengthBeryukEndB;
					}
					else
					{
						if(bFillEnd)
							xyEnd = xyArrGuide[xyArrGuide.GetUpperBound()-4] + vX*m_dLengthBeryukEndC;
						else
						{
							if(bLower2End)
								xyEnd = arrEnd.GetAt(0) + vX*dLJointGenJijumLo;
							else
							{
								xyEnd.x = xyArrGuide[xyArrGuide.GetUpperBound()].x;
								xyEnd.y = xyArrGuide[xyArrGuide.GetUpperBound()-2].y;
							}
						}
					}
				}
				else if (m_pDB->m_nQtyJigan==1)
				{
					if(bLower2End)
						xyEnd = arrEnd.GetAt(0) + vX*dLJointGenJijumLo;
					else
					{
						if(bExtToSlabEnd)
						{
							xyEnd.x = xyArrGuide[xyArrGuide.GetUpperBound()].x;
							xyEnd.y = xyArrGuide[xyArrGuide.GetUpperBound()-2].y;
						}
						else
							xyEnd = xyArrGuide[xyArrGuide.GetUpperBound()-2] + vX*m_dLengthBeryukEndC;
					}
				}
				else 
				{
					xyEnd = arrEnd.GetAt(0) + vX*dLJointGenJijumLo;
				}
			}
			else
			{
				if(bLower2Stt)	
					xyStt = arrStt.GetAt(arrStt.GetUpperBound()) - vX*(m_pDB->m_nQtyJigan==1 ? dLJointDanGenLo : dLJointGenJijumLo);
				else
					xyStt = xyArrGuide[0];
				xyEnd = arrEnd.GetAt(0) + vX*(m_pDB->m_nQtyJigan==1 ? dLJointDanGenLo : dLJointGenJijumLo);
			}
		}
		else if (n==nEnd-1)
		{
			if(m_pDB->m_dHeightTaperEnd>0)
			{
				xyStt = arrStt.GetAt(arrStt.GetUpperBound()) - vX*dLJointGenJijumLo;
				if (bLower2End)
				{
					if(bFillEnd)
						xyEnd  = xyArrGuide[xyArrGuide.GetUpperBound()-4] + vX*m_dLengthBeryukEndC;
					else
						xyEnd  = arrEnd.GetAt(0) + vX*dLJointDanGenLo;
				}
				else	
				{
					if(!bExtToSlabEnd)
					{
						if(bFillStt)
							xyEnd = xyArrGuide[xyArrGuide.GetUpperBound()-4] + vX*m_dLengthBeryukEndB;
						else
							xyEnd = xyArrGuide[xyArrGuide.GetUpperBound()-2] + vX*m_dLengthBeryukEndB;
					}
					else
					{
						if(bFillEnd)
						{
							xyEnd = xyArrGuide[xyArrGuide.GetUpperBound()-4] + vX*m_dLengthBeryukEndC;
						}
						else
						{
							xyEnd.x = xyArrGuide[xyArrGuide.GetUpperBound()].x;
							xyEnd.y = xyArrGuide[xyArrGuide.GetUpperBound()-2].y;
						}

					}
				}
			}
			else
			{
				xyStt = arrStt.GetAt(arrStt.GetUpperBound()) - vX*dLJointGenJijumLo;
				if(bLower2End)	
					xyEnd = arrEnd.GetAt(0) + vX*(m_pDB->m_nQtyJigan==1 ? dLJointDanGenLo : dLJointGenJijumLo);
				else
					xyEnd = xyArrGuide[xyArrGuide.GetUpperBound()];
			}
		}
		else
		{
			xyStt = arrStt.GetAt(arrStt.GetUpperBound()) - vX*dLJointGenJijumLo;
			xyEnd = arrEnd.GetAt(0) + vX*dLJointGenJijumLo;
		}

		xyArrRebar.Add(xyStt);		
		xyArrRebar.Add(xyEnd);
		
		sRebar.Format("일반부하부(%d)", n-nStt+1);
		stRebarShape.m_sName     = sRebar;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bValid	 = bInstallRebarCen;
		vRebarShape.push_back(stRebarShape);
		xyArrRebar.RemoveAll();
	}

	// SHAPE저장 
	sort(vRebarShape.begin(), vRebarShape.end(), LessThanMidPoint);	
}



// nSection위치에서 주철근을 생성함.
void CADeckData::MakeShapeMainRebar(long nSection)
{
	if(IsCycleValid(nSection)==FALSE)	return;

	RemoveRebarShapeBySection(nSection);	// 현재 단면의 주철근형태를 삭제함.
	long nCyclesu = GetRebarCycleSuBySection(nSection);

	if (nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END)		// 단부
	{
		for (long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			vector <REBAR_SHAPE> vRebarShape;
			MakeShapeMainRebarBySlabOutLine(nSection, nCycle, TRUE, vRebarShape);	// 슬래브 상면 주철근형태를 생성
			MakeShapeMainRebarBySlabOutLine(nSection, nCycle, FALSE, vRebarShape);	// 슬래브 하면 주철근형태를 생성

			AddRebarShapeArrAtSection(nSection, vRebarShape);
		}
	}
	else	// 일반부
	{
		for (long nCycle=0; nCycle<nCyclesu; nCycle++)
		{
			vector <REBAR_SHAPE> vRebarShape, vRebarShapeDn;

			BOOL bNoBrk   = !IsBridgeHasOutBracket();
			BOOL bNoHunch = IsBridgeHasSlabHunch()==FALSE || (bNoBrk&&m_pDB->GetGirdersu()==1) ? TRUE : FALSE;

			if(bNoHunch)
			{
				MakeShapeMainRebarBySlabOutLine(nSection, nCycle, TRUE, vRebarShape);	// 슬래브 상면 주철근형태를 생성
				MakeShapeMainRebarBySlabOutLine(nSection, nCycle, FALSE, vRebarShape);	// 슬래브 하면 주철근형태를 생성

				AddRebarShapeArrAtSection(nSection, vRebarShape);
			}
			else
			{
				MakeShapeMainRebarBySlabOutLine(nSection, nCycle, TRUE, vRebarShape);	// 슬래브 상면 주철근형태를 생성
				MakeShapeMainRebarLowerHasSlabHunch(nSection, nCycle, vRebarShapeDn);	// 슬래브 하면 주철근형태를 생성(헌치가 있는 경우)
				for (long n=0; n<(long)vRebarShapeDn.size(); n++)
					vRebarShape.push_back(vRebarShapeDn[n]);

				AddRebarShapeArrAtSection(nSection, vRebarShape);
			}
		}
	}
}



// nSection위치에서 슬래브의 외곽선을 따라서 상/하부의 주철근을 생성함.
void CADeckData::MakeShapeMainRebarBySlabOutLine(long nSection, long nCycle, BOOL bUpper, vector<REBAR_SHAPE>&  vRebarShape)
{
	CDPoint vAngSec = CDPoint(0,1);	// 횡단면에서 각
	if(nSection==SECTION_SKEW_STT)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	else if(nSection==SECTION_SKEW_END)
		vAngSec = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);

	double dRatio = fabs(vAngSec.y);

	CDPoint vX(1,0), vY(0,1);
	CString sRebar;
	double dDisDolchulL	= GetLengthMainDolchul(nSection, bUpper, TRUE);
	double dDisDolchulR	= GetLengthMainDolchul(nSection, bUpper, FALSE);
	CDPointArray xyArrGuide = GetXyArrGuideLineForMainRebar(nSection, bUpper);
	CDPointArray xyArrRebar, xyArrInnerJoint;
	double dCTCForJoint = 0;
	double dJointPre	= 0;	double dJointCur	= 0;	double dJointNex = 0;
	double dJointRebarStt = 0;	double dJointRebarEnd = 0;
	long nJointStyle = JOINT_TENS_NORMAL_B;
	if(bUpper && (nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END))
		nJointStyle = JOINT_TENS_UPPER_B;

	REBAR_CYCLE* pRebarCycle = GetRebarCycleBySection(nSection, nCycle);

	CDoubleArray arrJoint  = bUpper ? pRebarCycle->m_dArrJointUp : pRebarCycle->m_dArrJointDn;
	CDoubleArray arrDia    = bUpper ? pRebarCycle->m_dArrDiaUp   : pRebarCycle->m_dArrDiaDn;
	CDoubleArray arrValid  = bUpper ? pRebarCycle->m_dArrValidUp : pRebarCycle->m_dArrValidDn;
	CDoubleArray dArrPos;	// 원점(슬래브상/하면-주철근 시작위치)을 기준으로한 절대위치
	double	dSum = 0;
	long	n	= 0;
	for(n=0; n<arrJoint.GetSize(); n++)
	{
		dSum += arrJoint[n];
		dArrPos.Add(dSum);
	}
	dArrPos.Sort();
	CDPointArray xyArrOrigin;
	for( n=0; n<dArrPos.GetSize(); n++)
		xyArrOrigin.Add(CDPoint(xyArrGuide[0].x + dArrPos[n], 0));

	CDPointArray xyArrJoint = GetCDPointArrByPolyLine(xyArrOrigin, xyArrGuide, TRUE);	// 이음중심 위치

	long nShapeSu = arrJoint.GetSize()+1;

	// TU거더 단부 하면은 별도로 배근한다 ////////////////////////////////////
	BOOL bDanbu = nSection==SECTION_DAN || nSection==SECTION_SKEW_STT || nSection==SECTION_SKEW_END ? TRUE : FALSE;
	if(m_pDB->IsTUGir() && bDanbu && !bUpper)
	{
		CDPoint vAngRebar(0,0), xyPrePoint(0,0);
		for (long nShape=0; nShape<nShapeSu; nShape++)
		{
			REBAR_SHAPE stRebarShape;
			BOOL bValid = (BOOL)arrValid[nShape];
			xyArrRebar.RemoveAll();

			if(nShape==0)
			{
				vAngRebar = (xyArrGuide[2]-xyArrGuide[1]).Unit();
				xyArrRebar.Add(xyArrGuide[0]+vY*dDisDolchulL);
				xyArrRebar.Add(xyArrGuide[1]+vX*m_dCoverSd);
				xyArrRebar.Add(xyArrRebar.GetAt(xyArrRebar.GetUpperBound()) + vAngRebar*(arrJoint.GetAt(0)/vAngRebar.x));
			}
			else if(nShape==nShapeSu-1)
			{
				long nGuide = xyArrGuide.GetUpperBound();
				vAngRebar = (xyArrGuide[nGuide-1]-xyArrGuide[nGuide-2]).Unit();
				xyArrRebar.Add(xyArrGuide[nGuide]+vY*dDisDolchulR);
				xyArrRebar.Add(xyArrGuide[nGuide-1]-vX*m_dCoverSd);
				xyArrRebar.Add(xyPrePoint);
			}
			else
			{
				long nDir = nShape%2==0 ? 1 : -1;
//				..기존에는 이걸로 왜 문제를 발견하지 못했을까? 090522 APMS ATuGirder2008_삼표_유지 4119 에서 수정함
//				vAngRebar = ((xyArrGuide[nShape+2]-vX*m_dCoverSd*nDir)-(xyArrGuide[nShape+1]+vX*m_dCoverSd*nDir)).Unit();
				vAngRebar = ((xyArrGuide[nShape+3]-vX*m_dCoverSd*nDir)-(xyArrGuide[nShape+2]+vX*m_dCoverSd*nDir)).Unit();
				xyArrRebar.Add(xyPrePoint);
				xyArrRebar.Add(xyArrRebar.GetAt(xyArrRebar.GetUpperBound()) + vAngRebar*(arrJoint.GetAt(nShape)/vAngRebar.x));
			}			

			if(nShape%2==0)	sRebar.Format("하부%d", nShape/2+1);
			else			sRebar.Format("주철근구속콘크리트부%d", (nShape+1)/2);
			stRebarShape.m_sName     = sRebar;
			stRebarShape.m_dArrShape = xyArrRebar;
			stRebarShape.m_dDia      = arrDia[nShape];
			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
			stRebarShape.m_bValid	 = bValid;	
			stRebarShape.m_nCycle	 = nCycle;	
			stRebarShape.m_bUpper	 = FALSE;

			xyPrePoint = xyArrRebar.GetAt(xyArrRebar.GetUpperBound());	
			vRebarShape.push_back(stRebarShape);
		}
		return;
	}
	//////////////////////////////////////////////////////////////////////////	

	for (long nShape=0; nShape<nShapeSu; nShape++)
	{
		REBAR_SHAPE stRebarShape;
		BOOL bValid = (BOOL)arrValid[nShape];

		// 이음길이 결정
		if(nShape==0)				dCTCForJoint = GetCTCMainForJoint(bUpper, POS_SLAB_LEFT, nSection);
		else if(nShape==nShapeSu-1)	dCTCForJoint = GetCTCMainForJoint(bUpper, POS_SLAB_RIGH, nSection);
		else						dCTCForJoint = GetCTCMainForJoint(bUpper, POS_SLAB_CENTER, nSection);
		
		if(nShape==0)
		{
			dJointCur = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), arrDia[nShape], nJointStyle, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
			dJointNex = dJointCur;
			if(nShapeSu>1)
				dJointNex = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), arrDia[nShape+1], nJointStyle, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
			dJointRebarStt = dJointCur*dRatio;
			dJointRebarEnd = max(dJointCur, dJointNex)*dRatio;
		}
		else if(nShape==nShapeSu-1)
		{
			dJointCur = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), arrDia[nShape], nJointStyle, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
			dJointPre = dJointCur;
			if(nShapeSu>1)
				dJointPre = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), arrDia[nShape-1], nJointStyle, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
			dJointRebarStt = max(dJointPre, dJointCur)*dRatio;
			dJointRebarEnd = dJointCur*dRatio;
		}
		else
		{
			dJointPre = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), arrDia[nShape-1], nJointStyle, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
			dJointCur = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), arrDia[nShape], nJointStyle, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
			dJointRebarStt = max(dJointPre, dJointCur)*dRatio;
			
			dJointNex = GetValueJoint(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), arrDia[nShape+1], nJointStyle, toCm(bUpper?m_dCoverUp:m_dCoverDn), toCm(dCTCForJoint));
			dJointRebarEnd = max(dJointCur, dJointNex)*dRatio;
		}
					
		xyArrRebar.RemoveAll();
		
		if(nShapeSu==1)		// 이음위치가 없는 경우
		{
			xyArrRebar.Add(xyArrGuide[0]-(bUpper?1:-1)*vY*dDisDolchulL);	// 시점
			for( n=0; n<xyArrGuide.GetSize(); n++)
				xyArrRebar.Add(xyArrGuide[n]);

			xyArrRebar.Add(xyArrGuide[xyArrGuide.GetUpperBound()]-(bUpper?1:-1)*vY*dDisDolchulR);		// 종점

			stRebarShape.m_sName     = bUpper?"상부1":"하부1";
			stRebarShape.m_dArrShape = xyArrRebar;
			stRebarShape.m_dDia      = arrDia[nShape];
			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
			stRebarShape.m_bValid	 = bValid;	
			stRebarShape.m_nCycle	 = nCycle;	
			stRebarShape.m_bUpper	 = bUpper;	
		}
		else
		{
			if(nShape==0)
			{
				CDPoint vRebar = GetSlopeAtHDanByPos(xyArrGuide, xyArrJoint[nShape]);
				xyArrRebar.Add(xyArrGuide[0]-(bUpper?1:-1)*vY*dDisDolchulL);	
				double dRangeStt = xyArrGuide[0].x; 
				double dRangeEnd = xyArrJoint[nShape].x;
				for(long a=0; a<xyArrGuide.GetSize(); a++)
				{
					if(dRangeStt <= xyArrGuide[a].x && xyArrGuide[a].x <=dRangeEnd)
						xyArrRebar.Add(xyArrGuide[a]);
				}

				xyArrRebar.Add(xyArrJoint[nShape] + vRebar*dJointRebarEnd/2/fabs(vRebar.x));
			}
			else if(nShape==nShapeSu-1)
			{
				xyArrRebar.Add(xyArrGuide[xyArrGuide.GetUpperBound()] - (bUpper?1:-1)*vY*dDisDolchulR);
				double dRangeStt = xyArrJoint[nShape-1].x; 
				double dRangeEnd = xyArrGuide[xyArrGuide.GetUpperBound()].x;
				for(long a=xyArrGuide.GetUpperBound(); a>=0; a--)
				{
					if(dRangeStt <= xyArrGuide[a].x && xyArrGuide[a].x <=dRangeEnd)
						xyArrRebar.Add(xyArrGuide[a]);
				}
				CDPoint vRebar = GetSlopeAtHDanByPos(xyArrGuide, xyArrJoint[nShape-1]);
				xyArrRebar.Add(xyArrJoint[nShape-1] - vRebar*dJointRebarStt/2/fabs(vRebar.x));
			}
			else
			{
				CDPoint vRebar = GetSlopeAtHDanByPos(xyArrGuide, xyArrJoint[nShape-1]);
				xyArrRebar.Add(xyArrJoint[nShape-1] - vRebar*dJointRebarStt/2/fabs(vRebar.x));

				double dRangeStt = xyArrJoint[nShape-1].x; 
				double dRangeEnd = xyArrJoint[nShape].x;
				for(long a=0; a<xyArrGuide.GetSize(); a++)
				{
					if(dRangeStt <= xyArrGuide[a].x && xyArrGuide[a].x <=dRangeEnd)
						xyArrRebar.Add(xyArrGuide[a]);
				}
				
				vRebar = GetSlopeAtHDanByPos(xyArrGuide, xyArrJoint[nShape]);
				xyArrRebar.Add(xyArrJoint[nShape] + vRebar*dJointRebarEnd/2/fabs(vRebar.x));
			}

			sRebar.Format(bUpper?"상부%d":"하부%d", nShape+1);
			stRebarShape.m_sName     = sRebar;
			stRebarShape.m_dArrShape = xyArrRebar;
			stRebarShape.m_dDia      = arrDia[nShape];
			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
			stRebarShape.m_bValid	 = bValid;
			stRebarShape.m_nCycle	 = nCycle;	
			stRebarShape.m_bUpper	 = bUpper;	
		}
		vRebarShape.push_back(stRebarShape);
	}
}


void CADeckData::MakeShapeMainRebarLowerHasSlabHunch(long nSection, long nCycle, vector<REBAR_SHAPE>& vRebarShape)
{
	
	MakeShapeMainRebarAtUpperGirderWithoutCycle(nSection, nCycle, vRebarShape);
	if(IsBridgeHasCrossBeam())
		MakeShapeMainRebarAtUpperCbStgWithoutCycle(nSection, nCycle, vRebarShape);	// 가로보 및 세로보 상부의 슬래브 하면 철근을 생성
	
//	if(IsBridgeHasOutBracket())
//		MakeShapeMainRebarAtUpperOutBkWithoutCycle(nSection, nCycle, vRebarShape);	// 외측가로보 상부의 슬래브 하면 철근을 생성	

	sort(vRebarShape.begin(), vRebarShape.end(), LessThanMidPoint);	

	REBAR_CYCLE* pRebarCycle = GetRebarCycleBySection(nSection, nCycle);

	for (long nShape=0; nShape<(long)vRebarShape.size(); nShape++)
	{
		vRebarShape[nShape].m_bValid = (long)pRebarCycle->m_dArrValidDn[nShape];
		vRebarShape[nShape].m_dDia   = pRebarCycle->m_dArrDiaDn[nShape];
	}
}



// nSection위치에서 거더상부의 슬래브 하면철근을 생성
// Type1,2,3  외측거더상면 주철근
void CADeckData::MakeShapeMainRebarAtUpperGirderWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape)
{
	CDPoint vX(1,0), vY(0,1);
	CPlateBridgeApp      *pDB	 = m_pDB;

	long nGsu		 = m_pDB->GetGirdersu();
	BOOL bOneBox	 = nGsu==1 ? TRUE : FALSE;								// 단박스?
	BOOL bHasHunch   = IsBridgeHasSlabHunch();								// 슬래브헌치가 있나?
	BOOL bLeftBrk    = IsBridgeHasOutBracket(nSection, TRUE)&&bHasHunch ? TRUE : FALSE;					// 좌측브라켓만?
	BOOL bRighBrk    = IsBridgeHasOutBracket(nSection, FALSE)&&bHasHunch ? TRUE : FALSE;				// 우측브라켓만?	
	BOOL bLeftOnlyBrk=  bLeftBrk&&!bRighBrk ? TRUE : FALSE;					// 좌측브라켓만?
	BOOL bRighOnlyBrk= !bLeftBrk&& bRighBrk ? TRUE : FALSE;					// 우측브라켓만?	
	BOOL bAllBrk	 =  bLeftBrk&& bRighBrk ? TRUE : FALSE;					// 양측브라켓?
	BOOL bNoBrk		 = !bLeftBrk&&!bRighBrk? TRUE : FALSE;					// 브라켓이 없나?
	BOOL bMultiBoxHasHunch   = !bOneBox&& bHasHunch ? TRUE : FALSE;			// 다박스고 슬래브 헌치가 있나?
	double dDisDolchulLeft   = GetLengthMainDolchul(nSection, FALSE, TRUE);		// ㄱ형태 일 경우 돌출길이
	double dDisDolchulRigh   = GetLengthMainDolchul(nSection, FALSE, FALSE);
	if(m_nDetailMainUpper == 1)		dDisDolchulLeft = dDisDolchulRigh = 0.1;	// 약간 돌출되게 함(동일점 제거시 예외를 두기 위해서)
	
	double mL = 1/pDB->m_dSlopHunch;	//거더 좌측 헌치 기울기
	CPlateGirderApp *pGirL  = m_pDB->GetGirder(0);
	CPlateGirderApp *pGirR  = m_pDB->GetGirder(m_pDB->GetGirdersu()-1);
	CPlateGirderApp *pGirRR = m_pDB->GetGirder(m_pDB->GetGirdersu()-2);

	CDPoint A[10], C[4], xyMatch;
	CDPointArray xyArrRebar, xyArrInnerJoint;
	CPlateBasicIndex *pBx      = GetBxBySection(nSection);	
	CPlateBasicIndex *pBxMatch = NULL;
	
	xyArrRebar.RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	// stRebarShape 설정시 dDia, dValid는 shape생성후 shape을 정렬하여 설정함.
	//////////////////////////////////////////////////////////////////////////
	
	// TYPE 1
	if( (bOneBox&&bAllBrk) || (bMultiBoxHasHunch&&bLeftOnlyBrk) || (bMultiBoxHasHunch&&bRighOnlyBrk) || (bMultiBoxHasHunch&&bAllBrk))	
	{
		// 좌측
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirL);
		if(bLeftBrk && pBxMatch)
		{
			REBAR_SHAPE stRebarShape;

			pGirL->GetHunchXyDom(pBxMatch, A, FALSE);

			xyMatch = m_pDB->GetMatchRebarPointDom(pBx, A[0]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
			xyArrRebar.Add(A[0]+vY*m_dCoverDn);
			xyArrRebar.Add(A[3]+vY*m_dCoverDn);
			xyMatch = m_pDB->GetMatchRebarPointDom(pBx, A[3]+vY*m_dCoverDn, mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
			
			stRebarShape.m_sName     = "좌측거더상면";
			stRebarShape.m_nCycle    = nCycle;
			stRebarShape.m_dArrShape = xyArrRebar;
			stRebarShape.m_bUpper	 = FALSE;	
			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
			vRebarShape.push_back(stRebarShape);
			xyArrRebar.RemoveAll();
		}
		// 우측
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirRR);
		if((bRighBrk && !bOneBox) && pBxMatch)
		{
			REBAR_SHAPE stRebarShape;
			
			pGirRR->GetHunchXyDom(pBxMatch, A, FALSE);
			pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR);
			
			xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, A[6]+vY*m_dCoverDn, mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
			xyArrRebar.Add(A[6]+vY*m_dCoverDn);
			xyArrRebar.Add(A[9]+vY*m_dCoverDn);
			xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, A[9]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);

			stRebarShape.m_sName     = "우측거더상면";
			stRebarShape.m_nCycle    = nCycle;
			stRebarShape.m_dArrShape = xyArrRebar;
			stRebarShape.m_bUpper	 = FALSE;	
			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
			vRebarShape.push_back(stRebarShape);
			xyArrRebar.RemoveAll();
		}
	}
	// TYPE 2 ( 좌측거더상면 철근)
	pBxMatch = pBx->GetBxMatchByCrossBeam(pGirL);
	if(pBxMatch && ((bOneBox&&bRighOnlyBrk) || (bMultiBoxHasHunch&&bRighOnlyBrk) || (bMultiBoxHasHunch&&bNoBrk)))	
	{
		REBAR_SHAPE stRebarShape;

		CDPoint LeftH[3];

		m_pDB->GetXyFrontCantilever(pBxMatch,C, TRUE);
		pGirL->GetHunchXyDom(pBxMatch, A, FALSE);
		LeftH[1] = CDPoint(C[0].x+m_dCoverSd, C[0].y+m_dCoverDn);	
		LeftH[0] = CDPoint(C[0].x+m_dCoverSd, C[0].y+dDisDolchulLeft+m_dCoverDn);
		if(m_nDetailMainUpper==1)	LeftH[0] = LeftH[1];

		if(m_pDB->m_bUserSlabHunch)
		{
			if(m_bDivideRebarAtCant)
			{
				CDPoint xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, A[0], (C[1].y-C[2].y)/(C[1].x-C[2].x), m_dCoverUp, FALSE);
				xyArrRebar.Add(xyMatch);
				xyArrRebar.Add(CDPoint(A[0].x, A[0].y+m_dCoverDn));
				xyArrRebar.Add(CDPoint(A[3].x, A[3].y+m_dCoverDn));
				xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, A[3], (A[4].y-A[3].y)/(A[4].x-A[3].x), m_dCoverUp, FALSE);
				xyArrRebar.Add(xyMatch);
			}
			else
			{
				xyArrRebar.Add(LeftH[0]);
				xyArrRebar.Add(LeftH[1]);
				xyArrRebar.Add(C[1]+vY*m_dCoverDn);
				xyArrRebar.Add(C[2]+vY*m_dCoverDn);
				xyArrRebar.Add(CDPoint(A[0].x, A[0].y+m_dCoverDn));
				xyArrRebar.Add(CDPoint(A[3].x, A[3].y+m_dCoverDn));
				xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, A[3], (A[4].y-A[3].y)/(A[4].x-A[3].x), m_dCoverUp, FALSE);
				xyArrRebar.Add(xyMatch);
			}
		}
		else
		{
			xyArrRebar.Add(LeftH[0]);
			xyArrRebar.Add(LeftH[1]);
			xyArrRebar.Add(CDPoint(A[0].x, A[0].y+m_dCoverDn));
			xyArrRebar.Add(CDPoint(A[3].x, A[3].y+m_dCoverDn));
			CDPoint xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, CDPoint(A[3].x, A[3].y+m_dCoverDn), mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
		}
		stRebarShape.m_sName     = "좌측거더상면";
		stRebarShape.m_nCycle    = nCycle;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bUpper	 = FALSE;	
		stRebarShape.m_dArrInJoint = xyArrInnerJoint;
		vRebarShape.push_back(stRebarShape);
		xyArrRebar.RemoveAll();

		// 좌측 캔틸레버 상면
		if(m_bDivideRebarAtCant && m_pDB->m_bUserSlabHunch)
		{
			double dSettleL = GetValueSettle(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), m_dDiaMainRebarLower_Cent[POS_SLAB_LEFT], JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dGenMainRebarCTCLower[POS_SLAB_LEFT]/2));
			CDPoint vVec = (C[1]-C[0]).Unit();
			CDPoint xyLeft = C[1] + vY*m_dCoverDn + vVec*dSettleL;

			REBAR_SHAPE stRebarShape2;
			xyArrRebar.Add(LeftH[0]);
			xyArrRebar.Add(LeftH[1]);
			xyArrRebar.Add(C[1]+vY*m_dCoverDn);
			xyArrRebar.Add(xyLeft);

			stRebarShape2.m_sName     = "좌측캔틸레버상면";
			stRebarShape2.m_nCycle    = nCycle;
			stRebarShape2.m_dArrShape = xyArrRebar;
			stRebarShape2.m_bUpper	 = FALSE;	
			stRebarShape2.m_dArrInJoint = xyArrInnerJoint;
			vRebarShape.push_back(stRebarShape2);
			xyArrRebar.RemoveAll();
		}
	}
	// TYPE 3 ( 우측캔틸레버부 철근)
	pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR);
	if(pBxMatch && ((bOneBox&&bLeftOnlyBrk) || (bMultiBoxHasHunch&&bLeftOnlyBrk) || (bMultiBoxHasHunch&&bNoBrk)))	
	{
		REBAR_SHAPE stRebarShape;

		CDPoint RightH[3];
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR);
		m_pDB->GetXyFrontCantilever(pBxMatch,C, FALSE);
		pGirRR->GetHunchXyDom(pBx->GetBxMatchByCrossBeam(pGirRR), A, FALSE);

		RightH[1] = CDPoint(C[0].x-m_dCoverSd, C[0].y+m_dCoverDn);
		RightH[0] = CDPoint(C[0].x-m_dCoverSd, C[0].y+dDisDolchulRigh+m_dCoverDn);
		if(m_nDetailMainUpper==1)	RightH[0] = RightH[1];	

		if(m_pDB->m_bUserSlabHunch)
		{
			if(m_bDivideRebarAtCant)
			{
				CDPoint xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, C[2], (C[1].y-C[2].y)/(C[1].x-C[2].x), m_dCoverUp, FALSE);
				xyArrRebar.Add(xyMatch);
				xyArrRebar.Add(CDPoint(A[9].x, A[9].y+m_dCoverDn));
				xyArrRebar.Add(CDPoint(A[6].x, A[6].y+m_dCoverDn));
				xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, A[6], (A[5].y-A[6].y)/(A[5].x-A[6].x), m_dCoverUp, FALSE);
				xyArrRebar.Add(xyMatch);
			}
			else
			{
				xyArrRebar.Add(RightH[0]);
				xyArrRebar.Add(RightH[1]);
				xyArrRebar.Add(C[1]+vY*m_dCoverDn);
				xyArrRebar.Add(C[2]+vY*m_dCoverDn);
				xyArrRebar.Add(CDPoint(A[9].x, A[9].y+m_dCoverDn));
				xyArrRebar.Add(CDPoint(A[6].x, A[6].y+m_dCoverDn));
				xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, CDPoint(A[6].x, A[6].y+m_dCoverDn), -mL, m_dCoverUp, FALSE);
				xyArrRebar.Add(xyMatch);
			}
		}
		else
		{
			xyArrRebar.Add(RightH[0]);
			xyArrRebar.Add(RightH[1]);
			xyArrRebar.Add(CDPoint(A[9].x, A[9].y+m_dCoverDn));
			xyArrRebar.Add(CDPoint(A[6].x, A[6].y+m_dCoverDn));
			xyMatch = m_pDB->GetMatchRebarPointDom(pBxMatch, CDPoint(A[6].x, A[6].y+m_dCoverDn), -mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
		}
		
		stRebarShape.m_sName     = "우측거더상면";
		stRebarShape.m_nCycle    = nCycle;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bUpper	 = FALSE;	
		stRebarShape.m_dArrInJoint = xyArrInnerJoint;
		vRebarShape.push_back(stRebarShape);
		xyArrRebar.RemoveAll();

		// 우측 캔틸레버 상면
		if(m_bDivideRebarAtCant && m_pDB->m_bUserSlabHunch)
		{
			double dSettleL = GetValueSettle(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), m_dDiaMainRebarLower_Cent[POS_SLAB_RIGH], JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dGenMainRebarCTCLower[POS_SLAB_RIGH]/2));
			CDPoint vVec = (C[1]-C[0]).Unit();
			CDPoint xyRigh = C[1] + vY*m_dCoverDn + vVec*dSettleL;

			REBAR_SHAPE stRebarShape2;
			xyArrRebar.Add(RightH[0]);
			xyArrRebar.Add(RightH[1]);
			xyArrRebar.Add(C[1]+vY*m_dCoverDn);
			xyArrRebar.Add(xyRigh);

			stRebarShape2.m_sName     = "우측캔틸레버상면";
			stRebarShape2.m_nCycle    = nCycle;
			stRebarShape2.m_dArrShape = xyArrRebar;
			stRebarShape2.m_bUpper	 = FALSE;	
			stRebarShape2.m_dArrInJoint = xyArrInnerJoint;
			vRebarShape.push_back(stRebarShape2);
			xyArrRebar.RemoveAll();
		}
	}


	// 중앙부 거더상면 보강철근
	if(m_pDB->GetGirdersu() < 3)	return;
	
	CDPoint APre[10];
	CString sText;

	for(long nG=1; nG<=m_pDB->GetGirdersu()-2; nG++)
	{
		REBAR_SHAPE stRebarShape;

		CPlateGirderApp *pGirPre = m_pDB->GetGirder(nG-1);
		CPlateGirderApp *pGirMatch = m_pDB->GetGirder(nG);
		CPlateBasicIndex *pBxMatchPre = pBx->GetBxMatchByCrossBeam(pGirPre);
		pGirPre->GetHunchXyDom(pBxMatchPre, APre, FALSE);
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirMatch);
		pGirMatch->GetHunchXyDom(pBxMatch, A, FALSE);

		CDPoint vDirL = (APre[5]-A[6]).Unit();
		CDPoint vDirR = (A[4]-A[3]).Unit();
		CDPoint xyMatchL, xyMatchR;
		if(m_pDB->m_bUserSlabHunch)
		{
			xyMatchL = m_pDB->GetMatchRebarPointDom(pBxMatchPre, APre[6], (APre[5].y-APre[6].y)/(APre[5].x-APre[6].x), m_dCoverUp, FALSE);
			xyMatchR = m_pDB->GetMatchRebarPointDom(pBxMatch, A[3], (A[4].y-A[3].y)/(A[4].x-A[3].x), m_dCoverUp, FALSE);
		}
		else
		{
			xyMatchL = m_pDB->GetMatchRebarPointDom(pBxMatchPre, APre[6]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
			xyMatchR = m_pDB->GetMatchRebarPointDom(pBxMatch, A[3]+vY*m_dCoverDn,  mL, m_dCoverUp, FALSE);
		}

		xyArrRebar.Add(xyMatchL);			
		xyArrRebar.Add(APre[6]+vY*m_dCoverDn);			
		xyArrRebar.Add(A[3]+vY*m_dCoverDn);			
		xyArrRebar.Add(xyMatchR);
		
		sText.Format("G%d상면", nG);
		stRebarShape.m_sName     = sText;
		stRebarShape.m_nCycle    = nCycle;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bUpper	 = FALSE;	
		stRebarShape.m_dArrInJoint = xyArrInnerJoint;
		vRebarShape.push_back(stRebarShape);
		xyArrRebar.RemoveAll();
	}
}



// nSection위치에서 가로보 및 세로보 상부의 슬래브 하면 철근을 생성
void CADeckData::MakeShapeMainRebarAtUpperCbStgWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape)
{
	// 중앙부 철근
	CString sText;
	CDPoint  ALeft[10], ARigh[10], A[10], CLeft[4], CRigh[4], vVec;	// 수정
	CDPointArray xyArrRebar, xyArrInnerJoint;
	CDPoint vX(1,0), vY(0,1), xyLeft, xyRigh;
	long nGsu = m_pDB->GetGirdersu();

	CPlateBasicIndex *pBx = GetBxBySection(nSection);
	CPlateBasicIndex *pBxMatchL = pBx->GetBxMatchByCrossBeam(m_pDB->GetGirder(0));
	CPlateBasicIndex *pBxMatchR = pBx->GetBxMatchByCrossBeam(m_pDB->GetGirder(nGsu-2));

	double dDia		= m_dDiaMainRebarLower_Cent[0];
	double dSettleL = GetValueSettle(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), dDia, JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dGenMainRebarCTCLower[0]/2));
	m_pDB->GetXyFrontCantilever(pBxMatchL, CLeft, TRUE);
	m_pDB->GetXyFrontCantilever(pBx->GetBxMatchByCrossBeam(m_pDB->GetGirder(nGsu-1)), CRigh, FALSE);

	//////////////////////////////////////////////////////////////////////////
	// stRebarShape 설정시 dDia, dValid는 shape생성후 shape을 정렬하여 설정함.
	//////////////////////////////////////////////////////////////////////////
	m_pDB->GetGirder(0)->GetHunchXyDom(pBxMatchL, ALeft, FALSE);
	m_pDB->GetGirder(nGsu-2)->GetHunchXyDom(pBxMatchR, ARigh, FALSE);
	// 가로보부 철근
	vVec = (ARigh[5] - ALeft[4]).Unit();
	if(m_nDetailSettlement==0)	// 헌치 상단부 기준
	{
		xyLeft = ALeft[4] - vVec * dSettleL;
		xyRigh = ARigh[5] + vVec * dSettleL;
	}
	else if(m_nDetailSettlement==1)	// 헌치 하단부 기준
	{
		CDPoint xyMatchL, xyMatchR;
		GetXyMatchLineAndLine(ALeft[4], vVec, ALeft[3], CDPoint(0,1), xyMatchL);
		GetXyMatchLineAndLine(ARigh[5], vVec, ARigh[6], CDPoint(0,1), xyMatchR);
		xyLeft = xyMatchL - vVec * dSettleL;
		xyRigh = xyMatchR + vVec * dSettleL;
	}
	//
	if(m_nDetailMainLowerCenter==1)
	{
		xyArrRebar.Add(CDPoint(xyLeft.x, xyLeft.y+m_dCoverDn));
		xyArrRebar.Add(CDPoint(xyRigh.x, xyRigh.y+m_dCoverDn));
	}
	else
	{
//		CDPoint xySlabLeft = CDPoint(GetSideBarDistFromCenterGir(nSection, TRUE), 0);	
//		CDPoint xySlabRigh = CDPoint(GetSideBarDistFromCenterGir(nSection, FALSE), 0);	
//
//		vVec = (ALeft[5] - ALeft[4]).Unit();
//		GetXyMatchLineAndLine(xySlabLeft,CDPoint(0,1),CDPoint(ALeft[4].x,ALeft[4].y+m_dCoverDn),vVec,xySlabLeft);
//
//		vVec = (ARigh[5] - ARigh[4]).Unit();
//		GetXyMatchLineAndLine(xySlabRigh,CDPoint(0,1),CDPoint(ARigh[5].x,ARigh[5].y+m_dCoverDn),vVec,xySlabRigh);	
//
//		xyArrRebar.Add(xySlabLeft);
//		xyArrRebar.Add(xySlabRigh);	
		// 캔틸레버 하면을 따라가는 방법으로 수정함
		xyArrRebar.Add(CLeft[0]+vX*m_dCoverSd+vY*m_dCoverDn);
		if(m_pDB->m_bUserSlabHunch)
			xyArrRebar.Add(CLeft[1]+vY*m_dCoverDn);
		xyArrRebar.Add(CRigh[0]-vX*m_dCoverSd+vY*m_dCoverDn);
		if(m_pDB->m_bUserSlabHunch)
			xyArrRebar.Add(CRigh[1]+vY*m_dCoverDn);
	}
	
	for (long nG=0; nG<nGsu-1; nG++)
	{
		m_pDB->GetGirder(nG)->GetHunchXyDom(pBx, A, FALSE);
		xyArrRebar.Add(A[4]+vY*m_dCoverDn);
		xyArrRebar.Add(A[5]+vY*m_dCoverDn);
	}

	xyArrRebar.Sort(TRUE, TRUE);


	REBAR_SHAPE stRebarShape;	 
	stRebarShape.m_sName     = "가로보상부전체";
	stRebarShape.m_nCycle    = nCycle;
	stRebarShape.m_dArrShape = xyArrRebar;
	stRebarShape.m_bUpper	 = FALSE;	
	stRebarShape.m_dArrInJoint = xyArrInnerJoint;
	vRebarShape.push_back(stRebarShape);
	
//	// 세로보위치 보강철근
//	for(long g=0; g<m_pDB->GetGirdersu()-1; g++)
//	{
//		xyArrRebar.RemoveAll();
//		
//		CPlateGirderApp *pGir	 = m_pDB->GetGirder(g);
//		pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);
//		pGir->GetHunchXyDom(pBxMatch, A, FALSE);
//
//		CPlateCrossBeam *pC = pBxMatch->GetCrossBeam();
//		if(pC==NULL)	continue;
//
//		long nStgSu = pC->m_nQtyStringer_CR;
//		if(nStgSu > 0)
//		{
//			CDPoint vDirL = (A[11]-A[12]).Unit();
//			CDPoint vDirR = (A[15]-A[14]).Unit();
//			CDPoint xyMatchL = m_pDB->GetMatchRebarPointDom(pBxMatch, A[12]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
//			CDPoint xyMatchR = m_pDB->GetMatchRebarPointDom(pBxMatch, A[14]+vY*m_dCoverDn,  mL, m_dCoverUp, FALSE);
//			xyArrRebar.Add(xyMatchL);			
//			xyArrRebar.Add(A[12]+vY*m_dCoverDn);			
//			xyArrRebar.Add(A[14]+vY*m_dCoverDn);			
//			xyArrRebar.Add(xyMatchR);
//			
//			REBAR_SHAPE stRebarShape;
//			sText.Format("세로보보강(%d-%d)", g, 0);
//			stRebarShape.m_sName     = sText;
//			stRebarShape.m_nCycle    = nCycle;
//			stRebarShape.m_dArrShape = xyArrRebar;
//			stRebarShape.m_bUpper	 = FALSE;	
//			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
//			vRebarShape.push_back(stRebarShape);
//		}
//		if(nStgSu > 1)
//		{
//			CDPoint vDirL = (A[16]-A[17]).Unit();
//			CDPoint vDirR = (A[20]-A[19]).Unit();
//			CDPoint xyMatchL = m_pDB->GetMatchRebarPointDom(pBxMatch, A[17]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
//			CDPoint xyMatchR = m_pDB->GetMatchRebarPointDom(pBxMatch, A[19]+vY*m_dCoverDn,  mL, m_dCoverUp, FALSE);
//			xyArrRebar.Add(xyMatchL);			
//			xyArrRebar.Add(A[17]+vY*m_dCoverDn);			
//			xyArrRebar.Add(A[19]+vY*m_dCoverDn);			
//			xyArrRebar.Add(xyMatchR);
//			
//			REBAR_SHAPE stRebarShape;	
//			sText.Format("세로보보강(%d-%d)", g, 1);
//			stRebarShape.m_sName     = sText;
//			stRebarShape.m_nCycle    = nCycle;
//			stRebarShape.m_dArrShape = xyArrRebar;
//			stRebarShape.m_bUpper	 = FALSE;	
//			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
//			vRebarShape.push_back(stRebarShape);
//		}
//		if(nStgSu > 2)
//		{
//			CDPoint vDirL = (A[21]-A[22]).Unit();
//			CDPoint vDirR = (A[25]-A[24]).Unit();
//			CDPoint xyMatchL = m_pDB->GetMatchRebarPointDom(pBxMatch, A[22]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
//			CDPoint xyMatchR = m_pDB->GetMatchRebarPointDom(pBxMatch, A[24]+vY*m_dCoverDn,  mL, m_dCoverUp, FALSE);
//			xyArrRebar.Add(xyMatchL);			
//			xyArrRebar.Add(A[22]+vY*m_dCoverDn);			
//			xyArrRebar.Add(A[24]+vY*m_dCoverDn);			
//			xyArrRebar.Add(xyMatchR);
//			
//			REBAR_SHAPE stRebarShape;	
//			sText.Format("세로보보강(%d-%d)", g, 2);
//			stRebarShape.m_sName     = sText;
//			stRebarShape.m_nCycle    = nCycle;
//			stRebarShape.m_dArrShape = xyArrRebar;
//			stRebarShape.m_bUpper	 = FALSE;	
//			stRebarShape.m_dArrInJoint = xyArrInnerJoint;
//			vRebarShape.push_back(stRebarShape);
//		}
//	}
}



// nSection위치에서 외측가로보 상부의 슬래브 하면 철근을 생성	
void CADeckData::MakeShapeMainRebarAtUpperOutBkWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape)
{
/*	CDPoint vX(1,0), vY(0,1);
	
	double dDisDolchulLeft = GetLengthMainDolchul(nSection, FALSE, TRUE);		// ㄱ형태 일 경우 돌출길이
	double dDisDolchulRigh = GetLengthMainDolchul(nSection, FALSE, FALSE);
	if(m_nDetailMainUpper == 1)		dDisDolchulLeft = dDisDolchulRigh = 0.1;	// 약간 돌출되게 함(동일점 제거시 예외를 두기 위해서)

	double dJointL			  = GetJointLengthAtHDan(nSection, FALSE, POS_SLAB_LEFT);		// 슬래브좌측과 중앙부의 이음길이
	double dJointR			  = GetJointLengthAtHDan(nSection, FALSE, POS_SLAB_RIGH);		// 슬래브우측과 중앙부의 이음길이
	double dJointC			  = GetJointLengthAtHDan(nSection, FALSE, POS_SLAB_CENTER);		// 슬래브 중앙부 철근의 이음길이
	double dLengthCantilLeft  = m_pDB->GetWidSlabUpFlangeWeb(NULL, TRUE)  + m_dRangeMainRebarFromFlangeStt;
	double dLengthCantilRight = m_pDB->GetWidSlabUpFlangeWeb(NULL, FALSE) + m_dRangeMainRebarFromFlangeStt;

	// 좌측캔틸레버부 철근
	CPlateGirderApp *pGirL  = m_pDB->GetGirder(0);
	CPlateGirderApp *pGirR  = m_pDB->GetGirder(m_pDB->GetGirdersu()-1);
	CPlateGirderApp *pGirRR = m_pDB->GetGirder(m_pDB->GetGirdersu()-2);
	CPlateBasicIndex *pBx   = GetBxBySection(nSection);	
	BOOL bLeftBracket  = pBx->GetBxMatchByCrossBeam(pGirL)->GetBracket(TRUE)==NULL ? FALSE : TRUE;
	BOOL bRighBracket  = pBx->GetBxMatchByCrossBeam(pGirR)->GetBracket(FALSE)==NULL ? FALSE : TRUE;
	CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGirL);
	CDPoint xyMatch, vVec, xySlabLeft, xySlabRigh, xyResult, A[10], C[50], LeftH[3], RightH[3];
	CDPointArray xyArrRebar, xyArrInnerJoint;
	CString sText;

	double dSettleL = 0;
	double	mL	    = 1.0/3.0;		//거더 좌측 헌치 기울기

	//////////////////////////////////////////////////////////////////////////
	// stRebarShape 설정시 dDia, dValid는 shape생성후 shape을 정렬하여 설정함.
	//////////////////////////////////////////////////////////////////////////

	// 외측가로보(좌측) 존재시                                                                 
	if(bLeftBracket)
	{
		// 외측빔헌치보강(좌)
		dSettleL = GetValueSettle(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), m_dDiaMainRebarLower_Cent[0], JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dGenMainRebarCTCLower[0]/2));
		pDB->GetXyFrontCantilever(pBxMatch,C, TRUE);
		pGirL->GetHunchXyDom(pBxMatch, A, FALSE);
		if(pBxMatch==NULL)	return;

		if(EQUAL(C[8].x, C[9].x, 1) || EQUAL(C[8].y, C[9].y, 1))
		{
			xyArrRebar.Add(C[9]+vX*m_dCoverSd+vY*m_dCoverDn);
			xyArrRebar.Add(C[5]+vY*m_dCoverDn);
			xyMatch = m_pDB->GetMatchRebarPointDom(pBx, C[5]+vY*m_dCoverDn, mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
		}
		else
		{
			CDPoint vAngHunch9 = (CDPoint(0,1)+(C[8]-C[9]).Unit()).Unit();
			CDPoint vAngHunch8 = ((C[9]-C[8]).Unit()+(C[7]-C[8]).Unit()).Unit();
			CDPoint xyC9Offset = C[9] + vAngHunch9*(m_dCoverDn/fabs(vAngHunch9.x));
			CDPoint xyC8Offset = C[8] + vAngHunch8*(m_dCoverDn/fabs(vAngHunch8.y));
			
			GetXyMatchLineAndLine(xyC9Offset, (C[9]-C[8]).Unit(), C[9]+vX*m_dCoverSd, vY, xyResult);
			xyArrRebar.Add(xyResult+vY*dDisDolchulLeft);
			xyArrRebar.Add(xyResult);
			xyArrRebar.Add(xyC8Offset);
			xyArrRebar.Add(C[5]+vY*m_dCoverDn);
			xyMatch = m_pDB->GetMatchRebarPointDom(pBx, C[5]+vY*m_dCoverDn, mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
		}

		REBAR_SHAPE stRebarShape;	
		stRebarShape.m_sName     = "외측빔헌치보강(좌)";
		stRebarShape.m_nCycle    = nCycle;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bUpper	 = FALSE;	
		stRebarShape.m_dArrInJoint = xyArrInnerJoint;
		vRebarShape.push_back(stRebarShape); 
		xyArrRebar.RemoveAll();

		// 외측가로보상부(좌)
		vVec = (C[3] - C[4]).Unit();
		if(m_nDetailSettlement==0)		// 헌치 상단부 기준
		{
			xySlabLeft = C[4] - vVec * dSettleL +vY*m_dCoverDn;
			xySlabRigh = C[3] + vVec * dSettleL +vY*m_dCoverDn;
		}
		else if(m_nDetailSettlement==1)	// 헌치 하단부 기준
		{
			CDPoint xyMatchL, xyMatchR;
			GetXyMatchLineAndLine(C[4]+vY*m_dCoverDn, vVec, C[5], vY, xyMatchL);
			GetXyMatchLineAndLine(C[3]+vY*m_dCoverDn, vVec, C[2], vY, xyMatchR);
			xySlabLeft = xyMatchL - vVec * dSettleL;
			xySlabRigh = xyMatchR + vVec * dSettleL;
		}
		else if(m_nDetailSettlement==2)	// 복부판 내측기준
		{
			CDPoint xyMatchL, xyMatchR;
			GetXyMatchLineAndLine(C[4]+vY*m_dCoverDn, vVec, C[5], vY, xyMatchL);
			GetXyMatchLineAndLine(C[3]+vY*m_dCoverDn, vVec, C[0], vY, xyMatchR);
			xySlabLeft = xyMatchL - vVec * dSettleL;
			xySlabRigh = xyMatchR + vVec * dSettleL;
		}
		
		xyArrRebar.Add(xySlabLeft);
		xyArrRebar.Add(xySlabRigh);

		REBAR_SHAPE stRebarShape2;	
		stRebarShape2.m_sName     = "외측가로보상부(좌)";
		stRebarShape2.m_nCycle    = nCycle;
		stRebarShape2.m_dArrShape = xyArrRebar;
		stRebarShape2.m_bUpper	 = FALSE;	
		stRebarShape2.m_dArrInJoint = xyArrInnerJoint;
		vRebarShape.push_back(stRebarShape2); 
		xyArrRebar.RemoveAll();

	}
	// 외측가로보(우측) 존재시                                                                                                                             
	if(bRighBracket)
	{
		dSettleL = GetValueSettle(toKgPCm2(m_SigmaCK), toKgPCm2(m_SigmaY), m_dDiaMainRebarLower_Cent[1], JOINT_TENS_NORMAL_B, toCm(m_dCoverDn), toCm(m_dGenMainRebarCTCLower[1]/2));
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR);
		CPlateBasicIndex *pBxMatchRR = pBx->GetBxMatchByCrossBeam(pGirRR);
		if(pBxMatchRR==NULL)	return;
		
		pDB->GetXyFrontCantilever(pBxMatch,C, FALSE);
		pGirRR->GetHunchXyDom(pBxMatchRR, A, FALSE);

		// 외측빔헌치보강(우)
		if(EQUAL(C[8].x, C[9].x, 1) || EQUAL(C[8].y, C[9].y, 1))
		{
			xyArrRebar.Add(C[9]-vX*m_dCoverSd+vY*m_dCoverDn);
			xyArrRebar.Add(C[5]+vY*m_dCoverDn);
			xyMatch = m_pDB->GetMatchRebarPointDom(pBx, C[5]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
		}
		else
		{
			CDPoint vAngHunch9 = (CDPoint(0,1)+(C[8]-C[9]).Unit()).Unit();
			CDPoint vAngHunch8 = ((C[9]-C[8]).Unit()+(C[7]-C[8]).Unit()).Unit();
			CDPoint xyC9Offset = C[9] + vAngHunch9*(m_dCoverDn/fabs(vAngHunch9.x));
			CDPoint xyC8Offset = C[8] + vAngHunch8*(m_dCoverDn/fabs(vAngHunch8.y));

			GetXyMatchLineAndLine(xyC9Offset, (C[9]-C[8]).Unit(), C[9]+vX-m_dCoverSd, vY, xyResult);
			xyArrRebar.Add(xyResult+vY*dDisDolchulRigh);
			xyArrRebar.Add(xyResult);
			xyArrRebar.Add(xyC8Offset);
			xyArrRebar.Add(C[5]+vY*m_dCoverDn);
			xyMatch = m_pDB->GetMatchRebarPointDom(pBx, C[5]+vY*m_dCoverDn, -mL, m_dCoverUp, FALSE);
			xyArrRebar.Add(xyMatch);
		}

		REBAR_SHAPE stRebarShape;	
		stRebarShape.m_sName     = "외측빔헌치보강(우)";
		stRebarShape.m_nCycle    = nCycle;
		stRebarShape.m_dArrShape = xyArrRebar;
		stRebarShape.m_bUpper	 = FALSE;	
		stRebarShape.m_dArrInJoint = xyArrInnerJoint;
		vRebarShape.push_back(stRebarShape); 
		xyArrRebar.RemoveAll();

		// 외측가로보상부(좌)
		vVec = (C[3] - C[4]).Unit();
		if(m_nDetailSettlement==0)		// 헌치 상단부 기준
		{
			xySlabLeft = C[3] + vVec * dSettleL +vY*m_dCoverDn;
			xySlabRigh = C[4] - vVec * dSettleL +vY*m_dCoverDn;
		}
		else if(m_nDetailSettlement==1)	// 헌치 하단부 기준
		{
			CDPoint xyMatchL, xyMatchR;
			GetXyMatchLineAndLine(C[4]+vY*m_dCoverDn, vVec, C[5], vY, xyMatchR);
			GetXyMatchLineAndLine(C[3]+vY*m_dCoverDn, vVec, C[2], vY, xyMatchL);
			xySlabLeft = xyMatchL + vVec * dSettleL;
			xySlabRigh = xyMatchR - vVec * dSettleL;
		}
		else if(m_nDetailSettlement==2)	// 복부판 내측기준
		{
			CDPoint xyMatchL, xyMatchR;
			GetXyMatchLineAndLine(C[4]+vY*m_dCoverDn, vVec, C[5], vY, xyMatchR);
			GetXyMatchLineAndLine(C[3]+vY*m_dCoverDn, vVec, C[0], vY, xyMatchL);
			xySlabLeft = xyMatchL + vVec * dSettleL;
			xySlabRigh = xyMatchR - vVec * dSettleL;
		}
		xyArrRebar.Add(xySlabRigh);
		xyArrRebar.Add(xySlabLeft);

		REBAR_SHAPE stRebarShape2;	 
		stRebarShape2.m_sName     = "외측가로보상부(우)";
		stRebarShape2.m_nCycle    = nCycle;
		stRebarShape2.m_dArrShape = xyArrRebar;
		stRebarShape2.m_bUpper	 = FALSE;	
		stRebarShape2.m_dArrInJoint = xyArrInnerJoint;
		vRebarShape.push_back(stRebarShape2); 
		xyArrRebar.RemoveAll();
	}*/
}




void CADeckData::AddRebarShapeToRebarShape(vector<REBAR_SHAPE>& vRebarShapeOrigin, vector<REBAR_SHAPE>& vRebarShapeToAdd)
{
	for (long nShape=0; nShape<(long)vRebarShapeToAdd.size(); nShape++)
	{
		REBAR_SHAPE stRebarShape;
		stRebarShape.m_bUpper	  = vRebarShapeToAdd[nShape].m_bUpper	  ;
		stRebarShape.m_bValid	  = vRebarShapeToAdd[nShape].m_bValid	  ;
		stRebarShape.m_dArrInJoint= vRebarShapeToAdd[nShape].m_dArrInJoint;
		stRebarShape.m_dArrShape  = vRebarShapeToAdd[nShape].m_dArrShape  ;
		stRebarShape.m_dDia		  = vRebarShapeToAdd[nShape].m_dDia		  ;
		stRebarShape.m_nCycle	  = vRebarShapeToAdd[nShape].m_nCycle	  ;
		stRebarShape.m_sName	  = vRebarShapeToAdd[nShape].m_sName	  ;

		vRebarShapeOrigin.push_back(stRebarShape);
	}
}



void CADeckData::ModifyDimArr(CDPointArray &arrDim)
{
	CDoubleArray dArrOffset;
	CRebarPlacing RebarPlace;
	RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(arrDim, dArrOffset, TRUE);

	CDoubleArray dArrModifyAcul, dArrModify;
	long		 n = 0;
	dArrModifyAcul.Add(0);
	for (n=0; n<dArrOffset.GetSize(); n++)
	{
		if(n%2==1 || n==dArrOffset.GetUpperBound())
			dArrModifyAcul.Add(dArrOffset[n]);
	}

	for (n=0; n<dArrModifyAcul.GetSize()-1; n++)
		dArrModify.Add(dArrModifyAcul[n+1]-dArrModifyAcul[n]);

	RebarPlace.ConvertOffsetDoubleArrayToOffsetInfo(dArrModify, arrDim);
}


CDPointArray CADeckData::ModifyPointByCount(CDPointArray &xyArrPoint, long nCount)
{
	CDPointArray xyArrReturn;
	long nTotal = 0;
	for (long n=0; n<xyArrPoint.GetSize(); n++)
	{
		if(n%2==0 && nTotal<nCount)
		{
			xyArrReturn.Add(xyArrPoint[n]);
			nTotal++;
		}
	}

	return xyArrReturn;
}



// 일단 모든구간을 가변구간으로 가정함.
void CADeckData::SetCTCArrToCTCArr(CDPointArray &arrCTCSource, CDPointArray &arrCTCTarget, double dRatio) 
{
	arrCTCTarget.RemoveAll();

	for (long n=0; n<arrCTCSource.GetSize(); n++)
		arrCTCTarget.Add(CDPoint(arrCTCSource[n].x, arrCTCSource[n].y*dRatio));
}


void CADeckData::SetCTCBeryukRebar(CDPointArray& xyArrDim, long nSection, BOOL bUpper)
{
	if(nSection==SECTION_DAN || nSection==SECTION_SKEW_STT)
		m_arrCTCBeryukRebarDan[bUpper?0:1] = xyArrDim;
	else if(nSection==SECTION_CEN)
		m_arrCTCBeryukRebarCen[bUpper?0:1] = xyArrDim;
	else if(nSection==SECTION_PIER)	
		m_arrCTCBeryukRebarJijum[bUpper?0:1] = xyArrDim;

}





// 일단 모든구간을 가변구간으로 가정함.
void CADeckData::SetCycleToCycle(vector<REBAR_CYCLE> &vCycleSource, vector<REBAR_CYCLE> &vCycleTarget, double dRatio) 
{
	vCycleTarget.clear();
	vCycleTarget.reserve(vCycleSource.size());

	for (long nCycle=0; nCycle<(long)vCycleSource.size(); nCycle++) 
	{
		REBAR_CYCLE Cycle;
		
		CDoubleArray arrUp = vCycleSource[nCycle].m_dArrJointUp;
		CDoubleArray arrDn = vCycleSource[nCycle].m_dArrJointDn;
		long		 n	= 0;

		for (n=0; n<arrUp.GetSize(); n++) 
		{
			double dLength = arrUp[n];
			Cycle.m_dArrJointUp.Add(dLength*dRatio);	 
		}
		for ( n=0; n<arrDn.GetSize(); n++) 
		{
			double dLength = arrDn[n];
			Cycle.m_dArrJointDn.Add(dLength*dRatio);
		}

		Cycle.m_dArrDiaUp	 	=	vCycleSource[nCycle].m_dArrDiaUp;	 
		Cycle.m_dArrDiaDn	 	=	vCycleSource[nCycle].m_dArrDiaDn;	 
		Cycle.m_dArrValidUp    	=	vCycleSource[nCycle].m_dArrValidUp;    
		Cycle.m_dArrValidDn	 	=	vCycleSource[nCycle].m_dArrValidDn; 

		vCycleTarget.push_back(Cycle);
	}
}





double CADeckData::GetLengthCoverSd(BOOL bSttSlab, long nJongBasePos)
{
	CDPoint A[4], AA[4];
	double dRatio = 1;
	if(bSttSlab)
	{
		m_pDB->GetXySss(OFF_STT_SLAB, A);
		m_pDB->GetXySss(OFF_STT_COVER, AA);
		dRatio = fabs(m_pDB->GetGirder(-1)->GetAngleStt().y);
	}
	else
	{
		m_pDB->GetXySss(OFF_END_SLAB, A);
		m_pDB->GetXySss(OFF_END_COVER, AA);
		dRatio = fabs(m_pDB->GetGirder(-1)->GetAngleEnd().y);
	}
	CDPoint vDir  = (A[3]-A[0]).Unit();
	
	if(nJongBasePos==JONGBASEPOS_SLABLEFT)
	{
		return ~(A[0] - AA[0]);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)
	{
		return ~(A[3] - AA[3]);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABCENTER)
	{
		CDPoint xyStt = (A[0] + vDir*~(A[0]-A[3])/2);
		CDPoint xyEnd = (AA[0] + vDir*~(AA[0]-AA[3])/2);
		return ~(xyStt - xyEnd);
	}
	else if(nJongBasePos==JONGBASEPOS_SLABLC)
	{
		return ~((A[0]+vDir*m_dWidthSlabCantLeftForBeryuk/dRatio) - (AA[0]+vDir*m_dWidthSlabCantLeftForBeryuk/dRatio));

	}
	else if(nJongBasePos==JONGBASEPOS_SLABRC)
	{
		return ~((A[3]-vDir*m_dWidthSlabCantRighForBeryuk/dRatio) - (AA[3]-vDir*m_dWidthSlabCantRighForBeryuk/dRatio));
	}
	else if(nJongBasePos==JONGBASEPOS_ONCB)
	{
		double dSta  = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
		double dDist = GetDistByJongBase(nJongBasePos);
		double dDistL = m_pDB->GetDisSlabLeftAct(dSta);
		return ~((A[0]+vDir*(dDist-dDistL)/dRatio) - (AA[0]+vDir*(dDist-dDistL)/dRatio));
	}


	return 0;
}



long CADeckData::GetsuMainRebarShapeAtCen()
{
	long nGsu = m_pDB->GetGirdersu();
	CDPoint vX(1,0), vY(0,1);

	BOOL bOneBox	 = nGsu==1 ? TRUE : FALSE;								// 단박스?
	BOOL bHasHunch   = IsBridgeHasSlabHunch();								// 슬래브헌치가 있나?
	BOOL bLeftBrk    = IsBridgeHasOutBracket(SECTION_DAN, TRUE)&&bHasHunch ? TRUE : FALSE;					// 좌측브라켓만?
	BOOL bRighBrk    = IsBridgeHasOutBracket(SECTION_DAN, FALSE)&&bHasHunch ? TRUE : FALSE;					// 우측브라켓만?	
	BOOL bLeftOnlyBrk=  bLeftBrk&&!bRighBrk ? TRUE : FALSE;					// 좌측브라켓만?
	BOOL bRighOnlyBrk= !bLeftBrk&& bRighBrk ? TRUE : FALSE;					// 우측브라켓만?	
	BOOL bAllBrk	 =  bLeftBrk&& bRighBrk ? TRUE : FALSE;					// 양측브라켓?
	BOOL bNoBrk		 = !bLeftBrk&&!bRighBrk? TRUE : FALSE;					// 브라켓이 없나?
	BOOL bMultiBoxHasHunch   = !bOneBox&& bHasHunch ? TRUE : FALSE;			// 다박스고 슬래브 헌치가 있나?
	CPlateGirderApp *pGirL  = m_pDB->GetGirder(0);
	CPlateGirderApp *pGirR  = m_pDB->GetGirder(m_pDB->GetGirdersu()-1);
	CPlateGirderApp *pGirRR = m_pDB->GetGirder(m_pDB->GetGirdersu()-2);

	CPlateBasicIndex *pBx      = GetBxBySection(SECTION_DAN);	
	CPlateBasicIndex *pBxMatch = NULL;

	long nShapeTotal = 0;
	// TYPE 1
	if( (bOneBox&&bAllBrk) || (bMultiBoxHasHunch&&bLeftOnlyBrk) || (bMultiBoxHasHunch&&bRighOnlyBrk) || (bMultiBoxHasHunch&&bAllBrk))	
	{
		// 좌측
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirL);
		if(bLeftBrk && pBxMatch)
			nShapeTotal++;		
		// 우측
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGirRR);
		if((bRighBrk && !bOneBox) && pBxMatch)
			nShapeTotal++;		
	}
	
	// TYPE 2 ( 좌측거더상면 철근)
	pBxMatch = pBx->GetBxMatchByCrossBeam(pGirL);
	if(pBxMatch && ((bOneBox&&bRighOnlyBrk) || (bMultiBoxHasHunch&&bRighOnlyBrk) || (bMultiBoxHasHunch&&bNoBrk)))	
	{
		nShapeTotal++;		
		if(m_bDivideRebarAtCant && m_pDB->m_bUserSlabHunch)
			nShapeTotal++;		
	}

	// TYPE 3 ( 우측거더상면 철근)
	pBxMatch = pBx->GetBxMatchByCrossBeam(pGirR);
	if(pBxMatch && ((bOneBox&&bLeftOnlyBrk) || (bMultiBoxHasHunch&&bLeftOnlyBrk) || (bMultiBoxHasHunch&&bNoBrk)))	
	{
		nShapeTotal++;		
		if(m_bDivideRebarAtCant && m_pDB->m_bUserSlabHunch)
			nShapeTotal++;		
	}

	// 중앙부 거더상면 보강철근
	if(m_pDB->GetGirdersu() > 2)
		nShapeTotal += m_pDB->GetGirdersu()-2; 			

	// 가로보 상면 주철근
	if(IsBridgeHasCrossBeam())
	{
		// 중앙부 철근
//		if(m_nDetailMainLowerCenter==2)				// 전체배치형
			nShapeTotal++;		
//		else
//		{
//			if(m_nDetailMainLowerCenter==0)			// 분리배치형
//			{			
//			} 
//			else if(m_nDetailMainLowerCenter==1)	// 중앙부배치형
//				nShapeTotal += m_pDB->GetGirdersu()-1;		
//		}
		
		// 세로보위치 보강철근
		for(long g=0; g<m_pDB->GetGirdersu()-1; g++)
		{
			CPlateGirderApp *pGir = m_pDB->GetGirder(g);
			pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);

			CPlateCrossBeam *pC = pBxMatch->GetCrossBeam();
			if(pC==NULL)	continue;

			long nStgSu = pC->m_nQtyStringer_CR;
			if(nStgSu > 0)
				nShapeTotal++;		
			if(nStgSu > 1)
				nShapeTotal++;		
			if(nStgSu > 2)
				nShapeTotal++;		
		}
	}

	if(bLeftBrk)
		nShapeTotal+=2;
	if(bRighBrk)
		nShapeTotal+=2;
	
	if(nShapeTotal==0)
		return 1;
	
	return nShapeTotal;
}



double CADeckData::GetDistByJongBase(long nJongBasePos)
{
	CPlateBridgeApp      *pDB	 = m_pDB;
	CPlateGirderApp		*pGirCen = pDB->GetGirder(-1);
	
	double dSta = pGirCen->GetSSS(OFF_STT_SLAB);
	double dDist = 0;			// 선형거더 중심에서 이격거리
	if(nJongBasePos==JONGBASEPOS_SLABCENTER)	// 교량중심
	{
		if(pDB->GetDisSlabLeftAct(dSta)>0 || pDB->GetDisSlabRighAct(dSta)<0)	// 선형이 슬래브 밖에 있을때 
			dDist = (GetSideBarDistFromCenterGir(TRUE, FALSE) + GetSideBarDistFromCenterGir(TRUE, TRUE))/2;
		else	// 선형이 슬래브 안에 있을때
		{
			double dSlabW = pDB->GetWidthAct(dSta);
			dDist = dSlabW/2 + pDB->GetDisSlabLeftAct(dSta); 
		}
	}
	else if(nJongBasePos==JONGBASEPOS_SLABLEFT)	// 슬래브좌측
		dDist = pDB->GetDisSlabLeftAct(dSta)+m_dCoverSd;
	else if(nJongBasePos==JONGBASEPOS_SLABRIGHT)	// 슬래브우측
		dDist = pDB->GetDisSlabRighAct(dSta)-m_dCoverSd;
	else if(nJongBasePos==JONGBASEPOS_SLABLC)	// 슬래브우측
		dDist = m_pDB->GetDisSlabLeftAct(dSta) + m_dWidthSlabCantLeftForBeryuk; 
	else if(nJongBasePos==JONGBASEPOS_SLABRC)	// 슬래브우측
		dDist = m_pDB->GetDisSlabRighAct(dSta) - m_dWidthSlabCantRighForBeryuk; 
	else if(nJongBasePos==JONGBASEPOS_ONCB)	// 슬래브우측
	{
		CPlateGirderApp *pGir = pDB->GetGirder(0);
		dDist = (pGir->m_dGirderCenter+pGir->m_pRight->m_dGirderCenter)/2;
	}

	return dDist;

}

// sRebarName	: 철근의 이름
// vArrMain		: 철근집합
CDPointArray CADeckData::GetXyArrPoint(CString sRebarName, vector<REBAR_SHAPE>& vArrShape)
{
	CDPointArray xyArrRebar;

	for(long n=0; n<(long)vArrShape.size(); n++)
	{
		if(vArrShape[n].m_sName == sRebarName)
		{
			xyArrRebar = vArrShape[n].m_dArrShape;
			break;
		}
	}

	return xyArrRebar;
}


// sRebarName	: 철근의 이름
// vArrMain		: 철근집합
long CADeckData::GetNoShapeAtHDanShapeArr(CString sRebarName, vector<REBAR_SHAPE>& vArrShape)
{
	for(long n=0; n<(long)vArrShape.size(); n++)
	{
		if(vArrShape[n].m_sName == sRebarName)
		{
			return n;
		}
	}

	return 0;
}


CDPointArray CADeckData::GetXyArrJointPosAtJongBasePos(long nJongBasePos, long nCycle, BOOL bUpper)
{
	REBAR_CYCLE* pRebarCycle = GetRebarCycleByJongBasePos(nJongBasePos, nCycle);

	CDPointArray xyArrJoint;
	CDPointArray xyArrGuide = GetXyArrGuideLineForBeryukRebar(nJongBasePos, bUpper);
	
	CDoubleArray arrJoint = bUpper ? pRebarCycle->m_dArrJointUp : pRebarCycle->m_dArrJointDn;
	CDoubleArray dArrPos;	// 원점(슬래브상/하면-주철근 시작위치)을 기준으로한 절대위치
	double dSum = xyArrGuide[0].x;
	xyArrJoint.Add(CDPoint(dSum,0));
	for(long n=0; n<arrJoint.GetSize(); n++)
	{
		dSum += arrJoint[n];
		xyArrJoint.Add(CDPoint(dSum,0));
	}
	
	CDPointArray xyArrReturn = GetCDPointArrByPolyLine(xyArrJoint, xyArrGuide, TRUE);	// 이음중심 위치

	return xyArrReturn;
}



// 가로보 헌치하단 보강 철근 배열(V형)
// pBx = 가로보 Bx
void CADeckData::GetBoganRebarArrAtOutBrk(CDoubleArray &dDistArr, CPlateBasicIndex *pBx, BOOL bLeft)
{
/*	CPlateBridgeApp	*pDB  = m_pDB;
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CBracket	*pBrk = pBx->GetBracket(bLeft);
	if(!pBrk) return;

	long nOutBkIdx = pBrk->GetSelfCount();
	if(!bLeft)
		nOutBkIdx += pDB->GetLBracketsu();

	CDPoint xyDim = m_arrCTCBogangRebarAtOutBrk.GetAt(nOutBkIdx);

	CDPoint xySttCross = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, TRUE);
	CDPoint xyEndCross = pGir->GetXyPlanSlabOutBrkBase(pBx, bLeft, FALSE);

	double dLDist = pDB->m_pLine->GetDisMatchVerticalByXy(xySttCross);
	double dRDist = pDB->m_pLine->GetDisMatchVerticalByXy(xyEndCross);

	double dDist = (dLDist + dRDist)/2 - (xyDim.x * xyDim.y)/2;			
	for(long nCount=0; nCount<=xyDim.x; nCount++)
	{					
		dDistArr.Add(dDist);			
		dDist += xyDim.y;			
	}	*/
}




// 마크위치에서 슬래브 시점부나 종점부까지의거리를 계산하여 리턴함
double CADeckData::GetLengthExtendToSlabCenFrPoint(CDPointArray xyArrMark, long nJigan, double dExtPlus, BOOL bFromSttMark, BOOL bSttSlabCenLine)
{
	if(xyArrMark.GetSize() < 2)		return 0;

	CDPoint xyResult;
	double LDist = GetSideBarDistFromCenterGir(SECTION_CEN, TRUE);
	double RDist = GetSideBarDistFromCenterGir(SECTION_CEN, FALSE);

	CDPoint pt1, pt2; 
	double dStaBridgeStt = m_pDB->GetGirder(-1)->GetSSS(OFF_STT_SLAB);
	double dSta = dStaBridgeStt;

	long nEnd = min(nJigan*2+1+(bSttSlabCenLine?0:1), m_arrTrimRange.GetSize());
	for(long n=0; n<nEnd; n++)
		dSta += m_arrTrimRange.GetAt(n);				

	CDPoint xy1  = m_pDB->m_pLine->GetXyLineDis(dSta, LDist);
	CDPoint xy2  = m_pDB->m_pLine->GetXyLineDis(dSta, RDist);
	CDPoint vVec = (xy2-xy1).Unit();
	CDPoint xyc = xy1 + ~(xy2-xy1)/2 * vVec + (bSttSlabCenLine ? vVec.Rotate90() : vVec.RotateInv90()) * 1000;

	CDPoint xy1Temp  = xy1 + (bSttSlabCenLine ? vVec.Rotate90() : vVec.RotateInv90()) * TRIM_ARC_LEN;
	CDPoint xy2Temp  = xy2 + (bSttSlabCenLine ? vVec.Rotate90() : vVec.RotateInv90()) * TRIM_ARC_LEN;		
	CDPoint xycTemp = xy1Temp + ~(xy2Temp-xy1Temp)/2 * vVec + (bSttSlabCenLine? vVec.Rotate90() : vVec.RotateInv90()) * 100;

	GetXyMatchLineAndLine(xy1Temp, (xy1Temp-xy2Temp).Unit(), xyArrMark[0], (xyArrMark[0]-xyArrMark[xyArrMark.GetUpperBound()]).Unit(), xyResult);

	double dLengthExt = 0;
	
	if (bFromSttMark)
		dLengthExt = ~(xyResult-xyArrMark[0]) + dExtPlus;
	else
		dLengthExt = ~(xyResult-xyArrMark[xyArrMark.GetUpperBound()]) + dExtPlus;

	return dLengthExt;
}


BOOL CADeckData::IsPehapCycle(long nJongBasePos, long nCycle)
{
	BOOL bPehap = TRUE;
	vector <REBAR_SHAPE>* pvShape = GetRebarShapeArrByJongBasePos(nJongBasePos, nCycle);
	for (long nShape=0; nShape<(long)pvShape->size(); nShape++)
	{
		REBAR_SHAPE shape = pvShape->at(nShape);
		if(shape.m_sName.Find("시점부") != -1 || shape.m_sName.Find("종점부") != -1)		continue;

		if(!pvShape->at(nShape).m_bValid)
			bPehap = FALSE;
	}

	return bPehap;
}



BOOL CADeckData::IsPointInJigan(CDPoint xyCen, long nJigan)
{
	CPlateBridgeApp *pDB     = m_pDB;
	CPlateGirderApp *pGirCen = pDB->GetGirder(-1);

	CRebarPlacing RebarPlace;
	double dStaExt = 1000;

	double dStaStt = pGirCen->GetStationOnJijum(nJigan);
	if(nJigan==0)
		dStaStt   = pGirCen->GetSSS(OFF_STT_SLAB)-dStaExt;
	double dStaEnd = pGirCen->GetStationOnJijum(nJigan+1);
	if(nJigan+1==pDB->m_nQtyJigan)
		dStaEnd = pGirCen->GetSSS(OFF_END_SLAB)+dStaExt;

	CDPoint xySttLeft = pDB->m_pLine->GetXyLineDisByAng(dStaStt, pDB->GetDisSlabLeftAct(dStaStt)-2000, pGirCen->GetAngleOnJijum(nJigan));
	CDPoint xySttRigh = pDB->m_pLine->GetXyLineDisByAng(dStaStt, pDB->GetDisSlabRighAct(dStaStt)+2000, pGirCen->GetAngleOnJijum(nJigan));
	CDPoint xyEndLeft = pDB->m_pLine->GetXyLineDisByAng(dStaEnd, pDB->GetDisSlabLeftAct(dStaEnd)-2000, pGirCen->GetAngleOnJijum(nJigan+1));
	CDPoint xyEndRigh = pDB->m_pLine->GetXyLineDisByAng(dStaEnd, pDB->GetDisSlabRighAct(dStaEnd)+2000, pGirCen->GetAngleOnJijum(nJigan+1));
	
	CDPointArray xyArrPoly;
	xyArrPoly.Add(xySttLeft);
	xyArrPoly.Add(xySttRigh);
	xyArrPoly.Add(xyEndRigh);
	xyArrPoly.Add(xyEndLeft);
	xyArrPoly.Add(xySttLeft);

	return RebarPlace.CheckXyByPolygon(xyCen, xyArrPoly, TRUE);
}


long CADeckData::GetRebarsuAtPlanArr(vector<REBAR_PLAN> *pvPlan, long nCycle, long nShape, long nPos)
{
	long nRebarsu = 0;	
	for(long n=0; n<(long)pvPlan->size(); n++)
	{
		REBAR_PLAN *pPlan = &pvPlan->at(n);
		if(nCycle==pPlan->m_nCycle && nPos==pPlan->m_nPos && nShape==pPlan->m_nShape)
			nRebarsu++;
	}

	return nRebarsu;
}


double CADeckData::GetLengthBySlabEndRadius(double dStaNear, long nJongBasePosBase, long nJongBasePosTarget, double dBaseLen)
{
	if(!IsBridgeHasCurveLine())
		return dBaseLen;

	double dLeftRad = m_pDB->m_pLine->GetRadius_LineInfo(dStaNear) - GetDistByJongBase(JONGBASEPOS_SLABLEFT);
	double dCentRad = m_pDB->m_pLine->GetRadius_LineInfo(dStaNear) - GetDistByJongBase(JONGBASEPOS_SLABCENTER);
	double dRighRad = m_pDB->m_pLine->GetRadius_LineInfo(dStaNear) - GetDistByJongBase(JONGBASEPOS_SLABRIGHT);

	if(nJongBasePosBase==JONGBASEPOS_SLABLEFT)
	{
		if(nJongBasePosTarget==JONGBASEPOS_SLABCENTER)
			return (dBaseLen*dCentRad)/dLeftRad;
		else if(nJongBasePosTarget==JONGBASEPOS_SLABRIGHT)
			return (dBaseLen*dRighRad)/dLeftRad;

	}
	else if(nJongBasePosBase==JONGBASEPOS_SLABCENTER)
	{
		if(nJongBasePosTarget==JONGBASEPOS_SLABLEFT)
			return (dBaseLen*dLeftRad)/dCentRad;
		else if(nJongBasePosTarget==JONGBASEPOS_SLABRIGHT)
			return (dBaseLen*dRighRad)/dCentRad;

	}
	else if(nJongBasePosBase==JONGBASEPOS_SLABRIGHT)
	{
		if(nJongBasePosTarget==JONGBASEPOS_SLABLEFT)
			return (dBaseLen*dLeftRad)/dRighRad;
		else if(nJongBasePosTarget==JONGBASEPOS_SLABCENTER)
			return (dBaseLen*dCentRad)/dRighRad;

	}

	return 0;
}

