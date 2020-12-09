// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8B4EDC4F_2DFC_4DF0_BD58_4EC6FF390522__INCLUDED_)
#define AFX_MAINFRM_H__8B4EDC4F_2DFC_4DF0_BD58_4EC6FF390522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_PAGE_UPDATE			WM_USER + 1
#define WM_CREATE_INPUT			WM_USER + 2
#define WM_UPDATE_TREE			WM_USER + 8
#define WM_UPDATE_INPUT_TITLE	WM_USER + 10
#define WM_UPDATE_DRAWING_TITLE	WM_USER + 11
#define WM_UPDATE_STATUSBAR		WM_USER + 12
#define WM_UPDATE_TOOLBAR_DRAW	WM_USER + 13
#define WM_UPDATE_UNITCOMBO		WM_USER + 14
#define WM_UPDATE_SHOWALLCOMBO	WM_USER + 15
#define WM_EXPORT_ALINE			WM_USER + 16
#define WM_DESIGN_CHECK			WM_USER + 17
#define WM_PAGEINFO				WM_USER	+ 33
#define WM_PAGE_RESET			WM_USER	+ 44
#define WM_GET_CHILDBAR			WM_USER	+ 46
#define WM_CHANGE_MENUDLG		WM_USER	+ 47
#define WM_SAVE_JPG				WM_USER	+ 50

#define WM_USER_APPLYDOMYUN		WM_USER	+116
#define WM_MODIFY_INPUTDATA		WM_USER +300	// 입력데이터 수정
#define WM_USER_SECTION			WM_USER +200	// 단면검토 사용자 설계점검


#define PRJ_TEXT	1
#define ENG_TEXT	2
#define DIR_TEXT	3

#define PROGRESS_TEXT		1
#define PAGE_TEXT			2
#define MOUSE_AXIAL_TEXT	3
#define MOVE_TEXT			4
#define VERSION_TEXT		5

#include "DockPaneProc.h"
#include "ShortcutPaneStart.h"
#include "ShortcutPaneInput.h"
#include "ShortcutPaneDrawing.h"
#include "XTPTabClientWndInPlate.h"
#include "WaitForSubProgram.h"

class CAPlateView;
class CAPlateDoc;

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	BOOL m_bFullScreenMode;

	CXTPTabClientWndInPlate m_MTIClientWnd;

// Operations
public:
	void CreateShortcutBar();
	void AddHisData(CString szTextIn);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void OnComboPageUpdate(long nPage);
	void ShowPageInfo();
	void ResizePane(UINT nID, CSize size, BOOL bHor);

public:
	BOOL	m_bPrintPreview;	
	BOOL	m_bMessageDraw;
	CXTPCommandBars	*m_pCommandBars;
	CXTPDockingPaneManager	m_paneManager;
	CDockPaneProc<CXTPShortcutBar> m_wndShortcutBar;

	CShortcutPaneStart		m_paneShortcutStart;
	CShortcutPaneInput		m_paneShortcutInput;
	CShortcutPaneDrawing	m_paneShortcutDrawing;

	CXTPStatusBar			m_wndStatusBar;
	CXTFlatComboBox			m_ComboUnit;
	CXTFlatComboBox			m_ComboShowAll;

	void GetModifyDataList(UINT nID, CDWordArray &ModifyList);
	CPageInfoDlg	m_PageInfoDlg;	

	CAPlateDoc* GetDocument();
	CAPlateView* GetPlateView();

protected:  // control bar embedded members
	CXTCaption m_wndCaption;
	CToolBar m_wndToolBar;	
	CWaitForSubProgram m_WaitForALineDlg;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnFileNew();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnIdleUpdateCmdUI();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		
	afx_msg void OnViewCustom();
	afx_msg void OnEditPage();		
	afx_msg void OnOptPrg();
	afx_msg void OnDestroy();
	afx_msg void OnStructureCalc();
	afx_msg void OnShopoutPaint();
	afx_msg void OnShopoutBill();
	afx_msg void OnShopoutWeld();
	afx_msg void OnUpdateShopoutWeld(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewCommand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShopoutBill(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShopoutPaint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStructureCalc(CCmdUI* pCmdUI);
	afx_msg void OnPageRepos();
	afx_msg void OnRealupdate();
	afx_msg void OnHelpWebFaq();
	afx_msg void OnHelpWebHome();
	afx_msg void OnOutBm();
	afx_msg void OnUpdateOutBm(CCmdUI* pCmdUI);
	afx_msg void OnOutNondes();
	afx_msg void OnUpdateOutNondes(CCmdUI* pCmdUI);
	afx_msg void OnViewAligntoolbar();
	afx_msg void OnViewDomyunbar();
	afx_msg void OnViewPropbar();
	afx_msg void OnViewOsnapmode();
	afx_msg void OnHelpGenDetail();
	afx_msg void OnHelpCalc();
	afx_msg void OnHelpRebar();
	afx_msg void OnHelpOut();
	afx_msg void OnHelpAline();
	afx_msg void OnHelpAboarder();
 	afx_msg void OnShowF13TBrochure();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateList();
	afx_msg void OnOutWeldChange();
	afx_msg void OnUpdateOutWeldChange(CCmdUI* pCmdUI);
	afx_msg void OnViewControlpane();
	afx_msg void OnUpdateViewControlpane(CCmdUI* pCmdUI);
	afx_msg void OnViewInputpane();
	afx_msg void OnUpdateViewInputpane(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnShowUpdateDlg(WPARAM wp,LPARAM lp);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg void OnCustomize();
	afx_msg void OnInput(UINT nID);
	afx_msg LRESULT OnCreateInputPane(WPARAM nID, LPARAM nResize=1);
	afx_msg LRESULT OnUpdateInputTitle(WPARAM nMenuID, LPARAM lParam);
	afx_msg LRESULT OnShortcutBarNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelectPageCombo();
	afx_msg LRESULT OnEditPageUpdate(WPARAM, LPARAM);
	afx_msg void OnUpdateInput(CCmdUI* pCmdUI);
	afx_msg LRESULT OnUpdateTree(WPARAM wParam, LPARAM);	
	afx_msg void OnChangeFem(WPARAM wp, LPARAM);
	afx_msg LRESULT OnPageReset(WPARAM nPart, LPARAM);
	afx_msg LRESULT OnPageInfo(WPARAM, LPARAM);	
	afx_msg LRESULT OnGetChildBar(WPARAM, LPARAM);
	afx_msg LRESULT OnChangeMenuDlg(WPARAM id, LPARAM);
	afx_msg LRESULT SaveImageOnGLView(WPARAM, LPARAM);
	afx_msg LRESULT OnSectionCheck(WPARAM wp, LPARAM lp);	// 단면요약도 사용자 설계점검
	afx_msg LRESULT OnModifyInputData(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnApplyDomyun(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateToolbarDraw(WPARAM wp, LPARAM lp);
	afx_msg void OnCreateKey();
	DECLARE_MESSAGE_MAP()

	LRESULT ResetDomCombo();
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8B4EDC4F_2DFC_4DF0_BD58_4EC6FF390522__INCLUDED_)
