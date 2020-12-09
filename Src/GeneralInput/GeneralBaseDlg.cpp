// PPCInputBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralBaseDlg dialog

//CString CGeneralBaseDlg::m_szTitle = "";

CGeneralBaseDlg::CGeneralBaseDlg(UINT nID, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(nID, pParent)
{
	//{{AFX_DATA_INIT(CGeneralBaseDlg)
	//}}AFX_DATA_INIT
	m_bCenter = FALSE;
	m_nIDD =  nID;
	m_nMenuID = -1;
	m_pStd = NULL;	
//	m_bRememberSize = FALSE;			// 사이즈 저장 안함	
	m_bErrorInclude = FALSE;
	m_bUsedFlag = FALSE;				// 다이얼로그 사용여부 검사
	m_bChildDlg = FALSE;
	m_bEnablePin = TRUE;
	m_bGL = FALSE;
	m_nTab = 0;
	m_bOnInitDialog = FALSE;
	m_dScaleDim = 100;
	m_bModifyData	= FALSE;
}

void CGeneralBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralBaseDlg)
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CGeneralBaseDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CGeneralBaseDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()	
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(ID_BUTTON_APPLY, OnButtonApply)		
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
	ON_WM_TIMER()
    ON_WM_NCLBUTTONDOWN() 
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()		
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HIS_PREV,OnPrevHis)
	ON_COMMAND(ID_HIS_NEXT,OnNextHis)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRIDN, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRIDN, OnCellChangedGridN)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralBaseDlg message handlers

int CGeneralBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTResizeDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	

	if(!m_bCenter && m_bChildDlg)		
	{		
//		GetWindowText(m_szTitleMain);
//		m_szTitle = m_szTitleMain;
		ModifyStyle(WS_POPUPWINDOW | WS_THICKFRAME | WS_BORDER | WS_DLGFRAME, WS_CHILD);
	}

	return 0;
}

void CGeneralBaseDlg::CreateEx(CWnd *pParent, BOOL bCenter)
{
	//일단 막음
	//m_bCenter = bCenter;
	m_pView = (CDomyunGLView *)pParent;
	m_bChildDlg = FALSE;
	CXTResizeDialog::Create(m_nIDD, pParent);
}

void CGeneralBaseDlg::CreateByChild(CWnd *pParent, CWnd *pBar)
{
	m_pView = (CDomyunGLView *)pParent;	
	m_bChildDlg = TRUE;
	CXTResizeDialog::Create(m_nIDD, pParent);
	SetParent(pBar);
}

void CGeneralBaseDlg::MoveDialog()
{
	ASSERT(GetParent());

	CWnd *pBar = ((CFrameWnd *)AfxGetMainWnd())->GetMessageBar();

	CRect	wRect, rcParent,rcDlg, rcStats(0,0,0,0);
	int		nLeft, nTop;
	GetParent()->GetClientRect(&rcParent);
	wRect = rcParent;
	GetParent()->ClientToScreen(&wRect);
	GetWindowRect(&rcDlg);
	if(pBar) pBar->GetClientRect(&rcStats);	

	nLeft   = wRect.left + ((rcParent.Width() - rcDlg.Width() ) / 2);
	nTop    = wRect.bottom - rcDlg.Height() - rcStats.Height() - 2;
	MoveWindow(nLeft, nTop,rcDlg.Width(),rcDlg.Height());	
}

void CGeneralBaseDlg::OnDestroy() 
{
	m_pStd->SetCurruntDialog(NULL);
	CXTResize::RemoveAllControls();
	CDialog::OnDestroy();
}


void CGeneralBaseDlg::OnButtonNext()
{	
	UpdateData();
	SetDataSave();
 	SetDataBx();
	m_pView->GetDomyun()->ClearEtt(TRUE);
	m_pView->GetDomyun()->ZoomAll();
	m_pStd->MoveNextDialog();
}

void CGeneralBaseDlg::OnButtonPrev()
{
	UpdateData();
	SetDataSave();
 	SetDataBx();
	m_pView->GetDomyun()->ClearEtt(TRUE);
	m_pView->GetDomyun()->ZoomAll();
	
	m_pStd->MovePrevDialog();		
}

void CGeneralBaseDlg::OnButtonApply()
{
	UpdateData();
	SetDataSave();
 	SetDataBx();
	SetDataInit();

	DrawInputDomyunView(FALSE);
}

void CGeneralBaseDlg::OnButtonRecommend()
{
	SetDataDefault();
 	SetDataBx();
	SetDataInit();
	//가변구간처리때문에...
	SetDataSave();

	DrawInputDomyunView(FALSE);
}

BOOL CGeneralBaseDlg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();	
	m_bOnInitDialog = TRUE;

	if(GetView()) GetView()->SetGL(m_bGL);

	OnPreInitDialog(); 

	if(m_bCenter) CenterWindow();
	else		  MoveDialog();

	if(!IsUsedFlag()) SetDataDefault();
	SetDataInit();	

 	SetDataBx();						

 	DrawInputDomyunView();

	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			pGrid->ResetSelection();
			pGrid->SetFocusCell(pGrid->GetFixedRowCount(),pGrid->GetFixedColumnCount());		
			pGrid->SetTextBkColor(RGB(225, 250, 250));
		}
	}

	// Button Setting
	SetResize(ID_BUTTON_APPLY,	   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(ID_BUTTON_NEXT,	   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(ID_BUTTON_PREV,	   SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(ID_BUTTON_RECOMMEND,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetUsedFlag(TRUE);		
	SetDlgTitle();	
	m_bOnInitDialog = FALSE;

	return TRUE;  	              
}

void CGeneralBaseDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	if(!m_pView) return;
	if(bZoomAll) m_pView->ZoomAll();
	else 		 m_pView->RedrawAll();
}

void CGeneralBaseDlg::OnCellChangedData(NMHDR*nmgv,LRESULT*)
{
	SetDataSave();	
	SetDataBx();
	DrawInputDomyunView(FALSE);
}

void CGeneralBaseDlg::OnCellChanged(NMHDR*nmgv,LRESULT*)
{
	if(GetDlgItem(IDC_GRID)) 
	{
		NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	
		int nRow = pGridView->iRow;
		int nCol = pGridView->iColumn;
		OnCursorChange(nRow, nCol);
	}
}

void CGeneralBaseDlg::OnCellChangedGridN(NMHDR*nmgv,LRESULT*)
{
	if(GetDlgItem(IDC_GRIDN)) 
	{
		NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	
		int nRow = pGridView->iRow;
		int nCol = pGridView->iColumn;
		OnCursorChange(nRow, nCol);		
	}
}

void CGeneralBaseDlg::DrawTitleButton()
{
// 	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
// 	if(!(style & WS_MINIMIZEBOX)) return;
// 	if(IsIconic()) return;
// 	
//     // Put the close button on the caption
// 	DrawButton(m_bPressPin ? _BUTTON_PUSH_ : _BUTTON_UP_);
// 	DrawButton(_BUTTON_POS_UP_);
// 	DrawButton(_BUTTON_HELP_UP_);
// 	DrawButton(_BUTTON_PREV_UP_);
// 	DrawButton(_BUTTON_NEXT_UP_);
}

// Here we paint
void CGeneralBaseDlg::OnNcPaint() 
{
	// If you only change the menu bar, let the framework draw 
	// the window first
	Default();	
	DrawTitleButton();
}

BOOL CGeneralBaseDlg::OnNcActivate(BOOL bActive) 
{
   // If you want different look when inactive change this
   DrawTitleButton(); 
   return TRUE; 
}

DWORD CGeneralBaseDlg::HitTest(CPoint pt)
{
	// Check for buttons
	CRect rect=m_rcPosition;
	if(rect.PtInRect(pt))
		return (DWORD) _BUTTON_PUSH_;	

	if(m_rcPin.PtInRect(pt))
		return (DWORD) _BUTTON_POS_PUSH_;

	if(m_rcHelp.PtInRect(pt))
		return (DWORD) _BUTTON_HELP_PUSH_;

	if(m_rcPrev.PtInRect(pt))
		return (DWORD) _BUTTON_PREV_PUSH_;

	if(m_rcNext.PtInRect(pt))
		return (DWORD) _BUTTON_NEXT_PUSH_;

	return (DWORD) _BUTTON_UP_;
}

void CGeneralBaseDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	CPoint pt=point;
	ScreenToClient(&pt);
	pt.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	pt.x += 5;

	DWORD hitTest = HitTest(pt);

	switch(hitTest)
	{
	case _BUTTON_PUSH_:
	case _BUTTON_POS_PUSH_:
	case _BUTTON_HELP_PUSH_:
	case _BUTTON_PREV_PUSH_:
	case _BUTTON_NEXT_PUSH_:
			break;
	default:
			Default();
			break;
	}
}

void CGeneralBaseDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{ 
	// Coords are relative to screen
	CPoint pt=point;
	// Convert for relative to client area
	ScreenToClient(&pt);
	// Adjust for client area
	pt.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	pt.x += 5;

	// Now test for button
	DWORD hitTest = HitTest(pt);

	switch(hitTest)
	{
	case _BUTTON_PUSH_:
			{
//			m_bPressPin = !m_bPressPin;
//			AfxGetApp()->WriteProfileInt("Settings", "PressPin",CXTResizeDialog::m_bPressPin);
//		    	DrawButton(CXTResizeDialog::m_bPressPin ? _BUTTON_PUSH_ : _BUTTON_UP_);
//			AfxGetMainWnd()->SendMessage(WM_USER+47,GetMenuID());
			//ResizeParentWindowOrgin(m_bPressPin);			
			//if(!m_bPressPin) 
			//{
			//	ResizeWindowOrgin();
			//}
			//MoveDialog();			
			}
			break;
	case _BUTTON_POS_PUSH_:
			{
			DrawButton(hitTest);
			m_LastHit = hitTest;
			m_ButtonDownPin = hitTest;
			SetCapture();
			}
			break;
	case _BUTTON_HELP_PUSH_:
			{			
			DrawButton(hitTest);	
			m_LastHit = hitTest;
			m_ButtonDownPin = hitTest;
			SetCapture();
			}
			break;
	case _BUTTON_PREV_PUSH_:
			if(m_pStd->IsPrevHistory())
			{
			DrawButton(hitTest);	
			m_LastHit = hitTest;
			m_ButtonDownPin = hitTest;
			SetCapture();
			}
			break;
	case _BUTTON_NEXT_PUSH_:
			if(m_pStd->IsNextHistroy())
			{
			DrawButton(hitTest);	
			m_LastHit = hitTest;
			m_ButtonDownPin = hitTest;
			SetCapture();
			}
			break;
	default:
			Default(); 
			break;
	}
} 

void CGeneralBaseDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(this != GetCapture())
		return;
	
	// Coords are relative to window now
	CPoint pt=point;
	// Adjust for client area
	point.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	point.x += 5;

	// Now test for button
	DWORD hitTest = HitTest(point);
	switch(m_ButtonDownPin) // Release the pressed button
	{
	case _BUTTON_POS_PUSH_:
			DrawButton(_BUTTON_POS_UP_);			
			break;
	case _BUTTON_HELP_PUSH_:
			DrawButton(_BUTTON_HELP_UP_);			
			break;
	case _BUTTON_PREV_PUSH_:
			DrawButton(_BUTTON_PREV_UP_);			
			break;
	case _BUTTON_NEXT_PUSH_:
			DrawButton(_BUTTON_NEXT_UP_);			
			break;
	default:
			break;
	}

	switch(hitTest)		// Process command if released at on a button
	{
	case _BUTTON_POS_PUSH_:		
		//if(IsZoomed()) ShowWindow(SW_RESTORE);
		//if(!m_bPressPin) ResizeWindowOrgin();
		//else			 ResizeParentWindowOrgin();
//		ResizeWindowOrgin();
		MoveDialog();
		Invalidate();
			break;
	case _BUTTON_HELP_PUSH_:		
		SendMessage(WM_COMMAND,ID_HELP);
			break;
	case _BUTTON_PREV_PUSH_:			
		SendMessage(WM_COMMAND,ID_HIS_PREV);
			break;
	case _BUTTON_NEXT_PUSH_:
		SendMessage(WM_COMMAND,ID_HIS_NEXT);
			break;
	default:
			break;
	}	
	m_ButtonDownPin = 0;
	// Release mouse capture
	ReleaseCapture();
}

#if( _MFC_VER == 0x600 )
	UINT CGeneralBaseDlg::OnNcHitTest(CPoint point) 
#else
	LRESULT CGeneralBaseDlg::OnNcHitTest(CPoint point) 
#endif;
{
	if(this != GetCapture())
		return CXTResizeDialog::OnNcHitTest(point); // The default handler

	// Coords are relative to screen
	CPoint pt=point;
	// Convert for relative to client area
	ScreenToClient(&pt);
	// Adjust for client area
	pt.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	pt.x += 5;

	// Now test for button
	DWORD hitTest = HitTest(pt);

	if(hitTest == m_LastHit)
		return CXTResizeDialog::OnNcHitTest(point);

	m_LastHit = hitTest;

	DrawButton(hitTest);	
	
	// return value	
	return hitTest;
}

void CGeneralBaseDlg::DrawButton(int nType)
{
	if(m_bChildDlg) return;

	CWindowDC dc(this);

    CRect rc;
    GetWindowRect(rc);

    // Area for buttons
	int nSpace = 1;
	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
	if(style & WS_THICKFRAME) nSpace = 2;

    CRect posRect, pinRect, helpRect, prevRect, nextRect;

	prevRect.left = rc.right - rc.left - 25 - 156;
    prevRect.top = GetSystemMetrics(SM_CYFRAME) + nSpace;
    prevRect.right = rc.right - rc.left - 0 - 156;
    prevRect.bottom = GetSystemMetrics( SM_CYSIZE ) + nSpace;	

	nextRect.left = rc.right - rc.left - 25 - 128;
    nextRect.top = GetSystemMetrics(SM_CYFRAME) + nSpace;
    nextRect.right = rc.right - rc.left - 0 - 128;
    nextRect.bottom = GetSystemMetrics( SM_CYSIZE ) + nSpace;	

    helpRect.left = rc.right - rc.left - 20 - 100;
    helpRect.top = GetSystemMetrics(SM_CYFRAME) + nSpace;
    helpRect.right = rc.right - rc.left - 0 - 100;
    helpRect.bottom = GetSystemMetrics( SM_CYSIZE ) + nSpace;	

    posRect.left = rc.right - rc.left - 20 - 78;
    posRect.top = GetSystemMetrics(SM_CYFRAME) + nSpace;
    posRect.right = rc.right - rc.left - 0 - 78;
    posRect.bottom = GetSystemMetrics( SM_CYSIZE ) + nSpace;

	pinRect.left = rc.right - rc.left - 20 - 58;
    pinRect.top = GetSystemMetrics(SM_CYFRAME) + nSpace;
    pinRect.right = rc.right - rc.left - 0 - 58;
    pinRect.bottom = GetSystemMetrics( SM_CYSIZE ) + nSpace;


	switch(nType)
	{
	case _BUTTON_UP_:
		{
			dc.DrawFrameControl(posRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(IDB_BITMAP_PINSHAPE);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(posRect.left+2,posRect.top+2,16,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcPosition = posRect;
		}
		break;
	case _BUTTON_PUSH_:
		{
			dc.DrawFrameControl(posRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH | DFCS_PUSHED );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(IDB_BITMAP_PINSHAPE1);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(posRect.left+2,posRect.top+2,16,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcPosition = posRect;
		}
		break;
	case _BUTTON_POS_PUSH_:
		{
			dc.DrawFrameControl(pinRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH | DFCS_PUSHED );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(IDB_BITMAP_POSSHAPE);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(pinRect.left+3,pinRect.top+3,16,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcPin = pinRect;
		}
		break;
	case _BUTTON_POS_UP_:
		{
			dc.DrawFrameControl(pinRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(IDB_BITMAP_POSSHAPE);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(pinRect.left+2,pinRect.top+2,16,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcPin = pinRect;
		}
		break;
	case _BUTTON_HELP_PUSH_:
		{
			dc.DrawFrameControl(helpRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH | DFCS_PUSHED );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(IDB_BITMAP_HELP_INPUT);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(helpRect.left+2,helpRect.top+2,16,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcHelp = helpRect;
		}
		break;
	case _BUTTON_HELP_UP_:
		{
			dc.DrawFrameControl(helpRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(IDB_BITMAP_HELP_INPUT);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(helpRect.left+2,helpRect.top+2,16,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcHelp = helpRect;
		}
		break;
	case _BUTTON_PREV_PUSH_:
		{
			dc.DrawFrameControl(prevRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH | DFCS_PUSHED );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(m_pStd->IsPrevHistory() ? IDB_BITMAP_HIS_PREV : IDB_BITMAP_HIS_PREV_NO);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(prevRect.left+2,prevRect.top+2,28,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcPrev = prevRect;
		}
		break;
	case _BUTTON_PREV_UP_:
		{
			dc.DrawFrameControl(prevRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(m_pStd->IsPrevHistory() ? IDB_BITMAP_HIS_PREV : IDB_BITMAP_HIS_PREV_NO);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(prevRect.left+2,prevRect.top+2,28,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcPrev = prevRect;
		}
		break;
	case _BUTTON_NEXT_PUSH_:
		{
			dc.DrawFrameControl(nextRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH | DFCS_PUSHED );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(m_pStd->IsNextHistroy() ? IDB_BITMAP_HIS_NEXT : IDB_BITMAP_HIS_NEXT_NO);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(nextRect.left+2,nextRect.top+2,28,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcNext = nextRect;
		}
		break;
	case _BUTTON_NEXT_UP_:
		{
			dc.DrawFrameControl(nextRect,
			DFC_BUTTON,
			DFCS_BUTTONPUSH );
			
			CBitmap bitmap, *oldbitmap;
			CDC BitmapDC;
			bitmap.LoadBitmap(m_pStd->IsNextHistroy() ? IDB_BITMAP_HIS_NEXT : IDB_BITMAP_HIS_NEXT_NO);
			BitmapDC.CreateCompatibleDC(&dc);
			oldbitmap = BitmapDC.SelectObject(&bitmap);
			dc.BitBlt(nextRect.left+2,nextRect.top+2,28,10,&BitmapDC,0,0,SRCCOPY);
			BitmapDC.SelectObject(oldbitmap);

			m_rcNext = nextRect;
		}
		break;
	}
}

void CGeneralBaseDlg::OnCursorChange(long nRow, long nCol)
{
	if(m_pView)	m_pView->GetDomyun()->DeleteCursor();	
}

void CGeneralBaseDlg::OnSize(UINT nType, int cx, int cy)
{	
// 	CXTResizeDialog::OnSize(nType, cx, cy);
	
	if (m_bResizeControl)
		CXTResizeDialog::OnSize(nType, cx, cy);
	else
		CDialog::OnSize(nType, cx, cy);

// 	if(GetDlgItem(IDC_GRID)) 
// 	{
// 		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRID);
// 		pGrid->SetRedraw(TRUE,TRUE);
// 	}
	Invalidate();
}

BOOL CGeneralBaseDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(GetDlgItem(IDC_GRIDN)) 
	{
		if(((CGridCtrl *)GetDlgItem(IDC_GRIDN))->TranslateMsg(pMsg))
			return ((CGridCtrl *)GetDlgItem(IDC_GRIDN))->PreTranslateMessage(pMsg);		
	}
	else if(GetDlgItem(IDC_GRID)) 
	{
		if(((CGridCtrl *)GetDlgItem(IDC_GRID))->TranslateMsg(pMsg))
			return ((CGridCtrl *)GetDlgItem(IDC_GRID))->PreTranslateMessage(pMsg);		
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam >= VK_F2 && pMsg->wParam <= VK_F6)
	{
		switch(pMsg->wParam)
		{
		case VK_F3:	AfxGetMainWnd()->SendMessage(WM_COMMAND, 201); break;
		case VK_F4:	AfxGetMainWnd()->SendMessage(WM_COMMAND, 203); break;
		case VK_F5:	AfxGetMainWnd()->SendMessage(WM_COMMAND, 202); break;
		case VK_F6:	AfxGetMainWnd()->SendMessage(WM_COMMAND, 32853); break;
		case VK_F7:	AfxGetMainWnd()->SendMessage(WM_COMMAND, 32849); break;		
		}
	}
	if(pMsg->message==WM_KEYDOWN && IsCTRLpressed() && (pMsg->wParam=='G' || pMsg->wParam=='g'))
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_TOTALVIEW);
		if(pButton)
		{
			pButton->SetCheck(pButton->GetCheck() ? 0 : 1);
			SendMessage(WM_COMMAND,MAKEWPARAM(IDC_CHECK_TOTALVIEW, BN_CLICKED), (LPARAM)m_hWnd);
		}	
	}		
	CWnd *pWnd = FromHandle(pMsg->hwnd);
	TCHAR szClass [256];
	::GetClassName (pMsg->hwnd, szClass, 255);
	CString strClass = szClass;
	if(pWnd && strClass=="Edit" && pWnd->GetFocus() != pWnd && pMsg->message == WM_LBUTTONDOWN)
	{	
		CXTResizeDialog::PreTranslateMessage(pMsg);
		((CEdit *)pWnd)->SetSel(0,-1);
		return TRUE;
	}	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CGeneralBaseDlg::OnOK()
{	
	if( GetKeyState(VK_RETURN) < 0 ) 
	{		
		keybd_event((unsigned char)VkKeyScan(VK_TAB),0,KEYEVENTF_EXTENDEDKEY,0);							
		keybd_event((unsigned char)VkKeyScan(VK_TAB),0,KEYEVENTF_KEYUP,0);	
		return;
	}

	CXTResizeDialog::OnOK();
}

void CGeneralBaseDlg::OnCancel()
{
	if( GetKeyState(VK_ESCAPE) < 0 ) return;

	CXTResizeDialog::OnCancel();
}

void CGeneralBaseDlg::SetDlgTitle()
{
	// 다이얼로그 이름 지정
//	SetWindowText(m_szTitle);

	//if(m_bPressPin) GetParent()->Invalidate();
	
	DrawTitleButton();	
}

BOOL CGeneralBaseDlg::IsFocusItem(CString szTitle)
{
	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(0,nCol)==szTitle)
				{
					return TRUE;					
				}
			}
		}
	}
	return FALSE;
}

void CGeneralBaseDlg::SetFocusItem(CString szTitle)
{
	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			long nRow = pGrid->GetFocusCell().row;
			if(nRow < 0) nRow=1;
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(0,nCol)==szTitle)
				{
					GetFocus();
					pGrid->ResetSelection();
					pGrid->SetFocusCell(nRow,nCol);					
					pGrid->SendMessage(WM_KEYDOWN,VK_F2);
					break;
				}
			}
		}
	}
	if(GetDlgItem(IDC_GRIDN)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRIDN);
		if(pGrid->GetSafeHwnd())
		{
			long nRow = pGrid->GetFocusCell().row;
			
			if(nRow < 0) nRow=1;
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(0,nCol)==szTitle)
				{
					GetFocus();
					pGrid->ResetSelection();
					pGrid->SetFocusCell(nRow,nCol);					
					pGrid->SendMessage(WM_KEYDOWN,VK_F2);
					break;
				}
			}
		}
	}
}

void CGeneralBaseDlg::OnPrevHis()
{
	m_pStd->GoPrevHistory();
}

void CGeneralBaseDlg::OnNextHis()
{
	m_pStd->GoNextHistory();
}

// 컨트롤들을 xyMove만큼 이동시킴
// nspeed 이동 속도
// nBountMount 이동시 바운드 되는 비율.
void CGeneralBaseDlg::MoveControlGroup(CDoubleArray &arrID, CPoint xyMove, long nSpeed, long nDelay, double dBoundRate)
{

	CPoint xyTotalVector(0, 0);
	CPoint xyVector(0, 0);
	
	CRect rect;
	long nX = xyMove.x > 0 ? 1 : -1;
	long nY = xyMove.y > 0 ? 1 : -1;

	// 바운드 위치 까지 옮김.
	CPoint xyBound((int)(xyMove.x + ((double)xyMove.x * dBoundRate)), (int)(xyMove.y + ((double)xyMove.y * dBoundRate)));
	BOOL bArriveBoundX = FALSE;
	BOOL bArriveBoundY  = FALSE;

	while(!bArriveBoundX || !bArriveBoundY)
	{
		xyTotalVector.x += !bArriveBoundX ? nSpeed*nX : 0;
		xyTotalVector.y += !bArriveBoundY ? nSpeed*nY : 0;
		xyVector.x = !bArriveBoundX ? nSpeed*nX : 0;
		xyVector.y = !bArriveBoundY ? nSpeed*nY : 0;

		if(abs(xyBound.x) <= abs(xyTotalVector.x)) bArriveBoundX = TRUE;
		if(abs(xyBound.y) <= abs(xyTotalVector.y)) bArriveBoundY = TRUE;
		Sleep(nDelay);
		

		for(long i = 0; i < arrID.GetSize(); i++)
		{
			GetDlgItem((long)arrID[i])->GetWindowRect(&rect);
			ScreenToClient(rect);
			GetDlgItem((long)arrID[i])->SetWindowPos(NULL, rect.left+xyVector.x, rect.top+xyVector.y, rect.Width(), rect.Height(), NULL);
		}
	}

	// 원래 위치 까지 옮김
	nX = -nX;
	nY = -nY;
	BOOL bFinishMoveX = FALSE;
	BOOL bFinishMoveY = FALSE;

	while(!bFinishMoveX || !bFinishMoveY)
	{
		xyTotalVector.x += !bFinishMoveX ? nSpeed*nX : 0;
		xyTotalVector.y += !bFinishMoveY ? nSpeed*nY : 0;
		xyVector.x = !bFinishMoveX ? nSpeed*nX : 0;
		xyVector.y = !bFinishMoveY ? nSpeed*nY : 0;


		if(abs(xyMove.x) >= abs(xyTotalVector.x)) bFinishMoveX = TRUE;
		if(abs(xyMove.y) >= abs(xyTotalVector.y)) bFinishMoveY = TRUE;
		Sleep(nDelay);

		for(long i = 0; i < arrID.GetSize(); i++)
		{
			GetDlgItem((long)arrID[i])->GetWindowRect(rect);
			ScreenToClient(rect);
			GetDlgItem((long)arrID[i])->SetWindowPos(NULL, rect.left+xyVector.x, rect.top+xyVector.y, rect.Width(), rect.Height(), NULL);
		}
	}

	// 조금 벗어난 위치를 보정
	CDPoint xyOver(CDPoint(abs(xyTotalVector.x-xyBound.x), abs(xyTotalVector.y-xyBound.y)));
	nX = -nX;
	nY = -nY;
	xyOver.x *= nX;
	xyOver.y *= nY;
	for(long i = 0; i < arrID.GetSize(); i++)
	{
		GetDlgItem((long)arrID[i])->GetWindowRect(rect);
		ScreenToClient(rect);
		GetDlgItem((long)arrID[i])->SetWindowPos(NULL, (int)(rect.left+xyOver.x), (int)(rect.top+xyOver.y), rect.Width(), rect.Height(), NULL);
	}
}

void CGeneralBaseDlg::OnSelChangedTab(long nTab) 
{ 
	m_nTab = nTab; 
	DrawInputDomyunView(TRUE);
}


BOOL CGeneralBaseDlg::GetMyselfWindow(CWnd **pFindWnd)
{
	HWND hWnd = ::GetDesktopWindow();
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	CString strName;
	while(TRUE)
	{								
		if(hWnd)
		{			
			pWnd = CWnd::FromHandle(hWnd);
			pWnd->GetWindowText(strName);
			if(strName.Find("AEdit")!=-1 && pWnd->IsWindowVisible()) 
			{
				*pFindWnd = pWnd;
				return TRUE;				
			}
		}
		else 
		{
			break;
		}
		if(hWnd == ::GetDesktopWindow())	hWnd = ::GetWindow(hWnd,GW_CHILD);
		else								hWnd = ::GetWindow(hWnd,GW_HWNDNEXT);
		pWnd = CWnd::FromHandle(hWnd);
	}
	pFindWnd = NULL;
	return FALSE;
}

BOOL CGeneralBaseDlg::ContinueMsg()
{
	int ret;
	ret = MessageBox(	"기존에 입력된 정보가 수정됩니다.\n계속 하시겠습니까 ?",
						"기본값", MB_YESNO);
	if(ret == IDYES)	return TRUE;
	return FALSE;
}

void CGeneralBaseDlg::SetModifyInputData(BOOL bModifyData)
{
	AfxGetMainWnd()->SendMessage(WM_USER+300, (WPARAM)m_nMenuID, (LPARAM)bModifyData);
}


void CGeneralBaseDlg::RemoveCaptionBar()
{
	CRect rcWindow;	

	m_pWnd->GetWindowRect(rcWindow);	
	rcWindow.left +=GetSystemMetrics(SM_CXDLGFRAME);
	rcWindow.right -= GetSystemMetrics(SM_CXDLGFRAME)*2;
	rcWindow.bottom-=GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYDLGFRAME)*2;		
	m_bResizeControl = FALSE;	
	ModifyStyle(WS_CAPTION,0);
	SetWindowPos(&wndTop,rcWindow.left,rcWindow.top,rcWindow.Width(), rcWindow.Height(),SWP_NOREDRAW);	

	//m_rcInitWindow = rcWindow;
	//m_rcWindow = m_rcInitWindow;
	m_bResizeControl = TRUE;
}
