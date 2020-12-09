// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignCrossBeam.h: interface for the CXLDesignCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNCROSSBEAM_H__05AAABEE_368E_4902_B1B5_6CBE3EB1E133__INCLUDED_)
#define AFX_XLDESIGNCROSSBEAM_H__05AAABEE_368E_4902_B1B5_6CBE3EB1E133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLDesignCrossBeam : public CAPlateOutCalcXL  
{
public:
	CXLDesignCrossBeam(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignCrossBeam();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	///< 7.1 CROSS BEAM ����
	void DesignCrossBeam(CStringArray &strArrUseSheet, long &nIdx);
	void DesignCrossBeamDetail(CPlateBasicIndex *pBx, long &nRow);

	void DesignVBracingOut(CStringArray &strArrUseSheet, long &nIdx);

	void DesignEndCrossBeamDetail(CPlateBasicIndex *pBx, long &nRow);
	void DesignCenterCrossBeamDetail(CPlateBasicIndex *pBx, long &nRow);
	void DesignInverseVBracing(CPlateBasicIndex *pBx, long &nRow);
	void DesignVBracing(CPlateBasicIndex *pBx, long &nRow);
	void DesignHorBracing(CStringArray &strArrUseSheet, long &nIdx);
	void DesignConnection(long nIdx);
	void ConnectGussetType(CPlateBasicIndex *pBx, long UNType, long nInOut);	//���κ����� - �ż¹�� ���Ϻ� �÷���	
	void ConnectGussetWeb(CPlateBasicIndex *pBx, long LRType);	//���κ����� - ������ ���� ���� �÷���
	void ConnectExistTop(CPlateBasicIndex *pBx);				//���κ����� - ������� ��� �÷���
	void ConnectExistBottom(CPlateBasicIndex *pBx);				//���κ����� - ������� �Ϻ� �÷���
	void ConnectExistWeb(CPlateBasicIndex *pBx, long LRType);	//���κ����� - ������ �ִ� ���� �÷���

protected:
	long	m_nSttRow;

	CDoubleArray m_ArrFU;
	CDoubleArray m_ArrFL;
	CDoubleArray m_ArrMU;
	CDoubleArray m_ArrSU;

	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;

	void SetLines(int nLine);
	void DrawCrossBeam(CPlateBasicIndex *pBx, BOOL bLeft);
};

#endif // !defined(AFX_XLDESIGNCROSSBEAM_H__05AAABEE_368E_4902_B1B5_6CBE3EB1E133__INCLUDED_)
