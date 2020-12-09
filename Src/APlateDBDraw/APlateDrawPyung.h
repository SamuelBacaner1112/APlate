// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawPyung.h: interface for the CAPlateDrawPyung class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_APLATEDRAWPYUNG_H__49FD7A6F_2FDB_4829_974C_2D007F110FE2__INCLUDED_)
#define AFX_APLATEDRAWPYUNG_H__49FD7A6F_2FDB_4829_974C_2D007F110FE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CPlateBasicIndex;
class CPlateGirderApp;
class CAPlateDBDraw;
class AFX_EXT_CLASS CAPlateDrawPyung
{
public:
	CAPlateDrawPyung(CDataManage *pDataMng);
	virtual ~CAPlateDrawPyung();

private:
	CDataManage *m_pDataMng;  

protected:
	long	m_CurDirection;
	CDPoint	m_CurDimPoint;

public:
	long	m_nGirder;
	long	m_nGirdersu;
	long	m_nSwitch;

	enum GIRDRAW_TYPE { DRAW_SLOPTEXT = 0x01 };
	static BOOL	m_InputDomRotate;
	static BOOL IsInputDomRotate();

	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrBx;

public:
	void GetPointHBracingCLOnBx(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, CDPoint &resultStt, CDPoint &resultEnd);

public:	
	void DrawPyung(CDomyun *pDom, long nDrawType=0, BOOL bUpper = TRUE);
	void DrawPyungOnAirialView(CDomyun *pDom);
	//������
	void DrawSlabTotal(CDomyun *pDom);
	void DrawSlabVirtual(CDomyun *pDomP);
	void DrawSlabByBx(CDomyun *pDomP, CPlateBasicIndex *pBxLeftStt, CPlateBasicIndex *pBxLeftEnd,
												CPlateBasicIndex *pBxRightStt, CPlateBasicIndex *pBxRightEnd);
	void DrawSlabCantileverHunchLine(CDomyun *pDomP, CPlateBasicIndex *pBxLeftStt, CPlateBasicIndex *pBxLeftEnd,
												CPlateBasicIndex *pBxRightStt, CPlateBasicIndex *pBxRightEnd);
	void DrawSlabTaperLineStt(CDomyun *pDomP, long nPier);
	void DrawSlabTaperLineEnd(CDomyun *pDomP, long nG, long nPier, int nUpDown, CPlateBasicIndex* pBxLeftBrk, CPlateBasicIndex* pBxRighBrk);
	void DrawJointLine(CDomyun *pDomP, BOOL bSttJoint);
	void DrawTaperLinePyung(CDomyun *pDom, BOOL bStt=TRUE, BOOL bEnd=TRUE);
	void DrawExpJointLine(CDomyun *pDomP, BOOL bSttJoint, long nSide);
	void DrawUpDownLine(CDomyun *pDomP, double dStaStt, double dStaEnd);
	//��ü�Ŵ��׸���
	void DrawGirder(CDomyun *pDom, BOOL bUpper, long nSide=0);	
	//�����̼ǻ����� �Ŵ�����
	void DrawGirderWithenStation(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, double dStaEnd, BOOL bUpper, BOOL bEndCuttingLine=FALSE, BOOL bDrawGirder=TRUE);
	void DrawGirderWithenStationByLine(CDomyun *pDom, CPlateBasicIndex *pBx, double dStaStt, double dStaEnd, BOOL bUpper, BOOL bEndCuttingLine=FALSE);
	//BX������ �Ŵ� �ܰ��� ����
	void DrawGirderWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nSide, long nOption = 0xffff, BOOL bDrawSlab=FALSE);	
	//BX������ �Ŵ� ���� ����
	void DrawGirderWebWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, long nSide, long nLine=2);
	
	//�����극�̽��� �������� ������
	void DrawVBracingFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon=FALSE, BOOL bUpper =TRUE);
	//BX������ ����극�̽��� ������
	void DrawHBracingCLOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, BOOL bThickLine=FALSE);
	void DrawHBracingCLTotalGirder(CDomyun *pDom);
	void DrawHBracingCLGirder(CDomyun *pDom, long nGirStt, long nGirEnd, long nColStt, long nColEnd);
	void DrawHBracingCLWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt=NULL, CPlateBasicIndex *pBxEnd=NULL, BOOL bThickLine=FALSE);	
	//�Ŵ��� ���������� 
	void DrawVStiff(CDomyun *pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL);
	void DrawVStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUseAtHBracingGusset=FALSE, BOOL bUpper=FALSE);
	void DrawVStiffWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL);
	//�����극�̰̽� �Ŵ������������� ������
	void DrawVStiffSplice(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft, BOOL bInputDim);
	void DrawVStiffSpliceOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bInputDim);
	void DrawVStiffSpliceWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft, BOOL bInputDim);
	void DrawVBracingCenterSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim = TRUE);
	//���
	void DrawJackUp(CDomyun *pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL);		
	void DrawJackUpWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL);
	void DrawJSolplatePyung(CDomyun *pDom);
	void DimJackUp(CDomyun *pDom);
	//����������
	void DrawJiJumStiff(CDomyun *pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, BOOL bUpper = TRUE, BOOL bSolePlate = TRUE);
	void DrawJiJumStiffWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, BOOL bUpper = TRUE, BOOL bSolePlate = TRUE);
	void DrawJiJumStiffOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper = TRUE, BOOL bSolePlate = TRUE);
	void DimJijumStiff(CDomyun *pDom);
	//
	void DrawSplicePyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide, BOOL bUpperSp, BOOL bInput, BOOL bSolid = TRUE, BOOL bText=FALSE, long nShape=-2);
	void DrawSpliceWebPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide, BOOL bInput, BOOL bSolid = TRUE);
	void DrawFactJoinPyung(CDomyun *pDom, BOOL bUpper, BOOL bSolid);
	void DrawAbutPierCenterLine(CDomyun *pDom);
	void DrawPierCenterLineOne(CDomyun *pDom, long nPier);
	void DrawJijumLine(CDomyun *pDom, long nJijum);
	void DrawCenterJijum(CDomyun *pDom, BOOL bStt);	
	void DrawCenterLineEachGiderPyung(CDomyun *pDom, BOOL bGirderCenterDraw, BOOL bLineNameShow = TRUE);
	void DrawHatchInRect(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bHatch = TRUE);
	void DrawHatchInRectOnSlab(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	//����������
	void DrawGirderForSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bDivUpDn=FALSE);
	void DrawInputDetSplicePyung(CDomyun *pDom, long nG, long nSp);
	void DrawUpperFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bTitle = TRUE, BOOL bExcel = FALSE, BOOL bPaintMap=FALSE, BOOL bDim=TRUE);
	void DrawLowerFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bTitle = TRUE, BOOL bExcel = FALSE, BOOL bPaintMap=FALSE, BOOL bDim=TRUE);
	void DimUpperSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bExcel=FALSE, BOOL bPaintMap=FALSE);
	void DimLowerSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bExcel=FALSE, BOOL bPaintMap=FALSE);
	// BX ����
	void DrawSpliceWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bInSide, BOOL bInput, BOOL bSolid, BOOL bText=TRUE, long nShape=0);	
	void DrawSpliceUpperOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bInSide, BOOL bInput, BOOL bSolid, BOOL bText=TRUE, long nShape=0);	
	void DrawSpliceLowerOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide, BOOL bInSide, BOOL bInput, BOOL bSolid, BOOL bText=TRUE, long nShape=0);	
	void DrawFactJointOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bSolid);	
	void DrawFactJoinWebPyung(CDomyun *pDom, BOOL bSolid);
	//��, ������
	void DrawShoeBuje(CDomyun *pDomP, CPlateBasicIndex *pBx, long nUpDown, BOOL bSttHalf=TRUE, BOOL bEndHalf=TRUE);
	void DrawSolePlate(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, int nUpDown, long nLine, BOOL bFirstSkip=FALSE, BOOL bLastSkip=FALSE);
	void DrawShoeSymbol(CDomyun *pDom, long nG, long nJijum);
	void DrawShoe(CDomyun *pDom);
	void DrawShoeSymbolPlan(CDomyun *pDom, long nType, double BX, double BY, CDPoint vAng, double ScaleFactor=1.0);
	void SymbolTextCircle(CDomyun *pDom, long nGirder, double dSta, CDPoint &vAng, double dDist, CString &str);
	void DrawAbutPierSymbol(CDomyun *pDom, long g, BOOL bInput = TRUE);
	void DrawGirderNumber(CDomyun *pDom, long g, BOOL bInput = TRUE);
	void DrawJiganInformation(CDomyun *pDom, double dScale);
	double GetSlabTopStt(double dSta);
	double GetSlabBotStt(double dSta);
	// HBracing
	void DrawHBracingSpliceWithenBx(CDomyun* pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, BOOL bJewon = FALSE, long nType=0);
	void DrawHBracingBeamWithenBx(CDomyun* pDom, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, BOOL bJewon = FALSE, BOOL bHalfTrim=FALSE);
	void DimTextJiganUgan(CDomyun *pDom, BOOL bInput = TRUE, BOOL bStt = TRUE, BOOL bEnd = TRUE);
	void DimTextJiganSlabToGirder(CDomyun *pDom);
	void DimTextJiganGirderToShoe(CDomyun *pDom);
	void DimTextJiganLength(CDomyun *pDom);
	// ��Ÿ��
	void DrawCenterLine(CDomyun* pDom, long nG, double dStaStt, double dStaEnd, BOOL bTextRoadCenter=TRUE, BOOL bTextRadius=TRUE, BOOL bKor=TRUE, BOOL bTextRoadCenterEndPos=TRUE);
    void DrawTopBottomViewMark(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bKor=TRUE);
	void DstDrawVStiffPlan(CDomyun *pDomP, double BX, double BY, double Thick, double Width, double MoThick, double rotAng=0, BOOL bSide=TRUE);
	void DstDrawBoltHole(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
					double dD, long nE, double dF,
					double BoltSize, double SlopStt=0, double SlopEnd=0, double dF1=0,
					double RadRotate=0, BOOL bLeft=TRUE, BOOL bRight=TRUE, long nBoltArrange=0, CDPoint xy=CDPoint(0,0));
	void DstDrawBoltLine(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
					double dD, long nE, double dF,
					double BoltSize, double SlopStt=0, double SlopEnd=0, double dF1=0,
					double RadRotate=0, BOOL bLeft=TRUE, BOOL bRight=TRUE, long nBoltArrange=0, CDPoint xy=CDPoint(0,0));
	//���κ� ���� ������ �� ����Ʈ ���� , ������ �Ǵ� �����극�̽� ���
	void DrawCRBeamVBracingFlangeWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt=NULL, CPlateBasicIndex *pBxEnd=NULL, BOOL bUpper=TRUE, BOOL bHalfTrim=FALSE, BOOL bJewon=FALSE);
	void DrawCRBeamVBracingFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper=TRUE, BOOL bHGBr=FALSE);
	void DrawCRBeamWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftHalf=TRUE, BOOL bRightHalf=TRUE, long nLineWeb=2, long nLineFlange=2);
	//���κ� ����Ʈ(��ä��)
	void DrawCRBeamGussetWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide);
	void DrawCRBeamGussetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide);
	void DrawCRBeamBothGussetWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide, BOOL bDivType = FALSE, BOOL bFlangeView = FALSE, BOOL bDomyun = FALSE);
	void DrawCRBeamBothGussetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bBoltHole, BOOL bBoltLine, BOOL bMakeChip, long nSide);
	//���κ� ����Ʈ ��ǰ
	void DrawCRBeamGassetWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftGirder, long nLine=2);
	void DrawCRBeamBothGassetWebOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSide);
	void DrawCRBeamBothGassetWebWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide = 0, BOOL bFlangeView=FALSE);
	void DrawCRBeamGassetFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftGirder, BOOL bBoltHole,BOOL bBoltLine, BOOL bMakeChip, BOOL bFillCircle);
	//��Ÿ
	void DrawCRBeamCLTotalGirder(CDomyun *pDom);
	void DrawCRBeamCLWithenBx(CDomyun *pDom, CPlateBasicIndex *pBxStt=NULL, CPlateBasicIndex *pBxEnd=NULL);
	void DrawCRBeamAnchorBarOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeftHalf=TRUE, BOOL bRightHalf=TRUE);
	void DrawVBracingAnchorBarOnBx(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawCRBeamVStiffnerOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeftHalf=TRUE, BOOL bRightHalf=TRUE);
	void DrawCRBeamSpliceBoltOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, BOOL bHalfBoltOnly=FALSE);
	void DrawCRBeamSplicePlateOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft=TRUE, BOOL bMakeChild=TRUE);
	void DrawCRBeamDimAllDanmyun(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper);
	void DrawCRBeamFillerBothOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft);
	void DrawCRBeamFillerOneOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft);
	void DrawVBracingBeamDimAllDanmyun(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper=TRUE);
	 
	// ��Ÿ �ɺ�
	void DrawFlangeCut(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nBujeMark, long nSide, BOOL bText);
	void DrawSpliceCut(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nSide, BOOL bUpper, BOOL bText);
	void DrawGirderNumber(CDomyun *pDom, CPlateBasicIndex *pBx);

	// ��������
	void AddWeldPyung(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,double rotAng, BOOL bUpper, long nSide, BOOL bWeldMarkOne = FALSE);
	void AddWeldPyungFlange(CDomyun *pDom,CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd,double rotAng, BOOL bUpper, long nSide, BOOL bGrind = TRUE);
	void AddWeldSplicePos(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng, BOOL bUpper, long nSide, BOOL bGrind = TRUE);
	void AddWeldVStiffPos(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double rotAng, BOOL bUpper, long nSide);
	// Key Plan
	void AddKeyPlan(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, double Scale=100, 
		BOOL bSPMarkZigZag = FALSE, BOOL bSpliceMark=TRUE, BOOL bJijumMark=TRUE, 
		BOOL bDirection=FALSE, BOOL bDimension=FALSE, BOOL bDrawCrossBeam=TRUE,
		BOOL bOneLineBeam=TRUE, long nJijumStt=-1, long nJijumEnd=-1,
		BOOL bMarkAssemBig=FALSE, BOOL bMarkBoxNumber=FALSE,
		CString strTextStt=_T(""),CString strTextEnd=_T(""), BOOL bDawDiap=FALSE, double dDistDirection = -1);

	// ��ġ ǥ�� �Լ�
	void DrawPyungVStiffPosMark(CDomyun *pDom, long nG, long nPos);
	void DrawPyungCrossBeamMark(CDomyun *pDom, CPlateBasicIndex *pBx);

	// ���κ� & ��Ʈ���� ��ǥ ��� �� �׸���
	void DrawPlanCrossBeam(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void DrawOutSideCrossBeam(CDomyun* pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft);
	void DrawPlanMainPlateBxOnSlab(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, long nUpDown);
	void DrawPlanOutSidePlateBxOnSlab(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, long nUpDown, BOOL bLeft);
	int  GetPlanMainPlateHunch(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, CDPointArray ptArr[], BOOL bStt);
	int GetPlanOutSidePlateHunch(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxNext, CDPointArray ptArr[], BOOL bStt, BOOL bLeft);
	void ModifyHunchCross(CDPointArray ptArr[], CDPointArray ptArrNext[]);

protected:
//	void DrawAbutPierSymbol(CDomyun *pDomP, long nGirder);		// ���� �� ���� ��ȣ
//	void DrawJiganInformation(CDomyun *pDomP, double dScale);	// ���� ����

//	void DimTextJiganUgan(CDomyun *pDomP);						// ����
//	void DimTextJiganSlabToGirder(CDomyun *pDomP);				// �����꿡�� �Ŵ�
//	void DimTextJiganGirderToShoe(CDomyun *pDomP);				// �Ŵ����� ��
//	void DimTextJiganLength(CDomyun *pDomP);					// ������

//	void SymbolTextCircle(CDomyun *pDomP, long nGirder, double dSta, CDPoint &vAng, double dDist, CString &str);

public:
	void DrawSectionSymbol(CDomyun *pDom);
	void DrawTendonBySta(CDomyun *pDom, double dSta, long nFix);
	void DrawTendonArr(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	double DrawHBracingCalcForXL(CDomyun *pDom, long nBracing, BOOL bLeftSlab);
	void DrawPyungFieldSpliceMark(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DimJSoleplatePyung(CDomyun* pDom);
	void DrawAbutPier(CDomyun* pDom);
//	double GetSlabTopStt(double dSta);	// ������ ��ܽ��� (�����߽ɿ��� �̰ݵȰŸ�)
//	double GetSlabBotStt(double dSta);	// ������ �ϴܽ��� (�����߽ɿ��� �̰ݵȰŸ�)
	void SetCursorPosition(long nG, double staStt, double staEnd, CDomyun *pDom, BOOL bDelCursor=TRUE, double dLineThick=150, long nPos=0, long nLineColor=-1);
	void SetDirection(long nGirder, CString str);

	void DimMoveTo(CDomyun *pDom, double dSta, double dDist, long nCol, CString str =_T(""));
	void DimOneLineTo(CDomyun *pDom,double dSta, double dDistCntr, double dDist, long nCol, CString str =_T(""), BOOL bLeft = TRUE);
	void HPointLine(CDomyun *pDom, double dStaBase, double dTerm, double dDist, BOOL bUpper = TRUE);
	void VPointLine(CDomyun *pDom, double dStaBase, double dDist, BOOL bLeft = TRUE);
	void DimLineTo(CDomyun *pDom, double dDist, long nCol, CString str1, CString str2 =_T(""), BOOL bEndDraw = TRUE, BOOL bTextUp = TRUE);
	void DimLineToOver(CDomyun *pDom, double dDist, long nCol, CString str1, CString str2 =_T(""));
	void TextCircle(CDomyun *pDom, long nGirder, double dSta, CDPoint &vAng, double dDist, CString &str);
	double GetRotateAnglePyung(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	
};

#endif // !defined(AFX_DRAWPYUNG_H__49FD7A6F_2FDB_4829_974C_2D007F110FE2__INCLUDED_)
