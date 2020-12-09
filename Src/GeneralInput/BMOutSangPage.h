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

	// ������ ������ ������
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
	// ���ۿ�
	void SetGridSangPage();					// [01]�⺻���� ��������ǥ
	void SetGridStandardSangPage();			// [02]ǥ������ǥ
	void SetGridStandardShopPage();			// [03]ǥ������ǥ(���ۿ�)
	void SetGridStandardShopType2Page();	// [03]ǥ������ǥ(���ۿ�)
	void SetGridThickPage();				// [04]������ �β�������ǥ
	void SetGridThickSangPage();			// [04]������ �β�������ǥ
	void SetGridBujeMarkSangPage();			// [05]���縶ũ�� ������ǥ
	void SetGridBujeMarkTotalPage();		// [06]���縶ũ�� �Ѱ�����ǥ
	void SetGridBujeMarkDeductPage();		// [07]���縶ũ�� ��������ǥ
	void SetGridCarPage();					// [08]�������� ������ǥ
	void SetGridCarTotalPage();				// [09]�������� �Ѱ�����ǥ
	void SetGridGrandAssemSangPage();		// [10]�������� ������ǥ
	void SetGridGrandAssemTotalPage();		// [11]�������� �Ѱ�����ǥ
	void SetGridMidAssemSangPage();			// [12]�������� ������ǥ
	void SetGridMidAssemTotalPage();		// [13]�������� �Ѱ�����ǥ
	void SetGridAssemSangPage();			// [14]�������� ������ǥ
	void SetGridAssemTotalPage();			// [15]�������� �Ѱ�����ǥ
	void SetGridAssemDeductPage();			// [16]�������� ��������ǥ
	void SetGridAssemFactPage();			// [17]�������� �������ۿ�����ǥ
	void SetGridAssemFieldPage();			// [18]�������� ���弳ġ������ǥ
	void SetGridSectionCompPage();			// [19]�ܸ麰 ����ǥ(�⺻�������)
	void SetGridBujeCompPage();				// [20]���纰 ����ǥ(�⺻�������)
	void SetGridBasicCompPage();			// [21]�⺻�� ����� ����ǥ

	void DrawGridSangPage();				// [01]�⺻���� ��������ǥ
	void DrawGridStandardSangPage();		// [02]ǥ������ǥ
	void DrawGridStandardShopPage();		// [03]ǥ������ǥ(���ۿ�)
	void DrawGridStandardShopType2Page();	// [03]ǥ������ǥ(���ۿ�)
	void DrawGridThickPage();				// [04]������ �β�������ǥ
	void DrawGridThickSangPage();			// [04]������ �β�������ǥ
	void DrawGridBujeMarkSangPage();		// [05]���縶ũ�� ������ǥ
	void DrawGridBujeMarkTotalPage();		// [06]���縶ũ�� �Ѱ�����ǥ
	void DrawGridBujeMarkDeductPage();		// [07]���縶ũ�� ��������ǥ
	void DrawGridCarPage();					// [08]�������� ������ǥ
	void DrawGridCarTotalPage();			// [09]�������� �Ѱ�����ǥ
	void DrawGridGrandAssemSangPage();		// [10]�������� ������ǥ
	void DrawGridGrandAssemTotalPage();		// [11]�������� �Ѱ�����ǥ
	void DrawGridMidAssemSangPage();		// [12]�������� ������ǥ
	void DrawGridMidAssemTotalPage();		// [13]�������� �Ѱ�����ǥ
	void DrawGridAssemSangPage();			// [14]�������� ������ǥ
	void DrawGridAssemTotalPage();			// [15]�������� �Ѱ�����ǥ
	void DrawGridAssemDeductPage();			// [16]�������� ��������ǥ
	void DrawGridAssemFactPage();			// [17]�������� �������ۿ�����ǥ
	void DrawGridAssemFieldPage();			// [18]�������� ���弳ġ������ǥ
	void DrawGridSectionCompPage();			// [19]�ܸ麰 ����ǥ(�⺻�������)
	void DrawGridBujeCompPage();			// [20]���纰 ����ǥ(�⺻�������)
	void DrawGridBasicCompPage();			// [21]�⺻�� ����� ����ǥ


	// ����� ////////////////////////
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
