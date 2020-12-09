// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutExcel.h: interface for the CAPlateOutExcel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTEXCEL_H__81E2707B_20C2_4A61_B7EC_A20324331D55__INCLUDED_)
#define AFX_APLATEOUTEXCEL_H__81E2707B_20C2_4A61_B7EC_A20324331D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_MAKEANALYSIS_XL	(WM_APP + 1008)	// ��꼭 xls
#define ERROR_MODELING		0
#define ERROR_FILEOPEN		1

class CAPlateOutCalcStd;
class CHGAllowStress;

class CAPlateOutExcel : public CThreadWork
{
public: 
	CAPlateOutExcel();
	virtual ~CAPlateOutExcel();
public:
	// ���ο� Sheet ����
	virtual void NewSheet(CXLControl *pXL, CString strSheetName, long nCurFile, CString strFont="����ü", double dCellWidth=2.0, double dCellHeight=22.5, short nTextSize = 9);
	virtual void SetProgress(CString strText, long nlW);
	virtual long GetProgress()			{return m_nProgress;};
	virtual	UINT DoWork();
	void	OutCalcCover(CXLControl *pXL);
	void	OutCalcContents_WithOut_Tendon(CXLControl *pXL);
	void	OutCalcContents_With_Tendon(CXLControl *pXL);
	void	OutXLSelect(long nXLIdx, CString sXLSaveFile);
	int		OutAPlateXL(CXLControl *pXL, long nXLIdx);
	void	SetAllFiles(BOOL bAllFiles)	{m_bAllFiles = bAllFiles;};
	long	GetSheetNo()				{return m_nSheetNo;};
	long	GetZoomSize()				{return m_nZoomSize;};
	void	AddSheetSu()				{m_nSheetNo++;};
	CXLControl*	GetImageXL()			{return m_pImageXL;};

	CString GetCellAllowTableStt(long nTypeAllow)		{return m_strCellTableStt[nTypeAllow];	};
	CString GetCellAllowTableEnd(long nTypeAllow)		{return m_strCellTableEnd[nTypeAllow];	};
	CString GetCellThick(long nTypeAllow)				{return m_strCellThick[nTypeAllow];		};
	//////////////////////////////////////////
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	CString				m_sXLSaveFile[10];	///< XL�� ������ �̸�
	CString				m_sXLReadFile[10];	///< �о�� �����̸�
	CString ReturnChar(int Num);
protected:
	long				m_nCurFileIdx[10];	///< ��ü���� ��� �� ���� ��� ���� ���� �ε���
	long				m_nSheetNo;			///< �� ������ Sheet ��ȣ
	long				m_nZoomSize;		///< �������� ����
	long				m_nProgress;		///< Progress ���ప
	BOOL				m_bAllFiles;		///< ��ü���� ���
	CString				m_strImageXLPath;	///< �׸����� ���
	CXLControl			*m_pImageXL;		///< �׸� ����
	CMap<long, long, CString, LPCTSTR> m_strCellThick;
	CMap<long, long, CString, LPCTSTR> m_strCellTableStt;
	CMap<long, long, CString, LPCTSTR> m_strCellTableEnd;

	BOOL XLOutput_1(CXLControl *pXL);		///< 0
	BOOL XLOutput_2(CXLControl *pXL);		///< 1
	BOOL XLOutput_3(CXLControl *pXL);		///< 2
	BOOL XLOutput_4(CXLControl *pXL);		///< 3
	BOOL XLOutput_5(CXLControl *pXL);		///< 4
	BOOL XLOutput_6(CXLControl *pXL);		///< 5
	BOOL XLOutput_7(CXLControl *pXL);		///< 6
	BOOL XLOutput_8(CXLControl *pXL);		///< 7

	BOOL XLError(UINT nError);	
};

#endif // !defined(AFX_APLATEOUTEXCEL_H__81E2707B_20C2_4A61_B7EC_A20324331D55__INCLUDED_)
