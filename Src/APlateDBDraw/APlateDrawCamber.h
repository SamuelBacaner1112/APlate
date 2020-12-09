// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawCamber.h: interface for the CAPlateDrawCamber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWCAMBER_H__3332C30F_DF06_434A_82ED_A0602B8A0A07__INCLUDED_)
#define AFX_APLATEDRAWCAMBER_H__3332C30F_DF06_434A_82ED_A0602B8A0A07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage; 
class CPlateBasicIndex;
class CPlateGirderApp;

enum EnumCamberColor {
	ECC_DEFAULT1	= 7,
	ECC_DEFAULT2	= 10,

	ECC_JONGDAN		= 30,
	ECC_ONLYSTEEL	= 50,
	ECC_STEEL		= 70,
	ECC_VIVID		= 130,
	ECC_FACT		= 150,
	ECC_FIELD		= 170,
	ECC_REMOVESTEEL = 200,
	ECC_REMOVEVIVID = 210,
	ECC_USER		= 230,
	ECC_NOT         = -1,
	ECC_CONF		= 15,
	ECC_TURNING		= 20,
};

class AFX_EXT_CLASS CAPlateDrawCamber  
{
public:
	CAPlateDrawCamber(CDataManage *pDataMng);
	virtual ~CAPlateDrawCamber();

private:
	CDataManage *m_pDataMng;

public:
	BOOL AllDrawCamberOut(CDomyun* pCamberDom, long nG, long nCol);
	BOOL AllDrawCamber(CDomyun* pCamberDom, long nG, BOOL bTriangle=TRUE);
	CDPoint GetDrawRate(CDomyun *pDom, CDPoint* pBriPoint, long nG);
	void DrawCamberFact(CDomyun* pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberField(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberHeightLine(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, BOOL bDivide, long nG, EnumCamberColor Ecc);
	void DrawCamberJDanCurve(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberOnlySteelWeight(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberPyunSlopCurve(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberRemoveSteel(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberRemoveVivid(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberSteelWeight(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberTriangle(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, BOOL bDivide, long nG, EnumCamberColor Ecc);
	void DrawCamberUser(CDomyun *pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberVivid(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberConf(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawCamberTurning(CDomyun* pCamberDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc);
	void DrawDimCamber(CDomyun* pDomp,CDPoint* pBase,CDPoint* pRate,long nG);
	long GetMaxCamber(long nG);
	double GetMaxMinCamberCalc(long nG,long nCamber);

};

#endif // !defined(AFX_APLATEDRAWCAMBER_H__3332C30F_DF06_434A_82ED_A0602B8A0A07__INCLUDED_)
