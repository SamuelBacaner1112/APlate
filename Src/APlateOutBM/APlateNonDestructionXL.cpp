// APlateNonDestructionXL.cpp: implementation of the CAPlateNonDestructionXL class.
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

#define NONDES_STANDARD			0
#define NONDES_CALC_FOUNDATION	1
#define NONDES_SUMMARY			2

#define	OUTBMBLACK					1
#define RED							3
#define	BLUE						5
#define	YELLOW						6
#define GRAY						15
#define NEXT						1

CAPlateNonDestructionXL::CAPlateNonDestructionXL(CAPlateOutBMStd *pAPlateOutBMStd)
{
	m_pAPlateOutBMStd		= pAPlateOutBMStd;
	CWinApp	*pApp			= AfxGetApp();
	CString	csSection		= "Afx Directory"; 
	CString	csFixedItem		= "Fixed Directory";
	CString	csAPlate		= pApp->GetProfileString(csSection, csFixedItem);
	m_szReadFileName		= csAPlate + "\\APlateOutXLNonDestruction.dll";
	m_nTotSheetSu			= 3;
	
	for(long n = 0; n < 255; n++)
		m_nBMSheetIdx[n] = 0;
	
	m_nBMSheetIdx[0]	= NONDES_STANDARD;
	m_nBMSheetIdx[1]	= NONDES_CALC_FOUNDATION;
	m_nBMSheetIdx[2]	= NONDES_SUMMARY;	
	
	m_pAPlateOutBMBrInfo = new CAPlateOutBMBrInfo(pAPlateOutBMStd);	
}

CAPlateNonDestructionXL::~CAPlateNonDestructionXL()
{
	delete m_pAPlateOutBMBrInfo;
}

UINT CAPlateNonDestructionXL::DoWork()
{
	CPlateBridgeApp	*pDB		= m_pAPlateOutBMStd->GetBridgeApp();
	CStringArray	ArrDelSheet;
	CXLControl		XL;
	
	KillTheXL();	

	long	nProgress	= 0;
	CString	sText		= _T("");

	SetProgress("�����...", nProgress);

	try
	{

#ifdef _DEBUG
		XL.SetXLVisible(TRUE);
#endif	
		if(XL.OpenXL(m_szReadFileName) == FALSE)	return 1;

		for(long n = 0; n < m_nTotSheetSu; n++)
		{
			nProgress	+= (long)(100.0/(double)m_nTotSheetSu);
			sText.Format("%s", GetNamePrintSheet(n));
			SetProgress(sText, nProgress);

			APlateOutNonDes(&XL, m_nBMSheetIdx[n], ArrDelSheet);
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
			m_pAPlateOutBMStd->GetAPlateOutNonDesDlg()->m_ctrlProgress.SetPos(0);
			
			theException->Delete();
			return 1;
		}		
	}

	CAPlateOutNonDestructionDlg	*pAPlateOutNonDesDlg	= m_pAPlateOutBMStd->GetAPlateOutNonDesDlg();

	pAPlateOutNonDesDlg->GetDlgItem(IDC_BUTTON_NONDES_OUT)->EnableWindow(TRUE);
	pAPlateOutNonDesDlg->GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText("��¿Ϸ�");
	OnProgress(0, 0);
	return 0;
}

void CAPlateNonDestructionXL::KillTheXL()
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

void CAPlateNonDestructionXL::APlateOutNonDes(CXLControl	*pXL, long nOutIdx, CStringArray &ArrDelSheet)
{
	if(m_bAbort)	return;
	
	switch(nOutIdx)
	{
	case NONDES_STANDARD :				
		NonDestructionStandard(pXL, ArrDelSheet);
		break;
	case NONDES_CALC_FOUNDATION :		
		NonDestructionCalcFoundation(pXL, ArrDelSheet);
		break;
	case NONDES_SUMMARY :				
		NonDestructionSummary(pXL, ArrDelSheet);
		break;
	}
}

CString CAPlateNonDestructionXL::GetNamePrintSheet(long nSheet)
{
	CString	szPrintSheet	= _T("");
	if(m_nBMSheetIdx[nSheet] == NONDES_STANDARD)			szPrintSheet = _T("���ı��˻�(�Է°�)");
	if(m_nBMSheetIdx[nSheet] == NONDES_CALC_FOUNDATION)		szPrintSheet = _T("���ı��˻�(����ٰ�)");
	if(m_nBMSheetIdx[nSheet] == NONDES_SUMMARY)				szPrintSheet = _T("���ı��˻�(��������ǥ)");
	
	return szPrintSheet;
}

void CAPlateNonDestructionXL::NonDestructionStandard(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CAPlateOutBMBrInfo	*pBrInfo		= GetBridgeInfo(0);
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	CString	sCell	= _T("");
	
	pXL->SetActiveSheet("���ı��˻�(�Է°�)");
	ArrDelSheet.Add("���ı��˻�(�Է°�)");

	long	nSizeGir	= pDB->GetGirdersu();
	
	long	nSttGirCol	= 11;
	long	nEndGirCol	= nSttGirCol + (nSizeGir-1)*4;
	long	nSumCol1	= nEndGirCol + 4;
	long	nSumCol2	= nSumCol1 + 3;
	long	nSttRow		= 4;
	long	nEndRow		= 0;
	long	nRRow		= 0;
	CDoubleArray arrNumSize;
	
	CDoubleArray arrLengthButtWelding	= pBrInfo->GetLengthWebButtWelding();
	CDoubleArray arrLengthHStiffener	= pBrInfo->GetLengthHStiffener();
	
	CDoubleArray arrFlangeUpperWidth, arrFlangeLowerWidth;
	CDoubleArray arrCompressUpper, arrCompressLower;
	CDoubleArray arrFlangeUpperSort, arrFlangeLowerSort;

	double dCompressUpper[3][99];//[3] = 0:����, 1:����, 2:����... [99] = dSizeUpper��ŭ��...
	double dCompressLower[3][99];//[3] = 0:����, 1:����, 2:����... [99] = dSizeLower��ŭ��...

	//���/�Ϻ�  �÷��� ���� �� ���Ʈ ����.....
	pBrInfo->GetFlangeLengthCompress(TRUE, arrFlangeUpperWidth, arrCompressUpper, arrFlangeUpperSort, dCompressUpper);
	pBrInfo->GetFlangeLengthCompress(FALSE, arrFlangeLowerWidth, arrCompressLower, arrFlangeLowerSort, dCompressLower);		

	long dSizeUpper = arrFlangeUpperSort.GetSize();
	long dSizeLower = arrFlangeLowerSort.GetSize();
	double dSizeTotal = max(2,dSizeUpper+dSizeLower);

	long i = 0;
	long j = 0;
	long k = 0;

	//Ÿ��Ʋ�� ���� 
	nRRow	= nSttRow;
	// �Ŵ���
	pOut->SetXL(pXL, "�� �� �� ȣ",		nRRow, "B", "I", 0, 1, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "����",			nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);
	
	// �Ŵ�����
	pOut->SetXL(pXL, "�� �� �� ��",		nRRow, "B", "I", 0, 1, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "m",			    nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);	

	//Ÿ��Ʋ�� �������...
	CString strUnit = "m";
	CString strTitle1 = "�÷��� ����";
	CString strTitle2 = "�����";		
	CString strTitle3 = "����";	
	
	long nK = 0;
	long nCount = max(1, dSizeUpper);
	long nAdd = 0;
	
	for(i=0;i<3;i++)
	{
		if(i==0)
		{
			nK = 1;//�÷��� ������.. 1�� Loop
			pOut->SetXL(pXL, strTitle1,	nRRow, "B", "F", 0, OUTBMBLACK, 9, FALSE,TA_CENTER);
			pXL->SetMergeCell(nRRow-1, 1, (long)(nRRow+dSizeTotal-2) ,5);
				
		}
		else
		{
			nK = 3;//������ - ����, ����, ����� 3�� Loop
			pOut->SetXL(pXL, strTitle1,	nRRow, "B", "C", 0, OUTBMBLACK, 9, FALSE,TA_CENTER);
			pXL->SetMergeCell(nRRow-1, 1, (long)(nRRow+(dSizeTotal*3+nAdd)-2), 2);
			pXL->SetTextOrientation(nRRow-1,1,TR_VERTICAL);			
		}
		
		strTitle2 = "�����";

		for (long n=0;n<nK; n++)
		{
			if (nK > 1)
			{
				pOut->SetXL(pXL, strTitle2,	nRRow, "D", "F", 0, 1, 9, FALSE,TA_CENTER);
				pXL->SetMergeCell(nRRow-1, 3, (long)(nRRow+dSizeTotal-2) ,5);				
			}
			
			strTitle3 = "����";
			nCount = max(1, dSizeUpper);

			for (j=0; j<2; j++)
			{
				for (k=0; k<nCount; k++)
				{	
					if (nCount==1)		sText = strTitle3;
					else				sText.Format("%s%d", strTitle3, k+1);
					pOut->SetXL(pXL, sText,		nRRow, "G", "I", 0, 1, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, strUnit,	nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);					
				}
				
				nCount = max(1, dSizeLower);
				strTitle3 = "����";
			}
			
			if (n==0)
				strTitle2 = "������";
			else if (n==1)
				strTitle2 = "�����";
			
		}
		
		if (i==0)
		{
			strTitle1 = "������";					
			strUnit = "ea";
		}
		else if (i==1)
		{
			nAdd = 1;
			strTitle1 = "�����α���";
			strTitle2 = "�����";
			strUnit = "m";
		}		
	}

	//������ �´��..
	pOut->SetXL(pXL, "������ �´��",		nRRow, "D", "I", 0, 1, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "m",					nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);						

	//���򺸰���
	pOut->SetXL(pXL, "�� �� �� �� ��",		nRRow, "B", "I", 0, 1, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "m",					nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);
	
	//����������
	pOut->SetXL(pXL, "����������",			nRRow, "B", "C", 0, 1, 9, FALSE, TA_CENTER);
	pXL->SetMergeCell(nRRow-1, 1, nRRow+((pDB->m_nQtyJigan+1)*2+1)-2,2);
	pXL->SetTextOrientation(nRRow-1,1,TR_VERTICAL);	
	
	CString	szJijumName = _T("");

	for(long nJijum = 0; nJijum < pDB->m_nQtyJigan + 1; nJijum++)
	{
		if(nJijum==0 || nJijum==pDB->m_nQtyJigan)	sText	= _T("��������");
		else										sText	= _T("�߾�������");
		
		szJijumName = sText + "(" + pDB->m_strJijumName[nJijum] + ")";
		pOut->SetXL(pXL, szJijumName,			nRRow, "D", "I", 0, 1, 9, FALSE, TA_CENTER);
		pXL->SetMergeCell(nRRow-1, 3, nRRow,8);
		
		pOut->SetXL(pXL, "m",					nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "ea",					nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);
	}

	pOut->SetXL(pXL, "�� ��",					nRRow, "D", "I", 0, 1, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "m",						nRRow, "J", "K", 1, 1, 9, FALSE, TA_CENTER);


	pXL->SetCellColor(nSttRow-1, 1, nRRow-2, nSttGirCol-2, GRAY);
	

	for(long nG=0; nG<nSizeGir; nG++)
	{	
		CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
		nRRow	= nSttRow;

		long nSttCellCol	= nSttGirCol+nG*4;
		long nEndCellCol	= nSttGirCol+nG*4+3;
		CString szSttCol	= GetAlphabet(nSttCellCol);
		CString szEndCol	= GetAlphabet(nEndCellCol);
		arrNumSize.RemoveAll();
		
		// �Ŵ���
		sText.Format("G%d", nG+1);
		pOut->SetXL(pXL, sText,			nRRow, szSttCol, szEndCol, 1, OUTBMBLACK, 9, FALSE, TA_CENTER); 		
		
		// �Ŵ�����
		sText.Format("%.3f", toM(pBrInfo->GetLengthGirder(nG)));
		pOut->SetXL(pXL, sText,			nRRow, szSttCol, szEndCol, 1, RED,	9, FALSE, TA_CENTER, "GirderLength");		
		arrNumSize.Add(3);								

		//�÷��� ����
		CString strExcelName = "FlangeUpper";//�÷��� ���� ����..		"
		CString strExcelCnt = "1";
		CDoubleArray arrData = arrFlangeUpperSort;

		nCount = max(1, dSizeUpper);
		for (j=0; j<2; j++)
		{
			for (k=0; k<nCount; k++)
			{	
				strExcelCnt.Format("%d",k);
				strExcelName += strExcelCnt;
				if(j==0 && dSizeUpper>0)		sText.Format("%.3f", toM(arrData[k]));
				else if(j==1 && dSizeLower>0)	sText.Format("%.3f", toM(arrData[k]));
				else							sText.Format("0");
				pOut->SetXL(pXL, sText,	nRRow, szSttCol, szEndCol, 1, RED, 9, FALSE, TA_CENTER, strExcelName);
				arrNumSize.Add(3);
			}
			
			nCount = max(1, dSizeLower);
			strExcelName = "FlangeLower";//�÷��� ���� ����..
			arrData = arrFlangeLowerSort;
		}

		//������.		
		//0:����, 1:����, 2:����. = dSizeLower��ŭ��...						
		long n = 0;
		for (n=0;n<3; n++)
		{
			if (n == 0)			strExcelName = "TensionUpper";
			else if (n ==1)		strExcelName = "ComplexUpper";
			else				strExcelName = "CompressUpper";

			nCount = max(1, dSizeUpper);
			for (j=0; j<2; j++)
			{
				for (k=0; k<nCount; k++)
				{	
					strExcelCnt.Format("%d",k);
					strExcelName += strExcelCnt;
					if(j==0 && dSizeUpper>0)		sText.Format("%.0f", dCompressUpper[n][k]);						
					else if(j==1 && dSizeLower>0)	sText.Format("%.0f", dCompressLower[n][k]);
					else							sText.Format("0");
					pOut->SetXL(pXL, sText,	nRRow, szSttCol, szEndCol, 1, RED, 9, FALSE, TA_CENTER, strExcelName, 0);
					arrNumSize.Add(0);	
				}
				
				nCount = max(1, dSizeLower);
				
				if (n == 0)		strExcelName = "TensionLower";
				else if (n ==1)	strExcelName = "ComplexLower";
				else			strExcelName = "CompressLower";
			}									
		}

		CString P1, P2;

		for (n=0;n<3; n++)
		{
			if (n == 0)		P2 = "TensionUpper";
			else if (n ==1)	P2 = "ComplexUpper";
			else			P2 = "CompressUpper";
			
			nCount = max(1, dSizeUpper);
			P1 = "FlangeUpper";
			for (j=0; j<2; j++)
			{
				for (k=0; k<nCount; k++)
				{	
					strExcelCnt.Format("%d",k);
					P1 += strExcelCnt;
					P2 += strExcelCnt;
											
					sText.Format("=%s*%s", pOut->GetCellRef(P1),pOut->GetCellRef(P2));
					pOut->SetXL(pXL, sText,	nRRow, szSttCol, szEndCol, 1, BLACK, 9, FALSE, TA_CENTER, strExcelName);	
					arrNumSize.Add(3);
				}
				
				nCount = max(1, dSizeLower);
				P1 = "FlangeLower";

				if (n == 0)		P2 = "TensionLower";
				else if (n ==1)	P2 = "ComplexLower";
				else			P2 = "CompressLower";	
			}
			
		}
		
		// ������ �´��
		sText.Format("%.3f", toM(arrLengthButtWelding.GetAt(nG)));
		pOut->SetXL(pXL, sText, nRRow, nSttCellCol, nEndCellCol, 1, RED,	9, FALSE, TA_CENTER);
		arrNumSize.Add(3);

		//////////////////////////////////////////////////////////////////////////
		// ���򺸰��� ����
		sText.Format("%.3f", toM(arrLengthHStiffener.GetAt(nG)));
		pOut->SetXL(pXL, sText, nRRow, nSttCellCol, nEndCellCol, 1, RED,	9, FALSE, TA_CENTER);
		arrNumSize.Add(3);

		//////////////////////////////////////////////////////////////////////////
		// ����������
		CDoubleArray	arrSizeJijumStiffener;
		CDoubleArray	arrLengthJijumStiffener;
		CString			szSumRow				= _T("=");

		pBrInfo->GetLengthJijumStiffener(nG, arrLengthJijumStiffener, arrSizeJijumStiffener);

		for(long nJijum = 0; nJijum < pDB->m_nQtyJigan+1; nJijum++)
		{
			CPlateBasicIndex	*pBx	= pGir->GetBxOnJijum(nJijum);
			
			sText.Format("%.3f", toM(arrLengthJijumStiffener.GetAt(nJijum)));
			pOut->SetXL(pXL, sText, nRRow, nSttCellCol, nEndCellCol, 1, RED,	9, FALSE, TA_CENTER);
			sText.Format("+%s%d", GetAlphabet(nSttCellCol), nRRow-1);
			szSumRow += sText;
			arrNumSize.Add(3);

			sText.Format("%.0f", arrSizeJijumStiffener.GetAt(nJijum));
			pOut->SetXL(pXL, sText, nRRow, nSttCellCol, nEndCellCol, 1, RED,	9, FALSE, TA_CENTER, "", 0);
			arrNumSize.Add(0);
			
			// �հ�
			if(nJijum==pDB->m_nQtyJigan)
			{
				nEndRow	= nRRow;
				sText.Format("%s", szSumRow);
				pOut->SetXL(pXL, sText, nRRow, nSttCellCol, nEndCellCol, 1, BLACK,	9, FALSE, TA_CENTER);
				arrNumSize.Add(3);
			}
		}

		if(nG==nSizeGir-1)
		{
			nRRow	= nSttRow;
			
			sText.Format("�հ�");
			pOut->SetXL(pXL, sText, nRRow, nSumCol1, nSumCol2, 1, OUTBMBLACK,	9, FALSE, TA_CENTER);
			pXL->SetCellColor(nRRow-2, nSttGirCol, nRRow-2, nSumCol2, GRAY);

			long nTemp	= 0;
			long nR		= 0;
			for(nR=nRRow; nR<=nEndRow; nR++)
			{
				long nNum	= long(arrNumSize.GetAt(nTemp));
				sText.Format("=SUM(%s%d:%s%d)", GetAlphabet(nSttGirCol), nR, GetAlphabet(nEndGirCol+3), nR);
				pOut->SetXL(pXL, sText, nR, nSumCol1, nSumCol2, 0, OUTBMBLACK,	9, FALSE, TA_CENTER, "", nNum);
				nTemp++;
			}

			pXL->CellOutLineAll(nSttRow-1, 1, nR-2, nSumCol2);
		}
	}
	
	nSttRow	= nEndRow + 2;

	dSizeUpper = max(1, dSizeUpper);
	dSizeLower = max(1, dSizeLower);	

	//////////////////////////////////////////////////////////////////////////
	// Cell ����
	//////////////////////////////////////////////////////////////////////////
	nSttRow = 5;
	m_MapRefCell.RemoveAll();
	CString sSheetName	= _T("���ı��˻�(�Է°�)");
	CString sSumCol = GetAlphabet(nSumCol1);
	
	// �Ŵ� ����
	sText.Format("'%s'!%s%d", sSheetName, sSumCol, nSttRow);
	m_MapRefCell["Girder_Length"]		= sText;					// �Ŵ� ����
	nSttRow += (long)dSizeTotal+1;//
	
	// ������ ����
	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeUpper-1);
	m_MapRefCell["Connect_TenUp_Size"]		= sText;				// ���� ����� ���� ����
	nSttRow += dSizeUpper;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeLower-1);
	m_MapRefCell["Connect_TenLo_Size"]		= sText;				// ���� ����� ���� ����
	nSttRow += dSizeLower;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeUpper-1);
	m_MapRefCell["Connect_AlterUp_Size"]	= sText;				// ���� ������ ���� ����
	nSttRow += dSizeUpper;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeLower-1);
	m_MapRefCell["Connect_AlterLo_Size"]	= sText;				// ���� ������ ���� ����
	nSttRow += dSizeLower;
	nSttRow += (long)dSizeTotal;

	// ������ ����
	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeUpper-1);
	m_MapRefCell["Connect_TenUp_Length"]	= sText;				// ���� ����� ���� ����
	nSttRow += dSizeUpper;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeLower-1);
	m_MapRefCell["Connect_TenLo_Length"]	= sText;				// ���� ����� ���� ����
	nSttRow += dSizeLower;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeUpper-1);
	m_MapRefCell["Connect_AlterUp_Length"]	= sText;				// ���� ������ ���� ����
	nSttRow += dSizeUpper;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeLower-1);
	m_MapRefCell["Connect_AlterLo_Length"]	= sText;				// ���� ������ ���� ����
	nSttRow += dSizeLower;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeUpper-1);
	m_MapRefCell["Connect_ComUp_Length"]	= sText;				// ���� ����� ���� ����
	nSttRow += dSizeUpper;

	sText.Format("Sum('%s'!%s%d:%s%d)", sSheetName, sSumCol, nSttRow, sSumCol, nSttRow+dSizeLower-1);
	m_MapRefCell["Connect_ComLo_Length"]	= sText;				// ���� ����� ���� ����
	nSttRow += dSizeLower;

	sText.Format("'%s'!%s%d", sSheetName, sSumCol, nSttRow);
	m_MapRefCell["Connect_WebButtWeld_Length"]	= sText;			// ������ �´������ ����
	nSttRow++;


	// ���򺸰���
	sText.Format("'%s'!%s%d", sSheetName, sSumCol, nSttRow);
	m_MapRefCell["HStiffener_Length"]	= sText;					// ���򺸰��� ����
	nSttRow = nEndRow;

	// ����������
	sText.Format("'%s'!%s%d", sSheetName, sSumCol, nSttRow);
	m_MapRefCell["JijumStiffener_Length"]	= sText;				// ���������� ����
	nSttRow++;
}

void CAPlateNonDestructionXL::NonDestructionCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutBMStd->GetBridgeApp();
	CAPlateOutBMBrInfo	*pBrInfo		= GetBridgeInfo(0);
	CADeckData			*pADeckData		= m_pAPlateOutBMStd->GetDataManage()->GetDeckData();
	CAPlateOutBMFormat	*pOut			= m_pAPlateOutBMStd->GetAPlateOutBMFormat();
	CBMOption			*pBMOpt			= &pADeckData->m_BMOption;

	CString	sText	= _T("");
	CString	sCell	= _T("");

	pXL->SetActiveSheet("���ı��˻�(����ٰ�)");
	ArrDelSheet.Add("���ı��˻�(����ٰ�)");

	long	nSttRow	= 15;

	// 1.��缱��������
	sText.Format("=%s+%s", m_MapRefCell["Connect_TenUp_Size"], m_MapRefCell["Connect_TenLo_Size"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "L", 0, OUTBMBLACK,	9, FALSE, TA_CENTER, "", 0);
	nSttRow += 3;

	sText.Format("=%s+%s", m_MapRefCell["Connect_AlterUp_Size"], m_MapRefCell["Connect_AlterLo_Size"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "L", 0, OUTBMBLACK,	9, FALSE, TA_CENTER, "", 0);

	// 2.������Ž�����
	nSttRow = 45;
	sText.Format("=%s+%s", m_MapRefCell["Connect_TenUp_Length"], m_MapRefCell["Connect_TenLo_Length"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	nSttRow += 3;

	sText.Format("=%s+%s", m_MapRefCell["Connect_AlterUp_Length"], m_MapRefCell["Connect_AlterLo_Length"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	nSttRow += 3;

	sText.Format("=%s+%s", m_MapRefCell["Connect_ComUp_Length"], m_MapRefCell["Connect_ComLo_Length"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	nSttRow += 3;

	sText.Format("=%s", m_MapRefCell["Connect_WebButtWeld_Length"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	nSttRow += 3;

	sText.Format("=%s", m_MapRefCell["JijumStiffener_Length"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	nSttRow += 3;

	//sText.Format("=%s", m_MapRefCell["CrossBeam_Jijum_Length"]);
	//pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);

	//sText.Format("=%s", m_MapRefCell["CrossBeam_Gen_Length"]);
	//pOut->SetXL(pXL, sText, nSttRow, "O", "Q", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	//nSttRow += 2;

// 	if(pDB->GetLBracketsu()>0 || pDB->GetRBracketsu()>0)
// 	{
// 		nSttRow++;
// 
// 		sText.Format("=%s", m_MapRefCell["WingBeam_Jijum_Length"]);
// 		pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
// 
// 		sText.Format("=%s", m_MapRefCell["WingBeam_Gen_Length"]);
// 		pOut->SetXL(pXL, sText, nSttRow, "O", "Q", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
// 		nSttRow += 2;
// 	}
// 	else
// 	{
// 		pXL->DeleteRowLine(nSttRow, nSttRow+2);
// 		pXL->DeleteRowLineEnd();
// 	}
	
// 	sText.Format("=%s", m_MapRefCell["Stringer_Length"]);
// 	pOut->SetXL(pXL, sText, nSttRow, "X", "Z", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	nSttRow += 10;

	// 3.�ں�Ž�����
// 	sText.Format("=%s", m_MapRefCell["JRibUp_Length"]);
// 	pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
// 	
// 	sText.Format("=%s", m_MapRefCell["JRibLo_Length"]);
// 	pOut->SetXL(pXL, sText, nSttRow, "O", "Q", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	
	sText.Format("=%s", m_MapRefCell["HStiffener_Length"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "M", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);
	nSttRow += 3;

	sText.Format("=%s", m_MapRefCell["Girder_Length"]);
	pOut->SetXL(pXL, sText, nSttRow, "K", "L", 0, OUTBMBLACK,	9, FALSE, TA_CENTER);

}

void CAPlateNonDestructionXL::NonDestructionSummary(CXLControl *pXL, CStringArray &ArrDelSheet)
{
	pXL->SetActiveSheet("���ı��˻�(��������ǥ)");
	ArrDelSheet.Add("���ı��˻�(��������ǥ)");
}

void CAPlateNonDestructionXL::SetProgress(CString strText, long nlW)
{
	OnProgress(0, nlW);
	OnProgressText(1, strText);
}

CAPlateOutBMBrInfo* CAPlateNonDestructionXL::GetBridgeInfo(long nBridgeIdx)
{
	return m_pAPlateOutBMBrInfo;
}

CString CAPlateNonDestructionXL::GetAlphabet(long nCol)
{
	long nSttIdx	= (long)nCol/26;
	long nEndIdx	= nCol%26;
	CString	strStt	= _T("");

	if(nSttIdx!=0)
		strStt.Format("%c", (nSttIdx-1)+'a');
	
	switch(nEndIdx)
	{
	case 0: 
		strStt += "A";
		break;
	case 1: 
		strStt += "B";
		break;
	case 2: 
		strStt += "C";
		break;
	case 3: 
		strStt += "D";
		break;
	case 4: 
		strStt += "E";
		break;
	case 5: 
		strStt += "F";
		break;
	case 6: 
		strStt += "G";
		break;
	case 7: 
		strStt += "H";
		break;
	case 8: 
		strStt += "I";
		break;
	case 9: 
		strStt += "J";
		break;
	case 10: 
		strStt += "K";
		break;
	case 11: 
		strStt += "L";
		break;
	case 12: 
		strStt += "M";
		break;
	case 13: 
		strStt += "N";
		break;
	case 14: 
		strStt += "O";
		break;
	case 15: 
		strStt += "P";
		break;
	case 16: 
		strStt += "Q";
		break;
	case 17: 
		strStt += "R";
		break;
	case 18: 
		strStt += "S";
		break;
	case 19: 
		strStt += "T";
		break;
	case 20: 
		strStt += "U";
		break;
	case 21: 
		strStt += "V";
		break;
	case 22: 
		strStt += "W";
		break;
	case 23: 
		strStt += "X";
		break;
	case 24: 
		strStt += "Y";
		break;
	case 25: 
		strStt += "Z";
		break;
	default:
		strStt += "";
		break;
	}

	return	strStt;
}


///////////////////////////////////////////////////////////////////////////////////
//��š񢱢����� ���¥åĥťƥǥ�������ҥ��ɥ��������f ����������
//	���£��������¡áġҡ����� �� �� �� kgf/�� t��m  ����������������
//  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ������������¨èĨ�
//  ��������������������������  �� �� �Ѣ�
//  �� �� �� ������ �� ���� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
///////////////////////////////////////////////////////////////////////////////////// 
