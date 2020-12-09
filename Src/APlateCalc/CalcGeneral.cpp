// CalcGeneral.cpp: implementation of the CCalcGeneral class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateCalc/APlateCalc.h"
#include "CalcGeneral.h"
#include "APlateCalcStd.h"
#include "APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcGeneral::CCalcGeneral(CDataManage *pDataManage)
{
	m_pDataManage = pDataManage;
}

CCalcGeneral::~CCalcGeneral()
{

}
double CCalcGeneral::CalcMoveCoeff() const
{
	CPlateBridgeApp* pDB = m_pDataManage->GetBridge();

	double M=0;	// 지점수가 3개이상일때 휨모멘트에 0.8배 
	long GiJumsu = pDB->GetGirdersu();
	
	if(GiJumsu >=3)
		M=0.80;
	else
		M=1.0;
	
	return M;
}

void CCalcGeneral::CalcMovingLoad_I(double L, double& i, double& ii) const
{
	double A = 15, B = 40, C = 0.30;

	ii = A / ( B + L );
	if( ii > C )	i = C;
	else i = ii;
}

double CCalcGeneral::CalcMovingLoad_E(double L) const
{
	double E = 0;
	double A = 0.8, B = 1.140;

	E = A * L + B;

	return E;
}
double CCalcGeneral::CalcMovingLoad_Pr(double A, double I, double E) const
{
	double Pr = 0;
	double B = 1;
	Pr = (A / E) * (B+I);

	return Pr;
}

double CCalcGeneral::CalcMovingLoad_ML(double Pr, double L) const
{
	return ( Pr * L );
}


// 충돌하중 계산
double CCalcGeneral::CalcCollisionLoad_Co(double V) const
{
	double Co = 0;
/*
	double A = 60, B = 750, C = 250;
	double ton = 0.001;	// kgf을 ton으로

	Co = ( V / A )*( V / A ) * B + C;
	Co *=ton;
*/
///~~직선구간: 1.0 ton/m  , 곡선구간 : 2.0 ton/m
	Co = 2.0;
	return Co;
}

double CCalcGeneral::CalcCollisionLoad_Mo(double Co, double H) const
{
	double Mo = 0;
	Mo = Co * H;
	return Mo;
}

// 풍하중 계산
double CCalcGeneral::CalcWindLoad_DB(double B, double D) const
{
	double DB = 0;
	DB = B / D;
	return DB;
}

double CCalcGeneral::CalcWindLoad_DBPw(double B, double D) const
{
	// ① if( 1 <= (B/D) < 8	)	==> [400-20(B/D)]
	// [400-20*(B/D)*D
	
	// ② if( 8 <= (B/D)		==>	240
	// 240 * D

	double DBPw = 0;
	double E = 400, F = 20, H = 240;
	
	if( 1 <= (B / D) && (B / D) < 8 )		DBPw = (E - F*(B/D))*D;
	else if( 8 <= (B/D) )		DBPw = H*D;

	return DBPw;
}

double CCalcGeneral::CalcWindLoad_Pw(double B, double D) const
{
	double Pw = 0;
	double E = 400, F = 20, G = 600, H = 240;
	
	if( 1 <= (B / D) && (B / D) < 8 )
	{
		Pw = (E - F*(B/D))*D;
		if( Pw  >= G )	Pw = E - F*(B/D);
		else			Pw = G / D;
	}
	else if( 8 <= (B/D) )
	{
		Pw = H*D;
		if( Pw > G ) Pw = H;
		else		 Pw = G / D;
	}

	return Pw;
}

double CCalcGeneral::CalcWindLoad_Mw(double Pw, double D) const
{
	double Mw = 0;
	double ton = 0.001;
	Mw = (Pw*ton) * ( D/2 );
	return Mw;
}

// 원심 하중
double CCalcGeneral::CalcCentriodLoad_CF(double V, double R) const
{
	double CF = 0;
	double A = 0.79;
	
	CF = A * (V*V) / R;
	return CF;
}

//~~~~~~~~Pcf계산 잘못됨 V입력대신 100으로 현재 안사용함
double CCalcGeneral::CalcCentriodLoad_Pcf(double CF, double V) const
{
	CCalcData *pData  = m_pDataManage->GetCalcData();	

	double Pcf = 0;
	double B   = pData->DESIGN_CONDITION.m_dPr;

	Pcf = B * CF / V;

	return Pcf;
}

double CCalcGeneral::CalcCentriodLoad_Mcf(double Pcf, double LL) const
{
	double Mcf = 0;
	Mcf = Pcf * LL / 1000;
	return Mcf;
}


void CCalcGeneral::CalcReinRod_As(double Mu,double sigmaY,double sigmaCK,double d,double b, double &As, double &a) const
{
	double a_old,a_new;
	a = 0;
	As = 0;

	a_old=a=0.2*d;
	a_new=0;
	Mu=Mu*100000;
	
	while ((a_new-a_old)>0.00000001 || (a_old-a_new)>0.00000001)
	{
		As=Mu/(0.85*sigmaY*(d-a/2));
		a_new=As*sigmaY/0.85/sigmaCK/b;
		a_old=a;
		a=a_new;
	} 
}

double CCalcGeneral::CalcReinRod_Md(double BendingPi,double As,double sigmaY,double d,double a) const
{
	double Md = 0;
	Md = BendingPi*As*sigmaY*(d-a/2);
	return Md*0.000001;
}

double CCalcGeneral::CalcReinRod_Pused(double UseAs,double b,double d) const
{
	double Pused=0;
	Pused=UseAs/(b*d);
	return Pused;
}

double CCalcGeneral::CalcReinRod_Pmax(double sigmaCK,double sigmaY) const
{
	CCalcData *pCalcData = m_pDataManage->GetCalcData();
	
	double Pmax= 0;
	double Pb  = 0;
	double k1  = 0;
	double Es  = pCalcData->DESIGN_MATERIAL.m_dEs;

	if(sigmaCK>280) 	k1=0.85-(sigmaCK-280)/10*0.007;	
	else				k1=0.85;

	Pb   = (0.85*k1*sigmaCK*0.003)/(sigmaY*(0.003+sigmaY/Es));
	Pmax = 0.75*Pb;

	return Pmax;
}

double CCalcGeneral::CalcReinRod_Pmin(double sigmaY) const
{
	double Pmin=0;
	Pmin = 14/sigmaY;

	return Pmin;
}

double CCalcGeneral::CalcReinRod_a(double As, double sigmaY, double sigmaCK, double b) const
{
	double a=0;
	a = As*sigmaY/(0.85*sigmaCK*b);
	
	return a;
}

// 경험적 설계가능 여부
BOOL CCalcGeneral::CheckExperienceDesign(BOOL bLeft) const
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CCentSeparation		*pSep	= NULL;
	CPlateGirderApp		*pGir	= NULL;
	CPlateBasicIndex	*pBx	= NULL;

	BOOL	bExpDesign	= FALSE;
	BOOL	bGuardWall	= FALSE;
	double	dWidthSlab	= 0;
	double	dLength		= 0;
	double	dSlabThick	= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
	if(pDB->GetGirdersu()==1)	return FALSE;

	if(bLeft)
	{
		pGir		= pDB->GetGirder(0);
		pSep		= pDB->GetCurGuardWall(CCentSeparation::LEFT);
		dWidthSlab	= pDB->m_dWidthSlabLeft;
	}
	else
	{
		pGir		= pDB->GetGirder(pDB->GetGirdersu()-1);
		pSep		= pDB->GetCurGuardWall(CCentSeparation::RIGHT);
		dWidthSlab	= pDB->m_dWidthSlabRight;
	}


	CPlateBxFinder		Finder(pGir);
	pBx		= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	dLength	= dWidthSlab - pGir->GetThickJewon(G_W, pBx)/2;
	bGuardWall	= pSep ? TRUE : FALSE;

	if(bGuardWall)
	{
		if(dLength > dSlabThick*3)	bExpDesign = TRUE;
		else						bExpDesign = FALSE;
	}
	else
	{
		if(dLength > dSlabThick*5)	bExpDesign = TRUE;
		else						bExpDesign = FALSE;
	}

	return bExpDesign;
}

///////////////////////////////////////////////////////////////
///*중앙부바닥판의 지간[도로교 설계기준 3.6.1.3 바닥판의 지간
///1. 단순판의 경우(거더가 2개일때)는 지지보의 중심간격과 순지간에
///   바닥판 두께를 더한값중 작은값을 사용
///2. 연속판의 경우(거더가 3개이상일경우)는 인점한 상부 플랜지의 돌출폭 중앙점 사이의 거리
///////////////////////////////////////////////////////////////
double CCalcGeneral::CalcCentPart_L() const
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	CPlateBridgeApp	*pBridge= m_pDataManage->GetBridge();
	CPlateBasicIndex *pBx	= NULL;
	CPlateBasicIndex *pBxEnd= NULL;

	double	dResult			= 0;
	long	nDesignMethod	= pOpt->GetSlabCentDesignMethod();
	long	nGSu			= pDB->GetGirdersu();

	if(pBridge->IsTUGir())
	{
		double	dWidthBindConc	= pBridge->m_BindConc.m_dWidth;
		double	dLengthPure		= 0;//순지간

		for(long nG=0; nG< nGSu-1; nG++)
		{
			CPlateGirderApp* pGir = pDB->GetGirder(nG);
			CPlateGirderApp* pGirN = pDB->GetGirder(nG+1);
			CPlateBxFinder Finder(pGir);
			pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			pBxEnd = Finder.GetBxLast (BX_CROSSBEAM_VBRACING);
			while(pBx)
			{
				CPlateCrossBeam* pC = pBx->GetCrossBeam();
				if(!pC) 
				{
					pBx = Finder.GetBxNext();
					continue;
				}

				// 2008.08.13 simdongyoun - 바닥판 중앙지간 길이 변경(구속콘크리트 1/4 지점간의 거리)
//				dLengthPure	= (pGir->GetDisFrontPlateWebToWeb(pBx)+pGir->GetThickJewon(G_W,pBx)/2+pGirN->GetThickJewon(G_W,pBx)/2)
//							- dWidthBindConc + (pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch);
				dLengthPure	= (pGir->GetDisFrontPlateWebToWeb(pBx)+pGir->GetThickJewon(G_W,pBx)/2+pGirN->GetThickJewon(G_W,pBx)/2) - dWidthBindConc/2;
				
				dResult = max(dLengthPure, dResult);
				
				pBx = Finder.GetBxNext();
			}
		}
		
		return dResult;
	}

	if(!pBridge->IsTUGir() && !pBridge->m_bTendonInputComplete)
	{
		double dResult1	=	0.0;
		
		for(long nG=0; nG< nGSu-1; nG++)
		{
			CPlateGirderApp  *pGir  = pDB->GetGirder(nG);
			CPlateGirderApp  *pGirN = pDB->GetGirder(nG+1);
			CPlateBasicIndex *pBx   = pGir->GetBxOnJijum(0);
			CPlateBasicIndex *pBxN  = pGirN->GetBxOnJijum(0);
			double dL = ABS(pGir->m_dGirderCenter-pGirN->m_dGirderCenter);
			//	-pGir->GetLengthSharpOnBx(pBx, TRUE)
			//	-pGirN->GetLengthSharpOnBx(pBxN, TRUE);
			
			double dTW	=	pGir->GetThickJewon(G_W,pBx);
			
			dResult = dL - dTW - (pGir->GetMaxWidthFlange(TRUE) - dTW) /2;
			
			dResult1 = max(dResult1, dResult);
		}
		
		return dResult1;
	}
	else
	{
		if(nGSu==1)//플랜지폭(이런 경우는 없음)
		{
			CPlateGirderApp* pGir = pDB->GetGirder(0);
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			dResult = pGir->GetWidthFlangeUpper(pBx);
		}
		else if(nGSu==2)//단순판(가장 큰 지간길이를 리턴)
		{
			double dLengthPure    = 0;//순지간
			double dLengthGirToGir= 0;//거더간격
			for(long nG=0; nG< nGSu-1; nG++)
			{
				CPlateGirderApp* pGir = pDB->GetGirder(nG);
				CPlateGirderApp* pGirN = pDB->GetGirder(nG+1);
				CPlateBxFinder Finder(pGir);
				pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
				pBxEnd = Finder.GetBxLast (BX_CROSSBEAM_VBRACING);
				while(pBx)
				{
					CPlateCrossBeam* pC = pBx->GetCrossBeam();
					if(!pC) 
					{
						pBx = Finder.GetBxNext();
						continue;
					}
					
					dLengthPure     = pGir->GetDisFrontPlateWebToWeb(pBx) - pGir->GetLengthSharpOnBx(pBx, TRUE)*2
						+ (pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch);
					dLengthGirToGir = ABS(pGir->m_dGirderCenter-pGirN->m_dGirderCenter); 
					
					dResult = min(dLengthPure, dLengthGirToGir);
					if(min(dLengthPure, dLengthGirToGir)>dResult)	dResult = min(dLengthPure, dLengthGirToGir);
					
					pBx = Finder.GetBxNext();
				}
			}
		}
		else//연속판
		{
			BOOL bLeftCantilever	= CheckExperienceDesign(TRUE);
			BOOL bRightCantilever	= CheckExperienceDesign(FALSE);
			
			for(long nG=0; nG< nGSu-1; nG++)
			{
				CPlateGirderApp  *pGir  = pDB->GetGirder(nG);
				CPlateGirderApp  *pGirN = pDB->GetGirder(nG+1);
				CPlateBasicIndex *pBx   = pGir->GetBxOnJijum(0);
				CPlateBasicIndex *pBxN  = pGirN->GetBxOnJijum(0);
				double dL = ABS(pGir->m_dGirderCenter-pGirN->m_dGirderCenter)
					-pGir->GetLengthSharpOnBx(pBx, TRUE)
					-pGirN->GetLengthSharpOnBx(pBxN, TRUE);
				
				dResult = max(dL, dResult); 
			}
			if(nDesignMethod==0)
				return	dResult;	// 강도설계법
			else
			{
				CPlateGirderApp		*pGirLeft	= pDB->GetGirder(0);
				CPlateGirderApp		*pGirLeftN	= pDB->GetGirder(1);
				CPlateBasicIndex	*pBxLeft	= pGirLeft->GetBxOnJijum(0);
				CPlateBasicIndex	*pBxLeftN	= pGirLeftN->GetBxOnJijum(0);
				CPlateGirderApp		*pGirRight	= pDB->GetGirder(pDB->GetGirdersu()-1);
				CPlateGirderApp		*pGirRightN	= pDB->GetGirder(pDB->GetGirdersu()-2);
				CPlateBasicIndex	*pBxRight	= pGirLeft->GetBxOnJijum(0);
				CPlateBasicIndex	*pBxRightN	= pGirLeftN->GetBxOnJijum(0);
				double	dLeft	= ABS(pGirLeft->m_dGirderCenter-pGirLeftN->m_dGirderCenter)-pGirLeft->GetLengthSharpOnBx(pBxLeft, TRUE)-pGirLeftN->GetLengthSharpOnBx(pBxLeftN, TRUE);
				double	dRight	= ABS(pGirRight->m_dGirderCenter-pGirRightN->m_dGirderCenter)-pGirRight->GetLengthSharpOnBx(pBxRight, TRUE)-pGirRightN->GetLengthSharpOnBx(pBxRightN, TRUE);
				if(bLeftCantilever && !bRightCantilever)		return	dLeft;
				else if(!bLeftCantilever && bRightCantilever)	return	dRight;
				else if(!bLeftCantilever && !bRightCantilever)	return	max(dLeft, dRight);
				else if(bLeftCantilever && bRightCantilever)	return	dResult;
			}
		}

	}

	
	return dResult;
}

double CCalcGeneral::CalcCentPart_Md(double W, double L) const
{
	double Md = 0;
	double BaseLen = 1000;
	Md = W * ( L * L * BaseLen) / 10;
	return Md;
}

double CCalcGeneral::CalcCentPart_P(double P, double i) const
{
	double Pt = 0;

	Pt = P * (1+i);

	return Pt;

}

double CCalcGeneral::CalcCentPart_Ml(double L, double i, double t, double M) const
{
	//CCalcData	*pCalcData = m_pDataManage->GetCalcData();

	//double	dPr	= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dMl = (L + 0.6) / 9.6 * t * (1+i);
	
	// 연속 바닥판
	if(m_pDataManage->GetBridge()->GetQtyGirder()>2)
		dMl *= 0.8;

	return dMl;
}

double CCalcGeneral::GetWidthBodoDeadLoad(BOOL bLeft, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCentSeparation	*pSep= pDB->GetCurGuardWall(bLeft ? CCentSeparation::LEFT : CCentSeparation::RIGHT);

	double dWebT	= 0;
	double dQuater	= 0;
	double dL       = 0;
	double dWidth	= pSep ? pSep->GetWidth() : 0;
	double dD1		= pSep ? pSep->m_D1 : 0;
	double dTerm	= pBx ? pDB->m_dWidthSlabLeft : pDB->m_dWidthSlabRight;

	if(bLeft)	
	{
		CPlateGirderApp *pGir = pDB->GetGirder(0);
		dWebT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
		dQuater	= pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
		dL      = dTerm - dQuater - dWebT/2 - dWidth - dD1;
	}
	else
	{
		CPlateGirderApp *pGir = pDB->GetGirder(pDB->GetGirdersu()-1);
		dWebT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
		dQuater	= pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
		dL      = dTerm - dQuater - dWebT/2 - dWidth - dD1;	//D1 : 좌측 방호벽의 좌측 빈 공간
	}

	return dL;
}


void CCalcGeneral::AscendSort(CDDWordArray &DArrWheight, CString DArrHeight[])
{
	long Size = DArrWheight.GetSize();
	double Ascend[100];
	CString sAscend[100];
	long i = 0;
	for(i = 0; i < Size; i++)
	{
		Ascend[i] = DArrWheight.GetAt(i);
		sAscend[i] = DArrHeight[i];
	}
	double Min = Ascend[0];
	double temp = 0;
	CString stemp;
	CString sMin = sAscend[0];
	for(i = 1; i < Size; i++)
	{
		for(long j = i; j < Size; j++)
		{
			if(Min > Ascend[j])
			{
				temp = Ascend[j];
				Ascend[j] = Min;
				Min = temp;

				stemp = sAscend[j];
				sAscend[j] = sMin;
				sMin = stemp;
			}
			else continue;
		}
		Ascend[i-1] = Min;
		Min = Ascend[i];

		sAscend[i-1] = sMin;
		sMin = sAscend[i];
	}
	DArrWheight.RemoveAll();
	for(i = 0; i < Size; i++)
	{
		temp = Ascend[i];
		DArrWheight.Add(temp);
		DArrHeight[i] = sAscend[i];
	}
}

// 허용응력을 위한 강종
void CCalcGeneral::GetMaterial(CPlateBasicIndex *pBx, long nMaterial, CString &szLeft, CString &szRight)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CString	szMaterial		= _T("");
	CString szError			= _T("");
	CString szMat			= _T("");
	CString szMessage		= _T("");

	if(nMaterial==G_F_U || nMaterial==G_F_L || nMaterial==G_W)
	{
		CPlateFactChain	*pFactChain	= pBx->GetFactChain(nMaterial);
		szMaterial	= pFactChain->m_strSymbol;

		if(szMaterial.IsEmpty() && m_pDataManage->IsCheckEmptyMaterial())
		{
			CPlateGirderApp		*pGir		= pBx->GetGirder();
			CPlateBasicIndex	*pBxPrev	= pBx;

			if(nMaterial==G_F_U)		szMat.Format(_T("상판"));
			else if(nMaterial==G_F_L)	szMat.Format(_T("하판"));
			else if(nMaterial==G_W)		szMat.Format(_T("복부판"));

			szError.Format("%s에 강종이 설정되지 않은 구간이 존재합니다.\n단면제원 변화구간에서 확인하세요.", szMat);
			
			while(pBxPrev)
			{
				pBxPrev = pGir->GetBxByStationDir(pBxPrev->GetStation(), -1);
				if(pBxPrev)
					pFactChain = pBxPrev->GetFactChain(nMaterial);
				szMaterial	= pFactChain->m_strSymbol;
				if(pBxPrev==NULL || szMaterial.IsEmpty()==FALSE) 
					break;
			}

			szMessage = m_pDataManage->GetCheckMap(szMat);
			
			if(szMessage!=szMat)
			{
				AfxMessageBox(szError);
				m_pDataManage->SetCheckMap(szMat, szMat);
			}
		}
	}

	pDB->GetRegulationSteelMaterial(nMaterial, szMaterial, szLeft, szRight);
}

CPlateBasicIndex* CCalcGeneral::GetCorrectBxIfTaper(long nG, double &dSta)
{
	CPlateBridgeApp* pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx = pGir->GetBxByStation(dSta);
	CPlateBxFinder Finder(pGir);

	CPlateBasicIndex *pBxBendStt = Finder.GetBxFirst(BX_STT_BENDING_UPPER);
	CPlateBasicIndex *pBxBendEnd = Finder.GetBxFirst(BX_END_BENDING_UPPER);

	double dStaSttBending = pBxBendStt ? pBxBendStt->GetStation() : dSta;
	double dStaEndBending = pBxBendEnd ? pBxBendEnd->GetStation() : dSta;

	if(dSta == dStaSttBending && pBxBendStt)
	{
		dSta -= 30;
		pBx		= pGir->GetBxOnJijum(0);
//		dSta	= pBx->GetStation();
	}
	else if(dSta == dStaEndBending && pBxBendEnd)
	{
		dSta += 30;
		pBx		= pGir->GetBxOnJijum(pDB->m_nQtyJigan);
//		dSta	= pBx->GetStation();
	}

	return pBx;
}

//용접 검토 관련...
void CCalcGeneral::GetSectionWeld(long nType)
{
	
}

//Safe Factor
double CCalcGeneral::GetScaceFactor(long nLane)
{
	double dSF	=	0.0;

	if(nLane==3) 
		dSF=	0.9	;
	else if(nLane>3) 
		dSF=	0.75;
	else 
		dSF=	1.0	;
	return dSF;
}


long CCalcGeneral::GetQtyLane(double dWidth)
{
	long	nQtyLane	= 0;
	
	if(dWidth < 3.0 )		nQtyLane = 0;
	else if(dWidth < 6.0 )	nQtyLane = 1;
	else if(dWidth < 9.1 )	nQtyLane = 2;
	else if(dWidth < 12.8)	nQtyLane = 3;
	else if(dWidth < 16.4)	nQtyLane = 4;
	else if(dWidth < 20.1)	nQtyLane = 5;
	else if(dWidth < 23.8)	nQtyLane = 6;
	else if(dWidth < 27.4)	nQtyLane = 7;
	else if(dWidth < 31.1)	nQtyLane = 8;
	else if(dWidth < 34.7)	nQtyLane = 9;
	else if(dWidth < 38.4)	nQtyLane = 10;
	else					nQtyLane = long(dWidth / 3.0);
	
	return nQtyLane;
}
