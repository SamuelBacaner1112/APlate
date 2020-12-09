// APlateWeldChangeXL.cpp : implementation file
//

#include "stdafx.h"
#include "APlateOutBM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define NONDES_STANDARD				0
#define NONDES_CALC_FOUNDATION		1
#define NONDES_SUMMARY				2

#define	OUTBMBLACK					1
#define RED							3
#define	BLUE						5
#define	YELLOW						6
#define GRAY						15
#define NEXT						1

/////////////////////////////////////////////////////////////////////////////
// CAPlateWeldChangeXL

CAPlateWeldChangeXL::CAPlateWeldChangeXL(CAPlateOutBMStd *pAPlateOutBMStd)
{
	m_pAPlateOutBMStd		= pAPlateOutBMStd;
	CWinApp	*pApp			= AfxGetApp();
	CString	csSection		= "Afx Directory"; 
	CString	csFixedItem		= "Fixed Directory";
	CString	csAPlate		= pApp->GetProfileString(csSection, csFixedItem);

	m_szReadFileName		= csAPlate + "\\OutXLWeldChange.dll";	
	m_pAPlateOutBMBrInfo = new CAPlateOutBMBrInfo(pAPlateOutBMStd);
}

CAPlateWeldChangeXL::~CAPlateWeldChangeXL()
{
	delete m_pAPlateOutBMBrInfo;
}

UINT CAPlateWeldChangeXL::DoWork()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CStringArray		ArrDelSheet;
	CXLControl			XL;
	
	KillTheXL();
	
	long	nProgress	= 0;
	CString	sText		= _T("");
	
	
	SetProgress("출력중...", nProgress);
	
	try
	{
		
#ifdef _DEBUG
		XL.SetXLVisible(TRUE);
#endif	
		if(XL.OpenXL(m_szReadFileName) == FALSE)	return 1;

		APlateOutWeldChangeDes(&XL);
		m_pAPlateOutBMStd->m_dXLVersion	= XL.GetVersion();
		XL.SaveAs(m_szSaveFilePath);
		XL.QuitXL();
		
		SetProgress("출력완료", 100);
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
			OnProgressText(1, "출력완료...");
			m_pAPlateOutBMStd->GetAPlateOutWeldChangeDlg()->m_ctrlProgress.SetPos(0);
			
			theException->Delete();
			return 1;
		}		
	}
	
	CAPlateOutWeldChangeDlg	*pAPlateOutWeldChangeDlg	= m_pAPlateOutBMStd->GetAPlateOutWeldChangeDlg();
	pAPlateOutWeldChangeDlg->GetDlgItem(IDC_BUTTON_WELD_OUT)->EnableWindow(TRUE);
	pAPlateOutWeldChangeDlg->GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText("출력대기중...");
	OnProgressText(1, "출력완료...");
	OnProgress(0, 0);
	return 0;
}


void CAPlateWeldChangeXL::KillTheXL()
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

CAPlateOutBMBrInfo* CAPlateWeldChangeXL::GetBridgeInfo(long nBridgeIdx)
{
	return m_pAPlateOutBMBrInfo;
}

void CAPlateWeldChangeXL::APlateOutWeldChangeDes(CXLControl	*pXL)
{
	if(m_bAbort)	return;

	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CGeneralBaseStd		*pStd			= m_pAPlateOutBMStd->GetGeneralInputStd();
	CAPlateOutBMBrInfo	*pBrInfo		= GetBridgeInfo(0);
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;
	CBMOutSheet			*pBMSheet		= pStd->GetBMOutSheet();
	CWeldOutSheet		*pWeldSheet		= pStd->GetWeldOutSheet();
	CBMCreator			*pBMStd			= pBMSheet->m_pDataManage->GetBMStandard();
	CWeldStandard		*pWeldStd		= pWeldSheet->m_pDataManage->GetWeldStandard();

	CPtrList			*pBMList		= &pBMStd->m_BMList_BujeKindTotal;	
	CPtrList			*pWeldList		= &pWeldStd->m_ListWeldThick;
	pXL->SetActiveSheet("표준제작공수");

	// 교량 종류선택
	BOOL	bSimpleBeam = (pDB->m_nQtyJigan<2)? TRUE : FALSE;
	long	nRow = (bSimpleBeam)? 7 : 8;
	CString	sText	= _T("");
	sText = "○";
	pXL->SetXL(nRow, 13, TA_CENTER, sText);

	
	// 부재 비중 산출
	double dWBig = 0, dWSmall = 0, dW570 = 0;
	POSITION pos = pBMList->GetHeadPosition();

	while (pos)
	{
		CBMRecord * pRec = (CBMRecord *)pBMList->GetNext(pos);
		if(pRec->GetStrMaterial().Find("570") != -1)	dW570	+= pRec->m_dTWeight;	// SM570
		else if(pRec->m_nSizeType == 1)					dWBig	+= pRec->m_dTWeight;	// 대형부재
		else											dWSmall	+= pRec->m_dTWeight;	// 소형부재
	}

	nRow	= 14;	
	sText.Format("%.3f", toTon(dW570));
	pXL->SetXL(nRow, 3, TA_CENTER, sText);
	sText.Format("%.3f", toTon(dWBig));
	pXL->SetXL(nRow, 5, TA_CENTER, sText);
	sText.Format("%.3f", toTon(dWSmall));
	pXL->SetXL(nRow, 7, TA_CENTER, sText);
	
	if(dW570 != 0)	pXL->DeleteRowLine(28, 33);
	else			pXL->DeleteRowLine(34, 39);
	pXL->DeleteRowLineEnd();
	nRow = 87;
	
	// 환산 용접장 산출근거
	long nRowFillet = 88, nRowEtc = 0, nWeldNum = 0;
	pos = pWeldList->GetHeadPosition();
	BOOL bFilletFirst = TRUE, bEtcFirst = TRUE;
	long nCount = 1;
	while( pos )
	{	
		long nProgress = (long)(double(nCount)/pWeldList->GetCount()*100);
		SetProgress("출력중...", nProgress);
		nCount++;
		CWeldRecord *pRec = (CWeldRecord *)pWeldList->GetNext(pos);
		
		if(pRec->m_sWeldType.Find("필렛") != -1)
		{
			if(!bFilletFirst)	pXL->InsertCopyRowLine(nRowFillet, nRowFillet, nRow+1);
			else				bFilletFirst = FALSE;
			nWeldNum = 1;
		}
		else
		{
			if (pRec->m_sWeldType.Find("플래어") !=-1)
			{
				pRec->m_sImproveType = "K형";
			}

			if(!bEtcFirst)	pXL->InsertCopyRowLine(nRowEtc, nRowEtc, nRow+1);
			else
			{
				bEtcFirst = FALSE;
				sText.Format("=SUM(M%d:O%d)", nRowFillet, nRow);
				pXL->SetXL(nRow, 12, TA_LEFT, sText);				
				nRowEtc = nRow+2;
				nRow++;
			}
			if(pRec->m_sImproveType.Find("I") != -1) nWeldNum = 2;
			if(pRec->m_sImproveType.Find("L") != -1) nWeldNum = 3;
			if(pRec->m_sImproveType.Find("V") != -1) nWeldNum = 4;
			if(pRec->m_sImproveType.Find("X") != -1) nWeldNum = 5;
			if(pRec->m_sImproveType.Find("K") != -1) nWeldNum = 6;
		}
		
		pXL->SetXL(nRow, 0, TA_CENTER, pRec->m_sWeldType);
		pXL->SetXL(nRow, 2, TA_CENTER, pRec->m_sImproveType);
		pXL->SetXL(nRow, 4, TA_CENTER, pRec->m_sLayType);
		
		sText.Format("%.f", pRec->m_dThickWeld);
		if(pRec->m_sImproveType.Find("K") != -1 && pRec->m_sWeldType.Find("T") != -1)
			sText.Format("%.f", pRec->m_dThickWeld*2+2);
		
		pXL->SetXL(nRow, 7, TA_CENTER, sText);
		sText.Format("%d", nWeldNum);
		pXL->SetXL(nRow, 8, TA_CENTER, sText);
		sText.Format("%f", toM(pRec->m_dLengthWeld));
		pXL->SetXL(nRow, 9, TA_CENTER, sText);
		nRow++;
	}
	sText.Format("=SUM(M%d:O%d)", nRowEtc, nRow);
	pXL->SetXL(nRow, 12, TA_LEFT, sText);
}

void CAPlateWeldChangeXL::SetProgress(CString strText, long nlW)
{
	OnProgress(0, nlW);
	OnProgressText(1, strText);
}
