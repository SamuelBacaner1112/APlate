// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelDeduct.h: interface for the CSteelDeduct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELDEDUCT_H__EFE71779_DD34_4984_83BF_ECE15662B1E5__INCLUDED_)
#define AFX_STEELDEDUCT_H__EFE71779_DD34_4984_83BF_ECE15662B1E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelDeduct  
{
public:
	CSteelDeduct();
	virtual ~CSteelDeduct();

	long m_nMatApplyType;
	CStringArray m_MatApplyArr;

	long m_bDeductUpperFlange;		// 1. ����
	long m_bDeductLowerFlange;		// 2. ����
	long m_bDeductWeb;				// 3. ������
	long m_bDeductHStiff;			// 5. ���򺸰���
	long m_bDeductVerStiff;			// 6. ����������
	long m_bDeductCrossBeamWebPlate;// 7. ���κ� ������
	long m_bDeductCrossBeamBracketWebPlate;	// 8. ���κ� ����� ������
	long m_bDeductCrossBeamBracketUDFlange;	// 9. ���κ� ����� ��.����
	long m_bDeductCrossBeamBracketStiffner;	// 10. ���κ� ����� ������
	long m_bDeductCrossBeamBracketStiffPlate;//11. ���κ� ����� ������		
	long m_bDeductDongBari;			// 12. ���������� ���ٸ�	
	long m_bDeductGeneralScallop;	// 13. �Ϲ� ��Ķ��
	long m_bDeductEdgeCut;			// 14. ��ٱ� ����
	long m_bDeductBoltHole;			// 15. ��ƮȦ ����
	long m_bDeductSplice;			// 16. �������� ����
	long m_bDeductSolePlate;		// 17. ���÷���Ʈ ����
		
	long m_bSolePlateForJackUp; // ����� ���÷���Ʈ
	long m_bBSAWeightApply; // ��Ʈ, ���ܿ�����, ������ ��Ŀ �߷�����
	long m_bSectionWeightApply; // �ܸ� �⺻ �߷� ��������
	long m_nFloatNum;			// �ݿø� �ڸ���
	

// Implementation
public:
	CString GetMatApplyType();
	void SetMatApplyType(CString rtStr);
	void InitData();

public:	
	void Serialize(CArchive& ar);
};

#endif // !defined(AFX_STEELDEDUCT_H__EFE71779_DD34_4984_83BF_ECE15662B1E5__INCLUDED_)
