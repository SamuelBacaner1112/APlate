// ResultForceJoint.cpp: implementation of the CPlateResultForce class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResultForceJoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateResultForce::CPlateResultForce()
{	
	m_Disp = CVector(0,0,0);
	m_Rotate = CVector(0,0,0);
	m_Force = CVector(0,0,0);
	m_Moment = CVector(0,0,0);	
}

CPlateResultForce::~CPlateResultForce()
{

}

void CPlateResultForce::Serialize(CArchive& ar)
{
	long nFlag=0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_Disp;
		ar << m_Rotate;		
		ar << m_Force;
		ar << m_Moment;		
	}
	else
	{
		ar >> nFlag;
		ar >> m_Disp;
		ar >> m_Rotate;		
		ar >> m_Force;
		ar >> m_Moment;		
	}
}