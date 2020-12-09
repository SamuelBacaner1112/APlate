// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETSHCONNECTORVPOSDLG_H__F4818198_77BA_4BCC_A5D1_1BC7B46AC9F3__INCLUDED_)
#define AFX_DETSHCONNECTORVPOSDLG_H__F4818198_77BA_4BCC_A5D1_1BC7B46AC9F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetSHConnectorVPosDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetSHConnectorVPosDlg dialog

class CDetSHConnectorVPosDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetSHConnectorVPosDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetSHConnectorVPosDlg)
	enum { IDD = IDD_DET_23_SHEARCONNECTORVPOSITION };
	CGridCtrl	m_Grid;
	CGridCtrl	m_vGrid;
	int			m_nLengthType;
	BOOL		m_bApplyCSplice;
	double		m_StudMinDis;
	double		m_StudMidDis;
	double		m_StudMaxDis;
	BOOL		m_bApplyFact;
	BOOL		m_bSameApplySameCol;
	CXTFlatEdit	m_ctrlEdit1;
	CXTFlatEdit	m_ctrlEdit2;
	CXTFlatEdit	m_ctrlEdit3;
	BOOL	m_bApplyHBeamVStiff;
	int		m_nStudType;
	//}}AFX_DATA
	CButton	m_btnAdvice;
	CButton	m_btnMoveLeft;
	CButton	m_btnMoveRight;
	CButton	m_btnAutoSymmetry;
	CButton	m_btnSameAll;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetSHConnectorVPosDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	long GetRow	 (long nG) {return(nG*2+1);}	//거더번호에 따른 실제 열수
	void SetRow	 (long nG) {m_Grid.SetRows(GetRow(nG));}
	long GetGirder(long nRow) {if(nRow < 1) nRow = 1;	return((nRow-1)/2);	}
	long GetMaxDataV(BOOL bBase);
	void SetGridDataByChangeLengthType(CGridCtrl *pGrid, int nOldType);
	double GetStationStudTerm(CPlateBasicIndex *pBx, long nDir, double &dStaBase);
	void GetStudTermArr(long nG, double dSttSta, double dEndSta, CDPointArray &LenArr);
	double GetStationMatrix(CGridCtrl *pGrid, long nRow, long nCol);
	double GetLengthMatrix(long nRow, long nCol, double sta);
	void GetTermDivideFoward(CDPointArray &LenArr, CDDWordArray &TermArr);
	void GetTermDivideBackward(CDPointArray &LenArr, CDDWordArray &TermArr);
	void GetTermDivideBoth(CDPointArray &LenArr, CDDWordArray &TermArr);		
	BOOL ProcessStudTerm(CPlateBasicIndex *pBxPrev, CPlateBasicIndex *pBxNext, CDPointArray &LenArr);
	void SetText();
	
private:
	void OnPreInitDialog();
	void SetMatrixByStation(CGridCtrl *pGrid, long nRow, long nCol, double dSta);

	void SetDataInitUp();
	void SetDataInitLow();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void DataSaveGirder();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	void OnCursorChange(long nRow, long nCol);

	// Grid 설정
	void SetGridTitle();

	//
	CGridCtrl*	m_pGrid;

	double GetGirderLength(long nG);	
	double GetInputLength(long nG);
	double GetInputMinus(long nG);
	void MoveLeftRight(long nDir);

	//////////////////////////////////////////////////////////////////////////
	void	SetDataDefaultBase();
	void	SetDataDefaultDisSet();
	//////////////////////////////////////////////////////////////////////////
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetSHConnectorVPosDlg)
	afx_msg void OnSconnectPrevmove();
	afx_msg void OnSconnectNextmove();
	afx_msg void OnButtonRecommendUp();
	afx_msg void OnButtonRecommend();
	afx_msg void OnSconnectAllsameapply();
	afx_msg void OnSconnectAutoSymmetry();
	afx_msg void OnStudFactapply();
	afx_msg void OnSameApplyAllGirder();
	afx_msg void OnStudBridgelen();
	afx_msg void OnStudGirderlen();
	afx_msg void OnButtonApply();
	afx_msg void OnStudHbeamVstiff();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioStud();
	afx_msg void OnRadioAnchor();
	afx_msg void OnRadioSquare();
	afx_msg void OnCheckSplicecen();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedDataLower(NMHDR* nmgv, LRESULT*);

	afx_msg void OnCellChangedUpper(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedLower(NMHDR* nmgv, LRESULT*);

	afx_msg void OnColInsertedUpper(NMHDR* nmgv, LRESULT*);
	afx_msg void OnColDeleteUpper(NMHDR* nmgv, LRESULT*);
	afx_msg void OnColDeletedUpper(NMHDR* nmgv, LRESULT*);
	afx_msg void OnColInsertedLower(NMHDR* nmgv, LRESULT*);
	afx_msg void OnColDeleteLower(NMHDR* nmgv, LRESULT*);
	afx_msg void OnColDeletedLower(NMHDR* nmgv, LRESULT*);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETSHCONNECTORVPOSDLG_H__F4818198_77BA_4BCC_A5D1_1BC7B46AC9F3__INCLUDED_)
