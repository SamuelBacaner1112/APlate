// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutCalcXL.h: interface for the CAPlateOutCalcXL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTCALCXL_H__2C288457_5148_400F_A532_68E114348054__INCLUDED_)
#define AFX_APLATEOUTCALCXL_H__2C288457_5148_400F_A532_68E114348054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)
#define	BLACK				0
#define WHITE				2
#define RED					3
#define	BLUE				5
#define	YELLOW				6
#define GRAY				15
#define NEXT				1
#define CURRENT				0

class CAPlateOutCalcXL
{
public:
	
	CAPlateOutCalcXL();
	virtual ~CAPlateOutCalcXL();
	///< String Text 쓰기
	virtual void SetXL(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell, long nNextLinesu=1,
						short nTextColor=1, short nTextSize=9, BOOL bBold=FALSE, long TA_ALIGN=TA_LEFT, CString strCellID="", long nNumberSize=3);
	///< Double Text 쓰기
	virtual	void SetXL(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,long nNextLinesu=1,
						short nTextColor=1, short nTextSize=9, BOOL bBold=FALSE, long TA_ALIGN=TA_LEFT, CString strCellID="", long nNumberSize=3);
	///< BoxLine Text - String
	virtual void SetLineBoxText(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell, long nLinesu=1,
						short nTextColor=1, short nTextSize=9, BOOL bBold=FALSE, long TA_ALIGN=TA_CENTER, CString strCellID="", long nNumberSize=3, BOOL bLine = TRUE, long nCellColor = 2);
	virtual void SetLineBoxText(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,long nLinesu=1,
						short nTextColor=1, short nTextSize=9, BOOL bBold=FALSE, long TA_ALIGN=TA_CENTER, CString strCellID="", long nNumberSize=3, BOOL bLine = TRUE, long nCellColor = 2);
	///< UnderLine Text
	virtual void SetUnderLineText(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell, long nNextLinesu=1,
						short nTextColor=1, short nTextSize=9, BOOL bBold=FALSE, long TA_ALIGN=TA_CENTER, CString strCellID="", long nNumberSize=3);
	virtual void SetUnderLineText(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,long nNextLinesu=1,
						short nTextColor=1, short nTextSize=9, BOOL bBold=FALSE, long TA_ALIGN=TA_CENTER, CString strCellID="", long nNumberSize=3);
	///< Insert Domyun
	virtual void InsertImage(CXLControl *pXL, CString strImageName, long nSttRow, CString strInsertCell);
	virtual void InsertDomyun(CXLControl *pXL, CDomyun *pDom, long &nSttRow, double dLeftDom, long nDomCellsu,  BOOL bChangeRow = TRUE, BOOL bCenterPos = FALSE, BOOL bWide = FALSE);
	virtual void InsertDomyunByFixWidth(CXLControl *pXL, CDomyun *pDom, long &nSttRow, long nSttCol = 2, long nColsuAtDomWidth = 21, BOOL bCalcRow = TRUE, BOOL bCenterPos = TRUE, BOOL bWide = FALSE);
	virtual long InsertDomyunBy_FixWidth(CXLControl *pXL, CDomyun *pDom, long &nSttRow, long nSttCol, long nColsuAtDomWidth, long nColPerPage, BOOL bChangeRow = TRUE, BOOL bCenterPos = FALSE, BOOL bWide = FALSE);

	virtual void InsertDomyunByFixWidthAndHeight(CXLControl *pXL, CDomyun *pDom, long &nSttRow, long nSttCol, long nRowsuAtDomHeight, long nColsuAtDomWidth, long nColPerPage, BOOL bChangeRow = TRUE, BOOL bCenterPos = FALSE, BOOL bVCenterPos = FALSE);
	virtual void SetColsWidth(CXLControl *pXL, long nSttCol, long nEndCol, long nWidth);

	///< GetExcelValue
	virtual double GetCellValue(CXLControl *pXL, CString strCellID);

	virtual CString GetCellRef(CString strCellID);

	void SetTextFormat(CXLControl *pXL, long &nSttRow, CString strSttCell, CString strEndCell, long nLinesu, long nNumberSize);
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	virtual void ClearCellRef();
	virtual void OnSheetRefMode(BOOL bSheetRef);
	virtual void SetSheetFormat(CXLControl *pXL, CString strFont="굴림체", long nCellWidth=2, double dCellHeight=20.0, short nTextSize = 9);
	virtual void BackUpCellRef();		///< Cell 참조를 저장
	virtual void OnBackUpCellRef(BOOL bBackUp) {m_bBackUp = bBackUp;};	///< 저장한 Cell 참조
	virtual void SetCellRef(CXLControl *pXL, CString szKey, CString szCellID);		///< CellID1의 Cell번호를 CellID2와 동일하게
protected:
	CMapStringToString m_CelRefToCelID;
	CMapStringToString m_CellSheetRef;
	CMapStringToString m_BackUpSheetRef;
	BOOL	m_bSheetRef;
	BOOL	m_bBackUp;
};

#endif // !defined(AFX_APLATEOUTCALCXL_H__2C288457_5148_400F_A532_68E114348054__INCLUDED_)

