// APlateOutBMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "APlateOutBM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutBMDlg dialog


CAPlateOutBMDlg::CAPlateOutBMDlg(CAPlateOutBMStd *pBMStd, CWnd* pParent /*=NULL*/)
	: CDialog(CAPlateOutBMDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAPlateOutBMDlg)
	m_pBMStd	= pBMStd;
	m_szProgressText = _T("");
	//}}AFX_DATA_INIT
}


void CAPlateOutBMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAPlateOutBMDlg)
	DDX_Control(pDX, IDC_TREE_BMOUT, m_ctrlTreeBMOut);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Text(pDX, IDC_STATIC_PROGRESS, m_szProgressText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAPlateOutBMDlg, CDialog)
	//{{AFX_MSG_MAP(CAPlateOutBMDlg)
	ON_BN_CLICKED(IDC_BUTTON_BMOUT, OnButtonBmout)
	ON_BN_CLICKED(IDC_BUTTON_BM_FILE_OPEN, OnButtonFileOpen)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_ALL_PRINTOUT, OnButtonAllPrintout)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_DOMYUN, OnButtonOpenDomyun)
	ON_BN_CLICKED(IDC_BUTTON_ALL_OUTCANCEL, OnButtonAllOutCancel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_MAKEANALYSIS_BM, OnReceive)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAPlateOutBMDlg message handlers


BOOL CAPlateOutBMDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CXLControl::GetWmfViewer().CreateOrShowWindow(this,FALSE);
	// 열려져 있는 교량파일을 기본으로 한다.	
	CAPlateOutBMBrInfo	*pBrInfo	= m_pBMStd->GetAPlateBMXL()->GetBridgeInfo(0);
	pBrInfo->m_szBridgeName			= m_pBMStd->GetGeneralInputStd()->m_pDoc->GetTitle();
	pBrInfo->m_szBridgePath			= m_pBMStd->GetGeneralInputStd()->m_pDoc->GetPathName();

	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText("출력대기중...");
	m_nTotBridge = 1;
	InitTreeData(TRUE);
	SetDataSave();
	return TRUE;
}

void CAPlateOutBMDlg::OnButtonBmout() 
{		
	CAPlateBMXL			*pAPlateBMXL	= m_pBMStd->GetAPlateBMXL();
	CDocument			*pDoc			= m_pBMStd->GetGeneralInputStd()->m_pDoc;
	
	UpdateData(TRUE);
	SetDataSave();

	CString	csName	= pDoc->GetTitle();
	int		nPos	= 0;
	CString cs		= _T("");

	CXLControl excel;
	double dVersion = excel.GetVersion();

	if(csName.Find('.') >=0 )
	{
		nPos	= csName.Find('.');
		csName	= csName.Left(nPos);
	}

	cs.Format("_수량산출서");
	csName += cs;
	
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");

	CFileDialog FileDlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);
	FileDlg.m_ofn.lpstrTitle = "액셀 파일 저장";
	if(FileDlg.DoModal() != IDOK)
	{
		return;
	}

	CButton *pItem = (CButton*)GetDlgItem(IDC_BUTTON_BMOUT);
	pItem->EnableWindow(FALSE);

	pAPlateBMXL->m_szSaveFilePath	= FileDlg.GetPathName();
	pAPlateBMXL->Begin(this, WM_USER_MAKEANALYSIS_BM);
}

void CAPlateOutBMDlg::OnButtonFileOpen() 
{
	CAPlateBMXL	*pAPlateBMXL	= m_pBMStd->GetAPlateBMXL();
	CString		sSaveFilePath	= _T("");
		
	if(m_pBMStd->m_dXLVersion >= 12.0)
	{
		sSaveFilePath					= pAPlateBMXL->m_szSaveFilePath + _T("x");
		pAPlateBMXL->m_szSaveFilePath	= sSaveFilePath;
	}

	ShellExecute(NULL, "open", pAPlateBMXL->m_szSaveFilePath, NULL,NULL, SW_SHOWNORMAL);
}


LRESULT CAPlateOutBMDlg::OnReceive(WPARAM wp,LPARAM lp)
{
	LPCTSTR test = (LPCTSTR)lp;
	switch( wp )
	{
	case 0 :	m_ctrlProgress.SetPos((int)lp);
		break;
	case 1:		GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText((LPCTSTR)lp);
		break;
	}

	return 0;
}

void CAPlateOutBMDlg::SetDataSave()
{
	CAPlateOutBMBrInfo	*pBrInfo	= m_pBMStd->GetAPlateBMXL()->GetBridgeInfo(0);
	   
	pBrInfo->m_bOutTotSlab			= m_Options.GetOption("TOTAL_SLAB");
	pBrInfo->m_bOutTotRebar			= m_Options.GetOption("TOTAL_REBAR");
	pBrInfo->m_bOutSlab				= m_Options.GetOption("OUT_SLAB");
	pBrInfo->m_bOutRebar			= m_Options.GetOption("OUT_BEBAR");
	pBrInfo->m_bOutCalcFoundation	= m_Options.GetOption("OUT_CALC");
}

BOOL CAPlateOutBMDlg::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CAPlateOutBMDlg::OnButtonAllPrintout() 
{
	m_ctrlTreeBMOut.DeleteAllItems();

	InitTreeData(TRUE);
	SetDataSave();
}

void CAPlateOutBMDlg::OnButtonOpenDomyun() 
{
	CXLControl::GetWmfViewer().CreateOrShowWindow(this,TRUE);	
}

void CAPlateOutBMDlg::InitTreeData(BOOL bCheckAll)
{
	CPlateBridgeApp		*pDB		= m_pBMStd->GetBridgeApp();
	CAPlateOutBMBrInfo	*pBrInfo	= m_pBMStd->GetAPlateBMXL()->GetBridgeInfo(0);

	m_ctrlTreeBMOut.m_pOptions	= &m_Options;
	m_ctrlTreeBMOut.m_pOptions->m_Items.RemoveAll();

	BOOL	bChkTotSlab		= bCheckAll ? TRUE : pBrInfo->m_bOutTotSlab;
	BOOL	bChkTotRebar	= bCheckAll ? TRUE : pBrInfo->m_bOutTotRebar;
	BOOL	bChkSlab		= bCheckAll ? TRUE : pBrInfo->m_bOutSlab;
	BOOL	bChkRebar		= bCheckAll ? TRUE : pBrInfo->m_bOutRebar;
	BOOL	bChkCalcFoundation	= bCheckAll ? TRUE : pBrInfo->m_bOutCalcFoundation;

	CString	szTitle	=	_T("");
	szTitle	= pBrInfo->m_szBridgeName;
	
	m_Options.SetOptionItem(szTitle, IDB_BMITEMS, IDB_BMITEMS);
		m_Options.SetOption		("TOTAL_SLAB",	bChkTotSlab ?			CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem	("TOTAL_SLAB",	"\t 상부총괄집계표",	CCustomOptions::CheckTrue, IDB_CHECKON_BM, IDB_CHECKOFF_BM);			
		m_Options.SetOption		("TOTAL_REBAR",	bChkTotRebar ?			CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem	("TOTAL_REBAR",	"\t 철근총괄집계표",	CCustomOptions::CheckTrue, IDB_CHECKON_BM, IDB_CHECKOFF_BM);
		m_Options.SetOption		("OUT_SLAB",	bChkSlab ?				CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem	("OUT_SLAB",	"\t 상부수량집계표",	CCustomOptions::CheckTrue, IDB_CHECKON_BM, IDB_CHECKOFF_BM);
		m_Options.SetOption		("OUT_BEBAR",	bChkRebar ?				CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem	("OUT_BEBAR",	"\t 상부철근집계표",	CCustomOptions::CheckTrue, IDB_CHECKON_BM, IDB_CHECKOFF_BM);
		m_Options.SetOption		("OUT_CALC",	bChkCalcFoundation ?	CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem	("OUT_CALC",	"\t 산출근거",			CCustomOptions::CheckTrue, IDB_CHECKON_BM, IDB_CHECKOFF_BM);
	m_Options.BeginTrans();
	m_ctrlTreeBMOut.Initialize(NULL);
}

void CAPlateOutBMDlg::OnButtonAllOutCancel() 
{
	CAPlateOutBMBrInfo	*pBrInfo	= m_pBMStd->GetAPlateBMXL()->GetBridgeInfo(0);

	m_ctrlTreeBMOut.DeleteAllItems();

	pBrInfo->m_bOutTotSlab			= FALSE;
	pBrInfo->m_bOutTotRebar			= FALSE;
	pBrInfo->m_bOutSlab				= FALSE;
	pBrInfo->m_bOutRebar			= FALSE;
	pBrInfo->m_bOutCalcFoundation	= FALSE;

	InitTreeData(FALSE);
	SetDataSave();
}

void CAPlateOutBMDlg::OnCancel() 
{
	CAPlateBMXL	*pBMXL	= m_pBMStd->GetAPlateBMXL();

	if(pBMXL->GetWinThread())
	{
		pBMXL->Abort(TRUE);
		return;
	}

	CDialog::OnCancel();
}
