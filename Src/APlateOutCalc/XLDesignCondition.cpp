// XLDesignCondition.cpp: implementation of the CXLDesignCondition class.
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

CXLDesignCondition::CXLDesignCondition(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;

}

CXLDesignCondition::~CXLDesignCondition()
{
	
}

//<summary>
// 1. 설 계 기 준
//</summary>
//<param name="CXLDesignCondition::DesignCommon("></param>
void CXLDesignCondition::DesignCommon()
{
	CAPlateOutCalcStd	*pStd			= m_pAPlateOutCalcStd;
	CDataManage			*pDataManage	= pStd->GetDataManage();
	CPlateBridgeApp		*pBridge		= pDataManage->GetBridge();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pBridge->GetGirder(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CSteelSection		*pSteelSection	= pDataManage->GetSteelSection();
	CSteelPrestressPlate		*pPrestress		= pDataManage->GetSteelPrestress();
	long 				nIdx 			= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData	*pPSData		= pPrestress->GetSteelPrestressData(nIdx);

	CString				sText;
	CString				sBridgeType		= pCalcData->DESIGN_BASE.m_szBridgeType;
	CString				sGradeBridge	= pBridge->m_sDeungeub;
	CString				sBridgeLength;
	double				dBridgeWidth	= pCalcData->DESIGN_BASE.m_dBridgeWidth;
	double				dSpeed			= pCalcData->m_dSpeed;
	long				nQtyGirder		= pBridge->GetQtyGirder();

	CLineInfoApp	*pLineApp	=	pDataManage->GetLineInfoApp();
	CLineInfo		*pLine		=	pLineApp->GetLine(0);
	CLineSplit		*pSplit		=	NULL;
	double			dBC			=	0.0;
	double			dEC			=	0.0;	
	double			dRadius		=	0.0;	
	long			nRow		=	-1;
	int				nIPs		=	pLine->GetQtyIp();
	CDoubleArray	dArrRadius;
	
	//곡률반경 찾기
	//Comment-1 :	BC~EC				사이에 BridgeStt가 존재할 때
	//Comment-2 :	BC~EC				사이에 BridgeEnd가 존재할 때
	//Comment-3 :	BridgeStt~BridgeEnd	사이에 BC~EC	가 존재할 때

	for(long nIp = 0; nIp <nIPs; nIp++)	
	{
		pSplit = pLine->GetLineSplitByIp(nIp);
		
		if(pSplit->m_dR > 0)
		{				
			pLine->GetStationMatchByXyAndAngle(pSplit->m_xyBc,	CDPoint(0,0),	dBC);
			pLine->GetStationMatchByXyAndAngle(pSplit->m_xyEc,	CDPoint(0,0),	dEC);
			
			nRow++;

			if(dBC <= 0 || dEC <=0 )	continue;

			if((pBridge->GetStationBridgeStt() >= dBC && pBridge->GetStationBridgeStt() <= dEC)	||	//Comment-1
			   (pBridge->GetStationBridgeEnd() >= dBC && pBridge->GetStationBridgeEnd() <= dEC)	||	//Comment-2
			   (pBridge->GetStationBridgeStt() <= dBC && pBridge->GetStationBridgeEnd() >= dEC))	//Comment-3
			{
				CIPRecord* pIpRec = &pLine->m_pIpRec[nRow];

				dArrRadius.Add(toM(pIpRec->m_dR_IpRecord));
			}	
		}
	}
	
	if(dArrRadius.GetSize() >0)
		dRadius = dArrRadius[dArrRadius.MinIdx()];

	m_nSttRow = 3;
		
	if(pBridge->IsPlateGir())
		sText.Format("STEEL PLATE GIRDER(판형교)");
	else if(pBridge->IsSosuGir())
		sText.Format("%d주형 판형교(소수주거더교)", nQtyGirder);
	else if(pBridge->IsTUGir())
		sText.Format("%d주형 판형교(Turnover거더교)", nQtyGirder);
	
	pOut->SetXL(pXL, sText,			m_nSttRow, "H", "H", NEXT,	RED);
	pOut->SetXL(pXL, sGradeBridge,	m_nSttRow, "H",	"H", NEXT,	RED);

	//연장 구하기	
	long	nJigan		=	pBridge->m_nQtyJigan;
	long	nCount		=	1;
	CString	strTotal	=	_T("");
	double	dTotal		=	0.0;

	for(long nJ	= 0; nJ < nJigan; nJ++)
	{
		if(pBridge->m_dLengthJigan[nJ] == pBridge->m_dLengthJigan[nJ+1])
			++nCount;
		else
		{
			if(nCount ==1)
				sText.Format("%g",toM(pBridge->m_dLengthJigan[nJ]));
			else
				sText.Format("%d@%g", nCount, toM(pBridge->m_dLengthJigan[nJ]));

			if(nJ < nJigan-1)
				sText += " + ";

			strTotal += sText;
			nCount = 1;
		}
		dTotal += 	pBridge->m_dLengthJigan[nJ];
	}
	sText.Format(" = %g m", toM(dTotal));
	strTotal += sText;

	double dAngle	= pBridge->m_vAngJijum[0].GetAngleDegree();

	pOut->SetXL(pXL,	strTotal,		m_nSttRow,	"H",	"H",	NEXT,	RED);
	pOut->SetXL(pXL,	dBridgeWidth,	m_nSttRow,	"H",	"J",	NEXT,	RED,	9, FALSE, TA_CENTER);
	pOut->SetXL(pXL,	dAngle,			m_nSttRow,	"H",	"J",	NEXT,	RED,	9, FALSE,	TA_CENTER);
	pOut->SetXL(pXL,	dRadius,		m_nSttRow,	"H",	"J",	NEXT,	RED,	9, FALSE, TA_CENTER);
	pOut->SetXL(pXL,	dSpeed,			m_nSttRow,	"H",	"J",	4,		RED,	9, FALSE, TA_CENTER);

	//사용재료-주부재
	sText = pSteelSection->GetMaterialByMarkBuje(G_F_U) + " ( 상하 플랜지, 복부판, 종방향 보강재, 수평 보강재, ";
	pOut->SetXL(pXL,	sText,	m_nSttRow,	"M",	"AC",	NEXT,	RED);
	
	sText = "            지점 보강재, 주부재 이음판, 소울 플레이트 )";
	pOut->SetXL(pXL,	sText,	m_nSttRow,	"M",	"AC",	NEXT,	RED);

	//사용재료-부부재
	sText = pSteelSection->GetMaterialByMarkBuje(CP_W) + " ( 가로보, 수직 브레이싱, 수평 브레이싱, 수직 보강재 )";
	pOut->SetXL(pXL,	sText,	m_nSttRow,	"M",	"AC",	NEXT,	RED);

	//강재탄성계수
	pOut->SetXL(pXL, pCalcData->DESIGN_MATERIAL.m_dEst,	m_nSttRow,	"R",	"U", NEXT,	RED,	9,	FALSE,	TA_CENTER, "", 1);

	m_nSttRow += 112;
	
	//[바닥판 콘크리트]
	CString szType = _T("");
	double	dVal   = 0.0;

	// 콘크리트 설계 기준강도
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dSigmaCK,	m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "CONC_SIGMACK", 1);
	// 콘크리트 탄성계수
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dEc,		m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);

	if(pBridge->m_bTendonInputComplete)
	{
		////프리스트레스 도입시 콘크리트 기준강도
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dSigmaCi,	m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
		
		////프리스트레스 도입시 콘크리트 탄성계수
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dEci,		m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);

		//허용응력		
		////프리스트레스 도입 직후(크리프, 건조수축 손실전)
		double dFci	= pCalcData->DESIGN_MATERIAL.m_dSigmaCi;

		//////압축응력
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow,	m_nSttRow,	"R",	"U",	CURRENT, RED, 9, FALSE, TA_LEFT, "", 1);
		double dConstPreStressComp = pCalcData->GetConstPreStressCompress();

		dVal = dConstPreStressComp * dFci;
		pOut->SetXL(pXL, dVal,	m_nSttRow, "W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT, "", 1);
		
		//////인장응력
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow,	m_nSttRow,	"R",	"U",	CURRENT, RED, 9, FALSE, TA_LEFT, "", 1);
		double dPreStressStretch = pCalcData->GetConstPreStressStretch();

		dVal	= dPreStressStretch == 1.4 ? 1.4 : dPreStressStretch * sqrt(dFci);
		pOut->SetXL(pXL, dVal,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT, "", 1);
		
		////설계하중 작용시 (손실 후)
		double dFck = pCalcData->DESIGN_MATERIAL.m_dSigmaCK;

		//////압축응력
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow,	m_nSttRow,	"R",	"U",	CURRENT, RED, 9, FALSE, TA_LEFT, "", 1);
		double dDesignLoadCompress = pCalcData->GetConstDesignLoadCompress();

		dVal = dDesignLoadCompress * dFck;
		pOut->SetXL(pXL, dVal,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT, "", 1);
		
		//////인장응력
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow,	m_nSttRow,	"R",	"U",	CURRENT, RED, 9, FALSE, TA_LEFT, "", 1);
		double dDesignLoadStretch = pCalcData->GetConstDesignLoadStretch();

		dVal = dDesignLoadStretch * sqrt(dFck);
		pOut->SetXL(pXL, dVal,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT);
		
		////균열응력
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_strCrackPow,					m_nSttRow,	"R",	"U",	CURRENT, RED, 9, FALSE, TA_LEFT, "", 1);
		szType	= pCalcData->DESIGN_MATERIAL.m_strCrackPow;
		dVal	= (szType.Find("0.63")>-1)? 0.63*sqrt(dFck) : (szType.Find("0.54")>-1)? 0.54*dFck : 0.47*dFci;
		pOut->SetXL(pXL,	dVal,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT);
		
		//바닥판 콘크리트와 강재의 주거더와의 온도 차
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dTemparature,			m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
		
		//바닥판 콘크리트와 강재의 선팽창계수
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dAlpha,				m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 6);
	}
	else
	{
		//압축응력
		sText.Format("= 0.4 * %s", pOut->GetCellRef("CONC_SIGMACK"));
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT, "", 1);

		m_nSttRow++;

		//인장응력
		sText.Format("=IF(%s * 0.07 >2.5, 2.5, %s * 0.07)", pOut->GetCellRef("CONC_SIGMACK"), pOut->GetCellRef("CONC_SIGMACK"));
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT, "", 1);
	}
	
	//건조수축 계수
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dEpsilonS,				m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 6);
	
	//상대습도
	if(pBridge->m_bTendonInputComplete)
		pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity,	m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
	
	//CREEP 계수
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dCreep,				m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);

	m_nSttRow++;
	
	if(pBridge->IsTUGir())
	{
		m_nSttRow++;

		////콘크리트 설계 기준강도
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dBindSigmaCK,	m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "CONC_BIND_SIGMACK", 1);
		
		////콘크리트 탄성계수
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dBindEc,		m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
		
		//압축응력
		sText.Format("= 0.4 * %s", pOut->GetCellRef("CONC_BIND_SIGMACK"));
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT, "", 1);
		
		m_nSttRow++;
		
		//인장응력
		sText.Format("=IF(%s * 0.07 >2.5, 2.5, %s * 0.07)", pOut->GetCellRef("CONC_BIND_SIGMACK"), pOut->GetCellRef("CONC_BIND_SIGMACK"));
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"W",	"Y",	NEXT, RED, 9, FALSE, TA_RIGHT, "", 1);

		//바닥판 콘크리트와 강재의 주거더와의 온도 차
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dTemparature,			m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
		
		//바닥판 콘크리트와 강재의 선팽창계수
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dAlpha,				m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 6);

		//건조수축 계수
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dEpsilonS,				m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 6);
		
		//상대습도
		pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity,	m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
		
		//CREEP 계수
		pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dCreep,				m_nSttRow,	"R",	"U",	3, RED,	9,	FALSE,	TA_RIGHT, "", 1);
	}
	else
	{
		pXL->DeleteRowLineDirect(m_nSttRow, m_nSttRow + 11);
		m_nSttRow++;
	}
	
	//[바닥판 철근]
	////철근 설계 항복강도
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dSigmaY,		m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
	
	////철근 탄성계수
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dEs,			m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
	
	////정착구 검토시
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dSigmaSA,		m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
	
	////바닥판 검토시
	pOut->SetXL(pXL,	pCalcData->DESIGN_MATERIAL.m_dSigmaDeckSA,	m_nSttRow,	"R",	"U",	NEXT, RED,	9,	FALSE,	TA_RIGHT, "", 1);
	
	m_nSttRow += 1;

	//[PS 강재]
	
	//PS 강선 호칭
 	sText.Format("(%s)", pPSData->m_szName);
 	pOut->SetXL(pXL,	sText,	m_nSttRow,	"G",	"J",	NEXT,	RED);
	
	////단면적
//	m_nSttRow++;
	pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea,			m_nSttRow,	"N",	"P",	0,		RED, 9, FALSE, TA_CENTER, "", 1);
	sText.Format("%d",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty);
	pOut->SetXL(pXL,	sText,													m_nSttRow,	"R",	"S",	NEXT,	RED,	9,	FALSE,	TA_LEFT,	"",	0);
	
	////극한강도
	pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength,	m_nSttRow,	"R",	"U",	NEXT,	RED	,	9,	FALSE, TA_RIGHT, "", 1);
	
	////항복응력
	pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength,		m_nSttRow,	"R",	"U",	NEXT,	RED	,	9,	FALSE, TA_RIGHT, "", 1);
	
	CString			strValue = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax;
	CStringArray	strArr;
	BOOL			bExistUserValue = TRUE;
	strArr.Add("0.70·fpu");
	strArr.Add("0.83·fpy");
	strArr.Add("0.90·fpy");
	
	for(nIdx = 0; nIdx < strArr.GetSize(); nIdx++)
	{
		if(strValue == strArr[nIdx])	
		{
			bExistUserValue = FALSE;
			break;
		}
	}

	////Jack에 의한 최대 긴장 응력
	if(bExistUserValue)	//사용자 정의 값이 존재할 때
	{
		sText.Format("%g", pCalcData->GetJackMaxStrToDouble(strValue));
		pOut->SetXL(pXL,	"",		m_nSttRow,	"R",	"U",	0,	BLACK);
		pOut->SetXL(pXL,	"",		m_nSttRow,	"V",	"V",	0,	BLACK);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"W",	"Y",	0,	RED);
		pXL->DeleteColSell(m_nSttRow, 18, m_nSttRow, 21);
		pXL->SetMergeCell(m_nSttRow-1, 17, m_nSttRow-1, 20);
		m_nSttRow++;
	}
	else
	{
		pOut->SetXL(pXL,	strValue,	m_nSttRow,	"R",	"U",	NEXT,	RED, 9, FALSE, TA_LEFT, "", 1);
	}
	
	//탄성계수
	pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs,				m_nSttRow,	"R",	"U",	NEXT,	RED,	9,	FALSE,	TA_RIGHT, "", 1);
	
	//파상마찰계수
	pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction,	m_nSttRow,	"R",	"U",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
	
	//곡률마찰계수
	pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction,	m_nSttRow,	"R",	"U",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
	
	//정착장치의 활동량
	pOut->SetXL(pXL,	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip,		m_nSttRow,	"R",	"U",	NEXT,	RED,	9,	FALSE,	TA_RIGHT, "", 1);
}

//<summary>
// 1.2 하중조건
//</summary>
//<param name="CXLDesignCondition::LoadCondition("></param>
void CXLDesignCondition::LoadCondition()
{
	CAPlateOutCalcStd	*pStd			= m_pAPlateOutCalcStd;
	CDataManage			*pDataManage	= pStd->GetDataManage();
	CPlateBridgeApp		*pBridge		= pDataManage->GetBridge();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CString				sText			= _T("");
	
	double		dSteelCon		= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun);
	double		dMugun			= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dMugun);
	double		dAsphaltCon		= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt);
	double		dGanJae			= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dGangjae);
	
	CCalcGeneral	CalGeneral(pDataManage);

	CAPlateCalcStd  *pCalcStd		= pStd->GetAPlateCalcStd();
	CFEMManage		*pFemManage		= pCalcStd->GetHLLFemData(0);
	POSITION		pos				= pFemManage->m_ElementMap.GetStartPosition();
	double			dEffWidth		= 0.0;
	
	while(pos)
	{
		WORD			nIdx;
		CElement	*pE = NULL;
		
		pFemManage->m_ElementMap.GetNextAssoc(pos, nIdx, (void *&)pE);		

		CJoint *pJ_I = pFemManage->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFemManage->GetJoint(pE->m_nJEnd);
		
		double dDist= ~(pJ_I->m_vPoint - pJ_J->m_vPoint);

		dEffWidth += dDist;
	}	

	double nQtyLane	= CalGeneral.GetQtyLane(dEffWidth);

	m_nSttRow += 3;

	pOut->SetXL(pXL, dSteelCon,				m_nSttRow,	"J",	"L",	NEXT,	RED,	9,	FALSE,	TA_CENTER);
	pOut->SetXL(pXL, dMugun,				m_nSttRow,	"J",	"L",	NEXT,	RED,	9,	FALSE,	TA_CENTER);
	pOut->SetXL(pXL, dGanJae,				m_nSttRow,	"J",	"L",	NEXT,	RED,	9,	FALSE,	TA_CENTER);
	pOut->SetXL(pXL, dAsphaltCon,			m_nSttRow,	"J",	"L",	NEXT,	RED,	9,	FALSE,	TA_CENTER);

	m_nSttRow+=2;

	//설계 차선수 산정	
	if(pCalcData->m_nHLiveLoadType	==0)	//표준점유폭(3m)마다 연속으로 재하
	{
		pXL->DeleteRowLineDirect(m_nSttRow + 4,	m_nSttRow + 11);
		
		m_nSttRow++;
		
		pOut->SetXL(pXL,	dEffWidth,	m_nSttRow,	"G",	"I",	0,		RED,	9,	FALSE,	TA_CENTER);			//WC		
		pOut->SetXL(pXL,	nQtyLane,	m_nSttRow,	"N",	"O",	NEXT,	RED,	9,	FALSE,	TA_CENTER,	"",	0);	//N		
		pOut->SetXL(pXL,	3,			m_nSttRow,	"G",	"I",	NEXT,	RED,	9,	FALSE,	TA_CENTER);			//W

		m_nSttRow++;

	}
	else if(pCalcData->m_nHLiveLoadType	== 1)	//설계차로폭을 고려하여 재하
	{
		pXL->DeleteRowLineDirect(m_nSttRow,	m_nSttRow + 3);
		m_nSttRow++;
				
		pOut->SetXL(pXL,	dEffWidth,	m_nSttRow,	"G",	"I",	0,		RED,	9,	FALSE,	TA_CENTER);			//WC
		pOut->SetXL(pXL,	nQtyLane,	m_nSttRow,	"N",	"O",	NEXT,	RED,	9,	FALSE,	TA_CENTER,	"",	0);	//N
		pOut->SetXL(pXL,	3.6,		m_nSttRow,	"U",	"W",	NEXT,	RED,	9,	FALSE,	TA_CENTER);			//VALUE
		
		m_nSttRow++;

		pXL->DeleteRowLineDirect(m_nSttRow,	m_nSttRow+3);

	}
	else	//설계차로폭 사용자 정의
	{	
		pXL->DeleteRowLineDirect(m_nSttRow,	m_nSttRow + 7);
		m_nSttRow++;
		
		pOut->SetXL(pXL,	dEffWidth,						m_nSttRow,	"G",	"I",	0,		RED,	9,	FALSE,	TA_CENTER);			//WC
		pOut->SetXL(pXL,	nQtyLane,						m_nSttRow,	"N",	"O",	NEXT,	RED,	9,	FALSE,	TA_CENTER,	"",	0);	//N
		pOut->SetXL(pXL,	pCalcData->m_dUserLaneWidth,	m_nSttRow,	"G",	"I",	NEXT,	RED,	9,	FALSE,	TA_CENTER);			//W

		m_nSttRow++;
	}
	
	m_nSttRow	+= 11;
	//충격계수
	for(long nJ = 0; nJ < pBridge->m_nQtyJigan ; nJ++)
	{
		pXL->InsertRowLine(m_nSttRow,2);
		
		//엑셀 서식 참조
		pOut->SetXL(pXL,	"=D12",	m_nSttRow,	"D",	"D",	0,	BLACK);
		
		sText.Format("%d 지간", nJ+1);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"E",	"F",	NEXT,	BLACK);
		
		pOut->SetXL(pXL,	"i",				m_nSttRow,	"E",	"E",	0,	BLACK);
		pOut->SetXL(pXL,	"=",				m_nSttRow,	"F",	"F",	0,	BLACK);
		pOut->SetXL(pXL,	"15 / (40 + L) ",	m_nSttRow,	"G",	"J",	0,	BLACK);
		pOut->SetXL(pXL,	"=",				m_nSttRow,	"K",	"K",	0,	BLACK);
		
		sText.Format("%g", 15 / (40 + toM(pBridge->m_dLengthJigan[nJ])) );
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"L",	"N",	0,  RED);
		
		sText.Format("=IF(L%d>R%d,\">\",\"<\")", m_nSttRow,	m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"P",	"P",	0,	BLACK);

		pOut->SetXL(pXL,	0.300,	m_nSttRow,	"R",	"T",	0,	RED);
		pOut->SetXL(pXL,	"∴",	m_nSttRow,	"V",	"V",	0,	BLACK);
		pOut->SetXL(pXL,	"i",	m_nSttRow,	"W",	"W",	0,	BLACK);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"X",	"X",	0,	BLACK);
		
		sText.Format("=IF(L%d>R%d,R%d,L%d)",m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"Y",	"AA",	NEXT,	BLACK);

	}
	
	BOOL	bPeopleLoadRight	=	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bPeopleLoad;
	BOOL	bPeopleLoadLeft		=	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bPeopleLoad;
	BOOL	bBangEmRight		=	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;
	BOOL	bBangEmLeft			=	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
	BOOL	bNanGanRight		=	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan;
	BOOL	bNanGanLeft			=	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan;

	//군중하중
	if(!bPeopleLoadRight &&  !bPeopleLoadLeft)
		pXL->DeleteRowLine(m_nSttRow,	m_nSttRow+2);
	
	m_nSttRow += 12;

	//풍하중
	if(!bBangEmRight && !bBangEmLeft && !bNanGanRight && !bNanGanLeft)
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+5);
	
	m_nSttRow += 8;

	//충돌하중
	
	////1
	if(!bNanGanLeft  && !bNanGanRight)
		pXL->DeleteRowLine(m_nSttRow,	m_nSttRow+5);
	
	m_nSttRow += 6;
	
	////2
	if(bNanGanLeft && bNanGanRight)	pXL->DeleteRowLine(m_nSttRow,m_nSttRow+7);
	else							pOut->InsertImage(pXL, "CALC_COLLISION", m_nSttRow + 3, "E");

	m_nSttRow += 15;
	
	//특수하중
	
	if(!pCalcData->m_bTank && !pCalcData->m_bTrailer)
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+12);
		m_nSttRow += 18;
	}
	else
	{
		m_nSttRow += 2;
		
		////Tank
		if(!pCalcData->m_bTank)
		{
			pXL->DeleteRowLine(m_nSttRow,m_nSttRow+4);
			m_nSttRow += 5;
		}
		else
		{
			m_nSttRow++;
			
			//1) 탱크궤도 하중		
			pOut->SetXL(pXL,	pCalcData->m_TankWeight,	m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
			pOut->SetXL(pXL,	pCalcData->m_TankEdit1,		m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
			pOut->SetXL(pXL,	pCalcData->m_TankImpact,	m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
		}
		
		////Trailer
		if(!pCalcData->m_bTrailer)
			pXL->DeleteRowLine(m_nSttRow,m_nSttRow+6);
		else
		{
			
			double dTrailerWeight	=	(pCalcData->m_TrailP1 + pCalcData->m_TrailP2*2 + pCalcData->m_TrailP3 *2)*2;

			m_nSttRow += 2;
			
			//2)탱크 트레일러 하중
			pOut->SetXL(pXL,	dTrailerWeight,				m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
			pOut->SetXL(pXL,	pCalcData->m_TrailP1,		m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
			pOut->SetXL(pXL,	pCalcData->m_TrailP2,		m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
			pOut->SetXL(pXL,	pCalcData->m_TrailP3,		m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
			pOut->SetXL(pXL,	pCalcData->m_TrailImpact,	m_nSttRow,	"K",	"M",	NEXT,	RED,	9,	FALSE,	TA_RIGHT);
		}
	}
	
	if(pBridge->IsTUGir())	
	{
		TU_ProgressOfWork();
		sText = "   6) 참 고 문 헌";
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"A",	"F",	NEXT, BLACK,	9,	TRUE,	TA_LEFT);
	}
	else
	{
		long nCurRow = m_nSttRow;
		// 참고문헌
		if(pOpt->GetSteelStandardYear()==APPLYYEAR2010)
		{
			m_nSttRow += 60;
			pOut->SetXL(pXL, _T("도로교 설계기준 (2010)"), m_nSttRow,	"D",	"D",	NEXT, BLACK,	9,	FALSE,	TA_LEFT);
			m_nSttRow++;
			pOut->SetXL(pXL, _T("콘크리트 구조 설계기준 (2007)"), m_nSttRow,	"D",	"D",	NEXT, BLACK,	9,	FALSE,	TA_LEFT);
		}
		m_nSttRow = nCurRow;
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow + 58);
	}

	pXL->DeleteRowLineEnd();

	m_nSttRow = 18;
	if(pOpt->GetSteelStandardYear()==APPLYYEAR2010)
		pXL->InsertRowLine(m_nSttRow, 28);

	pXL->SetActiveSheet("0. 설계조건");
	if(pOpt->GetSteelStandardYear()==APPLYYEAR2010)
		pXL->CopyRange(113, 1, 250, 30);
	else
		pXL->CopyRange(2, 1, 111, 30);
	pXL->SetActiveSheet("1. 설계조건");
	pXL->AddDomWmf(NULL, m_nSttRow-1, 0, 0, 0);
}

//<summary>
// 1.3 사용재료
//</summary>
//<param name="CXLDesignCondition::UseMaterial("></param>
void CXLDesignCondition::UseMaterial()
{
	CAPlateOutCalcStd	*pStd			= m_pAPlateOutCalcStd;
	CDataManage			*pDataManage	= pStd->GetDataManage();
	CPlateBridgeApp		*pBridge		= pDataManage->GetBridge();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText;
	double	dFck	= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dEc		= pCalcData->DESIGN_MATERIAL.m_dEc;
	double	dFy		= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dEs		= pCalcData->DESIGN_MATERIAL.m_dEs;
//	long	nBridgeType	= pStd->GetBridgeType();

	long	nPSIdx			= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressPlate		*pSteelPS		= pDataManage->GetSteelPrestress();
	CSteelPrestressData			*pSteelPSData	= pSteelPS->GetSteelPrestressData(nPSIdx);
	CString	szStandard		= pSteelPSData->m_szStandard;
	CString	szSymbol		= pSteelPSData->m_szSymbol;
	double	dPSDiameter		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter;
	double	dPSArea			= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea;
	double	dPSEs			= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	double	dTensionLoad	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionLoad;
	double	dRateLoad		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dLoadOfElongation;
	long	nPSQty			= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;

	pOut->SetXL(pXL, "3) 사 용 재 료",			m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "ⅰ) 바 닥 판 콘크리트",	m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "① 강    도 : fck =",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dFck,						m_nSttRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "FCK", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "L", "L");
	pOut->SetXL(pXL, "② 탄성계수 : Ec  =",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dEc,						m_nSttRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "L", "L");

	if(pBridge->IsSosuGir() || pBridge->IsTUGir() && pBridge->m_bTendonInputComplete)
	{
		pXL->InsertRowLine(m_nSttRow, 5);

		pOut->SetXL(pXL, "③ 프리스트레스 도입시 콘크리트의 압축강도 fci = 0.70·fck", m_nSttRow, "D", "D", 0);
		sText.Format("=%s*0.70", pOut->GetCellRef("FCK"));
		pOut->SetXL(pXL, sText,					m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "FCI", 1);
		pOut->SetXL(pXL, "MPa",			m_nSttRow, "V", "V", 1);

		pOut->SetXL(pXL, "④ 프리스트레스 도입직 후",	m_nSttRow, "D", "D");
		pOut->SetXL(pXL, "· 휨 압축응력 : 0.55·fci",	m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.55*%s", pOut->GetCellRef("FCI"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "P", "P", 1);

		pOut->SetXL(pXL, "· 휨 인장응력 : 0.8√fci",	m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.8*SQRT(%s)", pOut->GetCellRef("FCI"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "P", "P", 1);

		pOut->SetXL(pXL, "⑤ 설계하중 작용시",			m_nSttRow, "D", "D");
		pOut->SetXL(pXL, "· 휨 압축응력 : 0.4·fck",	m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.4*%s", pOut->GetCellRef("FCK"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "P", "P", 1);

		pOut->SetXL(pXL, "· 휨 인장응력 : 1.6√fck",	m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=1.6*SQRT(%s)", pOut->GetCellRef("FCK"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "P", "P", 1);
	}

	pOut->SetXL(pXL, "ⅱ) 철 근",				m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "① 항복강도 : fy  =",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dFy,						m_nSttRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "L", "L");
	pOut->SetXL(pXL, "② 탄성계수 : Es  =",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dEs,						m_nSttRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "L", "L");

	pOut->SetXL(pXL, "ⅲ) 강 재",				m_nSttRow, "C", "C");
	sText.Format("① 주 부 재 : %s 사용 ( 상·하 FLANGE, WEB, 수평보강재, STRINGER,", pCalcData->DESIGN_MATERIAL.m_szSwsJu);
	pOut->SetXL(pXL, sText,						m_nSttRow, "D", "D", NEXT);
	pOut->SetXL(pXL, " 지점부 수직보강재, 주부재 연결판, SOLE PLATE )",		m_nSttRow, "J", "J");
	sText.Format("② 부 부 재 : %s 사용 ( CROSS BEAM, 수직브레이싱, 수평브레이싱, 수직보강재)", pCalcData->DESIGN_MATERIAL.m_szSwsBu);
	pOut->SetXL(pXL, sText,	m_nSttRow, "D", "D", 1);

	if(pBridge->IsSosuGir() || pBridge->IsTUGir() && pBridge->m_bTendonInputComplete)
	{
		pXL->InsertRowLine(m_nSttRow, 10);
		
		pOut->SetXL(pXL, "ⅳ) PS 강재 (저 릴렉셔이션 강재)", m_nSttRow, "C", "C", 0);
		pOut->SetXL(pXL, "(도로교 설계기준 P87)",		m_nSttRow, "U", "U", NEXT);
		pOut->SetXL(pXL, szStandard,					m_nSttRow, "D", "D", 0, RED);
		pOut->SetXL(pXL, szSymbol,						m_nSttRow, "F", "F", 0, RED);

		pOut->SetXL(pXL, "(",					m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "φ",					m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f", dPSDiameter);
		pOut->SetXL(pXL, sText,					m_nSttRow, "J", "K", 0, RED,   9, FALSE, TA_CENTER, "PS_DIA", 1);
		pOut->SetXL(pXL, "mm",					m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, ")",					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%d", nPSQty);
		pOut->SetXL(pXL, sText,					m_nSttRow, "O", "O", 0, RED,   9, FALSE, TA_CENTER, "PS_QTY", 0);
		pOut->SetXL(pXL, "EA",					m_nSttRow, "P", "P", 1, BLACK, 9);
		pOut->SetXL(pXL, "단면적 :",			m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "A",					m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",					m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f", dPSArea);
		pOut->SetXL(pXL, sText,					m_nSttRow, "I", "K", 0, RED,   9, FALSE, TA_CENTER, "PS_AREA");
		pOut->SetXL(pXL, "×",					m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("PS_QTY"));
		pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*%s", pOut->GetCellRef("PS_AREA"), pOut->GetCellRef("PS_QTY"));
		pOut->SetXL(pXL, sText,					m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PS_TOT_AREA");
		pOut->SetXL(pXL, "mm²",				m_nSttRow, "R", "R", 1);
		pOut->SetXL(pXL, "Es",					m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "=",					m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dPSEs,					m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, "MPa",					m_nSttRow, "I", "I", 1);

		pOut->SetXL(pXL, "① 극한강도 : fpu",			m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "=",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dTensionLoad,					m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "PS_TENSIONLOAD", 0);
		pOut->SetXL(pXL, "/",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("PS_TOT_AREA"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/%s", pOut->GetCellRef("PS_TENSIONLOAD"), pOut->GetCellRef("PS_TOT_AREA"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "FPU", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "U", "U");

		pOut->SetXL(pXL, "② 항복강도 : fpy",			m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "=",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dRateLoad,						m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "PS_RATELOAD", 0);
		pOut->SetXL(pXL, "/",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("PS_TOT_AREA"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/%s", pOut->GetCellRef("PS_RATELOAD"), pOut->GetCellRef("PS_TOT_AREA"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "FPY", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "U", "U");

		pOut->SetXL(pXL, "③ 정착구에서의 응력 : 0.7fpu",m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.7*%s", pOut->GetCellRef("FPU"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "P", "P");

		pOut->SetXL(pXL, "④ 손실 후 사용하중 상태에서의 응력 :",m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "0.8fpy",						m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",							m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.8*%s", pOut->GetCellRef("FPY"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "U", "U");
	}

	double	dEpsilonS	= pCalcData->DESIGN_MATERIAL.m_dEpsilonS;	///< 건조수축 계수
	double	dCreep2		= pCalcData->DESIGN_MATERIAL.m_dCreep2;		///< 크리프계수
	double	dAlpha		= pCalcData->DESIGN_MATERIAL.m_dAlpha;		///< 선팽창 계수
	double	dTemparature= pCalcData->DESIGN_MATERIAL.m_dTemparature;///< 바닥판 콘크리트와 강형과의 온도차

	if(pBridge->IsPlateGir())
		sText.Format("ⅳ) 재료의 물리적 특성치");
	else
		sText.Format("ⅴ) 재료의 물리적 특성치");
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.9.2.7,8)",				m_nSttRow, "U", "U", NEXT);
	pOut->SetXL(pXL, "① 건조수축 계수         : εs",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dEpsilonS,								m_nSttRow, "M", "O", 1, RED,   9, FALSE, TA_CENTER, "", 5);

	pOut->SetXL(pXL, "② 콘크리트 크리프계수   : ф₂",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "   = 2·ф₁",						m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dCreep2,								m_nSttRow, "O", "O", 1, RED,   9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, "③ 선팽창 계수           : α",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAlpha,								m_nSttRow, "M", "O", 1, RED,   9, FALSE, TA_CENTER, "", 6);

	pOut->SetXL(pXL, "④ 바닥판 콘크리트와 강형과의 온도차 :", m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "ΔT = ",								m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, dTemparature,							m_nSttRow, "P", "Q", 0, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "℃",									m_nSttRow, "R", "R", 1);
	SetLines(1);

	// 허용응력
	SetLines(117);
}


//<summary>
// 1.5 설계방법
//</summary>
//<param name="CXLDesignCondition::DesignMethod("></param>
void CXLDesignCondition::DesignMethod()
{
	CXLControl			*pXL		 = m_pXL;
	CAPlateOutCalcXL	*pOut		 = m_pAPlateOutCalcXL;
	CDataManage			*pDataManage = m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt		 = pDataManage->GetGlobalOption();

//	m_nSttRow	= 183;
	m_nSttRow++;
	pOut->SetXL(pXL, "4) 설 계 방 법",								m_nSttRow, "B", "B");
	CString sTemp = _T("");
	
	sTemp.Format("ⅰ) 바닥판                : %s",pOpt->GetStrSlabCentDesignMethod());	
	pOut->SetXL(pXL, sTemp,	m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "ⅱ) STEEL PLATE GIRDER    : 허용응력 설계법",		m_nSttRow, "C", "C", 2);
}

//<summary>
// 1.5 제작공정도(only TU Girder)
//</summary>
//<param name="CXLDesignCondition::TU_ProgressOfWork("></param>
void CXLDesignCondition::TU_ProgressOfWork()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	CString szText;
	const int nSize = 8;
	CString szContent[nSize]	= { "1. 강재거더 거치", "2. 구속콘크리트 철근조립",
									"3. 거푸집 설치 및 타설", "4. 탈형",
									"5. 양생 및 적치", "6. 턴오버",
									"7. 가설", "8. 바닥판 타설"	};

	m_nSttRow++;

	for(long n=0; n< nSize; n++)
	{
		if(n%2==0)
		{	
			szText.Format("TU_PIC0%d", n+1);
			pOut->InsertImage(pXL, szText, m_nSttRow+1, "D");
		}
		else
		{
			szText.Format("TU_PIC0%d", n+1);
			pOut->InsertImage(pXL, szText, m_nSttRow+1, "R");	
			m_nSttRow += 10;
		}		
	}

	m_nSttRow += 1;
	pOut->InsertImage(pXL, "TU_PIC_SPC2", m_nSttRow, "C");
	
	m_nSttRow += 17;
}

//<summary>
// 1.6 참고문헌
//</summary>
//<param name="CXLDesignCondition::RefLiterature("></param>
void CXLDesignCondition::RefLiterature()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CPlateBridgeApp		*pBridge		= pDataManage->GetBridge();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	CString szText;

	if(pBridge->IsTUGir())
		szText.Format("7) 참 고 문 헌");
	else
		szText.Format("5) 참 고 문 헌");

	pOut->SetXL(pXL, szText,		m_nSttRow, "B", "B");
	for(long n=0; n < pCalcData->m_szRefArray.GetSize(); n++)
	{
		szText.Format(" (%d) %s", n+1, pCalcData->m_szRefArray.GetAt(n));
		pOut->SetXL(pXL, szText, m_nSttRow, "C", "C");
	}
}

//<summary>
// 2. 단면가정
//</summary>
//<param name="CXLDesignCondition::DesignSection("></param>
void CXLDesignCondition::DesignSection()
{
	CPlateBridgeApp		*pBridge		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateDrawDanmyun	*pDanmyun		= m_pAPlateOutCalcStd->GetAPlateDrawDanmyun();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CARoadOptionStd		*pOpt			= pDataManage->GetOptionStd();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CDomyun				Dom;
	m_nSttRow = 1;

	pOut->SetXL(pXL, "2. 단 면 가 정", m_nSttRow, "A", "A", NEXT, BLACK, 11, TRUE);

	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	//////////////////////////////////////////////////////////////////////////////////////////////////
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(50);
/*
	CPlateBxFinder Finder(pBridge->GetGirder(0));
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	pBx = Finder.GetBxNext();
	pDanmyun->DrawGuardWall(&Dom, pBx);

	BOOL bWalkLoad	= FALSE;
	if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad ||
		pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)
		bWalkLoad	= TRUE;

	//지점부 슬래브 단면
	for(long nG=0;nG<pBridge->GetGirdersu();nG++)
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		pDanmyun->DrawDanmyunOnBx(&Dom, pBx, bWalkLoad);
	}

	///< 치수선
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 40*(dDiagonalL/8500);
	Dom.SetScaleDim(dScale);

	// 상부 플래지의 두께가 가장 큰 위치의 공장이음 시점에서 교량진행방향으로 가장 가까운 가로보위치(nDir=1)
	pBx = pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();

	pDanmyun->DimSlabUpper(&Dom, pBx, FALSE);
	pDanmyun->DimSlabHunch(&Dom, pBx);
	pDanmyun->DimSlabCenterLine(&Dom, pBx);
	pDanmyun->DimSlabHunchWidth(&Dom, pBx);
	pDanmyun->DimSlabBaseThick(&Dom, pBx);

	//헌치부 슬래브 단면 DOT LINE
	for(nG=0;nG<pBridge->GetGirdersu();nG++)
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			if(!pBx->IsJijum()) break;			
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
		if(!pBx) continue;
		pOpt->SetEnvType(&Dom, HCAD_STLH);
		pDanmyun->DrawSlabHunch(&Dom, pBx);
	}
	
	// Slop
	pOpt->SetEnvType(&Dom, HCAD_DIML);

	CPlateGirderApp	*pCRGir = pBridge->GetGirder(-1);
	CPlateBxFinder 	 FinderStt(pCRGir);
	CPlateBasicIndex *pBxCR = FinderStt.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
	pBxCR	 = FinderStt.GetBxNext();
	pDanmyun->DimTextSlopeArrow(&Dom, pBxCR);
	pDanmyun->DrawTensionBar(&Dom,pBxCR,TRUE);
*/

	CPlateBasicIndex *pBx = pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	pDanmyun->DrawGuardWall(&Dom, pBx);	// 방호벽
	pDanmyun->DrawDanmyunOnBx(&Dom, pBx, FALSE, TRUE, CAPlateDrawCross::NOTDRAWVSTIFF);	// 지점부 슬래브 단면
	pDanmyun->DrawTensionBar(&Dom, pBx, TRUE);	// 지점부 슬래브 단면
	
	///< 치수선그리기
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 75*(dDiagonalL/10000);
	Dom.SetScaleDim(dScale);
	
	pDanmyun->DimTensionBarOnLineInfo(&Dom, pBx, TRUE);	//직선
	pDanmyun->DimTensionBar(&Dom, pBx, TRUE);				//변곡선
	pDanmyun->DimTypicalSectionBottom(&Dom, pBx);
	pDanmyun->DimTextSlopeArrow(&Dom, pBx);


	Dom.Rotate(ToRadian(90),0,0);


	m_nSttRow = 2;
	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow);
	CString szCell1 = _T("A1");
	CString	szCell2;
	szCell2.Format("AC%d", m_nSttRow+5);
	pXL->SetPrintArea(szCell1, szCell2);

}

///< 최대 또는 최소의 거더폭을 찾는다.
double	CXLDesignCondition::FindMaxMinGirderWidth(BOOL bUpper, BOOL bIsMaxWidth)
{
	CPlateBridgeApp		*pBridge	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();

	long	nGirdersu	= pBridge->GetGirdersu();
	double	dValue		= bIsMaxWidth ? 0 : 9999.0;

	for(long nG = 0; nG < nGirdersu; nG++)
	{
		CPlateBxFinder		Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex	*pBx	= (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		CPlateGirderApp		*pGir	= pBridge->GetGirder(nG);
		while(pBx)
		{
			double dGirderWidth = bUpper ? pGir->GetWidthFlangeUpper(pBx) : pGir->GetWidthFlangeLower(pBx);
			if(bIsMaxWidth)
			{
				if(dValue < dGirderWidth)
					dValue = dGirderWidth;
			}
			else
			{
				if(dValue > dGirderWidth)
					dValue = dGirderWidth;
			}
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}
	return dValue;
}
///< 최대 또는 최소의 거더높이를 찾는다.
///< TU 거더일때 - 구속콘크리트 상면 ~ 하부플랜지 하면
///< TU 아닐때   - 거더높이기준 설정에 따른 길이 (횡단구성 입력값)
double	CXLDesignCondition::FindMaxMinGirderHeight(BOOL bIsMaxHeight)
{
	CPlateBridgeApp	*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CPlateGirderApp	*pGir	= NULL;

	long nGirdersu	= pDB->GetGirdersu();
	long nQtyJigan	= pDB->m_nQtyJigan;
	long nSelectG	= 0;
	long nSelectJ	= 0;
	double dValue	= pDB->GetGirder(0)->GetHeightGirderByJijum(0);

	for(long nJijum = 0; nJijum < nQtyJigan+1; nJijum++)
	{
		for(long nGir=0; nGir < nGirdersu; nGir++)
		{
			pGir	= pDB->GetGirder(nGir);
			double	dGirderHeight	= pGir->GetHeightGirderByJijum(nJijum);

			if(bIsMaxHeight && dValue < dGirderHeight)
			{
				dValue	 = dGirderHeight;
				nSelectG = nGir;
				nSelectJ = nJijum;
			}
			else if(!bIsMaxHeight && dValue > dGirderHeight)
			{
				dValue	 = dGirderHeight;
				nSelectG = nGir;
				nSelectJ = nJijum;
			}
		}
	}

	pGir	= pDB->GetGirder(nSelectG); 
	dValue	= pGir->GetHeightGirderByJijum(nSelectJ, pDB->IsTUGir());
	if(pDB->IsTUGir())
	{
		CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nSelectJ);
		double dHeightBindConc	= pDB->m_BindConc.m_dHeight;
		double dDisBindConc	= pDB->m_BindConc.m_dDis;

		// dValue = 복부판의 길이

		double dUpperThick = pGir->GetThickJewon(G_F_U,pBx);
		double dLowerThick = pGir->GetThickJewon(G_F_L,pBx);
		dValue += dUpperThick + dLowerThick + (dHeightBindConc - dDisBindConc);
	}

	return dValue;
}

void CXLDesignCondition::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}

///////////////////////////////////////////////////////////////////////////////////
//∫∴∵∮∑∏ㄷ ΑΒΓΔΕΖΗαβγδεζηΣθιΙκλμνξοπρf υφχψω
//	´≤＞＜≠±≤≥∞⌒×㎝⁴ ㎤ ㎠ ㎝ kgf/㎠ t·m  ¹²³⁴₁₂₃₄
//  ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩ ⑪ ⑫ ⑬ ⑭ ⑮ √½⅓㉮㉯㉰㉱㉲㉳㉴
//  ━━━━━━━━━━━━━  ⇒ ⊙
//  ┏ ┗ ┛ ┓┃━ ┣ ┫ⅰ ⅱ ⅲ ⅳ ⅴ ⅵ ⅶ ⅷ ⅸ ⅹ Ⅰ Ⅱ Ⅲ Ⅳ Ⅳ Ⅴ Ⅵ
///////////////////////////////////////////////////////////////////////////////////// 