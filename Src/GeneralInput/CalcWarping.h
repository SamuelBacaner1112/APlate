// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CALCWARPING_H__78D048AF_A7CC_4BF7_8C6C_6D8B919BAEE4__INCLUDED_)
#define AFX_CALCWARPING_H__78D048AF_A7CC_4BF7_8C6C_6D8B919BAEE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalcWarping.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcWarping dialog

class CCalcWarping : public CGeneralBaseDlg
{
// Construction
public:
	CCalcWarping(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcWarping)
	enum { IDD = IDD_CALC_WARPING };
	CXTFlatComboBox	m_TypeCombo;		// �ܸ�Ÿ��
	CString			m_CheckOKNG;		// Warping������	
	CString			m_WeakSectionType;	// ���� ����� �ܸ�(������Ʋ�� ����� ���� ���� �ܸ�)
	CString			m_strKFactor;
	CString			m_strIFactor;
	CXTFlatEdit		m_ctrlEdit1;
	CXTFlatEdit		m_ctrlEdit2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcWarping)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);	
	void OnPreInitDialog();

	double Calc_WarpingRatio(long nG, double dSta);

	long m_TypeNum;
	long m_nCurSectionType;

protected:
	// Generated message map functions
	//{{AFX_MSG(CCalcWarping)
	afx_msg void OnSelchangeWarpingGirderCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCWARPING_H__78D048AF_A7CC_4BF7_8C6C_6D8B919BAEE4__INCLUDED_)
