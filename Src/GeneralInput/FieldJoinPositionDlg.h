// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_FIELDJOINPOSITIONDLG_H__4211B560_9F1C_44FB_8EC7_5141051FDF2C__INCLUDED_)
#define AFX_FIELDJOINPOSITIONDLG_H__4211B560_9F1C_44FB_8EC7_5141051FDF2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldJoinPositionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFieldJoinPositionDlg dialog

class CFieldJoinPositionDlg : public CGeneralBaseDlg
{
// Construction
public:
	CFieldJoinPositionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFieldJoinPositionDlg)
	enum { IDD = IDD_GEN_18_FIELDJOINPOSITION };
	CXTFlatEdit m_Edit1;
	CXTFlatEdit m_Edit2;
	CButton	  m_Radio1;
	CButton   m_Radio2;
	CGridCtrl m_Grid;
	BOOL	m_bSameApplySameCol;
	int		m_nLengthType;
	double	m_dDisCenterOnSplice;
	BOOL	m_bCheckMove;
	double	m_dMoveTerm;
	//}}AFX_DATA
	CButton m_btnMoveLeft;
	CButton m_btnMoveRight;
	CButton m_btnAllSplice;
	CButton m_btnAllWeld;
	CButton m_btnMove1;
	CButton m_btnMove2;
	CButton m_btnAllSame;
	CButton m_btnSymmetry;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldJoinPositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
  
private:	
	long m_nCount;

private:	
	BOOL m_bFirstRun;
	void SetGridTitle();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();
	void SetDataBx();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	long GetTabCount() { return 2; }	
	CString GetTabTitle(long nIdx) { return nIdx==0 ? "평면도" : "종단면도"; }	

	// 내부용 함수
	long GetAutoCalcSpliceNum();

// Implementation
protected:
	void SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter);
	void SetMatrixByStation(long nRow, long nCol, double sta, BOOL bHistory = FALSE);
	double GetStationMatrix(long nRow, long nCol);
	void SpliceAutoSymmetry(long nG);
	void MoveByMomentStress(double dBaseTerm);
	long GetDirInKyunggan(long nG, double sta);
	BOOL m_bNextPrev;

	// Generated message map functions
	//{{AFX_MSG(CFieldJoinPositionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpliceAllGirder();
	afx_msg void OnSpliceGirderlen();
	afx_msg void OnSpliceBridgelen();
	afx_msg void OnSpliceAllequal();
	afx_msg void OnSpliceRightShift();
	afx_msg void OnSpliceLeftShift();
	afx_msg void OnSpliceCheckmove();
	afx_msg void OnSetDataSymmetry();
	afx_msg void MoveByMoment();
	afx_msg void MoveByStress();
	afx_msg void OnButtonApplyAllSplice();
	afx_msg void OnButtonApplyAllWeld();
	afx_msg void OnButtonNext();
	//}}AFX_MSG
	afx_msg void OnCellInsertedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeletedCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellDeleteCol(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChanged(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	afx_msg void OnCellCopiedData(NMHDR* nmgv, LRESULT*);

	DECLARE_MESSAGE_MAP()

protected:
	void	MoveLeftRight(long nDir);	// 현장이음 좌.우측으로 이동

	long GetNumberGirder(long nRow) { return (long)RoundDown((nRow+2)/3,0)-2; };
	long GetRow(long nG) { return nG*3+5; };
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDJOINPOSITIONDLG_H__4211B560_9F1C_44FB_8EC7_5141051FDF2C__INCLUDED_)
