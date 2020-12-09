// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETCROSSBEAMTYPEDLG_H__761E76B2_348A_4E7C_BDE9_1D7DF6935680__INCLUDED_)
#define AFX_DETCROSSBEAMTYPEDLG_H__761E76B2_348A_4E7C_BDE9_1D7DF6935680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetCrossBeamTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetCrossBeamTypeDlg dialog
#include "SubCrossHeightRistDlg.h"
#include "CrossBeamSubInputDlg.h"

class CDetCrossBeamTypeDlg : public CGeneralBaseDlg
{
// Construction
public:	
	CDetCrossBeamTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data  
	//{{AFX_DATA(CDetCrossBeamTypeDlg)
	enum { IDD = IDD_DET_13_CROSSBEAM_TYPE };
	CButton	m_bApplyCalc;
	CXTFlatComboBox	m_comboSteelMat;
	CXTFlatComboBox	m_comboSteelType;
	CXTFlatComboBox	m_ctlPosCombo;
	CXTFlatComboBox	m_cmbCurType;
	CGridCtrl	m_GridN;
	CGridCtrl	m_Grid;
	int		m_nCurType;
	int		m_nCurLevelType;
	BOOL	m_bTotalView;
	//}}AFX_DATA
	long m_nPos;
	long m_nCurPos;	
	CBitmap	m_BitMap;

	CButton m_btnDesignCheck;
	CButton m_btnAllCrossBeam;
	CButton m_btnSameType;
	CButton m_btnAllGirder;
	CButton m_btnAllCrossBeamBasic;
	CButton m_btnJijum;
	CButton m_btnSetRatio;
	CButton m_btnAllInput;

	CPlateCrossBeam* m_pCrossBeamCurrent;
	CSubCrossHeightRistDlg	m_SubCrossDlg;
	CCrossBeamSubInputDlg	m_CrossBeamSubInput;
	void ShowCrossBeamSubInput(CWnd *pDlgParent, CWnd *pWnd = NULL);
	void HideCrossBeamSubInput();
	long GetCrossBeamPosition(CPlateBasicIndex	*pBx);

	enum {POS_STT_END, POS_CEN_JIJUM, POS_GENERAL};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetCrossBeamTypeDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsValid();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	COLORREF COLOR_ADDFIX;
	void OnPreInitDialog();
	long GetRowSize();
	long GetColsSize();
	int GetFixRowAtGridN(const CString& sTitle);

private:	
	long m_nG;	    // 현재 거더 Index
	//overiding
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
	void SetVStiffsu();
	void SetVStiffsuByGirder(long nGir);
	void SetVStiffsuBySection(CPlateBasicIndex* pBx);
	void SetVStiffsuByBx(CPlateBasicIndex* pBx);
	void SetGridTitle();
	void SetDataAllGirderSame();
	void SetDataAllSame();
	long GetMaxVStiffNum(long nPos);
	long GetMaxHStiffNum(long nPos);
	long GetMaxAnchorNum(long nPos);
	void SetBoltInterver();
	void SetDataVStiffJewon(CPlateBasicIndex *pBx);
	//data init
	void SetInitPositionCombo();
	void SetInitBoltCombo();
	void SetInitAnchorCombo();
	void SetInitVStiffCombo();
	
	void SetDataInitGridN();
	void SetDataInitGridNCommon(long &nRow);
	void SetDataInitGridNSlabAngker(long &nRow);
	void SetDataInitGridNVStiff(long &nRow);
	void SetDataInitGridNHStiff(long &nRow);
	void SetDataInitGridNBoltConnect(long &nRow);
	void SetDataInitGridNBraketBogangpan(long &nRow);
	void SetDataInitGridNBraketFlangeUpper(long &nRow);
	void SetDataInitGridNBraketFlangeLower(long &nRow);
	void SetDataInitGridNShareConnectPan(long &nRow);
	void SetDataInitGridNMomentConnectPan(long &nRow);
	void SetDataSaveGridN();
	void SetDataSaveGridNCommon();
	void SetDataSaveGridNSlabAngker();
	void SetDataSaveGridNVStiff();
	void SetDataSaveGridNHStiff();
	void SetDataSaveGridNBoltConnect();
	void SetDataSaveGridNBraketBogangpan();
	void SetDataSaveGridNBraketFlangeUpper();
	void SetDataSaveGridNBraketFlangeLower();
	void SetDataSaveGridNShareConnectPan();
	void SetDataSaveGridNMomentConnectPan();
	void SetHunchTopConnect(long nG, long nPos);
	void SetSameLevel(CPlateCrossBeam *pCCur, CPlateCrossBeam *pCSec);

	void SetInitGita();
	void SetDataInit();
	void SetPIButtonText();
	//data save
	void SetDataSave();
	void SetDataSaveSubVBracing(CVBracing* pVr, long nG);
	//data default	
	void SetDataDefault();
	void SetDataDefaultSub(long nG, long nPos);
	void SetDataDefaultSubCrossBeam(long nG, long nPos);	
	//drawing
	
	long GetTabCount() { return 2; }
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "입력" : "횡단면도"; }

	void SetCrossBeamCurrent();
	void SetGridFocusCell(long nRow, long nCol);
	void SetCellReadOnly(CGridCtrl& Grid, long nRow, long nCol, BOOL bReadOnly=FALSE);
	void SetSteelTypeCombo();
	void SetSteelMaterialCombo();
	void SetDataSaveSteelMaterial();

	// Grid
	void SetGrid2Titel();
	void SetGrid2DataInit();
	void SetGrid2DataSave();
	long GetGrid2longValue(CString szValue);
	BOOL GetGrid2BoolValue(CString szValue);
	void Grid2RecommendBracketHeight();
	void Grid2RecommendMoment();
	void Grid2RecommendConnectionPlate();
	void Grid2RecommendHunchTopConnect();

public:
	void OnRistRecommendOnRatio(BOOL bAllCross, double dDan, double dGen, double dCenter,CPlateCrossBeam *pC = NULL);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetCrossBeamTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDialogNext();
	afx_msg void OnDialogPrev();		
	afx_msg void OnButtonRecommend();
	afx_msg void OnATypeInputcomplete();
	afx_msg void OnApplyAllGirderSameApplyButton();
	afx_msg void OnRadioLRHeightEqual();
	afx_msg void OnRadioLRHeightHorz();
	afx_msg void OnSelchangeCurPos();
	afx_msg void OnCheckTotalview();
	afx_msg void OnDlgUserDesign();
	afx_msg void OnButtonRecommendMoment();
	afx_msg void OnSelchangeCurTypeCombo();
	afx_msg void OnApplyAllcrossbeamSameapplyButton2();
	afx_msg void OnButtonSametype();
	afx_msg void OnButtonJijum();
	afx_msg void OnDestroy();
	afx_msg void OnRistRecommend();
	afx_msg void OnSelchangeComboSteeltype();
	afx_msg void OnSelchangeComboSteelmaterial();
	afx_msg void OnCheckApplyCalc();
	afx_msg void OnButtonAlldefault();
	//}}AFX_MSG
	afx_msg void OnCellChangedDataGridN(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRowGridN(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRowGrid(NMHDR* nmgv, LRESULT*);
	afx_msg long OnClickTotalView(WPARAM wp, LPARAM lp);
	afx_msg long OnCrossBeamSubInput(WPARAM wp, LPARAM lp);
	afx_msg long OnCrossBeamSubInputClose(WPARAM wp, LPARAM lp);
	afx_msg void OnCellButtonClickRecommend(NMHDR* nmgv, LRESULT*);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETCROSSBEAMTYPEDLG_H__761E76B2_348A_4E7C_BDE9_1D7DF6935680__INCLUDED_)
