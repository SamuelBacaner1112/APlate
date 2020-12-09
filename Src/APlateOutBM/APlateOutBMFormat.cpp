// APlateOutBMFormat.cpp: implementation of the CAPlateOutBMFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutBM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutBMFormat::CAPlateOutBMFormat(CAPlateOutBMStd *pAPlateOutBMStd)
{
	m_pAPlateOutBMStd = pAPlateOutBMStd;
}

CAPlateOutBMFormat::~CAPlateOutBMFormat()
{

}

///< String Text ����
void CAPlateOutBMFormat::SetXL(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell,
							 long nNextLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3;
	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow);

	pXL->SetXL(sCell1, strText);
	if(sCell1 != sCell2)		pXL->SetMergeCell(sCell1, sCell2);

	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

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

void CAPlateOutBMFormat::SetXL(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,
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
	pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

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

CString CAPlateOutBMFormat::GetCellRef(CString strCellID)
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

void CAPlateOutBMFormat::SetCellRef(CXLControl *pXL, CString szKey, CString szCellID)
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

///< Text ����� Line���� ������ ����
void CAPlateOutBMFormat::SetLineBoxText(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell,
							long nLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize, BOOL bLine, long nCellColor)
{
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
		if(sCell1 != sCell2)
			pXL->SetMergeCell(sCell1, sCell2);

		if(nCellColor != 2)
			pXL->SetCellColor(sCell1, sCell2, nCellColor);

		if(TA_ALIGN!=TA_RIGHT)
			pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	}

	if(nTextSize!=9 || nTextColor!=1 || bBold)
		pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

	if(bLine)
		pXL->CellOutLine(sCell1, sCell2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}

	CString str;
	if(nNumberSize == 0)		str="###0";
	else if(nNumberSize == 1)	str="###0.0";
	else if(nNumberSize == 2)	str="###0.00";
	else if(nNumberSize == 3)	str="###0.000";
	else if(nNumberSize == 4)	str="###0.0000";
	else if(nNumberSize == 5)	str="###0.00000";
	else if(nNumberSize == 6)	str="###0.000000";
	else if(nNumberSize == 7)	str="###0.0000000";
	else if(nNumberSize == 8)	str="###0.00000000";
	else if(nNumberSize == 9)	str="###0.000000000";

	pXL->SetNumberFormat(sCell1, sCell2, str);
}

void CAPlateOutBMFormat::SetLineBoxText(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,
							long nLinesu, short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize, BOOL bLine, long nCellColor)
{
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
	if(strText != "")
	{
		pXL->SetXL(sCell1, strText);

		if(sCell1 != sCell2)
			pXL->SetMergeCell(sCell1, sCell2);

		if(nCellColor != 2)	
			pXL->SetCellColor(sCell1, sCell2, nCellColor);

		if(TA_ALIGN!=TA_RIGHT)
			pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	}

	if(nTextSize!=9 || nTextColor!=1 || bBold)
		pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

	if(bLine)	
		pXL->CellOutLine(sCell1, sCell2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}

	CString str;
	if(nNumberSize == 0)		str="###0";
	else if(nNumberSize == 1)	str="###0.0";
	else if(nNumberSize == 2)	str="###0.00";
	else if(nNumberSize == 3)	str="###0.000";
	else if(nNumberSize == 4)	str="###0.0000";
	else if(nNumberSize == 5)	str="###0.00000";
	else if(nNumberSize == 6)	str="###0.000000";
	else if(nNumberSize == 7)	str="###0.0000000";
	else if(nNumberSize == 8)	str="###0.00000000";
	else if(nNumberSize == 9)	str="###0.000000000";

	pXL->SetNumberFormat(sCell1, sCell2, str);
}

///< UnderLineText - String Type
void CAPlateOutBMFormat::SetUnderLineText(CXLControl *pXL, CString strText, long &nSttRow, CString strSttCell, CString strEndCell,
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
	pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);
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
void CAPlateOutBMFormat::SetUnderLineText(CXLControl *pXL, double dText, long &nSttRow, CString strSttCell, CString strEndCell,
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
	pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);
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



void CAPlateOutBMFormat::InsertDomyun(CXLControl *pXL, CDomyun *pDom, long &nSttRow, double dLeftDom, long nDomCellsu,  BOOL bChangeRow, BOOL bCenterPos, BOOL bWide)
{
	CAPlateOutBMDlg	*pDlg	= m_pAPlateOutBMStd->GetAPlateOutBMDlg();
//	if(pDlg->m_buttonOpt.GetCheck() == FALSE)	return;
	pDom->SetCalcExtRect();
	// 0.6, 30 �� ������Ʈ�� ��, �÷�ũ�� ������ �����ؾ� ��.
	CDRect	rect		= pDom->GetExtRect();
	double  dExlHPerCell= pXL->GetCellHeight(1);	// �������� ���� 
	double	dDomHPerCell= 30;						// �ѿ��� ���� ����
	double  dDomDivExl  = dExlHPerCell/dDomHPerCell;// ������̸� ���� ���̷� ��ȯ�� ����
	double  dExlWPerCell= dExlHPerCell*0.55;			// �����÷��� ��(���� ������ 0.6 ������)
	double	dDomWPerCell= dDomHPerCell*0.55;			// ���÷��� ���� ��
	double	dRatio		= rect.Width() / rect.Height();// ������ ��Ⱦ��
	double	dHeightDom	= nDomCellsu * dDomHPerCell;// ������ ����
	double	dWidthDom	= dHeightDom*dRatio;		// ������ ��
	double  dWidthDomPre= dWidthDom;				// ���� �� ������..
	double  dColPerPage = bWide ? 56 : 45;			// ���������� ��ü �÷���

	if(dWidthDom > dColPerPage*dDomWPerCell-dLeftDom*2)		// �׸��� �������� �Ѿ ���
		dWidthDom = dColPerPage*dDomWPerCell-dLeftDom*2;	// Dom ���� ����(�¿��� ������ �����)
	if(bCenterPos)
		dLeftDom = (dColPerPage*dDomWPerCell)/2-dWidthDom/2;// �߾���ġ���� �׸����� ������ �������� �̵�.

	pDom->RedrawByHVScale(dWidthDom/dWidthDomPre, TRUE);	// Dom�� ��Ⱦ�� ��������� ���߾� ��.

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	pXL->AddDomWmfAuto(pDom, szPath, dLeftDom*dDomDivExl, dExlHPerCell*(nSttRow-1), dWidthDom, dHeightDom);

	CXLControl::GetWmfViewer().InsertDomyun(pDom);

	if(bChangeRow)	nSttRow += nDomCellsu+1;
}



void CAPlateOutBMFormat::InsertDomyunByFixWidth(CXLControl *pXL, CDomyun *pDom, long &nSttRow, double dLeftDom, long nColsuAtDomWidth,  BOOL bChangeRow, BOOL bCenterPos, BOOL bWide)
{
	CAPlateOutBMDlg	*pDlg	= m_pAPlateOutBMStd->GetAPlateOutBMDlg();
//	if(pDlg->m_buttonOpt.GetCheck() == FALSE)	return;
	pDom->SetCalcExtRect();
	// 0.6, 30 �� ������Ʈ�� ��, �÷�ũ�� ������ �����ؾ� ��.
	CDRect	rect		= pDom->GetExtRect();
	double  dExlHPerCell= pXL->GetCellHeight(1);	// �������� ���� 
	double	dDomHPerCell= 30;						// �ѿ��� ���� ����
	double  dDomDivExl  = dExlHPerCell/dDomHPerCell;// ������̸� ���� ���̷� ��ȯ�� ����
	double  dExlWPerCell= dExlHPerCell*0.6;			// �����÷��� ��(���� ������ 0.6 ������)
	double	dDomWPerCell= dDomHPerCell*0.6;			// ���÷��� ���� ��
	double	dRatio		= rect.Width() / rect.Height(); // ������ ��Ⱦ��
	double  dWidthDom   = nColsuAtDomWidth*dDomWPerCell;// ������ ��
	double	dHeightDom	= dWidthDom/dRatio;				// ������ ����
	double  dColPerPage = bWide ? 56 : 37;				// ���������� ��ü �÷���

	if(bCenterPos)
		dLeftDom = (dColPerPage*dDomWPerCell)/2-dWidthDom/2;// �߾���ġ���� �׸����� ������ �������� �̵�.

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	pXL->AddDomWmfAuto(pDom, szPath, dLeftDom*dDomDivExl, dExlHPerCell*(nSttRow-1), dWidthDom, dHeightDom);

	CXLControl::GetWmfViewer().InsertDomyun(pDom);

	nSttRow += (long)(dHeightDom/dDomHPerCell);

	if(bChangeRow)	nSttRow++;
}


double CAPlateOutBMFormat::GetCellValue(CXLControl *pXL, CString strCellID)
{
	double	dValue	= 0;
	CString	strCell	= GetCellRef(strCellID);

	dValue = atof(pXL->GetXL(strCell));
	return	dValue;
}

void CAPlateOutBMFormat::SetTextFormat(CXLControl *pXL, long &nSttRow, CString strSttCell, CString strEndCell, long nLinesu, long nNumberSize)
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

void CAPlateOutBMFormat::ClearCellRef()
{
	m_CelRefToCelID.RemoveAll();
	m_CellSheetRef.RemoveAll();
}

///< 
void CAPlateOutBMFormat::SetColsWidth(CXLControl *pXL, long nSttCol, long nEndCol, long nWidth)
{
	pXL->SetCellWidth(nSttCol, nEndCol, nWidth);
}
///< Excel File�� �о� �׸� ����
void CAPlateOutBMFormat::InsertImage(CXLControl *pXL, CString strImageName, long nSttRow, CString strInertCell)
{
//	CARoadOutXL	*pARoadOutXL = m_pARoadOutXLStd->GetARoadOutXL();
//
//	long nCurFile	= pARoadOutXL->m_nCurFileIdx[pARoadOutXL->m_nXLIdx];
//	CString	sCell;
//	sCell.Format("%s%d", strInertCell, nSttRow);
//
//	if(nCurFile == 1)
//		pXL->CopyPicture(pARoadOutXL->GetImageXL2(), strImageName, sCell);
//	else if(nCurFile == 3 || nCurFile == 4)
//		pXL->CopyPicture(pARoadOutXL->GetImageXL4(), strImageName, sCell);
//	else if(nCurFile == 6)
//		pXL->CopyPicture(pARoadOutXL->GetImageXL7(), strImageName, sCell);
}

///< ������ Sheet ���ۿ��� TRUE
///< ������ Sheet ������ FALSE
///< ������ Sheet ���ۿ��� TRUE
void CAPlateOutBMFormat::OnSheetRefMode(BOOL bSheetRef)
{
	m_bSheetRef = bSheetRef;
}

void CAPlateOutBMFormat::SetSheetFormat(CXLControl *pXL, CString strFont, long nCellWidth, double dCellHeight, short nTextSize)
{
	pXL->SetCellWidth(0, 255, nCellWidth);
	pXL->SetCellHeight(0, 65535, dCellHeight);
	pXL->SetFonts(0, 0, 65535, 255, nTextSize, strFont, 1, FALSE);
	pXL->SetVerAlign(0, 0, 65535, 255, 2);
	pXL->SetNumberFormat(0, 0, 65535, 255, "###0.000");
}

void CAPlateOutBMFormat::BackUpCellRef()
{
	long nCount = m_CellSheetRef.GetCount();
	if(nCount == 0) return;

	POSITION pos;
	CString szKey;
	CString	szCellRef;

	for(pos = m_CellSheetRef.GetStartPosition(); pos != NULL; )
	{
		m_CellSheetRef.GetNextAssoc(pos, szKey, szCellRef);
		m_BackUpSheetRef.SetAt(szKey, szCellRef);		///< �ٸ� Sheet ���� ����
	}
}

void CAPlateOutBMFormat::SetDomScaleByExtRect(CDomyun *pDom, double dFixScale, double dRectDiagonal)
{
	pDom->SetCalcExtRect();

	CDRect BoundRect  = pDom->GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = dFixScale*(dDiagonalL/dRectDiagonal);

	pDom->SetScaleDim(dScale);
}

void CAPlateOutBMFormat::SetXL(CXLControl *pXL, CString strText, long &nSttRow, long nSttCol, long nEndCol, long nNextLinesu,
						short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3, strSttCell, strEndCell;
	strSttCell = GetCellID(nSttCol);
	strEndCell = GetCellID(nEndCol);

	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow);

	pXL->SetXL(sCell1, strText);
	if(sCell1 != sCell2)		pXL->SetMergeCell(sCell1, sCell2);

	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

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

///< Double Text ���� - Column Index�� Long Type
void CAPlateOutBMFormat::SetXL(CXLControl *pXL, double dText, long &nSttRow, long nSttCol, long nEndCol, long nNextLinesu,
					short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize)
{
	if(pXL == NULL) return;

	CString strText;
	CString	sCell1, sCell2, sCell3, strSttCell, strEndCell;
	strSttCell = GetCellID(nSttCol);
	strEndCell = GetCellID(nEndCol);

	sCell1.Format("%s%d", strSttCell, nSttRow);
	sCell2.Format("%s%d", strEndCell, nSttRow);

	strText.Format("%.9f", dText);
	pXL->SetXL(sCell1, strText);
	if(sCell1 != sCell2)		pXL->SetMergeCell(sCell1, sCell2);

	pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

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

void CAPlateOutBMFormat::SetLineBoxText(CXLControl *pXL, CString strText, long &nSttRow, long nSttCol, long nEndCol, long nLinesu,
					short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize, BOOL bLine, long nCellColor)
{
	CString	sCell1, sCell2, sCell3, strSttCell, strEndCell;
	strSttCell = GetCellID(nSttCol);
	strEndCell = GetCellID(nEndCol);

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
		if(sCell1 != sCell2)
			pXL->SetMergeCell(sCell1, sCell2);

		if(nCellColor != 2)
			pXL->SetCellColor(sCell1, sCell2, nCellColor);
		
		if(TA_ALIGN!=TA_RIGHT)
			pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	}


	if(nTextSize!=9 || nTextColor!=1 || bBold)
		pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

	if(bLine)
		pXL->CellOutLine(sCell1, sCell2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}

	CString str;
	if(nNumberSize == 0)		str="###0";
	else if(nNumberSize == 1)	str="###0.0";
	else if(nNumberSize == 2)	str="###0.00";
	else if(nNumberSize == 3)	str="###0.000";
	else if(nNumberSize == 4)	str="###0.0000";
	else if(nNumberSize == 5)	str="###0.00000";
	else if(nNumberSize == 6)	str="###0.000000";
	else if(nNumberSize == 7)	str="###0.0000000";
	else if(nNumberSize == 8)	str="###0.00000000";
	else if(nNumberSize == 9)	str="###0.000000000";

	pXL->SetNumberFormat(sCell1, sCell2, str);
}

void CAPlateOutBMFormat::SetLineBoxText(CXLControl *pXL, double dText, long &nSttRow, long nSttCol, long nEndCol,long nLinesu,
				short nTextColor, short nTextSize, BOOL bBold, long TA_ALIGN, CString strCellID, long nNumberSize, BOOL bLine, long nCellColor)
{
	if(pXL == NULL) return;

	CString	sCell1, sCell2, sCell3, strSttCell, strEndCell;
	strSttCell = GetCellID(nSttCol);
	strEndCell = GetCellID(nEndCol);

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
	if(strText != "")
	{
		pXL->SetXL(sCell1, strText);
		if(sCell1 != sCell2)
			pXL->SetMergeCell(sCell1, sCell2);
		
		if(nCellColor != 2)
			pXL->SetCellColor(sCell1, sCell2, nCellColor);

		if(TA_ALIGN!=TA_RIGHT)
			pXL->SetHoriAlign(sCell1, sCell2, TA_ALIGN);
	}

	if(nTextSize!=9 || nTextColor!=1 || bBold)
		pXL->SetFonts(sCell1, sCell2, nTextSize, "����", nTextColor, bBold);

	if(bLine)
		pXL->CellOutLine(sCell1, sCell2);

	if(strCellID != "")
	{
		m_CelRefToCelID.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
		CString szSheetName = pXL->GetSheetName();
		sCell3.Format("'%s'!%s", szSheetName, sCell1);

		m_CellSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell3));
		if(m_bBackUp)
			m_BackUpSheetRef.SetAt(LPCTSTR(strCellID), LPCTSTR(sCell1));
	}

	CString str;
	if(nNumberSize == 0)		str="###0";
	else if(nNumberSize == 1)	str="###0.0";
	else if(nNumberSize == 2)	str="###0.00";
	else if(nNumberSize == 3)	str="###0.000";
	else if(nNumberSize == 4)	str="###0.0000";
	else if(nNumberSize == 5)	str="###0.00000";
	else if(nNumberSize == 6)	str="###0.000000";
	else if(nNumberSize == 7)	str="###0.0000000";
	else if(nNumberSize == 8)	str="###0.00000000";
	else if(nNumberSize == 9)	str="###0.000000000";

	pXL->SetNumberFormat(sCell1, sCell2, str);
}

CString CAPlateOutBMFormat::GetCellID(long nColIdx)
{
	char *strIndex[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
						"AA","AB","AC","AD","AE","AF","AG","AH","AI","AJ","AK","AL","AM","AN","AO","AP","AQ","AR","AS","AT","AU","AV","AW","AX","AY","AZ",
						"BA","BB","BC","BD","BE","BF","BG","BH","BI","BJ","BK","BL","BM","BN","BO","BP","BQ","BR","BS","BT","BU","BV","BW","BX","BY","BZ",
						"CA","CB","CC","CD","CE","CF","CG","CH","CI","CJ","CK","CL","CM","CN","CO","CP","CQ","CR","CS","CT","CU","CV","CW","CX","CY","CZ"};
	if(nColIdx > sizeof(strIndex)/sizeof(strIndex[0])) return _T("A");

	return strIndex[nColIdx];
}