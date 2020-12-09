// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ADeckData.h: interface for the CADeckData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADECKDATA_H__D5E29F58_6D9A_484D_99E9_80429B01650A__INCLUDED_)
#define AFX_ADECKDATA_H__D5E29F58_6D9A_484D_99E9_80429B01650A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ö�ٹ����ġ
#define POS_SLAB_LEFT			0
#define POS_SLAB_RIGH			1
#define POS_SLAB_CENTER			2
#define POS_SLAB_TOTAL			3 
// ��ö�� ����
#define CTC_MAIN_UPPER_LEFT		0
#define CTC_MAIN_UPPER_RIGHT	1
#define CTC_MAIN_UPPER_CENTER	2
#define CTC_MAIN_LOWER_LEFT		3
#define CTC_MAIN_LOWER_RIGHT	4
#define CTC_MAIN_LOWER_CENTER	5
#define CTC_MAIN_SIZE			6
// ���ö�� ����
#define CTC_BERYUK_UPPER		0
#define CTC_BERYUK_LOWER		1
#define CTC_BERYUK_SIZE			2
// �纸�� ö�� ����
#define CTC_SABOGANG_UPPER_STT 0
#define CTC_SABOGANG_UPPER_END 1
#define CTC_SABOGANG_LOWER_STT 2
#define CTC_SABOGANG_LOWER_END 3
#define CTC_SABOGANG_SIZE	   4
// �簢�� ö�� ����(���)
#define CTC_SKEWMAIN_UPPER_LEFT_STT 0
#define CTC_SKEWMAIN_UPPER_LEFT_END 1
#define CTC_SKEWMAIN_LOWER_LEFT_STT 2
#define CTC_SKEWMAIN_LOWER_LEFT_END 3
#define CTC_SKEWMAIN_UPPER_RIGH_STT 4
#define CTC_SKEWMAIN_UPPER_RIGH_END 5
#define CTC_SKEWMAIN_LOWER_RIGH_STT 6
#define CTC_SKEWMAIN_LOWER_RIGH_END 7
#define CTC_SKEWMAIN_UPPER_CENTER_STT 8
#define CTC_SKEWMAIN_UPPER_CENTER_END 9
#define CTC_SKEWMAIN_LOWER_CENTER_STT 10
#define CTC_SKEWMAIN_LOWER_CENTER_END 11
#define CTC_SKEWMAIN_SIZE	 	    12
// �簢�� ö�� ����(����)
#define CTC_SKEWJONG_UPPER_CENTER_STT 0
#define CTC_SKEWJONG_UPPER_CENTER_END 1
#define CTC_SKEWJONG_LOWER_CENTER_STT 2
#define CTC_SKEWJONG_LOWER_CENTER_END 3
#define CTC_SKEWJONG_SIZE	 	    4
// ��ö�� ������ġ(����)
#define JONGBASEPOS_SLABCENTER 0	// �������߽�
#define JONGBASEPOS_SLABLEFT   1	// ����������
#define JONGBASEPOS_SLABRIGHT  2	// ���������
#define JONGBASEPOS_SLABLC	   3	// ������������ �߾Ӻ� ���	
#define JONGBASEPOS_SLABRC	   4	// ����������� �߾Ӻ� ���	
#define JONGBASEPOS_ONCB	   5	// �Ŵ�0~�Ŵ�1���� ���κ� �߾���ġ�� ������ �ܸ�
// ���ö�� ��ġ(����)
#define JONGPOS_SLABSTTEND 0	// ������ �ܺ�
#define JONGPOS_SLABCENTER 1	// ������ �߽�
#define JONGPOS_SLABJIJUM  2	// ������ ������
// �����߷� �� �ܸ���
#define BASIC_WEIGHT_UNIT	0
#define BASIC_AREA_UNIT		1
#define BASIC_DIAMEATER		2
#define BASIC_GIRTH			3
// ��ö�� �׸��� �ɼ�(���)
#define DRAW_LEFTSIDE		-1
#define DRAW_RIGHTSIDE		 1
#define DRAW_SIDEALL		 0
// Ⱦ�ܸ� �����ġ
#define SECTION_DAN			0	// ������ ����(��,��)
#define SECTION_CEN			1	// ������ �߾Ӻ�(��,��)
#define SECTION_SKEW_STT	2	// �簢�� ����(��)
#define SECTION_SKEW_END	3	// �簢�� ����(��)
#define SECTION_PIER		4	// ������ ������(��)
#define SECTION_TOTAL		5	

// �Է¿� ���� ����(����Ŭ��)
typedef struct _REBAR_CYCLE
{
	CDoubleArray m_dArrJointUp;		// ��� ������ �߽���ġ	: (��ö��, ���ö��)
	CDoubleArray m_dArrJointDn;		// �Ϻ� ������ �߽���ġ	: (��ö��, ���ö��)		
	CDoubleArray m_dArrDiaUp;		// ��� ö������		: (��ö��, ���ö��)
	CDoubleArray m_dArrDiaDn;		// �Ϻ� ö������		: (��ö��, ���ö��)
	CDoubleArray m_dArrValidUp;		// 0 : ���� 1: ����		: (��ö��, ���ö��)
	CDoubleArray m_dArrValidDn;		// 0 : ���� 1: ����		: (��ö��, ���ö��)

} REBAR_CYCLE;

// ö�����¿� ���� ����(����Ŭ��)
// CDPoint x: ���������κ��� �̰ݰŸ�, y: ��������
typedef struct _REBAR_SHAPE
{
	CDPointArray m_dArrShape;		// ����
	CDPointArray m_dArrInJoint;		// ��������	
	double		 m_dDia;			// ����
	CString		 m_sName;			// ��Ī
	BOOL		 m_bValid;			// 0 : ���� 1: ����
	long		 m_nCycle;			// ���� ����Ŭ
	BOOL		 m_bUpper;			// ���/�Ϻ� ö��
} REBAR_SHAPE;


// ö�����¿� ���� ����(����Ŭ��)
// CDPoint x: ���������κ��� �̰ݰŸ�, y: ��������
typedef struct _REBAR_PLAN
{
	long m_nPos;	// -1:�ܺν���,  0:�Ϲݺ�,  1:�ܺ�����
	long m_nCycle;
	long m_nShape;
	CString m_sName;
	CTwinVector m_tVector;
} REBAR_PLAN;


class CPlateBasicIndex;
class CBoxOpenBar;
class CRoundOpenBar;
class CSupportRFBarSet;
class CAddRate;
class CBMOption;
class CDataManage;

class AFX_EXT_CLASS	CADeckData : public CBasicConcInfo  
{
// Constructor
public:
	CADeckData(CDataManage* pManage = NULL);
	virtual ~CADeckData();

// Attributes	
public:
	CDataManage* m_pDataManage;
	CPlateBridgeApp* m_pDB;	
	CBMOption	m_BMOption;

	enum { SP_LEFT=0, SP_RIGHT=1, SP_BOTH=2 };

	///////////////////////////////////////////////////////////////////////////////////////////
	// �⺻������
	///////////////////////////////////////////////////////////////////////////////////////////
	double m_dCoverUp;					// ��ũ��Ʈ�Ǻ��β�(���)
	double m_dCoverDn;					// ��ũ��Ʈ�Ǻ��β�(�Ϻ�)
	double m_dCoverSd;					// ��ũ��Ʈ�Ǻ��β�(����)
	long   m_nSelectRebarAddRate;		// ö�� ������ ���� ���

	long   m_nRoundRebarConnect;
	long   m_nTypeBMApply;				// �������� �������
	BOOL   m_bIncludeSumZero;			// �������迡 �����(zero) ����
	double m_dLengthRebarMax;
	BOOL   m_bShowMark;					// ����Է�ȭ�鿡�� ö�ٸ�ũ����		
	BOOL   m_bRecalcJewon;				// ����Է�[����]�� ��������
	BOOL   m_bCheckSinchukDetail;
	BOOL   m_bCheckSinchukSimple;
	double m_SigmaCK;					// ��ũ��Ʈ ���� ����  ARoadDesignData Dll�� CFStipData�� ���� �����ؼ� �����.
	double m_SigmaY;					// ö���� ����
	double m_SigmaSA;					// ö���� �������


	///////////////////////////////////////////////////////////////////////////////////////////
	// ��Ÿö��                                                             
	///////////////////////////////////////////////////////////////////////////////////////////
	CBoxOpenBar			m_WaterHoleBoxType;		// Box��ö��  ������ Class
	CRoundOpenBar		m_WaterHoleRoundType;	// Round��ö��  ������ Class
	CSpacer				m_SpacerRebar;			// Spacer Class
	CSupportRFBarSet	m_ShearRebarDanbu;		// �ܺ� ����ö�� Class 
	CSupportRFBarSet	m_ShearRebarJijum;		// ������ ����ö�� Class
	long				m_nTypeWaterHole;		// �����ö�� Ÿ��
	long				m_nWaterHolesu;			// ����� ����
	long				m_nWaterHoleDansu;		// ����� �ܼ� 
	BOOL				m_bInstallShearRebar;	// ���ܺ���ö��  ���� ������ �ƴ���..
	BOOL				m_bInstallWaterHole;	// ���������ö�� ���� ������ �ƴ���..
	///////////////////////////////////////////////////////////////////////////////////////////
	// ö���� ����
	///////////////////////////////////////////////////////////////////////////////////////////
	// ���	
	double m_dDiaMainRebar_Cent[POS_SLAB_TOTAL];			// ��� �Ϲݺ� ��ö�� Dia 
	double m_dDiaMainRebar_Dan[POS_SLAB_TOTAL];				// ��� �ܺ� ��ö�� Dia 
	double m_dDiaMainRebar_Cent2nd[POS_SLAB_TOTAL];			// ��� �Ϲݺ� ��ö�� Dia  -2Cycle
	double m_dDiaMainRebar_Dan2nd[POS_SLAB_TOTAL];			// ��� �ܺ� ��ö�� Dia 	-2Cycle
	double m_dDiaBeryukRebar_Cent[POS_SLAB_TOTAL];			// ��� �Ϲݺ� ���ö�� Dia 
	double m_dDiaBeryukRebar_Dan[POS_SLAB_TOTAL];			// ��� �ܺ� ���ö�� Dia 
	// �Ϻ�
	double m_dDiaMainRebarLower_Cent[POS_SLAB_TOTAL];		// �Ϻ� �Ϲݺ� ��ö�� Dia 
	double m_dDiaMainRebarLower_Dan[POS_SLAB_TOTAL];		// �Ϻ� �ܺ� ��ö�� Dia 
	double m_dDiaMainRebarLower_Cent2nd[POS_SLAB_TOTAL];
	double m_dDiaMainRebarLower_Dan2nd[POS_SLAB_TOTAL];
	double m_dDiaBeryukRebarLower_Cent[POS_SLAB_TOTAL];		// �Ϻ� �Ϲݺ� ���ö�� Dia 
	double m_dDiaBeryukRebarLower_Dan[POS_SLAB_TOTAL];		// �Ϻ� �ܺ� ���ö�� Dia 
	// ���κ� �� ���κ� ���� ö��
	double m_dDiaCrossBeam1;								// ���κ� ��ġ��ö��
	double m_dDiaCrossBeam2;								// ���κ� �ϸ� ö��
	double m_dDiaCrossBeam3;								// ���κ� ��� ö��
	double m_dDiaStringer1;								// ���κ� ��ġ�� ö��
	double m_dDiaStringer2;								// ���κ� �ϸ� ö��
	double m_dDiaStringer3;								// ���κ� ��� ö��
	double m_dDiaOutBrk1;								// �������κ� ��ġ�� ö��
	double m_dDiaOutBrk2;								// �������κ� �ϸ� ö��
	double m_dDiaOutBrk3;								// �������κ� ��� ö��
	// ��Ÿ
	double m_dDiaSabogangUp;							// �纸��ö�� ��� ���� 
	double m_dDiaSabogangDn;							// �纸��ö�� �Ϻ� ����
	double m_dDiaDanbuHunch;							// �ܺ� ��ġ ������ ��ġ ��ö�� Dia 
	// ������ �ٴ���
	double m_dDiaJijumRebarUp[POS_SLAB_TOTAL];			// ������ ��� �������ö�� Dia 
	double m_dDiaJijumRebarDn[POS_SLAB_TOTAL];			// ������ �Ϻ� �������ö�� Dia 
	// �����
	double m_dDiaDMainRebarUpper_Cent;					// ��ö�� ��� Dia 
	double m_dDiaDMainRebarLower_Cent;					// ��ö�� �ϸ� Dia 
	double m_dDiaDBeryukRebarUpper;						// ���ö�� ��� Dia 
	double m_dDiaDBeryukRebarLower;						// ���ö�� ��� Dia 
	double m_dDiaDMainRebar_Dan;						// �ܺκ��� ��ö�� Dia 
	double m_dDiaDBeryukRebar_Dan;						// �ܺκ��� ���ö�� Dia 

	///////////////////////////////////////////////////////////////////////////////////////////
	// ��� �⺻����
	///////////////////////////////////////////////////////////////////////////////////////////
	double m_dGenMainRebarCTC[POS_SLAB_TOTAL];			// ��� �Ϲݺ� ��ö�� CTC
	double m_dGenSupportRebarCTC[POS_SLAB_TOTAL];		// ��� �Ϲݺ� ���ö�� CTC
	double m_dEndMainRebarCTC[POS_SLAB_TOTAL];			// ��� �ܺ� ��ö�� CTC
	double m_dEndSupportRebarCTC[POS_SLAB_TOTAL];		// ��� �ܺ� ���ö�� CTC

	double m_dGenMainRebarCTCLower[POS_SLAB_TOTAL];		// �Ϻ� �Ϲݺ� ��ö�� CTC
	double m_dGenSupportRebarCTCLower[POS_SLAB_TOTAL];	// �Ϻ� �Ϲݺ� ���ö�� CTC
	double m_dEndMainRebarCTCLower[POS_SLAB_TOTAL];		// �Ϻ� �ܺ� ��ö�� CTC
	double m_dEndSupportRebarCTCLower[POS_SLAB_TOTAL];	// �Ϻ� �ܺ� ���ö�� CTC

	double m_dJijumRebarCTCUp[POS_SLAB_TOTAL];			// ������ 1�� �������ö�� CTC
	double m_dJijumRebarCTCDn[POS_SLAB_TOTAL];			// ������ 2�� �������ö�� CTC

	double m_dSabogangRebarCTCUp;						// �纸��ö��(���) CTC 	
	double m_dSabogangRebarCTCDn;						// �纸��ö��(�Ϻ�) CTC 	
	
	// ����� ö��
	double	m_dDMainRebarCTCUpper;					// ��ö�� ��� ����
	double	m_dDMainRebarCTCLower;					// ��ö�� �ϸ� ����
	double	m_dDBeryukRebarCTCUpper;				// ���ö�� ��� ����
	double	m_dDBeryukRebarCTCLower;				// ���ö�� ��� ����
	double	m_dDDanBogangMainCTC;					// �ܺκ��� ��ö�� ����
	double	m_dDDanBogangBeryukCTC;					// �ܺκ��� ���ö�� ����
	
	double m_dErrorRangeCTC;					    	// ��ö�� ���� ��������
	double m_dTrimRatio;								// Ʈ������		

	///////////////////////////////////////////////////////////////////////////////////////////
	// ��� ��������
	///////////////////////////////////////////////////////////////////////////////////////////
	CDPointArray m_arrCTCBeryukRebarDan[CTC_BERYUK_SIZE];// ���ö�� ���� : �ܺ�
	CDPointArray m_arrCTCBeryukRebarCen[CTC_BERYUK_SIZE];// ���ö�� ���� : �߾Ӻ�
	CDPointArray m_arrCTCBeryukRebarJijum[CTC_BERYUK_SIZE];	// ���ö�� ���� : ������
	CDoubleArray m_arrVariableBeryukRebar[CTC_BERYUK_SIZE];	// ���ö�� ����
	
	CDPointArray m_arrCTCMainRebar[CTC_MAIN_SIZE];			// ���� ���ܸ� ��ö�ٰ���
	CDPointArray m_arrCTCMainRebarForDim[CTC_MAIN_SIZE];	// ���� ���ܸ� ��ö�ٰ���(ġ������)
	CDPointArray m_arrCTCSabogangRebar[CTC_SABOGANG_SIZE];	// �纸��ö�� ����	
	CDPointArray m_arrCTCMainRebarAtSkew[CTC_SKEWMAIN_SIZE];// �纸������ ��ö�� ����
	CDPointArray m_arrCTCMainRebarAtSkewForDim[CTC_SKEWMAIN_SIZE];// �纸������ ��ö�� ����(ġ������)
	CDPointArray m_arrCTCMainRebarAtSkewJong[CTC_SKEWJONG_SIZE];// �簢�� ��ö�� ����-����
	CDPointArray m_arrCTCMainRebarAtSkewJongForDim[CTC_SKEWJONG_SIZE];// �簢�� ��ö�� ����(ġ������)-����
	CDPointArray m_arrCTCMainRebarUnderSinchuk;				// ���������� �ٷ� �Ʒ��� ��ö�� ����
	CDPointArray m_arrCTCBogangRebarAtCrossBeam;			// ���κ�����ö���� ����
	CDPointArray m_arrIndexBogangRebarAtCrossBeam;			// ���κ�����ö��(nG, nCB)
	CDPointArray m_arrCTCBogangRebarAtStringer;				// ���κ�����ö���� ����
	CDPointArray m_arrCTCBogangRebarAtOutBrk;				// �������κ�����ö���� ����
	CDPointArray m_arrCTCMainRebarDanHunch;					// �ܺ� ��ġ ������ ����
	CDPoint		 m_CTCBasicCrossBeam;						// ���κ�����ö���� ���ݽ� ǥ��ġ��
	CDPoint		 m_CTCBasicStringer;						// ���κ�����ö���� ���ݽ� ǥ��ġ��
	CDPoint		 m_CTCBasicOutBrk;							// �������κ�����ö���� ���ݽ� ǥ��ġ��
	CVectorArray m_arrStringer;								// ���κ����� ��ġ(x:�Ŵ�, y:���κ�, z:���κ�)

	//////////////////////////////////////////////////////////////////////////
	// ��ġ����
	//////////////////////////////////////////////////////////////////////////
	double m_dRangeDanbuStt;		// �ܺ� ���� ��ö�� ��ġ ����
	double m_dRangeDanbuEnd;		// �ܺ� ���� ��ö�� ��ġ ����

	//////////////////////////////////////////////////////////////////////////
	// ��ٻ���Ŭ
	//////////////////////////////////////////////////////////////////////////
	vector <REBAR_CYCLE> m_stMainCycleDan;		// �ܺ� ��ö�� cycle�� ���� ����
	vector <REBAR_CYCLE> m_stMainCycleCen;		// �Ϲݺ� ��ö�� cycle�� ���� ����
	vector <REBAR_CYCLE> m_stMainCyclePier;			// ������ ��ö�� cycle�� ���� ����
	vector <REBAR_CYCLE> m_stMainCycleSabogangStt;	// �纸�� ��ö�� cycle�� ���� ����
	vector <REBAR_CYCLE> m_stMainCycleSabogangEnd;	// �纸�� ��ö�� cycle�� ���� ����
	
	vector <REBAR_CYCLE> m_stBeryukCycleLeft;		// ���ö�� cycle�� ���� ����(����������)	
	vector <REBAR_CYCLE> m_stBeryukCycleRigh;		// ���ö�� cycle�� ���� ����(���������)	
	vector <REBAR_CYCLE> m_stBeryukCycleCent;		// ���ö�� cycle�� ���� ����(�������߾�)
	vector <REBAR_CYCLE> m_stBeryukCycleLC;			// ���ö�� cycle�� ���� ����(������������)
	vector <REBAR_CYCLE> m_stBeryukCycleRC;			// ���ö�� cycle�� ���� ����(�����������)
	vector <REBAR_CYCLE> m_stBeryukCycleOnCB;		// ���ö�� cycle�� ���� ����(�Ŵ�1~�Ŵ�2 ���κ��� �߾Ӵܸ�)

	//////////////////////////////////////////////////////////////////////////
	// ö�����¿� ���� ���� -    ����Ŭ=>Shape
	//////////////////////////////////////////////////////////////////////////
	vector<vector<REBAR_SHAPE> > m_stMainShapeDan;	// ������ ���� ��ö�� Shape�� ���� ����
	vector<vector<REBAR_SHAPE> > m_stMainShapeCen;	// �Ϲݺ� ��ö�� Shape�� ���� ����
	vector<vector<REBAR_SHAPE> > m_stMainShapePier;		// ������ ��ö�� Shape�� ���� ����
	vector<vector<REBAR_SHAPE> > m_stMainShapeSkewStt;	// ���� �纸�� ��ö�� Shape�� ���� ����
	vector<vector<REBAR_SHAPE> > m_stMainShapeSkewEnd;	// ���� �纸�� ��ö�� Shape�� ���� ����
	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeLeft;	// ���ö�� Shape�� ���� ����(����������)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeRigh;	// ���ö�� Shape�� ���� ����(���������)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeCent;	// ���ö�� Shape�� ���� ����(�������߾�)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeLC;		// ���ö�� Shape�� ���� ����(������������)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeRC;		// ���ö�� Shape�� ���� ����(�����������)	
	vector<vector<REBAR_SHAPE> > m_stBeryukShapeOnCB;	// ���ö�� Shape�� ���� ����(�Ŵ�1~�Ŵ�2 ���κ��� �߾Ӵܸ�)

	///////////////////////////////////////////////////////////////////////////////////////////
	// ��ٹ���ɼ�
	///////////////////////////////////////////////////////////////////////////////////////////
	long m_nTypeDanbuTaperBeryukRebar;					// �ܺ� Taper ��ġ ���ö�� ����
	BOOL m_bInstallDanbuTaperMainRebar;					// �ܺ��� Taper ��ġ ��ö�� ����
	BOOL m_bInstallMainRebarAtBlockOut;					// ��������ũ��Ʈ ��ö�� ��ġ
	BOOL m_bInstallBeryukRebarAtBlockOut;				// ��������ũ��Ʈ ���ö�� ��ġ
	long m_nJongCtcCalcPos;								// ���ܸ� ö��CTC ������ġ
	long m_nJongDrawPos;								// ���ܸ������ġ
	long m_nJijumBogangArrange;							// [�ٰ氣]�������� ��ö�� ��ġ
	long m_nOptMainRebarArrange;						// [�米��]��ö�� ��ġ
	long m_nOptSabogangRebarRange;						// [�米��]�纸��ö�� ��ġ�����ɼ�
	long m_nOptMainRebarAtSkewRange;					// [�米��]�簢�� ��ö�� ��ġ�����ɼ�
	long m_nOptBeryukRebarArrange;						// ���ö�� ¥���� ġ�� �ɼ�
	double m_dRangeSabogangRebarStt;					// [�米��]�纸��ö�� ��ġ����(������)
	double m_dRangeMainRebarAtSkewStt;					// [�米��]�簢�� ��ö�� ��ġ����(������)
	double m_dRangeSabogangRebarEnd;					// [�米��]�纸��ö�� ��ġ����(������)
	double m_dRangeMainRebarAtSkewEnd;					// [�米��]�簢�� ��ö�� ��ġ����(������)
	long  m_nUseJujangRebar;							// ������ ö�� ��� �ɼ� 0: ��ö�ٷ��� 1/3�� �������� 1/16���� ������  1 : ��ö�ٷ��� 1/2�� �������� 1/16���� ������
	int	  m_nTypeGuardCalc;								//��ȣ�� ���� ��� ���.
	long m_nDetailMainUpper;							// �����ö�� ��ġ �ɼ� (0 : ����Ÿ�� 1 : ��Ÿ��)
	long m_nDetailMainLowerCenter;						// �߾Ӻ� �Ϻ� ��ö�� ��ġ �ɼ� (0 : �и���ġ 1 : �߾Ӻι�ġ 2 : ��ü��ġ) 
	BOOL m_bInstallCrossBeamUpperBogangRebar;			// ���κ� ��麸��ö�� ��ġ �ɼ�(����ö��)
	BOOL m_bInstallStringerUpperBogangRebar;			// ���κ� ��麸��ö�� ��ġ �ɼ�(����ö��)
	BOOL m_bInstallOutBrkUpperBogangRebar;				// �������κ� ��麸��ö�� ��ġ �ɼ�(����ö��)	
	long m_nDetailSettlement;							// ������ ��ġ�ɼ�		(0 : // ��ġ ��ܺ� ���� 1: // ��ġ �ϴܺ� ����)
	long m_nSpaceTrimTypeMain;							// ��ö�� ¥���� ġ�� �ɼ� <������>
	long m_nCTCOptionGen;								// ���ö�� �Ϲݺ� CTC �ɼ�(Ȯ����)
	long m_nCTCOptionPier;								// ���ö�� ������ CTC �ɼ�(Ȯ����)
	BOOL m_bApplyBeryukCTC;								// ���ö��CTC���� ����(�������������)
	long m_nMainCyclesu;								// ��ö�� ����Ŭ��
	BOOL m_bMainRebarLowerLevel;						// ��ö�� �Ϻ� ö�� Level����(ö������)
	BOOL m_bPlusToSlabRebar;
	BOOL m_bDivideRebarAtCant;							// ĵƿ�����ο� ��ġ ���� ��� ��ö���� �и����� ����
	//////////////////////////////////////////////////////////////////////////
	// ��ٱ��̺���
	//////////////////////////////////////////////////////////////////////////
	double m_dLengthMainDolchulUp[2];			// �ܺ� �����ö���� ��Ÿ���� ��� �������( 0 : ����, 1 : ����)
	double m_dLengthMainDolchulLo[2];			// �ܺ� �Ϻ���ö���� ��Ÿ���� ��� �������
	double m_dLengthMainDolchulUpDan[2];		// �ܺ� �����ö���� ��Ÿ���� ��� �������
	double m_dLengthMainDolchulLoDan[2];		// �ܺ� �Ϻ���ö���� ��Ÿ���� ��� �������
	double m_dRangeDistribRebarOnCant;			// ĵƿ������ ��±� ����(Sharp���ܿ��� �Ŵ��������� ����)
	double m_dWidthSlabCantLeftForBeryuk;		// ���� ĵƿ������ ��±� ����(�������������ܿ��� ����)
	double m_dWidthSlabCantRighForBeryuk;		// ���� ĵƿ������ ��±� ����(������������ܿ��� ����)
	double m_dRangeDistribRebarOnGirder;		// �Ŵ��� ��±� ����
	double m_dLengthCrossBeamBogang;			// ���κ���κ���ö���� ����
	double m_dLengthCrossBeamBogangLower;		// ���κ��Ϻκ���ö���� �̰ݱ���(������÷��� ����~)
	double m_dLengthStringerBogang;				// ���κ��ϸ麸��ö���� ����
	double m_dLengthOutBrkBogang;				// �������κ��ϸ麸��ö���� ����
	double m_dLengthOutBrkBogangLower;			// �������κ��Ϻκ���ö���� ��������
	double m_dRangeMainRebarFromFlangeStt;		// ĵƿ������ ��ö���� �÷��� ���ܿ��� �߽ɹ������� ����Ÿ�
	double m_dLengthBeryukSttA;					// ���ö�� ������ �������
	double m_dLengthBeryukSttB;					// ���ö�� ������ ��ġ�� �������1
	double m_dLengthBeryukSttC;					// ���ö�� ������ ��ġ�� �������2
	double m_dLengthBeryukEndA;					// ���ö�� ������ �������
	double m_dLengthBeryukEndB;					// ���ö�� ������ ��ġ�� �������1
	double m_dLengthBeryukEndC;					// ���ö�� ������ ��ġ�� �������2
	double m_dRangeDistribRebarDup;				// ��±��� ��ġ�� ����

	//////////////////////////////////////////////////////////////////////////
	// ö������
	//////////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CRebar*>	m_arrRebarJewon;		// ���ǥ�� ö�ٸ���Ʈ
	CSafetyTypedPtrArray <CObArray, CRebar*>	m_arrRebarJewonGuard;	// ���ǥ�� ö�ٸ���Ʈ
	long		m_nAddRebarsu;					// �߰�ö�� ���� 
	CAddRate	m_AddRateRebar;					// ö�� ������ 

	//////////////////////////////////////////////////////////////////////////
	// ö�ٸ�ũ
	//////////////////////////////////////////////////////////////////////////
	vector<pair<CString, CDPointArray > >		m_arrMarkPosPlan[20];	// ö�ٸ�ũ(������ : �ִ� ���� 20��)
	long   m_nRebarNo;							// ��ũ��ȣ ������ �����..

	//////////////////////////////////////////////////////////////////////////
	// ��鵵 Trim���� ����
	//////////////////////////////////////////////////////////////////////////
	CDoubleArray m_arrTrimRange;			// ��ٵ� Trim ������ ���� ���� - Ȧ���� : �׸���, ¦���� : Trim	
	CDPointArray m_arrSangsePosCB;			// ���κ� ��ٻ�		- x,y(�����ġ)
	CDPointArray m_arrSangsePosST;			// ���κ� ��ٻ�		- x,y(�����ġ)
	CDPointArray m_arrSangsePosOBK;			// �������κ� ��ٻ�  - x,y(�����ġ)
	CVectorArray m_arrSangsePosDan;			// �ܺ� ��ٻ� �׸��� ��ġ       - x,y(�����ġ), z:-1(����), 1(����)
	CVectorArray m_arrSangsePosCant;		// ĵƿ������ ��ٻ� �׸��� ��ġ - x,y(�����ġ), z:-1(����), 1(����)
	CDPointArray m_arrSangseIndexCB;		// ���κ� ��ٻ�		- x:�Ŵ���ȣ, y:���κ���ȣ
	CDPointArray m_arrSangseIndexST;		// ���κ� ��ٻ�		- x:�Ŵ���ȣ, y:���κ���ȣ
	CDPointArray m_arrSangseIndexOBK;		// �������κ� ��ٻ�  - x:-1(����), 1(����) y:�������κ���ȣ
	double       m_dDiaSangsePosCB;			// ���κ� �󼼿��� ����
	double       m_dDiaSangsePosST;			// ���κ� �󼼿��� ����
	double       m_dDiaSangsePosOBK;		// �������κ� �󼼿��� ����
	double       m_dDiaSangsePosDan;		// �ܺ� �󼼿��� ����
	double       m_dDiaSangsePosCant;		// ĵƿ���� �󼼿��� ����
	
	//////////////////////////////////////////////////////////////////////////
	// ����ٽ�����ġ [ 0=���, 1=�Ϻ� ]
	//////////////////////////////////////////////////////////////////////////
	vector<REBAR_PLAN> m_vPlanMainRebar[2];				// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanMainRebarLeft[2];			// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarRigh[2];			// DRAW_RIGH_SIDE

	vector<REBAR_PLAN> m_vPlanMainRebarSkewStt[2];		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanMainRebarSkewLeftStt[2];	// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarSkewRighStt[2];	// DRAW_RIGH_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarSkewEnd[2];		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanMainRebarSkewLeftEnd[2];	// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanMainRebarSkewRighEnd[2];	// DRAW_RIGH_SIDE

	vector<REBAR_PLAN> m_vPlanSabogangStt[2];			// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanSabogangLeftStt[2];		// DRAW_LEFT_SIDE	
	vector<REBAR_PLAN> m_vPlanSabogangRighStt[2];		// DRAW_RIGH_SIDE
	vector<REBAR_PLAN> m_vPlanSabogangEnd[2];			// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanSabogangLeftEnd[2];		// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanSabogangRighEnd[2];		// DRAW_RIGH_SIDE

	vector<REBAR_PLAN> m_vPlanUnderSinchukStt;		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanUnderSinchukLeftStt;	// DRAW_LEFT_SIDE	
	vector<REBAR_PLAN> m_vPlanUnderSinchukRighStt;	// DRAW_RIGH_SIDE
	vector<REBAR_PLAN> m_vPlanUnderSinchukEnd;		// DRAW_SIDE_ALL 
	vector<REBAR_PLAN> m_vPlanUnderSinchukLeftEnd;	// DRAW_LEFT_SIDE
	vector<REBAR_PLAN> m_vPlanUnderSinchukRighEnd;	// DRAW_RIGH_SIDE
	
	//////////////////////////////////////////////////////////////////////////
	// LB DECK
	//////////////////////////////////////////////////////////////////////////
	double m_dHeightLatticeBarDanbu;		// �ܺ� Lattice Bar ����
	double m_dHeightLatticeBarGen;			// �Ϲݺ� Lattice Bar ����
	long m_nOptionLBDeckTensionRebarCTC;	// ����ö�� CTC �ɼ�
	long m_nOptionLBDeckLatticeRebarCTC;	// Lattice Bar CTC �ɼ�
	double m_dLengthLBDeckDanbuSettle;		// �ܺ� ���κ����� �г��� ��ġ�� ����

	CProgressCtrl m_progressCtrl;
	CStatusBar*	  m_pWndStatusBar;

// Implementation
public:
	void Serialize(CArchive& ar);
	void SerializePlan(CArchive& ar);

	//////////////////////////////////////////////////////////////////////////
	// ö�����¿� ���� ��������
	//////////////////////////////////////////////////////////////////////////
	void MakeShapeMainRebar(long nSection);
	void MakeShapeMainRebarBySlabOutLine(long nSection, long nCycle, BOOL bUpper, vector<REBAR_SHAPE>& vRebarShape);
	void MakeShapeMainRebarLowerHasSlabHunch(long nSection, long nCycle, vector<REBAR_SHAPE>& vRebarShape);

	void MakeShapeMainRebarAtUpperGirderWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	void MakeShapeMainRebarAtUpperCbStgWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	void MakeShapeMainRebarAtUpperOutBkWithoutCycle(long nSection, long nCycle, vector <REBAR_SHAPE>& vRebarShape);

	void AddRebarShapeToRebarShape(vector<REBAR_SHAPE>& vRebarShapeOrigin, vector<REBAR_SHAPE>& vRebarShapeToAdd);

	void MakeShapeBeryukRebar();
	void MakeShapeBeryukRebarUp(long nJongBasePos, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	void MakeShapeBeryukRebarDn(long nJongBasePos, long nCycle, vector <REBAR_SHAPE>& vRebarShape);
	//////////////////////////////////////////////////////////////////////////
	// ���ö��CTC ���� ��������
	//////////////////////////////////////////////////////////////////////////
	void MakeDimArrAtSection(long nSection, CDPointArray &xyArrDim, BOOL bUpper);
	void MakeDimArrAtSectionTU(long nSection, CDPointArray &xyArrDim, BOOL bUpper);	// TU�Ŵ��� ���ö�� (���� �ܺ� �Ϻθ� ������ 081113)
	void MakeDimArrAtDanbu();	
	void MakeDimArrAtJijum();
	void MakeDimArrAtCenterStrength();		// ���� �����
	void MakeDimArrAtCenterExperience();	// ������ �����
	double MakeDimArrAtCenterStrengthUp(long nSection, CDPointArray &xyArrDim);
	void MakeDimArrAtCenterStrengthDn(long nSection, CDPointArray &xyArrDim, double dExtLength);

	//////////////////////////////////////////////////////////////////////////
	// ö�������� ���� ���� ���
	//////////////////////////////////////////////////////////////////////////
	void MakeRebarJewon();					// ö����������
	void MakeRebarJewon_Main_Danbu(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);
	void MakeRebarJewon_Main_GenNoHunch(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);
	void MakeRebarJewon_Main_GenLowerAtHunch(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);
	void MakeRebarJewon_Beryuk(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, long nCycle);// ö���������� - ���ö��
	void MakeRebarJewon_Skew(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bSttSlab);				// ö���������� - �纸��ö��
	void MakeRebarJewon_SkewMainRebar(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bSttSlab);		// ö���������� - �簢�� ��ö��
	void MakeRebarJewon_CrossBeam(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);		// ö���������� - ���κ� ����ö��(C1, C2)
	void MakeRebarJewon_Stringer(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);			// ö���������� - ���κ� ����ö��(S1, S2)
	void MakeRebarJewon_OutBrk(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);			// ö���������� - �������κ� ����ö��(OB1, OB2, OB3)
	void MakeRebarJewon_Etc(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon);				// ö���������� - ������, ���ܺ���ö��, ����� ����ö��
	void MakeRebarJewon_GuardFence(CSafetyTypedPtrArray <CObArray, CRebar*> *pArrJewon, BOOL bCalcMeter = FALSE);	// ö���������� - ��ȣ��
	CRebar* GetRebarByName(CString szName);	// ö���� m_dScription�� �̿��Ͽ� ö���� ����

	//////////////////////////////////////////////////////////////////////////
	// ö�ٸ�ũ�� ���� ���� ���
	//////////////////////////////////////////////////////////////////////////
	void MakeRebarMarkPosPlan(long nJigan, double dScale);
	void MakeRebarMarkPosPlanMain(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos);
	void MakeRebarMarkPosPlanMainNoDiv(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos);	
	void MakeRebarMarkPosPlanMainDiv(long nJigan, BOOL bUpper, vector < pair<CString, CDPointArray > > &vMarkPos);	
	void MakeRebarMarkPosPlanMainLowerEtc(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);	
	void MakeRebarMarkPosPlanSkew(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);		// ����ٸ�ũ(�簢����ö��, �纸��ö��)
	void MakeRebarMarkPosPlanBeryukDanAtSkew(vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bSttSlab, BOOL bUpper, double dScale);	// ����ٸ�ũ(���ö��)
	void MakeRebarMarkPosPlanBeryukDanAtNoSkew(vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bSttSlab, BOOL bUpper, double dScale);	// ����ٸ�ũ(���ö��)
	void MakeRebarMarkPosPlanBeryukGen(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bUpper, double dScale);	// ����ٸ�ũ(���ö��)
	void MakeRebarMarkPosPlanBeryukJijum(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, BOOL bUpper, double dScale);	// ����ٸ�ũ(���ö��)
	void MakeRebarMarkPosPlanBeryukSub(vector < pair<CString, CDPointArray > > &vMarkPos, CDPoint xySttRebar, CDPoint vRebar, CDPointArray xyArrDimMark, CString szMark, long nCycle, BOOL bUpper, double dScale);
	void MakeRebarMarkPosPlanCross(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, double dScale);		// ����ٸ�ũ(���κ�ö��)	
	void MakeRebarMarkPosPlanStringer(long nJigan,  vector < pair<CString, CDPointArray > > &vMarkPos, double dScale);	// ����ٸ�ũ(���κ�ö��)
	void MakeRebarMarkPosPlanStringerSub(vector < pair<CString, CDPointArray > > &vMarkPos, long nSangseST, long nStgAtTotal, double dScale);
	void MakeRebarMarkPosPlanOutBrk(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos, double dScale);
	void MakeRebarMarkPosPlanSangseDanbu(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);
	void MakeRebarMarkPosPlanSangseCant(long nJigan, vector < pair<CString, CDPointArray > > &vMarkPos);

	//////////////////////////////////////////////////////////////////////////
	// ö�������� ��ġ
	//////////////////////////////////////////////////////////////////////////
	void MakeCycleMainRebar(long nSection);	
	void MakeCycleMainRebarReCalc(long nSection);		
	void MakeCycleBeryukRebar(CDoubleArray arrMomentLeft, CDoubleArray arrMomentRigh);	
	void MakeCycleBeryukRebar_LC_RC();	
	void MakeCycleMainRebar4Cy(long nSection);		
	void MakeCycleMainRebar1Cy(long nSection);		

	//////////////////////////////////////////////////////////////////////////
	// ö�ٰ��� ����Լ�
	//////////////////////////////////////////////////////////////////////////
	CDPointArray CalcCTCMainRebarBase(long nGugansu, long nGugan, double dBaseLen);
	void CalcCTCMainRebar();
	void CalcCTCMainRebarBySlabHDanPos();	// �����߽�, �������߽� ��ö�� ���ݰ��
	void CalcCTCMainRebarByMaxRadius();		// ����ݰ��� ū�� ��ö�� ���ݰ��
	void CalcCTCMainRebarForDim();			// ��ö�� ġ�������
	void CalcCTCMainRebarAtSkewForDim();	// �簢�� ö�ٰ��ݰ��
	void CalcCTCMainRebarAtSinchuk(BOOL bStt, CDPointArray &xyDimLeft, CDPointArray &xyDimRigh);
	void CalcCTCSabogangRebar();			// �纸��ö�ٰ��ݰ��
	void CalcCTCMainRebarAtSkew();			// �纸���� ��ö�� ���ݰ��
	void CalcCTCSinchukRebar();				// ���������� �Ϻ� ��ö�� ���ݰ��
	void CalcCTCBogangRebarAtCrossBeam();	// ���κ� ����ö�� ���� ���
	void CalcCTCBogangRebarAtStringer();	// ���κ� ����ö�� ���� ���
	void CalcCTCBogangRebarAtOutBrk();		// �������κ� ����ö�� ���� ���
	void CalcCTCDanHunchRebar();			// �ܺ� ��ġ ������	
	
	void CalcCTCMainRebarBySlabCentCTCBySlabUpper();	// ��� �߾Ӻ� ��ö�ٰ��ݱ��� �¿��� ��ö�� ���� ���
	void CalcCTCMainRebarBySlabLeftRighCTCBySlabUpper(BOOL bLeftBase);	// �¿��� ��ö�� ���ݱ��� �߾Ӻ� ��ö�� ���ݰ��

	//////////////////////////////////////////////////////////////////////////
	// ��鵵 ��ö�� ����
	//////////////////////////////////////////////////////////////////////////
	void MakePlanMainRebar();
	void MakePlanMainRebarSkew();
	void MakePlanSabogang();
	void MakePlanUnderSinchuk();
	// ���ö�� �Ÿ�(�Ϲ�)	/ ���ö�� �Ÿ�(�߰�������)==>  nRebarPos=0 : ���, nRebarPos=1 : �Ϻ� nRebarPos=2 : ���������� �ϸ�
	void GetBeryukRebarDistOnCenter(CVectorArray *pArr, CPlateBasicIndex *pBxSttJijum, CPlateBasicIndex *pBxEndJijum, BOOL bUpper, long nDrawSide=DRAW_SIDEALL);
	void GetBeryukRebarDistOnJijum(CVectorArray *pArr, long nJijum, long nRebarPos, long nDrawSide, long nLRType=0);
	// ö�ٸ�ũ ��ġ�� ����(�Ϲ�����)
	CDPointArray GetMarkPosArr(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, double dRatio, BOOL bUpper);
	CDPointArray GetMarkPosArrSkewMain(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, BOOL bUpper, double dRatio = 0.5);
	// ö�ٸ�ũ ��ġ�� ����(�簢��ö��)
	CDPointArray GetMarkPosArrAtMainSkew(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, BOOL bUpper, BOOL bFromStt);
	CDPointArray GetMarkPosArrAtSangCant(CTwinVectorArray *tvArrPlane, double dLenToNearMidCTC, BOOL bUpper);
	CDPointArray GetMarkPosArrByLenFrStt(CTwinVectorArray *tvArrPlane, CDPointArray xyArrDim, double dLenFrStt, BOOL bUpper);

	// ��ö�� �迭 ���ϱ�
	vector<REBAR_PLAN>* GetMainRebarArr	  (BOOL bUpper, long nDrawSide);
	// �纸���� ��ö�� �迭 ���ϱ�
	vector<REBAR_PLAN>* GetMainRebarArrAtSkew(BOOL bSttSlab, BOOL bUpper, long nDrawSide);
	// �纸��ö�� �迭 ���ϱ�
	vector<REBAR_PLAN>* GetSabogangArr		  (BOOL bSttSlab, BOOL bUpper, long nDrawSide);
	// ���������� �Ʒ� ��ö�� ���ϱ�
	vector<REBAR_PLAN>* GetMainRebarArrUnderSinChuk(BOOL bSttSlab, long nDrawSide);
	// ���κ� ��ġ�ϴ� ���� ö�� �迭(V��)
	vector<REBAR_PLAN>* GetBoganRebarArrAtCrossBeam(CDoubleArray &dDistArr, CPlateBasicIndex *pBx);
	// �������κ� ��ġ�ϴ� ���� ö�� �迭(V��)
	void GetBoganRebarArrAtOutBrk(CDoubleArray &dDistArr, CPlateBasicIndex *pBx, BOOL bLeft);
	//////////////////////////////////////////////////////////////////////////
	// �������Լ�
	//////////////////////////////////////////////////////////////////////////
	// ���� ���� �ִ� ��������
	void EraseSameVector(CDPointArray& xyArr);				
	// ö���� CTC, ������ �̿��Ͽ� ���̸� ����.
	double  GetRebarLengthByPointArray(CDPointArray xyArr);		
	// ����Ʈ�迭��  �������ο� ���� ����������
	CDPointArray GetCDPointArrByPolyLine(CDPointArray xyArrOrigin, CDPointArray xyArrPoly, BOOL bVert);	
	// CDoubleArray��  �������ο� ���� ����������
	CDPointArray GetCDPointArrByDoubleLine(CDoubleArray dArrOrigin, CDPointArray xyArrPoly);
	// ö�ٻ����� ���� �� ������ ������.
	void SetLengthRebar(CRebar *pRB, long nJointType, double dCover = 0, double dRebarSpace = 0, long nBeta=2, long nIp=1);
	// ���ǥ ������ ö�� Sorting
	void ShellSortRebarList(CTypedPtrArray <CObArray, CRebar*> *pArr, int (CRebar::*fcmp)(const CRebar*) const) const;
	// ö���� �����߷� ���ϱ�
	double GetValueBasic(long nDia, long nType);
	// ���ؼ��� ö���� CTC�� �̿��Ͽ� ��ö���� ��ġ�� ���ϱ�	
	CDPointArray MakePointRebarArr(CDPointArray *parrBase, CDPointArray *parrDot, BOOL bMove, BOOL bUpper);
	// xyArrDim�� �̿��Ͽ� ö�ټ��� ����
	long GetRebarSu(CDPointArray xyArrDim);
	// xyArrDim�� �̿��Ͽ� ö�ټ��� ����(dRange : ���۹���)
	long GetRebarSuAtRange(CDPointArray xyArrDim, double dRange);
	// CTC�迭�� �������� �������� dOffsetStt��ŭ ������ ��ġ���� CTC�߰����� ���� ������ ��ġ�� ã��.
	double GetLengthByCTCAndRebarsu(CDPointArray xyArrDim);
	// xyArrDim�� �̿��Ͽ� ���̸� ����.
	double GetPointByOffsetAndCTC(CDPointArray xyArrDim, double dOffsetStt);
	// CTC�迭�� Cycle���� ����
	CDPointArray GetMarkPosArrByCycle(CDPointArray xyArrMark, long nCycle, long nCyclesu);
	// ö�� �߰�
	BOOL AddRebar(int nType);
	// ö�� ����
	BOOL DeleteRebar(CString strMark);
	// ö�� ã��
	int SearchRebar(CString strMark, CRebar* pRB);
	// ����Ʈ�迭�� ���ȿ� �ִ°͸�...
	void GetPointArrInCircle(CDPointArray& xyArr, CDPoint xyCen, double dRad);
	// ����, ���� ö�ٹ�ٰ����� �̿��Ͽ� �߾��� ö�ٹ�ٰ����� �����(�߽���ġ)
	CDPointArray GetDimArrCenByLeftRight(CDPointArray &xyArrDimLeft, CDPointArray &xyArrDimRigh);
	// �����Լ�
	double Bogan(double x1, double x2, double y1, double y2, double x);
	// ��ö�ٻ�� ������ �Ϻΰ������� ������ ���
	void CalcCTCChildByCTCParent(CDPointArray &xyArrCTCParent, CDPointArray &xyArrCTCChild, double dOffsetStt, double dOffsetEnd);
	// ��ü ��ũ���� ����Ŭ���� ��ũ ����
	CDPointArray GetDimArrByCycle(CDPointArray xyArrMark, long nCycle, long nCyclesu);
	//////////////////////////////////////////////////////////////////////////
	// ����Լ�
	//////////////////////////////////////////////////////////////////////////
	// �ܸ麰 ������ġ�� ���� �������� �ܸ��� (����/����� ����)
	CDPointArray GetShapeSlabJongBasePos(long nJongBasePos);
	CDPointArray GetShapeSlabOnCB(long nG);
	// �ܸ麰 Ⱦ����ġ�� ���� ���ö�� ����
	double GetRebarDiaBySection(long nSection, long nHDanPos, long nCycle, BOOL bUpper);
	// �ܸ麰 ������ġ�� ���� ���ö�� ����
	double GetRebarDiaByJongBasePos(long nJongPos, long nJongBasePos, BOOL bUpper);
	// Ⱦ����ġ���� ����Ŭ�� �����ϴ���?
	BOOL IsCycleValid(long nSection);
	// ������ġ���� ����Ŭ�� �����ϴ���?
	BOOL IsCycleValidJong(long nJongBasePos);
	// Ⱦ����ġ���� ö�����°� �����ϴ���?
	BOOL IsShapeValid(long nSection);
	// ������ġ���� ö�����°� �����ϴ���?
	BOOL IsShapeValidJong(long nJongBasePos);
	// ��ö���� ���۽����̼�(���������� �� �Ǻ��� �������)
	double GetSttStationMainRebar(long nPos, BOOL bUpper);
	// �ܺο� �Ϲݺΰ� ������ ����Ŭ�ΰ�?
	BOOL IsSameCycleDanGen();
	// ����, ����, �߾Ӻ� ��ö���� ������ ö���ΰ�?
	BOOL IsSameMainRebarLRC(BOOL bUpper, BOOL bDanbu);
	// �ܺ�, �Ϲݺΰ� ������ ö���ΰ�?
	BOOL IsSameMainRebarDanGen(BOOL bUpper);
	BOOL IsSameMainRebarDanGen();
	BOOL IsSameMainRebarShapesuDanGen();
	// ����������, ����, �߾Ӻ��� ��ö�� CTC �����Ѱ�?
	BOOL IsSameMainRebarCTC(BOOL bDanbu, BOOL bUpper);
	// ���ܱ�����ġ�� �������Ǳ���
	double GetLengthSlabTotal(long nJongBasePos);
	double GetLengthSlabTotalByDist(double dDist);
	// nJigan�� ������ ����
	double GetLengthSlab(long nJigan, long nJongBasePos, BOOL bConsiderCoverSd=FALSE);
	double GetLengthSlabByDist(long nJigan, double dDist, BOOL bConsiderCoverSd=FALSE);
	// nJigan������ ������ ����
	double GetLengthSlabAccuml(long nJiganTo, long nJongBasePos, BOOL bConsiderCoverSd=FALSE);
	double GetLengthSlabAccumlByDist(long nJiganTo, double dDist, BOOL bConsiderCoverSd=FALSE);
	// ��ũ��ġ�� ����
	CDPointArray GetArrMarkPosPlanByName(long nJigan, CString szName);
	// ��,���� �� �������������ġ�� �߾� �����̼�
	CDoubleArray GetStationArrForMainRebarGugan(long nPosType=0);
	// �ܺ�ö���� ������ ����.
	long GetRebarSuAtDanbu(BOOL bUpper, BOOL bStt);
	// ���� CTC�� �̿��Ͽ� ���ϴ� ��ġ�� CTC�� ���� (���ռ������� �����)
	CDPointArray GetDimArrAtMultiCurve(double dStaStt, CDPointArray xyArrDimSource, double dDistSource, long nCalcPos, BOOL bPositiveDir=TRUE);
	// �ش�Section���� ��ö�� �����ܸ� ���̵������ ����
	CDPointArray GetXyArrGuideLineForMainRebar(long nSection, BOOL bUpper);
	// JongBasePos���� ���ö�� �����ܸ� ���̵������ ����
	CDPointArray GetXyArrGuideLineForBeryukRebar(long nJongBasePos, BOOL bUpper);
	CDPointArray GetXyArrGuideLineForBeryukRebarOnCB(long nG, BOOL bUpper);
	// �������κ��� �ֳ�?
	BOOL IsBridgeHasOutBracket();
	BOOL IsBridgeHasOutBracket(long nSection, BOOL bLeft);
	// �������� ��ġ�� �ֳ�?
	BOOL IsBridgeHasSlabHunch();
	// ���?
	BOOL IsBridgeHasCurveLine();
	// ���κ� �ֳ�?
	BOOL IsBridgeHasCrossBeam();
	BOOL IsBridgeHasCrossBeamHunch();
	// ���κ� �ֳ�? 
	BOOL IsBridgeHasStringer();
	// �米�ΰ� ? 
	BOOL IsSkew();
	// ������ ������ �米?
	BOOL IsSkewStt();
	// ������ ������ �米?
	BOOL IsSkewEnd();
	// ��ö�ٿ��� �Ǻ������� �Ÿ�
	double GetLengthMainRebarToCover(BOOL bStt, long nJongBasePos, BOOL bUpper, double &dLengthMainRebarAtSkew, BOOL bNotBlock = FALSE);
	// ��ö���� ���۵Ǵ� �����̼�
	double GetStationMainRebarPos(BOOL bSttMainRebar, long nPosType=0) const;
	// �������� ������ ��ö�� ����, ���� ������ �̰ݰŸ�(����)
	double GetSideBarDistFromCenterGir(long nSection, BOOL bLeft);
	// ������ ���� �׸��� ������ �ִ��� �˻�
	BOOL IsDrawMainRebarArea(CDPoint pt1, CDPoint pt2);
	// ����Ʈ�� ���� �׸��� ������ �ִ��� �˻�
	BOOL IsDrawArea(CDPoint pt1);
	// ������ ö�ٸ�ũ ��ġ�� ����
	CDPointArray ModifyMarkPosArrByJigan(long nJigan, CDPointArray xyArrMark, CTwinVectorArray *tvArrRebar);
	// Trim�Ǿ �߰��� ����ִ� xyArrMarkPos�� xyArrMarkPosStt�� xyArrMarkPosEnd�� ������. 
	void DivideMarkPosAtJigan(CDPointArray* xyArrMarkPos, CDPointArray* xyArrMarkPosStt, CDPointArray* xyArrMarkPosEnd, long nJigan);
	// �������� ����
	double GetAreaSlab();
	// ������ġ���� ���������� ��ٿɼǿ� ���� ���������� ��,���� ���̸� ���.
	void GetArrJointDisAtJijum(long nPier, CDPoint vAngStt, CDPoint vAngEnd, double dDisStt, double dDisEnd, double *dArrJoint);
	// ��ũ��ġ���� ������ �����γ� �����α����ǰŸ��� ����Ͽ� ������
	double GetLengthExtendToSlabSttEndFrPoint(CDPointArray xyArrMark, double dExtPlus, BOOL bFromSttMark, BOOL bSttSlabLine);
	// ��ũ��ġ���� ������ �߽�Ʈ������ �����γ� �����α����� �Ÿ��� ����Ͽ� ������
	double GetLengthExtendToSlabCenFrPoint(CDPointArray xyArrMark, long nJigan, double dExtPlus, BOOL bFromSttMark, BOOL bSttSlabCenLine);
	// ��ũ��ġ���� ������ ��������, �������ܱ����ǰŸ��� ����Ͽ� ������
	double GetLengthExtendToSlabLeftRighFrPoint(CDPoint xyPoint, double dExtPlus, BOOL bLeftSlab);
	// ������ Ⱦ���� ������ ������ ��ö���� ���⺤��
	CDPoint GetSlopeAtHDanByPos(CDPointArray xyArrHDan, CDPoint xyPos);
	// ������ Ⱦ�ܿ��� ��ġ���� ������ ���̸� �ڵ����� ������
	double GetJointLengthAtHDan(long nSection, BOOL bUpper, long nHDanPos);
	// ������ ���ܿ��� ��ġ���� ������ ���̸� �ڵ����� ������
	double GetJointLengthAtJDan(long nJongPos, BOOL bUpper, long nJongBasePos);
	// xyArrPoly�� ���� ���鼭 ������ �߰���ġ���� ����
	CDPoint GetMidPointAtPolyLine(CDPointArray xyArrPoly);
	// ö�������� ������ ���Ѵ�.
	double GetRebarWeightTotal(BOOL bMainRebar, BOOL bGuardRebar);
	// ���ܸ鿡���� �ش� ������ ������, ������ �����Ѵ�. 
	BOOL GetPosJiganAtJong(int nJigan, double& dStt, double& dEnd, long nJongBasePos);
	// ö�ټ��� ����Ŭ���� ������.	
	double GetRebarsuAtCycle(long nRebarsu, long nCycle, long nTotalCycle);
	// ���� ö���� �ִ� ��»���Ŭ ���� ����.
	long GetsuRealCycle(long nJongBasePos, BOOL bUpper);
	// �ܰ氣 = �Ϲݺ�/�ܺ�, �ٰ氣 = �Ϲݺ�/������ ������ϱ�.
	long GetBesu(long nJongBasePos, BOOL bUpper);
	// ��鵵���� tVector�� �߽���ġ�� Ⱦ�ܹ��⿡�� ��ġ�ϴ� ������ �����.
	double GetRatioOfMidPosRebarAtHDan(CTwinVector& tVector, double dSta, BOOL bUpper, double &dRatioPre);
	// ���ö���� CTC�� ����.
	CDPointArray *GetCTCBeryukRebar(long nSection, BOOL bUpper);	
	// Ⱦ����ġ���� ���ö���� CTC ������
	void SetCTCBeryukRebar(CDPointArray& xyArrDim, long nSection, BOOL bUpper);
	// ������ ��ü ��ö�� �� �纸�� ö�� ��ġ������ ���
	CDoubleArray GetRebarPosAtJongBasePos(BOOL bUpper, long nJongBasePos, BOOL bRealPos, long nRangeRebar=0);
	// ���ܸ� ������ġ���� ö�ٹ�� ġ��
	CDPointArray GetDimTotalAtJongBasePos(BOOL bUpper, long nJongBasePos, BOOL bRealDim);
	// ������������ ��(���ܱ�����ġ���� ���� ����)
	double GetWidthBlockOut(BOOL bSttSlab, long nJongBasePos);
	// ������ ��
	double GetWidthUgan(BOOL bSttSlab, long nJongBasePos);
	// �����Ǻ��β�
	double GetCoverSdToModify(BOOL bSttSlab, BOOL bLeftCalcPos);
	// ���������ο��� �����Ǻ��β�
	double GetCoverSdToModifyAtExp(BOOL bSttSlab, BOOL bLeftCalcPos, CDPoint xyDimStt);
	// ĵƿ������ �������� ö���� ����
	long GetsuBeryukAtCantilever(long nPos, BOOL bDanbu, BOOL bUpper);
	// Ⱦ����ġ�� ĵƿ������ö�� Ÿ��
	long GetTypeBeryukAtCantilever(BOOL bDanbu, BOOL bUpper);
	// ������ġ���� ���ö���� ��ġ�Ǵ� ����
	double GetLengthRebarRangeAtJongPos(long nJongBasePos, BOOL bUpper);
	/// Ⱦ����ġ���� �����̼�
	double GetStationBySection(long nSection);
	// Ⱦ����ġ���� ö�ٻ���Ŭ�� ����
	void RemoveRebarCycleBySection(long nSection);
	// Ⱦ����ġ�� ö�ٻ���Ŭ�߰�
	void AddRebarCycleBySection(long nSection, REBAR_CYCLE stRebarCycle);
	// Ⱦ����ġ���� ��������
	long GetSizeHDanJointBySection(long nSection, long nCycle, BOOL bUpper);
	// Ⱦ����ġ���� ����Ŭ��������
	REBAR_CYCLE* GetRebarCycleBySection(long nSection, long nCycle);
	// Ⱦ����ġ���� ����Ŭ��������
	vector<REBAR_CYCLE>* GetRebarCycleBySection(long nSection);
	// Ⱦ����ġ�� ����Ŭ��
	long GetRebarCycleSuBySection(long nSection);
	// Ⱦ����ġ�� ö����������
	void RemoveRebarShapeBySection(long nSection);
	// ������ġ�� ö����������
	void RemoveRebarShapeByJongBasePos(long nJongBasePos);
	// Ⱦ����ġ�� ö�������߰�
	void AddRebarShapeArrAtSection(long nSection, vector<REBAR_SHAPE> vRebarShape);
	// ������ġ�� ö�������߰�
	void AddRebarShapeArrAtJongBasePos(long nJongBasePos, vector<REBAR_SHAPE> vRebarShape);
	// ������ġ���� ö�ٻ���Ŭ�� ����
	void RemoveRebarCycleByJongBasePos(long nJongBasePos);
	// ������ġ�� ö�ٻ���Ŭ�߰�
	void AddRebarCycleByJongBasePos(long nJongBasePos, REBAR_CYCLE stRebarCycle);
	// ������ġ�� ö�ٻ���Ŭ�߰�
	void AddRebarCycleByJongBasePos(long nJongBasePos, CDoubleArray& arrUpLeftLong, CDoubleArray& arrDnLeftLong
									, CDoubleArray& dArrDiaUpLeft, CDoubleArray& dArrDiaDnLeft);
	// ������ġ���� ����Ŭ��������
	REBAR_CYCLE* GetRebarCycleByJongBasePos(long nJongBasePos, long nCycle);
	// ������ġ���� ����Ŭ��������
	vector<REBAR_CYCLE>* GetRebarCycleByJongBasePos(long nJongBasePos);
	// ������ġ�� ����Ŭ��
	long GetRebarCycleSuByJongBasePos(long nJongBasePos);
	// Ⱦ����ġ���� ����Ŭ�� ö������
	vector<REBAR_SHAPE>* GetRebarShapeArrBySection(long nSection, long nCycle);
	// ������ġ���� ����Ŭ�� ö������
	vector<REBAR_SHAPE>* GetRebarShapeArrByJongBasePos(long nJongBasePos, long nCycle);
	// Ⱦ����ġ���� ����Ŭ�� ö�����¸� ���Ϻ� �����ؼ� ���� ����
	vector<REBAR_SHAPE> GetNewRebarShapeArrBySection(long nSection, long nCycle, BOOL bUpper);
	// ������ġ���� ����Ŭ�� ö�����¸� ���Ϻ� �����ؼ� ���� ����
	vector<REBAR_SHAPE> GetNewRebarShapeArrByJongBasePos(long nJongBasePos, long nCycle, BOOL bUpper);
	// ������ġ���� ����Ŭ�� ö�����¸� ������
	CDPointArray GetRebarShapeByJongBasePos(long nJongBasePos, long nCycle, CString szName);	
	// ��ö�� �������
	double GetLengthMainDolchul(long nSection, BOOL bUpper, BOOL bLeft);
	// ���������� ����� ���� ��ö���� �⺻����
	double GetCTCMainForJoint(BOOL bUpper, long nHDanPos, long nSection);
	// ���������� ����� ���� ���ö���� �⺻����
	double GetCTCBeryukForJoint(BOOL bUpper, long nHDanPos, long nJongBasePos);
	// vCycleSource�� vCycleTarget�� dRatio��ŭ �������̸� ��ȭ���� ����.
	void SetCycleToCycle(vector<REBAR_CYCLE> &vCycleSource, vector<REBAR_CYCLE> &vCycleTarget, double dRatio=1); 
	// arrCTCSource�� arrCTCTarget�� dRatio��ŭ CTC�� ��ȭ���� ����.
	void SetCTCArrToCTCArr(CDPointArray &arrCTCSource, CDPointArray &arrCTCTarget, double dRatio=1); 
	// REBAR_PLAN �迭�߿��� nCycle�̰� nShape��  CTwinVector�迭�� ���� 
	void GetTwinVectorArrByCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycle, long nShape);
	void GetTwinVectorArrAtRebarPlan(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn);
	void GetTwinVectorArrByCycleAndName(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycle, CString sName);
	void GetTwinVectorArrByMultiCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycleStt, long nCycleEnd, long nShape);
	void GetTwinVectorArrByMultiCycleAndShape(vector <REBAR_PLAN> *vPlan, CTwinVectorArray *tvArrReturn, long nCycleStt, long nCycleEnd, CString sName);
	// �ִ� CTC�� ����
	double GetMaxCTC(CDPointArray &xyArrDim);
	// xyArrPoint�߿��� ¦���̰� nCount�� ��ŭ ����(��ũƽ��ġ������ �����)
	CDPointArray ModifyPointByCount(CDPointArray &xyArrPoint, long nCount);
	// Ȯ���������� ���ö�� CTC�� ������ �Ǵ� ��������ġ
	void GetXyBasePointPlanEx(CDPoint Base[9], CDPoint Real[4], BOOL bIncludeCover, BOOL bUpper, BOOL bIncludeSttEndCover=FALSE);
	// ���ܱ�����ġ���� ���ܹ������� �����Ǻ��� ��������(Ȯ�������)
	double GetLengthCoverSd(BOOL bSttSlab, long nJongBasePos=JONGBASEPOS_SLABCENTER);	
	// �߾Ӻ� ���ö�� ���ݻ����� �����(Ȯ��������)
	void ModifyDimArr(CDPointArray &arrDim);
	// �ܺ� �Ϻ� ���ö��2�� ��ġ�ϳ�?
	BOOL IsInstallBeryukRebarAtDanHunch(long nJongBasePos, BOOL bSttSlab, long nCycle);
	// �ܸ麰 �ش� Bx
	CPlateBasicIndex* GetBxBySection(long nSection, long nG=-1);		
	// �������ǽ��ۺ� ~ ������ ���ۺ�	
	double GetLengthSlabToTaper(BOOL bSttSlab, long nJongBasePos);
	double GetLengthSlabToTaperByDist(BOOL bSttSlab, double dDist);
	// �Ŵ����� ��������
	double GetLengthJiganByGirder(long nG, long nJigan);
	// ö���� CTC�� �̿��Ͽ� ö���� �������ϱ�
	long GetSuRebarArray(CDPointArray *xyArr, BOOL bIncludeCover);
	// ������ ��鿡�� ���κ��� ���̸� ����
//	double GetStgLengthAtSlab(CStringer *pStg);
	// ���κ� ö�� ���� ���ϱ�
	double GetLengthCrossBeamHunchRebar(CPlateBasicIndex *pBx, BOOL bUpper);
	// ���κ� ö�ٱ��� ���ϱ�
	double GetLengthStringerHunchRebar(CPlateBasicIndex *pBx, long nStg, BOOL bUpper) const;
	// �������κ� ö�ٱ��� ���ϱ�
	double GetLengthOutBrkHunchRebar(CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft);
	// CTC�迭�� �������� �������� dOffsetStt��ŭ ������ ��ġ���� CTC�߰����� ���� ������ ��ġ�� ã��.
	double GetNearstMidPointAtCTCArr(CDPointArray xyArrDim, double dOffsetStt);
	// ��鵵���� ö�ٸ�ũ�� ��ġ�ϴ� ������ �����.
	double GetRatioOfPlanMark(CDPointArray xyArr, BOOL bUpper, double &dRatioPre);
	//Stud�� ���� ó�� ��ġ ����(�Է¿�)
	CDPointArray GetPosFirstStud();							
	// PointArray�� ������ ��ȯ�Ѵ�.
	vector<double> GetLenArrFromLineArr(CDPointArray ptArr);
	// ��Ʈ���Ű� �����ۺο� �������� �˻�
//	void IsStringerMatchAtTaper(CStringer *pStg, BOOL &bMatchStt, BOOL &bMatchEnd, CDPoint &xyMatchStt, CDPoint &xyMatchEnd);
	// m_arrStringer���� �ش� Bx�� ���κ� ��ġ�� ����.
//	long GetStgNoByBx(CPlateBasicIndex *pBx, long nStg);
	// nStgAtTotal�� �̿��Ͽ� ���κ��� ����.
//	CStringer* GetStgByStgNo(long nStgAtTotal);
	// ������ ��/���� ö�� ������ �������� ��/���� ö�ٰ����� ����Ѵ�.
	double GetCTCBySlabLeftRighRatio(BOOL bCalcLeft, CDPointArray &xyArrSourceCTC, double dDataNew, long nGugan, BOOL bIncludeSinchuk);
	// SECTION_CEN ���� ��ö�� Shape�� ��
	long GetsuMainRebarShapeAtCen();
	CDPointArray GetXyArrSlabEnd(long nG);
	CDPointArray GetXyArrSlabHunch(long nG, CPlateBasicIndex *pBxCB);
	double GetLengthSlabEndToCBCen(long nG, CPlateBasicIndex *pBxCB);
	double GetDistByJongBase(long nJongBasePos);
	CDPointArray GetXyArrPoint(CString sRebarName, vector<REBAR_SHAPE>& vArrShape);
	long GetNoShapeAtHDanShapeArr(CString sRebarName, vector<REBAR_SHAPE>& vArrShape);
	long GetRebarsuAtPlanArr(vector<REBAR_PLAN> *pvPlan, long nCycle, long nShape, long nPos);
	BOOL IsInstallBeryukRebarAtCenter(long nCycle);
	BOOL IsPehapCycle(long nJongBasePos, long nCycle);
	BOOL IsPointInJigan(CDPoint xyCen, long nJigan);
	BOOL IsPointInCircle(CDPoint xyPos, CDPoint xyCen, double dRad);
	long GetXyPosIDWhereInSlabHDan(CDPoint xyPos);
	
	vector<CDoubleArray> GetJiganLenArr();
	CDPointArray GetDanbuJointPos();
	// �ܺ� ��ġ ������ 3~4�� ���� ���� ��ö�� ��ġ
	CDPointArray GetXyArrRebarJongPosAtDanHunch(BOOL bSttSlab, long nJongBasePos);
	CDPointArray GetXyArrJointPosAtJongBasePos(long nJongBasePos, long nCycle, BOOL bUpper);
	CDPoint GetXyPosByTwinVectorAndDist(CTwinVector tVector, double dDist);
	double GetDistByXyPos(CDPoint xyPos);
	double GetLengthBySlabEndRadius(double dStaNear, long nJongBasePosBase, long nJongBasePosTarget, double dBaseLen);
	CDPoint GetCTCRebarAtCB(CPlateBasicIndex *pBx, long &nCount);
};

#endif // !defined(AFX_ADECKDATA_H__D5E29F58_6D9A_484D_99E9_80429B01650A__INCLUDED_)
