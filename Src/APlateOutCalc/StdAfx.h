// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0C4D9F66_1F30_4594_AFB3_CCE075741BF1__INCLUDED_)
#define AFX_STDAFX_H__0C4D9F66_1F30_4594_AFB3_CCE075741BF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <HGXTToolkit.h>
#include "HGCommon.h"
#include "HGDomyun.h"
#include "HGGridCtrl.h"
#include "HGException.h"
#include "HGARoadConfig.h"
#include "HGXLControl.h"
#include <HGGhostConc.h>
#include <HGBaseConc.h>
#include "HGBaseDesign.h"
#include "HGLineApp.h"

#include "../APlateData/APlateData.h"
#include "../APlateCalc/APlateCalc.h"
#include "../GeneralInput/GeneralInput.h"
#include "../APlateCalc/APlateCalc.h"
#include "../PlateGangje/Gangje.h"
#include "../APlateDBDraw/APlateDBDraw.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0C4D9F66_1F30_4594_AFB3_CCE075741BF1__INCLUDED_)
