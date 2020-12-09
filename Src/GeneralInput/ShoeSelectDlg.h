// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHOESELECTDLG_H__81044BD2_4A67_4202_9B8C_2EECD37854B9__INCLUDED_)
#define AFX_SHOESELECTDLG_H__81044BD2_4A67_4202_9B8C_2EECD37854B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShoeSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShoeSelectDlg dialog
class CGeneralBaseStd;
class CShoeSelectDlg : public CDialog
{
// Construction
public:
	CShoeSelectDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShoeSelectDlg)
	enum { IDD = IDD_DIALOG_SHOESELECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGridCtrl m_Grid;
	CGeneralBaseStd *m_pStd;
	CButton	m_btnReset;
	CButton	m_btnOK;

// Attributes
protected:
	CStringArray 	m_StrShoe;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShoeSelectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CShoeSelectDlg)
	virtual BOOL OnInitDialog();			
	virtual void OnOK();
	afx_msg void OnButtonReset();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCellChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCellDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

// Implementation
protected:
	void	SetGridTitle();
	long	GetMaxShoesu();
	void	DataInit();
	void	DataSave();
	BOOL	GetLRFixShoe(long nG,long k);
	BOOL	GetUDFixShoe(long nG,long k);

	CString GetShoeDirection(long Dir);
	long	GetShoeDirection(BOOL bLRFix,BOOL bUDFix);
	long	GetShoeDirection(CString str);
public:
	void	SetShoeDirection(CString oStr,CString cStr);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOESELECTDLG_H__81044BD2_4A67_4202_9B8C_2EECD37854B9__INCLUDED_)
