 // APlateOutExcel.cpp: implementation of the CAPlateOutExcel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutExcel.h"
#include "APlateOutCalc.h"
#include "../APlateData\APlateData.h"
#include "APlateOutCalcXL.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)
#define WINDOW_ORGWIDTH			592
#define WINDOW_ORGHEIGHT		290
#define TOTOUTXLSU				8
#define TOTCHECKLISTSU			12
#define	BLACK				0
#define RED					3
#define	BLUE				5
#define	YELLOW				6
#define GRAY				15
#define NEXT				1

CAPlateOutExcel::CAPlateOutExcel()
{
	m_bAllFiles	= FALSE;
	m_nZoomSize	= 100;
	m_nSheetNo	= 0;
	m_nProgress	= 0;

	// �׸� ���� Path���ϱ�
	CWinApp	*pApp		= AfxGetApp();
	CString	csSection	= "Afx Directory"; 
	CString	csFixedItem	= "Fixed Directory";
	CString	csAPlate	= pApp->GetProfileString(csSection, csFixedItem);

	m_strImageXLPath	= csAPlate + "\\Analyxl12.dll";
}

CAPlateOutExcel::~CAPlateOutExcel()
{

}

int CAPlateOutExcel::OutAPlateXL(CXLControl *pXL, long nXLIdx)
{
	CDataManage			*pData		= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData           *pCalcData	= pData->GetCalcData();
	CAPlateCalcStd		*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CGlobarOption		*pOpt		= pData->GetGlobalOption();
	CXLControl			ImageXL;

	CFemStressCalc  StressCalc(pData, pCalcStd);
	if(pCalcStd->GetSizeByundanInfo(0) == 0)
	{
		StressCalc.SetSectionGugan();
		StressCalc.SetSectionSplice();
		for(long nG=0; nG<m_pAPlateOutCalcStd->GetAPlateBridgeApp()->GetGirdersu(); nG++)
			StressCalc.MakeDanmyunMomentData(nG);
	}

	m_pAPlateOutCalcStd->SetBridgeType(pOpt->GetStrDesignBridgeType());
	m_nProgress	= 0;
	long nCurFile	= m_nCurFileIdx[nXLIdx];
	m_pImageXL = &ImageXL;
	///< �׸����� ����
	m_pImageXL->OpenXL(m_strImageXLPath);
	switch(nCurFile)
	{
		case 0 :	XLOutput_1(pXL);	break;	///<  1. �������� �� �ٴ���
		case 1 :	XLOutput_2(pXL);	break;	///<  2. �𵨸� �� ��������
		case 2 :	XLOutput_3(pXL);	break;	///<  3. ����� ����
		case 3 :	XLOutput_4(pXL);	break;	///<  4. �ܸ����� ����
		case 4 :	XLOutput_5(pXL);	break;	///<  5. �������� ����
		case 5 :	XLOutput_6(pXL);	break;	///<  6. �������� ����
		case 6 :	XLOutput_7(pXL);	break;	///<  7. ���κ� �� �극�̽� ����
		case 7 :	XLOutput_8(pXL);	break;	///<  8. ���ܿ����� �� ��뼺 ����
	}
	m_pImageXL->QuitXL();
	pXL->RemoveTmpFiles();
	return FALSE;
}

UINT CAPlateOutExcel::DoWork()
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

	CAPlateOutputDlg	*pOutDlg	= m_pAPlateOutCalcStd->GetAPlateOutputDlg();

	pOutDlg->SetDlgEnable(TRUE);

	long nType	= pOutDlg->m_nType;
	long nSize	= pOutDlg->m_nPageArr.GetSize();
	long lW		= 0;
	long nPos	= 0;

	BOOL bUser = FALSE;
 	BOOL bCancel = FALSE;
	CString sXLSaveFile, sTitle;
	sXLSaveFile = m_sXLSaveFile[0];

	long nOutXLsu = 1;
	if(m_bAllFiles) nOutXLsu = TOTOUTXLSU;

	CXLControl DArr[10];

	OnProgressText(1, "��� �غ�");
	OnProgressText(3, "��� �غ�");
	
	long nProgressIdx = 0;

	for(long nXLIdx = 0; nXLIdx < nOutXLsu; nXLIdx++)
	{
		if(nType==1)	//���������
		{
			for(long i=0; i<nSize; i++)
			{
				long page = pOutDlg->m_nPageArr.GetAt(i);

				if(page==(nXLIdx+1)) bUser = TRUE;
			}
		}
		
		CXLControl *pXL = &DArr[nXLIdx];
	
#ifdef _DEBUG
		pXL->SetXLVisible(TRUE);
#endif

#ifndef _DEBUG
		try
		{
#endif
			OutXLSelect(nXLIdx, sXLSaveFile);
			
			if(nType==1 && !bUser) continue;
			long nCurFile	= m_nCurFileIdx[nXLIdx];
			if(nCurFile == 0)  sTitle.Format("1. ������� �� �ٴ���(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 1)  sTitle.Format("2. �𵨸� �� ��������(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 2)  sTitle.Format("3. ����� �� �ݷ�����(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 3)  sTitle.Format("4. �ܸ����� ����(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 4)  sTitle.Format("5. �������� ����(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 5)  sTitle.Format("6. �������� ����(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 6)  sTitle.Format("7. ���κ� �� �극�̰̽���(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 7)  sTitle.Format("8. ���ܿ����� �� ��뼺����(%d/%d)", nProgressIdx + 1, nSize);
					
			OnProgressText(3, sTitle);

			if(m_sXLReadFile[nXLIdx] == "")
			{
				if(pXL->NewXL() == FALSE)	 return 1;	//error
			}
			else
			{
				if(pXL->OpenXL(m_sXLReadFile[nXLIdx]) == FALSE)
					return 1;	// error
			}

			m_pAPlateOutCalcStd->m_dXLVersion = pXL->GetVersion();
			lW  = OutAPlateXL(pXL, nXLIdx);
			bUser = FALSE;
			
			if(m_bAbort) 
			{
				bCancel = TRUE;
				pXL->QuitXL();
				OnProgressText(1, "����غ�");
				OnProgressText(3, "����غ�");
				break;
			}			
			pXL->SaveAs(m_sXLSaveFile[nXLIdx]);
			pXL->QuitXL();

			nPos = (long)(100 * (double)(nProgressIdx+1) / (double)nSize);
			OnProgress(2, nPos);

			nProgressIdx++;
#ifndef _DEBUG
		}
		catch(CException *theException)
		{
			TCHAR   szCause[255];
		    CString strFormatted = "";
			// strFormatted = theException->GetErrorMessage(szCause,255);

			theException->GetErrorMessage(szCause,255);
			strFormatted.Format("%s",szCause);
			long nResult = AfxMessageBox(strFormatted,MB_OK | MB_ICONSTOP);			
			
			if(nResult == IDOK)
			{
				OnProgressText(1, "");
				OnProgressText(3, "");

				m_pAPlateOutCalcStd->GetAPlateOutputDlg()->m_ctrlProgress1.SetPos(0);
				m_pAPlateOutCalcStd->GetAPlateOutputDlg()->m_ctrlProgress2.SetPos(0);
				
				theException->Delete();
				return 1;
			}
		}
		catch(...)
		{
			AfxMessageBox("������꼭 �ۼ��� ������ �߻��߽��ϴ�.\n�ѱ����Ƽ�� �����Ͻʽÿ� ",MB_OK | MB_ICONSTOP);	
			return 1;
		}
#endif
	}

	pOutDlg->SetDlgEnable(FALSE);
	pOutDlg->SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);
	OnProgress(0, 0);
	OnProgress(2, 0);
	
	return FALSE;
}

void CAPlateOutExcel::OutXLSelect(long nXLIdx, CString sXLSaveFile)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutputDlg	*pOutDlg	= m_pAPlateOutCalcStd->GetAPlateOutputDlg();

	CString csName = "", cs;

	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sXl = dVersion < 12? _T("xls") : _T("xlsx");

	if(m_bAllFiles)
	{
		if( sXLSaveFile.Find('.') >=0)
		{
			int pos = sXLSaveFile.Find('.');
			csName = sXLSaveFile.Left(pos);
		}

		if(pOutDlg->m_nPageArr.GetSize()>1)
		{
			cs.Format("-%ld.%s", nXLIdx + 1, sXl);
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

	m_sXLReadFile[nXLIdx] = "";
	m_sXLSaveFile[nXLIdx] = csName;
	
	if(m_bAllFiles)	m_nCurFileIdx[nXLIdx] = nXLIdx;
	else			m_nCurFileIdx[nXLIdx] = m_pAPlateOutCalcStd->GetAPlateOutputDlg()->GetCurXLFile();

	if(m_nCurFileIdx[nXLIdx] == 0 || m_nCurFileIdx[nXLIdx]==1 || m_nCurFileIdx[nXLIdx]==2 || m_nCurFileIdx[nXLIdx]==3 || m_nCurFileIdx[nXLIdx]==6 || m_nCurFileIdx[nXLIdx]==7)
	{
		CWinApp* pApp = AfxGetApp();
		CString csSection     = "Afx Directory";
		CString csFixedItem   = "Fixed Directory";

		CString csAplate = pApp->GetProfileString(csSection, csFixedItem);

		CString csFN;
		if(pDB->m_bTendonInputComplete && m_nCurFileIdx[nXLIdx]==0)
			csFN.Format("\\Analyxl%ld-1.dll", m_nCurFileIdx[nXLIdx] + 1);
		else
			csFN.Format("\\Analyxl%ld.dll", m_nCurFileIdx[nXLIdx] + 1);

		csAplate += csFN;
		if( csFN == _T("") ) return;
		m_sXLReadFile[nXLIdx] = csAplate;
	}
}

// ǥ��
void CAPlateOutExcel::OutCalcCover(CXLControl *pXL)
{
	CDataManage			*pData		= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateOutCalcXL	*pCalcXL	= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	long	nSttRow = 10;
	CString	sText;

	pCalcXL->SetXL(pXL, "< �� �� �� �� �� >",	nSttRow, "A", "AA", NEXT, BLACK, 14, TRUE, TA_CENTER);

}

// ����	- ������ ���� ���
void CAPlateOutExcel::OutCalcContents_WithOut_Tendon(CXLControl *pXL)
{
	CAPlateOutCalcXL	*pCalcXL	= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();

	long nSttRow = 1;
	pCalcXL->SetXL(pXL, "�� ��",										nSttRow, "A", "Z", NEXT, BLACK, 11, TRUE, TA_CENTER);
	pCalcXL->SetXL(pXL, "1. �� �� �� ��",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "2. �� �� �� ��",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "3. �� �� �� ����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "3.1 ���� ĵƿ������",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.2 ���� ĵƿ������",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.3 �߾�������",								nSttRow, "C", "C", NEXT, BLACK, 9);

	long nNum = 4;
	if(!pDB->IsPlateGir())
	{
		CString szTitle = _T("");
		if(pDB->m_nSlabTensionSize)
		{
			szTitle.Format("3.%d ������Ʈ������ �ս�", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		szTitle.Format("3.%d �ٴ��� ������ ���°���", nNum++);
		pCalcXL->SetXL(pXL, szTitle,				nSttRow, "C", "C", NEXT, BLACK, 9);
		if(!pDB->IsTUGir())
		{
			szTitle.Format("3.%d ����ĵƿ������(����������)", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		if(pDB->m_nSlabTensionSize)
		{
			szTitle.Format("3.%d �������߿� ���� �ܸ����", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		if(pDB->m_nQtyJigan>1)
		{
			szTitle.Format("3.%d ���������� �ٴ��� ������ ����", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
			szTitle.Format("3.%d �ٴ��� ��� ö�ٷ�����", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		if(pDB->m_nSlabTensionSize)
		{
			szTitle.Format("3.%d ������ ���� ����", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
	}
	else if(pDB->m_nQtyJigan>1)//������...
	{		
		pCalcXL->SetXL(pXL, "3.4 ���������� �ٴ��� ������ ����",	nSttRow, "C", "C", NEXT, BLACK, 9);
	}
	
	pCalcXL->SetXL(pXL, "4. �� �� �� ��",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "4.1 �÷��� �� �ٴ����� ��ȿ�� ���",			nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.2 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.3 �ۿ������� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.4 ������ �ܸ�� ����",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) MODELING",									nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �� �� �� ��",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �� �� �� ��",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) ����� ����",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �ۿ����ߺ� ���������",					nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) ����� ���",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �� �� �� ��",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.5 ����Ʋ��(WARPING TORSION)�� ���� ����",	nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.6 2�����¿� ���� ���� ����",					nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.7 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.8 �� �� �� �� ��",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5. ������ ����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "5.1 �������� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5.2 �������� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6. ������ ����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "6.1 WEB ������",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.2 ������ ������",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.3 JackUp ������",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7. CROSS BEAM�� ����",							nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "7.1 ���κ��� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.2 ���� �극�̽��� ����",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.3 ���κ� ����",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "8. ���ܿ�����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9. ��뼺����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9.1 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "9.2 ó �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "10. ����(CAMBER)",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "11. �������� ����",							nSttRow, "B", "B", NEXT, BLACK, 10);
}

// ����	- ������ �ִ� ���
void CAPlateOutExcel::OutCalcContents_With_Tendon(CXLControl *pXL)
{
	CAPlateOutCalcXL	*pCalcXL	= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	long nSttRow = 1;
	pCalcXL->SetXL(pXL, "�� ��",										nSttRow, "A", "Z", NEXT, BLACK, 11, TRUE, TA_CENTER);

	pCalcXL->SetXL(pXL, "1. �� �� �� ��",								nSttRow, "B", "B", NEXT, BLACK, 10);

	pCalcXL->SetXL(pXL, "2. �� �� �� ��",								nSttRow, "B", "B", NEXT, BLACK, 10);

	pCalcXL->SetXL(pXL, "3. �� �� �� ����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "3.1 �ٴ��� �β� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.2 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.3 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.4 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.5 ������Ʈ������ �ս�",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.6 �ٴ��� ������ ���� ����",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.7 �������߿� ���� �ܸ����",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.8 ������ ����",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.9 ���� ������ ����� ����",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.10 �ٴ��� ��� ö�ٷ� ����",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.11 �Ŵ� �ܺ� �ٴ����� ����",								nSttRow, "C", "C", NEXT, BLACK, 9);

	
	pCalcXL->SetXL(pXL, "4. �� �� �� ��",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "4.1 �÷��� �� �ٴ����� ��ȿ�� ���",			nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.2 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.3 �ۿ������� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.4 ������ �ܸ�� ����",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) MODELING",									nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �� �� �� ��",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �� �� �� ��",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) ����� ����",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �ۿ����ߺ� ���������",					nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) ����� ���",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "��) �� �� �� ��",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.5 ����Ʋ��(WARPING TORSION)�� ���� ����",	nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.6 2�����¿� ���� ���� ����",					nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.7 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.8 �� �� �� �� ��",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5. ������ ����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "5.1 �������� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5.2 �������� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6. ������ ����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "6.1 WEB ������",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.2 ������ ������",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.3 JackUp ������",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7. CROSS BEAM�� ����",							nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "7.1 ���κ��� ����",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.2 ���� �극�̽��� ����",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.3 ���κ� ����",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "8. ���ܿ�����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9. ��뼺����",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9.1 �� �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "9.2 ó �� �� ��",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "10. ����(CAMBER)",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "11. �������� ����",							nSttRow, "B", "B", NEXT, BLACK, 10);
}

void CAPlateOutExcel::SetProgress(CString strText, long nlW)
{
	OnProgress(0, nlW);
	OnProgressText(1, strText);
	m_nProgress = nlW;
}

void CAPlateOutExcel::NewSheet(CXLControl *pXL, CString strSheetName, long nCurFile, CString strFont, double dCellWidth, double dCellHeight, short nTextSize)
{
	if(pXL == NULL)	return;

	CString szSheetName = pXL->GetSheetName();
	CString sSheetNumber;
	long nSheetSu = pXL->GetSheetsu();

	if(nCurFile == 0)
	{
//		if(nSheetSu <= m_nSheetNo)
		pXL->AddSheet();
		szSheetName = pXL->GetSheetName();
		sSheetNumber.Format("Sheet%d", m_nSheetNo);
	}
	else
	{
		if(nSheetSu < m_nSheetNo)
			pXL->AddSheet();
		szSheetName = pXL->GetSheetName();
		sSheetNumber.Format("Sheet%d", m_nSheetNo);
	}

	pXL->SetActiveSheet(sSheetNumber);
	szSheetName = pXL->GetSheetName();
	pXL->SetSheetName(strSheetName, m_nZoomSize);
	szSheetName = pXL->GetSheetName();
	pXL->SheetMoveCopy(FALSE,FALSE,strSheetName);

	pXL->SetCellWidth(0, 255, dCellWidth);
	pXL->SetCellHeight(0, 65535, dCellHeight);
	pXL->SetFonts(0, 0, 65535, 255, nTextSize, strFont, 1, FALSE);
	pXL->SetVerAlign(0, 0, 65535, 255, 2);
	pXL->SetNumberFormat(0, 0, 65535, 255, "###0.000");

	m_nSheetNo++;
}

BOOL CAPlateOutExcel::XLOutput_1(CXLControl *pXL)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CCalcData           *pCalcData			= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CGlobarOption		*pOpt				= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();

	CXLDesignCondition	*pXLDesignCondition	= m_pAPlateOutCalcStd->GetDesignCondiotion();
	CXLDesignFloor		*pXLDesignFloor		= m_pAPlateOutCalcStd->GetDesignFloor();
	pXLDesignCondition->SetXLControl(pXL);
	pXLDesignFloor->SetXLControl(pXL);
	m_nSheetNo = 1;

	CStringArray	strArrUseSheet;
	CString szText	= _T("");
	long nJigansu = pDB->m_nQtyJigan;

	m_nProgress = 0;		OnProgress(0, m_nProgress);	OnProgressText(1, "");

	if(!pCalcData->m_bMakeModeling && m_bAllFiles)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}

	// ������ ���翩��
	BOOL bTendon = pDB->m_bTendonInputComplete;

	//ǥ��
	if(m_bAbort) return FALSE;	
	m_nProgress = 5;		OnProgress(0, m_nProgress);	OnProgressText(1, "ǥ ��");	
	NewSheet(pXL, "ǥ ��", 0);	
	OutCalcCover(pXL);	
	pXL->SheetMoveCopy(FALSE, FALSE, "ǥ ��");
	strArrUseSheet.Add("ǥ ��");
		
	// ����
	if(m_bAbort) return FALSE;	
	m_nProgress = 10;		OnProgress(0, m_nProgress);	OnProgressText(1, "�� ��");	
	NewSheet(pXL, "�� ��", 0);
	if (bTendon) OutCalcContents_With_Tendon(pXL);
	else		 OutCalcContents_WithOut_Tendon(pXL);	
	pXL->SheetMoveCopy(FALSE, FALSE, "�� ��");
	strArrUseSheet.Add("�� ��");

// review sim
	// 1. ��������
	if(m_bAbort) return FALSE;	
	m_nProgress = 15;		OnProgress(0, m_nProgress);	OnProgressText(1, "1. ��������");	
	pXL->SetActiveSheet("1. ��������");
	pXLDesignCondition->DesignCommon();			///< 1.1 �Ϲݻ���
	if(m_bAbort) return FALSE;	
	pXLDesignCondition->LoadCondition();		///< 1.2 ��������	
	if(m_bAbort) return FALSE;
	strArrUseSheet.Add("1. ��������");

	// 2. �ܸ鰡��
	if(m_bAbort) return FALSE;	
	m_nProgress = 20;		OnProgress(0, m_nProgress);	OnProgressText(1, "2. �� �� �� ��");	
	pXL->SetActiveSheet("2. �ܸ鰡��");	
	pXLDesignCondition->DesignSection();
	if(m_bAbort) return FALSE;	
	strArrUseSheet.Add("2. �ܸ鰡��");

	//////////////////////////////////////////////////////////////////////////
	// ������ �����ϴ� ���
	if (bTendon)
	{
		// 3. �ٴ��� ����
		if(m_bAbort) return FALSE;
		szText.Format("3. �ٴ��� ����");
		m_nProgress = 30;		OnProgress(0, m_nProgress);	OnProgressText(1, "3. �ٴ��� ����");
		pXL->SetActiveSheet("3. �ٴ��� ����");
		pXLDesignFloor->TendonDesignFloor();
		strArrUseSheet.Add(szText);

		// 3.3 ���߻���
		if(m_bAbort) return FALSE;
		szText.Format("3.3 ���߻���");
		m_nProgress = 35;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.3 ���߻���");
		pXL->SetActiveSheet("3.3 ���߻���");
		pXLDesignFloor->TendonLoadSum();
		strArrUseSheet.Add(szText);
	
		// 3.4 �ܸ�� ����(1)
		if(m_bAbort) return FALSE;
		szText.Format("3.4 �ܸ�� ����(1)");
		m_nProgress = 40;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.4 �ܸ�� ����(1)");
		pXL->SetActiveSheet("3.4 �ܸ�� ����(1)");
		pXLDesignFloor->TendonSectionSum();
		strArrUseSheet.Add(szText);
		
		// 3.5 ������Ʈ������ �ս�(1)
		if(m_bAbort) return FALSE;
		szText.Format("3.5 ������Ʈ������ �ս�(1)");
		m_nProgress = 45;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.5 ������Ʈ������ �ս�(1)");
		pXL->SetActiveSheet("3.5 ������Ʈ������ �ս�(1)");
		pXLDesignFloor->TendonPreStressLose();
		strArrUseSheet.Add(szText);
		
		// 3.6 �ٴ��� ������ ���� ����
		if(m_bAbort) return FALSE;
		szText.Format("3.6 �ٴ��� ������ ���� ����");
		m_nProgress = 50;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.6 �ٴ��� ������ ���� ����");
		pXL->SetActiveSheet("3.6 �ٴ��� ������ ���� ����");
		pXLDesignFloor->TendonFloorStress();
		strArrUseSheet.Add(szText);
		
		// 3.7 �������߿� ���� �ܸ����
		if(m_bAbort) return FALSE;
		szText.Format("3.7 �������߿� ���� �ܸ����");
		m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.7 �������߿� ���� �ܸ����");
		pXL->SetActiveSheet("3.7 �������߿� ���� �ܸ����");
		pXLDesignFloor->TendonLimitStressCheck();
		strArrUseSheet.Add(szText);
		
		// 3.8 ������ ����
		if(m_bAbort) return FALSE;
		szText.Format("3.8 ������ ����");
		m_nProgress = 70;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.8 ������ ����");
		pXL->SetActiveSheet("3.8 ������ ����");
		pXLDesignFloor->TendonSettlement();
		strArrUseSheet.Add(szText);
		
		// 3.9 ���� ������ ����� ����
		if(m_bAbort) return FALSE;
		szText.Format("3.9 ���� ������ ����� ����");
		m_nProgress = 80;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.9 ���� ������ ����� ����");
		pXL->SetActiveSheet("3.9 ���� ������ ����� ����");
		pXLDesignFloor->TendonInnerTension();
		strArrUseSheet.Add(szText);
		
		// 3.11 �Ŵ� �ܺ� �ٴ����� ����
		if(m_bAbort) return FALSE;
		szText.Format("3.11 �Ŵ� �ܺ� �ٴ����� ����");
		m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.11 �Ŵ� �ܺ� �ٴ����� ����");
		pXL->SetActiveSheet("3.11 �Ŵ� �ܺ� �ٴ����� ����");
		pXLDesignFloor->TendonGirderReinforce();
		strArrUseSheet.Add(szText);

	}
	//////////////////////////////////////////////////////////////////////////
	// ������ �������� �ʴ� ���
	else
	{
 		if(m_bAbort) return FALSE;
 		m_nProgress = 30;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.1 ���� ĵƿ������");
 		pXLDesignFloor->DesignFloor(TRUE);
 		strArrUseSheet.Add("3.1 ���� ĵƿ������");

		if(m_bAbort) return FALSE;
		m_nProgress = 45;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.2 ���� ĵƿ������");
		pXLDesignFloor->DesignFloor(FALSE);
		strArrUseSheet.Add("3.2 ���� ĵƿ������");
			
		if(pOpt->GetSlabCentDesignMethod() == 0 || pOpt->GetSlabCentDesignMethod() == 2)		//0 : �߾Ӻ�  2 : LB-Deck 
		{
			if(m_bAbort) return FALSE;
			m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.3 �߾�������");
			NewSheet(pXL, "3.3 �߾�������", 0);
			pXLDesignFloor->DesignFloor_Center();
			strArrUseSheet.Add("3.3 �߾�������");
		}
			
		CCalcGeneral	CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
		long nNum = 3;
		// �������̸�..
		if(pOpt->GetSlabCentDesignMethod()==1)
		{
			
			if(!CalcGeneral.CheckExperienceDesign(TRUE) || !CalcGeneral.CheckExperienceDesign(FALSE))
			{
				if(m_bAbort) return FALSE;
				m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.3 �߾�������");
				NewSheet(pXL, "3.3 �߾�������", 0);
				pXLDesignFloor->DesignFloor_Center();
				strArrUseSheet.Add("3.3 �߾�������");

				nNum++;
			}
			
			szText.Format("3.%d ������ �������", nNum);
			if(m_bAbort) return FALSE;
			m_nProgress = 97;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);

			//pXL->SetActiveSheet("3.4 �������������");
			//pXL->SheetMoveCopy(FALSE, FALSE, szText);
			//pXLDesignFloor->DesignExperience();
			
			NewSheet(pXL, szText, 0);
			pXLDesignFloor->CheckFloorRebarCalc(eFloor_CenterExperience, CStringArray(), FALSE);

			strArrUseSheet.Add(szText);
			nNum++;
		}
		if(!pDB->IsPlateGir())
		{
			long nRow=0;
			if(pDB->m_nSlabTensionSize)
			{	
				if(m_bAbort) return FALSE;
				szText.Format("3.%d ������Ʈ������ �ս�", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXL->SetActiveSheet("3.4 ������Ʈ������ �ս�");
				pXL->SheetMoveCopy(FALSE, FALSE, szText);
				nRow = pXLDesignFloor->LossOfPreStress();
				strArrUseSheet.Add(szText);
				nNum++;
				
				if(m_bAbort) return FALSE;
				szText.Format("3.%d �ٴ��� ������ ���°���", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.5 �ٴ��� ������ ���°���");
				pXLDesignFloor->SlabStressCheck(nRow);
				nNum++;
			}
			
			if(!pDB->IsTUGir())
			{
				if(m_bAbort) return FALSE;
				szText.Format("3.%d ����ĵƿ������(����������)", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXL->SetActiveSheet("3.6 ����ĵƿ������(����������)");
				pXL->SheetMoveCopy(FALSE, FALSE, szText);
				pXLDesignFloor->ExpansionJointUltimateLoad();
				strArrUseSheet.Add(szText);
				nNum++;
			}		
			
			if(pDB->m_nSlabTensionSize)
			{		
				if(m_bAbort) return FALSE;
				szText.Format("3.%d �������߿� ���� �ܸ����", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXL->SetActiveSheet("3.7 �������߿� ���� �ܸ����");
				pXL->SheetMoveCopy(FALSE, FALSE, szText);
				pXLDesignFloor->SectionCheckAboutUltimateLoad();
				strArrUseSheet.Add(szText);
				nNum++;
			}
			
			if(nJigansu>1)
			{
				if(m_bAbort) return FALSE;
				szText.Format("3.%d ���������� �ٴ��� ������ ����", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				szText.Format("3.%d ������ �ٴ��ǰ���", nNum);
				NewSheet(pXL, szText, 0);

				pXLDesignFloor->CheckInnerJijum();

				strArrUseSheet.Add(szText);
				nNum++;
				
				if(m_bAbort) return FALSE;
				szText.Format("3.%d �ٴ��� ��� ö�ٷ�����", nNum);
				NewSheet(pXL, szText, 0);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXLDesignFloor->CheckFloorReinforce(szText);
				strArrUseSheet.Add(szText);
				nNum++;
				
				if(pDB->m_nSlabTensionSize)
				{			
					if(m_bAbort) return FALSE;
					szText.Format("3.%d ������ ���� ����", nNum);
					m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
					pXL->SetActiveSheet("3.10 ������ ���� ����");
					pXL->SheetMoveCopy(FALSE, FALSE, szText);
					pXLDesignFloor->DesignFixingReinforce();
					strArrUseSheet.Add(szText);
					nNum++;
				}
			}
			else
			{
				szText.Format("3.%d �ٴ��� ��� ö�ٷ�����", nNum);
				if(m_bAbort) return FALSE;
				NewSheet(pXL, szText, 0);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXLDesignFloor->CheckFloorReinforce(szText);
				strArrUseSheet.Add(szText);
				nNum++;
				
				if(pDB->m_nSlabTensionSize)
				{			
					if(m_bAbort) return FALSE;
					szText.Format("3.%d ������ ���� ����", nNum);
					m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
					pXL->SetActiveSheet("3.10 ������ ���� ����");
					pXL->SheetMoveCopy(FALSE, FALSE, szText);
					pXLDesignFloor->DesignFixingReinforce();
					strArrUseSheet.Add(szText);
					nNum++;
				}
			}	
		}
		else//�������̸�...
		{
			if(nJigansu > 1)
			{
				szText.Format("3.%d ������ �ٴ��ǰ���", nNum);
				if(m_bAbort) return FALSE;
				NewSheet(pXL, szText, 0);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXL->SetActiveSheet(szText);
				
				//pXLDesignFloor->CheckInnerJijum();
				pXLDesignFloor->CheckFloorRebarCalc(eFloor_MiddleJijum, CStringArray(), FALSE);
				
				strArrUseSheet.Add(szText);
				nNum++;
			}
		}
	}

	pXL->SetActiveSheet("ǥ ��");
	pXL->SheetMoveCopy("0. ��������", FALSE);
	pXL->SetActiveSheet("�� ��");
	pXL->SheetMoveCopy("0. ��������", FALSE);
	pXL->DeleteSheetNotUsed(strArrUseSheet);
	return FALSE;
}

BOOL CAPlateOutExcel::XLOutput_2(CXLControl *pXL)
{
	CCalcData       *pCalcData 		 = m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CXLDesignOfMold	*pXLDesignOfMold = m_pAPlateOutCalcStd->GetDesignOfMold();

	pXLDesignOfMold->SetXLControl(pXL);
	m_nSheetNo =1;
	if(!pCalcData->m_bMakeModeling)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}

	CString			strSheet;
	CStringArray	strArrUseSheet;

	if(m_bAbort) return FALSE;
	strSheet.Format("4.1 ��ȿ�� ���");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 5;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->CalcEffectWidth();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.2 �ܸ鰡��-�Ŵ�");
	if(m_pAPlateOutCalcStd->GetAPlateBridgeApp()->IsTUGir())
		pXL->SetActiveSheet("4.2 �ܸ鰡��-�Ŵ�(TU)");
	else
		pXL->SetActiveSheet("4.2 �ܸ鰡��-�Ŵ�(�Ϲ�)");	
	m_nProgress = 15;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->SectionAssumptionGirder();
	pXL->SetSheetName(strSheet);
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.2 �ܸ鰡��-���κ�");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 20;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->SectionAssumptionCR();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.3 �ۿ������� ����(1)");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 30;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 �ۿ������� ���� - Ⱦ���� ���ߺй�");
	pXLDesignOfMold->ComputeHLoad();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.3 �ۿ������� ����(2)");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 45;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 �ۿ������� ���� - ǳ����");
	pXLDesignOfMold->ComputeHLoad_Wind();

	if(m_bAbort) return FALSE;
	m_nProgress = 50;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 �ۿ������� ���� - ��ݰ��");
	pXLDesignOfMold->ComputeHLoad_Collision();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.3 �ۿ������� ����(3)");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 �ۿ������� ���� - ������ �ۿ�����");
	pXLDesignOfMold->ComputeHLoad_Mold();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.4 ��) �𵨸�");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 70;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->Modeling();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.4 ��) �ܸ�����");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 75;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->SectionJewon();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.4 ��) ��������");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 95;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->Load();
	strArrUseSheet.Add(strSheet);

	pXL->DeleteSheetNotUsed(strArrUseSheet);
	return FALSE;
}

BOOL CAPlateOutExcel::XLOutput_3(CXLControl *pXL)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CCalcData           *pCalcData			= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CGlobarOption		*pOpt				= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CXLDesignOfMold		*pXLDesignOfMold	= m_pAPlateOutCalcStd->GetDesignOfMold();
	pXLDesignOfMold->SetXLControl(pXL);

	if(!pCalcData->m_bReadFrameModelingOutput)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}

	m_nSheetNo =1;

	BOOL bHapSung	= pDB->IsHapsung();
	BOOL bCheck1	= pOpt->m_bCalcDocSub[0];
	BOOL bCheck2	= pOpt->m_bCalcDocSub[1];
	CStringArray	strArrUseSheet;

	if(bCheck1)
	{
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.4 ��) ���������");
 		m_nProgress = 1;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 ��) ����� ����");
 		pXLDesignOfMold->EleForceTotal();
 		strArrUseSheet.Add("4.4 ��) ���������");
 		
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.4 ��) �ۿ����ߺ� ���������");	
 		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 ��) �ۿ����ߺ� �����");
 		pXLDesignOfMold->EleForceLoadType();
 		strArrUseSheet.Add("4.4 ��) �ۿ����ߺ� ���������");
		
		if(m_bAbort) return FALSE;
		pXL->SetActiveSheet("4.4 ��) ����� ���");	
		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 ��) ����� ���");
		pXLDesignOfMold->EleForceSummary();
		strArrUseSheet.Add("4.4 ��) ����� ���");
	}

 	if(bCheck2)
 	{
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.4 ��) �� �� �� ��");	
 		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 ��) �� �� �� ��");
 		pXLDesignOfMold->EleForceReaction();
 		strArrUseSheet.Add("4.4 ��) �� �� �� ��");
 		
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.5 ����Ʋ��(WARPING TORSION)");	
 		OnProgress(0, m_nProgress);	OnProgressText(1, "4.5 ����Ʋ��(WARPING TORSION)�� ���� ����");
 		pXLDesignOfMold->WarpingTorsion();
 		strArrUseSheet.Add("4.5 ����Ʋ��(WARPING TORSION)");
 		
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.6 2������");
 		if(bHapSung && pDB->m_nQtyJigan > 1)
 		{
 			OnProgress(0, m_nProgress);	OnProgressText(1, "4.6 2�����¿� ���� ���� ����");
 			pXLDesignOfMold->InfluenceK();
 			strArrUseSheet.Add("4.6 2������");
 		}
 		else
 			pXL->SheetDelete();
 	}
	pXL->DeleteSheetNotUsed(strArrUseSheet);

	return FALSE;
}

BOOL CAPlateOutExcel::XLOutput_4(CXLControl *pXL)
{
	CDataManage			*pData				= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData			= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CXLSectionCheck		*pXLSectionCheck	= m_pAPlateOutCalcStd->GetSectionCheck();
	CAPlateCalcStd		*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	if(!pCalcData->m_bReadFrameModelingOutput)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}

	CFemStressCalc  StressCalc(pData, pCalcStd);
	if(pCalcStd->GetSizeByundanInfo(0) == 0)
	{
		StressCalc.SetSectionGugan();
		StressCalc.SetSectionSplice();
		for(long nG=0; nG<m_pAPlateOutCalcStd->GetAPlateBridgeApp()->GetGirdersu(); nG++)
			StressCalc.MakeDanmyunMomentData(nG);
	}
	CStringArray strArrUseSheet;

	pXLSectionCheck->SetXLControl(pXL);
	m_nSheetNo = 1;

	if(m_bAbort) return FALSE;
	m_nProgress = 10;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 �ܸ����(�����Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_SEC, strArrUseSheet);

 	if(m_bAbort) return FALSE;
 	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 �ܸ����(�θ��Ʈ)");
 	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_SEC, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 �ܸ����(�������� �����Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_SP, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 �ܸ����(�������� �θ��Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_SP, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 �߰��ܸ����(�����Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_SECPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 �߰��ܸ����(�θ��Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_SECPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 ���ܸ��߰��ܸ����(�����Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_BUYNPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 ���ܸ��߰��ܸ����(�θ��Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_BUYNPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 ������߰��ܸ����(�����Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_USERPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 ������߰��ܸ����(�θ��Ʈ)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_USERPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.8 �ܸ��൵");	
	pXL->SetActiveSheet("�ܸ��൵");
	pXL->SheetMoveCopy(FALSE, TRUE, "4.8 �ܸ��൵");
	pXL->SetActiveSheet("4.8 �ܸ��൵");

	pXLSectionCheck->SectionOutLine();
	strArrUseSheet.Add("4.8 �ܸ��൵");

	pXL->DeleteSheetNotUsed(strArrUseSheet);

	return FALSE;
}

BOOL CAPlateOutExcel::XLOutput_5(CXLControl *pXL)
{
	CCalcData           *pCalcData				= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CGlobarOption		*pOpt					= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CXLSpliceCheck		*pXLSpliceCheck			= m_pAPlateOutCalcStd->GetSpliceCheck();
	CXLWeldingCheck		*pXLWeldingCheck		= m_pAPlateOutCalcStd->GetWeldingCheck();
	pXLSpliceCheck->SetXLControl(pXL);
	pXLWeldingCheck->SetXLControl(pXL);
	CStringArray	strArrUseSheet;
	m_nSheetNo = 1;
	if(!pCalcData->m_bReadFrameModelingOutput)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}
	BOOL bCheck1	= pOpt->m_bCalcDocSub[2];
	BOOL bCheck2	= pOpt->m_bCalcDocSub[3];

	if (bCheck1)
	{
		if(m_bAbort) return FALSE;
		m_nProgress = 0;		OnProgress(0, m_nProgress);	OnProgressText(1, "5.1 �������� ����");
		NewSheet(pXL, "5.1 �������� ����", 4);
		pXLSpliceCheck->SpliceCheck();
		strArrUseSheet.Add("5.1 �������� ����");
	}

	if (bCheck2)
	{
		NewSheet(pXL, "5.2 ������ ����", 4);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "5.2 ������ ����");
		pXLWeldingCheck->WeldingCheck();
		strArrUseSheet.Add("5.2 ������ ����");
	}


	pXL->DeleteSheetNotUsed(strArrUseSheet);

	return FALSE;
}

BOOL CAPlateOutExcel::XLOutput_6(CXLControl *pXL)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CCalcData           *pCalcData		= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CXLDesignStiff		*pXLDesignStiff	= m_pAPlateOutCalcStd->GetDesignStiff();

	pXLDesignStiff->SetXLControl(pXL);
	m_nSheetNo = 1;
	CStringArray	strArrUseSheet;
	if(!pCalcData->m_bReadFrameModelingOutput)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}

	if(m_bAbort) return FALSE;	
	pXLDesignStiff->WebStiff(strArrUseSheet);	///< 6. �������� ����

	if(pDB->m_nQtyJigan > 1)
	{
		NewSheet(pXL, "6.2 ������ ������(�����)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.2 ������ ������ - �����");
		pXLDesignStiff->JijumSupport(strArrUseSheet, FALSE);

		NewSheet(pXL, "6.2 ������ ������(������)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.2 ������ ������ - ������");
		pXLDesignStiff->JijumSupport(strArrUseSheet, TRUE);

		NewSheet(pXL, "6.3 JackUp ������(�����)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.3 JackUp ������ - �����");
		pXLDesignStiff->JackUpSupport(strArrUseSheet, FALSE);

		NewSheet(pXL, "6.3 JackUp ������(������)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.3 JackUp ������ - ������");
		pXLDesignStiff->JackUpSupport(strArrUseSheet, TRUE);
	}
	else
	{
		NewSheet(pXL, "6.2 ������ ������(�����)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.2 ������ ������ - �����");
		pXLDesignStiff->JijumSupport(strArrUseSheet, FALSE);	

		NewSheet(pXL, "6.3 JackUp ������(�����)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.3 JackUp ������ - �����");
		pXLDesignStiff->JackUpSupport(strArrUseSheet, FALSE);
	}
	pXL->DeleteSheetNotUsed(strArrUseSheet);
	return FALSE;
}

BOOL CAPlateOutExcel::XLOutput_7(CXLControl *pXL)
{
	CCalcData           *pCalcData			= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CXLDesignCrossBeam	*pXLDesignCrossBeam	= m_pAPlateOutCalcStd->GetDesignCrossBeam();

	pXLDesignCrossBeam->SetXLControl(pXL);
	m_nProgress = 0;		OnProgress(0, 0);	OnProgressText(1, "7.1 ���κ�");

	if(!pCalcData->m_bReadFrameModelingOutput)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}

	CStringArray strArrUseSheet;
	m_nSheetNo = 1;

	CString szText = _T("");
	long nIdx = 1;
	if(m_bAbort) return FALSE;
	m_nProgress = 10;		OnProgress(0, m_nProgress);	OnProgressText(1, "7.1 ���κ�");
	
	pXLDesignCrossBeam->DesignCrossBeam(strArrUseSheet, nIdx);
	pXLDesignCrossBeam->DesignVBracingOut(strArrUseSheet, nIdx);

	if(m_bAbort) return FALSE;
	szText.Format("7.%d ����극�̽��� ����", nIdx);
	OnProgress(0, m_nProgress+10);	OnProgressText(1, szText);
	pXLDesignCrossBeam->DesignHorBracing(strArrUseSheet, nIdx);
	
	szText.Format("7.%d ���κ��� ����", nIdx);
	NewSheet(pXL, szText, 6, "����");
	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress+10);	OnProgressText(1, szText);
	pXLDesignCrossBeam->DesignConnection(nIdx);

	OnProgress(0, 100);	OnProgressText(1, szText);
	strArrUseSheet.Add(szText);
	pXL->DeleteSheetNotUsed(strArrUseSheet);
	
	return FALSE;
}

BOOL CAPlateOutExcel::XLOutput_8(CXLControl *pXL)
{
	CPlateBridgeApp		*pDB				= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CCalcData           *pCalcData			= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CXLDesignStud		*pXLDesignStud		= m_pAPlateOutCalcStd->GetDesignStud();
	CXLUsingCheck		*pXLUsingCheck		= m_pAPlateOutCalcStd->GetUsingCheck();
	CXLEtcCheck			*pXLEtcCheck		= m_pAPlateOutCalcStd->GetEtcCheck();

	pXLDesignStud->SetXLControl(pXL);
	pXLUsingCheck->SetXLControl(pXL);
	pXLEtcCheck->SetXLControl(pXL);

	OnProgress(0, m_nProgress);	OnProgressText(1, "8. ���ܿ�����, ��뼺���� �� �������� ���");

	if(!pCalcData->m_bReadFrameModelingOutput)
	{
		XLError(ERROR_MODELING);
		return FALSE;
	}

	CStringArray	strArrUseSheet;
	CString			strSheet;	

	m_nSheetNo = 1;

	if(m_bAbort) return FALSE;
	
	if(pDB->GetGirder(0)->m_pSangse->m_StudHori_Type == 2)
	{
		strSheet.Format("8.1 �������ܿ�����");
		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
		pXL->SetActiveSheet(strSheet);
		pXLDesignStud->SquareSHConnector();
		strArrUseSheet.Add(strSheet);

		strSheet.Format("8.2 ��������ö��");
		pXL->SetActiveSheet(strSheet);
		pXLDesignStud->HorSHSteel();
		strArrUseSheet.Add(strSheet);	
	}
	else
	{
		strSheet.Format("8. ���ܿ�����");
		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
		pXL->SetActiveSheet(strSheet);
		pXLDesignStud->ShearConnector();
		strArrUseSheet.Add(strSheet);
	}	
	
	if(m_bAbort) return FALSE;
	strSheet.Format("9.1 �Ƿΰ���");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 0;		OnProgress(0, m_nProgress);	OnProgressText(1, "9.1 �Ƿΰ���");
	pXLUsingCheck->CheckFatigueStress();
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("9.2 ó������");
	pXL->SetActiveSheet(strSheet);
	OnProgress(0, m_nProgress);	OnProgressText(1, "9.2 ó������");
	pXLUsingCheck->CheckDroop();
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("10. ����");
	pXL->SetActiveSheet(strSheet);
	OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLEtcCheck->CheckCamber();
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("11. ������������(���н�)");
	pXL->SetActiveSheet(strSheet);
	OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLEtcCheck->CheckExpasionJoint(ACCURACY_METHOD);
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("11. ������������(�����)");
	pXL->SetActiveSheet(strSheet);
	OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLEtcCheck->CheckExpasionJoint(SIMPLE_METHOD);
	strArrUseSheet.Add(strSheet);

	pXL->DeleteSheetNotUsed(strArrUseSheet);

	return FALSE;
}

BOOL CAPlateOutExcel::XLError(UINT nError)
{
	switch(nError)
	{
		case ERROR_MODELING :
			MessageBox(m_hWndOwner, "�����ؼ� Data�� �����ϴ�.\n�𵨸� ���� �Ǵ� ��������� �о��ֽʽÿ�.", "WARNING", MB_OK | MB_ICONINFORMATION);
			break;
		case ERROR_FILEOPEN :
			MessageBox(m_hWndOwner, "������ ���µ� �����Ͽ����ϴ�.", "WARNING", MB_OK | MB_ICONINFORMATION);
			break;
	}

	return FALSE;
}

CString CAPlateOutExcel::ReturnChar(int Num)
{
	int AL = 64;//65=A
	int quotient;//��
	int mod;//������
	char ch;
	CString reCh;

	if(Num <= 26)
	{
		ch = Num+AL;
		reCh.Format("%c", ch);
		return reCh;
	}
	else
	{
		quotient = Num/26;
		mod = Num % 26;	
		if (mod == 0)
			reCh.Format("%sZ",ReturnChar(quotient-1));
		else
			reCh.Format("%s%s",ReturnChar(quotient), ReturnChar(mod));

		return reCh;
	}


//	int nNmg = 10 / 3; ó�� �ϸ�.. ����.. 3 
//	int nNmg = 10 % 3; ó�� �ϸ�.. �������� 1

}