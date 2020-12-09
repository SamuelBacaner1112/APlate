// DanMyunProp.cpp: implementation of the CDanMyunProp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"
#include "DanMyunProp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDanMyunProp::CDanMyunProp(CAPlateCalcStd *pCalcStd, CPlateBasicIndex *pBx)
{	
	m_pCalcStd = pCalcStd;
	m_pBx = pBx;

	m_nByunType = EQUAL_HIGH;

	m_dMaxMoment_Left = 0;
	m_dMinMoment_Left = 0;
	m_dMaxMoment_Right = 0;
	m_dMinMoment_Right = 0;

	m_dMaxSta_Left = 0;
	m_dMinSta_Left = 0;
	m_dMaxSta_Right = 0;
	m_dMinSta_Right = 0;

	InitCalc();
}

CDanMyunProp::~CDanMyunProp()
{

}

CDanMyunProp::CDanMyunProp(const CDanMyunProp& rhs)
{	
	m_pCalcStd = rhs.m_pCalcStd;
	m_pBx = rhs.m_pBx;
	
	m_nByunType = rhs.m_nByunType;
	
	m_MatLeft  = rhs.m_MatLeft;
	m_MatRight = rhs.m_MatRight;
	
	m_dMaxMoment_Left = rhs.m_dMaxMoment_Left;
	m_dMinMoment_Left = rhs.m_dMinMoment_Left;
	m_dMaxSta_Left = rhs.m_dMaxSta_Left;
	m_dMinSta_Left = rhs.m_dMinSta_Left;

	m_dMaxMoment_Right = rhs.m_dMaxMoment_Right;
	m_dMinMoment_Right = rhs.m_dMinMoment_Right;
	m_dMaxSta_Right = rhs.m_dMaxSta_Right;
	m_dMinSta_Right = rhs.m_dMinSta_Right;
}

CDanMyunProp& CDanMyunProp::operator=(const CDanMyunProp& rhs)
{
	if(&rhs == this)
		return *this;
	
	m_pCalcStd = rhs.m_pCalcStd;
	m_pBx = rhs.m_pBx;
	
	m_nByunType = rhs.m_nByunType;
	
	m_MatLeft  = rhs.m_MatLeft;
	m_MatRight = rhs.m_MatRight;

	m_dMaxMoment_Left = rhs.m_dMaxMoment_Left;
	m_dMinMoment_Left = rhs.m_dMinMoment_Left;
	m_dMaxSta_Left = rhs.m_dMaxSta_Left;
	m_dMinSta_Left = rhs.m_dMinSta_Left;
	
	m_dMaxMoment_Right = rhs.m_dMaxMoment_Right;
	m_dMinMoment_Right = rhs.m_dMinMoment_Right;
	m_dMaxSta_Right = rhs.m_dMaxSta_Right;
	m_dMinSta_Right = rhs.m_dMinSta_Right;

	return *this;
}


void CDanMyunProp::InitCalc()
{
	CPlateGirderApp	*pGir	= m_pBx->GetGirder();

	if(pGir->IsByunByStation(m_pBx->GetStation()))
		m_nByunType		= BYUNDAN_TYPE;
}
/*
int CDanMyunProp::GetJiganByStation(int nG, double dSta)
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pStd);
	
	int nSttIdx = -1;
	int nEndIdx = -1;
	pFemData->GetDiapNodesu(nG, nSttIdx, nEndIdx);

	int nJ = -1;
	for(int i=nSttIdx; i<=nEndIdx; i++)
	{
		if(pFemData->m_vJoint[i].m_pBx->IsJijum())
		{
			if(i == nEndIdx)
				nJ;
			else
				nJ++;
		}
		if(pFemData->m_vJoint[i].m_pBx->GetStation() >= dSta)
			break;
	}

	return nJ;
}
*/
void CDanMyunProp::SetMaterialIndex()
{
	CPlateBridgeApp* pDB = m_pCalcStd->GetBridge();
	CPlateGirderApp* pGir = m_pBx->GetGirder();
	double dSttSta = pGir->GetStationOnJijum(0);
	double dEndSta = pGir->GetStationOnJijum(pDB->m_nQtyJigan);	
	CFEMManage      *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc ModelCalc(m_pCalcStd->m_pDataManage, m_pCalcStd);
	CCalcGeneral	CalcGen(m_pCalcStd->m_pDataManage);
	
	CString szJewon[3], szTemp;
	
	// 시종점부 테이퍼가 끝나는 부분에 생성된 단면은 
	// 시점부 좌측단면 : 시점부와 동일단면
	// 시점부 우측단면 : 테이퍼우측단면
	// 종점부 좌측단면 : 테이퍼좌측단면
	// 종점부 우측단면 : 종점부와 동일단면   임...
	CPlateBasicIndex *pBxTemp = NULL;
	if(m_nByunType == EQUAL_HIGH)
	{
		double dPreSta = m_pBx->GetStation()-10;
		if(dPreSta >= dSttSta)// || m_pBx == pGir->GetBxOnJijum(0)) #31766 관련하여 ARoad 참고 후 주석처리
		{
			if(m_pBx->IsState(BX_STT_BENDING_UPPER))
				pBxTemp = pGir->GetBxOnJijum(0);
			else
				pBxTemp = m_pBx;

			ModelCalc.GetSectionCoefficient(pBxTemp, FALSE, 0,-1);
			m_MatLeft.m_dBu		= ModelCalc.m_MapSecCoe["FLANGE_W_UP"];
			m_MatLeft.m_dTu		= ModelCalc.m_MapSecCoe["FLANGE_T_UP"];
			m_MatLeft.m_dBl		= ModelCalc.m_MapSecCoe["FLANGE_W_DN"];
			m_MatLeft.m_dTl		= ModelCalc.m_MapSecCoe["FLANGE_T_DN"];
			m_MatLeft.m_dH		= ModelCalc.m_MapSecCoe["WEB_H"];
			m_MatLeft.m_dTw		= ModelCalc.m_MapSecCoe["WEB_T"];
			
			m_MatLeft.m_pBx		= pBxTemp;
			m_MatLeft.m_dA		= ModelCalc.GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS);
			m_MatLeft.m_dI33	= ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBxTemp);
			m_MatLeft.m_dI22	= ModelCalc.GetSectionCoeffI22(BEFORE_COMPOSITE_ANALYSIS);
			m_MatLeft.m_dJ		= ModelCalc.GetSectionCoeffK(BEFORE_COMPOSITE_ANALYSIS);

			CalcGen.GetMaterial(pBxTemp, G_F_U, szJewon[FRM_UPPER], szTemp);
			CalcGen.GetMaterial(pBxTemp, G_F_L, szJewon[FRM_LOWER], szTemp);
			CalcGen.GetMaterial(pBxTemp, G_W,   szJewon[FRM_WEB], szTemp);
			CDPoint Propty[3];
			Propty[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
			Propty[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
			Propty[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);
			CFrameSection *pFrameSec	= (CFrameSection*)ModelCalc.GetFrameSectionByPrpty(pManage, Propty, szJewon);
			if(pFrameSec==NULL)
				m_pCalcStd->SetExtraDanmyun(m_MatLeft);			
			else
				m_MatLeft.m_nIdx	= atol(pFrameSec->m_szName)-1;			
		}		
		double dPostSta = m_pBx->GetStation()+10;
		if(dPostSta <= dEndSta)// || m_pBx == pGir->GetBxOnJijum(pDB->m_nQtyJigan)) #31766 관련하여 ARoad 참고 후 주석처리
		{					
			if(m_pBx->IsState(BX_END_BENDING_UPPER))
				pBxTemp = pGir->GetBxOnJijum(pDB->m_nQtyJigan);
			else
				pBxTemp = m_pBx;

			ModelCalc.GetSectionCoefficient(pBxTemp, FALSE, 0, 1);

			m_MatRight.m_dBu	= ModelCalc.m_MapSecCoe["FLANGE_W_UP"];
			m_MatRight.m_dTu	= ModelCalc.m_MapSecCoe["FLANGE_T_UP"];
			m_MatRight.m_dBl	= ModelCalc.m_MapSecCoe["FLANGE_W_DN"];
			m_MatRight.m_dTl	= ModelCalc.m_MapSecCoe["FLANGE_T_DN"];
			m_MatRight.m_dH		= ModelCalc.m_MapSecCoe["WEB_H"];
			m_MatRight.m_dTw	= ModelCalc.m_MapSecCoe["WEB_T"];
			
			m_MatRight.m_pBx	= pBxTemp;
			m_MatRight.m_dA		= ModelCalc.GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS);
			m_MatRight.m_dI33	= ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBxTemp);
			m_MatRight.m_dI22	= ModelCalc.GetSectionCoeffI22(BEFORE_COMPOSITE_ANALYSIS);
			m_MatRight.m_dJ		= ModelCalc.GetSectionCoeffK(BEFORE_COMPOSITE_ANALYSIS);

			CalcGen.GetMaterial(pBxTemp, G_F_U, szJewon[FRM_UPPER], szTemp);
			CalcGen.GetMaterial(pBxTemp, G_F_L, szJewon[FRM_LOWER], szTemp);
			CalcGen.GetMaterial(pBxTemp, G_W,   szJewon[FRM_WEB], szTemp);
			CDPoint Propty[3];
			Propty[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
			Propty[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
			Propty[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);
			CFrameSection *pFrameSec	= (CFrameSection*)ModelCalc.GetFrameSectionByPrpty(pManage, Propty, szJewon);
			if(pFrameSec==NULL)	
				m_pCalcStd->SetExtraDanmyun(m_MatRight);			
			else
				m_MatRight.m_nIdx	= atol(pFrameSec->m_szName)-1;
		}		
	}
	else
	{
		ModelCalc.GetSectionCoefficient(m_pBx, FALSE);
		
		m_MatLeft.m_dBu		= ModelCalc.m_MapSecCoe["FLANGE_W_UP"];
		m_MatLeft.m_dTu		= ModelCalc.m_MapSecCoe["FLANGE_T_UP"];
		m_MatLeft.m_dBl		= ModelCalc.m_MapSecCoe["FLANGE_W_DN"];
		m_MatLeft.m_dTl		= ModelCalc.m_MapSecCoe["FLANGE_T_DN"];
		m_MatLeft.m_dH		= ModelCalc.m_MapSecCoe["WEB_H"];
		m_MatLeft.m_dTw		= ModelCalc.m_MapSecCoe["WEB_T"];
		
		m_MatLeft.m_pBx		= pBxTemp;
		m_MatLeft.m_dA		= ModelCalc.GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS);
		m_MatLeft.m_dI33	= ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBxTemp);
		m_MatLeft.m_dI22	= ModelCalc.GetSectionCoeffI22(BEFORE_COMPOSITE_ANALYSIS);
		m_MatLeft.m_dJ		= ModelCalc.GetSectionCoeffK(BEFORE_COMPOSITE_ANALYSIS);

		CalcGen.GetMaterial(m_pBx, G_F_U, szJewon[FRM_UPPER], szTemp);
		CalcGen.GetMaterial(m_pBx, G_F_L, szJewon[FRM_LOWER], szTemp);
		CalcGen.GetMaterial(m_pBx, G_W,   szJewon[FRM_WEB], szTemp);
		CDPoint Propty[3];
		Propty[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
		Propty[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
		Propty[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);
		CFrameSection *pFrameSec	= (CFrameSection*)ModelCalc.GetFrameSectionByPrpty(pManage, Propty, szJewon);
		if(pFrameSec==NULL)		
			m_pCalcStd->SetExtraDanmyun(m_MatLeft);		
		else
			m_MatLeft.m_nIdx	= atol(pFrameSec->m_szName)-1;	

		m_MatRight = m_MatLeft;
	}	
}

BOOL CDanMyunProp::IsByunDanMyun()
{
	if(m_nByunType == EQUAL_HIGH)
		return FALSE;
	else
		return TRUE;
}

long CDanMyunProp::GetIndexUsedMaterial()
{
	return m_MatRight.m_bUsedCalc ? m_MatRight.m_nIdx : (m_MatLeft.m_bUsedCalc ? m_MatLeft.m_nIdx : -1);
}

