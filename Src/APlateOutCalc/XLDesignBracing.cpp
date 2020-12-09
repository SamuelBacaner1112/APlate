// XLDesignBracing.cpp: implementation of the CXLDesignBracing class.
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

CXLDesignBracing::CXLDesignBracing(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_dPw               = 0;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
}

CXLDesignBracing::~CXLDesignBracing()
{

}
void CXLDesignBracing::DesignVBracing()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	m_nSttRow = 1;
	pOut->SetXL(pXL, "8. 브레이싱의 설계"	 , m_nSttRow, "A", "A", 2, BLACK, 11);
	pOut->SetXL(pXL, "8.1 수직브레이싱"		 , m_nSttRow, "B", "B", 1, BLACK, 9);
	pOutExcel->SetProgress("8. 브레이싱의 설계", pOutExcel->GetProgress()+1);

	pStressCalc->SetSectionVBracingPos();
	long nVSec = pCalcData->m_ArrSecVBracing.GetSize();

	for(long n=0; n<nVSec; n++)
	{
		CVector vTemp = pCalcData->m_ArrSecVBracing.GetAt(n);
		CPlateGirderApp *pGir = pDB->GetGirder((long)vTemp.x);  
		CPlateBasicIndex *pBx = pGir->GetBxByStation(vTemp.y, BX_CROSSBEAM_VBRACING);
		DesignVBracing(pBx, n);
	}
}

void CXLDesignBracing::DesignVBracing(CPlateBasicIndex *pBx, long nIndex)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CFemModelingCalc    ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CCentSeparation		*pSepLeft		= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	CCentSeparation		*pSepRight		= pDB->GetCurGuardWall(CCentSeparation::RIGHT);

	long nG = pBx->GetNumberGirder();
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);
	CVBracing       *pVBracing	= pBx->GetSection()->GetVBracing();

	CString sText = "";
	pOut->ClearCellRef();
	pOut->SetColsWidth(pXL, 0, 100, 2);
	/// 필요한 데이터 계산
	double	dSlabThick		= toM(pDB->m_dThickSlabBase);
	double	dHeightLeft		= pSepLeft ? pSepLeft->GetHeight() : 0;
	double	dHeightRight	= pSepRight ? pSepRight->GetHeight() : 0;
	double	dLBangEumH		= toM(max(dHeightLeft, dHeightRight));	//방호벽높이(좌.우측중 큰것)
	double	dSta			= (pGir->GetBxOnJijum(1)->GetStation() - pGir->GetBxOnJijum(0)->GetStation())/2;//측경간의 중앙스테이션
	double	dGirderH		= toM(pGir->GetHeightGirderByStaAng(dSta) - pGir->GetThickFactByStation(dSta, G_F_U));		//거더의 높이(상부플랜지두께제외)
	double	dBridgeW		= toM(pDB->m_dBridgeWidth);
	long	nTotalVBr		= pGir->GetQtyVBracing();
	long	nCurVBracing	= pGir->GetVBracingPosByBx(pBx);

	double dTotGangJae = pCalcData->m_TotalWeight;
	double dLengthBridge = toM(pDB->GetLengthBridge());

	CPlateBasicIndex *pBxCompPre  = NULL;
	CPlateBasicIndex *pBxCompPost = NULL;
	if(pBx==pGir->GetBxOnJijum(0))	//처음 수직브레이싱
	{
		pBxCompPre  = NULL;
		pBxCompPost = pGir->GetBxByVBracingPos(1);
	}
	else if(pBx==pGir->GetBxOnJijum(pDB->m_nQtyJigan))	//마지막 수직브레이싱
	{
		pBxCompPre  = pGir->GetBxByVBracingPos(nTotalVBr-2);
		pBxCompPost = NULL;
	}
	else
	{	
		pBxCompPre  = pGir->GetBxByVBracingPos(nCurVBracing-1);
		pBxCompPost = pGir->GetBxByVBracingPos(nCurVBracing+1);
	}
	double dGapVBracingPre  = pBxCompPre ? fabs(pBxCompPre->GetStation()-pBx->GetStation()) : 0;
	double dGapVBracingPost = pBxCompPost? fabs(pBxCompPost->GetStation()-pBx->GetStation()) : 0;
	double dGapVBracing     = toM((dGapVBracingPre+dGapVBracingPost)/2);

	// 수직브레이싱 빔의 제원(일반부)
	CSteelMaterial *pMat = pDataManage->GetSteelMaterial();

//	long nSteelTypeUp = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_UVBEAM);
//	long nSteelTypeLo = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_LVBEAM);
//	long nSteelTypeC  = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_CVBEAM);
//	long nJewonTypeUp = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_UVBEAM);
//	long nJewonTypeLo = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_LVBEAM);
//	long nJewonTypeC  = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_CVBEAM);
	//상현
	CSteelMaterialData	*pMatDataUp	= pVBracing->GetSteelMaterialData(STEELMAT_UPPER);
	double	dUp_H		= pMatDataUp->m_dH;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dH;
	double	dUp_B		= pMatDataUp->m_dB;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dB;
	double	dUp_T1		= pMatDataUp->m_dT1;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dT1;
	double	dUp_As		= pMatDataUp->m_dSectionArea;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dSectionArea;
	double	dUp_GammaX	= pMatDataUp->m_dRadiusOfGyration.x;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dRadiusOfGyration.x;
	double	dUp_Length	= pVBracing->GetLengthBeam(0);
	//하현
	CSteelMaterialData	*pMatDataLo	= pVBracing->GetSteelMaterialData(STEELMAT_LOWER);
	double	dDn_H		= pMatDataLo->m_dH;//pMat->GetMatData(nSteelTypeLo,nJewonTypeLo)->m_dH;
	double	dDn_B		= pMatDataLo->m_dB;//pMat->GetMatData(nSteelTypeLo,nJewonTypeLo)->m_dB;
	double	dDn_T1		= pMatDataLo->m_dT1;//pMat->GetMatData(nSteelTypeLo,nJewonTypeLo)->m_dT1;
	double	dDn_As		= pMatDataLo->m_dSectionArea;//pMat->GetMatData(nSteelTypeLo,nJewonTypeLo)->m_dSectionArea;
	double	dDn_GammaX	= pMatDataLo->m_dRadiusOfGyration.x;//pMat->GetMatData(nSteelTypeLo,nJewonTypeLo)->m_dRadiusOfGyration.x;
	double	dDn_Length	= pVBracing->GetLengthBeam(1);
	//사재
	CSteelMaterialData	*pMatDataDia	= pVBracing->GetSteelMaterialData(STEELMAT_DIAGONAL);
	double	dC_H		= pMatDataDia->m_dH;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dH;
	double	dC_B		= pMatDataDia->m_dB;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dB;
	double	dC_T1		= pMatDataDia->m_dT1;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dT1;
	double	dSd_As		= pMatDataDia->m_dSectionArea;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dSectionArea;
	double	dC_GammaX	= pMatDataDia->m_dRadiusOfGyration.x;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dRadiusOfGyration.x;
	double	dC_Length	= pVBracing->GetLengthBeam(2);
	//
	double dHeightFrame = (pVBracing->GetLengthFrameLine(4));	// 프레임의 수직높이
	double dWidthHalf   = toM(pVBracing->GetLengthFrameLine(1)/2);// 프레임의 수평높이/2
	double dA			= (ABS(pGir->m_dGirderCenter-pGir->m_pRight->m_dGirderCenter));
	double dAngRad		= 0;
	double dUp_GammaMin	=MIN(pMatDataUp->m_dRadiusOfGyrationMaxMin.x, pMatDataUp->m_dRadiusOfGyrationMaxMin.y);
	double dDn_GammaMin	=MIN(pMatDataLo->m_dRadiusOfGyrationMaxMin.x, pMatDataLo->m_dRadiusOfGyrationMaxMin.y);
	double dC_GammaMin	=MIN(pMatDataDia->m_dRadiusOfGyrationMaxMin.x,pMatDataDia->m_dRadiusOfGyrationMaxMin.y);
	if(pVBracing->GetType()==TYPE_VBRACING)	///< V형
	{
		CDPoint xyUpDn  = CDPoint(0, -1);
		CDPoint xyLeft  = pVBracing->GetVectorFrame(CVBracing::LEFT);
		dAngRad = xyUpDn.BetweenAngle(xyLeft).GetAngleDegree();
	}
	else
	{
		CDPoint xyDnUp  = CDPoint(0, 1);
		CDPoint xyLeft  = pVBracing->GetVectorFrame(CVBracing::LEFT);
		dAngRad = xyLeft.BetweenAngle(xyDnUp).GetAngleDegree();
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	sText.Format("< 수직브레이싱 %d  - 거더:%d     %s>", nIndex+1, nG+1, GetStationForStringOut(frM(pBx->GetStation()),3,TRUE,TRUE));
	pOutExcel->SetProgress(sText, pOutExcel->GetProgress()+2);
	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1, BLUE, 9, TRUE, TA_LEFT);
	if(pBx->IsJijum())
	{
		pOut->SetXL(pXL, "지점부 수직브레이싱 검토는 아직 미지원 상태입니다."   , m_nSttRow, "C", "C", 2);
		return;
	}

	pOut->InsertImage(pXL, "단면가정_브레이싱역V형", m_nSttRow, "B");
	pOut->SetXL(pXL, "·h",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dHeightFrame,						m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "VBRACING_H", 1);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "·h1",							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, (pVBracing->m_dRH1),			m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "VBRACING_H1", 1);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "R", "R", 1);

	pOut->SetXL(pXL, "·h2",							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, (pVBracing->m_dRH2),			m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "VBRACING_H2", 1);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "R", "R", 1);

	pOut->SetXL(pXL, "·H/2",							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dWidthHalf,						m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "WIDTH_HALF", 1);
	pOut->SetXL(pXL, "m (브레이싱의수평 높이/2)",		m_nSttRow, "R", "R", 1);

	pOut->SetXL(pXL, "·D",								m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dA,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "D", 0);
	pOut->SetXL(pXL, "mm",								m_nSttRow, "R", "R", 1);

	pOut->SetXL(pXL, "·θ",							m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dAngRad,							m_nSttRow, "O", "Q", 0, RED,   9, FALSE, TA_CENTER, "ANGLE");
	pOut->SetXL(pXL, "°",								m_nSttRow, "R", "R", 3);

	pOut->SetXL(pXL, "● 사 용 강 재",					m_nSttRow, "A", "A", 1);
	pOut->SetLineBoxText(pXL, "현 재",					m_nSttRow, "A", "C", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "단 면",					m_nSttRow, "D", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "A (mm²)",				m_nSttRow, "L", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "γ (mm)",				m_nSttRow, "O", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "γmin (mm)",				m_nSttRow, "R", "T", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "L (mm)",					m_nSttRow, "U", "W", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "L / γ",					m_nSttRow, "X", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);
	pOut->SetLineBoxText(pXL, "상 현 재",				m_nSttRow, "A", "C", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "D", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dUp_B,								m_nSttRow, "D", "E", 0, RED,   9, FALSE, TA_CENTER, "UP_B", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dUp_H,								m_nSttRow, "G", "H", 0, RED,   9, FALSE, TA_CENTER, "UP_H", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dUp_T1,							m_nSttRow, "J", "K", 0, RED,   9, FALSE, TA_CENTER, "UP_T", 0);
	pOut->SetLineBoxText(pXL, dUp_As,					m_nSttRow, "L", "N", 1, RED,   9, FALSE, TA_CENTER, "UP_AS", 2);
	pOut->SetLineBoxText(pXL, dUp_GammaX,				m_nSttRow, "O", "Q", 1, RED,   9, FALSE, TA_CENTER, "UP_GAMMAX", 2);
	pOut->SetLineBoxText(pXL, dUp_GammaMin,				m_nSttRow, "R", "T", 1, RED,   9, FALSE, TA_CENTER, "UP_GAMMAMIN", 2);
	sText.Format("=%s", pOut->GetCellRef("D"));
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "W", 1, BLACK, 9, FALSE, TA_CENTER, "UP_L", 2);
	sText.Format("=%s/%s", pOut->GetCellRef("UP_L"), pOut->GetCellRef("UP_GAMMAX"));
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "X", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "", 2);
	SetLines(1);
	pOut->SetLineBoxText(pXL, "사   재",				m_nSttRow, "A", "C", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "D", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dC_B,								m_nSttRow, "D", "E", 0, RED,   9, FALSE, TA_CENTER, "C_B", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dC_H,								m_nSttRow, "G", "H", 0, RED,   9, FALSE, TA_CENTER, "C_H", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dC_T1,								m_nSttRow, "J", "K", 0, RED,   9, FALSE, TA_CENTER, "C_T", 0);
	pOut->SetLineBoxText(pXL, dSd_As,					m_nSttRow, "L", "N", 1, RED,   9, FALSE, TA_CENTER, "C_AS", 2);
	pOut->SetLineBoxText(pXL, dC_GammaX,				m_nSttRow, "O", "Q", 1, RED,   9, FALSE, TA_CENTER, "C_GAMMAX", 2);
	pOut->SetLineBoxText(pXL, dC_GammaMin,				m_nSttRow, "R", "T", 1, RED,   9, FALSE, TA_CENTER, "C_GAMMAMIN", 2);
	sText.Format("=SQRT(%s^2+(%s/2)^2)", pOut->GetCellRef("VBRACING_H"), pOut->GetCellRef("D"));
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "W", 1, BLACK, 9, FALSE, TA_CENTER, "C_L", 2);
	sText.Format("=%s/%s", pOut->GetCellRef("C_L"), pOut->GetCellRef("C_GAMMAX"));
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "X", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "", 2);
	SetLines(1);
	pOut->SetLineBoxText(pXL, "하 현 재",				m_nSttRow, "A", "C", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "D", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dDn_B,								m_nSttRow, "D", "E", 0, RED,   9, FALSE, TA_CENTER, "DN_B", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dDn_H,								m_nSttRow, "G", "H", 0, RED,   9, FALSE, TA_CENTER, "DN_H", 0);
	pOut->SetXL(pXL, "×",								m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dDn_T1,							m_nSttRow, "J", "K", 0, RED,   9, FALSE, TA_CENTER, "DN_T", 0);
	pOut->SetLineBoxText(pXL, dDn_As,					m_nSttRow, "L", "N", 1, RED,   9, FALSE, TA_CENTER, "DN_AS", 2);
	pOut->SetLineBoxText(pXL, dDn_GammaX,				m_nSttRow, "O", "Q", 1, RED,   9, FALSE, TA_CENTER, "DN_GAMMAX", 2);
	pOut->SetLineBoxText(pXL, dDn_GammaMin,				m_nSttRow, "R", "T", 1, RED,   9, FALSE, TA_CENTER, "DN_GAMMAMIN", 2);
	sText.Format("=%s", pOut->GetCellRef("UP_L"));
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "W", 1, BLACK, 9, FALSE, TA_CENTER, "DN_L", 2);
	sText.Format("=%s/%s", pOut->GetCellRef("DN_L"), pOut->GetCellRef("DN_GAMMAX"));
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "X", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "", 2);
	SetLines(2);

	pOut->SetXL(pXL, "가) 하 중 산 정",					m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "1) 풍 하 중",						m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "- 플레이트 거더교의 풍하중 강도",	m_nSttRow, "C", "C", 1);

	pOut->SetLineBoxText(pXL, "단 면 형 상",			m_nSttRow, "C", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "풍 하 중 (N/m)",		m_nSttRow, "G", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);
	pOut->SetLineBoxText(pXL, "1 ≤ B/D < 8",			m_nSttRow, "C", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "[ 400 - 20(B/D) ] D ≥ 600",	m_nSttRow, "G", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);
	pOut->SetLineBoxText(pXL, "8 ≤ B/D",				m_nSttRow, "C", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "240 D ≥ 600",			m_nSttRow, "G", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(1);
	pOut->SetXL(pXL, "- 풍하중 강도 계산",				m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "교량 폭원 (B) =",					m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, dBridgeW,							m_nSttRow, "G", "I", 0, BLUE,  9, FALSE, TA_CENTER, "BRIDGE_W");
	pOut->SetXL(pXL, "m",								m_nSttRow, "J", "J", 1);
	pOut->SetXL(pXL, "교량 높이 (D) =   거더높이 + SLAB두께 + 방음벽 높이", m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "=",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dGirderH,							m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "GIRDER_H");
	pOut->SetXL(pXL, "+",								m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dSlabThick,						m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "SLABTHICK");
	pOut->SetXL(pXL, "+",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dLBangEumH,						m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "BANGEUM_H");
	pOut->SetXL(pXL, "=",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s+%s+%s,3)", pOut->GetCellRef("GIRDER_H"), pOut->GetCellRef("SLABTHICK"), pOut->GetCellRef("BANGEUM_H"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "BRIDGE_H");
	pOut->SetXL(pXL, "m",								m_nSttRow, "V", "V", 1);

	pOut->SetXL(pXL, "B / D",							m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_RIGHT);
	pOut->SetXL(pXL, "=",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BRIDGE_W"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "/",								m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BRIDGE_H"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s/%s,3)", pOut->GetCellRef("BRIDGE_W"), pOut->GetCellRef("BRIDGE_H"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "B/D");
	pOut->SetXL(pXL, "m",								m_nSttRow, "V", "V", 1);
	sText.Format("=IF(%s < 8, \"I ≤ B / D < 8 이므로  Pw = [ 400 - 20 ( B / D ) ] D\",\"      8 ≤ B / D    이므로            Pw = 240 ·D\")", pOut->GetCellRef("B/D"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s < 8, (400 - 20 * %s) * %s, 240 * %s)", pOut->GetCellRef("B/D"), pOut->GetCellRef("B/D"), pOut->GetCellRef("BRIDGE_H"), pOut->GetCellRef("BRIDGE_H"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "PW");
	pOut->SetXL(pXL, "N/m",							m_nSttRow, "V", "V", 1);

	pOut->SetXL(pXL, "Pw",								m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 600.0,								m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "AI", 1);
	pOut->SetXL(pXL, "N/m",							m_nSttRow, "M", "M", 0);
	sText.Format("=IF(%s>=%s,\"≥\",\"＜\")", pOut->GetCellRef("PW"), pOut->GetCellRef("AI"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "I", "I", 1, RED,   9, TRUE,  TA_CENTER);

	pOut->SetXL(pXL, "- 풍하중에 의한 브레이싱의 작용력",	m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "풍하중의 작용범위               :",	m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dGapVBracing,							m_nSttRow, "M", "O", 0, RED,   9, FALSE, TA_CENTER, "MAX_TERM");
	pOut->SetXL(pXL, "m",									m_nSttRow, "P", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "풍하중 계산시 허용응력 할증계수 :"	,m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, 1.2,									m_nSttRow, "M", "O", 1, RED,   9, FALSE, TA_CENTER, "BONUS");
	pOut->SetXL(pXL, "P1",									m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("PW"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "P1_PARM1");
	pOut->SetXL(pXL, "×",									m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("MAX_TERM"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "P1_PARM2");
	pOut->SetXL(pXL, "/",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("BONUS"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "P1_PARM3");
	pOut->SetXL(pXL, "=",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s/%s", pOut->GetCellRef("P1_PARM1"), pOut->GetCellRef("P1_PARM2"), pOut->GetCellRef("P1_PARM3"));
	pOut->SetXL(pXL, sText,									m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "P1");
	pOut->SetXL(pXL, "N",									m_nSttRow, "T", "T", 1, BLACK, 9, FALSE, TA_CENTER);

	m_nSttRow++;
	pOut->SetXL(pXL, "2) 지진 하중",								m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "- 단위길이당 고정하중",						m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "Wd",											m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, "=",											m_nSttRow, "E", "E", 0);
	pOut->SetXL(pXL, dTotGangJae,									m_nSttRow, "F", "H", 0, RED,   9, FALSE, TA_CENTER, "TOT_WEIGHT");
	pOut->SetXL(pXL, "/",											m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dLengthBridge,									m_nSttRow, "J", "L", 0, RED,   9, FALSE, TA_CENTER, "BRIDGE_LENGTH");
	pOut->SetXL(pXL, "=",											m_nSttRow, "M", "M", 0);
	sText.Format("=%s/%s", pOut->GetCellRef("TOT_WEIGHT"), pOut->GetCellRef("BRIDGE_LENGTH"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "WD");
	pOut->SetXL(pXL, "kN/m",										m_nSttRow, "Q", "Q", 1);

	pOut->SetXL(pXL, "- 지진 가속도 계수(A)",						m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "내진등급 :",									m_nSttRow, "D", "D", 0);
	sText.Format("%s", pOpt->GetStrDegreeEarthQuake());
	pOut->SetXL(pXL, sText,											m_nSttRow, "G", "G", 0);
	pOut->SetXL(pXL, "⇒",											m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "위험도  계수 =",								m_nSttRow, "L", "L", 0);
	sText.Format("%.3f", pOpt->GetRiskCoefficient());
	pOut->SetXL(pXL, sText,											m_nSttRow, "P", "Q", 1, RED,   9, FALSE, TA_CENTER, "RISK_COEFF", 2);

	pOut->SetXL(pXL, "내진구역 :",									m_nSttRow, "D", "D", 0);
	sText.Format("%s", pOpt->GetStrRegionEarthQuake());
	pOut->SetXL(pXL, sText,											m_nSttRow, "G", "G", 0);
	pOut->SetXL(pXL, "⇒",											m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "지진구역계수 =",								m_nSttRow, "L", "L", 0);
	sText.Format("%.3f", pOpt->GetZoneCoefficient());
	pOut->SetXL(pXL, sText,											m_nSttRow, "P", "Q", 1, RED,   9, FALSE, TA_CENTER, "ZONE_COEFF", 2);

	pOut->SetXL(pXL, "A",											m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",											m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("RISK_COEFF"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "", 2);
	pOut->SetXL(pXL, "×",											m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("ZONE_COEFF"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",											m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s", pOut->GetCellRef("RISK_COEFF"), pOut->GetCellRef("ZONE_COEFF"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER, "A");

	pOut->SetXL(pXL, "- 단위길이당 지진하중",						m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "He",											m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",											m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("A"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",											m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("WD"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "×",											m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 1000.0,										m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "=",											m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s*%s*1000", pOut->GetCellRef("A"), pOut->GetCellRef("WD"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER, "EARTH_HE");
	pOut->SetXL(pXL, "kN/m",										m_nSttRow, "T", "T", 1);

	pOut->SetXL(pXL, "- 지진하중에 의한 브레이싱의 작용력",			m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "지진하중의 작용범위             :",			m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dGapVBracing,									m_nSttRow, "M", "O", 0, RED,   9, FALSE, TA_CENTER, "ACT_RANGE");
	pOut->SetXL(pXL, "m",											m_nSttRow, "P", "P", 1);
	pOut->SetXL(pXL, "풍하중 계산시 허용응력 할증계수 :",			m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, 1.5,											m_nSttRow, "M", "O", 1, RED,   9, FALSE, TA_CENTER, "EXTRA_COEFF", 2);
	pOut->SetXL(pXL, "수직 브레이싱의 개수            :",			m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, nTotalVBr,										m_nSttRow, "M", "O", 0, RED,   9, FALSE, TA_CENTER, "QTY_VBRACING", 0);
	pOut->SetXL(pXL, "EA",											m_nSttRow, "P", "P", 1);
	pOut->SetXL(pXL, "P2",											m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",											m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("EARTH_HE"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_CENTER, "PARM1");
	pOut->SetXL(pXL, "×",											m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("ACT_RANGE"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "I", "K", 0, BLACK, 9, FALSE, TA_CENTER, "PARM2");
	pOut->SetXL(pXL, "×",											m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "1/",											m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_RIGHT);
	sText.Format("=%s", pOut->GetCellRef("QTY_VBRACING"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_LEFT, "PARM3", 0);
	pOut->SetXL(pXL, "/",											m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("EXTRA_COEFF"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PARM4", 2);
	pOut->SetXL(pXL, "=",											m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=(%s*%s)/%s/%s", pOut->GetCellRef("PARM1"), pOut->GetCellRef("PARM2"), pOut->GetCellRef("PARM3"), pOut->GetCellRef("PARM4"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "P2");
	pOut->SetXL(pXL, "N",											m_nSttRow, "V", "V", 2);

	pOut->SetXL(pXL, "∴",											m_nSttRow, "C", "C", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "P",											m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",											m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=MAX(%s, %s)", pOut->GetCellRef("P1"), pOut->GetCellRef("P2"));
	pOut->SetXL(pXL, sText,											m_nSttRow, "F", "H", 0, BLACK, 9, FALSE, TA_CENTER, "DESIGN_LOAD");
	pOut->SetXL(pXL, "N",											m_nSttRow, "I", "I", 1);

	SetLines(NEXT);

	CString	sLeft, sNum, sRight, sBoltType;
	sBoltType	= pDB->GetMaterial(VB_BOLT);	// F10T
	pOut->SetXL(pXL, "3) 부재력 계산"	 								   , m_nSttRow, "C", "C", 1);												

	//////////////////////////////////////////////////////////////////////////////////////////////////
	pOut->SetXL(pXL, "① 부재력 계산(간략식으로 계산)"	, m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "ⓐ 하현재(ef, fg)"				, m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "Nℓ = -1/2 * He ="				, m_nSttRow, "E", "E", 0);
	sText.Format("=ROUND(-%s/2, 3)", pOut->GetCellRef("DESIGN_LOAD"));
	pOut->SetXL(pXL, sText								, m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Nl", 0);
	pOut->SetXL(pXL, "Kg"								, m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<0, \"(압축)\", \"(인장)\")", pOut->GetCellRef("Nl"));
	pOut->SetXL(pXL, sText								, m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "ⓑ 사재(af, bf)"				, m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "Nd(af) ="						, m_nSttRow, "E", "G", 0, BLACK, 9, FALSE, TA_LEFT);
	sText.Format("=\"- 1 / 2 × Nℓ × d / ℓ = -1/2 x \" &ROUND(%s, 3)& \" × \" &ROUND(SQRT(%s^2+%s^2), 3)& \" / \" &ROUND(%s, 3)",
					pOut->GetCellRef("Nl"), pOut->GetCellRef("VBRACING_H"), pOut->GetCellRef("WIDTH_HALF"), pOut->GetCellRef("WIDTH_HALF"));
	pOut->SetXL(pXL, sText							, m_nSttRow, "H", "H", 1);
	pOut->SetXL(pXL, "=",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=-1/2*%s*SQRT(%s^2+%s^2)/%s", pOut->GetCellRef("Nl"), pOut->GetCellRef("VBRACING_H"), pOut->GetCellRef("WIDTH_HALF"), pOut->GetCellRef("WIDTH_HALF"));
	pOut->SetXL(pXL, sText							, m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "Nd_Af", 0);
	pOut->SetXL(pXL, "kg"							, m_nSttRow, "K", "K", 0);
	sText.Format("=IF(%s<0, \"(압축)\", \"(인장)\")", pOut->GetCellRef("Nd_Af"));
	pOut->SetXL(pXL, sText							, m_nSttRow, "L", "L", 1);

	pOut->SetXL(pXL, "Nd(bf) = -Nd(af) =" 			, m_nSttRow, "E", "I", 0, BLACK, 9, FALSE, TA_LEFT);
	sText.Format("=-%s", pOut->GetCellRef("Nd_Af"));
	pOut->SetXL(pXL, sText							, m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "Nd_bf", 0);
	pOut->SetXL(pXL, "kg"							, m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<0, \"(압축)\", \"(인장)\")", pOut->GetCellRef("Nd_bf"));
	pOut->SetXL(pXL, sText							, m_nSttRow, "N", "O", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "ⓒ 상현재(ab)"						, m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "Nu  = -(He / 2) - Nd(af) × ℓ / d = ", m_nSttRow, "E", "E", 0);
	sText.Format("=ROUND(-%s/2, 3) &\" - \" &ROUND(%s, 3)& \" × \" &ROUND(%s, 3)& \" / \" &ROUND(SQRT(%s^2+%s^2), 3)", 
					pOut->GetCellRef("DESIGN_LOAD"), pOut->GetCellRef("Nd_Af"), pOut->GetCellRef("WIDTH_HALF"), pOut->GetCellRef("WIDTH_HALF"), pOut->GetCellRef("VBRACING_H"));
	pOut->SetXL(pXL, sText									, m_nSttRow, "N", "N", 1);
	pOut->SetXL(pXL, "="									, m_nSttRow, "F", "F", 0);
	sText.Format("=-%s/2-%s*%s/SQRT(%s^2+%s^2)", pOut->GetCellRef("DESIGN_LOAD"), pOut->GetCellRef("Nd_Af"), pOut->GetCellRef("WIDTH_HALF"), pOut->GetCellRef("WIDTH_HALF"), pOut->GetCellRef("VBRACING_H"));
	pOut->SetXL(pXL, sText									, m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "Nu", 0);
	pOut->SetXL(pXL, "kg"									, m_nSttRow, "J", "J", 0);
	sText.Format("=IF(%s<0, \"(압축)\", \"(인장)\")", pOut->GetCellRef("Nu"));
	pOut->SetXL(pXL, sText									, m_nSttRow, "K", "K", 2);

	CString szTextH, szTextB, szTextT, szTextAs, szTextGamma, szTextGammaMin;
	pOut->SetXL(pXL, "나) 부재의 설계",					m_nSttRow, "B", "B", 1);
	for(long n=0; n<3; n++)	
	{
		CString sJuType;
		long	nMaterialNo = 0;
		double	dLength		= 0;
		double	dAllowShear = 0;
		double	dWeldLength	= 0;
		CSteelMaterialData	*pMatData	= NULL;
		if(n==0)
		{
//			nSteelType = nSteelTypeUp;
			pMatData	= pVBracing->GetSteelMaterialData(STEELMAT_UPPER);
			sText.Format("1) 상 현 재");
			szTextH		= _T("UP_H");	szTextB = _T("UP_B");					szTextT = _T("UP_T");
			szTextAs	= _T("UP_AS");	szTextGammaMin	= _T("UP_GAMMAMIN");	szTextGamma = _T("UP_GAMMAX");
			dLength		= dUp_Length;
			nMaterialNo	= VB_UB;
			sJuType		= pDB->GetMaterial(nMaterialNo);
			CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
			CHGAllowStress	AllowStress(pSteel);

			dAllowShear	= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dUp_T1)*100;
			dWeldLength = (pVBracing->m_dULG);
		}
		if(n==1)
		{
//			nSteelType	= nSteelTypeC;
			pMatData	= pVBracing->GetSteelMaterialData(STEELMAT_DIAGONAL);
			sText.Format("2) 사 재");
			szTextH		= _T("C_H");	szTextB = _T("C_B");				szTextT = _T("C_T");
			szTextAs	= _T("C_AS");	szTextGammaMin = _T("C_GAMMAMIN");	szTextGamma = _T("C_GAMMAX");
			dLength		= dC_Length;
			nMaterialNo = VB_CR;
			sJuType		= pDB->GetMaterial(nMaterialNo);
			CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
			CHGAllowStress	AllowStress(pSteel);

			dAllowShear	= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dC_T1)*100;
			dWeldLength = (pVBracing->m_dC1LG);
		}
		if(n==2)
		{
//			nSteelType = nSteelTypeLo;
			pMatData	= pVBracing->GetSteelMaterialData(STEELMAT_LOWER);
			sText.Format("3) 하 현 재");
			szTextH		= _T("DN_B");	szTextB = _T("DN_B");				szTextT = _T("DN_T");
			szTextAs	= _T("DN_AS");	szTextGammaMin = _T("DN_GAMMAMIN");	szTextGamma = _T("DN_GAMMAX");
			dLength		= dDn_Length;
			nMaterialNo	= VB_LB;
			sJuType		= pDB->GetMaterial(nMaterialNo);
			CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
			CHGAllowStress	AllowStress(pSteel);

			dAllowShear	= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dDn_T1)*100;
			dWeldLength = (pVBracing->m_dDLG);
		}
		pOut->SetXL(pXL, sText							, m_nSttRow, "C", "C", 0);
		pOut->SetXL(pXL, "L-"							, m_nSttRow, "H", "H", 0);
		sText.Format("=%s", pOut->GetCellRef(szTextH));
		pOut->SetXL(pXL, sText	 						, m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "H", 0);
		pOut->SetXL(pXL, "×"				 			, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef(szTextB));
		pOut->SetXL(pXL, sText							, m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "B", 0);
		pOut->SetXL(pXL, "×"		 					, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef(szTextT));
		pOut->SetXL(pXL, sText							, m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER, "T1", 0);
		pOut->SetXL(pXL, "×"		 					, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dLength						, m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "LENGTH", 0);
		//재질
		pOut->SetXL(pXL, "(",							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		CString sLeft	= pDB->GetMaterialNumber(nMaterialNo, -1);
		CString sNum	= pDB->GetMaterialNumber(nMaterialNo);
		CString sRight	= pDB->GetMaterialNumber(nMaterialNo, 1);
		pOut->SetXL(pXL, sLeft,							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, sNum,							m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "USESTEEL", 0);
		pOut->SetXL(pXL, sRight,						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, ")",							m_nSttRow, "Z", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
		
		pOut->SetXL(pXL, "단면적                : A"		  , m_nSttRow, "H", "Q", 0);
		sText.Format("=%s", pOut->GetCellRef(szTextAs));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "AREA", 2);
		pOut->SetXL(pXL, "mm²"								  , m_nSttRow, "T", "T", 1);
		pOut->SetXL(pXL, "단면2차 반경의 최소값 :γmin"		  , m_nSttRow, "H", "Q", 0);
		sText.Format("=%s", pOut->GetCellRef(szTextGammaMin));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "GAMMA", 2);
		pOut->SetXL(pXL, "mm"								  , m_nSttRow, "T", "T", 1);

		pOut->SetXL(pXL, "단면2차 반경          :γx(= γy)"  , m_nSttRow, "H", "Q", 0);
		sText.Format("=%s", pOut->GetCellRef(szTextGamma));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER, "GAMMA_X", 2);
		pOut->SetXL(pXL, "mm"								  , m_nSttRow, "T", "T", 1);

		pOut->SetXL(pXL, "ⓐ 세장비 검토"					  , m_nSttRow, "D", "D", 1);
		sText.Format("= \"ℓ/γmin = \" &ROUND(%s/10, 1)& \" / \" &ROUND(%s,1)& \" = \"", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
		sText.Format("=%s/10/%s", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "세장비", 2);
		sText.Format("=IF(%s<120, \"<\", \">\")", pOut->GetCellRef("세장비"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, 120								  , m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
		sText.Format("=IF(%s<120, \"O.K.\", \"N.G\")", pOut->GetCellRef("세장비"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "ⓑ 허용압축응력"					  , m_nSttRow, "D", "D", 1);
		sText.Format("= \"ℓ/γy = \" &ROUND(%s,1)/10& \" / \" &ROUND(%s,1)& \" = \"", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA_X"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
		sText.Format("=%s/10/%s", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA_X"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "K", "L", 0 ,BLACK, 9, FALSE, TA_CENTER, "L_DIVIDE_Y", 2);
		pOut->SetXL(pXL, "(20"								  , m_nSttRow, "M", "M", 0 ,BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(20<%s,\"<\", \">\")", pOut->GetCellRef("L_DIVIDE_Y"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "N", "N", 0 ,BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "ℓ/γ"							  , m_nSttRow, "O", "P", 0 ,BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(%s<93,\"<\", \">\")", pOut->GetCellRef("L_DIVIDE_Y"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "Q", "Q", 0 ,BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "93)"								  , m_nSttRow, "R", "R", 1 ,BLACK, 9, FALSE, TA_CENTER);

		sText.Format("=\"σca = 1400 - 8.4(ℓ/γ - 20) = 1400 - 8.4 * (\"&ROUND(%s, 1)& \" - 20 )\"", pOut->GetCellRef("L_DIVIDE_Y"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=1400-8.4*(%s-20)", pOut->GetCellRef("L_DIVIDE_Y"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "SIGMA_CA", 1);
		pOut->SetXL(pXL, "kg/mm²"							  , m_nSttRow, "J", "J", 1);

		pOut->SetXL(pXL, "GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여야 한다.", m_nSttRow, "E", "E", 0);
		SetLines(2);

		pOut->SetXL(pXL, "저감률은  0.5 + (ℓ/γx) / 1000"	  , m_nSttRow, "E", "E", 1);
		sText.Format("=\"∴ σca＇ = σca×(0.5+(ℓ/γx) / 1000) = \" &ROUND(%s, 0)& \"×(0.5+\" &ROUND(%s, 0)& \" / 1000 )\"",
					pOut->GetCellRef("SIGMA_CA"), pOut->GetCellRef("L_DIVIDE_Y"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 1);
		pOut->SetXL(pXL, "=",								m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*(0.5+%s/1000)", pOut->GetCellRef("SIGMA_CA"), pOut->GetCellRef("L_DIVIDE_Y"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "SIGMA_CA'", 1);
		pOut->SetXL(pXL, "kg/mm²"							  , m_nSttRow, "K", "K", 1);

		pOut->SetXL(pXL, "ⓒ 응력검토"						  , m_nSttRow, "D", "D", 1);
		szTextAs = _T("AREA");
		if(n==1)	//형강이 인장력을 받을때의 유효단면적계산
		{
			pOut->SetXL(pXL, "인장 L형강의 유효단면적     (도로교 설계기준 p89)"	  , m_nSttRow, "E", "E", 1);
			sText.Format("= \"As = A × 3 / 4 = \" &%s& \" × 3 / 4 = \"", pOut->GetCellRef("AREA"));
			pOut->SetXL(pXL, sText							  , m_nSttRow, "E", "E", 0);
			sText.Format("=%s*3/4", pOut->GetCellRef("AREA"));
			pOut->SetXL(pXL, sText							  , m_nSttRow, "N", "O", 0, BLACK, 9, FALSE, TA_LEFT, "AREA_EFF", 2);
			pOut->SetXL(pXL, "mm²"							  , m_nSttRow, "P", "P", 1);
			szTextAs = _T("AREA_EFF");
		}
		
		sText.Format("=\"σ = \" &ROUND(ABS(%s), 3)& \" / \" &%s& \" = \"", pOut->GetCellRef("Nu"), pOut->GetCellRef(szTextAs));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
		sText.Format("=ABS(%s)/%s", pOut->GetCellRef("Nu"), pOut->GetCellRef(szTextAs));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "SIGMA", 1);
		pOut->SetXL(pXL, "kg/mm²"							  , m_nSttRow, "N", "N", 0);
		sText.Format("=IF(%s<%s, \"<\",\">\")", pOut->GetCellRef("SIGMA"), pOut->GetCellRef("SIGMA_CA'"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "σca＇"							  , m_nSttRow, "Q", "Q", 0);
		sText.Format("=IF(%s<%s, \"O.K.\", \"N.G.\")", pOut->GetCellRef("SIGMA"), pOut->GetCellRef("SIGMA_CA'"));
		pOut->SetXL(pXL, sText								  , m_nSttRow, "T", "T", 1);
		SetLines(1);

		pOut->SetXL(pXL, "ⓓ Gusset Plate와의 연결부 검토",		m_nSttRow, "D", "D", 1);
		pOut->SetXL(pXL, "Fillet 용접부의 허용응력 :",			m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "Va",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dAllowShear,							m_nSttRow, "N", "P", 0, RED,   9, FALSE, TA_CENTER, "ALLOW_SHEARSTRESS",0);
		pOut->SetXL(pXL, "N/mm²",							m_nSttRow, "Q", "Q", 1);

		pOut->SetXL(pXL, "Fillet 용접길이          :",			m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "La",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dWeldLength,							m_nSttRow, "N", "P", 0, RED,   9, FALSE, TA_CENTER, "WELD_LENGTH",0);
		pOut->SetXL(pXL, "mm",									m_nSttRow, "Q", "Q", 1);

		double dTw	= pGir->GetThickFactByStation(dSta, G_W);
		double dTu	= pGir->GetThickFactByStation(dSta, G_F_U);
		double dS = toCm(pDataManage->GetFilletWeldStandard(dTu, dTw));

		pOut->SetXL(pXL, "Fillet 용접목두께        :",			m_nSttRow, "E", "E", 0);
		pOut->SetXL(pXL, "Σa",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dS,									m_nSttRow, "N", "O", 0, RED,   9, FALSE, TA_CENTER, "PARAM1", 1);
		pOut->SetXL(pXL, "/",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "√2",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "×",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, 2.0,									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2", 0);
		pOut->SetXL(pXL, "EA",									m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/SQRT(2)*%s", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_CENTER, "WELD_THICK");
		pOut->SetXL(pXL, "mm",									m_nSttRow, "Y", "Y", 1);

		pOut->SetXL(pXL, "v",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "P / Σa·La",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",									m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("Nu"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM1");
		pOut->SetXL(pXL, "/ (",									m_nSttRow, "N", "N", 0);
		sText.Format("=%s", pOut->GetCellRef("WELD_LENGTH"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2", 0);
		pOut->SetXL(pXL, "×",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("WELD_THICK"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM3");
		pOut->SetXL(pXL, ")",									m_nSttRow, "V", "V", 1);
		pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s/(%s*%s)", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"), pOut->GetCellRef("PARAM3"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "V");
		pOut->SetXL(pXL, "kN/mm²",							m_nSttRow, "J", "J", 0);
		sText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("V"), pOut->GetCellRef("ALLOW_SHEARSTRESS"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Va",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("V"), pOut->GetCellRef("ALLOW_SHEARSTRESS"));
		pOut->SetXL(pXL, sText,									m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER);

	}

	CheckGussetPlate(pBx, nG);

}

void CXLDesignBracing::CheckGussetPlate(CPlateBasicIndex *pBx, long nG)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CPlateGirderApp		*pGir			= pDB->GetGirder(nG);
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CVBracing			*pVBracing		= pBx->GetSection()->GetVBracing();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CSteelMaterial		*pMat			= pDataManage->GetSteelMaterial();

	CString szText, szTitle, szTextAllow;
	CString szLeft		= pDB->GetMaterialNumber(VB_GU, -1);
	CString szNum		= pDB->GetMaterialNumber(VB_GU);
	CString szRight		= pDB->GetMaterialNumber(VB_GU, 1);
	CString	szJuType	= pDB->GetMaterial(VB_GU);
	
	CHGBaseSteel	*pSteel	 = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szJuType)];
	CHGAllowStress	AllowStress(pSteel);

	long	nBoltsu		= 0;
	double	dWidth		= 0;
	double	dHeight		= 0;
	double	dThick		= 0;
	double	dAllowStress= 0;
	double	dWeldLength	= 0;
	pOut->SetXL(pXL, "다) Gusset Plate의 검토",					m_nSttRow, "B", "B", 1);

	for(long n=0; n<3; n++)
	{
		dThick		= pVBracing->m_dT;
		if(n==0)	///< 상현재와 사재연결 Gusset
		{
			szTitle		= _T("1) 상현재/사재 연결 Gusset Plate");
			szTextAllow	= _T("fa");
			dWidth		= pVBracing->m_duL3 + pVBracing->m_duL4;
			dHeight		= pVBracing->m_duL1 + pVBracing->m_duL2;
			dAllowStress= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dThick)*100;
			nBoltsu		= pVBracing->m_uB;
		}
		if(n==1)	///< 하현재 연결 Gusset
		{
			szTitle		= _T("2) 하현재 연결 Gusset Plate");
			szTextAllow	= _T("fa");
			dWidth		= pVBracing->m_ddL3 + pVBracing->m_ddL4;
			dHeight		= pVBracing->m_ddL1 + pVBracing->m_ddL2;
			dAllowStress= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dThick)*100;
			nBoltsu		= pVBracing->m_dB;
		}
		if(n==2)	///< 사재와 하현재연결 GussetAA
		{
			szTitle		= _T("3) 사재/하현재 연결 Gusset Plate");
			szTextAllow	= _T("Va");
			dWidth		= pVBracing->m_dC*2;
			dHeight		= pVBracing->m_dc1 + pVBracing->m_dc2;
			dAllowStress= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dThick)*100;
			dWeldLength	= toCm((pVBracing->m_dC1LG+pVBracing->m_dC1RG)*2);
		}
		pOut->SetXL(pXL, szTitle,							m_nSttRow, "C", "C", 1);
		pOut->SetXL(pXL, "· 사용강재           :",			m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, dWidth,							m_nSttRow, "J", "K", 0, RED,   9, FALSE, TA_CENTER, "WIDTH", 0);
		pOut->SetXL(pXL, "×",								m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dThick,							m_nSttRow, "M", "N", 0, RED,   9, FALSE, TA_CENTER, "THICK", 0);
		pOut->SetXL(pXL, "×",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dHeight,							m_nSttRow, "P", "Q", 0, RED,   9, FALSE, TA_CENTER, "HEIGHT", 0);
		szText.Format("( %s %s %s )", szLeft, szNum, szRight);
		pOut->SetXL(pXL, szText,							m_nSttRow, "R", "R", 1, RED);

		pOut->SetXL(pXL, "· 강재의 허용응력    :",			m_nSttRow, "D", "D", 0);
		pOut->SetXL(pXL, szTextAllow,						m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dAllowStress,						m_nSttRow, "L", "N", 0, RED,   9, FALSE, TA_CENTER, "ALLOW_STRETCH_STRESS", 0);
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "O", "O", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "· 응력검토",						m_nSttRow, "D", "D", 1);
		pOut->SetXL(pXL, "P",								m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);

		if(n==0)
		{
			szText.Format("=%s", pOut->GetCellRef("DESIGN_LOAD"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM1");
			pOut->SetXL(pXL, "+",							m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s/COS(%s*PI()/180)", pOut->GetCellRef("DESIGN_LOAD"), pOut->GetCellRef("ANGLE"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2");
			pOut->SetXL(pXL, "×",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=\"cos \"&TEXT(%s,\"0.0\")", pOut->GetCellRef("ANGLE"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM3");
			pOut->SetXL(pXL, "=",							m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s+%s*COS(%s*PI()/180)", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"), pOut->GetCellRef("ANGLE"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "P");
			pOut->SetXL(pXL, "N",							m_nSttRow, "V", "V", 1);
			szText.Format("f");
		}
		else if(n==1)
		{
			szText.Format("=%s", pOut->GetCellRef("DESIGN_LOAD"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "P");
			pOut->SetXL(pXL, "N",							m_nSttRow, "J", "J", 1);
			szText.Format("f");
		}
		else if(n==2)
		{
			pOut->SetXL(pXL, 2,								m_nSttRow, "G", "G", 0, RED,   9, FALSE, TA_CENTER, "PARAM1", 0);
			pOut->SetXL(pXL, "EA",							m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "×",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s/COS(%s*PI()/180)", pOut->GetCellRef("DESIGN_LOAD"), pOut->GetCellRef("ANGLE"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "J", "L", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2");
			pOut->SetXL(pXL, "×",							m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=\"cos \"&TEXT(%s,\"0.0\")", pOut->GetCellRef("ANGLE"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM3");
			pOut->SetXL(pXL, "=",							m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s+%s*COS(%s*PI()/180)", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"), pOut->GetCellRef("ANGLE"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "P");
			pOut->SetXL(pXL, "N",							m_nSttRow, "W", "W", 1);
			szText.Format("V");

		}

		pOut->SetXL(pXL, szText,							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
		szText.Format("=%s", pOut->GetCellRef("P"));
		pOut->SetXL(pXL, szText,							m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM1");
		pOut->SetXL(pXL, "/",								m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		szText.Format("=%s*%s/100", pOut->GetCellRef("WIDTH"), pOut->GetCellRef("THICK"));
		pOut->SetXL(pXL, szText,							m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2");
		pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		szText.Format("=%s/%s", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"));
		pOut->SetXL(pXL, szText,							m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "F");
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		szText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("F"), pOut->GetCellRef("ALLOW_STRETCH_STRESS"));
		pOut->SetXL(pXL, szText,							m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, szTextAllow,						m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		szText.Format("=IF(%s<%s, \"...O.K\", \"...N.G\")", pOut->GetCellRef("F"), pOut->GetCellRef("ALLOW_STRETCH_STRESS"));
		pOut->SetXL(pXL, szText,							m_nSttRow, "W", "W", 1);

		CString szBoltMat		= pDB->GetMaterial(VB_BOLT, pBx);
		CString szBoltSize		= pVBracing->m_ZzsBoltSize;
		CString	szBoltDiaLeft	= szBoltSize.Left(1);
		CString szBoltDigRight	= szBoltSize.Right(2);

		pOut->SetXL(pXL, "· 연결부 검토",					m_nSttRow, "D", "D", 1);
		if(n==0 || n==1)
		{
			pOut->SetXL(pXL, "BOLT    :",					m_nSttRow, "E", "E", 0);
			pOut->SetXL(pXL, szBoltMat,						m_nSttRow, "H", "H", 0, RED);
			pOut->SetXL(pXL, szBoltDiaLeft,					m_nSttRow, "K", "K", 0, RED);
			pOut->SetXL(pXL, szBoltDigRight,				m_nSttRow, "L", "L", 0, RED,   9, FALSE, TA_CENTER, "BOLT_DIA", 0);
			pOut->SetXL(pXL, "(1면마찰)",					m_nSttRow, "N", "N", 1);
			pOut->SetXL(pXL, "ρa",							m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",							m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=IF(%s=22,4800,IF(%s=24,5600,\"NG\"))", pOut->GetCellRef("BOLT_DIA"), pOut->GetCellRef("BOLT_DIA"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM1", 0);
			pOut->SetXL(pXL, "×",							m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, 1.0,							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2", 0);
			pOut->SetXL(pXL, "=",							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s*%s", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "M", "O", 0, BLACK, 9, FALSE, TA_CENTER, "RHO", 0);
			pOut->SetXL(pXL, "N",							m_nSttRow, "P", "P", 1);

			pOut->SetXL(pXL, "필요 볼트수",					m_nSttRow, "E", "E", 1);
			pOut->SetXL(pXL, "n",							m_nSttRow, "F", "F", 0);
			pOut->SetXL(pXL, "=",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s", pOut->GetCellRef("P"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM1");
			pOut->SetXL(pXL, "/",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s", pOut->GetCellRef("RHO"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "H", "J", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2");
			pOut->SetXL(pXL, "=",							m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s/%s", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"));
			pOut->SetXL(pXL, szText,						m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER, "BOLT_QTY", 1);
			pOut->SetXL(pXL, "EA",							m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);

			pOut->SetXL(pXL, "→",							m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, (double)nBoltsu,				m_nSttRow, "P", "Q", 0, RED,   9, FALSE, TA_CENTER, "", 0);
			pOut->SetXL(pXL, "개 사용",						m_nSttRow, "R", "R", 1);

		}
		else
		{
			pOut->SetXL(pXL, "Fillet 용접부의 허용응력 :",			m_nSttRow, "E", "E", 0);
			pOut->SetXL(pXL, "Va",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s", pOut->GetCellRef("ALLOW_STRETCH_STRESS"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "N", "P", 0, RED,   9, FALSE, TA_CENTER, "",0);
			pOut->SetXL(pXL, "MPa",							m_nSttRow, "Q", "Q", 1);

			pOut->SetXL(pXL, "Fillet 용접길이          :",			m_nSttRow, "E", "E", 0);
			pOut->SetXL(pXL, "La",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dWeldLength,							m_nSttRow, "N", "P", 0, RED,   9, FALSE, TA_CENTER, "WELD_LENGTH",0);
			pOut->SetXL(pXL, "mm",									m_nSttRow, "Q", "Q", 1);

			double dSta = pBx->GetStation();
			double dTw	= pGir->GetThickFactByStation(dSta, G_W);
			double dTu	= pGir->GetThickFactByStation(dSta, G_F_U);
			double dS = toCm(pDataManage->GetFilletWeldStandard(dTw, dTu));
			
			pOut->SetXL(pXL, "Fillet 용접목두께        :",			m_nSttRow, "E", "E", 0);
			pOut->SetXL(pXL, "Σa",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, dS,									m_nSttRow, "N", "O", 0, RED,   9, FALSE, TA_CENTER, "PARAM1", 1);
			pOut->SetXL(pXL, "/",									m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "√2",									m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "×",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, 2.0,									m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2", 0);
			pOut->SetXL(pXL, "EA",									m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s/SQRT(2)*%s", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER, "WELD_THICK");
			pOut->SetXL(pXL, "mm",									m_nSttRow, "X", "X", 2);

			pOut->SetXL(pXL, "v",									m_nSttRow, "E", "E", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "P / Σa·La",							m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "=",									m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s", pOut->GetCellRef("P"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "K", "M", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM1");
			pOut->SetXL(pXL, "/ (",									m_nSttRow, "N", "N", 0);
			szText.Format("=%s", pOut->GetCellRef("WELD_LENGTH"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM2", 0);
			pOut->SetXL(pXL, "×",									m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s", pOut->GetCellRef("WELD_THICK"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_CENTER, "PARAM3");
			pOut->SetXL(pXL, ")",									m_nSttRow, "V", "V", 1);
			pOut->SetXL(pXL, "=",									m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=%s/(%s*%s)", pOut->GetCellRef("PARAM1"), pOut->GetCellRef("PARAM2"), pOut->GetCellRef("PARAM3"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "G", "I", 0, BLACK, 9, FALSE, TA_CENTER, "V");
			pOut->SetXL(pXL, "MPa",							m_nSttRow, "J", "J", 0);
			szText.Format("=IF(%s<%s, \"<\", \">\")", pOut->GetCellRef("V"), pOut->GetCellRef("ALLOW_STRETCH_STRESS"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Va",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
			szText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("V"), pOut->GetCellRef("ALLOW_SHEARSTRESS"));
			pOut->SetXL(pXL, szText,								m_nSttRow, "O", "O", 2, BLACK, 9, FALSE, TA_CENTER);

		}

		SetLines(NEXT);
	}
}

void CXLDesignBracing::DesignHBracing()
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

	m_nSttRow = 1;
	CString sText = "";
	pOut->ClearCellRef();
	pOut->SetColsWidth(pXL, 0, 100, 2);
	
	pOut->SetXL(pXL, "8.2 수평브레이싱"		   , m_nSttRow, "B", "B", 1, BLACK, 9);
	pOutExcel->SetProgress("8. 브레이싱의 설계", pOutExcel->GetProgress()+1);

	StressCalc.SetSectionHBracingPos();
	long nHSec = pCalcData->m_ArrSecHBracing.GetSize();

	for(long n=0; n<nHSec; n++)
	{
		CVector vTemp = pCalcData->m_ArrSecHBracing.GetAt(n);
		CPlateGirderApp *pGir = pDB->GetGirder((long)vTemp.x);  
		CPlateBasicIndex *pBx = pGir->GetBxByStation(vTemp.y, BX_CROSSBEAM_VBRACING);
		DesignHBracing(pBx, n);
	}
}

void CXLDesignBracing::DesignHBracing(CPlateBasicIndex *pBx, long nIndex)
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
	
	long nG = pBx->GetNumberGirder();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CString sText;
	sText.Format("< 수평브레이싱 %d  - 거더:%d     %s>", nIndex+1, nG+1, GetStationForStringOut(frM(pBx->GetStation()),3,TRUE,TRUE));

	//////////////////////////////////////////////////////////////////////////////////////
	// pBx가 몇번째 수평브레이싱인지 검토 
	BOOL bLeftSlab = pBx->GetNumberGirder()==0 ? TRUE : FALSE;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxComp = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nBracing = 0;
	while(pBxComp)
	{
		if(pBxComp==pBx)
			break;
		pBxComp = Finder.GetBxNext();
		nBracing++;
	}
	//////////////////////////////////////////////////////////////////////////////////////
	pOutExcel->SetProgress(sText, pOutExcel->GetProgress()+2);
	pOut->SetXL(pXL, sText,	m_nSttRow, "B", "B", 1, BLUE, 9, TRUE, TA_LEFT);

	pOut->SetXL(pXL, "(1) 하중산정" 						   , m_nSttRow, "C", "C", 1);
	pOut->SetXL(pXL, "풍하중 강도 : Pw = "					   , m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, m_dPw									   , m_nSttRow, "I", "J", 0, RED  , 9, FALSE, TA_CENTER, "PW", 0);
	pOut->SetXL(pXL, "kg/m"									   , m_nSttRow, "K", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	
	m_nSttRow++;
	pOut->SetXL(pXL, "작용 풍하중은 상판에 1/2이 작용하고 수평브레이싱에 1/2가 작용한다." , m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "수평브레이싱은 상부수평브레이싱과 하부수평브레이싱이 있으므로 각"   , m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "수평브레이싱은 작용 풍하중의 1/4을 담당하게 된다."			      , m_nSttRow, "D", "D", 1);
	pOut->SetXL(pXL, "그러므로 수평브레이싱에 작용하는 하중강도는"						  , m_nSttRow, "D", "D", 1);
	
	m_nSttRow++;
	sText.Format("=\"Pw＇ =\" &ROUND(%s, 3)& \" / 4 = \"", pOut->GetCellRef("PW"));
	pOut->SetXL(pXL, sText									   , m_nSttRow, "D", "H", 0);
	sText.Format("=ROUND(%s, 3)/4", pOut->GetCellRef("PW"));
	pOut->SetXL(pXL, sText									   , m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER, "PW'", 1);
	pOut->SetXL(pXL, "kg/m"									   , m_nSttRow, "K", "K", 1, BLACK, 9, FALSE, TA_CENTER);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 수평브레이싱은 하부만 존재
	/// 거더의 간격은 현재거더와 다음거더사이의 간격
	////////////////////////////////////////////////////////////////////////////////////////////////////
	CBracing *pHbr = pGir->GetBracingInTotal(nBracing);
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long   nCount = 0;
	double dGapHBracing = 0;
	double dGirderCenter = pGir->m_pRight ? pGir->m_pRight->m_dGirderCenter : pGir->m_pLeft->m_dGirderCenter;
	double dGapGirder   = fabs(pGir->m_dGirderCenter-dGirderCenter);
	while(Finder.GetBxFixNext())
	{
		CPlateBasicIndex *pBxN = Finder.GetBxFixNext();
		if(nCount==nBracing)
		{
			dGapHBracing = ABS(pBxN->GetStation() - pBx->GetStation());	
			break;
		}
		nCount++;
		pBx = Finder.GetBxNext();
	}

	CSteelMaterialData	*pMatData	= pHbr->GetSteelMaterialData();
//	long nSteelType = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_LHBEAM);
//	long nJewonType = pMat->GetJewonType(pBx, CSteelMaterial::BUJE_LHBEAM);

	double dH  = pMatData->m_dH;//pMat->GetMatData(nSteelType,nJewonType)->m_dH;
	double dB  = pMatData->m_dB;//pMat->GetMatData(nSteelType,nJewonType)->m_dB;
	double dT1 = pMatData->m_dT1;//pMat->GetMatData(nSteelType,nJewonType)->m_dT1;
	double dLength = pHbr->m_dLengthBeam[0];
	////////////////////////////////////////////////////////////////////////////////////////////////////
	pOut->SetXL(pXL, "(2) 부재력 계산" 						   , m_nSttRow, "C", "C", 1);
	sText.Format("=%s/4", pOut->GetCellRef("PW"));      
	pOut->SetXL(pXL, sText									   , m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, "kg/m"			 						   , m_nSttRow, "O", "O", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
	///< 도면 그림 삽입
	CAPlateDrawPyung DrawPyung(m_pAPlateOutCalcStd->GetDataManage());
	CDomyun	Dom;
	Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
	Dom.ClearEtt(TRUE);
	double dAreaInfK = DrawPyung.DrawHBracingCalcForXL(&Dom, nBracing, bLeftSlab);
	pOut->InsertDomyunByFixWidth(pXL, &Dom, m_nSttRow);
	SetLines(NEXT);
	//
	pOut->SetXL(pXL, "수평브레이싱간격" , m_nSttRow, "D", "G", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, toM(dGapHBracing)	, m_nSttRow, "H", "I", 0, RED  , 9, FALSE, TA_CENTER, "GAP_HBRACING", 1);
	pOut->SetXL(pXL, "m"			    , m_nSttRow, "J", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "거더간격"			, m_nSttRow, "L", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, toM(dGapGirder)	, m_nSttRow, "N", "O", 0, RED  , 9, FALSE, TA_CENTER, "GAP_GIRDER", 1);
	pOut->SetXL(pXL, "m"			    , m_nSttRow, "P", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "sinθ "			, m_nSttRow, "R", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s/SQRT((%s/2)^2+%s^2)", pOut->GetCellRef("GAP_GIRDER"), pOut->GetCellRef("GAP_HBRACING"), pOut->GetCellRef("GAP_GIRDER"));  
	pOut->SetXL(pXL, sText				, m_nSttRow, "T", "U", 1, RED  , 9, FALSE, TA_CENTER, "SINCETA", 3);
	
	pOut->SetXL(pXL, "bc 구간에 작용하는 전단력 영향선의 면적 =", m_nSttRow, "D", "D", 0);
	pOut->SetXL(pXL, dAreaInfK									, m_nSttRow, "N", "O", 0, RED, 9, FALSE, TA_CENTER, "AREA_SHEAR");
	pOut->SetXL(pXL, "m²"										, m_nSttRow, "P", "P", 1);

	sText.Format("= \"Sbc = \" &ROUND(%s, 3)& \" × \" &%s& \" = \"", pOut->GetCellRef("PW'"), pOut->GetCellRef("AREA_SHEAR"));
	pOut->SetXL(pXL, sText										, m_nSttRow, "D", "I", 0);
	sText.Format("= ROUND(%s, 3)*ROUND(%s,3)", pOut->GetCellRef("PW'"), pOut->GetCellRef("AREA_SHEAR"));
	pOut->SetXL(pXL, sText										, m_nSttRow, "J", "K", 0, BLACK, 9, FALSE, TA_CENTER, "Sbc", 1);
	pOut->SetXL(pXL, "Kg"										, m_nSttRow, "L", "L", 1);

	pOut->SetXL(pXL, "사재 ①의 부재력 : N1 = -Sbc / sinθ ="   , m_nSttRow, "D", "M", 0);
	sText.Format("=-%s/%s", pOut->GetCellRef("Sbc"), pOut->GetCellRef("SINCETA"));
	pOut->SetXL(pXL, sText									    , m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "N1", 1);
	pOut->SetXL(pXL, "kg"									    , m_nSttRow, "Q", "Q", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "사재 ②의 부재력 : N2 =  Sbc / sinθ ="   , m_nSttRow, "D", "M", 0);
	sText.Format("=%s/%s", pOut->GetCellRef("Sbc"), pOut->GetCellRef("SINCETA"));
	pOut->SetXL(pXL, sText									    , m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER, "N2", 1);
	pOut->SetXL(pXL, "kg"									    , m_nSttRow, "Q", "Q", 1, BLACK, 9, FALSE, TA_CENTER);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 단면결정 및 부재력 검토
	////////////////////////////////////////////////////////////////////////////////////////////////////
	pOut->SetXL(pXL, "⊙ 사재 :"					, m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "L-"							, m_nSttRow, "H", "H", 0);
	pOut->SetXL(pXL, dH					 			, m_nSttRow, "I", "J", 0, RED,   9, FALSE, TA_CENTER, "H", 0);
	pOut->SetXL(pXL, "×"				 			, m_nSttRow, "K", "K", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dB					 			, m_nSttRow, "L", "M", 0, RED,   9, FALSE, TA_CENTER, "B", 0);
	pOut->SetXL(pXL, "×"		 					, m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dT1				 			, m_nSttRow, "O", "P", 0, RED,   9, FALSE, TA_CENTER, "T1", 0);
	pOut->SetXL(pXL, "×"		 					, m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dLength						, m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "LENGTH", 0);
	//재질
	pOut->SetXL(pXL, "(",							m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	CString sLeft		= pDB->GetMaterialNumber(HB_B, -1);
	CString sNum		= pDB->GetMaterialNumber(HB_B);
	CString sRight		= pDB->GetMaterialNumber(HB_B,  1);
	pOut->SetXL(pXL, sLeft,							m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, sNum,							m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "USESTEEL", 0);
	pOut->SetXL(pXL, sRight,						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ")",							m_nSttRow, "Z", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	
	double dArea=pMatData->m_dSectionArea;//pMat->GetMatData(nSteelType,nJewonType)->m_dSectionArea;
	pOut->SetXL(pXL, "단면적                : A"		  , m_nSttRow, "H", "Q", 0);
	pOut->SetXL(pXL, dArea								  , m_nSttRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "AREA", 1);
	pOut->SetXL(pXL, "cm²"								  , m_nSttRow, "T", "T", 1);

//	double dGammaMin=MIN(pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyrationMaxMin.x, 
//						 pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyrationMaxMin.y);
	double dGammaMin=MIN(pMatData->m_dRadiusOfGyrationMaxMin.x, pMatData->m_dRadiusOfGyrationMaxMin.y);
	if(dGammaMin<0)
		dGammaMin = MIN(pMatData->m_dRadiusOfGyration.x, pMatData->m_dRadiusOfGyration.y);//pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyration.x,
					//	pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyration.y);
	pOut->SetXL(pXL, "단면2차 반경의 최소값 :γmin"		  , m_nSttRow, "H", "Q", 0);
	pOut->SetXL(pXL, dGammaMin							  , m_nSttRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "GAMMA_MIN", 1);
	pOut->SetXL(pXL, "cm"								  , m_nSttRow, "T", "T", 1);

	double dGammaX=pMatData->m_dRadiusOfGyration.x;//pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyration.x;
	pOut->SetXL(pXL, "단면2차 반경          :γx(= γy)"  , m_nSttRow, "H", "Q", 0);
	pOut->SetXL(pXL, dGammaX							  , m_nSttRow, "R", "S", 0, RED, 9, FALSE, TA_CENTER, "GAMMA_X", 1);
	pOut->SetXL(pXL, "cm"								  , m_nSttRow, "T", "T", 1);

	pOut->SetXL(pXL, "ⓐ 세장비 검토"					  , m_nSttRow, "D", "D", 1);
	sText.Format("= \"ℓ/γmin = \" &ROUND(%s/10, 1)& \" / \" &ROUND(%s,1)& \" = \"", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA_MIN"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
	sText.Format("=%s/10/%s", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA_MIN"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "세장비", 2);
	sText.Format("=IF(%s<150, \"<\", \">\")", pOut->GetCellRef("세장비"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, 150								  , m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("=IF(%s<150, \"O.K.\", \"N.G\")", pOut->GetCellRef("세장비"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetXL(pXL, "ⓑ 허용압축응력"					  , m_nSttRow, "D", "D", 1);
	sText.Format("= \"ℓ/γy = \" &ROUND(%s,1)/10& \" / \" &ROUND(%s,1)& \" = \"", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA_X"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
	sText.Format("=%s/10/%s", pOut->GetCellRef("LENGTH"), pOut->GetCellRef("GAMMA_X"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "K", "L", 0 ,BLACK, 9, FALSE, TA_CENTER, "L_DIVIDE_Y", 2);
	pOut->SetXL(pXL, "(20"								  , m_nSttRow, "M", "M", 0 ,BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(20<%s,\"<\", \">\")", pOut->GetCellRef("L_DIVIDE_Y"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "N", "N", 0 ,BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "ℓ/γ"							  , m_nSttRow, "O", "P", 0 ,BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<93,\"<\", \">\")", pOut->GetCellRef("L_DIVIDE_Y"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "Q", "Q", 0 ,BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "93)"								  , m_nSttRow, "R", "R", 1 ,BLACK, 9, FALSE, TA_CENTER);

	sText.Format("=\"σca = 1400 - 8.4(ℓ/γ - 20) = 1400 - 8.4 * (\"&ROUND(%s, 1)& \" - 20 ) =\"", pOut->GetCellRef("L_DIVIDE_Y"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
	sText.Format("=1400-8.4*(%s-20)", pOut->GetCellRef("L_DIVIDE_Y"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "T", "U", 0, BLACK, 9, FALSE, TA_CENTER, "SIGMA_CA", 1);
	pOut->SetXL(pXL, "kg/cm²"							  , m_nSttRow, "V", "V", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);

	pOut->SetXL(pXL, "GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여야 한다.", m_nSttRow, "E", "E", 0);
	m_nSttRow+=2;

	pOut->SetXL(pXL, "저감률은  0.5 + (ℓ/γx) / 1000"	  , m_nSttRow, "E", "E", 1);
	sText.Format("=\"∴ σca＇ = σca×(0.5+(ℓ/γx) / 1000) = \" &ROUND(%s, 0)& \"×(0.5+\" &ROUND(%s, 0)& \" / 1000 ) =\"",
				pOut->GetCellRef("SIGMA_CA"), pOut->GetCellRef("L_DIVIDE_Y"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
	sText.Format("=%s*(0.5+%s/1000)", pOut->GetCellRef("SIGMA_CA"), pOut->GetCellRef("L_DIVIDE_Y"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "SIGMA_CA'", 1);
	pOut->SetXL(pXL, "kg/cm²"							  , m_nSttRow, "X", "X", 1, BLACK, 9, FALSE, TA_LEFT);

	pOut->SetXL(pXL, "ⓒ 응력검토"						  , m_nSttRow, "D", "D", 1);
	sText.Format("=\"σ = \" &ROUND(MAX(ABS(%s),ABS(%s)), 3)& \" / \" &%s& \" = \"", pOut->GetCellRef("N1"), pOut->GetCellRef("N2"), pOut->GetCellRef("AREA"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "E", "E", 0);
	sText.Format("=MAX(ABS(%s),ABS(%s))/%s", pOut->GetCellRef("N1"), pOut->GetCellRef("N2"), pOut->GetCellRef("AREA"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER, "SIGMA", 3);
	pOut->SetXL(pXL, "kg/cm²"							  , m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("=IF(%s<%s, \"<\",\">\")", pOut->GetCellRef("SIGMA"), pOut->GetCellRef("SIGMA_CA'"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, "σca＇"							  , m_nSttRow, "P", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	sText.Format("=IF(%s<%s, \"O.K.\", \"N.G.\")", pOut->GetCellRef("SIGMA"), pOut->GetCellRef("SIGMA_CA'"));
	pOut->SetXL(pXL, sText								  , m_nSttRow, "R", "R", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);
	m_nSttRow++;
}

void CXLDesignBracing::SetLines(int nLine)
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