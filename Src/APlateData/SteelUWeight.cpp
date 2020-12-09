// SteelUWeight.cpp: implementation of the CSteelUWeight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelUWeight.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelUWeight::CSteelUWeight()
{
	DataInit();
}

CSteelUWeight::~CSteelUWeight()
{

}

void CSteelUWeight::Serialize(CArchive& ar)
{
	BYTE	bFlag = 1;

	if(ar.IsStoring()) 
	{
		ar << bFlag;
		ar << m_dUWeightGangjae;
		ar << m_dUWeightAluminum;
		ar << m_dUWeightChulgunCon;
		ar << m_dUWeightMugunCon;
		ar << m_dUWeightCement;
		ar << m_dUWeightMokjae;
		ar << m_dUWeightYukchung;
		ar << m_dUWeightAsphalt;
		ar << m_dUWeighGravel;
		ar << m_nTypePave;
	}
	else
	{
		ar >> bFlag;
		ar >> m_dUWeightGangjae;
		ar >> m_dUWeightAluminum;
		ar >> m_dUWeightChulgunCon;
		ar >> m_dUWeightMugunCon;
		ar >> m_dUWeightCement;
		ar >> m_dUWeightMokjae;
		ar >> m_dUWeightYukchung;
		ar >> m_dUWeightAsphalt;
		ar >> m_dUWeighGravel;
		if(bFlag>0)
			ar >> m_nTypePave;
	}
}

void CSteelUWeight::DataInit()
{
	m_dUWeightGangjae	= 0.00007850;	///< 강재 단위중량
	m_dUWeightAluminum	= 0.00002800;	///< 알미늄 단위중량
	m_dUWeightChulgunCon= 0.00002500;	///< 철근 콘크리트 단위중량
	m_dUWeightMugunCon	= 0.00002350;	///< 무근 콘크리트 단위중량
	m_dUWeightCement	= 0.00002150;	///< 시멘트 모르테 단위중량
	m_dUWeightMokjae	= 0.00000800;	///< 목재 단위중량
	m_dUWeightYukchung	= 0.00001100;	///< 역청재(방수용) 단위중량
	m_dUWeightAsphalt	= 0.00002300;	///< 아스팔트 포장 단위중량
	m_dUWeighGravel		= 0.00001900;	///< 자갈,쇄석 단위중량
	m_nTypePave			= 0;
}

CString CSteelUWeight::GetTypePave()
{
	CString strTypePave	= _T("");

	switch(m_nTypePave)
	{
	case 0 :
		strTypePave = _T("아스팔트");
		break;
	case 1 :
		strTypePave = _T("모르터");
		break;
	case 2 :
		strTypePave = _T("LMC");
		break;
	}

	return strTypePave;
}

long CSteelUWeight::GetTypePave(CString strTypePave)
{
	if(strTypePave == _T("아스팔트"))	return 0;
	if(strTypePave == _T("모르터"))		return 1;
	if(strTypePave == _T("LMC"))		return 2;

	return -1;
}

double CSteelUWeight::GetPaveUnitWeight(CString strTypePave)
{
	if(strTypePave == _T("아스팔트"))	return 0.00002300;
	if(strTypePave == _T("모르터"))		return 0.00002150;
	if(strTypePave == _T("LMC"))		return 0.00002350;

	return -1.0;
}