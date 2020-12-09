// PieceInfoApp.cpp: implementation of the CPieceInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlatePiecePoint.h"
#include "PlatePieceObject.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CPlatePieceObject::m_bUsePieceLayer = TRUE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 목적 : 사용자 변수 초기화
//
//
//
// 
//

CPlatePieceObject::CPlatePieceObject(CPlateGirderApp *pGir, long nMarkBuje, CPlatePieceData *pData, long nRow, long nCol, BOOL bFront)
{

	m_dwAttr = 0;

	SetBitFlag32(m_dwAttr,POJB_SOLID,TRUE);
	SetBitFlag32(m_dwAttr,POJB_HAS_ARC,FALSE);
	SetBitFlag32(m_dwAttr,POJB_CLICKWISE,TRUE);
	SetBitFlag32(m_dwAttr,POJB_2DIM,TRUE);
	SetBitFlag32(m_dwAttr,POJB_FOLD,FALSE);
	SetBitFlag32(m_dwAttr,POJB_HAS_OPEN_UNIT,FALSE);
	SetBitFlag32(m_dwAttr,POJB_PIECEMARK,FALSE);
	SetBitFlag32(m_dwAttr,POJB_LEFT,TRUE);
	SetBitFlag32(m_dwAttr,POJB_USE_CONV_DATA,FALSE);
	SetBitFlag32(m_dwAttr,POJB_3D,FALSE);


	m_nFaceSize	= 0;	
	m_pBxStt = NULL;
	m_pBxEnd = NULL;

	m_pParentObj= NULL;

	m_nRow   = nRow;
	m_nCol   = nCol;
	m_bFront = bFront;
	m_sMarkBuje = _T("");
	m_nQty = 0;
	

	m_pGir = pGir;
	m_pPieceData = pData;

	m_nMarkBuje = nMarkBuje;

	m_nCurIdx = 0;	
	m_nMinIdx = -1;
	m_nMaxIdx = -1;

	m_dHorScale = 1.0;
	m_dVerScale = 1.0;

	m_bAsc = TRUE;
	m_CurPType = 0;

	m_nLayer = -1;
	m_nColor = P_BYLAYER;
	m_bIgnoreAngle = FALSE;
}

CPlatePieceObject::~CPlatePieceObject()
{ 
	for(long i=0;i<GetChildCount();i++)
		delete GetChild(i);	
}
// 목적 : 점요소 추가
// uXy	: 추가를 원하는 좌표
// pType	: 원하는 좌표 속성 
// nIdx		: Insert 원하는 좌표 (-1 : 추가)
// pBx		: Basic Index			
// bulge	: 곡선일경우 Bulge
CPlatePiecePoint *CPlatePieceObject::AddElement(const CDPoint &uXy, P_TYPE pType, long nIdx, CPlateBasicIndex *pBx, double bulge)
{
	CPlatePiecePoint PieceObj(uXy, pType, pBx, bulge, TRUE);	
	CPlatePiecePoint* pResult = NULL;

	if(pType == P_TYPE_ARC || pType == P_TYPE_ELLIPSE) SetBitFlag32(m_dwAttr,POJB_HAS_ARC,TRUE);
	
	if (nIdx < 0 || nIdx == GetObjSize())
		pResult = Add(PieceObj);
	else   { 
		ASSERT (GetObjSize() > nIdx);
		pResult = InsertAt(nIdx, PieceObj);	
		long n = 0;
		for(n=0; n<m_nFaceSize; n++)
		{
			if(GetEndFace(n) >= nIdx)
				SetEndFace(n,GetEndFace(n) + 1);
		}
		for(n=0; n<m_nFaceSize; n++)
		{
			if(GetSttFace(n) >= nIdx)
			SetSttFace(n,GetSttFace(n) + 1);
		}
	}
	return pResult;
}
CPlatePiecePoint* CPlatePieceObject::Add(CPlatePiecePoint Obj)	
{ 
	CPlatePiecePointArray& rArr = m_TPAPoint[GetUseConvData()];
	rArr.Add(Obj); 
	return rArr.GetPAt(rArr.GetUpperBound());
}

CPlatePiecePoint* CPlatePieceObject::InsertAt(long nIdx, CPlatePiecePoint Obj)
{ 
	CPlatePiecePointArray& rArr = m_TPAPoint[GetUseConvData()];
	rArr.InsertAt(nIdx,Obj); 
	return rArr.GetPAt(nIdx);
}

void CPlatePieceObject::RemoveElement(long nIdx)
{
	ASSERT (GetObjSize() > nIdx);
	CPlatePiecePointArray& rArr = m_TPAPoint[GetUseConvData()];
	rArr.RemoveAt(nIdx);	
	long n = 0;
	for(n=0; n<m_nFaceSize; n++)
	{
		if(GetEndFace(n) >= nIdx)
			SetEndFace(n,GetEndFace(n) - 1);
	}
	for(n=0; n<m_nFaceSize; n++)
	{
		if(GetSttFace(n) >= nIdx)
		SetSttFace(n,GetSttFace(n) - 1);
	}		
}

CPlatePiecePoint *CPlatePieceObject::AddElement(const CDPoint &uXy, double R, long nIdx, CPlateBasicIndex *pBx, P_TYPE pType)
{
	ASSERT (GetObjSize() > 0);

	long nPreIdx = nIdx < 0 ? GetUpperBound() : GetPrevIdx(nIdx, P_TYPE_PLATE);
	double bulge = GetArcBulge(GetPoint(nPreIdx), uXy, fabs(R));
	bulge = R > 0 ? bulge : -bulge;

	return AddElement(uXy, pType, nIdx, pBx, bulge);
}

CPlatePiecePoint *CPlatePieceObject::AddCircle(const CDPoint &CenPoint, double R, CPlateBasicIndex *pBx)
{	
	CPlatePiecePoint *pPieceObj = AddElement(CenPoint,P_TYPE_CIRCLE,-1,pBx,R);
	pPieceObj->SetEnd();
	return pPieceObj;
}

void CPlatePieceObject::InsertElement(long nIdx, const CDPoint &uXy, P_TYPE pType, CPlateBasicIndex *pBx, BOOL bPrev)
{
	CPlatePiecePoint PieceObj(uXy, pType, pBx);		
	
	if(bPrev)
	{
		if(nIdx >= GetObjSize())		
			Add(PieceObj);					
		else		
			InsertAt(nIdx,PieceObj);			
	}
	else
	{
		ASSERT(nIdx + 1 >= 0);
		InsertAt(nIdx+1,PieceObj);		
	}
	long n = 0;
	for(n=0; n<m_nFaceSize; n++)
	{
		if(GetEndFace(n) >= nIdx)
			SetEndFace(n,GetEndFace(n) + 1);
	}
	for(n=0; n<m_nFaceSize; n++)
	{
		if(GetSttFace(n) >= nIdx)
		SetSttFace(n,GetSttFace(n) + 1);
	}


}

void CPlatePieceObject::InsertElementOnFace(long nFace, long nLocalIdx, const CDPoint &uXy, P_TYPE pType, CPlateBasicIndex *pBx, BOOL bStt)
{
	CPlatePiecePoint PieceObj(uXy, pType, pBx);		
	
	long nIdx;
	if(bStt)
	{
		nIdx = GetSttFace(nFace);		
		if(nIdx+nLocalIdx+1 >= GetObjSize())		
			Add(PieceObj);							
		else		
			InsertAt(nIdx+nLocalIdx+1,PieceObj);		
	}
	else
	{
		nIdx = GetEndFace(nFace);		
		if(nIdx==0) nIdx = GetObjSize();
		InsertAt(nIdx-nLocalIdx,PieceObj);		
	}
	long n = 0;
	for(n=0; n<m_nFaceSize; n++)
	{
		if(GetEndFace(n) >= nIdx)
			SetEndFace(n,GetEndFace(n) + 1);
	}
	for(n=0; n<m_nFaceSize; n++)
	{
		if(GetSttFace(n) >= nIdx)
		SetSttFace(n,GetSttFace(n) + 1);
	}

}

CPlatePiecePoint *CPlatePieceObject::AddElement(const CDPoint &CenPoint, double R, double DegSttAngle, double DegAngle, long nIdx,
									  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CDPoint CenP = CenPoint;
	CDPoint StartP	= CDPoint (CenPoint.x + R * cos(ToRadian(DegSttAngle)), 
		                       CenPoint.y + R * sin(ToRadian(DegSttAngle)));
	CDPoint EndP	= CDPoint (CenPoint.x + R * cos(ToRadian(DegSttAngle + DegAngle)), 
		                       CenPoint.y + R * sin(ToRadian(DegSttAngle + DegAngle)));
	CDPoint SP;

	if(DegAngle < 0)
	{
		if(IsClockWise()) SP = EndP;
		else			 SP = StartP;
	}
	else
	{
		if(IsClockWise()) SP = StartP;
		else			 SP = EndP;
	}

	AddElement(StartP,P_TYPE_LINE,nIdx,pBxStt);
	if (nIdx < 0)
		return AddElement(EndP,R,nIdx,pBxEnd);
	return AddElement(EndP,R,nIdx+1,pBxEnd);
}

void CPlatePieceObject::SetEnd()
{
	if(GetObjSize() == 0) return;	

	CPlatePiecePoint *pObj = GetLastPointObj();
	pObj->SetEnd();
}

void CPlatePieceObject::SetEnd(const CDPoint &uXy, P_TYPE pType,CPlateBasicIndex *pBx, double len)
{
	ASSERT(GetObjSize() > 0);

	CPlatePiecePoint *pObj = GetLastPointObj();
	pObj->SetPoint(uXy);
	pObj->SetType(pType);
	pObj->SetBx(pBx);
	pObj->SetLength(len);
	pObj->SetEnd();
}

void CPlatePieceObject::SetFace(long nIdx, BOOL bStt, long nStep)
{
	ASSERT(nIdx + nStep >= 0 && nStep <= 0 && nIdx < GetObjSize());
	
	if(bStt) AddSttFace(nIdx + nStep);
	else	 AddEndFace(nIdx + nStep);
	
	if(!bStt) m_nFaceSize ++;
}

void CPlatePieceObject::SetFaceRange(long nIdxStt, long nIdxEnd)
{
	AddSttFace(nIdxStt);
	AddEndFace(nIdxEnd);
	
	m_nFaceSize ++;
}

///////////////////////////// Operator 연산자
void CPlatePieceObject::SetApplyDPointArray(CDPointArray* pDArr)
{
/*	for(long n=0; n<DArr.GetSize(); n++)
	{
		AddElement(DArr.GetAt(n));
	}
	m_nFaceSize = 0;
	SetFaceRange(0,GetObjSize()/2-1);
	SetFaceRange(GetObjSize()/2-1,GetObjSize()/2);
	SetFaceRange(GetObjSize()/2,GetObjSize()-1);
	SetFaceRange(GetObjSize()-1,0);


	SetBitFlag32(m_dwAttr,POJB_CLICKWISE,FALSE);
	*/
}


void CPlatePieceObject::SetApplyVectorArray(CVectorArray* pDArr /*=NULL*/)
{
	CVectorArray vArray;
	if(!pDArr)
	{
		GetPiecePointArray().GetVectorArray(vArray);
		pDArr = &vArray;
	}

	CDPointArray FolderArray;
	GetFoldingVertex(*pDArr,FolderArray);
	long n = 0;
	for(n=1; n<=FolderArray.GetSize(); n+=2)
	{
		AddElement(FolderArray.GetAt(n));
	}	
	for(n=FolderArray.GetUpperBound()-1; n>=0; n-=2)
	{
		AddElement(FolderArray.GetAt(n));
	}
	m_nFaceSize = 0;
	SetFaceRange(0,GetObjSize()/2-1);
	SetFaceRange(GetObjSize()/2-1,GetObjSize()/2);
	SetFaceRange(GetObjSize()/2,GetObjSize()-1);
	SetFaceRange(GetObjSize()-1,0);
	SetClockWise(FALSE);	
}


CPlatePieceObject& CPlatePieceObject::operator=(const CPlatePieceObject& obj)
{	
	m_nLayer		= obj.m_nLayer;
	m_nColor		= obj.m_nColor;		// 라인색
	
	m_dwAttr		= obj.m_dwAttr;		
	m_nFaceSize		= obj.m_nFaceSize;	

	m_pParentObj	= obj.m_pParentObj;	
	m_nMarkBuje		= obj.m_nMarkBuje;
	m_nRow			= obj.m_nRow;
	m_nCol			= obj.m_nCol;

	m_pBxStt		= obj.m_pBxStt;
	m_pBxEnd		= obj.m_pBxEnd;
	
	m_TPAPoint[0] = obj.m_TPAPoint[0];
	m_TPAPoint[1] = obj.m_TPAPoint[1];	
	
	m_TPAFaceStt[0].Copy(obj.m_TPAFaceStt[0]);	
	m_TPAFaceStt[1].Copy(obj.m_TPAFaceStt[1]);	
	m_TPAFaceEnd[0].Copy(obj.m_TPAFaceEnd[0]);	
	m_TPAFaceEnd[1].Copy(obj.m_TPAFaceEnd[1]);	

	return *this;
}

long CPlatePieceObject::GetPrevIdx(long nIdx, P_TYPE pType, long nTime)
{
	long nSize = GetObjSize();
	long nFindNum = 1;
	while(1)
	{
		if (--nIdx < 0)
		{
			nIdx = nSize - 1;
			break;
		}
		if ((pType == 0 || pType & GetPTType(nIdx)))
		{
			if(nTime == nFindNum++) break;
		}			
	}
	return nIdx;
}

long CPlatePieceObject::GetNextIdx(long nIdx, P_TYPE pType, long nTime)
{
	long nSize = GetObjSize();
	long nFindNum = 1;
	while(1)
	{
		if (++nIdx >= nSize)
		{
			nIdx = 0;
			break;
		}
		if (pType == 0 || pType & GetPTType(nIdx))
		{
			if(nTime == nFindNum++) break;
		}	
	}
	return nIdx;
}

long CPlatePieceObject::GetPrevIdxOnBx(long nIdx, P_TYPE pType, __int64 BxType, long nTime)
{
	long nSize = GetObjSize();
	long nFindNum = 1;
	while(1)
	{
		if (--nIdx < 0)
		{
			nIdx = nSize - 1;
			break;
		}
		if(!GetBx(nIdx)) continue;
		if(GetBx(nIdx)->IsState(BxType) && IsPTType(nIdx,pType))
		{
			if(nTime == nFindNum++) break;
		}			
	}
	return nIdx;
}

long CPlatePieceObject::GetNextIdxOnBx(long nIdx, P_TYPE pType , __int64 BxType, long nTime)
{
	long nSize = GetObjSize();
	long nFindNum = 1;
	while(1)
	{
		if (++nIdx >= nSize)
		{
			nIdx = 0;
			break;
		}
		if(!GetBx(nIdx)) continue;
		if(GetBx(nIdx)->IsState(BxType) && IsPTType(nIdx,pType))
		{
			if(nTime == nFindNum++) break;
		}	
	}
	return nIdx;
}

long CPlatePieceObject::GetVertexSu(P_TYPE pType)
{
	long nSize = GetObjSize();
	if (pType == 0)
		return nSize;
	long nCount = 0;
	for(long nIdx = 0; nIdx < nSize; nIdx++)
	{
		if (pType & GetPTType(nIdx))
			nCount++;
	}
	return nCount;
}

void CPlatePieceObject::RegenVertex()
{
	m_TPAPoint[1] = m_TPAPoint[0];
	m_TPAFaceStt[1].Copy(m_TPAFaceStt[0]);
	m_TPAFaceEnd[1].Copy(m_TPAFaceEnd[0]);
	SetUseConvData(TRUE);
	long nIdx = 0;
	for(nIdx = 0; nIdx < GetObjSize(); nIdx++)
	{
		P_TYPE VtxType = GetPTType(nIdx);
		if (VtxType & P_TYPE_SCALLOP)
			nIdx = InsertScallop(nIdx);
		if (VtxType & P_TYPE_CHAMFER)
			nIdx = InsertChamfer(nIdx);
		if (VtxType & P_TYPE_FILLET)
			nIdx = InsertFillet(nIdx);		
		if (VtxType & P_TYPE_JRIB_C)
			nIdx = InsertSpliceScallopC(nIdx);
		if (VtxType & P_TYPE_JRIB_T)
			nIdx = InsertSpliceScallopT(nIdx);
	}

	for(nIdx = 0; nIdx < GetObjSize(); nIdx++)
	{
		P_TYPE VtxType = GetPTType(nIdx);
		if (VtxType & P_TYPE_BREAK)
			nIdx = InsertBreak(nIdx);
	}

	for(nIdx = 0; nIdx < GetChildCount(); nIdx++)
		GetChild(nIdx)->RegenVertex();
}

void CPlatePieceObject::RegenVertexEx(double dScallop, double dChamfer, double dSetBack)
{
	long nIdx = 0;
	for(nIdx = 0; nIdx < GetObjSize(); nIdx++)
	{
		P_TYPE VtxType = GetPTType(nIdx);
		if (VtxType & (P_TYPE_JRIB_T | P_TYPE_JRIB_C))   {
			CVector xy;
			if (VtxType & P_TYPE_JRIB_T)
				xy = GetSetBackPoint(nIdx, FALSE, dSetBack);
			else
				xy = GetSetBackPoint(nIdx,  TRUE, dSetBack);
			SetPTType(nIdx, P_TYPE_DIMPOINT);
			AddElement(xy, VtxType & ~(P_TYPE_JRIB_T | P_TYPE_JRIB_C), ++nIdx, NULL, xy.z);
			SetFaceRange(nIdx-1, nIdx);
		}
	}

	for(nIdx = 0; nIdx < GetObjSize(); nIdx++)
	{
		P_TYPE VtxType = GetPTType(nIdx);

		if (VtxType & P_TYPE_SCALLOP)
			nIdx = InsertScallopAtArc(nIdx, dScallop);
		if (VtxType & P_TYPE_CHAMFER)
			nIdx = InsertChamferAtArc(nIdx, dChamfer);
		if (VtxType & P_TYPE_FILLET)
			nIdx = InsertFillet(nIdx);
	}
}

long CPlatePieceObject::InsertScallop(long nIdx)
{

	double R = GetRadius(nIdx);
	CDPoint cCurr = GetPoint(nIdx);
	CDPoint cPrev = GetPoint(GetPrevIdx(nIdx));
	CDPoint cNext = GetPoint(GetNextIdx(nIdx));

	CDPoint vPrev = (cCurr - cPrev).Unit();
	CDPoint vNext = (cNext - cCurr).Unit();
	cPrev = cCurr - vPrev * R;
	cNext = cCurr + vNext * R;

	AddElement(cPrev, P_TYPE_LINE, ++nIdx);
	AddElement(cNext, IsClockWise() ? -R : R, ++nIdx);
	return nIdx;
}

long CPlatePieceObject::InsertChamfer(long nIdx)
{
	double D = GetRadius(nIdx);
	CDPoint cCurr = GetPoint(nIdx);
	CDPoint cPrev = GetPoint(GetPrevIdx(nIdx));
	CDPoint cNext = GetPoint(GetNextIdx(nIdx));

	CDPoint vPrev = (cCurr - cPrev).Unit();
	CDPoint vNext = (cNext - cCurr).Unit();
	cPrev = cCurr - vPrev * D;
	cNext = cCurr + vNext * D;

	AddElement(cPrev, P_TYPE_LINE, ++nIdx);
	AddElement(cNext, P_TYPE_LINE, ++nIdx);
	return nIdx;
}

long CPlatePieceObject::InsertFillet(long nIdx, P_TYPE nType)
{
	CDPoint cCurr = GetPoint(nIdx);
	CDPoint cPrev = GetPoint(GetPrevIdx(nIdx,nType));
	CDPoint cNext = GetPoint(GetNextIdx(nIdx,nType));
	double R = GetRadius(nIdx);
	if(cCurr == cPrev || cPrev == cNext || cCurr == cNext) return GetObjSize();
	double Dis = GetDisFillet(cCurr, cPrev, cNext, R);

	CDPoint vPrev = (cCurr - cPrev).Unit();
	CDPoint vNext = (cNext - cCurr).Unit();
	cPrev = cCurr - vPrev * Dis;
	cNext = cCurr + vNext * Dis;

	long nPrevIdx = GetPrevIdx(nIdx,nType);
	while (~(GetPoint(nPrevIdx) - cCurr) < Dis)
	{
		GetPointObj(nPrevIdx)->SetType(P_TYPE_DIMPOINT);
		nPrevIdx = GetPrevIdx(nPrevIdx,nType);
	}
	long nNextIdx = GetNextIdx(nIdx,nType);
	while (~(GetPoint(nNextIdx) - cCurr) < Dis)
	{
		GetPointObj(nNextIdx)->SetType(P_TYPE_DIMPOINT);
		nNextIdx = GetNextIdx(nNextIdx,nType);
	}
	AddElement(cPrev, P_TYPE_LINE, ++nIdx);
	AddElement(cNext, -GetDirectionRotateByPoint(cPrev, cCurr, cNext) * R, ++nIdx);
	return nIdx;
}

long CPlatePieceObject::InsertFilletAt(long nIdxPrev, long nIdx, long nIdxNext, P_TYPE nType)
{
	CDPoint cCurr = GetPoint(nIdx);
	CDPoint cPrev = GetPoint(nIdxPrev);
	CDPoint cNext = GetPoint(nIdxNext);
	double R = GetRadius(nIdx);
	double Dis = GetDisFillet(cCurr, cPrev, cNext, R);

	CDPoint vPrev = (cCurr - cPrev).Unit();
	CDPoint vNext = (cNext - cCurr).Unit();
	cPrev = cCurr - vPrev * Dis;
	cNext = cCurr + vNext * Dis;

	long nPrevIdx = GetPrevIdx(nIdx,nType);
	while (~(GetPoint(nPrevIdx) - cCurr) < Dis)
	{
		GetPointObj(nPrevIdx)->SetType(P_TYPE_DIMPOINT);
		nPrevIdx = GetPrevIdx(nPrevIdx,nType);
	}
	long nNextIdx = GetNextIdx(nIdx,nType);
	while (~(GetPoint(nNextIdx) - cCurr) < Dis)
	{
		GetPointObj(nNextIdx)->SetType(P_TYPE_DIMPOINT);
		nNextIdx = GetNextIdx(nNextIdx,nType);
	}
	AddElement(cPrev, P_TYPE_LINE, ++nIdx);
	AddElement(cNext, -GetDirectionRotateByPoint(cPrev, cCurr, cNext) * R, ++nIdx);
	return nIdx;
}

// 다이아프램 or 단면 상부에 테이퍼 등 꺾임 부분 추가.
long CPlatePieceObject::InsertBreak(long nIdx)
{
	CDPoint vPrev = GetPoint(GetPrevIdx(nIdx, P_TYPE_OUTLINE)) - GetPoint(nIdx);
	CDPoint vNext = GetPoint(GetNextIdx(nIdx, P_TYPE_OUTLINE)) - GetPoint(nIdx);

	if (double(vPrev ^ vNext) < 0)   {  // 리브 스켈롭 구간 밖에 있음
		SetPTType(nIdx, GetPTType(nIdx) & ~P_TYPE_BREAK | P_TYPE_LINE);
		nIdx++;
	}
	else   {  // 스켈롭 구간 안에 있음
		BOOL bPrev = ~vPrev > ~vNext;  // 리브 스켈롭 이전, 이후 Index 여부
		CDPoint xyRib1 = GetPoint(bPrev ? (nIdx + 2) : (nIdx - 2));
		CDPoint xyRib2 = GetPoint(bPrev ? (nIdx + 3) : (nIdx - 1));
		CDPoint xyBreak1 = GetPoint(nIdx);
		CDPoint xyBreak2 = GetPoint(bPrev ? (nIdx - 1) : (nIdx + 1));
		CDPoint xyMove;
		long nMoveIdx = bPrev ? (nIdx + 2) : (nIdx - 1);
		long nTypeIdx = bPrev ? (nMoveIdx + 1) : nMoveIdx;

		if (GetPTType(nTypeIdx) & P_TYPE_LINE)   {  // 수정 대상이 Line이면
			GetXyMatchSegAndSeg(xyBreak1, xyBreak2, xyRib1, xyRib2, xyMove);
		}
		else   {  // 수정 대상이 Arc면
			double bulge = GetRadius(nTypeIdx);
			CDPoint cen = GetCenterArc(xyRib1, xyRib2, bulge), temp;
			double R = ~(cen - xyRib1);
			GetXyMatchSegAndCircle(xyBreak1, xyBreak2, cen, R, xyMove, temp);
			bulge = GetArcBulge(xyMove, bPrev ? xyRib2 : xyRib1, R) * (bulge > 0 ? 1 : -1);
			m_TPAPoint[GetUseConvData()].GetAt(nTypeIdx).SetLength(bulge);
		}
		SetPoint(nMoveIdx, xyMove);
		CPlatePiecePointArray& rArr = m_TPAPoint[GetUseConvData()];
		rArr.RemoveAt(nIdx);
	}
	return nIdx;
}

long CPlatePieceObject::InsertChamferAtArc(long nIdx, double D)
{
	CVector cPrev = GetSetBackPoint(nIdx, TRUE, D);
	CVector cNext = GetSetBackPoint(nIdx, FALSE, D);
	P_TYPE Type = GetPTType(nIdx);

	SetPTType(nIdx, Type & ~P_TYPE_OUTLINE | P_TYPE_CHAMFER);
	m_TPAPoint[GetUseConvData()].GetAt(nIdx).SetLength(D);
	AddElement(cPrev, Type & P_TYPE_OUTLINE, ++nIdx, NULL, cPrev.z);
	AddElement(cNext, P_TYPE_LINE, ++nIdx);
	m_TPAPoint[GetUseConvData()].GetAt(GetNextIdx(nIdx, P_TYPE_OUTLINE)).SetLength(cNext.z);
	return nIdx;
}

long CPlatePieceObject::InsertScallopAtArc(long nIdx, double R)
{
	CVector cPrev = GetSetBackPoint(nIdx, TRUE, R);
	CVector cNext = GetSetBackPoint(nIdx, FALSE, R);
	P_TYPE Type = GetPTType(nIdx);

	SetPTType(nIdx, Type & ~P_TYPE_OUTLINE | P_TYPE_SCALLOP);
	m_TPAPoint[GetUseConvData()].GetAt(nIdx).SetLength(R);
	AddElement(cPrev, Type & P_TYPE_OUTLINE, ++nIdx, NULL, cPrev.z);
	AddElement(cNext, IsClockWise() ? -R : R, ++nIdx);
	m_TPAPoint[GetUseConvData()].GetAt(GetNextIdx(nIdx, P_TYPE_OUTLINE)).SetLength(cNext.z);
	return nIdx;
}


long CPlatePieceObject::InsertSpliceScallopC(long nIdx)
{
/*	CDPoint vPrev = (GetPoint(nIdx) - GetPoint(GetPrevIdx(nIdx))).Unit();
	CDPoint vNext = (GetPoint(GetNextIdx(nIdx)) - GetPoint(nIdx)).Unit();
	BOOL bUpper   = vPrev.x > 0;
	BOOL bMirror  = m_pGir->m_pSangse->m_ScallopRight;
	BOOL bGeneral = !m_pGir->m_pSangse->m_ScallopType;

	double dWidth = 100;
	double dThick = 15;

	CDPoint vX = bUpper ? (vPrev+vNext)/2 : -(vPrev+vNext)/2;
	CDPoint vY = vX.RotateInv90();

	long nBaseType = m_pGir->m_pSangse->m_ScallopBasePoint_Compression;
	if (!bUpper ^ bMirror)   {
		CDPoint temp = vPrev;
		vPrev = -vNext;
		vNext = -temp;
	}

	if (!bUpper)
		vY = -vY;
	if (bMirror)
		vX = -vX;

	double R1 = GetRadius(nIdx);
	double R2 = 20;//m_pGir->m_pSangse->m_Scallop_JRib_R1;
	double D1 = 20;//m_pGir->m_pSangse->m_Scallop_JRib_D1;
	double d1 = dThick / 2 + 1.0;
	double d2 = nBaseType == 0 ? 0 : (dThick / 2);
	double d3 = nBaseType == 1 ? -d2 : d2;
	double d4 = dThick / 2 + D1;

	if (!bGeneral)
		d2 = d3 = 0;

	CDPoint Base = GetPoint(nIdx);
	double slopPrev = fabs(1 / vPrev.x);
	double slopNext = fabs(1 / vNext.x);
	CVector xy[10];
	long nVtx = 0;

	xy[nVtx] = Base - vPrev * (d1 * slopPrev + R1);
	xy[nVtx++].z = 0;
	xy[nVtx] = Base - vPrev * (d1 * slopPrev) + vY * R1;
	xy[nVtx++].z = -R1;
	xy[nVtx] = Base - vX * d1 + vY * (dWidth - sqrt(R2*R2 - (d1-d2)*(d1-d2)));
	xy[nVtx++].z = 0;
	xy[nVtx] = Base - vX * d2 + vY * (dWidth + R2);
	xy[nVtx++].z = -R2;
	if (d2 + d3 != 0)   {
		xy[nVtx] = Base + vX * d3 + vY * (dWidth + R2);
		xy[nVtx++].z = 0;
	}
	if (bGeneral)   {
		xy[nVtx] = Base + vX * (d3 + R2) + vY * dWidth;
		xy[nVtx++].z = -R2;
		xy[nVtx] = Base + vNext * (d4 * slopNext);
		xy[nVtx++].z = 0;
	}
	else   {
		xy[nVtx] = Base + vX * d1 + vY * (dWidth - sqrt(R2*R2 - (d1-d2)*(d1-d2)));
		xy[nVtx++].z = -R2;
		xy[nVtx] = Base + vNext * (d1 * slopNext) + vY * R1;
		xy[nVtx++].z = 0;
		xy[nVtx] = Base + vNext * (d1 * slopNext + R1);
		xy[nVtx++].z = -R1;
	}

	AddElement(xy[nVtx-1], P_TYPE_LINE, ++nIdx);
	for (long n = nVtx-2; n >= 0; n--)   {
		P_TYPE Dim = (n == 1) ? P_TYPE_DIMPOINT : 0;
		if (xy[n+1].z == 0)
			AddElement(xy[n], P_TYPE_LINE | Dim, ++nIdx);
		else
			AddElement(xy[n], xy[n+1].z, ++nIdx, NULL, P_TYPE_ARC | Dim);
	}

	return nIdx;*/
	return 0;
}

long CPlatePieceObject::InsertSpliceScallopT(long nIdx)
{
/*	CDPoint vPrev = (GetPoint(nIdx) - GetPoint(GetPrevIdx(nIdx))).Unit();
	CDPoint vNext = (GetPoint(GetNextIdx(nIdx)) - GetPoint(nIdx)).Unit();

	CDPoint vX = (vPrev+vNext)/2;
	CDPoint vY = vX.RotateInv90();

	double dWidth = 100;
	double dThick = 15;
	double A = 0;
	double R = 0;
	if(ribT<=12)
	{
		A = pBridge->m_dSlotA[0];
		R = pBridge->m_dSlotR2[0];
	}
	else if(ribT>12 && ribT<=22)
	{
		A = pBridge->m_dSlotA[1];
		R = pBridge->m_dSlotR2[1];
	}
	else
	{
		A = pBridge->m_dSlotA[2];
		R = pBridge->m_dSlotR2[2];
	}
	double scallopW = (A-ribT)/2;
	double T = pGir->m_pSangse->m_ScallopBasePoint_Tension ? ribT/2: 0;

	double R  = 20;//m_pGir->m_pSangse->m_Scallop_JRib_R2;
	double D1 = 20;//m_pGir->m_pSangse->m_Scallop_JRib_D2;
	double d1 = dThick / 2 + D1;
	double d2 = nBaseType == 0 ? 0 : (dThick / 2);

	CDPoint Base = GetPoint(nIdx);
	double slopPrev = fabs(1 / vPrev.x);
	double slopNext = fabs(1 / vNext.x);
	CDPoint xy[8];
	long nVtx = 0;

	xy[nVtx] = Base - vPrev * (d1 * slopPrev);
	xy[nVtx] = Base - vX * (d2 + R) + vY * dWidth;
	xy[nVtx] = Base - vX * d2 + vY * (dWidth + R);
	if (d2 != 0)   {
		xy[nVtx] = Base + vX * d2 + vY * (dWidth + R);
	}
	xy[nVtx] = Base + vX * (d2 + R) + vY * dWidth;
	xy[nVtx] = Base + vNext * (d1 * slopNext);

	AddElement(xy[nVtx-1], P_TYPE_LINE, ++nIdx);
	for (long n = nVtx-2; n >= 0; n--)   {
		if (xy[n+1].z == 0)
			AddElement(xy[n], P_TYPE_LINE, ++nIdx);
		else
			AddElement(xy[n], xy[n+1].z, ++nIdx);
	}

	return nIdx;*/
	return 0;
}


// 한 Vertex를 인접 Vertex 방향으로 이동시킴.
// len 부호 : 다음점 향하는 방향 +.
// return의 Z값은 바뀐 bulge값임. ( 0 : 직선 )
CVector CPlatePieceObject::GetSetBackPoint(long nIdx, BOOL bPrev, double len)
{
	CDPoint cNext, cCurr, vDir;
	CVector Move;
	long nObjIdx = bPrev ? nIdx : GetNextIdx(nIdx, P_TYPE_OUTLINE);
	long nNextIdx = bPrev ? GetPrevIdx(nIdx, P_TYPE_OUTLINE) : nObjIdx;
	cCurr = GetPoint(nIdx);
	cNext = GetPoint(nNextIdx);

	if (GetPTType(nObjIdx) & P_TYPE_LINE)   {  // Line
		vDir = (cNext - cCurr).Unit();
		Move = cCurr + vDir * len;
		Move.z = 0;
	}
	else   {  // Arc
		long nDir = bPrev ? -1 : 1;
		double bulge = GetRadius(nObjIdx);
		CDPoint cen = GetCenterArc(cCurr, cNext, bulge * nDir);
		double R = ~(cCurr - cen);
		double ang = len / R;
		ang = (bulge * nDir) > 0 ? -ang : ang;
		Move = GetXyRotateRadian(cen, cCurr, ang);
		Move.z = GetArcBulge(Move, cNext, R) * (bulge > 0 ? 1 : -1);
	}
	return Move;
}

void CPlatePieceObject::SetBack(long nIdx, BOOL bPrev, double len)
{
	CVector Move = GetSetBackPoint(nIdx, bPrev, len);
	SetPoint(nIdx, Move);
	m_TPAPoint[GetUseConvData()].GetAt(nIdx).SetLength(Move.z);
}

double CPlatePieceObject::GetLength(long nIdx)
{
	double dLength = 0;
	if(GetObjSize() < 1) return 0;

	for(long n=nIdx;n<GetObjSize()-1;n++)
	{
		if(!IsContinue(n+1)) break;
		dLength += ~(GetPoint(n+1) - GetPoint(n));		
	}
	return dLength;
}

double CPlatePieceObject::GetLength(long nIdxStt, long nIdxEnd)
{
	double dLength = 0;
	if(GetObjSize() < 1) return 0;

	for(long n=nIdxStt;n<nIdxEnd-1;n++)
	{
		if(!IsContinue(n+1)) break;
		dLength += ~(GetPoint(n+1) - GetPoint(n));		
	}
	return dLength;
}

void CPlatePieceObject::RotateByPoint(CDPoint PoStt, CDPoint PoEnd)
{
	CDPoint vVec = (PoEnd - PoStt).Unit().GetMirrorVert();

	for(long n=0;n<GetObjSize();n++)
	{
		SetPoint(n,GetXyRotateVector(PoStt,GetPoint(n),vVec));
	}
	for(long i=0;i<GetChildCount();i++)	
	{
		GetChild(i)->RotateByPoint(PoStt,PoEnd);	
	}
}

void CPlatePieceObject::RotateByFace(long nFace)
{
	CDPoint SttPoint = GetSttFacePoint(nFace);
	CDPoint EndPoint = GetEndFacePoint(nFace);

	CDPoint vVec = (EndPoint - SttPoint).Unit().GetMirrorVert();

	for(long n=0;n<GetObjSize();n++)
	{
		SetPoint(n,GetXyRotateVector(SttPoint,GetPoint(n),vVec));
	}

	for(long i=0;i<GetChildCount();i++)	
	{
		GetChild(i)->RotateByPoint(SttPoint,EndPoint);	
	}
}

// Scallop과 Chamfer에 기준선 그리기
void CPlatePieceObject::DrawChamferCenterLine(CDomyun *pDom, long nIdx, double dLenRatio)
{
	CDPoint stt, end;
	for(long n = 0; n < GetObjSize(); n++)   {
		if ((nIdx == -1 || nIdx == n))   {
			if (GetPTType(n) & (P_TYPE_SCALLOP | P_TYPE_CHAMFER))   {
				stt = GetPoint(n);
				end = GetPoint(n+1);
				pDom->LineTo(stt, stt+(end-stt)*dLenRatio);
				end = GetPoint(n+2);
				pDom->LineTo(stt, stt+(end-stt)*dLenRatio);
			}
		}
	}
}

//
void CPlatePieceObject::DrawPieceOutline(CDomyun *pDom, CARoadOptionStd *pStd, BOOL bFillCircle)
{
	if(GetObjSize()==0) return;
	if(m_nLayer!=-1) pStd->SetEnvType(pDom,m_nLayer);
	if(IsMarkLine())
	{
		double MarkLen = pDom->GetMarkMaxLen();
		double dLen = GetDistance(0,GetObjSize());
		if(MarkLen > dLen && dLen!=0)	
			pDom->SetMarkMaxLen(dLen/2 + 100);
					
		CDPoint xy, xyPre;
		long nIdx = 0;
		long nSttIdx = nIdx;
		xyPre = GetPoint(nIdx);
		pDom->MoveToPiece(xyPre);		
		nIdx = GetNextIdx(nIdx, P_TYPE_PLATE);		
		while(nIdx != nSttIdx)
		{			
			
			xy = GetPoint(nIdx);

			if (IsPTType(nIdx,P_TYPE_LINE))   {
				if (IsContinue(nIdx))				
				{
					if(IsPTType(nIdx,P_TYPE_STTMARK))					
						pDom->LineToPieceEx(xy,IsLeft(),m_nColor);						
					else
						pDom->LineToPiece(xy,IsLeft(),m_nColor);				
				}
				xyPre = xy;
			}
			nIdx = GetNextIdx(nIdx, P_TYPE_PLATE);			
		}		

		pDom->SetMarkMaxLen(MarkLen);
	}
	else
	{
		CDPoint xy, xyPre;
		if(GetVertexSu(P_TYPE_PLATE)==0) return;
		long nIdx = GetNextIdx(0, P_TYPE_PLATE);
		long nSttIdx = nIdx;
		xyPre = GetPoint(nIdx);
		do  {
			nIdx = GetNextIdx(nIdx, P_TYPE_PLATE);
			xy = GetPoint(nIdx);
			if (GetPTType(nIdx) & P_TYPE_LINE)   {
				if (GetPTType(nIdx) & P_TYPE_SKIP)   {
					pDom->MoveTo(xyPre);
					xyPre = xy;							
				}	
				else if (IsContinue(GetPrevIdx(nIdx, P_TYPE_OUTLINE)))   {
					pDom->MoveTo(xyPre);
					pDom->LineTo(xy,m_nColor);
				}
				xyPre = xy;
			}
			if (GetPTType(nIdx) & P_TYPE_ARC)   {
				if (IsContinue(GetPrevIdx(nIdx, P_TYPE_OUTLINE)))   {
					double len = GetRadius(nIdx);
					CDPoint mid = (xy + xyPre) / 2 + (xy - xyPre).Unit().Rotate90() * len;
					pDom->Arc(xyPre.x, xyPre.y, mid.x, mid.y, xy.x, xy.y);
				}
				xyPre = xy;
			}		
			if (GetPTType(nIdx) & P_TYPE_CIRCLE)   {
				double R = GetRadius(nIdx);				
				pDom->Circle(xy,R,bFillCircle);								
			}

			// 현재 우리가 1000이상의 홀은 비정상적으로 볼때.... 무한루프 방지
			if(nIdx>1000) break;
		}
		while(nIdx != nSttIdx); 
		
/*		pDom->MoveTo(GetPoint(0));
		for(long n=1; n<GetObjSize(); n++)
		{
			if(GetPTType(n) & P_TYPE_LINE)
			{			
				if(IsContinue(n))
					pDom->LineTo(GetPoint(n));		
				else
					pDom->MoveTo(GetPoint(n));		
			}
		}
		pDom->LineTo(GetPoint(0));	
*/	}

	for(long n=0;n<GetChildCount();n++)
		GetChild(n)->DrawPieceOutline(pDom,pStd,bFillCircle);
}

//
void CPlatePieceObject::DrawPieceSectionMark(CDomyun *pDom, long nFace, CARoadOptionStd *pStd)
{
	if(GetObjSize()==0) return;

	long nStt = GetSttFace(nFace);
	long nEnd = GetEndFace(nFace);
	long nMatchFace = nFace + 2;
	
	pStd->SetEnvType(pDom,HCAD_STLC);	

	if(nMatchFace > 3) nMatchFace -= 4;

	long nSttMatch = GetSttFace(nMatchFace);
	long nEndMatch = GetEndFace(nMatchFace);
		
	for(long n1=nStt,n2=nEndMatch;n1<=nEnd && n2>=nSttMatch;)
	{
		if(!IsPTType(n1,P_TYPE_FLANGEMARK)) 
		{
			n1++;
			continue;		
		}
		if(!IsPTType(n2,P_TYPE_FLANGEMARK)) 
		{
			n2--;
			continue;		
		}
		CDPoint SttPoint = GetPoint(n1);		
		CDPoint EndPoint = GetPoint(n2);		
		double MarkLen = pDom->GetMarkMaxLen();
		double dLen = ~(SttPoint - EndPoint);
		if(MarkLen > dLen && dLen!=0)	
		pDom->SetMarkMaxLen(dLen/2 + 100);
		pDom->MoveToPiece(SttPoint);
		pDom->LineToPiece(EndPoint,TRUE,m_nColor);

		n1++;
		n2--;
	}			
}

void CPlatePieceObject::DrawPieceFace(CDomyun *pDom,long nFace, CARoadOptionStd *pStd)
{
	if(GetObjSize()==0) return;
	long nFirst = GetSttFace(nFace);
	long nNext  = GetEndFace(nFace);
	long nMax = (nNext == 0) ? GetObjSize() - 1 : nNext;

	if(nFirst < 0 || nNext < 0) return;
	if(m_nLayer!=-1)	pStd->SetEnvType(pDom,m_nLayer);
	if(IsMarkLine())
	{		
		double MarkLen = pDom->GetMarkMaxLen();
		double dLen = GetLength(nFirst,nMax);
		if(MarkLen > dLen && dLen!=0)	pDom->SetMarkMaxLen(dLen/2 + 100);
		pDom->MoveToPiece(GetPoint(nFirst));
		for(long n=nFirst+1; n<=nMax; n++)
		{
			if(IsContinue(n))
				pDom->LineToPiece(GetPoint(n),IsLeft(),m_nColor);		
			else
			{					
				dLen = GetLength(n,GetUpperBound());
				if(MarkLen > dLen && dLen!=0)	pDom->SetMarkMaxLen(dLen/2 + 100);
				pDom->MoveToPiece(GetPoint(n));	
			}
		}			
		pDom->SetMarkMaxLen(MarkLen);
	}
	else
	{
		pDom->MoveTo(GetPoint(nFirst));
		for(long n=nFirst+1; n<=nMax; n++)
		{
			if(IsContinue(n))
				pDom->LineTo(GetPoint(n),m_nColor);		
			else
				pDom->MoveTo(GetPoint(n));
		}	

		if(nNext == 0) pDom->LineTo(GetPoint(0),m_nColor);
	}
	for(long n=0;n<GetChildCount();n++)
		GetChild(n)->DrawPieceFace(pDom,nFace,pStd);	
}

CDPoint CPlatePieceObject::GetRectSize(CDPoint vDir, BOOL bAutoDir)
{
	CDPointArray pArr;

	if(GetObjSize()==0)	return CDPoint(0,0);

	for(long i=0; i<GetObjSize(); i++)
	{
		if (GetPTType(i) & P_TYPE_PLATE)
			pArr.Add(GetPoint(i));
	}

	return GetRectangleFromVertex(pArr,TRUE,4,bAutoDir,vDir); 
}

void CPlatePieceObject::SetFlangeBx(BOOL bUpper)
{
	/*
	CPlateBxFinder Finder(m_pGir);	
	CPlateBasicIndex *pBx;
	__int64 BxFlag;
	long nStt;
	double DL = m_pGir->m_dWidth/2 + (bUpper ? m_pGir->m_dULeftSharp  : m_pGir->m_dLeftSharp);
	double DR = m_pGir->m_dWidth/2 + (bUpper ? m_pGir->m_dURightSharp : m_pGir->m_dRightSharp);
	__int64 BxCross = bUpper ? (BX_UPPERFLANGE | BX_BENDING) : (BX_LOWERFLANGE | BX_BYUN | BX_BENDING);

	nStt = GetSttFace(0);	
	BxFlag =  bUpper ? BX_UPPER_CENTER : BX_LOWER_CENTER | BX_MANHOLE | BX_WATEROUT | BX_STT_BYUN_LOWER | BX_END_BYUN_LOWER;
	pBx = Finder.GetBxFirst(BxFlag,m_pBxStt, m_pBxEnd);
	while(pBx)
	{
		if(!m_pGir->GetCrossBxSu(pBx, DR, BxCross))
			SetBx(nStt,pBx->GetBxRight());			
		pBx = Finder.GetBxNext();
		nStt++;
	}	

	nStt = GetEndFace(2);
	BxFlag =  bUpper ? BX_UPPER_CENTER : BX_LOWER_CENTER | BX_MANHOLE | BX_WATEROUT | BX_STT_BYUN_LOWER | BX_END_BYUN_LOWER;
	pBx = Finder.GetBxFirst(BxFlag,m_pBxStt, m_pBxEnd);
	while(pBx)
	{
		if(!m_pGir->GetCrossBxSu(pBx, DL, BxCross))
			SetBx(nStt,pBx->GetBxLeft());			
		pBx = Finder.GetBxNext();
		nStt--;
	}	
	*/
}

void CPlatePieceObject::SetWebBx( __int64 BxFlag)
{
	CPlateBxFinder Finder(m_pGir);	
	long nStt = GetSttFace(0);	
	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,m_pBxStt, m_pBxEnd);
	TRACE("SetWebBx Start \n");
	while(pBx)
	{
		SetBx(nStt,pBx);
		SetBx(GetMatchIndex(2,nStt),pBx);
		TRACE("%.6f\n",pBx->GetStation());
		pBx = Finder.GetBxNext();
		nStt++;
		if(nStt == GetObjSize()/2) break;
	}	
}

void CPlatePieceObject::InsertFlangeAddSize(long nIdx, double dSizeU, double dSizeL, BOOL bLeft)
{
	long nStt = GetSttFace(0);
	long nEnd = GetEndFace(0);
	
	if(bLeft)
	{
		CDPoint LVec = (GetPoint(nIdx-1) - GetPoint(nIdx)).Unit();		
		CDPoint UVec = (GetPoint(GetMatchIndex(2,nIdx-1)) - GetPoint(GetMatchIndex(2,nIdx))).Unit();
		for(long n=nIdx-1;n>=nStt;n--)
		{				      
			CDPoint PrevPL = GetPoint(n) + LVec * dSizeL;			
			CDPoint PrevPU = GetPoint(GetMatchIndex(2,n)) + UVec * dSizeU;			
			SetPoint(n,PrevPL);
			SetPoint(GetMatchIndex(2,n),PrevPU);
		}				
	}
	else
	{
		CDPoint LVec = (GetPoint(nIdx+1) - GetPoint(nIdx)).Unit();				
		CDPoint UVec = (GetPoint(GetMatchIndex(2,nIdx+1)) - GetPoint(GetMatchIndex(2,nIdx))).Unit();
		for(long n=nIdx+1;n<=nEnd;n++)
		{				      
			CDPoint NextPL = GetPoint(n) + LVec * dSizeL;			
			CDPoint NextPU = GetPoint(GetMatchIndex(2,n)) + UVec * dSizeU;			
			SetPoint(n,NextPL);
			SetPoint(GetMatchIndex(2,n),NextPU);
		}		
	}
}

void CPlatePieceObject::InsertWebAddSize(long nIdx, double dSizeU, double dSizeL, BOOL bLeft)
{
	long nStt = GetSttFace(0);
	long nEnd = GetEndFace(0);
	
	if(bLeft)
	{
		for(long n=nIdx-1;n>=nStt;n--)
		{				      
			CDPoint PrevPL = GetPoint(n) - CDPoint(dSizeL, 0);			
			CDPoint PrevPU = GetPoint(GetMatchIndex(2,n)) - CDPoint(dSizeU, 0);			
			SetPoint(n,PrevPL);
			SetPoint(GetMatchIndex(2,n),PrevPU);
		}				
	}
	else
	{
		for(long n=nIdx+1;n<=nEnd;n++)
		{				      
			CDPoint NextPL = GetPoint(n) + CDPoint(dSizeL, 0);			
			CDPoint NextPU = GetPoint(GetMatchIndex(2,n)) + CDPoint(dSizeU, 0);			
			SetPoint(n,NextPL);
			SetPoint(GetMatchIndex(2,n),NextPU);
		}		
	}
}

void CPlatePieceObject::ResizeFlange(BOOL bUpper)
{
	ASSERT(m_pPieceData != NULL);	

	m_TPAPoint[1] = m_TPAPoint[0];
	m_TPAFaceStt[1].Copy(m_TPAFaceStt[0]);
	m_TPAFaceEnd[1].Copy(m_TPAFaceEnd[0]);
	SetUseConvData(TRUE);

	long nStt = GetSttFace(0);
	long nEnd = GetEndFace(0);	
	long n	  = 0;
	// 종방향 증가
	for(n = nStt;n < nEnd; n++)	
	{			
		if(!IsPTType(n,P_TYPE_PLATE)) continue;
		double dMoveXU = GetFlangeLMove(GetMatchIndex(2,n), GetMatchIndex(2,GetNextIdx(n,P_TYPE_PLATE)),bUpper);		
		double dMoveXL = GetFlangeLMove(n, GetNextIdx(n,P_TYPE_PLATE),bUpper);		
		//InsertFlangeAddSize(n,-dMoveXU/2,-dMoveXL/2,FALSE);
		InsertFlangeAddSize(n,dMoveXU,dMoveXL,FALSE);
	}

	// 횡방향 증가
	for(n=nStt;n<=nEnd;n++)
	{
		if(!IsPTType(n,P_TYPE_PLATE)) continue;
		CDPoint VerVec;
		if(GetBx(n) && GetBx(n)->GetAngle() != CDPoint(0,1))
		{
			if(n==nEnd)
				VerVec = (GetPoint(n) - GetPoint(n-1)).Unit();
			else
				VerVec = (GetPoint(n+1) - GetPoint(n)).Unit();							
		}
		else
			VerVec = (GetPoint(GetMatchIndex(2,n)) - GetPoint(n)).Unit();
		//
		double dMoveY = GetFlangeHMove(n,bUpper);		
		CDPoint NextP = GetPoint(GetMatchIndex(2,n)) + VerVec * (dMoveY / 2.0);			
		SetPoint(GetMatchIndex(2,n),NextP);		

		VerVec.MirrorVert();
		NextP = GetPoint(n) + VerVec * (dMoveY / 2.0);			
		SetPoint(n,NextP);		

	}	
}

void CPlatePieceObject::ResizeWeb(BOOL bLeft)
{
	ASSERT(m_pPieceData != NULL);	

	m_TPAPoint[1] = m_TPAPoint[0];
	m_TPAFaceStt[1].Copy(m_TPAFaceStt[0]);
	m_TPAFaceEnd[1].Copy(m_TPAFaceEnd[0]);
	SetUseConvData(TRUE);

	long nStt = GetSttFace(0);
	long nEnd = GetEndFace(0);	

	// 종방향 증가
	for(long n = nStt;n < nEnd; n++)	
	{			
		if(!IsPTType(n,P_TYPE_PLATE)) continue;
		double dMoveXU = GetWebLMove(n, GetNextIdx(n,P_TYPE_PLATE),bLeft,TRUE);		
		double dMoveXL = GetWebLMove(n, GetNextIdx(n,P_TYPE_PLATE),bLeft,FALSE);		
		InsertWebAddSize(n,dMoveXU,dMoveXL,FALSE);
	}
}

long CPlatePieceObject::GetFace(long nIdx, BOOL bEndInclude)
{
	for(long n=0;n<m_nFaceSize;n++)
	{
		long nStt = GetSttFace(n);
		long nEnd = GetEndFace(n);
		if(bEndInclude)
		{
			if (nStt < nEnd)
				if(nIdx >= nStt && nIdx <= nEnd) return n;
			else
				if(nIdx >= nStt || nIdx <= nEnd) return n;
		}
		else
		{
			if (nStt < nEnd)
				if(nIdx >= nStt && nIdx < nEnd) return n;
			else
				if(nIdx >= nStt || nIdx < nEnd) return n;
		}
	}
	return -1;
}

BOOL CPlatePieceObject::IsPTType(long nIdx, P_TYPE type)
{ 
	P_TYPE P_Value = GetPTType(nIdx) & type; 
	if(P_Value) return TRUE;
	return FALSE;
}

long CPlatePieceObject::GetObjSizeByPType(P_TYPE type)
{ 
	long nSize = 0, nTotal = GetObjSize();
	for(long n=0;n<nTotal;n++)
	{
		if(IsPTType(n,type)) nSize++;
	}		
	return nSize;
}
	
double CPlatePieceObject::GetDistance(long nIdxStt, long nIdxEnd, BOOL bOrg)
{
	double nDist = 0;
	long nIdx1 = min(nIdxStt, nIdxEnd);
	long nIdx2 = max(nIdxStt, nIdxEnd);
	for(long n=nIdx1; n<nIdx2; n++)
	{
		if(bOrg)
			nDist += ~(m_TPAPoint[0].GetAt(GetNextIdx(n)).GetPoint() - m_TPAPoint[0].GetAt(n).GetPoint());
		else
			nDist += ~(GetPoint(GetNextIdx(n)) - GetPoint(n));
	}
	return nDist;
}

double CPlatePieceObject::GetDistanceHor(long nIdxStt, long nIdxEnd, BOOL bOrg)
{
	double nDist = 0;
	long nIdx1 = min(nIdxStt, nIdxEnd);
	long nIdx2 = max(nIdxStt, nIdxEnd);
	for(long n=nIdx1; n<nIdx2; n++)
	{		
		if(bOrg)
			nDist += fabs(m_TPAPoint[0].GetAt(GetNextIdx(n)).GetPoint().x - m_TPAPoint[0].GetAt(n).GetPoint().x);
		else
			nDist += fabs(GetPoint(GetNextIdx(n)).x - GetPoint(n).x);
	}
	return nDist;
}

long CPlatePieceObject::GetMatchIndex(long nMatchFace, long nIdx, BOOL bEndInclude) 
{ 
	long nEndIdx = GetEndFace(nMatchFace);	
	long nRetIdx;

	nEndIdx = nEndIdx == 0 ? GetObjSize() : nEndIdx;	
	nRetIdx = nEndIdx - (nIdx - GetSttFace(GetFace(nIdx,bEndInclude))); 	
	return nRetIdx == GetObjSize() ? 0 : nRetIdx;
}

long CPlatePieceObject::GetMatchIndexByBx(long nIdx) 
{
	CPlateBasicIndex *pCurBxCen = GetBx(nIdx);	
	long nNextIdx = GetNextIdx(nIdx);
	
	while(nNextIdx != nIdx)
	{
		if(GetBx(nNextIdx) && pCurBxCen == GetBx(nNextIdx)) return nNextIdx;
		nNextIdx = GetNextIdx(nNextIdx);
	}
	ASSERT(FALSE);
	return -1;
}

long CPlatePieceObject::GetPFirst(P_TYPE pType, long nSttIdx, long nEndIdx)
{
	m_CurPType = pType;
	m_bAsc = TRUE;
	m_nMinIdx = nSttIdx;
	m_nMaxIdx = nEndIdx;
	m_nCurIdx = nSttIdx;

	if (m_CurPType == 0 || m_CurPType & GetPTType(nSttIdx)) return nSttIdx;

	return GetPNext();
}

long CPlatePieceObject::GetPLast(P_TYPE pType, long nSttIdx, long nEndIdx)
{
	m_CurPType = pType;
	m_bAsc = FALSE;
	m_nMinIdx = nSttIdx;
	m_nMaxIdx = nEndIdx;
	m_nCurIdx = nEndIdx;

	if (m_CurPType == 0 || m_CurPType & GetPTType(nEndIdx)) return nEndIdx;

	return GetPNext();
}

long CPlatePieceObject::GetPFixNext()
{
	return m_bAsc ? GetNextOne() : GetPrevOne();
}

long CPlatePieceObject::GetPFixPrev()
{
	return m_bAsc ? GetPrevOne() : GetNextOne();	
}

long CPlatePieceObject::GetPrevOne()
{
	if(m_nCurIdx == -1) return -1;
	long nIdx = m_nCurIdx;	
	while(1)
	{		
		if (--nIdx < 0) nIdx = GetObjSize() - 1;
		if(nIdx < m_nMinIdx || nIdx > m_nMaxIdx) break;
		if (m_CurPType == 0 || m_CurPType & GetPTType(nIdx)) return nIdx;				
		if(nIdx == m_nCurIdx) break;
	}
	return -1;
}

long CPlatePieceObject::GetNextOne()
{
	if(m_nCurIdx == -1) return -1;
	long nIdx = m_nCurIdx;		
	while(1)
	{		
		if (++nIdx >= GetObjSize()) nIdx = 0;
		if(nIdx < m_nMinIdx || nIdx > m_nMaxIdx) break;
		if (m_CurPType == 0 || m_CurPType & GetPTType(nIdx)) return nIdx;				
		if(nIdx == m_nCurIdx) break;
	}
	return -1;
}

long CPlatePieceObject::GetPNext(long nCount)
{
	long nIdx;
	for(long i=0;i<nCount;i++)
	{
		nIdx = m_bAsc ? GetNextOne() : GetPrevOne();
	}
	if(nIdx !=-1) m_nCurIdx = nIdx;
	return nIdx;
}

CDPoint CPlatePieceObject::GetP1P2(long nPrevFace, long nNextFace, double dPrevW, double dNextW)
{
	long nPrevEnd = GetEndFace(nPrevFace);
	long nNextStt = GetSttFace(nNextFace);
	CDPoint cPrevStt = GetPoint(GetPrevIdx(nPrevEnd, P_TYPE_OUTLINE));
	CDPoint cPrevEnd = GetPoint(nPrevEnd);
	CDPoint cNextStt = GetPoint(nNextStt);
	CDPoint cNextEnd = GetPoint(GetNextIdx(nNextStt, P_TYPE_OUTLINE));
	CDPoint Base = cPrevEnd;
	if (cPrevEnd != cNextStt)
		GetXyMatchSegAndSeg(cPrevStt, cPrevEnd, cNextStt, cNextEnd, Base);
	CDPoint P = GetDiffOffsetPos(Base, cPrevStt, cNextEnd, dPrevW, dNextW);
	return P;
}


void CPlatePieceObject::DrawBujeMark(CDomyun *pDomP, CDPoint Base, long nNumber)
{
	CDomyun *pDom = new CDomyun(pDomP);

	CPlateBasicIndex *pBx = NULL;

	for(long n=0; n<GetObjSize(); n++)
	{
		pBx = GetBx(n);
		if(pBx) break;
	}
	if(pBx==NULL) return;

	CString sSerial;
	CDPoint xy = CDPoint(0,0);

	sSerial.Format("%d",nNumber);
	pDom->TextCircle(xy.x,xy.y,sSerial,2);
	xy.x = pDom->GetTextHeight()*3.5;
	pDom->TextOut(xy.x,xy.y-pDom->GetTextHeight()/2,m_sMarkBuje);

	pDom->Move(Base);
	*pDomP << *pDom;
	delete pDom;
}


double CPlatePieceObject::GetFlangeLMove(long nSttIdx, long nEndIdx, BOOL bUpper)
{		
	/*
	long nStt = min(nSttIdx,nEndIdx);
	long nEnd = max(nSttIdx,nEndIdx);

	double dBending = m_pPieceData->m_dBendFoldValue;
	double dMoveX = dBending * GetDistance(nStt,nEnd,TRUE) / 1000.0;

	long nIdx = nSttIdx > nEndIdx ? GetPLast(P_TYPE_PLATE,nStt,nEnd) : GetPFirst(P_TYPE_PLATE,nStt,nEnd);
	while(GetPFixNext() != -1)
	{		
		double dThick = m_pGir->GetThickJewon(m_nMarkBuje,GetBx(nIdx));			
		double dMinJRibWidth = 0;
		CJRib *pRib = GetBx(nIdx)->GetJRib(bUpper);	
		long nRibSu = pRib->Getsu();
		double dDist = GetDistance(nIdx, GetPFixNext(),TRUE);	
		for(long nJ=0;nJ<nRibSu;nJ++)
		{
			if(dMinJRibWidth == 0 ) dMinJRibWidth = pRib->GetDisArr(nJ);
			else 
			if(pRib->GetDisArr(nJ) < dMinJRibWidth) dMinJRibWidth = pRib->GetDisArr(nJ);
		}		
		dMoveX += m_pPieceData->GetLByJRibWidth(dThick,dMinJRibWidth) * dDist / 1000.0;	
		if(GetBx(nIdx)->IsState(BX_STT_BYUN_LOWER | BX_END_BYUN_LOWER)) 	dMoveX += 0.5;
		if(GetBx(GetPFixNext())->IsState(BX_STT_BYUN_LOWER | BX_END_BYUN_LOWER)) 	dMoveX += 0.5;
		if(GetBx(nIdx)->IsState(BX_SPLICE) && GetBx(nIdx)->GetSplice()->m_uD == 0)	dMoveX -= 1;
		if(GetBx(GetPFixNext())->IsState(BX_SPLICE) && GetBx(GetPFixNext())->GetSplice()->m_uD == 0) dMoveX -= 1;
		if(GetBx(nIdx)->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF))
		{
			/*
			if(GetBx(nIdx)->IsState(BX_SECTION))
			{
				if(bUpper && GetBx(nIdx)->GetDanmyun()->m_enType != CDanmyun::TYPE_E ||
				  !bUpper && GetBx(nIdx)->GetDanmyun()->m_enType != CDanmyun::TYPE_B)
				{			
					dMoveX += m_pPieceData->GetLByHRibNum(dThick) / 2;	
				}
			}
			else
				dMoveX += m_pPieceData->GetLByHRibNum(dThick) / 2;	
			* /
		}
		if(GetBx(GetPFixNext())->IsState(BX_CROSSBEAM_VBRACING | BX_SECTION))
		{
			/*
			if(GetBx(GetPFixNext())->IsState(BX_SECTION))
			{
				if(bUpper && GetBx(GetPFixNext())->GetDanmyun()->m_enType != CDanmyun::TYPE_E ||
				  !bUpper && GetBx(GetPFixNext())->GetDanmyun()->m_enType != CDanmyun::TYPE_B)
				{			
					dMoveX += m_pPieceData->GetLByHRibNum(dThick) / 2;	
				}
			}
			else
				dMoveX += m_pPieceData->GetLByHRibNum(dThick) / 2;	
			* /
		}

		nIdx = GetPNext();
	}	
	return dMoveX;
	*/
	return 0;
}

double CPlatePieceObject::GetFlangeHMove(long nIdx, BOOL bUpper)
{	
	/*
	CJRib *pRib = GetBx(nIdx)->GetJRib(bUpper);	
	long nRibSu = pRib->Getsu();
	
	double dThick = m_pGir->GetThickJewon(m_nMarkBuje,GetBx(nIdx));
	return m_pPieceData->GetHByJRibNum(dThick) * nRibSu; 		
	*/
	return 0;
}

double CPlatePieceObject::GetWebLMove(long nSttIdx, long nEndIdx, BOOL bLeft, BOOL bTop)
{		
	long nStt = min(nSttIdx,nEndIdx);
	long nEnd = max(nSttIdx,nEndIdx);
		
	double dMoveX = 0;//m_pPieceData->GetWebLByThick( * GetDistance(nStt,nEnd,TRUE) / 1000.0;

	long nIdx = nSttIdx > nEndIdx ? GetPLast(P_TYPE_PLATE,nStt,nEnd) : GetPFirst(P_TYPE_PLATE,nStt,nEnd);
	while(GetPFixNext() != -1)
	{		
		while(!GetBx(nIdx)) { nIdx = GetPNext(); if(nIdx==-1) return dMoveX;}
		double dThick = m_pGir->GetThickJewon(m_nMarkBuje,GetBx(nIdx));					
		double dDist = GetDistanceHor(nIdx, GetPFixNext(),TRUE);
		BOOL bIsHStiff = FALSE;
		CWebHStiff *pHStiff = GetBx(nIdx)->GetWebHStiff();

		// HStiff 체킹
		if(pHStiff)
		{
			if(bTop)
			{
				for(long n=0;n<WEBHSTIFFSUMAX;n++) 
				{
					if(!pHStiff->IsEmpty(n, bLeft) && n < WEBHSTIFFSUMAX/2) 
					{
						bIsHStiff = TRUE;
						break;
					}
				}
			}
			else
			{
				for(long n=0;n<WEBHSTIFFSUMAX;n++) 
				{
					if(!pHStiff->IsEmpty(n, bLeft) && n >= WEBHSTIFFSUMAX/2) 
					{
						bIsHStiff = TRUE;
						break;
					}
				}
			}
		}
		// 대우의 요청에 따락 잭업부에서의 수평보강재 삭제 ====== 다시 복구
		//if(GetBx(nIdx) && GetBx(nIdx)->IsState(BX_JACKUP))	bIsHStiff = FALSE;
		//if(GetBx(GetPFixNext()) && GetBx(GetPFixNext())->IsState(BX_JACKUP))	bIsHStiff = FALSE;
		// ======================================================
		if(GetBx(nIdx) && GetBx(nIdx)->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_JACKUP))
		{
			if(bIsHStiff)							
				dMoveX += m_pPieceData->GetWebByVStiff(dThick,TRUE) / 2;			
			else			
				dMoveX += m_pPieceData->GetWebByVStiff(dThick,FALSE) / 2;			
		}
		if(bIsHStiff)	dMoveX += m_pPieceData->GetWebLByThick(dThick) * dDist / 1000.0;
		if(GetBx(GetPFixNext()) && GetBx(GetPFixNext())->IsState(BX_CROSSBEAM_VBRACING | BX_VSTIFF | BX_JACKUP))
		{			
			if(bIsHStiff)				
				dMoveX += m_pPieceData->GetWebByVStiff(dThick,TRUE) / 2;			
			else			
				dMoveX += m_pPieceData->GetWebByVStiff(dThick,FALSE) / 2;			
		}
		if(GetBx(nIdx) && GetBx(nIdx)->IsState(BX_SPLICE) && GetBx(nIdx)->GetSplice()->m_mG == 0)	dMoveX -= 1;
		if(GetBx(GetPFixNext()) && GetBx(GetPFixNext())->IsState(BX_SPLICE) && GetBx(GetPFixNext())->GetSplice()->m_mG == 0) dMoveX -= 1;
		
		nIdx = GetPNext();
	}	

	return dMoveX;
}

CPlateBasicIndex* CPlatePieceObject::GetBx(long nIdx)			
{ 
	if(nIdx>-1)
		return m_TPAPoint[GetUseConvData()].GetAt(nIdx).GetBx();		                      

	// nIdx가 -1일경우 가장 앞의 Bx를 return;
	CPlateBasicIndex *pBx = NULL;
	for(long i=0; i<GetObjSize(); i++)
	{
		pBx = m_TPAPoint[GetUseConvData()].GetAt(i).GetBx();
		if(pBx) break;
	}
	return pBx; 
}

void CPlatePieceObject::DrawObliqueLine(CDomyun *pDomP, long nFace, BOOL bText, long nIdxStt, long nIdxEnd)
{
	CDomyun	*pDom = new CDomyun(pDomP);

	if (nIdxStt == NULL) {
		nIdxStt = GetSttFace(nFace);
	}
	if (nIdxEnd == NULL) {
		nIdxEnd = GetEndFace(nFace);
	}

	CDPoint xyStt, xyEnd, xy, vDir;
	double term = pDom->Always(1.5);
	double length = term * 1.6;

	long nIdxTermStt = nIdxStt, nIdxTermEnd = nIdxStt;
	P_TYPE CheckType = (P_TYPE_SCALLOP | P_TYPE_CHAMFER | P_TYPE_JRIB_T | P_TYPE_JRIB_C);
	while(nIdxTermEnd != nIdxEnd)
	{
		while (nIdxTermEnd != nIdxEnd)   {
			nIdxTermEnd = GetNextIdx(nIdxTermEnd);
			if (GetPTType(nIdxTermEnd) & CheckType)
				break;
		}
//		if (GetPTType(nIdxStt) & (P_TYPE_SCALLOP | P_TYPE_CHAMFER))
//			nIdxStt += 2;
//		if (GetPTType(nIdxEnd) & (P_TYPE_SCALLOP | P_TYPE_CHAMFER))
//			nIdxEnd += 1;

		xyStt = GetPoint((GetPTType(nIdxTermStt) & CheckType) ? GetPrevIdx(nIdxTermEnd) : nIdxTermStt);
		xyEnd = GetPoint((GetPTType(nIdxTermEnd) & CheckType) ? GetNextIdx(nIdxTermEnd) : nIdxTermEnd);

		vDir = (xyEnd - xyStt).Unit();
		CDPoint vTerm = vDir * term;
		CDPoint vLength = vDir * length;
		vLength = GetXyRotateRadian(CDPoint(0,0), vLength, ToRadian(IsClockWise() ? -45 : 45));

		xy = xyStt;
		while (double (vDir ^ (xy - xyEnd)) < 0)   {
			if (double (vDir ^ (xy + vLength - xyEnd)) > 0)   {
				CDPoint vLength2 = vLength.Unit() * (~(xy - xyEnd) * sqrt(2.0));
				pDom->LineTo(xy, xy + vLength2);
			}
			else
				pDom->LineTo(xy, xy + vLength);

			xy += vTerm;
		}
		nIdxTermStt = nIdxTermEnd;
	}

	if (bText)   {
		double gap = pDom->Always(0.5);
		pDom->SetTextAlignHorz(TA_CENTER);
		vDir = IsClockWise() ? vDir.RotateInv90() : vDir.Rotate90();
		if (vDir.y > 0 || vDir.x == -1)   {
			xy = (xyStt + xyEnd) / 2 + vDir * (length/1.414 + gap);
			vDir = vDir.RotateInv90();
		}
		else   {
			xy = (xyStt + xyEnd) / 2 + vDir * (length/1.414 + gap + pDom->GetTextHeight());
			vDir = vDir.Rotate90();
		}
		pDom->SetTextAngle(vDir.GetAngleDegree());
		pDom->TextOut(xy, "VS45");
	}

	*pDomP << *pDom;
	delete pDom;
}

void CPlatePieceObject::GetOffset(long nStt, long nEnd, CDPoint &SttP, CDPoint &EndP, double dValue)
{
	CDPoint SttPoint = GetPoint(nStt);
	CDPoint EndPoint = GetPoint(nEnd);
	CDPoint vDir = (EndPoint - SttPoint).Unit();
	BOOL bDir = (IsClockWise() ? TRUE : FALSE) ^ (dValue > 0);

	long nSttS = bDir ? GetNextIdx(nStt, P_TYPE_OUTLINE) : GetPrevIdx(nStt, P_TYPE_OUTLINE);
	long nEndS = bDir ? GetPrevIdx(nEnd, P_TYPE_OUTLINE) : GetNextIdx(nEnd, P_TYPE_OUTLINE);

	CDPoint SttNext = GetPoint(nSttS);
	CDPoint EndNext = GetPoint(nEndS);
	
	CDPoint vSttDir = (SttNext - SttPoint).Unit();
	CDPoint vEndDir = (EndNext - EndPoint).Unit();

	SttP = SttPoint + vSttDir * fabs(dValue / (vDir * vSttDir));
	EndP = EndPoint + vEndDir * fabs(dValue / (vDir * vEndDir));
}

void CPlatePieceObject::GetPointArray(CDPointArray &pArr)
{
	for(long n=0;n<GetObjSize();n++) pArr.Add(GetPoint(n));
}

void CPlatePieceObject::RotateByRectSize()
{
	CDPointArray PointArray;

	CDPoint SttPoint = GetPoint(0);
	GetPointArray(PointArray);
	CDPoint vVec = GetRectangleFromVertex(PointArray,TRUE,5,TRUE);
	if(vVec.x < 0) vVec = -vVec;
	for(long n=0;n<GetObjSize();n++)
	{
		SetPoint(n,GetXyRotateVector(SttPoint,GetPoint(n),vVec.GetMirrorVert()));
	}
	for(long i=0;i<GetChildCount();i++)	GetChild(i)->RotateByRectSize();	
}

void CPlatePieceObject::ResizeByHVScale(BOOL bHor, double dScale)
{
	if(bHor)
	{
		for(long n=0;n<GetObjSize();n++) 
		{
			CDPoint Po = GetPoint(n);
			SetPoint(n,CDPoint(Po.x*dScale,Po.y));
		}
		m_dHorScale *= dScale;
	}
	else
	{
		for(long n=0;n<GetObjSize();n++) 
		{
			CDPoint Po = GetPoint(n);
			SetPoint(n,CDPoint(Po.x,Po.y*dScale));
		}
		m_dVerScale *= dScale;
	}
	for(long i=0;i<GetChildCount();i++)	GetChild(i)->ResizeByHVScale(bHor,dScale);	
}

double CPlatePieceObject::GetLengthOutline()
{
	if(GetObjSize()==0) return 0;
	double len = 0;
	CDPoint xy, xyPre;
	long nIdx = GetNextIdx(0, P_TYPE_OUTLINE);
	long nSttIdx = nIdx;
	xyPre = GetPoint(nIdx);
	do  {
		nIdx = GetNextIdx(nIdx, P_TYPE_OUTLINE);
		xy = GetPoint(nIdx);
		if (GetPTType(nIdx) & P_TYPE_LINE)   {
			if (IsContinue(GetPrevIdx(nIdx, P_TYPE_OUTLINE)))
				len += ~(xyPre - xy);
			xyPre = xy;
		}
		if (GetPTType(nIdx) & P_TYPE_ARC)   {
			if (IsContinue(GetPrevIdx(nIdx, P_TYPE_OUTLINE)))   {
				double bulge = GetRadius(nIdx);
				CDPoint cen = GetCenterArc(xyPre, xy, bulge);
				double dStt = (xyPre - cen).Unit().GetAngleRadian();
				double dEnd = (xy - cen).Unit().GetAngleRadian();
				double ang  = (dEnd - dStt) * (bulge > 0 ? -1 : 1);
				len += ~(xy - cen) * InAngle(ang, TRUE);
			}
			xyPre = xy;
		}
	}
	while(nIdx != nSttIdx);
	return len;
}

void CPlatePieceObject::SetBxRange(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{ 
	m_pBxStt = pBxStt->GetBxMin(); 
	m_pBxEnd = pBxEnd->GetBxMax(); 
}

double CPlatePieceObject::GetAreaInfo(CVector &cCenter, BOOL bPolygon, BOOL bHoleApply)
{
	if (IsMarkLine())
		return 0;
	CPlatePiecePointArray& vPiece = GetPiecePointArray();
	double areaSum = vPiece.GetAreaInfo(cCenter, bPolygon);
	CVector cSum = cCenter * areaSum;
	for (long n = 0; n < GetChildCount(); n++)   {
		CVector cChild;
		double areaChild = GetChild(n)->GetAreaInfo(cChild, bPolygon);
		areaSum += areaChild;
		cSum += cChild * areaChild;
	}
	cCenter = cSum / areaSum;
	return areaSum;
}

void CPlatePieceObject::SetVerifyFlange()
{
	long i;
	CDPoint vCkVec, vRealVec;
	double nV = 0;

	for(i=GetSttFace(0); i<GetEndFace(0); i++)
	{
		if(i==GetSttFace(0))		
			vCkVec = (GetEndFacePoint(0) - GetSttFacePoint(0)).Unit();		

		vRealVec = (GetPoint(i+1) - GetPoint(i)).Unit();
		nV = vCkVec ^ vRealVec;
		if(nV < 0)
		{
			CPlatePiecePoint PiecePoint =  GetPiecePointArray().GetAt(i+1);
			m_TPAPoint[GetUseConvData()].SetAt(i+1,GetPiecePointArray().GetAt(i));
			m_TPAPoint[GetUseConvData()].SetAt(i,PiecePoint);
		}		
	}
	for(i=GetSttFace(2); i<GetEndFace(2); i++)
	{
		if(i==GetSttFace(2))		
			vCkVec = (GetEndFacePoint(2) - GetSttFacePoint(2)).Unit();		

		vRealVec = (GetPoint(i+1) - GetPoint(i)).Unit();
		nV = vCkVec ^ vRealVec;
		if(nV < 0)
		{
			CPlatePiecePoint PiecePoint =  GetPiecePointArray().GetAt(i+1);
			m_TPAPoint[GetUseConvData()].SetAt(i+1,GetPiecePointArray().GetAt(i));
			m_TPAPoint[GetUseConvData()].SetAt(i,PiecePoint);
		}
	}
}
