// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_JRIBPOSITION_H__812F9124_1F0F_4152_9E41_BD97F9360F89__INCLUDED_)
#define AFX_JRIBPOSITION_H__812F9124_1F0F_4152_9E41_BD97F9360F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JRibPosition.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJRibPosition dialog

class CJRibPosition : public CGeneralBaseDlg
{
// Construction
public:
	CJRibPosition(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJRibPosition)
	enum { IDD = IDD_GEN_16_JRIBPOSITION };
	CButton m_Radio1;
	CButton m_Radio2;
	CXTFlatEdit	m_Edit;
	CTabCtrl	m_ctlTab;
	CGridCtrl	m_uGrid;	
	CGridCtrl	m_lGrid;	
	BOOL	m_bSameApplySameCol;
	double	m_dDisCenterOnJRib;
	int		m_nLengthType;
	BOOL	m_bSimpleItem;
	double	m_dCheckLength;
	//}}AFX_DATA
	CGridCtrl*		m_pGrid;
	BOOL			m_bUpper;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJRibPosition)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CButton m_btnMoveLeft;
	CButton m_btnMoveRight;

	CButton m_btnUp1;
	CButton m_btnUp2;
	CButton m_btnUp3;
	CButton m_btnDown1;
	CButton m_btnDown2;
	CButton m_btnDown3;

	CButton m_btnAutoSetting;
	CButton m_btnSameAll;
	CButton m_btnSymmetry;

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	BOOL IsValid();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();	

	void SetGridTitle();

	void OnPreInitDialog();

	long GetTabCount() { return 2; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "평면도" : "종단면도"; }	
	//
	void EnableButton();	// 좌우이동 버튼활성화
	BOOL IsData(long row,long col) const;
	void MoveSellData(long nStdRow,long col,BOOL bAdd);
	void MoveSellGirderGroup(long nG, long col,BOOL bAdd);
	void SetGridCellInsertedCol(BOOL bAdd);
	void MoveLeftRight(BOOL bLeft);
	void MoveLeftRightByValue(long nG,long nValue,long col,BOOL bLeft);
	void SetGridTitleColum(CGridCtrl * pGrid);
	void SetGridTitleSub(CGridCtrl* pGrid,long cols);
	void SetDataInitGir(long nG, BOOL bUpper);
	void SetDataDefaultUpper(long nG, long nDefaultType);
	void SetDataDefaultLower(long nG, long nDefaultType);	
	double GetStationMatrix(CGridCtrl *pGrid, long nRow, long nCol);	
	void GetBxByJRibUpperPos(long nG, CPtrArray& BxArr, double dFactor) const;
	void SetMatrixByStation(CGridCtrl *pGrid, long nRow, long nCol, double sta);
	long GetRecommendLowerCols(long nG);
	BOOL GetRecommendLowerContinue(long nG,long nCurCol);
	long GetRecommendCol(long nG,long col);
	void JRibAutoSymmetry(long nG);	

	//수평보강재 교번구간 고려한 자동 설정 관련...
	void SetDataAutoSettingUpper(long nG);
	void SetDataAutoSettingLower(long nG);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJRibPosition)	
	virtual BOOL OnInitDialog();
	afx_msg void OnRowColChangeLowerRibdetail();
	afx_msg void OnRowColChangeUpperRibdetail();
	afx_msg void OnButtonMoveleft();
	afx_msg void OnButtonMoveright();
	afx_msg void OnJribBridgelen();
	afx_msg void OnJribGirderlen();
	afx_msg void OnCheckSameapply();
	afx_msg void OnJribArrangeThisgirder();
	afx_msg void OnJribAutoSymmetry();
	afx_msg void OnJribItemtype();
	afx_msg void OnJribdetailRecommendLower1();
	afx_msg void OnJribdetailRecommendLower2();
	afx_msg void OnJribdetailRecommendLower3();
	afx_msg void OnJribdetailRecommendUpper1();
	afx_msg void OnJribdetailRecommendUpper2();
	afx_msg void OnJribdetailRecommendUpper3();
	afx_msg void OnWhstiffAutosetting();
	//}}AFX_MSG
	void SetGridDataByChangeLengthType(CGridCtrl *pGrid, int nOldType, double dOldDisCenter);
	afx_msg void OnKillfocusJribDistanceCenter(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataUpper(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataLower(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedUpper(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedLower(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);	
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);	

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JRIBPOSITION_H__812F9124_1F0F_4152_9E41_BD97F9360F89__INCLUDED_)
