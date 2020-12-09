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
	// ���÷���Ʈ �Է�
	double	m_dVSlop		;	// ���ܰ����
	double	m_dW			;	// ������ ����
	double	m_dW2			;	// ������ ����
	double	m_dW3			;	// ������ ����
	double	m_dH			;	// �������� ����
	double	m_dH2			;	// �������� ����
	double	m_dH3			;	// �������� ����
	double	m_dHoleD		;	// Ȧ������
	double	m_dT1			;	// ���� �������� �β�  
	double	m_dT			;	// �߾Ӻ� �β�
	double	m_dT2			;	// ���� ��  ���� �β�
	long	m_nTon			;	// SHOE�� ���� ���ݷ�
	double	m_dSH1			;	// SHOE�� �β�
	double	m_dSH2			;	// SHOE�� �β�
	double	m_dSH3			;	// �������Ż�� �β�
	double	m_dSH4			;	// SHOE BLOCK�� ����
	BYTE	m_bLevel		;	// ���� ����
	BOOL	m_bType			;	// Ȧ �Ǵ� �� ����
	double	m_dMW1			;	// ��Ż�� W1 - ����
	double	m_dMW2			;	// ��Ż�� W2 - ����
	double	m_dMW3			;	// ��Ż�� W3 - ���� �ݴ�

	double	m_dSolePlateW	;
	double	m_dSolePlateT	;

	long	m_nJijumSu		;	// ������
	long	m_nGirderSu		;	// �Ŵ���

	long	m_nJijum		;	// ���� ����
	long	m_nGirder		;	// ���� �Ŵ�

	long	m_nCurIndex		;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETSOLEPLATEDLG_H__01506E95_9B33_4865_B30F_9CD51CB96F14__INCLUDED_)


