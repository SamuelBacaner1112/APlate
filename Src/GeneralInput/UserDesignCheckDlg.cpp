// UserDesignCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserDesignCheckDlg dialog


CUserDesignCheckDlg::CUserDesignCheckDlg(CXLControl *pXL, CString szTitle, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CUserDesignCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserDesignCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	m_szTitle		= szTitle;
	m_szPathName	= _T("");
	m_pXL			= pXL;
}


void CUserDesignCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserDesignCheckDlg)
	DDX_Control(pDX, IDC_RICHEDIT_CHECK, m_RichEditCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserDesignCheckDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CUserDesignCheckDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILESAVE, OnButtonFilesave)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_FILEOPEN, OnButtonFileopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserDesignCheckDlg message handlers

BOOL CUserDesignCheckDlg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	m_RichEditCheck.SetReadOnly(TRUE);
	m_RichEditCheck.SetSel(0, -1);
	m_RichEditCheck.ReplaceSel("");

	if(m_szTitle!=_T(""))
		SetWindowText(m_szTitle);

	if(m_pXL)
	{
		long	nPageTerm	= 50;
		long	nQtyPage	= (m_nEndRow - m_nSttRow)/nPageTerm + 1;
		long	nSttRow		= m_nSttRow;
		long	nEndRow		= 0;
		
		for(long nPage = 0; nPage < nQtyPage; nPage++)
		{
			nEndRow	= (nPage+1)*nPageTerm;

			if(nEndRow>m_nEndRow)	nEndRow = m_nEndRow;
			m_RichEditCheck.InsetExcelObjectFromClipBoard(m_pXL, nSttRow, m_nSttCol, nEndRow, m_nEndCol, FALSE);

			nSttRow	= nEndRow+1;
		}
	}

	return TRUE;
}

void CUserDesignCheckDlg::SetXLRange(long nSttRow, long nSttCol, long nEndRow, long nEndCol)
{
	m_nSttRow	= nSttRow;
	m_nSttCol	= nSttCol;
	m_nEndRow	= nEndRow;
	m_nEndCol	= nEndCol;
}

void CUserDesignCheckDlg::OnButtonFilesave() 
{
	CString	szName	= _T("단면검토");

	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", szName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	dlg.m_ofn.lpstrTitle = "Excel File Save";
	
	if (dlg.DoModal() == IDOK)
	{
		m_szPathName = dlg.GetPathName();
		m_pXL->SaveAs(m_szPathName);
		m_pXL->QuitXL();
	}
}

void CUserDesignCheckDlg::OnClose() 
{
	m_pXL->QuitXL();
	
	CXTResizeDialog::OnClose();
	
}

void CUserDesignCheckDlg::OnButtonFileopen() 
{
	ShellExecute(NULL, "open", m_szPathName, NULL,NULL, SW_SHOWNORMAL);
}
