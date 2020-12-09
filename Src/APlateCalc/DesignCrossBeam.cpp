// DesignCrossBeam.cpp: implementation of the CDesignCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double GetBoltSize(CString strBoltSize)
{
	if(strBoltSize.Find("M")!=-1)
		strBoltSize.Delete(0);

	double dDia = atof(strBoltSize);
	return dDia;
}

CDesignCrossBeam::CDesignCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
	m_pEndCr			= NULL;
	m_pCenCr			= NULL;
	m_pInvVBracing		= NULL;
	m_pVBracing			= NULL;
}

CDesignCrossBeam::~CDesignCrossBeam()
{
	if(m_pEndCr)		delete m_pEndCr;
	if(m_pCenCr)		delete m_pCenCr;
	if(m_pInvVBracing)	delete m_pInvVBracing;
	if(m_pVBracing)		delete m_pVBracing;
}

void* CDesignCrossBeam::GetDesignCrossBeam()
{
	if(m_pBx)
	{
		CSection		*pSec	= m_pBx->GetSection();
		CPlateCrossBeam	*pC		= pSec->GetCrossBeam();
		CVBracing		*pBr	= pSec->GetVBracing();

		if(pSec->IsenType(SECTION_A))
		{
			// 단부 가로보
			if(pSec->GetenDetType()==SECDET_VBRACING)
			{
				m_pInvVBracing	= new CDesignInverseVBracing(m_pAPlateCalcStd, m_pBx);
				return m_pInvVBracing;
			}
			else
			{
				m_pEndCr	= new CDesignEndCrossBeam(m_pAPlateCalcStd, m_pBx);
				return m_pEndCr;
			}
		}
		else
		{
			// 중간 가로보
			if(pSec->GetenDetType()==SECDET_VBRACING)
			{
				m_pVBracing	= new CDesignVBracing(m_pAPlateCalcStd, m_pBx);
				return m_pVBracing;
			}
			else
			{
				m_pCenCr	= new CDesignCenterCrossBeam(m_pAPlateCalcStd, m_pBx);
				return m_pCenCr;
			}
		}
	}
	else
		return NULL;

	return NULL;
}

double CDesignCrossBeam::GetFlangeMoment()
{
	if(m_pEndCr)	return	m_pEndCr->m_strtFrameLoadComp.m_dMaxMoment;
	if(m_pCenCr)	return	max(fabs(m_pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), fabs(m_pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment));
	return 0;
}

double CDesignCrossBeam::GetFlangeStress(BOOL bUpper)
{
	if(m_pEndCr)	return	bUpper ? m_pEndCr->m_strtFrameLoadComp.m_dMaxFlangeUp : m_pEndCr->m_strtFrameLoadComp.m_dMaxFlangeLo;
	if(m_pCenCr)	return	bUpper ? m_pCenCr->m_strtSection.m_dFu : m_pCenCr->m_strtSection.m_dFl;
	return 0;
}

double CDesignCrossBeam::GetFlangeShear()
{
	if(m_pEndCr)	return	m_pEndCr->m_strtFrameLoadComp.m_dMaxShear;
	if(m_pCenCr)	return	m_pCenCr->m_strtSection.m_dShear;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 단부 가로보
CDesignEndCrossBeam::CDesignEndCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;

	SetDataInit();
	CSection	*pSection	= pBx->GetSection();
	if(pSection->GetenDetType() == SECDET_CROSSBEAM)
		SetGeneralType();	// 제작타입, H형강타입
}

CDesignEndCrossBeam::~CDesignEndCrossBeam()
{
	delete m_pJewonFlangeUp;
	delete m_pJewonFlangeLo;
	delete m_pJewonWeb;
	delete m_pFrameElementLoad;
	delete m_pVerStiff;
	delete m_pHorStiff;
}

void CDesignEndCrossBeam::SetDataInit()
{
	m_pJewonFlangeUp	= new CCrossBeamJewon(CP_F_U);
	m_pJewonFlangeLo	= new CCrossBeamJewon(CP_F_L);
	m_pJewonWeb			= new CCrossBeamJewon(CP_W);
	m_pFrameElementLoad	= new CFrameElementLoad();
	m_pVerStiff			= new CCrossBeamVerStiff(m_pBx, m_pAPlateCalcStd);
	m_pHorStiff			= new CCrossBeamHorStiff(m_pBx);
}

void CDesignEndCrossBeam::SetGeneralType()
{
	SetJewon();
	SetVerLoad();
	SetHorLoad();
	SetEarthQuakeLoad();
	SetElementLoad();
	SetFrameElementLoad();
	SetDesignSection();
	SetCompositeStress();
	SetMinThick();

	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);

	// 수직보강재
	CalcGen.GetMaterial(m_pBx, CP_V, strLeft, strRight);
	m_pVerStiff->m_dHeightWeb	= m_pJewonWeb->m_dHeight;
	m_pVerStiff->m_dThickWeb	= m_pJewonWeb->m_dWidth;
	m_pVerStiff->m_dAllowShear	= m_strtSection.m_dSheara;
	m_pVerStiff->m_dShear		= m_strtFrameLoadComp.m_dMaxShear;
	m_pVerStiff->m_strMaterial	= strLeft;
	m_pVerStiff->m_strRight		= strRight;
	m_pVerStiff->SetCrossBeamVerStiff();
	// 수평보강재
	CalcGen.GetMaterial(m_pBx, CP_H, strLeft, strRight);
	m_pHorStiff->m_strMaterial		= strLeft;
	m_pHorStiff->m_dVerStiffSpace	= m_pVerStiff->m_dVerStiffSpace;
	m_pHorStiff->m_dHeightWeb		= m_pJewonWeb->m_dHeight;
	m_pHorStiff->m_dThickWeb		= m_pJewonWeb->m_dWidth;
	m_pHorStiff->SetCrossBeamHorStiff();
}

void CDesignEndCrossBeam::SetMinThick()
{
	CSteel			*pSteelCtl = m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CString			strLeft		= _T("");
	CString			strRight	= _T("");
	
	CalcGen.GetMaterial(m_pBx, CP_W, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowMinThick(pSteel);
	double			dFca		= m_strtSection.m_dSigmata;
	double			dCeoff		= 0;
	double			dMinThick	= AllowMinThick.GetAllowStress(ALLOW_PLATEBRIDGE_WEBMINTHICK,	m_nQtyHorStiff);
	double			dRateSigma	= sqrt(dFca/m_strtFrameLoadComp.m_dMaxFlangeUp);

	if(dRateSigma>1.2)	dCeoff = 1.2;
	else				dCeoff = dRateSigma;
	m_strtMinThick.m_dThickWebReq		= m_pJewonWeb->m_dHeight/(dMinThick*dCeoff);
	m_strtMinThick.m_dMinThickFlangeUp	= (m_pJewonFlangeUp->m_dWidth-m_pJewonWeb->m_dWidth)/2.0/16.0;
	m_strtMinThick.m_dMinThickFlangeLo	= (m_pJewonFlangeLo->m_dWidth-m_pJewonWeb->m_dWidth)/2.0/16.0;
}

// 제원설정
void CDesignEndCrossBeam::SetJewon()
{
	CPlateCrossBeam	*pCr	= m_pBx->GetCrossBeam();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);

	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	CalcGen.GetMaterial(m_pBx, CP_F_U, strLeft, strRight);
	m_pJewonFlangeUp->m_strMaterial	= strLeft;
	CalcGen.GetMaterial(m_pBx, CP_F_L, strLeft, strRight);
	m_pJewonFlangeUp->m_strMaterial	= strLeft;
	CalcGen.GetMaterial(m_pBx, CP_W, strLeft, strRight);
	m_pJewonFlangeLo->m_strMaterial	= strLeft;
	m_pJewonFlangeUp->m_dWidth		= pCr->m_uM_CR*2;
	m_pJewonFlangeUp->m_dHeight		= pCr->m_T2_CR;
	m_pJewonFlangeLo->m_dWidth		= pCr->m_dM_CR*2;
	m_pJewonFlangeLo->m_dHeight		= pCr->m_T1_CR;
	m_pJewonWeb->m_dWidth			= pCr->m_T3_CR;
	m_pJewonWeb->m_dHeight			= pCr->m_H_CR;
	m_nQtyHorStiff					= pCr->m_nHStiffUppersu_CR;

	m_pJewonFlangeUp->m_dA			= m_pJewonFlangeUp->m_dWidth * m_pJewonFlangeUp->m_dHeight;
	m_pJewonFlangeUp->m_dY			= (m_pJewonFlangeUp->m_dHeight + m_pJewonWeb->m_dHeight)/2.0;
	m_pJewonFlangeUp->m_dAY			= m_pJewonFlangeUp->m_dA * m_pJewonFlangeUp->m_dY;
	m_pJewonFlangeUp->m_dAY2		= m_pJewonFlangeUp->m_dAY * m_pJewonFlangeUp->m_dY;
	m_pJewonFlangeUp->m_dIo			= m_pJewonFlangeUp->m_dWidth * pow(m_pJewonFlangeUp->m_dHeight,3) / 12.0;

	m_pJewonFlangeLo->m_dA			= m_pJewonFlangeLo->m_dWidth * m_pJewonFlangeLo->m_dHeight;
	m_pJewonFlangeLo->m_dY			= -(m_pJewonFlangeLo->m_dHeight + m_pJewonWeb->m_dHeight)/2.0;
	m_pJewonFlangeLo->m_dAY			= m_pJewonFlangeLo->m_dA * m_pJewonFlangeLo->m_dY;
	m_pJewonFlangeLo->m_dAY2		= m_pJewonFlangeLo->m_dAY * m_pJewonFlangeLo->m_dY;
	m_pJewonFlangeLo->m_dIo			= m_pJewonFlangeLo->m_dWidth * pow(m_pJewonFlangeLo->m_dHeight,3) / 12.0;

	m_pJewonWeb->m_dA				= m_pJewonWeb->m_dWidth * m_pJewonWeb->m_dHeight;
	m_pJewonWeb->m_dY				= 0;
	m_pJewonWeb->m_dAY				= m_pJewonWeb->m_dA * m_pJewonWeb->m_dY;
	m_pJewonWeb->m_dAY2				= m_pJewonWeb->m_dAY * m_pJewonWeb->m_dY;
	m_pJewonWeb->m_dIo				= m_pJewonWeb->m_dWidth * pow(m_pJewonWeb->m_dHeight,3) / 12.0;
}

// 연직하중
void CDesignEndCrossBeam::SetVerLoad()
{
	CPlateBridgeApp	*pDB		= m_pAPlateCalcStd->GetBridge();
	CPlateGirderApp	*pGir		= m_pBx->GetGirder();
	CCalcData		*pCalcData	= m_pAPlateCalcStd->GetCalcData();

	double	dUnitPave	= pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt;
	double	dUnitConc	= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;
	double	dThickSlab	= pGir->GetThickSlab(m_pBx, 0);

	m_strtVerLoad.m_dGirderSpace				= pGir->GetDisGirderToGirder(m_pBx);// 주형간격
	m_strtVerLoad.m_dDistVerBracingToEndSlab	= pDB->m_dLengthGirderShoeStt;		// 수직브레이싱에서 상판단부까지 거리
	m_strtVerLoad.m_dLoadPave					= pDB->m_dThickPave * dUnitPave;	// 포장하중
	m_strtVerLoad.m_dLoadSlab					= dThickSlab * dUnitConc;			// 상판하중
	m_strtVerLoad.m_dQ1							= m_strtVerLoad.m_dDistVerBracingToEndSlab*(m_strtVerLoad.m_dLoadPave+m_strtVerLoad.m_dLoadSlab);
	m_strtVerLoad.m_dQ2							= m_strtVerLoad.m_dGirderSpace/2*(m_strtVerLoad.m_dLoadPave+m_strtVerLoad.m_dLoadSlab);
	m_strtVerLoad.m_dCollisionCoeff				= 15.0/(40.0+toM(m_strtVerLoad.m_dGirderSpace));
	m_strtVerLoad.m_dCollisionCoeffUse			= m_strtVerLoad.m_dCollisionCoeff>0.3 ? 0.3 : m_strtVerLoad.m_dCollisionCoeff;
}

// 수평하중
void CDesignEndCrossBeam::SetHorLoad()
{
	CPlateBridgeApp	*pDB		= m_pAPlateCalcStd->GetBridge();
	CPlateGirderApp	*pGir		= m_pBx->GetGirder();
	CCalcData		*pCalcData	= m_pAPlateCalcStd->GetCalcData();
	CCentSeparation	*pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);

	double	dStation			= m_pBx->GetStation();
	long	nSpanIdx			= pGir->GetNumberJiganByStation(dStation);
	double	dHeightGirder		= m_pBx->GetHeightGirderByBx();
	double	dSlabThick			= pDB->m_dThickSlabBase;
	double	dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double	dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double	dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double	dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;

	m_strtHorLoad.m_dWidthBridge		= pDB->m_dBridgeWidth;
	m_strtHorLoad.m_dHeightBridge		= dHeightGirder+dSlabThick+dHeightGuardFence+dHeightSoundProof+dHeightHandRail;
	m_strtHorLoad.m_dRateWidthHeight	= m_strtHorLoad.m_dWidthBridge/m_strtHorLoad.m_dHeightBridge;
	if(m_strtHorLoad.m_dRateWidthHeight>=1 && m_strtHorLoad.m_dRateWidthHeight<8)
		m_strtHorLoad.m_dWw	= (4.0-0.2*m_strtHorLoad.m_dRateWidthHeight)*m_strtHorLoad.m_dHeightBridge/1000.0;
	else if(m_strtHorLoad.m_dRateWidthHeight>=8.0)
		m_strtHorLoad.m_dWw = 2.4*m_strtHorLoad.m_dHeightBridge/1000.0;
	else
		m_strtHorLoad.m_dWw = 0;

	m_strtHorLoad.m_dWwUse			= max(m_strtHorLoad.m_dWw, 6.0);
	m_strtHorLoad.m_dLengthSpan		= pDB->m_dLengthJigan[nSpanIdx];
	if(pDB->GetGirdersu()>=3)
		m_strtHorLoad.m_dHw			= 0.5*m_strtHorLoad.m_dWw*m_strtHorLoad.m_dLengthSpan;
	else
		m_strtHorLoad.m_dHw			= m_strtHorLoad.m_dWw*m_strtHorLoad.m_dLengthSpan;

}

void CDesignEndCrossBeam::SetEarthQuakeLoad()
{
	CPlateBridgeApp		*pDB			= m_pAPlateCalcStd->GetBridge();
	CGlobarOption		*pGlobarOption	= m_pAPlateCalcStd->m_pDataManage->GetGlobalOption();
	CFemStressCalc		StressCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFemModelingCalc	ModelCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFEMManage		*pFEMManage		= NULL;
	CResultArray		*pResult		= NULL;
	CJoint				*pJ				= NULL;
	CJointResult		*pJResult		= NULL;

	double	dRiskCoefficient	= pGlobarOption->GetRiskCoefficient();
	double	dZoneCoefficient	= pGlobarOption->GetZoneCoefficient();
	long	nCurJijum	= m_pBx->GetNumberJijum();

	//지진하중 시 지점 반력 합계 수정...070226...KB 구경...SIM 수정...
	m_strtEarthQuakeLoad.m_dReaction = 0;
	pFEMManage	= m_pAPlateCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	pResult		= m_pAPlateCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	int nJ = 0;
	for(nJ = 0; nJ < pFEMManage->GetJointSize(); nJ++)
	{
		pJ			= pFEMManage->GetJoint(nJ);
		pJResult	= pResult->m_JointResultArr.GetAt(nJ);
		long	nJijum = m_pAPlateCalcStd->GetBxByJoint(pJ)->GetNumberJijum();
		if(!(pJ->m_UZ))	continue;
		if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && nJijum == nCurJijum)		
			m_strtEarthQuakeLoad.m_dReaction += frkN(pJResult->m_vPreDeadSteel[JOINT_REACTION].z+pJResult->m_vPreDeadSlab[JOINT_REACTION].z);		
	}

	pFEMManage	= m_pAPlateCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
	pResult		= m_pAPlateCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
	for(nJ = 0; nJ < pFEMManage->GetJointSize(); nJ++)
	{
		pJ			= pFEMManage->GetJoint(nJ);
		pJResult	= pResult->m_JointResultArr.GetAt(nJ);
		long	nJijum = m_pAPlateCalcStd->GetBxByJoint(pJ)->GetNumberJijum();
		if(!(pJ->m_UZ))	continue;
		if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && nJijum == nCurJijum)
			m_strtEarthQuakeLoad.m_dReaction += frkN(pJResult->m_vPostDead[JOINT_REACTION].z);
	}

	m_strtEarthQuakeLoad.m_dAcceleration		= dRiskCoefficient*dZoneCoefficient;
	m_strtEarthQuakeLoad.m_dTotEarthQuakeLoad	= m_strtEarthQuakeLoad.m_dAcceleration*m_strtEarthQuakeLoad.m_dReaction;
	m_strtEarthQuakeLoad.m_dEarthQuakeLoad		= 1/(pDB->GetGirdersu()-1)*m_strtEarthQuakeLoad.m_dTotEarthQuakeLoad;
}

void CDesignEndCrossBeam::SetElementLoad()
{
	CCalcData		*pCalcData	= m_pAPlateCalcStd->GetCalcData();

	double	dPr	= pCalcData->DESIGN_CONDITION.m_dPr;

	m_strtElementLoad.m_dMd	= 1.0/8.0*m_strtVerLoad.m_dQ1*pow(m_strtVerLoad.m_dGirderSpace,2)+1.0/12.0*m_strtVerLoad.m_dQ2*pow(m_strtVerLoad.m_dGirderSpace,2);
	if(m_strtVerLoad.m_dGirderSpace>2400.0)
		m_strtElementLoad.m_dMl	= (1.0/4.0*dPr*m_strtVerLoad.m_dGirderSpace + 1.0/2.0*dPr*(m_strtVerLoad.m_dGirderSpace/2.0-1200.0))*(1.0+m_strtVerLoad.m_dCollisionCoeffUse);
	else
		m_strtElementLoad.m_dMl	= 1.0/4.0*dPr*m_strtVerLoad.m_dGirderSpace*(1.0+m_strtVerLoad.m_dCollisionCoeffUse);
	m_strtElementLoad.m_dSd		= 1.0/2.0*m_strtVerLoad.m_dQ1*m_strtVerLoad.m_dGirderSpace + 1.0/4.0*m_strtVerLoad.m_dQ2*m_strtVerLoad.m_dGirderSpace;
	m_strtElementLoad.m_dSl		= dPr*(1+(m_strtVerLoad.m_dGirderSpace-1200.0)/m_strtVerLoad.m_dGirderSpace)*(1.0+m_strtVerLoad.m_dCollisionCoeffUse);
	m_strtElementLoad.m_dHw		= m_strtHorLoad.m_dHw;
	m_strtElementLoad.m_dHe		= m_strtEarthQuakeLoad.m_dEarthQuakeLoad;

}

void CDesignEndCrossBeam::SetFrameElementLoad()
{
	CFemModelingCalc	ModelCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFemStressCalc		StressCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);

	long nElement = (long)ModelCalc.GetElemByCrossBx(m_pBx);

	if(nElement < 0)
		return;

	m_pFrameElementLoad->SetFrameLoad(&StressCalc, nElement);
}

void CDesignEndCrossBeam::SetDesignSection()
{
	CSteel			*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);

	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	CalcGen.GetMaterial(m_pBx, CP_F_U, strLeft, strRight);
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	
	CHGAllowStress	AllowBuckling(pSteel);
	CHGAllowStress	AllowBendingComp(pSteel);
	CHGAllowStress	AllowTension(pSteel);
	CHGAllowStress	AllowShear(pSteel);

	double	dTotA				= GetTotSectionArea();
	double	dTotAY				= GetTotSectionAY();
	double	dTotAY2				= GetTotSectionAY2();
	double	dTotIo				= GetTotSectionIo();
	
	double	dThick				= m_pJewonFlangeUp->m_dHeight;
	double	dWidth				= (m_pJewonFlangeUp->m_dWidth-m_pJewonWeb->m_dWidth)/2;
	double	dAw					= m_pJewonWeb->m_dA;
	double	dAc					= m_pJewonFlangeUp->m_dA;
	double	dDistFlangeFixed	= m_strtVerLoad.m_dGirderSpace;
	double	dWidthFlange		= m_pJewonFlangeUp->m_dWidth;

	AllowBuckling.SetAllowBuckling(dThick, dWidth);
	AllowBendingComp.SetAllowBendingStress(dAw, dAc, dDistFlangeFixed, dWidthFlange);
	

	m_strtSection.m_dDelta		= dTotAY/dTotA;
	m_strtSection.m_dI			= dTotIo + dTotAY2 - dTotA*m_strtSection.m_dDelta;
	m_strtSection.m_dYsu		= m_pJewonWeb->m_dHeight/2 + m_pJewonFlangeUp->m_dHeight-m_strtSection.m_dDelta;
	m_strtSection.m_dYsl		= m_pJewonWeb->m_dHeight/2 + m_pJewonFlangeLo->m_dHeight-m_strtSection.m_dDelta;
	m_strtSection.m_dSigmaca	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY,	m_pJewonFlangeUp->m_dWidth);
	m_strtSection.m_dFca		= min(AllowBendingComp.GetAllowStress(ALLOW_BENDINGCOMP_CASE2,	m_pJewonFlangeUp->m_dWidth), m_strtSection.m_dSigmaca);
	m_strtSection.m_dSigmata	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS,	m_pJewonFlangeUp->m_dWidth);
	m_strtSection.m_dSheara		= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS,	m_pJewonFlangeUp->m_dWidth);
}

void CDesignEndCrossBeam::SetCompositeStress()
{
	double	dYsu	= m_strtSection.m_dYsu;
	double	dYsl	= m_strtSection.m_dYsl;
	m_strtFrameLoadComp.m_dMaxMoment	= 0;
	m_strtFrameLoadComp.m_dMaxShear		= 0;
	m_strtFrameLoadComp.m_dMaxFlangeUp	= 0;
	m_strtFrameLoadComp.m_dMaxFlangeLo	= 0;
	double dAsSum	= GetTotSectionArea();

	m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]			= m_strtElementLoad.m_dMd;
	m_strtFrameLoadComp.m_dDeadLoad[COMP_SHEAR]				= m_strtElementLoad.m_dSd;
	m_strtFrameLoadComp.m_dDeadLoad[COMP_HORIZONTAL]		= 0;		
	m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGEUP]			= m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]*dYsu/m_strtSection.m_dI+m_strtFrameLoadComp.m_dDeadLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGELO]			= m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]*dYsl/m_strtSection.m_dI+m_strtFrameLoadComp.m_dDeadLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dDeadLoad[COMP_ALLOWSHEAR]		= m_strtFrameLoadComp.m_dDeadLoad[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dDeadLoad[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGELO]);

	m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT]			= m_strtElementLoad.m_dMl;
	m_strtFrameLoadComp.m_dLiveLoad[COMP_SHEAR]				= m_strtElementLoad.m_dSl;
	m_strtFrameLoadComp.m_dLiveLoad[COMP_HORIZONTAL]		= 0;
	m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGEUP]			= m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT]*dYsu/m_strtSection.m_dI+m_strtFrameLoadComp.m_dLiveLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGELO]			= m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT]*dYsl/m_strtSection.m_dI+m_strtFrameLoadComp.m_dLiveLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dLiveLoad[COMP_ALLOWSHEAR]		= m_strtFrameLoadComp.m_dLiveLoad[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dLiveLoad[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGELO]);

	m_strtFrameLoadComp.m_dWindLoad[COMP_MOMENT]			= 0;
	m_strtFrameLoadComp.m_dWindLoad[COMP_SHEAR]				= 0;
	m_strtFrameLoadComp.m_dWindLoad[COMP_HORIZONTAL]		= m_strtElementLoad.m_dHw;
	m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGEUP]			= m_strtFrameLoadComp.m_dWindLoad[COMP_MOMENT]*dYsu/m_strtSection.m_dI+m_strtFrameLoadComp.m_dWindLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGELO]			= m_strtFrameLoadComp.m_dWindLoad[COMP_MOMENT]*dYsl/m_strtSection.m_dI+m_strtFrameLoadComp.m_dWindLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dWindLoad[COMP_ALLOWSHEAR]		= m_strtFrameLoadComp.m_dWindLoad[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dWindLoad[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dWindLoad[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGELO]);

	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT]		= 0;
	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_SHEAR]		= 0;
	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_HORIZONTAL]	= m_strtElementLoad.m_dHe;
	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGEUP]	= m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT]*dYsu/m_strtSection.m_dI+m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGELO]	= m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT]*dYsl/m_strtSection.m_dI+m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_ALLOWSHEAR]	= m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGELO]);
	
	m_strtFrameLoadComp.m_dCompGen[COMP_MOMENT]				= m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]+m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT];
	m_strtFrameLoadComp.m_dCompGen[COMP_SHEAR]				= m_strtFrameLoadComp.m_dDeadLoad[COMP_SHEAR]+m_strtFrameLoadComp.m_dLiveLoad[COMP_SHEAR];
	m_strtFrameLoadComp.m_dCompGen[COMP_HORIZONTAL]			= 0;
	m_strtFrameLoadComp.m_dCompGen[COMP_FLANGEUP]			= m_strtFrameLoadComp.m_dCompGen[COMP_MOMENT]*dYsu/m_strtSection.m_dI+m_strtFrameLoadComp.m_dCompGen[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dCompGen[COMP_FLANGELO]			= m_strtFrameLoadComp.m_dCompGen[COMP_MOMENT]*dYsl/m_strtSection.m_dI+m_strtFrameLoadComp.m_dCompGen[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dCompGen[COMP_ALLOWSHEAR]			= m_strtFrameLoadComp.m_dCompGen[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dCompGen[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dCompGen[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dCompGen[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dCompGen[COMP_FLANGELO]);

	m_strtFrameLoadComp.m_dCompWind[COMP_MOMENT]			= (m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]+m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT]+m_strtFrameLoadComp.m_dWindLoad[COMP_MOMENT])/1.25;
	m_strtFrameLoadComp.m_dCompWind[COMP_SHEAR]				= (m_strtFrameLoadComp.m_dDeadLoad[COMP_SHEAR]+m_strtFrameLoadComp.m_dLiveLoad[COMP_SHEAR]+m_strtFrameLoadComp.m_dWindLoad[COMP_SHEAR])/1.25;
	m_strtFrameLoadComp.m_dCompWind[COMP_HORIZONTAL]		= (m_strtFrameLoadComp.m_dDeadLoad[COMP_HORIZONTAL]+m_strtFrameLoadComp.m_dLiveLoad[COMP_HORIZONTAL]+m_strtFrameLoadComp.m_dWindLoad[COMP_HORIZONTAL])/1.25;
	m_strtFrameLoadComp.m_dCompWind[COMP_FLANGEUP]			= m_strtFrameLoadComp.m_dCompWind[COMP_MOMENT]*dYsu/m_strtSection.m_dI+m_strtFrameLoadComp.m_dCompWind[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dCompWind[COMP_FLANGELO]			= m_strtFrameLoadComp.m_dCompWind[COMP_MOMENT]*dYsl/m_strtSection.m_dI+m_strtFrameLoadComp.m_dCompWind[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dCompWind[COMP_ALLOWSHEAR]		= m_strtFrameLoadComp.m_dCompWind[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dCompWind[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dCompWind[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dCompWind[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dCompWind[COMP_FLANGELO]);
	
	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_MOMENT]		= (m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]+m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT])/1.5;
	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_SHEAR]		= (m_strtFrameLoadComp.m_dDeadLoad[COMP_SHEAR]+m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT])/1.5;
	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_HORIZONTAL]	= (m_strtFrameLoadComp.m_dDeadLoad[COMP_HORIZONTAL]+m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT])/1.5;
	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGEUP]	= m_strtFrameLoadComp.m_dCompEarthQuake[COMP_MOMENT]*dYsu/m_strtSection.m_dI+m_strtFrameLoadComp.m_dCompEarthQuake[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGELO]	= m_strtFrameLoadComp.m_dCompEarthQuake[COMP_MOMENT]*dYsl/m_strtSection.m_dI+m_strtFrameLoadComp.m_dCompEarthQuake[COMP_HORIZONTAL]/dAsSum;
	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_ALLOWSHEAR]	= m_strtFrameLoadComp.m_dCompEarthQuake[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dCompEarthQuake[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dCompEarthQuake[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGELO]);
	
	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_MOMENT]		= max(fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment));
	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_SHEAR]		= max(fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMinShear));
	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_HORIZONTAL]	= max(fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMaxAxial), fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMinAxial));
	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGEUP]		= m_strtFrameLoadComp.m_dCompFrameLoad[COMP_MOMENT]*dYsu/m_strtSection.m_dI+fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMinAxial)/dAsSum;
	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGELO]		= m_strtFrameLoadComp.m_dCompFrameLoad[COMP_MOMENT]*dYsl/m_strtSection.m_dI+fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMaxAxial)/dAsSum;
	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_ALLOWSHEAR]	= m_strtFrameLoadComp.m_dCompFrameLoad[COMP_SHEAR]/m_pJewonWeb->m_dA;

	m_strtFrameLoadComp.m_dMaxMoment	= max(m_strtFrameLoadComp.m_dMaxMoment,		m_strtFrameLoadComp.m_dCompFrameLoad[COMP_MOMENT]);
	m_strtFrameLoadComp.m_dMaxShear		= max(m_strtFrameLoadComp.m_dMaxShear,		m_strtFrameLoadComp.m_dCompFrameLoad[COMP_ALLOWSHEAR]);
	m_strtFrameLoadComp.m_dMaxFlangeUp	= max(m_strtFrameLoadComp.m_dMaxFlangeUp,	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGEUP]);
	m_strtFrameLoadComp.m_dMaxFlangeLo	= max(m_strtFrameLoadComp.m_dMaxFlangeLo,	m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGELO]);
}

//////////////////////////////////////////////////////////////////////////
// 중간 가로보
CDesignCenterCrossBeam::CDesignCenterCrossBeam(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
	SetDataInit();
	CSection	*pSection	= pBx->GetSection();
	if(pSection->GetenDetType() == SECDET_CROSSBEAM)
		SetGeneralType();	// 제작타입, H형강타입
}

CDesignCenterCrossBeam::~CDesignCenterCrossBeam()
{
	delete	m_pJewonFlangeUp;
	delete	m_pJewonFlangeLo;
	delete	m_pJewonWeb;
	delete	m_pFrameElementLoad;
	delete	m_pVerStiff;
	delete	m_pHorStiff;
}

void CDesignCenterCrossBeam::SetDataInit()
{
	m_pJewonFlangeUp	= new CCrossBeamJewon(CG_F_U);
	m_pJewonFlangeLo	= new CCrossBeamJewon(CG_F_L);
	m_pJewonWeb			= new CCrossBeamJewon(CG_W);
	m_pFrameElementLoad	= new CFrameElementLoad();
	m_pVerStiff			= new CCrossBeamVerStiff(m_pBx, m_pAPlateCalcStd);
	m_pHorStiff			= new CCrossBeamHorStiff(m_pBx);
}

void CDesignCenterCrossBeam::SetGeneralType()
{
	SetJewon();
	SetFrameElementLoad();
	SetDesignSection();
	SetMinThick();

	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);

	// 수직보강재
	CalcGen.GetMaterial(m_pBx, CG_V, strLeft, strRight);
	m_pVerStiff->m_dHeightWeb	= m_pJewonWeb->m_dHeight;
	m_pVerStiff->m_dThickWeb	= m_pJewonWeb->m_dWidth;
	m_pVerStiff->m_dAllowShear	= m_strtSection.m_dSheara;
	m_pVerStiff->m_dShear		= m_strtSection.m_dShear;
	m_pVerStiff->m_strMaterial	= strLeft;
	m_pVerStiff->m_strRight		= strRight;
	m_pVerStiff->SetCrossBeamVerStiff();
	// 수평보강재
	CalcGen.GetMaterial(m_pBx, CG_H, strLeft, strRight);
	m_pHorStiff->m_strMaterial		= strLeft;
	m_pHorStiff->m_dVerStiffSpace	= m_pVerStiff->m_dVerStiffSpace;
	m_pHorStiff->m_dHeightWeb		= m_pJewonWeb->m_dHeight;
	m_pHorStiff->m_dThickWeb		= m_pJewonWeb->m_dWidth;
	m_pHorStiff->SetCrossBeamHorStiff();
}

void CDesignCenterCrossBeam::SetJewon()
{
	CPlateCrossBeam	*pCr	= m_pBx->GetCrossBeam();
	CPlateGirderApp	*pGir	= m_pBx->GetGirder();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);

	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	CalcGen.GetMaterial(m_pBx, CG_F_U, strLeft, strRight);
	m_pJewonFlangeUp->m_strMaterial	= strLeft;
	CalcGen.GetMaterial(m_pBx, CG_F_L, strLeft, strRight);
	m_pJewonFlangeUp->m_strMaterial	= strLeft;
	CalcGen.GetMaterial(m_pBx, CG_W, strLeft, strRight);
	m_pJewonFlangeLo->m_strMaterial	= strLeft;
	m_pJewonFlangeUp->m_dWidth		= pCr->m_uM_CR*2;
	m_pJewonFlangeUp->m_dHeight		= pCr->m_T2_CR;
	m_pJewonFlangeLo->m_dWidth		= pCr->m_dM_CR*2;
	m_pJewonFlangeLo->m_dHeight		= pCr->m_T1_CR;
	m_pJewonWeb->m_dWidth			= pCr->m_T3_CR;
	m_pJewonWeb->m_dHeight			= pCr->m_H_CR;
	m_nQtyHorStiff					= pCr->m_nHStiffUppersu_CR;

	m_pJewonFlangeUp->m_dA			= m_pJewonFlangeUp->m_dWidth * m_pJewonFlangeUp->m_dHeight;
	m_pJewonFlangeUp->m_dY			= (m_pJewonFlangeUp->m_dHeight + m_pJewonWeb->m_dHeight)/2.0;
	m_pJewonFlangeUp->m_dAY			= m_pJewonFlangeUp->m_dA * m_pJewonFlangeUp->m_dY;
	m_pJewonFlangeUp->m_dAY2		= m_pJewonFlangeUp->m_dAY * m_pJewonFlangeUp->m_dY;
	m_pJewonFlangeUp->m_dIo			= m_pJewonFlangeUp->m_dWidth * pow(m_pJewonFlangeUp->m_dHeight,3) / 12.0;

	m_pJewonFlangeLo->m_dA			= m_pJewonFlangeLo->m_dWidth * m_pJewonFlangeLo->m_dHeight;
	m_pJewonFlangeLo->m_dY			= -(m_pJewonFlangeLo->m_dHeight + m_pJewonWeb->m_dHeight)/2.0;
	m_pJewonFlangeLo->m_dAY			= m_pJewonFlangeLo->m_dA * m_pJewonFlangeLo->m_dY;
	m_pJewonFlangeLo->m_dAY2		= m_pJewonFlangeLo->m_dAY * m_pJewonFlangeLo->m_dY;
	m_pJewonFlangeLo->m_dIo			= m_pJewonFlangeLo->m_dWidth * pow(m_pJewonFlangeLo->m_dHeight,3) / 12.0;

	m_pJewonWeb->m_dA				= m_pJewonWeb->m_dWidth * m_pJewonWeb->m_dHeight;
	m_pJewonWeb->m_dY				= 0;
	m_pJewonWeb->m_dAY				= m_pJewonWeb->m_dA * m_pJewonWeb->m_dY;
	m_pJewonWeb->m_dAY2				= m_pJewonWeb->m_dAY * m_pJewonWeb->m_dY;
	m_pJewonWeb->m_dIo				= m_pJewonWeb->m_dWidth * pow(m_pJewonWeb->m_dHeight,3) / 12.0;
	m_dGirderSpace					= pGir->GetDisGirderToGirder(m_pBx);
}

void CDesignCenterCrossBeam::SetMinThick()
{
	CSteel			*pSteelCtl = m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	CalcGen.GetMaterial(m_pBx, CG_W, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowMinThick(pSteel);

	
	double	dFca		= m_strtSection.m_dSigmata;
	double	dCeoff		= 0;
	double	dMinThick	= AllowMinThick.GetAllowStress(ALLOW_PLATEBRIDGE_WEBMINTHICK, m_nQtyHorStiff);
	double	dRateSigma	= sqrt(dFca/m_strtSection.m_dFu);
	if(dRateSigma>1.2)	dCeoff = 1.2;
	else				dCeoff = dRateSigma;
	m_strtMinThick.m_dThickWebReq		= m_pJewonWeb->m_dHeight/(dMinThick*dCeoff);
	m_strtMinThick.m_dMinThickFlangeUp	= (m_pJewonFlangeUp->m_dWidth-m_pJewonWeb->m_dWidth)/2.0/16.0;
	m_strtMinThick.m_dMinThickFlangeLo	= (m_pJewonFlangeLo->m_dWidth-m_pJewonWeb->m_dWidth)/2.0/16.0;
}

void CDesignCenterCrossBeam::SetFrameElementLoad()
{
	CFemModelingCalc	ModelCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFemStressCalc		StressCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);

	long nElement = (long)ModelCalc.GetElemByCrossBx(m_pBx);
	m_pFrameElementLoad->SetFrameLoad(&StressCalc, nElement);
}

void CDesignCenterCrossBeam::SetDesignSection()
{
	CSteel			*pSteelCtl = m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);

	CString	strLeft				= _T("");
	CString	strRight			= _T("");

	CalcGen.GetMaterial(m_pBx, CG_F_U, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBuckling(pSteel);
	CHGAllowStress	AllowBendingComp(pSteel);
	CHGAllowStress	AllowTension(pSteel);
	CHGAllowStress	AllowShear(pSteel);

	double	dTotA				= GetTotSectionArea();
	double	dTotAY				= GetTotSectionAY();
	double	dTotAY2				= GetTotSectionAY2();
	double	dTotIo				= GetTotSectionIo();
	
	double	dThick				= m_pJewonFlangeUp->m_dHeight;
	double	dWidth				= (m_pJewonFlangeUp->m_dWidth-m_pJewonWeb->m_dWidth)/2;
	double	dAw					= m_pJewonWeb->m_dA;
	double	dAc					= m_pJewonFlangeUp->m_dA;
	double	dDistFlangeFixed	= m_dGirderSpace;
	double	dWidthFlange		= m_pJewonFlangeUp->m_dWidth;
	double	dMoment				= max(fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment));
	double	dShear				= max(fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), fabs(m_pFrameElementLoad->m_strtTotLoad.m_dMinShear));

	AllowBuckling.SetAllowBuckling(dThick, dWidth);
	AllowBendingComp.SetAllowBendingStress(dAw, dAc, dDistFlangeFixed, dWidthFlange);
	

	m_strtSection.m_dDelta		= dTotAY/dTotA;
	m_strtSection.m_dI			= dTotIo + dTotAY2 - dTotA*m_strtSection.m_dDelta;
	m_strtSection.m_dYsu		= m_pJewonWeb->m_dHeight/2 + m_pJewonFlangeUp->m_dHeight-m_strtSection.m_dDelta;
	m_strtSection.m_dYsl		= m_pJewonWeb->m_dHeight/2 + m_pJewonFlangeLo->m_dHeight-m_strtSection.m_dDelta;
	m_strtSection.m_dSigmaca	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY,	m_pJewonFlangeUp->m_dWidth);
	m_strtSection.m_dFca		= min(AllowBendingComp.GetAllowStress(ALLOW_BENDINGCOMP_CASE2,	m_pJewonFlangeUp->m_dWidth), m_strtSection.m_dSigmaca);
	m_strtSection.m_dSigmata	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS,	m_pJewonFlangeUp->m_dWidth);
	m_strtSection.m_dSheara		= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS,	m_pJewonFlangeUp->m_dWidth);
	m_strtSection.m_dFu			= dMoment * m_strtSection.m_dYsu/m_strtSection.m_dI + m_pFrameElementLoad->m_strtTotLoad.m_dMinAxial/dTotA;
	m_strtSection.m_dFl			= dMoment * m_strtSection.m_dYsl/m_strtSection.m_dI + m_pFrameElementLoad->m_strtTotLoad.m_dMaxAxial/dTotA;
	m_strtSection.m_dShear		= dShear / dTotA;
	if(fabs(m_strtSection.m_dFu/m_strtSection.m_dFca)>fabs(m_strtSection.m_dFl/m_strtSection.m_dSigmata))
		m_strtSection.m_dCompStress = pow(fabs(m_strtSection.m_dFu)/m_strtSection.m_dFca,2) - pow(m_strtSection.m_dShear/m_strtSection.m_dSheara,2);
	else
		m_strtSection.m_dCompStress = pow(fabs(m_strtSection.m_dFl)/m_strtSection.m_dSigmata,2) + pow(m_strtSection.m_dShear/m_strtSection.m_dSheara,2);
}

//////////////////////////////////////////////////////////////////////////
// 가로보 제원(제작타입, H형강타입)
CCrossBeamJewon::CCrossBeamJewon(long nMaterial)
{
	m_strMaterial	= _T("");
	m_nMaterial		= nMaterial;
	m_dWidth		= 0;
	m_dHeight		= 0;
	m_dA			= 0;
	m_dY			= 0;
	m_dAY			= 0;
	m_dAY2			= 0;
	m_dIo			= 0;
}

CCrossBeamJewon::~CCrossBeamJewon()
{

}
//////////////////////////////////////////////////////////////////////////
// 수직브레이싱 역V타입.V타입 제원 및 두께 검토
CBracingJewon::CBracingJewon()
{
	m_dWidth	= 0;
	m_dHeight	= 0;
	m_dThick1	= 0;
	m_dThick2	= 0;
	m_dArea		= 0;
	m_dGammaX	= 0;
	m_dGammaY	= 0;
	m_dGammaMin	= 0;
	m_dLength	= 0;
}
CBracingJewon::~CBracingJewon()
{
}

//////////////////////////////////////////////////////////////////////////
// Frame 해석결과
CFrameElementLoad::CFrameElementLoad()
{
	m_strtMoment.m_dPreDeadLoad		= 0;
	m_strtMoment.m_dPostDeadLoad	= 0;
	m_strtMoment.m_dMaxLiveLoad		= 0;
	m_strtMoment.m_dMinLiveLoad		= 0;
	m_strtMoment.m_dMaxSettlement	= 0;
	m_strtMoment.m_dMinSettlement	= 0;

	m_strtShear.m_dPreDeadLoad		= 0;
	m_strtShear.m_dPostDeadLoad		= 0;
	m_strtShear.m_dMaxLiveLoad		= 0;
	m_strtShear.m_dMinLiveLoad		= 0;
	m_strtShear.m_dMaxSettlement	= 0;
	m_strtShear.m_dMinSettlement	= 0;

	m_strtAxial.m_dPreDeadLoad		= 0;
	m_strtAxial.m_dPostDeadLoad		= 0;
	m_strtAxial.m_dMaxLiveLoad		= 0;
	m_strtAxial.m_dMinLiveLoad		= 0;
	m_strtAxial.m_dMaxSettlement	= 0;
	m_strtAxial.m_dMinSettlement	= 0;

	m_strtTotLoad.m_dMaxMoment		= 0;
	m_strtTotLoad.m_dMinMoment		= 0;
	m_strtTotLoad.m_dMaxShear		= 0;
	m_strtTotLoad.m_dMinShear		= 0;
	m_strtTotLoad.m_dMaxAxial		= 0;
	m_strtTotLoad.m_dMinAxial		= 0;
}

CFrameElementLoad::~CFrameElementLoad()
{
}

void CFrameElementLoad::SetFrameLoad(CFemStressCalc *pFemStressCalc, long nElement)
{
	if(nElement < 0) return;

	m_strtMoment.m_dPreDeadLoad		= frkNM(pFemStressCalc->GetOneEleMaxForce(ELE_MOMENT3, LC_PREV_S | LC_PREV_C, nElement));
	m_strtMoment.m_dPostDeadLoad	= frkNM(pFemStressCalc->GetOneEleMaxForce(ELE_MOMENT3, LC_NEXT, nElement));
	m_strtMoment.m_dMaxLiveLoad		= frkNM(pFemStressCalc->GetOneEleMaxForce(ELE_MOMENT3, LC_LIVE_MAX, nElement));
	m_strtMoment.m_dMinLiveLoad		= frkNM(pFemStressCalc->GetOneEleMaxForce(ELE_MOMENT3, LC_LIVE_MIN, nElement));
	m_strtMoment.m_dMaxSettlement	= frkNM(pFemStressCalc->GetOneEleMaxForce(ELE_MOMENT3, LC_DISP_MAX, nElement));
	m_strtMoment.m_dMinSettlement	= frkNM(pFemStressCalc->GetOneEleMaxForce(ELE_MOMENT3, LC_DISP_MIN, nElement));

	m_strtShear.m_dPreDeadLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_SHEAR_2, LC_PREV_S | LC_PREV_C, nElement));
	m_strtShear.m_dPostDeadLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_SHEAR_2, LC_NEXT, nElement));
	m_strtShear.m_dMaxLiveLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_SHEAR_2, LC_LIVE_MAX, nElement));
	m_strtShear.m_dMinLiveLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_SHEAR_2, LC_LIVE_MIN, nElement));
	m_strtShear.m_dMaxSettlement	= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_SHEAR_2, LC_DISP_MAX, nElement));
	m_strtShear.m_dMinSettlement	= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_SHEAR_2, LC_DISP_MIN, nElement));

	m_strtAxial.m_dPreDeadLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_AXFORCE, LC_PREV_S | LC_PREV_C, nElement));
	m_strtAxial.m_dPostDeadLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_AXFORCE, LC_NEXT, nElement));
	m_strtAxial.m_dMaxLiveLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_AXFORCE, LC_LIVE_MAX, nElement));
	m_strtAxial.m_dMinLiveLoad		= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_AXFORCE, LC_LIVE_MIN, nElement));
	m_strtAxial.m_dMaxSettlement	= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_AXFORCE, LC_DISP_MAX, nElement));
	m_strtAxial.m_dMinSettlement	= frkN(pFemStressCalc->GetOneEleMaxForce(ELE_AXFORCE, LC_DISP_MIN, nElement));

	m_strtTotLoad.m_dMaxMoment		= m_strtMoment.m_dPreDeadLoad+m_strtMoment.m_dPostDeadLoad+m_strtMoment.m_dMaxLiveLoad+m_strtMoment.m_dMaxSettlement;
	m_strtTotLoad.m_dMinMoment		= m_strtMoment.m_dPreDeadLoad+m_strtMoment.m_dPostDeadLoad+m_strtMoment.m_dMinLiveLoad+m_strtMoment.m_dMinSettlement;
	m_strtTotLoad.m_dMaxShear		= m_strtShear.m_dPreDeadLoad+m_strtShear.m_dPostDeadLoad+m_strtShear.m_dMaxLiveLoad+m_strtShear.m_dMaxSettlement;
	m_strtTotLoad.m_dMinShear		= m_strtShear.m_dPreDeadLoad+m_strtShear.m_dPostDeadLoad+m_strtShear.m_dMinLiveLoad+m_strtShear.m_dMinSettlement;
	m_strtTotLoad.m_dMaxAxial		= m_strtAxial.m_dPreDeadLoad+m_strtAxial.m_dPostDeadLoad+m_strtAxial.m_dMaxLiveLoad+m_strtAxial.m_dMaxSettlement;
	m_strtTotLoad.m_dMinAxial		= m_strtAxial.m_dPreDeadLoad+m_strtAxial.m_dPostDeadLoad+m_strtAxial.m_dMinLiveLoad+m_strtAxial.m_dMinSettlement;
}
//////////////////////////////////////////////////////////////////////////
// 수직보강재
CCrossBeamVerStiff::CCrossBeamVerStiff(CPlateBasicIndex *pBx,  CAPlateCalcStd *pAPlateCalcStd)
{
	m_pBx				= pBx;
	m_strMaterial		= _T("");
	m_bIsNeedVerStiff	= FALSE;
	m_dMaxSpace			= 0;
	m_dVerStiffSpace	= 0;
	m_dBvs				= 0;
	m_dTvs				= 0;
	m_dAllowShear		= 0;
	m_dShear			= 0;
	m_dHeightWeb		= 0;
	m_dThickWeb			= 0;
	m_dSigma			= 0;
	m_dBvsMin			= 0;
	m_dTvsMin			= 0;
	m_dRateSpaceHeight	= 0;
	m_dAllowSpaceCoeff	= 0;
	m_nQtyHorStiff		= 0;
	m_dGamma			= 0;
	m_dIReq				= 0;
	m_dIUse				= 0;
	m_nVerSide			= 0;
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_strRight			= "B";
}

CCrossBeamVerStiff::~CCrossBeamVerStiff()
{

}

void CCrossBeamVerStiff::SetCrossBeamVerStiff()
{
	if(m_pBx==NULL)
		return;

	CSteel			*pSteelCtl = m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CPlateCrossBeam	*pCr	= m_pBx->GetCrossBeam();

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(m_strMaterial + m_strRight)];

	CHGAllowStress	AllowSpace(pSteel);

	long	nQtyVerStiff	= pCr->m_nVStiffsu_CR;
	double	dCurSpace		= 0;

	m_dMaxSpaceCoeff	= AllowSpace.GetAllowStress(ALLOW_VSTIFFNOT,	0);
	m_dSpaceCoeff		= sqrt(m_dAllowShear/m_dShear);
	if(m_dSpaceCoeff>1.2)	m_dSpaceCoeffUse = 1.2;
	else					m_dSpaceCoeffUse = m_dSpaceCoeff;

	m_dMaxSpace	= m_dMaxSpaceCoeff * m_dThickWeb;// * m_dSpaceCoeffUse; 070209일자 수정....KB....
	
	m_dVerStiffSpace = 0.0;
	for(long nVStiff = 0; nVStiff < nQtyVerStiff; nVStiff++)
	{
		dCurSpace = pCr->m_VStiffDis_CR[nVStiff];
		if(nVStiff==0)
			dCurSpace += pCr->m_sA+pCr->m_sB*pCr->m_sC+pCr->m_sC1/2.0+pCr->m_P_CR;
		m_dVerStiffSpace = max(m_dVerStiffSpace, dCurSpace);
	}
	if(nQtyVerStiff>0)
	{
		dCurSpace = pCr->m_sA+pCr->m_sB*pCr->m_sC+pCr->m_sC1/2.0+pCr->m_P_CR;
		for(long nVStiff = 0; nVStiff < nQtyVerStiff; nVStiff++)
			dCurSpace += pCr->m_VStiffDis_CR[nVStiff];
		dCurSpace = m_pBx->GetGirder()->GetDisFrontPlateWebToWeb(m_pBx) - dCurSpace;
		m_dVerStiffSpace = max(m_dVerStiffSpace, dCurSpace);
	}
	if(nQtyVerStiff==0 || m_dMaxSpace>=m_dHeightWeb)
	{
		m_bIsNeedVerStiff = FALSE;
		if(nQtyVerStiff==0)
			return;
	}
	else	m_bIsNeedVerStiff	= TRUE;

	m_dRateSpaceHeight	= m_dVerStiffSpace/m_dHeightWeb;
	m_dBvs				= pCr->m_vW_CR;
	m_dTvs				= pCr->m_vT_CR;
	m_dBvsMin			= m_dHeightWeb/30.0+50.0;
	m_dTvsMin			= m_dBvs/13.0;
	if(m_nQtyHorStiff==0)
	{
		if(m_dRateSpaceHeight>1.0)
			m_dAllowSpaceCoeff = pow(m_dHeightWeb/(100*m_dThickWeb),4)*(pow(m_dSigma/365.0,2)+pow(m_dShear/(81.0+61.0*pow(m_dHeightWeb/m_dVerStiffSpace,2)),2));
		else
			m_dAllowSpaceCoeff = pow(m_dHeightWeb/(100*m_dThickWeb),4)*(pow(m_dSigma/365.0,2)+pow(m_dShear/(61.0+81.0*pow(m_dHeightWeb/m_dVerStiffSpace,2)),2));
	}
	else if(m_nQtyHorStiff==1)
	{
		if(m_dRateSpaceHeight>0.8)
			m_dAllowSpaceCoeff = pow(m_dHeightWeb/(100*m_dThickWeb),4)*(pow(m_dSigma/950.0,2)+pow(m_dShear/(127.0+61.0*pow(m_dHeightWeb/m_dVerStiffSpace,2)),2));
		else
			m_dAllowSpaceCoeff = pow(m_dHeightWeb/(100*m_dThickWeb),4)*(pow(m_dSigma/950.0,2)+pow(m_dShear/(95.0+81.0*pow(m_dHeightWeb/m_dVerStiffSpace,2)),2));
	}
	else if(m_nQtyHorStiff==2)
	{
		if(m_dRateSpaceHeight>0.64)
			m_dAllowSpaceCoeff = pow(m_dHeightWeb/(100*m_dThickWeb),4)*(pow(m_dSigma/3150.0,2)+pow(m_dShear/(197.0+61.0*pow(m_dHeightWeb/m_dVerStiffSpace,2)),2));
		else
			m_dAllowSpaceCoeff = pow(m_dHeightWeb/(100*m_dThickWeb),4)*(pow(m_dSigma/3150.0,2)+pow(m_dShear/(148.0+81.0*pow(m_dHeightWeb/m_dVerStiffSpace,2)),2));
	}
	if(pCr->m_cVSide_CR==0)	m_nVerSide = 2;
	else							m_nVerSide = 1;
	m_dGamma	= 8.0*pow(m_dHeightWeb/m_dVerStiffSpace,2);
	m_dIReq		= m_dHeightWeb * pow(m_dThickWeb,3) * m_dGamma / 10.92;
	m_dIUse		= m_dTvs * pow(m_dBvs,3) / 3 * m_nVerSide;
}

//////////////////////////////////////////////////////////////////////////
// 수평보강재
CCrossBeamHorStiff::CCrossBeamHorStiff(CPlateBasicIndex *pBx)
{
	m_pBx				= pBx;
	m_dHeightWeb		= 0;
	m_dThickWeb			= 0;
	m_dVerStiffSpace	= 0;
	m_dBhs				= 0;
	m_dThs				= 0;
	m_dThsMin			= 0;
	m_dGamma			= 0;
	m_dIReq				= 0;
	m_dIUse				= 0;
	m_nHorSide			= 0;
}

CCrossBeamHorStiff::~CCrossBeamHorStiff()
{

}

void CCrossBeamHorStiff::SetCrossBeamHorStiff()
{
	CPlateCrossBeam	*pCrossBeam	= m_pBx->GetCrossBeam();
	if(pCrossBeam->m_cVSide_CR==0)	m_nHorSide = 2;
	else							m_nHorSide = 1;

	m_dBhs		= pCrossBeam->m_hW_CR;
	m_dThs		= pCrossBeam->m_hT_CR;
	m_dThsMin	= m_dBhs/16.0;
	m_dGamma	= 30.0*(m_dVerStiffSpace/m_dHeightWeb);
	m_dIReq		= m_dHeightWeb*pow(m_dThickWeb,3)*m_dGamma/10.92;
	m_dIUse		= m_dThs*pow(m_dBhs,3)/3.0*m_nHorSide;
}

//////////////////////////////////////////////////////////////////////////
// 단부 수직브레이싱(역V형 타입)
CDesignInverseVBracing::CDesignInverseVBracing(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
	SetDataInit();
	CSection	*pSection	= pBx->GetSection();
	CVBracing	*pVBr		= pSection->GetVBracing();
	if(pVBr->GetType()==TYPE_INVVBRACING)
		SetInverseVBracingType();	// 역V형 타입
}

CDesignInverseVBracing::~CDesignInverseVBracing()
{
	delete m_pChordUp;
	delete m_pChordLo;
	delete m_pDiagonal;
}

void CDesignInverseVBracing::SetDataInit()
{
	m_pChordUp		= new CBracingJewon();
	m_pChordLo		= new CBracingJewon();
	m_pDiagonal		= new CBracingJewon();
}

void CDesignInverseVBracing::SetInverseVBracingType()
{
	SetJewon();
	SetVerLoad();
	SetHorLoad();
	SetEarthQuakeLoad();
	SetElementLoad();
	SetDesignElement();
	SetGussetPlate();
}

void CDesignInverseVBracing::SetJewon()
{
	CPlateGirderApp	*pGir			= m_pBx->GetGirder();
	CDataManage		*pDataManager	= m_pAPlateCalcStd->m_pDataManage;
	CVBracing       *pVBracing		= m_pBx->GetSection()->GetVBracing();

	CSteelMaterialData	*pChordUp	= pVBracing->GetSteelMaterialData(STEELMAT_UPPER);
	CSteelMaterialData	*pChordLo	= pVBracing->GetSteelMaterialData(STEELMAT_LOWER);
	CSteelMaterialData	*pDiagonal	= pVBracing->GetSteelMaterialData(STEELMAT_DIAGONAL);

	m_dHeight		= pVBracing->GetLengthFrameLine(4);
	m_dGirderSpace	= pGir->GetDisGirderToGirder(m_pBx);
	m_dTheta		= atan(m_dHeight/(m_dGirderSpace/2.0));

	m_pChordUp->m_nType		= pChordUp->m_nType;
	m_pChordUp->m_dHeight	= pChordUp->m_dH;
	m_pChordUp->m_dWidth	= pChordUp->m_dB;
	m_pChordUp->m_dThick1	= pChordUp->m_dT1;
	m_pChordUp->m_dThick2	= pChordUp->m_dT2;
	m_pChordUp->m_dArea		= frCm2(pChordUp->m_dSectionArea);
	m_pChordUp->m_dGammaX	= frCm(pChordUp->m_dRadiusOfGyration.x);
	m_pChordUp->m_dGammaY	= frCm(pChordUp->m_dRadiusOfGyration.y);
	m_pChordUp->m_dGammaMin	= frCm(MIN(pChordUp->m_dRadiusOfGyrationMaxMin.x, pChordUp->m_dRadiusOfGyrationMaxMin.y));
	if(m_pChordUp->m_dGammaMin==0) m_pChordUp->m_dGammaMin=frCm(MIN(pChordUp->m_dRadiusOfGyration.x, pChordUp->m_dRadiusOfGyration.y));

	m_pChordUp->m_dLength	= m_dGirderSpace;
	m_pChordUp->m_dZy		= frCm3(pChordUp->m_dModulusOfSection.y);
	if(pChordUp->m_nType==CSteelMaterialData::CSTEEL)
		m_pChordUp->m_dZy		= frCm3(pChordUp->m_dModulusOfSection.x);

	m_pChordLo->m_nType		= pChordLo->m_nType;
	m_pChordLo->m_dHeight	= pChordLo->m_dH;
	m_pChordLo->m_dWidth	= pChordLo->m_dB;
	m_pChordLo->m_dThick1	= pChordLo->m_dT1;
	m_pChordLo->m_dThick2	= pChordLo->m_dT2;
	m_pChordLo->m_dArea		= frCm2(pChordLo->m_dSectionArea);
	m_pChordLo->m_dGammaX	= frCm(pChordLo->m_dRadiusOfGyration.x);
	m_pChordLo->m_dGammaY	= frCm(pChordLo->m_dRadiusOfGyration.y);
	m_pChordLo->m_dGammaMin	= frCm(MIN(pChordLo->m_dRadiusOfGyrationMaxMin.x, pChordLo->m_dRadiusOfGyrationMaxMin.y));
	if(m_pChordLo->m_dGammaMin==0) m_pChordLo->m_dGammaMin=frCm(MIN(pChordLo->m_dRadiusOfGyration.x, pChordLo->m_dRadiusOfGyration.y));

	m_pChordLo->m_dLength	= m_dGirderSpace;
	m_pChordLo->m_dZy		= frCm3(pChordLo->m_dModulusOfSection.y);
	if(pChordLo->m_nType=CSteelMaterialData::CSTEEL)
		m_pChordLo->m_dZy		= frCm3(pChordLo->m_dModulusOfSection.x);

	m_pDiagonal->m_nType	= pDiagonal->m_nType;
	m_pDiagonal->m_dHeight	= pDiagonal->m_dH;
	m_pDiagonal->m_dWidth	= pDiagonal->m_dB;
	m_pDiagonal->m_dThick1	= pDiagonal->m_dT1;
	m_pDiagonal->m_dThick2	= pDiagonal->m_dT2;
	m_pDiagonal->m_dArea	= frCm2(pDiagonal->m_dSectionArea);
	m_pDiagonal->m_dGammaX	= frCm(pDiagonal->m_dRadiusOfGyration.x);
	m_pDiagonal->m_dGammaY	= frCm(pDiagonal->m_dRadiusOfGyration.y);
	m_pDiagonal->m_dGammaMin= frCm(MIN(pDiagonal->m_dRadiusOfGyrationMaxMin.x, pDiagonal->m_dRadiusOfGyrationMaxMin.y));
	if(m_pDiagonal->m_dGammaMin==0) m_pDiagonal->m_dGammaMin=frCm(MIN(pDiagonal->m_dRadiusOfGyration.x, pDiagonal->m_dRadiusOfGyration.y));

	m_pDiagonal->m_dLength	= sqrt(pow(m_dHeight,2) + pow(m_dGirderSpace/2.0,2));
	m_pDiagonal->m_dZy		= frCm3(pDiagonal->m_dModulusOfSection.y);
	if(pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
		m_pDiagonal->m_dZy		= frCm3(pDiagonal->m_dModulusOfSection.x);
}

void CDesignInverseVBracing::SetVerLoad()
{
	CPlateBridgeApp	*pDB		= m_pAPlateCalcStd->GetBridge();
	CPlateGirderApp	*pGir		= m_pBx->GetGirder();
	CCalcData		*pCalcData	= m_pAPlateCalcStd->GetCalcData();

	double	dPr			= pCalcData->DESIGN_CONDITION.m_dPr;
	double	dUnitPave	= pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt;
	double	dUnitConc	= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;
	double	dThickSlab	= pGir->GetThickSlab(m_pBx, 0);

	m_strtVerLoad.m_dDistVerBracingToEndSlab	= pDB->m_dLengthGirderShoeStt;		// 수직브레이싱에서 상판단부까지 거리
	m_strtVerLoad.m_dLoadPave					= pDB->m_dThickPave * dUnitPave;	// 포장하중
	m_strtVerLoad.m_dLoadSlab					= dThickSlab * dUnitConc;			// 상판하중
	m_strtVerLoad.m_dQ1							= m_strtVerLoad.m_dDistVerBracingToEndSlab*(m_strtVerLoad.m_dLoadPave+m_strtVerLoad.m_dLoadSlab);
	m_strtVerLoad.m_dQ2							= m_dGirderSpace/2*(m_strtVerLoad.m_dLoadPave+m_strtVerLoad.m_dLoadSlab);
	m_strtVerLoad.m_dPd		= m_strtVerLoad.m_dQ1*m_dGirderSpace/2.0 + 1.0/3.0*m_strtVerLoad.m_dQ2*m_dGirderSpace;
	if(m_dGirderSpace<2400.0)	m_strtVerLoad.m_dLength	= 1.0;
	else						m_strtVerLoad.m_dLength	= 1.0+1.0/m_dGirderSpace/2.0*(m_dGirderSpace/2.0-1200.0);
	m_strtVerLoad.m_dN1		= 0;
	m_strtVerLoad.m_dN2		= dPr * m_strtVerLoad.m_dLength/(2.0 * sin(m_dTheta));
	m_strtVerLoad.m_dN3		= dPr * m_strtVerLoad.m_dLength/(2.0 * tan(m_dTheta));
}

// 수평하중
void CDesignInverseVBracing::SetHorLoad()
{
	CPlateBridgeApp	*pDB		= m_pAPlateCalcStd->GetBridge();
	CPlateGirderApp	*pGir		= m_pBx->GetGirder();
	CCalcData		*pCalcData	= m_pAPlateCalcStd->GetCalcData();
	CCentSeparation	*pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);

	double	dStation			= m_pBx->GetStation();
	long	nSpanIdx			= pGir->GetNumberJiganByStation(dStation);
	double	dHeightGirder		= m_pBx->GetHeightGirderByBx();
	double	dSlabThick			= pDB->m_dThickSlabBase;
	double	dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double	dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double	dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double	dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;

	m_strtHorLoad.m_dWidthBridge		= pDB->m_dBridgeWidth;
	m_strtHorLoad.m_dHeightBridge		= dHeightGirder+dSlabThick+dHeightGuardFence+dHeightSoundProof+dHeightHandRail;
	m_strtHorLoad.m_dRateWidthHeight	= m_strtHorLoad.m_dWidthBridge/m_strtHorLoad.m_dHeightBridge;
	if(m_strtHorLoad.m_dRateWidthHeight>=1 && m_strtHorLoad.m_dRateWidthHeight<8)
		m_strtHorLoad.m_dWw	= (4.0-0.2*m_strtHorLoad.m_dRateWidthHeight)*m_strtHorLoad.m_dHeightBridge/1000.0;
	else if(m_strtHorLoad.m_dRateWidthHeight>=8.0)
		m_strtHorLoad.m_dWw = 2.4*m_strtHorLoad.m_dHeightBridge/1000.0;
	else
		m_strtHorLoad.m_dWw = 0;

	m_strtHorLoad.m_dWwUse			= max(m_strtHorLoad.m_dWw, 6000.0);
	m_strtHorLoad.m_dLengthSpan		= pDB->m_dLengthJigan[nSpanIdx];
	if(pDB->GetGirdersu()>=3)
		m_strtHorLoad.m_dHw			= 0.5*m_strtHorLoad.m_dWw*m_strtHorLoad.m_dLengthSpan/1.2;
	else
		m_strtHorLoad.m_dHw			= m_strtHorLoad.m_dWw*m_strtHorLoad.m_dLengthSpan/1.2;

}

void CDesignInverseVBracing::SetEarthQuakeLoad()
{
	CPlateBridgeApp		*pDB			= m_pAPlateCalcStd->GetBridge();
	CGlobarOption		*pGlobarOption	= m_pAPlateCalcStd->m_pDataManage->GetGlobalOption();
	CFemStressCalc		StressCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFemModelingCalc	ModelCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CResultArray		*pResult		= m_pAPlateCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);

	long	nIdxNode	= ModelCalc.GetShoeJointNoByBx(m_pBx);
	long	nG			= m_pBx->GetNumberGirder();
	long	nQtyGir		= pDB->GetGirdersu();
	double	dStation	= m_pBx->GetStation();
	BOOL	bPositive	= StressCalc.IsPositive(ELE_MOMENT3, nG, dStation);
	double	dRiskCoefficient	= pGlobarOption->GetRiskCoefficient();
	double	dZoneCoefficient	= pGlobarOption->GetZoneCoefficient();
	if(nIdxNode < 0) return;

	CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(nIdxNode);

	if(bPositive)	m_strtHorLoad.m_dReaction = frkN(pJResult->m_vTotalMax[JOINT_REACTION].z);
	else			m_strtHorLoad.m_dReaction = frkN(pJResult->m_vTotalMin[JOINT_REACTION].z);
	m_strtHorLoad.m_dAcceleration		= dRiskCoefficient*dZoneCoefficient;
	m_strtHorLoad.m_dTotEarthQuakeLoad	= m_strtHorLoad.m_dAcceleration*m_strtHorLoad.m_dReaction;
	m_strtHorLoad.m_dEarthQuakeLoad		= m_strtHorLoad.m_dTotEarthQuakeLoad/((double)nQtyGir-1.0)/1.5;
	m_strtHorLoad.m_dH					= MAX(m_strtHorLoad.m_dHw, m_strtHorLoad.m_dEarthQuakeLoad);
}

void CDesignInverseVBracing::SetElementLoad()
{
	CCalcData	*pCalcData	= m_pAPlateCalcStd->GetCalcData();

	double	dPr		= pCalcData->DESIGN_CONDITION.m_dPr;
	double	dPrY	= dPr*m_strtVerLoad.m_dLength;

	m_strtElementLoad.m_dCollCoefUp			= 15000.0/(40000.0+m_pChordUp->m_dLength/2.0);
	if(m_strtElementLoad.m_dCollCoefUp>0.3)			m_strtElementLoad.m_dCollCoefUp = 0.3;
	m_strtElementLoad.m_dCollCoefLo			= 15000.0/(40000.0+m_pChordLo->m_dLength/2.0);
	if(m_strtElementLoad.m_dCollCoefLo>0.3)			m_strtElementLoad.m_dCollCoefLo = 0.3;
	m_strtElementLoad.m_dCollCoefDiagonal	= 15000.0/(40000.0+m_pDiagonal->m_dLength/2.0);
	if(m_strtElementLoad.m_dCollCoefDiagonal>0.3)	m_strtElementLoad.m_dCollCoefDiagonal = 0.3;
	m_strtElementLoad.m_dN1					= m_strtHorLoad.m_dH/2.0;
	m_strtElementLoad.m_dMl					= dPr*(m_dGirderSpace/2.0)/4.0*(1.0+m_strtElementLoad.m_dCollCoefUp);
	m_strtElementLoad.m_dN2Hor				= m_strtHorLoad.m_dH/cos(m_dTheta)/4.0;
	m_strtElementLoad.m_dN2VerHor			= (m_strtVerLoad.m_dPd + dPrY*(1.0+m_strtElementLoad.m_dCollCoefDiagonal))/(2.0*sin(m_dTheta))+1.0/4.0*m_strtHorLoad.m_dH/cos(m_dTheta);
	m_strtElementLoad.m_dN3Ver				= (m_strtVerLoad.m_dPd + dPrY*(1.0+m_strtElementLoad.m_dCollCoefLo))/(2.0*tan(m_dTheta));
	m_strtElementLoad.m_dN3Hor				= 3.0/4.0*m_strtHorLoad.m_dH;
}

void CDesignInverseVBracing::SetDesignElement()
{
	CSteel			*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CPlateGirderApp	*pGir		= m_pBx->GetGirder();
	CVBracing       *pVBracing	= m_pBx->GetSection()->GetVBracing();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);
	
	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	CalcGen.GetMaterial(m_pBx, VB_UB, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBucklingUp(pSteel);
	CHGAllowStress	AllowCompUp(pSteel);
	CHGAllowStress	AllowShearUp(pSteel);
	

	// 상현재
	double	dThickGusset= pGir->GetThickJewon(VB_GU, m_pBx);
	
	AllowBucklingUp.SetAllowBucklingUnconsider(m_pChordUp->m_dLength, m_pChordUp->m_dGammaX);

	m_strtChordUp.m_strMaterial	= strLeft;
	if(m_pChordUp->m_nType==CSteelMaterialData::CTSTEEL)
	{	
		m_strtChordUp.m_sMaterialType.Format("CT-%.0fx%.0fx%.0fx%.0f", 
			m_pChordUp->m_dHeight,m_pChordUp->m_dWidth,m_pChordUp->m_dThick1,m_pChordUp->m_dThick2);
	}
	if(m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
	{	
		m_strtChordUp.m_sMaterialType.Format("ㄷ-%.0fx%.0fx%.0fx%.0f", 
			m_pChordUp->m_dHeight,m_pChordUp->m_dWidth,m_pChordUp->m_dThick1,m_pChordUp->m_dThick2);
	}
	else
	{
		m_strtChordUp.m_sMaterialType.Format("L-%.0fx%.0fx%.0f", 
			m_pChordUp->m_dHeight,m_pChordUp->m_dWidth,m_pChordUp->m_dThick1);
	}
	m_strtChordUp.m_dSlendernessRatio	= m_pChordUp->m_dLength/m_pChordUp->m_dGammaMin;
	m_strtChordUp.m_dFca		= AllowBucklingUp.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER,	m_pChordUp->m_dThick1);	// 국부좌굴을 고려하지 않은 허용축방향 압축응력
	m_strtChordUp.m_dFcaApo		= m_strtChordUp.m_dFca*(0.5+m_pChordUp->m_dLength/m_pChordUp->m_dGammaX/1000.0);
	m_strtChordUp.m_dFa			= AllowCompUp.GetAllowStress(ALLOW_COMPSTRESS,	m_pChordUp->m_dThick1);
	m_strtChordUp.m_dFc			= m_strtElementLoad.m_dN1/m_pChordUp->m_dArea;
	m_strtChordUp.m_dF			= m_strtElementLoad.m_dMl/m_pChordUp->m_dZy;
	m_strtChordUp.m_dAllowShear	= AllowShearUp.GetAllowStress(ALLOW_SHEARSTRESS,	m_pChordUp->m_dThick1);
	m_strtChordUp.m_dLengthWeld	= pVBracing->m_dULG*2.0;
	m_strtChordUp.m_dThroatDepth= (max(RoundUp(sqrt(2*max(m_pChordUp->m_dThick1, dThickGusset)), 0), 6));
	if(m_strtChordUp.m_dFc<0.75*m_strtChordUp.m_dFcaApo)
		m_strtChordUp.m_dN1Apo	= 0.75*m_pChordUp->m_dArea*m_strtChordUp.m_dFcaApo;
	else
		m_strtChordUp.m_dN1Apo	= m_strtElementLoad.m_dN1;
	m_strtChordUp.m_dShear		= m_strtChordUp.m_dN1Apo/(m_strtChordUp.m_dThroatDepth/sqrt(2.0)*m_strtChordUp.m_dLengthWeld);

	// 사재

	CalcGen.GetMaterial(m_pBx, VB_CR, strLeft, strRight);
	pSteel	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBucklingDiagonal(pSteel);
	CHGAllowStress	AllowCompDiagonal(pSteel);
	CHGAllowStress	AllowShearDiagonal(pSteel);
	
	AllowBucklingDiagonal.SetAllowBucklingUnconsider(m_pDiagonal->m_dLength, m_pDiagonal->m_dGammaX);

	m_strtDiagonal.m_strMaterial= strLeft;
	if(m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
	{	
		m_strtDiagonal.m_sMaterialType.Format("CT-%.0fx%.0fx%.0fx%.0f", 
			m_pDiagonal->m_dHeight,m_pDiagonal->m_dWidth,m_pDiagonal->m_dThick1,m_pDiagonal->m_dThick2);
	}
	if(m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
	{	
		m_strtDiagonal.m_sMaterialType.Format("ㄷ-%.0fx%.0fx%.0fx%.0f", 
			m_pDiagonal->m_dHeight,m_pDiagonal->m_dWidth,m_pDiagonal->m_dThick1,m_pDiagonal->m_dThick2);
	}
	else
	{
		m_strtDiagonal.m_sMaterialType.Format("L-%.0fx%.0fx%.0f", 
			m_pDiagonal->m_dHeight,m_pDiagonal->m_dWidth,m_pDiagonal->m_dThick1);
	}
	m_strtDiagonal.m_dSlendernessRatio	= m_pDiagonal->m_dLength/m_pDiagonal->m_dGammaMin;
	m_strtDiagonal.m_dFca		= AllowBucklingDiagonal.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, m_pDiagonal->m_dThick1);
	m_strtDiagonal.m_dFcaApo	= m_strtDiagonal.m_dFca*(0.5+m_pDiagonal->m_dLength/m_pDiagonal->m_dGammaX/1000.0);
	m_strtDiagonal.m_dFta		= AllowCompDiagonal.GetAllowStress(ALLOW_TENSIONSTRESS, m_pDiagonal->m_dThick1);
	if(m_pDiagonal->m_nType==CSteelMaterialData::LSTEEL || m_pDiagonal->m_nType==CSteelMaterialData::LVALSTEEL)
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea-(m_pDiagonal->m_dWidth-m_pDiagonal->m_dThick1)/2*m_pDiagonal->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea-(m_pDiagonal->m_dWidth-m_pDiagonal->m_dThick2)/2*m_pDiagonal->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea/2;
	else
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea;
	m_strtDiagonal.m_dFt			= m_strtElementLoad.m_dN2Hor/m_strtDiagonal.m_dAn;
	m_strtDiagonal.m_dFc			= m_strtElementLoad.m_dN2VerHor/m_pDiagonal->m_dArea;
	m_strtDiagonal.m_dAllowShear	= AllowShearDiagonal.GetAllowStress(ALLOW_SHEARSTRESS, m_pDiagonal->m_dThick1);
	m_strtDiagonal.m_dLengthWeld	= min(pVBracing->m_dC1LG, pVBracing->m_dC2LG)*2;
	m_strtDiagonal.m_dThroatDepth	= (max(RoundUp(sqrt(2*max(m_pDiagonal->m_dThick1, dThickGusset)), 0), 6));
	if(m_strtDiagonal.m_dFc<0.75*m_strtDiagonal.m_dFcaApo)
		m_strtDiagonal.m_dN2Apo	= 0.75*m_pDiagonal->m_dArea*m_strtDiagonal.m_dFcaApo;
	else
		m_strtDiagonal.m_dN2Apo	= m_strtElementLoad.m_dN2VerHor;
	m_strtDiagonal.m_dShear		= m_strtDiagonal.m_dN2Apo/(m_strtDiagonal.m_dThroatDepth/sqrt(2.0)*m_strtDiagonal.m_dLengthWeld);

	// 하현재
	CalcGen.GetMaterial(m_pBx, VB_LB, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBucklingLo(pSteel);
	CHGAllowStress	AllowCompLo(pSteel);
	CHGAllowStress	AllowShearLo(pSteel);
	
	AllowBucklingLo.SetAllowBucklingUnconsider(m_pChordLo->m_dLength, m_pChordLo->m_dGammaX);

	m_strtChordLo.m_strMaterial			= strLeft;
	if(m_pChordLo->m_nType==CSteelMaterialData::CTSTEEL)
	{	
		m_strtChordLo.m_sMaterialType.Format("CT-%.0fx%.0fx%.0fx%.0f", 
			m_pChordLo->m_dHeight,m_pChordLo->m_dWidth,m_pChordLo->m_dThick1,m_pChordLo->m_dThick2);
	}
	if(m_pChordLo->m_nType==CSteelMaterialData::CSTEEL)
	{	
		m_strtChordLo.m_sMaterialType.Format("ㄷ-%.0fx%.0fx%.0fx%.0f", 
			m_pChordLo->m_dHeight,m_pChordLo->m_dWidth,m_pChordLo->m_dThick1,m_pChordLo->m_dThick2);
	}
	else
	{
		m_strtChordLo.m_sMaterialType.Format("L-%.0fx%.0fx%.0f", 
			m_pChordLo->m_dHeight,m_pChordLo->m_dWidth,m_pChordLo->m_dThick1);
	}
	m_strtChordLo.m_dSlendernessRatio	= m_pChordLo->m_dLength/m_pChordLo->m_dGammaMin;
	m_strtChordLo.m_dFca				= AllowBucklingLo.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, m_pChordLo->m_dThick1);
	m_strtChordLo.m_dFcaApo				= m_strtChordLo.m_dFca*(0.5+m_pChordLo->m_dLength/m_pChordLo->m_dGammaX/1000.0);
	m_strtChordLo.m_dFta				= AllowCompLo.GetAllowStress(ALLOW_TENSIONSTRESS, m_pChordLo->m_dThick1);
	if(m_pChordLo->m_nType==CSteelMaterialData::LSTEEL || m_pChordLo->m_nType==CSteelMaterialData::LVALSTEEL)
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea-(m_pChordLo->m_dWidth-m_pChordLo->m_dThick1)/2*m_pChordLo->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea-(m_pChordLo->m_dWidth-m_pChordLo->m_dThick2)/2*m_pChordLo->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea/2;
	else
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea;
	m_strtChordLo.m_dFt					= m_strtElementLoad.m_dN3Ver/m_strtChordLo.m_dAn;
	m_strtChordLo.m_dFc					= m_strtElementLoad.m_dN3Hor/m_pChordLo->m_dArea;
	m_strtChordLo.m_dAllowShear			= AllowShearLo.GetAllowStress(ALLOW_SHEARSTRESS, m_pChordLo->m_dThick1);
	m_strtChordLo.m_dLengthWeld			= pVBracing->m_dDLG*2;
	m_strtChordLo.m_dThroatDepth		= (max(RoundUp(sqrt(2*max(m_pChordLo->m_dThick1, dThickGusset)), 0), 6));
	if(m_strtChordLo.m_dFc<0.75*m_strtChordLo.m_dFcaApo)
		m_strtChordLo.m_dN3Apo			= 0.75*m_pChordLo->m_dArea*m_strtChordLo.m_dFcaApo;
	else
		m_strtChordLo.m_dN3Apo			= m_strtElementLoad.m_dN3Hor;
	m_strtChordLo.m_dShear				= m_strtChordLo.m_dN3Apo/(m_strtChordLo.m_dThroatDepth/sqrt(2.0)*m_strtChordLo.m_dLengthWeld);
}

void CDesignInverseVBracing::SetGussetPlate()
{
	CVBracing		*pVBracing	= m_pBx->GetSection()->GetVBracing();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CCalcData		*pCalcData	= m_pAPlateCalcStd->GetCalcData();
	CSteelBolt		*pSteelBolt = m_pAPlateCalcStd->m_pDataManage->GetSteelBolt();
	CSteel			*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	double	dPr			= pCalcData->DESIGN_CONDITION.m_dPr;
	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	CalcGen.GetMaterial(m_pBx, VB_GU, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBending(pSteel);
	CHGAllowStress	AllowShear(pSteel);
	

	// 상현재 연결
	
	m_strtGussetUp.m_strMaterial	= strLeft;
	m_strtGussetUp.m_dHeight		= pVBracing->m_duL1 + pVBracing->m_duL2;
	m_strtGussetUp.m_dThick			= pVBracing->m_dT;
	m_strtGussetUp.m_dWidth			= pVBracing->m_duL3 + pVBracing->m_duL4;
	m_strtGussetUp.m_dAllowStress	= AllowBending.GetAllowStress(ALLOW_BENDINGCOMP_CASE1, m_strtGussetUp.m_dThick);
	m_strtGussetUp.m_dLoad			= m_strtElementLoad.m_dN1;

	CalcGen.GetMaterial(m_pBx, VB_BOLT, strLeft, strRight);
	m_strtGussetUp.m_strBoltMaterial	= strLeft+strRight;
	m_strtGussetUp.m_dDiaBolt			= GetBoltSize(pVBracing->m_ZzsBoltSize);

	CSteelBoltData *pBolt	= pSteelBolt->GetBolt(m_strtGussetUp.m_strBoltMaterial, m_strtGussetUp.m_dDiaBolt);
	double dHoleMargin		= 3;
	if(!pBolt)
	{
		if(m_strtGussetUp.m_dDiaBolt==20.0)			m_strtGussetUp.m_dBoltAllowStress	= 39000.0;
		else if(m_strtGussetUp.m_dDiaBolt==22.0)	m_strtGussetUp.m_dBoltAllowStress	= 48000.0;
		else										m_strtGussetUp.m_dBoltAllowStress	= 48000.0;
	}
	else
	{
		m_strtGussetUp.m_dBoltAllowStress	= pBolt->m_ForceAllow;
		dHoleMargin							= pBolt->m_HoleMargin;
	}
	
	m_strtGussetUp.m_dQtyBoltReq	= m_strtGussetUp.m_dLoad/m_strtGussetUp.m_dBoltAllowStress;
	m_strtGussetUp.m_dQtyBoltUse	= pVBracing->m_uB+1;
	m_strtGussetUp.m_dAn			= (m_strtGussetUp.m_dHeight - m_strtGussetUp.m_dQtyBoltUse*(m_strtGussetUp.m_dDiaBolt+dHoleMargin))*m_strtGussetUp.m_dThick;
	m_strtGussetUp.m_dStress		= m_strtGussetUp.m_dLoad/m_strtGussetUp.m_dAn;

	// 사재/상현재 연결
	CalcGen.GetMaterial(m_pBx, VB_GU, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowBending.SetBaseSteel(pSteel);
	AllowShear.SetBaseSteel(pSteel);

	m_strtGussetDiagonal.m_strMaterial	= strLeft;
	m_strtGussetDiagonal.m_dHeight		= pVBracing->m_dc1 + pVBracing->m_dc2;
	m_strtGussetDiagonal.m_dThick		= pVBracing->m_dT;
	m_strtGussetDiagonal.m_dWidth		= pVBracing->m_dC*2;
	m_strtGussetDiagonal.m_dAllowStress	= AllowBending.GetAllowStress(ALLOW_BENDINGCOMP_CASE1, m_strtGussetDiagonal.m_dThick);
	m_strtGussetDiagonal.m_dAllowShear	= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, m_strtGussetDiagonal.m_dThick);
	m_strtGussetDiagonal.m_dLoadHor		= m_strtChordUp.m_dN1Apo;
	m_strtGussetDiagonal.m_dLoadVer		= m_strtVerLoad.m_dPd + dPr*m_strtVerLoad.m_dLength*(1+m_strtElementLoad.m_dCollCoefUp);
	m_strtGussetDiagonal.m_dLoad		= sqrt(pow(m_strtGussetDiagonal.m_dLoadHor,2)+pow(m_strtGussetDiagonal.m_dLoadVer,2));
	m_strtGussetDiagonal.m_dAllowShearWeld	= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, m_strtGussetDiagonal.m_dThick);
	m_strtGussetDiagonal.m_dLengthWeld	= m_strtGussetDiagonal.m_dWidth*2;
	m_strtGussetDiagonal.m_dThroatDepth	= (max(RoundUp(sqrt(2*max(m_pDiagonal->m_dThick1, m_strtGussetDiagonal.m_dThick)), 0), 6));
	m_strtGussetDiagonal.m_dShearWeld	= m_strtGussetDiagonal.m_dLoad/(m_strtGussetDiagonal.m_dThroatDepth/sqrt(2.0)*m_strtGussetDiagonal.m_dLengthWeld);
	m_strtGussetDiagonal.m_dAgv			= m_strtGussetDiagonal.m_dWidth*m_strtGussetDiagonal.m_dThick;
	m_strtGussetDiagonal.m_dAgh			= m_strtGussetDiagonal.m_dHeight*m_strtGussetDiagonal.m_dThick;
	m_strtGussetDiagonal.m_dStress		= m_strtGussetDiagonal.m_dLoadVer/m_strtGussetDiagonal.m_dAgv;
	m_strtGussetDiagonal.m_dShearGusset	= m_strtGussetDiagonal.m_dLoadHor/m_strtGussetDiagonal.m_dAgh;
	m_strtGussetDiagonal.m_dCompStress	= sqrt(pow(m_strtGussetDiagonal.m_dStress/m_strtGussetDiagonal.m_dAllowStress,2)+
												pow(m_strtGussetDiagonal.m_dShearGusset/m_strtGussetDiagonal.m_dAllowShear,2));

	// 하현재/사재 연결
	CalcGen.GetMaterial(m_pBx, VB_GU, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowBending.SetBaseSteel(pSteel);
	AllowShear.SetBaseSteel(pSteel);

	m_strtGussetLo.m_strMaterial	= strLeft;
	m_strtGussetLo.m_dHeight		= pVBracing->m_ddL1 + pVBracing->m_ddL2;
	m_strtGussetLo.m_dThick			= pVBracing->m_dT;
	m_strtGussetLo.m_dWidth			= pVBracing->m_ddL3 + pVBracing->m_ddL4;
	m_strtGussetLo.m_dAllowStress	= AllowBending.GetAllowStress(ALLOW_BENDINGCOMP_CASE1, m_strtGussetDiagonal.m_dThick);
	m_strtGussetLo.m_dRh			= m_strtChordLo.m_dN3Apo+m_strtDiagonal.m_dN2Apo*cos(m_dTheta);
	m_strtGussetLo.m_dRv			= m_strtDiagonal.m_dN2Apo*sin(m_dTheta);
	m_strtGussetLo.m_dLoad			= sqrt(pow(m_strtDiagonal.m_dN2Apo,2)+pow(m_strtGussetLo.m_dRv,2));
	
	CalcGen.GetMaterial(m_pBx, VB_BOLT, strLeft, strRight);
	//APLATE-712 볼트구하는데 Steel정보를 쓸 필요가 없음.
	//pSteel = pSteelBolt->m_BoltArr[pSteelBolt->GetIndex(strLeft + strRight)];
	//AllowBending.SetBaseSteel(pSteel);
	//AllowShear.SetBaseSteel(pSteel);

	m_strtGussetLo.m_strBoltMaterial= strLeft+strRight;
	m_strtGussetLo.m_dDiaBolt		= GetBoltSize(pVBracing->m_ZzsBoltSize);

	pBolt		= pSteelBolt->GetBolt(m_strtGussetLo.m_strBoltMaterial, m_strtGussetLo.m_dDiaBolt);
	dHoleMargin = 3;
	if(!pBolt)
	{
		if(m_strtGussetLo.m_dDiaBolt==20.0)			m_strtGussetLo.m_dBoltAllowStress	= 39000.0;
		else if(m_strtGussetLo.m_dDiaBolt==22.0)	m_strtGussetLo.m_dBoltAllowStress	= 48000.0;
		else										m_strtGussetLo.m_dBoltAllowStress	= 48000.0;
	}
	else
	{
		m_strtGussetLo.m_dBoltAllowStress	= pBolt->m_ForceAllow;
		dHoleMargin							= pBolt->m_HoleMargin;
	}

	m_strtGussetLo.m_dQtyBoltReq	= m_strtGussetLo.m_dLoad/m_strtGussetLo.m_dBoltAllowStress;
	m_strtGussetLo.m_dQtyBoltUse	= pVBracing->m_dB+1;
	m_strtGussetLo.m_dAn			= (m_strtGussetLo.m_dHeight - m_strtGussetLo.m_dQtyBoltUse*(m_strtGussetLo.m_dDiaBolt+dHoleMargin))*m_strtGussetLo.m_dThick;
	m_strtGussetLo.m_dStress		= m_strtGussetLo.m_dLoad/m_strtGussetLo.m_dAn;
}

//////////////////////////////////////////////////////////////////////////
// 중간 수직브레이싱(V형 타입)
CDesignVBracing::CDesignVBracing(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
	SetDataInit();
	CSection	*pSec	= pBx->GetSection();
	CVBracing	*pVBr	= pSec->GetVBracing();
	if(pVBr->GetType()==TYPE_VBRACING)
		SetVBracingType();
}

CDesignVBracing::~CDesignVBracing()
{
	delete m_pChordUp;
	delete m_pChordLo;
	delete m_pDiagonal;
}

void CDesignVBracing::SetDataInit()
{
	m_pChordUp		= new CBracingJewon();
	m_pChordLo		= new CBracingJewon();
	m_pDiagonal		= new CBracingJewon();
}

void CDesignVBracing::SetVBracingType()
{
	SetJewon();
	SetLoadCalculate();
	SetElementLoad();
	SetDesignElement();
	SetGussetPlate();
}

void CDesignVBracing::SetJewon()
{
	CPlateGirderApp	*pGir			= m_pBx->GetGirder();
	CDataManage		*pDataManager	= m_pAPlateCalcStd->m_pDataManage;
	CVBracing       *pVBracing		= m_pBx->GetSection()->GetVBracing();

	CSteelMaterialData	*pChordUp	= pVBracing->GetSteelMaterialData(STEELMAT_UPPER);
	CSteelMaterialData	*pChordLo	= pVBracing->GetSteelMaterialData(STEELMAT_LOWER);
	CSteelMaterialData	*pDiagonal	= pVBracing->GetSteelMaterialData(STEELMAT_DIAGONAL);
	
	m_dHeight		= pVBracing->GetLengthFrameLine(4);
	m_dGirderSpace	= pGir->GetDisGirderToGirder(m_pBx);
	m_dTheta		= atan(m_dHeight/(m_dGirderSpace/2.0));
	m_pChordUp->m_nType		= pChordUp->m_nType;
	m_pChordUp->m_dHeight	= pChordUp->m_dH;
	m_pChordUp->m_dWidth	= pChordUp->m_dB;
	m_pChordUp->m_dThick1	= pChordUp->m_dT1;
	m_pChordUp->m_dThick2	= pChordUp->m_dT2;
	m_pChordUp->m_dArea		= frCm2(pChordUp->m_dSectionArea);
	m_pChordUp->m_dGammaX	= frCm(pChordUp->m_dRadiusOfGyration.x);
	m_pChordUp->m_dGammaY	= frCm(pChordUp->m_dRadiusOfGyration.y);
	m_pChordUp->m_dGammaMin	= frCm(MIN(pChordUp->m_dRadiusOfGyrationMaxMin.x, pChordUp->m_dRadiusOfGyrationMaxMin.y));
	if(m_pChordUp->m_dGammaMin==0) m_pChordUp->m_dGammaMin=frCm(MIN(pChordUp->m_dRadiusOfGyration.x, pChordUp->m_dRadiusOfGyration.y));
	m_pChordUp->m_dLength	= m_dGirderSpace;
	m_pChordUp->m_dZy		= frCm3(pChordUp->m_dModulusOfSection.y);
	if(pChordUp->m_nType==CSteelMaterialData::CSTEEL)
		m_pChordUp->m_dZy		= frCm3(pChordUp->m_dModulusOfSection.x);

	m_pChordLo->m_nType		= pChordLo->m_nType;
	m_pChordLo->m_dHeight	= pChordLo->m_dH;
	m_pChordLo->m_dWidth	= pChordLo->m_dB;
	m_pChordLo->m_dThick1	= pChordLo->m_dT1;
	m_pChordLo->m_dThick2	= pChordLo->m_dT2;
	m_pChordLo->m_dArea		= frCm2(pChordLo->m_dSectionArea);
	m_pChordLo->m_dGammaX	= frCm(pChordLo->m_dRadiusOfGyration.x);
	m_pChordLo->m_dGammaY	= frCm(pChordLo->m_dRadiusOfGyration.y);
	m_pChordLo->m_dGammaMin	= frCm(MIN(pChordLo->m_dRadiusOfGyrationMaxMin.x, pChordLo->m_dRadiusOfGyrationMaxMin.y));
	if(m_pChordLo->m_dGammaMin==0) m_pChordLo->m_dGammaMin=frCm(MIN(pChordLo->m_dRadiusOfGyration.x, pChordLo->m_dRadiusOfGyration.y));
	m_pChordLo->m_dLength	= m_dGirderSpace;
	m_pChordLo->m_dZy		= frCm3(pChordLo->m_dModulusOfSection.y);
	if(pChordLo->m_nType==CSteelMaterialData::CSTEEL)
		m_pChordLo->m_dZy		= frCm3(pChordLo->m_dModulusOfSection.x);

	m_pDiagonal->m_nType	= pDiagonal->m_nType;
	m_pDiagonal->m_dHeight	= pDiagonal->m_dH;
	m_pDiagonal->m_dWidth	= pDiagonal->m_dB;
	m_pDiagonal->m_dThick1	= pDiagonal->m_dT1;
	m_pDiagonal->m_dThick2	= pDiagonal->m_dT2;
	m_pDiagonal->m_dArea	= frCm2(pDiagonal->m_dSectionArea);
	m_pDiagonal->m_dGammaX	= frCm(pDiagonal->m_dRadiusOfGyration.x);
	m_pDiagonal->m_dGammaY	= frCm(pDiagonal->m_dRadiusOfGyration.y);
	m_pDiagonal->m_dGammaMin= frCm(MIN(pDiagonal->m_dRadiusOfGyrationMaxMin.x, pDiagonal->m_dRadiusOfGyrationMaxMin.y));
	if(m_pDiagonal->m_dGammaMin==0) m_pDiagonal->m_dGammaMin=frCm(MIN(pDiagonal->m_dRadiusOfGyration.x, pDiagonal->m_dRadiusOfGyration.y));
	m_pDiagonal->m_dLength	= sqrt(pow(m_dHeight,2) + pow(m_dGirderSpace/2.0,2));
	m_pDiagonal->m_dZy		= frCm3(pDiagonal->m_dModulusOfSection.y);
	if(pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
		m_pDiagonal->m_dZy		= frCm3(pDiagonal->m_dModulusOfSection.x);
}

void CDesignVBracing::SetLoadCalculate()
{
	CPlateBridgeApp		*pDB			= m_pAPlateCalcStd->GetBridge();
	CPlateGirderApp		*pGir			= m_pBx->GetGirder();
	CCentSeparation		*pSep			= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	CCalcData			*pCalcData		= m_pAPlateCalcStd->GetCalcData();
	CGlobarOption		*pGlobarOption	= m_pAPlateCalcStd->m_pDataManage->GetGlobalOption();
	CFEMManage		*pFEMManage		= m_pAPlateCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CResultArray		*pResult		= m_pAPlateCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	CFemStressCalc		StressCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFemModelingCalc	ModelCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);

	double	dStation			= m_pBx->GetStation();
	double	dHeightGirder		= m_pBx->GetHeightGirderByBx();
	double	dSlabThick			= pDB->m_dThickSlabBase;
	double	dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double	dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double	dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double	dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;

	m_strtWind.m_dWidthBridge	= pDB->m_dBridgeWidth;
	m_strtWind.m_dHeightBridge	= dHeightGirder+dSlabThick+dHeightGuardFence+dHeightSoundProof+dHeightHandRail;
	m_strtWind.m_dRateWidthHeight= m_strtWind.m_dWidthBridge/m_strtWind.m_dHeightBridge;

	if(m_strtWind.m_dRateWidthHeight>=1 && m_strtWind.m_dRateWidthHeight<8)
		m_strtWind.m_dWw	= (4.0-0.2*m_strtWind.m_dRateWidthHeight)*m_strtWind.m_dHeightBridge/1000.0;
	else if(m_strtWind.m_dRateWidthHeight>=8.0)
		m_strtWind.m_dWw = 2.4*m_strtWind.m_dHeightBridge/1000.0;
	else
		m_strtWind.m_dWw = 0;

	m_strtWind.m_dWwUse			= max(m_strtWind.m_dWw, 6.0);
	m_strtWind.m_dBracingSpace	= GetVBracingSpace();
	m_strtWind.m_dAllowCoeff	= 1.2;
	if(pDB->GetGirdersu()>=3)
		m_strtWind.m_dHw		= 1.0/2.0*m_strtWind.m_dWwUse*m_strtWind.m_dBracingSpace/m_strtWind.m_dAllowCoeff;
	else
		m_strtWind.m_dHw		= m_strtWind.m_dWwUse*m_strtWind.m_dBracingSpace/m_strtWind.m_dAllowCoeff;

	long	nQtyNode	= pFEMManage->GetJointSize();
	double	dRiskCoeff	= pGlobarOption->GetRiskCoefficient();
	double	dZoneCoeff	= pGlobarOption->GetZoneCoefficient();
	long	nQtyGirder	= pDB->GetGirdersu();

	// APLATE-654 지진시 고정하중 반력만 적용되어야 한다.
	m_strtEarthQuake.m_dReaction = 0;
	for(long nNode=0; nNode<nQtyNode; nNode++)
	{
		CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(nNode);
		double dPreDeadSteel	= pJResult->m_vPreDeadSteel[JOINT_REACTION].z;
		double dPreDeadSlab		= pJResult->m_vPreDeadSlab[JOINT_REACTION].z;
		double dPostDead		= pJResult->m_vPostDead[JOINT_REACTION].z;
		m_strtEarthQuake.m_dReaction += frkN(dPreDeadSteel + dPreDeadSlab + dPostDead);		
	}

	CResultArray	*pResultPost	= m_pAPlateCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
	for(long nNode=0; nNode<nQtyNode; nNode++)
	{
		CJointResult	*pJResult	= pResultPost->m_JointResultArr.GetAt(nNode);
		double dPostDead		= pJResult->m_vPostDead[JOINT_REACTION].z;
		m_strtEarthQuake.m_dReaction += frkN(dPostDead);		
	}

	m_strtEarthQuake.m_dLengthBridge	= pDB->GetLengthBridge();
	m_strtEarthQuake.m_dWd				= m_strtEarthQuake.m_dReaction/m_strtEarthQuake.m_dLengthBridge;
	m_strtEarthQuake.m_dAcceleration	= dRiskCoeff*dZoneCoeff;
	m_strtEarthQuake.m_dWe				= m_strtEarthQuake.m_dAcceleration*m_strtEarthQuake.m_dWd;
	m_strtEarthQuake.m_dAllowCoeff		= 1.5;
	m_strtEarthQuake.m_dHe				= 1.0/(nQtyGirder-1.0)*m_strtEarthQuake.m_dWe*m_strtWind.m_dBracingSpace/m_strtEarthQuake.m_dAllowCoeff;
	m_strtEarthQuake.m_dH				= max(m_strtWind.m_dHw, m_strtEarthQuake.m_dHe);
}

void CDesignVBracing::SetElementLoad()
{
	m_strtElementLoad.m_dN1			= -3.0/4.0 * m_strtEarthQuake.m_dH;
	m_strtElementLoad.m_dN2Tension	= 1.0/4.0 * m_strtEarthQuake.m_dH * 1.0/cos(m_dTheta);
	m_strtElementLoad.m_dN2Comp		= -1.0/4.0 * m_strtEarthQuake.m_dH * 1.0/cos(m_dTheta);
	m_strtElementLoad.m_dN3			= -1.0/2.0 * m_strtEarthQuake.m_dH;
}

void CDesignVBracing::SetDesignElement()
{
	CPlateGirderApp	*pGir		= m_pBx->GetGirder();
	CVBracing       *pVBracing	= m_pBx->GetSection()->GetVBracing();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CSteel			*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	CalcGen.GetMaterial(m_pBx, VB_UB, strLeft, strRight);

	CHGBaseSteel	*pSteel  = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBucklingUp(pSteel);
	CHGAllowStress	AllowShearUp(pSteel);
	

	// 상현재
	double	dThickGusset= pGir->GetThickJewon(VB_GU, m_pBx);
	
	AllowBucklingUp.SetAllowBucklingUnconsider(m_pChordUp->m_dLength, m_pChordUp->m_dGammaX);

	m_strtChordUp.m_strMaterial			= strLeft;
	if(m_pChordUp->m_nType==CSteelMaterialData::CTSTEEL)
	{	
		m_strtChordUp.m_sMaterialType.Format("CT-%.0fx%.0fx%.0fx%.0f", 
			m_pChordUp->m_dHeight,m_pChordUp->m_dWidth,m_pChordUp->m_dThick1,m_pChordUp->m_dThick2);
	}
	else if(m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
	{	
		m_strtChordUp.m_sMaterialType.Format("ㄷ-%.0fx%.0fx%.0fx%.0f", 
			m_pChordUp->m_dHeight,m_pChordUp->m_dWidth,m_pChordUp->m_dThick1,m_pChordUp->m_dThick2);
	}	
	else
	{
		m_strtChordUp.m_sMaterialType.Format("L-%.0fx%.0fx%.0f", 
			m_pChordUp->m_dHeight,m_pChordUp->m_dWidth,m_pChordUp->m_dThick1);
	}
	m_strtChordUp.m_dSlendernessRatio	= m_pChordUp->m_dLength/m_pChordUp->m_dGammaMin;
	m_strtChordUp.m_dFca		= AllowBucklingUp.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, m_pChordUp->m_dThick1);	// 국부좌굴을 고려하지 않은 허용축방향 압축응력
	m_strtChordUp.m_dFcaApo		= m_strtChordUp.m_dFca*(0.5+m_pChordUp->m_dLength/m_pChordUp->m_dGammaX/1000.0);
	m_strtChordUp.m_dFc			= fabs(m_strtElementLoad.m_dN1)/m_pChordUp->m_dArea;
	m_strtChordUp.m_dAllowShear	= AllowShearUp.GetAllowStress(ALLOW_SHEARSTRESS, m_pChordUp->m_dThick1);
	m_strtChordUp.m_dLengthWeld	= pVBracing->m_dULG*2.0;
	m_strtChordUp.m_dThroatDepth= (max(RoundUp(sqrt(2*max(m_pChordUp->m_dThick1, dThickGusset)), 0), 6));
	if(m_strtChordUp.m_dFc<0.75*m_strtChordUp.m_dFcaApo)
		m_strtChordUp.m_dN1Apo	= 0.75*m_pChordUp->m_dArea*m_strtChordUp.m_dFcaApo;
	else
		m_strtChordUp.m_dN1Apo	= m_strtElementLoad.m_dN1;
	m_strtChordUp.m_dShear		= m_strtChordUp.m_dN1Apo/(m_strtChordUp.m_dThroatDepth/sqrt(2.0)*m_strtChordUp.m_dLengthWeld);

	// 사재
	CalcGen.GetMaterial(m_pBx, VB_CR, strLeft, strRight);
	pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];

	CHGAllowStress	AllowBucklingDiagonal(pSteel);
	CHGAllowStress	AllowCompDiagonal(pSteel);
	CHGAllowStress	AllowShearDiagonal(pSteel);
	
	AllowBucklingDiagonal.SetAllowBucklingUnconsider(m_pDiagonal->m_dLength, m_pDiagonal->m_dGammaX);

	m_strtDiagonal.m_strMaterial= strLeft;
	if(m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
	{	
		m_strtDiagonal.m_sMaterialType.Format("CT-%.0fx%.0fx%.0fx%.0f", 
			m_pDiagonal->m_dHeight,m_pDiagonal->m_dWidth,m_pDiagonal->m_dThick1,m_pDiagonal->m_dThick2);
	}
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)//ㄷ 형강
	{	
		m_strtDiagonal.m_sMaterialType.Format("ㄷ-%.0fx%.0fx%.0fx%.0f", 
			m_pDiagonal->m_dHeight,m_pDiagonal->m_dWidth,m_pDiagonal->m_dThick1,m_pDiagonal->m_dThick2);
	}
	else
	{
		m_strtDiagonal.m_sMaterialType.Format("L-%.0fx%.0fx%.0f", 
			m_pDiagonal->m_dHeight,m_pDiagonal->m_dWidth,m_pDiagonal->m_dThick1);
	}
	m_strtDiagonal.m_dSlendernessRatio	= m_pDiagonal->m_dLength/m_pDiagonal->m_dGammaMin;
	m_strtDiagonal.m_dFca		= AllowBucklingDiagonal.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, m_pDiagonal->m_dThick1);
	m_strtDiagonal.m_dFcaApo	= m_strtDiagonal.m_dFca*(0.5+m_pDiagonal->m_dLength/m_pDiagonal->m_dGammaX/1000.0);
	m_strtDiagonal.m_dFta		= AllowCompDiagonal.GetAllowStress(ALLOW_TENSIONSTRESS, m_pDiagonal->m_dThick1);
	if(m_pDiagonal->m_nType==CSteelMaterialData::LSTEEL || m_pDiagonal->m_nType==CSteelMaterialData::LVALSTEEL)
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea-(m_pDiagonal->m_dWidth-m_pDiagonal->m_dThick1)/2*m_pDiagonal->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea-(m_pDiagonal->m_dWidth-m_pDiagonal->m_dThick2)/2*m_pDiagonal->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea/2;
	else
		m_strtDiagonal.m_dAn	= m_pDiagonal->m_dArea;
	m_strtDiagonal.m_dFt			= m_strtElementLoad.m_dN2Tension/m_strtDiagonal.m_dAn;
	m_strtDiagonal.m_dFc			= m_strtElementLoad.m_dN2Tension/m_pDiagonal->m_dArea;
	m_strtDiagonal.m_dAllowShear	= AllowShearDiagonal.GetAllowStress(ALLOW_SHEARSTRESS, m_pDiagonal->m_dThick1);
	m_strtDiagonal.m_dLengthWeld	= min(pVBracing->m_dC1LG, pVBracing->m_dC2LG)*2.0;
	
	double dS = m_pAPlateCalcStd->m_pDataManage->GetFilletWeldStandard(m_pDiagonal->m_dThick1, dThickGusset);
	m_strtDiagonal.m_dThroatDepth	= dS;
	if(m_strtDiagonal.m_dFc<0.75*m_strtDiagonal.m_dFcaApo)
		m_strtDiagonal.m_dN2Apo	= 0.75*m_pDiagonal->m_dArea*m_strtDiagonal.m_dFcaApo;
	else
		m_strtDiagonal.m_dN2Apo	= m_strtElementLoad.m_dN2Tension;
	m_strtDiagonal.m_dShear		= m_strtDiagonal.m_dN2Apo/(m_strtDiagonal.m_dThroatDepth/sqrt(2.0)*m_strtDiagonal.m_dLengthWeld);

	// 하현재
	CalcGen.GetMaterial(m_pBx, VB_LB, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBucklingLo(pSteel);
	CHGAllowStress	AllowShearLo(pSteel);
	
	AllowBucklingLo.SetAllowBucklingUnconsider(m_pChordLo->m_dLength, m_pChordLo->m_dGammaX);

	m_strtChordLo.m_strMaterial			= strLeft;
	if(m_pChordLo->m_nType==CSteelMaterialData::CTSTEEL)
	{	
		m_strtChordLo.m_sMaterialType.Format("CT-%.0fx%.0fx%.0fx%.0f", 
			m_pChordLo->m_dHeight,m_pChordLo->m_dWidth,m_pChordLo->m_dThick1,m_pChordLo->m_dThick2);
	}
	else if(m_pChordLo->m_nType==CSteelMaterialData::CSTEEL)
	{	
		m_strtChordLo.m_sMaterialType.Format("ㄷ-%.0fx%.0fx%.0fx%.0f", 
			m_pChordLo->m_dHeight,m_pChordLo->m_dWidth,m_pChordLo->m_dThick1,m_pChordLo->m_dThick2);
	}
	else
	{
		m_strtChordLo.m_sMaterialType.Format("L-%.0fx%.0fx%.0f", 
			m_pChordLo->m_dHeight,m_pChordLo->m_dWidth,m_pChordLo->m_dThick1);
	}
	m_strtChordLo.m_dSlendernessRatio	= m_pChordLo->m_dLength/m_pChordLo->m_dGammaMin;
	m_strtChordLo.m_dFca				= AllowBucklingLo.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, m_pChordLo->m_dThick1);
	m_strtChordLo.m_dFcaApo				= m_strtChordLo.m_dFca*(0.5+m_pChordLo->m_dLength/m_pChordLo->m_dGammaX/1000.0);
	if(m_pChordLo->m_nType==CSteelMaterialData::LSTEEL || m_pChordLo->m_nType==CSteelMaterialData::LVALSTEEL)
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea-(m_pChordLo->m_dWidth-m_pChordLo->m_dThick1)/2*m_pChordLo->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea-(m_pChordLo->m_dWidth-m_pChordLo->m_dThick2)/2*m_pChordLo->m_dThick1;
	else if(m_pDiagonal->m_nType==CSteelMaterialData::CSTEEL)
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea/2;
	else
		m_strtChordLo.m_dAn				= m_pChordLo->m_dArea;
	m_strtChordLo.m_dFc					= m_strtElementLoad.m_dN3/m_pChordLo->m_dArea;
	m_strtChordLo.m_dAllowShear			= AllowShearLo.GetAllowStress(ALLOW_SHEARSTRESS, m_pChordLo->m_dThick1);
	m_strtChordLo.m_dLengthWeld			= pVBracing->m_dDLG;
	
	dS = m_pAPlateCalcStd->m_pDataManage->GetFilletWeldStandard(m_pChordLo->m_dThick1, dThickGusset);
	m_strtChordLo.m_dThroatDepth		= dS;
	if(m_strtChordLo.m_dFc<0.75*m_strtChordLo.m_dFcaApo)
		m_strtChordLo.m_dN3Apo			= 0.75*m_pChordLo->m_dArea*m_strtChordLo.m_dFcaApo;
	else
		m_strtChordLo.m_dN3Apo			= m_strtElementLoad.m_dN3;
	m_strtChordLo.m_dShear				= m_strtChordLo.m_dN3Apo/(m_strtChordLo.m_dThroatDepth/sqrt(2.0)*m_strtChordLo.m_dLengthWeld);
}

void CDesignVBracing::SetGussetPlate()
{
	CVBracing		*pVBracing	= m_pBx->GetSection()->GetVBracing();
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CCalcData		*pCalcData	= m_pAPlateCalcStd->GetCalcData();
	CSteelBolt		*pSteelBolt	= m_pAPlateCalcStd->m_pDataManage->GetSteelBolt();
	CSteel			*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();

	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	CalcGen.GetMaterial(m_pBx, VB_GU, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBending(pSteel);
	CHGAllowStress	AllowShear(pSteel);

	// 상현재/사재 연결
	m_strtGussetUp.m_strMaterial	= strLeft;
	m_strtGussetUp.m_dHeight		= pVBracing->m_duL3 + pVBracing->m_duL4;
	m_strtGussetUp.m_dThick			= pVBracing->m_dT;
	m_strtGussetUp.m_dWidth			= pVBracing->m_duL1 + pVBracing->m_duL2;
	m_strtGussetUp.m_dAllowStress	= AllowBending.GetAllowStress(ALLOW_BENDINGCOMP_CASE1, m_strtGussetUp.m_dThick);
	m_strtGussetUp.m_dRh			= m_strtChordUp.m_dN1Apo+m_strtDiagonal.m_dN2Apo*cos(m_dTheta);
	m_strtGussetUp.m_dRv			= m_strtDiagonal.m_dN2Apo*sin(m_dTheta);
	m_strtGussetUp.m_dLoad			= sqrt(pow(m_strtGussetUp.m_dRh,2)+pow(m_strtGussetUp.m_dRv,2));

	CalcGen.GetMaterial(m_pBx, VB_BOLT, strLeft, strRight);
	m_strtGussetUp.m_strBoltMaterial	= strLeft+strRight;
	m_strtGussetUp.m_dDiaBolt			= GetBoltSize(pVBracing->m_ZzsBoltSize);

	CSteelBoltData *pBolt	= pSteelBolt->GetBolt(m_strtGussetUp.m_strBoltMaterial, m_strtGussetUp.m_dDiaBolt);
	double dHoleMargin		= 3;
	if(!pBolt)
	{
		if(m_strtGussetUp.m_dDiaBolt==20.0)			m_strtGussetUp.m_dBoltAllowStress	= 39000.0;
		else if(m_strtGussetUp.m_dDiaBolt==22.0)	m_strtGussetUp.m_dBoltAllowStress	= 48000.0;
		else										m_strtGussetUp.m_dBoltAllowStress	= 48000.0;
	}
	else
	{
		m_strtGussetUp.m_dBoltAllowStress	= pBolt->m_ForceAllow;
		dHoleMargin							= pBolt->m_HoleMargin;
	}

	m_strtGussetUp.m_dQtyBoltReq	= m_strtGussetUp.m_dLoad/m_strtGussetUp.m_dBoltAllowStress;
	m_strtGussetUp.m_dQtyBoltUse	= pVBracing->m_uB+1;
	m_strtGussetUp.m_dAn			= (m_strtGussetUp.m_dHeight - m_strtGussetUp.m_dQtyBoltUse*(m_strtGussetUp.m_dDiaBolt+dHoleMargin))*m_strtGussetUp.m_dThick;
	m_strtGussetUp.m_dStress		= m_strtGussetUp.m_dLoad/m_strtGussetUp.m_dAn;

	// 하현재
	CalcGen.GetMaterial(m_pBx, VB_GU, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowBending.SetBaseSteel(pSteel);

	m_strtGussetLo.m_strMaterial	= strLeft;
	m_strtGussetLo.m_dHeight		= pVBracing->m_ddL3 + pVBracing->m_ddL4;
	m_strtGussetLo.m_dThick			= pVBracing->m_dT;
	m_strtGussetLo.m_dWidth			= pVBracing->m_ddL1 + pVBracing->m_ddL2;
	m_strtGussetLo.m_dAllowStress	= AllowBending.GetAllowStress(ALLOW_BENDINGCOMP_CASE1, m_strtGussetDiagonal.m_dThick);
	m_strtGussetLo.m_dLoad			= m_strtChordLo.m_dN3Apo;
	CalcGen.GetMaterial(m_pBx, VB_BOLT, strLeft, strRight);
	m_strtGussetLo.m_strBoltMaterial= strLeft+strRight;
	m_strtGussetLo.m_dDiaBolt		= GetBoltSize(pVBracing->m_ZzsBoltSize);

	pBolt		= pSteelBolt->GetBolt(m_strtGussetLo.m_strBoltMaterial, m_strtGussetLo.m_dDiaBolt);
	dHoleMargin	= 3;
	if(!pBolt)
	{
		if(m_strtGussetLo.m_dDiaBolt==20.0)			m_strtGussetLo.m_dBoltAllowStress	= 39000.0;
		else if(m_strtGussetLo.m_dDiaBolt==22.0)	m_strtGussetLo.m_dBoltAllowStress	= 48000.0;
		else										m_strtGussetLo.m_dBoltAllowStress	= 48000.0;
	}
	else
	{
		m_strtGussetLo.m_dBoltAllowStress	= pBolt->m_ForceAllow;
		dHoleMargin							= pBolt->m_HoleMargin;
	}

	m_strtGussetLo.m_dQtyBoltReq	= m_strtGussetLo.m_dLoad/m_strtGussetLo.m_dBoltAllowStress;
	m_strtGussetLo.m_dQtyBoltUse	= pVBracing->m_dB+1;
	m_strtGussetLo.m_dAn			= (m_strtGussetLo.m_dHeight - m_strtGussetLo.m_dQtyBoltUse*(m_strtGussetLo.m_dDiaBolt+dHoleMargin))*m_strtGussetLo.m_dThick;
	m_strtGussetLo.m_dStress		= m_strtGussetLo.m_dLoad/m_strtGussetLo.m_dAn;

	// 사재/하현재 연결
	CalcGen.GetMaterial(m_pBx, VB_GU, strLeft, strRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowBending.SetBaseSteel(pSteel);
	AllowShear.SetBaseSteel(pSteel);

	m_strtGussetDiagonal.m_strMaterial	= strLeft;
	m_strtGussetDiagonal.m_dHeight		= pVBracing->m_dc1 + pVBracing->m_dc2;
	m_strtGussetDiagonal.m_dThick		= pVBracing->m_dT;
	m_strtGussetDiagonal.m_dWidth		= pVBracing->m_dc3*2;
	m_strtGussetDiagonal.m_dAllowStress	= AllowBending.GetAllowStress(ALLOW_BENDINGCOMP_CASE1, m_strtGussetDiagonal.m_dThick);
	m_strtGussetDiagonal.m_dAllowShear	= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, m_strtGussetDiagonal.m_dThick);
	m_strtGussetDiagonal.m_dLoad		= m_strtChordLo.m_dN3Apo;
	m_strtGussetDiagonal.m_dAllowShearWeld	= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, m_strtGussetDiagonal.m_dThick);
	m_strtGussetDiagonal.m_dLengthWeld	= m_strtGussetDiagonal.m_dWidth*2;
	double dS = m_pAPlateCalcStd->m_pDataManage->GetFilletWeldStandard(m_pDiagonal->m_dThick1, m_strtGussetDiagonal.m_dThick);
	m_strtGussetDiagonal.m_dThroatDepth	= dS;
	m_strtGussetDiagonal.m_dShearWeld	= m_strtGussetDiagonal.m_dLoad/(m_strtGussetDiagonal.m_dThroatDepth/sqrt(2.0)*m_strtGussetDiagonal.m_dLengthWeld);
	m_strtGussetDiagonal.m_dAg			= m_strtGussetDiagonal.m_dWidth*m_strtGussetDiagonal.m_dThick;
	m_strtGussetDiagonal.m_dShear		= m_strtGussetDiagonal.m_dLoad/m_strtGussetDiagonal.m_dAg;
}

double CDesignVBracing::GetVBracingSpace()
{
	CPlateBridgeApp		*pDB		= m_pAPlateCalcStd->GetBridge();
	CPlateGirderApp		*pGir		= m_pBx->GetGirder();
	CPlateBasicIndex	*pBxPre		= NULL;
	CPlateBasicIndex	*pBxPost	= NULL;

	long	nCurVBracing= pGir->GetVBracingPosByBx(m_pBx);
	long	nTotalVBr	= pGir->GetQtyVBracing();

	if(m_pBx==pGir->GetBxOnJijum(0))	//처음 수직브레이싱
	{
		pBxPre  = NULL;
		pBxPost = pGir->GetBxByVBracingPos(1);
	}
	else if(m_pBx==pGir->GetBxOnJijum(pDB->m_nQtyJigan))	//마지막 수직브레이싱
	{
		pBxPre  = pGir->GetBxByVBracingPos(nTotalVBr-2);
		pBxPost = NULL;
	}
	else
	{	
		pBxPre  = pGir->GetBxByVBracingPos(nCurVBracing-1);
		pBxPost = pGir->GetBxByVBracingPos(nCurVBracing+1);
	}
	double dVBracingPre  = pBxPre ? fabs(pBxPre->GetStation()-m_pBx->GetStation()) : 0;
	double dVBracingPost = pBxPost? fabs(pBxPost->GetStation()-m_pBx->GetStation()) : 0;

	return (dVBracingPre+dVBracingPost)/2;
}

//////////////////////////////////////////////////////////////////////////
// 가로보 이음
CDesignCrossBeamJoint::CDesignCrossBeamJoint(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
	m_pUpper			= new CDesignFlangeJoint(pAPlateCalcStd, pBx);
	m_pLower			= new CDesignFlangeJoint(pAPlateCalcStd, pBx);
	m_pWebLeft			= new CDesignWebJoint(pAPlateCalcStd, pBx);
	m_pWebRight			= new CDesignWebJoint(pAPlateCalcStd, pBx);

	CSection	*pSec	= m_pBx->GetSection();
	if(pSec->GetenDetType()==SECDET_VBRACING)
		return;

	SetCrossBeamJoint();
}

CDesignCrossBeamJoint::~CDesignCrossBeamJoint()
{
	delete m_pUpper;
	delete m_pLower;
	delete m_pWebLeft;
	delete m_pWebRight;
}

void CDesignCrossBeamJoint::SetCrossBeamJoint()
{
	CPlateCrossBeam		*pCrossBeam	= m_pBx->GetCrossBeam();

	long nType = 0;	
	long LType = 0;
	long RType = 0;
	
	if(pCrossBeam->m_bConnectionPlate)	
		nType = CROSSBEAMJOINT_GUSSETTYPE;
	else
		nType = CROSSBEAMJOINT_PLATETYPE;
	
	if (pCrossBeam->m_nLGirSpType)//좌측이음판 있으면 기본방식
		LType = CROSSBEAMWEB_USEJOINTTYPE;
	else
		LType = CROSSBEAMWEB_NOTJOINTTYPE;

	if (pCrossBeam->m_nRGirSpType)
		RType = CROSSBEAMWEB_USEJOINTTYPE;
	else
		RType = CROSSBEAMWEB_NOTJOINTTYPE;

	m_pUpper->m_nType		= nType;
	m_pUpper->m_bIsUpper	= TRUE;
	m_pLower->m_nType		= nType;
	m_pLower->m_bIsUpper	= FALSE;
	m_pWebLeft->m_nType		= LType;
	m_pWebRight->m_nType	= RType;

	m_pUpper->SetFlangeJoint();
	m_pLower->SetFlangeJoint();
	if (nType == CROSSBEAMJOINT_GUSSETTYPE)
	{
		m_pWebLeft->m_strtPlate.m_dF1	= m_pLower->m_strtGusset.m_dDesignStress;
		m_pWebRight->m_strtPlate.m_dF1	= m_pLower->m_strtGusset.m_dDesignStress;
	}
	else
	{
		m_pWebLeft->m_strtPlate.m_dF1	= m_pLower->m_strtPlate.m_dDesignStress;	
		m_pWebRight->m_strtPlate.m_dF1	= m_pLower->m_strtPlate.m_dDesignStress;
	}
	
	m_pWebLeft->SetWebJoint();	
	m_pWebRight->SetWebJoint();
}

//////////////////////////////////////////////////////////////////////////
// 가로보 플랜지 이음
CDesignFlangeJoint::CDesignFlangeJoint(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
}

CDesignFlangeJoint::~CDesignFlangeJoint()
{

}

void CDesignFlangeJoint::SetFlangeJoint()
{
	if(m_nType==CROSSBEAMJOINT_GUSSETTYPE)		SetGussetType();
	else if(m_nType==CROSSBEAMJOINT_PLATETYPE)	SetPlateType();
}

void CDesignFlangeJoint::SetGussetType()
{
	CPlateCrossBeam		*pCrossBeam	= m_pBx->GetCrossBeam();
	CCalcGeneral		CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CDesignCrossBeam	CrossBeam(m_pAPlateCalcStd, m_pBx);
	
	CDesignEndCrossBeam		*pEndCr	= (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CDesignCenterCrossBeam	*pCenCr	= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CSteelBolt				*pSteelBolt = m_pAPlateCalcStd->m_pDataManage->GetSteelBolt();
	CSteel					*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();

	BOOL	bIsJijum	= m_pBx->IsJijum();
	CString	strLeft		= _T("");
	CString	strRight	= _T("");

	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_F_L, strLeft, strRight);
	else			CalcGen.GetMaterial(m_pBx, CG_F_L, strLeft, strRight);

	CHGBaseSteel		*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress		AllowTension(pSteel);

	
	m_strtGusset.m_strMaterial		= strLeft;
	m_strtGusset.m_sRightMaterial	= strRight;				//강종 오른쪽 이름

	if(m_bIsUpper)//상부 플랜지면
	{
		m_strtGusset.m_dThick	= pCrossBeam->m_T1_CR;
		m_strtGusset.m_dWidth	= pCrossBeam->m_uM1 * 2;
		m_strtGusset.m_dBoltUse = (pCrossBeam->m_uB+1)*(pCrossBeam->m_uE+1)*2;//가혹조건 개념으로한쪽만..
		m_strtGusset.m_nQtyLine = (long)(pCrossBeam->m_uB+1);		// 사용열수

		m_strtGusset.m_nTopConEA = 1;								// 상면 연결판 갯수
		if(pCrossBeam->m_nRadioConnectionPlateInOut==0)				// 내외측적용		
			m_strtGusset.m_nBottomConEA = 2;						// 하면 연결판 갯수		
		else
			m_strtGusset.m_nBottomConEA = 0;		
		
		m_strtGusset.m_dTopWidth = pCrossBeam->m_uM1 * 2;
		m_strtGusset.m_dBottomWidth = pCrossBeam->m_uW;
		m_strtGusset.m_dTR = (pCrossBeam->m_uM1-pCrossBeam->m_uW)*2;
	}
	else//하부 플랜지면...
	{
		m_strtGusset.m_dThick	= pCrossBeam->m_T2_CR;
		m_strtGusset.m_dWidth	= pCrossBeam->m_dM1 * 2;
		m_strtGusset.m_dBoltUse = (pCrossBeam->m_dB+1)*(pCrossBeam->m_dE+1)*2;
		m_strtGusset.m_nQtyLine = (long)(pCrossBeam->m_dB+1);		// 사용열수
		
		m_strtGusset.m_nBottomConEA = 1;					// 하면 연결판 갯수

		if(pCrossBeam->m_nRadioConnectionPlateInOut==0)		// 내외측적용		
			m_strtGusset.m_nTopConEA = 2;					// 상면 연결판 갯수			
		else		
			m_strtGusset.m_nTopConEA = 0;					// 상면 연결판 갯수
		
		if(pCrossBeam->m_cGasset_CR) m_strtGusset.m_nBottomConEA = 2;//브라켓 보강판이 있으면..

		if (m_strtGusset.m_nTopConEA == 1)
			m_strtGusset.m_dTopWidth = pCrossBeam->m_dM1 * 2;
		else
			m_strtGusset.m_dTopWidth = pCrossBeam->m_dW;

		if (m_strtGusset.m_nBottomConEA == 1)
			m_strtGusset.m_dBottomWidth = pCrossBeam->m_dM1 * 2;
		else
			m_strtGusset.m_dBottomWidth = pCrossBeam->m_dW;

		m_strtGusset.m_dTR = (pCrossBeam->m_dM1-pCrossBeam->m_dW)*2;
	}


	m_strtGusset.m_dStress			= CrossBeam.GetFlangeStress(m_bIsUpper);							// 작용응력
	m_strtGusset.m_dAllowStress		= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, m_strtGusset.m_dThick);		// 허용응력
	m_strtGusset.m_dDesignStress	= max(m_strtGusset.m_dStress, 0.75*m_strtGusset.m_dAllowStress);	// 사용응력		
	m_strtGusset.m_dAreaFlange		= m_strtGusset.m_dThick*m_strtGusset.m_dWidth;						// 플랜지 단면적
	m_strtGusset.m_dAxialLoad		= m_strtGusset.m_dDesignStress*(m_strtGusset.m_dAreaFlange/1000);	// 설계축력 kN
	
	if(pCrossBeam->m_nRadioConnectionPlateInOut==0)		// 내외측 적용
		m_strtGusset.m_nQtySide = 2;					// 1면마찰, 2면마찰
	else
		m_strtGusset.m_nQtySide = 1;	
	
	m_strtGusset.m_dTopThick = pCrossBeam->m_T2_CR;
	m_strtGusset.m_dBottomThick = pCrossBeam->m_T2_CR;

	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_BOLT, strLeft, strRight);
	else			CalcGen.GetMaterial(m_pBx, CG_BOLT, strLeft, strRight);
	m_strtGusset.m_strBoltMaterial		= strLeft+strRight;	
	m_strtGusset.m_dDiaBolt				= GetBoltSize(pCrossBeam->m_ZzsBoltSize);

	CSteelBoltData *pBolt		= pSteelBolt->GetBolt(m_strtGusset.m_strBoltMaterial, m_strtGusset.m_dDiaBolt);
	if(!pBolt)
	{
		if(m_strtGusset.m_dDiaBolt==20.0)		m_strtGusset.m_dAllowBolt	= 39000.0;
		else if(m_strtGusset.m_dDiaBolt==22.0)	m_strtGusset.m_dAllowBolt	= 48000.0;
		else									m_strtGusset.m_dAllowBolt	= 48000.0;
	}
	else	
		m_strtGusset.m_dAllowBolt	= pBolt->m_ForceAllow;

	if (m_strtGusset.m_nQtySide == 1)//1면 마찰이면
	{
		m_strtGusset.m_dSumAs = m_strtGusset.m_nTopConEA*m_strtGusset.m_dTopWidth*m_strtGusset.m_dTopThick;
		m_strtGusset.m_dAsn	  = m_strtGusset.m_dSumAs - (25*m_strtGusset.m_nQtyLine+m_strtGusset.m_dTR)*m_strtGusset.m_dTopThick; 
	}
	else//2면 마찰이면
	{
		m_strtGusset.m_dAllowBolt = m_strtGusset.m_dAllowBolt*2;
		m_strtGusset.m_dSumAs = (m_strtGusset.m_nTopConEA*m_strtGusset.m_dTopWidth*m_strtGusset.m_dTopThick)
					+(m_strtGusset.m_nBottomConEA*m_strtGusset.m_dBottomWidth*m_strtGusset.m_dBottomThick);
		m_strtGusset.m_dAsn	  = m_strtGusset.m_dSumAs - ((25*m_strtGusset.m_nQtyLine+m_strtGusset.m_dTR)*m_strtGusset.m_dTopThick
					+(25*m_strtGusset.m_nQtyLine*m_strtGusset.m_dBottomThick));
	}
	m_strtGusset.m_dBoltReq = m_strtGusset.m_dAxialLoad/(m_strtGusset.m_dAllowBolt/1000);	// 필요볼트수 p/pa
	
	m_strtGusset.m_dConStress = (m_strtGusset.m_dAxialLoad*1000) / m_strtGusset.m_dAsn;			// 이음판 작용응력 
}
	

void CDesignFlangeJoint::SetPlateType()
{
	CPlateCrossBeam		*pCrossBeam	= m_pBx->GetCrossBeam();
	CCalcGeneral		CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CDesignCrossBeam	CrossBeam(m_pAPlateCalcStd, m_pBx);
	
	CDesignEndCrossBeam		*pEndCr	= (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CDesignCenterCrossBeam	*pCenCr	= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CSteelBolt				*pSteelBolt = m_pAPlateCalcStd->m_pDataManage->GetSteelBolt();
	CSection				*pSec		= m_pBx->GetSection();
	CSteel					*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	BOOL	bIsJijum		= m_pBx->IsJijum();
	CString	strLeft			= _T("");
	CString	strRight		= _T("");

	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_F_L, strLeft, strRight);
	else			CalcGen.GetMaterial(m_pBx, CG_F_L, strLeft, strRight);

	CHGBaseSteel		*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress		AllowTension(pSteel);

	if(m_bIsUpper)//상부 플랜지면
	{
		m_strtPlate.m_dThick	= pCrossBeam->m_T2_CR;
		m_strtPlate.m_dWidth	= pCrossBeam->m_uM_CR * 2;		
		m_strtPlate.m_dL1		= pCrossBeam->m_uW;
	}
	else//하부 플랜지면...
	{
		m_strtPlate.m_dThick	= pCrossBeam->m_T1_CR;
		m_strtPlate.m_dWidth	= pCrossBeam->m_dM_CR * 2;
		m_strtPlate.m_dL1		= pCrossBeam->m_dW;
	}
	
	m_strtPlate.m_strMaterial		= strLeft;
	m_strtPlate.m_dStress			= CrossBeam.GetFlangeStress(m_bIsUpper);
	m_strtPlate.m_dAllowStress		= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, m_strtPlate.m_dThick);
	m_strtPlate.m_dDesignStress		= max(m_strtPlate.m_dStress, 0.75*m_strtPlate.m_dAllowStress);
	m_strtPlate.m_dAf				= m_strtPlate.m_dThick*m_strtPlate.m_dWidth;
	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_BOLT, strLeft, strRight);
	else			CalcGen.GetMaterial(m_pBx, CG_BOLT, strLeft, strRight);
	m_strtPlate.m_strBoltMaterial	= strLeft+strRight;
	m_strtPlate.m_sLeftBoltMaterial = strLeft;
	m_strtPlate.m_dDiaBolt			= GetBoltSize(pCrossBeam->m_ZzsBoltSize);

	CSteelBoltData *pBolt	= pSteelBolt->GetBolt(m_strtPlate.m_strBoltMaterial, m_strtPlate.m_dDiaBolt);
	double dHoleMargin		= 3;
	long nSide				= 0;
	if(!pBolt)
	{
		if(m_strtPlate.m_dDiaBolt==20.0)		m_strtPlate.m_dAllowBolt	= 39000.0;
		else if(m_strtPlate.m_dDiaBolt==22.0)	m_strtPlate.m_dAllowBolt	= 48000.0;
		else									m_strtPlate.m_dAllowBolt	= 48000.0;
		nSide = 1;
	}
	else
	{
		m_strtPlate.m_dAllowBolt	= pBolt->m_ForceAllow;
		dHoleMargin					= pBolt->m_HoleMargin;
		nSide = 2;
	}

	m_strtPlate.m_dQtyBoltReq	= m_strtPlate.m_dAf*m_strtPlate.m_dDesignStress/(m_strtPlate.m_dAllowBolt*nSide);
	m_strtPlate.m_dQtyBoltUse	= (pCrossBeam->m_dB+1) * (pCrossBeam->m_dE+1) * 2;
	m_strtPlate.m_nQtyLine		= (long)(pCrossBeam->m_dB+1);
	m_strtPlate.m_dTw			= pCrossBeam->m_T3_CR;
	m_strtPlate.m_dA1			= pCrossBeam->m_dC;
	m_strtPlate.m_dA2			= pCrossBeam->m_dA;
	m_strtPlate.m_dA3			= pCrossBeam->m_uC1;
	// Splice 두께 산정
	m_strtPlate.m_dTsUse		= m_bIsUpper ? pCrossBeam->m_uT : pCrossBeam->m_dT;
	m_strtPlate.m_dTsCoeff		= m_strtPlate.m_dWidth-(m_strtPlate.m_dDiaBolt+dHoleMargin)*(m_strtPlate.m_dQtyBoltUse/m_strtPlate.m_nQtyLine)+
								  2.0*(m_strtPlate.m_dL1-(m_strtPlate.m_dDiaBolt+dHoleMargin)*(m_strtPlate.m_dQtyBoltUse/m_strtPlate.m_nQtyLine)/2);
	m_strtPlate.m_dTsMin		= m_strtPlate.m_dAf/m_strtPlate.m_dTsCoeff;
	// 연결판
	m_strtPlate.m_dSumAs		= m_strtPlate.m_dWidth*m_strtPlate.m_dTsUse + 2*m_strtPlate.m_dL1*m_strtPlate.m_dTsUse;
	if(m_bIsUpper)	
	{

		//수직보강재 연결인경우 2, 그외의 경우 1
		if (pCrossBeam->GetType() == CR_TYPE_VSTIFF)
			m_strtPlate.m_nTopConEA = 2;//
		else
			m_strtPlate.m_nTopConEA = 1;//

		m_strtPlate.m_nBottomConEA=2;//고정
		m_strtPlate.m_dAsn	= m_strtPlate.m_dSumAs;
	}
	else
	{

		m_strtPlate.m_nTopConEA = 2;//고정
		
		//브라켓보강판이 있으면 2, 없으면 1
		if(pCrossBeam->m_cGasset_CR)
			m_strtPlate.m_nBottomConEA=2;
		else
			m_strtPlate.m_nBottomConEA=1;

		m_strtPlate.m_dAsn	= m_strtPlate.m_dSumAs-m_strtPlate.m_dTsUse*(m_strtPlate.m_dDiaBolt+dHoleMargin)*(m_strtPlate.m_dQtyBoltUse/m_strtPlate.m_nQtyLine*2);
	}
	// Bolt 응력검토
	m_strtPlate.m_dShear		= CrossBeam.GetFlangeShear();//볼트의 작용응력
	m_strtPlate.m_dQ1			= pCrossBeam->m_H_CR/2+pCrossBeam->m_T1_CR/2;
	m_strtPlate.m_dQ			= m_strtPlate.m_dWidth*m_strtPlate.m_dThick*(m_strtPlate.m_dQ1-m_strtPlate.m_dThick/2);
	m_strtPlate.m_dI			= 0;

	if(pSec->IsenType(SECTION_A))
		m_strtPlate.m_dI	= pEndCr->m_strtSection.m_dI;
	else
		m_strtPlate.m_dI	= pCenCr->m_strtSection.m_dI;

	m_strtPlate.m_dRhop			= m_strtPlate.m_dThick*m_strtPlate.m_dWidth*m_strtPlate.m_dDesignStress/m_strtPlate.m_dQtyBoltUse;
	m_strtPlate.m_dRhoh			= frkN(m_strtPlate.m_dShear)*m_strtPlate.m_dQ*(m_strtPlate.m_dWidth/(m_strtPlate.m_dQtyBoltUse/m_strtPlate.m_nQtyLine))/(m_strtPlate.m_dI*m_strtPlate.m_nQtyLine);
	m_strtPlate.m_dRho			= sqrt(pow(m_strtPlate.m_dRhop,2)+pow(m_strtPlate.m_dRhoh,2));
	// 모재의 응력검토
	m_strtPlate.m_dAreaFlange	= m_strtPlate.m_dWidth * m_strtPlate.m_dThick;
	m_strtPlate.m_dAsnFlange	= m_strtPlate.m_dThick*(m_strtPlate.m_dWidth-(m_strtPlate.m_dDiaBolt+dHoleMargin)*m_strtPlate.m_dQtyBoltUse/m_strtPlate.m_nQtyLine);
	m_strtPlate.m_dStressFlange	= m_strtPlate.m_dAreaFlange*m_strtPlate.m_dDesignStress/m_strtPlate.m_dAsnFlange;
}

//////////////////////////////////////////////////////////////////////////
// 가로보 복부 이음
CDesignWebJoint::CDesignWebJoint(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
}

CDesignWebJoint::~CDesignWebJoint()
{
	
}

void CDesignWebJoint::SetWebJoint()
{
	if(m_nType==CROSSBEAMWEB_NOTJOINTTYPE)
		SetGussetType();
	else if(m_nType==CROSSBEAMWEB_USEJOINTTYPE)
		SetPlateType();
}

void CDesignWebJoint::SetGussetType()
{
	CPlateCrossBeam			*pCrossBeam	= m_pBx->GetCrossBeam();
	
	CDesignCrossBeam		CrossBeam(m_pAPlateCalcStd, m_pBx);
	CCalcGeneral			CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CDesignEndCrossBeam		*pEndCr	= (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CDesignCenterCrossBeam	*pCenCr	= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CSteelBolt				*pSteelBolt = m_pAPlateCalcStd->m_pDataManage->GetSteelBolt();
	CSteel					*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CFemModelingCalc		ModelCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFemStressCalc			StressCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);

	BOOL	bIsJijum	= m_pBx->IsJijum();
	CString	strLeft		= _T("");
	CString	strRight	= _T("");	
	long	nElement	= (long)ModelCalc.GetElemByCrossBx(m_pBx);
	BOOL	bPositive	= StressCalc.IsPositive(ELE_MOMENT3, m_pBx->GetNumberGirder(), m_pBx->GetStation());
	
	if(nElement < 0)
		return;

	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_W, strLeft, strRight);
	else			CalcGen.GetMaterial(m_pBx, CG_W, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowTension(pSteel);
	CHGAllowStress	AllowShear(pSteel);

	m_strtGusset.m_strMaterial = strLeft;

	if(bPositive)
		m_strtGusset.m_dApplyLoad	= frkN(StressCalc.GetOneEleMaxForce(ELE_SHEAR_2, LC_SUM_MAX, nElement));//
	else
		m_strtGusset.m_dApplyLoad	= frkN(StressCalc.GetOneEleMaxForce(ELE_SHEAR_2, LC_SUM_MIN, nElement));//frkN


	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_BOLT, strLeft, strRight);
	else			CalcGen.GetMaterial(m_pBx, CG_BOLT, strLeft, strRight);

	m_strtGusset.m_strBoltMaterial	= strLeft+strRight;
	m_strtGusset.m_sLeftBoltMaterial = strLeft;	
	m_strtGusset.m_dDiaBolt	= GetBoltSize(pCrossBeam->m_ZzsBoltSize);

	CSteelBoltData *pBolt		= pSteelBolt->GetBolt(m_strtGusset.m_strBoltMaterial, m_strtGusset.m_dDiaBolt);
	if(!pBolt)
	{
		if(m_strtGusset.m_dDiaBolt==20.0)		m_strtGusset.m_dAllowBolt	= 39000.0;
		else if(m_strtGusset.m_dDiaBolt==22.0)	m_strtGusset.m_dAllowBolt	= 48000.0;
		else									m_strtGusset.m_dAllowBolt	= 48000.0;
	}
	else	
		m_strtGusset.m_dAllowBolt	= pBolt->m_ForceAllow;
	
	if (pCrossBeam->m_cGasset_CR) //브라켓 보강판이 있으면
		if(pCrossBeam->m_nLGirSpType)
			m_strtGusset.m_dBoltUse = pCrossBeam->m_wE+1 + pCrossBeam->m_ltE+1;	// 사용볼트수
		else
			m_strtGusset.m_dBoltUse = pCrossBeam->m_wE+1 + pCrossBeam->m_rtE+1;	// 사용볼트수
	else
		m_strtGusset.m_dBoltUse = pCrossBeam->m_wE+1;						// 사용볼트수

	m_strtGusset.m_dBoltReq = (m_strtGusset.m_dApplyLoad/1000)/(m_strtGusset.m_dAllowBolt/1000);//S/pa
}

void CDesignWebJoint::SetPlateType()
{
	CPlateCrossBeam			*pCrossBeam	= m_pBx->GetCrossBeam();
	CDesignCrossBeam		CrossBeam(m_pAPlateCalcStd, m_pBx);
	CCalcGeneral			CalcGen(m_pAPlateCalcStd->m_pDataManage);
	CDesignEndCrossBeam		*pEndCr	= (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CDesignCenterCrossBeam	*pCenCr	= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CSteelBolt				*pSteelBolt = m_pAPlateCalcStd->m_pDataManage->GetSteelBolt();
	CSteel					*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CSection				*pSec	= m_pBx->GetSection();

	double	dTop		= (pCrossBeam->m_H_CR-(pCrossBeam->m_sE * pCrossBeam->m_sF))/2.0;
	double	dMid		= pCrossBeam->m_sE*pCrossBeam->m_sF;
	m_strtPlate.m_dsE	= pCrossBeam->m_sE;
	m_strtPlate.m_dsF	= pCrossBeam->m_sF;
	m_strtPlate.m_dTop	= dTop;
	double	dHeight		= 0;
	double	dShear		= CrossBeam.GetFlangeShear();
	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	BOOL	bIsJijum	= m_pBx->IsJijum();

	m_strtPlate.m_dApplyLoad = dShear;
	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_W, strLeft, strRight);
	else			CalcGen.GetMaterial(m_pBx, CG_W, strLeft, strRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowTension(pSteel);
	CHGAllowStress	AllowShear(pSteel);

	m_strtPlate.m_strMaterial		= strLeft;

	double	dAllowStress		= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, pCrossBeam->m_T1_CR);
	m_strtPlate.m_dAllowTension = dAllowStress;
	m_strtPlate.m_dStress		= CrossBeam.GetFlangeStress(FALSE);//하부플랜지 작용응력
	
	double dA1			= pCrossBeam->m_dC;
	double dA2			= pCrossBeam->m_dA;
	double dA3			= pCrossBeam->m_uC1;

	m_strtPlate.m_dA	= (2*dA1+dA2)*2+dA3;
	m_strtPlate.m_dA2	= dA2;

	m_strtPlate.m_dTw	= pCrossBeam->m_T3_CR;
	m_strtPlate.m_nB1	= pCrossBeam->m_sB;
	m_strtPlate.m_dB1	= pCrossBeam->m_sC;
	m_strtPlate.m_dB2	= pCrossBeam->m_sC1;
	m_strtPlate.m_dB	= pCrossBeam->m_sA;
	m_strtPlate.m_dHo	= (dMid + 2.0*dTop)/2.0;
	dHeight				= (dTop+m_strtPlate.m_dsF/2);
	m_strtPlate.m_dF2	= m_strtPlate.m_dF1*(m_strtPlate.m_dHo-dHeight)/m_strtPlate.m_dHo;
	dHeight += pCrossBeam->m_sF;
	m_strtPlate.m_dF3	= m_strtPlate.m_dF1*(m_strtPlate.m_dHo-dHeight)/m_strtPlate.m_dHo;
	dHeight += pCrossBeam->m_sF;
	m_strtPlate.m_dF4	= m_strtPlate.m_dF1*(m_strtPlate.m_dHo-dHeight)/m_strtPlate.m_dHo;
	m_strtPlate.m_dRhom	= (m_strtPlate.m_dF1+m_strtPlate.m_dF2)/2.0*m_strtPlate.m_dTw*(dTop+m_strtPlate.m_dsF/2)/(pCrossBeam->m_sB+1.0);// 제1열 pm1
	m_strtPlate.m_dRhos	= dShear*1000/((pCrossBeam->m_sE+1.0)*(pCrossBeam->m_sB+1.0));//ps =========> dShear 라운드 문제 체크할것 !!!!!!!!
	m_strtPlate.m_dRho	= sqrt(pow(m_strtPlate.m_dRhom,2)+pow(m_strtPlate.m_dRhos,2));//합성응력 p

	dHeight	= pCrossBeam->m_sE*pCrossBeam->m_sF + 2*dTop;
	double	dH					= (dHeight-pCrossBeam->m_sW)/2.0;
	double	dYsu				= dHeight/2.0-dH;
	m_strtPlate.m_dDiaBolt = GetBoltSize(pCrossBeam->m_ZzsBoltSize);

	CString strBoltL, strBoltR;
	if(bIsJijum)	CalcGen.GetMaterial(m_pBx, CP_BOLT, strBoltL, strBoltR);
	else			CalcGen.GetMaterial(m_pBx, CG_BOLT, strBoltL, strBoltR);

	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	AllowTension.SetBaseSteel(pSteel);
	AllowShear.SetBaseSteel(pSteel);

	CString strBoltMaterial	= strBoltL+strBoltR;

	CSteelBoltData *pBolt		= pSteelBolt->GetBolt(strBoltMaterial, m_strtPlate.m_dDiaBolt);
	double dHoleMargin			= 3;
	if(!pBolt)
	{
		if(m_strtPlate.m_dDiaBolt==20.0)		m_strtPlate.m_dAllowBolt	= 39000.0;
		else if(m_strtPlate.m_dDiaBolt==22.0)	m_strtPlate.m_dAllowBolt	= 48000.0;
		else									m_strtPlate.m_dAllowBolt	= 48000.0;
	}
	else	
	{
		m_strtPlate.m_dAllowBolt	= pBolt->m_ForceAllow;
		dHoleMargin					= pBolt->m_HoleMargin;
	}

	double	dI					= 0;
	if(pSec->IsenType(SECTION_A))
		dI	= pEndCr->m_strtSection.m_dI;
	else
		dI	= pCenCr->m_strtSection.m_dI;

	m_strtPlate.m_dsT1			= dH;
	m_strtPlate.m_dsT2			= m_strtPlate.m_dTop - dH;
	m_strtPlate.m_dI			= dI;
	m_strtPlate.m_dTsCoeff		= dH-(m_strtPlate.m_dDiaBolt+dHoleMargin)*(pCrossBeam->m_sE+1.0)*2.0;//Asn
	m_strtPlate.m_dTsMin		= m_strtPlate.m_dTw*dHeight/m_strtPlate.m_dTsCoeff;//ts
	m_strtPlate.m_dTsUse		= pCrossBeam->m_sT;
	m_strtPlate.m_dMoment		= tokNM(CrossBeam.GetFlangeMoment());
	m_strtPlate.m_dIw			= m_strtPlate.m_dTw*pow(dHeight,3)/12.0;
	m_strtPlate.m_dMw			= m_strtPlate.m_dMoment*m_strtPlate.m_dIw/dI;
	m_strtPlate.m_Is			= 2.0*m_strtPlate.m_dTsUse*pow(dH,3)/12.0;
	m_strtPlate.m_dFMax			= m_strtPlate.m_dMw*dYsu/m_strtPlate.m_Is;
	m_strtPlate.m_dSumAs		= 2*dHeight*m_strtPlate.m_dTsUse;						//사용단면적
	m_strtPlate.m_dReqArea		= m_strtPlate.m_dTw*(dMid+m_strtPlate.m_dTop*2);	    //필요단면적
	m_strtPlate.m_dAllowShear	= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, pCrossBeam->m_T2_CR);//전단허용응력
	m_strtPlate.m_dCompStress	= pow(m_strtPlate.m_dFMax/dAllowStress,2)+pow(m_strtPlate.m_dShear/m_strtPlate.m_dAllowShear,2);
}

//////////////////////////////////////////////////////////////////////////
// 수평브레이싱
CDesignHorBracing::CDesignHorBracing(CAPlateCalcStd *pAPlateCalcStd, long nG, long nIdxBracing)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_nG				= nG;
	m_nIndex			= nIdxBracing;
	m_dWw				= 0;
	m_dWe				= 0;
	m_pBracingJewon		= new CBracingJewon();
	SetHorBracing();
}

CDesignHorBracing::~CDesignHorBracing()
{
	delete m_pBracingJewon;
}

void CDesignHorBracing::SetHorBracing()
{
	CPlateBridgeApp		*pDB		= m_pAPlateCalcStd->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(m_nG);
	CCalcData			*pCalcData	= m_pAPlateCalcStd->GetCalcData();
	CCentSeparation		*pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	CGlobarOption		*pGlobarOpt	= m_pAPlateCalcStd->m_pDataManage->GetGlobalOption();
	CFEMManage			*pFEMManage	= m_pAPlateCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemStressCalc		StressCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CFemModelingCalc	ModelCalc(m_pAPlateCalcStd->m_pDataManage, m_pAPlateCalcStd);
	CBracing			*pHBracing	= pGir->GetBracingInTotal(m_nIndex);
	CPlateBasicIndex	*pBx		= pHBracing->GetBx();
	CPlateBasicIndex	*pBxMid		= pBx->GetBxNext(BX_HBRACINGGUSSET);
	CPlateBasicIndex	*pBxNext	= pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
	CSteelBolt			*pSteelBolt	= m_pAPlateCalcStd->m_pDataManage->GetSteelBolt();
	CSteel				*pSteelCtl	= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	CHBracingGusset		*pHBracingGusset	= pBx->GetHBracingGusset();//pGir->GetHBracingGussetInTotal(m_nIndex);
	if(!pHBracingGusset->IsReal())
		pHBracingGusset = pBxMid->GetHBracingGusset();

	CSteelMaterialData	*pMatData	= pHBracing->GetSteelMaterialData();
	m_pBracingJewon->m_nType		= pMatData->m_nType;
	m_pBracingJewon->m_dHeight		= pMatData->m_dH;
	m_pBracingJewon->m_dWidth		= pMatData->m_dB;
	m_pBracingJewon->m_dThick1		= pMatData->m_dT1;
	m_pBracingJewon->m_dThick2		= pMatData->m_dT2;
	m_pBracingJewon->m_dArea		= frCm2(pMatData->m_dSectionArea);
	m_pBracingJewon->m_dGammaX		= frCm(pMatData->m_dRadiusOfGyration.x);
	m_pBracingJewon->m_dGammaY		= frCm(pMatData->m_dRadiusOfGyration.y);
	if(pMatData->m_nType==CSteelMaterialData::CTSTEEL)
	{
		m_pBracingJewon->m_dGammaMin	= frCm(min(m_pBracingJewon->m_dGammaX, m_pBracingJewon->m_dGammaY));
		m_strMaterialType.Format("CT-%.0fx%.0fx%.0fx%.0f", 
			m_pBracingJewon->m_dHeight,m_pBracingJewon->m_dWidth,m_pBracingJewon->m_dThick1,m_pBracingJewon->m_dThick2);
	}
	else if(pMatData->m_nType==CSteelMaterialData::CSTEEL)
	{
		m_pBracingJewon->m_dGammaMin	= frCm(min(m_pBracingJewon->m_dGammaX, m_pBracingJewon->m_dGammaY));
		m_strMaterialType.Format("ㄷ-%.0fx%.0fx%.0fx%.0f", 
			m_pBracingJewon->m_dHeight,m_pBracingJewon->m_dWidth,m_pBracingJewon->m_dThick1,m_pBracingJewon->m_dThick2);
	}
	else
	{
		m_pBracingJewon->m_dGammaMin	= frCm(min(pMatData->m_dRadiusOfGyrationMaxMin.x, pMatData->m_dRadiusOfGyrationMaxMin.y));
		m_strMaterialType.Format("L-%.0fx%.0fx%.0f", 
			m_pBracingJewon->m_dHeight,m_pBracingJewon->m_dWidth,m_pBracingJewon->m_dThick1);
	}
	m_pBracingJewon->m_dLength		= pHBracing->GetBeamJewon(0, FALSE, 6);
	m_pBracingJewon->m_dZy			= frCm3(pMatData->m_dModulusOfSection.y);
	if(pMatData->m_nType==CSteelMaterialData::CSTEEL)
		m_pBracingJewon->m_dZy		= frCm3(pMatData->m_dModulusOfSection.x);

	double	dStation			= pBx->GetStation();
	double	dHeightGirder		= pDB->GetGirder(0)->GetBracingInTotal(0)->GetBx()->GetHeightGirderByBx();
	double	dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double	dSlabThick			= pDB->m_dThickSlabBase;
	double	dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double	dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double	dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;
	double	dWidthBridge		= pDB->m_dBridgeWidth;
	double	dLengthBridge		= pDB->GetLengthBridge();

	double	dHeightBridge		= dHeightGirder+dSlabThick+dHeightGuardFence+dHeightSoundProof+dHeightHandRail;
	double	dRateWidthHeight	= dWidthBridge/dHeightBridge;

	if(dRateWidthHeight>=1 && dRateWidthHeight<8)	m_dWindLoad	= max((4.0-0.2*dRateWidthHeight)*dHeightBridge/1000.0, 6.0);
	else if(dRateWidthHeight>=8.0)					m_dWindLoad	= max(2.4*dHeightBridge/1000.0, 6.0);
	else											m_dWindLoad	= 0;
	
	m_dWw	= m_dWindLoad/2.0/2.0/1.2;

	long	nQtyNode	= pFEMManage->GetJointSize();
	//BOOL	bPositive	= StressCalc.IsPositive(ELE_MOMENT3, pBx->GetNumberGirder(), pBx->GetStation());
	//계산서와 동일하게 처리함.//LSH_ZZ
	//BOOL	bPositive	= StressCalc.IsPositive(ELE_MOMENT3, 0, pDB->GetGirder(0)->GetBracingInTotal(0)->GetBx()->GetStation());
	double	dRiskCoeff	= pGlobarOpt->GetRiskCoefficient();
	double	dZoneCoeff	= pGlobarOpt->GetZoneCoefficient();
	double	dTotReaction= 0;

	// APLATE-654 지진시 고정하중 반력만 적용되어야 한다.
	CResultArray *pResult	= m_pAPlateCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	for(long nNode=0; nNode<nQtyNode; nNode++)
	{
		CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(nNode);
		double dPreDeadSteel	= pJResult->m_vPreDeadSteel[JOINT_REACTION].z;
		double dPreDeadSlab		= pJResult->m_vPreDeadSlab[JOINT_REACTION].z;
		double dPostDead		= pJResult->m_vPostDead[JOINT_REACTION].z;
		dTotReaction += frkN(dPreDeadSteel + dPreDeadSlab + dPostDead);		
	}

	CResultArray	*pResultPost	= m_pAPlateCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
	for(long nNode=0; nNode<nQtyNode; nNode++)
	{
		CJointResult	*pJResult	= pResultPost->m_JointResultArr.GetAt(nNode);
		double dPostDead		= pJResult->m_vPostDead[JOINT_REACTION].z;
		dTotReaction += frkN(dPostDead);
	}

	double	dWd				= dTotReaction/dLengthBridge;
	double	dAcceleration	= dRiskCoeff*dZoneCoeff;
	m_dEarthQuakeLoad	= dAcceleration*dWd; 
	m_dWe				= m_dEarthQuakeLoad/2.0/2.0/1.5;
	m_dLengthSection	= fabs(pBxNext->GetStation()-pBx->GetStation());
	long	nJoint		= ModelCalc.GetJointNoByBx(pBx);
	long	nJigan		= ModelCalc.GetJiganNoByJointID(nJoint);
	long	nJijum		= ModelCalc.GetJijumNoOfJoint(nJoint);;
	double	dLength		= pDB->m_dLengthJigan[nJigan];
	double	dX1			= pBx->GetStation() - pGir->GetBxOnJijum(nJigan)->GetStation();
	double	dX2			= pBxNext->GetStation() - pGir->GetBxOnJijum(nJigan)->GetStation();
	double	dH1			= -dX1/dLength*1000.0;
	double	dH2			= (dLength-dX2)/dLength*1000.0;
	double	dSlop		= (dH2-dH1)/(dX2-dX1);
	double	dYIntercept	= dH1-dSlop*dX1;
	double	dXIntercept	= -dYIntercept/dSlop;
	m_dAreaPositiveInf	= (dLength - dXIntercept)*dH2/2.0;
	m_dAreaNegativeInf	= dXIntercept*dH1/2.0;
	m_dTheta			= atan(m_dLengthSection/2.0/pGir->GetDisGirderToGirder(pBx));
	m_dLengthElem		= m_dLengthSection/(2.0*sin(m_dTheta));
	m_dNwPositive		= toM2(m_dAreaPositiveInf)*m_dWw/cos(m_dTheta);
	m_dNwNegative		= toM2(m_dAreaNegativeInf)*m_dWw/cos(m_dTheta);
	m_dNe				= toM2(m_dAreaPositiveInf+m_dAreaNegativeInf)*m_dWe/cos(m_dTheta);

	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	CCalcGeneral	CalcGen(m_pAPlateCalcStd->m_pDataManage);

	CalcGen.GetMaterial(pBx, HB_B, strLeft, strRight);
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowBucklingUp(pSteel);
	CHGAllowStress	AllowStress(pSteel);
	
	
	AllowBucklingUp.SetAllowBucklingUnconsider(m_dLengthElem, m_pBracingJewon->m_dGammaX);
	m_strMaterial		= strLeft;
	m_dSlendernessRatio	= m_dLengthElem/toCm(m_pBracingJewon->m_dGammaMin);
	m_dFca				= AllowBucklingUp.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, m_pBracingJewon->m_dThick1);
	m_dFcaApo			= m_dFca*(0.5+m_dLengthElem/m_pBracingJewon->m_dGammaX/1000.0);
	m_dFta				= AllowStress.GetAllowStress(ALLOW_COMPSTRESS, m_pBracingJewon->m_dThick1);
	if(m_pBracingJewon->m_nType==CSteelMaterialData::LSTEEL || m_pBracingJewon->m_nType==CSteelMaterialData::LVALSTEEL)
		m_dAn	= m_pBracingJewon->m_dArea-(m_pBracingJewon->m_dWidth-m_pBracingJewon->m_dThick1)/2*m_pBracingJewon->m_dThick1;
	else if(m_pBracingJewon->m_nType==CSteelMaterialData::CTSTEEL)
	{
		double dd1 = m_pBracingJewon->m_dArea;
		double dd2 = (m_pBracingJewon->m_dWidth-m_pBracingJewon->m_dThick2)/2*m_pBracingJewon->m_dThick1;
		double dd3 = 25*2*m_pBracingJewon->m_dThick2;
		m_dAn	=  dd1 - dd2 - dd3;
	}
	else
		m_dAn	= m_pBracingJewon->m_dArea;
	m_dFt				= max(max(fabs(m_dNwPositive), fabs(m_dNwNegative)), fabs(m_dNe))*1000 / m_dAn;
	m_dFc				= max(max(fabs(m_dNwPositive), fabs(m_dNwNegative)), fabs(m_dNe))*1000 / m_pBracingJewon->m_dArea;
	m_dDesignLoad		= max(0.75*m_dFcaApo*m_pBracingJewon->m_dArea/1000, max(max(fabs(m_dNwPositive), fabs(m_dNwNegative)), fabs(m_dNe)));
	CalcGen.GetMaterial(pBx, HB_BOLT, strLeft, strRight);
	m_strBoltMaterial	= strLeft+strRight;
	m_dDiaBolt			= GetBoltSize(pHBracingGusset->m_ZzsBoltSize);

	CSteelBoltData *pBolt = pSteelBolt->GetBolt(m_strBoltMaterial, m_dDiaBolt);
	if(!pBolt)
	{
		if(m_dDiaBolt==20.0)		m_dBoltAllowStress	= 39000.0;
		else if(m_dDiaBolt==22.0)	m_dBoltAllowStress	= 48000.0;
		else						m_dBoltAllowStress	= 48000.0;
	}
	else	
		m_dBoltAllowStress	= pBolt->m_ForceAllow;

	m_dQtyBoltReq		= m_dDesignLoad/(m_dBoltAllowStress/1000);
	m_dQtyBoltUse		= (pHBracingGusset->m_sB+1)*(pHBracingGusset->m_sE+1);
	m_nQtyLine			= 2;
}