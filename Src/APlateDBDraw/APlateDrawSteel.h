// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawSteel.h: interface for the CAPlateDrawSteel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWSTEEL_H__4F3A3B36_C06F_4FBC_9033_301787CBF97E__INCLUDED_)
#define AFX_APLATEDRAWSTEEL_H__4F3A3B36_C06F_4FBC_9033_301787CBF97E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPlateBasicIndex;

class AFX_EXT_CLASS CAPlateDrawSteel
{
public:
	CAPlateDrawSteel(CDataManage *pDataMng);
	virtual ~CAPlateDrawSteel();
private:
	CDataManage *m_pDataMng;
public:
	/************************************************************
	nDrawingType

	ALL    = xyStt에서 xyEnd 까지 생성(일반적인 상태)
	FROM_STT = 빔의 시작점에서 끝점 방향으로 이격거리만큼 생성
	FROM_END = 빔의 끝점에서 시작점 방향으로 이격거리만큼 생성
	FROM_MID = 빔의 중심점에서 양쪽으로 이격거리만큼 생성
	*/
	enum {ALL, FROM_STT, FROM_END, FROM_MID};
	/************************************************************
	nUsingType

	VBRACING = 수직브레이싱을 생성시 사용함
	HBRACING = 수평브레이싱을 생성시 사용함
	*/
	enum {VBRACING, HBRACING};
	/************************************************************
	nInstallPos

	UPPER_VFLANGE = 상부 수직브레이싱
	LOWER_VFLANGE = 하부 수직브레이싱
	LEFT_VFLANGE= 사재 좌측수직브레이싱 
	RIGHT_VFLANGE= 사재 우측수직브레이싱 
	UPPER_HFLANGE_FIRST = 상부 수평브레이싱 거더측 첫번째
	LOWER_HFLANGE_FIRST = 하부 수평브레이싱 거더측 첫번째
	UPPER_HFLANGE_SECOND = 상부 수평브레이싱 거더측 두번째
	LOWER_HFLANGE_SECOND = 하부 수평브레이싱 거더측 두번째
    */
	enum {UPPER_VFLANGE, LOWER_VFLANGE, LEFT_VFLANGE, RIGHT_VFLANGE,
		HFLANGE_FIRST, HFLANGE_SECOND};
	//	  LOWER_HFLANGE_FIRST, LOWER_HFLANGE_SECOND}
	/************************************************************/

	void DrawBeamL   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden);

	void DrawBeamLVal   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden);

	void DrawBeamC    (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden);
	
	void DrawBeamCT   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden);

	
	void DrawBeamI   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden);

	
	void DrawBeamH   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden);

	void DrawBeam   (CDomyun *pDom, CPlateBasicIndex* pBx, long nBeamType, long installDir[4], 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden=FALSE);

};

#endif // !defined(AFX_APLATEDRAWSTEEL_H__4F3A3B36_C06F_4FBC_9033_301787CBF97E__INCLUDED_)
