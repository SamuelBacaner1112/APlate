#if !defined(AFX_FILEPROGRESSDLG_H__606E2E0D_4294_4685_83A3_41484A8F28B3__INCLUDED_)
#define AFX_FILEPROGRESSDLG_H__606E2E0D_4294_4685_83A3_41484A8F28B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileProgressDlg.h : header file
//
#define WM_RUNTHREAD WM_USER+7000
class CCalcModeling;
/////////////////////////////////////////////////////////////////////////////
// CFileProgressDlg dialog

class CFileProgressDlg : public CDialog, public CThreadWork
{
// Construction
public:
	CFileProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileProgressDlg)
	enum { IDD = IDD_FILE_PROGRESS };
	//}}AFX_DATA
	CProgressCtrl	m_ctlProgress;
	long			m_nType;
	CWnd*			m_pParent;			

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT	DoWork();

	// Generated message map functions
	//{{AFX_MSG(CFileProgressDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEPROGRESSDLG_H__606E2E0D_4294_4685_83A3_41484A8F28B3__INCLUDED_)
