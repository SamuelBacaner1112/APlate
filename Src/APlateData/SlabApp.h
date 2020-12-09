// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SlabApp.h: interface for the CSlabApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLABAPP_H__004DB2E8_17E1_450E_9659_783537C23AD0__INCLUDED_)
#define AFX_SLABAPP_H__004DB2E8_17E1_450E_9659_783537C23AD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlateGirderApp.h"
class AFX_EXT_CLASS CSlabApp : public CPlateGirderApp  
{
public:
	CSlabApp(CPlateBridgeApp* pDB,CLineInfo* pLine,CPlateGirderApp* pLeft = NULL);
	virtual ~CSlabApp();

};

#endif // !defined(AFX_SLABAPP_H__004DB2E8_17E1_450E_9659_783537C23AD0__INCLUDED_)
