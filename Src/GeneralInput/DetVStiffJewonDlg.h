// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETVSTIFFJEWONDLG_H__1D50219B_D532_4B91_A336_71A5C7506A90__INCLUDED_)
#define AFX_DETVSTIFFJEWONDLG_H__1D50219B_D532_4B91_A336_71A5C7506A90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetVStiffJewonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetVStiffJewonDlg dialog

class CDetVStiffJewonDlg : public CGeneralBaseDlg
{
// Construction
public:
	BOOL IsPositiveRange(CPlateBasicIndex *pBx);
	CDetVStiffJewonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetVStiffJewonDlg)
	enum { IDD = IDD_DET_19_VSTIFFJEWON };
	CButton	m_checkArr;
	CButton	m_checkStdInput;
	CXTFlatComboBox	m_ctlComboType;
	CGridCtrl m_Grid;
	int		m_nCmpPos;
	CString	m_szStation;
	BOOL	m_bTotalView;
	int		m_radioArr;
	BOOL	m_cCarryUpper;
	BOOL	m_cCarryLower;
	//}}AFX_DATA
	CButton	m_btnDesignCheck;
	CButton	m_btnAutoCalc;
	CButton	m_btnInputBasicType;
	CButton	m_btnAllGirder;
	CButton	m_btnAllVStiff;
	CButton	m_btnAllVStiffInput;


	void SetComboString(long nG =0);
	void OnPreInitDialog();
	
private:
	BOOL m_bArr;
	long m_nG;

	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDataDefaultDepyoDanmyun();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	long GetTabCount();
	CString GetTabTitle(long nIdx);
	void SetDataAllGirderSame();
	void SetDataAllSame();
	void RecommandData(CVStiff *pVStiff, double dHGir);

	// Grid 설정
	void SetGridTitle();
	void SetStandardInput();
	BOOL IsValid();
	void SetVStiffSetting(long nMode =0);//편측배치 항목 세팅...
	void OnCheckVstiffArrangementSub(BOOL bMode = TRUE);

	void SetControl();

	double m_dLH;
	double m_dRH;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetVStiffJewonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDetVStiffJewonDlg)
	afx_msg void OnDialogNext();
	afx_msg void OnDialogPrev();	
	afx_msg void OnSelchangeComboPosition();
	afx_msg void OnVstiffInputcomplete();
	afx_msg void OnButtonAutocalc();
	afx_msg void OnCheckTotalview();
	afx_msg void OnCheckVstiffArrangement();
	afx_msg void OnButtonInputStandardType();
	afx_msg void OnRadioLeftOnlyVStiff();
	afx_msg void OnRadioRightOnlyVStiff();
	afx_msg void OnRadioSymmetryOnlyVStiff();
	afx_msg void OnCheckDongbaliDown();
	afx_msg void OnCheckDongbaliUp();
	afx_msg void OnButtonDesignCheck();
	afx_msg void OnButtonInputAllgirsame();
	afx_msg void OnButtonInputAllsame();
	afx_msg void OnButtonRecommend();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCursorChange(long nRow, long nCol);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	afx_msg long OnClickTotalView(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETVSTIFFJEWONDLG_H__1D50219B_D532_4B91_A336_71A5C7506A90__INCLUDED_)
