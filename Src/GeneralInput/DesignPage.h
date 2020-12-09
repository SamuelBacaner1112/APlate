// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DESIGNPAGE_H__26BFBDBC_C354_4EC4_A47D_B88055A5B3B4__INCLUDED_)
#define AFX_DESIGNPAGE_H__26BFBDBC_C354_4EC4_A47D_B88055A5B3B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignPage.h : header file
//
#include "StackDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDesignPage dialog
//#define ROW_COUNT_ETC 36

class CDataManage;
class CDesignSteelDimPage;
// 설계조건/기본조건
class AFX_EXT_CLASS CDesignPage : public TStackedPage
{
// Construction
public:
	CDesignPage(CDataManage *pMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDesignPage)
	enum { IDD = IDD_DESIGN_BASIC_CONDITION };
	CGridCtrl m_Grid;

	//}}AFX_DATA
	CDataManage *m_pDataManage;
// Overrides
	// ClassWizard generated virtual function overrides 
	//{{AFX_VIRTUAL(CDesignPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	enum COLTYPE {
COL_TOTAL_TYPE,						// 종 류           
COL_BRIDGE_KIND,					// 교량종류
COL_STEELSTAND,						// 강기준 
//COL_BRIDGE_HYUNGTAI,				// 교량형식
COL_BRIDGE_GRADE,					// 교량등급
//COL_BRIDGE_ANALYSIS,				// 해석방법(합성/비합성)
COL_STD_GIRHEIGHT_UPPER	,			// 거더높이 상부기준
COL_THICK_MIN_UP,					// 상판 최저두께
COL_THICK_MIN_Lo,					// 하판 최저두께
COL_THICK_MIN_WEB,					// 복부판 최저두께
COL_HEU_INTHICK,					// 인접 두께 편차 허용치
COL_WEIGHT_MAX_CAR,					// 차량적재 최대중량
COL_LEN_MAX_SPACE,					// 현장이음 최대길이
COL_GIR_STT_NUM	,					// 거더시작 번호
COL_SPACE_NUM,						// 현장이음 번호
COL_WEIGHT_STA_ELE_CODI	,			// 중량,STATION,ELEVATION,좌표 단위
COL_JARISU_PYEN_JONG,				// 편경사 및 종단기울기 표기 자리수
COL_STD_FILLET_WELD	,				// 필렛 용접 치수 기준
COL_LEN_MIN_FILLET_WELD	,			// 필렛 용접 최소 길이
COL_THICK_WELD_MUGAISUN	,			// 무개선 용접 적용 두께차
COL_THICK_WELD_XLINE,				// X 개선 용접 적용 두께
COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN,	// 볼트배치 지그재그 타입 가로보 상.하판
COL_BOLT_BACHI_WEBPAN_BYUN_TYPE	,	// 볼트배치 복부판 변단면 타입
COL_BOLT_BACHI_WEBPAN_CROSS,		// 볼트배치 복부판 가로보
COL_STD_WEB_EUPAN_JONG,				// 복부 이음판 상부 경사 기준
COL_STD_WEB_EUPAN_BYUN,				// 복부 이음판 하부 경사 기준
COL_JARISU_UNIT_KYUNGSA,			// 이음판 경사적용 단위(반올림 자리수)
COL_TYPE_CROSS_BRAKET,				// 가로보 브라켓 경사 타입
COL_SULCHI_CROSS_HSTIFF_BRAKET,		// 가로보 수평보강재 브라켓위치 설치
COL_STD_HBRACING_HG_DIR	,			// 수평브레이싱 형강 방향
COL_CALC_FLOORCENTER_BAR,			// 바닥판 중앙부 철근 계산방법
COL_EARTHQUACK_GRADE,				// 내진등급
COL_EARTHQUACK_ZONE,				// 지진구역 구분
COL_FASTLINESEARCH,					// 빠른 선형검색
COL_INSTALLBEAMATCEN,				// 브레이싱 골조선 도심축 설치
COL_ALLOW_CRACK_WIDTH,				// 허용균열폭 : 강재의 부식에 대한 환경조건
COL_INSULATIONVSTIFFUF,				// TU거더교 수직 보강재 상부플랜지 이격 옵션
// 여기 위로 추가

ROW_COUNT_ETC//총 개수
//COL_GIR_HYUNGAI,				//거더이음형태
//COL_APP_THICK_GANG,			//두께별 강종 자동 변환 적용
//COL_SULCHI_VSTIFF_CROSS,		//가로보사이 수직보강재 설치
//COL_SULCHI_HSTIFF,			//수평보강재 설치
//COL_BOLT_AUTO_CALC,			//모든 볼트길이 자동계산
//COL_SULCHI_VSTIFF_OUTSIDE,	// 외측거더외측방향 수직보강재설치
	};

private:
	static SHGRIDTITLE m_gt[ROW_COUNT_ETC];	

	void SetBridgeAnalysis(CString szStr);
	void SetBridgeGrade(CString szStr);
	CString GetStrBridgeAnalysis(long nType);
	CString GetStrBridgeGrade(CString szStr);
	CString GetBoltArrage(long nIdx);
	long GetBoltArrage(CString szStr);
	CEarthQUackGradeDlg m_EarthQUackGradeDlg;
	CEarthQuackDlg m_EarhtQuackDlg;
	CEarthQuackFactDlg m_EarthQuackFactDlg;
	CGirHeightUserDlg m_GirHeightUserDlg;

public:
	void OnPreInitDialog();
	void SetGridTitle();	
	void SetDataInit();	
	void SetDataSave();	
	void SetDataDefault();
	CDesignPageDlg	*m_pDesignPageDlg;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDesignPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* nmgv, LRESULT*);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESIGNPAGE_H__26BFBDBC_C354_4EC4_A47D_B88055A5B3B4__INCLUDED_)
