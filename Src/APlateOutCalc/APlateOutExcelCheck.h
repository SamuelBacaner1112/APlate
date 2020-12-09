// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutExcelCheck.h: interface for the CAPlateOutExcelCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTEXCELCHECK_H__1BFAB3DC_CC71_48E7_A22A_DD28F58B8977__INCLUDED_)
#define AFX_APLATEOUTEXCELCHECK_H__1BFAB3DC_CC71_48E7_A22A_DD28F58B8977__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_MAKEANALYSIS_XLCHECK	(WM_APP + 1010)	// Check xls
class CAPlateOutCalcStd;

class CAPlateOutExcelCheck : public CThreadWork
{
public:
	CAPlateOutExcelCheck();
	virtual ~CAPlateOutExcelCheck();

	virtual	UINT DoWork();
	void OutXLSelect(long nXLIdx, CString sXLSaveFile);
	int OutAPlateExcelCheck(CXLControl *pXL, long nXLIdx);

	CXLControl  *m_pXL;
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	CString		m_sXLCheckSaveFile[16];	// XL�� ������ �̸�
	CString		m_sXLCheckReadFile[16];	// XL�� ������ �̸�
	int XLCheckOutAll();
	int OutAPlateExcelCheckAll(CXLControl *pXL);
	void SetAllFiles(BOOL bAllFiles)	{m_bAllFiles = bAllFiles;};
	void NewSheet(CXLControl *pXL, CString strSheetName, long nCurFile, CString strFont, double dCellWidth=1.0, double dCellHeight=19.5, short nTextSize=9);
protected:
	long	m_nCurFileIdx[16];	// ��ü���� ��� �� ���� ��� ���� ���� �ε���
	long	m_nZoomSize;		// �������� ����
	long	m_nSheetNo;			///< �� ������ Sheet ��ȣ
	long	m_nProgress;		///< Progress ���ప
	BOOL	m_bAllFiles;		///< ��ü���� ���
};

#endif // !defined(AFX_APLATEOUTEXCELCHECK_H__1BFAB3DC_CC71_48E7_A22A_DD28F58B8977__INCLUDED_)
