// ShCheckButton.cpp : implementation file
//

#include "stdafx.h"
#include "ShCheckButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShCheckButton

CShCheckButton::CShCheckButton()
{
	m_bCaptured			= FALSE;
	m_bCheck			= FALSE;
	m_bOnOff			= FALSE;

	m_btnCheckOn.LoadBitmap(IDB_CHECK_ON);
	m_btnCheckOff.LoadBitmap(IDB_CHECK_OFF);
}

CShCheckButton::~CShCheckButton()
{
}


BEGIN_MESSAGE_MAP(CShCheckButton, CButton)
	//{{AFX_MSG_MAP(CShCheckButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShCheckButton message handlers

void CShCheckButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// Highlight the button when it is auto-repeating
	CDC* pDC	= CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect	= lpDrawItemStruct->rcItem;
	UINT state	= lpDrawItemStruct->itemState;	// pushed?

	CString strText;
	GetWindowText(strText);

	if(state & ODS_SELECTED)
		pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_FLAT | DFCS_BUTTONPUSH | DFCS_PUSHED);
	else
		pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_FLAT | DFCS_BUTTONPUSH);

	// Resize button in case its label was changed (not likely, but just in case)
	rect.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));

	pDC->FillSolidRect(rect, m_bOnOff ? RGB(220, 220, 220) : RGB(128,128,128));	// yellow means we are auto-repeating

	if(!strText.IsEmpty())
	{
		CSize Extent = pDC->GetTextExtent(strText);
		CPoint pt(rect.CenterPoint().x - Extent.cx/2,
			rect.CenterPoint().y - Extent.cy/2);

        if(state & ODS_SELECTED)
		{
            pt.Offset(1,1);
		}

        int nMode = pDC->SetBkMode(TRANSPARENT);

        if (state & ODS_DISABLED)
		{
            pDC->DrawState(pt, Extent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
        else
		{
			// Use this (not TextOut()) to retain the '&' indicators
			CDC dcBitmap;
			dcBitmap.CreateCompatibleDC(pDC);
			CBitmap *pOldBitmap;										
			pOldBitmap = dcBitmap.SelectObject(m_bOnOff ? &m_btnCheckOn : &m_btnCheckOff);
			pDC->BitBlt(7, (rect.Height()-10)/2, 16, 13, &dcBitmap, 0, 0,SRCCOPY);
			dcBitmap.SelectObject(pOldBitmap);							
			rect.left += 25;
			pDC->DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

        pDC->SetBkMode(nMode);
    }
}

void CShCheckButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(point.x<=20)
	{
		m_bCheck = m_bOnOff = m_bOnOff ? FALSE : TRUE;

		CWnd *pWnd = GetOwner();
		NMHDR hdr;
		hdr.hwndFrom = m_hWnd;
		hdr.idFrom   = GetDlgCtrlID();
//		hdr.code     = nMessage;
		if(pWnd && IsWindow(pWnd->m_hWnd))
			pWnd->SendMessage(WM_USER_BUTTON_PUSH, hdr.idFrom);
		Invalidate();
		
		return;
	}

	if (!m_bCaptured)
	{
		SetCapture ();
		m_bCaptured = TRUE;
	}	
	Invalidate ();
	UpdateWindow ();

	CButton::OnLButtonDown(nFlags, point);	
}

void CShCheckButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	Invalidate ();
	UpdateWindow ();

	CButton::OnLButtonUp(nFlags, point);

	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;
	}	

	if(point.x>30) 
	{
		m_bCheck = m_bCheck ? FALSE : TRUE;
		Invalidate();
	}

}

void CShCheckButton::SetCheck(BOOL bCheck)
{
	m_bCheck = m_bOnOff = bCheck;
	Invalidate();
	UpdateWindow();
}
