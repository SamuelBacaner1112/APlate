// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_CMDEDIT_H__1F14F812_67F8_45D3_BF5B_2F5CE0601353__INCLUDED_)
#define AFX_CMDEDIT_H__1F14F812_67F8_45D3_BF5B_2F5CE0601353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdEdit.h : header file
//

#define TYPE_NORMAL  0
#define TYPE_OBJ	10

/////////////////////////////////////////////////////////////////////////////
// CCmdEdit window

class CCmdEdit : public CEdit
{
// Construction
public:
	CCmdEdit();

// Attributes
public:	
	CEdit *m_pHisEdit;

	CString m_CompileStr;	
	CString	m_CmdStr;
	BOOL m_bParse;
	int m_nHeadNum;
	long m_nCmdTree;
	short m_sType;
	CString m_CmdName;
	WPARAM m_LButtonCmd;

	BOOL CmdParse();
	BOOL CmdParse2();
	void InitMembers(CString str = _T(""), short sType = TYPE_NORMAL);	
	void AddHistory(CString Str,BOOL bNew = TRUE);	
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCmdEdit();
	void SetChar(UINT nChar);
	BOOL OnLButtonCmd();

private:	
	void OnHelpApp();
	BOOL ErrorCmd();
	// Generated message map functions
protected:
	//{{AFX_MSG(CCmdEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint uXy);
	afx_msg void OnSetfocus();	
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDEDIT_H__1F14F812_67F8_45D3_BF5B_2F5CE0601353__INCLUDED_)
