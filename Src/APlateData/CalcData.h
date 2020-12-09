// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// CalcData.h: interface for the CCalcData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCDATA_H__499DDEDD_3ACB_451B_936E_1F344A037A90__INCLUDED_)
#define AFX_CALCDATA_H__499DDEDD_3ACB_451B_936E_1F344A037A90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FLOOR_LEFT	0
#define FLOOR_RIGHT 1
#define FLOOR_MID	2
#define FLOOR_SIZE	3
#define HLLMAXSU	20

#define FLOOR_STRENGTH_DESIGN		0		// �ٴ��� �߾Ӻ� ���� ����
#define FLOOR_EXPERIENCE_DESIGN		1		// �ٴ��� �߾Ӻ� ������ ����

enum FloorType{eFloor_LeftCantilever, eFloor_RightCantilever, eFloor_Center, eFloor_CenterExperience, eFloor_MiddleJijum};

class CConcBasic;
class CDataManage;
class AFX_EXT_CLASS CHLLGuardFence : public CObject
{
public :
	CHLLGuardFence();
	CHLLGuardFence(const CHLLGuardFence& rhs);
	virtual ~CHLLGuardFence();
	CHLLGuardFence& operator=(const CHLLGuardFence& rhs);

	long	m_nGuardIndex;	// CCentSeparation Index
	double	m_dSttHDanPos;	// Ⱦ�ܿ��� ��ȣ�� ������ġ(������ ���ܿ���)
	double	m_dEndHDanPos;	// Ⱦ�ܿ��� ��ȣ�� ����ġ(������ ���ܿ���)
};

class AFX_EXT_CLASS CHLLLaneElement : public CObject
{
public :
	CHLLLaneElement();
	CHLLLaneElement(const CHLLLaneElement& rhs);
	virtual ~CHLLLaneElement();
	CHLLLaneElement& operator=(const CHLLLaneElement& rhs);

	double	m_dLoad;
	double	m_dSttHDanPos;
	double	m_dEndHDanPos;
	BOOL	m_bMove;
};

class AFX_EXT_CLASS CHLLLaneUnit : public CObject
{
public :
	CHLLLaneUnit();
	CHLLLaneUnit(const CHLLLaneUnit& rhs);
	virtual ~CHLLLaneUnit();
	CHLLLaneUnit& operator=(const CHLLLaneUnit& rhs);

	long	m_nLaneType;		// DB or DL
	CString	m_szLCName;
	CString	m_szDescription;
	vector<CHLLLaneElement> m_vLaneElem;
	CHLLLaneElement& GetElementSide(BOOL bLeft);	// �ֿ��� element�� return
};

class AFX_EXT_CLASS CHLLLane : public CObject
{
public :
	CHLLLane();
	CHLLLane(const CHLLLane& rhs);
	virtual ~CHLLLane();
	CHLLLane& operator=(const CHLLLane& rhs);

	vector<CHLLLaneUnit> m_vLaneUnit;
};

class AFX_EXT_CLASS CCalcData
{
public:
	CCalcData(CDataManage* pManage = NULL);
	virtual ~CCalcData();

	CDataManage* m_pDataManage;

	void Serialize(CArchive& ar);

public:
	struct _DESIGN_BASE
	{
		CString m_szBridgeType;        ///<��������:CBaseInfoDlg���� ����
		CString m_szBridgeLength;      ///<��������:CJiganGusengDlg���� ����
		CString m_szBridgeLineType;    ///<��������:CVBracingPositionDlg ����
		double m_dBridgeWidth;         ///<������  :CVBracingPositionDlg 
		double m_dBridgeSkew;          ///<�簢    :CJijumAngleDlg
 	} DESIGN_BASE;

	struct _DESIGN_CONDITION
	{
		CString m_szBridgeGrade;       ///<�������						 :CBaseInfoDlg
		double m_dPf;                  ///<DB����-Pf					 :CBaseInfoDlg
		double m_dPr;                  ///<DB����-Pr					 :CBaseInfoDlg
		double m_dW;                   ///<DL����-���������			 :CBaseInfoDlg
		double m_dPm;                  ///<DL����-��������(���Ʈ ����):CBaseInfoDlg
		double m_dPs;                  ///<DL����-��������(���ܷ� ����):CBaseInfoDlg
	} DESIGN_CONDITION;

	struct _DESIGN_MATERIAL 
	{
		double m_dSigmaCK;            ///<��ũ��Ʈ ���భ��
		double m_dSigmaCi;			  ///<������Ʈ���� ���Խ��� ��ũ��Ʈ ���భ��
		double m_dBindSigmaCK;		  ///<������ũ��Ʈ ���భ��
		double m_dSigmaY;             ///<ö��     �������� 
		CString m_sSigmaY;			  ///400 MPa �̻� ö�� ǥ�� H~~,, ����...D~~
		double m_dSigmaSA;			  ///<ö���� �������
		double m_dSigmaDeckSA;		  ///<ö���� �������
		CString m_szSwsJu;            ///<��������(�ֺ���)
		CString m_szSwsBu;            ///<��������(�κ���)
		double m_dEc;                 ///<��ũ��Ʈ ź����� 
		double m_dEci;				  ///<��ũ��Ʈ ź�����
		double m_dBindEc;			  ///<������ũ��Ʈ ź�����
		double m_dEs;                 ///<ö�� ź�����
		double m_dEst;                ///<���� ź�����
		double m_dDummyEst;			  ///<���̺��� ź�����
		long   m_nN;                  ///<�ٴ��� ź�������
		long   m_nBindNc;			  ///<������ũ��Ʈ ź�������
		double m_dEpsilonS;           ///<���������� 
		double m_dAlpha;              ///<����â��� 
		double m_dCreep;              ///<ũ����
		double m_dCreep2;
		double m_dTemparature;        ///<���� ��ũ��Ʈ�� �µ��� 
		double m_dShearEs;			  ///<���� ź�����
		double m_RatePoasong;		  ///<��ũ��Ʈ ���ƽ���
		//�������
		CString	m_strAllow_PreStressPressPow;	//������Ʈ���� ���� ���� ��������(�������)
		CString	m_strAllow_PreStressStretchPow;	//������Ʈ���� ���� ���� ��������(�������)
		CString	m_strAllow_CreWeightPressPow;	//���� ���� �ۿ�� ��������(�������)
		CString	m_strAllow_CreWeightStretchPow;	//���� ���� �ۿ�� ��������(�������)
		CString	m_strAllow_DeckConcStretchPow;	//�ٴ��� ��� �� ��������(�������)
		CString	m_strCrackPow;					//�տ�����

	} DESIGN_MATERIAL;

	struct _DESIGN_UNIT_WEIGHT
	{       ///<�����߷�
		double m_dGangjae;			///< ���� �����߷�
		double m_dAluminum;			///< �˷�̴� �����߷�
		double m_dChulgun;			///< ö����ũ��Ʈ �����߷�
		double m_dMugun;			///< ���ّ�ũ��Ʈ �����߷�
		double m_dCement;			///< �ø�Ʈ �����߷�
		double m_dMokjae;			///< ���� �����߷�
		double m_dYukchung;			///< ��û��(�����) �����߷�
		double m_dAsphalt;			///< �ƽ���Ʈ �����߷�
		double m_dGravel;			///< �ڰ�,�⼮ �����߷�
	} DESIGN_UNIT_WEIGHT;

	struct _DESIGN_FLOOR_DATA 
	{		
		///<������
		BOOL   m_bBangEm;			///< ������ ����
		double m_dBangEmWeight;		///< ������ ����
		double m_dBangEmHeight;		///< ������ ����
		double m_dBangEmDis;		///< ������ ���ϰŸ�(�������ܺ���)
		///< ����
		BOOL   m_bNanGan;			///< ���� ����
		double m_dNanGanWeight;		///< ���� ����
		double m_dNanGanDis;		///< ���� ���ϰŸ�		
		double m_dNanGanHeight;		///< ���� ����
		///< ��������
		BOOL   m_bWalkLoad;	        ///< ���� ����
		BOOL   m_bPeopleLoad;		///< ���� ����
		double m_dWalkLoadWidth;	///< ���� ��
		double m_dWalkLoad1;		///< ���� ���ۺκ� ��������
		double m_dWalkLoad2;        ///< ���� ��  �κ� ��������
		double m_dWalkLoadMoment;	///< ���� ���Ʈ ���� ���� ����...070319...KB
		double m_dWalkLoadH;		///< ���� ����
		double m_dWalkLoadLEdge[HLLMAXSU];///<���� ���� ��ǥ(�������������ܿ���)
		double m_dWalkLoadREdge[HLLMAXSU];///<���� ������ ��ǥ(�������������ܿ���)
		///<��ȣ��
		BOOL   m_bBangHo;				///<��ȣ�� ����
		double m_dBangHoWidth;			///<��ȣ�� �� 
		double m_dBangHoWeight;			///<��ȣ�� ����
		double m_dBangHoLEdge[HLLMAXSU];///<��ȣ�� ���� ��ǥ(�������������ܿ���)
		double m_dBangHoREdge[HLLMAXSU];///<��ȣ�� ������ ��ǥ(�������������ܿ���)
	} DESIGN_FLOOR_DATA[FLOOR_SIZE];

	struct _CALC_CANTILEVER_DATA
	{
		double m_Min_T;	///< �ٴ��� �ּ� �β�
		double m_Req_T;	///< �ٴ��� �ʿ� �β�

		double	m_Stop_M;	///< ������ ���Ʈ( Slab ���� )
		double	m_Stop_Ton;	///< ������ ũ��  ( Slab ���� )
		double	m_StopE_M;	///< ������ ���Ʈ( Slab ���� )
		double	m_StopE_Ton;///< ������ ũ��  ( Slab ���� )
		double	m_StopB_M;	///< ������ ���Ʈ(Bracket)
		
		double	m_Move_L;	///<�÷��������� �߾ӿ��� Pr�������������� �Ÿ�
		double  m_Dead_L;   ///<�÷��������� �߾ӿ��� ���������������� �Ÿ�
		double  m_Tank_L;	///<�÷��������� �߾ӿ��� ��ũ���� ���������� �Ÿ�
		double  m_Trailer_L;///<�÷��������� �߾ӿ��� Ʈ���Ϸ� ���� ���������� �Ÿ�

		double	m_Move_E;   ///<Ȱ���߿� ���� ��������
		double	m_Dead_E;   ///<�����߿� ���� ��������
		double	m_Move_E3;
		double	m_Move_i;   ///<��ݰ��
		double	m_Move_Pr;  ///<Ÿ�̾�����
		double	m_Move_Ml;  ///<Ȱ���߿� ���� ���Ʈ
		double	m_Move_Ml_;  ///<Ȱ���߿� ���� ���Ʈ
		double	m_MoveB_Ml;
		double	m_MoveB_Ml_Tank;
		double	m_MoveB_Ml_Trailer;
		double	m_Move_Ml_Tank;///<��ũ�˵����߿� ���� ���Ʈ
		double	m_Move_Ml_Trailer;///<��ũƮ���Ϸ����߿� ���� ���Ʈ
		long	m_Move_GiJumsu;///<Ⱦ�ܸ��� ������(�Ŵ����� ������)
		double	m_Move_M;   ///<�������� ���� �ڸ��Ʈ���(�������� 3���̻��϶� �ڸ��Ʈ�� 0.8��)

		double	m_Coll_V;	///<�浹���� �ӵ�
		double	m_Coll_H;   ///<�浹���� �ۿ����
		double	m_Coll_Co;  ///<�浹����
		double	m_Coll_Mo;  ///<�浹���߸��Ʈ
		double  m_Nangan_VerF;///<������ �ۿ��ϴ� ������(�������⿡..��� �����浹��)
		double  m_Nangan_HoriF;///<������ �ۿ��ϴ� �����(������ ���ٰ� �ε�ĥ��)
		double  m_Nangan_dH; //������ �ۿ��ϴ� ����°� ������ ����
		double  m_Nangan_dL; //������ �ۿ��ϴ� �����°� ������ ����

		double	m_Wind_Pw;  ///<ǳ����
		double	m_Wind_Mw;  ///<ǳ���� ���Ʈ
		double  m_Wind_H;   ///<�V���� ���� ����

		double	m_Cent_R;			///<�������߹ݰ�
		double	m_Cent_Pcf;			///<��������ũ��
		double	m_Cent_Pcf_Tank;	///<��ũ��������ũ��
		double	m_Cent_Pcf_Trailer;	///<Ʈ���Ϸ���������ũ��
		double	m_Cent_Mcf;			///<�������߸��Ʈ
		double	m_Cent_Mcf_Tank;	///<��ũ�������߸��Ʈ
		double	m_Cent_Mcf_Trailer;	///<Ʈ���Ϸ��������߸��Ʈ
		double	m_CentB_Mcf;
		double	m_CentB_Mcf_Tank;
		double	m_CentB_Mcf_Trailer;
		
		double	m_Rein_Mu;	  ///<���Ѹ��Ʈ
		double	m_Rein_Md;	  ///<���� ö�ٷ� ���Ʈ 
		double  m_Rein_a;     ///<UseAs*fy/(k1*Ck*b)
		double  m_Rein_b;     ///<b
		double  m_Rein_d;     ///<m_Min_T-m_dCoverUp

		// �߾Ӻ� �ٴ��� ������ ����(�߾Ӻθ� ���)
		CString m_Rein_Up_CaseH1_1Cy;	// ö������(�Ϲݺ� ��ö�� ��� 1-Cycle)
		CString	m_Rein_Up_CaseH1_2Cy;	// ö������(�Ϲݺ� ��ö�� ��� 2-Cycle)
		long	m_Rein_Up_Combo1_1Cy;	// �Ϲݺ� ��ö�� ��� 1-Cycle
		long	m_Rein_Up_Combo1_2Cy;	// �Ϲݺ� ��ö�� ��� 2-Cycle
		double	m_Rein_Up_Ctc1;			// ö�ٰ���(�Ϲݺ� ��ö�� ���)
		double	m_Rein_Up_Asuse1_1Cy;	// ���ö�ٷ�(�Ϲݺ� ��ö�� ��� 1-Cycle)
		double	m_Rein_Up_Asuse1_2Cy;	// ���ö�ٷ�(�Ϲݺ� ��ö�� ��� 2-Cycle)
		double	m_Rein_Up_Asuse1;		// ���ö�ٷ�(�Ϲݺ� ��ö�� ���)
		double	m_Rein_Up_Asreq1;		// �ʿ�ö�ٷ�(�Ϲݺ� ��ö�� ���)
		CString	m_Rein_Up_CaseH2;		// ö������(�Ϲݺ� ���ö�� ���)
		long	m_Rein_Up_Combo2;		// �Ϲݺ� ���ö�� ���
		double	m_Rein_Up_Ctc2;			// ö�ٰ���(�Ϲݺ� ���ö�� ���)
		double	m_Rein_Up_Asuse2;		// ���ö�ٷ�(�Ϲݺ� ���ö�� ���)
		double	m_Rein_Up_Asreq2;		// �ʿ�ö�ٷ�(�Ϲݺ� ���ö�� ���)
		double	m_bCheck_Up;			// �Ϲݺ� ��ö�� ��� ����
		double	m_bCheck_UpB;			// �Ϲݺ� ���ö�� ��� ����
		double	m_Rein_Lo_Asreq;		// �Ϲݺ� ��ö�� �ϸ� �ʿ�ö�ٷ�
		double	m_Rein_Lo_Asuse;		// �Ϲݺ� ��ö�� �ϸ� ���ö�ٷ�
		double	m_Rein_Lo_BAsreq;		// �Ϲݺ� ���ö�� �ϸ� �ʿ�ö�ٷ�
		double	m_Rein_Lo_BAsuse;		// �Ϲݺ� ���ö�� �ϸ� ���ö�ٷ�

		CString m_Rein_CaseH1;		// ö������(�Ϲݺ� ��ö�� 1-Cycle)
		CString m_Rein_CaseH1_2Cy;	// ö������(�Ϲݺ� ��ö�� 2-Cycle)
		CString m_Rein_CaseH2;		// ö������(�ܺ�   ��ö�� 1-Cycle)
		CString m_Rein_CaseH2_2Cy;	// ö������(�ܺ�   ��ö�� 2-Cycle)
		CString m_Rein_CaseH3;		// ö������(�Ϲݺ� ���ö��)
		CString m_Rein_CaseH4;		// ö������(�ܺ�   ���ö��)
		int m_Rein_Combo1;			// �Ϲݺ� ��ö��Index 1-Cycle 
		int m_Rein_Combo1_2Cy;		// �Ϲݺ� ��ö��Index 2-Cycle
		int m_Rein_Combo2;			// �ܺ�   ��ö��Index 1-Cycle
		int m_Rein_Combo2_2Cy;		// �ܺ�   ��ö��Index 2-Cycle
		int m_Rein_Combo3;			// �Ϲݺ� ���ö��
		int m_Rein_Combo4;			// �ܺ�   ���ö��
		double m_Rein_Ctc1;			// ö�ٰ���(�Ϲݺ� ��ö��)
		double m_Rein_Ctc2;			// ö�ٰ���(�ܺ�   ��ö��)
		double m_Rein_CtcB;			// ö�ٰ���(�Ϲݺ� ���ö��)
		double m_Rein_CtcB1;		// ö�ٰ���(�ܺ�   ���ö��)

		double m_Rein_UseAs;		// ���ö�ٷ�(�Ϲݺ� ��ö�� ��)
		double m_Rein_UseAs_1Cy;	// ���ö�ٷ�(�Ϲݺ� ��ö�� 1-Cycle)
		double m_Rein_UseAs_2Cy;	// ���ö�ٷ�(�Ϲݺ� ��ö�� 2-Cycle)
		double m_Rein_UseAs2;		// ���ö�ٷ�(�ܺ�   ��ö�� ��)
		double m_Rein_UseAs2_1Cy;	// ���ö�ٷ�(�ܺ�   ��ö�� 1-Cycle)
		double m_Rein_UseAs2_2Cy;	// ���ö�ٷ�(�ܺ�   ��ö�� 2-Cycle)
		double m_Rein_UseAsBr;
		double m_Rein_BAsuse;		// ���ö�ٷ�(�Ϲݺ� ���ö��)
		double m_Rein_BAsuse2;		// ���ö�ٷ�(�ܺ�   ���ö��)

		double m_Rein_Asreq;		// �ʿ�ö�ٷ�(�Ϲݺ� ��ö��)
		double m_Rein_BAsreq;		// �ʿ�ö�ٷ�(�Ϲݺ� ���ö��)==>�ܺδ� �Ϲݺ��� 2����
		
		double m_Rein_Pused;		// m_Rein_UseAs/(b*d);
		double m_Rein_dia;			// ö��ġ���� ���� ����
		BOOL	m_bCheckGen1;		// �Ϲݺ� ��ö��   ����
		BOOL	m_bCheckGen2;		// �Ϲݺ� ���ö�� ����
		BOOL	m_bCheckDan1;		// �ܺ�   ��ö��   ����
		BOOL	m_bCheckDan2;		// �ܺ�   ���ö�� ����
		///<�տ�����
		double m_Crack_w;     ///<�տ���
		double m_Crack_wa;    ///<���տ�����
		double m_Crack_wBr;   ///<�տ��� - Bracket
		double m_Crack_waBr;  ///<���տ��� - Bracket

		/*
		CString	m_Rein_CaseH_T;	///< ö������(�Ϲݺ� ����) - �Ҽ�����
		int		m_Rein_Combo_T;	///< H19�Ϲݺ� ����
		double	m_Rein_Ctc_T;	///< �Ϲݺ� ���� ö�ٰ���
		double	m_Rein_UseAs_T;	///< �Ϲݺ� ���� ���ö�ٷ�

		CString	m_Rein_CaseH_C;	///< ö������(�Ϲݺ� ����) - �Ҽ�����
		int		m_Rein_Combo_C;	///< H19�Ϲݺ� ����
		double	m_Rein_Ctc_C;	///< �Ϲݺ� ���� ö�ٰ���
		double	m_Rein_UseAs_C;	///< �Ϲݺ� ���� ���ö�ٷ�
		*/
		// �Ҽ��������� ö��
		CString	m_strGenReinTDia_1Cy;		// ö������ �Ϲݺ� ��ö�� ����� 1Cycle
		CString	m_strGenReinTDia_2Cy;		// ö������ �Ϲݺ� ��ö�� ����� 2Cycle
		long	m_nGenReinTDiaIdx_1Cy;		// ö������ �Ϲݺ� ��ö�� ����� 1Cycle
		long	m_nGenReinTDiaIdx_2Cy;		// ö������ �Ϲݺ� ��ö�� ����� 2Cycle
		double	m_dGenReinTCTC;				// ö�ٰ��� �Ϲݺ� ��ö�� �����
		double	m_dGenReinTAsreq;			// ö������ �Ϲݺ� ��ö�� ����� �ʿ�ö�ٷ�
		double	m_dGenReinTAsuse;			// ö������ �Ϲݺ� ��ö�� ����� ���ö�ٷ�
		double	m_dGenReinTAsuse_1Cy;		// ö������ �Ϲݺ� ��ö�� ����� ���ö�ٷ� 1Cycle
		double	m_dGenReinTAsuse_2Cy;		// ö������ �Ϲݺ� ��ö�� ����� ���ö�ٷ� 2Cylcle

		CString	m_strGenReinCDia_1Cy;		// ö������ �Ϲݺ� ��ö�� ����� 1Cycle
		CString	m_strGenReinCDia_2Cy;		// ö������ �Ϲݺ� ��ö�� ����� 2Cycle
		long	m_nGenReinCDiaIdx_1Cy;		// ö������ �Ϲݺ� ��ö�� ����� 1Cycle
		long	m_nGenReinCDiaIdx_2Cy;		// ö������ �Ϲݺ� ��ö�� ����� 2Cycle
		double	m_dGenReinCCTC;				// ö�ٰ��� �Ϲݺ� ��ö�� �����
		double	m_dGenReinCAsreq;			// ö������ �Ϲݺ� ��ö�� ����� �ʿ�ö�ٷ�
		double	m_dGenReinCAsuse;			// ö������ �Ϲݺ� ��ö�� ����� ���ö�ٷ�
		double	m_dGenReinCAsuse_1Cy;		// ö������ �Ϲݺ� ��ö�� ����� ���ö�ٷ� 1Cycle
		double	m_dGenReinCAsuse_2Cy;		// ö������ �Ϲݺ� ��ö�� ����� ���ö�ٷ� 2Cylcle

	} CALC_CANTILEVER_DATA[FLOOR_SIZE];

	struct _DESIGN_STEELPRESTRESS_DATA
	{
		long	m_nSteelPrestressIdx;							///< ��� Pressstress ������ ��ȣ
		CString	m_szSteelPrestressName;							///< ��� Pressstress ������ ȣĪ
		double	m_dEs;											///< ź�����
		double	m_dArea;										///< ��Ī �ܸ���
		double	m_dDiameter;									///< ����
		double	m_dLoadOfElongation;							///< ��������
		double	m_dTensionLoad;									///< ��������
		double	m_dTensionStrength;								///< ���尭��
		double	m_dYieldStrength;								///< �׺�����		
		double	m_dWobbleFriction;							///< �Ļ������
		double	m_dCurvatureFriction;							///< ����������
		long	m_nDuctType;									///< 1:�ݼӽ��� 2:�ƿ����� �ݼӽ��� 3:�ƽ���Ʈ�Ǵ±׸������� 4:�ƿ����ݵȰ�����Ʈ
		long	m_nPrestressQty;								///< PS���� ����
		double	m_dPrestressCTC;								///< ����
//		double	m_dSttAnchorageSlip;							///< ������ ������ Ȱ����-2009���� ������
//		double	m_dEndAnchorageSlip;							///< ������ ������ Ȱ����-2009���� ������
		double	m_dAnchorageSlip;								///< ������ Ȱ���� 
		double	m_dDuctDiameter;								///< ��Ʈ ����
		double	m_dHumidity;									///< ��ջ�����		
		long	m_nVerRein;										///< ����ö�� combo		
		double	m_dVerReinAs;									///< ����ö�ٷ�		
		long	m_nVerReinQty;									///< ����ö�ټ�		
		double	m_dFixingPlateLength;							///< �������� ����

		//������ ���
		long	m_nHorReinQty1;									///< ���� ������ ö�ټ�
		long	m_nHorReinQty2;									///< ���� ������ ���� ö�ټ�
		long	m_nFixingPlateType;								///< �������� ����
		long	m_nHorRein1;									///< ���� ������ ö�� combo
		long	m_nHorRein2;									///< ���� ������ ���� ö�� combo
		double	m_dHorReinAs1;									///< ���� ������ ö�ٷ�
		double	m_dHorReinAs2;									///< ���� ������ ���� ö�ٷ�
		//�ű�...060831	
		long	m_nAnchorageEA;			//�����Ǵ� ������
		double  m_dAnchorageWidth;		//������ ����
		double  m_dAnchorageHeight;		//������ ����
		double  m_dSpallingForce;		//T1 ������ ��� �ҷ·�
		double  m_dBurstingForce;		//T2 ������ �պκ��� �Ŀ���
		double  m_dReqAs1;				//T1�� 
		double  m_dUseAs1;				//T1�� 
		double  m_dReqAs2;				//T2�� 
		double  m_dUseAs2;				//T2��
		double  m_dReinCTC;				//ö�ٰ���
		CString m_sCaseH1;
		CString m_sCaseH2;
		double  m_dPj;				//�������� ��������� ���

		double	m_dPSSteelYP;	//PS ������ ������ ���� ���(��p)
		double	m_dSheathW;		//������ ��
		double	m_dSheathH;		//������ ����
		CString	m_sJackMax;		//Jack�� ���� �ִ� ��������

		//�ű�...090723
		long	m_nVerReinQty2;									///< ���� ������ ���� ö�ټ�
		long	m_nVerRein2;									///< ���� ������ ���� ö�� combo

	} DESIGN_STEELPRESTRESS_DATA;

	int m_nLBDeckDiaIdx;					// LB-Deck ���� index
	double m_dLBDeckCTC;					// LB-Deck CTC
	double m_dLBDeckCover;					// LB-Deck �Ǻ�
	
	double  m_Pib;		  ///<�ڿ� ���� ���� ���� ���
	double  m_Pis;		  ///<���ܿ� ���� ���� ���� ���
	double  m_dCoverUp;   ///<��ũ��Ʈ�Ǻ��β�(���)
	double  m_dCoverDn;   ///<��ũ��Ʈ�Ǻ��β�(�Ϻ�)
	double  m_dCoverSd;   ///<��ũ��Ʈ�Ǻ��β�(����)
	double  m_dRebarPmax; ///<0.75*Pb;
	double m_Rein_Arrange;///<�ܺ�ö���� ��ġ����
	///< CGiJumJujngDlg (�ٴ��� ����+����� ����)
	double m_Asreq;       ///<���������� �ٴ��� ������� �ʿ� ö�ٷ�
	double m_Ag;          ///<��ũ��Ʈ �ܸ���
	double m_AsEa1;       ///<ö�ٰ���1
	double m_AsEa2;       ///<ö�ٰ���2 
	double m_Asuse1;      ///<ö�ٰ���1
	double m_Asuse2;	  ///<ö�ٷ�1	
	double m_Asuset;      ///<ö�ٷ�2 
	double m_Ju_ratio;    ///<�����
	double m_dia1;        ///<ö������1
	double m_dia2;        ///<ö������2
	double m_Ctc1;        ///<ö�ٰ���1
	double m_Ctc2;        ///<ö������2
	CString m_CaseH1;     ///<ö��ġ��1
	CString m_CaseH2;     ///<ö��ġ��2
	int m_ComboInnerJijumSlab1;///<ö�� ȣĪ�޺�1
	int m_ComboInnerJijumSlab2;///<ö�� ȣĪ�޺�2
	BOOL m_bCheckInnerJijum;	// ���������� ö�ٷ�
	long m_nEndPartRebarConditionIdx; // �߾Ӻ� �ܺ� ����� ĵƿ������ ö�ٿ� ���� �ʿ� ���� idx; 0 - 1�� , 1 - 2�� , ���� �߰��� ����� ��

	double m_dSpeed;				///< ���� �ӵ�
	double m_dWindLoadForce;		///< ǳ���� ����
	double m_dRadiusOfCurvature;	///< ���������
	BOOL m_bTank;					///< ��ũ��������
	BOOL m_bTrailer;				///< Ʈ���Ϸ���������
	BOOL m_bMugun;					///< ��ȣ�� �ٴ��� ������ũ��Ʈ�����߷�	
	BOOL m_bJungbunMugun;			///< �ߺд� �ٴ��� ������ũ��Ʈ ���뿩�� >
	BOOL m_bExtraLoad;				///< �ΰ����� ����
	BOOL m_bWind;					///< ǳ��������
	CStringArray m_ELoadTitleArr;	///< �ΰ����� Title
	CDoubleArray m_ELoadWeightArr;	///< �ΰ����� ����
	CDoubleArray m_ELoadDisArr;		///< �ΰ����� ���ϰŸ�(�������ܺ���)	
	CStringArray m_szRefArray;		///< ������

	BOOL m_bIsFirst;				///< bri �ε� �� ù �����ΰ�?
	BOOL m_bHLLError;				///< ��ȣ���� ���� 3�� �̻��϶� Error
	BOOL m_bLWalkHLL;				///< Ⱦ�й迡 ��������(����) ���ԵǾ���
	BOOL m_bRWalkHLL;				///< Ⱦ�й迡 ��������(������) ���ԵǾ���
	BOOL m_bNoPojang;				///< ����bri(Ⱦ�ܸ鵵�� �������߸� ����)

	CDoubleArray m_dStationHLLPos;	///<Ⱦ�й���ġ �����̼�
	long m_HLType;					///< Ⱦ�й� Ÿ��
	long m_HLsu;					///< Ⱦ�й� ��
	long m_nHLiveLoadType;			///< Ⱦ�й�� Ȱ���� ���� ���
	BOOL m_bNoDistOnNoGuard;		///< �������� ���� �ܺ�����

	BOOL m_bCheckSettlement;		///< ����ħ�ϸ𵨸��� �ϴ°�?
	BOOL m_bPassedCalcFloorDlg;		///< [�ٴ��� �����������Է�]�� �����°�?
	BOOL m_bRunCross;				///< Ⱦ�й踦 ����Ͽ��°�?
	BOOL m_bRunCrossWind;			///< ǳ���� �����͸� �о��°�?
	BOOL m_bRunInfluence;			///< ������ �𵨸��� �����߳�
	BOOL m_bMakeModeling;			///< �𵨸��� �Ͽ�����?(����: s2k�� ������ �𵨸��� �ϸ� ������ �����Ͱ� �������� ��)
	BOOL m_bReadFrameModelingOutput;///< �����𵨸� �����͸� �о��°�?
	BOOL m_bReadLiveDispOutput;     ///< Ȱ����ó�� �ؽ�Ʈ���� �о��°�?

	CString m_szFolderPath;		    ///< sap s2k������ ����(�𵨸�)
	CStringArray m_sz3DFrameOutFilePath;// 3D Frame Output ���� �н�


	long m_CurHL;					///< ComboBox ���� ���� ����(Ⱦ�й� Index)					
	long m_CurLiveHL;				///< �𵨸� ���Ͻ��� Ȱ���� ������ġ Ⱦ�й� Index 
	
	double m_dDesignLaneWidth;		///< ���� ������ ( <= 3.6m )
	double m_dUserLaneWidth;		///< ���� ������ (����� ����)
	CDDWordArray	m_nArrDesignLane;	// �������� ��
 	long   m_DesignChasunSu;		///< �������� ��(�ߺд� ���� or �ߺд� ������) 
	long   m_DesignChasunSuR;		///< �������� ��(�ߺд� ������)

	long m_AddRowTRL;				///< ���������� �߰��� ���� ActionGroup��ȣ ����ġ
	double m_MaxImpact;				///< ������ Ȱ���� ��ݰ���� �� ���� ū��
	double m_DBWidth;				///< DB ����        = 1.8
	double m_DBFromEdge;			///< DB �������κ����� �Ÿ� = 0.3
	double m_DBDis;					///< DB ����   �Ÿ� = 1.2
	double m_DLDis;					///< DL ������ �Ÿ� = 1.3

	///< Tank & Tailer ����
	double m_TrailWidth;
	double m_TrailDis;
	double m_TankWidth;
	double m_TankDis;

	double m_TankWeight;
	double m_TankEdit1;
	double m_TankEdit2;
	double m_TankEdit3;
	double m_TankImpact;

	double m_TrailEdit1;
	double m_TrailEdit2;
	double m_TrailEdit3;
	double m_TrailEdit4;
	double m_TrailEdit5;
	double m_TrailEdit6;
	double m_TrailImpact;

	double m_TrailP1;
	double m_TrailP2;
	double m_TrailP3;

	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexPyun;	///< Ⱦ�й�(���纯ȭ��ġ[���κ����� üũ])
	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexWidth;///< Ⱦ�й�(��������ȭ��ġ[���κ����� üũ])
	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexPyunWidth;///< Ⱦ�й�(���纯ȭ+��������ȭ��ġ[���κ����� üũ])
	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexRun;	///< Ⱦ�й�(���� Ⱦ�й踦 ������ ��ġ�� bx)

	BOOL m_bLoadLeftRight;                                  ///< Ⱦ�й� ���� ���

	CDDWordArray m_DArrDistanceDB[HLLMAXSU];				///< Ⱦ���� Ȱ����(���ʺ��� DB���߱����� �Ÿ�)
	CDDWordArray m_DArrDistanceDL[HLLMAXSU];				///< Ⱦ���� Ȱ����(���ʺ��� DL���߱����� �Ÿ�)

	CDDWordArray m_DArrDBChasun[HLLMAXSU];					///< Ⱦ���� Ȱ����(���� ��ȣ)
	CDDWordArray m_DArrDLChasun[HLLMAXSU];					///< Ⱦ���� Ȱ����(���� ��ȣ)

	CDDWordArray m_DArrMaxDBChasun[HLLMAXSU];				///< DB�ִ� ������
	CDDWordArray m_DArrMaxDLChasun[HLLMAXSU];				///< DL�ִ� ������

	CDDWordArray m_DArrDistanceTank[HLLMAXSU];				///< Tank ���� (���ʺ����� �Ÿ�)
	CDDWordArray m_DArrDistanceTrail[HLLMAXSU];				///< Trailer ���� (���ʺ����� �Ÿ�)

	// ǳ���� �����
	double m_dLeftWindFroceFromL[JIJUMSUMAX];				// ����,�ٶ����� -->
	double m_dLeftWindFroceFromR[JIJUMSUMAX];				// ����,�ٶ����� <--
	double m_dRightWindFroceFromL[JIJUMSUMAX];				// ����,�ٶ����� -->
	double m_dRightWindFroceFromR[JIJUMSUMAX];				// ����,�ٶ����� <--
	double m_dWindForceToGirderL[JIJUMSUMAX];				// Ȱ�������Ͻ� �Ŵ����鿡 �ۿ��ϴ� ����
	double m_dWindForceToGirderR[JIJUMSUMAX];				// Ȱ�������Ͻ� �Ŵ����鿡 �ۿ��ϴ� ����
	// ǳ���� ���Ʈ
	double m_dLeftWindMomentFromL[JIJUMSUMAX];				// ����,�ٶ����� -->
	double m_dLeftWindMomentFromR[JIJUMSUMAX];				// ����,�ٶ����� <--
	double m_dRightWindMomentFromL[JIJUMSUMAX];				// ����,�ٶ����� -->
	double m_dRightWindMomentFromR[JIJUMSUMAX];				// ����,�ٶ����� <--

	double  m_AddLoad;										///< �μӼ��� ����(kN/m2)
	double  m_TotalWeight;									///< �� ���緮(kN)
	double  m_ModelWeight;									///< �𵨸��� ���ϵ� ���緮
	double  m_BonusRate;									///< �κ��� ������(�𵨸� ���� ����)
	double  m_JijumDownSize;								///< ����ħ�Ϸ�
	long	m_nModelingSettleMethod;						///< ����ħ�� ��������
	BOOL	m_bApplyAddRow;									///< �ռ��� ������ : ����� �Է�(ActionGroupMat ��ȣ �и���?����)
	BOOL    m_bPic;											///< ���� ��½� �׸� �߰� ����
	BOOL	m_bBucklingStress;								///< ���� ������� �����±���� ����
	CStringArray m_ArrExtraSecTitle;						// ����� �ܸ� ���� �߰�(Ÿ��Ʋ) 
	CDWordArray  m_ArrExtraSecGir;							// ����� �ܸ� ���� �߰�(�Ŵ��ε���) 
	CDDWordArray m_ArrExtraSecSta;							// ����� �ܸ� ���� �߰�(Sta) 
	double m_dMatMinSpace;									// ����� �ܸ� ���� �ּұ��� 
	double m_dMatMaxSpace;									// ����� �ܸ� ���� �ִ����
	// �ܸ����µ����� ���.
	CDWordArray  m_ArrMomentZeroSta;						// �������� ��ġ
	CDWordArray  m_ArrMinZeroSta;							// �������� Min ��ġ
	CDWordArray  m_ArrMaxZeroSta;							// �������� Miax��ġ

	CVectorArray m_ArrSec;									// ��ü ������ ���� ���ĵ� ���� �ܸ�(x: �Ŵ���ȣ  y: �����̼�  z: �ܸ��ȣ)
	CVectorArray m_ArrSecSpDesign;							// ��ü ������ �������� ������ġ(5. ������ ����� ����)	

	CVectorArray m_ArrSecHBeam;								// H��������
	CVectorArray m_ArrSecHBeamLSp;							// H��������(������ ������ �����ΰ��)
	CVectorArray m_ArrSecHBeamRSp;							// H��������(������ ������ �����ΰ��)
	CVectorArray m_ArrSecHBeamLRSp;							// H��������(��, ������ ������ �����ΰ��)
	CVectorArray m_ArrSecCross;								// ���Ʈ�������� ���� ���κ���ġ
	CVectorArray m_ArrSecCrossMoment;						// ���Ʈ�������� �ִ� ���κ���ġ		
	CVectorArray m_ArrSecCrossWithStg;						// ���Ʈ�������� ���� ���κ����� ���κ���ġ
	CVectorArray m_ArrSecCrossWithStgMoment;				// ���Ʈ�������� �ִ� ���κ����� ���κ���ġ
	CVectorArray m_ArrSecCrossConnectionPlate;				// ���Ʈ�������� �ִ� ���κ����� ���κ���ġ
	CVectorArray m_ArrSecStringer;							// ���κ���ġ
	CVectorArray m_ArrSecVBracing;							// �����극�̽� ��ġ
	CVectorArray m_ArrSecHBracing;							// ����극�̽� ��ġ


	long m_nFatigueRoadType;								// �Ƿΰ���� ������ ����
	long m_nFatigueStructureType;							// �Ƿΰ���� ��������		

	int		m_nTypePos;										// ����������ġ(A1, A2)
	int		m_nTypeResist[2];							    // ������������
	int		m_nTypeArea[2];									// ����/ �ѷ� ����...
	double	m_dLenSinchukGir[2];							// ���� �麸�� �Ÿ�
	double	m_dHeightGir[2];								// ������ ���� h :
	double	m_RotAngle;										// ȸ���� ��:
	double	m_Bfa;											// ���� ��� b :
	double	m_Tfa;											// �µ���ȭ�� ��T :
	double	m_Afa;											// ����â��� a :	
	double	m_D1[2];										// ��ġ������ d1 :
	double	m_D2[2];										// �ΰ� ������ d2 :
	double  m_MaxUgan[2];									// �ִ�����
	double  m_MinUgan[2];									// �ּ�����
	CString m_szUseJoint[2];								// ����ƮŸ��

	double m_dEQd;											// ������ ��κ���;
	double  m_AllowUgan[2];									//�����෮...

	CString GetFixingPlateType(long nType);
	long GetFixingPlateType(CString strFixingPlateType);

	void	SetShoeGirderLower(BOOL bGirderLower)	{ m_bShoeGirderLower = bGirderLower;	}
	BOOL	IsShoeGirderLower()	{ return m_bShoeGirderLower;	}

	//���� => �� ��ȯ �Լ�

	//Jack�� ���� �ִ� �������� ��갪
	double GetJackMaxStrToDouble(CString strFormula);

	double GetConstPSfpu();
	double GetConstPSfy();
	CString GetStrConstPSfpu();
	CString GetStrConstPSfy();
	double GetConstPreStressCompress();		// ������Ʈ���� �������� - �������� 
	double GetConstPreStressStretch();		// ������Ʈ���� �������� - ��������
	double GetConstDesignLoadCompress();	// �������� �ۿ��(�ս���) - ��������
	double GetConstDesignLoadStretch();		// �������� �ۿ��(�ս���) - �������� 

	void GetDefaultConstPSf(CStringArray &strArrConstPSf);
	void SetDefaultAllowDesignMaterials();
	long	m_nFlag;

private:
	BOOL	m_bShoeGirderLower;		// ������ħ �Ŵ��ϴ� ����
	double GetPSfty(CString strFormula);
};

#endif // !defined(AFX_CALCDATA_H__499DDEDD_3ACB_451B_936E_1F344A037A90__INCLUDED_)
