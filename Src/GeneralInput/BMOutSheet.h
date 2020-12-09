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

	// ��������ǥ ���ۿ�   
	CBMOutSangPage*	m_pBMOutSangPage;				// [01]�⺻���� ��������ǥ
	CBMOutSangPage*	m_pBMOutStandardSangPage;		// [02]ǥ������ǥ
	CBMOutSangPage*	m_pBMOutStandardShopPage;		// [03]ǥ������ǥ(���ۿ�)
	CBMOutSangPage*	m_pBMOutStandardShopType2Page;	// [03]ǥ������ǥ(���ۿ�)
	CBMOutSangPage*	m_pBMOutThickPage;				// [04]������ �β�������ǥ
	CBMOutSangPage*	m_pBMOutThickSangPage;			// [04]������ �β�������ǥ
	CBMOutSangPage*	m_pBMOutBujeMarkSangPage;		// [05]���縶ũ�� ������ǥ
	CBMOutSangPage*	m_pBMOutBujeMarkTotalPage;		// [06]���縶ũ�� �Ѱ�����ǥ
	CBMOutSangPage*	m_pBMOutBujeMarkDeductPage;		// [07]���縶ũ�� ��������ǥ
	CBMOutSangPage*	m_pBMOutCarPage;				// [08]�������� ������ǥ
	CBMOutSangPage*	m_pBMOutCarTotalPage;			// [09]�������� �Ѱ�����ǥ
	CBMOutSangPage*	m_pBMOutGrandAssemSangPage;		// [10]�������� ������ǥ
	CBMOutSangPage*	m_pBMOutGrandAssemTotalPage;	// [11]�������� �Ѱ�����ǥ
	CBMOutSangPage*	m_pBMOutMidAssemSangPage;		// [12]�������� ������ǥ
	CBMOutSangPage*	m_pBMOutMidAssemTotalPage;		// [13]�������� �Ѱ�����ǥ
	CBMOutSangPage*	m_pBMOutAssemSangPage;			// [14]�������� ������ǥ
	CBMOutSangPage*	m_pBMOutAssemTotalPage;			// [15]�������� �Ѱ�����ǥ
	CBMOutSangPage*	m_pBMOutAssemDeductPage;		// [16]�������� ��������ǥ
	CBMOutSangPage*	m_pBMOutAssemFactPage;			// [17]�������� �������ۿ�����ǥ
	CBMOutSangPage*	m_pBMOutAssemFieldPage;			// [18]�������� ���弳ġ������ǥ
	CBMOutSangPage*	m_pBMOutSectionCompPage;		// [19]�ܸ麰 ����ǥ(�⺻�������)
	CBMOutSangPage*	m_pBMOutBujeCompPage;			// [20]���纰 ����ǥ(�⺻�������)
	CBMOutSangPage*	m_pBMOutBasicCompPage;			// [21]�⺻�� ����� ����ǥ

	// ��������ǥ �����(????)
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
