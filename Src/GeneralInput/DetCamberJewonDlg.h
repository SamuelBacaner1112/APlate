// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETCAMBERJEWONDLG_H__81795706_3CF7_4A97_8CBC_59045185ADC1__INCLUDED_)
#define AFX_DETCAMBERJEWONDLG_H__81795706_3CF7_4A97_8CBC_59045185ADC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetCamberJewonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetCamberJewonDlg dialog
#define ROW_COUNT_CAMBER 20

class CDetCamberJewonDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetCamberJewonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetCamberJewonDlg)
	enum { IDD = IDD_DET_20_CAMBERJEWON };
	CGridCtrl	m_Grid;
	CXTFlatEdit	m_ctrlEdit1;
	CXTFlatEdit	m_dDistance;
	int			m_nUnitCM;
	int			m_nDistanceType;
	//}}AFX_DATA
	CButton	m_btnCallCalc;
	CButton	m_btnAutoSymmetry;
	CButton	m_btnInitialItem;
	CButton	m_btnInitialStation;
	CButton	m_btnAllSameGirder;
	CButton	m_btnExchangeGirder;
	CButton	m_btnInput;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetCamberJewonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault(long nG);
	void SetDataSave();
	void SetDlgTitle();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	void OnCursorChange(long nRow, long nCol);

	// Grid ¼³Á¤
	void SetGridTitle();

// Implementation
public:
	BOOL CheckSapDataCompare();
	void SapRead();
	void SetCamber();
	static SHGRIDTITLE m_gt[ROW_COUNT_CAMBER];
	
protected:
	void SetMatrixByStation(long nRow, long nCol, double sta);
	void SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter);
	double DBType(double Data,BOOL bSave = FALSE);
	long m_nQtyCamberType;
	long m_nG;

	// Generated message map functions
	//{{AFX_MSG(CDetCamberJewonDlg)
		afx_msg void OnButtonApplyInitialAllItem();
		afx_msg void OnButtonExchangeSymmetryGirder();
		afx_msg void OnButtonApplyIdentityAllGirder();
		afx_msg void OnButtonAutomaticSymmetry();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioUnitCentimeter();
	afx_msg void OnRadioUnitMilimeter();
	afx_msg void OnRadioBasisDistance();
	afx_msg void OnRadioRealDistance();
	afx_msg void OnDialogNext();
	afx_msg void OnDialogPrev();
	afx_msg void OnButtonInputFinish();
	afx_msg void OnButtonAutoCalc();
	afx_msg void OnButtonInitStation();
	//}}AFX_MSG
	afx_msg void OnKillfocusCamberDistanceCenter();
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellCopiedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETCAMBERJEWONDLG_H__81795706_3CF7_4A97_8CBC_59045185ADC1__INCLUDED_)
