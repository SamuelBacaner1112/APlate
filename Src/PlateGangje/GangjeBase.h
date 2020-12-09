// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// GangjeBase.h: interface for the CGangjeBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GANGJEBASE_H__F832ABE2_AB7B_4B95_B10B_AC9B2604D43C__INCLUDED_)
#define AFX_GANGJEBASE_H__F832ABE2_AB7B_4B95_B10B_AC9B2604D43C__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FIRST_GIRDER	0
#define FIRST_CROSS		1
#define FIRST_STRINGER	2
#define FIRST_WINGBEAM	3
#define FIRST_ENDBEAM	4

// 부재수
#define MAX_ELEMAIN			11
#define MAX_ELESUB			46
#define MAX_KINDNAME		16
#define MAX_PROCESS_MID		38
#define MAX_PROCESS_BIG		11

// 부재종류
#define MATERIAL_KIND_PLATE			 0 
#define MATERIAL_KIND_BOLT			 1
#define MATERIAL_KIND_BOLT2NUT		 2
#define MATERIAL_KIND_ROUNDBAR		 3
#define MATERIAL_KIND_STUDWELD		 4 
#define MATERIAL_KIND_STUDBOLT		 5
#define MATERIAL_KIND_WIREMESH		 6
#define MATERIAL_KIND_PIPE			 7
#define MATERIAL_KIND_REBAR			 8
#define MATERIAL_KIND_ELBOW			 9
#define MATERIAL_KIND_ELBOWEQUAL	10
#define MATERIAL_KIND_SLEEVEFORPIPE	11
#define MATERIAL_KIND_BEAM			12
#define MATERIAL_KIND_AL			13
	
class CPlateBasicIndex;
class CPlateBridgeApp;
class CPlatePieceObject;
class CBMRecord;

class AFX_EXT_CLASS CGangjeBase  
{
public:
	CString GetStringWeight(double dWeight, long nRound);
	long GetMaterialKindNumber(long nMarkBuje);
	CGangjeBase(CPlateBridgeApp* pDB);
	CGangjeBase();
	virtual ~CGangjeBase(); 

	double GetPerimeterBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_RECT, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);
	double GetThickFilletWeld(double T1, double T2);	
	double GetAreaObject(CPlatePieceObject* pObj);

	// 면적을 구하는 함수
	double GetAreaFlangePart(long nMarkBuje, CPlateBasicIndex *pBx, BOOL bUpper = TRUE);

	static CString m_sEleNameMain[MAX_ELEMAIN];
	static CString m_sEleNameSub[MAX_ELESUB];
	static CString m_sKindName[MAX_KINDNAME];
	static CString m_sProcessBig[MAX_PROCESS_BIG];
	static CString m_sProcessMid[MAX_PROCESS_MID];

	double	GetAreaBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_BASIC, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);
	double	GetWeightBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_BASIC, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);
	double	GetWeightJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow=0, long nCol=0, DIMTYPE nDimType=DIM_BASIC, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE, CVector *pJewon=NULL);

	// 공제관련 함수........... 	
	double  GetAreaDeductCrossBeamBracketWeb(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol);
	double  GetAreaDeductCrossBeamBracketFlange(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol);
	double  GetAreaDeductCrossBeamBracketGasset(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol);
	double  GetAreaDeductCrossBeamHStiff(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol, BOOL bFront=TRUE);	
	double  GetAreaDeductWebTaper(CPlateBasicIndex* pBx, BOOL bLeft);
	double  GetAreaDeductBoltHole(long nMarkBuje, CPlateBasicIndex *pBx, CString sBoltSize);
	//
	CVector GetJewonDeductCrossBeamBracketWeb(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol);
	CVector GetJewonDeductCrossBeamBracketFlange(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol);
	CVector GetJewonDeductCrossBeamBracketGasset(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol);
	CVector GetJewonDeductCrossBeamHStiff(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol, BOOL bFront=TRUE);
	CVector GetJewonDeductDiaphragmHStiff(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol, BOOL bFront=TRUE);	
	CVector GetJewonDeductBoltHole(long nMarkBuje, CPlateBasicIndex *pBx, CString sBoltSize);
	CVector GetJewonDeductWebTaper(CPlateBasicIndex* pBx, BOOL bLeft);

protected:
	CPlateBridgeApp* m_pDB;

	////////////////////////////////////////////////////////////////////////
	// 면적관련 함수..........
	double GetAreaFlangeUpper(CPlateBasicIndex *pBx, DIMTYPE nDimType);
	double GetAreaFlangeLower(CPlateBasicIndex *pBx, DIMTYPE nDimType);
	double GetAreaFlangeWeb(CPlateBasicIndex *pBx, DIMTYPE nDimType);
	double GetAreaHStiff(CPlateBasicIndex *pBx, long nRow, BOOL bLeftWeb, DIMTYPE nDimType);
	double GetAreaVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType);
	double GetAreaVJijumStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType);
	double GetAreaVBrGusset(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType); 
	double GetAreaHBrGusset(CPlateBasicIndex *pBx, DIMTYPE nDimType); 
	double GetAreaCrossBeamHStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront);
	double GetAreaCrossBeamVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront);
	double GetAreaCrossBeamBracketWeb(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType);
	double GetAreaCrossBeamBracketGasset(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType);
	double GetAreaCrossBeamBracketGassetPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType);
	double GetAreaCrossBeamBracketFlange(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType);
	double GetAreaDongbariGori(CPlateBasicIndex *pBx, DIMTYPE nDimType);

private:
	double GetAngleCuttingHStiff(CPlateBasicIndex *pBx, CVector vJewon);	
	// 둘레를 구하는 함수들
	double GetPerimeterHStiff(CPlateBasicIndex *pBx, long nRow, BOOL bLeftWeb, DIMTYPE nDimType);
	double GetPerimeterCrossBeamHStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront);
	double GetPerimeterCrossBeamVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront);
	double GetPerimeterDongbariGori(CPlateBasicIndex *pBx, DIMTYPE nDimType);	
};

#endif // !defined(AFX_GANGJEBASE_H__F832ABE2_AB7B_4B95_B10B_AC9B2604D43C__INCLUDED_)
