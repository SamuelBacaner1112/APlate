// ResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReDir.h"
#include "ResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog

CResultDlg::CResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResultDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_szBtnText = "닫 기";
	m_pRedirect = new CReDir(this);
}

CResultDlg::~CResultDlg()
{
	delete m_pRedirect;
}

void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultDlg)
	DDX_Control(pDX, IDC_LIST_RESULT, m_ctlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	//{{AFX_MSG_MAP(CResultDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COPYDATA, OnCopyData)
	ON_MESSAGE(WM_STDIO_COMMAND, OnStdioCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultDlg message handlers

void CResultDlg::AddCommand(WPARAM wCommand, LPCSTR lpszCommand)
{
	(CButton*) m_pButton = (CButton*)GetDlgItem(IDOK);
	if(wCommand==WM_CALC_END)
	{
		m_pButton->EnableWindow(TRUE);
		m_pButton->SetFocus();		
	}

	if(wCommand==STDIO_OPEN)
	{
		//m_ctlList.ResetContent();
		m_pButton->EnableWindow(FALSE);
	}

	ASSERT(lpszCommand != NULL);	
    
	m_criticalSection.Lock();
	m_StdioCommand.AddTail(lpszCommand);	
    m_criticalSection.Unlock();		

	//SetWindowText ("Sap2000 Running - " + CString(lpszCommand));

	OnStdioCommand(wCommand, 0);
}

LRESULT CResultDlg::OnStdioCommand(WPARAM wCommand, LPARAM lParam)
{
	CString strCommand;
    m_criticalSection.Lock();
    ASSERT(m_StdioCommand.GetCount() > 0);
    strCommand = m_StdioCommand.RemoveHead();    
	
	switch (wCommand)
	{
		case STDIO_OPEN:
		{
			::SetCurrentDirectory(m_szCurrPath);
			if(!IsWindowVisible())
			{
				CenterWindow();
				ShowWindow(SW_SHOW);
			}

			CString strCmdLine;
			ASSERT(!strCommand.IsEmpty());
			GetCommandLine(AfxGetApp()->m_hInstance, strCmdLine, strCommand);
			if (m_pRedirect->StartChildProcess(strCmdLine, FALSE))
			{
				m_strCmdLine = strCmdLine;				
			}
			break;
		}
		case STDIO_CLOSE:
			{
				if (!strCommand.IsEmpty())
					m_pRedirect->WriteChildStdIn(strCommand);
				else
					m_pRedirect->TerminateChildProcess();			
				AddCommand(WM_CALC_END,"");										
			}			
			break;
		case STDIO_WRITE:
			m_pRedirect->WriteChildStdIn(strCommand);
			break;
	}
	m_criticalSection.Unlock();
	return 0;
}

LRESULT CResultDlg::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
	CString szData = (LPCSTR)pcds->lpData;
	CStringArray strArr;
	//SeperateStr(szData,strArr);

	switch (pcds->dwData)
	{		
		case SET_WINDOW_TEXT:
		{			
			if(szData=="End")			
			{
				long nCount = m_ctlList.GetCount();	
				nCount = m_ctlList.GetCount();
				m_ctlList.InsertString (nCount,"");
				m_ctlList.SetCurSel( nCount );
				m_ctlList.InsertString (nCount+1,"===================================================================================");
				m_ctlList.SetCurSel( nCount+1 );
				m_ctlList.InsertString (nCount+2,"");
				m_ctlList.SetCurSel( nCount+2 );

				CString strCommandNext = m_StrCmdList.GetCount() > 0 ? m_StrCmdList.RemoveHead() : "";  											

				if(m_bError)					
					AfxMessageBox("모델링에 에러가 존재합니다.", MB_ICONSTOP);									
				else if(!strCommandNext.IsEmpty())
				{							
					//SetWindowText ("Sap2000 Running - " + strCommandNext);
					AddCommand(STDIO_OPEN, strCommandNext);
				}
				else
				{
					SetWindowText ("Sap2000 Stop !");
										
					if(m_bWarning)					
						AfxMessageBox("모델링에 경고 메세지가 존재합니다.", MB_ICONINFORMATION);
//					else
//						AfxMessageBox("Sap2000 계산이 완료되었습니다.", MB_ICONINFORMATION);					
				}						
			}			
			break;
		}		
		case STDIO_WRITE_TEXT:
		{			
			long nCount = m_ctlList.GetCount();		
			if(szData.Find("* E R R O R *") != -1)
				m_bError = TRUE;
			else if(szData.Find("* W A R N I N G *") != -1)
				m_bWarning = TRUE;
			
			m_ctlList.InsertString (nCount,szData);
			m_ctlList.SetCurSel( nCount );		
			//SetFocus();
			break;
		}
		case STDERR_WRITE_TEXT:
		{
			long nCount;
			for(long n=0; n<strArr.GetSize(); n++)
			{
				nCount = m_ctlList.GetCount();
				m_ctlList.InsertString (nCount,strArr.GetAt(n));
				m_ctlList.SetCurSel( nCount );								
			}
			//SetFocus();
			break;
		}
	}
	return 0;
}

void CResultDlg::OnOK() 
{	
	m_StrCmdList.RemoveAll();	
	AddCommand(STDIO_CLOSE, "<프로그램 종료>");
	if(m_pMainWnd) 
		m_pMainWnd->SendMessage(WM_CALC_OK,0,0);
	else
		GetParent()->SendMessage(WM_CALC_OK,0,0);
	CDialog::OnOK();
}

BOOL CResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResultDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	GetDlgItem(IDOK)->SetWindowText(m_szBtnText);	
}
