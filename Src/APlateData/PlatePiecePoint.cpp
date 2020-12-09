// PiecePoint.cpp: implementation of the CPlatePiecePoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlatePiecePoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlatePiecePoint::CPlatePiecePoint()
{
	m_PType = P_TYPE_LINE;		
	m_pBx = NULL;
	m_dLength = 0;
	m_bContinue = TRUE;		
}

CPlatePiecePoint::~CPlatePiecePoint()
{

}
