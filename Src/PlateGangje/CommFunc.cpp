//	CommFunc.cpp
//#define _CRTDBG_MAP_ALLOC 
//#include <crtdbg.h>
#include <stdafx.h>
#include <winreg.h>
#include <stdarg.h>
#include "typedef.h"
#include <BridgeApp.h>
#include "CommFunc.h"

///////////////////////////////////////////////////////////////////////////////
BOOL InputGridTitle(CMSFlexGrid* pGrid, PGRIDTITLE pgt, int n)
{
	pGrid->SetCols(n);
	for(int i = 0 ; i < n ; i++) {
		pGrid->SetRowHeight(0,300);
		pGrid->SetColWidth(i, pgt[i].nLen);
		pGrid->SetColAlignment(i, pgt[i].Align);
		pGrid->SetFixedAlignment(i, ALIGN_CENTER);
		pGrid->SetTextMatrix(0, i, pgt[i].szTitle);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/* end of CommFunc.cpp */
