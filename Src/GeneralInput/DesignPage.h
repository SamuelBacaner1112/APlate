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
// ��������/�⺻����
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
COL_TOTAL_TYPE,						// �� ��           
COL_BRIDGE_KIND,					// ��������
COL_STEELSTAND,						// ������ 
//COL_BRIDGE_HYUNGTAI,				// ��������
COL_BRIDGE_GRADE,					// �������
//COL_BRIDGE_ANALYSIS,				// �ؼ����(�ռ�/���ռ�)
COL_STD_GIRHEIGHT_UPPER	,			// �Ŵ����� ��α���
COL_THICK_MIN_UP,					// ���� �����β�
COL_THICK_MIN_Lo,					// ���� �����β�
COL_THICK_MIN_WEB,					// ������ �����β�
COL_HEU_INTHICK,					// ���� �β� ���� ���ġ
COL_WEIGHT_MAX_CAR,					// �������� �ִ��߷�
COL_LEN_MAX_SPACE,					// �������� �ִ����
COL_GIR_STT_NUM	,					// �Ŵ����� ��ȣ
COL_SPACE_NUM,						// �������� ��ȣ
COL_WEIGHT_STA_ELE_CODI	,			// �߷�,STATION,ELEVATION,��ǥ ����
COL_JARISU_PYEN_JONG,				// ���� �� ���ܱ��� ǥ�� �ڸ���
COL_STD_FILLET_WELD	,				// �ʷ� ���� ġ�� ����
COL_LEN_MIN_FILLET_WELD	,			// �ʷ� ���� �ּ� ����
COL_THICK_WELD_MUGAISUN	,			// ������ ���� ���� �β���
COL_THICK_WELD_XLINE,				// X ���� ���� ���� �β�
COL_BOLT_JIGJAG_CRORSS_UPLOW_PAN,	// ��Ʈ��ġ ������� Ÿ�� ���κ� ��.����
COL_BOLT_BACHI_WEBPAN_BYUN_TYPE	,	// ��Ʈ��ġ ������ ���ܸ� Ÿ��
COL_BOLT_BACHI_WEBPAN_CROSS,		// ��Ʈ��ġ ������ ���κ�
COL_STD_WEB_EUPAN_JONG,				// ���� ������ ��� ��� ����
COL_STD_WEB_EUPAN_BYUN,				// ���� ������ �Ϻ� ��� ����
COL_JARISU_UNIT_KYUNGSA,			// ������ ������� ����(�ݿø� �ڸ���)
COL_TYPE_CROSS_BRAKET,				// ���κ� ����� ��� Ÿ��
COL_SULCHI_CROSS_HSTIFF_BRAKET,		// ���κ� ���򺸰��� �������ġ ��ġ
COL_STD_HBRACING_HG_DIR	,			// ����극�̽� ���� ����
COL_CALC_FLOORCENTER_BAR,			// �ٴ��� �߾Ӻ� ö�� �����
COL_EARTHQUACK_GRADE,				// �������
COL_EARTHQUACK_ZONE,				// �������� ����
COL_FASTLINESEARCH,					// ���� �����˻�
COL_INSTALLBEAMATCEN,				// �극�̽� ������ ������ ��ġ
COL_ALLOW_CRACK_WIDTH,				// ���տ��� : ������ �νĿ� ���� ȯ������
COL_INSULATIONVSTIFFUF,				// TU�Ŵ��� ���� ������ ����÷��� �̰� �ɼ�
// ���� ���� �߰�

ROW_COUNT_ETC//�� ����
//COL_GIR_HYUNGAI,				//�Ŵ���������
//COL_APP_THICK_GANG,			//�β��� ���� �ڵ� ��ȯ ����
//COL_SULCHI_VSTIFF_CROSS,		//���κ����� ���������� ��ġ
//COL_SULCHI_HSTIFF,			//���򺸰��� ��ġ
//COL_BOLT_AUTO_CALC,			//��� ��Ʈ���� �ڵ����
//COL_SULCHI_VSTIFF_OUTSIDE,	// �����Ŵ��������� ���������缳ġ
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
