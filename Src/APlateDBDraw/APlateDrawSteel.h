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

	ALL    = xyStt���� xyEnd ���� ����(�Ϲ����� ����)
	FROM_STT = ���� ���������� ���� �������� �̰ݰŸ���ŭ ����
	FROM_END = ���� �������� ������ �������� �̰ݰŸ���ŭ ����
	FROM_MID = ���� �߽������� �������� �̰ݰŸ���ŭ ����
	*/
	enum {ALL, FROM_STT, FROM_END, FROM_MID};
	/************************************************************
	nUsingType

	VBRACING = �����극�̽��� ������ �����
	HBRACING = ����극�̽��� ������ �����
	*/
	enum {VBRACING, HBRACING};
	/************************************************************
	nInstallPos

	UPPER_VFLANGE = ��� �����극�̽�
	LOWER_VFLANGE = �Ϻ� �����극�̽�
	LEFT_VFLANGE= ���� ���������극�̽� 
	RIGHT_VFLANGE= ���� ���������극�̽� 
	UPPER_HFLANGE_FIRST = ��� ����극�̽� �Ŵ��� ù��°
	LOWER_HFLANGE_FIRST = �Ϻ� ����극�̽� �Ŵ��� ù��°
	UPPER_HFLANGE_SECOND = ��� ����극�̽� �Ŵ��� �ι�°
	LOWER_HFLANGE_SECOND = �Ϻ� ����극�̽� �Ŵ��� �ι�°
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
