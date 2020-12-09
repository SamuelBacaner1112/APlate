// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EXPANSIONJOINTTYPEDLG_H__FF2CE8C8_54F1_49D6_869A_D2156C4A29D0__INCLUDED_)
#define AFX_EXPANSIONJOINTTYPEDLG_H__FF2CE8C8_54F1_49D6_869A_D2156C4A29D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpansionJointTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpansionJointTypeDlg dialog
const static char* ExpJointRef[] =
{	
//  단위 : mm, Kg, m2, m3
//   모델명		최대허용신축량	블럭아웃폭 블럭아웃높이 유간(최소)  유간(표준)  유간(최대) 무수축콘     거푸집  D13   D16
	"UCF-50S",	"50",			"300",		"230",		"30",		"55",		"80",		"0.113",	"0.56",	"",		"16.611",
	"UCF-75S",	"75",			"310",		"230",		"27.5",		"65",		"102.5",	"0.130",	"0.60",	"",		"16.762",
	"UCF-80S",	"80",			"310",		"230",		"30",		"70",		"110",		"0.130",	"0.60",	"",		"16.762",
	"UCF-100S",	"100",			"330",		"230",		"30",		"80",		"130",		"0.139",	"0.60",	"",		"16.838",
	"UCF-120S",	"120",			"330",		"250",		"30",		"90",		"150",		"0.173",	"0.70",	"",		"19.109",
	"UCF-150S",	"150",			"350",		"250",		"30",		"105",		"180",		"0.184",	"0.90",	"",		"21.260",
	"UCF-200S",	"200",			"380",		"350",		"30",		"130",		"230",		"0.279",	"0.90",	"",		"23.344",
	"UCF-250S",	"250",			"400",		"350",		"60",		"185",		"310",		"0.294",	"0.90",	"",		"23.621",
	"UCF-300S",	"300",			"430",		"350",		"60",		"210",		"360",		"0.316",	"0.90",	"",		"24.317",
	"UCF-350S",	"350",			"455",		"350",		"60",		"235",		"410",		"0.334",	"0.90",	"",		"25.012",
	"UCF-400S",	"400",			"485",		"350",		"60",		"260",		"460",		"0.356",	"0.90",	"",		"32.654",
	"UCF-450S",	"450",			"505",		"350",		"60",		"285",		"510",		"0.371",	"0.90",	"",		"33.349",
	"UCF-500S",	"500",			"525",		"350",		"60",		"310",		"560",		"0.386",	"0.90",	"",		"34.043",
	"UCF-550S",	"550",			"550",		"350",		"75",		"350",		"625",		"0.404",	"0.90",	"",		"34.530",
	"UCF-570S",	"570",			"570",		"350",		"100",		"400",		"700",		"0.419",	"0.90",	"",		"34.877",
	"MC-2G"   , "20"   ,        "250",      "150",      "50",       "60",       "70",       "0.072",    "",     "4.341","10.532",
	"MC-4G"   , "40"   ,        "250",      "150",      "80",       "100",      "120",      "0.072",    "",     "4.341","10.532",
	"MC-6G"   , "60"   ,        "250",      "150",      "80",       "110",      "140",      "0.072",    "",     "4.341","10.532",	
	"URJ 50"  , "50" ,          "200",      "200",      "25",       "50",       "75",       "0.111",    "0.176",     "","16.349",
	"URJ 80"  , "85" ,          "250",      "220",      "40",       "85",       "120",      "0.125",    "0.176",     "","16.349",
	"URJ 100" , "95" ,          "250",      "220",      "50",       "95",       "150",      "0.125",    "0.176",     "","16.349",
	"URJ 160" , "170" ,         "400",      "330",      "80",       "170",      "240",      "0.264",    "0.331",     "","35.818",
	"MC-30"   , "30"   ,        "320",      "250",      "45",       "60",       "75",       "",         "",     "",     "",
	"MC-50"   , "50"   ,        "315",      "250",      "45",       "70",       "95",       "",         "",     "",     "",
	"MC-60"   , "60"   ,        "332.5",    "250",      "55",       "85",       "115",      "",         "",     "",     "",
	"MC-80"   , "80"   ,        "343",      "250",      "74",       "114",      "154",      "",         "",     "",     "",
};

#define EXPANSIONTYPE_SIZE	26 //070207 일자로 제원 4개 추가...KB...

class CExpansionJointTypeDlg : public CDialog
{
// Construction
public:
	CString m_strDlgTitle;
	CExpansionJointTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExpansionJointTypeDlg)
	enum { IDD = IDD_DIALOG_EXPANSIONJOINT_TYPE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CGridCtrl		m_Grid;
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	void SetCellPaint(long nIndex);
	long GetExpansionJointType(CString strExpansionJoint);

	CString m_szName;
	double m_dMaxAllowExpL;
	double m_dBlockW;
	double m_dBlockH;
	double m_dUganMin;
	double m_dUganAve;
	double m_dUganMax;
	double m_dQuantityConcPerM2;
	double m_dQuantityFormPerM2;
	double m_dQuantityD13PerM2;
	double m_dQuantityD16PerM2;

	long m_nIndex;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpansionJointTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExpansionJointTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnCellChangedRow(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPANSIONJOINTTYPEDLG_H__FF2CE8C8_54F1_49D6_869A_D2156C4A29D0__INCLUDED_)
