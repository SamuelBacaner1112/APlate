// APlateOutNonDestructionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "APlateOutBM.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutNonDestructionDlg dialog


CAPlateOutNonDestructionDlg::CAPlateOutNonDestructionDlg(CAPlateOutBMStd *pBMStd, CWnd* pParent /*=NULL*/)
	: CDialog(CAPlateOutNonDestructionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAPlateOutNonDestructionDlg)
		m_pBMStd	= pBMStd;
		m_szProgressText = _T("");
	//}}AFX_DATA_INIT
}


void CAPlateOutNonDestructionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAPlateOutNonDestructionDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Text(pDX, IDC_STATIC_PROGRESS, m_szProgressText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAPlateOutNonDestructionDlg, CDialog)
	//{{AFX_MSG_MAP(CAPlateOutNonDestructionDlg)
	ON_BN_CLICKED(IDC_BUTTON_NONDES_OUT, OnButtonNonDesOut)
	ON_BN_CLICKED(IDC_BUTTON_NONDES_FILE_OPEN, OnButtonNonDesFileOpen)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_MAKEANALYSIS_NONDES, OnReceive)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutNonDestructionDlg message handlers

void CAPlateOutNonDestructionDlg::OnButtonNonDesOut() 
{
	CAPlateNonDestructionXL	*pAPlateNonDesXL	= m_pBMStd->GetAPlateNonDestructionXL();			
	CDocument				*pDoc				= m_pBMStd->GetGeneralInputStd()->m_pDoc;
	CWeldStandard			*pWeldStd			= m_pBMStd->GetDataManage()->GetWeldStandard();
	
	CPtrList				*pList				= &pWeldStd->m_ListSang;
	CXLControl excel;
	double dVersion = excel.GetVersion();

	if(pList->GetCount() <= 0)
	{
		AfxMessageBox("용접집계가 실행되지 않았습니다.\n용접집계표를 생성하시기 바랍니다.", MB_OK | MB_ICONERROR);
		return;
	}
	
	CString	csName	= pDoc->GetTitle();
	int		nPos	= 0;
	CString cs		= _T("");
	
	if(csName.Find('.') >=0 )
	{
		nPos	= csName.Find('.');
		csName	= csName.Left(nPos);
	}
	
	cs.Format("_비파괴검사서");
	csName += cs;

	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog FileDlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	FileDlg.m_ofn.lpstrTitle = "액셀 파일 저장";
	if(FileDlg.DoModal() != IDOK)
	{
		return;
	}
	
	//	m_ctrlProgress.ShowWindow(SW_SHOW);
	
	CButton *pItem = (CButton*)GetDlgItem(IDC_BUTTON_NONDES_OUT);
	pItem->EnableWindow(FALSE);
	
	pAPlateNonDesXL->m_szSaveFilePath	= FileDlg.GetPathName();
	pAPlateNonDesXL->Begin(this, WM_USER_MAKEANALYSIS_NONDES);
}

void CAPlateOutNonDestructionDlg::OnButtonNonDesFileOpen() 
{
	CAPlateNonDestructionXL	*pAPlateNonDesXL	= m_pBMStd->GetAPlateNonDestructionXL();
	ShellExecute(NULL, "open", pAPlateNonDesXL->m_szSaveFilePath, NULL,NULL, SW_SHOWNORMAL);
}

BOOL CAPlateOutNonDestructionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText("출력대기중...");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CAPlateOutNonDestructionDlg::OnReceive(WPARAM wp,LPARAM lp)
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

void CAPlateOutNonDestructionDlg::OnCancel() 
{
	CAPlateNonDestructionXL	*pAPlateNonDesXL	= m_pBMStd->GetAPlateNonDestructionXL();
	
	if(pAPlateNonDesXL->GetWinThread())
	{
		pAPlateNonDesXL->Abort(TRUE);
		return;
	}
	
	CDialog::OnCancel();
}