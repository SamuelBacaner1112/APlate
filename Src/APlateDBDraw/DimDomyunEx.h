// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DimDomyunEx.h: interface for the CDimDomyunEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIMDOMYUNEX_H__0CFE3D9C_4713_46AE_BBB0_1EE6F62B524D__INCLUDED_)
#define AFX_DIMDOMYUN_H__0CFE3D9C_4713_46AE_BBB0_1EE6F62B524D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DIM_LEFT	0
#define DIM_TOP		1
#define DIM_RIGHT   2
#define DIM_BOTTOM	3

#define DIR_LEFT	-1
#define DIR_CENTER	0
#define DIR_RIGHT	1
#define DIR_NONE	99

#define DIM_HOR		TRUE
#define DIM_VER		FALSE

//#include "ShopOutDC.h"
class CPlateBridgeApp;
class CPlatePieceObject;

class AFX_EXT_CLASS CDimDomyunEx : public CDomyun
{
public:	
	CDimDomyunEx(CDomyun *pDom);
	CDimDomyunEx(CPlateBridgeApp *pDB, CDomyun *pDom);	
	virtual ~CDimDomyunEx();

private:
	CPlateBridgeApp	*m_pDB;
	CDPoint		m_BasePoint;			// 직선 치수용
	CDPoint		m_BaseStaionPoint;		// 곡선 치수용
	CDPoint		m_InitPoint;			// 경사 치수용
	CDPoint		m_VAng;					// 경사 치수용

	long		m_nDir;	
	BOOL		m_bTextDir;
	double		m_SideMarkHeight;
	long		m_nDan;
	BOOL		m_bBlock;
	
	CDomyun		*m_pMainDom;

	static BOOL m_CutExtLine;

public:
    static void SetCutLineOn(BOOL bTrue);

private:
	void DrawLS(CDomyun* p, long nG, double sttSta, double endSta, double dist, double RepLen=600);
	CDPoint Getxy(long nG, double sta, double dist);
	CDPoint Getxy(double DisSTA, double H, double Angle,long nG);
	void BlockDot(double x, double y, double dAng=0, BOOL bStart=TRUE);
	void BlockText(double DisX, double DisY, double x, double y, CString Text);

protected:
	void	DimTextOut(CDPoint xy, CString Str, double RotAng, long nDir, long nAlign = TA_CENTER);
	///////////////////////// 일반적인 수직 수평 치수선 ///////////////////////////////
	CDPoint DimHorMoveTo(CDPoint xy, CString Str, double nLen, BOOL bExtend = FALSE, BOOL bArrow = TRUE);
	CDPoint DimHorMoveTo(double x,double y, CString Str, double nLen, BOOL bExtend = FALSE, BOOL bArrow = TRUE)		{ return DimHorMoveTo(CDPoint(x,y),Str,nLen,bExtend,bArrow); }	
	CDPoint DimHorLineTo(double xDis, double y, const CString &cs, const CString &cs2="",BOOL bOver=FALSE, double nLen=0, BOOL bExtend = FALSE, double dAddLen = 0, BOOL bEdgeArrow = TRUE);

	CDPoint DimVerMoveTo(CDPoint xy, CString Str, double nLen, BOOL bExtend = FALSE, BOOL bArrow = TRUE);
	CDPoint DimVerMoveTo(double x,double y, CString Str, double nLen, BOOL bExtend = FALSE, BOOL bArrow = TRUE)		{ return DimVerMoveTo(CDPoint(x,y),Str,nLen,bExtend,bArrow); }	
	CDPoint DimVerLineTo(double x, double yDis,const CString &cs, const CString &cs2="",BOOL bOver=FALSE, double nLen=0, BOOL bExtend = FALSE, double dAddLen = 0, BOOL bEdgeArrow = TRUE);												
	
	/////////////////////////  선형 수직 수평 치수선   ///////////////////////////////	
	CDPoint DimHorCurveMoveTo(double xSta, double y, long nG, CString Str, double nLen, BOOL bArrow = FALSE, BOOL bExtend = FALSE);
	CDPoint DimHorCurveLineTo(double xDis, double y,const CString &cs,const CString &cs2, long nG, BOOL bExtend,BOOL bOver,double nLen, BOOL bEdgeArrow = TRUE);

	CDPoint DimVerCurveMoveTo(double xSta, double y, long nG, CString Str, double nLen, BOOL bArrow = FALSE, BOOL bExtend = FALSE, BOOL bOver = FALSE);
	CDPoint DimVerCurveLineTo(double x, double yDis,const CString &cs,const CString &cs2, long nG, BOOL bOver, double nLen, BOOL bEdgeArrow = TRUE);												
	
	void DrawSideMark(double x, double y, BOOL bHor);
	void DrawSideMarkCurve(double x, double y, long nG, BOOL bHor);

public:
	void SetMainDom(CDomyun *pDom)				{ m_pMainDom = pDom; }
	void SetBlock(BOOL bBlock)					{ m_bBlock = bBlock; }
	BOOL IsBlock()								{ return m_bBlock; }

	void SetDimDan(long nDan)					{ m_nDan = nDan; }
	long GetDimDan()							{ return m_nDan; }

	void SetDimDir(long nDir)					{ m_nDir = nDir; }
	long GetDimDir()							{ return m_nDir; }

	void SetDimTextDir(BOOL bReverse)			{ m_bTextDir = bReverse; }
	BOOL GetDimTextDir()						{ return m_bTextDir; }

	void SetSideMarkHeight(double Height)		{ m_SideMarkHeight = Height; }
	double GetSideMarkHeight()					{ return m_SideMarkHeight; }
	
	void DimBasePoint(CDPoint BasePoint, long nDan = 0);
	void DimBasePoint(double x, double y, long nDan = 0)	{ DimBasePoint(CDPoint(x,y),nDan); }

	void DimBaseCurvePoint(CDPoint BasePoint, long nDan = 0);
	void DimBaseCurvePoint(double x, double y, long nDan = 0)	{ DimBaseCurvePoint(CDPoint(x,y),nDan); }

	CDPoint GetDimBasePoint()					{ return m_BasePoint; }
	CDPoint GetDimBaseCurvePoint()				{ return m_BaseStaionPoint; }

	void SetVAng(CDPoint vAng)					{ m_VAng = vAng.Unit(); }
	void SetVAng(CDPoint SttP, CDPoint EndP)	{ m_VAng = (EndP - SttP).Unit(); }
	CDPoint GetVAng()							{ return m_VAng; }
		
	CDPoint DimMoveTo(CDPoint xy, long nDan = 0, BOOL bDrawMark = FALSE, CString Str = "", double nLen = 0, BOOL bExtend = FALSE, BOOL bArrow = FALSE);		
	CDPoint DimMoveTo(double x, double y, long nDan = 0, BOOL bDrawMark = FALSE, CString Str = "", double nLen = 0, BOOL bExtend = FALSE, BOOL bArrow = FALSE)		
												{ return DimMoveTo(CDPoint(x,y),nDan,bDrawMark, Str, nLen, bExtend, bArrow); }
	CDPoint DimLineTo(double x, double y, const CString &cs, const CString &cs2="", BOOL bDrawMark = FALSE, BOOL bOver = FALSE, double nLen = 0, BOOL bExtend = FALSE, BOOL bEdgeArrow = FALSE);
	CDPoint DimLineToPiece(double x, double y, double dAddLen, const CString &cs, const CString &cs2="", BOOL bDrawMark = FALSE, BOOL bOver = FALSE, double nLen = 0, BOOL bExtend = FALSE, BOOL bEdgeArrow = FALSE);
	
	CDPoint DimCurveMoveTo(double xSta, double y, long nG, BOOL bDrawMark = FALSE, CString Str = "", double nLen = 0, BOOL bArrow = FALSE, BOOL bExtend = FALSE, BOOL bOver = FALSE);
	CDPoint DimCurveLineTo(double xDis, double y, const CString &cs, const CString &cs2, long nG, BOOL bDrawMark = FALSE, BOOL bExtend = FALSE, BOOL bOver = FALSE, double nLen = 0, BOOL bEdgeArrow = FALSE);

	// 경사 치수
	CDPoint DimAlignMoveTo(CDPoint xy, BOOL bInitSet = TRUE, CString Str="", double nLen=0);
	CDPoint DimAlignMoveTo(double x,double y, BOOL bInitSet = TRUE, CString Str="", double nLen=0)		{ return DimAlignMoveTo(CDPoint(x,y),bInitSet,Str,nLen); }	
	CDPoint DimAlignLineTo(double xDis, double y, const CString &cs, const CString &cs2="",BOOL bOver=FALSE, double nLen=0);

	// Piece 치수
	void DrawExtLine(CDPoint BaseP, CDPoint vPo1, CDPoint vPo2, double dLen, BOOL bLeft, BOOL bUpper);
	void DimArray(CDPointArray &Arr, BOOL bUpper, double nDan, BOOL bTextRot = FALSE, BOOL bTotal = FALSE, double dLeft = 0, CDPoint LeftP = CDPoint(0,0), double dRight = 0, CDPoint RightP = CDPoint(0,0));
	void DimPieceObj(CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace = -1, BOOL bTextRot = FALSE, BOOL bTotal = FALSE, BOOL bMove = FALSE, BOOL bFlgUpper = TRUE);
	void DimPieceObjOnBx(CPlatePieceObject *pObj, __int64 type, P_TYPE ptype, BOOL bUpper, double nDan, long nFace = -1, BOOL bTextRot = FALSE, BOOL bTotal = FALSE, BOOL bMove = FALSE, BOOL bFlgUpper = TRUE);
	void DimPieceObjReverse(CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace = -1, BOOL bTextRot = FALSE, BOOL bTotal = FALSE, BOOL bMove = FALSE, BOOL bFlgUpper = TRUE);
	void DimPieceObjReverseOnBx(CPlatePieceObject *pObj, __int64 type, P_TYPE ptype, BOOL bUpper, double nDan, long nFace = -1, BOOL bTextRot = FALSE, BOOL bTotal = FALSE, BOOL bMove = FALSE, BOOL bFlgUpper = TRUE);
	void DimPieceObjLevelPos(CDPoint BaseP, CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace = -1, BOOL bStart = TRUE, BOOL bDrawLine = FALSE);	
	void DimPieceObjLevelPosHor(CDPoint BaseP, CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace = -1, BOOL bStart = TRUE);	
	//
	void DimPieceFace(CPlatePieceObject *pObj, long nFace, long nDimDan, CString sDir, P_TYPE type, BOOL bTotal);
	void DimPieceFaceExt(CPlatePieceObject *pObj, long nFace, long nDimDan, CString sDir, P_TYPE type, BOOL bTotal, BOOL bEqu = TRUE);
	void DimPieceChamfer(CPlatePieceObject *pObj, long nIdx, long nDir, double R = 0);	
	void DimPieceChamferEx(CPlatePieceObject *pObj, long nIdx, BOOL bStt = TRUE, BOOL bInSide = TRUE, double R = 0);	
	//
	void DimPieceObjEx(CPlatePieceObject *pObj, P_TYPE type, __int64 BxFlag, CString sDir, long nDan, long nFace, BOOL bReverse = FALSE, BOOL bAt = FALSE , BOOL bTotal = FALSE );

private:
	inline double GetDist(const CDPoint& Stt, const CDPoint& End, CString szStr)
	{
		if(szStr=="TOP" || szStr=="BOTTOM") return fabs(End.x - Stt.x);
		else								return fabs(End.y - Stt.y);
	}
};

#endif // !defined(AFX_DIMDOMYUNEX_H__0CFE3D9C_4713_46AE_BBB0_1EE6F62B524D__INCLUDED_)
