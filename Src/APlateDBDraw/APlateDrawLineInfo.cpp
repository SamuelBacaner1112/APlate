// APlateDrawLineInfo.cpp: implementation of the CAPlateDrawLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDBDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateDrawLineInfo::CAPlateDrawLineInfo(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawLineInfo::~CAPlateDrawLineInfo()
{

}

void CAPlateDrawLineInfo::DrawLineInfoPlan(CDomyun *pDom, long nLine, BOOL bTotal)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= pBridge->GetGirder(-1);
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();

	CDomyun Dom  = new CDomyun(pDom);

	double dDistance = 0,
		   dRepLen   = 10000;

	if(nLine == 0)
	{
		for(long n = 0; n < pLineApp->GetLinesu(); n++)
		{
			CLineInfo* pLine = pLineApp->GetLine(n);

			double dStaStt = 0,
				   dStaEnd = 0;

			if(!bTotal)
			{
				dStaStt = pGir->GetStationStt();
				dStaEnd = pGir->GetStationEnd();
			}

			pLine->DrawLS(&Dom, dStaStt, dStaEnd, dDistance, dRepLen);
		}
	}
	else
	{
		CLineInfo* pLine = pLineApp->GetLine(nLine - 1);

		double dStaStt = 0,
			   dStaEnd = 0;

		if(!bTotal)
		{
			dStaStt = pGir->GetStationStt();
			dStaEnd = pGir->GetStationEnd();
		}

		pLine->DrawLS(&Dom, dStaStt, dStaEnd, dDistance, dRepLen);
	}

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawLineInfoPlanChainUnit(CDomyun *pDom, long nLine, BOOL bTotal)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= pBridge->GetGirder(-1);
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();

	CDomyun Dom  = new CDomyun(pDom);

	if(nLine == 0)
	{
		for(long n = 0; n < pLineApp->GetLinesu(); n++)
		{
			CLineInfo* pLine = pLineApp->GetLine(n);

			double dStaStt = 0,
				   dStaEnd = 0;

			if(!bTotal)
			{
				dStaStt = pGir->GetStationStt();
				dStaEnd = pGir->GetStationEnd();
			}

			pLine->DrawUnitChain(&Dom, dStaStt, dStaEnd);
		}
	}
	else
	{
		CLineInfo* pLine = pLineApp->GetLine(nLine - 1);

		double dStaStt = 0,
			   dStaEnd = 0;

		if(!bTotal)
		{
			dStaStt = pGir->GetStationStt();
			dStaEnd = pGir->GetStationEnd();
		}

		pLine->DrawUnitChain(&Dom, dStaStt, dStaEnd);
	}

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawLineInfoPlanInfo(CDomyun *pDom, long nLine, BOOL bTotal)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= pBridge->GetGirder(-1);
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();

	CDomyun Dom  = new CDomyun(pDom);

	if(nLine == 0)
	{
		for(long n = 0; n < pLineApp->GetLinesu(); n++)
		{
			CLineInfo* pLine = pLineApp->GetLine(n);

			double dStaStt = 0,
				   dStaEnd = 0;

			if(!bTotal)
			{
				dStaStt = pGir->GetStationStt();
				dStaEnd = pGir->GetStationEnd();
			}

			pLine->DrawCoordinateIpLineAndStation(&Dom); // BC station,BTC station ...
		}
	}
	else
	{
		CLineInfo* pLine = pLineApp->GetLine(nLine - 1);

		double dStaStt = 0,
			   dStaEnd = 0;

		if(!bTotal)
		{
			dStaStt = pGir->GetStationStt();
			dStaEnd = pGir->GetStationEnd();
		}

		pLine->DrawCoordinateIpLineAndStation(&Dom); // BC station,BTC station ...
	}

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawLineInfoPlanInfoIP(CDomyun *pDom, long nLine, BOOL bTotal)
{
	CPlateBridgeApp	*pBridge	= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir		= pBridge->GetGirder(-1);
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();

	CDomyun Dom  = new CDomyun(pDom);

	if(nLine == 0)
	{
		for(long n = 0; n < pLineApp->GetLinesu(); n++)
		{
			CLineInfo* pLine = pLineApp->GetLine(n);

			double dStaStt = 0,
				   dStaEnd = 0;

			if(!bTotal)
			{
				dStaStt = pGir->GetStationStt();
				dStaEnd = pGir->GetStationEnd();
			}

			pLine->DrawIPText(&Dom);   // IP1, IP2, IP3, ...
		}
	}
	else
	{
		CLineInfo* pLine = pLineApp->GetLine(nLine - 1);

		double dStaStt = 0,
			   dStaEnd = 0;

		if(!bTotal)
		{
			dStaStt = pGir->GetStationStt();
			dStaEnd = pGir->GetStationEnd();
		}

		pLine->DrawIPText(&Dom);   // IP1, IP2, IP3, ...
	}

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawLineInfoCoordinate(CDomyun *pDom, long nLine, double dUnit)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	if(nLine == 0)
	{
		BOOL bTotal = TRUE;

		pLineApp->DrawCoordinateMulti(&Dom, dUnit, bTotal);
	}
	else
	{
		pLine = pLineApp->GetLine(nLine-1);

		pLine->DrawCoordinate(&Dom, dUnit);
	}

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawLineInfoTable(CDomyun *pDom, long nLine, CDPoint TableBase)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CGridDomyunEx Dom(pDom);

	long nTotalIP = pLine->GetQtyIp();

	long j = 0;

	if(nTotalIP > 0)
	{
		for(long i = 0; i < nTotalIP; i++)
		{
			CLineSplit* pLineSplit = pLine->GetLineSplitByIp(i);

			BOOL bClothoid = (pLineSplit->m_dA1 == 0 && pLineSplit->m_dA2 == 0) ? FALSE : TRUE;

			if(!bClothoid)
				j += 2;
			else
				j += 4;
		}
	}

	long nRowTotal = (nTotalIP + j) + 3;

	double LeftTitleWidth[] = {5, 15, 50, 50, 55};

	long nAlign		= GRID_ALIGN_CENTER	| GRID_ALIGN_VCENTER;
	long nAlignLeft	= GRID_ALIGN_LEFT;

	double dTextHeight = 3.0;
	
	Dom.SetColumnCount(5);
	Dom.SetRowCount(nRowTotal);	
	Dom.SetBasePoint(TableBase);
	Dom.SetTextHeight(dTextHeight);
	Dom.SetRowHeightAll(Dom.Always(dTextHeight)*2);		
	Dom.SetMargin(3);

	for(long i = 0; i< Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i, Dom.Always(LeftTitleWidth[i]));

	Dom.SetTextMatrix(0, 0, "IP" , nAlign);
	Dom.SetTextMatrix(0, 1, ""   , nAlign);
	Dom.SetTextMatrix(0, 2, "X"  , nAlign);
	Dom.SetTextMatrix(0, 3, "Y"  , nAlign);
	Dom.SetTextMatrix(0, 4, ""   , nAlign);

	Dom.SetMergeCol(0, 0, 1);

	CString csIP, csElement, csXY_X, csXY_Y, csR, csA1, csA2, csNote;

	CDPoint XY = pLine->m_xyBp;

	csXY_X.Format("%.6lf", toM(XY.y));
	csXY_Y.Format("%.6lf", toM(XY.x));

	Dom.SetTextMatrix(1, 0, "BP"  );
	Dom.SetTextMatrix(1, 1, ""    );
	Dom.SetTextMatrix(1, 2, csXY_X);
	Dom.SetTextMatrix(1, 3, csXY_Y);
	Dom.SetTextMatrix(1, 4, ""    );

	Dom.SetMergeCol(1, 0, 1);

	long n = 2;

	for(long nIP = 0; nIP < nTotalIP; nIP++)
	{
		CLineSplit* pLineSplit = pLine->GetLineSplitByIp(nIP);

		BOOL bClothoid = (pLineSplit->m_dA1 == 0 && pLineSplit->m_dA2 == 0) ? FALSE : TRUE;

		csIP.Format("IP%2ld", nIP+1);

		XY = pLineSplit->m_xyIp2;

		csXY_X.Format("%.6lf", toM(XY.y));
		csXY_Y.Format("%.6lf", toM(XY.x));

		Dom.SetTextMatrix(n, 0, csIP  );
		Dom.SetTextMatrix(n, 1, ""    );
		Dom.SetTextMatrix(n, 2, csXY_X);
		Dom.SetTextMatrix(n, 3, csXY_Y);
		Dom.SetTextMatrix(n, 4, csNote);

		Dom.SetMergeCol(n, 0, 1);

		n++;

		if(bClothoid)
		{
			csElement = "BTC";

			XY = pLineSplit->m_xyBtc;

			csXY_X.Format("%.6lf", toM(XY.y));
			csXY_Y.Format("%.6lf", toM(XY.x));

			Dom.SetTextMatrix(n, 0, ""       );
			Dom.SetTextMatrix(n, 1, csElement);
			Dom.SetTextMatrix(n, 2, csXY_X   );
			Dom.SetTextMatrix(n, 3, csXY_Y   );
			Dom.SetTextMatrix(n, 4, ""       );

			n++;
		}

		csElement = "BC";

		XY = pLineSplit->m_xyBc;

		csXY_X.Format("%.6lf", toM(XY.y));
		csXY_Y.Format("%.6lf", toM(XY.x));

		Dom.SetTextMatrix(n, 0, ""       );
		Dom.SetTextMatrix(n, 1, csElement);
		Dom.SetTextMatrix(n, 2, csXY_X   );
		Dom.SetTextMatrix(n, 3, csXY_Y   );
		Dom.SetTextMatrix(n, 4, ""       );

		n++;

		csElement = "EC";

		XY = pLineSplit->m_xyEc;

		csXY_X.Format("%.6lf", toM(XY.y));
		csXY_Y.Format("%.6lf", toM(XY.x));

		Dom.SetTextMatrix(n, 0, ""       );
		Dom.SetTextMatrix(n, 1, csElement);
		Dom.SetTextMatrix(n, 2, csXY_X   );
		Dom.SetTextMatrix(n, 3, csXY_Y   );
		Dom.SetTextMatrix(n, 4, ""       );

		n++;

		if(bClothoid)
		{
			csElement = "ETC";

			XY = pLineSplit->m_xyEtc;

			csXY_X.Format("%.6lf", toM(XY.y));
			csXY_Y.Format("%.6lf", toM(XY.x));

			Dom.SetTextMatrix(n, 0, ""       );
			Dom.SetTextMatrix(n, 1, csElement);
			Dom.SetTextMatrix(n, 2, csXY_X   );
			Dom.SetTextMatrix(n, 3, csXY_Y   );
			Dom.SetTextMatrix(n, 4, ""       );

			n++;								
		}

		if(!bClothoid)
		{
			Dom.SetMergeRow(n-2, n-1, 0);
			Dom.SetMergeRow(n-3, n-1, 4);
		}
		else
		{
			Dom.SetMergeRow(n-4, n-1, 0);
			Dom.SetMergeRow(n-5, n-1, 4);
		}

		if(!bClothoid)
		{
			csNote.Format("R  = %.6lf", toM(pLineSplit->GetRadius_Split()));

			Dom.SetTextMatrix(n-3, 4, csNote, GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
		}
		else
		{
			csNote = "";
			csR.Format("R  = %.6lf\n", toM(pLineSplit->GetRadius_Split()));
			csNote += csR;

			csA1.Format("A1 = %.6lf\n", toM(pLineSplit->m_dA1));
			csNote += csA1;

			csA2.Format("A2 = %.6lf", toM(pLineSplit->m_dA2));
			csNote += csA2;

			csNote += "\n";
			csNote += " ";

			Dom.SetTextMatrix(n-5, 4, csNote, GRID_ALIGN_LEFT);
		}
	}

	XY = pLine->m_xyEp;

	csXY_X.Format("%.6lf", toM(XY.y));
	csXY_Y.Format("%.6lf", toM(XY.x));

	Dom.SetTextMatrix(nRowTotal-1, 0, "EP"  );
	Dom.SetTextMatrix(nRowTotal-1, 1, ""    );
	Dom.SetTextMatrix(nRowTotal-1, 2, csXY_X);
	Dom.SetTextMatrix(nRowTotal-1, 3, csXY_Y);
	Dom.SetTextMatrix(nRowTotal-1, 4, ""    );

	Dom.SetMergeCol(nRowTotal-1, 0, 1);

	Dom.Draw();
	*pDom << Dom;
}

/*****************************************************************************/
/*	평면 선형 IP 제원                                                        */
/*  =================                                                        */
/*	교차점 좌표와 곡선반경, 파라미터 값을 준다.                              */
/*---------------------------------------------------------------------------*/
/*  원곡선 구간                                                              */
/*  ===========                                                              */
/*	IP : 교차점                                                              */
/*	CO : 교차점 XY축 좌표                                                    */
/*	IA : 교각                                                                */
/*	R  : 곡선반경                                                            */
/*	TL : 접선장           -> BC(or EC) 와 교차점간 거리                      */
/*	CL : 곡선장           -> EC  - BC                                        */
/*	SL : 외할             -> 교차점 과 IP간 거리 - 곡선반경                  */
/*                                                                           */
/*  클로소이드 구간                                                          */
/*  ===============                                                          */
/*	IP : 교차점                                                              */
/*	CO : 교차점 XY축 좌표                                                    */
/*	IA : 교각                                                                */
/*	R  : 곡선반경                                                            */
/*	DR : 이정량                                                              */
/*	XM : BC점에 대한 원호 중점과 BTC와의 X 축으로 부터의 거리                */
/*	X  : K.E점의 시/종점부 X축으로 부터의 거리                               */
/*	Y  : K.E점의 시/종점부 Y축으로 부터의 거리                               */
/*	LC : 곡선장									-> EC  - BC                  */
/*	CL : 크로소이드 곡선과 원 곡선의 길이			-> ETC - BTC             */
/*	D  : 시/종점부 접선장                                                    */
/*	A  : 시/종점부 파라미터                                                  */
/*	L  : 시/종점부 크로소이드 곡선 길이			-> BC - BTC, ETC - EC        */
/*****************************************************************************/
void CAPlateDrawLineInfo::DrawLineInfoTableIP(CDomyun *pDom, long nLine, long nIP, CDPoint TableBase)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CGridDomyunEx Dom(pDom);

	double dTextHeight = 3.0;
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER;

	CLineSplit* pLineSplit = pLine->GetLineSplitByIp(nIP);

	CString csIP, csCO_X, csCO_Y, csIA, csR;

	CDPoint vAng;
	vAng = ToDPointFrRadian(pLineSplit->m_dZzAngGamma1);

	csIP.Format		("IP.%ld", nIP+1);
	csCO_X.Format	("%.6lf", toM(pLineSplit->m_xyIp2.y));
	csCO_Y.Format	("%.6lf", toM(pLineSplit->m_xyIp2.x));
	csIA.Format		("%s", vAng.GetDegreeStringUseHypen());
	csR.Format		("%.6lf", toM(pLineSplit->GetRadius_Split()));

	BOOL bClothoid = (pLineSplit->m_dA1 == 0 && pLineSplit->m_dA2 == 0) ? FALSE : TRUE;

	if(!bClothoid)
	{
		//
		double dTitleWidthArc[] = {Dom.Always(15), Dom.Always(10), Dom.Always(50)};

		Dom.SetColumnCount(3);
		Dom.SetRowCount(8);
		Dom.SetBasePoint(TableBase);
		Dom.SetTextHeight(dTextHeight);
		Dom.SetRowHeightAll(Dom.Always(dTextHeight)*2);
		Dom.SetMargin(3);

		for(long i = 0; i< Dom.GetColumnCount(); i++)
			Dom.SetColumnWidth(i, dTitleWidthArc[i]);

		CString csTL, csCL_R, csSL;

		csTL.Format		("%.6lf", toM(pLineSplit->m_Clothoid1_Split.GetLengthBtcToIp_Clothoid())											);
		csCL_R.Format	("%.6lf", toM(pLineSplit->m_dLenBcToEc)																	);
		csSL.Format		("%.6lf", toM(~((pLineSplit->m_xyIp2) - (pLineSplit->m_xyCircleCenter)) - pLineSplit->GetRadius_Split()));

		Dom.SetTextMatrix(0, 0, csIP  , nAlign);
		Dom.SetTextMatrix(0, 1, ""    , nAlign);
		Dom.SetTextMatrix(0, 2, ""    , nAlign);
		Dom.SetTextMatrix(1, 0, "CO"  , nAlign);
		Dom.SetTextMatrix(1, 1, "X"   , nAlign);
		Dom.SetTextMatrix(1, 2, csCO_X, nAlign);
		Dom.SetTextMatrix(2, 0, ""    , nAlign);
		Dom.SetTextMatrix(2, 1, "Y"   , nAlign);
		Dom.SetTextMatrix(2, 2, csCO_Y, nAlign);
		Dom.SetTextMatrix(3, 0, "IA"  , nAlign);
		Dom.SetTextMatrix(3, 1, csIA  , nAlign);
		Dom.SetTextMatrix(3, 2, ""    , nAlign);
		Dom.SetTextMatrix(4, 0, "R"   , nAlign);
		Dom.SetTextMatrix(4, 1, csR   , nAlign);
		Dom.SetTextMatrix(4, 2, ""    , nAlign);
		Dom.SetTextMatrix(5, 0, "TL"  , nAlign);
		Dom.SetTextMatrix(5, 1, csTL  , nAlign);
		Dom.SetTextMatrix(5, 2, ""    , nAlign);
		Dom.SetTextMatrix(6, 0, "CL"  , nAlign);
		Dom.SetTextMatrix(6, 1, csCL_R, nAlign);
		Dom.SetTextMatrix(6, 2, ""    , nAlign);
		Dom.SetTextMatrix(7, 0, "SL"  , nAlign);
		Dom.SetTextMatrix(7, 1, csSL  , nAlign);
		Dom.SetTextMatrix(7, 2, ""    , nAlign);

		Dom.SetMergeCol(0, 0, 2);
		Dom.SetMergeCol(3, 1, 2);
		Dom.SetMergeCol(4, 1, 2);
		Dom.SetMergeCol(5, 1, 2);
		Dom.SetMergeCol(6, 1, 2);
		Dom.SetMergeCol(7, 1, 2);

		Dom.SetMergeRow(1, 2, 0);
	}
	else
	{
		//
		double dTitleWidthClothoid[] = {Dom.Always(15), Dom.Always(10), Dom.Always(30), Dom.Always(40), Dom.Always(15), Dom.Always(40), Dom.Always(40)};

		Dom.SetColumnCount(7);
		Dom.SetRowCount(7);
		Dom.SetBasePoint(TableBase);
		Dom.SetTextHeight(dTextHeight);
		Dom.SetRowHeightAll(Dom.Always(dTextHeight)*2);
		Dom.SetMargin(3);

		for(long i = 0; i< Dom.GetColumnCount(); i++)
			Dom.SetColumnWidth(i, dTitleWidthClothoid[i]);

		CString csDR_A1, csDR_A2,
				csXM_A1, csXM_A2,
				csX_A1 , csX_A2 ,
				csY_A1 , csY_A2 ,
				csLC   , csCL   ,
				csD_A1 , csD_A2 ,
				csA_A1 , csA_A2 ,
				csL_A1 , csL_A2 ;

		csDR_A1.Format	("%.6lf", toM(pLineSplit->m_Clothoid1_Split.GetLengthDeltaR1_Clothoid())	);
		csDR_A2.Format	("%.6lf", toM(pLineSplit->m_Clothoid2_Split.GetLengthDeltaR1_Clothoid())	);
		csXM_A1.Format	("%.6lf", toM(pLineSplit->m_Clothoid1_Split.GetXm1_Clothoid())				);
		csXM_A2.Format	("%.6lf", toM(pLineSplit->m_Clothoid2_Split.GetXm1_Clothoid())				);
		csX_A1.Format	("%.6lf", toM(pLineSplit->m_Clothoid1_Split.GetLengthKE_A1().x)				);
		csX_A2.Format	("%.6lf", toM(pLineSplit->m_Clothoid2_Split.GetLengthKE_A2().x)				);
		csY_A1.Format	("%.6lf", toM(pLineSplit->m_Clothoid1_Split.GetLengthKE_A1().y)				);
		csY_A2.Format	("%.6lf", toM(pLineSplit->m_Clothoid2_Split.GetLengthKE_A2().y)				);
		csLC.Format		("%.6lf", toM(pLineSplit->m_dLenBcToEc)										);
		csCL.Format		("%.6lf", toM(pLineSplit->m_dLenBtcToEtc)									);
		csD_A1.Format	("%.6lf", toM(pLineSplit->m_Clothoid1_Split.GetLengthBtcToIp_Clothoid())	);
		csD_A2.Format	("%.6lf", toM(pLineSplit->m_Clothoid2_Split.GetLengthBtcToIp_Clothoid())	);
		csA_A1.Format	("%.6lf", toM(pLineSplit->m_dA1)											);
		csA_A2.Format	("%.6lf", toM(pLineSplit->m_dA2)											);
		csL_A1.Format	("%.6lf", toM(pLineSplit->m_dLenBtcToBc)									);
		csL_A2.Format	("%.6lf", toM(pLineSplit->m_dLenEcToEtc)									);

		Dom.SetTextMatrix(0, 0, csIP   , nAlign);
		Dom.SetTextMatrix(0, 1, ""     , nAlign);
		Dom.SetTextMatrix(0, 2, ""     , nAlign);
		Dom.SetTextMatrix(0, 3, ""     , nAlign);
		Dom.SetTextMatrix(0, 4, "X"    , nAlign);
		Dom.SetTextMatrix(0, 5, csX_A1 , nAlign);
		Dom.SetTextMatrix(0, 6, csX_A2 , nAlign);
		Dom.SetTextMatrix(1, 0, "CO"   , nAlign);
		Dom.SetTextMatrix(1, 1, "X"    , nAlign);
		Dom.SetTextMatrix(1, 2, csCO_X , nAlign);
		Dom.SetTextMatrix(1, 3, ""     , nAlign);
		Dom.SetTextMatrix(1, 4, "Y"    , nAlign);
		Dom.SetTextMatrix(1, 5, csY_A1 , nAlign);
		Dom.SetTextMatrix(1, 6, csY_A2 , nAlign);
		Dom.SetTextMatrix(2, 0, ""     , nAlign);
		Dom.SetTextMatrix(2, 1, "Y"    , nAlign);
		Dom.SetTextMatrix(2, 2, csCO_Y , nAlign);
		Dom.SetTextMatrix(2, 3, ""     , nAlign);
		Dom.SetTextMatrix(2, 4, "LC"   , nAlign);
		Dom.SetTextMatrix(2, 5, csLC   , nAlign);
		Dom.SetTextMatrix(2, 6, ""     , nAlign);
		Dom.SetTextMatrix(3, 0, "IA"   , nAlign);
		Dom.SetTextMatrix(3, 1, csIA   , nAlign);
		Dom.SetTextMatrix(3, 2, ""     , nAlign);
		Dom.SetTextMatrix(3, 3, ""     , nAlign);
		Dom.SetTextMatrix(3, 4, "CL"   , nAlign);
		Dom.SetTextMatrix(3, 5, csCL   , nAlign);
		Dom.SetTextMatrix(3, 6, ""     , nAlign);
		Dom.SetTextMatrix(4, 0, "R"    , nAlign);
		Dom.SetTextMatrix(4, 1, csR    , nAlign);
		Dom.SetTextMatrix(4, 2, ""     , nAlign);
		Dom.SetTextMatrix(4, 3, ""     , nAlign);
		Dom.SetTextMatrix(4, 4, "D"    , nAlign);
		Dom.SetTextMatrix(4, 5, csD_A1 , nAlign);
		Dom.SetTextMatrix(4, 6, csD_A2 , nAlign);
		Dom.SetTextMatrix(5, 0, "DR"   , nAlign);
		Dom.SetTextMatrix(5, 1, csDR_A1, nAlign);
		Dom.SetTextMatrix(5, 2, ""     , nAlign);
		Dom.SetTextMatrix(5, 3, csDR_A2, nAlign);
		Dom.SetTextMatrix(5, 4, "A"    , nAlign);
		Dom.SetTextMatrix(5, 5, csA_A1 , nAlign);
		Dom.SetTextMatrix(5, 6, csA_A2 , nAlign);
		Dom.SetTextMatrix(6, 0, "XM"   , nAlign);
		Dom.SetTextMatrix(6, 1, csXM_A1, nAlign);
		Dom.SetTextMatrix(6, 2, ""     , nAlign);
		Dom.SetTextMatrix(6, 3, csXM_A2, nAlign);
		Dom.SetTextMatrix(6, 4, "L"    , nAlign);
		Dom.SetTextMatrix(6, 5, csL_A1 , nAlign);
		Dom.SetTextMatrix(6, 6, csL_A2 , nAlign);

		Dom.SetMergeCol(0, 0, 3);
		Dom.SetMergeCol(1, 2, 3);
		Dom.SetMergeCol(2, 2, 3);
		Dom.SetMergeCol(3, 1, 3);
		Dom.SetMergeCol(4, 1, 3);
		Dom.SetMergeCol(5, 1, 2);
		Dom.SetMergeCol(6, 1, 2);
		Dom.SetMergeCol(2, 5, 6);
		Dom.SetMergeCol(3, 5, 6);

		Dom.SetMergeRow(1, 2, 0);
	}

	Dom.Draw();
	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawJongDanPlan(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	// nVIP에서 nVIP까지 종단 곡선 그림
	pLine->DrawJongDanCurve(&Dom, dStaStt, dStaEnd, dRatio, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawJongDanPlanLine(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	// nVIP에서 nVIP까지 종단 VIP 그림
	pLine->DrawJongDanLine(&Dom, dStaStt, dStaEnd, dRatio, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawJongDanPlanLineInfo(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	BOOL bScaleDom = TRUE;

	// 종경사(%), nVIP 편차 (H), 종경사 길이(L) 출력
	pLine->DrawJongDanHeightLengthTextOut(&Dom, dStaStt, dStaEnd, dRatio, bUser, bScaleDom);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawJongDanPlanInfoVIP(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	// 종단정보(nVIP, evc, bvc station, elevation) 출력
	pLine->DrawJongDanVIPTextOut(&Dom, dStaStt, dStaEnd, dRatio, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawPyungubaePlan(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	pLine->DrawPyungubaePlan(&Dom, dStaStt, dStaEnd, dRatio, nCase, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawPyungubaePlanInfo(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bTotal, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	Dom.SetTextAngle(90);
	Dom.SetTextAlignHorz(TA_LEFT);
	pLine->DrawPyungubaeStationDim(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawPyungubaeLineInfo(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bTotal, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	Dom.SetTextAngle(90);
	Dom.SetTextAlignHorz(TA_LEFT);
	pLine->DrawPyungubaeLineInfoDim(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawPyungubaePlanCenterLine(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, double dChainUnit, int nCase, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();	

	CDomyun Dom  = new CDomyun(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextAlignHorz(TA_LEFT);
	pLine->DrawPyungubaeCenterLineText(&Dom, dStaStt, nCase, bUser);

	*pDom << Dom;

	pOptStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.SetTextAlignHorz(TA_CENTER);
	pLine->DrawPyungubaeCenterLine(&Dom, dStaStt, dStaEnd, nCase, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawPyungubaePlanUnitChain(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, double dChainUnit, int nCase, BOOL bTotal, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	pLine->DrawPyungubaeCenterLineDim(&Dom, dStaStt, dStaEnd, dChainUnit, nCase, bUser);

	*pDom << Dom;
}

void CAPlateDrawLineInfo::DrawPyungubaeRatioBar(CDomyun *pDom, double dStaStt, double dStaEnd, double dRatio, long nLine, int nCase, BOOL bTotal, BOOL bUser)
{
	CLineInfoApp	*pLineApp	= m_pDataMng->GetLineInfoApp();
	CLineInfo		*pLine		= pLineApp->GetLine(nLine);

	CDomyun Dom  = new CDomyun(pDom);

	pLine->DrawPyungubaeRatioBar(&Dom, dStaStt, dStaEnd, dRatio, nCase, bTotal, bUser);

	*pDom << Dom;
}
