// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// 3DView.h : interface of the C3DView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DVIEW_H__0D78CFE4_B411_4363_B959_CD7C061C19F1__INCLUDED_)
#define AFX_3DVIEW_H__0D78CFE4_B411_4363_B959_CD7C061C19F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "APlateCalc.h"

class C3DView : public CView,
					 public CFemModelingDraw
{
protected: // create from serialization only
	C3DView();
	DECLARE_DYNCREATE(C3DView)

public:		
	enum  ZOOMVIEW_MODE {
		NORMAL, PREROTATE, ROTATE, PREUPDOWN, UPDOWN, PREMOVE, MOVE, PREWINDOW, WINDOW, 
		PREDYNWINDOW, DYNWINDOW, PRELINE, LINE, PREPARALLE, PARALLE, PREDISTLINE, DIST, SELLINE,
		SELOBJECT,MULTISELOBJECT,MIRROROBJECT,TEXTEDIT,WHEELMOUSE,MULTIDELETEOBJECT,
		UPDATEDOM = 100,
		MACRO_SELGET = 101
		}  m_ViewMode;

// Attributes
public:
	CAPlateDoc* GetDocument();

protected:
	int		m_PixelFormat;
	HGLRC	m_hRC;
	HDC		m_hDC;					

	CPoint	m_PreCur, m_OrgRectCur;	
	BOOL m_bCrossDraw;
	BOOL m_bSelRectDraw;
	BOOL m_bDrawCursor;
	CPoint m_CurDrawPoint;
	CPoint m_bPreMousePoint;
	CPoint m_PreMouseRectPoint;
	double m_dCrosssAngle;
	long m_nCrosssLength;

// Operations
public:
	BOOL SetupPixelFormat(CDC *pDC);
	BOOL InitializeOpenGL();			

	// Action
	void SetViewModeCtrl(ZOOMVIEW_MODE A);
	void DrawMouseLine(BOOL bNew);
	void DrawMouseRect(BOOL bNew);
	void DrawRectangle(const CPoint& P1,const CPoint& P2) const;

protected:		

public:	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C3DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(C3DView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint uXy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);		
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);	
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);	
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnZoomAll();
	afx_msg void OnZoomWindow();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomMove();
	afx_msg void OnDxfOutCur();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in 3DView.cpp
inline CAPlateDoc* C3DView::GetDocument()
   { return (CAPlateDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEW_H__0D78CFE4_B411_4363_B959_CD7C061C19F1__INCLUDED_)
