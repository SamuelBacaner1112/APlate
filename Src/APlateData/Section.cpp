// Section.cpp: implementation of the CSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateCrossBeam.h"
#include "VBracing.h"
#include "Section.h"
#include "VStiff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSection::CSection()
{
	m_enType = -1;
	m_enTypeNum = 0;
	m_enDetType = -1;

	m_pCrossBeam = NULL;
	m_pVBracing = NULL;
	m_pVStiff = NULL;
	m_vAngLeft = CDPoint(0,0);
	m_vAngRight = CDPoint(0,0);
}

CSection::~CSection()
{
	if(m_enDetType!=-1)
	{
		if(m_enDetType == SECDET_CROSSBEAM)		delete m_pCrossBeam;
		else if(m_enDetType == SECDET_VBRACING)	delete m_pVBracing;
		else if(m_enDetType == SECDET_VSITFF)	delete m_pVStiff;
	}
}

void CSection::SetenDetType(long nType)
{
	if(m_enDetType != -1)
	{
		if(m_enDetType == nType)
			return; // 같을 경우 기존데이타 보호
		if(m_enDetType == SECDET_CROSSBEAM)		delete m_pCrossBeam, m_pCrossBeam = NULL;
		else if(m_enDetType == SECDET_VBRACING)	delete m_pVBracing, m_pVBracing = NULL;
		else if(m_enDetType == SECDET_VSITFF)	delete m_pVStiff, m_pVStiff = NULL;
	}

	m_enDetType = nType;
	if(m_enDetType == SECDET_CROSSBEAM)	
	{
		m_pCrossBeam = new CPlateCrossBeam(this);
		m_pCrossBeam->Create(GetBx()->GetGirder(), GetBx());
	}
	else if(m_enDetType == SECDET_VBRACING)
	{
		m_pVBracing = new CVBracing(this);
		m_pVBracing->Create(GetBx()->GetGirder(), GetBx());
	}
	else if(m_enDetType == SECDET_VSITFF)	
	{
		m_pVStiff = new CVStiff(this);
		m_pVStiff->Create(GetBx()->GetGirder(), GetBx());
	}
}

void CSection::Serialize(CArchive &ar)
{
	long nFlag = 1;

	CPlateBxObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_enType;
		ar << m_enTypeNum;
		ar << m_enDetType;	
		ar << m_vAngLeft;
		ar << m_vAngRight;
		ar << m_strCalcSectionName;
		ar << m_nSecType;
		
		if(m_enDetType == SECDET_CROSSBEAM) 		
			m_pCrossBeam->Serialize(ar);		
		else if(m_enDetType == SECDET_VBRACING)		
			m_pVBracing->Serialize(ar);
		else if(m_enDetType == SECDET_VSITFF)
			m_pVStiff->Serialize(ar);
		ar << m_dStationLeft;
		ar << m_dStationRight;
	}
	else
	{
		ar >> nFlag;
		ar >> m_enType;
		ar >> m_enTypeNum;
		ar >> m_enDetType;		
		ar >> m_vAngLeft;
		ar >> m_vAngRight;
		ar >> m_strCalcSectionName;
		ar >> m_nSecType;
		
		if(m_enDetType == SECDET_CROSSBEAM) 
		{
			if(!m_pCrossBeam) m_pCrossBeam = new CPlateCrossBeam(this);
			m_pCrossBeam->Serialize(ar);
		}
		else if(m_enDetType == SECDET_VBRACING)
		{
			if(!m_pVBracing) m_pVBracing = new CVBracing(this);
			m_pVBracing->Serialize(ar);
		}
		else if(m_enDetType == SECDET_VSITFF)
		{
			if(!m_pVStiff) m_pVStiff = new CVStiff(this);
			m_pVStiff->Serialize(ar);
		}
		if(nFlag>0)
		{
			ar >> m_dStationLeft;
			ar >> m_dStationRight;
		}
	}
}

inline CVStiff* CSection::GetVStiff()
{
	if(m_enDetType == SECDET_VSITFF)
		return m_pVStiff;
	else if(m_enDetType == SECDET_VBRACING)
		return m_pVBracing->m_pVStiff;
	else if(m_enDetType == SECDET_CROSSBEAM)
		return m_pCrossBeam->m_pVStiff;

	return NULL;
//	return m_enDetType == SECDET_VSITFF ? m_pVStiff : m_enDetType == SECDET_VBRACING ? m_pVBracing->m_pVStiff : NULL; 
}

//nPos=0  상현재 
//nPos=1  하현재 
//nPos=2  사현재 
double CSection::GetBeamJewon(long nPos, long nJewon)
{
	CPlateBridgeApp		*pDB		= (CPlateBridgeApp *)GetBx()->GetGirder()->GetBridgeApp();
	CVBracing			*pVr		= ((CPlateBasicIndex*)GetBx())->GetSection()->GetVBracing();
	CSteelMaterialData	*pMatData	= NULL;
	CBracing			*pBracing	= GetBx()->GetBracing();

	if(nPos==CSection::SEC_UP)		pMatData = pVr->GetSteelMaterialData(STEELMAT_UPPER);
	if(nPos==CSection::SEC_DOWN)	pMatData = pVr->GetSteelMaterialData(STEELMAT_LOWER);
	if(nPos==CSection::SEC_LEFT || nPos==CSection::SEC_RIGHT)
		pMatData = pVr->GetSteelMaterialData(STEELMAT_DIAGONAL);
	if(nPos==CSection::SEC_UHBEAM || nPos==CSection::SEC_LHBEAM)
	{
		if(pBracing)
			pMatData = pBracing->GetSteelMaterialData();
	}
	
	long	nType		= 0;
	long	nSteelType	= pMatData->m_nType;
	double	dValue		= 0;
	double	dCenDx		= 0;
	double	dCenDy		= 0;
	if(nPos==0)
		nType = CSteelMaterial::BUJE_UVBEAM;
	if(nPos==1)
		nType = CSteelMaterial::BUJE_LVBEAM;
	if(nPos==2 || nPos==3)
		nType = CSteelMaterial::BUJE_CVBEAM;
	if(nPos==4 || nPos==5)	
		nType = CSteelMaterial::BUJE_LHBEAM;
	
//	long nSteelType = pMat->GetSteelType(GetBx(), nType);
//	long nJewonType = pMat->GetJewonType(GetBx(), nType);

	switch (nJewon)
	{
	case 0 :
		dValue = pMatData->m_dH;
		break;
	case 1 :
		dValue = pMatData->m_dB;
		break;
	case 2 :
		dValue = pMatData->m_dT1;
		break;
	case 3 :
		dValue = pMatData->m_dT2;
		break;
	case 4 :
		dValue = pMatData->m_dR1;
		break;
	case 5 :
		dValue = pMatData->m_dR2;
		break;
	case 6 :
		dValue = pVr ? pVr->GetLengthBeam(nPos) : 0;
		 break;
	case 7 :
		dCenDx = pMatData->m_dCenterOfGravity.x;
		if(nSteelType==CSteelMaterialData::CTSTEEL && nType == CSteelMaterial::BUJE_LHBEAM)
			return pMatData->m_dB/2;
		if(dCenDx==0)	dValue = pMatData->m_dH/2;
		else			dValue = frCm(dCenDx);
		break;
	case 8 :
		dCenDy = pMatData->m_dCenterOfGravity.y;
		if(dCenDy==0)
			dValue = pMatData->m_dB/2;
		else
			dValue = frCm(dCenDy);
	}


	// 수직브레이싱정면, 수평브레이싱평면 : LSTEEL, LVALSTEEL, CSTEEL -> Cx사용, CTSTEEL -> dB/2;
	// 수직브레이싱평면, 수평브레이싱정면 : LSTEEL, LVALSTEEL, CSTEEL -> Cy사용, CTSTEEL -> Cx 사용;

	return dValue;
}

//nPos=0  상현재 
//nPos=1  하현재 
//nPos=2  사현재 
double CSection::GetBeamBaseHeight(long nPos, BOOL bVBracing, BOOL bFront)
{
	CPlateBridgeApp	*pDB	= (CPlateBridgeApp *)GetBx()->GetGirder()->GetBridgeApp();
	CVBracing		*pBr	= GetVBracing();
	CBracing		*pBracing	= GetBx()->GetBracing();
	CSteelMaterialData	*pMatData	= NULL;

	if(nPos==CSection::SEC_UP)
		pMatData = pBr->GetSteelMaterialData(STEELMAT_UPPER);
	if(nPos==CSection::SEC_DOWN)
		pMatData = pBr->GetSteelMaterialData(STEELMAT_LOWER);
	if(nPos==CSection::SEC_LEFT || nPos==CSection::SEC_RIGHT)
		pMatData = pBr->GetSteelMaterialData(STEELMAT_DIAGONAL);
	if(nPos==CSection::SEC_UHBEAM || nPos==CSection::SEC_LHBEAM)
	{
		if(pBracing)
			pMatData = pBracing->GetSteelMaterialData();
	}

/*
	long nType = 0;
	if(nPos==0)
		nType = CSteelMaterial::BUJE_UVBEAM;
	if(nPos==1)
		nType = CSteelMaterial::BUJE_LVBEAM;
	if(nPos==2 || nPos==3)
		nType = CSteelMaterial::BUJE_CVBEAM;
	if(nPos==4 || nPos==5)
		nType = CSteelMaterial::BUJE_LHBEAM;
*/
//	long nSteelType = pMat->GetSteelType(GetBx(), nType);
//	long nJewonType = pMat->GetJewonType(GetBx(), nType);
	double dH =  pMatData->m_dH;
	double dB =  pMatData->m_dB;

	// 수직브레이싱에서는 높이를 입면폭으로 본다.
	// 수평브레이싱에서는 높이를 평면폭으로 본다.

	if(bVBracing)
	{
		return bFront?dH:dB;
	}
	else
	{
		switch(pMatData->m_nType)
		{
		case CSteelMaterialData::LSTEEL:
			return bFront?dB:dH;
		case CSteelMaterialData::LVALSTEEL:
			return bFront?dB:dH;
		case CSteelMaterialData::CSTEEL:
			return bFront?dB:dH;
		case CSteelMaterialData::CTSTEEL:
			return bFront?dH:dB;
		case CSteelMaterialData::ISTEEL:
			return bFront?dB:dH;
		case CSteelMaterialData::HSTEEL:
			return bFront?dB:dH;
		}
	}
	
	return 0;
}

