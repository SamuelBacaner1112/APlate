// PageViewBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "../APlateData/APlateData.h"
#include "../GeneralInput/GeneralInputex.h"
#include "PageViewBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageViewBar dialog
#define WM_PAGE_UPDATE			(WM_USER+1)
#define WM_CLICKMODEL_TAB		(WM_USER+2)

CPageViewBar::CPageViewBar()
{	
	m_bMultiLine = FALSE;
	m_bModelMode = FALSE;	
	m_pIn = NULL;
	m_pDrawStd = NULL;
	m_TabFont = new CFont();
	m_TabFont->CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_PRECIS, 
						 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, 
						 "±¼¸²");		
}

CPageViewBar::~CPageViewBar()
{	
	m_pIn = NULL;
	delete m_TabFont;
}


BEGIN_MESSAGE_MAP(CPageViewBar, CControlBar)
	//{{AFX_MSG_MAP(CPageViewBar)
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DOM, OnSelchangeTab)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageViewBar message handlers

void CPageViewBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL /*bDisableIfNoHndler*/)
{		
	if(m_pIn && m_pIn->GetCurMenuID())
	{		
		if(m_pIn->GetCurMenuID() == 9999)
		{
			long nStt = m_bModelMode ? 1 : 0;	
			long nSize = m_pDrawStd->GetDrawPageMng()->GetPagePartSize();
			long nPage = m_pDrawStd->GetCurPage();
			long nIdx = m_pDrawStd->GetDrawPageMng()->GetIdxOfPage(nPage);			
			if(m_wndTab.GetItemCount()!=nSize+nStt)
			{			
				m_wndTab.DeleteAllItems();
				if(m_bModelMode) 
					m_wndTab.AddTab("MODEL",3);
				for(long i=0;i<nSize;i++)
					m_wndTab.AddTab(m_pDrawStd->GetDrawPageMng()->GetPage(i)->GetPartTitle(),5);			
				if(m_bModelMode) 
					m_wndTab.SetCurSel(nIdx+nStt);
				((CFrameWnd*)GetParent())->RecalcLayout();
			}
			if(m_bModelMode) 	
			{
				if(m_wndTab.GetCurSel() != 0 && nIdx != m_wndTab.GetCurSel()+nStt) m_wndTab.SetCurSel(nIdx+nStt);										
			}
			else
			{
				if(nIdx != m_wndTab.GetCurSel()) m_wndTab.SetCurSel(nIdx);				
			}
		}
		else if(m_pIn->GetCurruntDlg())
		{
			CGeneralBaseDlg *pDlg = m_pIn->GetCurruntDlg();
			BOOL bDiffrent = FALSE;

			if(pDlg->GetTabCount() != m_wndTab.GetItemCount()) bDiffrent = TRUE;
			else
			{
				for(long n=0; n<m_wndTab.GetItemCount(); n++)
				{
					char label[64];
					TC_ITEM item;
					item.mask = TCIF_TEXT|TCIF_IMAGE;
					item.pszText = label;     
					item.cchTextMax = 63;    	
					if (!m_wndTab.GetItem(n, &item))		continue;					
					if(CString(item.pszText) != pDlg->GetTabTitle(n))
					{
						bDiffrent = TRUE;
						break;
					}
				}
			}
			
			if(bDiffrent)
			{
				m_wndTab.DeleteAllItems();
				for(long n=0; n<pDlg->GetTabCount(); n++)
				{
					m_wndTab.AddTab(pDlg->GetTabTitle(n));
				}				
			}
			m_wndTab.SetCurSel(pDlg->m_nTab);
		}	
		else
			m_wndTab.DeleteAllItems();
	}	
	else
			m_wndTab.DeleteAllItems();
}

BOOL CPageViewBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	dwStyle |= WS_CLIPSIBLINGS;
	// force WS_CLIPSIBLINGS (avoids SetWindowPos bugs)
	m_dwStyle = (dwStyle & CBRS_ALL);

	if (nID == AFX_IDW_TOOLBAR)
	{
		m_dwStyle |= CBRS_HIDE_INPLACE;
	}

	dwStyle &= ~CBRS_ALL;

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();
	LPCTSTR lpszClass = AfxRegisterWndClass(0, ::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_BTNFACE+1), NULL);

	if (!CWnd::Create(lpszClass, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;	
	
	m_wndTab.Create(WS_CHILD | WS_VISIBLE | (m_bMultiLine ? TCS_MULTILINE : TCS_SINGLELINE) | TCS_OWNERDRAWFIXED,rect,this,IDC_TAB_DOM);		
	m_wndTab.SetFont(m_TabFont);		
	m_ImageList.Create (IDB_TABS_BMP, 15, 5, RGB (255, 255, 255));	
	m_wndTab.SetImageList(&m_ImageList);	
	
	return TRUE;
}

void CPageViewBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CControlBar::OnShowWindow(bShow, nStatus);	
}

void CPageViewBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CControlBar::OnWindowPosChanged(lpwndpos);	
}

LRESULT CPageViewBar::OnSizeParent(WPARAM wParam, LPARAM lParam)
{	
	return CControlBar::OnSizeParent(wParam, lParam);
}

CSize CPageViewBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT(bHorz);
	CSize size = CControlBar::CalcFixedLayout(bStretch, bHorz);
	CRect rectSize;
	rectSize.SetRectEmpty();
	CalcInsideRect(rectSize, bHorz);       // will be negative size	
	//size.cy = 18 - rectSize.Height();
	if(m_wndTab.GetSafeHwnd() && m_wndTab.GetItemCount())
	{
		size.cy = 18 * m_wndTab.GetRowCount() - rectSize.Height();
	}
	else
		size.cy = 18 - rectSize.Height();
		
	return size;
}

void CPageViewBar::SetMultiLine(BOOL bMultiLine)
{
	m_bMultiLine = bMultiLine;
	if(bMultiLine)				
		m_wndTab.ModifyStyle(TCS_SINGLELINE, TCS_MULTILINE);	
	else
		m_wndTab.ModifyStyle(TCS_MULTILINE, TCS_SINGLELINE);		
	
	if(GetParent()->GetSafeHwnd())
		((CFrameWnd *)GetParent())->RecalcLayout();
}

void CPageViewBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	CControlBar::OnWindowPosChanging(lpwndpos);
	CRect rect;
	GetClientRect(rect);
	int minx = min(rect.Width(), lpwndpos->cx);
	int maxx = max(rect.Width(), lpwndpos->cx);
	rect.SetRect(minx-2, rect.bottom - 6, minx, rect.bottom);
	InvalidateRect(rect);
	rect.SetRect(maxx-2, rect.bottom - 6, maxx, rect.bottom);
	InvalidateRect(rect);
}

void CPageViewBar::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here		
	int sel = m_wndTab.GetCurSel();	
	if(m_pIn && m_pIn->GetCurMenuID() == 9999)
	{
		long nStt = m_bModelMode ? 1 : 0;		
		long nIdx = m_wndTab.GetCurSel();
		if(nIdx==0 && m_bModelMode) 
		{
			GetParent()->SendMessage(WM_CLICKMODEL_TAB,2,0);
		}
		else
		{
			COutMsgDlg pmsgDlg(m_pDrawStd->m_pWnd,m_pDrawStd->GetDrawPageMng());
			m_pDrawStd->m_nCurPage = m_pDrawStd->GetDrawPageMng()->GetPageByIdx(nIdx - nStt);
			GetParent()->SendMessage(WM_PAGE_UPDATE,0,0);
			pmsgDlg.Run();		
		}
	}
	else
	{
		if(m_pIn->GetCurruntDlg())	m_pIn->GetCurruntDlg()->OnSelChangedTab(sel);			
	}
	
	*pResult = 0;
}

void CPageViewBar::OnSize(UINT nType, int cx, int cy) 
{
	CControlBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);	

	if(m_wndTab.GetSafeHwnd())
		m_wndTab.SetWindowPos(&wndTop,1,1,rect.Width()-1,501,/*rect.Height()-1,*/SWP_DRAWFRAME);	
}

int CPageViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}
