// XLUsingCheck.cpp: implementation of the CXLUsingCheck class.
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
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

CXLUsingCheck::CXLUsingCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;

}

CXLUsingCheck::~CXLUsingCheck()
{

}

void CXLUsingCheck::CheckFatigueStress()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CFemModelingCalc    ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CString szGussetType = pBridgeApp->m_dRoundCrossLower < 50 ? "E" : "D";
	m_nSttRow = 1;
	CString sMark, sText;
	pOut->ClearCellRef();
	pOut->SetXL(pXL, "9. ��뼺 ����"						, m_nSttRow, "A", "A", 1, BLACK, 11, TRUE);
	pOut->SetXL(pXL, "9.1 �Ƿΰ��� (�� ��.��.�� 3.3.4.1)"   , m_nSttRow, "B", "B", 1, BLACK, 9);
	pOutExcel->SetProgress("10. ��뼺 ����", pOutExcel->GetProgress()+10);

	pOut->SetXL(pXL, "��) ���¹ݺ�Ƚ��"								, m_nSttRow, "B", "B", 1);
	///< ���̺� �׸���
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "J", 2);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "K", "X", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AE", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF", "AN", 2);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "K", "Q", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "R", "X", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AE", 1);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "C",  "J",  1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "K",  "Q",  1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "R",  "X",  1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y",  "AE", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF", "AN", 1);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "C",  "J",  1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "K",  "Q",  1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "R",  "X",  1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y",  "AE", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF", "AN", 1);
	m_nSttRow-=3;
	pOut->SetXL			(pXL, "���� �� ���߱���"	, m_nSttRow, "D",  "J",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "������ �ֺ���"		, m_nSttRow, "K",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "Ⱦ���� ����"			, m_nSttRow, "Y",  "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "��  ��"				, m_nSttRow, "AF", "AN", 2, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow++;
	pOut->SetXL			(pXL, "������ ����"			, m_nSttRow, "C",  "G",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "Ʈ������"			, m_nSttRow, "K",  "Q",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "��������"			, m_nSttRow, "R",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "Ʈ������"			, m_nSttRow, "Y",  "AE", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			(pXL,"���,����,�ְ��� ����", m_nSttRow, "C",  "J",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "2 �鸸"				, m_nSttRow, "K",  "Q",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "50 ��"				, m_nSttRow, "R",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "2 �鸸 �̻�"			, m_nSttRow, "Y",  "AE", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			(pXL, "��Ÿ����"			, m_nSttRow, "C",  "J",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "10 ��"				, m_nSttRow, "K",  "Q",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "10 ��"				, m_nSttRow, "R",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "50 ��"				, m_nSttRow, "Y",  "AE", 1, BLACK, 9, FALSE, TA_CENTER);

	m_nSttRow++;
	pOut->SetXL(pXL, "��) ������� ����  fsr ( MPa )", m_nSttRow, "B", "B", 1);
	///< ���̺� �׸���
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "F", 2);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "G", "W", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "X", "AN",1);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "G", "J", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "K", "N", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "O", "R", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "S", "W", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "X", "AA", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI", 1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN", 1);
	m_nSttRow++;
	long n = 0;
	for(n=0; n<9; n++)
	{
		if(pOutExcel->m_bAbort)	return;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "F",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "G", "J",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "K", "N",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "O", "R",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "S", "W",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "X", "AA", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN", 1);
		m_nSttRow++;
	}

	m_nSttRow-=11;
	pOut->SetXL(pXL, "�� ��"		  , m_nSttRow, "C", "F",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "������ ��α���", m_nSttRow, "G", "W",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "������ ��α���", m_nSttRow, "X", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "�� ��"		  , m_nSttRow, "C", "F",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "10 ��ȸ"		  , m_nSttRow, "G", "J",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "50 ��ȸ"		  , m_nSttRow, "K", "N",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 ��ȸ"		  , m_nSttRow, "O", "R",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 ��ȸ �̻�"  , m_nSttRow, "S", "W",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "10 ��ȸ"		  , m_nSttRow, "X", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "50 ��ȸ"		  , m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 ��ȸ"		  , m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 ��ȸ �̻�"  , m_nSttRow, "AJ","AN", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

	char* szBaseStr[] = {
		"A",   "442", "260", "168", "168", "351", "203", "168", "168",  				
		"B",   "344", "203", "126", "112", "274", "161", "112", "112",
		"B��", "274", "161", "101", "84",   "218", "126", "77",   "77",
		"C",   "250", "147", "91",   "70",   "196", "112", "70",   "63",
		"",    ""     , ""     , ""   ,   "84*",	""     , ""   ,	  "84*",  "77*",
		"D",   "196", "112", "70",   "49",   "154", "91",   "56",   "35",
		"E",   "154", "91 " , "56",   "31",   "119", "70",   "42",   "16",
		"E��", "112", "64 " , "40",   "18",   "84",   "49",   "28",   "9",
		"F",   "105", "84 " , "63",   "56",   "84",   "63",   "49",   "42"
		};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray sArr;
	for(n = 0; n < nCount; n++)
	{
		if(pOutExcel->m_bAbort)	return;
		pOut->SetXL(pXL, szBaseStr[n],   m_nSttRow, "C", "F",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+1], m_nSttRow, "G", "J",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+2], m_nSttRow, "K", "N",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+3], m_nSttRow, "O", "R",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+4], m_nSttRow, "S", "W",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+5], m_nSttRow, "X", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+6], m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+7], m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, szBaseStr[n+8], m_nSttRow, "AJ","AN", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		m_nSttRow++; 
		n+=8;
	}

	m_nSttRow++;
	pOut->SetXL(pXL, "��) �Ƿ�������ġ �� ���¹����� ����"														 	 , m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "Ȱ���߿� ���� ���Ʈ�� ���������� ū �ܸ鿡�� �������� �� ���������� �߻��ϴ� �󼼺ο� ���Ͽ� ", m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "�Ƿΰ��並 ���� �Ѵ�."																		 , m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "�� �Ƿ����� �ܸ���ġ"																			 , m_nSttRow, "C", "C", 1);
	
	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(500);
	StressCalc.OutputOfFatigueCheckPos(&Dom, FALSE, 7);
	if(Dom.GetExtHeight()>0)	// �����ΰ� ���°�� �Ƿ����°��䰡 ������ �ʴ´� #25502
		pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 2, 15, TRUE, FALSE);
	else
		pOut->SetXL(pXL, "�Ƿ����� �ܸ��� �������� �ʽ��ϴ�.", m_nSttRow, "D", "D", 1);
	m_nSttRow++;
//	pOut->InsertDomyun(pXL, &Dom, m_nSttRow, 0, 7, TRUE);
	
	CString sRoadType = "�ְ�������";
	CString sStruType = "�����ϰ��";

	if(pCalcData->m_nFatigueRoadType == 1) sRoadType = "��Ÿ����";
	if(pCalcData->m_nFatigueStructureType == 1) sStruType ="�����ϰ��";

	pOut->SetXL(pXL, "�� ������"	, m_nSttRow, "C", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, sRoadType		, m_nSttRow, "G", "J", 0, RED,   9, FALSE, TA_CENTER, "��������", 0);
	pOut->SetXL(pXL, "�� ���ϴ�"	, m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, sStruType		, m_nSttRow, "O", "R", 0, RED,   9, FALSE, TA_CENTER, "���ϰ��", 0);
	pOut->SetXL(pXL, "�����μ�"		, m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "Ʈ������"		, m_nSttRow, "W", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"�ְ�������\",\"2�鸸ȸ\",\"10��ȸ\")", pOut->GetCellRef("��������"));
	pOut->SetXL(pXL, sText			, m_nSttRow, "AA","AC", 0, BLACK, 9, FALSE, TA_CENTER, "Ʈ�����߼�", 0); 
	pOut->SetXL(pXL, "�� ��������"	, m_nSttRow, "AD","AH", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"�ְ�������\",\"50��ȸ\",\"10��ȸ\"", pOut->GetCellRef("��������"));
	pOut->SetXL(pXL, sText			, m_nSttRow, "AI","AK", 1, BLACK, 9, FALSE, TA_CENTER, "�������߼�", 0); 
	pOut->SetXL(pXL, "�� ���Ͽ� ���� �ϴ� ������ �Ѵ�.", m_nSttRow, "C","C", 1, BLACK, 9, FALSE, TA_LEFT);
	
	long nChecksu = min(pBridgeApp->m_nQtyJigan, 3);
	CPlateBasicIndex *pBx = NULL;
	///< �ܸ�1	--------------------------------------------------------------
	pOut->SetXL(pXL, "�ܸ� 1", m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->InsertImage(pXL, "��뼺����_�ܸ�1", m_nSttRow, "G");
	m_nSttRow+=12;
	// ���׸���
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D", 2);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "X", 2);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AA",2);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AI",1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN",2);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE",1);
	pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI",1);
	m_nSttRow++;
	for(n=0; n<4; n++)
	{
		if(pOutExcel->m_bAbort)	return;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "X",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AA", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN", 1);
		m_nSttRow++;
	}
	//Grid text
	m_nSttRow-=6;
	pOut->SetLineBoxText(pXL, "����"				, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "��          ��"		, m_nSttRow, "E", "X",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetXL			(pXL, "�� ��"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, " fsr ( MPa ) "	, m_nSttRow, "AB","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetLineBoxText(pXL, "��  ��"				, m_nSttRow, "AJ","AN", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	m_nSttRow++;
	pOut->SetXL			(pXL, "�� ��"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "DB ����"				, m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "DL ����"				, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	m_nSttRow++;
	pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "�÷����� �������� ���� �ʷ� ������", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, "B"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O13,G13),IF(%s=\"2�鸸ȸ\",AF13,X13))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_1_DB", 0); 
	sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K13,G13),IF(%s=\"50��ȸ\",AB13,X13))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_1_DL", 0); 
	m_nSttRow++;	
	pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "������������ ���� ���κ�", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O15,G15),IF(%s=\"2�鸸ȸ\",AF15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_2_DB", 0); 
	sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K15,G15),IF(%s=\"50��ȸ\",AB15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_2_DL", 0); 
	m_nSttRow++;
	pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "���򺸰��� ���� ���κ�", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, "E"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O18,G18),IF(%s=\"2�鸸ȸ\",AF18,X18))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_3_DB", 0); 
	sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K18,G18),IF(%s=\"50��ȸ\",AB18,X18))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_3_DL", 0); 
	m_nSttRow++;
	pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "�����ǿ� ������ ���κ� ����� �ż��� ���� ���κ�", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, szGussetType			, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "�ż��� ����", 0); 
	sText.Format("=IF(%s=\"E\",IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O18,G18),IF(%s=\"2�鸸ȸ\",AF18,X18)),IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O17,G17),IF(%s=\"2�鸸ȸ\",AF17,X17)))",
		pOut->GetCellRef("�ż��� ����"), pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"),
		pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_4_DB", 0); 
	sText.Format("=IF(%s=\"E\",IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K18,G18),IF(%s=\"50��ȸ\",AB18,X18)),IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K17,G17),IF(%s=\"50��ȸ\",AB17,X17)))",
		pOut->GetCellRef("�ż��� ����"), pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"),
		pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_4_DL", 0); 
	sText.Format("* �ż��� ���� 50 �̻��� ��� D ���� ����");
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AQ", "AQ", 0);
	m_nSttRow+=2; 

	CheckFatigueStressSub(nChecksu);
	
	pOut->SetXL(pXL, "��) �����Ƿ�����"											 , m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "�� �ڸ��Ʈ�� ���� �Ƿΰ���"								 , m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "���⼭, ��f: �����Ƿ����¹��� (�ִ����°� �ּ������� ��)"	 , m_nSttRow, "K", "K", 1);
	pOut->SetXL(pXL, "��M = Mmax - Mmin  (4.4 ��.�Ƿ����� �ۿ���� ����) "		 , m_nSttRow, "K", "K", 1);
	pOut->SetXL(pXL, "Iv : �ռ��� �ܸ��� �ܸ�2�� ���Ʈ"						 , m_nSttRow, "K", "K", 1);
	pOut->SetXL(pXL, " y : �ռ��� �ܸ� �������κ����� �Ÿ�"						 , m_nSttRow, "K", "K", 1);
	m_nSttRow-=2;
	pOut->SetLineBoxText  (pXL, "��f =", m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "��M"  , m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetLineBoxText  (pXL, "y"    , m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	m_nSttRow++;
	pOut->SetXL			  (pXL, "Iv"   , m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	m_nSttRow+=2;

	long nNodeCheck = 0;
	long nElmCheck  = 0;
	double dRatio   = 0; 
	double DB=0, DL=0, Iv=0, Yvsu=0, Yvsl=0, Y1=0, Y2=0, Y3=0, Y4=0, dThickFlUP=0, dThickFlDN=0;
	long   nG = 0;
	long nHStiffDan=0;
	double dHeightWStiff=0, dSta=0;
	for(n=0; n<nChecksu; n++)
	{
		if(pOutExcel->m_bAbort)	return;
		nNodeCheck = ModelCalc.GetMaxFatigueIndex(n+1, nElmCheck, dRatio, 1);  ///< DB ���� check
		if(nNodeCheck<=0)	continue;

		sText.Format("�ܸ� %d", n+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "B", "B");
		//���׸���
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "L", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "M", "Q", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "R", "T", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "U", "AB", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AC", "AE", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF", "AM", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AN", "AP", 2);
		m_nSttRow++;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "H");
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "I", "L");
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "U", "X");
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AB");
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF", "AI");
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ", "AM");
		m_nSttRow++;
		for(long nRow=0; nRow<4; nRow++)
		{
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D");
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "R", "T");
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "U", "X");
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AB");
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AC","AE");
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI");
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AM");
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AN","AP");
			m_nSttRow++;
		}
		m_nSttRow-=4;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "H", 4);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "I", "L", 4);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "M", "Q", 4);
		//�׸��幮��
		m_nSttRow-=2;
		pOut->SetLineBoxText(pXL, "����"			 , m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��M ( kN m )"	 , m_nSttRow, "E", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "Iv (mm4)"		 , m_nSttRow, "M", "Q", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "y (mm)"			 , m_nSttRow, "R", "T", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��f ( MPa )"  , m_nSttRow, "U", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "�� ��"			 , m_nSttRow, "AC","AE", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, " fsr ( MPa )" , m_nSttRow, "AF","AM", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��"			 , m_nSttRow, "AN","AP", 2, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow++;
		pOut->SetXL			(pXL, "DB ����"			, m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL ����"			, m_nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DB ����"			, m_nSttRow, "U", "X", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL ����"			, m_nSttRow, "Y", "AB",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DB ����"			, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL ����"			, m_nSttRow, "AJ","AM",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "�� ��"			, m_nSttRow, "AC","AE",1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow-=4;
		
		DB = StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MAX, nElmCheck, dRatio) -
			 StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MIN, nElmCheck, dRatio);

		DL = StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DL_MAX, nElmCheck, dRatio) -
			 StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DL_MIN, nElmCheck, dRatio);

		pBx  = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nNodeCheck));
		nG   = ModelCalc.GetGirderNoByJointID(nNodeCheck);
		dSta = pBx->GetStation();

		CVStiff    *pVStiff = pBx->GetSection()->GetVStiff();
		CWebHStiff *pWStiff = pBx->GetWebHStiff(); 
		CHBracingGusset *pHGusset = pBx->GetHBracingGusset();
		BOOL bPositiveM = StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);

		ModelCalc.GetSectionCoefficient(nG, dSta, TRUE);
		StressCalc.GetSectionStress(nG, dSta);

		if(pBridgeApp->IsTUGir())
		{
			if(bPositiveM)	Iv = ModelCalc.GetMapValueSecCo("Ivd33");
			else			Iv = StressCalc.GetMapValueSecStress("Irv");
		}
		else
		{
			if(bPositiveM)	Iv = ModelCalc.GetMapValueSecCo("Iv33");
			else			Iv = StressCalc.GetMapValueSecStress("Irv");
		}

		Yvsu = (StressCalc.GetMapValueSecStress("Yvsu"));
		Yvsl = (StressCalc.GetMapValueSecStress("Yvsl"));
		dThickFlUP = (pBridgeApp->GetGirder(nG)->GetThickFactByStation(dSta, G_F_U));
		dThickFlDN =( pBridgeApp->GetGirder(nG)->GetThickFactByStation(dSta, G_F_L));
		
		if(n==0 || n==2)	nHStiffDan = 0;
		else				nHStiffDan = MAX(pWStiff->GetUpperDan(TRUE)+pWStiff->GetLowerDan(TRUE)
			                                ,pWStiff->GetUpperDan(FALSE)+pWStiff->GetLowerDan(FALSE)) -1;
		dHeightWStiff		 =  (pWStiff->GetHeightHStiffBySta(nHStiffDan, TRUE, dSta));
		if(dHeightWStiff==0)	(pWStiff->GetHeightHStiffBySta(nHStiffDan, FALSE, dSta));

		if(n==0 || n==2)
		{
			Y1 = Yvsl+dThickFlDN;						///< �÷����� �������� ���� �ʷ� ������
			Y2 = Yvsl+dThickFlDN+(MIN(pVStiff->m_dGap[0], pVStiff->m_dGap[1]));		///< ������������ ���� ���κ�
			Y3 = Yvsu-dThickFlUP-dHeightWStiff;			///< ���򺸰��� ���� ���κ�
			Y4 = Yvsl+(pHGusset->m_ddH-pHGusset->m_dT);	///< �����ǿ� ������ ���κ� ����� �ż��� ���� ���κ�	
		}
		else
		{
			Y1 = Yvsu-dThickFlUP;						///< �÷����� �������� ���� �ʷ� ������
			Y2 = Yvsu-dThickFlUP;						///< �����κ����� ���� ���κ�
			Y3 = Yvsu-dThickFlUP-dHeightWStiff;			///< ���򺸰��� ���� ���κ�
			Y4 = Yvsu;									///< ���͵忡 ������ ������ �÷�����	
		}
		pOut->SetLineBoxText(pXL, DB,		m_nSttRow, "E","H", 4, RED, 9, FALSE, TA_CENTER, "DB", 3);
		pOut->SetLineBoxText(pXL, DL,		m_nSttRow, "I","L", 4, RED, 9, FALSE, TA_CENTER, "DL", 3);
		pOut->SetLineBoxText(pXL, Iv,		m_nSttRow, "M","Q", 4, RED, 9, FALSE, TA_CENTER, "Iv", 0);
		pOut->SetXL			(pXL, ABS(Y1),  m_nSttRow, "R","T", 1, RED, 9, FALSE, TA_CENTER, "Y1", 1);
		pOut->SetXL			(pXL, ABS(Y2),  m_nSttRow, "R","T", 1, RED, 9, FALSE, TA_CENTER, "Y2", 1);
		pOut->SetXL			(pXL, ABS(Y3),  m_nSttRow, "R","T", 1, RED, 9, FALSE, TA_CENTER, "Y3", 1);
		pOut->SetXL			(pXL, ABS(Y4),  m_nSttRow, "R","T", 1, RED, 9, FALSE, TA_CENTER, "Y4", 1);
		m_nSttRow-=4;	///< DB
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DB"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y1"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "U","X", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F1_DB");
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DB"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y2"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "U","X", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F2_DB");
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DB"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y3"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "U","X", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F3_DB");
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DB"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y4"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "U","X", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F4_DB");
		m_nSttRow-=4;  ///< DL
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DL"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y1"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "Y","AB", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F1_DL");
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DL"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y2"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "Y","AB", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F2_DL");
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DL"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y3"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "Y","AB", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F3_DL");
		sText.Format("=%s*10^6/%s*%s", pOut->GetCellRef("DL"), pOut->GetCellRef("Iv"), pOut->GetCellRef("Y4"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "Y","AB", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_F4_DL");
		m_nSttRow-=4;  ///< ���¹���
		pOut->SetXL			(pXL, "B", m_nSttRow, "AC","AE", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "C", m_nSttRow, "AC","AE", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "E", m_nSttRow, "AC","AE", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format(n==1? "C" : szGussetType);
		pOut->SetXL			(pXL, sText, m_nSttRow, "AC","AE", 1, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow-=4;  ///< fsr-DB
		sText.Format("SEC%d_1_DB", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AF","AI", 1, BLACK, 9, FALSE, TA_CENTER, "FSR1_DB", 0);
		sText.Format("SEC%d_2_DB", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AF","AI", 1, BLACK, 9, FALSE, TA_CENTER, "FSR2_DB", 0);
		sText.Format("SEC%d_3_DB", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AF","AI", 1, BLACK, 9, FALSE, TA_CENTER, "FSR3_DB", 0);
		sText.Format("SEC%d_4_DB", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AF","AI", 1, BLACK, 9, FALSE, TA_CENTER, "FSR4_DB", 0);
		m_nSttRow-=4;  ///< fsr-DL
		sText.Format("SEC%d_1_DL", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AJ","AM", 1, BLACK, 9, FALSE, TA_CENTER, "FSR1_DL", 0);
		sText.Format("SEC%d_2_DL", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AJ","AM", 1, BLACK, 9, FALSE, TA_CENTER, "FSR2_DL", 0);
		sText.Format("SEC%d_3_DL", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AJ","AM", 1, BLACK, 9, FALSE, TA_CENTER, "FSR3_DL", 0);
		sText.Format("SEC%d_4_DL", n+1);
		sText.Format("=%s", pOut->GetCellRef(sText));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AJ","AM", 1, BLACK, 9, FALSE, TA_CENTER, "FSR4_DL", 0);
		m_nSttRow-=4;  ///< ���
		sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K.\",\"N.G.\")", pOut->GetCellRef("DELTA_F1_DB"), pOut->GetCellRef("FSR1_DB")
																, pOut->GetCellRef("DELTA_F1_DL"), pOut->GetCellRef("FSR1_DL"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K.\",\"N.G.\")", pOut->GetCellRef("DELTA_F2_DB"), pOut->GetCellRef("FSR2_DB")
																, pOut->GetCellRef("DELTA_F2_DL"), pOut->GetCellRef("FSR2_DL"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K.\",\"N.G.\")", pOut->GetCellRef("DELTA_F3_DB"), pOut->GetCellRef("FSR3_DB")
																, pOut->GetCellRef("DELTA_F3_DL"), pOut->GetCellRef("FSR3_DL"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K.\",\"N.G.\")", pOut->GetCellRef("DELTA_F4_DB"), pOut->GetCellRef("FSR4_DB")
																, pOut->GetCellRef("DELTA_F4_DL"), pOut->GetCellRef("FSR4_DL"));
		pOut->SetXL			(pXL, sText, m_nSttRow, "AN","AP", 1, BLACK, 9, FALSE, TA_CENTER);
	}
	
	if(pOutExcel->m_bAbort)	return;
	m_nSttRow++;
	pOut->SetXL(pXL, "�� ���ܿ� ���� �Ƿΰ��� - �÷����� �������� �ʷ������ο� ���� �����Ƿΰ���", m_nSttRow, "C","C", 1);
	pOut->SetXL(pXL, "�� ������ �Ƿλ󼼹��ִ� F �̸�, ���� �ܸ��� ���ܷ��� ���� ū ����"		 , m_nSttRow, "D","D", 0);

	nNodeCheck = ModelCalc.GetMaxFatigueIndex(4, nElmCheck, dRatio, 1);  ///< �ִ����ܷ�
	pOut->SetXL(pXL, nNodeCheck+1, m_nSttRow, "AC","AD", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "�� ���� �Ͽ���"															 , m_nSttRow, "AE","AE", 1);
	m_nSttRow++;
	pOut->SetXL(pXL, "���⼭, �ĥ�:  ���������Ƿ����¹��� (�ִ����°� �ּ������� ��)"			 , m_nSttRow, "N","N", 1);
	pOut->SetXL(pXL, "��S = Smax - Smin   (4.4 ��.�Ƿ����� �ۿ���� ����) "						 , m_nSttRow, "Q","Q", 1);
	///< �׸�����
	pOut->InsertImage(pXL, "���������°���", m_nSttRow, "B");
	pOut->SetXL(pXL, "Q    : ������ ���� �÷����� �Ѵܸ��� �߸��࿡ ����"						 , m_nSttRow, "Q","Q", 1);
	pOut->SetXL(pXL, "�ܸ� 1�� ���Ʈ (mm3)  =  A(mm��)��y(mm) "								 , m_nSttRow, "T","T", 1);
	pOut->SetXL(pXL, "y = Yvsu(l) - tu(l)/2 "													 , m_nSttRow, "T","T", 1);
	pOut->SetXL(pXL, "Iv  :   �ռ��� �ܸ��� �ܸ�2�� ���Ʈ"										 , m_nSttRow, "Q","Q", 1);
	pOut->SetXL(pXL, "��a :   �ʷ� ������ ��β��� �� (mm) = 4EA ��s / �� 2"					 , m_nSttRow, "Q","Q", 1);
	pOut->SetXL(pXL, "s   :   �ʷ� ���� ġ�� (mm)"												 , m_nSttRow, "Q","Q", 1);
	m_nSttRow-=8;
	pOut->SetLineBoxText  (pXL, "�ĥ�="		, m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "��S ��Q"	, m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	m_nSttRow++;
	pOut->SetXL			  (pXL, "Iv����a"   , m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	m_nSttRow+=8;

	for(n=0; n<2; n++)
	{
		if(nNodeCheck < 0)  continue;

		if(n==0)	pOut->SetXL(pXL, "�� ��� �÷����� ������"		, m_nSttRow, "C","C", 1);
		if(n==1)	pOut->SetXL(pXL, "�� �Ϻ� �÷����� ������"		, m_nSttRow, "C","C", 1);
		//���׸���
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "B", "D",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "H",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "I", "J",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "K", "M",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "N", "P",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "Q", "S",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "T", "W",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "X", "AA", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AC", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AD","AF", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AG","AJ", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AK","AN", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AO","AP", 2);
		m_nSttRow+=2;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "B", "D",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "H",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "I", "J",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "K", "M",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "N", "P",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "Q", "S",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "T", "W",  2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "X", "AA", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AC", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AD","AF", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AG","AJ", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AK","AN", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AO","AP", 1);
		m_nSttRow++;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "B", "D",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "H",  1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AG","AJ", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AK","AN", 1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AO","AP", 1);
		// �׸��� ����
		m_nSttRow-=3;
		pOut->SetLineBoxText(pXL, "��  ��", m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��S"   , m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "tu"    , m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "B"     , m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "A"     , m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, n==0?"Yvsu":"Yvsl"  , m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "Q"     , m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "Iv"    , m_nSttRow, "X", "AA",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "s"     , m_nSttRow, "AB","AC",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��a"   , m_nSttRow, "AD","AF",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "�ĥ�"  , m_nSttRow, "AG","AJ",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "��sr"  , m_nSttRow, "AK","AN",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "���"  , m_nSttRow, "AO","AP",2, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow++;
		pOut->SetXL			(pXL, "(kN)"   , m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm��)"   , m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm��)"   , m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm��)"   , m_nSttRow, "X", "AA",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "AB","AC",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "AD","AF",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(MPa)", m_nSttRow, "AG","AJ",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(MPa)", m_nSttRow, "AK","AN",0, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow++;
		pOut->SetXL			(pXL, "DB����", m_nSttRow, "B","D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL����", m_nSttRow, "B","D", 1, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow-=2;
		///< ���----------------------------------------------------------------------------------
		DB = StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, nElmCheck, dRatio)
			 -StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, nElmCheck, dRatio);

		DL = StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, nElmCheck, dRatio)
			 -StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, nElmCheck, dRatio);

		pBx  = pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nNodeCheck));
		nG   = ModelCalc.GetGirderNoByJointID(nNodeCheck);
		CPlateGirderApp *pGir = pBridgeApp->GetGirder(nG);
		dSta = pBx->GetStation();

		StressCalc.GetSectionStress(nG, pBx->GetStation());
//		Yvsu = (StressCalc.GetMapValueSecStress("Yvsu"));
//		Yvsl = -(StressCalc.GetMapValueSecStress("Yvsl"));
		ModelCalc.GetSectionCoefficient(nG, dSta);
		Yvsu = (ModelCalc.GetMapValueSecCo("Yvsu"));
		Yvsl = -(ModelCalc.GetMapValueSecCo("Yvsl"));
		BOOL bFixPostiveMoment = FALSE;
		if(pBx->IsDanbuJijum())	// �Ŵ� �����ΰ� �θ��Ʈ�� �̼��ϰ� ���� ���� �����ϰ� �����Ʈ�� ó����.
			bFixPostiveMoment = TRUE;

		BOOL bPositiveM = StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);

		if(pBridgeApp->IsTUGir())
		{
			if(bPositiveM)	Iv = ModelCalc.GetMapValueSecCo("Ivd33");
			else			Iv = StressCalc.GetMapValueSecStress("Irv");
		}
		else
		{
			if(bPositiveM)	Iv = ModelCalc.GetMapValueSecCo("Iv33");
			else			Iv = StressCalc.GetMapValueSecStress("Irv");
		}

		double tu = pGir->GetThickFactByStation(dSta, G_F_U);
		double tl = pGir->GetThickFactByStation(dSta, G_F_L);
		double tw = pGir->GetThickFactByStation(dSta, G_W);
		double Bu = (pGir->GetWidthFlangeUpper(pBx));
		double Bl = (pGir->GetWidthFlangeLower(pBx));
		double su = max(RoundUp(sqrt(2 * max(tu, tw)), 0), 6.0);
		double sl = max(RoundUp(sqrt(2 * max(tl, tw)), 0), 6.0);
		///< ��S
		pOut->SetXL			(pXL, DB, m_nSttRow, "E", "H", 1, RED, 9, FALSE, TA_CENTER, "DELTA_S_DB", 3); 
		pOut->SetXL			(pXL, DL, m_nSttRow, "E", "H", 0, RED, 9, FALSE, TA_CENTER, "DELTA_S_DL", 3); 
		m_nSttRow--;
		///< t, B, A
		pOut->SetLineBoxText(pXL, n==0?tu:tl		, m_nSttRow, "I", "J", 2, RED,   9, FALSE, TA_CENTER, "FLANGE_T", 0, FALSE);
		pOut->SetLineBoxText(pXL, n==0?Bu:Bl		, m_nSttRow, "K", "M", 2, RED,   9, FALSE, TA_CENTER, "FLANGE_W", 1, FALSE);
		sText.Format("=%s*%s", pOut->GetCellRef("FLANGE_T"), pOut->GetCellRef("FLANGE_W"));
		///< Area
		pOut->SetLineBoxText(pXL, sText				, m_nSttRow, "N", "P", 2, BLACK, 9, FALSE, TA_CENTER, "AREA", 1, FALSE);
		///< Yvsu, Yvsl
		pOut->SetLineBoxText(pXL, n==0?Yvsu:Yvsl	, m_nSttRow, "Q", "S", 2, RED,   9, FALSE, TA_CENTER, "Yvs", 1, FALSE);
		///< Q(�ܸ�1�����Ʈ)
		sText.Format("=%s*(%s-%s/20)", pOut->GetCellRef("AREA"), pOut->GetCellRef("Yvs"), pOut->GetCellRef("FLANGE_T"));
		pOut->SetLineBoxText(pXL, sText				, m_nSttRow, "T", "W", 2, BLACK, 9, FALSE, TA_CENTER, "Q", 0, FALSE);
		///< Iv(�ռ� �� �ܸ�2�����Ʈ)
		pOut->SetLineBoxText(pXL, Iv				, m_nSttRow, "X","AA", 2, RED,   9, FALSE, TA_CENTER, "Iv", 0, FALSE);
		
		sText.Format("X%d", m_nSttRow);
		pXL->SetNumberFormat(sText, "0.000E+00");
		
		///<  s(�ʷ�����ġ��)
		pOut->SetLineBoxText(pXL, n==0?su:sl		, m_nSttRow,"AB","AC", 2, RED,   9, FALSE, TA_CENTER, "s", 0, FALSE);
		///< ��a
		sText.Format("=4*%s/SQRT(2)", pOut->GetCellRef("s"));
		pOut->SetLineBoxText(pXL, sText				, m_nSttRow, "AD","AF", 2, BLACK,   9, FALSE, TA_CENTER, "SUM_A", 2, FALSE);
		///< �ĥ�			
		sText.Format("=%s*1000*%s/(%s*%s)", pOut->GetCellRef("DELTA_S_DB"), pOut->GetCellRef("Q"), pOut->GetCellRef("Iv"),pOut->GetCellRef("SUM_A"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_DB", 2); 
		sText.Format("=%s*1000*%s/(%s*%s)", pOut->GetCellRef("DELTA_S_DL"), pOut->GetCellRef("Q"), pOut->GetCellRef("Iv"),pOut->GetCellRef("SUM_A"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AG", "AJ", 0, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_DL", 2); 
		m_nSttRow--;	
		///< ��sr
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O20,G20),IF(%s=\"2�鸸ȸ\",AF20,X20))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_R_DB", 2); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K20,G20),IF(%s=\"50��ȸ\",AB20,X20))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AK", "AN", 0, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_R_DL", 2); 
		m_nSttRow--;
		///< Ok���� ********************
		sText.Format("=IF(%s>%s,\"NG\",\"OK\")", pOut->GetCellRef("DELTA_SHEAR_DB"), pOut->GetCellRef("DELTA_SHEAR_R_DB"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s>%s,\"NG\",\"OK\")", pOut->GetCellRef("DELTA_SHEAR_DL"), pOut->GetCellRef("DELTA_SHEAR_R_DL"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	}
}

void CXLUsingCheck::CheckFatigueStressSub(long nChecksu)
{
	if(nChecksu<2) return;

	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString szGussetType = pBridgeApp->m_dRoundCrossLower < 50 ? "E" : "D";
	CString sText;	

	///< �ܸ�2	--------------------------------------------------------------
	if(nChecksu>=2)
	{
		pOut->SetXL(pXL, "�ܸ� 2", m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->InsertImage(pXL,"��뼺����_�ܸ�2", m_nSttRow, "G");
		m_nSttRow+=12;
		// ���׸���
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "X", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AA",2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AI",1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN",2);
		m_nSttRow++;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE",1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI",1);
		m_nSttRow++;
		for(long n=0; n<5; n++)
		{
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D",  1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "X",  1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AA", 1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE", 1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI", 1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN", 1);
			m_nSttRow++;
		}
		//Grid text
		m_nSttRow-=7;
		pOut->SetLineBoxText(pXL, "����"				, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "��          ��"		, m_nSttRow, "E", "X",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetXL			(pXL, "�� ��"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, " fsr ( MPa ) "	, m_nSttRow, "AB","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetLineBoxText(pXL, "��  ��"				, m_nSttRow, "AJ","AN", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		m_nSttRow++;
		pOut->SetXL			(pXL, "�� ��"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DB ����"				, m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DL ����"				, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		m_nSttRow++;
		pOut->SetLineBoxText(pXL, "��"					, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE); 
		pOut->SetLineBoxText(pXL, "�÷����� �������� ���� �ʷ� ������", m_nSttRow, "E", "X", 2, BLACK, 9, FALSE, TA_LEFT, "", 0, FALSE); 
		pOut->SetXL			(pXL, "B"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O13,G13),IF(%s=\"2�鸸ȸ\",AF13,X13))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K13,G13),IF(%s=\"50��ȸ\",AB13,X13))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1_DL", 0); 
		m_nSttRow++;	
		pOut->SetXL			(pXL, "F"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O20,G20),IF(%s=\"2�鸸ȸ\",AF20,X20))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1S_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K20,G20),IF(%s=\"50��ȸ\",AB20,X20))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1S_DL", 0); 
		pOut->SetXL			(pXL, "�����Ƿΰ���"		, m_nSttRow, "AJ", "AN", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		m_nSttRow++;	
		pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "�����κ����� ���� ���κ�", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O15,G15),IF(%s=\"2�鸸ȸ\",AF15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_2_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K15,G15),IF(%s=\"50��ȸ\",AB15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_2_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "���򺸰��� ���� ���κ�", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "E"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O18,G18),IF(%s=\"2�鸸ȸ\",AF18,X18))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_3_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K18,G18),IF(%s=\"50��ȸ\",AB18,X18))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_3_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "���͵忡 ������ ������ �÷�����", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O15,G15),IF(%s=\"2�鸸ȸ\",AF15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_4_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K15,G15),IF(%s=\"50��ȸ\",AB15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_4_DL", 0); 
		m_nSttRow+=2; 
	}
	if(nChecksu>=3)
	{
		///< �ܸ�3	--------------------------------------------------------------
		pOut->SetXL(pXL, "�ܸ� 3", m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->InsertImage(pXL,"��뼺����_�ܸ�1", m_nSttRow, "G");
		m_nSttRow+=12;
		// ���׸���
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "X", 2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AA",2);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AI",1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN",2);
		m_nSttRow++;
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE",1);
		pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI",1);
		m_nSttRow++;
		for(long n=0; n<4; n++)
		{
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "C", "D",  1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "E", "X",  1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "Y", "AA", 1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AB","AE", 1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AF","AI", 1);
			pOut->SetLineBoxText(pXL, "", m_nSttRow, "AJ","AN", 1);
			m_nSttRow++;
		}
		//Grid text
		m_nSttRow-=6;
		pOut->SetLineBoxText(pXL, "����"				, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "��          ��"		, m_nSttRow, "E", "X",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetXL			(pXL, "�� ��"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, " fsr ( MPa ) "	, m_nSttRow, "AB","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetLineBoxText(pXL, "��  ��"				, m_nSttRow, "AJ","AN", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		m_nSttRow++;
		pOut->SetXL			(pXL, "�� ��"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DB ����"				, m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DL ����"				, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "�÷����� �������� ���� �ʷ� ������", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "B"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O13,G13),IF(%s=\"2�鸸ȸ\",AF13,X13))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_1_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K13,G13),IF(%s=\"50��ȸ\",AB13,X13))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_1_DL", 0); 
		m_nSttRow++;	
		pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "������������ ���� ���κ�", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O15,G15),IF(%s=\"2�鸸ȸ\",AF15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_2_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K15,G15),IF(%s=\"50��ȸ\",AB15,X15))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_2_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "���򺸰��� ���� ���κ�", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "E"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O18,G18),IF(%s=\"2�鸸ȸ\",AF18,X18))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_3_DB", 0); 
		sText.Format("=IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K18,G18),IF(%s=\"50��ȸ\",AB18,X18))", pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_3_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "��"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "�����ǿ� ������ ���κ� ����� �ż��� ���� ���κ�", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, szGussetType			, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "�ż��� ����", 0); 
		sText.Format("=IF(%s=\"E\",IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O18,G18),IF(%s=\"2�鸸ȸ\",AF18,X18)),IF(%s=\"�����ϰ��\",IF(%s=\"2�鸸ȸ\",O17,G17),IF(%s=\"2�鸸ȸ\",AF17,X17)))",
		pOut->GetCellRef("�ż��� ����"), pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"),
		pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("Ʈ�����߼�"), pOut->GetCellRef("Ʈ�����߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_4_DB", 0);
		sText.Format("=IF(%s=\"E\",IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K18,G18),IF(%s=\"50��ȸ\",AB18,X18)),IF(%s=\"�����ϰ��\",IF(%s=\"50��ȸ\",K17,G17),IF(%s=\"50��ȸ\",AB17,X17)))",
		pOut->GetCellRef("�ż��� ����"), pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"),
		pOut->GetCellRef("���ϰ��"), pOut->GetCellRef("�������߼�"), pOut->GetCellRef("�������߼�"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_4_DL", 0);
		sText.Format("* �ż��� ���� 50 �̻��� ��� D ���� ����");
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AQ", "AQ", 0);
		m_nSttRow+=2; 
	}
}

void CXLUsingCheck::CheckDroop()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcDisplacement	*pCalcDisp		= m_pAPlateOutCalcStd->GetGeneralInputStd()->GetCalcSlabDisplacement();
	CFemModelingCalc    ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	m_nSttRow = 1;
	CString sMark, sText;
	pOut->ClearCellRef();
	pOut->SetXL(pXL, "'9.2 ó�� ����"						, m_nSttRow, "A", "A", 1, BLACK, 9);
	pOutExcel->SetProgress("9.2 ó�� ����", pOutExcel->GetProgress()+10);

	pOut->SetXL(pXL, "    ��������� ������ Ȱ���߿� ���� �� ��ġ�� �ִ� ó���� ( ���� OUTPUT ���� )  ( �� ��.��.�� 56)", m_nSttRow, "A", "A", 1);

	if(!pCalcData->m_bReadLiveDispOutput)
	{
		pOut->SetXL(pXL, "***** ó�������� ���� �ʾҽ��ϴ�.              *****", m_nSttRow, "A", "A", 1, BLACK, 9);			
		return;
	}
	
	char sBuf[40];
	int nRow = 3, nNodeColSu = 10;
	long nLoadCase = pManage->GetLoadCaseIndex("DESIGN MIN");

	// �Ŵ����� ���
	long nSttOutLineRow, nEndOutLineRow;
	for (long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		nSttOutLineRow = nRow + 2;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		long nIdxPrev, nIdxNext;

		ModelCalc.GetJointRangeByGirder(nG, nIdxPrev, nIdxNext);

		sprintf(sBuf, "< Girder %d >", nG + 1);
		pXL->SetXL(nRow, 0, sBuf);
		pXL->SetXL(nRow, nNodeColSu - 1, "( ���� : mm )");
		int nCol = 0;
		int nNode = 0;
		for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++)
		{
			if (nCol == 0 || nCol > nNodeColSu)   
			{   // �� �ѱ�.
				nRow += 2;
				pXL->SetHoriAlign(nRow, 0, nRow + 1, nNodeColSu, TA_CENTER);
				pXL->SetXL(nRow,     0, "����NO.");
				pXL->SetXL(nRow + 1, 0, "ó �� ��");
				nCol = 1;
			}
			pXL->SetXL(nRow, nCol, _ITOT_S(nNode + 1, sBuf, 10));   // ������ȣ
			pXL->SetNumberFormat(nRow, nCol, "0");

			CJoint *pJ = pManage->GetJoint(nNode);
			double dValue = -frM(pJ->GetResultForce(nLoadCase)->m_Disp.z);
			if(ModelCalc.IsRestraintJoint(nNode))	dValue = 0; 

			sprintf(sBuf, "'%.3f", dValue);
			pXL->SetXL(nRow + 1, nCol, sBuf);   // ó����
			nCol++;
		}
		nEndOutLineRow = nRow + 1;
		pXL->CellOutLineAll(nSttOutLineRow, 0, nEndOutLineRow, nNodeColSu);

		// �׸� �߰�.
		nRow += 4;
		CDomyun Dom;
		Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom.SetScaleDim(500.0);
		pCalcDisp->DisplacementOutput(&Dom, nG, nLoadCase);

		double dScale = min(480 / pDB->GetLengthBridge(), 0.008);
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		double dWidthh = rect.Width();
		double dHeight = rect.Height();
		double dPicRatio = dWidthh / dHeight;
		double dPicScale = dScale * 1.35;
		CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");

		pXL->AddDomWmfAuto(&Dom, szPath,10, nRow*22.5, dHeight*dPicScale*dPicRatio, dHeight*dPicScale);

		nRow += 4;

		pXL->SetXL(nRow++, 0, "�� ��� ó����");
		int nJigan = 0;
		double dValue, dMaxValue = 0.0;
		for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++) 
		{
			CJoint *pJ = pManage->GetJoint(nNode);
			dValue = -frM(pJ->GetResultForce(nLoadCase)->m_Disp.z);
			dMaxValue = max(dValue, dMaxValue);
			if(nNode != nIdxPrev && ModelCalc.IsRestraintJoint(nNode))
			{
				sprintf(sBuf, "��%d  =", nJigan + 1);
				pXL->SetXL(nRow, 1, sBuf);
				sprintf(sBuf, "%.3f mm", dMaxValue);
				pXL->SetXL(nRow, 2, sBuf);

				double dDa, dLength = toM(pDB->m_dLengthJigan[nJigan]);   // ���� �Ÿ�.
				char sBuf2[20];
				if(dLength > 40)
				{
					sprintf (sBuf,  "��a = L/500 =");
					sprintf (sBuf2, "%.1f m  /  500 =", dLength);
					dDa = dLength / 500;
				}
				else if (dLength > 10)
				{
					sprintf (sBuf,  "��a = L��/20000 =");
					sprintf (sBuf2, "%.1f�� m / 20000 =", dLength);
					dDa = dLength * dLength / 20000;
				}
				else
				{
					sprintf (sBuf,  "��a = L /2000 =");
					sprintf (sBuf2, "%.1f m / 2000 =", dLength);
					dDa = dLength / 2000;
				}
				dDa *= 1000;
				pXL->SetMergeCell(pXL->GetCellStr(nRow, 4), pXL->GetCellStr(nRow, 5));
				pXL->SetMergeCell(pXL->GetCellStr(nRow, 6), pXL->GetCellStr(nRow, 7));
				pXL->SetXL(nRow, 4, sBuf);
				pXL->SetXL(nRow, 6, sBuf2);

				sprintf(sBuf, "%.2f mm", dDa);
				pXL->SetXL(nRow, 8, sBuf);
				if (dDa >= dMaxValue)   {
					pXL->SetXL(nRow, 3, "<");
					pXL->SetXL(nRow, 10, "O.K");
				}
				else   {
					pXL->SetXL(nRow, 3, ">");
					pXL->SetXL(nRow, 10, "N.G");
					pXL->SetFonts(nRow, 10, 9, "����ü", 3, FALSE);
				}
				pXL->SetHoriAlign(nRow, 1, nRow, 10, TA_CENTER);
				pXL->SetHoriAlign(nRow, 4, nRow, 7, TA_LEFT);
				dMaxValue = 0.0;
				nJigan ++;
				nRow ++;
			}
		}
		nRow += 3;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//��š񢱢����� ���¥åĥťƥǥ�������ҥ��ɥ��������f ����������
//	���£��������¡áġҡ����� �� �� �� kgf/�� t��m  ����������������
//  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ������������¨èĨ�
//  ��������������������������  �� ��
//  �� �� �� ������ �� ���� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
///////////////////////////////////////////////////////////////////////////////////// 