// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PPCInputStd.h: interface for the CGeneralBaseStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PPCINPUTSTD_H__A02A1035_67BF_4B24_8B3F_503B8072978D__INCLUDED_)
#define AFX_PPCINPUTSTD_H__A02A1035_67BF_4B24_8B3F_503B8072978D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CLR_BACK	 RGB(253, 250, 255)
#define CLR_READONLY RGB(255, 242, 200)

#define ID_INPUT_END	88888	///< 입력 대화상자 종점

#define NEXT_PROCESS  1
#define PREV_PROCESS -1
#define TITLE_HEIGHT 20

#define IDD_MENU_GEN	0
#define IDD_MENU_DETAIL	1
#define IDD_MENU_CALC	2
#define IDD_MENU_DECK	3
#define IDD_MENU_SIZE	4

#define IDD_MENU_APLATETYPE_FULL	5	// 기본메뉴 갯수


/// 입력 그리드 환경변수
#define GRID_TEXT_BK_COLOR		RGB(253, 250, 255)
#define GRID_TEXT_BK_COLOR_REV	RGB(220, 250, 255)
#define GRID_TEXT_BK_COLOR2		RGB(255, 255, 128)
#define GRID_TEXT_BK_COLOR3		RGB(128, 255, 128)

/// 입력창 스케일
#define MIN_SCALE		1		///< 최소 스케일
#define MAX_SCALE		500		///< 최대 스케일
#define GAP_SCALE		20		///< 갭 스케일
#define DEFAULT_SCALE	20		///< 기본 스케일
#define DEFAULT_TEXT_HEIGHT 3.4

#include "stdAfx.h"
#include "GeneralInput.h"
#include "TotalViewDlg.h"

class CDataManage;
class CAPlateCalcStd;
class CADeckData;
class CADeckDataTU;
class CGeneralBaseDlg;

typedef struct STRUCT_MOUSE_EVENT
{
	BOOL bViewMouseEvent;	///< 마우스 이벤트 플래그
	CDPoint xy;				///< 마우스 이벤트 좌표
}structMouseEvent;
class AFX_EXT_CLASS CGeneralBaseStd  
{
public:
	CGeneralBaseStd(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd);
	virtual ~CGeneralBaseStd();
	
private:
	CWordArray		  m_DlgHistroyArray;
	long			  m_nHistoryIdx;	
	
	CTypedPtrArray	  <CObArray, CGeneralBaseDlg*> m_DialogList;		
	CGeneralBaseDlg			*m_pCurruntDlg;	
	CTotalViewDlg			m_TotalViewDlg;
	// 다이얼로그 종류		
	CSelectLineDlg			m_SelLineDlg;				// 0
	CDesignPageDlg			m_BaseInputDlg;				// 1
	CBaseInfoDlg			m_BaseInfoDlg;				// 2
	CHDanDlg				m_HDanDlg;					// 3
	CJiganGusengDlg			m_JiganGusengDlg;			// 5
	CJijumAngleDlg			m_JijumAngleDlg;			// 6
	CJackupPositionDlg		m_JackupPositionDlg;		// 8
	CJijumStiffDlg			m_JijumStiffDlg;			// 9
	CStartPointJewonDlg		m_StartPointJewonDlg;		// 11
	CByunDanmyunJewonDlg	m_ByunDanmyunJewonDlg;		// 12
	CVBracingPositionDlg	m_VBracingPositionDlg;		// 13
	CVStiffPositionDlg		m_VStiffPositionDlg;		// 14
	CHBracingPositionDlg	m_HBracingPositionDlg;		// 15
	CJRibPosition			m_JRibPositionDlg;			// 16
	CDanmyunTypeDlg			m_DanmyunTypeDlg;			// 17
	CFieldJoinPositionDlg	m_FieldJoinPositionDlg;		// 18
	CFactJoinPositionDlg	m_FactJoinPositionDlg;		// 19
	CSlabJewonDlg			m_SlabJewonDlg;				// 20
	//
	CDetEtcInputDlg			m_DetEtcInputDlg;
	CDetFieldJoinDlg		m_DetFieldJoinDlg;			// 33
	CDetCrossBeamTypeDlg	m_DetCrossBeamTypeDlg;		// 34
	CDetVBracingTypeDlg		m_DetVBracingTypeDlg;		// 36
	CDetHBracingDlg			m_DetHBracingDlg;			// 39
	CDetVStiffJewonDlg		m_DetVStiffJewonDlg;		// 30
	CDetCamberJewonDlg		m_DetCamberJewonDlg;		// 41
	CDetWebHStiffPosDlg		m_DetWebHStiffPosDlg;		// 42
	CDetSHConnectorVPosDlg	m_DetSHConnectorVPosDlg;	// 44
	
	CCalcCantileverSetup	m_CalcSlabSetupDlg;
	CCalcCantileverDeadLoad m_CalcCantileverDeadLoadDlg;// 46
	CCalcCantileverModeling	m_CalcCantileverModeling;
	CCalcCantileverLoadCombo	m_CalcCantileverLoadCombo;
	CCalcCantileverModelResult	m_CalcCantileverModelResult;
	CCalcCantileverLoadDlg	m_CalcCantileverLoadDlg;	// 캔틸레버부 하중계산
	CCalcLossOfPrestress	m_CalcLossOfPrestress;		// 프리스트레스의 손실
	CCalcFloorSlabStress	m_CalcFloorSlabStress;		// 바닥판 슬래브응력검토
	CCalcCantileverRebarDlg	m_CalcCantileverRebarDlg;   // 47
	CCalcUltimateLoadDlg	m_CalcUltimateLoadDlg;		// 극한하중에 대한 단면검토
	CCalcCantileverDroop	m_CalcCantileverDroopDlg;   // 48
	CCalcSectionConceptDlg	m_CalcSectionConceptDlg;
	CCalcSecForceDivCalc	m_CalcSecForceDivCalcDlg;   // 52
	CCalcSecForceDivResult  m_CalcSecForceDivResultDlg; // 53
	CCalcWindForce          m_CalcWindForceDlg;			// 54 
	CCalcModelingSetup		m_CalcModelingSetupDlg;     // 55
	CCalcFrameComboLoadDlg	m_CalcFrameComboLoadDlg;	//모델링 하중조합
	CCalcModelingResult		m_CalcModelingResultDlg;    // 57 
	CCalcInfluenceK			m_CalcInfluenceKDlg;        // 58   
	CCalcSecStress			m_CalcSecStressDlg;         // 59  
	CCalcSecOutline			m_CalcSecOutlineDlg;		// 60
	CCalcWarping			m_CalcWarping;				// 61	
	CCalcDisplacement       m_CalcDisplacement;         // 62
	CCalcFatigue            m_CalcFatigue;              // 63
	CCalcWeld				m_CalcWeld;                 // 64
	CDesignBaseDlg			m_DesignBase;				// 66
	
	CBMOutSheet				m_BMOutSheet;		// 강재재료표
	CPaintOutSheet			m_PaintOutSheet;	// 도장재료표
	CWeldOutSheet			m_WeldOutSheet;		// 용접수량집계표
	// 바닥판 배근=============================================================
	CDeckBasicDataDlg			m_DeckBasicDataDlg;		// 1. 기초자료
	CDeckRebarInfoDlg			m_DeckRebarInfoDlg;		// 2. 배근정보
	CDeckMainRebarDlg			m_DeckMainRebarDlg;		// 3. 주철근 배치
	CDeckDistriRebarCTCDlg		m_DeckDistriRebarCTCDlg;// 4. 배력철근 간격
	CDeckDistriRebarDlg			m_DeckDistriRebarDlg;	// 5. 배력철근 배치
	CDeckMainRebarCTCDlg		m_DeckMainRebarCTCDlg;	// 6.0 주철근 간격
	CDeckMainSkewRebarCTCDlg	m_DeckMainSkewRebarCTCDlg;// 6.1 사각부주철근 간격
	CDeckSabogangCTCDlg			m_DeckSabogangCTCDlg;	// 6.2 사보강철근 간격
	CDeckBindConcRebarDlg		m_DeckBindConcRebarDlg;	// 6.3 사보강철근 간격
	CDeckCrossStringerDlg		m_DeckCrossStringerDlg;	// 6.5 가로보,세로보보강철근
	CDeckGuardFenceDlg			m_DeckGuardFenceDlg;	
	CDeckPlanRebarDlg			m_DeckPlanRebarDlg;		// 7. 전체배근
	CDeckEtcRebarDlg			m_DeckEtcRebarDlg;		// 8. 기타철근배치
	CDeckRebarJewonDlg			m_DeckRebarJewonDlg;	// 9. 철근제원	
	CDeckBMOptionDlg			m_DeckBMOptionDlg;		// 10. 수량 출력 옵션
	
	UINT 		m_nSaveMenuIDD;

	
public:		
	//	CMenu m_GenMenu;
	CWordArray	 m_IDDArray[IDD_MENU_SIZE];
	
	CDataManage	     *m_pDataManage;	
	CAPlateCalcStd	 *m_pCalcStd;
	CFEMDraw		m_FEMDraw;
	CDocument		 *m_pDoc;
	CProgressCtrl	  m_ctlProgress;
	CADeckData		 *m_pDeckData;
	CADeckDataTU	 *m_pDeckDataTU;

	static BOOL m_bInclude_Module_Deck;
	static BOOL m_bInclude_Module_TUGirder;
	static BOOL m_bInclude_Module_DesignStd;
	static void SetModuleDeck(BOOL bInclude);
	static void SetModuleTUGirder(BOOL bInclude);
	static void SetModuleDesignStd(BOOL bInclude);
	static BOOL IsModuleDeck();
	static BOOL IsModuleTUGirder();
	static BOOL IsModuleDesignStd();
	
	void Serialize(CArchive &ar);

	structMouseEvent		m_strME;				///< 뷰에서 보내는 마우스 이벤트 정보 저장
	CMenu					m_menuInput;			///< 입력 메뉴
	CMenu					m_MenuBearingDB;
private:	
	CGeneralBaseDlg *FindValidDlg(CGeneralBaseDlg *pBaseDlg, BOOL bNext);	
	void InsertDialog(UINT nID, CGeneralBaseDlg *pDlg, long nType);
	void InsertHistory(UINT nIDD);
	
public:
	CGeneralBaseDlg *GetDlgByIndex(long nDlgIdx) { return m_DialogList[nDlgIdx];	}
	void HideCurrentDialog();
	long GetSizeDialogList() { return m_DialogList.GetSize();	}
	UINT GetLastInputMenuID();
	CTotalViewDlg *GetTotalView() { return &m_TotalViewDlg; }
	void ShowTotalView(CWnd *pWnd = NULL);
	void HideTotalView();
	CGeneralBaseDlg *GetNextDlg(CGeneralBaseDlg *pBaseDlg, BOOL bNext=TRUE, long nCount=1);
	UINT GetMenuID(CGeneralBaseDlg *pDlg);
	BOOL IsEnableDialog(UINT nID);	
	void GoCurrentDlg();	
	
	void GoNextHistory();
	void GoPrevHistory();
	BOOL IsNextHistroy();
	BOOL IsPrevHistory();

	void InsertInputMenu(CXTPMenuBar* pMenuBar);
	UINT GetCurrentMenuID() { return m_pCurruntDlg ? m_pCurruntDlg->GetMenuID() : 0; }

	UINT ConvertResource(UINT nID);
				
public:	
	void OutputViewOnAView(CStringArray &Arr);
	CString GetStrSaveFileName();
	void SetCurruntDialog(CGeneralBaseDlg *pDlg) { m_pCurruntDlg = pDlg; }
	void SetDialogEnable();
	
	CGeneralBaseDlg* GetCurruntDlg() { return m_pCurruntDlg; }
	CGeneralBaseDlg* GetDlgOfMenuID(UINT nMenuID);
	CGeneralBaseDlg* GetDlgOfIDD(UINT nIDD);
	CGeneralBaseDlg* GetDialogByDir(BOOL bNext);
	UINT GetCurMenuID() { return (m_pCurruntDlg) ? m_pCurruntDlg->GetMenuID() : 0; }
	UINT OnShowDialog(CGeneralBaseDlg *pDlg, CWnd *pParent = NULL, BOOL bCenter = FALSE, int nFlag = NEXT_PROCESS, BOOL bAddHis=TRUE);	
	void MoveNextDialog();
	void MovePrevDialog();	
	
	UINT GetSaveMenuIDD() { return m_nSaveMenuIDD; }
	void SetSaveMenuIDD(UINT nIDD) { m_nSaveMenuIDD = nIDD; }
	long GetMenuTypeOnIDD(UINT nIDD);
	
	CDataManage				*GetDataManage()	const		{ return m_pDataManage; }
	CBMOutSheet				*GetBMOutSheet()				{ return &m_BMOutSheet; }
	CWeldOutSheet			*GetWeldOutSheet()				{ return &m_WeldOutSheet;}
	CPaintOutSheet			*GetPaintOutSheet()				{ return &m_PaintOutSheet; }
	CCalcSecForceDivCalc	*GetCalcSecForceDivCalc()		{ return &m_CalcSecForceDivCalcDlg;  }
	CCalcSecForceDivResult	*GetCalcSecForceDivResult()		{ return &m_CalcSecForceDivResultDlg;  }
	CCalcCantileverSetup	*GetCalcCantileverSetup()		{ return &m_CalcSlabSetupDlg;	}
	CCalcWindForce			*GetCalcWindForce()				{ return &m_CalcWindForceDlg;        }
	CCalcInfluenceK			*GetCalcInfluenceK()			{ return &m_CalcInfluenceKDlg;		  }
	CCalcSecOutline			*GetCalcSecOutline()			{ return &m_CalcSecOutlineDlg;       }
	CCalcDisplacement		*GetCalcSlabDisplacement()		{ return &m_CalcDisplacement;		  }
	CDetFieldJoinDlg		*GetDetFieldJoinDlg()			{ return &m_DetFieldJoinDlg;		  }
	CDetCrossBeamTypeDlg    *GetDetCrossBeamTypeDlg()		{ return &m_DetCrossBeamTypeDlg;		  }
	CDetVBracingTypeDlg		*GetDetVBracingTypeDlg()		{ return &m_DetVBracingTypeDlg;		  }
	CDetHBracingDlg			*GetDetHBracing()				{ return &m_DetHBracingDlg;		  }	
	CDetVStiffJewonDlg		*GetDetVStiffJewonDlg()			{ return &m_DetVStiffJewonDlg;		  }	
	CCalcCantileverLoadDlg	*GetCalcCantileverLoadDlg()		{ return &m_CalcCantileverLoadDlg;	}
	CCalcFatigue			*GetCalcFatigue()				{ return &m_CalcFatigue; }
	CDeckBasicDataDlg		*GetDeckBasicDataDlg()			{ return &m_DeckBasicDataDlg; } 		
	CDeckRebarInfoDlg		*GetDeckRebarInfoDlg()			{ return &m_DeckRebarInfoDlg; } 		
	CDeckMainRebarDlg		*GetDeckMainRebarDlg()			{ return &m_DeckMainRebarDlg; } 		
	CDeckDistriRebarCTCDlg	*GetDeckDistriRebarCTCDlg()		{ return &m_DeckDistriRebarCTCDlg; } 	
	CDeckDistriRebarDlg		*GetDeckDistriRebarDlg()		{ return &m_DeckDistriRebarDlg; } 	
	CDeckMainRebarCTCDlg	*GetDeckMainRebarCTCDlg()		{ return &m_DeckMainRebarCTCDlg; } 	
	CDeckMainSkewRebarCTCDlg*GetDeckMainSkewRebarCTCDlg()	{ return &m_DeckMainSkewRebarCTCDlg; } 	
	CDeckSabogangCTCDlg		*GetDeckSabogangCTCDlg()		{ return &m_DeckSabogangCTCDlg; } 	
	CDeckCrossStringerDlg	*GetDeckCrossStringerDlg()		{ return &m_DeckCrossStringerDlg; } 	
	CDeckBindConcRebarDlg	*GetDeckBindConcRebarDlg()		{ return &m_DeckBindConcRebarDlg; } 	
	CDeckPlanRebarDlg		*GetDeckPlanRebarDlg()			{ return &m_DeckPlanRebarDlg; } 		
	CDeckEtcRebarDlg		*GetDeckEtcRebarDlg()			{ return &m_DeckEtcRebarDlg; } 		
	CDeckRebarJewonDlg		*GetDeckRebarJewonDlg()			{ return &m_DeckRebarJewonDlg; } 		
	CDeckGuardFenceDlg		*GetDeckGuardFenceDlg()			{ return &m_DeckGuardFenceDlg;}
	CDeckBMOptionDlg		*GetDeckBMOptionDlg()			{ return &m_DeckBMOptionDlg;	}
	CCalcFrameComboLoadDlg	*GetCalcFrameComboLoadDlg()		{ return &m_CalcFrameComboLoadDlg;}
	CLineInfoApp *GetLineInfoApp() const;
	CPlateBridgeApp *GetBridge() const;
	CString GetBearingDBName(int nMenuID);
// 캔틸래버 하중, 횡분배, 횡분배 결과 공통
	long	GetLoadApplyMemberCount(CFEMManage *pFEM);
	void	DrawDimLoadCase(CForceDiagram *pDom, CFEMManage *pFEM, long nCase, double dTextHeight=1);
	void	DrawDimJoint(CForceDiagram *pDom, CFEMManage *pFEM, double dTextHeight=1, BOOL bElemNum=TRUE);
	void	DrawWallGuard(CForceDiagram *pDom, double dTextHeight=1);
	CDPointArray GetWallGuardPoint(CDPoint* pPoint);
	long GetLoadCase(CFEMManage *pFEM, CString szLoadCase);
};

#endif // !defined(AFX_PPCINPUTSTD_H__A02A1035_67BF_4B24_8B3F_503B8072978D__INCLUDED_)
