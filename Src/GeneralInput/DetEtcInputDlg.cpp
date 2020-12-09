// DetEtcInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetEtcInputDlg dialog


CDetEtcInputDlg::CDetEtcInputDlg(CWnd* pParent /*=NULL*/)
	: TStackedDialog(IDC_PLACEHOLDER, CDetEtcInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetEtcInputDlg)
	//}}AFX_DATA_INIT
}


void CDetEtcInputDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetEtcInputDlg)
	DDX_Control(pDX, IDC_TREE_ECT_MENU, m_TreeMenu);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER,	m_btnSameAll);
	DDX_Control(pDX, IDC_BUTTON_APPLY_IDENTITY_PART,		m_btnSameEnd);
	DDX_Control(pDX, IDC_BUTTON_APPLY_IDENTITY_ALLJIJUM,	m_btnSameJijum);
	DDX_Control(pDX, IDC_BUTTON_APPLY_COMPLETEINPUT_ALL,	m_btnInputDetail);
}

BEGIN_MESSAGE_MAP(CDetEtcInputDlg, TStackedDialog)
	//{{AFX_MSG_MAP(CDetEtcInputDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ECT_MENU, OnSelchangedTreeMenu)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER, OnButtonApplyIdentityAllgirder)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_IDENTITY_PART, OnButtonApplyIdentityPart)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_IDENTITY_ALLJIJUM, OnButtonApplyIdentityAlljijum)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_COMPLETEINPUT_ALL, OnButtonApplyCompleteinputAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetEtcInputDlg message handlers
void CDetEtcInputDlg::SetDataDefault()
{
	GetPage(IDD_DET_01_SOLEPLATE)->GetPage()->SetDataDefault();				// 교좌받침 상세
	GetPage(IDD_DET_HBRACING_SCALLOP)->GetPage()->SetDataDefault();			// 스켈럽 상세
	GetPage(IDD_DET_04_HSTIFF)->GetPage()->SetDataDefault();				// 수평보강재 부착상세
	GetPage(IDD_DET_05_DANMYUNSHIFTTAPER)->GetPage()->SetDataDefault();		// 단면변화부 테이퍼 상세
	GetPage(IDD_DET_07_STAGINGRING)->GetPage()->SetDataDefault();			// 동바리 고리 상세
	GetPage(IDD_DET_08_SLABANCHOR)->GetPage()->SetDataDefault();			// 슬래브 앵커 상세
	GetPage(IDD_DET_09_BRACKET)->GetPage()->SetDataDefault();				// 브라켓 보강판 상세
	GetPage(IDD_DET_10_BRACKETSTIFF)->GetPage()->SetDataDefault();			// 브라켓 보강판 부착 상세
}

void CDetEtcInputDlg::SetDataInit()
{
	GetPage(IDD_DET_01_SOLEPLATE)->GetPage()->SetDataInit();			// 교좌받침 상세
	GetPage(IDD_DET_HBRACING_SCALLOP)->GetPage()->SetDataInit();		// 스켈럽 상세
	GetPage(IDD_DET_04_HSTIFF)->GetPage()->SetDataInit();				// 수평보강재 부착상세
	GetPage(IDD_DET_05_DANMYUNSHIFTTAPER)->GetPage()->SetDataInit();	// 단면변화부 테이퍼 상세
	GetPage(IDD_DET_07_STAGINGRING)->GetPage()->SetDataInit();			// 동바리 고리 상세
	GetPage(IDD_DET_08_SLABANCHOR)->GetPage()->SetDataInit();			// 슬래브 앵커 상세
	GetPage(IDD_DET_09_BRACKET)->GetPage()->SetDataInit();				// 브라켓 보강판 상세
	GetPage(IDD_DET_10_BRACKETSTIFF)->GetPage()->SetDataInit();			// 브라켓 보강판 부착 상세
}

void CDetEtcInputDlg::SetDataSave()
{
	GetPage(IDD_DET_01_SOLEPLATE)->GetPage()->SetDataSave();			// 교좌받침 상세
	GetPage(IDD_DET_HBRACING_SCALLOP)->GetPage()->SetDataSave();		// 스켈럽 상세
	GetPage(IDD_DET_04_HSTIFF)->GetPage()->SetDataSave();				// 수평보강재 부착상세
	GetPage(IDD_DET_05_DANMYUNSHIFTTAPER)->GetPage()->SetDataSave();	// 단면변화부 테이퍼 상세
	GetPage(IDD_DET_07_STAGINGRING)->GetPage()->SetDataSave();			// 동바리 고리 상세
	GetPage(IDD_DET_08_SLABANCHOR)->GetPage()->SetDataSave();			// 슬래브 앵커 상세
	GetPage(IDD_DET_09_BRACKET)->GetPage()->SetDataSave();				// 브라켓 보강판 상세
	GetPage(IDD_DET_10_BRACKETSTIFF)->GetPage()->SetDataSave();			// 브라켓 보강판 부착 상세	
}

void CDetEtcInputDlg::OnPreInitDialog()
{
	SetResize(IDC_TREE_ECT_MENU,	SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_PLACEHOLDER,		SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_IDENTITY_PART,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_IDENTITY_ALLJIJUM,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_COMPLETEINPUT_ALL,	SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	m_TreeMenu.SetImageList(&m_TreeImageList,TVSIL_NORMAL);

	m_MenuStr.RemoveAll();
	m_nIdArray.RemoveAll();
	
	m_MenuStr.Add("교량받침 상세");					m_nIdArray.Add(IDD_DET_01_SOLEPLATE);
	m_MenuStr.Add("스캘럽 상세");					m_nIdArray.Add(IDD_DET_HBRACING_SCALLOP);	
	m_MenuStr.Add("수평보강재 부착 상세");			m_nIdArray.Add(IDD_DET_04_HSTIFF);	
	m_MenuStr.Add("단면변화부 상세");				m_nIdArray.Add(IDD_DET_05_DANMYUNSHIFTTAPER);	
	m_MenuStr.Add("동바리 고리 상세");				m_nIdArray.Add(IDD_DET_07_STAGINGRING);	
	m_MenuStr.Add("슬래브앵커 및 전단연결재 상세");	m_nIdArray.Add(IDD_DET_08_SLABANCHOR);	
	m_MenuStr.Add("브라켓 상하판 상세");			m_nIdArray.Add(IDD_DET_09_BRACKET);	
	m_MenuStr.Add("브라켓 보강판 부착 상세");		m_nIdArray.Add(IDD_DET_10_BRACKETSTIFF);	

	for(long nIdx=0; nIdx<m_MenuStr.GetSize(); nIdx++)
		m_TreeMenu.InsertItem(m_MenuStr.GetAt(nIdx), nIdx, nIdx);

	SetPageInit(IDD_DET_01_SOLEPLATE);			// 교좌받침 상세
	SetPageInit(IDD_DET_HBRACING_SCALLOP);		// 스켈럽 상세
	SetPageInit(IDD_DET_04_HSTIFF);				// 수평보강재 부착상세
	SetPageInit(IDD_DET_05_DANMYUNSHIFTTAPER);	// 단면변화부 테이퍼 상세
	SetPageInit(IDD_DET_07_STAGINGRING);		// 동바리 고리 상세
	SetPageInit(IDD_DET_08_SLABANCHOR);			// 슬래브 앵커 상세
	SetPageInit(IDD_DET_09_BRACKET);			// 브라켓 보강판 상세
	SetPageInit(IDD_DET_10_BRACKETSTIFF);		// 브라켓 보강판 부착 상세

	SetPage(IDD_DET_01_SOLEPLATE);
	ShowHideItem(IDD_DET_01_SOLEPLATE);
	GetPage(IDD_DET_01_SOLEPLATE)->GetPage()->DrawInputDomyunView();
	m_TreeMenu.EnsureVisible(m_TreeMenu.GetRootItem());

//
//	SetResize(IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
//	SetResize(IDC_BUTTON_APPLY_IDENTITY_PART,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
//	SetResize(IDC_BUTTON_APPLY_IDENTITY_ALLJIJUM,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
//	SetResize(IDC_BUTTON_APPLY_COMPLETEINPUT_ALL,	SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
//
//	SetResize(IDC_PLACEHOLDER,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

}

void CDetEtcInputDlg::SetPageInit(long nID)
{
	AddPage(nID);
//	GetPage(nID)->GetPage()->OnPreInitDialog();
}

void CDetEtcInputDlg::ShowHideItem(long nId)
{
	switch(nId)
	{
	case IDD_DET_01_SOLEPLATE :
		GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_PART)->ShowWindow(TRUE);
		GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_ALLJIJUM)->ShowWindow(TRUE);
		break;
	default :
		GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_ALLGIRDER)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_PART)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_ALLJIJUM)->ShowWindow(FALSE);
	}
}

TStackedPage *CDetEtcInputDlg::CreatePage(UINT nId)
{
	switch (nId)
	{
		case IDD_DET_01_SOLEPLATE:			return new CDetSolePlateDlg(m_pStd, this);
		case IDD_DET_HBRACING_SCALLOP:		return new CDetHBracingScallop(m_pStd, this);
		case IDD_DET_04_HSTIFF:				return new CDetHStiffDlg(m_pStd, this);
		case IDD_DET_05_DANMYUNSHIFTTAPER:	return new CDetDanTaperDlg(m_pStd, this);
		case IDD_DET_07_STAGINGRING:		return new CDetStagingRingDlg(m_pStd, this);
		case IDD_DET_08_SLABANCHOR:			return new CDetSlabAnchor(m_pStd, this);
		case IDD_DET_09_BRACKET:			return new CDetBracketDlg(m_pStd, this);
		case IDD_DET_10_BRACKETSTIFF:		return new CDetBracketStiffDlg(m_pStd, this);
	}

	return NULL;    // this will assert
}

void CDetEtcInputDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();

	pDom->ClearEtt(TRUE);

	GetPage()->GetPage()->DrawInputDomyunView(bZoomAll);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetEtcInputDlg::OnSelchangedTreeMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hitem = m_TreeMenu.GetSelectedItem();
	if(m_TreeMenu.ItemHasChildren(hitem)) return;	
	
	TV_ITEM tv;
	char text[80],ptext[80];
	memset(text,0,80);
	memset(ptext,0,80);
	tv.hItem = hitem;
	tv.mask = TVIF_HANDLE | TVIF_TEXT;
	tv.pszText = text;
	tv.cchTextMax = 30;
	m_TreeMenu.GetItem(&tv);
	
	hitem = m_TreeMenu.GetParentItem(hitem);
	tv.hItem = hitem;
	tv.mask = TVIF_HANDLE | TVIF_TEXT;
	tv.pszText = ptext;
	tv.cchTextMax = 30;
	m_TreeMenu.GetItem(&tv);
	long n = 0;
	for(n=0;n<m_MenuStr.GetSize();n++)
	{
		if(m_MenuStr.GetAt(n) == text)	
		{
			SetPage(m_nIdArray.GetAt(n));
			GetPage(m_nIdArray.GetAt(n))->GetPage()->DrawInputDomyunView();
			SetWindowText(m_MenuStr.GetAt(n));
			break;
		}
	}

	MovePageWindow();
	ShowHideItem(m_nIdArray.GetAt(n));

	*pResult = 0;
}

int CDetEtcInputDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TStackedDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_SHOWSELALWAYS);
	if(!m_TreeImageList.GetSafeHandle())
		m_TreeImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,3,1);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_TREEMENU);
	m_TreeImageList.Add(&bitmap,RGB(192,192,192));
	
	return 0;
}

void CDetEtcInputDlg::OnSize(UINT nType, int cx, int cy) 
{
	TStackedDialog::OnSize(nType, cx, cy);
	MovePageWindow();
}

void CDetEtcInputDlg::MovePageWindow()
{
	CRect rect;
	if(GetDlgItem(IDC_PLACEHOLDER)->GetSafeHwnd())
	{
		GetDlgItem(IDC_PLACEHOLDER)->GetWindowRect(&rect);		
		ScreenToClient(rect);

		TStackedPageHelperList::iterator it;

		for (it = m_PageList.begin(); it != m_PageList.end(); it ++) 					
			(*it).GetPage()->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	}

	RedrawWindow();
}

void CDetEtcInputDlg::OnPageChanged(UINT nId, BOOL bActivated)
{
}

void CDetEtcInputDlg::OnButtonApplyIdentityAllgirder() 
{
	GetPage()->GetPage()->ApplyIdentityAllGirder();
}

void CDetEtcInputDlg::OnButtonApplyIdentityPart() 
{
	GetPage()->GetPage()->ApplyIdentityPart();
}

void CDetEtcInputDlg::OnButtonApplyIdentityAlljijum() 
{
	GetPage()->GetPage()->ApplyIdentityAllJijum();	
}

void CDetEtcInputDlg::OnButtonApplyCompleteinputAll() 
{
	SetDataSave();
	m_pStd->MoveNextDialog();	
}

void CDetEtcInputDlg::OnButtonNext()
{
	long	nDlgID	= GetPage()->GetID();
	long	nSize	= m_nIdArray.GetSize();

	if(nDlgID==m_nIdArray.GetAt(nSize-1))
	{
		m_pStd->MoveNextDialog();
		return;
	}

	for(long n=0; n<m_nIdArray.GetSize(); n++)
	{
		if(m_nIdArray.GetAt(n) == nDlgID)
		{
			SetPage(m_nIdArray.GetAt(n+1));

			HTREEITEM hitem = m_TreeMenu.GetSelectedItem();
			HTREEITEM hitemNext = m_TreeMenu.GetNextItem(hitem, TVGN_NEXT);
			m_TreeMenu.SelectItem(hitemNext);
			m_TreeMenu.EnsureVisible(hitemNext);
			MovePageWindow();
			ShowHideItem(m_nIdArray.GetAt(n+1));
			DrawInputDomyunView();
			return;
		}
	}
	DrawInputDomyunView();	
}

void CDetEtcInputDlg::OnButtonPrev()
{
	long	nDlgID	= GetPage()->GetID();

	if(nDlgID==m_nIdArray.GetAt(0))
	{
		m_pStd->MovePrevDialog();
		return;
	}

	for(long n=0; n<m_nIdArray.GetSize(); n++)
	{
		if(m_nIdArray.GetAt(n) == nDlgID)
		{
			SetPage(m_nIdArray.GetAt(n-1));
			HTREEITEM hitem = m_TreeMenu.GetSelectedItem();
			HTREEITEM hitemPre = m_TreeMenu.GetNextItem(hitem, TVGN_PREVIOUS);
			m_TreeMenu.SelectItem(hitemPre);
			m_TreeMenu.EnsureVisible(hitemPre);

			MovePageWindow();
			ShowHideItem(m_nIdArray.GetAt(n-1));
			DrawInputDomyunView();
			return;
		}
	}
	DrawInputDomyunView();
}

