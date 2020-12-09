// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BMOUTSANGPAGE_H__B9DD19FF_189E_498B_B865_7A794FC64ECF__INCLUDED_)
#define AFX_BMOUTSANGPAGE_H__B9DD19FF_189E_498B_B865_7A794FC64ECF__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMOutSangPage.h : header file
//
#include "resource.h"
  
class CBMOutSheet;
class CGridCtrl;
class CXLCell; 
  
/////////////////////////////////////////////////////////////////////////////
// CBMOutSangPage dialog

class AFX_EXT_CLASS CBMOutSangPage : public CPropertyPage,
									 public CThreadWork
{
	DECLARE_DYNCREATE(CBMOutSangPage)

// Construction
public:
	CBMOutSangPage();
	~CBMOutSangPage();


// Attributes
public:
	CBMOutSheet* m_pBMOutSheet;
	CGridCtrl*	m_pGrid;

	// 페이지 구별의 변수들
	long m_nMakeFlag;

// Dialog Data
	//{{AFX_DATA(CBMOutSangPage)
	enum { IDD = IDD_DET_BMSANG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBMOutSangPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetGrid();
	void DrawGrid();
	UINT DoWork();

protected:


	// Generated message map functions
	//{{AFX_MSG(CBMOutSangPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetColorSubTotal(long nRow);
	void InitGridFirst(long nRows, long nCols);
	void InitGridTitle(long nCols, char *Title[]);
	// 제작용
	void SetGridSangPage();					// [01]기본도의 강재집계표
	void SetGridStandardSangPage();			// [02]표준집계표
	void SetGridStandardShopPage();			// [03]표준집계표(제작용)
	void SetGridStandardShopType2Page();	// [03]표준집계표(제작용)
	void SetGridThickPage();				// [04]강종별 두께별집계표
	void SetGridThickSangPage();			// [04]강종별 두께별집계표
	void SetGridBujeMarkSangPage();			// [05]부재마크별 상세집계표
	void SetGridBujeMarkTotalPage();		// [06]부재마크별 총괄집계표
	void SetGridBujeMarkDeductPage();		// [07]부재마크별 공제집계표
	void SetGridCarPage();					// [08]차량적재 상세집계표
	void SetGridCarTotalPage();				// [09]차량적재 총괄집계표
	void SetGridGrandAssemSangPage();		// [10]대조립별 상세집계표
	void SetGridGrandAssemTotalPage();		// [11]대조립별 총괄집계표
	void SetGridMidAssemSangPage();			// [12]중조립별 상세집계표
	void SetGridMidAssemTotalPage();		// [13]중조립별 총괄집계표
	void SetGridAssemSangPage();			// [14]소조립별 상세집계표
	void SetGridAssemTotalPage();			// [15]소조립별 총괄집계표
	void SetGridAssemDeductPage();			// [16]소조립별 공제집계표
	void SetGridAssemFactPage();			// [17]소조립별 공장제작용집계표
	void SetGridAssemFieldPage();			// [18]소조립별 현장설치용집계표
	void SetGridSectionCompPage();			// [19]단면별 집계표(기본도정산용)
	void SetGridBujeCompPage();				// [20]부재별 집계표(기본도정산용)
	void SetGridBasicCompPage();			// [21]기본도 정산용 집계표

	void DrawGridSangPage();				// [01]기본도의 강재집계표
	void DrawGridStandardSangPage();		// [02]표준집계표
	void DrawGridStandardShopPage();		// [03]표준집계표(제작용)
	void DrawGridStandardShopType2Page();	// [03]표준집계표(제작용)
	void DrawGridThickPage();				// [04]강종별 두께별집계표
	void DrawGridThickSangPage();			// [04]강종별 두께별집계표
	void DrawGridBujeMarkSangPage();		// [05]부재마크별 상세집계표
	void DrawGridBujeMarkTotalPage();		// [06]부재마크별 총괄집계표
	void DrawGridBujeMarkDeductPage();		// [07]부재마크별 공제집계표
	void DrawGridCarPage();					// [08]차량적재 상세집계표
	void DrawGridCarTotalPage();			// [09]차량적재 총괄집계표
	void DrawGridGrandAssemSangPage();		// [10]대조립별 상세집계표
	void DrawGridGrandAssemTotalPage();		// [11]대조립별 총괄집계표
	void DrawGridMidAssemSangPage();		// [12]중조립별 상세집계표
	void DrawGridMidAssemTotalPage();		// [13]중조립별 총괄집계표
	void DrawGridAssemSangPage();			// [14]소조립별 상세집계표
	void DrawGridAssemTotalPage();			// [15]소조립별 총괄집계표
	void DrawGridAssemDeductPage();			// [16]소조립별 공제집계표
	void DrawGridAssemFactPage();			// [17]소조립별 공장제작용집계표
	void DrawGridAssemFieldPage();			// [18]소조립별 현장설치용집계표
	void DrawGridSectionCompPage();			// [19]단면별 집계표(기본도정산용)
	void DrawGridBujeCompPage();			// [20]부재별 집계표(기본도정산용)
	void DrawGridBasicCompPage();			// [21]기본도 정산용 집계표


	// 설계용 ////////////////////////
	void SetGridSectionTypePage();
	void SetGridBujePage();
	void SetGridBujeTotalPage();
	void SetGridDeductSangPage();
	void SetGridDeductBujePage();
	void SetGridGirderTotalPage();
	void SetGridBujeKindTotalPage();
	
	//
	void DrawGridSectionTypePage();
	void DrawGridBujePage();
	void DrawGridBujeTotalPage();
	void DrawGridDeductSangPage();
	void DrawGridDeductBujePage();
	void DrawGridGirderTotalPage();
	void DrawGridBujeKindTotalPage();
	

	CString m_sTotalRecord;
	CString m_sTotalWeight;
	CString m_sUnitWeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMOUTSANGPAGE_H__B9DD19FF_189E_498B_B865_7A794FC64ECF__INCLUDED_)
