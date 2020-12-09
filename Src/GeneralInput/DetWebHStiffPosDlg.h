// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETWEBHSTIFFPOSDLG_H__1869F90A_0960_47A3_83B6_16FAE39BB544__INCLUDED_)
#define AFX_DETWEBHSTIFFPOSDLG_H__1869F90A_0960_47A3_83B6_16FAE39BB544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetWebHStiffPosDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetWebHStiffPosDlg dialog

class CDetWebHStiffPosDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetWebHStiffPosDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetWebHStiffPosDlg)
	enum { IDD = IDD_DET_21_WEBHSTIFFPOSITION };
	CGridCtrl	m_Grid;
	int			m_bHstiffDefault;
	BOOL		m_cHStiffDimRound;
	CXTFlatEdit	m_dHStiffTRibApply;
	BOOL		m_bSameApplySameCol;
	CXTFlatEdit	m_dHStiffLengthMin;
	int			m_cHStiffDimRoundValue;
	BOOL		m_cHStiffHeightOffset;
	CXTFlatEdit	m_dHStiffHeight1;
	CXTFlatEdit	m_dHStiffHeight2;
	CXTFlatEdit	m_dHStiffHeight3;
	BOOL		m_bJackupVStiff;
	CXTFlatEdit	m_dHStiffMinLenJackup;
	CXTFlatEdit	m_dDisCenterOnHStiff;
	int			m_nLengthType;
	//}}AFX_DATA
	CButton	m_btnAutoSymmetry;
	CButton	m_btnMoveLeft;
	CButton	m_btnMoveRight;
	CButton	m_btnAutoCalc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetWebHStiffPosDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetGridTitle(long nQtyCols = 0);
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDataBx();

	void ClearHStiffObj();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	long GetTabCount() { return 2; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "평면도" : "종단면도"; }	

	void OnCursorChange(long nRow, long nCol);

	long GetQtyNodeMax();
	void DataInitGir(long nG);
	void SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter);

	void SetHStiffTermAdjust(CWebHStiff *pHs, CWebHStiff *pHsP, CWebHStiff *pHsN, double *pStaEnd);
	void SetHStiffLengthAdjust(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, BOOL bLeft);
	double GetOffsetHStiffOnJackup(CPlateBasicIndex *pBx, BOOL bLeft, CPlateBasicIndex *pBxN);
	double GetOffsetHStiffOnSplice(CPlateBasicIndex *pBx, double dStiffH, BOOL bLeft, double dSlop, BOOL bFront);
	double GetOffsetHStiffOnVStiff(CPlateBasicIndex *pBx, double dStiffH, BOOL bLeft, double dSlop, BOOL bStt);
	double GetOffsetHStiffOnCrossBeam(CPlateBasicIndex *pBx, double dStiffH, BOOL bLeft, double dSlop);
	double GetOffsetHStiffOnSttEnd(CPlateBasicIndex *pBx, double dStiffH);	
	double GetHFactor(long nUpper, long nLower, long nDan);
	double GetSlopHStiff(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, double dPrevH, double dNextH, double dWidth);
	BOOL SetHStiffOneObj(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, BOOL bLeft);
	long GetGridColumnByBx(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxN, long nRow);
	void SetMatrixByStation(long nRow, long nCol, double sta);
	double GetStationMatrix(long nRow, long nCol);
	void SetHStiffObj();
	double GetOffsetHStiffOnJijumStiff(CPlateBasicIndex *pBx, BOOL bLeft, CPlateBasicIndex *pBxN);

	void MoveTerm(long nG,long nArea,BOOL bLeft);
	void MoveUpperDan(long nG,long nArea,BOOL bLeft);
	void MoveLowerDan(long nG,long nArea,BOOL bLeft);
	void MoveWidth(long nG,long nArea,BOOL bLeft);
	void MoveThick(long nG,long nArea,BOOL bLeft);
	void WhstiffAutosymmetry(long nG);

// Implementation
protected:
	long m_Row;
	long m_Col;
	long m_nQtyCols;

	// Generated message map functions
	//{{AFX_MSG(CDetWebHStiffPosDlg)
	afx_msg void OnWebhstiffRadio1();
	afx_msg void OnHstiffDim10();
	afx_msg void OnMoveLeft();
	afx_msg void OnMoveRight();
	afx_msg void OnWebhstiffAll();
	afx_msg void OnWhstiff5mm();
	afx_msg void OnWhstiff10mm();
	afx_msg void OnWhstiffMinlencalc();
	afx_msg void OnSetDataSymmetry();
	afx_msg void OnHstiffHeightoffset();
	afx_msg void OnHstiffGirderlen();
	afx_msg void OnHstiffBridgelen();
	afx_msg void OnWebHstiffRadio2();
	afx_msg void OnButtonRecommend();	
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKillfocusHstiffTribapply();
	afx_msg void OnKillfocusHstiffDistanceCenter();	
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETWEBHSTIFFPOSDLG_H__1869F90A_0960_47A3_83B6_16FAE39BB544__INCLUDED_)
