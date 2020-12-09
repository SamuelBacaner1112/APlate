           // APlateOutExcelCheck.cpp: implementation of the CAPlateOutExcelCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define WINDOW_ORGWIDTH			592
#define WINDOW_ORGHEIGHT		290
#define TOTOUTXLSU				8
#define TOTCHECKLISTSU			12

CAPlateOutExcelCheck::CAPlateOutExcelCheck()
{
	m_bAllFiles	= FALSE;
}

CAPlateOutExcelCheck::~CAPlateOutExcelCheck()
{

}

UINT CAPlateOutExcelCheck::DoWork()
{
	CAPlateOutputDlg* pOutDlg = m_pAPlateOutCalcStd->GetAPlateOutputDlg();
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
					GetWindowThreadProcessId( pWnd->m_hWnd, &pid );
					HANDLE     hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
					TerminateProcess(hProcess, 0);
				}
			}
		}
		else 
		{
			break;
		}
		if(hWnd == ::GetDesktopWindow())
			hWnd = ::GetWindow(hWnd,GW_CHILD);
		else
			hWnd = ::GetWindow(hWnd,GW_HWNDNEXT);
		pWnd = CWnd::FromHandle(hWnd);
	}

	pOutDlg->SetDlgEnable(TRUE);

	long nPos = 0;
	long nType = pOutDlg->m_nType;
	long nSize = pOutDlg->m_nPageArr.GetSize();

 	BOOL bUser = FALSE;
	BOOL bCancel = FALSE;
	CString sXLSaveFile, sTitle;
	sXLSaveFile = m_sXLCheckSaveFile[0];

	long nOutXLsu = 1;
	if(m_bAllFiles) nOutXLsu = 12;

	CXLControl DArr[20];
	
	OnProgressText(1, "��� �غ�");
	OnProgressText(3, "��� �غ�");

	if(pOutDlg->m_bOneFiles)
	{
		int nResult = XLCheckOutAll();
		return nResult;
	}
	else
	{
		for(long nXLIdx = 0; nXLIdx < nOutXLsu; nXLIdx++)
		{
			nPos = 100 * (nXLIdx+1) / nOutXLsu;
			OnProgress(2, nPos);

			if(nType==4)	//���������
			{
				for(long i=0; i<nSize; i++)
				{
					long page = pOutDlg->m_nPageArr.GetAt(i);

					if(page==(nXLIdx+1)) bUser = TRUE;
				}
			}

			if(nType==4 && !bUser) continue;

			if(nXLIdx == 0) sTitle.Format("1. �ٴ���(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 1) sTitle.Format("2. �ݷ°���(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 2) sTitle.Format("3. ����Ʋ��(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 3) sTitle.Format("4. �ܸ����(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 4) sTitle.Format("5. �ܸ��൵(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 5) sTitle.Format("6. ��������(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 6) sTitle.Format("7. ��������(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 7) sTitle.Format("8. ���������(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 8) sTitle.Format("9. ���κ� �� �극�̽� ����(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 9) sTitle.Format("10. ���ܿ��������(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 10) sTitle.Format("11. ��뼺����(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 11) sTitle.Format("12. ������������(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 12) sTitle.Format("13. ��������(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 13) sTitle.Format("14. �ð���(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 14) sTitle.Format("15. ����ǥ �� EL(%d/%d)", nXLIdx + 1, nOutXLsu);
			if(nXLIdx == 15) sTitle.Format("16. NG LIST(%d/%d)", nXLIdx + 1, nOutXLsu);

			CXLControl *pXL = &DArr[nXLIdx];

			OnProgressText(3, sTitle);
	#ifdef _DEBUG
			pXL->SetXLVisible(TRUE);
	#endif

			try
			{
				OutXLSelect(nXLIdx, sXLSaveFile);

				if(pXL->NewXL() == FALSE)	 return 1;

				m_pAPlateOutCalcStd->m_dXLVersion = pXL->GetVersion();
				
				long lW  = OutAPlateExcelCheck(pXL, nXLIdx);
				bUser = FALSE;
				
				if(m_bAbort) 
				{
					bCancel = TRUE;
					pXL->QuitXL();
					OnProgressText(1, "����غ�");
					OnProgressText(3, "����غ�");
					break;
				}
				
				pXL->SaveAs(m_sXLCheckSaveFile[nXLIdx]);
				pXL->QuitXL();
			}
			catch(CException* theException)
			{
				TCHAR   szCause[255];
				CString strFormatted;

				strFormatted.Format("%s",szCause);
				long nResult = AfxMessageBox(strFormatted,MB_OK | MB_ICONSTOP);			
				
				if(nResult == IDOK)
				{
					OnProgressText(1, "");
					OnProgressText(3, "");

					pOutDlg->m_ctrlProgress1.SetPos(0);
					pOutDlg->m_ctrlProgress2.SetPos(0);
					
					theException->Delete();
					return 1;
				}
			}
		}	

		pOutDlg->SetDlgEnable(FALSE);
		pOutDlg->SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);

		OnProgress(0, 0);
		OnProgress(2, 0);
	}
	return 0;
}

int CAPlateOutExcelCheck::XLCheckOutAll()
{
	CAPlateOutputDlg* pOutDlg = m_pAPlateOutCalcStd->GetAPlateOutputDlg();	
	CXLControl	XL;
	CXLControl	*pXL		= &XL;
	BOOL		bCancel		= FALSE;
	long		nProgress	= 0;

	try
	{
#ifdef _DEBUG
		pXL->SetXLVisible(TRUE);
#endif
		CWinApp* pApp = AfxGetApp();
		CString csSection     = "Afx Directory";
		CString csFixedItem   = "Fixed Directory";
		CString csAplate = pApp->GetProfileString(csSection, csFixedItem);

		CString csFN = _T("\\AnalyxlCheck.dll");
		csAplate += csFN;
		if(!pXL->OpenXL(csAplate))	return 1;	// error

		nProgress  = OutAPlateExcelCheckAll(pXL);

		if(m_bAbort) 
		{
			bCancel = TRUE;
			pXL->QuitXL();
			OnProgressText(1, "����غ�");
			OnProgressText(3, "����غ�");
			return 1;
		}

		pXL->SaveAs(m_sXLCheckSaveFile[0]);
		pXL->QuitXL();
	}
	catch(CException* theException)
	{
		TCHAR   szCause[255];
		CString strFormatted;

		strFormatted.Format("%s",szCause);
		long nResult = AfxMessageBox(strFormatted,MB_OK | MB_ICONSTOP);			
		
		if(nResult == IDOK)
		{
			OnProgressText(1, "");
			OnProgressText(3, "");

			pOutDlg->m_ctrlProgress1.SetPos(0);
			pOutDlg->m_ctrlProgress2.SetPos(0);
			
			theException->Delete();
			return 1;
		}
	}

	pOutDlg->SetDlgEnable(FALSE);
	pOutDlg->SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);

	OnProgress(0, 0);
	OnProgress(2, 0);

	return 0;
}

int CAPlateOutExcelCheck::OutAPlateExcelCheck(CXLControl *pXL, long nXLIdx)
{
	CXLCheckListFloor		*pFloor		= m_pAPlateOutCalcStd->GetXLCheckListFloor();
	CXLCheckListSection		*pSection	= m_pAPlateOutCalcStd->GetXLCheckListSection();
	CXLCheckListStiff		*pStiff		= m_pAPlateOutCalcStd->GetXLCheckListStiff();
	CXLCheckListUsing		*pUsing		= m_pAPlateOutCalcStd->GetXLCheckListUsing();
	CXLCheckListCrossBeam	*pCross		= m_pAPlateOutCalcStd->GetXLCheckListCrossBeam();
	CXLCheckListStud		*pStud		= m_pAPlateOutCalcStd->GetXLCheckListStud();
	
	pFloor->SetXLControl(pXL);
	pSection->SetXLControl(pXL);
	pCross->SetXLControl(pXL);
	pUsing->SetXLControl(pXL);
	pStiff->SetXLControl(pXL);
	pStud->SetXLControl(pXL);

	long lW  = 0; 
	long nCurFile = m_nCurFileIdx[nXLIdx];
	CStringArray	ArrUsedSheet;

	if(nCurFile == 0)// 1. �ٴ���
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "1. �ٴ���");
		pXL->SetActiveSheet("Sheet1");
		pXL->SetSheetName("1. �ٴ���");
//		NewSheet(pXL, "1. �ٴ���", 0, "����ü");
		pFloor->Floor();
		ArrUsedSheet.Add("1. �ٴ���");
	}
	else if(nCurFile == 1)// 2. �ݷ°���
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "2. �ݷ°���");
		NewSheet(pXL, "2. �ݷ°���", 0, "����ü");
		pFloor->Reaction();
		ArrUsedSheet.Add("2. �ݷ°���");
	}
	else if(nCurFile == 2)// 3. ����Ʋ��
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "3. ����Ʋ��");
		NewSheet(pXL, "3. ����Ʋ��", 0, "����ü");
		pFloor->Torsion();
		ArrUsedSheet.Add("3. ����Ʋ��");
	}
	else if(nCurFile == 3)// 4. �ܸ����
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "4. �ܸ����");
		NewSheet(pXL, "4. �ܸ����(�ۿ�����)", 0, "����ü");
		pSection->SectionCheck();
		ArrUsedSheet.Add("4. �ܸ����(�ۿ�����)");
		NewSheet(pXL, "4. �ܸ����(��������)", 0, "����ü");
		pSection->SectionUltimate();
		ArrUsedSheet.Add("4. �ܸ����(��������)");
	}
	
	else if(nCurFile == 4)// 5. �ܸ��൵
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "5. �ܸ��൵");
		NewSheet(pXL, "5. �ܸ��൵", 0, "����ü");
		pSection->SectionSummary();
		ArrUsedSheet.Add("5. �ܸ��൵");
	}
	
	else if(nCurFile == 5)// 6. ��������
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "6. ��������");
		NewSheet(pXL, "6. ��������", 0, "����ü");
		pSection->Splice();
		ArrUsedSheet.Add("6. ��������");
	}
	
	else if(nCurFile == 6)// 7. ��������
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "7. ��������");
		NewSheet(pXL, "7. ��������", 0, "����ü");
		pSection->Weld();
		ArrUsedSheet.Add("7. ��������");
	}

	
	else if(nCurFile == 7)// 8. ���������
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "8. ���������");
		NewSheet(pXL, "8. ���������", 0, "����ü");
		pStiff->Stiff();
		ArrUsedSheet.Add("8. ���������");
	}
	
	else if(nCurFile == 8)// 9. ���κ� �� �극�̽� ����
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "9. ���κ� ����");

		NewSheet(pXL, "9. ���κ� ����(�ܺ�,�߰���)", 0, "����ü");
		pCross->CrossBeam();
		ArrUsedSheet.Add("9. ���κ� ����(�ܺ�,�߰���)");

		NewSheet(pXL, "9. ���κ� ����(�극�̽�)", 0, "����ü");
		pCross->CrossBeamHorVerBracing();
		ArrUsedSheet.Add("9. ���κ� ����(�극�̽�)");

	}
	
	else if(nCurFile == 9)// 10. ���ܿ��������
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "10. ���ܿ��������");
		NewSheet(pXL, "10. ���ܿ��������", 0, "����ü");
		pStud->CheckStud();
		ArrUsedSheet.Add("10. ���ܿ��������");
	}
	
	else if(nCurFile == 10)// 11. ��뼺����
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "11. ��뼺����");
		NewSheet(pXL, "11. ��뼺����", 0, "����ü");
		pUsing->Fatigue();
		ArrUsedSheet.Add("11. ��뼺����");

		NewSheet(pXL, "11.2 ó������", 0, "����ü");
		pUsing->OutExcelCheck11_Deflection();
		ArrUsedSheet.Add("11.2 ó������");
	}
	
	else if(nCurFile == 11)// 12. ������������
	{
		m_nSheetNo = 1;
		if(m_bAbort) return 0;
		lW = 10;	OnProgress(0, lW);	OnProgressText(1, "12. ������������");
		pXL->SetActiveSheet("12. ������������");
		pXL->SheetMoveCopy(FALSE, FALSE, "12. ������������");		
		pUsing->ExpansionJoint();
		ArrUsedSheet.Add("12. ������������");
	}

	if(ArrUsedSheet.GetSize() > 0)
		pXL->DeleteSheetNotUsed(ArrUsedSheet);

	return 0;
}


int CAPlateOutExcelCheck::OutAPlateExcelCheckAll(CXLControl *pXL)
{
	CXLCheckListFloor		*pFloor		= m_pAPlateOutCalcStd->GetXLCheckListFloor();
	CXLCheckListSection		*pSection	= m_pAPlateOutCalcStd->GetXLCheckListSection();
	CXLCheckListStiff		*pStiff		= m_pAPlateOutCalcStd->GetXLCheckListStiff();
	CXLCheckListUsing		*pUsing		= m_pAPlateOutCalcStd->GetXLCheckListUsing();
	CXLCheckListCrossBeam	*pCross		= m_pAPlateOutCalcStd->GetXLCheckListCrossBeam();
	CXLCheckListStud		*pStud		= m_pAPlateOutCalcStd->GetXLCheckListStud();
	CAPlateOutputDlg		*pDlg		= m_pAPlateOutCalcStd->GetAPlateOutputDlg();
	CAPlateOutExcel			*pOutExcel	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	
	pFloor->SetXLControl(pXL);
	pSection->SetXLControl(pXL);
	pCross->SetXLControl(pXL);
	pUsing->SetXLControl(pXL);
	pStiff->SetXLControl(pXL);
	pStud->SetXLControl(pXL);
	
	CString	sTitle = _T(""), sNum = _T("");
	CStringArray	ArrUsedSheet;
	long lW			= 0;
	long nOutXLsu	= TOTCHECKLISTSU;
	long nPos		= 0;
	long nPagesu	= pDlg->m_nPageArr.GetSize();
	m_nSheetNo = 1;

	for(long nPage = 0;	nPage < nPagesu; nPage++)
	{
		long nPageNum = pDlg->m_nPageArr.GetAt(nPage);
		if(pOutExcel->m_bAbort) return 0;
		
		if(nPageNum == 1)
		{
			sTitle.Format("1. �ٴ���");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);

			nPos = (long)(100.0 * 1.0 / (double)nPagesu);
			OnProgress(2, nPos);
			pFloor->Floor();
			
			ArrUsedSheet.Add("1. �ٴ���");
		}

		if(nPageNum == 2)
		{
			sTitle.Format("2. �ݷ°���");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);

			nPos = (long)(100.0 * 2.0 / (double)nPagesu);
			OnProgress(2, nPos);
			pFloor->Reaction();
			ArrUsedSheet.Add("2. �ݷ°���");
		}

		if(nPageNum == 3)
		{
			sTitle.Format("3. ����Ʋ��");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);
			
			nPos = (long)(100.0 * 3.0 / (double)nPagesu);
			OnProgress(2, nPos);
			pFloor->Torsion();
			ArrUsedSheet.Add("3. ����Ʋ��");
		}

		if(nPageNum == 4)
		{
			sTitle.Format("4. �ܸ����");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);
			nPos = (long)(100.0 * 4.0 / (double)nPagesu);
			OnProgress(2, nPos);

			NewSheet(pXL, "4. �ܸ����(�ۿ�����)", 0, "����ü");
			pSection->SectionCheck();
			
			NewSheet(pXL, "4. �ܸ����(��������)", 0, "����ü");
			pSection->SectionUltimate();

			ArrUsedSheet.Add("4. �ܸ����(�ۿ�����)");
			ArrUsedSheet.Add("4. �ܸ����(��������)");
		}

		if(nPageNum == 5)
		{
			sTitle.Format("5. �ܸ��൵");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);
			
			nPos = (long)(100.0 * 5.0 / (double)nOutXLsu);
			OnProgress(2, nPos);
			pSection->SectionSummary();
			ArrUsedSheet.Add("5. �ܸ��൵");
		}

		if(nPageNum == 6)
		{
			sTitle.Format("6. ��������");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);

			nPos = (long)(100.0 * 6.0 / (double)nOutXLsu);
			OnProgress(2, nPos);
			pSection->Splice();
			ArrUsedSheet.Add("6. ��������");
		}

		if(nPageNum == 7)
		{
			sTitle.Format("7. ��������");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);			
			
			nPos = (long)(100.0 * 7.0 / (double)nOutXLsu);
			OnProgress(2, nPos);
			pSection->Weld();
			ArrUsedSheet.Add("7. ��������");
		}

		if(nPageNum == 8)
		{
			sTitle.Format("8. ���������");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);
			
			nPos = (long)(100.0 * 8.0 / (double)nOutXLsu);
			OnProgress(2, nPos);
			pStiff->Stiff();
			ArrUsedSheet.Add("8. ���������");
		}

		if(nPageNum == 9)
		{
			sTitle.Format("9. ���κ� ����");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			sTitle.Format("9. ���κ� ����(%d/%d)", 9, nOutXLsu);
			OnProgressText(3, sTitle);
			nPos = (long)(100.0 * 9.0 / (double)nOutXLsu);
			OnProgress(2, nPos);

			NewSheet(pXL, "9. ���κ� ����(�ܺ�,�߰���)", 0, "����ü");
			pCross->CrossBeam();
			
			NewSheet(pXL, "9. ���κ� ����(�극�̽�)", 0, "����ü");
			pCross->CrossBeamHorVerBracing();

			ArrUsedSheet.Add("9. ���κ� ����(�ܺ�,�߰���)");
			ArrUsedSheet.Add("9. ���κ� ����(�극�̽�)");		
		}

		if(nPageNum == 10)
		{
			sTitle.Format("10. ���ܿ��������");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);
			
			nPos = (long)(100.0 * 10.0 / (double)nOutXLsu);
			OnProgress(2, nPos);
			pStud->CheckStud();
			ArrUsedSheet.Add("10. ���ܿ��������");
		}

		if(nPageNum == 11)
		{
			sTitle.Format("11. ��뼺����");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			NewSheet(pXL, sTitle, 0, "����ü");
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);

			nPos = (long)(100.0 * 11.0 / (double)nOutXLsu);
			OnProgress(2, nPos);
			pUsing->Fatigue();

			sTitle.Format("11.2 ó������");
			NewSheet(pXL, sTitle, 0, "����ü");
			pUsing->OutExcelCheck11_Deflection();

			ArrUsedSheet.Add("11. ��뼺����");
			ArrUsedSheet.Add("11.2 ó������");
		}
		
		if(nPageNum == 12)
		{
			sTitle.Format("12. ������������");
			lW = 10;	OnProgress(0, lW);	OnProgressText(1, sTitle);
			pXL->SetActiveSheet(sTitle);
			pXL->SheetMoveCopy(FALSE, FALSE, sTitle);
			sNum.Format("(%d/%d)", nPageNum, nPagesu);
			OnProgressText(3, sTitle+sNum);

			nPos = (long)(100.0 * 12.0 / (double)nOutXLsu);
			OnProgress(2, nPos);
			pUsing->ExpansionJoint();
			ArrUsedSheet.Add("12. ������������");
		}
	}

	if(ArrUsedSheet.GetSize() > 0)
		pXL->DeleteSheetNotUsed(ArrUsedSheet);
	return 0;
}

void CAPlateOutExcelCheck::OutXLSelect(long nXLIdx, CString sXLSaveFile)
{
	CAPlateOutputDlg	*pDlg	= m_pAPlateOutCalcStd->GetAPlateOutputDlg();

	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sXl = dVersion < 12? _T("xls") : _T("xlsx");

	CString csName = "", cs;

	if(m_bAllFiles)
	{
		if( sXLSaveFile.Find('.') >=0)
		{
			int pos = sXLSaveFile.Find('.'); 
			csName = sXLSaveFile.Left(pos);
		}

		if(pDlg->m_nPageArr.GetSize()>1)
		{
			cs.Format("%ld.%s", nXLIdx + 1, sXl);
			csName += cs;
		}
		else
		{
			cs.Format(".%s", sXl);
			csName += cs;
		}
	}
	else
		csName = sXLSaveFile;

	m_sXLCheckSaveFile[nXLIdx] = csName;

	if(m_bAllFiles)	m_nCurFileIdx[nXLIdx] = nXLIdx;
	else			m_nCurFileIdx[nXLIdx] = pDlg->GetCurXLCheckFile();
}

void CAPlateOutExcelCheck::NewSheet(CXLControl *pXL, CString strSheetName, long nCurFile, CString strFont, double dCellWidth, double dCellHeight, short nTextSize)
{
	if(pXL == NULL)	return;
	CAPlateOutputDlg* pOutDlg = m_pAPlateOutCalcStd->GetAPlateOutputDlg();

	CString szSheetName = pXL->GetSheetName();
	CString sSheetNumber;
	long nSheetSu = pXL->GetSheetsu();

	sSheetNumber.Format("Sheet%d", m_nSheetNo);
	if(pOutDlg->m_bOneFiles || m_nSheetNo > 3)
	{
		pXL->AddSheet();
		pXL->SetActiveSheet(sSheetNumber);
		pXL->SetSheetName(strSheetName);
	}
	else
	{
		pXL->SetActiveSheet(sSheetNumber);
		pXL->SetSheetName(strSheetName);
	}

	szSheetName = pXL->GetSheetName();
	pXL->SheetMoveCopy(FALSE, FALSE, strSheetName);

	pXL->SetCellWidth(0, 255, dCellWidth);
	pXL->SetCellHeight(0, 65535, dCellHeight);
	pXL->SetFonts(0, 0, 65535, 255, nTextSize, strFont, 1, FALSE);
	pXL->SetVerAlign(0, 0, 65535, 255, 2);
	pXL->SetNumberFormat(0, 0, 65535, 255, "###0.000");

	m_nSheetNo++;	
}
