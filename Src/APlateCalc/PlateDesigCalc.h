// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateDesigCalc.h: interface for the CPlateDesigCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEDESIGCALC_H__AC906A08_F55C_4FB6_8FBA_2A68A641DE3E__INCLUDED_)
#define AFX_PLATEDESIGCALC_H__AC906A08_F55C_4FB6_8FBA_2A68A641DE3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CPlateDesigCalc : public CObject 
{
public:
	CPlateDesigCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd);
	virtual ~CPlateDesigCalc();

	void GetMapWeldReview(long nType, CMap <CString, LPCTSTR, double, double>& Map);		//용접 검토
	BOOL GetMapWeldJijumStiff(CMap <CString, LPCTSTR, double, double>& Map, BOOL bIsPier);	//지점 보강재 용접 검토
	void GetMapStud(long nNodeIdx, CMap <CString, LPCTSTR, double, double>& Map);
	double GetMaxGirderHight(CPlateBasicIndex *pBxMax, long nG, double dSttSta, double dEndSta);

private:
	CDataManage		*m_pDataManage;
	CAPlateCalcStd	*m_pCalcStd;
};

#endif // !defined(AFX_PLATEDESIGCALC_H__AC906A08_F55C_4FB6_8FBA_2A68A641DE3E__INCLUDED_)
