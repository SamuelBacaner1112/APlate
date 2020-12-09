// XLWeldingCheck.cpp: implementation of the CXLWeldingCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcStd.h"
#include "XLWeldingCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

CXLWeldingCheck::CXLWeldingCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_nG				= 0;
	m_nSectionSize		= 0;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
	m_pVStiffBx			= NULL;
}

CXLWeldingCheck::~CXLWeldingCheck()
{

}

void CXLWeldingCheck::WeldingCheck()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CString strText = _T("");

	m_nSttRow = 1;
	m_nSectionSize=0;
	pOut->ClearCellRef();
	pOut->SetXL(pXL, "5.2 용접부의 검토",					m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "·주요부재의 응력을 전달하는 필렛용접의 치수는 다음의 제한내에 있는 것을 표준으로 한다.",m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "  다만 최소 모재의 두께가 8mm이상인 경우는 필렛용접 치수를 6mm이상으로 한다.",m_nSttRow, "B", "B", 0);
	
	if(pOpt->GetSteelStandardYear()==APPLYYEAR2010)
		strText = _T("(☞ 도.설.기 2010 3.5.2.4)");
	else
		strText = _T("(☞ 도.설.기 2008 3.5.2.4)");

	pOut->SetXL(pXL, strText, m_nSttRow, "U", "U", 1, BLACK, 9, FALSE, TA_LEFT, "BOOK_REF_3_5_2_4");
	
	long nSttRowTemp = m_nSttRow;
	pOut->SetXL(pXL, "t₁:  얇은쪽 모재의 두께 (mm)",		nSttRowTemp, "L", "L");
	pOut->SetXL(pXL, "t₂:  두꺼운쪽 모재의 두께 (mm)",	nSttRowTemp, "L", "L");
	pOut->SetXL(pXL, "S  :  필렛용접 치수 (mm)",			nSttRowTemp, "L", "L");

	m_nSttRow++;

	if(pOpt->GetSteelStandardYear()==APPLYYEAR2010)
	{	
		strText = _T("t₁> S ≥ 6mm(t₂≤ 20mm)");
		pOut->SetXL(pXL, strText, m_nSttRow, "C", "C");
		strText = _T("t₁> S ≥ 8mm(t₂＞20mm)");
		pOut->SetXL(pXL, strText, m_nSttRow, "C", "C", 2);
	}
	else
	{	
		strText = _T("t₁＞ S  ≥ √2·t₂");
		pOut->SetXL(pXL, strText, m_nSttRow, "B", "B", 2);
	}

	pOut->SetXL(pXL, "가) 플랜지와 복부판의 용접부 검토",	m_nSttRow, "B", "B");
	FilletWeldMeasureCheck();

	WeldStressCheck();
	pOut->ClearCellRef();
	ReinforceWeldCheck(FALSE);	// 교대부

	pOut->ClearCellRef();
	ReinforceWeldCheck(TRUE);	// 교각부

}

///< 필렛용접 치수 검토
void CXLWeldingCheck::FilletWeldMeasureCheck()
{

	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText, sCellRef_Tu, sCellRef_Tl, sCellRef_Tw, sCellRef_Smin, sCellRef_Smax, sCellRef_Suse;
	long	nIndex	= 0;
	long	nG		= 0;
	double	dSta	= 0;
	pOut->SetXL(pXL, "1) 필렛용접 치수 검토",				m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "tu : 상부 플랜지 두께",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "tl : 하부 플랜지 두께",				m_nSttRow, "J", "J", 0);
	pOut->SetXL(pXL, "tw : 복부판 두께 ",					m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, "(단위 : mm)",							m_nSttRow, "X", "X");

	pOut->SetLineBoxText(pXL, "구 분",						m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "모재의 사용두께",			m_nSttRow, "E", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "상부 플랜지와 복부판",		m_nSttRow, "K", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "하부 플랜지와 복부판",		m_nSttRow, "S", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "tu",							m_nSttRow, "E", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "tl",							m_nSttRow, "G", "H", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "tw",							m_nSttRow, "I", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smax",						m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smin",						m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Suse",						m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",						m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smax",						m_nSttRow, "S", "T", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smin",						m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Suse",						m_nSttRow, "W", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "판정",						m_nSttRow, "Y", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);

	long	nPosSec	= -1;
	long	nSec	= 0;
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		nSec	= atoi(pFrameSec->m_szName);
		nG		= -1;
		dSta	= 0;
		ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_SHEAR_2, CFemModelingCalc::MAX_ABS, FALSE);

		if (nG<0) nG=0;
		double	dStaSec	= ModelCalc.GetStationSecJewon(nSec, nG, dSta, nPosSec);

		CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
		if(!pGir)	continue;

		CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dStaSec);
		if(!pBx)	continue;

		double	dTu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
		double	dTl	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
		double	dTw	= pBx->GetFactChain(G_W)->m_dFactChainThick;
		double	dSu	= pDataManage->GetFilletWeldStandard(dTu, dTw); 
		double	dSl	= pDataManage->GetFilletWeldStandard(dTl, dTw);
		
		sText.Format("단면 - %d", nIndex+1);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "B", "D", 1, RED,   9, FALSE, TA_CENTER);
		///< tu
		sCellRef_Tu.Format("TU_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTu,						m_nSttRow, "E", "F", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tu, 0);
		///< tl
		sCellRef_Tl.Format("TL_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTl,						m_nSttRow, "G", "H", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tl, 0);
		///< tw
		sCellRef_Tw.Format("TW_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTw,						m_nSttRow, "I", "J", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tw, 0);
		///< Smax - 상부
		sText.Format("=MIN(%s,%s)", pOut->GetCellRef(sCellRef_Tu), pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smax.Format("UPPER_S_MAX_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smax, 1);
		///< Smin - 상부
		sText.Format("=IF(%s=\"(☞ 도.설.기 2010 3.5.2.4)\",IF(MAX(E%d,I%d)<=20,6,8),IF(%s<8,SQRT(2*MAX(%s,%s)),MAX(6,SQRT(2*MAX(%s,%s)))))", pOut->GetCellRef("BOOK_REF_3_5_2_4"), m_nSttRow, m_nSttRow,
					pOut->GetCellRef(sCellRef_Smax),pOut->GetCellRef(sCellRef_Tu),pOut->GetCellRef(sCellRef_Tw),pOut->GetCellRef(sCellRef_Tu),pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smin.Format("UPPER_S_MIN_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smin, 1);
		///< Suse - 상부
		sCellRef_Suse.Format("UPPER_SUSE_UP_%d", nIndex);
		pOut->SetLineBoxText(pXL, dSu,						m_nSttRow, "O", "P", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Suse, 0);
		///< 판정
		sText.Format("=IF(AND(%s<=%s,%s>=%s),\"OK\",\"NG\")", pOut->GetCellRef(sCellRef_Smin), pOut->GetCellRef(sCellRef_Suse),
					pOut->GetCellRef(sCellRef_Smax), pOut->GetCellRef(sCellRef_Suse));
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER);
		///< Smax - 하부
		sText.Format("=MIN(%s,%s)", pOut->GetCellRef(sCellRef_Tl), pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smax.Format("LOWER_S_MAX_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "S", "T", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smax, 1);
		///< Smin - 하부
		sText.Format("=IF(%s=\"(☞ 도.설.기 2010 3.5.2.4)\",IF(MAX(G%d,I%d)<=20,6,8),IF(%s<8,SQRT(2*MAX(%s,%s)),MAX(6,SQRT(2*MAX(%s,%s)))))", pOut->GetCellRef("BOOK_REF_3_5_2_4"), m_nSttRow, m_nSttRow,
					pOut->GetCellRef(sCellRef_Smax),pOut->GetCellRef(sCellRef_Tl),pOut->GetCellRef(sCellRef_Tw),pOut->GetCellRef(sCellRef_Tl),pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smin.Format("LOWER_S_MIN_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smin, 1);
		///< Suse - 하부
		sCellRef_Suse.Format("LOWER_SUSE_UP_%d", nIndex);
		pOut->SetLineBoxText(pXL, dSl,						m_nSttRow, "W", "X", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Suse, 0);
		///< 판정
		sText.Format("=IF(AND(%s<=%s,%s>=%s),\"OK\",\"NG\")", pOut->GetCellRef(sCellRef_Smin), pOut->GetCellRef(sCellRef_Suse),
					pOut->GetCellRef(sCellRef_Smax), pOut->GetCellRef(sCellRef_Suse));
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "Y", "Z", 1, BLACK, 9, FALSE, TA_CENTER);

		SetLines(NEXT);
		nIndex++;
	}

	SetLines(NEXT);
}
///< 용접부의 응력 검토
void CXLWeldingCheck::WeldStressCheck()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CSteel				*pSteel			= pDataManage->GetSteel();
	CFEMManage          *pFEM		   = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CSteel				*pSteeCtl		= pDataManage->GetSteel();
	CString	sText;
	pOut->SetXL(pXL, "2) 용접부의 응력 검토",				m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "용접부에 작용하는 전단응력",			m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, "[용접강도로교 표준시방서] 26Page",	m_nSttRow, "S", "S", 1);

	///< 이미지삽입
	pOut->InsertImage(pXL, "용접부응력검토1", m_nSttRow, "B");
	pOut->SetXL(pXL, "tu",		m_nSttRow, "F", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "s",		m_nSttRow, "B", "B", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Yvsu",	m_nSttRow, "H", "I", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Yvsl",	m_nSttRow, "H", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "tl",		m_nSttRow, "B", "B", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "B",		m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);

	SetLines(2);
	pOut->InsertImage(pXL, "용접부응력검토", m_nSttRow, "B");
	SetLines(NEXT);
	pOut->SetXL(pXL, "s",									m_nSttRow, "E", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "s",									m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-9);

	pOut->SetLineBoxText(pXL, "υ",							m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",							m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "S · Q",					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "<",							m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "υa ",						m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(NEXT);
	pOut->SetXL(pXL, "Iv·Σa",								m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "υ",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "전단 응력 (MPa)",				m_nSttRow, "N", "N", 1);
	pOut->SetXL(pXL, "υa",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "허용 전단 응력",						m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "(☞ 도.설.기 3.3.2.1) ",			m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "S",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "총 작용전단력 (N)",					m_nSttRow, "N", "N", 1);
	pOut->SetXL(pXL, "Q",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "용접선 외측 플랜지의 총단면의 중립축에 관한",m_nSttRow, "N", "N");
	pOut->SetXL(pXL, "단면 1차 모멘트 (mm³)",				m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "A(mm²)·Y(mm)",						m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "y",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Yvsu(l) - tu(l)/2",					m_nSttRow, "P", "P");
	pOut->SetXL(pXL, "Iv",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "총단면의 중립축에 관한 단면 2차 모멘트(mm⁴)",m_nSttRow, "N", "N");
	pOut->SetXL(pXL, "Σa",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "필렛용접의 목두께의 합 (cm)",			m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "2EA × s /  √2",						m_nSttRow, "V", "V");
	pOut->SetXL(pXL, "s",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "필렛 용접 치수 (mm)",					m_nSttRow, "N", "N", 4);

	pOut->SetXL(pXL, "응력 검토 결과",						m_nSttRow, "B", "B");
	CString	sFlange, sSmax, sTu, sB, sA, sYvsu, sQ, sIv, sS, sSigmaA, sNu, sANu, sJuType;
	long	nIndex			= 0;
	long	nMaterial		= 0;
//	long	nSteelIdx		= 0;
	double	dSmax			= 0;
	double	dTu				= 0;
	double	dWf				= 0;
	double	dYvsu			= 0;
	double	dIv				= 0;
	double	dS				= 0;
	double	dTw				= 0;
	double	dANu			= 0;
	
	for(long n=0; n < 2; n++)
	{
		if(n==0)
		{
			pOut->SetXL(pXL, "① 상부 플랜지와 복부판",				m_nSttRow, "B", "B");
			sFlange = "UPPER";
		}
		else
		{
			sFlange = "LOWER";
			pOut->SetXL(pXL, "② 하부 플랜지와 복부판",				m_nSttRow, "B", "B");
		}
		nIndex = 0;
		pOut->SetLineBoxText(pXL, "구 분",					m_nSttRow, "A", "C", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "G", "H", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "J", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "K", "L", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "M", "N", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "O", "Q", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "U", "W", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "X", "Z", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "비고",					m_nSttRow,"AA","AA", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Smax",							m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		if(n==0)
		{
			pOut->SetXL(pXL, "tu",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Bu",								m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "A",								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Yvsu",							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		else
		{
			pOut->SetXL(pXL, "tl",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Bl",								m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "A",								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Yvsl",							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		pOut->SetXL(pXL, "Q",								m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Iv",								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "s",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Σa",								m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "υ",								m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "υa",								m_nSttRow, "X", "Z", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "(kN)",							m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "F", "F", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm²)",							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm³)",							m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm⁴)",							m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "R", "R", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(MPa)",						m_nSttRow, "U", "W", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(MPa)",						m_nSttRow, "X", "Z", 1, BLACK, 8, FALSE, TA_CENTER);

		double dSmaxTmp=0;
		long	nPosSec = -1;
		long	nSec	= 0;
		long   nG   = 0;
		double dSta = 0;
		double dStaSec = 0;
		POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
		while(pos)
		{
			CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
			if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

			nSec	= atoi(pFrameSec->m_szName);
			nG   = -1;
			dSta = 0;
			ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_SHEAR_2, CFemModelingCalc::MAX_ABS, FALSE);

			if (nG<0) nG=0;
			dStaSec = ModelCalc.GetStationSecJewon(nSec, nG, dSta, nPosSec);

			CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
			CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dStaSec);

			dSmax	= fabs(StressCalc.GetElementForce(ELE_SHEAR_2, 0, nG, dSta, 4));	///< 최대전단력
			dTw		=  pBx->GetFactChain(G_W)->m_dFactChainThick;

			ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
			ModelCalc.GetSectionCoefficient(nG, dStaSec);
			StressCalc.GetSectionStress(nG, dSta, TRUE, dStaSec);
			BOOL bPositiveM = StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);

			if(n==0)	nMaterial = G_F_U;
			else		nMaterial = G_F_L;
			CString	strLeft		= _T("");
			CString	strRight	= _T("");
			pCalcGeneral->GetMaterial(pBx, nMaterial, strLeft, strRight);

			CHGBaseSteel	*pSteel = pSteeCtl->m_SteelArr[pSteeCtl->GetIndex(strLeft + strRight)];
			CHGAllowStress	AllowShear(pSteel);
			
			if(n==0)
			{
				if(bPositiveM)
					dYvsu = fabs(ModelCalc.GetMapValueSecCo("Yvsu"));
				else
					dYvsu = fabs(StressCalc.GetMapValueSecStress("Yvsu"));
				dTu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
				dWf	= pGir->GetWidthOnStation(dSta, TRUE);
				
				if(dSmaxTmp < dSmax)
				{
					dSmaxTmp	= dSmax;
					m_pVStiffBx	= pBx;
					m_nG		= nG;
				}
				m_nSectionSize++;	
			}
			else
			{
				if(bPositiveM)
					dYvsu = fabs(ModelCalc.GetMapValueSecCo("Yvsl"));
				else
					dYvsu = fabs(StressCalc.GetMapValueSecStress("Yvsl"));
				dTu	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
				dWf	= pGir->GetWidthOnStation(dSta, FALSE);
			
			}
			
			dS = pDataManage->GetFilletWeldStandard(dTu, dTw);
			dANu = AllowShear.GetAllowStress(ALLOW_WELD_SHEARSTRESS, dTu);
			if(pBridgeApp->IsTUGir())
			{
				if(bPositiveM)	dIv = ModelCalc.GetMapValueSecCo("Ivd33");
				else			dIv = StressCalc.GetMapValueSecStress("Irv");
			}
			else
			{
				if(bPositiveM)	dIv = ModelCalc.GetMapValueSecCo("Iv33");
				else			dIv = StressCalc.GetMapValueSecStress("Irv");
			}

			CString	sCell1, sCell2;
			sText.Format("단면 - %d", nIndex+1);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "A", "C", 1, RED,   9, FALSE, TA_CENTER);
			///< Smax
			sSmax.Format("SMAX_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dSmax,					m_nSttRow, "D", "E", 1, RED,   9, FALSE, TA_CENTER, sSmax, 1);
			///< tu
			sTu.Format("TU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dTu,						m_nSttRow, "F", "F", 1, RED,   9, FALSE, TA_CENTER, sTu, 0);
			///< B
			sB.Format("B_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dWf,						m_nSttRow, "G", "H", 1, RED,   9, FALSE, TA_CENTER, sB, 1);
			///< A
			sText.Format("=ROUND(%s*%s,3)", pOut->GetCellRef(sTu), pOut->GetCellRef(sB));
			sA.Format("A_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "I", "J", 1, BLACK, 9, FALSE, TA_CENTER, sA, 1);
			///< Yvsu
			sYvsu.Format("YVSU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dYvsu,					m_nSttRow, "K", "L", 1, RED,   9, FALSE, TA_CENTER, sYvsu, 1);
			///< Q
			sCell1.Format("M%d", m_nSttRow);	sCell2.Format("M%d", m_nSttRow);
			sText.Format("=ROUND(%s*(%s-%s),3)", pOut->GetCellRef(sA), pOut->GetCellRef(sYvsu), pOut->GetCellRef(sTu));
			sQ.Format("Q_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER, sQ, 0);
			pXL->SetNumberFormat(sCell1, sCell2, "0.0E+00");
			///< Iv
			sCell1.Format("O%d", m_nSttRow);	sCell2.Format("Q%d", m_nSttRow);
			sIv.Format("IV_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dIv,						m_nSttRow, "O", "Q", 1, RED,   9, FALSE, TA_CENTER, sIv, 0);
			pXL->SetNumberFormat(sCell1, sCell2, "0.000E+00");
			///< s
			sS.Format("S_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dS,						m_nSttRow, "R", "R", 1, RED,   9, FALSE, TA_CENTER, sS, 0);
			///< Σa
			sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef(sS));
			sSigmaA.Format("SIGMAA_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "S", "T", 1, BLACK, 9, FALSE, TA_CENTER, sSigmaA, 2);
			///< υ
			sText.Format("=ROUND(%s*10^3*%s/(%s*%s),3)", pOut->GetCellRef(sSmax), pOut->GetCellRef(sQ), pOut->GetCellRef(sIv), pOut->GetCellRef(sSigmaA));
			sNu.Format("NU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "W", 1, BLACK, 9, FALSE, TA_CENTER, sNu, 1);
			///< υa
			sANu.Format("ANU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dANu,						m_nSttRow, "X", "Z", 1, RED,   9, FALSE, TA_CENTER, sANu, 0);
			///< 비고
			sText.Format("=IF(%s>%s,\"NG\",\"OK\")", pOut->GetCellRef(sNu), pOut->GetCellRef(sANu));
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow,"AA","AA", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			nIndex++;
		}
		SetLines(1);
	}
}
///< 지점부 보강재 용접부 검토
void CXLWeldingCheck::ReinforceWeldCheck(BOOL bIsPier)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	CString	sText;

	GetJijumVStiffBxByMaxReaction(bIsPier);
	CPlateBasicIndex	*pBx		= pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)m_nMaxJoint));
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CVStiff				*pVStiff	= pGir->GetBxByStation(pBx->GetStation(), BX_JIJUMSTIFF)
									? pGir->GetBxByStation(pBx->GetStation(), BX_JIJUMSTIFF)->GetJijumStiff() : NULL;
	if(!pVStiff) return;

	long	nQty	= pGir->GetQtyJijumVStiffByBx(pBx);
	double	dHGir	= pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double	dWidth	= pVStiff->m_dWidth[0];
	double	dThick	= pVStiff->m_dThick[0];
	double	dHeight	= pVStiff->GetVStiffHeight(TRUE, dHGir);

	long	nWeldType	= 0;//용접 형태..
	long    dsThick     = 0;//필렛용접 치수...

	CString sJuType		= pBridgeApp->GetMaterial(G_F_U);
	
	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	pCalcGeneral->GetMaterial(pBx, G_F_U, strLeft, strRight);
	
	long	nMarkBuje1 = 0;
	long	nMarkBuje2 = 0;

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowShear(pSteel);
	double			dANu	= AllowShear.GetAllowStress(ALLOW_WELD_SHEARSTRESS, dThick);

	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
		{
			if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
			{
				nMarkBuje1 = V_ST; nMarkBuje1 = G_W;
			}
			else
			{
				if(pBx->IsJijum())
				{
					nMarkBuje1 = CP_BR_W;
					nMarkBuje2 = G_W;
				}
				else
				{
					nMarkBuje1 = CG_BR_W;
					nMarkBuje2 = G_W;
				}
			}
		}
		else if(pBx->GetSection())
		{
			nMarkBuje1 = V_ST;
			nMarkBuje2 = G_W;
		}
	}
	long nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje1,nMarkBuje2,pBx);
	if(nIdx==-1)
		nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje2,nMarkBuje1,pBx);
	CSteelWeldData *pWeld = pDataManage->GetSteelWeld()->GetBaseWeldByIndex(nIdx);

	nWeldType	= pWeld->m_nWeldType;//용접형태
	dsThick		= (long)pGir->GetSizeWeldFillet(pBx, nMarkBuje1, nMarkBuje2);//필렛용접치수

	if(bIsPier)
	{
		if(m_ArrBxPier.GetSize() == 0) return;
		pOut->SetXL(pXL, "·교각부",			m_nSttRow, "B", "B");
	}
	else
	{
		if(m_ArrBxAbut.GetSize() == 0) return;
		pOut->SetXL(pXL, "나) 지점부 보강재 용접부 검토",		m_nSttRow, "B", "B");
		pOut->SetXL(pXL, "·교대부",			m_nSttRow, "B", "B");
	}

	if(nWeldType == 0)//필렛용접...
	{
		pOut->SetXL(pXL, "Rmax",							m_nSttRow, "B", "C", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, m_dMaxReaction,					m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "MAX_REACTION");
		pOut->SetXL(pXL, "kN",								m_nSttRow, "H", "H", 0);

		pOut->SetXL(pXL, "필렛용접 치수 s = ",				m_nSttRow, "L", "L", 0);
		pOut->SetXL(pXL, dsThick,								m_nSttRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "DSTHICK", 1);
		pOut->SetXL(pXL, "mm",								m_nSttRow, "S", "S", 1);


		pOut->InsertImage(pXL, "필렛용접", m_nSttRow, "C");

		pOut->SetXL(pXL, "사용보강재",						m_nSttRow, "L", "L", 0);
		pOut->SetXL(pXL, ":",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, nQty,								m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_QTY", 0);
		pOut->SetXL(pXL, "PL",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dWidth,							m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_W", 0);
		pOut->SetXL(pXL, "×",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dThick,							m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_T", 0);
		pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dHeight,							m_nSttRow, "X", "Y", 1, RED,   9, FALSE, TA_CENTER, "VSTIFF_H", 0);

		///< 그림관련 치수기입
		//pOut->SetXL(pXL, 2.0,		m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "PIC_DIM", 0);
		sText.Format("=%s", pOut->GetCellRef("DSTHICK"));
		pOut->SetXL(pXL, sText,		m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "THICK", 0);
		
		pOut->SetXL(pXL, "용접 목두께  a = s / √2 = ",		m_nSttRow, "L", "L", 0);
		sText.Format("=%s/sqrt(2)",pOut->GetCellRef("DSTHICK"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "ATHICK", 1);
		pOut->SetXL(pXL, "mm",								m_nSttRow, "U", "U", 2);

		pOut->SetLineBoxText(pXL, "υ",						m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetUnderLineText(pXL, "P",					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

		pOut->SetUnderLineText(pXL, "",						m_nSttRow, "R", "S", 0);
		sText.Format("=%s", pOut->GetCellRef("MAX_REACTION"));
		pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_REACTION_COEFF");
		pOut->SetUnderLineText(pXL, "×",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetUnderLineText(pXL, "10³",					m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_LEFT);
		
		pOut->SetXL(pXL, "Σa·l",							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*2", pOut->GetCellRef("VSTIFF_QTY"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_QTY_COEFF", 0);
		pOut->SetXL(pXL, "EA",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		//if(pWeld->m_nLayType!=1)	sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
		//else						sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef("THICK"));

		sText.Format("=%s", pOut->GetCellRef("ATHICK"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_T_COEFF", 1);
		pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_H"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_H_COEFF", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=ROUND(%s*10^3/(%s*%s*%s), 3)", pOut->GetCellRef("MAX_REACTION_COEFF"), pOut->GetCellRef("VSTIFF_QTY_COEFF"), pOut->GetCellRef("VSTIFF_T_COEFF"), pOut->GetCellRef("VSTIFF_H_COEFF"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "NU");
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "R", 0);
		pOut->SetXL(pXL, "υa",								m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dANu,								m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "ANU", 0);
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
		sText.Format("=IF(%s>%s, \">\", \"≤\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
		sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AA", 0, RED);
		SetLines(3);
	}
	else
	{
		pOut->SetXL(pXL, "Rmax",							m_nSttRow, "B", "C", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, m_dMaxReaction,					m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "MAX_REACTION");
		pOut->SetXL(pXL, "kN",							m_nSttRow, "H", "H", 1);

		pOut->InsertImage(pXL, "용접검토", m_nSttRow, "C");

		pOut->SetXL(pXL, "사용보강재",						m_nSttRow, "L", "L", 0);
		pOut->SetXL(pXL, ":",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, nQty,								m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_QTY", 0);
		pOut->SetXL(pXL, "PL",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dWidth,							m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_W", 0);
		pOut->SetXL(pXL, "×",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dThick,							m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_T", 0);
		pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dHeight,							m_nSttRow, "X", "Y", 1, RED,   9, FALSE, TA_CENTER, "VSTIFF_H", 0);

		///< 그림관련 치수기입
		pOut->SetXL(pXL, 2.0,		m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_LEFT, "PIC_DIM", 0);
		sText.Format("=(%s-%s)/2", pOut->GetCellRef("VSTIFF_T"), pOut->GetCellRef("PIC_DIM"));
		pOut->SetXL(pXL, sText,		m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_RIGHT, "THICK", 0);

		pOut->SetLineBoxText(pXL, "υ",						m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetUnderLineText(pXL, "P",					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

		pOut->SetUnderLineText(pXL, "",						m_nSttRow, "R", "S", 0);
		sText.Format("=%s", pOut->GetCellRef("MAX_REACTION"));
		pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_REACTION_COEFF");
		pOut->SetUnderLineText(pXL, "×",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetUnderLineText(pXL, "10³",					m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_LEFT);
		
		pOut->SetXL(pXL, "Σa·l",							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_QTY"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_QTY_COEFF", 0);
		pOut->SetXL(pXL, "EA",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
//		if(pWeld->m_nLayType!=1)	sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
//		else						sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef("THICK"));
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_T_COEFF", 1);
		pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_H"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_H_COEFF", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=ROUND(%s*10^3/(%s*%s*%s), 3)", pOut->GetCellRef("MAX_REACTION_COEFF"), pOut->GetCellRef("VSTIFF_QTY_COEFF"), pOut->GetCellRef("VSTIFF_T_COEFF"), pOut->GetCellRef("VSTIFF_H_COEFF"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "NU");
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "R", 0);
		pOut->SetXL(pXL, "υa",								m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dANu,								m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "ANU", 0);
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
		sText.Format("=IF(%s>%s, \">\", \"≤\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
		sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AA", 0, RED);
		SetLines(5);
	}

}

void CXLWeldingCheck::WeldVStiffCheck()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	CVStiff				*pVStiff= NULL;
	CSection *pSec = m_pVStiffBx->GetSection();
	if(pSec==NULL)		return;

	if(pSec->IsenDetType(SECDET_CROSSBEAM))
		pVStiff= pSec->GetCrossBeam()->m_pVStiff;
	else if(pSec->IsenDetType(SECDET_VBRACING))
		pVStiff= pSec->GetVBracing()->m_pVStiff;
	else 
		pVStiff= pSec->GetVStiff();
	CPlateBasicIndex *pBx = pVStiff->GetBx();
	long nVStiffsu	= 0;
	long nSide		= -1;
	if(pVStiff->m_dWidth[0]>0)
	{
		nSide = 0;
		nVStiffsu++;
	}
	else if(pVStiff->m_dWidth[1]>0)
	{
		nSide = 1;
		nVStiffsu++;
	}

	CString	sText, sText1, sText2;
	if(!pVStiff || nSide == -1) return;

	long	nQty	= nVStiffsu;
	double	dWidth	= pVStiff->m_dWidth[nSide];
	double	dThick	= pVStiff->m_dThick[nSide];
	double	dHeight	= pVStiff->GetVStiffHeight(nSide==0?TRUE:FALSE);

	CString sJuType		= pBridgeApp->GetMaterial(G_F_U);
	
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
	CHGAllowStress	AllowStress(pSteel);

	double	dANu		= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dThick);
	long	nMarkBuje1 = 0;
	long	nMarkBuje2 = 0;
	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
		{
			if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
			{
				nMarkBuje1 = V_ST;
				nMarkBuje2 = G_W;
			}
			else
			{
				if(pBx->IsJijum())
				{
					nMarkBuje1 = CP_BR_W;
					nMarkBuje2 = G_W;
				}
				else
				{
					nMarkBuje1 = CG_BR_W;
					nMarkBuje2 = G_W;
				}
			}
		}
		else if(pBx->GetSection())
		{
			nMarkBuje1 = V_ST;
			nMarkBuje2 = G_W;
		}
	}

	long nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje1,nMarkBuje2,pBx);
	if(nIdx==-1)
		nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje2,nMarkBuje1,pBx);
	CSteelWeldData *pWeld = pDataManage->GetSteelWeld()->GetBaseWeldByIndex(nIdx);

	pOut->SetXL(pXL, "다) 수직보강재 용접부 검토",			m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "Rmax",							m_nSttRow, "B", "C", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);

	sText1.Format("SMAX_%s_%d", _T("UPPER"), 0);
	sText2.Format("SMAX_%s_%d", _T("UPPER"), m_nSectionSize-1);
	sText.Format("=MAX(%s:%s)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2));
	pOut->SetXL(pXL, sText,								m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "MAX_REACTION");
	pOut->SetXL(pXL, "kN",							m_nSttRow, "H", "H", 1);

	pOut->InsertImage(pXL, "용접검토_지점부보강재", m_nSttRow, "C");

	pOut->SetXL(pXL, "사용보강재",						m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, ":",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, nQty,								m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_QTY", 0);
	pOut->SetXL(pXL, "PL",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dWidth,							m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_W", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dThick,							m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_T", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dHeight,							m_nSttRow, "X", "Y", 1, RED,   9, FALSE, TA_CENTER, "VSTIFF_H", 0);

	///< 그림관련 치수기입
	pOut->SetXL(pXL, 2.0,		m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "PIC_DIM", 0);
	sText.Format("=(%s-%s)/2", pOut->GetCellRef("VSTIFF_T"), pOut->GetCellRef("PIC_DIM"));
	pOut->SetXL(pXL, sText,		m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "THICK", 0);

	pOut->SetLineBoxText(pXL, "υ",						m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P",					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	pOut->SetUnderLineText(pXL, "",						m_nSttRow, "R", "S", 0);
	sText.Format("=%s", pOut->GetCellRef("MAX_REACTION"));
	pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_REACTION_COEFF");
	pOut->SetUnderLineText(pXL, "×",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetUnderLineText(pXL, "10³",					m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_LEFT);
	
	pOut->SetXL(pXL, "Σa·l",							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("VSTIFF_QTY"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_QTY_COEFF", 0);
	pOut->SetXL(pXL, "EA",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	if(pWeld->m_nLayType!=1)	sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
	else						sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef("THICK"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_T_COEFF", 1);
	pOut->SetXL(pXL, "×",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("VSTIFF_H"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_H_COEFF", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*10^3/(%s*%s*%s), 3)", pOut->GetCellRef("MAX_REACTION_COEFF"), pOut->GetCellRef("VSTIFF_QTY_COEFF"), pOut->GetCellRef("VSTIFF_T_COEFF"), pOut->GetCellRef("VSTIFF_H_COEFF"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "NU");
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "R", 0);
	pOut->SetXL(pXL, "υa",								m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dANu,								m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "ANU", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
	sText.Format("=IF(%s>%s, \">\", \"≤\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AA", 0, RED);	

	SetLines(7);
}

// m_nMaxJoint : 절점번호 (0 부터 시작)
void CXLWeldingCheck::GetJijumVStiffBxByMaxReaction(BOOL bPier)
{
	CDataManage		*pDataManage= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd	*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage	*pManage    = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CResultArray	*pResult	= pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);

	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	m_ArrBxAbut.RemoveAll();
	m_ArrBxPier.RemoveAll();

	m_nMaxJoint		= 0;
	m_dMaxReaction	= 0;
	CJoint	*pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, FALSE);

	if(!bPier)
	{
		if(pJ)
		{
			m_ArrBxAbut.Add(pCalcStd->GetBxByJoint(pJ));
			m_nMaxJoint		= pJ->m_nIdx;
			CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(m_nMaxJoint);
			m_dMaxReaction	= maxabs(pJResult->m_vTotalMax[JOINT_REACTION].z, pJResult->m_vTotalMin[JOINT_REACTION].z);
		}
	}
	
	pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, TRUE);

	if(bPier)
	{
		if(pJ)
		{
			m_ArrBxPier.Add(pCalcStd->GetBxByJoint(pJ));
			m_nMaxJoint		= pJ->m_nIdx;
			CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(m_nMaxJoint);
			m_dMaxReaction	= maxabs(pJResult->m_vTotalMax[JOINT_REACTION].z, pJResult->m_vTotalMin[JOINT_REACTION].z);
		}
	}
}

double CXLWeldingCheck::GetMaxReaction(BOOL bPier)
{
	GetJijumVStiffBxByMaxReaction(bPier);
	return m_dMaxReaction;
}

long CXLWeldingCheck::GetMaxJoint(BOOL bPier)
{
	GetJijumVStiffBxByMaxReaction(bPier);
	return m_nMaxJoint;
}

void CXLWeldingCheck::SetLines(int nLine)
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