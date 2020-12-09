// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PiecePoint.h: interface for the CPlatePiecePoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEPIECEPOINT_H__C54AC207_7642_493A_859A_57E61389FF9D__INCLUDED_)
#define AFX_PLATEPIECEPOINT_H__C54AC207_7642_493A_859A_57E61389FF9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//=====================================================================================================
//	< 데이터의 원칙 > 
//	
//	1. 교축방향과 직각인 부재는 교축방향에서 좌측하단을 시작점으로 한다.
//	2. 교축방향 부재는 시작점 스테이션 위치의 좌측하단을 시작점으로 한다.
//	3. 좌표의 순서는 솔리드 일경우 시계방향을 원칙으로 하고 개구부일 경우는 반시계방향을 원칙으로 한다.
//	4. P_TYPE 정보는 그리기 시작점의 다음점이 정보를 가진다.
//
//=====================================================================================================

// Line Color 

enum 
{
	P_BYLAYER = -1,
	P_RED	= 1,
	P_YELLOW = 2,
	P_GREEN = 3,
	P_CYAN = 4,
	P_BLUE = 5,
	P_MAGENTA = 6,
	P_WHITE = 7,
	P_DARKGRAY = 8,
	P_LIGHTGRAY = 9
};

// Piece Object
const long FACE_NO		= 0x0000;
const long FACE_POINT	= 0x0001;

// 포인트 속성 정의 
typedef __int64 P_TYPE;
const P_TYPE P_TYPE_LINE			= 0x0001;
const P_TYPE P_TYPE_ARC				= 0x0002;
const P_TYPE P_TYPE_ELLIPSE			= 0x0004;
const P_TYPE P_TYPE_SCALLOP			= 0x0008;
const P_TYPE P_TYPE_CHAMFER			= 0x0010;
const P_TYPE P_TYPE_JRIB_T			= 0x0020;
const P_TYPE P_TYPE_JRIB_C			= 0x0040;
const P_TYPE P_TYPE_CIRCLE			= 0x0080;
const P_TYPE P_TYPE_DIMPOINT		= 0x0100;
const P_TYPE P_TYPE_FLANGEMARK		= 0x0200;
const P_TYPE P_TYPE_FILLET			= 0x0400;
const P_TYPE P_TYPE_STTMARK			= 0x0800;
const P_TYPE P_TYPE_BREAK			= 0x1000;
const P_TYPE P_TYPE_SKIP			= 0x2000;
const P_TYPE P_TYPE_OUTLINE	= P_TYPE_LINE | P_TYPE_ARC;
const P_TYPE P_TYPE_PLATE	= P_TYPE_LINE | P_TYPE_ARC | P_TYPE_CIRCLE;


const long POJB_SOLID			= 1;	// 플레이트 속성인지 개구부 속성인지 구분
const long POJB_HAS_ARC			= 2;	// 아크를 가지고 있는가 ?
const long POJB_CLICKWISE		= 3;	// 시계방향 배열인지 반시계 방향인지...
const long POJB_2DIM			= 4;	// 2차원 평면인지 3차원인지...
const long POJB_FOLD			= 5;	// 3차원을 2차원으로 폈다면 ?
const long POJB_HAS_OPEN_UNIT	= 6;	// 개구부를 가지고 있는가 ?		
const long POJB_PIECEMARK		= 7;	// 객체가 아닌 Mark인지를 검사	
const long POJB_LEFT			= 8;	// 좌측 방향이 두께 방향일 경우
const long POJB_USE_CONV_DATA	= 9;	
const long POJB_3D				= 10;	// 좌표배열이 3차원인경우
const long POJB_BOLTHOLE		= 11;	// Vertex가 Bolt Hole 중심인지...

class CPlateBasicIndex;
class CPlateGirderApp;
class CDomyun;
class CPlatePieceData;
class CARoadOptionStd;

class AFX_EXT_CLASS CPlatePiecePoint : public CVector
{
public:
	CPlatePiecePoint();	
	virtual ~CPlatePiecePoint();	
	CPlatePiecePoint(const CDPoint &uXy, CPlateBasicIndex *pBx = NULL, BOOL bContinue = TRUE) 
	{
		x = uXy.x;
		y = uXy.y;
		m_PType = P_TYPE_LINE;		
		m_pBx = pBx;
		m_dLength = 0;
		m_bContinue = bContinue;		
	};
	CPlatePiecePoint(const CDPoint &uXy, P_TYPE type = P_TYPE_LINE, CPlateBasicIndex *pBx = NULL, double dLength = 0, BOOL bContinue = TRUE) 
	{
		x = uXy.x;
		y = uXy.y;
		m_PType = type;		
		m_pBx = pBx;
		m_dLength = dLength;
		m_bContinue = bContinue;				
	};

	operator CVector() { return *(CVector*)this; }
	CDPoint GetPoint()						{ return *(CDPoint*)this; }
	void	SetPoint(const CDPoint &uXy)	{ x = uXy.x; y = uXy.y; }
	void	AddType(P_TYPE type)			{ m_PType = m_PType | type; }
	void	SetType(P_TYPE type)			{ m_PType = type; }
	P_TYPE	GetType()						{ return m_PType; }
	void	SetLength(double len)			{ m_dLength = len; }
	double	GetLength()						{ return m_dLength; }
	BOOL	IsContinue()					{ return m_bContinue; }
	void	SetEnd()						{ m_bContinue = FALSE; }	
	CPlateBasicIndex *GetBx()					{ return m_pBx; }
	void	SetBx(CPlateBasicIndex *pBx)			{ m_pBx = pBx; }
	
public:
	CPlatePiecePoint& operator=(const CVector& obj)
	{
		CVector::operator =(obj);
		return *this;
	}
	CPlatePiecePoint& operator=(const CPlatePiecePoint& obj)
	{
		CVector::operator =(obj);

		m_pBx		= obj.m_pBx;
		m_PType		= obj.m_PType;	
		m_dLength	= obj.m_dLength;	
		m_bContinue	= obj.m_bContinue;		

		return *this;
	};

private:
	CPlateBasicIndex* m_pBx;
	P_TYPE		m_PType;		// 다음점과의 상관관계
	double		m_dLength;		// 스캘럽 R, Chamfer D, Arc Bulge값 등 저장
	BOOL		m_bContinue;	// 다음점의 존재유무		
};

class AFX_EXT_CLASS CPlatePiecePointArray : public CSimpleTypedArray<CPlatePiecePoint> 
{
public:
	CPlatePiecePointArray();
	virtual ~CPlatePiecePointArray();
	DWORD	m_dwAttr;
	CVector m_vNormal;  // 작업면의 법선. (다각형의 법선이 아님!)


//implementation
public:
	void SetSolid(BOOL bSolid)			{ SetBitFlag32(m_dwAttr,POJB_SOLID,bSolid); }
	void SetClockWise(BOOL bClockWise)	{ SetBitFlag32(m_dwAttr,POJB_CLICKWISE,bClockWise); }
	void SetBoltHole(BOOL bBoltHole)	{ SetBitFlag32(m_dwAttr,POJB_BOLTHOLE,bBoltHole); }
	BOOL IsSolid()		const	{ return GetBitFlag32(m_dwAttr,POJB_SOLID); }
	BOOL IsClockWise()	const	{ return GetBitFlag32(m_dwAttr,POJB_CLICKWISE); }
	BOOL IsBoltHole()	const	{ return GetBitFlag32(m_dwAttr,POJB_BOLTHOLE); }

	double GetAreaInfo(CVector &cCenter, BOOL bPolygon = TRUE)	const;
	void GetVectorArray(CVectorArray& vArr,P_TYPE ptype=P_TYPE_OUTLINE)	const;
	void SetVectorArray(const CVectorArray& vArr);

	CPlatePiecePointArray& operator = (const CVectorArray& obj)
	{	SetVectorArray(obj);	return *this;	}
	CPlatePiecePointArray& RotateAsAxis(const CVector &cBase,const CVector &vAxis,const CDPoint &Angle);
	CPlatePiecePointArray& Move(const CVector& vMove);
	CPlatePiecePointArray& Transform(const CVector& cBaseS, const CVector& cBaseT, const CVector& vDir1S,
								const CVector& vDir1T, const CVector& vDir2S, const CVector& vDir2T);
	CPlatePiecePointArray& TransformForHori(const CVector& cBaseS, const CVector& cBaseT, const CDPoint& vXYDir);
	CPlatePiecePointArray& TransformForVert(const CVector& cBaseS, const CVector& cBaseT, const CDPoint& vXYDir);

};

#endif // !defined(AFX_PLATEPIECEPOINT_H__C54AC207_7642_493A_859A_57E61389FF9D__INCLUDED_)
