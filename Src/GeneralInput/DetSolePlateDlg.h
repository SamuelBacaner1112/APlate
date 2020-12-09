// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETSOLEPLATEDLG_H__01506E95_9B33_4865_B30F_9CD51CB96F14__INCLUDED_)
#define AFX_DETSOLEPLATEDLG_H__01506E95_9B33_4865_B30F_9CD51CB96F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetSolePlateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetSolePlateDlg dialog

class CDetSolePlateDlg : public TStackedPage
{
// Construction
public:
	CDetSolePlateDlg(CGeneralBaseStd *pStd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetSolePlateDlg)
	enum { IDD = IDD_DET_01_SOLEPLATE };
	CComboBox	m_CompanyCombo;
	CXTTabCtrl	m_TabSelect;
	CGridCtrl	m_Grid;
	//}}AFX_DATA

	long			m_nTab;
	CShoeJewon		m_ShoeJeownDlg;
	long			m_nCurrentShoeJewon;
	CGeneralBaseStd *m_pStd;
	CButton			m_btnShoe;
	CString			m_strDBRefPath;
	CXTPButton		m_btnBearingDB;
	CFont			m_Font;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetSolePlateDlg)
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void ApplyIdentityAllGirder();
	void ApplyIdentityPart();
	void ApplyIdentityAllJijum();
	void ApplyCompleteInputAllSolePlate();
	void SetDataInit();
	void SetDataDefault();
	void SetDataDefaultSub(long row, long nJijum);
	void SetDataSave();
	void SetDlgText();	
	void SetGridTitle();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnCursorChange(long nRow, long nCol);
	void OnPreInitDialog();
	void MakeSolePlateSameData(CShoe *pShoe1, CShoe *pShoe2);

	CString GetShoeName();
	CString GetShoeDir();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetSolePlateDlg)
	afx_msg void OnSelchangeTabSolePlate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRecommend();
	afx_msg void OnButtonShoeRef();
	afx_msg void OnSelchangeComboCompany();
	afx_msg void OnSetfocusComboCompany();
	afx_msg void OnButtonSolePlateDropDown();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChanged		(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData	(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedRow(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedCol(NMHDR* nmgv, LRESULT*);

protected:
	// 솔플레이트 입력
	double	m_dVSlop		;	// 종단경사율
	double	m_dW			;	// 종방향 길이
	double	m_dW2			;	// 종방향 길이
	double	m_dW3			;	// 종방향 길이
	double	m_dH			;	// 교축직각 길이
	double	m_dH2			;	// 교축직각 길이
	double	m_dH3			;	// 교축직각 길이
	double	m_dHoleD		;	// 홀싸이즈
	double	m_dT1			;	// 교축 시작점의 두께  
	double	m_dT			;	// 중앙부 두께
	double	m_dT2			;	// 교축 끝  점의 두께
	long	m_nTon			;	// SHOE의 연직 허용반력
	double	m_dSH1			;	// SHOE의 두께
	double	m_dSH2			;	// SHOE의 두께
	double	m_dSH3			;	// 무수축몰탈의 두께
	double	m_dSH4			;	// SHOE BLOCK의 높이
	BYTE	m_bLevel		;	// 레벨 적용
	BOOL	m_bType			;	// 홀 또는 탭 구분
	double	m_dMW1			;	// 몰탈부 W1 - 교축
	double	m_dMW2			;	// 몰탈부 W2 - 교축
	double	m_dMW3			;	// 몰탈부 W3 - 교축 반대

	double	m_dSolePlateW	;
	double	m_dSolePlateT	;

	long	m_nJijumSu		;	// 지점수
	long	m_nGirderSu		;	// 거더수

	long	m_nJijum		;	// 현재 지점
	long	m_nGirder		;	// 현재 거더

	long	m_nCurIndex		;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETSOLEPLATEDLG_H__01506E95_9B33_4865_B30F_9CD51CB96F14__INCLUDED_)


