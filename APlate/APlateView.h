// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateView.h : interface of the CAPlateView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEVIEW_H__6086AEA7_F535_427B_99FD_13CA4B8761C4__INCLUDED_)
#define AFX_APLATEVIEW_H__6086AEA7_F535_427B_99FD_13CA4B8761C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Src/APlateCalc/APlateCalc.h"

class CAPlateView : public CDomyunGLView
{
protected: // create from serialization only
	CAPlateView();
	DECLARE_DYNCREATE(CAPlateView)

// Attributes
public:
	CAPlateDoc* GetDocument();

	CToolTipCtrl	m_wndToolTip;
	
	BOOL m_bUseToolTip;
	BOOL m_bModeCursor;
	BOOL m_bOldOwnerMouse;
	
	HCURSOR	m_ArrCur;
	HCURSOR m_MoveCur;
	
	BOOL		m_bClickCircle;
	CPoint		m_ptClick;
	long		m_nClickCircleRadius;

protected:
	BOOL		m_bPainted;
	BOOL		m_bPush;
	BOOL		m_bDomSet;

// Operations
public:
	void	SetHVMode(BOOL bMode) { m_bOnlyVHDraw = bMode; }
	BOOL	IsHVMode() { return m_bOnlyVHDraw; }
	void	SetUseToolTip(BOOL bValue) { m_bUseToolTip = bValue; }
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPlateView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);

	//}}AFX_VIRTUAL
	void	DestroyDialog();
	void	SetPrevExe(BOOL bFirst=FALSE);
	void	SetAutoSave(BOOL bStart);
	void	SetUseHCadBlock(BOOL vUseBlock);
	virtual ~CAPlateView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bMoveCursor;
	BOOL m_bTransZoomMove;
	static UINT	m_nIDEventLock;

	CCalcLineInfoDlg m_dlgCalcLineInfo;
	CCalcStationDlg m_dlgCalcStation;

// Generated message map functions
protected:
	//{{AFX_MSG(CAPlateView)
	afx_msg void OnZoomAll();
	afx_msg void OnZoomDynamic();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomMove();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomPrevious();
	afx_msg void OnZoomRealtime();
	afx_msg void OnZoomWindow();
 	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPageRedraw();
	afx_msg void OnInputEnd_PageRedraw();
	afx_msg void OnSetLayer();
	afx_msg void OnSetDim();
	afx_msg void OnSetFont();
	afx_msg void OnSetLinewidth();
	afx_msg void OnDxfOutCur();
	afx_msg void OnDraw3dmodel();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnOption();
	afx_msg void OnPageNext();
	afx_msg void OnPagePrev();	
	afx_msg void OnButtonEndpoint();
	afx_msg void OnUpdateButtonEndpoint(CCmdUI* pCmdUI);
	afx_msg void OnButtonPerpoint();
	afx_msg void OnUpdateButtonPerpoint(CCmdUI* pCmdUI);
	afx_msg void OnButtonNear();
	afx_msg void OnUpdateButtonNear(CCmdUI* pCmdUI);
	afx_msg void OnButtonCenpoint();
	afx_msg void OnUpdateButtonCenPoint(CCmdUI* pCmdUI);
	afx_msg void OnButtonMidpoint();
	afx_msg void OnUpdateButtonMidPoint(CCmdUI* pCmdUI);
	afx_msg void OnButtonCrosspoint();
	afx_msg void OnUpdateButtonCrossPoint(CCmdUI* pCmdUI);
	afx_msg void OnButtonNode();
	afx_msg void OnUpdateButtonNode(CCmdUI* pCmdUI);			
	afx_msg void OnPageChange();
	afx_msg void OnCalcDist();
	afx_msg void OnObjInfo();
	afx_msg void OnPagePrevFirst();
	afx_msg void OnPageNextLast();
	afx_msg void OnPagepartPrev();
	afx_msg void OnPagepartNext();
	afx_msg void OnInitDrawpage();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnObjMove();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
 	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
 	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdateObjMove(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDxfOut();
	afx_msg void OnAlginAll();
	afx_msg void OnMoveUndo();
	afx_msg void OnMoveRedo();
	afx_msg void OnUpdateMoveRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveUndo(CCmdUI* pCmdUI);
	afx_msg void OnRectTop();
	afx_msg void OnRectBottom();
	afx_msg void OnAlignApplyall();
	afx_msg void OnObjectInfo();	
	afx_msg void OnUpdateOutLine(CCmdUI* pCmdUI);
	afx_msg void OnAlignLeft();
	afx_msg void OnAlignRight();
	afx_msg void OnAlignTop();
	afx_msg void OnAlignBottom();
	afx_msg void OnAlignHorz();
	afx_msg void OnAlignVer();
	afx_msg void OnAttachHorz();
	afx_msg void OnAttachVer();
	afx_msg void OnAlignCenter();
	afx_msg void OnAlignVcenter();
 	afx_msg void OnExportBrx();
	afx_msg void OnUpdateExportBrx(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnInsertNote();
	afx_msg void OnUpdateInsertNote(CCmdUI* pCmdUI);	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnImportBrx();
	afx_msg void OnExportText();
	afx_msg void OnImportText();
	afx_msg void OnUpdateExportText(CCmdUI* pCmdUI);
	afx_msg void OnExportBrx2007();
	afx_msg void OnUpdateExportBrx2007(CCmdUI* pCmdUI);
	afx_msg void OnExportAdmanager();
	afx_msg void OnUpdateExportAdmanager(CCmdUI* pCmdUI);
	afx_msg void OnCalcStation();
	afx_msg void OnCalcLineInfo();
	afx_msg void OnUpdateCalcStation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCalcLineInfo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUndo(WPARAM nID);
	afx_msg void OnRedo(WPARAM nID);
	afx_msg void OnMakeInputmenu();
	afx_msg void OnUpdateDomyunAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlign(CCmdUI* pCmdUI);
	afx_msg void OnAirialView();
	afx_msg BOOL OnToolbarControl(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in APlateView.cpp
inline CAPlateDoc* CAPlateView::GetDocument()
   { return (CAPlateDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLATEVIEW_H__6086AEA7_F535_427B_99FD_13CA4B8761C4__INCLUDED_)
