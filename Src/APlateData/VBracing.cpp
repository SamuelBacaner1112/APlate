// VBracing.cpp: implementation of the CVBracing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVBracing::CVBracing(CSection *pSection) : m_pSection(pSection)
{
	InitData();

	m_pVStiff			= new CVStiff(pSection);
	m_pSteelMatUp		= new CSteelMaterialData();		// 상현재
	m_pSteelMatLo		= new CSteelMaterialData();		// 하현재
	m_pSteelMatDiagonal	= new CSteelMaterialData();		// 사재
	m_bSectionCheck		= FALSE;
}  

CVBracing::~CVBracing()
{
	delete m_pVStiff;
	delete m_pSteelMatUp;
	delete m_pSteelMatLo;
	delete m_pSteelMatDiagonal;
}

CPlateBasicIndex *CVBracing::GetBx()
{ 
	return (CPlateBasicIndex *)m_pSection->GetBx(); 
}

CVBracing& CVBracing::operator<<=(const CVBracing& Obj)
{	
	m_duL1 = Obj.m_duL1;
	m_duL2 = Obj.m_duL2;
	m_duL3 = Obj.m_duL3;
	m_duL4 = Obj.m_duL4;
	m_ddL1 = Obj.m_ddL1;
	m_ddL2 = Obj.m_ddL2;
	m_ddL3 = Obj.m_ddL3;
	m_ddL4 = Obj.m_ddL4;
	m_duR1 = Obj.m_duR1;
	m_duR2 = Obj.m_duR2;
	m_duR3 = Obj.m_duR3;
	m_duR4 = Obj.m_duR4;
	m_ddR1 = Obj.m_ddR1;
	m_ddR2 = Obj.m_ddR2;
	m_ddR3 = Obj.m_ddR3;
	m_ddR4 = Obj.m_ddR4;
	m_dc1 = Obj.m_dc1;
	m_dc2 = Obj.m_dc2;
	m_dc3 = Obj.m_dc3;
	m_dc4 = Obj.m_dc4;
	m_dLH1 = Obj.m_dLH1;
	m_dLH2 = Obj.m_dLH2;
	m_dRH1 = Obj.m_dRH1;
	m_dRH2 = Obj.m_dRH2;
	m_dgW = Obj.m_dgW;

	m_dULG = Obj.m_dULG ;
	m_dURG = Obj.m_dURG ;
	m_dDLG = Obj.m_dDLG ;
	m_dDRG = Obj.m_dDRG ;
	m_dC1LG= Obj.m_dC1LG;
	m_dC1RG= Obj.m_dC1RG;
	m_dC2LG= Obj.m_dC2LG;
	m_dC2RG= Obj.m_dC2RG;

	for(long n=0; n<8; n++)
		m_dG[n] = Obj.m_dG[n];

	m_dS  = Obj.m_dS;
	m_dT  = Obj.m_dT;
	m_duA = Obj.m_duA;
	m_uB = Obj.m_uB;
	m_uC = Obj.m_uC;
	m_duE = Obj.m_duE;
	m_ddA = Obj.m_ddA;
	m_dB = Obj.m_dB;
	m_dC = Obj.m_dC;
	m_ddE = Obj.m_ddE;
	m_dSL = Obj.m_dSL;

	m_nType = Obj.m_nType; 
	m_ZzsBoltSize = Obj.m_ZzsBoltSize;
	m_nAnchorSu = Obj.m_nAnchorSu;
	m_nLevelType = Obj.m_nLevelType;
	m_bEdgeCutting = Obj.m_bEdgeCutting;
	m_bCutLevel     = Obj.m_bCutLevel;
	m_bSameLeftRight = Obj.m_bSameLeftRight;

	return *this;
}
void CVBracing::InitData()
{
	m_duL1 = 90; 
	m_duL2 =250;
	m_duL3 =100;
	m_duL4 =200;
	m_ddL1 =150;
	m_ddL2 =90;	
	m_ddL3 =100;
	m_ddL4 =150;
	m_duR1 =90;	
	m_duR2 =250;
	m_duR3 =100;
	m_duR4 =200;
	m_ddR1 =150;
	m_ddR2 =90;	
	m_ddR3 =100;
	m_ddR4 =150;
	m_dc1  =130;
	m_dc2  =90;	
	m_dc3  =180;
	m_dc4  =180;
	m_dLH1 =200;
	m_dLH2 =200;
	m_dRH1 =200;
	m_dRH2 =200;
	m_dgW  =10;	
	m_dULG =200;
	m_dURG =200;
	m_dDLG =200;
	m_dDRG =200;
	m_dC1LG=200;
	m_dC1RG=200;
	m_dC2LG=200;
	m_dC2RG=200;
	long n = 0;
	for(n=0; n<8; n++)
		m_dG[n] = 0;
	for(n=0; n<10; n++)
		m_AnchorDis[n] = 0;

	m_dS  = 70;	
	m_dT  = 15;
	m_duA = 50;	
	m_uB  =  3;	
	m_uC  = 80;	
	m_duE = 50;	
	m_ddA = 50;	
	m_dB  =  2;	
	m_dC  = 60;	
	m_ddE = 50;
	m_dSL = 80;

	m_vW_G = 150;
	m_vW1_G= 0;
	m_vT_G = 12;
	m_nType        = TYPE_VBRACING; 
	m_ZzsBoltSize  = "M22";
	m_nAnchorSu    = 0;
	m_nLevelType   = 0;
	m_bEdgeCutting = FALSE;
	m_bSameLeftRight = FALSE;
	m_bCutLevel      = FALSE;
	m_bBoltInterver = FALSE;
	m_dBoltInterver = 80.0;
	m_bSectionCheck	= FALSE;
}

void CVBracing::Serialize(CArchive& ar)
{
	long nFlag = 2;	// TU-Girder

	CPlateBxObject::Serialize(ar);
	long n = 0;
	if( ar.IsStoring() )
	{
		ar << nFlag;	

		m_pVStiff->Serialize(ar);

		ar << m_duL1;
		ar << m_duL2;
		ar << m_duL3;
		ar << m_duL4;
		ar << m_ddL1;
		ar << m_ddL2;
		ar << m_ddL3;
		ar << m_ddL4;
		ar << m_duR1;
		ar << m_duR2;
		ar << m_duR3;
		ar << m_duR4;
		ar << m_ddR1;
		ar << m_ddR2;
		ar << m_ddR3;
		ar << m_ddR4;
		ar << m_dc1;
		ar << m_dc2;
		ar << m_dc3;
		ar << m_dc4;
		ar << m_dLH1;
		ar << m_dLH2;
		ar << m_dRH1;
		ar << m_dRH2;
		ar << m_dgW;

		ar << m_dULG ;
		ar << m_dURG ;
		ar << m_dDLG ;
		ar << m_dDRG ;
		ar << m_dC1LG;
		ar << m_dC1RG;
		ar << m_dC2LG;
		ar << m_dC2RG;

		ar << m_dS;

		ar << m_nType;
		ar << m_ZzsBoltSize;
		ar << m_nAnchorSu;
		ar << m_nLevelType; 
		ar << m_bEdgeCutting;
		ar << m_bCutLevel;
		ar << m_bSameLeftRight;
		//추가
		ar << m_duA;
		ar << m_uB;
		ar << m_uC;
		ar << m_duE;
		ar << m_ddA;
		ar << m_dB;
		ar << m_dC;
		ar << m_ddE;
		ar << m_dT;
		for(n=0; n<8; n++)
			ar << m_dG[n];
		for(n=0; n<10; n++)
			ar << m_AnchorDis[n];

		ar << m_dSL;
		ar << m_vW_G;
		ar << m_vT_G;
		ar << m_vW1_G;

		ar << m_dBoltInterver;
		ar << m_bBoltInterver;

		m_pSteelMatUp->Serialize(ar);
		m_pSteelMatLo->Serialize(ar);
		m_pSteelMatDiagonal->Serialize(ar);
		ar << m_bSectionCheck;
	}
	else 
	{
		ar >> nFlag;

		m_pVStiff->Serialize(ar);
		
		ar >> m_duL1;
		ar >> m_duL2;
		ar >> m_duL3;
		ar >> m_duL4;
		ar >> m_ddL1;
		ar >> m_ddL2;
		ar >> m_ddL3;
		ar >> m_ddL4;
		ar >> m_duR1;
		ar >> m_duR2;
		ar >> m_duR3;
		ar >> m_duR4;
		ar >> m_ddR1;
		ar >> m_ddR2;
		ar >> m_ddR3;
		ar >> m_ddR4;
		ar >> m_dc1;
		ar >> m_dc2;
		ar >> m_dc3;
		ar >> m_dc4;
		ar >> m_dLH1;
		ar >> m_dLH2;
		ar >> m_dRH1;
		ar >> m_dRH2;
		ar >> m_dgW;

		ar >> m_dULG ;
		ar >> m_dURG ;
		ar >> m_dDLG ;
		ar >> m_dDRG ;
		ar >> m_dC1LG;
		ar >> m_dC1RG;
		ar >> m_dC2LG;
		ar >> m_dC2RG;
		
		ar >> m_dS;

		ar >> m_nType;
		ar >> m_ZzsBoltSize;
		ar >> m_nAnchorSu;
		ar >> m_nLevelType; 
		ar >> m_bEdgeCutting;
		m_bEdgeCutting = FALSE;	// 081126 지원안함
		ar >> m_bCutLevel;
		ar >> m_bSameLeftRight;
		//추가
		ar >> m_duA;
		ar >> m_uB;
		ar >> m_uC;
		ar >> m_duE;
		ar >> m_ddA;
		ar >> m_dB;
		ar >> m_dC;
		ar >> m_ddE;
		ar >> m_dT;
		for(n=0; n<8; n++)
			ar >> m_dG[n];
		for(n=0; n<10; n++)
			ar >> m_AnchorDis[n];

		ar >> m_dSL;
		ar >> m_vW_G;
		ar >> m_vT_G;
		ar >> m_vW1_G;
		ar >> m_dBoltInterver;
		ar >> m_bBoltInterver;

		if(nFlag>0)
		{
			m_pSteelMatUp->Serialize(ar);
			m_pSteelMatLo->Serialize(ar);
			m_pSteelMatDiagonal->Serialize(ar);
		}
		if(nFlag>1)
			ar >> m_bSectionCheck;
	}
}

void CVBracing::CreatePieceObj(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nPos)
{
	switch(nPos)
	{
	case 0:
		CreatePieceObjLTSplice(pObj);
		CreatePieceBoltObjLTSplice(pObj, bMakeBoltHole, bMakeBoltLine);
		break;
	case 1:
		CreatePieceObjLDSplice(pObj);
		CreatePieceBoltObjLDSplice(pObj, bMakeBoltHole, bMakeBoltLine);
		break;
	case 2:
		CreatePieceObjRTSplice(pObj);
		CreatePieceBoltObjRTSplice(pObj, bMakeBoltHole, bMakeBoltLine);
		break;
	case 3:
		CreatePieceObjRDSplice(pObj);
		CreatePieceBoltObjRDSplice(pObj, bMakeBoltHole, bMakeBoltLine);
		break;
	case 4:
		CreatePieceObjCENSplice(pObj);
		CreatePieceBoltObjCENSplice(pObj, bMakeBoltHole, bMakeBoltLine);
		break;
	}

}

void CVBracing::CreatePieceObjLTSplice(CPlatePieceObject *pObj)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CDPoint vX(1,0), vY(0,1);

	double dThickWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	CDPoint	vDir = m_bCutLevel ? CDPoint(1,0) : (GetPointFrame(RT)-GetPointFrame(LT)).Unit();
	//골조선의 중심
	CDPoint xyOri	= GetPointFrame(LT);
	CDPoint xyCenUp = (GetPointFrame(LT) + GetPointFrame(RT))/2;
	CDPoint xyCenLo = (GetPointFrame(LD) + GetPointFrame(RD))/2;
	CDPoint xyCen   = GetCenterSplice(LT);
	CDPoint xyPiece1, xyPiece2, xyPiece3, xyPiece4, xy1, xy2, xy3, xy4, xyMatch;

//		1┌──┐2
//		 │    │
//		3└──┘4
	xyPiece1 = xyCen - vDir*m_duL3/vDir.x + vY*m_duL1;
	xyPiece2 = xyCen + vDir*m_duL4/vDir.x + vY*m_duL1;
	xyPiece3 = xyCen - vDir*m_duL3/vDir.x - vY*m_duL2;
	xyPiece4 = xyCen + vDir*m_duL4/vDir.x - vY*m_duL2;

	// xyOri 에서 상부빔의 시작점까지의 거리를 셋팅
	m_dG[0] = (m_duL3+m_duL4 - m_dULG)/vX.x - (GetCenterSplice(LT).x-xyOri.x);

	CDPoint vDirL	= GetVectorFrame(LEFT);
	double	dBeamH	= m_pSteelMatDiagonal->m_dH;
	if(m_nType==TYPE_VBRACING)
	{		
		pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);
		
		// xyOri 에서 사빔의 시작점까지의 거리를 셋팅
		GetXyMatchLineAndLine(xyPiece3, vDir, xyOri, vDirL, xyMatch);
		if(m_dC1LG<0)
		{
			CDPoint xyMatch1, xyMatch2, xyMatch3;
			GetXyMatchLineAndLine(xyOri+GetVectorFrame(UPPER)*m_dG[0],vY,xyOri-vDirL.Rotate90()*dBeamH/2,vDirL,xyMatch1);
			GetXyMatchLineAndLine(xyCen-vY*25,vX,xyOri+vDirL.Rotate90()*dBeamH/2,vDirL,xyMatch2);
			
			xyMatch3 = (~(xyOri-xyMatch1) > ~(xyOri-xyMatch2)) ? xyMatch1 : xyMatch2;
			GetXyMatchLineAndLine(xyMatch3, vDirL.Rotate90(), xyOri, vDirL, xyMatch1);
			m_dC1LG = ~(xyMatch1-xyMatch);
		}
		
		m_dG[2] = ~(xyOri-xyMatch)-m_dC1LG;
	}
	else
	{
		GetXyMatchLineAndLine(xyPiece3, vDir, xyCen, vY, xy1);
		xy2 = xy1 + vDir*m_dgW;
		GetXyMatchLineAndLine(xyPiece2, vY, xyCen, vDir, xy3);
		xy4 = xy3 - vY*m_dgW;

		pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);		
		pObj->AddElement(xy4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xy2, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);
	}	

	//======================================================================================================
	//수평보강재 간섭 시 구멍내기

	pObj->GetLastPointObj()->AddType(P_TYPE_DIMPOINT);
	CDPoint ptLeftUp = pObj->GetPoint(0);
	CDPoint ptLeftDn = pObj->GetPoint(pObj->GetObjSize()-1);
	
	CWebHStiff *pHs   = pBx->GetWebHStiff();
	if(!pHs)
	{
		pObj->RegenVertex();
		return;
	}
	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CDPoint xyUpCen  = (A[1]+A[2])/2;
	BOOL   bLeft = FALSE;
	long   nCol     = bLeft ? nCol=0 : nCol=1;
	double dWHStiff = pHs->m_dWidthHStiff[nCol]; 

	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		if(pHs->IsEmpty(nDan, bLeft))	continue;

		double	dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
		CDPoint ptHStiff = xyUpCen - vY*dHeightHStiff;

		if(ptLeftUp.y > ptHStiff.y && ptLeftDn.y < ptHStiff.y)
		{
			pObj->AddElement(CDPoint(ptLeftUp.x, ptHStiff.y-20), P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x + dThickWeb/2+dWHStiff+20,  ptHStiff.y-20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x + dThickWeb/2+dWHStiff+20,  ptHStiff.y+20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptLeftUp.x, ptHStiff.y+20), P_TYPE_LINE,-1,pBx);

			pObj->InsertScallopAtArc(pObj->GetObjSize()-3,20);
			pObj->InsertScallopAtArc(pObj->GetObjSize()-2,20);

			break;
		}
	}
	
	pObj->RegenVertex();
	//======================================================================================================
}

void CVBracing::CreatePieceObjLDSplice(CPlatePieceObject *pObj)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CDPoint vX(1,0), vY(0,1);
	
	//거더제원(좌측, 우측)
	double dThickWeb   = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	CDPoint	vDir = m_bCutLevel ? CDPoint(1,0) : (GetPointFrame(RD)-GetPointFrame(LD)).Unit();
	//골조선의 중심
	CDPoint xyOri	= GetPointFrame(LD);
	CDPoint xyCenUp = (GetPointFrame(LT) + GetPointFrame(RT))/2;
	CDPoint xyCenLo = (GetPointFrame(LD) + GetPointFrame(RD))/2;
	CDPoint xyCen   = GetCenterSplice(LD);
	CDPoint xyPiece1, xyPiece2, xyPiece3, xyPiece4, xy1, xy2, xy3, xy4, xyMatch;

//		1┌──┐2
//		 │    │
//		3└──┘4
	xyPiece1 = xyCen - vDir*m_ddL3/vDir.x + vY*m_ddL1;
	xyPiece2 = xyCen + vDir*m_ddL4/vDir.x + vY*m_ddL1;
	xyPiece3 = xyCen - vDir*m_ddL3/vDir.x - vY*m_ddL2;
	xyPiece4 = xyCen + vDir*m_ddL4/vDir.x - vY*m_ddL2;

	// xyOri 에서 하부빔의 시작점까지의 거리를 셋팅
	m_dG[6] = (m_ddL3+m_ddL4 - m_dDLG)/vX.x - (GetCenterSplice(LD).x-xyOri.x);

	CDPoint vDirL = GetVectorFrame(LEFT);
	double	dBeamH	= m_pSteelMatDiagonal->m_dH;
	if(m_nType==TYPE_INVVBRACING)
	{		
		pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);

		// xyOri 에서 사빔의 시작점까지의 거리를 셋팅
		GetXyMatchLineAndLine(xyPiece1, vDir, xyOri, vDirL, xyMatch);
		if(m_dC1LG<0)
		{
			CDPoint xyMatch1, xyMatch2, xyMatch3;
			GetXyMatchLineAndLine(xyOri+vDir*m_dG[6],vY,xyOri+vDirL.Rotate90()*dBeamH/2,vDirL,xyMatch1);
			GetXyMatchLineAndLine(xyCen+vY*25,vX,xyOri-vDirL.Rotate90()*dBeamH/2,vDirL,xyMatch2);
			
			xyMatch3 = (~(xyOri-xyMatch1) > ~(xyOri-xyMatch2)) ? xyMatch1 : xyMatch2;
			GetXyMatchLineAndLine(xyMatch3, vDirL.Rotate90(), xyOri, vDirL, xyMatch1);
			m_dC1LG = ~(xyMatch1-xyMatch);
		}
		m_dG[2] = ~(xyOri-xyMatch)-m_dC1LG;
	}
	else
	{
		GetXyMatchLineAndLine(xyPiece1, vDir, xyCen, vY, xy1);
		xy2 = xy1 + vDir*m_dgW;
		GetXyMatchLineAndLine(xyPiece2, vY, xyCen, vDir, xy3);
		xy4 = xy3 + vY*m_dgW;

		pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xy2, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xy4, P_TYPE_LINE,-1,pBx);		
		pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);
	}
	
	
	//======================================================================================================
	//수평보강재 간섭 시 구멍내기

	pObj->GetLastPointObj()->AddType(P_TYPE_DIMPOINT);
	CDPoint ptLeftUp = pObj->GetPoint(0);
	CDPoint ptLeftDn = pObj->GetPoint(pObj->GetObjSize()-1);	
	CWebHStiff *pHs   = pBx->GetWebHStiff();

	if(!pHs)
	{
		pObj->RegenVertex();
		return;
	}

	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CDPoint xyUpCen  = (A[1]+A[2])/2;
	BOOL   bLeft = FALSE;
	long   nCol     = bLeft ? nCol=0 : nCol=1;
	double dWHStiff = pHs->m_dWidthHStiff[nCol]; 

	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		if(pHs->IsEmpty(nDan, bLeft))	continue;

		double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
		CDPoint ptHStiff = xyUpCen - vY*dHeightHStiff;		
		if(ptLeftUp.y > ptHStiff.y && ptLeftDn.y < ptHStiff.y)
		{
			pObj->AddElement(CDPoint(ptLeftUp.x, ptHStiff.y-20), P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x + dThickWeb/2+dWHStiff+20,  ptHStiff.y-20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x + dThickWeb/2+dWHStiff+20,  ptHStiff.y+20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptLeftUp.x, ptHStiff.y+20), P_TYPE_LINE,-1,pBx);
			pObj->InsertScallopAtArc(pObj->GetObjSize()-3,20);
			pObj->InsertScallopAtArc(pObj->GetObjSize()-2,20);
			break;
		}
	}		
	
	pObj->RegenVertex();
	//======================================================================================================
}
void CVBracing::CreatePieceObjRTSplice(CPlatePieceObject *pObj)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CDPoint vX(1,0), vY(0,1);
	
	//거더제원(좌측, 우측)
	double dThickWeb     = pGirN->GetThickFactByStation(pBxN->GetStation(), G_W);
	CDPoint	vDir = m_bCutLevel ? CDPoint(-1,0) : (GetPointFrame(LT)-GetPointFrame(RT)).Unit();
	//골조선의 중심
	CDPoint xyOri = GetPointFrame(RT);
	CDPoint xyCenUp = (GetPointFrame(LT) + GetPointFrame(RT))/2;
	CDPoint xyCenLo = (GetPointFrame(LD) + GetPointFrame(RD))/2;
	CDPoint xyCen   = GetCenterSplice(RT);
	CDPoint xyPiece1, xyPiece2, xyPiece3, xyPiece4, xy1, xy2, xy3, xy4, xyMatch;

//		1┌──┐2
//		 │    │
//		3└──┘4
	xyPiece1 = xyCen - vDir*m_duR4/vDir.x  + vY*m_duR1;
	xyPiece2 = xyCen + vDir*m_duR3/vDir.x  + vY*m_duR1;
	xyPiece3 = xyCen - vDir*m_duR4/vDir.x  - vY*m_duR2;
	xyPiece4 = xyCen + vDir*m_duR3/vDir.x  - vY*m_duR2;
	
	// xyOri 에서 상부빔의 단부까지의 거리를 셋팅
	m_dG[1] = (m_duR3 + m_duR4 - m_dURG)/ABS(vX.x) - (xyOri.x-GetCenterSplice(RT).x);

	CDPoint vDirR = GetVectorFrame(RIGHT);
	double	dBeamH	= m_pSteelMatDiagonal->m_dH;
	if(m_nType==TYPE_VBRACING)
	{	
		pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);		
		pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);
		// xyOri 에서 사빔의 단부까지의 거리를 셋팅
		GetXyMatchLineAndLine(xyPiece3, vDir, xyOri, vDirR, xyMatch);
		if(m_dC2RG<0)
		{
			CDPoint xyMatch1, xyMatch2, xyMatch3;
			GetXyMatchLineAndLine(xyOri-GetVectorFrame(UPPER)*m_dG[1],vY,xyOri+vDirR.Rotate90()*dBeamH/2,vDirR,xyMatch1);
			GetXyMatchLineAndLine(xyCen-vY*25,vX,xyOri-vDirR.Rotate90()*dBeamH/2,vDirR,xyMatch2);
			
			xyMatch3 = (~(xyOri-xyMatch1) > ~(xyOri-xyMatch2)) ? xyMatch1 : xyMatch2;
			GetXyMatchLineAndLine(xyMatch3, vDirR.Rotate90(), xyOri, vDirR, xyMatch1);
			m_dC2RG = ~(xyMatch1-xyMatch);

			if(m_bSameLeftRight)	m_dC2RG = m_dC1LG;
		}
		m_dG[4] = ~(xyOri-xyMatch)-m_dC2RG;		
	}
	else
	{
		GetXyMatchLineAndLine(xyPiece4, vDir, xyCen, vY, xy1);
		xy2 = xy1 + vDir*m_dgW;
		GetXyMatchLineAndLine(xyPiece1, vY, xyCen, vDir, xy3);
		xy4 = xy3 - vY*m_dgW;

		pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xy2, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xy4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);
	}	

	//======================================================================================================
	//수평보강재 간섭 시 구멍내기

	pObj->GetLastPointObj()->AddType(P_TYPE_DIMPOINT);
	CDPoint ptRightUp = pObj->GetPoint(pObj->GetObjSize()-1);
	CDPoint ptRightDn = pObj->GetPoint(0);

	CWebHStiff *pHs   = pBxN->GetWebHStiff();
	if(!pHs)
	{
		pObj->RegenVertex();
		return;
	}

	CDPoint A[10];
	pGirN->GetHunchXyDom(pBxN,A);
	CDPoint xyUpCen  = (A[1]+A[2])/2;
	BOOL   bLeft = TRUE;
	long   nCol     = bLeft ? nCol=0 : nCol=1;
	double dWHStiff = pHs->m_dWidthHStiff[nCol]; 

	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		if(pHs->IsEmpty(nDan, bLeft))	continue;

		double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
		CDPoint ptHStiff = xyUpCen - vY*dHeightHStiff;		
		if(ptRightUp.y > ptHStiff.y && ptRightDn.y < ptHStiff.y)
		{
			pObj->AddElement(CDPoint(ptRightUp.x, ptHStiff.y+20), P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x - dThickWeb/2-dWHStiff-20,  ptHStiff.y+20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x - dThickWeb/2-dWHStiff-20,  ptHStiff.y-20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptRightUp.x, ptHStiff.y-20), P_TYPE_LINE,-1,pBx);

			pObj->InsertScallopAtArc(pObj->GetObjSize()-3,20);
			pObj->InsertScallopAtArc(pObj->GetObjSize()-2,20);
			break;
		}
	}

	pObj->RegenVertex();
	//======================================================================================================
}


void CVBracing::CreatePieceObjRDSplice(CPlatePieceObject *pObj)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CDPoint vX(1,0), vY(0,1);	

	//거더제원(좌측, 우측)
	double dThickWeb     = pGirN->GetThickFactByStation(pBxN->GetStation(), G_W);
	CDPoint	vDir = m_bCutLevel ? CDPoint(-1,0) : (GetPointFrame(LD)-GetPointFrame(RD)).Unit();
	//골조선의 중심
	CDPoint xyOri	= GetPointFrame(RD);
	CDPoint xyCenUp = (GetPointFrame(LT) + GetPointFrame(RT))/2;
	CDPoint xyCenLo = (GetPointFrame(LD) + GetPointFrame(RD))/2;
	CDPoint xyCen   = GetCenterSplice(RD);
	CDPoint xyPiece1, xyPiece2, xyPiece3, xyPiece4, xy1, xy2, xy3, xy4, xyMatch;

//		1┌──┐2
//		 │    │
//		3└──┘4
	xyPiece1 = xyCen - vDir*m_ddR4/vDir.x + vY*m_ddR1;
	xyPiece2 = xyCen + vDir*m_ddR3/vDir.x + vY*m_ddR1;
	xyPiece3 = xyCen - vDir*m_ddR4/vDir.x - vY*m_ddR2;
	xyPiece4 = xyCen + vDir*m_ddR3/vDir.x - vY*m_ddR2;

	// xyOri 에서 하부빔의 시작점까지의 거리를 셋팅
	m_dG[7] = (m_ddR3+m_ddR4-m_dDLG)/vX.x - (xyOri.x-GetCenterSplice(RD).x);

	CDPoint vDirR = GetVectorFrame(RIGHT);
	double	dBeamH	= m_pSteelMatDiagonal->m_dH;
	if(m_nType==TYPE_INVVBRACING)
	{	
		pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);
		// xyOri 에서 사빔의 시작점까지의 거리를 셋팅
		GetXyMatchLineAndLine(xyPiece2, vDir, xyOri, vDirR, xyMatch);
		if(m_dC2RG<0)
		{
			CDPoint xyMatch1, xyMatch2, xyMatch3;
			GetXyMatchLineAndLine(xyOri-GetVectorFrame(LOWER)*m_dG[7],vY,xyOri-vDirR.Rotate90()*dBeamH/2,vDirR,xyMatch1);
			GetXyMatchLineAndLine(xyCen+vY*25,vX,xyOri+vDirR.Rotate90()*dBeamH/2,vDirR,xyMatch2);
			
			xyMatch3 = (~(xyOri-xyMatch1) > ~(xyOri-xyMatch2)) ? xyMatch1 : xyMatch2;
			GetXyMatchLineAndLine(xyMatch3, vDirR.Rotate90(), xyOri, vDirR, xyMatch1);
			m_dC2RG = ~(xyMatch1-xyMatch);

			if(m_bSameLeftRight)	m_dC2RG = m_dC1LG;
		}
		m_dG[4] = ~(xyOri-xyMatch)-m_dC2RG;
	}
	else
	{
		GetXyMatchLineAndLine(xyPiece2, vDir, xyCen, vY, xy1);
		xy2 = xy1 + vDir*m_dgW;
		GetXyMatchLineAndLine(xyPiece3, vY, xyCen, vDir, xy3);
		xy4 = xy3 + vY*m_dgW;
		
		pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xy4, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xy2, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);
	}

	//======================================================================================================
	//수평보강재 간섭 시 구멍내기

	pObj->GetLastPointObj()->AddType(P_TYPE_DIMPOINT);
	CDPoint ptRightUp = pObj->GetPoint(pObj->GetObjSize()-1);	
	CDPoint ptRightDn = pObj->GetPoint(0);
	CWebHStiff *pHs   = pBxN->GetWebHStiff();
	if(!pHs)
	{
		pObj->RegenVertex();
		return;
	}

	CDPoint A[10];
	pGirN->GetHunchXyDom(pBxN,A);
	CDPoint xyUpCen  = (A[1]+A[2])/2;
	BOOL   bLeft = TRUE;
	long   nCol     = bLeft ? nCol=0 : nCol=1;
	double dWHStiff = pHs->m_dWidthHStiff[nCol]; 

	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		if(pHs->IsEmpty(nDan, bLeft))	continue;

		double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
		CDPoint ptHStiff = xyUpCen  - vY*dHeightHStiff;		
		if(ptRightUp.y > ptHStiff.y && ptRightDn.y < ptHStiff.y)
		{
			pObj->AddElement(CDPoint(ptRightUp.x, ptHStiff.y+20), P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x - dThickWeb/2-dWHStiff-20,  ptHStiff.y+20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptHStiff.x - dThickWeb/2-dWHStiff-20,  ptHStiff.y-20) , P_TYPE_LINE,-1,pBx);
			pObj->AddElement(CDPoint(ptRightUp.x, ptHStiff.y-20), P_TYPE_LINE,-1,pBx);
			
			pObj->InsertScallopAtArc(pObj->GetObjSize()-3,20);
			pObj->InsertScallopAtArc(pObj->GetObjSize()-2,20);
			break;
		}
	}

	pObj->RegenVertex();
	//======================================================================================================
}


void CVBracing::CreatePieceObjCENSplice(CPlatePieceObject *pObj)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();

	CDPoint	vDirU = m_bCutLevel ? CDPoint(1,0) : (GetPointFrame(RT)-GetPointFrame(LT)).Unit();
	CDPoint	vDirL = m_bCutLevel ? CDPoint(1,0) : (GetPointFrame(RD)-GetPointFrame(LD)).Unit();
	//골조선의 중심
	CDPoint xyCenUp = (GetPointFrame(LT) + GetPointFrame(RT))/2;
	CDPoint xyCenLo = (GetPointFrame(LD) + GetPointFrame(RD))/2;
	CDPoint xyOri	= m_nType==TYPE_VBRACING ? xyCenLo : xyCenUp;
	CDPoint xyCen   = GetCenterSplice(CEN);
	CDPoint vCuttingDir1 = m_nType==0 ? (GetPointFrame(LT)-xyOri).Unit() : (GetPointFrame(LD)-xyOri).Unit(); 
	CDPoint vCuttingDir2 = m_nType==0 ? (GetPointFrame(RT)-xyOri).Unit() : (GetPointFrame(RD)-xyOri).Unit();
	CDPoint vX(1,0), vY(0,1), xyPiece1, xyPiece2, xyPiece3, xyPiece4, xyMatch;
	vX = m_nType==TYPE_VBRACING ? vDirL : vDirU;

	xyPiece1 = xyCen - vX*m_dc3/vX.x + vY*m_dc1;
	xyPiece2 = xyCen + vX*m_dc4/vX.x + vY*m_dc1;
	xyPiece3 = xyCen - vX*m_dc3/vX.x - vY*m_dc2;
	xyPiece4 = xyCen + vX*m_dc4/vX.x - vY*m_dc2;
	pObj->AddElement(xyPiece1, P_TYPE_LINE,-1,pBx);
	pObj->AddElement(xyPiece2, P_TYPE_LINE,-1,pBx);
	pObj->AddElement(xyPiece4, P_TYPE_LINE,-1,pBx);
	pObj->AddElement(xyPiece3, P_TYPE_LINE,-1,pBx);
	
	vDirL = GetVectorFrame(LEFT);
	CDPoint vDirR = GetVectorFrame(RIGHT);
	double	dBeamH	= m_pSteelMatDiagonal->m_dH;
	if(m_nType==TYPE_VBRACING)
	{
		CDPoint xyMatchL, xyMatchR, xyMatch1, xyMatch2;
		GetXyMatchLineAndLine(xyPiece1, vX, xyOri, vDirL, xyMatchL);
		if(m_dC1RG<0)
		{
			GetXyMatchLineAndLine(xyCen+vY*25,GetVectorFrame(LOWER),xyOri-vDirL.Rotate90()*dBeamH/2,vDirL,xyMatch1);
			GetXyMatchLineAndLine(xyMatch1, vDirL.Rotate90(), xyOri, vDirL, xyMatch2);
			m_dC1RG = ~(xyMatch2-xyMatchL);
		}		

		GetXyMatchLineAndLine(xyPiece1, vX, xyOri, vDirR, xyMatchR);
		if(m_dC2LG<0)
		{
			GetXyMatchLineAndLine(xyCen+vY*25,GetVectorFrame(LOWER),xyOri+vDirR.Rotate90()*dBeamH/2,vDirR,xyMatch1);
			GetXyMatchLineAndLine(xyMatch1, vDirR.Rotate90(), xyOri, vDirR, xyMatch2);
			m_dC2LG = ~(xyMatch2-xyMatchR);
			
		}
		if(m_bSameLeftRight)
		{
			m_dC1RG = min(m_dC1RG, m_dC2LG);
			m_dC2LG = min(m_dC1RG, m_dC2LG);
		}
		
		m_dG[3] = ~(xyOri-xyMatchL)-m_dC1RG;
		m_dG[5] = ~(xyOri-xyMatchR)-m_dC2LG;
	}
	else
	{
		CDPoint xyMatchL, xyMatchR, xyMatch1, xyMatch2;
		GetXyMatchLineAndLine(xyPiece3, vX, xyOri, vDirL, xyMatchL);
		if(m_dC1RG<0)
		{
			GetXyMatchLineAndLine(xyCen-vY*25,GetVectorFrame(UPPER),xyOri+vDirL.Rotate90()*dBeamH/2,vDirL,xyMatch1);
			GetXyMatchLineAndLine(xyMatch1, vDirL.Rotate90(), xyOri, vDirL, xyMatch2);
			m_dC1RG = ~(xyMatch2-xyMatchL);			
		}
		
		GetXyMatchLineAndLine(xyPiece3, vX, xyOri, vDirR, xyMatchR);
		if(m_dC2LG<0)
		{
			GetXyMatchLineAndLine(xyCen-vY*25,GetVectorFrame(UPPER),xyOri-vDirR.Rotate90()*dBeamH/2,vDirR,xyMatch1);
			GetXyMatchLineAndLine(xyMatch1, vDirR.Rotate90(), xyOri, vDirR, xyMatch2);
			m_dC2LG = ~(xyMatch2-xyMatchR);
		}

		if(m_bSameLeftRight)
		{
			m_dC1RG = min(m_dC1RG, m_dC2LG);
			m_dC2LG = min(m_dC1RG, m_dC2LG);
		}
		
		m_dG[3] = ~(xyOri-xyMatchL)-m_dC1RG;
		m_dG[5] = ~(xyOri-xyMatchR)-m_dC2LG;
	}	

	pObj->RegenVertex();
}
void CVBracing::CreatePieceBoltObjLTSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	CPlateBasicIndex* pBx  = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen = (CPlateGirderApp*)pBx->GetGirder();

	CDPoint B[10];
	pGir->GetHunchXyDom(pBx, B);

	//======================================================================================================
	//수평보강재 간섭 시
	BOOL bHStiffExist = FALSE;
	CDPoint ptHStiffPos;
	if(pGir->GetNumberGirder() < pGir->GetBridgeApp()->GetGirdersu()/2)	
	{	
		CDPoint vX(1,0), vY(0,1);
		CDPoint ptLeftUp = pObj->GetPoint(0);
		long nSearch = m_nType==0 ? 1 : pObj->GetObjSize()-1;
		CDPoint ptLeftDn = pObj->GetPoint(nSearch);	
		while(nSearch!=0 && !pObj->IsPTType(nSearch, P_TYPE_DIMPOINT))
		{				
			m_nType==0 ? ++nSearch : --nSearch;
			CDPoint ptPos = pObj->GetPoint(nSearch);	
			if(ptPos.y < ptLeftDn.y) ptLeftDn = ptPos;
		}
		CWebHStiff *pHs   = pBx->GetWebHStiff();	
		if(pHs)
		{
			CDPoint xyUpCen  = (B[1]+B[2])/2;
			BOOL   bLeft = FALSE;

			for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
			{
				if(pHs->IsEmpty(nDan, bLeft))	continue;

				double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
				CDPoint ptHStiff = xyUpCen /*+ vX*(dThickWeb/2+dWHStiff)*/ - vY*dHeightHStiff;		
				if(ptLeftUp.y > ptHStiff.y && ptLeftDn.y < ptHStiff.y)
				{
					ptHStiffPos = ptHStiff;
					bHStiffExist = TRUE;
					break;
				}
			}
		}
	}
	//======================================================================================================

	CDPoint vFrame = GetVectorFrame(UPPER);
	CDPoint vX(1,0), vY(0,1);
	//볼트홀
	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir);
	CPlatePieceObject *pLine = new CPlatePieceObject(pGir);
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);

	double dBoltR = pDB->GetBoltRad(m_ZzsBoltSize);
	
	CDPoint xyStt, xyEnd, xyMid;
	CDPoint xy1 = GetCenterSplice(LT)-vX*(m_duL3-m_duE);
	CDPoint xy2 = xy1+vY*m_duL1;
	GetXyMatchLineAndLine(xy1, vY, xy2, vFrame, xyStt);
	xyEnd = xyStt - vY*(m_duL1+m_duL2);
	xyMid = (xyStt+xyEnd)/2;

	CDPoint ptHolePos;
	long n = 0;
	if(m_uB%2==0)
	{
		long nHalf = m_uB/2;
		for(n=0; n<=nHalf; n++)
		{			
			ptHolePos = xyMid+vY*m_uC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)			
				pBolt->AddCircle(ptHolePos, dBoltR, GetBx());
			pLine->AddElement(ptHolePos);			
		}
		pLine->SetEnd();
		pLine->AddElement(xyMid);
		for(n=1; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*m_uC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)			
				pBolt->AddCircle(ptHolePos, dBoltR, GetBx());
			pLine->AddElement(ptHolePos);			
			if(n==nHalf)
				xyBoltPos[0] = xyMid-vY*m_uC*n;
		}
		pLine->SetEnd();
	}
	else
	{
		long nHalf = (m_uB-1)/2;
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid+vY*(m_uC/2+m_uC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)			
				pBolt->AddCircle(ptHolePos, dBoltR, GetBx());
			pLine->AddElement(ptHolePos);			
		}
		pLine->SetEnd();
		
		pLine->AddElement(xyMid+vY*m_uC/2);
		pLine->AddElement(xyMid-vY*m_uC/2);
		pLine->SetEnd();
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*(m_uC/2+m_uC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR, GetBx());			
			pLine->AddElement(ptHolePos);
			
			if(n==nHalf)
				xyBoltPos[0] = xyMid-vY*(m_uC/2+m_uC*n);
		}
		pLine->SetEnd();

	}
	
	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}
void CVBracing::CreatePieceBoltObjLDSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	CPlateBasicIndex* pBx  = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen = (CPlateGirderApp*)pBx->GetGirder();

	CDPoint B[10];
	pGir->GetHunchXyDom(pBx, B);

	//======================================================================================================
	//수평보강재 간섭 시
	BOOL bHStiffExist = FALSE;
	CDPoint ptHStiffPos;
	if(pGir->GetNumberGirder() < pGir->GetBridgeApp()->GetGirdersu()/2)	
	{	
		CDPoint vX(1,0), vY(0,1);
		CDPoint ptLeftUp = pObj->GetPoint(0);
		long nSearch=m_nType==0 ? 1 : pObj->GetObjSize()-1;
		CDPoint ptLeftDn = pObj->GetPoint(nSearch);	
		while(nSearch!=0 && !pObj->IsPTType(nSearch, P_TYPE_DIMPOINT))
		{				
			m_nType==0 ? ++nSearch : --nSearch;
			CDPoint ptPos = pObj->GetPoint(nSearch);	
			if(ptPos.y < ptLeftDn.y) ptLeftDn = ptPos;
		}
		CWebHStiff *pHs   = pBx->GetWebHStiff();	
		if(pHs)
		{
			CDPoint xyUpCen  = (B[1]+B[2])/2;
			BOOL   bLeft = FALSE;

			for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
			{
				if(pHs->IsEmpty(nDan, bLeft))	continue;

				double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
				CDPoint ptHStiff = xyUpCen /*+ vX*(dThickWeb/2+dWHStiff)*/ - vY*dHeightHStiff;		
				if(ptLeftUp.y > ptHStiff.y && ptLeftDn.y < ptHStiff.y)
				{
					ptHStiffPos = ptHStiff;
					bHStiffExist = TRUE;
					break;
				}
			}
		}
	}
	//======================================================================================================

	CDPoint vFrame = GetVectorFrame(LOWER);
	CDPoint vX(1,0), vY(0,1);
	//볼트홀
	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir);
	CPlatePieceObject *pLine = new CPlatePieceObject(pGir);
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);

	double dBoltR = pDB->GetBoltRad(m_ZzsBoltSize);
	
	CDPoint xyStt, xyEnd, xyMid;
	CDPoint xy1 = GetCenterSplice(LD)-vX*(m_duL3-m_ddE);
	CDPoint xy2 = xy1+vY*m_ddL1;
	GetXyMatchLineAndLine(xy1, vY, xy2, vFrame, xyStt);
	xyEnd = xyStt - vY*(m_ddL1+m_ddL2);
	xyMid = (xyStt+xyEnd)/2;

	CDPoint ptHolePos;
	long n = 0;
	if(m_dB%2==0)
	{
		long nHalf = m_dB/2;
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid+vY*m_dC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)	
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
		}
		pLine->SetEnd();
		pLine->AddElement(xyMid);
		for(n=1; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*m_dC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)	
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
			if(n==nHalf)
				xyBoltPos[1] = xyMid-vY*m_dC*n;
		}
		pLine->SetEnd();
	}
	else
	{
		long nHalf = (m_dB-1)/2;
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid+vY*(m_dC/2+m_dC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)	
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
		}
		pLine->SetEnd();
		
		pLine->AddElement(xyMid+vY*m_dC/2);
		pLine->AddElement(xyMid-vY*m_dC/2);
		pLine->SetEnd();
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*(m_dC/2+m_dC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
			if(n==nHalf)
				xyBoltPos[1] = xyMid-vY*(m_dC/2+m_dC*n);
		}
		pLine->SetEnd();

	}
	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}
void CVBracing::CreatePieceBoltObjRTSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	CPlateBasicIndex* pBx  = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);

	CDPoint B[10];
	pGirN->GetHunchXyDom(pBxN, B);

	//======================================================================================================
	//수평보강재 간섭 시
	BOOL bHStiffExist = FALSE;
	CDPoint ptHStiffPos;
	if(pGir->GetNumberGirder() < pGir->GetBridgeApp()->GetGirdersu()/2-1)		
	{
		CDPoint vX(1,0), vY(0,1);
		CDPoint ptLeftUp = pObj->GetPoint(0);
		long nSearch=m_nType==0 ? 1 : pObj->GetObjSize()-1;
		CDPoint ptLeftDn = pObj->GetPoint(nSearch);	
		while(nSearch!=0 && !pObj->IsPTType(nSearch, P_TYPE_DIMPOINT))
		{				
			m_nType==0 ? ++nSearch : --nSearch;
			CDPoint ptPos = pObj->GetPoint(nSearch);	
			if(ptPos.y < ptLeftDn.y) ptLeftDn = ptPos;
		}		
		CWebHStiff *pHs   = pBx->GetWebHStiff();	
		if(pHs)
		{
			CDPoint xyUpCen  = (B[1]+B[2])/2;
			BOOL   bLeft = TRUE;

			for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
			{
				if(pHs->IsEmpty(nDan, bLeft))	continue;

				double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
				CDPoint ptHStiff = xyUpCen /*+ vX*(dThickWeb/2+dWHStiff)*/ - vY*dHeightHStiff;		
				if(ptLeftUp.y > ptHStiff.y && ptLeftDn.y < ptHStiff.y)
				{
					ptHStiffPos = ptHStiff;
					bHStiffExist = TRUE;
					break;
				}
			}
		}
	}

	CDPoint vFrame = GetVectorFrame(UPPER);
	CDPoint vX(1,0), vY(0,1);
	//볼트홀
	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir);
	CPlatePieceObject *pLine = new CPlatePieceObject(pGir);
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);
	
	double dBoltR = pDB->GetBoltRad(m_ZzsBoltSize);
	
	CDPoint xyStt, xyEnd, xyMid;
	CDPoint xy1 = GetCenterSplice(RT)+vX*(m_duR3-m_duE);
	CDPoint xy2 = xy1+vY*m_duR1;
	GetXyMatchLineAndLine(xy1, vY, xy2, vFrame, xyStt);
	xyEnd = xyStt - vY*(m_duR1+m_duR2);
	xyMid = (xyStt+xyEnd)/2;

	CDPoint ptHolePos;
	long n = 0;
	if(long(m_uB)%2==0)
	{
		long nHalf = m_uB/2;
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid+vY*m_uC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
		}
		pLine->SetEnd();
		pLine->AddElement(xyMid);
		for(n=1; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*m_uC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
			if(n==nHalf)
				xyBoltPos[2] = xyMid-vY*m_uC*n;
		}
		pLine->SetEnd();
	}
	else
	{
		long nHalf = (m_uB-1)/2;
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid+vY*(m_uC/2+m_uC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
		}
		pLine->SetEnd();
		
		pLine->AddElement(xyMid+vY*m_uC/2);
		pLine->AddElement(xyMid-vY*m_uC/2);
		pLine->SetEnd();
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*(m_uC/2+m_uC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
			if(n==nHalf)
				xyBoltPos[2] = xyMid-vY*(m_uC/2+m_uC*n);
		}
		pLine->SetEnd();

	}

	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}
void CVBracing::CreatePieceBoltObjRDSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	CPlateBasicIndex* pBx  = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp  *pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);

	CDPoint B[10];
	pGirN->GetHunchXyDom(pBxN, B);

	//======================================================================================================
	//수평보강재 간섭 시
	BOOL bHStiffExist = FALSE;
	CDPoint ptHStiffPos;
	if(pGir->GetNumberGirder() >= pGir->GetBridgeApp()->GetGirdersu()/2-1)		
	{
		CDPoint vX(1,0), vY(0,1);
		CDPoint ptLeftUp = pObj->GetPoint(0);
		long nSearch=m_nType==0 ? 1 : pObj->GetObjSize()-1;
		CDPoint ptLeftDn = pObj->GetPoint(nSearch);	
		while(nSearch!=0 && !pObj->IsPTType(nSearch, P_TYPE_DIMPOINT))
		{				
			m_nType==0 ? ++nSearch : --nSearch;
			CDPoint ptPos = pObj->GetPoint(nSearch);	
			if(ptPos.y < ptLeftDn.y) ptLeftDn = ptPos;
		}
		CWebHStiff *pHs   = pBx->GetWebHStiff();	
		if(pHs)
		{
			CDPoint xyUpCen  = (B[1]+B[2])/2;
			BOOL   bLeft = TRUE;

			for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
			{
				if(pHs->IsEmpty(nDan, bLeft))	continue;

				double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, pBx->GetStation());
				CDPoint ptHStiff = xyUpCen /*+ vX*(dThickWeb/2+dWHStiff)*/ - vY*dHeightHStiff;		
				if(ptLeftUp.y > ptHStiff.y && ptLeftDn.y < ptHStiff.y)
				{
					ptHStiffPos = ptHStiff;
					bHStiffExist = TRUE;
					break;
				}
			}
		}
	}

	CDPoint vFrame = GetVectorFrame(LOWER);
	CDPoint vX(1,0), vY(0,1);
	//볼트홀
	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir);
	CPlatePieceObject *pLine = new CPlatePieceObject(pGir);
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);
	
	double dBoltR = pDB->GetBoltRad(m_ZzsBoltSize);
	
	CDPoint xyStt, xyEnd, xyMid;
	CDPoint xy1 = GetCenterSplice(RD)+vX*(m_ddR3-m_ddE);
	CDPoint xy2 = xy1+vY*m_ddR1;
	GetXyMatchLineAndLine(xy1, vY, xy2, vFrame, xyStt);
	xyEnd = xyStt - vY*(m_ddR1+m_ddR2);
	xyMid = (xyStt+xyEnd)/2;

	CDPoint ptHolePos;
	long n = 0;
	if(long(m_dB)%2==0)
	{
		long nHalf = m_dB/2;
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid+vY*m_dC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)				
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
		}
		pLine->SetEnd();
		pLine->AddElement(xyMid);
		for(n=1; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*m_dC*n;
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
			if(n==nHalf)
				xyBoltPos[3] = xyMid-vY*m_dC*n;
		}
		pLine->SetEnd();
	}
	else
	{
		long nHalf = (m_dB-1)/2;
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid+vY*(m_dC/2+m_dC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
		}
		pLine->SetEnd();
		
		pLine->AddElement(xyMid+vY*m_dC/2);
		pLine->AddElement(xyMid-vY*m_dC/2);
		pLine->SetEnd();
		for(n=0; n<=nHalf; n++)
		{
			ptHolePos = xyMid-vY*(m_dC/2+m_dC*n);
			if(ptHolePos.y > ptHStiffPos.y-60 && ptHolePos.y < ptHStiffPos.y-20)
				ptHolePos.y = ptHStiffPos.y-60;
			else if(ptHolePos.y < ptHStiffPos.y+60 && ptHolePos.y > ptHStiffPos.y+20)
				ptHolePos.y = ptHStiffPos.y+60;
			if(!bHStiffExist || ptHolePos.y < ptHStiffPos.y-20 || ptHolePos.y > ptHStiffPos.y+20)
				pBolt->AddCircle(ptHolePos, dBoltR);
			pLine->AddElement(ptHolePos);
			if(n==nHalf)
				xyBoltPos[3] = xyMid-vY*(m_dC/2+m_dC*n);
		}
		pLine->SetEnd();

	}
	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}
void CVBracing::CreatePieceBoltObjCENSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{

}

double CVBracing::GetGiulFrameLine(BOOL bUpper)
{
	CPlateBasicIndex* pBx = GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGirN);
	double dWidthGirN = pGirN->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	double dHeightGirN = pGirN->GetHeightGirderByStaAng(pBxN->GetStation());	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyCen1 = (A[1]+A[2])/2 + vX*m_dS - vY*m_dLH1;
	CDPoint xyCen2 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*m_dRH1;
	CDPoint xyCen3 = (A[1]+A[2])/2 + vX*m_dS       - vY*(dHeightGir-m_dLH2);
	CDPoint xyCen4 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*(dHeightGirN-m_dRH2);

	if(bUpper)
	{
		CDPoint xy = (xyCen2 - xyCen1).Unit();
		return xy.y/xy.x;
	}
	else
	{
		CDPoint xy = (xyCen4 - xyCen3).Unit();
		return xy.y/xy.x;
	}
}
double CVBracing::GetLengthFrameLine(long nPos)
{
	CPlateBasicIndex* pBx = GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGirN);
	double dWidthGirN = pGirN->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	double dHeightGirN = pGirN->GetHeightGirderByStaAng(pBxN->GetStation());	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyCen1 = (A[1]+A[2])/2 + vX*m_dS - vY*m_dLH1;
	CDPoint xyCen2 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*m_dRH1;
	CDPoint xyCen3 = (A[1]+A[2])/2 + vX*m_dS       - vY*(dHeightGir-m_dLH2);
	CDPoint xyCen4 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*(dHeightGirN-m_dRH2);
	CDPoint xyCen5 = (xyCen3+xyCen4)/2;
	if(m_nType==1)
		xyCen5 = (xyCen1+xyCen2)/2;
	switch(nPos)
	{
	case UPPER:
		return ~(xyCen2-xyCen1);
	case LOWER:
		return ~(xyCen4-xyCen3);
	case LEFT:
		{
			if(m_nType==0)
				return ~(xyCen1-xyCen5);
			if(m_nType==1)
				return ~(xyCen2-xyCen5);
		}
	case RIGHT:
		{
			if(m_nType==0)
				return ~(xyCen3-xyCen5);
			if(m_nType==1)
				return ~(xyCen4-xyCen5);
		}
	case 4:
		return ~(xyCen1-xyCen3);
	case 5:
		return ~(xyCen2-xyCen4);
	}
	return 0;
}
double CVBracing::GetLengthBeam(long nPos)
{
	CPlateBasicIndex* pBx = GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGirN);
	double dWidthGirN = pGirN->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	double dHeightGirN = pGirN->GetHeightGirderByStaAng(pBxN->GetStation());	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);

	CDPoint vGir  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vGirN = pGirN->GetLine()->GetAngleAzimuth(pBxN->GetStation());
	CVStiff	*pVStiff = pBx->GetSection() ? pBx->GetSection()->GetVStiff() : NULL;
	CVStiff	*pVStiffN= pBxN->GetSection() ? pBxN->GetSection()->GetVStiff() : NULL;
	CDPoint vFrontUp = GetVectorFrame(0);
	CDPoint vFrontLo = GetVectorFrame(1);

	CDPoint xyCenStt(0,0), xyCenEnd(0,0), xyStt(0,0), xyEnd(0,0);
	xyCenStt = pGir->GetXyGirderDis(pBx->GetStation());
	xyCenEnd = pGirN->GetXyGirderDis(pBxN->GetStation());
	CDPoint vDir  = (xyCenEnd-xyCenStt).Unit();
	CDPoint vPlan = pBx->GetAngleSectionCrossVBracing().RotateInv(vGir);

	CDPoint xyBeamStt = xyCenStt + vDir*(m_dS+m_dG[0]*fabs(vFrontUp.x));
	CDPoint xyBeamEnd = xyCenEnd - vDir*(m_dS+m_dG[1]*fabs(vFrontUp.x));
	if(nPos==LOWER)
	{
		xyBeamStt = xyCenStt + vDir*(m_dS+m_dG[6]*fabs(vFrontLo.x));
		xyBeamEnd = xyCenEnd - vDir*(m_dS+m_dG[7]*fabs(vFrontLo.x));
	}

	CDPoint vX(1,0), vY(0,1);
	CDPoint xyCen1 = (A[1]+A[2])/2 + vX*m_dS - vY*m_dLH1;
	CDPoint xyCen2 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*m_dRH1;
	CDPoint xyCen3 = (A[1]+A[2])/2 + vX*m_dS       - vY*(dHeightGir-m_dLH2);
	CDPoint xyCen4 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*(dHeightGirN-m_dRH2);
	CDPoint xyCen5 = (xyCen3+xyCen4)/2;
	if(m_nType==1)
		xyCen5 = (xyCen1+xyCen2)/2;

	// 투영단면의 실제길이(경사고려)에 평면의 경사를 적용해 실제 빔 길이를 구한다
	switch(nPos)
	{
	case UPPER:
		return ~(xyBeamStt-xyBeamEnd);
	case LOWER:
		return ~(xyBeamStt-xyBeamEnd);
	case LEFT:
		if(m_nType==0)
			return (~(xyCen1-xyCen5)-m_dG[2]-m_dG[3])/fabs(vPlan.y);
		else
			return (~(xyCen3-xyCen5)-m_dG[2]-m_dG[3])/fabs(vPlan.y);

	case RIGHT:
		if(m_nType==0)
			return (~(xyCen2-xyCen5)-m_dG[5]-m_dG[4])/fabs(vPlan.y);
		else
			return (~(xyCen4-xyCen5)-m_dG[5]-m_dG[4])/fabs(vPlan.y);
	}
	return 0;
}
double CVBracing::GetJewonSplice(long nPos, long nDimType)
{
	CPlateBasicIndex* pBx = GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	switch(nPos)
	{
	case LT:
		{
			if(nDimType==HEIGHT)
				return m_duL1+m_duL2;
			else if(nDimType==WIDTH)
				return m_duL3+m_duL4;
			else if(nDimType==THICK)
				return m_dT;
		}
	case LD:
		{
			if(nDimType==HEIGHT)
				return m_ddL1+m_ddL2;
			else if(nDimType==WIDTH)
				return m_ddL3+m_ddL4;
			else if(nDimType==THICK)
				return m_dT;
		}
	case RT:
		{
			if(nDimType==HEIGHT)
				return m_duR1+m_duR2;
			else if(nDimType==WIDTH)
				return m_duR3+m_duR4;
			else if(nDimType==THICK)
				return m_dT;
		}
	case RD:
		{
			if(nDimType==HEIGHT)
				return m_ddR1+m_ddR2;
			else if(nDimType==WIDTH)
				return m_ddR3+m_ddR4;
			else if(nDimType==THICK)
				return m_dT;
		}
	case CEN:
		{
			if(nDimType==HEIGHT)
				return m_dc1+m_dc2;
			else if(nDimType==WIDTH)
				return m_dc3+m_dc4;
			else if(nDimType==THICK)
				return m_dT;
		}
	}
	return 0;
}

CDPoint CVBracing::GetCenterSplice(long nPos)
{
	CGlobarOption *pEtc = ((CPlateBridgeApp*)GetBx()->GetGirder()->GetBridgeApp())->m_pDataManage->GetGlobalOption();
	CPlateBasicIndex* pBx = GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGirN);
	double dWidthGirN = pGirN->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dTWeb = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);
	CDPoint vX(1,0), vY(0,1);
	double dTWStiff = m_pVStiff->m_dWidth[1];
	CDPoint xyMatch;
	switch(nPos)
	{
	case LT:
		{
			double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
			double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
			if(!pEtc->GetInstallBeamAtCenter())
				dHBeamCen = dHBeam/2;
			CDPoint xy1 = GetPointFrame(nPos)-vY*(dHBeam-dHBeamCen);
			CDPoint xy2 = (A[1]+A[2])/2+vX*(dTWeb/2+dTWStiff);
			CVector vFrame = GetVectorFrame(UPPER);
			GetXyMatchLineAndLine(xy1, vFrame, xy2, vY, xyMatch);
				return xyMatch;

		}
	case LD:
		{
			double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
			double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);
			if(!pEtc->GetInstallBeamAtCenter())
				dHBeamCen = dHBeam/2;
			CDPoint xy1 = GetPointFrame(nPos)+vY*(dHBeam-dHBeamCen);
			CDPoint xy2 = (A[1]+A[2])/2+vX*(dTWeb/2+dTWStiff);
			CVector vFrame = GetVectorFrame(LOWER);
			GetXyMatchLineAndLine(xy1, vFrame, xy2, vY, xyMatch);
				return xyMatch;
		}
	case RT:
		{
			double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
			double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
			if(!pEtc->GetInstallBeamAtCenter())
				dHBeamCen = dHBeam/2;
			CDPoint xy1 = GetPointFrame(nPos)-vY*(dHBeam-dHBeamCen);
			CDPoint xy2 = A[7]+vX*(-dTWeb/2-dTWStiff+dWidthGirN/2);
			CVector vFrame = GetVectorFrame(UPPER);
			GetXyMatchLineAndLine(xy1, vFrame, xy2, vY, xyMatch);
				return xyMatch;
		}
	case RD:
		{
			double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
			double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);
			if(!pEtc->GetInstallBeamAtCenter())
				dHBeamCen = dHBeam/2;
			CDPoint xy1 = GetPointFrame(nPos)+vY*(dHBeam-dHBeamCen);
			CDPoint xy2 = A[7]+vX*(-dTWeb/2-dTWStiff+dWidthGirN/2);
			CVector vFrame = GetVectorFrame(LOWER);
			GetXyMatchLineAndLine(xy1, vFrame, xy2, vY, xyMatch);
				return xyMatch;
		}
	case CEN:
		{
			double dHBeam = pBx->GetSection()->GetBeamBaseHeight(m_nType==0 ? CSection::SEC_DOWN : CSection::SEC_UP, TRUE, TRUE);
			double dHBeamCen = pBx->GetSection()->GetBeamJewon(m_nType==0 ? CSection::SEC_DOWN : CSection::SEC_UP, CSection::CENTERDX);
			if(!pEtc->GetInstallBeamAtCenter())
				dHBeamCen = dHBeam/2;
			if(m_nType==TYPE_VBRACING)
				return GetPointFrame(nPos)+vY*(dHBeam-dHBeamCen);
			else
				return GetPointFrame(nPos)-vY*(dHBeam-dHBeamCen);
		}
	}
	return CDPoint(0,0);

}
CDPoint CVBracing::GetPointFrame(long nPos)
{
	CPlateBasicIndex* pBx = GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGirN);
	double dWidthGirN = pGirN->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	double dHeightGirN = pGirN->GetHeightGirderByStaAng(pBxN->GetStation());	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyCen1 = (A[1]+A[2])/2 + vX*m_dS - vY*m_dLH1;
	CDPoint xyCen2 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*m_dRH1;
	CDPoint xyCen3 = (A[1]+A[2])/2 + vX*m_dS       - vY*(dHeightGir-m_dLH2);
	CDPoint xyCen4 = A[7] + vX*(dWidthGirN/2-m_dS) - vY*(dHeightGirN-m_dRH2);
	CDPoint xyCen5 = (xyCen3+xyCen4)/2;
	if(m_nType==1)
		xyCen5 = (xyCen1+xyCen2)/2;

	if(nPos==LT)
		return xyCen1;
	else if(nPos==RT)
		return xyCen2;
	else if(nPos==LD)
		return xyCen3;
	else if(nPos==RD)
		return xyCen4;
	else if(nPos==CEN)
		return xyCen5;
	
	return CDPoint(0,0);

}

CDPoint CVBracing::GetVectorFrame(long nPos)
{
	CPlateBasicIndex* pBx = GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateGirderApp* pGirN = (CPlateGirderApp*)pGir->m_pRight;
	if(pGirN==NULL)		return CDPoint(1,0);

	CPlateBasicIndex* pBxN = pBx->GetBxMatchByCrossBeam(pGirN);
	double dWidthGirN = pGirN->GetWidthOnStation(pBxN->GetStation(), TRUE);
	double dHeightGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());	
	double dHeightGirN = pGirN->GetHeightGirderByStaAng(pBxN->GetStation());	
	CDPoint A[10];
	
	pGir->GetHunchXyDom(pBx, A);

	CDPoint vX(1,0), vY(0,1);
	CDPoint xyCen1 = (A[1]+A[2])/2 + vX*m_dS - vY * m_dLH1;
	CDPoint xyCen2 = A[7] + vX * (dWidthGirN / 2 - m_dS) - vY*m_dRH1;
	CDPoint xyCen3 = (A[1] + A[2]) / 2 + vX * m_dS       - vY * (dHeightGir - m_dLH2);
	CDPoint xyCen4 = A[7] + vX * (dWidthGirN / 2 - m_dS) - vY * (dHeightGirN - m_dRH2);
	CDPoint xyCen5 = (xyCen3 + xyCen4) / 2;

// 	if(pDom != (CDomyun*)0xcdcdcdcd && pDom != (CDomyun*)0x00139b18)
// 	{
// 		CDomyun Dom(pDom);
// 		Dom.Circle(A[1], 100);
// 
// 		*pDom << Dom;
// 	}
// 	
	if(m_nType==1)
		xyCen5 = (xyCen1+xyCen2)/2;
	switch(nPos)
	{
	case UPPER:
		return (xyCen2-xyCen1).Unit();
	case LOWER:
		return (xyCen4-xyCen3).Unit();
	case LEFT:
		{
			if(m_nType==0)
				return (xyCen5-xyCen1).Unit();
			if(m_nType==1)
				return (xyCen5-xyCen3).Unit();
		}
	case RIGHT:
		{
			if(m_nType==0)
				return (xyCen5-xyCen2).Unit();
			if(m_nType==1)
				return (xyCen5-xyCen4).Unit();
		}
	}
	return CDPoint(0,0);
}

CSteelMaterialData* CVBracing::GetSteelMaterialData(long nSteel)
{
	CSteelMaterialData	*pSteelMat = NULL;
	switch(nSteel)
	{
	case STEELMAT_UPPER :
		pSteelMat = m_pSteelMatUp;
		break;
	case STEELMAT_LOWER :
		pSteelMat = m_pSteelMatLo;
		break;
	case STEELMAT_DIAGONAL :
		pSteelMat = m_pSteelMatDiagonal;
		break;
	}

	return pSteelMat;
}

void CVBracing::SetSteelMaterialData(CSteelMaterialData *pSteelMaterialData, long nSteel)
{
	switch(nSteel)
	{
	case STEELMAT_UPPER :
		m_pSteelMatUp = pSteelMaterialData;
		break;
	case STEELMAT_LOWER :
		m_pSteelMatLo = pSteelMaterialData;
		break;
	case STEELMAT_DIAGONAL :
		m_pSteelMatDiagonal = pSteelMaterialData;
		break;
	}
}

void CVBracing::GetXyBaseLR(CDPoint &xyCrL, CDPoint &xyCrR)
{
	xyCrL = xyCrR = CDPoint(0,0);
	CPlateBasicIndex *pBx = GetBx();	
	if(!pBx) return;

	CPlateGirderApp	 *pGir	= pBx->GetGirder();		
	CPlateGirderApp	 *pGirR	= pGir->m_pRight;

	if(!pGirR)	// 마지막 거더에서 가로보를 찾을경우 이전 거더에서 체크한다
	{
		pGirR	= pGir;
		pGir	= pGir->m_pLeft;
		pBx		= pBx->GetBxMatchByCrossBeam(pGir);
	}
	CPlateBasicIndex *pBxR	= pBx->GetBxMatchByCrossBeam(pGirR);
	if(pBxR==NULL) return;

	double	dGirT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W)/2;
	double	dGirTR	= pGirR->GetThickFactByStation(pBxR->GetStation(), G_W)/2;
	CDPoint xyL		= pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint xyR		= pGirR->GetXyGirderDis(pBxR->GetStation());
	CDPoint vCross	= (xyR-xyL).Unit();
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());

	xyCrL	= pGir->GetXyGirderDis(pBx->GetStation(), dGirT, vCross.RotateInv(vGir));
	xyCrR	= pGirR->GetXyGirderDis(pBxR->GetStation(), -dGirTR, vCross.RotateInv(vGir));
}

CDPoint CVBracing::GetVectorPlan()
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	if(!pBx) return CDPoint(0,0);

	CDPoint xyCrL(0,0), xyCrR(0,0);

	GetXyBaseLR(xyCrL, xyCrR);
	return (xyCrR-xyCrL).Unit();
}

// 거더대비 직각인지, 아닌지
BOOL CVBracing::IsSkew()
{
	CPlateBasicIndex *pBx = GetBx();	
	if(!pBx) return FALSE;

	CPlateGirderApp	 *pGir	= pBx->GetGirder();	

	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dCross	= fabs(GetVectorPlan().RotateInv(vGir).y);
	BOOL   bSkew	= Round(dCross,3)==1 ? FALSE : TRUE;

	return bSkew;
}
