// XLDesignOfMold.cpp: implementation of the CXLDesignOfMold class.
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


CXLDesignOfMold::CXLDesignOfMold(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_nNumber			= 5;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
	m_strCell[0]  = "A";m_strCell[1]  = "B";m_strCell[2]  = "C";m_strCell[3]  = "D";m_strCell[4]  = "E";m_strCell[5]  = "F";
	m_strCell[6]  = "G";m_strCell[7]  = "H";m_strCell[8]  = "I";m_strCell[9]  = "J";m_strCell[10] = "K";m_strCell[11] = "L";
	m_strCell[12] = "M";m_strCell[13] = "N";m_strCell[14] = "O";m_strCell[15] = "P";m_strCell[16] = "Q";m_strCell[17] = "R";
	m_strCell[18] = "S";m_strCell[19] = "T";m_strCell[20] = "U";m_strCell[21] = "V";m_strCell[22] = "W";m_strCell[23] = "X";
	m_strCell[24] = "Y";m_strCell[25] = "Z";
	m_strCell[26] = "AA";m_strCell[27] = "AB";m_strCell[28] = "AC";m_strCell[29] = "AD";m_strCell[30] = "AE";m_strCell[31] = "AF";
	m_strCell[32] = "AG";m_strCell[33] = "AH";m_strCell[34] = "AI";m_strCell[35] = "AJ";m_strCell[36] = "AK";m_strCell[37] = "AL";
	m_strCell[38] = "AM";m_strCell[39] = "AN";m_strCell[40] = "AO";m_strCell[41] = "AP";m_strCell[42] = "AQ";m_strCell[43] = "AR";
	m_strCell[44] = "AS";m_strCell[45] = "AT";m_strCell[46] = "AU";m_strCell[47] = "AV";m_strCell[48] = "AW";m_strCell[49] = "AX";
}

CXLDesignOfMold::~CXLDesignOfMold()
{

}

void CXLDesignOfMold::CalcEffectWidth()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CDomyun				Dom;

	pOut->ClearCellRef();
	m_nSttRow = 13;

	BOOL	bHapSung	= FALSE;
	if(pBridgeApp->GetNumHaiseokCombo() == 0) bHapSung = TRUE;

	CalcFlangeEffectWidth();///< 플랜지 유효폭

	if(bHapSung)
		CalcSlabEffectWidth();
}

void CXLDesignOfMold::CalcFlangeEffectWidth()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
   	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;	

	CString	sText, sJiganCellRef, sTemp;
	
	double	dDistance		= 0;
	double	dCoef			= 0;
	double	dSharpUp_Cen	= 0;	// 지간별 플랜지 상부 돌출폭(중앙부)
	double	dSharpLo_Cen	= 0;	// 지간별 플랜지 하부 돌출폭(중앙부)
	double	dSharpUp_Jijum	= 0;	// 지간별 플랜지 상부 돌출폭(지점부)
	double	dSharpLo_Jijum	= 0;	// 지간별 플랜지 하부 돌출폭(지점부)
	long	nQtyJigan		= pBridgeApp->m_nQtyJigan;
	long	n = 0;
	for(n = 0; n < nQtyJigan; n++)
	{
		dDistance = toM(pBridgeApp->m_dLengthJigan[n]);
		sJiganCellRef.Format("Length_L%d", n+1);
		if(n>1) pXL->InsertCopyRowLine(m_nSttRow-1, m_nSttRow-1, m_nSttRow);
		sText.Format("L%d =", n+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "C", "D", 0);
		pOut->SetXL(pXL, dDistance, m_nSttRow, "E", "G", 1, RED,   9, FALSE, TA_CENTER, sJiganCellRef);
	}

	if(nQtyJigan == 1)
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+1);
		pXL->DeleteRowLineEnd();
	}

	BOOL bHapSung = pBridgeApp->GetNumHaiseokCombo() == 0 ? TRUE : FALSE;
	double dProgress = (10/nQtyJigan)/(bHapSung?2:1);

	m_nSttRow+=2;
	for(n = 0; n < nQtyJigan; n++)
	{
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.1 유효폭 계산", long(5+dProgress*n));
		if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
		CPlateGirderApp *pGir = pBridgeApp->GetGirder(0);
		double dStaStt = pGir->GetBxOnJijum(n)->GetStation();
		double dStaEnd = pGir->GetBxOnJijum(n+1)->GetStation();
		
		dSharpUp_Cen = toM(pGir->GetWidthOnStation((dStaStt+dStaEnd)/2, TRUE)/2); //- pGir->GetThickFactByStation((dStaStt+dStaEnd)/2, G_W)/2;
		dSharpLo_Cen = toM(pGir->GetWidthOnStation((dStaStt+dStaEnd)/2, FALSE)/2);// - pGir->GetThickFactByStation((dStaStt+dStaEnd)/2, G_W)/2;
		dSharpUp_Jijum = toM(pGir->GetWidthOnStation(dStaEnd, TRUE)/2);// - pGir->GetThickFactByStation(dStaEnd, G_W)/2;
		dSharpLo_Jijum = toM(pGir->GetWidthOnStation(dStaEnd, FALSE)/2); //- pGir->GetThickFactByStation(dStaEnd, G_W)/2;

		if(n == 0 || n == nQtyJigan-1)	dCoef = 0.8;	// 지간 중앙부 양끝단
		else							dCoef = 0.6;	// 지간 중앙부 가운데
		
		if(nQtyJigan == 1)	// 단경간
		{
			m_nSttRow++;
			sText.Format("%lf", dSharpUp_Cen);
			pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 5, RED, 9, FALSE, TA_CENTER, "", 2);
			sText.Format("%lf", dSharpLo_Cen);
			pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 5, RED, 9, FALSE, TA_CENTER, "", 2);

			pXL->DeleteRowLine(m_nSttRow, m_nSttRow+11);
			pXL->DeleteRowLineEnd();
			
			return;
		}

		if(n!=0)
		{
			m_nSttRow++;
			pXL->InsertCopyRowLine(m_nSttRow-24, m_nSttRow-1, m_nSttRow);
		}

		sText.Format("%d. 제 %d 지간 중앙부", n*2+1, n+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "C", "C");
		if(n==0)
		{
			pXL->InsertCopyRowLine(m_nSttRow-3, m_nSttRow-3, m_nSttRow);
			pXL->DeleteRowLineDirect(m_nSttRow+1, m_nSttRow+1);
			
			long nRow1 = m_nSttRow;
			long nRow2 = m_nSttRow+1;
			long nRow3 = m_nSttRow+3;
			sText.Format("=IF(AND(0.05<Q%d, Q%d<0.3),(1.1-2*Q%d)*F%d,IF(Q%d<0.05,F%d,0.15*Q%d))", nRow2,nRow2,nRow2,nRow2,nRow2,nRow2,nRow1);
			pOut->SetXL(pXL, sText, nRow3, "Q", "S", 0, 1, 9, FALSE, TA_CENTER);
		}

		pOut->SetXL(pXL, dCoef, m_nSttRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 1);
		sJiganCellRef.Format("Length_L%d", n+1);
		sText.Format("=%s", pOut->GetCellRef(sJiganCellRef));
		pOut->SetXL(pXL, sText, m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);

		if(n==0)
		{
			long nRowTemp = m_nSttRow+5;
			
			sTemp.Format("=Q%d", m_nSttRow-1);
			pOut->SetXL(pXL, sTemp, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, sTemp, nRowTemp, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);

			long nRow1 = m_nSttRow-1;
			long nRow2 = nRowTemp;
			long nRow3 = nRowTemp+2;
			sText.Format("=IF(AND(0.05<Q%d, Q%d<0.3),(1.1-2*Q%d)*F%d,IF(Q%d<0.05,F%d,0.15*Q%d))", nRow2,nRow2,nRow2,nRow2,nRow2,nRow2,nRow1);
			pOut->SetXL(pXL, sText, nRow3, "Q", "S", 0, 1, 9, FALSE, TA_CENTER);
		}

		sText.Format("%lf", dSharpUp_Cen);
		pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 2, RED, 9, FALSE, TA_CENTER, "", 2);

		sText.Format("λL%d", n+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "D", "D", 3);

		sText.Format("%lf", dSharpLo_Cen);
		pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 5, RED, 9, FALSE, TA_CENTER, "", 2);

		if(n == nQtyJigan-1)
		{
			pXL->DeleteRowLineDirect(13+nQtyJigan, 13+nQtyJigan);
			pXL->DeleteRowLineDirect(m_nSttRow, m_nSttRow+11);
			m_nSttRow--;
			return;
		}

		sText.Format("%d. 제 %d 지간 지점부", n*2+2, n+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "C", "C");
		
		sText.Format("=%s", pOut->GetCellRef(sJiganCellRef));
		pXL->SetXL(m_nSttRow-1, 7, sText);
		sJiganCellRef.Format("Length_L%d", n+2);
		sText.Format("=%s", pOut->GetCellRef(sJiganCellRef));
		pXL->SetXL(m_nSttRow-1, 10, sText);

		m_nSttRow++;

		sText.Format("%lf", dSharpUp_Jijum);
		pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 2, RED, 9, FALSE, TA_CENTER, "", 1);

		sText.Format("λS%d", n+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "D", "D", 3);

		sText.Format("%lf", dSharpLo_Jijum);
		pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 4, RED, 9, FALSE, TA_CENTER, "", 1);
	}
}

void CXLDesignOfMold::CalcSlabEffectWidth()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CFemModelingCalc	FemModelingCalc(pDataManage, pCalcStd);
	CString	sText, sJiganCellRef, sTemp;
	double	dCoef		= 0;
	double	dEsp1		= 0;
	double	dEsp2		= 0;
	double	dBUpper		= 0;
	double  dWidthHunch = 0;	// 상부플팬지 끝단에서 헌치시작부 까지의 거리 pDB->m_dWidthHunch값이지만 플랜지 폭의 변화로 구함.
	long	nQtyJigan	= pDB->m_nQtyJigan;
	long	nGirCopyStt = 0;
	long	nCheckInOut	= 0;

	m_nSttRow += 3;
	CPlateGirderApp		*pGir = NULL;
	CPlateBasicIndex	*pBx  = NULL;
	CPlateBasicIndex	*pBxPreMatch = NULL;

	//외측거더, 내측거더====>거더0, 거더1번
	for(long nG = 0; nG < 2; nG++)
	{
		if(nG==1 && pDB->GetQtyGirder()==2)	break;
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.1 유효폭 계산", 12+nG*2);

		pGir = pDB->GetGirder(nG);	
		nCheckInOut = m_nSttRow;
		if(nG==0)
		{
			sText.Format("< 외측거더 >");
			pOut->SetXL(pXL, sText,						m_nSttRow, "B", "B", NEXT);
			if(pDB->IsTUGir())
				pOut->InsertImage(pXL, "슬래브유효폭_외측_좌_TU", m_nSttRow, "C");
			else
				pOut->InsertImage(pXL, "슬래브유효폭_외측", m_nSttRow, "C");
			SetLines(8);
			nGirCopyStt = m_nSttRow;
		}
		else if(nG==1)
		{
			sText.Format("< 내측거더 >");
			pOut->SetXL(pXL, sText,						m_nSttRow, "B", "B", NEXT);
			if(pDB->IsTUGir())
				pOut->InsertImage(pXL, "슬래브유효폭_내측_TU", m_nSttRow, "E");
			else
				pOut->InsertImage(pXL, "슬래브유효폭_내측", m_nSttRow, "E");
			SetLines(7);
			pXL->InsertCopyRowLine(nGirCopyStt, m_nSttRow-9, m_nSttRow+1);
		}

		for(long n=0; n < nQtyJigan; n++)
		{
			double dStaStt = pGir->GetBxOnJijum(n)->GetStation();
			double dStaEnd = pGir->GetBxOnJijum(n+1)->GetStation();
			
			dBUpper     = pDB->IsTUGir() ? pDB->m_BindConc.m_dWidth : FemModelingCalc.GetEffectWidthFlange(nG, (dStaStt+dStaEnd)/2);//pGir->GetWidthOnStation((dStaStt+dStaEnd)/2, TRUE);
			pBx         = pGir->GetBxByStation((dStaStt+dStaEnd)/2);
			CDPoint A[HDANLINESU];
			pGir->GetHunchXyDom(pBx, A);
			dWidthHunch = ~(A[3]-A[2]);

			if(nG==0)
			{
				dEsp1	= toM(pDB->m_dWidthSlabLeft - dBUpper/2);
				if(pDB->IsTUGir())
					dEsp2 = toM(pGir->GetDisGirderToGirder(pBx) - dBUpper)/2;
				else
					dEsp2 = toM(pGir->GetDisGirderToGirder(pBx) - dBUpper - pDB->m_dThickSlabHunch*2) / 2;
			}	
			else	// 내측거더
			{
				pBxPreMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);	
				dEsp1		= toM(pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch) - dBUpper - pDB->m_dThickSlabHunch*2)/2;
				if(pDB->IsTUGir())
					dEsp2 = toM(pGir->GetDisGirderToGirder(pBx) - dBUpper)/2;
				else
					dEsp2 = toM(pGir->GetDisGirderToGirder(pBx) - dBUpper - pDB->m_dThickSlabHunch*2)/2;
			}

			if(n == 0 || n == nQtyJigan-1)	dCoef = 0.8;	// 지간 중앙부 양끝단
			else							dCoef = 0.6;	// 지간 중앙부 가운데

			if(nG==0 && n!=0)
			{
				if(nQtyJigan==1)
					pXL->InsertCopyRowLine(m_nSttRow-13, m_nSttRow-1, m_nSttRow);
				else
					pXL->InsertCopyRowLine(m_nSttRow-30, m_nSttRow-1, m_nSttRow);
			}

			if(nQtyJigan>1)
			{
				sText.Format("%d. 제 %d 지간 중앙부", n*2+1, n+1);
				pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", 1);

				if(nG==0&&n==0)
				{
					pXL->InsertCopyRowLine(m_nSttRow-2, m_nSttRow-2, m_nSttRow);
					pXL->DeleteRowLineDirect(m_nSttRow+1, m_nSttRow+1);
					sTemp.Format("=Q%d", m_nSttRow);
					long nRowTemp = m_nSttRow+5;
					pOut->SetXL(pXL, sTemp, nRowTemp, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);
				}
				sJiganCellRef.Format("Length_L%d", n+1);
				sText.Format("=%s", pOut->GetCellRef(sJiganCellRef));
				pOut->SetXL(pXL, dCoef, m_nSttRow, "F", "F", 0, RED, 9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, sText, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER);
				m_nSttRow++;
			}
			else
			{
				sJiganCellRef.Format("Length_L%d", n+1);
				sText.Format("=%s", pOut->GetCellRef(sJiganCellRef));
				pOut->SetXL(pXL, "'- 전경간",	m_nSttRow, "C", "C", 1);
				pOut->SetXL(pXL, sText, m_nSttRow, "F", "H", 1, BLACK, 9, FALSE, TA_CENTER);
			}

			pOut->SetXL(pXL, toM(pDB->m_dThickSlabHunch),		m_nSttRow, "F", "H", 1, BLUE,  9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dEsp1,								m_nSttRow, "F", "H", 1, BLUE,  9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dEsp2,								m_nSttRow, "F", "H", 1, BLUE,  9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, toM(dBUpper),						m_nSttRow, "F", "H", 8, BLUE,  9, FALSE, TA_CENTER); 

			sText.Format("=IF(B%d=\"< 외측거더 >\",\"플랜지 폭 + λL%d + λL%d + h\",\"플랜지 폭 + λL%d + λL%d + h×2\")", nCheckInOut, n+1, n+2, n+1, n+2);
			pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 0);
			sText.Format("=IF(B%d=\"< 외측거더 >\",F%d+Q%d+Q%d+F%d,F%d+Q%d+Q%d+F%d*2)", nCheckInOut, m_nSttRow-8, m_nSttRow-5, m_nSttRow-2, m_nSttRow-11, m_nSttRow-8, m_nSttRow-5, m_nSttRow-2, m_nSttRow-11);
			pOut->SetXL(pXL, sText, m_nSttRow, "Q", "S", 1, RED, 9, FALSE, TA_CENTER);

			if(nQtyJigan==1 || n==nQtyJigan-1)
			{
				pXL->DeleteRowLineDirect(m_nSttRow, m_nSttRow+14);
				continue;
			}
			m_nSttRow++;

			dBUpper     = pDB->IsTUGir() ? pDB->m_BindConc.m_dWidth : FemModelingCalc.GetEffectWidthFlange(nG, dStaEnd);//pGir->GetWidthOnStation(dStaEnd, TRUE);
			pBx         = pGir->GetBxByStation(dStaEnd);
			pGir->GetHunchXyDom(pBx, A);
			dWidthHunch = ~(A[3]-A[2]);

			if(nG==0)
			{
				dEsp1	= toM(pDB->m_dWidthSlabLeft - dBUpper/2);
				if(pDB->IsTUGir())
					dEsp2	= toM(pGir->GetDisGirderToGirder(pBx) - dBUpper) / 2;
				else
					dEsp2	= toM(pGir->GetDisGirderToGirder(pBx) - dBUpper - pDB->m_dThickSlabHunch*2) / 2;
			}	
			else	// 내측거더
			{
				pBxPreMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);
				dEsp1		= toM(pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch) - dBUpper - pDB->m_dThickSlabHunch*2)/2;
				if(pDB->IsTUGir())
					dEsp2	= toM(pGir->GetDisGirderToGirder(pBx) - dBUpper)/2;
				else
					dEsp2	= toM(pGir->GetDisGirderToGirder(pBx) - dBUpper - pDB->m_dThickSlabHunch*2)/2;
			}
			
			sText.Format("%d. 제 %d 지간 지점부", n*2+2, n+1);
			pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", 1);

			pOut->SetXL(pXL, toM(pDB->m_dThickSlabHunch),		m_nSttRow, "F", "H", 1, BLUE,  9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dEsp1,								m_nSttRow, "F", "H", 1, BLUE,  9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dEsp2,								m_nSttRow, "F", "H", 1, BLUE,  9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, toM(dBUpper),						m_nSttRow, "F", "H", 1, BLACK,  9, FALSE, TA_CENTER);

			sText.Format("=%s", pOut->GetCellRef(sJiganCellRef));
			pXL->SetXL(m_nSttRow-1, 7, sText);
			sJiganCellRef.Format("Length_L%d", n+2);
			sText.Format("=%s", pOut->GetCellRef(sJiganCellRef));
			pXL->SetXL(m_nSttRow-1, 10, sText);

			m_nSttRow += 8;

			sText.Format("=IF(B%d=\"< 외측거더 >\",\"플랜지 폭 + λS%d + λS%d + h\",\"플랜지 폭 + λS%d + λS%d + h×2\")", nCheckInOut, n+1, n+2, n+1, n+2);
			pOut->SetXL(pXL, sText, m_nSttRow, "F", "F", 0);
			sText.Format("=IF(B%d=\"< 외측거더 >\",F%d+Q%d+Q%d+F%d,F%d+Q%d+Q%d+F%d*2)", nCheckInOut, m_nSttRow-9, m_nSttRow-5, m_nSttRow-2, m_nSttRow-12, m_nSttRow-9, m_nSttRow-5, m_nSttRow-2, m_nSttRow-12);
			pOut->SetXL(pXL, sText, m_nSttRow, "Q", "S", 2, RED, 9, FALSE, TA_CENTER);
		}
		if(nG==0)
		{
			pXL->DeleteRowLineDirect(nGirCopyStt-1, nGirCopyStt-1);
		}
	}
}


///< 단면가정 - 거더
void CXLDesignOfMold::SectionAssumptionGirder()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CGlobarOption       *pGlopt         = pDataManage->GetGlobalOption();

	m_nSttRow			= 3;
	double	dSta        = 0;
	long nDanmyunCheckType = pGlopt->GetDesignDanmyunCheck();	// 0: 외측거더, 내측거더 구분적용  1: 거더구분하지 않음  2: 모든거더 구분적용
	long nCopyStt = 4;
	long nCopyEnd = pDB->IsTUGir() ? 83 : 57;

	CString	sText;

	pOut->SetXL(pXL, pGlopt->GetDesignHeigtBaseUpper() == 1 ? "상판내측" : "상판외측", m_nSttRow, "AB", "AB", 0);
	
	if(nDanmyunCheckType==0)
	{
		pOut->SetXL(pXL, "< 외측거더 >", m_nSttRow, "B", "B");
		m_nSttRow += (nCopyEnd-nCopyStt)+1;
		POSITION posSec = pManage->m_FrameSectionList.GetHeadPosition();
		while(posSec)
		{
			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
			CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);  
			
			if(pFrSec->m_nFlag != POS_GIRDER)		continue;	// 거더가 아닐 경우

			long nGSec = 0;
			if(pModelCalc->GetPositionBySectionName(pFrSec->m_szName, nGSec, dSta, 0))
			{
				pXL->InsertCopyRowLine(nCopyStt, nCopyEnd, m_nSttRow);
				SectionAssumptionGirderSub(nGSec, dSta, pFrSec);
			}
		}
		if(pDB->GetGirdersu() > 2)
		{
			pOut->SetXL(pXL, "< 내측거더 >", m_nSttRow, "B", "B");
			POSITION posSec = pManage->m_FrameSectionList.GetHeadPosition();
			while(posSec)
			{
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);  
			
				if(pFrSec->m_nFlag != POS_GIRDER)		continue;	// 거더가 아닐 경우

				long nGSec = 1;
				if(pModelCalc->GetPositionBySectionName(pFrSec->m_szName, nGSec, dSta, 0))
				{
					pXL->InsertCopyRowLine(nCopyStt, nCopyEnd, m_nSttRow);
					SectionAssumptionGirderSub(nGSec, dSta, pFrSec);
				}
			}
		}
	}
	else if(nDanmyunCheckType==1)
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		m_nSttRow += (nCopyEnd-nCopyStt)+2;
		POSITION posSec = pManage->m_FrameSectionList.GetHeadPosition();
		while(posSec)
		{
			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
			CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);  

			if(pFrSec->m_nFlag > 0)		continue;	// 거더가 아닐 경우

			long nGSec = -1;
			if(pModelCalc->GetPositionBySectionName(pFrSec->m_szName, nGSec, dSta, 0))
			{
				pXL->InsertCopyRowLine(nCopyStt, nCopyEnd, m_nSttRow);
				SectionAssumptionGirderSub(nGSec, dSta, pFrSec);
			}
		}
	}
	else
	{
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			sText.Format("< 거더 %d >", nG+1);	
			pOut->SetXL(pXL, sText, m_nSttRow, "B", "B");
			if(nG==0)
				m_nSttRow += (nCopyEnd-nCopyStt)+1;
			POSITION posSec = pManage->m_FrameSectionList.GetHeadPosition();
			while(posSec)
			{
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);  
				if(pFrSec->m_nFlag > 0)		continue;	// 거더가 아닐 경우
				long nGSec = nG;
				if(pModelCalc->GetPositionBySectionName(pFrSec->m_szName, nGSec, dSta, 0))
				{
					pXL->InsertCopyRowLine(nCopyStt, nCopyEnd, m_nSttRow);
					SectionAssumptionGirderSub(nGSec, dSta, pFrSec);
				}
			}
		}
	}
	pXL->DeleteRowLine(nCopyStt, nCopyEnd);
	pXL->DeleteRowLineEnd();
}


///< 단면가정 - 거더
void CXLDesignOfMold::SectionAssumptionGirderSub(long nG, double dSta, CFrameSection *pFrSec)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText;
	
	pModelCalc->GetSectionCoefficient(nG, dSta, FALSE, dSta, 1);

	double	dNd			= pCalcData->DESIGN_MATERIAL.m_nN;
	double	dNc			= pCalcData->DESIGN_MATERIAL.m_nBindNc;
	double	dWidthSlab  = pModelCalc->GetWidthSlab(nG, dSta);
	double	dSlabT		= pModelCalc->GetMapValueSecCo("SLAB_THICK");
	double	dHunch		= pModelCalc->GetMapValueSecCo("HUNCH");
	double	dUpFlangeW	= pModelCalc->GetMapValueSecCo("FLANGE_W_UP");
	double	dUpFlangeT	= pModelCalc->GetMapValueSecCo("FLANGE_T_UP");
	double	dWebT		= pModelCalc->GetMapValueSecCo("WEB_T");
	double	dWebH		= pModelCalc->GetMapValueSecCo("WEB_H");
	double	dDnFlangeW	= pModelCalc->GetMapValueSecCo("FLANGE_W_DN");
	double	dDnFlangeT	= pModelCalc->GetMapValueSecCo("FLANGE_T_DN");
	double	dBind_W		= pDB->m_BindConc.m_dWidth;
	double	dBind_T		= pDB->m_BindConc.m_dHeight;
	double	dBind_Dist	= pDB->m_BindConc.m_dDeep;


	sText.Format("◎ 단면 - %s", pFrSec->m_szName);
	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 2);

	if(pDB->IsTUGir())
	{
		pOut->InsertImage(pXL, "단면가정_거더_TU", m_nSttRow, "B");
		m_nSttRow--;
		pOut->SetXL(pXL, dWidthSlab,	m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dSlabT,		m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dBind_W,		m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dBind_T,		m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dUpFlangeW,	m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dUpFlangeT,	m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dWebT,			m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dWebH,			m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dDnFlangeW,	m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dDnFlangeT,	m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dBind_Dist,	m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dNc,			m_nSttRow, "W", "Y", 1, RED,  9, FALSE, TA_CENTER, "N",0);
		pOut->SetXL(pXL, dNd,			m_nSttRow, "W", "Y", 68, RED,  9, FALSE, TA_CENTER, "N",0);
	}
	else
	{
		pOut->InsertImage(pXL, "단면가정_거더", m_nSttRow, "B");
		m_nSttRow--;
		pOut->SetXL(pXL, dWidthSlab,	m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dSlabT,		m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dHunch,		m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dUpFlangeW,	m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dUpFlangeT,	m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dWebT,			m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dWebH,			m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dDnFlangeW,	m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dDnFlangeT,	m_nSttRow, "S", "U", 1, RED,  9, FALSE, TA_CENTER, "",0);
		pOut->SetXL(pXL, dNd,			m_nSttRow, "S", "U", 44, RED,  9, FALSE, TA_CENTER, "N",0);
	}
}	

///< 4.2 단면가정 - 가로보
void CXLDesignOfMold::SectionAssumptionCR()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutXL			= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	vector <CSectionJewon> arrSecCr;
	pOut->ClearCellRef();
	pModelCalc->GetCrossBeamFrameSectionArray(&arrSecCr);
	long	nG			= 0;
	long	nIndex		= 1;
	long	nSecSize	= arrSecCr.size();
	double  dSta        = 0;
	CString	sText;

	long nCopySttCr = 1;
	long nCopyEndCr = 25;
	long nCopySttBr = 26;
	long nCopyEndBr = 48;
	m_nSttRow		= nCopyEndBr+1;

	for(long nSec=0; nSec<nSecSize; nSec++)
	{
		if(pOutXL->m_bAbort)	return;
		nG		= arrSecCr[nSec].m_pBx->GetNumberGirder();
		dSta	= arrSecCr[nSec].m_pBx->GetStation();
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta, BX_CROSSBEAM_VBRACING);
		CSection			*pSec	= pBx->GetSection();
		CVBracing			*pVBr	= pSec->GetVBracing();
		CPlateCrossBeam		*pC		= pSec->GetCrossBeam();
		long				nSecType= pSec->GetenDetType();

		if(pVBr && nSecType==SECDET_VBRACING)
		{	
			pXL->InsertCopyRowLine(nCopySttBr, nCopyEndBr, m_nSttRow);
			sText.Format("◎ 가로보 - %d (수직 브레이싱 타입 )", nIndex, nG+1, dSta);
			pOut->SetXL(pXL, sText, m_nSttRow, "A", "A", 0);

			sText.Format("< 거더 : %d    STA : %.1f >", nG+1, dSta);
			pOut->SetXL(pXL, sText, m_nSttRow, "M", "M", 2);

			TypeVBracing(pBx, pVBr);
			nIndex++;
			m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.2 단면가정-가로보", m_pAPlateOutCalcStd->GetAPlateOutExcel()->GetProgress() + 1);
		}
		else if(pC && nSecType==SECDET_CROSSBEAM)
		{
			if(pC->GetType()==CR_TYPE_HSTEEL)			
				sText.Format("◎ 가로보 - %d (형강 타입-일체형)", nIndex);
			else if(pC->GetType()==CR_TYPE_MANUFACTURE)
				sText.Format("◎ 가로보 - %d (공장제작 타입)", nIndex);
			else
				sText.Format("◎ 가로보 - %d (수직보강재 연결 타입)", nIndex);

			pXL->InsertCopyRowLine(nCopySttCr, nCopyEndCr, m_nSttRow);				
			pOut->SetXL(pXL, sText, m_nSttRow, "A", "A", 0);
			sText.Format("< 거더 : %d    STA : %.1f >", nG+1, dSta);
			pOut->SetXL(pXL, sText, m_nSttRow, "Q", "Q", 1);				

			TypeCrossBeam(pBx, pC);
			nIndex++;
			m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.2 단면가정-가로보", m_pAPlateOutCalcStd->GetAPlateOutExcel()->GetProgress() + 1);
		}
	}
	pXL->DeleteRowLine(nCopySttCr, nCopyEndBr);
	pXL->DeleteRowLineEnd();

	if(m_nSttRow == nCopyEndBr+1)	// 출력할 가로보가 하나도 없을경우
		pXL->SheetDelete();
}

///< 가로보(H형강/I형강/공장제작/수직보강재) 형태일경우
void CXLDesignOfMold::TypeCrossBeam(CPlateBasicIndex *pBx, CPlateCrossBeam *pC)
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	double	dUpFlangeW	= 0;
	double	dUpFlangeT	= 0;
	double	dDnFlangeW	= 0;
	double	dDnFlangeT	= 0;
	double	dWebH		= 0;
	double	dWebT		= 0;

	dUpFlangeW	= pC->m_uM_CR*2;
	dUpFlangeT	= pC->m_T2_CR;
	dDnFlangeW	= pC->m_dM_CR*2;
	dDnFlangeT	= pC->m_T1_CR;
	dWebH		= pC->m_H_CR;
	dWebT		= pC->m_T3_CR;

	pOut->InsertImage(pXL, "가로보제원", m_nSttRow, "E");
	pOut->SetXL(pXL, dUpFlangeW,	m_nSttRow, "W", "Y", 1,  RED,  9, FALSE, TA_CENTER,"",0);
	pOut->SetXL(pXL, dDnFlangeW,	m_nSttRow, "W", "Y", 1,  RED,  9, FALSE, TA_CENTER,"",0);
	pOut->SetXL(pXL, dUpFlangeT,	m_nSttRow, "W", "Y", 1,  RED,  9, FALSE, TA_CENTER,"",0);
	pOut->SetXL(pXL, dDnFlangeT,	m_nSttRow, "W", "Y", 1,  RED,  9, FALSE, TA_CENTER,"",0);
	pOut->SetXL(pXL, dWebH,			m_nSttRow, "W", "Y", 1,  RED,  9, FALSE, TA_CENTER,"",0);
	pOut->SetXL(pXL, dWebT,			m_nSttRow, "W", "Y", 19, RED,  9, FALSE, TA_CENTER,"",0);
}

///< 수직브레이싱 타입
void CXLDesignOfMold::TypeVBracing(CPlateBasicIndex *pBx, CVBracing *pVBr)
{
	CPlateGirderApp		*pGir			= pBx->GetGirder();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText, sSpecUp, sSpecLo, sSpecC;

	CSteelMaterialData	*pSteelUp	= pVBr->GetSteelMaterialData(STEELMAT_UPPER);
	CSteelMaterialData	*pSteelLo	= pVBr->GetSteelMaterialData(STEELMAT_LOWER);
	CSteelMaterialData	*pSteelC	= pVBr->GetSteelMaterialData(STEELMAT_DIAGONAL);
	
	long nTypeUp		= pSteelUp->m_nType;
	long nTypeLo		= pSteelLo->m_nType;
	long nTypeC			= pSteelC->m_nType;

	double	dUp_B		= pSteelUp->m_dB;
	double	dUp_H		= pSteelUp->m_dH;
	double	dUp_T		= pSteelUp->m_dT1;
	double	dUp_T2		= pSteelUp->m_dT2;
	double	dDn_B		= pSteelLo->m_dB;
	double	dDn_H		= pSteelLo->m_dH;
	double	dDn_T		= pSteelLo->m_dT1;
	double	dDn_T2		= pSteelLo->m_dT2;
	double	dC_B		= pSteelC->m_dB;
	double	dC_H		= pSteelC->m_dH;
	double	dC_T		= pSteelC->m_dT1;
	double	dC_T2		= pSteelC->m_dT2;
	
	if (nTypeUp == CSteelMaterialData::CTSTEEL)	
		sSpecUp.Format("CT - %.1f x %.0f x %.1f x %.0f", dUp_H, dUp_B, dUp_T, dUp_T2);
	else if(nTypeUp == CSteelMaterialData::CSTEEL)
		sSpecUp.Format("ㄷ - %.0f x %.0f x %.1f x %.0f", dUp_H, dUp_B, dUp_T, dUp_T2);
	else
		sSpecUp.Format("L - %.0f x %.0f x %.0f", dUp_H, dUp_B, dUp_T);

	if (nTypeLo == CSteelMaterialData::CTSTEEL)	
		sSpecLo.Format("CT - %.1f x %.0f x %.1f x %.0f", dDn_H, dDn_B, dDn_T, dDn_T2);
	else if(nTypeLo == CSteelMaterialData::CSTEEL)
		sSpecLo.Format("ㄷ - %.0f x %.0f x %.1f x %.0f", dDn_H, dDn_B, dDn_T, dDn_T2);
	else
		sSpecLo.Format("L - %.0f x %.0f x %.0f", dDn_H, dDn_B, dDn_T);

	if (nTypeC == CSteelMaterialData::CTSTEEL)	
		sSpecC.Format("CT - %.1f x %.0f x %.1f x %.0f", dC_H, dC_B, dC_T, dC_T2);
	else if(nTypeC == CSteelMaterialData::CSTEEL)
		sSpecC.Format("ㄷ - %.0f x %.0f x %.1f x %.0f", dC_H, dC_B, dC_T, dC_T2);
	else
		sSpecC.Format("L - %.0f x %.0f x %.0f", dC_H, dC_B, dC_T);

	double	dUp_GammaX	= frCm(pSteelUp->m_dRadiusOfGyration.x);
	double	dDn_GammaX	= frCm(pSteelLo->m_dRadiusOfGyration.x);
	double	dC_GammaX	= frCm(pSteelC->m_dRadiusOfGyration.x);

	double dUp_GammaMin	= frCm(MIN(pSteelUp->m_dRadiusOfGyrationMaxMin.x, pSteelUp->m_dRadiusOfGyrationMaxMin.y));
	double dDn_GammaMin	= frCm(MIN(pSteelLo->m_dRadiusOfGyrationMaxMin.x, pSteelLo->m_dRadiusOfGyrationMaxMin.y));
	double dC_GammaMin	= frCm(MIN(pSteelC->m_dRadiusOfGyrationMaxMin.x, pSteelC->m_dRadiusOfGyrationMaxMin.y));

	double	dAsUp		= frCm2(pSteelUp->m_dSectionArea);
	double	dAsDn		= frCm2(pSteelLo->m_dSectionArea);
	double	dAsSd		= frCm2(pSteelC->m_dSectionArea);
	double	dH				= (pVBr->GetLengthFrameLine(4));
	double	dA		= (ABS(pGir->m_dGirderCenter-pGir->m_pRight->m_dGirderCenter));

	if(nTypeUp==CSteelMaterialData::CTSTEEL || nTypeUp==CSteelMaterialData::CSTEEL)
		dUp_GammaMin	= frCm(MIN(pSteelUp->m_dRadiusOfGyration.x, pSteelUp->m_dRadiusOfGyration.y));
	if(nTypeLo==CSteelMaterialData::CTSTEEL || nTypeUp==CSteelMaterialData::CSTEEL)
		dDn_GammaMin	= frCm(MIN(pSteelLo->m_dRadiusOfGyration.x, pSteelLo->m_dRadiusOfGyration.y));
	if(nTypeC==CSteelMaterialData::CTSTEEL || nTypeUp==CSteelMaterialData::CSTEEL)
		dC_GammaMin	= frCm(MIN(pSteelC->m_dRadiusOfGyration.x, pSteelC->m_dRadiusOfGyration.y));

	if(pVBr->GetType()==TYPE_INVVBRACING)
		pOut->InsertImage(pXL, "단면가정_브레이싱역V형", m_nSttRow, "B");
	else if(pVBr->GetType()==TYPE_VBRACING)
		pOut->InsertImage(pXL, "단면가정_브레이싱V형", m_nSttRow, "B");
	m_nSttRow--;
		
	pOut->SetXL(pXL, dH,			m_nSttRow, "P", "R", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dA,			m_nSttRow, "P", "R", 7, RED, 9, FALSE, TA_CENTER, "", 0);

	pOut->SetXL(pXL, sSpecUp,		m_nSttRow, "D", "K", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAsUp,			m_nSttRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, dUp_GammaMin,	m_nSttRow, "O", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, dUp_GammaX,	m_nSttRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 2);

	pOut->SetXL(pXL, sSpecC,		m_nSttRow, "D", "K", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAsSd,			m_nSttRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, dC_GammaMin,	m_nSttRow, "O", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, dC_GammaX,		m_nSttRow, "R", "T", 1, RED, 9, FALSE, TA_CENTER, "", 2);

	pOut->SetXL(pXL, sSpecLo,		m_nSttRow, "D", "K", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAsDn,			m_nSttRow, "L", "N", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, dDn_GammaMin,	m_nSttRow, "O", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, dDn_GammaX,	m_nSttRow, "R", "T", 5, RED, 9, FALSE, TA_CENTER, "", 2);

	if(dAsUp==dAsDn)
		m_nSttRow+=3;

	pXL->DeleteRowLine(m_nSttRow, m_nSttRow+2);
	pXL->DeleteRowLineEnd();

	if(dAsUp==dAsDn)	m_nSttRow++;
	else				m_nSttRow+=4;
}

///< 단면가정 - 세로보
void CXLDesignOfMold::SectionAssumptionST()
{

}

void CXLDesignOfMold::DrawHLoad(CDomyun *pDom, CFEMManage *pFEM, long nLoadCase)
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();	

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetScaleDim(0.1);
	Dom.SetDimVLen(Dom.GetDimVLen()/2);

	CForceDiagram ForceDom(pFEM);
	ForceDom.SetScaleDim(0.1);
	ForceDom.SetTextHeight(3);
	ForceDom.DrawFrameModel(TRUE, FALSE, TRUE);
	ForceDom.DrawLoadCase(nLoadCase, FALSE, TRUE, 0.02, FALSE, 1);
//	long nLC = pFEM->GetLoadCaseByString("GUARD");
//	if(nLoadCase==1)	// 합성후 고정하중일때는 방호벽하중도 함께 출력함
//		ForceDom.DrawLoadCase(nLC, FALSE, TRUE, 0.02, FALSE, 1);
	Dom << ForceDom;
	//////////////////////////////////////////////////////////////////////////
	Dom.SetDirection("BOTTOM");
	
	double	dSum		= 0;
	for(long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CJoint		*pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		CJoint		*pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);
		
		double dX	  =  pJ_J->m_vPoint.x-pJ_I->m_vPoint.x ;
		double dXHalf = -dX/2;
		
		if(pJ_I->m_UX==1 || pJ_I->m_UY==1 || pJ_I->m_UZ==1)
		{
			CDPointArray ptArr;
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x, pJ_I->m_vPoint.y));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x-Dom.Always(1.5), pJ_I->m_vPoint.y-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x+Dom.Always(1.5), pJ_I->m_vPoint.y-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x, pJ_I->m_vPoint.y));
			Dom.HatchingByCDPointArray(ptArr, 45, Dom.Always(1), TRUE);
		}
		
		if(pJ_J->m_UX==1 || pJ_J->m_UY==1 || pJ_J->m_UZ==1)
		{
			CDPointArray ptArr;
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x, pJ_J->m_vPoint.y));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x-Dom.Always(1.5), pJ_J->m_vPoint.y-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x+Dom.Always(1.5), pJ_J->m_vPoint.y-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x, pJ_J->m_vPoint.y));
			Dom.HatchingByCDPointArray(ptArr, 45, Dom.Always(1), TRUE);
		}
		
		Dom.DimMoveTo(dSum, pJ_I->m_vPoint.y,0);
		dSum += dX;
		Dom.DimLineTo(dX,0,COMMA(frM(dX)));		
		Dom.TextCircle(dSum+dXHalf,-(pJ_I->m_vPoint.y+Dom.Always(5)),COMMA(nElem+1), 10);
	}
	//////////////////////////////////////////////////////////////////////////	
	
	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	Dom.SetCalcExtRect();
	CDRect rectDom = Dom.GetExtRect();
	rectDom.bottom	+= ForceDom.GetTextHeight()*2;
	rectDom.left	-= ForceDom.Always(4);
	rectDom.right	+= ForceDom.Always(4);
	Dom.Rectangle(rectDom);

	*pDom << Dom;
}

void CXLDesignOfMold::WriteLoadForce(CFEMManage *pFEM, long nLoadCase)
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	pOut->SetLineBoxText(pXL, "하중종류",	m_nSttRow, "C", "G", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "H", "K", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "재하부재",	m_nSttRow, "L", "M", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "N", "Q", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "산 식",		m_nSttRow, "R", "U", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",			m_nSttRow, "V", "Y", 2, BLACK, 9, FALSE, TA_CENTER);		
	pOut->SetXL(pXL, "누가거리",			m_nSttRow, "H", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "이격거리",			m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "하중크기",			m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(m)",					m_nSttRow, "H", "K", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(m)",					m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "(kN/m)",				m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER);

	double dist1 = 0;
	double dist2 = 0;
	CVector vForce, vPos, vForce2, vPosStt, vPosEnd;
	CString	strName, sText;
	POSITION pos = pFEM->m_ElementList.GetHeadPosition();

	CString szLoad = _T("");

	for(long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		CElement	*pE			= pFEM->GetElement((unsigned short)nElem);
		CJoint		*pJ_I		= pFEM->GetJoint(pE->m_nJStt);
		CJoint		*pJ_J		= pFEM->GetJoint(pE->m_nJEnd);
		CLoadForce	*pLoadForce	= pE->GetLoadForce(nLoadCase);
		if(!pLoadForce)	return;

		if(pLoadForce->IsFlag(EXIST_DISTFORCE))
		{
			for(long nLoad=0; nLoad<pLoadForce->m_PointForceArr.GetSize(); nLoad++)
			{
				CLoadForcePoint	*pPoint	= pLoadForce->m_PointForceArr[nLoad];
				CString szLoadName = pPoint->m_strName;

				dist1	= pPoint->m_dDist;
				vForce	= pPoint->m_vForce;														// 이격거리
				strName = pPoint->m_strName;													// 하중종류
				vPosStt = pJ_I->m_vPoint + (pJ_J->m_vPoint - pJ_I->m_vPoint).Unit() * dist1;	// 누가거리

				pOut->SetLineBoxText(pXL, strName,				m_nSttRow, "C", "G", 1, BLACK, 9, FALSE, TA_CENTER);			// 하중종류
				pOut->SetLineBoxText(pXL, vPosStt.x,			m_nSttRow, "H", "K", 1, BLACK, 9, FALSE, TA_CENTER);			// 누가거리
				pOut->SetLineBoxText(pXL, pE->m_nIndex+1,		m_nSttRow, "L", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);		// 재하부재
				pOut->SetLineBoxText(pXL, dist1,				m_nSttRow, "N", "Q", 1, BLACK, 9, FALSE, TA_CENTER);			// 이격거리
				pOut->SetLineBoxText(pXL, "-",					m_nSttRow, "R", "U", 1, BLACK, 9, FALSE, TA_CENTER);			// 산식
				pOut->SetLineBoxText(pXL, -vForce.z,			m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER);			// 하중크기
				m_nSttRow++;
			}
		}

		if(pLoadForce->IsFlag(EXIST_TRAP))
		{
			for(long nLoad=0; nLoad<pLoadForce->m_TrapForceArr.GetSize(); nLoad++)
			{
				CLoadForceTrap	*pTrap	= pLoadForce->m_TrapForceArr[nLoad];
				CString szLoadName = pTrap->m_strName;

				dist1	= pTrap->m_dRStt;
				dist2	= pTrap->m_dREnd;
				vForce	= pTrap->m_vTrapStt;
				vForce2 = pTrap->m_vTrapEnd;
				strName = pTrap->m_strName;
				vPosStt = pJ_I->m_vPoint + (pJ_J->m_vPoint - pJ_I->m_vPoint).Unit() * dist1;		// 누가거리1
				vPosEnd = pJ_I->m_vPoint + (pJ_J->m_vPoint - pJ_I->m_vPoint).Unit() * dist2;		// 누가거리2

				pOut->SetLineBoxText(pXL, strName,				m_nSttRow, "C", "G", 1, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("%.3f ~ %.3f", vPosStt.x, vPosEnd.x);
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "H", "K", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, pE->m_nIndex+1,		m_nSttRow, "L", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%.3f ~ %.3f", dist1, dist2);
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "N", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, "-",					m_nSttRow, "R", "U", 1, BLACK, 9, FALSE, TA_CENTER);			// 산식
//				if(pTrap->m_strName == "포장하중")				sText.Format("%.3f × %.3f", toM(pDB->m_dThickPave), dUnitPave);	
//				else if(pTrap->m_strName == "보도하중")			sText.Format("%.3f × %.3f", pCalcData->DESIGN_FLOOR_DATA[0].m_dWalkLoadH, dUnitWalk);	
//				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "R", "U", 1, BLACK, 9, FALSE, TA_CENTER);
//				if(pTrap->m_strName == "포장하중")				sText.Format("%.3f", -vForce.z);
//				else
					sText.Format("%.3f ~ %.3f", -vForce.z, -vForce2.z);
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
				m_nSttRow++;
			}
		}
	}
}

///< 4.3 작용하중의 산정
void CXLDesignOfMold::ComputeHLoad()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetHLLFemData(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();	
	CXLControl			*pXL			= m_pXL;
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();

	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	CString sText, szProgress=_T("4.3 작용하중의 산정 - 횡방향 하중분배");
	pOut->ClearCellRef();
	m_nSttRow			= 48;
	BOOL	bHapSung	= FALSE;
	long	nNum		= 1, nNumMid = 1;
	if(pDB->GetNumHaiseokCombo() == 0)	bHapSung = TRUE;
	if(pCalcStd->GetSizeHLLFemData()==0)	return;

	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);

	if(bHapSung)	sText.Format("%d) 합성전 고정하중(슬래브 자중)", nNum);
	else			sText.Format("%d) 1차 고정하중(슬래브 자중)", nNum);
	nNum++;

	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1);
	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(szProgress, 31);

	BOOL bAll = pOpt->m_nCalcDocSub[2] == 1 ? TRUE : FALSE;
	long ixHLLStt = bAll ? 0 : pCalcData->m_CurLiveHL;
	long ixHLLEnd = bAll ? pCalcStd->GetSizeHLLFemData()-1 : pCalcData->m_CurLiveHL;

	for(long ixHLL = ixHLLStt; ixHLL <= ixHLLEnd; ++ixHLL)
	{
		CFEMManage *pFEM = pCalcStd->GetHLLFemData(ixHLL);
		if(bAll)
		{
			double dStation = pCalcData->m_dStationHLLPos[ixHLL];
			sText.Format("(%d) 슬래브자중(%s)", nNumMid++, GetStationForStringOut(frM(dStation),3,TRUE,TRUE));
			pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", 1);
		}

		DrawHLoad(&Dom, pFEM, 0);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 2, 4, 27, 30, TRUE, TRUE, TRUE);
		SetLines(2);

		double dUnitConc = tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun);	
		double dist1 = 0, dist2 = 0, dLoadMember = 0, dHeight = 0;
		CVector vValStt, vValEnd, vPos1, vPos2;

		pOut->SetLineBoxText(pXL, "하중 NO.",	m_nSttRow, "C", "F", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",			m_nSttRow, "G", "I", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "재하부재",	m_nSttRow, "J", "L", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",			m_nSttRow, "M", "O", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "산 식",		m_nSttRow, "P", "V", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",			m_nSttRow, "W", "Y", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "누가거리",			m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "이격거리",			m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "하중크기",			m_nSttRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(m)",					m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(m)",					m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(kN/m)",				m_nSttRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER);

		long nRow = 0;
		POSITION pos = pFEM->m_ElementList.GetHeadPosition();
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(szProgress, 32);
		while(pos)
		{
			CElement	*pE		= (CElement*)pFEM->m_ElementList.GetNext(pos);
			CJoint		*pJ_I	= pFEM->GetJoint(pE->m_nJStt);
			CJoint		*pJ_J	= pFEM->GetJoint(pE->m_nJEnd);
			CLoadForce *pLoadForce = pE->m_LoadForceArray.GetAt(0);//합성전사하중
			if(pLoadForce->IsFlag(EXIST_TRAP))
			{
				for(long n=0;n<pLoadForce->m_TrapForceArr.GetSize(); n++)
				{
					dist1		= pLoadForce->m_TrapForceArr[n]->m_dRStt;
					dist2		= pLoadForce->m_TrapForceArr[n]->m_dREnd;
					vValStt		= pLoadForce->m_TrapForceArr[n]->m_vTrapStt;
					vValEnd		= pLoadForce->m_TrapForceArr[n]->m_vTrapEnd;
					vPos1		= pJ_I->m_vPoint + (pJ_J->m_vPoint - pJ_I->m_vPoint).Unit() * (dist1);
					vPos2		= pJ_I->m_vPoint + (pJ_J->m_vPoint - pJ_I->m_vPoint).Unit() * (dist2);
					dLoadMember	= double(pE->m_nIndex+1);									// 재하부재

					dHeight		= -vValStt.z/dUnitConc;

					sText.Format("W%d", nRow+1);
					pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "C", "F", 1, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetLineBoxText(pXL, vPos1.x,			m_nSttRow, "G", "I", 1, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetLineBoxText(pXL, dLoadMember,		m_nSttRow, "J", "L", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
					pOut->SetLineBoxText(pXL, dist1,			m_nSttRow, "M", "O", 1, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetLineBoxText(pXL, "",				m_nSttRow, "P", "V", 1, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, dHeight,					m_nSttRow, "P", "R", 0, RED,  9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "×",						m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, dUnitConc,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetLineBoxText(pXL, -vValStt.z,		m_nSttRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER);

					nRow++;
					m_nSttRow++;

					if(nRow == pFEM->GetTrapForceSize())//마지막 요소에서 force끝부분을 포함
					{
						dHeight = -vValEnd.z/dUnitConc;
						sText.Format("W%d", nRow+1);
						pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "C", "F", 1, BLACK, 9, FALSE, TA_CENTER);
						pOut->SetLineBoxText(pXL, vPos2.x,		m_nSttRow, "G", "I", 1, BLACK, 9, FALSE, TA_CENTER);
						pOut->SetLineBoxText(pXL, dLoadMember,	m_nSttRow, "J", "L", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
						pOut->SetLineBoxText(pXL, dist2,		m_nSttRow, "M", "O", 1, BLACK, 9, FALSE, TA_CENTER);
						pOut->SetLineBoxText(pXL, "",			m_nSttRow, "P", "V", 1, BLACK, 9, FALSE, TA_CENTER);
						pOut->SetXL(pXL, dHeight,				m_nSttRow, "P", "R", 0, RED,  9, FALSE, TA_CENTER);
						pOut->SetXL(pXL, "×",					m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
						pOut->SetXL(pXL, dUnitConc,				m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER);
						pOut->SetLineBoxText(pXL, -vValEnd.z,	m_nSttRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER);

						SetLines(2);
					}
				}
			}
		}
	}	

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(szProgress, 34);

	nNumMid = 1;
	for(long ixHLL = ixHLLStt; ixHLL <= ixHLLEnd; ++ixHLL)
	{
		CFEMManage *pFEM = pCalcStd->GetHLLFemData(ixHLL);
		
		long	nLoadCaseIndex	= pFEM->GetLoadCaseIndex("BEF_TU");
		if(nLoadCaseIndex>0)
		{
			sText.Format("%d) 합성전 고정하중 (구속콘크리트 자중)", nNum++);
			pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1);
			if(bAll)
			{
				double dStation = pCalcData->m_dStationHLLPos[ixHLL];
				sText.Format("(%d) 구속콘크리트 자중(%s)", nNumMid++, GetStationForStringOut(frM(dStation),3,TRUE,TRUE));
				pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", 1);
			}
			Dom.ClearEtt(TRUE);
			DrawHLoad(&Dom, pFEM, nLoadCaseIndex);
			pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 2, 4, 27, 30, TRUE, TRUE, TRUE);
			SetLines(2);

			WriteLoadForce(pFEM, nLoadCaseIndex);
			SetLines(NEXT);
		}
	}
	
	SetLines(NEXT);

	if(bHapSung)	sText.Format("%d) 합성후 고정하중", nNum++);
	else			sText.Format("%d) 2차 고정하중", nNum++);
	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1);
	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.3 작용하중의 산정 - 횡방향 하중분배", 34+1*3);

	long	nMaxLoadCaseSize	= 8;
	long	nIdx				= 0;
	CString	szLoadCase	= _T("");
	CString	szTitle		= _T("");
	long	nLoadCase	= 0;
	CString szSta = _T("");

	nNumMid = 1;
	for(long ixHLL = ixHLLStt; ixHLL <= ixHLLEnd; ++ixHLL)
	{
		CFEMManage *pFEM = pCalcStd->GetHLLFemData(ixHLL);
		if(bAll)
		{
			double dStation = pCalcData->m_dStationHLLPos[ixHLL];
			szSta.Format(_T("(%s)"), GetStationForStringOut(frM(dStation),3,TRUE,TRUE));
		}
		for(nLoadCase=0; nLoadCase<nMaxLoadCaseSize; nLoadCase++)
		{
			switch(nLoadCase)
			{
			case 0 : szLoadCase	= _T("PAVE");	szTitle	= _T("포장 하중");		break;
			case 1 : szLoadCase = _T("GUARD");	szTitle	= _T("방호벽 하중");	break;
			case 2 : szLoadCase = _T("WALK");	szTitle	= _T("보도 하중");		break;
			case 3 : szLoadCase = _T("SOUNDL");	szTitle	= _T("방음벽(좌) 하중");break;
			case 4 : szLoadCase = _T("SOUNDR");	szTitle	= _T("방음벽(우) 하중");break;
			case 5 : szLoadCase = _T("HANDL");	szTitle	= _T("난간(좌) 하중");	break;
			case 6 : szLoadCase = _T("HANDR");	szTitle	= _T("난간(우) 하중");	break;
			case 7 : szLoadCase = _T("EXTRA");	szTitle	= _T("기타 하중");		break;
			}
			long nLoadCaseIndex = pFEM->GetLoadCaseIndex(szLoadCase);
			if(nLoadCaseIndex<0)	continue;

			sText.Format("(%d) %s%s", nIdx+1, szTitle, szSta);
			pOut->SetXL(pXL, sText,	m_nSttRow, "C", "C", 1);
			Dom.ClearEtt(TRUE);
			DrawHLoad(&Dom, pFEM, nLoadCaseIndex);
			pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 2, 4, 27, 30, TRUE, TRUE, TRUE);
			SetLines(2);

			WriteLoadForce(pFEM, nLoadCaseIndex);
			SetLines(NEXT);
			nIdx++;
		}
	}
	

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(szProgress, 42);

	SetLines(1);
	sText.Format("%d) 활 하 중", nNum);	nNum++;
	
	pOut->SetXL(pXL, sText,		m_nSttRow, "B", "B", 1);

	sText.Format("구분");
	pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "C", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, WHITE);
	
	sText.Format("연석간의 교폭");
	pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, WHITE);

	sText.Format("설계 차로수");
	pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, WHITE);

	sText.Format("설계 차로폭");
	pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "N", "Q", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, WHITE);

	SetLines(1);

	long	nHDanNode	=	pDB->GetQtyHDanNode();
	long	nHDanType	=	0;
	double	dLenHDanDom	=	0.0;
	double	dEndHDan	=	0.0;
	double	dSttRoadway	=	0.0;
	double	dEndRoadway	=	0.0;
	double	dSttHDan	=	0.0;
	double	dEffWidth	=	0.0;
	long	nQtyLane	=	(long)0.0;
	long	nHDanTmp	=	1;
	BOOL	bStt		=	FALSE;
	
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	double				dStaStt		= pGir->GetStationStt();
	CPlateBasicIndex	*pBx		= pGir->GetBxByStation(dStaStt, BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);

	for(long nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBxMatch, nHDan));
		dEndHDan	+= dLenHDanDom;

		if(!bStt && nHDanType==HT_NONE)
		{
			dSttRoadway	= dSttHDan;
			bStt		= TRUE;
		}
		if((bStt && nHDanType!=HT_NONE) || (nHDan==nHDanNode-1 && nHDanType==HT_NONE))
		{
			if(nHDanType==HT_NONE)	dEndRoadway	= dEndHDan;
			else					dEndRoadway	= dEndHDan-dLenHDanDom;
			dEffWidth	= fabs(dEndRoadway-dSttRoadway);
			nQtyLane	= CalcGeneral.GetQtyLane(dEffWidth);
			bStt		= FALSE;
			
			sText.Format("횡단 %d", nHDanTmp);
			pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "C", "E", 1, RED, 9, FALSE, TA_CENTER, "", 0, TRUE, WHITE);
			sText.Format("%g",dEffWidth);
			pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "F", "I", 1, RED, 9, FALSE, TA_CENTER, "", 3, TRUE, WHITE);
			sText.Format("%d",nQtyLane);
			pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "J", "M", 1, RED, 9, FALSE, TA_CENTER, "", 0, TRUE, WHITE);			
			sText.Format("=MIN(F%d/J%d,3.6)",m_nSttRow,m_nSttRow);			
			pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "N", "Q", 1, RED, 9, FALSE, TA_CENTER, "", 3, TRUE, WHITE);
			SetLines(1);
			nHDanTmp++;
		}

		dSttHDan = dEndHDan;
	}

	if(pCalcData->m_nHLiveLoadType ==0)
	{
		SetLines(1);
		pXL->InsertCopyRowLine(3,5,m_nSttRow);
		SetLines(3);
	}

	CString	szLCName;

	CFemHLLCalc HLLCalc(m_pAPlateOutCalcStd->GetDataManage(), m_pAPlateOutCalcStd->GetAPlateCalcStd());

	for(long n=0; n<pFEM->GetLoadCaseSize(); n++)
	{
		szLCName = pFEM->m_szLCName.GetAt(n);

		if(ModelCalc.IsLiveCombMaxLC(szLCName))
		{
			sText = RetMaxString(szLCName);			
			pOut->SetXL(pXL, sText, m_nSttRow, "B", "B", 1, BLUE);
			Dom.ClearEtt(TRUE);
			pOptStd->SetEnvType(&Dom, HCAD_DIML);
			DrawHLoad(&Dom, pFEM, n);

			pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 2, 4, 27, 30, TRUE, TRUE, TRUE);
			SetLines(2);
		}
	}

	for(nLoadCase=0; nLoadCase<pFEM->GetLoadCaseSize(); nLoadCase++)
	{
		szLoadCase = pFEM->GetLoadCaseString(nLoadCase);
		if(szLoadCase.Find("PEOPLE") != -1)
		{
			pOut->SetXL(pXL, szLoadCase, m_nSttRow, "B", "B", 1, BLUE);
			Dom.ClearEtt(TRUE);
			DrawHLoad(&Dom, pFEM, nLoadCase);
			pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 2, 4, 27, 30, TRUE, TRUE, TRUE);
			SetLines(2);
			WriteLoadForce(pFEM, nLoadCase);
			SetLines(NEXT);
			nIdx++;
		}
	}

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(szProgress, 44);

	long nCombo = atol(pDB->m_sDeungeub)-1;
	
	if(nCombo< 0 || nCombo>2) nCombo= 0;

	double	dPr	= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dPf	= tokN(pCalcData->DESIGN_CONDITION.m_dPf);
	double	dPm = tokN(pCalcData->DESIGN_CONDITION.m_dPm);
	double	dW	= tokNPM(pCalcData->DESIGN_CONDITION.m_dW);
	double	dPs	= tokN(pCalcData->DESIGN_CONDITION.m_dPs);
	
	pXL->InsertCopyRowLine(12, 45, m_nSttRow);

	sText.Format("%d) 활하중의 크기", nNum);	nNum++;

	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1);

	CString sLevelDB, sLevelDL;

	if(nCombo == 0)
	{
		sLevelDB.Format("DB - 24");
		sLevelDL.Format("DL - 24");
	}

	if(nCombo == 1)
	{
		sLevelDB.Format("DB - 18");
		sLevelDL.Format("DL - 18");
	}

	if(nCombo == 2)
	{
		sLevelDB.Format("DB - 13.5");
		sLevelDL.Format("DL - 13.5");
	}

	pOut->SetXL(pXL, sLevelDB,	m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, dPf,		m_nSttRow, "G", "I", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPr,		m_nSttRow, "G", "I", 1, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sLevelDL,	m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, dW,		m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPm,		m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dPs,		m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER);

	if(pCalcData->m_bTank)
	{
		pOut->SetXL(pXL, pCalcData->m_TankWeight,	m_nSttRow, "G", "I", 1, RED, 9, FALSE, TA_CENTER);
		pOut->InsertImage(pXL, "탱크궤도하중",		m_nSttRow, "B");

		SetLines(9);

		pOut->SetXL(pXL, pCalcData->m_TankEdit1,	m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TankEdit2,	m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TankEdit3,	m_nSttRow, "N", "O", 4, RED,   9, FALSE, TA_CENTER);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+13);
		m_nSttRow+=14;
	}

	if(pCalcData->m_bTrailer)
	{
		m_nSttRow++;
		pOut->InsertImage(pXL, "탱크트레일러하중",	m_nSttRow, "B");
		SetLines(5);
		pOut->SetXL(pXL, pCalcData->m_TrailEdit6,	m_nSttRow, "R", "T", 1, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TrailEdit5,	m_nSttRow, "Q", "R", 2, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TrailEdit1,	m_nSttRow, "D", "F", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TrailEdit2,	m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TrailEdit3,	m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TrailEdit4,	m_nSttRow, "N", "O", 2, RED,   9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, pCalcData->m_TrailP1,		m_nSttRow, "F", "G", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TrailP2,		m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, pCalcData->m_TrailP3,		m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+11);
		m_nSttRow+=12;
	}

	pXL->DeleteRowLine(2, 45);
	pXL->DeleteRowLineEnd();
}

void CXLDesignOfMold::ComputeHLoad_Wind()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(-1);

	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	m_nSttRow = 1;
	long nNum = pDB->IsTUGir() ? 6 : 5;
	long nRowL		= 5;	// 좌측 방음벽 있을때 시작
	long nRowROnly	= 50;	// 우측에만 있을때 시작
	if(!pCalcData->m_bWind)
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+59);
		pXL->DeleteRowLineEnd();
		return;
	}
		
	CString		sText;
	CMatrixStr	MatArrPier;
	long		nWindType	= 0;
	long		nQtyJigan	= pDB->m_nQtyJigan;
	double		dBridgeW	= toM(pDB->m_dBridgeWidth);
	double		dSta		= 0.0;
	double		dGirderH	= 0.0;
	double		dSlabThick	= toM(pDB->m_dThickSlabBase);
	double		dLBangEumH	= toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight);
	double		dRBangEumH	= toM(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight);
	BOOL		bLBangEum	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm;
	BOOL		bRBangEum	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm;

	if(bLBangEum && bRBangEum)			nWindType = 1;
	else if(bLBangEum && !bRBangEum)	nWindType = 2;
	else if(!bLBangEum && bRBangEum)	nWindType = 3;

	MatArrPier.Resize(nQtyJigan+1, 1);
	long nK = 0;
	for(nK = 0; nK < nQtyJigan+1; nK++)
		MatArrPier(nK,0) = pDB->m_strJijumName[nK];

	sText.Format("%d) 풍 하 중", nNum);
	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 3);

	long nWind = nQtyJigan+1;
	if(pDB->IsByunDanInBridge()==FALSE)
		nWind = 1;

	for(nK = 0; nK < nWind; nK++)
	{
		CPlateBasicIndex	*pBx		= pGir->GetBxOnJijum(nK);

		dGirderH	= toM(pBx->GetHeightGirderByBx());
		dSta		= toM(pBx->GetStation());

		pOut->ClearCellRef();
		sText.Format("< %s 위치 : %s >", MatArrPier(nK, 0), GetStationForStringOut(frM(dSta),3,TRUE,TRUE));
		pOut->SetXL(pXL, sText,									m_nSttRow, "B", "B", 1, RED);

		if(nK!=0)	pXL->InsertCopyRowLine(nRowL, nRowROnly+9, m_nSttRow);

		///< 좌측
		if(nWindType == 1 || nWindType == 2)
		{
			m_nSttRow++;
			pOut->SetXL(pXL, dBridgeW,							m_nSttRow, "G", "I", 2, BLUE,  9, FALSE, TA_CENTER, "L_BRIDGE_W");
			pOut->SetXL(pXL, dGirderH,							m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "L_GIRDER_H");
			pOut->SetXL(pXL, dSlabThick,						m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "L_SLABTHICK");
			pOut->SetXL(pXL, dLBangEumH,						m_nSttRow, "O", "Q", 6, BLACK, 9, FALSE, TA_CENTER, "L_BANGEUM_H");

			pOut->InsertImage(pXL, "풍좌_외측에서내측", m_nSttRow, "C");
			m_nSttRow += 8;
			///< 좌방음벽만 있을경우
			if(nWindType == 2)
			{
				m_nSttRow += 2;
				pOut->InsertImage(pXL, "풍좌_내측에서외측", m_nSttRow, "B");
				m_nSttRow += 8;				
			}
			else
			{
				pXL->DeleteRowLine(m_nSttRow, m_nSttRow+9);
				m_nSttRow += 10;				
			}			
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow+26);
			m_nSttRow += 27;
		}
		
		///< 우측
		if(nWindType == 1 || nWindType == 3)
		{
			m_nSttRow++;

			pOut->SetXL(pXL, dBridgeW,							m_nSttRow, "G", "I", 2, BLUE,  9, FALSE, TA_CENTER, "R_BRIDGE_W");
			pOut->SetXL(pXL, dGirderH,							m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "R_GIRDER_H");
			pOut->SetXL(pXL, dSlabThick,						m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "R_SLABTHICK");
			pOut->SetXL(pXL, dRBangEumH,						m_nSttRow, "O", "Q", 6, BLACK, 9, FALSE, TA_CENTER, "R_BANGEUM_H");

			pOut->InsertImage(pXL, "풍우_외측에서내측", m_nSttRow, "D");
			m_nSttRow += 8;

			///< 우방음벽만 있을경우
			if(nWindType == 3)
			{
				m_nSttRow += 2;
				pOut->InsertImage(pXL, "풍우_내측에서외측", m_nSttRow, "D");
				m_nSttRow += 8;
			}
			else
			{
				pXL->DeleteRowLine(m_nSttRow, m_nSttRow+9);
				m_nSttRow += 10;				
			}
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow+26);
			m_nSttRow += 26;			
		}		
	}
	m_nSttRow+=2;
}

///< 충격계수
void CXLDesignOfMold::ComputeHLoad_Collision()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sBridgeLength;
	CString	sText, sText1, sText2, sText3, sText4;
	long	nQtyJigan	= pDB->m_nQtyJigan;
	long	nK			= 0;

	long nNum = 5;
	if(pCalcData->m_bWind)	nNum++;
	if(pDB->IsTUGir())		nNum++;
	pOut->ClearCellRef();
	sText.Format("%d) 충 격 계 수", nNum);
	pOut->SetXL(pXL, sText,							m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "(☞도.설.기 9 Page)",			m_nSttRow, "U", "U", 2);
	pOut->SetXL(pXL, "지 간 :",						m_nSttRow, "C", "C", 0);

	for(nK = 0; nK < nQtyJigan; nK++)
	{
		sText.Format("L%d =", nK+1);
		pOut->SetXL(pXL, sText,									m_nSttRow, "E", "F", 0);
		sText.Format("L%d", nK+1);
		pOut->SetXL(pXL, toM(pDB->m_dLengthJigan[nK]),	m_nSttRow, "G", "I", 0, RED, 9, FALSE, TA_CENTER, sText);
		pOut->SetXL(pXL, "m",									m_nSttRow, "J", "J", 1);
	}

	pOut->SetXL(pXL, "따라서, 내부 지점 및 지간내에 재하되는 하중에 대한 충격계수는 아래와 같다.", m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "다만, 탱크궤도하중과 탱크 트레일러 하중에는 일률적으로 충격계수를 0.15로 적용 한다.", m_nSttRow, "C", "C", 2);

	///< 경간부
	for(nK = 0; nK < nQtyJigan; nK++)
	{
		sText.Format("◁ 제 %d 지간 ", nK+1);
		pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C", 1);
		pOut->SetLineBoxText(pXL, "i",						m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		sText1.Format("15_%d", nK+1);
		pOut->SetUnderLineText(pXL, 15.0,					m_nSttRow, "F", "L", 0, BLACK, 9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "",								m_nSttRow, "Z", "Z", 1);
		sText1.Format("40_%d", nK+1);
		pOut->SetXL(pXL, 40.0,								m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "+",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);

		sText1.Format("L%d", nK+1);
		sText.Format("=%s", pOut->GetCellRef(sText1));
		sText1.Format("L%d_F", nK+1);
		pOut->SetXL(pXL, sText,								m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, sText1);

		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		sText1.Format("15_%d", nK+1);sText2.Format("40_%d", nK+1);sText3.Format("L%d_F", nK+1);
		sText.Format("=%s/(%s+%s)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2), pOut->GetCellRef(sText3));
		sText1.Format("I_%d", nK+1);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, sText1, 3, FALSE);

		sText1.Format("AI_%d", nK+1);
		pOut->SetLineBoxText(pXL, 0.3,						m_nSttRow, "R", "T", 0, BLACK, 9, FALSE, TA_CENTER, sText1, 3, FALSE);

		sText1.Format("I_%d", nK+1); sText2.Format("AI_%d", nK+1);
		sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2));
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE,  TA_CENTER, "", 0, FALSE);

		pOut->SetLineBoxText(pXL, "⇒",						m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

		sText1.Format("I_%d", nK+1); sText2.Format("AI_%d", nK+1);
		sText.Format("=IF(%s>%s, %s, %s)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2), pOut->GetCellRef(sText2), pOut->GetCellRef(sText1));
		sText3.Format("JIGAN_I_%d", nK+1);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_CENTER, sText3, 3, FALSE);

		pOut->SetXL(pXL, "",								m_nSttRow, "Z", "Z", 2);
	}

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.3 작용하중의 산정 - 충격계수", 55);
	///< 지점부
	for(nK = 0; nK < nQtyJigan-1; nK++)
	{
		sText.Format("◁ 제 %d 내부지점 ", nK+1);
		pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C", 1);
		pOut->SetLineBoxText(pXL, "i",						m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		sText1.Format("15IN_%d", nK+1);
		pOut->SetUnderLineText(pXL, 15.0,					m_nSttRow, "F", "T", 0, BLACK, 9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "",								m_nSttRow, "Z", "Z", 1);
		sText1.Format("40IN_%d", nK+1);
		pOut->SetXL(pXL, 40.0,								m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "+",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(",								m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);

		sText1.Format("L%d", nK+1);
		sText.Format("=%s", pOut->GetCellRef(sText1));
		sText1.Format("L%d_PRE", nK+1);
		pOut->SetXL(pXL, sText,								m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, sText1);
		pOut->SetXL(pXL, "+",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);

		sText1.Format("L%d", nK+2);
		sText.Format("=%s", pOut->GetCellRef(sText1));
		sText1.Format("L%d_POST", nK+2);
		pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, sText1);

		pOut->SetXL(pXL, ")",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "/",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pXL->SetNumberFormat(m_nSttRow-1, 19, m_nSttRow-1, 19, "#0");		
		pOut->SetXL(pXL, 2.0,								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

		sText1.Format("15IN_%d", nK+1);sText2.Format("40IN_%d", nK+1);sText3.Format("L%d_PRE", nK+1);sText4.Format("L%d_POST", nK+2);
		sText.Format("=%s / (%s + (%s+%s)/2)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2), pOut->GetCellRef(sText3), pOut->GetCellRef(sText4));
		sText1.Format("I_IN_%d", nK+1);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_CENTER, sText1, 3, FALSE);

		pOut->SetXL(pXL, "",								m_nSttRow, "Z", "Z", 1);

		sText1.Format("I_IN_%d", nK+1);
		sText.Format("=%s", pOut->GetCellRef(sText1));
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "I", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 3, FALSE);

		sText1.Format("AI_IN_%d", nK+1);
		pOut->SetLineBoxText(pXL, 0.3,						m_nSttRow, "M", "O", 2, BLACK, 9, FALSE, TA_CENTER, sText1, 3, FALSE);

		sText1.Format("I_IN_%d", nK+1); sText2.Format("AI_IN_%d", nK+1);
		sText.Format("=IF(%s>%s, \">\", \"<\")", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2));
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "L", "L", 2, BLACK, 9, FALSE,  TA_CENTER, "", 0, FALSE);

		pOut->SetLineBoxText(pXL, "⇒",						m_nSttRow, "P", "P", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

		sText1.Format("I_IN_%d", nK+1); sText2.Format("AI_IN_%d", nK+1);
		sText.Format("=IF(%s>%s, %s, %s)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2), pOut->GetCellRef(sText2), pOut->GetCellRef(sText1));
		sText3.Format("JIJUM_I_%d", nK+1);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "Q", "S", 2, BLACK, 9, FALSE, TA_CENTER, sText3, 3, FALSE);

		pOut->SetXL(pXL, "",								m_nSttRow, "Z", "Z", 2);
	}

	pOut->SetXL(pXL, "∴ 계산된 충격계수 중 가장 큰",		m_nSttRow, "C", "C", 0);

	sText.Format("=MAX(");
	for(nK = 0; nK < nQtyJigan; nK++)
	{
		sText1.Format("JIGAN_I_%d", nK+1);
		sText += pOut->GetCellRef(sText1) + ",";
	}
	for(nK = 0; nK < nQtyJigan-1; nK++)
	{
		sText1.Format("JIJUM_I_%d", nK+1);
		sText += pOut->GetCellRef(sText1) + ",";
	}
	sText.Delete(sText.GetLength()-1);
	sText += ")";

	pOut->SetXL(pXL, sText,									m_nSttRow, "K", "M", 0, RED,   9, FALSE,  TA_CENTER);
	pOut->SetXL(pXL, "를 적용함",							m_nSttRow, "N", "N", 2);

	pXL->DeleteRowLineEnd();
}

///< 4.3 나) 주형별 작용하중
void CXLDesignOfMold::ComputeHLoad_Mold()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();	
	CFEMManage			*pManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	m_nSttRow	= 4;
	pOut->ClearCellRef();
	CString		sText;
	CString		szSec;
	long	nG				= 1;
	double	dBonusRate		= pCalcData->m_BonusRate;
	double	dTotalWeight	= pCalcData->m_TotalWeight;
	double	dModelWeight	= pCalcData->m_ModelWeight;
	double	dSta			= 0;
	double	dUnitWeight		= pCalcData->DESIGN_UNIT_WEIGHT.m_dGangjae;

	sText.Format("=(%.6f-1)*100", dBonusRate);
	pOut->SetXL(pXL, sText,			m_nSttRow, "R", "T", 1, BLUE,  9, FALSE, TA_CENTER, "BONUS", 6);
	pOut->SetXL(pXL, dTotalWeight,	m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "TOTAL_WEIGHT");
	pOut->SetXL(pXL, dModelWeight,	m_nSttRow, "O", "Q", 1, BLACK, 9, FALSE, TA_CENTER, "MODEL_WEIGHT");

	// 단면
	long nCount=0;
	POSITION posSec = pManage->m_FrameSectionList.GetHeadPosition();
	while(posSec)
	{
		CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);  
		if(pFrSec->m_nFlag==POS_GIRDER )
		{
			nG = -1;
			ModelCalc.GetPositionBySectionName(pFrSec->m_szName, nG, dSta, 0);	
			szSec = _T("");
			szSec = GetJointArrszByI33(pFrSec->m_dIx, 0);
			ModelCalc.GetSectionCoefficient(nG, dSta, FALSE, dSta, 1);	
			CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
			CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);
			double	dGirderAs	= toM2(ModelCalc.GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS, pBx));

			if(nCount!=0)
				pXL->InsertCopyRowLine(m_nSttRow-1, m_nSttRow-1, m_nSttRow);

			sText.Format("· 단면 - %d :", ++nCount);
			pOut->SetXL(pXL, sText,					m_nSttRow, "C", "C", 0);
			pOut->SetXL(pXL, dGirderAs,				m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, tokNPM3(dUnitWeight),	m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("=%s/100+1", pOut->GetCellRef("BONUS"));
			pOut->SetXL(pXL, sText,					m_nSttRow, "S", "U", 1, BLACK, 9, FALSE, TA_CENTER);
		}
	}

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.3 작용하중의 산정 - 주형별 작용하중", 62);
	if(pDB->IsTUGir())
	{
		m_nSttRow++;
		ComputeHLoad_Mold_TU();
	}	
	else
	{
		ComputeHLoad_Mold_Cross();
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+8);
		pXL->DeleteRowLineEnd();
	}
	m_nSttRow++;

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.3 작용하중의 산정 - 주형별 작용하중", 64);
	///< 합성전. 합성후 고정하중
	ComputeHLoad_DeadLoad();

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.3 작용하중의 산정 - 주형별 작용하중", 67);
	///< 활하중
	ComputeHLoad_LiveLoad();

	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.3 작용하중의 산정 - 주형별 작용하중", 69);
	// 보도하중
	if((pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bPeopleLoad)
		|| (pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bPeopleLoad))
	{
		ComputeHLoad_PeopleLoad();
	}

	// 풍하중
	if(pCalcData->m_bWind)
	{
		ComputeHLoad_WindLoad();
	}
}

void CXLDesignOfMold::ComputeHLoad_Mold_Cross()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	CString		sText;
	CString		szSec;
	long	nG				= 1;
	double	dSta			= 0;
	double	dUnitWeight		= pCalcData->DESIGN_UNIT_WEIGHT.m_dGangjae;

	CDPointArray vArrJewon;		//(x : 단면이름, y : Area)
	CDPoint		ArrSec;
	POSITION posSec = pManage->m_FrameSectionList.GetHeadPosition();
	
	while(posSec)
	{
		CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);
		if(pFrSec->m_nFlag==POS_CROSSBEAM )
		{
			nG = -1;
			ModelCalc.GetPositionBySectionName(pFrSec->m_szName, nG, dSta, 1);	
			CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxByStation(dSta, BX_CROSSBEAM_VBRACING);
			ModelCalc.GetSectionCoefficientCR(pBx);
			double	dCrossBeamAs = toM2(ModelCalc.GetMapValueSecCoCR("Sum_As"));
			ArrSec.x = atof(pFrSec->m_szName);
			ArrSec.y = dCrossBeamAs;
			vArrJewon.Add(ArrSec);
		}
	}
	
	long nSize = vArrJewon.GetSize();
	for(long k=0; k<vArrJewon.GetSize(); k++)
	{
		double dArea1 = vArrJewon.GetAt(k).y;
		for(long l=k+1; l<nSize; l++)
		{
			double dArea2 = vArrJewon.GetAt(l).y;
			if(dArea1 == dArea2)
			{
				vArrJewon.RemoveAt(l--);
				nSize--;
			}
		}
	}

	for(long n=0; n<vArrJewon.GetSize(); n++)
	{
		double	dCrossBeamAs = vArrJewon.GetAt(n).y;

		pXL->InsertCopyRowLine(6, 6, m_nSttRow);

		sText.Format("· 가로보 - %d :", n+1);
		pOut->SetXL(pXL, sText,					m_nSttRow, "C", "C", 0);
		pOut->SetXL(pXL, dCrossBeamAs,			m_nSttRow, "G", "I", 0, RED,   9, FALSE, TA_CENTER, "", 3);
		pOut->SetXL(pXL, tokNPM3(dUnitWeight),	m_nSttRow, "L", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/100+1", pOut->GetCellRef("BONUS"));
		pOut->SetXL(pXL, sText,					m_nSttRow, "S", "U", 1, BLACK, 9, FALSE, TA_CENTER);
	}
}

void CXLDesignOfMold::ComputeHLoad_Mold_TU()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();	
	CFEMManage			*pManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	CString		sText, szSec, szBindWeight;
	long	nG				= 1;
	double	dSta			= 0;
	double	dUnitWeight		= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;

	double	dBindW	= pDB->m_BindConc.m_dWidth;
	double	dBindH	= pDB->m_BindConc.m_dHeight;

	m_nSttRow++;
	pOut->SetXL(pXL, toM(dBindH),			m_nSttRow, "C", "E", 0, RED,	9, FALSE, TA_CENTER, "BindH");
	pOut->SetXL(pXL, toM(dBindW),			m_nSttRow, "G", "I", 0, RED,	9, FALSE, TA_CENTER, "BindW");
	pOut->SetXL(pXL, tokNPM3(dUnitWeight),	m_nSttRow, "K", "M", 0, BLACK,	9, FALSE, TA_CENTER, "Bind_Unit_Weight");
	szBindWeight.Format("=Q%d", m_nSttRow);
	m_nSttRow += 6;

	//3) 180˚TURNING (강재거더 자중 + 구속콘크리트 자중)
	// 단면
	long nCount=0;
	POSITION posSec = pManage->m_FrameSectionList.GetHeadPosition();
	while(posSec)
	{
		CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);  
		if(pFrSec->m_nFlag==POS_GIRDER )
		{
			nG = -1;
			ModelCalc.GetPositionBySectionName(pFrSec->m_szName, nG, dSta, 0);	
			szSec = _T("");
			szSec = GetJointArrszByI33(pFrSec->m_dIx, 0);
			ModelCalc.GetSectionCoefficient(nG, dSta, FALSE, dSta, 1);	
			
			if(nCount!=0)
				pXL->InsertCopyRowLine(m_nSttRow-1, m_nSttRow-1, m_nSttRow);

			sText.Format("· 단면 - %d :", ++nCount);
			pOut->SetXL(pXL, sText,			m_nSttRow, "C", "C", 0);
			pOut->SetXL(pXL, szBindWeight,	m_nSttRow, "L", "N", 1);
		}
	}
}

///< 합성전. 합성후 고정하중
void CXLDesignOfMold::ComputeHLoad_DeadLoad()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetHLLFemData(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CString sText;

	long	nGirdersu	= pDB->GetGirdersu();
	
	for(long n = 0; n < 2; n++)
	{
		///< n = 0 : 합성전 고정하중 n = 1 : 합성후 고정하중		
		if(n == 0)
		{
			if(pDB->IsTUGir())
			{
				sText.Format("4) 가로보 + 바닥판 자중");
				pOut->SetXL(pXL, sText,		m_nSttRow, "B", "B");
				ComputeHLoad_Mold_Cross();
				m_nSttRow++;
			}
			else
			{
				sText.Format("2) 합성전 고정하중(슬래브 자중)");
				pOut->SetXL(pXL, sText,		m_nSttRow, "B", "B");
			}
			
			if(!pDB->IsTUGir())
			{
				pOut->InsertImage(pXL, "작용하중의산정_합성전", m_nSttRow, "K");
				SetLines(8);	///< 그림 삽입
			}
			sText.Format("합성전");
			pOut->SetLineBoxText(pXL, sText, m_nSttRow, "B", "F", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
		}
		else
		{
			sText.Format("%d) 합성후 고정하중", 3+(pDB->IsTUGir()?2:0));
			pOut->SetXL(pXL, sText,					m_nSttRow, "B", "B");
			sText.Format("합성후");
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "B", "F", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
		}
		
		long	nMaxLoadCaseSize = n == 0 ? 1 : 10;
		CString	szLoadCase		 = _T("");
		CString	szTitle			 = _T("");
		long	nLoadCaseIndex	 = 0;

		for(long nLoadCase = 0; nLoadCase < nMaxLoadCaseSize; nLoadCase++)
		{
			long nCellIdx = 6;

			if(n != 0)	//After
			{
				switch(nLoadCase)
				{
				case 0 : szLoadCase	= _T("PAVE");	szTitle	= _T("포장 하중");		break;
				case 1 : szLoadCase = _T("GUARD");	szTitle	= _T("방호벽 하중");	break;
				case 2 : szLoadCase = _T("WALK");	szTitle	= _T("보도 하중");		break;
				case 3 : szLoadCase = _T("SOUNDL");	szTitle	= _T("방음벽(좌)하중"); break;
				case 4 : szLoadCase = _T("SOUNDR");	szTitle	= _T("방음벽(우)하중"); break;
				case 5 : szLoadCase = _T("HANDL");	szTitle	= _T("난간(좌) 하중");	break;
				case 6 : szLoadCase = _T("HANDR");	szTitle	= _T("난간(우) 하중");	break;
				case 7 : szLoadCase = _T("EXTRA");	szTitle	= _T("기타 하중");		break;
				}
				
				nLoadCaseIndex = pFEM->GetLoadCaseIndex(szLoadCase);
				if(nLoadCaseIndex<0)	continue;

				if(nLoadCase != 0 )
				{
					long nSttRomTemp = m_nSttRow + 1;

					sText.Format("합성후");
					pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "B", "F", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
					pOut->SetLineBoxText(pXL, "단위",	m_nSttRow, "Y","AA", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
					pOut->SetLineBoxText(pXL, "kN/m",	nSttRomTemp, "Y", "AA", 1, BLACK, 9, FALSE, TA_CENTER);
				}
				
			}
			
			for(long nG = 0; nG < nGirdersu; nG++)
			{
				if(nG != 0 && nG % 6 == 0)
				{
					pOut->SetLineBoxText(pXL, "단위",	m_nSttRow, "Y","AA", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
					SetLines(NEXT);
					pOut->SetLineBoxText(pXL, "kN/m",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx + 2], 1, BLACK, 9, FALSE, TA_CENTER);
					SetLines(NEXT);
					if(n == 0)	sText.Format("합성전");
					else		sText.Format("합성후");
					pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "B", "F", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
					nCellIdx = 6;
				}
				
				CJoint *pJ			  = pFEM->GetJoint((unsigned short)nG + 1);
				double dReactionForce = pJ->GetResultForce(nLoadCaseIndex)->m_Force.z;
				
				if(n==1 && pDB->IsTUGir())
					dReactionForce = pJ->GetResultForce(nLoadCaseIndex + 1)->m_Force.z;
				
				sText.Format("G - %d", nG + 1);
				pOut->SetLineBoxText(pXL, sText,		m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx + 2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
				SetLines(NEXT);

				if(n == 0)	sText = _T("슬래브자중");
				else		sText = szLoadCase;

				pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, "P",			m_nSttRow, "E", "F", 1, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("%.3f", dReactionForce);
				pOut->SetLineBoxText(pXL, sText,		m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx + 2], 1, RED, 9, FALSE, TA_CENTER);
				
				SetLines(-1);
				
				nCellIdx += 3;
				
				if(nG == nGirdersu - 1)
				{
					pOut->SetLineBoxText(pXL, "단위",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx + 2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
					SetLines(1);
					pOut->SetLineBoxText(pXL, "kN/m",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx + 2], 1, BLACK, 9, FALSE, TA_CENTER);
					SetLines(-1);
				}
			}
			SetLines((long)(nGirdersu / 6 + 0.5) * 2 + 3);
		}
	}

	double	dAddLoad		= pCalcData->m_AddLoad;
	double	dBridgeWidth	= pCalcData->DESIGN_BASE.m_dBridgeWidth;

	pOut->SetXL(pXL, "상기 하중과 점검로 등의 부속설비 하중으로", m_nSttRow, "B", "B", 0);
	sText.Format("%.3f", dAddLoad);
	pOut->SetXL(pXL, sText,							m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "ADDLOAD", 0);
	pOut->SetXL(pXL, "N/m²을 설계에 반영 [강도로교상세부설계지침  P131]",	m_nSttRow, "N", "N", 1);
	sText.Format("=\"거더당 작용하는 부속설비 하중 W(kN/m) = \"&%s/1000&\" kN/m² × 슬래브 전폭(m) / 총거더수\"", pOut->GetCellRef("ADDLOAD"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "W",							m_nSttRow, "B", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",							m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/1000", pOut->GetCellRef("ADDLOAD"));
	pOut->SetXL(pXL, sText,							m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "W_PARA1");
	pOut->SetXL(pXL, "×",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%.3f", dBridgeWidth);
	pOut->SetXL(pXL, sText,							m_nSttRow, "I", "K", 0, RED,   9, FALSE, TA_CENTER, "W_PARA2");
	pOut->SetXL(pXL, "/",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("%d", nGirdersu);
	pXL->SetNumberFormat(m_nSttRow - 1, 12, m_nSttRow - 1, 12, "#0");
	pOut->SetXL(pXL, sText,							m_nSttRow, "M", "M", 0, RED,   9, FALSE, TA_CENTER, "W_PARA3", 0);
	pOut->SetXL(pXL, "=",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s/%s", pOut->GetCellRef("W_PARA1"), pOut->GetCellRef("W_PARA2"), pOut->GetCellRef("W_PARA3"));	
	pOut->SetXL(pXL, sText,							m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "kN/m",						m_nSttRow, "R", "R", 2);
}

void CXLDesignOfMold::ComputeHLoad_LiveLoad()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM		= pCalcStd->GetHLLFemData(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CString				sText	=	_T("");

	long	nGirdersu		= pDB->GetGirdersu();
	long	nLoadCase		= pFEM->GetLoadCaseSize();
	long	nChasunsu		= 0;
	long	nCellIdx		= 5;
	long	nSttTemp		= 0;
	double	dReactionForce	= 0;
	CString szComp("");

	sText.Format("%d) 활 하 중", 4+(pDB->IsTUGir()?2:0));
	pOut->SetXL(pXL, sText,					m_nSttRow, "B", "B", 1);

	///< DB 하중 >
	pOut->SetXL(pXL, "< DB 하중 >",			m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "(단위하중재하결과)",	m_nSttRow, "L", "L", 1);

	long icData = 6;
	long icTable = RoundUp((double)nGirdersu/icData, 0);
	for(long ixTable = 0; ixTable < icTable; ++ixTable)
	{
		long nGStt = ixTable*icData;
		long nGEnd = MIN(nGStt+icData, nGirdersu);
		nCellIdx = 5;

		pOut->SetLineBoxText(pXL, "차 선",		m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
		for(long nG = nGStt; nG < nGEnd; nG++)
		{
			sText.Format("G - %d", nG+1);
			pOut->SetLineBoxText(pXL, sText,	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
			nCellIdx+=3;
		}
		pOut->SetLineBoxText(pXL, "비고",		m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
		SetLines(NEXT);

		nSttTemp = m_nSttRow;
		nChasunsu = 0;

		for(long nLC = 0; nLC < nLoadCase; nLC++)
		{
			long nFind			=	pFEM->m_szLCName[nLC].Find("DB");
			CString szLCName	=	pFEM->m_szLCName[nLC];

			if(nFind!=-1)
			{

				if(pFEM->m_szLCName[nLC].Find("DBR-1")!=-1)
				{
					sText.Format("L->R");
					pOut->SetLineBoxText(pXL, sText,	nSttTemp, "B", "C", nChasunsu, BLACK, 9, FALSE, TA_CENTER, "", 0);

					nSttTemp += nChasunsu;
					nChasunsu = 0;
				}	

				nChasunsu += 1;

				sText.Format("%d 차선", nChasunsu);
				pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "D", "E", 1, BLACK, 9, FALSE, TA_CENTER);

				nCellIdx = 5;

				for(long nG = nGStt; nG < nGEnd; nG++)
				{
					CJoint *pJ		= pFEM->GetJoint((unsigned short)nG+1);

					dReactionForce	= pJ->GetResultForce(nLC)->m_Force.z;

					sText.Format("%.3f", dReactionForce);

					pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);

					nCellIdx+=3;
				}


				CString szLane = pFEM->m_szLCName[nLC].Right(1);

				sText.Format("Sf=%.3f", GetScaceFactor(atol(szLane)));
				pOut->SetLineBoxText(pXL, sText,	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER, "", 2);

				SetLines(NEXT);
			}
		}	
		sText.Format("R->L");
		pOut->SetLineBoxText(pXL, sText,	nSttTemp, "B", "C", nChasunsu, BLACK, 9, FALSE, TA_CENTER, "", 0);
	}

	SetLines(1);

	///< DL 하중 >
	for(long ixTable = 0; ixTable < icTable; ++ixTable)
	{
		long nGStt = ixTable*icData;
		long nGEnd = MIN(nGStt+icData, nGirdersu);

		nCellIdx	= 5;
		pOut->SetXL(pXL, "< DL 하중 >",			m_nSttRow, "B", "B", 1);
		pOut->SetLineBoxText(pXL, "차 선",		m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);

		for(long nG = nGStt; nG < nGEnd; nG++)
		{
			sText.Format("G - %d", nG+1);
			pOut->SetLineBoxText(pXL, sText,	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
			nCellIdx+=3;
		}
		pOut->SetLineBoxText(pXL, "비고",		m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
		SetLines(NEXT);

		nSttTemp	=	m_nSttRow;
		nChasunsu	=	0;

		for(long nLC = 0; nLC < nLoadCase; nLC++)
		{
			CString szLCName	=	pFEM->m_szLCName[nLC];

			long nFind			=	szLCName.Find("DL");

			if(pFEM->m_szLCName[nLC].Find("SOUND") > -1) nFind = -1;

			if(nFind!=-1)
			{

				if(szLCName.Find("DLR-1")!=-1)
				{
					sText.Format("L->R");
					pOut->SetLineBoxText(pXL, sText,	nSttTemp, "B", "C", nChasunsu, BLACK, 9, FALSE, TA_CENTER, "", 0);

					nSttTemp += nChasunsu;
					nChasunsu = 0;
				}	

				nChasunsu += 1;

				sText.Format("%d 차선", nChasunsu);
				pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "D", "E", 1, BLACK, 9, FALSE, TA_CENTER);

				nCellIdx = 5;

				for(long nG = nGStt; nG < nGEnd; nG++)
				{
					CJoint *pJ		= pFEM->GetJoint((unsigned short)nG+1);

					dReactionForce	= pJ->GetResultForce(nLC)->m_Force.z;

					sText.Format("%.3f", dReactionForce);

					pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);

					nCellIdx+=3;
				}

				CString szLane = pFEM->m_szLCName[nLC].Right(1);

				sText.Format("Sf=%.3f", GetScaceFactor(atol(szLane)));

				pOut->SetLineBoxText(pXL, sText,	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER, "", 2);

				SetLines(NEXT);
			}
		}	

		sText.Format("R->L");
		pOut->SetLineBoxText(pXL, sText,	nSttTemp, "B", "C", nChasunsu, BLACK, 9, FALSE, TA_CENTER, "", 0);
	}
	

	///< TANK 하중 재하시
	if(pCalcData->m_bTank)
	{
		SetLines(1);
		pOut->SetXL(pXL, "< TANK 하중 재하시 >", m_nSttRow, "B", "B", 0);
		pOut->SetXL(pXL, "(단위하중재하)",	m_nSttRow, "L", "L", 1);

		for(long ixTable = 0; ixTable < icTable; ++ixTable)
		{
			long nGStt = ixTable*icData;
			long nGEnd = MIN(nGStt+icData, nGirdersu);
			nCellIdx	= 5;
			
			pOut->SetLineBoxText(pXL, "차 선",	m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);

			for(long nG = nGStt; nG < nGEnd; nG++)
			{
				sText.Format("G - %d", nG+1);
				pOut->SetLineBoxText(pXL, sText,m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
				nCellIdx+=3;
			}
			SetLines(NEXT);
			long nTank = 1;
			for(long nLC = 0; nLC < nLoadCase; nLC++)
			{
				CString szLC = pFEM->m_szLCName[nLC];
				nCellIdx = 5;
				szComp.Format("TANK-%d", nTank);
				if(szLC==szComp)
				{
					sText.Format(szLC);
					pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

					for(long nG = nGStt; nG < nGEnd; nG++)
					{
						CJoint *pJ		= pFEM->GetJoint((unsigned short)nG+1);
						dReactionForce	= pJ->GetResultForce(nLC)->m_Force.z;
						sText.Format("%.3f", dReactionForce);
						pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
						nCellIdx+=3;
					}
					SetLines(NEXT);
					nTank++;
				}
			}
		}
		
	}

	///< < TANK TRAILER 하중 재하시 >  :  단위하중재하
	if(pCalcData->m_bTrailer)
	{
		SetLines(1);
		pOut->SetXL(pXL, "< TRAILER 하중 재하시 >", m_nSttRow, "B", "B", 0);
		pOut->SetXL(pXL, "(단위하중재하)",	m_nSttRow, "L", "L", 1);

		for(long ixTable = 0; ixTable < icTable; ++ixTable)
		{
			long nGStt = ixTable*icData;
			long nGEnd = MIN(nGStt+icData, nGirdersu);
			nCellIdx	= 5;
			
			pOut->SetLineBoxText(pXL, "차 선",		m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);

			for(long nG = nGStt; nG < nGEnd; nG++)
			{
				sText.Format("G - %d", nG+1);
				pOut->SetLineBoxText(pXL, sText,	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
				nCellIdx+=3;
			}
			SetLines(NEXT);
			long nTrailer = 1;
			for(long nLC = 0; nLC < nLoadCase; nLC++)
			{
				CString szLC = pFEM->m_szLCName[nLC];
				nCellIdx = 5;
				szComp.Format("TRAIL-%d", nTrailer);
				if(szLC==szComp)
				{
					sText.Format(szLC);
					pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

					for(long nG = nGStt; nG < nGEnd; nG++)
					{
						CJoint *pJ		= pFEM->GetJoint((unsigned short)nG+1);
						dReactionForce	= pJ->GetResultForce(nLC)->m_Force.z;
						sText.Format("%.3f", dReactionForce);
						pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
						nCellIdx+=3;
					}
					SetLines(NEXT);
					nTrailer++;
				}

			}
		}
	}
}

void CXLDesignOfMold::ComputeHLoad_PeopleLoad()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM		= pCalcStd->GetHLLFemData(0);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CString sText;

	long	nGirdersu		= pBridgeApp->GetGirdersu();
	long	nLoadCase		= pFEM->GetLoadCaseSize();
	long	nCellIdx		= 5;
	double	dReactionForce	= 0;

	SetLines(NEXT);
	///< < 군집하중 >
	pOut->SetXL(pXL, "< 군집하중 >",			m_nSttRow, "B", "B", 1);

	long icData = 6;
	long icTable = RoundUp((double)nGirdersu/icData, 0);
	for(long ixTable = 0; ixTable < icTable; ++ixTable)
	{
		long nGStt = ixTable*icData;
		long nGEnd = MIN(nGStt+icData, nGirdersu);
		nCellIdx = 5;

		pOut->SetLineBoxText(pXL, "하 중",			m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
		for(long nG = nGStt; nG < nGEnd; nG++)
		{
			sText.Format("G - %d", nG+1);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
			nCellIdx+=3;
		}
		pOut->SetLineBoxText(pXL, "비고",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
		SetLines(NEXT);

		CString szComp("");
		for(long nLC = 0; nLC < nLoadCase; nLC++)
		{
			CString szLC = pFEM->m_szLCName[nLC];
			if(szLC.Find("PEOPLE")==-1)	continue;
			
			sText.Format("%s", szLC);
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER);

			nCellIdx = 5;
			for(long nG = nGStt; nG < nGEnd; nG++)
			{
				CJoint *pJ		= pFEM->GetJoint((unsigned short)nG+1);
				dReactionForce	= pJ->GetResultForce(nLC)->m_Force.z;
				sText.Format("%.3f", dReactionForce);
				pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
				nCellIdx+=3;
			}
			
			pOut->SetLineBoxText(pXL, "",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
		}
	}
}

void CXLDesignOfMold::ComputeHLoad_WindLoad()
{
	CPlateBridgeApp		*pBridge		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM		= pCalcStd->GetHLLFemData(0);
	CPlateGirderApp		*pGir			= pBridge->GetGirder(-1);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc ModelCalc(pDataManage, pCalcStd);
	
	long	nGirdersu		= pBridge->GetGirdersu();
	long	nCellIdx		= 5;
	double	dVForce			= 0;
	double	dHForce			= 0;

	CString		sText;
	CMatrixStr	MatArrPier;
	long		nQtyJigan	= pBridge->m_nQtyJigan;
	double		dSta		= 0.0;
	
	MatArrPier.Resize(nQtyJigan+1, 1);
	long nK = 0;
	for(nK = 0; nK < nQtyJigan+1; nK++)
		MatArrPier(nK,0) = pBridge->m_strJijumName[nK];

	SetLines(NEXT);
	///< < 풍하중 >
	pOut->SetXL(pXL, "< 풍하중 >",			m_nSttRow, "B", "B", 1);

	long nWind = nQtyJigan+1;
	if(pBridge->IsByunDanInBridge()==FALSE)
		nWind = 1;

	for(nK = 0; nK < nWind; nK++)
	{
		CFEMManage			*pManage	= pCalcStd->GetWindFemData(nK);
		CPlateBasicIndex	*pBx		= pGir->GetBxOnJijum(nK);

//		if(pManage->GetTotalResultSize()==0 || pCalcData->m_bRunCrossWind==FALSE)	continue;
		if(pManage->GetTotalResultSize()==0)	continue;

		dSta		= toM(pBx->GetStation());

		pOut->ClearCellRef();
		sText.Format("  - %s 위치 : %s ", MatArrPier(nK, 0), GetStationForStringOut(frM(dSta),3,TRUE,TRUE));
		pOut->SetXL(pXL, sText,									m_nSttRow, "B", "B", 1, RED);
		pOut->ClearCellRef();

		long icData = 6;
		long icTable = RoundUp((double)nGirdersu/icData, 0);
		for(long ixTable = 0; ixTable < icTable; ++ixTable)
		{
			long nGStt = ixTable*icData;
			long nGEnd = MIN(nGStt+icData, nGirdersu);
			nCellIdx	= 5;

			pOut->SetLineBoxText(pXL, "하 중",			m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
			for(long nG = nGStt; nG < nGEnd; nG++)
			{
				sText.Format("G - %d", nG+1);
				pOut->SetLineBoxText(pXL, sText,		m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
				nCellIdx+=3;
			}
			pOut->SetLineBoxText(pXL, "비고",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER, "", 0, TRUE, GRAY);
			SetLines(NEXT);

			for(long n=0; n<pManage->m_szLCName.GetSize(); n++)
			{
				CString szLCTitle = pManage->m_szLCName[n]; 
				pOut->SetLineBoxText(pXL, szLCTitle,	m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);

				nCellIdx = 5;
				for(long nG = nGStt; nG < nGEnd; nG++)
				{
					CResultForce *pResult = pManage->GetJoint((unsigned short)(nGirdersu+2+nG))->GetResultForce(n);

					dVForce	= pResult->m_Force.z;
					dHForce = pResult->m_Force.x;

					if(nG==nGStt)
					{
						pOut->SetLineBoxText(pXL, "V",	m_nSttRow, "E", "E", 1, BLACK, 9, FALSE, TA_CENTER);
						SetLines(NEXT);
						pOut->SetLineBoxText(pXL, "H",	m_nSttRow, "E", "E", 1, BLACK, 9, FALSE, TA_CENTER);
						SetLines(-1);
					}

					sText.Format("%.3f", dVForce);
					pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
					SetLines(NEXT);

					sText.Format("%.3f", dHForce);
					pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
					SetLines(-1);

					nCellIdx+=3;

					if(nG==nGEnd-1)
					{
						pOut->SetLineBoxText(pXL, "",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
						SetLines(NEXT);
						pOut->SetLineBoxText(pXL, "",	m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+2], 1, BLACK, 9, FALSE, TA_CENTER);
						SetLines(-1);
					}
				}
				SetLines(2);
			}
		}
	}	
}

///< 4.4 가) 모델링
void CXLDesignOfMold::Modeling()
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CARoadOptionStd		*pOptStd		= m_pAPlateOutCalcStd->GetDataManage()->GetOptionStd();
	
	m_nSttRow = 3;

	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	ModelPlanOutput(&Dom);
	Dom.Rotate(ConstPi*1./2.,0,0);
	pOut->InsertDomyun(pXL, &Dom, m_nSttRow, 30, 34, FALSE, TRUE);
}

///< 4.4 나) 단면제원
void CXLDesignOfMold::SectionJewon()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage			*pManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	m_nSttRow = 4;
	CString	sText;
	CString szSec;
	long	nGirdersu	= pDB->GetGirdersu();
	long	nG			= 0;
	double	dSta		= 0;
	double	dAs			= 0;
	double	dI33		= 0;
	double	dI22		= 0;
	double	dK			= 0;
	double	dAvs		= 0;
	double	dIv33		= 0;
	double	dIv22		= 0;
	double	dKv			= 0;
	double	dAcs		= 0;
	double	dIc33		= 0;
	double	dIc22		= 0;
	double	dKc			= 0;
	
	if(!pDB->IsTUGir())
	{
		pOut->SetXL(pXL, "합성전", m_nSttRow, "D", "G", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "합성후", m_nSttRow, "D", "G", 1, BLACK, 9, FALSE, TA_CENTER);
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		pXL->DeleteRowLineEnd();
	}
	m_nSttRow = 7+(pDB->IsTUGir()?1:0);

	long nSecCount = pManage->m_FrameSectionList.GetCount();
	double dProgress = 10.0 / (double)(nSecCount * nGirdersu);
	long nCount = 1;
	POSITION posSec;
	CLongArray narrGirder;
	
	narrGirder.Add(0); // 외측거더
	if(nGirdersu > 2)
		narrGirder.Add(1); // 내측거더

	long nSize = narrGirder.GetSize();
	for(long nIdx = 0; nIdx < nSize; ++nIdx)
	{
		long nG = narrGirder.GetAt(nIdx);

		posSec = pManage->m_FrameSectionList.GetHeadPosition();

		while(posSec)
		{
			m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.4 나) 단면제원", long(75+dProgress*nCount++));
			CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec); 
			if(pFrSec->m_nFlag==POS_GIRDER )
			{
				BOOL bExist = ModelCalc.GetPositionBySectionName(pFrSec->m_szName, nG, dSta, 0);
				if(bExist == FALSE)	continue;

				szSec = _T("");
				szSec = GetJointArrszByI33(pFrSec->m_dIx, 0, nG);
				if(szSec.IsEmpty()) continue;

				pXL->InsertCopyRowLine(4, 6+(pDB->IsTUGir()?1:0), m_nSttRow);
				
				ModelCalc.GetSectionCoefficient(nG, dSta, FALSE, dSta, 1);

				CPlateGirderApp *pGir = pDB->GetGirder(nG);
				dAs		= ModelCalc.GetMapValueSecCo("As_SUM");
				dI33	= ModelCalc.GetMapValueSecCo("I33");
				dI22	= ModelCalc.GetMapValueSecCo("I22");
				dK		= ModelCalc.GetMapValueSecCo("K");

				if(pDB->IsTUGir())
				{
					// 구속콘크리트 합성단면
					dAcs	= ModelCalc.GetMapValueSecCo("Avc");
					dIc33	= ModelCalc.GetMapValueSecCo("Ivc33");
					dIc22	= ModelCalc.GetMapValueSecCo("Ivc22");
					dKc		= ModelCalc.GetMapValueSecCo("Kvc");
					// 바닥판콘크리트 합성단면
					dAvs	= ModelCalc.GetMapValueSecCo("Avd");
					dIv33	= ModelCalc.GetMapValueSecCo("Ivd33");
					dIv22	= ModelCalc.GetMapValueSecCo("Ivd22");
					dKv		= ModelCalc.GetMapValueSecCo("Kvd");
				}
				else
				{
					// 합성후
					dAvs	= ModelCalc.GetMapValueSecCo("Av_SUM");
					dIv33	= ModelCalc.GetMapValueSecCo("Iv33");
					dIv22	= ModelCalc.GetMapValueSecCo("Iv22");
					dKv		= ModelCalc.GetMapValueSecCo("Kv");
				}

				long nSecRow = m_szArrSecJointNum.GetSize();
				// 합성전 | 강재단면
				pOut->SetXL(pXL, dAs,	m_nSttRow, "H", "K", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, dI33,	m_nSttRow, "L", "O", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, dI22,	m_nSttRow, "P", "S", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, dK,	m_nSttRow, "T", "W", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, "",	m_nSttRow, "X", "AA",1, BLACK, 9, FALSE, TA_CENTER, "", 1);

				if(pDB->IsTUGir())
				{
					// 구속콘크리트 합성단면
					pOut->SetXL(pXL, dAcs,	m_nSttRow, "H", "K", 0, RED,   9, FALSE, TA_CENTER, "", 1);
					pOut->SetXL(pXL, dIc33,	m_nSttRow, "L", "O", 0, RED,   9, FALSE, TA_CENTER, "", 1);
					pOut->SetXL(pXL, dIc22,	m_nSttRow, "P", "S", 0, RED,   9, FALSE, TA_CENTER, "", 1);
					pOut->SetXL(pXL, dKc,	m_nSttRow, "T", "W", 0, RED,   9, FALSE, TA_CENTER, "", 1);
					pOut->SetXL(pXL, "",	m_nSttRow, "X", "AA",1, BLACK, 9, FALSE, TA_CENTER, "", 1);
				}
				// 합성후 | 바닥판콘크리트 합성단면
				pOut->SetXL(pXL, dAvs,		m_nSttRow, "H", "K", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, dIv33,		m_nSttRow, "L", "O", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, dIv22,		m_nSttRow, "P", "S", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, dKv,		m_nSttRow, "T", "W", 0, RED,   9, FALSE, TA_CENTER, "", 1);
				pOut->SetXL(pXL, "",		m_nSttRow, "X", "AA",1, BLACK, 9, FALSE, TA_CENTER, "", 1);

				if(nSecRow>1)
				{
					pXL->InsertCopyRowLine(m_nSttRow, m_nSttRow+nSecRow-2, m_nSttRow);
					pOut->SetLineBoxText(pXL, "절점번호",	m_nSttRow, "D", "G", nSecRow, BLACK, 9, FALSE, TA_CENTER);
				}
				for(long n = 0; n < nSecRow; n++)
				{
					if(n== 0)
						pOut->SetLineBoxText(pXL, "",	m_nSttRow, "H", "AA", nSecRow, BLACK, 9, FALSE, TA_CENTER);
					sText.Format("%s", m_szArrSecJointNum.GetAt(n));
					pOut->SetXL(pXL, sText,				m_nSttRow, "H", "AA", 1, 1, 9, FALSE, TA_LEFT, "", 0);
				}
				nSecRow += 2+(pDB->IsTUGir()?1:0);
				m_nSttRow -= nSecRow;
				sText.Format("단면 - %s", pFrSec->m_szName);
				pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "A", "C", nSecRow, BLACK, 9, FALSE, TA_CENTER);
				m_nSttRow += nSecRow;
			}
		}
	}

	nCount = 0;
	posSec = pManage->m_FrameSectionList.GetHeadPosition();
	dProgress = 10.0 / (double)(nSecCount);

	long nIdxSec = 1;
	while(posSec)
	{
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("4.4 나) 단면제원", long(85+dProgress*nCount++));
		CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(posSec);

		if(pFrSec->m_nFlag==POS_CROSSBEAM)
		{
			nG = -1;
			ModelCalc.GetPositionBySectionName(pFrSec->m_szName, nG, dSta, 1);	
			if(nG==pDB->GetGirdersu()-1)
			{
				AfxMessageBox("요소정렬에 문제가 발생했습니다.");	// 단면값과 동일한 가로보가 없는 경우..
				continue;
			}
			else
				szSec = GetJointArrszByI33(pFrSec->m_dIx, 1);

			if(szSec == "") continue;
		
			CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxByStation(dSta, BX_CROSSBEAM_VBRACING);
			ModelCalc.GetSectionCoefficientCR(pBx);

			dAs		= ModelCalc.GetMapValueSecCoCR("Sum_As");
			dI33	= ModelCalc.GetMapValueSecCoCR("I33");
			dI22	= ModelCalc.GetMapValueSecCoCR("I22");
			dK		= ModelCalc.GetMapValueSecCoCR("K");

			sText.Format("가로보 - %d", nIdxSec++);
			long nSecRow = m_szArrSecJointNum.GetSize();
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "A", "C", nSecRow+1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "단면제원",	m_nSttRow, "D", "G", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dAs,			m_nSttRow, "H", "K", 1, RED,   9, FALSE, TA_CENTER, "", 1);
			pOut->SetLineBoxText(pXL, dI33,			m_nSttRow, "L", "O", 1, RED,   9, FALSE, TA_CENTER, "", 1);
			pOut->SetLineBoxText(pXL, dI22,			m_nSttRow, "P", "S", 1, RED,   9, FALSE, TA_CENTER, "", 1);
			pOut->SetLineBoxText(pXL, dK,			m_nSttRow, "T", "W", 1, RED,   9, FALSE, TA_CENTER, "", 1);
			pOut->SetLineBoxText(pXL, "",			m_nSttRow, "X", "AA", 1, BLACK, 9, FALSE, TA_CENTER, "", 1);
			pOut->SetXL(pXL,		  "",			m_nSttRow, "X", "AA");
			
			pOut->SetLineBoxText(pXL, "부재번호",	m_nSttRow, "D", "G", nSecRow, BLACK, 9, FALSE, TA_CENTER);
			for(long n = 0; n < nSecRow; n++)
			{
				if(n == 0)
					pOut->SetLineBoxText(pXL, "",	m_nSttRow, "H", "AA", nSecRow, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("%s", m_szArrSecJointNum.GetAt(n));
				pOut->SetXL(pXL, sText,				m_nSttRow, "H", "AA", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);
			}
		}
	}

	pXL->DeleteRowLine(4, 6+(pDB->IsTUGir()?1:0));
	pXL->DeleteRowLineEnd();
}

///< 4.4 다) 하중재하
void CXLDesignOfMold::Load()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	m_nSttRow = 2;
	CString	sText;
	
	if(pDB->IsTUGir())
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+2);
		m_nSttRow = 37;
	}
	else
	{
		m_nSttRow+=3;
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+6);
		m_nSttRow = 13;
		pOut->SetXL(pXL, "2) CASE 2 : 합성후 고정하중", m_nSttRow, "B", "B", 3);
		pOut->SetXL(pXL, "3) CASE 3 : 활하중",			m_nSttRow, "B", "B", 20);
		pOut->SetXL(pXL, "4) CASE 4 : 지점침하",		m_nSttRow, "B", "B", 1);
	}

	pOut->SetXL(pXL, pCalcData->m_JijumDownSize,		m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0);

	CString sCell;
	// 교각 교대 정보
	long nKsu = pDB->m_nQtyJigan;
	CMatrixStr DArrPier;
	DArrPier.Resize(nKsu + 1, 1);

	for(long nK = 0; nK < nKsu+1; nK++)
		DArrPier(nK,0) = pDB->m_strJijumName[nK];

	long nJijumSize = DArrPier.GetRows();
	long nNum = 1;
	long s = 0;
	CString sTitle, str;
	
	for(s = 0; s < nJijumSize; s++)
	{
		sTitle = DArrPier(s,0);
		sText.Format("%2d : %s 지점 침하", nNum++, sTitle); 
		pOut->SetXL(pXL, sText,			m_nSttRow, "D", "D", 1);
	}

	// 홀수 
	sTitle = "";
	for(s = 0; s < nJijumSize; s++)
	{
		if(s % 2 != 0) continue;
		sTitle += DArrPier(s,0);
		if(s != nJijumSize - 2 && s != nJijumSize - 1)
			sTitle += ", ";
	}
	sText.Format("%2d : %s 지점 침하", nNum++, sTitle);
	pOut->SetXL(pXL, sText,			m_nSttRow, "D", "D", 1);;

	// 짝수
	sTitle = "";
	for(s = 0; s < nJijumSize; s++)
	{
		if(s % 2 != 1) continue;
		sTitle += DArrPier(s,0);
		if(s != nJijumSize - 2 && s != nJijumSize - 1)
			sTitle += ", ";
	}
	sText.Format("%2d : %s 지점 침하", nNum++, sTitle); 
	pOut->SetXL(pXL, sText,		m_nSttRow, "D", "D", 1);

	pXL->DeleteRowLineEnd();
}

///< 4.4 라) 부재력 집계
void CXLDesignOfMold::EleForceTotal()
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutCalcXL	*pOut	= m_pAPlateOutCalcXL;
	CXLControl			*pXL	= m_pXL;
	
	pOut->ClearCellRef();

	if(!pDB->IsTUGir())
	{
		CString sText;
		m_nSttRow = 2;
		pOut->SetXL(pXL, "Steel",	m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Concrete",m_nSttRow, "D", "D", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=C3+D3+G3+H3+J3");
		pOut->SetXL(pXL, sText,		m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=C3+D3+G3+I3+K3");
		pOut->SetXL(pXL, sText,		m_nSttRow, "M", "M", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=C4+D4+G4+H4+J4");
		pOut->SetXL(pXL, sText,		m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=C4+D4+G4+I4+K4");
		pOut->SetXL(pXL, sText,		m_nSttRow, "M", "M", 1, BLACK, 9, FALSE, TA_CENTER);
	}

	m_nSttRow = 5;
	pOut->SetXL(pXL, "라) 부재력 집계",		m_nSttRow, "A", "A", 2);
	
	pOut->SetXL(pXL, "  1) 휨 모멘트",		m_nSttRow, "A", "A", 0);	
	EleForceSetTable(ELE_MOMENT3);		///< 휨 모멘트
	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("라) 부재력 집계 1) 휨 모멘트", m_pAPlateOutCalcStd->GetAPlateOutExcel()->GetProgress() + 5);
	SetLines(NEXT);

	pOut->SetXL(pXL, "  2) 전 단 력",		m_nSttRow, "A", "A", 0);
	EleForceSetTable(ELE_SHEAR_2);		///< 전단력
	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("라) 부재력 집계 2) 전 단 력", m_pAPlateOutCalcStd->GetAPlateOutExcel()->GetProgress() + 5);
	SetLines(NEXT);

	pOut->SetXL(pXL, "  3) 비 틀 림",		m_nSttRow, "A", "A", 0);
	EleForceSetTable(ELE_AXTORSN);		///< 비틀림
	m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress("라) 부재력 집계 3) 비 틀 림", m_pAPlateOutCalcStd->GetAPlateOutExcel()->GetProgress() + 5);
	SetLines(NEXT);

	if(!pDB->IsTUGir())
	{
		pXL->DeleteColSell(1, 5, m_nSttRow, 6);
		pXL->SetCellWidth(4,  6.56);
		pXL->SetCellWidth(6,  6);
		pXL->SetCellWidth(9,  6.56);
		pXL->SetCellWidth(10, 6.56);
		pXL->SetCellWidth(11, 3);
		pXL->SetCellWidth(12, 2);
		pXL->SetCellWidth(13, 2);

		CString sz;
		sz.Format("L%d", m_nSttRow);
		pXL->SetPrintArea("A1", sz);		
	}
	pXL->SetPageSetupCenterHori(TRUE);
	pXL->DeleteRowLine(1, 4);
	pXL->DeleteRowLineEnd();
}

///< 4.4 라) 부재력 집계 - Table
void CXLDesignOfMold::EleForceSetTable(long nForceType)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd		*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc	= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CAPlateOutExcel		*pOutXL		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcXL;
	CXLControl			*pXL		= m_pXL;

	CString	sText, szName1, szName2;
	
	long	nCurGir		= -1;
	long	nBujeNum	= 0;
	long	nJointNum	= 0;
	double	dPreSteel	= 0;
	double	dPreConc	= 0;
	double	dPreTurn	= 0;
	double	dPreSlab	= 0;
	double	dPos		= 0;
	double	dLivMax		= 0;
	double	dLivMin		= 0;
	double	dSetMax		= 0;
	double	dSetMin		= 0;

	CFEMManage	*pFemPre = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);		///< 합성전 고정하중
	CFEMManage	*pFemPos = pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);		///< 합성후 고정하중
	CFEMManage	*pFemLiv = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);			///< 활하중
	CFEMManage	*pFemSet = pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS);			///< 지점침하
	CFEMManage	*pFemCon = NULL;

	CResultArray	*pPreResult = pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);		///< 합성전 고정하중
	CResultArray	*pPosResult = pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);		///< 합성후 고정하중
	CResultArray	*pLivResult = pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);			///< 활하중
	CResultArray	*pSetResult = pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);			///< 지점침하
	CResultArray	*pConResult = NULL;
	if(pDB->IsTUGir())
	{
		pFemCon = pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);			///< 합성전-180도 터닝 + 슬래브(+가로보)
		pConResult = pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS);
	}

	for(long nE = 0; nE < pFemPre->GetElementSize(); nE++)
	{
		if(pOutXL->m_bAbort)	return;
		CElement *pEPre	= pFemPre->GetElement((unsigned short)nE);
		CElement *pECon = pFemCon ? pFemCon->GetElement((unsigned short)nE) : NULL;

		if(pEPre->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			CElementResult *pEPreResult	= pPreResult->m_EleResultArr.GetAt(nE);
			CElementResult *pEPosResult	= pPosResult->m_EleResultArr.GetAt(nE);
			CElementResult *pELivResult	= pLivResult->m_EleResultArr.GetAt(nE);
			CElementResult *pESetResult	= pSetResult->m_EleResultArr.GetAt(nE);
			CElementResult *pEConResult = pFemCon ? pConResult->m_EleResultArr.GetAt(nE) : NULL;

			if(pEPre->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT || pEPre->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT || pEPre->m_LongAttrib[ELEMENT_TYPE] & FRAME_STRINGER_ELEMENT)
			{
				pModelCalc->GetJijumNameOfJoint(pEPre->m_nJStt, szName1);
				pModelCalc->GetJijumNameOfJoint(pEPre->m_nJEnd, szName2);

				CJoint *pJoint = pFemPre->GetJoint(pEPre->m_nJStt);
				long	nG	= pJoint->m_LongAttrib[JOINT_GIR];

				if(nG != nCurGir)
				{// Girder가 바뀌면
					// Header 출력
					nCurGir = nG;
					
					SetLines(1);
					sText.Format("  < Girder %d >", nG+1);
					pOut->SetXL(pXL, sText,	m_nSttRow, "A", "A");
					pXL->InsertCopyRowLine(1, 4, m_nSttRow);
					SetLines(2);
				}
				else
					pXL->InsertCopyRowLine(m_nSttRow-2, m_nSttRow-1, m_nSttRow);

				nBujeNum	= pEPre->m_nIndex+1;
				nJointNum	= pEPre->m_nJStt+1;

				dPreSteel	= pEPreResult->m_dPreDeadSteel[nForceType][0];
				dPreConc	= pECon ? pEConResult->m_dPreDeadConf[nForceType][0] : pEPreResult->m_dPreDeadSlab[nForceType][0];
				dPreTurn	= pECon ? pEConResult->m_dPreDeadTurning[nForceType][0] : 0;
				dPreSlab	= pECon ? pEConResult->m_dPreDeadSlab[nForceType][0] : 0;
				dPos		= pEPosResult->m_dPostDead[nForceType][0];
				dLivMax		= pELivResult->m_dLiveMax[nForceType][0];
				dLivMin		= pELivResult->m_dLiveMin[nForceType][0];
				dSetMax		= pESetResult->m_dUnSettleMax[nForceType][0];
				dSetMin		= pESetResult->m_dUnSettleMin[nForceType][0];

				// 부재번호
				pOut->SetXL(pXL, nBujeNum,	m_nSttRow, "A", "A", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				// 절점번호
				pOut->SetXL(pXL, nJointNum,	m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				// 합성전 고정하중 - 강재자중	or Steel
				pOut->SetXL(pXL, dPreSteel,	m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
				// 합성전 고정하중 - 구속콘자중 or Concrete
				pOut->SetXL(pXL, dPreConc,	m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
				// 합성전 고정하중 - 180도 터닝
				pOut->SetXL(pXL, dPreTurn,	m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
				// 합성전 고정하중 - 가로보+바닥판
				pOut->SetXL(pXL, dPreSlab,	m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);

				///< 합성후 고정하중
				pOut->SetXL(pXL, dPos,		m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);

				///< 활하중
				pOut->SetXL(pXL, dLivMax,	m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dLivMin,	m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
				
				///< 지점침하
				pOut->SetXL(pXL, dSetMax,	m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dSetMin,	m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
				
				///< 비고
				pOut->SetXL(pXL, szName1,	m_nSttRow, "N", "N", 1, BLACK, 9, FALSE, TA_CENTER);


				nJointNum	= pEPre->m_nJEnd+1;

				dPreSteel	= pEPreResult->m_dPreDeadSteel[nForceType][FRAME_SEG_SIZE];
				dPreConc	= pECon ? pEConResult->m_dPreDeadConf[nForceType][FRAME_SEG_SIZE]	 :
									  pEPreResult->m_dPreDeadSlab[nForceType][FRAME_SEG_SIZE];
				dPreTurn	= pECon ? pEConResult->m_dPreDeadTurning[nForceType][FRAME_SEG_SIZE] : 0;
				dPreSlab	= pECon ? pEConResult->m_dPreDeadSlab[nForceType][FRAME_SEG_SIZE]	 : 0;
				dPos		= pEPosResult->m_dPostDead[nForceType][FRAME_SEG_SIZE];
				dLivMax		= pELivResult->m_dLiveMax[nForceType][FRAME_SEG_SIZE];
				dLivMin		= pELivResult->m_dLiveMin[nForceType][FRAME_SEG_SIZE];
				dSetMax		= pESetResult->m_dUnSettleMax[nForceType][FRAME_SEG_SIZE];
				dSetMin		= pESetResult->m_dUnSettleMin[nForceType][FRAME_SEG_SIZE];

				// 절점번호
				pOut->SetXL(pXL, nJointNum,	m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				// 합성전 고정하중 - 강재자중	or Steel
				pOut->SetXL(pXL, dPreSteel,	m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
				// 합성전 고정하중 - 구속콘자중 or Concrete
				pOut->SetXL(pXL, dPreConc,	m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
				// 합성전 고정하중 - 180도 터닝
				pOut->SetXL(pXL, dPreTurn,	m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
				// 합성전 고정하중 - 가로보+바닥판
				pOut->SetXL(pXL, dPreSlab,	m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);

				///< 합성후 고정하중
				pOut->SetXL(pXL, dPos,		m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);

				///< 활하중
				pOut->SetXL(pXL, dLivMax,	m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dLivMin,	m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
				
				///< 지점침하
				pOut->SetXL(pXL, dSetMax,	m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dSetMin,	m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
				
				///< 비고
				pOut->SetXL(pXL, szName2,	m_nSttRow, "N", "N", 1, BLACK, 9, FALSE, TA_CENTER);
			}
		}
	}
	SetLines(NEXT);
}

///< 4.4 마) 작용하중별 부재력 집계
void CXLDesignOfMold::EleForceLoadType()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM		= pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CFemStressCalc  	*pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	m_nSttRow = 5;
	pOut->ClearCellRef();

	CString	sText, szTemp, szName1, szName2;
	long	nGirdersu		= pDB->GetGirdersu();
	long	nSttElm			= 0;
	long	nEndElm			= 0;
	long	nSttJoint		= 0;
	long	nEndJoint		= 0;
	long	nLcMax			= 0;
	long	nLcMin			= 0;
	BOOL	bDB				= TRUE;
	BOOL	bFirst			= TRUE;
	
	pOut->SetXL(pXL, "  마) 작용하중별 부재력 집계",	m_nSttRow, "A", "A", 2);

	for(long nLoop=0; nLoop<2; nLoop++)
	{
		if(nLoop==0)
		{
			nLcMax = LC_DB_MAX;
			nLcMin = LC_DB_MIN;
			bDB	   = TRUE;
			sText.Format("  < DB 하중 >");
		}
		else
		{
			nLcMax = LC_DL_MAX;
			nLcMin = LC_DL_MIN;
			bDB	   = FALSE;
			sText.Format("  < DL 하중 >");
		}
		pOut->SetXL(pXL, sText,	m_nSttRow, "A", "A", 2);

		for(long nG = 0; nG < nGirdersu; nG++)
		{
			long nProgress = 0;
			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;

			if(bDB)	sText.Format("< DB 하중 부재력 집계 >");
			else	sText.Format("< DL 하중 부재력 집계 >");
			m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(sText, nProgress);
			
			sText.Format("  < Girder %d >", nG+1);
			pOut->SetXL(pXL, sText,									m_nSttRow, "A", "A");

			pXL->InsertCopyRowLine(1, 4, m_nSttRow);
			m_nSttRow+=2;

			pModelCalc->GetElemRangeByGirder(nG, nSttElm, nEndElm);

			bFirst = TRUE;
			for(long nElm = nSttElm; nElm <= nEndElm; nElm++)
			{
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				
				if(bDB)	sText.Format("< DB 하중 부재력 집계 GIRDER : %d  ELEMENT : %d >", nG+1, nElm+1);
				else	sText.Format("< DL 하중 부재력 집계 GIRDER : %d  ELEMENT : %d >", nG+1, nElm+1);
				double dStation = nSttElm-nEndElm;
				nProgress += (long)Round(100.0/fabs(dStation),0);
				m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(sText, nProgress);
		
				CElement *pElm = pFEM->GetElement((unsigned short)nElm);
				nSttJoint = pElm->m_nJStt;
				nEndJoint = pElm->m_nJEnd;
				pModelCalc->GetJijumNameOfJoint(nSttJoint, szName1);
				pModelCalc->GetJijumNameOfJoint(nEndJoint, szName2);
				// MAX
				double dMomentMaxStt	= pStressCalc->GetOneEleForce(ELE_MOMENT3, nLcMax, nElm, 0);
				double dMomentMaxEnd	= pStressCalc->GetOneEleForce(ELE_MOMENT3, nLcMax, nElm, 1);
				double dShearMaxStt		= pStressCalc->GetOneEleForce(ELE_SHEAR_2, nLcMax, nElm, 0);
				double dShearMaxEnd		= pStressCalc->GetOneEleForce(ELE_SHEAR_2, nLcMax, nElm, 1);
				// MIN	
				double dMomentMinStt= pStressCalc->GetOneEleForce(ELE_MOMENT3, nLcMin, nElm, 0);
				double dMomentMinEnd= pStressCalc->GetOneEleForce(ELE_MOMENT3, nLcMin, nElm, 1);
				double dShearMinStt	= pStressCalc->GetOneEleForce(ELE_SHEAR_2, nLcMin, nElm, 0);
				double dShearMinEnd	= pStressCalc->GetOneEleForce(ELE_SHEAR_2, nLcMin, nElm, 1);

				double dFatigueMaxSttM	= dMomentMaxStt - dMomentMinStt;
				double dFatigueMaxSttS	= dShearMaxStt  - dShearMinStt;
				double dFatigueMinEndM	= dMomentMaxEnd - dMomentMinEnd;
				double dFatigueMinEndS	= dShearMaxEnd  - dShearMinEnd;

				if(!bFirst)
					pXL->InsertCopyRowLine(m_nSttRow-2, m_nSttRow-1, m_nSttRow);

				sText.Format("%d", nElm+1);
				pOut->SetXL(pXL, sText,				m_nSttRow, "A", "A", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				sText.Format("%d", nSttJoint+1);
				pOut->SetXL(pXL, sText,				m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				pOut->SetXL(pXL, dMomentMaxStt,		m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dMomentMinStt,		m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dShearMaxStt,		m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dShearMinStt,		m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dFatigueMaxSttM,	m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dFatigueMaxSttS,	m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, szName1,			m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);
				
				sText.Format("%d", nEndJoint+1);
				pOut->SetXL(pXL, sText,				m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				pOut->SetXL(pXL, dMomentMaxEnd,		m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dMomentMinEnd,		m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dShearMaxEnd,		m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dShearMinEnd,		m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dFatigueMinEndM,	m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dFatigueMinEndS,	m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, szName2,			m_nSttRow, "I", "I", 1, BLACK, 9, FALSE, TA_CENTER);

				bFirst = FALSE;
			}
			SetLines(2);
		}
	}

	pXL->DeleteRowLine(1, 4);
	pXL->DeleteRowLineEnd();
}

///< 4.4 바) 부재력 요약
void CXLDesignOfMold::EleForceSummary()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();	
	CFemStressCalc  	*pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CFEMManage			*pFEM		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	m_nSttRow = 1;
	pOut->ClearCellRef();

	long	nForce		= 0;
	long	nSplicesu	= 0;
	long	nSecGir		= 0;
	double	dPreSteel	= 0;
	double	dPreConc	= 0;
	double	dPreTurn	= 0;
	double	dPreSlab	= 0;
	double	dPos		= 0;
	double	dLiv		= 0;
	double	dSet		= 0;
	double	dRatio		= 0;
	double	dLength		= 0;
	BOOL bHapSung = !pDB->GetNumHaiseokCombo();
	CString	sText;

	if(!bHapSung)
	{
		pOut->SetXL(pXL, "1차 고정하중", m_nSttRow, "B", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "2차",			 m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	}
	if(!pDB->IsTUGir())
	{
		m_nSttRow = 2;
		pOut->SetXL(pXL, "STEEL",		m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "CONCRETE",	m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	}
	m_nSttRow = 4;

	pOut->SetXL(pXL, "바) 부재력 요약",	m_nSttRow, "A", "A", 1);

	///< 거더 단면 개수를 구함. (가로보 단면 이전 Index값 구함.)
	CBOOLArray bArrExist;
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag==POS_GIRDER)
		{
			double dSta = 0;
			long nG = -1;
			BOOL bExist = ModelCalc.GetPositionBySectionName(pFrameSec->m_szName, nG, dSta, 0);
			bArrExist.Add(bExist);
			nSecGir++;
		}
	}

	///< 거더별 현장이음 개수를 구함.
	for (long nG = 0; nG < pDB->GetGirdersu(); nG++)   
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		nSplicesu = max(pGir->GetQtySplice(), nSplicesu);
	}

	// 거더 단면 순환 루프
	long nCount = 1;
	for (long nType = 0; nType < nSecGir + nSplicesu; nType++)   
	{
		if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
		if (nType == nSecGir)	m_nSttRow += 2;		///< 첫 현장이음 출력 공백줄.

		long nCheckNode = -1;
		long nCheckG	= -1;
		double dCheckSta = 0;
		if (nType < nSecGir)
		{
			if(bArrExist.GetAt(nType)==FALSE) continue;
			nCheckNode = ModelCalc.GetMaxForcePositionOfSection(nType+1, nCheckG, dCheckSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);   // 합계 최대인 위치 찾음.

			sText.Format("  %2d. 단면 - %d", nCount, nType + 1);				///< 거더단면 출력	
		}
		else
		{
			for(long n=0; n<pDB->GetGirdersu(); n++)
			{
				if (nType - nSecGir < pDB->GetGirder(n)->GetQtySplice())
				{
					nCheckNode = 0;
					break;
				}
			}			
			sText.Format("  %2d. 현장이음 - %d", nCount, nType + 1 - nSecGir);	///< 현장이음 출력
		}
		if(nCheckNode == -1) continue;

		//////////////////////////////////////////////////////////////////////////
		if (nType < nSecGir)
			sText.Format("  %2d. 단면 - %d", nCount, nType + 1);				///< 거더단면 출력	
		else
			sText.Format("  %2d. 현장이음 - %d", nCount, nType + 1 - nSecGir);	///< 현장이음 출력
		//
		nCount++;

		pOut->SetXL(pXL, sText, m_nSttRow, "A", "A");
	
		// 부재력 종류 순환 루프
		for (long nTempForce = 0; nTempForce < 3; nTempForce++) 
		{
			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
			switch (nTempForce) 
			{
			case 0 :
				nForce = ELE_MOMENT3;
				pOut->SetXL(pXL, "< BENDING MOMENT >", m_nSttRow, "A", "A");
				break;
			case 1 :
				nForce = ELE_SHEAR_2;
				pOut->SetXL(pXL, "< SHEAR FORCE >", m_nSttRow, "A", "A");
				break;
			case 2 :
				nForce = ELE_AXTORSN;
				pOut->SetXL(pXL, "< TORSION MOMENT >", m_nSttRow, "A", "A");
				break;
			}

			pXL->InsertCopyRowLine(1, 3, m_nSttRow);
			m_nSttRow+=2;

			// 전체 거더 순환 루프
			for (long nG = 0; nG < pDB->GetGirdersu(); nG++)  
			{
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				long	nNode		= 0;
				long	nElement	= 0;
				double	dSta		= 0;

				if(nG>0)
					pXL->InsertCopyRowLine(m_nSttRow-1, m_nSttRow-1, m_nSttRow);

				sText.Format("G%d", nG+1);				
				pOut->SetXL(pXL, sText, m_nSttRow, "A", "A", 0, BLACK, 9, FALSE, TA_CENTER);
				
				if (nType < nSecGir)
					nNode = ModelCalc.GetMaxForcePositionOfSection(nType+1, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);   // 합계 최대인 위치 찾음.
				else   
				{
					CPlateGirderApp *pGir = pDB->GetGirder(nG);
					if (nType - nSecGir < pGir->GetQtySplice())
					{
						dSta = pGir->GetBxBySpliceNo(nType - nSecGir)->GetStation();
						nNode = 0;
					}
					else
						nNode = -1;
				}					
				if (nNode < 0)		// 해당 단면 지정 거더에 없음.
				{  
					pOut->SetXL(pXL, "-", m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, "-", m_nSttRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);
					continue;
				}

				nElement = ModelCalc.GetElemNoAndRatio(nG, dSta, dRatio);
				nNode    = pFEM->GetElement((unsigned short)nElement)->m_nJStt;
				dLength  = ModelCalc.GetElemLength(nElement) * dRatio;

				// 합성전 고정하중 (1차)
				dPreSteel= Round(pStressCalc->GetElementForce(nForce, LC_PREV_S, nG, dSta, 0), 3);
				dPreSlab = Round (pStressCalc->GetElementForce(nForce, LC_PREV_C, nG, dSta, 0), 3);
				if(pDB->IsTUGir())
				{
					dPreConc = Round(pStressCalc->GetElementForce(nForce, LC_PREV_CONF, nG, dSta, 0), 3);
					dPreTurn = Round(pStressCalc->GetElementForce(nForce, LC_PREV_TURN, nG, dSta, 0), 3);
				}
				else
					swap(dPreConc, dPreSlab);
				// 합성후 고정하중 (2차)
				dPos	 = Round (pStressCalc->GetElementForce(nForce, LC_NEXT, nG, dSta, 0), 3);

				if (pStressCalc->GetElementForce(nForce, 0, nG, dSta, 4) > 0) 
				{
					dLiv = Round(pStressCalc->GetElementForce(nForce, LC_LIVE_MAX, nG, dSta, 0), 3);	// 활하중
					dSet = Round(pStressCalc->GetElementForce(nForce, LC_DISP_MAX, nG, dSta, 0), 3);	// 지점침하
				}
				else
				{
					dLiv = Round(pStressCalc->GetElementForce(nForce, LC_LIVE_MIN, nG, dSta, 0), 3);	// 활하중
					dSet = Round(pStressCalc->GetElementForce(nForce, LC_DISP_MIN, nG, dSta, 0), 3);	// 지점침하
				}

				// 위치
				sText.Format("  절점%4d 에서%5.2f M", nNode + 1, dLength);

				pOut->SetXL(pXL, dPreSteel,	m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dPreConc,	m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dPreTurn,	m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dPreSlab,	m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dPos,		m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dLiv,		m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dSet,		m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, sText,		m_nSttRow, "J", "J", 1, BLACK, 9, FALSE, TA_CENTER);
			}
			m_nSttRow++;
		}
		m_nSttRow++;
	}

	if(!pDB->IsTUGir())
	{
		pXL->DeleteColSell(1, 4, m_nSttRow, 5);
		pXL->SetCellWidth(7,  16);
		pXL->SetCellWidth(8,  2);
		pXL->SetCellWidth(9,  2);

		sText.Format("H%d", m_nSttRow);
		pXL->SetPrintArea("A1", sText);		
	}
	pXL->SetPageSetupCenterHori(TRUE);
	pXL->DeleteRowLine(1, 3);
	pXL->DeleteRowLineEnd();
}


///< 4.4 사) 반 력 집 계
void CXLDesignOfMold::EleForceReaction()
{	
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFEMManage			*pFEM		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	
	pOut->ClearCellRef();

	CString	sText, szName;
	char	nTitle		= 'A';
	long	nForce		= JOINT_REACTION;
	long	nJijumNum	= 0;
	long	nJointNum	= 0;
	double	dPreSteel	= 0;
	double	dPreConc	= 0;
	double	dPreTurn	= 0;
	double	dPreSlab	= 0;
	double	dPos		= 0;
	double	dLivMax		= 0;
	double	dLivMin		= 0;
	double	dSetMax		= 0;
	double	dSetMin		= 0;
	BOOL	bExist		= FALSE;
	BOOL	bHapSung	= pDB->IsHapsung();
	
	m_nSttRow = 0;

	if(!pDB->IsTUGir())
		pXL->DeleteRowLineDirect(m_nSttRow + 4, m_nSttRow + 7);
	else
		pXL->DeleteRowLineDirect(m_nSttRow + 1,	m_nSttRow + 4);

	if(!bHapSung)
	{
		m_nSttRow = 1;
		pOut->SetXL(pXL, "1차 고정하중",m_nSttRow, "F", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "2차",			m_nSttRow, "V", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
	}
	if(!pDB->IsTUGir())
	{
		m_nSttRow = 2;
		pOut->SetXL(pXL, "STEEL",		m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "CONCRETE",	m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=F3+J3+V3+Z3+AH3");
		pOut->SetXL(pXL, sText,			m_nSttRow,"AP","AS", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=F3+J3+V3+AD3+AL3");		
		pOut->SetXL(pXL, sText,			m_nSttRow,"AT","AW", 0, BLACK, 9, FALSE, TA_CENTER);
	}
	m_nSttRow = 4;

	pOut->SetXL(pXL, "사) 반 력 집 계",	m_nSttRow, "B", "B", 2);
	
	BOOL bTUGir = pDB->IsTUGir();

	for(long nLoadGir=0; nLoadGir < 2; nLoadGir++)
	{
		if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
		if(nLoadGir==0)	sText.Format("%c. 좌측 최대인 경우", nTitle++);
		else			sText.Format("%c. 우측 최대인 경우", nTitle++);
		pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1, BLACK);
		
		long nLoadTypesu = 4;
		if(nLoadGir == 1) nLoadTypesu = 6; // #APLATE-665, 보도 하중과 풍하중은 한 번만 나와야 한다.

		for(long nLoadType = 1; nLoadType <= nLoadTypesu; nLoadType++)
		{
			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
			
			pModelCalc->GetLiveLoadReaction(nLoadType, nLoadGir, 0, 0, bExist);   // 해당 LC 있는지 조사.
			if (!bExist)   
			{
				pModelCalc->GetLiveLoadReaction(nLoadType, nLoadGir, 1, 0, bExist);   // 해당 LC 있는지 조사.				
				if (!bExist) continue;
			}
			
			CString strCell = "C";
			switch (nLoadType)
			{
			case 1 :	sText.Format("1) DB 하중");				break;
			case 2 :	sText.Format("2) DL 하중");				break;
			case 3 :	sText.Format("3) TANK 하중");			break;
			case 4 :	sText.Format("4) TANK TRAILER 하중");	break;
			case 5 :	sText.Format("%c. 보도 하중", nTitle++); strCell = "B"; break;
			case 6 :	sText.Format("%c. 풍하중", nTitle++);	 strCell = "B"; break;
			}
			pOut->SetXL(pXL, sText, m_nSttRow, strCell, strCell);

			long nMaxLanesu = 20;
			for(long nLane = 0; nLane <= nMaxLanesu; nLane++)
			{
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;

				pModelCalc->GetLiveLoadReaction(nLoadType, nLoadGir, nLane, 0, bExist);   // 해당 차선 있는지 조사.
				if(!bExist) continue;

				if(nLoadType <= 2)
				{
					sText.Format("%c. %d차선 재하", nLane+'a', nLane+1);
					pOut->SetXL(pXL, sText, m_nSttRow, "D", "D");
				}
				for(long nK = 0; nK <=  pDB->m_nQtyJigan; nK++)
				{										
					sText.Format("< %s >", pDB->m_strJijumName[nK]);
					pOut->SetXL(pXL, sText, m_nSttRow, "B", "B");
					
					pXL->InsertCopyRowLine(1, 3, m_nSttRow);

					switch (nLoadType)
					{
						case 1 :	sText.Format("DB 하중");			break;
						case 2 :	sText.Format("DL 하중");			break;
						case 3 :	sText.Format("TANK 하중");			break;
						case 4 :	sText.Format("TANK TRAILER 하중");	break;
						case 5 :	sText.Format("보도 하중");			break;
						case 6 :	sText.Format("풍 하 중");			break;
					}
					pOut->SetXL(pXL, sText, m_nSttRow, !bTUGir ? "Z" : "R",  !bTUGir ? "AG" : "Y",  1, BLACK, 9, FALSE, TA_CENTER);
					
					if(nLoadType>5)
					{
						pOut->SetXL(pXL, "좌->우",	m_nSttRow, !bTUGir ? "Z" : "R",  !bTUGir ? "AG" : "Y",  0, BLACK, 9, FALSE, TA_CENTER);
						pOut->SetXL(pXL, "우->좌",	m_nSttRow, !bTUGir ? "AH" : "Z",  !bTUGir ? "AO" : "AG",  0, BLACK, 9, FALSE, TA_CENTER);
					}
					m_nSttRow++;
					BOOL bFirst = TRUE;

					for(int nJ = 0; nJ < pFEM->GetJointSize(); nJ++)
					{
						CJoint* pJ = pFEM->GetJoint(nJ);
						if(!(pJ->m_UZ))	continue;
						if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;

						CPlateBasicIndex *pBx = pCalcStd->GetBxByJoint(pJ);

						long nJijum = pBx->GetNumberJijum();

						if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && nJijum == nK)
						{
							CJoint *pPreJ, *pPosJ, *pSetJ, *pConJ = NULL;
							CJointResult *pPreJResult, *pPosJResult, *pSetJResult, *pConJResult = NULL;
							pPreJ = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
							pPosJ = pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
							pSetJ = pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS)->GetJoint(pJ->m_nIdx);

							pPreJResult = pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
							pPosJResult = pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
							pSetJResult = pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);

							if(pDB->IsTUGir())
							{
								pConJ = pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
								pConJResult = pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
							}

							if(!bFirst)
								pXL->InsertCopyRowLine(m_nSttRow-1, m_nSttRow-1, m_nSttRow);							

							nJijumNum	= pJ->m_LongAttrib[JOINT_GIR]+1;
							nJointNum	= pJ->m_nIdx+1;

							dPreSteel	= pPreJResult->m_vPreDeadSteel[nForce].z;
							dPreConc	= pConJ ? pConJResult->m_vPreDeadConf[nForce].z		: pPreJResult->m_vPreDeadSlab[nForce].z;
							dPreTurn	= pConJ ? pConJResult->m_vPreDeadTurning[nForce].z	: 0;
							dPreSlab	= pConJ ? pConJResult->m_vPreDeadSlab[nForce].z		: 0;
							dPos		= pPosJResult->m_vPostDead[nForce].z;
							dLivMax		= pModelCalc->GetLiveLoadReaction(nLoadType, nLoadGir, nLane, nJ, bExist);
							dLivMin		= pModelCalc->GetLiveLoadReaction(-nLoadType, nLoadGir, nLane, nJ, bExist);
							dSetMax		= pSetJResult->m_vUnSettleMax[nForce].z;
							dSetMin		= pSetJResult->m_vUnSettleMin[nForce].z;

							sText.Format("G%d", nJijumNum);
							pOut->SetXL(pXL, sText,		m_nSttRow, "B", "C",	0, BLACK, 9, FALSE, TA_CENTER);
							pOut->SetXL(pXL, nJointNum,	m_nSttRow, "D", "E",	0, BLACK, 9, FALSE, TA_CENTER, "", 0);
							
							//합성전 고정하중
							if(!pDB->IsTUGir())
							{
								pOut->SetXL(pXL, dPreSteel,	m_nSttRow, "F", "I",	0, BLACK, 9, FALSE, TA_CENTER);
								pOut->SetXL(pXL, dPreConc,	m_nSttRow, "J", "M",	0, BLACK, 9, FALSE, TA_CENTER);
								pOut->SetXL(pXL, dPreTurn,	m_nSttRow, "N", "Q",	0, BLACK, 9, FALSE, TA_CENTER);
							}
							else if(!pDB->IsTUContinuousSpan())
								pOut->SetXL(pXL, dPreSteel+dPreConc+dPreTurn,	m_nSttRow, "F", "I",	0, BLACK, 9, FALSE, TA_CENTER);
							else
							{
								dPreSteel	= pPreJResult->m_vPreDeadSteelConf[nForce].z;
								dPreSlab	+= dPreConc;
								pOut->SetXL(pXL, dPreSteel,	m_nSttRow, "F", "I",	0, BLACK, 9, FALSE, TA_CENTER);
							}

							//가로보바닥
							pOut->SetXL(pXL, dPreSlab,	m_nSttRow, !bTUGir ? "R" : "J", !bTUGir ? "U" : "M",	0, BLACK, 9, FALSE, TA_CENTER);

							//고정하중
							pOut->SetXL(pXL, dPos,		m_nSttRow, !bTUGir ? "V" : "N", !bTUGir ? "Y" : "Q",	0, BLACK, 9, FALSE, TA_CENTER);
							
							//DB 하중 MAX
							pOut->SetXL(pXL, dLivMax,	m_nSttRow, !bTUGir ? "Z" : "R", !bTUGir ? "AC" : "U",	0, BLACK, 9, FALSE, TA_CENTER);
							
							//DB 하중 MIN
							pOut->SetXL(pXL, dLivMin,	m_nSttRow, !bTUGir ? "AD" : "V", !bTUGir ? "AG" : "Y",	0, BLACK, 9, FALSE, TA_CENTER);
							
							//지점침하 MAX
							pOut->SetXL(pXL, dSetMax,	m_nSttRow, !bTUGir ? "AH" : "Z", !bTUGir ? "AK" : "AC",	0, BLACK, 9, FALSE, TA_CENTER);
							
							//지점침하 MIN
							pOut->SetXL(pXL, dSetMin,	m_nSttRow, !bTUGir ? "AL" : "AD", !bTUGir ? "AO" : "AG",	0, BLACK, 9, FALSE, TA_CENTER);
							
							if(!pDB->IsTUGir())
							{
								sText.Format("=F%d+J%d+V%d+Z%d+AH%d", m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow);
								pOut->SetXL(pXL, sText,	m_nSttRow,	"AP", "AS",	0,	BLACK,	9,	FALSE,	TA_CENTER);
								
								sText.Format("=F%d+J%d+V%d+AD%d+AL%d", m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow,m_nSttRow);
								pOut->SetXL(pXL, sText,	m_nSttRow,	"AT", "AW",	1,	BLACK,	9,	FALSE,	TA_CENTER);
							}
							else
							{
								sText.Format("=F%d+J%d+N%d+R%d+Z%d", m_nSttRow,	m_nSttRow,	m_nSttRow,	m_nSttRow, m_nSttRow);
								pOut->SetXL(pXL, sText,	m_nSttRow,	"AH", "AK",	0,	BLACK,	9,	FALSE,	TA_CENTER);

								sText.Format("=F%d+J%d+N%d+V%d+AD%d", m_nSttRow,	m_nSttRow,	m_nSttRow,	m_nSttRow, m_nSttRow);
								pOut->SetXL(pXL, sText,	m_nSttRow,	"AL", "AO",	1,	BLACK,	9,	FALSE,	TA_CENTER);

							}
							
							bFirst = FALSE;
						}
					}
				}
				m_nSttRow++;
				if(nLoadType >= 3)  // 탱크와 트레일러는 차선이 없음.
					break;
			}
			m_nSttRow++;
		}
	}

	long nRow = 1;
	pOut->SetXL(pXL, "활 하 중",	nRow, !bTUGir ? "Z" : "R", !bTUGir ? "AG" : "Y", 0, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetLineBoxText(pXL, "",	nRow, !bTUGir ? "AX" : "AP", !bTUGir ? "AY" : "AQ", 2, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "사 용",		nRow,	!bTUGir ? "AX" : "AP", !bTUGir ? "AY" : "AQ", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "SHOE",		nRow,	!bTUGir ? "AX" : "AP", !bTUGir ? "AY" : "AQ", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",	nRow,	!bTUGir ? "AX" : "AP", !bTUGir ? "AY" : "AQ", 1, BLACK, 9, FALSE, TA_CENTER);
	
	sText.Format("%c. 최대 반력 집계", nTitle++);
	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1, BLACK);

	for(long nK = 0; nK <= pDB->m_nQtyJigan; nK++)
	{
		sText.Format("< %s >", pDB->m_strJijumName[nK]);
		pOut->SetXL(pXL, sText,								m_nSttRow, "B",  "B",  1, BLACK);

		pXL->InsertCopyRowLine(1, 3, m_nSttRow);
		m_nSttRow+=2;
		BOOL bFirst = TRUE;

		for(int nJ = 0; nJ < pFEM->GetJointSize(); nJ++)
		{
			CJoint* pJ = pFEM->GetJoint(nJ);
			if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
			CPlateBasicIndex	*pBx	= pCalcStd->GetBxByJoint(pJ);
			long	nJijum = pBx->GetNumberJijum();

			if(!(pJ->m_UZ))	continue;

			if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && nJijum == nK)
			{
				CShoe	*pShoe =  pDB->GetGirder(pJ->m_LongAttrib[JOINT_GIR])->GetShoeByJijumNo(nJijum);
				CJoint	*pPreJ, *pPosJ, *pLivJ, *pSetJ, *pConJ = NULL;
				CJointResult *pPreJResult, *pPosJResult, *pLivJResult, *pSetJResult, *pConJResult = NULL;

				pPreJ = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
				pPosJ = pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
				pSetJ = pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS)->GetJoint(pJ->m_nIdx);
				pLivJ = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS)->GetJoint(pJ->m_nIdx);

				pPreJResult = pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				pPosJResult = pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				pSetJResult = pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				pLivJResult = pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);

				if(pDB->IsTUGir())
				{
					pConJ = pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
					pConJResult = pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				}

				if(!bFirst)
					pXL->InsertCopyRowLine(m_nSttRow-1, m_nSttRow-1, m_nSttRow);

				nJijumNum	= pJ->m_LongAttrib[JOINT_GIR]+1;
				nJointNum	= pJ->m_nIdx+1;

				dPreSteel	= pPreJResult->m_vPreDeadSteel[nForce].z;
				dPreConc	= pConJ ? pConJResult->m_vPreDeadConf[nForce].z		: pPreJResult->m_vPreDeadSlab[nForce].z;
				dPreTurn	= pConJ ? pConJResult->m_vPreDeadTurning[nForce].z	: 0;
				dPreSlab	= pConJ ? pConJResult->m_vPreDeadSlab[nForce].z		: pPreJResult->m_vPreDeadSlab[nForce].z;
				dPos		= pPosJResult->m_vPostDead[nForce].z;
				dLivMax		= pLivJResult->m_vLiveMax[nForce].z;
				dLivMin		= pLivJResult->m_vLiveMin[nForce].z;
				dSetMax		= pSetJResult->m_vUnSettleMax[nForce].z;
				dSetMin		= pSetJResult->m_vUnSettleMin[nForce].z;

				sText.Format("G%d", nJijumNum);
				pOut->SetXL(pXL, sText,		m_nSttRow, "B", "C",	0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, nJointNum,	m_nSttRow, "D", "E",	0, BLACK, 9, FALSE, TA_CENTER, "", 0);
				
				if(!bTUGir)
				{
					pOut->SetXL(pXL, dPreSteel,	m_nSttRow, "F", "I",	0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, dPreConc,	m_nSttRow, "J", "M",	0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, dPreTurn,	m_nSttRow, "N", "Q",	0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, dPreSlab,	m_nSttRow, "R", "U",	0, BLACK, 9, FALSE, TA_CENTER);
				}
				else if(!pDB->IsTUContinuousSpan())
				{
					pOut->SetXL(pXL, dPreSteel + dPreConc + dPreTurn,	m_nSttRow, "F", "I",	0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, dPreSlab,							m_nSttRow, "J", "M",	0, BLACK, 9, FALSE, TA_CENTER);
				}
				else
				{
					dPreSteel	= pPreJResult->m_vPreDeadSteelConf[nForce].z;
					dPreSlab	+= dPreConc;
					pOut->SetXL(pXL, dPreSteel,							m_nSttRow, "F", "I",	0, BLACK, 9, FALSE, TA_CENTER);
					pOut->SetXL(pXL, dPreSlab,							m_nSttRow, "J", "M",	0, BLACK, 9, FALSE, TA_CENTER);
				}
				pOut->SetXL(pXL, dPos,		m_nSttRow, !bTUGir ? "V" : "N",		!bTUGir ? "Y" : "Q",		0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dLivMax,	m_nSttRow, !bTUGir ? "Z" : "R",		!bTUGir ? "AC" : "U",		0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dLivMin,	m_nSttRow, !bTUGir ? "AD" : "V",	!bTUGir ? "AG" : "Y",		0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dSetMax,	m_nSttRow, !bTUGir ? "AH" : "Z",	!bTUGir ? "AK" : "AC",		0, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetXL(pXL, dSetMin,	m_nSttRow, !bTUGir ? "AL" : "AD",	!bTUGir ? "AO" : "AG",	0, BLACK, 9, FALSE, TA_CENTER);

				sText.Format("%3d", pShoe->m_dForceShoe * 10);
				pOut->SetXL(pXL, sText,		m_nSttRow, !bTUGir ? "AX" : "AP", !bTUGir ? "AY" : "AQ", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

				bFirst = FALSE;
			}
		}
	}

	if(!pDB->IsTUGir())
	{
		pXL->DeleteColSell(1, 14, m_nSttRow, 21);

		sText.Format("AQ%d", m_nSttRow);
		pXL->SetPrintArea("A1", sText);		
	}
	pXL->SetPageSetupCenterHori(TRUE);
	pXL->DeleteRowLine(1, 3);
	pXL->DeleteRowLineEnd();
}

///< 4.5 뒴비틀림
void CXLDesignOfMold::WarpingTorsion()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc      StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	pOut->ClearCellRef();
	CString	sText;
	///< 전체 단면 중 순수비틀림 상수비가 가장 작은 단면 찾기
	long	nIdx			= 0;
	long	nG				= 0;
	long	nSectionNo		= 0;
	long	nCurSectionType	= 0;
	double	dSta			= 0;
	double	dAlphaRatio		= 100000;
	double	dAlphaRatio_Old	= 0;
	double	dEst			= pCalcData->DESIGN_MATERIAL.m_dEst;///< 탄성계수
	double	dShearEs		= pCalcData->DESIGN_MATERIAL.m_dShearEs;
	long	nGCur			= 0;
	double	dStaCur			= 0;
	double dStaSec = 0;
	long nPosSec = 0, nCheckSec = 0;

	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		long	nSec	= atoi(pFrameSec->m_szName);
		nG   = -1;
		dSta = 0;
		ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
		ModelCalc.GetSectionCoefficient(nG, dSta);
	
		dAlphaRatio_Old	= ModelCalc.GetMapValueSecCo("WarpingAlpha");

		if(dAlphaRatio > dAlphaRatio_Old)
		{
			dAlphaRatio		= dAlphaRatio_Old;
			nCurSectionType	= nSectionNo;
			nGCur			= nG;
			dStaCur			= dSta;
			nCheckSec = nSec;
		}
		nSectionNo++;
	}

	nG	 = nGCur;
	dSta = dStaCur;
	dSta = StressCalc.GetStationCheckPos(nG, dSta);

	CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);

	double dWidthUp = 0, dThickUp = 0, dWidthDn = 0, dThickDn = 0, dHeightWeb = 0, dThickWeb = 0;
	double	dWarpingH	= dHeightWeb+(dThickUp+dThickDn)/2;	///< 상단중심에서 하단플래지 중심까지
	double	dWarpingI1	= dThickUp*dWidthUp*dWidthUp*dWidthUp/12;	///<
	double	dWarpingI2	= dThickDn*dWidthDn*dWidthDn*dWidthDn/12;	///<
	double	dWarpingIw	= (dWarpingI1*dWarpingI2)/(dWarpingI1+dWarpingI2)*dWarpingH*dWarpingH;///< 
	double	dK			= (dWidthUp*pow(dThickUp, 3)+dWidthDn*pow(dThickDn, 3)+dHeightWeb*pow(dThickWeb, 3))/3;///< 비틀림상수
	double	dLengthJigan= (pBridgeApp->m_dLengthJigan[pGir->GetNumberJiganByStation(dSta)]);
	
	if(nG != -1)
		dStaSec	= ModelCalc.GetStationSecJewon(nCheckSec, nG, dSta, nPosSec);
	ModelCalc.GetSectionCoefficient(nG, dStaSec, FALSE, dStaSec, nPosSec);
	
	dWidthUp = ModelCalc.GetMapValueSecCo("FLANGE_W_UP");
	dThickUp = ModelCalc.GetMapValueSecCo("FLANGE_T_UP");
	dWidthDn = ModelCalc.GetMapValueSecCo("FLANGE_W_DN");
	dThickDn = ModelCalc.GetMapValueSecCo("FLANGE_T_DN");
	dHeightWeb = ModelCalc.GetMapValueSecCo("WEB_H");
	dThickWeb = ModelCalc.GetMapValueSecCo("WEB_T");
	dAlphaRatio	= ModelCalc.GetMapValueSecCo("WarpingAlpha");

	m_nSttRow = 4;
	sText.Format("%d", nCurSectionType+1);
	pOut->SetXL(pXL, sText,			m_nSttRow, "I", "I", 1, RED,   9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dWidthUp,		m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dThickUp,		m_nSttRow, "M", "O", 1, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dWidthDn,		m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dThickDn,		m_nSttRow, "M", "O", 1, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dHeightWeb,	m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dThickWeb,		m_nSttRow, "M", "O", 1, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dWarpingH,		m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dLengthJigan,	m_nSttRow, "M", "O",13, RED,   9, FALSE, TA_CENTER, "", 1);

	pOut->SetXL(pXL, dEst,			m_nSttRow, "V", "X", 1, RED,   9, FALSE, TA_CENTER, "EST", 0);
	pOut->SetXL(pXL, dShearEs,		m_nSttRow, "W", "Y", 6, RED,   9, FALSE, TA_CENTER, "SHEAR_ES", 0);

	nIdx	= 1;
	CString szWidthUp, szThickUp, szWidthDn, szThickDn, szHeightWeb, szThickWeb, szLengthJigan,
			szWarpingH, szWarpingIw, szK;

	pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		nG   = -1;
		dSta = 0;
		long	nSec	= atoi(pFrameSec->m_szName);
		ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
		
		if(nG != -1)
			dStaSec	= ModelCalc.GetStationSecJewon(nSec, nG, dSta, nPosSec);

		CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
	
		ModelCalc.GetSectionCoefficient(nG, dStaSec, FALSE, dStaSec, nPosSec);

		dWidthUp = ModelCalc.GetMapValueSecCo("FLANGE_W_UP");
		dThickUp = ModelCalc.GetMapValueSecCo("FLANGE_T_UP");
		dWidthDn = ModelCalc.GetMapValueSecCo("FLANGE_W_DN");
		dThickDn = ModelCalc.GetMapValueSecCo("FLANGE_T_DN");
		dHeightWeb = ModelCalc.GetMapValueSecCo("WEB_H");
		dThickWeb = ModelCalc.GetMapValueSecCo("WEB_T");

		dLengthJigan= (pBridgeApp->m_dLengthJigan[pGir->GetNumberJiganByStation(dSta)]);
		dWarpingIw	= ModelCalc.GetMapValueSecCo("WarpingIw")/1000000;
		dK			= ModelCalc.GetMapValueSecCo("K")/10000;
		dAlphaRatio	= ModelCalc.GetMapValueSecCo("WarpingAlpha");

		if(nIdx != 1)
			pXL->InsertCopyRowLine(m_nSttRow-1, m_nSttRow-1, m_nSttRow);
		
		sText.Format("단면-%d", nIdx++);
		pOut->SetXL(pXL, sText,			m_nSttRow, "A", "B", 0, RED,   9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dWidthUp,		m_nSttRow, "C", "D", 0, RED,   9, FALSE, TA_CENTER, szWidthUp,1);
		pOut->SetXL(pXL, dThickUp,		m_nSttRow, "E", "F", 0, RED,   9, FALSE, TA_CENTER, szThickUp,1);
		pOut->SetXL(pXL, dWidthDn,		m_nSttRow, "G", "H", 0, RED,   9, FALSE, TA_CENTER, szWidthDn,1);
		pOut->SetXL(pXL, dThickDn,		m_nSttRow, "I", "J", 0, RED,   9, FALSE, TA_CENTER, szThickDn,1);
		pOut->SetXL(pXL, dHeightWeb,	m_nSttRow, "K", "L", 0, RED,   9, FALSE, TA_CENTER, szHeightWeb,1);
		pOut->SetXL(pXL, dThickWeb,		m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, szThickWeb,1);
		pOut->SetXL(pXL, dLengthJigan,	m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, szLengthJigan, 0);

		sText.Format("=O%d*SQRT(%s*W%d/(%s*S%d))", m_nSttRow, pOut->GetCellRef("SHEAR_ES"), m_nSttRow, pOut->GetCellRef("EST"), m_nSttRow);
		pOut->SetXL(pXL, sText,			m_nSttRow, "Z","AA", 1, BLACK, 9, FALSE, TA_CENTER);
	}
}

///< 4.6 2차응력 - 영향계수 K
void CXLDesignOfMold::InfluenceK()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM		= NULL;
	CXLControl			*pXL			= m_pXL;
	CARoadOptionStd		*pOptStd		= pDataManage->GetOptionStd();	

	m_nSttRow = 48;
	pOut->ClearCellRef();

	CString	sText;
	long nGirdersu	= pBridgeApp->GetGirdersu();
	long nEleSize	= 0;
	long nCellIdx	= 0;

	for(long nG = 0; nG < nGirdersu; nG++)
	{
		pFEM	= pCalcStd->GetInfluenceFemData(nG);
		
		nEleSize	= pFEM->GetElementSize();

		sText.Format("< 거더 - %d >", nG+1);
		pOut->SetXL(pXL, sText,											m_nSttRow, "A", "A", 1, BLUE);

		///< 그림삽입
		CDomyun	Dom;
		Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom.ClearEtt(TRUE);
		pOptStd->SetEnvType(&Dom, HCAD_DIML);		

		CDomyun DomTemp;
		DomTemp.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		DrawInfK(&DomTemp, nG);
		DomTemp.SetCalcExtRect();
		CDRect	BoundRect	= DomTemp.GetExtRect();
		double	dDiagonalL	= sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
		double	dScale		= 250*(dDiagonalL/60000);
		Dom.SetScaleDim(dScale);
		DrawInfK(&Dom, nG);
		Dom.RedrawByHVScale(1.8, TRUE);
		Dom.SetCalcExtRect();

		pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow);
		SetLines(NEXT);
		
		if(nEleSize < 10)	nCellIdx = 12 - nEleSize+1;
		else				nCellIdx = 1;

		pOut->SetLineBoxText(pXL, "위치",			m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1);
		SetLines(NEXT);
		pOut->SetLineBoxText(pXL, "K값",			m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1);
		SetLines(-1);

		for(long nE=0; nE< nEleSize; nE++)
		{
			CElement			*pElem		= pFEM->GetElement((unsigned short)nE);

			long nIndex = pFEM->GetLoadCaseIndex("LC1");
			CResultForceElement *pResult = pElem->GetResultForce(nIndex);

			nCellIdx += 2;
			if(nCellIdx > 23)
			{
				nCellIdx = 3;
				SetLines(2);
				pOut->SetLineBoxText(pXL, "위치",								m_nSttRow, "B", "C", 1);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "K값",								m_nSttRow, "B", "C", 1);
				SetLines(-1);
			}
			if(pResult)
			{
				sText.Format("절점 %d", pFEM->GetElement((unsigned short)nE)->m_nJStt+1);
				pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				sText.Format("%.3f", pResult->m_ResultForceElement[0]->m_dForceOnElement[ELE_MOMENT2]);
				pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(-1);
			}
			if(nE == pFEM->GetElementSize()-1)
			{
				nCellIdx += 2;
				sText.Format("절점 %d", pFEM->GetElement((unsigned short)nE)->m_nJEnd+1);
				pOut->SetLineBoxText(pXL, sText, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, 1.0, m_nSttRow, m_strCell[nCellIdx], m_strCell[nCellIdx+1], 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(-1);
				break;
			}
		}
		SetLines(2);
	}

	SetLines(NEXT);
	pXL->InsertCopyRowLine(12, 47, m_nSttRow);
	pXL->DeleteRowLine(12, 47);
	pXL->DeleteRowLineEnd();
}

void CXLDesignOfMold::DrawInfK(CDomyun* pDom, long nG)
{
	CDataManage		*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CARoadOptionStd	*pOptStd		= pDataManage->GetOptionStd();	
	CPlateBridgeApp	*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CPlateGirderApp	*pGir			= pDB->GetGirder(nG);
	CAPlateCalcStd	*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage		*pFEM		= pCalcStd->GetInfluenceFemData(nG);
	CFemModelingCalc ModelCalc(pDataManage, pCalcStd);

	long nNodesu = pFEM->GetJointSize();

	CPlateBasicIndex* pBxStt = NULL;
	CPlateBasicIndex* pBxEnd = NULL;
	CPlateBxFinder Finder(pGir);

	if (!pBxStt || !pBxEnd)  
	{
		pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
	}

	double staStt = pBxStt->GetStation();
	double staEnd = pBxEnd->GetStation();
	double staGirStt = pGir->GetSSS(OFF_STT_SHOE);
	double sta=0;
	double BaseY = 0.0;
	double dPreSta=0;
	double Len = staEnd - staStt;
	if(pDB->m_nQtyJigan == 1) Len = Len*3;

	CDDWordArray DimSta, LenSta;
	CPlateBasicIndex *pBx = NULL;
	CString cs="", cs2="";
	
	CDomyun Dom(pDom);
	double dSeparatorL = Dom.Always(1.5);

	for(long i=0; i<2; i++)	
	{
		if(i==1) 		BaseY = -Len*0.3;
		// 기준선 그림.
		pOptStd->SetEnvType(&Dom, HCAD_MIS2);
		Dom.MoveTo(staStt, BaseY);
		Dom.LineTo(staEnd, BaseY);

		// 절점번호 그림.
		Dom.SetTextHeight(4);
		Dom.SetTextAlignHorz(TA_CENTER);

		for(long nNodeIdx = 0; nNodeIdx < nNodesu; nNodeIdx++)
		{
			CJoint *pJ = pFEM->GetJoint((unsigned short)nNodeIdx);
			sta = pJ->m_vPointReal.x;

			Dom.MoveTo(sta, BaseY + dSeparatorL);
			Dom.LineTo(sta, BaseY - dSeparatorL);
			CDPoint P1(sta, BaseY + dSeparatorL*1.2);
			cs.Format("%d", nNodeIdx + 1);

			if(pJ->IsRestraintJoint()) 
				Dom.TextOut (P1, cs);
			if(pJ->IsRestraintJoint())	Dom.Triangle(sta, BaseY, 5);  // 지점 Symbol 그림.
		}
		
	}

	//모멘트하중표시
	double radius = Len*0.02;
	Dom.SetScaleDim(pDom->GetScaleDim()*1.3);
	pOptStd->SetEnvType(&Dom, HCAD_SYMB);
	Dom.SetTextHeight(7);
	Dom.TextOut(staStt, Len*0.05, "1N·m");
	Dom.TextOut(staEnd, Len*0.05, "1N·m");

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	double dRH = 0.03;
	Dom.Arc(staStt, Len*dRH, radius, 0, 180);
	Dom.Arc(staEnd, Len*dRH, radius, 0, 180);
	Dom.LineTo(staStt+radius, Len*dRH, staStt+radius+radius*0.15, Len*dRH+radius*0.4);
	Dom.LineTo(staStt+radius, Len*dRH, staStt+radius-radius*0.3, Len*dRH+radius*0.3);
	Dom.LineTo(staEnd-radius, Len*dRH, staEnd-radius-radius*0.15, Len*dRH+radius*0.4);
	Dom.LineTo(staEnd-radius, Len*dRH, staEnd-radius+radius*0.3, Len*dRH+radius*0.3);

	*pDom << Dom;

	//모멘트 Diagram
	CDomyun DomBM(pDom);		
	DomBM.SetScaleDim(pDom->GetScaleDim());
	
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	sta = pBx->GetStation();
	DomBM.MoveTo(sta, 0);
	double YScale   = Len*0.1;
	double BMValue  = 0, BMValueEnd = 0;
	
	for(long nElm = 0; nElm < pFEM->GetElementSize(); nElm++)
	{
		CElement *pElm  = pFEM->GetElement((unsigned short)nElm); 
		CJoint   *pJStt = pFEM->GetJoint(pElm->m_nJStt);
		CJoint   *pJEnd = pFEM->GetJoint(pElm->m_nJEnd);
		
		sta = pJStt->m_vPointReal.x;
		BMValue = pElm->GetResultForce(0)->m_ResultForceElement[0]->m_dForceOnElement[ELE_MOMENT2];
		if(nElm==pFEM->GetElementSize()-1)
		{
			staEnd = pJEnd->m_vPointReal.x;
			BMValueEnd = pElm->GetResultForce(0)->m_ResultForceElement[1]->m_dForceOnElement[ELE_MOMENT2];
			BMValueEnd = 1.0;
		}

		pOptStd->SetEnvType(&DomBM, HCAD_STLC);

		DomBM.LineTo(sta, -BMValue*YScale);
		if(nElm==pFEM->GetElementSize()-1)
			DomBM.LineTo(staEnd, -BMValueEnd*YScale);

		if(pJStt->IsRestraintJoint() || (nElm==pFEM->GetElementSize()-1 && pJEnd->IsRestraintJoint()))
		{
			pOptStd->SetEnvType(&DomBM, HCAD_SYMB);
			DomBM.SetTextHeight(8);

			CDPoint P1(0,0);
			if(nElm==pFEM->GetElementSize()-1)
			{
				P1 = CDPoint(staEnd, BMValueEnd + Len*0.04);
				cs.Format("%.3f", BMValueEnd);
				DomBM.TextOut (P1, cs);
			}
			else
			{
				P1 = CDPoint(sta, BMValue + Len*0.04);
				cs.Format("%.3f", BMValue);
				DomBM.TextOut (P1, cs);
			}
		}
	}

	pOptStd->SetEnvType(&DomBM, HCAD_STLC);
	DomBM.LineTo(staEnd, 0);
	DomBM.Move(0, BaseY);
	*pDom << DomBM;

	// 지간구성 치수선 그림.	
	Dom.SetDirection("BOTTOM");
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetTextHeight(7);

	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING);
	dPreSta = staGirStt;
	while(pBx)   
	{
		if (!pBx->IsState(BX_CROSSBEAM_VBRACING) || pBx->IsJijum())   {
			sta = pBx->GetStation();
			if (staStt < sta && DimSta.GetSize() == 0)   {  // 치수선 첫부분이 중간에 걸림
				DimSta.Add(staStt);
				LenSta.Add(dPreSta);
			}
			if (staStt <= sta && sta <= staEnd)   {
				DimSta.Add(sta);
				LenSta.Add(sta);
			}
			if (sta > staEnd)   {  // 치수선 마지막이 중간에 걸림
				DimSta.Add(staEnd);
				LenSta.Add(sta);
			}
			if (sta >= staEnd)
				break;
			dPreSta = sta;
		}
		pBx = Finder.GetBxNext();
	}

	long nDan = 0;
	double staDim = DimSta.GetAt(0);
	double staLen = LenSta.GetAt(0);
	Dom.DimMoveTo(staDim, 0, nDan);

	double staDimPre = staDim;
	double staLenPre = staLen;
	long nSize = DimSta.GetSize();
	for (long nDim = 1; nDim < nSize; nDim++)   {
		staDim = DimSta.GetAt(nDim);
		staLen = LenSta.GetAt(nDim);
		CString cs = Comma(staLen - staLenPre);
		if (staDim == staLen)
			Dom.DimLineTo(staDim - staDimPre, nDan, cs);
		else
			Dom.DimLineToExtend(staDim - staDimPre, nDan, cs, "", FALSE);

		staDimPre = staDim;
		staLenPre = staLen;
	}

	DimSta.RemoveAll();
	LenSta.RemoveAll();

	*pDom << Dom;

}

CString CXLDesignOfMold::GetJointArrszByI33(double dI33, long nType, long nG)
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CFEMManage			*pFemData		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	long	nJointsu	= pFemData->GetJointSize();
	long	nElmsu		= pFemData->GetElementSize();
	CStringArray	szArrSec;
	CString			szTemp;
	CString			szSec;

	dI33 = frM4(dI33);
	
	if(nType == 0)	///< 거더
	{
		BOOL bIsGirInner = IsOuterGirder(nG);
		BOOL bIsSecGirInner = FALSE;
		long nSecGirderNumber = 0;
		for(long nJoint = 0; nJoint < nJointsu; nJoint++)
		{
			CPlateBasicIndex *pBxSec = pCalcStd->GetBxByJoint(pFemData->GetJoint((unsigned short)nJoint));
			nSecGirderNumber= pBxSec->GetNumberGirder();
			
			bIsSecGirInner = IsOuterGirder(nSecGirderNumber);
		
			if(bIsGirInner != bIsSecGirInner)
				continue;
			
			ModelCalc.GetSectionCoefficient(nSecGirderNumber, pBxSec->GetStation(), FALSE, pBxSec->GetStation(), 1);

			double	dI33Sec	= ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBxSec);
			if(ModelCalc.IsSameI33(dI33, dI33Sec))
			{
				szTemp.Format("%d", nJoint+1);
				szArrSec.Add(szTemp);
			}
		}
	}
	else if(nType == 1)	///< 가로보
	{
		for(long nElm = 0; nElm < nElmsu; nElm++)
		{
			CElement *pElm = pFemData->GetElement((unsigned short)nElm);
			CPlateBasicIndex *pBxSec = pCalcStd->GetBxByJoint(pFemData->GetJoint(pElm->m_nJStt));
			if(!pBxSec) continue;
			if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)
			{
				ModelCalc.GetSectionCoefficientCR(pBxSec);
				double	dI33Sec	= ModelCalc.GetMapValueSecCoCR("I33");
				if(ModelCalc.IsSameI33(dI33, dI33Sec))
				{
					szTemp.Format("%d", nElm+1);
					szArrSec.Add(szTemp);
				}
			}
		}
	}
	long n = 0;
	for(n=0; n<szArrSec.GetSize(); n++)
	{
		szTemp.Format(" %s,", szArrSec.GetAt(n));
		if(n==szArrSec.GetSize()-1)
			szTemp.Format(" %s", szArrSec.GetAt(n));
		szSec += szTemp;
	}

	CString szSecTemp;
	long nSecSize = szArrSec.GetSize();
	m_szArrSecJointNum.RemoveAll();
	for(n=0; n<szArrSec.GetSize(); n+=15)
	{
		szSecTemp = _T("");
		for(long k=n; k<n+15; k++)
		{
			szTemp.Format(" %s,", szArrSec.GetAt(k));
			if(k==nSecSize-1)
			{
				szTemp.Format(" %s", szArrSec.GetAt(k));
				szSecTemp += szTemp;
				break;
			}
			szSecTemp += szTemp;
		}

		m_szArrSecJointNum.Add(szSecTemp);
	}

	return szSec;
}

CString CXLDesignOfMold::RetMaxString(CString szStr)
{
	CString szReStr	="";
	CString szOne	= "";
	CString szTwo	= "";
	CString szThree = "";
	int nCnt = 0;

	if(szStr.Find("DB") != -1)
	{
		szTwo = "DB";
		nCnt = szStr.Find("DB");
	}
	else if(szStr.Find("DL") != -1)
	{
		szTwo = "DL";
		nCnt = szStr.Find("DL");
	}
	else
	{
		return szStr;
	}
	
	szOne = szStr.Left(nCnt);
	szThree = szStr.Mid(nCnt+2);
	
	szReStr.Format("%s %s %s차선", szOne, szTwo, szThree);

	return szReStr;
}

void CXLDesignOfMold::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}

double CXLDesignOfMold::GetScaceFactor(long nLane)
{
	CCalcGeneral CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());
	
	return CalcGeneral.GetScaceFactor(nLane);
}
///////////////////////////////////////////////////////////////////////////////////
//∫∴∵∮∑∏ㄷ ΑΒΓΔΕΖΗαβγδεζηΣθιΙκλμνξοπρf υφχψω
//	´≤＞＜≠±≤≥∞⌒×㎝⁴ ㎤ ㎠ ㎝ kgf/㎠ t·m  ¹²³⁴₁₂₃₄
//  ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩ ⑪ ⑫ ⑬ ⑭ ⑮ √½⅓㉮㉯㉰㉱㉲㉳㉴
//  ━━━━━━━━━━━━━  ⇒ ⊙
//  ┏ ┗ ┛ ┓┃━ ┣ ┫ⅰ ⅱ ⅲ ⅳ ⅴ ⅵ ⅶ ⅷ ⅸ ⅹ Ⅰ Ⅱ Ⅲ Ⅳ Ⅳ Ⅴ Ⅵ
///////////////////////////////////////////////////////////////////////////////////// 

CDPoint CXLDesignOfMold::GetCenterOffsetPos(double sta, double dOffset, long nG)
{
	CPlateBridgeApp	*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd	*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc ModelCalc(pCalcStd->m_pDataManage, pCalcStd);

	CDPoint xy (0,0), org (0,0);
	CDPoint SttAng = ModelCalc.GetStdAngle();

	CPlateGirderApp	*pGir = pDB->GetGirder(nG);  // 중심거더 기준.
	xy = pGir->GetLine()->GetXyLineDis(sta, dOffset);
	xy = GetXyRotateVector(org,xy,SttAng);

	return xy;
}

void CXLDesignOfMold::CalcNodeCoord(CDPointArray &pNode, double &dShoeLength, double dWidth)
{
	CAPlateCalcStd	*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage		*pFemData	= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	dShoeLength = 0;
	double z = 0;
	double prez = 0;
	CDPoint xy (0,0), prexy(0,0);
	CVectorArray Vertex;
	CVector node;

	for(long nNodeIdx = 0; nNodeIdx < pFemData->GetJointSize(); nNodeIdx++)
	{
		CJoint *pJ = pFemData->GetJoint((unsigned short)nNodeIdx);

		xy.x = pJ->m_vPoint.x;
		xy.y = pJ->m_vPoint.y * (dWidth+1);
		z	 = pJ->m_vPoint.z;

		if(prexy==xy)
		{
			dShoeLength = (xy.x-prexy.x)*(xy.y-prexy.y) + (xy.y-prexy.y)*(xy.y-prexy.y)
						+ (z - prez)*(z - prez);
			dShoeLength = sqrt(dShoeLength);
		}

		pNode.Add(xy);
		prez = z;

		node.x = pJ->m_vPoint.x;
		node.y = pJ->m_vPoint.y;
		node.z = pJ->m_vPoint.z;

		Vertex.Add(&node);
	}
}

void CXLDesignOfMold::ModelPlanOutput(CDomyun *pDom)
{
	CPlateBridgeApp	*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd	*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage		*pFemData	= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc ModelCalc(pCalcStd->m_pDataManage, pCalcStd);
	
	// 교량길이에 대한 비율구하기.
	double	dWidth		= 0.5;
	double	x			= 0;
	double	y			= 0;
	
	CDPoint P1, P2;
	CString sLabel;

	CDomyun Dom;	
	
	// 부재 그리기
	CDPoint Shoe(0,0);
	double dShoeLength;
	CDPointArray pNodeArr;	
	CalcNodeCoord(pNodeArr, dShoeLength, dWidth);
	long nEleIdx = 0;
	for(nEleIdx = 0; nEleIdx < pFemData->GetElementSize(); nEleIdx++)
	{
		CElement *pEle = pFemData->GetElement((unsigned short)nEleIdx);
		int nIIdx = pEle->m_nJStt;
		int nJIdx = pEle->m_nJEnd;
		
		switch (pEle->m_LongAttrib[ELEMENT_TYPE])
		{
		case FRAME_GIRDER_ELEMENT :  // 거더
			pDom->SetLineColor(2);		break;
		case FRAME_CROSS_ELEMENT :  // 가로보
			pDom->SetLineColor(4);		break;
		case FRAME_STRINGER_ELEMENT :  // 세로보
			pDom->SetLineColor(3);		break;
		default :
//		case 4 :  // Rigid
//		case 5 :
//		case 6 :
			pDom->SetLineColor(1);		break;
		}
		// 부재 라인 그리기		
		P1 = CDPoint(pNodeArr[nIIdx].x, pNodeArr[nIIdx].y);
		P2 = CDPoint(pNodeArr[nJIdx].x, pNodeArr[nJIdx].y);

		if(P1==P2)
		{			
			P2.x -= dShoeLength*cos(45.0);
			P2.y -= dShoeLength*sin(45.0);					
		}
//		else if(pEle->m_LongAttrib[ELEMENT_TYPE] & 5 || pEle->m_LongAttrib[ELEMENT_TYPE]==6)
//		{
//			P1.x -= dShoeLength*cos(45);
//			P1.y -= dShoeLength*sin(45);
//
//			P2.x -= dShoeLength*cos(45);
//			P2.y -= dShoeLength*sin(45);
//		}

		Dom.MoveTo(P1);
		Dom.LineTo(P2);
	}

	*pDom << Dom;

	// 좌표축 그리기
	Dom.SetLineColor(7);
	Dom.SetTextAngle(-90);
	Dom.SetTextHeight(dWidth*3);
	double AxisLen = 2.5;
	CVector vPoint = pFemData->GetJoint(0)->m_vPoint;
	x = vPoint.x - AxisLen * 4;
	y = vPoint.y - AxisLen * 8;
	P1 = CDPoint(x, y);
	
	Dom.Circle(P1,AxisLen*0.1,TRUE);
	Dom.LineTo(P1,CDPoint(P1.x+AxisLen*2,P1.y));
	Dom.LineTo(P1,CDPoint(P1.x,P1.y+AxisLen*2));
	Dom.LineTo(P1,CDPoint(P1.x+(AxisLen*2)*cos(45.0),P1.y+(AxisLen*2)*cos(45.0)));
	Dom.SetArrowHeadSize(1);
	Dom.Arrow(CDPoint(P1.x+AxisLen*2,P1.y),0);
	Dom.Arrow(CDPoint(P1.x,P1.y+AxisLen*2),ConstPi/2);
	Dom.Arrow(CDPoint(P1.x+(AxisLen*2)*cos(45.0),P1.y+(AxisLen*2)*cos(45.0)),ConstPi/4);
	
	Dom.TextOut(P1.x+AxisLen*2,P1.y,"X");
	Dom.TextOut(P1.x-1,P1.y+AxisLen*2.5,"Y");
	Dom.TextOut(P1.x+(AxisLen*2)*cos(45.0),P1.y+(AxisLen*2)*cos(45.0)+1,"Z");

	*pDom << Dom;

	// 절점 번호 그리기	
	Dom.SetLineColor(9);
	long nNodeIdx = 0;
	for(nNodeIdx = 0; nNodeIdx<pFemData->GetJointSize(); nNodeIdx++)
	{
		CJoint *pJ = pFemData->GetJoint((unsigned short)nNodeIdx);
		P1 = CDPoint(pNodeArr[nNodeIdx].x, pNodeArr[nNodeIdx].y);

		if(pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE || pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_LEFT_NODE || pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_RIGHT_NODE)
		{
			CDPoint P3(P1.x-dShoeLength*cos(45.0)-1, P1.y-dShoeLength*cos(45.0)-1);
			sLabel.Format("%d", pJ->m_nIdx+1);
			Dom.TextOut(P3.x, P3.y, sLabel);
		}
		else
		{			
			CDPoint P3(pNodeArr[nNodeIdx].x+1, pNodeArr[nNodeIdx].y-1);
			sLabel.Format("%d", pJ->m_nIdx+1);
			Dom.TextOut(P3.x, P3.y, sLabel);
		}		
	}
	*pDom << Dom;
	
	// 부재 번호 그리기
	Dom.SetLineColor(7);
	for (nEleIdx = 0; nEleIdx < pFemData->GetElementSize(); nEleIdx++)   
	{
		CElement *pEle = pFemData->GetElement((unsigned short)nEleIdx);
		int nIIdx = pEle->m_nJStt;
		int nJIdx = pEle->m_nJEnd;

		P1 = CDPoint(pNodeArr[nIIdx].x, pNodeArr[nIIdx].y);
		P2 = CDPoint(pNodeArr[nJIdx].x, pNodeArr[nJIdx].y);
		if(P1==P2)
		{
			P2.x -= dShoeLength*cos(45.0);
			P2.y -= dShoeLength*sin(45.0);	

			sLabel.Format("%d", pEle->m_nIndex+1);
			CDPoint cp = CDPoint((P1.x+P2.x)*0.5,(P1.y+P2.y)*0.5);
			Dom.TextOut(cp, sLabel);		
		}
//		else if(pEle->m_nMain==5 || pEle->m_nMain==6)
//		{
//			P1.x -= dShoeLength*cos(45);
//			P1.y -= dShoeLength*sin(45);
//
//			P2.x -= dShoeLength*cos(45);
//			P2.y -= dShoeLength*sin(45);
//			CDPoint cp = CDPoint((P1.x+P2.x)*0.5+1,(P1.y+P2.y)*0.5+1);
//			Dom.TextOut(cp, sLabel);
//		}
		else
		{
			sLabel.Format("%d", pEle->m_nIndex+1);
			CDPoint cp = CDPoint((P1.x+P2.x)*0.5+1,(P1.y+P2.y)*0.5-2);
			Dom.TextOut(cp, sLabel);		
		}
	}		
	*pDom << Dom;

	// 거더 & 지점 Symbol 그리기
	Dom.SetLineColor(6);
	Dom.SetTextHeight(dWidth*4);
	CDPoint Point, Pre, vDir;
	long nPrevGir = -1, nJijum = 0;
	for (nNodeIdx = 0; nNodeIdx < pFemData->GetJointSize(); nNodeIdx++)
	{
		CJoint *pJ = pFemData->GetJoint((unsigned short)nNodeIdx);
		if(pJ->m_LongAttrib[JOINT_TYPE] & FRAME_GIRDER_NODE)
		{
			long nG = pJ->m_LongAttrib[JOINT_GIR];
			CPlateBasicIndex *pBx = pCalcStd->GetBxByJoint(pJ);
			CString sLabel;
			if(nG != nPrevGir)   
			{  // 거더의 시작부분.
				nPrevGir = nG;
				// 거더 심볼 그리기.
				vDir = (pNodeArr[nNodeIdx + 1] - pNodeArr[nNodeIdx]).Unit();
				Point = pNodeArr[nNodeIdx] - vDir * 6;			
				sLabel.Format("G%d", nG + 1);
				Dom.TextCircle(Point.x,Point.y,sLabel,0,TRUE);
				Pre = Point;
			}

			if(nG != 0)	continue;

			if(pBx->IsJijum())   
			{
				// 지점 심볼 그리기.
				if(pBx->IsDanbuJijumStt())
					vDir = (pNodeArr[nNodeIdx + 1] - pNodeArr[nNodeIdx]).Unit().Rotate90();
				else
					vDir = (pNodeArr[nNodeIdx - 1] - pNodeArr[nNodeIdx]).Unit().RotateInv90();
				Point = pNodeArr[nNodeIdx] + vDir * 6;

				nJijum = pBx->GetNumberJijum();
				if(nJijum!=-1)
				{
					sLabel = pDB->m_strJijumName[nJijum];
					Dom.TextCircle(Point.x,Point.y,sLabel,0,TRUE);
				}
			}
		}
	}
	*pDom << Dom;
}

BOOL CXLDesignOfMold::IsOuterGirder(long nG)
{	
	CPlateBridgeApp *pDB = m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	BOOL bResult =  TRUE;
	
	if(nG == 0 || nG == pDB->GetGirdersu() - 1)
		bResult = FALSE;

	return bResult;
}
