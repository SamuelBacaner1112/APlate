// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// VBracing.h: interface for the CVBracing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VBRACING_H__2185A8A9_454F_4140_A3F9_91CBE6C1435D__INCLUDED_)
#define AFX_VBRACING_H__2185A8A9_454F_4140_A3F9_91CBE6C1435D__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlatePieceObject;
class CPlateBasicIndex;
class CSection;
class CVStiff;

#define	TYPE_VBRACING			0
#define	TYPE_INVVBRACING		1
#define STEELMAT_UPPER			0
#define STEELMAT_LOWER			1
#define STEELMAT_DIAGONAL		2

class AFX_EXT_CLASS CVBracing : public CPlateBxObject  
{
public:
	CVBracing(CSection *pSection);
	virtual ~CVBracing();

	CSection *m_pSection;
	CPlateBasicIndex *GetBx();
	enum{LT, LD, RT, RD, CEN};
	enum{UPPER, LOWER, LEFT, RIGHT};
	enum{HEIGHT, WIDTH, THICK};
	CDPoint xyBoltPos[4];	//이음판 볼트홀 하단 시작위치
public:	
	virtual void Serialize(CArchive& ar);
	CVBracing& operator<<=(const CVBracing& Obj);
	void InitData();
	
	void CreatePieceObj(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE, long nPos=0);
	void CreatePieceObjLTSplice(CPlatePieceObject *pObj);
	void CreatePieceObjLDSplice(CPlatePieceObject *pObj);
	void CreatePieceObjRTSplice(CPlatePieceObject *pObj);
	void CreatePieceObjRDSplice(CPlatePieceObject *pObj);
	void CreatePieceObjCENSplice(CPlatePieceObject *pObj);
	void CreatePieceBoltObjLTSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE);
	void CreatePieceBoltObjLDSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE);
	void CreatePieceBoltObjRTSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE);
	void CreatePieceBoltObjRDSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE);
	void CreatePieceBoltObjCENSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE);

	double GetGiulFrameLine(BOOL bUpper);
	double GetLengthFrameLine(long nPos);
	double GetLengthBeam(long nPos);
	double GetJewonSplice(long nPos, long nDimType);
	CDPoint GetCenterSplice(long nPos);
	CDPoint GetPointFrame(long nPos);
	CDPoint GetVectorFrame(long nPos);

	void GetXyBaseLR(CDPoint &xyCrL, CDPoint &xyCrR);
	CDPoint GetVectorPlan();
	BOOL IsSkew();

	long GetType()	{ return m_nType;}
	void SetType(long nType)	{ m_nType = nType;}

	CSteelMaterialData* GetSteelMaterialData(long nSteel);
	void SetSteelMaterialData(CSteelMaterialData *pSteelMaterialData, long nSteel);
	BOOL IsSectionCheck()				{ return m_bSectionCheck;	}
	void SetSectionCheck(BOOL bCheck)	{m_bSectionCheck = bCheck;	}

public:
	CVStiff *m_pVStiff;

	double m_duL1;
	double m_duL2;
	double m_duL3;
	double m_duL4;
	double m_ddL1;
	double m_ddL2;
	double m_ddL3;
	double m_ddL4;
	double m_duR1;
	double m_duR2;
	double m_duR3;
	double m_duR4;
	double m_ddR1;
	double m_ddR2;
	double m_ddR3;
	double m_ddR4;
	double m_dc1;
	double m_dc2;
	double m_dc3;
	double m_dc4;
	double m_dLH1;
	double m_dLH2;
	double m_dRH1;
	double m_dRH2;
	double m_dgW;
	double m_dS;
	double m_dT;
	double m_duA;
	long   m_uB;
	long   m_uC;
	double m_duE;
	double m_dSL;

	double m_ddA;
	long   m_dB;
	long   m_dC;
	double m_ddE;

	double m_dULG ;
	double m_dURG ;
	double m_dDLG ;
	double m_dDRG ;
	double m_dC1LG;
	double m_dC1RG;
	double m_dC2LG;
	double m_dC2RG;
	double m_dG[8];

	double m_vW_G;
	double m_vW1_G;
	double m_vT_G;

	double m_AnchorDis[10];
	double m_dBoltInterver;

//option..
	CString m_ZzsBoltSize;
	long    m_nAnchorSu;
	long	m_nLevelType;
	BOOL	m_bEdgeCutting;
	BOOL    m_bSameLeftRight;
	BOOL	m_bBoltInterver;
	BOOL    m_bCutLevel;

private:
	long	m_nType;
	BOOL	m_bSectionCheck;
	CSteelMaterialData	*m_pSteelMatUp;		// 상현재
	CSteelMaterialData	*m_pSteelMatLo;		// 하현재
	CSteelMaterialData	*m_pSteelMatDiagonal;	// 사재
};

#endif // !defined(AFX_VBRACING_H__2185A8A9_454F_4140_A3F9_91CBE6C1435D__INCLUDED_)
