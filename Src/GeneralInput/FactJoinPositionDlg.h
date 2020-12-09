// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_FACTJOINPOSITIONDLG_H__79BD21C7_C74E_4798_BCBA_18FDD77053D6__INCLUDED_)
#define AFX_FACTJOINPOSITIONDLG_H__79BD21C7_C74E_4798_BCBA_18FDD77053D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FactJoinPositionDlg.h : header file
//
// 그리드에서 한개 거더가 의미하는 Row의 수

#define UL_CYCLE  5
#define W_CYCLE   4

/////////////////////////////////////////////////////////////////////////////
// CFactJoinPositionDlg dialog
 
class CFactJoinPositionDlg : public CGeneralBaseDlg
{
// Construction
public:
	CFactJoinPositionDlg(CWnd*pParent = NULL);   // standard constructor swap
	~CFactJoinPositionDlg();
// Attributes
protected:	
	int				m_ID;
	long			m_nPreGridColumnCount;
	
public:
	//{{AFX_DATA(CFactJoinPositionDlg)
	enum { IDD = IDD_GEN_19_FACTJOINPOSITION };
	CXTFlatEdit	m_Edit1;
	CXTFlatEdit	m_Edit2;
	CXTFlatEdit	m_Edit3;
	CButton		m_Radio1;
	CButton		m_Radio2;
	int			m_ComboGir;
	BOOL		m_bSameApplySameCol;
	BOOL		m_cFactThickFix;
	CXTFlatEdit	m_dDisOnCenterFactJoin;
	int			m_nLengthType;
	CXTFlatEdit	m_dDisCenterOnFact;
	BOOL		m_bCheckMove;
	CXTFlatEdit	m_dFlangeSameWidth;
	BOOL		m_bCheckSymbol;
	//}}AFX_DATA
	CXTTabCtrl *m_TabWnd; 
	CGridCtrl m_GridU;
	CGridCtrl m_GridL;
	CGridCtrl m_GridW;

	CButton m_btnMoveLeft;
	CButton m_btnMoveRight;
	CButton m_btnMove1;
	CButton m_btnMove2;

	CButton m_btnSymmetry;
	CButton m_btnSameWidth;
	
	CButton m_btnAutoLength;
	CButton m_btnAutoThick;
	CButton m_btnAutoConnect;

	CButton m_btnMaterial;
	CButton m_btnAdvice1;
	CButton m_btnAdvice2;
	CButton m_btnAdvice3;
	CButton m_btnSameAll;

	long nCurMark;


	enum {  UL_LEN ,UL_THICK, UL_WIDTH, UL_SYMBOL, UL_MOVEDIS };
	enum {  W_LEN , W_THICK, W_SYMBOL, W_MOVEDIS };
	BOOL IsActiveWeb(CGridCtrl* pGrid=NULL) const 
	{
		if(pGrid)
			return pGrid==&m_GridW;
		return m_TabWnd && (m_TabWnd->GetCurSel()==2);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFactJoinPositionDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CBitmap		m_BitMap;
	BOOL		m_bBeginDlg;	
	HANDLE		m_hThread;
	BOOL		m_IsAutoThick;
	BOOL		Create(CWnd* pWnd);

	void	OnEndCalc();
	
	void	DataSave(long nMarkBujeFact = -1); // -1 == 모두저장 
	void	DrawInputDomyunView(BOOL bZoomAll = TRUE);

	long	GetTabCount();	
	CString GetTabTitle(long nIdx);

	void	FlangeThickCheck(CPlateFactChain *pFactChain, double dFlSharp);
	void	WebThickCheck(long nG, double dSttSta, double dEndSta);
	long	SelectedGirder(long nFocusCell_Row, long nQ);

protected:
	void	SetDataDefault();
	void	SetDataInit();
	void	SetDataDefaultByBuje(long nMarkBujeFact, long nAdvice, BOOL isRist = FALSE);

	void	SetBxFact(long nMarkBujeFact);

	//////////////////////////////////////////////////////////////////////////////
	void	InitializeFact(long nMarkBujeFact, long nG, CDoubleArray &DArUpperStaTotal, long nAdvice);
	long	GetDirInKyunggan(long nG, double sta);
	BOOL	IsFactSpliceByStation(long nG, double sta, __int64 BxFlag);
	void	CheckWarn();

	////////////////////////////////////////////////////////////////////////////////////
	// 기존 현장이음과 공장이음을 검사하여 중복되지않는 단면사이 구간의 Station을 리턴
	double	GetOptimizeSta(long nG, double sta, long nDir, __int64 BxFlag);

	void	SetDataSymmetryByBuje(long nMarkBujeFact, long nG);
	void	SetFactTerm(long nMarkBujeFact, long nG, double dTerm);
	void	MoveByMomentStress(double dTerm);
	void	MoveLeftRight(long nDir);

	void	SetGridDataByChangeLengthType(CGridCtrl *pGrid, int nOldType, double dOldDisCenter);
	void	SetMatrixByStation(CGridCtrl *pGrid, long nRow, long nCol, double sta);
	double	GetStationMatrix(CGridCtrl *pGrid, long nRow, long nCol);

	BOOL	SetGridTitle(CGridCtrl* pGrid);
	CGridCtrl *GetGridCurrentTab();
	CGridCtrl *GetGridByFactType(long nMarkBujeFact);

	void	SetFactEqual(long nMarkBujeFact);	// 공장이음 동일적용

	long	GetRow		(long nG)	{return	((nG+1)*3+1);		}
	long	GetRowLength(long nG)	{return (GetRow(nG));		}
	long	GetRowThick	(long nG)	{return (GetRow(nG)+1);	}
	long	GetRowTerm	(long nG)	{return (GetRow(nG)+2);	}

	void	InputGrid(long nMarkBujeFact, long nG, CDoubleArray &DArStaTotal, BOOL isRist = FALSE);
	void	DataInitFact(long nMarkBujeFact, long nG);

	BOOL	ContinueMsg();
	void	RowColChangeFactjoinGrid(long nMarkBujeFact, NMHDR* nmgv);


	// Generated message map functions
	//{{AFX_MSG(CFactJoinPositionDlg)
	virtual BOOL OnInitDialog();		
	afx_msg void OnMoveRight();
	afx_msg void OnMoveLeft();
	afx_msg void OnDialogPrev();		
	afx_msg void OnFactjoinAutocalcthick();
	afx_msg void OnFactjoinUsercalc();
	afx_msg void OnFactjoinAutorevision();		
	afx_msg void OnDestroy();
	afx_msg void OnFactjoinChangethick();
	afx_msg void OnSameApplySameCol();	
	afx_msg void OnSetDataDefault();
	afx_msg void OnSetDataDefaultSecond();
	afx_msg void OnSetDataDefaultThird();
	afx_msg void OnDialogDataSaveAndRedraw();	
	afx_msg void OnMoveByMoment();
	afx_msg void OnMoveByStress();	
	afx_msg void OnFactjoinLowerAdvice3();
	afx_msg void OnFactjoinThick();
	afx_msg void OnSetDataSymmetry();
	afx_msg void OnFactjoinAllequal();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnFactjoinBridgelen();
	afx_msg void OnFactjoinGirderlen();	
	afx_msg void OnFactjoinCheckmove();
	afx_msg void OnButtonFlangeWidthSame();
	afx_msg void OnFactjoinChecksymbol();
	afx_msg void OnRistRecommend();
	afx_msg void OnButtonRistRef1();
	afx_msg void OnButtonRistRef2();
	afx_msg void OnButtonRistRef3();	
	afx_msg void OnDefaultMaterial();
	//}}AFX_MSG
	afx_msg void OnKillfocusFactjoinMovedis();
	afx_msg void OnKillfocusFactjoinDistanceCenter();

	afx_msg void OnCellChangedLower(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedUpper(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedWeb(NMHDR* nmgv, LRESULT*);	
	//
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);

	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellCopiedData(NMHDR* nmgv, LRESULT*);

	void SetDataSave();
	// Tab	
	afx_msg LRESULT OnTabChangePre(WPARAM iTab, LPARAM);
	afx_msg LRESULT OnTabChange(WPARAM iTab, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACTJOINPOSITIONDLG_H__79BD21C7_C74E_4798_BCBA_18FDD77053D6__INCLUDED_)
