// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutSlabGen.h: interface for the CAPlateOutSlabGen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTSLABGEN_H__53DE4346_00B7_4311_BD98_E3A63CCC44EA__INCLUDED_)
#define AFX_APLATEOUTSLABGEN_H__53DE4346_00B7_4311_BD98_E3A63CCC44EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDimDomyunEx;
class CAPlateOutSlabGen : public CDrawPage
{
public:
	CAPlateOutSlabGen();
	virtual ~CAPlateOutSlabGen();

	void DrawOnePage(); 
	long GetPageSu();
	CString GetTitle();
	double GetScale();
	double m_dLastY; 
	CDRect m_rectDgk;

	int m_nDimensionTableVCount;	// ġ����ȭ ���̺� V����
	int m_nDimensionTableHCount;	// ġ����ȭ ���̺� H����

	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrBx;

private:
	CPlateBasicIndex *GetBxSttPage(long nSubPage, long nG, BOOL bSttSlab = TRUE);
	CPlateBasicIndex *GetBxEndPage(long nSubPage, long nG, BOOL bEndSlab = TRUE);

	void AddFrontDetail(CDomyun *pDomP, long nSubPage);
	void AddFront(CDomyun *pDomP, long nSubPage, BOOL bLeft);
	void AddKeyPlan(CDomyun *pDomP, long nSubPage, double dXDis, double dYDis);

	void AddPlan(CDomyun *pDom, long nSubPage);
	void AddDimPlan(CDomyun *pDom, long nSubPage);
	void AddDimensionTable(CDomyun *pDomP, long nSubPage);
	void SetDimensionTableFormat(CGridDomyunEx *pGDom, int nSubPage);
	void AddTitleArray(CStringArray &sTitleArr);
	void DrawDimensionTableContents(CGridDomyunEx *pGDom, int nSubPage);
	double GetSlabDimHeight(CPlateBasicIndex *pBx, CDPoint po);
	double GetSlabDimWidth(CDPoint poStt, CDPoint poEnd);

private:
	void AddSectionMark(CDomyun *pDom, long nSubPage, long nType);
	void DrawSlabByPage(CDomyun *pDomP, long nSubPage, BOOL bDrawCantileverHunch=TRUE);
	void DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDan);
	void AddPlanGirderLine(CDomyun *pDom, long nSubPage);
	void DrawCenterLineAndMark(CDomyun *pDomP, long subPage);

private:
	//���ܸ鵵
	void AddJongDanmyunDo(CDomyun *pDom, long nPage);
	void DrawSlabLineUpperGirder(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DimTextJongMatchLine(CDomyun *pDom, long nSubPage, long nG, long nDan);

	void AddDimJong(CDomyun *pDomP, long nSubPage);
	CDPointArray m_poArrStart;
	CDPointArray m_poArrEnd;
	 // Ⱦ�ܸ鵵 ġ����...
	void AddFrontDetailSlabDim(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawSlabDimHeight(CDimDomyunEx* pDim, CPlateBasicIndex *pBx, CDPoint po, int nVNumber);
	void DrawSlabDimWidth(CDimDomyunEx* pDim, CDPoint poStt, CDPoint poEnd, int nHNumber);

private:
	BOOL IsStringerInPage(long nPage);
	BOOL IsStringerInPage(long nPage, long nG);
	CPlateBasicIndex *GetBxDimensionTable(long nPage);
	CPlateBasicIndex *m_pBxStt;
	CPlateBasicIndex *m_pBxEnd;
	long	m_nG;
	double  m_dPreStation;
	// UTILITY
	void	SetMinMaxAndSame(double aSlabH[], BOOL &bSame);
	void	GetCenSSSAAAOnGirder(int nType, double &dFirCenSSS, double &dSecCenSSS, CDPoint &FirAAA, CDPoint &dSecAAA);
	void	GetSideDis(int nType, BOOL bSectionAABB, double &dDisLeft, double &dDisRight);
	void	GetSlabLength(int nType, BOOL bSectionAABB, double aChgSlabH[], BOOL &bSameLen);

	void	GetSlabHeight(int nType, BOOL bSectionAABB, double aSlabH[], BOOL &bSameLen);
	void	GetChgSlabHeight(int nType, BOOL bSectionAABB, double aChgSlabH[], double aChgSlopH[], BOOL &bSameLen);

	// Draw Section
	void AddSectionAABB(CDomyun *pDom, long nPage, BOOL bSectionAABB, BOOL bStt, BOOL bSection, CPlateBasicIndex *pBx = NULL, BOOL bBindConc=FALSE);
	void AddSectionAABBDim(CDomyun *pDom, long nPage, BOOL bSectionAABB, BOOL bStt, BOOL bSection);
	void AddSectionCC(CDomyun *pDom, long nPage, BOOL bSection, CPlateBasicIndex *pBx = NULL);
	void AddGuardWall(CDomyun *pDom, long nPage, long nGuard, BOOL bRight);
	void AddNotch(CDomyun *pDom, long nPage);			// SLAB �Ϲݵ�
	// ���� ������
	void AddSectionDD(CDomyun *pDom, long nPage);		// ���κ� ��ġ�� ���
	void AddSectionDDDim(CDomyun *pDom, long nPage);	// ���κ� ��ġ�� ���
	void AddSectionEE(CDomyun *pDom, long nPage);		// ���κ� ��ġ�� ���
	void AddSectionWingBeam(CDomyun *pDom, long nPage, BOOL bSection, CPlateBasicIndex *pBx = NULL);	// AddSectionCC()�� ����
	void AddSectionEndBeam(CDomyun *pDom, long nPage);	// AddSectionEE()�� ����

	void AddRectangleNotch(CDomyun *pDom, long nPage);
	void AddTrapezoidNotch(CDomyun *pDom, long nPage);
};

#endif // !defined(AFX_APLATEOUTSLABGEN_H__53DE4346_00B7_4311_BD98_E3A63CCC44EA__INCLUDED_)
