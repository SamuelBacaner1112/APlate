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
	long			m_nG;		// 거더 Index
	long			m_nSp;	// Splice Number
	CStringArray	m_CalcArr;		// 계산서 Text 저장.
	//----------------------------------------------------------------
	// 상부
	double m_n;			// 필요 볼트 수
	double m_na;		// 현재 볼트 수
	double m_As;		// 이음판 필요 단면적
	double m_Ass;		// 이음판 실제 단면적
	double m_Su;		// 이음판 설계 응력
	double m_Ss;		// 이음판 실제 응력
	double m_R;			// 실제 합성 전단력
	// 하부
	double m_nl;
	double m_nal;
	double m_Asl;
	double m_Assl;
	double m_Sl;
	double m_Ssl;
	double m_Rl;
	// 복부판 현장 이음 해석
	double m_SaAr[6];	// 볼트의 실제 전단력 1열~6열
	double m_Rwh;		// 볼트 1개의 실제 전단력
	double m_Rw;		// 실제 합성 전단력
	double m_Swu;		// 상부 이음판 실제 응력
	double m_Swl;		// 하부 이음판 실제 응력

	double m_Sft;		// 인장부 플랜지 응력
	//------------------------------------------
	// BaseData
   	double m_Ra;	// 기초설정 : 허용력 Bolt 마찰면 * 계수 * 2;
	double m_Sa;	// 기초설정 : 강종 허용 인장응력 : kgf/㎠

	CBrush m_bkBrush;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLICEDESIGNRESULT_H__E4DEE9D3_4C3D_4E68_AD7B_641467A19A46__INCLUDED_)
