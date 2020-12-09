// ShortcutPaneInput.cpp : implementation file
//

#include "stdafx.h"
#include "APlate.h"
#include "ShortcutPaneInput.h"

#include "MainFrm.h"
#include "APlateDoc.h"
#include "APlateView.h"
//#include "PPCInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneInput

CShortcutPaneInput::CShortcutPaneInput()
{
}

CShortcutPaneInput::~CShortcutPaneInput()
{
}


BEGIN_MESSAGE_MAP(CShortcutPaneInput, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneInput)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_INPUT, OnSelchangedTreeInput)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneInput message handlers
BOOL CShortcutPaneInput::Create(LPCTSTR lpszCaption, CXTPShortcutBar *pParent)
{
	CXTPShortcutBarPane::Create(lpszCaption, pParent);	

	m_wndTreeInput.Create(WS_VISIBLE|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS, CXTPEmptyRect(), this, IDC_TREE_INPUT);	

	return TRUE;
}

BOOL CShortcutPaneInput::CreateIconByModule()
{
	if(m_ilTreeIcons.GetSafeHandle()) return FALSE;
	
	CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
	CMDIChildWnd	*pChild = pFrm->MDIGetActive();
	if(!pChild)	return FALSE;
	CAPlateDoc		*pDoc	= (CAPlateDoc*)pChild->GetActiveDocument();
	if(!pDoc)	return FALSE;
	CGeneralBaseStd	*pIn	= pDoc->GetPlateInputStd();

	CBitmap bmpHead, bitmap1, bitmap2, bitmap3, bitmap4;
	bmpHead.LoadBitmap(IDB_BITMAP_INPUT_HEAD);

	bitmap1.LoadBitmap(IDR_TOOLBAR_GENINPUT);
	bitmap2.LoadBitmap(IDR_TOOLBAR_DETINPUT);
	bitmap3.LoadBitmap(IDR_TOOLBAR_CALCINPUT);
	if(pIn->IsModuleTUGirder())
		bitmap4.LoadBitmap(IDR_TOOLBAR_DECK_TU);
	else
		bitmap4.LoadBitmap(IDR_TOOLBAR_DECK_GEN);

	m_ilTreeIcons.Create(16, 16, ILC_COLOR32|ILC_MASK, 2, 1);
	m_ilTreeIcons.Add(&bmpHead, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap1, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap2, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap3, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap4, RGB(192, 192, 192));

	m_wndTreeInput.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	COLORREF clrBack = RGB(253, 250, 255);
	m_wndTreeInput.SetBkColor(clrBack);

	CString strMenu, strSubMenu;

	int nImage = 4;
	HTREEITEM hItemFirst, hItem;
 	for(UINT i=0; i<IDD_MENU_SIZE; i++) 
	{
		pIn->m_menuInput.GetMenuString(i, strMenu, MF_BYPOSITION);
		strMenu = strMenu.Mid(0,strMenu.Find("("));
		hItem = m_wndTreeInput.InsertItem(strMenu, i, i);
		if(i==0)
			hItemFirst = hItem;

		CMenu* pMenu = pIn->m_menuInput.GetSubMenu(i);
		for(UINT j=0; j<(UINT)pMenu->GetMenuItemCount(); j++)
		{
			pMenu->GetMenuString(j, strSubMenu, MF_BYPOSITION);
			m_wndTreeInput.InsertItem(strSubMenu, nImage, nImage, hItem);
			m_cmdStrArr.Add(strMenu + " - " + strSubMenu);
			m_cmdIDArr.Add(pMenu->GetMenuItemID(j));
			nImage++;
		}
		m_wndTreeInput.SetItemBold(hItem);
		m_wndTreeInput.Expand(hItem, TVE_EXPAND);
	}
	m_wndTreeInput.SetItemHeight(18);
	m_wndTreeInput.SelectSetFirstVisible(hItemFirst);
 	
	InitTreeState();

	return TRUE;
}

void CShortcutPaneInput::InitTreeState()
{
	if( m_wndTreeInput.GetSafeHwnd() == NULL ) return;

	CMainFrame		*pFrm	= (CMainFrame*)AfxGetMainWnd();
	CMDIChildWnd	*pChild = ((CMDIFrameWnd *)AfxGetMainWnd())->MDIGetActive();
	if(!pChild)	return;
	CAPlateDoc		*pDoc	= (CAPlateDoc*)pChild->GetActiveDocument();
	if(!pDoc)	return;
	CGeneralBaseStd	*pIn	= pDoc->GetPlateInputStd();

	HTREEITEM hParentItem = m_wndTreeInput.GetRootItem();
	HTREEITEM hChildItem = m_wndTreeInput.GetChildItem(hParentItem);
	UINT nItemSize = m_cmdIDArr.GetSize();
	BOOL bEnable = FALSE;

	for(UINT i=0; i<nItemSize; i++)
	{
		if(m_wndTreeInput.GetChildItem(hChildItem)==NULL)
		{
			bEnable = pIn->IsEnableDialog(m_cmdIDArr.GetAt(i));
			
			COLORREF clrText = bEnable ? RGB(0, 0, 0) : RGB(166, 166, 166);
			
			m_wndTreeInput.SetItemColor(hChildItem, clrText);
			if(!bEnable)
			{
				m_wndTreeInput.SetItemState(hChildItem, 0x0001, TVIS_SELECTED);
			}
		}

		hChildItem = m_wndTreeInput.GetNextItem(hChildItem, TVGN_NEXT);
		if(hChildItem==NULL)
		{
			hParentItem = m_wndTreeInput.GetNextItem(hParentItem, TVGN_NEXT);
			if(hParentItem!=NULL)
			{
				hChildItem = m_wndTreeInput.GetChildItem(hParentItem);
			}
		}
	}
}

void CShortcutPaneInput::OnSelchangedTreeInput(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CMainFrame		*pFrm	= (CMainFrame*)AfxGetMainWnd();
	CMDIChildWnd	*pChild = ((CMDIFrameWnd *)AfxGetMainWnd())->MDIGetActive();
	if(!pChild)	return;
	CAPlateDoc		*pDoc	= (CAPlateDoc*)pChild->GetActiveDocument();
	if(!pDoc)	return;
	CGeneralBaseStd	*pIn	= pDoc->GetPlateInputStd();

	HTREEITEM hItem = m_wndTreeInput.GetSelectedItem();
	HTREEITEM hParentItem = m_wndTreeInput.GetParentItem(hItem);
	BOOL bEnable = FALSE;

	if(m_wndTreeInput.GetChildItem(hItem)==NULL)
	{
		CString strItem = m_wndTreeInput.GetItemText(hItem);
		CString strParentItem = m_wndTreeInput.GetItemText(hParentItem);

		int nTreeSize = m_cmdStrArr.GetSize();
		for(int i=0; i<nTreeSize; i++)
		{
			CString s	= m_cmdStrArr.GetAt(i);
			DWORD id	= m_cmdIDArr.GetAt(i);
			if(m_cmdStrArr.GetAt(i)==strParentItem + " - " + strItem)
			{
				if(i==0)
				{
					COLORREF clrText = RGB(0, 0, 0);
				
					m_wndTreeInput.SetItemColor(hItem, clrText);
					/*if(!bEnable)
					{
						m_wndTreeInput.SetItemState(hItem, 0x0001, TVIS_SELECTED);
					}
					*/
				}
				else
				{
					bEnable = pIn->IsEnableDialog(m_cmdIDArr.GetAt(i));
					
					COLORREF clrText = bEnable ? RGB(0, 0, 0) : RGB(166, 166, 166);

					m_wndTreeInput.SetItemColor(hItem, clrText);

					if(!bEnable)
					{
						m_wndTreeInput.SetItemState(hItem, 0x0001, TVIS_SELECTED);
					}
				}
				pFrm->SendMessage(WM_COMMAND, m_cmdIDArr.GetAt(i));
				break;
			}
		}
	}
}

void CShortcutPaneInput::SetItemPosition(UINT nMenuID)
{
	HTREEITEM hParentItem = m_wndTreeInput.GetRootItem();
	HTREEITEM hItem = m_wndTreeInput.GetChildItem(hParentItem);
	CMDIChildWnd	*pChild = ((CMDIFrameWnd *)AfxGetMainWnd())->MDIGetActive();
	if(!pChild)	return;
	CAPlateDoc		*pDoc	= (CAPlateDoc*)pChild->GetActiveDocument();
	CGeneralBaseStd	*pIn	= pDoc->GetPlateInputStd();

	UINT nItemSize = m_cmdIDArr.GetSize();
	for(UINT i=0; i<nItemSize; i++)
	{
		if(m_wndTreeInput.GetChildItem(hItem)==NULL)
		{
			if(m_cmdIDArr.GetAt(i)==nMenuID)
			{
				BOOL bEnable = TRUE;//pIn->IsEnableDialog(m_cmdIDArr.GetAt(i));
				COLORREF clrText = bEnable ?  RGB(0, 0, 0) : RGB(166, 166, 166);
				
				m_wndTreeInput.SetItemColor(hItem, clrText);
			
				m_wndTreeInput.SelectItem(hItem);				
				m_wndTreeInput.EnsureVisible(hItem);
				
			}
		}

		hItem = m_wndTreeInput.GetNextItem(hItem, TVGN_NEXT);
		if(hItem==NULL)
		{
			hParentItem = m_wndTreeInput.GetNextItem(hParentItem, TVGN_NEXT);
			if(hParentItem!=NULL)
			{
				hItem = m_wndTreeInput.GetChildItem(hParentItem);
			}
		}
	}
}

void CShortcutPaneInput::SetItemPositionUpdate(UINT nMenuID, BOOL bSelect)
{
	HTREEITEM hItem = m_wndTreeInput.GetRootItem();
	HTREEITEM hParentItem = hItem;
	hItem = m_wndTreeInput.GetChildItem(hItem);

	CRect rect;
	long nCount = 0;
	while(hItem)
	{
		if(m_wndTreeInput.GetChildItem(hItem)==NULL)
		{
			if(m_cmdIDArr.GetAt(nCount)==nMenuID-1)
			{
				if(bSelect) 
					m_wndTreeInput.Select(hItem, TVGN_CARET);
				else
				{
					m_wndTreeInput.GetItemRect(hItem, &rect, TRUE);
					m_wndTreeInput.InvalidateRect(rect);
				}
				break;
			}
			++nCount;
		}
		hItem = m_wndTreeInput.GetNextItem(hItem, TVGN_NEXT);
		if(hItem==NULL)
		{
			hParentItem = m_wndTreeInput.GetNextItem(hParentItem, TVGN_NEXT);
			if(hParentItem)
				hItem = m_wndTreeInput.GetChildItem(hParentItem);
		}
	}
}

void CShortcutPaneInput::OnSize(UINT nType, int cx, int cy) 
{
	CXTPShortcutBarPane::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CXTPClientRect rc(this);
	if(m_wndTreeInput.GetSafeHwnd())
	{
		m_wndTreeInput.MoveWindow(rc);
	}	
}
