// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// RoundOpenBar.h: interface for the CRoundOpenBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUNDOPENBAR_H__38AEE7C3_586B_4B01_AC5D_BBE4C4129BCD__INCLUDED_)
#define AFX_ROUNDOPENBAR_H__38AEE7C3_586B_4B01_AC5D_BBE4C4129BCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OpenBar.h"
#include <vector>

using namespace std;

class AFX_EXT_CLASS CRoundOpenBar : public COpenBar  
{
private:
	double m_dRadius;			//곡선부분 반지름 
	double m_dAngle;			//곡선부분 각도
	double m_dAnchorageLength;	//정착길이 

public:
	CRoundOpenBar();
	virtual ~CRoundOpenBar();

//Method
public:
	void			Serialize(CArchive& ar);
	void			SetRadius(double dRadius){m_dRadius = dRadius;}
	double			GetRadius(){return m_dRadius;}
	void			SetAngle(double dAngle){m_dAngle = dAngle;}
	double			GetAngle(){return m_dAngle;}
	void			GetPosRebar(vector<CDPointArray>& vecUpper, vector<CDPointArray>& vecLower);
	double			GetLengthLine();
	void			GetPosFirstRebar(CDPointArray& ptArrUpper, CDPointArray& ptArrLower);
	CDPointArray	GetPosMirrorRebar(const CDPointArray& ptArr);
	double			GetLengthArc();
	double			GetSpaceForCalc();
	void			SetLengthAnchorage(double dLength){m_dAnchorageLength = dLength;}
	double			GetLengthAnchorage(){return m_dAnchorageLength;}
	void			SetDataDefault(BOOL bRecommend);


};

#endif // !defined(AFX_ROUNDOPENBAR_H__38AEE7C3_586B_4B01_AC5D_BBE4C4129BCD__INCLUDED_)
