// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_PPCINPUTBASEDLG_H__61D575F4_DBF1_446B_BF1E_3BFF254EDBFF__INCLUDED_)
#define AFX_PPCINPUTBASEDLG_H__61D575F4_DBF1_446B_BF1E_3BFF254EDBFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPCInputBaseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralBaseDlg dialog

#define MAXTAB 10
#define TIMER_DRAW_SET 100

class CDomyunView;
class CDomyunGLView;

class CGeneralBaseStd;

class AFX_EXT_CLASS CGeneralBaseDlg : public CXTPResizeDialog
{
// Construction
public:
	CGeneralBaseDlg(UINT nID, CWnd* pParent = NULL);   // standard constructor
//	CGeneralBaseDlg(UINT nPlaceholder, UINT nID, CWnd* pParent = NULL);
                               
	friend class CGeneralBaseStd;

private:		
	CRect m_rcPin;
	CRect m_rcPosition;
	CRect m_rcHelp;
	CRect m_rcPrev;
	CRect m_rcNext;

	CBitmap m_bitmapNext;
	CBitmap m_bitmapPrev;
	CBitmap m_bitmapNextAll;
	CBitmap m_bitmapPrevAll;
	CBitmap m_bitmapApply;
	CBitmap m_bitmapRecommend;

protected:	
	BOOL m_bEnablePin;
	BOOL m_bChildDlg;
	BOOL m_bCenter;
	BOOL m_bErrorInclude;
	BOOL m_bZoomAll;
	BOOL m_bGL;

	CGeneralBaseStd *m_pStd;	
	UINT	m_nIDD;
	UINT	m_nMenuID;
	BOOL	m_bUsedFlag;	
	int		m_nPos;	// + : Next 방향 - : Prev 방향		
	BOOL	m_bOnInitDialog;
	BOOL	m_bModifyData;

	BOOL	m_bResizeControl;		///< 컨트롤 리사이즈 여부

public:
	CDomyunGLView		*m_pView;		///< GL 도면 뷰
	CDomyunView			*m_pRightView;	///< 도면 뷰
	long	m_nTab;					// 평면도, 종단면도 등 의 공통 옵션을 사용하기 위한 것
	double	m_dScaleDim;

	double m_dScaleOffset;				///< 대화상자 스케일 옵셋
	double m_dScaleDefault;				///< 대화상자 기본 스케일
	double m_dTextHeight;

public:
	void SetModifyInputData(BOOL bModifyData);
	void SetModifyData(BOOL bModifyData) { m_bModifyData = bModifyData;}
	BOOL IsModifyData()				{ return m_bModifyData;		}
	BOOL IsInitDialog()				{ return m_bOnInitDialog;	}
	void MoveControlGroup(CDoubleArray &arrID, CPoint xyMove, long nSpeed, long nDelay, double dBoundRate);
	// 항상 존재 하여야 할 함수	
	virtual void SetDataSave() {}
	virtual void SetDataInit() {}
	virtual void SetDataDefault() {}
	virtual void SetDataBx() {};
	virtual void OnCursorChange(long nRow, long nCol);
	virtual void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	virtual void SetDlgTitle();
	virtual BOOL IsValid() { return TRUE;}
	virtual CString GetHtmlPage() { return ""; }
	virtual void SetFocusItem(CString szTitle);
	virtual BOOL IsFocusItem(CString szTitle);
	// 상부 탭 관련 - 종단, 횡단
	virtual long GetTabCount() { return 1; }
	virtual void OnSelChangedTab(long nTab);
	virtual CString GetTabTitle(long nIdx) { return "입력"; }	
	// 추가 옵션
	virtual void OnPreInitDialog() {}; 
	// 전체 자동 설정 함수
	virtual void OnAutoCalcAll() {};
	// 기타 질문 함수
	virtual BOOL ContinueMsg();

	virtual void OnMouseMove(UINT nFlags, CPoint point) {}
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {}
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point) {}

	// 공통함수
	void CreateEx(CWnd *pParent, BOOL bCenter);
	void CreateByChild(CWnd *pParent, CWnd *pBar);
	void MoveDialog();
	CDomyunGLView *GetView() const	{ return m_pView; }	
	UINT GetIDD() const				{ return m_nIDD; }
	void SetMenuID(UINT nID)		{ m_nMenuID = nID; }
	UINT GetMenuID() const			{ return m_nMenuID; }
	void SetParentStd(CGeneralBaseStd *pStd) { m_pStd = pStd; }	
	CGeneralBaseStd* GetStd()		{ return m_pStd; }
	BOOL IsErrorInclude()			{ return m_bErrorInclude; }
	void SetErrorInclude(BOOL bError) { m_bErrorInclude = bError; }	
	BOOL IsGL()						{ return m_bGL; }
	void SetUsedFlag(BOOL bFlag)	{ m_bUsedFlag = bFlag; }
	BOOL IsUsedFlag()				{ return m_bUsedFlag; }
	BOOL IsCenter()					{ return m_bCenter; }
	BOOL IsEnablePin()				{ return m_bEnablePin; }
	BOOL GetMyselfWindow(CWnd **pFindWnd);
	double GetDrawScaleDim()		{ return m_dScaleDim; }
	void SetScaleDim(double dScale) { m_dScaleDim = dScale; }

	void RemoveCaptionBar();

// Dialog Data
	//{{AFX_DATA(CGeneralBaseDlg)	
	//}}AFX_DATA	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralBaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();	
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DrawTitleButton();
	DWORD HitTest(CPoint pt);
	DWORD m_LastHit;
	DWORD m_ButtonDownPin;		

	enum { _BUTTON_UP_, _BUTTON_PUSH_, _BUTTON_POS_PUSH_, _BUTTON_POS_UP_, _BUTTON_HELP_PUSH_, _BUTTON_HELP_UP_,
		   _BUTTON_PREV_UP_, _BUTTON_PREV_PUSH_, _BUTTON_NEXT_UP_, _BUTTON_NEXT_PUSH_
	};

	void DrawButton(int nType);	

	// Generated message map functions
	//{{AFX_MSG(CGeneralBaseDlg)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual afx_msg void OnButtonNext();
	virtual afx_msg void OnButtonPrev();
	virtual afx_msg void OnButtonApply();
	virtual afx_msg void OnButtonRecommend();
	virtual afx_msg void OnCellChangedData(NMHDR*nmgv,LRESULT*);
	virtual afx_msg void OnCellChanged(NMHDR*nmgv,LRESULT*);
	virtual afx_msg void OnCellChangedGridN(NMHDR*nmgv,LRESULT*);
	virtual afx_msg BOOL OnInitDialog();
	virtual afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual afx_msg void	OnNcPaint();
	virtual afx_msg BOOL	OnNcActivate(BOOL bActive);
    virtual afx_msg void	OnNcLButtonDown(UINT nHitTest, CPoint point);
	virtual afx_msg void	OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	virtual afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);	

#if ( _MFC_VER == 0x600 )
	virtual afx_msg UINT	OnNcHitTest(CPoint point);
#else
	virtual afx_msg LRESULT	OnNcHitTest(CPoint point);
#endif
	virtual afx_msg void OnPrevHis();
	virtual afx_msg void OnNextHis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPCINPUTBASEDLG_H__61D575F4_DBF1_446B_BF1E_3BFF254EDBFF__INCLUDED_)
