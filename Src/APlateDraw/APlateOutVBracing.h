// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutVBracing.h: interface for the CAPlateOutVBracing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTVBRACING_H__EDF357C1_4808_47D0_9C64_E0703844EDFB__INCLUDED_)
#define AFX_APLATEOUTVBRACING_H__EDF357C1_4808_47D0_9C64_E0703844EDFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef CTypedPtrArray	<CObArray, CVBracing*> CVBracingList;

class CAPlateOutVBracing : public CDrawPage  
{
public:
	CAPlateOutVBracing();
	virtual ~CAPlateOutVBracing();

	CStringArray m_sNoteArr;
	CTypedPtrArray	<CObArray, CVBracing*> m_VBracingList;

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();

	void InitNote();
	void DimNoteTypeVertical(CDomyun *pDomP);
	void AddNote(CDomyun *pDomP, long nSubPage);
	void AddHDanVBracing(CDomyun *pDom, CPlateBasicIndex* pBx);
	void AddVBracingPyungTotalDanmyun(CDomyun *pDom, CPlateBasicIndex* pBx, BOOL bUpper);
	void AddHDanVBracingSangse(CDomyun *pDom, CPlateBasicIndex* pBx, long nPos);
	void AddHDanVBracingDanmyunAA(CDomyun *pDom, CPlateBasicIndex* pBx);
	void AddHDanVBracingDanmyunBB(CDomyun *pDom, CPlateBasicIndex* pBx);
	void MakeVBracingList();

	void DimHDanVBracingJewonText(CDomyun *pDom, CPlateBasicIndex* pBx);
	void DrawDetailAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx);
	void AddKeyPan(CDomyun *pDom, double xDis, double yDis, CPlateBasicIndex *pBxCr);

	void AddDetailBM(CDomyun *pDom, CPlateBasicIndex *pBx);
};

#endif // !defined(AFX_APLATEOUTVBRACING_H__EDF357C1_4808_47D0_9C64_E0703844EDFB__INCLUDED_)
