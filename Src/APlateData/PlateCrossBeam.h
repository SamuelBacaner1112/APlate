// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateCrossBeam.h: interface for the CPlateCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATECROSSBEAM_H__5A857AD2_47BB_4967_8BAD_96BFB41C980E__INCLUDED_)
#define AFX_PLATECROSSBEAM_H__5A857AD2_47BB_4967_8BAD_96BFB41C980E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlatePieceObject;
class CPlateBasicIndex;
class CPlateGirderApp;
class CPlateBridgeApp;
class CSection;
class CVStiff;

#define CR_TYPE_HSTEEL				0
#define CR_TYPE_MANUFACTURE			1
#define CR_TYPE_VSTIFF				2

class AFX_EXT_CLASS CPlateCrossBeam : public CPlateSpliceBase 
{
public:
	CPlateCrossBeam();
	CPlateCrossBeam(CSection *pSection);
	virtual ~CPlateCrossBeam();
	virtual void Serialize(CArchive& ar);

	CPlateCrossBeam& operator<<=(const CPlateCrossBeam& Obj);
	BOOL IsSamePlateJewon(CPlateCrossBeam *pCrComp);
public:
	CSection			*m_pSection;
	CVStiff				*m_pVStiff;	
	void InitData();

	long	m_enTypeNum;
	double	m_H_CR				;			//가로보 높이
	double	m_T1_CR				;			//가로보 하판 두께
	double	m_T2_CR				;			//가로보 상판 두께
	double	m_T3_CR				;			//가로보 복부판 두께
	double	m_P_CR				;			//브라켓 돌출 길이
	double	m_uN_CR				;			//상판 브라켓
	double	m_uM_CR				;			//상판 너비/2
	double	m_dN_CR				;			//하판 브라켓
	double	m_dM_CR				;			//하판 너비/2
	double  m_X_CR				;
	double  m_vW_CR				;			//수직보강재
	double  m_vT_CR				;
	double  m_vL_CR				;
	double  m_vT_G				;			// 거더 연결 수직보강재 두께
	double  m_vW_G				;			// 거더 연결 수직보강재 폭
	double  m_vW1_G				;			// 거더 연결 수직보강재 폭
	double  m_hW_CR				;			//수평보강재
	double  m_hT_CR				;
	long	m_nVStiffsu_CR		;
	long	m_nQtyStringer_CR	;
	long	m_nAnchorsu_CR		;
	long	m_nHStiffUppersu_CR	;
	long	m_nHStiffLowersu_CR	;
	BYTE	m_cVSide_CR			;			//수직보강재 설치 방향 0 : 양쪽, 1 : 전면, 2 : 후면
	BYTE	m_cGasset_CR		;			//브라켓 보강재 유무
	double  m_VStiffDis_CR[9]	;			//수직보강재 간격
	double  m_AnchorDis_CR[9]	;			//슬래브 앵커 간격
	/////////////////////////////////
	double m_dLH1;      
	double m_dLH2;
	double m_dRH1;
	double m_dRH2;
	double m_dbL1;
	double m_dbL2;
	double m_dbL3;
	double m_dbL4;
	double m_dbR1;
	double m_dbR2;
	double m_dbR3;
	double m_dbR4;
	double m_dwB;
	double m_dwR;
	double m_dwH;
	long m_wE;
	long m_wF;
	double m_dltH;
	long m_ltE;
	long m_ltF;
	double m_drtH;
	long m_rtE;
	long m_rtF;
	
	long m_nLevelType;
	long m_nLGirSpType;
	long m_nRGirSpType;

	BOOL m_bBoltInterver;
	double	m_dBoltInterver;
	BOOL m_bScallopUpperFlange;	// 플랜지 상부와 복부판사이에 스캘롭설치(wB, wR, wH위치)
	BOOL m_bScallopLowerFlange; // 플랜지 하부와 복부판사이에 스캘롭설치(wB, wR, wH위치)

	CDPoint xyBoltPos[4];
	enum{B, H, R, S1, S2, S3};

	long m_nFillerUseType;//채움판 Filler 타입..
	long m_dThickFiller;//채움판 두께...

	BOOL m_bMomentPan;
	BOOL m_bConnectionPlate;
	long m_nRadioConnectionPlateInOut;

	BOOL m_bHunchTopConnect;

	double m_uM1;
	double m_uD1;
	double m_uE1;
	double m_uF1;
	double m_uB1;
	double m_dM1;
	double m_dD1;
	double m_dE1;
	double m_dF1;
	double m_dB1;

private:
	long m_nType;
	long m_nSteelIndex;
	BOOL m_bSectionCheck;		// 구조계산서 출력
	CSteelMaterialData	*m_pSteelMaterialData;

	/////////////////////////////////////////////////////////////////////////////
public:
	double GetLengthGirWebToCrWeb(BOOL bLeft);
	long GetsuStringer();
	void CreatePieceObjPlate(CPlatePieceObject *pObj, BOOL bMakeBoltHole = TRUE, BOOL bMakeBoltLine = TRUE, BOOL bMakeChip = FALSE);
	//중앙 복부판
	void CreatePieceObjPlateMainPlate(CPlatePieceObject *pObj);
	void CreatePieceObjPlateMainPlateBolt(CPlatePieceObject* pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine);
	void CreatePieceObjPlateFlange(CPlatePieceObject *pObj);
	//중앙 플랜지 볼트
	void CreatePieceObjPlateFlangeBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine);
	//중앙 복부 전단 이음판
	void CreatePieceObjPlateSpliceWebShear(CPlatePieceObject *pObj);
	//중앙 복부 전단 이음판 볼트
	void CreatePieceObjPlateSpliceWebShearBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir=0);
	//Filler 이음판..
	void CreatePieceObjPlateSpliceWebFiller(CPlatePieceObject *pObj);
	//Filler 이음판 볼트
	void CreatePieceObjPlateSpliceWebFillerBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir=0);	
	//중앙 복부 모멘트 이음판
	void CreatePieceObjPlateSpliceWebMoment(CPlatePieceObject *pObj);
	//중앙 복부 모멘트 이음판 볼트
	void CreatePieceObjPlateSpliceWebMomentBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir=0);
	//블라켓 플랜지
	void CreatePieceObjPlateGassetFlange(CPlatePieceObject *pObj, BOOL bRotate, BOOL bMakeBoltHole, BOOL bMakeBoltLine, BOOL bMakeChip, long nMode = 0);
	//블라켓 플랜지 볼트
	void CreatePieceObjPlateGassetFlangeBolt(CPlatePieceObject *pObj, CDPoint gP[], BOOL bRotate, BOOL bMakeBoltHole, BOOL bMakeBoltLine);
	//블라켓 보강재
	void CreatePieceObjPlateGassetPlate(CPlatePieceObject *pObj);
	void CreatePieceObjPlateGassetPlateByBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE);
	//블라켓 복부판
	void CreatePieceObjPlateGassetMain(CPlatePieceObject *pObj);
	//블라켓 복부판 볼트
	void CreatePieceObjPlateGassetMainBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole=TRUE, BOOL bMakeBoltLine=TRUE);
	//중앙 플랜지 상하판이음판
	void CreatePieceObjPlateSpliceFlange(CPlatePieceObject *pObj);
	void CreatePieceObjPlateSpliceFlangeUpperUpperBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir=0);
	void CreatePieceObjPlateSpliceFlangeUpperLowerBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir=0);
	void CreatePieceObjPlateSpliceFlangeLowerUpperBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir=0);
	void CreatePieceObjPlateSpliceFlangeLowerLowerBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir=0);
	//수직보강재 
	void CreatePieceObjPlateVStiffner(CPlatePieceObject *pObj);
	//블라켓 보강판
	void CreatePieceObjPlateGassetBoard(CPlatePieceObject *pObj);
	//
	void GetXyMatrixWebSH(CMatrixPoint *xyMat, long nRow,long nCol);
	void GetXyMatrixWebFH(CMatrixPoint *xyMat, long nRow,long nCol);
	void GetXyMatrixWebMO(CMatrixPoint *xyMat, long nRow,long nCol);
	void GetXyMatrixFlangeUpper(CMatrixPoint *xyMat, long nRow,long nCol, BOOL bLine = FALSE);
	void GetXyMatrixFlangeLower(CMatrixPoint *xyMat, long nRow,long nCol,BOOL bLine = FALSE);
	void GetXyMatrixFlangeLowerConnectionPlate(CMatrixPoint *xyMat, long nRow,long nCol, BOOL bLine = FALSE, BOOL bCrossConnect = TRUE);
	void GetXyMatrixFlangeUpperConnectionPlate(CMatrixPoint *xyMat, long nRow,long nCol, BOOL bLine = FALSE, BOOL bCrossConnect = TRUE);
	CDPoint GetPointGassetSupPlateEndBx(CPlateBasicIndex *pBx, CDPoint vAng, BOOL bUpper, BOOL bLeft);
	CDPoint GetPointGassetSupPlateByBoltEndBx(CPlateBasicIndex *pBx, CDPoint vAng, BOOL bUpper, BOOL bLeft);
	CPlateBasicIndex *GetBx();
	void GetCrossXyDom(CDPoint* A);
	void	GetXyBaseLR(CDPoint &xyCrL, CDPoint &xyCrR);
	CDPoint GetVectorCross();
	CDPoint GetVectorGasset();
	CDPoint GetVectorPlan();

	long GetHStiffUpperDistRate(double Rate[]) const ;
	long GetHStiffLowerDistRate(double Rate[]) const ;


	static CDPoint GetGassetPlateVertex(long nPoint, double BX, double BY, double dis1, double dis2, double W1, double W2,
			double R, double C, BOOL bLeft, BOOL bFront, CDPoint vAng = CDPoint(0,1), CDPoint vRotate = CDPoint(1,0));

	void SerGirder(CPlateGirderApp* pGir) { m_pGir = pGir; }
	
	double CPlateCrossBeam::GetFillerThick();
	double CPlateCrossBeam::GetFillerBoltLength(CPlateBasicIndex *pBx, BOOL bLeft);

	long GetType()				{ return m_nType;}
	void SetType(long nType)	{ m_nType=nType;}
	CSteelMaterialData* GetSteelMaterialData()	{return m_pSteelMaterialData;	};
	void SetSteelMaterial(CSteelMaterialData *pSteelMaterialData)	{m_pSteelMaterialData=pSteelMaterialData;};

	BOOL IsSectionCheck()				{ return m_bSectionCheck;}
	void SetSectionCheck(BOOL bCheck)	{ m_bSectionCheck = bCheck;	}
	void SetHSteelData();
	BOOL IsSkew();
	BOOL IsLeftHBGu();
	BOOL IsRightHBGu();
};

#endif // !defined(AFX_PLATECROSSBEAM_H__5A857AD2_47BB_4967_8BAD_96BFB41C980E__INCLUDED_)

