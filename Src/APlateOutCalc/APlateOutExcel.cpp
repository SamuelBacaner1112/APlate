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

	// 그림 파일 Path구하기
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
	///< 그림파일 열기
	m_pImageXL->OpenXL(m_strImageXLPath);
	switch(nCurFile)
	{
		case 0 :	XLOutput_1(pXL);	break;	///<  1. 설계조건 및 바닥판
		case 1 :	XLOutput_2(pXL);	break;	///<  2. 모델링 및 하중재하
		case 2 :	XLOutput_3(pXL);	break;	///<  3. 부재력 집계
		case 3 :	XLOutput_4(pXL);	break;	///<  4. 단면응력 검토
		case 4 :	XLOutput_5(pXL);	break;	///<  5. 이음부의 설계
		case 5 :	XLOutput_6(pXL);	break;	///<  6. 보강재의 설계
		case 6 :	XLOutput_7(pXL);	break;	///<  7. 가로보 및 브레이싱 검토
		case 7 :	XLOutput_8(pXL);	break;	///<  8. 전단연결재 및 사용성 검토
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

	OnProgressText(1, "출력 준비");
	OnProgressText(3, "출력 준비");
	
	long nProgressIdx = 0;

	for(long nXLIdx = 0; nXLIdx < nOutXLsu; nXLIdx++)
	{
		if(nType==1)	//사용자정의
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
			if(nCurFile == 0)  sTitle.Format("1. 설계기준 및 바닥판(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 1)  sTitle.Format("2. 모델링 및 하중재하(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 2)  sTitle.Format("3. 부재력 및 반력집계(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 3)  sTitle.Format("4. 단면응력 검토(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 4)  sTitle.Format("5. 이음부의 설계(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 5)  sTitle.Format("6. 보강재의 설계(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 6)  sTitle.Format("7. 가로보 및 브레이싱검토(%d/%d)", nProgressIdx + 1, nSize);
			if(nCurFile == 7)  sTitle.Format("8. 전단연결재 및 사용성검토(%d/%d)", nProgressIdx + 1, nSize);
					
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
				OnProgressText(1, "출력준비");
				OnProgressText(3, "출력준비");
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
			AfxMessageBox("구조계산서 작성시 오류가 발생했습니다.\n한길아이티에 문의하십시오 ",MB_OK | MB_ICONSTOP);	
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

// 표지
void CAPlateOutExcel::OutCalcCover(CXLControl *pXL)
{
	CDataManage			*pData		= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateOutCalcXL	*pCalcXL	= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	long	nSttRow = 10;
	CString	sText;

	pCalcXL->SetXL(pXL, "< 구 조 계 산 서 >",	nSttRow, "A", "AA", NEXT, BLACK, 14, TRUE, TA_CENTER);

}

// 목차	- 강선이 없는 경우
void CAPlateOutExcel::OutCalcContents_WithOut_Tendon(CXLControl *pXL)
{
	CAPlateOutCalcXL	*pCalcXL	= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();

	long nSttRow = 1;
	pCalcXL->SetXL(pXL, "목 차",										nSttRow, "A", "Z", NEXT, BLACK, 11, TRUE, TA_CENTER);
	pCalcXL->SetXL(pXL, "1. 설 계 조 건",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "2. 단 면 가 정",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "3. 바 닥 판 설계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "3.1 좌측 캔틸레버부",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.2 우측 캔틸레버부",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.3 중앙지간부",								nSttRow, "C", "C", NEXT, BLACK, 9);

	long nNum = 4;
	if(!pDB->IsPlateGir())
	{
		CString szTitle = _T("");
		if(pDB->m_nSlabTensionSize)
		{
			szTitle.Format("3.%d 프리스트레스의 손실", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		szTitle.Format("3.%d 바닥판 슬래브 응력검토", nNum++);
		pCalcXL->SetXL(pXL, szTitle,				nSttRow, "C", "C", NEXT, BLACK, 9);
		if(!pDB->IsTUGir())
		{
			szTitle.Format("3.%d 지점캔틸레버부(신축이음부)", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		if(pDB->m_nSlabTensionSize)
		{
			szTitle.Format("3.%d 극한하중에 대한 단면검토", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		if(pDB->m_nQtyJigan>1)
		{
			szTitle.Format("3.%d 내부지점부 바닥판 주장율 검토", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
			szTitle.Format("3.%d 바닥판 배력 철근량산정", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
		if(pDB->m_nSlabTensionSize)
		{
			szTitle.Format("3.%d 정착부 보강 설계", nNum++);
			pCalcXL->SetXL(pXL, szTitle,			nSttRow, "C", "C", NEXT, BLACK, 9);
		}
	}
	else if(pDB->m_nQtyJigan>1)//판형교...
	{		
		pCalcXL->SetXL(pXL, "3.4 내부지점부 바닥판 주장율 검토",	nSttRow, "C", "C", NEXT, BLACK, 9);
	}
	
	pCalcXL->SetXL(pXL, "4. 주 형 설 계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "4.1 플랜지 및 바닥판의 유효폭 계산",			nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.2 단 면 가 정",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.3 작용하중의 산정",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.4 주형의 단면력 산정",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "가) MODELING",									nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "나) 단 면 제 원",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "다) 하 중 재 하",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "라) 부재력 집계",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "마) 작용하중별 부재력집계",					nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "바) 부재력 요약",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "사) 반 력 집 계",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.5 뒴비틀림(WARPING TORSION)에 대한 검토",	nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.6 2차응력에 대한 영향 검토",					nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.7 단 면 검 토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.8 단 면 요 약 도",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5. 이음부 설계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "5.1 현장이음 설계",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5.2 용접부의 검토",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6. 보강재 설계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "6.1 WEB 보강재",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.2 지점부 보강재",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.3 JackUp 보강재",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7. CROSS BEAM의 설계",							nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "7.1 가로보의 설계",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.2 수평 브레이싱의 설계",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.3 가로보 연결",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "8. 전단연결재",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9. 사용성검토",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9.1 피 로 검 토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "9.2 처 짐 검 토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "10. 솟음(CAMBER)",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "11. 신축이음 검토",							nSttRow, "B", "B", NEXT, BLACK, 10);
}

// 목차	- 강선이 있는 경우
void CAPlateOutExcel::OutCalcContents_With_Tendon(CXLControl *pXL)
{
	CAPlateOutCalcXL	*pCalcXL	= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	long nSttRow = 1;
	pCalcXL->SetXL(pXL, "목 차",										nSttRow, "A", "Z", NEXT, BLACK, 11, TRUE, TA_CENTER);

	pCalcXL->SetXL(pXL, "1. 설 계 조 건",								nSttRow, "B", "B", NEXT, BLACK, 10);

	pCalcXL->SetXL(pXL, "2. 단 면 가 정",								nSttRow, "B", "B", NEXT, BLACK, 10);

	pCalcXL->SetXL(pXL, "3. 바 닥 판 설계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "3.1 바닥판 두께 산정",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.2 단 면 제 원",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.3 하 중 산 정",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.4 해 석 결 과",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.5 프리스트레스의 손실",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.6 바닥판 슬래브 응력 검토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.7 극한하중에 대한 단면검토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.8 정착구 설계",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.9 내부 지점부 주장률 산정",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.10 바닥판 배력 철근량 산정",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "3.11 거더 단부 바닥판의 보강",								nSttRow, "C", "C", NEXT, BLACK, 9);

	
	pCalcXL->SetXL(pXL, "4. 주 형 설 계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "4.1 플랜지 및 바닥판의 유효폭 계산",			nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.2 단 면 가 정",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.3 작용하중의 산정",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.4 주형의 단면력 산정",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "가) MODELING",									nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "나) 단 면 제 원",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "다) 하 중 재 하",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "라) 부재력 집계",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "마) 작용하중별 부재력집계",					nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "바) 부재력 요약",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "사) 반 력 집 계",								nSttRow, "D", "D", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.5 뒴비틀림(WARPING TORSION)에 대한 검토",	nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.6 2차응력에 대한 영향 검토",					nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.7 단 면 검 토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "4.8 단 면 요 약 도",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5. 이음부 설계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "5.1 현장이음 설계",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "5.2 용접부의 검토",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6. 보강재 설계",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "6.1 WEB 보강재",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.2 지점부 보강재",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "6.3 JackUp 보강재",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7. CROSS BEAM의 설계",							nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "7.1 가로보의 설계",							nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.2 수평 브레이싱의 설계",						nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "7.3 가로보 연결",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "8. 전단연결재",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9. 사용성검토",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "9.1 피 로 검 토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "9.2 처 짐 검 토",								nSttRow, "C", "C", NEXT, BLACK, 9);
	pCalcXL->SetXL(pXL, "10. 솟음(CAMBER)",								nSttRow, "B", "B", NEXT, BLACK, 10);
	pCalcXL->SetXL(pXL, "11. 신축이음 검토",							nSttRow, "B", "B", NEXT, BLACK, 10);
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

	// 강선의 존재여부
	BOOL bTendon = pDB->m_bTendonInputComplete;

	//표지
	if(m_bAbort) return FALSE;	
	m_nProgress = 5;		OnProgress(0, m_nProgress);	OnProgressText(1, "표 지");	
	NewSheet(pXL, "표 지", 0);	
	OutCalcCover(pXL);	
	pXL->SheetMoveCopy(FALSE, FALSE, "표 지");
	strArrUseSheet.Add("표 지");
		
	// 목차
	if(m_bAbort) return FALSE;	
	m_nProgress = 10;		OnProgress(0, m_nProgress);	OnProgressText(1, "목 차");	
	NewSheet(pXL, "목 차", 0);
	if (bTendon) OutCalcContents_With_Tendon(pXL);
	else		 OutCalcContents_WithOut_Tendon(pXL);	
	pXL->SheetMoveCopy(FALSE, FALSE, "목 차");
	strArrUseSheet.Add("목 차");

// review sim
	// 1. 설계조건
	if(m_bAbort) return FALSE;	
	m_nProgress = 15;		OnProgress(0, m_nProgress);	OnProgressText(1, "1. 설계조건");	
	pXL->SetActiveSheet("1. 설계조건");
	pXLDesignCondition->DesignCommon();			///< 1.1 일반사항
	if(m_bAbort) return FALSE;	
	pXLDesignCondition->LoadCondition();		///< 1.2 하중조건	
	if(m_bAbort) return FALSE;
	strArrUseSheet.Add("1. 설계조건");

	// 2. 단면가정
	if(m_bAbort) return FALSE;	
	m_nProgress = 20;		OnProgress(0, m_nProgress);	OnProgressText(1, "2. 단 면 가 정");	
	pXL->SetActiveSheet("2. 단면가정");	
	pXLDesignCondition->DesignSection();
	if(m_bAbort) return FALSE;	
	strArrUseSheet.Add("2. 단면가정");

	//////////////////////////////////////////////////////////////////////////
	// 강선이 존재하는 경우
	if (bTendon)
	{
		// 3. 바닥판 설계
		if(m_bAbort) return FALSE;
		szText.Format("3. 바닥판 설계");
		m_nProgress = 30;		OnProgress(0, m_nProgress);	OnProgressText(1, "3. 바닥판 설계");
		pXL->SetActiveSheet("3. 바닥판 설계");
		pXLDesignFloor->TendonDesignFloor();
		strArrUseSheet.Add(szText);

		// 3.3 하중산정
		if(m_bAbort) return FALSE;
		szText.Format("3.3 하중산정");
		m_nProgress = 35;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.3 하중산정");
		pXL->SetActiveSheet("3.3 하중산정");
		pXLDesignFloor->TendonLoadSum();
		strArrUseSheet.Add(szText);
	
		// 3.4 단면력 집계(1)
		if(m_bAbort) return FALSE;
		szText.Format("3.4 단면력 집계(1)");
		m_nProgress = 40;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.4 단면력 집계(1)");
		pXL->SetActiveSheet("3.4 단면력 집계(1)");
		pXLDesignFloor->TendonSectionSum();
		strArrUseSheet.Add(szText);
		
		// 3.5 프리스트레스의 손실(1)
		if(m_bAbort) return FALSE;
		szText.Format("3.5 프리스트레스의 손실(1)");
		m_nProgress = 45;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.5 프리스트레스의 손실(1)");
		pXL->SetActiveSheet("3.5 프리스트레스의 손실(1)");
		pXLDesignFloor->TendonPreStressLose();
		strArrUseSheet.Add(szText);
		
		// 3.6 바닥판 슬래브 응력 검토
		if(m_bAbort) return FALSE;
		szText.Format("3.6 바닥판 슬래브 응력 검토");
		m_nProgress = 50;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.6 바닥판 슬래브 응력 검토");
		pXL->SetActiveSheet("3.6 바닥판 슬래브 응력 검토");
		pXLDesignFloor->TendonFloorStress();
		strArrUseSheet.Add(szText);
		
		// 3.7 극한하중에 대한 단면검토
		if(m_bAbort) return FALSE;
		szText.Format("3.7 극한하중에 대한 단면검토");
		m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.7 극한하중에 대한 단면검토");
		pXL->SetActiveSheet("3.7 극한하중에 대한 단면검토");
		pXLDesignFloor->TendonLimitStressCheck();
		strArrUseSheet.Add(szText);
		
		// 3.8 정착구 설계
		if(m_bAbort) return FALSE;
		szText.Format("3.8 정착구 설계");
		m_nProgress = 70;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.8 정착구 설계");
		pXL->SetActiveSheet("3.8 정착구 설계");
		pXLDesignFloor->TendonSettlement();
		strArrUseSheet.Add(szText);
		
		// 3.9 내부 지점부 주장률 검토
		if(m_bAbort) return FALSE;
		szText.Format("3.9 내부 지점부 주장률 검토");
		m_nProgress = 80;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.9 내부 지점부 주장률 검토");
		pXL->SetActiveSheet("3.9 내부 지점부 주장률 검토");
		pXLDesignFloor->TendonInnerTension();
		strArrUseSheet.Add(szText);
		
		// 3.11 거더 단부 바닥판의 보강
		if(m_bAbort) return FALSE;
		szText.Format("3.11 거더 단부 바닥판의 보강");
		m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.11 거더 단부 바닥판의 보강");
		pXL->SetActiveSheet("3.11 거더 단부 바닥판의 보강");
		pXLDesignFloor->TendonGirderReinforce();
		strArrUseSheet.Add(szText);

	}
	//////////////////////////////////////////////////////////////////////////
	// 강선이 존재하지 않는 경우
	else
	{
 		if(m_bAbort) return FALSE;
 		m_nProgress = 30;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.1 좌측 캔틸레버부");
 		pXLDesignFloor->DesignFloor(TRUE);
 		strArrUseSheet.Add("3.1 좌측 캔틸레버부");

		if(m_bAbort) return FALSE;
		m_nProgress = 45;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.2 우측 캔틸레버부");
		pXLDesignFloor->DesignFloor(FALSE);
		strArrUseSheet.Add("3.2 우측 캔틸레버부");
			
		if(pOpt->GetSlabCentDesignMethod() == 0 || pOpt->GetSlabCentDesignMethod() == 2)		//0 : 중앙부  2 : LB-Deck 
		{
			if(m_bAbort) return FALSE;
			m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.3 중앙지간부");
			NewSheet(pXL, "3.3 중앙지간부", 0);
			pXLDesignFloor->DesignFloor_Center();
			strArrUseSheet.Add("3.3 중앙지간부");
		}
			
		CCalcGeneral	CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
		long nNum = 3;
		// 경험적이면..
		if(pOpt->GetSlabCentDesignMethod()==1)
		{
			
			if(!CalcGeneral.CheckExperienceDesign(TRUE) || !CalcGeneral.CheckExperienceDesign(FALSE))
			{
				if(m_bAbort) return FALSE;
				m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.3 중앙지간부");
				NewSheet(pXL, "3.3 중앙지간부", 0);
				pXLDesignFloor->DesignFloor_Center();
				strArrUseSheet.Add("3.3 중앙지간부");

				nNum++;
			}
			
			szText.Format("3.%d 경험적 설계검토", nNum);
			if(m_bAbort) return FALSE;
			m_nProgress = 97;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);

			//pXL->SetActiveSheet("3.4 경험적설계검토");
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
				szText.Format("3.%d 프리스트레스의 손실", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXL->SetActiveSheet("3.4 프리스트레스의 손실");
				pXL->SheetMoveCopy(FALSE, FALSE, szText);
				nRow = pXLDesignFloor->LossOfPreStress();
				strArrUseSheet.Add(szText);
				nNum++;
				
				if(m_bAbort) return FALSE;
				szText.Format("3.%d 바닥판 슬래브 응력검토", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, "3.5 바닥판 슬래브 응력검토");
				pXLDesignFloor->SlabStressCheck(nRow);
				nNum++;
			}
			
			if(!pDB->IsTUGir())
			{
				if(m_bAbort) return FALSE;
				szText.Format("3.%d 지점캔틸레버부(신축이음부)", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXL->SetActiveSheet("3.6 지점캔틸레버부(신축이음부)");
				pXL->SheetMoveCopy(FALSE, FALSE, szText);
				pXLDesignFloor->ExpansionJointUltimateLoad();
				strArrUseSheet.Add(szText);
				nNum++;
			}		
			
			if(pDB->m_nSlabTensionSize)
			{		
				if(m_bAbort) return FALSE;
				szText.Format("3.%d 극한하중에 대한 단면검토", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXL->SetActiveSheet("3.7 극한하중에 대한 단면검토");
				pXL->SheetMoveCopy(FALSE, FALSE, szText);
				pXLDesignFloor->SectionCheckAboutUltimateLoad();
				strArrUseSheet.Add(szText);
				nNum++;
			}
			
			if(nJigansu>1)
			{
				if(m_bAbort) return FALSE;
				szText.Format("3.%d 내부지점부 바닥판 주장율 검토", nNum);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				szText.Format("3.%d 지점부 바닥판검토", nNum);
				NewSheet(pXL, szText, 0);

				pXLDesignFloor->CheckInnerJijum();

				strArrUseSheet.Add(szText);
				nNum++;
				
				if(m_bAbort) return FALSE;
				szText.Format("3.%d 바닥판 배력 철근량산정", nNum);
				NewSheet(pXL, szText, 0);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXLDesignFloor->CheckFloorReinforce(szText);
				strArrUseSheet.Add(szText);
				nNum++;
				
				if(pDB->m_nSlabTensionSize)
				{			
					if(m_bAbort) return FALSE;
					szText.Format("3.%d 정착부 보강 설계", nNum);
					m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
					pXL->SetActiveSheet("3.10 정착부 보강 설계");
					pXL->SheetMoveCopy(FALSE, FALSE, szText);
					pXLDesignFloor->DesignFixingReinforce();
					strArrUseSheet.Add(szText);
					nNum++;
				}
			}
			else
			{
				szText.Format("3.%d 바닥판 배력 철근량산정", nNum);
				if(m_bAbort) return FALSE;
				NewSheet(pXL, szText, 0);
				m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
				pXLDesignFloor->CheckFloorReinforce(szText);
				strArrUseSheet.Add(szText);
				nNum++;
				
				if(pDB->m_nSlabTensionSize)
				{			
					if(m_bAbort) return FALSE;
					szText.Format("3.%d 정착부 보강 설계", nNum);
					m_nProgress = 90;		OnProgress(0, m_nProgress);	OnProgressText(1, szText);
					pXL->SetActiveSheet("3.10 정착부 보강 설계");
					pXL->SheetMoveCopy(FALSE, FALSE, szText);
					pXLDesignFloor->DesignFixingReinforce();
					strArrUseSheet.Add(szText);
					nNum++;
				}
			}	
		}
		else//판형교이면...
		{
			if(nJigansu > 1)
			{
				szText.Format("3.%d 지점부 바닥판검토", nNum);
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

	pXL->SetActiveSheet("표 지");
	pXL->SheetMoveCopy("0. 설계조건", FALSE);
	pXL->SetActiveSheet("목 차");
	pXL->SheetMoveCopy("0. 설계조건", FALSE);
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
	strSheet.Format("4.1 유효폭 계산");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 5;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->CalcEffectWidth();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.2 단면가정-거더");
	if(m_pAPlateOutCalcStd->GetAPlateBridgeApp()->IsTUGir())
		pXL->SetActiveSheet("4.2 단면가정-거더(TU)");
	else
		pXL->SetActiveSheet("4.2 단면가정-거더(일반)");	
	m_nProgress = 15;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->SectionAssumptionGirder();
	pXL->SetSheetName(strSheet);
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.2 단면가정-가로보");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 20;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->SectionAssumptionCR();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.3 작용하중의 산정(1)");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 30;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 작용하중의 산정 - 횡방향 하중분배");
	pXLDesignOfMold->ComputeHLoad();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.3 작용하중의 산정(2)");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 45;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 작용하중의 산정 - 풍하중");
	pXLDesignOfMold->ComputeHLoad_Wind();

	if(m_bAbort) return FALSE;
	m_nProgress = 50;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 작용하중의 산정 - 충격계수");
	pXLDesignOfMold->ComputeHLoad_Collision();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.3 작용하중의 산정(3)");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 60;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.3 작용하중의 산정 - 주형별 작용하중");
	pXLDesignOfMold->ComputeHLoad_Mold();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.4 가) 모델링");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 70;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->Modeling();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.4 나) 단면제원");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 75;		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLDesignOfMold->SectionJewon();
	strArrUseSheet.Add(strSheet);

	if(m_bAbort) return FALSE;
	strSheet.Format("4.4 다) 하중재하");
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
 		pXL->SetActiveSheet("4.4 라) 부재력집계");
 		m_nProgress = 1;		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 라) 부재력 집계");
 		pXLDesignOfMold->EleForceTotal();
 		strArrUseSheet.Add("4.4 라) 부재력집계");
 		
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.4 마) 작용하중별 부재력집계");	
 		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 마) 작용하중별 부재력");
 		pXLDesignOfMold->EleForceLoadType();
 		strArrUseSheet.Add("4.4 마) 작용하중별 부재력집계");
		
		if(m_bAbort) return FALSE;
		pXL->SetActiveSheet("4.4 바) 부재력 요약");	
		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 바) 부재력 요약");
		pXLDesignOfMold->EleForceSummary();
		strArrUseSheet.Add("4.4 바) 부재력 요약");
	}

 	if(bCheck2)
 	{
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.4 사) 반 력 집 계");	
 		OnProgress(0, m_nProgress);	OnProgressText(1, "4.4 사) 반 력 집 계");
 		pXLDesignOfMold->EleForceReaction();
 		strArrUseSheet.Add("4.4 사) 반 력 집 계");
 		
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.5 뒴비틀림(WARPING TORSION)");	
 		OnProgress(0, m_nProgress);	OnProgressText(1, "4.5 뒴비틀림(WARPING TORSION)에 대한 검토");
 		pXLDesignOfMold->WarpingTorsion();
 		strArrUseSheet.Add("4.5 뒴비틀림(WARPING TORSION)");
 		
 		if(m_bAbort) return FALSE;
 		pXL->SetActiveSheet("4.6 2차응력");
 		if(bHapSung && pDB->m_nQtyJigan > 1)
 		{
 			OnProgress(0, m_nProgress);	OnProgressText(1, "4.6 2차응력에 대한 영향 검토");
 			pXLDesignOfMold->InfluenceK();
 			strArrUseSheet.Add("4.6 2차응력");
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
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 단면검토(정모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_SEC, strArrUseSheet);

 	if(m_bAbort) return FALSE;
 	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 단면검토(부모멘트)");
 	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_SEC, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 단면검토(현장이음 정모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_SP, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 단면검토(현장이음 부모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_SP, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 추가단면검토(정모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_SECPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 추가단면검토(부모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_SECPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 변단면추가단면검토(정모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_BUYNPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 변단면추가단면검토(부모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_BUYNPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 사용자추가단면검토(정모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::POSITIVE_USERPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.7 사용자추가단면검토(부모멘트)");
	pXLSectionCheck->SectionCheck(CXLSectionCheck::NEGATIVE_USERPLUS, strArrUseSheet);

	if(m_bAbort) return FALSE;
	OnProgress(0, m_nProgress);	OnProgressText(1, "4.8 단면요약도");	
	pXL->SetActiveSheet("단면요약도");
	pXL->SheetMoveCopy(FALSE, TRUE, "4.8 단면요약도");
	pXL->SetActiveSheet("4.8 단면요약도");

	pXLSectionCheck->SectionOutLine();
	strArrUseSheet.Add("4.8 단면요약도");

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
		m_nProgress = 0;		OnProgress(0, m_nProgress);	OnProgressText(1, "5.1 현장이음 설계");
		NewSheet(pXL, "5.1 현장이음 설계", 4);
		pXLSpliceCheck->SpliceCheck();
		strArrUseSheet.Add("5.1 현장이음 설계");
	}

	if (bCheck2)
	{
		NewSheet(pXL, "5.2 용접부 검토", 4);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "5.2 용접부 검토");
		pXLWeldingCheck->WeldingCheck();
		strArrUseSheet.Add("5.2 용접부 검토");
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
	pXLDesignStiff->WebStiff(strArrUseSheet);	///< 6. 보강재의 설계

	if(pDB->m_nQtyJigan > 1)
	{
		NewSheet(pXL, "6.2 지점부 보강재(교대부)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.2 지점부 보강재 - 교대부");
		pXLDesignStiff->JijumSupport(strArrUseSheet, FALSE);

		NewSheet(pXL, "6.2 지점부 보강재(교각부)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.2 지점부 보강재 - 교각부");
		pXLDesignStiff->JijumSupport(strArrUseSheet, TRUE);

		NewSheet(pXL, "6.3 JackUp 보강재(교대부)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.3 JackUp 보강재 - 교대부");
		pXLDesignStiff->JackUpSupport(strArrUseSheet, FALSE);

		NewSheet(pXL, "6.3 JackUp 보강재(교각부)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.3 JackUp 보강재 - 교각부");
		pXLDesignStiff->JackUpSupport(strArrUseSheet, TRUE);
	}
	else
	{
		NewSheet(pXL, "6.2 지점부 보강재(교대부)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.2 지점부 보강재 - 교대부");
		pXLDesignStiff->JijumSupport(strArrUseSheet, FALSE);	

		NewSheet(pXL, "6.3 JackUp 보강재(교대부)", 5);
		if(m_bAbort) return FALSE;
		OnProgress(0, m_nProgress);	OnProgressText(1, "6.3 JackUp 보강재 - 교대부");
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
	m_nProgress = 0;		OnProgress(0, 0);	OnProgressText(1, "7.1 가로보");

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
	m_nProgress = 10;		OnProgress(0, m_nProgress);	OnProgressText(1, "7.1 가로보");
	
	pXLDesignCrossBeam->DesignCrossBeam(strArrUseSheet, nIdx);
	pXLDesignCrossBeam->DesignVBracingOut(strArrUseSheet, nIdx);

	if(m_bAbort) return FALSE;
	szText.Format("7.%d 수평브레이싱의 설계", nIdx);
	OnProgress(0, m_nProgress+10);	OnProgressText(1, szText);
	pXLDesignCrossBeam->DesignHorBracing(strArrUseSheet, nIdx);
	
	szText.Format("7.%d 가로보의 연결", nIdx);
	NewSheet(pXL, szText, 6, "굴림");
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

	OnProgress(0, m_nProgress);	OnProgressText(1, "8. 전단연결재, 사용성검토 및 신축이음 계산");

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
		strSheet.Format("8.1 각형전단연결재");
		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
		pXL->SetActiveSheet(strSheet);
		pXLDesignStud->SquareSHConnector();
		strArrUseSheet.Add(strSheet);

		strSheet.Format("8.2 수평전단철근");
		pXL->SetActiveSheet(strSheet);
		pXLDesignStud->HorSHSteel();
		strArrUseSheet.Add(strSheet);	
	}
	else
	{
		strSheet.Format("8. 전단연결재");
		OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
		pXL->SetActiveSheet(strSheet);
		pXLDesignStud->ShearConnector();
		strArrUseSheet.Add(strSheet);
	}	
	
	if(m_bAbort) return FALSE;
	strSheet.Format("9.1 피로검토");
	pXL->SetActiveSheet(strSheet);
	m_nProgress = 0;		OnProgress(0, m_nProgress);	OnProgressText(1, "9.1 피로검토");
	pXLUsingCheck->CheckFatigueStress();
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("9.2 처짐검토");
	pXL->SetActiveSheet(strSheet);
	OnProgress(0, m_nProgress);	OnProgressText(1, "9.2 처짐검토");
	pXLUsingCheck->CheckDroop();
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("10. 솟음");
	pXL->SetActiveSheet(strSheet);
	OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLEtcCheck->CheckCamber();
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("11. 신축이음검토(정밀식)");
	pXL->SetActiveSheet(strSheet);
	OnProgress(0, m_nProgress);	OnProgressText(1, strSheet);
	pXLEtcCheck->CheckExpasionJoint(ACCURACY_METHOD);
	strArrUseSheet.Add(strSheet);
	
	if(m_bAbort) return FALSE;
	strSheet.Format("11. 신축이음검토(간편식)");
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
			MessageBox(m_hWndOwner, "구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.", "WARNING", MB_OK | MB_ICONINFORMATION);
			break;
		case ERROR_FILEOPEN :
			MessageBox(m_hWndOwner, "파일을 여는데 실패하였습니다.", "WARNING", MB_OK | MB_ICONINFORMATION);
			break;
	}

	return FALSE;
}

CString CAPlateOutExcel::ReturnChar(int Num)
{
	int AL = 64;//65=A
	int quotient;//몫
	int mod;//나머지
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


//	int nNmg = 10 / 3; 처럼 하면.. 몫인.. 3 
//	int nNmg = 10 % 3; 처럼 하면.. 나머지인 1

}