// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutCamber.h: interface for the CAPlateOutCamber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTCAMBER_H__5C72F4C7_AD79_49E0_A5EA_4D20B510366F__INCLUDED_)
#define AFX_APLATEOUTCAMBER_H__5C72F4C7_AD79_49E0_A5EA_4D20B510366F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutCamber : public CDrawPage  
{
public:
	CAPlateOutCamber();
	virtual ~CAPlateOutCamber();

// Attributes
protected:
	double	m_YRatio;	//ķ�� ����
	double	m_YRatio2;	//���ؼ� ����
	long	m_nG;		//���� �Ŵ�
	double	m_SttSta;	//���� �����̼�
	double	m_EndSta;	//�� �����̼�
public:	
	long	m_nDWSStyle;		//��� ��Ÿ�� 1:��� 2:���
//	long	m_nPagePerJigan;	//�������� ��µ� ������

public:
	void DrawOnePage() { AddOnePage(); };

// Implementaion
protected:
	CString GetTitle();

	void DrawYSymbol(CDomyun *pDomP);

	//Ⱦ�ܸ鵵
	void DimGirderDist(CDomyun *pDomP,CPlateBasicIndex *pBx);
	void DimSlabThick(CDomyun *pDomP,CPlateBasicIndex *pBx);
	void DimTextEtc(CDomyun *pDomP,CPlateBasicIndex *pBx);
	//�Թ��� ����
	void TextOutMultiLine(CDomyun *pDom, double x, double y, CString &cs, long nVertAlign = 0);
	void GetCamberStation(CVectorArray &staArr, long nSectionType, long nG);
	void SetPageData();
	double GetMaxCamberInGirder();

	//���۵� Ÿ��1
	void DrawAllCamberFact1(CDomyun *pDomP, CDPoint Base);
	void DrawDWSCamberDiagram(CDomyun *pDomP, CDPoint Base);
	void DrawDWSCamberDim(CDomyun *pDomP, CDPoint Base);
	void DrawDWSCamberTable(CDomyun *pDomP, CDPoint Base);

	//���۵� Ÿ��2
	void DrawAllCamberFact2(CDomyun *pDomP, CDPoint Base);			//��üȣ��
	void DrawDMCamberUppdrDim(CDomyun *pDomP, CDPoint Base);	//����� ġ��
	void DrawDMCamberTable(CDomyun *pDomP, CDPoint Base);		//�������̺�	
	void AddBumlaeMark(CDomyun *pDomP, CDPoint Base);

	//���赵 Ÿ��
	void DrawAllCamberDesign(CDomyun *pDomP, CDPoint Base);
	void DrawCamberDesign(CDomyun *pDomP, CDPoint Base);
	CDPoint DrawCamberDesignTable(CDomyun *pDomP, CDPoint Base, long nG);
	void DrawCamberDesignLegend(CDomyun *pDomP, CDPoint Base);
	//��� Ÿ�� ���̺� �׸��� �Լ�
	void SetDMTableTitleArr(CStringArray &arrTitle);
	//�������� �Լ�
	BOOL IsSpot(CVector vSta);	//�������� �ƴ���...

	//������ ����
	void AddSmallTitle(CDomyun *pDomP);

public:
	void AddOnePage();
	long GetPageSu();
	CString GetPageTitle(long nPage, BOOL bShort);
	double GetScale();

};

#endif // !defined(AFX_APLATEOUTCAMBER_H__5C72F4C7_AD79_49E0_A5EA_4D20B510366F__INCLUDED_)
