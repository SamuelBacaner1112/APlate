// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DETVBEAMDLG_H__B29DB84F_0E62_43BC_85C6_78C32AE4E2AA__INCLUDED_)
#define AFX_DETVBEAMDLG_H__B29DB84F_0E62_43BC_85C6_78C32AE4E2AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetVBeamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetVBeamDlg dialog

class CDetVBeamDlg : public CGeneralBaseDlg
{
// Construction
public:
	CDetVBeamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetVBeamDlg)
	enum { IDD = IDD_DET_17_VERTICALBEAM };
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetVBeamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:	
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	BOOL IsValid();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	void OnCursorChange(long nRow, long nCol);

	// Grid ¼³Á¤
	void SetGridTitle();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetVBeamDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETVBEAMDLG_H__B29DB84F_0E62_43BC_85C6_78C32AE4E2AA__INCLUDED_)
