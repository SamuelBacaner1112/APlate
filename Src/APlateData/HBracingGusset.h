// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// HBracingGusset.h: interface for the CHBracingGusset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HBRACINGGUSSET_H__4D352748_F7ED_4E83_8F7F_F30E33CB0B08__INCLUDED_)
#define AFX_HBRACINGGUSSET_H__4D352748_F7ED_4E83_8F7F_F30E33CB0B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlatePieceObject;
class AFX_EXT_CLASS CHBracingGusset : public CPlateBxObject  
{
public:
	CHBracingGusset();
	virtual ~CHBracingGusset();
	CHBracingGusset& operator<<=(const CHBracingGusset& Obj);

public:
	enum { LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM };
	enum { MID, FAR};
	enum {NONE, CEN_LU, CEN_RU, CEN_LD, CEN_RD, CEN_LD_RD, CEN_LU_RU, CEN_LU_LD, CEN_RU_RD, ALL};
	enum nGasPos {CUR_STT, CUR_MID, CUR_END, NEXT_STT, NEXT_MID, NEXT_END};

	// 판은 총 두개로, TOP과 BOTTOM으로 분리된다. 거더 좌측이 TOP
	long m_nConnect;
	long m_nType;			// 형강타입	
	long m_nMatchCol[4];	// LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM
	CDPoint xyDir[4];
	double	m_dDis[4];		// 거더중심에서 각 빔까지 떨어진 옵셋거리 - 이하 m_gW까지 m_nMatchCol[]의 여부에 따라감
	double	m_dWidth[4];
	double	m_gW[4];		// 빔끝에서 모따기 연장될 길이(좌우동일)
	double	m_dH[2];		// 거세트판 높이
	double	m_sA;			// 볼트제원
	long	m_sB;
	double	m_sC;
	double	m_sD;
	long	m_sE;
	double	m_sL;			// 볼트의 길이
	double	m_dT;			// 수평브레이싱 거세트판의 두께	
	double	m_ddH;			// 거더 하부플랜지 상면~거세트판 하면의

	// 빔 부재정보를 위한 Section 저장..CreatePieceObj(~) 하면서 저장됨. Dim 등에 사용하므로 Serialize 하지 않는다
	CSection *pSec0;
	CSection *pSec1;
	CSection *pSec2;
	CSection *pSec3;	
	
	BOOL m_bEdgeCutting;	// 현재 무조건 컷팅하지만 후일을 위해 남겨둠..	
	CString m_ZzsBoltSize;
	
public:
	void CreatePieceObj(CPlatePieceObject *pObj1, CPlatePieceObject *pObj2, BOOL bMakeBoltHole, BOOL bMakeBoltLine);

	// 수평브레이싱 한개
	void SetOneHPieceXy(CPlatePieceObject *pObj, CDPoint *xyHg, BOOL bUpper, BOOL bLeft, BOOL bCrossBeam, CDPoint vCross);
	// 가로보 없이 수평브레이싱 두개 (수직브레이싱 위치)
	void SetTwoHPieceXy(CPlatePieceObject *pObj, CDPoint *xyHg);
	// 모든 판에 스캘럽 존재
	void SetScallopXy(CPlatePieceObject *pObj, BOOL bUpper, CDPoint vCross);
	// nCol : 해당Col, pC : 연결된 가로보(NULL:수직브레이싱), dHBeamW : 연결된 횡브레이싱의 폭
	CDPoint GetXy25mmOffset(long nCol, CPlateCrossBeam *pC, double dHBeamW);

	long GetMatchSu() {return m_nMatchCol[0]+m_nMatchCol[1]+m_nMatchCol[2]+m_nMatchCol[3];}
	void Serialize(CArchive &ar);
	void SerializeForOld(CArchive &ar);
	BOOL IsSamePlateJewon(CHBracingGusset *pBrGuComp);

public:
	CPlateBasicIndex* GetBxBracing();
	long GetBoltsu();
	BOOL IsReal();
	BOOL IsReal(long nRow);
	double GetSL();//모든 볼트 길이 자동계산.
};

#endif // !defined(AFX_HBRACINGGUSSET_H__4D352748_F7ED_4E83_8F7F_F30E33CB0B08__INCLUDED_)

