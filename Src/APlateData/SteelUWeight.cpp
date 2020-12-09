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
	m_dUWeightGangjae	= 0.00007850;	///< ���� �����߷�
	m_dUWeightAluminum	= 0.00002800;	///< �˹̴� �����߷�
	m_dUWeightChulgunCon= 0.00002500;	///< ö�� ��ũ��Ʈ �����߷�
	m_dUWeightMugunCon	= 0.00002350;	///< ���� ��ũ��Ʈ �����߷�
	m_dUWeightCement	= 0.00002150;	///< �ø�Ʈ ���� �����߷�
	m_dUWeightMokjae	= 0.00000800;	///< ���� �����߷�
	m_dUWeightYukchung	= 0.00001100;	///< ��û��(�����) �����߷�
	m_dUWeightAsphalt	= 0.00002300;	///< �ƽ���Ʈ ���� �����߷�
	m_dUWeighGravel		= 0.00001900;	///< �ڰ�,�⼮ �����߷�
	m_nTypePave			= 0;
}

CString CSteelUWeight::GetTypePave()
{
	CString strTypePave	= _T("");

	switch(m_nTypePave)
	{
	case 0 :
		strTypePave = _T("�ƽ���Ʈ");
		break;
	case 1 :
		strTypePave = _T("����");
		break;
	case 2 :
		strTypePave = _T("LMC");
		break;
	}

	return strTypePave;
}

long CSteelUWeight::GetTypePave(CString strTypePave)
{
	if(strTypePave == _T("�ƽ���Ʈ"))	return 0;
	if(strTypePave == _T("����"))		return 1;
	if(strTypePave == _T("LMC"))		return 2;

	return -1;
}

double CSteelUWeight::GetPaveUnitWeight(CString strTypePave)
{
	if(strTypePave == _T("�ƽ���Ʈ"))	return 0.00002300;
	if(strTypePave == _T("����"))		return 0.00002150;
	if(strTypePave == _T("LMC"))		return 0.00002350;

	return -1.0;
}