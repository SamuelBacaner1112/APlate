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

// 평면도 부재 표현
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

// 측면도 출력 관련
#define DRAW_JONG_HSTIFF			0x000001
#define DRAW_JONG_VSTIFF			0x000002
#define DRAW_JONG_SPLICE			0x000004
#define DRAW_JONG_FACT				0x000008
#define DRAW_JONG_SOLEPLATE			0x000010
#define DRAW_JONG_JACKUP			0x000020
#define DRAW_JONG_SLAB				0x000040
#define DRAW_JONG_ALL				0xFFFFFF

// 부재정보 출력
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

	// 전체사항	
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

	// 거더 일반도
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
	DWORD	m_Gen_BujeInfo;		// 부재정보출력
	DWORD	m_Gen_PyungBuje;
	DWORD	m_Gen_JongBuje;

	CDoubleArray m_Gen_Scale;

	//강상형 일반도 치수선 ===============================================================	
	__int64 m_GenDim_Plan_Upper; 
	__int64 m_GenDim_Plan_Lower; 
	__int64 m_GenDim_Jong_Upper; 
	__int64 m_GenDim_Jong_Lower; 	
	
	//거더 상세도 ======================================================================
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
	BOOL	m_Det_bFieldWeldDetail; //현장용접상세 그리기 여부...
	BOOL	m_Det_bSolePlateDetail;
	BOOL	m_Det_bVStiffDetail;
	BOOL	m_Det_bHStiffDetail;	
	BOOL	m_Det_bLegend;
	BOOL	m_Det_bBasicNote;
	BOOL	m_Det_bBasicNoteVert;
	
	BOOL	m_Det_bGirderdan;
	BOOL	m_Det_bDiapHStiffPyung;	// 안씀
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

	//강상형 상세도 치수선 ===============================================================	
	__int64 m_DetailDim_Plan_Upper; 
	__int64 m_DetailDim_Plan_Lower; 
	__int64 m_DetailDim_Jong_Upper; 
	__int64 m_DetailDim_Jong_Lower; 

	// 수평 브레이싱
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

	// 현장이음 상세도
	BOOL    m_SP_bTopView;
	BOOL    m_SP_bWebView;
	BOOL    m_SP_bBottomView;
	BOOL    m_SP_bTopRib;
	BOOL    m_SP_bBottomRib;
	BOOL    m_SP_bSangSeRib;
	BOOL	m_SP_bHort;			//플랜지 수평 출력
	BOOL    m_SP_bSpliceFull;   // 전체거더 출력
	long	m_SP_nJewon;
	CDoubleArray m_SP_Scale;
	BOOL    m_SP_bAllGirder;

	// 강재집계표
	DWORD	m_BM_OutSelect;		// 재료표 출력 선택
	DWORD	m_BM_PaintSelect;	// 재료표 출력 선택
	DWORD	m_BM_WeldSelect;	// 재료표 출력 선택
	long	m_BM_TableSu;		// 한페이지당 테이블 수
	long	m_BM_LineSu;		// 한테이블당 라인 수
	long	m_BM_Paint_TableSu;		// 한페이지당 테이블 수
	long	m_BM_Paint_LineSu;		// 한테이블당 라인 수
	long	m_BM_Weld_TableSu;		// 한페이지당 테이블 수
	long	m_BM_Weld_LineSu;		// 한테이블당 라인 수	
	long	m_nTypeJewonOut;
	
	// 슈배치도
	double	m_Shoe_dScaleHori;		// 슈배치도 수평스케일
	double	m_Shoe_dScaleVert;		// 슈배치도 수직스케일
	BOOL	m_Shoe_bShoeCoord;		// 슈좌표도
	BOOL	m_Shoe_bTableCoord;		// 슈좌표테이블
	BOOL	m_Shoe_bTableLegend;	// 범례
	BOOL	m_Shoe_bTableShoeBM;	// 슈집계표
	BOOL	m_Shoe_bCoordZ;			//3차원 좌표출력
	BOOL	m_Shoe_bAngle;			//Angle 출력
	BOOL	m_Shoe_bLineInfo;
	BOOL	m_Shoe_bCLSlabXY; 
	BOOL	m_Shoe_bCLSttEndXY;
	BOOL	m_Shoe_bSlabXY;	
	BOOL	m_Shoe_bSttEndXY;
	BOOL	m_Shoe_bSpanCenter;	 //교량받침배치도 치수
	BOOL	m_Shoe_bTotalCenter; //교량받침배치도 치수

	long	m_Shoe_dColumSu;
	double	m_Elev_dScaleSection;	// ELEVATION 산출근거도 단면
	double	m_Elev_dScaleShoeDetail;	// ELEVATION 산출근거도 교좌면상세
	double	m_Elev_dScaleTable;		// ELEVATION 산출근거도 테이블
	BOOL	m_Elev_bSection;		// ELEVATION 산출근거도 단면
	BOOL	m_Elev_bShoeDetail;		// ELEVATION 산출근거도 교좌면상세
	BOOL	m_Elev_bTable;			// ELEVATION 산출근거도 테이블

	//가로보 상세도 
	BOOL	m_Cross_Summary;
	BOOL	m_Cross_Pyung;					//정면도 ??
	BOOL	m_Cross_Table;					//정면도 
	BOOL	m_Cross_Front;					//정면도 
	BOOL	m_Cross_Plan;					//평면도
	
	BOOL	m_Cross_PlanUpper;				//평면도 상부
	BOOL	m_Cross_PlanLower;				//평면도 하부
	BOOL	m_Cross_Sangse;					//연결부 상세 (예전:단면별 타입 상세도)

	BOOL	m_Cross_GassetUpperDetail;		//브라켓 상부
	BOOL	m_Cross_GassetLowerDetail;		//브라켓 하부
	BOOL	m_Cross_GassetSupportDetail;	//브라켓 보강재
	BOOL	m_Cross_VStiffDetail;			//수직보강재
	BOOL	m_Cross_HStiffDetail;			//수평보강재
	BOOL    m_Cross_SpliceDetail;           //복부 이음판 상세
	BOOL	m_Cross_AnchorBarDetail;		//앵커바 
	BOOL	m_Cross_GassetScallopDetail;	//브라켓 스캘럽 	
	BOOL	m_Cross_BM;						//재료표 
	BOOL	m_Cross_BKSupport;				//브라켓 보강판 상세 	
	BOOL	m_Cross_DrawDiap;				//다이아프램 표시 
	CDoubleArray m_CrossBeam_Scale;			//스케일 0, 1은 가로보 경사도 수평 , 수직 스케일 	

	BOOL	m_Cross_AllGirder;				//전체 거더 출력
	BOOL	m_Cross_TwoDrawing;				//상세도 2장에 나눠서 출력 
	BOOL	m_Cross_KeyPlan;				//요약도 출력
	BOOL	m_Cross_KeyPlanRoVer;			//요약도 수직회전
	BOOL	m_Cross_KeyPlanSpliceMark;
	BOOL	m_Cross_KeyPlanJijumMark;
	BOOL	m_Cross_KeyPlanCrossBeam;
	BOOL	m_Cross_KeyPlanDimension;
	BOOL	m_Cross_KeyPlanDirection;
	BOOL	m_Cross_KeyPlanOneLine;

	long    m_Cross_Type;                   //도면타입 0: 가로보별 1: 단면별
	
	//가로보 치수표 
	BOOL	m_Cross_bSpliceMark;			//현장이음 마크 표시
	BOOL	m_Cross_bJiJumMark;
	BOOL	m_Cross_bCrossBeam;
	BOOL	m_Cross_bDimension;
	BOOL	m_Cross_bDirection;
	BOOL	m_Cross_bOneline;
	BOOL	m_Cross_bLineInfo;	
	
	long	m_Cross_nRowSu;					//테이블 줄수
	double  m_Cross_dPosHScale;				//가로보치수표 수평 스케일
	double  m_Cross_dPosVScale;				//가로보치수표 수직 스케일

	//솟음도
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

	//용접시험도
	BOOL m_bWeldLeftGirder;			//거더출력옵션
	BOOL m_bWeldCamberLevel;		//측면도 레벨 적용
	BOOL m_bWeldKeyPlan;			//키플랜 출력 옵션
	BOOL m_bWeldKeyPlanSpliceMark;
	BOOL m_bWeldKeyPlanJijumMark;
	BOOL m_bWeldKeyPlanCrossBeam;
	BOOL m_bWeldKeyPlanDimension;
	BOOL m_bWeldKeyPlanDirection;
	BOOL m_bWeldKeyPlanOneLine;
	BOOL m_bWeldKeyPlanRoVer;	
	BOOL m_bWeldTypicalSection;		//TypicalSection 출력 옵션
	BOOL m_bWeldNote;				//노트 출력 옵션
	BOOL m_bWeldLineType;			//선타입 출력 옵션
	BOOL m_bWeldMarkSystem;			//마크시스템 출력 옵션
	BOOL m_bWeldDivDomyun;			//도면 분할방식 옵션
	BOOL m_bWeldDimText;			//치수 문자 표시 옵션
	long m_nWeldDanmyunScale;		//단면도 스케일 옵션
	long m_nWeldSangseScale;			//용접상세 스케일 옵션
	long m_nWeldTypicalScale;		//TypicalSection 스케일 옵션
	long m_nWeldKeyPlanHor;			//키플랜 수평 스케일 옵션
	long m_nWeldKeyPlanVer;			//키플랜 수직 스케일 옵션
	long m_nWeldDivBlock;			//블록 개수
	long m_nWeldDivDomyun;			//분할 현장이음 개수	
	long m_nWeldMark;				// 용접 마크 출력 옵션


	//단면요약도 ===============================================================
	long m_nSum_DivType;			// 도면 분할 방식
	long m_nSum_DivNum;				// 출력 단위 개수	
	double m_dSum_Scale;			// 단면요약도 스케일
	double m_dSum_MomentY;
	double m_dSum_HeightY;
	double m_dSum_FlangeY;
	
	// 도장 상세도
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

	//수직브레이싱 상세도
	BOOL m_VBracing_Summary;		
	BOOL m_VBracing_Pyung;			
	BOOL m_VBracing_Table;			
	BOOL m_VBracing_Plan;			
	BOOL m_VBracing_Front;			
	BOOL m_VBracing_Detail;			
	BOOL m_VBracing_AnchorBarDetail;
	BOOL m_VBracing_BM;				
	BOOL m_VBracing_AllGirder;		
	BOOL m_VBracing_KeyPlan;				//요약도 출력
	BOOL m_VBracing_KeyPlanRoVer;			//요약도 수직회전
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

	// 일반 평면도
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

	// 선형제원도
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

	// 슬래브 일반도
	BOOL m_bSlab_PlanView;	// 평면도
	BOOL m_bSlab_JongView;	// 측면도
	BOOL m_bSlab_FrontView; // 횡단면도
	BOOL m_bSlab_FrontViewLeft;  // 횡단면도(좌측)
	BOOL m_bSlab_FrontViewRight; // 횡단면도(우측)	
	BOOL m_bSlab_FrontDetView; // 횡단면도 상세
	BOOL m_bSlab_SectionAA;		// 거더부 단면 상세
	BOOL m_bSlab_SectionBB;		// 박스부 단면 상세
	BOOL m_bSlab_SectionCC;		// 박스부 중앙단면 상세
	BOOL m_bSlab_SectionDD;		// 가로보 단면 상세
	BOOL m_bSlab_SectionEE;		// 스트링거 상세
	BOOL m_bSlab_GuideWall;			// 방호벽 상세
	BOOL m_bSlab_Notch;			// NOTCH 상세
	BOOL m_bSlab_KeyPlan;		// 요약도
	BOOL m_bSlab_bKeyPlanRoVer;	
	long m_nSlab_DivType;		// 도면 분할 방식
	BOOL m_bSlab_UpperSlabLeftDim;	// 다이아프램(좌)
	BOOL m_bSlab_UpperJiganLeftDim;	// 상부 지간구성(좌)
	BOOL m_bSlab_LowerSlabRightDim;	// 다이아프램(우)
	BOOL m_bSlab_LowerJiganRightDim;	// 하부 지간구성(우)
	BOOL m_bSlab_MatchLine;		// 매치라인 표현
	BOOL m_bSlab_bPyungLineInfo;	//선형정보 

	double m_dSlab_ScaleSlabGen;	// 슬래브 일반도 스케일
	double m_dSlab_ScaleFrontDet;	// 횡단면도 상세 스케일
	double m_dSlab_ScaleSectionAA;	
	double m_dSlab_ScaleSectionBB;
	double m_dSlab_ScaleSectionCC;
	double m_dSlab_ScaleSectionDD;
	double m_dSlab_ScaleSectionEE;
	double m_dSlab_ScaleGuideWall;
	double m_dSlab_ScaleNothch;
	double m_dSlab_ScaleKeyPlanHor;
	double m_dSlab_ScaleKeyPlanVer;
	long m_nSlab_DivNum;			// 출력 단위 개수

	BOOL m_bSlab_DimTable;					// 치수변화 테이블
	long m_nSlab_DimTableType;
	BOOL m_bSlab_DimTableFirstPage;
	BOOL m_bSlab_Pyungubae;
	double m_dSlab_DimTableScale;
	double m_dSlab_DimTableHeightFactor;
	double m_dSlab_DimTableWidhtFactor;

	BOOL    m_bSlab_KeyPlanSpliceMark;		// 요약도
	BOOL    m_bSlab_KeyPlanJijumMark;
	BOOL    m_bSlab_KeyPlanCrossBeam;
	BOOL    m_bSlab_KeyPlanDimension;
	BOOL    m_bSlab_KeyPlanDirection;
	BOOL    m_bSlab_KeyPlanOneLine;

	// Notch
	double	m_dSlab_NotchHeight;		// Notch 제원	:	높이
	double	m_dSlab_NotchTopWidth;		//				:	폭(Top) 
	double	m_dSlab_NotchBotWidth;		//				:	폭(Bottom)
	long	m_nSlab_NotchType;			// Notch Type	: 0-직사각형, 1-제형

	// TENDON 배치도
	BOOL m_bTendon_PlanView;
	BOOL m_bTendon_SectionAA;
	long m_nTendon_DivType;
	BOOL m_bTendon_MatchLine;
	double m_dTendon_ScaleGen;
	double m_dTendon_ScaleSectionAA;
	long   m_nTendon_DivNum;

	// TurnOver 거더 배근도
	BOOL m_bTU_Plan;			// 평면도
	BOOL m_bTU_Jong;			// 종단면도
	BOOL m_bTU_Detail;			// 구속콘크리트 배근단면
	BOOL m_bTU_DetailSteel;		// 철근 상세도
	BOOL m_bTU_TableSteel;		// 철근 재료표
	long m_nTU_Trim;			// 0-배근도 Trim 1-Trim안함

	double m_dTUScale;
	double m_dTUScale_Plan;		// 평면도 스케일
	double m_dTUScale_Jong;		// 종단면도 스케일
	double m_dTUScale_Detail;	// 구속콘크리트 배근단면 스케일

	// 평면도치수 표현옵션
	long m_nSlab_PlanView_DimOpt;		// 평면도 치수 Type : 0-거더폭 표현, 1-거더간격 표현
	BOOL m_bDeckRebar_PlanView;					// 평면도
	BOOL m_bDeckRebar_JongView;					// 측면도	
	BOOL m_bDeckRebar_FrontView;				// 횡단면도
	BOOL m_bDeckRebar_FrontViewCenter;			// 횡단면도(중앙부)
	BOOL m_bDeckRebar_FrontViewJijum;			// 횡단면도(지점부)	
	BOOL m_bDeckRebar_MainRebarView;			// 주철근 조립도
	BOOL m_bDeckRebar_BelyckView;				// 배력철근 조립도
	BOOL m_bDeckRebar_Sangse;					// 상세도 (상위트리)
	BOOL m_bDeckRebar_SangseDanbu;				//		슬래브 단부 상세도
	BOOL m_bDeckRebar_SangseDanmyunAA;			//		단면A-A 상세도
	BOOL m_bDeckRebar_SangseWaterReinforce;		//		배수구 보강상세도
	BOOL m_bDeckRebar_SangseCentReinforce;		//		중앙지점부 보강철근 이음상세도
	BOOL m_bDeckRebar_SangseDoubleChain;		//		철근 겹이음 상세도
	BOOL m_bDeckRebar_SangseA;					//		상세도-A
	BOOL m_bDeckRebar_SangseChairBlock;			//		상세도-조립철근 및 chair block상세
	BOOL m_bDeckRebar_SangseShearReinforce;		//		전단보강철근 상세
	BOOL m_bDeckRebar_SangseShearArray;			//		조립철근배치도
	BOOL m_bDeckRebar_SangseBlockOut;			//		Block Out 상세
	BOOL m_bDeckRebar_SangseCrossBeam;			//		가로보 상세
	BOOL m_bDeckRebar_SangseStringer;			//		세로보 상세
	BOOL m_bDeckRebar_SangseRebar;				// 철근상세 (상위트리)
	BOOL m_bDeckRebar_SangseMatrial;			//		철근상세도
	BOOL m_bDeckRebar_SangseMatrialTable;		//		철근상세표
	BOOL m_bDeckRebar_SangseGuard;				// 방호벽 상세 (상위트리)
	BOOL m_bDeckRebar_GuardSection;				//		방호벽 단면도
	BOOL m_bDeckRebar_GuardDetail;				//		방호벽 철근상세
	BOOL m_bDeckRebar_GuardTable;				//		방호벽 철근테이블 

	double m_dDeck_ScaleRebarPlan;					// 슬래브 평면도 스케일
	double m_dDeck_ScaleRebarJong;					// 슬래브 측면도 스케일
	double m_dDeck_ScaleRebarFront;					// 슬래브 횡단면도 스케일
	double m_dDeck_ScaleRebarSangseDanbu;			// 슬래브 상세도-단부상세도 스케일
	double m_dDeck_ScaleRebarSangseDanmyunAA;		// 슬래브 상세도-단면A-A 스케일
	double m_dDeck_ScaleRebarSangseWaterReinforce;	// 배수구 보강 상세도
	double m_dDeck_ScaleRebarSangseCentReinforce;	// 중앙지점부 보강철근 이음 상세도
	double m_dDEck_ScaleRebarSangseDoubleChain;		// 철근 겹이음 상세 스케일
	double m_dDeck_ScaleSangseA;					// 슬래브 상세도-A 스케일
	double m_dDeck_ScaleSangseChairBlock;			// 슬래브 상세도-조립철근 및 chair block상세 스케일
	double m_dDeck_SacleSangseShearReinforce;		// 전단보강철근 상세
	double m_dDeck_ScaleSangseShearArray;			// 슬래브 상세도-조립철근 배치상세 스케일
	double m_dDeck_ScaleSangseBlockOut;				// BLOCKOUT 상세
	double m_dDeck_ScaleSangseCrossBeam;			// 가로보 상세
	double m_dDeck_ScaleSangseStringer;				// 세로보 상세
	double m_dDeck_ScaleRebarMain;					// 주철근 조립도 스케일
	double m_dDeck_MainRebarCycle;					// 주철근 조립도 사이클수
	double m_dDeck_ScaleRebarBelyck;				// 배력철근 조립도 스케일
	double m_dDeck_BelyckCycle;						// 배력철근 조립도 사이클수
	double m_dDeck_SpaceBetweenJewon;				// 철근 제원간격
	double m_dDeck_DatasuOnRebarTable;				// 테이블당 데이터수
	double m_dDeck_TypeRebarFrontLeft;				// 슬래브 횡단면도 타입(지점부 : 0, 일반부 : 1)
	double m_dDeck_TypeRebarFrontRigh;				// 슬래브 횡단면도 타입(지점부 : 0, 일반부 : 1)
	double m_dDeck_ScaleGuardRebar;					// 방호벽철근 스케일 
	long   m_nDeck_JongDrawPos;						// 슬래브 종단면도배근 형상 (슬래브중심 : 0, 슬래브좌측 : 1, 슬래브우측 : 2)	
	long   m_nDeck_Slab_DivType;					// 도면 분할 방식
	long   m_nDeck_Slab_RebarTableOpt;				// 철근재료표 표현방법
	long   m_nTypeRebarTable;						// 0 : 기본타입, 1 : 간소화타입
	BOOL   m_bDeck_ShowJointMainRebar;				// 겹이음장보기(주철근)
	BOOL   m_bDeck_ShowJointBeryukRebar;			// 겹이음장보기(배력철근)
	double m_dWidthRebarTableRect;					// 철근재료표 간소화 타입에서 철근상세 Rect하나의 폭
	double m_dHeightRebarTableRect;					// 철근재료표 간소화 타입에서 철근상세 Rect하나의 높이

};

#endif // !defined(AFX_OPTIONSTD_H__8B89B1AA_4BD1_4C99_95F8_1BDF997A69DA__INCLUDED_)


