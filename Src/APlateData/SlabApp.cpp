// SlabApp.cpp: implementation of the CSlabApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "SlabApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSlabApp::CSlabApp(CPlateBridgeApp* pDB,CLineInfo* pLine,CPlateGirderApp* pLeft) : CPlateGirderApp(pDB, pLine, pLeft)
{

}

CSlabApp::~CSlabApp()
{

}
