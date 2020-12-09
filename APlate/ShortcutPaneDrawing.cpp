// ShortcutPaneDrawing.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "APlate.h"
#include "APlateDoc.h"
#include "ShortcutPaneDrawing.h"
#include <afxcmn.h>	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneDrawing

CShortcutPaneDrawing::CShortcutPaneDrawing()
{
}

CShortcutPaneDrawing::~CShortcutPaneDrawing()
{
}


BEGIN_MESSAGE_MAP(CShortcutPaneDrawing, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneDrawing)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DRAWING, OnSelchangedTreeDrawing)	
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneDrawing message handlers
BOOL CShortcutPaneDrawing::Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	CXTPShortcutBarPane::Create(lpszCaption, pParent);

	m_wndTreeDrawing.Create(WS_VISIBLE|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS, CXTPEmptyRect(), this, IDC_TREE_DRAWING);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_DRAWING);

	m_ImageList.Create(16, 16, ILC_COLOR32|ILC_MASK, 2, 1);
	m_ImageList.Add(&bitmap, RGB(192, 192, 192));
	m_wndTreeDrawing.SetImageList(&m_ImageList, TVSIL_NORMAL);

	COLORREF clrBack = RGB(253, 250, 255);
	m_wndTreeDrawing.SetBkColor(clrBack);

	InitTree();

	return TRUE;
}

void CShortcutPaneDrawing::InitTree()
{
	CMainFrame		*pFrm		= (CMainFrame*)AfxGetMainWnd();
	if(!pFrm) return;
	CAPlateDoc		*pDoc		= (CAPlateDoc *)pFrm->GetDocument();
	CAPlateDrawStd	*pDraw		= pDoc->m_pPlateDrawStd;
	CGeneralBaseStd	*pInput		= pDoc->m_pGenInputStd;
	CDrawPageMng	*pManage	= pDraw->GetDrawPageMng();
	if(pManage->m_bBusy) return;

	m_wndTreeDrawing.DeleteAllItems();


	//-------- 트리 생성 --------//	
	long nCurPage = pDraw->m_nCurPage;
	long nSttPage = 0;
	long nIdxPage = 0;
	long nPageTotal = pDraw->GetPageDomSize();
	long nCountCate	= pManage->GetPagePartSize();
	CString szTitle, szPage;

	HTREEITEM hItemFirst, hItem;

	for(long i=0; i<nCountCate; i++)				
	{
		if (!pManage->IsPageInit()) pDraw->GetDrawPageMng()->SetPageInit();	
		if(pDoc && pInput->GetDlgOfIDD(IDD_DET_23_SHEARCONNECTORVPOSITION)->IsUsedFlag() && pManage->IsPageInit())
		{	
			if(!CGeneralBaseStd::m_bInclude_Module_TUGirder && szTitle.Find("Turnover")!= -1) continue;

			long nPageSu = pManage->GetPage(i)->m_nPage;
			szTitle = pManage->GetPage(i)->GetPartTitle();
			szTitle.Format("%s(%d)", pManage->GetPage(i)->GetPartTitle(), nPageSu);
			hItem = m_wndTreeDrawing.InsertItem(szTitle, 1, 1);
			m_wndTreeDrawing.SetItemData(hItem, (DWORD)-1);

			if(i==0)
				hItemFirst = hItem;
		
			for(long n=0; n<nPageSu; n++)
			{			
				pDoc->m_pPlateDrawStd->m_nCurPage = ++nIdxPage;

				szPage.Format("%02d : %s", nIdxPage, pManage->GetPage(i)->GetTitle());
				m_wndTreeDrawing.InsertItem(szPage, 2, 2, hItem);	
				m_wndTreeDrawing.SetItemData(hItem, nIdxPage);
				m_wndTreeDrawing.Expand(hItem, TVE_EXPAND);
			}
		}				
		else
		{
			HTREEITEM hSubItem;
 			CString szTitle = pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetPage(i)->GetPartTitle();
 			hSubItem = m_wndTreeDrawing.InsertItem(szTitle, 1, 10);
 			if(!CGeneralBaseStd::m_bInclude_Module_TUGirder && szTitle.Find("Turnover")!= -1)
 				continue;
 			m_wndTreeDrawing.InsertItem(szTitle, 2, 2, hSubItem);	
		}
	}

	//m_wndTreeDrawing.SetItemBold(hItemFirst);
	//m_wndTreeDrawing.Expand(hItemFirst,TVE_EXPAND);
	m_wndTreeDrawing.SetItemHeight(18);
	//m_wndTreeDrawing.SelectSetFirstVisible(hItemFirst);	

	pDraw->m_nCurPage = nCurPage;
	pDraw->SetModifiedAll();
	pDraw->GetDrawPageMng()->SetPageModifyAll();
}

void CShortcutPaneDrawing::OnSelchangedTreeDrawing(NMHDR* pNMHDR, LRESULT* pResult)
{				
	CMainFrame		*pFrm		= (CMainFrame*)AfxGetMainWnd();
	CAPlateDoc		*pDoc		= (CAPlateDoc *)pFrm->GetDocument();
	CAPlateDrawStd	*pDraw	= pDoc->m_pPlateDrawStd;
	CGeneralBaseStd *pInput = pDoc->m_pGenInputStd;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = m_wndTreeDrawing.GetSelectedItem();

	if(m_wndTreeDrawing.GetChildItem(hItem)==NULL && pInput->GetDlgOfIDD(IDD_DET_23_SHEARCONNECTORVPOSITION)->IsUsedFlag())
	{
		CString szItem	 = m_wndTreeDrawing.GetItemText(hItem);
		CString	szNum = _T("");
		long nLeft = szItem.Find(':');
		szNum = szItem.Left(nLeft);
		long nPage = atoi(szNum);

		if(nPage>0)
		{
			pDraw->m_nCurPage = nPage;
			pFrm->SendMessage(WM_COMMAND, ID_PAGE_CHANGE);							
		}
	}
	
	*pResult = 0;
}

void CShortcutPaneDrawing::SetItemPositionUpdate(long nCurPage)
{
	HTREEITEM hParentItem = m_wndTreeDrawing.GetRootItem();
	HTREEITEM hItem = m_wndTreeDrawing.GetChildItem(hParentItem);
	HTREEITEM hPrevItem = NULL;

	CString strPage = _T("");
	long nPage = 0;
	while(hItem)
	{
		if(m_wndTreeDrawing.GetChildItem(hItem)==NULL)
		{
			strPage = m_wndTreeDrawing.GetItemText(hItem);
			nPage = atoi(strPage.Left(3));
			if(nCurPage==nPage)
			{
				m_wndTreeDrawing.Expand(m_wndTreeDrawing.GetParentItem(hItem), TVE_EXPAND);	
				m_wndTreeDrawing.Select(hItem, TVGN_CARET);
				break;
			}
			hPrevItem = hItem;
			hItem = m_wndTreeDrawing.GetNextItem(hItem, TVGN_NEXT);
		}
		else
		{
			hPrevItem = hItem;
			hItem = m_wndTreeDrawing.GetChildItem(hItem);
		}

		if(hItem==NULL)
		{			
			hItem = m_wndTreeDrawing.GetParentItem(hPrevItem);
			hItem = m_wndTreeDrawing.GetNextItem(hItem, TVGN_NEXT);
			if(hItem==NULL)
			{
				hItem = m_wndTreeDrawing.GetNextItem(hParentItem, TVGN_NEXT);
				hParentItem = hItem;
				if(hItem==NULL)
					break;
			}
		}
	}
}

void CShortcutPaneDrawing::OnSize(UINT nType, int cx, int cy) 
{
	CXTPShortcutBarPane::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CXTPClientRect rc(this);
	if(m_wndTreeDrawing.GetSafeHwnd())
	{
		m_wndTreeDrawing.MoveWindow(rc);
	}		
}
