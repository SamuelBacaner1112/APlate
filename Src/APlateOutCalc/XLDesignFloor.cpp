// XLDesignFloor.cpp: implementation of the CXLDesignFloor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "XLDesignFloor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)
#define EPSILON 0.000000001

CXLDesignFloor::CXLDesignFloor(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_pAPlateOutCalcStd	= pStd;
	m_pBx				= NULL;
	m_pSep				= NULL;
	m_bLeft				= TRUE;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
	m_dPreMd			= 0;
	m_dPostMd			= 0;
	m_nW3				= 0;
}

CXLDesignFloor::~CXLDesignFloor()
{
}

void CXLDesignFloor::DesignFloor(BOOL bLeft)
{
	m_bLeft = bLeft;

	CPlateBridgeApp		*pDB				= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage		= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData			= pDataManage->GetCalcData();
	CXLControl			*pXL				= m_pXL;
	CAPlateOutExcel		*pAPlateOutXL		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutCalcXL	*pOut				= m_pAPlateOutCalcXL;
	CPlateGirderApp		*pGir				= pDB->GetGirder(0);
	CARoadOptionStd		*pOptStd			= pDataManage->GetOptionStd();
	CDomyun				Dom;
	CAPlateDrawCalc		DrawCalc(pDataManage);	
	CCalcGeneral		CalcGeneral(pDataManage);
	CCalcFloor			CalcFloor(pDataManage);

	Initialize();
	
	long	nFloorIdx	= m_bLeft ? FLOOR_LEFT : FLOOR_RIGHT;
	double	dLen		= 0;
	double	dPr			= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dAsphalt	= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt);//*pow(10,6);
	double	dChulgun	= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun);//*pow(10,6);
	double	dMugunUW	= tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dMugun);//*pow(10,6);
	double	dV			= pCalcData->m_dSpeed;
	double	dR			= pCalcData->CALC_CANTILEVER_DATA[nFloorIdx].m_Cent_R;
	double	dBangEmH	= toM(pCalcData->DESIGN_FLOOR_DATA[nFloorIdx].m_dBangEmHeight);
	BOOL	bBangEm		= pCalcData->DESIGN_FLOOR_DATA[nFloorIdx].m_bBangEm;
	BOOL	bTank		= pCalcData->m_bTank;
	BOOL	bTrailer	= pCalcData->m_bTrailer;
	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nFloorIdx].m_bWalkLoad;
	long	dVelocity   = (long)pCalcData->m_dSpeed;	//충돌하중 속도
	double	dWindLoadForce = pCalcData->m_dWindLoadForce;	//  풍하중 강도         

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(20);
	
	double	dDis_Car		= CalcGeneral.GetDBLOAD_Dis_CAR();
	double	dDis_Tank		= CalcGeneral.GetDBLOAD_Dis_Tank();
	double	dDis_Trailer	= CalcGeneral.GetDBLOAD_Dis_Trailer();
	long	nType			= 0;
	long	nGuardType		= 0;
	DrawCalc.SetDBLoad(dDis_Car, dDis_Tank, dDis_Trailer);

	
	if(m_bLeft)
	{
		CPlateBxFinder		Finder(pDB->GetGirder(0));
		
		m_pBx		= pDB->GetGirder(0)->GetCrossBxAtMinFlangeWidth();
		m_pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);
		dLen		= pDB->GetLengthHDanDom(m_pBx, 0);
		if(!m_pSep)	dLen	= 0;

		pAPlateOutXL->NewSheet(pXL, "3.1 좌측 캔틸레버부", 0);
		pOut->SetXL(pXL, "3.1 좌측 캔틸레버부",		m_nSttRow, "A", "A", 1, BLACK, 11, TRUE);
		
		m_nSttRow++;

		//pOut->SetXL(pXL, "3.1 좌측 캔틸레버부",	m_nSttRow, "B", "B", NEXT);

		Dom.SetScaleDim(30);
		nType	= CalcFloor.GetGuardWallType(FLOOR_LEFT);
		DrawCalc.SetGuardType(nType);
		DrawCalc.DrawGuardWallInputLeft(&Dom, m_pSep, dLen);
		DrawCalc.DrawGuardDivideDimLeft(&Dom, m_pSep, dLen, TRUE);		
		nGuardType = pDB->GetValueTypeHDan(0, 1);
	}
	else
	{
		CPlateBxFinder		Finder(pDB->GetGirder(-1));
		m_pBx		= pDB->GetGirder(0)->GetCrossBxAtMinFlangeWidth();
		m_pSep		= pDB->GetCurGuardWall(CCentSeparation::RIGHT);
		dLen		= pDB->GetLengthHDanDom(m_pBx, pDB->GetQtyHDanNode()-1);
		if(!m_pSep)	dLen	= 0;

		pAPlateOutXL->NewSheet(pXL, "3.2 우측 캔틸레버부", 0);
		pOut->SetXL(pXL, "3.2 우측 캔틸레버부",		m_nSttRow, "A", "A", 1, BLACK, 11, TRUE);
		
		m_nSttRow += 2;

		//pOut->SetXL(pXL, "3.2 우측 캔틸레버부",	m_nSttRow, "B", "B", 2);

		Dom.SetScaleDim(30);
		nType	= CalcFloor.GetGuardWallType(FLOOR_RIGHT);
		DrawCalc.SetGuardType(nType);
		DrawCalc.DrawGuardWallInputRight(&Dom, m_pSep, dLen);
		DrawCalc.DrawGuardDivideDimRight(&Dom, m_pSep, dLen, TRUE);
		
		nGuardType = pDB->GetValueTypeHDan(pDB->GetQtyHDanNode() - 1, 1);
	}

	if(nGuardType == HT_CENTERGUARDFENCE)
	{
		if(pCalcData->m_bJungbunMugun == TRUE)
			dChulgun = dMugunUW;
	}

	pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 2, 12, 21, 32, TRUE, TRUE, TRUE);
	SetLines(NEXT);

	long nRowTemp1 = m_nSttRow;
	pOut->SetXL(pXL, "콘크리트 단위중량     =",			m_nSttRow, "A", "F", 0);
	pOut->SetXL(pXL, dChulgun,							m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "kN/m³",							m_nSttRow, "J", "J", NEXT);

	pOut->SetXL(pXL, "아스팔트 포장단위중량 = ",		m_nSttRow, "A", "F", 0);
	pOut->SetXL(pXL, dAsphalt,							m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "kN/m³",							m_nSttRow, "J", "J", NEXT);

	pOut->SetXL(pXL, "설 계 속 도           = ",		m_nSttRow, "A", "F", 0);
	pOut->SetXL(pXL, dV,								m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "VELOCITY");
	pOut->SetXL(pXL, "km/h",							m_nSttRow, "J", "J", NEXT);

	pOut->SetXL(pXL, "풍하중 강도           = ",		m_nSttRow, "A", "F", 0);
	pOut->SetXL(pXL, dWindLoadForce,					m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "WINDLOADFORCE");
	pOut->SetXL(pXL, "kN/m²",							m_nSttRow, "J", "J", NEXT);

	pOut->SetXL(pXL, "트럭의 1후륜하중(Pr)  = ",		m_nSttRow, "A", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPr,								m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "PR");
	pOut->SetXL(pXL, "kN",								m_nSttRow, "J", "J", NEXT);

	pOut->SetXL(pXL, "곡 선 반 경           = ",		m_nSttRow, "A", "F", 0);
	CString sText;
	if(dR==0)
		sText = "∞";
	else
		sText = Comma(dR);
	
	pOut->SetXL(pXL, sText,								m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "R", 0);
	pOut->SetXL(pXL, "m",								m_nSttRow, "J", "J", NEXT);

	pOut->SetXL(pXL, "충돌 하중 속도        = ",		m_nSttRow, "A", "F", 0);
	pOut->SetXL(pXL, dVelocity,							m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "DVELOCITY");
	pOut->SetXL(pXL, "km/h",							m_nSttRow, "J", "J", NEXT);

	if(bBangEm)
	{
		pOut->SetXL(pXL, "방음벽 높이           =",		m_nSttRow, "A", "F", 0);
		pOut->SetXL(pXL, dBangEmH,						m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "m",							m_nSttRow, "J", "J", NEXT);
	}

	if(bTank || bTrailer)
	{		
		pOut->SetXL(pXL, "탱크 설계속도         = ",	m_nSttRow, "A", "F", 0);
		pOut->SetXL(pXL, 40.0,							m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "VELOCITY_TANK");
		pOut->SetXL(pXL, "km/h",						m_nSttRow, "J", "J", NEXT);
	}	

	long nRowTemp2	= m_nSttRow;
	m_nSttRow = nRowTemp1;
	DimGuardWall();							// 방호벽제원

	if(m_nSttRow<=nRowTemp2)	m_nSttRow	= nRowTemp2+1;

	CalcDeadLoad();							// 사하중
	if(bWalkLoad)
	{
		CalcWalkLoad();			// 보도하중
	}
	else if(CalcFloor.IsLaneLoad(nFloorIdx))
	{
		double	dMoveL = toM(pCalcData->CALC_CANTILEVER_DATA[nFloorIdx].m_Move_L);
		if(dMoveL > 0)
		{
			CalcLiveLoad();			// 활하중
		}
		if(m_pSep)		CalcCollisionLoad();	// 충돌하중
		if(bTank)		CalcTankLoad();			// 탱크하중
		if(bTrailer)	CalcTrailerLoad();		// 트레일러 하중
		if(dMoveL > 0)
		{
			CalcCentriodLoad();
		}
	}
	CalcWindLoad();							// 풍하중, #APLATE-660, 풍하중에 대한 내용을 무조건 출력한다.
	if(pDB->m_nSlabTensionSize==0)
		CalcComposeLoad();					// 하중조합
	DesignOfCantilDanbu();					// 캔틸레버부 단부설계
	CheckMinThick(nFloorIdx);				// 최소두계 검토

// 	if(pDB->m_nSlabTensionSize==0)
// 		DesignOfSection(nFloorIdx);			// 단면검토
	FloorType eType = m_bLeft ? eFloor_LeftCantilever : eFloor_RightCantilever;
	if(pDB->m_nSlabTensionSize==0)
		CheckFloorRebarCalc(eType, CStringArray(), FALSE);

	if(pDB->m_bTendonInputComplete)
	{
		pOut->OnSheetRefMode(TRUE);
		if(m_bLeft)		m_nSttRow = 77;		///< Page2 시작번호
		else			m_nSttRow = 180;//179;	///< Page3 시작번호
		CString szSheetName = pXL->GetSheetName();
		pXL->SetActiveSheet("3.7 극한하중에 대한 단면검토");
		CalcComposeLoad();					///< 하중조합
		m_nUltSttRow[nFloorIdx] = m_nSttRow;///< 하중조합 이후 Row 저장
		if(m_bLeft)
		{
			if(m_nSttRow < 98)
			{
				pXL->DeleteRowLine(m_nSttRow, 98);
				pXL->DeleteRowLineEnd();
				pXL->InsertRowLine(m_nSttRow+78, 99-m_nSttRow);
			}
		}
		else
		{
			if(m_nSttRow < 201)//200)
			{
				pXL->DeleteRowLine(m_nSttRow, 201);//200);//196);//070410일자 수정...KB...
				pXL->DeleteRowLineEnd();
			}
		}

		pXL->DeleteRowLineEnd();
		pXL->SetActiveSheet(szSheetName);
		pOut->OnSheetRefMode(FALSE);
	}
}

void CXLDesignFloor::DimGuardWall()
{
	CAPlateCalcStd		*pCalStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcGeneral		CalcGeneral(pDataManage);
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	if(m_bLeft==FALSE)
		pGir = pDB->GetGirder(pDB->GetGirdersu()-1);

	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	long nSide		= m_bLeft ? 0 : 1;
	CString	sText, sText1, sCell1, sCell2;	
	long	nLIdx	= 1;
	long	nHIdx	= 1;
	long	nType	= pCalStd->m_pCalcFloor->GetGuardWallType(nSide);	
	double	dD1		= 0;
	double	dW1		= 0;
	double	dW2		= 0;
	double	dW3		= 0;
	double	dW4		= 0;
	double	dW5		= 0;

	double	dH1		= m_pSep ? toM(m_pSep->m_H1) : 0;
	double	dH2		= m_pSep ? toM(m_pSep->m_H2) : 0;
	double	dH3		= m_pSep ? toM(m_pSep->m_H3) : 0;	
	double	dWebT	= toM(pGir->GetThickFactByStation(m_pBx->GetStation(), G_W));
	double	dQuater	= toM(pGir->GetLengthSharpOnBx(m_pBx, TRUE))/2;
	double	dSharp	= toM(pGir->GetLengthSharpOnBx(m_pBx, TRUE));
	double	dTerm	= 0;
	double	dLEN	= 0;
	double	dDis	= toM(pDB->m_dWidthHunch);
	double	dWidth	= m_pSep ? toM(m_pSep->GetWidth()) : 0;
	double	dTc		= toM(pDB->m_dThickSlabBase);
	double	dTa		= 0;
	double	dPavement = toM(pDB->m_dThickPave);

	double	dDis_Car		= CalcGeneral.GetDBLOAD_Dis_CAR();
	double	dDis_Tank		= CalcGeneral.GetDBLOAD_Dis_Tank();
	double	dDis_Trailer	= CalcGeneral.GetDBLOAD_Dis_Trailer();

	double dWidthBindConc	= toM(pDB->m_BindConc.m_dWidth);
	
	if(m_bLeft)//좌측이면...
	{
		dLEN	= toM(pDB->GetLengthHDanDom(m_pBx, 0));
		if(nType==99)	dLEN	= 0;
		dD1		= m_pSep ? toM(m_pSep->m_D1) : 0;
		dW1		= m_pSep ? toM(m_pSep->m_W1) : 0;
		dW2		= m_pSep ? toM(m_pSep->m_W2) : 0;
		dW3		= m_pSep ? toM(m_pSep->m_W3) : 0;
		dW4		= m_pSep ? toM(m_pSep->m_W4) : 0;
		dW5		= m_pSep ? toM(m_pSep->m_W5) : 0;

		dTerm	= toM(pDB->m_dWidthSlabLeft);		
		dTa		= toM(pDB->m_dThickSlabLeft);
	}
	else//우측이면
	{	
		dLEN	= toM(pDB->GetLengthHDanDom(m_pBx, pDB->GetQtyHDanNode()-1));
		if(nType==99)	dLEN	= 0;
		dD1		= m_pSep ? dLEN - dWidth : 0;
		dW1		= m_pSep ? toM(m_pSep->m_W5) : 0;
		dW2		= m_pSep ? toM(m_pSep->m_W4) : 0;
		dW3		= m_pSep ? toM(m_pSep->m_W3) : 0;
		dW4		= m_pSep ? toM(m_pSep->m_W2) : 0;
		dW5		= m_pSep ? toM(m_pSep->m_W1) : 0;
		
		dTerm	= toM(pDB->m_dWidthSlabRight);		
		dTa		= toM(pDB->m_dThickSlabRight);
	}
	
	if(nType!=99)
	{
		if(dD1 != 0)
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			m_ArrayGuardWallTopL.Add(sText);
			pOut->SetXL(pXL, dD1,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
			CString sTemp = pOut->GetCellRef(sText);
		}
		if(dW1 != 0)
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			m_ArrayGuardWallTopL.Add(sText);
			pOut->SetXL(pXL, dW1,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}
		if(dW2 != 0)
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			m_ArrayGuardWallTopL.Add(sText);
			pOut->SetXL(pXL, dW2,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;

		}
		if(dW3 != 0)
		{
			sText.Format("L%d", nLIdx);
			m_nW3	= nLIdx;//군중하중 계산을 위해...
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			m_ArrayGuardWallTopL.Add(sText);
			pOut->SetXL(pXL, dW3,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}
		if(dW4 != 0)
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			m_ArrayGuardWallTopL.Add(sText);
			pOut->SetXL(pXL, dW4,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}
		if(dW5 != 0)
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			m_ArrayGuardWallTopL.Add(sText);
			pOut->SetXL(pXL, dW5,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}
	}

	double	dMomentLen	= 0;

	sText.Format("L%d", nLIdx);
	pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	if(pDB->IsTUGir())	dMomentLen	= dTerm-dLEN-dWidthBindConc/4;
	else				dMomentLen	= dTerm-dLEN-dQuater-dWebT/2;
	sText.Format("%g", dMomentLen);
	sText1.Format("L%d", nLIdx);
	m_ArrayGuardWallTopL.Add(sText1);
	pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText1);
	pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
	nLIdx++;

	if(!pDB->IsTUGir())
	{
		dMomentLen	= dTerm - (dWebT/2+dDis+dSharp);
		if(dMomentLen != 0)//L6
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g", dMomentLen);
			sText1.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}

		dMomentLen	= dDis+dQuater;
		if(dMomentLen != 0)	//헌치L7
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g", dMomentLen);
			sText1.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}

		dMomentLen	= dQuater;
		if(dMomentLen != 0)	//슬래브L8
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g",dMomentLen);
			sText1.Format("L%d", nLIdx);
			//m_ArrayGuardWallTopL.Add(sText1);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}
	}
	else
	{
		dMomentLen	= dTerm - dWidthBindConc/2;
		if(dMomentLen != 0)//L6
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g", dMomentLen);
			sText1.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}

		dMomentLen	= dWidthBindConc/4;
		if(dMomentLen != 0)//L7
		{
			sText.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g", dMomentLen);
			sText1.Format("L%d", nLIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 1);
			nLIdx++;
		}
	}
		
	if(!pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad)	//  DB하중 거리
	{
		long nSize = m_ArrayGuardWallTopL.GetSize();			
		
		if(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_L > 0)
		{
			sText.Format("Pr");
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			
			sText.Format("%g",dDis_Car);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "LPr");
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", NEXT);
			nLIdx++;
		}	

		if(pCalcData->m_bTank)	//탱크하중 작용시
		{
			sText.Format("Tk");
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%g",dDis_Tank);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "LTank");
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 0);
			nLIdx++;
			
			pOut->SetXL(pXL, "탱크하중 작용거리     =",		m_nSttRow, "A", "F", 0);
			sText.Format("=%s-%s", pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(nSize-1)), pOut->GetCellRef("LTank"));
			pOut->SetXL(pXL, sText,							m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "LTank_X");
			pOut->SetXL(pXL, "m",							m_nSttRow, "J", "J", NEXT);						
		}

		if(pCalcData->m_bTrailer)	//탱크트레일러하중 작용시
		{
			sText.Format("Tr");
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			
			sText.Format("%g",dDis_Trailer);
			pOut->SetXL(pXL, sText,					m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "LTrailer");
			pOut->SetXL(pXL, "m",					m_nSttRow, "W", "W", 0);
			nLIdx++;

			pOut->SetXL(pXL, "트레일러 작용거리     = ",	 m_nSttRow, "A", "F", 0);
			sText.Format("=%s-%s", pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(nSize-1)), pOut->GetCellRef("LTrailer"));
			pOut->SetXL(pXL, sText,							m_nSttRow, "G", "I", 0, RED,  9, FALSE, TA_CENTER, "LTrailer_X");
 			pOut->SetXL(pXL, "m",							m_nSttRow, "J", "J", NEXT);	
		}
	}

	SetLines(-nLIdx+1);

	if(dTa != 0)
	{
		sText.Format("H%d", nHIdx);
		pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%g", dTa);
		sText1.Format("H%d", nHIdx);
		m_ArrayGuardWallH.Add(sText1);
		pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "m",					m_nSttRow, "Q", "Q", NEXT);
		nHIdx++;
	}

	if(nType!=99)
	{
		if(dH1 != 0)
		{
			sText.Format("H%d", nHIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g", dH1);
			sText1.Format("H%d", nHIdx);
			m_ArrayGuardWallH.Add(sText1);
			pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "Q", "Q", NEXT);
			nHIdx++;
		}
		if(dH2 != 0)
		{
			sText.Format("H%d", nHIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g", dH2);
			sText1.Format("H%d", nHIdx);
			m_ArrayGuardWallH.Add(sText1);
			pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "Q", "Q", NEXT);
			nHIdx++;

		}
		if(dH3 != 0)
		{
			sText.Format("H%d", nHIdx);
			pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%g", dH3);
			sText1.Format("H%d", nHIdx);
			m_ArrayGuardWallH.Add(sText1);
			pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, sText1);
			pOut->SetXL(pXL, "m",					m_nSttRow, "Q", "Q", NEXT);
			nHIdx++;
		}
	}
	if(dTc != 0)
	{
		sText.Format("H%d", nHIdx);
		pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%g", dTc);
		sText1.Format("H%d", nHIdx);
		m_ArrayGuardWallH.Add(sText1);
		pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "m",					m_nSttRow, "Q", "Q", NEXT);
		nHIdx++;
	}
	if(dPavement != 0)
	{
		sText.Format("H%d", nHIdx);
		pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%g", dPavement);
		sText1.Format("H%d", nHIdx);
		m_ArrayGuardWallH.Add(sText1);
		pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "m",					m_nSttRow, "Q", "Q", NEXT);
		nHIdx++;
	}

	if(pDB->IsTUGir())
	{
		sText.Format("H%d", nHIdx);
		pOut->SetXL(pXL, sText,					m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",					m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%g", dTc - toM(pDB->m_BindConc.m_dDeep));
		sText1.Format("H%d", nHIdx);
		pOut->SetXL(pXL, sText,					m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "m",					m_nSttRow, "Q", "Q", NEXT);
		nHIdx++;
	}
	long nTemp = abs(nHIdx - nLIdx);
	SetLines(nTemp+1);
}

// 1) 사하중
void CXLDesignFloor::CalcDeadLoad()
{
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CStringArray		ArrDeadLoad;

	if(m_bLeft)		pCalcFloor->CalcSlabLeftDeadLoad(ArrDeadLoad);
	else			pCalcFloor->CalcSlabRightDeadLoad(ArrDeadLoad);
	long	nSize	= ArrDeadLoad.GetSize();
	double	dIdx	= 1;
	CString	sText;

	sText.Format("%s 고 정 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C");
	pOut->SetLineBoxText(pXL, "NO",						m_nSttRow, "C", "D");
	pOut->SetLineBoxText(pXL, "제 원",	m_nSttRow, "E", "P");
	pOut->SetLineBoxText(pXL, "중 량 (kN)",				m_nSttRow, "Q", "S");
	pOut->SetLineBoxText(pXL, "거 리( m )",				m_nSttRow, "T", "V");
	pOut->SetLineBoxText(pXL, "모멘트(kN·m)",			 m_nSttRow, "W", "Z");
	m_nSttRow++;

	long nSaveRow1 = m_nSttRow;
	
	for(long n = 0; n < nSize-3; n+=4)
	{
		pOut->SetLineBoxText(pXL, dIdx,						m_nSttRow, "C", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetLineBoxText(pXL, ArrDeadLoad.GetAt(n),		m_nSttRow, "E", "P", 1, BLACK, 9, FALSE, TA_LEFT);
		sText.Format("%.6f", atof(ArrDeadLoad.GetAt(n+1)));
		pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER, "NUM1");
		sText.Format("%.6f", atof(ArrDeadLoad.GetAt(n+2)));
		pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "T", "V", 1, BLACK, 9, FALSE, TA_CENTER, "NUM2");
		sText.Format("=%s*%s",pOut->GetCellRef("NUM1"),pOut->GetCellRef("NUM2"));
		pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "W", "Z");//ArrDeadLoad.GetAt(n+3)

		dIdx++;m_nSttRow++;
	}
	pOut->SetLineBoxText(pXL, "Total",						m_nSttRow, "C", "D");
	pOut->SetLineBoxText(pXL, "",							m_nSttRow, "E", "P");
	sText.Format("=SUM(Q%d:Q%d)",nSaveRow1,m_nSttRow-1);
	pOut->SetLineBoxText(pXL, sText ,	m_nSttRow, "Q", "S");//ArrDeadLoad.GetAt(nSize-2)
	pOut->SetLineBoxText(pXL, "",							m_nSttRow, "T", "V");

	sText.Format("=SUM(W%d:W%d)",nSaveRow1,m_nSttRow-1);//ArrDeadLoad.GetAt(nSize-1)
	m_nDanRow[m_bLeft?2:3] = m_nSttRow;
	pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "W", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "DEADLOAD", 3);
	SetLines(1);
	sText.Format("=\"∴ Md = \"& ROUND(%s,3)&\" kN·m \"", pOut->GetCellRef("DEADLOAD"));
	pOut->SetXL(pXL, sText,				m_nSttRow, "V", "Z");
	m_nSttRow+=2;

}

// 2) 활하중
void CXLDesignFloor::CalcLiveLoad()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	long	nIndex	= m_bLeft ? 0 : 1;
	double	dX		= 0;
	
	CString	sText;
	if(pCalcData->DESIGN_FLOOR_DATA[nIndex].m_bWalkLoad) return;

	dX = toM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_L);
	if(dX < 0) return;
	
	sText.Format("%s 활 하 중",  m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);

	if(dX>1.8)
		pOut->SetXL(pXL, "◎ 활하중-1",					m_nSttRow, "C", "C", NEXT);
	
	pOut->SetXL(pXL, "① 윤하중 분포폭",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.6.1.4)",			m_nSttRow, "T", "T", NEXT);
	SetLines(2);
	pOut->SetXL(pXL, "· 여기서, ",							m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "X =",									m_nSttRow, "F", "F", 0);
	
	long nSize = m_ArrayGuardWallTopL.GetSize();	

	sText.Format("=%s-%s", pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(nSize-1)), pOut->GetCellRef("LPr"));

	pOut->SetXL(pXL, sText,									m_nSttRow, "G", "I", 0,	BLACK,  9, FALSE, TA_CENTER, "MOVEL");
	pOut->SetXL(pXL, "m",									m_nSttRow, "J", "J", 0,	BLACK, 9, FALSE, TA_CENTER);
	SetLines(-2);
	pOut->SetXL(pXL, "E",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "0.8 × X + 1.14",						m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"0.8 × \"&ROUND(%s,3)&\" + 1.14 \"",	pOut->GetCellRef("MOVEL"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "K", "K");

	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.8*%s + 1.14", pOut->GetCellRef("MOVEL"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "E");
	pOut->SetXL(pXL, "m",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(2);
	pOut->SetXL(pXL, "② 충 격 계 수",						m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 2.1.4)",				m_nSttRow, "T", "T", NEXT);
	pOut->SetXL(pXL, "i",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, " 15 / (40 + L)",						m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" 15 / ( 40 + \"&ROUND(%s,3)&\" )\"",	pOut->GetCellRef("MOVEL"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=15/ (40 + %s)",				pOut->GetCellRef("MOVEL"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "IMPACTCOEFF");
	pOut->SetXL(pXL, 0.3,									m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "AIMPACTCOEFF", 3);

	sText.Format("=IF(%s > %s, \">\", \"<\")",				pOut->GetCellRef("IMPACTCOEFF"), pOut->GetCellRef("AIMPACTCOEFF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", 1, RED,   9, TRUE,  TA_CENTER);

	pOut->SetXL(pXL, "∴i",									m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=MIN(%s, %s)",							pOut->GetCellRef("IMPACTCOEFF"), pOut->GetCellRef("AIMPACTCOEFF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "IMPACT");
	pOut->SetXL(pXL, "적용",								m_nSttRow, "I", "I");

	pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, " Pr / E × X × ( 1 + i )",			m_nSttRow, "F", "F");
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" ( \"&ROUND(%s,3)&\" / \"&ROUND(%s,3)&\" ) × \"&ROUND(%s, 3)&\" × ( 1 + \"&ROUND(%s,3)&\" ) \"",
					pOut->GetCellRef("PR"), pOut->GetCellRef("E"), pOut->GetCellRef("MOVEL"), pOut->GetCellRef("IMPACT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	m_nDanRow[nIndex] = m_nSttRow;
	sText.Format("=%s/%s*%s*(1+%s)",
					pOut->GetCellRef("PR"), pOut->GetCellRef("E"), pOut->GetCellRef("MOVEL"), pOut->GetCellRef("IMPACT"));
	pOut->SetXL(pXL, sText, m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, dX>1.8?"LIVELOAD1":"LIVELOAD");//pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml/pow(10,6)
	pOut->SetXL(pXL, "kN·m / m",							m_nSttRow, "I", "I", 2);

	if(dX>1.8)
	{
		if(dX>1.8)
		pOut->SetXL(pXL, "◎ 활하중-2",					m_nSttRow, "C", "C", NEXT);		

		pOut->SetXL(pXL, "① 윤하중 분포폭",					m_nSttRow, "C", "C", 0);
		pOut->SetXL(pXL, "(☞ 도.설.기 3.6.1.4)",			m_nSttRow, "T", "T", NEXT);
		SetLines(2);
		pOut->SetXL(pXL, "· 여기서, ",							m_nSttRow, "C", "C", 0);
		pOut->SetXL(pXL, "X =",									m_nSttRow, "F", "F", 0);
		
		long nSize = m_ArrayGuardWallTopL.GetSize();	

		sText.Format("=(%s-%s)-1.8", pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(nSize-1)), pOut->GetCellRef("LPr"));

		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "I", 0,	BLACK,  9, FALSE, TA_CENTER, "MOVEL_");
		pOut->SetXL(pXL, "m",									m_nSttRow, "J", "J", 0,	BLACK, 9, FALSE, TA_CENTER);
		SetLines(-2);
		pOut->SetXL(pXL, "E",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "0.8 × X + 1.14",						m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"0.8 × \"&ROUND(%s,3)&\" + 1.14 \"",	pOut->GetCellRef("MOVEL_"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "K", "K");

		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.8*%s + 1.14", pOut->GetCellRef("MOVEL_"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "E_");
		pOut->SetXL(pXL, "m",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(2);
		pOut->SetXL(pXL, "② 충 격 계 수",						m_nSttRow, "C", "C", 0);
		pOut->SetXL(pXL, "(☞ 도.설.기 2.1.4)",				m_nSttRow, "T", "T", NEXT);
		pOut->SetXL(pXL, "i",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, " 15 / (40 + L)",						m_nSttRow, "F", "F", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" 15 / ( 40 + \"&ROUND(%s,3)&\" )\"",	pOut->GetCellRef("MOVEL_"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=15/ (40 + %s)",				pOut->GetCellRef("MOVEL_"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "IMPACTCOEFF_");
		pOut->SetXL(pXL, 0.3,									m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "AIMPACTCOEFF_", 3);

		sText.Format("=IF(%s > %s, \">\", \"<\")",				pOut->GetCellRef("IMPACTCOEFF_"), pOut->GetCellRef("AIMPACTCOEFF_"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", 1, RED,   9, TRUE,  TA_CENTER);

		pOut->SetXL(pXL, "∴i",									m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=MIN(%s, %s)",							pOut->GetCellRef("IMPACTCOEFF_"), pOut->GetCellRef("AIMPACTCOEFF_"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "IMPACT_");
		pOut->SetXL(pXL, "적용",								m_nSttRow, "I", "I");

		pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, " Pr / E × X × ( 1 + i )",			m_nSttRow, "F", "F");
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" ( \"&ROUND(%s,3)&\" / \"&ROUND(%s,3)&\" ) × \"&ROUND(%s, 3)&\" × ( 1 + \"&ROUND(%s,3)&\" ) \"",
						pOut->GetCellRef("PR"), pOut->GetCellRef("E_"), pOut->GetCellRef("MOVEL_"), pOut->GetCellRef("IMPACT_"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("=%s/%s*%s*(1+%s)",
						pOut->GetCellRef("PR"), pOut->GetCellRef("E_"), pOut->GetCellRef("MOVEL_"), pOut->GetCellRef("IMPACT_"));
		pOut->SetXL(pXL, sText, m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "LIVELOAD2");//pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml/pow(10,6)
		pOut->SetXL(pXL, "kN·m / m",							m_nSttRow, "I", "I", NEXT);


		pOut->SetXL(pXL, "적용",								m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"활하중 1 + 활하중 2 = \"&%s&\" + \"&%s", pOut->GetCellRef("LIVELOAD1"), pOut->GetCellRef("LIVELOAD2"));
		pOut->SetXL(pXL, sText,	m_nSttRow, "F", "F", NEXT);		
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		m_nDanRow[nIndex] = m_nSttRow;
		sText.Format("=%s+%s", pOut->GetCellRef("LIVELOAD1"), pOut->GetCellRef("LIVELOAD2"));
		pOut->SetXL(pXL, sText, m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "LIVELOAD");//pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml/pow(10,6)
		pOut->SetXL(pXL, "kN·m / m",							m_nSttRow, "I", "I", NEXT);
	}
}

// 3) 보도하중
void CXLDesignFloor::CalcWalkLoad()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CCentSeparation		*pSep			= pDB->GetCurGuardWall(m_bLeft ? CCentSeparation::LEFT : CCentSeparation::RIGHT);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText("");
	long	nIndex	= m_bLeft ? 0 : 1;
	double	dMl		= 0;
	//double	dMco	= 0;
	double	dHoriF	= tokNPM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Nangan_HoriF);
	double  dVeriF	= tokNPM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Nangan_VerF);
	BOOL	bNanGan	= pCalcData->DESIGN_FLOOR_DATA[nIndex].m_bNanGan;

	double	dH		= pSep ? toM(pSep->GetHeight()) : 0;
	if(bNanGan)	dH  += toM(pCalcData->DESIGN_FLOOR_DATA[nIndex].m_dNanGanHeight);

	double dL = pCalcGeneral->GetWidthBodoDeadLoad(m_bLeft, m_pBx);

	dMl		= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml);
	//dMco	= dHoriF * dH;
	
	if(!pCalcData->DESIGN_FLOOR_DATA[nIndex].m_bWalkLoad || dL < 0) return;

	sText.Format("%s 군 중 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 2.1.3)",				m_nSttRow, "T", "T", NEXT);
	
	pOut->SetXL(pXL, "Ml",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "5 kN/㎡ × L²/ 2",					m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%.3f × %.3f²/ 2", 5.00, toM(dL));
	pOut->SetXL(pXL, sText,									m_nSttRow, "L", "L");
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dMl,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "WALKLOAD");
	pOut->SetXL(pXL, "kN·m/m",							m_nSttRow, "I", "I", NEXT);
	pOut->SetXL(pXL, "· 보도등에 재하하는 등분포하중에는 충격계수를 고려하지 아니한다.", m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 2.1.3)",				m_nSttRow, "T", "T", 2);

	sText.Format("%s 군중에 의한 추력 또는 차량의 2차 충돌하중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 2.4.3.3)",		m_nSttRow, "T", "T", NEXT);
	pOut->SetXL(pXL, "· 수평충돌력 H와 수직력 V가 상단부와 난간 정상부에 작용하는 것으로 한다.", m_nSttRow, "C", "C");

	pOut->SetXL(pXL, "H",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dHoriF,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "HORIF");
	pOut->SetXL(pXL, "kN / m",							m_nSttRow, "I", "I", NEXT);
	pOut->SetXL(pXL, "V",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dVeriF,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "VERIF");
	pOut->SetXL(pXL, "kN / m",							m_nSttRow, "I", "I", NEXT);
	pOut->SetXL(pXL, "Mco",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=%s", pOut->GetCellRef("HORIF"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "W1");
	pOut->SetXL(pXL, "×",								 m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);	
	pOut->SetXL(pXL, dH,								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "W2");
	pOut->SetXL(pXL, "+",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("VERIF"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "W3");
	pOut->SetXL(pXL, "×",								 m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);

	long nSize = m_ArrayGuardWallTopL.GetSize();
	if(m_nW3>0)
	{
		sText.Format("=(SUM(%s:%s)+%s/2)",pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(m_nW3)),
			pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(nSize-1)), pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(m_nW3-1)));
	}
	else
		sText.Format("%.3f", 0.0);

	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "W4");
	pOut->SetXL(pXL, "=",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=%s*%s+%s*%s", pOut->GetCellRef("W1"), pOut->GetCellRef("W2"),
					pOut->GetCellRef("W3"),pOut->GetCellRef("W4"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "COLLISIONLOAD_WALK");
	pOut->SetXL(pXL, "kN·m/m",							 m_nSttRow, "T", "T", 2);

}

// 4) 충돌하중
void CXLDesignFloor::CalcCollisionLoad()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCentSeparation		*pSep			= m_pSep;
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	long	nFloorIndex	= m_bLeft ? 0 : 1;
	if(m_bLeft==FALSE)	pGir = pDB->GetGirder(pDB->GetGirdersu()-1);
	double	dMoveL		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_L;
	double	dVelocity	= 0;
	BOOL	bNanGan		= pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_bNanGan;
	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_bWalkLoad;
	double	dHeightGuard	= pSep ? pSep->GetHeight() : 0;
	CString	sText;

	// APLATE-727
//	if(dMoveL <= 0)	return;
	if(bWalkLoad)	return;

	//  1 -> 방호벽
	dVelocity	= pCalcData->m_dSpeed;
	sText.Format("%s 충 돌 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.해 2.4.3.5)",		m_nSttRow, "T", "T", NEXT);

	pOut->SetXL(pXL, "① 수평충돌력 H가 노면상 1m 높이에 교축을 따라 1m 마다 작용시",	m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "H",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, " (  V  /  60 )²× 7500 ＋ 2500",	m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" ( \"&ROUND(%s,3)&\" / 60 )²× 7500 ＋ 2500\"",	pOut->GetCellRef("DVELOCITY"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s/60)^2 * 7500 ＋ 2500",	pOut->GetCellRef("DVELOCITY"));
	pOut->SetXL(pXL, sText,						m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER,"DVELOCITY1");
	pOut->SetXL(pXL, "N / m",							m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s / 1000",	pOut->GetCellRef("DVELOCITY1"));
	pOut->SetXL(pXL, sText,					m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "HORIFORCE");
	pOut->SetXL(pXL, "kN/m",							m_nSttRow, "O", "O", NEXT);

	long nSize = m_ArrayGuardWallH.GetSize();
	pOut->SetXL(pXL, "· 작용높이 h = 1.0 + 포장두께 =",	m_nSttRow, "D", "D", 0);
	sText.Format("=\" 1.0  +  \"&ROUND(%s,3)", pOut->GetCellRef(m_ArrayGuardWallH.GetAt(nSize-1)));
	pOut->SetXL(pXL, sText,								m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=1.0+%s", pOut->GetCellRef(m_ArrayGuardWallH.GetAt(nSize-1)));
	pOut->SetXL(pXL, sText,								m_nSttRow, "Q", "S", 0, BLACK,  9, FALSE, TA_CENTER, "HEIGHT");
	pOut->SetXL(pXL, "m",								m_nSttRow, "T", "T", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Mco",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)", pOut->GetCellRef("HORIFORCE"), pOut->GetCellRef("HEIGHT"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s * %s",pOut->GetCellRef("HORIFORCE"), pOut->GetCellRef("HEIGHT"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "MOMENT1");
	pOut->SetXL(pXL, "kN·m/m",							m_nSttRow, "O", "O", NEXT);

	pOut->SetXL(pXL, "② 방호벽 상단에 횡방향으로 H = 10 kN/m 의 수평하중 재하시",	m_nSttRow, "C", "C", 1);

	pOut->SetXL(pXL, "H",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 10,								m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "HORIFORCE1");
	pOut->SetXL(pXL, " kN/m",							m_nSttRow, "I", "I", NEXT);

	pOut->SetXL(pXL, "Mco",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" \"&ROUND(%s,3)&\" × \"&ROUND(SUM(%s:%s),3)", pOut->GetCellRef("HORIFORCE1"), pOut->GetCellRef(m_ArrayGuardWallH.GetAt(1)),
		pOut->GetCellRef(m_ArrayGuardWallH.GetAt(nSize-3)));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*SUM(%s:%s)", pOut->GetCellRef("HORIFORCE1"), pOut->GetCellRef(m_ArrayGuardWallH.GetAt(1)),
		pOut->GetCellRef(m_ArrayGuardWallH.GetAt(nSize-3)));
	pOut->SetXL(pXL, sText,							m_nSttRow, "K", "M", 0, BLACK,  9, FALSE, TA_CENTER, "MOMENT2");
	pOut->SetXL(pXL, "kN·m/m",						m_nSttRow, "N", "N", 2);

	sText.Format("=IF( OR(%s>200, %s=0.0),\"· 곡선반경 R ＞ 200m 이므로 ①과 ②에 의해 계산된 값중에서 큰 값을 적용 \",\"곡선반경 R ≤ 200m 이므로 ①과 ②에 의해 계산된 값중에서 큰 값의 2배를 적용\")",
					pOut->GetCellRef("R"), pOut->GetCellRef("R"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C");
	pOut->SetXL(pXL, "Mco",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF( OR(%s>200, %s=0.0),MAX(%s,%s),2.0*MAX(%s,%s))",
				pOut->GetCellRef("R"), pOut->GetCellRef("R"), pOut->GetCellRef("MOMENT1"), pOut->GetCellRef("MOMENT2"), pOut->GetCellRef("MOMENT1"), pOut->GetCellRef("MOMENT2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "COLLISIONLOAD");
	pOut->SetXL(pXL, "kN·m/m",							m_nSttRow, "I", "I", 2);
}

// 5) 탱크하중
void CXLDesignFloor::CalcTankLoad()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	long	nIndex		= m_bLeft ? 0 : 1;

	double	dTankWeight	= pCalcData->m_TankWeight;
	double	dL_Tank		= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Tank_L;  //Round(toM(dTerm - dSharp/2.0 - (350.0 + dLen)),2);
	double	dTankEdit1	= pCalcData->m_TankEdit1;	///< 탱크 궤도 접지 길이

	if(dL_Tank < 0)		return;

	CString	sText;
	sText.Format("%s 탱 크 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);

	pOut->SetXL(pXL, "· 탱크 총 중량 :",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, dTankWeight,							m_nSttRow, "H", "J", 0, BLACK,  9, FALSE, TA_CENTER, "TANKWEIGHT");
	pOut->SetXL(pXL, "kN",									m_nSttRow, "K", "K", 0);
	pOut->SetXL(pXL, "· 궤도 접지 길이 :",					m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, dTankEdit1,							m_nSttRow, "S", "U", 0, BLACK,  9, FALSE, TA_CENTER, "TANKEDIT1");
	pOut->SetXL(pXL, "m",									m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "여기서, L =",							m_nSttRow, "D", "D", 0);

	sText.Format("=%s",pOut->GetCellRef("LTank_X"));	
	pOut->SetXL(pXL, sText,								m_nSttRow, "G", "I", 0, BLACK,  9, FALSE, TA_CENTER, "TANKL");
	pOut->SetXL(pXL, "m",									m_nSttRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Pr",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "W  /  2  /  L ( 탱크접지길이 )",		m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" \"&ROUND(%s,3)&\"  /  2  /  \"&ROUND(%s,3)", pOut->GetCellRef("TANKWEIGHT"), pOut->GetCellRef("TANKEDIT1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "O", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("=%s/2.0/%s", pOut->GetCellRef("TANKWEIGHT"), pOut->GetCellRef("TANKEDIT1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "TANKPR");
	pOut->SetXL(pXL, "kN/m",								m_nSttRow, "I", "I", NEXT);
	pOut->SetXL(pXL, "충격계수 i = 0.15 ( 충격계수는  활하중의 15%를 고려한다.)", m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Pr  ×  L ( 모멘트길이 )  × (1 + i)",m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" \"&ROUND(%s,3)&\"  ×  \"&ROUND(%s,3)&\"  ×  ( 1 + 0.15 )\"", pOut->GetCellRef("TANKPR"), pOut->GetCellRef("TANKL"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=%s*%s*(1 + 0.15 )", pOut->GetCellRef("TANKPR"), pOut->GetCellRef("TANKL"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "TANKLOAD");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", 2);

}

// 6) 트레일러 하중
void CXLDesignFloor::CalcTrailerLoad()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	long	nFloorIndex	= m_bLeft ? 0 : 1;
	double	dTrailer	= max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
	double	dL			= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Trailer_L; //Round(toM(dTerm - dSharp/2 - (487.5 + dLen)),3);
	CString	sText;
	

	sText.Format("%s 탱크 트레일러 하중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);

	if(dL < 0)
	{
		pOut->SetXL(pXL, "",									m_nSttRow, "Z", "Z", NEXT);
		pOut->SetXL(pXL, "- 탱크트레일러 하중 계산 필요없음.",	m_nSttRow, "D", "D", 2);
		return;
	}
	pOut->SetXL(pXL, "( 700 kN :  Pr = ",						m_nSttRow, "J", "J", 0);
	pOut->SetXL(pXL, dTrailer,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "TRAILERPR");
	pOut->SetXL(pXL, "kN )",									m_nSttRow, "R", "R", NEXT);
	pOut->SetXL(pXL, "여기서, L =",								m_nSttRow, "D", "D", 0);
	
	sText.Format("=%s",pOut->GetCellRef("LTrailer_X"));	

	pOut->SetXL(pXL, sText,										m_nSttRow, "G", "I", 0, BLACK,  9, FALSE, TA_CENTER, "TRAILERL");
	pOut->SetXL(pXL, "m",										m_nSttRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "· 윤하중 분포폭",						m_nSttRow, "C", "C", NEXT);

	sText.Format("=\" E   =  0.8  ×  L  +  1.14  =  0.8  ×  \"&ROUND(%s,3)&\" +  1.14\"", pOut->GetCellRef("TRAILERL"));
	pOut->SetXL(pXL, sText,										m_nSttRow, "D", "D", NEXT);
	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("=0.8*%s+1.14", pOut->GetCellRef("TRAILERL"));
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "TRAILERE");
	pOut->SetXL(pXL, "m",										m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Ml+i",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Pr × L / E × ( 1 + i )",				m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",										m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" /  \"&ROUND(%s,3)&\"  ×  ( 1 + 0.15 )\"", pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILERL"), pOut->GetCellRef("TRAILERE"));
	pOut->SetXL(pXL, sText,										m_nSttRow, "M", "M", NEXT);
	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("=%s*%s/%s*(1 + 0.15)", pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILERL"), pOut->GetCellRef("TRAILERE"));
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "TRAILERLOAD");
	pOut->SetXL(pXL, "kN·m",									m_nSttRow, "I", "I", 2);

}

// 7) 풍하중
void CXLDesignFloor::CalcWindLoad()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	long	nFloorIndex		= m_bLeft ? 0 : 1;
	long	nSize			= pDB->GetQtyHDanNode();
	double	dPaveT			= pDB->m_dThickPave;
	double	dWalkLoadH		= pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_dWalkLoadH;
	double	dHH				= toM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Wind_H);
	BOOL	bWalkLoad		= pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_bWalkLoad;
	
	CString	sText;
	long nTypeHDan = m_bLeft ? pDB->GetValueTypeHDan(0, 1) : pDB->GetValueTypeHDan(nSize-1, 1);
	//	if(nTypeHDan == 1 || bBangEm || bNangan)//  1 -> 방호벽ㅇ

	// #APLATE-660, 풍하중에 대한 내용이 없더라도 무조건 출력한다.
	if(nTypeHDan == 0)
	{
		sText.Format("%s 풍 하 중", m_ArrayNumber.GetAt(m_nNumber++));
		pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "※ 풍하중에 대한 내용이 없습니다.",   m_nSttRow, "D", "D", NEXT);
		SetLines(NEXT);
		return ;
	}
	else if(nTypeHDan == 1)
	{
		sText.Format("%s 풍 하 중", m_ArrayNumber.GetAt(m_nNumber++));
		pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "Pw",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("WINDLOADFORCE"));
		pOut->SetXL(pXL, sText,  								m_nSttRow, "F", "G", 0,BLACK,9,FALSE,TA_CENTER,"PW1");
		pOut->SetXL(pXL, "kN/m² × ",  						m_nSttRow, "H", "J", 0,BLACK,9,FALSE,TA_CENTER);
		pOut->SetXL(pXL, dHH,									m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "WINDH");
		pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s * %s",pOut->GetCellRef("PW1"),pOut->GetCellRef("WINDH"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK,  9, FALSE, TA_CENTER, "WINDPW");
		pOut->SetXL(pXL, "kN/m",								m_nSttRow, "P", "Q", NEXT);

		pOut->SetXL(pXL, "Mw",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("WINDPW"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN/m² × ",  						  m_nSttRow, "H", "J", 0,BLACK,9,FALSE,TA_CENTER);

		if(bWalkLoad)	sText.Format("=%s/2 + %g", pOut->GetCellRef("WINDH"), toM(dWalkLoadH));
		else			sText.Format("=%s/2 + %g", pOut->GetCellRef("WINDH"), toM(dPaveT));
		pOut->SetXL(pXL, sText,									m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "WINDMOMENTH");

		pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s * %s", pOut->GetCellRef("WINDPW"), pOut->GetCellRef("WINDMOMENTH"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, RED,   9, FALSE, TA_CENTER, "WINDLOAD");
		pOut->SetXL(pXL, "kN·m/m",								m_nSttRow, "P", "Q", 2);
	}
}

// 8) 원심하중
void CXLDesignFloor::CalcCentriodLoad()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	long	nFloorIndex	= m_bLeft ? 0 : 1;
	double	dE			= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_E;
	BOOL	bTank		= pCalcData->m_bTank;
	BOOL	bTrailer	= pCalcData->m_bTrailer;
	
	CString	sText;

	// #APLATE-660, 원심하중에 대한 내용이 없어도 무조건 출력한다.
	double dR = pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Cent_R;
	if(dR <= 0)
	{
		sText.Format("%s 원 심 하 중", m_ArrayNumber.GetAt(m_nNumber++));
		pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "※ 원심하중에 대한 내용이 없습니다.", m_nSttRow, "D", "D", NEXT);
		SetLines(NEXT);
		return ;
	}

	sText.Format("%s 원 심 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "- 교면상 1.8 M 높이에 작용하는 것으로 한다.",	m_nSttRow, "G", "G", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 2.1.17)",			m_nSttRow, "T", "T", NEXT);
	if(bTank || bTrailer)
		pOut->SetXL(pXL, "· 일반활하중 작용시",			m_nSttRow, "C", "C", NEXT);
	
	pOut->SetXL(pXL, "CF",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "0.79  ×  (  V² /  R  ) (%)",		m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" 0.79 ×  (  \"&ROUND(%s,3)&\"²  /  \"&ROUND(%s, 3)&\"  ) (%%) \"", pOut->GetCellRef("VELOCITY"), pOut->GetCellRef("R"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=0.79*%s^2/%s", pOut->GetCellRef("VELOCITY"), pOut->GetCellRef("R"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "CF");
	pOut->SetXL(pXL, "(%)",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Pcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( Pr / E ) × CF / 100.0",			m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" ( \"&ROUND(%s,3)&\" / \"&ROUND(%.3f,3)&\" ) × \"&ROUND(%s,3)&\" / 100.0\"", pOut->GetCellRef("PR"), toM(dE), pOut->GetCellRef("CF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=%s/%g*%s/100.0", pOut->GetCellRef("PR"), toM(dE), pOut->GetCellRef("CF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "PCF");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

	pOut->SetXL(pXL, "Mcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Pcf × 1.8",							m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,3)&\" × 1.8\"", pOut->GetCellRef("PCF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*1.8", pOut->GetCellRef("PCF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "CENTLOAD");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

	double	dL_Tank		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Tank_L;
	if(bTank && dL_Tank > 0)
	{
		pOut->SetXL(pXL, "· 탱크하중 작용시",					m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "CF",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "0.79  ×  (  V² /  R  ) (%)",		m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" 0.79 ×  (  \"&ROUND(%s,3)&\"²  /  \"&ROUND(%s, 3)&\"  ) (%%) \"", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.79*%s^2/%s", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "CF_TANK");
		pOut->SetXL(pXL, "(%)",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "Pcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, " Pr × CF / 100.0",					m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" / 100.0\"", pOut->GetCellRef("TANKPR"), pOut->GetCellRef("CF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*%s/100", pOut->GetCellRef("TANKPR"), pOut->GetCellRef("CF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "PCF_TANK");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

		pOut->SetXL(pXL, "Mcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Pcf × 1.8",							m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"\"&ROUND(%s,3)&\" × 1.8\"", pOut->GetCellRef("PCF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*1.8", pOut->GetCellRef("PCF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "CENTLOAD_TANK");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);
	}

	double	dL_Trailer		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Trailer_L;	
	if(bTrailer && dL_Trailer > 0)
	{	
		pOut->SetXL(pXL, "· 탱크트레일러 하중 작용시",			m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "CF",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "0.79  ×  (  V² /  R  ) (%)",		m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" 0.79 ×  (  \"&ROUND(%s,3)&\"²  /  \"&ROUND(%s, 3)&\"  ) (%%) \"", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.79*%s^2/%s", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "CF_TRAILER");
		pOut->SetXL(pXL, "(%)",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "Pcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, " Pr/E × CF / 100.0",					m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" \"&ROUND(%s,3)&\" /  \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" / 100.0\"", pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILERE"), pOut->GetCellRef("CF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/%s*%s/100", pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILERE"), pOut->GetCellRef("CF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "PCF_TRAILER");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

		pOut->SetXL(pXL, "Mcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Pcf × 1.8",							m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"\"&ROUND(%s,3)&\" × 1.8\"", pOut->GetCellRef("PCF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*1.8", pOut->GetCellRef("PCF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "CENTLOAD_TRAILER");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);
	}

	pOut->SetXL(pXL, "",										m_nSttRow, "Z", "Z", NEXT);
}

// 9) 하중조합
void CXLDesignFloor::CalcComposeLoad()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	
	long	nFloorIndex		= m_bLeft ? FLOOR_LEFT : FLOOR_RIGHT;
	double	dD1				= m_pSep ? m_pSep->m_D1 : 0;
	double	dWidth			= m_pSep ? m_pSep->GetWidth() : 0;
	double	dSharp			= pGir->GetLengthSharpOnBx(m_pBx, TRUE);
	double	dTerm			= pDB->m_dWidthSlabLeft;
	if(nFloorIndex==1)	dTerm  = pDB->m_dWidthSlabRight;
	
	double	dD   = tokNM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Stop_M);			// 사하중
	double	dCo  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Coll_Mo);			// 충돌하중
	double	dCF  = tokNM((pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Cent_Mcf));		// 원심하중
	double	dCF2 = (pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Cent_Mcf_Tank);			// 원심하중(탱크궤도하중일때)
	double	dCF3 = (pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Cent_Mcf_Trailer);		// 원심하중(탱크트레일러하중일때)
	double	dLi  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml);			// 활하중
	double	dLi2 = tokNM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Tank);		// 활하중(탱크궤도하중일때)
	double	dLi3 = tokNM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Trailer);	// 활하중(탱크트레일러하중일때)
	double	dW   = tokNM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Wind_Mw);			// 풍하중
	double	dMu  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_Mu);
	double	dMu_Use	= Round(dD + dLi + 0.3 * dW, 3);

	CString	sText;
	sText.Format("%s 하 중 조 합", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 2.2.3.2)",			m_nSttRow, "T", "T", NEXT);

	pOut->SetXL(pXL, "- 최대 계수 모멘트 계산",			m_nSttRow, "C", "C", NEXT);

	// 보도하중인 경우
	if(pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_bWalkLoad)
	{
		double	dMu_1 = Round(1.3 * dD + 2.15 * dLi, 3);
		dMu = dMu > dMu_1 ? dMu : dMu_1;
		double	dMu_2 = Round(1.3 * dD + 1.3 * dLi + 1.3 * dCo, 3);
		dMu = dMu > dMu_2 ? dMu : dMu_2;
		double	dMu_3 = Round(1.3 * dD + 1.3 * dLi + 0.65 * dW, 3);
		dMu = dMu > dMu_3 ? dMu : dMu_3;
		double	dMu_4 = Round(1.2 * dD + 1.2 * dW + 1.2 * dCo, 3);
		dMu = dMu > dMu_4 ? dMu : dMu_4;
		double	dMu_5 = Round(1.3 * dD + 1.3 * dW, 3);
		dMu = dMu > dMu_5 ? dMu : dMu_5;

		// ================================================
		sText.Format("① Mu = 1.3D + 2.15(L+i)");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 2.15 × %.3f", dD, dLi);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_1);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

		// ================================================
		sText.Format("② Mu = 1.3D + 1.3(L+i) + 1.3CO");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f", dD, dLi, dCo);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_2);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

		// ================================================
		sText.Format("③ Mu = 1.3D + 1.3(L+i) + 0.65W");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 1.3 × %.3f + 0.65 × %.3f", dD, dLi, dW);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_3);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);
		
		// ================================================
		sText.Format("④ Mu = 1.2D + 1.2W + 1.2CO");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.2 × %.3f + 1.2 × %.3f + 1.2 × %.3f", dD, dW, dCo);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_4);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

		// ================================================
		sText.Format("⑤ Mu = 1.3D + 1.3W");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 1.3 × %.3f", dD, dW);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_5);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);
	}
	// 보도하중이 아닌 경우
	else
	{
		double	dMu_1 = Round(1.3 * dD + 2.15 * dLi + 1.3 * dCF, 3);
		dMu = dMu > dMu_1 ? dMu : dMu_1;
		double	dMu_2 = Round(1.3 * dD + 1.3 * dLi + 1.3 * dCF + 1.3 * dCo, 3);
		dMu = dMu > dMu_2 ? dMu : dMu_2;
		double	dMu_3 = Round(1.3 * dD + 1.3 * dLi + 1.3 * dCF + 0.65 * dW, 3);
		dMu = dMu > dMu_3 ? dMu : dMu_3;
		double	dMu_4 = Round(1.2 * dD + 1.2 * dW + 1.2 * dCo, 3);
		dMu = dMu > dMu_4 ? dMu : dMu_4;
		double	dMu_5 = Round(1.3 * dD + 1.3 * dW, 3);
		dMu = dMu > dMu_5 ? dMu : dMu_5;

		// ================================================
		sText.Format("① Mu = 1.3D + 2.15(L+i) + 1.3CF");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 2.15 × %.3f + 1.3 × %.3f", dD, dLi, dCF);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_1);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

		// ================================================
		sText.Format("② Mu = 1.3D + 1.3(L+i) + 1.3CF + 1.3CO");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f", dD, dLi, dCF, dCo);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_2);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);
		
		// ================================================
		sText.Format("③ Mu = 1.3D + 1.3(L+i) + 1.3CF + 0.65W");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f + 0.65 × %.3f", dD, dLi, dCF, dW);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_3);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);
		
		// ================================================
		sText.Format("④ Mu = 1.2D + 1.2W + 1.2CO");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.2 × %.3f + 1.2 × %.3f + 1.2 × %.3f", dD, dW, dCo);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_4);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

		// ================================================
		sText.Format("⑤ Mu = 1.3D + 1.3W");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f + 1.3 × %.3f", dD, dW);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_5);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

		if(pCalcData->m_bTank)
		{
			double dMu_6 = 1.3 * dD + 1.3 * dLi2 + 1.3 * dCF2;
			dMu = dMu > dMu_6 ? dMu : dMu_6;

			sText.Format("⑥ Mu = 1.3D + 1.3(L+i) + 1.3CF");
			pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

			pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
			pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f", dD, dLi2, dCF2);
			pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

			pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%.3f", dMu_6);
			pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "kN·m/m( 탱크궤도하중작용시 )",			m_nSttRow, "I", "I", NEXT);
		}

		if(pCalcData->m_bTrailer)
		{
			double dMu_7 = 1.3 * dD + 1.3 * dLi3 + 1.3 * dCF3;
			dMu = dMu > dMu_7 ? dMu : dMu_7;

			if(!pCalcData->m_bTank)
			{
				sText.Format("⑥ Mu = 1.3D + 1.3(L+i) + 1.3CF");
				pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

				pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
				pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

				sText.Format("1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f", dD, dLi3, dCF3);
				pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

				pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

				sText.Format("%.3f", dMu_7);
				pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, "kN·m/m( 탱크트레일러하중작용시 )",		m_nSttRow, "I", "I", NEXT);
			}
			else
			{
				sText.Format("⑦ Mu = 1.3D + 1.3(L+i) + 1.3CF");
				pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

				pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
				pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

				sText.Format("1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f", dD, dLi3, dCF3);
				pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

				pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

				sText.Format("%.3f", dMu_7);
				pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, "kN·m/m( 탱크트레일러하중작용시 )",		m_nSttRow, "I", "I", NEXT);
			}
		}
	}

	SetLines(NEXT);

	pOut->SetXL(pXL, "▷ 최대 계수 모멘트는 하중조합중에서 가장 큰 값을 사용",	m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "Mu",	m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",	m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", dMu);
	pOut->SetXL(pXL, sText,	m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "kN·m/m",	m_nSttRow, "I", "I", NEXT);

	SetLines(NEXT);

	pOut->SetXL(pXL, "- 최대 사용 모멘트 계산",			m_nSttRow, "C", "C", NEXT);

	sText.Format("① Ms = D + Li + 0.3W");
	pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

	pOut->SetXL(pXL, "Ms",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f + %.3f + 0.3 × %.3f", dD, dLi, dW);
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", dMu_Use);
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

	SetLines(NEXT);

	pOut->SetXL(pXL, "▷ 최대 사용 모멘트는 하중조합중에서 가장 큰 값을 사용",	m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "Ms",	m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",	m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", dMu_Use);
	pOut->SetXL(pXL, sText,	m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "kN·m/m",	m_nSttRow, "I", "I", NEXT);
	SetLines(NEXT);
}

// 10) 캔틸레버부 단부설계
void CXLDesignFloor::DesignOfCantilDanbu()
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText;
	sText.Format("%s 캔틸레버판 단부설계", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 4.7.6.4)",			m_nSttRow, "T", "T", NEXT);
	pOut->SetXL(pXL, "①",									m_nSttRow, "C", "C", 0, FALSE, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "주철근량   : 들보 단부 이외의 켄틸레버부 바닥판에서 필요한", m_nSttRow, "D", "D");
	pOut->SetXL(pXL, "             주철근량의 2배를 주철근으로 배근한다.", m_nSttRow, "D", "D");
	pOut->SetXL(pXL, "②",									m_nSttRow, "C", "C", 0, FALSE, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "배력철근량 : 들보단부 이외의 켄틸레버부 바닥판의 상측에", m_nSttRow, "D", "D");
	pOut->SetXL(pXL, "             배력철근량의 2배를 배력철근으로 배근한다.", m_nSttRow, "D", "D");
	pOut->SetXL(pXL, "③",									m_nSttRow, "C", "C", 0, FALSE, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "배치범위   : 도로교 설계기준 강교편 바닥판에 배치범위가 명시되어 있지 않으므로 콘크리트교편", m_nSttRow, "D", "D");
	pOut->SetXL(pXL, "             P346 을 적용하여 켄틸레버판의 사하중에 대한 지간을 배치범위로 적용한다.", m_nSttRow, "D", "D", 2);
}

// 11) 최소두께 검토
void CXLDesignFloor::CheckMinThick(long nFloorIndex)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();

	CMap <CString, LPCTSTR, double, double> MapStress;
	pCalcFloor->CalcPreStress(MapStress, nFloorIndex);

	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_bWalkLoad;
	double	dTuThick	= pDB->IsTUGir() ? pDB->m_BindConc.m_dHeight : 0;
	double	dTuDeepDis	= pDB->IsTUGir() ? pDB->m_BindConc.m_dDeep : 0;
	double	dSlabThick	= MapStress["Slab_Thick"] + dTuThick - dTuDeepDis;
	double	dL			= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_L;//<활하중에 의한 지간
	double	dmT			= 0.25;
	double	dUseThick	= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Min_T + dTuThick - dTuDeepDis;
	long	nGirdersu	= pDB->GetGirdersu();
		
	BOOL bUsePreStress	= pDB->m_nSlabTensionSize==0 ? FALSE : TRUE;//프리스트레스 사용인지...여부...

	double  dLastCheck	= 220;//차도부 바닥판의 최소두께는 220
	if(bUsePreStress)	dLastCheck = 200;//프리스트레스 적용이면...(소수주거더교)
	
	CString	sText, sText1, sText2, sText3;
	sText.Format("%s 바닥판의 최소두께 검토", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.6.1.5)",			m_nSttRow, "T", "T", NEXT);


	if(bWalkLoad)
	{
		//pOut->SetXL(pXL, "· 활하중이 없으므로 차도부 바닥판 최소두께 220mm 이상을 사용한다.", m_nSttRow, "D", "D", NEXT);
		pOut->SetXL(pXL, "· 보도부 바닥판의 최소 두께는 140 mm를 표준으로 한다.", m_nSttRow, "D", "D", NEXT);
		pOut->SetXL(pXL, "∴ T",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dUseThick,							m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER, "USETHICK");
		pOut->SetXL(pXL, "mm 사용",							m_nSttRow, "I", "I", 1);
	}	
	else if(dL > 0)//활하중에 의한 지간이면...
	{
		if(nFloorIndex != FLOOR_MID)		///< 중앙지간부가 아니고 좌우측...캔틸레버판이면..
		{
			pOut->SetXL(pXL, "L",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		
			sText.Format("=%s",pOut->GetCellRef("MOVEL"));
			pOut->SetXL(pXL, sText,							m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "MOVEL2");
			pOut->SetXL(pXL, dmT,							m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "AMOVEL");

			sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("MOVEL2"), pOut->GetCellRef("AMOVEL"));
			pOut->SetXL(pXL, sText,							m_nSttRow, "J", "J", 0, RED,   9, TRUE,  TA_CENTER);

			pOut->SetXL(pXL, "인 경우",						m_nSttRow, "O", "O");
			pOut->SetXL(pXL, "hmin",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
			if(toM(dL) > dmT)
			{
				if(bUsePreStress)//
				{
					sText1 = "(80×L + 230) × 0.9";
					sText2.Format("=\"(80×\"&ROUND(%s,3)&\" + 230) × 0.9\"", pOut->GetCellRef("MOVEL"));
					sText3.Format("=(80*%s+230)*0.9", pOut->GetCellRef("MOVEL"));					
				}
				else
				{
					sText1 = "80×L + 230";
					sText2.Format("=\"80×\"&ROUND(%s,3)&\" + 230\"", pOut->GetCellRef("MOVEL"));										
					sText3.Format("=80*%s+230", pOut->GetCellRef("MOVEL"));										
				}
			}
			else
			{
				if(bUsePreStress)
				{
					sText1 = "(280×L + 180) × 0.9";
					sText2.Format("=\"(280 × \"&ROUND(%s,3)&\" + 180) × 0.9\"", pOut->GetCellRef("MOVEL"));
					sText3.Format("=(280 * %s + 180)*0.9", pOut->GetCellRef("MOVEL"));										
				}
				else
				{
					sText1 = "280×L + 180";
					sText2.Format("=\"280 × \"&ROUND(%s,3)&\" + 180\"", pOut->GetCellRef("MOVEL"));
					sText3.Format("=280 * %s + 180", pOut->GetCellRef("MOVEL"));	
				}
			}		
		}
		else if(nGirdersu > 2)	///< 당연히 중앙지간부이고...연속판(거더개수 3개이상)이면...
		{
			pOut->SetXL(pXL, "hmin",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			if(bUsePreStress)//
			{
				sText1 = "(30×L + 130) × 0.9";
				sText2.Format("=\"(30×\"&ROUND(%s,3)&\" + 130) × 0.9\"", pOut->GetCellRef("CEN_DEAD_L"));
				sText3.Format("=(30*%s+130)*0.9", pOut->GetCellRef("CEN_DEAD_L"));
			}
			else
			{
				sText1 = "30×L + 130";
				sText2.Format("=\"30×\"&ROUND(%s,3)&\" + 130\"",			pOut->GetCellRef("CEN_DEAD_L"));
				sText3.Format("=30*%s+130", pOut->GetCellRef("CEN_DEAD_L"));				
			}
		}
		else///< 중앙지간부이고...단순판 이면
		{
			pOut->SetXL(pXL, "hmin",						m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			if(bUsePreStress)
			{
				sText1 = "(40×L + 130) × 0.9";
				sText2.Format("=\"(40×\"&ROUND(%s,3)&\" + 130) × 0.9\"", pOut->GetCellRef("CEN_DEAD_L"));
				sText3.Format("=(40*%s+130)*0.9", pOut->GetCellRef("CEN_DEAD_L"));
			}
			else
			{				
				sText1 = "40×L + 130";
				sText2.Format("=\"40×\"&ROUND(%s,3)&\" + 130\"",			pOut->GetCellRef("CEN_DEAD_L"));
				sText3.Format("=40*%s+130", pOut->GetCellRef("CEN_DEAD_L"));
			}
		}

		if(bUsePreStress)
		{			
			pOut->SetXL(pXL, sText1, m_nSttRow, "F", "K", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",	 m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);			
			pOut->SetXL(pXL, sText2, m_nSttRow, "M", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",	 m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);			
			pOut->SetXL(pXL, sText3, m_nSttRow, "U", "W", 0, BLACK,  9, FALSE, TA_CENTER, "CALTHICK", 1);
			pOut->SetXL(pXL, "mm",	 m_nSttRow, "X", "X", 1, BLACK, 9, FALSE, TA_CENTER);				
		}
		else
		{
			pOut->SetXL(pXL, sText1, m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",	 m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);			
			pOut->SetXL(pXL, sText2, m_nSttRow, "K", "O", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",	 m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);			
			pOut->SetXL(pXL, sText3, m_nSttRow, "Q", "S", 0, BLACK,  9, FALSE, TA_CENTER, "CALTHICK", 1);
			pOut->SetXL(pXL, "mm",	 m_nSttRow, "T", "T", 1, BLACK, 9, FALSE, TA_CENTER);			
		}		
		
		sText.Format("=MAX(%s,%.1f)",pOut->GetCellRef("CALTHICK"), dLastCheck);
		pOut->SetXL(pXL, sText,		 m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "MINTHICK", 1);
		pOut->SetXL(pXL, "mm",		 m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);	
				
		pOut->SetXL(pXL, dSlabThick,	m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "USETHICK", 1);		
		sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("MINTHICK"), pOut->GetCellRef("USETHICK"));
		pOut->SetXL(pXL, sText,			m_nSttRow, "K", "K", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "mm 사용 ⇒",	m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(%s<%s, \"...O.K\", \"...N.G\")", pOut->GetCellRef("MINTHICK"), pOut->GetCellRef("USETHICK"));
		pOut->SetXL(pXL, sText,			m_nSttRow, "S", "U", 1, RED,   9, TRUE,  TA_CENTER, "USETHICK");

		if(bUsePreStress)
		{
			pOut->SetXL(pXL, "· 바닥판 지간 방향으로 Prestress를 도입한 경우 계산량의 90% 적용 (☞ 도.설.기 4.7.4.2)", m_nSttRow, "D", "D", 1);		
			pOut->SetXL(pXL, "· 최소두께 계산값과 200 mm 중에서 큰값을 사용한다. (☞ 도.설.기 4.7.4.1)", m_nSttRow, "D", "D", 2);
		}
		else
			pOut->SetXL(pXL, "· 최소두께 계산값과 220 mm 중에서 큰값을 사용한다. (☞ 도.설.기 3.6.1.5)", m_nSttRow, "D", "D", 2);
	}	
	else
	{
		pOut->SetXL(pXL, "· 활하중이 없으므로 차도부 바닥판 최소두께 220mm 이상을 사용한다.", m_nSttRow, "D", "D", NEXT);
		//pOut->SetXL(pXL, "· 보도부 바닥판의 최소 두께는 140 mm를 표준으로 한다.", m_nSttRow, "D", "D", NEXT);
		pOut->SetXL(pXL, "∴ T",							m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dUseThick,							m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER, "USETHICK");
		pOut->SetXL(pXL, "mm 사용",							m_nSttRow, "I", "I", 1);
	}	
}

// 11) 단면설계
void CXLDesignFloor::DesignOfSection(long nFloorIndex)
{
	CXLControl			*pXL	= m_pXL;
	CAPlateOutCalcXL	*pOut	= m_pAPlateOutCalcXL;
	CGlobarOption		*pOpt	= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
	CString	sText;

	sText.Format("%s 단 면 검 토", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);

	if(nFloorIndex==FLOOR_MID)
	{
		if(pOpt->GetSlabCentDesignMethod()==0)
		{
			// 강도설계
			CalcReBarQuantity(nFloorIndex);	///< 철근량 산정
			CheckUsefully(nFloorIndex);		///< 사용성 검토
		}
		else
		{
			// 경험적 설계
			BOOL	bLeftCheck	= CalcGeneral.CheckExperienceDesign(TRUE);
			BOOL	bRightCheck	= CalcGeneral.CheckExperienceDesign(FALSE);
			if(!bLeftCheck || !bRightCheck)
				CalcReBarQuantity(nFloorIndex);
			CaclRebarExperienceDesign();
		}
	}
	else
	{
		CalcReBarQuantity(nFloorIndex);	///< 철근량 산정
		CheckUsefully(nFloorIndex);		///< 사용성 검토
	}
}

void CXLDesignFloor::CaclRebarExperienceDesign()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CCalcData			*pCalcData	= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
	CPlateGirderApp		*pGirFir	= pDB->GetGirder(0);
	CPlateBxFinder		FinderStt(pGirFir);
	CPlateBasicIndex	*pBxFir		= FinderStt.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateGirderApp		*pGirEnd	= pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBxFinder		FinderEnd(pGirEnd);
	CPlateBasicIndex	*pBxEnd		= FinderEnd.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CXLControl			*pXL		= m_pXL;

	double	dSlabThick				= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;//pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Req_T;	// 바닥판 최소 두께
	double	dLengthCen				= CalcGeneral.CalcCentPart_L();						// 중앙부 유효길이
	double	dLengthLeft				= pDB->m_dWidthSlabLeft - pGirFir->GetThickJewon(G_W, pBxFir)/2;	// 좌측 캔틸레버부 끝단에서 첫번째거더 좌측복부판 중앙까지 거리
	double	dLengthRight			= pDB->m_dWidthSlabRight - pGirEnd->GetThickJewon(G_W, pBxEnd)/2;	// 우측 캔틸레버부 끝단에서 마지막거더 우측복부판 중앙까지 거리
	double	dCoverUp				= pCalcData->m_dCoverUp;							// 철근덮게(상면)
	double	dCoverDn				= pCalcData->m_dCoverDn;							// 철근덮게(하면)
	double	dMainDiaUpper_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1];			// 주철근 상면 Dia Idx 1-Cylce
	double	dMainDiaUpper_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy];		// 주철근 상면 Dia Idx 2-Cycle
	double	dMainCTCUpper			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1;						// 주철근 상면 간격
	double	dMainDiaLower_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy];	// 주철근 하면 Dia Idx 1-Cylce
	double	dMainDiaLower_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy];	// 주철근 하면 Dia Idx 2-Cycle
	double	dMainCTCLower			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1;					// 주철근 하면 간격
	double	dBaeRuckDiaUpper		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3];			// 배력철근 상면 Dia Idx
	double	dBaeRuckCTCUpper		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB;						// 배력철근 상면 간격
	double	dBaeRuckDiaLower		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2];		// 배력철근 하면 Dia Idx
	double	dBaeRuckCTCLower		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2;					// 배력철근 하면 간격
	double	dEndReinforceDiaMain_1Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2];			// 단부보강 주철근 Dia Idx 1-Cycle
	double	dEndReinforceDiaMain_2Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy];		// 단부보강 주철근 Dia Idx 2-Cycle
	double	dEndReinforceCTCMain	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// 단부보강 주철근 간격
	double	dEndReinforceDiaBeryuk	= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4];			// 단부보강 배력철근 Dia Idx
	double	dEndReinforceCTCBeryuk	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1;						// 단부보강 배력철근 간격
	double	dSigmack				= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	CString	szLeftGuard				= _T("");
	CString	szRightGuard			= _T("");
	CString	sRebar					= pCalcData->DESIGN_MATERIAL.m_dSigmaY >= 400 ? _T("H") : _T("D");
	CString	szText					= _T("");

	CCentSeparation	*pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::LEFT);
	if(pCentSeparation)	szLeftGuard		= _T("방호책 있음");
	else				szLeftGuard		= _T("방호책 없음");
	pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
	if(pCentSeparation)	szRightGuard	= _T("방호책 있음");
	else				szRightGuard	= _T("방호책 없음");

	pOut->SetXL(pXL, "①",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("지지부재    :  강거더");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 0, BLACK);
	szText.Format("바 닥 판 제 원");
	pOut->SetLineBoxText(pXL, szText,	m_nSttRow, "S","AA", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetXL(pXL, "②",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("콘크리트가 현장타설되고 습윤양생되어야 함.");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 0, BLACK);
	szText.Format("바닥판두께");
	pOut->SetLineBoxText(pXL, szText,	m_nSttRow, "S", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",		m_nSttRow, "X","AA", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dSlabThick);
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 0, RED,   9, FALSE, TA_CENTER, "THICK_SLAB", 1);
	pOut->SetXL(pXL, "mm",				m_nSttRow,"AA","AA", 1);

	pOut->SetXL(pXL, "③",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("주거더 플랜지부의 헌치와 같이 국부적으로 두껍게");
	pOut->SetXL(pXL, szText,				m_nSttRow, "D", "D", 0, BLACK);
	szText.Format("유효지간");
	pOut->SetLineBoxText(pXL, szText,	m_nSttRow, "S", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",		m_nSttRow, "X","AA", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dLengthCen);
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 0, RED,   9, FALSE, TA_CENTER, "LENGTH_SLAB", 1);
	pOut->SetXL(pXL, "mm",				m_nSttRow,"AA","AA", 1);

	szText.Format("한 곳을 제외한 상태에서 전체적으로 바닥판의 두께가 일정함.");
	pOut->SetXL(pXL, szText,				m_nSttRow, "D", "D", 0, BLACK);
	szText.Format("철근덮개 (상면)");
	pOut->SetLineBoxText(pXL, szText,	m_nSttRow, "S", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",		m_nSttRow, "X","AA", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dCoverUp);
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 0, RED,   9, FALSE, TA_CENTER, "COVER_UP", 1);
	pOut->SetXL(pXL, "mm",				m_nSttRow,"AA","AA", 1);

	pOut->SetXL(pXL, "④",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("바닥판 두께에 대한 유효지간의 비가 6 이상 15 이하");
	pOut->SetXL(pXL, szText,				m_nSttRow, "D", "D", 0, BLACK);
	szText.Format("철근덮개 (하면)");
	pOut->SetLineBoxText(pXL, szText,	m_nSttRow, "S", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",		m_nSttRow, "X","AA", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dCoverDn);
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 0, RED,   9, FALSE, TA_CENTER, "COVER_DN", 1);
	pOut->SetXL(pXL, "mm",				m_nSttRow,"AA","AA", 1);

	pOut->SetXL(pXL, 6.0,				m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_SLABTHICK_RATE", 0);
	szText.Format("=%s", pOut->GetCellRef("LENGTH_SLAB"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "F", "G", 0, BLACK, 9, FALSE, TA_CENTER, "LENGTH_SLAB_PARA", 0);
	pOut->SetXL(pXL, "'/",				m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("THICK_SLAB"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "THICK_SLAB_PARA", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s/%s", pOut->GetCellRef("LENGTH_SLAB_PARA"), pOut->GetCellRef("THICK_SLAB_PARA"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "RATE", 2);
	pOut->SetXL(pXL, 15.0,				m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_SLABTHICK_RATE", 0);
	szText.Format("=IF(%s<(%s/%s),\"<\",\">\")", pOut->GetCellRef("MIN_SLABTHICK_RATE"), pOut->GetCellRef("LENGTH_SLAB_PARA"), pOut->GetCellRef("THICK_SLAB_PARA"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF((%s/%s)<%s,\"<\",\">\")", pOut->GetCellRef("LENGTH_SLAB_PARA"), pOut->GetCellRef("THICK_SLAB_PARA"), pOut->GetCellRef("MAX_SLABTHICK_RATE"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s<%s,\"... O.K\",\"...N.G\")", pOut->GetCellRef("RATE"), pOut->GetCellRef("MAX_SLABTHICK_RATE"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "Q", "Q", 1, RED);
	pOut->SetXL(pXL, "⑤",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("바닥판의 상부와 하부에 배근된 철근의 외측면 사이의 두께가 150mm 이상");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 2);
	pOut->SetXL(pXL, "⑥",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("유효지간이 표준차선폭 3.6m 이하");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 1);
	szText.Format("=%s/1000", pOut->GetCellRef("LENGTH_SLAB"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER, "LENGTH_SLAB_PARA", 3);
	pOut->SetXL(pXL, 3.6,				m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "WIDTH_LANE", 3);
	szText.Format("=IF(%s<%s,\"<\",\">\")", pOut->GetCellRef("LENGTH_SLAB_PARA"), pOut->GetCellRef("WIDTH_LANE"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s<%s,\"... O.K\",\"... N.G\")", pOut->GetCellRef("LENGTH_SLAB_PARA"), pOut->GetCellRef("WIDTH_LANE"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "J", "J", 1, RED);
	pOut->SetXL(pXL, "⑦",				m_nSttRow, "C", "C", 0);
	szText.Format("바닥판의 흠집, 마모면, 그리고 보호덮개층을 제외한 바닥판의 최소두께가 240mm");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 1, BLACK, 9);
	pOut->SetXL(pXL, "⑧",				m_nSttRow, "C", "C", 0);
	szText.Format("캔틸레버부의 길이가 내측바닥판 두께의 5배 이상이거나, 캔틸레버부의 길이가 내측");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 1);
	szText.Format("바닥판 두께의 3배 이상이고 구조적으로 연속적인 콘크리트 방호책과 합성이 된 경우");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 1);
	szText.Format("좌측 캔틸레버 길이");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 0);
	szText.Format("(%s)", szLeftGuard);
	pOut->SetXL(pXL, szText,			m_nSttRow, "I", "L", 0, RED,   9, FALSE, TA_CENTER, "ISLEFTGUARD");
	pOut->SetXL(pXL, "':",				m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%g", toM(dLengthLeft));
	pOut->SetXL(pXL, szText,			m_nSttRow, "N", "O", 0, RED,   9, FALSE, TA_CENTER, "LENGTH_LEFT", 3);
	szText.Format("=%s/1000", pOut->GetCellRef("THICK_SLAB"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "Q", "R", 0, BLACK, 9, FALSE, TA_CENTER, "THICK_SLAB_PARA", 3);
	pOut->SetXL(pXL, "×",				m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=\"(방호책있음)\",3,5)", pOut->GetCellRef("ISLEFTGUARD"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER, "LEFT_GUARD_COEFF", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*%s", pOut->GetCellRef("THICK_SLAB_PARA"), pOut->GetCellRef("LEFT_GUARD_COEFF"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_LENGTH_LEFT");
	szText.Format("=IF(%s>%s,\">\",\"<\")", pOut->GetCellRef("LENGTH_LEFT"), pOut->GetCellRef("MIN_LENGTH_LEFT"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s>%s,\"... O.K\",\"... N.G\")", pOut->GetCellRef("LENGTH_LEFT"), pOut->GetCellRef("MIN_LENGTH_LEFT"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "Y", "Y", 1, RED,   9);

	szText.Format("우측 캔틸레버 길이");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 0);
	szText.Format("(%s)", szRightGuard);
	pOut->SetXL(pXL, szText,			m_nSttRow, "I", "L", 0, RED,   9, FALSE, TA_CENTER, "ISRIGHTGUARD");
	pOut->SetXL(pXL, "':",				m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%g", toM(dLengthRight));
	pOut->SetXL(pXL, szText,			m_nSttRow, "N", "O", 0, RED,   9, FALSE, TA_CENTER, "LENGTH_RIGHT", 3);
	szText.Format("=%s/1000", pOut->GetCellRef("THICK_SLAB"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "Q", "R", 0, BLACK, 9, FALSE, TA_CENTER, "THICK_SLAB_PARA", 3);
	pOut->SetXL(pXL, "×",				m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=\"(방호책있음)\",3,5)", pOut->GetCellRef("ISRIGHTGUARD"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER, "GUARDCOEFF_RIGHT", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*%s", pOut->GetCellRef("THICK_SLAB_PARA"), pOut->GetCellRef("GUARDCOEFF_RIGHT"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "V", "W", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_LENGTH_RIGHT");
	szText.Format("=IF(%s>%s,\">\",\"<\")", pOut->GetCellRef("LENGTH_RIGHT"), pOut->GetCellRef("MIN_LENGTH_RIGHT"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s>%s,\"... O.K\",\"... N.G\")", pOut->GetCellRef("LENGTH_RIGHT"), pOut->GetCellRef("MIN_LENGTH_RIGHT"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "Y", "Y", 1, RED,   9);
	pOut->SetXL(pXL, "⑨",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("콘크리트의 28일 압축강도는 %.0f MPa 이상", dSigmack);
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "⑩",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("철근 콘크리트 바닥판은 바닥판을 지지하는 구조부재들과 완전합성거동");
	pOut->SetXL(pXL, szText,			m_nSttRow, "D", "D", 2);

	pOut->SetXL(pXL, "2) 철근 배근량",	m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "①",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "지간방향",		m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "':",				m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("하부철근량 - 콘크리트 바닥판 단면의 0.4%% 이상");
	pOut->SetXL(pXL, szText,			m_nSttRow, "J", "J", 1);
	szText.Format("상부철근량 - 콘크리트 바닥판 단면의 0.3%% 이상");
	pOut->SetXL(pXL, szText,			m_nSttRow, "J", "J", 1);
	pOut->SetXL(pXL, "②",				m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "지간방향에 직각방향",	m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "':",				m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("하부철근량 - 콘크리트 바닥판 단면의 0.3%% 이상");
	pOut->SetXL(pXL, szText,			m_nSttRow, "J", "J", 1);
	szText.Format("상부철근량 - 콘크리트 바닥판 단면의 0.3%% 이상");
	pOut->SetXL(pXL, szText,			m_nSttRow, "J", "J", 1);

	pOut->SetXL(pXL, "3) 사용 철근량",	m_nSttRow, "B", "B", 1);
	szText.Format("* 여기서 바닥판 슬래브 단면적");
	pOut->SetXL(pXL, "'=",				m_nSttRow, "K", "K", 0);
	pOut->SetXL(pXL, 1000.0,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "UNIT_LEN", 0);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("THICK_SLAB"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "THICK_SLAB_PARA", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*%s", pOut->GetCellRef("UNIT_LEN"), pOut->GetCellRef("THICK_SLAB_PARA"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "SLAB_AREA");
	pOut->SetXL(pXL, "㎟",				m_nSttRow, "U", "U", 1);

	pOut->SetLineBoxText(pXL, "위    치",	m_nSttRow, "B", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "산 정 식",	m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "검  토",		m_nSttRow, "X", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow++;
	pOut->SetLineBoxText(pXL, "주 철 근",	m_nSttRow, "B", "D", 8, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "하면철근",	m_nSttRow, "E", "G", 4, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "필요철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			m_nSttRow, "X", "Z", 4, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("SLAB_AREA"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARA1", 0);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 0.4,				m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "PARA2", 1);
	pOut->SetXL(pXL, "%",				m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*%s/100", pOut->GetCellRef("PARA1"), pOut->GetCellRef("PARA2"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asreq_MainLo", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용철근량",	m_nSttRow, "H", "J", 3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainDiaLower_1Cy);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED, 9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainCTCLower);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse1", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);

	CString	sCell1, sCell2;
	sCell1.Format("L%d", m_nSttRow);	sCell2.Format("V%d", m_nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainDiaLower_2Cy);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED, 9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainCTCLower);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse2", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "소계",			m_nSttRow, "Q", "Q", 0);
	szText.Format("=%s+%s", pOut->GetCellRef("Asuse1"), pOut->GetCellRef("Asuse2"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow-=3;
	szText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Asuse"), pOut->GetCellRef("Asreq_MainLo"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 4, RED,   9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "상면철근",	m_nSttRow, "E", "G", 4, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "필요철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			m_nSttRow, "X", "Z", 4, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("SLAB_AREA"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARA1", 0);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 0.3,				m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "PARA2", 1);
	pOut->SetXL(pXL, "%",				m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*%s/100", pOut->GetCellRef("PARA1"), pOut->GetCellRef("PARA2"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asreq", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용철근량",	m_nSttRow, "H", "J", 3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainDiaUpper_1Cy);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED, 9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainCTCUpper);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse1", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);

	sCell1.Format("L%d", m_nSttRow);	sCell2.Format("V%d", m_nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainDiaUpper_2Cy);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED, 9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dMainCTCUpper);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse2", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "소계",			m_nSttRow, "Q", "Q", 0);
	szText.Format("=%s+%s", pOut->GetCellRef("Asuse1"), pOut->GetCellRef("Asuse2"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow-=3;
	szText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Asuse"), pOut->GetCellRef("Asreq"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 4, RED,   9, FALSE, TA_CENTER);
	// 배력철근 하면
	pOut->SetLineBoxText(pXL, "배력철근",	m_nSttRow, "B", "D", 4, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "하면철근",	m_nSttRow, "E", "G", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "필요철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			m_nSttRow, "X", "Z", 2, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("SLAB_AREA"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARA1", 0);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 0.3,				m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "PARA2", 1);
	pOut->SetXL(pXL, "%",				m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*%s/100", pOut->GetCellRef("PARA1"), pOut->GetCellRef("PARA2"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asreq_BaeLo", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER);
	szText.Format("%g", dBaeRuckDiaLower);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED,   9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dBaeRuckCTCLower);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow--;
	szText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Asuse"), pOut->GetCellRef("Asreq_BaeLo"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 2, RED,   9, FALSE, TA_CENTER);
	// 배력철근 상면
	pOut->SetLineBoxText(pXL, "상면철근",	m_nSttRow, "E", "G", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "필요철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			m_nSttRow, "X", "Z", 2, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("SLAB_AREA"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARA1", 0);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 0.3,				m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "PARA2", 1);
	pOut->SetXL(pXL, "%",				m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*%s/100", pOut->GetCellRef("PARA1"), pOut->GetCellRef("PARA2"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asreq", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER);
	szText.Format("%g", dBaeRuckDiaUpper);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED,   9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dBaeRuckCTCUpper);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow--;
	szText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Asuse"), pOut->GetCellRef("Asreq"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 2, RED,   9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "단부보강철근",	m_nSttRow, "B", "D", 6, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "주철근",			m_nSttRow, "E", "G", 4, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "필요철근량",		m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",				m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",				m_nSttRow, "X", "Z", 4, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("Asreq_MainLo"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARA1", 0);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "2 배",			m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*2", pOut->GetCellRef("PARA1"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asreq", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용철근량",	m_nSttRow, "H", "J", 3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%g", dEndReinforceDiaMain_1Cy);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED, 9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dEndReinforceCTCMain);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse1", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);

	sCell1.Format("L%d", m_nSttRow);	sCell2.Format("V%d", m_nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%g", dEndReinforceDiaMain_2Cy);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED, 9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dEndReinforceCTCMain);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED, 9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse2", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "소계",			m_nSttRow, "Q", "Q", 0);
	szText.Format("=%s+%s", pOut->GetCellRef("Asuse1"), pOut->GetCellRef("Asuse2"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow-=3;
	szText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Asuse"), pOut->GetCellRef("Asreq"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 4, RED,   9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "배력철근",	m_nSttRow, "E", "G", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "필요철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			m_nSttRow, "X", "Z", 2, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s", pOut->GetCellRef("Asreq_BaeLo"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARA1", 0);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "2 배",			m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=%s*2", pOut->GetCellRef("PARA1"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asreq", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "사용철근량",	m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "K", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,			m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER);
	szText.Format("%g", dEndReinforceDiaBeryuk);
	pOut->SetXL(pXL, szText,			m_nSttRow, "M", "M", 0, RED,   9, FALSE, TA_CENTER, "DIA", 0);
	pOut->SetXL(pXL, "@",				m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("%g", dEndReinforceCTCBeryuk);
	pOut->SetXL(pXL, szText,			m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, "CTC", 0);
	pOut->SetXL(pXL, "'=",				m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("=IF(%s=32,7.942*10^5/%s,IF(%s=29,6.424*10^5/%s,IF(%s=25,5.067*10^5/%s,IF(%s=22,3.871*10^5/%s,IF(%s=19,2.865*10^5/%s,IF(%s=16,1.986*10^5/%s,IF(%s=13,1.267*10^5/%s,0)))))))",
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"),
		pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"),
		pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"), pOut->GetCellRef("DIA"), pOut->GetCellRef("CTC"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "Asuse", 1);
	pOut->SetXL(pXL, "mm²",			m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	m_nSttRow--;
	szText.Format("=IF(%s>%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Asuse"), pOut->GetCellRef("Asreq"));
	pOut->SetXL(pXL, szText,			m_nSttRow, "X", "Z", 2, RED,   9, FALSE, TA_CENTER);

}


void CXLDesignFloor::CalcReBarQuantity(long nFloorIndex)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	double	dFck		= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dFy			= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dPibok		= pCalcData->m_dCoverUp;
	if(nFloorIndex == 2) 
		dPibok	= pCalcData->m_dCoverDn;
	double	dB			= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_b;
	double	dDeadL		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Dead_L;
	double  dH          = 0;	// 슬래브 두께
	if(nFloorIndex==0)		dH = pDB->m_dThickSlabBase;
	else if(nFloorIndex==1)	dH = pDB->m_dThickSlabBase;
	else if(nFloorIndex==2)	dH = pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;
	//if(pDB->IsTUGir() && nFloorIndex!=2)	dH -= (pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDeep);
	//dH -= pDB->m_BindConc.m_dDeep;
	
	CString	sCaseH1_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CaseH1;		// 일반부 주철근 철근지름 - 1Cycle
	CString	sCaseH1_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CaseH1_2Cy;	// 일반부 주철근 철근지름 - 2Cycle
	double	dCtc1		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_Ctc1;			// 일반부 주철근간격
	CString	sCaseH2_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CaseH2;		// 단부 주철근 철근지름 - 1Cycle
	CString	sCaseH2_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CaseH2_2Cy;	// 단부 주철근 철근지름 - 2Cycle
	double	dCtc2		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_Ctc2;			// 단부 주철근 간격
	CString	sCaseH3		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CaseH3;		// 일반부 배력철근 지름
	double	dCtcB		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CtcB;			// 일반부 배력철근 간격
	CString sCaseH4		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CaseH4;		// 단부 배력철근 철근지름
	double	dCtcB1		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_CtcB1;		// 단부 배력철근 간격
	
	CString	sText;

	pOut->SetXL(pXL, "◀ 철근량 산정",						m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "① 일반부",							m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "[ DESIGN CONDITION ]",				m_nSttRow, "D", "D", NEXT);

	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);

	if(nFloorIndex==2)
		pOut->InsertImage(pXL, "철근단면_중앙", m_nSttRow, "D");
	else
		pOut->InsertImage(pXL, "철근단면_좌우", m_nSttRow, "D");

	pOut->SetXL(pXL, "fck",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dFck,									m_nSttRow, "R", "T", 0, BLACK,  9, FALSE, TA_CENTER, "FCK", 0);
	pOut->SetXL(pXL, "MPa",								m_nSttRow, "U", "U", NEXT);

	pOut->SetXL(pXL, "fy",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dFy,									m_nSttRow, "R", "T", 0, BLACK,  9, FALSE, TA_CENTER, "FY", 0);
	pOut->SetXL(pXL, "MPa",								m_nSttRow, "U", "U", NEXT);

	pOut->SetXL(pXL, "Mu",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("MAXMOMENT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "R", "T", 0, BLACK,  9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "U", "U", NEXT);
	
	long nSize = m_ArrayGuardWallTopL.GetSize();
	pOut->SetXL(pXL, "L",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	if(nSize != 0)
	{
		sText.Format("=(SUM(%s:%s)*1000)",pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(0)),
			pOut->GetCellRef(m_ArrayGuardWallTopL.GetAt(nSize-1)));				
	}
	else	sText.Format("%g", dDeadL);

	pOut->SetXL(pXL, sText,									m_nSttRow, "R", "T", 0, RED,  9, FALSE, TA_CENTER, "DEAD_L");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "U", "U", NEXT);

	pOut->SetXL(pXL, "b",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dB,									m_nSttRow, "R", "T", 0, RED,  9, FALSE, TA_CENTER, "B");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "U", "U", NEXT);

	pOut->SetXL(pXL, "H",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dH,									m_nSttRow, "R", "T", 0, RED,  9, FALSE, TA_CENTER, "H");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "U", "U", NEXT);

	pOut->SetXL(pXL, "d'",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPibok,								m_nSttRow, "R", "T", 0, RED,  9, FALSE, TA_CENTER, "D");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "U", "U", NEXT);

	pOut->SetXL(pXL, "d",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dH-dPibok,								m_nSttRow, "R", "T", 0, RED,  9, FALSE, TA_CENTER, "AVAIL_D");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "U", "U", NEXT);

	pOut->SetXL(pXL, "k",									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>28,0.85-(%s-28)*0.007,0.85)", pOut->GetCellRef("FCK"), pOut->GetCellRef("FCK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "R", "T", 1, BLACK, 9, FALSE, TA_CENTER, "K");

	pOut->SetXL(pXL, "Ø(Bending)",							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 0.85,									m_nSttRow, "R", "T", 2, BLACK, 9, FALSE, TA_CENTER, "BENDING");

	pOut->SetXL(pXL, "[ Req'd d&As ]",						m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "(☞ 콘.설.기 6.3.2)",					m_nSttRow, "T", "T", NEXT);
	pOut->SetXL(pXL, "a",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "As ×fy / ( k × fck × b )"	,		m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/(%s*%s*%s)", pOut->GetCellRef("FY"), pOut->GetCellRef("K"), pOut->GetCellRef("FCK"), pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "COEFFCIENT_a");
	pOut->SetXL(pXL, "As",									m_nSttRow, "R", "R", NEXT);

	pOut->SetXL(pXL, "Mu",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, " Φ × As × fy × ( d - a/2) ",		m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=-0.5*%s*%s*%s", pOut->GetCellRef("COEFFCIENT_a"), pOut->GetCellRef("BENDING"), pOut->GetCellRef("FY"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "COEFFCIENT_A");
	pOut->SetXL(pXL, "As²",								m_nSttRow, "R", "R", 0);
	pOut->SetXL(pXL, "+",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s*%s", pOut->GetCellRef("BENDING"), pOut->GetCellRef("FY"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 0, BLACK,  9, FALSE, TA_CENTER, "COEFFCIENT_B");
	pOut->SetXL(pXL, "As",									m_nSttRow, "W", "W", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*1000000", pOut->GetCellRef("MAXMOMENT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 2, BLACK,  9, FALSE, TA_CENTER, "COEFFCIENT_C", 0);

	pOut->SetXL(pXL, "∴ Req'd",							m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "As",									m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(-%s + SQRT(%s^2 - 4 * %s *(-1*%s)))/(2*%s)", pOut->GetCellRef("COEFFCIENT_B"), pOut->GetCellRef("COEFFCIENT_B"), pOut->GetCellRef("COEFFCIENT_A"), pOut->GetCellRef("COEFFCIENT_C"), pOut->GetCellRef("COEFFCIENT_A"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "REQ_AS");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", NEXT);

	pOut->SetXL(pXL, "Min.",								m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "As = Req'd As × 4/3",				m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*4/3", pOut->GetCellRef("REQ_AS"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_AS1");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", NEXT);

	pOut->SetXL(pXL, "Min.",								m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "As = 0.002 × b × d",				m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.002 * %s * %s", pOut->GetCellRef("B"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_AS2");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", NEXT);

	pOut->SetXL(pXL, "Min.",								m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "As = 1.4 / fy × b × d",				m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=1.4 / %s * %s * %s", pOut->GetCellRef("FY"), pOut->GetCellRef("B"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "MIN_AS3");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", NEXT);

	pOut->SetXL(pXL, "Min.",								m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "As = 0.25×√fck/fy×b×d",			m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.25 * SQRT(%s) / %s * %s * %s", pOut->GetCellRef("FCK"), pOut->GetCellRef("FY"), pOut->GetCellRef("B"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "MIN_AS4");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", NEXT);

	pOut->SetXL(pXL, "USE",									m_nSttRow, "C", "E", 0, BLACK, 9, TRUE,  TA_RIGHT);
	pOut->SetXL(pXL, "H",									m_nSttRow, "F", "F", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, sCaseH1_1Cy,							m_nSttRow, "G", "H", 0, RED,   9, TRUE,  TA_CENTER, "CASEH1_1CYCLE", 0);
	pOut->SetXL(pXL, "@",									m_nSttRow, "I", "I", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, dCtc1,									m_nSttRow, "J", "K", 0, RED,   9, TRUE,  TA_CENTER, "CTC1", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
				   pOut->GetCellRef("CASEH1_1CYCLE"), pOut->GetCellRef("CTC1"), pOut->GetCellRef("CASEH1_1CYCLE"), pOut->GetCellRef("CTC1"),
				   pOut->GetCellRef("CASEH1_1CYCLE"), pOut->GetCellRef("CTC1"), pOut->GetCellRef("CASEH1_1CYCLE"), pOut->GetCellRef("CTC1"),
				   pOut->GetCellRef("CASEH1_1CYCLE"), pOut->GetCellRef("CTC1"), pOut->GetCellRef("CASEH1_1CYCLE"), pOut->GetCellRef("CTC1"),
				   pOut->GetCellRef("CTC1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, RED,   9, TRUE,  TA_CENTER, "USEAS1_1CYCLE");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 1);
	pOut->SetXL(pXL, "H",									m_nSttRow, "F", "F", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, sCaseH1_2Cy,							m_nSttRow, "G", "H", 0, RED,   9, TRUE,  TA_CENTER, "CASEH1_2CYCLE", 0);
	pOut->SetXL(pXL, "@",									m_nSttRow, "I", "I", 0, RED,   9, TRUE,  TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("CTC1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "J", "K", 0, RED,   9, TRUE,  TA_CENTER, "CTC1_2CYCLE", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
				   pOut->GetCellRef("CASEH1_2CYCLE"), pOut->GetCellRef("CTC1_2CYCLE"), pOut->GetCellRef("CASEH1_2CYCLE"), pOut->GetCellRef("CTC1_2CYCLE"),
				   pOut->GetCellRef("CASEH1_2CYCLE"), pOut->GetCellRef("CTC1_2CYCLE"), pOut->GetCellRef("CASEH1_2CYCLE"), pOut->GetCellRef("CTC1_2CYCLE"),
				   pOut->GetCellRef("CASEH1_2CYCLE"), pOut->GetCellRef("CTC1_2CYCLE"), pOut->GetCellRef("CASEH1_2CYCLE"), pOut->GetCellRef("CTC1_2CYCLE"),
				   pOut->GetCellRef("CTC1_2CYCLE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, RED,   9, TRUE,  TA_CENTER, "USEAS1_2CYCLE");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 1);
	pOut->SetXL(pXL, "합계 :",								m_nSttRow, "M", "M", 0, BLACK);
	sText.Format("=%s+%s", pOut->GetCellRef("USEAS1_1CYCLE"), pOut->GetCellRef("USEAS1_2CYCLE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, TRUE, TA_CENTER, "USEAS1", 3);
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 0);
	sText.Format("=IF(MAX(%s:%s)<=%s,IF(%s>=%s,\"... O.K\",\"... N.G\"),IF(MAX(%s:%s)>=%s,IF(%s>=%s,\"... O.K\",\"... N.G\"),IF(%s>=MAX(%s:%s),\"... O.K\",\"... N.G\")))",
		pOut->GetCellRef("MIN_AS2"), pOut->GetCellRef("MIN_AS4"), pOut->GetCellRef("REQ_AS"), pOut->GetCellRef("USEAS1"), pOut->GetCellRef("REQ_AS"),
		pOut->GetCellRef("MIN_AS2"), pOut->GetCellRef("MIN_AS4"), pOut->GetCellRef("MIN_AS1"), pOut->GetCellRef("USEAS1"), pOut->GetCellRef("MIN_AS1"),
		pOut->GetCellRef("USEAS1"), pOut->GetCellRef("MIN_AS2"), pOut->GetCellRef("MIN_AS4"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 2, RED,   9, TRUE);

	pOut->SetXL(pXL, "[ RE-BAR RATIO CHECK ]",				m_nSttRow, "D", "D", NEXT);
	pOut->SetXL(pXL, "P",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "As / ( b × d )",						m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&%s&\" / ( \"&%s&\" × \"&%s&\" ) \"", pOut->GetCellRef("USEAS1"), pOut->GetCellRef("B"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/(%s*%s)", pOut->GetCellRef("USEAS1"), pOut->GetCellRef("B"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "P");

	pOut->SetXL(pXL, "Pmax",								m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "0.75 × Pb",							m_nSttRow, "M", "M", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.75*0.85*%s*(600/(600+%s))/%s*MAX(%s,0.65)",
					pOut->GetCellRef("FCK"), pOut->GetCellRef("FY"), pOut->GetCellRef("FY"), pOut->GetCellRef("K"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "PMAX", 5);

	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("P"), pOut->GetCellRef("PMAX"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "I", "I", 1, RED,   9, FALSE, TA_CENTER);

	sText.Format("=IF(%s>%s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("P"), pOut->GetCellRef("PMAX"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 2, RED,   9, TRUE);

	pOut->SetXL(pXL, "[ DESIGN MOMENT CHECK ]",				m_nSttRow, "D", "D", NEXT);
	pOut->SetXL(pXL, "Max.",								m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "c = 0.75×600 / (600+fy) × d",		m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.75 * 600 / (600 + %s) * %s", pOut->GetCellRef("FY"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_C");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Max.",								m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "a = 0.85 × c",						m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.85*%s", pOut->GetCellRef("MAX_C"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_A");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "a = fy * As / (0.85 * fck * b)",		m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s * %s / (0.85 * %s * %s)", pOut->GetCellRef("FY"), pOut->GetCellRef("USEAS1"), pOut->GetCellRef("FCK"), pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "a");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Max. a",								m_nSttRow, "T", "T", 0);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("a"), pOut->GetCellRef("MAX_A"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE,  TA_CENTER);

	pOut->SetXL(pXL, "jd = d - a / 2",						m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s - %s /2", pOut->GetCellRef("AVAIL_D"), pOut->GetCellRef("a"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "jd");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "C = 0.85 × fck × a × b",			m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.85 * %s * %s * %s/1000", pOut->GetCellRef("FCK"), pOut->GetCellRef("a"), pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "C");
	pOut->SetXL(pXL, "kN",								m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "T = fy × As",						m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s * %s/1000", pOut->GetCellRef("FY"), pOut->GetCellRef("USEAS1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "T");
	pOut->SetXL(pXL, "kN",								m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "ΦMn = Φfy ×As ×jd",				m_nSttRow, "F", "M", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s * %s * %s * %s/1000000", pOut->GetCellRef("BENDING"), pOut->GetCellRef("FY"), pOut->GetCellRef("USEAS1"), pOut->GetCellRef("jd"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "BENDING_MN");
	pOut->SetXL(pXL, "kN",								m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "ΦMn",								m_nSttRow, "G", "H", 0, BLACK, 9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "Mu",									m_nSttRow, "J", "J", 0, BLACK, 9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "K", "K", 0, BLACK, 9, TRUE,  TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("MAXMOMENT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "L", "N", 0, BLACK, 9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "O", "O", 0);

	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("BENDING_MN"), pOut->GetCellRef("MAXMOMENT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "I", "I", 0, RED,   9, TRUE,  TA_CENTER);

	sText.Format("=IF(%s>%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("BENDING_MN"), pOut->GetCellRef("MAXMOMENT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 2, RED,   9, TRUE);

	pOut->SetXL(pXL, "· 배력철근의 계산",					m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.6.1.4)",				m_nSttRow, "T", "T", NEXT);
	pOut->SetXL(pXL, "배력철근의 주철근에 대한 백분율 : ",	m_nSttRow, "E", "E", NEXT);
	pOut->SetXL(pXL, "120 / √L",							m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"120 / √ \"&%s/1000", pOut->GetCellRef("DEAD_L"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=120/SQRT(%s/1000)", pOut->GetCellRef("DEAD_L"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "P", 0, BLACK,  9, FALSE, TA_CENTER, "RATE", 0);
	pOut->SetXL(pXL, "%",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 67.0,									m_nSttRow, "S", "U", 0, BLACK,  9, FALSE, TA_CENTER, "ARATE", 0);
	pOut->SetXL(pXL, "%",									m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("RATE"), pOut->GetCellRef("ARATE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "R", "R", 1, RED,   9, TRUE, TA_CENTER);
	sText.Format("=\"As × \"&IF(%s>%s, ROUND(%s/100,3), ROUND(%s/100,3))&\"%% = \"&ROUND(%s,3)&\" × \"&IF(%s>%s, ROUND(%s/100, 3), ROUND(%s/100,3))",
				pOut->GetCellRef("RATE"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("RATE"), pOut->GetCellRef("REQ_AS"),
				pOut->GetCellRef("RATE"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("RATE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s * IF(%s>=%s, %s,%s)/100", pOut->GetCellRef("REQ_AS"), pOut->GetCellRef("RATE"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("RATE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "AS_BAERUCK");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", NEXT);
	pOut->SetXL(pXL, "수축·온도철근량 :",					m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 5.7.2)",		m_nSttRow, "T", "T", NEXT);
	sText.Format("=\"0.0020 × b × h  =  0.0020 × \"&%s&\" ×  \"&%s", pOut->GetCellRef("B"), pOut->GetCellRef("H"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.002 * %s * %s", pOut->GetCellRef("B"), pOut->GetCellRef("H"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "R", "T", 0, BLACK,  9, FALSE, TA_CENTER, "AS_TEMPARATURE");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "USE",									m_nSttRow, "E", "F", 0, BLACK, 9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "H",									m_nSttRow, "G", "G", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, sCaseH3,								m_nSttRow, "H", "I", 0, RED,   9, TRUE,  TA_CENTER, "CASEH3", 0);
	pOut->SetXL(pXL, "@",									m_nSttRow, "J", "J", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, dCtcB,									m_nSttRow, "K", "L", 0, RED,   9, TRUE,  TA_CENTER, "CTCB", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
				   pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"), pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"),
				   pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"), pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"),
				   pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"), pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"),
				   pOut->GetCellRef("CTCB"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, RED,   9, TRUE,  TA_CENTER, "USEAS_BAERUCK");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 1);
	sText.Format("=IF(AND(%s>=%s, %s>=%s), \"... O.K\", \"... N.G\")", pOut->GetCellRef("USEAS_BAERUCK"), pOut->GetCellRef("AS_BAERUCK"), pOut->GetCellRef("USEAS_BAERUCK"), pOut->GetCellRef("AS_TEMPARATURE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE, TA_CENTER);

	pOut->SetXL(pXL, "② 단부설계",				m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "[ Req'd d&As ]",						m_nSttRow, "D", "D", NEXT);
	pOut->SetXL(pXL, "Req'd",								m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "As",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,3)&\" mm²×  2\"", pOut->GetCellRef("REQ_AS"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "I", "M", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*2", pOut->GetCellRef("REQ_AS"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "REQAS_DANBU");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R");

	pOut->SetXL(pXL, "USE",									m_nSttRow, "E", "F", 0, BLACK, 9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "H",									m_nSttRow, "G", "G", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, sCaseH2_1Cy,							m_nSttRow, "H", "I", 0, RED,   9, TRUE,  TA_CENTER, "CASEH2_1CYCLE", 0);
	pOut->SetXL(pXL, "@",									m_nSttRow, "J", "J", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, dCtc2,									m_nSttRow, "K", "L", 0, RED,   9, TRUE,  TA_CENTER, "CTC2", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
				   pOut->GetCellRef("CASEH2_1CYCLE"), pOut->GetCellRef("CTC2"), pOut->GetCellRef("CASEH2_1CYCLE"), pOut->GetCellRef("CTC2"),
				   pOut->GetCellRef("CASEH2_1CYCLE"), pOut->GetCellRef("CTC2"), pOut->GetCellRef("CASEH2_1CYCLE"), pOut->GetCellRef("CTC2"),
				   pOut->GetCellRef("CASEH2_1CYCLE"), pOut->GetCellRef("CTC2"), pOut->GetCellRef("CASEH2_1CYCLE"), pOut->GetCellRef("CTC2"),
				   pOut->GetCellRef("CTC2"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, RED,   9, TRUE,  TA_CENTER, "USEAS_DANBU_1CYCLE");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 1);

	pOut->SetXL(pXL, "H",									m_nSttRow, "G", "G", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, sCaseH2_2Cy,							m_nSttRow, "H", "I", 0, RED,   9, TRUE,  TA_CENTER, "CASEH2_2CYCLE", 0);
	pOut->SetXL(pXL, "@",									m_nSttRow, "J", "J", 0, RED,   9, TRUE,  TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("CTC2"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "K", "L", 0, RED,   9, TRUE,  TA_CENTER, "CTC2_2CYCLE", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
				   pOut->GetCellRef("CASEH2_2CYCLE"), pOut->GetCellRef("CTC2_2CYCLE"), pOut->GetCellRef("CASEH2_2CYCLE"), pOut->GetCellRef("CTC2_2CYCLE"),
				   pOut->GetCellRef("CASEH2_2CYCLE"), pOut->GetCellRef("CTC2_2CYCLE"), pOut->GetCellRef("CASEH2_2CYCLE"), pOut->GetCellRef("CTC2_2CYCLE"),
				   pOut->GetCellRef("CASEH2_2CYCLE"), pOut->GetCellRef("CTC2_2CYCLE"), pOut->GetCellRef("CASEH2_2CYCLE"), pOut->GetCellRef("CTC2_2CYCLE"),
				   pOut->GetCellRef("CTC2_2CYCLE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, RED,   9, TRUE,  TA_CENTER, "USEAS_DANBU_2CYCLE");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 1);
	pOut->SetXL(pXL, "합계 :",								m_nSttRow, "L", "L", 0, BLACK, 9);
	sText.Format("=%s+%s", pOut->GetCellRef("USEAS_DANBU_1CYCLE"), pOut->GetCellRef("USEAS_DANBU_2CYCLE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, TRUE, TA_CENTER, "USEAS_DANBU", 3);
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 0);
	sText.Format("=IF(%s > %s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("REQAS_DANBU"), pOut->GetCellRef("USEAS_DANBU"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE);

	pOut->SetXL(pXL, "[ 배력철근 ]",						m_nSttRow, "D", "D", NEXT);
	pOut->SetXL(pXL, "Req'd",								m_nSttRow, "E", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "As",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,3)&\" mm²×  2\"", pOut->GetCellRef("AS_BAERUCK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "I", "M", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*2", pOut->GetCellRef("AS_BAERUCK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "REQAS_DANBU_BAERUCK");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R");

	pOut->SetXL(pXL, "USE",									m_nSttRow, "E", "F", 0, BLACK, 9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "H",									m_nSttRow, "G", "G", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, sCaseH4,								m_nSttRow, "H", "I", 0, RED,   9, TRUE,  TA_CENTER, "CASEH4", 0);
	pOut->SetXL(pXL, "@",									m_nSttRow, "J", "J", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, dCtcB1,								m_nSttRow, "K", "L", 0, RED,   9, TRUE,  TA_CENTER, "CTCB1", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
				   pOut->GetCellRef("CASEH4"), pOut->GetCellRef("CTCB1"), pOut->GetCellRef("CASEH4"), pOut->GetCellRef("CTCB1"),
				   pOut->GetCellRef("CASEH4"), pOut->GetCellRef("CTCB1"), pOut->GetCellRef("CASEH4"), pOut->GetCellRef("CTCB1"),
				   pOut->GetCellRef("CASEH4"), pOut->GetCellRef("CTCB1"), pOut->GetCellRef("CASEH4"), pOut->GetCellRef("CTCB1"),
				   pOut->GetCellRef("CTCB1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, RED,   9, TRUE,  TA_CENTER, "USEAS_DANBU_BAERUCK");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", 1);

	sText.Format("=IF(%s > %s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("REQAS_DANBU_BAERUCK"), pOut->GetCellRef("USEAS_DANBU_BAERUCK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE);

	pOut->SetXL(pXL, "[ 배치범위 ]",						m_nSttRow, "D", "D", NEXT);
	if(nFloorIndex != 2)
		sText.Format("=\" 캔틸레버판의 고정하중에 의한 지간 ( L = \"&%s/1000&\"m )이상 적용\"", pOut->GetCellRef("DEAD_L"));
	else
		sText.Format("=\" 고정하중에 의한 지간 ( L = \"&%s/1000&\"m )의 ½ 이상 --------------> \"&%s/2/1000&\" m 적용\"", pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("DEAD_L"));
	
	pOut->SetXL(pXL, sText,									m_nSttRow, "E", "E", 2);
}

void CXLDesignFloor::CheckUsefully(long nFloorIndex)
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	
	double	dEs			= pCalcData->DESIGN_MATERIAL.m_dEs;// 철근 탄성계수
	double	dEc			= pCalcData->DESIGN_MATERIAL.m_dEc;// 콘크리트 탄성계수
	double	dR			= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Cent_R;
	double	dMoveL		= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_L;
	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_bWalkLoad;
	double  dDroopFactor= nFloorIndex==2 ? 28 : 10;
	CString	sText;
	long	n		= long(dEs / dEc);

	double		ddWaCoeff		= pOpt->GetdCoeffDesignAllowCrackWidthEnv();
	CString		strAllowCrackWidth = pOpt->GetStrDesignAllowCrackWidthEnv();

	pOut->SetXL(pXL, "◀ 사용성 검토",						m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "① 처 짐 검 토",						m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "- 처짐을 계산하지 않는 부재의 최소 두께", m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, "(☞ 콘.설.기 4.3)",		m_nSttRow, "T", "T", NEXT);
	pOut->SetXL(pXL, "Tmin",								m_nSttRow, "C", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&IF(%s=400, %s, %s*(0.43+%s/700))&\" / %.0lf\"", pOut->GetCellRef("FY"), pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("FY"), dDroopFactor);
	pOut->SetXL(pXL, sText,									m_nSttRow, "G", "G", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=400, %s, %s*(0.43+%s/700)) / %.0lf", pOut->GetCellRef("FY"), pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("FY"), dDroopFactor);
	pOut->SetXL(pXL, sText,									m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "DROOP");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("USETHICK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "T", 0);
	sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("DROOP"), pOut->GetCellRef("USETHICK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "P", "P", 1, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"∵ 별도의 처짐검토 필요없음.\", \"∵ 별도의 처짐검토 필요.\")", pOut->GetCellRef("DROOP"), pOut->GetCellRef("USETHICK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "E", "E", 2, BLACK,  9);

	pOut->SetXL(pXL, "② 균 열 검 토",						m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 4.4.9)",		m_nSttRow, "T", "T", NEXT);
	pOut->SetXL(pXL, "사용하중하에서 최대 하중을 유발하는 하중조합을 사용한다.", m_nSttRow, "D", "D");

	pOut->SetXL(pXL, "M",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	if(bWalkLoad)
	{
		pOut->SetXL(pXL, "Md   +   Ml   +   0.3Mw",				m_nSttRow, "F", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("DEADLOAD"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU1");
		pOut->SetXL(pXL, "+",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("WALKLOAD"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "R", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU2");
		if(pOut->GetCellRef("WINDLOAD")==_T(""))
		{
			pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s + %s",pOut->GetCellRef("CRACK_MU1"),pOut->GetCellRef("CRACK_MU2"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACKCHECK");
			pOut->SetXL(pXL, "kN·m",								 m_nSttRow, "P", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
		}
		else
		{
			pOut->SetXL(pXL, "+",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=0.3 * %s", pOut->GetCellRef("WINDLOAD"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU3");
			pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s + %s + %s", pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"), pOut->GetCellRef("CRACK_MU3"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACKCHECK");
			pOut->SetXL(pXL, "kN·m",								 m_nSttRow, "P", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
		}
	}
	else if(dMoveL>0)
	{
		if(dR>0)	//pXL->SetXL(sCell1, "M  =  Md  +  Ml+i  +  Mcf  +  0.3Mw");		// 원심하중이 있는경우
		{
			pOut->SetXL(pXL, "Md  +  Ml+i  +  Mcf  +  0.3Mw",		m_nSttRow, "F", "L", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s", pOut->GetCellRef("DEADLOAD"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU1");
			pOut->SetXL(pXL, "+",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);		
//활하중...
			if(pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
				sText.Format("=MAX(%s, %s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"), pOut->GetCellRef("TRAILERLOAD"));
			else if(pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") == _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"));
			else if(pOut->GetCellRef("TANKLOAD") == _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TRAILERLOAD"));
			else
				sText.Format("=%s", pOut->GetCellRef("LIVELOAD"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "R", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU2");
//원심하중..
			pOut->SetXL(pXL, "+",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);

			if(pOut->GetCellRef("CENTLOAD") != _T("") && pOut->GetCellRef("CENTLOAD_TANK") != _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") != _T(""))
				sText.Format("=MAX(%s, %s, %s)", pOut->GetCellRef("CENTLOAD"), pOut->GetCellRef("CENTLOAD_TANK"), pOut->GetCellRef("CENTLOAD_TRAILER"));
			else if(pOut->GetCellRef("CENTLOAD") != _T("") && pOut->GetCellRef("CENTLOAD_TANK") != _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") == _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("CENTLOAD"), pOut->GetCellRef("CENTLOAD_TANK"));
			else if(pOut->GetCellRef("CENTLOAD") != _T("") && pOut->GetCellRef("CENTLOAD_TANK") == _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") != _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("CENTLOAD"), pOut->GetCellRef("CENTLOAD_TRAILER"));
			else if(pOut->GetCellRef("CENTLOAD") == _T("") && pOut->GetCellRef("CENTLOAD_TANK") != _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") != _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("CENTLOAD_TANK"), pOut->GetCellRef("CENTLOAD_TRAILER"));
			else
				sText.Format("=%s", pOut->GetCellRef("CENTLOAD"));

			pOut->SetXL(pXL, sText,									m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU3");

//풍하중...
			if(nFloorIndex!=FLOOR_MID && pOut->GetCellRef("WINDLOAD") != _T(""))
			{
				pOut->SetXL(pXL, "+",									m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=0.3 * %s", pOut->GetCellRef("WINDLOAD"));
				pOut->SetXL(pXL, sText,									m_nSttRow, "W", "X", 1, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU4");		
								
				sText.Format("=%s + %s + %s + %s",pOut->GetCellRef("CRACK_MU1"),pOut->GetCellRef("CRACK_MU2"),
												  pOut->GetCellRef("CRACK_MU3"),pOut->GetCellRef("CRACK_MU4"));
			}
			else
			{
				m_nSttRow++;
				sText.Format("=%s + %s + %s",pOut->GetCellRef("CRACK_MU1"),pOut->GetCellRef("CRACK_MU2"), pOut->GetCellRef("CRACK_MU3"));
			}
			pOut->SetXL(pXL, "=",				m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, sText,				m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACKCHECK");
			pOut->SetXL(pXL, "kN·m",			m_nSttRow, "P", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
	
		}
		else		//pXL->SetXL(sCell1, "M  =  Md  +  Ml+i  +  0.3Mw");
		{
			if(nFloorIndex==FLOOR_MID)
			{
				pOut->SetXL(pXL, "Md  +  Ml+i",		m_nSttRow, "F", "L", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=%s", pOut->GetCellRef("DEADLOAD"));
				pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU1");
				pOut->SetXL(pXL, "+",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);

				if(pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
					sText.Format("=MAX(%s, %s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"), pOut->GetCellRef("TRAILERLOAD"));
				else if(pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") == _T(""))
					sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"));
				else if(pOut->GetCellRef("TANKLOAD") == _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
					sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TRAILERLOAD"));
				else
					sText.Format("=%s", pOut->GetCellRef("LIVELOAD"));
				pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU2");

				pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=%s + %s",pOut->GetCellRef("CRACK_MU1"),pOut->GetCellRef("CRACK_MU2"));
				pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACKCHECK");
				pOut->SetXL(pXL, "kN·m",								 m_nSttRow, "P", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
			}
			else
			{
				pOut->SetXL(pXL, "Md  +  Ml+i  +  0.3Mw",		m_nSttRow, "F", "L", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=%s", pOut->GetCellRef("DEADLOAD"));
				pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU1");
				pOut->SetXL(pXL, "+",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);

				if(pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
					sText.Format("=MAX(%s, %s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"), pOut->GetCellRef("TRAILERLOAD"));
				else if(pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") == _T(""))
					sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"));
				else if(pOut->GetCellRef("TANKLOAD") == _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
					sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TRAILERLOAD"));
				else
					sText.Format("=%s", pOut->GetCellRef("LIVELOAD"));
				pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "R", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU2");

				if(pOut->GetCellRef("WINDLOAD")!="")
				{
					pOut->SetXL(pXL, "+",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
					sText.Format("=0.3 * %s", pOut->GetCellRef("WINDLOAD"));
				}
				else
					sText.Format("");				
				pOut->SetXL(pXL, sText,									m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU3");		
				
				pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=%s + %s + %s",pOut->GetCellRef("CRACK_MU1"),pOut->GetCellRef("CRACK_MU2"),pOut->GetCellRef("CRACK_MU3"));
				pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACKCHECK");
				pOut->SetXL(pXL, "kN·m",								 m_nSttRow, "P", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
			}
		}
	}
	else if(dMoveL<=0)
	{
		//pXL->SetXL(sCell1, "M  =  Md + 0.3Mw");
		pOut->SetXL(pXL, "Md   +   0.3Mw",						m_nSttRow, "F", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("DEADLOAD"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU1");
		if(pOut->GetCellRef("WINDLOAD")==_T(""))
		{
			pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s",pOut->GetCellRef("CRACK_MU1"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "CRACKCHECK");
			pOut->SetXL(pXL, "kN·m",								 m_nSttRow, "V", "X", 1, BLACK, 9, FALSE, TA_CENTER);

		}
		else
		{
			pOut->SetXL(pXL, "+",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=0.3 * %s", pOut->GetCellRef("WINDLOAD"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU2");		

			pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s + %s",pOut->GetCellRef("CRACK_MU1"),pOut->GetCellRef("CRACK_MU2"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "CRACKCHECK");
			pOut->SetXL(pXL, "kN·m",								 m_nSttRow, "V", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		}
	}
	
	pOut->SetXL(pXL, "∴ Mmax  =",							m_nSttRow, "E", "E", 0);
	sText.Format("=%s", pOut->GetCellRef("CRACKCHECK"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "H", "J", 0, RED,   9, FALSE, TA_CENTER, "CRACK_MAX");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "K", "K", 2);

	pOut->SetXL(pXL, "n",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%.0f / %.0f", dEs, dEc);
	pOut->SetXL(pXL, sText,								m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	//sText.Format("=ROUND(136/SQRT(%s),0)", pOut->GetCellRef("FCK"));
	pOut->SetXL(pXL, n,									m_nSttRow, "S", "U", 1, BLACK,  9, FALSE, TA_CENTER, "N", 0);

	pOut->SetXL(pXL, "p",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "As / ( b × d )",						m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&%s&\" / ( \"&%s&\" × \"&%s&\" )\"", pOut->GetCellRef("USEAS1"), pOut->GetCellRef("B"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/(%s*%s)", pOut->GetCellRef("USEAS1"), pOut->GetCellRef("B"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "U", 1, BLACK,  9, FALSE, TA_CENTER, "P", 6);

	pOut->SetXL(pXL, "k",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "-np + √{(np)²+ 2np}",				m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"-\"&ROUND(%s,6)&\"×\"&ROUND(%s,6)&\"+√{(\"&ROUND(%s,6)&\"×\"&ROUND(%s,6)&\")²+2×\"&ROUND(%s,6)&\"×\"&ROUND(%s,6)&\"}\"",
					pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=-%s*%s+SQRT((%s*%s)^2+2*%s*%s)", pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "U", 1, BLACK,  9, FALSE, TA_CENTER, "K", 3);

	pOut->SetXL(pXL, "j",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "1 - ( k / 3 )",						m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"1 - ( \"&ROUND(%s,3)&\" / 3 )\"", pOut->GetCellRef("K"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=1 - %s/3", pOut->GetCellRef("K"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "U", 2, BLACK,  9, FALSE, TA_CENTER, "J", 3);

	pOut->SetLineBoxText(pXL, "fsmax =",					m_nSttRow, "D", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "Mmax",						m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",							m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("=\"\"&ROUND(%s,3)&\" × 10^6\"", pOut->GetCellRef("CRACK_MAX"));
	pOut->SetUnderLineText(pXL, sText,						m_nSttRow, "L", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "p×b×j×d²",						m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,6)&\"×\"&ROUND(%s,3)&\"×\"&ROUND(%s,3)&\"×\"&ROUND(%s,3)&\"²\"", pOut->GetCellRef("P"), pOut->GetCellRef("B"), pOut->GetCellRef("J"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "L", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*10^6/(%s * %s * %s * %s^2)", pOut->GetCellRef("CRACK_MAX"), pOut->GetCellRef("P"), pOut->GetCellRef("B"), pOut->GetCellRef("J"), pOut->GetCellRef("AVAIL_D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "FSMAX");
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, "0.6×fy",								m_nSttRow, "S", "S", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=0.6*%s", pOut->GetCellRef("FY"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "V", "X", 0, BLACK,  9, FALSE, TA_CENTER, "ALLOW_FY");
	pOut->SetXL(pXL, "MPa",							m_nSttRow, "Y", "Y", 0);
	sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("FSMAX"), pOut->GetCellRef("ALLOW_FY"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "R", "R", 1, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s > %s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("FSMAX"), pOut->GetCellRef("ALLOW_FY"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE);

	pOut->SetXL(pXL, "A",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"b × h / \"&%s/%s*2&\" = \"&%s&\" × \"&%s*2&\" / \"&%s/%s*2",
					pOut->GetCellRef("B"), pOut->GetCellRef("CTC1"), pOut->GetCellRef("B"), pOut->GetCellRef("D"), pOut->GetCellRef("B"), pOut->GetCellRef("CTC1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);

	pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s*2/(%s/%s*2)", pOut->GetCellRef("B"), pOut->GetCellRef("D"), pOut->GetCellRef("B"), pOut->GetCellRef("CTC1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "U", 0, BLACK,  9, FALSE, TA_CENTER, "A");
	pOut->SetXL(pXL, "mm²",								m_nSttRow, "V", "V", 2);

	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(20);
	DrawReBarQuantity(&Dom, nFloorIndex);
	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 3, 6, FALSE, FALSE, FALSE);

	pOut->SetXL(pXL, "A  : 주인장철근 주위의 콘크리트 단면적을", m_nSttRow, "O", "O");
	pOut->SetXL(pXL, "     철근의 갯수로 나눈 유효인장 단면적", m_nSttRow, "O", "O");
	pOut->SetXL(pXL, "dc : 인장연단에서 가장 가까이 위치한", m_nSttRow, "O", "O");
	pOut->SetXL(pXL, "     철근 중심까지지의 피복두께",		m_nSttRow, "O", "O", 4);

	pOut->SetXL(pXL, "③ 균열폭 검토",						m_nSttRow, "D", "D", NEXT);
	pOut->SetXL(pXL, "㉮",									m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, "ωa",									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%.4f", ddWaCoeff);
	pOut->SetXL(pXL, sText,									m_nSttRow, "H", "I", 0, BLACK, 9, FALSE, TA_CENTER, "DWACoeff", 4);
	sText.Format("× tc (%s)", strAllowCrackWidth);
	pOut->SetXL(pXL, sText, m_nSttRow, "J", "J");
	pOut->SetXL(pXL, "여기서, tc : 철근 콘크리트 최소피복 두께 ( mm )",	m_nSttRow, "F", "F");
	pOut->SetXL(pXL, "㉯",									m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, "ωa",									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);	
	sText.Format("=%s&\"× ( \"&%s&\" - \"&%s&\" / 2 )\"", pOut->GetCellRef("DWACoeff"), pOut->GetCellRef("D"), pOut->GetCellRef("CASEH1_1CYCLE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "H", "H", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s * (%s-%s/2)", pOut->GetCellRef("DWACoeff"), pOut->GetCellRef("D"), pOut->GetCellRef("CASEH1_1CYCLE"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "U", 0, BLACK,  9, FALSE, TA_CENTER, "WA");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "V", "V", NEXT);

	pOut->SetXL(pXL, "㉰",									m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, "βc",									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "단면의 중립축에서 인장연단까지의 거리 / 단면의 중립축에서 철근도심까지의 거리", m_nSttRow, "H", "H", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"( \"&ROUND(%s,3)&\" - \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" ) / ( \"&ROUND(%s,3)&\" - \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" - \"&ROUND(%s,3)&\" )\"",
				pOut->GetCellRef("H"), pOut->GetCellRef("K"), pOut->GetCellRef("AVAIL_D"), pOut->GetCellRef("H"), pOut->GetCellRef("K"), pOut->GetCellRef("AVAIL_D"), pOut->GetCellRef("D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "H", "H", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s-%s*%s)/(%s-%s*%s-%s)", pOut->GetCellRef("H"), pOut->GetCellRef("K"), pOut->GetCellRef("AVAIL_D"),
					pOut->GetCellRef("H"), pOut->GetCellRef("K"), pOut->GetCellRef("AVAIL_D"), pOut->GetCellRef("D"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER, "BC");

	pOut->SetXL(pXL, "㉱",									m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, "ω",									m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "1.08  ×  βc  ×  fs  ×  ³√( dc × A ) × 0.00001", m_nSttRow, "H", "H", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"1.08  ×  \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" × ³√( \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" )  × 0.00001\"",
					pOut->GetCellRef("BC"), pOut->GetCellRef("FSMAX"), pOut->GetCellRef("D"), pOut->GetCellRef("A"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "H", "H", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=1.08 * %s * %s * (%s*%s)^(1/3) * 0.00001", pOut->GetCellRef("BC"), pOut->GetCellRef("FSMAX"), pOut->GetCellRef("D"), pOut->GetCellRef("A"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "ALLOW_W");
	pOut->SetXL(pXL, "mm",									m_nSttRow, "K", "K", 0);
	pOut->SetXL(pXL, "ωa",									m_nSttRow, "O", "O", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("WA"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "mm",									m_nSttRow, "T", "T", 0);
	sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("ALLOW_W"), pOut->GetCellRef("WA"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "M", "M", 1, RED,   9, TRUE,  TA_CENTER);
	sText.Format("=IF(%s > %s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("ALLOW_W"), pOut->GetCellRef("WA"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE);

	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(10);
	DrawCrackWidthCheck(&Dom, nFloorIndex);
	DimCrackWidthCheck(&Dom, nFloorIndex);
	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 2, 20);
	SetLines(NEXT);
}
// 경험적 설계검토시
void CXLDesignFloor::DesignExperience()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	CString szText = _T(""), sLeftGuard = _T(""), sRightGuard = _T(""), szTemp = _T("");
	
	//////////////////////////////////////////////////////////////////////////
	CCalcGeneral		CalcGeneral(pDataManage);
	CPlateGirderApp		*pGirFir	= pDB->GetGirder(0);
	CPlateGirderApp		*pGirEnd	= pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBxFinder		FinderStt(pGirFir);
	CPlateBxFinder		FinderEnd(pGirEnd);
	CPlateBasicIndex	*pBxFir		= FinderStt.GetBxFirst(BX_CROSSBEAM_VBRACING);	
	CPlateBasicIndex	*pBxEnd		= FinderEnd.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
	double	dSlabThick		= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;					// 바닥판 두께
	double	dLengthCen		= CalcGeneral.CalcCentPart_L();										// 유효지간
	double	dLengthLeft		= pDB->m_dWidthSlabLeft - pGirFir->GetThickJewon(G_W, pBxFir)/2;	// 좌측 캔틸레버부 끝단에서 첫번째거더 좌측복부판 중앙까지 거리
	double	dLengthRight	= pDB->m_dWidthSlabRight - pGirEnd->GetThickJewon(G_W, pBxEnd)/2;	// 우측 캔틸레버부 끝단에서 마지막거더 우측복부판 중앙까지 거리
	double	dCoverUp		= pCalcData->m_dCoverUp;											// 철근덮게(상면)
	double	dCoverDn		= pCalcData->m_dCoverDn;

	CCentSeparation	*pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::LEFT);
	if(pCentSeparation)	sLeftGuard	= _T("(방호책있음)");
	else				sLeftGuard	= _T("(방호책없음)");
	pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
	if(pCentSeparation)	sRightGuard	= _T("(방호책있음)");
	else				sRightGuard	= _T("(방호책없음)");

	CString	sRebar					= pCalcData->DESIGN_MATERIAL.m_sSigmaY;	// H
	double	dMainDiaUpper_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy];	// 주철근 상면 Dia Idx 1-Cylce
	double	dMainDiaUpper_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy];	// 주철근 상면 Dia Idx 2-Cycle
	double	dMainCTCUpper			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1;					// 주철근 상면 간격
	double	dMainDiaLower_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1];			// 주철근 하면 Dia Idx 1-Cylce
	double	dMainCTCLower			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1;						// 주철근 하면 간격
	double	dBaeRuckDiaUpper		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2];		// 배력철근 상면 Dia Idx
	double	dBaeRuckCTCUpper		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2;					// 배력철근 상면 간격
	double	dBaeRuckDiaLower		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3];			// 배력철근 하면 Dia Idx
	double	dBaeRuckCTCLower		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB;						// 배력철근 하면 간격
	double	dEndReinforceDiaMain_1Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2];			// 단부보강 주철근 Dia Idx 1-Cycle
	double	dEndReinforceDiaMain_2Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy];		// 단부보강 주철근 Dia Idx 2-Cycle
	double	dEndReinforceCTCMain	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// 단부보강 주철근 간격
	double	dEndReinforceDiaBeryuk	= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4];			// 단부보강 배력철근 Dia Idx
	double	dEndReinforceCTCBeryuk	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1;						// 단부보강 배력철근 간격
	//////////////////////////////////////////////////////////////////////////
	
	m_nSttRow = 1;

	szTemp = pXL->GetSheetName();
	szText = szTemp.Left(szTemp.Find("경")-1);
	szText += _T(" 경험적 설계법에 의한 바닥판 슬래브계산");

	pOut->SetXL(pXL, szText,			m_nSttRow,  "A",  "A", 3, BLACK, 9, TRUE);
	pOut->SetXL(pXL, dSlabThick,		m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dLengthCen,		m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dCoverUp,			m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dCoverDn,			m_nSttRow, "AL", "AN", 9, BLACK, 9, FALSE, TA_CENTER, "", 0);
	
	pOut->SetXL(pXL, sLeftGuard,		m_nSttRow,  "M",  "Q", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, toM(dLengthLeft),	m_nSttRow,  "S",  "U", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRightGuard,		m_nSttRow,  "M",  "Q", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, toM(dLengthRight),	m_nSttRow,  "S",  "U",12, RED, 9, FALSE, TA_CENTER);

	// 주철근-상면철근
	pOut->SetXL(pXL, sRebar,			m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dMainDiaUpper_1Cy,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTCUpper,		m_nSttRow,  "Y",  "Z", 1, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, sRebar,			m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dMainDiaUpper_2Cy,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTCUpper,		m_nSttRow,  "Y",  "Z", 2, RED, 9, FALSE, TA_CENTER, "", 0);

	// 주철근-하면철근
	pOut->SetXL(pXL, sRebar,			m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dMainDiaLower_1Cy,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTCLower,		m_nSttRow,  "Y",  "Z", 1, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, sRebar,			m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dMainDiaLower_1Cy,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTCLower,		m_nSttRow,  "Y",  "Z", 2, RED, 9, FALSE, TA_CENTER, "", 0);

	// 배력철근-상면철근
	pOut->SetXL(pXL, sRebar,			m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dBaeRuckDiaUpper,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dBaeRuckCTCUpper,	m_nSttRow,  "Y",  "Z", 2, RED, 9, FALSE, TA_CENTER, "", 0);

	// 배력철근-하면철근
	pOut->SetXL(pXL, sRebar,			m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dBaeRuckDiaLower,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dBaeRuckCTCLower,	m_nSttRow,  "Y",  "Z", 2, RED, 9, FALSE, TA_CENTER, "", 0);

	// 단부보강철근-주철근
	pOut->SetXL(pXL, sRebar,					m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dEndReinforceDiaMain_1Cy,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dEndReinforceCTCMain,		m_nSttRow,  "Y",  "Z", 1, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, sRebar,					m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dEndReinforceDiaMain_2Cy,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dEndReinforceCTCMain,		m_nSttRow,  "Y",  "Z", 2, RED, 9, FALSE, TA_CENTER, "", 0);

	// 단부보강철근-배력철근
	pOut->SetXL(pXL, sRebar,					m_nSttRow,  "U",  "U", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dEndReinforceDiaBeryuk,	m_nSttRow,  "V",  "W", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dEndReinforceCTCBeryuk,	m_nSttRow,  "Y",  "Z", 1, RED, 9, FALSE, TA_CENTER, "", 0);
}

void CXLDesignFloor::DesignFloor_Center()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();

	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[2].m_bWalkLoad;
	BOOL	bTank		= pCalcData->m_bTank;
	BOOL	bTrailer	= pCalcData->m_bTrailer;

	Initialize();
	pOut->SetXL(pXL, "3. 바 닥 판 설 계",		m_nSttRow, "A", "A", 1, BLACK, 11, TRUE);
	CalcDeadLoad_Center();							///< 고정하중
	if(bWalkLoad)
	{
		CalcWalkLoad();								///< 보도하중
	}
	else if(pCalcFloor->IsLaneLoad(FLOOR_MID))
	{
		CalcLiveLoad_Center();						///< 활하중
		if(bTank)		CalcTankLoad_Center();		///< 탱크하중
		if(bTrailer)	CalcTrailerLoad_Center();	///< 트레일러 하중
		CalcCentriodLoad_Center();	///< 원심하중
	}
	
	CheckMinThick(FLOOR_MID);								///< 최소두께 검토
	pOut->BackUpCellRef();

	long nSttRow = m_nSttRow;
	if(!pDB->IsPlateGir())
	{
		pOut->OnSheetRefMode(TRUE);
		m_nSttRow = 5;
		CString szSheetName = pXL->GetSheetName();
		pXL->SetActiveSheet("3.7 극한하중에 대한 단면검토");
		CalcComposeLoad_Center(FALSE); // 여기에 나오는 코드는 "3.3 중앙지간부"에 쓰는게 아니라 "3.7 극한하중"에 쓰는 부분이다.
		m_nUltSttRow[2] = m_nSttRow;   // 하중조합 이후 Row를 별도로 저장
		if(m_nSttRow < 17)
		{
			pXL->DeleteRowLine(m_nSttRow, 17);
			pXL->DeleteRowLineEnd();
			pXL->InsertRowLine(m_nSttRow+57, 18-m_nSttRow);
		}
		pXL->SetActiveSheet(szSheetName);
		pOut->BackUpCellRef();
		pOut->OnSheetRefMode(FALSE);
	}
	
	m_nSttRow = nSttRow;
	CalcComposeLoad_Center(); // 여기에 나오는 코드는 "3.3 중앙지간부"에 쓰는 부분이다.

// 	if(!pDB->m_bTendonInputComplete)
// 		DesignOfSection(FLOOR_MID);

	if(pDB->m_nSlabTensionSize==0)
		CheckFloorRebarCalc(eFloor_Center, CStringArray(), FALSE);
	
	pOut->OnBackUpCellRef(FALSE);
}

// 1) 중앙지간 - 사하중
void CXLDesignFloor::CalcDeadLoad_Center()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcGeneral		CalcGeneral(pDataManage);
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CStringArray		ArrDeadLoad;

	pCalcFloor->CalcSlabCenterDeadLoad(ArrDeadLoad);
	long	nGsu		= pDB->GetGirdersu();
	long	nSize		= ArrDeadLoad.GetSize();
	long	nGInner		= 0;
	double	dThickInner	= 0;
	double	dGirToGir	= 0;
	double	dDeadL		= toM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Dead_L); //toM(CalcGeneral.CalcCentPart_L());
	//double	dPave		= pDB->m_dThickPave;
	double	dSlabBase	= pDB->m_dThickSlabBase;
	double  dSlabHunch	= pDB->m_dThickSlabHunch;

	CString	sText;

	nGInner = (long)(nGsu/2-0.5);
	CPlateGirderApp		*pGir	= pDB->GetGirder(nGInner);
	CPlateBasicIndex	*pBx	= pGir->GetCrossBxAtMinFlangeWidth();

	dThickInner = toM(dSlabBase-dSlabHunch);//toM(dSlabBase-dPave);
	dGirToGir	= toM(pGir->GetDisGirderToGirder(pBx));

	pOut->SetXL(pXL, "3.3 중앙지간부",				m_nSttRow, "B", "B", 2);
	sText.Format("%s 고 정 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,							m_nSttRow, "C", "C", 1);

	for(long n = 0; n < nSize-7; n+=4)
	{
		pOut->SetXL(pXL, "·",						m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, ArrDeadLoad.GetAt(n),		m_nSttRow, "E", "O", 0);
		pOut->SetXL(pXL, ArrDeadLoad.GetAt(n+1),	m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "WD");
		pOut->SetXL(pXL, "kN",					m_nSttRow, "S", "S", 1);
	}

	pOut->SetXL(pXL, "계산지간 L",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);

	if(nGsu > 2)
	{
			pOut->SetXL(pXL, dDeadL,	m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "CEN_DEAD_L");		
			pOut->SetXL(pXL, "m",		m_nSttRow, "J", "J", 0);

		double	dResult1		=	0.0;
		double	dResult			=	0.0;
		double	dMaxdL			=	0.0;
		double	dMaxdTW			=	0.0;
		double	dMaxFlangeWid	=	0.0;

		for(long nG = 0; nG < nGsu - 1; nG++)
		{
			CPlateGirderApp  *pGir	=	pDB->GetGirder(nG);
			CPlateGirderApp  *pGirN =	pDB->GetGirder(nG+1);
			CPlateBasicIndex *pBx   =	pGir->GetBxOnJijum(0);
			CPlateBasicIndex *pBxN  =	pGirN->GetBxOnJijum(0);
			
			double dL	=	ABS(pGir->m_dGirderCenter-pGirN->m_dGirderCenter);
			double dTW	=	pGir->GetThickJewon(G_W,pBx);
			
			dResult = dL - dTW - (pGir->GetMaxWidthFlange(TRUE) - dTW) /2;
			
			if(dResult > dResult1)
			{
				dMaxdL			=	dL;
				dMaxdTW			=	dTW;
				dMaxFlangeWid	=	pGir->GetMaxWidthFlange(TRUE);
			}
		}

		sText.Format("(%.1f - %.3f - (%.2f - %.3f) / 2)", toM(dMaxdL),	toM(dMaxdTW),	toM(dMaxFlangeWid),	toM(dMaxdTW));
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"K", "V",	2);
		
	}
	else
	{
		if(dGirToGir > (dDeadL+dThickInner))
		{
			pOut->SetXL(pXL, dDeadL,						m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "INNER_DEAD_L");
			pOut->SetXL(pXL, "+",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dThickInner,					m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, "THICK_SLAB");
			pOut->SetXL(pXL, "=",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s+%s", pOut->GetCellRef("INNER_DEAD_L"), pOut->GetCellRef("THICK_SLAB"));
			pOut->SetXL(pXL, sText,							m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "CEN_DEAD_L");
			pOut->SetXL(pXL, "m",							m_nSttRow, "Q", "Q", 1);
		}
		else
		{
			pOut->SetXL(pXL, dGirToGir,						m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "CEN_DEAD_L");
			pOut->SetXL(pXL, "m",							m_nSttRow, "J", "J", 1);
		}

		pOut->SetXL(pXL, "· 단순지지의 경우 바닥판의 지간은 지지거더의 중심간격으로 설정한다.", m_nSttRow, "C", "C", 1);
		pOut->SetXL(pXL, "   이때 지간은 바닥판의 순지간에 바닥판 두께를 더한 길이를 초과할 필요는 없다.", m_nSttRow, "C", "C", 2);
	}
	pOut->SetXL(pXL, "∴",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Md",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Wd × L²/10",				m_nSttRow, "G", "J", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s^2/10", pOut->GetCellRef("WD"), pOut->GetCellRef("CEN_DEAD_L"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER, "DEADLOAD");
	pOut->SetXL(pXL, "kN·m",						m_nSttRow, "O", "O", 2);
}

// 2) 활하중
void CXLDesignFloor::CalcLiveLoad_Center()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcGeneral CalcGeneral(pDataManage);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	double	dDeadL			= toM(CalcGeneral.CalcCentPart_L());
	double	dPr				= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	CString	sGradeBridge	= pDB->m_sDeungeub;
	CString	sText;

	if(dDeadL < 0) return;

	sText.Format("%s 활 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, sGradeBridge,							m_nSttRow, "G", "G", 1);
	pOut->SetXL(pXL, "Pr",									m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPr,									m_nSttRow, "F", "H", 0, BLACK,  9, TRUE,  TA_CENTER, "PR");
	pOut->SetXL(pXL, "kN",								m_nSttRow, "I", "I", 0);
	pOut->SetXL(pXL, "L",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("CEN_DEAD_L"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "S", 0, BLACK,  9, TRUE,  TA_CENTER, "DEAD_L");
	pOut->SetXL(pXL, "m",									m_nSttRow, "T", "T", 2);

	pOut->SetXL(pXL, "① 충 격 계 수",						m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "i",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, " 15 / (40 + L)",						m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" 15 / ( 40 + \"&ROUND(%s,3)&\" )\"",	pOut->GetCellRef("DEAD_L"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=15/ (40 + %s)",				pOut->GetCellRef("DEAD_L"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "IMPACTCOEFF");
	pOut->SetXL(pXL, 0.3,									m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "AIMPACTCOEFF", 3);
	sText.Format("=IF(%s > %s, \">\", \"<\")",				pOut->GetCellRef("IMPACTCOEFF"), pOut->GetCellRef("AIMPACTCOEFF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", 1, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "∴i",									m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=MIN(%s, %s)",							pOut->GetCellRef("IMPACTCOEFF"), pOut->GetCellRef("AIMPACTCOEFF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "IMPACT");
	pOut->SetXL(pXL, "적용",								m_nSttRow, "I", "I", 2);

	pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( L + 0.6 ) / 9.6 × Pr × ( 1 + i )",m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"( \"&ROUND(%s,3)&\" + 0.6 ) / 9.6 × \"&ROUND(%s,3)&\" × ( 1 + \"&ROUND(%s,3)&\" )\"",
					pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("PR"), pOut->GetCellRef("IMPACT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s+0.6)/9.6*%s*(1+%s)", pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("PR"), pOut->GetCellRef("IMPACT"));
	if(pDB->GetGirdersu() > 2)
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "ML");
	else
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "LIVELOAD");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

	if(pDB->GetGirdersu() > 2)
	{
		pOut->SetXL(pXL, "연속바닥판 이므로,",					m_nSttRow, "D", "D", NEXT);
		pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"0.8 × \"&ROUND(%s,3)", pOut->GetCellRef("ML"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.8*%s", pOut->GetCellRef("ML"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER, "LIVELOAD");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);
	}

	SetLines(NEXT);
}

///< 3) 탱크하중
void CXLDesignFloor::CalcTankLoad_Center()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	double	dTankWeight	= pCalcData->m_TankWeight;
	double	dL			= pCalcData->CALC_CANTILEVER_DATA[2].m_Dead_L;
	double	dTankEdit1	= pCalcData->m_TankEdit1;	///< 탱크 궤도 접지 길이
	
	if(dL < 0)		return;

	CString	sText;
	sText.Format("%s 탱 크 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);

	pOut->SetXL(pXL, "· 탱크 총 중량 :",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, dTankWeight,							m_nSttRow, "H", "J", 0, BLACK,  9, FALSE, TA_CENTER, "TANKWEIGHT");
	pOut->SetXL(pXL, "tonf",								m_nSttRow, "K", "K", 0);
	pOut->SetXL(pXL, "· 궤도 접지 길이 :",					m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, dTankEdit1,							m_nSttRow, "S", "U", 0, BLACK,  9, FALSE, TA_CENTER, "TANKEDIT1");
	pOut->SetXL(pXL, "m",									m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "Pr",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "W  /  2  /  L ( 궤도 접지 길이 )",	m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" \"&ROUND(%s,3)&\"  /  2  /  \"&ROUND(%s,3)", pOut->GetCellRef("TANKWEIGHT"), pOut->GetCellRef("TANKEDIT1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "O", "O", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/2.0/%s", pOut->GetCellRef("TANKWEIGHT"), pOut->GetCellRef("TANKEDIT1"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "TANKPR");
	pOut->SetXL(pXL, "kN/m",								m_nSttRow, "I", "I", NEXT);

	pOut->SetXL(pXL, "충격계수  i = ",						m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, 0.15,									m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER, "TANK_IMPACT");

	pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( L + 0.6 ) / 9.6 × Pr × ( 1 + i )",m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"( \"&ROUND(%s,3)&\" + 0.6 ) / 9.6 × \"&ROUND(%s,3)&\" × ( 1 + \"&ROUND(%s,3)&\" )\"",
					pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("TANKPR"), pOut->GetCellRef("TANK_IMPACT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s+0.6)/9.6*%s*(1+%s)", pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("TANKPR"), pOut->GetCellRef("TANK_IMPACT"));
	if(pDB->GetGirdersu() > 2)
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "ML_TANK");
	else
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "TANKLOAD");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

	if(pDB->GetGirdersu() > 2)
	{
		pOut->SetXL(pXL, "연속바닥판 이므로,",					m_nSttRow, "D", "D", NEXT);
		pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"0.8 × \"&ROUND(%s,3)", pOut->GetCellRef("ML_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.8*%s", pOut->GetCellRef("ML_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER, "TANKLOAD");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);
	}

	SetLines(NEXT);
}

///< 4) 트레일러 하중
void CXLDesignFloor::CalcTrailerLoad_Center()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	double	dTrailer	= max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
	double	dDeadL		= toM(pCalcData->CALC_CANTILEVER_DATA[2].m_Dead_L);
	CString	sText;
	if(dDeadL < 0)	return;

	sText.Format("%s 탱크 트레일러 하중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", 0);

	pOut->SetXL(pXL, "( 70 kN :  Pr = ",					m_nSttRow, "J", "J", 0);
	pOut->SetXL(pXL, dTrailer,								m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "TRAILERPR");
	pOut->SetXL(pXL, "kN )",								m_nSttRow, "R", "R", NEXT);
	pOut->SetXL(pXL, "충격계수  i = ",						m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, 0.15,									m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER, "TRAILER_IMPACT");

	pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( L + 0.6 ) / 9.6 × Pr × ( 1 + i )",m_nSttRow, "F", "F", 1);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"( \"&ROUND(%s,3)&\" + 0.6 ) / 9.6 × \"&ROUND(%s,3)&\" × ( 1 + \"&ROUND(%s,3)&\" )\"",
					pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILER_IMPACT"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s+0.6)/9.6*%s*(1+%s)", pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILER_IMPACT"));
	if(pDB->GetGirdersu() > 2)
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "ML_TRAILER");
	else
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "TRAILERLOAD");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

	if(pDB->GetGirdersu() > 2)
	{
		pOut->SetXL(pXL, "연속바닥판 이므로,",					m_nSttRow, "D", "D", NEXT);
		pOut->SetXL(pXL, "Ml+i",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"0.8 × \"&ROUND(%s,3)", pOut->GetCellRef("ML_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.8*%s", pOut->GetCellRef("ML_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER, "TRAILERLOAD");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);
	}

	SetLines(NEXT);
}
///< 4) 원심하중
void CXLDesignFloor::CalcCentriodLoad_Center()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	//CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	double	dR			= pCalcData->CALC_CANTILEVER_DATA[2].m_Cent_R;
	double	dV			= pCalcData->m_dSpeed;
	//double	dCF			= pCalcGeneral->CalcCentriodLoad_CF(dV, dR);
	//double	dPcf1		= pCalcData->CALC_CANTILEVER_DATA[2].m_Cent_Pcf/pow(10,6);
	//double	dMcf1		= pCalcData->CALC_CANTILEVER_DATA[2].m_Cent_Mcf/pow(10,6);
	double	dV_Tank		= 40.0;
	BOOL	bTank		= pCalcData->m_bTank;
	BOOL	bTrailer	= pCalcData->m_bTrailer;

	// #APLATE-660, 원심하중에 대한 내용이 없어도 무조건 출력한다.
	CString	sText;
	if(dR <= 0)
	{
		sText.Format("%s 원 심 하 중", m_ArrayNumber.GetAt(m_nNumber++));
		pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "※ 원심하중에 대한 내용이 없습니다.", m_nSttRow, "D", "D", NEXT);
		SetLines(NEXT);
		return ;
	}

	sText.Format("%s 원 심 하 중", m_ArrayNumber.GetAt(m_nNumber++));
	pOut->SetXL(pXL, sText,									m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "·설계속도 V = ",						m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dV,									m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "VELOCITY");
	pOut->SetXL(pXL, "km/h",								m_nSttRow, "K", "K", 0);
	pOut->SetXL(pXL, "·곡률반지름 R = ",					m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, dR,									m_nSttRow, "S", "U", 1, BLACK, 9, FALSE, TA_CENTER, "R");
	pOut->SetXL(pXL, "·탱크 설계속도 = ",					m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dV_Tank,								m_nSttRow, "I", "K", 0, BLACK,  9, FALSE, TA_CENTER, "VELOCITY_TANK");
	pOut->SetXL(pXL, "km/h",								m_nSttRow, "L", "L", NEXT);

	if(bTank || bTrailer)
		pOut->SetXL(pXL, "· 일반활하중 작용시",			m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "CF",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "0.79  ×  (  V² /  R  ) (%)",		m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" 0.79 ×  (  \"&ROUND(%s,3)&\"²  /  \"&ROUND(%s, 3)&\"  ) (%%) \"", pOut->GetCellRef("VELOCITY"), pOut->GetCellRef("R"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=0.79*%s^2/%s", pOut->GetCellRef("VELOCITY"), pOut->GetCellRef("R"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "CF");
	pOut->SetXL(pXL, "(%)",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "E",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "2.4 × L / ( L + 0.6)",				m_nSttRow, "F", "F",NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" 2.4 ×  \"&ROUND(%s,3)&\"  /  ( \"&ROUND(%s, 3)&\" + 0.6 ) \"", pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("DEAD_L"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=2.4*%s/(%s+0.6)", pOut->GetCellRef("DEAD_L"), pOut->GetCellRef("DEAD_L"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 1, BLACK, 9, FALSE, TA_CENTER, "TRAILERE");

	pOut->SetXL(pXL, "Pcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "( Pr / E ) × CF / 100.0",			m_nSttRow, "F", "F", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\" ( \"&ROUND(%s,3)&\" / \"&ROUND(%s,3)&\" ) × \"&ROUND(%s,3)&\" / 100.0\"", pOut->GetCellRef("PR"), pOut->GetCellRef("TRAILERE"), pOut->GetCellRef("CF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=%s/%s*%s/100.0", pOut->GetCellRef("PR"), pOut->GetCellRef("TRAILERE"), pOut->GetCellRef("CF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "PCF");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

	pOut->SetXL(pXL, "Mcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Pcf × 1.8",							m_nSttRow, "F", "F", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=\"\"&ROUND(%s,3)&\" × 1.8\"", pOut->GetCellRef("PCF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", NEXT);
	pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*1.8", pOut->GetCellRef("PCF"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "CENTLOAD");
	pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

	if(bTank)
	{
		pOut->SetXL(pXL, "· 탱크하중 작용시",					m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "CF",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "0.79  ×  (  V² /  R  ) (%)",		m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" 0.79 ×  (  \"&ROUND(%s,3)&\"²  /  \"&ROUND(%s, 3)&\"  ) (%%) \"", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.79*%s^2/%s", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "CF_TANK");
		pOut->SetXL(pXL, "(%)",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "Pcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, " Pr × CF / 100.0",					m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" / 100.0\"", pOut->GetCellRef("TANKPR"), pOut->GetCellRef("CF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*%s/100", pOut->GetCellRef("TANKPR"), pOut->GetCellRef("CF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "PCF_TANK");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

		pOut->SetXL(pXL, "Mcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Pcf × 1.8",							m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"\"&ROUND(%s,3)&\" × 1.8\"", pOut->GetCellRef("PCF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*1.8", pOut->GetCellRef("PCF_TANK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "CENTLOAD_TANK");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);
	}

	if(bTrailer)
	{	
		pOut->SetXL(pXL, "· 탱크트레일러 하중 작용시",			m_nSttRow, "C", "C", NEXT);
		pOut->SetXL(pXL, "CF",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "0.79  ×  (  V² /  R  ) (%)",		m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" 0.79 ×  (  \"&ROUND(%s,3)&\"²  /  \"&ROUND(%s, 3)&\"  ) (%%) \"", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.79*%s^2/%s", pOut->GetCellRef("VELOCITY_TANK"), pOut->GetCellRef("R"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "CF_TRAILER");
		pOut->SetXL(pXL, "(%)",									m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "Pcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, " Pr/E × CF / 100.0",					m_nSttRow, "F", "F", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\" \"&ROUND(%s,3)&\" /  \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" / 100.0\"", pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILERE"), pOut->GetCellRef("CF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F");
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/%s*%s/100", pOut->GetCellRef("TRAILERPR"), pOut->GetCellRef("TRAILERE"), pOut->GetCellRef("CF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, BLACK,  9, FALSE, TA_CENTER, "PCF_TRAILER");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);

		pOut->SetXL(pXL, "Mcf",									m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Pcf × 1.8",							m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"\"&ROUND(%s,3)&\" × 1.8\"", pOut->GetCellRef("PCF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*1.8", pOut->GetCellRef("PCF_TRAILER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "CENTLOAD_TRAILER");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "I", "I", NEXT);
	}

	pOut->SetXL(pXL, "",										m_nSttRow, "Z", "Z", NEXT);
}

///< 5) 하중조합 - 극한모멘트계산
void CXLDesignFloor::CalcComposeLoad_Center( BOOL bCount /*= TRUE*/ )
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	
	CString	sText;

	double	dD		= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Stop_M),3);
	double	dML1	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml),3);
	double	dML2	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Tank),3);
	double	dML3	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Trailer),3);
	double	dMcf1	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf),3);
	double	dMcf2	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Tank),3);
	double	dMcf3	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Trailer),3);
	double	dW		= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Wind_Mw),3);
	double	dMu_1	= Round(1.3 * dD + 2.15 * dML1 + 1.3 * dMcf1,3);
	double	dMu_2	= Round(1.3 * dD +  1.3 * dML2 + 1.3 * dMcf2,3);
	double	dMu_3	= Round(1.3 * dD +  1.3 * dML3 + 1.3 * dMcf3,3);
	double	dMu_Use	= Round(dD + dML1 + 0.3 * dW, 3);

	if(bCount)
		sText.Format("%s 하 중 조 합", m_ArrayNumber.GetAt(m_nNumber++));
	else
		sText.Format("%s 하 중 조 합", m_ArrayNumber.GetAt(0));

	pOut->SetXL(pXL, sText,							m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 P32)",			m_nSttRow, "T", "T", NEXT);

	pOut->SetXL(pXL, "- 최대 계수 모멘트 계산",			m_nSttRow, "C", "C", NEXT);

	sText.Format("① Mu = 1.3D + 2.15(L+i) + 1.3CF");
	pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

	pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("1.3 × %.3f + 2.15 × %.3f + 1.3 × %.3f", dD, dML1, dMcf1);
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", dMu_1);
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

	if(pCalcData->m_bTank)
	{
		dMu_1 = dMu_1 > dMu_2 ? dMu_1 : dMu_2;

		sText.Format("② Mu = 1.3D + 1.3(L+i) + 1.3CF");
		pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

		pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("1.3 × %.3f +  1.3 × %.3f + 1.3 × %.3f", dD, dML2, dMcf2);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

		pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

		sText.Format("%.3f", dMu_2);
		pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "kN.m/m( 탱크궤도하중작용시 )",			m_nSttRow, "I", "I", NEXT);
	}

	if(pCalcData->m_bTrailer)
	{
		dMu_1 = dMu_1 > dMu_3 ? dMu_1 : dMu_3;

		if(!pCalcData->m_bTank)
		{
			sText.Format("② Mu = 1.3D + 1.3(L+i) + 1.3CF");
			pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

			pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
			pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("1.3 × %.3f +  1.3 × %.3f + 1.3 × %.3f", dD, dML3, dMcf3);
			pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

			pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%.3f", dMu_3);
			pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "kN.m/m( 탱크트레일러하중작용시 )",		m_nSttRow, "I", "I", NEXT);
		}
		else
		{
			sText.Format("③ Mu = 1.3D + 1.3(L+i) + 1.3CF");
			pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

			pOut->SetXL(pXL, "Mu",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
			pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("1.3 × %.3f +  1.3 × %.3f + 1.3 × %.3f", dD, dML3, dMcf3);
			pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

			pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

			sText.Format("%.3f", dMu_3);
			pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "kN.m/m( 탱크트레일러하중작용시 )",		m_nSttRow, "I", "I", NEXT);
		}
	}

	SetLines(NEXT);

	pOut->SetXL(pXL, "▷ 최대 계수 모멘트는 하중조합중에서 가장 큰 값을 사용",	m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "Mu",	m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",	m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", dMu_1);
	pOut->SetXL(pXL, sText,	m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "kN·m/m",	m_nSttRow, "I", "I", NEXT);

	SetLines(NEXT);

	pOut->SetXL(pXL, "- 최대 사용 모멘트 계산",			m_nSttRow, "C", "C", NEXT);

	sText.Format("① Ms = D + Li + 0.3W");
	pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", NEXT);

	pOut->SetXL(pXL, "Ms",										m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f + %.3f + 0.3 × %.3f", dD, dML1, dW);
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "F", NEXT);

	pOut->SetXL(pXL, "=",										m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", dMu_Use);
	pOut->SetXL(pXL, sText,										m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "kN·m/m",									m_nSttRow, "I", "I", NEXT);

	SetLines(NEXT);

	pOut->SetXL(pXL, "▷ 최대 사용 모멘트는 하중조합중에서 가장 큰 값을 사용",	m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "Ms",	m_nSttRow, "C", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",	m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("%.3f", dMu_Use);
	pOut->SetXL(pXL, sText,	m_nSttRow, "F", "H", 0, RED,   9, TRUE,  TA_CENTER);
	pOut->SetXL(pXL, "kN·m/m",	m_nSttRow, "I", "I", NEXT);

	m_bComposeOnlyOne = FALSE;
}

void CXLDesignFloor::CheckInnerJijum()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage		= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData			= pDataManage->GetCalcData();
	CXLControl			*pXL				= m_pXL;
	CAPlateOutCalcXL	*pOut				= m_pAPlateOutCalcXL;
	CCalcFloor			*pCalcFloor			= m_pAPlateOutCalcStd->GetCalcFloor();
	Initialize();

	double	dH		= pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;//pCalcData->CALC_CANTILEVER_DATA[2].m_Min_T;
	double	dB		= (pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_b);//toCm
	double	dCtc1	= pCalcData->m_Ctc1;
	double	dCtc2	= pCalcData->m_Ctc2;
	CString	sCaseH1	= pCalcData->m_CaseH1;
	CString sCaseH2	= pCalcData->m_CaseH2;
	CString	sRebar	= pCalcData->DESIGN_MATERIAL.m_sSigmaY;
	CString	sText;

	//주장률 검토
	double	dAsEa1	= dB / dCtc1;
	double	dAsEa2	= dB / dCtc2;
	double	dDia1	= DiaReturn((long)atof(sCaseH1));
	double	dDia2	= DiaReturn((long)atof(sCaseH2));
	double	dJu_ratio = pCalcData->m_Ju_ratio;

	if(!pDB->IsPlateGir())
		sText.Format("3.8 내부지점부 바닥판 주장율 검토");
	else
		sText.Format("3.4 내부지점부 바닥판 주장율 검토");

	pOut->SetXL(pXL, sText,												m_nSttRow, "A", "A", 2, BLACK, 9, TRUE);
	pOut->SetXL(pXL, "(지점부 부모멘트 구간에 인장을 받는 바닥판 콘크리트의 교축방향 철근)", m_nSttRow, "C", "C", NEXT);
	sText.Format("· 바닥판 콘크리트 단면적의 %.1f%% 이상", pCalcFloor->GetFloorConcAreaCoefficient());
	pOut->SetXL(pXL, sText,				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "( ☞ 도.설.기 3.9.2.3 )",							m_nSttRow, "T", "T", NEXT);
	sText.Format("· 주장률 %.4f mm / mm2 이상", pCalcFloor->GetFloorJujangStd());
	pOut->SetXL(pXL, sText,					m_nSttRow, "C", "C", NEXT);
	pOut->SetXL(pXL, "Ac",												m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",												m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dB,												m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "B", 1);
	pOut->SetXL(pXL, "×",												m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dH,												m_nSttRow, "J", "L", 0, RED,   9, FALSE, TA_CENTER, "H", 1);
	pOut->SetXL(pXL, "=",												m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("H"), pOut->GetCellRef("B"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "N", "P", 0, BLACK,  9, FALSE, TA_CENTER, "AC", 1);
	pOut->SetXL(pXL, "mm²",											m_nSttRow, "Q", "Q", NEXT);

	pOut->SetXL(pXL, "As'",												m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",												m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("AC"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "×",												m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL,  pCalcFloor->GetFloorConcAreaCoefficient()/100,	m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL(pXL, "=",												m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%.3f", pOut->GetCellRef("AC"), pCalcFloor->GetFloorConcAreaCoefficient()/100);
	pOut->SetXL(pXL, sText,												m_nSttRow, "N", "P", 0, BLACK,  9, FALSE, TA_CENTER, "REQ_AS", 1);
	pOut->SetXL(pXL, "mm²",											m_nSttRow, "Q", "Q", 2);

	pOut->SetXL(pXL, "· 사용철근량 :",									m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "상면",											m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,											m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, sCaseH1,											m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "CASEH1", 0);
	pOut->SetXL(pXL, "@",												m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dCtc1,												m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "CTC1", 1);
	pOut->SetXL(pXL, "=",												m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
		pOut->GetCellRef("CASEH1"),pOut->GetCellRef("CTC1"),pOut->GetCellRef("CASEH1"),
		pOut->GetCellRef("CTC1"),pOut->GetCellRef("CASEH1"),pOut->GetCellRef("CTC1"),
		pOut->GetCellRef("CASEH1"),pOut->GetCellRef("CTC1"),pOut->GetCellRef("CASEH1"),
		pOut->GetCellRef("CTC1"),pOut->GetCellRef("CASEH1"),pOut->GetCellRef("CTC1"),pOut->GetCellRef("CTC1"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "AS_UPPER", 1);
	pOut->SetXL(pXL, "mm²",											m_nSttRow, "S", "S", 1);

	pOut->SetXL(pXL, "하면",											m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sRebar,												m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, sCaseH2,											m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "CASEH2", 0);
	pOut->SetXL(pXL, "@",												m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dCtc2,												m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "CTC2", 1);
	pOut->SetXL(pXL, "=",												m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
		pOut->GetCellRef("CASEH2"),pOut->GetCellRef("CTC2"),pOut->GetCellRef("CASEH2"),
		pOut->GetCellRef("CTC2"),pOut->GetCellRef("CASEH2"),pOut->GetCellRef("CTC2"),
		pOut->GetCellRef("CASEH2"),pOut->GetCellRef("CTC2"),pOut->GetCellRef("CASEH2"),
		pOut->GetCellRef("CTC2"),pOut->GetCellRef("CASEH2"),pOut->GetCellRef("CTC2"),pOut->GetCellRef("CTC2"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "AS_LOWER",1);
	pOut->SetXL(pXL, "mm²",											m_nSttRow, "S", "S", 1);

	pOut->SetXL(pXL, "As",												m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",												m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s+%s", pOut->GetCellRef("AS_UPPER"), pOut->GetCellRef("AS_LOWER"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "P", "R", 0, BLACK,  9, FALSE,  TA_CENTER, "USEAS", 1);
	pOut->SetXL(pXL, "mm²",											m_nSttRow, "S", "S", 2);

	pOut->SetXL(pXL, "∴ As =",											m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("USEAS"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "mm²",											m_nSttRow, "J", "J", 0);

	pOut->SetXL(pXL, "As' =",											m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("REQ_AS"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "P", "R", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "mm²",											m_nSttRow, "S", "S", 0);
	sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("USEAS"), pOut->GetCellRef("REQ_AS"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "L", "L", 0, RED,   9, TRUE,  TA_CENTER);
	sText.Format("=IF(%s>%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("USEAS"), pOut->GetCellRef("REQ_AS"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "W", "W", 2, RED,   9, TRUE);

	pOut->SetLineBoxText(pXL, "· 주장률 =",							m_nSttRow, "C", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("π × %.1f × %.1fEA + π × %.1f × %.1fEA", dDia1, dAsEa1, dDia2, dAsEa2);

	pOut->SetUnderLineText(pXL, sText,									m_nSttRow, "F", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",										m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	sText.Format("%.5f", dJu_ratio);
	pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "Q", "S", 2, BLACK,  9, FALSE,  TA_CENTER, "JUJANG", 5, FALSE);

	sText.Format("%.4f", pCalcFloor->GetFloorJujangStd());
	pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "U", "W", 2, BLACK,  9, FALSE, TA_CENTER, "ALLOW_JUJANG", 5, FALSE);
	sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("JUJANG"), pOut->GetCellRef("ALLOW_JUJANG"));
	pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "T", "T", 2, RED,   9, TRUE,  TA_CENTER, "", 0, FALSE);

	pOut->SetXL(pXL, "",												m_nSttRow, "Z", "Z", NEXT);
	sText.Format("%.1f × %.1f", dB, dH);
	pOut->SetXL(pXL, sText,												m_nSttRow, "F", "O", 1, BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=IF(%s>=%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("JUJANG"), pOut->GetCellRef("ALLOW_JUJANG"));
	pOut->SetXL(pXL, sText,												m_nSttRow, "W", "W", 2, RED,   9, TRUE);
}

void CXLDesignFloor::Initialize()
{
	CAPlateOutCalcXL *pOut = m_pAPlateOutCalcXL;
	m_nSttRow			= 1;
	m_nNumber			= 0;
	m_nW3				= 0;

	pOut->ClearCellRef();
	m_ArrayGuardWallTopL.RemoveAll();
	m_ArrayGuardWallH.RemoveAll();
	m_ArrayNumber.RemoveAll();
	m_ArrayNumber.Add("가)");
	m_ArrayNumber.Add("나)");
	m_ArrayNumber.Add("다)");
	m_ArrayNumber.Add("라)");
	m_ArrayNumber.Add("마)");
	m_ArrayNumber.Add("바)");
	m_ArrayNumber.Add("사)");
	m_ArrayNumber.Add("아)");
	m_ArrayNumber.Add("자)");
	m_ArrayNumber.Add("차)");
	m_ArrayNumber.Add("카)");
}

void CXLDesignFloor::DrawCrackWidthCheck(CDomyun *pDom, long nType)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage		= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd		*pOptStd			= pDataManage->GetOptionStd();
	CCalcData			*pCalcData			= pDataManage->GetCalcData();
	CDomyun		Dom(pDom);
	long nIndex = nType;

	double	dPibok			= 0;
	double	dRebarCTC1		= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Rein_Ctc1;
	double	dTc				= 0;
	double	dRebarDia		= 0;
	long	nRebarCombo1	= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Rein_Combo1;
//	if(!pDB->IsPlateGir())
//	{
//		dRebarCTC1		= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_dGenReinTCTC;
//		nRebarCombo1	= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_nGenReinTDiaIdx_1Cy;
//	}
	if(nIndex == 2)
	{
		dTc		= pDB->m_dThickSlabBase - pDB->m_dHunchDisForBaseThick;
		dPibok	= pCalcData->m_dCoverDn;
	}
	else
	{
		dTc		= pDB->m_dThickSlabBase;
		dPibok	= pCalcData->m_dCoverUp;
	}

//	double Scale = 15;
//	Dom.SetScaleDim(Scale);

	Dom.MoveTo( 0, 0 );
	Dom.LineTo( dRebarCTC1 * 3, 0 );
	Dom.LineTo( dRebarCTC1 * 3, dTc );
	Dom.LineTo( 0, dTc );
	Dom.LineTo( 0, 0 );

	switch(nRebarCombo1)
	{
		case 0: {dRebarDia  = 13.0;	break;}
		case 1: {dRebarDia  = 16.0;	break;}
		case 2: {dRebarDia  = 19.0;	break;}
		case 3: {dRebarDia  = 22.0;	break;}
		case 4: {dRebarDia  = 25.0;	break;}
		case 5: {dRebarDia  = 29.0;	break;}
		case 6: {dRebarDia  = 32.0;	break;}
		case 7: {dRebarDia  = 35.0;	break;}
		default: {	break;}
	}

	double dX = dRebarCTC1/2;
	double dY = 0;

	if(nIndex == 2)		dY = dPibok;
	else				dY = dTc - dPibok;

	for(long i=0; i<3; i++)
	{
		Dom.SetLineColor(9);
		Dom.Circle(dX, dY, dRebarDia, TRUE);
		dX += dRebarCTC1;
	}

	Dom.Move(500 - dRebarCTC1*3/2, -dTc - 500);

	*pDom << Dom;

}

void CXLDesignFloor::DimCrackWidthCheck(CDomyun *pDom, long nType)
{

	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage		= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd		*pOptStd			= pDataManage->GetOptionStd();
	CCalcData			*pCalcData			= pDataManage->GetCalcData();
	CDomyun		Dom(pDom);

	long nIndex = nType;

	double	dPibok		= 0;
	double	dRebarCTC1	= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Rein_Ctc1;
//	if(!pDB->IsPlateGir())
//			dRebarCTC1	= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_dGenReinTCTC;
	double	dTc			= 0;
	double	dEs			= pCalcData->DESIGN_MATERIAL.m_dEs;		// 철근 탄성계수
	double	dEc			= pCalcData->DESIGN_MATERIAL.m_dEc;		// 콘크리트 탄성계수
	long	nN			= long(dEs / dEc);
	double	dAs			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Rein_UseAs;
//	if(!pDB->IsPlateGir())
//			dAs			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_dGenReinTAsuse;
	double	dD			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Rein_d;
	double	dB			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Rein_b;
	double	dAsRatio	= dAs/(dB * dD);
	double	dK			= -nN * dAsRatio + sqrt(pow(nN * dAsRatio, 2.0) + 2 * nN * dAsRatio);
	double	dX			= dK * dD;

	if(nIndex == 2)
	{
		dTc		= pDB->m_dThickSlabBase - pDB->m_dHunchDisForBaseThick;
		dPibok	= pCalcData->m_dCoverDn;
	}
	else
	{
		dTc		= pDB->m_dThickSlabBase;
		dPibok	= pCalcData->m_dCoverUp;
	}

	Dom.SetScaleDim(Dom.GetScaleDim()*2);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("LEFT");
	if(nIndex == 2)
	{
		Dom.DimMoveTo(0, dPibok, 0 );
		Dom.DimLineToExtend(dTc-dPibok, 0, "d", Comma(dTc-dPibok));
	}
	else
	{
		Dom.DimMoveTo(0, 0, 0 );
		Dom.DimLineToExtend(dTc-dPibok, 0, "d",  Comma(dTc-dPibok));
	}

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo( dRebarCTC1*3, 0, 2);
	if(nIndex == 2)
	{
		Dom.DimLineTo(dPibok, 2, Comma(dPibok));
		Dom.DimLineTo(dTc - dX - dPibok, 2, Comma(dTc - dX - dPibok));
		Dom.DimLineTo(dX, 2, Comma(dX));
	}
	else
	{
		Dom.DimLineTo(dX, 2, Comma(dX));
		Dom.DimLineTo(dTc - dX - dPibok, 2, Comma(dTc - dX - dPibok));
		Dom.DimLineTo(dPibok, 2, Comma(dPibok/10));
	}

	pOptStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);

	if(nIndex == 2)
	{
		double x1 = dRebarCTC1*3 + 200;
		double x2 = x1 - 0.6*(dTc-dPibok-dX);
		double x3 = x1;
		double x4 = x1 + 0.6*dX;
		double y1 = dPibok;
		double y2 = dPibok;
		double y3 = dTc;
		double y4 = dTc;

		Dom.LineTo(x1, y1, x2, y2);
		Dom.LineTo(x1, y1, x3, y3);
		Dom.LineTo(x2, y2, x4, y4);
		Dom.LineTo(x3, y3, x4, y4);

		pOptStd->SetEnvType(&Dom, HCAD_TXTG);

		Dom.LineTo(x1+10, y1, x1+110, y1);	//T
		Dom.LineTo(x1+110, y1, x1+80, y1+10);
		Dom.LineTo(x1+110, y1, x1+80, y1-10);
		Dom.LineTo(x1+80, y1+10, x1+80, y1-10);
		Dom.Solid(x1+80, y1+10, x1+80, y1-10, x1+110, y1, x1+80, y1+10);
		Dom.TextOut(x1+130, y1, "T");

		Dom.LineTo(x3+10, y3-dX/3, x3+110, y3-dX/3);	//C
		Dom.LineTo(x3+10, y3-dX/3, x3+40, y3-dX/3+10);
		Dom.LineTo(x3+10, y3-dX/3, x3+40, y3-dX/3-10);
		Dom.LineTo(x3+40, y3-dX/3+10, x3+40, y3-dX/3-10);
		Dom.Solid(x3+10, y3-dX/3, x3+40, y3-dX/3-10, x3+40, y3-dX/3+10, x3+10, y3-dX/3);
		Dom.TextOut(x3+130, y3-dX/3, "C");
	}
	else
	{
		double x1 = dRebarCTC1*3+200;
		double x2 = x1+0.6*dX;
		double x3 = x1;
		double x4 = x1-0.6*(dTc-dPibok-dX);
		double y1 = 0;
		double y2 = 0;
		double y3 = dTc-dPibok;
		double y4 = dTc-dPibok;

		Dom.LineTo(x1, y1, x2, y2);
		Dom.LineTo(x1, y1, x3, y3);
		Dom.LineTo(x2, y2, x4, y4);
		Dom.LineTo(x3, y3, x4, y4);

		pOptStd->SetEnvType(&Dom, HCAD_TXTG);

		Dom.LineTo(x3+10, y3, x3+110, y3);	//T
		Dom.LineTo(x3+110, y3, x3+80, y3+10);
		Dom.LineTo(x3+110, y3, x3+80, y3-10);
		Dom.LineTo(x3+80, y3+10, x3+80, y3-10);
		Dom.Solid(x3+110, y3, x3+80, y3-10, x3+80, y3+10, x3+110, y3);
		Dom.TextOut(x3+130, y3, "T");

		Dom.LineTo(x1+10, y1+dX/3, x1+110, y1+dX/3);	//C
		Dom.LineTo(x1+10, y1+dX/3, x1+40, y1+dX/3+10);
		Dom.LineTo(x1+10, y1+dX/3, x1+40, y1+dX/3-10);
		Dom.LineTo(x1+40, y1+dX/3+10, x1+40, y1+dX/3-10);
		Dom.Solid(x1+10, y1+dX/3, x1+40, y1+dX/3-10, x1+40, y1+dX/3+10, x1+10, y1+dX/3);
		Dom.TextOut(x1+130, y1+dX/3, "C");
	}

	Dom.Move(500 - dRebarCTC1*3/2, -dTc - 500);
	*pDom << Dom;
}

void CXLDesignFloor::DrawReBarQuantity(CDomyun *pDom, long nFloorIndex)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage		= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData			= pDataManage->GetCalcData();
	CARoadOptionStd		*pOptStd			= pDataManage->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(20);
	Dom.SetDimVLen(0.5);
	Dom.SetTextHeight(0.5);

	double	dB1		= 300.0;
	double	dH1		= 400.0;
	double	dPibok1	= 30;
	double	dB		= toCm(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_b);
	double	dPibok	= nFloorIndex == 2 ? toCm(pCalcData->m_dCoverDn) : toCm(pCalcData->m_dCoverUp);
	double	dH		= nFloorIndex == 2 ? toCm(pDB->m_dThickSlabBase) - toCm(pDB->m_dThickSlabHunch) : toCm(pDB->m_dThickSlabBase);

	CDPoint vX(1,0), vY(0,1);
	CDPoint xyStt = CDPoint(-dB1/2, 0);
	CDPoint xyEnd = CDPoint( dB1/2, 0);

	Dom.LineTo(xyStt, xyEnd);
	Dom.LineTo(xyStt, xyStt - vY * dH1);	
	Dom.LineTo(xyEnd, xyEnd - vY * dH1);
	Dom.LineTo(xyStt - vY * dH1, xyEnd - vY * dH1);

	if(nFloorIndex == 2)
	{
		Dom.LineTo(xyStt - vY * ( dH1 - dPibok1 * 2), xyEnd - vY * (dH1 -  dPibok1 * 2));
		Dom.Circle(0,  -dH1+dPibok1, 10);
	}
	else
	{
		Dom.LineTo(xyStt - vY * dPibok1 * 2, xyEnd - vY * dPibok1 * 2);
		Dom.Circle(0,  -dPibok1, 10);
	}

	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyEnd.x, xyEnd.y, 0, "");
	if(nFloorIndex == 2)
	{
		Dom.DimLineToExtend(-dH1+dPibok1, 0, COMMA(dH-dPibok), "");
		Dom.DimLineToExtend(-dPibok1, 0, COMMA(dPibok), "");
	}
	else
	{
		Dom.DimLineToExtend(-dPibok1, 0, COMMA(dPibok), "");
		Dom.DimLineToExtend(-(dH1-dPibok1), 0, COMMA(dH-dPibok), "");
	}

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x, xyStt.y - dH1, 0, "");
	Dom.DimLineToExtend(dB1, 0, COMMA(dB), "");

	*pDom << Dom;
}

long CXLDesignFloor::LossOfPreStress()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	
	CDPoint	xy(0,0);
	long nIndex = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressPlate		*pPrestress		= pDataManage->GetSteelPrestress();
	CSteelPrestressData			*pPrestressData	= pPrestress->GetSteelPrestressData(nIndex);

	CString sText;
	CMap <CString, LPCTSTR, double, double> MapCen;
	CMap <CString, LPCTSTR, double, double> MapLeft;
	CMap <CString, LPCTSTR, double, double> MapRight;
	pCalcFloor->CalcPreStress(MapCen, FLOOR_MID);
	pCalcFloor->CalcPreStress(MapLeft, FLOOR_LEFT);
	pCalcFloor->CalcPreStress(MapRight, FLOOR_RIGHT);

	long	nPSQty				= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	long	nRow				= 1;
	double	dX					= 0;
	double	dAngle				= 0;
	double	dThickInner			= MapCen["SlabThick_Inner"];
	double	dCurvatureFriction	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction;	///< 곡률 마찰계수
	double	dUndulateFriction	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction;	///< 파상 마찰계수
	double	dPrestressEs		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	double	dPreDeadLoad		= 0;
	double	dPostDeadLoad		= 0;
	double	dLength				= 0;
	double	dTensionStrength	= MapCen["dTensionStrength"];
	double	dYieldStrength		= MapCen["dYieldStrength"];
	double	dEc					= pCalcData->DESIGN_MATERIAL.m_dEc;

	nRow = 3;
	sText.Format("%.f", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC);
	pOut->SetXL(pXL, sText, nRow, "H", "H", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	///< Insert Domyun
	CDomyun	Dom;
	CPlateGirderApp		*pGir	= pDB->GetGirder(0);
	CPlateBasicIndex	*pBx	= pGir->GetCrossBxAtMaxFlangeWidth();
	CARoadOptionStd		*pOpt	= pDataManage->GetOptionStd();
	CAPlateDrawDanmyun	DrawDanmyun(pDataManage);
	CAPlateDrawCross	DrawCross(pDataManage);

	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);

	pOpt->SetEnvType(&Dom, HCAD_STLC);
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx);			// 지점부 슬래브 단면
	DrawDanmyun.DrawTensionBar(&Dom, pBx, TRUE);	// 지점부 슬래브 단면	
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	DrawDanmyun.DrawGuardWall(&Dom, pBx);			// 방호벽
	///< 치수선그리기
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 75*(dDiagonalL/10000);
	Dom.SetScaleDim(dScale);

	DrawDanmyun.DimTensionBarOnLineInfo(&Dom, pBx, TRUE);	//  치수
	DrawDanmyun.DimTensionBar(&Dom, pBx, TRUE);
	DrawDanmyun.DimTypicalSectionBottom(&Dom, pBx);	
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);

	nRow+=2;
	pOut->InsertDomyunByFixWidth(pXL, &Dom, nRow, 2, 21, FALSE);
	/////////////
	nRow = 22;
	sText.Format("%g", dThickInner);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	nRow++;
	sText.Format("%g", pDB->m_dThickSlabBase);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	nRow++;
	sText.Format("%g", pDB->m_dThickSlabBase);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 1);
///< 슬래브 중앙부
	///< 1) Pre-Stress 도입
	/// 가) 정착단에서 PS 강재의 초기 긴장력
	dLength			= MapCen["Length"];
	m_dPreMd		= MapCen["MomentPreDeadLoad"];
	m_dPostMd		= MapCen["MomentPostDeadLoad"];
	dPreDeadLoad	= MapCen["MomentPreDeadLoad"];
	dPostDeadLoad	= MapCen["MomentPostDeadLoad"];
	dX				= MapCen["X"];
	dAngle			= MapCen["Alpha"];

	nRow = 30;
	sText.Format("%s", pPrestressData->m_szStandard);
	pOut->SetXL(pXL, sText, nRow, "G", "G", 0, RED, 9);
	sText.Format("%s", pPrestressData->m_szSymbol);
	pOut->SetXL(pXL, sText, nRow, "I", "I", 0, RED, 9);
	sText.Format("%.d", nPSQty);
	pOut->SetXL(pXL, sText, nRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("%.1f", pPrestressData->m_dDiameter);
	pOut->SetXL(pXL, sText, nRow, "N", "N", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	sText.Format("%.2f", pPrestressData->m_dArea*nPSQty);
	pOut->SetXL(pXL, sText, nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	nRow++;
	sText.Format("%g", dTensionStrength);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%g", dYieldStrength);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	///< 나) PC 강재와 Sheath사이의 마찰에 의한 손실
	nRow+=5;
	sText.Format("%.8f", toM(dX));
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dCurvatureFriction);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dAngle);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dUndulateFriction);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	///< 다) 정착장치 활동에 의한 손실(PS 강재와 쉬스 사이에 마찰이 있는 경우)
	nRow+=9;
	sText.Format("%g", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	nRow+=6;
	sText.Format("%g", dPrestressEs);
	pOut->SetXL(pXL, sText, nRow, "O", "O", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=1;
	sText.Format("F%d", nRow);
	double	dlset	= atof(pXL->GetXL(sText));
	nRow+=1;
	if(dlset < dX)
	{
		pXL->DeleteRowLine(nRow, nRow+7);
		pXL->DeleteRowLineEnd();
		nRow+=1;
		pOut->SetXL(pXL, 0.0, nRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		nRow+=7;
	}
	else
	{
		nRow += 16;	
	}
	///< 라) 콘크리트 탄성수축에 의한 손실량
	sText.Format("%.1f", dLength);
	pOut->SetXL(pXL, sText, nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	nRow++;
	sText.Format("%g", tokNM(dPreDeadLoad));
	pOut->SetXL(pXL, sText, nRow, "G", "G", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow+=2;
	sText.Format("%g", dEc);
	pOut->SetXL(pXL, sText, nRow, "N", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);

	///< 2) 설계하중 작용시 손실량
	///< 나) 콘크리트의 크리프에 의한 손실량
	nRow+=13;
	sText.Format("%g", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity);
	pOut->SetXL(pXL, sText, nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=7;
	sText.Format("%g", tokNM(dPostDeadLoad));
	pOut->SetXL(pXL, sText, nRow, "I", "I", 0, RED, 9, FALSE, TA_CENTER, "", 3);

///<캔틸레버부 좌측
	///< 1) Pre-Stress 도입
	/// 가) 정착단에서 PS 강재의 초기 긴장력
	dPreDeadLoad	= MapLeft["MomentPreDeadLoad"];
	dPostDeadLoad	= MapLeft["MomentPostDeadLoad"];
	dX				= MapLeft["X"];
	dAngle			= MapLeft["Alpha"];
	dLength			= MapLeft["Length"];

	nRow += 17;
	sText.Format("%s", pPrestressData->m_szStandard);
	pOut->SetXL(pXL, sText, nRow, "G", "G", 0, RED, 9);
	sText.Format("%s", pPrestressData->m_szSymbol);
	pOut->SetXL(pXL, sText, nRow, "I", "I", 0, RED, 9);
	sText.Format("%d", nPSQty);
	pOut->SetXL(pXL, sText, nRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("%.1f", pPrestressData->m_dDiameter);
	pOut->SetXL(pXL, sText, nRow, "N", "N", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	sText.Format("%.2f", pPrestressData->m_dArea*nPSQty);
	pOut->SetXL(pXL, sText, nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	nRow++;
	sText.Format("%g", dTensionStrength);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%g", dYieldStrength);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	///< 나) PC 강재와 Sheath사이의 마찰에 의한 손실
	nRow+=5;
	sText.Format("%.8f", toM(dX));
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dCurvatureFriction);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dAngle);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dUndulateFriction);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	///< 다) 정착장치 활동에 의한 손실(PS 강재와 쉬스 사이에 마찰이 있는 경우)
	nRow+=9;
	sText.Format("%g", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	nRow+=7;
	sText.Format("F%d", nRow);
	dlset	= atof(pXL->GetXL(sText));
	nRow+=1;
	if(dlset < dX)
	{
		pXL->DeleteRowLine(nRow, nRow+7);
		pXL->DeleteRowLineEnd();
		nRow+=1;
		pOut->SetXL(pXL, 0.0, nRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		nRow+=7;
	}
	else
	{
		nRow+=16;
	}

	///< 라) 콘크리트 탄성수축에 의한 손실량
//	nRow+=18;
	sText.Format("%.1f", dLength);
	pOut->SetXL(pXL, sText, nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	nRow++;
	sText.Format("%g", tokNM(dPreDeadLoad));
	pOut->SetXL(pXL, sText, nRow, "G", "G", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow+=2;
	///< 2) 설계하중 작용시 손실량
	///< 나) 콘크리트의 크리프에 의한 손실량
	nRow+=12;
	sText.Format("%g", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity);
	pOut->SetXL(pXL, sText, nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=7;
	sText.Format("%g", tokNM(dPostDeadLoad));
	pOut->SetXL(pXL, sText, nRow, "I", "I", 0, RED, 9, FALSE, TA_CENTER, "", 3);

///<캔틸레버부 우측
	///< 1) Pre-Stress 도입
	/// 가) 정착단에서 PS 강재의 초기 긴장력
	dPreDeadLoad	= MapRight["MomentPreDeadLoad"];
	dPostDeadLoad	= MapRight["MomentPostDeadLoad"];
	dX				= MapRight["X"];
	dAngle			= MapRight["Alpha"];
	dLength			= MapRight["Length"];

	nRow += 18;
	sText.Format("%s", pPrestressData->m_szStandard);
	pOut->SetXL(pXL, sText, nRow, "G", "G", 0, RED, 9);
	sText.Format("%s", pPrestressData->m_szSymbol);
	pOut->SetXL(pXL, sText, nRow, "I", "I", 0, RED, 9);
	sText.Format("%d", nPSQty);
	pOut->SetXL(pXL, sText, nRow, "L", "L", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("%.1f", pPrestressData->m_dDiameter);
	pOut->SetXL(pXL, sText, nRow, "N", "N", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	sText.Format("%.2f", pPrestressData->m_dArea*nPSQty);
	pOut->SetXL(pXL, sText, nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	nRow++;
	sText.Format("%g", dTensionStrength);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%g", dYieldStrength);
	pOut->SetXL(pXL, sText, nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	///< 나) PC 강재와 Sheath사이의 마찰에 의한 손실
	nRow+=5;
	sText.Format("%.8f", toM(dX));
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dCurvatureFriction);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dAngle);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow++;
	sText.Format("%.8f", dUndulateFriction);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	///< 다) 정착장치 활동에 의한 손실(PS 강재와 쉬스 사이에 마찰이 있는 경우)
	nRow+=9;
	sText.Format("%g", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	pOut->SetXL(pXL, sText, nRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	nRow+=7;
	sText.Format("F%d", nRow);
	dlset	= atof(pXL->GetXL(sText));
	nRow+=1;
	if(dlset < dX)
	{
		pXL->DeleteRowLine(nRow, nRow+7);
		pXL->DeleteRowLineEnd();
		nRow+=1;
		pOut->SetXL(pXL, 0.0, nRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		nRow+=7;
	}
	else
	{
		nRow+=16;
	}
	
	///< 라) 콘크리트 탄성수축에 의한 손실량
//	nRow+=18;
	sText.Format("%.1f", dLength);
	pOut->SetXL(pXL, sText, nRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	nRow++;
	sText.Format("%g", tokNM(dPreDeadLoad));
	pOut->SetXL(pXL, sText, nRow, "G", "G", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow+=2;

	///< 2) 설계하중 작용시 손실량
	///< 나) 콘크리트의 크리프에 의한 손실량
	nRow+=12;
	sText.Format("%g", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity);
	pOut->SetXL(pXL, sText, nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=7;
	sText.Format("%g", tokNM(dPostDeadLoad));
	pOut->SetXL(pXL, sText, nRow, "I", "I", 0, RED, 9, FALSE, TA_CENTER, "", 3);

	m_nSttRow	= nRow + 12;
	return m_nSttRow;
}

void CXLDesignFloor::SlabStressCheck(long &nRow)
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	CMap <CString, LPCTSTR, double, double> MapCen;
	CMap <CString, LPCTSTR, double, double> MapLeft;
	CMap <CString, LPCTSTR, double, double> MapRight;
	pCalcFloor->CalcPreStress(MapCen, FLOOR_MID);
	pCalcFloor->CalcPreStress(MapLeft, FLOOR_LEFT);
	pCalcFloor->CalcPreStress(MapRight, FLOOR_RIGHT);

	double	dTotInner		= MapCen["DesignLoad"];
	//double	dCPreDeadLoad	= MapCen["MomentPreDeadLoad"];
	double	dTotLeft		= MapLeft["DesignLoad"];
	//double	dLPreDeadLoad	= MapLeft["MomentPreDeadLoad"];
	double	dTotRight		= MapRight["DesignLoad"];
	//double	dRPreDeadLoad	= MapRight["MomentPreDeadLoad"];
	double	dfck			= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double  dfci            = pCalcData->DESIGN_MATERIAL.m_dSigmaCi;
	//double	dfca_i			= 0.55*0.80*dfck;
	//double	dfca_t			= 0.4*dfck;

	CString sText;
	
// 중앙부
	nRow += 3;
	pOut->SetXL(pXL, dfck, nRow, "Y", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow += 1;
	pOut->SetXL(pXL, dfci, nRow, "Y", "Z", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow += 15;

	sText.Format("%g", tokNM(dTotInner));
	pOut->SetXL(pXL, sText, nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow+=27;
	m_nDanRow[4] = nRow+2;
	sText.Format("%g", tokNM(dTotLeft));
	pOut->SetXL(pXL, sText, nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	nRow+=27;
	m_nDanRow[5] = nRow+2;
	sText.Format("%g", tokNM(dTotRight));
	pOut->SetXL(pXL, sText, nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 3);	
}

void CXLDesignFloor::SectionCheckAboutUltimateLoad()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	pOut->OnSheetRefMode(TRUE);
	CMap <CString, LPCTSTR, double, double> MapCen;
	CMap <CString, LPCTSTR, double, double> MapLeft;
	CMap <CString, LPCTSTR, double, double> MapRight;
	CMap <CString, LPCTSTR, double, double> Map;

	pCalcFloor->CalcPreStress(MapCen, FLOOR_MID);
	pCalcFloor->CalcPreStress(MapLeft, FLOOR_LEFT);
	pCalcFloor->CalcPreStress(MapRight, FLOOR_RIGHT);

	CString	szText, szCell;
	double	dFck	= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dFy		= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dPibokDn= pCalcData->m_dCoverDn;
	double	dPibokUp= pCalcData->m_dCoverUp;
	double	dTc		= pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;
	CString	sRebar	= pCalcData->DESIGN_MATERIAL.m_sSigmaY;//.m_dSigmaY >= 400 ? _T("H") : _T("D");

	///< 슬래브 중앙부
	double	dThickInner	= MapCen["SlabThick_Inner"];
	double	dThickStt	= MapCen["SlabThick_Stt"];
	double	dThickEnd	= MapCen["SlabThick_End"];
	double	dLength		= MapCen["Length_SlabToPS"];
	//double	dUseAs2		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_dGenReinCAsuse;			///<사용철근량(일반부 인장주철근)
	double	dCtc1		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_dGenReinTCTC;		///< 일반부 인장주철근간격
	double	dCtc2		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_dGenReinCCTC;		///< 일반부 압축주철근간격
	CString	szCaseH1_1Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_strGenReinTDia_1Cy;		///< 일반부 인장주철근 직경
	CString	szCaseH1_2Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_strGenReinTDia_2Cy;		///< 일반부 인장주철근 직경
	CString	szCaseH2_1Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_strGenReinCDia_1Cy;		///< 일반부 압축주철근 직경
	CString	szCaseH2_2Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_strGenReinCDia_2Cy;		///< 일반부 압축주철근 직경

	long nRow = m_bComposeOnlyOne ? m_nUltSttRow[2]+6 : m_nUltSttRow[2]+7;
	pOut->SetXL(pXL, dFck,	nRow, "P", "V", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	pOut->SetXL(pXL, dFy,	nRow, "N", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);

	nRow+=6;
	pOut->SetXL(pXL, dLength, nRow, "S", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);

	nRow++;
	//pOut->SetXL(pXL, dUseAs,		nRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL(pXL, dTc-dPibokDn,	nRow, "S", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH1_1Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc1);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH1_2Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc1);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=2;
	//pOut->SetXL(pXL, dUseAs2,		nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL(pXL, dTc-dPibokUp,	nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH2_1Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc2);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH2_2Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc2);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=15;
	pOut->SetXL(pXL, dThickInner,	nRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	nRow+=6;
	szText.Format("=F%d", m_nUltSttRow[2]-1);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);	///< 극한 모멘트
	
	///< 슬래브 좌측
	dLength		= MapLeft["Length_SlabToPS"];
	dCtc1		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_dGenReinTCTC;			///< 일반부 인장주철근간격
	dCtc2		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_dGenReinCCTC;			///< 일반부 압축주철근간격
	szCaseH1_1Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_strGenReinTDia_1Cy;		///< 일반부 인장주철근 직경
	szCaseH1_2Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_strGenReinTDia_2Cy;		///< 일반부 인장주철근 직경
	szCaseH2_1Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_strGenReinCDia_1Cy;		///< 일반부 압축주철근 직경
	szCaseH2_2Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_strGenReinCDia_2Cy;		///< 일반부 압축주철근 직경
	dTc			= pDB->m_dThickSlabBase;
	nRow		= m_nUltSttRow[0]+5;

	pOut->SetXL(pXL, dFck,	nRow, "P", "V", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	pOut->SetXL(pXL, dFy,	nRow, "N", "P", 0, RED, 9, FALSE, TA_CENTER, "", 0);

	nRow+=6;
	pOut->SetXL(pXL, dLength, nRow, "S", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);

	nRow++;
	//pOut->SetXL(pXL, dUseAs,		nRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER, "", 3);	///< 일반부 사용  철근량
	pOut->SetXL(pXL, dTc-dPibokDn,	nRow, "S", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);	///< 인장부 높이
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH1_1Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc1);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH1_2Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc1);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=2;
	//pOut->SetXL(pXL, dUseAs2,		nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 3);	///< 일반부 사용  철근량
	pOut->SetXL(pXL, dTc-dPibokUp,	nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);	///< 압축 높이
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH2_1Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc2);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH2_2Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc2);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=15;
	pOut->SetXL(pXL, dThickStt,		nRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 1);	///< 중앙부 바닥판 투께

	nRow+=6;
	szText.Format("=F%d", m_nUltSttRow[0]-2);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 19, BLACK, 9, FALSE, TA_CENTER, "", 3);	///< 극한 모멘트

	// 단부
	pCalcFloor->SetCantileverUltimateLoad(FLOOR_LEFT, Map);
	CString	strDia_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Rein_CaseH2;
	CString	strDia_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Rein_CaseH2_2Cy;
	double	dMainCTC	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Rein_Ctc2;
	double	dFsa		= pCalcData->DESIGN_MATERIAL.m_dSigmaSA;
	double	dUpper		= Map["D_Upper"];
	double	dLower		= Map["D_Lower"];

	szText.Format("='3.1 좌측 캔틸레버부'!F%d", m_nDanRow[0]);
	pOut->SetXL(pXL, szText,		nRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("='3.1 좌측 캔틸레버부'!W%d", m_nDanRow[2]);
	pOut->SetXL(pXL, szText,		nRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("='3.4 프리스트레스의 손실'!K%d/1000", m_nDanRow[4]);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("='3.4 프리스트레스의 손실'!Q%d/10^6", m_nDanRow[4]+1);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER);

	szText.Format("%f", dUpper);
	pOut->SetXL(pXL, szText,		nRow, "H", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%f", dLower);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 2, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, sRebar,		nRow, "I", "I", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strDia_1Cy,	nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTC,		nRow, "L", "M", 1, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, sRebar,		nRow, "I", "I", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strDia_2Cy,	nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTC,		nRow, "L", "M", 5, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dFsa,			nRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	///< 슬래브 우측
	dLength		= MapRight["Length_SlabToPS"];
	dCtc1		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_dGenReinTCTC;				///< 일반부 인장주철근간격
	dCtc2		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_dGenReinCCTC;				///< 일반부 압축주철근간격
	szCaseH1_1Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_strGenReinTDia_1Cy;		///< 일반부 인장주철근 직경
	szCaseH1_2Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_strGenReinTDia_2Cy;		///< 일반부 인장주철근 직경
	szCaseH2_1Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_strGenReinCDia_1Cy;		///< 일반부 압축주철근 직경
	szCaseH2_2Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_strGenReinCDia_2Cy;		///< 일반부 압축주철근 직경

	nRow = m_nUltSttRow[1]+5;
	
	pOut->SetXL(pXL, dFck,	nRow, "P", "V", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dFy,	nRow, "N", "P", 6, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dLength, nRow, "S", "S", 1, RED, 9, FALSE, TA_CENTER, "", 0);

	//pOut->SetXL(pXL, dUseAs,		nRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL(pXL, dTc-dPibokDn,	nRow, "S", "S", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH1_1Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc1);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH1_2Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc1);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=2;
	//pOut->SetXL(pXL, dUseAs2,		nRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	pOut->SetXL(pXL, dTc-dPibokUp,	nRow, "T", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH2_1Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc2);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow++;
	szText.Format("%s", sRebar);
	pOut->SetXL(pXL, szText,		nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%s", szCaseH2_2Cy);
	pOut->SetXL(pXL, szText,		nRow, "K", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%g", dCtc2);
	pOut->SetXL(pXL, szText,		nRow, "M", "N", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	nRow+=15;
	pOut->SetXL(pXL, dThickEnd,	nRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	nRow+=6;
	szText.Format("=F%d", m_nUltSttRow[1]-2);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 19, BLACK, 9, FALSE, TA_CENTER, "", 3);	///< 극한 모멘트

	pCalcFloor->SetCantileverUltimateLoad(FLOOR_RIGHT, Map);
	strDia_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Rein_CaseH2;
	strDia_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Rein_CaseH2_2Cy;
	dMainCTC	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Rein_Ctc2;
	dFsa		= pCalcData->DESIGN_MATERIAL.m_dSigmaSA;
	dUpper		= Map["D_Upper"];
	dLower		= Map["D_Lower"];

	szText.Format("='3.2 우측 캔틸레버부'!F%d", m_nDanRow[1]);
	pOut->SetXL(pXL, szText,		nRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("='3.2 우측 캔틸레버부'!W%d", m_nDanRow[3]);
	pOut->SetXL(pXL, szText,		nRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("='3.4 프리스트레스의 손실'!K%d/1000", m_nDanRow[5]);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER);
	szText.Format("='3.4 프리스트레스의 손실'!Q%d/10^6", m_nDanRow[5]+1);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER);
	
	szText.Format("%f", dUpper);
	pOut->SetXL(pXL, szText,		nRow, "H", "J", 0, RED, 9, FALSE, TA_CENTER);
	szText.Format("%f", dLower);
	pOut->SetXL(pXL, szText,		nRow, "Q", "S", 2, RED, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, sRebar,		nRow, "I", "I", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strDia_1Cy,	nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTC,		nRow, "L", "M", 1, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, sRebar,		nRow, "I", "I", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, strDia_2Cy,	nRow, "J", "J", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dMainCTC,		nRow, "L", "M", 5, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, dFsa,			nRow, "K", "M", 5, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->OnSheetRefMode(FALSE);

	long nDeleteRow = m_nUltSttRow[0]+72;
	if(nDeleteRow < 177)
		pXL->DeleteRowLine(nDeleteRow, 177);
	nDeleteRow = m_nUltSttRow[2]+57;
	if(nDeleteRow < 73)
		pXL->DeleteRowLine(nDeleteRow, 74);	
	pXL->DeleteRowLineEnd();
}

void CXLDesignFloor::CheckFloorReinforce(CString szSheetName)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CCalcFloor			*pCalcFloor		= m_pAPlateOutCalcStd->GetCalcFloor();
	CCalcGeneral		CalcGeneral(pDataManage);
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	CString sText;
	m_nSttRow = 1;

	///< 3.8 바닥판 배력 철근 산정
	pOut->SetXL(pXL, szSheetName,								m_nSttRow, "A", "A", 2, BLACK, 9, TRUE);
	long nFloorIndex = 0;
	for(nFloorIndex=0 ; nFloorIndex<3 ; nFloorIndex++) 
	{
		long	nFloor		= nFloorIndex==0 ? FLOOR_MID : nFloorIndex-1;
		double	dH			= pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Min_T;
		double	dB			= (pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Rein_b);
		double	dDeadL		= toM(pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Dead_L);	//toM(CalcGeneral.CalcCentPart_L());
		CString	sCaseH2		= pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Rein_CaseH2;	///< 단부 주철근 철근지름
		CString	sCaseH3		= pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Rein_CaseH3;	///< 일반부 배력철근 지름
		double	dCtcB		= pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Rein_CtcB;		///< 일반부 배력철근 간격
		CString sCaseH4		= pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Rein_CaseH4;	///< 단부 배력철근 철근지름
		double	dCtcB1		= pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Rein_CtcB1;		///< 단부 배력철근 간격
		double	dUseAs		= pDB->m_bTendonInputComplete ? pCalcData->CALC_CANTILEVER_DATA[nFloor].m_dGenReinTAsuse : pCalcData->CALC_CANTILEVER_DATA[nFloor].m_Rein_Asreq;		///< 일반부 주철근 면적
		
		CString sCaseName = "상연 :";

		if(nFloorIndex == 0)
		{
			sText.Format("가. 슬래브 중앙부");
			sCaseName = "하연 :";
		}
		else if(nFloorIndex == 1)
			sText.Format("나. 좌측 캔틸레버부");
		else if(nFloorIndex == 2)
			sText.Format("다. 우측 캔틸레버부");

		pOut->SetXL(pXL, sText,									m_nSttRow, "B", "B", 1);
		pOut->SetXL(pXL, "1) 도로교 설계기준 Page 113",			m_nSttRow, "C", "C", 1);
		pOut->SetXL(pXL, "☞ 주철근이 차량진행방향에 직각일 경우 120/√L 과 67% 중 작은 값 이상 사용", m_nSttRow, "E", "E", 1);
		pOut->SetXL(pXL, "2) 배력철근의 계산",					m_nSttRow, "C", "C", 1);

		pOut->SetXL(pXL, "As",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dUseAs,								m_nSttRow, "H", "I", 0, RED,   9, FALSE, TA_CENTER, "AS", 0);
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "J", "J", 1);
		pOut->SetXL(pXL, "배력철근의 주철근에 대한 백분율 : ",	m_nSttRow, "E", "E", NEXT);
		pOut->SetXL(pXL, "120 / √L",							m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"120 / √ \"&ROUND(%.3f,3)", dDeadL);
		pOut->SetXL(pXL, sText,									m_nSttRow, "J", "J", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=120/SQRT(%g)", dDeadL);
		pOut->SetXL(pXL, sText,									m_nSttRow, "O", "P", 0, BLACK,  9, FALSE, TA_CENTER, "RATE", 0);
		pOut->SetXL(pXL, "%",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, 67.0,									m_nSttRow, "S", "T", 0, BLACK,  9, FALSE, TA_CENTER, "ARATE", 0);
		pOut->SetXL(pXL, "%",									m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("RATE"), pOut->GetCellRef("ARATE"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "R", "R", 1, RED,   9, TRUE, TA_CENTER);
		sText.Format("=\"As × \"&ROUND(MIN(%s,%s)/100,3)&\" = \"&ROUND(%s,3)&\" × \"&ROUND(MIN(%s,%s)/100,3)",
					pOut->GetCellRef("RATE"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("AS"), pOut->GetCellRef("RATE"), pOut->GetCellRef("ARATE"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s * MIN(%s,%s)/100", pOut->GetCellRef("AS"), pOut->GetCellRef("ARATE"), pOut->GetCellRef("RATE"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK,  9, FALSE, TA_CENTER, "AS_BAERUCK");
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "R", "R", NEXT);
		pOut->SetXL(pXL, "수축·온도철근량 :",					m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "(☞ 콘.설.기 P103)",		m_nSttRow, "T", "T", NEXT);
		sText.Format("=\"0.0020 × b × h  =  0.0020 × \"&ROUND(%.3f,3)&\" ×  \"&ROUND(%.3f,3)", dB, dH);
		pOut->SetXL(pXL, sText,									m_nSttRow, "F", "F", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.002 * %g * %g", dB, dH);
		pOut->SetXL(pXL, sText,									m_nSttRow, "R", "T", 0, BLACK,  9, FALSE, TA_CENTER, "AS_TEMPARATURE");
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "U", "U");
		pOut->SetXL(pXL, "3) 사용철근량",						m_nSttRow, "C", "C", 1);
		pOut->SetXL(pXL, "Req.As",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=MAX(%s,%s)", pOut->GetCellRef("AS_BAERUCK"), pOut->GetCellRef("AS_TEMPARATURE"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "AS_REQ_B");
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "K", "K", 1);
		pOut->SetXL(pXL, "USE",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetUnderLineText(pXL, "",							m_nSttRow, "G", "Y", 0);
		pOut->SetXL(pXL, sCaseName,								m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "H",									m_nSttRow, "K", "K", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, sCaseH3,								m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER, "CASEH3", 0);
		pOut->SetXL(pXL, "@",									m_nSttRow, "M", "M", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dCtcB,									m_nSttRow, "N", "O", 0, RED,   9, FALSE, TA_CENTER, "CTCB", 0);
		pOut->SetXL(pXL, "mm",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, "As",									m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(%s=13,126.7*1000/%s,IF(%s=16,198.6*1000/%s,IF(%s=19,286.5*1000/%s,IF(%s=22,387.1*1000/%s,IF(%s=25,506.7*1000/%s,IF(%s=29,642.4*1000/%s,794.2*1000/%s))))))",
					   pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"), pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"),
					   pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"), pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"),
					   pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"), pOut->GetCellRef("CASEH3"), pOut->GetCellRef("CTCB"),
					   pOut->GetCellRef("CTCB"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "U", "W", 0, BLACK, 9, FALSE,  TA_CENTER, "USEAS_BAERUCK");
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "X", "X", 1);
		
/*
		pOut->SetXL(pXL, "하연 :",								m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "H",									m_nSttRow, "K", "K", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, sCaseH3,								m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER, "CASEH3", 0);
		pOut->SetXL(pXL, "@",									m_nSttRow, "M", "M", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dCtcB,									m_nSttRow, "N", "O", 0, RED,   9, FALSE, TA_CENTER, "CTCB", 0);
		pOut->SetXL(pXL, "mm",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, "As",									m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s",	pOut->GetCellRef("USEAS_BAERUCK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "X", "X", 1);
*/
		pOut->SetXL(pXL, "As",									m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s",	pOut->GetCellRef("USEAS_BAERUCK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "X", "X", 0);
		sText.Format("=IF(%s>=%s, \"... O.K\", \"... N.G\")", pOut->GetCellRef("USEAS_BAERUCK"), pOut->GetCellRef("AS_REQ_B"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "Z", "Z", 1, BLACK, 9, FALSE, TA_CENTER);

		if(nFloorIndex != 0)
		{
			pXL->InsertCopyRowLine(m_nSttRow-4, m_nSttRow-1, m_nSttRow);
			pOut->SetXL(pXL, "4) 단부 사용철근량", m_nSttRow, "C", "C");
			sText.Format("=%s", pOut->GetCellRef("AS_REQ_B"));
			pOut->SetXL(pXL, sText, m_nSttRow, "H", "J", 0, 1, 9, FALSE, TA_CENTER, "AS_REQ_B1");
			pOut->SetXL(pXL, "×",	m_nSttRow, "K", "K", 0, 1, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "2",	m_nSttRow, "L", "L", 0, 1, 9, FALSE, TA_CENTER, "AS_REQ_B2", 0);
			pOut->SetXL(pXL, "=",	m_nSttRow, "M", "M", 0, 1, 9, FALSE, TA_CENTER);
			sText.Format("=%s*%s", pOut->GetCellRef("AS_REQ_B1"), pOut->GetCellRef("AS_REQ_B2"));
			pOut->SetXL(pXL, sText,	m_nSttRow, "N", "P", 0, 1, 9, FALSE, TA_CENTER, "AS_REQ_B");
			pOut->SetXL(pXL, "mm²",m_nSttRow, "Q", "Q");
			
			pOut->SetXL(pXL, "",	m_nSttRow, "H", "H", 0);
			pOut->SetXL(pXL, sCaseH4,	m_nSttRow, "L", "L", 0, 0, 9, FALSE, TA_CENTER, "CASE4", 0);
			pOut->SetXL(pXL, dCtcB1,	m_nSttRow, "N", "O", 1, 0, 9, FALSE, TA_CENTER, "CTCB", 0);
			sText.Format("=IF(U%d>=%s, \"... O.K\", \"... N.G\")", m_nSttRow-1, pOut->GetCellRef("AS_REQ_B"));
			pOut->SetXL(pXL, sText,		m_nSttRow, "Z", "Z", 2, BLACK, 9, FALSE, TA_CENTER);			
		}
		else
			m_nSttRow++;		
	}

	///< 3.9 균열검토
	if(!pDB->IsPlateGir())	return;
	double	dFy			= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dFck		= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dEs			= pCalcData->DESIGN_MATERIAL.m_dEs;
	double	dEc			= pCalcData->DESIGN_MATERIAL.m_dEc;
	
	if(pDB->m_nQtyJigan > 1)
		sText.Format("3.6 균열 검토");
	else
		sText.Format("3.5 균열 검토");

	pOut->SetXL(pXL, sText,						m_nSttRow, "A", "A", 2, BLACK, 9, TRUE);
	
	for(nFloorIndex=0 ; nFloorIndex<3 ; nFloorIndex++)
	{
		double	dH			= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Min_T;
		double	dB			= pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Rein_b;
		double	dPibok		= pCalcData->m_dCoverUp;
		if(nFloorIndex == 2) dPibok	= pCalcData->m_dCoverDn;
		double	dAvail		= dH-dPibok;										// Min_t - cover

		CMap <CString, LPCTSTR, double, double> Map;
		pCalcFloor->SetCantileverCrack(Map, nFloorIndex);

		double	dM_Stop		= tokNM(Map["CRACK_MSTOP"]);
		double	dM_Move		= tokNM(Map["CRACK_MMOVE"]);
		double	dM_Cent		= Map["CRACK_MCENT"];
		double	dUseAs		= Map["CRACK_As"] + Map["CRACK_Ap"];
		double	dnAps		= Map["CRACK_EA"];
		double	dMinC		= Map["CRACK_C"];
		
		if(nFloorIndex == 0)
			pOut->SetXL(pXL, "가. 좌측 캔틸레버부",				m_nSttRow, "B", "B", 1);
		else if(nFloorIndex == 1)
			pOut->SetXL(pXL, "나. 우측 캔틸레버부",				m_nSttRow, "B", "B", 1);
		else if(nFloorIndex == 2)
			pOut->SetXL(pXL, "다. 지간 중앙부",					m_nSttRow, "B", "B", 1);

		pOut->SetXL(pXL, "1) 균열 검토",						m_nSttRow, "C", "C", 1);
		pOut->SetXL(pXL, "사용하중하에서 최대 하중을 유발하는 하중조합을 사용한다.",	m_nSttRow, "C", "C", 0);
		pOut->SetXL(pXL, "[콘크리트구조설계기준] 76Page",		m_nSttRow, "T", "T", 1);
		pOut->SetXL(pXL, "M₂",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%f", dM_Stop);
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "CRACK_MU1", 5);
		pOut->SetXL(pXL, "+",									m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%f", dM_Move);
		pOut->SetXL(pXL, sText,									m_nSttRow, "K", "M", 0, RED,   9, FALSE, TA_CENTER, "CRACK_MU2", 5);
		
		if(dM_Cent != 0)
		{
			pOut->SetXL(pXL, "+",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%f", dM_Cent);
			pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU3", 5);
		}
		pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		if(dM_Cent != 0)
			sText.Format("=%s+%s+%s",	pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"), pOut->GetCellRef("CRACK_MU3"));
		else
			sText.Format("=%s+%s",		pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"));
		
		pOut->SetXL(pXL, sText,									m_nSttRow, "S", "U", 0, BLACK,  9, FALSE, TA_CENTER, "CRACK_MAX", 5);
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "V", "V", 1);
		pOut->SetXL(pXL, "∴ Mmax  =",							m_nSttRow, "E", "E", 0);
		if(dM_Cent != 0)
			sText.Format("=%s+%s+%s",	pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"), pOut->GetCellRef("CRACK_MU3"));
		else
			sText.Format("=%s+%s",		pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "H", "J", 0, RED,   9, FALSE, TA_CENTER, "CRACK_MAX");
		pOut->SetXL(pXL, "kN·m",								m_nSttRow, "K", "K", 1);
		/*		sText.Format("=%s", pOut->GetCellRef("DEADLOAD"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU1");
		pOut->SetXL(pXL, "+",									m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		if(pOut->GetCellRef("LIVELOAD") != _T("") && pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
			sText.Format("=MAX(%s, %s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"), pOut->GetCellRef("TRAILERLOAD"));
		else if(pOut->GetCellRef("LIVELOAD") != _T("") && pOut->GetCellRef("TANKLOAD") != _T("") && pOut->GetCellRef("TRAILERLOAD") == _T(""))
			sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TANKLOAD"));
		else if(pOut->GetCellRef("LIVELOAD") != _T("") && pOut->GetCellRef("TANKLOAD") == _T("") && pOut->GetCellRef("TRAILERLOAD") != _T(""))
			sText.Format("=MAX(%s, %s)", pOut->GetCellRef("LIVELOAD"), pOut->GetCellRef("TRAILERLOAD"));
		else if(bWalkLoad)
			sText.Format("=%s", pOut->GetCellRef("WALKLOAD"));
		else
			sText.Format("=%s", pOut->GetCellRef("LIVELOAD"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU2");
		if(dR > 0)
		{
			pOut->SetXL(pXL, "+",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);

			if(pOut->GetCellRef("CENTLOAD") != _T("") && pOut->GetCellRef("CENTLOAD_TANK") != _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") != _T(""))
				sText.Format("=MAX(%s, %s, %s)", pOut->GetCellRef("CENTLOAD"), pOut->GetCellRef("CENTLOAD_TANK"), pOut->GetCellRef("CENTLOAD_TRAILER"));
			else if(pOut->GetCellRef("CENTLOAD") != _T("") && pOut->GetCellRef("CENTLOAD_TANK") != _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") == _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("CENTLOAD"), pOut->GetCellRef("CENTLOAD_TANK"));
			else if(pOut->GetCellRef("CENTLOAD") != _T("") && pOut->GetCellRef("CENTLOAD_TANK") == _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") != _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("CENTLOAD"), pOut->GetCellRef("CENTLOAD_TRAILER"));
			else if(pOut->GetCellRef("CENTLOAD") == _T("") && pOut->GetCellRef("CENTLOAD_TANK") != _T("") && pOut->GetCellRef("CENTLOAD_TRAILER") != _T(""))
				sText.Format("=MAX(%s, %s)", pOut->GetCellRef("CENTLOAD_TANK"), pOut->GetCellRef("CENTLOAD_TRAILER"));
			else
				sText.Format("=%s", pOut->GetCellRef("CENTLOAD"));
			pOut->SetXL(pXL, sText,									m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "CRACK_MU3");
		}
		pOut->SetXL(pXL, "=",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		if(pOut->GetCellRef("CRACK_MU2") != _T("") && pOut->GetCellRef("CRACK_MU3") != _T(""))
			sText.Format("=ROUND(%s+%s+%s,5)", pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"), pOut->GetCellRef("CRACK_MU3"));
		else if(pOut->GetCellRef("CRACK_MU2") == _T("") && pOut->GetCellRef("CRACK_MU3") != _T(""))
			sText.Format("=ROUND(%s+%s,5)", pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU3"));
		else if(pOut->GetCellRef("CRACK_MU2") == _T("") && pOut->GetCellRef("CRACK_MU3") == _T(""))
			sText.Format("=ROUND(%s,5)", pOut->GetCellRef("CRACK_MU1"));
		else if(pOut->GetCellRef("CRACK_MU2") != _T("") && pOut->GetCellRef("CRACK_MU3") == _T(""))
			sText.Format("=ROUND(%s+%s,5)", pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 0, BLACK,  9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "tonf·m",								m_nSttRow, "W", "W", NEXT);

		pOut->SetXL(pXL, "∴ Mmax  =",							m_nSttRow, "E", "E", 0);
		if(pOut->GetCellRef("CRACK_MU2") != _T("") && pOut->GetCellRef("CRACK_MU3") != _T(""))
			sText.Format("=ROUND(%s+%s+%s,5)", pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"), pOut->GetCellRef("CRACK_MU3"));
		else if(pOut->GetCellRef("CRACK_MU2") == _T("") && pOut->GetCellRef("CRACK_MU3") != _T(""))
			sText.Format("=ROUND(%s+%s,5)", pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU3"));
		else if(pOut->GetCellRef("CRACK_MU2") == _T("") && pOut->GetCellRef("CRACK_MU3") == _T(""))
			sText.Format("=ROUND(%s,3)", pOut->GetCellRef("CRACK_MU1"));
		else if(pOut->GetCellRef("CRACK_MU2") != _T("") && pOut->GetCellRef("CRACK_MU3") == _T(""))
			sText.Format("=ROUND(%s+%s,5)", pOut->GetCellRef("CRACK_MU1"), pOut->GetCellRef("CRACK_MU2"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "H", "J", 0, RED,   9, FALSE, TA_CENTER, "CRACK_MAX");
		pOut->SetXL(pXL, "tonf·m",								m_nSttRow, "K", "K", 2);
*/
		pOut->SetXL(pXL, "fy",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dFy,									m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "FY", 0);
		pOut->SetXL(pXL, "MPa",								m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "fck",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dFck,									m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "FCK", 0);
		pOut->SetXL(pXL, "MPa",								m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "As",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dUseAs,								m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "USEAS");
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "n(As)",								m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dnAps,									m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "N_APS");
		pOut->SetXL(pXL, "EA",									m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "b",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dB,									m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "B");
		pOut->SetXL(pXL, "mm",									m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "d",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dAvail,								m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "DEPTH");
		pOut->SetXL(pXL, "mm",									m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "d'",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dPibok,								m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "COVER");
		pOut->SetXL(pXL, "mm",									m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "n",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Es / Ec",								m_nSttRow, "G", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=ROUNDDOWN(%f/%f,0)", dEs, dEc);
		pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 1, BLACK,  9, FALSE, TA_CENTER, "N", 0);

		pOut->SetXL(pXL, "p",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "As / ( b × d )",						m_nSttRow, "G", "G", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"\"&ROUND(%s,3)&\" / ( \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" )\"", pOut->GetCellRef("USEAS"), pOut->GetCellRef("B"), pOut->GetCellRef("DEPTH"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/(%s*%s)", pOut->GetCellRef("USEAS"), pOut->GetCellRef("B"), pOut->GetCellRef("DEPTH"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 1, BLACK,  9, FALSE, TA_CENTER, "P", 7);

		pOut->SetXL(pXL, "k",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "-np + √{(np)²+ 2np}",				m_nSttRow, "G", "G", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"-\"&ROUND(%s,6)&\"×\"&ROUND(%s,6)&\"+√{(\"&ROUND(%s,6)&\"×\"&ROUND(%s,6)&\")²+2×\"&ROUND(%s,6)&\"×\"&ROUND(%s,6)&\"}\"",
						pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=-%s*%s+SQRT((%s*%s)^2+2*%s*%s)", pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"), pOut->GetCellRef("N"), pOut->GetCellRef("P"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 1, BLACK,  9, FALSE, TA_CENTER, "K", 7);

		pOut->SetXL(pXL, "j",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "1 - ( k / 3 )",						m_nSttRow, "G", "G", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"1 - ( \"&ROUND(%s,3)&\" / 3 )\"", pOut->GetCellRef("K"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=1 - %s/3", pOut->GetCellRef("K"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 1, BLACK,  9, FALSE, TA_CENTER, "J", 5);

		pOut->SetLineBoxText(pXL, "fsmax =",					m_nSttRow, "D", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetUnderLineText(pXL, "Mmax",						m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "=",							m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		sText.Format("=\"\"&ROUND(%s,3)&\" × 10^6\"", pOut->GetCellRef("CRACK_MAX"));
		pOut->SetUnderLineText(pXL, sText,						m_nSttRow, "L", "R", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "p×b×j×d²",						m_nSttRow, "G", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"\"&ROUND(%s,6)&\"×\"&ROUND(%s,3)&\"×\"&ROUND(%s,3)&\"×\"&ROUND(%s,3)&\"²\"", pOut->GetCellRef("P"), pOut->GetCellRef("B"), pOut->GetCellRef("J"), pOut->GetCellRef("DEPTH"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "L", "R", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*10^6/(%s * %s * %s * %s^2)",			pOut->GetCellRef("CRACK_MAX"), pOut->GetCellRef("P"), pOut->GetCellRef("B"), pOut->GetCellRef("J"), pOut->GetCellRef("DEPTH"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "L", "N", 0, BLACK,  9, FALSE, TA_CENTER, "FSMAX", 3);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "O", "O", 0);
		pOut->SetXL(pXL, "0.6×fy",								m_nSttRow, "S", "S", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.6*%s", pOut->GetCellRef("FY"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "V", "X", 0, BLACK,  9, FALSE, TA_CENTER, "ALLOW_FY", 3);
		pOut->SetXL(pXL, "MPa",							m_nSttRow, "Y", "Y", 0);
		sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("FSMAX"), pOut->GetCellRef("ALLOW_FY"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "R", "R", 1, RED,   9, FALSE, TA_CENTER);
		sText.Format("=IF(%s > %s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("FSMAX"), pOut->GetCellRef("ALLOW_FY"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE);

		pOut->SetXL(pXL, "A",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=\"b × h / \"&ROUND(%s,0)&\" = \"&ROUND(%s,3)&\" × \"&ROUND(%s*2,3)&\" / \"&ROUND(%s,0)",
						pOut->GetCellRef("N_APS"), pOut->GetCellRef("B"), pOut->GetCellRef("COVER"), pOut->GetCellRef("N_APS"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "G", 0);

		pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*%s*2/%s", pOut->GetCellRef("B"),		pOut->GetCellRef("COVER"), pOut->GetCellRef("N_APS"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 0, BLACK,  9, FALSE, TA_CENTER, "A", 3);
		pOut->SetXL(pXL, "mm²",								m_nSttRow, "W", "W", 2);

		CDomyun	Dom;
		Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom.ClearEtt(TRUE);
		DrawReBarQuantity(&Dom, nFloorIndex);
		pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 3, 6, FALSE, FALSE, FALSE);

		pOut->SetXL(pXL, "A  : 주인장철근 주위의 콘크리트 단면적을", m_nSttRow, "O", "O");
		pOut->SetXL(pXL, "     철근의 갯수로 나눈유효인장 단면적", m_nSttRow, "O", "O");
		pOut->SetXL(pXL, "dc : 인장연단에서 가장 가까이 위치한", m_nSttRow, "O", "O");
		pOut->SetXL(pXL, "     철근 중심까지지의 피복두께",		m_nSttRow, "O", "O");
		pOut->SetXL(pXL, "z  : 휨철근의 분배를 제한하는 수량",	m_nSttRow, "O", "O", 3);

		pOut->SetXL(pXL, "z",									m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"fs × ( dc × A )^⅓  =  \"&ROUND(%s,3)&\" × ( \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" )^⅓\"", pOut->GetCellRef("FSMAX"), pOut->GetCellRef("COVER"), pOut->GetCellRef("A"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "G", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s * ( %s * %s )^(1/3)", pOut->GetCellRef("FSMAX"), pOut->GetCellRef("COVER"), pOut->GetCellRef("A"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "I", 0, BLACK,  9, FALSE, TA_CENTER, "Z", 3);
		pOut->SetXL(pXL, "N/mm",								m_nSttRow, "J", "J", 0);
		pOut->SetXL(pXL, 23170.0,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "ALLOW_Z");
		pOut->SetXL(pXL, "N/mm",								m_nSttRow, "R", "R", 0);
		sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef("Z"), pOut->GetCellRef("ALLOW_Z"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "M", "M", 0, RED,  9, TRUE,  TA_CENTER);
		sText.Format("=IF(%s>%s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("Z"), pOut->GetCellRef("ALLOW_Z"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,  9, TRUE);

		pOut->SetXL(pXL, "2) 균열폭 검토",						m_nSttRow, "C", "C", 1);
		pOut->SetXL(pXL, "㉮",									m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "ωa",									m_nSttRow, "F", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "0.005 × tc ( 철근 콘크리트 일반환경조건 )", m_nSttRow, "I", "I", 1);
		pOut->SetXL(pXL, "여기서, tc : 철근 콘크리트 최소피복 두께 ( mm )",	m_nSttRow, "F", "F");
		pOut->SetXL(pXL, "㉯",									m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "ωa",									m_nSttRow, "F", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"0.005 × \"&ROUND(%.3f,3)",			dMinC);
		pOut->SetXL(pXL, sText,									m_nSttRow, "I", "I", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=0.005 * %g",					dMinC);
		pOut->SetXL(pXL, sText,									m_nSttRow, "T", "V", 0, BLACK,  9, FALSE, TA_CENTER, "WA", 3);
		pOut->SetXL(pXL, "mm",									m_nSttRow, "W", "W", 1);

		pOut->SetXL(pXL, "㉰",									m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "βc",									m_nSttRow, "F", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "단면의 중립축에서 인장연단까지의 거리 / 단면의 중립축에서 철근도심까지의 거리", m_nSttRow, "I", "I", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"( \"&ROUND(%.3f,3)&\" - \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" ) / ( \"&ROUND(%.3f,3)&\" - \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" - \"&ROUND(%s,3)&\" )\"",
					dH, pOut->GetCellRef("K"), pOut->GetCellRef("DEPTH"), dH, pOut->GetCellRef("K"), pOut->GetCellRef("DEPTH"), pOut->GetCellRef("COVER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "I", "I", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=(%g-%s*%s)/(%g-%s*%s-%s)", dH, pOut->GetCellRef("K"), pOut->GetCellRef("DEPTH"),
						dH, pOut->GetCellRef("K"), pOut->GetCellRef("DEPTH"), pOut->GetCellRef("COVER"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "I", "K", 1, BLACK, 9, FALSE, TA_CENTER, "BC", 3);

		pOut->SetXL(pXL, "㉱",									m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, "ω",									m_nSttRow, "F", "G", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "1.08  ×  βc  ×  fs  ×  ³√( dc × A ) × 0.00001", m_nSttRow, "I", "I", NEXT);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=\"1.08  ×  \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" × ³√( \"&ROUND(%s,3)&\" × \"&ROUND(%s,3)&\" )  × 0.00001\"",
						pOut->GetCellRef("BC"), pOut->GetCellRef("FSMAX"), pOut->GetCellRef("COVER"), pOut->GetCellRef("A"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "I", "I", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=1.08 * %s * %s * (%s*%s)^(1/3) * 0.00001", pOut->GetCellRef("BC"), pOut->GetCellRef("FSMAX"), pOut->GetCellRef("COVER"), pOut->GetCellRef("A"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "ALLOW_W", 3);
		pOut->SetXL(pXL, "mm",									m_nSttRow, "L", "L", 0);
		pOut->SetXL(pXL, "ωa",									m_nSttRow, "P", "P", 0);
		pOut->SetXL(pXL, "=",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("WA"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
		pOut->SetXL(pXL, "mm",									m_nSttRow, "U", "U", 0);
		sText.Format("=IF(%s > %s, \">\", \"<\")", pOut->GetCellRef("ALLOW_W"), pOut->GetCellRef("WA"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "N", "N", 1, RED,   9, TRUE,  TA_CENTER);
		sText.Format("=IF(%s > %s, \"... N.G\", \"... O.K\")", pOut->GetCellRef("ALLOW_W"), pOut->GetCellRef("WA"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "S", "S", 1, RED,   9, TRUE);
//		pOut->OnBackUpCellRef(TRUE);

		Dom.ClearEtt(TRUE);
		Dom.SetScaleDim(10);
		DrawCrackWidthCheck(&Dom, nFloorIndex);
		DimCrackWidthCheck(&Dom, nFloorIndex);
		pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow, 2, 20);

		pOut->SetXL(pXL, "",									m_nSttRow, "A", "A", 2);
	}	
}

///< 정착부 보강 설계
void CXLDesignFloor::DesignFixingReinforce()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;

	double dFy		= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double dFpy		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;//강연선 fpy,항복강도
	long dNt		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	double dAt		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea;//강연선 단면적
	double dW		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth;
	double dB		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight;
	double dH		= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;		///< 중앙부 슬래브 두께
	CString sCaseH1 = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH1;
	CString sCaseH2 = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH2;
	long dQty1		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1;
	long dQty2		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2;
	double dGap		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dReinCTC;

	m_nSttRow = 4;	
	pOut->SetXL(pXL, dH,			m_nSttRow, "Y", "Z", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dNt,			m_nSttRow, "Y", "Z", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dW,			m_nSttRow, "Y", "Z", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dB,			m_nSttRow, "Y", "Z", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dFy,			m_nSttRow, "Y", "Z", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dFpy,			m_nSttRow, "Y", "Z", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dAt,			m_nSttRow, "Y", "Z", 1, RED, 9, FALSE,  TA_CENTER, "", 0);	
	SetLines(17);
	pOut->SetXL(pXL, dGap,			m_nSttRow, "H", "H", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	SetLines(8);
	pOut->SetXL(pXL, sCaseH1,		m_nSttRow, "I", "I", 0, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dQty1,			m_nSttRow, "N", "N", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
	SetLines(2);
	pOut->SetXL(pXL, sCaseH2,		m_nSttRow, "I", "I", 0, RED, 9, FALSE,  TA_CENTER, "", 0);
	pOut->SetXL(pXL, dQty2,			m_nSttRow, "N", "N", 1, RED, 9, FALSE,  TA_CENTER, "", 0);
}

void CXLDesignFloor::ExpansionJointUltimateLoad()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CCalcData			*pCalcData	= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CGlobarOption		*pGlobalOpt	= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CCalcFloor			CalcFloor(m_pAPlateOutCalcStd->GetDataManage());
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CXLControl			*pXL		= m_pXL;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBx		= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateCrossBeam		*pCr		= pBx->GetCrossBeam();
	m_nSttRow = 4;

	CMap <CString, LPCTSTR, double, double> MapCen;
	CalcFloor.DesignExpansionJointUltimateLoad(MapCen);
	CString	strText		= _T("");
	CString	strCell		= _T("");
	double	dFck		= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dFy			= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dLengthLive	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_L;
	double	dHeightExp	= pGlobalOpt->GetSttExpansionJointHeight();
	double	dUnitConc	= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;
	double	dB			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_b;
	double	dPis		= pCalcData->m_Pis;

	strText.Format("%g", tokNPM3(dUnitConc));
	pOut->SetXL(pXL, strText,	m_nSttRow, "T", "V", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	strCell.Format("Y%d", m_nSttRow-1);
	strText.Format("%g", dHeightExp);
	pXL->SetXL(strCell, strText);
	m_nSttRow+=2;
	strCell.Format("Y%d", m_nSttRow-1);
	strText.Format("%g", MapCen["ThickSlab"]-dHeightExp);
	pXL->SetXL(strCell, strText);
	m_nSttRow++;
	strText.Format("%g", toM(dLengthLive));
	pOut->SetXL(pXL, strText,	m_nSttRow, "N", "P", 5, RED, 9, FALSE, TA_CENTER, "", 3);
	strText.Format("%g", MapCen["LengthArm"]);
	pOut->SetXL(pXL, strText,	m_nSttRow,"AD","AF", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", pCr->m_uM_CR/2);
	pOut->SetXL(pXL, strText,	m_nSttRow,"AI","AJ", 9, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", dFck);
	pOut->SetXL(pXL, strText,	m_nSttRow, "Q", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", dFy);
	pOut->SetXL(pXL, strText,	m_nSttRow, "Z","AB", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", dB);
	pOut->SetXL(pXL, strText,	m_nSttRow,"AI","AK", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", MapCen["D"]);
	pOut->SetXL(pXL, strText,	m_nSttRow, "F", "I", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	strText.Format("%g", MapCen["Cover"]);
	pOut->SetXL(pXL, strText,	m_nSttRow, "Q", "S", 0, RED, 9, FALSE, TA_CENTER, "", 0);
//	strText.Format("%g", dPib);
//	pOut->SetXL(pXL, strText,	m_nSttRow, "Z","AB", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	strText.Format("%g", dPis);
	pOut->SetXL(pXL, strText,	m_nSttRow,"AI","AK", 0, RED, 9, FALSE, TA_CENTER, "", 2);
}

double CXLDesignFloor::GetAs(long AsNum, CString &Case_H)
{
	//Case_H = "H" + _dRebarDia[AsNum];
	//return _dRebarArea[AsNum];

	switch(AsNum)
	{
		case 0: {Case_H = "H13";		return 1.267;}//"H13" cm2
		case 1: {Case_H = "H16";		return 1.986;}//"H16" cm2
		case 2: {Case_H = "H19";		return 2.865;}//"H19" cm2
		case 3: {Case_H = "H22";		return 3.871;}//"H22" cm2
		case 4: {Case_H = "H25";		return 5.067;}//"H25" cm2
		case 5: {Case_H = "H29";		return 6.424;}//"H29" cm2
		case 6: {Case_H = "H32";		return 7.942;}//"H32" cm2
		case 7: {Case_H = "H35";		return 9.566;}//"H35" cm2
		default: {Case_H = "사용안함";	return 0.0;} //cm2
	}
	
}


void CXLDesignFloor::SetLines(int nLine)
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
// ※
///////////////////////////////////////////////////////////////////////////////////// 

double CXLDesignFloor::DiaReturn(long AsNum)
{

	for(int n =0; n < sizeof(_dRebarDia)/sizeof(double); n++)
	{
		if (_dRebarDia[n] == AsNum)
			return _dRebarDiadim[n];			
	}

	return 0;
}

double CXLDesignFloor::AreaReturn(long AsNum)
{

	for(int n =0; n < sizeof(_dRebarDia)/sizeof(double); n++)
	{
		if (_dRebarDia[n] == AsNum)
			return _dRebarArea[n];			
	}

	return 0;
}

void CXLDesignFloor::SeekFlange_Web(double& dFlange, double& dWeb)
{
	dFlange = dWeb = 0;

	__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;
	CPlateBridgeApp		*pDB = m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CPlateBxFinder		Finder(pDB->GetGirder(0));
	CPlateBasicIndex	*pIndex = Finder.GetBxFirst(bxFlag);

	while(Finder.GetBxFixNext())
	{
		double dFlangeLen = pIndex->GetFactChain(G_F_U)->m_dWidth;

		if (dFlange<dFlangeLen)
		{
			dFlange = dFlangeLen;
			double dWebThick = pIndex->GetFactChain(G_W)->m_dFactChainThick;
			dWeb = (dWeb<dWebThick)? dWebThick : dWeb;
		}
		pIndex = Finder.GetBxNext();
	}
}

void CXLDesignFloor::GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr)
{
	//-------------------------------------------------------------------------------------------
	// Seek Flange X-Position
	// 0: Top Flange Left, 1: Top Flange Center, 2: Top Flange Right
	
	CDoubleArray FlangePos;
	BOOL	bFlangeLevel = FALSE;
	double  dFlangeLevel = 0;
	double	dSlabLevel	 = 0;
	
	for (long nJ=0; nJ<pFEM->GetJointSize(); nJ++)
	{
		CJoint* pJoint		= pFEM->GetJoint((unsigned short)nJ);
		double  dJoint_z	= pJoint->m_vPoint.z;
		double  dJoint_x	= pJoint->m_vPoint.x;
		
		if (!bFlangeLevel && dJoint_z !=dSlabLevel)
		{
			bFlangeLevel = TRUE;
			dFlangeLevel = dJoint_z;
		}
		
		if (bFlangeLevel && dFlangeLevel==dJoint_z)
			FlangePos.Add(dJoint_x);
	}
	FlangePos.Sort();
	
	//-------------------------------------------------------------------------------------------
	// Seek Element Index
	if (nSide==0)	// Left Cantilever
	{
		// Target Position
		double dTarget = FlangePos.GetAt(0);
		
		// Seek Element
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			
			if (pJ_S->m_vPoint.x>dTarget) break;
			nArr.Add(nE);
		}
	}
	else if (nSide==2)	// Right Cantilever
	{
		// Target Position
		double dTarget = FlangePos.GetAt(FlangePos.GetUpperBound());
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			CJoint*   pJ_E  = pFEM->GetJoint(pElem->m_nJEnd);
			
			if (pJ_S->m_vPoint.z<dSlabLevel) break;
			if (pJ_E->m_vPoint.x<dTarget) continue;
			nArr.Add(nE);
		}
	}
	else	// Span Middle Member
	{
		// Target Position
		CDoubleArray dTarget;
		for (long nP=2; nP<FlangePos.GetSize()-1; nP+=3)
		{
			double dPos = (FlangePos.GetAt(nP) + FlangePos.GetAt(nP+1)) / 2;
			dTarget.Add(dPos);
		}
		
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			CJoint*   pJ_E  = pFEM->GetJoint(pElem->m_nJEnd);
			
			if (pJ_S->m_vPoint.z<dSlabLevel) break;
			
			for (long n=0; n<dTarget.GetSize(); n++)
			{
				double dPos = dTarget.GetAt(n);
				if (fabs(pJ_S->m_vPoint.x-dPos)<EPSILON || fabs(pJ_E->m_vPoint.x-dPos)<EPSILON)
					nArr.Add(nE);
			}
		}		
	}
}

double CXLDesignFloor::GetDeflection(long nPos, CString szLoad)
{
	CAPlateCalcStd		*pCalc		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM		= pCalc->GetCantileverFemData(0);
	double dDisp = 0;

	// 0-left, 1-middle, 2-right
	long nLoadCase = pFEM->GetLoadCaseIndex(szLoad);
	if (nLoadCase<0) return dDisp;

	// seek member index
	CLongArray IdxElem;
	GetElementIndex_Cantilever(pFEM, nPos, IdxElem);

	// seek deflection
	CResultForce* pResult1 = NULL;
	CResultForce* pResult2 = NULL;

	for (long n=0; n<IdxElem.GetSize(); n++)
	{		
		long nElem = IdxElem.GetAt(n);
		CElement* pElem = pFEM->GetElement((unsigned short)nElem);
		CJoint*	  pJ1	= pFEM->GetJoint(pElem->m_nJStt);
		CJoint*	  pJ2	= pFEM->GetJoint(pElem->m_nJEnd);

		pResult1 = pJ1->GetResultForce(nLoadCase);
		pResult2 = pJ2->GetResultForce(nLoadCase);
		if (!pResult1) continue;
		if (!pResult2) continue;

		double dDis1 = pResult1->m_Disp.z;
		double dDis2 = pResult2->m_Disp.z;
		
		if (fabs(dDisp)<fabs(dDis1)) dDisp = dDis1;
		if (fabs(dDisp)<fabs(dDis2)) dDisp = dDis2;
	}

	return dDisp;
}

//////////////////////////////////////////////////////////////////////////
// 강선이 있는 경우
///< 3. 바닥판 설계
void CXLDesignFloor::TendonDesignFloor()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	
	long	nRow	= 1;
	CString szCell	= _T("");
	CString	szText	= _T("");

	// Set Girder Data
	CDoubleArray dSpaceArr;
	long   nGirSu	= pDB->GetGirdersu();
	double dLeftX   = pDB->GetSlabLeft()->m_dGirderCenter;
	double dRightX  = pDB->GetSlabRight()->m_dGirderCenter;
	double dLen = pDB->GetGirder(0)->m_dGirderCenter - dLeftX;
	dSpaceArr.Add(dLen);
	long nG = 0;
	for (nG=0; nG<nGirSu-1; nG++)
	{
		double dGirderCenterPrev = pDB->GetGirder(nG)->m_dGirderCenter;
		double dGirderCenterNext = pDB->GetGirder(nG+1)->m_dGirderCenter;
		dLen = dGirderCenterNext - dGirderCenterPrev;
		dSpaceArr.Add(dLen);
	}
	dLen = dRightX - pDB->GetGirder(pDB->GetGirdersu()-1)->m_dGirderCenter;
	dSpaceArr.Add(dLen);

	CCentSeparation* pGuardL = pDB->GetGuardFencLeft();
	CCentSeparation* pGuardR = pDB->GetGuardFencRight();


	for (long k=0; k<28; k++)
	{
		pXL->SetCellHeight(k, k, 0);
	}

	// 3.1 바닥판 설계
	// 1) 좌측 캔틸레버부
	double dGurdWidth	= (pGuardL)? pGuardL->GetWidth()+pGuardL->m_D1 : 0;
	double dSpace		= dSpaceArr.GetAt(0);
	double dCar			= 300;
	double dFlange		= 0;
 	double dWeb			= 0;
	SeekFlange_Web(dFlange, dWeb);
	double dLengthLeft	= toM((dSpace-dGurdWidth) - dCar  - dWeb/2 - (dFlange/4 - dWeb/4));

	// ① 충 격 계 수
	nRow = 39;
	szCell.Format("L%d", nRow);
	szText.Format("%.3f", 15 / (40+dLengthLeft));
	pXL->SetXL(szCell, szText);

	// ⑤ 처 짐 검 토
	nRow = 54;
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", dLengthLeft);
	pXL->SetXL(szCell, szText);

	double dLeftDis = fabs(GetDeflection(0, "DB MIN"));
	
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", frM(dLeftDis));	// --> 해석 결과 수정 요망
	pXL->SetXL(szCell, szText);

	// 2) 중 앙 부
	// ① 충 격 계 수
	dSpace = 0;
	for (nG=1; nG<dSpaceArr.GetSize()-1; nG++)
	{
		double dDis = dSpaceArr.GetAt(nG);
		dSpace = (dSpace<dDis)? dDis : dSpace;
	}
	if (nGirSu!=2)
		dSpace -= dWeb - (dFlange-dWeb)/2;
	
	double dLengthCen = toM(dSpace);
	if (nGirSu==2)
	{
		nRow = 59;
		szCell.Format("L%d", nRow);
		szText.Format("%.3f", 15 / (40+dLengthCen));
		pXL->SetXL(szCell, szText);

		nRow = 60;
		pXL->SetCellHeight(nRow-1, nRow-1, 0);

		nRow = 64;
		pXL->SetCellHeight(nRow-1, nRow-1, 0);

		nRow = 73;
		pXL->SetCellHeight(nRow-1, nRow-1, 0);
	}
	else
	{
		nRow = 60;
		szCell.Format("L%d", nRow);
		szText.Format("%.3f", 15 / (40+dLengthCen));
		pXL->SetXL(szCell, szText);

		nRow = 59;
		pXL->SetCellHeight(nRow-1, nRow-1, 0);

		nRow = 63;
		pXL->SetCellHeight(nRow-1, nRow-1, 0);

		nRow = 72;
		pXL->SetCellHeight(nRow-1, nRow-1, 0);
	}

	// ⑤ 처 짐 검 토
	nRow = 77;
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", dLengthCen);
	pXL->SetXL(szCell, szText);

	double dMidDis = fabs(GetDeflection(1, "DB MIN"));
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", frM(dMidDis));	// --> 해석 결과 수정 요망	
	pXL->SetXL(szCell, szText);

	// ⑨ 처 짐 검 토
	nRow = 98;
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", dLengthCen);
	pXL->SetXL(szCell, szText);

	double dTankDis = fabs(GetDeflection(1, "TK MIN"));
	double dTrailerDis = fabs(GetDeflection(1, "TR MIN"));
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", frM(dTankDis));	// --> 해석 결과 수정 요망	
	pXL->SetXL(szCell, szText);

	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", frM(dTrailerDis));	// --> 해석 결과 수정 요망	
	pXL->SetXL(szCell, szText);


	// 3) 우측 캔틸레버부
	// ① 충 격 계 수
	dGurdWidth	= (pGuardR)? pGuardR->GetWidth()+pGuardR->m_D1 : 0;
	dSpace = dSpaceArr.GetAt(dSpaceArr.GetSize()-1);
	double dLengthRight	= toM((dSpace-dGurdWidth) - dCar  - dWeb/2 + (dFlange/4 - dWeb/4));

	nRow = 104;
	szCell.Format("L%d", nRow);
	szText.Format("%.3f", 15 + (40/dLengthRight));
	pXL->SetXL(szCell, szText);
	
	// ⑤ 처 짐 검 토
	nRow = 119;
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", dLengthRight);
	pXL->SetXL(szCell, szText);

	double dRightDis = fabs(GetDeflection(2, "DB MIN"));
	szCell.Format("H%d", nRow++);
	szText.Format("%.3f", frM(dRightDis));
	pXL->SetXL(szCell, szText);	// --> 해석 결과 수정 요망

	// 탱크, 트레일러
	if(!pCalcData->m_bTank && !pCalcData->m_bTrailer)
		pXL->DeleteRowLine(80, 101);

	// 3.2 단 면 제 원
	// 1) 좌측 캔틸레버부
	// ① 총 단 면
	double dThickTension = pDB->m_dHeightLeftTension;
	double dThickSlab = pDB->m_dThickSlabBase;
	nRow = 126;
	szCell.Format("L%d", nRow++);
	szText.Format("%.3f", dThickTension);
	pXL->SetXL(szCell, szText);

	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dThickSlab);
	pXL->SetXL(szCell, szText);

	// ② 순 단 면
	// --> 정보 부족
	double dSection_H = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH;
	double dSection_W = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW;
	nRow = 141;
	szCell.Format("H%d", nRow);
	szText.Format("%.0f", dSection_H);
	pXL->SetXL(szCell, szText);

	szCell.Format("K%d", nRow);
	szText.Format("%.0f", dSection_W);
	pXL->SetXL(szCell, szText);

	// ③ P.S 강연선 환산단면
	nRow = 155;
	szCell.Format("H%d", nRow);
	szText = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName;
	long nPos = szText.Find("m");
	szText = szText.Left(nPos);
	pXL->SetXL(szCell, szText);

	szCell.Format("N%d", nRow);
	szText.Format("%.2f", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea);
	pXL->SetXL(szCell, szText);

	// 2) 중 앙 부
	// ① 총 단 면
	dThickTension = 0;
	dThickSlab =  pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
	for (long n=1; n<pDB->m_nSlabTensionSize-1; n++)
		dThickTension = (dThickTension < pDB->m_SlabTension[n].m_dH)? pDB->m_SlabTension[n].m_dH : dThickTension;

	nRow = 173;
	szCell.Format("L%d", nRow++);
	szText.Format("%.3f",dThickTension);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dThickSlab);
	pXL->SetXL(szCell, szText);

	// ③ P.S 강연선 환산단면
	nRow = 202;
	szCell.Format("H%d", nRow);
	szText = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName;
	nPos = szText.Find("m");
	szText = szText.Left(nPos);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("N%d", nRow);
	szText.Format("%.2f", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea);
	pXL->SetXL(szCell, szText);

	// 3) 우측 캔틸레버부
	// ① 총 단 면
	dThickTension = pDB->m_dHeightRightTension;
	dThickSlab = pDB->m_dThickSlabBase;

	nRow = 220;
	szCell.Format("L%d", nRow++);
	szText.Format("%.3f", dThickTension);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dThickSlab);
	pXL->SetXL(szCell, szText);

	// ③ P.S 강연선 환산단면
	nRow = 249;
	szCell.Format("H%d", nRow);
	szText = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName;
	nPos = szText.Find("m");
	szText = szText.Left(nPos);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("N%d", nRow);
	szText.Format("%.2f", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea);
	pXL->SetXL(szCell, szText);

	pXL->DeleteRowLineEnd();
}

CDPointArray CXLDesignFloor::GetWallGuardPoint(CDPoint* pPoint)
{
	CPlateBridgeApp *pBridge = m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDPointArray xyArr;
	double dLimitBottom = pPoint[8].y;
	double dLen = fabs(pBridge->GetGirder(0)->m_dGirderCenter - pBridge->GetWidthSlabLeft());
	
	for (long n=0; n<9; n++)
	{
		if (pPoint[n].y < dLimitBottom) continue;
		
		pPoint[n].x += dLen;
		pPoint[n].x /= 1000;
		pPoint[n].y -= dLimitBottom;
		pPoint[n].y /= 1000;
		
		xyArr.Add(pPoint[n]);
	}
	xyArr.Add(xyArr.GetAt(0));
	
	return xyArr;
}

void CXLDesignFloor::DrawWallGuard(CDomyun *pDom, double dTextHeight)
{
	CPlateBridgeApp *pBridge		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage		*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd	*pOptStd		= pDataManage->GetOptionStd();
	CDomyun Dom(pDom);
	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetScaleDim(0.1);
	CPlateBasicIndex *pBx = pBx = pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	CAPlateDrawDanmyun DrawDanmyun(pDataManage);
	
	for (long n=0; n<pBridge->GetQtyHDanNode(); n++)
	{
		if(pBridge->IsTypeGuardFenceDom(n))  //방호벽이면
		{
			CDPoint A[9];
			pBridge->GetXyGuardFenceDom(pBx, n, A, TRUE);
			CDPointArray xyWall = GetWallGuardPoint(A);
			for (long i=0; i<xyWall.GetSize()-1; i++)
				Dom.LineTo(xyWall[i+0], xyWall[i+1]);
			
		}
	}
	*pDom << Dom;
}

long CXLDesignFloor::GetLoadApplyMemberCount(CFEMManage* pFEM)
{	
	long nCnt = 0;
	
	for (long nE=0; nE<pFEM->GetElementSize(); nE++)
	{
		CElement* pElem = pFEM->GetElement((unsigned short)nE);
		CVector vStt    = pFEM->GetJoint(pElem->m_nJStt)->m_vPoint;
		CVector vEnd    = pFEM->GetJoint(pElem->m_nJEnd)->m_vPoint;
		
		if (vStt.z==0 && vEnd.z==0) nCnt++; 
	}
	
	return nCnt;
}

void CXLDesignFloor::DrawDimJoint(CDomyun *pDom, CFEMManage *pFEM, double dTextHeight, BOOL bElemNum)
{
	CDomyun Dom(pDom);
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd		*pOptStd= pDataManage->GetOptionStd();
	CElement* pElem = NULL;
	CJoint	* pJ_I	= NULL;
	CJoint	* pJ_J	= NULL;
	long	nApplyElem = GetLoadApplyMemberCount(pFEM);
	
	// seek bottom level
	double dBottom = 0;
	long nElem = 0;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		pElem	= pFEM->GetElement((unsigned short)nElem);
		pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);
		
		double dPos = min( pJ_I->m_vPoint.z, pJ_J->m_vPoint.z );
		dBottom = min( dPos, dBottom );
	}
	
	
	Dom.SetDirection("BOTTOM");
	
	double	dSum		= 0;
	for(nElem=0; nElem < nApplyElem; nElem++)
	{
		pElem	= pFEM->GetElement((unsigned short)nElem);
		pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);
		
		double dX	  =  pJ_J->m_vPoint.x-pJ_I->m_vPoint.x ;
		
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		if(pJ_I->m_UX==1 || pJ_I->m_UY==1 || pJ_I->m_UZ==1)
		{
			CDPointArray ptArr;
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x, dBottom));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x-Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x+Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x, dBottom));
			Dom.HatchingByCDPointArray(ptArr, 45, Dom.Always(1), TRUE);
		}
		
		if(pJ_J->m_UX==1 || pJ_J->m_UY==1 || pJ_J->m_UZ==1)
		{
			CDPointArray ptArr;
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x, dBottom));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x-Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x+Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x, pJ_J->m_vPoint.y));
			Dom.HatchingByCDPointArray(ptArr, 45, Dom.Always(1), TRUE);
		}
		
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		
		Dom.SetTextHeight(dTextHeight);
		Dom.DimMoveTo(dSum, dBottom,0);
		dSum += dX;
		Dom.DimLineTo(dX,0,COMMA(frM(dX)));
		
		if(nElem==nApplyElem-1) //맨 마지막 턴을 출력하는 곳
		{
			Dom.DimMoveTo(dSum, dBottom, 0);
			Dom.DimMoveTo(0, dBottom, 1);
			Dom.DimLineTo(dSum, 1, COMMA(frM(dSum)));
		}		
	}
	
	*pDom << Dom;
}

void CXLDesignFloor::DrawDimLoadCase(CDomyun *pDom, CFEMManage *pFEM, long nCase, double dTextHeight)
{
	// 1. seek Distance
	CDoubleArray ArrDis;

	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pE = (CElement *)pFEM->m_ElementList.GetNext(pos);		
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);
		CLoadForce *pLoadForce = pE->GetLoadForce(nCase);
		
		if(pLoadForce && pLoadForce->IsFlag(EXIST_TRAP))
		{
			for(long n=0; n<pLoadForce->m_TrapForceArr.GetSize(); n++)
			{
				if(!pLoadForce->m_TrapForceArr[n]->m_bApply) continue;
				ArrDis.Add(pJ_I->m_vPoint.x);
				ArrDis.Add(pJ_J->m_vPoint.x);
			}
		}
		if(pLoadForce && pLoadForce->IsFlag(EXIST_DISTFORCE))
		{
			CString szData;
			for(long n=0; n<pLoadForce->m_PointForceArr.GetSize(); n++)				
			{
				if(!pLoadForce->m_PointForceArr[n]->m_bApply)	continue;
				CDPoint ptStt = CDPoint(pJ_I->m_vPoint.x, pJ_I->m_vPoint.z);
				CDPoint ptEnd = CDPoint(pJ_J->m_vPoint.x, pJ_J->m_vPoint.z);
				if(pLoadForce->m_PointForceArr[n]->m_vForce.x!=0)
				{				
					CDPoint vPos = ptStt + (ptEnd - ptStt).Unit() * ~(ptEnd - ptStt) * pLoadForce->m_PointForceArr[n]->m_dDist;
					if(!pLoadForce->m_PointForceArr[n]->m_bRateDist)
						vPos = CDPoint(ptStt.x+pLoadForce->m_PointForceArr[n]->m_dDist, ptStt.y);
					
					ArrDis.Add(vPos.x);
				}
				if(pLoadForce->m_PointForceArr[n]->m_vForce.z!=0)
				{				
					CDPoint vPos = ptStt + (ptEnd - ptStt).Unit() * ~(ptEnd - ptStt) * pLoadForce->m_PointForceArr[n]->m_dDist;				
					if(!pLoadForce->m_PointForceArr[n]->m_bRateDist)
						vPos = CDPoint(ptStt.x+pLoadForce->m_PointForceArr[n]->m_dDist, ptStt.y);

					ArrDis.Add(vPos.x);
				}
				if ( ~(pLoadForce->m_PointForceArr[n]->m_vMoment)!=0 )
				{
					CDPoint vPos = ptStt + (ptEnd - ptStt).Unit() * ~(ptEnd - ptStt) * pLoadForce->m_PointForceArr[n]->m_dDist;
					ArrDis.Add(vPos.x);
				}
			}
		}	
	}

	double dLen = 0;
	pos = pFEM->m_JointList.GetHeadPosition();
	while(pos)
	{
		CJoint *pJ = (CJoint *)pFEM->m_JointList.GetNext(pos);
		
		dLen = max(dLen, pJ->m_vPoint.x);
		CLoadForceJoint *pLoadForce = pJ->GetLoadForce(nCase);
		if(pLoadForce && pLoadForce->IsFlag(EXIST_JOINT_FORCE))
		{
			CDPoint ptPos = CDPoint(pJ->m_vPoint.x, pJ->m_vPoint.z);		
			if(!pLoadForce->m_bApply) continue;
			
			if(pLoadForce->m_vForce.x!=0)
			{
				ArrDis.Add(ptPos.x);
			}
			if(pLoadForce->m_vForce.z!=0)
			{							
				ArrDis.Add(ptPos.x);
			}		
			if(~(pLoadForce->m_vMoment)!=0)
			{
				ArrDis.Add(ptPos.x);
			}
		}
	}
	ArrDis.Add(0);
	ArrDis.Add(dLen);
	ArrDis.RemoveSameValue();
	ArrDis.Sort();
		

	// 2. Set Draw
	CDomyun			Dom(pDom);
	CDataManage		*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd	*pOptStd		= pDataManage->GetOptionStd();
	double	dY = Dom.Always(10);
	double	dSum	= 0.0;
	
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("TOP");
	Dom.SetTextHeight(dTextHeight);
	
	for(long n = 0; n < ArrDis.GetSize() - 1; n++)
	{	
		Dom.DimMoveTo(dSum,	dY,	0);
		Dom.DimLineTo(ArrDis[n+1] - ArrDis[n],	0,	COMMA(frM(ArrDis[n+1] - ArrDis[n])));
		
		dSum += (ArrDis[n+1] - ArrDis[n]);
	}
	
	*pDom << Dom;
}

long CXLDesignFloor::GetLoadCase(CString szLoad)
{
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFemManage		= pCalcStd->GetCantileverFemData(0);

	long nLoadCaseSize = pFemManage->GetLoadCaseSize() - pFemManage->m_nOutputCaseAddSize;

	for (long nCase=0; nCase<nLoadCaseSize; nCase++)
	{
		CString szCaseName = pFemManage->GetLoadCaseString(nCase);
		if (szCaseName.Find(szLoad)>-1) return nCase;
	}

	return -1;
}

long CXLDesignFloor::GetLoadCaseQty(CString szLoad)
{
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFemManage		= pCalcStd->GetCantileverFemData(0);
	
	long nLoadCaseSize = pFemManage->GetLoadCaseSize() - pFemManage->m_nOutputCaseAddSize;
	long nQty = 0;

	for (long nCase=0; nCase<nLoadCaseSize; nCase++)
	{
		CString szCaseName = pFemManage->GetLoadCaseString(nCase);
		if (szCaseName.Find(szLoad)>-1) nQty++;
	}
	
	return nQty;
}

void CXLDesignFloor::DrawForceDiaghram(CDomyun *pDom, CFEMManage *pFem, long nCase, BOOL bTopDim, BOOL bBotDim)
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(0.1);
	Dom.SetTextHeight(3);
	
	// 부제 및 하중표현
	CForceDiagram ForceDom(pFem);
	ForceDom.SetScaleDim(0.1);
	ForceDom.SetTextHeight(3);
	ForceDom.DrawFrameModel(TRUE, FALSE, TRUE);
	ForceDom.DrawLoadCase(nCase, FALSE, TRUE, 0.02, FALSE, 1);
	ForceDom.Move(0, Dom.GetExtRect().Height());
	Dom << ForceDom;
	
	// 	방호벽 삽입
	DrawWallGuard(&Dom);

	if (bTopDim) DrawDimLoadCase(&Dom, pFem, nCase, 3);
	if (bBotDim) DrawDimJoint(&Dom, pFem, 3);
	
	Dom.SetCalcExtRect();
	CDRect Bound = Dom.GetExtRect();
	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	Dom.Rectangle(Bound.left-Dom.Always(8), 	Bound.top-Dom.Always(3),
				  Bound.right+Dom.Always(8), 	Bound.bottom+Dom.Always(7));

	*pDom << Dom;
}

///< 3.3 하중산정
void CXLDesignFloor::TendonLoadSum()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();
 	CFEMManage			*pFEM			= pCalcStd->GetCantileverFemData(0);
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CDomyun				Dom;

	BOOL	bIsCase = FALSE;
	long	nRow	= 5;
	long	nCase	= 0;
	long	nRowStt = 0;
	long	nCutRow = 0;
	CString szCell	= _T("");
	CString	szText	= _T("");

	//--------------------------------------------------------------------------------------------------
	// 3.3 하 중 산 정
	// 1) 횡방향 하중 분배
	// ① 합성전 고정하중슬래브 자중)
	//--------------------------------------------------------------------------------------------------
	// 도면삽입 - 슬래브 자중

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	DrawForceDiaghram(&Dom, pFEM, nCase, FALSE, TRUE);
 	pOut->InsertDomyunByFixWidth(pXL, &Dom, nRow, 2, 19, TRUE, TRUE);

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 슬래브 자중

	long nRectRow1 = 16;
	long nRectRow2 = 17;
	long nRectRow3 = 0;
	CString szRect1 = _T("");
	CString szRect2 = _T("");

	nRow	= 18;
	nRowStt = nRow;
	long nElem = 0;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;

		if(pForce->IsFlag(EXIST_TRAP))
		{
			for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
			{
				CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
				if (nRow!=nRowStt) pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("W%d", nRow-17);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pTrap->m_dRStt);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapStt.z);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 위 치 (m)
				szCell.Format("R%d", nRow);
				szText.Format("%.3f", pTrap->m_dREnd);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 하 중 (kN/m2)
				szCell.Format("V%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapEnd.z);
				pXL->SetXL(szCell, szText);
				
				nRow++;
			}
		}
	}
	// Table OutLine
	nRectRow3 = nRow-1;
	szRect1.Format("D%d", nRectRow1);
	szRect2.Format("AC%d", nRectRow2);
	pXL->CellOutLine(szRect1, szRect2);
	szRect2.Format("AC%d", nRectRow3);
	pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);

	//--------------------------------------------------------------------------------------------------	
	// ② 합성후 고정하중
	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 방호벽 하중

	nRow+=6;
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("CURB");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	
	if (bIsCase)
	{
		long nFixRow = nRow+14;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidth(pXL, &Dom, nRow, 2, 19, TRUE, TRUE);
		nRow = nFixRow;
	}
	else
	{
		for (long n=0; n<17; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=15;
		nCutRow += 17;
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 방호벽 하중
	nRectRow1 = nRow-2;
	nRectRow2 = nRow-1;
	nRowStt = nRow;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		if (!bIsCase) continue;
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;
		
		if(pForce->IsFlag(EXIST_DISTFORCE))
		{
			for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
			{
				CLoadForcePoint	*pPoint	= pForce->m_PointForceArr[nLoad];
				if (nRow!=nRowStt) pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("P%d", nRow-nRowStt+1);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pPoint->m_dDist);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pPoint->m_vForce.z);
				pXL->SetXL(szCell, szText);				
				nRow++;
			}
		}
	}

	// Table Outline
	nRectRow3 = nRow-1;
	szRect1.Format("D%d", nRectRow1);
	szRect2.Format("AC%d", nRectRow2);
	pXL->CellOutLine(szRect1, szRect2);
	szRect2.Format("AC%d", nRectRow3);
	pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);

	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 방음벽 하중	
	nRow +=2;
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("S-WALL");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		long nNewRow = nRow-nCutRow-1;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidth(pXL, &Dom, nNewRow, 2, 19, TRUE, TRUE);
	}
	else
	{
		for (long n=0; n<17; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=15;
		nCutRow += 17;
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 방음벽 하중
	
	if (bIsCase)
	{
		nRow += 14;
		nRectRow1 = nRow-2;
		nRectRow2 = nRow-1;
	}
	nRowStt = nRow;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		if (!bIsCase) continue;
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;
		
		if(pForce->IsFlag(EXIST_DISTFORCE))
		{
			for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
			{
				CLoadForcePoint	*pPoint	= pForce->m_PointForceArr[nLoad];
				if (nRow!=nRowStt) pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("P%d", nRow-nRowStt+1);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pPoint->m_dDist);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pPoint->m_vForce.z);
				pXL->SetXL(szCell, szText);				
				nRow++;
			}
		}
	}

	// table outline
	if (bIsCase)
	{
		// Table Outline
		nRectRow3 = nRow-1;
		szRect1.Format("D%d", nRectRow1);
		szRect2.Format("AC%d", nRectRow2);
		pXL->CellOutLine(szRect1, szRect2);
		szRect2.Format("AC%d", nRectRow3);
		pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);
	}

	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 난간 하중

	nRow +=2;
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("FENCE");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		long nNewRow = nRow-nCutRow-1;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidth(pXL, &Dom, nNewRow, 2, 19, TRUE, TRUE);
	}
	else
	{
		for (long n=0; n<17; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=15;
		nCutRow += 17;
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 난간 하중
	
	if (bIsCase)
	{
		nRow += 14;
		nRectRow1 = nRow-2;
		nRectRow2 = nRow-1;
	}
	nRowStt = nRow;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		if (!bIsCase) continue;
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;
		
		if(pForce->IsFlag(EXIST_DISTFORCE))
		{
			for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
			{
				CLoadForcePoint	*pPoint	= pForce->m_PointForceArr[nLoad];
				if (nRow!=nRowStt) pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("P%d", nRow-nRowStt+1);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pPoint->m_dDist);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pPoint->m_vForce.z);
				pXL->SetXL(szCell, szText);				
				nRow++;
			}
		}
	}
	// table outline
	if (bIsCase)
	{
		nRectRow3 = nRow-1;
		szRect1.Format("D%d", nRectRow1);
		szRect2.Format("AC%d", nRectRow2);
		pXL->CellOutLine(szRect1, szRect2);
		szRect2.Format("AC%d", nRectRow3);
		pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);
	}

	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 포장 하중
	
	nRow +=2;
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("PAVE");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		long nNewRow = nRow-nCutRow-1;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidth(pXL, &Dom, nNewRow, 2, 19, TRUE, TRUE);
		nRectRow1 = nRow-2;
		nRectRow2 = nRow-1;
	}
	else
	{
		for (long n=0; n<17; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=15;
		nCutRow += 17;
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 포장 하중
	
	if (bIsCase)
	{
		nRow += 14;
		nRectRow1 = nRow-2;
		nRectRow2 = nRow-1;
	}
	nRowStt = nRow;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		if (!bIsCase) continue;
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;
		
		if(pForce->IsFlag(EXIST_TRAP))
		{
			for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
			{
				CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
				if (nRow!=nRowStt) pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("W%d", nRow-nRowStt+1);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pTrap->m_dRStt);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapStt.z);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 위 치 (m)
				szCell.Format("R%d", nRow);
				szText.Format("%.3f", pTrap->m_dREnd);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 하 중 (kN/m2)
				szCell.Format("V%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapEnd.z);
				pXL->SetXL(szCell, szText);
				
				nRow++;
			}
		}
	}
	// table outline
	if (bIsCase)
	{
		nRectRow3 = nRow-1;
		szRect1.Format("D%d", nRectRow1);
		szRect2.Format("AC%d", nRectRow2);
		pXL->CellOutLine(szRect1, szRect2);
		szRect2.Format("AC%d", nRectRow3);
		pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);
	}

	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 보도부 하중
	
	nRow +=2;
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("WALKWAY");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		long nNewRow = nRow-nCutRow-1;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidth(pXL, &Dom, nNewRow, 2, 19, TRUE, TRUE);
	}
	else
	{
		for (long n=0; n<17; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=15;
		nCutRow += 17;
	}
	
	//--------------------------------------------------------------------------------------------------
	// Talble Data - 보도부 하중
	
	if (bIsCase)
	{
		nRow += 14;
		nRectRow1 = nRow-2;
		nRectRow2 = nRow-1;
	}
	nRowStt = nRow;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		if (!bIsCase) continue;
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;
		
		if(pForce->IsFlag(EXIST_TRAP))
		{
			for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
			{
				CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
				if (nRow!=nRowStt) pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("W%d", nRow-nRowStt+1);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pTrap->m_dRStt);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapStt.z);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 위 치 (m)
				szCell.Format("R%d", nRow);
				szText.Format("%.3f", pTrap->m_dREnd);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 하 중 (kN/m2)
				szCell.Format("V%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapEnd.z);
				pXL->SetXL(szCell, szText);
				
				nRow++;
			}
		}
	}
	// table outline
	if (bIsCase)
	{
		nRectRow3 = nRow-1;
		szRect1.Format("D%d", nRectRow1);
		szRect2.Format("AC%d", nRectRow2);
		pXL->CellOutLine(szRect1, szRect2);
		szRect2.Format("AC%d", nRectRow3);
		pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);
	}

	//--------------------------------------------------------------------------------------------------
	// ③ 활 하 중		
	// 도면 삽입 - 좌측 편재하 DB 1차로 (LDB-1)

	nRow +=2;
	Dom.ClearEtt(TRUE);

	long	nLoadCaseSize = pFEM->GetLoadCaseSize();
	CString szLoadCase = "";
	long	nLane = 0;
	long nLoadCase = 0;
	for(nLoadCase = 0; nLoadCase < nLoadCaseSize; nLoadCase++)
	{
		szLoadCase = pFEM->GetLoadCaseString(nLoadCase);
		if(szLoadCase.Find("DBL") != -1)
		{
			szText.Format(" - 좌측 편재하 DB %d차로 (%s)", nLane+1, szLoadCase);
			szCell.Format("E%d", nRow);
			pXL->SetXL(szCell, szText);
			nRow++;
			Dom.ClearEtt(TRUE);
			DrawForceDiaghram(&Dom, pFEM, nLoadCase, TRUE, FALSE);
			pXL->InsertRowLine(nRow, 11);
			pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	

			nLane++;
		}
	}
	pXL->InsertRowLine(nRow, 1);
	nLane = 0;
	for(nLoadCase = 0; nLoadCase < nLoadCaseSize; nLoadCase++)
	{
		szLoadCase = pFEM->GetLoadCaseString(nLoadCase);
		if(szLoadCase.Find("DBR") != -1)
		{
			szText.Format(" - 우측 편재하 DB %d차로 (%s)", nLane+1, szLoadCase);
			szCell.Format("E%d", nRow);
			pXL->SetXL(szCell, szText);
			nRow++;
			Dom.ClearEtt(TRUE);
			DrawForceDiaghram(&Dom, pFEM, nLoadCase, TRUE, FALSE);
			pXL->InsertRowLine(nRow, 11);
			pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	

			nLane++;
		}
	}

	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 좌측 군집하중 (L-PEOPLE)	
	nRow +=3;
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("L-PEOPLE");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		nRow++;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	
	}
	else
	{
		for (long n=0; n<17; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=17;
	}
	
	//--------------------------------------------------------------------------------------------------
	// Talble Data - 좌측 군집하중 (L-PEOPLE)
	
	if (bIsCase)
	{
		nRow += 4;
		nRectRow1 = nRow-2;
		nRectRow2 = nRow-1;
	}
	nRowStt = nRow;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		if (!bIsCase) continue;
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;
		
		if(pForce->IsFlag(EXIST_TRAP))
		{
			for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
			{
				CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
				if (nRow!=nRowStt)
					pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("W%d", nRow-nRowStt+1);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pTrap->m_dRStt);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapStt.z);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 위 치 (m)
				szCell.Format("R%d", nRow);
				szText.Format("%.3f", pTrap->m_dREnd);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 하 중 (kN/m2)
				szCell.Format("V%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapEnd.z);
				pXL->SetXL(szCell, szText);
				
				nRow++;
			}
		}
	}
	// table outline
	if (bIsCase)
	{
		nRectRow3 = nRow-1;
		szRect1.Format("D%d", nRectRow1);
		szRect2.Format("AC%d", nRectRow2);
		pXL->CellOutLine(szRect1, szRect2);
		szRect2.Format("AC%d", nRectRow3);
		pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);
		nRow++;
	}

	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 우측 군집하중 (R-PEOPLE)
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("R-PEOPLE");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		nRow++;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	
	}
	else
	{
		for (long n=0; n<17; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=17;
	}
	
	//--------------------------------------------------------------------------------------------------
	// Talble Data - 우측 군집하중 (R-PEOPLE)
	
	if (bIsCase)
	{
		nRow += 4;
		nRectRow1 = nRow-2;
		nRectRow2 = nRow-1;
	}
	nRowStt = nRow;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		if (!bIsCase) continue;
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForce	= pElem->GetLoadForce(nCase);
		if(!pForce)	return;
		
		if(pForce->IsFlag(EXIST_TRAP))
		{
			for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
			{
				CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
				if (nRow!=nRowStt) pXL->InsertCopyRowLine(nRow-1, nRow-1, nRow);
				
				// 하 중 명
				szCell.Format("D%d", nRow);
				szText.Format("W%d", nRow-nRowStt+1);
				pXL->SetXL(szCell, szText);
				
				// 재하부재
				szCell.Format("G%d", nRow);
				szText.Format("%d", nElem+1);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 위 치 (m)
				szCell.Format("J%d", nRow);
				szText.Format("%.3f", pTrap->m_dRStt);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 좌측 제원 - 하 중 (kN/m2)
				szCell.Format("N%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapStt.z);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 위 치 (m)
				szCell.Format("R%d", nRow);
				szText.Format("%.3f", pTrap->m_dREnd);
				pXL->SetXL(szCell, szText);
				
				// 작용하중 우측 제원 - 하 중 (kN/m2)
				szCell.Format("V%d", nRow);
				szText.Format("%.3f", -pTrap->m_vTrapEnd.z);
				pXL->SetXL(szCell, szText);
				
				nRow++;
			}
		}
	}
	// table outline
	if (bIsCase)
	{
		nRectRow3 = nRow-1;
		szRect1.Format("D%d", nRectRow1);
		szRect2.Format("AC%d", nRectRow2);
		pXL->CellOutLine(szRect1, szRect2);
		szRect2.Format("AC%d", nRectRow3);
		pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);
		nRow++;
	}

	//--------------------------------------------------------------------------------------------------
	// ④ 특수하중	
	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 탱크 궤도하중
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("TANK");
	long nCaseQty = GetLoadCaseQty("TANK"); 
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		nRow += 2;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	
		nRow++;
		//if (nCaseQty>1)
		//{
		//	nRow += 11;
		//	for (long n=1; n<nCaseQty; n++)
		//	{
		//		CString szCase = _T("");
		//		szCase.Format("TANK-%d", n+1);
		//		nCase= GetLoadCase(szCase);
		//		pXL->InsertRowLine(nRow, 11);
		//		Dom.ClearEtt(TRUE);
		//		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		//		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	
		//	}
		//}
	}
	else
	{
		for (long n=0; n<13; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=13;
	}
	
	//--------------------------------------------------------------------------------------------------
	// 도면 삽입 - 트레일러 하중
	
	Dom.ClearEtt(TRUE);	
	nCase = GetLoadCase("TRAILER");
	nCaseQty = GetLoadCaseQty("TRAILER");
	bIsCase = (nCase>-1)? TRUE : FALSE;
	if (bIsCase)
	{
		nRow++;
		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	

		//if (nCaseQty>1)
		//{
		//	nRow += 11;
		//	for (long n=1; n<nCaseQty; n++)
		//	{
		//		CString szCase = _T("");
		//		szCase.Format("TRAILER-%d", n+1);
		//		nCase = GetLoadCase(szCase);
		//		pXL->InsertRowLine(nRow, 11);
		//		Dom.ClearEtt(TRUE);
		//		DrawForceDiaghram(&Dom, pFEM, nCase, TRUE, FALSE);
		//		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, nRow, 1, 10, 28, 30, TRUE, FALSE, FALSE);	
		//	}
		//}

		nRow ++;
	}
	else
	{
		for (long n=0; n<13; n++)
			pXL->SetCellHeight(nRow+n-2, nRow+n-2, 0);
		nRow +=12;
	}

	//--------------------------------------------------------------------------------------------------	
	// ⑤ 풍 하 중	
	//--------------------------------------------------------------------------------------------------
	// 작용 풍하중 강도
	long nWindLL = GetLoadCase(LOAD_WIND_LL);
	long nWindLR = GetLoadCase(LOAD_WIND_LR);
	long nWindRL = GetLoadCase(LOAD_WIND_RL);
	long nWindRR = GetLoadCase(LOAD_WIND_RR);
	
	if (nWindLL<0 && nWindLR<0 && nWindRL<0 && nWindRR<0)
	{
		pXL->SetCellHeight(nRow-1, nRow-1, 0);
		pXL->SetCellHeight(nRow+0, nRow+0, 0);
	}
	
	// Talble Data - 좌측 방음벽 (좌측 ⇒ 우측)
	nRow += 2;
	bIsCase = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
	if (bIsCase)
	{
		nRow++;
		pOut->InsertImage(pXL, "방음벽1", nRow, "K");
	}
	else
	{
		nRow++;
	}
	nRow += 10;


	// 재하부재
	szCell.Format("G%d", nRow);
	szText.Format("%d", 1);
	if (bIsCase) pXL->SetXL(szCell, szText);

	// h1
	double dFlange		= 0;
	double dWeb			= 0;
	SeekFlange_Web(dFlange, dWeb);

	double dh1 = toM(dFlange);
	szCell.Format("I%d", nRow);
	szText.Format("%.3f", dh1);
	if (bIsCase) pXL->SetXL(szCell, szText);

	// h2
	double dh2 = toM(pDB->m_dThickSlabBase);
	szCell.Format("L%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h3
	double dh3 = toM(pDB->m_dThickPave);
	szCell.Format("O%d", nRow);
	szText.Format("%.3f", dh3);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h4
	double dh4 = toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight);
	dh4 -= dh3;
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh4);
	if (bIsCase) pXL->SetXL(szCell, szText);

	if (!bIsCase)
	{
 		for (long j=0; j<13; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 좌측 방음벽 (우측 ⇒ 좌측)
	nRow += 2;
	if (bIsCase)
	{
		nRow++;
		pOut->InsertImage(pXL, "방음벽2", nRow, "K");
	}
	else
	{
		nRow++;
	}
	nRow += 10;
	
	// h2
	dh2 = toM(pDB->m_dThickSlabBase);
	szCell.Format("L%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h3
	dh3 = toM(pDB->m_dThickPave);
	szCell.Format("O%d", nRow);
	szText.Format("%.3f", dh3);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h4
	dh4 = toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight);
	dh4 -= dh3;
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh4);
	if (bIsCase) pXL->SetXL(szCell, szText);

	if (!bIsCase)
	{
		for (long j=0; j<13; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 우측 방음벽 (우측 ⇒ 좌측)
	nRow += 2;
	bIsCase = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;
	if (bIsCase)
	{
		nRow++;
		pOut->InsertImage(pXL, "방음벽3", nRow, "K");
	}
	else
	{
		nRow++;
	}
	nRow += 10;

	// 재하부재
	szCell.Format("G%d", nRow);
	szText.Format("%d", pFEM->GetElementSize());
	if (bIsCase) pXL->SetXL(szCell, szText);
	

	dh1 = toM(dFlange);
	szCell.Format("I%d", nRow);
	szText.Format("%.3f", dh1);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h2
	dh2 = toM(pDB->m_dThickSlabBase);
	szCell.Format("L%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h3
	dh3 = toM(pDB->m_dThickPave);
	szCell.Format("O%d", nRow);
	szText.Format("%.3f", dh3);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h4
	dh4 = toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight);
	dh4 -= dh3;
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh4);
	if (bIsCase) pXL->SetXL(szCell, szText);

	if (!bIsCase)
	{
		for (long j=0; j<13; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 우측 방음벽 (좌측 ⇒ 우측)
	nRow += 2;
	if (bIsCase)
	{
		nRow++;
		pOut->InsertImage(pXL, "방음벽4", nRow, "K");
	}
	else
	{
		nRow++;
	}
	nRow += 10;

	// 재하부재
	szCell.Format("G%d", nRow);
	szText.Format("%d", pFEM->GetElementSize());
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	
	// h2
	dh2 = toM(pDB->m_dThickSlabBase);
	szCell.Format("L%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h3
	dh3 = toM(pDB->m_dThickPave);
	szCell.Format("O%d", nRow);
	szText.Format("%.3f", dh3);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h4
	dh4 = toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight);
	dh4 -= dh3;
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh4);
	if (bIsCase) pXL->SetXL(szCell, szText);

	if (!bIsCase)
	{
		for (long j=0; j<13; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}

	//--------------------------------------------------------------------------------------------------
	// ⑥ 충 돌 하 중
	// Table Data - 좌 측 난 간
	long nCrashL = GetLoadCase(LOAD_CRASH_L);
	long nCrashR = GetLoadCase(LOAD_CRASH_R);
	nRow += 2;

	if (nCrashL<0 && nCrashR<0)
		pXL->SetCellHeight(nRow-1, nRow-1, 0);

	bIsCase = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan;
	if (bIsCase)
	{
		nRow +=2;
		pOut->InsertImage(pXL, "난간1", nRow, "K");
	}
	else
	{
		pXL->SetCellHeight(nRow, nRow, 0);
		nRow +=2;
	}
	
	nRow += 10;
	CCentSeparation *pGuard = pCalcStd->GetGuardWall(TRUE);
	//BOOL bMatch = pGuard ? TRUE : FALSE;
	//long i = 0;
	//for (i=0; i<pDB->GetGuardFencesu(); i++)
	//{
	//	pGuard = pDB->GetGuardFence(i);
	//	if (pGuard->m_strName==_T("방호벽(좌)"))
	//	{
	//		bMatch = TRUE;
	//		break;
	//	}
	//	if (!bMatch) pGuard = NULL;
	//}
	
	// h1
	dh1 = (pGuard)? toM(pGuard->GetHeight()) : 0;
	szCell.Format("O%d", nRow);
	szText.Format("%.3f", dh1);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h2
	dh2 = toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight);
	dh2 -= dh1;
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	if (!bIsCase)
	{
		for (long j=0; j<12; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}

	//--------------------------------------------------------------------------------------------------
	// Talble Data - 우 측 난 간
	nRow += 2;
	bIsCase = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan;
	if (bIsCase)
	{
		nRow++;
		pOut->InsertImage(pXL, "난간2", nRow, "K");
	}
	else
	{
		pXL->SetCellHeight(nRow-1, nRow-1, 0);
		nRow ++;
	}
	nRow += 10;
	pGuard = pCalcStd->GetGuardWall(FALSE);
	//bMatch = pGuard ? TRUE : FALSE;
	//for (i=0; i<pDB->GetGuardFencesu(); i++)
	//{
	//	pGuard = pDB->GetGuardFence(i);
	//	if (pGuard->m_strName==_T("방호벽(우)"))
	//	{
	//		bMatch = TRUE;
	//		break;
	//	}
	//	if (!bMatch) pGuard = NULL;
	//}
	
	// 재하부재
	szCell.Format("I%d", nRow);
	szText.Format("%d", pFEM->GetElementSize());
	if (bIsCase) pXL->SetXL(szCell, szText);

	// h1
	dh1 = (pGuard)? toM(pGuard->GetHeight()) : 0;
	szCell.Format("O%d", nRow);
	szText.Format("%.3f", dh1);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h2
	dh2 = toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight);
	dh2 -= dh1;
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	if (!bIsCase)
	{
		for (long j=0; j<12; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}
	
	//--------------------------------------------------------------------------------------------------
	// 	Talble Data - 좌측 방호벽

	pGuard = pCalcStd->GetGuardWall(TRUE);
	bIsCase = pGuard ? TRUE : FALSE;
	//for (i=0; i<pDB->GetGuardFencesu(); i++)
	//{
	//	pGuard = pDB->GetGuardFence(i);
	//	if (pGuard->m_strName==_T("방호벽(좌)"))
	//	{
	//		bIsCase = TRUE;
	//		break;
	//	}
	//	if (!bIsCase) pGuard = NULL;
	//}

	nRow += 5;
	if (bIsCase) pOut->InsertImage(pXL, "방호벽1", nRow, "E");
	if (bIsCase) pOut->InsertImage(pXL, "방호벽2", nRow, "S");
	nRow += 10;

	// h1
	dh1 = toM(pDB->m_dThickPave);
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh1);
	if (bIsCase) pXL->SetXL(szCell, szText);

	// h2
	dh2 = (pGuard)? toM(pGuard->GetHeight()) : 0;
	dh2 -= dh1;
	szCell.Format("U%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	if (!bIsCase)
	{
		long nLimit = (nCrashL<0)? 15 : 12;
		for (long j=0; j<nLimit; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}

  //--------------------------------------------------------------------------------------------------
	// 	Talble Data - 우측 방호벽
	
	pGuard = pCalcStd->GetGuardWall(FALSE);
	bIsCase = pGuard ? TRUE : FALSE;
	//for (i=0; i<pDB->GetGuardFencesu(); i++)
	//{
	//	pGuard = pDB->GetGuardFence(i);
	//	if (pGuard->m_strName==_T("방호벽(우)"))
	//	{
	//		bIsCase = TRUE;
	//		break;
	//	}
	//	if (!bIsCase) pGuard = NULL;
	//}

	nRow += 5;
	if (bIsCase) pOut->InsertImage(pXL, "방호벽3", nRow, "E");
	if (bIsCase) pOut->InsertImage(pXL, "방호벽4", nRow, "S");
	nRow += 10;
	
	// 재하부재
	szCell.Format("I%d", nRow);
	szText.Format("%d", pFEM->GetElementSize());
	if (bIsCase) pXL->SetXL(szCell, szText);

	// h1
	dh1 = toM(pDB->m_dThickPave);
	szCell.Format("R%d", nRow);
	szText.Format("%.3f", dh1);
	if (bIsCase) pXL->SetXL(szCell, szText);
	
	// h2
	dh2 = (pGuard)? toM(pGuard->GetHeight()) : 0;
	dh2 -= dh1;
	szCell.Format("U%d", nRow);
	szText.Format("%.3f", dh2);
	if (bIsCase) pXL->SetXL(szCell, szText);
	if (!bIsCase)
	{
		for (long j=0; j<12; j++)
			pXL->SetCellHeight(nRow-j, nRow-j, 0);
	}

	// ⑦ 원심하중
	long nCentriLoad = GetLoadCase("CF");
	nRow += 1;
	
 	if (nCentriLoad>1)
	{
		nRow +=12;
		nRow +=46;

		for (long j=0; j<49; j++)
		{
			pXL->SetCellHeight(nRow+j, nRow+j, 0);
		}
	}
	else
	{
		for (long j=0; j<108; j++)
		{
			pXL->SetCellHeight(nRow+j, nRow+j, 0);
		}
	}

}

void GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr)
{
	//-------------------------------------------------------------------------------------------
	// Seek Flange X-Position
	// nSide - 0:Top Flange Left, 2:Top Flange Center, 1:Top Flange Right
	
	CDoubleArray FlangePos;
	BOOL	bFlangeLevel = FALSE;
	double  dFlangeLevel = 0;
	double	dSlabLevel	 = 0;
	
	for (long nJ=0; nJ<pFEM->GetJointSize(); nJ++)
	{
		CJoint* pJoint		= pFEM->GetJoint((unsigned short)nJ);
		double  dJoint_z	= pJoint->m_vPoint.z;
		double  dJoint_x	= pJoint->m_vPoint.x;
		
		if (!bFlangeLevel && dJoint_z !=dSlabLevel)
		{
			bFlangeLevel = TRUE;
			dFlangeLevel = dJoint_z;
		}
		
		if (bFlangeLevel && dFlangeLevel==dJoint_z)
			FlangePos.Add(dJoint_x);
	}
	FlangePos.Sort();
	
	//-------------------------------------------------------------------------------------------
	// Seek Element Index
	if (nSide==0)	// Left Cantilever
	{
		// Target Position
		double dTarget = FlangePos.GetAt(0);
		
		// Seek Element
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			
			if (pJ_S->m_vPoint.x>dTarget) break;
			nArr.Add(nE);
		}
	}
	else if (nSide==1)	// Right Cantilever
	{
		// Target Position
		double dTarget = FlangePos.GetAt(FlangePos.GetUpperBound());

		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			CJoint*   pJ_E  = pFEM->GetJoint(pElem->m_nJEnd);
			
			if (pJ_S->m_vPoint.z<dSlabLevel) break;
			if (pJ_E->m_vPoint.x<dTarget) continue;
			nArr.Add(nE);
		}
	}
	else	// Span Middle Member
	{
		// Target Position
		CDoubleArray dTarget;
		for (long nP=2; nP<FlangePos.GetSize()-1; nP+=3)
		{
			double dPos = (FlangePos.GetAt(nP) + FlangePos.GetAt(nP+1)) / 2;
			dTarget.Add(dPos);
		}
		
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			CJoint*   pJ_E  = pFEM->GetJoint(pElem->m_nJEnd);
			
			if (pJ_S->m_vPoint.z<dSlabLevel) break;
			
			for (long n=0; n<dTarget.GetSize(); n++)
			{
				double dPos = dTarget.GetAt(n);
				if (fabs(pJ_S->m_vPoint.x-dPos)<EPSILON || fabs(pJ_E->m_vPoint.x-dPos)<EPSILON)
					nArr.Add(nE);
			}
		}		
	}
}

///< 3.4 단면력 집계(1)
void CXLDesignFloor::TendonSectionSum()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();
	CFEMManage			*pFEM			= pCalcStd->GetCantileverFemData(0);
	CXLControl			*pXL			= m_pXL;
	CDomyun				Dom;
	CCalcFloor			CalcFloor(pDataManage);
	
	long	nRow	= 5;
	CString szCell	= _T("");
	CString	szText	= _T("");

	//--------------------------------------------------------------------------------------------------
	// 3.4 하 중 산 정
	// ㆍ 단면력 정리
	double dRes_Before[4]	= { 0, 0, 0, 0 };
	double dRes_After[4]	= { 0, 0, 0, 0 };
	double dRes_Live_Max[4]	= { 0, 0, 0, 0 };
	double dRes_Live_Min[4] = { 0, 0, 0, 0 };
	double dRes_Tank_Max[4]	= { 0, 0, 0, 0 };
	double dRes_Tank_Min[4]	= { 0, 0, 0, 0 };
	double dRes_Trailer_Max[4]= { 0, 0, 0, 0 };
	double dRes_Trailer_Min[4]= { 0, 0, 0, 0 };
	double dRes_Wind_Max[4]	= { 0, 0, 0, 0 };
	double dRes_Wind_Min[4] = { 0, 0, 0, 0 };
	double dRes_Crash_Max[4]= { 0, 0, 0, 0 };
	double dRes_Crash_Min[4]= { 0, 0, 0, 0 };
	double dRes_CF_Max[4]	= { 0, 0, 0, 0 };
	double dRes_CF_Min[4]	= { 0, 0, 0, 0 };

	for (long nPos = 0; nPos<4; nPos++)
	{
		dRes_Before[nPos]			= CalcFloor.GetMemberForce(pFEM, "BEFORE", ELE_MOMENT2, nPos);
		dRes_After[nPos]			= CalcFloor.GetMemberForce(pFEM, "AFTER", ELE_MOMENT2, nPos);
		dRes_Live_Max[nPos]		= CalcFloor.GetMemberForce(pFEM, "LIVE MAX", ELE_MOMENT2, nPos);
		dRes_Live_Min[nPos]		= CalcFloor.GetMemberForce(pFEM, "LIVE MIN", ELE_MOMENT2, nPos);
		dRes_Tank_Max[nPos]	 = CalcFloor.GetMemberForce(pFEM, "TK MAX", ELE_MOMENT2, nPos);
		dRes_Tank_Min[nPos]		 = CalcFloor.GetMemberForce(pFEM, "TK MIN", ELE_MOMENT2, nPos);
		dRes_Trailer_Max[nPos]	 = CalcFloor.GetMemberForce(pFEM, "TR MAX", ELE_MOMENT2, nPos);
		dRes_Trailer_Min[nPos]	 = CalcFloor.GetMemberForce(pFEM, "TR MIN", ELE_MOMENT2, nPos);
		dRes_Wind_Max[nPos]	 = CalcFloor.GetMemberForce(pFEM, "WIND MAX", ELE_MOMENT2, nPos);
		dRes_Wind_Min[nPos]	 = CalcFloor.GetMemberForce(pFEM, "WIND MIN", ELE_MOMENT2, nPos);
		dRes_CF_Max[nPos]		 = CalcFloor.GetMemberForce(pFEM, "CF MAX", ELE_MOMENT2, nPos);
		dRes_CF_Min[nPos]		 = CalcFloor.GetMemberForce(pFEM, "CF MIN", ELE_MOMENT2, nPos);
		dRes_Crash_Max[nPos]	= CalcFloor.GetMemberForce(pFEM, "CRASH MAX", ELE_MOMENT2, nPos);
		dRes_Crash_Min[nPos]	= CalcFloor.GetMemberForce(pFEM, "CRASH MIN", ELE_MOMENT2, nPos);
	}

	//--------------------------------------------------------------------------------------------------
	// 합성전 고정하중
	nRow = 54;
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Before[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Before[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_Before[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Before[1]);	pXL->SetXL(szCell, szText);
	
	//--------------------------------------------------------------------------------------------------
	// 합성전 고정하중
	nRow = 56;
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_After[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_After[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_After[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_After[1]);	pXL->SetXL(szCell, szText);
	
	//--------------------------------------------------------------------------------------------------
	// 활하중
	nRow = 58;
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Live_Max[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+1);	szText.Format("%.3f", dRes_Live_Min[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Live_Max[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_Live_Min[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_Live_Max[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+3);	szText.Format("%.3f", dRes_Live_Min[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Live_Max[1]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+1);	szText.Format("%.3f", dRes_Live_Min[1]);	pXL->SetXL(szCell, szText);
	
	//--------------------------------------------------------------------------------------------------
	// 탱크하중
	nRow = 62;
// 	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Tank[0]);	pXL->SetXL(szCell, szText);
// 	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Tank[1]);	pXL->SetXL(szCell, szText);
// 	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_Tank[2]);	pXL->SetXL(szCell, szText);
// 	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Tank[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Tank_Max[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+1);	szText.Format("%.3f", dRes_Tank_Min[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Tank_Max[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_Tank_Min[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_Tank_Max[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+3);	szText.Format("%.3f", dRes_Tank_Min[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Tank_Max[1]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+1);	szText.Format("%.3f", dRes_Tank_Min[1]);	pXL->SetXL(szCell, szText);
	
	//--------------------------------------------------------------------------------------------------
	// 트레일러하중
	nRow = 66;
// 	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Trailer[0]);	pXL->SetXL(szCell, szText);
// 	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Trailer[1]);	pXL->SetXL(szCell, szText);
// 	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_Trailer[2]);	pXL->SetXL(szCell, szText);
// 	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Trailer[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Trailer_Max[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+1);	szText.Format("%.3f", dRes_Trailer_Min[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Trailer_Max[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_Trailer_Min[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_Trailer_Max[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+3);	szText.Format("%.3f", dRes_Trailer_Min[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Trailer_Max[1]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+1);	szText.Format("%.3f", dRes_Trailer_Min[1]);	pXL->SetXL(szCell, szText);

	//--------------------------------------------------------------------------------------------------
	// 풍하중
	nRow = 70;
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Wind_Max[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+1);	szText.Format("%.3f", dRes_Wind_Min[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Wind_Max[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_Wind_Min[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_Wind_Max[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+3);	szText.Format("%.3f", dRes_Wind_Min[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Wind_Max[1]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+1);	szText.Format("%.3f", dRes_Wind_Min[1]);	pXL->SetXL(szCell, szText);
	
	//--------------------------------------------------------------------------------------------------
	// 충돌하중
	nRow = 74;
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_Crash_Max[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+1);	szText.Format("%.3f", dRes_Crash_Min[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_Crash_Max[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_Crash_Min[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_Crash_Max[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+3);	szText.Format("%.3f", dRes_Crash_Min[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_Crash_Max[1]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+1);	szText.Format("%.3f", dRes_Crash_Min[1]);	pXL->SetXL(szCell, szText);
	
	//--------------------------------------------------------------------------------------------------
	// 원심하중
	nRow = 78;
	szCell.Format("J%d", nRow+0);	szText.Format("%.3f", dRes_CF_Max[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow+1);	szText.Format("%.3f", dRes_CF_Min[0]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+0);	szText.Format("%.3f", dRes_CF_Max[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+1);	szText.Format("%.3f", dRes_CF_Min[2]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+2);	szText.Format("%.3f", dRes_CF_Max[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow+3);	szText.Format("%.3f", dRes_CF_Min[3]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+0);	szText.Format("%.3f", dRes_CF_Max[1]);	pXL->SetXL(szCell, szText);
	szCell.Format("T%d", nRow+1);	szText.Format("%.3f", dRes_CF_Min[1]);	pXL->SetXL(szCell, szText);
}

double CXLDesignFloor::GetDistance(CDPoint& p1, CDPoint& p2)
{
	double dx = p1.x - p2.x;
	double dy = p1.y - p2.y;
	double dis = sqrt(dx*dx + dy*dy);
	return dis;
}

void CXLDesignFloor::ReSetTendonDistance(CDPointArray& PosArr, long nPos, CDoubleArray& DisArr)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();

	// 고정단 위치
	BOOL bLeftFix = (pDB->m_nTendonFix != 1) ? TRUE : FALSE;
	
	// 이격거리
	double dSecMoveStt = pDB->m_dLengthSecMove;
	double dSecMoveEnd = pDB->m_dLengthSecMoveEnd;

	// Seek Top Flange Width
	double dFlange		= 0;
	double dWeb			= 0;
	SeekFlange_Web(dFlange, dWeb);

	////////////////////////////////////////////////////////////////////////////////////
	// Seek Girder Position Array
	////////////////////////////////////////////////////////////////////////////////////
	CDPoint xyPos = PosArr.GetAt(0);
	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	CDPointArray xyGirArr;
	CDPointArray xyDis;

	xyGirArr.Add(xyPos);

	double dLen = pDB->GetWidthSlabLeft();

	if (bLeftFix)
	{
		dLen -= dSecMoveStt;
		dLen -= dFlange/2;
	}
	else
	{
		dLen -= dSecMoveEnd;
		dLen -= 140;
		dLen -= dFlange/2;
	}

	xyPos.x += dLen;
	xyGirArr.Add(xyPos);

	dLen = 0;
	for (long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
 		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(0));
		dLen = pDB->GetGirder(nG)->GetDisGirderToGirder(pBxMatch);

		if (nG==0) dLen += dFlange/2;
		if (nG==pDB->GetGirdersu()-2) dLen += dFlange/2;
		
	}
	xyPos.x += dLen/2;
	xyGirArr.Add(xyPos);
	xyPos.x += dLen/2;
	xyGirArr.Add(xyPos);

	dLen = pDB->GetWidthSlabRight();
	if (bLeftFix)
	{
 		dLen -= dSecMoveEnd;
		dLen -= 140;
		dLen -= dFlange/2;
	}
	else
	{
 		dLen -= dSecMoveStt;
		dLen -= dFlange/2;
	}
	xyPos.x += dLen;
	xyGirArr.Add(xyPos);


	// set tendon distance by position 
	CDPoint xyTarget = xyGirArr.GetAt(1);

	switch (nPos)
	{
	case 0:  xyTarget = xyGirArr.GetAt(1);	break;
	case 1:  xyTarget = xyGirArr.GetAt(2);	break;
	case 2:  xyTarget = xyGirArr.GetAt(3);	break;
	}

	long	nCnt = 0;	
	
	for (long n=0; n<PosArr.GetSize(); n++)
	{
		xyPos = PosArr.GetAt(n);
		if (xyPos.x < xyTarget.x) continue;
		nCnt++;
		
		if (nCnt==1)
		{
			CDPoint p1 = PosArr.GetAt(n-1);
			CDPoint p2 = xyPos;
			CDPoint Ang = (p2-p1).Unit();
			CDPoint p3;
			
			GetXyMatchLineAndLine(p1, Ang, xyTarget, CDPoint(0,1), p3);
			xyDis.Add(p3);
		}
		xyDis.Add(xyPos);
	}

	// Seek Distance
	DisArr.RemoveAll();
	for (long k=0; k<xyDis.GetSize()-1; k++)
	{
		double Dis = GetDistance(xyDis.GetAt(k), xyDis.GetAt(k+1));
		DisArr.Add(Dis);
	}
}

///< 3.5 프리스트레스의 손실(1)
void CXLDesignFloor::TendonPreStressLose()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CDomyun				Dom;
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();
	
	long	nRow	= 3;
	CString szCell	= _T("");
	CString	szText	= _T("");

	// 3.5 프리스트레스의 손실
	// 도면 삽입
	CAPlateDrawDanmyun DrawDanmyun(pDataManage);
	CAPlateDrawCross DrawCross(pDataManage);
	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	CStringArray szPosArr;
	CDoubleArray dAngArr;
	CDoubleArray ReDisArr;
	CDPointArray xyPosArr; 

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);

	DrawDanmyun.DrawGuardWall(&Dom, pBx);													// 방호벽
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, TRUE, CAPlateDrawCross::NOTDRAWVSTIFF);	// 지점부 슬래브 단면
	DrawDanmyun.DrawTensionBar_ExcelOut(&Dom, pBx, szPosArr, xyPosArr, dAngArr, TRUE);		// 지점부 슬래브 단면	

	///< 치수선그리기
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 75*(dDiagonalL/10000);
	Dom.SetScaleDim(dScale);
	
	DrawDanmyun.DimTensionBarOnLineInfo(&Dom, pBx, TRUE);	//직선
	DrawDanmyun.DimTensionBar(&Dom, pBx, TRUE);				//변곡선
	DrawDanmyun.DimTypicalSectionBottom(&Dom, pBx);
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
	pOut->InsertDomyunByFixWidth(pXL, &Dom, nRow, 2, 21, TRUE, TRUE);


	// Tendon 배치 간격
	nRow = 2;
	double dCTC		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	szCell.Format("L%d", nRow);
	szText.Format("%.1f", dCTC);
	pXL->SetXL(szCell, szText);

	// 2) Pre-stress 도입
	nRow = 23;
	szCell.Format("E%d", nRow);
	szText = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName;
	long nPos = szText.Find("m");
	szText = szText.Left(nPos);
	nPos = szText.Find(" ");
	szText = szText.Right(nPos-2);

	pXL->SetXL(szCell, szText);

	// 정착단에서 PS강재의 초기 긴장응력
	CString strJackMax = _T(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax);
	if(strJackMax.Find(_T("fpu")) < 0 && strJackMax.Find(_T("fpy")) < 0)	//사용자 정의 선택 시
	{
		nRow = 25;
		double dJackMax = atof(strJackMax);
		szCell.Format("F%d", nRow);
		szText.Format("%.3f", dJackMax);
		pXL->SetXL(szCell, szText);
		 
		szCell.Format("J%d", nRow);
		szText.Format("");
		pXL->SetXL(szCell, szText);
		 
		szCell.Format("M%d", nRow);
		szText.Format("");
		pXL->SetXL(szCell, szText);
	}


	// 정착위치
	BOOL bLeftFix = (pDB->m_nTendonFix != 1) ? TRUE : FALSE;	// 고정단이 좌측&지그재그 : TRUE, 우측 : FALSE

	// (1) 좌측 캔틸레버부 (고정단)						
	// ①	PC 강재와 Sheath 사이의 마찰에 의한 손실
	nRow = 28;
	szCell.Format("I%d", nRow);
	szText = (bLeftFix)? _T("고정단") : _T("긴장단");
	pXL->SetXL(szCell, szText);

	ReSetTendonDistance(xyPosArr, 0, ReDisArr);
	long nResize = dAngArr.GetSize() - ReDisArr.GetSize();
	double dAlpha = 0;
	long n = 0;
	for (n=0; n<ReDisArr.GetSize(); n++)
		dAlpha += dAngArr.GetAt(n+nResize);
		
	nRow = 32;
	szCell.Format("G%d", nRow);
	szText.Format("%.5f", dAlpha);
	pXL->SetXL(szCell, szText);

	nRow = 34;
	szCell.Format("G%d", nRow);
	szText.Format("%.3f", ReDisArr.Sum());
	pXL->SetXL(szCell, szText);


	nRow = 37;
	long nCnt = 0;
	long nRectRow1 = nRow-1;
	CString szRect1 = _T("");
	CString szRect2 = _T("");

	for (n=0; n<ReDisArr.GetSize(); n++)
	{
		if (n==0)
		{
			szCell.Format("D%d", nRow);
			szText.Format("%s%s", szPosArr.GetAt(n+nResize+1), _T("x"));
			pXL->SetXL(szCell, szText);

			szCell.Format("Z%d", nRow);
			szText= _T("검토위치");
			pXL->SetXL(szCell, szText);
		}
		else
		{
			pXL->InsertCopyRowLine(nRow, nRow, nRow+1);

			szCell.Format("D%d", nRow);
			szText.Format("%s%s", szPosArr.GetAt(n+nResize+1), szPosArr.GetAt(n+nResize+0));
			pXL->SetXL(szCell, szText);

			szCell.Format("Z%d", nRow);
			szText= (n!=ReDisArr.GetSize()-1)? _T("") : (bLeftFix)? _T("긴장단") : _T("고정단");
			pXL->SetXL(szCell, szText);
		}	
	
		szCell.Format("G%d", nRow);
		szText.Format("%.6f", toM(ReDisArr.GetAt(n)));
		pXL->SetXL(szCell, szText);
		
		szCell.Format("N%d", nRow);
		szText.Format("%.5f", dAngArr.GetAt(n+nResize));
		pXL->SetXL(szCell, szText);		
		nCnt++;
	}
	nRow += nCnt;
	long nRectRow2 = nRow-1;

	// table outline
	szRect1.Format("D%d", nRectRow1);
	szRect2.Format("AB%d", nRectRow1);
	pXL->CellOutLine(szRect1, szRect2);
	szRect2.Format("AB%d", nRectRow2);
	pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);


	// ② 정착장치 활동에 의한 손실 ( PS 강재와 쉬스 사이에 마찰이 있는 경우 )
	nRow += 4;
	szCell.Format("G%d", nRow);
	szText.Format("%d", (bLeftFix)? 0 : 6);
	pXL->SetXL(szCell, szText);
	nRow += 46;
	
	szCell.Format("O%d", nRow);
	szText = _T(pCalcData->GetStrConstPSfpu());
	pXL->SetXL(szCell, szText);

	szCell.Format("Q%d", nRow);
	szText.Format("%g", pCalcData->GetConstPSfpu());
	pXL->SetXL(szCell, szText);
	nRow += 1;

	szCell.Format("O%d", nRow);
	szText = _T(pCalcData->GetStrConstPSfy());
	pXL->SetXL(szCell, szText);

	szCell.Format("Q%d", nRow);
	szText.Format("%g", pCalcData->GetConstPSfy());
	pXL->SetXL(szCell, szText);
	nRow += 30;

	// (2) 슬래브 중앙부			
	// ①	PC 강재와 Sheath 사이의 마찰에 의한 손실
	ReSetTendonDistance(xyPosArr, 1, ReDisArr);
	nResize = dAngArr.GetSize() - ReDisArr.GetSize();
	dAlpha = 0;
	for (n=0; n<ReDisArr.GetSize(); n++)
		dAlpha += dAngArr.GetAt(n+nResize);

	szCell.Format("G%d", nRow);
	szText.Format("%.5f", dAlpha);
	pXL->SetXL(szCell, szText);

	nRow += 2;
	szCell.Format("G%d", nRow);
	szText.Format("%.3f", ReDisArr.Sum());
	pXL->SetXL(szCell, szText);

	nCnt = 0;
	nRow += 3;
	nRectRow1 = nRow-1;

	for (n=0; n<ReDisArr.GetSize(); n++)
	{
		if (n==0)
		{
			szCell.Format("D%d", nRow);
			szText.Format("%s%s", szPosArr.GetAt(n+nResize+1), _T("x"));
			pXL->SetXL(szCell, szText);
			
			szCell.Format("Z%d", nRow);
			szText= _T("검토위치");
			pXL->SetXL(szCell, szText);
		}
		else
		{
			pXL->InsertCopyRowLine(nRow, nRow, nRow+1);
			
			szCell.Format("D%d", nRow);
			szText.Format("%s%s", szPosArr.GetAt(n+nResize+1), szPosArr.GetAt(n+nResize+0));
			pXL->SetXL(szCell, szText);
			
			szCell.Format("Z%d", nRow);
			szText= (n!=ReDisArr.GetSize()-1)? _T("") : (bLeftFix)? _T("긴장단") : _T("고정단");
			pXL->SetXL(szCell, szText);
		}	
		
		szCell.Format("G%d", nRow);
		szText.Format("%.3f", toM(ReDisArr.GetAt(n)));
		pXL->SetXL(szCell, szText);
		
		szCell.Format("N%d", nRow);
		szText.Format("%.5f", dAngArr.GetAt(n+nResize));
		pXL->SetXL(szCell, szText);		
		nCnt++;
	}
	nRow += nCnt;
	nRectRow2 = nRow-1;

	// table outline
	szRect1.Format("D%d", nRectRow1);
	szRect2.Format("AB%d", nRectRow1);
	pXL->CellOutLine(szRect1, szRect2);
	szRect2.Format("AB%d", nRectRow2);
	pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);
	nRow += 51;
	
	szCell.Format("O%d", nRow);
	szText = pCalcData->GetStrConstPSfpu();
	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow);
	szText.Format("%g", pCalcData->GetConstPSfpu());
	pXL->SetXL(szCell, szText);
	nRow += 1;

	szCell.Format("O%d", nRow);
	szText = pCalcData->GetStrConstPSfy();
	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow);
	szText.Format("%g", pCalcData->GetConstPSfy());
	pXL->SetXL(szCell, szText);

	// (3) 우측 캔틸레버부
	nRow += 26;
	szCell.Format("I%d", nRow);
	szText = (bLeftFix)? _T("긴장단") : _T("고정단");
	pXL->SetXL(szCell, szText);

	ReSetTendonDistance(xyPosArr, 2, ReDisArr);
	nResize = dAngArr.GetSize() - ReDisArr.GetSize();
	dAlpha = 0;
	for (n=0; n<ReDisArr.GetSize(); n++)
		dAlpha += dAngArr.GetAt(n+nResize);

	nRow += 4;
	szCell.Format("G%d", nRow);
	szText.Format("%.5f", dAlpha);
	pXL->SetXL(szCell, szText);

	nRow += 2;
	szCell.Format("G%d", nRow);
	szText.Format("%.3f", ReDisArr.Sum());
	pXL->SetXL(szCell, szText);

	nCnt = 0;
	nRow += 3;
	nRectRow1 = nRow-1;

	for (n=0; n<ReDisArr.GetSize(); n++)
	{
		if (n==0)
		{
			szCell.Format("D%d", nRow);
			szText.Format("%s%s", szPosArr.GetAt(n+nResize+1), _T("x"));
			pXL->SetXL(szCell, szText);
			
			szCell.Format("Z%d", nRow);
			szText= _T("검토위치");
			pXL->SetXL(szCell, szText);
		}
		else
		{
			pXL->InsertCopyRowLine(nRow, nRow, nRow+1);
			
			szCell.Format("D%d", nRow);
			szText.Format("%s%s", szPosArr.GetAt(n+nResize+1), szPosArr.GetAt(n+nResize+0));
			pXL->SetXL(szCell, szText);
			
			szCell.Format("Z%d", nRow);
			szText= (n!=ReDisArr.GetSize()-1)? _T("") : (bLeftFix)? _T("긴장단") : _T("고정단");
			pXL->SetXL(szCell, szText);
		}	
		
		szCell.Format("G%d", nRow);
		szText.Format("%.3f", toM(ReDisArr.GetAt(n)));
		pXL->SetXL(szCell, szText);
		
		szCell.Format("N%d", nRow);
		szText.Format("%.5f", dAngArr.GetAt(n+nResize));
		pXL->SetXL(szCell, szText);		
		nCnt++;
	}
	nRow += nCnt;
	nRectRow2 = nRow-1;

	// table outline
	szRect1.Format("D%d", nRectRow1);
	szRect2.Format("AB%d", nRectRow1);
	pXL->CellOutLine(szRect1, szRect2);
	szRect2.Format("AB%d", nRectRow2);
	pXL->CellOutLine(szRect1, szRect2, 1, 1, 3);

	nRow += 50;

	szCell.Format("O%d", nRow);
	szText = pCalcData->GetStrConstPSfpu();
	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow);
	szText.Format("%g", pCalcData->GetConstPSfpu());
	pXL->SetXL(szCell, szText);
	nRow += 1;

	szCell.Format("O%d", nRow);
	szText = pCalcData->GetStrConstPSfy();
	pXL->SetXL(szCell, szText);
	szCell.Format("Q%d", nRow);
	szText.Format("%g", pCalcData->GetConstPSfy());
	pXL->SetXL(szCell, szText);
}

///< 3.6 바닥판 슬래브 검토
void CXLDesignFloor::TendonFloorStress()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	
	long	nRow	= 2;
	CString szCell	= _T("");
	CString	szText	= _T("");
	
	BOOL bLeftFix = (pDB->m_nTendonFix != 1) ? TRUE : FALSE;	// 고정단이 좌측&지그재그 : TRUE, 우측 : FALSE
	CString szDanLeft = _T("");
	CString szDanRight = _T("");
	if(bLeftFix == TRUE)
	{
		szDanLeft.Format("고정단");
		szDanRight.Format("긴장단");
	}
	else
	{
		szDanLeft.Format("긴장단");
		szDanRight.Format("고정단");
	}

	szCell.Format("L%d", nRow);
	szText.Format("좌측 캔틸레버(%s)", szDanLeft);
	pXL->SetXL(szCell, szText);

	szCell.Format("V%d", nRow);
	szText.Format("우측 캔틸레버(%s)", szDanRight);
	pXL->SetXL(szCell, szText);

	nRow = 19;
	szCell.Format("D%d", nRow);
	szText.Format("좌측 캔틸레버부 (%s)", szDanLeft);
	pXL->SetXL(szCell, szText);

	nRow = 81;
	szCell.Format("D%d", nRow);
	szText.Format("우측 캔틸레버부 (%s)", szDanRight);
	pXL->SetXL(szCell, szText);

	// 응력검토 결과
	nRow = 113;
	szCell.Format("L%d", nRow);
	szText.Format("좌측 캔틸레버 \n(%s)", szDanLeft);
	pXL->SetXL(szCell, szText);

	szCell.Format("T%d", nRow);
	szText.Format("우측 캔틸레버 \n(%s)", szDanRight);
	pXL->SetXL(szCell, szText);

}

CString GetRebar(long nIdx)
{
	if(nIdx == 0)	return _T("13");
	if(nIdx == 1)	return _T("16");
	if(nIdx == 2)	return _T("19");
	if(nIdx == 3)	return _T("22");
	if(nIdx == 4)	return _T("25");
	if(nIdx == 5)	return _T("29");
	if(nIdx == 6)	return _T("32");
	if(nIdx == 7)	return _T("35");
	
	return _T("");
}


///< 3.7 극한하중에 대한 단면검토
void CXLDesignFloor::TendonLimitStressCheck()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	
	long	nRow	= 34;
	CString szCell	= _T("");
	CString	szText	= _T("");

	// 3.7 극한하중에 대한 단면검토
	// 1) 고정단 캔틸레버부
	// (2) 설계 휨 강도
	// ① 부착된 부재의 PS 강재의 응력 (fps)
	szCell.Format("I%d", nRow);
	szText.Format("%.2f", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP);
	pXL->SetXL(szCell, szText);
	
	nRow = 40;
	szCell.Format("J%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_nGenReinTDiaIdx_1Cy);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("L%d", nRow++);
	szText.Format("%.0f", pCalcData->CALC_CANTILEVER_DATA[0].m_dGenReinTCTC);
	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow);
	
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_nGenReinTDiaIdx_2Cy);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("L%d", nRow++);
	szText.Format("%.0f", pCalcData->CALC_CANTILEVER_DATA[0].m_dGenReinTCTC);
	pXL->SetXL(szCell, szText);
	
	// 2) 중앙부 슬래브
	// (2) 설계 휨 강도
	// ① 부착된 부재의 PS 강재의 응력 (fps)
	nRow = 123;
	szCell.Format("I%d", nRow);
	szText.Format("%.2f", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP);
	pXL->SetXL(szCell, szText);
	
	nRow = 129;
	szCell.Format("J%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_nGenReinTDiaIdx_1Cy);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("L%d", nRow++);
	szText.Format("%.0f", pCalcData->CALC_CANTILEVER_DATA[1].m_dGenReinTCTC);
	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow);
	
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[1].m_nGenReinTDiaIdx_2Cy);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("L%d", nRow++);
	szText.Format("%.0f", pCalcData->CALC_CANTILEVER_DATA[1].m_dGenReinTCTC);
	pXL->SetXL(szCell, szText);
	
	// 3) 긴장단 캔틸레버부
	// (2) 설계 휨 강도
	// ① 부착된 부재의 PS 강재의 응력 (fps)
	nRow = 212;
	szCell.Format("I%d", nRow);
	szText.Format("%.2f", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP);
	pXL->SetXL(szCell, szText);
	
	nRow = 218;
	szCell.Format("J%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_nGenReinTDiaIdx_1Cy);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("L%d", nRow++);
	szText.Format("%.0f", pCalcData->CALC_CANTILEVER_DATA[2].m_dGenReinTCTC);
	pXL->SetXL(szCell, szText);
	szCell.Format("J%d", nRow);
	
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[2].m_nGenReinTDiaIdx_2Cy);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("L%d", nRow++);
	szText.Format("%.0f", pCalcData->CALC_CANTILEVER_DATA[2].m_dGenReinTCTC);
	pXL->SetXL(szCell, szText);	
}

///< 3.8 정착구 설계
void CXLDesignFloor::TendonSettlement()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	
	long	nRow	= 5;
	CString szCell	= _T("");
	CString	szText	= _T("");
	
	// 3.8 정착구 설계
	double dSlabThick =  (pDB->m_nTendonFix<1)? pDB->m_dThickSlabRight : pDB->m_dThickSlabLeft;
	double dWidth = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth;
	double dHeight= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight;

	szCell.Format("G%d", nRow);
	szText.Format("%.0f", dSlabThick);
	pXL->SetXL(szCell, szText);

	szCell.Format("I%d", nRow);
	szText.Format("%.0f", dHeight);
	pXL->SetXL(szCell, szText);

	nRow += 4;
	szCell.Format("L%d", nRow);
	szText.Format("%.0f", dWidth);
	pXL->SetXL(szCell, szText);

	// ① 횡방향 파열력에 대한 보강철근량 산정
	// 철근량 검토
	nRow = 30;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2);
	pXL->SetXL(szCell, szText);

	szCell.Format("K%d", nRow++);
	szText.Format("%d", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2);
	pXL->SetXL(szCell, szText);

	// ② 수직방향 파열력에 대한 보강철근량 산정
	// 철근량 검토
	nRow = 41;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2);
	pXL->SetXL(szCell, szText);

	szCell.Format("K%d", nRow++);
	szText.Format("%d", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2);
	pXL->SetXL(szCell, szText);

	// ③ 단부 인장력에 대한 보강철근량 산정
	nRow = 52;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein1);
	pXL->SetXL(szCell, szText);

	szCell.Format("K%d", nRow++);
	szText.Format("%d", pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1);
	pXL->SetXL(szCell, szText);
}

///< 3.9 내부 지점부 주장률 검토
void CXLDesignFloor::TendonInnerTension()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CADeckData			*pDeckData		= pDataManage->GetDeckData();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	CCalcFloor			CalcFloor(pDataManage);

	long	nRow		= 0;
	CString szCell		= _T("");
	CString	szText		= _T("");

	// 3.9 내부 지점부 주장률 검토
	// 사용 철근량 산정
	// 상 면
	nRow = 4;
	szCell.Format("D%d", nRow);
	szText.Format("%.1f", CalcFloor.GetFloorConcAreaCoefficient());
	pXL->SetXL(szCell, szText);

	nRow = 5;
	szCell.Format("L%d", nRow);
	szText.Format("%.4f", CalcFloor.GetFloorJujangStd());
	pXL->SetXL(szCell, szText);

	nRow = 10;
	szCell.Format("H%d", nRow);
	szText = GetRebar(pCalcData->m_ComboInnerJijumSlab1);
	pXL->SetXL(szCell, szText);

	szCell.Format("J%d", nRow++);
	szText.Format("%.0f", pCalcData->m_Ctc1);
	pXL->SetXL(szCell, szText);
		
	// 하 면
	szCell.Format("H%d", nRow);
	szText = GetRebar(pCalcData->m_ComboInnerJijumSlab2);
	pXL->SetXL(szCell, szText);

	szCell.Format("J%d", nRow++);
	szText.Format("%.0f", pCalcData->m_Ctc2);
	pXL->SetXL(szCell, szText);

	// 3.10 바닥판 배력 철근량 산정								
	// ① 좌측 캔틸레버 바닥판의 배력근 산정
	// 사용 철근량 검토(배력근)
	nRow = 40;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_Combo3);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_CtcB);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_Combo3);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_CtcB);
	pXL->SetXL(szCell, szText);
	
	// 단부 사용 철근량 검토
	nRow = 46;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_Combo4);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_CtcB1);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_Combo4);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_CtcB1);
	pXL->SetXL(szCell, szText);

	// ② 중앙부 바닥판의 배력근 산정
	// 사용 철근량 검토(배력근)
	nRow = 62;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_Combo3);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_CtcB);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_Combo3);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_CtcB);
	pXL->SetXL(szCell, szText);

	// 단부 사용 철근량 검토
	nRow = 68;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_Combo4);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_CtcB1);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_Combo4);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[2].m_Rein_CtcB1);
	pXL->SetXL(szCell, szText);


	// ③ 우측 캔틸레버 바닥판의 배력근 산정							
	// 사용 철근량 검토(배력근)
	nRow = 84;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_Combo3);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_CtcB);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_Combo3);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_CtcB);
	pXL->SetXL(szCell, szText);
	
	// 단부 사용 철근량 검토
	nRow = 90;
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_Combo4);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_CtcB1);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("I%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_Combo4);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("K%d", nRow++);
	szText.Format("%0.f", pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_CtcB1);
	pXL->SetXL(szCell, szText);
}

///< 3.11 거더 단부 바닥판의 보강
void CXLDesignFloor::TendonGirderReinforce()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CADeckData			*pDeckData		= pDataManage->GetDeckData();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CXLControl			*pXL			= m_pXL;
	
	long	nRow		= 0;
	CString szCell		= _T("");
	CString	szText		= _T("");

	// 정착위치
	BOOL bLeftFix = (pDB->m_nTendonFix != 1) ? TRUE : FALSE;

	// 3.11 거더 단부 바닥판의 보강	
	// ① 좌측 캔틸레버부 철근보강 (고정단)
	nRow = 3;
	szCell.Format("L%d", nRow);
	szText= (bLeftFix)? _T("(고정단)") : _T("(긴장단)");
	pXL->SetXL(szCell, szText);
	
	// 	Use As
	nRow = 29;
	szCell.Format("H%d", nRow++);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_Combo2);
	pXL->SetXL(szCell, szText);

	szCell.Format("H%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[0].m_Rein_Combo2_2Cy);
	pXL->SetXL(szCell, szText);

	// ② 우측 캔틸레버 철근보강(긴장단)
	nRow = 35;
	szCell.Format("K%d", nRow);
	szText= (bLeftFix)? _T("(긴장단)") : _T("(고정단)");
	pXL->SetXL(szCell, szText);

	// 	Use As
	nRow = 61;
	szCell.Format("H%d", nRow++);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_Combo2);
	pXL->SetXL(szCell, szText);
	
	szCell.Format("H%d", nRow);
	szText = GetRebar(pCalcData->CALC_CANTILEVER_DATA[1].m_Rein_Combo2_2Cy);
	pXL->SetXL(szCell, szText);
}

void CXLDesignFloor::CheckFloorRebarCalc( FloorType eType, CStringArray &ArrDelSheet, BOOL bNewSheet /*= TRUE*/ )
{
	CCalcFloor *pCalcFloor = m_pAPlateOutCalcStd->GetCalcFloor();

	CXLControl *pXL = m_pXL;

	CString sSheetName = _T("");
	switch(eType)
	{
	case eFloor_LeftCantilever:
		sSheetName.Format(_T("철근량 검토(좌측)"));
		break;

	case eFloor_RightCantilever:
		sSheetName.Format(_T("철근량 검토(우측)"));
		break;

	case eFloor_Center:
		sSheetName.Format(_T("철근량 검토(중앙)"));
		break;

	case eFloor_CenterExperience:
		sSheetName.Format(_T("철근량 검토(경험적설계 중앙)"));
		m_nSttRow = 0;
		break;

	case eFloor_MiddleJijum:
		sSheetName.Format(_T("철근량 검토(중간지점부)"));
		m_nSttRow = 0;
		break;
	}

	if( bNewSheet )
	{
		pXL->AddSheet();
		pXL->SetCellWidth(0, 80, 1.6);
		pXL->SetCellHeight(0, 100, 20);
		pXL->SetVerAlign(0, 0, 100, 80, 2);
		pXL->SetFontsAllCell(9, _T("돋움체"), 1, FALSE);
		pXL->SetSheetName(sSheetName);
		pXL->SheetMoveCopy(FALSE, FALSE);
		ArrDelSheet.Add(sSheetName);
	}

	CXLFormatText *pXLText = new CXLFormatText(pXL);
	pXLText->SetAutoDivePage(FALSE);

	pXLText->GoPosition(m_nSttRow, 0);

	CConcBasic *pConcGen = pCalcFloor->MakeConcBasicData(eType, REBAR_PART_GENERAL);
	CConcBasic *pConcEnd = pCalcFloor->MakeConcBasicData(eType, REBAR_PART_END);
	CConcBasic *pConcMid = pCalcFloor->MakeConcBasicData(eType, REBAR_PART_MIDDLE);

	CRectBeam BeamGen(pConcGen);
	CRectBeam BeamEnd(pConcEnd);
	CRectBeam BeamMID(pConcMid);

	switch(eType)
	{
	case eFloor_LeftCantilever:
	case eFloor_RightCantilever:
		BeamGen.MakeOutputDesignExcelFile(pXLText, 0, TRUE, TRUE);
		BeamGen.MakeOutputSupportExcelFile(pXLText, 0, TRUE);
		BeamGen.MakeOutputCrackExcelFile(pXLText, 0, TRUE);
		BeamEnd.MakeOutputEndPartExcelFile(pXLText, 0, TRUE);
		BeamEnd.MakeOutputDeflectionExcelFile(pXLText, 0, TRUE);
		break;

	case eFloor_Center:
		BeamGen.MakeOutputDesignExcelFile(pXLText, 0, TRUE, TRUE);
		BeamGen.MakeOutputSupportExcelFile(pXLText, 0, TRUE);
		BeamGen.MakeOutputCrackExcelFile(pXLText, 0, TRUE);
		BeamEnd.MakeOutputEndPartExcelFile(pXLText, 0, TRUE);
		BeamEnd.MakeOutputDeflectionExcelFile(pXLText, 0, TRUE);

		break;

	case eFloor_CenterExperience:
		BeamGen.MakeOutputExperienceDesignConditionExcelFile(pXLText, 0, TRUE);
		BeamGen.MakeOutputExperienceDesignExcelFile(pXLText, 0, TRUE);
		BeamEnd.MakeOutputExperienceDesignEndPartExcelFile(pXLText, 0, TRUE);

		break;

	case eFloor_MiddleJijum:
		BeamMID.MakeOutputMainLengthRateExcelFile(pXLText, 0, TRUE);
		break;
	}

	delete pXLText;
}
