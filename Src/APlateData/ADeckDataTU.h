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

// ���鱸��
#define DRAW_PLAN		0	// ���
#define DRAW_JONG		1	// ����
#define DRAW_CROSS		2	// Ⱦ��

class AFX_EXT_CLASS CADeckDataTU : public CBasicConcInfo  
{
public:
	CADeckDataTU(CDataManage* pManage = NULL);
	virtual ~CADeckDataTU();

public:
	CDataManage* m_pDataManage;
	CPlateBridgeApp* m_pDB;	
	CBMOption	m_BMOption;
	CSafetyTypedPtrArray <CObArray, CRebar*>	m_arrRebarJewonTU;		// ���ǥ�� ö�ٸ���Ʈ
	long		m_nAddRebarsuTU;					// �߰�ö�� ���� 

	CTwinVectorArray m_arrTVectorPlan[20];
	CTwinVectorArray m_arrTVectorPlanHunchStt[20];
	CTwinVectorArray m_arrTVectorPlanHunchEnd[20];
	CTwinVectorArray m_arrTVectorPlanSkewStt[20];
	CTwinVectorArray m_arrTVectorPlanSkewEnd[20];
	CTwinVectorArray m_arrTVectorPlanSkewStt_Cut[20][3];	// ������ �ܺ� ����� [nG][nPos] (nPos 0:������������ö�� 1:������ö�� 2:�ܺ�ö��(Cut Line)) 
	CTwinVectorArray m_arrTVectorPlanSkewEnd_Cut[20][3];	// ������ �ܺ� ����� [nG][nPos] (nPos 0:������������ö�� 1:������ö�� 2:�ܺ�ö��(Cut Line))
//	CTwinVector m_TVLineCutPlanSkewStt[20];
//	CTwinVector m_TVLineCutPlanSkewEnd[20];

	// ���ö��shape
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeTU;	// ���ö������ - [�Ŵ���][�Ŵ�����ö��]
	vector<vector<REBAR_SHAPE> > m_stStrupShapeTU;	// Ⱦ���� ö��(��Ʈ��, �ܺ�, ��Ŀ) - [�Ŵ���][�Ŵ�����ö��]

	// ö�ٸ�ũ
	vector<pair<CString, CDPointArray > >	m_arrMarkPosTU[20];

	double m_dCTCAnchor;	// ��Ŀö�ٰ���
	double m_dRangeDanbuBogang;	// �ܺκ�������
	long m_nQtySabogang;	// �纸������
	long m_nQtyDivideJong;	// ��������Ҽ�
	long m_nQtyHunch;		// ��ġö�� ����
	long m_nQtyAnchor;		// ������ ��Ŀ����(1~3)

	// �Ǻ�
	double m_dCoverUp;		// �Ǻ�(���)
	double m_dCoverDn;		// �Ǻ�(�ϸ�)
	double m_dCoverSd;		// �Ǻ�(����)
	double m_dCoverDanbu;	// �Ǻ�(�ܺ�)
	double m_dCoverHunch;	// �Ǻ�(��ġ)

	// Ⱦ����ö������(����)
	double m_dDiaStrup;		// Ⱦ����ö��
	double m_dDiaDanbu;		// �ܺ�ö��
	double m_dDiaAnchor;	// ��Ŀö��
	double m_dDiaSabong;	// �纸��ö��

	// ������ö������(����)
	double m_dDiaUpper;		// ���
	double m_dDiaLower;		// �ϸ�
	double m_dDiaSide;		// ����
	double m_dDiaEdge;		// �𼭸�

	// ��ġ���� ö������
	double m_dDiaHunch;		// ��ġ����ö�� ����
	double m_dOutLength;	// �����������
	double m_dDisHunch;		// ��ġ����	

	// ��Ŀö������	
	double m_dWidthAnchor;	// ��Ŀ��
	double m_dLengthDolchul;// �������
	double m_dDisAnchor;	// ��Ŀ���� �̰ݰŸ�

	CDPointArray m_arrCTC[20];		// �Ŵ��� ö�ٰ���

public:
	void Serialize(CArchive& ar);
	CLineInfo* GetLineInfo(long nG);

	// ���ġ��
	void MakeDimArr();
	void MakeDimArrSub(long nG);
	// ���shape
	void MakePlanMainRebarTU();
	void MakePlanSabogangRebarTU();
	void MakePlanHunchRebarTU();
	// ����Shape
	void MakeShapeBeryukRebarTU();
	CDPointArray GetRebarShapeByJongName(long nG, CString szName);
	// Ⱦ��Shape
	void MakeShapeStrupRebarTU();
	CDPointArray GetRebarShapeByStrupName(long nG, CString szName);

	double GetStaMainRebar(BOOL bStt, long nG, BOOL bIncludeCover);
	double GetStaAtGir(double dStaCen, CDPoint vAng, long nG);
	double GetRangeMainRebar(long nG, BOOL bIncludeCover);
	double GetLengthSkewTriangle(long nG, BOOL bStt);
	CDPoint GetXySkewRange(long nG, BOOL bStt, BOOL bLeft);
	double GetStaMainRebarExceptSkew(BOOL bStt, long nG);

	// ��Ʈ��, ��ġö���� ���� �ִ뺹���� �β�
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
