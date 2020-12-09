// XLEtcCheck.cpp: implementation of the CXLEtcCheck class.
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

CXLEtcCheck::CXLEtcCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
}

CXLEtcCheck::~CXLEtcCheck()
{

}

void CXLEtcCheck::CheckCamber()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CFemModelingCalc    ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFEMManage			*pManageTu		= pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);
	CPlateGirderApp     *pGir           = NULL;
	long nRow		= 1;
	long nCurGir	= -1;
	long nCamberIdx	= 0;
	long nUnit		= pDB->m_cCamberUnitToMM;
	CString sText;
	BOOL bHapSung = pDB->IsHapsung();
	BOOL bFirst		= TRUE;

	if(!bHapSung)
	{
		pOut->SetXL(pXL, "1차 고정하중", nRow, "B", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "2차",			 nRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	}
	if(!pDB->IsTUGir())
	{
		nRow = 2;
		pOut->SetXL(pXL, "Steel",		 nRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Concrete",	 nRow, "C", "C", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=B3+C3+F3");
		pOut->SetXL(pXL, sText,			 nRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	}

	nRow = 4;
	pOut->ClearCellRef();
	pOut->SetXL(pXL, "10. 솟음 (CAMBER)", nRow, "A", "A", 1, BLACK, 11, TRUE);
	pOutExcel->SetProgress("10. 솟음 (CAMBER)", pOutExcel->GetProgress()+10);

	// 결과 Data Line 출력
	long nG   = 0;
	CJoint *pJ = NULL;
	for (int i = 0; i < pManage->GetJointSize(); i++)   
	{
		pJ = pManage->GetJoint(i);
		//if(pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE)	continue;

		nG = ModelCalc.GetGirderNoByJointID(i);
		
		if (nG != nCurGir)   
		{  // Girder가 바뀌면
			// Header 출력
			nCurGir = nG;
			pGir = pDB->GetGirder(nCurGir);
			nCamberIdx = 0;
			nRow++;
			
			sText.Format(" < Girder %d >", nCurGir + 1);
			pOut->SetXL(pXL, sText, nRow, "A", "A");
			sText = nUnit ? _T("( 단위 : mm )") : _T("( 단위 : cm )");
			pOut->SetXL(pXL, sText, nRow, "I", "I");

			pXL->InsertCopyRowLine(1, 3, nRow);
			nRow += 2;
			bFirst = TRUE;
		}

		double dSta = pJ->m_vPointReal.x;
		nCamberIdx = pGir->GetCamberNodeByStation(dSta);
		CCamber	*pCamber	= pGir->GetCamberByNode(nCamberIdx);

		double dJong		= pCamber->GetCamberByType(pDB->GetBridgeType(), 0);	// 종곡선 보정량
		double dSlop		= pCamber->GetCamberByType(pDB->GetBridgeType(), 1);	// 편경사 보정량
		double dPreSteel	= pCamber->GetCamberByType(pDB->GetBridgeType(), 2);
		double dPreConf		= pCamber->GetCamberByType(pDB->GetBridgeType(), 3);	//구속콘자중
		double dPreTurn		= 0;
		double dPreSlab		= 0;													//가로보 + 바닥판
		double dPos			= pCamber->GetCamberByType(pDB->GetBridgeType(), 4);
		long   nJijum		= ModelCalc.GetJijumNoOfJoint(i);
		
		if(pDB->IsTUGir())
		{
			dPreTurn = pCamber->GetCamberByType(pDB->GetBridgeType(), 4);
			dPreSlab = pCamber->GetCamberByType(pDB->GetBridgeType(), 5);
			dPos	 = pCamber->GetCamberByType(pDB->GetBridgeType(), 6);
		}		

		//TU거더이고 연속교일 때 구속콘자중과 가로보+바닥판을 처짐값으로 변경한다.
		if(pDB->IsTUGir() && pDB->m_nQtyJigan > 1)
		{
			long nCaseSteelIdx	 = pManage->GetLoadCaseIndex("CONSTEEL");
			long nCaseSteelTUIdx = pManageTu->GetLoadCaseIndex("CONSTEEL");
			long nCaseConcTUIdx	 = pManageTu->GetLoadCaseIndex("CONCRETE");

			double dConSteel	= frM(StressCalc.GetOneNodForceSub(pManage,		NOD_DISPLACE,	nCaseSteelIdx,		i, 'Z'));
			double dConSteelTU	= frM(StressCalc.GetOneNodForceSub(pManageTu,	NOD_DISPLACE,	nCaseSteelTUIdx,	i, 'Z'));
			double dConcTU		= frM(StressCalc.GetOneNodForceSub(pManageTu,	NOD_DISPLACE,	nCaseConcTUIdx,		i, 'Z'));

			dPreConf = dConSteel;
			dPreSlab = fabs(dConSteelTU + dConcTU);
		}

		if(!bFirst)
			pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);

		if(nUnit != 1)
		{
			dJong		= toCm(dJong);
			dSlop		= toCm(dSlop);
			dPreSteel	= toCm(dPreSteel);
			dPreConf	= toCm(dPreConf);
			dPreTurn	= toCm(dPreTurn);
			dPreSlab	= toCm(dPreSlab);	
			dPos		= toCm(dPos);
		}

		///< 절점번호
		sText.Format("%d", i+1);
		pOut->SetXL(pXL, sText,		nRow, "A", "A", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dPreSteel,	nRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER);
		
		if(pDB->IsTUGir() && pDB->m_nQtyJigan > 1)
		{
			pOut->SetXL(pXL, dPreConf,	nRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dPreSlab,	nRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		else
		{
			pOut->SetXL(pXL, dPreConf,	nRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dPreSlab,	nRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		}

		pOut->SetXL(pXL, dPreTurn,	nRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dPos,		nRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dJong,		nRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dSlop,		nRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);

		if(ModelCalc.IsRestraintJoint(i)) 	
			pOut->SetXL(pXL, pDB->m_strJijumName[nJijum], nRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);
		else
			pOut->SetXL(pXL, "", nRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);

		bFirst = FALSE;
		pJ = NULL;
	}
	
	if(!pDB->IsTUGir())
	{
		pXL->DeleteColSell(1, 4, nRow, 5);

		sText.Format("H%d", nRow);
		pXL->SetPrintArea("A1", sText);
	}
	pXL->SetPageSetupCenterHori(TRUE);
	pXL->DeleteRowLine(1, 3);
	pXL->DeleteRowLineEnd();
}

void CXLEtcCheck::CheckExpasionJoint(long nMethod)
{
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl			*pXL			= m_pXL;
	CDesignExpansionJoint	*pDesignExp	= pCalcStd->m_pDesignExpanJoint;

	long nSttRow	= 1;
	CString sCell	= _T("");
	CStringArray	Arr;

	if(nMethod==ACCURACY_METHOD)
	{
		for(long nExp=0; nExp<pDesignExp->GetSize(); nExp++)
		{
			pDesignExp->SetExpansionJoint(nExp);
			pDesignExp->GetTextExpansionJoint(nExp, ACCURACY_METHOD, Arr);
			Arr.Add(_T(""));
		}

		for(long nRow=0; nRow<Arr.GetSize(); nRow++)
		{	
			sCell.Format("A%d", nRow+1);
			pXL->SetXL(sCell, Arr.GetAt(nRow));
		}
	}

	if(nMethod==SIMPLE_METHOD)
	{
		nSttRow = 2;
		CShRichEdit EditTemp;
		for(long nExp=0; nExp<pDesignExp->GetSize(); ++nExp)
		{
			pDesignExp->GetTextSimpleExpansionJoint(nExp, nSttRow, EditTemp, pXL, TRUE);
			nSttRow += 2;
		}

		pXL->InsertRowLine(1);
		pXL->SetXL("A1", " 11. 신축이음(간략식)");
		pXL->SetFonts("A1", 11, _T("굴림체"));
	}
}
