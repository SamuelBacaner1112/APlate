// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OptionStd.h: interface for the COptionStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONSTD_H__8B89B1AA_4BD1_4C99_95F8_1BDF997A69DA__INCLUDED_)
#define AFX_OPTIONSTD_H__8B89B1AA_4BD1_4C99_95F8_1BDF997A69DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ��鵵 ���� ǥ��
#define DRAW_PLAN_WEB				0x000001
#define DRAW_PLAN_JACKUP			0x000002
#define DRAW_PLAN_SPLICE			0x000004
#define DRAW_PLAN_FACT				0x000008
#define DRAW_PLAN_STRINGER			0x000010
#define DRAW_PLAN_CROSSBEAM			0x000020
#define DRAW_PLAN_BRACKET			0x000040
#define DRAW_PLAN_BRBEAM			0x000080
#define DRAW_PLAN_SOLEPLATE			0x000100
#define DRAW_PLAN_SLAB				0x000200
#define DRAW_PLAN_ALL				0xFFFFFF

// ���鵵 ��� ����
#define DRAW_JONG_HSTIFF			0x000001
#define DRAW_JONG_VSTIFF			0x000002
#define DRAW_JONG_SPLICE			0x000004
#define DRAW_JONG_FACT				0x000008
#define DRAW_JONG_SOLEPLATE			0x000010
#define DRAW_JONG_JACKUP			0x000020
#define DRAW_JONG_SLAB				0x000040
#define DRAW_JONG_ALL				0xFFFFFF

// �������� ���
#define BUJEINFO_FLANGE_NAME		0x000001
#define BUJEINFO_FLANGE_MAT			0x000002
#define BUJEINFO_WEB_NAME			0x000004
#define BUJEINFO_WEB_MAT			0x000008
#define BUJEINFO_HSTIFF_NAME		0x000010
#define BUJEINFO_HSTIFF_MAT			0x000020
#define BUJEINFO_VSTIFF_NAME		0x000040
#define BUJEINFO_VSTIFF_MAT			0x000080
#define BUJEINFO_JACKUP_NAME		0x000100
#define BUJEINFO_JACKUP_MAT			0x000200
#define BUJEINFO__ALL				0xFFFFFF

#define OPTION_PAGE_AROAD_GEN			0
#define OPTION_PAGE_AROAD_GENERAL		1
#define OPTION_PAGE_AROAD_GENERALDIM	2
#define OPTION_PAGE_AROAD_DETAIL		3
#define OPTION_PAGE_AROAD_DETAILDIM		4

#define OPTION_PAGE_AROAD_SPLICE		5
#define OPTION_PAGE_AROAD_CROSS			6
#define OPTION_PAGE_AROAD_HBRACING		7
#define OPTION_PAGE_AROAD_VBRACING		8
#define OPTION_PAGE_AROAD_BM			9

#define OPTION_PAGE_AROAD_SHOE			10
#define OPTION_PAGE_AROAD_CAMBER		11
#define OPTION_PAGE_AROAD_ETCDETAIL		12
#define OPTION_PAGE_AROAD_SUMMARY		13
#define OPTION_PAGE_AROAD_PLANGEN		14
#define OPTION_PAGE_AROAD_LINEINFO		15
#define OPTION_PAGE_AROAD_PAINT			16
#define OPTION_PAGE_AROAD_SLABGEN		17
#define OPTION_PAGE_AROAD_TENDON		18
#define OPTION_PAGE_AROAD_DECKREBAR		19
#define OPTION_PAGE_AROAD_TU			20
#define OPTION_PAGE_AROAD_WELDMAP		21

class AFX_EXT_CLASS COptionStd  
{
public:
	COptionStd();
	virtual ~COptionStd();

public:
	virtual void Serialize(CArchive &ar);
	void InitOptionGeneral();	
	void InitOptionBM();

	long m_nOptionCur;

	// ��ü����	
	CString	m_sDomNumHead;
	CString	m_szDGKName;
	CString	m_szXRefPath;
	CString m_sTextBridgeDirection;
	CString m_sTextCounterDirection;
	BOOL	m_bTitleDogak;
	CString m_sHead_DomyunNumber;
	BOOL	m_bUsedHCad;
	
	CWordArray m_nDomPageArray;
	CWordArray m_nDomTitleArray;

	// �Ŵ� �Ϲݵ�
	BOOL	m_Gen_bDivPlanJong;	
	BOOL    m_Gen_bBujeThick;
	BOOL    m_Gen_bKeyPlanOneLine;
	BOOL	m_Gen_bKeyPlanRoVer;		
	BOOL    m_bMarkAlwaysInCircle;

	BOOL	m_Gen_bPlanView;
	BOOL	m_Gen_bJongView;
	BOOL	m_Gen_bFrontView;
	BOOL	m_Gen_bVerTitleFront;
	BOOL	m_Gen_bKeyPlan;
	BOOL    m_Gen_bLegend;
	BOOL    m_Gen_bBasicNote;
	BOOL    m_Gen_bForceText;
	BOOL    m_Gen_bForceTextLow;
	BOOL    m_Gen_bByundan;
	BOOL    m_Gen_bMatchLine;
	long	m_Gen_nPyungDim;
	long	m_Gen_nDivPage;
		
	long	m_Gen_nStyleDanmyunType;
	BOOL	m_Gen_bJongCenterGir;
	BOOL	m_Gen_bJongGenGir;
	long	m_Gen_nOutGirder;
	BOOL    m_Gen_bKeyPlanSpliceMark;
	BOOL    m_Gen_bKeyPlanJijumMark;
	BOOL    m_Gen_bKeyPlanCrossBeam;
	BOOL    m_Gen_bKeyPlanDimension;
	BOOL    m_Gen_bKeyPlanDirection;
	
	BOOL    m_Gen_bVerticalNote;
	BOOL    m_Gen_bJijumSymbol;
	BOOL    m_Gen_bJijumSimple;
	BOOL	m_Gen_bJongLevel;
	BOOL	m_Gen_bMarkCorss;
	BOOL	m_Gen_bMarkStringer;
	BOOL	m_Gen_bPyungJijumStation;
	BOOL	m_Gen_bPyungLineInfo;
	BOOL	m_Gen_bJongJijumStation;
	BOOL	m_Gen_bDimCenterGirder;	
	BOOL	m_Gen_bDimNormalGirder;	
	BOOL	m_Gen_bPyungAllHalf;
	
	long	m_Gen_nUnitEaDraw;	
	DWORD	m_Gen_BujeInfo;		// �����������
	DWORD	m_Gen_PyungBuje;
	DWORD	m_Gen_JongBuje;

	CDoubleArray m_Gen_Scale;

	//������ �Ϲݵ� ġ���� ===============================================================	
	__int64 m_GenDim_Plan_Upper; 
	__int64 m_GenDim_Plan_Lower; 
	__int64 m_GenDim_Jong_Upper; 
	__int64 m_GenDim_Jong_Lower; 	
	
	//�Ŵ� �󼼵� ======================================================================
	BOOL	m_Det_bPlanView;
	BOOL	m_Det_bJongView;	
	BOOL	m_Det_bKeyPlan;
	BOOL	m_Det_bKeyPlanSpliceMark;
	BOOL	m_Det_bKeyPlanJijumMark;
	BOOL	m_Det_bKeyPlanCrossBeam;
	BOOL	m_Det_bKeyPlanDimension;
	BOOL	m_Det_bKeyPlanDirection;
	BOOL	m_Det_bKeyPlanOneLine;
	BOOL	m_Det_bKeyPlanRoVer;	
	BOOL	m_Det_bWeldDetail;
	BOOL	m_Det_bFieldWeldDetail; //��������� �׸��� ����...
	BOOL	m_Det_bSolePlateDetail;
	BOOL	m_Det_bVStiffDetail;
	BOOL	m_Det_bHStiffDetail;	
	BOOL	m_Det_bLegend;
	BOOL	m_Det_bBasicNote;
	BOOL	m_Det_bBasicNoteVert;
	
	BOOL	m_Det_bGirderdan;
	BOOL	m_Det_bDiapHStiffPyung;	// �Ⱦ�
	BOOL	m_Det_bSttEndStationPyung;
	BOOL	m_Det_bPierAbutPyung;
	BOOL	m_Det_bLineInfoPyung;

	BOOL	m_Det_bLevelJong;	
	BOOL	m_Det_bSectionMarkJong;
	BOOL	m_Det_bHStiffJong;
	BOOL	m_Det_bSolePlateJong;
	BOOL	m_Det_bPierAbutJong;
	BOOL	m_Det_bVStiffLeftRight;

	long	m_Det_nDivPage;		
	BOOL	m_Det_bVStiffJewon;
	BOOL	m_Det_bVStiffAll;

	BOOL	m_Det_UpperWeldbGrind;
	BOOL	m_Det_LowerWeldbGrind;
		
	long	m_Det_nUnitEaDraw;
	long    m_Det_MaxSebuDetailSu;

	CDoubleArray m_Det_Scale;

	//������ �󼼵� ġ���� ===============================================================	
	__int64 m_DetailDim_Plan_Upper; 
	__int64 m_DetailDim_Plan_Lower; 
	__int64 m_DetailDim_Jong_Upper; 
	__int64 m_DetailDim_Jong_Lower; 

	// ���� �극�̽�
	BOOL m_HBracing_Summary;		
	BOOL m_HBracing_Pyung;			
	BOOL m_HBracing_Table;			
	BOOL m_HBracing_Plan;			
//	BOOL m_HBracing_Front;			
	BOOL m_HBracing_Detail;			
	BOOL m_HBracing_BM;				
	BOOL m_HBracing_AllGirder;		
	BOOL m_HBracing_Note;

	BOOL m_HBracing_bJiJumMark;		
	BOOL m_HBracing_bDimension;		
	BOOL m_HBracing_bDirection;		
	BOOL m_HBracing_bOneline;		
	BOOL m_HBracing_bLineInfo;	
	long m_HBracing_nRowSu;
	CDoubleArray m_HBracing_Scale;

	// �������� �󼼵�
	BOOL    m_SP_bTopView;
	BOOL    m_SP_bWebView;
	BOOL    m_SP_bBottomView;
	BOOL    m_SP_bTopRib;
	BOOL    m_SP_bBottomRib;
	BOOL    m_SP_bSangSeRib;
	BOOL	m_SP_bHort;			//�÷��� ���� ���
	BOOL    m_SP_bSpliceFull;   // ��ü�Ŵ� ���
	long	m_SP_nJewon;
	CDoubleArray m_SP_Scale;
	BOOL    m_SP_bAllGirder;

	// ��������ǥ
	DWORD	m_BM_OutSelect;		// ���ǥ ��� ����
	DWORD	m_BM_PaintSelect;	// ���ǥ ��� ����
	DWORD	m_BM_WeldSelect;	// ���ǥ ��� ����
	long	m_BM_TableSu;		// ���������� ���̺� ��
	long	m_BM_LineSu;		// �����̺�� ���� ��
	long	m_BM_Paint_TableSu;		// ���������� ���̺� ��
	long	m_BM_Paint_LineSu;		// �����̺�� ���� ��
	long	m_BM_Weld_TableSu;		// ���������� ���̺� ��
	long	m_BM_Weld_LineSu;		// �����̺�� ���� ��	
	long	m_nTypeJewonOut;
	
	// ����ġ��
	double	m_Shoe_dScaleHori;		// ����ġ�� ��������
	double	m_Shoe_dScaleVert;		// ����ġ�� ����������
	BOOL	m_Shoe_bShoeCoord;		// ����ǥ��
	BOOL	m_Shoe_bTableCoord;		// ����ǥ���̺�
	BOOL	m_Shoe_bTableLegend;	// ����
	BOOL	m_Shoe_bTableShoeBM;	// ������ǥ
	BOOL	m_Shoe_bCoordZ;			//3���� ��ǥ���
	BOOL	m_Shoe_bAngle;			//Angle ���
	BOOL	m_Shoe_bLineInfo;
	BOOL	m_Shoe_bCLSlabXY; 
	BOOL	m_Shoe_bCLSttEndXY;
	BOOL	m_Shoe_bSlabXY;	
	BOOL	m_Shoe_bSttEndXY;
	BOOL	m_Shoe_bSpanCenter;	 //������ħ��ġ�� ġ��
	BOOL	m_Shoe_bTotalCenter; //������ħ��ġ�� ġ��

	long	m_Shoe_dColumSu;
	double	m_Elev_dScaleSection;	// ELEVATION ����ٰŵ� �ܸ�
	double	m_Elev_dScaleShoeDetail;	// ELEVATION ����ٰŵ� ���¸��
	double	m_Elev_dScaleTable;		// ELEVATION ����ٰŵ� ���̺�
	BOOL	m_Elev_bSection;		// ELEVATION ����ٰŵ� �ܸ�
	BOOL	m_Elev_bShoeDetail;		// ELEVATION ����ٰŵ� ���¸��
	BOOL	m_Elev_bTable;			// ELEVATION ����ٰŵ� ���̺�

	//���κ� �󼼵� 
	BOOL	m_Cross_Summary;
	BOOL	m_Cross_Pyung;					//���鵵 ??
	BOOL	m_Cross_Table;					//���鵵 
	BOOL	m_Cross_Front;					//���鵵 
	BOOL	m_Cross_Plan;					//��鵵
	
	BOOL	m_Cross_PlanUpper;				//��鵵 ���
	BOOL	m_Cross_PlanLower;				//��鵵 �Ϻ�
	BOOL	m_Cross_Sangse;					//����� �� (����:�ܸ麰 Ÿ�� �󼼵�)

	BOOL	m_Cross_GassetUpperDetail;		//����� ���
	BOOL	m_Cross_GassetLowerDetail;		//����� �Ϻ�
	BOOL	m_Cross_GassetSupportDetail;	//����� ������
	BOOL	m_Cross_VStiffDetail;			//����������
	BOOL	m_Cross_HStiffDetail;			//���򺸰���
	BOOL    m_Cross_SpliceDetail;           //���� ������ ��
	BOOL	m_Cross_AnchorBarDetail;		//��Ŀ�� 
	BOOL	m_Cross_GassetScallopDetail;	//����� ��Ķ�� 	
	BOOL	m_Cross_BM;						//���ǥ 
	BOOL	m_Cross_BKSupport;				//����� ������ �� 	
	BOOL	m_Cross_DrawDiap;				//���̾����� ǥ�� 
	CDoubleArray m_CrossBeam_Scale;			//������ 0, 1�� ���κ� ��絵 ���� , ���� ������ 	

	BOOL	m_Cross_AllGirder;				//��ü �Ŵ� ���
	BOOL	m_Cross_TwoDrawing;				//�󼼵� 2�忡 ������ ��� 
	BOOL	m_Cross_KeyPlan;				//��൵ ���
	BOOL	m_Cross_KeyPlanRoVer;			//��൵ ����ȸ��
	BOOL	m_Cross_KeyPlanSpliceMark;
	BOOL	m_Cross_KeyPlanJijumMark;
	BOOL	m_Cross_KeyPlanCrossBeam;
	BOOL	m_Cross_KeyPlanDimension;
	BOOL	m_Cross_KeyPlanDirection;
	BOOL	m_Cross_KeyPlanOneLine;

	long    m_Cross_Type;                   //����Ÿ�� 0: ���κ��� 1: �ܸ麰
	
	//���κ� ġ��ǥ 
	BOOL	m_Cross_bSpliceMark;			//�������� ��ũ ǥ��
	BOOL	m_Cross_bJiJumMark;
	BOOL	m_Cross_bCrossBeam;
	BOOL	m_Cross_bDimension;
	BOOL	m_Cross_bDirection;
	BOOL	m_Cross_bOneline;
	BOOL	m_Cross_bLineInfo;	
	
	long	m_Cross_nRowSu;					//���̺� �ټ�
	double  m_Cross_dPosHScale;				//���κ�ġ��ǥ ���� ������
	double  m_Cross_dPosVScale;				//���κ�ġ��ǥ ���� ������

	//������
	long		m_nCamber_Type;
	long		m_nCamber_TableColSu;
	long		m_nCamber_DivType;
	long		m_nCamber_UnitEa;
	BOOL		m_bCamber_Table;
	BOOL		m_bCamber_AllGirder;
	BOOL		m_bCamber_AllGirderFull;
	BOOL		m_bCamber_Note;
	BOOL		m_bCamber_Legend;
	double		m_dCamber_HScale;
	double		m_dCamber_VScale;
	double		m_dCamber_TableHScale;

	//�������赵
	BOOL m_bWeldLeftGirder;			//�Ŵ���¿ɼ�
	BOOL m_bWeldCamberLevel;		//���鵵 ���� ����
	BOOL m_bWeldKeyPlan;			//Ű�÷� ��� �ɼ�
	BOOL m_bWeldKeyPlanSpliceMark;
	BOOL m_bWeldKeyPlanJijumMark;
	BOOL m_bWeldKeyPlanCrossBeam;
	BOOL m_bWeldKeyPlanDimension;
	BOOL m_bWeldKeyPlanDirection;
	BOOL m_bWeldKeyPlanOneLine;
	BOOL m_bWeldKeyPlanRoVer;	
	BOOL m_bWeldTypicalSection;		//TypicalSection ��� �ɼ�
	BOOL m_bWeldNote;				//��Ʈ ��� �ɼ�
	BOOL m_bWeldLineType;			//��Ÿ�� ��� �ɼ�
	BOOL m_bWeldMarkSystem;			//��ũ�ý��� ��� �ɼ�
	BOOL m_bWeldDivDomyun;			//���� ���ҹ�� �ɼ�
	BOOL m_bWeldDimText;			//ġ�� ���� ǥ�� �ɼ�
	long m_nWeldDanmyunScale;		//�ܸ鵵 ������ �ɼ�
	long m_nWeldSangseScale;			//������ ������ �ɼ�
	long m_nWeldTypicalScale;		//TypicalSection ������ �ɼ�
	long m_nWeldKeyPlanHor;			//Ű�÷� ���� ������ �ɼ�
	long m_nWeldKeyPlanVer;			//Ű�÷� ���� ������ �ɼ�
	long m_nWeldDivBlock;			//��� ����
	long m_nWeldDivDomyun;			//���� �������� ����	
	long m_nWeldMark;				// ���� ��ũ ��� �ɼ�


	//�ܸ��൵ ===============================================================
	long m_nSum_DivType;			// ���� ���� ���
	long m_nSum_DivNum;				// ��� ���� ����	
	double m_dSum_Scale;			// �ܸ��൵ ������
	double m_dSum_MomentY;
	double m_dSum_HeightY;
	double m_dSum_FlangeY;
	
	// ���� �󼼵�
	BOOL m_bPaint_GirGen;
	BOOL m_bPaint_GirGenFront;
	BOOL m_bPaint_TableBox;

	BOOL m_bPaint_Section;
	BOOL m_bPaint_SectionFront;
	BOOL m_bPaint_TableSection;

	BOOL m_bPaint_Splice;
	BOOL m_bPaint_SpliceUp;
	BOOL m_bPaint_SpliceWeb;
	BOOL m_bPaint_SpliceDown;
	BOOL m_bPaint_TableSplice;

	BOOL m_bPaint_Stringer;
	BOOL m_bPaint_StringerFront;
	BOOL m_bPaint_StringerPlanUp;
	BOOL m_bPaint_StringerPlanDown;
	BOOL m_bPaint_TableStringer;

	BOOL m_bPaint_CrossFront;
	BOOL m_bPaint_Cross;
	BOOL m_bPaint_CrossPlan;
	BOOL m_bPaint_TableCross;

	double m_dPaint_Scale_GirGen;
	double m_dPaint_Scale_Section;
	double m_dPaint_Scale_Splice;
	double m_dPaint_Scale_Stringer;
	double m_dPaint_Scale_Cross;

	//�����극�̽� �󼼵�
	BOOL m_VBracing_Summary;		
	BOOL m_VBracing_Pyung;			
	BOOL m_VBracing_Table;			
	BOOL m_VBracing_Plan;			
	BOOL m_VBracing_Front;			
	BOOL m_VBracing_Detail;			
	BOOL m_VBracing_AnchorBarDetail;
	BOOL m_VBracing_BM;				
	BOOL m_VBracing_AllGirder;		
	BOOL m_VBracing_KeyPlan;				//��൵ ���
	BOOL m_VBracing_KeyPlanRoVer;			//��൵ ����ȸ��
	BOOL m_VBracing_KeyPlanSpliceMark;
	BOOL m_VBracing_KeyPlanJijumMark;
	BOOL m_VBracing_KeyPlanCrossBeam;
	BOOL m_VBracing_KeyPlanDimension;
	BOOL m_VBracing_KeyPlanDirection;
	BOOL m_VBracing_KeyPlanOneLine;
	BOOL m_VBracing_Note;

	BOOL m_VBracing_bJiJumMark;		
	BOOL m_VBracing_bDimension;		
	BOOL m_VBracing_bDirection;		
	BOOL m_VBracing_bOneline;		
	BOOL m_VBracing_bLineInfo;	
	long m_VBracing_nRowSu;
	CDoubleArray m_VBracing_Scale;

	// �Ϲ� ��鵵
	BOOL	m_PlanGen_Plane_bIP			;
	BOOL	m_PlanGen_Plane_bChain		;
	BOOL	m_PlanGen_Plane_bCoord		;

	long	m_PlanGen_nDimPyung				;
	BOOL	m_PlanGen_bMarkDanPyung			;
	BOOL	m_PlanGen_bJijumInfoPyung		;
	BOOL	m_PlanGen_bJijumInfoPyung_Sta	;
	BOOL	m_PlanGen_bJijumInfoPyung_Mark	;
	BOOL	m_PlanGen_bPlaneInfoPyung		;

	double	m_PlanGen_Plane_dStart		;
	double	m_PlanGen_Plane_dEnd		;

	long	m_PlanGen_Plane_nUnitChain	;
	long	m_PlanGen_Plane_nUnitCoord	;

	// ����������
	BOOL   m_bLineInfo_PlanInfoTable;
	BOOL   m_bLineInfo_PlanInfoTable_IP;
	BOOL   m_bLineInfo_PlanGrid;
	BOOL   m_bLineInfo_JongDanLine;
	BOOL   m_bLineInfo_JongDanPlanInfoTable;
	BOOL   m_bLineInfo_PyungubaePlanChainUnit;
	BOOL   m_bLineInfo_PyungubaePlanLeft;
	BOOL   m_bLineInfo_PyungubaePlanRight;
	BOOL   m_bLineInfo_PyungubaePlanInfoLeft;
	BOOL   m_bLineInfo_PyungubaePlanInfoRight;
	BOOL   m_bLineInfo_PyungubaePlanLineInfo;
	BOOL   m_bLineInfo_PyungubaePlanInfoTable;

	double m_dLineInfo_PlaneScale;
	double m_dLineInfo_JongDanHScale;
	double m_dLineInfo_JongDanVScale;
	double m_dLineInfo_PyungubaeScale;
	double m_dLineInfo_PyungubaeRatio;

	// ������ �Ϲݵ�
	BOOL m_bSlab_PlanView;	// ��鵵
	BOOL m_bSlab_JongView;	// ���鵵
	BOOL m_bSlab_FrontView; // Ⱦ�ܸ鵵
	BOOL m_bSlab_FrontViewLeft;  // Ⱦ�ܸ鵵(����)
	BOOL m_bSlab_FrontViewRight; // Ⱦ�ܸ鵵(����)	
	BOOL m_bSlab_FrontDetView; // Ⱦ�ܸ鵵 ��
	BOOL m_bSlab_SectionAA;		// �Ŵ��� �ܸ� ��
	BOOL m_bSlab_SectionBB;		// �ڽ��� �ܸ� ��
	BOOL m_bSlab_SectionCC;		// �ڽ��� �߾Ӵܸ� ��
	BOOL m_bSlab_SectionDD;		// ���κ� �ܸ� ��
	BOOL m_bSlab_SectionEE;		// ��Ʈ���� ��
	BOOL m_bSlab_GuideWall;			// ��ȣ�� ��
	BOOL m_bSlab_Notch;			// NOTCH ��
	BOOL m_bSlab_KeyPlan;		// ��൵
	BOOL m_bSlab_bKeyPlanRoVer;	
	long m_nSlab_DivType;		// ���� ���� ���
	BOOL m_bSlab_UpperSlabLeftDim;	// ���̾�����(��)
	BOOL m_bSlab_UpperJiganLeftDim;	// ��� ��������(��)
	BOOL m_bSlab_LowerSlabRightDim;	// ���̾�����(��)
	BOOL m_bSlab_LowerJiganRightDim;	// �Ϻ� ��������(��)
	BOOL m_bSlab_MatchLine;		// ��ġ���� ǥ��
	BOOL m_bSlab_bPyungLineInfo;	//�������� 

	double m_dSlab_ScaleSlabGen;	// ������ �Ϲݵ� ������
	double m_dSlab_ScaleFrontDet;	// Ⱦ�ܸ鵵 �� ������
	double m_dSlab_ScaleSectionAA;	
	double m_dSlab_ScaleSectionBB;
	double m_dSlab_ScaleSectionCC;
	double m_dSlab_ScaleSectionDD;
	double m_dSlab_ScaleSectionEE;
	double m_dSlab_ScaleGuideWall;
	double m_dSlab_ScaleNothch;
	double m_dSlab_ScaleKeyPlanHor;
	double m_dSlab_ScaleKeyPlanVer;
	long m_nSlab_DivNum;			// ��� ���� ����

	BOOL m_bSlab_DimTable;					// ġ����ȭ ���̺�
	long m_nSlab_DimTableType;
	BOOL m_bSlab_DimTableFirstPage;
	BOOL m_bSlab_Pyungubae;
	double m_dSlab_DimTableScale;
	double m_dSlab_DimTableHeightFactor;
	double m_dSlab_DimTableWidhtFactor;

	BOOL    m_bSlab_KeyPlanSpliceMark;		// ��൵
	BOOL    m_bSlab_KeyPlanJijumMark;
	BOOL    m_bSlab_KeyPlanCrossBeam;
	BOOL    m_bSlab_KeyPlanDimension;
	BOOL    m_bSlab_KeyPlanDirection;
	BOOL    m_bSlab_KeyPlanOneLine;

	// Notch
	double	m_dSlab_NotchHeight;		// Notch ����	:	����
	double	m_dSlab_NotchTopWidth;		//				:	��(Top) 
	double	m_dSlab_NotchBotWidth;		//				:	��(Bottom)
	long	m_nSlab_NotchType;			// Notch Type	: 0-���簢��, 1-����

	// TENDON ��ġ��
	BOOL m_bTendon_PlanView;
	BOOL m_bTendon_SectionAA;
	long m_nTendon_DivType;
	BOOL m_bTendon_MatchLine;
	double m_dTendon_ScaleGen;
	double m_dTendon_ScaleSectionAA;
	long   m_nTendon_DivNum;

	// TurnOver �Ŵ� ��ٵ�
	BOOL m_bTU_Plan;			// ��鵵
	BOOL m_bTU_Jong;			// ���ܸ鵵
	BOOL m_bTU_Detail;			// ������ũ��Ʈ ��ٴܸ�
	BOOL m_bTU_DetailSteel;		// ö�� �󼼵�
	BOOL m_bTU_TableSteel;		// ö�� ���ǥ
	long m_nTU_Trim;			// 0-��ٵ� Trim 1-Trim����

	double m_dTUScale;
	double m_dTUScale_Plan;		// ��鵵 ������
	double m_dTUScale_Jong;		// ���ܸ鵵 ������
	double m_dTUScale_Detail;	// ������ũ��Ʈ ��ٴܸ� ������

	// ��鵵ġ�� ǥ���ɼ�
	long m_nSlab_PlanView_DimOpt;		// ��鵵 ġ�� Type : 0-�Ŵ��� ǥ��, 1-�Ŵ����� ǥ��
	BOOL m_bDeckRebar_PlanView;					// ��鵵
	BOOL m_bDeckRebar_JongView;					// ���鵵	
	BOOL m_bDeckRebar_FrontView;				// Ⱦ�ܸ鵵
	BOOL m_bDeckRebar_FrontViewCenter;			// Ⱦ�ܸ鵵(�߾Ӻ�)
	BOOL m_bDeckRebar_FrontViewJijum;			// Ⱦ�ܸ鵵(������)	
	BOOL m_bDeckRebar_MainRebarView;			// ��ö�� ������
	BOOL m_bDeckRebar_BelyckView;				// ���ö�� ������
	BOOL m_bDeckRebar_Sangse;					// �󼼵� (����Ʈ��)
	BOOL m_bDeckRebar_SangseDanbu;				//		������ �ܺ� �󼼵�
	BOOL m_bDeckRebar_SangseDanmyunAA;			//		�ܸ�A-A �󼼵�
	BOOL m_bDeckRebar_SangseWaterReinforce;		//		����� �����󼼵�
	BOOL m_bDeckRebar_SangseCentReinforce;		//		�߾������� ����ö�� �����󼼵�
	BOOL m_bDeckRebar_SangseDoubleChain;		//		ö�� ������ �󼼵�
	BOOL m_bDeckRebar_SangseA;					//		�󼼵�-A
	BOOL m_bDeckRebar_SangseChairBlock;			//		�󼼵�-����ö�� �� chair block��
	BOOL m_bDeckRebar_SangseShearReinforce;		//		���ܺ���ö�� ��
	BOOL m_bDeckRebar_SangseShearArray;			//		����ö�ٹ�ġ��
	BOOL m_bDeckRebar_SangseBlockOut;			//		Block Out ��
	BOOL m_bDeckRebar_SangseCrossBeam;			//		���κ� ��
	BOOL m_bDeckRebar_SangseStringer;			//		���κ� ��
	BOOL m_bDeckRebar_SangseRebar;				// ö�ٻ� (����Ʈ��)
	BOOL m_bDeckRebar_SangseMatrial;			//		ö�ٻ󼼵�
	BOOL m_bDeckRebar_SangseMatrialTable;		//		ö�ٻ�ǥ
	BOOL m_bDeckRebar_SangseGuard;				// ��ȣ�� �� (����Ʈ��)
	BOOL m_bDeckRebar_GuardSection;				//		��ȣ�� �ܸ鵵
	BOOL m_bDeckRebar_GuardDetail;				//		��ȣ�� ö�ٻ�
	BOOL m_bDeckRebar_GuardTable;				//		��ȣ�� ö�����̺� 

	double m_dDeck_ScaleRebarPlan;					// ������ ��鵵 ������
	double m_dDeck_ScaleRebarJong;					// ������ ���鵵 ������
	double m_dDeck_ScaleRebarFront;					// ������ Ⱦ�ܸ鵵 ������
	double m_dDeck_ScaleRebarSangseDanbu;			// ������ �󼼵�-�ܺλ󼼵� ������
	double m_dDeck_ScaleRebarSangseDanmyunAA;		// ������ �󼼵�-�ܸ�A-A ������
	double m_dDeck_ScaleRebarSangseWaterReinforce;	// ����� ���� �󼼵�
	double m_dDeck_ScaleRebarSangseCentReinforce;	// �߾������� ����ö�� ���� �󼼵�
	double m_dDEck_ScaleRebarSangseDoubleChain;		// ö�� ������ �� ������
	double m_dDeck_ScaleSangseA;					// ������ �󼼵�-A ������
	double m_dDeck_ScaleSangseChairBlock;			// ������ �󼼵�-����ö�� �� chair block�� ������
	double m_dDeck_SacleSangseShearReinforce;		// ���ܺ���ö�� ��
	double m_dDeck_ScaleSangseShearArray;			// ������ �󼼵�-����ö�� ��ġ�� ������
	double m_dDeck_ScaleSangseBlockOut;				// BLOCKOUT ��
	double m_dDeck_ScaleSangseCrossBeam;			// ���κ� ��
	double m_dDeck_ScaleSangseStringer;				// ���κ� ��
	double m_dDeck_ScaleRebarMain;					// ��ö�� ������ ������
	double m_dDeck_MainRebarCycle;					// ��ö�� ������ ����Ŭ��
	double m_dDeck_ScaleRebarBelyck;				// ���ö�� ������ ������
	double m_dDeck_BelyckCycle;						// ���ö�� ������ ����Ŭ��
	double m_dDeck_SpaceBetweenJewon;				// ö�� ��������
	double m_dDeck_DatasuOnRebarTable;				// ���̺�� �����ͼ�
	double m_dDeck_TypeRebarFrontLeft;				// ������ Ⱦ�ܸ鵵 Ÿ��(������ : 0, �Ϲݺ� : 1)
	double m_dDeck_TypeRebarFrontRigh;				// ������ Ⱦ�ܸ鵵 Ÿ��(������ : 0, �Ϲݺ� : 1)
	double m_dDeck_ScaleGuardRebar;					// ��ȣ��ö�� ������ 
	long   m_nDeck_JongDrawPos;						// ������ ���ܸ鵵��� ���� (�������߽� : 0, ���������� : 1, ��������� : 2)	
	long   m_nDeck_Slab_DivType;					// ���� ���� ���
	long   m_nDeck_Slab_RebarTableOpt;				// ö�����ǥ ǥ�����
	long   m_nTypeRebarTable;						// 0 : �⺻Ÿ��, 1 : ����ȭŸ��
	BOOL   m_bDeck_ShowJointMainRebar;				// �������庸��(��ö��)
	BOOL   m_bDeck_ShowJointBeryukRebar;			// �������庸��(���ö��)
	double m_dWidthRebarTableRect;					// ö�����ǥ ����ȭ Ÿ�Կ��� ö�ٻ� Rect�ϳ��� ��
	double m_dHeightRebarTableRect;					// ö�����ǥ ����ȭ Ÿ�Կ��� ö�ٻ� Rect�ϳ��� ����

};

#endif // !defined(AFX_OPTIONSTD_H__8B89B1AA_4BD1_4C99_95F8_1BDF997A69DA__INCLUDED_)


