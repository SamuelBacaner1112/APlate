// PaintOutSheet.cpp: implementation of the CPaintOutSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "generalinput.h"
#include "../PlateGangje\Gangje.h"
#include "PaintOutAssembly.h"
#include "PaintOutSheet.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CPaintOutSheet
#define WM_PAGE_RESET WM_USER+7
IMPLEMENT_DYNAMIC(CPaintOutSheet, CPropertySheet)

CPaintOutSheet::CPaintOutSheet():CPropertySheet("", NULL, 0)
{
	m_pPaintSang = new CPaintOutAssembly;
	m_pPaintSang->m_pPaintOutSheet = this;
	m_pPaintSang->m_nMakeFlag = PAINTMAKE_SANG;
	m_pPaintSang->m_psp.dwFlags |= PSP_USETITLE;
	m_pPaintSang->m_psp.pszTitle = "상세 집계표";

	m_pPaintBujeSang = new CPaintOutAssembly;
	m_pPaintBujeSang->m_pPaintOutSheet = this;
	m_pPaintBujeSang->m_nMakeFlag = PAINTMAKE_BUJE_SANG;
	m_pPaintBujeSang->m_psp.dwFlags |= PSP_USETITLE;
	m_pPaintBujeSang->m_psp.pszTitle = "부재별 상세";

/*	m_pPaintBujeTotal = new CPaintOutAssembly;
	m_pPaintBujeTotal->m_pPaintOutSheet = this;
	m_pPaintBujeTotal->m_nMakeFlag = PAINTMAKE_BUJE_TOTAL;
	m_pPaintBujeTotal->m_psp.dwFlags |= PSP_USETITLE;
	m_pPaintBujeTotal->m_psp.pszTitle = "부재별 총괄";

	m_pPaintMatAndThick = new CPaintOutAssembly;
	m_pPaintMatAndThick->m_pPaintOutSheet = this;
	m_pPaintMatAndThick->m_nMakeFlag = PAINTMAKE_MAT_THICK;
	m_pPaintMatAndThick->m_psp.dwFlags |= PSP_USETITLE;
	m_pPaintMatAndThick->m_psp.pszTitle = "두께별 상세";

	m_pPaintBoltSang = new CPaintOutAssembly;
	m_pPaintBoltSang->m_pPaintOutSheet = this;
	m_pPaintBoltSang->m_nMakeFlag = PAINTMAKE_BOLT_SANG;
	m_pPaintBoltSang->m_psp.dwFlags |= PSP_USETITLE;
	m_pPaintBoltSang->m_psp.pszTitle = "볼트 상세집계표";

	m_pPaintBoltTotal = new CPaintOutAssembly;
	m_pPaintBoltTotal->m_pPaintOutSheet = this;
	m_pPaintBoltTotal->m_nMakeFlag = PAINTMAKE_BOLT_TOTAL;
	m_pPaintBoltTotal->m_psp.dwFlags |= PSP_USETITLE;
	m_pPaintBoltTotal->m_psp.pszTitle = "볼트 총괄집계표";
*/
	m_pPaintTotalPage = new CPaintOutAssembly;
	m_pPaintTotalPage->m_pPaintOutSheet = this;
	m_pPaintTotalPage->m_nMakeFlag = PAINTMAKE_SUMMARY;
	m_pPaintTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pPaintTotalPage->m_psp.pszTitle = "총괄 집계표";

	m_bNoSize = FALSE;

	m_MenuMakeButton.LoadMenu (IDR_MENUBUT_BMMAKE);	
	m_sXLFileName = _T("");
	m_nMakePaintFlag = 0;
	m_nDimType = DIM_BASIC;
}

 
CPaintOutSheet::~CPaintOutSheet()
{
	if(m_pPaintSang) delete m_pPaintSang;
	if(m_pPaintBujeSang) delete m_pPaintBujeSang;
//	if(m_pPaintBujeTotal) delete m_pPaintBujeTotal;
//	if(m_pPaintMatAndThick) delete m_pPaintMatAndThick;
//	if(m_pPaintBoltSang) delete m_pPaintBoltSang;
//	if(m_pPaintBoltTotal) delete m_pPaintBoltTotal;
	if(m_pPaintTotalPage) delete m_pPaintTotalPage;
}


BEGIN_MESSAGE_MAP(CPaintOutSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPaintOutSheet)
	ON_CBN_DROPDOWN(IDC_BMSHEET_MAKEGANGJE, OnButtonMakeCreatorDropDown)
	ON_CBN_DROPDOWN(IDC_BMSHEET_SAVE, OnButtonSaveDropDown)
	ON_CBN_DROPDOWN(IDC_BMSHEET_PRINT, OnButtonPrintDropDown)

	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BMSHEET_MAKEGANGJE, OnButtonMakeCreator)
	ON_BN_CLICKED(IDC_BMSHEET_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PAINT_PROGRESS,OnProgress)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintOutSheet message handlers
BOOL CPaintOutSheet::Create(CWnd* pParentWnd, long nMakeFlag, long nARoadFlag, DIMTYPE nDimType)
{
	DWORD flag = WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_THICKFRAME
				| WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	m_nMakePaintFlag = nMakeFlag;
	m_nDimType = nDimType;
	if(GetSafeHwnd())// 이미 로드시
		ShowWindow(SW_SHOWNORMAL);
	else	
	{
		if(!GetPageCount())
		{
			if(nMakeFlag & PAINTMAKE_SANG)						AddPage(m_pPaintSang);
			if(nMakeFlag & PAINTMAKE_BUJE_SANG)					AddPage(m_pPaintBujeSang);
//			if(nMakeFlag & PAINTMAKE_BUJE_TOTAL)				AddPage(m_pPaintBujeTotal);
//			if(nMakeFlag & PAINTMAKE_MAT_THICK)					AddPage(m_pPaintMatAndThick);
//			if(nMakeFlag & PAINTMAKE_BOLT_SANG)					AddPage(m_pPaintBoltSang);
//			if(nMakeFlag & PAINTMAKE_BOLT_TOTAL)				AddPage(m_pPaintBoltTotal);
			if(nMakeFlag & PAINTMAKE_SUMMARY)					AddPage(m_pPaintTotalPage);
		}
		CPropertySheet::Create(pParentWnd,flag);
	}

//	CTabCtrl*TabCtrl = GetTabControl();
//	::SetWindowLong(TabCtrl->m_hWnd,GWL_STYLE,::GetWindowLong(TabCtrl->m_hWnd,GWL_STYLE) & ~TCS_MULTILINE);
	
	return TRUE;
}

LRESULT CPaintOutSheet::OnProgress(WPARAM wp, LPARAM lp)
{
	int nValue = (int)wp;

	if(nValue == 0) // progress
	{
		CString sFmt;
		int nPos = (int)lp;
		sFmt.Format("%ld%%",nPos);
		m_Progress.SetPos(nPos);
	}
	else if(nValue == 1)	// PaneText
	{
		LPCTSTR lpstr = (LPCTSTR)(lp);
		m_ProgressText.SetWindowText(lpstr);
	}
	else if(nValue == 4)	// 종료 or 정지
	{
		Finish();
//		AfxGetMainWnd()->SendMessage(WM_USER+44,/*PAGE_BM*/7,0);
	}

	return FALSE;
}


BOOL CPaintOutSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
    SetWindowText("도장집계표");

	return bResult;
}

void CPaintOutSheet::OnSize(UINT nType, int cx, int cy) 
{
	CPropertySheet::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if(m_bNoSize || cx==0 || cy==0) return;

	CRect Rect;
	const long ButtonWidth = 90;
	const long ButtonHeight = 21;
	const long ProgressWidth = 200;
	const long ProgressTxtWidth = 200;
	const CRect ButtonRect(0,0,ButtonWidth,ButtonHeight);
	const CRect ProgressRect(0,0,ProgressWidth,ButtonHeight);
	const CRect ProgressTxtRect(0,0,ProgressTxtWidth,ButtonHeight);

	//  Create
	CRect BRect;
	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(10,cy - ButtonHeight - 2);
	if(m_MakeButton.GetSafeHwnd())
		m_MakeButton.MoveWindow(&BRect);

	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(10 + ButtonWidth + 10,cy - ButtonHeight - 2);
	if(m_SaveButton.GetSafeHwnd())
		m_SaveButton.MoveWindow(&BRect);

	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(10 + ButtonWidth*2 + 20,cy - ButtonHeight - 2);
	if(m_PrintButton.GetSafeHwnd())
		m_PrintButton.MoveWindow(&BRect);

	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(cx - ButtonWidth - 2,cy - ButtonHeight - 2);
	if(m_CloseButton.GetSafeHwnd())
		m_CloseButton.MoveWindow(&BRect);

	// Progress
	BRect.CopyRect(&ProgressRect);
	BRect.OffsetRect(50 +3*ButtonWidth+30,cy - ButtonHeight - 2);
	if(m_Progress.GetSafeHwnd())
		m_Progress.MoveWindow(&BRect);

	// Progress text
	BRect.CopyRect(&ProgressTxtRect);
	BRect.OffsetRect(10 + 3*ButtonWidth+ ProgressWidth + 30,cy - ButtonHeight - 2);
	if(m_ProgressText.GetSafeHwnd())
		m_ProgressText.MoveWindow(&BRect);

	// m_CountText
	BRect.CopyRect(&ProgressRect);
	BRect.OffsetRect(50 +3*ButtonWidth+20, cy - ButtonHeight + 3);
	if(m_CountText.GetSafeHwnd())
		m_CountText.MoveWindow(&BRect);

	CTabCtrl* pTab = GetTabControl();
	if(pTab->GetSafeHwnd())		
		pTab->SetWindowPos(NULL,0,0,cx-13,cy-32,SWP_NOMOVE);		


	if(GetSafeHwnd() && pTab->GetSafeHwnd())
	for(long n = 0; n < GetPageCount(); n++)
	{
		CPaintOutAssembly* pPage = (CPaintOutAssembly*)GetPage(n);
		if(pPage->GetSafeHwnd())
			pPage->PostMessage(WM_SIZE);
	}
}

void CPaintOutSheet::OnDestroy() 
{
	CPropertySheet::OnDestroy();
	
	m_SheetFont.DeleteObject();
}


// 생성버튼
void CPaintOutSheet::OnButtonMakeCreator()
{
	m_pDataManage->m_pPaintCreator->SetWorkValue(0,"",m_nDimType,m_nMakePaintFlag);
	
	Start(IDR_BMMAKE_CREATOR);
	
	m_pDataManage->m_pPaintCreator->Begin(this,WM_PAINT_PROGRESS);
}

void CPaintOutSheet::OnButtonMakeCreatorDropDown()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENUBUT_BMMAKE);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CXTPButton* pButton = DYNAMIC_DOWNCAST(CXTPButton, GetDlgItem(IDC_BMSHEET_MAKEGANGJE));
	ASSERT(pButton);
	
	DWORD dwStyle = pButton->GetPushButtonStyle();
	ASSERT(pButton->IsDropDownStyle());
	
	CXTPWindowRect rect(pButton);
	
	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(),
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left,
		dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom,
		0, m_hWnd, 0);
	
	switch (nCmd)
	{
	case IDR_BMMAKE_CREATOR:
		OnButtonMakeCreator();
		break;
		
	case IDR_BMMAKE_FILEOPEN_XL:
		OnButtonMakeFileOpenXL();
		break;
	}
}

void CPaintOutSheet::OnButtonMakeFileOpenXL()
{
	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	dlg.m_ofn.lpstrTitle = "엑셀 파일열기";
	
	if(IDOK == dlg.DoModal())
	{
		m_sXLFileName = dlg.GetPathName();
		
		m_pDataManage->m_pPaintCreator->SetWorkValue(1, m_sXLFileName, m_nDimType,m_nMakePaintFlag);
		
		Start(IDR_BMMAKE_FILEOPEN_XL);
		
		m_pDataManage->m_pPaintCreator->Begin(this,WM_PAINT_PROGRESS);
	}
}


// 저장 버튼
void CPaintOutSheet::OnButtonSaveDropDown()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_BMOUT_SAVE);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CXTPButton* pButton = DYNAMIC_DOWNCAST(CXTPButton, GetDlgItem(IDC_BMSHEET_SAVE));
	ASSERT(pButton);
	
	DWORD dwStyle = pButton->GetPushButtonStyle();
	ASSERT(pButton->IsDropDownStyle());
	
	CXTPWindowRect rect(pButton);
	
	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(),
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left,
		dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom,
		0, m_hWnd, 0);
	
	switch (nCmd)
	{
	case IDR_BMMAKE_FILESAVE_XL:
		OnButtonFileSaveXL();
		break;
		
	case IDR_BMMAKE_FILESAVEALL_XL:
		OnButtonFileSaveAllXL();
		break;
	case IDR_BMMAKE_OUTFILEOPEN_XL:
		OnButtonFileOutFileOpenXL();
		break;
	}
}

void CPaintOutSheet::OnButtonFileSaveXL()
{
	CPaintOutAssembly* pPage = (CPaintOutAssembly*)GetActivePage();
	CDocument* pDoc = m_pDataManage->m_pPaintCreator->GetTStandard()->m_pDoc;
	CString sFileName = pDoc->GetTitle();
	if( sFileName.Find('.') >= 0 )
	{
		int pos = sFileName.Find('.');
		sFileName = sFileName.Left(pos);
	}
	sFileName += "_도장_";
	sFileName += pPage->m_psp.pszTitle;

	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	dlg.m_ofn.lpstrTitle = "엑셀 파일저장";
	if(IDOK == dlg.DoModal())
	{
		m_sXLFileName = dlg.GetPathName();
		CMsgDlg Dlg(this);
		Dlg.SetCmdType(CMD_SAVE);
		Dlg.AddGridCtrl(pPage->m_pGrid);
		Dlg.AddSaveSheetName(pPage->m_psp.pszTitle);
		Dlg.SetSaveFileName(m_sXLFileName);
		Dlg.Run();					
	}
}

void CPaintOutSheet::OnButtonFileSaveAllXL()
{
	CDocument* pDoc = m_pDataManage->m_pPaintCreator->GetTStandard()->m_pDoc;
	CString sFileName = pDoc->GetTitle()+"_도장";
	if( sFileName.Find('.') >= 0 )
	{
		int pos = sFileName.Find('.');
		sFileName = sFileName.Left(pos);
	}
	sFileName += "_도장집계표";
	
	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	dlg.m_ofn.lpstrTitle = "엑셀 파일저장";
	if(IDOK == dlg.DoModal())
	{
		m_sXLFileName = dlg.GetPathName();
		CMsgDlg Dlg(this);
		Dlg.SetCmdType(CMD_SAVE);
		for(long i=0; i<GetPageCount(); i++)
		{
			CPaintOutAssembly* pPage = (CPaintOutAssembly*)GetPage(i);
			if(!pPage->m_pGrid->GetSafeHwnd())
			{
				SetActivePage(pPage);			
				while(pPage->GetWinThread())
				{
					MSG msg;	
					while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
					{
						AfxGetThread()->PumpMessage();							
					}			
				}
			}
			Dlg.AddGridCtrl(pPage->m_pGrid);
			Dlg.AddSaveSheetName(pPage->m_psp.pszTitle);
		}
		Dlg.SetSaveFileName(m_sXLFileName);
		Dlg.Run();
	}
}

void CPaintOutSheet::OnButtonFileOutFileOpenXL()
{
	CXLControl	XL;
	XL.NewXL();
	double dVersion = XL.GetVersion();
	XL.QuitXL();
	
	CString szPath = m_sXLFileName;
	
	if(dVersion>=12.0)	szPath += "x";
	ShellExecute(NULL, "open", szPath, NULL,NULL, SW_SHOWNORMAL);
}


//출력
void CPaintOutSheet::OnButtonPrintDropDown()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_BMOUT_PRINT);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CXTPButton* pButton = DYNAMIC_DOWNCAST(CXTPButton, GetDlgItem(IDC_BMSHEET_PRINT));
	ASSERT(pButton);
	
	DWORD dwStyle = pButton->GetPushButtonStyle();
	ASSERT(pButton->IsDropDownStyle());
	
	CXTPWindowRect rect(pButton);
	
	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(),
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left,
		dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom,
		0, m_hWnd, 0);
	
	switch (nCmd)
	{
	case IDR_PRINT_PREVIEW:
		OnButtonPrintPreview();
		break;
	case IDR_PRINT_THIS:
		OnButtonPrintThis();
		break;
	}
}

void CPaintOutSheet::OnButtonPrintPreview()
{	
	CPaintOutAssembly* pPage = (CPaintOutAssembly*)GetActivePage();
	pPage->m_pGrid->PrintPreview();
}

void CPaintOutSheet::OnButtonPrintThis()
{
	CPaintOutAssembly* pPage = (CPaintOutAssembly*)GetActivePage();
	pPage->m_pGrid->Print();
}


// 닫기 버튼
void CPaintOutSheet::OnButtonClose()
{
	// 스레드가 진행중인 경우
	if(m_pDataManage->GetPaintStandard()->GetWinThread())	
	{	
		m_pDataManage->GetPaintStandard()->Abort();	
		return;	
	}
	if(m_pDataManage->GetBMStandard()->m_BMList_Sang.GetCount())
		AfxGetMainWnd()->SendMessage(WM_PAGE_RESET,7);

	DestroyWindow();
}

void CPaintOutSheet::Start(long nMakeType)
{
	CRect rect;
	GetClientRect(&rect);
	if(nMakeType==IDR_BMMAKE_CREATOR)
	{
		m_Progress.ShowWindow(SW_SHOW);
		m_ProgressText.MoveWindow(rect.left+560,rect.bottom-17,190,20);
		m_ProgressText.ShowWindow(SW_SHOW);
	}
	if(nMakeType==IDR_BMMAKE_FILEOPEN_XL)
	{
		m_ProgressText.MoveWindow(rect.left+310,rect.bottom-17,440,20);
		m_ProgressText.ShowWindow(SW_SHOW);
		CString str1,str2;
		str1 = m_sXLFileName.Right(m_sXLFileName.GetLength()-m_sXLFileName.ReverseFind('\\')-1);
		str2.Format("%s 파일을 여는중....",str1);
		m_ProgressText.SetWindowText(str2);
	}
	m_CloseButton.SetWindowText("취소(&S)");

	m_MakeButton.EnableWindow(FALSE);
	m_SaveButton.EnableWindow(FALSE);
	m_PrintButton.EnableWindow(FALSE);
	m_CountText.ShowWindow(SW_HIDE);
}

void CPaintOutSheet::Finish()
{
	m_Progress.ShowWindow(SW_HIDE);
	m_ProgressText.ShowWindow(SW_HIDE);
	m_CloseButton.SetWindowText("닫기(&C)");

	m_MakeButton.EnableWindow(TRUE);
	m_SaveButton.EnableWindow(TRUE);
	m_PrintButton.EnableWindow(TRUE);

	for(long n = 0; n < GetPageCount(); n++)
		((CPaintOutAssembly*)GetPage(n))->DrawGrid();
	m_CountText.ShowWindow(SW_SHOW);
	GetActivePage()->OnSetActive();
}

int CPaintOutSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 폰트 생성
	m_SheetFont.CreateFont( 12,
		0,//int nWidth,
		0,//int nEscapement, 폰트의 각( degree, 직각인경우 900)
		0,//int nOrientation,한문자의 각 
		FW_NORMAL,// 폰트의 두께//int nWeight,
		FALSE,//BYTE bItalic, 
		FALSE,//BYTE bUnderline, 
		FALSE,//BYTE cStrikeOut, 
		DEFAULT_CHARSET,//BYTE nCharSet, DEFAULT_CHARSET,ANSI_CHARSET
		OUT_DEVICE_PRECIS,//BYTE nOutPrecision, OUT_DEVICE_PRECIS,OUT_DEFAULT_PRECIS
		CLIP_LH_ANGLES,//BYTE nClipPrecision, CLIP_LH_ANGLES,CLIP_DEFAULT_PRECIS
		DEFAULT_QUALITY,//BYTE nQuality, 
		VARIABLE_PITCH|FF_SWISS,//BYTE nPitchAndFamily, 
		"굴림체"//LPCTSTR lpszFacename 
	);

	///////////////////////////////////////////////////////////////////
	// 동적 버튼 
	DWORD BFlag = WS_CHILD | WS_VISIBLE|BS_PUSHBUTTON;
	CRect rc;
	GetClientRect(&rc);
	rc.bottom += 100;
	m_bNoSize = TRUE;
	SetWindowPos(NULL,0,0,rc.Width(),rc.Height(),SWP_NOMOVE);
	m_bNoSize = FALSE;
	GetClientRect(&rc);

	//  Create
	CRect BRect(0,0,0,0);
	m_MakeButton.Create("생성(&R)",BFlag,BRect,this,IDC_BMSHEET_MAKEGANGJE);
	m_MakeButton.SetFont(&m_SheetFont);
	m_MakeButton.ShowWindow(SW_SHOW);
	m_MakeButton.SetPushButtonStyle(xtpButtonDropDown);

	m_SaveButton.Create("저장(&S)",BFlag,BRect,this,IDC_BMSHEET_SAVE);
	m_SaveButton.SetFont(&m_SheetFont);
	m_SaveButton.ShowWindow(SW_SHOW);
	m_SaveButton.SetPushButtonStyle(xtpButtonDropDown);

	m_PrintButton.Create("인쇄(&P)",BFlag,BRect,this,IDC_BMSHEET_PRINT);
	m_PrintButton.SetFont(&m_SheetFont);
	m_PrintButton.ShowWindow(SW_SHOW);
	m_PrintButton.SetPushButtonStyle(xtpButtonDropDown);

	m_CloseButton.Create("닫기(&C)",BFlag,BRect,this,IDC_BMSHEET_CLOSE);
	m_CloseButton.SetFont(&m_SheetFont);
	m_CloseButton.ShowWindow(SW_SHOW);

	// Progress
	BFlag = WS_CHILD | WS_VISIBLE;
	m_Progress.Create(BFlag,BRect,this,IDC_BMSHEET_PRGRESS);
	m_Progress.ShowWindow(SW_HIDE);//SW_HIDE

	// Progress text
	m_ProgressText.Create("",BFlag,BRect,this,IDC_BMSHEET_PRGRESSTXT);
	m_ProgressText.SetFont(&m_SheetFont);
	m_ProgressText.ShowWindow(SW_HIDE);

	// Count text
	m_CountText.Create("",BFlag,BRect,this,IDC_DET_BMRECCOUNT);
	m_CountText.SetFont(&m_SheetFont);
	m_CountText.ShowWindow(SW_HIDE);
	
	CenterWindow( GetParent() );
	DWORD lp = MAKELONG(rc.Width(),rc.Height());
	
	EnableStackedTabs(FALSE);	
	return 0;
}
