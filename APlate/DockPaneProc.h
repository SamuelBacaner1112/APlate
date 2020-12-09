// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DockPaneProc.h : main header file for the ARCBRIDGE application
//

#if !defined(__DOCKPANEPROC_H__)
#define __DOCKPANEPROC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class BASE_CLASS>
class CDockPaneProc : public BASE_CLASS
{
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_NCCALCSIZE:
			{
				NCCALCSIZE_PARAMS FAR* lpncsp = (NCCALCSIZE_PARAMS FAR*)lParam;
				
				lpncsp->rgrc[0].left   += 1;
				lpncsp->rgrc[0].top    += 1;
				lpncsp->rgrc[0].right  -= 1;
				lpncsp->rgrc[0].bottom -= 1;

				return TRUE;
			}
		}
		
		return BASE_CLASS::WindowProc(message, wParam, lParam);
	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DOCKPANEPROC_H__)
