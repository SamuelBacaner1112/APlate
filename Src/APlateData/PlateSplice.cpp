// PlateSplice.cpp: implementation of the CPlateSplice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateSplice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateSplice::CPlateSplice()
{
	m_enTypeNum = 0;
	m_bSpliceType = TRUE;
	m_pBx = NULL;
}

CPlateSplice::~CPlateSplice()
{

}

void CPlateSplice::Serialize(CArchive& ar)
{
	long nFlag = 0;
	CPlateSpliceBase::SerializeSpliceBase(ar);

	if(ar.IsStoring())//MAXJRIBSU
	{
		ar << nFlag;
		ar << m_enTypeNum;
		ar << m_bSpliceType;
	}
	else
	{
		ar >> nFlag;
		ar >> m_enTypeNum;
		ar >> m_bSpliceType;
	}
}

void CPlateSplice::CreatePlatePieceObj(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nSide, BOOL bCamberApply, long nShape)
{
	switch(pObj->m_nMarkBuje)
	{
	case SP_U_U:
		CreatePlatePieceObjUpperUpperSplice(pObj, nSide, nShape);
		if(nSide==0)
		{
			CreatePlatePieceBoltObjUpperUpperSplice(pObj, bMakeBoltHole, bMakeBoltLine, -1, nShape);
			if(nShape!=99)
				CreatePlatePieceBoltObjUpperUpperSplice(pObj, bMakeBoltHole, bMakeBoltLine, 1, nShape);
		}
		else
			CreatePlatePieceBoltObjUpperUpperSplice(pObj, bMakeBoltHole, bMakeBoltLine, nSide, nShape);
		break;
	case SP_U_L:
		CreatePlatePieceObjUpperLowerSplice(pObj, nSide, nShape);
		CreatePlatePieceBoltObjUpperLowerSplice(pObj, bMakeBoltHole, bMakeBoltLine, nSide, nShape);
		break;
	case SP_L_U:
		CreatePlatePieceObjLowerUpperSplice(pObj, nSide, nShape);
		CreatePlatePieceBoltObjLowerUpperSplice(pObj, bMakeBoltHole, bMakeBoltLine, nSide, nShape);
		break;
	case SP_L_L:
		CreatePlatePieceObjLowerLowerSplice(pObj, nSide, nShape);
		if(nSide==0)
		{
			CreatePlatePieceBoltObjLowerLowerSplice(pObj, bMakeBoltHole, bMakeBoltLine, -1, nShape);
			CreatePlatePieceBoltObjLowerLowerSplice(pObj, bMakeBoltHole, bMakeBoltLine, 1, nShape);
		}
		else
			CreatePlatePieceBoltObjLowerLowerSplice(pObj, bMakeBoltHole, bMakeBoltLine, nSide, nShape);
		break;
	case SP_W_SH:
		if(m_nWebType!=SP_WEB_FLMO)
		{
			CreatePlatePieceObjWebSHSP(pObj, bCamberApply, nShape);
			CreatePlatePieceBoltObjWebSHSP(pObj, bMakeBoltHole, bMakeBoltLine, bCamberApply, nShape);
		}
		else	// 모멘트판 일체형
		{
			CreatePlatePieceObjWebMOSHSP(pObj, bCamberApply, nShape);
			CreatePlatePieceBoltObjWebMOSHSP(pObj, bMakeBoltHole, bMakeBoltLine, bCamberApply, nShape);
		}
		
		break;
	case SP_W_MO:
		CreatePlatePieceObjWebMOSP(pObj, bCamberApply, nShape);
		CreatePlatePieceBoltObjWebMOSP(pObj, bMakeBoltHole, bMakeBoltLine, bCamberApply, nShape);
		break;
	case SP_W_PLAN:
		CreatePlatePieceObjWebPlan(pObj, nSide, nShape);
		break;
	case SP_W_PLAN_F:
		CreatePlatePieceObjWebPlanFiller(pObj, nSide, nShape);
		break;		
	}
	return;
}

void CPlateSplice::CreatePlatePieceObjUpperUpperSplice(CPlatePieceObject *pObj, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();

	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double gW		= min(gWPre, gWNext);
	double Length = (m_uA + m_uB * m_uC + m_uC1/2)*2;
	double x = 0;
	double y = 0;
	if(nSide==-1)
	{
		if(nShape==-1)
		{
			pObj->AddElement(CDPoint(0,0));
			y -= gW/2;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += gW/2;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y), P_TYPE_SKIP|P_TYPE_LINE,-1,pBx);
			y -= gW/2;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y), P_TYPE_SKIP|P_TYPE_LINE);
			y += gW/2;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y));
			y -= gW/2;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += gW/2;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
	}
	else if(nSide==0)
	{
		if (nShape==-1)
		{
			pObj->AddElement(CDPoint(0,0));
			y -= gW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += gW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y),  0 | P_TYPE_LINE,-1,pBx);
			y -= gW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y), 0 | P_TYPE_LINE);
			y += gW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y));
			y -= gW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += gW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==99)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y),  0 | P_TYPE_LINE,-1,pBx);
			y -= m_uW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y), 0 | P_TYPE_LINE);
			y += m_uW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
	}
	else if(nSide==1)
	{
		if (nShape==-1)
		{
			pObj->AddElement(CDPoint(0,0));
			y -= gW/2;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += gW/2;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			y = -gW/2;
			pObj->AddElement(CDPoint(x,y), P_TYPE_SKIP|P_TYPE_LINE,-1,pBx);
			y -= gW/2;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y), P_TYPE_SKIP|P_TYPE_LINE);
			y += gW/2;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			y = -gW/2;
			pObj->AddElement(CDPoint(x,y));
			y -= gW/2;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += gW/2;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
	}

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,4);
	pObj->SetFaceRange(4,0);
}


void CPlateSplice::CreatePlatePieceBoltObjUpperUpperSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	//
	double BoltR = GetBoltSize();
	long	B  = m_uB;
	double	C  = m_uC;
	double	C1 = m_uC1;
	//nSide조정
	if(nSide==-1)	nSide=0;
	if(nSide==1)	nSide=1;
	long	E = m_uE;
	double RowBoltSu = E + 1;
	double ColBoltSu = (B + 1) * 2;
	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double gW		= min(gWPre, gWNext);
	//
	CDPoint	xy, xytemp1, xytemp2;
	CPlatePieceObject pObjTemp(pBx->GetGirder());
	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,SP_U_U);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,SP_U_U);
	CPlatePieceObject *pObjVLine = new CPlatePieceObject(pGir,SP_U_U);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CreatePlatePieceObjUpperLowerSplice(&pObjTemp, nSide==0?-1:1);
	xytemp1.y = pObjTemp.GetPoint(0).y;
	xytemp2.y = pObjTemp.GetPoint(1).y;
	xy.x = -(B * C + C1/2);
	xy.y = xytemp1.y - (fabs(xytemp2.y) - fabs(xytemp1.y))/2 + (m_uE * m_uF)/2;
	//가로선
	for (long row = 0; row < RowBoltSu; row ++)
	{
		if(nSide==0 && xy.y < -gW/2) continue;
		if(nSide==1 && xy.y > -gW/2) continue;
		pObjLine->AddElement(xy);
		for (long col = 0; col < ColBoltSu; col ++)
		{				
			if( (nShape==-1 && xy.x<0) || (nShape==1 && xy.x>0) || nShape==0 || nShape==99)
			{
				pObjBolt->AddCircle(xy,BoltR,GetBx());
				if(row==0 || row==RowBoltSu-1)
					pObjVLine->AddCircle(xy,BoltR,GetBx());
			}

			if (col == ColBoltSu/2-1)
				xy.x += C1;
			else if (col == ColBoltSu-1)
				xy.x += 0;
			else
				xy.x += C;
		}
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
		xy.x = -(B * C + C1/2);
		xy.y -= m_uF;
	}
	//세로선 
	long ColSu = pObjVLine->GetObjSize();
	for (long n =0; n < ColSu/2; n ++) 
	{
		if(nShape==-1 && long(ColSu/2)<0)	continue;
		if(nShape== 1 && long(ColSu/2)>0)	continue;

		xy = pObjVLine->GetPoint(n);
		pObjLine->AddElement(xy);
		xy = pObjVLine->GetPoint(n+ColSu/2);
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
	}

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);
	//
	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;

	if (bMakeBoltLine)	pObj->AddChildObj(pObjLine);
	else delete pObjLine;

	delete pObjVLine;
}



//nSide=-1:좌측, nSide=1:우측 이나 m_uE[nSide]에서 조정함.
void CPlateSplice::CreatePlatePieceObjUpperLowerSplice(CPlatePieceObject *pObj, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();

	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double gW		= min(gWPre, gWNext);
	double Length = (m_uA + m_uB * m_uC + m_uC1/2)*2;
	double x = 0;
	double y = 0;
	
	if(nSide==-1)
	{
		if(nShape==-1)
		{
			pObj->AddElement(CDPoint(0,0));
			y -= m_uW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_uW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y), P_TYPE_LINE,-1,pBx);
			y -= m_uW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y));
			y += m_uW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y), P_TYPE_LINE,-1,pBx);
			y -= m_uW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_uW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}

		pObj->SetFaceRange(0,1);
		pObj->SetFaceRange(1,2);
		pObj->SetFaceRange(2,3);
		pObj->SetFaceRange(3,4);
		pObj->SetFaceRange(4,0);
	}
	else if(nSide==1)
	{
		double uW1 = gW - 2*m_uW;
		if(nShape==-1)
		{
			pObj->AddElement(CDPoint(0,0));
			y -= m_uW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_uW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			y = -m_uW-uW1;

			pObj->AddElement(CDPoint(x,y), P_TYPE_LINE,-1,pBx);
			y -= m_uW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y));
			y += m_uW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			y = -m_uW-uW1;

			pObj->AddElement(CDPoint(x,y), P_TYPE_LINE,-1,pBx);
			y -= m_uW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_uW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}

		pObj->SetFaceRange(0,1);
		pObj->SetFaceRange(1,2);
		pObj->SetFaceRange(2,3);
		pObj->SetFaceRange(3,4);
		pObj->SetFaceRange(4,0);
	}
}



//nSide=-1:좌측, nSide=1:우측 이나 m_uE[nSide]에서 조정함.
void CPlateSplice::CreatePlatePieceBoltObjUpperLowerSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	//
	double BoltR = GetBoltSize();
	long	B  = m_uB;
	double	C  = m_uC;
	double	C1 = m_uC1;
	//nSide조정
	if(nSide==-1)	nSide=0;
	if(nSide==1)	nSide=1;
	long	E = m_uE;
	double RowBoltSu = E + 1;
	double ColBoltSu = (B + 1) * 2;
	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double gW		= min(gWPre, gWNext);
	//
	CDPoint	xy, xytemp1, xytemp2;
	CPlatePieceObject pObjTemp(pBx->GetGirder());
	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,SP_U_L);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,SP_U_L);
	CPlatePieceObject *pObjVLine = new CPlatePieceObject(pGir,SP_U_L);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CreatePlatePieceObjUpperLowerSplice(&pObjTemp, nSide==0?-1:1);
	xytemp1.y = pObjTemp.GetPoint(0).y;
	xytemp2.y = pObjTemp.GetPoint(1).y;
	xy.x = -(B * C + C1/2);
	xy.y = xytemp1.y - (fabs(xytemp2.y) - fabs(xytemp1.y))/2 + (m_uE * m_uF)/2;
	//가로선
	for (long row = 0; row < RowBoltSu; row ++)
	{
		if(nSide==0 && xy.y < -gW/2) continue;
		if(nSide==1 && xy.y > -gW/2) continue;
		pObjLine->AddElement(xy);
		for (long col = 0; col < ColBoltSu; col ++)
		{				
			if( (nShape==-1 && xy.x<0) || (nShape==1 && xy.x>0) || nShape==0)
			{
				pObjBolt->AddCircle(xy,BoltR,GetBx());
				if(row==0 || row==RowBoltSu-1)
					pObjVLine->AddCircle(xy,BoltR,GetBx());
			}
			if (col == ColBoltSu/2-1)
				xy.x += C1;
			else if (col == ColBoltSu-1)
				xy.x += 0;
			else
				xy.x += C;
		}
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
		xy.x = -(B * C + C1/2);
		xy.y -= m_uF;
	}
	//세로선 
	long ColSu = pObjVLine->GetObjSize();
	for (long n =0; n < ColSu/2; n ++) 
	{
		if(nShape==-1 && long(ColSu/2)<0)	continue;
		if(nShape== 1 && long(ColSu/2)>0)	continue;
		
		xy = pObjVLine->GetPoint(n);
		pObjLine->AddElement(xy);
		xy = pObjVLine->GetPoint(n+ColSu/2);
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
	}

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);
	//
	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;

	if (bMakeBoltLine)	pObj->AddChildObj(pObjLine);
	else delete pObjLine;

	delete pObjVLine;
}
void CPlateSplice::CreatePlatePieceObjLowerUpperSplice(CPlatePieceObject *pObj, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();

	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double gW		= min(gWPre, gWNext);
	double Length = (m_dA + m_dB * m_dC + m_dC1/2)*2;
	double x = 0;
	double y = 0;
	
	if(nSide==-1)
	{
		if(nShape==-1)
		{
			x = 0;
			y = 0;
			pObj->AddElement(CDPoint(x,y));
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y), P_TYPE_LINE,-1,pBx);
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y));
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			y = 0;
			pObj->AddElement(CDPoint(x,y));
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==99)
		{
			double dW1 = gW - 2*m_dW;
			x = Length/2;
			y = -(gW + dW1)/2;
			pObj->AddElement(CDPoint(x,y),  0 | P_TYPE_LINE,-1,pBx);
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y), 0 | P_TYPE_LINE);
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}

		pObj->SetFaceRange(0,1);
		pObj->SetFaceRange(1,2);
		pObj->SetFaceRange(2,3);
		pObj->SetFaceRange(3,4);
		pObj->SetFaceRange(4,0);
	}
	else if(nSide==1)
	{
		double dW1 = gW - 2*m_dW;

		if(nShape==-1)
		{
			x = 0;
			y = -m_dW-dW1;
			pObj->AddElement(CDPoint(x,y));
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			y = -m_dW-dW1;
			pObj->AddElement(CDPoint(x,y), P_TYPE_LINE,-1,pBx);
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y));
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			y = -m_dW-dW1;
			pObj->AddElement(CDPoint(x,y), P_TYPE_LINE,-1,pBx);
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==99)	// 존재하지 않는 Shape
		{
			double dW1 = gW - 2*m_dW;
			x = Length/2;
			y = -(gW + dW1)/2;
			pObj->AddElement(CDPoint(x,y),  0 | P_TYPE_LINE,-1,pBx);
			y -= m_dW;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y), 0 | P_TYPE_LINE);
			y += m_dW;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}

		pObj->SetFaceRange(0,1);
		pObj->SetFaceRange(1,2);
		pObj->SetFaceRange(2,3);
		pObj->SetFaceRange(3,4);
		pObj->SetFaceRange(4,0);
	}

}

void CPlateSplice::CreatePlatePieceBoltObjLowerUpperSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	//
	double BoltR = GetBoltSize();
	long	B  = m_dB;
	double	C  = m_dC;
	double	C1 = m_dC1;
	//nSide조정
	if(nSide==-1)	nSide=0;
	if(nSide==1)	nSide=1;
	long	E = m_dE;
	double RowBoltSu = E + 1;
	double ColBoltSu = (B + 1) * 2;
	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double gW		= min(gWPre, gWNext);
	//
	CDPoint	xy, xytemp1, xytemp2;
	CPlatePieceObject pObjTemp(pBx->GetGirder());
	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,SP_L_U);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,SP_L_U);
	CPlatePieceObject *pObjVLine = new CPlatePieceObject(pGir,SP_L_U);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CreatePlatePieceObjLowerUpperSplice(&pObjTemp, nSide==0?-1:1, nShape);
	xytemp1.y = pObjTemp.GetPoint(0).y;
	xytemp2.y = pObjTemp.GetPoint(1).y;
	xy.x = -(B * C + C1/2);
	xy.y = xytemp1.y - (fabs(xytemp2.y) - fabs(xytemp1.y))/2 + (m_dE * m_dF)/2;
	//가로선
	for (long row = 0; row < RowBoltSu; row ++)
	{
		if(!nShape)
		{
			if(nSide==0 && xy.y < -gW/2) continue;
			if(nSide== 1 && xy.y > -gW/2) continue;
		}
		pObjLine->AddElement(xy);
		for (long col = 0; col < ColBoltSu; col ++)
		{				
			if( (nShape==-1 && xy.x<0) || (nShape==1 && xy.x>0) || nShape==0 || nShape==99)
			{
				pObjBolt->AddCircle(xy,BoltR,GetBx());
				if(row==0 || row==RowBoltSu-1)
					pObjVLine->AddCircle(xy,BoltR,GetBx());
			}
			if (col == ColBoltSu/2-1)
				xy.x += C1;
			else if (col == ColBoltSu-1)
				xy.x += 0;
			else
				xy.x += C;
		}
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
		xy.x = -(B * C + C1/2);
		xy.y -= m_dF;
	}
	//세로선 
	long ColSu = pObjVLine->GetObjSize();
	for (long n =0; n < ColSu/2; n ++) 
	{
		if(nShape==-1 && long(ColSu/2)<0)	continue;
		if(nShape== 1 && long(ColSu/2)>0)	continue;
		
		xy = pObjVLine->GetPoint(n);
		pObjLine->AddElement(xy);
		xy = pObjVLine->GetPoint(n+ColSu/2);
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
	}

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);
	//
	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;

	if (bMakeBoltLine)	pObj->AddChildObj(pObjLine);
	else delete pObjLine;

	delete pObjVLine;
}

void CPlateSplice::CreatePlatePieceObjLowerLowerSplice(CPlatePieceObject *pObj, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();

	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double gW		= min(gWPre, gWNext);
	double Length = (m_dA + m_dB * m_dC + m_dC1/2)*2;
	double x = Length/2;
	double y = (nSide==1) ? -gW/2 : 0;
	pObj->AddElement(CDPoint(x,y), ((nSide==1 || nSide==-1) ? P_TYPE_SKIP : 0) | P_TYPE_LINE,-1,pBx);
	y -= (nSide==0) ? gW : gW/2;
	pObj->AddElement(CDPoint(x,y));
	x -= Length;
	pObj->AddElement(CDPoint(x,y), ((nSide==1 || nSide==-1) ? P_TYPE_SKIP : 0) | P_TYPE_LINE);
	y += (nSide==0) ? gW : gW/2;
	pObj->AddElement(CDPoint(x,y));
	pObj->AddElement(CDPoint(x,y));

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,4);
	pObj->SetFaceRange(4,0);
}

void CPlateSplice::CreatePlatePieceBoltObjLowerLowerSplice(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();
	//
	double BoltR = GetBoltSize();
	long	B  = m_dB;
	double	C  = m_dC;
	double	C1 = m_dC1;
	//nSide조정
	if(nSide==-1)	nSide=0;
	if(nSide==1)	nSide=1;
	long	E = m_dE;
	double RowBoltSu = E + 1;
	double ColBoltSu = (B + 1) * 2;
	double gWPre    = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, FALSE);
	double gWNext   = pGir->GetWidthOnStation(pBx->GetStation(), FALSE, TRUE);
	double gW		= min(gWPre, gWNext);
	//
	CDPoint	xy, xytemp1, xytemp2;
	CPlatePieceObject pObjTemp(pBx->GetGirder());
	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,SP_L_U);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,SP_L_U);
	CPlatePieceObject *pObjVLine = new CPlatePieceObject(pGir,SP_L_U);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CreatePlatePieceObjLowerUpperSplice(&pObjTemp, nSide==0?-1:1);
	xytemp1.y = pObjTemp.GetPoint(0).y;
	xytemp2.y = pObjTemp.GetPoint(1).y;
	xy.x = -(B * C + C1/2);
	xy.y = xytemp1.y - (fabs(xytemp2.y) - fabs(xytemp1.y))/2 + (m_dE * m_dF)/2;
	//가로선
	for (long row = 0; row < RowBoltSu; row ++)
	{
		if(nSide==0 && xy.y < -gW/2) continue;
		if(nSide== 1 && xy.y > -gW/2) continue;
		pObjLine->AddElement(xy);
		for (long col = 0; col < ColBoltSu; col ++)
		{				
			if( (nShape==-1 && xy.x<0) || (nShape==1 && xy.x>0) || nShape==0)
			{
				pObjBolt->AddCircle(xy,BoltR,GetBx());
				if(row==0 || row==RowBoltSu-1)
					pObjVLine->AddCircle(xy,BoltR,GetBx());
			}
			if (col == ColBoltSu/2-1)
				xy.x += C1;
			else if (col == ColBoltSu-1)
				xy.x += 0;
			else
				xy.x += C;
		}
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
		xy.x = -(B * C + C1/2);
		xy.y -= m_dF;
	}
	//세로선 
	long ColSu = pObjVLine->GetObjSize();
	for (long n =0; n < ColSu/2; n ++) 
	{
		if(nShape==-1 && long(ColSu/2)<0)	continue;
		if(nShape== 1 && long(ColSu/2)>0)	continue;
		
		xy = pObjVLine->GetPoint(n);
		pObjLine->AddElement(xy);
		xy = pObjVLine->GetPoint(n+ColSu/2);
		pObjLine->AddElement(xy);
		pObjLine->SetEnd();
	}

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);
	//
	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;

	if (bMakeBoltLine)	pObj->AddChildObj(pObjLine);
	else delete pObjLine;

	delete pObjVLine;
}



//복부판 모멘트 이음판 외측 내측 
//기준(0,0) 상단 중앙 (12시방향) 
//시계방향 
//x = 진행방향 y = 진행방향 직각방향 
void CPlateSplice::CreatePlatePieceObjWebMOSP(CPlatePieceObject *pObj, BOOL bCamberApply, long nShape)
{
	CPlateBasicIndex	*pBx	= GetBx();
	CPlateGirderApp		*pGir	= pBx->GetGirder();

	double JongSlop = pGir->GetGiulWebSplice(GetBx(), TRUE);
	double BottSlop = pGir->GetGiulWebSplice(GetBx(),FALSE);
	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;

	long nRow = pObj->m_nRow;
	double dLengthWeb	= pGir->GetHeightGirderByStaAng(pBx->GetStation());
	if(!CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper())
		dLengthWeb -= max(0, upperTn-upperTp);
	if(CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower())
		dLengthWeb -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이
	
	double Cy  = nRow ? dLengthWeb - m_mG - m_mW : m_mG;
	double wid = m_mA + m_mB * m_mC + m_mC1/2;

	CDPoint xyRT;
	xyRT.x = wid;
	xyRT.y = -Cy + wid*JongSlop;

	CDPoint xyRD = xyRT;
	if (nRow == 0)
		xyRD.y -= m_mW;
	else
		xyRD.y -= m_mW + wid*JongSlop - wid*BottSlop;

	CDPoint xyLD = xyRD;
	xyLD.x = -wid;
	if (nRow == 0)
		xyLD.y -= wid*2*JongSlop;
	else
		xyLD.y -= wid*2*BottSlop;

	CDPoint xyLT = xyLD;
	if (nRow == 0)
		xyLT.y += m_mW;
	else
		xyLT.y += m_mW + wid*BottSlop - wid*JongSlop;
	
	if(nShape==-1)
	{
		pObj->AddElement((xyRT+xyLT)/2);
		pObj->AddElement((xyRD+xyLD)/2);
		pObj->AddElement(xyLD);
		pObj->AddElement(xyLT);
		pObj->AddElement(xyLT);
	}
	else if(nShape==0)
	{
		pObj->AddElement(xyRT, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyRD);
		pObj->AddElement(xyLD);
		pObj->AddElement(xyLT);
		pObj->AddElement(xyLT);
	}
	else if(nShape==1)
	{
		pObj->AddElement(xyRT, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyRD);
		pObj->AddElement((xyRD+xyLD)/2);
		pObj->AddElement((xyRT+xyLT)/2);
		pObj->AddElement((xyRT+xyLT)/2);
	}

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,4);
	pObj->SetFaceRange(4,0);
}

//복부판 전단 이음판 외측 내측 
//기준(0,0) 상단 중앙 (12시방향) 
//시계방향 
//x = 진행방향 y = 진행방향 직각방향 
void CPlateSplice::CreatePlatePieceObjWebMOSHSP(CPlatePieceObject *pObj, BOOL bCamberApply, long nShape)
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp	*pGir  = pBx->GetGirder();

	double JongSlop = pGir->GetGiulWebSplice(GetBx(), TRUE);
	double BottSlop = pGir->GetGiulWebSplice(GetBx(),FALSE);
	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;
	
	double dLengthWeb  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	if(!CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper())
		dLengthWeb -= max(0, upperTn-upperTp);
	if(CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower())
		dLengthWeb -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이
	double CyM	= m_mG;
	double widM	= m_mA + m_mB * m_mC + m_mC1/2;

	double mG	=  m_mG ;
	double mW	=  m_mW ;
	double sW	=  m_sW ;
	double sA	=  m_sA ;
	double sB	=  m_sB ;
	double sC	=  m_sC ;
	double sC1	=  m_sC1;
	double Dis	= ( dLengthWeb - (mG*2+mW*2+sW) ) / 2;
	double CyS	= mG + mW + Dis;
	double widS	= sA + sB * sC + sC1/2;

	// 상부 모멘트 ///////////////////////////////////////////////////////////	
	CDPoint xyTRT(widM, -CyM + widM*JongSlop);
	CDPoint xyTRD(xyTRT.x, xyTRT.y-mW);
	CDPoint xyTLD(xyTRD.x-widM*2, xyTRD.y-widM*2*JongSlop);
	CDPoint xyTLT(xyTLD.x, xyTLD.y+mW);
	// 전단이음 //////////////////////////////////////////////////////////////
	CDPoint xyMRT(widS, -CyS + widS*JongSlop);
	CDPoint xyMRD(widS, xyMRT.y - sW);
	CDPoint xyMLD(-widS, -widS*JongSlop - CyS - sW);
	CDPoint xyMLT(-widS, xyMLD.y + sW);
	// 하부 모멘트 ///////////////////////////////////////////////////////////
	CyM  = dLengthWeb - m_mG - m_mW;
	CDPoint xyDRT(widM, -CyM + widM*JongSlop);
	CDPoint xyDRD(xyDRT.x, xyDRT.y-mW - widM*JongSlop + widM*BottSlop);
	CDPoint xyDLD(xyDRD.x-widM*2, xyDRD.y-widM*2*BottSlop);
	CDPoint xyDLT(xyDLD.x, xyDLD.y+mW - widM*JongSlop + widM*BottSlop);
	//////////////////////////////////////////////////////////////////////////	

	if(nShape==-1)
	{
		pObj->AddElement(xyMLT, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyTLD);
		pObj->AddElement(xyTLT);
		pObj->AddElement((xyTLT+xyTRT)/2);
		pObj->AddElement((xyDLD+xyDRD)/2);
		pObj->AddElement(xyDLD);
		pObj->AddElement(xyDLT);
		pObj->AddElement(xyMLD);
		pObj->AddElement(xyMLD);
	}
	else if(nShape==0)
	{
		pObj->AddElement(xyMLT, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyTLD);
		pObj->AddElement(xyTLT);
		pObj->AddElement(xyTRT);
		pObj->AddElement(xyTRD);
		pObj->AddElement(xyMRT);
		pObj->AddElement(xyMRD);
		pObj->AddElement(xyDRT);
		pObj->AddElement(xyDRD);
		pObj->AddElement(xyDLD);
		pObj->AddElement(xyDLT);
		pObj->AddElement(xyMLD);
		pObj->AddElement(xyMLD);
	}
	else if(nShape==1)
	{
		pObj->AddElement((xyTLT+xyTRT)/2);
		pObj->AddElement(xyTLD);
		pObj->AddElement(xyTLT);
		pObj->AddElement(xyTRT);
		pObj->AddElement(xyTRD);
		pObj->AddElement(xyMRT);
		pObj->AddElement(xyMRD);
		pObj->AddElement(xyDRT);
		pObj->AddElement(xyDRD);
		pObj->AddElement((xyDLD+xyDRD)/2);
		pObj->AddElement((xyDLD+xyDRD)/2);
	}
	long n = 0;
	for(n=0; n<pObj->GetObjSize()-1; n++)
		pObj->SetFaceRange(n, n+1);
	pObj->SetFaceRange(n, 0);
}

// 복부판 평면 필러
// nSide : -1: 거더좌측, 0 : 좌우 모두, 1 : 거더우측 .. 0:좌우 모두 는 그리지 않음-거더 복부판에 의해 나뉘므로
// nShape : -1:복부판 좌측절반, 0:복부판 전면, 1:복부판 우측절반
void CPlateSplice::CreatePlatePieceObjWebPlanFiller(CPlatePieceObject *pObj, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();

	double dWebTPre		= pBx->GetFactChain(G_W, FALSE)->m_dFactChainThick;
	double dWebTNext	= pBx->GetFactChain(G_W, TRUE)->m_dFactChainThick;
	double dWebT		= max(dWebTPre, dWebTNext);
	double dFillerT		= fabs(dWebTPre-dWebTNext)/2;
	double Length		= (m_sA + m_sB * m_sC + m_sC1/2)*2;
	double x = 0;
	double y = 0;
	if(m_nWebType != SP_WEB_FL)	// 모멘트or모멘트일체형 일 경우
	{
		Length	= (m_mA + m_mB * m_mC + m_mC1/2)*2;
	}

	BOOL bNextFiller = TRUE;
	if(!pBx->IsFiller(BX_WEB, bNextFiller))	return;	// 복부판 필러가 없으면 리턴
	if(nShape==-1	&& bNextFiller)			return;	// 좌측절반을 그리는데 필러가 우측에 있으면 리턴
	if(nShape==1	&& !bNextFiller)		return;	// 좌측절반을 그리는데 필러가 좌측에 있으면 리턴

	if(!bNextFiller)	x = 0;
	else				x = Length/2;

	if(nSide==-1)		y = dWebT/2;
	else if(nSide==1)	y = -dWebT/2+dFillerT;

	pObj->AddElement(CDPoint(x,y));
	y -= dFillerT;
	pObj->AddElement(CDPoint(x,y));
	x -= Length/2;
	pObj->AddElement(CDPoint(x,y));
	y += dFillerT;
	pObj->AddElement(CDPoint(x,y));
	pObj->AddElement(CDPoint(x,y));

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,4);
	pObj->SetFaceRange(4,0);
}

// 복부판 평면도
// nSide : -1: 거더좌측, 0 : 좌우 모두, 1 : 거더우측 .. 0:좌우 모두 는 그리지 않음-거더 복부판에 의해 나뉘므로
// nShape : -1:복부판 좌측절반, 0:복부판 전면, 1:복부판 우측절반
void CPlateSplice::CreatePlatePieceObjWebPlan(CPlatePieceObject *pObj, long nSide, long nShape)
{
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)GetBx();
	CPlateGirderApp* pGir = (CPlateGirderApp*)pBx->GetGirder();

	double dWebTPre		= pBx->GetFactChain(G_W, FALSE)->m_dFactChainThick;
	double dWebTNext	= pBx->GetFactChain(G_W, TRUE)->m_dFactChainThick;
	double dWebT		= max(dWebTPre, dWebTNext);
	double dSpT			= m_sT;
	double Length		= (m_sA + m_sB * m_sC + m_sC1/2)*2;
	double x = 0;
	double y = 0;
	if(m_nWebType != SP_WEB_FL)	// 모멘트or모멘트일체형 일 경우
	{
		dSpT	= m_mT;
		Length	= (m_mA + m_mB * m_mC + m_mC1/2)*2;
	}

	if(nSide==-1)
	{
		y = dWebT/2+dSpT;
		if(nShape==-1)
		{
			x  = 0;
			pObj->AddElement(CDPoint(x,y));
			y -= dSpT;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += dSpT;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			pObj->AddElement(CDPoint(x,y));
			y -= dSpT;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y));
			y += dSpT;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			pObj->AddElement(CDPoint(x,y));
			y -= dSpT;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += dSpT;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
	}
	else if(nSide==1)
	{
		y = -dWebT/2;
		if (nShape==-1)
		{
			x = 0;
			pObj->AddElement(CDPoint(x,y));
			y -= dSpT;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += dSpT;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==0)
		{
			x = Length/2;
			pObj->AddElement(CDPoint(x,y));
			y -= dSpT;
			pObj->AddElement(CDPoint(x,y));
			x -= Length;
			pObj->AddElement(CDPoint(x,y));
			y += dSpT;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
		else if(nShape==1)
		{
			x = Length/2;
			pObj->AddElement(CDPoint(x,y));
			y -= dSpT;
			pObj->AddElement(CDPoint(x,y));
			x -= Length/2;
			pObj->AddElement(CDPoint(x,y));
			y += dSpT;
			pObj->AddElement(CDPoint(x,y));
			pObj->AddElement(CDPoint(x,y));
		}
	}

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,4);
	pObj->SetFaceRange(4,0);
}

//복부판 전단 이음판 외측 내측 
//기준(0,0) 상단 중앙 (12시방향) 
//시계방향 
//x = 진행방향 y = 진행방향 직각방향 
void CPlateSplice::CreatePlatePieceObjWebSHSP(CPlatePieceObject *pObj, BOOL bCamberApply, long nShape)
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp	*pGir  = pBx->GetGirder();

	double JongSlop = pGir->GetGiulWebSplice(GetBx(), TRUE);
	double BottSlop = pGir->GetGiulWebSplice(GetBx(),FALSE);
	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;

	double dLengthWeb  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	if(!CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper())
		dLengthWeb -= max(0, upperTn-upperTp);
	if(CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower())
		dLengthWeb -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이
	
	double mG  =  m_mG ;
	double mW  =  m_mW ;
	double sW  =  m_sW ;
	double sA  =  m_sA ;
	double sB  =  m_sB ;
	double sC  =  m_sC ;
	double sC1 =  m_sC1;
	double Dis = ( dLengthWeb - (mG*2+mW*2+sW) ) / 2;
	double Cy  = mG + mW + Dis;
	double wid = sA + sB * sC + sC1/2;

	CDPoint xyLT, xyRT, xyLD, xyRD;
	xyRT.x = wid; 
	xyRT.y = -Cy + wid*JongSlop;

	xyRD.x = wid; 
	if(m_mW==0)
		xyRD.y = xyRT.y - (sW + wid*JongSlop - wid*BottSlop);
	else
		xyRD.y = xyRT.y - sW;

	xyLD.x = -wid;
	if(m_mW==0)
		xyLD.y = -wid*BottSlop - Cy - sW;
	else
		xyLD.y = -wid*JongSlop - Cy - sW;	

	xyLT.x = -wid; 
	if(m_mW==0)
		xyLT.y = xyLD.y + (sW + wid*BottSlop - wid*JongSlop);
	else
		xyLT.y = xyLD.y + sW;		
	
	if(nShape==-1)
	{
		pObj->AddElement((xyRT+xyLT)/2);
		pObj->AddElement((xyRD+xyLD)/2);
		pObj->AddElement(xyLD);
		pObj->AddElement(xyLT);
		pObj->AddElement(xyLT);
	}
	else if(nShape==0)
	{
		pObj->AddElement(xyRT, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyRD);
		pObj->AddElement(xyLD);
		pObj->AddElement(xyLT);
		pObj->AddElement(xyLT);
	}
	else if(nShape==1)
	{
		pObj->AddElement(xyRT, P_TYPE_LINE,-1,pBx);
		pObj->AddElement(xyRD);
		pObj->AddElement((xyRD+xyLD)/2);
		pObj->AddElement((xyRT+xyLT)/2);
		pObj->AddElement((xyRT+xyLT)/2);
	}

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,4);
	pObj->SetFaceRange(4,0);
}

//복부판 모멘트 이음판 외측 내측 
//기준(0,0) 상단 중앙 (12시방향) 
//첫번째 볼트 좌표 ....좌측 상단 --> 우측으로 
//x = 진행방향 y = 진행방향 직각방향 		
void CPlateSplice::CreatePlatePieceBoltObjWebMOSP(CPlatePieceObject *pObj, BOOL bMakeBoltHole, 
										BOOL bMakeBoltLine, BOOL bCamberApply, long nShape)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	*pGir = pBx->GetGirder();

	double BoltR = GetBoltSize();
	CMatrixPoint *xyMat = new CMatrixPoint;
	GetXyMatrix(xyMat,pObj->m_nMarkBuje,pObj->m_nRow,pObj->m_nCol);

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1(0,0);
	CDPoint xy2(0,0);
	long r = 0;
	for(r=0; r<xyMat->GetRows(); r++)
	{
	// 볼트홀
		for(long c=0; c<xyMat->GetCols(); c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			if ((nShape==-1 && xy1.x<0) || (nShape==1 && xy1.x>0) || nShape==0)
				pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}
	// 볼트라인 : 가로방향
	for(r=0; r<xyMat->GetRows(); r++)
	{
		xy1 = xyMat->GetMatrixPoint(r,0);
		xy2 = xyMat->GetMatrixPoint(r,xyMat->GetCols()-1);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	// 볼트라인 : 세로방향
	for(long c=0; c<xyMat->GetCols(); c++)
	{
		xy1 = xyMat->GetMatrixPoint(0,c);
		xy2 = xyMat->GetMatrixPoint(xyMat->GetRows()-1,c);

		if ((nShape==-1 && xy1.x<0) || (nShape==1 && xy1.x>0) || nShape==0)
		{
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
	}

	delete xyMat;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);
	if (bMakeBoltHole)	
		pObj->AddChildObj(pObjBolt);
	else 
		delete pObjBolt;
	if (bMakeBoltLine)	
		pObj->AddChildObj(pObjLine);
	else 
		delete pObjLine;

}
//복부판 전단 이음판 외측 내측 
//기준(0,0) 상단 중앙 (12시방향) 
//첫번째 볼트 좌표 ....좌측 상단 --> 우측으로 
//x = 진행방향 y = 진행방향 직각방향 
void CPlateSplice::CreatePlatePieceBoltObjWebSHSP(CPlatePieceObject *pObj, BOOL bMakeBoltHole, 
										BOOL bMakeBoltLine, BOOL bCamberApply, long nShape)
{
	CreatePlatePieceBoltObjWebMOSP(pObj,bMakeBoltHole,bMakeBoltLine,bCamberApply, nShape);
}

//복부판 전단 이음판 외측 내측 
//기준(0,0) 상단 중앙 (12시방향) 
//첫번째 볼트 좌표 ....좌측 상단 --> 우측으로 
//x = 진행방향 y = 진행방향 직각방향 
void CPlateSplice::CreatePlatePieceBoltObjWebMOSHSP(CPlatePieceObject *pObj, BOOL bMakeBoltHole, 
										BOOL bMakeBoltLine, BOOL bCamberApply, long nShape)
{
	pObj->m_nMarkBuje	= SP_W_MO;
	pObj->m_nRow		= 0;
	pObj->m_nCol		= 1;
	CreatePlatePieceBoltObjWebMOSP(pObj,bMakeBoltHole,bMakeBoltLine,bCamberApply, nShape);
	pObj->m_nMarkBuje	= SP_W_SH;
	pObj->m_nRow		= 0;
	pObj->m_nCol		= 0;
	CreatePlatePieceBoltObjWebMOSP(pObj,bMakeBoltHole,bMakeBoltLine,bCamberApply, nShape);
	pObj->m_nMarkBuje	= SP_W_MO;
	pObj->m_nRow		= 1;
	pObj->m_nCol		= 0;
	CreatePlatePieceBoltObjWebMOSP(pObj,bMakeBoltHole,bMakeBoltLine,bCamberApply, nShape);
}

CPlateSplice& CPlateSplice::operator<<=(const CPlateSplice& Obj)
{
	if( this == &Obj ) return *this;

	CPlateGirderApp* pGir = m_pGir;
	POS psBx = GetPOS();

	*this = Obj;
	this->m_enTypeNum = Obj.m_enTypeNum; 
	this->m_bSpliceType= Obj.m_bSpliceType; 

	SetPOS( psBx );
	m_pGir = pGir;

	return *this;
}

CPlateSplice& CPlateSplice::operator=(const CPlateSplice& Obj)
{
	m_enTypeNum   = Obj.m_enTypeNum;
	m_bSpliceType = Obj.m_bSpliceType;

	if( this == &Obj ) return *this;
	CPlateSpliceBase::operator=(Obj);
	
	return *this;
}
BOOL CPlateSplice::operator ==(const CPlateSplice & Obj)
{
	if(m_enTypeNum	  != Obj.m_enTypeNum)   return FALSE;
	if(m_bSpliceType  != Obj.m_bSpliceType)   return FALSE;

	CPlateSpliceBase *pBase = (CPlateSpliceBase *)&Obj;
	if( !(*pBase == *(CPlateSpliceBase*)this) ) return FALSE;

	return TRUE;
}

BOOL CPlateSplice::IsSameByMarkAssem(CPlateBxObject* pLK,long nMarkAssm,long nRow, long nRowLK) const
{
	return pLK == (CPlateBxObject*)this;
}

BOOL CPlateSplice::IsSameByMarkSection(CPlateBxObject* pLK) const
{
	return pLK == (CPlateBxObject*)this;
}

void CPlateSplice::GetXyMatrix(CMatrixPoint *xyMat, long nMarkBuje, long nRow, long nCol)
{
	switch(nMarkBuje)
	{
		case SP_U_U :
		case SP_U_L :
			GetXyMatrixFlangeUpper(xyMat,nRow,nCol);
			break;
		case SP_L_L :
		case SP_L_U :
			GetXyMatrixFlangeLower(xyMat,nRow,nCol);
			break;
		case SP_W_MO :
			GetXyMatrixWebMoment(xyMat,nRow,nCol);
			break;
		case SP_W_SH :
			GetXyMatrixWebShear(xyMat,nRow,nCol);
			break;
	}
}

void CPlateSplice::GetXyMatrixFlangeUpper(CMatrixPoint *xyMat, long nRow, long nCol)
{
}

void CPlateSplice::GetXyMatrixFlangeLower(CMatrixPoint *xyMat, long nRow, long nCol)
{
}

void CPlateSplice::GetXyMatrixWebShear(CMatrixPoint *xyMat, long nRow, long nCol)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CGlobarOption    *pGlopt = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetGlobalOption();

	double GiulUpper = pGir->GetGiulWebSplice(pBx,TRUE);
	double GiulLower = pGir->GetGiulWebSplice(pBx,FALSE);
	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;

	long	B  = m_sB ;
	double	C  = m_sC ;
	double	C1 = m_sC1;
	long	E  = m_sE ;
	double	F  = m_sF ;
	long	E1 = m_sE1 ;
	double	F1 = m_sF1 ;
	double	G  = m_mG ;
	if(B<0 || E<0) return;

	double dLengthWeb  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	if(!CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper())
		dLengthWeb -= max(0, upperTn-upperTp);
	if(CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower())
		dLengthWeb -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이
	double gap = (dLengthWeb-(G*2+m_mW*2+E*F+E1*F1*2))/2;
	long nArrange = pGlopt->GetDesignBoltHoleArrangeByunSectionType();
	CDPoint xyCen  = CDPoint(0,-(G+m_mW+gap));
	double GiulVar = 0;
	if(m_mW==0 && nArrange==0)	GiulVar = (GiulLower-GiulUpper)/E;

	long nRowsu = E+1;
	long nColsu = (B+1);
	CDPoint xyL(0,0),xyR(0,0);
	xyMat->SetSize(nRowsu+E1*2,nColsu*2);
	long r = 0;
	for(r=0; r<E1; r++)
	{
		double GiulCur = GiulUpper+GiulVar*r;
		for(long c=0; c<nColsu; c++)
		{
			xyL.x = -(C1/2 + C*c);
			xyL.y = xyCen.y - r*F1 + xyL.x*(GiulCur);
			xyR.x = +(C1/2 + C*c);
			xyR.y = xyCen.y - r*F1 + xyR.x*(GiulCur);
			xyMat->SetMatrixPoint(r,nColsu-1-c,xyL);
			xyMat->SetMatrixPoint(r,nColsu+c  ,xyR);
		}
	}
	xyCen.y -= E1*F1;
	for(r=0; r<nRowsu; r++)
	{
		double GiulCur = GiulUpper+GiulVar*(r+E1);
		if(r==(nRowsu+E1*2)-1 && nRow==1 && m_mW==0)
			GiulCur = GiulLower;
		for(long c=0; c<nColsu; c++)
		{
			xyL.x = -(C1/2 + C*c);
			xyL.y = xyCen.y - r*F + xyL.x*(GiulCur);
			xyR.x = +(C1/2 + C*c);
			xyR.y = xyCen.y - r*F + xyR.x*(GiulCur);
			xyMat->SetMatrixPoint(r+E1,nColsu-1-c,xyL);
			xyMat->SetMatrixPoint(r+E1,nColsu+c  ,xyR);
		}
	}
	xyCen.y -= E*F;
	for(r=1; r<E1+1; r++)
	{
		double GiulCur = GiulUpper+GiulVar*(r+E1+E);
		if(r==E1-1 && E1==1 && m_mW==0)
			GiulCur = GiulLower;
		for(long c=0; c<nColsu; c++)
		{
			xyL.x = -(C1/2 + C*c);
			xyL.y = xyCen.y - r*F1 + xyL.x*(GiulCur);
			xyR.x = +(C1/2 + C*c);
			xyR.y = xyCen.y - r*F1 + xyR.x*(GiulCur);
			xyMat->SetMatrixPoint(r+E1+E,nColsu-1-c,xyL);
			xyMat->SetMatrixPoint(r+E1+E,nColsu+c  ,xyR);
		}
	}
}

// (0,0)은 측판이음면의 상단(하부플랜지 하면)
void CPlateSplice::GetXyMatrixWebMoment(CMatrixPoint *xyMat, long nRow, long nCol)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();	
	CGlobarOption    *pGlopt = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetGlobalOption();

	double GiulUpper = pGir->GetGiulWebSplice(pBx,TRUE);
	double GiulLower = pGir->GetGiulWebSplice(pBx,FALSE);
	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;

	long	B  = m_mB ;
	double	C  = m_mC ;
	double	C1 = m_mC1;
	double	D  = m_mD ;
	long	E  = m_mE ;
	double	F  = m_mF ;
	if(B<0 || E<0) return;

	double dLengthWeb = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	if(!CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper())
		dLengthWeb -= max(0, upperTn-upperTp);
	if(CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower())
		dLengthWeb -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이
	long   nArrange   = pGlopt->GetDesignBoltHoleArrangeByunSectionType(); // 균등,일반,수평(x)
	//
	CDPoint xyCen  = CDPoint(0,-D);
	double GiulVar = 0;

	if(nRow==0) 	GiulLower = GiulUpper;
	if(nRow==1)		xyCen.y = -dLengthWeb + (D+E*F);
	if(nArrange==0)	GiulVar = (GiulLower-GiulUpper)/E;
	long nRowsu = E+1;
	long nColsu = (B+1);
	CDPoint xyL(0,0),xyR(0,0);
	xyMat->SetSize(nRowsu,nColsu*2);
	for(long r=0; r<nRowsu; r++)
	{
		double GiulCur = GiulUpper+GiulVar*r;
		if(r==nRowsu-1 && nRow==1)
			GiulCur = GiulLower;
		for(long c=0; c<nColsu; c++)
		{
			xyL.x = -(C1/2 + C*c);
			xyL.y = xyCen.y - r*F + xyL.x*(GiulCur);
			xyR.x = +(C1/2 + C*c);
			xyR.y = xyCen.y - r*F + xyR.x*(GiulCur);
			xyMat->SetMatrixPoint(r,nColsu-1-c,xyL);
			xyMat->SetMatrixPoint(r,nColsu+c  ,xyR);
		}
	}
}

CString CPlateSplice::GetMarkstring()
{
	CPlateSplice    *pSp  = (CPlateSplice*)this;
	CPlateGirderApp *pGir = pSp->GetBx()->GetGirder();

// 	if(pSp->m_bSpliceType==FALSE)	// 용접타입
// 		return "FW";

	CString sPreMark = "";
	long nCnt = 0;
	if(pSp->m_bSpliceType==FALSE)	// 용접타입
		sPreMark = "FW";
	else
		sPreMark = "SP";

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	long nSp = 0;
	long nFw = 0;
	CString sMark="";
	while(pBx)
	{
		CPlateSplice *pSpMatch = pBx->GetSplice();
		if(pSpMatch->m_bSpliceType)
		{
			nSp++;
			nCnt = nSp;
		}
		else
		{
			nFw++;
			nCnt = nFw;
		}

		if(pSp==pSpMatch)
		{
			sMark.Format("%s%d", sPreMark,nCnt);
			return sMark;
		}
		pBx = Finder.GetBxNext();
	}
	return "";
}

double CPlateSplice::GetBoltSize()
{
	CString szBoltSize = m_ZzsBoltSize;
	szBoltSize.Delete(0);
	double dDia = atof(szBoltSize);
	return 22.5/2 + (dDia-20);
}
