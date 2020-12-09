// APlateDrawCamber.cpp: implementation of the CAPlateDrawCamber class.
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
 
CAPlateDrawCamber::CAPlateDrawCamber(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}
CAPlateDrawCamber::~CAPlateDrawCamber()
{
 
}

/////////////////////////////////////////////////////////////////////////////
// CAPlateDrawCamber message handlers

BOOL CAPlateDrawCamber::AllDrawCamber(CDomyun* pCamberDom, long nG, BOOL bTriangle)
{
	CPlateBridgeApp	*pDB		= (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd	*pOptStd	= m_pDataMng->GetOptionStd();
	CDomyun Dom(pCamberDom);

	if(Dom.GetCWnd() == NULL) return FALSE;

	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint BasePoint, BriPoint, Rate;
	BOOL	bDivide = FALSE;

	CString str;

	Rate = GetDrawRate(&Dom, &BriPoint, nG);
	Dom.SetScaleDim(300*Rate.x);
	BasePoint.x = 0;
	BasePoint.y = 200;
	double sttSta = pGir->GetStationStt();
	//	①②③④⑤⑥⑦⑧
	if(bDivide)
	{
		pOptStd->SetEnvType(&Dom, HCAD_TXTG);
		Dom.TextOut(CDPoint(BasePoint.x+BriPoint.x*Rate.x*2+sttSta, BasePoint.y+BriPoint.y*Rate.y/2+100),
							_T("계산 솟음량((1)~(4)"));
	}

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DrawCamberJDanCurve			(&Dom, &BasePoint, &Rate, nG, ECC_JONGDAN);	
	DrawCamberPyunSlopCurve		(&Dom, &BasePoint, &Rate, nG, ECC_DEFAULT1);
	DrawCamberOnlySteelWeight	(&Dom, &BasePoint, &Rate, nG, ECC_ONLYSTEEL);	
	DrawCamberSteelWeight		(&Dom, &BasePoint, &Rate, nG, ECC_STEEL);
	DrawCamberVivid				(&Dom, &BasePoint, &Rate, nG, ECC_VIVID);

	if(pDB->IsTUGir())
	{
		DrawCamberConf(&Dom, &BasePoint, &Rate, nG, ECC_CONF);
		DrawCamberTurning(&Dom, &BasePoint, &Rate, nG, ECC_TURNING);
	}

	if(bDivide)
	{
		DrawCamberHeightLine		(&Dom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
		DrawCamberTriangle			(&Dom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
		BasePoint.y = -(BriPoint.y*Rate.y*1.2)-100;
	}	
	if(bDivide)
	{
		pOptStd->SetEnvType(&Dom, HCAD_TXTG);
		Dom.TextOut(CDPoint(BasePoint.x+BriPoint.x*Rate.x*2+sttSta, BasePoint.y+BriPoint.y*Rate.y/2+100),
							_T("시공 단계별 솟음량((5)~(8))"));
	}
	DrawCamberFact			(&Dom, &BasePoint, &Rate, nG, ECC_FACT);
	DrawCamberField			(&Dom, &BasePoint, &Rate, nG, ECC_FIELD);
	DrawCamberRemoveSteel	(&Dom, &BasePoint, &Rate, nG, ECC_REMOVESTEEL);
	DrawCamberRemoveVivid	(&Dom, &BasePoint, &Rate, nG, ECC_REMOVEVIVID);
	DrawCamberHeightLine	(&Dom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
	if(bTriangle)
	{
		pOptStd->SetEnvType(&Dom, HCAD_TXTG);
		DrawCamberTriangle		(&Dom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
	}	
	DrawDimCamber(&Dom,&BasePoint,&Rate,nG);

	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	*pCamberDom << Dom;	
	pCamberDom->Invalidate();
	return TRUE; 
}

BOOL CAPlateDrawCamber::AllDrawCamberOut(CDomyun *pCamberDom, long nG, long nCol)
{
	pCamberDom->ClearEtt(TRUE);

	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();
	CDomyun Dom(pCamberDom);

	CDPoint BasePoint, BriPoint, Rate;
	BOOL	bDivide = 1;

	CString str;

/*	BOOL c1, c2, c3, c4, c5, c6, c7, c8;
	// Backup
	c1 = m_pEnv->m_bJongDan;
	c2 = m_pEnv->m_bOnlySteel;
	c3 = m_pEnv->m_bSteel;
	c4 = m_pEnv->m_bVivid;
	c5 = m_pEnv->m_bFact;
	c6 = m_pEnv->m_bField;	c7 = m_pEnv->m_bRemoveSteel;
	c8 = m_pEnv->m_bRemoveVivid;
	// Set 1
	m_pEnv->m_bJongDan		= TRUE;
	m_pEnv->m_bOnlySteel	= TRUE;
	m_pEnv->m_bSteel		= TRUE;
	m_pEnv->m_bVivid		= TRUE;
	m_pEnv->m_bFact			= TRUE;
	m_pEnv->m_bField		= TRUE;
	m_pEnv->m_bRemoveSteel	= TRUE;
	m_pEnv->m_bRemoveVivid	= TRUE;
*/
	Rate = GetDrawRate(&Dom, &BriPoint, nG);
	BasePoint.x = 0;
	BasePoint.y = 200;


	//	①②③④⑤⑥⑦⑧
	if(bDivide)
	{
		pOptStd->SetEnvType(pCamberDom, HCAD_TXTG);
		pCamberDom->TextOut(CDPoint(BasePoint.x+BriPoint.x*Rate.x*2, BasePoint.y+BriPoint.y*Rate.y/2+100),
							_T("계산 솟음량((1)~(4))"));
	}

	DrawCamberJDanCurve			(pCamberDom, &BasePoint, &Rate, nG, ECC_JONGDAN);
	DrawCamberPyunSlopCurve		(&Dom, &BasePoint, &Rate, nG, ECC_DEFAULT1);//주석해제 ..070115...KB ..왜 주석처리돼  있었는지 모름..
	DrawCamberOnlySteelWeight	(pCamberDom, &BasePoint, &Rate, nG, ECC_ONLYSTEEL);
	DrawCamberSteelWeight		(pCamberDom, &BasePoint, &Rate, nG, ECC_STEEL);
	DrawCamberVivid				(pCamberDom, &BasePoint, &Rate, nG, ECC_VIVID);

	if(pDB->IsTUGir())
	{
		DrawCamberConf(&Dom, &BasePoint, &Rate, nG, ECC_CONF);
		DrawCamberTurning(&Dom, &BasePoint, &Rate, nG, ECC_TURNING);
	}

	if(bDivide)
	{
		DrawCamberHeightLine		(pCamberDom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
		DrawCamberTriangle			(pCamberDom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
		BasePoint.y = -(BriPoint.y*Rate.y*1.1)-100;
	}
	bDivide = 2;
	if(bDivide)
	{
		pOptStd->SetEnvType(pCamberDom, HCAD_TXTG);
		pCamberDom->TextOut(CDPoint(BasePoint.x+BriPoint.x*Rate.x*2, BasePoint.y+BriPoint.y*Rate.y/2+100),
							_T("시공 단계별 솟음량((5)~(8))"));
	}
	DrawCamberFact				(pCamberDom, &BasePoint, &Rate, nG, ECC_FACT);
	DrawCamberField				(pCamberDom, &BasePoint, &Rate, nG, ECC_FIELD);
	DrawCamberRemoveSteel		(pCamberDom, &BasePoint, &Rate, nG, ECC_REMOVESTEEL);
	DrawCamberRemoveVivid		(pCamberDom, &BasePoint, &Rate, nG, ECC_REMOVEVIVID);
	DrawCamberHeightLine		(pCamberDom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
	DrawCamberTriangle			(pCamberDom, &BasePoint, &Rate, bDivide, nG, ECC_DEFAULT2);
/*
	m_pEnv->m_bJongDan		= c1;
	m_pEnv->m_bOnlySteel	= c2;
	m_pEnv->m_bSteel		= c3;
	m_pEnv->m_bVivid		= c4;
	m_pEnv->m_bFact			= c5;
	m_pEnv->m_bField		= c6;
	m_pEnv->m_bRemoveSteel	= c7;
	m_pEnv->m_bRemoveVivid	= c8;
*/	return TRUE;
}

void CAPlateDrawCamber::DrawCamberConf(CDomyun* pDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp	*pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd	*pOptStd  = m_pDataMng->GetOptionStd();

	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDom, HCAD_STLC);
		pDom->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;
	
	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 3);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)	
// 			pDom->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
		else		
// 			pDom->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
	}

// 	pDom->PLineEnd();
	if(arr.GetSize() == 0)return;

	pDom->AddBSpline(arr);
}

void CAPlateDrawCamber::DrawCamberTurning(CDomyun* pDom, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp	*pDB		= (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd	*pOptStd	= m_pDataMng->GetOptionStd();
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);

	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDom, HCAD_STLC);
		pDom->SetLineColor(Ecc);
	}

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;
	
	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 4);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 			pDom->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDom->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}

	if(arr.GetSize() == 0)return;

	pDom->AddBSpline(arr);
// 	pDom->PLineEnd();
}

// 공장 제작시 솟음량
void CAPlateDrawCamber::DrawCamberFact(CDomyun *pDomP, CDPoint *pBase, CDPoint *pRate, long nG, enum EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bFact == 0) return;	
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	long	nBridgeType	= pDB->GetBridgeType();
	double sum = 0,result;
	CVectorArray arr;

	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(nBridgeType, 5);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}
	
	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}
// 현장 거치시 솟음량
void CAPlateDrawCamber::DrawCamberField(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bField == 0) return;	
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	nBridgeType	= pDB->GetBridgeType();
	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;

	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(nBridgeType, 6);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}
	
	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}
//	bDivide	:	도면을 나눌것인지 구별하는 flag
//		0	:	도면을 나누지 않음
//		1	:	도면을 나눔(LEFT)
//		2	:	도면을 나눔(RIGHT)
void CAPlateDrawCamber::DrawCamberHeightLine(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, BOOL bDivide, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

	pOptStd->SetEnvType(pDomP, HCAD_DIML);	

	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double h = 0.0;
	double sx, sy = 0;
	CString str;

	for(su = 0;su < camb;su++)
	{
		h = 0.0;
		sx = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su == 0) pDomP->PMoveTo(pBase->x+sx*pRate->x, pBase->y);

		for(long type=0; type<11; type++)
		{
			if(fabs(h) < fabs(pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), type)))
				h = pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), type);
//			h = max(fabs(h), fabs(pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), type)));

			pDomP->LineTo(pBase->x+sx*pRate->x , pBase->y+sy*pRate->y,pBase->x+sx*pRate->x,  pBase->y+h*pRate->y);
		}

		double dX = pBase->x+sx*pRate->x;
		double dY = pBase->y+(sy-3000*pRate->x);
		str.Format("%d", su+1);
		pDomP->TextOut(CDPoint(dX, dY),str);
	}
}
// 종단 곡선에 의한 조정량
void CAPlateDrawCamber::DrawCamberJDanCurve(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bJongDan == 0) return;
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;
	
	for(su = 0; su<camb; su++)
	{		
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 0);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}
	
	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}

// 강재자중에 의한 솟음량
void CAPlateDrawCamber::DrawCamberOnlySteelWeight(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bOnlySteel == 0) return;	
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;
	
	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 2);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}

	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}
// 편경사에 의한 조정량
void CAPlateDrawCamber::DrawCamberPyunSlopCurve(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bJongDan == 0) return;
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;
	
	for(su = 0;su < camb;su++)
	{		
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 1);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);

	}

	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}
// 1차사하중 재하시 솟음량
void CAPlateDrawCamber::DrawCamberRemoveSteel(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bRemoveSteel == 0) return;	
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;

	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 7);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));


	}
	
	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
}
// 2차사하중 재하시 솟음량
void CAPlateDrawCamber::DrawCamberRemoveVivid(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bRemoveVivid == 0) return;	
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;

	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 8);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}

	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}
// 합성전 1차사하중에 의한 솟음량
void CAPlateDrawCamber::DrawCamberSteelWeight(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bSteel == 0) return;	
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;
	
	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 3);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}
	
	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}

void CAPlateDrawCamber::DrawCamberTriangle(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, BOOL bDivide, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

	CDomyun *pDom = new CDomyun(pDomP);
	//pDom->SetScaleDim(100);
//	pOptStd->SetEnvType(pDom, HCAD_MIS2);
	pOptStd->SetEnvType(pDom, HCAD_TXTG);

	if(pDB->GetGirdersu() <= 0)  {delete pDom; return;}
	long	gigan,gigansu = pDB->m_nQtyJigan;
	if(gigansu <= 0 )  {delete pDom; return;}
	double	sta = 0, sttsta;
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	CString str;
	CDPoint	s, e;//	Line용
	double	Offset;
	pDom->SetTextHeight(2);
	// BMD 지간분할 삼각형
	sttsta = pGir->GetSSS(OFF_STT_SHOE);
	for(gigan = 0; gigan <= gigansu; gigan++)
	{
		sta = pGir->GetStationOnJijum(gigan);
		pDom->Triangle(pBase->x+(sta/*-sttsta*/)*pRate->x,pBase->y,50*pRate->x);
	}

	if(!bDivide)
	{
		pDom->SetTextAlignHorz(TA_LEFT);
		Offset = (pDom->GetTextHeight()*1.5);
		s.x = pBase->x+10 + sttsta*pRate->x;
		s.y = pBase->y+Offset+pDom->GetTextHeight()*2;
		e.x = pBase->x+100 + sttsta*pRate->x;

		e.y = s.y;

		e.y += Offset*4;
		pDom->SetLineColor(ECC_VIVID);		
		pDom->SetTextColor(ECC_VIVID%10);		
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("합성후 2차사하중");
		pDom->TextOut(CDPoint(e.x, e.y),str);

		e.y += Offset;
		pDom->SetLineColor(ECC_STEEL);
		pDom->SetTextColor(ECC_STEEL%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("합성전 1차사하중");
		pDom->TextOut(CDPoint(e.x, e.y),str);

		if(pDB->IsTUGir())
		{
			e.y += Offset;
			pDom->SetLineColor(ECC_TURNING);
			pDom->SetTextColor(ECC_TURNING%10);
			pDom->LineTo(s.x, e.y, e.x, e.y);
			str = _T("180˚ Turnning");
			pDom->TextOut(CDPoint(e.x, e.y),str);

			e.y += Offset;
			pDom->SetLineColor(ECC_CONF);
			pDom->SetTextColor(ECC_CONF%10);
			pDom->LineTo(s.x, e.y, e.x, e.y);
			str = _T("구속콘크리트자중");
			pDom->TextOut(CDPoint(e.x, e.y),str);
		}

		e.y += Offset;
		pDom->SetLineColor(ECC_ONLYSTEEL);
		pDom->SetTextColor(ECC_ONLYSTEEL%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("강재 자중");
		pDom->TextOut(CDPoint(e.x, e.y),str);

		e.y += Offset;
		pDom->SetLineColor(ECC_DEFAULT1);
		pDom->SetTextColor(ECC_DEFAULT1%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("편경사");
		pDom->TextOut(CDPoint(e.x, e.y),str);

		e.y += Offset;
		pDom->SetLineColor(ECC_JONGDAN);
		pDom->SetTextColor(ECC_JONGDAN%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("종단 곡선");
		pDom->TextOut(CDPoint(e.x, e.y),str);
	}

	Offset = -(pDom->GetTextHeight()*1.5);

	e.y = -s.y;
	e.y = Offset*2;

	if(!bDivide)
	{
		e.y += Offset;
		pDom->SetLineColor(ECC_FACT);
		pDom->SetTextColor(ECC_FACT%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("공장 제작시");
		pDom->TextOut(CDPoint(e.x, e.y),str);

		e.y += Offset;
		pDom->SetLineColor(ECC_FIELD);
		pDom->SetTextColor(ECC_FIELD%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("현장 거치시");
		pDom->TextOut(CDPoint(e.x, e.y),str);

		e.y += Offset;
		pDom->SetLineColor(ECC_REMOVESTEEL);
		pDom->SetTextColor(ECC_REMOVESTEEL%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("1차 사하중 재하시");
		pDom->TextOut(CDPoint(e.x, e.y),str);

		e.y += Offset;
		pDom->SetLineColor(ECC_REMOVEVIVID);
		pDom->SetTextColor(ECC_REMOVEVIVID%10);
		pDom->LineTo(s.x, e.y, e.x, e.y);
		str = _T("2차 사하중 재하시");
		pDom->TextOut(CDPoint(e.x, e.y),str);
	}

	pDom->SetTextColor(3);
	pDom->SetTextHeight(4);
	str.Format("< Girder%d >", nG+1);

	if(!bDivide)
	{
		pDom->SetCalcExtRect();
		CDRect rectDom = pDom->GetExtRect();		
		
		pDom->TextOut(rectDom.left, rectDom.bottom + pDom->GetTextHeight()/2, str);
	}
	else
	{
		e.x = pBase->x+100 + sttsta*pRate->x;
		e.y = pBase->y;
		pDom->TextOut(e.x, e.y+pDom->Always(40), str);
	}
	
	*pDomP << *pDom;
	delete pDom;
}
// 사용자 입력 솟음량
void CAPlateDrawCamber::DrawCamberUser(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	long nUserSu = pGir->GetQtyCamberByUser();
	CVectorArray arr;

	for (long nUser = 0; nUser < nUserSu; nUser++)
	{
		double sum = 0, dResult = 0;		
		for(su = 0;su < camb;su++)
		{
			dResult = pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), nUser+5);
			sum = pGir->GetCamberByNode(su)->GetCamberStation();
			if(su==0)
				arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 				pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
			else
				arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+dResult*pRate->y, 0));
// 				pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+dResult*pRate->y);
		}
		
		if(arr.GetSize() == 0)return;

		pDomP->AddBSpline(arr);
// 		pDomP->PLineEnd(); 
	}
}
// 합성후 2차사하중에 의한 솟음량
void CAPlateDrawCamber::DrawCamberVivid(CDomyun *pDomP, CDPoint* pBase, CDPoint* pRate, long nG, EnumCamberColor Ecc)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();

//	if(m_pEnv->m_bVivid == 0) return;	
	if(Ecc!=ECC_NOT)
	{
		pOptStd->SetEnvType(pDomP, HCAD_STLC);
		pDomP->SetLineColor(Ecc);
	}
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	long	su, camb = pGir->GetQtyNodeCamber();
	double sum = 0,result;
	CVectorArray arr;
	
	for(su = 0;su < camb;su++)
	{
		result =pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(), 4);
		sum = pGir->GetCamberByNode(su)->GetCamberStation();
		if(su==0)
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y, 0));
// 			pDomP->PMoveTo(pBase->x+sum*pRate->x, pBase->y);
		else
			arr.Add(CVector(pBase->x+sum*pRate->x, pBase->y+result*pRate->y, 0));
// 			pDomP->PLineTo(pBase->x+sum*pRate->x, pBase->y+result*pRate->y);
	}
	
	if(arr.GetSize() == 0)return;

	pDomP->AddBSpline(arr);
// 	pDomP->PLineEnd();
}

CDPoint CAPlateDrawCamber::GetDrawRate(CDomyun *pDom, CDPoint* pBriPoint, long nG)
{
	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CPlateGirderApp  *pGir = pDB->GetGirder(nG);

	double	h = pGir->GetMaxCamberInGirder();
	if(fabs(h) < 0.01)	h = 1;

	CRect	ScrRect;
	CDPoint	Rate;

	pBriPoint->x = (pGir->GetSSS(OFF_END_SLAB) - pGir->GetSSS(OFF_STT_SLAB))/4.5;
	pBriPoint->y = fabs(h)*2;

	CDomyun Dom(pDom);
	Dom.GetCWnd()->GetClientRect(&ScrRect);
	
	Rate.x = (ScrRect.right/pBriPoint->x);
	Rate.y = (ScrRect.bottom/pBriPoint->y);

	return (Rate);
}

void CAPlateDrawCamber::DrawDimCamber(CDomyun* pDomp,CDPoint* pBase,CDPoint* pRate,long nG)
{
	CPlateBridgeApp *pDB		= (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CPlateGirderApp *pGir		= pDB->GetGirder(nG);
	CARoadOptionStd *pOptStd	= m_pDataMng->GetOptionStd();
	CDomyun			Dom(pDomp);
	double			dResult		= 0;
	double			dSum		= 0;
	CString			str;
	
	//if(nCamb <3) return 0;
	long nQtyCamb = pGir->GetQtyNodeCamber();

	if(nQtyCamb <=0) return;
	long   nCamb  = GetMaxCamber(nG);
	double dH	  = GetMaxMinCamberCalc(nG,nCamb);
	long   nCount = 0;
	double *dMax;
	double dCut;
	
	
	dMax = (double*)malloc(sizeof(double)*nQtyCamb);
	dCut = dH/4;
	long n = 0;
	for(n=0;n<nQtyCamb;n++)
	{
		dH = GetMaxMinCamberCalc(nG,n);
		if(dH>= dCut)
		{
			dMax[nCount] = n;
			nCount++;
		}
		
	}
	
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	for(n=0;n<11;n++)
	{		
		str.Format("(%d)",n+1);
		dSum    = pGir->GetCamberByNode((long)dMax[n%nCount])->GetCamberStation();
		dResult = pGir->GetCamberByNode((long)dMax[n%nCount])->GetCamberByType(pDB->GetBridgeType(), n);
		Dom.GiSiArrowExt(pBase->x+dSum*pRate->x, pBase->y+dResult*pRate->y,70,70,1,str);			
	}
	*pDomp << Dom;
	free(dMax);

}
long CAPlateDrawCamber::GetMaxCamber(long nG)
{
	CPlateBridgeApp *pDB	 = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	 = pDB->GetGirder(nG);
	double  dMaxTemp;
	double  dMinTemp;
	double  dResult;
	double* dMax;
	long	camb;
	camb = pGir->GetQtyNodeCamber();
	dMax = (double*)malloc(camb*sizeof(double));
	for(long su = 0;su<camb;su++)
	{
		dMaxTemp = pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(),0);
		dMinTemp = dMaxTemp;
		for(long n = 1;n<11;n++)
		{
			dResult  = pGir->GetCamberByNode(su)->GetCamberByType(pDB->GetBridgeType(),n);
			dMaxTemp = max(dResult,dMaxTemp);
			dMinTemp = min(dResult,dMinTemp);
		}
		dMax[su] = dMaxTemp-dMinTemp;
	}
	dMaxTemp  = dMax[0];
	long nMax = 0;
	for(long nn=1;nn<camb;nn++)
	{
		if(dMaxTemp <dMax[nn])
			nMax     = nn;
			dMaxTemp = dMax[nn];
	}
	free(dMax);
	return nMax;
	
}

double CAPlateDrawCamber::GetMaxMinCamberCalc(long nG,long nCamber)
{
	CPlateBridgeApp *pDB	 = (CPlateBridgeApp*)m_pDataMng->GetBridge();
	CPlateGirderApp *pGir	 = pDB->GetGirder(nG);
	double dResult;
	double dMaxTemp;
	double dMinTemp;
		dMaxTemp = pGir->GetCamberByNode(nCamber)->GetCamberByType(pDB->GetBridgeType(),0);
		dMinTemp = dMaxTemp;
		for(long n = 1;n<11;n++)
		{
			dResult =pGir->GetCamberByNode(nCamber)->GetCamberByType(pDB->GetBridgeType(),n);
			dMaxTemp = max(dResult,dMaxTemp);
			dMinTemp = min(dResult,dMinTemp);
		}
		
		return dMaxTemp-dMinTemp;
}