// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETFIELDJOINDLG_H__23644543_C47F_47E1_A873_AFC27B074793__INCLUDED_)
#define AFX_DETFIELDJOINDLG_H__23644543_C47F_47E1_A873_AFC27B074793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetFieldJoinDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDetFieldJoinDlg dialog

#define	SP_ROWS	8
#define SP_COLS	12
#define	FU_ROW	0
#define	FL_ROW	2
#define	WEB_ROW	4
#define	MO_ROW	6	

class CPlateSplice;
class CDetFieldJoinDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetFieldJoinDlg(CWnd* pParent = NULL);   // standard constructor
	long GetTabCount();
	CString GetTabTitle(long nIndex);
	void SetFocusItem(CString szTitle);
	void SetGridFocusCell(long nRow, long nCol);

// Dialog Data
	//{{AFX_DATA(CDetFieldJoinDlg)
	enum { IDD = IDD_DET_12_FIELDJOIN };
	CGridCtrl m_Grid;
	CXTFlatComboBox	m_ComboBox2;
	CXTFlatComboBox	m_CurrSplice;
	CString		m_ZzsBoltSize;
	BOOL		m_bSpliceSimpleType;
	BOOL	m_bTotalView;
	double	m_dPermissrate;
	BOOL	m_bAutoCalcAllG;
	//}}AFX_DATA
	CButton m_btnBoltAutoLength;
	CButton m_btnBoltAutoLengthAll;
	CButton m_btnAllTension;
	CButton m_btnAllCompress;
	CButton m_btnAllMiddle;
	CButton m_btnToptoBottom;
	CButton m_btnSameType;
	CButton m_btnAllGirder;
	CButton m_btnAllSP;
	CButton m_btnDesignCheck;
	CButton m_btnAutoConnect;
	CButton m_btnAllInputSP;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetFieldJoinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	CProgressCtrl m_ctlProgress;
	void OnPreInitDialog();	
	//overiding
	BOOL IsFocusItem(CString szTitle);
	
	void SetDataDefault();
	
	void SetDataDefaultSplieceUpper(long nG, long nSp);
	void SetDataDefaultSplieceLower(long nG, long nSp);
	void SetDataDefaultSplieceWeb(long nG, long nSp);
	void SetDataDefaultLengthBolt(long nG, CPlateSplice* pSp) const;

	void SetDataInitSub(long nSp);
	void SetDataInitSubGirder(long nG, long nSp);
	void SetDataSave();
	void SetDataSaveSub(long nG, long nSp);
	void SetGridTitle(long nSp);
	void ResetGrid();
	void SetDlgTitle();

	void SetInitSpliceCombo();
	void SetInitBoltCombo();	// Bolt Material, Bolt Size
	void SetSpliceCombo();
	void SetBoltCombo(long nG, long nSp);
	void DataSaveWebToWeb(CPlateBasicIndex *pBx, CPlateBasicIndex* pBxSec);
	void DataSaveUpperToUpper(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec); 
	void DataSaveUpperToLower(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec);
	void DataSaveLowerToUpper(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec); 
	void DataSaveLowerToLower(CPlateBasicIndex *pBx, CPlateBasicIndex *pBxSec);

	void SetSpliceAutoCalc(long nG, long nSp);

	void ChangeSpliceIndex(BOOL bInc);
	BOOL ContinueMsg();
	
public:
	BOOL IsValid();
	void SetDataInit();
	void SetDataDefaultSub(long nG, long nSp);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetFieldJoinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDialogDataSaveAndRedraw();
	afx_msg void OnDialogNext();
	afx_msg void OnDialogPrev();
	afx_msg void OnSelchangeSpliceDlgBoltsize();
	afx_msg void OnSpliceDlgAllgirderapply();
	afx_msg void OnSpliceDlgBoltlengthAutocalc();
	afx_msg void OnSpliceDlgInputfinish();
	afx_msg void OnSpliceDlgAllsplicesame();
	afx_msg void OnSpliceDlgSimplytype();
	afx_msg void OnSelchangeSpliceDlgCurrentsplice();
	afx_msg void OnSpliceDlgBoltlengthAutocalcAll();
	afx_msg void OnSpliceDlgAlltypeapplyWeb();
	afx_msg void OnSpliceDlgAlltypeapplyStrength();
	afx_msg void OnSpliceDlgAlltypeapplyFlange();
	afx_msg void OnSpliceDlgUserdesign();
	afx_msg void OnSpliceDlgAlltypeapplyCompress();
	afx_msg void OnCheckTotalview();
	afx_msg void OnButtonRecommend();
	afx_msg void OnSameApplySameType();
	afx_msg void OnSplicedlgAutocalc();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg void STMakeCursor(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRow   (NMHDR* nmgv, LRESULT*);
	afx_msg void OnCursorChange(long nRow, long nCol);
	afx_msg long OnClickTotalView(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

protected:
	//	inline
	BOOL IsInt(double val) { return ((val == (double)((long)(val))) ? TRUE : FALSE); }
	BOOL IsDimSpace(CDomyun *pDomP, double Term, CString str, long reduce = 0)
	{	return ((Term > (pDomP->Always(270)*(str.GetLength()-reduce))) ? TRUE : FALSE); }

//	Variable
public:
	double	m_DimScale;
	long	m_nG;	// 현재 거더 Index
	long	m_nSp;	// 현재 거더의 Splice Index
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETFIELDJOINDLG_H__23644543_C47F_47E1_A873_AFC27B074793__INCLUDED_)
