// APlateDrawDetailDim.cpp: implementation of the CAPlateDrawDetailDim class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateDrawDetailDim::CAPlateDrawDetailDim(CAPlateDrawStd *pStd, CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	m_pDom = pDom;
	m_pBxStt = pBxStt;
	m_pBxEnd = pBxEnd;
	m_pStd = pStd;

	m_nUpperDan		= 1;
	m_nLowerDan		= 1;
	m_nWebUpperDan	= 1;
	m_nWebLowerDan	= 1;	

	m_PyunDimFlag	= 0;
	m_JongDimFlag	= 0;

	m_dLen_Title	= 0;
	m_nDirDimension = SIDE_CENTER; // 치수선이 그려지는 거더의 방향(좌측,우측,중심......측면도만 해당됨)
	m_bSlabDim		= FALSE;
}

CAPlateDrawDetailDim::~CAPlateDrawDetailDim()
{

}

void CAPlateDrawDetailDim::SetPyungDimDan(long nDan, BOOL bLeft, BOOL bSlabDim)
{
	if(bLeft) m_nUpperDan = nDan;
	else	  m_nLowerDan = nDan;
	m_bSlabDim = bSlabDim;
}

void CAPlateDrawDetailDim::SetJongDimDan(long nDan, BOOL bUpper, BOOL bSlabDim)
{
	if(bUpper) m_nWebUpperDan = nDan;
	else	   m_nWebLowerDan = nDan;
	m_bSlabDim = bSlabDim;
}

void CAPlateDrawDetailDim::DimTotal(CDomyun *pDomP,__int64 nType, BOOL bUpper, BOOL bLeftPos, BOOL bPyung, BOOL bGen, BOOL bStartPage, BOOL bLastPage, BOOL bGirderNumber)
{		
	CPlateBridgeApp *pBridge = m_pStd->GetDataManage()->GetBridge();

	/////////////////////////////////// 전단연결재 ////////////////////////////////////
	CString str = _T("");
	if(bGirderNumber)
	{
		str.Format("(G%d) ",m_pBxStt->GetNumberGirderEx()+1);
		if(m_pBxStt->GetNumberGirder()==-1)
			str = "(도로중심) ";
		m_dLen_Title = 50;
	}
	else
		m_dLen_Title = 35;
	if(nType & DIM_BASE_WELDSTUD)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "전단연결재";
		MakeStudBx(&DimData);
		// 평면에서는 전단연결재를 일단 사용안함
		//if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen);
		if(!bPyung) DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,TRUE,FALSE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 수직보강재 ////////////////////////////////////
	if(nType & DIM_BASE_VSTIFF)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "수직보강재";
		MakeVStiffBx(&DimData,SIDE_CENTER);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,FALSE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,FALSE,bStartPage,bLastPage);
	}	
	/////////////////////////////////// 가로보 ////////////////////////////////////
	if(nType & DIM_BASE_DIAP)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "가로보";
		MakeCrossBeamBx(&DimData,SIDE_CENTER);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,FALSE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,FALSE,bStartPage,bLastPage);
	}
	
	/////////////////////////////////// 상부공장이음 ////////////////////////////////////
	if(nType & DIM_BASE_FACTUPPER)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "상부공장이음";
		MakeFlangeBx(&DimData,SIDE_CENTER,TRUE);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,TRUE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,TRUE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 하부공장이음 ////////////////////////////////////
	if(nType & DIM_BASE_FACTLOWER)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "하부공장이음";
		MakeFlangeBx(&DimData,SIDE_CENTER,FALSE);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,TRUE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,TRUE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 웨브공장이음 ////////////////////////////////////
	if(nType & DIM_BASE_FACTWEB)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "복부공장이음";
		MakeWebBx(&DimData);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,TRUE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,TRUE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 상판&현장이음 ////////////////////////////////////
	if(nType & DIM_BASE_JOINUPPER)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "상판";
		MakeFlangeSpliceBx(&DimData,SIDE_CENTER,TRUE);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,TRUE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,TRUE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 하판&현장이음 ////////////////////////////////////
	if(nType & DIM_BASE_JOINLOWER)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "하판";
		MakeFlangeSpliceBx(&DimData,SIDE_CENTER,FALSE);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,TRUE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,TRUE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 웨브&현장이음 ////////////////////////////////////
	if(nType & DIM_BASE_JOINWEB)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "복부판";
		MakeWebSpliceBx(&DimData);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,TRUE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,TRUE,bStartPage,bLastPage);
	}

	/////////////////////////////////// 현장이음 ////////////////////////////////////
	if(nType & DIM_BASE_SPLICE)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "현장이음";
		MakeFieldSpliceBx(&DimData,SIDE_CENTER);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,FALSE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,FALSE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 변단면 ////////////////////////////////////
	if(nType & DIM_BASE_BYUN && pBridge->m_cByunDan)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "변단면";
		MakeByunBx(&DimData,SIDE_CENTER,FALSE);
		if(DimData.GetSize()>1)
		{
			if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,FALSE,bStartPage,bLastPage);
			else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,FALSE,bStartPage,bLastPage);
		}
	}
	/////////////////////////////////// TENDON ////////////////////////////////////
	if(nType & DIM_BASE_TENDON)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "TENDON";
		MakeTendonBx(&DimData, SIDE_CENTER);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,FALSE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,FALSE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 지간구성 ////////////////////////////////////
	if(nType & DIM_BASE_SPAN)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "지간구성";
		MakeJiGanBx(&DimData,SIDE_CENTER);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,FALSE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,FALSE,bStartPage,bLastPage);
	}
	/////////////////////////////////// 총길이 ////////////////////////////////////
	if(nType & DIM_BASE_TOTAL)
	{
		DimDataArr DimData;	
		m_DimTitle = str + "총길이";
		MakeTotalBx(&DimData,SIDE_CENTER);
		if(bPyung) DrawDimPyungBx(pDomP,&DimData,SIDE_CENTER,bUpper,bLeftPos,bGen,FALSE,bStartPage,bLastPage);
		else	   DrawDimJongBx(pDomP,&DimData,SIDE_CENTER,bUpper,FALSE,FALSE,bStartPage,bLastPage);
	}
}

double CAPlateDrawDetailDim::GetDisFlangeBySta(CPlateGirderApp *pGir, double SttSta, double EndSta, BOOL bUpper, long nDir)
{	
	CPlateBridgeApp *pBridge	= m_pStd->GetDataManage()->GetBridge();
	
	double Dist		= 0;	

	if(m_bSlabDim && nDir==-1)
		Dist = -pBridge->m_dWidthSlabLeft;
	if(m_bSlabDim && nDir== 1)
		Dist = pBridge->m_dWidthSlabRight;

	return pGir->GetLengthDimType(SttSta, EndSta, Dist, TRUE, DIM_BASIC);
}

double CAPlateDrawDetailDim::GetStaFlangeByBx(CPlateBasicIndex *pBx, BOOL bUpper, long nDir, BOOL bApplyEdge)
{	
	CPlateGirderApp*			pGir = pBx->GetGirder();

	double Dist   = 0;
	double RetSta;

	if(pBx->IsState(BX_STT_GIRDER) && bApplyEdge)
	{
		RetSta = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_GIRDER),
	                                  Dist,
									  pGir->GetAAA(OFF_STT_GIRDER));
	} 
	else
	if(pBx->IsState(BX_END_GIRDER) && bApplyEdge)
	{
		RetSta = pGir->GetStationBySc(pGir->GetSSS(OFF_END_GIRDER),
	                                  Dist,
									  pGir->GetAAA(OFF_END_GIRDER));
	}
	else
	{
		RetSta = pGir->GetStationBySc(pBx->GetStation(),
	                                  Dist,
									  pBx->GetAngle());
	}
	return RetSta;
}

double CAPlateDrawDetailDim::GetDisWebBySta(CPlateGirderApp *pGir, double SttSta, double EndSta, long nDir)
{	
	return pGir->GetLengthDimType(SttSta, EndSta, 0, TRUE, DIM_BASIC);
}

double CAPlateDrawDetailDim::GetStaWebByBx(CPlateBasicIndex *pBx, long nDir)
{	
	CPlateGirderApp*			pGir = pBx->GetGirder();

	double RetSta;

	RetSta = pGir->GetStationBySc(pBx->GetStation(),0, pBx->GetAngle());	
	return RetSta;
}

void CAPlateDrawDetailDim::DimDetailSttJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bRotate, BOOL bApplyJongdan, long nDir, BOOL bAuto)
{	
	/*
	CARoadOptionStd*	pOpt = m_pStd->m_pOptStd;	
	CPlateBridgeApp* pBridge= m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp* pGir   = (CPlateGirderApp*)pBxStt->GetGirder();
	CPlateGirderApp* pGirCen= m_pStd->GetDataManage()->GetBridge()->GetGirder(-1);

	CLineInfo*			pLine	 = pGir->GetLine();	
	CPlateBxFinder			Finder(pGir);	

	CDimDomyunEx Dom(pDom);	
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("LEFT");
	Dom.SetDimDir(DIR_NONE);	

	double dSta = pBxStt->GetStation();	

	if(bRotate)			Dom.SetDimTextDir(TRUE);	
	if(bApplyJongdan)	Dom.SetDimObq(ToDegree(atan(pGir->GetGiulFlangeUpperLowerCamber(dSta))));

	CPlateBasicIndex *pBx;
	CWebHStiff *pWebHStiff = pBxStt->GetWebHStiff(nDir);
	
	__int64 BxFlag = BX_PLATEHSTIFF_CENTER;
	if(nDir == SIDE_LEFT) BxFlag = BX_PLATEHSTIFF_LEFT;
	if(nDir == SIDE_RIGHT) BxFlag = BX_PLATEHSTIFF_RIGHT;

	pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	if (!pBx->GetWebHStiff(nDir)) return;

	BOOL bExist = FALSE;
	while(pBx)
	{		
		for(long i=0;i<6;i++)
		{
			if(pBx->GetWebHStiff(nDir) && !pBx->GetWebHStiff(nDir)->IsEmpty(i)) bExist = TRUE;
		}
		if(bExist) break;
		pBx = Finder.GetBxNext();
	}
	if(!bExist) return;
	double Height = pGir->GetHeightGirderByStaAng(dSta, pGir->GetWidthFlangeUpper(pBx)/2*nDir, pBx->GetAngle());	
	long UpperDan	= pBx->GetWebHStiff(nDir)->GetUpperDan();
	long LowerDan	= pBx->GetWebHStiff(nDir)->GetLowerDan();			

	if(!pBxEnd->IsState(BX_END_GIRDER) && bAuto)
	{
		long UpperDanEnd = pBxEnd->GetWebHStiff(nDir)->GetUpperDan();
		long LowerDanEnd = pBxEnd->GetWebHStiff(nDir)->GetLowerDan();
		if(UpperDan == UpperDanEnd && 
		   LowerDan == LowerDanEnd && 
		   pBxStt->GetHeightGirderByBx() == pBxEnd->GetHeightGirderByBx())
		   return;
	}

	double y1	= pGir->GetElevationFlangeCamber(dSta, pGir->m_dWidth/2*nDir, CDPoint(0,1), TRUE) + pGir->GetHeightFlangeUpperByTaper(dSta);
	double y2	= pGir->GetElevationFlangeCamber(dSta, pGir->m_dWidth/2*nDir, CDPoint(0,1), FALSE);
	double xDis = Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis());
	
	if(pBxStt->IsState(BX_SPLICE)) xDis += Dom.Always(Dom.GetDimLevelDis());

	// 출입구 제원
	double C  = pBridge->m_dHeightTaperStt;	// EXP. JOINT에의한 단차
	double D  = pGir->GetHeightInOutUpper(TRUE);		// 출입구상단~거더상단
	double E  = pBridge->m_dHeightInOutStt;		// 출입구 높이
	BOOL bDrawInout = FALSE;
	long nFacePlateUse_Stt = pGirCen->m_pSangse->m_nFacePlateUse_Stt;
	if (nFacePlateUse_Stt != 0)
	{
		if (nDir == -1 && (nFacePlateUse_Stt == 1 || nFacePlateUse_Stt == 2))
			bDrawInout = TRUE;
		if (nDir ==  1 && (nFacePlateUse_Stt == 1 || nFacePlateUse_Stt == 3))
			bDrawInout = TRUE;
		if (nDir ==  0 && nFacePlateUse_Stt != 0)
			bDrawInout = TRUE;
	}

	// 시작부일 경우 1단
	if(pBxStt->IsState(BX_STT_GIRDER) && bDrawInout)
	{	
		if(D!=0 && E!=0)
		{
			Dom.DimBasePoint(dSta - xDis, y1 - C);				
			Dom.DimMoveTo(dSta,y1);
			Dom.DimLineTo(dSta,-C,COMMA(C));		
			Dom.DimLineTo(dSta,-D,COMMA(D));
			Dom.DimLineTo(dSta,-E,COMMA(E));
			Dom.DimLineTo(dSta,-Height+D+E,COMMA(Height - D - E));			
			xDis += Dom.Always(Dom.GetDimLevelDis());
		}
		// 시작부일 경우 2단 아닐경우 1단
		Dom.DimBasePoint(dSta - xDis, y1 - C);				
		Dom.DimMoveTo(dSta,y1);
		Dom.DimLineTo(dSta,-C,COMMA(C));		
	}
	else
	{
		// 시작부일 경우 2단 아닐경우 1단
		Dom.DimBasePoint(dSta - xDis, y1 - pGir->GetHeightFlangeUpperByTaper(dSta));	
		Dom.DimMoveTo(dSta,y1 - pGir->GetHeightFlangeUpperByTaper(dSta));			
	}
			
	double dAddLen = 0;
	CWebHStiff *pHStiff = pBx->GetWebHStiff(nDir);
	for(long i=0;i<WEBHSTIFFSUMAX;i++)
	{
		if(pHStiff->IsEmpty(i)) continue;
		double dLen = pHStiff->GetHeightHStiffOnStt(i,TRUE);
		Dom.DimLineTo(dSta,-(dLen-dAddLen),COMMA(dLen-dAddLen));
		dAddLen = dLen;
	}
	Dom.DimLineTo(dSta,dAddLen - Height,COMMA(Height-dAddLen));

	xDis += Dom.Always(Dom.GetDimLevelDis());
	Dom.SetDimDir(DIR_LEFT);
	Dom.DimBasePoint(dSta - xDis, y1);

	// 전체 높이
	if(pBxStt->IsState(BX_STT_GIRDER))
	{		
		double Dis = GetDisWebBySta(pGir, pGir->GetSSS(OFF_STT_HUNCHEND), pGir->GetSSS(OFF_STT_GIRDER), nDir);
		Dom.DimMoveTo(dSta,y1);
		Dom.DimLineTo(dSta,-pGir->GetHeightFlangeUpperByTaper(dSta), COMMA(pGir->GetHeightFlangeUpperByTaper(dSta)),"",TRUE);
	}
	else			
		Dom.DimMoveTo(dSta,y1,0,TRUE);
	
	Dom.SetDimDir(DIR_RIGHT);
	Dom.DimLineTo(dSta,-Height,COMMA(Height),"",TRUE);

	*pDom << Dom;	
	*/
}

void CAPlateDrawDetailDim::DimDetailEndJong(CDomyun *pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bRotate, BOOL bApplyJongdan, long nDir)
{	
	/*
	CARoadOptionStd*	pOpt  = m_pStd->m_pOptStd;	
	CPlateBridgeApp *pBridge  = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*			pGir  = pBxStt->GetGirder();
	CPlateGirderApp* pGirCen  = m_pStd->m_pDataManage->GetBridge()->GetGirder(-1);
	CLineInfo*			pLine = pGir->GetLine();
	CPlateBxFinder			Finder(pGir);

	double GWidth = pGir->m_dWidth;
	double SttSta = pBxStt->GetStation();
	double EndSta = pBxEnd->GetStation();
	double DisSta;		

	if(nDir == SIDE_LEFT)	DisSta = pGir->GetLengthDimType(SttSta,EndSta,-GWidth/2, TRUE, DIM_BASIC);		
	if(nDir == SIDE_CENTER)	DisSta = pGir->GetLengthDimType(SttSta,EndSta,0, TRUE, DIM_BASIC);		
	if(nDir == SIDE_RIGHT)	DisSta = pGir->GetLengthDimType(SttSta,EndSta,GWidth/2, TRUE, DIM_BASIC);				
	
	double Fixx = SttSta + DisSta;

	CDimDomyunEx Dom(pDom);	
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("RIGHT");
	Dom.SetDimDir(DIR_NONE);	
	
	if(bRotate)			Dom.SetDimTextDir(TRUE);	
	if(bApplyJongdan)	Dom.SetDimObq(ToDegree(-atan(pGir->GetGiulFlangeUpperLowerCamber(Fixx))));
	
	double Sta = Fixx;	
	CPlateBasicIndex *pBx;
	CWebHStiff *pWebHStiff = pBxEnd->GetWebHStiff(nDir);
	__int64 BxFlag = BX_PLATEHSTIFF_CENTER;

	if(nDir == SIDE_LEFT) BxFlag = BX_PLATEHSTIFF_LEFT;
	if(nDir == SIDE_RIGHT) BxFlag = BX_PLATEHSTIFF_RIGHT;

	if(!pWebHStiff) 
		pBx = Finder.GetBxLast(BxFlag,pBxEnd, pBxStt);
	else
		pBx = pBxEnd;

	if (!pBx->GetWebHStiff(nDir)) return;
	
	double Height = pGir->GetHeightGirderByStaAng(EndSta, pGir->GetWidthFlangeUpper(pBx)/2*nDir, pBx->GetAngle());	
	BOOL bExist = FALSE;
	while(pBx)
	{		
		for(long i=0;i<6;i++)
		{
			if(pBx->GetWebHStiff(nDir) && !pBx->GetWebHStiff(nDir)->IsEmpty(i)) bExist = TRUE;
		}
		if(bExist) break;
		pBx = Finder.GetBxNext();
	}
	if(!bExist) return;

	long UpperDan	= pBx->GetWebHStiff(nDir)->GetUpperDan();
	long LowerDan	= pBx->GetWebHStiff(nDir)->GetLowerDan();																
	
	double y1	= pGir->GetElevationFlangeCamber(EndSta, pGir->m_dWidth/2*nDir, CDPoint(0,1), TRUE) + pGir->GetHeightFlangeUpperByTaper(Sta);
	double y2	= pGir->GetElevationFlangeCamber(EndSta, pGir->m_dWidth/2*nDir, CDPoint(0,1), FALSE);
	double xDis = Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis());

	if(pBxEnd->IsState(BX_SPLICE)) xDis += Dom.Always(Dom.GetDimLevelDis());
	
	double C  = pBridge->m_dHeightTaperEnd;	// EXP. JOINT에의한 단차
	double D  = pGir->GetHeightInOutUpper(FALSE);		// 출입구상단~거더상단
	double E  = pBridge->m_dHeightInOutEnd;		// 출입구 높이
	BOOL bDrawInout = FALSE;
	long nFacePlateUse_End = pGirCen->m_pSangse->m_nFacePlateUse_End;
	if (nFacePlateUse_End != 0)
	{
		if (nDir == -1 && (nFacePlateUse_End == 1 || nFacePlateUse_End == 2))
			bDrawInout = TRUE;
		if (nDir ==  1 && (nFacePlateUse_End == 1 || nFacePlateUse_End == 3))
			bDrawInout = TRUE;
		if (nDir ==  0 && nFacePlateUse_End != 0)
			bDrawInout = TRUE;
	}
		
	if(pBxEnd->IsState(BX_END_GIRDER) && bDrawInout)
	{
		// 출입구 제원		
		if(D!=0 && E!=0)
		{
			Dom.DimBasePoint(Fixx + xDis, y1);		
			Dom.DimMoveTo(Fixx,y1);		
			Dom.DimLineTo(Fixx,-C,COMMA(C));
			Dom.DimLineTo(Fixx,-D,COMMA(D));
			Dom.DimLineTo(Fixx,-E,COMMA(E));		
			Dom.DimLineTo(Fixx,-Height+D+E,COMMA(Height - D - E));	
			xDis += Dom.Always(Dom.GetDimLevelDis());
		}
		Dom.DimBasePoint(Fixx + xDis, y1);	
		Dom.DimMoveTo(Fixx,y1);		
		Dom.DimLineTo(Fixx,-C,COMMA(C));	
	}		
	else
	{
		// 시작부일 경우 2단 아닐경우 1단
		Dom.DimBasePoint(Fixx + xDis, y1 - pGir->GetHeightFlangeUpperByTaper(Sta));	
		Dom.DimMoveTo(Fixx,y1 - pGir->GetHeightFlangeUpperByTaper(Sta));		
	}
	
	double dAddLen = 0;
	CWebHStiff *pHStiff = pBx->GetWebHStiff(nDir);
	for(long i=0;i<WEBHSTIFFSUMAX;i++)
	{
		if(pHStiff->IsEmpty(i)) continue;
		double dLen = pHStiff->GetHeightHStiffOnStt(i,FALSE);
		Dom.DimLineTo(Fixx,-(dLen-dAddLen),COMMA(dLen-dAddLen));
		dAddLen = dLen;
	}
	Dom.DimLineTo(Fixx,dAddLen - Height,COMMA(Height-dAddLen));
		
	xDis += Dom.Always(Dom.GetDimLevelDis());
	Dom.SetDimDir(DIR_LEFT);
	Dom.DimBasePoint(Fixx + xDis, y1);	
	if(pBxEnd->IsState(BX_END_GIRDER))
	{		
		Dom.DimMoveTo(Fixx,y1);	
		Dom.DimLineTo(Fixx,-pGir->GetHeightFlangeUpperByTaper(EndSta),COMMA(pGir->GetHeightFlangeUpperByTaper(EndSta)),"",TRUE);
	}
	else
		Dom.DimMoveTo(Fixx,y1,0,TRUE);	
	
	Dom.SetDimDir(DIR_RIGHT);
	Dom.DimLineTo(Fixx,-Height,COMMA(Height),"",TRUE);			

	*pDom << Dom;	
	*/
}

void CAPlateDrawDetailDim::DimJongByundanFactor(CDomyun *pDomP, CPlateBasicIndex *pBxStt,  CPlateBasicIndex *pBxEnd)
{
	if(pBxStt==NULL || pBxEnd==NULL)	return;

   CPlateBridgeApp *pBridge= m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir   = m_pBxStt->GetGirder();

	CPlateBxFinder Finder(pGir);
	__int64 BxFlag = BX_STT_BYUN_UPPER|BX_END_BYUN_UPPER;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);
	double Stastt = pBxStt->GetStation();	
	double Sta,R,El,Len;
	CString str;
	long nDir,nK;
	CDPoint xy;	

	CDimDomyunEx *pDom = new CDimDomyunEx(pDomP);

	while(pBx) 
	{
		Sta = pBx->GetStation();
		nK = pGir->GetNumberJiganByStation(Sta);
		R  = pGir->GetFactorByByunDan(Sta);
		if(pBridge->m_nByunCurveType[nK]==0)
			str.Format("R=%.1f",R);
		else if(pBridge->m_nByunCurveType[nK]==1)
			str.Format("A=%.8f",R);
		else
			str.Format("N=%.6f",R);

		if(pBx->IsState(BX_STT_BYUN_UPPER))
		{
			CPlateBxFinder FinderByun(pGir);
			CPlateBasicIndex *pBxByunEnd = FinderByun.GetBxLast(BX_STT_BYUN_LOWER, pBx, pBxStt);
			if(pBxByunEnd)
				Sta  = ( pBx->GetStation() + pBxByunEnd->GetStation() ) / 2;
			else
				Sta  = pBx->GetStation();
			El   = pGir->GetElevationFlangeCamber(Sta,0, CDPoint(0,1), FALSE) 
				 - pGir->GetThickJewon(G_F_L,pGir->GetBxByStation(Sta));
			nDir = 0;
			Len  = pDom->GetTextWidth(str)*1.1;
		}
		else
		{
			CPlateBxFinder FinderByun(pGir);
			CPlateBasicIndex *pBxByunStt = FinderByun.GetBxFirst(BX_END_BYUN_LOWER, pBx, pBxEnd);
			if(pBxByunStt)
				Sta  = ( pBx->GetStation() + pBxByunStt->GetStation() ) / 2;
			else
				Sta  = pBx->GetStation();
			El   = pGir->GetElevationFlangeCamber(Sta,0, CDPoint(0,1), FALSE) 
				 - pGir->GetThickJewon(G_F_L,pGir->GetBxByStation(Sta));
			nDir = 4;
			Len  = pDom->GetTextWidth(str)*-1.1;
		}
		if(pBridge->m_nByunCurveType[nK]==0) 
		{
			double dSlop = pGir->GetGiulFlangeUpperLowerCamber(Sta,0,FALSE);
			CDPoint rAng = CDPoint(cos(atan(dSlop)),sin(atan(dSlop)));
			xy.x = Stastt + pGir->GetLengthDimType(Stastt, Sta, 0, TRUE);
			xy.y = El;
			pDom->GiSiArrowExt(xy.x,xy.y,fabs(Len/4),Len,nDir,str);
			if(pBx->IsState(BX_STT_BYUN_UPPER))
				pDom->Rotate(xy,rAng.RotateInv90());
			else
				pDom->Rotate(xy,rAng.Rotate90());
			*pDomP << *pDom;
		}
		//
		pBx = Finder.GetBxNext();
	}

	delete pDom;
}

void CAPlateDrawDetailDim::DrawDimPyungBx(CDomyun *pDom, DimDataArr* pDimData, long nDir, BOOL bUpper, BOOL bLeftPos, BOOL bGen, BOOL bTextThick, BOOL bStartPage, BOOL bLastPage)
{		
	CARoadOptionStd *pOpt   = m_pStd->m_pOptStd;
	CPlateBridgeApp *pBridge= m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir   = m_pBxStt->GetGirder();
	CLineInfo       *pLine  = pGir->GetLine();	

	long nG = m_pBxStt->GetNumberGirder();
	
	// 데이타가 없을때...
	if(pDimData->GetSize()==0) return;	
	
	CDimDomyunEx Dom(pBridge,pDom);
	pOpt->SetEnvType(&Dom,HCAD_DIML); 	

	long   nDan = (bLeftPos) ? m_nUpperDan : m_nLowerDan;
	double dY = pGir->m_dGirderCenter;
	double dHeight = Dom.Always( Dom.GetDimExo()+Dom.GetDimVLen()+Dom.GetDimLevelDis()*(nDan-1) ) * (bLeftPos ? -1 : 1);
	if(bLeftPos)	Dom.SetDirection("BOTTOM");			
	else			Dom.SetDirection("TOP");							

	if(bGen)
	{
		double dSlabEnd = bLeftPos ? pBridge->m_dWidthSlabLeft : pBridge->m_dWidthSlabRight;
		long nGir = bLeftPos ? 0 : pBridge->GetGirdersu()-1;
		dY = pBridge->GetGirder(nGir)->m_dGirderCenter + (dSlabEnd) * (bLeftPos ? -1 : 1);
	}

	double dStaBxStt = m_pBxStt->GetStation();
	double dStaBxEnd = m_pBxEnd->GetStation();
	if(m_bSlabDim)
	{
		double Dist  = 0;
		double dSta  = dStaBxStt;
		CDPoint vAng = m_pBxStt->GetAngle();
		if(nG==0)
			Dist = - pBridge->m_dWidthSlabLeft;
		if(nG==pBridge->GetGirdersu()-1)
			Dist = pBridge->m_dWidthSlabRight;
		dStaBxStt = pGir->GetStationBySc(dSta,Dist,vAng);
		dSta = dStaBxEnd;
		vAng = m_pBxEnd->GetAngle();
		dStaBxEnd = pGir->GetStationBySc(dSta,Dist,vAng);	
	}
	Dom.DimBaseCurvePoint(dStaBxStt,dY + dHeight,nDan);

	CString strUpper = _T("");	
	CString strLower = _T("");

	BOOL bDimStart	  = TRUE;
	BOOL bDimContinue = TRUE;
	double NextLength = 0;	
	double StaLen = 0, LengthIn = 0, Length = 0, Length2 = 0;
	double dBaseSta = 0;
	long dNum = 0;
	double dLenMakeLowerDim = 2500*pDom->GetScaleDim()/100.0;  // 1:100을 기준으로 2500

	for(int i=0;i<pDimData->GetSize()-1;i++) 
	{	
		double dStaStt = pDimData->GetDataSta(i);
		double dStaEnd = pDimData->GetDataSta(i+1);
		dBaseSta = (dNum == 0.0) ? pDimData->GetBxSta(i,m_bSlabDim) : dBaseSta;
		strUpper.Empty();
		strLower.Empty();
		if(dStaEnd==pGir->GetSSS(OFF_END_GIRDER)) bDimContinue = FALSE;
						
		StaLen = dStaEnd - dStaStt;
		Length = GetDisFlangeBySta(pGir,dStaStt,dStaEnd,bUpper,nDir);			
		NextLength = (i < pDimData->GetSize()-2) ? GetDisFlangeBySta(pGir,pDimData->GetDataSta(i+1),pDimData->GetDataSta(i+2),bUpper,nDir) : 0;		
		if( !bTextThick && (Round(Length,0) == Round(NextLength,0)) )
		{ 
			if(dNum==0) LengthIn = Length;
			dNum++; 
			continue; 
		}
		if(pDimData->GetBxSta(i+1,m_bSlabDim) <= dStaBxStt) 	
		{
			dNum = 0;
			continue;		
		}
						
		if(dNum > 0)
		{
			double dTotalLen = LengthIn*(dNum+1);
			strUpper.Format("%d@%s=%s",dNum+1,COMMA(LengthIn),COMMA(dTotalLen));
			if(dLenMakeLowerDim>dTotalLen && !bTextThick)
			{
				strUpper.Format("%d@%s",dNum+1,COMMA(LengthIn));
				strLower.Format("=%s",COMMA(dTotalLen));
			}
		}
		else
		{
			if(bTextThick)
			{
				if(bGen && Length<5000)
				{
					strUpper.Format("%s",COMMA(Length));
					strLower.Format("(T=%.0fmm, W=%.0fmm)",pDimData->GetThickFact(i), pDimData->GetWidthFact(i));
				}
				else
					strUpper.Format("%s(T=%.0fmm, W=%.0fmm)",COMMA(Length),pDimData->GetThickFact(i), pDimData->GetWidthFact(i));
			}
			else
				strUpper = COMMA(Length);
		}

		if(pDimData->m_bUseMulti) 
		{
			Length2 = GetDisFlangeBySta(pGir,pDimData->GetSubSta(i),pDimData->GetSubSta(i+1),bUpper,nDir*(-1));			
			if(dNum > 0)
			{
				strLower.Format("(%d@%s=%s)",dNum+1,COMMA(Length2),COMMA(Length2 * (dNum+1)));
			}
			else		 
			{
				if(bTextThick)
				{
					if(bGen && Length<5000)
					{
						strUpper.Format("%s(%s)",COMMA(Length),COMMA(Length2));
						strLower.Format("(T=%.0fmm, W=%.0fmm)",pDimData->GetThickFact(i), pDimData->GetWidthFact(i));
					}
					else
						strUpper.Format("%s(%s)(T=%.0fmm, W=%.0fmm)",COMMA(Length),COMMA(Length2),pDimData->GetThickFact(i), pDimData->GetWidthFact(i));
				}
				else
					strLower = COMMA(Length2);
			}
		}						

		if(bDimStart)
		{
			if(dStaBxStt > dBaseSta) 			
				Dom.DimCurveMoveTo(dStaBxStt,dY,nG,FALSE,m_DimTitle,Dom.Always(m_dLen_Title),FALSE,TRUE);																		
			else							
				Dom.DimCurveMoveTo(dBaseSta, dY, nG,FALSE,m_DimTitle,Dom.Always(m_dLen_Title));						
			bDimStart = FALSE;
		}	
		if (!bStartPage)
		{
			double dHeadSize = Dom.GetArrowHeadSize();
			Dom.SetArrowHeadSize(0);
			Dom.DimCurveLineTo(0, dY,"","",nG);
			Dom.SetArrowHeadSize(dHeadSize);
		}

				
		double dRealLength = StaLen;
		if(dNum > 0) dRealLength = StaLen * (dNum + 1);		
		if(pDimData->GetBxSta(i+1,m_bSlabDim) > dStaBxEnd) 
		{
			if(dStaBxStt > dBaseSta)
				dBaseSta = dStaBxStt;			
			if(dStaBxEnd > dBaseSta)
				Dom.DimCurveLineTo(dStaBxEnd - dBaseSta, dY,strUpper,strLower,nG,FALSE,TRUE,FALSE,0,FALSE);										
			break;
		}
		else 
		{
			if(dBaseSta < dStaBxStt)	
			{
				if(dStaBxStt < dStaEnd)
					Dom.DimCurveLineTo(dStaEnd - dStaBxStt, dY,strUpper,strLower,nG);												
			}
			else
			{
				if(strLower=="")
				{
					if(dRealLength > 300)	// 300mm 이하의 공간은 하단에 씀.
						Dom.DimCurveLineTo(dRealLength, dY, strUpper, "", nG); 	
					else
						Dom.DimCurveLineTo(dRealLength, dY, "", strUpper, nG); 	
				}
				else
				{
						Dom.DimCurveLineTo(dRealLength, dY, strUpper, strLower, nG); 	
				}
			}
		}
		
		dNum = 0;
	}	
	if(!bLastPage)
	{
		double dLen = pGir->GetStationByActLength(dStaBxEnd, Dom.Always(20), dY) - dStaBxEnd;
		double dHeadSize = Dom.GetArrowHeadSize();
		Dom.SetArrowHeadSize(0);
		Dom.DimCurveLineTo(0, dY,"","",nG);
		Dom.SetArrowHeadSize(dHeadSize);
		Dom.DimCurveLineTo(dLen, dY,"","",nG, FALSE,FALSE,FALSE,0,TRUE);
	}
	
	if (bLeftPos) m_nUpperDan++; else m_nLowerDan++;
	*pDom << Dom;	
}

void CAPlateDrawDetailDim::DrawDimJongBx(CDomyun *pDom, DimDataArr* pDimData, long nDir, BOOL bUpper, BOOL bStud, BOOL bTextThick, BOOL bStartPage, BOOL bLastPage)
{			
	CARoadOptionStd *pOpt    = m_pStd->m_pOptStd;
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();
	CLineInfo       *pLine   = pGir->GetLine();	
	
	// 데이타가 없을때...
	if(pDimData->GetSize()==0) return;	

	CDimDomyunEx Dom(pDom);
	pOpt->SetEnvType(&Dom,HCAD_DIML);
 	Dom.SetScaleDim(pDom->GetScaleDim());		

	double dY;	
	long   nDan = (bUpper) ? m_nWebUpperDan : m_nWebLowerDan;
	double Height = Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis() * (nDan-1));		
	double dSttY = pGir->GetElevationFlangeCamber(m_pBxStt->GetStation(), 0, CDPoint(0,1), bUpper);
	double dEndY = pGir->GetElevationFlangeCamber(m_pBxEnd->GetStation(), 0, CDPoint(0,1), bUpper);
	if(bUpper)
	{
		dY = max(dSttY,dEndY);
		Dom.SetDirection("TOP");			
		Dom.DimBasePoint(m_pBxStt->GetStationByScInBx(pGir->GetWidthFlangeUpper(m_pBxStt)/2*m_nDirDimension), dY+Height);		
	}
	else
	{
		dY = min(dSttY,dEndY);
		Dom.SetDirection("BOTTOM");			
		Dom.DimBasePoint(m_pBxStt->GetStationByScInBx(pGir->GetWidthFlangeUpper(m_pBxStt)/2*m_nDirDimension), dY-Height);
	}	
			
	CString strUpper=_T(""), strLower=_T("");//, strLen2=_T("");	
	BOOL bDimStart    = TRUE;	
	BOOL bDimContinue = TRUE;
	double NextLength = 0;	
	double StaLen = 0, StaLengthIn = 0,Length = 0, Length2 = 0;
	long dNum = 0;
	double dRealLength;
	double CompareLen1, CompareLen2;
	double dLenMakeLowerDim = 2500*pDom->GetScaleDim()/100.0;  // 1:100을 기준으로 2500
	double dBaseSta  = 0.0;
	double dBaseSta2 = 0.0;
	for(int i=0;i<pDimData->GetSize()-1;i++) 
	{	
		double dStaStt  = pDimData->GetDataSta(i);
		double dStaEnd  = pDimData->GetDataSta(i+1);
		dBaseSta  = (dNum == 0) ? pDimData->GetBxSta(i) : dBaseSta;
		dBaseSta2 = (dNum == 0) ? pDimData->GetDataSta(i,m_nDirDimension) : dBaseSta2;
		strUpper.Empty();
		strLower.Empty();

		if(dStaEnd==pGir->GetSSS(OFF_END_GIRDER)) bDimContinue = FALSE;
		
		StaLen = GetDisWebBySta(pGir,dStaStt,dStaEnd,nDir);
		Length = GetDisWebBySta(pGir,dStaStt,dStaEnd,nDir);
		NextLength = (i < pDimData->GetSize()-2) ? GetDisWebBySta(pGir,pDimData->GetDataSta(i+1),pDimData->GetDataSta(i+2),nDir) : 0;		

		// 중심일 경우 Station 으로 비교한다 - 오차 방지를 위하여
		CompareLen1 = bStud ? dStaEnd - dStaStt : Length;
		if(i < pDimData->GetSize()-2) 
			CompareLen2 = bStud ? pDimData->GetDataSta(i+2) - pDimData->GetDataSta(i+1) : NextLength;
		else
			CompareLen2 = 0;

		if( !bTextThick && (COMMA(CompareLen1) == COMMA(CompareLen2)) )
		{ 
			if(dNum++ == 0) StaLengthIn = dStaStt;						
			continue; 
		}
		if(dStaEnd <= m_pBxStt->GetStation()) 	
		{
			dNum = 0;
			continue;		
		}
		
		dY = pGir->GetElevationFlangeCamber(dStaEnd,  0, CDPoint(0,1), bUpper);		
				
		if(dNum > 0) 
		{
			dRealLength = GetDisWebBySta(pGir,StaLengthIn,dStaEnd,nDir);
			strUpper.Format("%d@%s=%s",dNum+1,COMMA(dRealLength/(dNum+1)),COMMA(dRealLength));
			if(dLenMakeLowerDim>dRealLength && !bTextThick)
			{
				strUpper.Format("%d@%s",dNum+1,COMMA(dRealLength/(dNum+1)));
				strLower.Format("=%s",COMMA(dRealLength));
			}
		}
		else 
		{
			if(bTextThick)
//				strUpper.Format("%s(T=%.0fmm, W=%.0fmm)",COMMA(Length),pDimData->GetThickFact(i), pDimData->GetWidthFact(i));
				strUpper.Format("%s(T=%.0fmm)",COMMA(Length),pDimData->GetThickFact(i));
			else
				strUpper = COMMA(Length);
		}
				
		// 좌우측 동시 기입일 경우
		if(pDimData->m_bUseMulti) 
		{
			Length2 = GetDisWebBySta(pGir,pDimData->GetSubSta(i),pDimData->GetSubSta(i+1),nDir*(-1));			
			if(dNum > 0) 
			{
				dRealLength = GetDisWebBySta(pGir,StaLengthIn,dStaEnd,nDir * (-1));
				strLower.Format("(%d@%s=%s)",dNum+1,COMMA(dRealLength/(dNum+1)),COMMA(dRealLength));
			}
			else
			{
				if(bTextThick)
				{
					strUpper.Format("%s(%s)(T=%.0fmm, W=%.0fmm)",COMMA(Length),COMMA(Length2),pDimData->GetThickFact(i), pDimData->GetWidthFact(i));
				}
				else
				{
					strLower = "(" + COMMA(Length2) + ")";
				}
			}
		}						

		if(bDimStart)
		{
			if(m_pBxStt->GetStationByScInBx(0) > dBaseSta2) 			
				Dom.DimMoveTo(m_pBxStt->GetStationByScInBx(0), dSttY,0,FALSE,m_DimTitle,Dom.Always(m_dLen_Title),TRUE,FALSE);
			else							
				Dom.DimMoveTo(dBaseSta2, dSttY,0,FALSE,m_DimTitle,Dom.Always(m_dLen_Title));						

			bDimStart = FALSE;
		}	
		if (!bStartPage)
		{
			double dHeadSize = Dom.GetArrowHeadSize();
			Dom.SetArrowHeadSize(0);
			Dom.DimLineTo(0, dY,"");
			Dom.SetArrowHeadSize(dHeadSize);
		}
				
		dStaStt  = pDimData->GetDataSta(i, m_nDirDimension);
		dStaEnd  = pDimData->GetDataSta(i+1, m_nDirDimension);
		dRealLength = GetDisWebBySta(pGir,dStaStt,dStaEnd,m_nDirDimension);
		if( !bTextThick && (COMMA(CompareLen1) == COMMA(CompareLen2)) )
		{ 
			if(dNum++ == 0) StaLengthIn = dStaStt;						
			continue; 
		}
		if(pDimData->GetDataSta(i+1) <= m_pBxStt->GetStation()) 	
		{
			dNum = 0;
			continue;		
		}

		if(dNum > 0) dRealLength = GetDisWebBySta(pGir,StaLengthIn,dStaEnd,m_nDirDimension);

		if(dStaEnd > m_pBxEnd->GetStation()) 
		{
			if(m_pBxStt->GetStation() > dBaseSta2)
				dBaseSta2 = m_pBxStt->GetStation();

			dRealLength = GetDisWebBySta(pGir,dBaseSta2,m_pBxEnd->GetStation(),m_nDirDimension);

			if(pDimData->GetSize()==2)	Dom.DimLineTo(0, dY,"","");
			if(dRealLength > 0) 
				Dom.DimLineTo(dRealLength, dY,strUpper,strLower,FALSE,FALSE,0,TRUE,FALSE);										
			break;
		}
		else 
		{
			if(dBaseSta2 < m_pBxStt->GetStation())
			{			
				dRealLength = GetDisWebBySta(pGir,m_pBxStt->GetStation(),dStaEnd,m_nDirDimension);
				Dom.DimLineTo(dRealLength, dY,strUpper,strLower);										
			}
			else
			{
				if(strLower=="")
				{
					if(dRealLength > 300)	// 300mm 이하의 공간은 하단에 씀.
						Dom.DimLineTo(dRealLength, dY,strUpper,"");
					else
						Dom.DimLineTo(dRealLength, dY,"",strUpper);
				}
				else
				{
						Dom.DimLineTo(dRealLength, dY,strUpper,strLower);
				}
			}
		}

		dNum = 0;
	}	

	if(!m_pBxEnd->IsState(BX_END_GIRDER|BX_END_JOINT))
	{
		double dLen = pGir->GetStationByActLength(m_pBxEnd->GetStation(), Dom.Always(20), dY) - m_pBxEnd->GetStation();
		double dHeadSize = Dom.GetArrowHeadSize();
		Dom.SetArrowHeadSize(0);
		Dom.DimLineTo(0, dY,"");
		Dom.SetArrowHeadSize(dHeadSize);
		Dom.DimLineTo(Dom.Always(20), dY,"","",FALSE,FALSE,0,TRUE,TRUE);
	}

	if (bUpper) m_nWebUpperDan++; else m_nWebLowerDan++;

	*pDom << Dom;	
}

/************************************************************************
 * 상부 전단연결재 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeStudBx(DimDataArr* DimData)
{	
	CPlateGirderApp*			pGir = m_pBxStt->GetGirder();		

	double length = 0;
	double dSta = pGir->GetSSS(OFF_STT_GIRDER);		

	DimData->Add((CPlateBasicIndex*)NULL,dSta,dSta,dSta,dSta,dSta);		
	for (long i = 0; i < STUD_JONG_SU; i++)   
	{
		long n = pGir->m_pSangse->m_StudJong_Studsu[i];		
		length = pGir->m_pSangse->m_StudJong_StudDis[i];
		if (length < 1)
			break;
		for(long nNum=0;nNum<n;nNum++)
		{
			dSta += length;
			DimData->Add((CPlateBasicIndex*)NULL,dSta,dSta);		
		}
		if (pGir->m_pSangse->m_StudJong_Studsu[i+1] == 0)
			break;					
	}
	dSta = pGir->GetSSS(OFF_END_GIRDER);	
	DimData->Add((CPlateBasicIndex*)NULL,dSta,dSta);	
}

/************************************************************************
 * 상 하부 수평 보강재 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeVStiffBx(DimDataArr* DimData, long nDir)
{			
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
		
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | BX_VSTIFF;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);		
	while(pBx)
	{		
		double Sta  = pBx->GetStation();
		double StaL = pBx->GetStationByScInBx(0);
		double StaR = pBx->GetStationByScInBx(0);
		DimData->Add(pBx,Sta,Sta, StaL, StaR,Sta);	
		pBx = Finder.GetBxNext();		
	}
}	

/************************************************************************
 * 가로보 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeCrossBeamBx(DimDataArr* DimData, long nDir)
{	
	CPlateBridgeApp *pBridge	= m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir		= m_pBxStt->GetGirder();	

	CPlateBxFinder	Finder(pGir);			
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING;	

	if(m_bSlabDim)
		BxFlag |= (BX_STT_JOINT | BX_STT_SLAB | BX_END_JOINT | BX_END_SLAB);
//		BxFlag |= (BX_STT_SLAB |  BX_END_SLAB);

	long nG = m_pBxStt->GetNumberGirder();
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while(pBx)
	{		
		double Sta  = pBx->GetStation();
		if(m_bSlabDim && nDir!=SIDE_CENTER)
		{
			double Dist  = 0;
			if(nG==0)
				Dist = -pBridge->m_dWidthSlabLeft;
			if(nG==pBridge->GetGirdersu()-1)
				Dist = pBridge->m_dWidthSlabRight;
			Sta = pGir->GetStationBySc(pBx->GetStation(),Dist,pBx->GetAngle());
		}
		DimData->Add(pBx,Sta,pBx->GetStation(),pBx->GetStation(),pBx->GetStation(),Sta);
		pBx = Finder.GetBxNext();
	}
}

/************************************************************************
 * 현장이음 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeFieldSpliceBx(DimDataArr* DimData, long nDir)
{	
	CPlateGirderApp*			pGir = m_pBxStt->GetGirder();	
	CPlateBxFinder			Finder(pGir);			

	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE;		
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while(pBx)
	{		
		double Sta  = pBx->GetStation();
		double StaL = pBx->GetStationByScInBx(-pGir->GetWidthFlangeUpper(pBx)/2);
		double StaR = pBx->GetStationByScInBx( pGir->GetWidthFlangeUpper(pBx)/2);
		DimData->Add(pBx,Sta,Sta,StaL,StaR,Sta);	
		pBx = Finder.GetBxNext();		
	}
}

/************************************************************************
 * 공장이음 상하부 플랜지 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeFlangeBx(DimDataArr* DimData, long nDir, BOOL bUpper)
{		
	CPlateGirderApp*			pGir = m_pBxStt->GetGirder();	
	CPlateBxFinder			Finder(pGir);
			
	__int64 BxFlag = (bUpper) ? BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE :
	                            BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE ;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	

	long nMarkBuje = bUpper ? G_F_U : G_F_L;
	double dWidth = 0;

	while(pBx)
	{
		dWidth = bUpper ? pGir->GetWidthFlangeUpper(pBx) : pGir->GetWidthFlangeLower(pBx);
		double sta  = GetStaFlangeByBx(pBx,bUpper,nDir,TRUE);
		double staSub = GetStaFlangeByBx(pBx,bUpper,nDir*-1,TRUE);
		double staC = pBx->GetStation();
		double staL = pBx->GetStationByScInBx(-dWidth/2);
		double staR = pBx->GetStationByScInBx( dWidth/2);

		DimData->Add(pBx,sta, staC, staL, staR, staSub,nMarkBuje);
		pBx = Finder.GetBxNext();
	}
}	

/************************************************************************
 * 웨브 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeWebBx(DimDataArr* DimData)
{		
	CPlateGirderApp *pGir   = m_pBxStt->GetGirder();	
	CPlateBxFinder	Finder(pGir);
			
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_WEB;
	long nMarkBuje = G_W;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while(pBx)
	{
		double dSta  = pBx->GetStation();
//		double dStaL = pBx->GetBxLeft()->GetStation();
//		double dStaR = pBx->GetBxRight()->GetStation();
		DimData->Add(pBx,dSta,dSta,dSta,dSta,dSta,nMarkBuje);						 
		pBx = Finder.GetBxNext();
	}
}	

/************************************************************************
 * 상하부 플랜지 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeFlangeSpliceBx(DimDataArr* DimData, long nDir, BOOL bUpper)
{		
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();	
	CPlateBxFinder	Finder(pGir);
			
	__int64 BxFlag = (bUpper) ? BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_UPPERFLANGE :
	                            BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_LOWERFLANGE ;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	long nMarkBuje = bUpper ? G_F_U : G_F_L;

	while(pBx)
	{
		double dSta  = pBx->GetStation();
		DimData->Add(pBx,dSta,dSta,dSta,dSta,dSta,nMarkBuje);						 
		pBx = Finder.GetBxNext();
	}
}	

/************************************************************************
 * 웨브&공장이음 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeWebSpliceBx(DimDataArr* DimData)
{		
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();	
	CPlateBxFinder	Finder(pGir);
	
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE | BX_WEB;	                 

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while(pBx)
	{
		double dSta  = pBx->GetStation();
		DimData->Add(pBx,dSta,dSta,dSta,dSta,dSta,G_W);						 
		pBx = Finder.GetBxNext();
	}
}	

/************************************************************************
 * 변단면 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeByunBx(DimDataArr* DimData, long nDir, BOOL bUpper)
{
	if(bUpper) return;	
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();	
	CPlateBxFinder	Finder(pGir);
	
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | 
					 BX_STT_BYUN_LOWER | BX_END_BYUN_LOWER| 
					 BX_STT_BYUN_UPPER | BX_END_BYUN_UPPER;

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	

	while(pBx)
	{
		double dSta = pBx->GetStation();
		DimData->Add(pBx,dSta,dSta,dSta,dSta,dSta);
		pBx = Finder.GetBxNext();
	}
}

/************************************************************************
 * Tendon구성 위치점 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeTendonBx(DimDataArr* DimData, long nDir)
{	
	CPlateBridgeApp *pBridge = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();	
	CPlateBxFinder	Finder(pGir);
	
	CPlateBasicIndex *pBx = NULL;

	long nTendon = pBridge->m_arrTendon.GetSize(); 
	double dSta  = 0;
	double dStaSttSlab = pGir->GetSSS(OFF_STT_SLAB);	// 슬래브 시작 스테이션
	double dStaEndSlab = pGir->GetSSS(OFF_END_SLAB);	// 슬래브 끝   스테이션 
	DimData->Add(pBx, dStaSttSlab, dStaSttSlab, dStaSttSlab, dStaSttSlab, dStaSttSlab);
	for(long n=0; n<nTendon; n++)
	{
		dSta = pBridge->m_arrTendon.GetAt(n).x;	
		DimData->Add(pBx, dSta, dSta, dSta, dSta, dSta);
	}
	DimData->Add(pBx, dStaEndSlab, dStaEndSlab, dStaEndSlab, dStaEndSlab, dStaEndSlab);

}

/************************************************************************
 * 지간구성 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeJiGanBx(DimDataArr* DimData, long nDir)
{	
	CPlateBridgeApp *pBridge = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();	
	CPlateBxFinder	Finder(pGir);
	
	__int64 BxFlag  = BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING;
	__int64 BxState = BX_STT_GIRDER | BX_END_GIRDER;

	if(m_bSlabDim)
	{
		BxFlag  = BX_STT_JOINT | BX_END_JOINT | BX_CROSSBEAM_VBRACING;
		BxState = BX_STT_JOINT | BX_END_JOINT;
//		BxFlag  = BX_STT_SLAB | BX_END_SLAB | BX_CROSSBEAM_VBRACING;
//		BxState = BX_STT_SLAB | BX_END_SLAB;
	}

	long nG = m_pBxStt->GetNumberGirder();
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while(pBx)
	{
		if(pBx->IsPier() || pBx->IsState(BxState))
		{			
			double dSta = pBx->GetStation();
			double dStaC = pBx->GetStation();
			double dStaL = pBx->GetStationByScInBx(-pGir->GetWidthFlangeUpper(pBx)/2);
			double dStaR = pBx->GetStationByScInBx( pGir->GetWidthFlangeUpper(pBx)/2);
			if(m_bSlabDim && nDir!=SIDE_CENTER)
			{
				double Dist  = 0;
				if(nG==0)
					Dist = -pBridge->m_dWidthSlabLeft;
				if(nG==pBridge->GetGirdersu()-1)
					Dist = pBridge->m_dWidthSlabRight;
				dSta = pGir->GetStationBySc(pBx->GetStation(),Dist,pBx->GetAngle());
			}
			DimData->Add(pBx,dSta,dStaC,dStaL,dStaR,dSta);
		}
		pBx = Finder.GetBxNext();
	}
}

/************************************************************************
 * 총길이 위치점 Bx 및 Station 생성
 *************************************************************************/
void CAPlateDrawDetailDim::MakeTotalBx(DimDataArr* DimData, long nDir)
{		
	CPlateGirderApp *pGir    = m_pBxStt->GetGirder();	
	CPlateBxFinder Finder(pGir);
	
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER;
	if(m_bSlabDim)
		BxFlag = BX_STT_SLAB | BX_END_SLAB;
		//BxFlag = BX_STT_JOINT | BX_END_JOINT;
	
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);	
	while(pBx)
	{
		double dSta = pBx->GetStation();
		DimData->Add(pBx,dSta,dSta,dSta,dSta,dSta);
		pBx = Finder.GetBxNext();
	}
}

void CAPlateDrawDetailDim::MakeBendingDimPyungBx(CPlateBasicIndex *pBxStt,CPlateBasicIndex *pBxEnd,DimDataArr* DimData, long nDir, BOOL bUpper)
{	
	/*
	CPlateBridgeApp *pBridge	= m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*			pGir = pBxStt->GetGirder();	
	CPlateBxFinder Finder(pGir);
	
	__int64 BxFlag = (nDir==SIDE_LEFT) ? BX_STT_GIRDER | BX_END_GIRDER | BX_BENDING_LEFT :
										 BX_STT_GIRDER | BX_END_GIRDER | BX_BENDING_RIGHT;
	CPlateBasicIndex *pBx = nDir == SIDE_LEFT ?  Finder.GetBxFirst(BxFlag,pBxStt->GetBxLeft(),pBxEnd->GetBxLeft()) :
	 										Finder.GetBxFirst(BxFlag,pBxStt->GetBxRight(),pBxEnd->GetBxRight());
	while(pBx)
	{
		DimData->Add(pBx,GetStaFlangeByBx(pBx->GetBxCenter(),bUpper,nDir,TRUE)
						,GetStaFlangeByBx(pBx->GetBxCenter(),bUpper,-1*nDir,TRUE));
		pBx = Finder.GetBxNext();
	}
	*/
}

void CAPlateDrawDetailDim::MakeLowerBendingDimPyungBx(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, DimDataArr* DimData, long nDir)
{		
	CPlateGirderApp*			pGir = pBxStt->GetGirder();	
	CPlateBxFinder		Finder(pGir);

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_BYUN_LOWER|BX_END_BYUN_LOWER|BX_VSTIFF|BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	BOOL bFind = FALSE;

	while(pBx)
	{
		if(pBx->IsJijum() || pBx->IsState(BX_STT_BYUN_LOWER|BX_END_BYUN_LOWER))
		{
			if(pBx->IsState(BX_STT_BYUN_LOWER|BX_END_BYUN_LOWER)) bFind = TRUE;
			DimData->Add(pBx,GetStaFlangeByBx(pBx,FALSE,nDir,TRUE),
							 GetStaFlangeByBx(pBx,FALSE,-1*nDir,TRUE));
		}
		pBx = Finder.GetBxNext();
	}
	if(!bFind) DimData->RemoveAll();
}	

void CAPlateDrawDetailDim::DimDetailMLMarkPyung(CDomyun *pDom, long nNum, BOOL bUpper, long nUpperDan, long nLowerDan)
{	
	CPlateBridgeApp*	pDB	 = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*	pGir = (CPlateGirderApp*)m_pBxStt->GetGirder();	
	CARoadOptionStd*	pOpt = m_pStd->m_pOptStd;

	long nUDan = (nUpperDan == -1) ? m_nUpperDan - 1 : nUpperDan;
	long nLDan = (nLowerDan == -1) ? m_nLowerDan - 1 : nLowerDan;

	CSymbolDom Dom(pDom, m_pStd->m_pOptStd);
	double dBaseHeight = Dom.Always(Dom.GetDimExo());
	double dTopHeight  = Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis() * (nUDan));		
//	double dBottomHeight  = Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis() * (nLDan-1));		
	double dBottomHeight  = 0;

	double dWidthStt	= pGir->GetWidthOnStation(m_pBxStt->GetStation(), bUpper) / 2;
	double dWidthEnd	= pGir->GetWidthOnStation(m_pBxEnd->GetStation(), bUpper) / 2;
	if(bUpper && pDB->IsTUGir())
	{
		dWidthStt = pDB->m_BindConc.m_dWidth / 2;
		dWidthEnd = pDB->m_BindConc.m_dWidth / 2;
	}
	
	CDPoint BaseP1  = pGir->GetXyGirderDis(m_pBxStt->GetStation(),-dWidthStt-dBaseHeight);
	CDPoint UpperP1 = pGir->GetXyGirderDis(m_pBxStt->GetStation(),-dWidthStt-dTopHeight);
	CDPoint BaseP2  = pGir->GetXyGirderDis(m_pBxStt->GetStation(),+dWidthStt+dBaseHeight);
	CDPoint UpperP2 = pGir->GetXyGirderDis(m_pBxStt->GetStation(),+dWidthStt+dBottomHeight);
	CDPoint vAng1	= pGir->GetLine()->GetAngleAzimuth(m_pBxStt->GetStation());

	CDPoint BaseP3  = pGir->GetXyGirderDis(m_pBxEnd->GetStation(),-dWidthEnd-dBaseHeight);
	CDPoint UpperP3 = pGir->GetXyGirderDis(m_pBxEnd->GetStation(),-dWidthEnd-dTopHeight);
	CDPoint BaseP4  = pGir->GetXyGirderDis(m_pBxEnd->GetStation(),+dWidthEnd+dBaseHeight);
	CDPoint UpperP4 = pGir->GetXyGirderDis(m_pBxEnd->GetStation(),+dWidthEnd+dBottomHeight);
	CDPoint vAng2	= pGir->GetLine()->GetAngleAzimuth(m_pBxEnd->GetStation());
		
	pOpt->SetEnvType(&Dom, HCAD_DIML);
	CString szText;
	szText.Format("M.L%d",nNum);
	double TextHeight = pDom->GetTextHeight()*2.2;

	if(!m_pBxStt->IsState(BX_STT_GIRDER))
	{
		Dom.LineTo(UpperP1,UpperP2);
		UpperP1 = pGir->GetXyGirderDis(m_pBxStt->GetStation(),-dWidthStt-dTopHeight-TextHeight/2);
		Dom.DrawSymbolHexagonStr(UpperP1,vAng1,szText);
	}
	szText.Format("M.L%d",nNum+1);
	if(!m_pBxEnd->IsState(BX_END_GIRDER))
	{
		Dom.LineTo(UpperP3,UpperP4);
		UpperP3 = pGir->GetXyGirderDis(m_pBxEnd->GetStation(),-dWidthEnd-dTopHeight-TextHeight/2);
		Dom.DrawSymbolHexagonStr(UpperP3,vAng2,szText);
	}
	*pDom << Dom;
}

void CAPlateDrawDetailDim::DimDetailMLMarkJong(CDomyun *pDom, long nNum, long nDir)
{	
	CPlateGirderApp *pGir = m_pBxStt->GetGirder();	
	CARoadOptionStd *pOpt = m_pStd->m_pOptStd;

	double dSttY = pGir->GetElevationFlange(m_pBxStt->GetStation());
	double dEndY = pGir->GetElevationFlange(m_pBxEnd->GetStation());		
	double dMaxEL  = pGir->GetMaxPlanEL(m_pBxStt, m_pBxEnd);

	CSymbolDom Dom(pDom, m_pStd->m_pOptStd);
	double dBaseHeight = 0;
	double dTopHeight  = Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis() * (m_nWebUpperDan - 1));		
	double TextHeight = Dom.GetTextHeight()*2.2;
		
	pOpt->SetEnvType(&Dom, HCAD_DIML);
	CString szText;
	szText.Format("M.L%d",nNum);
	if(!m_pBxStt->IsState(BX_STT_GIRDER))
	{
		dBaseHeight = m_pBxStt->GetHeightGirderByBx();
		Dom.LineTo(m_pBxStt->GetStation(),dSttY - dBaseHeight, m_pBxStt->GetStation(), dSttY + dTopHeight+(dMaxEL-dSttY));
		Dom.DrawSymbolHexagonStr(CDPoint(m_pBxStt->GetStation(),dSttY + dTopHeight+(dMaxEL-dSttY)+TextHeight/2),CDPoint(0,0),szText);
	}
	szText.Format("M.L%d",nNum+1);
	if(!m_pBxEnd->IsState(BX_END_GIRDER))
	{
		double dDis = GetDisWebBySta(pGir,m_pBxStt->GetStation(),m_pBxEnd->GetStation(),nDir);
		dBaseHeight = m_pBxEnd->GetHeightGirderByBx();
		Dom.LineTo(m_pBxStt->GetStation() + dDis,dEndY - dBaseHeight, m_pBxStt->GetStation() + dDis, dEndY + dTopHeight+(dMaxEL-dEndY));
		Dom.DrawSymbolHexagonStr(CDPoint(m_pBxStt->GetStation() + dDis, dEndY + dTopHeight+(dMaxEL-dEndY)+TextHeight/2),CDPoint(0,0),szText);
	}
	*pDom << Dom;
}

void CAPlateDrawDetailDim::DimDetailSttPyung(CDomyun *pDom, CPlateBasicIndex *pBxStt)
{
	/*
	CARoadOptionStd* pOpt = m_pStd->m_pOptStd;
	CPlateBridgeApp *pBridge	= m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*		pGir  = pBxStt->GetGirder();
	CLineInfo*		pLine = pGir->GetLine();		
	long			nG	  = pBxStt->GetNumberGirder();

	CString csFmt;
	double width,RibDis,LeftSharp, RightSharp;
	
	CDimDomyunEx Dom(pBridge,pDom);	
	pOpt->SetEnvType(&Dom,HCAD_DIML);		
	Dom.SetDirection("LEFT");			
	
	double Fixx = pBxStt->GetStation();
	CDPoint vAng = pBxStt->GetAngle();
	if(pBxStt->IsAbut()) 
	{
		Fixx = pGir->GetSSS(OFF_STT_GIRDER);
		vAng = pGir->GetAAA(OFF_STT_GIRDER);
	}	
	
	double xDis = -Dom.Always(Dom.GetDimExo() + Dom.GetDimLevelDis());
	////////////////////// 살려야 함
//	if (pStd->m_pDomOptionStd->m_Det_nDivPage == 1)
//	{
//		CPlateSplice *pSp = pGir->GetBxByStation(pBxStt->GetStation(), BX_SPLICE)->GetSplice();
//		xDis -= (pSp->m_uA + pSp->m_uB * pSp->m_uC + pSp->m_uC1/2);
//	}
	for(long i=0;i<2;i++)
	{
		LeftSharp  = (i==0) ? pGir->m_dLeftSharp  : pGir->m_dULeftSharp;
		RightSharp = (i==0) ? pGir->m_dRightSharp : pGir->m_dURightSharp;
		width	 = pGir->m_dGirderCenter + pGir->m_dWidth/2 + RightSharp;

		if(i==0)
		{
			if(pBxStt->GetAngle().GetAngleRadian() > ConstPi/2) 
				Fixx = pGir->GetStationBySc(Fixx,-pGir->m_dWidth/2-LeftSharp,vAng);
			else
				Fixx = pGir->GetStationBySc(Fixx,pGir->m_dWidth/2+RightSharp,vAng);			
		}
		
		Dom.DimBaseCurvePoint(xDis, width);			
		Dom.DimCurveMoveTo(Fixx,width,nG);	

		CJRib*	pRib  = pBxStt->GetJRib(i==0 ? FALSE : TRUE);
		RibDis = pRib->GetDisBetweenJRib(pRib->Getsu());

		Dom.DimCurveLineTo(Fixx,-RightSharp,COMMA(RightSharp),"",nG);
		long	cnt = 1,n = -1;
		for(long m = pRib->Getsu()-1;m >= 0;m--)
		{
			if( RibDis == pRib->GetDisBetweenJRib(m) )
			{
				cnt++;n = m;
				continue;
			}
			else
			{	
				if( cnt <= 1 )
				{
					csFmt.Format("%s",COMMA(RibDis));
					Dom.DimCurveLineTo(Fixx,-RibDis,csFmt,"",nG);
					cnt = 1;
				}
				else
				{
					csFmt.Format("%d@%s=%s",cnt,COMMA(RibDis),COMMA(RibDis*cnt));
					Dom.DimCurveLineTo(Fixx,-RibDis*cnt,csFmt,"",nG);
					cnt = 1;
				}
			}
			RibDis = pRib->GetDisBetweenJRib(m);
		}
		if( n != 0){
			csFmt.Format("%s",COMMA(pRib->GetDisBetweenJRib(0)));
			Dom.DimCurveLineTo(Fixx,-pRib->GetDisBetweenJRib(0),csFmt,"",nG);
		}
		else
		{
			csFmt.Format("%d@%s=%s",cnt,COMMA(RibDis),COMMA(RibDis*cnt));
			Dom.DimCurveLineTo(Fixx,-RibDis*cnt,csFmt,"",nG);
		}
		Dom.DimCurveLineTo(Fixx,-LeftSharp,COMMA(LeftSharp),"",nG);
		xDis -= Dom.Always(Dom.GetDimLevelDis());
	}	
	LeftSharp  = pGir->m_dLeftSharp;
	RightSharp = pGir->m_dRightSharp;
	width	 = pGir->m_dGirderCenter + pGir->m_dWidth/2 + RightSharp;
	Dom.DimBaseCurvePoint(xDis, width);			
	Dom.DimCurveMoveTo(Fixx,width,nG);		
	Dom.DimCurveLineTo(Fixx,-(pGir->m_dWidth+LeftSharp+RightSharp),COMMA(pGir->m_dWidth+LeftSharp+RightSharp),"",nG);
	*pDom << Dom;	
	*/
}

void CAPlateDrawDetailDim::DimDetailEndPyung(CDomyun *pDom, CPlateBasicIndex *pBxEnd)
{
	/*
	CARoadOptionStd* pOpt = m_pStd->m_pOptStd;
	CPlateBridgeApp *pBridge	= m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp*		pGir  = pBxEnd->GetGirder();
	CLineInfo*		pLine = pGir->GetLine();	
	long			nG	  = pBxEnd->GetNumberGirder();
	CString csFmt;
	double width,RibDis,LeftSharp, RightSharp;
	
	CDimDomyunEx Dom(pBridge,pDom);	
	pOpt->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("RIGHT");
	
	double Fixx = pBxEnd->GetStation();
	CDPoint vAng = pBxEnd->GetAngle();
	if(pBxEnd->IsAbut()) 
	{
		Fixx = pGir->GetSSS(OFF_END_GIRDER);
		vAng = pGir->GetAAA(OFF_END_GIRDER);
	}
	
	double xDis = Dom.Always(Dom.GetDimExo() + Dom.GetDimLevelDis());
	////////////// 살려야함
//	if (pStd->m_pDomOptionStd->m_Det_nDivPage == 1)
//	{
//		CPlateSplice *pSp = pGir->GetBxByStation(pBxEnd->GetStation(), BX_SPLICE)->GetSplice();
//		xDis += (pSp->m_uA + pSp->m_uB * pSp->m_uC + pSp->m_uC1/2);
//	}
	for(long i=0;i<2;i++)
	{
		LeftSharp  = (i==0) ? pGir->m_dLeftSharp  : pGir->m_dULeftSharp;
		RightSharp = (i==0) ? pGir->m_dRightSharp : pGir->m_dURightSharp;
		width	 = pGir->m_dGirderCenter + pGir->m_dWidth/2 + RightSharp;

		if(i==0)
		{
			if(pBxEnd->GetAngle().GetAngleRadian() > ConstPi/2) 
				Fixx = pGir->GetStationBySc(Fixx,pGir->m_dWidth/2+LeftSharp,vAng);
			else
				Fixx = pGir->GetStationBySc(Fixx,-pGir->m_dWidth/2-RightSharp,vAng);
		}
	
		Dom.DimBaseCurvePoint(xDis, width);			
		Dom.DimCurveMoveTo(Fixx,width,nG);	

		CJRib*	pRib  = pBxEnd->GetJRib(i==0 ? FALSE : TRUE);
		RibDis = pRib->GetDisBetweenJRib(pRib->Getsu());

		Dom.DimCurveLineTo(Fixx,-RightSharp,COMMA(RightSharp),"",nG);
		long	cnt = 1,n = -1;
		for(long m = pRib->Getsu()-1;m >= 0;m--)
		{
			if( RibDis == pRib->GetDisBetweenJRib(m) )
			{
				cnt++;n = m;
				continue;
			}
			else
			{	
				if( cnt <= 1 )
				{
					csFmt.Format("%s",COMMA(RibDis));
					Dom.DimCurveLineTo(Fixx,-RibDis,csFmt,"",nG);
					cnt = 1;
				}
				else
				{
					csFmt.Format("%d@%s=%s",cnt,COMMA(RibDis),COMMA(RibDis*cnt));
					Dom.DimCurveLineTo(Fixx,-RibDis*cnt,csFmt,"",nG);
					cnt = 1;
				}
			}
			RibDis = pRib->GetDisBetweenJRib(m);
		}
		if( n != 0){
			csFmt.Format("%s",COMMA(pRib->GetDisBetweenJRib(0)));
			Dom.DimCurveLineTo(Fixx,-pRib->GetDisBetweenJRib(0),csFmt,"",nG);
		}
		else
		{
			csFmt.Format("%d@%s=%s",cnt,COMMA(RibDis),COMMA(RibDis*cnt));
			Dom.DimCurveLineTo(Fixx,-RibDis*cnt,csFmt,"",nG);
		}
		Dom.DimCurveLineTo(Fixx,-LeftSharp,COMMA(LeftSharp),"",nG);
		xDis += Dom.Always(Dom.GetDimLevelDis());
	}
	*pDom << Dom;
	*/
}


// 시종점 단면의 치수
void CAPlateDrawDetailDim::DimJongSttEndSideBx(CDomyun* pDom,  CPlateBasicIndex *pBxStt, 
										  CPlateBasicIndex *pBxEnd, BOOL bBothSide)
{
	if(pBxStt==NULL || pBxEnd==NULL)	return;
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp	*pGir = pBxStt->GetGirder();

	CDimDomyunEx Dom(pDom);
	m_pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double dStdSta = pBxStt->GetStation();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_CROSSBEAM_VBRACING | BX_SPLICE, pBxStt, pBxEnd);
	if(pBx) 
	{
		CDPoint xy;
		xy.x = dStdSta + pGir->GetLengthDimType(dStdSta,pBx->GetStation());
		xy.y = pGir->GetElevationFlangeCamber(pBx->GetStation())
			 + pGir->GetHeightFlangeUpperByTaper(pBx->GetStation());
		//

		double C = 0;
		if(pBx->IsState(BX_STT_GIRDER))
			C = pStd->GetDataManage()->GetBridge()->m_dHeightTaperStt;

		double H = pGir->GetHeightGirderByStaAng(pBx->GetStation(), 0, CDPoint(0,1), FALSE);
		long DimDan = 2;		
		Dom.SetDirection("LEFT");

		if(pBx->IsState(BX_STT_GIRDER) && C!=0)
		{
			Dom.DimMoveTo(xy.x,xy.y-H-C,DimDan++); 
			Dom.DimLineTo(xy.x,H,COMMA(H));
			Dom.DimLineTo(xy.x,C,COMMA(C),"",FALSE,TRUE);
		}
		// TOTAL
		Dom.DimMoveTo(xy.x,xy.y-H-C,DimDan++);
		Dom.DimLineTo(xy.x,C+H,COMMA(C+H));

		*pDom << Dom;
		if(!bBothSide) return;
	}

	pBx = Finder.GetBxLast(BX_END_GIRDER | BX_CROSSBEAM_VBRACING | BX_SPLICE, pBxEnd, pBxStt);
	if(pBx)
	{
		CDPoint xy;
		xy.x = dStdSta + pGir->GetLengthDimType(dStdSta,pBx->GetStation());
		xy.y = pGir->GetElevationFlangeCamber(pBx->GetStation())
			 + pGir->GetHeightFlangeUpperByTaper(pBx->GetStation());
		//
		double C = 0;
		if(pBx->IsState(BX_END_GIRDER))
			C = pStd->GetDataManage()->GetBridge()->m_dHeightTaperEnd;

		double H = pGir->GetHeightGirderByStaAng(pBx->GetStation(), 0, CDPoint(0,1), FALSE);
		long DimDan = 2;		
		Dom.SetDirection("RIGHT");
		if(pBx->IsState(BX_END_GIRDER) && C!=0)
		{
			Dom.DimMoveTo(xy.x,xy.y-H-C,DimDan++);
			Dom.DimLineTo(xy.x,H,COMMA(H));
			Dom.DimLineTo(xy.x,C,COMMA(C),"",FALSE,TRUE);
		}
		// TOTAL
		Dom.DimMoveTo(xy.x,xy.y-H-C,DimDan++); 
		Dom.DimLineTo(xy.x,C+H,COMMA(C+H));
		
		*pDom << Dom;
	}

}


// 시종점 단면의 치수
void CAPlateDrawDetailDim::DimJongTaperBx(CDomyun* pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(pBxStt==NULL || pBxEnd==NULL)	return;

	CPlateBridgeApp *pBridge = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp	*pGir    = pBxStt->GetGirder();

	CPlateBasicIndex *pBxSttJoint = pGir->GetBxOnJijum(0);
	CPlateBasicIndex *pBxEndJoint = pGir->GetBxOnJijum(pBridge->m_nQtyJigan);
	double dTFlangeStt = pGir->GetThickFactByStation(pBxSttJoint->GetStation(), G_F_U);
	double dTFlangeEnd = pGir->GetThickFactByStation(pBxEndJoint->GetStation(), G_F_U);

	long nBeingTaper = 0;
	if((pBxStt->GetStation() < pBxSttJoint->GetStation() && pBxSttJoint->GetStation() < pBxEnd->GetStation()) 
		&& pBridge->m_dHeightTaperStt > 0)
		nBeingTaper = 0;	// 시점부 테이퍼
	else if((pBxStt->GetStation() < pBxEndJoint->GetStation() && pBxEndJoint->GetStation() < pBxEnd->GetStation())
		&& pBridge->m_dHeightTaperEnd > 0)
		nBeingTaper = 1;	// 종점부 테이퍼
	else	
		nBeingTaper = 2;	// 테이퍼 없음.

	if(nBeingTaper==2)	return;

	CDomyun Dom(pDom);
	m_pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);

	if(nBeingTaper==0)
	{
		Dom.SetDirection("TOP");
		CDPoint xy;
		xy.x = pGir->GetSSS(OFF_STT_GIRDER);
		xy.y = pGir->GetElevationFlangeCamber(pBxSttJoint->GetStation()) + dTFlangeStt + pBridge->m_dHeightTaperStt;

		Dom.DimMoveTo(xy.x, xy.y, 0); 
		Dom.DimLineTo(pBridge->m_dLengthGirderShoeStt, 0, COMMA(pBridge->m_dLengthGirderShoeStt));
		Dom.DimLineTo(pBridge->m_dLengthShoeToTaperStt, 0, COMMA(pBridge->m_dLengthShoeToTaperStt));
		Dom.DimLineTo(pBridge->m_dLengthTaperToTaperStt, 0, COMMA(pBridge->m_dLengthTaperToTaperStt));
	}	
	else if(nBeingTaper==1)
	{
		Dom.SetDirection("TOP");
		CDPoint xy;
		xy.x = pGir->GetSSS(OFF_END_GIRDER)-pBridge->m_dLengthGirderShoeEnd-pBridge->m_dLengthShoeToTaperEnd-pBridge->m_dLengthTaperToTaperEnd;
		xy.y = pGir->GetElevationFlangeCamber(pBxEndJoint->GetStation()) + dTFlangeEnd + pBridge->m_dHeightTaperEnd;

		Dom.DimMoveTo(xy.x, xy.y, 0); 
		Dom.DimLineTo(pBridge->m_dLengthTaperToTaperEnd, 0, COMMA(pBridge->m_dLengthTaperToTaperEnd));
		Dom.DimLineTo(pBridge->m_dLengthShoeToTaperEnd, 0, COMMA(pBridge->m_dLengthShoeToTaperEnd));
		Dom.DimLineTo(pBridge->m_dLengthGirderShoeEnd, 0, COMMA(pBridge->m_dLengthGirderShoeEnd));
	}
	*pDom << Dom;
}


// 수직보강재위치에 단면 MARK
void CAPlateDrawDetailDim::DimJongSectionMark(CDomyun *pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bSlabBase, double dOffsetHeight, BOOL bSameSize)
{
	CPlateGirderApp *pGir    = pBxStt->GetGirder();

	CDomyun Dom(pDom);
	CDPoint CP  = CDPoint(0,0);
	CString str = _T("");

	__int64 BxFlag = BX_CROSSBEAM_VBRACING|BX_VSTIFF;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);

	double stdSta = pBxStt->GetStation();
	long   nEaStr = bSameSize ? -1 : 4;
	while(pBx) 
	{
		double dOffDist = dOffsetHeight;
		if(bSlabBase) 
			dOffDist += pGir->GetThickSlabPave(pBx->GetStation());
		str = pBx->GetMarkString();
		if(bSameSize && str.GetLength()>2)
		{
			if(str.GetLength()==3)	Dom.SetTextWidthFactor(0.80);
			if(str.GetLength()==4)	Dom.SetTextWidthFactor(0.65);
			if(str.GetLength()>4)	Dom.SetTextWidthFactor(0.55);
		}
		else
			Dom.SetTextWidthFactor(1.0);

		CP.x = pGir->GetLengthDimType(stdSta, pBx->GetStation())+ stdSta;
		CP.y = pGir->GetElevationFlangeCamber(pBx->GetStation())+ dOffDist + Dom.Always(5);

		Dom.TextCircle(CP.x,CP.y,str,nEaStr);
		pBx = Finder.GetBxNext();
	}

	*pDom << Dom;
}

void CAPlateDrawDetailDim::DimJongJackUpMark(CDomyun *pDom,  CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
 	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	CARoadOptionStd *pOpt    = m_pStd->m_pOptStd;

	CSymbolDom	Dom(pDom, pOpt);
	CDPoint CP  = CDPoint(0,0);
	CString str = _T("");
	pOpt->SetEnvType(&Dom, HCAD_SYMB);
//	Dom.SetScaleDim(m_Scale);

	double stdSta = pBxStt->GetStation();
	long nJStt = pGir->GetNumberJijumByBx(pBxStt);
	long nJEnd = pGir->GetNumberJijumByBx(pBxEnd);
	for(long nJ=nJStt; nJ<=nJEnd; nJ++)
	{
//		EXIT_RTN;
		CPlateBasicIndex *pBxJack = pGir->GetJackupBxByJijumBx(pGir->GetBxOnJijum(nJ),-3,0);
		if(pBxJack==NULL) pBxJack = pGir->GetJackupBxByJijumBx(pGir->GetBxOnJijum(nJ),-2,0);
		if(pBxJack==NULL) pBxJack = pGir->GetJackupBxByJijumBx(pGir->GetBxOnJijum(nJ),-1,0);
		if(pBxJack)
		{
			str  = "수정";//pDB->GetMarkSection(SECTION_MAIN,pBxJack);
			CP.x = pGir->GetLengthDimType(stdSta,pBxJack->GetStation())
				 + stdSta;
			CP.y = pGir->GetElevationFlangeCamber(pBxJack->GetStation())+Dom.Always(20);
			Dom.SymbolTextCircle(CP,str);
		}
		pBxJack = pGir->GetJackupBxByJijumBx(pGir->GetBxOnJijum(nJ),+3,0);
		if(pBxJack==NULL) pBxJack = pGir->GetJackupBxByJijumBx(pGir->GetBxOnJijum(nJ),+2,0);
		if(pBxJack==NULL) pBxJack = pGir->GetJackupBxByJijumBx(pGir->GetBxOnJijum(nJ),+1,0);
		if(pBxJack)
		{
			str  = "수정";//pDB->GetMarkSection(SECTION_MAIN,pBxJack);
			CP.x = pGir->GetLengthDimType(stdSta,pBxJack->GetStation())
				 + stdSta;
			CP.y = pGir->GetElevationFlangeCamber(pBxJack->GetStation())+Dom.Always(20);
			Dom.SymbolTextCircle(CP,str);
		}
	}

	*pDom << Dom;
}

void CAPlateDrawDetailDim::DimTextJongTotal(CDomyun* pDom, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	COptionStd	    *pDomOpt = m_pStd->GetDomOptionStd();	 	

	BOOL bJewonWebBuje		= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_WEB_NAME)			? TRUE : FALSE;
	BOOL bJewonWebMat		= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_WEB_MAT)			? TRUE : FALSE;
	BOOL bJewonHStiffBuje	= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_HSTIFF_NAME)		? TRUE : FALSE;
	BOOL bJewonHStiffMat	= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_HSTIFF_MAT)		? TRUE : FALSE;

	// 응력구간
	if(pDomOpt->m_Gen_bForceText)
		DimTextJongForce(pDom, pBxStt, pBxEnd);

	// 수평보강재
	if(bJewonHStiffBuje || bJewonHStiffMat)
		DimTextJongHStiff(pDom, pBxStt, pBxEnd);

	// 측판
	if(bJewonWebBuje || bJewonWebMat)
		DimTextJongWeb(pDom, pBxStt, pBxEnd);

	// 변단면 계수 출력
	if(pDomOpt->m_Gen_bByundan)
		DimJongByundanFactor(pDom, pBxStt, pBxEnd);
}


// 각구간의 중심에 표현
void CAPlateDrawDetailDim::DimTextJongForce(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
  	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	CARoadOptionStd *pOpt    = m_pStd->m_pOptStd;
	COptionStd	    *pDomOpt = m_pStd->GetDomOptionStd();	 	

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_VSTIFF, pBxStt, pBxEnd);

	long nCompressPre = pBx->GetCompressTypeByComplex(); // 1-상부압축,2-하부압축,3-교번부
	long nCompressCur = pBx->GetCompressTypeByComplex();
	double StaPre     = pBx->GetStation();
	double StaStt     = pBxStt->GetStation();

	CDomyun Dom(pDomP);
	pOpt->SetEnvType(&Dom, HCAD_SMALL_TITLE);

	CString sText;
	CDPoint xy;
	while(pBx)
	{
		nCompressCur = pBx->GetCompressTypeByComplex();
		if(nCompressCur!=nCompressPre)
		{
			if(nCompressPre==1)	sText = "- 압 축 구 간 -";
			if(nCompressPre==2)	sText = "- 인 장 구 간 -";
			if(nCompressPre==3)	sText = "- 교 번 구 간 -";
			CDPoint xy;
			xy.x = StaStt + pGir->GetLengthDimType(StaStt, (StaPre+pBx->GetStation())/2);
			xy.y = pGir->GetElevationFlange(StaPre)/2 
				 + pGir->GetElevationFlange(pBx->GetStation())/2 + Dom.Always(16);
			Dom.TextOut(xy,sText);
			if(pDomOpt->m_Gen_bForceTextLow)
			{
				if(nCompressPre==1)	sText = "- 인 장 구 간 -";
				if(nCompressPre==2)	sText = "- 압 축 구 간 -";
				if(nCompressPre==3)	sText = "";

				double Height = pGir->GetHeightGirderByStaAng((StaPre+pBx->GetStation())/2);
				xy.y = pGir->GetElevationFlange(StaPre)/2 
					 + pGir->GetElevationFlange(pBx->GetStation())/2 - Height - Dom.Always(8);
				Dom.TextOut(xy, sText);
			}

			StaPre = pBx->GetStation();
		}
		nCompressPre = nCompressCur;
		pBx = Finder.GetBxNext();
	}
	// 마지막구간
	if(nCompressPre==1)	sText = "- 압 축 구 간 -";
	if(nCompressPre==2)	sText = "- 인 장 구 간 -";
	if(nCompressPre==3)	sText = "- 교 번 구 간 -";
	
	xy.x = StaStt + pGir->GetLengthDimType(StaStt,(StaPre+pBxEnd->GetStation())/2);
	xy.y = pGir->GetElevationFlange(StaPre)/2 
		 + pGir->GetElevationFlange(pBxEnd->GetStation())/2 + Dom.Always(16);
	Dom.TextOut(xy,sText);
	if(pDomOpt->m_Gen_bForceTextLow)
	{
		if(nCompressPre==1)	sText = "- 인 장 구 간 -";
		if(nCompressPre==2)	sText = "- 압 축 구 간 -";
		if(nCompressPre==3)	sText = "";
		double Height = pGir->GetHeightGirderByStaAng(pBxEnd->GetStation());
		xy.y = pGir->GetElevationFlange(StaPre)/2 
			 + pGir->GetElevationFlange(pBxEnd->GetStation())/2 - Height - Dom.Always(8);
		Dom.TextOut(xy, sText);
	}

	*pDomP << Dom;
}


// 각구간의 중심에 표현
void CAPlateDrawDetailDim::DimTextJongHStiff(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
  	CPlateBridgeApp *pBridge = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	COptionStd	    *pDomOpt = m_pStd->GetDomOptionStd();	 	

	__int64 BxFlag = BX_CROSSBEAM_VBRACING|BX_VSTIFF;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag,pBxStt,pBxEnd);

	long nCompressPre = pBx->GetCompressTypeByComplex(); // 1-상부압축,2-하부압축,3-교번부
	long nCompressCur = pBx->GetCompressTypeByComplex();
	double StaPre     = pBx->GetStation();
	double StaStt     = pBxStt->GetStation();

	CDomyun Dom(pDomP);

	CDPoint xy;
	BOOL bFirst = TRUE;
	while(pBx)
	{
		BOOL bJewonBuje	= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_HSTIFF_NAME)	? TRUE : FALSE;	
		BOOL bJewonMat	= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_HSTIFF_MAT)	? TRUE : FALSE;
		CString strBuje = bJewonBuje ? "수평보강재" : _T("");
		long nMarkBuje = G_H_L;
		BOOL bLeft     = TRUE;
		if(pGir->m_nInstallVStiff==0 || pGir->m_nInstallVStiff==2)
		{
			nMarkBuje = G_H_R;
			bLeft     = FALSE;
		}

		CString strMat  = bJewonMat  ? pBridge->GetMaterial(nMarkBuje, pBx) : _T("");
		
		nCompressCur = pBx->GetCompressTypeByComplex();
		if(nCompressCur!=nCompressPre)
		{
			double dStaHS = (StaPre+pBx->GetStation())/2;
			double dStaHSold = 0;
			double endsta = pBxEnd->GetStation();
			double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));
			CDPoint xy;
			CPlateBasicIndex *pBxHs = pGir->GetBxByStationDir(dStaHS, -1, BX_PLATEHSTIFF);
			CWebHStiff       *pHs   = pBxHs->GetWebHStiff();
			double SttSta=0 , EndSta=0;
			for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
			{
				if(pHs->IsEmpty(nDan, bLeft))	continue;
				else
				{
					SttSta = pHs->m_dStationStt[bLeft?0:1][nDan];
					EndSta = pHs->m_dStationEnd[bLeft?0:1][nDan];
					break;
				}
			}
			dStaHS = (SttSta + EndSta)/2;
			if (bFirst)
			{
				dStaHSold = dStaHS;
				bFirst = FALSE;
			}

			xy.x = StaStt + pGir->GetLengthDimType(StaStt, dStaHS);
			xy.y = xy.x + textLen;

			double chkdStaHS = dStaHS;
			while (1)
			{
				if (CheckStation(xy.x, xy.y))
				{
					chkdStaHS +=1000;
					if (endsta <= chkdStaHS || pBx->GetStation() <= chkdStaHS)
					{
						dStaHS= chkdStaHS;
						break;
					}
					pHs  = pBxHs->GetWebHStiff();
					for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
					{
						if(pHs->IsEmpty(nDan, bLeft))	continue;
						else
						{
							SttSta = pHs->m_dStationStt[bLeft?0:1][nDan];
							EndSta = pHs->m_dStationEnd[bLeft?0:1][nDan];
							break;
						}
					}
					dStaHS = (SttSta + EndSta)/2;
					xy.x = StaStt + pGir->GetLengthDimType(StaStt, dStaHS);
					xy.y = xy.x + textLen;
				}
				else
					break;
			}

			if (endsta <= dStaHS || pBx->GetStation() <= chkdStaHS)
				dStaHS = dStaHSold;

			DimTextJongHStiffOne(&Dom, pGir->GetBxByStationDir(dStaHS, -1, BX_PLATEHSTIFF), pBxStt, dStaHS);
			bFirst = TRUE;
			//
			StaPre = pBx->GetStation();
		}
		nCompressPre = nCompressCur;
		pBx = Finder.GetBxNext();
	}

	*pDomP << Dom;
}

BOOL CAPlateDrawDetailDim::CheckStation(double sta1, double sta2)
{
	BOOL bCheck = FALSE;
	for (long n=0; n < m_JweonInfoStaArr.GetSize(); n++)
	{
		double dStaPre = m_JweonInfoStaArr[n].x;
		double dStaEnd = m_JweonInfoStaArr[n].y;

		if((dStaPre >= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd >= sta2) || 
		   (dStaPre <= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd >= sta2) ||
		   (dStaPre <= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd <= sta2) ||
		   (dStaPre >= sta1 && dStaEnd >= sta1 && dStaPre <= sta2 && dStaEnd <= sta2))
			bCheck = TRUE;
	}

	return bCheck;
}


void CAPlateDrawDetailDim::DimTextJongHStiffOne(CDomyun *pDomP, CPlateBasicIndex *pBx, CPlateBasicIndex *pBxStt, double dStdSta)
{
	if(pBx==NULL) return;
 
  	CPlateBridgeApp *pBridge = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	CARoadOptionStd *pOpt    = m_pStd->m_pOptStd;
	COptionStd	    *pDomOpt = m_pStd->GetDomOptionStd();	 	

	CPlateBxFinder Finder(pGir);

	long nDir = 0;
	CWebHStiff *pHs = pBx->GetWebHStiff();
	if(pHs==NULL) return;
	
	BOOL bJewonBuje	= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_HSTIFF_NAME)	? TRUE : FALSE;
	BOOL bJewonMat	= (pDomOpt->m_Gen_BujeInfo & BUJEINFO_HSTIFF_MAT)	? TRUE : FALSE;

	CDPoint xy(0,0);
 	double R = pDomP->GetArrowHeadSize()*pDomP->GetScaleDim()/2;
	double dSttY  = 0;
	double dEndY  = 0;
	double StaStt = pBxStt->GetStation();
	long nMarkBuje= G_H_L;
	BOOL bLeft    = TRUE;
	if(pGir->m_nInstallVStiff==0 || pGir->m_nInstallVStiff==2)
	{
		nMarkBuje = G_H_R;
		bLeft     = FALSE;
	}		
	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		if(pHs->IsEmpty(nDan, bLeft))	continue;

		xy.x = StaStt + pGir->GetLengthDimType(StaStt, dStdSta);
		xy.y = pGir->GetElevationFlange(dStdSta) - pHs->GetHeightHStiffBySta(nDan, bLeft, dStdSta);
		pDomP->Circle(xy,R,TRUE);
		if(dSttY==0) dSttY = xy.y;
	}
	if(dSttY==0) return;
	CString strBuje = bJewonBuje ? "수평보강재" : _T("");
	CString strMat  = bJewonMat  ? pBridge->GetMaterial(nMarkBuje, pBx) : _T("");
	dEndY = pGir->GetElevationFlange(dStdSta,0,CDPoint(0,1),FALSE) - pDomP->Always(13);
	pDomP->LineTo(xy.x,dSttY,xy.x,dEndY);

	double textLen = max(pDomP->GetTextWidth(strBuje), pDomP->GetTextWidth(strMat));

	m_JweonInfoStaArr.Add(CDPoint(xy.x,xy.x+textLen));
	TextOutJewonAndMat(pDomP, CDPoint(xy.x,dEndY), strBuje, strMat);
}

void CAPlateDrawDetailDim::TextOutJewonAndMat(CDomyun *pDomP, CDPoint xy,CString strBuje, CString strMat, BOOL bDirForward)
{
	double dLenStr  = max(pDomP->GetTextWidth(strBuje),pDomP->GetTextWidth(strMat));
	if(!bDirForward) dLenStr *= -1.0;

	pDomP->LineTo(xy.x,xy.y,xy.x+dLenStr,xy.y);
	if(strBuje.GetLength())
		pDomP->TextOut(xy.x+dLenStr/2,xy.y+pDomP->GetTextHeight()*1/4,strBuje);
	if(strMat.GetLength())
	{
		double dY = xy.y-pDomP->GetTextHeight()*5/4;
		if(strBuje.GetLength()==0)
			dY = xy.y+pDomP->GetTextHeight()*1/4;
		pDomP->TextOut(xy.x+dLenStr/2,dY,strMat);
	}
}


// 시작구간이 다음 현장이음을 지난 2~3번째 수직보강재(다이아프램)
void CAPlateDrawDetailDim::DimTextJongWeb(CDomyun *pDomP, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
  	CPlateGirderApp *pGir    = pBxStt->GetGirder();
	COptionStd	    *pDomOpt = m_pStd->GetDomOptionStd();	 	

	BOOL bJewonWebBuje = (pDomOpt->m_Gen_BujeInfo & BUJEINFO_WEB_NAME)	? TRUE : FALSE;
	//BOOL bJewonWebMat  = (pDomOpt->m_Gen_BujeInfo & BUJEINFO_WEB_MAT)	? TRUE : FALSE;

	CDomyun	Dom(pDomP);
	__int64	BxFlag	= BX_CROSSBEAM_VBRACING | BX_WEB;
	CString	szBuje	= bJewonWebBuje ? "측판" : _T("");
	CString	szMat	= _T("");//bJewonWebMat  ? pBridge->GetMaterial(G_W,pBx1) : _T("");
	CString	szMaterial	= _T("");

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd);
	while(pBx)
	{
		CPlateBasicIndex	*pBxNext	= Finder.GetBxFixNext();
		if(!pBxNext)	pBxNext=pBx;
		CPlateFactChain		*pFact		= pBx->GetFactChain(G_W);
		CPlateFactChain		*pFactNext	= pBxNext->GetFactChain(G_W);

		if(szMaterial != pFact->m_strSymbol)
		{
			double	R			= Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
			double	dStaGisi	= (pBx->GetStation()+pBxNext->GetStation())/2;
			CDPoint	xy1, xy2;
			double	dHeight	= pBx->GetHeightGirderByBx();

			xy1.x = dStaGisi;
			xy1.y = pGir->GetElevationFlange(pBx->GetStation()) - dHeight/2;
			xy2.x = xy1.x;
			xy2.y = xy1.y - dHeight/2 - Dom.Always(13);

			Dom.Circle(xy1.x,xy1.y,R,TRUE);
			Dom.LineTo(xy1,xy2);
			//
			szMat = pFact->m_strSymbol;
			double dTextLen		= max(Dom.GetTextWidth(szBuje), Dom.GetTextWidth(szMat));
			double dTextstax	= xy1.x + dTextLen;
			m_JweonInfoStaArr.Add(CDPoint(xy1.x, dTextstax));
			TextOutJewonAndMat(&Dom,xy2,szBuje,szMat);

			*pDomP << Dom;
		}
		szMaterial	= pFact->m_strSymbol;
		pBx = Finder.GetBxNext();
	}
/*
	__int64 BxFlag = BX_CROSSBEAM_VBRACING|BX_VSTIFF;
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxSp = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	if (pBxSp==NULL)
		pBxSp = pBxStt;	
	CPlateBasicIndex *pBx1  = Finder.GetBxFirst(BxFlag,pBxSp,pBxEnd);
	CPlateBasicIndex *pBx2  = Finder.GetBxNext();

	if (pBx1 == NULL || pBx2 == NULL)
	{
		pBx1 = pBxStt;
		pBx2 = pBxEnd;
	}

	CDomyun Dom(pDomP);

	double StaStt = pBxStt->GetStation();
	double StaEnd = pBxEnd->GetStation();
	double staSpStt = pBx1->GetStation(); 
	double staSpEnd = pBx2->GetStation();
	if (staSpStt < StaStt)
		staSpStt = StaStt;
	if (staSpEnd > StaEnd)
		staSpEnd = StaEnd;
	double StaMid = (staSpStt+staSpEnd)/2;
	double gH = pBx1->GetHeightGirderByBx();
	CDPoint xy1,xy2;
	xy1.x = StaStt + pGir->GetLengthDimType(StaStt,StaMid);
	xy1.y = pGir->GetElevationFlange(pBx1->GetStation()) - gH/2;
	xy2.x = xy1.x;
	xy2.y = xy1.y - gH/2 - Dom.Always(13);

	CString strBuje	= bJewonWebBuje ? "측판" : _T("");
	CString strMat	= bJewonWebMat  ? pBridge->GetMaterial(G_W,pBx1) : _T("");

	double textLen = max(Dom.GetTextWidth(strBuje), Dom.GetTextWidth(strMat));
	double textstax = xy1.x + textLen;
	CPlateBasicIndex *pBx1old = pBx1;
	CPlateBasicIndex *pBx2old = pBx2;

	while (1)
	{
		if (CheckStation(xy1.x, textstax))
		{
			pBx1 = pBx2;
			pBx2 = Finder.GetBxFirst(BxFlag, pBx1, pBxEnd, FALSE, FALSE);
			if (pBx1==NULL || pBx2==NULL)
				break;
			StaMid = (pBx1->GetStation()+pBx2->GetStation())/2;
			xy1.x = StaStt + pGir->GetLengthDimType(StaStt,StaMid);
			xy1.y = pGir->GetElevationFlange(pBx1->GetStation()) - gH/2;
			xy2.x = xy1.x;
			xy2.y = xy1.y - gH/2 - Dom.Always(13);
			textstax = xy1.x + textLen;
		}
		else
			break;
	}

	if (pBx1==NULL || pBx2==NULL)
	{

		pBx1 = pBx1old;
		pBx2 = pBx2old;

		StaMid = (pBx1->GetStation()+pBx2->GetStation())/2;
		xy1.x = StaStt + pGir->GetLengthDimType(StaStt,StaMid);
		xy1.y = pGir->GetElevationFlange(pBx1->GetStation()) - gH/2;
		xy2.x = xy1.x;
		xy2.y = xy1.y - gH/2 - Dom.Always(13);
		textstax = xy1.x + textLen;
	}

	double R = Dom.GetArrowHeadSize()*Dom.GetScaleDim()/2;
	Dom.Circle(xy1.x,xy1.y,R,TRUE);
	Dom.LineTo(xy1,xy2);
	//
	m_JweonInfoStaArr.Add(CDPoint(xy1.x, textstax));
	TextOutJewonAndMat(&Dom,xy2,strBuje,strMat);

	*pDomP << Dom;
	*/
}
