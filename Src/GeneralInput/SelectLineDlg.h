// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SELECTLINEDLG_H__35C8635A_3E6C_44D9_828E_72E391B9EE93__INCLUDED_)
#define AFX_SELECTLINEDLG_H__35C8635A_3E6C_44D9_828E_72E391B9EE93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectLineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectLineDlg dialog
class CLineInfoApp;
class CFTextOutDlg;

class CSelectLineDlg : public CGeneralBaseDlg
{
// Construction
public:
	CSelectLineDlg(CWnd* pParent = NULL);   // standard constructor
	~CSelectLineDlg();

// Dialog Data
	//{{AFX_DATA(CSelectLineDlg)
	enum { IDD = IDD_GEN_01_SELECTLINE };
	CStatic	m_stcLineName;
	CEdit	m_edtLineName;
	CEdit	m_edtFilePath;
	CGridCtrl	m_Grid;	
	CString	m_strFilePath;
	CString	m_strLineName;
	CString	m_strStationStt;
	int		m_nLineImport;
	//}}AFX_DATA
	CXTTabCtrl	m_ctlTab;
	CButton	m_btnSymLR;
	CButton	m_btnLineSave;
	CButton	m_btnLineOpen;
	CButton	m_btnInsert;
	CButton	m_btnEQLR;
	CButton	m_btnDelete;
	CButton	m_btnAdd;

	CComboBox	m_ctlBridgeType;
	CComboBox	m_ctlLineSelect;

	BOOL		m_bJongDan;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectLineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioLineimport();
	afx_msg void OnRadioLineinput();
	afx_msg void OnButtonLineopen();
	afx_msg void OnButtonLinesave();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnComboBridgeType();
	afx_msg void OnButtonAddRow();
	afx_msg void OnButtonDelRow();
	afx_msg void OnButtonEqual();
	afx_msg	void OnButtonSymtri();
	afx_msg void OnCheckJonDan();
	DECLARE_MESSAGE_MAP()

	
public:
	
private:
	double m_dVScaleDraw;
	
	void	OnPreInitDialog();
	void	InitTabCtrl();
	void	SetGridTitle();
	void	SetGridTitleLinePlane();
	void	SetGridTitleVertGroundEL();
	void	SetGridTitleVertPlanEL();
	void	SetGridTitleSlope();

	void	SetDataInit();
	void	SetDataInitLinePlane();
	void	SetDataInitVertGroundEL();
	void	SetDataInitVertPlanEL();
	void	SetDataInitSlope();

	void	SetDataDefault();

	void	SetDataSave();
	void	SetDataSaveLinePlane();
	void	SetDataSaveVertPlanEL();
	void	SetDataSaveVertGroundEL();
	void	SetDataSaveSlope();

	void	CheckInput();
	void	SetEnableWindow();
	void	SetLineInfoFile(CString strFilePath);
	
	void	DrawLinePlane(CDomyun* pDom);
	void	DrawVertGroundEL(CDomyun* pDom);
	void	DrawVertPlanEL(CDomyun* pDom);
	void	DrawSlope(CDomyun* pDom, BOOL bLeft);
	void	DrawCursor(CDomyun* pDom, int nRow);

	void	DrawJongGuBaeDo();

	void	SetViewItem(long nTab);	
	void	SetLineName(long nCur);
	CLineInfo* GetCurLineInfo();

	void	SetAddRowLinePlane();
	void	SetAddRowVertPlanEL();
	void	SetAddRowVertGroundEL();
	void	SetAddRowSlope();
	
	void	SetDeleteRowLinePlane();
	void	SetDeleteRowVertPlanEL();
	void	SetDeleteRowVertGroundEL();
	void	SetDeleteRowSlope();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTLINEDLG_H__35C8635A_3E6C_44D9_828E_72E391B9EE93__INCLUDED_)
