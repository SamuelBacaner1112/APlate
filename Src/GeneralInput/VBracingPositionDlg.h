// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_VBRACINGPOSITIONDLG_H__699403B3_02C5_4969_88B2_F6516904F315__INCLUDED_)
#define AFX_VBRACINGPOSITIONDLG_H__699403B3_02C5_4969_88B2_F6516904F315__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VBracingPositionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVBracingPositionDlg dialog

class CVBracingPositionDlg : public CGeneralBaseDlg
{
// Construction
public:
	void SetGridDataByChangeLengthType(int nLengthType, double dDisCenter);
	CVBracingPositionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVBracingPositionDlg)
	enum { IDD = IDD_GEN_13_VBRACINGPOSITION };
	CXTFlatEdit	m_btnBaseDist;
	CXTFlatEdit	m_ctrlEdit1;
	CGridCtrl	m_Grid;
	double		m_dBaseDist;
	BOOL		m_bSameApplySameCol;
	int			m_cCross90;
	double		m_dDisCenterOnDiap;
	int			m_nLengthType;
	//}}AFX_DATA
	CButton	m_btnAllSame;
	CButton	m_btnAutoSet;


private:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVBracingPositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDataBx();	

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	
	void SetGridCellInsertedCol(int nRow, int nCol);
	void SetDataSaveGrid();

	CString GetLineInfo();
	double GetStationMatrix(long nRow, long nCol);
	void SetMatrixByStation(long nRow, long nCol, double sta);
	void SetSelEditByUser(long row,double dDifDis, BOOL m_bNext = FALSE);

	void GetDisArr(long nG, CDDWordArray &dDisArr);
	void GetDisArrMultiJiganFirst(long nG, long nJigan, CDDWordArray &dDisArr);
	void GetDisArrMultiJiganLast(long nG, long nJigan, CDDWordArray &dDisArr);
	void GetDisArrJiganCenter(long nG, long nJigan, CDDWordArray &dDisArr);
	
	void GetDisArrDiap(long nG, CDDWordArray &dDisArr);
	void GetDisArrDiapMultiJiganFirst(long nG, long nJigan, CDDWordArray &dDisArr);
	void GetDisArrDiapMultiJiganLast(long nG, long nJigan, CDDWordArray &dDisArr);
	void GetDisArrDiapJiganCenter(long nG, long nJigan, CDDWordArray &dDisArr);

	void GetDisArrCross(long nG, CDDWordArray &dDisArr);
	void GetDisArrCrossJiganCenter(long nG, long nJigan, CDDWordArray &dDisArr);
	void GetDisArrCrossMultiJiganFirst(long nG, long nJigan, CDDWordArray &dDisArr);
	void GetDisArrCrossMultiJiganLast(long nG, long nJigan, CDDWordArray &dDisArr);

	double GetLengthBaseReal(long nG, long nJigan);
	double GetStationGirderLineByJijum(long nG, long nJijum);

	void SetBxCross();
	void SetBxJackupHeight();
	void SetBxJijumVStiffHeight();
	void SetBxBending();
	void SetBxCrossSub(long nG);
	void SetBxByun();

// Implementation
protected:
	long GetCrossGridSuInBridge();
	long GetCrossGridSuInOneJigan(long nJigan);

	double GetStationOnCRLineStt();
	double GetStationOnCRLineEnd();
	double GetStationOnCRLineByJijum(long nJijum);


	// Generated message map functions
	//{{AFX_MSG(CVBracingPositionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRecommend();
	afx_msg void OnCheckSametype();
	afx_msg void OnButtonGirderSameApply();
	afx_msg void OnCrossGirderlen();
	afx_msg void OnCrossBridgelen();
	//}}AFX_MSG
	afx_msg void OnCellInsertCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VBRACINGPOSITIONDLG_H__699403B3_02C5_4969_88B2_F6516904F315__INCLUDED_)
