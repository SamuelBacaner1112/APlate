// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_VSTIFFPOSITIONDLG_H__06CFF501_3741_4DB4_90FA_8B58A548474C__INCLUDED_)
#define AFX_VSTIFFPOSITIONDLG_H__06CFF501_3741_4DB4_90FA_8B58A548474C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VStiffPositionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVStiffPositionDlg dialog

class CVStiffPositionDlg : public CGeneralBaseDlg
{
// Construction
public:
	CVStiffPositionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVStiffPositionDlg();
// Dialog Data
	//{{AFX_DATA(CVStiffPositionDlg)
	enum { IDD = IDD_GEN_14_VSTIFFPOSITION };
	CButton	m_Radio1;
	CButton	m_Radio2;
	CXTFlatEdit	m_Edit1;
	CXTFlatEdit	m_Edit2;
	CGridCtrl m_Grid;
	double	m_dBaseDist;
	double	m_dDisCenterOnVst;
	BOOL	m_bSameApplySameCol;	
	int		m_nLengthType;
	BOOL	m_bInputAngleVStiff;
	BOOL	m_bCheckNotInstall;
	//}}AFX_DATA
	CButton	m_btnSameAll;
	CButton	m_btnSymetry;
	CButton	m_btnSetting1;
	CButton	m_btnSetting2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVStiffPositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
 
private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	long GetMaxVStiffsuForGridsu() const;
	void SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter);
	void SetGridByBaseLen(long nSetType);
	void SetMatrixByStation(long nRow, long nCol, double sta);
	void SetSellAttrUnitCross();
	double GetStationMatrix(long nRow, long nCol);
	void SetGridAttrUnit();
	BOOL IsCellAttrFix(long row,long col) const;
	void SetDataSaveGrid();
	
	void SetGridTitle();
	void SetDataBx();

	void DataInitGridData();

	long GetColMatchGirder(long nCurRow, long nCurCol, long nTarRow);
	void SetGridCellInsertedCol(long row,long col);	
	void SetOneStepSel(long row, long col, double dDifDis, double dPrevDis);
	void EnableCheckInstall(BOOL bEnable);
	void VStiffAutoSymmetry(long nG);
	void DataSaveGrid();
// Implementation
protected:
	CPtrArray	m_bSellArr;	// CDWordArray
	BOOL	m_bRowColChange;

	// Generated message map functions
	//{{AFX_MSG(CVStiffPositionDlg)
	afx_msg void OnButtonRecommend();
	afx_msg void OnCheckSametype();
	afx_msg void OnVstiffArrangeThisgirder();
	afx_msg void OnVstiffBridgelen();
	afx_msg void OnVstiffGirderlen();
	afx_msg void OnVstiffInputangle();
	afx_msg void OnVstiffSetting1();
	afx_msg void OnVstiffSetting2();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetDataSymmetry();
	afx_msg void OnCheckNotinstall();
	//}}AFX_MSG
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VSTIFFPOSITIONDLG_H__06CFF501_3741_4DB4_90FA_8B58A548474C__INCLUDED_)
