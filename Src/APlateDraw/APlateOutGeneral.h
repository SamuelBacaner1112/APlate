// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutGeneral.h: interface for the CAPlateOutGeneral class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTGENERAL_H__055836B8_F73D_449E_ADA2_A97E9E1BE331__INCLUDED_)
#define AFX_APLATEOUTGENERAL_H__055836B8_F73D_449E_ADA2_A97E9E1BE331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutGeneral : public CDrawPage  
{
public:
	CAPlateOutGeneral();
	virtual ~CAPlateOutGeneral();

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle(); 
	void SortSta();

	CStringArray m_sNoteArr;
	CDPointArray m_JweonInfoStaArr;		//x : 시작 스테이션 y: 끝 스테이션	

protected:
	CPlateBasicIndex* GetPageBxStt(long nG, long nSubPage, BOOL bFrontMatch=FALSE, BOOL bNextMatch=FALSE);
	CPlateBasicIndex* GetPageBxEnd(long nG, long nSubPage, BOOL bFrontMatch=FALSE, BOOL bNextMatch=FALSE);

	void DrawPierJong(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	BOOL CheckStation(double sta1, double sta2);	
	void TextOutJewonAndMat(CDomyun *pDomP, CDPoint xy, CString strBuje, CString strMat, BOOL bDirForward=TRUE);
	void DimTextPyungFlange(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi);
	void DimTextPyungVStiff(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper, double dLenGisi);
	void DimTextPyungTotal(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nUpper);
	void DimTextJongMatchLine(CDomyun *pDom, long nSubPage, long nG, long nDan);
	void DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDanUp, long nDanLow);

public:
	void AddPlan(CDomyun *pDom, long nSubPage);
	void AddDimPlan(CDomyun *pDomP, long nSubPage);
	void AddJong(CDomyun *pDom, long nSubPage);
	void AddDimJong(CDomyun *pDomP, long nSubPage, long nG);
	void AddFront(CDomyun *pDom, long nSubPage);
	void AddHDanJiJum(CDomyun *pDom);
	void AddHDanVBracing(CDomyun *pDom);
	void AddHDanHGirder(CDomyun *pDom);
	void AddKeyPan(CDomyun *pDom, long nSubPage, double xDis, double yDis);
	void AddLegend(CDomyun *pDomP);
	//void DrawAbutPier(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void InitNote();
	void AddNote(CDomyun *pDomP, long nSubPage);
	void DimNoteTypeVertical(CDomyun *pDomP);
	void DimNoteTypeHorizontol(CDomyun *pDomP);	
	void DimTextMarkPlan(CDomyun *pDomP, long nSubPage);	
	void DimTextLineInfo(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimPyungJijumText(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
									    double dist, double distExo, BOOL bIncludeSttEnd);
	void DimJongJijumText(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, 
				      double Height, double HeightExo, BOOL bIncludeSttEnd, BOOL bSameHeight);

	int GetDimDan(__int64 DimFlag);

};

#endif // !defined(AFX_APLATEOUTGENERAL_H__055836B8_F73D_449E_ADA2_A97E9E1BE331__INCLUDED_)
