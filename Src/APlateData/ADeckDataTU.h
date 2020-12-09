// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ADeckDataTU.h: interface for the CADeckDataTU class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADECKDATATU_H__C93B7A0D_367A_4E0A_8CE6_951563035423__INCLUDED_)
#define AFX_ADECKDATATU_H__C93B7A0D_367A_4E0A_8CE6_951563035423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBasicIndex;
class CAddRate;
class CBMOption;
class CDataManage;

// 도면구분
#define DRAW_PLAN		0	// 평면
#define DRAW_JONG		1	// 종단
#define DRAW_CROSS		2	// 횡단

class AFX_EXT_CLASS CADeckDataTU : public CBasicConcInfo  
{
public:
	CADeckDataTU(CDataManage* pManage = NULL);
	virtual ~CADeckDataTU();

public:
	CDataManage* m_pDataManage;
	CPlateBridgeApp* m_pDB;	
	CBMOption	m_BMOption;
	CSafetyTypedPtrArray <CObArray, CRebar*>	m_arrRebarJewonTU;		// 재료표용 철근리스트
	long		m_nAddRebarsuTU;					// 추가철근 개수 

	CTwinVectorArray m_arrTVectorPlan[20];
	CTwinVectorArray m_arrTVectorPlanHunchStt[20];
	CTwinVectorArray m_arrTVectorPlanHunchEnd[20];
	CTwinVectorArray m_arrTVectorPlanSkewStt[20];
	CTwinVectorArray m_arrTVectorPlanSkewEnd[20];
	CTwinVectorArray m_arrTVectorPlanSkewStt_Cut[20][3];	// 시점부 단부 모따기 [nG][nPos] (nPos 0:기존길이유지철근 1:모따기된철근 2:단부철근(Cut Line)) 
	CTwinVectorArray m_arrTVectorPlanSkewEnd_Cut[20][3];	// 종점부 단부 모따기 [nG][nPos] (nPos 0:기존길이유지철근 1:모따기된철근 2:단부철근(Cut Line))
//	CTwinVector m_TVLineCutPlanSkewStt[20];
//	CTwinVector m_TVLineCutPlanSkewEnd[20];

	// 배력철근shape
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeTU;	// 배력철근좌측 - [거더별][거더내부철근]
	vector<vector<REBAR_SHAPE> > m_stStrupShapeTU;	// 횡방향 철근(스트럽, 단부, 앵커) - [거더별][거더내부철근]

	// 철근마크
	vector<pair<CString, CDPointArray > >	m_arrMarkPosTU[20];

	double m_dCTCAnchor;	// 앵커철근간격
	double m_dRangeDanbuBogang;	// 단부보강구간
	long m_nQtySabogang;	// 사보강개수
	long m_nQtyDivideJong;	// 종방향분할수
	long m_nQtyHunch;		// 헌치철근 개수
	long m_nQtyAnchor;		// 구속콘 앵커개수(1~3)

	// 피복
	double m_dCoverUp;		// 피복(상면)
	double m_dCoverDn;		// 피복(하면)
	double m_dCoverSd;		// 피복(측면)
	double m_dCoverDanbu;	// 피복(단부)
	double m_dCoverHunch;	// 피복(헌치)

	// 횡방향철근제원(직경)
	double m_dDiaStrup;		// 횡방향철근
	double m_dDiaDanbu;		// 단부철근
	double m_dDiaAnchor;	// 앵커철근
	double m_dDiaSabong;	// 사보강철근

	// 종방향철근제원(직경)
	double m_dDiaUpper;		// 상면
	double m_dDiaLower;		// 하면
	double m_dDiaSide;		// 측면
	double m_dDiaEdge;		// 모서리

	// 헌치보강 철근제원
	double m_dDiaHunch;		// 헌치보강철근 직경
	double m_dOutLength;	// 외측돌출길이
	double m_dDisHunch;		// 배치간격	

	// 앵커철근제원	
	double m_dWidthAnchor;	// 앵커폭
	double m_dLengthDolchul;// 돌출높이
	double m_dDisAnchor;	// 앵커간의 이격거리

	CDPointArray m_arrCTC[20];		// 거더별 철근간격

public:
	void Serialize(CArchive& ar);
	CLineInfo* GetLineInfo(long nG);

	// 평면치수
	void MakeDimArr();
	void MakeDimArrSub(long nG);
	// 평면shape
	void MakePlanMainRebarTU();
	void MakePlanSabogangRebarTU();
	void MakePlanHunchRebarTU();
	// 종단Shape
	void MakeShapeBeryukRebarTU();
	CDPointArray GetRebarShapeByJongName(long nG, CString szName);
	// 횡단Shape
	void MakeShapeStrupRebarTU();
	CDPointArray GetRebarShapeByStrupName(long nG, CString szName);

	double GetStaMainRebar(BOOL bStt, long nG, BOOL bIncludeCover);
	double GetStaAtGir(double dStaCen, CDPoint vAng, long nG);
	double GetRangeMainRebar(long nG, BOOL bIncludeCover);
	double GetLengthSkewTriangle(long nG, BOOL bStt);
	CDPoint GetXySkewRange(long nG, BOOL bStt, BOOL bLeft);
	double GetStaMainRebarExceptSkew(BOOL bStt, long nG);

	// 스트럽, 헌치철근을 위한 최대복부판 두께
	double GetMaxWebT(long nG);

	void MakeRebarJewonTU();
	double GetRebarWeightTotal();
	double GetAreaSlab();
	BOOL AddRebar();

	void MakeRebarMarkPosPlanTU(long nG, double dScale);
	CDPointArray GetArrMarkPosByNameTU(long nG, CString szName);
	CDPointArray GetMarkPosArrTU(long nG, CTwinVectorArray *tvArrPlane, double dCTC, double dRatio, BOOL bUpper);
	CRebar* GetRebarByName(CString szName);
	CDPointArray GetXyArrPosByTwinVectorAndDist(CTwinVector tVector1, CTwinVector tVector2, double dCTC);

	void MakeRebarMarkPosJongTU(long nG, double dScale);
	void MakeRebarMarkPosCrossTU(long nG, double dScale);
};

#endif // !defined(AFX_ADECKDATATU_H__C93B7A0D_367A_4E0A_8CE6_951563035423__INCLUDED_)
