// SteelDeduct.cpp: implementation of the CSteelDeduct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelDeduct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelDeduct::CSteelDeduct()
{
	InitData();

	m_MatApplyArr.Add("���ġ�� ��.�� ����");
	m_MatApplyArr.Add("���ġ�� �Ŵ� �߾� ����");
	m_MatApplyArr.Add("����ġ�� �¿� ����");
	m_MatApplyArr.Add("����ġ�� ���簢�� ����");
	m_MatApplyArr.Add("����ġ�� ���෮, �¿� ����");
	m_MatApplyArr.Add("����ġ�� ���෮, ���簢�� ����");
	m_MatApplyArr.Add("���߷� ����");
}

CSteelDeduct::~CSteelDeduct()
{
	
}
void CSteelDeduct::InitData()
{
	m_bDeductUpperFlange = TRUE;	// 1. ����
	m_bDeductLowerFlange = TRUE;	// 2. ����
	m_bDeductWeb = FALSE;			// 3. ������
	m_bDeductHStiff = FALSE;		// 5. ���򺸰���
	m_bDeductVerStiff = FALSE;		// 6. ����������
	m_bDeductCrossBeamWebPlate			= FALSE; // 7. ���κ� ������
	m_bDeductCrossBeamBracketWebPlate	= FALSE; // 8. ���κ� ����� ������
	m_bDeductCrossBeamBracketUDFlange	= FALSE; // 9. ���κ� ����� ��.����
	m_bDeductCrossBeamBracketStiffner	= FALSE; // 10. ���κ� ����� ������
	m_bDeductCrossBeamBracketStiffPlate = FALSE; // 11. ���κ� ����� ������		
	m_bDeductDongBari		= FALSE;	// 12. ���������� ���ٸ�		
	m_bDeductGeneralScallop = FALSE;	// 13. �Ϲ� ��Ķ��
	m_bDeductEdgeCut		= FALSE;	// 14. ����� ����
	m_bDeductBoltHole		= FALSE;	// 15. ��ƮȦ ����
	m_bDeductSplice			= FALSE;	// 16. �������� ����
	m_bDeductSolePlate		= FALSE;	// 17. ���÷���Ʈ ����
	m_nMatApplyType = 1;				//��������ǥ �������						

	m_bSolePlateForJackUp = 1;
	m_bBSAWeightApply = 1;				// ��Ʈ, ���ܿ�����, ������ ��Ŀ �߷�����
	m_bSectionWeightApply = 0;			// �ܸ� �⺻ �߷� ��������
	m_nFloatNum = 4;					// �ݿø� �ڸ���	
}

CString CSteelDeduct::GetMatApplyType()
{	
	return m_MatApplyArr.GetAt(m_nMatApplyType);		
}

void CSteelDeduct::SetMatApplyType(CString rtStr)
{
	for(long i=0;i<m_MatApplyArr.GetSize();i++)
	{
		if(m_MatApplyArr.GetAt(i) == rtStr)
		{
			m_nMatApplyType = i;
			break;
		}
	}		
}

void CSteelDeduct::Serialize(CArchive& ar)
{
	long nLong = 0;
//	long nFlag = 0;
	long nFlag = 2;	// �������� Ÿ���߰��� ���� ������������ �߰� ..081002 lina

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nMatApplyType;
		ar << nLong;	// ������ ����(YES : 0 )
		ar << nLong;
		ar << nLong;		// ��Ȧ ����(YES : 0 )
		ar << nLong;
		ar << nLong; // �������κ� ����
		ar << m_bDeductCrossBeamWebPlate;
		ar << m_bDeductWeb;		// ������ ����
		ar << m_bDeductUpperFlange;
		ar << m_bDeductCrossBeamBracketStiffPlate;// ����� ������
		ar << m_bDeductCrossBeamBracketStiffner;
		ar << m_bDeductCrossBeamBracketUDFlange;
		ar << m_bDeductVerStiff;
		ar << m_bDeductHStiff;
		ar << nLong;
		ar << nLong;
		ar << nLong;
		ar << nLong;
		ar << m_bSolePlateForJackUp;	// ����� ���÷���Ʈ ����
		ar << m_bBSAWeightApply; // ��Ʈ, ���ܿ�����, ������ ��Ŀ �߷�����
		ar << m_bSectionWeightApply; // �ܸ� �⺻ �߷� ��������
		ar << m_nFloatNum;			// �ݿø� �ڸ���
		ar << m_bDeductBoltHole;		

		ar << m_bDeductLowerFlange;
		ar << nLong;
		ar << m_bDeductCrossBeamBracketWebPlate;
		ar << nLong;
		ar << m_bDeductDongBari;
		ar << m_bDeductSplice;
		ar << m_bDeductSolePlate;
	}
	else
	{		
		ar >> nFlag;
		ar >> m_nMatApplyType;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> m_bDeductCrossBeamWebPlate;
		ar >> m_bDeductWeb;		// ������ ����
		ar >> m_bDeductUpperFlange;
		ar >> m_bDeductCrossBeamBracketStiffPlate;// ����� ������
		ar >> m_bDeductCrossBeamBracketStiffner;
		ar >> m_bDeductCrossBeamBracketUDFlange;
		ar >> m_bDeductVerStiff;
		ar >> m_bDeductHStiff;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;

		ar >> m_bSolePlateForJackUp;	// ����� ���÷���Ʈ ����
		ar >> m_bBSAWeightApply;		// ��Ʈ, ���ܿ�����, ������ ��Ŀ �߷�����
		ar >> m_bSectionWeightApply;	// �ܸ� �⺻ �߷� ��������
		ar >> m_nFloatNum;				// �ݿø� �ڸ���
		ar >> m_bDeductBoltHole;		

		ar >> m_bDeductLowerFlange;
		ar >> nLong;
		ar >> m_bDeductCrossBeamBracketWebPlate;
		ar >> nLong;
		ar >> m_bDeductDongBari;
		
		if(nFlag > 0)	ar >> m_bDeductSplice;
		if(nFlag > 1)	ar >> m_bDeductSolePlate;
	}
}