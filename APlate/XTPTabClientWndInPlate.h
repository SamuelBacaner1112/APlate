// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XTPTabClientWnd.h : interface for the CXTPTabClientWndInPlate class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// 004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTPTABCLIENTWNDINPPC_H__)
#define __XTPTABCLIENTWNDINPPC_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

#define WM_XTP_GETTABICON (WM_USER + 131)
#define WM_XTP_PRETRANSLATEMOUSEMSG (WM_USER + 132)
#define WM_XTP_GETWINDOWTEXT (WM_USER + 133)

#ifndef WM_XTP_GETTABCOLOR
#define WM_XTP_GETTABCOLOR (WM_USER + 134)

#define XTP_TABCOLOR_BLUE		0x1000000
#define XTP_TABCOLOR_YELLOW		0x1000001
#define XTP_TABCOLOR_GREEN		0x1000002
#define XTP_TABCOLOR_RED		0x1000003
#define XTP_TABCOLOR_PURPLE		0x1000004
#define XTP_TABCOLOR_CYAN		0x1000005
#define XTP_TABCOLOR_ORANGE		0x1000006
#define XTP_TABCOLOR_MAGENTA	0x1000007
#endif

// Workspace buttons enumerator.

class CXTPTabWorkspaceInPlate;
class CXTPTabClientWndInPlate;

//:Ignore	

// Summary: CXTPTabWorkspaceInPlate  is a CWnd derived class. internal used.
class CXTPTabWorkspaceInPlate : public CWnd
{

	class CPaintManagerInPlate
	{
	public:
		CPaintManagerInPlate();
		void RefreshMetrics();
		
		void DrawTabControl(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw);
		void DrawTabControlOffice(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw);
		void DrawTabControlOffice2003(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw);
		void DrawTabControlDefault(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw);
		void DrawTabControlNativeXP(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw);
		COLORREF DrawButton(CDC* pDC, CRect rc, COLORREF clrDark, BOOL bFirst, BOOL bFocused, COLORREF clrBorder, COLORREF clrLeftHighlight, COLORREF clrTopHighlight);
		
	
	protected:
		CXTPWinThemeWrapper m_themeTab;
		CXTPWinThemeWrapper m_themeToolbar;
		
		COLORREF m_clrBackgroundLight, m_clrBackgroundDark;
		COLORREF m_clrBorderSelected;
		COLORREF m_clrBorder;
		COLORREF m_clrBorderLeftHighlight;
		COLORREF m_clrBorderTopHighlight;
		COLORREF m_clrHighlight;

		CXTPTabWorkspaceInPlate* m_pWorkspace;

		friend class CXTPTabWorkspaceInPlate;
	};
	
	
	
	struct TAB_ITEM_ENTRY
	{
		CString strText;
		HWND hWnd;
		CRect rcItem;
		BOOL bFound;
		int nColor;
	} ;
	
	
	class CTabArray: public CArray<TAB_ITEM_ENTRY, TAB_ITEM_ENTRY>
	{
	public:
		void Swap(int i, int j)
		{
			TAB_ITEM_ENTRY p = GetAt(i);
			SetAt(i, GetAt(j));
			SetAt(j, p);
		}
		void SortIndexes(int* nIndex)
		{
			pCompare = this;
			qsort(nIndex, GetSize(), sizeof(int), CTabArray::CompareRect);
		}
		
		static CTabArray* pCompare;
		static int _cdecl CompareRect(const void *arg1, const void *arg2 )	
		{	
			CRect rc1 = pCompare->GetAt(*(int*)arg1).rcItem;
			CRect rc2 = pCompare->GetAt(*(int*)arg2).rcItem;
			if (rc1.Width() > rc2.Width()) return -1;
			if (rc1.Width() < rc2.Width()) return 1;
			
			return 0;
		}
	};
	
public:
	CPaintManagerInPlate m_paintManager; 

	CPaintManagerInPlate* PaintManager() {return &m_paintManager;} 

	class CDrawButton
	{
	public:
		CDrawButton (DWORD wStyle) { m_wStyle = wStyle;	}
		void Draw (CDC* pDC, COLORREF clrButton, BOOL bFrame = TRUE, COLORREF clrFrameTopLeft = GetSysColor(COLOR_BTNTEXT), COLORREF clrFrameBottomRight = GetSysColor(COLOR_3DFACE));
		DWORD Click (CXTPTabWorkspaceInPlate* pWnd, CPoint pt, UINT nIDRepeat = 0);
		void SetRect (CRect rc) { m_Rect = rc;}
		CRect GetRect() { return m_Rect;}
		BOOL PtInRect (POINT pt) const {return m_Rect.PtInRect (pt) != 0; }
		int GetStyle() {return m_wStyle;}

		void CheckForMouseOver (CWnd* pWnd, CPoint pt);
		void SetEnable(BOOL bEnable);
	private:
		CRect m_Rect;
		DWORD m_wStyle;
	};
	friend class CDrawButton;


// Construction
public:
	CXTPTabWorkspaceInPlate(); 

// Attributes
public:

	// scan through all MDIChild windows and update corresponding 
	// tab items if any changes happened (e.g. window text or active MDIChild).
	// If bAddNewWindows is set to TRUE then for any new found MDIChild
	// window the new tab item will be created (this option is useful when
	// it is called for first time and there are already some MDIChild windows
	// created) 
	void UpdateContents(BOOL bAddNewWindows=FALSE);

	
	CDrawButton m_btnClose;		
	CDrawButton m_btnLeft;		
	CDrawButton m_btnRight; 	
	DWORD m_dwFlags;		


protected:
	// array of tab items (every item corresponds to an MDIChild window)
    CTabArray m_arrTab;
	CArray<CRect, CRect&> m_lstRects;
	// array of MDIChild windows icons used in the tab control
    CArray<HICON, HICON> m_arrImage;
	int m_nSelected;
	int m_nItemTracking;
	int m_nItemFocused;

	int m_nOffset;
    

	// pointer to our substitute for MDIClient window. If it is NULL then
	// the tab control is not defined and there shouldn't be any action 
	// taken on it
	CXTPTabClientWndInPlate* m_pTabClientWnd;

// Operations
public:

	int GetCurSel() { return m_nSelected;};
	void SetCurSel(int nIndex) { m_nSelected = nIndex; Invalidate(FALSE);};
	CTabArray& GetTabsArray() { return m_arrTab; }

	int GetOffset() { return m_nOffset;}
	int GetItemTracking() { return m_nItemTracking;}
	int GetItemCount() { return (int)m_arrTab.GetSize();	}
	
	int HitTest(CPoint point);
	CWnd* GetChildWnd(int nIndex);
	int GetItemFocused();
	void AdjustTabs(int nWidth);
	HICON GetIcon(int nIndex);
	COLORREF GetColor(int nIndex);

protected:

	
	CWnd* MDIGetActive();
	void MDIActivate(CWnd* pWnd);
	void HighlightFocused();
	CMDIFrameWnd* GetParentFrame() const;

	// returns text for child window to be displayed in corresponding
	// tab item
	CString GetTextForTabItem(const CWnd* pChildWnd) const;

	// finds the tab item that corresponds to the specified window
	inline int FindTabItem(const CWnd* pWnd) const {
		ASSERT(pWnd!=NULL);
		ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
		return FindTabItem(pWnd->GetSafeHwnd());
	}
	int FindTabItem(const CString& szTitle) const;

	// finds the tab item that corresponds to the specified window
	int FindTabItem(const HWND hWnd) const;
	// adds new tab item for the specified window
	BOOL AddTabItem(long &s_nColor, const CWnd* pChildWnd,  CString szTitle, BOOL bRedraw=TRUE, 
		BOOL bOnlyVisible=TRUE);
	// removes the tab item for the specified window
	BOOL RemoveTabItem(const HWND hWnd, BOOL bRedraw=TRUE);

// Overrides

	//{{AFX_VIRTUAL(CXTPTabWorkspaceInPlate)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXTPTabWorkspaceInPlate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXTPTabWorkspaceInPlate)
	afx_msg void OnIdleUpdateCmdUI();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    friend class CXTPTabClientWndInPlate;		
	friend class CPaintManagerInPlate;			
};


//:End Ignore	

/////////////////////////////////////////////////////////////////////////////
// CXTPTabClientWndInPlate is a CWnd derived class. It represents a tabbed workspace.
class CXTPTabClientWndInPlate : public CWnd
{
// Construction
public:
	// Summary: Constructs a CXTPTabClientWndInPlate object
	CXTPTabClientWndInPlate();

// Attributes
public:
	// Summary: Rerieves the paint manager.
	CXTPTabWorkspaceInPlate::CPaintManagerInPlate* PaintManager() {return &m_tab.m_paintManager;}

	// Summary: Call this member to set flags of the tab workspace. See XTPWorkspaceButtonsInPPC enumerators.
	// See also: GetFlags
	void SetFlags(DWORD dwButtons);

	// Summary: Retrieves flags of the tab workspace.
	DWORD GetFlags();

protected:
	// tab control
	CXTPTabWorkspaceInPlate m_tab;
	
	// pointer to the corresponding parent MDIFrame window
	CMDIFrameWnd* m_pParentFrame;

	// flag that specifies that layout of tab control and MDIClient 
	// must be recalculated
	BOOL m_bForceToRecalc;

// Operations
public:

	// Input:	pParentFrame - Points to a CMDIFrameWnd object
	//			dwTabCtrlStyle - Style of docking.
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Attaches the tabbed workspace.
	BOOL Attach(const CMDIFrameWnd* pParentFrame, 
		DWORD dwTabCtrlStyle = 0);
	
	// Returns: TRUE if successful; otherwise returns FALSE
	// Summary: Detaches the tabbed workspace.
	BOOL Detach();

	// Returns: TRUE if attached; otherwise returns FALSE
	// Summary: Determines if a tab workspace is attached.
	inline BOOL IsAttached() const { 
		return (m_pParentFrame!=NULL ? TRUE : FALSE); 
	}

	// Returns: Tab control.
	// Summary: Retrieves the tab control.
	inline CXTPTabWorkspaceInPlate* GetTabCtrl() {
		if(!IsAttached())
			return NULL;
		return &m_tab;
	}

	// Summary: Retrieves the parent frame of the control.
	inline CMDIFrameWnd* GetParentFrame() { 
#ifdef _DEBUG
		if(!IsAttached())
			ASSERT(m_pParentFrame==NULL);
		else
		{
			ASSERT(m_pParentFrame!=NULL);
		#ifndef _XTP_ACTIVEX
			ASSERT(m_pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
		#endif
		}
#endif
		return m_pParentFrame; 
	}


	// Summary: Saves docking state.
	void SaveState()
	{
		ASSERT(m_tab.m_hWnd);
		AfxGetApp()->WriteProfileInt(_T("TabDocking"), _T("Top"), m_tab.GetStyle() & TCS_BOTTOM? FALSE: TRUE);
	}
	// Summary: Loads previous docking state.
	void LoadState()
	{
		ASSERT(m_tab.m_hWnd);
		int nValue = AfxGetApp()->GetProfileInt(_T("TabDocking"), _T("Top"), TRUE);
		if (nValue) m_tab.ModifyStyle(TCS_BOTTOM, 0); else m_tab.ModifyStyle(0, TCS_BOTTOM);
	}
	
// Overrides
//:Ignore
	//{{AFX_VIRTUAL(CXTPTabClientWndInPlate)
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType=adjustBorder);
	//}}AFX_VIRTUAL
//:End Ignore

// Implementation
public:
	// Summary: Destroys a CXTPTabClientWndInPlate object, handles cleanup and de-allocation.
	virtual ~CXTPTabClientWndInPlate();

	// Generated message map functions
protected:
	
//:Ignore

	//{{AFX_MSG(CXTPTabClientWndInPlate)
	afx_msg LONG OnMDIActivate(UINT wParam, LONG lParam);
	afx_msg LONG OnMDICreate(UINT wParam, LONG lParam);
	afx_msg LONG OnMDIDestroy(UINT wParam, LONG lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


#ifdef _XTP_ACTIVEX

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()	


	enum {
		dispidFlags = 1L,	
	};
	
#endif

	friend class CXTPTabWorkspaceInPlate;
//:End Ignore

};

/////////////////////////////////////////////////////////////////////////////

#endif// #if !defined(__XTPTABCLIENTWNDINPPC_H__)
