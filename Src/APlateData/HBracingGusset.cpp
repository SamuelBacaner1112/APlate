// HBracingGusset.cpp: implementation of the CHBracingGusset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "HBracingGusset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHBracingGusset::CHBracingGusset()
{
	m_nConnect		= 0;
	m_nType			= 0;	
	m_ZzsBoltSize	= "M22";
	m_bEdgeCutting	= FALSE;	

	for(long n=0; n<4; n++)
	{
		m_nMatchCol[n]	= 0;
		xyDir[n]		= CDPoint(0,0);
		m_dDis[n]		= 150;
		m_dWidth[n]		= 200;
		m_gW[n]			= 20;
	}
	m_dH[0]	= 0;
	m_dH[1]	= 0;

	m_sA		= 0;
	m_sB		= 0;
	m_sC		= 0;
	m_sD		= 0;
	m_sE		= 1;
	m_sL		= 80;
	m_dT		= 10;
	m_ddH		= 270;

	pSec0		= NULL;
	pSec1		= NULL;
	pSec2		= NULL;
	pSec3		= NULL;
}

CHBracingGusset::~CHBracingGusset()
{
}

BOOL CHBracingGusset::IsSamePlateJewon(CHBracingGusset *pBrGuComp)
{
	if(m_nConnect			!= pBrGuComp->m_nConnect		)return FALSE;
	if(m_nType				!= pBrGuComp->m_nType			)return FALSE;	
	if(m_ZzsBoltSize		!= pBrGuComp->m_ZzsBoltSize		)return FALSE;
	if(m_bEdgeCutting		!= pBrGuComp->m_bEdgeCutting	)return FALSE;
	long n = 0;
	for(n=0; n<4; n++)
	{
		if(m_nMatchCol[n]	!= pBrGuComp->m_nMatchCol[n])	return FALSE;
		if(xyDir[n]			!= pBrGuComp->xyDir[n]		)	return FALSE;
		if(m_dDis[n]		!= pBrGuComp->m_dDis[n]		)	return FALSE;	
	}
	if(m_dH[0]				!= pBrGuComp->m_dH[n]		)	return FALSE;
	if(m_dH[1]				!= pBrGuComp->m_dH[n]		)	return FALSE;

	if(m_sA					!= pBrGuComp->m_sA		)	return FALSE;
	if(m_sB					!= pBrGuComp->m_sB		)	return FALSE;
	if(m_sC					!= pBrGuComp->m_sC		)	return FALSE;
	if(m_sD					!= pBrGuComp->m_sD		)	return FALSE;
	if(m_sE					!= pBrGuComp->m_sE		)	return FALSE;
	if(m_sL					!= pBrGuComp->m_sL		)	return FALSE;
	if(m_gW					!= pBrGuComp->m_gW		)	return FALSE;
	if(m_dT					!= pBrGuComp->m_dT		)	return FALSE;
	if(m_ddH				!= pBrGuComp->m_ddH		)	return FALSE;

	return TRUE;
}

void CHBracingGusset::SerializeForOld(CArchive &ar)
{
	long nAnchorSu, nScallopType, nConnectPos[4];
	double dDis[4], dh1L, dh2L, dh1R, dh2R, duH, dsE, dgW;

	if(ar.IsStoring())
	{
	}
	else
	{
		ar >> m_nConnect;
		ar >> m_nType;
		ar >> nAnchorSu;
		ar >> m_ZzsBoltSize;
		ar >> m_bEdgeCutting;
		m_bEdgeCutting = TRUE;	// 081127 ..수평브레이싱 거세트판은 모서리절단여부를 지원하지 않는다. (무조건절단) 이리나,이종원
		long n = 0;
		for(n=0; n<4; n++)
			ar >> m_nMatchCol[n];
		for(n=0; n<4; n++)
			ar >> dDis[n];

		ar >> dh1L;
		ar >> dh2L;
		ar >> dh1R;
		ar >> dh2R;
		ar >> m_dWidth[0];
		ar >> m_dWidth[1];
		ar >> m_dWidth[2];
		ar >> m_dWidth[3];
		ar >> dgW;
		ar >> m_sA;
		ar >> m_sB;
		ar >> m_sC;
		ar >> m_sD;
		ar >> dsE;
		ar >> m_dT;
		ar >> duH;
		ar >> m_ddH;

		for( n=0; n<4; n++)
			ar >> nConnectPos[n];

		ar >> nScallopType;
		ar >> m_sL;
		for( n=0; n<4; n++)
			ar >> xyDir[n];

		// 기존 double Type이었음
		m_sE = long(dsE);
		for(n=0; n<4; n++)
		{
			if(m_nMatchCol[n]==0)	m_dDis[n]=0;
			else if(m_dDis[n]==0)	m_dDis[n]=150;
			m_gW[n] = dgW;
		}
	}
}

void CHBracingGusset::Serialize(CArchive &ar)
{
//	long nFlag = 1;
//	long nFlag = 2;	// 081127 사교 및 거세트판 작업하면서 다 뜯어고침 lina
	long nFlag = 3;	// 작업중 생성파일도 열리게 하느라..

	double dTemp = 0;
	
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nConnect;
		ar << m_nType;
		ar << m_ZzsBoltSize;
		ar << m_bEdgeCutting;		
		long n = 0;
		for(n=0; n<4; n++)		ar << m_nMatchCol[n];
		for(n=0; n<4; n++)		ar << xyDir[n];
		for(n=0; n<4; n++)		ar << m_dDis[n];
		for(n=0; n<4; n++)		ar << m_dWidth[n];
		for(n=0; n<4; n++)		ar << m_gW[n];
		for(n=0; n<2; n++)		ar << m_dH[n];

		ar << m_sA;
		ar << m_sB;
		ar << m_sC;
		ar << m_sD;
		ar << m_sE;
		ar << m_sL;		
		ar << m_dT;
		ar << m_ddH;
	}
	else	
	{
		ar >> nFlag;

		if(nFlag<2)
		{
			SerializeForOld(ar);
			return;
		}

		ar >> m_nConnect;
		ar >> m_nType;
		ar >> m_ZzsBoltSize;
		ar >> m_bEdgeCutting;		
		long n = 0;
		for(n=0; n<4; n++)	ar >> m_nMatchCol[n];
		for(n=0; n<4; n++)		ar >> xyDir[n];
		for(n=0; n<4; n++)		ar >> m_dDis[n];
		for(n=0; n<4; n++)		ar >> m_dWidth[n];
		for(n=0; n<4; n++)		ar >> m_gW[n];
		
		if(nFlag<3)
			for(n=0; n<2; n++)		ar >> dTemp;
		
		for(n=0; n<2; n++)		ar >> m_dH[n];

		ar >> m_sA;
		ar >> m_sB;
		ar >> m_sC;
		ar >> m_sD;
		ar >> m_sE;
		ar >> m_sL;
		ar >> m_dT;
		ar >> m_ddH;
	}
}

BOOL CHBracingGusset::IsReal()
{
	if(IsReal(0)+IsReal(1)>0)	return TRUE;
	else						return FALSE;
}

BOOL CHBracingGusset::IsReal(long nRow)
{
	long nTotal = 0;
	if(nRow==0)	nTotal = m_nMatchCol[LEFTTOP]	+ m_nMatchCol[RIGHTTOP];
	else		nTotal = m_nMatchCol[LEFTBOTTOM]+ m_nMatchCol[RIGHTBOTTOM];
		
	if(nTotal>0)	return TRUE;
	else			return FALSE;
}

long CHBracingGusset::GetBoltsu()
{
	return (m_sB+1)*min(2,m_sE+1)	* GetMatchSu();
}

CPlateBasicIndex* CHBracingGusset::GetBxBracing()
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp  *pGir = pBx->GetGirder();

	return pGir->GetBxByStationDir(pBx->GetStation()+10, -1, BX_CROSSBEAM_VBRACING);
}

double CHBracingGusset::GetSL()
{
	CGlobarOption *pEtc = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetGlobalOption();
	BOOL bBoltLengthAuto = pEtc->GetBoltLengthAuto();

	if(!bBoltLengthAuto)
		return m_sL;

	CSteelBolt		*pBolt	= ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetSteelBolt();

	CPlateBasicIndex	*pBx		= GetBxBracing();
	CSection			*pSecBr		= pBx->GetSection();
	CBracing			*pBracing	= pBx->GetBracing();

	long		nSteelType	= pBracing->GetSteelMaterialData()->m_nType;
	double		dBeamT		= pBracing->GetSteelMaterialData()->m_dT2;

	if(nSteelType == CSteelMaterialData::LSTEEL || nSteelType == CSteelMaterialData::LVALSTEEL)
		dBeamT = pBracing->GetSteelMaterialData()->m_dT1;

	double dT = m_dT + dBeamT;

	return pBolt->GetBoltLength(GetBx(), HB_BOLT, m_ZzsBoltSize, dT);
}

//주의사항: 전체데이터를 대입한것이 아니고 위치별 이음판의 형태는 유지하고 내부값만 대입시킴(볼트및 속성등)
CHBracingGusset& CHBracingGusset::operator<<=(const CHBracingGusset& Obj)
{
	m_sA	= Obj.m_sA;
	m_sB	= Obj.m_sB;
	m_sC	= Obj.m_sC;
	m_sD	= Obj.m_sD;
	m_sE	= Obj.m_sE;
	m_sL	= Obj.m_sL;
	m_dT	= Obj.m_dT;
		
	m_ZzsBoltSize = Obj.m_ZzsBoltSize;

	return *this;
}

//////////////////////////////////////////////////////////////////////////

// 횡브레이싱가셋트의 최대판갯수는 2장 - 주석의 상하좌우 기준은 평면도상세를 봤을때의 위치임
void CHBracingGusset::CreatePieceObj(CPlatePieceObject *pObj1, CPlatePieceObject *pObj2, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	CPlateBasicIndex *pBx     = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirL   = pGir->m_pLeft;
	CPlateGirderApp  *pGirR   = pGir->m_pRight;	
	CPlateBasicIndex *pBxCross	= pGir->GetBxByStationDir(pBx->GetStation()+10, -1, BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxCrossL	= pGirL ? pBxCross->GetBxMatchByCrossBeam(pGirL, 1) : NULL;
	CPlateBasicIndex *pBxCrossR	= pGirR ? pBxCross->GetBxMatchByCrossBeam(pGirR, 1) : NULL;
	double dTWeb = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint xyHgU[8], xyHgL[8], xyBaseUp(0,0), xyBaseLo(0,0);
	double dBeamWU = pBxCross->GetSection()->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);
	double dBeamWL = pBxCross->GetSection()->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);
	
	CDPoint xyCen	= pGir->GetXyGirderDis(pBx->GetStation()), xyEnd(0,0), xyBeam(0,0), xyMatch(0,0);
	BOOL	bMid	= pBx->IsState(BX_CROSSBEAM_VBRACING) ? FALSE : TRUE;
	double	dStaEnd = 0;
	
	CPlateBasicIndex *pBxL	= (pGirL&&pBx->GetSection()) ?pBx->GetBxMatchByCrossBeam(pGirL, 1):NULL;
	CPlateCrossBeam *pCU	= pBxL ? pBxL->GetCrossBeamLeft() :  NULL;
	CPlateCrossBeam *pCL	= pBx->GetCrossBeam();
	CDPoint vCrossU			= pBxL ? pBxL->GetAngleSectionCrossVBracing() : CDPoint(0,0);
	CDPoint vCrossL			= pBx->GetSection() ? pBx->GetAngleSectionCrossVBracing() : CDPoint(0,0);

	if(pBx->GetSection() && !pBx->IsState(BX_CROSSBEAM_VBRACING))
		vCrossL = vCrossL.RotateInv(CDPoint(-1,0));

	if(vCrossU==CDPoint(0,0))	vCrossU = vGir.RotateInv90();
	if(vCrossL==CDPoint(0,0))	vCrossL = vGir.RotateInv90();

	GetXyMatchLineAndLine(xyCen + vGir.Rotate90()*dTWeb/2, vGir, xyCen, vCrossU, xyBaseUp);
	GetXyMatchLineAndLine(xyCen + vGir.RotateInv90()*dTWeb/2, vGir, xyCen, vCrossL, xyBaseLo);

	BOOL bDefault0 = FALSE;
	BOOL bDefault1 = FALSE;

	m_dH[0] = m_dH[1] = 0;
	// Dir 설정 //////////////////////////////////////////////////////////////
	if(m_nMatchCol[0]==1)
	{		
		if(!pBxCrossL) pBxCrossL = pGirL->GetBxByStation(pBxCross->GetStation(), BX_CROSSBEAM_VBRACING);
		if(!pBxCrossL) return;
		CPlateBasicIndex *pBxCrossLPrev = pBxCrossL->GetBxPrev(BX_CROSSBEAM_VBRACING, 2);
		dStaEnd		= (bMid || !pBxCrossLPrev) ? pBxCrossL->GetStation() : (pBxCrossL->GetStation()+pBxCrossLPrev->GetStation())/2;

		dStaEnd		= pGirL->GetBxByStation(dStaEnd, BX_HBRACINGGUSSET)->GetStation();
		xyEnd		= pGirL->GetXyGirderDis(dStaEnd);					// 좌측상단의 거세트위치
		xyDir[0]	= (xyEnd-xyCen).Unit();
	}
	if(m_nMatchCol[1]==1)
	{		
		CPlateBasicIndex *pBxLN	= pBxCrossL->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
		if(!pBxLN)		pBxLN	= pGir->GetBxOnJijum(pGir->GetBridgeApp()->m_nQtyJigan);
		dStaEnd		= bMid	? pBxLN->GetStation()
							:(pBxCrossL->GetStation()+pBxLN->GetStation())/2;
		dStaEnd		= pGirL->GetBxByStation(dStaEnd, BX_HBRACINGGUSSET)->GetStation();
		xyEnd		= pGirL->GetXyGirderDis(dStaEnd);					// pBxMatch1 : 우측상단의 거세트위치
		xyDir[1]	= (xyEnd-xyCen).Unit();
	}
	if(m_nMatchCol[2]==1)
	{
	    if(!pBxCrossR) return;
		CPlateBasicIndex *pBxCrossRPrev = pBxCrossR->GetBxPrev(BX_CROSSBEAM_VBRACING, 1);
		dStaEnd		= (bMid || !pBxCrossRPrev) ? pBxCrossR->GetStation() : (pBxCrossR->GetStation()+pBxCrossRPrev->GetStation())/2;
		dStaEnd		= pGirR->GetBxByStation(dStaEnd, BX_HBRACINGGUSSET)->GetStation();
		xyEnd		= pGirR->GetXyGirderDis(dStaEnd);	// pBxMatch2 : 좌측하단의 거세트위치		
		xyDir[2]	= (xyEnd-xyCen).Unit();
	}
	if(m_nMatchCol[3]==1)
	{
		if(!pBxCrossR) return;
		dStaEnd		= bMid	? pBxCrossR->GetBxNext(BX_CROSSBEAM_VBRACING, 1)->GetStation()
							: (pBxCrossR->GetStation() + pBxCrossR->GetBxNext(BX_CROSSBEAM_VBRACING, 1)->GetStation())/2;
		dStaEnd		= pGirR->GetBxByStation(dStaEnd, BX_HBRACINGGUSSET)->GetStation();
		xyEnd		= pGirR->GetXyGirderDis(dStaEnd);	// pBxMatch2 : 우측하단의 거세트위치		
		xyDir[3]	= (xyEnd-xyCen).Unit();
	}
	//////////////////////////////////////////////////////////////////////////
	
	if(m_nMatchCol[0]==1)
	{
		if(!pBxCrossL)	pBxCrossL = pGirL->GetBxByStation(pBxCross->GetStation(), BX_CROSSBEAM_VBRACING);
		if(!pBxCrossL) return;
		CPlateBasicIndex *pBxCrossLPrev = pBxCrossL->GetBxPrev(BX_CROSSBEAM_VBRACING, 2);
		pSec0		= (bMid || !pBxCrossLPrev) ? pBxCrossL->GetSection() : pBxCrossLPrev->GetSection();
		dBeamWU		= pSec0->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);

		if(m_dDis[0]<0)
		{
			xyBeam = GetXy25mmOffset(0, pCU, dBeamWU);
			m_dDis[0] = ~(xyCen-xyBeam);
		}

		xyBeam		= xyCen + xyDir[0]*(m_dDis[0]+m_sA+m_sB*m_sC+m_sD);
		xyHgU[1]	= xyBeam + xyDir[0].Rotate90()*(dBeamWU/2+m_gW[0]);
		xyHgU[2]	= xyBeam - xyDir[0].Rotate90()*(dBeamWU/2+m_gW[0]);
		if(m_dWidth[0]<0)
		{
			bDefault0 = TRUE;
			GetXyMatchLineAndLine(xyHgU[1], vGir.Rotate90(), xyBaseUp, vGir, xyHgU[0]);
			m_dWidth[0] = max(m_dWidth[2], ~(xyBaseUp - xyHgU[0]));
		}		
		xyHgU[0]	= xyBaseUp - vGir*m_dWidth[0];
	}
	if(m_nMatchCol[1]==1)
	{		
		pSec1		= pBxCrossL->GetSection();
		dBeamWU		= pSec1->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);

		if(m_dDis[1]<0)
		{
			xyBeam = GetXy25mmOffset(1, pCU, dBeamWU);
			m_dDis[1] = ~(xyCen-xyBeam);
		}

		xyBeam		= xyCen + xyDir[1]*(m_dDis[1]+m_sA+m_sB*m_sC+m_sD);
		xyHgU[5]	= xyBeam + xyDir[1].Rotate90()*(dBeamWU/2+m_gW[1]);
		xyHgU[6]	= xyBeam - xyDir[1].Rotate90()*(dBeamWU/2+m_gW[1]);
		
		if(m_dWidth[1]<0)
		{
			bDefault1 = TRUE;
			GetXyMatchLineAndLine(xyHgU[6], vGir.Rotate90(), xyBaseUp, vGir, xyHgU[7]);
			m_dWidth[1] = max(m_dWidth[3], ~(xyBaseUp-xyHgU[7]));
		}		
		xyHgU[7]	= xyBaseUp + vGir*m_dWidth[1];
	}
	if(m_nMatchCol[2]==1)
	{
		CPlateBasicIndex *pBxCrossPrev = pBxCross->GetBxPrev(BX_CROSSBEAM_VBRACING, 1);
		pSec2		= (bMid || !pBxCrossPrev) ? pBxCross->GetSection() : pBxCrossPrev->GetSection();
		dBeamWL		= pSec2->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);

		if(m_dDis[2]<0)
		{
			xyBeam = GetXy25mmOffset(2, pCL, dBeamWL);
			m_dDis[2] = ~(xyCen-xyBeam);
		}

		xyBeam		= xyCen + xyDir[2]*(m_dDis[2]+m_sA+m_sB*m_sC+m_sD);
		xyHgL[1]	= xyBeam + xyDir[2].RotateInv90()*(dBeamWL/2+m_gW[2]);
		xyHgL[2]	= xyBeam - xyDir[2].RotateInv90()*(dBeamWL/2+m_gW[2]);
		
		if(m_dWidth[2]<0)
		{
			GetXyMatchLineAndLine(xyHgL[1], vGir.RotateInv90(), xyBaseLo, vGir, xyHgL[0]);
			m_dWidth[2] = max(m_dWidth[0], ~(xyBaseLo-xyHgL[0]));
			if(bDefault0 && ~(xyBaseLo-xyHgL[0])>m_dWidth[0])
			{
				m_dWidth[0] = m_dWidth[2];
				xyHgU[0]	= xyBaseUp - vGir*m_dWidth[0];
			}
		}
		
		xyHgL[0]	= xyBaseLo - vGir*m_dWidth[2];
	}
	if(m_nMatchCol[3]==1)
	{
		pSec3		= pBxCross->GetSection();
//		dBeamWL		= pSec3->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);
		dBeamWL		= pBxCross->GetSection()->GetBeamBaseHeight(CSection::SEC_LHBEAM, FALSE, FALSE);

		if(m_dDis[3]<0)
		{
			xyBeam = GetXy25mmOffset(3, pCL, dBeamWL);
			m_dDis[3] = ~(xyCen-xyBeam);
		}

		xyBeam		= xyCen + xyDir[3]*(m_dDis[3]+m_sA+m_sB*m_sC+m_sD);
		xyHgL[5]	= xyBeam + xyDir[3].RotateInv90()*(dBeamWL/2+m_gW[3]);
		xyHgL[6]	= xyBeam - xyDir[3].RotateInv90()*(dBeamWL/2+m_gW[3]);
		
		if(m_dWidth[3]<0)
		{
			GetXyMatchLineAndLine(xyHgL[6], vGir.RotateInv90(), xyBaseLo, vGir, xyHgL[7]);
			m_dWidth[3] = max(m_dWidth[1], ~(xyBaseLo-xyHgL[7]));
			if(bDefault1 && ~(xyBaseLo-xyHgL[7])>m_dWidth[1])
			{
				m_dWidth[1] = m_dWidth[3];
				xyHgU[7]	= xyBaseUp + vGir*m_dWidth[1];
			}
		}
		xyHgL[7]	= xyBaseLo + vGir*m_dWidth[3];
	}

	if(IsReal(0) && pCU)	// 거더 좌측에 가로보 있을때
	{		
		CDPoint vCr = vCrossU.RotateInv(vGir);
		double dCrossT = pCU->GetType()==CR_TYPE_HSTEEL ? pCU->m_T3_CR : 0;
		xyBeam		= xyCen - vCrossU*(pCU->m_P_CR/fabs(vCr.y)+dTWeb/2);
		xyHgU[3]	= xyBeam + vCrossU.RotateInv90()*(pCU->m_dM_CR+dCrossT);
		xyHgU[4]	= xyBeam - vCrossU.RotateInv90()*(pCU->m_dM_CR-dCrossT);

		if(pCU->GetType()==CR_TYPE_HSTEEL)
		{
			if(m_nMatchCol[0]==0)
			{
				xyHgU[4]	= xyHgU[5];
				GetXyMatchLineAndLine(xyHgU[3], vCr, xyHgU[4], vGir, xyHgU[3]);

			}
			else
			{
				xyHgU[3]	= xyHgU[2];
				GetXyMatchLineAndLine(xyHgU[4], vCr, xyHgU[2], vGir, xyHgU[4]);
			}
		}

		if((m_nMatchCol[0]+m_nMatchCol[1])==2)
		{
			for(long n=0; n<8; n++)
				pObj1->AddElement(xyHgU[n], P_TYPE_LINE,-1,pBx);
		}
		m_dH[0] = ~((xyHgU[3]+xyHgU[4])/2 - (xyCen - vCrossU*dTWeb/2));
	}
	else if(m_nMatchCol[0]==1)
	{
		GetXyMatchLineAndLine(xyHgU[2], vCrossU, xyCen - vCrossU*dTWeb/2, vGir, xyMatch);
		m_dH[0] = ~(xyHgU[2] - xyMatch);
	}
	else if(m_nMatchCol[1]==1)
	{
		GetXyMatchLineAndLine(xyHgU[5], vCrossU, xyCen - vCrossU*dTWeb/2, vGir, xyMatch);
		m_dH[0] = ~(xyHgU[5] - xyMatch);
	}

	if(IsReal(1) && pCL)	// 거더 우측에 가로보 있을때
	{
		CDPoint vCr = vCrossL.RotateInv(vGir);
		double dCrossT = pCL->GetType()==CR_TYPE_HSTEEL ? pCL->m_T3_CR : 0;
		xyBeam		= xyCen + vCrossL*(pCL->m_P_CR/fabs(vCr.y)+dTWeb/2);
		xyHgL[4]	= xyBeam + vCrossL.Rotate90()*(pCL->m_dM_CR-dCrossT);
		xyHgL[3]	= xyBeam - vCrossL.Rotate90()*(pCL->m_dM_CR+dCrossT);

		if(pCL->GetType()==CR_TYPE_HSTEEL)
		{			
			if(m_nMatchCol[2]==0)
			{
				xyHgL[4]	= xyHgL[5];
				GetXyMatchLineAndLine(xyHgL[3], vCr, xyHgL[4], vGir, xyHgL[3]);
			}
			else
			{
				xyHgL[3] = xyHgL[2];
				GetXyMatchLineAndLine(xyHgL[4], vCr, xyHgL[2], vGir, xyHgL[4]);
			}
		}

		if((m_nMatchCol[2]+m_nMatchCol[3])==2)
		{
			for(long n=0; n<8; n++)
				pObj2->AddElement(xyHgL[n], P_TYPE_LINE,-1,pBx);
		}
		m_dH[1] = ~((xyHgL[3]+xyHgL[4])/2 - (xyCen + vCrossL*dTWeb/2));
	}
	else if(m_nMatchCol[2]==1)
	{
		GetXyMatchLineAndLine(xyHgL[2], vCrossL, xyCen - vCrossL*dTWeb/2, vGir, xyMatch);
		m_dH[1] = ~(xyHgL[2] - xyMatch);
	}
	else if(m_nMatchCol[3]==1)
	{
		GetXyMatchLineAndLine(xyHgL[5], vCrossL, xyCen - vCrossL*dTWeb/2, vGir, xyMatch);
		m_dH[1] = ~(xyHgL[5] - xyMatch);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//
	// 여기까지 왔을때 브라켓의 좌우에 빔이있고, 가로보도 있을경우 모든 좌표지정 완료-볼트, 스캘럽남음
	//
	//////////////////////////////////////////////////////////////////////////

	
	if((m_nMatchCol[0]+m_nMatchCol[1])==1)
		SetOneHPieceXy(pObj1, xyHgU, TRUE, m_nMatchCol[0]==1?TRUE:FALSE, pCU?TRUE:FALSE, vCrossU);
	else if((m_nMatchCol[0]+m_nMatchCol[1])==2 && !pCU)
		SetTwoHPieceXy(pObj1, xyHgU);

	if((m_nMatchCol[2]+m_nMatchCol[3])==1)
		SetOneHPieceXy(pObj2, xyHgL,FALSE, m_nMatchCol[2]==1?TRUE:FALSE, pCL?TRUE:FALSE, vCrossL);
	else if((m_nMatchCol[2]+m_nMatchCol[3])==2 && !pCL)
		SetTwoHPieceXy(pObj2, xyHgL);

	//////////////////////////////////////////////////////////////////////////
	//
	// 브라켓의 외곽선 완료. 이후 스캘럽을 추가한다.
	//
	//////////////////////////////////////////////////////////////////////////

	if(pObj1->GetObjSize()>0)	SetScallopXy(pObj1, TRUE, vCrossU);
	if(pObj2->GetObjSize()>0)	SetScallopXy(pObj2, FALSE, vCrossL);

	pObj1->RegenVertex();
	pObj2->RegenVertex();
	long n = 0;
	for(long nCol=0; nCol<4; nCol++)
	{
		if(m_nMatchCol[nCol]==0)	continue;
	
		// 하다보니 이런짓을... ㅠㅠ
		CPlatePieceObject *pBolt1 = new CPlatePieceObject(pGir);
		CPlatePieceObject *pBolt2 = new CPlatePieceObject(pGir);
		CPlatePieceObject *pLine1 = new CPlatePieceObject(pGir);		
		CPlatePieceObject *pLine2 = new CPlatePieceObject(pGir);
		
		double dRad = 12;
		if(m_sE==0)
		{
			for(n=0; n<=m_sB; n++)
			{
				pBolt1->AddCircle(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC), dRad);
				pBolt2->AddCircle(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC), dRad);
				pLine1->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC));
				pLine2->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC));
			}
		}
		else
		{

			for(n=0; n<=m_sB; n++)
			{
				pBolt1->AddCircle(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)+xyDir[nCol].Rotate90()*m_sC/2, dRad);
				pBolt2->AddCircle(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)+xyDir[nCol].Rotate90()*m_sC/2, dRad);
				pLine1->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)+xyDir[nCol].Rotate90()*m_sC/2);
				pLine2->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)+xyDir[nCol].Rotate90()*m_sC/2);
			}
			for(n=m_sB; n>=0; n--)
			{
				pBolt1->AddCircle(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)-xyDir[nCol].Rotate90()*m_sC/2, dRad);
				pBolt2->AddCircle(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)-xyDir[nCol].Rotate90()*m_sC/2, dRad);
				pLine1->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)-xyDir[nCol].Rotate90()*m_sC/2);
				pLine2->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA + n*m_sC)-xyDir[nCol].Rotate90()*m_sC/2);				
			}
			pLine1->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA)+xyDir[nCol].Rotate90()*m_sC/2);
			pLine2->AddElement(xyCen+xyDir[nCol]*(m_dDis[nCol] + m_sA)+xyDir[nCol].Rotate90()*m_sC/2);
		}
		
		pLine1->SetEnd();				pLine2->SetEnd();
		pBolt1->SetBoltHole(TRUE);		pBolt2->SetBoltHole(TRUE);
		pBolt1->SetSolid(FALSE);		pBolt2->SetSolid(FALSE);

		if(pObj1->GetObjSize()>0 && bMakeBoltHole)	pObj1->AddChildObj(pBolt1);
		else										delete pBolt1;
		if(pObj2->GetObjSize()>0 && bMakeBoltHole)	pObj2->AddChildObj(pBolt2);
		else										delete pBolt2;

		if(pObj1->GetObjSize()>0 && bMakeBoltHole)	pObj1->AddChildObj(pLine1);
		else										delete pLine1;
		if(pObj2->GetObjSize()>0 && bMakeBoltHole)	pObj2->AddChildObj(pLine2);
		else										delete pLine2;
	}
	
}

// 수평브레이싱 한개 
void CHBracingGusset::SetOneHPieceXy(CPlatePieceObject *pObj, CDPoint *xyHg,BOOL bUpper,  BOOL bLeft, BOOL bCrossBeam, CDPoint vCross)
{
	CPlateBasicIndex *pBx     = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint xyCen	= pGir->GetXyGirderDis(pBx->GetStation());
	double	dTWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	long	nMax	= 0, nMin = 0;	

	if(bLeft)
	{
		CDPoint xyBase(0,0), xyTemp(0,0);
		GetXyMatchLineAndLine(xyCen + (bUpper?vGir.Rotate90():vGir.RotateInv90())*dTWeb/2, vGir, xyCen, vCross, xyBase);

		if(bCrossBeam)	// 가로보
		{
			if(m_dWidth[bUpper?1:3]<0)
			{
				GetXyMatchLineAndLine(xyBase, vGir, xyHg[4], vGir.Rotate90(), xyTemp);
				m_dWidth[bUpper?1:3] = max(100, ~(xyCen-xyTemp));
			}

			xyHg[5]	= xyBase + vGir*m_dWidth[bUpper?1:3];
			nMax = 5;
		}
		else			// 수직브레이싱
		{
			if(m_dWidth[bUpper?1:3]<0)
				m_dWidth[bUpper?1:3] = 100;

			xyHg[4]	= xyBase+ vGir*m_dWidth[bUpper?1:3];
			GetXyMatchLineAndLine(xyHg[2], vGir, xyCen, vGir.RotateInv90(), xyHg[3]);
			xyHg[3] += vGir*m_gW[bUpper?1:3];
			nMax = 4;	
		}

		for(long n=0; n<nMax+1; n++)
			pObj->AddElement(xyHg[n], P_TYPE_LINE,-1,pBx);
	}
	else
	{
		CDPoint xyBase(0,0), xyTemp(0,0);
		GetXyMatchLineAndLine(xyCen + (bUpper?vGir.Rotate90():vGir.RotateInv90())*dTWeb/2, vGir, xyCen, vCross, xyBase);

		if(bCrossBeam)	// 가로보
		{
			if(m_dWidth[bUpper?0:2]<0)
			{				
				GetXyMatchLineAndLine(xyBase, vGir, xyHg[3], vGir.Rotate90(), xyTemp);
				m_dWidth[bUpper?0:2] = max(100, ~(xyCen-xyTemp));
			}

			xyHg[2]	= xyBase - vGir*m_dWidth[bUpper?0:2];
			nMin = 2;
		}
		else			// 수직브레이싱
		{
			if(m_dWidth[bUpper?0:2]<0)
				m_dWidth[bUpper?0:2] = 100;
		
			xyHg[3]	= xyBase - vGir*m_dWidth[bUpper?0:2];
			GetXyMatchLineAndLine(xyHg[5], vGir, xyCen, vGir.RotateInv90(), xyHg[4]);
			xyHg[4] -= vGir*m_gW[bUpper?0:2];
			nMin = 3;
		}

		for(long n=nMin; n<8; n++)
			pObj->AddElement(xyHg[n], P_TYPE_LINE,-1,pBx);
	}
}

// 수평브레이싱 두개 (수직브레이싱 위치) 
void CHBracingGusset::SetTwoHPieceXy(CPlatePieceObject *pObj, CDPoint *xyHg)
{
	CPlateBasicIndex *pBx = GetBx();
	for(long n=0; n<8; n++)
	{
		if(n==3 || n==4)	continue;
		pObj->AddElement(xyHg[n], P_TYPE_LINE,-1,pBx);
	}
}

void CHBracingGusset::SetScallopXy(CPlatePieceObject *pObj, BOOL bUpper, CDPoint vCross)
{
	CPlateBasicIndex *pBx	= (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir	= pBx->GetGirder();
	CPlateBridgeApp	 *pDB	= (CPlateBridgeApp*)pGir->GetBridgeApp();
	
	CDPoint xyCen(0,0),	xyPoint(0,0), xyEnd(0,0);
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint xyCheck1= pObj->GetPoint(0);
	CDPoint xyCheck2= pObj->GetPoint(pObj->GetObjSize()-1);
	double dTWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);

	long nIdx = 0;
	if(dTWeb > 22)		nIdx = 2;
	else if(dTWeb > 12)	nIdx = 1;	
	
	double dScallopA	= pDB->m_dSlotA[nIdx];
	double dScallopR	= pDB->m_dSlotR2[nIdx];

	if(bUpper) vCross = vCross.Rotate(CDPoint(-1,0));
	if(!pBx->IsState(BX_CROSSBEAM_VBRACING))	vCross = bUpper ? vGir.Rotate90() : vGir.RotateInv90();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex	*pBxCur = Finder.GetBxFirst(BX_VSTIFF|BX_CROSSBEAM_VBRACING);

	while (pBxCur)
	{
		if(pBxCur->GetStation() < pBx->GetStation()-pGir->GetWidthHBracingGussetJewon(pBx, bUpper?0:1))
		{
			pBxCur = Finder.GetBxNext();
			continue;
		}

		CDPoint vAng = vCross;
		xyCen	= pGir->GetXyGirderDis(pBxCur->GetStation());
		vGir	= pGir->GetLine()->GetAngleAzimuth(pBxCur->GetStation());
		if(!bUpper)	vGir = vGir.Rotate(CDPoint(-1,0));

		if(!GetXyMatchSegAndLine(xyCheck1, xyCheck2, xyCen, vAng))
		{
			pBxCur = Finder.GetBxNext();
			continue;
		}
		if(pBx->GetStation()+pGir->GetWidthHBracingGussetJewon(pBx, bUpper?0:1) < pBxCur->GetStation())	break;

		if(!pBxCur->IsState(BX_CROSSBEAM_VBRACING))	vAng = vGir.Rotate90();		

		CVStiff *pVStiff = pBxCur->GetSection() ? pBxCur->GetSection()->GetVStiff() : pBxCur->GetJijumStiff();
		double dWidth = pVStiff ? pVStiff->m_dWidth[bUpper?0:1] : 150;

		xyEnd	= xyCen + vAng*(dWidth+dTWeb/2);

		xyPoint = xyCen + vGir.Rotate90()*dTWeb/2 + vGir*dScallopA/2;
		pObj->AddElement(xyPoint, P_TYPE_LINE,-1,pBxCur);

		xyPoint = xyEnd+vAng*dScallopR+vAng.RotateInv90()*dScallopR;
		pObj->AddElement(xyPoint, P_TYPE_LINE,-1,pBxCur);

		xyPoint = xyEnd+vAng*dScallopR;
		pObj->AddElement(xyPoint, P_TYPE_LINE,-1,pBxCur);

		xyPoint = xyEnd+vAng*dScallopR+vAng.Rotate90()*dScallopR;
		pObj->AddElement(xyPoint, P_TYPE_LINE,-1,pBxCur);
		
		xyPoint = xyCen + vGir.Rotate90()*dTWeb/2 - vGir*dScallopA/2;
		pObj->AddElement(xyPoint, P_TYPE_LINE,-1,pBxCur);

		pObj->InsertScallopAtArc(pObj->GetObjSize()-4,dScallopR);
		pObj->InsertScallopAtArc(pObj->GetObjSize()-2,dScallopR);
			
		pBxCur = Finder.GetBxNext();
	}
}

CDPoint CHBracingGusset::GetXy25mmOffset(long nCol, CPlateCrossBeam *pC, double dHBeamW)
{
	CPlateBasicIndex *pBx     = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateBridgeApp	 *pDB	  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	
	BOOL bUpper		= (nCol==0 || nCol==1) ? TRUE : FALSE;
	BOOL bLeft		= (nCol==0 || nCol==2) ? TRUE : FALSE;
	CVBracing	*pVr = NULL;

 	if(bUpper)
 	{
		CPlateBasicIndex *pBXL = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);
		pVr = pBXL->GetSection() ? pBXL->GetSection()->GetVBracing() : NULL;
		
 	}
 	else
 	{
 		pVr = pBx->GetSection() ? pBx->GetSection()->GetVBracing() : NULL;
 	}
	
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint xyCen	= pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint vCr		= pC ? pC->GetVectorPlan() : -pBx->GetAngleSectionCrossVBracing();

	
	double dTWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dOffset	= 25;
	CDPoint xyBase(0,0), xyBeam(0,0), xyTemp(0,0), xyReturn(0,0);
	
	if(bUpper	&& vCr.y<0)	vCr *= -1;	// vCr을 거더에서의 진행방향으로 조정함
	if(!bUpper	&& vCr.y>0)	vCr *= -1;	// vCr을 거더에서의 진행방향으로 조정함

	xyBase = xyCen;
	if(!pC && pVr)	// 수직브레이싱 연결시
	{
		if(bLeft)	xyBase = xyCen - vCr.Rotate90()*(pVr->GetSteelMaterialData(CVBracing::LOWER)->m_dH-pVr->m_vT_G/2+dOffset);
		else		xyBase = xyCen + vCr.Rotate90()*(pVr->m_vT_G/2+pVr->m_dT+dOffset);
	}
	else if(pC && pC->GetType()==CR_TYPE_HSTEEL)
	{
		if(bUpper!=bLeft)	xyBase = xyCen - vCr.Rotate90()*(pC->m_dM_CR+(pC->m_vT_G-pC->m_T3_CR/2)+dOffset);
		else				xyBase = xyCen + vCr.Rotate90()*(pC->m_dM_CR-(pC->m_vT_G-pC->m_T3_CR/2)+dOffset);
	}
	else if(pC && pC->GetType()==CR_TYPE_MANUFACTURE)
	{
		double dDis = max(pC->m_vT_G/2, pC->m_T3_CR/4);
		if(bUpper!=bLeft)	xyBase = xyCen - vCr.Rotate90()*(dDis+dOffset);
		else				xyBase = xyCen + vCr.Rotate90()*(dDis+dOffset);
	}
	else if(pC)
	{
		if(bUpper!=bLeft)	xyBase = xyCen - vCr.Rotate90()*(pC->m_vT_G+dOffset);
		else				xyBase = xyCen + vCr.Rotate90()*(pC->m_vT_G+dOffset);
		// 간섭체크 //////////////////////////////////////////////////////////////
		xyBeam = xyCen + (bUpper!=bLeft ? xyDir[nCol].Rotate90() : xyDir[nCol].RotateInv90())*dHBeamW/2;
		GetXyMatchLineAndLine(xyBase, vCr, xyBeam, xyDir[nCol], xyTemp);
		
		xyReturn = xyBase + vCr*(pC->m_vW_G+dTWeb/2);
		if(bLeft)	xyReturn -= vCr.Rotate90()*pC->m_dM_CR;
		else		xyReturn += vCr.Rotate90()*pC->m_dM_CR;
		if(GetXyMatchSegAndSeg(xyTemp, xyTemp+xyDir[nCol]*3000, xyReturn, xyReturn+vCr*3000, xyTemp))
		{
			if(bUpper!=bLeft)	xyBase = xyCen - vCr.Rotate90()*(pC->m_dM_CR+dOffset);
			else				xyBase = xyCen + vCr.Rotate90()*(pC->m_dM_CR+dOffset);
		}
		//////////////////////////////////////////////////////////////////////////		
	}
	else if(pBx->GetSection() && pBx->GetSection()->GetVStiff())
	{
		CVStiff *pVStiff = pBx->GetSection()->GetVStiff();
		if(pVStiff->m_dWidth[bUpper?0:1] > 0)
		{
			if(bUpper!=bLeft)	xyBase = xyCen - vCr.Rotate90()*(pVStiff->m_dThick[bUpper?0:1]/2+dOffset);
			else				xyBase = xyCen + vCr.Rotate90()*(pVStiff->m_dThick[bUpper?0:1]/2+dOffset);
		}		
	}
	
	// 해당위치에 아무것도 없음 .. 브레이싱간의 이격간격맞춤
	if(xyBase==xyCen)
	{
		if(bUpper)	vCr = (xyDir[0]+xyDir[1]).Unit();
		else		vCr = (xyDir[2]+xyDir[3]).Unit();
		CDPoint vAng = xyDir[nCol].BetweenAngle(vCr);

		double dSlop	= fabs(vAng.x/vAng.y);
		double dDis		= dSlop*(dHBeamW/2);
		dDis += sqrtl(pow(dSlop*(dOffset/2),2)+pow(dOffset/2,2));

		return xyCen+xyDir[nCol]*dDis;
	}

	xyBeam = xyCen + (bUpper!=bLeft ? xyDir[nCol].Rotate90() : xyDir[nCol].RotateInv90())*dHBeamW/2;

	GetXyMatchLineAndLine(xyBase, vCr, xyBeam, xyDir[nCol], xyTemp);
	GetXyMatchLineAndLine(xyTemp, xyDir[nCol].Rotate90(), xyCen, xyDir[nCol], xyReturn);

	// 거더에 인접하면..
	if(GetXyMatchSegAndLine(xyReturn-xyDir[nCol].Rotate90()*dHBeamW/2, xyReturn+xyDir[nCol].Rotate90()*dHBeamW/2,
		xyCen+(bUpper?vGir.Rotate90():vGir.RotateInv90())*(dTWeb/2+dOffset), vGir, xyTemp))
	{
		GetXyMatchLineAndLine(xyBase, vCr, xyCen+(bUpper?vGir.Rotate90():vGir.RotateInv90())*(dTWeb/2+dOffset), vGir, xyTemp);
		GetXyMatchLineAndLine(xyTemp, xyDir[nCol].Rotate90(), xyCen, xyDir[nCol], xyReturn);
	}

	// 지점보강재 간섭체크 ///////////////////////////////////////////////////
	CVStiff *pJStiff = pBx->GetJijumStiff();
	if(!pBx->IsJijum() || !pJStiff) return xyReturn;

	double dThick	= pJStiff->m_dThick[bUpper?0:1];
	double dWidth	= pJStiff->m_dWidth[bUpper?0:1];
	long nQtyJack	= pDB->GetQtyJijumStiff(pBx->GetNumberJijum());

	xyBase = xyCen;
	for(long nJack=0; nJack<nQtyJack; nJack++)
	{
		double dDis		= pDB->m_dJiJumTerm[pBx->GetNumberJijum()][nJack];
		CDPoint vAng	= bUpper ? vGir.Rotate90() : vGir.RotateInv90();
		CDPoint vAngStiff = (dDis==0) ? vCr : vAng;

		CDPoint xyBeamL = xyReturn - xyDir[nCol].Rotate90()*dHBeamW/2;
		CDPoint xyBeamR = xyReturn + xyDir[nCol].Rotate90()*dHBeamW/2;

		// 좌측면
		CDPoint xyStiffL1 = xyCen+vGir*(dDis-dThick/2-dOffset)+vAng*dTWeb/2;
		CDPoint xyStiffL2 = xyStiffL1+vAngStiff*(dWidth+dOffset);
		// 지점보강재에 간섭함
		BOOL bInter = FALSE;
		if(GetXyMatchSegAndSeg(xyStiffL1, xyStiffL2, xyBeamL, xyBeamR,					xyTemp))	bInter = TRUE;
		if(GetXyMatchSegAndSeg(xyStiffL1, xyStiffL2, xyBeamL, xyBeamL+xyDir[nCol]*1000, xyTemp))	bInter = TRUE;
		if(GetXyMatchSegAndSeg(xyStiffL1, xyStiffL2, xyBeamL, xyBeamR+xyDir[nCol]*1000, xyTemp))	bInter = TRUE;
		xyStiffL1 = xyStiffL2 - vAngStiff*dOffset + vGir*dOffset;
		if(bInter)
			xyBase = (~(xyBase-xyCen) > ~(xyStiffL1-xyCen)) ? xyBase : xyStiffL1;

		// 우측면
		CDPoint xyStiffR1 = xyCen+vGir*(dDis+dThick/2+dOffset)+vAng*dTWeb/2;
		CDPoint xyStiffR2 = xyStiffR1+vAngStiff*(dWidth+dOffset);		
		// 지점보강재에 간섭함
		bInter = FALSE;
		if(GetXyMatchSegAndSeg(xyStiffR1, xyStiffR2, xyBeamL, xyBeamR,					xyTemp))	bInter = TRUE;
		if(GetXyMatchSegAndSeg(xyStiffR1, xyStiffR2, xyBeamL, xyBeamL+xyDir[nCol]*1000, xyTemp))	bInter = TRUE;
		if(GetXyMatchSegAndSeg(xyStiffR1, xyStiffR2, xyBeamL, xyBeamR+xyDir[nCol]*1000, xyTemp))	bInter = TRUE;
		xyStiffR1 = xyStiffR2 - vAngStiff*dOffset - vGir*dOffset;
		if(bInter)
			xyBase = (~(xyBase-xyCen) > ~(xyStiffR1-xyCen)) ? xyBase : xyStiffR1;

		if(xyBase != xyCen)
		{
			GetXyMatchLineAndLine(xyCen, xyDir[nCol], xyBase, xyDir[nCol].Rotate90(), xyReturn);
			xyReturn += xyDir[nCol]*dOffset;
		}
		xyBase = xyCen;
	}
	
	return xyReturn;
}
