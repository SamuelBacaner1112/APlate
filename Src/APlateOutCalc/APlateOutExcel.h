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

#define WM_USER_MAKEANALYSIS_XL	(WM_APP + 1008)	// 계산서 xls
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
	// 새로운 Sheet 생성
	virtual void NewSheet(CXLControl *pXL, CString strSheetName, long nCurFile, CString strFont="굴림체", double dCellWidth=2.0, double dCellHeight=22.5, short nTextSize = 9);
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
	CString				m_sXLSaveFile[10];	///< XL로 저장할 이름
	CString				m_sXLReadFile[10];	///< 읽어올 파일이름
	CString ReturnChar(int Num);
protected:
	long				m_nCurFileIdx[10];	///< 전체파일 출력 중 현재 출력 중인 파일 인덱스
	long				m_nSheetNo;			///< 한 파일의 Sheet 번호
	long				m_nZoomSize;		///< 문서보기 배율
	long				m_nProgress;		///< Progress 진행값
	BOOL				m_bAllFiles;		///< 전체파일 출력
	CString				m_strImageXLPath;	///< 그림파일 경로
	CXLControl			*m_pImageXL;		///< 그림 엑셀
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
