// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ResultForceJoint.h: interface for the CPlateResultForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTFORCEJOINT_H__535B04EC_C2BB_4623_8A83_FD64E7963EF1__INCLUDED_)
#define AFX_RESULTFORCEJOINT_H__535B04EC_C2BB_4623_8A83_FD64E7963EF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateResultForce : public CObject
{
public:
	CPlateResultForce();
	virtual ~CPlateResultForce();

	void Serialize(CArchive& ar);

public:
	// For Joint
	CVector		m_Disp;
	CVector		m_Rotate;
	CVector		m_Force;
	CVector		m_Moment;	
};

#endif // !defined(AFX_RESULTFORCEJOINT_H__535B04EC_C2BB_4623_8A83_FD64E7963EF1__INCLUDED_)
