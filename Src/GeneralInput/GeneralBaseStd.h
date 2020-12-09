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

#define ID_INPUT_END	88888	///< �Է� ��ȭ���� ����

#define NEXT_PROCESS  1
#define PREV_PROCESS -1
#define TITLE_HEIGHT 20

#define IDD_MENU_GEN	0
#define IDD_MENU_DETAIL	1
#define IDD_MENU_CALC	2
#define IDD_MENU_DECK	3
#define IDD_MENU_SIZE	4

#define IDD_MENU_APLATETYPE_FULL	5	// �⺻�޴� ����


/// �Է� �׸��� ȯ�溯��
#define GRID_TEXT_BK_COLOR		RGB(253, 250, 255)
#define GRID_TEXT_BK_COLOR_REV	RGB(220, 250, 255)
#define GRID_TEXT_BK_COLOR2		RGB(255, 255, 128)
#define GRID_TEXT_BK_COLOR3		RGB(128, 255, 128)

/// �Է�â ������
#define MIN_SCALE		1		///< �ּ� ������
#define MAX_SCALE		500		///< �ִ� ������
#define GAP_SCALE		20		///< �� ������
#define DEFAULT_SCALE	20		///< �⺻ ������
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
	BOOL bViewMouseEvent;	///< ���콺 �̺�Ʈ �÷���
	CDPoint xy;				///< ���콺 �̺�Ʈ ��ǥ
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
	// ���̾�α� ����		
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
	CCalcCantileverLoadDlg	m_CalcCantileverLoadDlg;	// ĵƿ������ ���߰��
	CCalcLossOfPrestress	m_CalcLossOfPrestress;		// ������Ʈ������ �ս�
	CCalcFloorSlabStress	m_CalcFloorSlabStress;		// �ٴ��� ���������°���
	CCalcCantileverRebarDlg	m_CalcCantileverRebarDlg;   // 47
	CCalcUltimateLoadDlg	m_CalcUltimateLoadDlg;		// �������߿� ���� �ܸ����
	CCalcCantileverDroop	m_CalcCantileverDroopDlg;   // 48
	CCalcSectionConceptDlg	m_CalcSectionConceptDlg;
	CCalcSecForceDivCalc	m_CalcSecForceDivCalcDlg;   // 52
	CCalcSecForceDivResult  m_CalcSecForceDivResultDlg; // 53
	CCalcWindForce          m_CalcWindForceDlg;			// 54 
	CCalcModelingSetup		m_CalcModelingSetupDlg;     // 55
	CCalcFrameComboLoadDlg	m_CalcFrameComboLoadDlg;	//�𵨸� ��������
	CCalcModelingResult		m_CalcModelingResultDlg;    // 57 
	CCalcInfluenceK			m_CalcInfluenceKDlg;        // 58   
	CCalcSecStress			m_CalcSecStressDlg;         // 59  
	CCalcSecOutline			m_CalcSecOutlineDlg;		// 60
	CCalcWarping			m_CalcWarping;				// 61	
	CCalcDisplacement       m_CalcDisplacement;         // 62
	CCalcFatigue            m_CalcFatigue;              // 63
	CCalcWeld				m_CalcWeld;                 // 64
	CDesignBaseDlg			m_DesignBase;				// 66
	
	CBMOutSheet				m_BMOutSheet;		// �������ǥ
	CPaintOutSheet			m_PaintOutSheet;	// �������ǥ
	CWeldOutSheet			m_WeldOutSheet;		// ������������ǥ
	// �ٴ��� ���=============================================================
	CDeckBasicDataDlg			m_DeckBasicDataDlg;		// 1. �����ڷ�
	CDeckRebarInfoDlg			m_DeckRebarInfoDlg;		// 2. �������
	CDeckMainRebarDlg			m_DeckMainRebarDlg;		// 3. ��ö�� ��ġ
	CDeckDistriRebarCTCDlg		m_DeckDistriRebarCTCDlg;// 4. ���ö�� ����
	CDeckDistriRebarDlg			m_DeckDistriRebarDlg;	// 5. ���ö�� ��ġ
	CDeckMainRebarCTCDlg		m_DeckMainRebarCTCDlg;	// 6.0 ��ö�� ����
	CDeckMainSkewRebarCTCDlg	m_DeckMainSkewRebarCTCDlg;// 6.1 �簢����ö�� ����
	CDeckSabogangCTCDlg			m_DeckSabogangCTCDlg;	// 6.2 �纸��ö�� ����
	CDeckBindConcRebarDlg		m_DeckBindConcRebarDlg;	// 6.3 �纸��ö�� ����
	CDeckCrossStringerDlg		m_DeckCrossStringerDlg;	// 6.5 ���κ�,���κ�����ö��
	CDeckGuardFenceDlg			m_DeckGuardFenceDlg;	
	CDeckPlanRebarDlg			m_DeckPlanRebarDlg;		// 7. ��ü���
	CDeckEtcRebarDlg			m_DeckEtcRebarDlg;		// 8. ��Ÿö�ٹ�ġ
	CDeckRebarJewonDlg			m_DeckRebarJewonDlg;	// 9. ö������	
	CDeckBMOptionDlg			m_DeckBMOptionDlg;		// 10. ���� ��� �ɼ�
	
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

	structMouseEvent		m_strME;				///< �信�� ������ ���콺 �̺�Ʈ ���� ����
	CMenu					m_menuInput;			///< �Է� �޴�
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
// ĵƿ���� ����, Ⱦ�й�, Ⱦ�й� ��� ����
	long	GetLoadApplyMemberCount(CFEMManage *pFEM);
	void	DrawDimLoadCase(CForceDiagram *pDom, CFEMManage *pFEM, long nCase, double dTextHeight=1);
	void	DrawDimJoint(CForceDiagram *pDom, CFEMManage *pFEM, double dTextHeight=1, BOOL bElemNum=TRUE);
	void	DrawWallGuard(CForceDiagram *pDom, double dTextHeight=1);
	CDPointArray GetWallGuardPoint(CDPoint* pPoint);
	long GetLoadCase(CFEMManage *pFEM, CString szLoadCase);
};

#endif // !defined(AFX_PPCINPUTSTD_H__A02A1035_67BF_4B24_8B3F_503B8072978D__INCLUDED_)
