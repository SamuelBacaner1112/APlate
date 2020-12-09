// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETVBRACINGTYPEDLG_H__BFECF008_8AE6_4E92_B7CC_F64CBA1B4549__INCLUDED_)
#define AFX_DETVBRACINGTYPEDLG_H__BFECF008_8AE6_4E92_B7CC_F64CBA1B4549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetVBracingTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetVBracingTypeDlg dialog
#define	ROW_COMMON				0		// 공통
#define	ROW_CHORDUP				2		// 상현재 좌우측
#define	ROW_CHORDLO				4		// 하현재 좌우측
#define	ROW_CENT				6		// 상현재 또는 하현재 중앙부
#define	ROW_DIAGONAL			8		// 사재 용접부
#define	ROW_ANCHOR				10		// 슬래브 앵커

class CDetVBracingTypeDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetVBracingTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data  
	//{{AFX_DATA(CDetVBracingTypeDlg)
	enum { IDD = IDD_DET_15_VBRACING_TYPE };	
	CButton	m_bApplyCalc;	
	CButton	m_ctrlCutLevel;
	CButton	m_checkBoltInterver;
	CButton	m_ctrlSameLeftRight;
	CXTFlatEdit		m_ctrlEdit1;
	int m_nCurAnchorSu;
	int m_nCurLevelType;
	BOOL m_bEdgeCutting;
	BOOL m_bCutLevel;
	BOOL m_bSameLeftRight;
	BOOL m_bTotalView;
	CString		m_ZzsBoltSize;
	double		m_dBoltInterver;
	long		m_nPos;
	long		m_nCurPos;
	CGridCtrl	m_Grid;
	//}}AFX_DATA
	CXTFlatComboBox	m_ctlAnchorCombo;  
	CXTFlatComboBox	m_ctlBoltSizeCombo;
	CXTFlatComboBox	m_ctlPosCombo;
	CXTFlatComboBox	m_comboSteelMatUp;
	CXTFlatComboBox	m_comboSteelTypeUp;
	CXTFlatComboBox	m_comboSteelMatLo;
	CXTFlatComboBox	m_comboSteelTypeLo;
	CXTFlatComboBox	m_comboSteelMatDiagonal;
	CXTFlatComboBox	m_comboSteelTypeDiagonal;	
	CButton	m_btnUserDesign;
	CButton	m_btnAutoSplice;
	CButton	m_btnBoltLenght;
	CButton	m_btnAllGirder;
	CButton	m_btnAllBracingBasic;
	CButton	m_btnSamePosition;
	CButton	m_btnAllInput;
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetVBracingTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void OnPreInitDialog();

private:	
	long m_nG;	    // 현재 거더 Index
	CVBracing *	m_pVBracingCurrent;


	//overiding
	BOOL IsValid();
	BOOL IsFocusItem(CString szTitle);
	void SetFocusItem(CString szTitle);
	//event
	void ChangePosIndex(BOOL bInc);
	void SetDlgTitle();
	void OnCursorChange(long nRow, long nCol);
	//data set
	void SetBoltCombo(long nG, long nPos);
	void SetAnchorsu();
	void SetAnchorsuByGirder(long nGir);
	void SetAnchorsuBySection(CPlateBasicIndex* pBx);
	void SetAnchorsuByBx(CPlateBasicIndex* pBx);
	void SetDataAllGirderSame();
	void SetDataAllSame();
	long GetMaxVStiffNum(long nPos);
	long GetMaxHStiffNum(long nPos);
	long GetMaxAnchorNum(long nPos);
	void SetBoltInterver();
	void SetDataSpliceCalc(long nG, long nPos);
	//data init
	void SetInitPositionCombo();
	void SetInitBoltCombo();
	void SetInitAnchorCombo();
	//void SetInitTypeCombo();
	void SetInitGita();
	void SetDataInit();
	//data save
	void SetDataSave();
	//data default	
	void SetDataDefault();
	void SetDataDefaultSub(long nG, long nPos);
	void SetGridInit(long nRows, long nCols);
	//drawing
	void SetUnusedGrid(long nG);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);	
	long GetTabCount() { return 2; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "입력" : "횡단면도"; }
	void SetGridCellType(long nRow, long nCol);

	void SetVBracingCurrent();
	void SetDataVStiffJewon(CPlateBasicIndex *pBx);

	long GetRowSize();
	long GetColSize();
	void SetDataInitCommon();		// 공통
	void SetDataInitChordUp();		// 상현재 좌우측
	void SetDataInitChordLo();		// 하현재 좌우측
	void SetDataInitCent();			// 상현재 또는 하현재 중앙부
	void SetDataInitDiagonal();		// 사재 용접부
	void SetDataInitAnchor();		// 슬래브 앵커

	void SetDataSaveCommon();		// 공통
	void SetDataSaveChordUp();		// 상현재 좌우측
	void SetDataSaveChordLo();		// 하현재 좌우측
	void SetDataSaveCent();			// 상현재 또는 하현재 중앙부
	void SetDataSaveDiagonal();		// 사재 용접부
	void SetDataSaveAnchor();		// 슬래브 앵커
	void SetGridFocusCell(long nRow, long nCol);
	void SetSteelTypeCombo(long nSteelType);
	void SetSteelMaterialCombo(long nComboID);
	void SetDataSaveSteelMaterial();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetVBracingTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDialogNext();
	afx_msg void OnDialogPrev();		
	afx_msg void OnButtonRecommend();
	afx_msg void OnButtonAutoSpliceCalc();
	afx_msg void OnBTypeInputcomplete();
	afx_msg void OnApplyAllGirderSameApplyButton();
	afx_msg void OnAllSameApply();
	afx_msg void OnRadioLRHeightEqual();
	afx_msg void OnRadioLRHeightHorz();
	afx_msg void OnAutoBoltLen();
	afx_msg void OnSelchangeAnchorsu();
	afx_msg void OnSelchangeBoltsize();
	afx_msg void OnSelchangeCurPos();
	afx_msg void OnCheckCutLevel();
	afx_msg void OnCheckTotalview();
	afx_msg void OnCheckLeftRightSame();
	afx_msg void OnDlgUserDesign();
	afx_msg void OnCheckBoltBasicInterver();
	afx_msg void OnChangeEditBoltBasicInterver();
	afx_msg void OnSelchangeComboSteeltypeup();
	afx_msg void OnSelchangeComboSteeltypelo();
	afx_msg void OnSelchangeComboSteeltypediagonal();
	afx_msg void OnSelchangeComboSteelmatup();
	afx_msg void OnSelchangeComboSteelmatlo();
	afx_msg void OnSelchangeComboSteelmatdiagonal();
	afx_msg void OnCheckApplyCalc();
	afx_msg void OnButtonAlldefault();
	//}}AFX_MSG
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);
	afx_msg long OnClickTotalView(WPARAM wp, LPARAM lp);
	afx_msg void STMakeCursor(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETVBRACINGTYPEDLG_H__BFECF008_8AE6_4E92_B7CC_F64CBA1B4549__INCLUDED_)
