// OptionStd.cpp: implementation of the COptionStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionStd::COptionStd()
{
	m_nOptionCur = 0; 

	InitOptionGeneral();
	InitOptionBM();
}

COptionStd::~COptionStd()
{

}

void COptionStd::InitOptionGeneral()
{
	// �Ϲݻ���
	m_nDomPageArray.RemoveAll();
	m_nDomTitleArray.RemoveAll();
	long i = 0;
	for(i=0;i < 18;i++)
	{
		m_nDomPageArray.Add(0);
		m_nDomTitleArray.Add(1);
	}
		
	m_sDomNumHead           =				  "";
	m_szDGKName             = "C:\\APlateWork\\DGK_Sample\\Default.dgk";
	m_szXRefPath			= "";
	m_sTextBridgeDirection  =			  _T("");
	m_sTextCounterDirection =			  _T("");
	m_bTitleDogak			=				TRUE;		//���� Ÿ��Ʋ �κ� ������ �ֱ� 
	m_sHead_DomyunNumber	=				  "";
	m_bUsedHCad				= TRUE;

	// ������ �Ϲݵ�
	m_Gen_bDivPlanJong			= FALSE;
	m_Gen_bBujeThick			=  TRUE;
	m_Gen_bKeyPlanOneLine		=  TRUE;
	m_Gen_bKeyPlanRoVer			= FALSE;
	m_bMarkAlwaysInCircle		=  TRUE;
	m_Gen_bPlanView				=  TRUE;
	m_Gen_bJongView				=  TRUE;
	m_Gen_bFrontView			=  TRUE;
	m_Gen_bKeyPlan				=  TRUE;
	m_Gen_bLegend				=  TRUE;
	m_Gen_bBasicNote			=  TRUE;
	m_Gen_bForceText			=  TRUE;
	m_Gen_bForceTextLow			=  TRUE;
	m_Gen_bByundan				=  TRUE;
	m_Gen_bVerTitleFront		=  TRUE;
	m_Gen_bMatchLine			=  TRUE;
	m_Gen_nPyungDim				=     1;
	m_Gen_nDivPage				=     0;
	m_Gen_nStyleDanmyunType		=     0;
	m_Gen_bJongCenterGir		= FALSE;
	m_Gen_bJongGenGir			= TRUE;
	m_Gen_nOutGirder			=     0;
	m_Gen_bKeyPlanSpliceMark	=  TRUE;
	m_Gen_bKeyPlanJijumMark		=  TRUE;
	m_Gen_bKeyPlanCrossBeam		=  TRUE;
	m_Gen_bKeyPlanDimension		=  TRUE;
	m_Gen_bKeyPlanDirection		=  TRUE;
	m_Gen_bJongLevel			=  TRUE;
	m_Gen_bVerticalNote			= FALSE;
	m_Gen_bJijumSymbol			=  TRUE;
	m_Gen_bJijumSimple			=  TRUE;
	m_Gen_bMarkCorss			= FALSE;
	m_Gen_bMarkStringer			= FALSE;
	m_Gen_bPyungJijumStation	= FALSE;
	m_Gen_bPyungLineInfo		=  TRUE;
	m_Gen_bJongJijumStation		= TRUE;
	m_Gen_bDimCenterGirder		= FALSE;		
	m_Gen_bDimNormalGirder		=  TRUE;	
	m_Gen_bPyungAllHalf			= FALSE;

	m_Gen_nUnitEaDraw   = 1;
	m_Gen_BujeInfo		= BUJEINFO__ALL;
	m_Gen_PyungBuje		= DRAW_PLAN_ALL;
	m_Gen_JongBuje		= DRAW_JONG_ALL;
	m_GenDim_Plan_Upper = DIM_BASE_JOINUPPER| DIM_BASE_SPLICE	| DIM_BASE_TOTAL;
	m_GenDim_Plan_Lower = DIM_BASE_JOINLOWER| DIM_BASE_DIAP		| DIM_BASE_SPLICE;
	m_GenDim_Jong_Upper = DIM_BASE_VSTIFF	| DIM_BASE_JOINWEB	| DIM_BASE_TOTAL;
	m_GenDim_Jong_Lower = DIM_BASE_BYUN		| DIM_BASE_SPAN;		

	// ������ �󼼵�
	m_Det_bPlanView				=  TRUE;
	m_Det_bJongView				=  TRUE;
	m_Det_bKeyPlan				=  TRUE;
	m_Det_bKeyPlanSpliceMark	=  TRUE;
	m_Det_bKeyPlanJijumMark		=  TRUE;
	m_Det_bKeyPlanCrossBeam		=  TRUE;
	m_Det_bKeyPlanDimension		=  TRUE;
	m_Det_bKeyPlanDirection		=  TRUE;
	m_Det_bKeyPlanOneLine		=  TRUE;
	m_Det_bKeyPlanRoVer			= FALSE;		
	m_Det_bWeldDetail			=  TRUE;
	m_Det_bFieldWeldDetail		=  TRUE;
	m_Det_bSolePlateDetail		=  TRUE;
	m_Det_bVStiffDetail			=  TRUE;
	m_Det_bHStiffDetail			=  TRUE;	
	m_Det_bLegend				=  TRUE;
	m_Det_bBasicNote			=  TRUE;
	m_Det_bBasicNoteVert		= FALSE;
	
	m_Det_bGirderdan			=  TRUE;
	m_Det_bSttEndStationPyung	=  TRUE;
	m_Det_bPierAbutPyung		= FALSE;
	m_Det_bLineInfoPyung		=  TRUE;

	m_Det_bLevelJong			=  TRUE;
	m_Det_bSectionMarkJong		=  TRUE;
	m_Det_bHStiffJong			=  TRUE;
	m_Shoe_bCLSlabXY			=  TRUE;	 //������ħ��ġ�� ��ǥ
	m_Det_bSolePlateJong		=  TRUE;
	m_Det_bPierAbutJong			= FALSE;
	m_Shoe_bCLSttEndXY			=  TRUE;	 //������ħ��ġ�� ��ǥ
	m_Det_bVStiffJewon	        =  TRUE;
	m_Det_bVStiffAll			= FALSE;

	m_Det_nDivPage = 2;//�Ŵ� �󼼵� ������� ��� ���κ� ������ ���ҷ�...�ʱⰪ ����...070129..KB..

	m_Shoe_bSpanCenter	= TRUE;	 //������ħ��ġ�� ġ��
	m_Shoe_bTotalCenter	= TRUE;	 //������ħ��ġ�� ġ��

	m_Det_Scale.Add( 30);
	m_Det_Scale.Add(500);
	m_Det_Scale.Add(500);
	m_Det_Scale.Add(  3);
	m_Det_Scale.Add( 20);
	m_Det_Scale.Add( 10);
	m_Det_Scale.Add( 10);
	m_Det_Scale.Add( 10);
	m_Det_Scale.Add( 30);

	m_Det_UpperWeldbGrind = TRUE;
	m_Det_LowerWeldbGrind = TRUE;

	m_Det_nUnitEaDraw			= 2;
	m_Det_MaxSebuDetailSu		= 0;	//0�ϰ�� ��� ���� 	

	m_DetailDim_Plan_Upper = DIM_BASE_FACTUPPER | DIM_BASE_SPLICE | DIM_BASE_TOTAL;
	m_DetailDim_Plan_Lower = DIM_BASE_DIAP      | DIM_BASE_FACTLOWER;
	m_DetailDim_Jong_Upper = DIM_BASE_WELDSTUD  | DIM_BASE_VSTIFF | DIM_BASE_FACTWEB | DIM_BASE_TOTAL;
	m_DetailDim_Jong_Lower = DIM_BASE_BYUN; 

	m_Gen_Scale.Add(100);
	m_Gen_Scale.Add(50);
	m_Gen_Scale.Add(800);
	m_Gen_Scale.Add(800);	

	// ���� �극�̽�
	m_HBracing_Summary		= TRUE;
	m_HBracing_Pyung		= TRUE;
	m_HBracing_Table		= TRUE;
	m_HBracing_Plan			= TRUE;
//	m_HBracing_Front		= TRUE;
	m_HBracing_Detail		= TRUE;
	m_HBracing_BM			= TRUE;
	m_HBracing_Note			= TRUE;

	m_HBracing_bJiJumMark	= TRUE;
	m_HBracing_bDimension	= TRUE;
	m_HBracing_bDirection	= TRUE;
	m_HBracing_bOneline		= TRUE;
	m_HBracing_bLineInfo	= TRUE;
	m_HBracing_nRowSu		= TRUE;
	
	m_HBracing_Scale.Add(50);
	m_HBracing_Scale.Add(10);
	m_HBracing_Scale.Add(500);
	m_HBracing_Scale.Add(500);


// �������� �󼼵�
	m_SP_bTopView    =  TRUE;
	m_SP_bWebView    =  TRUE;
	m_SP_bBottomView =  TRUE;
//	m_SP_bTopRib     =  TRUE;
//	m_SP_bBottomRib  =  TRUE;
//	m_SP_bSangSeRib  =  TRUE;
//	m_SP_bHort       = FALSE;
//	m_SP_bSpliceFull = FALSE;
//	m_SP_bOutBM	     =  TRUE;
	m_SP_nJewon      =     0;
	m_SP_bAllGirder	 = TRUE;

	for(i=0; i<10; i++)
	{
		if(i>=0 && i<=2)
			m_SP_Scale.Add(20);
//		else if(i >=3 && i<=4)
//			m_SP_Scale.Add(10);
//		else
//			m_SP_Scale.Add(5);
	}

	// ���κ� 
	m_Cross_Summary				=  TRUE;	//��絵
	m_Cross_Pyung				=  TRUE;	//��絵
	m_Cross_Table				=  TRUE;	//ġ�����̺�
	m_Cross_Front				=  TRUE;	//���鵵
	m_Cross_Plan				=  TRUE;	//��鵵

	m_Cross_PlanUpper			=  TRUE;	//��鵵 ���
	m_Cross_PlanLower			=  TRUE;	//��鵵 �Ϻ�
	m_Cross_Sangse				=  TRUE;	//�ܸ麰 Ÿ�� �󼼵�
	
	m_Cross_GassetUpperDetail	=  TRUE;	//����� ���
	m_Cross_GassetLowerDetail	=  TRUE;	//����� �Ϻ�
	m_Cross_GassetSupportDetail	=  TRUE;	//����� ������
	m_Cross_VStiffDetail		=  TRUE;	//����������
	m_Cross_HStiffDetail		=  TRUE;	//���򺸰���
	m_Cross_AnchorBarDetail		=  TRUE;	//��Ŀ��
	m_Cross_GassetScallopDetail	=  TRUE;	//����� ��Ķ��
	m_Cross_BM					=  TRUE;	//���ǥ
	m_Cross_BKSupport			=  TRUE;	//����� ������ ��
	m_Cross_SpliceDetail        =  TRUE; 
	m_Cross_nRowSu				=    20;
	m_Cross_dPosHScale			=   200;
	m_Cross_dPosVScale			=   200;

	m_Shoe_bSttEndXY			=  TRUE;	//������ħ��ġ�� ��ǥ ǥ�� ����
//	m_Cross_DrawDiap			=  TRUE;	//���̾����� ǥ��
	m_Shoe_bSlabXY				=  TRUE;	//������ħ��ġ�� ��ǥ ǥ�� ����

	m_Cross_AllGirder			= FALSE;	//��ü �Ŵ� ���
	m_Cross_TwoDrawing			= FALSE;	//�󼼵� 2�忡 ������ ���
	m_Cross_KeyPlan				= TRUE;		//��൵ ���
	m_Cross_KeyPlanRoVer		= FALSE;	//��൵ ����ȸ��
	m_Cross_KeyPlanSpliceMark	= TRUE;
	m_Cross_KeyPlanJijumMark	= TRUE;
	m_Cross_KeyPlanCrossBeam	= TRUE;
	m_Cross_KeyPlanDimension	= TRUE;
	m_Cross_KeyPlanDirection	= TRUE;
	m_Cross_KeyPlanOneLine		= TRUE;
	m_Cross_Type                = 1;        //�ܸ麰 ���

	//���κ� ġ��ǥ 
	m_Cross_bSpliceMark			=  TRUE;
	m_Cross_bJiJumMark			=  TRUE;
	m_Cross_bCrossBeam			=  TRUE;
	m_Cross_bDimension			=  TRUE;
	m_Cross_bDirection			=  TRUE;
	m_Cross_bOneline			=  TRUE;
	m_Cross_bLineInfo			=  TRUE;

	m_CrossBeam_Scale.Add(30);	// ���鵵 
	//
	m_CrossBeam_Scale.Add(30);	// //��鵵 ���
	m_CrossBeam_Scale.Add(30);	// //��鵵 �Ϻ�
	m_CrossBeam_Scale.Add(10);	// //�ܸ麰 Ÿ�� �󼼵�
	m_CrossBeam_Scale.Add(10);	// //���������� ��
	m_CrossBeam_Scale.Add(5);	// �ܸ麰 Ÿ�� ����� ��
	//
	m_CrossBeam_Scale.Add(10);	// ����� ���
	m_CrossBeam_Scale.Add(10);	// ����� �Ϻ�
	m_CrossBeam_Scale.Add(10);	// ����� ������
	m_CrossBeam_Scale.Add(10);	// ����������
	m_CrossBeam_Scale.Add(10);	// ���򺸰���
	m_CrossBeam_Scale.Add(10);	// ��Ŀ�� 
	m_CrossBeam_Scale.Add(10);	// ����� ������ �� 
	m_CrossBeam_Scale.Add(10);	// ����� ���̷�
	m_CrossBeam_Scale.Add(500);	// ��൵ ����
	m_CrossBeam_Scale.Add(500);	// ��൵ ����	

	// ����ġ��
	m_Shoe_dScaleHori		=  300;
	m_Shoe_dScaleVert		=  300;
	m_Shoe_bShoeCoord		= TRUE;
	m_Shoe_bTableCoord		= TRUE;
	m_Shoe_bTableLegend		= TRUE;
	m_Shoe_bTableShoeBM		= TRUE;
	m_Elev_dScaleSection	=   50;
	m_Elev_dScaleShoeDetail	=   10;
	m_Elev_dScaleTable		=  100;
	
	m_Elev_bSection		=  TRUE;
	m_Elev_bShoeDetail	=  TRUE;
	m_Elev_bTable		=  TRUE;
	m_Shoe_bCoordZ		=  TRUE;
	m_Shoe_bAngle		=  TRUE;
	m_Shoe_bLineInfo	= FALSE;
	m_Shoe_dColumSu		=     4;

	//������;
	m_nCamber_Type			=    0;
	m_nCamber_TableColSu	=   30;
	m_nCamber_DivType		=    1;
	m_nCamber_UnitEa		=    1;
	m_bCamber_Table			= TRUE;
	m_bCamber_AllGirder		= TRUE;
	m_bCamber_AllGirderFull	= FALSE;
	m_bCamber_Note			= FALSE;
	m_bCamber_Legend		= TRUE;
	m_dCamber_HScale		=  200;
	m_dCamber_VScale		=  200;
	m_dCamber_TableHScale	=    1;

	// ����
	m_bWeldLeftGirder			=  FALSE;
	m_bWeldCamberLevel			=  TRUE;
	m_bWeldKeyPlan				=  TRUE;
	m_bWeldKeyPlanSpliceMark	= FALSE;
	m_bWeldKeyPlanJijumMark		=  TRUE;
	m_bWeldKeyPlanCrossBeam		=  TRUE;
	m_bWeldKeyPlanDimension		= FALSE;
	m_bWeldKeyPlanDirection		=  TRUE;
	m_bWeldKeyPlanOneLine		= FALSE;
	m_bWeldKeyPlanRoVer			= FALSE; 	

	m_bWeldTypicalSection	= TRUE;
	m_bWeldNote				= TRUE;
	m_bWeldLineType			= TRUE;
	m_bWeldMarkSystem		= TRUE;
	m_bWeldDivDomyun		= TRUE;
	m_bWeldDimText			= TRUE;
	m_nWeldDanmyunScale		=  100;
	m_nWeldSangseScale		=    3;
	m_nWeldTypicalScale		=   20;
	m_nWeldKeyPlanHor		=  500;
	m_nWeldKeyPlanVer		=  500;
	m_nWeldDivBlock			=    3;
	m_nWeldDivDomyun		=    4;
	m_nWeldMark				=    0;

	//�����극�̽� �󼼵�
	m_VBracing_Summary			= TRUE; 		
	m_VBracing_Pyung			= TRUE; 		
	m_VBracing_Table			= TRUE; 		
	m_VBracing_Plan				= TRUE; 			
	m_VBracing_Front			= TRUE; 		
	m_VBracing_Detail			= TRUE; 
	m_VBracing_AnchorBarDetail	= TRUE; 
	m_VBracing_BM				= TRUE; 				
	m_VBracing_AllGirder		= FALSE;
	m_VBracing_KeyPlan			= TRUE; 			//��൵ ���
	m_VBracing_KeyPlanRoVer		= FALSE;			//��൵ ����ȸ��
	m_VBracing_KeyPlanSpliceMark= TRUE; 
	m_VBracing_KeyPlanJijumMark = TRUE; 
	m_VBracing_KeyPlanCrossBeam = TRUE; 
	m_VBracing_KeyPlanDimension = TRUE; 
	m_VBracing_KeyPlanDirection = TRUE; 
	m_VBracing_KeyPlanOneLine   = TRUE; 
	m_VBracing_Note				= FALSE;

	m_VBracing_bJiJumMark		= TRUE; 		
	m_VBracing_bDimension		= TRUE; 	
	m_VBracing_bDirection		= TRUE; 		
	m_VBracing_bOneline			= TRUE; 		
	m_VBracing_bLineInfo		= TRUE; 	

	m_VBracing_Scale.Add(300);
	m_VBracing_Scale.Add(300);
	m_VBracing_Scale.Add(25);// ���鵵 
	m_VBracing_Scale.Add(25);// ��鵵
	m_VBracing_Scale.Add(10);// �����극�̽� ��
	m_VBracing_Scale.Add(10);// ��Ŀ�� 
	m_VBracing_Scale.Add(500);// ��൵ ���� 
	m_VBracing_Scale.Add(500);// ��൵ ����
	m_VBracing_nRowSu			= 20;

	// �Ϲ� ��鵵
	m_PlanGen_Plane_bIP			= TRUE;
	m_PlanGen_Plane_bChain		= TRUE;
	m_PlanGen_Plane_bCoord		= TRUE;

	m_PlanGen_nDimPyung				=    0;
	m_PlanGen_bMarkDanPyung			= TRUE;
	m_PlanGen_bJijumInfoPyung		= TRUE;
	m_PlanGen_bJijumInfoPyung_Sta	= TRUE;
	m_PlanGen_bJijumInfoPyung_Mark	= TRUE;
	m_PlanGen_bPlaneInfoPyung		= TRUE;

	m_PlanGen_Plane_dStart		=  0.0;
	m_PlanGen_Plane_dEnd		=  0.0;

	m_PlanGen_Plane_nUnitChain	=  100;
	m_PlanGen_Plane_nUnitCoord	=  100;

	// ������ȹ ������ : ���� ����
	m_bLineInfo_PlanInfoTable          = TRUE;
	m_bLineInfo_PlanInfoTable_IP       = TRUE;
	m_bLineInfo_PlanGrid               = TRUE;
	m_bLineInfo_JongDanLine            = TRUE;
	m_bLineInfo_JongDanPlanInfoTable   = TRUE;
	m_bLineInfo_PyungubaePlanChainUnit = TRUE;
	m_bLineInfo_PyungubaePlanLeft      = TRUE;
	m_bLineInfo_PyungubaePlanRight     = TRUE;
	m_bLineInfo_PyungubaePlanInfoLeft  = TRUE;
	m_bLineInfo_PyungubaePlanInfoRight = TRUE;
	m_bLineInfo_PyungubaePlanLineInfo  = TRUE;
	m_bLineInfo_PyungubaePlanInfoTable = TRUE;

	//                 : SCALE
	m_dLineInfo_PlaneScale      = 1200;
	m_dLineInfo_JongDanHScale   = 1200;
	m_dLineInfo_JongDanVScale   =  200;
	m_dLineInfo_PyungubaeScale  = 1200;
	m_dLineInfo_PyungubaeRatio  =  600;

	//�ܸ��൵
	m_nSum_DivType	=   3;	// ���� ���� ���(default 3: ��ü������ �� �忡)
	m_nSum_DivNum	=   1;	// ��� ���� ����	
	m_dSum_Scale	= 300;	// �ܸ��൵ ������
	m_dSum_MomentY	=   3;
	m_dSum_HeightY	= 1.8;
	m_dSum_FlangeY	=  25;

	// ���� �󼼵�
	m_bPaint_GirGen				= TRUE;
	m_bPaint_GirGenFront		= TRUE;
	m_bPaint_TableBox			= TRUE;

	m_bPaint_Section			= TRUE;
	m_bPaint_SectionFront		= TRUE;
	m_bPaint_TableSection		= TRUE;
	
	m_bPaint_Splice				= TRUE;
	m_bPaint_SpliceUp			= TRUE;
	m_bPaint_SpliceWeb			= TRUE;
	m_bPaint_SpliceDown			= TRUE;
	m_bPaint_TableSplice		= TRUE;

	m_bPaint_Stringer			= TRUE;
	m_bPaint_StringerFront		= TRUE;
	m_bPaint_StringerPlanUp		= TRUE;
	m_bPaint_StringerPlanDown	= TRUE;
	m_bPaint_TableStringer		= TRUE;

	m_bPaint_Cross				= TRUE;
	m_bPaint_CrossFront			= TRUE;
	m_bPaint_CrossPlan			= TRUE;
	m_bPaint_TableCross			= TRUE;

	m_dPaint_Scale_GirGen	= 20;
	m_dPaint_Scale_Section	= 20;
	m_dPaint_Scale_Splice	= 20;
	m_dPaint_Scale_Stringer	= 20;
	m_dPaint_Scale_Cross	= 20;

	// ������ �Ϲݵ�
	m_bSlab_PlanView			=  TRUE;	// ��鵵
	m_bSlab_JongView			=  TRUE;	// ���鵵
	m_bSlab_FrontView			=  TRUE;	// Ⱦ�ܸ鵵
	m_bSlab_FrontViewLeft		=  TRUE;	// Ⱦ�ܸ絵(����)
	m_bSlab_FrontViewRight		= FALSE;	// Ⱦ�ܸ鵵(����)
	m_bSlab_FrontDetView		=  TRUE;	// Ⱦ�ܸ鵵 ��
	m_bSlab_SectionAA			=  TRUE;	// �Ŵ��� �ܸ� ��
	m_bSlab_SectionBB			=  TRUE;	// �ڽ��� �ܸ� ��
	m_bSlab_SectionCC			=  TRUE;	// �ڽ��� �߾Ӵܸ� ��
	m_bSlab_SectionDD			=  TRUE;	// ���κ� �ܸ� ��
	m_bSlab_SectionEE			=  TRUE;	// ��Ʈ���� ��
	m_bSlab_GuideWall			=  TRUE;	// ��ȣ�� ��
	m_bSlab_Notch				=  TRUE;	// NOTCH ��
	m_bSlab_KeyPlan				=  TRUE;		// ��൵
	m_bSlab_bKeyPlanRoVer		= FALSE;	
	m_nSlab_DivType				=     0;		// ���� ���� ���
	m_bSlab_UpperSlabLeftDim	=  TRUE;	// ���̾�����(��)
	m_bSlab_UpperJiganLeftDim	=  TRUE;	// ��� ��������(��)
	m_bSlab_LowerSlabRightDim	=  TRUE;	// ���̾�����(��)
	m_bSlab_LowerJiganRightDim	=  TRUE;	// �Ϻ� ��������(��)
	m_bSlab_MatchLine			=  TRUE;
	m_bSlab_bPyungLineInfo		=  TRUE;	//�������� 

	m_dSlab_ScaleSlabGen	= 100;	// ������ �Ϲݵ� ������
	m_dSlab_ScaleFrontDet	=  50;	// Ⱦ�ܸ鵵 �� ������
	m_dSlab_ScaleSectionAA	=  30;	
	m_dSlab_ScaleSectionBB	=  30;
	m_dSlab_ScaleSectionCC	=  30;
	m_dSlab_ScaleSectionDD	=  30;
	m_dSlab_ScaleSectionEE	=  30;
	m_dSlab_ScaleGuideWall	=  30;
	m_dSlab_ScaleNothch		=   2;
	m_dSlab_ScaleKeyPlanHor	= 800;
	m_dSlab_ScaleKeyPlanVer	= 800;
	m_nSlab_DivNum			=   1;

	m_bSlab_DimTable				=  TRUE;
	m_nSlab_DimTableType			=     1;
	m_bSlab_DimTableFirstPage		= FALSE;
	m_bSlab_Pyungubae				=  TRUE;
	m_dSlab_DimTableScale			=   100;
	m_dSlab_DimTableHeightFactor	=     1;
	m_dSlab_DimTableWidhtFactor		=     1;

	m_bSlab_KeyPlanSpliceMark	= FALSE;
	m_bSlab_KeyPlanJijumMark	=  TRUE;
	m_bSlab_KeyPlanCrossBeam	=  TRUE;
	m_bSlab_KeyPlanDimension	= FALSE;
	m_bSlab_KeyPlanDirection	=  TRUE;
	m_bSlab_KeyPlanOneLine		= FALSE;

	m_dSlab_NotchHeight			= 20;	
	m_dSlab_NotchTopWidth		= 30;	
	m_dSlab_NotchBotWidth		= 30;	
	m_nSlab_NotchType			= 0;

	// TENDON ��ġ��
	m_bTendon_PlanView       = TRUE;
	m_bTendon_SectionAA      = TRUE;
	m_nTendon_DivType        = 0;
	m_bTendon_MatchLine      = TRUE;
	m_dTendon_ScaleGen       = 100;
	m_dTendon_ScaleSectionAA = 30;
	m_nTendon_DivNum         = 1;

	m_nSlab_PlanView_DimOpt	 = 0;

	//������ ��ٵ�
	m_bDeckRebar_PlanView				= TRUE;			// ��鵵
	m_bDeckRebar_JongView				= TRUE;			// ���鵵	
	m_bDeckRebar_FrontView				= TRUE;			// Ⱦ�ܸ鵵
	m_bDeckRebar_FrontViewCenter		= TRUE;			// Ⱦ�ܸ鵵(�߾Ӻ�)
	m_bDeckRebar_FrontViewJijum			= TRUE;			// Ⱦ�ܸ鵵(������)	
	m_bDeckRebar_MainRebarView			= TRUE;			// ��ö�� ������
	m_bDeckRebar_BelyckView				= TRUE;			// ���ö�� ������
	m_bDeckRebar_Sangse					= TRUE;			// �󼼵� (����Ʈ��)
	m_bDeckRebar_SangseDanbu			= TRUE;			//		������ �ܺ� �󼼵�
	m_bDeckRebar_SangseDanmyunAA		= TRUE;			//		�ܸ�A-A �󼼵�
	m_bDeckRebar_SangseWaterReinforce	= TRUE;			//		����� �����󼼵�
	m_bDeckRebar_SangseCentReinforce	= TRUE;			//		�߾������� ����ö�� �����󼼵�
	m_bDeckRebar_SangseDoubleChain		= TRUE;			//		ö�� ������ �󼼵�
	m_bDeckRebar_SangseA				= TRUE;			//		�󼼵�-A
	m_bDeckRebar_SangseChairBlock		= TRUE;			//		�󼼵�-����ö�� �� chair block��
	m_bDeckRebar_SangseShearReinforce	= TRUE;			//		���ܺ���ö�� ��
	m_bDeckRebar_SangseShearArray		= TRUE;			//		����ö�ٹ�ġ��
	m_bDeckRebar_SangseBlockOut			= TRUE;			//		Block Out ��
	m_bDeckRebar_SangseCrossBeam		= TRUE;			//		���κ� ��
	m_bDeckRebar_SangseStringer			= TRUE;			//		���κ� ��
	
	m_bDeckRebar_SangseGuard			= TRUE;			// ��ȣ�� ��	(����Ʈ��)
	m_bDeckRebar_GuardSection			= TRUE;			//		��ȣ�� �ܸ鵵
	m_bDeckRebar_GuardDetail			= TRUE;			//		��ȣ�� �󼼵�
	m_bDeckRebar_GuardTable				= TRUE;			//		��ȣ�� ���ǥ

	m_bDeckRebar_SangseRebar			= TRUE;			// ö�ٻ� (����Ʈ��)
	m_bDeckRebar_SangseMatrial			= TRUE;			//		ö�ٻ󼼵�
	m_bDeckRebar_SangseMatrialTable		= TRUE;			//		ö�ٻ�ǥ

	m_dDeck_ScaleRebarPlan					= 50;			// ������ ��鵵 ������
	m_dDeck_ScaleRebarJong					= 50;			// ������ ���鵵 ������
	m_dDeck_ScaleRebarFront					= 50;			// ������ Ⱦ�ܸ鵵 ������
	m_dDeck_ScaleRebarSangseDanbu			= 30;			// ������ �󼼵�-�ܺλ󼼵� ������
	m_dDeck_ScaleRebarSangseDanmyunAA		= 30;			// ������ �󼼵�-�ܸ�A-A ������
	m_dDeck_ScaleRebarSangseWaterReinforce	= 20;			// ����� ���� �󼼵�
	m_dDeck_ScaleRebarSangseCentReinforce	= 80;			// �߾������� ����ö�� ���� �󼼵�
	m_dDEck_ScaleRebarSangseDoubleChain		= 80;			// ö�� ������ �� ������
	m_dDeck_ScaleSangseA					= 15;			// ������ �󼼵�-A ������
	m_dDeck_ScaleSangseChairBlock			= 80;			// ������ �󼼵�-����ö�� �� chair block�� ������
	m_dDeck_SacleSangseShearReinforce		= 50;			// ���ܺ���ö�� ��
	m_dDeck_ScaleSangseShearArray			= 20;			// ������ �󼼵�-����ö�� ��ġ�� ������
	m_dDeck_ScaleSangseBlockOut				= 80;			// BLOCKOUT ��
	m_dDeck_ScaleSangseCrossBeam			= 20;			// ���κ� ��
	m_dDeck_ScaleSangseStringer				= 20;			// ���κ� ��
	m_dDeck_ScaleRebarMain					= 50;			// ��ö�� ������ ������
	m_dDeck_MainRebarCycle					= 3;			// ��ö�� ������ ����Ŭ��
	m_dDeck_ScaleRebarBelyck				= 30;			// ���ö�� ������ ������
	m_dDeck_BelyckCycle						= 3;			// ���ö�� ������ ����Ŭ��
	m_dDeck_SpaceBetweenJewon				= 1000;			// ö�� ��������
	m_dDeck_DatasuOnRebarTable				= 70;			// ���̺�� �����ͼ�
	m_dDeck_TypeRebarFrontLeft				= 0;			// ������ Ⱦ�ܸ鵵 �����ġ (������)
	m_dDeck_TypeRebarFrontRigh				= 2;			// ������ Ⱦ�ܸ鵵 �����ġ (����)
	m_dDeck_ScaleGuardRebar					= 10;			// ��ȣ�� ������ 
	m_nDeck_JongDrawPos						= 0;			// ������ ���ܸ鵵 ��� ����	
	m_nDeck_Slab_DivType					= 0;			// �氣�� ����
	m_nDeck_Slab_RebarTableOpt				= 0;			// ö�����ǥ ��ü�� ǥ��
	m_bDeck_ShowJointMainRebar				= FALSE;
	m_bDeck_ShowJointBeryukRebar			= FALSE;
	m_nTypeRebarTable						= FALSE;		//ö�����ǥ Ÿ��(0:�Ϲ�, 1:����ȭ)
	m_dWidthRebarTableRect					= 4000;			//ö�ٻ� Rect ��
	m_dHeightRebarTableRect					= 4000;			//ö�ٻ� Rect ����
	m_bTU_Plan			= TRUE;		// ��鵵
	m_bTU_Jong			= TRUE;		// ���ܸ鵵
	m_bTU_Detail		= TRUE;		// ������ũ��Ʈ ��ٴܸ�
	m_bTU_DetailSteel	= TRUE;		// ö�� �󼼵�
	m_bTU_TableSteel	= TRUE;		// ö�� ���ǥ
	m_nTU_Trim			= 0;		// ��� Trim

	m_dTUScale			= 50;
	m_dTUScale_Plan		= 50;		// ��鵵 ������
	m_dTUScale_Jong		= 50;		// ���ܸ鵵 ������
	m_dTUScale_Detail	= 20;		// ������ũ��Ʈ ��ٴܸ� ������
}

void COptionStd::InitOptionBM()
{
	m_BM_OutSelect		= 0xFFFFFFFF;
	m_BM_PaintSelect	= 0xFFFFFFFF;
	m_BM_WeldSelect		= 0xFFFFFFFF;	
	m_BM_TableSu		=		   3;	// ���������� ���̺� ��
	m_BM_LineSu			=		  50;	// �����̺�� ���� ��
	m_BM_Paint_TableSu	=		   2;	// ���������� ���̺� ��
	m_BM_Paint_LineSu	=		  50;	// �����̺�� ���� ��
	m_BM_Weld_TableSu	=		   2;	// ���������� ���̺� ��
	m_BM_Weld_LineSu	=		  50;	// �����̺�� ���� ��	
	m_nTypeJewonOut		=		   0;
}

void COptionStd::Serialize(CArchive &ar)
{
//	long nFlag = 4;		
//	long nFlag = 6;		// 2006.11.14
//	long nFlag = 8;		// 2006.12.07
//	long nFlag = 9;		// 2007.01.16 ......... ��൵ �߰�...KB...
//	long nFlag = 10;	// 2007.01.17 ......... ���κ���ġ��ǥ �߰�...KB...
//	long nFlag = 11;	// 2007.04.03 ......... ���κ� �󼼵� 2�忡 ������ �ⷰ...KB...
//	long nFlag = 12;	// 2008.02.01 ......... TU�Ŵ� ��ٵ� �߰�...Lina...
//	long nFlag = 13;	// 2008.02.04 ......... TU�Ŵ� ��ٵ� Trim�ɼ� �߰�...Lina...
//	long nFlag = 14;	// 2008.05.07 ......... ������ ���̺� ���������ɼ� �߰�...Lina
//	long nFlag = 15;	// 2008.08.14 ......... HCad Block ���ɼ� �߰�...Lina
//	long nFlag = 16;	// 2008.12.22 ......... TU�Ŵ� ���������� �ɼ� �߰�...Lina
//	long nFlag = 17;	// 2009.07.13 ......... ���������� ��(��)ǥ�� �߰�..LSH
    long nFlag = 18;    // 2013.01.18 ......... ���鰣��ȭŸ�� �߰�..JJKim

	double dValueTemp = 0;
	if (ar.IsStoring())
	{
		ar << nFlag;

		ar << m_sDomNumHead;
		ar << m_szDGKName;
		ar << m_szXRefPath;
		ar << m_sTextBridgeDirection;
		ar << m_sTextCounterDirection;
		ar << m_bTitleDogak;
		ar << m_sHead_DomyunNumber;
		ar << m_bUsedHCad;
	
		m_nDomPageArray.Serialize(ar);
		m_nDomTitleArray.Serialize(ar);
		
		// �Ŵ� �Ϲݵ�
		ar << m_Gen_bDivPlanJong;	
		ar << m_Gen_bBujeThick;
		ar << m_Gen_bKeyPlanOneLine;
		ar << m_Gen_bKeyPlanRoVer;		

		ar << m_Gen_bPlanView;
		ar << m_Gen_bJongView;
		ar << m_Gen_bFrontView;
		ar << m_Gen_bVerTitleFront;
		ar << m_Gen_bKeyPlan;
		ar << m_Gen_bLegend;
		ar << m_Gen_bBasicNote;
		ar << m_Gen_bForceText;
		ar << m_Gen_bForceTextLow;
		ar << m_Gen_bByundan;
		ar << m_Gen_bMatchLine;
		ar << m_Gen_nPyungDim;
		ar << m_Gen_nDivPage;
		ar << m_Gen_nStyleDanmyunType;
			
		ar << m_Gen_bJongCenterGir;
		ar << m_Gen_bJongGenGir;
		ar << m_Gen_nOutGirder;
		ar << m_Gen_bKeyPlanSpliceMark;
		ar << m_Gen_bKeyPlanJijumMark;
		ar << m_Gen_bKeyPlanCrossBeam;
		ar << m_Gen_bKeyPlanDimension;
		ar << m_Gen_bKeyPlanDirection;
		
		ar << m_Gen_bJijumSymbol;
		ar << m_Gen_bJijumSimple;
		ar << m_Gen_bMarkCorss;
		ar << m_Gen_bMarkStringer;
		ar << m_Gen_bJongLevel;
		ar << m_Gen_bVerticalNote;

		ar << m_Gen_bPyungJijumStation;
		ar << m_Gen_bPyungLineInfo;
		ar << m_Gen_bJongJijumStation;
		ar << m_Gen_bDimCenterGirder;	
		ar << m_Gen_bDimNormalGirder;			
		ar << m_Gen_nUnitEaDraw;	
		ar << m_Gen_Scale;

		// ������ �Ϲݵ� ġ��
		Serialint64(ar,m_GenDim_Plan_Upper);
		Serialint64(ar,m_GenDim_Plan_Lower);
		Serialint64(ar,m_GenDim_Jong_Upper);
		Serialint64(ar,m_GenDim_Jong_Lower);

		//������ �󼼵� ======================================================================
		ar << m_Det_bPlanView;
		ar << m_Det_bJongView;	
		ar << m_Det_bKeyPlan;
		ar << m_Det_bKeyPlanSpliceMark;
		ar << m_Det_bKeyPlanJijumMark;
		ar << m_Det_bKeyPlanCrossBeam;
		ar << m_Det_bKeyPlanDimension;
		ar << m_Det_bKeyPlanDirection;
		ar << m_Det_bKeyPlanOneLine;
		ar << m_Det_bKeyPlanRoVer;	
		ar << m_Det_bWeldDetail;
		ar << m_Det_bFieldWeldDetail; //2006.12.07�� �߰�...nFlag = 7
		ar << m_Det_bSolePlateDetail;
		ar << m_Det_bVStiffDetail;
		ar << m_Det_bHStiffDetail;	
		ar << m_Det_bLegend;
		ar << m_Det_bBasicNote;
		ar << m_Det_bBasicNoteVert;
		
		ar << m_Det_bGirderdan;
		ar << m_Det_bDiapHStiffPyung;
		ar << m_Det_bSttEndStationPyung;
		ar << m_Det_bPierAbutPyung;
		ar << m_Det_bLineInfoPyung;

		ar << m_Det_bLevelJong;	
		ar << m_Det_bSectionMarkJong;
		ar << m_Det_bHStiffJong;
		ar << m_Det_bSolePlateJong;
		ar << m_Det_bPierAbutJong;	
		ar << m_Det_nDivPage;		
		ar << m_Det_UpperWeldbGrind;
		ar << m_Det_LowerWeldbGrind;		
		ar << m_Det_nUnitEaDraw;
		ar << m_Det_MaxSebuDetailSu;	

		ar << m_Det_Scale;
		ar << m_Det_bVStiffJewon;
		ar << m_Det_bVStiffAll;

		//������ �󼼵� ġ���� ===============================================================	
		Serialint64(ar, m_DetailDim_Plan_Upper); 
		Serialint64(ar, m_DetailDim_Plan_Lower); 
		Serialint64(ar, m_DetailDim_Jong_Upper); 
		Serialint64(ar, m_DetailDim_Jong_Lower); 

		// ���� �극�̽�
		ar << m_HBracing_Summary;
		ar << m_HBracing_Pyung;
		ar << m_HBracing_Table;
		ar << m_HBracing_Plan;
//		ar << m_HBracing_Front;
		ar << m_HBracing_Detail;
		ar << m_HBracing_BM;
		ar << m_HBracing_Note;

		ar << m_HBracing_bJiJumMark;
		ar << m_HBracing_bDimension;
		ar << m_HBracing_bDirection;
		ar << m_HBracing_bOneline;
		ar << m_HBracing_bLineInfo;
		ar << m_HBracing_nRowSu;
		ar << m_HBracing_Scale;

		// �������� �󼼵�
		ar << m_SP_bTopView;
		ar << m_SP_bWebView;
		ar << m_SP_bBottomView;
		ar << m_SP_bTopRib;
		ar << m_SP_bBottomRib;
		ar << m_SP_bSangSeRib;
		ar << m_SP_bHort;			//�÷��� ���� ���
		ar << m_SP_bSpliceFull;   // ��ü�Ŵ� ���
		ar << m_SP_nJewon;
		ar << m_SP_Scale;
		ar << m_SP_bAllGirder;

		// ��������ǥ
		ar << m_BM_OutSelect;		// ���ǥ ��� ����
		ar << m_BM_PaintSelect;	// ���ǥ ��� ����
		ar << m_BM_WeldSelect;	// ���ǥ ��� ����
		ar << m_BM_TableSu;		// ���������� ���̺� ��
		ar << m_BM_LineSu;		// �����̺�� ���� ��
		ar << m_BM_Paint_TableSu;		// ���������� ���̺� ��
		ar << m_BM_Paint_LineSu;		// �����̺�� ���� ��
		ar << m_BM_Weld_TableSu;		// ���������� ���̺� ��
		ar << m_BM_Weld_LineSu;		// �����̺�� ���� ��	
		ar << m_nTypeJewonOut;
		
		// ����ġ��
		ar << m_Shoe_dScaleHori;		// ����ġ�� ��������
		ar << m_Shoe_dScaleVert;		// ����ġ�� ����������
		ar << m_Shoe_bShoeCoord;		// ����ǥ��
		ar << m_Shoe_bTableCoord;		// ����ǥ���̺�
		ar << m_Shoe_bTableLegend;	// ����
		ar << m_Shoe_bTableShoeBM;	// ������ǥ
		ar << m_Shoe_bCoordZ;			//3���� ��ǥ���
		ar << m_Shoe_bAngle;			//Angle ���
		ar << m_Shoe_bLineInfo;
		ar << m_Shoe_bCLSlabXY; 
		ar << m_Shoe_bCLSttEndXY;
		ar << m_Shoe_bSlabXY;	
		ar << m_Shoe_bSttEndXY;
		ar << m_Shoe_bSpanCenter;	 //������ħ��ġ�� ġ��
		ar << m_Shoe_bTotalCenter; //������ħ��ġ�� ġ��

		ar << m_Shoe_dColumSu;
		ar << m_Elev_dScaleSection;	// ELEVATION ����ٰŵ� �ܸ�
		ar << m_Elev_dScaleShoeDetail;	// ELEVATION ����ٰŵ� ���¸��
		ar << m_Elev_dScaleTable;		// ELEVATION ����ٰŵ� ���̺�
		ar << m_Elev_bSection;		// ELEVATION ����ٰŵ� �ܸ�
		ar << m_Elev_bShoeDetail;		// ELEVATION ����ٰŵ� ���¸��
		ar << m_Elev_bTable;			// ELEVATION ����ٰŵ� ���̺�

		//���κ� �󼼵� 
		ar << m_Cross_Summary;
		ar << m_Cross_Pyung;					//���鵵 
		ar << m_Cross_Table;					//���鵵 
		ar << m_Cross_Front;					//���鵵 
		ar << m_Cross_Plan;					//��鵵
		ar << m_Cross_GassetUpperDetail;		//����� ���
		ar << m_Cross_GassetLowerDetail;		//����� �Ϻ�
		ar << m_Cross_GassetSupportDetail;	//����� ������
		ar << m_Cross_VStiffDetail;			//����������
		ar << m_Cross_HStiffDetail;			//���򺸰���
		ar << m_Cross_AnchorBarDetail;		//��Ŀ�� 
		ar << m_Cross_GassetScallopDetail;	//����� ��Ķ�� 	
		ar << m_Cross_BM;						//���ǥ 
		ar << m_Cross_BKSupport;				//����� ������ �� 		
		ar << m_Cross_nRowSu;					//���̺� �ټ�
//		ar << m_Cross_DrawDiap;				//���̾����� ǥ�� 
		ar << m_CrossBeam_Scale;			//������ 0, 1�� ���κ� ��絵 ���� , ���� ������ 	

		ar << m_Cross_AllGirder;				//��ü �Ŵ� ��� 
		ar << m_Cross_TwoDrawing;			//�󼼵� 2�忡 ������ ���
		ar << m_Cross_KeyPlan;				//��൵ ���
		ar << m_Cross_KeyPlanRoVer;			//��൵ ����ȸ��		
		ar << m_Cross_KeyPlanSpliceMark;
		ar << m_Cross_KeyPlanJijumMark;
		ar << m_Cross_KeyPlanCrossBeam;
		ar << m_Cross_KeyPlanDimension;
		ar << m_Cross_KeyPlanDirection;
		ar << m_Cross_KeyPlanOneLine;	

		ar << m_Cross_dPosHScale;
		ar << m_Cross_dPosVScale;
		ar << m_Cross_bSpliceMark;		
		ar << m_Cross_bCrossBeam;

		//���κ� ġ��ǥ 
		ar << m_Cross_bJiJumMark;
		ar << m_Cross_bDimension;
		ar << m_Cross_bDirection;
		ar << m_Cross_bOneline;
		ar << m_Cross_bLineInfo;
		
		//������
		ar << m_nCamber_Type;
		ar << m_bCamber_Table;
		ar << m_bCamber_AllGirder;
		ar << m_bCamber_Note;
		ar << m_bCamber_Legend;
		ar << m_dCamber_HScale;
		ar << m_dCamber_VScale;
		ar << m_dCamber_TableHScale;
		ar << m_nCamber_DivType;
		ar << m_nCamber_TableColSu;
		ar << m_nCamber_UnitEa;
		ar << m_bCamber_AllGirderFull;

		//�������赵
		ar << m_bWeldLeftGirder; 
		ar << m_bWeldCamberLevel;	
		ar << m_bWeldDivDomyun; 
		ar << m_bWeldKeyPlan; 
		ar << m_bWeldKeyPlanSpliceMark;
		ar << m_bWeldKeyPlanJijumMark; 
		ar << m_bWeldKeyPlanCrossBeam; 
		ar << m_bWeldKeyPlanDimension; 
		ar << m_bWeldKeyPlanDirection; 
		ar << m_bWeldKeyPlanOneLine; 
		ar << m_bWeldKeyPlanRoVer;

		ar << m_bWeldTypicalSection; 
		ar << m_bWeldNote; 
		ar << m_bWeldMarkSystem; 
		ar << m_bWeldLineType; 
		ar << m_bWeldDimText; 
		ar << m_nWeldMark; 

		ar << m_nWeldDanmyunScale;
		ar << m_nWeldSangseScale;
		ar << m_nWeldTypicalScale;
		ar << m_nWeldKeyPlanHor;
		ar << m_nWeldKeyPlanVer;
		ar << m_nWeldDivBlock;
		ar << m_nWeldDivDomyun;

		//�����극�̽� �󼼵�
		ar << m_VBracing_Summary;		
		ar << m_VBracing_Pyung;			
		ar << m_VBracing_Table;			
		ar << m_VBracing_Plan;			
		ar << m_VBracing_Front;			
		ar << m_VBracing_Detail;
		ar << m_VBracing_AnchorBarDetail;
		ar << m_VBracing_BM;				
		ar << m_VBracing_AllGirder;
		ar << m_VBracing_KeyPlan;				//��൵ ���
		ar << m_VBracing_KeyPlanRoVer;			//��൵ ����ȸ��
		ar << m_VBracing_KeyPlanJijumMark;
		ar << m_VBracing_KeyPlanCrossBeam;
		ar << m_VBracing_KeyPlanDimension;
		ar << m_VBracing_KeyPlanDirection;
		ar << m_VBracing_KeyPlanOneLine;  
		ar << m_VBracing_Note;			

		ar << m_VBracing_bJiJumMark;		
		ar << m_VBracing_bDimension;		
		ar << m_VBracing_bDirection;		
		ar << m_VBracing_bOneline;		
		ar << m_VBracing_bLineInfo;	

		ar << m_VBracing_Scale;
		ar << m_VBracing_nRowSu;
		ar << m_Cross_Type;
		//
		ar << m_Cross_PlanUpper;	
		ar << m_Cross_PlanLower;	
		ar << m_Cross_Sangse;		
		ar << m_Cross_SpliceDetail;

		// �Ϲ� ��鵵
		ar << m_PlanGen_Plane_bIP		;
		ar << m_PlanGen_Plane_bChain	;
		ar << m_PlanGen_Plane_bCoord	;

		ar << m_PlanGen_nDimPyung			;
		ar << m_PlanGen_bMarkDanPyung		;
		ar << m_PlanGen_bJijumInfoPyung		;
		ar << m_PlanGen_bJijumInfoPyung_Sta	;
		ar << m_PlanGen_bJijumInfoPyung_Mark;
		ar << m_PlanGen_bPlaneInfoPyung		;

		ar << m_PlanGen_Plane_dStart	;
		ar << m_PlanGen_Plane_dEnd		;

		ar << m_PlanGen_Plane_nUnitChain;
		ar << m_PlanGen_Plane_nUnitCoord;

		// ���������� : ���� ����
		ar << m_bLineInfo_PlanInfoTable;
		ar << m_bLineInfo_PlanInfoTable_IP;
		ar << m_bLineInfo_PlanGrid;
		ar << m_bLineInfo_JongDanLine;
		ar << m_bLineInfo_JongDanPlanInfoTable;
		ar << m_bLineInfo_PyungubaePlanChainUnit;
		ar << m_bLineInfo_PyungubaePlanLeft;
		ar << m_bLineInfo_PyungubaePlanRight;
		ar << m_bLineInfo_PyungubaePlanInfoLeft;
		ar << m_bLineInfo_PyungubaePlanInfoRight;
		ar << m_bLineInfo_PyungubaePlanLineInfo;
		ar << m_bLineInfo_PyungubaePlanInfoTable;

		// ���������� : SCALE
		ar << m_dLineInfo_PlaneScale;
		ar << m_dLineInfo_JongDanHScale;
		ar << m_dLineInfo_JongDanVScale;
		ar << m_dLineInfo_PyungubaeScale;
		ar << m_dLineInfo_PyungubaeRatio;

		//�ܸ��൵
		ar <<   m_nSum_DivType;	//���� ���� ���
		ar <<   m_dSum_Scale;
		ar <<   m_dSum_MomentY;
		ar <<   m_dSum_HeightY;
		ar <<   m_dSum_FlangeY;
		ar <<   m_nSum_DivNum;

		// ���� �󼼵�
		ar <<   m_bPaint_GirGen;
		ar <<   m_bPaint_GirGenFront;
		ar <<   m_bPaint_TableBox;
		ar <<   m_bPaint_Section;
		ar <<   m_bPaint_SectionFront;
		ar <<   m_bPaint_TableSection;

		ar <<   m_bPaint_Splice;
		ar <<   m_bPaint_SpliceUp;
		ar <<   m_bPaint_SpliceWeb;
		ar <<   m_bPaint_SpliceDown;
		ar <<   m_bPaint_TableSplice;
		
		ar <<   m_bPaint_Stringer;
		ar <<   m_bPaint_StringerFront;
		ar <<   m_bPaint_StringerPlanUp;
		ar <<   m_bPaint_StringerPlanDown;
		ar <<   m_bPaint_TableStringer;
		
		ar <<   m_bPaint_Cross;
		ar <<   m_bPaint_CrossFront;
		ar <<   m_bPaint_CrossPlan;
		ar <<   m_bPaint_TableCross;
		
		ar <<   m_dPaint_Scale_GirGen;
		ar <<   m_dPaint_Scale_Section;
		ar <<   m_dPaint_Scale_Splice;
		ar <<   m_dPaint_Scale_Stringer;
		ar <<   m_dPaint_Scale_Cross;

		// ������ �Ϲݵ�
		ar << 	m_bSlab_PlanView;	// ��鵵
		ar << 	m_bSlab_JongView;	// ���鵵
		ar << 	m_bSlab_FrontView; // Ⱦ�ܸ鵵
		ar <<	m_bSlab_FrontViewLeft;	// Ⱦ�ܸ鵵(����)
		ar <<	m_bSlab_FrontViewRight;	// Ⱦ�ܸ鵵(����)
		ar << 	m_bSlab_FrontDetView; // Ⱦ�ܸ鵵 ��
		ar << 	m_bSlab_SectionAA;		// �Ŵ��� �ܸ� ��
		ar << 	m_bSlab_SectionBB;		// �ڽ��� �ܸ� ��
		ar << 	m_bSlab_SectionCC;		// �ڽ��� �߾Ӵܸ� ��
		ar << 	m_bSlab_SectionDD;		// ���κ� �ܸ� ��
		ar << 	m_bSlab_SectionEE;		// ��Ʈ���� ��
		ar << 	m_bSlab_GuideWall;			// ��ȣ�� ��
		ar << 	m_bSlab_Notch;			// NOTCH ��
		ar << 	m_bSlab_KeyPlan;		// ��൵
		ar <<	m_bSlab_bKeyPlanRoVer;		
		ar << 	m_nSlab_DivType;		// ���� ���� ���
		ar << 	m_bSlab_UpperSlabLeftDim;	// ���̾�����(��)
		ar << 	m_bSlab_UpperJiganLeftDim;	// ��� ��������(��)
		ar << 	m_bSlab_LowerSlabRightDim;	// ���̾�����(��)
		ar <<	m_bSlab_LowerJiganRightDim;	// �Ϻ� ��������(��)
		ar <<	m_bSlab_MatchLine;
		ar <<	m_bSlab_bPyungLineInfo;		//��������
		
		ar << 	m_dSlab_ScaleSlabGen;	// ������ �Ϲݵ� ������
		ar << 	m_dSlab_ScaleFrontDet;	// Ⱦ�ܸ鵵 �� ������
		ar << 	m_dSlab_ScaleSectionAA;	
		ar << 	m_dSlab_ScaleSectionBB;
		ar << 	m_dSlab_ScaleSectionCC;
		ar << 	m_dSlab_ScaleSectionDD;
		ar << 	m_dSlab_ScaleSectionEE;
		ar << 	m_dSlab_ScaleGuideWall;
		ar <<	m_dSlab_ScaleNothch;
		ar <<	m_dSlab_ScaleKeyPlanHor;
		ar <<	m_dSlab_ScaleKeyPlanVer;
		ar <<	m_nSlab_DivNum;			// ��� ���� ����
		ar <<	m_bSlab_DimTable;				// ġ����ȭ ���̺�
		ar <<	m_nSlab_DimTableType;
		ar <<	m_bSlab_DimTableFirstPage;
		ar <<	m_bSlab_Pyungubae;
		ar <<	m_dSlab_DimTableScale;
		ar <<	m_dSlab_DimTableHeightFactor;
		ar <<	m_dSlab_DimTableWidhtFactor;

		ar <<	m_bSlab_KeyPlanSpliceMark;			// ��൵
		ar <<	m_bSlab_KeyPlanJijumMark;
		ar <<	m_bSlab_KeyPlanCrossBeam;
		ar <<	m_bSlab_KeyPlanDimension;
		ar <<	m_bSlab_KeyPlanDirection;
		ar <<	m_bSlab_KeyPlanOneLine;

		ar <<	m_bTendon_PlanView;
		ar <<	m_bTendon_SectionAA;
		ar <<	m_nTendon_DivType;
		ar <<	m_bTendon_MatchLine;
		ar <<	m_dTendon_ScaleGen;
		ar <<	m_dTendon_ScaleSectionAA;
		ar <<   m_nTendon_DivNum;

		ar << m_nSlab_NotchType;
		ar << m_dSlab_NotchHeight;
		ar << m_dSlab_NotchTopWidth;
		ar << m_dSlab_NotchBotWidth;

		ar << m_nSlab_PlanView_DimOpt;

		ar << m_bDeckRebar_PlanView;					// ��鵵
		ar << m_bDeckRebar_JongView;					// ���鵵	
		ar << m_bDeckRebar_FrontView;					// Ⱦ�ܸ鵵
		ar << m_bDeckRebar_FrontViewCenter;				// Ⱦ�ܸ鵵(�߾Ӻ�)
		ar << m_bDeckRebar_FrontViewJijum;				// Ⱦ�ܸ鵵(������)	
		ar << m_bDeckRebar_MainRebarView;				// ��ö�� ������
		ar << m_bDeckRebar_BelyckView;					// ���ö�� ������
		ar << m_bDeckRebar_Sangse;						// �󼼵� (����Ʈ��)
		ar << m_bDeckRebar_SangseDanbu;					//		������ �ܺ� �󼼵�
		ar << m_bDeckRebar_SangseDanmyunAA;				//		�ܸ�A-A �󼼵�
		ar << m_bDeckRebar_SangseWaterReinforce;		//		����� �����󼼵�
		ar << m_bDeckRebar_SangseCentReinforce;			//		�߾������� ����ö�� �����󼼵�
		ar << m_bDeckRebar_SangseDoubleChain;			//		ö�� ������ �󼼵�
		ar << m_bDeckRebar_SangseA;						//		�󼼵�-A
		ar << m_bDeckRebar_SangseChairBlock;			//		�󼼵�-����ö�� �� chair block��
		ar << m_bDeckRebar_SangseShearReinforce;		//		���ܺ���ö�� ��
		ar << m_bDeckRebar_SangseShearArray;			//		����ö�ٹ�ġ��
		ar << m_bDeckRebar_SangseBlockOut;				//		Block Out ��
		ar << m_bDeckRebar_SangseCrossBeam;				//		���κ� ��
		ar << m_bDeckRebar_SangseStringer;				//		���κ� ��
		ar << m_bDeckRebar_SangseMatrial;				//		ö�ٻ󼼵�
		ar << m_bDeckRebar_SangseMatrialTable;			//		ö�ٻ�ǥ
		ar << m_bDeckRebar_GuardSection;				//		��ȣ�� �ܸ鵵
		ar << m_bDeckRebar_GuardDetail;					//		��ȣ�� ö�ٻ�
		ar << m_bDeckRebar_GuardTable;					//		��ȣ�� ö�����̺� 


		ar << m_dDeck_ScaleRebarPlan;					// ������ ��鵵 ������
		ar << m_dDeck_ScaleRebarJong;					// ������ ���鵵 ������
		ar << m_dDeck_ScaleRebarFront;					// ������ Ⱦ�ܸ鵵 ������
		ar << m_dDeck_ScaleRebarSangseDanbu;			// ������ �󼼵�-�ܺλ󼼵� ������
		ar << m_dDeck_ScaleRebarSangseDanmyunAA;		// ������ �󼼵�-�ܸ�A-A ������
		ar << m_dDeck_ScaleRebarSangseWaterReinforce;	// ����� ���� �󼼵�
		ar << m_dDeck_ScaleRebarSangseCentReinforce;	// �߾������� ����ö�� ���� �󼼵�
		ar << m_dDEck_ScaleRebarSangseDoubleChain;		// ö�� ������ �� ������
		ar << m_dDeck_ScaleSangseA;						// ������ �󼼵�-A ������
		ar << m_dDeck_ScaleSangseChairBlock;			// ������ �󼼵�-����ö�� �� chair block�� ������
		ar << m_dDeck_SacleSangseShearReinforce;		// ���ܺ���ö�� ��
		ar << m_dDeck_ScaleSangseShearArray;			// ������ �󼼵�-����ö�� ��ġ�� ������
		ar << m_dDeck_ScaleSangseBlockOut;				// BLOCKOUT ��
		ar << m_dDeck_ScaleSangseCrossBeam;				// ���κ� �󼼽�����			
		ar << m_dDeck_ScaleSangseStringer;				// ���κ� �󼼽�����			
		ar << m_dDeck_ScaleRebarMain;					// ��ö�� ������ ������
		ar << m_dDeck_MainRebarCycle;					// ��ö�� ������ ����Ŭ��
		ar << m_dDeck_ScaleRebarBelyck;					// ���ö�� ������ ������
		ar << m_dDeck_BelyckCycle;						// ���ö�� ������ ����Ŭ��
		ar << m_dDeck_SpaceBetweenJewon;				// ö�� ��������
		ar << m_dDeck_DatasuOnRebarTable;				// ���̺�� �����ͼ�
		ar << m_dDeck_TypeRebarFrontLeft;				// ������ Ⱦ�ܸ鵵 ���Ÿ�� 
		ar << m_dDeck_TypeRebarFrontRigh;				// ������ Ⱦ�ܸ鵵 ���Ÿ�� 
		ar << m_dDeck_ScaleGuardRebar;					// ��ȣ��ö�� ������ 
		ar << m_bDeckRebar_SangseRebar;					// ö�� ��(����Ʈ��)
		ar << m_bDeckRebar_SangseGuard;					// ��ȣ�� ��(����Ʈ��)
		ar << m_nDeck_JongDrawPos;						// ������ ���ܸ鵵 ��� ����
		ar << m_nDeck_Slab_DivType;
		ar << m_nDeck_Slab_RebarTableOpt;
		ar << m_bDeck_ShowJointMainRebar;
		ar << m_bDeck_ShowJointBeryukRebar;
		ar << m_nTypeRebarTable;						//ö�����ǥ Ÿ��
		ar << m_dWidthRebarTableRect;					//ö�ٻ� Rect ��
		ar << m_dHeightRebarTableRect;					//ö�ٻ� Rect ����

		// TU�Ŵ� ��ٵ�
		ar << m_bTU_Plan;
		ar << m_bTU_Jong;
		ar << m_bTU_Detail;
		ar << m_bTU_DetailSteel;
		ar << m_bTU_TableSteel;

		ar << m_dTUScale;
		ar << m_dTUScale_Plan;	
		ar << m_dTUScale_Jong;	
		ar << m_dTUScale_Detail;

		ar << m_nTU_Trim;
		ar << m_Det_bVStiffLeftRight;
	}
	else 
	{
		ar >> nFlag;

		ar >> m_sDomNumHead;
		ar >> m_szDGKName;
		ar >> m_szXRefPath;
		ar >> m_sTextBridgeDirection;
		ar >> m_sTextCounterDirection;
		ar >> m_bTitleDogak;
		ar >> m_sHead_DomyunNumber;
		if(nFlag>14)
			ar >> m_bUsedHCad;
	
		m_nDomPageArray.Serialize(ar);
		m_nDomTitleArray.Serialize(ar);
				
		// ������ �Ϲݵ�
		ar >> m_Gen_bDivPlanJong;	
		ar >> m_Gen_bBujeThick;
		ar >> m_Gen_bKeyPlanOneLine;
		ar >> m_Gen_bKeyPlanRoVer;		

		ar >> m_Gen_bPlanView;
		ar >> m_Gen_bJongView;
		ar >> m_Gen_bFrontView;
		ar >> m_Gen_bVerTitleFront;
		ar >> m_Gen_bKeyPlan;
		ar >> m_Gen_bLegend;
		ar >> m_Gen_bBasicNote;
		ar >> m_Gen_bForceText;
		ar >> m_Gen_bForceTextLow;
		ar >> m_Gen_bByundan;
		ar >> m_Gen_bMatchLine;
		ar >> m_Gen_nPyungDim;
		ar >> m_Gen_nDivPage;
		ar >> m_Gen_nStyleDanmyunType;
			
		ar >> m_Gen_bJongCenterGir;
		ar >> m_Gen_bJongGenGir;
		ar >> m_Gen_nOutGirder;
		ar >> m_Gen_bKeyPlanSpliceMark;
		ar >> m_Gen_bKeyPlanJijumMark;
		ar >> m_Gen_bKeyPlanCrossBeam;
		ar >> m_Gen_bKeyPlanDimension;
		ar >> m_Gen_bKeyPlanDirection;
		
		ar >> m_Gen_bJijumSymbol;
		ar >> m_Gen_bJijumSimple;
		ar >> m_Gen_bMarkCorss;
		ar >> m_Gen_bMarkStringer;
		ar >> m_Gen_bJongLevel;
		ar >> m_Gen_bVerticalNote;

		ar >> m_Gen_bPyungJijumStation;
		ar >> m_Gen_bPyungLineInfo;
		ar >> m_Gen_bJongJijumStation;
		ar >> m_Gen_bDimCenterGirder;	
		ar >> m_Gen_bDimNormalGirder;			
		ar >> m_Gen_nUnitEaDraw;	
		ar >> m_Gen_Scale;

		// ������ �Ϲݵ� ġ��
		Serialint64(ar,m_GenDim_Plan_Upper);
		Serialint64(ar,m_GenDim_Plan_Lower);
		Serialint64(ar,m_GenDim_Jong_Upper);
		Serialint64(ar,m_GenDim_Jong_Lower);

		//������ �󼼵� ======================================================================
		ar >> m_Det_bPlanView;
		ar >> m_Det_bJongView;	
		ar >> m_Det_bKeyPlan;
		ar >> m_Det_bKeyPlanSpliceMark;
		ar >> m_Det_bKeyPlanJijumMark;
		ar >> m_Det_bKeyPlanCrossBeam;
		ar >> m_Det_bKeyPlanDimension;
		ar >> m_Det_bKeyPlanDirection;
		ar >> m_Det_bKeyPlanOneLine;
		ar >> m_Det_bKeyPlanRoVer;	
		ar >> m_Det_bWeldDetail;
		if (nFlag > 6)
			ar >> m_Det_bFieldWeldDetail;//2006.12.07 �߰� ..

		ar >> m_Det_bSolePlateDetail;
		ar >> m_Det_bVStiffDetail;
		ar >> m_Det_bHStiffDetail;	
		ar >> m_Det_bLegend;
		ar >> m_Det_bBasicNote;
		ar >> m_Det_bBasicNoteVert;
		
		ar >> m_Det_bGirderdan;
		ar >> m_Det_bDiapHStiffPyung;
		ar >> m_Det_bSttEndStationPyung;
		ar >> m_Det_bPierAbutPyung;
		ar >> m_Det_bLineInfoPyung;

		ar >> m_Det_bLevelJong;	
		ar >> m_Det_bSectionMarkJong;
		ar >> m_Det_bHStiffJong;
		ar >> m_Det_bSolePlateJong;
		ar >> m_Det_bPierAbutJong;	
		ar >> m_Det_nDivPage;		
		ar >> m_Det_UpperWeldbGrind;
		ar >> m_Det_LowerWeldbGrind;		
		ar >> m_Det_nUnitEaDraw;
		ar >> m_Det_MaxSebuDetailSu;	

		ar >> m_Det_Scale;
		ar >> m_Det_bVStiffJewon;
		if(nFlag>3)
			ar >> m_Det_bVStiffAll;

		//������ �󼼵� ġ���� ===============================================================	
		Serialint64(ar, m_DetailDim_Plan_Upper); 
		Serialint64(ar, m_DetailDim_Plan_Lower); 
		Serialint64(ar, m_DetailDim_Jong_Upper); 
		Serialint64(ar, m_DetailDim_Jong_Lower); 

		// ���� �극�̽�
		ar >> m_HBracing_Summary;
		ar >> m_HBracing_Pyung;
		ar >> m_HBracing_Table;
		ar >> m_HBracing_Plan;
//		ar >> m_HBracing_Front;
		ar >> m_HBracing_Detail;
		ar >> m_HBracing_BM;
		ar >> m_HBracing_Note;

		ar >> m_HBracing_bJiJumMark;
		ar >> m_HBracing_bDimension;
		ar >> m_HBracing_bDirection;
		ar >> m_HBracing_bOneline;
		ar >> m_HBracing_bLineInfo;
		ar >> m_HBracing_nRowSu;
		ar >> m_HBracing_Scale;

		// �������� �󼼵�
		ar >> m_SP_bTopView;
		ar >> m_SP_bWebView;
		ar >> m_SP_bBottomView;
		ar >> m_SP_bTopRib;
		ar >> m_SP_bBottomRib;
		ar >> m_SP_bSangSeRib;
		ar >> m_SP_bHort;			//�÷��� ���� ���
		ar >> m_SP_bSpliceFull;   // ��ü�Ŵ� ���
		ar >> m_SP_nJewon;
		ar >> m_SP_Scale;
		ar >> m_SP_bAllGirder;

		// ��������ǥ
		ar >> m_BM_OutSelect;		// ���ǥ ��� ����
		ar >> m_BM_PaintSelect;	// ���ǥ ��� ����
		ar >> m_BM_WeldSelect;	// ���ǥ ��� ����
		ar >> m_BM_TableSu;		// ���������� ���̺� ��
		ar >> m_BM_LineSu;		// �����̺�� ���� ��
		ar >> m_BM_Paint_TableSu;		// ���������� ���̺� ��
		ar >> m_BM_Paint_LineSu;		// �����̺�� ���� ��
		ar >> m_BM_Weld_TableSu;		// ���������� ���̺� ��
		ar >> m_BM_Weld_LineSu;		// �����̺�� ���� ��	
		ar >> m_nTypeJewonOut;
		
		// ����ġ��
		ar >> m_Shoe_dScaleHori;		// ����ġ�� ��������
		ar >> m_Shoe_dScaleVert;		// ����ġ�� ����������
		ar >> m_Shoe_bShoeCoord;		// ����ǥ��
		ar >> m_Shoe_bTableCoord;		// ����ǥ���̺�
		ar >> m_Shoe_bTableLegend;	// ����
		ar >> m_Shoe_bTableShoeBM;	// ������ǥ
		ar >> m_Shoe_bCoordZ;			//3���� ��ǥ���
		ar >> m_Shoe_bAngle;			//Angle ���
		ar >> m_Shoe_bLineInfo;
		ar >> m_Shoe_bCLSlabXY; 
		ar >> m_Shoe_bCLSttEndXY;
		ar >> m_Shoe_bSlabXY;	
		ar >> m_Shoe_bSttEndXY;
		ar >> m_Shoe_bSpanCenter;	 //������ħ��ġ�� ġ��
		ar >> m_Shoe_bTotalCenter; //������ħ��ġ�� ġ��

		ar >> m_Shoe_dColumSu;
		ar >> m_Elev_dScaleSection;	// ELEVATION ����ٰŵ� �ܸ�
		ar >> m_Elev_dScaleShoeDetail;	// ELEVATION ����ٰŵ� ���¸��
		ar >> m_Elev_dScaleTable;		// ELEVATION ����ٰŵ� ���̺�
		ar >> m_Elev_bSection;		// ELEVATION ����ٰŵ� �ܸ�
		ar >> m_Elev_bShoeDetail;		// ELEVATION ����ٰŵ� ���¸��
		ar >> m_Elev_bTable;			// ELEVATION ����ٰŵ� ���̺�

		//���κ� �󼼵� 
		ar >> m_Cross_Summary;
		ar >> m_Cross_Pyung;					//���鵵 
		ar >> m_Cross_Table;					//���鵵 
		ar >> m_Cross_Front;					//���鵵 
		ar >> m_Cross_Plan;					//��鵵
		ar >> m_Cross_GassetUpperDetail;		//����� ���
		ar >> m_Cross_GassetLowerDetail;		//����� �Ϻ�
		ar >> m_Cross_GassetSupportDetail;	//����� ������
		ar >> m_Cross_VStiffDetail;			//����������
		ar >> m_Cross_HStiffDetail;			//���򺸰���
		ar >> m_Cross_AnchorBarDetail;		//��Ŀ�� 
		ar >> m_Cross_GassetScallopDetail;	//����� ��Ķ�� 	
		ar >> m_Cross_BM;						//���ǥ 
		ar >> m_Cross_BKSupport;				//����� ������ �� 		
		ar >> m_Cross_nRowSu;					//���̺� �ټ�
//		ar >> m_Cross_DrawDiap;				//���̾����� ǥ�� 
		ar >> m_CrossBeam_Scale;			//������ 0, 1�� ���κ� ��絵 ���� , ���� ������ 	

		ar >> m_Cross_AllGirder;				//��ü �Ŵ� ��� 
		if(nFlag>10)
			ar >> m_Cross_TwoDrawing;
		ar >> m_Cross_KeyPlan;					//��൵ ���
		ar >> m_Cross_KeyPlanRoVer;			//��൵ ����ȸ��		
		if(nFlag>0)
		{
			ar >> m_Cross_KeyPlanSpliceMark;
			ar >> m_Cross_KeyPlanJijumMark;
			ar >> m_Cross_KeyPlanCrossBeam;
			ar >> m_Cross_KeyPlanDimension;
			ar >> m_Cross_KeyPlanDirection;
			ar >> m_Cross_KeyPlanOneLine;			
		}

		if(nFlag>9)
		{
			ar >> m_Cross_dPosHScale;				
			ar >> m_Cross_dPosVScale;
			ar >> m_Cross_bSpliceMark;			
			ar >> m_Cross_bCrossBeam;
		}
		//���κ� ġ��ǥ 
		ar >> m_Cross_bJiJumMark;
		ar >> m_Cross_bDimension;
		ar >> m_Cross_bDirection;
		ar >> m_Cross_bOneline;
		ar >> m_Cross_bLineInfo;	
		
		//������
		ar >> m_nCamber_Type;
		ar >> m_bCamber_Table;
		ar >> m_bCamber_AllGirder;
		ar >> m_bCamber_Note;
		ar >> m_bCamber_Legend;
		ar >> m_dCamber_HScale;
		ar >> m_dCamber_VScale;
		ar >> m_dCamber_TableHScale;
		if(nFlag < 14)
			m_dCamber_TableHScale = 1;	// ������ ������� �ʴ� ���� ��Ȱ��
		ar >> m_nCamber_DivType;
		ar >> m_nCamber_TableColSu;
		ar >> m_nCamber_UnitEa;
		ar >> m_bCamber_AllGirderFull;

		//�������赵
		ar >> m_bWeldLeftGirder; 
		ar >> m_bWeldCamberLevel;	
		ar >> m_bWeldDivDomyun; 
		ar >> m_bWeldKeyPlan; 
		ar >> m_bWeldKeyPlanSpliceMark;
		ar >> m_bWeldKeyPlanJijumMark; 
		ar >> m_bWeldKeyPlanCrossBeam; 
		ar >> m_bWeldKeyPlanDimension; 
		ar >> m_bWeldKeyPlanDirection; 
		ar >> m_bWeldKeyPlanOneLine; 
		ar >> m_bWeldKeyPlanRoVer;

		ar >> m_bWeldTypicalSection; 
		ar >> m_bWeldNote; 
		ar >> m_bWeldMarkSystem; 
		ar >> m_bWeldLineType; 
		ar >> m_bWeldDimText; 
		ar >> m_nWeldMark; 

		ar >> m_nWeldDanmyunScale;
		ar >> m_nWeldSangseScale;
		ar >> m_nWeldTypicalScale;
		ar >> m_nWeldKeyPlanHor;
		ar >> m_nWeldKeyPlanVer;
		ar >> m_nWeldDivBlock;
		ar >> m_nWeldDivDomyun;

		//�����극�̽� �󼼵�
		ar >> m_VBracing_Summary;		
		ar >> m_VBracing_Pyung;			
		ar >> m_VBracing_Table;			
		ar >> m_VBracing_Plan;			
		ar >> m_VBracing_Front;			
		ar >> m_VBracing_Detail;
		ar >> m_VBracing_AnchorBarDetail;
		ar >> m_VBracing_BM;				
		ar >> m_VBracing_AllGirder;
		ar >> m_VBracing_KeyPlan;				//��൵ ���
		ar >> m_VBracing_KeyPlanRoVer;			//��൵ ����ȸ��
		if(nFlag>0)
		{
			ar >> m_VBracing_KeyPlanJijumMark;
			ar >> m_VBracing_KeyPlanCrossBeam;
			ar >> m_VBracing_KeyPlanDimension;
			ar >> m_VBracing_KeyPlanDirection;
			ar >> m_VBracing_KeyPlanOneLine;  
		}
		ar >> m_VBracing_Note;

		ar >> m_VBracing_bJiJumMark;		
		ar >> m_VBracing_bDimension;		
		ar >> m_VBracing_bDirection;		
		ar >> m_VBracing_bOneline;		
		ar >> m_VBracing_bLineInfo;	

		ar >> m_VBracing_Scale;
		ar >> m_VBracing_nRowSu;

		ar >> m_Cross_Type;
		ar >> m_Cross_PlanUpper;	
		ar >> m_Cross_PlanLower;	
		ar >> m_Cross_Sangse;	
		ar >> m_Cross_SpliceDetail;

		// �Ϲ� ��鵵
		ar >> m_PlanGen_Plane_bIP		;
		ar >> m_PlanGen_Plane_bChain	;
		ar >> m_PlanGen_Plane_bCoord	;

		ar >> m_PlanGen_nDimPyung			;
		ar >> m_PlanGen_bMarkDanPyung		;
		ar >> m_PlanGen_bJijumInfoPyung		;
		ar >> m_PlanGen_bJijumInfoPyung_Sta	;
		ar >> m_PlanGen_bJijumInfoPyung_Mark;
		ar >> m_PlanGen_bPlaneInfoPyung		;

		ar >> m_PlanGen_Plane_dStart	;
		ar >> m_PlanGen_Plane_dEnd		;

		ar >> m_PlanGen_Plane_nUnitChain;
		ar >> m_PlanGen_Plane_nUnitCoord;

		// ���������� : ���� ����
		ar >> m_bLineInfo_PlanInfoTable;
		ar >> m_bLineInfo_PlanInfoTable_IP;
		ar >> m_bLineInfo_PlanGrid;
		ar >> m_bLineInfo_JongDanLine;
		ar >> m_bLineInfo_JongDanPlanInfoTable;
		ar >> m_bLineInfo_PyungubaePlanChainUnit;
		ar >> m_bLineInfo_PyungubaePlanLeft;
		ar >> m_bLineInfo_PyungubaePlanRight;
		ar >> m_bLineInfo_PyungubaePlanInfoLeft;
		ar >> m_bLineInfo_PyungubaePlanInfoRight;
		ar >> m_bLineInfo_PyungubaePlanLineInfo;
		ar >> m_bLineInfo_PyungubaePlanInfoTable;

		// ���������� : SCALE
		ar >> m_dLineInfo_PlaneScale;
		ar >> m_dLineInfo_JongDanHScale;
		ar >> m_dLineInfo_JongDanVScale;
		ar >> m_dLineInfo_PyungubaeScale;
		ar >> m_dLineInfo_PyungubaeRatio;

		//�ܸ��൵
		if (nFlag > 8)
		{
			ar >>   m_nSum_DivType;
			ar >>   m_dSum_Scale;
			ar >>   m_dSum_MomentY;
			ar >>   m_dSum_HeightY;
			ar >>   m_dSum_FlangeY;
			ar >>   m_nSum_DivNum;
		}		

		// ���� �󼼵�
		ar >>   m_bPaint_GirGen;
		ar >>   m_bPaint_GirGenFront;
		ar >>   m_bPaint_TableBox;
		ar >>   m_bPaint_Section;
		ar >>   m_bPaint_SectionFront;
		ar >>   m_bPaint_TableSection;

		ar >>   m_bPaint_Splice;
		ar >>   m_bPaint_SpliceUp;
		ar >>   m_bPaint_SpliceWeb;
		ar >>   m_bPaint_SpliceDown;
		ar >>   m_bPaint_TableSplice;
		
		ar >>   m_bPaint_Stringer;
		ar >>   m_bPaint_StringerFront;
		ar >>   m_bPaint_StringerPlanUp;
		ar >>   m_bPaint_StringerPlanDown;
		ar >>   m_bPaint_TableStringer;

		ar >>   m_bPaint_Cross;
		ar >>   m_bPaint_CrossFront;
		ar >>   m_bPaint_CrossPlan;
		ar >>   m_bPaint_TableCross;

		ar >> m_dPaint_Scale_GirGen;
		ar >> m_dPaint_Scale_Section;
		ar >> m_dPaint_Scale_Splice;
		ar >> m_dPaint_Scale_Stringer;
		ar >> m_dPaint_Scale_Cross;	
		if(nFlag > 1)
		{
			// ������ �Ϲݵ�
			ar >> 	m_bSlab_PlanView;	// ��鵵
			ar >> 	m_bSlab_JongView;	// ���鵵
			ar >> 	m_bSlab_FrontView; // Ⱦ�ܸ鵵
			ar >> m_bSlab_FrontViewLeft;	// Ⱦ�ܸ鵵(����)
			ar >> m_bSlab_FrontViewRight;	// Ⱦ�ܸ鵵(����)
			ar >> 	m_bSlab_FrontDetView; // Ⱦ�ܸ鵵 ��
			ar >> 	m_bSlab_SectionAA;		// �Ŵ��� �ܸ� ��
			ar >> 	m_bSlab_SectionBB;		// �ڽ��� �ܸ� ��
			ar >> 	m_bSlab_SectionCC;		// �ڽ��� �߾Ӵܸ� ��
			ar >> 	m_bSlab_SectionDD;		// ���κ� �ܸ� ��
			ar >> 	m_bSlab_SectionEE;		// ��Ʈ���� ��
			ar >> 	m_bSlab_GuideWall;			// ��ȣ�� ��
			ar >> 	m_bSlab_Notch;			// NOTCH ��
			ar >> 	m_bSlab_KeyPlan;		// ��൵		
			ar >>	m_bSlab_bKeyPlanRoVer;			
			ar >> 	m_nSlab_DivType;		// ���� ���� ���
			ar >> 	m_bSlab_UpperSlabLeftDim;	// ���̾�����(��)
			ar >> 	m_bSlab_UpperJiganLeftDim;	// ��� ��������(��)
			ar >> 	m_bSlab_LowerSlabRightDim;	// ���̾�����(��)
			ar >>	m_bSlab_LowerJiganRightDim;	// �Ϻ� ��������(��)		
			ar >> m_bSlab_MatchLine;
			ar >> m_bSlab_bPyungLineInfo;
			ar >> 	m_dSlab_ScaleSlabGen;	// ������ �Ϲݵ� ������
			ar >> 	m_dSlab_ScaleFrontDet;	// Ⱦ�ܸ鵵 �� ������
			ar >> 	m_dSlab_ScaleSectionAA;	
			ar >> 	m_dSlab_ScaleSectionBB;
			ar >> 	m_dSlab_ScaleSectionCC;
			ar >> 	m_dSlab_ScaleSectionDD;
			ar >> 	m_dSlab_ScaleSectionEE;
			ar >> 	m_dSlab_ScaleGuideWall;
			ar >>	m_dSlab_ScaleNothch;
			ar >>	m_dSlab_ScaleKeyPlanHor;
			ar >>	m_dSlab_ScaleKeyPlanVer;
			ar >>	m_nSlab_DivNum;			// ��� ���� ����

			ar >>	m_bSlab_DimTable;
			ar >>	m_nSlab_DimTableType;
			ar >>	m_bSlab_DimTableFirstPage;
			ar >>	m_bSlab_Pyungubae;
			ar >>	m_dSlab_DimTableScale;
			ar >>	m_dSlab_DimTableHeightFactor;
			ar >>	m_dSlab_DimTableWidhtFactor;
			ar >>	m_bSlab_KeyPlanSpliceMark;			// ��൵
			ar >>	m_bSlab_KeyPlanJijumMark;
			ar >>	m_bSlab_KeyPlanCrossBeam;
			ar >>	m_bSlab_KeyPlanDimension;
			ar >>	m_bSlab_KeyPlanDirection;
			ar >>	m_bSlab_KeyPlanOneLine;
			
			if(nFlag>2)
			{
				ar >>	m_bTendon_PlanView;
				ar >>	m_bTendon_SectionAA;
				ar >>	m_nTendon_DivType;
				ar >>	m_bTendon_MatchLine;
				ar >>	m_dTendon_ScaleGen;
				ar >>	m_dTendon_ScaleSectionAA;
				ar >>   m_nTendon_DivNum;
			}

			if(nFlag>4)
			{
				ar >> m_nSlab_NotchType	;
				ar >> m_dSlab_NotchHeight;
				ar >> m_dSlab_NotchTopWidth;
				ar >> m_dSlab_NotchBotWidth;
			}

			if(nFlag>5)
				ar >> m_nSlab_PlanView_DimOpt;
		}
		if(nFlag > 7)
		{
			ar >> m_bDeckRebar_PlanView;					// ��鵵
			ar >> m_bDeckRebar_JongView;					// ���鵵	
			ar >> m_bDeckRebar_FrontView;				// Ⱦ�ܸ鵵
			ar >> m_bDeckRebar_FrontViewCenter;			// Ⱦ�ܸ鵵(�߾Ӻ�)
			ar >> m_bDeckRebar_FrontViewJijum;			// Ⱦ�ܸ鵵(������)	
			ar >> m_bDeckRebar_MainRebarView;			// ��ö�� ������
			ar >> m_bDeckRebar_BelyckView;				// ���ö�� ������
			ar >> m_bDeckRebar_Sangse;					// �󼼵� 
			ar >> m_bDeckRebar_SangseDanbu;				// ������ �ܺ� �󼼵�
			ar >> m_bDeckRebar_SangseDanmyunAA;			// �ܸ�A-A �󼼵�
			ar >> m_bDeckRebar_SangseWaterReinforce;		// ����� �����󼼵�
			ar >> m_bDeckRebar_SangseCentReinforce;		// �߾������� ����ö�� �����󼼵�
			ar >> m_bDeckRebar_SangseDoubleChain;		// ö�� ������ �󼼵�
			ar >> m_bDeckRebar_SangseA;					// �󼼵�-A
			ar >> m_bDeckRebar_SangseChairBlock;			// �󼼵�-����ö�� �� chair block��
			ar >> m_bDeckRebar_SangseShearReinforce;		// ���ܺ���ö�� ��
			ar >> m_bDeckRebar_SangseShearArray;			// ����ö�ٹ�ġ��
			ar >> m_bDeckRebar_SangseBlockOut;			// Block Out ��
			ar >> m_bDeckRebar_SangseCrossBeam;		// ���κ� ��
			ar >> m_bDeckRebar_SangseStringer;		// ���κ� ��
			ar >> m_bDeckRebar_SangseMatrial;			// ö�ٻ󼼵�
			ar >> m_bDeckRebar_SangseMatrialTable;		// ö�ٻ�ǥ

			ar >> m_bDeckRebar_GuardSection;				// ��ȣ�� �ܸ鵵
			ar >> m_bDeckRebar_GuardDetail;					// ��ȣ�� ö�ٻ�
			ar >> m_bDeckRebar_GuardTable;					// ��ȣ�� ö�����̺� 

			ar >> m_dDeck_ScaleRebarPlan;					// ������ ��鵵 ������
			ar >> m_dDeck_ScaleRebarJong;					// ������ ���鵵 ������
			ar >> m_dDeck_ScaleRebarFront;					// ������ Ⱦ�ܸ鵵 ������
			ar >> m_dDeck_ScaleRebarSangseDanbu;			// ������ �󼼵�-�ܺλ󼼵� ������
			ar >> m_dDeck_ScaleRebarSangseDanmyunAA;		// ������ �󼼵�-�ܸ�A-A ������
			ar >> m_dDeck_ScaleRebarSangseWaterReinforce;	// ����� ���� �󼼵�
			ar >> m_dDeck_ScaleRebarSangseCentReinforce;	// �߾������� ����ö�� ���� �󼼵�
			ar >> m_dDEck_ScaleRebarSangseDoubleChain;		// ö�� ������ �� ������
			ar >> m_dDeck_ScaleSangseA;					// ������ �󼼵�-A ������
			ar >> m_dDeck_ScaleSangseChairBlock;			// ������ �󼼵�-����ö�� �� chair block�� ������
			ar >> m_dDeck_SacleSangseShearReinforce;		// ���ܺ���ö�� ��
			ar >> m_dDeck_ScaleSangseShearArray;			// ������ �󼼵�-����ö�� ��ġ�� ������
			ar >> m_dDeck_ScaleSangseBlockOut;				// BLOCKOUT ��
			ar >> m_dDeck_ScaleSangseCrossBeam;				// ���κ� �󼼽�����
			ar >> m_dDeck_ScaleSangseStringer;				// ���κ� �󼼽�����
			ar >> m_dDeck_ScaleRebarMain;					// ��ö�� ������ ������
			ar >> m_dDeck_MainRebarCycle;					// ��ö�� ������ ����Ŭ��
			ar >> m_dDeck_ScaleRebarBelyck;					// ���ö�� ������ ������
			ar >> m_dDeck_BelyckCycle;						// ���ö�� ������ ����Ŭ��

			ar >> m_dDeck_SpaceBetweenJewon;				// ö�� ��������
			ar >> m_dDeck_DatasuOnRebarTable;				// ���̺�� �����ͼ�		
			ar >> m_dDeck_TypeRebarFrontLeft;				//������ Ⱦ�ܸ鵵 ��¿ɼ� 
			ar >> m_dDeck_TypeRebarFrontRigh;				//������ Ⱦ�ܸ鵵 ��¿ɼ� 

			ar >> m_dDeck_ScaleGuardRebar;					// ��ȣ��ö�� ������ 
			ar >> m_bDeckRebar_SangseRebar;					// ö�� ��(����Ʈ��)
			ar >> m_bDeckRebar_SangseGuard;					// ��ȣ�� ��(����Ʈ��)
			ar >> m_nDeck_JongDrawPos;						// ������ ���ܸ鵵 ��� ����
			ar >> m_nDeck_Slab_DivType;
			ar >> m_nDeck_Slab_RebarTableOpt;
			ar >> m_bDeck_ShowJointMainRebar;
			ar >> m_bDeck_ShowJointBeryukRebar;
		}

		//���鰣��ȭŸ��
		if(nFlag > 17)
		{
			ar >> m_nTypeRebarTable;						//ö�����ǥ Ÿ��
			ar >> m_dWidthRebarTableRect;					//ö�ٻ� Rect ��
			ar >> m_dHeightRebarTableRect;					//ö�ٻ� Rect ����
		}
		if(nFlag > 11)
		{
			// TU�Ŵ� ��ٵ�
			ar >> m_bTU_Plan;
			ar >> m_bTU_Jong;
			ar >> m_bTU_Detail;
			ar >> m_bTU_DetailSteel;
			ar >> m_bTU_TableSteel;
		}
		if(nFlag > 15)
			ar >> m_dTUScale;

		if(nFlag > 11)
		{
			ar >> m_dTUScale_Plan;	
			ar >> m_dTUScale_Jong;	
			ar >> m_dTUScale_Detail;
		}
		
		if(nFlag > 12)
			ar >> m_nTU_Trim;
		if(nFlag > 16)
			ar >> m_Det_bVStiffLeftRight;
	}
}
