// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateSplice.h: interface for the CPlateSplice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATESPLICE_H__D7032493_2C1A_4BDF_A9B8_55ADF496C931__INCLUDED_)
#define AFX_PLATESPLICE_H__D7032493_2C1A_4BDF_A9B8_55ADF496C931__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlatePieceObject;
class CPlateBasicIndex;
class CPlateGirderApp;
class CPlateBridgeApp;

class AFX_EXT_CLASS CPlateSplice : public CPlateSpliceBase  
{
public:
	CPlateSplice();
	virtual ~CPlateSplice();

	long m_enTypeNum;
	BOOL m_bSpliceType;
	long GetenTypeNum() const { return m_enTypeNum;}
	void SetenTypeNum(long x) { SETDATA(m_enTypeNum,x)}
public:
	BOOL IsSameByMarkAssem(CPlateBxObject* pLK,long nMarkAssm,long nRow, long nRowLK) const;
	BOOL IsSameByMarkSection(CPlateBxObject* pLK) const;
	virtual DWORD GetLinkType() { return LK_SPLICE; }
	void Serialize(CArchive& ar);  
	CPlateSplice& operator=(const CPlateSplice&	Obj);
	CPlateSplice& operator<<=(const CPlateSplice&	Obj);
	BOOL operator==( const CPlateSplice& Obj);
// Piece Draw
public:	
	void GetXyMatrix(CMatrixPoint *xyMat, long nMarkBuje, long nRow=0, long nCol=0);
	void GetXyMatrixFlangeUpper(CMatrixPoint *xyMat, long nRow, long nCol);
	void GetXyMatrixFlangeLower(CMatrixPoint *xyMat, long nRow, long nCol);
	void GetXyMatrixWebShear(CMatrixPoint *xyMat, long nRow, long nCol);
	void GetXyMatrixWebMoment(CMatrixPoint *xyMat, long nRow, long nCol);

public:
	CString GetMarkstring();
	double GetBoltSize();
	void CreatePlatePieceObj(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, long nSide=0, BOOL bCamberApply=TRUE, long nShape=0);
	void CreatePlatePieceObjUpperUpperSplice(CPlatePieceObject *pObj, long nSide = 0, long nShape=0);
	void CreatePlatePieceBoltObjUpperUpperSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, long nSide=0, long nShape=0);
	void CreatePlatePieceObjUpperLowerSplice(CPlatePieceObject *pObj, long nSide = 0, long nShape=0);
	void CreatePlatePieceBoltObjUpperLowerSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, long nSide = 0, long nShape=0);
	void CreatePlatePieceObjLowerUpperSplice(CPlatePieceObject *pObj, long nSide = 0, long nShape=0);
	void CreatePlatePieceBoltObjLowerUpperSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, long nSide = 0, long nShape=0);
	void CreatePlatePieceObjLowerLowerSplice(CPlatePieceObject *pObj, long Side =0, long nShape=0);
	void CreatePlatePieceBoltObjLowerLowerSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, long nSide = 0, long nShape=0);
	void CreatePlatePieceObjWebMOSHSP(CPlatePieceObject *pObj, BOOL bCamberApply=TRUE, long nShape=0);
	void CreatePlatePieceBoltObjWebMOSHSP(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, BOOL bCamberApply=TRUE, long nShape=0);
	void CreatePlatePieceObjWebSHSP(CPlatePieceObject *pObj, BOOL bCamberApply=TRUE, long nShape=0);
	void CreatePlatePieceBoltObjWebSHSP(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, BOOL bCamberApply=TRUE, long nShape=0);
	void CreatePlatePieceObjWebMOSP(CPlatePieceObject *pObj, BOOL bCamberApply=TRUE, long nShape=0);
	void CreatePlatePieceBoltObjWebMOSP(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, BOOL bCamberApply=TRUE, long nShape=0);
	void CreatePlatePieceObjWebPlan(CPlatePieceObject *pObj, long nSide, long nShape);
	void CreatePlatePieceObjWebPlanFiller(CPlatePieceObject *pObj, long nSide, long nShape);
	
CPlateBasicIndex *m_pBx;
};

#endif // !defined(AFX_PLATESPLICE_H__D7032493_2C1A_4BDF_A9B8_55ADF496C931__INCLUDED_)
