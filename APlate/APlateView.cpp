// APlateView.cpp : implementation of the CAPlateView class
//

#include "stdafx.h"
#include "APlate.h"

#include "APlateDoc.h"
#include "APlateView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPlateView
#define AUTOSAVE_TIMER 300
#define UK_PAGE_UP		33
#define UK_PAGE_DOWN	34
#define CLICK_TIMER	   379
UINT CAPlateView::m_nIDEventLock = 0;

IMPLEMENT_DYNCREATE(CAPlateView, CDomyunGLView)

BEGIN_MESSAGE_MAP(CAPlateView, CDomyunGLView)
	//{{AFX_MSG_MAP(CAPlateView)
	ON_COMMAND(ID_ZOOM_ALL, OnZoomAll)
	ON_COMMAND(ID_ZOOM_DYNAMIC, OnZoomDynamic)
	ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
	ON_COMMAND(ID_ZOOM_MOVE, OnZoomMove)
	ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
	ON_COMMAND(ID_ZOOM_PREVIOUS, OnZoomPrevious)
	ON_COMMAND(ID_ZOOM_REALTIME, OnZoomRealtime)
	ON_COMMAND(ID_ZOOM_WINDOW, OnZoomWindow)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()	
	ON_WM_MOUSEMOVE()
 	ON_COMMAND(ID_PAGE_REDRAW, OnPageRedraw)
	ON_COMMAND(ID_SET_LAYER, OnSetLayer)
	ON_COMMAND(ID_SET_DIM, OnSetDim)
	ON_COMMAND(ID_SET_FONT, OnSetFont)
	ON_COMMAND(ID_SET_LINEWIDTH, OnSetLinewidth)
	ON_COMMAND(ID_DXF_OUT_CUR, OnDxfOutCur)
	ON_COMMAND(ID_DRAW_3DMODEL, OnDraw3dmodel)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPTION, OnOption)
	ON_COMMAND(ID_PAGE_NEXT, OnPageNext)
	ON_COMMAND(ID_PAGE_PREV, OnPagePrev)
	ON_COMMAND(ID_BUTTON_ENDPOINT, OnButtonEndpoint)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ENDPOINT, OnUpdateButtonEndpoint)
	ON_COMMAND(ID_BUTTON_PERPOINT, OnButtonPerpoint)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PERPOINT, OnUpdateButtonPerpoint)
	ON_COMMAND(ID_BUTTON_NEAR, OnButtonNear)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_NEAR, OnUpdateButtonNear)
	ON_COMMAND(ID_BUTTON_CENPOINT, OnButtonCenpoint)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CENPOINT, OnUpdateButtonCenPoint)
	ON_COMMAND(ID_BUTTON_MIDPOINT, OnButtonMidpoint)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MIDPOINT, OnUpdateButtonMidPoint)
	ON_COMMAND(ID_BUTTON_CROSSPOINT, OnButtonCrosspoint)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CROSSPOINT, OnUpdateButtonCrossPoint)
	ON_COMMAND(ID_BUTTON_NODE, OnButtonNode)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_NODE, OnUpdateButtonNode)		
 	ON_COMMAND(ID_PAGE_CHANGE, OnPageChange)
	ON_COMMAND(ID_CALC_DIST, OnCalcDist)
	ON_COMMAND(ID_OBJ_INFO, OnObjInfo)
	ON_COMMAND(ID_PAGE_PREV_FIRST, OnPagePrevFirst)
	ON_COMMAND(ID_PAGE_NEXT_LAST, OnPageNextLast)
	ON_COMMAND(ID_PAGEPART_PREV, OnPagepartPrev)
	ON_COMMAND(ID_PAGEPART_NEXT, OnPagepartNext)
	ON_COMMAND(ID_INIT_DRAWPAGE, OnInitDrawpage)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_OBJ_MOVE, OnObjMove)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_UPDATE_COMMAND_UI(ID_OBJ_MOVE, OnUpdateObjMove)
	ON_WM_CHAR()
	ON_COMMAND(ID_DXF_OUT, OnDxfOut)
	ON_COMMAND(ID_ALGIN_ALL, OnAlginAll)
	ON_COMMAND(ID_MOVE_UNDO, OnMoveUndo)
	ON_COMMAND(ID_MOVE_REDO, OnMoveRedo)
	ON_UPDATE_COMMAND_UI(ID_MOVE_REDO, OnUpdateMoveRedo)
	ON_UPDATE_COMMAND_UI(ID_MOVE_UNDO, OnUpdateMoveUndo)
	ON_COMMAND(ID_RECT_TOP, OnRectTop)
	ON_COMMAND(ID_RECT_BOTTOM, OnRectBottom)
	ON_COMMAND(ID_ALIGN_APPLYALL, OnAlignApplyall)
	ON_COMMAND(ID_OBJECT_INFO, OnObjectInfo)	
	ON_UPDATE_COMMAND_UI(ID_OUT_LINE, OnUpdateOutLine)
	ON_COMMAND(ID_ALIGN_LEFT, OnAlignLeft)
	ON_COMMAND(ID_ALIGN_RIGHT, OnAlignRight)
	ON_COMMAND(ID_ALIGN_TOP, OnAlignTop)
	ON_COMMAND(ID_ALIGN_BOTTOM, OnAlignBottom)
	ON_COMMAND(ID_ALIGN_HORZ, OnAlignHorz)
	ON_COMMAND(ID_ALIGN_VER, OnAlignVer)
	ON_COMMAND(ID_ATTACH_HORZ, OnAttachHorz)
	ON_COMMAND(ID_ATTACH_VER, OnAttachVer)
	ON_COMMAND(ID_ALIGN_CENTER, OnAlignCenter)
	ON_COMMAND(ID_ALIGN_VCENTER, OnAlignVcenter)
 	ON_COMMAND(ID_EXPORT_BRX, OnExportBrx)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_BRX, OnUpdateExportBrx)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_COMMAND(ID_INSERT_NOTE, OnInsertNote)
	ON_UPDATE_COMMAND_UI(ID_INSERT_NOTE, OnUpdateInsertNote)	
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_IMPORT_BRX, OnImportBrx)
	ON_COMMAND(ID_EXPORT_TEXT, OnExportText)
	ON_COMMAND(ID_IMPORT_TEXT, OnImportText)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_TEXT, OnUpdateExportText)
	ON_COMMAND(ID_EXPORT_BRX2007, OnExportBrx2007)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_BRX2007, OnUpdateExportBrx2007)
	ON_COMMAND(ID_EXPORT_ADMANAGER, OnExportAdmanager)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_ADMANAGER, OnUpdateExportAdmanager)
	ON_COMMAND(ID_CALC_STATION, OnCalcStation)
	ON_COMMAND(ID_CALC_LINEINFO, OnCalcLineInfo)
	ON_UPDATE_COMMAND_UI(ID_CALC_STATION, OnUpdateCalcStation)
	ON_UPDATE_COMMAND_UI(ID_CALC_LINEINFO, OnUpdateCalcLineInfo)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_AIRIALVIEW, OnAirialView)
	ON_COMMAND_RANGE(ID_MOVE_UNDO_DUMMY,ID_MOVE_UNDO_DUMMY+50,OnUndo)
	ON_COMMAND_RANGE(ID_MOVE_REDO_DUMMY,ID_MOVE_REDO_DUMMY+50,OnRedo)		
	ON_COMMAND(ID_MAKE_INPUTMENU, OnMakeInputmenu)
	ON_COMMAND(ID_INPUT_END, OnInputEnd_PageRedraw)
	ON_COMMAND(ID_FILE_PRINT, CDomyunGLView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CDomyunGLView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CDomyunGLView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DOMYUN_TOOL_START, ID_DOMYUN_TOOL_END, OnUpdateDomyunAll)	
	ON_UPDATE_COMMAND_UI_RANGE(ID_ALIGN_TOOL_START, ID_ALIGN_TOOL_END, OnUpdateAlign)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_START, ID_TOOL_END, OnUpdateAll)
	ON_XTP_EXECUTE_RANGE(ID_EDIT_PAGE, ID_COMBO_GOINDEX, OnToolbarControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPlateView construction/destruction


CAPlateView::CAPlateView():CDomyunGLView(FALSE)
{
	COutMsgDlg::m_bStatsMessage = TRUE;
	m_bUseToolTip = TRUE;
	m_bTransZoomMove = FALSE;
	m_bModeCursor = FALSE;	
	m_bPainted	= FALSE;
	m_bPush = FALSE;
	m_bClickCircle	= FALSE;
		
	SetDF(DF_EDGE_LINE_DRAW,FALSE);
	SetDF(DF_THICK_DRAW,TRUE); 
	SetDF(DF_BLEND,FALSE); 
		
	m_ArrCur= AfxGetApp()->LoadStandardCursor(IDC_ARROW);	
	m_MoveCur= AfxGetApp()->LoadCursor(IDC_MOVE);
	
	m_bDomSet = FALSE;
}
	
CAPlateView::~CAPlateView()
{	
}

BOOL CAPlateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAPlateView drawing

void CAPlateView::OnDraw(CDC* pDC)
{
	CAPlateDoc	*pDoc	= GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_pGenInputStd->GetSaveMenuIDD() == ID_INPUT_END && IsGL()) 
		SetGL(FALSE);
	
	if(!IsGL()) 
	{
		CDomyunGLView::OnDraw(pDC);
	}
	else
	{
		if(!IsOwnerMouseMode())
			SetOwnerMouseMode(TRUE);
		
		DrawMouseLine(FALSE);
		DrawMouseRect(FALSE);

		SetRanderContextState(TRUE);
		pDoc->m_pGenInputStd->m_FEMDraw.Render(pDC);	
		
		SwapBuffers(m_hDC);				
		glFinish();
		SetRanderContextState(FALSE);		
		
		if(m_bOwnerMouse && GetFocus()==this && !m_bPrint)
		{
			DrawMouseLine(TRUE);
			DrawMouseRect(TRUE);		
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAPlateView printing

BOOL CAPlateView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CAPlateView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	CAPlateDoc* pDoc = GetDocument();
	SetLineThickStruct(((CARoadOptionStd*)pDoc->m_pDataManage->GetOptionStd())->m_LineThick);
// 	if(pInfo->m_bPreview) pMFrm->m_wndPageViewBar.ShowWindow(SW_HIDE);
	CDomyunGLView::OnBeginPrinting(pDC, pInfo);
}

void CAPlateView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CDomyunGLView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CAPlateView diagnostics

#ifdef _DEBUG
void CAPlateView::AssertValid() const
{
	CDomyunGLView::AssertValid();
}

void CAPlateView::Dump(CDumpContext& dc) const
{
	CDomyunGLView::Dump(dc);
}

CAPlateDoc* CAPlateView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAPlateDoc)));
	return (CAPlateDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAPlateView message handlers

void CAPlateView::DestroyDialog()
{
	CMainFrame	*pFrm  = (CMainFrame*)AfxGetMainWnd();
	CAPlateDoc	*pDoc  = GetDocument();
	
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);
	
	pDoc->GetPlateInputStd()->HideCurrentDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->GetPlateInputStd()->SetSaveMenuIDD(ID_INPUT_END);
}


void CAPlateView::OnInitialUpdate() 
{
	CDomyunGLView::OnInitialUpdate();

	CMainFrame			*pMFrm		= (CMainFrame*)AfxGetMainWnd();
	CXTPMenuBar			*pMenuBar	= pMFrm->m_pCommandBars->GetMenuBar();
	CAPlateDoc			*pDoc		= GetDocument();
	CGeneralBaseStd		*pInStd		= pDoc->GetPlateInputStd();

	CWnd *pCWnd	= GetDomyun()->GetCWnd();
	pDoc->GetAPlateOutCalcStd()->SetCWnd(pCWnd);
	pDoc->GetAPlateOutBMStd()->SetWnd(pCWnd);

	pDoc->m_pDataManage->SetDomyunView(this);
	pDoc->m_pPlateDrawStd->m_pWnd = this;
	
	pDoc->m_pPlateDrawStd->SetDomyun(this);	
	pDoc->m_pPlateDrawStd->SetInitDogak(pDoc->m_pDomOptionStd->m_szDGKName,"");
	SetRanderContextState(TRUE);
	pDoc->m_pGenInputStd->m_FEMDraw.InitializeOpenGL(m_hDC);
	SetRanderContextState(FALSE);
	
	SetDomyun();

	pMFrm->m_MTIClientWnd.Attach(pMFrm);
	pMFrm->m_MTIClientWnd.EnableToolTips();

	pDoc->m_pGenInputStd->m_FEMDraw.SetGLView(this);
 	pInStd->InsertInputMenu(pMenuBar);

	// 도각정보 재조정
	CAPlateDrawStd	*pDrawStd	= pDoc->m_pPlateDrawStd;
	CDrawDogak		*pDgk		= pDrawStd->GetDogak();
	COptionStd		*pOptionStd	= pDoc->m_pDomOptionStd;
	pDrawStd->SetInitDogak(pOptionStd->m_szDGKName, pOptionStd->m_sHead_DomyunNumber);	

	CBMCreator *pBMStandard = pDoc->m_pDataManage->GetBMStandard();
	pBMStandard->m_pTStandard->SetStatusBar((CXTPStatusBar*)&pMFrm->m_wndStatusBar);
	pBMStandard->m_pTStandard->m_pWnd = this;

	SetOwnerMouseMode(TRUE);

	////////////////////////////////////////////////////////////////////////////	
	// 제일 처음 실행일 경우 설정창을 보여줌
	CEnvRegist env;
	if(!env.LoadEnvInt(CEnvRegist::SET_PRGOPT))
		pMFrm->SendMessage(WM_COMMAND, ID_OPT_PRG);

	SetAutoSave(env.LoadEnvInt(CEnvRegist::AUTO_SAVE));
	SetCrossAngle(env.LoadEnvDouble(CEnvRegist::CROSSCURSOR_ANGLE));
	
	CRect rect;
	long nLen = 0;
	GetClientRect(&rect);
	double dLenRatio = env.LoadEnvDouble(CEnvRegist::CROSSCURSOR_LENGTH);
	if(dLenRatio==0)
		nLen = 30;
	else
	{
		if(dLenRatio!=100)
			nLen = (long)(rect.Width()/2*dLenRatio/100);
	}
	SetCrossLength(nLen);

	CARoadOptionStd	*pARoadOptStd	= pDoc->m_pDataManage->GetOptionStd();
	SetLineThickStruct(pARoadOptStd->m_LineThick);
	////////////////////////////////////////////////////////////////////////////
	CString sPath = env.LoadEnvStr(CEnvRegist::FONT_DIR)+"\\";	
	CAPlateDrawPyung::m_InputDomRotate = env.LoadEnvInt(CEnvRegist::INPUTDOM_ROTATE);
	if(sPath=="\\")
		sPath = env.LoadEnvStr(CEnvRegist::PRG_DIR) + "\\FONTS\\";
	if(!LoadShxFont(sPath + "romans.shx"))					
		pMFrm->AddHisData("Romans.shx File Not Found !\r\n");
	if(!LoadShxFont(sPath + "whgtxt.shx"))					
		pMFrm->AddHisData("whgtxt.shx File Not Found !\r\n");
	SetFontDir(sPath);
	SetUseShx(env.LoadEnvInt(CEnvRegist::USE_SHXFONT));		
	SetUseToolTip(env.LoadEnvInt(CEnvRegist::USE_MOVETOOLTIP));
	CGridCtrl::SetComboButton(env.LoadEnvInt(CEnvRegist::USE_GRIDCOMBO_BUTTON));
	m_bUseToolTip = env.LoadEnvInt(CEnvRegist::USE_MOVETOOLTIP);
	COutMsgDlg::m_bStatsMessage = env.LoadEnvInt(CEnvRegist::USE_PROGRESSDLG);	

	pMFrm->m_paneShortcutInput.CreateIconByModule();
	////////////////////////////////////////////////////////////////////////////
	// 선형 데이터 Set
	CString csErrorMsg(_T(""));
	if( pDoc->m_pDataManage->GetLineInfoApp()->SetDataIp(csErrorMsg) == FALSE )
	{
		if(csErrorMsg!="")	MessageBox(csErrorMsg);		
		return;
	}	
	if(pMFrm->m_wndShortcutBar.GetSafeHwnd()==0)
		pMFrm->CreateShortcutBar();
	pMFrm->m_paneShortcutStart.InitMRUList();
	////////////////////////////////////////////////////////////////////////////
	// 계산 모듈 실행
	SetPrevExe(TRUE);
	
	if(pDoc->m_pGenInputStd->GetSaveMenuIDD())		
	{
		if(pDoc->m_pGenInputStd->GetSaveMenuIDD() != ID_INPUT_END || env.LoadEnvInt(CEnvRegist::USE_DOMDRAWATSTART))
			pDoc->m_pGenInputStd->GoCurrentDlg();
	}

	pDoc->SetModifiedFlag(FALSE);

	////////////////////////////////////////////////////////////////////////////
	// PageViewBar Set
	
	CDWordArray ModifyList;
	CGeneralBaseStd	*pStd	= pDoc->m_pGenInputStd;
	
	for(long n=0; n<pStd->GetSizeDialogList(); n++)
	{
		CGeneralBaseDlg *pDlg = pStd->GetDlgByIndex(n);
		if(pDlg->IsModifyData())
		{
			long nID = pDlg->GetMenuID();
			ModifyList.Add(pDlg->GetMenuID());
		}
	}
	pMFrm->SendMessage(WM_UPDATE_STATUSBAR);
	pMFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);

	AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	Sleep(0);
}

void CAPlateView::SetAutoSave(BOOL bStart)
{
	UINT nElapse = 300000;//1000 * 60 * 5(5분)
	if(bStart)
		SetTimer(AUTOSAVE_TIMER, nElapse, NULL);
	else
		KillTimer(AUTOSAVE_TIMER);
}

void CAPlateView::OnZoomAll() 
{
	GetDomyunCtrl().ZoomAll();	
}

void CAPlateView::OnZoomDynamic() 
{
	GetDomyunCtrl().SetZoomRect(3);				
}

void CAPlateView::OnZoomIn() 
{
	GetDomyunCtrl().ZoomPlusNegaExt(TRUE);		
}

void CAPlateView::OnZoomMove() 
{
	GetDomyunCtrl().SetZoomRect(2);	
}

void CAPlateView::OnZoomOut() 
{
	GetDomyunCtrl().ZoomPlusNegaExt(FALSE);	
}

void CAPlateView::OnZoomPrevious() 
{
	GetDomyunCtrl().ZoomPrew();	
}

void CAPlateView::OnZoomRealtime() 
{
	GetDomyunCtrl().SetZoomRect(4);				
}

void CAPlateView::OnZoomWindow() 
{
	GetDomyunCtrl().SetZoomRect(1);	
}

void CAPlateView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(!m_bTransZoomMove) return;

//	if(GetDomyunCtrl().TransZoomMove()) return;	
	
	CDrawTool* pTool = FindTool(c_drawShape);	
	if(pTool->m_drawShape != CDrawTool::normal) return;

	CMenu menuZoom, menuOrder, *pContextMenuZoom=NULL;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	
	//입력과 도면의 팝업메뉴 구성이 틀려짐.
	
	if(m_bModeCursor)	menuZoom.LoadMenu(IDR_POPUP_ALIGN);
	else				menuZoom.LoadMenu(IDR_POPUP_ZOOM);

	
	pContextMenuZoom = menuZoom.GetSubMenu(0);
	pFrm->m_pCommandBars->TrackPopupMenu(pContextMenuZoom, TPM_LEFTALIGN,  point.x, point.y, this);	
}

BOOL CAPlateView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	//return CDomyunGLView::OnEraseBkgnd(pDC);
}

void CAPlateView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CAPlateDoc* pDoc = GetDocument();			
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	CMainFrame *pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CString cs;
	CDPoint xy = GetDomyunCtrl().ClientToExt(point);	
	
	cs.Format("%14.6lf, %14.6lf",xy.y,xy.x);

	//pMainFrm->m_wndStatusBar.SetPaneText(MOUSE_AXIAL_TEXT,cs);	
	CXTPStatusBarPane *pPane = pMainFrm->m_wndStatusBar.GetPane(MOUSE_AXIAL_TEXT);
	pPane->SetText(cs);
	
	pStd->OnMouseMove(nFlags, point);
	if(pDoc->m_bMoveCursor)
		CView::OnMouseMove(nFlags, point);
	else
		CDomyunGLView::OnMouseMove(nFlags, point);
	
	CDrawTool* pTool = FindTool(c_drawShape);	
	CGeneralBaseDlg *pDlg = pDoc->m_pGenInputStd->GetCurruntDlg();
	if(!IsGL() && pDlg && pTool->m_drawShape == CDrawTool::normal) SetTimer(77, 10, NULL);

	CDomyunGLView::OnMouseMove(nFlags, point);
}

void CAPlateView::OnInputEnd_PageRedraw() 
{
	CMainFrame* pMFrm = (CMainFrame*)AfxGetMainWnd();
	CAPlateDoc* pDoc = GetDocument();
	
	pMFrm->m_paneShortcutDrawing.InitTree();
	
	OnPageRedraw();
}

void CAPlateView::OnPageRedraw() 
{
	CAPlateDoc* pDoc = GetDocument();			
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	DestroyDialog();	
	pDoc->m_pGenInputStd->SetSaveMenuIDD(ID_INPUT_END);
	
	if(!pStd->GetDrawPageMng()->IsPageInit())
		pStd->GetDrawPageMng()->SetPageInit();
	if(pStd->GetCurPage() < 1 || pStd->m_nCurPage > pStd->GetTotalDomyunPage())
		pStd->m_nCurPage = 1;
	pStd->SetModified(pStd->GetCurPage(),FALSE);
	long nSel = pStd->GetDrawPageMng()->GetIdxOfPage(pStd->GetCurPage());
	pStd->GetDrawPageMng()->SetModified(nSel);	
	COutMsgDlg msgDlg(this,pStd->GetDrawPageMng());	
	msgDlg.Run();

	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);					
	AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);
}

void CAPlateView::OnSetLayer() 
{	
	CAPlateDoc* pDoc = GetDocument();
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	CARoadOptionSheet pSht("설정",pDoc->m_pDataManage->GetOptionStd(),this);	
	pSht.SetActivePage(0);
	if(pSht.DoModal()==IDOK)	
		pStd->SetModifiedAll();	
	
}

void CAPlateView::OnSetDim() 
{
	CAPlateDoc* pDoc = GetDocument();
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	CARoadOptionSheet pSht("설정",pDoc->m_pDataManage->GetOptionStd(),this);	
	pSht.SetActivePage(1);
	if(pSht.DoModal()==IDOK)	
		pStd->SetModifiedAll();		
}

void CAPlateView::OnSetFont() 
{
	CAPlateDoc* pDoc = GetDocument();
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	CARoadOptionSheet pSht("설정",pDoc->m_pDataManage->GetOptionStd(),this);	
	pSht.SetActivePage(2);
	if(pSht.DoModal()==IDOK)	
		pStd->SetModifiedAll();		
}

void CAPlateView::OnSetLinewidth() 
{
	CAPlateDoc* pDoc = GetDocument();
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	CARoadOptionSheet pSht("설정",pDoc->m_pDataManage->GetOptionStd(),this);	
	pSht.SetActivePage(3);
	if(pSht.DoModal()==IDOK)	
		pStd->SetModifiedAll();		
}

void CAPlateView::OnDxfOutCur() 
{
	CString sFilter;
	sFilter.Format(hgwgettext_be("데이터 (*.dxf)|*.dxf|모든 파일 (*.*)|*.*||"));
	CFileDialog dlg(FALSE, _T("*.dxf|*.*"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFilter, this);

	CString sTitle = _T("DXF 데이터 파일 저장");
	dlg.m_ofn.lpstrTitle = sTitle.GetBuffer(0);

	CString szPath;
	if (dlg.DoModal() == IDOK)
	{
		szPath = dlg.GetPathName();
	}
	else
	{
		AfxMessageBox(_T("DXF 파일 저장에 실패하였습니다.\nLayer등 도면설정을 확인하여 주시기 바랍니다."));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	GetDomyunCtrl().GetDomyun()->SetDxfVer(1);
	if(GetDomyunCtrl().GetDomyun()->DxfOut(szPath))
	{
		CString sMsg;
		sMsg = hgwgettext_be("내보내기가 완료되었습니다.\n내보낸 파일을 여시겠습니까?");

		if(AfxMessageBox(sMsg, MB_YESNO) == IDYES)
		{
			ShellExecute(NULL, NULL, szPath, _T(""), NULL, SW_SHOWNORMAL);
		}
	}
}

void CAPlateView::OnDraw3dmodel() 
{	

}

void CAPlateView::OnUpdateDomyunAll(CCmdUI* pCmdUI)
{
	CAPlateDoc*		pDoc	= GetDocument();
	CAPlateDrawStd *pStd	= pDoc->m_pPlateDrawStd;
	CDrawPageMng   *pManage	= pStd->GetDrawPageMng();

	if(pDoc==NULL)	return;
	
	if(!pDoc->m_pGenInputStd) return;
	
	if(!pDoc->m_pGenInputStd->GetDlgOfMenuID(ID_DETINPUT_9)->IsUsedFlag())
		pCmdUI->Enable(FALSE);
	else
	{
		pCmdUI->Enable(TRUE);

		if (!m_bDomSet)
		{
			AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
			m_bDomSet = TRUE;
		}
	}

}

void CAPlateView::OnUpdateAll(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	long nPage = pStd->m_nCurPage;
	if (nPage<1)
	{
		nPage = 1;
		pStd->SetCurPage(nPage);
	}
	
	CString strPage;

	strPage.Format(" 페이지 : [ %ld / %ld ]",pStd->GetCurPage(),pStd->GetTotalPageSize());	
	CXTPStatusBarPane *pPane = pMFrm->m_wndStatusBar.GetPane(0);
	
	pPane->SetText(strPage);
	
	COutMovePoint *pMove = pStd->GetMovePoint(nPage);
	if(pMove != NULL)
		pStd->GetMovePoint(nPage)->SetVHMode(m_bOnlyVHDraw);
	if(pDoc->m_bBrpFileOpen != 0 && pStd->GetMovePoint(nPage) != NULL)
	{
		CXTPStatusBarPane *pPane = pMFrm->m_wndStatusBar.GetPane(MOVE_TEXT);
		if(pStd->GetMovePoint(nPage)->IsVHMode())
			//pMFrm->m_wndStatusBar.SetPaneText(MOVE_TEXT,"직각",TRUE);
			pPane->SetText("직각");
		else
			pPane->SetText("자유");
			//pMFrm->m_wndStatusBar.SetPaneText(MOVE_TEXT,"자유",TRUE);
	}		
	
	if(pDoc && pDoc->m_bBrpFileOpen)	pCmdUI->Enable(TRUE);
	else								pCmdUI->Enable(FALSE);
}

void CAPlateView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	
	m_bTransZoomMove = m_bContextMenu;
	CDomyunGLView::OnRButtonUp(nFlags, point);
}

void CAPlateView::OnOption() 
{
	CAPlateDoc* pDoc = GetDocument();
	COptionSheet OptDlg("APlate 도면 설정", this);

	OptDlg.m_pPlateDrawStd	= pDoc->m_pPlateDrawStd;
	pDoc->m_pDomOptionStd->m_nOptionCur = pDoc->m_nOptionCur;
	OptDlg.DoModal();
	pDoc->m_nOptionCur = pDoc->m_pDomOptionStd->m_nOptionCur;
}

void CAPlateView::OnPageNext() 
{
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;

	DestroyDialog();
	
	if(!pStd->GetDrawPageMng()->IsPageInit())
		pStd->GetDrawPageMng()->SetPageInit();
	pStd->GetDrawPageMng()->GoNextPage();	

	AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);	
}

void CAPlateView::OnPagePrev() 
{	
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;

	DestroyDialog();
	
	if(!pStd->GetDrawPageMng()->IsPageInit())
		pStd->GetDrawPageMng()->SetPageInit();
	pStd->GetDrawPageMng()->GoPrevPage();	

	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);
	AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
}

void CAPlateView::OnPageChange() 
{
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

	DestroyDialog();
	
	pDoc->m_pGenInputStd->SetSaveMenuIDD(ID_INPUT_END);	
	if(pStd->GetCurPage() < 1 || pStd->m_nCurPage > pStd->GetTotalDomyunPage()) pStd->m_nCurPage = 1;	
	COutMsgDlg msgDlg(this,pStd->GetDrawPageMng());	
	msgDlg.Run();

// 	AfxGetMainWnd()->SendMessage(WM_CREATE_INPUT,0,0);
	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);	

	pFrm->SendMessage(WM_UPDATE_DRAWING_TITLE, pStd->m_nCurPage);
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	
	Sleep(0);
}

void CAPlateView::OnCalcDist() 
{	
	SetZoomRect(6);
}

void CAPlateView::OnObjInfo() 
{		
	SetZoomRect(SELOBJECT);
}

void CAPlateView::OnButtonEndpoint() 
{
	long nPre = GetOsnapMode();
	//nPre &= OSNAP_END;
	nPre ^= OSNAP_END;
	SetOsnapMode(nPre);	
}

void CAPlateView::OnUpdateButtonEndpoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here	
	long nPre = GetOsnapMode();
	
	if(GetDocument()->m_bBrpFileOpen) pCmdUI->Enable(TRUE);		
	else							  pCmdUI->Enable(FALSE);			

	if(nPre & OSNAP_END) pCmdUI->SetCheck(1);
	else				 pCmdUI->SetCheck(0);
}

void CAPlateView::OnButtonPerpoint() 
{
	long nPre = GetOsnapMode();
	nPre ^= OSNAP_PER;
	SetOsnapMode(nPre);	
}

void CAPlateView::OnUpdateButtonPerpoint(CCmdUI* pCmdUI) 
{	
	long nPre = GetOsnapMode();

	if(GetDocument()->m_bBrpFileOpen) pCmdUI->Enable(TRUE);		
	else							  pCmdUI->Enable(FALSE);			

	if(nPre & OSNAP_PER) pCmdUI->SetCheck(1);
	else				 pCmdUI->SetCheck(0);
}
void CAPlateView::OnButtonNear() 
{
	long nPre = GetOsnapMode();
	nPre ^= OSNAP_NEAR;
	SetOsnapMode(nPre);
}

void CAPlateView::OnUpdateButtonNear(CCmdUI* pCmdUI) 
{
	long nPre = GetOsnapMode();
	
	if(GetDocument()->m_bBrpFileOpen) pCmdUI->Enable(TRUE);		
	else							  pCmdUI->Enable(FALSE);			

	if(nPre & OSNAP_NEAR) pCmdUI->SetCheck(1);
	else				  pCmdUI->SetCheck(0);
}

void CAPlateView::OnButtonCenpoint() 
{
	long nPre = GetOsnapMode();
	nPre ^= OSNAP_CEN;
	SetOsnapMode(nPre);
}

void CAPlateView::OnUpdateButtonCenPoint(CCmdUI* pCmdUI) 
{
	long nPre = GetOsnapMode();
	
	if(GetDocument()->m_bBrpFileOpen) pCmdUI->Enable(TRUE);		
	else							  pCmdUI->Enable(FALSE);			

	if(nPre & OSNAP_CEN) pCmdUI->SetCheck(1);
	else				 pCmdUI->SetCheck(0);
}

void CAPlateView::OnButtonMidpoint() 
{
	long nPre = GetOsnapMode();
	nPre ^= OSNAP_MID;
	SetOsnapMode(nPre);	
}

void CAPlateView::OnUpdateButtonMidPoint(CCmdUI* pCmdUI) 
{
	long nPre = GetOsnapMode();
	
	if(GetDocument()->m_bBrpFileOpen) pCmdUI->Enable(TRUE);		
	else							  pCmdUI->Enable(FALSE);			

	if(nPre & OSNAP_MID)  pCmdUI->SetCheck(1);
	else				  pCmdUI->SetCheck(0);
}

void CAPlateView::OnButtonCrosspoint() 
{
	long nPre = GetOsnapMode();
	nPre ^= OSNAP_CROSS;
	SetOsnapMode(nPre);	
}

void CAPlateView::OnUpdateButtonCrossPoint(CCmdUI* pCmdUI) 
{
	long nPre = GetOsnapMode();
	
	if(GetDocument()->m_bBrpFileOpen) pCmdUI->Enable(TRUE);		
	else							  pCmdUI->Enable(FALSE);			

	if(nPre & OSNAP_CROSS)  pCmdUI->SetCheck(1);
	else					pCmdUI->SetCheck(0);
}

void CAPlateView::OnButtonNode() 
{	
	long nPre = GetOsnapMode();
	nPre ^= OSNAP_NODE;
	SetOsnapMode(nPre);	
}

void CAPlateView::OnUpdateButtonNode(CCmdUI* pCmdUI) 
{		
	long nPre = GetOsnapMode();

	if(GetDocument()->m_bBrpFileOpen) pCmdUI->Enable(TRUE);		
	else							  pCmdUI->Enable(FALSE);			

	if(nPre & OSNAP_NODE)  pCmdUI->SetCheck(1);
	else				  pCmdUI->SetCheck(0);
}

void CAPlateView::OnPagePrevFirst() 
{	
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	DestroyDialog();

	long nPage = pStd->m_nCurPage;
	long nCombo = pStd->GetDrawPageMng()->GetIdxOfPage(nPage);	
	if(nCombo == 0) return;

	pStd->m_nCurPage = pStd->GetDrawPageMng()->GetPageByIdx(nCombo);
	COutMsgDlg msgDlg(this,pStd->GetDrawPageMng());
	msgDlg.Run();	
	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);	
	AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);
}

void CAPlateView::OnPageNextLast() 
{
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;

	long nPage = pStd->m_nCurPage;
	long nCombo = pStd->GetDrawPageMng()->GetIdxOfPage(nPage);	
	if(nCombo == DOMTYPENUM-1) return;
			
	DestroyDialog();
	
	pStd->m_nCurPage = pStd->GetDrawPageMng()->GetPageByIdx(++nCombo)-1;
	COutMsgDlg msgDlg(this,pStd->GetDrawPageMng());
	msgDlg.Run();
	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);	
	AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);	
}

void CAPlateView::OnPagepartPrev() 
{
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
	
	long nPage = pStd->m_nCurPage;
	long nCombo = pStd->GetDrawPageMng()->GetIdxOfPage(nPage);	
	if(nCombo == 0) return;

	DestroyDialog();

	pStd->m_nCurPage = pStd->GetDrawPageMng()->GetPageByIdx(--nCombo);		
	COutMsgDlg msgDlg(this,pStd->GetDrawPageMng());
	msgDlg.Run();
	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);	
	AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);
	
}

void CAPlateView::OnPagepartNext() 
{
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;

	long nPage = pStd->m_nCurPage;
	long nCombo = pStd->GetDrawPageMng()->GetIdxOfPage(nPage);	
	if(nCombo == DOMTYPENUM-1) return;
			
	DestroyDialog();
	
	pStd->m_nCurPage = pStd->GetDrawPageMng()->GetPageByIdx(++nCombo);
	COutMsgDlg msgDlg(this,pStd->GetDrawPageMng());
	msgDlg.Run();	
	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);	
	AfxGetMainWnd()->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);		
}

void CAPlateView::OnInitDrawpage() 
{
	CAPlateDoc* pDoc = GetDocument();	
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;	

	if(MessageBox("현재도면의 위치정보를 삭제하고 재배치 합니다.\n 계속 하시겠습니까 ?","확인",MB_ICONQUESTION | MB_YESNO)==IDYES)
	{
		pStd->InitMovePointInfo();						
 		OnPageRedraw();
	}
}

BOOL CAPlateView::PreTranslateMessage(MSG* pMsg) 
{
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	CAPlateDoc* pDoc = GetDocument();
	
	if(m_bUseToolTip) m_wndToolTip.RelayEvent (pMsg);

	if(pMsg->wParam == 27 && pMsg->message == WM_KEYDOWN &&
		(GetDomyunCtrl().GetDrawShape() != CDrawTool::normal || pDoc->m_bMoveCursor)) 
	{							
		if(pDoc->m_bMoveCursor) OnObjMove();
		m_bModeCursor = FALSE;
		GetDomyunCtrl().SetViewModeCtrl(CDomyunGLView::NORMAL);
		CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
		COutMovePoint *pMove = pStd->GetMovePoint(pStd->GetCurPage());		
		if(pMove) pMove->ClearSelect();			
	}

	if(pMsg->wParam == 27 && pMsg->message == WM_KEYDOWN &&
		(GetDomyunCtrl().GetDrawShape() != CDrawTool::normal || pDoc->m_bMoveCursor)) 
	{							
		if(pDoc->m_bMoveCursor) OnObjMove();
		m_bModeCursor = FALSE;
		GetDomyunCtrl().SetViewModeCtrl(CDomyunGLView::NORMAL);
		CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;
		COutMovePoint *pMove = pStd->GetMovePoint(pStd->GetCurPage());		
		if(pMove) pMove->ClearSelect();			
	}
	if(pMsg->wParam == VK_F11 && pMsg->message == WM_KEYDOWN)
	{
		SetUseShx(IsUseShp() ? FALSE : TRUE);
		if(m_bFastTextDraw && !IsUseShp())
			OnPageRedraw();
		else
			RedrawAll();
	}
	
	return CDomyunGLView::PreTranslateMessage(pMsg);
}


void CAPlateView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
	CAPlateDoc* pDoc = GetDocument();
	
	if(nChar == VK_ESCAPE && !pDoc->m_bEnableCrossCursor) 
	{
		m_bModeCursor = FALSE;		
		if(pDoc->m_bMoveCursor) OnObjMove();
	}
	
	if(nChar == VK_ESCAPE && GetDomyun()->m_SelectedObjMap.GetCount()) 
	{
		m_bModeCursor = FALSE;
		GetDomyun()->m_SelectedObjMap.RemoveAll();
	}
	switch((int)nChar)
	{	
	case VK_F8 : SetHVMode(!IsHVMode()); return;
	}	
	
	pDoc->m_pPlateDrawStd->OnKeyDown(nChar,nFlags,nFlags);	
	CDomyunGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAPlateView::OnObjMove() 
{	
	CAPlateDoc* pDoc = GetDocument();
	
	if(pDoc->m_bMoveCursor) 
	{
		if(m_bUseToolTip) 
		{
			m_wndToolTip.UpdateTipText("",this);
			m_wndToolTip.Activate(FALSE);
		}
		m_bMoveCursor = FALSE;
		m_bOwnerMouse = m_bOldOwnerMouse;		
		pDoc->m_bMoveCursor = FALSE;
		SetViewModeCtrl(NORMAL);
		GetDomyunCtrl().SetViewModeCtrl(CDomyunGLView::NORMAL);
		CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;
		COutMovePoint *pMove = pStd->GetMovePoint(pStd->GetCurPage());		
		if(pMove) 
			pMove->ClearSelect();
		ShowTaskButton(FALSE);
	}
	else 
	{
		pDoc->m_bMoveCursor = TRUE;
		m_bMoveCursor = TRUE;
		if(m_bUseToolTip) m_wndToolTip.Activate(TRUE);
		m_bOldOwnerMouse = m_bOwnerMouse;
		m_bOwnerMouse = FALSE;
		ShowTaskButton();
	}
}

int CAPlateView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDomyunGLView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndToolTip.Create(this);	
	m_wndToolTip.Activate(FALSE);
	m_wndToolTip.AddTool(this, _T(""));
	
	long nChk = atoi(AfxGetApp()->GetProfileString("Program Option", "CheckVersion",""));
	if(nChk==0) SetTimer(100, 1000 * 60 * 20, NULL);
	
	return 0;
}

void CAPlateView::SetUseHCadBlock(BOOL vUseBlock)
{
	CHgBaseDrawStd::SetUseHCadBlock(vUseBlock);
}

void CAPlateView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMainFrame			*pFrm	= (CMainFrame*)AfxGetMainWnd();
	CAPlateDoc			*pDoc	= pFrm->GetDocument();
	CAPlateDrawStd		*pDraw	= pDoc->m_pPlateDrawStd;
	CGeneralBaseDlg		*pDlg	= pDoc->m_pGenInputStd->GetCurruntDlg();
	CDrawTool			*pTool	= FindTool(c_drawShape);
	CXTPDockingPane		*pPane	= pFrm->m_paneManager.FindPane(IDR_PANE_CONTROL);

	if(pDlg && pTool->m_drawShape==CDrawTool::normal) 
		pDlg->OnLButtonDown(nFlags, point);

	if(pDraw->IsInsertNoteMode() && pDoc->m_pGenInputStd->GetSaveMenuIDD()==ID_INPUT_END)
		pDraw->OnLButtonDown(nFlags, point);		
	if(m_bMoveCursor>0 && GetDrawShape()==CDrawTool::normal)
		pDraw->OnLButtonDown(nFlags, point);
	
	if(m_bMoveCursor>0 && m_bUseToolTip>0) 
		m_wndToolTip.Activate(FALSE);

	// 화면 클릭시 자동연결
	if(pDoc && pPane->GetTitle()==_T("입력"))
	{
		if(pDoc->m_pGenInputStd && pTool->m_drawShape == CDrawTool::normal)
		{
			CDPoint xy = ClientToExt(point);
			pDoc->m_pGenInputStd->m_strME.xy = xy;
			pDoc->m_pGenInputStd->m_strME.bViewMouseEvent = TRUE;
			CGeneralBaseDlg* pDlg = pDoc->m_pGenInputStd->GetCurruntDlg();
			if(pDlg->GetSafeHwnd())
				pDlg->SendMessage(WM_LBUTTONDOWN);
		}
	}

 	if(IsGL() && m_ViewMode == NORMAL && !pDoc->m_pGenInputStd->m_FEMDraw.GetFlag(FEMVIEW_LOADCASE) && (nFlags & MK_LBUTTON))
 	{
 		SetRanderContextState(TRUE);
 		if(pDoc->m_pGenInputStd->m_FEMDraw.MousePickingForce(point))
 			Invalidate(FALSE);
 		SetRanderContextState(FALSE);
 	}

	if(pDlg && pTool->m_drawShape == CDrawTool::normal)
	{
		POINT pt;
		GetCursorPos(&pt);						
		ScreenToClient(&pt);
		CRect rect(pt.x-m_nRectSize,pt.y-m_nRectSize,pt.x+m_nRectSize,pt.y+m_nRectSize);	
		m_bStopFind = FALSE;
		CObArray obArr;
		if(Intersects(&obArr,rect,INTER_TEXT))
		{
			CDrawObject* pSelObj = (CDrawObject*)obArr.GetAt(0);

			if(pSelObj->GetEttKind()==ETT_TEXT)
			{						
				CObText *pText = (CObText *)pSelObj;	
				CString szText = pText->m_TextString;
				if(pDlg->IsFocusItem(szText))				
				{					
					CDC* pDC = GetDC();
					CDRect rectText = GetDomyun()->GetTextRect(pText, szText);						
					
					rectText.left += pText->m_SttPoint.x;
					rectText.top  += pText->m_SttPoint.y;
					rectText.right += pText->m_SttPoint.x;
					rectText.bottom += pText->m_SttPoint.y;

					CRect rect = ExtToClient(rectText);	
					rect.NormalizeRect();
					rect.DeflateRect(-2,-2);

					DrawMouseLine(FALSE);
					DrawMouseRect(FALSE);
					pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));												
					rect.DeflateRect (1, 1);
					pDC->Draw3dRect (rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_3DLIGHT));

					if(m_bOwnerMouse && GetFocus()==this && !m_bPrint && m_DrawToolZoom.m_ZoomMode !=  CDrawTool::predynamic)
					{	
						DrawMouseLine(TRUE);
						DrawMouseRect(TRUE);		
					}		
					ReleaseDC(pDC);					
					m_bPush = TRUE;
				}			
			}
		}		
	}

	CDomyunGLView::OnLButtonDown(nFlags, point);		
}

void CAPlateView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CAPlateDoc		*pDoc	= GetDocument();
	CAPlateDrawStd	*pStd	= pDoc->m_pPlateDrawStd;
	CMainFrame		*pFrm	= (CMainFrame*)GetParentOwner();
	CXTPDockingPane	*pPane	= pFrm->m_paneManager.FindPane(IDR_PANE_CONTROL);
	CGeneralBaseDlg	*pDlg	= pDoc->m_pGenInputStd->GetCurruntDlg();
	CDrawTool		*pTool	= FindTool(c_drawShape);

	if(m_bMoveCursor)
	{
		pStd->OnLButtonUp(nFlags, point);	
		OnSetCursor(this,0,0);
	}

	// 화면 클릭시 자동연결	
	if(m_bPush && pDlg && pTool->m_drawShape == CDrawTool::normal)
	{
		POINT pt;
		GetCursorPos(&pt);						
		ScreenToClient(&pt);	
		CRect rect(pt.x-m_nRectSize,pt.y-m_nRectSize,pt.x+m_nRectSize,pt.y+m_nRectSize);	
		//CRect rect(pt.x-2,pt.y-2,pt.x+2,pt.y+2);	
		m_bStopFind = FALSE;
		CObArray obArr;
		if(Intersects(&obArr,rect,INTER_TEXT))
		{
			CDrawObject* pSelObj = (CDrawObject*)obArr.GetAt(0);
			for(long i = 0; i < obArr.GetSize(); i++)
			{
				pSelObj	= (CDrawObject*)obArr.GetAt(i);
				if(pSelObj->GetEttKind() == ETT_TEXT)
					break;
			}
			if(pSelObj->GetEttKind()==ETT_TEXT)
			{				
				CObText *pText = (CObText *)pSelObj;					
				CString szText = pText->m_TextString;
				if(pDlg->IsFocusItem(szText))// && !m_bHasFocus)					
				{					
					CDC* pDC = GetDC();
					CDRect rectText = GetDomyun()->GetTextRect(pText, szText);						
					
					rectText.left += pText->m_SttPoint.x;
					rectText.top  += pText->m_SttPoint.y;
					rectText.right += pText->m_SttPoint.x;
					rectText.bottom += pText->m_SttPoint.y;

					CRect rect = ExtToClient(rectText);	
					rect.NormalizeRect();
					rect.DeflateRect(-2,-2);

					DrawMouseLine(FALSE);
					DrawMouseRect(FALSE);
					pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));												
					rect.DeflateRect (1, 1);
					pDC->Draw3dRect (rect, ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
					// 마우스 그림
					if(m_bOwnerMouse && GetFocus()==this && !m_bPrint && m_DrawToolZoom.m_ZoomMode !=  CDrawTool::predynamic)
					{	
						DrawMouseLine(TRUE);
						DrawMouseRect(TRUE);		
					}		
					pDlg->SetFocusItem(szText);
					ReleaseDC(pDC);						
				}			
			}
		}			
		m_bPush = FALSE;
	}	

	if(pDoc && pPane->GetTitle()==_T("입력"))
	{
		if(pDoc->m_pGenInputStd && pTool->m_drawShape == CDrawTool::normal)
		{
			CDPoint xy = ClientToExt(point);
			pDoc->m_pGenInputStd->m_strME.xy = xy;
			pDoc->m_pGenInputStd->m_strME.bViewMouseEvent = TRUE;
			CGeneralBaseDlg	*pDlg = pDoc->m_pGenInputStd->GetCurruntDlg();
			if(pDlg->GetSafeHwnd())
				pDlg->SendMessage(WM_LBUTTONUP);
		}
	}
	CDomyunGLView::OnLButtonUp(nFlags, point);
}

BOOL CAPlateView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CAPlateDoc* pDoc = GetDocument();
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;

	if( pStd->IsInsertNoteMode())
	{
		Sleep(0);
		DrawMouseLine(FALSE);
		DrawMouseRect(FALSE);
		if(pWnd==this) 
		{
			pStd->OnSetCursor(pWnd, nHitTest, message);			
		}		
		return TRUE;
	}
	if(GetDocument()->m_bMoveCursor && GetDomyunCtrl().GetDrawShape() == CDrawTool::normal)
	{		
		long nPage = pStd->m_nCurPage;
		COutMovePoint *pMove = pStd->GetMovePoint(nPage);
		if(pMove && pMove->m_bContinue) return TRUE;		
		Sleep(0);
		DrawMouseLine(FALSE);
		DrawMouseRect(FALSE);
		if(pWnd==this) 
		{
			if(!pStd->OnSetCursor(pWnd, nHitTest, message))
			{				
				m_bModeCursor = FALSE;				
				m_wndToolTip.Activate (FALSE);
				SetCursor(m_MoveCur);
			}
			else
			{
				m_bModeCursor = TRUE;
				if(m_bUseToolTip && !pStd->GetMovePoint(pStd->m_nCurPage)->IsSelectRect())
				{												
					m_wndToolTip.Activate (TRUE);
					m_wndToolTip.UpdateTipText(pStd->GetToolTipInfo(),this);															
				}			
				return TRUE;
			}			
		}	
		SetCursor(m_MoveCur);
		return TRUE;
	}

	return CDomyunGLView::OnSetCursor(pWnd, nHitTest, message);
}

void CAPlateView::OnUpdateObjMove(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	long nPre = GetOsnapMode();

	if( pDoc->m_bBrpFileOpen != 0)	pCmdUI->Enable(TRUE);	
	else							pCmdUI->Enable(FALSE);
	
	if( pDoc->m_bMoveCursor) pCmdUI->SetCheck(1);
	else				     pCmdUI->SetCheck(0);	
}

void CAPlateView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMainFrame		*pMFrm		= (CMainFrame*)GetParentOwner();
	CAPlateDoc		*pDoc		= GetDocument();
	CAPlateCalcStd	*pCalcStd	= pDoc->m_pCalcStd;
	CAPlateDrawStd	*pStd		= pDoc->m_pPlateDrawStd;

	if(nChar == 27 && pStd->IsInsertNoteMode()) 
	{
		pStd->SetInsertNoteMode(FALSE);
	}

	if(nChar == 27 && pDoc->m_bMoveCursor) 
	{		
		m_bModeCursor = FALSE;		
		OnObjMove();
	}
	if(nChar == 27 && GetDomyun()->m_SelectedObjMap.GetCount()) 
	{
		m_bModeCursor = FALSE;
		GetDomyun()->m_SelectedObjMap.RemoveAll();
	}
	
	switch(nChar)
	{
	case 'j':
	case 'J':
		 pDoc->m_pGenInputStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_NUMBER,!pDoc->m_pGenInputStd->m_FEMDraw.GetFlag(FEMVIEW_JOINT_NUMBER));	
		Invalidate(FALSE);
		break;
	case 'e':
	case 'E':
		pDoc->m_pGenInputStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_NUMBER,!pDoc->m_pGenInputStd->m_FEMDraw.GetFlag(FEMVIEW_ELEMENT_NUMBER));	
		Invalidate(FALSE);
		break;
	case 'c':
	case 'C':
		pDoc->m_pGenInputStd->m_FEMDraw.SetFlag(FEMVIEW_EDGE_CUT,!pDoc->m_pGenInputStd->m_FEMDraw.GetFlag(FEMVIEW_EDGE_CUT));	
		Invalidate(FALSE);
		break;
	case 'r':
	case 'R':
		pDoc->m_pGenInputStd->m_FEMDraw.SetFlag(FEMVIEW_RENDER_RESTRAINT,!pDoc->m_pGenInputStd->m_FEMDraw.GetFlag(FEMVIEW_RENDER_RESTRAINT));	
		Invalidate(FALSE);
		break;
	}

	OnSetCursor(this,0,0);
	
	CDomyunGLView::OnChar(nChar, nRepCnt, nFlags);
}

void CAPlateView::OnDxfOut() 
{
	CAPlateDoc* pDoc = GetDocument();
	CString szDir = ((CAPlateApp *)AfxGetApp())->GetFixedDir();

	((CAPlateDrawStd* )pDoc->m_pPlateDrawStd)->ShowShopDxfDlg(szDir);	
}


void CAPlateView::OnUpdateAlign(CCmdUI* pCmdUI) 
{	
	CAPlateDoc* pDoc = GetDocument();		
	CAPlateDrawStd *pStd = pDoc->m_pPlateDrawStd;

	if(pDoc==NULL || pStd==NULL) return;
	
	long nPage = pStd->m_nCurPage;
	
	if(pStd->GetMovePoint(nPage)==NULL) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(pStd->GetMovePoint(nPage)->m_SelectedMPE.GetSize() < 2) 
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CAPlateView::OnAlginAll() 
{
	CAlignSelectDlg selDlg(GetDocument()->m_pPlateDrawStd);

	selDlg.DoModal();
}


void CAPlateView::OnMakeInputmenu() 
{
	CMainFrame* pMFrm = (CMainFrame*)AfxGetMainWnd();
	CAPlateDoc* pDoc = GetDocument();
				
}

void CAPlateView::OnUndo(WPARAM nID) 
{
	CAPlateDoc* pDoc = GetDocument();		
	CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;
	long nPage = pStd->m_nCurPage;
	COutMovePoint *pMove = pStd->GetMovePoint(nPage);
	
	long nSelID = nID - ID_MOVE_UNDO_DUMMY;
	
	for(int i=pMove->m_UnDoMPE.GetSize()-1;i>=nSelID;i--)
		pMove->OnUnDo(i);		
}

void CAPlateView::OnRedo(WPARAM nID) 
{	
	CAPlateDoc* pDoc = GetDocument();		
	CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;
	long nPage = pStd->m_nCurPage;
	COutMovePoint *pMove = pStd->GetMovePoint(nPage);
	
	long nSelID = nID - ID_MOVE_REDO_DUMMY;
	
	for(int i=pMove->m_ReDoMPE.GetSize()-1;i>=nSelID;i--)
		pMove->OnReDo(i);		
}


void CAPlateView::OnMoveUndo() 
{
	CAPlateDoc* pDoc = GetDocument();		
	CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;
	long nPage = pStd->m_nCurPage;
	COutMovePoint *pMove = pStd->GetMovePoint(nPage);

	OnUndo(ID_MOVE_UNDO_DUMMY + pMove->m_UnDoMPE.GetSize() - 1);	
}

void CAPlateView::OnMoveRedo() 
{
	CAPlateDoc* pDoc = GetDocument();		
	CAPlateDrawStd *pStd = (CAPlateDrawStd *)pDoc->m_pPlateDrawStd;
	long nPage = pStd->m_nCurPage;
	COutMovePoint *pMove = pStd->GetMovePoint(nPage);

	OnRedo(ID_MOVE_REDO_DUMMY + pMove->m_ReDoMPE.GetSize() - 1);	
}

void CAPlateView::OnUpdateMoveRedo(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	CAPlateDrawStd *pStd = (CAPlateDrawStd*)pDoc->m_pPlateDrawStd;
	long nPage = pStd->GetCurPage();
	COutMovePoint *pMove = pStd->GetMovePoint(nPage);
		
	if(pMove!=NULL) 
	{
		if(pMove->m_ReDoMPE.GetSize())	pCmdUI->Enable(TRUE);
		else							pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);	
}

void CAPlateView::OnUpdateMoveUndo(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	CAPlateDrawStd *pStd = (CAPlateDrawStd*)pDoc->m_pPlateDrawStd;;
	long nPage = pStd->GetCurPage();
	COutMovePoint *pMove = pStd->GetMovePoint(nPage);
		
	if(pMove!=NULL) 
	{
		if(pMove->m_UnDoMPE.GetSize()) pCmdUI->Enable(TRUE);
		else						  pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CAPlateView::OnRectTop() 
{
	GetDocument()->m_pPlateDrawStd->OnRectTop();			
}

void CAPlateView::OnRectBottom() 
{
	GetDocument()->m_pPlateDrawStd->OnRectBottom();			
}

void CAPlateView::OnAlignApplyall() 
{
	GetDocument()->m_pPlateDrawStd->OnSamePosition();	
}

void CAPlateView::OnObjectInfo() 
{
	GetDocument()->m_pPlateDrawStd->OnInfoDlg();				
}

void CAPlateView::OnUpdateOutLine(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	if(pDoc && pDoc->m_bBrpFileOpen)	pCmdUI->Enable(TRUE);
	else								pCmdUI->Enable(FALSE);	
}

void CAPlateView::OnAlignLeft() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignLeft();		
}

void CAPlateView::OnAlignCenter() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignCenter();	
}

void CAPlateView::OnAlignRight() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignRight();	
}

void CAPlateView::OnAlignTop() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignTop();
}

void CAPlateView::OnAlignVcenter() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignVCenter();	
}

void CAPlateView::OnAlignBottom() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignBottom();
}

void CAPlateView::OnAlignHorz() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignHorz();
}

void CAPlateView::OnAlignVer() 
{
	GetDocument()->m_pPlateDrawStd->OnAlignVer();	
}

void CAPlateView::OnAttachHorz() 
{
	GetDocument()->m_pPlateDrawStd->OnAttachHor();
}

void CAPlateView::OnAttachVer() 
{
	GetDocument()->m_pPlateDrawStd->OnAttachVer();	
}

void CAPlateView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();
// 	pMFrm->m_wndPageViewBar.ShowWindow(SW_SHOW);
	CDomyunGLView::OnEndPrintPreview(pDC, pInfo, point, pView);
}


void CAPlateView::OnExportBrx() 
{
	CAPlateDoc* pDoc = GetDocument();
	CFileDialog dlg(FALSE, "*.brx",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "APlate Export Files (*.brx)|*.brx||");
	dlg.m_ofn.lpstrTitle = "APier용 정보파일 저장";
	if (dlg.DoModal() == IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&f,CArchive::store);
		pDoc->ExportAPierInfo(ar);		
		ar.Close();
		f.Close();
	}	
}

void CAPlateView::OnUpdateExportBrx(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();			
	if( pDoc->m_bBrpFileOpen == 0 )	pCmdUI->Enable(FALSE);
	else							pCmdUI->Enable(TRUE);	
}

void CAPlateView::OnSetFocus(CWnd* pOldWnd) 
{
	CDomyunGLView::OnSetFocus(pOldWnd);

}

void CAPlateView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CAPlateDoc *pDoc=GetDocument();		
	CMainFrame* pMFrm = (CMainFrame*)GetParentOwner();

	if(pDoc->m_bMoveCursor)
	{
		OnSetCursor(this,0,0);
	}	

	if(pDoc->m_pGenInputStd->GetCurMenuID()==ID_CLINPUT_22)
	{
		CCalcSecOutline *pDlg = pDoc->m_pGenInputStd->GetCalcSecOutline();
		pDlg->SetLButtonDblClk(nFlags, point);
		return;
	}	

	if(pDoc->m_pGenInputStd->GetCurruntDlg() && pDoc->m_pGenInputStd->GetCurruntDlg()->GetIDD()==IDD_DECK_07_PLANREBAR)
	{
		CDeckPlanRebarDlg *pDlg = (CDeckPlanRebarDlg*)pDoc->m_pGenInputStd->GetCurruntDlg();
		if(pDlg) 
		{
			m_ptClick = point;
			m_nClickCircleRadius = 1;
			m_bClickCircle = FALSE;
			SetTimer(CLICK_TIMER, 1, NULL);
		}
	}

	CDomyunGLView::OnLButtonDblClk(nFlags, point);
}

void CAPlateView::OnTimer(UINT nIDEvent) 
{
	CAPlateApp *pApp = (CAPlateApp*)AfxGetApp();
	CAPlateDoc *pDoc = GetDocument();	
	
	if(nIDEvent == 77)
	{		
		CDrawTool* pTool = FindTool(c_drawShape);	
		CGeneralBaseDlg *pDlg = pDoc->m_pGenInputStd->GetCurruntDlg();
		if(pDlg && pTool->m_drawShape == CDrawTool::normal)
		{
			POINT pt;
			GetCursorPos(&pt);						
			ScreenToClient(&pt);			
			CRect rect(pt.x-2,pt.y-2,pt.x+2,pt.y+2);	
			m_bStopFind = FALSE;
			CObArray obArr;
			if(Intersects(&obArr,rect,INTER_TEXT))
			{
				CDrawObject* pSelObj = (CDrawObject*)obArr.GetAt(0);		
				if(pSelObj->GetEttKind()==ETT_TEXT)
				{				
					CObText *pText = (CObText *)pSelObj;					
					if(pDlg->IsFocusItem(pText->m_TextString))// && !m_bHasFocus)					
					{					
						if (m_bPainted)							
							return;					
						else 
						{
							m_bPainted = TRUE;	
							CDC* pDC = GetDC();
							CDRect rectText = GetDomyun()->GetTextRect(pText, pText->m_TextString);						
							
							rectText.left += pText->m_SttPoint.x;
							rectText.top  += pText->m_SttPoint.y;
							rectText.right += pText->m_SttPoint.x;
							rectText.bottom += pText->m_SttPoint.y;

							CRect rect = ExtToClient(rectText);	
							rect.NormalizeRect();
							rect.DeflateRect(-2,-2);

							DrawMouseLine(FALSE);
							DrawMouseRect(FALSE);						

							if(m_bPush)
							{
								pDC->Draw3dRect(rect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));												
								rect.DeflateRect (1, 1);
								pDC->Draw3dRect (rect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_3DLIGHT));
							}
							else
							{
								pDC->Draw3dRect(rect, ::GetSysColor(COLOR_BTNHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));												
								rect.DeflateRect (1, 1);
								pDC->Draw3dRect (rect, ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_BTNSHADOW));
							}
							// 마우스 그림
							if(m_bOwnerMouse && GetFocus()==this && !m_bPrint && m_DrawToolZoom.m_ZoomMode !=  CDrawTool::predynamic)
							{	
								DrawMouseLine(TRUE);
								DrawMouseRect(TRUE);		
							}		
							ReleaseDC(pDC);		
						}		
					}
					else
					{
						KillTimer(77);
						if (m_bPainted == TRUE) 
						{											
							Invalidate();							
							m_bActivate = FALSE; //깜박이 제어
							m_bPainted = FALSE;
						}							
						return;
					}				 
				}
			}
			else
			{
				KillTimer(77);
				if (m_bPainted == TRUE) 
				{
					Invalidate();					
					m_bPainted = FALSE;
				}							
				return;
			}
		}				
	}

	if(nIDEvent == AUTOSAVE_TIMER)
	{
		
		CString szDir = ((CAPlateApp *)AfxGetApp())->GetFixedDir();
		szDir += "\\AutoSave.brp";
		CFile f;
		if(f.Open(szDir,CFile::modeCreate|CFile::modeWrite)!=FALSE)
		{
			CArchive ar(&f,CArchive::store);
			pDoc->Serialize(ar);
			ar.Close();
			f.Close();
		}
		
	}

	if(nIDEvent == 100)
	{
		CString szRunTime;
		aUpdateCheck("", atoi(AfxGetApp()->GetProfileString("Program Option", "CheckVersion","")), szRunTime);
		if(!szRunTime.IsEmpty())	
			AfxGetApp()->WriteProfileString("Settings", "LastRunDate",szRunTime);	
	}

	
	if(nIDEvent == CLICK_TIMER)
	{
		// 이전 원 삭제
		CDC* pDC = GetDC();
		CPen Pen(PS_SOLID,3,RGB_YELLOW), *pOldPen;
		pOldPen = pDC->SelectObject(&Pen);
		pDC->SetROP2(R2_XORPEN);
		CRect rect(m_ptClick.x-m_nClickCircleRadius,m_ptClick.y-m_nClickCircleRadius,
				   m_ptClick.x+m_nClickCircleRadius,m_ptClick.y+m_nClickCircleRadius);
		POINT pt = {m_nClickCircleRadius*2, m_nClickCircleRadius*2};
		if(m_bClickCircle)						
			pDC->Arc(&rect,pt,pt);
		
		if(m_nClickCircleRadius<40)
		{			
			m_nClickCircleRadius += 2;			
			rect = CRect(m_ptClick.x-m_nClickCircleRadius,m_ptClick.y-m_nClickCircleRadius,
				   m_ptClick.x+m_nClickCircleRadius,m_ptClick.y+m_nClickCircleRadius);
			pt.x = m_nClickCircleRadius*2;
			pt.y = m_nClickCircleRadius*2;
			
			pDC->Arc(&rect,pt,pt);					
		}
		else
		{
			KillTimer(CLICK_TIMER);
			CDeckPlanRebarDlg *pDlg = (CDeckPlanRebarDlg*)pDoc->m_pGenInputStd->GetCurruntDlg();
			if(pDlg) pDlg->SetPosition(m_ptClick);
		}
		pDC->SelectObject(pOldPen);
		ReleaseDC(pDC);
		m_bClickCircle = TRUE;
	}
	
	CDomyunGLView::OnTimer(nIDEvent);
}

void CAPlateView::OnInsertNote() 
{
	if(!GetDocument()->m_pPlateDrawStd->IsInsertNoteMode())
		GetDocument()->m_pPlateDrawStd->SetInsertNoteMode(TRUE);
	else
		GetDocument()->m_pPlateDrawStd->SetInsertNoteMode(FALSE);		
}

void CAPlateView::OnUpdateInsertNote(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_pGenInputStd->GetSaveMenuIDD() == ID_INPUT_END)	
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);

	if(GetDocument()->m_pPlateDrawStd->IsInsertNoteMode()) 
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CAPlateView::OnAirialView()
{
	CAPlateDoc* pDoc = GetDocument();
	CGeneralBaseDlg *pDlg = pDoc->m_pGenInputStd->GetCurruntDlg();
	if(pDlg)
	{
		CButton *pButton = (CButton *)pDlg->GetDlgItem(IDC_CHECK_TOTALVIEW);	
		if(pButton)	
		{
			if(pButton->GetCheck()==0)
				pButton->SetCheck(1);	
			else
				pButton->SetCheck(0);	
			pDlg->UpdateData();
		}
		pDlg->DrawInputDomyunView();
	}
}

void CAPlateView::SetPrevExe(BOOL bFirst)
{
	CAPlateDoc		*pDoc		= GetDocument();	
	CAPlateCalcStd	*pCalcStd	= pDoc->m_pCalcStd;
	CDataManage		*pDataMng	= pDoc->m_pDataManage;
	CGeneralBaseStd	*pInStd		= pDoc->m_pGenInputStd;
	CCalcGeneral	*pCalcGen	= pCalcStd->m_pCalcGeneral;

	CFemCantileverCalc FemCantilever(pDataMng, pCalcStd);
	CFemHLLCalc      HLCalc(pDataMng, pCalcStd);
	CFemModelingCalc ModelCalc(pDataMng, pCalcStd);
	CFemStressCalc   StressCalc(pDataMng, pCalcStd);

	pDataMng->SetCheckEmptyMaterial(TRUE);
	
	BOOL bCheck[5] = { FALSE, FALSE, FALSE, FALSE, FALSE };
	BOOL bUse[5] = { FALSE, FALSE, FALSE, FALSE, FALSE };
	long nSize[5] = { -1, -1, -1, -1, -1 };
	
	bUse[0] = pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_SECFORCE_DIV_CALC)->IsUsedFlag();
	bUse[1] = pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_WINDFORCE)->IsUsedFlag();
	bUse[2] = pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_MODELING_SETUP)->IsUsedFlag();
	bUse[3] = pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_INF_K)->IsUsedFlag();
	bUse[4] = pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_CANTILEVER_MODELING)->IsUsedFlag();

	nSize[0]=pCalcStd->GetSizeHLLFemData();
	nSize[1]=pCalcStd->GetSizeWindFemData();
	nSize[2]=pCalcStd->GetSizeModelingFemData();
	nSize[3]=pCalcStd->GetSizeInfluenceFemData();
	nSize[4]=pCalcStd->GetSizeCantileverFemData();

//	try 
	{
		CGeneralBaseDlg	*pDlg = pInStd->GetDlgOfIDD(IDD_CALC_CANTILEVER_MODELING);
		CMainFrame *pMainFrm = (CMainFrame*) AfxGetMainWnd();

		pMainFrm->m_wndStatusBar.SetPaneText(0,"바닥판 해석중입니다...");

		if(pDlg && pDlg->IsUsedFlag() && nSize[4]==0)
		{
//			if(강선이 있으면.)
			FemCantilever.MakeCantileverModeling(MAKE_ALL);
		}
		// 횡분배
		if(pInStd->GetDlgOfIDD(IDD_CALC_SECFORCE_DIV_CALC)->IsUsedFlag() && nSize[0]==0)
		{
			HLCalc.RunCrossAnalysis();
			nSize[0] = pCalcStd->GetSizeHLLFemData();
		}
		if (nSize[0]>0)	bCheck[0] = TRUE;

		// 풍하중
		if(pInStd->GetDlgOfIDD(IDD_CALC_WINDFORCE)->IsUsedFlag() && nSize[1]==0)
		{
			HLCalc.RunWindAnalysis();
			nSize[1] = pCalcStd->GetSizeWindFemData();
		}
		if (nSize[1])	bCheck[1] = TRUE;

		// 프레임 모델링
		if(bCheck[0]&&pInStd->GetDlgOfIDD(IDD_CALC_MODELING_SETUP)->IsUsedFlag() && nSize[2]==0)
		{
			ModelCalc.MakeFrameModeling();
			nSize[2] = pCalcStd->GetSizeModelingFemData();
		}
		if (bCheck[0] && nSize[2])	bCheck[2] = TRUE;

		// 영향계수 계산
		if(pInStd->GetDlgOfIDD(IDD_CALC_INF_K)->IsUsedFlag() && nSize[3]==0)
		{
			HLCalc.RunInfluenceAnalysis();
			nSize[3] = pCalcStd->GetSizeInfluenceFemData();
		}
		if (nSize[3])	bCheck[3] = TRUE;
	}
// 	catch(...)
// 	{
// 		AfxMessageBox("계산중 오류가 발생합니다. !\n한길아이티로 문의하시기 바랍니다.", MB_ICONSTOP | MB_OK);
// 	}

	if(bFirst)
	{	
		if (bCheck[2]) ModelCalc.MakeResultData();
		if (bCheck[2]) ModelCalc.MakeSummaryOfElementForce();
		if (bCheck[2]) ModelCalc.MakeSummaryOfJointForce();
		if (bCheck[2]) ModelCalc.MakeMaxMinForceDB();
		if (bCheck[2]) ModelCalc.MakeMaxMinForceDL();
		if (bCheck[2]) ModelCalc.MakeMaxMinForceMTY();
		if (bCheck[2]) ModelCalc.MakeMaxMinForcePeople();
		if(pDoc->m_pCalcStd->GetCalcData()->m_bWind)
			if (bCheck[2]) ModelCalc.MakeMaxMinForceWind();

		// 단면구성도
		if(pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_SECTION_OUTLINE)->IsUsedFlag())
		{
			StressCalc.SetSectionGugan();
			StressCalc.SetSectionSplice();
			for(long nG=0; nG<pDoc->m_pDataManage->GetBridge()->GetGirdersu(); nG++)
				StressCalc.MakeDanmyunMomentData(nG);
		}
	}

	// Moving Load Check
	if (nSize[2]>0)
	{
		CFEMManage	*pFEM	= ModelCalc.m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);

		POSITION pos = pFEM->m_MovingLoadList.GetHeadPosition();
		while (pos)
		{
			CMovingLoad *pMovingLoad = (CMovingLoad *)pFEM->m_MovingLoadList.GetNext(pos);

			if (pMovingLoad->m_nRF<1) pMovingLoad->m_nRF = 1;
		}
	}

	pDataMng->SetCheckEmptyMaterial(FALSE);
}

void CAPlateView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(!IsGL() && m_bModeCursor)				
		GetDocument()->m_pPlateDrawStd->OnRButtonDown(nFlags, point);
	
	CDomyunGLView::OnRButtonDown(nFlags, point);
}

void CAPlateView::OnImportBrx() 
{
	CAPlateDoc* pDoc = GetDocument();
	CFileDialog dlg(TRUE, "*.brx",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "ARoad Export Files (*.brx)|*.brx||");
	dlg.m_ofn.lpstrTitle = "ARoad 정보파일 읽기";
	if (dlg.DoModal() == IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;
		CArchive ar(&f,CArchive::load);
		pDoc->ImportAPierInfo(ar);		
		ar.Close();
		f.Close();
	}	
	
}

void CAPlateView::OnExportText() 
{
	CAPlateDoc* pDoc = GetDocument();
	CFileDialog dlg(FALSE, "*.btx",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "교량정보파일 Files (*.btx)|*.btx||");
	dlg.m_ofn.lpstrTitle = "교량정보파일 저장";

	CString newName, newNameLin;
	newName = GetDocument()->GetPathName();
	if (newName.IsEmpty())
	{
		newName = GetDocument()->GetTitle();
		int iBad = newName.FindOneOf(_T(" #%;/\\"));    // dubious filename
		if (iBad != -1)
			newName.ReleaseBuffer(iBad);

		// append the default suffix if there is one
		newName += ".btx";
	}	
	newName.Replace(".brp",".btx");	
	dlg.m_ofn.lpstrFile = newName.GetBuffer(_MAX_PATH);
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString szAEdit = szPath + "\\AEdit.exe";
	if (dlg.DoModal() == IDOK)
	{
		newNameLin = dlg.GetPathName();
		newNameLin.Replace(".btx",".lin");
		CFile f;
		if(f.Open(newNameLin,CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&f,CArchive::store);
		pDoc->m_pDataManage->GetLineInfoApp()->Serialize(ar);
		ar.Close();
		f.Close();
		
		ofstream outfile (dlg.GetPathName(),ofstream::out);		
		pDoc->ExportBridgeInfo(outfile, dlg.GetPathName());				
		outfile.close();
		ShellExecute(NULL,NULL,szAEdit,"\"\tNEW" + dlg.GetPathName() + "\"",NULL,SW_SHOWNORMAL);	
	}	
}

void CAPlateView::OnUpdateExportText(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();
	if( pDoc->m_bBrpFileOpen == 0 )	pCmdUI->Enable(FALSE);
	else							pCmdUI->Enable(TRUE);			
}

void CAPlateView::OnImportText() 
{
	CAPlateDoc* pDoc = GetDocument();	
	CFileDialog dlg(TRUE, "*.btx",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "교량정보파일 Import Files (*.btx)|*.btx||");
	dlg.m_ofn.lpstrTitle = "교량정보파일 가져오기";
	CString newLineName;
	BOOL bResult;
	if (dlg.DoModal() == IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;
		CArchive ar(&f,CArchive::load);
		bResult = pDoc->ImportBridgeInfo(ar);		
		ar.Close();
		f.Close();

		newLineName = dlg.GetPathName();
		newLineName.Replace(".btx",".lin");
		BOOL bFindOK = FALSE;
		CFileFind ff;
		bFindOK = ff.FindFile(newLineName);
		if(bFindOK)
		{		
			if(f.Open(newLineName,CFile::modeRead)==FALSE) return;
			// 파일 정보 저장
			CArchive ar(&f,CArchive::load);
			pDoc->m_pDataManage->GetLineInfoApp()->Serialize(ar);
			ar.Close();
			f.Close();
		}

		if(bResult)
		{
			// 불들어와야 할 항목정의
			pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_GEN_02_BASEINFO)->SetUsedFlag(TRUE);
			pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_GEN_03_HDAN)->SetUsedFlag(TRUE);				
			pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_GEN_05_JIGANGUSENG)->SetUsedFlag(TRUE);		
			pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_GEN_06_JIJUMANGLE)->SetUsedFlag(TRUE);
			pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_GEN_20_SLABJEWON)->SetUsedFlag(TRUE);
			pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_CANTILEVER_SETUP)->SetUsedFlag(TRUE);			
			pDoc->m_pGenInputStd->GetDlgOfIDD(IDD_CALC_SECFORCE_DIV_CALC)->SetUsedFlag(TRUE);
			AfxGetMainWnd()->SendMessage(WM_CHANGE_MENUDLG, bFindOK ? ID_GNINPUT_1 : ID_GNINPUT_0);			
		}		
	}	
}


void CAPlateView::OnExportBrx2007() 
{
	GetDocument()->ExportAAbutPier2007Info();
}

void CAPlateView::OnUpdateExportBrx2007(CCmdUI* pCmdUI) 
{
	CAPlateDoc* pDoc = GetDocument();			
	if( pDoc->m_bBrpFileOpen == 0 )	pCmdUI->Enable(FALSE);
	else							pCmdUI->Enable(TRUE);	
}

void CAPlateView::OnExportAdmanager() 
{
	CAPlateDoc	*pDoc	= GetDocument();
	CMainFrame	*pMFrm	= (CMainFrame*)GetParentOwner();

	CString sPath = ((CAPlateApp*)AfxGetApp())->GetFixedDir();
	
	pDoc->m_pShareWithADManagerStd->m_pView = this;
	pDoc->m_pShareWithADManagerStd->m_bIncludeGroup	= FALSE;
	pDoc->m_pShareWithADManagerStd->PushToADManager(sPath);	
}

void CAPlateView::OnUpdateExportAdmanager(CCmdUI* pCmdUI) 
{
	if(!GetDocument()->m_bBrpFileOpen)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	CDomyunView::OnUpdateButtonADManager(pCmdUI);
}

BOOL CAPlateView::OnToolbarControl(UINT nID, NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	CXTPControls* pControls = tagNMCONTROL->pControl->GetControls();
	
	CXTPControlEdit		*pPage		= (CXTPControlEdit*)pControls->GetAt(0);
	CXTPControlComboBox	*pCategory	= (CXTPControlComboBox*)pControls->GetAt(1);
	
	CAPlateDoc		*pDoc	= GetDocument();
	CMainFrame		*pFrm	= (CMainFrame*)AfxGetMainWnd();
	CAPlateDrawStd	*pDraw	= pDoc->m_pPlateDrawStd;
	
	if(pDraw->m_pDrawPageMng->m_nTotalPageSize == 0) return FALSE;
	
	pFrm->m_bMessageDraw = TRUE;
	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);
	
	CString str = _T("");
	int nSubPage = 0;
	int nBri = 0;
	int nCategory = 0;
	if(nID==ID_EDIT_PAGE)
	{
		nSubPage = atoi(pPage->GetEditText());
		if(nSubPage<1)
		{
			nSubPage = 1;
			pPage->SetEditText(_T("1"));
		}
		if(nSubPage>pDraw->m_pDrawPageMng->m_nTotalPageSize)
		{
			nSubPage = pDraw->m_pDrawPageMng->m_nTotalPageSize;
			str.Format(_T("%d"), nSubPage);
			pPage->SetEditText(str);
		}
		pDraw->m_nCurPage = nSubPage;
		nCategory = pDraw->GetDrawPageMng()->GetIdxOfPage(nSubPage);
		pCategory->SetCurSel(nCategory);
	}
	if (nID==ID_COMBO_GOINDEX)
	{
		nCategory = pCategory->GetCurSel();
		str = pDraw->GetDrawPageMng()->GetPage(nCategory)->GetPartTitle();
		nSubPage = pDraw->GetDrawPageMng()->GetPageByIdx(nCategory);
		pDraw->m_nCurPage = nSubPage;
		str.Format(_T("%d"), nSubPage);
		pPage->SetEditText(str);
	}

	DestroyDialog();
	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());	
	msgDlg.Run();

	AfxGetMainWnd()->SendMessage(WM_PAGE_UPDATE,0,0);
	pFrm->SendMessage(WM_UPDATE_DRAWING_TITLE, pDraw->m_nCurPage);
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	return TRUE;

}

void CAPlateView::OnCalcStation() 
{
	CAPlateDoc	*pDoc	= GetDocument();

	if(!m_dlgCalcStation.GetSafeHwnd())
		m_dlgCalcStation.Create(this, pDoc->m_pDataManage->GetLineInfoApp());
	m_dlgCalcStation.ShowWindow(SW_SHOW);
}

void CAPlateView::OnCalcLineInfo() 
{
	CAPlateDoc	*pDoc	= GetDocument();

	if(!m_dlgCalcLineInfo.GetSafeHwnd())
		m_dlgCalcLineInfo.Create(this, pDoc->m_pDataManage->GetLineInfoApp());
	m_dlgCalcLineInfo.ShowWindow(SW_SHOW);
}

void CAPlateView::OnUpdateCalcStation(CCmdUI* pCmdUI)
{
	CAPlateDoc	*pDoc	= GetDocument();

	if(pDoc->m_pDataManage->GetLineInfoApp()->IsValidLineInuptData())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CAPlateView::OnUpdateCalcLineInfo(CCmdUI* pCmdUI)
{
	CAPlateDoc	*pDoc	= GetDocument();

	if(pDoc->m_pDataManage->GetLineInfoApp()->IsValidLineInuptData())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}