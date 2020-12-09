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

	// ���� �� �ΰ���, TOP�� BOTTOM���� �и��ȴ�. �Ŵ� ������ TOP
	long m_nConnect;
	long m_nType;			// ����Ÿ��	
	long m_nMatchCol[4];	// LEFTTOP, RIGHTTOP, LEFTBOTTOM, RIGHTBOTTOM
	CDPoint xyDir[4];
	double	m_dDis[4];		// �Ŵ��߽ɿ��� �� ������ ������ �ɼ°Ÿ� - ���� m_gW���� m_nMatchCol[]�� ���ο� ����
	double	m_dWidth[4];
	double	m_gW[4];		// �������� ����� ����� ����(�¿쵿��)
	double	m_dH[2];		// �ż�Ʈ�� ����
	double	m_sA;			// ��Ʈ����
	long	m_sB;
	double	m_sC;
	double	m_sD;
	long	m_sE;
	double	m_sL;			// ��Ʈ�� ����
	double	m_dT;			// ����극�̽� �ż�Ʈ���� �β�	
	double	m_ddH;			// �Ŵ� �Ϻ��÷��� ���~�ż�Ʈ�� �ϸ���

	// �� ���������� ���� Section ����..CreatePieceObj(~) �ϸ鼭 �����. Dim � ����ϹǷ� Serialize ���� �ʴ´�
	CSection *pSec0;
	CSection *pSec1;
	CSection *pSec2;
	CSection *pSec3;	
	
	BOOL m_bEdgeCutting;	// ���� ������ ���������� ������ ���� ���ܵ�..	
	CString m_ZzsBoltSize;
	
public:
	void CreatePieceObj(CPlatePieceObject *pObj1, CPlatePieceObject *pObj2, BOOL bMakeBoltHole, BOOL bMakeBoltLine);

	// ����극�̽� �Ѱ�
	void SetOneHPieceXy(CPlatePieceObject *pObj, CDPoint *xyHg, BOOL bUpper, BOOL bLeft, BOOL bCrossBeam, CDPoint vCross);
	// ���κ� ���� ����극�̽� �ΰ� (�����극�̽� ��ġ)
	void SetTwoHPieceXy(CPlatePieceObject *pObj, CDPoint *xyHg);
	// ��� �ǿ� ��Ķ�� ����
	void SetScallopXy(CPlatePieceObject *pObj, BOOL bUpper, CDPoint vCross);
	// nCol : �ش�Col, pC : ����� ���κ�(NULL:�����극�̽�), dHBeamW : ����� Ⱦ�극�̽��� ��
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
	double GetSL();//��� ��Ʈ ���� �ڵ����.
};

#endif // !defined(AFX_HBRACINGGUSSET_H__4D352748_F7ED_4E83_8F7F_F30E33CB0B08__INCLUDED_)

