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
	pOut->SetXL(pXL, "9. 사용성 검토"						, m_nSttRow, "A", "A", 1, BLACK, 11, TRUE);
	pOut->SetXL(pXL, "9.1 피로검토 (☞ 도.설.기 3.3.4.1)"   , m_nSttRow, "B", "B", 1, BLACK, 9);
	pOutExcel->SetProgress("10. 사용성 검토", pOutExcel->GetProgress()+10);

	pOut->SetXL(pXL, "가) 응력반복횟수"								, m_nSttRow, "B", "B", 1);
	///< 테이블 그리기
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
	pOut->SetXL			(pXL, "부재 및 하중구분"	, m_nSttRow, "D",  "J",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "종방향 주부재"		, m_nSttRow, "K",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "횡방향 부재"			, m_nSttRow, "Y",  "AE", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "비  고"				, m_nSttRow, "AF", "AN", 2, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow++;
	pOut->SetXL			(pXL, "도로의 종류"			, m_nSttRow, "C",  "G",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "트럭하중"			, m_nSttRow, "K",  "Q",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "차선하중"			, m_nSttRow, "R",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "트럭하중"			, m_nSttRow, "Y",  "AE", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			(pXL,"고속,국도,주간선 도로", m_nSttRow, "C",  "J",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "2 백만"				, m_nSttRow, "K",  "Q",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "50 만"				, m_nSttRow, "R",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "2 백만 이상"			, m_nSttRow, "Y",  "AE", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL			(pXL, "기타도로"			, m_nSttRow, "C",  "J",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "10 만"				, m_nSttRow, "K",  "Q",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "10 만"				, m_nSttRow, "R",  "X",  0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL			(pXL, "50 만"				, m_nSttRow, "Y",  "AE", 1, BLACK, 9, FALSE, TA_CENTER);

	m_nSttRow++;
	pOut->SetXL(pXL, "나) 허용응력 범위  fsr ( MPa )", m_nSttRow, "B", "B", 1);
	///< 테이블 그리기
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
	pOut->SetXL(pXL, "응 력"		  , m_nSttRow, "C", "F",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "다재하 경로구조", m_nSttRow, "G", "W",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "단재하 경로구조", m_nSttRow, "X", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "범 주"		  , m_nSttRow, "C", "F",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "10 만회"		  , m_nSttRow, "G", "J",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "50 만회"		  , m_nSttRow, "K", "N",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 만회"		  , m_nSttRow, "O", "R",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 만회 이상"  , m_nSttRow, "S", "W",  0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "10 만회"		  , m_nSttRow, "X", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "50 만회"		  , m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 만회"		  , m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "200 만회 이상"  , m_nSttRow, "AJ","AN", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

	char* szBaseStr[] = {
		"A",   "442", "260", "168", "168", "351", "203", "168", "168",  				
		"B",   "344", "203", "126", "112", "274", "161", "112", "112",
		"B´", "274", "161", "101", "84",   "218", "126", "77",   "77",
		"C",   "250", "147", "91",   "70",   "196", "112", "70",   "63",
		"",    ""     , ""     , ""   ,   "84*",	""     , ""   ,	  "84*",  "77*",
		"D",   "196", "112", "70",   "49",   "154", "91",   "56",   "35",
		"E",   "154", "91 " , "56",   "31",   "119", "70",   "42",   "16",
		"E´", "112", "64 " , "40",   "18",   "84",   "49",   "28",   "9",
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
	pOut->SetXL(pXL, "다) 피로조사위치 및 응력범주의 설정"														 	 , m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "활하중에 의한 모멘트의 변동범위가 큰 단면에서 인장응력 및 교번응력이 발생하는 상세부에 대하여 ", m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "피로검토를 수행 한다."																		 , m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "⊙ 피로조사 단면위치"																			 , m_nSttRow, "C", "C", 1);
	
	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(500);
	StressCalc.OutputOfFatigueCheckPos(&Dom, FALSE, 7);
	if(Dom.GetExtHeight()>0)	// 차도부가 없는경우 피로응력검토가 나오지 않는다 #25502
		pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 2, 15, TRUE, FALSE);
	else
		pOut->SetXL(pXL, "피로조사 단면이 존재하지 않습니다.", m_nSttRow, "D", "D", 1);
	m_nSttRow++;
//	pOut->InsertDomyun(pXL, &Dom, m_nSttRow, 0, 7, TRUE);
	
	CString sRoadType = "주간선도로";
	CString sStruType = "다재하경로";

	if(pCalcData->m_nFatigueRoadType == 1) sRoadType = "기타도로";
	if(pCalcData->m_nFatigueStructureType == 1) sStruType ="단재하경로";

	pOut->SetXL(pXL, "본 교량은"	, m_nSttRow, "C", "F", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, sRoadType		, m_nSttRow, "G", "J", 0, RED,   9, FALSE, TA_CENTER, "도로형태", 0);
	pOut->SetXL(pXL, "에 속하는"	, m_nSttRow, "K", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, sStruType		, m_nSttRow, "O", "R", 0, RED,   9, FALSE, TA_CENTER, "재하경로", 0);
	pOut->SetXL(pXL, "구조로서"		, m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "트럭하중"		, m_nSttRow, "W", "Z", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"주간선도로\",\"2백만회\",\"10만회\")", pOut->GetCellRef("도로형태"));
	pOut->SetXL(pXL, sText			, m_nSttRow, "AA","AC", 0, BLACK, 9, FALSE, TA_CENTER, "트럭하중수", 0); 
	pOut->SetXL(pXL, "와 차선하중"	, m_nSttRow, "AD","AH", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"주간선도로\",\"50만회\",\"10만회\"", pOut->GetCellRef("도로형태"));
	pOut->SetXL(pXL, sText			, m_nSttRow, "AI","AK", 1, BLACK, 9, FALSE, TA_CENTER, "차선하중수", 0); 
	pOut->SetXL(pXL, "에 대하여 검토 하는 것으로 한다.", m_nSttRow, "C","C", 1, BLACK, 9, FALSE, TA_LEFT);
	
	long nChecksu = min(pBridgeApp->m_nQtyJigan, 3);
	CPlateBasicIndex *pBx = NULL;
	///< 단면1	--------------------------------------------------------------
	pOut->SetXL(pXL, "단면 1", m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->InsertImage(pXL, "사용성검토_단면1", m_nSttRow, "G");
	m_nSttRow+=12;
	// 선그리기
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
	pOut->SetLineBoxText(pXL, "구분"				, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "상          세"		, m_nSttRow, "E", "X",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetXL			(pXL, "응 력"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, " fsr ( MPa ) "	, m_nSttRow, "AB","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetLineBoxText(pXL, "비  고"				, m_nSttRow, "AJ","AN", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	m_nSttRow++;
	pOut->SetXL			(pXL, "범 주"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "DB 하중"				, m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "DL 하중"				, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	m_nSttRow++;
	pOut->SetXL			(pXL, "①"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "플랜지와 복부판의 연속 필렛 용접부", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, "B"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O13,G13),IF(%s=\"2백만회\",AF13,X13))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_1_DB", 0); 
	sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K13,G13),IF(%s=\"50만회\",AB13,X13))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_1_DL", 0); 
	m_nSttRow++;	
	pOut->SetXL			(pXL, "②"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "수직보강재의 용접 끝부분", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O15,G15),IF(%s=\"2백만회\",AF15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_2_DB", 0); 
	sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K15,G15),IF(%s=\"50만회\",AB15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_2_DL", 0); 
	m_nSttRow++;
	pOut->SetXL			(pXL, "③"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "수평보강재 용접 끝부분", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, "E"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O18,G18),IF(%s=\"2백만회\",AF18,X18))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_3_DB", 0); 
	sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K18,G18),IF(%s=\"50만회\",AB18,X18))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_3_DL", 0); 
	m_nSttRow++;
	pOut->SetXL			(pXL, "④"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	pOut->SetXL			(pXL, "복부판에 부착된 가로보 연결용 거셋판 용접 끝부분", m_nSttRow, "E", "X", 0);
	pOut->SetXL			(pXL, szGussetType			, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "거셋판 응력", 0); 
	sText.Format("=IF(%s=\"E\",IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O18,G18),IF(%s=\"2백만회\",AF18,X18)),IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O17,G17),IF(%s=\"2백만회\",AF17,X17)))",
		pOut->GetCellRef("거셋판 응력"), pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"),
		pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_4_DB", 0); 
	sText.Format("=IF(%s=\"E\",IF(%s=\"다재하경로\",IF(%s=\"50만회\",K18,G18),IF(%s=\"50만회\",AB18,X18)),IF(%s=\"다재하경로\",IF(%s=\"50만회\",K17,G17),IF(%s=\"50만회\",AB17,X17)))",
		pOut->GetCellRef("거셋판 응력"), pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"),
		pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC1_4_DL", 0); 
	sText.Format("* 거셋판 라운드 50 이상일 경우 D 범주 적용");
	pOut->SetXL			(pXL, sText					, m_nSttRow, "AQ", "AQ", 0);
	m_nSttRow+=2; 

	CheckFatigueStressSub(nChecksu);
	
	pOut->SetXL(pXL, "라) 설계피로응력"											 , m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "⊙ 휨모멘트에 대한 피로검토"								 , m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "여기서, Δf: 설계피로응력범위 (최대응력과 최소응력의 차)"	 , m_nSttRow, "K", "K", 1);
	pOut->SetXL(pXL, "ΔM = Mmax - Mmin  (4.4 라.피로하중 작용범위 참조) "		 , m_nSttRow, "K", "K", 1);
	pOut->SetXL(pXL, "Iv : 합성후 단면의 단면2차 모멘트"						 , m_nSttRow, "K", "K", 1);
	pOut->SetXL(pXL, " y : 합성후 단면 도심으로부터의 거리"						 , m_nSttRow, "K", "K", 1);
	m_nSttRow-=2;
	pOut->SetLineBoxText  (pXL, "Δf =", m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "ΔM"  , m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
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
		nNodeCheck = ModelCalc.GetMaxFatigueIndex(n+1, nElmCheck, dRatio, 1);  ///< DB 하중 check
		if(nNodeCheck<=0)	continue;

		sText.Format("단면 %d", n+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "B", "B");
		//선그리기
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
		//그리드문자
		m_nSttRow-=2;
		pOut->SetLineBoxText(pXL, "구분"			 , m_nSttRow, "C", "D", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "ΔM ( kN m )"	 , m_nSttRow, "E", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "Iv (mm4)"		 , m_nSttRow, "M", "Q", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "y (mm)"			 , m_nSttRow, "R", "T", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "Δf ( MPa )"  , m_nSttRow, "U", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "응 력"			 , m_nSttRow, "AC","AE", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, " fsr ( MPa )" , m_nSttRow, "AF","AM", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비 고"			 , m_nSttRow, "AN","AP", 2, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow++;
		pOut->SetXL			(pXL, "DB 하중"			, m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL 하중"			, m_nSttRow, "I", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DB 하중"			, m_nSttRow, "U", "X", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL 하중"			, m_nSttRow, "Y", "AB",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DB 하중"			, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL 하중"			, m_nSttRow, "AJ","AM",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "범 주"			, m_nSttRow, "AC","AE",1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "①"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "②"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "③"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "④"				, m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER);
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
			Y1 = Yvsl+dThickFlDN;						///< 플랜지와 복부판의 연속 필렛 용접부
			Y2 = Yvsl+dThickFlDN+(MIN(pVStiff->m_dGap[0], pVStiff->m_dGap[1]));		///< 수직보강재의 용접 끝부분
			Y3 = Yvsu-dThickFlUP-dHeightWStiff;			///< 수평보강재 용접 끝부분
			Y4 = Yvsl+(pHGusset->m_ddH-pHGusset->m_dT);	///< 복부판에 부착된 가로보 연결용 거셋판 용접 끝부분	
		}
		else
		{
			Y1 = Yvsu-dThickFlUP;						///< 플랜지와 복부판의 연속 필렛 용접부
			Y2 = Yvsu-dThickFlUP;						///< 지점부보강재 용접 끝부분
			Y3 = Yvsu-dThickFlUP-dHeightWStiff;			///< 수평보강재 용접 끝부분
			Y4 = Yvsu;									///< 스터드에 인접한 인장측 플랜지부	
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
		m_nSttRow-=4;  ///< 응력범주
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
		m_nSttRow-=4;  ///< 비고
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
	pOut->SetXL(pXL, "⊙ 전단에 대한 피로검토 - 플랜지와 복부판의 필렛용접부에 대한 전단피로검토", m_nSttRow, "C","C", 1);
	pOut->SetXL(pXL, "이 부위의 피로상세범주는 F 이며, 검토 단면은 전단력이 가장 큰 절점"		 , m_nSttRow, "D","D", 0);

	nNodeCheck = ModelCalc.GetMaxFatigueIndex(4, nElmCheck, dRatio, 1);  ///< 최대전단력
	pOut->SetXL(pXL, nNodeCheck+1, m_nSttRow, "AC","AD", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "을 설정 하였음"															 , m_nSttRow, "AE","AE", 1);
	m_nSttRow++;
	pOut->SetXL(pXL, "여기서, Δυ:  설계전단피로응력범위 (최대응력과 최소응력의 차)"			 , m_nSttRow, "N","N", 1);
	pOut->SetXL(pXL, "ΔS = Smax - Smin   (4.4 라.피로하중 작용범위 참조) "						 , m_nSttRow, "Q","Q", 1);
	///< 그림삽임
	pOut->InsertImage(pXL, "용접부응력검토", m_nSttRow, "B");
	pOut->SetXL(pXL, "Q    : 용접선 외측 플랜지의 총단면의 중립축에 관한"						 , m_nSttRow, "Q","Q", 1);
	pOut->SetXL(pXL, "단면 1차 모멘트 (mm3)  =  A(mm²)·y(mm) "								 , m_nSttRow, "T","T", 1);
	pOut->SetXL(pXL, "y = Yvsu(l) - tu(l)/2 "													 , m_nSttRow, "T","T", 1);
	pOut->SetXL(pXL, "Iv  :   합성후 단면의 단면2차 모멘트"										 , m_nSttRow, "Q","Q", 1);
	pOut->SetXL(pXL, "Σa :   필렛 용접의 목두께의 합 (mm) = 4EA ×s / √ 2"					 , m_nSttRow, "Q","Q", 1);
	pOut->SetXL(pXL, "s   :   필렛 용접 치수 (mm)"												 , m_nSttRow, "Q","Q", 1);
	m_nSttRow-=8;
	pOut->SetLineBoxText  (pXL, "Δυ="		, m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "ΔS ·Q"	, m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	m_nSttRow++;
	pOut->SetXL			  (pXL, "Iv·Σa"   , m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
	m_nSttRow+=8;

	for(n=0; n<2; n++)
	{
		if(nNodeCheck < 0)  continue;

		if(n==0)	pOut->SetXL(pXL, "⊙ 상부 플랜지와 복부판"		, m_nSttRow, "C","C", 1);
		if(n==1)	pOut->SetXL(pXL, "⊙ 하부 플랜지와 복부판"		, m_nSttRow, "C","C", 1);
		//선그리기
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
		// 그리드 글자
		m_nSttRow-=3;
		pOut->SetLineBoxText(pXL, "구  분", m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "ΔS"   , m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "tu"    , m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "B"     , m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "A"     , m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, n==0?"Yvsu":"Yvsl"  , m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "Q"     , m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "Iv"    , m_nSttRow, "X", "AA",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "s"     , m_nSttRow, "AB","AC",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "Σa"   , m_nSttRow, "AD","AF",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "Δυ"  , m_nSttRow, "AG","AJ",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "υsr"  , m_nSttRow, "AK","AN",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비고"  , m_nSttRow, "AO","AP",2, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow++;
		pOut->SetXL			(pXL, "(kN)"   , m_nSttRow, "E", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm²)"   , m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm³)"   , m_nSttRow, "T", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm⁴)"   , m_nSttRow, "X", "AA",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "AB","AC",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(mm)"     , m_nSttRow, "AD","AF",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(MPa)", m_nSttRow, "AG","AJ",0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "(MPa)", m_nSttRow, "AK","AN",0, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow++;
		pOut->SetXL			(pXL, "DB하중", m_nSttRow, "B","D", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL			(pXL, "DL하중", m_nSttRow, "B","D", 1, BLACK, 9, FALSE, TA_CENTER);
		m_nSttRow-=2;
		///< 계산----------------------------------------------------------------------------------
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
		if(pBx->IsDanbuJijum())	// 거더 시점부가 부모멘트가 미세하게 생길 경우는 무시하고 정모멘트로 처리함.
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
		///< ΔS
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
		///< Q(단면1차모멘트)
		sText.Format("=%s*(%s-%s/20)", pOut->GetCellRef("AREA"), pOut->GetCellRef("Yvs"), pOut->GetCellRef("FLANGE_T"));
		pOut->SetLineBoxText(pXL, sText				, m_nSttRow, "T", "W", 2, BLACK, 9, FALSE, TA_CENTER, "Q", 0, FALSE);
		///< Iv(합성 후 단면2차모멘트)
		pOut->SetLineBoxText(pXL, Iv				, m_nSttRow, "X","AA", 2, RED,   9, FALSE, TA_CENTER, "Iv", 0, FALSE);
		
		sText.Format("X%d", m_nSttRow);
		pXL->SetNumberFormat(sText, "0.000E+00");
		
		///<  s(필렛용접치수)
		pOut->SetLineBoxText(pXL, n==0?su:sl		, m_nSttRow,"AB","AC", 2, RED,   9, FALSE, TA_CENTER, "s", 0, FALSE);
		///< Σa
		sText.Format("=4*%s/SQRT(2)", pOut->GetCellRef("s"));
		pOut->SetLineBoxText(pXL, sText				, m_nSttRow, "AD","AF", 2, BLACK,   9, FALSE, TA_CENTER, "SUM_A", 2, FALSE);
		///< Δυ			
		sText.Format("=%s*1000*%s/(%s*%s)", pOut->GetCellRef("DELTA_S_DB"), pOut->GetCellRef("Q"), pOut->GetCellRef("Iv"),pOut->GetCellRef("SUM_A"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_DB", 2); 
		sText.Format("=%s*1000*%s/(%s*%s)", pOut->GetCellRef("DELTA_S_DL"), pOut->GetCellRef("Q"), pOut->GetCellRef("Iv"),pOut->GetCellRef("SUM_A"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AG", "AJ", 0, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_DL", 2); 
		m_nSttRow--;	
		///< υsr
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O20,G20),IF(%s=\"2백만회\",AF20,X20))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_R_DB", 2); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K20,G20),IF(%s=\"50만회\",AB20,X20))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText				, m_nSttRow, "AK", "AN", 0, BLACK, 9, FALSE, TA_CENTER, "DELTA_SHEAR_R_DL", 2); 
		m_nSttRow--;
		///< Ok판정 ********************
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

	///< 단면2	--------------------------------------------------------------
	if(nChecksu>=2)
	{
		pOut->SetXL(pXL, "단면 2", m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->InsertImage(pXL,"사용성검토_단면2", m_nSttRow, "G");
		m_nSttRow+=12;
		// 선그리기
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
		pOut->SetLineBoxText(pXL, "구분"				, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "상          세"		, m_nSttRow, "E", "X",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetXL			(pXL, "응 력"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, " fsr ( MPa ) "	, m_nSttRow, "AB","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetLineBoxText(pXL, "비  고"				, m_nSttRow, "AJ","AN", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		m_nSttRow++;
		pOut->SetXL			(pXL, "범 주"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DB 하중"				, m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DL 하중"				, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		m_nSttRow++;
		pOut->SetLineBoxText(pXL, "①"					, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE); 
		pOut->SetLineBoxText(pXL, "플랜지와 복부판의 연속 필렛 용접부", m_nSttRow, "E", "X", 2, BLACK, 9, FALSE, TA_LEFT, "", 0, FALSE); 
		pOut->SetXL			(pXL, "B"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O13,G13),IF(%s=\"2백만회\",AF13,X13))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K13,G13),IF(%s=\"50만회\",AB13,X13))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1_DL", 0); 
		m_nSttRow++;	
		pOut->SetXL			(pXL, "F"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O20,G20),IF(%s=\"2백만회\",AF20,X20))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1S_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K20,G20),IF(%s=\"50만회\",AB20,X20))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_1S_DL", 0); 
		pOut->SetXL			(pXL, "전단피로검토"		, m_nSttRow, "AJ", "AN", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		m_nSttRow++;	
		pOut->SetXL			(pXL, "②"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "지점부보강재 용접 끝부분", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O15,G15),IF(%s=\"2백만회\",AF15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_2_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K15,G15),IF(%s=\"50만회\",AB15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_2_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "③"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "수평보강재 용접 끝부분", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "E"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O18,G18),IF(%s=\"2백만회\",AF18,X18))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_3_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K18,G18),IF(%s=\"50만회\",AB18,X18))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_3_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "④"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "스터드에 인접한 인장측 플랜지부", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O15,G15),IF(%s=\"2백만회\",AF15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_4_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K15,G15),IF(%s=\"50만회\",AB15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC2_4_DL", 0); 
		m_nSttRow+=2; 
	}
	if(nChecksu>=3)
	{
		///< 단면3	--------------------------------------------------------------
		pOut->SetXL(pXL, "단면 3", m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->InsertImage(pXL,"사용성검토_단면1", m_nSttRow, "G");
		m_nSttRow+=12;
		// 선그리기
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
		pOut->SetLineBoxText(pXL, "구분"				, m_nSttRow, "C", "D",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "상          세"		, m_nSttRow, "E", "X",  2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetXL			(pXL, "응 력"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, " fsr ( MPa ) "	, m_nSttRow, "AB","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetLineBoxText(pXL, "비  고"				, m_nSttRow, "AJ","AN", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		m_nSttRow++;
		pOut->SetXL			(pXL, "범 주"				, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DB 하중"				, m_nSttRow, "AB","AE", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "DL 하중"				, m_nSttRow, "AF","AI", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "①"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "플랜지와 복부판의 연속 필렛 용접부", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "B"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O13,G13),IF(%s=\"2백만회\",AF13,X13))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_1_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K13,G13),IF(%s=\"50만회\",AB13,X13))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_1_DL", 0); 
		m_nSttRow++;	
		pOut->SetXL			(pXL, "②"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "수직보강재의 용접 끝부분", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "C"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O15,G15),IF(%s=\"2백만회\",AF15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_2_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K15,G15),IF(%s=\"50만회\",AB15,X15))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_2_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "③"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "수평보강재 용접 끝부분", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, "E"					, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O18,G18),IF(%s=\"2백만회\",AF18,X18))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_3_DB", 0); 
		sText.Format("=IF(%s=\"다재하경로\",IF(%s=\"50만회\",K18,G18),IF(%s=\"50만회\",AB18,X18))", pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_3_DL", 0); 
		m_nSttRow++;
		pOut->SetXL			(pXL, "④"					, m_nSttRow, "C", "D",  0, BLACK, 9, FALSE, TA_CENTER, "", 0); 
		pOut->SetXL			(pXL, "복부판에 부착된 가로보 연결용 거셋판 용접 끝부분", m_nSttRow, "E", "X", 0);
		pOut->SetXL			(pXL, szGussetType			, m_nSttRow, "Y", "AA", 0, BLACK, 9, FALSE, TA_CENTER, "거셋판 응력", 0); 
		sText.Format("=IF(%s=\"E\",IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O18,G18),IF(%s=\"2백만회\",AF18,X18)),IF(%s=\"다재하경로\",IF(%s=\"2백만회\",O17,G17),IF(%s=\"2백만회\",AF17,X17)))",
		pOut->GetCellRef("거셋판 응력"), pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"),
		pOut->GetCellRef("재하경로"), pOut->GetCellRef("트럭하중수"), pOut->GetCellRef("트럭하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AB", "AE", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_4_DB", 0);
		sText.Format("=IF(%s=\"E\",IF(%s=\"다재하경로\",IF(%s=\"50만회\",K18,G18),IF(%s=\"50만회\",AB18,X18)),IF(%s=\"다재하경로\",IF(%s=\"50만회\",K17,G17),IF(%s=\"50만회\",AB17,X17)))",
		pOut->GetCellRef("거셋판 응력"), pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"),
		pOut->GetCellRef("재하경로"), pOut->GetCellRef("차선하중수"), pOut->GetCellRef("차선하중수"));
		pOut->SetXL			(pXL, sText					, m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_CENTER, "SEC3_4_DL", 0);
		sText.Format("* 거셋판 라운드 50 이상일 경우 D 범주 적용");
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
	pOut->SetXL(pXL, "'9.2 처짐 검토"						, m_nSttRow, "A", "A", 1, BLACK, 9);
	pOutExcel->SetProgress("9.2 처짐 검토", pOutExcel->GetProgress()+10);

	pOut->SetXL(pXL, "    충격하중을 포함한 활하중에 의한 각 위치별 최대 처짐량 ( 전산 OUTPUT 참조 )  ( ☞ 도.설.기 56)", m_nSttRow, "A", "A", 1);

	if(!pCalcData->m_bReadLiveDispOutput)
	{
		pOut->SetXL(pXL, "***** 처짐파일을 읽지 않았습니다.              *****", m_nSttRow, "A", "A", 1, BLACK, 9);			
		return;
	}
	
	char sBuf[40];
	int nRow = 3, nNodeColSu = 10;
	long nLoadCase = pManage->GetLoadCaseIndex("DESIGN MIN");

	// 거더별로 출력
	long nSttOutLineRow, nEndOutLineRow;
	for (long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		nSttOutLineRow = nRow + 2;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		long nIdxPrev, nIdxNext;

		ModelCalc.GetJointRangeByGirder(nG, nIdxPrev, nIdxNext);

		sprintf(sBuf, "< Girder %d >", nG + 1);
		pXL->SetXL(nRow, 0, sBuf);
		pXL->SetXL(nRow, nNodeColSu - 1, "( 단위 : mm )");
		int nCol = 0;
		int nNode = 0;
		for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++)
		{
			if (nCol == 0 || nCol > nNodeColSu)   
			{   // 줄 넘김.
				nRow += 2;
				pXL->SetHoriAlign(nRow, 0, nRow + 1, nNodeColSu, TA_CENTER);
				pXL->SetXL(nRow,     0, "절점NO.");
				pXL->SetXL(nRow + 1, 0, "처 짐 량");
				nCol = 1;
			}
			pXL->SetXL(nRow, nCol, _ITOT_S(nNode + 1, sBuf, 10));   // 절점번호
			pXL->SetNumberFormat(nRow, nCol, "0");

			CJoint *pJ = pManage->GetJoint(nNode);
			double dValue = -frM(pJ->GetResultForce(nLoadCase)->m_Disp.z);
			if(ModelCalc.IsRestraintJoint(nNode))	dValue = 0; 

			sprintf(sBuf, "'%.3f", dValue);
			pXL->SetXL(nRow + 1, nCol, sBuf);   // 처짐량
			nCol++;
		}
		nEndOutLineRow = nRow + 1;
		pXL->CellOutLineAll(nSttOutLineRow, 0, nEndOutLineRow, nNodeColSu);

		// 그림 추가.
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

		pXL->SetXL(nRow++, 0, "◈ 허용 처짐량");
		int nJigan = 0;
		double dValue, dMaxValue = 0.0;
		for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++) 
		{
			CJoint *pJ = pManage->GetJoint(nNode);
			dValue = -frM(pJ->GetResultForce(nLoadCase)->m_Disp.z);
			dMaxValue = max(dValue, dMaxValue);
			if(nNode != nIdxPrev && ModelCalc.IsRestraintJoint(nNode))
			{
				sprintf(sBuf, "δ%d  =", nJigan + 1);
				pXL->SetXL(nRow, 1, sBuf);
				sprintf(sBuf, "%.3f mm", dMaxValue);
				pXL->SetXL(nRow, 2, sBuf);

				double dDa, dLength = toM(pDB->m_dLengthJigan[nJigan]);   // 지간 거리.
				char sBuf2[20];
				if(dLength > 40)
				{
					sprintf (sBuf,  "δa = L/500 =");
					sprintf (sBuf2, "%.1f m  /  500 =", dLength);
					dDa = dLength / 500;
				}
				else if (dLength > 10)
				{
					sprintf (sBuf,  "δa = L²/20000 =");
					sprintf (sBuf2, "%.1f² m / 20000 =", dLength);
					dDa = dLength * dLength / 20000;
				}
				else
				{
					sprintf (sBuf,  "δa = L /2000 =");
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
					pXL->SetFonts(nRow, 10, 9, "굴림체", 3, FALSE);
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
//∫∴∵∮∑∏ㄷ ΑΒΓΔΕΖΗαβγδεζηΣθιΙκλμνξοπρf υφχψω
//	´≤＞＜≠±≤≥∞⌒×㎝⁴ ㎤ ㎠ ㎝ kgf/㎠ t·m  ¹²³⁴₁₂₃₄
//  ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩ ⑪ ⑫ ⑬ ⑭ ⑮ √½⅓㉮㉯㉰㉱㉲㉳㉴
//  ━━━━━━━━━━━━━  ⇒ ⊙
//  ┏ ┗ ┛ ┓┃━ ┣ ┫ⅰ ⅱ ⅲ ⅳ ⅴ ⅵ ⅶ ⅷ ⅸ ⅹ Ⅰ Ⅱ Ⅲ Ⅳ Ⅳ Ⅴ Ⅵ
///////////////////////////////////////////////////////////////////////////////////// 