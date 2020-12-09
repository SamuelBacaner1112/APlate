// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PieceInfoApp.h: interface for the CPieceInfo class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_PLATEPIECEINFOAPP_H__726CD1C9_A199_46E8_8389_875850D4018F__INCLUDED_)
#define AFX_PLATEPIECEINFOAPP_H__726CD1C9_A199_46E8_8389_875850D4018F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CPlatePieceObject : public CObject  
{
public:
	CPlatePieceObject(CPlateGirderApp *pGir, long nMarkBuje=-1, CPlatePieceData *pData = NULL, long nRow=0, long nCol=0, BOOL bFront=TRUE);
	virtual ~CPlatePieceObject();	

	void SetSolid(BOOL bSolid)	{ m_TPAPoint[GetUseConvData()].SetSolid(bSolid); }
 	BOOL IsSolid()	const		{ return m_TPAPoint[GetUseConvData()].IsSolid(); }
 	void SetBoltHole(BOOL bBolt)	{ m_TPAPoint[GetUseConvData()].SetBoltHole(bBolt); }
	BOOL IsBoltHole()	const	{ return m_TPAPoint[GetUseConvData()].IsBoltHole(); }
	BOOL HaveArc()	const		{ return GetBitFlag32(m_dwAttr,POJB_HAS_ARC); }
	BOOL Is2Dim()	const		{ return GetBitFlag32(m_dwAttr,POJB_2DIM); }
	BOOL IsFold()	const		{ return GetBitFlag32(m_dwAttr,POJB_FOLD); }
	BOOL IsClockWise()	const	{ return m_TPAPoint[GetUseConvData()].IsClockWise(); }
	BOOL HasOpeUnit()	const	{ return GetBitFlag32(m_dwAttr,POJB_HAS_OPEN_UNIT); }
	BOOL HasChild()	const		{ return (m_ChildArray.GetSize()!=0) ? TRUE : FALSE; }
	BOOL IsTopPiece() const		{ return (m_pParentObj) ? FALSE : TRUE; }
	void SetMarkLine(BOOL bPieceMark)	{ SetBitFlag32(m_dwAttr,POJB_PIECEMARK,bPieceMark); }	
	BOOL IsMarkLine() const		{ return GetBitFlag32(m_dwAttr,POJB_PIECEMARK); }		
	void SetLeftThick(BOOL bValue)		{ SetBitFlag32(m_dwAttr,POJB_LEFT,bValue); }
	BOOL IsLeft() const			{ return GetBitFlag32(m_dwAttr,POJB_LEFT); }
	void SetClockWise(BOOL bClockWise)	{ m_TPAPoint[GetUseConvData()].SetClockWise(bClockWise); }
	BOOL IsUseConvData() const			{ return GetBitFlag32(m_dwAttr,POJB_USE_CONV_DATA); }
	void SetUseConvData(BOOL bValue)	{ SetBitFlag32(m_dwAttr,POJB_USE_CONV_DATA,bValue); }	
	void Set3D(BOOL bValue)		{ SetBitFlag32(m_dwAttr,POJB_3D,bValue); }
	BOOL Is3D() const			{ return GetBitFlag32(m_dwAttr,POJB_3D); }
	long GetUseConvData() const	{ return IsUseConvData() ? 1 : 0; }



	void SetFaceNum(long nFace) { m_nFaceSize = nFace; }
	long GetFaceNum()			{ return m_nFaceSize; }
	long GetFace(long nIdx, BOOL bEndInclude = TRUE);

	void AddChildObj(CPlatePieceObject *pObj)	{ m_ChildArray.Add(pObj); }	
	void SetParentObj(CPlatePieceObject *pObj)  { m_pParentObj = pObj; pObj->AddChildObj(this); }	

	CPlatePieceObject *GetParent()		{ return m_pParentObj; }
	long	GetChildCount()		{ return m_ChildArray.GetSize(); }
	CPlatePieceObject *GetChild(long n)	{ return m_ChildArray.GetAt(n); }
	double GetLength(long nIdx = 0);
	double GetLength(long nIdxStt, long nIdxEnd);			

public:
	long	m_nMarkBuje;
	long	m_nRow;
	long	m_nCol;
	BOOL	m_bFront;
	CString m_sMarkBuje;
	long	m_nQty;

	double	m_dHorScale;
	double	m_dVerScale;

	static BOOL m_bUsePieceLayer;

protected:
	CPlateGirderApp *m_pGir;
	CPlatePieceData *m_pPieceData;	
		
	long	m_nLayer;		// ARoadConfig 에 정의된 레이어
	long	m_nColor;		// 라인색
	long	m_nFaceSize;	// 면(방향)의 개수를 설정
	DWORD	m_dwAttr;


	CPlateBasicIndex *m_pBxStt;
	CPlateBasicIndex *m_pBxEnd;

	CPlatePieceObject *m_pParentObj;// 자신이 소유한 부모 배열 포인터	

	CTypedPtrArray <CObArray,CPlatePieceObject*>	  m_ChildArray;
	CDWordArray m_TPAFaceStt[2];	// FACE 배열
	CDWordArray m_TPAFaceEnd[2];	// FACE 배열
	CPlatePiecePointArray m_TPAPoint[2];	// 좌표배열		

// 내부용 함수 ========================================================
private:
	CPlatePiecePoint* Add(CPlatePiecePoint Obj);
	CPlatePiecePoint* InsertAt(long nIdx, CPlatePiecePoint pObj);	
	void InsertFlangeAddSize(long nIdx, double dSizeU, double dSizeL, BOOL bLeft);	
	void InsertWebAddSize(long nIdx, double dSizeU, double dSizeL, BOOL bLeft);	

// Operator overload ====================================================
public:
	CPlatePieceObject& operator=(const CPlatePieceObject& obj);
//	CPlatePieceObject& operator=(const CDPointArray& DArr);
//	CPlatePieceObject& operator=(const CVectorArray& DArr);
	void SetApplyVectorArray(CVectorArray* pDArr = NULL);
	void SetApplyDPointArray(CDPointArray* pDArr = NULL);
	
// 속성 읽기 함수 ====================================================
public:
	CPlateGirderApp *GetGirder()					{ return m_pGir; }
	CPlateBasicIndex *GetBx(long nIdx);
	long	GetLayer()						{ return m_nLayer; }
	long	GetLineColor()					{ return m_nColor; }	
	CPlatePieceData* GetPieceData()			{ return m_pPieceData; }

public:
	long GetObjSize()						{ return m_TPAPoint[GetUseConvData()].GetSize(); }
	long GetObjSizeByPType(P_TYPE type);
	long GetUpperBound()					{ return m_TPAPoint[GetUseConvData()].GetUpperBound(); }
	//
	BOOL	IsContinue(long nIdx)			{ return m_TPAPoint[GetUseConvData()].GetAt(nIdx).IsContinue(); }	
	P_TYPE GetPTType(long nIdx)				{ return m_TPAPoint[GetUseConvData()].GetAt(nIdx).GetType(); }
	void SetPTType(long nIdx, P_TYPE type)	{ m_TPAPoint[GetUseConvData()].GetAt(nIdx).SetType(type); }
	BOOL IsPTType(long nIdx, P_TYPE type);
	//
	CPlatePiecePoint *GetPointObj(long nIdx)	{ return m_TPAPoint[GetUseConvData()].GetPAt(nIdx); }
	CPlatePiecePoint *GetFirstPointObj()		{ return m_TPAPoint[GetUseConvData()].GetPAt(0); }
	CPlatePiecePoint *GetLastPointObj()		{ return m_TPAPoint[GetUseConvData()].GetPAt(m_TPAPoint[GetUseConvData()].GetUpperBound()); }					
	CDPoint GetPoint(long nIdx)			{ return m_TPAPoint[GetUseConvData()].GetAt(nIdx).GetPoint(); }	
	//
	double GetRadius(long nIdx)			{ return m_TPAPoint[GetUseConvData()].GetAt(nIdx).GetLength(); }
	double GetDistance(long nIdxStt, long nIdxEnd, BOOL bOrg = FALSE);
	double GetDistanceHor(long nIdxStt, long nIdxEnd, BOOL bOrg = FALSE);
	CDPoint GetRectSize(CDPoint vDir = CDPoint(0,0), BOOL bAutoDir=TRUE);	
	long GetMatchIndex(long nMatchFace, long nIdx, BOOL bEndInclude = TRUE); 
	long GetMatchIndexByBx(long nIdx);
	CDPoint GetP1P2(long nPrevFace, long nNextFace, double dPrevW, double dNextW);
	CVector GetSetBackPoint(long nIdx, BOOL bPrev, double len);	
	void GetPointArray(CDPointArray &pArr);

	void SetBack(long nIdx, BOOL bPrev, double len);

// Face 관련 함수 ====================================================
public:
	CDPoint GetSttFacePoint(long nFace) { return GetPoint(GetSttFace(nFace)); }
	CDPoint GetEndFacePoint(long nFace) { return GetPoint(GetEndFace(nFace)); }
	long GetSttFace(long nFace)			{ return m_TPAFaceStt[GetUseConvData()].GetAt(nFace); }
	long GetEndFace(long nFace)			{ return m_TPAFaceEnd[GetUseConvData()].GetAt(nFace); }
	void SetSttFace(long n, long nIdx)	{ m_TPAFaceStt[GetUseConvData()].SetAt(n,nIdx); }
	void SetEndFace(long n, long nIdx)	{ m_TPAFaceEnd[GetUseConvData()].SetAt(n,nIdx); }
	void SetEnd();
	void SetEnd(const CDPoint &uXy, P_TYPE pType,CPlateBasicIndex *pBx, double len);
	void SetFace(long nIdx, BOOL bStt, long nStep = 0 /* 0 보다 작을것 */);	
	void SetFaceRange(long nIdxStt, long nIdxEnd);		
	void AddSttFace(long nIdx)			{m_TPAFaceStt[GetUseConvData()].Add(nIdx); }
	void AddEndFace(long nIdx)			{m_TPAFaceEnd[GetUseConvData()].Add(nIdx); }

// 속성 쓰기 함수 ====================================================
public:
	void SetBxRange(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void SetPoint(long nIdx, const CDPoint &uXy) { m_TPAPoint[GetUseConvData()].GetAt(nIdx).SetPoint(uXy); }	
	void SetBx(long nIdx, CPlateBasicIndex *pBx) { m_TPAPoint[GetUseConvData()].GetAt(nIdx).SetBx(pBx); }
	void ToggleConv()						{ SetUseConvData(!IsUseConvData()); }
	void SetLayer(long nLayer)				{ m_nLayer = nLayer; }
	void SetLineColor(long nColor)			{ m_nColor = nColor; }

public:	
	// 추가 함수	
	CPlatePiecePoint *AddElement(const CDPoint &uXy, P_TYPE pType = P_TYPE_LINE, long nIdx = -1, CPlateBasicIndex *pBx = NULL, double bulge = 0);
	CPlatePiecePoint *AddElement(const CDPoint &uXy, double R, long nIdx = -1, CPlateBasicIndex *pBx = NULL, P_TYPE pType = P_TYPE_ARC);
	CPlatePiecePoint *AddElement(const CDPoint &CenPoint, double R, double DegSttAngle, double DegAngle, long nIdx = -1, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL);
	CPlatePiecePoint *AddCircle(const CDPoint &CenPoint, double R, CPlateBasicIndex *pBx=NULL);
	// 추가 함수
	void InsertElement(long nIdx, const CDPoint &uXy, P_TYPE pType = P_TYPE_LINE, CPlateBasicIndex *pBx = NULL, BOOL bPrev = TRUE);
	void InsertElementOnFace(long nFace, long nLocalIdx, const CDPoint &uXy, P_TYPE pType = P_TYPE_LINE, CPlateBasicIndex *pBx = NULL, BOOL bStt = TRUE);
	// 삭제 함수
	void RemoveElement(long nIdx);

	// 타입설정						
	void AddType(long nIdx, P_TYPE type)	{m_TPAPoint[GetUseConvData()].GetAt(nIdx).AddType(type); }	
	// 좌표 얻기
	void GetOffset(long nStt, long nEnd, CDPoint &SttP, CDPoint &EndP, double dValue);
	// Piece 특정 포인터 추가			
	long InsertScallop(long nIdx);
	long InsertChamfer(long nIdx);
	long InsertFillet(long nIdx, P_TYPE nType = 0);
	long InsertFilletAt(long nIdxPrev, long nIdx, long nIdxNext, P_TYPE nType = 0);		
	long InsertChamferAtArc(long nIdx, double D);
	long InsertScallopAtArc(long nIdx, double R);
	long InsertBreak(long nIdx);
	long InsertSpliceScallopC(long nIdx);
	long InsertSpliceScallopT(long nIdx);

// Finder 함수 ========================================================
private:
	P_TYPE m_CurPType;

	long m_nCurIdx;	
	long m_nMinIdx;
	long m_nMaxIdx;
	BOOL m_bAsc;	
	BOOL m_bIgnoreAngle;

	long GetPrevOne();
	long GetNextOne();

public:
	long GetPFirst(P_TYPE pType, long nSttIdx = -1, long nEndIdx = -1);
	long GetPLast(P_TYPE pType, long nSttIdx = -1, long nEndIdx = -1);
	long GetPFixNext();
	long GetPFixPrev();
	long GetPNext(long nCount = 1);
	//
	long GetPrevIdx(long nIdx, P_TYPE pType = 0, long nTime = 1);  // pType == 0 : Type에 상관없이 모두.
	long GetNextIdx(long nIdx, P_TYPE pType = 0, long nTime = 1);
	//
	long GetPrevIdxOnBx(long nIdx, P_TYPE pType = 0, __int64 BxType = 0, long nTime = 1);  // pType == 0 : Type에 상관없이 모두.
	long GetNextIdxOnBx(long nIdx, P_TYPE pType = 0, __int64 BxType = 0, long nTime = 1);
	long GetVertexSu(P_TYPE pType = 0);
	void SetIgnoreAngle(BOOL bIgnoreAngle) { m_bIgnoreAngle = bIgnoreAngle; }
	BOOL GetIgnoreAngle() { return m_bIgnoreAngle; }

// 사용자 편의 가공함수 ===============================================
public:
	CPlatePiecePointArray& GetPiecePointArray()	{ return m_TPAPoint[GetUseConvData()]; }

	void ResizeFlange(BOOL bUpper);
	void ResizeWeb(BOOL bLeft);
	void RegenVertex();	
	void RegenVertexEx(double dScallop, double dChamfer, double dSetBack);
	void RotateByFace(long nFace);
	void RotateByPoint(CDPoint PoStt, CDPoint PoEnd);
	void SetFlangeBx(BOOL bUpper);
	void SetWebBx(__int64 BxFlag = 0);
	void SetVerifyFlange();					// 프랜지 에서 인덱스 엇갈림 점검	

	double GetFlangeLMove(long nSttIdx, long nEndIdx, BOOL bUpper);
	double GetFlangeHMove(long nIdx, BOOL bUpper);
	double GetWebLMove(long nSttIdx, long nEndIdx, BOOL bLeft, BOOL bTop);

	void RotateByRectSize();
	void ResizeByHVScale(BOOL bHor, double dScale);
	double GetAreaInfo(CVector &cCenter, BOOL bPolygon = TRUE, BOOL bHoleApply = FALSE);

// Piece Draw함수 =====================================================
public:
	double GetLengthOutline();
	void DrawObliqueLine(CDomyun *pDomP, long nFace, BOOL bText = FALSE, long nIdxStt=NULL, long nIdxEnd=NULL);
	void DrawBujeMark(CDomyun *pDomP, CDPoint Base, long nNumber);
	void DrawPieceOutline(CDomyun *pDom, CARoadOptionStd *pStd, BOOL bFillCircle=FALSE);
	void DrawPieceSectionMark(CDomyun *pDom, long nFace, CARoadOptionStd *pStd);
	void DrawPieceFace(CDomyun *pDom,long nFace, CARoadOptionStd *pStd);
	void DrawChamferCenterLine(CDomyun *pDom, long nIdx = -1, double dLenRatio = 0.8);
	
};

#endif // !defined(AFX_PLATEPIECEINFOAPP_H__726CD1C9_A199_46E8_8389_875850D4018F__INCLUDED_)
