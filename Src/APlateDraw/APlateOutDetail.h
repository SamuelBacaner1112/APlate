// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutDetail.h: interface for the APlateOutDetail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTDETAIL_H__3696AC88_0D73_4240_A79E_8A0900FD570C__INCLUDED_)
#define AFX_APLATEOUTDETAIL_H__3696AC88_0D73_4240_A79E_8A0900FD570C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutDetail : public CDrawPage  
{
public:
	CAPlateOutDetail();
	virtual ~CAPlateOutDetail();

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();	

private:
	long m_WeldDetailMoveNum;
	long m_MaxSebuDetailSu;
	CHAR m_cWeldTypeName;

	CStringArray m_sNoteArr;

private:
	CPlateBasicIndex* GetBxSttPage(long nSubPage);
	CPlateBasicIndex* GetBxEndPage(long nSubPage);	

protected:
	CDPoint DrawSolPlate(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawEtcSymbolPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double Angle, BOOL bUpper);
	void AddBeomye(CDomyun *pDom);
	CDPoint DrawWeldDetail(CDomyun *pDomP, double dX, double dY, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpDown);
	CDPoint DrawingWeldDetail(CDomyun *pDomP, double dX, double dY, CSteelWeldData *pWeld, long nUpDown2, double T1, double T2, BOOL bWeb=FALSE);
	void DrawFieldWeldDetail(CDomyun *pDomP, double dX, double dY, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	CDPoint DrawHStiffDetail(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	CDPoint DrawHStiffDetailByRib(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	CDPoint DrawVStiffDetail(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawAbutPier(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawAbutSubJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CPlateBasicIndex *pBx, BOOL bStt);
	void DrawPierSubJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CPlateBasicIndex *pBx);
	void DrawFlangeJewon(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double Angle, BOOL bUpper);
	void DrawWebJewon(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide);
	void InitNote();
	void DimNoteTypeVertical(CDomyun *pDomP);
	void DimNoteTypeHorizontol(CDomyun *pDomP);
	//
	void DrawJijumStiffJewon(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double dAng, BOOL bUpper);	
	void DrawAbutSubPyung(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bStt);
	void DrawPierSubPyung(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawAddAbutPierPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawStationMark(CDomyun* pDom, CPlateBasicIndex *pBx,int nDan);

public:
	void AddPlan(CDomyun *pDom, long nSubPage, BOOL bUpper);
	void AddJong(CDomyun *pDom, long nSubPage);
	void AddDan(CDomyun *pDom, long nSubPage);
	void AddKeyPan(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void AddNote(CDomyun *pDomP, long nSubPage);
};

#endif // !defined(AFX_APLATEOUTDETAIL_H__3696AC88_0D73_4240_A79E_8A0900FD570C__INCLUDED_)
