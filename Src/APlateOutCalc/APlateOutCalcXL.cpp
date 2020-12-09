// APlateOutCalcXL.cpp: implementation of the CAPlateOutCalcXL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcXL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAPlateOutCalcXL::CAPlateOutCalcXL()
{
	m_CelRefToCelID.RemoveAll();
	m_CellSheetRef.RemoveAll();
	m_bSheetRef = FALSE;
	m_bBackUp = FALSE;
}

CAPlateOutCalcXL::~CAPlateOutCalcXL()
{

}

///< String Text 쓰기
void CAPlateOutCalcXL::SetXL(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell,
							 long nNextLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3;
	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow);

	pXL->SetXL(sCell1, strText);
	if(sCell1 != sCell2)		pXL->SetMergeCell(sCell1, sCell2);
	if(TA_ALIGN!=TA_LEFT)
		pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	if(nTextSize!=9 || nTextColor != BLACK || bBold != FALSE)
		pXL->SetFonts(sCell1, sCell2, nTextSize, "굴림체", nTextColor, bBold);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));

		CString szSheetName = pXL->GetSheetName();

		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));

		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}
	SetTextFormat(pXL, nSttRow, strSttCell, strEndCell, 1, nNumberSize);
	nSttRow += nNextLinesu;
}

void CAPlateOutCalcXL::SetXL(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,
							long nNextLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3;
	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow);

	CString	strText;
	strText.Format("%.9f", dText);
	pXL->SetXL(sCell1, strText);
	if(sCell1 != sCell2)
	{
		pXL->SetMergeCell(sCell1, sCell2);
	}
	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "굴림체", nTextColor, bBold);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));

		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}
	SetTextFormat(pXL, nSttRow, strSttCell, strEndCell, 1, nNumberSize);
	nSttRow += nNextLinesu;
}

CString CAPlateOutCalcXL::GetCellRef(CString strCellID)
{
	CString	strCell;
	CString	sCell;

	m_CelRefToCelID.Lookup(LPCTSTR(strCellID), (CString &)strCell);
	if(m_bSheetRef)
		m_CellSheetRef.Lookup(LPCTSTR(strCellID), (CString &)strCell);
	if(m_bBackUp)
		m_BackUpSheetRef.Lookup(LPCTSTR(strCellID), (CString &)strCell);

	return strCell;
}

void CAPlateOutCalcXL::SetCellRef(CXLControl *pXL, CString szKey, CString szCellID)
{
	CString	strCell;
	CString	sCell;

	m_CelRefToCelID.Lookup(LPCTSTR(szKey), (CString &)strCell);
	if(strCell != "")
	{
		CString szSheetName = pXL->GetSheetName();
		m_CelRefToCelID.SetAt(LPCTSTR(szCellID), LPCTSTR(strCell));

		sCell.Format("'%s'!%s", szSheetName, strCell);
		if(m_bSheetRef)
			m_CellSheetRef.SetAt(LPCTSTR(szCellID), LPCTSTR(strCell));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(szCellID), LPCTSTR(strCell));
	}
}

///< Text 출력후 Line수는 변하지 않음
void CAPlateOutCalcXL::SetLineBoxText(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell,
							long nLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize, BOOL bLine, long nCellColor)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3;
	if(nLinesu == 0)
	{
		sCell1.Format("%s%d", strSttCell, nSttRow+nLinesu-1);
		sCell2.Format("%s%d", strEndCell, nSttRow);
	}
	else
	{
		sCell1.Format("%s%d", strSttCell, nSttRow);
		sCell2.Format("%s%d", strEndCell, nSttRow+nLinesu-1);	
	}

	if(strText != "")
	{
		pXL->SetXL(sCell1, strText);
		pXL->SetMergeCell(sCell1, sCell2);
		if(nCellColor != 2)
			pXL->SetCellColor(sCell1, sCell2, nCellColor);
	}
	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "굴림체", nTextColor, bBold);

	if(bLine)	pXL->CellOutLine(sCell1, sCell2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}
	SetTextFormat(pXL, nSttRow, strSttCell, strEndCell, nLinesu, nNumberSize);

}

void CAPlateOutCalcXL::SetLineBoxText(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,
							long nLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize, BOOL bLine, long nCellColor)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3;
	if(nLinesu == 0)
	{
		sCell1.Format("%s%d", strSttCell, nSttRow+nLinesu-1);
		sCell2.Format("%s%d", strEndCell, nSttRow);
	}
	else
	{
		sCell1.Format("%s%d", strSttCell, nSttRow);
		sCell2.Format("%s%d", strEndCell, nSttRow+nLinesu-1);	
	}

	CString	strText;
	strText.Format("%.9f", dText);
	pXL->SetXL(sCell1, strText);
	pXL->SetMergeCell(sCell1, sCell2);
	if(nCellColor != 2)	pXL->SetCellColor(sCell1, sCell2, nCellColor);

	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "굴림체", nTextColor, bBold);

	if(bLine)	pXL->CellOutLine(sCell1, sCell2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}
	SetTextFormat(pXL, nSttRow, strSttCell, strEndCell, nLinesu, nNumberSize);
}

///< UnderLineText - String Type
void CAPlateOutCalcXL::SetUnderLineText(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell,
							long nNextLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3;
	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow);

	if(strText != "")
	{
		pXL->SetXL(sCell1, strText);
		pXL->SetMergeCell(sCell1, sCell2);
	}
	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "굴림체", nTextColor, bBold);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}
	SetTextFormat(pXL, nSttRow, strSttCell, strEndCell, 1, nNumberSize);
	nSttRow += nNextLinesu;
}

///< UnderLineText - double Type
void CAPlateOutCalcXL::SetUnderLineText(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,
							long nNextLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3;
	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow);

	CString	strText;
	strText.Format("%.lf", dText);
	pXL->SetXL(sCell1, strText);
	pXL->SetMergeCell(sCell1, sCell2);

	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "굴림체", nTextColor, bBold);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}
	SetTextFormat(pXL, nSttRow, strSttCell, strEndCell, 1, nNumberSize);
	nSttRow += nNextLinesu;
}

void CAPlateOutCalcXL::InsertDomyun(CXLControl *pXL, CDomyun *pDom, long &nSttRow, double dLeftDom, long nDomCellsu,  BOOL bChangeRow, BOOL bCenterPos, BOOL bWide)
{
	CAPlateOutputDlg	*pDlg	= m_pAPlateOutCalcStd->GetAPlateOutputDlg();
	if(pDlg->m_ctrlTab.GetCurSel()==0 && pDlg->m_buttonOpt.GetCheck() == FALSE)	return;

	pDom->SetCalcExtRect();

	// 0.6, 30 은 엑셀쉬트의 열, 컬럼크기 조정시 변경해야 함.
	CDRect	rect		= pDom->GetExtRect();
	double  dExlHPerCell= pXL->GetCellHeight(1);		// 엑셀열의 높이
	double	dDomHPerCell= pXL->GetCellWidth(1)*10;		// 한열의 도면 높이
	double  dDomDivExl  = dExlHPerCell/dDomHPerCell;	// 도면높이를 엑셀 높이로 변환시 비율
	double	dDomWPerCell= dDomHPerCell*dDomDivExl;//0.6;// 한컬럼의 도면 폭
	double	dRatio		= rect.Width() / rect.Height();	// 도면의 종횡비
	double	dHeightDom	= nDomCellsu * dDomHPerCell;	// 도면의 높이
	double	dWidthDom	= dHeightDom*dRatio;			// 도면의 폭
	double  dWidthDomPre= dWidthDom;					// 도면 폭 변경전..
	double  dColPerPage = bWide ? 40 : 27;				// 한페이지의 전체 컬럼수

	if(dWidthDom > dColPerPage*dDomWPerCell-dLeftDom*2)		// 그림이 페이지를 넘어갈 경우
		dWidthDom = dColPerPage*dDomWPerCell-dLeftDom*2;	// Dom 폭을 변경(좌우측 마진을 고려함)
	if(bCenterPos)
		dLeftDom = (dColPerPage*dDomWPerCell)/2-dWidthDom/2;// 중앙위치에서 그림폭의 절반을 좌측으로 이동.

//	pDom->RedrawByHVScale(dWidthDom/dWidthDomPre, TRUE);	// Dom의 종횡비를 엑셀사이즈에 맞추어 줌.
	pDom->RedrawByScale(dWidthDom/dWidthDomPre);
	pDom->SetCalcExtRect();

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	pXL->AddDomWmfAuto(pDom, szPath, dLeftDom*dDomDivExl, dExlHPerCell*(nSttRow-1), dWidthDom, dHeightDom);

	CXLControl::GetWmfViewer().InsertDomyun(pDom);

	if(bChangeRow)	nSttRow += (long)nDomCellsu+1;
}

void CAPlateOutCalcXL::InsertDomyunByFixWidthAndHeight(CXLControl *pXL, CDomyun *pDom, long &nSttRow, long nSttCol, long nRowsuAtDomHeight, long nColsuAtDomWidth, long nColPerPage, BOOL bChangeRow, BOOL bCenterPos, BOOL bVCenterPos)
{	
	CAPlateOutputDlg	*pDlg	= m_pAPlateOutCalcStd->GetAPlateOutputDlg();
	if(pDlg->m_buttonOpt.GetCheck() == FALSE)	return;
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	
	long nRow = pXL->InsertDomyun_FixWidthAndHeight(pDom, szPath, nSttRow-1, max(1, nSttCol-1), nRowsuAtDomHeight, nColsuAtDomWidth, nColPerPage, bCenterPos, bVCenterPos);
	if(bChangeRow) nSttRow += nRow;
}

long CAPlateOutCalcXL::InsertDomyunBy_FixWidth(CXLControl *pXL, CDomyun *pDom, long &nSttRow, long nSttCol, long nColsuAtDomWidth, long nColPerPage, BOOL bChangeRow, BOOL bCenterPos, BOOL bWide)
{
	CAPlateOutputDlg	*pDlg	= m_pAPlateOutCalcStd->GetAPlateOutputDlg();
	if(pDlg->m_buttonOpt.GetCheck() == FALSE)	return 0;
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	
	long nRow = pXL->InsertDomyun_FixWidth(pDom, szPath, nSttRow-1, max(1, nSttCol-1), nColsuAtDomWidth, nColPerPage, bCenterPos);
	if(bChangeRow) nSttRow += nRow;

	return nRow;
}


// nSttCol : 도면이 시작되는 컬럼(From 0)
// bCalcRow: 도면의 높이를 계산하여 열수를 바꾸어줌
// bCenterPos : nSttCol을 무시하고 도면을 중앙에 배치함.
// bWide : 엑셀이 가로출력일 경우 사용함.
// NewSheet 기본값 바뀌면 dDomHPerCell, dDomWPerCell, dColPerPage만 수정하면 됨 
void CAPlateOutCalcXL::InsertDomyunByFixWidth(CXLControl *pXL, CDomyun *pDom, long &nSttRow, 
											  long nSttCol, long nColsuAtDomWidth,  BOOL bCalcRow, BOOL bCenterPos, BOOL bWide)
{
	CAPlateOutputDlg	*pDlg	= m_pAPlateOutCalcStd->GetAPlateOutputDlg();
	if(pDlg->m_buttonOpt.GetCheck() == FALSE)	return;
	pDom->SetCalcExtRect();

	CDRect	rect		= pDom->GetExtRect();
	double	dDomHPerCell= 30;						// 한열당 도면높이(30픽셀)
	double	dDomWPerCell= 25;						// 한행당 도면폭  (25픽셀)
	double  dExlHPerCell= pXL->GetCellHeight(1);	// 엑셀열의 높이 (높이:22.5)
	double  dExlWPerCell= dExlHPerCell*(dDomWPerCell/dDomHPerCell);	// 엑셀행의 (폭:18.742)
	double	dRatio		= rect.Width() / rect.Height(); // 도면의 종횡비
	double  dWidthDom   = nColsuAtDomWidth*dDomWPerCell;// 도면의 폭
	double	dHeightDom	= dWidthDom/dRatio;				// 도면의 높이
	double  dColPerPage = bWide ? 38 : 27;				// 한페이지의 전체 컬럼수
	
	double dLeft = nSttCol*dExlWPerCell;
	if(bCenterPos)
		dLeft = (dExlWPerCell*dColPerPage)/2-(nColsuAtDomWidth*dExlWPerCell)/2;	// 중앙위치에서 그림폭의 절반을 좌측으로 이동.

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	// dLeft와 dTop은 엑셀크기 기준
	// dWidth와 dHeight는 도면 크기(픽셀) 기준임.
	pXL->AddDomWmfAuto(pDom, szPath, dLeft, dExlHPerCell*(nSttRow-1), dWidthDom, dHeightDom);

	CXLControl::GetWmfViewer().InsertDomyun(pDom);

	if(bCalcRow)	nSttRow += (long)Round(dHeightDom/dExlHPerCell, 0);
}

double CAPlateOutCalcXL::GetCellValue(CXLControl *pXL, CString strCellID)
{
	double	dValue	= 0;
	CString	strCell	= GetCellRef(strCellID);

	dValue = atof(pXL->GetXL(strCell));
	return	dValue;
}

void CAPlateOutCalcXL::SetTextFormat(CXLControl *pXL, long &nSttRow, CString strSttCell, CString strEndCell, long nLinesu, long nNumberSize)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2;
	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow+nLinesu-1);
	CString	str;
	if(nNumberSize == 0)	str="###0";
	else					str="###0.";

	for(long n = 0; n < nNumberSize; n++)
	{
		str += "0";
	}
	pXL->SetNumberFormat(sCell1, sCell2, str);
}

void CAPlateOutCalcXL::ClearCellRef()
{
	m_CelRefToCelID.RemoveAll();
	m_CellSheetRef.RemoveAll();
}

///< 
void CAPlateOutCalcXL::SetColsWidth(CXLControl *pXL, long nSttCol, long nEndCol, long nWidth)
{
	pXL->SetCellWidth(nSttCol, nEndCol, nWidth);
}
///< Excel File을 읽어 그림 삽입
void CAPlateOutCalcXL::InsertImage(CXLControl *pXL, CString strImageName, long nSttRow, CString strInertCell)
{
	CAPlateOutExcel *pOutExcel = m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CString	sCell;
	sCell.Format("%s%d", strInertCell, nSttRow);

	pXL->CopyPicture(pOutExcel->GetImageXL(), strImageName, sCell);
}

///< 참조될 Sheet 시작에서 TRUE
///< 참조될 Sheet 끝에서 FALSE
///< 참조할 Sheet 시작에서 TRUE
void CAPlateOutCalcXL::OnSheetRefMode(BOOL bSheetRef)
{
	m_bSheetRef = bSheetRef;
}

void CAPlateOutCalcXL::SetSheetFormat(CXLControl *pXL, CString strFont, long nCellWidth, double dCellHeight, short nTextSize)
{
	pXL->SetCellWidth(0, 255, nCellWidth);
	pXL->SetCellHeight(0, 65535, dCellHeight);
	pXL->SetFonts(0, 0, 65535, 255, nTextSize, strFont, 1, FALSE);
	pXL->SetVerAlign(0, 0, 65535, 255, 2);
	pXL->SetNumberFormat(0, 0, 65535, 255, "###0.000");
}

void CAPlateOutCalcXL::BackUpCellRef()
{
	long nCount = m_CellSheetRef.GetCount();
	if(nCount == 0) return;

	POSITION pos;
	CString szKey;
	CString	szCellRef;

	for(pos = m_CellSheetRef.GetStartPosition(); pos != NULL; )
	{
		m_CellSheetRef.GetNextAssoc(pos, szKey, szCellRef);
		m_BackUpSheetRef.SetAt(szKey, szCellRef);		///< 다른 Sheet 참조 형태
	}
}
