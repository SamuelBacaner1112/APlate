// PlateDesigCalc.cpp: implementation of the CPlateDesigCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Aplatecalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SQ(v)	((v)*(v))
#define maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateDesigCalc::CPlateDesigCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd)
{
	m_pDataManage	= pDataManage;
	m_pCalcStd		= pCalcStd;
}

CPlateDesigCalc::~CPlateDesigCalc()
{

}

//용접 검토..
void CPlateDesigCalc::GetMapWeldReview(long nType, CMap <CString, LPCTSTR, double, double>& Map)
{
	CPlateBridgeApp *pDB       = m_pDataManage->GetBridge();
	CGlobarOption	*pOpt	   = m_pDataManage->GetGlobalOption();
	CSteel          *pSteelCtl = m_pDataManage->GetSteel();
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CFemStressCalc   StressCalc(m_pDataManage, m_pCalcStd);

	///< 필렛 용접 치수 검토.
	long	nPosSec	= -1;
	long	nG		= -1;
	double	dSta	= -1;
	ModelCalc.GetMaxForcePositionOfSection(nType+1, nG, dSta, ELE_SHEAR_2, CFemModelingCalc::MAX_ABS, FALSE);	

	if(nG<0)	return;
	double	dStaSec	= ModelCalc.GetStationSecJewon(nType+1, nG, dSta, nPosSec);

	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxSec	= pGir->GetBxByStation(dStaSec);

	double Smax  = fabs(frkN(StressCalc.GetElementForce(ELE_SHEAR_2, 0, nG, dSta, 4)));///< 최대전단력(kN->N)
	double Tfu   = pBxSec->GetFactChain(G_F_U)->m_dFactChainThick;
	double Tfl   = pBxSec->GetFactChain(G_F_L)->m_dFactChainThick;
	double Tw    = pBxSec->GetFactChain(G_W)->m_dFactChainThick;
	double Wfu   = pGir->GetWidthOnStation(dStaSec, TRUE);
	double Wfl   = pGir->GetWidthOnStation(dStaSec, FALSE);
	double Su = m_pDataManage->GetFilletWeldStandard(Tfu, Tw);
	double Sl = m_pDataManage->GetFilletWeldStandard(Tfl, Tw);

	double Sumax = min(Tfu, Tw);
	double Slmax = min(Tfl, Tw);
	double Sumin = sqrt(2*max(Tfu, Tw));
		   Sumin = Sumax >= 6 ? max(Sumin, 6) : Sumin;
	double Slmin = sqrt (2 * max(Tfl, Tw));
		   Slmin = Slmax >= 6 ? max(Slmin, 6) : Slmin;
	double Yvsu=0, Yvsl=0, Iv=0;

	ModelCalc.GetMaxForcePositionOfSection(nType+1, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
	BOOL bPositiveM = StressCalc.IsPositive(ELE_MOMENT3, nG, dSta); 
	StressCalc.GetSectionStress(nG, dSta);
	ModelCalc.GetSectionCoefficient(nG, dSta, TRUE);

	if(pOpt->GetSteelStandardYear() == APPLYYEAR2010)
	{
		Sumin = Su;
		Slmin = Sl;
	}

	if(bPositiveM)	// 정모멘트부
	{
		Yvsu = fabs(ModelCalc.GetMapValueSecCo("Yvsu"));
		Yvsl = fabs(ModelCalc.GetMapValueSecCo("Yvsl"));
		Iv   = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);//ModelCalc.GetMapValueSecCo("Iv33");
	}
	else			// 부모멘트부
	{
		Yvsu = fabs(StressCalc.GetMapValueSecStress("Yvsu"));
		Yvsl = fabs(StressCalc.GetMapValueSecStress("Yvsl"));
		Iv   = StressCalc.GetMapValueSecStress("Irv");
	}
		

	double Qu = Round(Tfu * Wfu * (Yvsu - Tfu),0);
	double Tu = (Round(Smax,3) * Qu) / (Round(Iv,1) * Su*2 / sqrt (2.0));
	double Ql = Round(Tfl * Wfl * (Yvsl - toCm(Tfl)),0);
	double Tl = (Round(Smax,3) * Ql) / (Round(Iv,1) * Sl*2 / sqrt (2.0));

	CString sJuType = pDB->GetMaterial(G_F_U);
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
	CHGAllowStress	AllowStress(pSteel);

	double  Tau     = AllowStress.GetAllowStress(ALLOW_SHEARSTRESS, Tfu);	// MPa

	sJuType    = pDB->GetMaterial(G_F_L);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
	AllowStress.SetBaseSteel(pSteel);
	double Tal = AllowStress.GetAllowStress(ALLOW_SHEARSTRESS, Tfl); ///<  MPa

	Map["Smax"]		= Smax;
	Map["Tfu"]		= Tfu;
	Map["Tfl"]		= Tfl;
	Map["Tw"]		= Tw;
	Map["Yvsu"]		= Yvsu;
	Map["Yvsl"]		= Yvsl;
	Map["Iv"]		= Iv;
	Map["Su"]		= Su;
	Map["Sl"]		= Sl;
	Map["Tu"]		= Tu;
	Map["Tl"]		= Tl;
	Map["Sumin"]	= Sumin;
	Map["Sumax"]	= Sumax;
	Map["Slmin"]	= Slmin;
	Map["Slmax"]	= Slmax;	
	Map["Tau"]		= Tau;
	Map["Tal"]		= Tal;
}

//지점 보강재 용접 검토..
BOOL CPlateDesigCalc::GetMapWeldJijumStiff(CMap <CString, LPCTSTR, double, double>& Map, BOOL bIsPier)
{
 	CSteel          *pSteelCtl	= m_pDataManage->GetSteel();
 	CSteelSection   *pSection	= m_pDataManage->GetSteelSection();
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	CJoint			*pJ			= NULL;
 	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
			
	long Nstiff=0;
	double dRectionMax = 0, Ws = 0, Ts = 0, Ls = 0, Ta = 0;
	double Tau = 0;
	CString ST = pSection->GetMaterialByMarkBuje(V_J_ST);
	pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, bIsPier);
	if(!pJ)	return FALSE;

	CJointResult		*pJResult	= pResult->m_JointResultArr.GetAt(pJ->m_nIdx);
	CPlateBasicIndex	*pBx		= m_pCalcStd->GetBxByJoint(pJ);
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CVStiff *pJijumVStiff = pGir->GetJijumVStiffByJijum(pBx->GetNumberJijum(), 0);

	if(pJijumVStiff==NULL)	return FALSE;

	dRectionMax = maxabs(pJResult->m_vTotalMax[JOINT_REACTION].z, pJResult->m_vTotalMin[JOINT_REACTION].z);
	Ws     = maxabs(pJijumVStiff->m_dWidth[0], pJijumVStiff->m_dWidth[1]);	// 지점 보강재 폭
	Ts     = maxabs(pJijumVStiff->m_dThick[0], pJijumVStiff->m_dThick[1]);	// 지점 보강재 두께
	Ls     = pGir->GetHeightGirderByStaAng(pBx->GetStation());						// 지점 보강재 길이 (복부판 높이)
	Nstiff = pGir->GetQtyJijumVStiffByBx(pBx);								// 지점 보강재 개수
	Tau    = frkN(dRectionMax) /  (Nstiff * Ts * Ls);						// 전단응력

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(ST)];
	CHGAllowStress	AllowStress(pSteel);

	Ta     = AllowStress.GetAllowStress(ALLOW_SHEARSTRESS, Ts);	// 허용전단 응력

	// 지점보강재 용접 검토
	Map["Nstiff"]		= Nstiff;
	Map["Ws"]			= Ws;
	Map["Ts"]			= Ts;
	Map["Ls"]			= Ls;
	Map["Tau"]			= Tau;
	Map["Ta"]			= Ta;
	Map["dRectionMax"]	= dRectionMax;	

	return TRUE;
}

void CPlateDesigCalc::GetMapStud(long nNodeIdx, CMap <CString, LPCTSTR, double, double>& Map)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CFEMManage			*pFEM	= m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();	
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CPlateBasicIndex	*pBx		= m_pCalcStd->GetBxByJoint(pFEM->GetJoint((unsigned short)nNodeIdx));
	CPlateGirderApp		*pCenGir	= pDB->GetGirder(-1);
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CFemStressCalc   StressCalc(m_pDataManage, m_pCalcStd);
	
	long	nG		= pBx->GetNumberGirder();
	double	dSta	= pBx->GetStation();	
	ModelCalc.GetSectionCoefficient(nG, dSta, TRUE);
	double	dAcPn		= ModelCalc.GetMapValueSecCo("Acs_SUM");
	double	dDvc		= ModelCalc.GetMapValueSecCo("dvc");
	double	dQvc		= dAcPn*dDvc;
	double	dIv			= ModelCalc.GetMapValueSecCo("Iv33");
	double	dVli		= 0;
	double	dVd			= frkN(StressCalc.GetElementForce(ELE_SHEAR_2, LC_NEXT, nG, dSta, 0));
	double	dMaxForce	= frkN(StressCalc.GetElementForce(ELE_SHEAR_2, LC_SUM_MAX, nG, dSta, 0));
	double	dMinForce	= frkN(StressCalc.GetElementForce(ELE_SHEAR_2, LC_SUM_MIN, nG, dSta, 0));
	double	dDeadLoad	= frkN(StressCalc.GetElementForce(ELE_SHEAR_2, LC_PREV_S | LC_PREV_C, nG, dSta, 0));

	if(StressCalc.IsPositive(ELE_SHEAR_2, nG, dSta))	dVli = dMaxForce - dDeadLoad;
	else									dVli = dMinForce - dDeadLoad;

	dVd = fabs(dVd);
	dVli = fabs(dVli);
	dVli -= dVd;

	double	dIs			= ModelCalc.GetMapValueSecCo("I33");
	double	dIc			= ModelCalc.GetMapValueSecCo("Igc");
	double	dDv			= ModelCalc.GetMapValueSecCo("dv");
	double	dAs			= ModelCalc.GetMapValueSecCo("As_SUM");
	double	dAc			= ModelCalc.GetMapValueSecCo("As_SLAB");	
	double	dEpsilonS	= pCalcData->DESIGN_MATERIAL.m_dEpsilonS;
	double	dTemparature = pCalcData->DESIGN_MATERIAL.m_dTemparature;
	double	dAlpha		= pCalcData->DESIGN_MATERIAL.m_dAlpha;
	long	nStudSu			= pGir->m_pSangse->m_StudHori_Qty;
	double	dD				= pGir->m_pSangse->m_StudHori_D;
	double	dFck			= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dVd_Vli			= dVd+dVli;
	double	dSv				= dQvc*dVd_Vli/dIv;
	double	dN				= pCalcData->DESIGN_MATERIAL.m_nN;
	double	dN2				= 24/*dN*(1.0 + pCalcData->DESIGN_MATERIAL.m_dCreep*1.5/2.0)*/;
	double	dEst			= pCalcData->DESIGN_MATERIAL.m_dEst;	
	double	dP2				= dEst*dEpsilonS*dAc/dN2;
	double	dF2				= 1.0/( 1.0+dAc/(dN2*dAs) + dAc*pow(dDv,2)/(dIc+dN2*dIs) );
	double	dNcs2			= Round(dP2,3)*Round(dF2,3);

	double	dGirToGir		= pGir->GetDisGirderToGirder(pBx);
	double	dBridgeL		= pDB->GetLengthBridge();	
	double	dShearWidth		= dGirToGir < dBridgeL/10 ? dGirToGir : dBridgeL;
	double	dSs				= 2*dNcs2/dShearWidth;
	double	dP1				= dEst*dAlpha*dTemparature*dAc/dN;
	double	dF1				= 1.0/( 1.0+dAc/(dN*dAs) + dAc*pow(dDv,2)/(dIc+dN*dIs) );
	double	dNcs1			= Round(dP1,3)*Round(dF1,3);
	double	dSt				= 2*dNcs1/dShearWidth;
	double	dQa				= nStudSu * 30 * dD * dD * sqrt(dFck);//nStudSu * 9.5 * dD*dD * sqrt(dFck);
	double	dSpacereq		= dQa/max(dSv+dSt, dSs+dSt);

	Map["Qa"]	= dQa;
	Map["Sv"]	= dSv;
	Map["Ss"]	= dSs;
	Map["St"]	= dSt;
	Map["ReqSpace"]	= dSpacereq;
}

double CPlateDesigCalc::GetMaxGirderHight(CPlateBasicIndex *pBxMax, long nG, double dSttSta, double dEndSta)
{
	CPlateBridgeApp		*pDB	= m_pCalcStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt	= pGir->GetBxByStation(dSttSta);
	CPlateBasicIndex	*pBxEnd	= pGir->GetBxByStation(dEndSta);
	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_JACKUP | BX_JIJUMSTIFF| BX_CROSSBEAM_VBRACING | BX_VSTIFF, pBxStt, pBxEnd);

	double	dMaxHeight	 = 0;

	while(pBx)
	{
		double	dSta	= pBx->GetStation();
		if(dSta>=dSttSta && dSta<=dEndSta)
		{
			if(dMaxHeight < pGir->GetHeightGirderByStaAng(dSta))
			{
				dMaxHeight	= pGir->GetHeightGirderByStaAng(dSta);
				pBxMax		= pBx;
			}
		}
		pBx = Finder.GetBxNext();
	}

	return dMaxHeight;
}
