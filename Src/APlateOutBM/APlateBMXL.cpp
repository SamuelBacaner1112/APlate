// APlateBMXL.cpp: implementation of the CAPlateBMXL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutBM.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateUI\APlateUI.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateBMXL::CAPlateBMXL(CAPlateOutBMStd *pAPlateOutBMStd)
{
	m_pAPlateOutBMStd		= pAPlateOutBMStd;
	m_nTotSheetSu			= 0;
	m_nRebarColIdx			= 0;
	m_szRebarCol.Add("B");m_szRebarCol.Add("C");m_szRebarCol.Add("D");m_szRebarCol.Add("E");
	m_szRebarCol.Add("F");m_szRebarCol.Add("G");

	m_pAPlateOutBMBrInfo = new CAPlateOutBMBrInfo(pAPlateOutBMStd);

	for(long n=0; n<2; n++)
	{
		m_bSlabRebar[n]				= FALSE;
		m_bGuardFenceRebar[n]		= FALSE;
		m_bExpansionJointRebar[n]	= FALSE;
		m_bConfinementConc[n]		= FALSE;
	}
}

CAPlateBMXL::~CAPlateBMXL()
{
	delete m_pAPlateOutBMBrInfo;
}

UINT CAPlateBMXL::DoWork()
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CStringArray	ArrDelSheet;
	CXLControl		XL;
	CXLControl		ImageXL;

	m_pImageXL = &ImageXL;

	KillTheXL();
	GetOutBMList();
	for(long n=0; n<2; n++)
	{
		m_bSlabRebar[n]				= 0;
		m_bGuardFenceRebar[n]		= 0;
		m_bExpansionJointRebar[n]	= 0;
	}

	long	nProgress	= 0;
	CString	sText		= _T("");
	CWinApp	*pApp		= AfxGetApp();
	CString	szSection	= "Afx Directory"; 
	CString	szFixedItem	= "Fixed Directory";
	CString	csAPlate	= pApp->GetProfileString(szSection, szFixedItem);
	CString	szFileName	= csAPlate + "\\PlateXLSheetPic2.dll";
	m_pImageXL->OpenXL(szFileName);
	m_MapGuardCell.RemoveAll();

	if(pDB->IsTUGir())	m_szReadFileName = csAPlate + "\\APlateOutBMSheet_TU.dll";
	else				m_szReadFileName = csAPlate + "\\APlateOutBMSheet.dll";

	SetProgress("�����...", nProgress);

	try
	{

#ifdef _DEBUG
		XL.SetXLVisible(TRUE);
#endif	
		if(XL.OpenXL(m_szReadFileName) == FALSE)	return 1;
		
		m_pAPlateOutBMStd->m_dXLVersion = XL.GetVersion();

		ArrDelSheet.Add("����(1)");
		ArrDelSheet.Add("����(2)");
		for(long n = 0; n < m_nTotSheetSu; n++)
		{
			nProgress	+= (long)(100.0/(double)m_nTotSheetSu);
			sText.Format("%s", GetNamePrintSheet(n));
			SetProgress(sText, nProgress);

			APlateOutBM(&XL, m_nBMSheetIdx[n], ArrDelSheet);
		}

		XL.DeleteSheetNotUsed(ArrDelSheet);
		XL.SaveAs(m_szSaveFilePath);
		XL.QuitXL();
	}
	catch(CException* theException)
	{
		TCHAR   szCause[255];
		CString strFormatted = "";
		theException->GetErrorMessage(szCause, 255);

		strFormatted.Format("%s",szCause);
		long nResult = AfxMessageBox(strFormatted,MB_OK | MB_ICONSTOP);			
		
		if(nResult == IDOK)
		{
			OnProgressText(1, "��¿Ϸ�");
			m_pAPlateOutBMStd->GetAPlateOutBMDlg()->m_ctrlProgress.SetPos(0);
			
			theException->Delete();
			return 1;
		}		
	}

	CAPlateOutBMDlg	*pARoadOutBMDlg	= m_pAPlateOutBMStd->GetAPlateOutBMDlg();

	pARoadOutBMDlg->GetDlgItem(IDC_BUTTON_BMOUT)->EnableWindow(TRUE);
	pARoadOutBMDlg->GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText("��¿Ϸ�");
	OnProgress(0, 0);
	return 0;
}

void CAPlateBMXL::KillTheXL()
{
	CString strName;
	HWND hWnd = ::GetDesktopWindow();
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	while(1)
	{								
		if(hWnd)
		{			
			pWnd = CWnd::FromHandle(hWnd);
			pWnd->GetWindowText(strName);
			if(strName.Find("Microsoft Excel")!=-1)
			{				
				if(!pWnd->IsWindowVisible())
				{ 
					DWORD pid;
					GetWindowThreadProcessId(pWnd->m_hWnd, &pid );
					HANDLE     hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
					TerminateProcess(hProcess, 0);
				}
			}
		}
		else 
			break;

		if(hWnd == ::GetDesktopWindow())
			hWnd = ::GetWindow(hWnd,GW_CHILD);
		else
			hWnd = ::GetWindow(hWnd,GW_HWNDNEXT);
		pWnd = CWnd::FromHandle(hWnd);
	}
}


void CAPlateBMXL::GetOutBMList()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CAPlateOutBMBrInfo	*pBrInfo	= GetBridgeInfo(0);
	if(m_bAbort)	return;

	for(long n = 0; n < 255; n++)
		m_nBMSheetIdx[n] = 0;

	long nIdx = 0;

	if(pBrInfo->m_bOutRebar)			m_nBMSheetIdx[nIdx++]	= REBAR_SUMMARY;
	if(pBrInfo->m_bOutTotRebar)			m_nBMSheetIdx[nIdx++]	= REBAR_TOTAL_SUMMARY;
	if(pBrInfo->m_bOutCalcFoundation)	m_nBMSheetIdx[nIdx++]	= SLAB_CALC_FOUNDATION;
	if(pBrInfo->m_bOutCalcFoundation)	m_nBMSheetIdx[nIdx++]	= GUARDFENCE_CALC_FOUNDATION;
	if(pBrInfo->m_bOutSlab)				m_nBMSheetIdx[nIdx++]	= SLAB_SUMMARY;
	if(pBrInfo->m_bOutCalcFoundation && pDB->IsTUGir())
		m_nBMSheetIdx[nIdx++]	= CONFINEMENT_CONCRETE;

	m_nTotSheetSu = nIdx;
}

void CAPlateBMXL::APlateOutBM(CXLControl	*pXL, long nOutIdx, CStringArray &ArrDelSheet)
{
	if(m_bAbort)	return;

	switch(nOutIdx)
	{
	case SLAB_TOTAL_SUMMARY :		// ������ ���� �Ѱ� ����ǥ
		TotalSummary(pXL, ArrDelSheet);
		break;
	case REBAR_TOTAL_SUMMARY :		// ö�� ���� �Ѱ� ����ǥ
		RebarTotalSummary(pXL, ArrDelSheet);
		break;
	case SLAB_SUMMARY :				// ������ ���� ����ǥ
		SlabSummary(pXL, ArrDelSheet);
		break;
	case REBAR_SUMMARY :			// ö�� ����ǥ
		RebarSummary(pXL, ArrDelSheet);
		break;
	case SLAB_CALC_FOUNDATION :			// ����ٰ�(������)
		SlabCalcFoundation(pXL, ArrDelSheet);
		break;
	case GUARDFENCE_CALC_FOUNDATION :	// ����ٰ�(��ȣ��)
		GuardCalcFoundation(pXL, ArrDelSheet);
		break;
	case CONFINEMENT_CONCRETE :			// ������ũ��Ʈ
		ConfinementConcrete(pXL, ArrDelSheet);
		break;
	}
}


CString CAPlateBMXL::GetNamePrintSheet(long nSheet)
{
	CString szSheetName;
	return szSheetName;
}


// �Ѱ� ����ǥ
void CAPlateBMXL::TotalSummary(CXLControl *pXL, CStringArray &ArrDelSheet)
{

}

// ö�� ���� �Ѱ� ����ǥ
void CAPlateBMXL::RebarTotalSummary(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	CAPlateOutBMFormat	*pOut	= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	COptionStd			*pOpt	= m_pAPlateOutBMStd->GetOptionStd();

	CString	szText	= _T("");
	CString	szRef	= _T("");
	long	nSttRow	= 1;

	pXL->SetActiveSheet("ö���Ѱ�����");
	ArrDelSheet.Add("ö���Ѱ�����");

	if(pOpt->m_nDeck_Slab_RebarTableOpt==0)
	{
		nSttRow = 50;
		pXL->DeleteRowLine(nSttRow, nSttRow+50);
		pXL->DeleteRowLineEnd();
	}
	else
	{
		szText.Format("������ ö�� ����ǥ (��ö��)");
		pOut->SetXL(pXL, szText,		nSttRow, "A", "H", 0, BLACK, 16, TRUE, TA_CENTER);

		nSttRow = 51;
		szText.Format("������ ö�� ����ǥ (��Ÿö��)");
		pOut->SetXL(pXL, szText,		nSttRow, "A", "H", 0, BLACK, 16, TRUE, TA_CENTER);
	}
}


// ö�� ���� ����ǥ
void CAPlateBMXL::RebarSummary(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();

	CString	sText		= _T("");
	CString	szSheetName	= _T("");

	pXL->SetActiveSheet("ö������ǥ");
	ArrDelSheet.Add("ö������ǥ");

	if(pOpt->m_nDeck_Slab_RebarTableOpt==0)
	{
		RebarSummarySlab(pXL);
		RebarSummaryGuardFence(pXL);
		if(pDB->IsTUGir())
			RebarSummaryConfinement(pXL);

		pXL->DeleteRowLine(50, 100);
		pXL->DeleteRowLineEnd();
	}
	else
	{
		RebarSummarySlabDivide(pXL);
		RebarSummaryGuardFenceDivide(pXL);
		if(pDB->IsTUGir())
			RebarSummaryConfinementDivide(pXL);
	}
	
	RebarSummaryWaterHole(pXL);
	RebarSummaryExpansionJoint(pXL);

	long nSttRow = 2;
	sText.Format("%s", m_pAPlateOutBMBrInfo->m_szBridgeDir);
	pOut->SetXL(pXL, sText,	nSttRow, "G", "H", 0, RED, 9, TRUE, TA_RIGHT, "", 0);

	if(pBMOpt->m_structRebarManufacture.m_nSlabType != 4)
	{
		sText.Format("%d", pBMOpt->m_structRebarManufacture.m_nSlabType+1);
		pOut->SetXL(pXL, sText,	nSttRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	if(pBMOpt->m_structRebarManufacture.m_nGuardFenceType != 4)
	{
		sText.Format("%d", pBMOpt->m_structRebarManufacture.m_nGuardFenceType+1);
		pOut->SetXL(pXL, sText,	nSttRow, "M", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);	
	}
	if(pBMOpt->m_structRebarManufacture.m_nBindConcType != 4 && pDB->IsTUGir())
	{
		sText.Format("%d", pBMOpt->m_structRebarManufacture.m_nBindConcType+1);
		pOut->SetXL(pXL, sText,	nSttRow, "N", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);	
	}
	if(pBMOpt->m_structRebarManufacture.m_nExpansionJointType != 4)
	{
		sText.Format("%d", pBMOpt->m_structRebarManufacture.m_nExpansionJointType+1);
		if(pDB->IsTUGir())
			pOut->SetXL(pXL, sText,	nSttRow, "O", "O", 0, RED, 9, FALSE, TA_CENTER, "", 0);		
		else
			pOut->SetXL(pXL, sText,	nSttRow, "N", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);		
	}

	pXL->SetXL("L13", " ");	pXL->SetXL("N13", " ");	pXL->SetXL("P13", " ");	pXL->SetXL("R13", " ");
	pXL->SetXL("L14", " ");	pXL->SetXL("N14", " ");	pXL->SetXL("P14", " ");	pXL->SetXL("R14", " ");

	CString	sCell1		= _T("");
	CString	sCell2		= _T("");
	CString	sCol		= _T("");
	long	nRowH	= 13;
	long	nRowD	= 14;

	// ������
	if(pBMOpt->m_structRebarManufacture.m_nSlabType == 0)	sCol	= _T("L");
	if(pBMOpt->m_structRebarManufacture.m_nSlabType == 1)	sCol	= _T("N");
	if(pBMOpt->m_structRebarManufacture.m_nSlabType == 2)	sCol	= _T("P");
	if(pBMOpt->m_structRebarManufacture.m_nSlabType == 3)	sCol	= _T("R");
	if(m_bSlabRebar[0])
	{
		sCell1.Format("%s%d", sCol, nRowH);
		sText = pXL->GetXL(sCell1);
		if(sText == _T(" "))	sText	= _T("������");
		else					sText	+= _T("+������");
		pXL->SetXL(sCell1, sText);
	}
	if(m_bSlabRebar[1])
	{
		sCell1.Format("%s%d", sCol, nRowD);
		sText = pXL->GetXL(sCell1);
		if(sText == _T(" "))	sText	= _T("������");
		else					sText	+= _T("+������");
		pXL->SetXL(sCell1, sText);
	}
	// ��ȣ��
	if(pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 0)	sCol	= _T("L");
	if(pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 1)	sCol	= _T("N");
	if(pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 2)	sCol	= _T("P");
	if(pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 3)	sCol	= _T("R");
	if(m_bGuardFenceRebar[0])
	{
		sCell1.Format("%s%d", sCol, nRowH);
		sText = pXL->GetXL(sCell1);
		if(sText == _T(" "))	sText	= _T("��ȣ��");
		else					sText	+= _T("+��ȣ��");
		pXL->SetXL(sCell1, sText);
	}
	if(m_bGuardFenceRebar[1])
	{
		sCell1.Format("%s%d", sCol, nRowD);
		sText = pXL->GetXL(sCell1);
		if(sText == _T(" "))	sText	= _T("��ȣ��");
		else					sText	+= _T("+��ȣ��");
		pXL->SetXL(sCell1, sText);
	}
	// ��������
	if(pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 0)	sCol	= _T("L");
	if(pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 1)	sCol	= _T("N");
	if(pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 2)	sCol	= _T("P");
	if(pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 3)	sCol	= _T("R");
	if(m_bExpansionJointRebar[0])
	{
		sCell1.Format("%s%d", sCol, nRowH);
		sText = pXL->GetXL(sCell1);
		if(sText == _T(" "))	sText	= _T("��������");
		else					sText	+= _T("+��������");
		pXL->SetXL(sCell1, sText);
	}
	if(m_bExpansionJointRebar[1])
	{
		sCell1.Format("%s%d", sCol, nRowD);
		sText = pXL->GetXL(sCell1);
		if(sText == _T(" "))	sText	= _T("��������");
		else					sText	+= _T("+��������");
		pXL->SetXL(sCell1, sText);
	}
	if(pDB->IsTUGir())
	{
		if(pBMOpt->m_structRebarManufacture.m_nBindConcType == 0)	sCol	= _T("L");
		if(pBMOpt->m_structRebarManufacture.m_nBindConcType == 1)	sCol	= _T("N");
		if(pBMOpt->m_structRebarManufacture.m_nBindConcType == 2)	sCol	= _T("P");
		if(pBMOpt->m_structRebarManufacture.m_nBindConcType == 3)	sCol	= _T("R");
		
		if(m_bConfinementConc[0])
		{
			sCell1.Format("%s%d", sCol, nRowH);
			sText = pXL->GetXL(sCell1);
			if(sText == _T(" "))	sText	= _T("������ũ��Ʈ");
			else					sText	+= _T("+������ũ��Ʈ");
			pXL->SetXL(sCell1, sText);
		}
		if(m_bConfinementConc[1])
		{
			sCell1.Format("%s%d", sCol, nRowD);
			sText = pXL->GetXL(sCell1);
			if(sText == _T(" "))	sText	= _T("������ũ��Ʈ");
			else					sText	+= _T("+������ũ��Ʈ");
			pXL->SetXL(sCell1, sText);
		}
	}
}

// ö�� ���� ������ (ö����ü)
void CAPlateBMXL::RebarSummarySlab(CXLControl *pXL)
{
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();	
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();
	CRebar				*pRB			= NULL;

	pADeckData->ShellSortRebarList(&pADeckData->m_arrRebarJewon, &CRebar::CompareForTable);
	m_nRebarColIdx = 0;

	CString	sText		= _T("");
	long	nSttRow		= 2;
	long	nSize		= pADeckData->m_arrRebarJewon.GetSize();
	long	nDiaIdx		= 0;
	CString	szDia		= _T("");
	CString szCol		= m_szRebarCol.GetAt(m_nRebarColIdx);
	double	dValue		= 0;
	double	dLength		= 0;
	double	dLengthSum	= 0;
	double	dWeight		= 0;
	double	dWeightUnit	= 0;
	double	dWeightSum	= 0;
	CMap	<CString, LPCTSTR, double, double> MapWeightTot;

	for(long n=0; n<nSize; n++)
	{

		pRB			= pADeckData->m_arrRebarJewon.GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength * (double)pRB->m_nEa;
		dWeightSum	= toTon(dWeightUnit  * dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bSlabRebar[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bSlabRebar[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= dWeightSum;
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}

	sText.Format("(SD40)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "A", 0, RED, 9, TRUE);
	sText.Format("%s����", m_pAPlateOutBMBrInfo->m_szBridgeDir);
	pOut->SetXL(pXL, sText,		nSttRow, "L", "L", 0, RED, 9, TRUE, TA_CENTER);
	nSttRow++;
	pOut->SetXL(pXL, "������",		nSttRow, szCol, szCol, 0, BLACK, 9, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);
	m_nRebarColIdx++;
}


// ö�� ���� ������ (ö�ٺи�)
void CAPlateBMXL::RebarSummarySlabDivide(CXLControl *pXL)
{
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();	
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();
	CRebar				*pRB			= NULL;
	
	CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarMain = new CSafetyTypedPtrArray<CObArray, CRebar*>;
	CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarEtc  = new CSafetyTypedPtrArray<CObArray, CRebar*>;

	// ��ö��
	MakeRebarList(1, pArrRebarMain);

	pADeckData->ShellSortRebarList(pArrRebarMain, &CRebar::CompareForTable);
	m_nRebarColIdx = 0;

	CString	sText		= _T("");
	long	nSttRow		= 1;
	long	nSize		= pArrRebarMain->GetSize();
	long	nDiaIdx		= 0;
	CString	szDia		= _T("");
	CString szCol		= m_szRebarCol.GetAt(m_nRebarColIdx);
	double	dValue		= 0;
	double	dLength		= 0;
	double	dLengthSum	= 0;
	double	dWeight		= 0;
	double	dWeightUnit	= 0;
	double	dWeightSum	= 0;
	long	n			= 0;

	CMap	<CString, LPCTSTR, double, double> MapWeightTot;
	MapWeightTot.RemoveAll();

	for(n=0; n<nSize; n++)
	{

		pRB			= pArrRebarMain->GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength * (double)pRB->m_nEa;
		dWeightSum	= toTon(dWeightUnit  * dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bSlabRebar[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bSlabRebar[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= dWeightSum;
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}
	sText.Format("������ ö�� ����ǥ (��ö��)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "H", 0, BLACK, 16, TRUE, TA_CENTER);
	sText.Format("ö�� �и�");
	pOut->SetXL(pXL, sText,		nSttRow, "O", "O", 0, RED, 9, TRUE);
	nSttRow++;
	sText.Format("(SD40)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "A", 0, RED, 9, TRUE);
	sText.Format("%s����", m_pAPlateOutBMBrInfo->m_szBridgeDir);
	pOut->SetXL(pXL, sText,		nSttRow, "L", "L", 0, RED, 9, TRUE, TA_CENTER);
	nSttRow++;
	pOut->SetXL(pXL, "������",		nSttRow, szCol, szCol, 0, BLACK, 9, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);

	// ��Ÿö��
	MakeRebarList(2, pArrRebarEtc);

	pADeckData->ShellSortRebarList(pArrRebarEtc, &CRebar::CompareForTable);
	
	nSttRow		= 51;
	nSize		= pArrRebarEtc->GetSize();
	MapWeightTot.RemoveAll();

	for(n=0; n<nSize; n++)
	{
		pRB			= pArrRebarEtc->GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength * (double)pRB->m_nEa;
		dWeightSum	= toTon(dWeightUnit  * dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bSlabRebar[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bSlabRebar[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= dWeightSum;
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}

	sText.Format("������ ö�� ����ǥ (��Ÿö��)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "H", 0, BLACK, 16, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);
	m_nRebarColIdx++;

}


// ö�� ���� ��ȣ��
void CAPlateBMXL::RebarSummaryGuardFence(CXLControl *pXL)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();
	CRebar				*pRB			= NULL;
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;

	pADeckData->ShellSortRebarList(&pADeckData->m_arrRebarJewonGuard, &CRebar::CompareForTable);

	CString	sText		= _T("");
	long	nSttRow		= 2;
	long	nSize		= pADeckData->m_arrRebarJewonGuard.GetSize();
	long	nDiaIdx		= 0;
	CString	szDia		= _T("");
	CString szCol		= m_szRebarCol.GetAt(m_nRebarColIdx);
	double	dValue		= 0;
	double	dLength		= 0;
	double	dLengthSum	= 0;
	double	dWeight		= 0;
	double	dWeightUnit	= 0;
	double	dWeightSum	= 0;
	double	dBRLength	= toM(pADeckData->m_nTypeGuardCalc ? pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT) : 1000.0);
	CMap	<CString, LPCTSTR, double, double> MapWeightTot;
	
	for(long n=0; n<nSize; n++)
	{

		pRB			= pADeckData->m_arrRebarJewonGuard.GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength*pRB->m_dEa;
		dWeightSum	= toTon(dWeightUnit*dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bGuardFenceRebar[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bGuardFenceRebar[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= Round(dWeightSum, 3);
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}

	nSttRow++;
	pOut->SetXL(pXL, "��ȣ��",	nSttRow, szCol, szCol, 0, BLACK, 9, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, dBRLength);
	m_nRebarColIdx++;

}

void CAPlateBMXL::RebarSummaryConfinement(CXLControl *pXL)
{
	CADeckDataTU		*pADeckDataTU	= m_pAPlateOutBMStd->GetDataManage()->GetDeckDataTU();	
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();	
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();
	CRebar				*pRB			= NULL;

	pADeckData->ShellSortRebarList(&pADeckDataTU->m_arrRebarJewonTU, &CRebar::CompareForTable);

	CString	sText		= _T("");
	long	nSttRow		= 2;
	long	nSize		= pADeckDataTU->m_arrRebarJewonTU.GetSize();
	long	nDiaIdx		= 0;
	CString	szDia		= _T("");
	CString szCol		= m_szRebarCol.GetAt(m_nRebarColIdx);
	double	dValue		= 0;
	double	dLength		= 0;
	double	dLengthSum	= 0;
	double	dWeight		= 0;
	double	dWeightUnit	= 0;
	double	dWeightSum	= 0;
	CMap	<CString, LPCTSTR, double, double> MapWeightTot;

	for(long n=0; n<nSize; n++)
	{
		pRB			= pADeckDataTU->m_arrRebarJewonTU.GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength * (double)pRB->m_nEa;
		dWeightSum	= toTon(dWeightUnit  * dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bConfinementConc[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bConfinementConc[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= dWeightSum;
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}

	sText.Format("(SD40)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "A", 0, RED, 9, TRUE);
	sText.Format("%s����", m_pAPlateOutBMBrInfo->m_szBridgeDir);
	pOut->SetXL(pXL, sText,		nSttRow, "L", "L", 0, RED, 9, TRUE, TA_CENTER);
	nSttRow++;
	pOut->SetXL(pXL, "������ũ��Ʈ",		nSttRow, szCol, szCol, 0, BLACK, 9, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);
	m_nRebarColIdx++;

	nSttRow = 51;
	pXL->DeleteRowLine(nSttRow, nSttRow+50);
	pXL->DeleteRowLineEnd();
}

void CAPlateBMXL::RebarSummaryConfinementDivide(CXLControl *pXL)
{
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CADeckDataTU		*pADeckDataTU	= m_pAPlateOutBMStd->GetDataManage()->GetDeckDataTU();
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();
	CRebar				*pRB			= NULL;
	
	CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarMain = new CSafetyTypedPtrArray<CObArray, CRebar*>;
	CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarEtc  = new CSafetyTypedPtrArray<CObArray, CRebar*>;

	// ��ö��
	MakeRebarList(1, pArrRebarMain);
	pADeckData->ShellSortRebarList(pArrRebarMain, &CRebar::CompareForTable);

	CString	sText		= _T("");
	long	nSttRow		= 1;
	long	nSize		= pArrRebarMain->GetSize();
	long	nDiaIdx		= 0;
	long	n			= 0;
	CString	szDia		= _T("");
	CString szCol		= m_szRebarCol.GetAt(m_nRebarColIdx);
	double	dValue		= 0;
	double	dLength		= 0;
	double	dLengthSum	= 0;
	double	dWeight		= 0;
	double	dWeightUnit	= 0;
	double	dWeightSum	= 0;
	CMap	<CString, LPCTSTR, double, double> MapWeightTot;
	MapWeightTot.RemoveAll();

	for(n=0; n<nSize; n++)
	{

		pRB			= pArrRebarMain->GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength * (double)pRB->m_nEa;
		dWeightSum	= toTon(dWeightUnit  * dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bConfinementConc[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bConfinementConc[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= dWeightSum;
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}
	sText.Format("������ ö�� ����ǥ (��ö��)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "H", 0, BLACK, 16, TRUE, TA_CENTER);
	sText.Format("ö�� �и�");
	pOut->SetXL(pXL, sText,		nSttRow, "O", "O", 0, RED, 9, TRUE);
	nSttRow++;
	sText.Format("(SD40)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "A", 0, RED, 9, TRUE);
	sText.Format("%s����", m_pAPlateOutBMBrInfo->m_szBridgeDir);
	pOut->SetXL(pXL, sText,		nSttRow, "L", "L", 0, RED, 9, TRUE, TA_CENTER);
	nSttRow++;
	pOut->SetXL(pXL, "������",		nSttRow, szCol, szCol, 0, BLACK, 9, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);

	// ��Ÿö��
	MakeRebarList(2, pArrRebarEtc);
	pADeckData->ShellSortRebarList(pArrRebarEtc, &CRebar::CompareForTable);

	nSttRow		= 51;
	nSize		= pArrRebarEtc->GetSize();
	MapWeightTot.RemoveAll();

	for(n=0; n<nSize; n++)
	{
		pRB			= pArrRebarEtc->GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength * (double)pRB->m_nEa;
		dWeightSum	= toTon(dWeightUnit  * dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bConfinementConc[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bConfinementConc[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= dWeightSum;
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}

	sText.Format("������ ö�� ����ǥ (��Ÿö��)");
	pOut->SetXL(pXL, sText,		nSttRow, "A", "H", 0, BLACK, 16, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);
	m_nRebarColIdx++;
}

// ö�� ���� ��ȣ��(ö�ٺи�)
void CAPlateBMXL::RebarSummaryGuardFenceDivide(CXLControl *pXL)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();
	CRebar				*pRB			= NULL;
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;
		
	CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarMain = new CSafetyTypedPtrArray<CObArray, CRebar*>;
	CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebarEtc  = new CSafetyTypedPtrArray<CObArray, CRebar*>;

	// ��ö��
	MakeRebarListGuard(1, pArrRebarMain);
	
	pADeckData->ShellSortRebarList(pArrRebarMain, &CRebar::CompareForTable);

	CString	sText		= _T("");
	long	nSttRow		= 2;
	long	nSize		= pArrRebarMain->GetSize();
	long	nDiaIdx		= 0;
	long	n			= 0;
	CString	szDia		= _T("");
	CString szCol		= m_szRebarCol.GetAt(m_nRebarColIdx);
	double	dValue		= 0;
	double	dLength		= 0;
	double	dLengthSum	= 0;
	double	dWeight		= 0;
	double	dWeightUnit	= 0;
	double	dWeightSum	= 0;
	double	dBRLength	= toM(pADeckData->m_nTypeGuardCalc ? pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT) : 1000.0);
	CMap	<CString, LPCTSTR, double, double> MapWeightTot;
	MapWeightTot.RemoveAll();
	
	for(n=0; n<nSize; n++)
	{

		pRB			= pArrRebarMain->GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength*pRB->m_dEa;
		dWeightSum	= toTon(dWeightUnit*dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bGuardFenceRebar[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bGuardFenceRebar[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= Round(dWeightSum, 3);
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}

	nSttRow++;
	pOut->SetXL(pXL, "��ȣ��",	nSttRow, szCol, szCol, 0, BLACK, 9, TRUE, TA_CENTER);
	nSttRow+=2;

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, dBRLength);

	// ��Ÿö��
	MakeRebarListGuard(2, pArrRebarEtc);
	
	pADeckData->ShellSortRebarList(pArrRebarEtc, &CRebar::CompareForTable);

	nSttRow		= 55;
	nSize		= pArrRebarEtc->GetSize();
	MapWeightTot.RemoveAll();
	
	for(n=0; n<nSize; n++)
	{

		pRB			= pArrRebarEtc->GetAt(n);
		nDiaIdx		= GetDiaIdx(pRB->GetStringDia());
		dWeightUnit	= pADeckData->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength		= Round(toM(pRB->GetLength()),3);
		dLengthSum	= dLength*pRB->m_dEa;
		dWeightSum	= toTon(dWeightUnit*dLengthSum);

		if(nDiaIdx > 0 && nDiaIdx < 8)	m_bGuardFenceRebar[0]	= TRUE;
		if(nDiaIdx > 7 && nDiaIdx < 17)	m_bGuardFenceRebar[1]	= TRUE;

		switch(nDiaIdx)
		{
		case DIA_H13:
			dWeight	= MapWeightTot["H13"];
			dWeight	+= dWeightSum;
			MapWeightTot["H13"] = dWeight;
			break;
		case DIA_H16 :
			dWeight	= MapWeightTot["H16"];
			dWeight	+= Round(dWeightSum, 3);
			MapWeightTot["H16"] = dWeight;
			break;
		case DIA_H19 :
			dWeight	= MapWeightTot["H19"];
			dWeight	+= dWeightSum;
			MapWeightTot["H19"] = dWeight;
			break;
		case DIA_H22 :
			dWeight	= MapWeightTot["H22"];
			dWeight	+= dWeightSum;
			MapWeightTot["H22"] = dWeight;
			break;
		case DIA_H25 :
			dWeight	= MapWeightTot["H25"];
			dWeight	+= dWeightSum;
			MapWeightTot["H25"] = dWeight;
			break;
		case DIA_H29 :
			dWeight	= MapWeightTot["H29"];
			dWeight	+= dWeightSum;
			MapWeightTot["H29"] = dWeight;
			break;
		case DIA_H32 :
			dWeight	= MapWeightTot["H32"];
			dWeight	+= dWeightSum;
			MapWeightTot["H32"] = dWeight;
			break;
		case DIA_H35 :
			dWeight	= MapWeightTot["H35"];
			dWeight	+= dWeightSum;
			MapWeightTot["H35"] = dWeight;
			break;
		case DIA_D10:
			dWeight	= MapWeightTot["D10"];
			dWeight	+= dWeightSum;
			MapWeightTot["D10"] = dWeight;
			break;
		case DIA_D13:
			dWeight	= MapWeightTot["D13"];
			dWeight	+= dWeightSum;
			MapWeightTot["D13"] = dWeight;
			break;
		case DIA_D16 :
			dWeight	= MapWeightTot["D16"];
			dWeight	+= dWeightSum;
			MapWeightTot["D16"] = dWeight;
			break;
		case DIA_D19 :
			dWeight	= MapWeightTot["D19"];
			dWeight	+= dWeightSum;
			MapWeightTot["D19"] = dWeight;
			break;
		case DIA_D22 :
			dWeight	= MapWeightTot["D22"];
			dWeight	+= dWeightSum;
			MapWeightTot["D22"] = dWeight;
			break;
		case DIA_D25 :
			dWeight	= MapWeightTot["D25"];
			dWeight	+= dWeightSum;
			MapWeightTot["D25"] = dWeight;
			break;
		case DIA_D29 :
			dWeight	= MapWeightTot["D29"];
			dWeight	+= dWeightSum;
			MapWeightTot["D29"] = dWeight;
			break;
		case DIA_D32 :
			dWeight	= MapWeightTot["D32"];
			dWeight	+= dWeightSum;
			MapWeightTot["D32"] = dWeight;
			break;
		case DIA_D35 :
			dWeight	= MapWeightTot["D35"];
			dWeight	+= dWeightSum;
			MapWeightTot["D35"] = dWeight;
			break;
		}
	}

	WriteRebarSummary(pXL, MapWeightTot, nSttRow, dBRLength);
	
	m_nRebarColIdx++;
}

// ö�� ���� ����� ����ö��
void CAPlateBMXL::RebarSummaryWaterHole(CXLControl *pXL)
{
	// ������ ��ٿ� ����
	// �ɼ� ���� �ʿ�
}

// ö�� ���� ��������
void CAPlateBMXL::RebarSummaryExpansionJoint(CXLControl *pXL)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	COptionStd			*pOpt			= m_pAPlateOutBMStd->GetOptionStd();
	CRebar				*pRB			= NULL;
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;	
	
	if(!pBMOpt->m_structExpansionJoint.m_bApply) return;

	BOOL	bUnitLength	= pBMOpt->m_structExpansionJoint.m_bUnitLength;
	BOOL	bEqSttEnd	= FALSE;
	long	nSttRow		= 3;
	long	nSttDiaIdx	= 0;
	long	nEndDiaIdx	= 0;
	double	dSigmaY		= toKgPCm2(pADeckData->m_SigmaY);
	double	dSttWeight	= 0;
	double	dEndWeight	= 0;
	double	dTotWeight	= 0;
	double	dWeight		= 0;
	CString	sText		= _T("");
	CString szCol		= m_szRebarCol.GetAt(m_nRebarColIdx);
	CString szSttDia	= _T("");
	CString szEndDia	= _T("");
	CMap	<CString, LPCTSTR, double, double> MapWeightTot;

	pOut->SetXL(pXL, "��������", nSttRow, szCol, szCol, 0, BLACK, 9, TRUE, TA_CENTER);

	if(!bUnitLength)
	{
		for(long nExpJoint = 0; nExpJoint < BMOPT_EXPANSIONJOINT_MAX; nExpJoint++)
		{
			bEqSttEnd	= FALSE;
			szSttDia	= pBMOpt->GetRebarDia(pBMOpt->m_structExpansionJoint.m_nSttDiaIdx[nExpJoint], dSigmaY);
			szEndDia	= pBMOpt->GetRebarDia(pBMOpt->m_structExpansionJoint.m_nEndDiaIdx[nExpJoint], dSigmaY);
			nSttDiaIdx	= GetDiaIdx(szSttDia);
			nEndDiaIdx	= GetDiaIdx(szEndDia);
			dSttWeight	= toTon(pBMOpt->m_structExpansionJoint.m_dSttWeight[nExpJoint]);
			dEndWeight	= toTon(pBMOpt->m_structExpansionJoint.m_dEndWeight[nExpJoint]);
			if(szSttDia == szEndDia)
			{
				bEqSttEnd	= TRUE;
				dTotWeight	= dSttWeight + dEndWeight;
			}
				
			if(nSttDiaIdx > 0 && nSttDiaIdx < 8)	m_bExpansionJointRebar[0]	= TRUE;
			if(nSttDiaIdx > 7 && nSttDiaIdx < 17)	m_bExpansionJointRebar[1]	= TRUE;
			switch(nSttDiaIdx)
			{
			case DIA_H13 :
				dWeight				= MapWeightTot["H13"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H13"]	= dWeight;
				break;
			case DIA_H16 :
				dWeight				= MapWeightTot["H16"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H16"]	= dWeight;
				break;
			case DIA_H19 :
				dWeight				= MapWeightTot["H19"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H19"]	= dWeight;
				break;
			case DIA_H22 :
				dWeight				= MapWeightTot["H22"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H22"]	= dWeight;
				break;
			case DIA_H25 :
				dWeight				= MapWeightTot["H25"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H25"]	= dWeight;
				break;
			case DIA_H29 :
				dWeight				= MapWeightTot["H29"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H29"]	= dWeight;
				break;
			case DIA_H32 :
				dWeight				= MapWeightTot["H32"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H32"]	= dWeight;
				break;
			case DIA_H35 :
				dWeight				= MapWeightTot["H35"];
				dWeight				+= bEqSttEnd ? dTotWeight : dSttWeight;
				MapWeightTot["H35"]	= dWeight;
				break;
			}
			nSttRow = 5;
			WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);
			if(bEqSttEnd)	continue;

			switch(nEndDiaIdx)
			{
			case DIA_H13 :
				dWeight				= MapWeightTot["H13"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H13"]	= dWeight;
				break;
			case DIA_H16 :
				dWeight				= MapWeightTot["H16"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H16"]	= dWeight;
				break;
			case DIA_H19 :
				dWeight				= MapWeightTot["H19"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H19"]	= dWeight;
				break;
			case DIA_H22 :
				dWeight				= MapWeightTot["H25"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H22"]	= dWeight;
				break;
			case DIA_H25 :
				dWeight				= MapWeightTot["H29"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H25"]	= dWeight;
				break;
			case DIA_H29 :
				dWeight				= MapWeightTot["H13"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H29"]	= dWeight;
				break;
			case DIA_H32 :
				dWeight				= MapWeightTot["H13"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H32"]	= dWeight;
				break;
			case DIA_H35 :
				dWeight				= MapWeightTot["H13"];
				dWeight				+= bEqSttEnd ? dTotWeight : dEndWeight;
				MapWeightTot["H35"]	= dWeight;
				break;
			}
			nSttRow = 5;
			WriteRebarSummary(pXL, MapWeightTot, nSttRow, 1);
		}
		m_nRebarColIdx++;
	}
	else
	{
	
	}
}


void CAPlateBMXL::WriteRebarSummary(CXLControl *pXL, CMap<CString, LPCTSTR, double, double> &MapWeightTot, long &nSttRow, double dBrLength)
{
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CAddRate			AddRate			= pADeckData->m_AddRateRebar;
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	
	CString szCol	= m_szRebarCol.GetAt(m_nRebarColIdx);
	double	dValue	= 0;
	CString szText	= _T(""), szCell = _T("");

	dValue	= MapWeightTot["H35"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(35)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["H32"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(32)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["H29"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(29)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["H25"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(25)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["H22"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(22)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;
	
	dValue	= MapWeightTot["H19"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(19)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["H16"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(16)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["H13"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(13)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["H10"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(10)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow+=4;

	// D
	nSttRow+=2;
	dValue	= MapWeightTot["D35"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(35)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D32"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(32)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D29"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(29)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D25"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(25)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D22"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(22)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D19"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(19)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D16"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(16)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D13"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(13)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;

	dValue	= MapWeightTot["D10"]*dBrLength;
	if(dValue != 0)
	{
		pOut->SetXL(pXL, dValue,		nSttRow, szCol, szCol, 0, RED, 8, FALSE, TA_LEFT, "", 3);
		nSttRow++;
	}
	else
		nSttRow++;

	szText.Format("%.2f", (AddRate.GetAddRateFromDia(10)-1)*100);
	szCell.Format("H%d", nSttRow);
	pXL->SetXL(szCell, szText);
	nSttRow++;
}

// ������ ���� ����ǥ
void CAPlateBMXL::SlabSummary(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CAPlateOutBMBrInfo	*pBrInfo		= GetBridgeInfo(0);
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;
	CCalcData			*pData			= m_pAPlateOutBMStd->GetDataManage()->GetCalcData();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutBMStd->GetGeneralInputStd()->m_pCalcStd;
	CCalcFloor			CalcFloor(m_pAPlateOutBMStd->GetDataManage());

	CString	sText	= _T("");
	CString	sCell	= _T("");

	pXL->SetActiveSheet("����������ǥ");
	ArrDelSheet.Add("����������ǥ");

	long	nSttRow = 3, nTemp=4;
	BOOL	bGuard	= pDB->GetGuardWallSu() > 0 ? TRUE : FALSE;
	double	dSigmaY	= pADeckData->m_SigmaY;

	if(pDB->IsTUGir())
	{
		pOut->SetXL(pXL, "������ũ��Ʈ",	nSttRow, "G", "G", 0, BLACK, 10, TRUE, TA_CENTER);		
		sText.Format("25-%.0f-15", pData->DESIGN_MATERIAL.m_dSigmaCK);
		pOut->SetXL(pXL, sText,	nTemp, "C", "C", 0, BLACK, 10, TRUE, TA_CENTER);
		nTemp++;
	}
	pOut->SetXL(pXL, "������",	nSttRow, "E", "E", 0, BLACK, 10, TRUE, TA_CENTER);
	sText.Format("25-%.0f-15", pData->DESIGN_MATERIAL.m_dSigmaCK);
	pOut->SetXL(pXL, sText,	nTemp, "C", "C", 0, BLACK, 10, TRUE, TA_CENTER);
	nTemp++;
	if(bGuard)
	{
		pOut->SetXL(pXL, "��ȣ��",	nSttRow, "F", "F", 0, BLACK, 10, TRUE, TA_CENTER);		
		sText.Format("19-%.0f-15", pDB->GetGuardFence(0)->GetRebar()->GetFck()/10);
		pOut->SetXL(pXL, sText,	nTemp, "C", "C", 0, BLACK, 10, TRUE, TA_CENTER);
	}

	if(pDB->IsTUGir())	nSttRow = 5;
	else				nSttRow = 4;
	// ��ũ��Ʈ ����
	if(pBMOpt->m_bConcreteBuy)
	{
		if(bGuard)
		{
			nSttRow++;
			if(m_bGuardFenceRebar[1] == TRUE)
				pOut->SetXL(pXL, m_MapGuardCell["CON_BUY"], nSttRow, "F", "F", 0, BLACK, 9, TRUE, TA_CENTER);
		}
	}
	else	pXL->SetCellHeight(nSttRow-1, nSttRow, 0);

	// ��ũ��Ʈ Ÿ��
	if(pDB->IsTUGir())	nSttRow = 7;
	else				nSttRow = 6;
	if(pBMOpt->m_bConcretePlacing)
	{
		pOut->SetXL(pXL, m_MapGuardCell["CON_PLC"],	nSttRow, "F", "F", 0, BLACK, 9, TRUE, TA_CENTER);
	}
	else	pXL->SetCellHeight(nSttRow-1, nSttRow, 0);
	// ��Ǫ��
	if(pDB->IsTUGir())	nSttRow = 12;
	else				nSttRow = 11;
	if(pBMOpt->m_bMold)
	{
		if(pADeckData->m_nTypeBMApply==0)
		{
			long nRowTemp	= nSttRow;
			pOut->SetXL(pXL, "����3ȸ",		nRowTemp, "B", "B", 0, BLACK, 9, TRUE, TA_CENTER);
			nRowTemp += 3;
			pOut->SetXL(pXL, "���̰�Ǫ��",	nRowTemp, "B", "B", 0, BLACK, 9, TRUE, TA_CENTER);
			nRowTemp++;
			pOut->SetXL(pXL, "����4ȸ",		nRowTemp, "B", "B", 0, BLACK, 9, TRUE, TA_CENTER);
			nRowTemp++;
			pOut->SetXL(pXL, "����6ȸ",		nRowTemp, "B", "B", 0, BLACK, 9, TRUE, TA_CENTER);
		}

		nSttRow++;
		pOut->SetXL(pXL, m_MapGuardCell["MOLD_SLOP"],	nSttRow, "F", "F", 0, BLACK, 9, TRUE, TA_CENTER);
		nSttRow++;
		pOut->SetXL(pXL, m_MapGuardCell["MOLD_VER"],	nSttRow, "F", "F", 0, BLACK, 9, TRUE, TA_CENTER);
	}
	else	pXL->SetCellHeight(nSttRow-1, nSttRow+4, 0);

	// ���ٸ�
	if(pDB->IsTUGir())	nSttRow = 18;
	else				nSttRow = 17;
	if(pADeckData->m_nTypeBMApply!=1)
	{
		if(!pBMOpt->m_structTimber.m_bApply)
			pXL->SetCellHeight(nSttRow-1, nSttRow, 0);
	}
	else		// ��������(��������и�)�� ��� : �̺κ��� ����� �Ʒ��ʿ� �и��Ͽ� ǥ��
	{
		pXL->SetCellHeight(nSttRow-1, nSttRow, 0);
	}

	// ����������ġ
	if(pDB->IsTUGir())	nSttRow = 20;
	else				nSttRow = 19;
	if(pBMOpt->m_structExpansionJoint.m_bApply)
	{
		CString szSttName = pCalcStd->m_pDesignExpanJoint->GetSize() ? pCalcStd->m_pDesignExpanJoint->GetExpansionJointData(0)->m_strExpanJoint : "";
		CString szEndName = pCalcStd->m_pDesignExpanJoint->GetSize() ? pCalcStd->m_pDesignExpanJoint->GetExpansionJointData(1)->m_strExpanJoint : "";
		sText.Format("%s", szSttName);
		pOut->SetXL(pXL, sText,	nSttRow, "C", "C", 0, BLACK, 9, TRUE, TA_CENTER);
		nSttRow++;
		sText.Format("%s", szEndName);
		pOut->SetXL(pXL, sText,	nSttRow, "C", "C", 0, BLACK, 9, TRUE, TA_CENTER);
	}
	else	pXL->SetCellHeight(nSttRow-1, nSttRow+3, 0);

	// ������ ��ũ��Ʈ
	if(pDB->IsTUGir())	nSttRow = 25;
	else				nSttRow = 24;
	if(!pBMOpt->m_structShrinkageConcrete.m_bApply)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);

	// ��������
	// �������
	// ������
	if(pDB->IsTUGir())	nSttRow = 28;
	else				nSttRow = 27;
	if(!pBMOpt->m_bDrainBridgeSurface)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// �����̼ż�ġ
	if(pDB->IsTUGir())	nSttRow = 27;
	else				nSttRow = 28;
	if(!pBMOpt->m_bSpacer)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// �ƽ�������
	if(pDB->IsTUGir())	nSttRow = 30;
	else				nSttRow = 29;
	if(pBMOpt->m_structPave.m_bApply)
	{
		sText.Format("( t = %.1f cm )", toCm(pBMOpt->m_structPave.m_dPaveThick));
		pOut->SetXL(pXL, sText,	nSttRow, "C", "C", 0, BLACK, 9, TRUE, TA_CENTER);
	}
	else	pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// ������
	if(pDB->IsTUGir())	nSttRow = 31;
	else				nSttRow = 30;
	if(pBMOpt->m_structTackCoating.m_bApply)
	{
		sText.Format("( %.1f �� )", pBMOpt->m_structTackCoating.m_dTackCoating);
		pOut->SetXL(pXL, sText,	nSttRow, "C", "C", 0, BLACK, 9, TRUE, TA_CENTER);
	}
	else	pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// ��Ƽ����
	if(pDB->IsTUGir())	nSttRow = 32;
	else				nSttRow = 31;
	if(pBMOpt->m_structStyrofoam.m_bApply)
	{
		sText.Format("( t = %.0f mm )", pBMOpt->m_structStyrofoam.m_dThick);
		pOut->SetXL(pXL, sText,	nSttRow, "C", "C", 0, BLACK, 9, TRUE, TA_CENTER);
	}
	else	pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	
	// ��ȣ��
	if(pDB->IsTUGir())	nSttRow = 33;
	else				nSttRow = 32;
	CCentSeparation	*pGuard	= NULL;
	long	nGuardsu	= pDB->GetGuardWallSu();
	long	nGuardIdx	= 0;
	long	nGuardMatch	= -1;
	CString	szSheetName	= _T("");
	for(long nGuard=0; nGuard<nGuardsu; nGuard++)
	{
		pGuard		= pDB->GetGuardFence(nGuard);
		szSheetName	= GetGuardFenceSheetName(nGuard);
		BOOL	bSameGuard	= IsSameGuardFence(nGuard, nGuardMatch);
		if(!bSameGuard || (bSameGuard && nGuard<nGuardMatch))
		{
			if(nGuardIdx !=0)
			{
				pXL->InsertRowLine(nSttRow+1, 1);
				nSttRow++;
			}
			sText.Format("%s%d", szSheetName, nGuardIdx+1);
			pOut->SetXL(pXL, sText, nSttRow, "A", "B", 0, BLACK, 9, TRUE, TA_CENTER);
			sText.Format("H=%.3f", toM(pGuard->GetHeight()));
			pOut->SetXL(pXL, sText,	nSttRow, "C", "C", 0, BLACK, 9, TRUE, TA_CENTER);
			pOut->SetXL(pXL, "m",	nSttRow, "D", "D", 0, BLACK, 9, TRUE, TA_CENTER);
			sCell.Format("%s%d", szSheetName, nGuardIdx+1);
			sText.Format("=%s", m_MapGuardCell[sCell]);
			pOut->SetXL(pXL, sText,	nSttRow, "E", "E", 0, BLACK, 9, TRUE, TA_CENTER);
			sText.Format("=SUM(E%d:F%d)", nSttRow, nSttRow);
			pOut->SetXL(pXL, sText,	nSttRow, "G", "G", 0, BLACK, 9, TRUE, TA_CENTER);
			nGuardIdx++;
		}
	}

	long nGuardTypeL = CalcFloor.GetGuardWallType(FLOOR_LEFT);
	long nGuardTypeR = CalcFloor.GetGuardWallType(FLOOR_RIGHT);
	BOOL bBangEmExistL = (pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && nGuardTypeL<=8) ? TRUE : FALSE;
	BOOL bBangEmExistR = (pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && nGuardTypeR<=8) ? TRUE : FALSE;

	// �����ǳ�
	nSttRow++;
	if(pBMOpt->m_structSoundProof.m_bApply)
	{
		if(!bBangEmExistL)
			pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
		if(!bBangEmExistR)
			pXL->SetCellHeight(nSttRow, nSttRow, 0);
	}
	else
	{
		pXL->SetCellHeight(nSttRow-1, nSttRow, 0);
	}
	// ������
	nSttRow+=2;
	if(!pBMOpt->m_structBridgeName.m_bApply)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// ������
	nSttRow++;
	if(!pBMOpt->m_structBridgeNamePlate.m_bApply)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// ������
	nSttRow++;
	if(!pBMOpt->m_structExplainPlate.m_bApply)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// TBM ��ġ
	nSttRow++;
	if(!pBMOpt->m_structEstablishTBM.m_bApply)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// NOTCH ��ġ
	nSttRow++;
	if(!pBMOpt->m_structNotch.m_bApply)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// ����ü�
	nSttRow++;
	if(pBMOpt->m_bDrainEstablish)
	{
		if(pBMOpt->m_nDrainEstablishType == RIVER)			// ��õ��
		{
			nSttRow+=2;
			pXL->SetCellHeight(nSttRow-1, nSttRow+15, 0);
			nSttRow+=17;
		}
		else if(pBMOpt->m_nDrainEstablishType == WALKWAY)	// ������
		{
			if(pBMOpt->m_structWalkWayDrainEstablish.m_nType==0)
			{
				pXL->SetCellHeight(nSttRow-1, nSttRow, 0);
				nSttRow+=7;
				pXL->SetCellHeight(nSttRow-1, nSttRow+10, 0);
				nSttRow+=12;
			}
			else
			{
				pXL->SetCellHeight(nSttRow-1, nSttRow+5, 0);
				nSttRow+=19;
			}
		}
		else if(pBMOpt->m_nDrainEstablishType == RIVERWALKWAY)	// ��õ �� ������
		{
			nSttRow+=2;
			if(pBMOpt->m_structWalkWayDrainEstablish.m_nType==0)
			{
				nSttRow+=5;
				pXL->SetCellHeight(nSttRow-1, nSttRow+10, 0);
				nSttRow+=12;
			}
			else
			{
				pXL->SetCellHeight(nSttRow-1, nSttRow+3, 0);
				nSttRow+=17;
			}
		}
	}
	else
	{
		pXL->SetCellHeight(nSttRow-1, nSttRow+17, 0);
		nSttRow += 19;
	}
	// ����
	// ��������
	nSttRow+=2;
	// ���鹰�����
	nSttRow+=8;
	if(!pBMOpt->m_bWaterDraw)
		pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
	// ���Ϲ�������
	nSttRow++;
	if(pBMOpt->m_structDroppingPrevent.m_bApply)
	{
		if(pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal)
			pXL->SetXL(nSttRow, 4, "0");
	}
	else
		pXL->SetCellHeight(nSttRow-1, nSttRow+1, 0);
	// ö�ٰ�������
	nSttRow+=3;
	if(pADeckData->m_nTypeBMApply!=0)			// �Ϲݼ��������� �ƴϸ�(���������̸�) ���溰�� �� ������
	{
		if(pBMOpt->m_structRebarManufacture.m_bApply)
		{
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 0 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 0 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 0))
				pXL->SetCellHeight(nSttRow-1, nSttRow+2, 0);
			nSttRow+=4;
			
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 1 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 1 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 1))
				pXL->SetCellHeight(nSttRow-1, nSttRow+2, 0);
			nSttRow+=4;
			
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 2 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 2 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 2))
				pXL->SetCellHeight(nSttRow-1, nSttRow+2, 0);
			nSttRow+=4;
			
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 3 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 3 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 3))
				pXL->SetCellHeight(nSttRow-1, nSttRow+2, 0);
			nSttRow+=4;
		}
		else
		{
			pXL->SetCellHeight(nSttRow-1, nSttRow+14, 0);
			nSttRow+=16;
		}
		// �հ踸 �����ִ°��� ����
		pXL->SetCellHeight(nSttRow-1, nSttRow+2, 0);
		nSttRow+=4;
	}
	else			// �Ϲݼ����̸� �հ踸 ������
	{
		pXL->SetCellHeight(nSttRow-1, nSttRow+14, 0);
		nSttRow+=16;

		if(pBMOpt->m_structRebarManufacture.m_bApply)
		{
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 0 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 0 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 0))
				pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
			nSttRow++;
			
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 1 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 1 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 1))
				pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
			nSttRow++;
			
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 2 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 2 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 2))
				pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
			nSttRow++;
			
			if(!(pBMOpt->m_structRebarManufacture.m_nSlabType == 3 || pBMOpt->m_structRebarManufacture.m_nGuardFenceType == 3 || pBMOpt->m_structRebarManufacture.m_nExpansionJointType == 3))
				pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
			nSttRow++;
		}
		else
		{
			pXL->SetCellHeight(nSttRow-1, nSttRow+2, 0);
			nSttRow+=4;
		}
	}
	
	// ������������
	if(pBMOpt->m_structExpansionJointCover.m_bApply)
	{
		for(long n=0; n<7; n++)
		{
			if(pBMOpt->m_structExpansionJointCover.m_nQty[n] == 0)
				pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
			nSttRow++;
		}
	}
	else
	{
		pXL->SetCellHeight(nSttRow-1, nSttRow+6, 0);
		nSttRow+=7;
	}
	// ����
	if(pBMOpt->m_structParapet.m_bApply)
	{
		if(pBMOpt->m_structParapet.m_dGuardLength[0]==0 && pBMOpt->m_structParapet.m_dGuardLength[1]==0)
			pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
		nSttRow++;
		if(pBMOpt->m_structParapet.m_dCurbLength*pBMOpt->m_structParapet.m_nCurbQty == 0)
			pXL->SetCellHeight(nSttRow-1, nSttRow-1, 0);
		CString sCell1;
		CString sCell2;
		sCell1.Format("A%d", nSttRow+1);
		sCell2.Format("B%d", nSttRow+1);
		pXL->CellLine(sCell1, sCell2, 3);
		nSttRow++;
	}
	else
	{
		pXL->SetCellHeight(nSttRow-1, nSttRow, 0);
		nSttRow+=2;
	}
	// ������
	if(pBMOpt->m_structElecWireHole.m_bApply)
	{
		if(pBMOpt->m_structElecWireHole.m_nEA100[0]==0 && pBMOpt->m_structElecWireHole.m_nEA100[1]==0)
		{	
			pXL->DeleteRowLine(nSttRow, nSttRow);
			pXL->DeleteRowLineEnd();

			pOut->SetXL(pXL, "������", nSttRow, "A", "A", 0, BLACK, 9, TRUE, TA_CENTER);
		}
		else
			nSttRow++;
			
		if(pBMOpt->m_structElecWireHole.m_nEA125[0]==0 && pBMOpt->m_structElecWireHole.m_nEA125[1]==0)
		{	
			pXL->DeleteRowLine(nSttRow, nSttRow);
			pXL->DeleteRowLineEnd();

			pOut->SetXL(pXL, "������", nSttRow, "A", "A", 0, BLACK, 9, TRUE, TA_CENTER);
		}
		else
			nSttRow++;

		if(pBMOpt->m_structElecWireHole.m_nEA150[0]==0 && pBMOpt->m_structElecWireHole.m_nEA150[1]==0)
		{	
			pXL->DeleteRowLine(nSttRow, nSttRow);
			pXL->DeleteRowLineEnd();
		}
		else
			nSttRow++;
	}
	else
	{
		pXL->SetCellHeight(nSttRow-1, nSttRow+1, 0);
		nSttRow+=3;
	}
	
	// ������� ����
	nSttRow+=4;
	if(pADeckData->m_nTypeBMApply!=1)
		pXL->SetCellHeight(nSttRow-1, nSttRow+3, 0);

}


// ����ٰ�(������)
void CAPlateBMXL::SlabCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet)
{	
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp		*pGirCen		= pDB->GetGirder(-1);	//���� �߽� �Ŵ�
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CAPlateOutBMBrInfo	*pBrInfo		= GetBridgeInfo(0);
	CCalcData			*pData			= m_pAPlateOutBMStd->GetDataManage()->GetCalcData();	
	CCalcFloor			CalcFloor(m_pAPlateOutBMStd->GetDataManage());	
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;	
	CGlobarOption		*pGlopt			= m_pAPlateOutBMStd->GetDataManage()->GetGlobalOption();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutBMStd->GetGeneralInputStd()->m_pCalcStd;

	CPlateBasicIndex	*pBx			= pBrInfo->GetBxSection(SECTION_GEN);
	CPlateBasicIndex	*pBxBr			= NULL;
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	CDomyun				Dom;
	CVectorArray		ptArrDan;
	CVectorArray		ptArrGen;

	CString	sText				= _T("");
	CString	sCell1				= _T("");
	CString	sCell2				= _T("");
	CString	sCell3				= _T("");
	BOOL	pBrBeamLeft			= FALSE;
	BOOL	pBrBeamRigh			= FALSE;
	BOOL	bBracket			= FALSE;
	BOOL	bCrossBeam			= FALSE;
	long	nSttRow				= 3;
	long	nGirdersu			= pDB->GetGirdersu();
	long	nGuardsu			= pDB->GetGuardWallSu();
	long	nJigansu			= pDB->m_nQtyJigan;
	long	nGirdersu1			= 0;
	long	nGirdersu2			= 0;
	double	dSta				= pBx->GetStation();
	double	dDisLStt			= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT);		// ������ ���� ����
	double	dDisRStt			= pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT);	// ������ ���� ����
	double	dAngleStt			= pGirCen->GetAngleStt().GetAngleDegree();					// ���� �簢
	double	dAngleEnd			= pGirCen->GetAngleEnd().GetAngleDegree();					// ���� �簢
	double	dWidthBridge		= pDB->GetWidthAct(pBx->GetStation());						// ���� ���Ÿ�
	double	dCantileverLeft		= pDB->m_dWidthSlabLeft;//pBrInfo->GetCantileverWidth(TRUE);
	double	dCantileverRigh		= pDB->m_dWidthSlabRight;//pBrInfo->GetCantileverWidth(FALSE);
	double	dSharpLeft			= 0;//pGirCen->GetSharp(TRUE, TRUE);						// ���� �÷��� ������
	double	dHunch				= pDB->m_dWidthHunch;										// ��ġ ��
	double	dGirderWidth1		= 0;
	double	dGirderWidth2		= 0;
	double	dBrWidth			= 0;
	double	dGuardHeightLeft	= pDB->GetGuardFence(0)->GetHeight();
	double	dGuardHeightRigh	= pDB->GetGuardFence(nGuardsu-1)->GetHeight();
	double	dWingLeftStt		= pBMOpt->m_structWingWall.m_dLengthLeftStt;
	double	dWingLeftEnd		= pBMOpt->m_structWingWall.m_dLengthLeftEnd;
	double	dWingRighStt		= pBMOpt->m_structWingWall.m_dLengthRighStt;
	double	dWingRighEnd		= pBMOpt->m_structWingWall.m_dLengthRighEnd;

	double	dExpJointSttHeight	= pGlopt->GetSttExpansionJointHeight();
	double	dExpJointSttWidth	= pGlopt->GetSttExpansionJointWidth();
	double	dExpJointEndHeight	= pGlopt->GetEndExpansionJointHeight();
	double	dExpJointEndWidth	= pGlopt->GetEndExpansionJointWidth();

	double	dSttHunchHeight		= pDB->m_dHeightTaperStt;
	double	dEndHunchHeight		= pDB->m_dHeightTaperEnd;
	double	dMinGirderHeight	= pBrInfo->GetGirderHeight(FALSE);
	double	dMaxGirderHeight	= pBrInfo->GetGirderHeight(TRUE);
	double	dTu					= pGir->GetThickFactByStation(dSta, G_F_U);
	double	dLenAver			= pBrInfo->GetConfinementLengthAver();

	if(nGirdersu > 1)					bCrossBeam	= TRUE;

	if(!pBMOpt->m_structWingWall.m_bApply)
	{
		dWingLeftStt	= 0;
		dWingLeftEnd	= 0;
		dWingRighStt	= 0;
		dWingRighEnd	= 0;
	}

	Dom.SetCWnd(m_pAPlateOutBMStd->GetWnd());
	pXL->SetActiveSheet("����ٰ�");
	ArrDelSheet.Add("����ٰ�");

	if(m_bAbort)	return;

	nSttRow = 5;
	Dom.ClearEtt(TRUE);
	Dom.SetDimExo(1);
	Dom.SetDimVLen(1);
	Dom.SetDimLevelDis(1);
	Dom.SetTextHeight(2.5);
	Dom.SetScaleDim(150);
	pBrInfo->DrawSlabFrontGen(&Dom);
	pOut->InsertDomyun(pXL, &Dom, nSttRow, 25, 13, FALSE, TRUE, FALSE);

	nSttRow = 3;
	// ����
	sText.Format("%.3f", toM(dDisLStt));
	pOut->SetXL(pXL, sText,		nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dDisRStt));
	pOut->SetXL(pXL, sText,		nSttRow, "AZ", "BC", 2, RED, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL(pXL, dAngleStt,	nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL(pXL, dAngleEnd,	nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dWidthBridge));
	pOut->SetXL(pXL, sText,		nSttRow, "AZ", "BC", 4, RED, 9, FALSE, TA_CENTER, "", 3);
	// ��Ÿ
	nSttRow = 3;
	WriteSlabEctLength(pXL, nSttRow);
	// ĵƿ���� �� �Ŵ�
	nSttRow = 12;
	sText.Format("%.3f", toM(dCantileverLeft));
	pOut->SetXL(pXL, sText,		nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dCantileverRigh));
	pOut->SetXL(pXL, sText,		nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);

	sText.Format("%d", nGirdersu);
	pOut->SetXL(pXL, sText,		nSttRow, "AX", "AY", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("%.3f", toM(dGirderWidth1));
	pOut->SetXL(pXL, sText,		nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%d", nGirdersu2);
	pOut->SetXL(pXL, sText,		nSttRow, "AX", "AY", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("%.3f", toM(dGirderWidth2));
	pOut->SetXL(pXL, sText,		nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);

	// ����
	//APlate�� ���� �׸�� �� 0����...070314....KB..
	pOut->SetXL(pXL, 0.0,	nSttRow, "AX", "AX", 0, RED, 9, FALSE, TA_CENTER, "OUTSIDE_CROSS_QTY_LEFT", 0);
	pOut->SetXL(pXL, 0.0,	nSttRow, "AY", "AY", 0, RED, 9, FALSE, TA_CENTER, "OUTSIDE_CROSS_QTY_RIGH", 0);
	sText.Format("%.3f", toM(dBrWidth));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "OUTSIDE_CROSS_WIDTH", 3);
	
	// �÷��� ������ ����
	//APlate ������ ���� �÷��� ���� ��...
	dSharpLeft = pGirCen->GetWidthFlangeUpper(pBx)/2;
	sText.Format("%.3f", toM(dSharpLeft));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);

	// �������
	sText.Format("%.3f", toM(dHunch));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);


	// �ܺ���ġ�� ����
	sText.Format("%.3f", toM(dSttHunchHeight));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);	
	sText.Format("%.3f", toM(dEndHunchHeight));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 2, RED, 9, FALSE, TA_CENTER, "", 3);	
	// ��ȣ����
	sText.Format("%.3f", toM(dGuardHeightLeft));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dGuardHeightRigh));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	// ������
	sText.Format("%.3f", toM(dWingLeftStt));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dWingLeftEnd));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dWingRighStt));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dWingRighEnd));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 2, RED, 9, FALSE, TA_CENTER, "", 3);
	// �Ŵ�
	sText.Format("%.3f", toM(dMinGirderHeight));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dMaxGirderHeight));
	pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);

	if(pDB->IsTUGir())
	{
		nSttRow = 33;
		sText.Format("%.3f", toM(pDB->m_BindConc.m_dWidth));
		pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
		sText.Format("%.3f", toM(pDB->m_BindConc.m_dDis-dTu));
		pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
		sText.Format("%.3f", toM(dLenAver));
		pOut->SetXL(pXL, sText,			nSttRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}

	if(m_bAbort)	return;
	
	// ���������� ġ��ǥ
	nSttRow = 55;
	WriteSlabDimTable(pXL, nSttRow);

	// ���������� BLOCK OUT
	nSttRow+=4;
	sText.Format("%.3f", toM(dExpJointSttWidth));
	pOut->SetXL(pXL, sText,			nSttRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dExpJointSttHeight));
	pOut->SetXL(pXL, sText,			nSttRow, "P", "R", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dExpJointEndWidth));
	pOut->SetXL(pXL, sText,			nSttRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	sText.Format("%.3f", toM(dExpJointEndHeight));
	pOut->SetXL(pXL, sText,			nSttRow, "P", "R", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	// ���κ� �� �������κ�
	WriteCrossBeam(pXL, nSttRow);

	nSttRow = 86;
	CMap <CString, LPCTSTR, CString, CString> MapHeightDan;
	CMap <CString, LPCTSTR, CString, CString> MapLengthDan;
	CMap <CString, LPCTSTR, CString, CString> MapSlopDan;
	CMap <CString, LPCTSTR, CString, CString> MapAreaDan;

	// ������ ������ ���� ���ϱ�
	pBx	= pBrInfo->GetBxSection(SECTION_DANBU_STT);
	pBrInfo->GetSlabHunch(pBx, ptArrDan);
	Dom.ClearEtt(TRUE);
	pBrInfo->DrawMainDomyun(&Dom, SECTION_DANBU_STT);
	pOut->InsertDomyun(pXL, &Dom, nSttRow, 25, 6, FALSE, TRUE, FALSE);
	nSttRow +=9;
	
	long nLinesu	= 0;
	long nSize		= ptArrDan.GetSize();

	if(nSize>13)	nLinesu = (long)RoundUp(nSize/13,0);
	else			nLinesu = 0;
	pXL->InsertRowLine(nSttRow, (nLinesu+1)*8);
	WriteSlabHeight(pXL, ptArrDan, nSttRow, MapHeightDan);
	WriteSlabLength(pXL, ptArrDan, nSttRow, MapLengthDan);
	WriteSlabSlop(pXL, ptArrDan, nSttRow, MapHeightDan, MapLengthDan, MapSlopDan);
	WriteSlabArea(pXL, ptArrDan, nSttRow, MapHeightDan, MapLengthDan, MapAreaDan);

	// �Ϲݺ� ������ ���� ���ϱ�
	CMap <CString, LPCTSTR, CString, CString> MapHeightGen;
	CMap <CString, LPCTSTR, CString, CString> MapLengthGen;
	CMap <CString, LPCTSTR, CString, CString> MapSlopGen;
	CMap <CString, LPCTSTR, CString, CString> MapAreaGen;
	nSttRow += 3;
	pBx	= pBrInfo->GetBxSection(SECTION_GEN);
	pBrInfo->GetSlabHunch(pBx, ptArrGen);
	Dom.ClearEtt(TRUE);
	pBrInfo->DrawMainDomyun(&Dom, SECTION_GEN);
	pOut->InsertDomyun(pXL, &Dom, nSttRow, 25, 6, FALSE, TRUE, FALSE);
	nSttRow +=9;
	nSize = ptArrGen.GetSize();

	if(nSize>13)	nLinesu = (long)RoundUp(nSize/13,0);
	else			nLinesu = 0;
	pXL->InsertRowLine(nSttRow, (nLinesu+1)*8);
	WriteSlabHeight(pXL, ptArrGen, nSttRow, MapHeightGen);
	WriteSlabLength(pXL, ptArrGen, nSttRow, MapLengthGen);
	WriteSlabSlop(pXL, ptArrGen, nSttRow, MapHeightGen, MapLengthGen, MapSlopGen);
	WriteSlabArea(pXL, ptArrGen, nSttRow, MapHeightGen, MapLengthGen, MapAreaGen);

	if(m_bAbort)	return;

	nSttRow+=3;

	// ��ũ��Ʈ ����
	if(pBMOpt->m_bConcreteBuy)
	{
		nSttRow += 1;
		sText.Format("(25-%.0f-15)", pData->DESIGN_MATERIAL.m_dSigmaCK);
		pOut->SetXL(pXL, sText,	nSttRow, "A", "F", 0, BLACK, 9, FALSE, TA_CENTER);

		nSttRow += 1;
		// ��ũ��Ʈ - ������ �ܺ�
		sText.Format("=%s", MapAreaDan["SUM_AREA"]);
		pOut->SetXL(pXL, sText,		nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		// ��ũ��Ʈ - ������ �Ϲݺ�
		nSttRow += 2;
		sText.Format("=%s", MapAreaGen["SUM_AREA"]);
		pOut->SetXL(pXL, sText,		nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		// ��ũ��Ʈ - ������ ��ġ��
		nSttRow+=2;

		// ūũ��Ʈ - ���κ���
		nSttRow++;
		if(bCrossBeam)
		{
			nSttRow++;
			sText.Format("=%s", pOut->GetCellRef("CR_HU_LOW_WIDTH"));
			pOut->SetXL(pXL, sText,		nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_HU_UP_LENGTH"));
			pOut->SetXL(pXL, sText,		nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_HU_AVE_HEIGHT"));
			pOut->SetXL(pXL, sText,		nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_AVE_LENGTH"));
			pOut->SetXL(pXL, sText,		nSttRow, "Z","AB", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_HU_QTY"));
			pOut->SetXL(pXL, sText,		nSttRow,"AD","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
			nSttRow++;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+1);
			pXL->DeleteRowLineEnd();
		}

		// ��ũ��Ʈ - ���� ���κ���
//		if(bBracket)
//		{
//			nSttRow++;
//			sText.Format("=%s", pOut->GetCellRef("BR_HU_LOW_WIDTH"));
//			pOut->SetXL(pXL, sText,		nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
//			sText.Format("=%s", pOut->GetCellRef("BR_HU_UP_LENGTH"));
//			pOut->SetXL(pXL, sText,		nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
//			sText.Format("=%s", pOut->GetCellRef("BR_HU_AVE_HEIGHT"));
//			pOut->SetXL(pXL, sText,		nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
//			sText.Format("=%s", pOut->GetCellRef("BR_AVE_LENGTH"));
//			pOut->SetXL(pXL, sText,		nSttRow, "Z","AB", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
//			sText.Format("=%s", pOut->GetCellRef("BR_HU_QTY"));
//			pOut->SetXL(pXL, sText,		nSttRow,"AD","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
//			nSttRow+=6;
//		}
//		else
//		{
			pXL->DeleteRowLine(nSttRow, nSttRow+1);
			pXL->DeleteRowLineEnd();
			if(pDB->IsTUGir())	nSttRow+=9;
			else				nSttRow+=5;
//		}
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+16);
		pXL->DeleteRowLineEnd();
	}

	// ��ũ��Ʈ Ÿ��
	if(pBMOpt->m_bConcretePlacing)
	{
		nSttRow+=4;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+3);
		pXL->DeleteRowLineEnd();
	}

	// ��Ǫ��
	if(pBMOpt->m_bMold)
	{
		// 1. �����
		nSttRow+=2;
		if(pADeckData->m_nTypeBMApply==0)
		{
			pOut->SetXL(pXL, "(����3ȸ)",	nSttRow, "A", "F", 0, BLACK, 9, TRUE, TA_CENTER);		
		}
		nSttRow++;

		sText.Format("=%s", MapSlopDan["SUM_SLOP"]);
		pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		nSttRow+=2;
		sText.Format("=%s", MapSlopGen["SUM_SLOP"]);
		pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		// ������
		nSttRow+=4;
		if(nGirdersu2==0)
		{
			pXL->DeleteRowLine(nSttRow, nSttRow);
			pXL->DeleteRowLineEnd();
		}
		else
			nSttRow+=2;

		if(bCrossBeam)
		{
			nSttRow++;
			sText.Format("=%s", pOut->GetCellRef("CR_HU_UP_LENGTH"));
			pOut->SetXL(pXL, sText,	nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_AVE_LENGTH"));
			pOut->SetXL(pXL, sText, nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_HU_QTY"));
			pOut->SetXL(pXL, sText,	nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
			nSttRow++;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+1);
			pXL->DeleteRowLineEnd();		
		}

		if(bBracket)
		{
// 			nSttRow++;
// 			sText.Format("=%s", pOut->GetCellRef("BR_HU_UP_LENGTH"));
// 			pOut->SetXL(pXL, sText,	nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
// 			sText.Format("=%s", pOut->GetCellRef("BR_AVE_LENGTH"));
// 			pOut->SetXL(pXL, sText, nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
// 			sText.Format("=%s", pOut->GetCellRef("BR_HU_QTY"));
// 			pOut->SetXL(pXL, sText,	nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
// 			nSttRow++;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+1);
			pXL->DeleteRowLineEnd();
		}

		if(pBrBeamLeft)
		{
// 			nSttRow++;
// 			sText.Format("=%s", pOut->GetCellRef("OUTSIDE_CROSS_WIDTH"));
// 			pOut->SetXL(pXL, sText,	nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
// 			sText.Format("=%s", pOut->GetCellRef("OUTSIDE_CROSS_QTY_LEFT"));
// 			pOut->SetXL(pXL, sText,	nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		}
		else
		{
			pXL->DeleteRowLine(nSttRow+1, nSttRow+1);
			pXL->DeleteRowLineEnd();
		}

		if(pBrBeamRigh)
		{
// 			nSttRow++;
// 			sText.Format("=%s", pOut->GetCellRef("OUTSIDE_CROSS_WIDTH"));
// 			pOut->SetXL(pXL, sText,	nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
// 			sText.Format("=%s", pOut->GetCellRef("OUTSIDE_CROSS_QTY_RIGH"));
// 			pOut->SetXL(pXL, sText,	nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
// 			nSttRow++;
		}
		else
		{
			if(pBrBeamLeft)		pXL->DeleteRowLine(nSttRow+2, nSttRow+2);
			else				pXL->DeleteRowLine(nSttRow+1, nSttRow+1);
			pXL->DeleteRowLineEnd();
		}

		if(!pBrBeamLeft && !pBrBeamRigh)
		{
			pXL->DeleteRowLine(nSttRow, nSttRow);
			pXL->DeleteRowLineEnd();
		}

		// 2. ����
		if(pDB->IsTUGir())	nSttRow+=7;
		else				nSttRow+=5;
		sText.Format("=%s", MapSlopDan["SUM_SLOP"]);
		pOut->SetXL(pXL, sText, nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		sText.Format("=%s", MapSlopGen["SUM_SLOP"]);
		pOut->SetXL(pXL, sText, nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);

		nSttRow+=4;
		if(bCrossBeam)
		{
			nSttRow++;
			sText.Format("=%s", pOut->GetCellRef("CR_HU_SLOW_LENGTH"));
			pOut->SetXL(pXL, sText,	nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_AVE_LENGTH"));
			pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("CR_HU_QTY"));
			pOut->SetXL(pXL, sText,	nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
			nSttRow++;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+1);
			pXL->DeleteRowLineEnd();
		}

		if(bBracket)
		{
			nSttRow++;
			sText.Format("=%s", pOut->GetCellRef("BR_HU_SLOW_LENGTH"));
			pOut->SetXL(pXL, sText,	nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("BR_AVE_LENGTH"));
			pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("=%s", pOut->GetCellRef("BR_HU_QTY"));
			pOut->SetXL(pXL, sText,	nSttRow, "X", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
			nSttRow++;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+1);
			pXL->DeleteRowLineEnd();
		}
		nSttRow+=5;
		// 3. ������
		sText.Format("=%s", MapHeightDan["H1"]);
		pOut->SetXL(pXL, sText, nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		sText.Format("=%s", MapHeightGen["H1"]);
		pOut->SetXL(pXL, sText, nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		if(!pBMOpt->m_structStyrofoam.m_bApply)
		{
			pOut->SetXL(pXL, 1.0,	nSttRow, "AE", "AF", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			nSttRow+=2;
			pOut->SetXL(pXL, 1.0,	nSttRow, "AE", "AF", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		}
		else
		{
			if(pBMOpt->m_structStyrofoam.m_bLeft)
			{
				pOut->SetXL(pXL, 1.0,	nSttRow, "AE", "AF", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				nSttRow+=2;
				pOut->SetXL(pXL, 0.0,	nSttRow, "AE", "AF", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);	
			}
			else
			{
				pOut->SetXL(pXL, 0.0,	nSttRow, "AE", "AF", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				nSttRow+=2;
				pOut->SetXL(pXL, 1.0,	nSttRow, "AE", "AF", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
			}
		}
		nSttRow+=2;
		sText.Format("=%s", MapAreaDan["SUM_AREA"]);
		pOut->SetXL(pXL, sText,	nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		// 4. ��Ǫ������
		if(pDB->IsTUGir())	nSttRow+=21;
		else				nSttRow+=19;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+51);
		pXL->DeleteRowLineEnd();
	}


	if(m_bAbort)	return;

	// �����̼� ��ġ
	// ���ٸ�
	if(pBMOpt->m_structTimber.m_bApply)
	{
		nSttRow++;
		// InsertDomyun
		pBx	= pBrInfo->GetBxSection(SECTION_GEN);

		CDomyun DomTemp;
		DomTemp.SetCWnd(m_pAPlateOutBMStd->GetWnd());
		pBrInfo->DrawSlabDongbari(&DomTemp, pBx);
		DomTemp.SetCalcExtRect();
		CDRect BoundRect  = DomTemp.GetExtRect();
		double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
		double dScale     = 60*(dDiagonalL/16000);

		Dom.ClearEtt(TRUE);
		Dom.SetScaleDim(dScale);		
		pBrInfo->DrawSlabDongbari(&Dom, pBx);
		pOut->InsertDomyun(pXL, &Dom, nSttRow, 25, 9, FALSE, TRUE, FALSE);
		nSttRow+=13;
		if(pDB->m_cByunDan)
		{
			sText.Format("< �ܸ� ��ȭ�� ��ǥ �ܸ� >");
			pOut->SetXL(pXL, sText,	nSttRow, "Q", "Q", 1);
			sText.Format("K%d", nSttRow-1);
			pXL->CopyPicture(m_pImageXL, "BM_SECTION", sText);
			double	dLeftLen	= 0;
			double	dRighLen	= 0;
			double	dByun		= 0;
			if(nJigansu == 1)
			{
				dLeftLen	= pDB->m_dLenLeftCurve[0];
				dRighLen	= pDB->m_dLenRightCurve[0];
				dByun		= pDB->m_dLenLeftByun[0]+pDB->m_dLenRightByun[0];
			}
			else
			{
				dLeftLen	= pDB->m_dLenRightCurve[0];
				dRighLen	= pDB->m_dLenLeftCurve[1];
				dByun		= pDB->m_dLenRightByun[0]+pDB->m_dLenLeftByun[1];
			}
			nSttRow+=5;
			sText.Format("%.3f", toM(dLeftLen));
			pOut->SetXL(pXL, sText,	nSttRow, "O", "S", 0, RED, 9, FALSE, TA_CENTER, "LEFT_LEN", 3);
			sText.Format("%.3f", toM(dRighLen));
			pOut->SetXL(pXL, sText,	nSttRow, "X","AA", 0, RED, 9, FALSE, TA_CENTER, "RIGHT_LEN", 3);
			nSttRow++;
			sText.Format("%.3f", toM(dByun));
			pOut->SetXL(pXL, sText,	nSttRow, "T","V", 0, RED, 9, FALSE, TA_CENTER, "BYUN", 3);
			nSttRow+=2;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+6);
			pXL->DeleteRowLineEnd();
			nSttRow+=2;
		}
		sText.Format("(%s)", pBMOpt->GetTimberQuality(pBMOpt->m_structTimber.m_nQuality));
		pOut->SetXL(pXL, sText,	nSttRow, "A", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		nSttRow++;
		if(pBMOpt->m_structTimber.m_nPosDeckFinisher != 0)
		{
			if(pBMOpt->m_structTimber.m_nPosDeckFinisher==2)		// ��������
			{
				nSttRow+=2;
				pXL->DeleteRowLine(nSttRow, nSttRow+1);
				pXL->DeleteRowLineEnd();
			}
			else if(pBMOpt->m_structTimber.m_nPosDeckFinisher==1)	// ��������
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+1);
				pXL->DeleteRowLineEnd();
				nSttRow+=2;
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+3);
				pXL->DeleteRowLineEnd();
			}
		}
		else	nSttRow+=4;

		if(pDB->m_cByunDan)
		{
			nSttRow += 2;
			double dByunsu = 0;
			// ���ܸ� ������
			for(long n=0; n<nJigansu; n++)
			{
				if(pDB->m_nStyleByun[n]==1 || pDB->m_nStyleByun[n]==4)
					dByunsu +=1;
				else dByunsu +=0.5;
			}
			sText.Format("=%s", pOut->GetCellRef("BYUN"));
			pOut->SetXL(pXL, sText,	nSttRow, "M", "P", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s+%s+%s", pOut->GetCellRef("LEFT_LEN"), pOut->GetCellRef("BYUN"), pOut->GetCellRef("RIGHT_LEN"));
			pOut->SetXL(pXL, sText,	nSttRow, "R", "U", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.1f", dByunsu);
			pOut->SetXL(pXL, sText,	nSttRow, "AF", "AG", 0, RED, 9, FALSE, TA_CENTER, "", 1);
			nSttRow++;

			if(pBMOpt->m_structTimber.m_nPosDeckFinisher != 0)
			{
				if(pBMOpt->m_structTimber.m_nPosDeckFinisher==2)		// ��������
				{
					nSttRow+=4;
					pXL->DeleteRowLine(nSttRow, nSttRow+3);
					pXL->DeleteRowLineEnd();
				}
				else if(pBMOpt->m_structTimber.m_nPosDeckFinisher==1)	// ��������
				{
					pXL->DeleteRowLine(nSttRow, nSttRow+3);
					pXL->DeleteRowLineEnd();
					nSttRow+=4;
				}
				else
				{
					pXL->DeleteRowLine(nSttRow, nSttRow+7);
					pXL->DeleteRowLineEnd();
				}
				nSttRow += 6;
			}
			else	nSttRow+=17;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+10);
			pXL->DeleteRowLineEnd();
			nSttRow += 6;
		}

		// ���ٸ� ��ũ�Ǵϼ�
		if(pBMOpt->m_structTimber.m_nPosDeckFinisher == 0)
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+18);
			pXL->DeleteRowLineEnd();
		}
		else
		{
			if(pBMOpt->m_structTimber.m_nPosDeckFinisher == 1)
			{
				nSttRow+=2;
				sText.Format("=%.3f", toM(pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth));
				pOut->SetXL(pXL, sText, nSttRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
				nSttRow++;
				pXL->DeleteRowLine(nSttRow, nSttRow+1);
				pXL->DeleteRowLineEnd();
			}
			else if(pBMOpt->m_structTimber.m_nPosDeckFinisher == 2)
			{
				nSttRow++;
				pXL->DeleteRowLine(nSttRow, nSttRow+1);
				pXL->DeleteRowLineEnd();
				pOut->SetXL(pXL, "���ٸ�",	nSttRow, "B", "E", 0, BLACK, 9, TRUE, TA_CENTER);
				nSttRow++;
				pOut->SetXL(pXL, "(��ũ�Ǵϼ�)",	nSttRow, "A", "F", 0, BLACK, 9, TRUE, TA_CENTER);
				sText.Format("=%.3f", toM(pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth));
				pOut->SetXL(pXL, sText, nSttRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
				nSttRow++;
			}
			else
			{
				nSttRow+=2;
				sText.Format("=%.3f", toM(pBMOpt->m_structTimber.m_dDeckFinisherWorkWidth));
				pOut->SetXL(pXL, sText, nSttRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
				nSttRow+=2;
				pOut->SetXL(pXL, sText, nSttRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
				nSttRow++;
			}

			// ���ܸ� �߰� �κ�
			if(pDB->m_cByunDan)
			{
				nSttRow++;
				if(pBMOpt->m_structTimber.m_nPosDeckFinisher == 1)		// ��������
				{
					nSttRow+=4;
					pXL->DeleteRowLine(nSttRow, nSttRow+3);
					pXL->DeleteRowLineEnd();
				}
				else if(pBMOpt->m_structTimber.m_nPosDeckFinisher == 2)	// ��������
				{
					pXL->DeleteRowLine(nSttRow, nSttRow+3);
					pXL->DeleteRowLineEnd();
					nSttRow+=4;
				}
				else	nSttRow += 8;
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+8);
				pXL->DeleteRowLineEnd();
			}
			nSttRow += 5;
		}
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+63);
		pXL->DeleteRowLineEnd();	
	}

	// �ð����� - ��Ƽ����
	if(pBMOpt->m_structStyrofoam.m_bApply)
	{
		nSttRow++;
		if(pBMOpt->m_structStyrofoam.m_bLeft)	sText.Format("����");
		else									sText.Format("����");
		pOut->SetXL(pXL, sText,	nSttRow, "AU", "AU", 0, RED);
		nSttRow+=2;
		// ����������������...
		sCell1.Format("A%d", nSttRow);
		sText.Format("%.0f", pBMOpt->m_structStyrofoam.m_dThick);
		pXL->SetXL(sCell1, sText);
		nSttRow+=3;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+5);
		pXL->DeleteRowLineEnd();
	}

	// ������ ���
	nSttRow+=2;
	sText.Format("=%s", pOut->GetCellRef("ETC_SUM"));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	// �����(��ũ�Ǵϼ�)
	nSttRow+=4;
	// ������
	nSttRow+=5;
	if(pBMOpt->m_bDrainBridgeSurface)
		nSttRow+=5;
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+4);
		pXL->DeleteRowLineEnd();
	}
	// �ƽ�������
	if(pBMOpt->m_structPave.m_bApply)
	{
		nSttRow+=2;
		sCell1.Format("A%d", nSttRow);
		sText.Format("%.0f", toCm(pBMOpt->m_structPave.m_dPaveThick));
		pXL->SetXL(sCell1, sText);
		nSttRow+=3;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+4);
		pXL->DeleteRowLineEnd();
	}
	// ������
	if(pBMOpt->m_structTackCoating.m_bApply)
	{
		nSttRow++;
		sText.Format("%.3f", pBMOpt->m_structTackCoating.m_dTackCoating);
		pOut->SetXL(pXL, sText,	nSttRow, "N", "P", 0, RED, 9, FALSE, TA_CENTER, "", 1);
		nSttRow+=4;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+4);
		pXL->DeleteRowLineEnd();
	}
	// ��������
	if(pBMOpt->m_structExpansionJoint.m_bApply)
	{
		CString szSttName = pCalcStd->m_pDesignExpanJoint->GetSize() ? pCalcStd->m_pDesignExpanJoint->GetExpansionJointData(0)->m_strExpanJoint : "";
		CString szEndName = pCalcStd->m_pDesignExpanJoint->GetSize() ? pCalcStd->m_pDesignExpanJoint->GetExpansionJointData(1)->m_strExpanJoint : "";
		
		nSttRow+=3;
		sText.Format("�� %s", szSttName);
		pOut->SetXL(pXL, sText,	nSttRow, "H", "N", 0, RED, 9, FALSE, TA_LEFT);
		nSttRow++;
 		if(!pBMOpt->m_structExpansionJoint.m_bIncludeWalkLoad)
 			sText.Format("=%s", pOut->GetCellRef("ETC_SUM"));
 		else
			sText.Format("=%s-%s", pOut->GetCellRef("ETC_SUM"), pOut->GetCellRef("LEN_WALKROAD"));
		pOut->SetXL(pXL, sText,	nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		sText.Format("%.3f", toM(pBMOpt->m_structExpansionJoint.m_dMargin));
		pOut->SetXL(pXL, sText,	nSttRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER, "", 3);
		sText.Format("%d", pBMOpt->m_structExpansionJoint.m_nMarginsu);
		pOut->SetXL(pXL, sText,	nSttRow, "X", "X", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		nSttRow+=3;
		sText.Format("�� %s", szEndName);
		pOut->SetXL(pXL, sText,	nSttRow, "H", "N", 0, RED, 9, FALSE, TA_LEFT);
		nSttRow++;
 		if(!pBMOpt->m_structExpansionJoint.m_bIncludeWalkLoad)
 			sText.Format("=%s", pOut->GetCellRef("ETC_SUM"));
 		else
			sText.Format("=%s-%s", pOut->GetCellRef("ETC_SUM"), pOut->GetCellRef("LEN_WALKROAD"));
		pOut->SetXL(pXL, sText,	nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		nSttRow+=4;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+11);
		pXL->DeleteRowLineEnd();
	}
	// ��������ũ��Ʈ
	if(pBMOpt->m_structShrinkageConcrete.m_bApply)
	{
		nSttRow+=7;
		sText.Format("%d", pBMOpt->m_structShrinkageConcrete.m_nSttQty);
		pOut->SetXL(pXL, sText,	nSttRow, "Y", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		nSttRow++;
		sText.Format("%d", pBMOpt->m_structShrinkageConcrete.m_nEndQty);
		pOut->SetXL(pXL, sText,	nSttRow, "Y", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		nSttRow+=5;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+12);
		pXL->DeleteRowLineEnd();
	}

	// ������������
	if(pBMOpt->m_structExpansionJointCover.m_bApply)
	{
		nSttRow++;
		long nRow = nSttRow;
		for(long n=0; n<7; n++)
		{
			if(pBMOpt->m_structExpansionJointCover.m_nQty[n]!=0)
			{
				sText.Format("%d", pBMOpt->m_structExpansionJointCover.m_nQty[n]);
				pOut->SetXL(pXL, sText,	nSttRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 0);
				nSttRow+=3;
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+2);
				pXL->DeleteRowLineEnd();
			}
		}
		pOut->SetXL(pXL, "������������", nRow, "A", "F", 0, BLACK, 9, TRUE, TA_CENTER);
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+21);
		pXL->DeleteRowLineEnd();
	}

	// ������
	if(pBMOpt->m_structBridgeName.m_bApply)
	{
		nSttRow+=1;
		sText.Format("%d", pBMOpt->m_structBridgeName.m_nQty);
		pOut->SetXL(pXL, sText,	nSttRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		nSttRow+=3;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+3);
		pXL->DeleteRowLineEnd();	
	}
	// ������
	if(pBMOpt->m_structBridgeNamePlate.m_bApply)
	{
		nSttRow+=1;
		sText.Format("%d", pBMOpt->m_structBridgeNamePlate.m_nQty);
		pOut->SetXL(pXL, sText,	nSttRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		nSttRow+=3;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+3);
		pXL->DeleteRowLineEnd();	
	}
	// ������
	if(pBMOpt->m_structExplainPlate.m_bApply)
	{
		nSttRow+=1;
		sText.Format("%d", pBMOpt->m_structExplainPlate.m_nQty);
		pOut->SetXL(pXL, sText,	nSttRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		nSttRow+=3;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+3);
		pXL->DeleteRowLineEnd();	
	}
	// TBM ��ġ
	if(pBMOpt->m_structEstablishTBM.m_bApply)
	{
		nSttRow+=1;
		sText.Format("%d", pBMOpt->m_structEstablishTBM.m_nQty);
		pOut->SetXL(pXL, sText,	nSttRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		nSttRow+=3;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+3);
		pXL->DeleteRowLineEnd();	
	}
	// NOTCH
	if(pBMOpt->m_structNotch.m_bApply)
	{
		nSttRow+=1;
		sText.Format("%s", pBMOpt->m_structNotch.m_szNotchType);
		pOut->SetXL(pXL, sText,	nSttRow, "I", "I", 0, RED, 9, FALSE, TA_LEFT);
		nSttRow+=5;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+3);
		pXL->DeleteRowLineEnd();		
	}
	// ����ü�
	if(pBMOpt->m_bDrainEstablish)
	{
		if(pBMOpt->m_nDrainEstablishType == RIVER)				// ��õ��
		{
			nSttRow+=2;
			sText.Format("%d", pBMOpt->m_structRiverDrainEstablish.m_nWaterCollect);
			pOut->SetXL(pXL, sText,	nSttRow, "AI", "AL", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			nSttRow+=3;
			sText.Format("%.3f", toM(pBMOpt->m_structRiverDrainEstablish.m_dDrain));
			pOut->SetXL(pXL, sText,	nSttRow, "AI", "AL", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			nSttRow+=3;
			pXL->DeleteRowLine(nSttRow, nSttRow+20);
			pXL->DeleteRowLineEnd();
		}
		else if(pBMOpt->m_nDrainEstablishType == WALKWAY)			// ������
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+7);					// ��õ�� ����
			pXL->DeleteRowLineEnd();
			if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)	// TYPE 1
			{
				nSttRow++;
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinWaterCollect);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoint	);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				pXL->DeleteRowLine(nSttRow, nSttRow+13);					// TYPE 2 ����
				pXL->DeleteRowLineEnd();
				nSttRow++;
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+5);					// TYPE 1 ����
				pXL->DeleteRowLineEnd();
				nSttRow++;
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);				// ������
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nDrain);						// �����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinDrain);					// ��������
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));	// ����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[1]));	// ����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);				// ���
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[1]);				// ���
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[0]);				// û�ұ�
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[1]);				// û�ұ�
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[0]);				// ��ΰ�����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[1]);				// ��ΰ�����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCollecting);				// ħ����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				nSttRow+=2;
			}
		}
		else if(pBMOpt->m_nDrainEstablishType == RIVERWALKWAY)	// ��õ�� ������
		{
			nSttRow+=2;
			sText.Format("%d", pBMOpt->m_structRiverDrainEstablish.m_nWaterCollect);
			pOut->SetXL(pXL, sText,	nSttRow, "AI", "AL", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			nSttRow+=3;
			sText.Format("%.3f", toM(pBMOpt->m_structRiverDrainEstablish.m_dDrain));
			pOut->SetXL(pXL, sText,	nSttRow, "AI", "AL", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			nSttRow+=3;
			if(pBMOpt->m_structWalkWayDrainEstablish.m_nType == 0)	// TYPE 1
			{
				nSttRow++;
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinWaterCollect);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoint	);
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				pXL->DeleteRowLine(nSttRow, nSttRow+13);					// TYPE 2 ����
				pXL->DeleteRowLineEnd();
				nSttRow++;
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+5);					// TYPE 1 ����
				pXL->DeleteRowLineEnd();
				nSttRow++;
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nWaterCollect);				// ������
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nDrain);						// �����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nJoinDrain);					// ��������
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[0]));	// ����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%.3f", toM(pBMOpt->m_structWalkWayDrainEstablish.m_dStraightTube[1]));	// ����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[0]);				// ���
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCurveTube[1]);				// ���
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[0]);				// û�ұ�
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCleanTube[1]);				// û�ұ�
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[0]);				// ��ΰ�����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nUpperFixed[1]);				// ��ΰ�����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", pBMOpt->m_structWalkWayDrainEstablish.m_nCollecting);				// ħ����
				pOut->SetXL(pXL, sText,	nSttRow, "T", "W", 1, RED, 9, FALSE, TA_CENTER, "", 0);
				nSttRow+=2;
			}
		}
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+28);
		pXL->DeleteRowLineEnd();		
	}

	if(m_bAbort)	return;
	
	// ������.�����ǳ�
	long nGuardTypeL = CalcFloor.GetGuardWallType(FLOOR_LEFT);
	long nGuardTypeR = CalcFloor.GetGuardWallType(FLOOR_RIGHT);
	BOOL bBangEmExistL = (pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && nGuardTypeL<=8) ? TRUE : FALSE;
	BOOL bBangEmExistR = (pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && nGuardTypeR<=8) ? TRUE : FALSE;

	CCentSeparation	*pLCen	= pDB->GetGuardFence(0);
	CCentSeparation	*pRCen	= pDB->GetGuardFence(pDB->GetGuardWallSu()-1);
	if(bBangEmExistL || bBangEmExistR)
	{
		nSttRow++;
		if(!bBangEmExistL)
		{
			pOut->SetXL(pXL, "", nSttRow, "J", "M", 0, BLACK);
			pOut->SetXL(pXL, "", nSttRow, "N", "N", 0, BLACK);
		}
		if(!bBangEmExistR)
		{
			pOut->SetXL(pXL, "", nSttRow, "N", "N", 0, BLACK);
			pOut->SetXL(pXL, "", nSttRow, "O", "R", 0, BLACK);
		}
		nSttRow += 4;

		// �����ǳ�
		if(pBMOpt->m_structSoundProof.m_bApply)
		{
			// ����
			nSttRow++;
			if(bBangEmExistL)
			{
				nSttRow++;
				sText.Format("%.3f", toM(pBMOpt->m_structSoundProof.m_dLHeight));
				sCell1.Format("A%d", nSttRow);
				pXL->SetXL(sCell1, sText);
				nSttRow+=2;		
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+3);
				pXL->DeleteRowLineEnd();
			}
			// ����
			if(bBangEmExistR)
			{
				if(bBangEmExistL)	nSttRow += 2;
				else									nSttRow++;
				sText.Format("%.3f", toM(pBMOpt->m_structSoundProof.m_dRHeight));
				sCell1.Format("A%d", nSttRow);
				pXL->SetXL(sCell1, sText);
				nSttRow+=3;		
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow+3);
				pXL->DeleteRowLineEnd();
				nSttRow++;
			}
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+8);
			pXL->DeleteRowLineEnd();
		}
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+13);
		pXL->DeleteRowLineEnd();
	}

	BOOL bGuardL	= pBMOpt->m_structParapet.m_dGuardLength[0]>0 ? TRUE : FALSE;
	BOOL bGuardR	= pBMOpt->m_structParapet.m_dGuardLength[1]>0 ? TRUE : FALSE;
	BOOL bBodo		= pBMOpt->m_structParapet.m_dCurbLength*pBMOpt->m_structParapet.m_nCurbQty>0 ? TRUE : FALSE;
	// ����
	if(pBMOpt->m_structParapet.m_bApply)
	{
		nSttRow++;
		// ��ȣ����
		if(bGuardL || bGuardR)
		{
			if(bGuardL)
			{
				nSttRow++;
				sText.Format("%.3f", toM(pBMOpt->m_structParapet.m_dGuardLength[0]));
				pOut->SetXL(pXL, sText,	nSttRow, "P", "S", 0, RED, 9, FALSE, TA_CENTER, "", 3);
				nSttRow++;
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow);
				pXL->DeleteRowLineEnd();
			}
			if(bGuardR)
			{
				sText.Format("%.3f", toM(pBMOpt->m_structParapet.m_dGuardLength[1]));
				pOut->SetXL(pXL, sText,	nSttRow, "P", "S", 0, RED, 9, FALSE, TA_CENTER, "", 3);
				nSttRow+=5;
			}
			else
			{
				pXL->DeleteRowLine(nSttRow, nSttRow);
				pXL->DeleteRowLineEnd();
				nSttRow+=4;
			}
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+6);
			pXL->DeleteRowLineEnd();
			pOut->SetXL(pXL, "�� ��",	nSttRow, "A", "F", 0, BLACK, 9, TRUE, TA_CENTER);
		}

		// ������
		if(bBodo)
		{
			nSttRow++;
			sText.Format("%.3f", toM(pBMOpt->m_structParapet.m_dCurbLength));
			pOut->SetXL(pXL, sText,	nSttRow, "P", "S", 0, RED, 9, FALSE, TA_CENTER, "", 3);
			sText.Format("%d", pBMOpt->m_structParapet.m_nCurbQty);
			pOut->SetXL(pXL, sText,	nSttRow, "U", "V", 0, RED, 9, FALSE, TA_CENTER, "", 0);
			nSttRow+=4;
		}
		else
		{
			nSttRow--;
			pXL->DeleteRowLine(nSttRow, nSttRow+5);
			pXL->DeleteRowLineEnd();
		}
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+12);
		pXL->DeleteRowLineEnd();
	}

	// ���Ϲ�������
	if(pBMOpt->m_structDroppingPrevent.m_bApply)
	{
		nSttRow+=2;
		sText.Format("%.3f", toM(pBMOpt->m_structDroppingPrevent.m_dHorSpaceHeight));
		pOut->SetXL(pXL, sText,	nSttRow, "O", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 3);
		if(pBMOpt->m_structDroppingPrevent.m_bIsOnlyHorizontal)
		{
			nSttRow+=4;
			pXL->DeleteRowLine(nSttRow, nSttRow+4);
			pXL->DeleteRowLineEnd();
		}
		else
		{
			nSttRow+=5;
			sText.Format("%.3f", toM(pBMOpt->m_structDroppingPrevent.m_dVerSpaceHeight));
			pOut->SetXL(pXL, sText,	nSttRow, "O", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 3);
			nSttRow+=4;
		}
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+10);
		pXL->DeleteRowLineEnd();
	}

	// ���鹰����
	if(pBMOpt->m_bWaterDraw)		nSttRow+=6;
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+5);
		pXL->DeleteRowLineEnd();
	}

	// ö�ٰ�������
	if(pBMOpt->m_structRebarManufacture.m_bApply)
	{
		nSttRow+=2;
		double	dValueH		= 0;
		double	dValueD		= 0;
		BOOL	bVeryDiff	= TRUE;
		BOOL	bDiff		= TRUE;
		BOOL	bNormal		= TRUE;
		BOOL	bEasy		= TRUE;

		// �ſ캹��
		sCell1.Format("AN%d", nSttRow+5);
		sCell2.Format("AN%d", nSttRow+11);
		dValueH  = atof(pXL->GetXL(sCell1));
		dValueD  = atof(pXL->GetXL(sCell2));
		if(dValueH == 0.0 && dValueD == 0.0)		bVeryDiff	= FALSE;
		// ����
		sCell1.Format("AN%d", nSttRow+18);
		sCell2.Format("AN%d", nSttRow+24);
		dValueH  = atof(pXL->GetXL(sCell1));
		dValueD  = atof(pXL->GetXL(sCell2));
		if(dValueH == 0.0 && dValueD == 0.0)		bDiff	= FALSE;
		// ����
		sCell1.Format("AN%d", nSttRow+31);
		sCell2.Format("AN%d", nSttRow+37);
		dValueH  = atof(pXL->GetXL(sCell1));
		dValueD  = atof(pXL->GetXL(sCell2));
		if(dValueH == 0.0 && dValueD == 0.0)		bNormal	= FALSE;
		// ����
		sCell1.Format("AN%d", nSttRow+44);
		sCell2.Format("AN%d", nSttRow+50);
		dValueH  = atof(pXL->GetXL(sCell1));
		dValueD  = atof(pXL->GetXL(sCell2));
		if(dValueH == 0.0 && dValueD == 0.0)		bEasy	= FALSE;

		// �ſ캹��
		if(!bVeryDiff)
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+12);
			pXL->DeleteRowLineEnd();
		}
		else
		{
//			sText.Format("'1.");
//			pOut->SetXL(pXL, sText,	nSttRow, "H", "H", 0, BLACK, 9, TRUE);
			nSttRow+=14;
		}

		// ����
		if(!bDiff)
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+12);
			pXL->DeleteRowLineEnd();
		}
		else
		{
//			sText.Format("'2.");
//			pOut->SetXL(pXL, sText,	nSttRow, "H", "H", 0, BLACK, 9, TRUE);
			nSttRow+=13;
		}
		// ����
		if(!bNormal)
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+12);
			pXL->DeleteRowLineEnd();
		}
		else
		{
//			sText.Format("'3.");
//			pOut->SetXL(pXL, sText,	nSttRow, "H", "H", 0, BLACK, 9, TRUE);
			nSttRow+=13;
		}
		// ����
		if(!bEasy)
		{
//			sText.Format("'4.");
//			pOut->SetXL(pXL, sText,	nSttRow, "H", "H", 0, BLACK, 9, TRUE);
			pXL->DeleteRowLine(nSttRow, nSttRow+12);
			pXL->DeleteRowLineEnd();
		}
		else	nSttRow+=13;
	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+53);
		pXL->DeleteRowLineEnd();
	}
	
	CBMCreator		*pBMStd		= m_pAPlateOutBMStd->GetDataManage()->GetBMStandard();
	CPaintCreator   *pPaStd		= m_pAPlateOutBMStd->GetDataManage()->GetPaintStandard();	
	CPtrList		*pPaintList	= &pPaStd->m_ListStandard;
	CPtrList		*pSteelList	= &pBMStd->m_BMList_Thick;

	double	dSteelSum	= 0;
	double	dBoltSum	= 0;
	double	dSumPaintArea[PAINT_ITEM_MAX];

	POSITION pos = pSteelList->GetHeadPosition();
	while(pos)
	{
		CBMRecord* pRec = (CBMRecord *)pSteelList->GetNext(pos);
		sText = pRec->GetStrMaterial();
		if(sText.Find("SM") != -1 || sText.Find("HSB") != -1)
			dSteelSum += pRec->m_dTWeight;
		if(sText.Find("T") != -1 && sText.Find("F") != -1)
			dBoltSum += pRec->m_dTWeight;
	}
	
	long i = 0;

	for(i=0; i<PAINT_ITEM_MAX; i++) dSumPaintArea[i]=0;

	pos = pPaintList->GetHeadPosition();
	while(pos)
	{	
		CPaintRecord *pRec = (CPaintRecord *)pPaintList->GetNext(pos);
		for(i=0; i<PAINT_ITEM_MAX; i++)
			dSumPaintArea[i] += pRec->m_dPaintArea[i];
	}
	nSttRow+=1;
	// ���� ����
	sText.Format("%.3f", toTon(dSteelSum));
	pOut->SetXL(pXL, sText,	nSttRow, "Y", "AB", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// ������� �� ����
	nSttRow+=4;
	sText.Format("%.3f", toTon(dSteelSum));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// ���� �ܺε���(����)
	nSttRow+=4;
	sText.Format("%.3f", toM2(dSumPaintArea[OUTSIDE_FACT]));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// ���� �ܺε���(����)
	nSttRow+=4;
	sText.Format("%.3f", toM2(dSumPaintArea[OUTSIDE_FIELD]));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// ���� ���ε���(����)
	nSttRow+=4;
// 	sText.Format("%.3f", toM2(dSumPaintArea[INSIDE_FACT]));
// 	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// ������(����)
	nSttRow+=4;
	sText.Format("%.3f", toM2(dSumPaintArea[SPLICE_FACT]));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// �ܺ� ��Ʈ(����)
	nSttRow+=4;
	sText.Format("%.3f", toM2(dSumPaintArea[BOLT_FIELD_OUTSIDE]));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// ���� ��Ʈ(����)
	nSttRow+=4;
// 	sText.Format("%.3f", toM2(dSumPaintArea[BOLT_FIELD_INSIDE]));
// 	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// �ܺ������
	nSttRow+=4;
	sText.Format("%.3f", toM2(dSumPaintArea[PAVE_FACT]));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	// ��Ʈ �� ��Ʈ
	nSttRow+=4;
	sText.Format("%.3f", toTon(dBoltSum));
	pOut->SetXL(pXL, sText,	nSttRow, "O", "R", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	// ������
	nSttRow+=3;
	if(pBMOpt->m_structElecWireHole.m_bApply)
	{
		nSttRow += 2;

		// 100
		if(pBMOpt->m_structElecWireHole.m_nEA100[0]>0 || pBMOpt->m_structElecWireHole.m_nEA100[1]>0)
		{
			nSttRow++;
			
			sText.Format("%d", pBMOpt->m_structElecWireHole.m_nEA100[0]);
			pOut->SetXL(pXL,	sText,	nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
			nSttRow++;

			sText.Format("%d", pBMOpt->m_structElecWireHole.m_nEA100[1]);
			pOut->SetXL(pXL,	sText,	nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
			nSttRow += 4;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+5);
			pXL->DeleteRowLineEnd();
		}

		// 125
		if(pBMOpt->m_structElecWireHole.m_nEA125[0]>0 || pBMOpt->m_structElecWireHole.m_nEA125[1]>0)
		{
			nSttRow++;
			
			sText.Format("%d", pBMOpt->m_structElecWireHole.m_nEA125[0]);
			pOut->SetXL(pXL,	sText,	nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
			nSttRow++;

			sText.Format("%d", pBMOpt->m_structElecWireHole.m_nEA125[1]);
			pOut->SetXL(pXL,	sText,	nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
			nSttRow += 4;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+5);
			pXL->DeleteRowLineEnd();
		}

		// 150
		if(pBMOpt->m_structElecWireHole.m_nEA150[0]>0 || pBMOpt->m_structElecWireHole.m_nEA150[1]>0)
		{
			nSttRow++;
			
			sText.Format("%d", pBMOpt->m_structElecWireHole.m_nEA150[0]);
			pOut->SetXL(pXL,	sText,	nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
			nSttRow++;

			sText.Format("%d", pBMOpt->m_structElecWireHole.m_nEA150[1]);
			pOut->SetXL(pXL,	sText,	nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
			nSttRow += 4;
		}
		else
		{
			pXL->DeleteRowLine(nSttRow, nSttRow+5);
			pXL->DeleteRowLineEnd();
		}

	}
	else
	{
		pXL->DeleteRowLine(nSttRow, nSttRow+19);
		pXL->DeleteRowLineEnd();
	}

}


void CAPlateBMXL::WriteSlabHeight(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight)
{
	CAPlateOutBMFormat	*pOut	= m_pAPlateOutBMStd->GetAPlateOutBMFormat();

	CString	sCell1		= _T("");
	CString	sCell2		= _T("");
	CString	sText		= _T("");
	long	nSize		= ptArr.GetSize();
	long	nSttCol		= 0;
	long	nEndCol		= 0;
	long	nColIdx		= 0;
	long	nRow		= nSttRow;
	double	dHeight		= 0;
	double	nCellTerm	= 3;

	sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
	pXL->SetXL(sCell1, TA_CENTER, "����\n(m)");
	pXL->SetMergeCell(sCell1, sCell2);
	pXL->CellOutLine(sCell1, sCell2);
	pXL->SetCellColor(sCell1, sCell2, GRAY);
	pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

	// ������ ����
	for(long n=0; n<nSize; n++)
	{
		if(n!= 0 && n%13 == 0)
		{
			nRow += 8;
			sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, "����\n(m)");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);
			nColIdx = 0;
		}
		dHeight	= toM(ptArr[n].z);
		nSttCol	= (long)(nColIdx*nCellTerm+4);
		nEndCol	= (long)(nColIdx*nCellTerm+6);
		if(n==nSize-1)	nEndCol+=1;
		// H
		sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow);
		sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow);
		sText.Format("H%d", n+1);
		pXL->SetXL(sCell1, TA_CENTER, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		pXL->CellOutLine(sCell1, sCell2);
		pXL->SetCellColor(sCell1, sCell2, GRAY);
		pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

		sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow+1);
		sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow+1);
		sText.Format("%.3f", dHeight);
		pXL->SetXL(sCell1, TA_CENTER, sText);
		pXL->SetMergeCell(sCell1, sCell2);
		pXL->SetFonts(sCell1, sCell2, 9, "����", RED, FALSE);
		pXL->CellOutLine(sCell1, sCell2);

		sText.Format("H%d", n+1);
		MapHeight[sText] = sCell1;

		nColIdx++;
	}

	nSttRow+=2;
}

void CAPlateBMXL::WriteSlabLength(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapLength)
{
	CAPlateOutBMFormat	*pOut	= m_pAPlateOutBMStd->GetAPlateOutBMFormat();

	CString	sCell1		= _T("");
	CString	sCell2		= _T("");
	CString	sText		= _T("");
	CString	sCellRef	= _T("");
	long	nSize		= ptArr.GetSize();
	long	nSttCol		= 0;
	long	nEndCol		= 0;
	long	nRow		= nSttRow;
	long	nColIdx		= 0;
	double	dDis		= 0;
	double	nCellTerm	= 3;

	sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
	pXL->SetXL(sCell1, TA_CENTER, "����\n(m)");
	pXL->SetMergeCell(sCell1, sCell2);
	pXL->CellOutLine(sCell1, sCell2);
	pXL->SetCellColor(sCell1, sCell2, GRAY);
	pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

	// ������ ����
	for(long n=0; n<nSize; n++)
	{
		if(n!= 0 && n%13 == 0)
		{
			nRow += 8;
//			pXL->InsertRowLine(nRow, 2);
			sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, "����\n(m)");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);
			nColIdx = 0;
		}

		nSttCol	= (long)(nColIdx*nCellTerm+4);
		nEndCol	= (long)(nColIdx*nCellTerm+6);
		if(n==nSize-1)	nEndCol+=1;
		// L
		if(n < nSize-1)
		{
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow);
			sText.Format("L%d", n+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow+1);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow+1);
			sText.Format("%.3f", toM(fabs(ptArr[n].x - ptArr[n+1].x)));
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->SetFonts(sCell1, 9, "����", RED, FALSE);
			pXL->CellOutLine(sCell1, sCell2);

			sText.Format("L%d", n+1);
			MapLength[sText] = sCell1;
		}
		else
		{
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow);
			sText.Format("�Ұ�");
			pXL->SetXL(sCell1, TA_CENTER, "�Ұ�");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sText = _T("=");
			for(long n=0; n<nSize-1; n++)
			{
				sCell1.Format("L%d", n+1);
				if(n != nSize-2)	sCell2.Format("%s+", MapLength[sCell1]);
				else				sCell2.Format("%s", MapLength[sCell1]);
				sText += sCell2;
			}
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow+1);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);
			pXL->CellOutLine(sCell1, sCell2);
		}
		nColIdx++;
	}

	nSttRow+=2;
}

void CAPlateBMXL::WriteSlabSlop(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight, CMap <CString, LPCTSTR, CString, CString> &MapLength, CMap <CString, LPCTSTR, CString, CString> &MapSlop)
{
	CAPlateOutBMFormat	*pOut	= m_pAPlateOutBMStd->GetAPlateOutBMFormat();

	CString	sCell1		= _T("");
	CString	sCell2		= _T("");
	CString	sText		= _T("");
	CString	sCellRef	= _T("");
	long	nSize		= ptArr.GetSize();
	long	nSttCol		= 0;
	long	nEndCol		= 0;
	long	nRow		= nSttRow;
	long	nColIdx		= 0;
	double	dDis		= 0;
	double	nCellTerm	= 3;

	sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
	pXL->SetXL(sCell1, TA_CENTER, "���\n(m)");
	pXL->SetMergeCell(sCell1, sCell2);
	pXL->CellOutLine(sCell1, sCell2);
	pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);
	pXL->SetCellColor(sCell1, sCell2, GRAY);

	for(long n=0; n<nSize; n++)
	{
		if(n!= 0 && n%13 == 0)
		{
			nRow += 8;
//			pXL->InsertRowLine(nRow, 2);
			sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, "���\n(m)");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);
			nColIdx = 0;
		}

		nSttCol	= (long)(nColIdx*nCellTerm+4);
		nEndCol	= (long)(nColIdx*nCellTerm+6);
		if(n==nSize-1)	nEndCol+=1;

		if(n < nSize-1)
		{
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow);
			sText.Format("S%d", n+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sCell1.Format("H%d", n+1);		// ����
			sCell2.Format("H%d", n+2);		// ����
			sCellRef.Format("L%d", n+1);	// ����
//			sText.Format("=IF(ROUNDDOWN(SQRT((%s-%s)^2+%s^2),3)=%s,\"0\",ROUND(SQRT((%s-%s)^2+%s^2),3))",
//				MapHeight[sCell2], MapHeight[sCell1], MapLength[sCellRef], MapLength[sCellRef], MapHeight[sCell2], MapHeight[sCell1], MapLength[sCellRef]);
			double	dHeight1	= atof(pXL->GetXL(MapHeight[sCell1]));
			double	dHeight2	= atof(pXL->GetXL(MapHeight[sCell2]));
			if(ptArr.GetAt(n).y == ptArr.GetAt(n+1).y || dHeight1 == dHeight2)
			{
				sText.Format("%.3f", 0.0);
			}
			else
			{
				sText.Format("=ROUND(SQRT((%s-%s)^2+%s^2),3)", MapHeight[sCell2], MapHeight[sCell1], MapLength[sCellRef]);
			}
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow+1);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sText.Format("S%d", n+1);
			MapSlop[sText] = sCell1;
		}
		else
		{
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow);
			sText.Format("�Ұ�");
			pXL->SetXL(sCell1, TA_CENTER, "�Ұ�");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sText = _T("=");
			for(long n=0; n<nSize-1; n++)
			{
				sCell1.Format("S%d", n+1);
				if(n != nSize-2)	sCell2.Format("%s+", MapSlop[sCell1]);
				else				sCell2.Format("%s", MapSlop[sCell1]);
				sText += sCell2;
			}
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow+1);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			MapSlop["SUM_SLOP"] = sCell1;
		}
		nColIdx++;
	}

	nSttRow+=2;
}

void CAPlateBMXL::WriteSlabArea(CXLControl *pXL, CVectorArray &ptArr, long &nSttRow, CMap <CString, LPCTSTR, CString, CString> &MapHeight, CMap <CString, LPCTSTR, CString, CString> &MapLength, CMap <CString, LPCTSTR, CString, CString> &MapArea)
{
	CAPlateOutBMFormat	*pOut	= m_pAPlateOutBMStd->GetAPlateOutBMFormat();

	CString	sCell1		= _T("");
	CString	sCell2		= _T("");
	CString	sText		= _T("");
	CString	sCellRef	= _T("");
	long	nSize		= ptArr.GetSize();
	long	nSttCol		= 0;
	long	nEndCol		= 0;
	long	nRow		= nSttRow;
	long	nColIdx		= 0;
	double	dDis		= 0;
	double	nCellTerm	= 3;

	sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
	pXL->SetXL(sCell1, TA_CENTER, "����\n(m��)");
	pXL->SetMergeCell(sCell1, sCell2);
	pXL->CellOutLine(sCell1, sCell2);
	pXL->SetCellColor(sCell1, sCell2, GRAY);

	for(long n=0; n<nSize; n++)
	{
		if(n!= 0 && n%13 == 0)
		{
			nRow += 8;
//			pXL->InsertRowLine(nRow, 2);
			sCell1.Format("B%d", nRow);	sCell2.Format("D%d", nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, "����\n(m��)");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->CellOutLine(sCell1, sCell2);
			nColIdx = 0;
		}

		nSttCol	= (long)(nColIdx*nCellTerm+4);
		nEndCol	= (long)(nColIdx*nCellTerm+6);
		if(n==nSize-1)	nEndCol+=1;

		if(n < nSize-1)
		{
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow);
			sText.Format("A%d", n+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sCell1.Format("H%d", n+1);		// ����
			sCell2.Format("H%d", n+2);		// ����
			sCellRef.Format("L%d", n+1);	// ����
			sText.Format("=0.5*(%s+%s)*%s", MapHeight[sCell1], MapHeight[sCell2], MapLength[sCellRef]);
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow+1);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sText.Format("A%d", n+1);
			MapArea[sText] = sCell1;
		}
		else
		{
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow);
			sText.Format("	");
			pXL->SetXL(sCell1, TA_CENTER, "�Ұ�");
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetCellColor(sCell1, sCell2, GRAY);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			sText = _T("=");
			for(long n=0; n<nSize-1; n++)
			{
				sCell1.Format("A%d", n+1);
				if(n != nSize-2)	sCell2.Format("%s+", MapArea[sCell1]);
				else				sCell2.Format("%s", MapArea[sCell1]);
				sText += sCell2;
			}
			sCell1.Format("%s%d", pOut->GetCellID(nSttCol), nRow+1);
			sCell2.Format("%s%d", pOut->GetCellID(nEndCol), nRow+1);
			pXL->SetXL(sCell1, TA_CENTER, sText);
			pXL->SetMergeCell(sCell1, sCell2);
			pXL->CellOutLine(sCell1, sCell2);
			pXL->SetFonts(sCell1, sCell2, 9, "����", BLACK, FALSE);

			MapArea["SUM_AREA"] = sCell1;
			nSttRow = nRow;
		}
		nColIdx++;
	}

	nSttRow+=2;
}

// ����ٰ�(��ȣ��)
void CAPlateBMXL::GuardCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CAPlateOutBMBrInfo	*pBrInfo	= GetBridgeInfo(0);
	CADeckData			*pADeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CBMOption			*pBMOpt		= &pADeckData->m_BMOption;
	CAPlateOutBMFormat	*pOut		= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CCentSeparation		*pGuard		= NULL;

	CString	sText			= _T("");
	CString	szSheetName		= _T("");
	CString	szCopySheet		= _T("");
	long	nSttRow			= 10;
	long	nGuardsu		= pDB->GetGuardWallSu();
	long	nType			= 0;
	long	nGuardIdx		= 0;
	double	dBrLen			= 0;
	double	dBrMatchLen		= 0;
	double	dW1				= 0;
	double	dW2				= 0;
	double	dW3				= 0;
	double	dW4				= 0;
	double	dW5				= 0;
	double	dH1				= 0;
	double	dH2				= 0;
	double	dH3				= 0;
	double	dWingLeftStt	= pBMOpt->m_structWingWall.m_dLengthLeftStt;
	double	dWingLeftEnd	= pBMOpt->m_structWingWall.m_dLengthLeftEnd;
	double	dWingRighStt	= pBMOpt->m_structWingWall.m_dLengthRighStt;
	double	dWingRighEnd	= pBMOpt->m_structWingWall.m_dLengthRighEnd;
	CString	szCellConBuy	= _T("=");		// ��ũ��Ʈ����
	CString	szCellConPlc	= _T("=");		// ��ũ��ƮŸ��
	CString	szCellMoldSlop	= _T("=");	// ��Ǫ��-�����
	CString	szCellMoldVer	= _T("=");	// ��Ǫ��-����

	for(long nGuard=0; nGuard<nGuardsu; nGuard++)
	{
		nSttRow = 5;
		pGuard	= pDB->GetGuardFence(nGuard);
		dW1		= pGuard->m_W1;
		dW2		= pGuard->m_W2;
		dW3		= pGuard->m_W3;
		dW4		= pGuard->m_W4;
		dW5		= pGuard->m_W5;
		dH1		= pGuard->m_H1;
		dH2		= pGuard->m_H2;
		dH3		= pGuard->m_H3;

		szSheetName = GetGuardFenceSheetName(nGuard);
		long	nGuardMatch = -1;
		BOOL	bSameGuard	= IsSameGuardFence(nGuard, nGuardMatch);
		if(!bSameGuard || (bSameGuard && nGuard<nGuardMatch))
		{
			pXL->SetActiveSheet(szSheetName);
			szCopySheet.Format("%s%d", szSheetName, nGuardIdx+1);
			pXL->SheetMoveCopy(szSheetName, TRUE, szCopySheet);
			pXL->SetActiveSheet(szCopySheet);
			ArrDelSheet.Add(szCopySheet);
			dBrLen	= pBrInfo->GetBridgeLength(nGuard);
			if(nGuard == 0)
			{
				sText.Format("=%.3f+%.3f+%.3f", toM(pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT)), toM(dWingLeftStt), toM(dWingLeftEnd));
				if(bSameGuard && nGuardMatch==nGuardsu-1)
				{
					sText.Format("=%.3f+%.3f+%.3f+%.3f+%.3f+%.3f", toM(pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABLEFT)), toM(dWingLeftStt), toM(dWingLeftEnd),
						toM(pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT)), toM(dWingRighStt), toM(dWingRighEnd));
				}
			}
			else if(nGuard == nGuardsu-1)
			{
				sText.Format("=%.3f+%.3f+%.3f", toM(pADeckData->GetLengthSlabTotal(JONGBASEPOS_SLABRIGHT)), toM(dWingRighStt), toM(dWingRighEnd));
			}
			else
			{
				sText.Format("%.3f", toM(dBrLen));
				
				if(bSameGuard)
				{
					dBrMatchLen	= pBrInfo->GetBridgeLength(nGuardMatch);
					sText.Format("=%.3f+%.3f", toM(dBrLen), toM(dBrMatchLen));
				}
			}
			pOut->SetXL(pXL, sText,	nSttRow,  "X", "AA", 1, RED, 9, FALSE, TA_CENTER, "BR_LENGTH", 3);
			sText.Format("'%s'!X5", szCopySheet);
			m_MapGuardCell[szCopySheet] = sText;	// ��������� ������

			nSttRow = 10;
			sText.Format("%.3f", dW1);
			pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "W1", 0);
			sText.Format("%.3f", dW2);
			pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "W2", 0);
			sText.Format("%.3f", dW3);
			pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "W3", 0);
			if(szSheetName == _T("����+����") && nGuard == nGuardsu-1)
			{
				sText.Format("=(%s+%s-(%s+%.3f))", pOut->GetCellRef("W1"), pOut->GetCellRef("W2"), pOut->GetCellRef("W3"), dW4);
				pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "", 0);
			}
			else
			{
				sText.Format("%.3f", dW4);
				pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "", 0);			
			}
			sText.Format("%.3f", dW5);
			pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "", 0);
			sText.Format("%.3f", dH1);
			pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "", 0);
			sText.Format("%.3f", dH2);
			pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "", 0);
			sText.Format("%.3f", dH3);
			pOut->SetXL(pXL, sText,	nSttRow, "AG", "AJ", 1, RED, 9, FALSE, TA_CENTER, "", 0);

			sText.Format("(19-%.0f-15)", pDB->GetGuardFence(nGuard)->GetRebar()->GetFck()/10);
			long nTemp = 22;
			pOut->SetXL(pXL, sText,	nTemp/*nSttRow*/, "A", "F", 0, BLACK, 9, FALSE, TA_CENTER);

			if(szSheetName != _T("��ȣ��B"))
			{
				if(szSheetName != _T("��ȣ��C"))
				{
					if(nGuardIdx == 0)	sText.Format("'%s'!AN27", szCopySheet);
					else				sText.Format("+'%s'!AN27", szCopySheet);
					szCellConBuy+=sText;
					
					if(nGuardIdx == 0)	sText.Format("'%s'!AN31", szCopySheet);
					else				sText.Format("+'%s'!AN31", szCopySheet);
					szCellConPlc+=sText;
					
					if(nGuardIdx == 0)	sText.Format("'%s'!AI37", szCopySheet);
					else				sText.Format("+'%s'!AI37", szCopySheet);
					szCellMoldSlop+=sText;
					
					if(nGuardIdx == 0)	sText.Format("'%s'!AI44", szCopySheet);
					else				sText.Format("+'%s'!AI44", szCopySheet);
					szCellMoldVer+=sText;
					
					nSttRow = 35;
					if(pADeckData->m_nTypeBMApply==0)
					{
						pOut->SetXL(pXL, "(����3ȸ)",	nSttRow, "A", "F", 0, BLACK, 9, TRUE, TA_CENTER);		
					}

				}
				else
				{
					if(nGuardIdx == 0)	sText.Format("'%s'!AN25", szCopySheet);
					else				sText.Format("+'%s'!AN25", szCopySheet);
					szCellConBuy+=sText;
					
					if(nGuardIdx == 0)	sText.Format("'%s'!AN29", szCopySheet);
					else				sText.Format("+'%s'!AN29", szCopySheet);
					szCellConPlc+=sText;
					
					if(nGuardIdx == 0)	sText.Format("'%s'!AI34", szCopySheet);
					else				sText.Format("+'%s'!AI34", szCopySheet);
					szCellMoldSlop+=sText;
					
					if(nGuardIdx == 0)	sText.Format("'%s'!AI38", szCopySheet);
					else				sText.Format("+'%s'!AI38", szCopySheet);
					szCellMoldVer+=sText;
					
					nSttRow = 35;
					if(pADeckData->m_nTypeBMApply==0)
					{
						pOut->SetXL(pXL, "(����3ȸ)",	nSttRow, "A", "F", 0, BLACK, 9, TRUE, TA_CENTER);		
					}

				}
				
			}
			else
			{
				if(nGuardIdx == 0)	sText.Format("'%s'!AN24", szCopySheet);
				else				sText.Format("+'%s'!AN24", szCopySheet);
				szCellConBuy+=sText;

				if(nGuardIdx == 0)	sText.Format("'%s'!AN28", szCopySheet);
				else				sText.Format("+'%s'!AN28", szCopySheet);
				szCellConPlc+=sText;

				if(nGuardIdx == 0)	sText.Format("'%s'!AI32", szCopySheet);
				else				sText.Format("+'%s'!AI32", szCopySheet);
				szCellMoldVer+=sText;

				nSttRow = 32;
				if(pADeckData->m_nTypeBMApply==0)
				{
					pOut->SetXL(pXL, "(����3ȸ)",	nSttRow, "A", "F", 0, BLACK, 9, TRUE, TA_CENTER);		
				}
			}
			nGuardIdx++;
		}
	}

	m_MapGuardCell["CON_BUY"]	= szCellConBuy;
	m_MapGuardCell["CON_PLC"]	= szCellConPlc;
	m_MapGuardCell["MOLD_SLOP"]	= szCellMoldSlop==_T("=") ? "" : szCellMoldSlop;
	m_MapGuardCell["MOLD_VER"]	= szCellMoldVer;
}

void CAPlateBMXL::ConfinementConcrete(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	CAPlateOutBMBrInfo	*pBrInfo	= GetBridgeInfo(0);
	CADeckData			*pADeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CBMOption			*pBMOpt		= &pADeckData->m_BMOption;
	CAPlateOutBMFormat	*pOut		= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CPlateBasicIndex	*pBxPos		= pGir->GetBxOnJijum(0);
	CPlateBasicIndex	*pBxNeg		= pGir->GetBxOnJijum(1);

	pXL->SetActiveSheet("������ũ��Ʈ");
	ArrDelSheet.Add("������ũ��Ʈ");

	CString	szText	= _T("");
	long	nRow	= 5;
	double	dStaPos	= pBxPos->GetStation();
	double	dStaNeg	= pBxNeg ? pBxNeg->GetStation() : 0;
	double	dBuPos	= pGir->GetWidthOnStation(dStaPos, TRUE);
	double	dTuPos	= pGir->GetThickFactByStation(dStaPos, G_F_U);
	double	dLenPos	= pBrInfo->GetUpperLengthAver(TRUE);

	double	dBuNeg	= pBxNeg ? pGir->GetWidthOnStation(dStaNeg, TRUE) : 0;
	double	dTuNeg	= pBxNeg ? pGir->GetThickFactByStation(dStaNeg, G_F_U) : 0;
	double	dLenNeg	= pBrInfo->GetUpperLengthAver(FALSE);
	double	dTw		= pGir->GetThickFactByStation(dStaPos, G_W);
	double	dStrup	= 200;
	CDPoint sttAngle	= pDB->m_vAngJijum[0];
	CDPoint	endAngle	= pDB->m_vAngJijum[pDB->m_nQtyJigan];

	CDomyun		Dom;
	Dom.SetCWnd(m_pAPlateOutBMStd->GetWnd());
	Dom.SetScaleDim(45);
	pBrInfo->DrawCrossRebarBindConc(&Dom);
	pOut->InsertDomyun(pXL, &Dom, nRow, 25, 12, FALSE, TRUE, FALSE);

	nRow = 3;
	szText.Format("%d", pDB->GetQtyGirder());
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.3f", toM(pDB->m_BindConc.m_dWidth));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(pDB->m_BindConc.m_dHeight));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 2, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dBuPos));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dTuPos));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dLenPos));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dBuNeg));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dTuNeg));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dLenNeg));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dTw));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(pDB->m_BindConc.m_dDis-dTuPos));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", toM(dStrup));
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", sttAngle.GetAngleDegree());
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.3f", endAngle.GetAngleDegree());
	pOut->SetXL(pXL, szText,	nRow, "AZ", "BC", 1, RED, 9, FALSE, TA_CENTER, "", 3);
}

long CAPlateBMXL::GetDiaIdx(CString szDia)
{
	if(szDia == _T("H13"))			return DIA_H13;
	else if(szDia == _T("H16"))		return DIA_H16;
	else if(szDia == _T("H19"))		return DIA_H19;
	else if(szDia == _T("H22"))		return DIA_H22;
	else if(szDia == _T("H25"))		return DIA_H25;
	else if(szDia == _T("H29"))		return DIA_H29;
	else if(szDia == _T("H32"))		return DIA_H32;
	else if(szDia == _T("H35"))		return DIA_H35;
	if(szDia == _T("D10"))			return DIA_D10;
	else if(szDia == _T("D13"))		return DIA_D13;
	else if(szDia == _T("D16"))		return DIA_D16;
	else if(szDia == _T("D19"))		return DIA_D19;
	else if(szDia == _T("D22"))		return DIA_D22;
	else if(szDia == _T("D25"))		return DIA_D25;
	else if(szDia == _T("D29"))		return DIA_D29;
	else if(szDia == _T("D32"))		return DIA_D32;
	else if(szDia == _T("D35"))		return DIA_D35;

	return -1;
}

void CAPlateBMXL::SetProgress(CString strText, long nlW)
{
	OnProgress(0, nlW);
	OnProgressText(1, strText);
}

CAPlateOutBMBrInfo* CAPlateBMXL::GetBridgeInfo(long nBridgeIdx)
{
	return m_pAPlateOutBMBrInfo;
}

//	nValueHDan = 0  =>  ���͸�(������ ���� ���)
//	nValueHDan = 1  =>  ��ȣ��
//	nValueHDan = 2  =>  �ߺд�
//	nValueHDan = 3  =>  ����
//	nPos = 0 => ����
//	nPos = 1 => ����
//	nPos = 2 => �߾Ӻ�
void CAPlateBMXL::WriteSlabEctLength(CXLControl *pXL, long &nSttRow)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CPlateGirderApp		*pGir		= pDB->GetGirder(-1);	//���� �߽� �Ŵ�
	CADeckData			*pADeckData	= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CAPlateOutBMBrInfo	*pBrInfo	= GetBridgeInfo(0);
	CAPlateOutBMFormat	*pOut		= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
	CString	sText		= _T("");
	CString	sCell1		= _T("");
	CString	sCell2		= _T("");
	double	dLength		= 0;
	long	nQtyHDan	= pDB->GetQtyHDanAct(pBx->GetStation());
	long	nRow		= nSttRow;

	for(long nH=0; nH<nQtyHDan; nH++)
	{
		long nType    = pDB->GetValueTypeHDan(nH, 1);
		dLength	= pDB->GetLengthHDanDom(pBx, nH);
		sText.Format("%.3f", toM(dLength));
		
		switch (nType)
		{
		case 1 :		// ��ȣ��
			pOut->SetLineBoxText(pXL, "��ȣ��",	nSttRow, "BF", "BJ", 1, BLACK);
			pOut->SetLineBoxText(pXL, "",		nSttRow, "BK", "BO", 1, BLACK);
			pOut->SetXL(pXL, sText,				nSttRow, "BK", "BN", 0, RED,   9, FALSE, TA_CENTER, "", 3);
			pOut->SetXL(pXL, "m",				nSttRow, "BO", "BO", 0, BLACK);
			nSttRow++;
			break;
		case 2 :		// �ߺд�
			pOut->SetLineBoxText(pXL, "�ߺд�",	nSttRow, "BF", "BJ", 1, BLACK);
			pOut->SetLineBoxText(pXL, "",		nSttRow, "BK", "BO", 1, BLACK);
			pOut->SetXL(pXL, sText,				nSttRow, "BK", "BN", 0, RED,   9, FALSE, TA_CENTER, "", 3);
			pOut->SetXL(pXL, "m",				nSttRow, "BO", "BO", 0, BLACK);
			nSttRow++;
			break;
		case 3 :		// ����
			pOut->SetLineBoxText(pXL, "�� ��",	nSttRow, "BF", "BJ", 1, BLACK);
			pOut->SetLineBoxText(pXL, "",		nSttRow, "BK", "BO", 1, BLACK);
			pOut->SetXL(pXL, sText,				nSttRow, "BK", "BN", 0, RED,   9, FALSE, TA_CENTER, "LEN_WALKROAD", 3);
			pOut->SetXL(pXL, "m",				nSttRow, "BO", "BO", 0, BLACK);
			nSttRow++;
			break;
		}		
	}

	pOut->SetLineBoxText(pXL, "�� ��",	nSttRow, "BF", "BJ", 1, BLACK);
	pOut->SetLineBoxText(pXL, "",		nSttRow, "BK", "BO", 1, BLACK);

	sCell1.Format("BK%d", nRow);	sCell2.Format("BN%d", nSttRow-1);
	sText.Format("=SUM(%s:%s)", sCell1, sCell2);
	pOut->SetXL(pXL, sText,				nSttRow, "BK", "BN", 0, RED,   9, FALSE, TA_CENTER, "ETC_SUM", 3);
	pOut->SetXL(pXL, "m",				nSttRow, "BO", "BO", 0, BLACK);
}

void CAPlateBMXL::WriteCrossBeam(CXLControl *pXL, long &nSttRow)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CAPlateOutBMFormat	*pOut		= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CAPlateOutBMBrInfo	*pBrInfo	= GetBridgeInfo(0);

	CString	sText		= _T("");
	CString	sCell1		= _T("");
	CString sCell2		= _T("");
	long	nGirdersu	= pDB->GetGirdersu();
	BOOL	bCrossBeam	= FALSE;
	if(nGirdersu > 1)	bCrossBeam	= TRUE;

	nSttRow-=3;
	if(nGirdersu > 1)
		pOut->SetXL(pXL, "< �� �� �� >",	nSttRow, "AD", "AO", 2, BLACK, 9, FALSE, TA_CENTER);

	nSttRow++;
	sText.Format("AA%d", nSttRow);
	pXL->CopyPicture(m_pImageXL, "BM_CROSSBEAM", sText);

	nSttRow--;
	pOut->SetLineBoxText(pXL, "�� ��",			nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "�ּ� ����",		nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "�ִ� ����",		nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "��ġ �ּҳ���",	nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "��ġ �ִ����",	nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "��ġ ��ճ���",	nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "��ġ ������",	nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "��ġ�� ����",	nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "��ġ�� �ϸ���",	nSttRow, "AU", "AZ", 1);	nSttRow++;
	pOut->SetLineBoxText(pXL, "��ġ�� �����",	nSttRow, "AU", "AZ", 1);

	nSttRow-=9;

	if(bCrossBeam)
	{
		long	nQtyHunch		= 0;
		double	dCRMinLength	= 0;//toM(pBrInfo->GetLengthCrossBeam(FALSE));
		double	dCRMaxLength	= 0;//toM(pBrInfo->GetLengthCrossBeam(TRUE));				
		double	dHunchMinHeight	= 0;//toM(pBrInfo->GetHeightCrossBeamHunch(FALSE));
		double	dHunchMaxHeight	= 0;//toM(pBrInfo->GetHeightCrossBeamHunch(TRUE));
		double	dHunchLowWidth	= 0;//toM(pBrInfo->GetWidthCrossBeamLow());

		pBrInfo->GetCrossBeamHunch(dCRMaxLength, dCRMinLength, nQtyHunch, dHunchMinHeight, dHunchMaxHeight, dHunchLowWidth);

		dCRMaxLength	= toM(dCRMaxLength);
		dCRMinLength	= toM(dCRMinLength);
		dHunchMaxHeight = toM(dHunchMaxHeight);
		dHunchMinHeight = toM(dHunchMinHeight);
		dHunchLowWidth	= toM(dHunchLowWidth);		

		//long	nQtyHunch		= pBrInfo->GetQtyCrossBeamHunch();
		pOut->SetLineBoxText(pXL, "�� �� ��",		nSttRow, "BA", "BD", 1);														nSttRow++;
		pOut->SetLineBoxText(pXL, dCRMinLength,		nSttRow, "BA", "BD", 1, RED, 9, FALSE, TA_CENTER, "CR_MIN_LENGTH", 3);			nSttRow++;	// �ּұ���
		pOut->SetLineBoxText(pXL, dCRMaxLength,		nSttRow, "BA", "BD", 1, RED, 9, FALSE, TA_CENTER, "CR_MAX_LENGTH", 3);			nSttRow++;	// �ִ� ����
		pOut->SetLineBoxText(pXL, dHunchMinHeight,	nSttRow, "BA", "BD", 1, RED, 9, FALSE, TA_CENTER, "CR_HU_MIN_HEIGHT", 3);		nSttRow++;	// ��ġ �ּ� ����
		pOut->SetLineBoxText(pXL, dHunchMaxHeight,	nSttRow, "BA", "BD", 1, RED, 9, FALSE, TA_CENTER, "CR_HU_MAX_HEIGHT", 3);		nSttRow++;	// ��ġ �ִ� ����
		sText.Format("=(%s+%s)/2", pOut->GetCellRef("CR_HU_MIN_HEIGHT"), pOut->GetCellRef("CR_HU_MAX_HEIGHT"));
		pOut->SetLineBoxText(pXL, sText,			nSttRow, "BA", "BD", 1, BLACK, 9, FALSE, TA_CENTER, "CR_HU_AVE_HEIGHT", 3);		nSttRow++;	// ��ġ ��ճ���
		sText.Format("=(%s+%s)/2", pOut->GetCellRef("CR_HU_MIN_HEIGHT"), pOut->GetCellRef("CR_HU_MAX_HEIGHT"));
		pOut->SetLineBoxText(pXL, sText,			nSttRow, "BA", "BD", 1, BLACK, 9, FALSE, TA_CENTER, "CR_HU_SLOW_LENGTH", 3);	nSttRow++;	// ��ġ ������
		sText.Format("%d", nQtyHunch);
		pOut->SetLineBoxText(pXL, sText,			nSttRow, "BA", "BD", 1, RED, 9, FALSE, TA_CENTER, "CR_HU_QTY", 0);				nSttRow++;	// ��ġ�� ����
		pOut->SetLineBoxText(pXL, dHunchLowWidth,	nSttRow, "BA", "BD", 1, RED, 9, FALSE, TA_CENTER, "CR_HU_LOW_WIDTH", 3);		nSttRow++;	// ��ġ�� �ϸ���
		sText.Format("=(%s+%s)/2", pOut->GetCellRef("CR_HU_MIN_HEIGHT"), pOut->GetCellRef("CR_HU_MAX_HEIGHT"));
		pOut->SetLineBoxText(pXL, sText,			nSttRow, "BA", "BD", 1, BLACK, 9, FALSE, TA_CENTER, "CR_HU_UP_LENGTH", 3);				// ��ġ�� �����

		// �׸� ġ��
		nSttRow-=7;
		pOut->SetXL(pXL, "������",		nSttRow, "AH", "AJ", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*1000", pOut->GetCellRef("CR_HU_MAX_HEIGHT"));
		pOut->SetXL(pXL, sText,			nSttRow,  "Z",  "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		sCell1.Format("Z%d", nSttRow-1);	sCell2.Format("Z%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);
		nSttRow++;
		pOut->SetXL(pXL, "'1:3",		nSttRow, "AE", "AF", 0, BLACK);
		pOut->SetXL(pXL, "~",			nSttRow, "Z", "Z", 2, BLACK);
		pOut->SetXL(pXL, "��ձ���",	nSttRow, "AN", "AN", 1, BLACK);
		pOut->SetXL(pXL, "L=",			nSttRow, "AN", "AN", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=(%s+%s)/2", pOut->GetCellRef("CR_MIN_LENGTH"), pOut->GetCellRef("CR_MAX_LENGTH"));
		pOut->SetXL(pXL, sText,			nSttRow, "AO", "AR", 0, BLACK, 9, FALSE, TA_CENTER, "CR_AVE_LENGTH", 3);
		pOut->SetXL(pXL, "��ձ���",	nSttRow, "AB", "AB", 0);
		sText.Format("=ROUND(3*(%s+%s)/2,3)*1000", pOut->GetCellRef("CR_HU_MIN_HEIGHT"), pOut->GetCellRef("CR_HU_MAX_HEIGHT"));
		pOut->SetXL(pXL, sText,			nSttRow, "AF", "AG", 0, BLACK, 9, FALSE, TA_CENTER, "CR_AVE_HUNCH", 0);
		sText.Format("=%s*1000", pOut->GetCellRef("CR_HU_LOW_WIDTH"));
		pOut->SetXL(pXL, sText,			nSttRow, "AH", "AJ", 0, BLACK, 9, FALSE, TA_CENTER, "CR_HU_LOW_WIDTH_IMG", 0);
		sText.Format("=ROUND(3*(%s+%s)/2,3)*1000", pOut->GetCellRef("CR_HU_MIN_HEIGHT"), pOut->GetCellRef("CR_HU_MAX_HEIGHT"));
		pOut->SetXL(pXL, sText,			nSttRow, "AK", "AL", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);

		nSttRow--;
		sText.Format("=%s*1000", pOut->GetCellRef("CR_HU_MIN_HEIGHT"));
		pOut->SetXL(pXL, sText,			nSttRow, "Z", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		sCell1.Format("Z%d", nSttRow-1);	sCell2.Format("Z%d", nSttRow);
		pXL->SetMergeCell(sCell1, sCell2);
		nSttRow++;

		sText.Format("=SQRT((%s*1000)^2+%s^2)/1000", pOut->GetCellRef("CR_HU_AVE_HEIGHT"), pOut->GetCellRef("CR_AVE_HUNCH"));
		pOut->SetLineBoxText(pXL, sText,			nSttRow, "BA", "BD", 1, BLACK, 9, FALSE, TA_CENTER, "CR_HU_SLOW_LENGTH", 3);	// ��ġ ������
		nSttRow+=3;
		sText.Format("=(%s*2+%s)/1000", pOut->GetCellRef("CR_AVE_HUNCH"), pOut->GetCellRef("CR_HU_LOW_WIDTH_IMG"));
		pOut->SetLineBoxText(pXL, sText,			nSttRow, "BA", "BD", 1, BLACK, 9, FALSE, TA_CENTER, "CR_HU_UP_LENGTH", 3);				// ��ġ�� �����

	}
}

void CAPlateBMXL::WriteSlabDimTable(CXLControl *pXL, long &nSttRow)
{
	CAPlateOutBMBrInfo	*pBrInfo	= GetBridgeInfo(0);
	CAPlateOutBMFormat	*pOut		= m_pAPlateOutBMStd->GetAPlateOutBMFormat();

	CString			sText = _T("");
	CVectorArray	ptArr;
	pBrInfo->GetSlabVerticalDimensions(ptArr);

	sText.Format("%.3f", toM(ptArr.GetAt(0).x));
	pOut->SetXL(pXL, sText,		nSttRow, "L", "O", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(1).x));
	pOut->SetXL(pXL, sText,		nSttRow, "P", "S", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(2).x));
	pOut->SetXL(pXL, sText,		nSttRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(3).x));
	pOut->SetXL(pXL, sText,		nSttRow, "X","AA", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(4).x));
	pOut->SetXL(pXL, sText,		nSttRow,"AB","AE", 1, RED, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", toM(ptArr.GetAt(0).y));
	pOut->SetXL(pXL, sText,		nSttRow, "L", "O", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(1).y));
	pOut->SetXL(pXL, sText,		nSttRow, "P", "S", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(2).y));
	pOut->SetXL(pXL, sText,		nSttRow, "T", "W", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(3).y));
	pOut->SetXL(pXL, sText,		nSttRow, "X","AA", 0, RED, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", toM(ptArr.GetAt(4).y));
	pOut->SetXL(pXL, sText,		nSttRow,"AB","AE", 0, RED, 9, FALSE, TA_CENTER);

}

BOOL CAPlateBMXL::IsSameGuardFence(long nGuardFence, long &nGuardMatch)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CCentSeparation		*pSep	= NULL;

	BOOL	bSameGuard	= FALSE;
	double	dArea		= pDB->GetGuardFence(nGuardFence)->GetArea();
	long	nGuardsu	= pDB->GetGuardWallSu();
	double	dCurArea	= 0;

	for(long nGuard = 0; nGuard < nGuardsu; nGuard++)
	{
		pSep		= pDB->GetGuardFence(nGuard);
		dCurArea	= pSep->GetArea();
		if(nGuardFence != nGuard && dCurArea == dArea)
		{	
			bSameGuard	= TRUE;
			nGuardMatch	= nGuard;
		}
	}
	return bSameGuard;
}

CString CAPlateBMXL::GetGuardFenceSheetName(long nGuard)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutBMStd->GetBridgeApp();
	CCentSeparation	*pSep	= pDB->GetGuardFence(nGuard);

	CString	szSheetName = _T("");

	BOOL	bHeight	= FALSE;
	BOOL	bWidth	= FALSE;
	double	dW1	= pSep->m_W1;
	double	dW2	= pSep->m_W2;
	double	dW3	= pSep->m_W3;
	double	dW4	= pSep->m_W4;
	double	dW5	= pSep->m_W5;
	double	dH1	= pSep->m_H1;
	double	dH2	= pSep->m_H2;
	double	dH3	= pSep->m_H3;

	if((dW1!=0 && dW2==0 && dW3==0 && dW4==0 && dW5==0) || (dW1==0 && dW2!=0 && dW3==0 && dW4==0 && dW5==0) || (dW1==0 && dW2==0 && dW3!=0 && dW4==0 && dW5==0) ||
			(dW1==0 && dW2==0 && dW3==0 && dW4!=0 && dW5==0) || (dW1==0 && dW2==0 && dW3==0 && dW4==0 && dW5!=0))
			bWidth = TRUE;
	if((dH1!=0 && dH2==0 && dH3==0) || (dH1==0 && dH2!=0 && dH3==0) || (dH1==0 && dH2==0 && dH3!=0))
			bHeight = TRUE;

	if(pSep->m_nDirType == -1)	// ���� ��ȣ��
	{
		if(dW1==0 && dW2==0 && dW3!=0 && dW4!=0 && dW5!=0 && dH1!=0 && dH2!=0 && dH3!= 0)
			szSheetName	= _T("��ȣ��A");
		else if(dW1!=0 && dW2!=0 && dW3!=0 && dW4!=0 && dW5!=0 && dH1!=0 && dH2!=0 && dH3!=0)
		{
			szSheetName	= _T("�ߺд�");			
		}
		else if(dW1 == 0 && dW2 == 0 && dW3 != 0 && dW4 == 0 && dW5 != 0 && dH1 == 0 && dH2 != 0 && dH3 == 0)
		{
			szSheetName = _T("��ȣ��C");
		}
		else
		{
			if(bWidth == TRUE && bHeight == TRUE)
				szSheetName	= _T("��ȣ��B");
		}
	}
	else
	{
		if(dW1!=0 && dW2!=0 && dW3!=0 && dW4==0 && dW5==0 && dH1!=0 && dH2!=0 && dH3!= 0)
			szSheetName	= _T("��ȣ��A");
		else if(dW1!=0 && dW2!=0 && dW3!=0 && dW4!=0 && dW5!=0 && dH1!=0 && dH2!=0 && dH3!=0)
		{
			szSheetName	= _T("�ߺд�");		
		}
		else if(dW1 != 0 && dW2 == 0 && dW3 != 0 && dW4 == 0 && dW5 == 0 && dH1 == 0 && dH2 != 0 && dH3 == 0)
		{
			szSheetName = _T("��ȣ��C");
		}
		else
		{
			if(bWidth == TRUE && bHeight == TRUE)
				szSheetName	= _T("��ȣ��B");
		}
	}

	return szSheetName;
}

//////////////////////////////////////////////////////////////////////////
// ��ȣ���� ������ ö��
// long nType : ���� ��� 
// long nType - 0 : ����
// long nType - 1 : ��ö�ٸ�
// long nType - 2 : ��Ÿö�ٸ�
void CAPlateBMXL::MakeRebarList(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar)
{
	CPlateBridgeApp	*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CADeckData		*pDeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CADeckDataTU	*pDeckDataTU	= m_pAPlateOutBMStd->GetDataManage()->GetDeckDataTU();
	CRebar			*pRB			= NULL;

	BOOL	bTu		= pDB->IsTUGir();
	BOOL	bOK		= FALSE;
	long	nSize	= bTu ? pDeckDataTU->m_arrRebarJewonTU.GetSize() : pDeckData->m_arrRebarJewon.GetSize();

	for(long nList=0; nList<nSize; nList++)
	{
		pRB = bTu ? pDeckDataTU->m_arrRebarJewonTU.GetAt(nList) : pDeckData->m_arrRebarJewon.GetAt(nList);
		if(pRB->m_sMark == "" || pRB->m_dDia==0 || pRB->m_nEa==0 || pRB->GetLength()==0) continue;

		if(nType == 0) bOK = TRUE;
		else if(nType == 1) bOK = pRB->m_bMainRebar ? TRUE : FALSE;
		else if(nType == 2) bOK = pRB->m_bMainRebar ? FALSE : TRUE;
		else bOK = FALSE;

		if(bOK)
			pArrRebar->Add(new CRebar(pRB));
	}

}

//////////////////////////////////////////////////////////////////////////
// ��ȣ�� ö��
// long nType : ���� ��� 
// long nType - 0 : ����
// long nType - 1 : ��ö�ٸ�
// long nType - 2 : ��Ÿö�ٸ�
void CAPlateBMXL::MakeRebarListGuard(long nType, CSafetyTypedPtrArray<CObArray, CRebar*> *pArrRebar)
{
	CADeckData	*pDeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();

	CRebar *pRB;
	BOOL bOK = FALSE;

	long nSize = pDeckData->m_arrRebarJewonGuard.GetSize();
	for(long nList=0; nList<nSize; nList++)
	{
		pRB = pDeckData->m_arrRebarJewonGuard.GetAt(nList);
		if(pRB->m_sMark == "" || pRB->m_dDia==0 || /*pRB->m_nEa==0 ||*/ pRB->GetLength()==0) continue;

		if(nType == 0) bOK = TRUE;
		else if(nType == 1) bOK = pRB->m_bMainRebar ? TRUE : FALSE;
		else if(nType == 2) bOK = pRB->m_bMainRebar ? FALSE : TRUE;
		else bOK = FALSE;
		
		if(bOK)
			pArrRebar->Add(new CRebar(pRB));
	}

}
///////////////////////////////////////////////////////////////////////////////////
//��š񢱢����� ���¥åĥťƥǥ�������ҥ��ɥ��������f ����������
//	���£��������¡áġҡ����� �� �� �� kgf/�� t��m  ����������������
//  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ������������¨èĨ�
//  ��������������������������  �� �� �Ѣ�
//  �� �� �� ������ �� ���� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
///////////////////////////////////////////////////////////////////////////////////// 