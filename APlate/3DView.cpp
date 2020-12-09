// 3DView.cpp : implementation of the C3DView class
//

#include "stdafx.h"
#include "APlateCalc.h"
#include "APlate.h"
#include "APlateDoc.h"
//#include "3DView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DView

IMPLEMENT_DYNCREATE(C3DView, CView)

BEGIN_MESSAGE_MAP(C3DView, CView)
	//{{AFX_MSG_MAP(C3DView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()	
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(ID_ZOOM_WINDOW, OnZoomWindow)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_MOVE, OnZoomMove)
	ON_COMMAND(ID_DXF_OUT_CUR, OnDxfOutCur)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DView construction/destruction

C3DView::C3DView()
{			
	m_ViewMode = NORMAL;

	m_OrgRectCur = m_PreCur = CPoint(0,0);	
	m_bCrossDraw = FALSE;
	m_bSelRectDraw = FALSE;
	m_bDrawCursor = FALSE;
	m_dCrosssAngle = 0;
	m_nCrosssLength = 0;

	SetFlag(VIEW_RENDER_RESTRAINT,TRUE);
	SetFlag(VIEW_ELEMENT_FORCE,TRUE);	
	// Test 용
	//SetFlag(VIEW_RENDER_RESTRAINT,TRUE);
	//SetFlag(VIEW_REACTION,TRUE);	
	//SetFlag(VIEW_JOINT_NUMBER,TRUE);		
}

C3DView::~C3DView()
{
	wglMakeCurrent(m_hDC, NULL);	
}

BOOL C3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// C3DView drawing

void C3DView::OnDraw(CDC* pDC)
{
	if(!GetFEMManage()) return;

	DrawMouseLine(FALSE);
	DrawMouseRect(FALSE);

	Render(pDC);	

	// 마우스 그림
	if(GetFocus()==this)
	{	
		DrawMouseLine(TRUE);
		DrawMouseRect(TRUE);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// C3DView printing

BOOL C3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void C3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void C3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// C3DView diagnostics

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CView::AssertValid();
}

void C3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAPlateDoc* C3DView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAPlateDoc)));
	return (CAPlateDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C3DView message handlers

int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;		
	
	InitializeOpenGL();		
	return 0;
}

BOOL C3DView::SetupPixelFormat(CDC *pDC)
{
  static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |            // support OpenGL
        PFD_DOUBLEBUFFER,               // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 24-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        32,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    
	m_PixelFormat=ChoosePixelFormat(pDC->GetSafeHdc(), &pfd);
	SetPixelFormat(pDC->GetSafeHdc(), m_PixelFormat, &pfd);

    return TRUE;
}

BOOL C3DView::InitializeOpenGL()
{
	CClientDC DC(this);
	m_hDC = DC.GetSafeHdc();

	SetupPixelFormat(&DC);
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);
	
	CFemModelingDraw::InitializeOpenGL(m_hDC);	

	return TRUE;
}

void C3DView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);	
	
	if(GetFEMManage()) SetSize(cx, cy);
}

BOOL C3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint uXy)
{	
	if(zDelta > 0) 	Zoom (0.2);
	else			Zoom (-0.2);			
	Invalidate(FALSE);
	return CWnd::OnMouseWheel(nFlags, zDelta, uXy);
}

BOOL C3DView::OnEraseBkgnd(CDC* pDC) 
{	
	return FALSE;	
}

void C3DView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	OnDraw(&dc);	
}

void C3DView::OnInitialUpdate() 
{
	//SetTimer(0, 500, NULL);
	CAPlateDoc* pDoc = GetDocument();	
	SetFEMManage(pDoc->m_pCalcStd->GetModelingFemData(0));		

	CRect rect;
	GetClientRect(&rect);
	SetSize(rect.Width(), rect.Height());
	CView::OnInitialUpdate();			
}

void C3DView::SetViewModeCtrl(ZOOMVIEW_MODE A)
{
	m_ViewMode = A;
}

void C3DView::DrawRectangle(const CPoint& P1,const CPoint& P2) const
{
	CClientDC dc ((CWnd*)this);
	CPen MyPen (PS_SOLID, 0, RGB_WHITE), *pOldPen;
	int nOldMode = dc.SetROP2 (R2_XORPEN);
	pOldPen       = dc.SelectObject (&MyPen);
	
	dc.MoveTo (P1.x, P1.y);
	dc.LineTo (P1.x, P2.y);
	dc.LineTo (P2.x, P2.y);
	dc.LineTo (P2.x, P1.y);
	dc.LineTo (P1.x, P1.y);	

	dc.SelectObject (pOldPen);
	dc.SetROP2 (nOldMode);
}

void C3DView::OnMouseMove(UINT nFlags, CPoint point) 
{
	DrawMouseLine(FALSE);
	DrawMouseRect(FALSE);		
	if (m_ViewMode != PREWINDOW && m_ViewMode != PREMOVE)   
	{
		if (m_ViewMode == WINDOW)   
		{
			// 사각형 그림.
			CClientDC dc (this);
			CPen MyPen (PS_SOLID, 0, RGB_YELLOW), *OldPen;
			int nOldMode = dc.SetROP2 (R2_XORPEN);
			OldPen       = dc.SelectObject (&MyPen);
			DrawRectangle (m_OrgRectCur, m_PreCur);   // 기존 사각형 삭제
			DrawRectangle (m_OrgRectCur, point);   // 현재 사각형 그리기
			dc.SelectObject (OldPen);
			dc.SetROP2 (nOldMode);
		}
		else 
		{
			BOOL bDrawValue = FALSE;										
			switch (m_ViewMode)   {
			case ROTATE :
				Rotate (point.x - m_PreCur.x, point.y - m_PreCur.y, 0);
				break;
			case UPDOWN :
				Zoom ((m_PreCur.y - point.y) / 300.0f);
				break;
			case MOVE :
				Translate (point.x - m_PreCur.x, point.y - m_PreCur.y);
				break;
			case NORMAL :
				if (MK_LBUTTON & nFlags)
					bDrawValue = TRUE;
			}			
			if ((m_ViewMode != NORMAL && m_ViewMode != UPDATEDOM) || bDrawValue)
				Invalidate(FALSE);
		}
		m_PreCur = point;
	}
	
	if(m_ViewMode == NORMAL && GetFlag(VIEW_ELEMENT_FORCE) && (nFlags & MK_LBUTTON))
	{
		if(MousePickingForce(point))
			Invalidate(FALSE);
	}

	if(m_ViewMode != WINDOW)
	{
		DrawMouseLine(TRUE);
		DrawMouseRect(TRUE);	
	}	
	CView::OnMouseMove(nFlags, point);
}

void C3DView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (MK_CONTROL & nFlags && !(MK_SHIFT & nFlags))  
	{ 
		SetViewModeCtrl(ROTATE);
	}
	else if (!(MK_CONTROL & nFlags) && MK_SHIFT & nFlags) // SHIFT Key Pressed, Ctrl No!
	{ 		
		SetViewModeCtrl(UPDOWN);		
	}
	else if (!(MK_CONTROL & nFlags || MK_SHIFT & nFlags))  // SHIFT, Ctrl Key Not Pressed
	{ 
		switch (m_ViewMode)   
		{
			case WINDOW :   
				{
					CRect rect (m_OrgRectCur, point);
					rect.NormalizeRect ();
					CalcClippingVolume (rect.left, rect.right, m_nHeight - rect.bottom, m_nHeight - rect.top, 1.0);
					Invalidate(FALSE);
					SetViewModeCtrl( NORMAL);
				}
				break;
			case PREWINDOW :
				SetViewModeCtrl( WINDOW );
				m_OrgRectCur = point;
				break;			
			case PREMOVE :
				//PushView ();
				SetViewModeCtrl(MOVE);
				break;	
			case MOVE :				
				SetViewModeCtrl(NORMAL);
				break;
			case NORMAL :
				if(GetFlag(VIEW_ELEMENT_FORCE))
				{
					if(MousePickingForce(point))
						Invalidate(FALSE);
				}
				return;										
			default:
				break;
		}	
	}
	m_PreCur = point;
	SetCapture ();
	
	CView::OnLButtonDown(nFlags, point);
}

void C3DView::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	if (m_ViewMode != MOVE && m_ViewMode != WINDOW)   
	{
		SetViewModeCtrl( NORMAL );
	}

	ReleaseCapture ();
	CView::OnLButtonUp(nFlags, point);
}

void C3DView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetViewModeCtrl(MOVE);		
	m_PreCur = point;
	SetCapture ();			
}

void C3DView::OnMButtonUp(UINT nFlags, CPoint point)
{
	SetViewModeCtrl(NORMAL);
	ReleaseCapture ();	
}

void C3DView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (!(MK_CONTROL & nFlags) && MK_SHIFT & nFlags)  // SHIFT Key Pressed, Ctrl No!
	{ 		
		SetViewModeCtrl(MOVE);	
	}

	if ((MK_CONTROL & nFlags) && !(MK_SHIFT & nFlags) ) // Ctrl Key Pressed, SHIFT No!
	{ 
		SetViewModeCtrl(PARALLE);		
	}

	m_PreCur = point;
	SetCapture ();

	CView::OnRButtonDown(nFlags, point);
}

void C3DView::OnRButtonUp(UINT nFlags, CPoint point) 
{	
	SetViewModeCtrl(NORMAL);
	ReleaseCapture ();
	CView::OnRButtonUp(nFlags, point);
}


void C3DView::DrawMouseLine(BOOL bNew)
{
	//CACadDoc* pDoc = GetDocument();	
	CClientDC dc ((CWnd*)this);
	CRect rect;
	CPoint CurPos;
	CPen DrawPen(PS_SOLID,1,RGB(255,255,255)), *OldPen;	

	int LeftHeight = 0, RightHeight = 0;

	dc.SetROP2(R2_XORPEN);

	GetClientRect(&rect);	
	
	if(bNew)	
	{
		if(GetFocus()==this)
		{
			if(m_bCrossDraw) return;
			GetCursorPos(&CurPos);	
			ScreenToClient(&CurPos);
			// 자석 효과
			if(m_bDrawCursor)
			{
				CRect rect(m_CurDrawPoint.x-5,m_CurDrawPoint.y-5,m_CurDrawPoint.x+5,m_CurDrawPoint.y+5); 
				if(rect.PtInRect(CurPos))			
					CurPos = m_CurDrawPoint;
			}		
			m_bPreMousePoint = CurPos;			
			m_bCrossDraw = TRUE;
		}
	}
	else
	{
		if(!m_bCrossDraw) return;
		CurPos = m_bPreMousePoint;		
		m_bCrossDraw = FALSE;
	}

	OldPen = dc.SelectObject((CPen*)&DrawPen);
	LeftHeight = (int)(CurPos.x * tan(ToRadian(m_dCrosssAngle)));
	RightHeight = (int)((rect.Width() - CurPos.x) * tan(ToRadian(m_dCrosssAngle)));

	if(m_nCrosssLength == 0) 
	{
		dc.MoveTo(rect.left,CurPos.y + LeftHeight);
		dc.LineTo(rect.right,CurPos.y - RightHeight);
	}
	else
	{
		dc.MoveTo(CurPos.x-m_nCrosssLength,CurPos.y + LeftHeight);
		dc.LineTo(CurPos.x+m_nCrosssLength,CurPos.y - RightHeight);
	}

	LeftHeight = (int)(CurPos.y * tan(ToRadian(m_dCrosssAngle)));
	RightHeight = (int)((rect.Height() - CurPos.y) * tan(ToRadian(m_dCrosssAngle)));

	if(m_nCrosssLength == 0) 
	{	
		dc.MoveTo(CurPos.x - LeftHeight,rect.top);
		dc.LineTo(CurPos.x + RightHeight,rect.bottom);		
	}
	else
	{
		dc.MoveTo(CurPos.x - LeftHeight,CurPos.y-m_nCrosssLength);
		dc.LineTo(CurPos.x + RightHeight,CurPos.y+m_nCrosssLength);		
	}
	
	dc.SelectObject(OldPen);		
}

void C3DView::DrawMouseRect(BOOL bNew)
{	
	CClientDC dc ((CWnd*)this);
	CRect rect;
	CPoint CurPos;
	CPen DrawPen(PS_SOLID,1,RGB(255,255,255)), *OldPen;

	int LeftHeight = 0, RightHeight = 0;

	dc.SetROP2(R2_XORPEN);

	GetClientRect(&rect);	
	
	if(bNew)	
	{
		if(GetFocus()==this)
		{
			if(m_bSelRectDraw) return;
			GetCursorPos(&CurPos);	
			ScreenToClient(&CurPos);
			m_PreMouseRectPoint = CurPos;			
			m_bSelRectDraw = TRUE;
		}
	}
	else
	{
		if(!m_bSelRectDraw) return;
		CurPos = m_PreMouseRectPoint;		
		m_bSelRectDraw = FALSE;
	}

	OldPen = dc.SelectObject((CPen*)&DrawPen);
	
	// Rectangle	
	dc.MoveTo(CurPos.x - 3,CurPos.y - 3);
	dc.LineTo(CurPos.x + 3,CurPos.y - 3);
	dc.LineTo(CurPos.x + 3,CurPos.y + 3);
	dc.LineTo(CurPos.x - 3,CurPos.y + 3);
	dc.LineTo(CurPos.x - 3,CurPos.y - 3);
	
	dc.SelectObject(OldPen);		
}

BOOL C3DView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(GetFocus()!=this) return CView::OnSetCursor(pWnd, nHitTest, message);	
	if(nHitTest != 0) SetCursor(NULL);
	return TRUE;
	// TODO: Add your message handler code here and/or call default
	
	//return CView::OnSetCursor(pWnd, nHitTest, message);
}

void C3DView::OnContextMenu(CWnd* pWnd, CPoint point) 
{	
	((CAPlateApp *)AfxGetApp())->ShowPopupMenu (IDR_POPUP_FEMZOOM, point, this);
}

void C3DView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);	

	DrawMouseLine(FALSE);
	DrawMouseRect(FALSE);	
}

void C3DView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(MousePickingInfo(this,point)) 
		Invalidate(FALSE);

	CView::OnLButtonDblClk(nFlags, point);
}

void C3DView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case 'j':
	case 'J':
		SetFlag(VIEW_JOINT_NUMBER,!GetFlag(VIEW_JOINT_NUMBER));	
		Invalidate(FALSE);
		break;
	case 'e':
	case 'E':
		SetFlag(VIEW_ELEMENT_NUMBER,!GetFlag(VIEW_ELEMENT_NUMBER));	
		Invalidate(FALSE);
		break;
	case 'c':
	case 'C':
		SetFlag(VIEW_EDGE_CUT,!GetFlag(VIEW_EDGE_CUT));	
		Invalidate(FALSE);
		break;
	case 'r':
	case 'R':
		SetFlag(VIEW_RENDER_RESTRAINT,!GetFlag(VIEW_RENDER_RESTRAINT));	
		Invalidate(FALSE);
		break;
	}

	CView::OnChar(nChar, nRepCnt, nFlags);
}

void C3DView::OnZoomAll() 
{
	CFemModelingDraw::ZoomAll();
	Invalidate(FALSE);
}

void C3DView::OnZoomWindow() 
{
	SetViewModeCtrl( PREWINDOW );	
}

void C3DView::OnZoomIn() 
{
	Zoom(0.1);	
	Invalidate(FALSE);		
}

void C3DView::OnZoomOut() 
{
	Zoom(-0.1);	
	Invalidate(FALSE);	
}

void C3DView::OnZoomMove() 
{
	SetViewModeCtrl(PREMOVE);
}

void C3DView::OnDxfOutCur() 
{
	CDomyun Dom;
	ConvertGLtoDomyun(&Dom);
	Dom.DxfOut();
}

void C3DView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	
}
