// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "APlate.h"
#include "APlateDoc.h"
#include "APlateView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "Splash.h"
//#include "PrgOptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_APLATE_FILE_NEW, OnFileNew)
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_CREATE()	
	ON_COMMAND(ID_VIEW_CUSTOM,	OnViewCustom)
	ON_COMMAND(ID_OPT_PRG,		OnOptPrg)
	ON_WM_DESTROY()
	ON_COMMAND(ID_STRUCTURE_CALC,	OnStructureCalc)
	ON_COMMAND(ID_SHOPOUT_PAINT,	OnShopoutPaint)
	ON_COMMAND(ID_SHOPOUT_BILL,		OnShopoutBill)
	ON_COMMAND(ID_SHOPOUT_WELD,		OnShopoutWeld)
	ON_UPDATE_COMMAND_UI(ID_SHOPOUT_WELD,	OnUpdateShopoutWeld)
	ON_UPDATE_COMMAND_UI(ID_SHOPOUT_BILL,	OnUpdateShopoutBill)
	ON_UPDATE_COMMAND_UI(ID_SHOPOUT_PAINT,	OnUpdateShopoutPaint)
	ON_UPDATE_COMMAND_UI(ID_STRUCTURE_CALC, OnUpdateStructureCalc)
	ON_COMMAND(ID_PAGE_REPOS,		OnPageRepos)
	ON_COMMAND(ID_REALUPDATE,		OnRealupdate)
	ON_COMMAND(ID_HELP_WEB_FAQ,		OnHelpWebFaq)
	ON_COMMAND(ID_HELP_WEB_HOME,	OnHelpWebHome)
	ON_COMMAND(ID_OUT_BM,			OnOutBm)
	ON_UPDATE_COMMAND_UI(ID_OUT_BM, OnUpdateOutBm)
	ON_COMMAND(ID_OUT_NONDES, OnOutNondes)
	ON_UPDATE_COMMAND_UI(ID_OUT_NONDES, OnUpdateOutNondes)
	ON_COMMAND(ID_HELP_GEN_DETAIL, OnHelpGenDetail)
	ON_COMMAND(ID_HELP_CALC, OnHelpCalc)
	ON_COMMAND(ID_HELP_REBAR, OnHelpRebar)
	ON_COMMAND(ID_HELP_OUT, OnHelpOut)
	ON_COMMAND(ID_HELP_ALINE, OnHelpAline)
	ON_COMMAND(ID_HELP_ABOARDER, OnHelpAboarder)
 	ON_COMMAND(ID_F13T_BROCHURE, OnShowF13TBrochure)
	ON_WM_TIMER()
	ON_COMMAND(ID_UPDATE_LIST,		OnUpdateList)
	ON_COMMAND(ID_OUT_WELD_CHANGE, OnOutWeldChange)
	ON_UPDATE_COMMAND_UI(ID_OUT_WELD_CHANGE, OnUpdateOutWeldChange)
	ON_COMMAND(ID_VIEW_CONTROLPANE, OnViewControlpane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTROLPANE, OnUpdateViewControlpane)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_VIEW_INPUTPANE, OnViewInputpane)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INPUTPANE, OnUpdateViewInputpane)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_REGMSG_SHOW_UPDATEDLG, OnShowUpdateDlg)
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_COMMAND_RANGE(ID_GNINPUT_0, ID_DECK_15, OnInput)
	ON_UPDATE_COMMAND_UI_RANGE(ID_GNINPUT_0, ID_DECK_15, OnUpdateInput)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
 	ON_MESSAGE(XTPWM_SHORTCUTBAR_NOTIFY, OnShortcutBarNotify)
	ON_MESSAGE(WM_CREATE_INPUT, OnCreateInputPane)
	ON_MESSAGE(WM_UPDATE_INPUT_TITLE, OnUpdateInputTitle)
	ON_MESSAGE(WM_UPDATE_TREE, OnUpdateTree)
	ON_COMMAND(ID_HELP_FINDER, CXTPMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CXTPMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CXTPMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CXTPMDIFrameWnd::OnHelpFinder)	
	ON_MESSAGE(WM_PAGE_UPDATE, OnEditPageUpdate)
	ON_MESSAGE(WM_UPDATE_TOOLBAR_DRAW, OnUpdateToolbarDraw)
	ON_CBN_SELENDOK(ID_COMBO_GOINDEX, OnSelectPageCombo)
	ON_XTP_CREATECONTROL()
	ON_MESSAGE(WM_PAGE_RESET, OnPageReset)	
	ON_MESSAGE(WM_PAGEINFO, OnPageInfo)
	ON_MESSAGE(WM_CHANGE_MENUDLG,OnChangeMenuDlg)
	ON_MESSAGE(WM_GET_CHILDBAR,OnGetChildBar)	
	ON_MESSAGE(WM_SAVE_JPG, SaveImageOnGLView)
	ON_MESSAGE(WM_USER_SECTION, OnSectionCheck)
	ON_MESSAGE(WM_MODIFY_INPUTDATA, OnModifyInputData)
	ON_MESSAGE(WM_USER_APPLYDOMYUN, OnApplyDomyun)
	ON_COMMAND(ID_CREATE_LICENSE , &CMainFrame:: OnCreateKey)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PROGRESS,
	ID_INDICATOR_PAGENUM,
	ID_INDICATOR_EXT,
	ID_INDICATOR_RIGHTANG,
	ID_INDICATOR_VERSIONINFO,	
};

static UINT shortcuts[] =
{
	ID_SHORTCUT_START,
	ID_SHORTCUT_INPUT,
	ID_SHORTCUT_DRAWING,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_bFullScreenMode(FALSE)
{
	m_bMessageDraw	= FALSE;
	m_bPrintPreview	= FALSE;	// 미리 보기 중인지?		
}

CMainFrame::~CMainFrame()
{
	m_bPrintPreview	= FALSE;	// 미리 보기 중인지?
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set Office 2003 Theme
	CXTPPaintManager::SetTheme(xtpThemeOffice2007);
	
	//----------------------------------------------------------------------------------------------
	//  SKIN
	//----------------------------------------------------------------------------------------------
	XTPSkinManager()->SetAutoApplyNewThreads(FALSE);
	XTPSkinManager()->GetResourceFile()->SetModuleHandle(AfxGetInstanceHandle());
	XTPSkinManager()->SetApplyOptions(/*XTPSkinManager()->GetApplyOptions() | xtpSkinApplyFrame | xtpSkinApplyMetrics | */xtpSkinApplyColors);
	XTPSkinManager()->LoadSkin(NULL, _T("NORMALBLUE.INI"));	
	
	
	if(!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태바 생성 실패!\n");
		return -1;
	}
	
	int cxWidth = 24 + 150;
	m_wndStatusBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_NOBORDERS | SBPS_STRETCH,500);
	m_wndStatusBar.SetPaneInfo(PROGRESS_TEXT,ID_INDICATOR_PROGRESS,0,100);
	m_wndStatusBar.SetPaneInfo(PAGE_TEXT,ID_INDICATOR_PAGENUM,0,120);	
	m_wndStatusBar.SetPaneInfo(MOUSE_AXIAL_TEXT,ID_INDICATOR_EXT,0,cxWidth);
	m_wndStatusBar.SetPaneInfo(MOVE_TEXT,ID_INDICATOR_PAGENUM,0,30);

//	CString szCmpDate = AfxGetApp()->GetProfileString("Settings", "BuildVersion","");	
//	//m_wndStatusBar.SetPaneText(VERSION_TEXT,"Build Date : " + szCmpDate,TRUE);
//	m_wndStatusBar.SetPaneText(VERSION_TEXT,"Build Date : " + szCmpDate);

	CAPlateApp *pApp = (CAPlateApp*)AfxGetApp();
	if(pApp)
	{
		CString sVersion = _T("");
		sVersion.Format("버전%s", pApp->GetStringVersion()); 
		m_wndStatusBar.SetPaneText(VERSION_TEXT, sVersion);
	}

	
	// Initialize the command bars
	if(!InitCommandBars())
	{
		TRACE0("커맨드바 초기화 실패!\n");
		return -1;
	}
	
	// Get a pointer to the command bars object.
	m_pCommandBars = GetCommandBars();
	if(m_pCommandBars==NULL)
	{
		TRACE0("커맨드바 객체 생성 실패!\n");
		return -1;
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = m_pCommandBars->SetMenu(_T("메뉴 모음"), IDR_MAINFRAME); 
	if(pMenuBar==NULL)
	{
		TRACE0("메뉴바 생성 실패!\n");
		return -1;
	}
	XTPImageManager()->SetMaskColor(RGB(192, 192, 192));
	
	CXTPToolBar* pToolBarFile = (CXTPToolBar*)m_pCommandBars->Add(_T("기본 도구 상자"), xtpBarTop);
	if(!pToolBarFile || !pToolBarFile->LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("파일 도구바 생성 실패!\n");
		return -1;
	}

	/************************************************************************/
	/* 정렬														*/
	/************************************************************************/
	CXTPToolBar* pToolBarAlign = (CXTPToolBar*)m_pCommandBars->Add(_T("정렬 도구 상자"), xtpBarTop);
	if(!pToolBarAlign || !pToolBarAlign->LoadToolBar(IDR_TOOLBAR_ALIGN))
	{
		TRACE0("확대/축소 설정 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarAlign, pToolBarFile);


	/************************************************************************/
	/* 도면출력																*/
	/************************************************************************/
	CXTPToolBar* pToolBarDomyun = (CXTPToolBar*)m_pCommandBars->Add(_T("도면출력 도구 상자1"), xtpBarTop);
	if(!pToolBarDomyun || !pToolBarDomyun->LoadToolBar(IDR_TOOLBAR_DOMYUN))
	{
		TRACE0("도면 설정1 도구바 생성 실패!\n");
		return -1;
	}
	
	CXTPToolBar* pToolBarProp = (CXTPToolBar*)m_pCommandBars->Add(_T("도면출력 도구 상자2"), xtpBarTop);
	if(!pToolBarProp || !pToolBarProp->LoadToolBar(IDR_TOOLBAR_PROP))
	{
		TRACE0("도면 설정2 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarProp, pToolBarDomyun);
	
	/************************************************************************/
	/* Zoom																	*/
	/************************************************************************/
	CXTPToolBar* pToolBarZoom = (CXTPToolBar*)m_pCommandBars->Add(_T("확대/축소 도구 상자"), xtpBarTop);
	if(!pToolBarZoom || !pToolBarZoom->LoadToolBar(IDR_TOOLBAR_ZOOM))
	{
		TRACE0("확대/축소 설정 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarZoom, pToolBarProp);
	
	/************************************************************************/
	/* OSNAP																*/
	/************************************************************************/
	CXTPToolBar* pToolBarOSnap = (CXTPToolBar*)m_pCommandBars->Add(_T("OSNAP 설정 도구 상자"), xtpBarTop);
	if(!pToolBarOSnap || !pToolBarOSnap->LoadToolBar(IDR_TOOLBAR_OSNAP))
	{
		TRACE0("OSNAP 설정 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarOSnap, pToolBarZoom);
	
	/************************************************************************/
	/* 일반입력																*/
	/************************************************************************/
	CXTPToolBar* pToolBarGen = (CXTPToolBar*)m_pCommandBars->Add(_T("일반입력 도구 상자"), xtpBarTop);
	if(!pToolBarGen || !pToolBarGen->LoadToolBar(IDR_TOOLBAR_GENINPUT))
	{
		TRACE0("일반입력 도구바 생성 실패!\n");
		return -1;
	}
//	DockRightOf(pToolBarGen, pToolBarFile);

	/************************************************************************/
	/* 상세입력																*/
	/************************************************************************/
	CXTPToolBar* pToolBarDetail = (CXTPToolBar*)m_pCommandBars->Add(_T("상세입력 도구 상자"), xtpBarTop);
	if(!pToolBarDetail || !pToolBarDetail->LoadToolBar(IDR_TOOLBAR_DETINPUT))
	{
		TRACE0("상세입력 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarDetail, pToolBarGen);
	
	/************************************************************************/
	/* 계산입력																*/
	/************************************************************************/
	CXTPToolBar* pToolBarCalc = (CXTPToolBar*)m_pCommandBars->Add(_T("계산입력 도구 상자"), xtpBarTop);
	if(!pToolBarCalc || !pToolBarCalc->LoadToolBar(IDR_TOOLBAR_CALCINPUT))
	{
		TRACE0("계산입력 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarCalc, pToolBarDetail);
	
	/************************************************************************/
	/* 배근입력																*/
	/************************************************************************/
	CXTPToolBar* pToolBarDeck = (CXTPToolBar*)m_pCommandBars->Add(_T("배근입력 도구 상자"), xtpBarTop);
	if(!pToolBarDeck || !pToolBarDeck->LoadToolBar(CGeneralBaseStd::IsModuleTUGirder() ? IDR_TOOLBAR_DECK_TU : IDR_TOOLBAR_DECK_GEN))
	{
		TRACE0("배근입력 도구바 생성 실패!\n");
		return -1;
	}
	DockRightOf(pToolBarDeck, pToolBarCalc);

	/************************************************************************/
	/*선형정보																*/
	/************************************************************************/
	CXTPToolBar* pToolBarLine = (CXTPToolBar*)m_pCommandBars->Add(_T("선형"), xtpBarTop);
	if(!pToolBarLine || !pToolBarLine->LoadToolBar(IDR_TOOLBAR_LINE))
	{
		TRACE0("선형 도구바 생성 실패!\n");
		return -1;
	}

//////////////////////////////////////////////////////////////////////////
	XTP_COMMANDBARS_ICONSINFO* pIconsInfo = XTPPaintManager()->GetIconsInfo();
	pIconsInfo->bUseDisabledIcons = TRUE;
	pIconsInfo->bIconsWithShadow = TRUE;

	LoadCommandBars(_T("CommandBars"));	
	
	////////////////////////////////////////////////////////////////////////////
	// Frame Setting
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeOutlook2007);	
	m_paneManager.CreatePane(IDR_PANE_INPUT, CRect(0, 0, 250, 250), xtpPaneDockBottom);	
	m_paneManager.CreatePane(IDR_PANE_CONTROL, CRect(0, 0, 227, 250), xtpPaneDockLeft);	
	m_paneManager.ClosePane(IDR_PANE_INPUT);

	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	WNDCLASS wndcls;	
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!::GetClassInfo(hInst, szAPlateClass, &wndcls))
	{
		// get default stuff
		::GetClassInfo(hInst, cs.lpszClass, &wndcls);
		wndcls.style &= ~(CS_HREDRAW|CS_VREDRAW);
		// register a new class
		wndcls.lpszClassName = szAPlateClass;
		wndcls.hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
		ASSERT(wndcls.hIcon != NULL);
		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
	cs.lpszClass = szAPlateClass;

	cs.x = AfxGetApp()->GetProfileInt("Pos","Left",cs.x);
	cs.y = AfxGetApp()->GetProfileInt("Pos","Top",cs.y);
	cs.cx = AfxGetApp()->GetProfileInt("Pos","Right",cs.cx);
	cs.cy = AfxGetApp()->GetProfileInt("Pos","Bottom",cs.cy);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

CAPlateDoc* CMainFrame::GetDocument()
{
	CMDIChildWnd	*pChild = (CMDIChildWnd*)MDIGetActive();
	if(pChild == NULL) return NULL;
	return ((CAPlateDoc*)pChild->GetActiveDocument());	
}

CAPlateView* CMainFrame::GetPlateView()
{
	CMDIChildWnd	*pChild = (CMDIChildWnd*)MDIGetActive();
	if(pChild == NULL) return NULL;
	return (CAPlateView*)pChild->GetActiveView();
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnViewCustom() 
{

}

void CMainFrame::OnUpdateInput(CCmdUI* pCmdUI) 
{
	if(!aIsLockOK() && !LDK::aIsLogIn()) // 2015. 07. 28. Software Lock 작업, 김형준
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	CAPlateDoc	*pDoc	= GetDocument();
	if(!pDoc)	return;
	CGeneralBaseStd* pIn = pDoc->GetPlateInputStd();
	
	int nCurID = pIn->GetCurrentMenuID();
	for(int i=ID_GNINPUT_0; i<=ID_DECK_15; i++)
		pCmdUI->SetCheck(pCmdUI->m_nID==nCurID);
	
	if(ID_GNINPUT_0==pCmdUI->m_nID)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	
	CAPlateView *pView	= GetPlateView();
	if(!pView->IsKindOf(RUNTIME_CLASS(CAPlateView)))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	if(pDoc)
	{
		BOOL bEnable = pIn->IsEnableDialog(pCmdUI->m_nID);
		pCmdUI->Enable(bEnable);
	}
}

void CMainFrame::OnSelectPageCombo()
{
	CMDIChildWnd* pChild=MDIGetActive();
	if( pChild == NULL ) return;
	CAPlateDoc* pDoc = (CAPlateDoc*)pChild->GetActiveDocument();
	
	CXTPControlComboBox* pSrcCombo = NULL;
	
	CObList listButtons;
// 	if (CShToolBar::GetCommandButtons (ID_COMBO_GOINDEX, listButtons) > 0)
// 	{
// 		for (POSITION posCombo = listButtons.GetHeadPosition (); 
// 		pSrcCombo == NULL && posCombo != NULL;)
// 		{
// 			CXTPControlComboBox* pCombo = 
// 				DYNAMIC_DOWNCAST (CXTPControlComboBox, listButtons.GetNext (posCombo));
// 			ASSERT (pCombo != NULL);
// 			
// 			if (CShToolBar::IsLastCommandFromButton (pCombo))
// 			{
// 				pSrcCombo = pCombo;
// 			}
// 		}
// 	}
	
	if (pSrcCombo != NULL)
	{
		int SelNum = pSrcCombo->GetCurSel();
		if(SelNum!=-1)
		{
			pDoc->m_pPlateDrawStd->m_nCurPage = pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetPageByIdx(SelNum);								
			OnEditPageUpdate(0,0);			
			PostMessage(WM_COMMAND,ID_PAGE_CHANGE);			
		}
		
	}
	else
	{
		//MessageBox ("Enter name here....");
	}
}

void CMainFrame::OnComboPageUpdate(long nPage) 
{	
	CMDIChildWnd* pChild=MDIGetActive();
	if( pChild == NULL ) return;
	CAPlateDoc* pDoc = (CAPlateDoc*)pChild->GetActiveDocument();
	CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;	
	CXTPControlComboBox* pSrcCombo = NULL;
	
	CObList listButtons;

// 	if (CShToolBar::GetCommandButtons (ID_COMBO_GOINDEX, listButtons) > 0)
// 	{
// 		for (POSITION posCombo = listButtons.GetHeadPosition (); 
// 		pSrcCombo == NULL && posCombo != NULL;)
// 		{
// 			CXTPControlComboBox* pCombo = 
// 				DYNAMIC_DOWNCAST (CXTPControlComboBox, listButtons.GetNext (posCombo));
// 			ASSERT (pCombo != NULL);
// 			
// 			//if (CShToolBar::IsLastCommandFromButton (pCombo))
// 			//{
// 			pSrcCombo = pCombo;
// 			//}
// 		}
// 	}
	
	if (pSrcCombo != NULL)
	{
		if(nPage==0) nPage=1;
		long nSel = pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetIdxOfPage(nPage);
		if(nSel!=-1)				
			pSrcCombo->SetCurSel(nSel);		
	}	
}

LRESULT CMainFrame::OnEditPageUpdate(WPARAM, LPARAM) 
{

 	CMDIChildWnd *pChild=MDIGetActive();
// 	if( pChild == NULL ) return;
// 	CAPlateDoc* pDoc = (CAPlateDoc*)pChild->GetActiveDocument();
// 	CAPlateView* pView = (CAPlateView*)pChild->GetActiveView();
// 		
// 	CShToolbarEditBoxButton* pSrcEdit = NULL;
// 
// 	CObList listButtons;
// 	if (CShToolBar::GetCommandButtons (ID_EDIT_PAGE, listButtons) > 0)
// 	{
// 		for (POSITION pos = listButtons.GetHeadPosition (); 
// 		pSrcEdit == NULL && pos != NULL;)
// 		{
// 			CShToolbarEditBoxButton* pEdit = 
// 				DYNAMIC_DOWNCAST (CShToolbarEditBoxButton, listButtons.GetNext (pos));
// 			
// 			pSrcEdit = pEdit;			
// 		}
// 	}
// 	
// 	if (pSrcEdit != NULL && pSrcEdit->GetEditBox () != NULL &&
// 		(pSrcEdit->GetEditBox ()->GetStyle () & WS_VISIBLE))
// 	{		
// 		CString strName;		
// 		long nPage = pDoc->m_pPlateDrawStd->GetCurPage();
// 		if(nPage==0) nPage=1;
// 
// 		strName.Format("%d",nPage);
// 		pSrcEdit->GetEditBox ()->SetWindowText (strName);			
// 		OnComboPageUpdate(nPage);
// 
// 		pDoc->SetModifiedFlag(TRUE);
// 	}	
// 	pDoc->m_pGenInputStd->SetSaveMenuIDD(ID_INPUT_END);
	return 0;
}

void CMainFrame::AddHisData(CString szTextIn)
{
/*	CHisEdit *pEdit = &m_wndCommandBar.m_HisEdit;
	int iTotalTextLength = pEdit->GetWindowTextLength();
	int TotalLine = pEdit->GetLineCount();
	pEdit->SetSel(iTotalTextLength, iTotalTextLength);
	pEdit->ReplaceSel(szTextIn);
	int iStartPos = iTotalTextLength;
		
	int ScrollLine = pEdit->GetLineCount() - TotalLine;
	
	pEdit->LineScroll(ScrollLine);	*/
}

void CMainFrame::OnOptPrg() 
{
	CString szTitle = "프로그램 환경설정";
	CEnvRegist env;		
	BOOL bInitStart = env.LoadEnvInt(CEnvRegist::SET_PRGOPT,"0");
	if(!bInitStart) 
	{
		((CAPlateApp *)AfxGetApp())->SetFixDirectory();	
		szTitle = "프로그램 환경설정 - [프로그램 최초설정]";
	}
	
	CPrgOptSheet OptDlg(szTitle,this);
	OptDlg.DoModal();
	
	CMDIChildWnd* pChild=MDIGetActive();
	if( pChild == NULL ) return;	
	CAPlateView* pView = (CAPlateView*)pChild->GetActiveView();	
	
	CEnvRegist reg;
	if(reg.LoadEnvInt(CEnvRegist::USE_SHXFONT)) 
		pView->SetUseShx(TRUE);
	else
		pView->SetUseShx(FALSE);
}

LRESULT CMainFrame::OnPageReset(WPARAM nPart, LPARAM)
{
	CMDIChildWnd* pChild=MDIGetActive();
	if( pChild == NULL ) return 0;
	CAPlateDoc* pDoc = (CAPlateDoc*)pChild->GetActiveDocument();
	CAPlateView* pView = (CAPlateView*)pChild->GetActiveView();
	CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;	
	
	pStd->GetDrawPageMng()->SetModified(nPart);

	if(!pStd->GetDrawPageMng()->IsPageInit()) pStd->GetDrawPageMng()->SetPageInit();
	
	m_paneShortcutDrawing.InitTree();
	
	return 0;
}

void CMainFrame::OnDestroy() 
{
	CXTPMDIFrameWnd::OnDestroy();
	
	if(!IsIconic())
	{	
		CRect r;
		GetWindowRect(r);
		CWinApp* pApp = AfxGetApp();
		pApp->WriteProfileInt("Pos","Left", r.left);
		pApp->WriteProfileInt("Pos","Top",  r.top);
		pApp->WriteProfileInt("Pos","Right",r.right);
		pApp->WriteProfileInt("Pos","Bottom",r.bottom);
		
		int bMax = IsZoomed();
		pApp->WriteProfileInt("Size","Zoomed",bMax);
	}	
}

void CMainFrame::ShowPageInfo()
{	
	CAPlateDoc* pDoc = GetDocument();	
	if( pDoc == NULL ) return;
	
	if(pDoc->m_bBrpFileOpen == 0) return;	
	CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;	
	
	if(!m_PageInfoDlg.Create(this)) return;	
	if(!pStd->GetDrawPageMng()->IsPageInit()) pStd->GetDrawPageMng()->SetPageInit();
	for(long i=0; i<pStd->GetDrawPageMng()->GetPagePartSize();i++)
	{				
		m_PageInfoDlg.AddPageInfo(pStd->GetDrawPageMng()->GetPage(i)->GetPartTitle(),pStd->GetDrawPageMng()->GetPageByIdx(i),pStd->GetDrawPageMng()->GetPage(i)->m_nPage);
	}		
	m_PageInfoDlg.SetListData();
	m_PageInfoDlg.Show();
}

LRESULT CMainFrame::OnPageInfo(WPARAM, LPARAM)
{
	ShowPageInfo();
	return 0;
}

// 구조계산서 출력
void CMainFrame::OnStructureCalc() 
{
	CMDIChildWnd		*pChild	= MDIGetActive();
	CAPlateDoc			*pDoc	= GetDocument();
	CAPlateApp			*pApp	= (CAPlateApp*)AfxGetApp();
	CAPlateOutCalcStd	*pStd	= pDoc->GetAPlateOutCalcStd();
	
	if(pStd)
	{
		CAPlateOutputDlg	*pDlg	= pDoc->GetAPlateOutCalcStd()->GetAPlateOutputDlg();
		pDlg->SetWnd(this);
		pDlg->DoModal();
	}
}
// 구조계산서 출력
void CMainFrame::OnUpdateStructureCalc(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	
	if(pDoc && pDoc->m_pGenInputStd->IsEnableDialog(ID_CLINPUT_24))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnShopoutPaint() 
{
	CAPlateDoc* pDoc = GetDocument();

	pDoc->m_pGenInputStd->GetPaintOutSheet()->Create(this,
		PAINTMAKE_SANG|PAINTMAKE_BUJE_SANG|PAINTMAKE_BUJE_TOTAL|
		PAINTMAKE_MAT_THICK|PAINTMAKE_BOLT_SANG|
		PAINTMAKE_BOLT_TOTAL|PAINTMAKE_SUMMARY , 0, DIM_BASIC);
}

void CMainFrame::OnUpdateShopoutPaint(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	
	if(pDoc && pDoc->m_pGenInputStd->IsEnableDialog(ID_DETINPUT_9))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnShopoutBill() 
{
	CAPlateDoc* pDoc = GetDocument();

	pDoc->m_pGenInputStd->GetBMOutSheet()->Create(
		this,
		MAKE_BM_SANG|MAKE_BM_STDANDARD_SANG|MAKE_BM_THICK|MAKE_BM_CAR|MAKE_BM_CARTOTAL|
		MAKE_BM_SECTIONTYPE|MAKE_BM_BUJE|MAKE_BM_BUJE_TOTAL|MAKE_BM_GIRDER_TOTAL|MAKE_BM_DEDUCT_SANG|
		MAKE_BM_DEDUCT_BUJE|MAKE_BM_BUJEKIND_TOTAL, 0, DIM_BASIC);
}

void CMainFrame::OnUpdateShopoutBill(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	
	if(pDoc && pDoc->m_pGenInputStd->IsEnableDialog(ID_DETINPUT_9))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnShopoutWeld() 
{
	CAPlateDoc* pDoc = GetDocument();
	pDoc->m_pGenInputStd->GetWeldOutSheet()->Create(this,WELDMAKE_SANG|WELDMAKE_WELDTYPE|WELDMAKE_WELDTHICK,DIM_BASIC,0);
}

void CMainFrame::OnUpdateShopoutWeld(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	
	if(pDoc && pDoc->m_pGenInputStd->IsEnableDialog(ID_DETINPUT_9))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

LRESULT CMainFrame::OnGetChildBar(WPARAM, LPARAM)
{
	return 0;
}

LRESULT CMainFrame::OnChangeMenuDlg(WPARAM id, LPARAM)
{
	CAPlateApp* pApp = (CAPlateApp*)AfxGetApp();
	CMDIChildWnd *pMDIChildWnd = NULL;
	CMDIChildWnd *pChild=MDIGetActive();
	CView *pView = pChild->GetActiveView();
	CAPlateDoc   *pDoc = GetDocument();	
	pDoc->m_pGenInputStd->HideCurrentDialog();	
	OnInput(id);
	return 0;
}

LRESULT CMainFrame::ResetDomCombo()
{
/*	CObList listButtons;
	m_wndPageViewBar.m_wndTab.DeleteAllItems();
	if (CShToolBar::GetCommandButtons (ID_COMBO_GOINDEX, listButtons) > 0)
	{
		for (POSITION posCombo = listButtons.GetHeadPosition (); 
		posCombo != NULL;)
		{
			CShToolbarComboBoxButton* pCombo = 
				DYNAMIC_DOWNCAST (CShToolbarComboBoxButton, listButtons.GetNext (posCombo));			
			
			if (pCombo != NULL)
			{
				CMDIChildWnd* pChild=MDIGetActive();
				if( pChild != NULL )
				{
					CAPlateDoc   *pDoc = GetDocument();	
					pCombo->RemoveAllItems();
					for(long i=0;i<pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetPagePartSize();i++)						
						pCombo->AddItem(pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetPage(i)->GetPartTitle());								
					pCombo->SelectItem(0);		
				}
			}	
		}
	}*/
	return 0;
}

void CMainFrame::OnPageRepos() 
{
	CMDIChildWnd	*pChild	= MDIGetActive();
	if( pChild == NULL ) return;
	CAPlateDoc		*pDoc	= GetDocument();
	CAPlateDrawStd	*pStd	= pDoc->m_pPlateDrawStd;
	CAPlateView		*pView	= (CAPlateView*)pChild->GetActiveView();
	
	pView->SetDomyun(pView->GetBasicDomyun());
	pView->RedrawAll();
	
	if(pStd->GetDrawPageMng()->ResetPageSort()==IDOK)
	{
		ResetDomCombo();
 		m_paneShortcutDrawing.InitTree();
	}
}

void CMainFrame::OnUpdateList() 
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\Upgrade.txt";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);
}

void CMainFrame::OnRealupdate() 
{
	CString szPath = aGetAUpdateProgramInstallPath();
	szPath += "\\AUpdate.exe";
	CFileFind ff;
	if(!ff.FindFile(szPath))
	{		
		if(AfxMessageBox("AUpdate 모듈이 존재하지 않습니다.\n웹에서 다운로드 후 설치하시겠습니까 ?", MB_YESNO | MB_ICONINFORMATION)==IDYES)
		{
			ShellExecute(NULL,NULL, "http://www.aroad.co.kr/22-download/RealUpdate_AUpdate/AUpdate_Setup.exe" ,NULL,NULL,SW_SHOW);
		}
	}
	else
		ShellExecute(NULL,NULL, szPath ,NULL,NULL,SW_SHOW);
	ff.Close();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && IsCTRLpressed() && (pMsg->wParam=='G' || pMsg->wParam=='g'))
	{
		CAPlateDoc   *pDoc  = GetDocument();	
		if(pDoc && pDoc->m_pGenInputStd->GetCurruntDlg())
		{	
			CButton *pButton = (CButton *)pDoc->m_pGenInputStd->GetCurruntDlg()->GetDlgItem(IDC_CHECK_TOTALVIEW);
			if(pButton)
			{
				pButton->SetCheck(pButton->GetCheck() ? 0 : 1);
				pDoc->m_pGenInputStd->GetCurruntDlg()->SendMessage(WM_COMMAND,MAKEWPARAM(IDC_CHECK_TOTALVIEW, BN_CLICKED), (LPARAM)pDoc->m_pGenInputStd->GetCurruntDlg()->m_hWnd);
			}
		}
	}		
	
	return CXTPMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnHelpWebFaq() 
{
	ShellExecute(NULL,NULL, "http://www.aroad.co.kr/customer/qna.asp?combo1=1_2"  ,NULL,NULL,SW_SHOW);	    
}

void CMainFrame::OnHelpWebHome() 
{
	ShellExecute(NULL,NULL, "http://www.aroad.co.kr"  ,NULL,NULL,SW_SHOW);
}

LRESULT CMainFrame::SaveImageOnGLView(WPARAM, LPARAM)
{
	CMDIChildWnd *pChild=MDIGetActive();
	CAPlateDoc   *pDoc  = GetDocument();	
	CAPlateView  *pView = (CAPlateView*)pChild->GetActiveView();
	if(!pView) return 0;
	
	CDC *pDC;				
	CDC *pBitDC = new CDC;
	CRect rect;
	CBitmap bmp, *pOldBitmap;	
	CImage Image;
	
	CFileDialog dlg(FALSE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"JPG 파일 (*.jpg)|*.jpg|BMP 파일 (*.bmp)|*.bmp||");
	dlg.m_ofn.lpstrTitle = "파일 저장";
	if (dlg.DoModal() != IDOK) 
	{
		delete pBitDC;
		return 0;
	}
	
	pDC = pView->GetDC();	
	CString szFileName = dlg.GetPathName();
	pView->GetClientRect(&rect);
	pBitDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);				
	pOldBitmap = pBitDC->SelectObject(&bmp);	
	pView->OnDraw(pDC);	// 이미지로 저장할 화면	
	pBitDC->BitBlt(0,0,rect.right,rect.bottom,pDC,0,0,SRCCOPY);	
	if(!Image.CopyFromBmp(&bmp))
	{
		AfxMessageBox("지원할수 없는 형식입니다 !");
		delete pBitDC;
		return 0;
	}	
	Image.SaveImage(szFileName.GetBuffer(szFileName.GetLength()));	
	pBitDC->SelectObject(pOldBitmap);
	
	ReleaseDC(pDC);
	delete pBitDC;
	return 0;
}

void CMainFrame::OnOutBm() 
{
	CAPlateDoc			*pDoc		= GetDocument();
	CAPlateOutBMStd		*pBMStd		= pDoc->GetAPlateOutBMStd();
	CAPlateOutBMDlg		*pBMDlg		= pBMStd->GetAPlateOutBMDlg();
	
	if(pDoc && !CGeneralBaseStd::m_bInclude_Module_Deck)
	{
		AfxMessageBox("APlateDeck 모듈이 설치되지 않아 작업할 수 없습니다", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	
	if(pDoc)
	{
		pBMDlg->SetWnd(this);
		pBMDlg->DoModal();		
	}	
}
//일반수량산출서 출력...
void CMainFrame::OnUpdateOutBm(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	
	//굳이 DECK모듈 체크 안해도 될듯....070214...KB...
	if(pDoc && pDoc->m_pGenInputStd->IsEnableDialog(ID_DECK_15))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

void CMainFrame::OnOutNondes() 
{
	CAPlateDoc					*pDoc		= GetDocument();
	CAPlateOutBMStd				*pBMStd		= pDoc->GetAPlateOutBMStd();	
	CAPlateOutNonDestructionDlg	*pNonDesDlg	= pBMStd->GetAPlateOutNonDesDlg();
	
	if(pDoc && !CGeneralBaseStd::m_bInclude_Module_Deck)
	{
		AfxMessageBox("APlateDeck 모듈이 설치되지 않아 작업할 수 없습니다", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	
	if(pDoc)	
	{
		pNonDesDlg->SetWnd(this);
		pNonDesDlg->DoModal();		
	}	
}

//비파괴 검사서 출력
void CMainFrame::OnUpdateOutNondes(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	
	if(pDoc && pDoc->m_pGenInputStd->IsEnableDialog(ID_DECK_15))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

LRESULT CMainFrame::OnUpdateTree(WPARAM wParam, LPARAM)
{
	int nID = (int)wParam;

	if(nID<0)	m_paneShortcutInput.InitTreeState();
	else		m_paneShortcutInput.SetItemPositionUpdate(nID, TRUE);

	m_paneShortcutDrawing.InitTree();
	return 0;
}

// 사용자 설계점검 - 단면검토
LRESULT CMainFrame::OnSectionCheck(WPARAM wp, LPARAM lp)
{
	CMDIChildWnd		*pChild	= MDIGetActive();
	if(!pChild) return 0;
	CAPlateDoc			*pDoc	= (CAPlateDoc*)pChild->GetActiveDocument();
	CXLControl			*pXL	= (CXLControl*)wp;
	CDPoint *vBx = (CDPoint*)lp;
	long	nG		= (long)vBx->x;
	double	dSta	= vBx->y;
	
	long	nEndRow	= 0;
	CStringArray szArr;
	if(pDoc)
	{
		CXLSectionCheck	*pXLSectionCheck	= pDoc->GetAPlateOutCalcStd()->GetSectionCheck();
		
		pXLSectionCheck->SetXLControl(pXL);
		nEndRow = pXLSectionCheck->SectionUserCheck(nG, dSta);
		szArr.Add(pXL->GetSheetName());
		pXL->DeleteSheetNotUsed(szArr);
		pXL->SetActiveSheet(szArr[0]);
	}
	
	return nEndRow;
} 

LRESULT CMainFrame::OnModifyInputData(WPARAM wp, LPARAM lp)
{
	CMDIChildWnd		*pChild	= MDIGetActive();
	if(!pChild) return 0;
	CAPlateDoc			*pDoc	= (CAPlateDoc*)pChild->GetActiveDocument();
	CGeneralBaseStd	*pStd	= pDoc->m_pGenInputStd;
	
	CDWordArray ModifyList;
	BOOL	bModify				= (BOOL)lp;
	UINT	nLastInputMenuID	= pStd->GetLastInputMenuID();
	
	if(bModify)
	{
		GetModifyDataList((UINT)wp, ModifyList);
		// 진행되지 않은 Menu Dlg 삭제
		for(long n=0; n<ModifyList.GetSize(); n++)
		{
			if(ModifyList.GetAt(n) >= nLastInputMenuID)
			{
				ModifyList.RemoveAt(n);
				n--;
			}
		}
	}
	else
		ModifyList.Add((UINT)wp);
	
	for(long nID=0; nID<ModifyList.GetSize(); nID++)
	{
		CGeneralBaseDlg *pDlg = pStd->GetDlgOfMenuID(ModifyList.GetAt(nID));
		pDlg->SetModifyData(bModify);
	}
 	if(pDoc->m_bOptModifyCheck)
		m_paneShortcutDrawing.InitTree();

	return 0;
}

void CMainFrame::GetModifyDataList(UINT nID, CDWordArray &ModifyList)
{

}

void CMainFrame::OnHelpGenDetail() 
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\01_APlate_입력부문pdf";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);	
}

void CMainFrame::OnHelpCalc() 
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\02_APlate_구조계산.pdf";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);	
}

void CMainFrame::OnHelpRebar() 
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\06_APlate_바닥판부문.pdf";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);
}

void CMainFrame::OnHelpOut() 
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\03_APlate_출력부문.pdf";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);
}

void CMainFrame::OnHelpAline() 
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\01_ALine_사용설명서.pdf";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);
}

void CMainFrame::OnHelpAboarder() 
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\04_ABorder_사용설명서.pdf";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);
}

void CMainFrame::OnShowF13TBrochure()
{
	CString szPath = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
	szPath += "\\포스코-고력볼트(kpf동아).pdf";
	ShellExecute(NULL,"open", szPath,NULL,NULL,SW_SHOW);
}

LRESULT CMainFrame::OnApplyDomyun(WPARAM, LPARAM)
{
	CMDIChildWnd	*pChild	= MDIGetActive();
	if(!pChild) return 0;
	CAPlateDoc		*pDoc	= (CAPlateDoc*)pChild->GetActiveDocument();
	
	pDoc->m_pShareWithADManagerStd->ApplyFromADManager();
	
	return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CAPlateDoc			*pDoc = NULL;

	CMDIChildWnd		*pChild	= MDIGetActive();
	if(pChild)
	{	
		CAPlateView	 *pView		= (CAPlateView*)pChild->GetActiveView();
		pDoc					= pView->GetDocument();
	}
	
	if (LDK::aIsLogIn()) // 2015. 07. 28. Software Lock 작업, 김형준
	{
		LDK::aIsLockOKOnTimer(nIDEvent, pDoc);
	}
	else
	{
		aIsLockOKOnTimer(nIDEvent, pDoc);		
	}
}

void CMainFrame::CreateShortcutBar()
{	
	m_wndShortcutBar.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CXTPEmptyRect(), this, 0);
	m_wndShortcutBar.SetOwner(this);

	COLORREF clr = XTPImageManager()->SetMaskColor(RGB(192, 192, 192));
	XTPImageManager()->SetIcons(IDB_BITMAP_SHORTCUTS_SMALL, shortcuts, sizeof(shortcuts), CSize(16, 16));
	XTPImageManager()->SetMaskColor(clr);
	XTPImageManager()->SetIcons(IDB_BITMAP_SHORTCUTS_BIG, shortcuts, sizeof(shortcuts), CSize(24, 24));
	XTPImageManager()->SetMaskColor(clr);

	VERIFY(m_paneShortcutStart.Create(_T(""), &m_wndShortcutBar));
	VERIFY(m_paneShortcutInput.Create( _T(""), &m_wndShortcutBar));
	VERIFY(m_paneShortcutDrawing.Create(_T(""), &m_wndShortcutBar));

	CXTPShortcutBarItem* pItemStart = m_wndShortcutBar.AddItem(ID_SHORTCUT_START, &m_paneShortcutStart);
	m_wndShortcutBar.AddItem(ID_SHORTCUT_INPUT, &m_paneShortcutInput);
	m_wndShortcutBar.AddItem(ID_SHORTCUT_DRAWING, &m_paneShortcutDrawing);

	m_paneShortcutStart.ShowCaption(FALSE);	

	m_wndShortcutBar.SelectItem(pItemStart);
	m_wndShortcutBar.LoadState(_T("ShortcutBar"));		

	CXTPDockingPane		*pPane	= m_paneManager.FindPane(IDR_PANE_CONTROL);
	pPane->SetTitle(_T("시작"));
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CXTPMDIFrameWnd::OnSize(nType, cx, cy);
}

void CMainFrame::OnFileNew()
{
	CAPlateApp* pApp = (CAPlateApp*)AfxGetApp();
	pApp->OnFileNew();
	
	m_paneShortcutInput.InitTreeState();
	m_paneShortcutDrawing.InitTree();
	
	CXTPShortcutBarItem* pSelItem = m_wndShortcutBar.GetSelectedItem();	
	if(pSelItem->GetID()==ID_SHORTCUT_INPUT)
	{		
		OnInput(ID_GNINPUT_0);
	}
	else
	{	
		pSelItem = m_wndShortcutBar.GetItem(1);
		m_wndShortcutBar.SelectItem(pSelItem);		
	}
}

void CMainFrame::OnClose() 
{
	SaveCommandBars(_T("CommandBars"));
	m_wndShortcutBar.SaveState(_T("ShortcutBar"));
	
	CFrameWnd::OnClose();
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	CAPlateDoc* pDoc = GetDocument();	

	if(lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if(!pToolBar)
			return FALSE;
		
		if(pToolBar->GetBarID()==IDR_TOOLBAR_DOMYUN)
		{
			if(lpCreateControl->nID==ID_EDIT_PAGE)
			{
				CXTPControlEdit* pPage = (CXTPControlEdit*)CXTPControlEdit::CreateObject();
				pPage->SetCaption(_T("페이지"));
				pPage->SetWidth(72);	// 여기서는 사이즈 영향 없음.
				lpCreateControl->pControl = pPage;
				return TRUE;
			}
			if(lpCreateControl->nID==ID_COMBO_GOINDEX)
			{
				CXTPControlComboBox* pCategory = (CXTPControlComboBox*)CXTPControlComboBox::CreateObject();
				pCategory->SetCaption(_T("카테고리 선택"));
				pCategory->SetWidth(170);	// 여기서는 사이즈 영향 없음.
				lpCreateControl->pControl = pCategory;
				return TRUE;
			}

		}
	}
	
	return FALSE;
}

void CMainFrame::OnCustomize()
{
	CXTPCustomizeSheet cs(GetCommandBars());
	
	CXTPCustomizeOptionsPage pageOptions(&cs);
	cs.AddPage(&pageOptions);
	
	CXTPCustomizeCommandsPage* pCommands = cs.GetCommandsPage();
	pCommands->AddCategories(IDR_MAINFRAME);
	
	pCommands->InsertAllCommandsCategory();
	pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pCommands->InsertNewMenuCategory();
	
	cs.DoModal();
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam==XTP_DPN_SHOWWINDOW)
	{
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		
		if(!pPane->IsValid())
		{
			switch(pPane->GetID())
			{
			case IDR_PANE_CONTROL:
				if(m_wndShortcutBar.GetSafeHwnd()==0)
				{
					CreateShortcutBar();
				}
				pPane->Attach(&m_wndShortcutBar);
				break;

			}
		}
		return TRUE;
	}
	
	return FALSE;
}

LRESULT CMainFrame::OnShortcutBarNotify(WPARAM wParam, LPARAM lParam)
{
	if(!LDK ::aCheckLicenseHGKey()) return 0;

	if(m_bPrintPreview) return 0;	
	
	if(wParam==XTP_SBN_SELECTION_CHANGING)
	{
		CAPlateDoc	*pDoc	= (CAPlateDoc*)GetDocument();
		if(!pDoc)	return 0;
		CAPlateView			*pView	= (CAPlateView*)GetPlateView();
		CGeneralBaseStd		*pIn	= pDoc->GetPlateInputStd();
		CXTPDockingPane		*pPane	= m_paneManager.FindPane(IDR_PANE_CONTROL);
		CXTPShortcutBarItem	*pItem	= (CXTPShortcutBarItem*)lParam;
		CDataManage			*pStd	= pDoc->m_pDataManage;
		UINT nShorcutID = pItem->GetID();
		
		if(nShorcutID==ID_SHORTCUT_START)
			pPane->SetTitle(_T("시작"));
		if(nShorcutID==ID_SHORTCUT_INPUT)
		{
			pPane->SetTitle(_T("입력"));
			
			UINT nMenuID = pIn->GetCurrentMenuID();
			if(nMenuID<ID_GNINPUT_0 || nMenuID>ID_INPUT_END)
				nMenuID = ID_GNINPUT_0;
			
			pDoc->SetFileOpen(TRUE);			
			OnInput(nMenuID);
			m_paneManager.ShowPane(IDR_PANE_INPUT);
		}
		if(nShorcutID==ID_SHORTCUT_DRAWING)
		{
			pPane->SetTitle(_T("도면"));
			m_paneShortcutDrawing.InitTree();
			
			if(!m_bMessageDraw)
			{
				CAPlateDoc			*pDoc	= (CAPlateDoc*)GetDocument();
 				CDataManage			*pStd	= pDoc->m_pDataManage;
 				COptionStd			*pOpt	= pDoc->m_pDomOptionStd;
				CAPlateDrawStd		*pDraw	= pDoc->m_pPlateDrawStd;
				
				m_paneManager.ClosePane(IDR_PANE_INPUT);
				pDoc->GetPlateInputStd()->HideCurrentDialog();
				m_paneManager.ClosePane(IDR_PANE_INPUT);
				pDoc->GetPlateInputStd()->SetSaveMenuIDD(ID_INPUT_END);

				CDrawPageMng		*pDrawMng	= pDraw->GetDrawPageMng();
				if(!pDrawMng->IsPageInit()) pDraw->GetDrawPageMng()->SetPageInit();

				if (pIn->GetDlgOfIDD(IDD_DET_23_SHEARCONNECTORVPOSITION)->IsUsedFlag())
				{
					COutMsgDlg msgDlg(this, pDrawMng);
					pView->SetGL(FALSE);
					msgDlg.Run();

					if(pDraw->m_nCurPage<1)
						pDraw->m_nCurPage = 1;
					OnUpdateToolbarDraw(wParam, lParam);
				}

				pDraw->GetDrawPageMng()->SetModified(wParam);
				if(!pDraw->GetDrawPageMng()->IsPageInit())
				{
					pDraw->GetDrawPageMng()->SetPageInit();
				}
			}
		}
		m_bMessageDraw = FALSE;
		
		return 1;
	}
	
	return 0;
}


void CMainFrame::OnInput(UINT nID)
{
	CAPlateDoc   *pDoc  = GetDocument();	
	CAPlateView  *pView = (CAPlateView*)GetPlateView();
	CGeneralBaseStd	*pIn= pDoc->GetPlateInputStd();

	if(!aIsLockOK() && !LDK::aIsLogIn()) // 2015. 07. 28. Software Lock 작업, 김형준
		return;

	if (pDoc)
	{
		pDoc->SetFileOpen(TRUE);
		if(pIn->GetCurrentMenuID()!=nID)
		{
			OnCreateInputPane(nID);

			CXTPShortcutBarItem* pSelItem = m_wndShortcutBar.GetItem(1);
			m_wndShortcutBar.SelectItem(pSelItem);
			
			// 싸이즈 조정
			CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_INPUT);
			CRect rect;
			int nTitleHeight = 25;
			int nHelpHeight = 0;
			
			CGeneralBaseDlg *pDlg = pIn->GetCurruntDlg();

			pIn->GetCurruntDlg()->GetView()->SetGL(pDlg->IsGL());
			pIn->GetCurruntDlg()->GetWindowRect(&rect);	
			pIn->GetCurruntDlg()->DrawInputDomyunView(TRUE);
			ResizePane(IDR_PANE_INPUT,CSize(250,rect.Height() + nTitleHeight + nHelpHeight),TRUE);
			
		}
	}
}

LRESULT CMainFrame::OnCreateInputPane(WPARAM nID, LPARAM nResize)
{
	CAPlateDoc		*pDoc		= (CAPlateDoc*)GetDocument();
	CAPlateView		*pView		= (CAPlateView*)GetPlateView();
	CGeneralBaseStd	*pInputStd	= pDoc->GetPlateInputStd();
	CXTPDockingPane	*pPane		= m_paneManager.FindPane(IDR_PANE_INPUT);
	
	if(!pDoc)	return 0;
	
	if(nID==ID_INPUT_END)
		nID = ID_GNINPUT_0;

	pView->Clear();
	
	pPane->Detach();
	if(pInputStd->GetCurruntDlg())
		pInputStd->GetCurruntDlg()->DestroyWindow();
	
	OnUpdateInputTitle(nID, nResize);
	
	CGeneralBaseDlg* pDlg = pInputStd->GetDlgOfMenuID(nID);
	pInputStd->SetCurruntDialog(pDlg);
	long nDlgID = pDlg->GetIDD();
	if(!pDlg->GetSafeHwnd())	
	{
		pDlg->m_pView = (CDomyunGLView*)pView;		
		pDlg->m_pRightView = pView->m_pRightView;
		pDlg->Create(pDlg->GetIDD(), this);		
		pDlg->RemoveCaptionBar();
	}
	
	// 싸이즈 조정		
	if(nResize==1)
	{	
		CRect rect;
		int nTitleHeight = 25;
		int nHelpHeight  = 0;
		pInputStd->GetCurruntDlg()->GetWindowRect(&rect);
		pInputStd->SetSaveMenuIDD(pDlg->GetIDD());				
		ResizePane(IDR_PANE_INPUT,CSize(250,rect.Height() + nTitleHeight + nHelpHeight),TRUE);			
	}
	pInputStd->SetSaveMenuIDD(pDlg->GetIDD());
	m_paneShortcutInput.SetItemPosition(nID);	
	
	pPane->Attach(pDlg);
	pPane->Select();
	
	m_paneManager.RedrawWindow();
	m_paneShortcutStart.m_wndTaskPanel.RedrawWindow();
	m_paneShortcutInput.m_wndTreeInput.RedrawWindow();
	m_paneShortcutDrawing.m_wndTreeDrawing.RedrawWindow();
	m_wndShortcutBar.RedrawWindow();
	pView->RedrawWindow();
	return 0;
}


LRESULT CMainFrame::OnUpdateInputTitle(WPARAM nMenuID, LPARAM lParam)
{
	CString strTitle = _T("");
	UINT nItemSize = m_paneShortcutInput.m_cmdIDArr.GetSize();
	BOOL bFind	 = FALSE;
	UINT i		 = 0;
	for(i=0; i<nItemSize; i++)
	{
		UINT nID = m_paneShortcutInput.m_cmdIDArr.GetAt(i);
		if(nID==nMenuID)
		{
			bFind = TRUE;
			break;
		}
	}

	strTitle = m_paneShortcutInput.m_cmdStrArr.GetAt(i);

	CString strBri = _T("");
	strTitle = strBri + strTitle;
	
	CXTPDockingPane* pPane = m_paneManager.FindPane(IDR_PANE_INPUT);
	pPane->SetTitle(strTitle);
	return 0;
}

LRESULT CMainFrame::OnUpdateToolbarDraw(WPARAM wp, LPARAM lp)
{
	CAPlateDoc		*pDoc	= (CAPlateDoc*)GetDocument();
	if(!pDoc)	return 0;
	CGeneralBaseStd	*pIn	= pDoc->GetPlateInputStd();
	CAPlateDrawStd	*pDraw	= pDoc->m_pPlateDrawStd;
	CDrawPageMng	*pManage	= pDraw->GetDrawPageMng();
	
	CXTPToolBar			*pToolBar	= m_pCommandBars->GetToolBar(IDR_TOOLBAR_DOMYUN);
	CXTPControlEdit		*pPage		= (CXTPControlEdit*)pToolBar->GetControl(0);
	CXTPControlComboBox	*pCategory	= (CXTPControlComboBox*)pToolBar->GetControl(1);
	
	long	nPartPage	= pDraw->GetDrawPageMng()->GetPagePartSize();
	CString	str			= _T("");
	long	nCategory	= 0;
	long	nSubPage	= 0;

	if(pDoc && pIn->GetDlgOfIDD(IDD_DET_23_SHEARCONNECTORVPOSITION)->IsUsedFlag())
	{
		nSubPage	= pDoc->m_pPlateDrawStd->GetCurPage();
 		nCategory	= pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetIdxOfPage(nSubPage);
		
		// 페이지
		str.Format(_T("%d"), nSubPage);
		pPage->SetEditText(str);
		
		// 카테고리 선택
		pCategory->ResetContent();
		pCategory->SetWidth(170);	// 실제로 콤보 사이즈가 여기서 조정됨
		int nCountCate = pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetPagePartSize();

		for(long i=0; i<nCountCate; i++)
		{
			str = pDoc->m_pPlateDrawStd->GetDrawPageMng()->GetPage(i)->GetPartTitle();
			pCategory->AddString(str);
		}
		if(nCountCate>0)
			pCategory->SetCurSel(nCategory);
	}
	else
	{
		pPage->SetEditText(_T(""));
		pCategory->ResetContent();
	}
	return 0;
}

void CMainFrame::OnIdleUpdateCmdUI()
{
	BOOL bIdleReCalcRayout = m_nIdleFlags & idleLayout;	
	CFrameWnd::OnIdleUpdateCmdUI();
	if(bIdleReCalcRayout)
	{
		CAPlateDoc	*pDoc		= (CAPlateDoc*)GetDocument();
		if(!pDoc)	return;
		CAPlateView	*pView		= (CAPlateView*)GetPlateView();
		CGeneralBaseStd	*pInputStd	= pDoc->m_pGenInputStd;
		if(pInputStd->GetCurruntDlg())
		{
			pView->SendMessage(WM_COMMAND, ID_ZOOM_ALL);
		}
	}
}

LRESULT CMainFrame::OnShowUpdateDlg(WPARAM wp,LPARAM lp)
{
	CAPlateApp *pApp = (CAPlateApp*)AfxGetApp();
	if(!pApp)	return 0;

	CString sVersion = pApp->GetStringVersion();
	CString sLockInfo = "";		// APlate는 별도의 Edition이 없음(Standard, Basic 구분 없음)

	CDialogAbout	Dlg;
	Dlg.SetProgramName(szAPlateName);
	Dlg.SetLockInfo(sLockInfo);
	Dlg.SetEtcInfo("");
	Dlg.SetVersion(sVersion);
	Dlg.DoModal();

	return 0;
}

void CMainFrame::OnOutWeldChange() 
{
	CAPlateDoc					*pDoc	= GetDocument();
	CAPlateOutBMStd				*pBMStd	= pDoc->GetAPlateOutBMStd();	
	CAPlateOutWeldChangeDlg		*pDlg	= pBMStd->GetAPlateOutWeldChangeDlg();
	
	if(pDoc && !CGeneralBaseStd::m_bInclude_Module_Deck)
	{
		AfxMessageBox("APlateDeck 모듈이 설치되지 않아 작업할 수 없습니다", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	
	if(pDoc)	
	{
		pDlg->SetWnd(this);
		pDlg->DoModal();		
	}		
}

void CMainFrame::OnUpdateOutWeldChange(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	
	if(pDoc && pDoc->m_pGenInputStd->IsEnableDialog(ID_DECK_15))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnViewControlpane() 
{
	if(m_paneManager.IsPaneClosed(IDR_PANE_CONTROL))
		m_paneManager.ShowPane(IDR_PANE_CONTROL);
	else
		m_paneManager.ClosePane(IDR_PANE_CONTROL);
}

void CMainFrame::OnUpdateViewControlpane(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_paneManager.IsPaneClosed(IDR_PANE_CONTROL));
	
}

void CMainFrame::OnViewInputpane() 
{
/**
	if(m_paneManager.IsPaneClosed(IDR_PANE_INPUT))
		m_paneManager.ShowPane(IDR_PANE_INPUT);
	else
		m_paneManager.ClosePane(IDR_PANE_INPUT);
**/
}

void CMainFrame::OnUpdateViewInputpane(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_paneManager.IsPaneClosed(IDR_PANE_INPUT));	
}


void CMainFrame::ResizePane(UINT nID, CSize size, BOOL bHor)
{
	CXTPDockingPane* pPane = m_paneManager.FindPane(nID);
	if (!pPane)	return;

	if(bHor)
	{	
		pPane->SetMinTrackSize(CSize(size.cx, size.cy));
		pPane->SetMaxTrackSize(CSize(32767, size.cy));		
	}
	else
	{
		pPane->SetMinTrackSize(CSize(size.cx, size.cy));
		pPane->SetMaxTrackSize(CSize(size.cx, 32767));		
	}
	m_paneManager.RecalcFrameLayout(pPane);
	m_paneManager.NormalizeSplitters();
	pPane->SetMinTrackSize(CSize(0, 32));
	pPane->SetMaxTrackSize(CSize(32767, 32767));	
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if(!LDK ::aCheckLicenseHGKey() && nID != ID_APP_EXIT && nID != ID_CREATE_LICENSE)
		return FALSE ;

	return CXTPMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame ::OnCreateKey()
{                              	
	LDK:: aLicenseDlgProc(LDK ::aGetLockInfo(), LDK::aGetLockResult ());
}

