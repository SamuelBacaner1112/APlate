// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawOpenRebar.h: interface for the CDrawOpenRebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWOPENREBAR_H__E39F9886_5E58_450E_8AC5_59851EF94424__INCLUDED_)
#define AFX_DRAWOPENREBAR_H__E39F9886_5E58_450E_8AC5_59851EF94424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;

class AFX_EXT_CLASS CDrawOpenRebar  
{
private:
	int m_nBarType;


public:
	CDrawOpenRebar(CDataManage *pDataMng);
	virtual ~CDrawOpenRebar();

	CDataManage *m_pDataMng;

public:
	void SetType(int nType){m_nBarType = nType;}
	int	 GetType(){return m_nBarType;}

public:
	//Interface
	void DrawRebar(CDomyun* pDom);
	void DrawOpen(CDomyun *pDom, BOOL bDrawName = FALSE);
	void DimRebar(CDomyun* pDom, BOOL bInput = TRUE);
	void MarkRebar(CDomyun* pDom);

	//Box
	void DrawBoxRebar(CDomyun* pDom);
	void DimBoxRebar(CDomyun* pDom, BOOL bInput = TRUE);
	void DimBoxDiagonalBar(CDomyun *pDom, BOOL bInput = TRUE);
	void DimBoxStraightBar(CDomyun *pDom, BOOL bInput = TRUE);
	void MarkBoxRebar(CDomyun* pDom);

	//Round
	void DrawRoundRebar(CDomyun* pDom);
	void DrawPosRoundRebar(CDomyun *pDom, vector<CDPointArray>& vecBar, BOOL bUpper);
	void DimRoundRebar(CDomyun* pDom, BOOL bInput = TRUE);
	void MarkRoundRebar(CDomyun* pDom);

};

#endif // !defined(AFX_DRAWOPENREBAR_H__E39F9886_5E58_450E_8AC5_59851EF94424__INCLUDED_)
