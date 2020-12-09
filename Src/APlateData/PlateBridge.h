// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Bridge.h: interface for the CPlateBridge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATEBRIDGE_H__5F50DD8D_BE7B_4FDB_A781_1D4BA535B029__INCLUDED_)
#define AFX_PLATEBRIDGE_H__5F50DD8D_BE7B_4FDB_A781_1D4BA535B029__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		G_F_U				0 	//�Ϲ� 	����
#define		G_F_L				1	//�Ϲ� 	����
#define		G_W					2	//�Ϲ� 	������
#define		G_H_L				3	//�Ϲ� 	���򺸰���
#define		G_H_R				4	//�Ϲ� 	���򺸰���

#define		V_ST				5	// ����������
#define		V_J_ST				6	// ������ ����������(����������)

#define		VB_UB				7	// VBracing ������
#define		VB_LB				8	// VBracing ������
#define		VB_CR				9	// VBracing ����
#define		VB_GU				10	// VBracing ����Ʈ
#define		VB_BOLT				11	// VBracing ��Ʈ

#define		HB_B				12	// HBracing ��
#define		HB_GU				13	// HBracing �� ����Ʈ
#define		HB_BOLT				14	// HBracing ��Ʈ
 
#define		SP_U_U				15	//��� ��������	�ܺ� �����
#define		SP_U_L				16	//��� ��������	���� ������
#define		SP_U_FILL			17	//��� �������� �ʷ��÷���Ʈ
#define		SP_U_BOLT			18	//��� ��������	��Ʈ&��Ʈ
#define		SP_L_L				19	//�Ϻ� ��������	���� ������
#define		SP_L_U				20	//�Ϻ� ��������	���� ������
#define		SP_L_FILL			21	//�Ϻ� �������� �ʷ��÷���Ʈ
#define		SP_L_BOLT			22	//�Ϻ� ��������	��Ʈ&��Ʈ
#define		SP_W_MO				23	//������ �������� ���Ʈ ������(����)
#define		SP_W_SH				24	//������ �������� ���� ������(����)
#define		SP_W_FILL			25	//������ �������� �ʷ��÷���Ʈ
#define		SP_W_BOLT			26	//������ �������� ��Ʈ&��Ʈ

#define		CP_W				27	//���κ� ������	������
#define		CP_F_U				28	//���κ� ������	����
#define		CP_F_L				29	//���κ� ������	����
#define		CP_STEEL			30	//���κ� ������ ����
#define		CP_BR_W				31	//���κ� ������	����� ������
#define		CP_BR_F_U			32	//���κ� ������	����� ����
#define		CP_BR_F_L			33	//���κ� ������	����� ����
#define		CP_BR_GA			34	//���κ� ������	����� ������
#define		CP_BR_GA_FA			35	//���κ� ������	����� ������
#define		CP_V				36	//���κ� ������	����������
#define		CP_H				37	//���κ� ������	���򺸰���
#define		CP_SP_U_U			38	//���κ� ������	��� ������(���)
#define		CP_SP_U_L			39	//���κ� ������	��� ������(�ϸ�)
#define		CP_SP_L_U			40	//���κ� ������	�Ϻ� ������(���)
#define		CP_SP_L_L			41	//���κ� ������	�Ϻ� ������(�ϸ�)
#define		CP_SP_W_MO			42	//���κ� ������	������ ������ ���Ʈ
#define		CP_SP_W_SH			43	//���κ� ������	������ ������ ����
#define		CP_SP_W_FH			44	//Filler �β��� 9mm�� ������� ������ �߰�...
#define		CP_BOLT				45	//���κ� ������	��Ʈ&��Ʈ

#define		CG_W				46	//���κ� �Ϲݺ�	������
#define		CG_F_U				47	//���κ� �Ϲݺ�	����
#define		CG_F_L				48	//���κ� �Ϲݺ�	����
#define		CG_STEEL			49	//���κ� �Ϲݺ� ����
#define		CG_BR_W				50	//���κ� �Ϲݺ�	����� ������
#define		CG_BR_F_U			51	//���κ� �Ϲݺ�	����� ����
#define		CG_BR_F_L			52	//���κ� �Ϲݺ�	����� ����
#define		CG_BR_GA			53	//���κ� �Ϲݺ�	����� ������
#define		CG_BR_GA_FA			54	//���κ� �Ϲݺ�	����� ������
#define		CG_V				55	//���κ� �Ϲݺ�	����������
#define		CG_H				56	//���κ� �Ϲݺ�	���򺸰���
#define		CG_SP_U_U			57	//���κ� �Ϲݺ�	��� ������(���)
#define		CG_SP_U_L			58	//���κ� �Ϲݺ�	��� ������(�ϸ�)
#define		CG_SP_L_U			59	//���κ� �Ϲݺ�	�Ϻ� ������(���)
#define		CG_SP_L_L			60	//���κ� �Ϲݺ�	�Ϻ� ������(�ϸ�)
#define		CG_SP_W_MO			61	//���κ� �Ϲݺ�	������ ������	���Ʈ
#define		CG_SP_W_SH			62	//���κ� �Ϲݺ�	������ ������ ����
#define		CG_SP_W_FH			63	//Filler �β��� 9mm�� ������� ������ �߰�...
#define		CG_BOLT				64	//���κ� �Ϲݺ�	������ ��Ʈ&��Ʈ

#define		ST_W				65	//���κ�	������
#define		ST_F_U				66	//���κ�	����
#define		ST_F_L				67	//���κ�	����
#define		ST_BR_W				68	//���κ�	����� ������
#define		ST_BR_F_U			69	//���κ�	����� ����
#define		ST_BR_F_L			70	//���κ�	����� ����
#define		ST_BR_GA			71	//���κ�	����� ������
#define		ST_BR_GA_FA			72	//���κ�	����� ������
#define		ST_V				73	//���κ�	����������
#define		ST_H				74	//���κ�	���򺸰���
#define		ST_SP_U_U			75	//���κ�	��� ������(���)
#define		ST_SP_U_L			76	//���κ�	��� ������(�ϸ�)
#define		ST_SP_L_U			77	//���κ�	�Ϻ� ������(���)
#define		ST_SP_L_L			78	//���κ�	�Ϻ� ������(�ϸ�)
#define		ST_SP_W				79  //���κ�	������ ������
#define		ST_BOLT				80	//���κ�	��Ʈ&��Ʈ

#define		WI_P_W				81	//�������κ� ������	������
#define		WI_P_F_U			82	//�������κ� ������	����
#define		WI_P_F_L			83	//�������κ� ������	����
#define		WI_P_BR_W			84	//�������κ� ������	����� ������
#define		WI_P_BR_F_U			85  //�������κ� ������	����� ����
#define		WI_P_BR_F_L			86	//�������κ� ������	����� ����
#define		WI_P_BR_GA			87	//�������κ� ������	����� ������
#define		WI_P_BR_GA_FA		88	//�������κ� ������	����� ������
#define		WI_P_V				89	//�������κ� ������	����������
#define		WI_P_H				90	//�������κ� ������	���򺸰���
#define		WI_P_SP_U_U			91	//�������κ� ������	��� ������(���)
#define		WI_P_SP_U_L			92	//�������κ� ������	��� ������(�ϸ�)
#define		WI_P_SP_L_U			93	//�������κ� ������	�Ϻ� ������(���)
#define		WI_P_SP_L_L			94	//�������κ� ������	�Ϻ� ������(�ϸ�)
#define		WI_P_SP_W			95	//�������κ� ������	������ ������
#define		WI_P_BOLT			96	//�������κ� ������	��Ʈ&��Ʈ

#define		WI_G_W				97	//�������κ� �Ϲݺ�	������
#define		WI_G_F_U			98	//�������κ� �Ϲݺ�	����
#define		WI_G_F_L			99	//�������κ� �Ϲݺ�	����
#define		WI_G_BR_W			100	//�������κ� �Ϲݺ�	����� ������
#define		WI_G_BR_F_U			101	//�������κ� �Ϲݺ�	����� ����
#define		WI_G_BR_F_L			102	//�������κ� �Ϲݺ�	����� ����
#define		WI_G_BR_GA			103	//�������κ� �Ϲݺ�	����� ������
#define		WI_G_BR_GA_FA		104 //�������κ� �Ϲݺ�	����� ������
#define		WI_G_V				105	//�������κ� �Ϲݺ�	����������
#define		WI_G_H				106 //�������κ� �Ϲݺ�	���򺸰���
#define		WI_G_SP_U_U			107 //�������κ� �Ϲݺ�	��� ������(���)
#define		WI_G_SP_U_L			108	//�������κ� �Ϲݺ�	��� ������(�ϸ�)
#define		WI_G_SP_L_U			109	//�������κ� �Ϲݺ�	�Ϻ� ������(���)
#define		WI_G_SP_L_L			110	//�������κ� �Ϲݺ�	�Ϻ� ������(�ϸ�)
#define		WI_G_SP_W			111	//�������κ� �Ϲݺ�	������ ������
#define		WI_G_BOLT			112	//�������κ� �Ϲݺ�	��Ʈ&��Ʈ

#define		EN_W				113	//������ 	������
#define		EN_F_U				114	//������ 	����
#define		EN_F_L				115	//������ 	����
#define		EN_BR_W				116	//������ 	����� ������
#define		EN_BR_F_U			117	//������ 	����� ����
#define		EN_BR_F_L			118	//������ 	����� ����
#define		EN_V				119 //������ 	����������
#define		EN_H				120	//������ 	���򺸰���
#define		EN_SP_U_U			121	//������ 	��� ������(���)
#define		EN_SP_U_L			122	//������ 	��� ������(�ϸ�)
#define		EN_SP_L_U			123	//������ 	�Ϻ� ������(���)
#define		EN_SP_L_L			124	//������ 	�Ϻ� ������(�ϸ�)
#define		EN_SP_W				125	//������ 	������ ������
#define		EN_BOLT				126	//������ 	��Ʈ&��Ʈ

#define		GI_SB				127	//��Ÿ����	���ܿ�����
#define		GI_SQ				128	//��Ÿ����	���ܿ����� ����
#define		GI_SA				129	//��Ÿ����	SLAB ANCHOR
#define		GI_SO				130	//��Ÿ����	���÷���Ʈ 
#define		GI_FA				131	//��Ÿ����	FACE PLATE
#define		GI_LP				132	//��Ÿ����	���
#define		GI_LP_R				133	//��� ��
#define		GI_LP_PL			134	//��� ������
#define		GI_JA_L				135	//��Ÿ����	��������� ����
#define		GI_JA_R				136	//��Ÿ����	��������� ����
#define		GI_JA_SA			137	//��Ÿ����	��� ���÷���Ʈ
#define		GI_ZST				138	//��Ÿ����	��������å
#define		GI_HG				139	//��Ÿ����	���������� ���ٸ�

#define		TOTAL_PIECE_VALUE	140	//�� ����

// ======================= ������ ���� �κ� ���� =============================
#define		WELD_FIELDU			200
#define		WELD_FIELDL			201
#define		WELD_FIELDWEB		202
#define		TOTAL_WELD_VALUE	3
// ===================== ��鵵��� ������ ��������&�ʷ� =====================
#define		SP_W_PLAN			1000	//������ �������� ���
#define		SP_W_PLAN_F			1001	//������ �������� ����ʷ�

const long 	KYUNGGANSUMAX		= 40;					// �ִ� ������
const long	JIJUMSUMAX			= KYUNGGANSUMAX+1;		// �ִ� ������
const long  MAXSTRING			= 5;					// �ִ� ��Ʈ���ż�

//////////////////////////////////////////////////////////////////////////
// static const
static const long STT_POS = 0;
static const long END_POS = 1;
static const long CENTERGIRDER = -1;

const long EQUAL_VALUE_PLATE = 6;

//////////////////////////////////////////////////////////////////////////
// class CPlateBridge
#define  GIRDERSUMAX    20
#define  JACKUPSUMAX    6
#define  HDANLINESU		50
#define  MAX_TENSION	40

enum { SP_WEB_FL, SP_WEB_MO, SP_WEB_FLMO };
enum { PLATE_GIR, SOSU_GIR, TU_GIR };
enum { LEFT, RIGHT, COUNT };
enum { UP, DOWN };
enum { FRM_UPPER=0, FRM_LOWER=1, FRM_WEB=2};

struct _TENSION {
	double m_dL;
	double m_dH;
	double m_dR;
	long m_nType;
};

struct _BINDCONC {
	double m_dWidth;	// ��
	double m_dHeight;	// ����
	double m_dDeep;		// ���Ա���
	double m_dDis;		// �̰ݰŸ�
};

class CLineInfo;
class CPlateGirderApp;
class CCentSeparation;
class CPierInfo;

//typedef CTypedPtrArray <CObArray, CPlateGirderApp*> CPlateGirderArray;

//�Ŵ�, ��ȣ��, �Ǿ� ---- ����
class AFX_EXT_CLASS  CPlateBridge : public CObject 
{
// Constructor
public:
	CPlateBridge(CLineInfo* pLine = NULL);
	virtual ~CPlateBridge();

// Attributes
protected:	
	CPlateGirderApp	*m_pCRGirder;				// ���� �߽ɰŴ�
	CTypedPtrArray <CObArray, CPlateGirderApp*>   m_pGirder;				// �Ŵ� �迭 ��ü
	CTypedPtrArray <CObArray, CCentSeparation*>   m_pGuardWall;//��ȣ�� 

	long	m_nQtyHDanNode						;	// ������ ����
	double	m_dLengthHDan[HDANLINESU]			;	// ���� ����(����)		
	long	m_nValueTypeHDan[HDANLINESU][3]		;	// ����

public:
	long	GetGuardWallSu() const { return m_pGuardWall.GetSize(); }
	long	GetGirdersu()    const { return m_pGirder.GetSize();}
	long	GetNumHaiseokCombo() 
	{
		//������ �ռ�������(���ռ� ��������) 
		m_NumHaiseokCombo = 0; 
		return m_NumHaiseokCombo;
	}
	
	CCentSeparation *GetGuardWall(long nGuard) const;
	CCentSeparation *GetCurGuardWall(long nPlace) const;	// ��, ��, �߾Ӻ� ��ȣ��
	BOOL	IsTypeGuardFenceDom(long nHDan) const;
	long	GetGuardFenceType(long nHDan) const; 
	long	GetValueTypeHDan(long nHDan, long nTypeHDan) const; 
	long	GetNumGuardFenceFrLeft(long nHDan) const;

	CPlateGirderApp* GetGirder(long nG)		   const
	{
		if(nG < 0 || nG >= m_pGirder.GetSize()) return m_pCRGirder;	
		return m_pGirder[nG];
	}

	void SetNumHaiseokCombo(long nNumber) {m_NumHaiseokCombo = nNumber; }
public:
	CLineInfo *m_pLine;
	CPierInfo *m_PierInfo;

	CString	m_sNameBridge					;	//[�⺻ �Է�]
	CString	m_sNameOwner					;	// ����ó
	CString	m_sNameConstructor				;	// �ð���		
	CString	m_sNameAssembler				;	// ���ۻ�
	CString	m_sNameSite						;	// �����
	CString	m_sNameEngineering				;	// �����
	BOOL    m_bUseLine						;   // �����������
	double  m_dBridgeWidth					;	// ����
	double  m_dDisLineToBridgeCen			;	// �������� �������߽ɱ����� �Ÿ�
	double  m_dGirderSpace					;	// �Ŵ�����
	long    m_nLineType                     ;   // ����Ÿ��(�, ����)

	CString	m_sDeungeub						;	//������
	long	m_nTypeAbutOrPier[2]			;	//�������� ���� or����		  
	double	m_dStationBridgeStt				;	//�������� �����̼�
	double	m_dLengthTotalSlab				;	//������ ����
	long	m_nQtyJigan						;	//������
	double	m_dWidthSlabLeft				;	//����ĵƿ������
	double	m_dWidthSlabRight				;	//����ĵƿ������
	CDoubleArray m_dHunchArray				;	//������ ��ġ ����	
	CDPointArray m_arrTendon		        ;	// x: Tendon ��ġ �����̼� (�����߽�), y: ������ ��ġ 0=����, 1=���� 2=�������
	CLongArray m_arrTendonDom				;	//���鿡���� ���� ��ġ ���� ����
	long    m_nTendonFix					;   // 0 : ����������, 1 : ���������
	long	m_nTendonDomFix					;	// 0 : ����������, 1 : ���������, 2 : �������

	long	m_nSlabTensionSize				;	// ��ġ���� ����
	struct _TENSION m_SlabTension[MAX_TENSION];	// ��ġ���� �ִ� 40�� (20�Ŵ� ���� 39�� Ȯ��)
	double	m_dHeightLeftTension			;	//������ġ ��������
	double	m_dHeightRightTension			;	//������ġ ��������
	double  m_dLengthGinjangdan				;	// ����� ����
	double  m_dLengthJungchakdan			;	// ������ ����
	double  m_dLengthSecMove				;	// ������ �ܺ� �̰ݰŸ�..070208���� �߰�.....KB...
	double  m_dLengthSecMoveEnd				;	// ����� �ܺ� �̰ݰŸ�..070208���� �߰�.....KB...
	double  m_dLengthDuctOffset				;   // ������ ��Ʈ �ɼ°Ÿ�..

	double	m_dRoundCrossUpper				;	//���κ� Rounding ó��
	double	m_dRoundChipCrossUpper			;
	double	m_dRoundCrossLower				;
	double	m_dRoundChipCrossLower			;
	double	m_dRoundStringerUpper			;
	double	m_dRoundChipStringerUpper		;	//���κ����ƴ������
	double	m_dRoundStringerLower			;
	double	m_dRoundChipStringerLower		;	//���κ��Ϻ�ƴ������

	BYTE	m_cBracketForm					;	// �������κ� ����
	BYTE	m_cBracketType					;	// �������κ� ��ġ( 0 : ���̾����� ��ġ,1 : ���������� 2��° )
	BYTE	m_bLeftBracket					;	// ���� �������κ� ����
	BYTE	m_bRightBracket					;	// ���� �������κ� ����
	BYTE	m_cBrBeamType					;	// ������ ����
	BYTE	m_bBracketBeamSpliceApply		;	// ������ �������� ���� ����
	double	m_dRoundBracketUpper			;
	double	m_dRoundChipBracketUpper		;
	double	m_dRoundBracketLower			;
	double	m_dRoundChipBracketLower		;

	struct	_BINDCONC m_BindConc			;	//������ũ��Ʈ
	CString m_strJijumName[JIJUMSUMAX]		;	//�����̸�(�ܸ������������� ����)
	double	m_dLengthJigan[KYUNGGANSUMAX]	;	//��������
	double	m_dLengthUganStt				;	//���� ���� �Ÿ�
	double	m_dLengthSlabGirderStt			;	//����������� �Ŵ����ܰ��� �Ÿ�
	double	m_dLengthGirderShoeStt			;	//�Ŵ� ���ܿ��� �������� �Ÿ�
	double	m_dLengthGirderShoeEnd			;	//
	double	m_dLengthSlabGirderEnd			;	//����������� �Ŵ��������� ����
	double	m_dLengthUganEnd				;	//���� ���� �Ÿ�
	CDPoint m_vAngJijum[JIJUMSUMAX]			;	//��������

	BYTE	m_cOffSetStt					;	//���� �ɼ�
	BYTE	m_cOffSetEnd					;	//���� �ɼ�
	double	m_dDisExtLineSttLeft			;	//Ȯ���� ��쿡�� ���
	double	m_dDisExtLineEndLeft			;	//Ȯ���� ��쿡�� ���
	double	m_dDisExtLineSttRight			;	//Ȯ���� ��쿡�� ���
	double	m_dDisExtLineEndRight			;	//Ȯ���� ��쿡�� ���
	BYTE	m_cDirShoe						;	//�� ����->����, ������ ����, ������, ����������
	BYTE	m_cDirVStiffJijum				;	//���������� ����->����, ������ ����, ������, ����������
	
	double	m_dLengthShoeToTaperStt			;	//������ �����۽��ۺα���[�������� �Է�]
	double	m_dLengthTaperToTaperStt		;	//������ ����
	double	m_dHeightTaperStt				;	//������ ����
	double	m_dLengthShoeToTaperEnd			;	//������ �����۽��ۺα���[�������� �Է�]
	double	m_dLengthTaperToTaperEnd		;	//������ ����
	double	m_dHeightTaperEnd				;	//������ ����
	double  m_HTaper_W						;	
	double  m_HTaper_H						;

	double	m_dHeightInOutStt				;	//�������Ա� ����
	double	m_dWidthInOutStt				;	//�������Ա� ����
	double	m_dRadiusInOutStt				;	//�������Ա� round ������
	double	m_dHeightInOutLowerStt			;	//�������Ա� �ϴ� ����
	double	m_dHeightInOutEnd				;	//�������Ա� ����
	double	m_dWidthInOutEnd				;	//�������Ա� ����
	double	m_dRadiusInOutEnd				;	//�������Ա� round ������
	double	m_dHeightInOutLowerEnd			;	//�������Ա� �ϴ� ����

	BYTE	m_cTaperAngleTypeStt			;	//������ ����->����, �ɼ�, ������ ����
	BYTE	m_cTaperAngleTypeEnd			;	//������ ����->����, �ɼ�, ������ ����

	BYTE	m_cFacePlateRoundType			;	//���̽� �÷���Ʈ ����ó��
	BYTE	m_cFacePlateUse					;	//���̽� �÷���Ʈ ��ġ����
	BYTE	m_cFaceManhole					;	//��Ȧ��ġ ����
	
	BYTE	m_cByunDan						;	//[���ܸ� ����]
	long	m_nStyleByun[KYUNGGANSUMAX]		;	//[���ܸ� �Է�] ���ܺ� ��Ÿ��
	long	m_nByunCurveType[KYUNGGANSUMAX]	;	//��, 2�� ����, n�� ����
	double	m_dHeightByun[KYUNGGANSUMAX]	;
	double	m_dLenLeftByun[KYUNGGANSUMAX]	;
	double	m_dLenRightByun[KYUNGGANSUMAX]	;
	double	m_dLenLeftCurve[KYUNGGANSUMAX]	;
	double	m_dLenRightCurve[KYUNGGANSUMAX]	;

	CMatrixStrGrid m_CrossAngleGrid			;	//[���̾����� ��ġ �Է�]
	double	m_dLengthBaseCrossBeam			;	//���� ���� ��ġ 

	double	m_dDisCenterOnCrBeam			;	//���� �߽� ����
	double	m_dDisCenterOnVst				;
	double	m_dDisCenterOnJRib				;
	double	m_dDisCenterOnSplice			;
	double	m_dDisCenterOnFact				;
	double	m_dDisCenterOnCamber			;
	double	m_dDisCenterOnStud				;
	double	m_dDisCenterOnHStiff			;

	int		m_nLengthTypeOnCrBeam			;
	int		m_nLengthTypeOnVst				;
	int		m_nLengthTypeOnJRib				;
	int		m_nLengthTypeOnSplice			;
	int		m_nLengthTypeOnFact				;
	int		m_nLengthTypeOnCamber			;
	int		m_nLengthTypeOnStud				;
	int		m_nLengthTypeOnHStiff			;

	double m_dMoveTermOnSplice;

	double  m_dJiJumTerm[JIJUMSUMAX][JACKUPSUMAX];  // ���������� ���� (���Ÿ���.)
	long	m_nQtyJijumStiff[JIJUMSUMAX];
	CMatrixStrGrid m_VStiffGrid				;	//[���������� ��ġ�Է�]
	BOOL	m_bInputAngleVStiff				;	//���������� ���� �Է� ����
	long	m_nCrossArrangeType				;	//���κ� ��ġ����
	double	m_dLengthBaseVStiff				;
	long	m_nHStiffDefault				;	//[���򺸰��� ��ġ�Է�]
	BYTE	m_cHStiffDimRound				;
	BYTE	m_cHStiffDimRoundValue			;	// 5mm or 10mm
	double	m_dHStiffLengthMin				;
	double	m_dHStiffTRibApply				;	// ���򺸰���� Ⱦ���� ��������
	double  m_dHStiffCheckLength			;	// ���򺸰��� �������� üũ�� ���� ����...
	BYTE	m_cHStiffHeightOffset			;
	double	m_dHStiffHeight1				;
	double	m_dHStiffHeight2				;
	double	m_dHStiffHeight3				;
	double	m_dHStiffMinLenJackup			;
	BYTE	m_cHStiffJackupVStiff			;
	BYTE	m_cFactThickFix					;	// �������� �β� ����
	double	m_dDisOnCenterFactJoin			;	// �������� �̵�
	BYTE	m_cSpliceSimpleType				;	//[�������� ���Է�]

	// [������ ����]
	double	m_dThickSlabLeft				;	// ������ ���� �β�
	double	m_dThickSlabBase				;	// ������ ���� �β�	
	double	m_dThickSlabRight				;	// ������ ���� �β�
	double	m_dThickSlabHunch				;	// ��ġ ����
	double  m_dSlopHunch					;   // ��ġ ���
	double	m_dThickPave					;	// ���� �β�
	double	m_dWidthHunch					;	// ������ ���� ����
	double	m_dHunchDisForBaseThick			;	// ���� �β� ��ġ
	BYTE	m_cLevelRoadCenterToSlabEdge	;	// �����߽ɰ��� ��������, �����߽��� ������ ��.������ ����� ���
	double  m_dHeightDosang					;   // ö���� �������	
	double	m_dMortalProtected				;	// ��ȣ ��Ż �β�
	double  m_dThickSlabLeftExt				;	// ���� �β� For �Ҽ�����
	double  m_dThickSlabRightExt			;	// ���� �β� For �Ҽ�����

	BYTE	m_cCamberUnitToMM				;	//[ķ�� �Է�]
	double  m_dJackUpTerm[JIJUMSUMAX][JACKUPSUMAX];  // ��������� ���� (���Ÿ���.)	
	struct _JACKUP_SOLEPLATE_
	{
		double m_dL;
		double m_dD;
		double m_dH;
		double m_dW;
		double m_dT;
	} m_structJackupJewon[JIJUMSUMAX][2];

	
	BYTE	m_cSameApplySameColCross		;//���ϱ��� ��������
	BYTE	m_cSameApplySameColVStiff		;//���ϱ��� ��������
	BYTE	m_cSameApplySameColJRib			;//���ϱ��� ��������
	BYTE	m_cSameApplySameColDanmyun		;//���ϱ��� ��������
	BYTE	m_cSameApplySameColSplice		;//���ϱ��� ��������
	BYTE	m_cSameApplySameColFactJoin		;//���ϱ��� ��������
	BYTE	m_cSameApplySameColOthers		;//���ϱ��� ��������
	BYTE	m_cSameApplySameColHStiff		;//���ϱ��� ��������
	BYTE	m_cSameApplySameColStud			;//���ϱ��� ��������

	BYTE	m_cFactoryType;					//���ۼ��� �ڵ���� ���� 0 : �ִ밪 ���� 1 : ��������
	double  m_dMaxFactoryValue;				//�ִ밪 ���� ��
	double	m_dRateFactoryValue;			//�������� ��
	//����Ϻ����ǻ�
	int m_nGussetStiff_Gen[3][2];	//[Type][Weld:Bolt]
	double m_dGS_T[3][2];
	double m_dGS_S1[3][2];
	double m_dGS_S2[3][2];
	double m_dGS_S3[3][2];
	double m_dGS_W[3][2];
	double m_dGS_dA[3][2];
	double m_dGS_Gen_dB[3][2];
	double m_dGS_Gen_dC[3][2];
	double m_dGS_Trap_dB[3][2];
	double m_dGS_Trap_dC[3][2];
	double m_dGS_uA[3][2];
	double m_dGS_uB[3][2];
	double m_dGS_uC[3][2];
	//����Ʈ
	double m_dSlotA[3];
	double m_dSlotB[3];
	double m_dSlotR[3];
	double m_dSlotR2[3];
	BOOL m_bApplyDefaultSlot;

	double  m_FieldScallop_BaseT			;   //���� ���� ���� t
	double  m_FieldScallop_R1				;   //���� ���� �� ������t < ����t
	double  m_FieldScallop_R2				;   //���� ���� �� ������t >= ����t
	//
	double m_dGS_Bolt_B[3];			//��Ʈ ���� ����
	double m_dGS_Bolt_H[3];
	double m_dGS_Bolt_R[3];

	double m_dGS_Sub_sT[3][2];			//�κ�����
	double m_dGS_Sub_uD[3][2];
	double m_dGS_Sub_dD[3][2];
	double m_dGS_Sub_uE[3][2];
	double m_dGS_Sub_dE[3][2];
	// ���������� ��ǥ�ܸ��Է½� �ʿ��� ����
	double	m_dA_p; // �Ϲݺ�
	double	m_dB_p;
	double	m_dC_p;
	double	m_dT_p;
	double	m_dC_n; // �θ��Ʈ��
	double	m_dT_n;
	long    m_nInstallVStiff; // ����, ����, ����, ����

	// ����극�̽� ������ ��ǥ�ܸ��Է�
	double m_dHBuSL;
	double m_dHBuGw;
	double m_dHBuSA;
	double m_dHBuSB;
	double m_dHBuSC;
	double m_dHBuSD;
	double m_dHBuSE;
	double m_dHBuStep1;
	double m_dHBuStep2;
	double m_dHBuStep3;
	double m_dHBuStep4;
	double m_dHBuStep5;
	double m_dHBuW;
	double m_dHBuW1;
	CString m_szHBrSpBoltDia;

	///< ���κ��� ���������� ��ġ
	long	m_nInstallVStiffOnCrossBeam; ///< 0 : ��ġ���� 1:�����θ� ��ġ 2:��簡�κ���ġ
	BOOL    m_bTendonInputComplete;


	// ����ȭ �߰� ================
	BOOL m_bUserSlabHunch;
	BOOL m_bQuickLineSearch; // �������� �˻�
	// ============================

	// �ɵ��� �߰� ================
	BOOL m_bUseJackupSolePlate;
	// ============================
	double m_dSameFlangeWidth;
	BYTE	m_cSlabThickAddTypeStt			;	// 0 : ���� 1 : �ι�° ���κ����� 2 : �ܺ� �̰ݰŸ�
	BYTE	m_cSlabThickAddTypeEnd			;	// 0 : ���� 1 : �ι�° ���κ����� 2 : �ܺ� �̰ݰŸ�

	BOOL m_bApplyHunchAtCB; // ���κ� ��ġ����(�Ŵ����� ��ġ��)   ����/������

	// TU�Ŵ��� ���� ==============
	double	m_dLengthTUBindConc_Stt;			// ������ũ��Ʈ ������ �������
	double	m_dLengthTUBindConc_End;			// ������ũ��Ʈ ������ �������

	// �Ŵ��ܺ� ����� ���ذ���
	double	m_dGirEndStndGuttingAngle;

	long m_nBaseHeightPosHBraceSplice;
	double m_dBaseHeightFixPosHBraceSplice;
	double m_dBaseHeightRatioPosHBraceSplice;

// Implementation
protected:
	void SetPOSAndEachPointer();
	// �ɵ��� �߰� ================
//	long m_nBridgeType;				///< ���� ���� 0 : ������, 1 : �Ҽ�����, 2: TU�Ŵ���

public:
	void Serialize(CArchive& ar);

	BOOL CheckSelfData(BOOL bAssertUse);
	CPlateBridge& operator=(const CPlateBridge& obj);
	void MakeGuardWall(long nMake);
	void MakeGuardFence(long nMake, CStringArray& strName);
	void MakeGirder(long nMake, BOOL bCopy=TRUE);

	BOOL IsExtLine(long nG = -1) const;
	BOOL IsJustModified() const;
	BOOL IsModify() const;
	BOOL IsDataFilled() const;

	void SetJustModified(BOOL bModify);
	void SetLengthHDanByLine(double dHDanLen, long nQtyLine) { m_dLengthHDan[nQtyLine] = dHDanLen; }
	double GetLengthHDanByLine(long nQtyLine) { return m_dLengthHDan[nQtyLine]; }
	long GetNumberGirderNearestToCenterLine();
	void SetGirderSearchIP();
	double	GetLengthBridge() const {return m_dLengthUganStt+m_dLengthTotalSlab + m_dLengthUganEnd;}
	BOOL IsHapsung();
	double GetStdMaxTendonHeight();
private:
	long    m_NumHaiseokCombo;	// �ؼ����
};
#endif // !defined(AFX_PLATEBRIDGE_H__5F50DD8D_BE7B_4FDB_A781_1D4BA535B029__INCLUDED_)
