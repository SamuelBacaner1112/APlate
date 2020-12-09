// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignStiff.h: interface for the CXLDesignStiff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNSTIFF_H__DE8FC61D_5B82_4D6F_92AF_E0CE6862745C__INCLUDED_)
#define AFX_XLDESIGNSTIFF_H__DE8FC61D_5B82_4D6F_92AF_E0CE6862745C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLDesignStiff : public CAPlateOutCalcXL  
{
public:
	CXLDesignStiff(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignStiff();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	///< �÷��� ���� ������ ����	- ������, �Ϲݺ�
	void WebStiff(CStringArray &strArrUseSheet);
	///< ������ ������ ����			- �����, ������
	void JijumSupport(CStringArray &strArrUseSheet, BOOL bIsPier);
	///< ��� ������ ����			- �����, ������
	void JackUpSupport(CStringArray &strArrUseSheet, BOOL bIsPier);

	// SHOE 1���Ҵ� JACK UP��ġ ��ġ���Ҽ� ���ϱ�
	long GetJackupBuildsuByOneShoe(CPlateBasicIndex *pBx);
protected:
	long				m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	void SetLines(int nLine);
	void WebStiff(CPlateBasicIndex *pBx, BOOL bUpper);
};

#endif // !defined(AFX_XLDESIGNSTIFF_H__DE8FC61D_5B82_4D6F_92AF_E0CE6862745C__INCLUDED_)
