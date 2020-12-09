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
	// 일반사항
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
	m_bTitleDogak			=				TRUE;		//도각 타이틀 부분 페이지 넣기 
	m_sHead_DomyunNumber	=				  "";
	m_bUsedHCad				= TRUE;

	// 강상형 일반도
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

	// 강상형 상세도
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
	m_Shoe_bCLSlabXY			=  TRUE;	 //교량받침배치도 좌표
	m_Det_bSolePlateJong		=  TRUE;
	m_Det_bPierAbutJong			= FALSE;
	m_Shoe_bCLSttEndXY			=  TRUE;	 //교량받침배치도 좌표
	m_Det_bVStiffJewon	        =  TRUE;
	m_Det_bVStiffAll			= FALSE;

	m_Det_nDivPage = 2;//거더 상세도 도면분할 방식 가로보 개수로 분할로...초기값 변경...070129..KB..

	m_Shoe_bSpanCenter	= TRUE;	 //교량받침배치도 치수
	m_Shoe_bTotalCenter	= TRUE;	 //교량받침배치도 치수

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
	m_Det_MaxSebuDetailSu		= 0;	//0일경우 모두 츨력 	

	m_DetailDim_Plan_Upper = DIM_BASE_FACTUPPER | DIM_BASE_SPLICE | DIM_BASE_TOTAL;
	m_DetailDim_Plan_Lower = DIM_BASE_DIAP      | DIM_BASE_FACTLOWER;
	m_DetailDim_Jong_Upper = DIM_BASE_WELDSTUD  | DIM_BASE_VSTIFF | DIM_BASE_FACTWEB | DIM_BASE_TOTAL;
	m_DetailDim_Jong_Lower = DIM_BASE_BYUN; 

	m_Gen_Scale.Add(100);
	m_Gen_Scale.Add(50);
	m_Gen_Scale.Add(800);
	m_Gen_Scale.Add(800);	

	// 수평 브레이싱
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


// 현장이음 상세도
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

	// 가로보 
	m_Cross_Summary				=  TRUE;	//경사도
	m_Cross_Pyung				=  TRUE;	//경사도
	m_Cross_Table				=  TRUE;	//치수테이블
	m_Cross_Front				=  TRUE;	//정면도
	m_Cross_Plan				=  TRUE;	//평면도

	m_Cross_PlanUpper			=  TRUE;	//평면도 상부
	m_Cross_PlanLower			=  TRUE;	//평면도 하부
	m_Cross_Sangse				=  TRUE;	//단면별 타입 상세도
	
	m_Cross_GassetUpperDetail	=  TRUE;	//브라켓 상부
	m_Cross_GassetLowerDetail	=  TRUE;	//브라켓 하부
	m_Cross_GassetSupportDetail	=  TRUE;	//브라켓 보강재
	m_Cross_VStiffDetail		=  TRUE;	//수직보강재
	m_Cross_HStiffDetail		=  TRUE;	//수평보강재
	m_Cross_AnchorBarDetail		=  TRUE;	//앵커바
	m_Cross_GassetScallopDetail	=  TRUE;	//브라켓 스캘럽
	m_Cross_BM					=  TRUE;	//재료표
	m_Cross_BKSupport			=  TRUE;	//브라켓 보강판 상세
	m_Cross_SpliceDetail        =  TRUE; 
	m_Cross_nRowSu				=    20;
	m_Cross_dPosHScale			=   200;
	m_Cross_dPosVScale			=   200;

	m_Shoe_bSttEndXY			=  TRUE;	//교량받침배치도 좌표 표시 변수
//	m_Cross_DrawDiap			=  TRUE;	//다이아프램 표시
	m_Shoe_bSlabXY				=  TRUE;	//교량받침배치도 좌표 표시 변수

	m_Cross_AllGirder			= FALSE;	//전체 거더 출력
	m_Cross_TwoDrawing			= FALSE;	//상세도 2장에 나눠서 출력
	m_Cross_KeyPlan				= TRUE;		//요약도 출력
	m_Cross_KeyPlanRoVer		= FALSE;	//요약도 수직회전
	m_Cross_KeyPlanSpliceMark	= TRUE;
	m_Cross_KeyPlanJijumMark	= TRUE;
	m_Cross_KeyPlanCrossBeam	= TRUE;
	m_Cross_KeyPlanDimension	= TRUE;
	m_Cross_KeyPlanDirection	= TRUE;
	m_Cross_KeyPlanOneLine		= TRUE;
	m_Cross_Type                = 1;        //단면별 출력

	//가로보 치수표 
	m_Cross_bSpliceMark			=  TRUE;
	m_Cross_bJiJumMark			=  TRUE;
	m_Cross_bCrossBeam			=  TRUE;
	m_Cross_bDimension			=  TRUE;
	m_Cross_bDirection			=  TRUE;
	m_Cross_bOneline			=  TRUE;
	m_Cross_bLineInfo			=  TRUE;

	m_CrossBeam_Scale.Add(30);	// 정면도 
	//
	m_CrossBeam_Scale.Add(30);	// //평면도 상부
	m_CrossBeam_Scale.Add(30);	// //평면도 하부
	m_CrossBeam_Scale.Add(10);	// //단면별 타입 상세도
	m_CrossBeam_Scale.Add(10);	// //복부이음판 상세
	m_CrossBeam_Scale.Add(5);	// 단면별 타입 브라켓 상세
	//
	m_CrossBeam_Scale.Add(10);	// 브라켓 상부
	m_CrossBeam_Scale.Add(10);	// 브라켓 하부
	m_CrossBeam_Scale.Add(10);	// 브라켓 보강재
	m_CrossBeam_Scale.Add(10);	// 수직보강재
	m_CrossBeam_Scale.Add(10);	// 수평보강재
	m_CrossBeam_Scale.Add(10);	// 앵커바 
	m_CrossBeam_Scale.Add(10);	// 브라켓 보강판 상세 
	m_CrossBeam_Scale.Add(10);	// 브라켓 스켈럽
	m_CrossBeam_Scale.Add(500);	// 요약도 수평
	m_CrossBeam_Scale.Add(500);	// 요약도 수직	

	// 슈배치도
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

	//솟음도;
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

	// 용접
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

	//수직브레이싱 상세도
	m_VBracing_Summary			= TRUE; 		
	m_VBracing_Pyung			= TRUE; 		
	m_VBracing_Table			= TRUE; 		
	m_VBracing_Plan				= TRUE; 			
	m_VBracing_Front			= TRUE; 		
	m_VBracing_Detail			= TRUE; 
	m_VBracing_AnchorBarDetail	= TRUE; 
	m_VBracing_BM				= TRUE; 				
	m_VBracing_AllGirder		= FALSE;
	m_VBracing_KeyPlan			= TRUE; 			//요약도 출력
	m_VBracing_KeyPlanRoVer		= FALSE;			//요약도 수직회전
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
	m_VBracing_Scale.Add(25);// 정면도 
	m_VBracing_Scale.Add(25);// 평면도
	m_VBracing_Scale.Add(10);// 수직브레이싱 상세
	m_VBracing_Scale.Add(10);// 앵커바 
	m_VBracing_Scale.Add(500);// 요약도 수평 
	m_VBracing_Scale.Add(500);// 요약도 수직
	m_VBracing_nRowSu			= 20;

	// 일반 평면도
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

	// 선형계획 제원도 : 도면 제어
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

	//단면요약도
	m_nSum_DivType	=   3;	// 도면 분할 방식(default 3: 전체교량을 한 장에)
	m_nSum_DivNum	=   1;	// 출력 단위 개수	
	m_dSum_Scale	= 300;	// 단면요약도 스케일
	m_dSum_MomentY	=   3;
	m_dSum_HeightY	= 1.8;
	m_dSum_FlangeY	=  25;

	// 도장 상세도
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

	// 슬래브 일반도
	m_bSlab_PlanView			=  TRUE;	// 평면도
	m_bSlab_JongView			=  TRUE;	// 측면도
	m_bSlab_FrontView			=  TRUE;	// 횡단면도
	m_bSlab_FrontViewLeft		=  TRUE;	// 횡단며도(좌측)
	m_bSlab_FrontViewRight		= FALSE;	// 횡단면도(우측)
	m_bSlab_FrontDetView		=  TRUE;	// 횡단면도 상세
	m_bSlab_SectionAA			=  TRUE;	// 거더부 단면 상세
	m_bSlab_SectionBB			=  TRUE;	// 박스부 단면 상세
	m_bSlab_SectionCC			=  TRUE;	// 박스부 중앙단면 상세
	m_bSlab_SectionDD			=  TRUE;	// 가로보 단면 상세
	m_bSlab_SectionEE			=  TRUE;	// 스트링거 상세
	m_bSlab_GuideWall			=  TRUE;	// 방호벽 상세
	m_bSlab_Notch				=  TRUE;	// NOTCH 상세
	m_bSlab_KeyPlan				=  TRUE;		// 요약도
	m_bSlab_bKeyPlanRoVer		= FALSE;	
	m_nSlab_DivType				=     0;		// 도면 분할 방식
	m_bSlab_UpperSlabLeftDim	=  TRUE;	// 다이아프램(좌)
	m_bSlab_UpperJiganLeftDim	=  TRUE;	// 상부 지간구성(좌)
	m_bSlab_LowerSlabRightDim	=  TRUE;	// 다이아프램(우)
	m_bSlab_LowerJiganRightDim	=  TRUE;	// 하부 지간구성(우)
	m_bSlab_MatchLine			=  TRUE;
	m_bSlab_bPyungLineInfo		=  TRUE;	//선형정보 

	m_dSlab_ScaleSlabGen	= 100;	// 슬래브 일반도 스케일
	m_dSlab_ScaleFrontDet	=  50;	// 횡단면도 상세 스케일
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

	// TENDON 배치도
	m_bTendon_PlanView       = TRUE;
	m_bTendon_SectionAA      = TRUE;
	m_nTendon_DivType        = 0;
	m_bTendon_MatchLine      = TRUE;
	m_dTendon_ScaleGen       = 100;
	m_dTendon_ScaleSectionAA = 30;
	m_nTendon_DivNum         = 1;

	m_nSlab_PlanView_DimOpt	 = 0;

	//슬래브 배근도
	m_bDeckRebar_PlanView				= TRUE;			// 평면도
	m_bDeckRebar_JongView				= TRUE;			// 측면도	
	m_bDeckRebar_FrontView				= TRUE;			// 횡단면도
	m_bDeckRebar_FrontViewCenter		= TRUE;			// 횡단면도(중앙부)
	m_bDeckRebar_FrontViewJijum			= TRUE;			// 횡단면도(지점부)	
	m_bDeckRebar_MainRebarView			= TRUE;			// 주철근 조립도
	m_bDeckRebar_BelyckView				= TRUE;			// 배력철근 조립도
	m_bDeckRebar_Sangse					= TRUE;			// 상세도 (상위트리)
	m_bDeckRebar_SangseDanbu			= TRUE;			//		슬래브 단부 상세도
	m_bDeckRebar_SangseDanmyunAA		= TRUE;			//		단면A-A 상세도
	m_bDeckRebar_SangseWaterReinforce	= TRUE;			//		배수구 보강상세도
	m_bDeckRebar_SangseCentReinforce	= TRUE;			//		중앙지점부 보강철근 이음상세도
	m_bDeckRebar_SangseDoubleChain		= TRUE;			//		철근 겹이음 상세도
	m_bDeckRebar_SangseA				= TRUE;			//		상세도-A
	m_bDeckRebar_SangseChairBlock		= TRUE;			//		상세도-조립철근 및 chair block상세
	m_bDeckRebar_SangseShearReinforce	= TRUE;			//		전단보강철근 상세
	m_bDeckRebar_SangseShearArray		= TRUE;			//		조립철근배치도
	m_bDeckRebar_SangseBlockOut			= TRUE;			//		Block Out 상세
	m_bDeckRebar_SangseCrossBeam		= TRUE;			//		가로보 상세
	m_bDeckRebar_SangseStringer			= TRUE;			//		세로보 상세
	
	m_bDeckRebar_SangseGuard			= TRUE;			// 방호벽 상세	(상위트리)
	m_bDeckRebar_GuardSection			= TRUE;			//		방호벽 단면도
	m_bDeckRebar_GuardDetail			= TRUE;			//		방호벽 상세도
	m_bDeckRebar_GuardTable				= TRUE;			//		방호벽 재료표

	m_bDeckRebar_SangseRebar			= TRUE;			// 철근상세 (상위트리)
	m_bDeckRebar_SangseMatrial			= TRUE;			//		철근상세도
	m_bDeckRebar_SangseMatrialTable		= TRUE;			//		철근상세표

	m_dDeck_ScaleRebarPlan					= 50;			// 슬래브 평면도 스케일
	m_dDeck_ScaleRebarJong					= 50;			// 슬래브 측면도 스케일
	m_dDeck_ScaleRebarFront					= 50;			// 슬래브 횡단면도 스케일
	m_dDeck_ScaleRebarSangseDanbu			= 30;			// 슬래브 상세도-단부상세도 스케일
	m_dDeck_ScaleRebarSangseDanmyunAA		= 30;			// 슬래브 상세도-단면A-A 스케일
	m_dDeck_ScaleRebarSangseWaterReinforce	= 20;			// 배수구 보강 상세도
	m_dDeck_ScaleRebarSangseCentReinforce	= 80;			// 중앙지점부 보강철근 이음 상세도
	m_dDEck_ScaleRebarSangseDoubleChain		= 80;			// 철근 겹이음 상세 스케일
	m_dDeck_ScaleSangseA					= 15;			// 슬래브 상세도-A 스케일
	m_dDeck_ScaleSangseChairBlock			= 80;			// 슬래브 상세도-조립철근 및 chair block상세 스케일
	m_dDeck_SacleSangseShearReinforce		= 50;			// 전단보강철근 상세
	m_dDeck_ScaleSangseShearArray			= 20;			// 슬래브 상세도-조립철근 배치상세 스케일
	m_dDeck_ScaleSangseBlockOut				= 80;			// BLOCKOUT 상세
	m_dDeck_ScaleSangseCrossBeam			= 20;			// 가로보 상세
	m_dDeck_ScaleSangseStringer				= 20;			// 세로보 상세
	m_dDeck_ScaleRebarMain					= 50;			// 주철근 조립도 스케일
	m_dDeck_MainRebarCycle					= 3;			// 주철근 조립도 사이클수
	m_dDeck_ScaleRebarBelyck				= 30;			// 배력철근 조립도 스케일
	m_dDeck_BelyckCycle						= 3;			// 배력철근 조립도 사이클수
	m_dDeck_SpaceBetweenJewon				= 1000;			// 철근 제원간격
	m_dDeck_DatasuOnRebarTable				= 70;			// 테이블당 데이터수
	m_dDeck_TypeRebarFrontLeft				= 0;			// 슬래브 횡단면도 출력위치 (지점부)
	m_dDeck_TypeRebarFrontRigh				= 2;			// 슬래브 횡단면도 출력위치 (없음)
	m_dDeck_ScaleGuardRebar					= 10;			// 방호벽 스케일 
	m_nDeck_JongDrawPos						= 0;			// 슬래브 종단면도 배근 형상	
	m_nDeck_Slab_DivType					= 0;			// 경간별 분할
	m_nDeck_Slab_RebarTableOpt				= 0;			// 철근재료표 일체로 표현
	m_bDeck_ShowJointMainRebar				= FALSE;
	m_bDeck_ShowJointBeryukRebar			= FALSE;
	m_nTypeRebarTable						= FALSE;		//철근재료표 타입(0:일반, 1:간소화)
	m_dWidthRebarTableRect					= 4000;			//철근상세 Rect 폭
	m_dHeightRebarTableRect					= 4000;			//철근상세 Rect 높이
	m_bTU_Plan			= TRUE;		// 평면도
	m_bTU_Jong			= TRUE;		// 종단면도
	m_bTU_Detail		= TRUE;		// 구속콘크리트 배근단면
	m_bTU_DetailSteel	= TRUE;		// 철근 상세도
	m_bTU_TableSteel	= TRUE;		// 철근 재료표
	m_nTU_Trim			= 0;		// 배근 Trim

	m_dTUScale			= 50;
	m_dTUScale_Plan		= 50;		// 평면도 스케일
	m_dTUScale_Jong		= 50;		// 종단면도 스케일
	m_dTUScale_Detail	= 20;		// 구속콘크리트 배근단면 스케일
}

void COptionStd::InitOptionBM()
{
	m_BM_OutSelect		= 0xFFFFFFFF;
	m_BM_PaintSelect	= 0xFFFFFFFF;
	m_BM_WeldSelect		= 0xFFFFFFFF;	
	m_BM_TableSu		=		   3;	// 한페이지당 테이블 수
	m_BM_LineSu			=		  50;	// 한테이블당 라인 수
	m_BM_Paint_TableSu	=		   2;	// 한페이지당 테이블 수
	m_BM_Paint_LineSu	=		  50;	// 한테이블당 라인 수
	m_BM_Weld_TableSu	=		   2;	// 한페이지당 테이블 수
	m_BM_Weld_LineSu	=		  50;	// 한테이블당 라인 수	
	m_nTypeJewonOut		=		   0;
}

void COptionStd::Serialize(CArchive &ar)
{
//	long nFlag = 4;		
//	long nFlag = 6;		// 2006.11.14
//	long nFlag = 8;		// 2006.12.07
//	long nFlag = 9;		// 2007.01.16 ......... 요약도 추가...KB...
//	long nFlag = 10;	// 2007.01.17 ......... 가로보상세치수표 추가...KB...
//	long nFlag = 11;	// 2007.04.03 ......... 가로보 상세도 2장에 나눠서 출럭...KB...
//	long nFlag = 12;	// 2008.02.01 ......... TU거더 배근도 추가...Lina...
//	long nFlag = 13;	// 2008.02.04 ......... TU거더 배근도 Trim옵션 추가...Lina...
//	long nFlag = 14;	// 2008.05.07 ......... 솟음도 테이블 높이조정옵션 추가...Lina
//	long nFlag = 15;	// 2008.08.14 ......... HCad Block 사용옵션 추가...Lina
//	long nFlag = 16;	// 2008.12.22 ......... TU거더 도각스케일 옵션 추가...Lina
//	long nFlag = 17;	// 2009.07.13 ......... 수직보강재 좌(우)표현 추가..LSH
    long nFlag = 18;    // 2013.01.18 ......... 도면간소화타입 추가..JJKim

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
		
		// 거더 일반도
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

		// 강상형 일반도 치수
		Serialint64(ar,m_GenDim_Plan_Upper);
		Serialint64(ar,m_GenDim_Plan_Lower);
		Serialint64(ar,m_GenDim_Jong_Upper);
		Serialint64(ar,m_GenDim_Jong_Lower);

		//강상형 상세도 ======================================================================
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
		ar << m_Det_bFieldWeldDetail; //2006.12.07일 추가...nFlag = 7
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

		//강상형 상세도 치수선 ===============================================================	
		Serialint64(ar, m_DetailDim_Plan_Upper); 
		Serialint64(ar, m_DetailDim_Plan_Lower); 
		Serialint64(ar, m_DetailDim_Jong_Upper); 
		Serialint64(ar, m_DetailDim_Jong_Lower); 

		// 수평 브레이싱
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

		// 현장이음 상세도
		ar << m_SP_bTopView;
		ar << m_SP_bWebView;
		ar << m_SP_bBottomView;
		ar << m_SP_bTopRib;
		ar << m_SP_bBottomRib;
		ar << m_SP_bSangSeRib;
		ar << m_SP_bHort;			//플랜지 수평 출력
		ar << m_SP_bSpliceFull;   // 전체거더 출력
		ar << m_SP_nJewon;
		ar << m_SP_Scale;
		ar << m_SP_bAllGirder;

		// 강재집계표
		ar << m_BM_OutSelect;		// 재료표 출력 선택
		ar << m_BM_PaintSelect;	// 재료표 출력 선택
		ar << m_BM_WeldSelect;	// 재료표 출력 선택
		ar << m_BM_TableSu;		// 한페이지당 테이블 수
		ar << m_BM_LineSu;		// 한테이블당 라인 수
		ar << m_BM_Paint_TableSu;		// 한페이지당 테이블 수
		ar << m_BM_Paint_LineSu;		// 한테이블당 라인 수
		ar << m_BM_Weld_TableSu;		// 한페이지당 테이블 수
		ar << m_BM_Weld_LineSu;		// 한테이블당 라인 수	
		ar << m_nTypeJewonOut;
		
		// 슈배치도
		ar << m_Shoe_dScaleHori;		// 슈배치도 수평스케일
		ar << m_Shoe_dScaleVert;		// 슈배치도 수직스케일
		ar << m_Shoe_bShoeCoord;		// 슈좌표도
		ar << m_Shoe_bTableCoord;		// 슈좌표테이블
		ar << m_Shoe_bTableLegend;	// 범례
		ar << m_Shoe_bTableShoeBM;	// 슈집계표
		ar << m_Shoe_bCoordZ;			//3차원 좌표출력
		ar << m_Shoe_bAngle;			//Angle 출력
		ar << m_Shoe_bLineInfo;
		ar << m_Shoe_bCLSlabXY; 
		ar << m_Shoe_bCLSttEndXY;
		ar << m_Shoe_bSlabXY;	
		ar << m_Shoe_bSttEndXY;
		ar << m_Shoe_bSpanCenter;	 //교량받침배치도 치수
		ar << m_Shoe_bTotalCenter; //교량받침배치도 치수

		ar << m_Shoe_dColumSu;
		ar << m_Elev_dScaleSection;	// ELEVATION 산출근거도 단면
		ar << m_Elev_dScaleShoeDetail;	// ELEVATION 산출근거도 교좌면상세
		ar << m_Elev_dScaleTable;		// ELEVATION 산출근거도 테이블
		ar << m_Elev_bSection;		// ELEVATION 산출근거도 단면
		ar << m_Elev_bShoeDetail;		// ELEVATION 산출근거도 교좌면상세
		ar << m_Elev_bTable;			// ELEVATION 산출근거도 테이블

		//가로보 상세도 
		ar << m_Cross_Summary;
		ar << m_Cross_Pyung;					//정면도 
		ar << m_Cross_Table;					//정면도 
		ar << m_Cross_Front;					//정면도 
		ar << m_Cross_Plan;					//평면도
		ar << m_Cross_GassetUpperDetail;		//브라켓 상부
		ar << m_Cross_GassetLowerDetail;		//브라켓 하부
		ar << m_Cross_GassetSupportDetail;	//브라켓 보강재
		ar << m_Cross_VStiffDetail;			//수직보강재
		ar << m_Cross_HStiffDetail;			//수평보강재
		ar << m_Cross_AnchorBarDetail;		//앵커바 
		ar << m_Cross_GassetScallopDetail;	//브라켓 스캘럽 	
		ar << m_Cross_BM;						//재료표 
		ar << m_Cross_BKSupport;				//브라켓 보강판 상세 		
		ar << m_Cross_nRowSu;					//테이블 줄수
//		ar << m_Cross_DrawDiap;				//다이아프램 표시 
		ar << m_CrossBeam_Scale;			//스케일 0, 1은 가로보 경사도 수평 , 수직 스케일 	

		ar << m_Cross_AllGirder;				//전체 거더 출력 
		ar << m_Cross_TwoDrawing;			//상세도 2장에 나눠서 출력
		ar << m_Cross_KeyPlan;				//요약도 출력
		ar << m_Cross_KeyPlanRoVer;			//요약도 수직회전		
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

		//가로보 치수표 
		ar << m_Cross_bJiJumMark;
		ar << m_Cross_bDimension;
		ar << m_Cross_bDirection;
		ar << m_Cross_bOneline;
		ar << m_Cross_bLineInfo;
		
		//솟음도
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

		//용접시험도
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

		//수직브레이싱 상세도
		ar << m_VBracing_Summary;		
		ar << m_VBracing_Pyung;			
		ar << m_VBracing_Table;			
		ar << m_VBracing_Plan;			
		ar << m_VBracing_Front;			
		ar << m_VBracing_Detail;
		ar << m_VBracing_AnchorBarDetail;
		ar << m_VBracing_BM;				
		ar << m_VBracing_AllGirder;
		ar << m_VBracing_KeyPlan;				//요약도 출력
		ar << m_VBracing_KeyPlanRoVer;			//요약도 수직회전
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

		// 일반 평면도
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

		// 선형제원도 : 도면 제어
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

		// 선형제원도 : SCALE
		ar << m_dLineInfo_PlaneScale;
		ar << m_dLineInfo_JongDanHScale;
		ar << m_dLineInfo_JongDanVScale;
		ar << m_dLineInfo_PyungubaeScale;
		ar << m_dLineInfo_PyungubaeRatio;

		//단면요약도
		ar <<   m_nSum_DivType;	//도면 분할 방식
		ar <<   m_dSum_Scale;
		ar <<   m_dSum_MomentY;
		ar <<   m_dSum_HeightY;
		ar <<   m_dSum_FlangeY;
		ar <<   m_nSum_DivNum;

		// 도장 상세도
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

		// 슬래브 일반도
		ar << 	m_bSlab_PlanView;	// 평면도
		ar << 	m_bSlab_JongView;	// 측면도
		ar << 	m_bSlab_FrontView; // 횡단면도
		ar <<	m_bSlab_FrontViewLeft;	// 횡단면도(좌측)
		ar <<	m_bSlab_FrontViewRight;	// 횡단면도(우측)
		ar << 	m_bSlab_FrontDetView; // 횡단면도 상세
		ar << 	m_bSlab_SectionAA;		// 거더부 단면 상세
		ar << 	m_bSlab_SectionBB;		// 박스부 단면 상세
		ar << 	m_bSlab_SectionCC;		// 박스부 중앙단면 상세
		ar << 	m_bSlab_SectionDD;		// 가로보 단면 상세
		ar << 	m_bSlab_SectionEE;		// 스트링거 상세
		ar << 	m_bSlab_GuideWall;			// 방호벽 상세
		ar << 	m_bSlab_Notch;			// NOTCH 상세
		ar << 	m_bSlab_KeyPlan;		// 요약도
		ar <<	m_bSlab_bKeyPlanRoVer;		
		ar << 	m_nSlab_DivType;		// 도면 분할 방식
		ar << 	m_bSlab_UpperSlabLeftDim;	// 다이아프램(좌)
		ar << 	m_bSlab_UpperJiganLeftDim;	// 상부 지간구성(좌)
		ar << 	m_bSlab_LowerSlabRightDim;	// 다이아프램(우)
		ar <<	m_bSlab_LowerJiganRightDim;	// 하부 지간구성(우)
		ar <<	m_bSlab_MatchLine;
		ar <<	m_bSlab_bPyungLineInfo;		//선형정보
		
		ar << 	m_dSlab_ScaleSlabGen;	// 슬래브 일반도 스케일
		ar << 	m_dSlab_ScaleFrontDet;	// 횡단면도 상세 스케일
		ar << 	m_dSlab_ScaleSectionAA;	
		ar << 	m_dSlab_ScaleSectionBB;
		ar << 	m_dSlab_ScaleSectionCC;
		ar << 	m_dSlab_ScaleSectionDD;
		ar << 	m_dSlab_ScaleSectionEE;
		ar << 	m_dSlab_ScaleGuideWall;
		ar <<	m_dSlab_ScaleNothch;
		ar <<	m_dSlab_ScaleKeyPlanHor;
		ar <<	m_dSlab_ScaleKeyPlanVer;
		ar <<	m_nSlab_DivNum;			// 출력 단위 개수
		ar <<	m_bSlab_DimTable;				// 치수변화 테이블
		ar <<	m_nSlab_DimTableType;
		ar <<	m_bSlab_DimTableFirstPage;
		ar <<	m_bSlab_Pyungubae;
		ar <<	m_dSlab_DimTableScale;
		ar <<	m_dSlab_DimTableHeightFactor;
		ar <<	m_dSlab_DimTableWidhtFactor;

		ar <<	m_bSlab_KeyPlanSpliceMark;			// 요약도
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

		ar << m_bDeckRebar_PlanView;					// 평면도
		ar << m_bDeckRebar_JongView;					// 측면도	
		ar << m_bDeckRebar_FrontView;					// 횡단면도
		ar << m_bDeckRebar_FrontViewCenter;				// 횡단면도(중앙부)
		ar << m_bDeckRebar_FrontViewJijum;				// 횡단면도(지점부)	
		ar << m_bDeckRebar_MainRebarView;				// 주철근 조립도
		ar << m_bDeckRebar_BelyckView;					// 배력철근 조립도
		ar << m_bDeckRebar_Sangse;						// 상세도 (상위트리)
		ar << m_bDeckRebar_SangseDanbu;					//		슬래브 단부 상세도
		ar << m_bDeckRebar_SangseDanmyunAA;				//		단면A-A 상세도
		ar << m_bDeckRebar_SangseWaterReinforce;		//		배수구 보강상세도
		ar << m_bDeckRebar_SangseCentReinforce;			//		중앙지점부 보강철근 이음상세도
		ar << m_bDeckRebar_SangseDoubleChain;			//		철근 겹이음 상세도
		ar << m_bDeckRebar_SangseA;						//		상세도-A
		ar << m_bDeckRebar_SangseChairBlock;			//		상세도-조립철근 및 chair block상세
		ar << m_bDeckRebar_SangseShearReinforce;		//		전단보강철근 상세
		ar << m_bDeckRebar_SangseShearArray;			//		조립철근배치도
		ar << m_bDeckRebar_SangseBlockOut;				//		Block Out 상세
		ar << m_bDeckRebar_SangseCrossBeam;				//		가로보 상세
		ar << m_bDeckRebar_SangseStringer;				//		세로보 상세
		ar << m_bDeckRebar_SangseMatrial;				//		철근상세도
		ar << m_bDeckRebar_SangseMatrialTable;			//		철근상세표
		ar << m_bDeckRebar_GuardSection;				//		방호벽 단면도
		ar << m_bDeckRebar_GuardDetail;					//		방호벽 철근상세
		ar << m_bDeckRebar_GuardTable;					//		방호벽 철근테이블 


		ar << m_dDeck_ScaleRebarPlan;					// 슬래브 평면도 스케일
		ar << m_dDeck_ScaleRebarJong;					// 슬래브 측면도 스케일
		ar << m_dDeck_ScaleRebarFront;					// 슬래브 횡단면도 스케일
		ar << m_dDeck_ScaleRebarSangseDanbu;			// 슬래브 상세도-단부상세도 스케일
		ar << m_dDeck_ScaleRebarSangseDanmyunAA;		// 슬래브 상세도-단면A-A 스케일
		ar << m_dDeck_ScaleRebarSangseWaterReinforce;	// 배수구 보강 상세도
		ar << m_dDeck_ScaleRebarSangseCentReinforce;	// 중앙지점부 보강철근 이음 상세도
		ar << m_dDEck_ScaleRebarSangseDoubleChain;		// 철근 겹이음 상세 스케일
		ar << m_dDeck_ScaleSangseA;						// 슬래브 상세도-A 스케일
		ar << m_dDeck_ScaleSangseChairBlock;			// 슬래브 상세도-조립철근 및 chair block상세 스케일
		ar << m_dDeck_SacleSangseShearReinforce;		// 전단보강철근 상세
		ar << m_dDeck_ScaleSangseShearArray;			// 슬래브 상세도-조립철근 배치상세 스케일
		ar << m_dDeck_ScaleSangseBlockOut;				// BLOCKOUT 상세
		ar << m_dDeck_ScaleSangseCrossBeam;				// 가로보 상세스케일			
		ar << m_dDeck_ScaleSangseStringer;				// 세로보 상세스케일			
		ar << m_dDeck_ScaleRebarMain;					// 주철근 조립도 스케일
		ar << m_dDeck_MainRebarCycle;					// 주철근 조립도 사이클수
		ar << m_dDeck_ScaleRebarBelyck;					// 배력철근 조립도 스케일
		ar << m_dDeck_BelyckCycle;						// 배력철근 조립도 사이클수
		ar << m_dDeck_SpaceBetweenJewon;				// 철근 제원간격
		ar << m_dDeck_DatasuOnRebarTable;				// 테이블당 데이터수
		ar << m_dDeck_TypeRebarFrontLeft;				// 슬래브 횡단면도 출력타입 
		ar << m_dDeck_TypeRebarFrontRigh;				// 슬래브 횡단면도 출력타입 
		ar << m_dDeck_ScaleGuardRebar;					// 방호벽철근 스케일 
		ar << m_bDeckRebar_SangseRebar;					// 철근 상세(상위트리)
		ar << m_bDeckRebar_SangseGuard;					// 방호벽 상세(상위트리)
		ar << m_nDeck_JongDrawPos;						// 슬래브 종단면도 배근 형상
		ar << m_nDeck_Slab_DivType;
		ar << m_nDeck_Slab_RebarTableOpt;
		ar << m_bDeck_ShowJointMainRebar;
		ar << m_bDeck_ShowJointBeryukRebar;
		ar << m_nTypeRebarTable;						//철근재료표 타입
		ar << m_dWidthRebarTableRect;					//철근상세 Rect 폭
		ar << m_dHeightRebarTableRect;					//철근상세 Rect 높이

		// TU거더 배근도
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
				
		// 강상형 일반도
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

		// 강상형 일반도 치수
		Serialint64(ar,m_GenDim_Plan_Upper);
		Serialint64(ar,m_GenDim_Plan_Lower);
		Serialint64(ar,m_GenDim_Jong_Upper);
		Serialint64(ar,m_GenDim_Jong_Lower);

		//강상형 상세도 ======================================================================
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
			ar >> m_Det_bFieldWeldDetail;//2006.12.07 추가 ..

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

		//강상형 상세도 치수선 ===============================================================	
		Serialint64(ar, m_DetailDim_Plan_Upper); 
		Serialint64(ar, m_DetailDim_Plan_Lower); 
		Serialint64(ar, m_DetailDim_Jong_Upper); 
		Serialint64(ar, m_DetailDim_Jong_Lower); 

		// 수평 브레이싱
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

		// 현장이음 상세도
		ar >> m_SP_bTopView;
		ar >> m_SP_bWebView;
		ar >> m_SP_bBottomView;
		ar >> m_SP_bTopRib;
		ar >> m_SP_bBottomRib;
		ar >> m_SP_bSangSeRib;
		ar >> m_SP_bHort;			//플랜지 수평 출력
		ar >> m_SP_bSpliceFull;   // 전체거더 출력
		ar >> m_SP_nJewon;
		ar >> m_SP_Scale;
		ar >> m_SP_bAllGirder;

		// 강재집계표
		ar >> m_BM_OutSelect;		// 재료표 출력 선택
		ar >> m_BM_PaintSelect;	// 재료표 출력 선택
		ar >> m_BM_WeldSelect;	// 재료표 출력 선택
		ar >> m_BM_TableSu;		// 한페이지당 테이블 수
		ar >> m_BM_LineSu;		// 한테이블당 라인 수
		ar >> m_BM_Paint_TableSu;		// 한페이지당 테이블 수
		ar >> m_BM_Paint_LineSu;		// 한테이블당 라인 수
		ar >> m_BM_Weld_TableSu;		// 한페이지당 테이블 수
		ar >> m_BM_Weld_LineSu;		// 한테이블당 라인 수	
		ar >> m_nTypeJewonOut;
		
		// 슈배치도
		ar >> m_Shoe_dScaleHori;		// 슈배치도 수평스케일
		ar >> m_Shoe_dScaleVert;		// 슈배치도 수직스케일
		ar >> m_Shoe_bShoeCoord;		// 슈좌표도
		ar >> m_Shoe_bTableCoord;		// 슈좌표테이블
		ar >> m_Shoe_bTableLegend;	// 범례
		ar >> m_Shoe_bTableShoeBM;	// 슈집계표
		ar >> m_Shoe_bCoordZ;			//3차원 좌표출력
		ar >> m_Shoe_bAngle;			//Angle 출력
		ar >> m_Shoe_bLineInfo;
		ar >> m_Shoe_bCLSlabXY; 
		ar >> m_Shoe_bCLSttEndXY;
		ar >> m_Shoe_bSlabXY;	
		ar >> m_Shoe_bSttEndXY;
		ar >> m_Shoe_bSpanCenter;	 //교량받침배치도 치수
		ar >> m_Shoe_bTotalCenter; //교량받침배치도 치수

		ar >> m_Shoe_dColumSu;
		ar >> m_Elev_dScaleSection;	// ELEVATION 산출근거도 단면
		ar >> m_Elev_dScaleShoeDetail;	// ELEVATION 산출근거도 교좌면상세
		ar >> m_Elev_dScaleTable;		// ELEVATION 산출근거도 테이블
		ar >> m_Elev_bSection;		// ELEVATION 산출근거도 단면
		ar >> m_Elev_bShoeDetail;		// ELEVATION 산출근거도 교좌면상세
		ar >> m_Elev_bTable;			// ELEVATION 산출근거도 테이블

		//가로보 상세도 
		ar >> m_Cross_Summary;
		ar >> m_Cross_Pyung;					//정면도 
		ar >> m_Cross_Table;					//정면도 
		ar >> m_Cross_Front;					//정면도 
		ar >> m_Cross_Plan;					//평면도
		ar >> m_Cross_GassetUpperDetail;		//브라켓 상부
		ar >> m_Cross_GassetLowerDetail;		//브라켓 하부
		ar >> m_Cross_GassetSupportDetail;	//브라켓 보강재
		ar >> m_Cross_VStiffDetail;			//수직보강재
		ar >> m_Cross_HStiffDetail;			//수평보강재
		ar >> m_Cross_AnchorBarDetail;		//앵커바 
		ar >> m_Cross_GassetScallopDetail;	//브라켓 스캘럽 	
		ar >> m_Cross_BM;						//재료표 
		ar >> m_Cross_BKSupport;				//브라켓 보강판 상세 		
		ar >> m_Cross_nRowSu;					//테이블 줄수
//		ar >> m_Cross_DrawDiap;				//다이아프램 표시 
		ar >> m_CrossBeam_Scale;			//스케일 0, 1은 가로보 경사도 수평 , 수직 스케일 	

		ar >> m_Cross_AllGirder;				//전체 거더 출력 
		if(nFlag>10)
			ar >> m_Cross_TwoDrawing;
		ar >> m_Cross_KeyPlan;					//요약도 출력
		ar >> m_Cross_KeyPlanRoVer;			//요약도 수직회전		
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
		//가로보 치수표 
		ar >> m_Cross_bJiJumMark;
		ar >> m_Cross_bDimension;
		ar >> m_Cross_bDirection;
		ar >> m_Cross_bOneline;
		ar >> m_Cross_bLineInfo;	
		
		//솟음도
		ar >> m_nCamber_Type;
		ar >> m_bCamber_Table;
		ar >> m_bCamber_AllGirder;
		ar >> m_bCamber_Note;
		ar >> m_bCamber_Legend;
		ar >> m_dCamber_HScale;
		ar >> m_dCamber_VScale;
		ar >> m_dCamber_TableHScale;
		if(nFlag < 14)
			m_dCamber_TableHScale = 1;	// 기존에 사용하지 않던 변수 재활용
		ar >> m_nCamber_DivType;
		ar >> m_nCamber_TableColSu;
		ar >> m_nCamber_UnitEa;
		ar >> m_bCamber_AllGirderFull;

		//용접시험도
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

		//수직브레이싱 상세도
		ar >> m_VBracing_Summary;		
		ar >> m_VBracing_Pyung;			
		ar >> m_VBracing_Table;			
		ar >> m_VBracing_Plan;			
		ar >> m_VBracing_Front;			
		ar >> m_VBracing_Detail;
		ar >> m_VBracing_AnchorBarDetail;
		ar >> m_VBracing_BM;				
		ar >> m_VBracing_AllGirder;
		ar >> m_VBracing_KeyPlan;				//요약도 출력
		ar >> m_VBracing_KeyPlanRoVer;			//요약도 수직회전
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

		// 일반 평면도
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

		// 선형제원도 : 도면 제어
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

		// 선형제원도 : SCALE
		ar >> m_dLineInfo_PlaneScale;
		ar >> m_dLineInfo_JongDanHScale;
		ar >> m_dLineInfo_JongDanVScale;
		ar >> m_dLineInfo_PyungubaeScale;
		ar >> m_dLineInfo_PyungubaeRatio;

		//단면요약도
		if (nFlag > 8)
		{
			ar >>   m_nSum_DivType;
			ar >>   m_dSum_Scale;
			ar >>   m_dSum_MomentY;
			ar >>   m_dSum_HeightY;
			ar >>   m_dSum_FlangeY;
			ar >>   m_nSum_DivNum;
		}		

		// 도장 상세도
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
			// 슬래브 일반도
			ar >> 	m_bSlab_PlanView;	// 평면도
			ar >> 	m_bSlab_JongView;	// 측면도
			ar >> 	m_bSlab_FrontView; // 횡단면도
			ar >> m_bSlab_FrontViewLeft;	// 횡단면도(좌측)
			ar >> m_bSlab_FrontViewRight;	// 횡단면도(우측)
			ar >> 	m_bSlab_FrontDetView; // 횡단면도 상세
			ar >> 	m_bSlab_SectionAA;		// 거더부 단면 상세
			ar >> 	m_bSlab_SectionBB;		// 박스부 단면 상세
			ar >> 	m_bSlab_SectionCC;		// 박스부 중앙단면 상세
			ar >> 	m_bSlab_SectionDD;		// 가로보 단면 상세
			ar >> 	m_bSlab_SectionEE;		// 스트링거 상세
			ar >> 	m_bSlab_GuideWall;			// 방호벽 상세
			ar >> 	m_bSlab_Notch;			// NOTCH 상세
			ar >> 	m_bSlab_KeyPlan;		// 요약도		
			ar >>	m_bSlab_bKeyPlanRoVer;			
			ar >> 	m_nSlab_DivType;		// 도면 분할 방식
			ar >> 	m_bSlab_UpperSlabLeftDim;	// 다이아프램(좌)
			ar >> 	m_bSlab_UpperJiganLeftDim;	// 상부 지간구성(좌)
			ar >> 	m_bSlab_LowerSlabRightDim;	// 다이아프램(우)
			ar >>	m_bSlab_LowerJiganRightDim;	// 하부 지간구성(우)		
			ar >> m_bSlab_MatchLine;
			ar >> m_bSlab_bPyungLineInfo;
			ar >> 	m_dSlab_ScaleSlabGen;	// 슬래브 일반도 스케일
			ar >> 	m_dSlab_ScaleFrontDet;	// 횡단면도 상세 스케일
			ar >> 	m_dSlab_ScaleSectionAA;	
			ar >> 	m_dSlab_ScaleSectionBB;
			ar >> 	m_dSlab_ScaleSectionCC;
			ar >> 	m_dSlab_ScaleSectionDD;
			ar >> 	m_dSlab_ScaleSectionEE;
			ar >> 	m_dSlab_ScaleGuideWall;
			ar >>	m_dSlab_ScaleNothch;
			ar >>	m_dSlab_ScaleKeyPlanHor;
			ar >>	m_dSlab_ScaleKeyPlanVer;
			ar >>	m_nSlab_DivNum;			// 출력 단위 개수

			ar >>	m_bSlab_DimTable;
			ar >>	m_nSlab_DimTableType;
			ar >>	m_bSlab_DimTableFirstPage;
			ar >>	m_bSlab_Pyungubae;
			ar >>	m_dSlab_DimTableScale;
			ar >>	m_dSlab_DimTableHeightFactor;
			ar >>	m_dSlab_DimTableWidhtFactor;
			ar >>	m_bSlab_KeyPlanSpliceMark;			// 요약도
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
			ar >> m_bDeckRebar_PlanView;					// 평면도
			ar >> m_bDeckRebar_JongView;					// 측면도	
			ar >> m_bDeckRebar_FrontView;				// 횡단면도
			ar >> m_bDeckRebar_FrontViewCenter;			// 횡단면도(중앙부)
			ar >> m_bDeckRebar_FrontViewJijum;			// 횡단면도(지점부)	
			ar >> m_bDeckRebar_MainRebarView;			// 주철근 조립도
			ar >> m_bDeckRebar_BelyckView;				// 배력철근 조립도
			ar >> m_bDeckRebar_Sangse;					// 상세도 
			ar >> m_bDeckRebar_SangseDanbu;				// 슬래브 단부 상세도
			ar >> m_bDeckRebar_SangseDanmyunAA;			// 단면A-A 상세도
			ar >> m_bDeckRebar_SangseWaterReinforce;		// 배수구 보강상세도
			ar >> m_bDeckRebar_SangseCentReinforce;		// 중앙지점부 보강철근 이음상세도
			ar >> m_bDeckRebar_SangseDoubleChain;		// 철근 겹이음 상세도
			ar >> m_bDeckRebar_SangseA;					// 상세도-A
			ar >> m_bDeckRebar_SangseChairBlock;			// 상세도-조립철근 및 chair block상세
			ar >> m_bDeckRebar_SangseShearReinforce;		// 전단보강철근 상세
			ar >> m_bDeckRebar_SangseShearArray;			// 조립철근배치도
			ar >> m_bDeckRebar_SangseBlockOut;			// Block Out 상세
			ar >> m_bDeckRebar_SangseCrossBeam;		// 가로보 상세
			ar >> m_bDeckRebar_SangseStringer;		// 세로보 상세
			ar >> m_bDeckRebar_SangseMatrial;			// 철근상세도
			ar >> m_bDeckRebar_SangseMatrialTable;		// 철근상세표

			ar >> m_bDeckRebar_GuardSection;				// 방호벽 단면도
			ar >> m_bDeckRebar_GuardDetail;					// 방호벽 철근상세
			ar >> m_bDeckRebar_GuardTable;					// 방호벽 철근테이블 

			ar >> m_dDeck_ScaleRebarPlan;					// 슬래브 평면도 스케일
			ar >> m_dDeck_ScaleRebarJong;					// 슬래브 측면도 스케일
			ar >> m_dDeck_ScaleRebarFront;					// 슬래브 횡단면도 스케일
			ar >> m_dDeck_ScaleRebarSangseDanbu;			// 슬래브 상세도-단부상세도 스케일
			ar >> m_dDeck_ScaleRebarSangseDanmyunAA;		// 슬래브 상세도-단면A-A 스케일
			ar >> m_dDeck_ScaleRebarSangseWaterReinforce;	// 배수구 보강 상세도
			ar >> m_dDeck_ScaleRebarSangseCentReinforce;	// 중앙지점부 보강철근 이음 상세도
			ar >> m_dDEck_ScaleRebarSangseDoubleChain;		// 철근 겹이음 상세 스케일
			ar >> m_dDeck_ScaleSangseA;					// 슬래브 상세도-A 스케일
			ar >> m_dDeck_ScaleSangseChairBlock;			// 슬래브 상세도-조립철근 및 chair block상세 스케일
			ar >> m_dDeck_SacleSangseShearReinforce;		// 전단보강철근 상세
			ar >> m_dDeck_ScaleSangseShearArray;			// 슬래브 상세도-조립철근 배치상세 스케일
			ar >> m_dDeck_ScaleSangseBlockOut;				// BLOCKOUT 상세
			ar >> m_dDeck_ScaleSangseCrossBeam;				// 가로보 상세스케일
			ar >> m_dDeck_ScaleSangseStringer;				// 세로보 상세스케일
			ar >> m_dDeck_ScaleRebarMain;					// 주철근 조립도 스케일
			ar >> m_dDeck_MainRebarCycle;					// 주철근 조립도 사이클수
			ar >> m_dDeck_ScaleRebarBelyck;					// 배력철근 조립도 스케일
			ar >> m_dDeck_BelyckCycle;						// 배력철근 조립도 사이클수

			ar >> m_dDeck_SpaceBetweenJewon;				// 철근 제원간격
			ar >> m_dDeck_DatasuOnRebarTable;				// 테이블당 데이터수		
			ar >> m_dDeck_TypeRebarFrontLeft;				//슬래브 횡단면도 출력옵션 
			ar >> m_dDeck_TypeRebarFrontRigh;				//슬래브 횡단면도 출력옵션 

			ar >> m_dDeck_ScaleGuardRebar;					// 방호벽철근 스케일 
			ar >> m_bDeckRebar_SangseRebar;					// 철근 상세(상위트리)
			ar >> m_bDeckRebar_SangseGuard;					// 방호벽 상세(상위트리)
			ar >> m_nDeck_JongDrawPos;						// 슬래브 종단면도 배근 형상
			ar >> m_nDeck_Slab_DivType;
			ar >> m_nDeck_Slab_RebarTableOpt;
			ar >> m_bDeck_ShowJointMainRebar;
			ar >> m_bDeck_ShowJointBeryukRebar;
		}

		//도면간소화타입
		if(nFlag > 17)
		{
			ar >> m_nTypeRebarTable;						//철근재료표 타입
			ar >> m_dWidthRebarTableRect;					//철근상세 Rect 폭
			ar >> m_dHeightRebarTableRect;					//철근상세 Rect 높이
		}
		if(nFlag > 11)
		{
			// TU거더 배근도
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
