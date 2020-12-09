// APlateOutWeldChangeDlg.cpp : implementation file
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
// CAPlateOutWeldChangeDlg dialog


CAPlateOutWeldChangeDlg::CAPlateOutWeldChangeDlg(CAPlateOutBMStd *pBMStd, CWnd* pParent /*=NULL*/)
	: CDialog(CAPlateOutWeldChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAPlateOutWeldChangeDlg)
	//}}AFX_DATA_INIT
	m_pBMStd		= pBMStd;
	m_szProgressText= _T("");
}


void CAPlateOutWeldChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAPlateOutWeldChangeDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Text(pDX, IDC_STATIC_PROGRESS, m_szProgressText);
}


BEGIN_MESSAGE_MAP(CAPlateOutWeldChangeDlg, CDialog)
	//{{AFX_MSG_MAP(CAPlateOutWeldChangeDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_WELD_OUT, OnButtonOut)
	ON_BN_CLICKED(IDC_BUTTON_WELD_FILE_OPEN, OnButtonFileOpen)
	ON_MESSAGE(WM_USER_MAKEANALYSIS_WELDCHANGE, OnReceive)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutWeldChangeDlg message handlers


void CAPlateOutWeldChangeDlg::OnButtonOut() 
{
	CAPlateWeldChangeXL		*pAPlateXL	= m_pBMStd->GetAPlateWeldChangeXL();			
	CDocument				*pDoc		= m_pBMStd->GetGeneralInputStd()->m_pDoc;
	CWeldStandard			*pWeldStd	= m_pBMStd->GetDataManage()->GetWeldStandard();
	
	CPtrList				*pList		= &pWeldStd->m_ListSang;
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
	
	cs.Format("_표준제작공수");
	csName += cs;
	
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog FileDlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);


	FileDlg.m_ofn.lpstrTitle = "액셀 파일 저장";
	if(FileDlg.DoModal() != IDOK)
	{
		return;
	}
	
	//	m_ctrlProgress.ShowWindow(SW_SHOW);
	
	CButton *pItem = (CButton*)GetDlgItem(IDC_BUTTON_WELD_OUT);
	pItem->EnableWindow(FALSE);

	pAPlateXL->m_szSaveFilePath= FileDlg.GetPathName();
	pAPlateXL->Begin(this, WM_USER_MAKEANALYSIS_WELDCHANGE);
}

void CAPlateOutWeldChangeDlg::OnButtonFileOpen() 
{
	CAPlateNonDestructionXL	*pAPlateNonDesXL	= m_pBMStd->GetAPlateNonDestructionXL();
	ShellExecute(NULL, "open", pAPlateNonDesXL->m_szSaveFilePath, NULL,NULL, SW_SHOWNORMAL);
}

BOOL CAPlateOutWeldChangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText("출력대기중...");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CAPlateOutWeldChangeDlg::OnReceive(WPARAM wp,LPARAM lp)
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

void CAPlateOutWeldChangeDlg::OnCancel() 
{
	CAPlateWeldChangeXL	*pAPlateXL	= m_pBMStd->GetAPlateWeldChangeXL();
	
	if(pAPlateXL->GetWinThread())
	{
		pAPlateXL->Abort(TRUE);
		return;
	}
	
	CDialog::OnCancel();
}
