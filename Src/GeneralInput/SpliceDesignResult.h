// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SPLICEDESIGNRESULT_H__E4DEE9D3_4C3D_4E68_AD7B_641467A19A46__INCLUDED_)
#define AFX_SPLICEDESIGNRESULT_H__E4DEE9D3_4C3D_4E68_AD7B_641467A19A46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpliceDesignResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpliceDesignResult dialog
#include "resource.h"
class CSpliceDesignResult : public CDialog
{
// Construction
public:
	CSpliceDesignResult(CGeneralBaseStd *pStd, long nG, long nSp, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpliceDesignResult)
	enum { IDD = IDD_SPLICE_DESIGN_RESULT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CButton	m_btnOutText;
	CButton	m_btnOk;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpliceDesignResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	enum SP_RESULT {NONE, NG, OK};
	// Splice number
	void SetSplicePoint(long nG, long nType);

// Implementation
protected:
	void SetDataInit();				// initialize member by zero or NULL
	void DrawResult() const;		// Dialogue Resource Edit Box Contents
	void SetSpliceDesignData ();
	int  FindString (CStringArray &Arr, char *pFindStr, int &nStartRow, int &nFoundCol);
	SP_RESULT GetDesignResult(UINT nID); //NONE, OK, NG check
	// Generated message map functions
	//{{AFX_MSG(CSpliceDesignResult)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnSpliceDesignResultToText();
	DECLARE_MESSAGE_MAP()

protected:
	CGeneralBaseStd *m_pStd;
	long			m_nG;		// �Ŵ� Index
	long			m_nSp;	// Splice Number
	CStringArray	m_CalcArr;		// ��꼭 Text ����.
	//----------------------------------------------------------------
	// ���
	double m_n;			// �ʿ� ��Ʈ ��
	double m_na;		// ���� ��Ʈ ��
	double m_As;		// ������ �ʿ� �ܸ���
	double m_Ass;		// ������ ���� �ܸ���
	double m_Su;		// ������ ���� ����
	double m_Ss;		// ������ ���� ����
	double m_R;			// ���� �ռ� ���ܷ�
	// �Ϻ�
	double m_nl;
	double m_nal;
	double m_Asl;
	double m_Assl;
	double m_Sl;
	double m_Ssl;
	double m_Rl;
	// ������ ���� ���� �ؼ�
	double m_SaAr[6];	// ��Ʈ�� ���� ���ܷ� 1��~6��
	double m_Rwh;		// ��Ʈ 1���� ���� ���ܷ�
	double m_Rw;		// ���� �ռ� ���ܷ�
	double m_Swu;		// ��� ������ ���� ����
	double m_Swl;		// �Ϻ� ������ ���� ����

	double m_Sft;		// ����� �÷��� ����
	//------------------------------------------
	// BaseData
   	double m_Ra;	// ���ʼ��� : ���� Bolt ������ * ��� * 2;
	double m_Sa;	// ���ʼ��� : ���� ��� �������� : kgf/��

	CBrush m_bkBrush;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLICEDESIGNRESULT_H__E4DEE9D3_4C3D_4E68_AD7B_641467A19A46__INCLUDED_)
