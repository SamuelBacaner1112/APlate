// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawSebu.h: interface for the CAPlateDrawSebu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWSEBU_H__E0A93016_55EB_4288_831B_9FE88C3A4E91__INCLUDED_)
#define AFX_APLATEDRAWSEBU_H__E0A93016_55EB_4288_831B_9FE88C3A4E91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class AFX_EXT_CLASS CAPlateDrawSebu  
{
public:
	CAPlateDrawSebu(CDataManage *pDataMng);
	virtual ~CAPlateDrawSebu();

private:
	CDataManage *m_pDataMng;  

public:
	// ���ɷ� ��
	// ������ �ܺ� ��
	// ���򺸰��� ������
	void AddHStiffDetail_WithVStiff(CDomyun *pDomP, CPlateBasicIndex *pBx, double Scale, BOOL bDrawDetailDlg = TRUE);
	void AddHStiffDetail_WithHRib(CDomyun *pDomP, CPlateBasicIndex *pBx, long nSide, double DimScale);
	void AddHStiffDetail_WithSplice(CDomyun *pDomP, CPlateBasicIndex	*pBx, double Scale, BOOL bDrawDetailDlg=TRUE);
	
	//	���ܿ����� ���� ��
	// ���������� ������
	void AddVStiffDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX, double BY, double DimScale);	
	void DimVStiffDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX, double BY, double DimScale);	

	// �������� ������ : NEW CODE by HA (2000.10.11) Renew code by min
	// ���⼭ CType�̶� �����ǿ����� �ǹ�..���A,�Ϻ�B,����C�� �����Ͽ� �ۼ��� �ڵ�� ����
	void AddFactDetail(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2, double termH2=6, double AngleV=60, double AngleX=90, BOOL bCenter = FALSE, BOOL bShop=TRUE, BOOL bWeb=FALSE);
	void DrawFactDetail_VType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2,double AngleV=60, BOOL bCenter = FALSE, BOOL bDimDraw = FALSE, BOOL bShop=TRUE);
	void DrawFactDetail_VType_CType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2,double AngleV=60, BOOL bCenter = FALSE, BOOL bDimDraw = FALSE, BOOL bShop=TRUE);

	void DrawFactDetail_XType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2, double termH2=6,double AngleV=60, double AngleX=90, BOOL bCenter = FALSE, BOOL bShop=TRUE);
	void DrawFactDetail_XType_CType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2, double termH2=6,double AngleV=60, double AngleX=90, BOOL bCenter = FALSE, BOOL bShop=TRUE);
	void DrawFactDetail_XTypeByWeb(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2, double termH2=6,double AngleV=60, double AngleX=90, BOOL bCenter = FALSE, BOOL bShop=TRUE);
	void DimFactDetail_VType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2,double AngleV=60, BOOL bCenter = FALSE, BOOL bDimDraw = FALSE, BOOL bShop=TRUE);
	void DimFactDetail_XType(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2, double termH2=6,double AngleV=60, double AngleX=90, BOOL bCenter = FALSE, BOOL bShop=TRUE);
	void DimFactDetail_XTypeByWeb(CDomyun *pDomP,double T1,double T2, double Scale, long nUpDown
						  ,double termW=2, double termH=2, double termH2=6,double AngleV=60, double AngleX=90, BOOL bCenter = FALSE, BOOL bShop=TRUE);

	// ���÷���Ʈ �� (�Ϲݵ�,���λ�)
	void AddSolePlateDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bGen=TRUE);
	void DrawSolePlatePlanDetail(CDomyun *pDomP, CPlateBasicIndex *pBx);
	void DrawSolePlateSideDetail(CDomyun *pDomP, CPlateBasicIndex *pBx);
	void DimSolePlatePlan(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bGen=TRUE);
	void DimSolePlateSide(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bGen=TRUE);
	// ��
	void DrawShoeBoxDetail(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX=0, double BY=0, double dFactorThick=1.0, BOOL bLongitude = TRUE, double dBogus = 0);
	//���̷�
	void DrawScallopExtend(CDomyun *pDomP, CPlateBasicIndex *pBx, double BX, double BY, double ScallopR, 
										   double Length, 
										   double Slop1,  double Slop2, 
										   double Thick1, double Thick2, 
										   double WebT, BOOL bLeft, BOOL bExtend, long nScallop, BOOL bStringer, BOOL bShop);

};

#endif // !defined(AFX_APLATEDRAWSEBU_H__E0A93016_55EB_4288_831B_9FE88C3A4E91__INCLUDED_)
