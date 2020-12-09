// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__74E677BD_6DD5_41A0_B097_CC22968C1680__INCLUDED_)
#define AFX_STDAFX_H__74E677BD_6DD5_41A0_B097_CC22968C1680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxsock.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <HGXTToolkit.h>
#include <htmlhelp.h>
#include "HGCommon.h"
#include "HGDomyun.h"
#include "HGGridCtrl.h"
#include "HGException.h"
#include "HGARoadConfig.h"
#include "HGDrawManage.h"
#include "HGLineApp.h"
#include "HGXLControl.h"
#include <HGBaseConc.h>
#include <HGBaseDraw.h>
#include <HGBaseDesign.h>

#include <HGsql.h>
#include <HGDBControl.h>
#include <HGBearingInfo.h>
#include <HGCommonBridge.h>
#include <HGShareBridgeInfo.h>
#include <HGShareWithADManager.h>


#include "../Src/APlateData/APlateData.h"
#include "../Src/GeneralInput/GeneralInput.h"
#include "../Src/APlateUI/APlateUI.h"
#include "../Src/APlateDraw/APlateDraw.h"
#include "../Src/APlateOutCalc/APlateOutCalc.h"
#include "../Src/APlateOutBM/APlateOutBM.h"
#include "versionInfo.h"

// ========= Lock ===========================
#define _OLDSTATIC_LOCK_LOAD_	// 삼표 incops 보안프로그램 때문에 OLD 라이브러리 사용함
#include <LockCommon.h>
#include "HGLdk.h"
#include "HGLdkDlg.h"
// ==========================================

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__74E677BD_6DD5_41A0_B097_CC22968C1680__INCLUDED_)
