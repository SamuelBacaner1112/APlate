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

#define FLOOR_STRENGTH_DESIGN		0		// 바닥판 중앙부 강도 설계
#define FLOOR_EXPERIENCE_DESIGN		1		// 바닥판 중앙부 경험적 설계

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
	double	m_dSttHDanPos;	// 횡단에서 방호벽 시작위치(슬래브 끝단에서)
	double	m_dEndHDanPos;	// 횡단에서 방호벽 끝위치(슬래브 끝단에서)
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
	CHLLLaneElement& GetElementSide(BOOL bLeft);	// 최외측 element를 return
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
		CString m_szBridgeType;        ///<교량형식:CBaseInfoDlg에서 저장
		CString m_szBridgeLength;      ///<교량연장:CJiganGusengDlg에서 저장
		CString m_szBridgeLineType;    ///<교량선형:CVBracingPositionDlg 저장
		double m_dBridgeWidth;         ///<교량폭  :CVBracingPositionDlg 
		double m_dBridgeSkew;          ///<사각    :CJijumAngleDlg
 	} DESIGN_BASE;

	struct _DESIGN_CONDITION
	{
		CString m_szBridgeGrade;       ///<교량등급						 :CBaseInfoDlg
		double m_dPf;                  ///<DB하중-Pf					 :CBaseInfoDlg
		double m_dPr;                  ///<DB하중-Pr					 :CBaseInfoDlg
		double m_dW;                   ///<DL하중-등분포하중			 :CBaseInfoDlg
		double m_dPm;                  ///<DL하중-집중하중(모멘트 계산시):CBaseInfoDlg
		double m_dPs;                  ///<DL하중-집중하중(전단력 계산시):CBaseInfoDlg
	} DESIGN_CONDITION;

	struct _DESIGN_MATERIAL 
	{
		double m_dSigmaCK;            ///<콘크리트 압축강도
		double m_dSigmaCi;			  ///<프리스트레스 도입시의 콘크리트 압축강도
		double m_dBindSigmaCK;		  ///<구속콘크리트 압축강도
		double m_dSigmaY;             ///<철근     인장응력 
		CString m_sSigmaY;			  ///400 MPa 이상 철근 표기 H~~,, 이하...D~~
		double m_dSigmaSA;			  ///<철근의 허용응력
		double m_dSigmaDeckSA;		  ///<철근의 허용응력
		CString m_szSwsJu;            ///<강재재질(주부재)
		CString m_szSwsBu;            ///<강재재질(부부재)
		double m_dEc;                 ///<콘크리트 탄성계수 
		double m_dEci;				  ///<콘크리트 탄성계수
		double m_dBindEc;			  ///<구속콘크리트 탄성계수
		double m_dEs;                 ///<철근 탄성계수
		double m_dEst;                ///<강재 탄성계수
		double m_dDummyEst;			  ///<더미부재 탄성계수
		long   m_nN;                  ///<바닥판 탄성계수비
		long   m_nBindNc;			  ///<구속콘크리트 탄성계수비
		double m_dEpsilonS;           ///<건조수축계수 
		double m_dAlpha;              ///<선팽창계수 
		double m_dCreep;              ///<크리프
		double m_dCreep2;
		double m_dTemparature;        ///<강과 콘크리트의 온도차 
		double m_dShearEs;			  ///<전단 탄성계수
		double m_RatePoasong;		  ///<콘크리트 포아슨비
		//허용응력
		CString	m_strAllow_PreStressPressPow;	//프리스트레스 도입 직후 압축응력(허용응력)
		CString	m_strAllow_PreStressStretchPow;	//프리스트레스 도입 직후 인장응력(허용응력)
		CString	m_strAllow_CreWeightPressPow;	//설계 하중 작용시 압축응력(허용응력)
		CString	m_strAllow_CreWeightStretchPow;	//설계 하중 작용시 인장응력(허용응력)
		CString	m_strAllow_DeckConcStretchPow;	//바닥판 허용 휨 인장응력(허용응력)
		CString	m_strCrackPow;					//균열응력

	} DESIGN_MATERIAL;

	struct _DESIGN_UNIT_WEIGHT
	{       ///<단위중량
		double m_dGangjae;			///< 강재 단위중량
		double m_dAluminum;			///< 알루미뉴 단위중량
		double m_dChulgun;			///< 철근콘크리트 단위중량
		double m_dMugun;			///< 무근뫁크리트 단위중량
		double m_dCement;			///< 시멘트 단위중량
		double m_dMokjae;			///< 목재 단위중량
		double m_dYukchung;			///< 역청재(방수용) 단위중량
		double m_dAsphalt;			///< 아스팔트 단위중량
		double m_dGravel;			///< 자갈,쇄석 단위중량
	} DESIGN_UNIT_WEIGHT;

	struct _DESIGN_FLOOR_DATA 
	{		
		///<방음벽
		BOOL   m_bBangEm;			///< 방음벽 점검
		double m_dBangEmWeight;		///< 방음벽 자중
		double m_dBangEmHeight;		///< 방음벽 높이
		double m_dBangEmDis;		///< 방음벽 재하거리(좌측끝단부터)
		///< 난간
		BOOL   m_bNanGan;			///< 난간 점검
		double m_dNanGanWeight;		///< 난간 자중
		double m_dNanGanDis;		///< 난간 재하거리		
		double m_dNanGanHeight;		///< 난간 높이
		///< 보도하중
		BOOL   m_bWalkLoad;	        ///< 보도 점검
		BOOL   m_bPeopleLoad;		///< 군집 하중
		double m_dWalkLoadWidth;	///< 보도 폭
		double m_dWalkLoad1;		///< 보도 시작부분 재하하중
		double m_dWalkLoad2;        ///< 보도 끝  부분 재하하중
		double m_dWalkLoadMoment;	///< 보도 모멘트 기준 재하 하중...070319...KB
		double m_dWalkLoadH;		///< 보도 높이
		double m_dWalkLoadLEdge[HLLMAXSU];///<보도 왼쪽 좌표(슬래브좌측끝단에서)
		double m_dWalkLoadREdge[HLLMAXSU];///<보도 오른쪽 좌표(슬래브좌측끝단에서)
		///<방호벽
		BOOL   m_bBangHo;				///<방호벽 점검
		double m_dBangHoWidth;			///<방호벽 폭 
		double m_dBangHoWeight;			///<방호벽 자중
		double m_dBangHoLEdge[HLLMAXSU];///<방호벽 왼쪽 좌표(슬래브좌측끝단에서)
		double m_dBangHoREdge[HLLMAXSU];///<방호벽 오른쪽 좌표(슬래브좌측끝단에서)
	} DESIGN_FLOOR_DATA[FLOOR_SIZE];

	struct _CALC_CANTILEVER_DATA
	{
		double m_Min_T;	///< 바닥판 최소 두께
		double m_Req_T;	///< 바닥판 필요 두께

		double	m_Stop_M;	///< 사하중 모멘트( Slab 포함 )
		double	m_Stop_Ton;	///< 사하중 크기  ( Slab 포함 )
		double	m_StopE_M;	///< 사하중 모멘트( Slab 제외 )
		double	m_StopE_Ton;///< 사하중 크기  ( Slab 제외 )
		double	m_StopB_M;	///< 사하중 모멘트(Bracket)
		
		double	m_Move_L;	///<플랜지반쪽의 중앙에서 Pr하중지점까지의 거리
		double  m_Dead_L;   ///<플랜지반쪽의 중앙에서 사하중지점까지의 거리
		double  m_Tank_L;	///<플랜지반쪽의 중앙에서 탱크하중 지점까지의 거리
		double  m_Trailer_L;///<플랜지반쪽의 중앙에서 트레일러 하중 지점까지의 거리

		double	m_Move_E;   ///<활하중에 의한 윤하중폭
		double	m_Dead_E;   ///<사하중에 의한 윤하중폭
		double	m_Move_E3;
		double	m_Move_i;   ///<충격계수
		double	m_Move_Pr;  ///<타이어하중
		double	m_Move_Ml;  ///<활하중에 의한 모멘트
		double	m_Move_Ml_;  ///<활하중에 의한 모멘트
		double	m_MoveB_Ml;
		double	m_MoveB_Ml_Tank;
		double	m_MoveB_Ml_Trailer;
		double	m_Move_Ml_Tank;///<탱크궤도하중에 의한 모멘트
		double	m_Move_Ml_Trailer;///<탱크트레일러하중에 의한 모멘트
		long	m_Move_GiJumsu;///<횡단면의 지점수(거더수와 동일함)
		double	m_Move_M;   ///<지점수에 따른 휨모멘트계수(지점수가 3개이상일때 휨모멘트에 0.8배)

		double	m_Coll_V;	///<충돌하중 속도
		double	m_Coll_H;   ///<충돌하중 작용높이
		double	m_Coll_Co;  ///<충돌하중
		double	m_Coll_Mo;  ///<충돌하중모멘트
		double  m_Nangan_VerF;///<난간에 작용하는 수직력(차선방향에..고로 정면충돌시)
		double  m_Nangan_HoriF;///<난간에 작용하는 수평력(옆으로 가다가 부디칠때)
		double  m_Nangan_dH; //난간에 작용하는 수평력과 곱해줄 높이
		double  m_Nangan_dL; //난간에 작용하는 수직력과 곱해줄 길이

		double	m_Wind_Pw;  ///<풍하중
		double	m_Wind_Mw;  ///<풍하중 모멘트
		double  m_Wind_H;   ///<픙하중 재하 높이

		double	m_Cent_R;			///<원심하중반경
		double	m_Cent_Pcf;			///<원심하중크기
		double	m_Cent_Pcf_Tank;	///<탱크원심하중크기
		double	m_Cent_Pcf_Trailer;	///<트레일러원심하중크기
		double	m_Cent_Mcf;			///<원심하중모멘트
		double	m_Cent_Mcf_Tank;	///<탱크원심하중모멘트
		double	m_Cent_Mcf_Trailer;	///<트레일러원심하중모멘트
		double	m_CentB_Mcf;
		double	m_CentB_Mcf_Tank;
		double	m_CentB_Mcf_Trailer;
		
		double	m_Rein_Mu;	  ///<극한모멘트
		double	m_Rein_Md;	  ///<계산된 철근량 모멘트 
		double  m_Rein_a;     ///<UseAs*fy/(k1*Ck*b)
		double  m_Rein_b;     ///<b
		double  m_Rein_d;     ///<m_Min_T-m_dCoverUp

		// 중앙부 바닥판 경험적 설계(중앙부만 사용)
		CString m_Rein_Up_CaseH1_1Cy;	// 철근지름(일반부 주철근 상면 1-Cycle)
		CString	m_Rein_Up_CaseH1_2Cy;	// 철근지름(일반부 주철근 상면 2-Cycle)
		long	m_Rein_Up_Combo1_1Cy;	// 일반부 주철근 상면 1-Cycle
		long	m_Rein_Up_Combo1_2Cy;	// 일반부 주철근 상면 2-Cycle
		double	m_Rein_Up_Ctc1;			// 철근간격(일반부 주철근 상면)
		double	m_Rein_Up_Asuse1_1Cy;	// 사용철근량(일반부 주철근 상면 1-Cycle)
		double	m_Rein_Up_Asuse1_2Cy;	// 사용철근량(일반부 주철근 상면 2-Cycle)
		double	m_Rein_Up_Asuse1;		// 사용철근량(일반부 주철근 상면)
		double	m_Rein_Up_Asreq1;		// 필요철근량(일반부 주철근 상면)
		CString	m_Rein_Up_CaseH2;		// 철근지름(일반부 배력철근 상면)
		long	m_Rein_Up_Combo2;		// 일반부 배력철근 상면
		double	m_Rein_Up_Ctc2;			// 철근간격(일반부 배력철근 상면)
		double	m_Rein_Up_Asuse2;		// 사용철근량(일반부 배력철근 상면)
		double	m_Rein_Up_Asreq2;		// 필요철근량(일반부 배력철근 상면)
		double	m_bCheck_Up;			// 일반부 주철근 상면 검토
		double	m_bCheck_UpB;			// 일반부 배력철근 상면 검토
		double	m_Rein_Lo_Asreq;		// 일반부 주철근 하면 필요철근량
		double	m_Rein_Lo_Asuse;		// 일반부 주철근 하면 사용철근량
		double	m_Rein_Lo_BAsreq;		// 일반부 배력철근 하면 필요철근량
		double	m_Rein_Lo_BAsuse;		// 일반부 배력철근 하면 사용철근량

		CString m_Rein_CaseH1;		// 철근지름(일반부 주철근 1-Cycle)
		CString m_Rein_CaseH1_2Cy;	// 철근지름(일반부 주철근 2-Cycle)
		CString m_Rein_CaseH2;		// 철근지름(단부   주철근 1-Cycle)
		CString m_Rein_CaseH2_2Cy;	// 철근지름(단부   주철근 2-Cycle)
		CString m_Rein_CaseH3;		// 철근지름(일반부 배력철근)
		CString m_Rein_CaseH4;		// 철근지름(단부   배력철근)
		int m_Rein_Combo1;			// 일반부 주철근Index 1-Cycle 
		int m_Rein_Combo1_2Cy;		// 일반부 주철근Index 2-Cycle
		int m_Rein_Combo2;			// 단부   주철근Index 1-Cycle
		int m_Rein_Combo2_2Cy;		// 단부   주철근Index 2-Cycle
		int m_Rein_Combo3;			// 일반부 배력철근
		int m_Rein_Combo4;			// 단부   배력철근
		double m_Rein_Ctc1;			// 철근간격(일반부 주철근)
		double m_Rein_Ctc2;			// 철근간격(단부   주철근)
		double m_Rein_CtcB;			// 철근간격(일반부 배력철근)
		double m_Rein_CtcB1;		// 철근간격(단부   배력철근)

		double m_Rein_UseAs;		// 사용철근량(일반부 주철근 합)
		double m_Rein_UseAs_1Cy;	// 사용철근량(일반부 주철근 1-Cycle)
		double m_Rein_UseAs_2Cy;	// 사용철근량(일반부 주철근 2-Cycle)
		double m_Rein_UseAs2;		// 사용철근량(단부   주철근 합)
		double m_Rein_UseAs2_1Cy;	// 사용철근량(단부   주철근 1-Cycle)
		double m_Rein_UseAs2_2Cy;	// 사용철근량(단부   주철근 2-Cycle)
		double m_Rein_UseAsBr;
		double m_Rein_BAsuse;		// 사용철근량(일반부 배력철근)
		double m_Rein_BAsuse2;		// 사용철근량(단부   배력철근)

		double m_Rein_Asreq;		// 필요철근량(일반부 주철근)
		double m_Rein_BAsreq;		// 필요철근량(일반부 배력철근)==>단부는 일반부의 2배사용
		
		double m_Rein_Pused;		// m_Rein_UseAs/(b*d);
		double m_Rein_dia;			// 철근치수에 따른 지름
		BOOL	m_bCheckGen1;		// 일반부 주철근   검토
		BOOL	m_bCheckGen2;		// 일반부 배력철근 검토
		BOOL	m_bCheckDan1;		// 단부   주철근   검토
		BOOL	m_bCheckDan2;		// 단부   배력철근 검토
		///<균열검토
		double m_Crack_w;     ///<균열폭
		double m_Crack_wa;    ///<허용균열폭ㅋ
		double m_Crack_wBr;   ///<균열폭 - Bracket
		double m_Crack_waBr;  ///<허용균열폭 - Bracket

		/*
		CString	m_Rein_CaseH_T;	///< 철근지름(일반부 인장) - 소수주형
		int		m_Rein_Combo_T;	///< H19일반부 인장
		double	m_Rein_Ctc_T;	///< 일반부 인장 철근간격
		double	m_Rein_UseAs_T;	///< 일반부 인장 사용철근량

		CString	m_Rein_CaseH_C;	///< 철근지름(일반부 압축) - 소수주형
		int		m_Rein_Combo_C;	///< H19일반부 압축
		double	m_Rein_Ctc_C;	///< 일반부 압축 철근간격
		double	m_Rein_UseAs_C;	///< 일반부 압축 사용철근량
		*/
		// 소수주형교량 철근
		CString	m_strGenReinTDia_1Cy;		// 철근지름 일반부 주철근 인장부 1Cycle
		CString	m_strGenReinTDia_2Cy;		// 철근지름 일반부 주철근 인장부 2Cycle
		long	m_nGenReinTDiaIdx_1Cy;		// 철근지름 일반부 주철근 인장부 1Cycle
		long	m_nGenReinTDiaIdx_2Cy;		// 철근지름 일반부 주철근 인장부 2Cycle
		double	m_dGenReinTCTC;				// 철근간격 일반부 주철근 인장부
		double	m_dGenReinTAsreq;			// 철근지름 일반부 주철근 인장부 필요철근량
		double	m_dGenReinTAsuse;			// 철근지름 일반부 주철근 인장부 사용철근량
		double	m_dGenReinTAsuse_1Cy;		// 철근지름 일반부 주철근 인장부 사용철근량 1Cycle
		double	m_dGenReinTAsuse_2Cy;		// 철근지름 일반부 주철근 인장부 사용철근량 2Cylcle

		CString	m_strGenReinCDia_1Cy;		// 철근지름 일반부 주철근 압축부 1Cycle
		CString	m_strGenReinCDia_2Cy;		// 철근지름 일반부 주철근 압축부 2Cycle
		long	m_nGenReinCDiaIdx_1Cy;		// 철근지름 일반부 주철근 압축부 1Cycle
		long	m_nGenReinCDiaIdx_2Cy;		// 철근지름 일반부 주철근 압축부 2Cycle
		double	m_dGenReinCCTC;				// 철근간격 일반부 주철근 압축부
		double	m_dGenReinCAsreq;			// 철근지름 일반부 주철근 압축부 필요철근량
		double	m_dGenReinCAsuse;			// 철근지름 일반부 주철근 압축부 사용철근량
		double	m_dGenReinCAsuse_1Cy;		// 철근지름 일반부 주철근 압축부 사용철근량 1Cycle
		double	m_dGenReinCAsuse_2Cy;		// 철근지름 일반부 주철근 압축부 사용철근량 2Cylcle

	} CALC_CANTILEVER_DATA[FLOOR_SIZE];

	struct _DESIGN_STEELPRESTRESS_DATA
	{
		long	m_nSteelPrestressIdx;							///< 사용 Pressstress 강선의 번호
		CString	m_szSteelPrestressName;							///< 사용 Pressstress 강선의 호칭
		double	m_dEs;											///< 탄성계수
		double	m_dArea;										///< 공칭 단면적
		double	m_dDiameter;									///< 지름
		double	m_dLoadOfElongation;							///< 연신하중
		double	m_dTensionLoad;									///< 인장하중
		double	m_dTensionStrength;								///< 인장강도
		double	m_dYieldStrength;								///< 항복강도		
		double	m_dWobbleFriction;							///< 파상마찰계수
		double	m_dCurvatureFriction;							///< 곡률마찰계수
		long	m_nDuctType;									///< 1:금속쉬스 2:아연도금 금속쉬스 3:아스팔트또는그리스코팅 4:아연도금된강성덕트
		long	m_nPrestressQty;								///< PS강재 갯수
		double	m_dPrestressCTC;								///< 간격
//		double	m_dSttAnchorageSlip;							///< 시점부 정착구 활동량-2009에서 사용안함
//		double	m_dEndAnchorageSlip;							///< 종점부 정착구 활동량-2009에서 사용안함
		double	m_dAnchorageSlip;								///< 정착구 활동량 
		double	m_dDuctDiameter;								///< 덕트 직경
		double	m_dHumidity;									///< 평균상대습도		
		long	m_nVerRein;										///< 수직철근 combo		
		double	m_dVerReinAs;									///< 수직철근량		
		long	m_nVerReinQty;									///< 수직철근수		
		double	m_dFixingPlateLength;							///< 정착판의 길이

		//기존꺼 사용
		long	m_nHorReinQty1;									///< 수평 정착부 철근수
		long	m_nHorReinQty2;									///< 수평 정착부 보강 철근수
		long	m_nFixingPlateType;								///< 정착판의 종류
		long	m_nHorRein1;									///< 수평 정착부 철근 combo
		long	m_nHorRein2;									///< 수평 정착부 보강 철근 combo
		double	m_dHorReinAs1;									///< 수평 정착부 철근량
		double	m_dHorReinAs2;									///< 수평 정착부 보강 철근량
		//신규...060831	
		long	m_nAnchorageEA;			//정착판당 정착구
		double  m_dAnchorageWidth;		//정착판 폭원
		double  m_dAnchorageHeight;		//정착판 높이
		double  m_dSpallingForce;		//T1 정착구 배면 할력력
		double  m_dBurstingForce;		//T2 정착구 앞부분의 파열력
		double  m_dReqAs1;				//T1의 
		double  m_dUseAs1;				//T1의 
		double  m_dReqAs2;				//T2의 
		double  m_dUseAs2;				//T2의
		double  m_dReinCTC;				//철근간격
		CString m_sCaseH1;
		CString m_sCaseH2;
		double  m_dPj;				//정착부의 국부인장력 계산

		double	m_dPSSteelYP;	//PS 강재의 종류에 따른 계수(γp)
		double	m_dSheathW;		//쉬스관 폭
		double	m_dSheathH;		//쉬스관 높이
		CString	m_sJackMax;		//Jack에 의한 최대 긴장응력

		//신규...090723
		long	m_nVerReinQty2;									///< 수직 정착부 보강 철근수
		long	m_nVerRein2;									///< 수직 정착부 보강 철근 combo

	} DESIGN_STEELPRESTRESS_DATA;

	int m_nLBDeckDiaIdx;					// LB-Deck 직경 index
	double m_dLBDeckCTC;					// LB-Deck CTC
	double m_dLBDeckCover;					// LB-Deck 피복
	
	double  m_Pib;		  ///<휨에 대한 강도 감소 계수
	double  m_Pis;		  ///<전단에 대한 강도 감소 계수
	double  m_dCoverUp;   ///<콘크리트피복두께(상부)
	double  m_dCoverDn;   ///<콘크리트피복두께(하부)
	double  m_dCoverSd;   ///<콘크리트피복두께(측면)
	double  m_dRebarPmax; ///<0.75*Pb;
	double m_Rein_Arrange;///<단부철근의 배치범위
	///< CGiJumJujngDlg (바닥판 보강+주장률 검토)
	double m_Asreq;       ///<내부지점부 바닥판 교축방향 필요 철근량
	double m_Ag;          ///<콘크리트 단면적
	double m_AsEa1;       ///<철근갯수1
	double m_AsEa2;       ///<철근갯수2 
	double m_Asuse1;      ///<철근갯수1
	double m_Asuse2;	  ///<철근량1	
	double m_Asuset;      ///<철근량2 
	double m_Ju_ratio;    ///<주장률
	double m_dia1;        ///<철근지름1
	double m_dia2;        ///<철근지름2
	double m_Ctc1;        ///<철근간격1
	double m_Ctc2;        ///<철근지름2
	CString m_CaseH1;     ///<철근치수1
	CString m_CaseH2;     ///<철근치수2
	int m_ComboInnerJijumSlab1;///<철근 호칭콤보1
	int m_ComboInnerJijumSlab2;///<철근 호칭콤보2
	BOOL m_bCheckInnerJijum;	// 내부지점부 철근량
	long m_nEndPartRebarConditionIdx; // 중앙부 단부 검토시 캔틸레버부 철근에 대한 필요 비율 idx; 0 - 1배 , 1 - 2배 , 추후 추가시 기록할 것

	double m_dSpeed;				///< 설계 속도
	double m_dWindLoadForce;		///< 풍하중 강도
	double m_dRadiusOfCurvature;	///< 곡률반지름
	BOOL m_bTank;					///< 탱크하중적용
	BOOL m_bTrailer;				///< 트레일러하중적용
	BOOL m_bMugun;					///< 방호벽 바닥판 무근콘크리트단위중량	
	BOOL m_bJungbunMugun;			///< 중분대 바닥판 무근콘크리트 적용여부 >
	BOOL m_bExtraLoad;				///< 부가하중 점검
	BOOL m_bWind;					///< 풍하중적용
	CStringArray m_ELoadTitleArr;	///< 부가하중 Title
	CDoubleArray m_ELoadWeightArr;	///< 부가하중 자중
	CDoubleArray m_ELoadDisArr;		///< 부가하중 재하거리(좌측끝단부터)	
	CStringArray m_szRefArray;		///< 참고문헌

	BOOL m_bIsFirst;				///< bri 로딩 후 첫 실행인가?
	BOOL m_bHLLError;				///< 방호벽의 수가 3개 이상일때 Error
	BOOL m_bLWalkHLL;				///< 횡분배에 군중하중(왼쪽) 포함되었음
	BOOL m_bRWalkHLL;				///< 횡분배에 군중하중(오른쪽) 포함되었음
	BOOL m_bNoPojang;				///< 육교bri(횡단면도상에 보도하중만 있음)

	CDoubleArray m_dStationHLLPos;	///<횡분배위치 스테이션
	long m_HLType;					///< 횡분배 타입
	long m_HLsu;					///< 횡분배 수
	long m_nHLiveLoadType;			///< 횡분배시 활하중 재하 방법
	BOOL m_bNoDistOnNoGuard;		///< 연석없는 구간 단부적용

	BOOL m_bCheckSettlement;		///< 지점침하모델링을 하는가?
	BOOL m_bPassedCalcFloorDlg;		///< [바닥판 설계고려사항입력]을 지났는가?
	BOOL m_bRunCross;				///< 횡분배를 계산하였는가?
	BOOL m_bRunCrossWind;			///< 풍하중 데이터를 읽었는가?
	BOOL m_bRunInfluence;			///< 영향계수 모델링을 실행했나
	BOOL m_bMakeModeling;			///< 모델링을 하였는지?(이유: s2k를 읽은후 모델링을 하면 기존의 데이터가 없어지게 됨)
	BOOL m_bReadFrameModelingOutput;///< 최종모델링 데이터를 읽었는가?
	BOOL m_bReadLiveDispOutput;     ///< 활하중처짐 텍스트값을 읽었는가?

	CString m_szFolderPath;		    ///< sap s2k파일의 폴더(모델링)
	CStringArray m_sz3DFrameOutFilePath;// 3D Frame Output 파일 패스


	long m_CurHL;					///< ComboBox 현재 선택 사항(횡분배 Index)					
	long m_CurLiveHL;				///< 모델링 재하시의 활하중 기준위치 횡분배 Index 
	
	double m_dDesignLaneWidth;		///< 설계 차로폭 ( <= 3.6m )
	double m_dUserLaneWidth;		///< 설계 차로폭 (사용자 정의)
	CDDWordArray	m_nArrDesignLane;	// 설계차선 수
 	long   m_DesignChasunSu;		///< 설계차선 수(중분대 왼쪽 or 중분대 없을때) 
	long   m_DesignChasunSuR;		///< 설계차선 수(중분대 오른쪽)

	long m_AddRowTRL;				///< 보도하중의 추가에 따른 ActionGroup번호 증가치
	double m_MaxImpact;				///< 종방향 활하중 충격계수값 중 가장 큰값
	double m_DBWidth;				///< DB 차폭        = 1.8
	double m_DBFromEdge;			///< DB 연석으로부터의 거리 = 0.3
	double m_DBDis;					///< DB 차간   거리 = 1.2
	double m_DLDis;					///< DL 차선간 거리 = 1.3

	///< Tank & Tailer 정보
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

	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexPyun;	///< 횡분배(편경사변화위치[가로보에서 체크])
	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexWidth;///< 횡분배(교량폭변화위치[가로보에서 체크])
	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexPyunWidth;///< 횡분배(편경사변화+교량폭변화위치[가로보에서 체크])
	CTypedPtrArray < CObArray, CPlateBasicIndex* >	m_DArrHLBasicIndexRun;	///< 횡분배(실제 횡분배를 수행할 위치의 bx)

	BOOL m_bLoadLeftRight;                                  ///< 횡분배 계산시 사용

	CDDWordArray m_DArrDistanceDB[HLLMAXSU];				///< 횡방향 활하중(왼쪽부터 DB하중까지의 거리)
	CDDWordArray m_DArrDistanceDL[HLLMAXSU];				///< 횡방향 활하중(왼쪽부터 DL하중까지의 거리)

	CDDWordArray m_DArrDBChasun[HLLMAXSU];					///< 횡방향 활하중(차선 번호)
	CDDWordArray m_DArrDLChasun[HLLMAXSU];					///< 횡방향 활하중(차선 번호)

	CDDWordArray m_DArrMaxDBChasun[HLLMAXSU];				///< DB최대 차선수
	CDDWordArray m_DArrMaxDLChasun[HLLMAXSU];				///< DL최대 차선수

	CDDWordArray m_DArrDistanceTank[HLLMAXSU];				///< Tank 하중 (왼쪽부터의 거리)
	CDDWordArray m_DArrDistanceTrail[HLLMAXSU];				///< Trailer 하중 (왼쪽부터의 거리)

	// 풍하중 수평력
	double m_dLeftWindFroceFromL[JIJUMSUMAX];				// 좌측,바람방향 -->
	double m_dLeftWindFroceFromR[JIJUMSUMAX];				// 좌측,바람방향 <--
	double m_dRightWindFroceFromL[JIJUMSUMAX];				// 우측,바람방향 -->
	double m_dRightWindFroceFromR[JIJUMSUMAX];				// 우측,바람방향 <--
	double m_dWindForceToGirderL[JIJUMSUMAX];				// 활하중재하시 거더측면에 작용하는 하중
	double m_dWindForceToGirderR[JIJUMSUMAX];				// 활하중재하시 거더측면에 작용하는 하중
	// 풍하중 모멘트
	double m_dLeftWindMomentFromL[JIJUMSUMAX];				// 좌측,바람방향 -->
	double m_dLeftWindMomentFromR[JIJUMSUMAX];				// 좌측,바람방향 <--
	double m_dRightWindMomentFromL[JIJUMSUMAX];				// 우측,바람방향 -->
	double m_dRightWindMomentFromR[JIJUMSUMAX];				// 우측,바람방향 <--

	double  m_AddLoad;										///< 부속설비 하중(kN/m2)
	double  m_TotalWeight;									///< 총 강재량(kN)
	double  m_ModelWeight;									///< 모델링에 재하된 강재량
	double  m_BonusRate;									///< 부부재 할증율(모델링 강재 자중)
	double  m_JijumDownSize;								///< 지점침하량
	long	m_nModelingSettleMethod;						///< 지점침하 하중조합
	BOOL	m_bApplyAddRow;									///< 합성후 사하중 : 사용자 입력(ActionGroupMat 번호 밀리기?적용)
	BOOL    m_bPic;											///< 엑셀 출력시 그림 추가 여부
	BOOL	m_bBucklingStress;								///< 상판 허용응력 국부좌굴고려 여부
	CStringArray m_ArrExtraSecTitle;						// 사용자 단면 검토 추가(타이틀) 
	CDWordArray  m_ArrExtraSecGir;							// 사용자 단면 검토 추가(거더인덱스) 
	CDDWordArray m_ArrExtraSecSta;							// 사용자 단면 검토 추가(Sta) 
	double m_dMatMinSpace;									// 사용자 단면 검토 최소길이 
	double m_dMatMaxSpace;									// 사용자 단면 검토 최대길이
	// 단면응력도에서 사용.
	CDWordArray  m_ArrMomentZeroSta;						// 교번구간 위치
	CDWordArray  m_ArrMinZeroSta;							// 교번구간 Min 위치
	CDWordArray  m_ArrMaxZeroSta;							// 교번구간 Miax위치

	CVectorArray m_ArrSec;									// 전체 교량에 대한 정렬된 검토 단면(x: 거더번호  y: 스테이션  z: 단면번호)
	CVectorArray m_ArrSecSpDesign;							// 전체 교량중 현장이음 검토위치(5. 이음부 설계시 사용됨)	

	CVectorArray m_ArrSecHBeam;								// H형강형태
	CVectorArray m_ArrSecHBeamLSp;							// H형강형태(좌측이 이음판 연결인경우)
	CVectorArray m_ArrSecHBeamRSp;							// H형강형태(우측이 이음판 연결인경우)
	CVectorArray m_ArrSecHBeamLRSp;							// H형강형태(좌, 우측이 이음판 연결인경우)
	CVectorArray m_ArrSecCross;								// 모멘트이음판이 없는 가로보위치
	CVectorArray m_ArrSecCrossMoment;						// 모멘트이음판이 있는 가로보위치		
	CVectorArray m_ArrSecCrossWithStg;						// 모멘트이음판이 없는 세로보딸린 가로보위치
	CVectorArray m_ArrSecCrossWithStgMoment;				// 모멘트이음판이 있는 세로보딸린 가로보위치
	CVectorArray m_ArrSecCrossConnectionPlate;				// 모멘트이음판이 있는 세로보딸린 가로보위치
	CVectorArray m_ArrSecStringer;							// 세로보위치
	CVectorArray m_ArrSecVBracing;							// 수직브레이싱 위치
	CVectorArray m_ArrSecHBracing;							// 수평브레이싱 위치


	long m_nFatigueRoadType;								// 피로검토시 도로의 종류
	long m_nFatigueStructureType;							// 피로검토시 구조형식		

	int		m_nTypePos;										// 신축이음위치(A1, A2)
	int		m_nTypeResist[2];							    // 절점구속조건
	int		m_nTypeArea[2];									// 보통/ 한랭 지방...
	double	m_dLenSinchukGir[2];							// 신축 들보의 거리
	double	m_dHeightGir[2];								// 주형의 형고 h :
	double	m_RotAngle;										// 회전각 Θ:
	double	m_Bfa;											// 저감 계수 b :
	double	m_Tfa;											// 온도변화량 ΔT :
	double	m_Afa;											// 선팽창계수 a :	
	double	m_D1[2];										// 설치여유량 d1 :
	double	m_D2[2];										// 부가 여유량 d2 :
	double  m_MaxUgan[2];									// 최대유간
	double  m_MinUgan[2];									// 최소유간
	CString m_szUseJoint[2];								// 조인트타입

	double m_dEQd;											// 지진시 상부변위;
	double  m_AllowUgan[2];									//허용신축량...

	CString GetFixingPlateType(long nType);
	long GetFixingPlateType(CString strFixingPlateType);

	void	SetShoeGirderLower(BOOL bGirderLower)	{ m_bShoeGirderLower = bGirderLower;	}
	BOOL	IsShoeGirderLower()	{ return m_bShoeGirderLower;	}

	//공식 => 값 변환 함수

	//Jack에 의한 최대 긴장응력 계산값
	double GetJackMaxStrToDouble(CString strFormula);

	double GetConstPSfpu();
	double GetConstPSfy();
	CString GetStrConstPSfpu();
	CString GetStrConstPSfy();
	double GetConstPreStressCompress();		// 프리스트레스 도입직후 - 압축응력 
	double GetConstPreStressStretch();		// 프리스트레스 도입직후 - 인장응력
	double GetConstDesignLoadCompress();	// 설계하중 작용시(손실후) - 압축응력
	double GetConstDesignLoadStretch();		// 설계하중 작용시(손실후) - 인장응력 

	void GetDefaultConstPSf(CStringArray &strArrConstPSf);
	void SetDefaultAllowDesignMaterials();
	long	m_nFlag;

private:
	BOOL	m_bShoeGirderLower;		// 교량받침 거더하단 적용
	double GetPSfty(CString strFormula);
};

#endif // !defined(AFX_CALCDATA_H__499DDEDD_3ACB_451B_936E_1F344A037A90__INCLUDED_)
