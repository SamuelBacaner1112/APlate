// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutPaintMap.h: interface for the CAPlateOutPaintMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTPAINTMAP_H__2B4CFEA2_7F78_43CE_842F_E35AA3750994__INCLUDED_)
#define AFX_APLATEOUTPAINTMAP_H__2B4CFEA2_7F78_43CE_842F_E35AA3750994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutPaintMap : public CDrawPage  
{
public:
	CAPlateOutPaintMap();
	virtual ~CAPlateOutPaintMap();

public:
	void InsertDomyun(CDomyun *pDom, long nSubPage);
	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();
private:

	void AddGeneral(CDomyun *pDom);
	void AddDanmyun(CDomyun *pDom);
	void AddSplice(CDomyun *pDom);
	void AddCrossBeam(CDomyun *pDom);


	void DrawGeneral(CDomyun *pDom, CPlateBasicIndex *pBx);	//�Ϲݵ� �׸���..
	void DrawSection(CDomyun *pDom, CPlateBasicIndex *pBx, long nMode);
	void DimTextNoteGen(CDomyun *pDom, CDPoint Base, long nMode);		//�Ϲݵ� ��Ʈ..
	void DrawTableGen(CDomyun *pDom);						//�Ϲݵ� ���̺�...
	void DrawTableSec(CDomyun *pDom);
	void DrawTableSplice(CDomyun *pDom);
	void DrawTableCROSSBEAM(CDomyun *pDom);

	void DrawFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper); //�������� ������..�׸���...
	void DrawWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx); //�������� ������..

	void DrawCrossBeamPlan(CDomyun *pDom, CPlateBasicIndex *pBx);//���κ� ��鵵..
	void DrawCrossBeamFront(CDomyun *pDom, CPlateBasicIndex *pBx);//���κ� ���鵵..

	void ReplaceData(CDomyun *pDom);
};

#endif // !defined(AFX_APLATEOUTPAINTMAP_H__2B4CFEA2_7F78_43CE_842F_E35AA3750994__INCLUDED_)
