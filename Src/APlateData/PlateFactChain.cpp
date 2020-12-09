// PlateFactChain.cpp: implementation of the CPlateFactChain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateFactChain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateFactChain::CPlateFactChain()
{
	m_dFactChainThick    = 0;
	m_dFactChainThickOut = 0;
	m_nMarkBujeFact      = 0;
	m_dWidth             = 400;	
	m_dWidthOut          = 400;	
	m_strSymbol = _T("");

}

CPlateFactChain::~CPlateFactChain()
{

}

void CPlateFactChain::Serialize(CArchive& ar)
{
	long nFlag = 0;

	CPlateBxObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nMarkBujeFact;
		ar << m_dFactChainThick;
		ar << m_dFactChainThickOut;
		ar << m_dWidth;
		ar << m_dWidthOut;
		ar << m_strSymbol;
	}
	else
	{
		ar >> nFlag;
		ar >> m_nMarkBujeFact;
		ar >> m_dFactChainThick;
		ar >> m_dFactChainThickOut;
		ar >> m_dWidth;
		ar >> m_dWidthOut;
		ar >> m_strSymbol;
	}
}

CPlateFactChain& CPlateFactChain::operator=(const CPlateFactChain& Obj)
{
	if( this == &Obj ) return *this;

	CPlateBxObject::operator = (Obj);

	m_dWidth             = Obj.m_dWidth;
	m_dWidthOut          = Obj.m_dWidthOut;
	m_dFactChainThick    = Obj.m_dFactChainThick;
	m_dFactChainThickOut = Obj.m_dFactChainThickOut;
	m_nMarkBujeFact      = Obj.m_nMarkBujeFact;			
	m_strSymbol      = Obj.m_strSymbol;			

	return *this;
}

//<summary>
//
//</summary>
//<return></return>
DWORD CPlateFactChain::GetLinkType()
{ 
	switch ( m_nMarkBujeFact )
	{
		case 0 : return LK_FACTCHAIN_UPPER; 
		case 1 : return LK_FACTCHAIN_LOWER; 
		case 2 : return LK_FACTCHAIN_WEB; 		
	}
	return LK_FACTCHAIN_UPPER;
}

//<summary>
//
//</summary>
//<param name="pLK"></param>
//<return></return>
BOOL CPlateFactChain::IsSameByMarkSection(CPlateBxObject* pLK) const
{
	return pLK == (CPlateBxObject*)this;
}
//<summary>
//
//</summary>
//<param name="bStt"></param>
//<return></return>
BOOL CPlateFactChain::IsSpliceInFactChain(BOOL bStt) const
{
	return bStt ? GetBx()->IsState(BX_SPLICE) : GetBxEnd()->IsState(BX_SPLICE);
}

//<summary>
//
//</summary>
//<param name="bStt"></param>
//<return></return>
CPlateBasicIndex* CPlateFactChain::GetBxOnlyFact(BOOL bStt) const
{
	CPlateFactChain* pFact = (CPlateFactChain*)this;
	if(bStt)
	{
		while(pFact->IsSpliceInFactChain(bStt))
			pFact = pFact->GetBx()->GetFactChain(m_nMarkBujeFact,FALSE);
		return pFact->GetBx();
	}
	else
	{
		while(pFact->IsSpliceInFactChain(bStt))
			pFact = pFact->GetBxEnd()->GetFactChain(m_nMarkBujeFact);
		return pFact->GetBxEnd();
	}

	return NULL;
}
//<summary>
//
//</summary>
//<param name="pLK"></param>
//<return></return>
BOOL CPlateFactChain::IsSameJongBujeInMerge(CPlateBxObject* pLK) const
{
	CPlateFactChain* pFact = (CPlateFactChain*)pLK;

	if(m_dFactChainThick != pFact->m_dFactChainThick) return FALSE;

	return TRUE;
}

//<summary>
//
//</summary>
//<param name="*pObj"></param>
void CPlateFactChain::CreatePieceObj(CPlatePieceObject *pObj) const
{
	CreatePieceObject3D(pObj);

	pObj->SetApplyVectorArray();
	pObj->RotateByFace(0);
	pObj->Set3D(FALSE);
}

//<summary>
//
//</summary>
//<param name="*pObj"></param>
void CPlateFactChain::CreatePieceObject3D(CPlatePieceObject *pObj) const
{
	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd();

	CPlateGirderApp	 *pGir  = pBxStt->GetGirder();
	CLineInfo	 *pLine = pGir->GetLine();	

	CVectorArray vFlange, vHoleStt, vHoleEnd;
	double staStt = pBxStt->GetStation();
	double staEnd = pBxEnd->GetStation();

	switch( m_nMarkBujeFact )
	{
		case	G_F_U:	pGir->GetVertexFlangeByModel(vFlange,pBxStt->GetStation(),pBxEnd->GetStation(),TRUE);
						GetBoltHoleFlange(vHoleStt, pBxStt, TRUE, FALSE);
						GetBoltHoleFlange(vHoleEnd, pBxEnd, TRUE, TRUE) ;	break;
		case	G_F_L:	pGir->GetVertexFlangeByModel(vFlange,pBxStt->GetStation(),pBxEnd->GetStation(),FALSE);
						GetBoltHoleFlange(vHoleStt, pBxStt, FALSE, FALSE);
						GetBoltHoleFlange(vHoleEnd, pBxEnd, FALSE, TRUE) ;	break;
		case	G_W:	pGir->GetVertexWebByModel(vFlange, staStt, staEnd,TRUE);
						GetBoltHoleWeb(vHoleStt, pBxStt, TRUE, FALSE);
						GetBoltHoleWeb(vHoleEnd, pBxEnd, TRUE, TRUE) ;	break;		
	}

	pObj->GetPiecePointArray() = vFlange;

	CPlatePieceObject *pChild = new CPlatePieceObject(pGir);
	pChild->GetPiecePointArray() = vHoleStt;
	pChild->SetBoltHole(TRUE);
	pChild->SetSolid(FALSE);
	pObj->AddChildObj(pChild);

	pChild = new CPlatePieceObject(pGir);
	pChild->GetPiecePointArray() = vHoleEnd;
	pChild->SetBoltHole(TRUE);
	pChild->SetSolid(FALSE);
	pObj->AddChildObj(pChild);
}

void CPlateFactChain::GetBoltHoleFlange(CVectorArray &vArr, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bSttDir) const
{
	vArr.RemoveAll();
	CPlateSplice *pSp = pBx->GetSplice();
	if (!pSp)
		return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	double sta = pBx->GetStation();
	CVector cBase = pGir->GetXyGirderDis(sta);
	cBase.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), bUpper);
	CVector vXDir = pGir->GetLine()->GetAngleAzimuth(sta);
	CVector vYDir = vXDir.Rotate90();
	vXDir.z = pGir->GetGiulFlangeUpperLowerCamber(sta, 0, bUpper);
	vXDir = vXDir.Unit();

	CPlatePieceObject obj(pGir);
	obj.m_nMarkBuje = bUpper ? SP_U_U : SP_L_L;
	pSp->CreatePlatePieceObj(&obj,TRUE,FALSE);

	CPlatePiecePointArray &vPiece = obj.GetChild(0)->GetPiecePointArray();
	for (long nHole = 0; nHole < vPiece.GetSize(); nHole++)   {
		CDPoint pos = vPiece.GetAt(nHole);
		if ((bSttDir && pos.x >= 0) || (!bSttDir && pos.x <= 0))
			continue;
		vArr.Add(cBase + vXDir * pos.x + vYDir * pos.y);
	}

}

void CPlateFactChain::GetBoltHoleWeb(CVectorArray &vArr, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bSttDir) const
{
	vArr.RemoveAll();
	CPlateSplice *pSp = pBx->GetSplice();
	if (!pSp)
		return;

	CPlateGirderApp	*pGir = pBx->GetGirder();
	double sta = pBx->GetStation();
	CVector cBase = pGir->GetXyGirderDis(sta);
	cBase.z = pGir->GetElevationFlangeCamber(sta, 0, CDPoint(0, 1), TRUE);
	CVector vXDir = pGir->GetLine()->GetAngleAzimuth(sta);
	CDPoint vDirWeb2D(0,0);
	CVector vX = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CVector vY = vX.Rotate90();
	CVector vZ = CVector(0, 0, 1);
	vDirWeb2D = CDPoint(0, bLeft?1:-1).Unit(); 
	CVector   vAxis   = vX.Rotate(pBx->GetAngle()).RotateInv90();
	CVector   vDirWeb = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()).Rotate(pBx->GetAngle());
	          vDirWeb = vDirWeb.GetRotateAsAxis(CVector(0,0,0), vAxis, vDirWeb2D);
	vDirWeb = vDirWeb.Unit();//lower->upper
	vDirWeb	*= (bLeft?1:-1);	

	for (long nType = 0; nType < 3; nType++)  
	{
		CPlatePieceObject obj(pGir);
		switch (nType) 
		{
			case 0 :
					obj.m_nMarkBuje = SP_W_MO;
					obj.m_nRow = 0;
					break;
			case 1 :
					obj.m_nMarkBuje = SP_W_MO;
					obj.m_nRow = 1;
					break;
			case 2 :
					obj.m_nMarkBuje = SP_W_SH;
					break;
		}
		pSp->CreatePlatePieceObj(&obj,TRUE,FALSE);
		CPlatePiecePointArray &vPiece = obj.GetChild(0)->GetPiecePointArray();
		for (long nHole = 0; nHole < vPiece.GetSize(); nHole++)   {
			CDPoint pos = vPiece.GetAt(nHole);
			if ((bSttDir && pos.x >= 0) || (!bSttDir && pos.x <= 0))
				continue;
			vArr.Add(cBase + vXDir * pos.x + vDirWeb * pos.y);
		}
	}
}

/*
void CPlateFactChain::DrawFlangePiece(CDomyun *pDom)
{
	BOOL bUpper = (m_nMarkBujeFact == 0) ? TRUE : FALSE;

	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd();
	
	CPlateGirderApp	 *pGir  = pBxStt->GetGirder();
	CLineInfo	 *pLine = pGir->GetLine();	

	double LSharp = bUpper ? pGir->m_dULeftSharp : pGir->m_dRightSharp;
	double RSharp = bUpper ? pGir->m_dURightSharp: pGir->m_dLeftSharp;		

	if(!m_vFlange.GetSize())
	{
		pGir->GetVertexFlangeByModel(m_vFlange,pBxStt->GetStation(),pBxEnd->GetStation(),bUpper);
		GetFoldingVertex(m_vFlange,m_poFlange);
	}

	pDom->MoveTo(m_poFlange[0]);
	for(long i=2;i<m_poFlange.GetSize();i+=2)
		pDom->LineTo(m_poFlange[i]);
	
	pDom->MoveTo(m_poFlange[1]);
	for(i=3;i<m_poFlange.GetSize();i+=2)
		pDom->LineTo(m_poFlange[i]);

	if(pBxStt->IsState(BX_SPLICE | BX_STT_GIRDER)) pDom->LineTo(m_poFlange[0],m_poFlange[1]);
	if(pBxEnd->IsState(BX_SPLICE | BX_END_GIRDER)) pDom->LineTo(m_poFlange[m_poFlange.GetUpperBound()-1],m_poFlange[m_poFlange.GetUpperBound()]);	
}

void CPlateFactChain::DrawSectionPiece(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	BOOL bUpper = (m_nMarkBujeFact == 0) ? TRUE : FALSE;
	
	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd();
	CPlateGirderApp	 *pGir  = GetGirder();

	double LSharp = bUpper ? pGir->m_dULeftSharp : pGir->m_dRightSharp;
	double RSharp = bUpper ? pGir->m_dURightSharp: pGir->m_dLeftSharp;		

	if(!m_vFlange.GetSize())
	{
		pGir->GetVertexFlangeByModel(m_vFlange,pBxStt->GetStation(),pBxEnd->GetStation(),bUpper);
		GetFoldingVertex(m_vFlange,m_poFlange);
	}

	double dT, Dist;
	CVector vs,ve;
	CDPoint sp,ep;

	if(pBx->IsState(BX_CROSSBEAM_VBRACING))
	{
		dT = pBx->GetDiaphragm()->m_dT_DpDp;
		Dist   = - pGir->m_dWidth/2 - LSharp;
		vs = pGir->GetXyGirderDis(pBx->GetBxLeft()->GetStation() - dT,Dist);		
		sp = GetFoldingVertexOne(vs,m_vFlange,m_poFlange);
		Dist   = pGir->m_dWidth/2 + RSharp;
		ve = pGir->GetXyGirderDis(pBx->GetBxRight()->GetStation() - dT,Dist);		
		ep = GetFoldingVertexOne(ve,m_vFlange,m_poFlange);
		
		double OldLen = pDom->GetMarkMaxLen();
		pDom->SetMarkMaxLen(~(sp-ep)/2);
		pDom->MoveToPiece(sp);
		pDom->LineToPiece(ep,FALSE);
		pDom->SetMarkMaxLen(OldLen);
	}
	if(pBx->IsState(BX_SECTION))
	{
		dT = bUpper ? pBx->GetDanmyun()->m_uRib.m_T1 : pBx->GetDanmyun()->m_dRib.m_T1;
		Dist   = - pGir->m_dWidth/2 - LSharp;
		vs = pGir->GetXyGirderDis(pBx->GetBxLeft()->GetStation() - dT,Dist);		
		sp = GetFoldingVertexOne(vs,m_vFlange,m_poFlange);
		Dist   = pGir->m_dWidth/2 + RSharp;
		ve = pGir->GetXyGirderDis(pBx->GetBxRight()->GetStation() - dT,Dist);		
		ep = GetFoldingVertexOne(ve,m_vFlange,m_poFlange);

		double OldLen = pDom->GetMarkMaxLen();
		pDom->SetMarkMaxLen(~(sp-ep)/2);
		pDom->MoveToPiece(sp);
		pDom->LineToPiece(ep,FALSE);
		pDom->SetMarkMaxLen(OldLen);
	}

}

void CPlateFactChain::DrawWebPiecePyung(CDomyun *pDom)
{
	BOOL bUpper = (m_nMarkBujeFact == 0) ? TRUE : FALSE;

	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd();	
	CPlateGirderApp	 *pGir  = pBxStt->GetGirder();

	CVectorArray vWeb;
	CDPointArray poWeb;	
	pGir->GetVertexPointByDis(vWeb,pBxStt->GetStation(),pBxEnd->GetStation(),-pGir->m_dWidth/2,pGir->m_dWidth/2,bUpper);
	GetFoldingVertex(vWeb,poWeb);

	pDom->MoveToPiece(poWeb[0]);
	for(long i=2;i<poWeb.GetSize();i+=2)
		pDom->LineToPiece(poWeb[i],FALSE);
	
	pDom->MoveToPiece(poWeb[1]);
	for(i=3;i<poWeb.GetSize();i+=2)
		pDom->LineToPiece(poWeb[i],TRUE);	
}

void CPlateFactChain::DrawWebPieceJong(CDomyun *pDom)
{
	BOOL bLeft = (m_nMarkBujeFact == 2) ? TRUE : FALSE;

	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd(); 
	
	CPlateGirderApp	 *pGir  = GetGirder();
	CLineInfo	 *pLine = pGir->GetLine();

	if(!m_poWeb.GetSize())
	{
		pGir->GetVertexWebByModel(m_vWeb,pBxStt->GetStation(),pBxEnd->GetStation(),bLeft);
		GetFoldingVertex(m_vWeb,m_poWeb);
	}

	pDom->MoveTo(m_poWeb[0]);
	for(long i=2;i<m_poWeb.GetSize();i+=2)
		pDom->LineTo(m_poWeb[i]);

	pDom->MoveTo(m_poWeb[1]);
	for(i=3;i<m_poWeb.GetSize();i+=2)
		pDom->LineTo(m_poWeb[i]);

	if(pBxStt->IsState(BX_SPLICE)) pDom->LineTo(m_poWeb[0],m_poWeb[1]);
	if(pBxStt->IsState(BX_STT_GIRDER)) DrawPieceWebFlate(pDom,TRUE);
	if(pBxEnd->IsState(BX_SPLICE)) pDom->LineTo(m_poWeb[m_poWeb.GetUpperBound()-1],m_poWeb[m_poWeb.GetUpperBound()]);	
	if(pBxEnd->IsState(BX_END_GIRDER)) DrawPieceWebFlate(pDom,FALSE);
}

void CPlateFactChain::DrawPieceWebFlate(CDomyun *pDom, BOOL bStart)
{
	BOOL bLeft = (m_nMarkBujeFact == 2) ? TRUE : FALSE;

	CPlateGirderApp	 *pGir  = GetGirder();
	CPlateBridge		 *pDB	= (CPlateBridge*)pGir->GetBridgeApp();
	CPlateBasicIndex  *pBx	= bStart ? GetBx() : GetBxEnd();		

	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd(); 

	if(!m_poWeb.GetSize())
	{
		pGir->GetVertexWebByModel(m_vWeb,pBxStt->GetStation(),pBxEnd->GetStation(),bLeft);
		GetFoldingVertex(m_vWeb,m_poWeb);
	}

	if(bStart)
	{
		if(pBx->IsState(BX_STT_GIRDER))
		{
			double dSta = m_poWeb[0].x;
			double H  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			double C = pDB->m_dHeightTaperStt;
			double D = pGir->GetHeightInOutUpper(TRUE);		// 출입구상단~거더상단
			double E = pDB->m_dHeightInOutStt;
			double R = pDB->m_dRadiusInOutStt;
			double G = pDB->m_dWidthInOutStt - R;
			double Ele = m_poWeb[0].y;

			pDom->LineTo(dSta,Ele ,dSta  ,-D + Ele);
			pDom->LineTo(dSta,-D + Ele    ,dSta + G,-D + Ele);
			pDom->LineTo(dSta,-D-E + Ele  ,dSta + G,-D-E + Ele);
			pDom->LineTo(dSta,-D-E + Ele  ,dSta + G,-D-E + Ele);
			pDom->LineTo(dSta,-D-E + Ele  ,dSta    ,-H + Ele);

			pDom->LineTo(dSta + G+R,-D-R + Ele ,dSta + G+R,-D-E+R + Ele);
			pDom->Arc(dSta + G,-D-R + Ele  ,R,  0,90);	// 기준점,반지름,시작각도,진행각도
			pDom->Arc(dSta + G,-D-E+R + Ele,R,270,90);
		}
		else
		{
			if(pBx->IsState(BX_SPLICE))
			{
				pDom->LineTo(pBx->GetStation(),pGir->GetElevationFlangeCamber(pBx->GetStation(),TRUE),
							 pBx->GetStation(),pGir->GetElevationFlangeCamber(pBx->GetStation(),FALSE));
			}
		}
	}
	else
	{
		if(pBx->IsState(BX_END_GIRDER))
		{
			double dSta = m_poWeb[m_poWeb.GetUpperBound()-1].x;
			double H  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			double C  = pDB->m_dHeightTaperEnd;
			double D  = pGir->GetHeightInOutUpper(FALSE);
			double E  = pDB->m_dHeightInOutEnd;
			double R  = pDB->m_dRadiusInOutEnd;
			double G  = pDB->m_dWidthInOutEnd*(-1.0) + R;
			double Ele = m_poWeb[m_poWeb.GetUpperBound()-1].y;

			pDom->LineTo(dSta,Ele ,dSta  ,-D + Ele);
			pDom->LineTo(dSta,-D + Ele    ,dSta + G,-D + Ele);
			pDom->LineTo(dSta,-D-E + Ele  ,dSta + G,-D-E + Ele);
			pDom->LineTo(dSta,-D-E + Ele  ,dSta + G,-D-E + Ele);
			pDom->LineTo(dSta,-D-E + Ele  ,dSta    ,-H + Ele);

			pDom->LineTo(dSta + G+R*(-1),-D-R + Ele ,dSta + G+R*(-1),-D-E+R + Ele);
			pDom->Arc(dSta + G,-D-R + Ele  ,R, 90,90);
			pDom->Arc(dSta + G,-D-E+R + Ele,R,180,90);
		}
		else
		{
			if(pBx->IsState(BX_SPLICE))
			{
				pDom->LineTo(pBx->GetStation(),pGir->GetElevationFlangeCamber(pBx->GetStation(),TRUE),
							 pBx->GetStation(),pGir->GetElevationFlangeCamber(pBx->GetStation(),FALSE));
			}
		}
	}
}

CDPoint CPlateFactChain::GetP1(double dLeft, double dBottom, BOOL bUpper)
{
	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd();
	CPlateGirderApp	 *pGir  = GetGirder();

	if(!m_vFlange.GetSize())
	{
		pGir->GetVertexFlangeByModel(m_vFlange,pBxStt->GetStation(),pBxEnd->GetStation(),bUpper);
		GetFoldingVertex(m_vFlange,m_poFlange);
	}

	CDPoint vVec = (m_poFlange[0] - m_poFlange[1]).Unit();
	CDPoint vVecBottom = (m_poFlange[1+2] - m_poFlange[1]).Unit();
	
	CDPoint rtPoint;
	GetXyMatchLineAndLine(m_poFlange[1] + vVec.RotateInv90() * dLeft, vVec,
							m_poFlange[1] + vVecBottom.Rotate90() * dBottom,vVecBottom,
							rtPoint);						    
	return rtPoint;
}

CDPoint CPlateFactChain::GetP2(double dRight, double dBottom, BOOL bUpper)
{
	CPlateBasicIndex *pBxStt = GetBx();
	CPlateBasicIndex *pBxEnd = GetBxEnd();
	CPlateGirderApp	 *pGir  = GetGirder();

	if(!m_vFlange.GetSize())
	{
		pGir->GetVertexFlangeByModel(m_vFlange,pBxStt->GetStation(),pBxEnd->GetStation(),bUpper);
		GetFoldingVertex(m_vFlange,m_poFlange);
	}

	CDPoint vVec = (m_poFlange[m_poFlange.GetUpperBound()-1] - m_poFlange[m_poFlange.GetUpperBound()]).Unit();
	CDPoint vVecBottom = (m_poFlange[m_poFlange.GetUpperBound()-2] - m_poFlange[m_poFlange.GetUpperBound()]).Unit();

	CDPoint rtPoint;
	GetXyMatchLineAndLine(m_poFlange[m_poFlange.GetUpperBound()] + vVec.Rotate90() * dRight, vVec,
							m_poFlange[m_poFlange.GetUpperBound()] + vVecBottom.RotateInv90() * dBottom,vVecBottom,
							rtPoint);						    
	return rtPoint;
}
*/
