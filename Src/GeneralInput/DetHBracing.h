// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETHBRACING_H__4B7332D4_582F_4B14_88E6_B5FE96B95386__INCLUDED_)
#define AFX_DETHBRACING_H__4B7332D4_582F_4B14_88E6_B5FE96B95386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetHBracing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetHBracingDlg dialog
 // 수평브레이싱 상세 
class CDetHBracingDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetHBracingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data  
	//{{AFX_DATA(CDetHBracingDlg)
	enum { IDD = IDD_DET_18_HORIBRACING };
	CXTFlatComboBox	m_ctlPosCombo;
	CXTFlatComboBox	m_ctlBoltSizeCombo;
	CGridCtrl	m_Grid;
	CString		m_ZzsBoltSize;
	int		m_nCurPos;
	BOOL	m_bTotalView;
	long	m_nPos;
	CComboBox	m_comboSteelMat;
	CComboBox	m_comboSteelType;
	//}}AFX_DATA
	CButton	m_btnAutoPlate;
	CButton	m_btnBoltLength;
	CButton	m_btnAllAutoPlate;
	CButton	m_btnAllGirder;
	CButton	m_btnAllHBracing;
	CButton	m_btnAllHBracingBasic;
	CButton	m_btnAllHBracingInput;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetHBracingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void OnPreInitDialog();
// Implementation
protected:
	BOOL IsFocusItem(CString szTitle);
	void SetFocusItem(CString szTitle);

	long m_nG;	    // 현재 거더 Index
	void SetInitPositionCombo();
	void SetInitBoltCombo();
	void SetInitGita();
	void SetDataSaveSub(long nG, long nPos);
	void ChangePosIndex(BOOL bInc);
	void SetBoltCombo(long nG, long nPos);
	void OnCursorChange(long nRow, long nCol);
	void SetDataAllGirderSame();
	void SetDataAllSame();
//	void SetDataSpliceCalc(long nG, long nPos);
	void SetDataCalcHBracing(CPlateBasicIndex *pBx);

	void SetDataInit();
	void SetDataInitSub(long nPos);
	void SetDataDefault();
	void SetDataDefaultSub(long nG, long nPos, BOOL bInputStandard=FALSE);
	void SetDataSave();
	void SetGridTitle();	
	BOOL IsValid();
	void SetStandardInput();

	long GetTabCount() { return 2; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "입력" : "횡단면도"; }

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	// Generated message map functions
	//{{AFX_MSG(CDetHBracingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDialogNext();
	afx_msg void OnDialogPrev();		
	afx_msg void OnHBracingInputcomplete();
	afx_msg void OnAutoBoltlen();
	afx_msg void OnButtonRecommend();
	afx_msg void OnSelChangeBoltsizeCombo();
	afx_msg void OnSelchangeCurPos();
	afx_msg void OnApplyAllGirderSameApply();
	afx_msg void OnAllSameApply();
	afx_msg void OnCheckTotalview();
	afx_msg void OnButtonAutoSpliceCalc();
	afx_msg void OnButtonAutocalcSpliceAll();
	afx_msg void OnButtonInputBasictype();
	//}}AFX_MSG
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRow   (NMHDR* nmgv, LRESULT*);
	afx_msg long OnClickTotalView(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETHBRACING_H__4B7332D4_582F_4B14_88E6_B5FE96B95386__INCLUDED_)
