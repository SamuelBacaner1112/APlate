// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawLineInfo.h: interface for the CAPlateDrawLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWLINEINFO_H__F682122A_C6DF_48AA_93B9_FBC91FEDC0BA__INCLUDED_)
#define AFX_APLATEDRAWLINEINFO_H__F682122A_C6DF_48AA_93B9_FBC91FEDC0BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class AFX_EXT_CLASS CAPlateDrawLineInfo  
{
public:
	CAPlateDrawLineInfo(CDataManage *pDataMng);
	virtual ~CAPlateDrawLineInfo();

	void DrawLineInfoPlan(CDomyun *pDom, long nLine, BOOL bTotal);
	void DrawLineInfoPlanChainUnit(CDomyun *pDom, long nLine, BOOL bTotal);
	void DrawLineInfoPlanInfo(CDomyun *pDom, long nLine, BOOL bTotal);
	void DrawLineInfoPlanInfoIP(CDomyun *pDom, long nLine, BOOL bTotal);
	void DrawLineInfoCoordinate(CDomyun *pDom, long nLine, double dUnitCoordinate);
	void DrawLineInfoTable(CDomyun *pDom, long nLine, CDPoint TableBase);
	void DrawLineInfoTableIP(CDomyun *pDom, long nLine, long nIP, CDPoint TableBase);

	void DrawJongDanPlan(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser);
	void DrawJongDanPlanLine(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser);
	void DrawJongDanPlanLineInfo(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser);
	void DrawJongDanPlanInfoVIP(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser);

	void DrawPyungubaePlan(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bTotal);
	void DrawPyungubaePlanInfo(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bTotal, BOOL bUser);
	void DrawPyungubaePlanCenterLine(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, double dChainUnit, int nCase, BOOL bUser);
	void DrawPyungubaePlanUnitChain(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, double dChainUnit, int nCase, BOOL bTotal, BOOL bUser);
	void DrawPyungubaeLineInfo(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bTotal, BOOL bUser);
	void DrawPyungubaeRatioBar(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bTotal, BOOL bUser);

private:
	CDataManage *m_pDataMng;  
};

#endif // !defined(AFX_APLATEDRAWLINEINFO_H__F682122A_C6DF_48AA_93B9_FBC91FEDC0BA__INCLUDED_)
