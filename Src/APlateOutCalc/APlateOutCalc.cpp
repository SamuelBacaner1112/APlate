// APlateOutCalc.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE APlateOutCalcDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("APLATEOUTCALC.DLL Initializing!\n");

		if (!AfxInitExtensionModule(APlateOutCalcDLL, hInstance))
			return 0;

		new CDynLinkLibrary(APlateOutCalcDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("APLATEOUTCALC.DLL Terminating!\n");
		AfxTermExtensionModule(APlateOutCalcDLL);
	}
	return 1;
}
