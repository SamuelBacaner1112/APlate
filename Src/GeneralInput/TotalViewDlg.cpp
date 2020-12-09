// TotalViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "TotalViewDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTotalViewDlg dialog

#define CLICK_TIMER 379

CTotalViewDlg::CTotalViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTotalViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTotalViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nFlag = 0;
	m_bClickCircle	= FALSE;
	m_DomView.m_bEnablePopupMenu = TRUE;
}


void CTotalViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTotalViewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX,IDC_DOMVIEW_TOTAL,m_DomView);
}


BEGIN_MESSAGE_MAP(CTotalViewDlg, CDialog)
	//{{AFX_MSG_MAP(CTotalViewDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()		
	ON_WM_TIMER()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTotalViewDlg message handlers
void CTotalViewDlg::Clear()
{
	m_DomView.GetDomyun()->ClearEtt(TRUE);
}

void CTotalViewDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if(m_DomView.GetSafeHwnd())
	{
		m_DomView.SetWindowPos(&wndTop,1,1,cx-2,cy-2,SWP_DRAWFRAME);
		m_DomView.ZoomAll();
	}
}

void CTotalViewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

BOOL CTotalViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	CRect rect;
	GetClientRect(&rect);
	m_DomView.SetWindowPos(&wndTop,1,1,rect.Width()-2,rect.Height()-2,SWP_DRAWFRAME);	
	return TRUE;
}

void CTotalViewDlg::OnClose() 
{
	CGeneralBaseDlg *pDlg = m_pStd->GetCurruntDlg();
	if(pDlg)
	{
		CButton *pButton = (CButton *)pDlg->GetDlgItem(IDC_CHECK_TOTALVIEW);	
		if(pButton)	pButton->SetCheck(0);	
	}

	CDialog::OnClose();
}

void CTotalViewDlg::OnCancel() 
{	
	CGeneralBaseDlg *pDlg = m_pStd->GetCurruntDlg();
	if(pDlg)
	{
		CButton *pButton = (CButton *)pDlg->GetDlgItem(IDC_CHECK_TOTALVIEW);	
		if(pButton)	pButton->SetCheck(0);	
	}

	CDialog::OnCancel();
}

BOOL CTotalViewDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && IsCTRLpressed() && (pMsg->wParam=='G' || pMsg->wParam=='g'))
	{
		if(m_pStd->GetCurruntDlg())
		{	
			CButton *pButton = (CButton *)m_pStd->GetCurruntDlg()->GetDlgItem(IDC_CHECK_TOTALVIEW);
			if(pButton)
			{
				pButton->SetCheck(pButton->GetCheck() ? 0 : 1);
				m_pStd->GetCurruntDlg()->SendMessage(WM_COMMAND,MAKEWPARAM(IDC_CHECK_TOTALVIEW, BN_CLICKED), (LPARAM)m_pStd->GetCurruntDlg()->m_hWnd);
			}
		}
	}
	
	if(pMsg->hwnd == m_DomView.GetSafeHwnd() && pMsg->message == WM_LBUTTONDBLCLK)
	{		
		int xPos = LOWORD(pMsg->lParam); 
		int yPos = HIWORD(pMsg->lParam);

		m_ptClick = CPoint(xPos,yPos);
		m_nClickCircleRadius = 1;
		m_bClickCircle = FALSE;
		SetTimer(CLICK_TIMER, 1, NULL);
	}
	if(pMsg->hwnd == m_DomView.GetSafeHwnd() && (pMsg->message == WM_MBUTTONDOWN || pMsg->message == WM_MBUTTONUP))
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	else if(pMsg->message == WM_MOUSEWHEEL)
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CTotalViewDlg::OnTimer(UINT nIDEvent) 
{
	CAPlateDrawJong	DrawJong(m_pStd->m_pDataManage);
	if(nIDEvent == CLICK_TIMER)
	{
		// 이전 원 삭제
		CDC* pDC = m_DomView.GetDC();
		CPen Pen(PS_SOLID,2,RGB_YELLOW), *pOldPen;
		pOldPen = pDC->SelectObject(&Pen);
		pDC->SetROP2(R2_XORPEN);
		CRect rect(m_ptClick.x-m_nClickCircleRadius,m_ptClick.y-m_nClickCircleRadius,
				   m_ptClick.x+m_nClickCircleRadius,m_ptClick.y+m_nClickCircleRadius);
		POINT pt = {m_nClickCircleRadius*2, m_nClickCircleRadius*2};
		if(m_bClickCircle)						
			pDC->Arc(&rect,pt,pt);
		
		if(m_nClickCircleRadius<20)
		{			
			m_nClickCircleRadius += 1;			
			rect = CRect(m_ptClick.x-m_nClickCircleRadius,m_ptClick.y-m_nClickCircleRadius,
				   m_ptClick.x+m_nClickCircleRadius,m_ptClick.y+m_nClickCircleRadius);
			pt.x = m_nClickCircleRadius*2;
			pt.y = m_nClickCircleRadius*2;
			
			pDC->Arc(&rect,pt,pt);					
		}
		else
		{
			KillTimer(CLICK_TIMER);
			CDPoint xyClickPos = m_DomView.ClientToExt(m_ptClick.x, m_ptClick.y);
			DrawJong.SetCenterAngle();
			xyClickPos = GetXyRotateVector(CDPoint(0,0),xyClickPos,DrawJong.m_vRotate);
			CPlateBasicIndex *pBx = m_pStd->GetBridge()->GetBxOnNearByPoint(xyClickPos, m_nFlag, FALSE);
			CGeneralBaseDlg *pDlg = m_pStd->GetCurruntDlg();
			if(pDlg && pBx) pDlg->SendMessage(WM_GET_NEARBX, (WPARAM)pBx, 0);
		}
		pDC->SelectObject(pOldPen);
		ReleaseDC(pDC);
		m_bClickCircle = TRUE;
	}
	
	CDialog::OnTimer(nIDEvent);
}
