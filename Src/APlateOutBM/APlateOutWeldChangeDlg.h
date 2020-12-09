#if !defined(AFX_APLATEOUTWELDCHANGEDLG_H__233ADDAC_A9D7_47A4_9735_F82A28AC3BBA__INCLUDED_)
#define AFX_APLATEOUTWELDCHANGEDLG_H__233ADDAC_A9D7_47A4_9735_F82A28AC3BBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// APlateOutWeldChangeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutWeldChangeDlg dialog

class CAPlateOutBMStd;

class CAPlateOutWeldChangeDlg : public CDialog
{
// Construction
public:
	CAPlateOutWeldChangeDlg(CAPlateOutBMStd *pBMStd, CWnd* pParent = NULL);   // standard constructor
	CAPlateOutBMStd		*m_pBMStd;
	void SetWnd(CWnd* pWnd) { m_pWnd = pWnd; };

// Dialog Data
	//{{AFX_DATA(CAPlateOutWeldChangeDlg)
	enum { IDD = IDD_DIALOG_APLATE_WELD_CHANGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CProgressCtrl	m_ctrlProgress;
	CString			m_szProgressText;
	CWnd*			m_pWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPlateOutWeldChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAPlateOutWeldChangeDlg)
	//}}AFX_MSG
	afx_msg void OnButtonOut();
	afx_msg void OnButtonFileOpen();
	afx_msg LRESULT OnReceive(WPARAM wp, LPARAM lp);

	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLATEOUTWELDCHANGEDLG_H__233ADDAC_A9D7_47A4_9735_F82A28AC3BBA__INCLUDED_)
