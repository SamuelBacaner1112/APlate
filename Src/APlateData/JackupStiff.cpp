// JackupStiff.cpp: implementation of the CJackupStiff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "JackupStiff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
  
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJackupStiff::CJackupStiff()
{	
	m_vJewon[0] = CVector(0,0,0);
	m_vJewon[1] = CVector(0,0,0);
	m_dVL = 0;
	m_dVG = 0;
}

CJackupStiff::~CJackupStiff()
{

}

DWORD CJackupStiff::GetLinkType()
{ 
	return LK_JACKUP;
}

void CJackupStiff::Serialize(CArchive& ar)
{
	long nFlag = 1;

	CPlateBxObject::Serialize(ar);

	if( ar.IsStoring() )
	{
		ar << nFlag;
		ar << m_vJewon[0];		
		ar << m_vJewon[1];		
		ar << m_dVL;
		ar << m_dVG;
	}
	else 
	{
		ar >> nFlag;		
		ar >> m_vJewon[0];		
		ar >> m_vJewon[1];		
		if(nFlag>0)
		{
			ar >> m_dVL;
			ar >> m_dVG;
		}
	}
}


CJackupStiff& CJackupStiff::operator=(const CJackupStiff& obj)
{
	if( this == &obj) return *this;

	CPlateBxObject::operator =(obj);
	
	m_vJewon[0] = obj.m_vJewon[0];	
	m_vJewon[1] = obj.m_vJewon[1];	

	return *this;
}

double CJackupStiff::GetHeight(BOOL bLeft) const
{
	long nSide = bLeft ? 0 : 1;
	return m_vJewon[nSide].z;
}
double CJackupStiff::GetThick(BOOL bLeft) const
{
	long nSide = bLeft ? 0 : 1;
	return m_vJewon[nSide].y;
}
double CJackupStiff::GetWidth(BOOL bLeft) const
{
	long nSide = bLeft ? 0 : 1;
	return m_vJewon[nSide].x;
}
CVector CJackupStiff::GetJewon(BOOL bLeft) const
{
	long nSide = bLeft ? 0 : 1;
	return m_vJewon[nSide];
}

BOOL CJackupStiff::IsSameByMarkSection(CPlateBxObject* pLK, BOOL bLeft) const
{
	CJackupStiff* pJackSelf = GetBx()->GetJackupStiff();
	CJackupStiff* pJack = pLK->GetBx()->GetJackupStiff();
	long nSide = bLeft ? 0 : 1;	
	CVector	vJewonSelf,vJewon;	
	
	vJewonSelf = CVector(Round(pJackSelf->m_vJewon[nSide].x,0), Round(pJackSelf->m_vJewon[nSide].y,0), Round(pJackSelf->m_vJewon[nSide].z,0));	
	vJewon = CVector(Round(pJack->m_vJewon[nSide].x,0), Round(pJack->m_vJewon[nSide].y,0), Round(pJack->m_vJewon[nSide].z,0));	
	
	if(vJewonSelf != vJewon) return FALSE;	
	
	return TRUE;
}

void CJackupStiff::CreatePlatePieceObj(CPlatePieceObject *pObj)
{
	/*
	CPlateBasicIndex *pBx  = GetBx();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)GetGirder()->GetBridgeApp();
	CPlatePieceData  *pData = &pDB->m_pBaseChief->m_PieceData;
	CPlateGirderApp	*pGir  = pBx->GetGirder();

	long nSide = GetSide();
	long nMarkBuje = pObj->m_nMarkBuje;
	double H = m_vJewon[0].z;
	double W = GetWidth(bOutSide);
	BOOL bLeft = nSide == -1 ? TRUE : FALSE;
	double GiulWeb = pGir->GetGiulWeb(pBx, bLeft);
	double gH = bLeft ? pGir->GetHeightGirderByStaAng(pBx->GetStation(), -pGir->m_dUWidth/2, pBx->GetAngle())
					  : pGir->GetHeightGirderByStaAng(pBx->GetStation(),  pGir->m_dUWidth/2, pBx->GetAngle());
	double gWdis = pGir->GetWidthDiffBox(pBx, gH, bLeft);
	double WebT = pGir->GetThickJewon(bLeft ? G_W : G_W_R, pBx);
	if (W <= 0)	return;
	
	long nMarkBuje1 = bOutSide ? GI_JA_R : GI_JA_L;
	long nMarkBuje2 = nSide < 0 ? G_W : G_W_R;
	long nScallopUpper = pDB->GetIndexBaseScallop(nMarkBuje1, nMarkBuje2, G_F_U, pBx); // 스,채,모,없
	long nScallopLower = pDB->GetIndexBaseScallop(nMarkBuje1, nMarkBuje2, G_F_L, pBx); // 스,채,모,없

	P_TYPE ScallopTypeUpper = pData->GetPTypeByScallop(nScallopUpper);	//스캘럽 타입 
	P_TYPE ScallopTypeLower = pData->GetPTypeByScallop(nScallopLower);	//스캘럽 타입 

	double R = GetThick(bOutSide) <= 16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
	double C = GetThick(bOutSide) <= 16 ? pGir->m_pSangse->m_Chamfer_C1 : pGir->m_pSangse->m_Chamfer_C2;
	
	double scallopRChampUpper = 0; ////
	double scallopRChampLower = 0;
	if (ScallopTypeUpper == P_TYPE_SCALLOP)
		scallopRChampUpper = R;
	else if (ScallopTypeUpper == P_TYPE_CHAMFER)
		scallopRChampUpper = C;

	if (ScallopTypeLower == P_TYPE_SCALLOP)
		scallopRChampLower = R;
	else if (ScallopTypeLower == P_TYPE_CHAMFER)
		scallopRChampLower = C;

	if (ScallopTypeUpper == P_TYPE_SCALLOP || ScallopTypeLower == P_TYPE_SCALLOP)
	{
		double MinWeldThick = pDB->m_pBaseChief->m_BaseEtcCtl.GetDesignMinLenOfFilletWeld();	//용접 최소 길이
		if (MinWeldThick > W - R - C)
		{
			ScallopTypeLower = P_TYPE_CHAMFER;
			ScallopTypeUpper = P_TYPE_CHAMFER;
			scallopRChampUpper = C;
			scallopRChampLower = C;
		}
	}

	CDPoint A[41];
	pGir->GetXyHunchShop(pBx,A);
	CBaseChief	*pBet = pDB->m_pBaseChief;
	double SlopFlange = (A[7].y - A[2].y)/(A[7].x - A[2].x);
	if (pBet->m_BaseEtcCtl.GetDesignBoxType() != 2)
		SlopFlange =0;

	double Wdir = W * nSide * (bOutSide ? 1 : -1);
	gWdis = gWdis * -nSide;
	pObj->SetClockWise(Wdir > 0);
	CDPoint xy(0, 0);
	if (bOutSide)
		xy.y = WebT * SlopFlange * nSide * (bOutSide ? 1 : -1);

	pObj->AddElement(xy, ScallopTypeUpper|P_TYPE_DIMPOINT, -1, pBx, scallopRChampUpper);
	xy.x = Wdir;
	xy.y = Wdir*SlopFlange;
	if (bOutSide)
		xy.y += WebT * SlopFlange * nSide * (bOutSide ? 1 : -1);
	pObj->AddElement(xy, P_TYPE_CHAMFER|P_TYPE_DIMPOINT, -1, pBx, C);
	xy.x = Wdir + gWdis;
	xy.y = -H;
	pObj->AddElement(xy, P_TYPE_CHAMFER|P_TYPE_DIMPOINT, -1, pBx, C);
	xy.x = gWdis;
	pObj->AddElement(xy, ScallopTypeLower|P_TYPE_DIMPOINT, -1, pBx, scallopRChampLower);
	//
	pObj->RegenVertex();
	*/
}
