// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BMOUTSHEET_H__B8F58BC8_A595_48BC_AC00_6EAB163FB44A__INCLUDED_)
#define AFX_BMOUTSHEET_H__B8F58BC8_A595_48BC_AC00_6EAB163FB44A__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMOutSheet.h : header file
// 
 
  
class CDataManage;
class CBMOutSangPage;
class CXLCell;
 
 
/////////////////////////////////////////////////////////////////////////////
// CBMOutSheet
class AFX_EXT_CLASS CBMOutSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CBMOutSheet)

// Construction
public:
	CBMOutSheet();
	virtual ~CBMOutSheet();

// Attributes
public:	
	CDataManage		*m_pDataManage;
	CFont			m_SheetFont;
	CXTPButton		m_MakeButton;
	CXTPButton		m_SaveButton;
	CXTPButton		m_PrintButton;
	CMenu			m_MenuMakeButton;
	CButton			m_CloseButton;

	CProgressCtrl	m_Progress;
	CStatic			m_ProgressText;
	CStatic			m_CountText;
	CStatic			m_WeightText;
	CStatic			m_UnitWeightText;

	// 강제집계표 제작용   
	CBMOutSangPage*	m_pBMOutSangPage;				// [01]기본도의 강재집계표
	CBMOutSangPage*	m_pBMOutStandardSangPage;		// [02]표준집계표
	CBMOutSangPage*	m_pBMOutStandardShopPage;		// [03]표준집계표(제작용)
	CBMOutSangPage*	m_pBMOutStandardShopType2Page;	// [03]표준집계표(제작용)
	CBMOutSangPage*	m_pBMOutThickPage;				// [04]강종별 두께별집계표
	CBMOutSangPage*	m_pBMOutThickSangPage;			// [04]강종별 두께별집계표
	CBMOutSangPage*	m_pBMOutBujeMarkSangPage;		// [05]부재마크별 상세집계표
	CBMOutSangPage*	m_pBMOutBujeMarkTotalPage;		// [06]부재마크별 총괄집계표
	CBMOutSangPage*	m_pBMOutBujeMarkDeductPage;		// [07]부재마크별 공제집계표
	CBMOutSangPage*	m_pBMOutCarPage;				// [08]차량적재 상세집계표
	CBMOutSangPage*	m_pBMOutCarTotalPage;			// [09]차량적재 총괄집계표
	CBMOutSangPage*	m_pBMOutGrandAssemSangPage;		// [10]대조립별 상세집계표
	CBMOutSangPage*	m_pBMOutGrandAssemTotalPage;	// [11]대조립별 총괄집계표
	CBMOutSangPage*	m_pBMOutMidAssemSangPage;		// [12]중조립별 상세집계표
	CBMOutSangPage*	m_pBMOutMidAssemTotalPage;		// [13]중조립별 총괄집계표
	CBMOutSangPage*	m_pBMOutAssemSangPage;			// [14]소조립별 상세집계표
	CBMOutSangPage*	m_pBMOutAssemTotalPage;			// [15]소조립별 총괄집계표
	CBMOutSangPage*	m_pBMOutAssemDeductPage;		// [16]소조립별 공제집계표
	CBMOutSangPage*	m_pBMOutAssemFactPage;			// [17]소조립별 공장제작용집계표
	CBMOutSangPage*	m_pBMOutAssemFieldPage;			// [18]소조립별 현장설치용집계표
	CBMOutSangPage*	m_pBMOutSectionCompPage;		// [19]단면별 집계표(기본도정산용)
	CBMOutSangPage*	m_pBMOutBujeCompPage;			// [20]부재별 집계표(기본도정산용)
	CBMOutSangPage*	m_pBMOutBasicCompPage;			// [21]기본도 정산용 집계표

	// 강제집계표 설계용(????)
	CBMOutSangPage*	m_pBMOutSectionTypePage;
	CBMOutSangPage*	m_pBMOutBujePage;
	CBMOutSangPage*	m_pBMOutBujeTotalPage;
	CBMOutSangPage*	m_pBMOutGirderTotalPage;
	CBMOutSangPage*	m_pBMOutBujeKindTotalPage;
	CBMOutSangPage*	m_pBMOutDeductSangPage;
	CBMOutSangPage*	m_pBMOutDeductBujePage;

	BOOL	m_bNoSize;
	long	m_nMakeBMFlag;
	long	m_nARoadFlag;	// 0-AROAD,1-ASHOP,2-AMODEL,3-APIECE
	DIMTYPE	m_nDimType;
	CString	m_sXLFileName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMOutSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CWnd* pParentWnd,long nMakeFlag, long m_nARoadFlag, DIMTYPE nDimType);

	// Generated message map functions
protected:
	void Start(long nMakeType);
	void Finish();

	//{{AFX_MSG(CBMOutSheet)
	afx_msg void OnButtonMakeCreator();
	afx_msg void OnButtonMakeCreatorDropDown();
	afx_msg void OnButtonMakeFileOpenXL();
	afx_msg void OnButtonSaveDropDown();
	afx_msg void OnButtonPrintDropDown();
	afx_msg void OnButtonFileSaveXL();
	afx_msg void OnButtonFileSaveAllXL();
	afx_msg void OnButtonPrintPreview();
	afx_msg void OnButtonPrintThis();
	afx_msg void OnButtonClose();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnProgress(WPARAM wp, LPARAM lp);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMOUTSHEET_H__B8F58BC8_A595_48BC_AC00_6EAB163FB44A__INCLUDED_)
