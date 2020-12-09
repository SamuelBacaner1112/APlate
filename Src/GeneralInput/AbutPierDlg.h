// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_ABUTPIERDLG_H__B50CBE7C_26B8_4B8B_990C_C71EA81BF4A9__INCLUDED_)
#define AFX_ABUTPIERDLG_H__B50CBE7C_26B8_4B8B_990C_C71EA81BF4A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AbutPierDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
/// CAbutPierDlg dialog
/// ����,������ ������ �Է¹޴´�.
/// �Ϲݵ������� ���� ��������� �ʴ´�.
/////////////////////////////////////////////////////////////////////////////
class CAbutPierDlg : public CGeneralBaseDlg
{
// Construction
public:
	CAbutPierDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAbutPierDlg)
	enum { IDD = IDD_GEN_07_ABUTPIER };
	CTabCtrl	m_tabSelect;///<������ Tab
	CGridCtrl	m_Grid;		///<���̾�α��� �׸���
	//}}AFX_DATA
	long m_nTab;
	long m_nCurPos;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbutPierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL	

private:	
	void SetGridTitle();///<�׸����� ������ �����Ѵ�.
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();	
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);	
	
	void OnCursorChange(long nRow, long nCol);
	void OnPreInitDialog();	
	void OnChangeTab();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAbutPierDlg)	
	afx_msg void OnSelchangeTabAbutpier(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnButtonRecommend();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* nmgv,LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABUTPIERDLG_H__B50CBE7C_26B8_4B8B_990C_C71EA81BF4A9__INCLUDED_)
