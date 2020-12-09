// APlateDrawCross.cpp: implementation of the CAPlateDrawCross class.
//  
//////////////////////////////////////////////////////////////////////
 
#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "APlateDrawCross.h"

#ifdef _DEBUG 
#undef THIS_FILE 
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW  
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateDrawCross::CAPlateDrawCross(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{
  
}

CAPlateDrawCross::~CAPlateDrawCross()
{

}

//<summary>
//V타입
//</summary>
//<param name="*pDom"></param>
//<param name="*pBx"></param>
void CAPlateDrawCross::DrawInputVBracingFront(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();

	if(pBx->GetSection()->IsenDetType(SECDET_VBRACING)==NULL) return;

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	DrawGirderFront(&Dom, pBx, ALL);
	DrawGirderFront(&Dom, pBxNext, ALL);
	DrawVBracingSpliceOnCRBx(&Dom, pBx, TRUE);
	DrawVBracingAnchorBar(&Dom, pBx);
	DrawVBracingBeamOnCRBx(&Dom, pBx, FALSE);

	DrawHStiffFront(&Dom, pBx, TRUE);				// 수평보강재
	DrawHStiffFront(&Dom, pBx, FALSE);				// 수평보강재
	DrawHStiffFront(&Dom, pBxNext, TRUE);			// 수평보강재
	DrawHStiffFront(&Dom, pBxNext, FALSE);			// 수평보강재

	DrawFrontHGussetSplice(&Dom, pBx, TRUE);		// 수평 브레이싱 연결판
	DrawFrontHGussetSplice(&Dom, pBxNext, TRUE);	// 수평 브레이싱 연결판

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DimVBracingFrontSplice(&Dom, pBx, 0);
	DimVBracingFront(&Dom, pBx);

	// 이동 : 좌측 플랜지하단의 중심을 (0,0)으로
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CDPoint xyOrigin = (A[1]+A[2])/2;
	Dom.Move(-xyOrigin);

	*pDom << Dom;
}

void CAPlateDrawCross::DrawInputVBracingPlan(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();

	CAPlateDrawPyung DrawPyung(m_pDataMng);
	if(pBx->GetSection()->IsenDetType(SECDET_VBRACING)==NULL) return;

	CDomyun Dom(pDom);
	double dGirLen = 300;
	DrawPyung.DrawVBracingFlangeOnBx(&Dom, pBx);
	DrawPyung.DrawVStiffOnBx(&Dom, pBx);
	DrawPyung.DrawVStiffOnBx(&Dom, pBxNext);
	DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBx, TRUE, TRUE);
	DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBx, FALSE, TRUE);
	DrawPyung.DrawVBracingCenterSplice(&Dom, pBx);

	DrawPyung.DrawGirderWithenStation(&Dom, pBx, pBx->GetStation()-dGirLen, pBx->GetStation()+dGirLen, TRUE);
	DrawPyung.DrawGirderWithenStation(&Dom, pBxNext, pBxNext->GetStation()-dGirLen, pBxNext->GetStation()+dGirLen, FALSE);
	//지점부 수직보강재
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	if(pBx->IsJijum())		DrawPyung.DrawJiJumStiffOnBx(&Dom, pBx, TRUE, FALSE);
	if(pBxNext->IsJijum())	DrawPyung.DrawJiJumStiffOnBx(&Dom, pBxNext, TRUE, FALSE);

	DrawPyung.DrawVBracingAnchorBarOnBx(&Dom,pBx);
	DimVBracingPlanLeft(&Dom,pBx);
	DimVBracingPlanRight(&Dom,pBx);
	// 이동 및 회전 : 좌측복부판의 중심을 (0,0)으로
	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CDPoint GirL = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint GirR = pGir->m_pRight->GetXyGirderDis(pBxSec->GetStation());
	CDPoint vCros = -(GirL-GirR).Unit();	// 가로보의 각도
	CDPoint vAng  = CDPoint(0,1);
	CDPoint vAngSec = pBx->GetAngleSection();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	Dom.Move(-GirL.x,-GirL.y);
	Dom.Rotate(-vGir.GetAngleRadian(),0,0);
	Dom.Rotate(vAng.GetAngleRadian(),0,0);
	//DIMENSION 
	double BX=0,BY=0;
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("TOP");
	DimVBracingPlanTop(&Dom,pBx,BX,BY);
	*pDom << Dom;
}

//comment: BX_VSTIFF_CENTER이나 BX_CROSSBEAM_CENTER인 경우는 수직보강재 생성
//         BX_HBRACINGGUSSET일 경우는 거더만 생성.
void CAPlateDrawCross::DrawGirderFront(CDomyun *pDom, CPlateBasicIndex *pBx, long nDrawingType)
{
	if(pBx==NULL) return;

	CPlateBridgeApp* pDB	 = m_pDataMng->GetBridge();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption   *pOpt    = m_pDataMng->GetGlobalOption();

	long nLastGir = pDB->GetGirdersu()-2;
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CPlateGirderApp *pGirEE  = pDB->GetGirder(nLastGir+1);

	CSection  *pSec    = NULL;
	CVStiff   *pVStiff = NULL;
	CVBracing *pVr     = NULL;

	CPlateBasicIndex* pBxEE = pBx->GetBxMatchByCrossBeam(pGirEE);
	if(pBxEE == NULL)	return;

	if(pBx->GetNumberGirder()==pDB->GetGirdersu()-1)
		pSec = pBxEE->GetSection();
	else		
		pSec = pBx->GetSection();
	if(pSec)
	{
		pVStiff = pSec->GetVStiff();
		pVr     = pSec->GetVBracing();
	}

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	double dThickTop = pGir->GetThickFactByStation(pBx->GetStation(), 0);
	double dThickBot = pGir->GetThickFactByStation(pBx->GetStation(), 1);
	double dThickWeb = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dHeight   = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dWidthUp  = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWidthLo  = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
	double dRatio = 0.4;
	double dR1         = m_pDataMng->GetBridge()->GetGirder(-1)->m_pSangse->m_Scallop_R1==0
		               ? 35 : m_pDataMng->GetBridge()->GetGirder(-1)->m_pSangse->m_Scallop_R1;	
	double dWVStiffL = 0;
	double dWVStiffR = 0;
	if(pVStiff)
	{
		dWVStiffL = pVStiff->m_dWidth[0];
		dWVStiffR = pVStiff->m_dWidth[1];
	}
	CDPoint A[10], vX(1,0), vY(0,1);
	pGir->GetHunchXyDom(pBx,A, FALSE, FALSE, pDB->IsTUGir());

	CDPoint xyUpCen  = (A[1]+A[2])/2;
	if(pDB->IsTUGir())
		xyUpCen += CDPoint(0,-1)*(pDB->m_BindConc.m_dHeight-pDB->m_BindConc.m_dDis);
	if(pOpt->GetDesignHeigtBaseUpper()==0)
		xyUpCen += CDPoint(0,-1)*dThickTop;
	CDPoint xyLoCen  = xyUpCen + CDPoint(0,-1)*dHeight;

	if(nDrawingType==UPPER)
	{ 
		double dUpper = pVr==NULL ? dRatio*dHeight
			: (pBx->GetNumberGirder()<pDB->GetGirdersu()-1 ? pVr->m_dLH1+pVr->m_duL1+pVr->m_duL2 : pVr->m_dRH1+pVr->m_duR1+pVr->m_duR2);

		Dom.Rectangle(-dWidthUp/2+xyUpCen.x, xyUpCen.y, dWidthUp/2+xyUpCen.x, xyUpCen.y+dThickTop);//상판
		Dom.MoveTo(xyUpCen-vX*dThickWeb/2);//복부판 
		Dom.LineTo(xyUpCen-vX*dThickWeb/2-vY*dUpper);
		Dom.MoveTo(xyUpCen+vX*dThickWeb/2);
		Dom.LineTo(xyUpCen+vX*dThickWeb/2-vY*dUpper);
	}
	else if(nDrawingType==LOWER)
	{
		double dLower = pVr==NULL ? dRatio*dHeight
			: (pBx->GetNumberGirder()<pDB->GetGirdersu()-1 ? pVr->m_dLH2+pVr->m_ddL1+pVr->m_ddL2 : pVr->m_dRH2+pVr->m_ddR1+pVr->m_ddR2);

		Dom.Rectangle(-dWidthLo/2   +xyLoCen.x, xyLoCen.y,         dWidthLo/2+xyLoCen.x,  xyLoCen.y-dThickBot);  //하판
		Dom.MoveTo(xyLoCen-vX*dThickWeb/2);//복부판 
		Dom.LineTo(xyLoCen-vX*dThickWeb/2+vY*dLower);
		Dom.MoveTo(xyLoCen+vX*dThickWeb/2);
		Dom.LineTo(xyLoCen+vX*dThickWeb/2+vY*dLower);
	}
	else
	{
		Dom.Rectangle(-dWidthLo/2   +xyLoCen.x, xyLoCen.y, dWidthLo/2+xyLoCen.x, xyLoCen.y-dThickBot);  //하판
		Dom.Rectangle(-dWidthUp/2   +xyLoCen.x, xyUpCen.y, dWidthUp/2+xyUpCen.x, xyUpCen.y+dThickTop);//상판
		Dom.Rectangle(-dThickWeb/2+xyLoCen.x,	xyLoCen.y, dThickWeb/2+xyLoCen.x, xyUpCen.y); //복부판
	}
	
	//거더 수직보강재
	if(nDrawingType==ALL && pVStiff && (dWVStiffL>0 || dWVStiffR>0))
	{
		if(nDrawingType!=NOTDRAWVSTIFF)
			DrawGirderVStiffFrontOnBx(&Dom, pBx);
	}
	else if(nDrawingType==UPPER)
	{
		//left
		if(dWVStiffL>0)
		{
			double dUpper = pVr==NULL ? dRatio*dHeight
				: (pBx->GetNumberGirder()<pDB->GetGirdersu()-1 ? pVr->m_dLH1+pVr->m_duL1+pVr->m_duL2 : pVr->m_dRH1+pVr->m_duR1+pVr->m_duR2);

			CDPoint xyStt = xyUpCen-vX*(dWVStiffL+dThickWeb/2);
			CDPoint xyMid = xyUpCen-vX*(dThickWeb/2);
			CDPoint xyEnd = xyUpCen-vX*(dThickWeb/2)-vY*dUpper;
			Dom.FilletWithThreePoint(xyStt, xyMid, xyEnd, dR1);
			Dom.LineTo(xyStt, xyStt-vY*dUpper);
		}
		//right
		if(dWVStiffR>0)
		{
			double dUpper = pVr==NULL ? dRatio*dHeight
				: (pBx->GetNumberGirder()<pDB->GetGirdersu()-1 ? pVr->m_dLH1+pVr->m_duL1+pVr->m_duL2 : pVr->m_dRH1+pVr->m_duR1+pVr->m_duR2);

			CDPoint xyStt = xyUpCen+vX*(dWVStiffR+dThickWeb/2);
			CDPoint xyMid = xyUpCen+vX*(dThickWeb/2);
			CDPoint xyEnd = xyUpCen+vX*(dThickWeb/2)-vY*dUpper;
			Dom.FilletWithThreePoint(xyStt, xyMid, xyEnd, dR1);
			Dom.LineTo(xyStt, xyStt-vY*dUpper);
		}
	}
	else if(nDrawingType==LOWER)
	{
		//left
		if(dWVStiffL>0)
		{
			double dLower = pVr==NULL ? dRatio*dHeight
				: (pBx->GetNumberGirder()<pDB->GetGirdersu()-1 ? pVr->m_dLH2+pVr->m_ddL1+pVr->m_ddL2 : pVr->m_dRH2+pVr->m_ddR1+pVr->m_ddR2);
			CDPoint xyStt = xyLoCen-vX*(dWVStiffL+dThickWeb/2);
			CDPoint xyMid = xyLoCen-vX*(dThickWeb/2);
			CDPoint xyEnd = xyLoCen-vX*(dThickWeb/2)+vY*dLower;
			Dom.FilletWithThreePoint(xyStt, xyMid, xyEnd, dR1);
			Dom.LineTo(xyStt, xyStt+vY*dLower);
		}
		//right
		if(dWVStiffR>0)
		{
			double dLower = pVr==NULL ? dRatio*dHeight
				: (pBx->GetNumberGirder()<pDB->GetGirdersu()-1 ? pVr->m_dLH2+pVr->m_ddL1+pVr->m_ddL2 : pVr->m_dRH2+pVr->m_ddR1+pVr->m_ddR2);
			CDPoint xyStt = xyLoCen+vX*(dWVStiffR+dThickWeb/2);
			CDPoint xyMid = xyLoCen+vX*(dThickWeb/2);
			CDPoint xyEnd = xyLoCen+vX*(dThickWeb/2)+vY*dLower;
			Dom.FilletWithThreePoint(xyStt, xyMid, xyEnd, dR1);
			Dom.LineTo(xyStt, xyStt+vY*dLower);
		}
	}

	if(pDB->IsTUGir() && nDrawingType!=LOWER)
	{
		CAPlateDrawDanmyun DrawDan(m_pDataMng);
		DrawDan.DrawGirderBindConc(&Dom, pBx);
	}
	
	*pDom << Dom;
}

void CAPlateDrawCross::DrawGirderVStiffFrontOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;

	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CSection        *pSec    = pBx->GetSection();

	if(pSec==NULL)	return;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	CAPlateDrawDanmyun DrawDan(m_pDataMng);
	DrawDan.DrawVStiffOnBx(&Dom, pBx);
	*pDom << Dom;

}

void CAPlateDrawCross::DrawSlabLineFront(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetLineColor(150);
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	Dom.MoveTo(A[0].x, A[0].y);
	for(long n=1; n<10 ;n++)
		Dom.LineTo(A[n].x, A[n].y);  //슬라브라인
	
	*pDom << Dom;
}

//<summary>
//빔 전체를 그림
//</summary>
//<param name="*pDom"></param>
//<param name="*pBx"></param>
void CAPlateDrawCross::DrawVBracingBeamSangSeOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, long nSangNo)
{
	if(pBx==NULL) return;	
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateGirderApp *pGirNext = pGir->m_pRight;
	CARoadOptionStd *pOptStd  = m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBxNext = (CPlateBasicIndex*)pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CAPlateDrawSteel DrawSteel(m_pDataMng);
	CVBracing        *pVr     = pBx->GetSection()->GetVBracing();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CDPoint vX(1,0), vY(0,1);
	//변수대입
	double dLH1 = pVr->m_dLH1;
	double dRH1 = pVr->m_dRH1;
	double dLH2 = pVr->m_dLH2;
	double dRH2 = pVr->m_dRH2;
	double ds   = pVr->m_dS;
	//기초설정(형강타입 및 제원)
//	long nSteelTypeUp = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_UVBEAM);
//	long nSteelTypeLo = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_LVBEAM);
//	long nSteelTypeC  = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_CVBEAM);
//	long nJewonTypeUp = pMat->GetJewonType(pBx, CSteelMaterial::BUJE_UVBEAM);
//	long nJewonTypeLo = pMat->GetJewonType(pBx, CSteelMaterial::BUJE_LVBEAM);
//	long nJewonTypeC  = pMat->GetJewonType(pBx, CSteelMaterial::BUJE_CVBEAM);
	//상현
	CSteelMaterialData	*pMatData	= pVr->GetSteelMaterialData(STEELMAT_UPPER);
	long nSteelTypeUp = pMatData->m_nType;//pMat->GetSteelType(pBx, CSteelMaterial::BUJE_UVBEAM);
	//하현
	pMatData	= pVr->GetSteelMaterialData(STEELMAT_LOWER);
	long nSteelTypeLo = pMatData->m_nType;//pMat->GetSteelType(pBx, CSteelMaterial::BUJE_LVBEAM);
	//사재
	pMatData	= pVr->GetSteelMaterialData(STEELMAT_DIAGONAL);
	long nSteelTypeC  = pMatData->m_nType;//pMat->GetSteelType(pBx, CSteelMaterial::BUJE_CVBEAM);
	//거더제원
	double dHeightGir   = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dHeightGirN  = pGirNext->GetHeightGirderByStaAng(pBxNext->GetStation());
	double dWidthGirN   = pGirNext->GetWidthOnStation(pBxNext->GetStation(), TRUE, TRUE);
	double dThickWebN   = pGirNext->GetThickFactByStation(pBxNext->GetStation(), 2);
	CDPoint xyStt, xyEnd;
	CDPoint xyCen1, xyCen2, xyCen3, xyCen4;
	double dHeightBeamU = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
	double dHeightBeamC = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_LEFT, TRUE, TRUE);
	double dHeightBeamD = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	long nDrawingType = 0;
	BOOL bJewon = FALSE;
	BOOL bEndDraw = TRUE;

	//상현재(골조선 및 빔)///////////////////////////////////////////////////////////////////////////////
	xyCen1 =  (A[1]+A[2])/2 + vX*ds - vY*dLH1;
	xyCen2 =  A[7] + vX*(dWidthGirN/2-ds) - vY*dRH1;
	//골조선 방향(상부와 하부는 같다.)
	CDPoint vDir = (xyCen2-xyCen1).Unit();
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	//빔
	xyStt = xyCen1 + vDir*pVr->m_dG[0];
	xyEnd = xyCen2 - vDir*pVr->m_dG[1];
	//nDrawingType
	if((pVr->GetType()==TYPE_VBRACING && (nSangNo==1 || nSangNo==3)) || (pVr->GetType()==TYPE_INVVBRACING && (nSangNo==1 || nSangNo==5|| nSangNo==3)))
	{
		if(pVr->GetType()==TYPE_VBRACING)
		{
			if(nSangNo==1)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else
				nDrawingType = CAPlateDrawSteel::FROM_END;
		}
		else
		{
			if(nSangNo==1)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else if(nSangNo==3)
				nDrawingType = CAPlateDrawSteel::FROM_END;
			else if(nSangNo==5)
				nDrawingType = CAPlateDrawSteel::FROM_MID;
		}
		long nInstallDir[4] = {0, 0, 0, 0};
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeUp, nInstallDir, 0, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, nDrawingType, bJewon, bEndDraw, CAPlateDrawSteel::UPPER_VFLANGE);
	}
	//하현재(골조선 및 빔)///////////////////////////////////////////////////////////////////////////////
	xyCen3 = (A[1]+A[2])/2 + vX*ds       - vY*(dHeightGir-dLH2);
	xyCen4 = A[7] + vX*(dWidthGirN/2-ds) - vY*(dHeightGirN-dRH2);
	vDir = (xyCen4-xyCen3).Unit();
	xyStt = xyCen3 + vDir*pVr->m_dG[6];
	xyEnd = xyCen4 - vDir*pVr->m_dG[7];
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	//
	if((pVr->GetType()==TYPE_VBRACING && (nSangNo==2 || nSangNo==4 || nSangNo==5)) || (pVr->GetType()==TYPE_INVVBRACING && (nSangNo==2 || nSangNo==4)))
	{
		if(pVr->GetType()==TYPE_VBRACING)
		{
			if(nSangNo==2)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else if(nSangNo==4)
				nDrawingType = CAPlateDrawSteel::FROM_END;
			else if(nSangNo==5)
				nDrawingType = CAPlateDrawSteel::FROM_MID;
		}
		else
		{
			if(nSangNo==2)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else if(nSangNo==4)
				nDrawingType = CAPlateDrawSteel::FROM_END;
		}
		long nInstallDir[4] = {1, 1, 0, 0};
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeLo, nInstallDir, 0, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, nDrawingType, bJewon, bEndDraw, CAPlateDrawSteel::LOWER_VFLANGE);

	}	
	//사재(좌측골조선 및 빔)///////////////////////////////////////////////////////////////////////////////
	if((pVr->GetType()==TYPE_VBRACING && (nSangNo==1 || nSangNo==5)) || (pVr->GetType()==TYPE_INVVBRACING && (nSangNo==2 || nSangNo==5)))
	{
		if(pVr->GetType()==TYPE_VBRACING)//V type
		{
			vDir = ((xyCen3+xyCen4)/2-xyCen1).Unit();
			xyStt = xyCen1 + vDir*pVr->m_dG[2];
			xyEnd = (xyCen3+xyCen4)/2 - vDir*pVr->m_dG[3];
		}
		else  //Inv VType  
		{
			vDir = ((xyCen1+xyCen2)/2-xyCen3).Unit();
			xyStt = xyCen3 + vDir*pVr->m_dG[2];
			xyEnd = (xyCen1+xyCen2)/2 - vDir*pVr->m_dG[3];
		}
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		//nDrawingType설정

		if(pVr->GetType()==TYPE_VBRACING)
		{
			if(nSangNo==1)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else if(nSangNo==5)
				nDrawingType = CAPlateDrawSteel::FROM_END;
		}
		else
		{
			if(nSangNo==2)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else if(nSangNo==5)
				nDrawingType = CAPlateDrawSteel::FROM_END;
		}

		long nInstallDir[4] = {0, 1, 0, 0};
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeC, nInstallDir, 0, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, nDrawingType, bJewon, bEndDraw, CAPlateDrawSteel::LEFT_VFLANGE);
	}

	//사재(우측)///////////////////////////////////////////////////////////////////////////////
	if((pVr->GetType()==TYPE_VBRACING && (nSangNo==3 || nSangNo==5)) || (pVr->GetType()==TYPE_INVVBRACING && (nSangNo==4 || nSangNo==5)))
	{
		if(pVr->GetType()==TYPE_VBRACING)//V type
		{
			vDir = ((xyCen3+xyCen4)/2-xyCen2).Unit();
			xyStt = xyCen2 + vDir*pVr->m_dG[4];
			xyEnd = (xyCen3+xyCen4)/2 - vDir*pVr->m_dG[5];
		}
		else  //Inv VType
		{
			vDir = ((xyCen1+xyCen2)/2-xyCen4).Unit();
			xyStt = xyCen4 + vDir*pVr->m_dG[4];
			xyEnd = (xyCen1+xyCen2)/2 - vDir*pVr->m_dG[5];
		}
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		//nDrawingType설정
		if(pVr->GetType()==TYPE_VBRACING)
		{
			if(nSangNo==3)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else if(nSangNo==5)
				nDrawingType = CAPlateDrawSteel::FROM_END;
		}
		else
		{
			if(nSangNo==4)
				nDrawingType = CAPlateDrawSteel::FROM_STT;
			else if(nSangNo==5)
				nDrawingType = CAPlateDrawSteel::FROM_END;
		}
		long nInstallDir[4] = {1, 1, 0, 0};
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeC, nInstallDir, 0, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, nDrawingType, bJewon, bEndDraw, CAPlateDrawSteel::RIGHT_VFLANGE);

	}
	*pDom << Dom;

}

void CAPlateDrawCross::DrawVBracingBeamOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bJewon)
{
	if(pBx==NULL) return;	
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateGirderApp *pGirNext = pGir->m_pRight;
	CARoadOptionStd *pOptStd  = m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBxNext = (CPlateBasicIndex*)pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CAPlateDrawSteel DrawSteel(m_pDataMng);
	CVBracing        *pVr     = pBx->GetSection()->GetVBracing();

	if(pVr==NULL) return;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CDPoint vX(1,0), vY(0,1);
	//변수대입
	double dLH1 = pVr->m_dLH1;
	double dRH1 = pVr->m_dRH1;
	double dLH2 = pVr->m_dLH2;
	double dRH2 = pVr->m_dRH2;
	double ds   = pVr->m_dS;
	//기초설정(형강타입 및 제원)	
	long nSteelTypeUp = pVr->GetSteelMaterialData(STEELMAT_UPPER)->m_nType;		//상현	
	long nSteelTypeLo = pVr->GetSteelMaterialData(STEELMAT_LOWER)->m_nType;		//하현	
	long nSteelTypeC  = pVr->GetSteelMaterialData(STEELMAT_DIAGONAL)->m_nType;	//사재
	//거더제원
	double dHeightGir   = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dHeightGirN  = pGirNext->GetHeightGirderByStaAng(pBxNext->GetStation());
	double dWidthGirN   = pGirNext->GetWidthOnStation(pBxNext->GetStation(), TRUE, TRUE);
	CDPoint xyStt, xyEnd;
	CDPoint xyCen1, xyCen2, xyCen3, xyCen4;
	
	//상현재(골조선 및 빔)///////////////////////////////////////////////////////////////////////////////
	xyCen1 =  (A[1]+A[2])/2 + vX*ds - vY*dLH1;
	xyCen2 =  A[7] + vX*(dWidthGirN/2-ds) - vY*dRH1;
	//골조선 방향(상부와 하부는 같다.)
	CDPoint vDir = (xyCen2-xyCen1).Unit();
	//골조선
	pOptStd->SetEnvType(&Dom, HCAD_CENT);	
	Dom.MoveTo(xyCen1);  
	Dom.LineTo(xyCen2);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	//빔
	
	xyStt = xyCen1 + vDir*pVr->m_dG[0];
	xyEnd = xyCen2 - vDir*pVr->m_dG[1];

	long nInstallDir[4] = {0, 0, 0, 0};
	DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeUp, nInstallDir, 0, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, CAPlateDrawSteel::ALL, bJewon, FALSE,  CAPlateDrawSteel::UPPER_VFLANGE);

	//하현재(골조선 및 빔)///////////////////////////////////////////////////////////////////////////////
	xyCen3 = (A[1]+A[2])/2 + vX*ds       - vY*(dHeightGir-dLH2);
	xyCen4 = A[7] + vX*(dWidthGirN/2-ds) - vY*(dHeightGirN-dRH2);
	vDir = (xyCen4-xyCen3).Unit();
	xyStt = xyCen3 + vDir*pVr->m_dG[6];
	xyEnd = xyCen4 - vDir*pVr->m_dG[7];
	//골조선
	pOptStd->SetEnvType(&Dom, HCAD_CENT);	
	Dom.MoveTo(xyCen3);  
	Dom.LineTo(xyCen4);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	//빔
	long nInstallDir1[4] = {1, 1, 1, 1};
	DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeLo, nInstallDir1, 0, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, CAPlateDrawSteel::ALL, bJewon, FALSE,  CAPlateDrawSteel::LOWER_VFLANGE);

	//사재(좌측골조선 및 빔)///////////////////////////////////////////////////////////////////////////////
	CDPoint xyFrameStt, xyFrameEnd;
	for(long n=0; n<2; n++)
	{
		if(pVr->GetType()==TYPE_VBRACING)//V type
		{
			if(n==0)
			{
				vDir = ((xyCen3+xyCen4)/2-xyCen1).Unit();
				xyStt = xyCen1 + vDir*pVr->m_dG[2];
				xyEnd = (xyCen3+xyCen4)/2 - vDir*pVr->m_dG[3];
				xyFrameStt = xyCen1;
				xyFrameEnd = (xyCen3+xyCen4)/2;
			}
			else
			{
				vDir = (xyCen2-(xyCen3+xyCen4)/2).Unit();
				xyStt = (xyCen3+xyCen4)/2 + vDir*pVr->m_dG[5];
				xyEnd = xyCen2 - vDir*pVr->m_dG[4];
				xyFrameStt = (xyCen3+xyCen4)/2;
				xyFrameEnd = xyCen2;
			}
		}
		else  //Inv VType  
		{
			if(n==0)
			{
				vDir = ((xyCen1+xyCen2)/2-xyCen3).Unit();
				xyStt = xyCen3 + vDir*pVr->m_dG[2];
				xyEnd = (xyCen1+xyCen2)/2 - vDir*pVr->m_dG[3];
				xyFrameStt = xyCen3;
				xyFrameEnd = (xyCen1+xyCen2)/2;
			}
			else
			{
				vDir = (xyCen4-(xyCen1+xyCen2)/2).Unit();
				xyStt = (xyCen1+xyCen2)/2 + vDir*pVr->m_dG[5];
				xyEnd = xyCen4  - vDir*pVr->m_dG[4];
				xyFrameStt = (xyCen1+xyCen2)/2;
				xyFrameEnd = xyCen4;
			}
		}	
		pOptStd->SetEnvType(&Dom, HCAD_CENT);	
		Dom.MoveTo(xyFrameStt);  //중심선
		Dom.LineTo(xyFrameEnd);
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		long nVBeamPos = n==0 ?  CAPlateDrawSteel::LEFT_VFLANGE : CAPlateDrawSteel::RIGHT_VFLANGE;
		long nInstallDir[4] = {0, 0, 0, 0};
		DrawSteel.DrawBeam(&Dom, pBx, nSteelTypeC, nInstallDir, 0, xyStt, xyEnd, CAPlateDrawSteel::VBRACING, CAPlateDrawSteel::ALL, bJewon, FALSE,  nVBeamPos);
	}
	//상하현재 시작점 및 끝점 중심선///////////////////////////////////////////////////////////////////////////////
	pOptStd->SetEnvType(&Dom, HCAD_CENT);	
	Dom.MoveTo(xyCen1);
	Dom.LineTo(xyCen3);
	Dom.MoveTo(xyCen2);
	Dom.LineTo(xyCen4);

	*pDom << Dom;


}
void CAPlateDrawCross::DimVBracingFront(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;	

	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	CPlateBridgeApp *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateGirderApp *pGirNext = pGir->m_pRight;
	CARoadOptionStd *pOptStd  = m_pDataMng->GetOptionStd();
	CPlateBasicIndex *pBxNext = (CPlateBasicIndex*)pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CAPlateDrawSteel DrawSteel(m_pDataMng);
	CVBracing *pVr     = pBx->GetSection()->GetVBracing();
	CVBracing *pVrN    = pBxNext->GetSection()->GetVBracing();

	if(!pVr || !pVrN) return;

	//
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CDPoint vX(1,0), vY(0,1), xyStt, xyEnd;
	double dHeightGir    = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dWidthGir     = pGir->GetWidthOnStation(pBx->GetStation(), TRUE, TRUE);
	double dThickLower   = pGir->GetThickFactByStation(pBx->GetStation(), 1);
	double dThickWeb     = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dHeightGirN   = pGirNext->GetHeightGirderByStaAng(pBxNext->GetStation());
	double dWidthGirN    = pGirNext->GetWidthOnStation(pBxNext->GetStation(), TRUE, TRUE);
	double dThickLowerN  = pGirNext->GetThickFactByStation(pBxNext->GetStation(), 1);
	double dThickWebN    = pGirNext->GetThickFactByStation(pBxNext->GetStation(), 2);
	double dWStiffLR = pVr->m_pVStiff->m_dWidth[1];//좌측거더오른쪽
	double dWStiffRL = pVrN->m_pVStiff->m_dWidth[0];//우측거더왼쪽
	//
	//기초설정(형강타입 및 제원)
//	long nSteelTypeUp = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_UVBEAM);
//	long nSteelTypeLo = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_LVBEAM);
//	long nSteelTypeC  = pMat->GetSteelType(pBx, CSteelMaterial::BUJE_CVBEAM);
//	long nJewonTypeUp = pMat->GetJewonType(pBx, CSteelMaterial::BUJE_UVBEAM);
//	long nJewonTypeLo = pMat->GetJewonType(pBx, CSteelMaterial::BUJE_LVBEAM);
//	long nJewonTypeC  = pMat->GetJewonType(pBx, CSteelMaterial::BUJE_CVBEAM);

	//골조선 방향(상부, 하부)
	CDPoint vDir;//부재두께 사먹일때 사용
	if(pVr->m_nLevelType==0) vDir = CDPoint(1,0);
	else					 vDir = ((A[7]+vX*dWidthGirN/2-vY*(pVr->m_dRH1+pVr->m_duR1))
		                           -((A[1]+A[2])/2-vY*(pVr->m_dLH1+pVr->m_duL1))).Unit();
	double dLH1 = pVr->m_dLH1;	double dRH1 = pVr->m_dRH1;
	double dLH2 = pVr->m_dLH2;	double dRH2 = pVr->m_dRH2;
	//이음판에 용접되는 부분의 부재 높이(L형강의 경우는 수직력때문에 긴쪽을 용접함)
	double dHeightBeamU = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
	double dHeightBeamD = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	double dHeightBeamC = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_LEFT, TRUE, TRUE);
	double dCenBeamU = pBx->GetSection()->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
	double dCenBeamD = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);
	if(!pOpt->GetInstallBeamAtCenter())
	{
		dCenBeamU = dHeightBeamU/2;
		dCenBeamD = dHeightBeamC/2;
	}

	CDPoint xyCen1 = A[2] - vX*(dWidthGir/2-dThickWeb/2-dWStiffLR)   - vY*(dLH1 + (dHeightBeamU-dCenBeamU)/vDir.x);
	CDPoint xyCen2 = A[7] + vX*(dWidthGirN/2-dThickWebN/2-dWStiffRL) - vY*(dRH1 + (dHeightBeamU-dCenBeamU)/vDir.x);
	CDPoint xyCen3 = A[2] - vX*(dWidthGir/2-dThickWeb/2-dWStiffLR)   - vY*(dHeightGir-dLH2-(dHeightBeamD-dCenBeamD)/vDir.x);
	CDPoint xyCen4 = A[7] + vX*(dWidthGirN/2-dThickWebN/2-dWStiffRL) - vY*(dHeightGir-dRH2-(dHeightBeamD-dCenBeamD)/vDir.x);
	//
	CDomyun Dom(pDom);	
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	long nDimDan =0;
	//좌측
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(A[1].x, A[1].y-dHeightGir, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_dLH2, nDimDan, "LH2", TRUE);
	//
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyCen3.x+pVr->m_ddL4, xyCen3.y-pVr->m_ddL2, nDimDan, "dL2");
	Dom.DimLineTo(pVr->m_ddL2, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_ddL1, nDimDan, "dL1", TRUE);
	//
	Dom.DimMoveTo(xyCen1.x+pVr->m_duL4, xyCen1.y-pVr->m_duL2, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_duL2, nDimDan, "uL2", TRUE);
	Dom.DimLineToOver(pVr->m_duL1, nDimDan, "uL1", TRUE);
	//
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(A[1].x, A[1].y-pVr->m_dLH1, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_dLH1, nDimDan, "LH1", TRUE);
	//우측
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(A[7].x+dWidthGirN, A[7].y-dHeightGirN, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_dRH2, nDimDan, "RH2", TRUE);
	//
	if(!pVr->m_bSameLeftRight)
	{
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyCen4.x+pVr->m_ddR3, xyCen4.y-pVr->m_ddR2, nDimDan, "dR2");
		Dom.DimLineTo(pVr->m_ddR2, nDimDan, "", TRUE);
		Dom.DimLineTo(pVr->m_ddR1, nDimDan, "dR1", TRUE);
		//
		Dom.DimMoveTo(xyCen2.x+pVr->m_duR3, xyCen2.y-pVr->m_duR2, nDimDan, "", TRUE);
		Dom.DimLineTo(pVr->m_duR2, nDimDan, "uR2", TRUE);
		Dom.DimLineToOver(pVr->m_duR1, nDimDan, "uR1", TRUE);
	}
	//
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(A[7].x+dWidthGirN, A[7].y-pVr->m_dRH1, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_dRH1, nDimDan, "RH1", TRUE);
	//
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyCen1.x-pVr->m_duL3, xyCen1.y-pVr->m_duL2, nDimDan, "uL3");
	Dom.DimLineTo(pVr->m_duL3, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_duL4, nDimDan, "uL4", TRUE);
	//
	if(!pVr->m_bSameLeftRight)
	{
		Dom.DimMoveTo(xyCen2.x-pVr->m_duR4, xyCen2.y-pVr->m_duR2, nDimDan, "", TRUE);
		Dom.DimLineTo(pVr->m_duR4, nDimDan, "uR4", TRUE);
		Dom.DimLineToOver(pVr->m_duR3, nDimDan, "uR3", TRUE);
	}
	//
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyCen3.x-pVr->m_ddL3, xyCen3.y-dHeightBeamD/vDir.x, nDimDan, "dL3");
	Dom.DimLineTo(pVr->m_ddL3, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_ddL4, nDimDan, "dL4", TRUE);
	//
	if(!pVr->m_bSameLeftRight)
	{
		Dom.DimMoveTo(xyCen4.x-pVr->m_ddR4, xyCen4.y-dHeightBeamD/vDir.x, nDimDan, "", TRUE);
		Dom.DimLineTo(pVr->m_ddR4, nDimDan, "dR4", TRUE);
		Dom.DimLineToOver(pVr->m_ddR3, nDimDan, "dR3", TRUE);
	}
	//중앙부이음판
	CDPoint xyCen5;
	if(pVr->GetType()==TYPE_VBRACING)  //V 타입
		xyCen5 = (xyCen3+xyCen4)/2;
	if(pVr->GetType()==TYPE_INVVBRACING)  //역 V 타입
		xyCen5 = (xyCen1+xyCen2)/2;

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyCen5.x-pVr->m_dc3, xyCen5.y-pVr->m_dc2, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_dc3, nDimDan, "C3", TRUE);
	if(!pVr->m_bSameLeftRight)
		Dom.DimLineTo(pVr->m_dc4, nDimDan, "C4", TRUE);
	
	//좌측 수직보강재 폭
	if(pVr->m_vW1_G>0 && pBridge->m_nInstallVStiffOnCrossBeam>0 && pBx->GetNumberGirder()==0)
	{
		xyStt = (A[1]+A[2])/2-vY*(dHeightGir+dThickLower)-vX*(dThickWeb/2+pVr->m_vW1_G);
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
		Dom.DimLineTo(pVr->m_vW1_G, 0, "vW1G");
	}
	xyStt = (A[1]+A[2])/2-vY*(dHeightGir+dThickLower)+vX*(dThickWeb/2);
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineTo(pVr->m_vW_G, 0, "vWG");
	Dom.CircleArrow(xyStt.x+100, xyStt.y+100, Dom.Always(10), Dom.Always(10), 7, "vTG", "");
	//우측 수직보강재 폭
	if(pVr->m_vW1_G>0 && pBridge->m_nInstallVStiffOnCrossBeam>0 && pBx->GetNumberGirder()==pBridge->GetGirdersu()-2)
	{
		xyStt = A[7]+vX*(dWidthGirN/2+dThickWeb/2)-vY*(dHeightGirN+dThickLowerN);
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
		Dom.DimLineTo(pVr->m_vW1_G, 0, "vW1G");
	}
	xyStt = A[7]+vX*(dWidthGirN/2)-vY*(dHeightGirN+dThickLowerN)-vX*(dThickWebN/2+pVr->m_vW_G);
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineTo(pVr->m_vW_G, 0, "vWG");
	//
	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyCen5.x+pVr->m_dc4, xyCen5.y-pVr->m_dc2, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_dc2, nDimDan, "C2", TRUE);
	Dom.DimLineTo(pVr->m_dc1, nDimDan, "C1", TRUE);
	//
	Dom.SetDirection("TOP");
	double dLevel=A[2].y>A[7].y?A[2].y:A[7].y;
	Dom.DimMoveTo(((A[1]+A[2])/2).x, dLevel, nDimDan, "");
	Dom.DimLineTo(pVr->m_dS, nDimDan, "S", TRUE);
	//볼트홀상단
	Dom.SetDirection("LEFT");
	double dLowerGap = (pVr->m_duL1+pVr->m_duL2-(pVr->m_uB*pVr->m_uC))/2;
	xyStt  = pVr->xyBoltPos[0];
	Dom.DimMoveTo(xyStt.x, xyStt.y-dLowerGap, 0, "", TRUE);
	Dom.DimLineToExtend(dLowerGap, 0, "", COMMA(dLowerGap), TRUE);
	Dom.DimLineTo(pVr->m_uB*pVr->m_uC, 0, "uB@uC", TRUE);
	Dom.DimLineToExtend(dLowerGap, 0,"",  COMMA(dLowerGap), TRUE);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x-pVr->m_duE, xyStt.y, 0, "", TRUE);
	Dom.DimLineTo(pVr->m_duE, 0, "uE", TRUE);
	//볼트홀하단
	Dom.SetDirection("LEFT");
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	dLowerGap = (pVr->m_ddL1+pVr->m_ddL2-(pVr->m_dB*pVr->m_dC))/2;
	xyStt  = pVr->xyBoltPos[1];
	Dom.DimMoveTo(xyStt.x, xyStt.y-dLowerGap, 0, "", TRUE);
	Dom.DimLineToExtend(dLowerGap, 0, "", COMMA(dLowerGap), TRUE);
	Dom.DimLineTo(pVr->m_dB*pVr->m_dC, 0, "dB@dC", TRUE);
	Dom.DimLineToExtend(dLowerGap, 0,"",  COMMA(dLowerGap), TRUE);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x-pVr->m_ddE, xyStt.y, 0, "", TRUE);
	Dom.DimLineTo(pVr->m_ddE, 0, "dE", TRUE);
	
	*pDom << Dom;
}

//<summary>
//수직브레이싱 이음판전체
//</summary>
//<param name="*pDom">도면</param>
//<param name="*pBx">수직브레이싱 bx</param>
void CAPlateDrawCross::DrawVBracingSpliceOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CVBracing* pVr = pBx->GetSection()->GetVBracing();
	CDomyun Dom(pDom);

	if(pVr==NULL) return;

	CPlatePieceObject pObj1(pGir);
	CPlatePieceObject pObj2(pGir);
	CPlatePieceObject pObj3(pGir);
	CPlatePieceObject pObj4(pGir);
	CPlatePieceObject pObj5(pGir);
	//LeftTop
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	pVr->CreatePieceObj(&pObj1, TRUE, TRUE, 0);
	pObj1.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//LeftDown
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	pVr->CreatePieceObj(&pObj2, TRUE, TRUE, 1);
	pObj2.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//RightTop
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	pVr->CreatePieceObj(&pObj3, TRUE, TRUE, 2);
	pObj3.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//RightDown
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	pVr->CreatePieceObj(&pObj4, TRUE, TRUE, 3);
	pObj4.DrawPieceOutline(&Dom, pOptStd, TRUE);
	//Cen
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	pVr->CreatePieceObj(&pObj5, TRUE, TRUE, 4);
	pObj5.DrawPieceOutline(&Dom, pOptStd, TRUE);

	if(bDim)
	{
		for(long nPos=0; nPos<5; nPos++)
			DimVBracingFrontSplice(&Dom, pBx, nPos);
	}

	*pDom << Dom;
}

void CAPlateDrawCross::DrawVBracingSpliceSangseOnCRBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim, long nDetailNum)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CVBracing        *pVr     = pBx->GetSection()->GetVBracing();

	if(nDetailNum==3 && nDetailNum==4)
	{
		pGir = pBridge->GetGirder(pBridge->GetGirdersu()-2);
		pBx  = pBx->GetBxMatchByCrossBeam(pGir);	//끝에서 두번째 거더의 BX
		pVr  = pBx->GetSection()->GetVBracing();
	}
	long nPos=0;
	if(nDetailNum==1)			nPos = CVBracing::LT;
	else if(nDetailNum==2)		nPos = CVBracing::LD;
	else if(nDetailNum==3)		nPos = CVBracing::RT;
	else if(nDetailNum==4)		nPos = CVBracing::RD;
	else if(nDetailNum==5)		nPos = CVBracing::CEN;
	
	CDomyun Dom(pDom);

	CPlatePieceObject pObj(pGir);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	pVr->CreatePieceObj(&pObj, TRUE, TRUE, nPos);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
	*pDom << Dom;

	if(bDim)
	{

	}
	*pDom << Dom;

}
//<summary>
//수직브레이싱 이음판전체
//</summary>
//<param name="*pDom">도면</param>
//<param name="*pBx">수직브레이싱 bx</param>
void CAPlateDrawCross::DimVBracingFrontSplice(CDomyun *pDom, CPlateBasicIndex *pBx, long nPos)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CSection         *pSec    = pBx->GetSection();
	CVBracing        *pVr     = pSec->GetVBracing();
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	CDPoint xyStt(0,0);
	
	switch(nPos)
	{
	case CVBracing::LT:
		{
			CDPoint xyOri	= pVr->GetPointFrame(CVBracing::LT);
			CDPoint vDirU	= pVr->GetVectorFrame(CVBracing::UPPER);
			CDPoint vDirL	= pVr->GetVectorFrame(CVBracing::LEFT);

			Dom.SetDirection("TOP");

			xyStt	= xyOri+vDirU*pVr->m_dG[0];
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineTo(pVr->m_dULG, 0, "uLG");
			Dom.Rotate(xyStt, vDirU);
			*pDom << Dom;
			
			if(pVr->GetType()==TYPE_VBRACING)
			{
				xyStt = xyOri+vDirL*pVr->m_dG[2];
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
				Dom.DimLineTo(pVr->m_dC1LG, 0, "c1LG");
				Dom.Rotate(xyStt, vDirL);
				*pDom << Dom;
			}
			break;
		}
	case CVBracing::LD:
		{
			CDPoint xyOri	= pVr->GetPointFrame(CVBracing::LD);
			CDPoint vDirD	= pVr->GetVectorFrame(CVBracing::LOWER);
			CDPoint vDirL	= pVr->GetVectorFrame(CVBracing::LEFT);

			Dom.SetDirection("BOTTOM");

			xyStt	= xyOri+vDirD*pVr->m_dG[6];
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineTo(pVr->m_dDLG, 0, "dLG");
			Dom.Rotate(xyStt, vDirD);
			*pDom << Dom;
			
			if(pVr->GetType()==TYPE_INVVBRACING)
			{
				xyStt = xyOri+vDirL*pVr->m_dG[2];
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
				Dom.DimLineTo(pVr->m_dC1LG, 0, "c1LG");
				Dom.Rotate(xyStt, vDirL);
				*pDom << Dom;
			}
			break;
		}
	case CVBracing::RT:
		{
			CDPoint xyOri	= pVr->GetPointFrame(CVBracing::RT);
			CDPoint vDirU	= pVr->GetVectorFrame(CVBracing::UPPER);
			CDPoint vDirR	= -pVr->GetVectorFrame(CVBracing::RIGHT);

			Dom.SetDirection("TOP");

			xyStt	= xyOri-vDirU*pVr->m_dG[1];
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineTo(-pVr->m_dURG, 0, "uRG");
			Dom.Rotate(xyStt, vDirU);
			*pDom << Dom;
			
			if(pVr->GetType()==TYPE_VBRACING)
			{
				xyStt = xyOri-vDirR*pVr->m_dG[4];
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
				Dom.DimLineTo(-pVr->m_dC2RG, 0, "c2RG");
				Dom.Rotate(xyStt, vDirR);
				*pDom << Dom;
			}
			break;
		}
	case CVBracing::RD:
		{
			CDPoint xyOri	= pVr->GetPointFrame(CVBracing::RD);
			CDPoint vDirD	= pVr->GetVectorFrame(CVBracing::LOWER);
			CDPoint vDirR	= -pVr->GetVectorFrame(CVBracing::RIGHT);

			Dom.SetDirection("BOTTOM");

			xyStt	= xyOri-vDirD*pVr->m_dG[7];
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineTo(-pVr->m_dDRG, 0, "dRG");
			Dom.Rotate(xyStt, vDirD);
			*pDom << Dom;
			
			if(pVr->GetType()==TYPE_INVVBRACING)
			{
				xyStt = xyOri-vDirR*pVr->m_dG[4];
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
				Dom.DimLineTo(-pVr->m_dC2RG, 0, "c2RG");
				Dom.Rotate(xyStt, vDirR);
				*pDom << Dom;
			}
			break;
		}
	case CVBracing::CEN:
		{
			CDPoint xyOri	= pVr->GetPointFrame(CVBracing::CEN);
			CDPoint vDirL	= pVr->GetVectorFrame(CVBracing::LEFT);
			CDPoint vDirR	= -pVr->GetVectorFrame(CVBracing::RIGHT);

			if(pVr->GetType()==TYPE_VBRACING)			Dom.SetDirection("TOP");
			else if(pVr->GetType()==TYPE_INVVBRACING)	Dom.SetDirection("BOTTOM");
			
			xyStt = xyOri-vDirL*pVr->m_dG[3];
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineTo(-pVr->m_dC1RG, 0, "c1RG");
			Dom.Rotate(xyStt, vDirL);
			*pDom << Dom;

			xyStt = xyOri+vDirR*pVr->m_dG[5];
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineTo(pVr->m_dC2LG, 0, "c2LG");
			Dom.Rotate(xyStt, vDirR);
			*pDom << Dom;
			break;
		}
	}

	*pDom << Dom;
}
void CAPlateDrawCross::DrawInputCRBeamPlan(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bPaintMap)
{
	if(pBx==NULL) return;
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirR   = pGir->m_pRight;
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();

	CAPlateDrawPyung DrawPyung(m_pDataMng);
	double dWGir   = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dGirLen = 500;
	CDPoint A[10];
	pC->GetCrossXyDom(A);

	CDomyun Dom(pDom);
	BOOL bUpper = TRUE;
	// CROSS BEAM : 상면
	DrawPyung.DrawCRBeamWebOnBx(&Dom,pBx,bUpper,TRUE,FALSE);
	if(pC->m_nLGirSpType)
	{
		DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom,pBx,bUpper,TRUE, TRUE, FALSE, FALSE,TRUE);
		if(pC->GetType()==CR_TYPE_MANUFACTURE)	//제작단면 일 경우만
			DrawPyung.DrawCRBeamGassetWebOnBx(&Dom,pBx,bUpper,TRUE);
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,TRUE,TRUE);
	}
	DrawPyung.DrawCRBeamVStiffnerOnBx(&Dom,pBx,bUpper,TRUE,FALSE);
	DrawPyung.DrawCRBeamAnchorBarOnBx(&Dom,pBx,TRUE,FALSE);

	// CROSS BEAM : 하면
	bUpper = FALSE;
	DrawPyung.DrawCRBeamWebOnBx(&Dom,pBx,bUpper,FALSE,TRUE);
	if(pC->m_nRGirSpType)
	{
		DrawPyung.DrawCRBeamGassetFlangeOnBx(&Dom,pBx,bUpper,FALSE, TRUE, FALSE, FALSE,TRUE);
		if(pC->GetType()==CR_TYPE_MANUFACTURE)	//제작단면 일 경우만
			DrawPyung.DrawCRBeamGassetWebOnBx(&Dom,pBx,bUpper,FALSE);
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,bUpper,FALSE,TRUE);
	}
	//가로보 복부 수직보강재
	DrawPyung.DrawCRBeamVStiffnerOnBx(&Dom,pBx,bUpper,FALSE,TRUE);
	//	filler
	if(pC->m_nFillerUseType==1)//한쪽
	{
		DrawPyung.DrawCRBeamFillerOneOnBx(&Dom,pBx,TRUE);
		DrawPyung.DrawCRBeamFillerOneOnBx(&Dom,pBxNext,FALSE);
	}
	else if(pC->m_nFillerUseType==2)//양쪽
	{
		DrawPyung.DrawCRBeamFillerBothOnBx(&Dom,pBx,TRUE);
		DrawPyung.DrawCRBeamFillerBothOnBx(&Dom,pBxNext,FALSE);
	}

	//거더 복부 수직보강재	- 제작타입의 경우, 수직보강재가 "있지만" 그리지 않는다.
	if(pC->GetType() != CR_TYPE_MANUFACTURE)
	{
		if(pBx->IsJijum())		DrawPyung.DrawJiJumStiff(&Dom, pBx, pBx);
		else					DrawPyung.DrawVStiffOnBx(&Dom, pBx);

		if(pBxNext->IsJijum())	DrawPyung.DrawJiJumStiff(&Dom, pBxNext, pBxNext);
		else					DrawPyung.DrawVStiffOnBx(&Dom, pBxNext);
	}

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	//거더라인
	BOOL	bDraw	= FALSE;
	double	dHeightGirder	= pBx->GetHeightGirderByBx();
	if(pC->m_dLH1==0||pC->m_dRH1==0)	bDraw	= TRUE;
	DrawPyung.DrawGirderWithenStation(&Dom, pBx, pBx->GetStation()-dGirLen, pBx->GetStation()+dGirLen, TRUE, FALSE, bDraw);

	bDraw	= FALSE;
	if(pC->m_H_CR >= dHeightGirder - pC->m_dRH1 - pC->m_T1_CR - pC->m_T2_CR)
		bDraw	= TRUE;
	DrawPyung.DrawGirderWithenStation(&Dom, pBxNext, pBxNext->GetStation()-dGirLen, pBxNext->GetStation()+dGirLen, FALSE, FALSE, bDraw);
	// 이동 및 회전 : 좌측복부판의 중심을 (0,0)으로
	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CDPoint GirL = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint GirR = pGir->m_pRight->GetXyGirderDis(pBxSec->GetStation());
	CDPoint vCros = -(GirL-GirR).Unit();	// 가로보의 각도
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vAng  = CDPoint(0,1);
	CDPoint vAngSec  = pBx->GetAngleSection();

	Dom.Move(-GirL.x,-GirL.y);
	Dom.Rotate(-vGir.GetAngleRadian(),0,0);
	Dom.Rotate(vAng.GetAngleRadian(),0,0);
	//DIMENSION 
	double BX=0;
	double BY=0;
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("TOP");

	DimCRBeamPlanTop(&Dom,pBx,BX,BY);

	if(bPaintMap)							DimCRBeamPlanPaint(&Dom,pBx,BX,BY);
	else if(pC->GetType()!=CR_TYPE_HSTEEL)	DimCRBeamPlanLeftRight(&Dom,pBx,BX,BY);

	Dom.SetCalcExtRect();
	CDRect BoundRect = Dom.GetExtRect();
	*pDom << Dom;

	//상부이음판 및 하부이음판입력부 추가(볼트이음과 용접이음이 같이 존재시)
	double dMoveX=0;
	double dMoveY=0;
	CDRect BoundRect1;
	if(pC->m_nLGirSpType && !pC->m_nRGirSpType)//하부이음판의 추가
	{
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,FALSE,TRUE,TRUE);
		dMoveX = (~(A[9]-A[2]) + dWGir*2);
		// 이동 및 회전 : 좌측복부판의 중심을 (0,0)으로
		Dom.Move(-GirL.x,-GirL.y);
		Dom.Rotate(-vGir.GetAngleRadian(),0,0);
		Dom.Rotate(vAng.GetAngleRadian(),0,0);
		Dom.SetCalcExtRect();
		BoundRect1 = Dom.GetExtRect();
		Dom.TextOut(BoundRect1.left+BoundRect1.Width()/2, BoundRect1.bottom+Dom.Always(5), "하부이음판");
	}
	if(!pC->m_nLGirSpType && pC->m_nRGirSpType)//상부이음판의 추가
	{
		DrawPyung.DrawCRBeamSplicePlateOnBx(&Dom,pBx,TRUE,FALSE,TRUE);
		dMoveX = (~(A[9]-A[5]) + dWGir*2);
		// 이동 및 회전 : 좌측복부판의 중심을 (0,0)으로
		Dom.Move(-GirL.x,-GirL.y);
		Dom.Rotate(-vGir.GetAngleRadian(),0,0);
		Dom.Rotate(vAng.GetAngleRadian(),0,0);
		Dom.SetCalcExtRect();
		BoundRect1 = Dom.GetExtRect();
		Dom.TextOut(BoundRect1.left+BoundRect1.Width()/2, BoundRect1.bottom+Dom.Always(5), "상부이음판");
	}
	
	dMoveY = BoundRect.bottom -BoundRect1.bottom;
	Dom.Move(dMoveX,dMoveY);
	*pDom << Dom;

	if(!bPaintMap)
	{
		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		if(pC->m_nLGirSpType && !pC->m_nRGirSpType)//하부이음판의 추가
			DimCRBeamPlanFlSp(&Dom, pBx, FALSE, TRUE, 0, 0);
		if(!pC->m_nLGirSpType && pC->m_nRGirSpType)//상부이음판의 추가
			DimCRBeamPlanFlSp(&Dom, pBx, TRUE, FALSE, 0, 0);
	}

	Dom.Move(dMoveX,dMoveY);
	*pDom << Dom;

}

void CAPlateDrawCross::DrawInputCRBeamFront(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bPaintMap)
{
	if(pBx==NULL) return;

	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirR   = pGir->m_pRight;
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGirR);
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
 
	CAPlateDrawPyung DrawPyung(m_pDataMng);
	double dWGirR = pGirR->GetWidthOnStation(pBxNext->GetStation(), TRUE);
	double dWebT  = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWebTR = pGirR->GetThickFactByStation(pBxNext->GetStation(), 2);
	double dSharp = pGir->GetLengthSharpOnBx(pBx,  TRUE);
	CDomyun Dom(pDom);

	//정면도
	DrawGirderFront(&Dom, pBx, ALL);
	DrawGirderFront(&Dom, pBxNext, ALL);

	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);// 복부판, 상판, 하판
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCRBeamSpliceOnBx(&Dom, pBx);      // 상하플랜지 이음판, 전단이음판, 모멘트이음판
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCRBeamGassetOnBx(&Dom, pBx);      // 가셋트-상하플랜지판, 복부판, 보강판
	DrawCRBeamVStiffnerOnBx(&Dom, pBx);   // 수직보강재
	DrawCRBeamAnchorBar(&Dom, pBx);       // 슬래브앵커
	DrawFrontHStiffBx(&Dom, pBx);		  // 가로보 부착 수평보강재
	
	DrawHStiffFront(&Dom, pBx, TRUE);	  // 거더 부착 수평보강재
	DrawHStiffFront(&Dom, pBx, FALSE);	  // 거더 부착 수평보강재
	DrawHStiffFront(&Dom, pBxNext, TRUE);	  // 수평보강재
	DrawHStiffFront(&Dom, pBxNext, FALSE);	  // 수평보강재
	
	DrawFrontHGussetSplice(&Dom, pBx, TRUE);
	DrawFrontHGussetSplice(&Dom, pBxNext, TRUE);

	// 치수
	double BX=0,BY=0;

	if(bPaintMap)
	{
		DimCRBeamFrontPaint(&Dom,pBx,BX,BY);		
	}
	else
	{	
		DimCRBeamFrontTop(&Dom,pBx,BX,BY);
		DimCRBeamFrontBottom(&Dom,pBx,BX,BY);
		DimCRBeamFrontRight(&Dom,pBx,BX,BY);
		DimCRBeamFrontLeft(&Dom,pBx,BX,BY);
	}
	//상세원
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);

	CDPoint vDir = pC->GetVectorCross();
	CDPoint vX(1,0), vY(0,1);
	CDPoint xyCen;
	if(!pC->m_nLGirSpType)
	{
		xyCen = (A[1]+A[2])/2+vDir*(pC->m_pVStiff->m_dWidth[1]+dWebT/2+pC->m_dbL4)/vDir.x-vY*(pC->m_dLH1+pC->m_T2_CR);
		if(pC->m_dLH1==0)
			xyCen = A[2] + vDir*(pC->m_pVStiff->m_dWidth[1]+pC->m_dbL4-dSharp)/vDir.x;
	}
	else
	{
		xyCen = A[7]+vX*dWGirR/2-vDir*(pBxNext->GetSection()->GetVStiff()->m_dWidth[0]+dWebTR/2+pC->m_dbR4)/vDir.x-vY*(pC->m_dRH1+pC->m_T2_CR);
		if(pC->m_dRH1==0)
			xyCen = A[7] - vDir*(pBxNext->GetSection()->GetVStiff()->m_dWidth[0]+pC->m_dbR4-dSharp)/vDir.x;
	}
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	if(!pC->m_nLGirSpType || !pC->m_nRGirSpType)
		Dom.Circle(xyCen, pC->m_dwB*3);

	CDPoint xyOrigin = (A[1]+A[2])/2;
	Dom.Move(-xyOrigin);

	*pDom << Dom;
}

void CAPlateDrawCross::DrawInputCRBeamSangse(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirR   = pGir->m_pRight;
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex *pBxR    = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateCrossBeam  *pCR     = pBxR->GetSection()->GetCrossBeam();
	
	CAPlateDrawPyung DrawPyung(m_pDataMng);

	double dWGirR = pGirR->GetWidthOnStation(pBxR->GetStation(), TRUE);
	double dWebT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWebTR = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
	double dSharp = pGirR->GetLengthSharpOnBx(pBx,  TRUE);
	double dScale = 5;
	CDPoint vX(1,0), vY(0,1);
	CDomyun Dom(pDom);
//drawing routine Start
	//정면도
	Dom.SetScaleDim(dScale);
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);//정면도(복부판, 상판, 하판)
	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	DrawCRBeamGassetOnBx(&Dom, pBx);//정면도(가셋트-상하플랜지판, 복부판, 보강판)
	//
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	CDPoint vDir = pC->GetVectorCross();
	CDPoint xyCen;
	if(!pC->m_nLGirSpType)
	{
		xyCen = (A[1]+A[2])/2+vDir*(pC->m_pVStiff->m_dWidth[1]+dWebT/2+pC->m_dbL4)/vDir.x-vY*(pC->m_dLH1+pC->m_T2_CR);
		if(pC->m_dLH1==0)
			xyCen = A[2] + vDir*(pC->m_pVStiff->m_dWidth[1]+pC->m_dbL4-dSharp)/vDir.x;
	}
	else
	{
		xyCen = A[7]+vX*dWGirR/2-vDir*(pBxR->GetSection()->GetVStiff()->m_dWidth[0]+dWebTR/2+pC->m_dbR4)/vDir.x-vY*(pC->m_dRH1+pC->m_T2_CR);
		if(pC->m_dRH1==0)
			xyCen = A[7] - vDir*(pBxR->GetSection()->GetVStiff()->m_dWidth[0]+pC->m_dbR4-dSharp)/vDir.x;
	}
	//치수선
	if(!pC->m_nLGirSpType)
	{
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		Dom.Circle(xyCen, 50/*pC->m_dwB*2*/);
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.TrimByCircle(xyCen, 50/*pC->m_dwB*2*/, TRUE);
		if(pC->m_dwB>0)
		{
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyCen.x, xyCen.y, 0);
			Dom.DimLineTo(pC->m_dwB, 0, "wB");
		}
		if(pC->m_dwH>0)
		{
			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyCen.x, xyCen.y-pC->m_dwH, 0);
			Dom.DimLineTo(pC->m_dwH, 0, "wH");
		}
		Dom.SetDirection("TOP");
		CDPoint xyGisi = xyCen + vDir*(pC->m_dwB-pC->m_dwR) + CDPoint(1,-1).Unit()*pC->m_dwR;
		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(5), Dom.Always(5), 7, "wR");
	}
	else if(!pC->m_nRGirSpType)
	{
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
//		Dom.Circle(xyCen, pC->m_dwB*2);
		Dom.Circle(xyCen, 50);
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
//		Dom.TrimByCircle(xyCen, pC->m_dwB*2, TRUE);
		Dom.TrimByCircle(xyCen, 50, TRUE);
		if(pC->m_dwB>0)
		{
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyCen.x-pC->m_dwB, xyCen.y, 0);
			Dom.DimLineTo(pC->m_dwB, 0, "wB");
		}
		if(pC->m_dwH>0)
		{
			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyCen.x, xyCen.y-pC->m_dwH, 0);
			Dom.DimLineTo(pC->m_dwH, 0, "wH");
		}
		Dom.SetDirection("TOP");
		CDPoint xyGisi = xyCen - vDir*(pC->m_dwB-pC->m_dwR) + CDPoint(-1,-1).Unit()*pC->m_dwR;
		Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(5), Dom.Always(5), 1, "wR");
	}
	//
	CDPoint xyOrigin =xyCen;
//	double dMoveY = max(A[1].y, A[7].y);
//	Dom.Move(-xyOrigin.x-Dom.Always(50), -dMoveY);
	xyOrigin.x += Dom.Always(50);
	Dom.Move(-xyOrigin);
	//
	Dom.RedrawByScale(pDom->GetScaleDim()/dScale);
	*pDom << Dom;
}

void CAPlateDrawCross::DrawCRBeamTotalFrontOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDrawGirder, BOOL bJewon, BOOL bGirderNo)
{
	if(pBx==NULL) return;
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirR   = pGir->m_pRight;
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);

	CDomyun Dom(pDom);
	//정면도
	if(bDrawGirder)
	{
		DrawGirderFront(&Dom, pBx, ALL);
		DrawGirderFront(&Dom, pBxNext, ALL);
	}

	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		pOptStd->SetEnvType(&Dom,HCAD_STLC);
		DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);//정면도(복부판, 상판, 하판)
		pOptStd->SetEnvType(&Dom,HCAD_STLC);
		DrawCRBeamSpliceOnBx(&Dom, pBx);   //정면도 (상하플랜지 이음판, 전단이음판, 모멘트이음판)
		pOptStd->SetEnvType(&Dom,HCAD_STLC);
		DrawCRBeamGassetOnBx(&Dom, pBx);//정면도(가셋트-상하플랜지판, 복부판, 보강판)
		DrawCRBeamVStiffnerOnBx(&Dom, pBx);//정면도(수직보강재)
		DrawCRBeamAnchorBar(&Dom, pBx);//정면도(슬래브앵커)
		DrawFrontHStiffBx(&Dom, pBx);		  // 가로보 부착 수평보강재	
		DrawFrontHGussetSplice(&Dom, pBx, FALSE);
		DrawFrontHGussetSplice(&Dom, pBxNext, FALSE);
	}
	else if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
	{
		DrawVBracingSpliceOnCRBx(&Dom, pBx, FALSE);
		DrawVBracingBeamOnCRBx(&Dom, pBx, bJewon);
		DrawVBracingAnchorBar(&Dom, pBx);
		DrawFrontHGussetSplice(&Dom, pBx, FALSE);
		DrawFrontHGussetSplice(&Dom, pBxNext, FALSE);
	}

	if(bGirderNo==FALSE)
	{
		*pDom << Dom;
		return;
	}
	
	
	CLongArray nArrGir;
	pBridge->GetIndexGirderToCrossBeam(nArrGir, pBx, TRUE);


	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CString cs;
	for(long nG=0; nG<m_pDataMng->GetBridge()->GetGirdersu(); nG++)
	{
		BOOL bMat = FALSE;
		for(long ix =0; ix < nArrGir.GetSize(); ix++)
		{
			if(nG == nArrGir.GetAt(ix))
			{
				bMat = TRUE;
				break;
			}
		}
		if(bMat == FALSE)
			continue;

		CPlateGirderApp *pGir = m_pDataMng->GetBridge()->GetGirder(nG);	
		CDPoint A[HDANLINESU];
		pGir->GetHunchXyDom(pBx, A);
		CDPoint xyCen = (A[1]+A[2])/2 + CDPoint(0, -1)*(pGir->GetHeightGirderByStaAng(pBx->GetStation())+Dom.Always(15));
		
		cs.Format("G%d", nG+1);
		Dom.TextCircle(xyCen.x, xyCen.y, cs);
	}

	*pDom << Dom;
}

//복부판, 상판, 하판(정면도)
void CAPlateDrawCross::DrawCRBeamWebAndFlangeOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirN   = pGir->m_pRight;
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	
	if(pC == NULL)	return;
	if(pGir->m_pRight==NULL)	return;

	CDPoint A[10], B[10];
	pC->GetCrossXyDom(A);
	pGir->m_pRight->GetHunchXyDom(pBx, B);
	CDomyun	Dom(pDom);

	//복부판
	CPlatePieceObject pObj(pGir);
	pObj.m_nMarkBuje = pBx->IsJijum() ? CP_W : CG_W;
	pC->CreatePieceObjPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	//상판, 하판
	double Tu = pC->m_T2_CR;	// 가로보의 상판 두께
	double Tl = pC->m_T1_CR;	// 가로보의 하판 두께 
	CDPoint vDir1 = (A[3] - A[2]).Unit();
	CDPoint vDir2 = (A[5] - A[4]).Unit();
	CDPoint xy, xy1, xy2, xy3;
	pOptStd->SetEnvType(&Dom, HCAD_STLC);	
	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		for(long n = 2; n < 5; n++ )
		{
			xy  = CDPoint(A[n].x, A[n].y);
			xy1 = CDPoint(A[n].x, A[n].y+Tu);
			xy2 = CDPoint(A[n+1].x, A[n+1].y);
			xy3 = CDPoint(A[n+1].x, A[n+1].y+Tu);
			//상부 플레이트
			Dom.LineTo(xy  ,xy2);
			Dom.LineTo(xy1 ,xy3);
			if (n==2)
				Dom.LineTo(xy  ,xy1);
			if (n==4)
				Dom.LineTo(xy2  ,xy3);
			xy  = CDPoint(A[n].x, A[n].y-pC->m_H_CR);
			xy1 = CDPoint(A[n].x, A[n].y-pC->m_H_CR-Tl);
			xy2 = CDPoint(A[n+1].x, A[n+1].y-pC->m_H_CR);
			xy3 = CDPoint(A[n+1].x, A[n+1].y-pC->m_H_CR-Tl);
			//하부 플레이트
			Dom.LineTo(xy  ,xy2);
			Dom.LineTo(xy1 ,xy3);
			if (n==2)
				Dom.LineTo(xy  ,xy1);
			if (n == 4)
				Dom.LineTo(xy2  ,xy3);
		}
	}
	else
	{
/*		//레벨타입이고 양쪽이 모두 볼트이음인 경우는 일반타입인 경우로 그린다.
		if(bLevel && !pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			//상부 플레이트
			xy  = CDPoint(A[0].x, A[0].y);
			xy1 = CDPoint(A[0].x, A[0].y+Tu);
			xy2 = CDPoint(A[7].x, A[7].y);
			xy3 = CDPoint(A[7].x, A[7].y+Tu);
			Dom.LineTo(xy  ,xy2);
			Dom.LineTo(xy1 ,xy3);
			Dom.LineTo(xy, xy1);
			Dom.LineTo(xy2, xy3);
			//하부 플레이트
			xy  = CDPoint(A[0].x, A[0].y-pC->m_H_CR);
			xy1 = CDPoint(A[0].x, A[0].y-pC->m_H_CR-Tl);
			xy2 = CDPoint(A[7].x, A[7].y-pC->m_H_CR);
			xy3 = CDPoint(A[7].x, A[7].y-pC->m_H_CR-Tl);
			Dom.LineTo(xy  ,xy2);
			Dom.LineTo(xy1 ,xy3);
			Dom.LineTo(xy, xy1);
			Dom.LineTo(xy2, xy3);
		}	
		else
		{*/
			for(long n = 0; n < 7; n++ )
			{
				//상부 플레이트
				xy  = CDPoint(A[n].x, A[n].y);
				xy1 = CDPoint(A[n].x, A[n].y+Tu);
				xy2 = CDPoint(A[n+1].x, A[n+1].y);
				xy3 = CDPoint(A[n+1].x, A[n+1].y+Tu);
				Dom.LineTo(xy  ,xy2);
				Dom.LineTo(xy1 ,xy3);
				if (n==2 && pC->m_nLGirSpType)
					Dom.LineTo(xy  ,xy1);
				if (n==4 && pC->m_nRGirSpType)
					Dom.LineTo(xy2  ,xy3);
				if(n==0)
					Dom.LineTo(xy, xy1);
				if(n==6)
					Dom.LineTo(xy2, xy3);
				//하부 플레이트
				if(n==0 || n==6)
				{
					xy  = CDPoint(A[n].x,   A[n].y-pC->m_H_CR)     +vDir1*(pC->m_dbL3-pC->m_dbL4)/fabs(vDir1.x);
					xy1 = CDPoint(A[n].x,   A[n].y-pC->m_H_CR-Tl)  +vDir1*(pC->m_dbL3-pC->m_dbL4)/fabs(vDir1.x);
					xy2 = CDPoint(A[n+1].x, A[n+1].y-pC->m_H_CR)   +vDir2*(pC->m_dbR4-pC->m_dbR3)/fabs(vDir2.x);
					xy3 = CDPoint(A[n+1].x, A[n+1].y-pC->m_H_CR-Tl)+vDir2*(pC->m_dbR4-pC->m_dbR3)/fabs(vDir2.x);
				}
				else
				{
					xy  = CDPoint(A[n].x,   A[n].y-pC->m_H_CR)     ;
					xy1 = CDPoint(A[n].x,   A[n].y-pC->m_H_CR-Tl)  ;
					xy2 = CDPoint(A[n+1].x, A[n+1].y-pC->m_H_CR)   ;
					xy3 = CDPoint(A[n+1].x, A[n+1].y-pC->m_H_CR-Tl);
				}
				Dom.LineTo(xy  ,xy2);
				Dom.LineTo(xy1 ,xy3);
				if (n==2 && pC->m_nLGirSpType)
					Dom.LineTo(xy, xy1);
				if (n==4 && pC->m_nRGirSpType)
					Dom.LineTo(xy2, xy3);
				if(n==0)
					Dom.LineTo(xy, xy1);
				if(n==6)
					Dom.LineTo(xy2, xy3);
			}
//		}
	}

	*pDom << Dom;
}

void CAPlateDrawCross::DrawCRBeamSpliceOnBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeftDraw, BOOL bRightDraw)
{
	if(pBx==NULL) return;
	
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;

	pC->m_dThickFiller = (long)pC->GetFillerThick();
				
	// 상.하 플랜지 이음판
	CDomyun Dom(pDom);
	if(pC->m_nLGirSpType && bLeftDraw)
	{
		DrawFrontSplicePlate_FlangeBx(&Dom,pBx,TRUE);
		DrawFrontSplicePlate_ShearBx(&Dom,pBx,TRUE);
		
		if(pC->m_dThickFiller >= 9)		
			DrawFrontSplicePlate_FillerBx(&Dom,pBx,TRUE);

		DrawFrontSplicePlate_MomentBx(&Dom,pBx,TRUE, TRUE, TRUE);
		DrawFrontSplicePlate_MomentBx(&Dom,pBx,TRUE, FALSE, TRUE);
	}
	if(pC->m_nRGirSpType && bRightDraw)
	{
		DrawFrontSplicePlate_FlangeBx(&Dom,pBx,FALSE);
		DrawFrontSplicePlate_ShearBx(&Dom,pBx,FALSE);

		if(pC->m_dThickFiller >= 9)
			DrawFrontSplicePlate_FillerBx(&Dom,pBx,FALSE);

		DrawFrontSplicePlate_MomentBx(&Dom,pBx,FALSE, TRUE, TRUE);
		DrawFrontSplicePlate_MomentBx(&Dom,pBx,FALSE, FALSE, TRUE);
	}

	*pDom << Dom;
}

void CAPlateDrawCross::DrawCRBeamGassetOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	if(pBx==NULL) return;
	if(pBx->GetSection()->GetCrossBeam()==NULL) return;

	CDomyun Dom(pDom);

	if(pC->m_nLGirSpType)
	{
		DrawCRBeamFrontGassetPlate_FlangeBx(&Dom,pBx,TRUE);
		if(pC->GetType()==CR_TYPE_MANUFACTURE)	//제작타입일 경우만
			DrawCRBeamFrontGassetPlate_WebLeftBx(&Dom,pBx);
	}
	if(pC->m_nRGirSpType)
	{
		DrawCRBeamFrontGassetPlate_FlangeBx(&Dom,pBx,FALSE);
		if(pC->GetType()==CR_TYPE_MANUFACTURE)	//제작타입일 경우만
			DrawCRBeamFrontGassetPlate_WebRightBx(&Dom,pBx);
	}
	if(pC->m_cGasset_CR)
	{
		if(pC->m_nLGirSpType)
			DrawCRBeamFrontGassetSupportPlateLeftBx(&Dom,pBx); //용접타입
		else
			DrawCRBeamFrontGassetSupportPlateByBoltLeftBx(&Dom,pBx); //볼트이음타입

		if(pC->m_nRGirSpType)
			DrawCRBeamFrontGassetSupportPlateRightBx(&Dom,pBx);//용접타입
		else
			DrawCRBeamFrontGassetSupportPlateByBoltRightBx(&Dom,pBx);//볼트이음타입
	}

	*pDom << Dom;
}

//확장스캘롭이 아닐 경우 에러....(수정요함. 나중에ksg)
void CAPlateDrawCross::DrawCRBeamFrontGassetPlate_FlangeBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex *pBxR    = pBx->GetBxMatchByCrossBeam(pGir->m_pRight ? pGir->m_pRight : pGir);

	double dTWeb  = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dTWebR = pGir->m_pRight ? pGir->m_pRight->GetThickFactByStation(pBxR->GetStation(), G_W) : 0;
	double gH	  = bLeft ? pGir->GetHeightGirderByStaAng(pBx->GetStation(), 0, pBx->GetAngle()) : pGir->GetHeightGirderByStaAng(pBxR->GetStation(), 0, pBxR->GetAngle());
	double dSharpLower = bLeft ? pGir->GetLengthSharpOnBx(pBx, FALSE) : pGir->GetLengthSharpOnBx(pBxR, FALSE);

	if(pC == NULL) return;

	CDPoint A[10], vX(1,0), vY(0,1), xyBaseUpper, xyBaseLower, vAng;
	pC->GetCrossXyDom(A);
	vAng = pC->GetVectorCross();

	if(bLeft) 
	{
		// UPPER LINE
		if(pC->m_dLH1 > 0)	xyBaseUpper = A[8]+vX*dTWeb;
		else				xyBaseUpper = A[0];
		DstParallelogramXyBottom(pDom, xyBaseUpper, A[2], pC->m_T2_CR);
		
		// LOWER LINE
		if(pC->m_H_CR == gH) xyBaseLower = A[8]+vX*(dTWeb + dSharpLower);
		else				 xyBaseLower = A[8]+vX*dTWeb;

		CDPoint xyBotR = A[2] - vY*(pC->m_H_CR+pC->m_T1_CR), xyBotL;
		GetXyMatchLineAndLine(xyBotR, vAng, xyBaseLower, vY, xyBotL);
		if(!pC->IsLeftHBGu())			
			DstParallelogramXyBottom(pDom, xyBotL, xyBotR, pC->m_T1_CR);		
	}
	else 
	{
		// UPPER LINE
		if(pC->m_dRH1 > 0)	xyBaseUpper = A[9]-vX*dTWebR;
		else				xyBaseUpper = A[7];
		DstParallelogramXyBottom(pDom, A[5], xyBaseUpper, pC->m_T2_CR);

		// LOWER LINE
		if(pC->m_H_CR == gH) xyBaseLower = A[9]-vX*(dTWebR + dSharpLower);
		else				 xyBaseLower = A[9]-vX*dTWebR;

		CDPoint xyBotL = A[5] - vY*(pC->m_H_CR+pC->m_T1_CR), xyBotR;
		GetXyMatchLineAndLine(xyBotL, vAng, xyBaseLower, vY, xyBotR);
		if(!pC->IsRightHBGu())
			DstParallelogramXyBottom(pDom, xyBotL, xyBotR, pC->m_T1_CR);
	}
}
void CAPlateDrawCross::DrawCRBeamFrontGassetPlate_WebLeftBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bBolt, BOOL bBoltLine, BOOL bFillCircle)
{
	if(pBx==NULL) return;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CPlateCrossBeam *pC      = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDomyun	Dom(pDom);

	CPlatePieceObject pObj(pGir,CP_BR_W);
	pObj.m_nCol	= 0;
	pC->CreatePieceObjPlate(&pObj, TRUE, TRUE, FALSE);
	pObj.DrawPieceOutline(&Dom, pOptStd, bFillCircle);

	*pDom << Dom;

}
void CAPlateDrawCross::DrawCRBeamFrontGassetPlate_WebRightBx(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bBolt, BOOL bBoltLine, BOOL bFillCircle)
{
	if(pBx==NULL) return;
	CARoadOptionStd* pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir = pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;

	CDomyun	Dom(pDom);

	CPlatePieceObject pObj(pGir,CP_BR_W);
	pObj.m_nCol	= 1;
	pC->CreatePieceObjPlate(&pObj, TRUE, TRUE, FALSE);
	pObj.DrawPieceOutline(&Dom, pOptStd, bFillCircle);

	*pDom << Dom;
}

void CAPlateDrawCross::DrawCRBeamFrontGassetSupportPlateLeftBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CARoadOptionStd* pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir = pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;
//	if(pC->m_cGasset_CR == 0)	return;
	if (pC->m_H_CR >= pGir->GetHeightGirderByStaAng(pBx->GetStation())) return;

	CDPoint vAng = CDPoint(0,1);
	//
	CDPoint Gp1 = pC->GetPointGassetSupPlateEndBx(pBx,vAng, TRUE,TRUE);
	CDPoint Gp2 = pC->GetPointGassetSupPlateEndBx(pBx,vAng,FALSE,TRUE);
	//
	CDomyun	Dom(pDom);

	CPlatePieceObject pObj(pGir,CP_BR_GA);
	pObj.m_nCol	= 0;
	pC->CreatePieceObjPlateGassetPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	DrawCRBeamFrontGassetSupportPlateSubBx(&Dom,pBx,Gp1,Gp2,TRUE);

	*pDom << Dom;

}
void CAPlateDrawCross::DrawCRBeamFrontGassetSupportPlateRightBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CARoadOptionStd* pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir = pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;
//	if(pC->m_cGasset_CR == 0)	return;
	if (pC->m_H_CR >= pGir->GetHeightGirderByStaAng(pBx->GetStation())) return;

	CDPoint vAng  = CDPoint(0,1);
	//
	CDPoint Gp1 = pC->GetPointGassetSupPlateEndBx(pBx,vAng, TRUE,FALSE);
	CDPoint Gp2 = pC->GetPointGassetSupPlateEndBx(pBx,vAng,FALSE,FALSE);
	//

	CDomyun	Dom(pDom);

	CPlatePieceObject pObj(pGir,CP_BR_GA);
	pObj.m_nCol	= 1;
	pC->CreatePieceObjPlateGassetPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	DrawCRBeamFrontGassetSupportPlateSubBx(pDom,pBx,Gp1,Gp2,FALSE);

	*pDom << Dom;
}

void CAPlateDrawCross::DrawCRBeamFrontGassetSupportPlateByBoltLeftBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CARoadOptionStd* pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir = pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;
	if(pC->m_H_CR >= pGir->GetHeightGirderByStaAng(pBx->GetStation())) return;

	CDPoint vAng = CDPoint(0,1);
	//
	CDPoint Gp1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx,vAng, TRUE,TRUE);
	CDPoint Gp2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx,vAng,FALSE,TRUE);
	//
	CDomyun	Dom(pDom);

	CPlatePieceObject pObj(pGir);
	pObj.m_nCol	= 0;
	pC->CreatePieceObjPlateGassetPlateByBolt(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	DrawCRBeamFrontGassetSupportPlateSubBx(&Dom,pBx,Gp1,Gp2,TRUE);

	*pDom << Dom;

}
void CAPlateDrawCross::DrawCRBeamFrontGassetSupportPlateByBoltRightBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CARoadOptionStd* pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir = pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return;
	if(pC->m_H_CR >= pGir->GetHeightGirderByStaAng(pBx->GetStation())) return;

	CDPoint vAng  = CDPoint(0,1);
	//
	CDPoint Gp1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx,vAng, TRUE,FALSE);
	CDPoint Gp2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx,vAng,FALSE,FALSE);
	//
	
	CDomyun	Dom(pDom);

	CPlatePieceObject pObj(pGir,CP_BR_GA);
	pObj.m_nCol	= 1;
	pC->CreatePieceObjPlateGassetPlateByBolt(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	DrawCRBeamFrontGassetSupportPlateSubBx(pDom,pBx,Gp1,Gp2,FALSE);

	*pDom << Dom;
}

void CAPlateDrawCross::DrawCRBeamFrontGassetSupportPlateSubBx(CDomyun *pDom, CPlateBasicIndex *pBx, CDPoint BP, CDPoint EP, BOOL bLeft)
{
	if(pBx==NULL) return;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen = pBridge->GetGirder(-1);
	CPlateCrossBeam *pC      = pBx->GetSection()->GetCrossBeam();

	int nCol = bLeft?0:1;
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	else if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음

	int nGen = (int)pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_GEN);
	double uB = nGen==0 ? pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UB) : 0;
	double dB = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DB);
	double Thick = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_T); 
	double dE = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DE);
	double dD = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DD);
	double uD = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UD);
	double uE = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UE);
	double sT = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_ST);
	double sR = sT<=16 ? pGirCen->m_pSangse->m_Scallop_R1 : pGirCen->m_pSangse->m_Scallop_R2;
	double bR = pC->m_T3_CR<=16 ? pGirCen->m_pSangse->m_Scallop_R1 : pGirCen->m_pSangse->m_Scallop_R2;
		                 
	if(Thick==0) return;

	CDPoint Gp1=BP, Gp2=EP;

	CDPoint vGaAng = (Gp1-Gp2).Unit();
	Gp1 -= vGaAng * uB / (vGaAng.RotateInv90()*vGaAng);
	Gp2 += vGaAng * dB / (vGaAng.RotateInv90()*vGaAng);
	//
	CDPoint xy, xy0, xy1, xy2, Kb0, Kb1, Kb2, Kb3, A[10];
	pC->GetCrossXyDom(A);
	CDPoint vCRAng = (A[3]-A[2]).Unit();
	xy.x = A[2].x;
	xy.y = A[2].y-pC->m_H_CR-pC->m_T1_CR;
	CDPoint vGaAngVert = bLeft ? vGaAng.RotateInv90() : vGaAng.Rotate90(); 
	CDPoint Gp3 = Gp1 + vGaAngVert * Thick / fabs(vGaAng*vGaAngVert);
	CDPoint Gp4 = Gp2 + vGaAngVert * Thick / fabs(vGaAng*vGaAngVert);

	CDomyun	Dom(pDom);

	pOptStd->SetEnvType(pDom, HCAD_STLC);
	if(nGen==1)
	{
		GetXyMatchLineAndLine(Gp3, vGaAng, xy, vCRAng, xy0);	//하부플랜지와 가세트 교점
		xy1 = xy0 - dE*CDPoint(0, 1);
		GetXyMatchLineAndLine(Gp3, vGaAng, xy1, CDPoint(1, 0), Kb0);	//Knee Bracing과 가세트 교점
		Gp3 = xy0;
		Kb1 = bLeft ? Kb0 + dD*CDPoint(1, 0) : Kb0 - dD*CDPoint(1, 0);
		xy2 = bLeft ? xy0 + uE*CDPoint(1, 0) : xy0 - uE*CDPoint(1, 0);
		GetXyMatchLineAndLine(xy2, CDPoint(0, 1), xy, vCRAng, Kb3);	//Knee Bracing과 하부플랜지 교점
		Kb2 = Kb3 - uD*CDPoint(0, 1);
		//Knee Bracing
		Dom.LineTo(Kb0, Kb1);
		Dom.LineTo(Kb1, Kb2);
		Dom.LineTo(Kb2, Kb3);
		
		//스켈럽
		if(bLeft)
		{
			Dom.Arc(Gp1, bR, vCRAng.GetAngleDegree() + 180, vGaAng.GetAngleDegree()+360-vCRAng.GetAngleDegree());
			Dom.Arc(xy0, sR, vCRAng.GetAngleDegree(), vGaAng.GetAngleDegree()+180-vCRAng.GetAngleDegree());
		}
		else
		{
			Dom.Arc(xy0, sR, vCRAng.GetAngleDegree() + 180, vGaAng.GetAngleDegree()+360-vCRAng.GetAngleDegree());
			Dom.Arc(Gp1, bR, vCRAng.GetAngleDegree(), vGaAng.GetAngleDegree()+180-vCRAng.GetAngleDegree());
		}
	}
	Dom.LineTo(Gp1,Gp3);
	Dom.LineTo(Gp2,Gp4);
	Dom.LineTo(Gp3,Gp4);
	*pDom << Dom;
}

void CAPlateDrawCross::DrawFrontSplicePlate_FlangeBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;

	CDPoint A[10];
	pC->GetCrossXyDom(A);

	double Sx   = bLeft ? A[2].x : A[5].x;
	double Sy   = bLeft ? A[2].y : A[5].y;	
 	double slope = (A[3]-A[2]).Unit().y;
	CDPoint vCross = (A[3]-A[2]).Unit();
	CDPoint cp, vX(1,0), vY(0,1);
	double Len = 0, Len2 = 0;

	// 상부 상면 이음판
	if(pC->m_bConnectionPlate==FALSE)				// 미적용
	{
		Len = pC->m_uA+pC->m_uB*pC->m_uC+pC->m_uC1/2; // 1/2 폭;
		cp.x = Sx - Len;
		cp.y = Sy - Len*slope + pC->m_T2_CR + pC->m_uT/2;
		DstParallelogramHorz(pDomP,Len*2,pC->m_uT,slope,cp.x,cp.y);
	}
	else 
	{
		Len = pC->m_uA+pC->m_uB1*pC->m_uC+pC->m_uC1/2; 
		Len2 = pC->m_uC1/2+pC->m_uA+pC->m_uB*pC->m_uC;
		cp.x = Sx - (bLeft?Len:Len2);
		cp.y = Sy - (bLeft?Len:Len2)*slope + pC->m_T2_CR + pC->m_uT/2;
		DstParallelogramHorz(pDomP,Len+Len2,pC->m_uT,slope,cp.x,cp.y);
	}

	// 상부 하면 이음판
	if(pC->m_bConnectionPlate==FALSE)					// 미적용
	{
		cp.y = Sy - Len*slope - pC->m_T2_CR/2;
		DstParallelogramHorz(pDomP,Len*2,pC->m_uT,slope,cp.x,cp.y);
	}
	else if(pC->m_nRadioConnectionPlateInOut==0)		// 내외측적용
	{
		Len2 = pC->m_uC1/2+pC->m_uA+pC->m_uB*pC->m_uC;
		cp.y = Sy - (bLeft?Len:Len2)*slope - pC->m_T2_CR/2;
		DstParallelogramHorz(pDomP,Len+Len2,pC->m_uT,slope,cp.x,cp.y);
	}

	Sy  -= pC->m_H_CR+pC->m_T1_CR+pC->m_T2_CR+pC->m_dT;
	CDPoint xySttLower = CDPoint(Sx, Sy);
	// 하부 상면 이음판
	if(pC->m_bConnectionPlate==FALSE)				// 미적용
	{
		Len = pC->m_dA+pC->m_dB*pC->m_dC+pC->m_dC1/2; // 1/2 폭;
		cp.x = Sx - Len;
		cp.y = Sy - Len*slope + pC->m_T2_CR + pC->m_dT/2;
		DstParallelogramHorz(pDomP,Len*2,pC->m_dT,slope,cp.x,cp.y);
	}
	else 
	{
		Len = pC->m_dA+pC->m_dB1*pC->m_dC+pC->m_dC1/2; 
		Len2 = pC->m_dC1/2+pC->m_dA+pC->m_dB*pC->m_dC;
		cp.x = Sx - (bLeft?Len:Len2);
		cp.y = Sy - (bLeft?Len:Len2)*slope + pC->m_T2_CR + pC->m_dT/2;
		DstParallelogramHorz(pDomP,Len+Len2,pC->m_dT,slope,cp.x,cp.y);
	}

	// 하부 하면 이음판
	
	if(pC->m_bConnectionPlate==FALSE)					// 미적용
	{
		cp += vY*(pC->m_T1_CR + pC->m_dT);
		DstParallelogramHorz(pDomP,Len*2,pC->m_dT,slope,cp.x,cp.y);
	}
	else if(pC->m_nRadioConnectionPlateInOut==0)		// 내외측적용
	{
		Len2 = pC->m_dC1/2+pC->m_dA+pC->m_dB*pC->m_dC;
		cp += vY*(pC->m_T1_CR + pC->m_dT);
		DstParallelogramHorz(pDomP,Len+Len2,pC->m_dT,slope,cp.x,cp.y);
	}
}

void CAPlateDrawCross::DrawFrontSplicePlate_ShearBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	if(pC == NULL)	return;

	CDomyun	Dom(pDomP);

	CPlatePieceObject pObj(pGir, CP_SP_W_SH);
	pObj.m_nCol =bLeft ? 0 : 1;
	pC->CreatePieceObjPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	*pDomP << Dom;
}

//Filler 두께가 9mm가 넘을경우 이음판 추가...CP_SP_W_FH
void CAPlateDrawCross::DrawFrontSplicePlate_FillerBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	if(pC == NULL)	return;

	CDomyun	Dom(pDomP);

	CPlatePieceObject pObj(pGir, CP_SP_W_FH);
	pObj.m_nCol =bLeft ? 0 : 1;
	pC->CreatePieceObjPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	*pDomP << Dom;
}

void CAPlateDrawCross::DrawFrontSplicePlate_MomentBx(CDomyun *pDomP, CPlateBasicIndex *pBx, BOOL bLeft, BOOL bUpper, BOOL bDrawBolt)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	if(pC == NULL)	return;

	double dDistWeb = pGir->GetDisFrontPlateWebToWeb(pBx);
	if((dDistWeb-pC->m_P_CR*2)<1.00 && !bLeft)  return;

	CDomyun	Dom(pDomP);

	CPlatePieceObject pObj(pGir, CP_SP_W_MO);
	pObj.m_nCol = bLeft  ? 0 : 1;
	pObj.m_nRow = bUpper ? 0 : 1;
	pC->CreatePieceObjPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	*pDomP << Dom;
}

// 평행사변형 : 하단의 두 좌표, 높이
void CAPlateDrawCross::DstParallelogramXyBottom(CDomyun *pDomP, CDPoint xyStt, CDPoint xyEnd, double dHeight)
{
	if(xyStt == xyEnd || dHeight < 0) return;
	pDomP->LineTo(xyStt, xyEnd);
	pDomP->LineTo(xyStt, xyStt+CDPoint(0, 1)*dHeight);
	pDomP->LineTo(xyEnd, xyEnd+CDPoint(0, 1)*dHeight);
	pDomP->LineTo(xyStt+CDPoint(0, 1)*dHeight, xyEnd+CDPoint(0, 1)*dHeight);
}

// 평행사변형 : 폭,길이 - 직사각형의 값
// 중심 : 좌측변의 중앙
void CAPlateDrawCross::DstParallelogramHorz(CDomyun *pDomP, double Length, double Width, 
										   double slop ,double BX, double BY)
{
	if(Length<=0)	return;
	pDomP->LineTo(BX,BY+Width/2, BX+Length, BY+Width/2+Length*slop);
	pDomP->LineTo(BX,BY-Width/2, BX+Length, BY-Width/2+Length*slop);
	pDomP->LineTo(BX,BY+Width/2, 
		          BX,BY-Width/2);
	pDomP->LineTo(BX+Length, BY+Width/2+Length*slop,
		          BX+Length, BY-Width/2+Length*slop);
}

// 평행사변형 : 두께,길이 - 직사각형의 값
// 중심 : 상단의 중앙
void CAPlateDrawCross::DstParallelogramVert(CDomyun *pDomP, double Height, double Thick
										   , double slop ,double BX, double BY)
{
	if(Height<=0 || Thick<=0) return;

	pDomP->LineTo(BX-Thick/2, BY-Thick/2*slop, 
		          BX-Thick/2, BY-Thick/2*slop-Height);
	pDomP->LineTo(BX+Thick/2, BY+Thick/2*slop, 
		          BX+Thick/2, BY+Thick/2*slop-Height);
	pDomP->LineTo(BX-Thick/2, BY-Thick/2*slop, 
		          BX+Thick/2, BY+Thick/2*slop);
	pDomP->LineTo(BX-Thick/2, BY-Thick/2*slop-Height, 
		          BX+Thick/2, BY+Thick/2*slop-Height);
} 
void CAPlateDrawCross::DrawCRBeamVStiffnerOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CDomyun Dom(pDom);
	DrawFrontVStiffnerBx(&Dom, pBx);
	*pDom << Dom;
}

void CAPlateDrawCross::DrawFrontVStiffnerBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	if(pC == NULL) return;
	if(pC->m_nVStiffsu_CR < 0)	return;

	CDPoint A[10];
	pC->GetCrossXyDom(A);

	double VLen    = pC->m_vL_CR;
	if(pC->m_vL_CR<=0 || pC->m_vT_CR<=0 || pC->m_vW_CR<=0)	return;

	double webT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	CDPoint vAng = CDPoint(0,1);
	CDPoint vCross = (A[2]-A[0]).Unit();
	CDPoint vBrk   = (A[3]-A[1]).Unit();

	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	double giulGasset = (A[3]-A[2]).Unit().y;
	double giulCross = (A[4]-A[3]).Unit().y;
	double slopeTu = giulCross*pC->m_vT_CR/2;
	double slopeTl = slopeTu;
	double SpLen   =  A[3].x-A[2].x;
	CDPoint xyStt;
	if(!pC->m_nLGirSpType)
		xyStt = A[8]+CDPoint(1,0)*webT+vCross*pC->GetLengthGirWebToCrWeb(TRUE)/vCross.x;
	else
		xyStt = A[2]+vBrk*dWSp/2;
	double sX = xyStt.x;
	double sY = xyStt.y;

	CDomyun	Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	if(pC->m_cVSide_CR == 2)	//후면
		pOptStd->SetEnvType(&Dom, HCAD_STLH);

	for(long i = 0 ; i < pC->m_nVStiffsu_CR ; i++)
	{
		sX   += pC->m_VStiffDis_CR[i]/vAng.y;
		sY    = A[2].y + SpLen*giulGasset + (sX - A[3].x)*giulCross;
		VLen = pC->m_vL_CR;
		slopeTu = slopeTl;
		//	draw
		if(pC->m_H_CR==pC->m_vL_CR)
		{	// 두줄
			Dom.LineTo(sX-pC->m_vT_CR/2, sY-pC->m_vL_CR-slopeTu+VLen, sX-pC->m_vT_CR/2, sY-pC->m_vL_CR-slopeTl);
			Dom.LineTo(sX+pC->m_vT_CR/2, sY-pC->m_vL_CR+slopeTu+VLen, sX+pC->m_vT_CR/2, sY-pC->m_vL_CR+slopeTl);
		}
		else
		{	// 세줄
			Dom.LineTo(sX-pC->m_vT_CR/2, sY-pC->m_vL_CR-slopeTu+VLen, sX-pC->m_vT_CR/2, sY-pC->m_vL_CR);
			Dom.LineTo(sX+pC->m_vT_CR/2, sY-pC->m_vL_CR);
			Dom.LineTo(sX+pC->m_vT_CR/2, sY-pC->m_vL_CR+slopeTu+VLen);
		}
	}

	*pDom << Dom;
}

void CAPlateDrawCross::DrawFrontVStiffnerBxPrv(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	if(pC == NULL) return;
	if(pC->m_nVStiffsu_CR < 0 || (int)pC->m_cVSide_CR == 2)	return;

	CDPoint A[10];
	pC->GetCrossXyDom(A);

	double VLen  = pC->m_vL_CR;
	CDPoint vAng = CDPoint(0,1);
	double giulGasset = (A[3]-A[2]).Unit().y;
	double giulCross = (A[4]-A[3]).Unit().y;
	double slopeTu = giulCross*pC->m_vT_CR/2;
	double slopeTl = slopeTu;
	double SpLen   =  A[3].x-A[2].x;
	double sX	   =  A[2].x;
	double sY	   =  A[2].y;

/*	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxPrv = Finder.GetBxLast(BX_CROSSBEAM_VBRACING, pBx,NULL,FALSE);
	while(pBxPrv)
	{
		if(pBxPrv->IsCrossBeamRight())
			break;
		pBxPrv = Finder.GetBxNext();
	}
*/
	CDomyun	Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	for(long i = 0 ; i < pC->m_nVStiffsu_CR ; i++)
	{
		sX   += pC->m_VStiffDis_CR[i]/vAng.y;
		sY    = A[2].y + SpLen*giulGasset + (sX - A[3].x)*giulCross;
		VLen = pC->m_vL_CR;
		slopeTu = slopeTl;
		//	draw
		if(pC->m_H_CR==pC->m_vL_CR) {	// 두줄
			Dom.LineTo(sX-pC->m_vT_CR/2, sY-pC->m_vL_CR-slopeTu+VLen, 
				         sX-pC->m_vT_CR/2, sY-pC->m_vL_CR-slopeTl);
			Dom.LineTo(sX+pC->m_vT_CR/2, sY-pC->m_vL_CR+slopeTu+VLen,
				         sX+pC->m_vT_CR/2, sY-pC->m_vL_CR+slopeTl);
		}
		else {	// 세줄
			Dom.LineTo(sX-pC->m_vT_CR/2, sY-pC->m_vL_CR-slopeTu+VLen, 
				         sX-pC->m_vT_CR/2, sY-pC->m_vL_CR);
			Dom.LineTo(sX+pC->m_vT_CR/2, sY-pC->m_vL_CR);
			Dom.LineTo(sX+pC->m_vT_CR/2, sY-pC->m_vL_CR+slopeTu+VLen);
		}
	}

	*pDom << Dom;
}


void CAPlateDrawCross::DrawCRBeamAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateBridgeApp	 *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	 *pGirCen = pBridge->GetGirder(-1);
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	if(pC == NULL) return;

	if(pC->m_nAnchorsu_CR <= 0)	return;
	
	CDPoint A[10];
	pC->GetCrossXyDom(A);
	double webT       = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWSp       = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	double dUpperSp   = (pC->m_uA+pC->m_uB*pC->m_uC)*2+pC->m_uC1;
	double giulCross  = pGir->GetGiulCrossBeamCenter(pBx);		// 가로보 MAIN위치의 경사
	BOOL bLevel = pOpt->GetDesignCrossBeamBracketType();
//	if(bLevel && (!pC->m_nLGirSpType && !pC->m_nRGirSpType))
//		giulCross = (A[7]-A[0]).Unit().y/(A[7]-A[0]).Unit().x;

	double dWVStiff   = pC->m_pVStiff->m_dWidth[1];
	
	CDPoint vCross = (A[2]-A[0]).Unit();
	if(bLevel)// && !(!pC->m_nLGirSpType && !pC->m_nRGirSpType))
		vCross = CDPoint(1, 0);
	CDPoint vBrk   = (A[3]-A[1]).Unit();
	CDPoint xyStt, vX(1,0);

	if(!pC->m_nLGirSpType)
	{
		if(pC->m_dLH1>0)
			xyStt = A[8] + vX*webT + vCross*pC->GetLengthGirWebToCrWeb(TRUE)/vCross.x;
		else
		{
			xyStt    = A[8] + vX*(webT+pC->GetLengthGirWebToCrWeb(TRUE));
			xyStt.y -= (dWVStiff+pC->m_dbL4-pC->GetLengthGirWebToCrWeb(TRUE))*vCross.y;
		}
	}
	else
		xyStt = A[2]+vBrk*dWSp/2;
	
	double sX = xyStt.x;
	double sY = xyStt.y;
	double dZ = ((dUpperSp-dWSp)/2+pC->m_X_CR) * giulCross;//전단이음판 우측끝단과 레벨이 꺽이는 곳의 거리차의 Z보정량
	if(bLevel)/* && !(!pC->m_nLGirSpType && !pC->m_nRGirSpType))*/
		sY -= dZ;
	for(long i = 0 ; i < pC->m_nAnchorsu_CR ; i++)
	{
		sX   += pC->m_AnchorDis_CR[i];
		sY   += giulCross*pC->m_AnchorDis_CR[i];
		pDom->AnchorBarFront(sX, sY+pC->m_T2_CR, pGirCen->m_pSangse->m_SlabAnchor_T,atan(giulCross));
	}
}

//슬래브 앵커 정면도
void CAPlateDrawCross::DrawVBracingAnchorBar(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateBridgeApp	*pBridge  = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen  = pBridge->GetGirder(-1);
	CVBracing       *pVr      = pBx->GetSection()->GetVBracing();
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	if(pVr == NULL) return;

	if(pVr->m_nAnchorSu <= 0)	return;
	
	CDomyun Dom(pDom);

	CDPoint vUpper	= pVr->GetVectorFrame(CVBracing::UPPER);
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dHBeam	= pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
	double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
	double dPlan	= fabs(pBx->GetAngleSectionCrossVBracing().RotateInv(vGir).y);

	if(!pOpt->GetInstallBeamAtCenter())
		dHBeamCen = dHBeam/2;

	CDPoint vX(1,0), vY(0,1), xyStt;
	CDPoint xyCenLU = pVr->GetPointFrame(CVBracing::LT);
	xyStt = xyCenLU + vUpper*pVr->m_dG[0] + vUpper.Rotate90()*dHBeamCen;	
	for(long i = 0 ; i < pVr->m_nAnchorSu ; i++)
	{
		xyStt += vUpper*pVr->m_AnchorDis[i]*dPlan;
		Dom.AnchorBarFront(xyStt.x, xyStt.y, pGirCen->m_pSangse->m_SlabAnchor_T, vUpper.y);
	}

	*pDom << Dom;
}


void CAPlateDrawCross::DimVBracingFrontAllGirder(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL)	return;

	CPlateBridgeApp	*pBridge		= m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir			= pBridge->GetGirder(0);
	CPlateGirderApp	*pGirR			= pBridge->GetGirder(1);
	CPlateBasicIndex* pBxR			= NULL;
	CVBracing* pVr    = pBx->GetSection()->GetVBracing();
	CVBracing* pVrCur = NULL;
	
	long nGirsu = pBridge->GetGirdersu();
	CDPoint vX(1,0), vY(0,1);
	double dX[20];
	CDPoint vCrossAng = pBx->GetAngleSection();
	BOOL bSkew = EQUAL(vCrossAng.y,1, 0.001) ? FALSE : TRUE;

	CDomyun Dom(pDom);
	// 상면 치수선 : 거더 배치
	Dom.SetDirection("TOP");
	long nG = 0;
	// BOX POINT (좌우측 WEB)
	for(nG=0; nG<nGirsu; nG++)
	{
		pGir = pBridge->GetGirder(nG);
		dX[nG] = pGir->m_dGirderCenter;
	}
	double dSumGir = 0;
	for(nG=0; nG<nGirsu-1; nG++)
		dSumGir += dX[nG+1]-dX[nG];
	//
	long DimDan = 0;
	double dWebT=0, dWebTR=0, temp=0;

	double GTerm=0, disSum=0, dStt=0, dEnd=0;
	CPlateBasicIndex* pBxCur = pBx;
	
	for(nG=0; nG<nGirsu-1; nG++)
	{
		disSum = 0;
		pGir   = pBridge->GetGirder(nG);
		pGirR  = pBridge->GetGirder(nG+1);
		pBxCur = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBx, pGir);
		pBxR   = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
		if(pBxCur==NULL || pBxR==NULL)	return;
		
		pVrCur = pBxCur->GetSection()->GetVBracing();

		if(pVrCur==NULL)
		{
			pBx = pBxR;
			continue;
		}
		
		dWebT  = pGir->GetThickFactByStation(pBxCur->GetStation(), 2);
		dWebTR = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
		GTerm  = pGir->GetDisFrontPlateWebToWeb(pBxCur)+(dWebT+dWebTR)/2;
		dStt   = pBxCur->GetSection()->GetVStiff()->m_dWidth[1] + pVrCur->m_duL4 - pVrCur->m_dULG + dWebT/2;
		dEnd   = pBxR->GetSection()->GetVStiff()->m_dWidth[0] + pVrCur->m_duR4 - pVrCur->m_dURG + dWebTR/2;
	
		Dom.DimMoveTo(pGir->m_dGirderCenter, 0, DimDan);
		Dom.DimLineTo(dStt, 0, COMMA(dStt));
		for(long i = 0 ; i < pVr->m_nAnchorSu; i++)
		{
			disSum += pVrCur->m_AnchorDis[i];
			temp = pVrCur->m_AnchorDis[i];
			Dom.DimLineTo(temp, 0, bSkew ? COMMA(temp) + "(" + COMMA(temp/vCrossAng.y) + ")" : COMMA(temp));
		}
		temp = GTerm-disSum-(dStt+dEnd);
		Dom.DimLineTo(temp, 0, bSkew ? COMMA(temp) + "(" + COMMA(temp/vCrossAng.y) + ")" : COMMA(temp));
		Dom.DimLineTo(dEnd, 0, COMMA(dEnd));

		pBx = pBxR;
	}

	//2단
	DimDan = 1;
	Dom.DimMoveTo(dX[0],0,DimDan);
	temp = dSumGir;
	Dom.DimLineTo(temp, 1, bSkew ? COMMA(temp) + "(" + COMMA(temp/vCrossAng.y) + ")" : COMMA(temp));
	
	//상세표시원/////////////////////////////////////////////////////////////
	double dRadius = 500;
	CPlateGirderApp* pGirStt = pBridge->GetGirder(0);
	CPlateGirderApp* pGirBefoEnd = pBridge->GetGirder(nGirsu-2);
	CPlateGirderApp* pGirEnd = pBridge->GetGirder(nGirsu-1);
	CPlateBasicIndex* pBxStt = pBx;	
	CPlateBasicIndex* pBxEnd = pBx->GetBxMatchByCrossBeam(pGirBefoEnd);	
	CVBracing*        pVrStt = pBxStt->GetSection()->GetVBracing();
	CVBracing*        pVrEnd = pBxEnd->GetSection()->GetVBracing();
	double dHeightGirStt = pGirStt->GetHeightGirderByStaAng(pBxStt->GetStation());
	double dHeightGirEnd = pGirEnd->GetHeightGirderByStaAng(pBxEnd->GetStation());
	double dWidthGirStt = pGirStt->GetWidthOnStation(pBxStt->GetStation(), TRUE);
	double dWidthGirEnd = pGirEnd->GetWidthOnStation(pBxEnd->GetStation(), TRUE);
	CDPoint xyCen1, xyCen2, xyCen3, xyCen4, xyGisiArrow, xyPoint;
	CDPoint Astt[10], Aend[10];
	pGirStt->GetHunchXyDom(pBxStt, Astt);
	pGirBefoEnd->GetHunchXyDom(pBxEnd, Aend);

	if(pVr)
	{
		xyCen1 = pVr->GetPointFrame(CVBracing::LT);
		xyCen2 = pVr->GetPointFrame(CVBracing::RT);
		xyCen3 = pVr->GetPointFrame(CVBracing::LD);
		xyCen4 = pVr->GetPointFrame(CVBracing::RD);

		xyGisiArrow = xyCen1+CDPoint(-1, 1).Unit()*dRadius;
		Dom.CircleArrow(xyGisiArrow.x, xyGisiArrow.y, dRadius/2, -dRadius/2, 3, "상세 1", "");
		Dom.Circle(xyCen1, dRadius);//상세1
		
		xyGisiArrow = xyCen3+CDPoint(-1, -1).Unit()*dRadius;
		Dom.CircleArrow(xyGisiArrow.x, xyGisiArrow.y, dRadius/2, -dRadius/2, 5, "상세 2", "");
		Dom.Circle(xyCen3, dRadius);//상세2

		if(pVr->GetType()==TYPE_VBRACING)
		{
			xyGisiArrow = (xyCen3+xyCen4)/2+CDPoint(1, -1).Unit()*dRadius;
			Dom.CircleArrow(xyGisiArrow.x, xyGisiArrow.y, dRadius/2, dRadius/2, 7, "상세 5", "");
			Dom.Circle((xyCen3+xyCen4)/2, dRadius);//상세5
		}
		else
		{
			xyGisiArrow = (xyCen1+xyCen2)/2+CDPoint(1, 1).Unit()*dRadius;
			Dom.CircleArrow(xyGisiArrow.x, xyGisiArrow.y, dRadius/2, dRadius/2, 1, "상세 5", "");
			Dom.Circle((xyCen1+xyCen2)/2, dRadius);//상세5
		}
	}	

	pVr = pBx->GetBxMatchByCrossBeam(pGirBefoEnd)->GetSection()->GetVBracing();
	if(pVr)
	{
		xyCen2 = pVr->GetPointFrame(CVBracing::RT);
		xyCen4 = pVr->GetPointFrame(CVBracing::RD);

		xyGisiArrow = xyCen2+CDPoint(1, 1).Unit()*dRadius;
		Dom.CircleArrow(xyGisiArrow.x, xyGisiArrow.y, dRadius/2, dRadius/2, 1, "상세 3", "");
		Dom.Circle(xyCen2, dRadius);//상세3
		
		xyGisiArrow = xyCen4+CDPoint(1, -1).Unit()*dRadius;
		Dom.CircleArrow(xyGisiArrow.x, xyGisiArrow.y, dRadius/2, dRadius/2, 7, "상세 4", "");
		Dom.Circle (xyCen4, dRadius);//상세4
	}
	
	//좌측치수선
	if(pVrStt)
	{
		DimDan=0;
		Dom.SetDirection("LEFT");
		xyPoint = Astt[1] - vY*dHeightGirStt;
		Dom.DimMoveTo(xyPoint.x, xyPoint.y, DimDan);
		Dom.DimLineTo(pVrStt->m_dLH2, DimDan, COMMA(pVrStt->m_dLH2));
		Dom.DimLineTo(dHeightGirStt-(pVrStt->m_dLH1+pVrStt->m_dLH2), DimDan, COMMA(dHeightGirStt-(pVrStt->m_dLH1+pVrStt->m_dLH2)));
		Dom.DimLineTo(pVrStt->m_dLH1, DimDan, COMMA(pVrStt->m_dLH1));
		DimDan=1;
		Dom.DimMoveTo(xyPoint.x, xyPoint.y, DimDan);
		Dom.DimLineTo(dHeightGirStt, DimDan, COMMA(dHeightGirStt));
	}
	
	//우측치수선
	if(pVrEnd)
	{
		DimDan=0;
		Dom.SetDirection("RIGHT");
		xyPoint = Aend[7]+vX*dWidthGirEnd - vY*dHeightGirEnd;
		Dom.DimMoveTo(xyPoint.x, xyPoint.y, DimDan);
		Dom.DimLineTo(pVrEnd->m_dRH2, DimDan, COMMA(pVrEnd->m_dRH2));
		Dom.DimLineTo(dHeightGirEnd-(pVrEnd->m_dRH1+pVrEnd->m_dRH2), DimDan, COMMA(dHeightGirEnd-(pVrEnd->m_dRH1+pVrEnd->m_dRH2)));
		Dom.DimLineTo(pVrEnd->m_dRH1, DimDan, COMMA(pVrEnd->m_dRH1));
		DimDan=1;
		Dom.DimMoveTo(xyPoint.x, xyPoint.y, DimDan);
		Dom.DimLineTo(dHeightGirEnd, DimDan, COMMA(dHeightGirEnd));
	}	
	*pDom << Dom;
	
	//하단 치수선(치수선 방향이 서로 반대)
	DimDan=0;

	double dDist, dDistHeight;
	for(long i = 0; i < nGirsu-1; i++)
	{
		Dom.SetDirection("RIGHT");

		pGirStt = pBridge->GetGirder(i);
		pGirEnd = pBridge->GetGirder(i+1);

		CPlateBasicIndex *pBxStt = pBx->GetBxMatchByCrossBeam(pGirStt);
		CPlateBasicIndex *pBxEnd = pBx->GetBxMatchByCrossBeam(pGirEnd);

		dHeightGirStt = pGirStt->GetHeightGirderByStaAng(pBxStt->GetStation());
		dHeightGirEnd = pGirEnd->GetHeightGirderByStaAng(pBxEnd->GetStation());

		pGirStt->GetHunchXyDom(pBxStt, Astt);
		pGirEnd->GetHunchXyDom(pBxEnd, Aend);

		CDPoint xyPoint1 = Astt[2] - vY*dHeightGirStt;
		CDPoint xyPoint2 = Aend[1] - vY*dHeightGirEnd;

		dDist = xyPoint2.x - xyPoint1.x;
		dDistHeight = xyPoint1.y - xyPoint2.y;

		Dom.SetStartPointExtend(TRUE);
		Dom.SetDimVLen(20);
		Dom.SetDimExo(0);
		Dom.DimMoveTo(xyPoint1.x, xyPoint1.y, DimDan, COMMA(dDistHeight));
		Dom.Move(dDist/2-Dom.Always(Dom.GetDimVLen()), 0);
		*pDom << Dom;
		Dom.SetDirection("LEFT");
		Dom.DimLineTo(-dDistHeight, DimDan, "");
		Dom.Move(dDist/2+Dom.Always(Dom.GetDimVLen()), 0);
		*pDom << Dom;
	}
	//상세 단면 표시
	//단면 마크 
	pGirStt = pBridge->GetGirder(0);
	pBx = pBx->GetBxMatchByCrossBeam(pGirStt);
	if(pBx && pBx->GetSection())
	{
		pGirStt->GetHunchXyDom(pBx, Astt);
		CAPlateSymbolDom DomSym(pDom, m_pDataMng);
		DomSym.SetScaleDim(pDom->GetScaleDim());
		CDPoint xySymbol = Astt[7]+vX*dWidthGirStt/2 - vY*dHeightGirStt*0.3;
		DomSym.DrawSymbolSectionGisiArrow(xySymbol, dWidthGirStt, "A", "A", 180);
		long a = pBx->GetNumberGirder();
		double dHBeam = pBx->GetSection()->GetBeamBaseHeight(1, TRUE, TRUE); 
		double dLBeam = pBx->GetSection()->GetBeamJewon(1, CSection::LENGTH); 
		CDPoint vDirBeam = pBx->GetSection()->GetVBracing()->GetVectorFrame(1);
		xySymbol = xyCen3+vDirBeam*dLBeam*0.1;
		DomSym.DrawSymbolSectionGisiArrow(xySymbol, dHBeam*1.2, "B", "B", -90);
		*pDom << DomSym;
	}	
}

void CAPlateDrawCross::DimCRBeamFrontAllGirder(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL)	return;

	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	 *pBridge = m_pDataMng->GetBridge();

	CDomyun Dom(pDom);
	CAPlateSymbolDom sDom(&Dom, m_pDataMng);
	sDom.SetScaleDim(Dom.GetScaleDim());

	long	nGirsu = pBridge->GetGirdersu();
	double	dSumGir = 0;
	double	dX[20];
	CDPoint B[10];
	CDPoint	vX(1,0), vY(0,1);
	long	nG = 0;
	for (long ix =0; ix < 20; ix++)
	{
		dX[nG] = 0;
	}


	for(nG=0; nG<nGirsu; nG++)
	{
		CPlateGirderApp	 *pGir1	 = pBridge->GetGirder(nG);
		dX[nG] = pGir1->m_dGirderCenter;
	}

	pBridge->GetGirder(0)->GetHunchXyDom(pBx, B);

	double dTotL = 0;
	CLongArray nArrBeam;
	pBridge->GetIndexGirderToCrossBeam(nArrBeam, pBx, TRUE);
	if(nArrBeam.GetSize() == 0 ) return;

	for (long ix =0; ix < nArrBeam.GetAt(0) ; ix++)
	{
		dTotL += fabs(dX[ix]);
	}



	//상단 치수선
	BOOL bWeldMarkVStiff = FALSE;
	BOOL bWeldMarkAnchor = FALSE;
	BOOL bWeldMarkFlange = FALSE;
	long nDan = 0;

	CPlateBasicIndex* pBxGir = pBx;
	for(nG=0; nG<nGirsu; nG++)
	{
		BOOL bMat = FALSE;
		for(long ix =0; ix < nArrBeam.GetSize(); ix++)
		{
			if(nG == nArrBeam.GetAt(ix))
			{
				bMat = TRUE;
				break;
			}
		}
		if(bMat == FALSE)
			continue;


		nDan = 0;
		CPlateGirderApp	 *pGir	 = pBridge->GetGirder(nG);
		CPlateCrossBeam	 *pC	 = pBx->GetBxMatchByCrossBeam(pGir)->GetSection()->GetCrossBeam();

		CPlateGirderApp* pGirR	= pBridge->GetGirder(nG+1);
		if(pBxGir == NULL)
		{
			pBxGir = pBx;
			continue;
		}

		CPlateBasicIndex *pBxGirR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBxGir, pGirR);		
		if(pBxGirR == NULL)
		{
			pBxGir = pBxGirR;
			continue;
		}
		pBxGir = pBxGirR;


		if(!pC) continue;

		double dLGirToWebLeft  = pC->GetLengthGirWebToCrWeb(TRUE);
		double dLGirToWebRight = pC->GetLengthGirWebToCrWeb(FALSE);
		CPlateSangSeDo *pSang  = pGir->m_pSangse;
		if(pC == NULL)	return;

		double webT = pGir->GetThickFactByStation(pBx->GetBxMatchByCrossBeam(pGir)->GetStation(), 2);
		CDPoint A[10];
		pC->GetCrossXyDom(A);

		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		//  수직보강재 텍스트, 슬래브 앵커 치수
		long nV = pC->m_nVStiffsu_CR;
		Dom.SetDirection("TOP");
		double disSum = 0;
		double GTerm  = pGir->GetDisFrontPlateWebToWeb(pBx);
		double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
		double dGiulCross = (A[4]-A[3]).Unit().y;
		
		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			//1단
			Dom.DimMoveTo(A[8].x+webT/2,  max(B[2].y, B[7].y), nDan++,"");
			Dom.DimLineTo(dLGirToWebLeft+webT/2, 0, COMMA(dLGirToWebLeft+webT/2));
			Dom.DimLineTo(GTerm-(dLGirToWebLeft+dLGirToWebRight),0,COMMA(GTerm-(dLGirToWebLeft+dLGirToWebRight)));
			Dom.DimLineTo(dLGirToWebRight+webT/2, 0, COMMA(dLGirToWebRight+webT/2));
			//2단
			Dom.DimMoveTo(A[8].x+webT/2,  max(B[2].y, B[7].y), nDan++,"");
			for(long n=0; n<nV; n++)
			{
				if(n==0)
				{
					Dom.DimLineTo(dLGirToWebLeft+webT/2+pC->m_VStiffDis_CR[n], 1, COMMA(dLGirToWebLeft+webT/2+pC->m_VStiffDis_CR[n]));
					//수직보강재 용접
					double dVSWeldPosX = webT/2+dLGirToWebLeft+pC->m_VStiffDis_CR[n]+pC->m_vT_CR/2;
					double dVSWeldPosY = A[8].y+dVSWeldPosX*dGiulCross-pC->m_H_CR*3/4;
					if(bWeldMarkVStiff==FALSE)
					{
						Dom.Weld(A[8].x+webT/2+dVSWeldPosX, dVSWeldPosY, 1, 1, 0, FALSE, TRUE, 6, 0);
						bWeldMarkVStiff = TRUE;
					}
				}
				else
					Dom.DimLineTo(pC->m_VStiffDis_CR[n],1,COMMA(pC->m_VStiffDis_CR[n]));
				disSum += pC->m_VStiffDis_CR[n];
			}
			if(disSum==0)
				Dom.DimLineTo(GTerm+webT,1,COMMA(GTerm+webT));
			else
				Dom.DimLineTo(GTerm-disSum-dLGirToWebLeft+webT/2,1,COMMA(GTerm-disSum-dLGirToWebLeft+webT/2));
			//앵커 용접
			if(pC->m_nAnchorsu_CR>0)
			{
				double dAncWeldPosX = dLGirToWebLeft+pC->m_AnchorDis_CR[0]+webT/2;
				double dAncWeldPosY = A[8].y+dAncWeldPosX*dGiulCross+pC->m_T2_CR;
				if(bWeldMarkAnchor==FALSE)
				{
					sDom.DrawWeldMark(vX*(A[8].x+dAncWeldPosX)+vY*dAncWeldPosY, GI_SA, GI_SA, pBx, 1, 7, 0, FALSE, FALSE, 0);
					bWeldMarkAnchor = TRUE;
				}
				Dom << sDom;
			}
		}
		else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			//1단
			Dom.DimMoveTo(A[8].x+webT/2,  max(B[2].y, B[7].y), nDan++,"");
			Dom.DimLineTo(pC->m_P_CR+webT/2, 0, COMMA(pC->m_P_CR+webT/2));
			Dom.DimLineTo(GTerm-pC->m_P_CR-dLGirToWebRight,0,COMMA(GTerm-pC->m_P_CR-dLGirToWebRight));
			Dom.DimLineTo(dLGirToWebRight+webT/2, 0, COMMA(dLGirToWebRight+webT/2));
			//2단
			Dom.DimMoveTo(A[8].x+webT/2,  max(B[2].y, B[7].y), nDan++,"");
			for(long n=0; n<nV; n++)
			{
				if(n==0)
				{
					Dom.DimLineTo(webT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n], 1, COMMA(webT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n]));
					//수직보강재 용접
					double dVSWeldPosX = webT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n]+pC->m_vT_CR/2;
					double dVSWeldPosY = A[8].y+dVSWeldPosX*dGiulCross-pC->m_H_CR*3/4;
					if(bWeldMarkVStiff==FALSE)
					{
						Dom.Weld(A[8].x+webT/2+dVSWeldPosX, dVSWeldPosY, 1, 1, 0, FALSE, TRUE, 6, 0);
						bWeldMarkVStiff = TRUE;
					}
				}
				else
					Dom.DimLineTo(pC->m_VStiffDis_CR[n],1,COMMA(pC->m_VStiffDis_CR[n]));
				disSum += pC->m_VStiffDis_CR[n];
			}
			if(disSum==0)
				Dom.DimLineTo(GTerm+webT,1,COMMA(GTerm+webT));
			else
				Dom.DimLineTo(GTerm-disSum-pC->m_P_CR-dWSp/2+webT/2,1,COMMA(GTerm-disSum-pC->m_P_CR-dWSp/2+webT/2));
			//앵커 용접
			if(pC->m_nAnchorsu_CR>0)
			{
				double dAncWeldPosX = webT/2+pC->m_P_CR+dWSp/2+pC->m_AnchorDis_CR[0]+pSang->m_SlabAnchor_B/2;
				double dAncWeldPosY = A[8].y+dAncWeldPosX*dGiulCross+pC->m_T2_CR;
				if(bWeldMarkAnchor==FALSE)
				{
					sDom.DrawWeldMark(vX*(A[8].x+dAncWeldPosX)+vY*(dAncWeldPosY), GI_SA, GI_SA, pBx, 1, 7, 0, FALSE, FALSE, 0);
					bWeldMarkAnchor = TRUE;
				}
				Dom << sDom;
			}
		}
		else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
		{
			//1단
			Dom.DimMoveTo(A[8].x+webT/2,  max(B[2].y, B[7].y), nDan++,"");
			Dom.DimLineTo(dLGirToWebLeft+webT/2, 0, COMMA(dLGirToWebLeft+webT/2));
			Dom.DimLineTo(GTerm-pC->m_P_CR-dLGirToWebLeft,0,COMMA(GTerm-pC->m_P_CR-dLGirToWebLeft));
			Dom.DimLineTo(pC->m_P_CR+webT/2,0, COMMA(pC->m_P_CR+webT/2));
			//2단
			Dom.DimMoveTo(A[8].x+webT/2,  max(B[2].y, B[7].y), nDan++,"");
			for(long n=0; n<nV; n++)
			{
				if(n==0)
				{
					Dom.DimLineTo(dLGirToWebLeft+webT/2+pC->m_VStiffDis_CR[n], 1, COMMA(dLGirToWebLeft+webT/2+pC->m_VStiffDis_CR[n]));
					//수직보강재 용접
					double dVSWeldPosX = webT/2+dLGirToWebLeft+pC->m_VStiffDis_CR[n]+pC->m_vT_CR/2;
					double dVSWeldPosY = A[8].y+dVSWeldPosX*dGiulCross-pC->m_H_CR*3/4;
					if(bWeldMarkVStiff==FALSE)
					{
						Dom.Weld(A[8].x+webT/2+dVSWeldPosX, dVSWeldPosY, 1, 1, 0, FALSE, TRUE, 6, 0);
						bWeldMarkVStiff = TRUE;
					}
				}
				else
					Dom.DimLineTo(pC->m_VStiffDis_CR[n],1,COMMA(pC->m_VStiffDis_CR[n]));
				disSum += pC->m_VStiffDis_CR[n];
			}
			if(disSum==0)
				Dom.DimLineTo(GTerm+webT,1,COMMA(GTerm+webT));
			else
				Dom.DimLineTo(GTerm-disSum-dLGirToWebLeft+webT/2,1,COMMA(GTerm-disSum-dLGirToWebLeft+webT/2));
			//앵커 용접
			if(pC->m_nAnchorsu_CR>0)
			{
				double dAncWeldPosX = webT/2+dLGirToWebLeft+pC->m_AnchorDis_CR[0]+pSang->m_SlabAnchor_B/2;
				double dAncWeldPosY = A[8].y+dAncWeldPosX*dGiulCross+pC->m_T2_CR;
				if(bWeldMarkAnchor==FALSE)
				{
					sDom.DrawWeldMark(vX*(A[8].x+dAncWeldPosX)+vY*dAncWeldPosY, GI_SA, GI_SA, pBx, 1, 7, 0, FALSE, FALSE, 0);
					bWeldMarkAnchor = TRUE;
				}
				Dom << sDom;
			}
		}
		else
		{
			//1단
			Dom.DimMoveTo(A[8].x+webT/2 ,  max(B[2].y, B[7].y), nDan++,"");
			Dom.DimLineTo(pC->m_P_CR+webT/2, 0, COMMA(pC->m_P_CR+webT/2));
			Dom.DimLineTo(GTerm-pC->m_P_CR*2,0,COMMA(GTerm-pC->m_P_CR*2));
			Dom.DimLineTo(pC->m_P_CR+webT/2,0, COMMA(pC->m_P_CR+webT/2));
			//2단
			Dom.DimMoveTo(A[8].x+webT/2 ,  max(B[2].y, B[7].y), nDan++,"");
			for(long n=0; n<nV; n++)
			{
				if(n==0)
				{
					Dom.DimLineTo(webT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n], 1, COMMA(webT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n]));
					//수직보강재 용접
					double dVSWeldPosX = webT/2+pC->m_P_CR+dWSp/2+pC->m_VStiffDis_CR[n]+pC->m_vT_CR/2;
					double dVSWeldPosY = A[8].y+dVSWeldPosX*dGiulCross-pC->m_H_CR*3/4;
					if(bWeldMarkVStiff==FALSE)
					{
						Dom.Weld(A[8].x+webT/2+dVSWeldPosX, dVSWeldPosY, 1, 1, 0, FALSE, TRUE, 6, 0);
						bWeldMarkVStiff = TRUE;
					}
				}
				else
					Dom.DimLineTo(pC->m_VStiffDis_CR[n],1,COMMA(pC->m_VStiffDis_CR[n]));
				disSum += pC->m_VStiffDis_CR[n];
			}
			if(disSum==0)
				Dom.DimLineTo(GTerm+webT,1,COMMA(GTerm+webT));
			else
				Dom.DimLineTo(GTerm-disSum-pC->m_P_CR-dWSp/2+webT/2,1,COMMA(GTerm-disSum-pC->m_P_CR-dWSp/2+webT/2));
			//앵커 용접
			if(pC->m_nAnchorsu_CR>0)
			{
				CDPoint xyWeld = A[2]+pC->GetVectorCross()*(dWSp/2+pC->m_AnchorDis_CR[0]+pSang->m_SlabAnchor_B/4)+pC->GetVectorCross().Rotate90()*pC->m_T2_CR;
				if(bWeldMarkAnchor==FALSE)
				{
					sDom.DrawWeldMark(xyWeld, GI_SA, GI_SA, pBx, 1, 7, 0, FALSE, FALSE, 0);
					bWeldMarkAnchor = TRUE;
				}
			}
		}

		if(bWeldMarkFlange==FALSE)
		{
			// 상부플랜지 용접
			long	nVStiff		= pC->m_nVStiffsu_CR;
			long	nMarkBuje1	= pBx->IsJijum() ? CP_F_U : CG_F_U;
			long	nMarkBuje2	= pBx->IsJijum() ? CP_W : CG_W;
			double	dDis		= 0;
			CDPoint	xy(0,0);
			for(long i=0; i< nVStiff ; i++)
			{
				dDis += pC->m_VStiffDis_CR[i];
				if(i >= nVStiff/2)	break;
			}
			if(dDis==0)
				dDis = fabs(A[8].x-A[9].x)/2;
			xy.x = A[8].x + dDis - sDom.Always(10);
			xy.y = A[8].y + (dDis - sDom.Always(10))*dGiulCross;
			if(pC->GetType()!=CR_TYPE_HSTEEL)
				sDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx, -1, 7, 0, FALSE, FALSE, 0, TRUE, "");
			Dom << sDom;
			// 하부플랜지 용접
			nMarkBuje1 = pBx->IsJijum() ? CP_F_L : CG_F_L;
			nMarkBuje2 = pBx->IsJijum() ? CP_W : CG_W;
			xy.y -= pC->m_H_CR;
			if(pC->GetType()!=CR_TYPE_HSTEEL)
				sDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx, -1, 7, 0, FALSE, TRUE, 0, TRUE, "");
			Dom << sDom;

			if(nArrBeam.GetAt(0) == 0)
			{
				nMarkBuje1 = pBx->IsJijum() ? CP_BR_F_U : CG_BR_F_U;
				nMarkBuje2 = G_F_U;
				xy.x = B[7].x;
				xy.y = B[7].y+pC->m_T2_CR;
				if(pC->GetType()!=CR_TYPE_HSTEEL)
					sDom.DrawWeldMark(xy, nMarkBuje1, nMarkBuje2, pBx, -1, 10, 0, FALSE, TRUE, 0, TRUE, "");
				Dom << sDom;
			}			

			bWeldMarkFlange = TRUE;
		}

		if(nG+1 < nGirsu )
			dSumGir += dX[nG+1]-dX[nG];
	}
	
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dX[0] + dTotL, max(B[2].y, B[7].y), 2,"");
	Dom.DimLineTo(dSumGir, 2, COMMA(dSumGir));
	*pDom << Dom;

}

void CAPlateDrawCross::DimCRBeamSangseOne(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	  = pBx->GetGirder();
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateCrossBeam	 *pC	  = pBx->GetSection()->GetCrossBeam();

	if(pC == NULL)	return;

	CDPoint A[10], B[10];
	CDPoint vX(1,0), vY(0,1);
 	
	pC->GetCrossXyDom(A);
 	pGir->GetHunchXyDom(pBx, B);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	
	double dHGir			= pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dGussetStiffH	= dHGir - (pC->m_T1_CR+pC->m_H_CR+pC->m_dLH1 + (pC->m_dLH1>0?pC->m_T2_CR:0));
	double dWVStiffL		= pC->m_pVStiff->m_dWidth[0];
	double dWVStiffR		= pC->m_pVStiff->m_dWidth[1];
	double dTFlangeU		= pGir->GetThickFactByStation(pBx->GetStation(), 0);
	double dTFlangeD		= pGir->GetThickFactByStation(pBx->GetStation(), 1);
	double dTWeb			= pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWidthGirD		= pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
	long   nCol				= 0;
	double dLGirToWebRight = pC->GetLengthGirWebToCrWeb(FALSE);

	CDPoint xyStt			= B[1] - vY*dHGir;
	CDPoint vDir			= pC->GetVectorCross();

	// 거더 수직보강재 제원 ========================================
	Dom.SetDirection("TOP");
	CVector vJ      = pGir->GetJewonTotal(V_ST, pBx, 0, 1);//좌측수직보강재
	CString szJewon = "";

	CVStiff *pVStiff = pBx->GetSection()->GetVStiff();
	if(pVStiff->m_dWidth[1] > 0 && vJ.z > 0)
	{
		szJewon = pOptStd->GetJewonString(1, vJ.x, vJ.y, vJ.z);
		Dom.CircleArrow((B[1].x+B[2].x)/2+dWVStiffR*0.3, B[1].y-dHGir+dWVStiffR/2, dWVStiffR, -dWVStiffR*2, 3, "수직보강재", szJewon);
	}

	// 가로보제원 ==================================================
	Dom.SetDirection("LEFT");

	if(pC->m_nLGirSpType)//용접이음
	{
		Dom.DimMoveTo(xyStt.x, xyStt.y ,nCol);
		Dom.DimLineTo(dGussetStiffH, nCol, COMMA(dGussetStiffH));
		Dom.DimLineTo(pC->m_T1_CR, nCol, COMMA(pC->m_T1_CR));
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		if(pC->m_dLH1>0)
		{
			Dom.DimLineTo(pC->m_T2_CR, nCol, COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dLH1, nCol, COMMA(pC->m_dLH1));
		}
	}
	else                 //볼트이음
	{
		double dLowerSp = pGir->GetHeightGirderByStaAng(pBx->GetStation()) 
			            - pC->m_dLH1 - pC->m_T1_CR - (pC->m_dLH1==0 ? 0 : pC->m_T2_CR)
						- pC->m_H_CR;
		Dom.DimMoveTo(xyStt.x, xyStt.y ,nCol);
		Dom.DimLineTo(dLowerSp, nCol, COMMA(dLowerSp));
		Dom.DimLineTo(pC->m_T1_CR,nCol, COMMA(pC->m_T1_CR));
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		if(pC->m_dLH1>0)
		{
			Dom.DimLineTo(pC->m_T2_CR,nCol, COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dLH1, nCol, COMMA(pC->m_dLH1));
		}
	}

	DimCRBeamSpliceTotal(&Dom, pBx, A[2].x, A[2].y, FALSE, 2);
	
	if(!pC->m_nLGirSpType)
	{
		Dom.SetDirection("RIGHT");
		//복부판 볼트이음
		nCol = 0;
		double dYoundanDisWeb = (pC->m_H_CR-(pC->m_wE*pC->m_wF))/2;
		xyStt = pC->xyBoltPos[0];
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));
		CString str, str1, str2;
		if(pC->m_wE>1)
		{
			str1.Format("%d@%d", pC->m_wE, pC->m_wF);
			str2.Format("=%s", COMMA(pC->m_wE*pC->m_wF));
		}
		else
		{
			str1.Format("%s", COMMA(pC->m_wF));
			str2 = _T("");
		}
		Dom.DimLineToExtend(pC->m_wE*pC->m_wF, nCol, str1, str2);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));

		///
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(xyStt.x, xyStt.y+pC->m_wE*pC->m_wF+dYoundanDisWeb-pC->m_dwH, nCol);
		Dom.DimLineTo(pC->m_dwH, nCol, COMMA(pC->m_dwH));
		Dom.SetDirection("RIGHT");
		///
		nCol = 1;
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		//브라켓보강재 볼트이음
		if(pC->m_cGasset_CR)
		{
			double dLowerSpace = (pC->m_dltH - pC->m_ltE*pC->m_ltF)/2;
			double dB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
			Dom.DimMoveTo(pC->xyBoltPos[1].x, pC->xyBoltPos[1].y-dLowerSpace, 0, "", TRUE);
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineTo(pC->m_ltE*pC->m_ltF, 0, COMMA(pC->m_ltE*pC->m_ltF));
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineToExtend(dB, 0, "", COMMA(dB));
			Dom.DimMoveTo(pC->xyBoltPos[1].x, pC->xyBoltPos[1].y-dLowerSpace, 1, "", TRUE);
			Dom.DimLineTo(pC->m_dltH+dB, 1, COMMA(pC->m_dltH+dB));
		}
	}
	//Bottom Dimension
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음
	double dS1 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S1);
	xyStt = (B[1]+B[2])/2-vX*dWidthGirD/2 - vY*(dTFlangeD+dHGir);

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "", TRUE);
	if(dWVStiffL>0)
	{
		Dom.DimLineTo(dWidthGirD/2-dWVStiffL-dTWeb/2, 0, COMMA(dWidthGirD/2-dWVStiffL-dTWeb/2));
		Dom.DimLineTo(dWVStiffL, 0, COMMA(dWVStiffL));
		Dom.DimLineToExtend(dTWeb, 0, "", COMMA(dTWeb));
	}
	else
	{
		Dom.DimLineTo(dWidthGirD/2-dTWeb/2, 0, COMMA(dWidthGirD/2-dTWeb/2));
		Dom.DimLineToExtend(dTWeb, 0, "", COMMA(dTWeb));
	}
	if(dWVStiffR>0)
	{
		if(pC->m_nLGirSpType)
		{
			Dom.DimLineTo(dWidthGirD/2-dTWeb/2-dS1, 0, COMMA(dWidthGirD/2-dTWeb/2-dS1));
			Dom.DimLineTo(dS1, 0, COMMA(dS1));
		}
		else
		{
			Dom.DimLineTo(dLGirToWebRight, 0, COMMA(dLGirToWebRight));
			Dom.DimLineTo(pC->m_dbR2, 0, COMMA(pC->m_dbR2));
			Dom.DimLineTo(dWVStiffR-dLGirToWebRight-pC->m_dbR2, 0, COMMA(dWVStiffR-dLGirToWebRight-pC->m_dbR2));
			Dom.DimLineTo(dWidthGirD/2-dWVStiffR-dTWeb/2, 0, COMMA(dWidthGirD/2-dWVStiffR-dTWeb/2));
		}
	}
	else
	{
		Dom.DimLineTo(dWidthGirD/2-dTWeb/2, 0, COMMA(dWidthGirD/2-dTWeb/2));
	}
	//RIGHT Dimension HBracing	
	Dom.SetDirection("RIGHT");
	CHBracingGusset* pBrGu = pBx->GetHBracingGusset();
	if(pBrGu->IsReal())
	{
		double dWHBr = pBrGu->m_dH[1];
		CDPoint xyHBr = (B[1]+B[2])/2 + vX*(dTWeb/2+dWHBr) - vY*dHGir;
		Dom.DimMoveTo(xyHBr.x, xyHBr.y, 0, "", TRUE);
		Dom.DimLineTo(dWHBr, 0, COMMA(dWHBr));
		Dom.DimLineTo(pBrGu->m_dT, 0, COMMA(pBrGu->m_dT));
		CDPoint xyHBrGisi = xyHBr+vY*(dWHBr+pBrGu->m_dT/2);
		Dom.SetDirection("TOP");
		Dom.CircleArrow(xyHBrGisi.x, xyHBrGisi.y, Dom.Always(20), Dom.GetTextWidth("수평브레이싱 참조"), 7, "수평브레이싱 참조", "");
		
	}
	Dom.SetDirection("TOP");
	if(!pC->m_nLGirSpType)
	{
		Dom.DimMoveTo(B[2].x, B[2].y, 0, COMMA(dWVStiffR+pC->m_dbL4-pGir->GetLengthSharpOnBx(pBx, TRUE)));
		Dom.DimLineTo(dWVStiffR+pC->m_dbL4-pGir->GetLengthSharpOnBx(pBx, TRUE), 0, "");
	}

	//weld
	CDPoint xyWeld = (B[1]+B[2])/2 - vX*(dTWeb/2) - vY*dHGir; 
	Dom.WeldByValue(xyWeld.x, xyWeld.y, 6, 0, TRUE, TRUE); 
	//gisisun(상부플랜지와의용접)
	CString str = "";
	double dArcR = dTFlangeU <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
	str.Format("R=%d", (long)dArcR);
	CDPoint xyGisi = (B[1]+B[2])/2 + vX*(dTWeb/2) + CDPoint(1,-1).Unit()*dArcR; 
	Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(8), Dom.Always(8), 7, str);
	//gisisun(하부플랜지와의 용접)
	dArcR = dTFlangeD <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
	str.Format("R=%d", (long)dArcR);
	xyGisi = (B[1]+B[2])/2 + vX*(dTWeb/2) - vY*dHGir + CDPoint(1,1).Unit()*dArcR; 
	Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(8), Dom.Always(8), 1, str);

	*pDom << Dom;	
}

void CAPlateDrawCross::DimCRBeamSangseTwo(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	 *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirR   = pBridge->GetGirder(1);
	CPlateCrossBeam  *pC      = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex *pBxR    = pBx->GetBxMatchByCrossBeam(pGirR);
	CPlateCrossBeam  *pCR     = pBxR->GetSection()->GetCrossBeam();
	CHBracingGusset  *pBrGu   = pBxR->GetHBracingGusset();

	if(pC == NULL)	return;

	CDPoint A[10], B[10];
	CDPoint vX(1,0), vY(0,1);

 	pC->GetCrossXyDom(A);
 	pGir->GetHunchXyDom(pBx, B);
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);

	double dHGir         = pGirR->GetHeightGirderByStaAng(pBxR->GetStation());
	double dGussetStiffH = dHGir - (pC->m_T1_CR + pC->m_H_CR + pC->m_dRH1 + (pC->m_dRH1>0?pC->m_T2_CR:0));
	double dWidthGirU    = pGirR->GetWidthOnStation(pBxR->GetStation(), TRUE);
	double dWidthGirD    = pGirR->GetWidthOnStation(pBxR->GetStation(), FALSE);
	double dWVStiffL     = pCR ? pCR->m_pVStiff->m_dWidth[0] : 0;
	double dWVStiffR     = pCR ? pCR->m_pVStiff->m_dWidth[1] : 0;
	double dTFlangeU     = pGirR->GetThickFactByStation(pBxR->GetStation(), 0);
	double dTFlangeD     = pGirR->GetThickFactByStation(pBxR->GetStation(), 1);
	double dTWeb         = pGirR->GetThickFactByStation(pBxR->GetStation(), 2);
	long   nCol          = 0;
	double dLGirToWebLeft  = pC->GetLengthGirWebToCrWeb(TRUE);
	
	CDPoint xyStt = B[7] + vX*dWidthGirU - vY*dHGir;
	double dSpace = 0;
	if(pCR && pCR->m_nLGirSpType)							// 이음판 있을때
		dSpace = dTWeb/2 + pCR->m_P_CR + pCR->m_sA+pCR->m_sB*pCR->m_sC+pCR->m_sC1 + 100;				
	else if(pCR && pCR->m_cGasset_CR)						// 이음판이 없고 브라켓보강판이 있을경우
		dSpace = dTWeb/2 + pCR->m_P_CR + 100;
	CDPoint xyDim = (B[7]+B[8])/2  + vX*dSpace;
	xyStt.x = xyDim.x;

	CDPoint vDir         = pC->GetVectorCross();

	// 거더 수직보강재 제원 ========================================
	Dom.SetDirection("TOP");
	CVector vJ      = pGirR->GetJewonTotal(V_ST, pBxR, 0, 0);//우측수직보강재
	CString szJewon = "";

	CVStiff *pVStiff = pBxR->GetSection()->GetVStiff();
	if(pVStiff->m_dWidth[0] > 0 && vJ.z > 0)
	{
		szJewon = pOptStd->GetJewonString(1, vJ.x, vJ.y, vJ.z);
		Dom.CircleArrow((B[7].x+B[8].x)/2-dWVStiffL*0.3, B[7].y-dHGir+dWVStiffL/2, dWVStiffL, dWVStiffL*2, 1, "수직보강재", szJewon);
	}

	//가로보제원	========================================
	Dom.SetDirection("RIGHT");

	if(pC->m_nRGirSpType)//용접이음
	{
		Dom.DimMoveTo(xyStt.x, xyStt.y ,nCol);
		Dom.DimLineTo(dGussetStiffH, nCol, COMMA(dGussetStiffH));
		Dom.DimLineTo(pC->m_T1_CR, nCol, COMMA(pC->m_T1_CR));
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		if(pC->m_dRH1>0)
		{
			Dom.DimLineTo(pC->m_T2_CR, nCol, COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dRH1, nCol, COMMA(pC->m_dRH1));
		}
	}
	else  //볼트이음
	{
		double dLowerSp = pGir->GetHeightGirderByStaAng(pBxR->GetStation()) 
			            - pC->m_dRH1 - pC->m_T1_CR - (pC->m_dRH1==0 ? 0 : pC->m_T2_CR)
						- pC->m_H_CR;
		Dom.DimMoveTo(xyStt.x, xyStt.y ,nCol);
		Dom.DimLineTo(dLowerSp, nCol, COMMA(dLowerSp));
		Dom.DimLineTo(pC->m_T1_CR,nCol, COMMA(pC->m_T1_CR));
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		if(pC->m_dRH1>0)
		{
			Dom.DimLineTo(pC->m_T2_CR,nCol, COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dRH1, nCol, COMMA(pC->m_dRH1));
		}
	}

	DimCRBeamSpliceTotal(&Dom, pBx, A[5].x, A[5].y, FALSE, 2);
	if(!pC->m_nRGirSpType)
	{
		Dom.SetDirection("LEFT");
		//복부판 볼트이음
		nCol = 0;
		double dYoundanDisWeb = (pC->m_H_CR-(pC->m_wE*pC->m_wF))/2;
		xyStt = pC->xyBoltPos[2];
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));
		CString str, str1, str2;
		if(pC->m_wE>1)
		{
			str1.Format("%d@%d", pC->m_wE, pC->m_wF);
			str2.Format("=%s", COMMA(pC->m_wE*pC->m_wF));
		}
		else
		{
			str1.Format("%s", COMMA(pC->m_wF));
			str2 = _T("");
		}
		Dom.DimLineToExtend(pC->m_wE*pC->m_wF, nCol, str1, str2);
		Dom.DimLineTo(dYoundanDisWeb , nCol, COMMA(dYoundanDisWeb));

		///
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyStt.x, xyStt.y+pC->m_wE*pC->m_wF+dYoundanDisWeb-pC->m_dwH, nCol);
		Dom.DimLineTo(pC->m_dwH, nCol, COMMA(pC->m_dwH));
		Dom.SetDirection("LEFT");
		///
		nCol = 1;
		Dom.DimMoveTo(xyStt.x, xyStt.y-dYoundanDisWeb, nCol);
		Dom.DimLineTo(pC->m_H_CR, nCol, COMMA(pC->m_H_CR));
		//브라켓보강재 볼트이음
		if(pC->m_cGasset_CR)
		{
			double dLowerSpace = (pC->m_drtH - pC->m_rtE*pC->m_rtF)/2;
			double dB = pBridge->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
			Dom.DimMoveTo(pC->xyBoltPos[3].x, pC->xyBoltPos[3].y-dLowerSpace, 0, "", TRUE);
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineTo(pC->m_rtE*pC->m_rtF, 0, COMMA(pC->m_rtE*pC->m_rtF));
			Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
			Dom.DimLineToExtend(dB, 0, "", COMMA(dB));
			Dom.DimMoveTo(pC->xyBoltPos[3].x, pC->xyBoltPos[3].y-dLowerSpace, 1, "", TRUE);
			Dom.DimLineTo(pC->m_drtH+dB, 1, COMMA(pC->m_drtH+dB));
		}
	}

	//Bottom Dimension
	xyStt = B[7] + vX*(dWidthGirU/2-dWidthGirD/2) - vY*(dTFlangeD+dHGir);
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음
	double dS1 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S1);
	
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "", TRUE);
	if(dWVStiffL>0)
	{
		if(pC->m_nRGirSpType)
		{
			Dom.DimLineTo(dS1, 0, COMMA(dS1));
			Dom.DimLineTo(dWidthGirD/2-dTWeb/2-dS1, 0, COMMA(dWidthGirD/2-dTWeb/2-dS1));
			Dom.DimLineToExtend(dTWeb, 0, "", COMMA(dTWeb));
		}
		else
		{
			Dom.DimLineTo(dWidthGirD/2-dWVStiffL-dTWeb/2, 0, COMMA(dWidthGirD/2-dWVStiffL-dTWeb/2));
			Dom.DimLineTo(dWVStiffL-dLGirToWebLeft-pC->m_dbL2, 0, COMMA(dWVStiffL-dLGirToWebLeft-pC->m_dbL2));
			Dom.DimLineTo(pC->m_dbL2, 0, COMMA(pC->m_dbL2));
			Dom.DimLineTo(dLGirToWebLeft, 0, COMMA(dLGirToWebLeft));
			Dom.DimLineToExtend(dTWeb, 0, "", COMMA(dTWeb));
		}
	}
	else
	{
		Dom.DimLineTo(dWidthGirD/2-dTWeb/2, 0, COMMA(dWidthGirD/2-dTWeb/2));
		Dom.DimLineTo(dTWeb, 0, COMMA(dTWeb));
	}
	if(dWVStiffR>0)
	{
		Dom.DimLineTo(dWVStiffR, 0, COMMA(dWVStiffR));
		Dom.DimLineTo(dWidthGirD/2-dTWeb/2-dWVStiffR, 0, COMMA(pC->m_dbR2));
	}
	else
	{
		Dom.DimLineTo(dWidthGirD/2-dTWeb/2, 0, COMMA(dWidthGirD/2-dTWeb/2));
	}
	//RIGHT Dimension HBracing	
	Dom.SetDirection("LEFT");
	if(pBrGu->IsReal())
	{
		double dWHBr = pBrGu->m_dH[0];
		CDPoint xyHBr = B[7] - vX*(dTWeb/2+dWHBr-dWidthGirD/2) - vY*dHGir;
		Dom.DimMoveTo(xyHBr.x, xyHBr.y, 0, "", TRUE);
		Dom.DimLineTo(dWHBr, 0, COMMA(dWHBr));
		Dom.DimLineTo(pBrGu->m_dT, 0, COMMA(pBrGu->m_dT));
		CDPoint xyHBrGisi = xyHBr+vY*(dWHBr+pBrGu->m_dT/2) + vX*dWHBr*2;
		Dom.SetDirection("TOP");
		Dom.CircleArrow(xyHBrGisi.x, xyHBrGisi.y, Dom.Always(20), Dom.GetTextWidth("수평브레이싱 참조"), 1, "수평브레이싱 참조", "");
	}
	Dom.SetDirection("TOP");
	if(!pC->m_nRGirSpType)
	{
		Dom.DimMoveTo(B[7].x-(dWVStiffL+pC->m_dbR4-pGirR->GetLengthSharpOnBx(pBxR, TRUE)), B[7].y, 0, COMMA(dWVStiffL+pC->m_dbR4-pGirR->GetLengthSharpOnBx(pBxR, TRUE)));
		Dom.DimLineTo((dWVStiffL+pC->m_dbR4-pGirR->GetLengthSharpOnBx(pBxR, TRUE)), 0, "");
	}

	//weld
	CDPoint xyWeld = (B[7]+B[8])/2 + vX*(dTWeb/2) - vY*dHGir; 
	Dom.WeldByValue(xyWeld.x, xyWeld.y, 6, 0, FALSE, TRUE);
	//gisisun(상부플랜지와의용접)
	CString str = "";
	double dArcR = dTFlangeU <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
	str.Format("R=%d", (long)dArcR);
	CDPoint xyGisi = B[7]+vX*pGir->GetLengthSharpOnBx(pBx, TRUE) + CDPoint(-1,-1).Unit()*dArcR; 
	Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(8), -Dom.Always(8), 5, str);
	//gisisun(하부플랜지와의용접)
	dArcR = dTFlangeD <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
	str.Format("R=%d", (long)dArcR);
	xyGisi = (B[7]+B[8])/2 + vX*(-dTWeb/2) - vY*dHGir + CDPoint(-1,1).Unit()*dArcR; 
	Dom.GiSiArrowExt(xyGisi.x, xyGisi.y, Dom.Always(8), -Dom.Always(8), 3, str);

	*pDom << Dom;	

}

void CAPlateDrawCross::DrawVBracingDetailAA(CDomyun* pDom, CPlateBasicIndex* pBx, BOOL bDim)
{
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CAPlateDrawPyung DrawPyung(m_pDataMng);
	if(pBx->GetSection()->IsenDetType(SECDET_VBRACING)==NULL) return;

	CDomyun Dom(pDom);
	double dGirLen = 400;
	CDPoint vX(1,0);
	CDPoint vAng  = pBx->GetAngleSectionCrossVBracing();
	CDPoint xyCen = pGir->m_pRight->GetXyGirderDis(pBxNext->GetStation());
	CDPoint vGir  = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	vX = vAng;
	DrawPyung.DrawVBracingFlangeOnBx(&Dom, pBx);
	DrawPyung.DrawVBracingFlangeOnBx(&Dom, pBxNext);
	DrawPyung.DrawVStiffOnBx(&Dom, pBxNext);
	DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBx, FALSE, FALSE);
	DrawPyung.DrawVStiffSpliceOnBx(&Dom, pBxNext, TRUE, FALSE);
	DrawPyung.DrawGirderWithenStationByLine(&Dom, pBxNext, pBxNext->GetStation()-dGirLen, pBxNext->GetStation()+dGirLen, FALSE);
	//지점부 수직보강재
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	if(pBxNext->IsJijum())
		DrawPyung.DrawJiJumStiffOnBx(&Dom, pBxNext, TRUE, FALSE);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.Circle(xyCen, 400);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.TrimByCircle(xyCen, 400, TRUE);
	Dom.Rotate(-vGir.GetAngleRadian(),xyCen.x,xyCen.y);
	Dom.Rotate(CDPoint(0,1).GetAngleRadian(),xyCen.x,xyCen.y);
	*pDom << Dom;

	if(bDim)
	{
		CVBracing* pVr   = pBx->GetSection()->GetVBracing();
		CVStiff* pVStiff = pBx->GetSection()->GetVStiff();
		double dHBeam = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, H);
		CDPoint vVr = pBx->GetAngleSectionCrossVBracing().Rotate90();
		CDPoint xyArrow = xyCen + vGir*dGirLen/2;
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetDirection("TOP");
		vGir.y = vGir.y*-1;
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, CDPoint(0,1)); 
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, vGir); 
		Dom.CircleArrow(xyArrow.x, xyArrow.y, 0, dGirLen/2, 0 ,"주형", "");
	
		xyArrow = xyCen - vX*20; 
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, CDPoint(0,1)); 
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, vGir); 
		Dom.CircleArrow(xyArrow.x, xyArrow.y, dGirLen/2, -dGirLen/2, 2 ,"수직보강재", "");
		
		xyArrow = xyCen + vX*pBx->GetSection()->GetVStiff()->m_dWidth[1]
			            + vGir*pBx->GetSection()->GetVStiff()->m_dThick[1]; 
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, CDPoint(0,1)); 
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, vGir); 
		Dom.CircleArrow(xyArrow.x, xyArrow.y, dGirLen/2, Dom.GetTextWidth("GUSSET PLATE"), 2 ,"GUSSET PLATE", "");

		xyArrow = xyCen + vX*(pVStiff->m_dWidth[1]+pVr->m_duL4/2)
			            - vGir*(dHBeam)/2; 
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, CDPoint(0,1)); 
		xyArrow  = xyArrow.GetXyRotateByBase(xyCen, vGir); 
		Dom.CircleArrow(xyArrow.x, xyArrow.y, dGirLen/2, Dom.GetTextWidth("하현재"), 2 ,"하현재", "");

		*pDom << Dom;
	}

}


void CAPlateDrawCross::DrawVBracingDetailBB(CDomyun* pDom, CPlateBasicIndex* pBx, BOOL bDim)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();

	if(pBx->GetSection()->IsenDetType(SECDET_VBRACING)==NULL) return;

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CVBracing* pVr = pBx->GetSection()->GetVBracing();
	double dT = pVr->GetJewonSplice(CVBracing::LD, CVBracing::THICK); 
	double dH = pVr->GetJewonSplice(CVBracing::LD, CVBracing::HEIGHT); 
	CDPoint xyStt(0,0), vX(1,0), vY(0,1);
	//수직브레이싱 이음판
	Dom.LineTo(xyStt, xyStt-vY*dH);
	Dom.LineTo(xyStt-vY*dH, xyStt-vY*dH+vX*dT);
	Dom.LineTo(xyStt-vY*dH+vX*dT, xyStt+vX*dT);
	Dom.LineTo(xyStt, xyStt+vX*dT);
	//수직브레이싱 단면
	double dHBeam    = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	double dTBeam    = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::T1);
	double dLowerGap = pVr->m_ddL2-dHBeam/2;
	CDPoint xyMid    = xyStt-vY*(dH-dLowerGap)+vX*dT;
	long nSteelType  = pVr->GetSteelMaterialData(STEELMAT_UPPER)->m_nType;//pMat->GetSteelType(pBx, CSteelMaterial::BUJE_UVBEAM);
	switch(nSteelType)
	{
	case CSteelMaterialData::LSTEEL:
		{
			CDPoint xyTemp = xyMid+vY*dHBeam/2;
			Dom.MoveTo(xyTemp);
			xyTemp -= vY*dHBeam;
			Dom.LineTo(xyTemp);
			xyTemp += vX*dHBeam;
			Dom.LineTo(xyTemp);
			xyTemp += vY*dTBeam;
			Dom.LineTo(xyTemp);
			xyTemp -= vX*(dHBeam-dTBeam);
			Dom.LineTo(xyTemp);
			xyTemp += vY*(dHBeam-dTBeam);
			Dom.LineTo(xyTemp);
			xyTemp -= vX*dTBeam;
			Dom.LineTo(xyTemp);
		}
		break;
	case CSteelMaterialData::LVALSTEEL:
		{

		}
		break;
	case CSteelMaterialData::CSTEEL:
		{

		}
		break;
	case CSteelMaterialData::CTSTEEL:
		{

		}
		break;
	case CSteelMaterialData::ISTEEL:
		{

		}
		break;
	case CSteelMaterialData::HSTEEL:
		{

		}
		break;
	}	
	//지시선
	if(bDim)
	{
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.CircleArrow(xyMid.x-dT/2, xyStt.y-dH*0.3, 0, Dom.GetTextWidth("GUSSET PLATE"), 0 ,"GUSSET PLATE", "");
		CDPoint xyLower = xyMid+vX*dTBeam/2-vY*(dHBeam/2);
		Dom.CircleArrow(xyLower.x, xyLower.y, dH/2, Dom.GetTextWidth("하현재"), 6,"하현재", "");
	}
	*pDom << Dom;

}


void CAPlateDrawCross::DrawVBracingDetail(CDomyun* pDom, CPlateBasicIndex* pBx, long nPos)
{
	CPlateBridgeApp	*pBridge		= m_pDataMng->GetBridge();
	long nGirsu = pBridge->GetGirdersu();
	CDPoint vX(1,0), vY(0,1);
	CPlateGirderApp* pGirStt = pBridge->GetGirder(0);
	CPlateBasicIndex *pBxStt = pBx->GetBxMatchByCrossBeam(pGirStt);	
	CPlateGirderApp* pGirEnd = pBridge->GetGirder(nGirsu-1);
	CPlateBasicIndex *pBxEnd = pBx->GetBxMatchByCrossBeam(pGirEnd);	
	CPlateGirderApp* pGirEE = pBridge->GetGirder(nGirsu-2);
	CPlateBasicIndex *pBxEE = pBx->GetBxMatchByCrossBeam(pGirEE);	

	CVBracing* pVrStt = pBxStt->GetSection()->GetVBracing();
	CVBracing* pVrEnd = pBxEnd->GetSection()->GetVBracing();
	CDPoint Astt[10], Aend[10];
	pGirStt->GetHunchXyDom(pBxStt, Astt);
	pGirEnd->GetHunchXyDom(pBxEnd, Aend);
	CDomyun Dom(pDom);
	//상세1
	if(nPos==1)
	{
		DrawGirderFront(&Dom, pBxStt, UPPER);
		DrawHStiffFront(&Dom, pBxStt, TRUE);
		DrawHStiffFront(&Dom, pBxStt, FALSE);
		DrawVBracingSpliceSangseOnCRBx(&Dom, pBxStt, FALSE, nPos);
		DrawVBracingBeamSangSeOnCRBx(&Dom, pBxStt, nPos);
	}
	if(nPos==2)
	{
		DrawGirderFront(&Dom, pBxStt, LOWER);
		DrawHStiffFront(&Dom, pBxStt, TRUE);
		DrawHStiffFront(&Dom, pBxStt, FALSE);
		DrawVBracingSpliceSangseOnCRBx(&Dom, pBxStt, FALSE, nPos);
		DrawVBracingBeamSangSeOnCRBx(&Dom, pBxStt, nPos);
		DrawFrontHGussetSplice(&Dom, pBx, FALSE);
	}
	if(nPos==3)
	{
		DrawGirderFront(&Dom, pBxEnd, UPPER);
		DrawHStiffFront(&Dom, pBxStt, TRUE);
		DrawHStiffFront(&Dom, pBxStt, FALSE);
		DrawVBracingSpliceSangseOnCRBx(&Dom, pBxEE, FALSE, nPos);
		DrawVBracingBeamSangSeOnCRBx(&Dom, pBxEE, nPos);
	}
	if(nPos==4)
	{
		DrawGirderFront(&Dom, pBxEnd, LOWER);
		DrawHStiffFront(&Dom, pBxStt, TRUE);
		DrawHStiffFront(&Dom, pBxStt, FALSE);
		DrawVBracingSpliceSangseOnCRBx(&Dom, pBxEE, FALSE, nPos);
		DrawVBracingBeamSangSeOnCRBx(&Dom, pBxEE, nPos);
		DrawFrontHGussetSplice(&Dom, pBxEnd, FALSE);
	}
	if(nPos==5)
	{
		DrawVBracingSpliceSangseOnCRBx(&Dom, pBxStt, FALSE, nPos);
		DrawVBracingBeamSangSeOnCRBx(&Dom, pBxStt, nPos);
	}
	*pDom << Dom;
}

void CAPlateDrawCross::DimVBracingDetail(CDomyun* pDom, CPlateBasicIndex* pBx, long nPos)
{
	CDomyun Dom(pDom);
	
	switch(nPos)
	{
	case 1:
		DimVBracingDetailOne(&Dom, pBx);
		break;
	case 2:
		DimVBracingDetailTwo(&Dom, pBx);
		break;
	case 3:
		DimVBracingDetailThree(&Dom, pBx);
		break;
	case 4:
		DimVBracingDetailFour(&Dom, pBx);
		break;
	case 5:
		DimVBracingDetailFive(&Dom, pBx);
		break;
	}
	*pDom << Dom;

}

void CAPlateDrawCross::DimVBracingDetailOne(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	CARoadOptionStd *pOptStd		= m_pDataMng->GetOptionStd();
	CPlateGirderApp* pGir           = pBx->GetGirder();//0번거더
	CVBracing* pVr   = pBx->GetSection()->GetVBracing();
	CVStiff* pVStiff = pBx->GetSection()->GetVStiff();
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);
	double dSharp  = pGir->GetLengthSharpOnBx(pBx, TRUE); 
	double dHGir   = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dTW     = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWStiffL= pVStiff->m_dWidth[0];
	double dWStiff = pVStiff->m_dWidth[1];
	double dTStiffL= pVStiff->m_dThick[0];
	double dHBeam  = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
	double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
	if(!pOpt->GetInstallBeamAtCenter())
		dHBeamCen = dHBeam/2;
	double dRatio  = 0.4;
	long   nDimDan = 0;
	CDPoint vUpperBr = pVr->GetVectorFrame(0);
	CDPoint vX(1,0), vY(0,1);
	CString str;

	CDomyun Dom(pDom);
	Dom.SetDirection("LEFT");
	CDPoint vOrigin = pVr->xyBoltPos[0];
	double dGap  = (pVr->m_duL1+pVr->m_duL2-(pVr->m_uB*pVr->m_uC))/2;
	//수직보강재 우측모서리와 LT이음판의 상단 끝단이 교차하는 점
	CDPoint xyLT_VStiff = (A[1]+A[2])/2 - vY*pVr->m_dLH1 + vUpperBr*(dWStiff+dTW/2)/fabs(vUpperBr.x)
		                 + vY*(pVr->m_duL1-((dHBeam-dHBeamCen)/fabs(vUpperBr.x)));
	double dLenTotal = 0;
	Dom.DimMoveTo(A[1].x, vOrigin.y-dGap, nDimDan, "", TRUE);	
	Dom.DimLineTo(dGap, nDimDan, COMMA(dGap), TRUE);		dLenTotal+=Round(dGap, 0);

	CPlatePieceObject Obj1(pGir);
	pVr->CreatePieceObj(&Obj1, TRUE, TRUE, 0);

	long n = 0;
	if(Obj1.GetChildCount()>0 && Obj1.GetChild(0)->GetObjSize()==(pVr->m_uB+1))
	{	
		str.Format("%d@%d=%d", pVr->m_uB, pVr->m_uC, pVr->m_uB*pVr->m_uC);
		Dom.DimLineToExtend(pVr->m_uB*pVr->m_uC, nDimDan, "", str, TRUE);	dLenTotal+=pVr->m_uB*pVr->m_uC;
	}
	else
	{
		double dTLen=0;
		CDPointArray ptPosArr;
		for(n=0; n<Obj1.GetChild(0)->GetObjSize(); n++)
			ptPosArr.Add(Obj1.GetChild(0)->GetPoint(n));
		// Cut
		if(Obj1.GetObjSize() >= 8)
		{
			ptPosArr.Add(Obj1.GetPoint(Obj1.GetObjSize()-1));
			ptPosArr.Add(Obj1.GetPoint(Obj1.GetObjSize()-8));
			ptPosArr.Sort(FALSE,TRUE);
			for(n=0; n<ptPosArr.GetSize()-1; n++)
			{
				CDPoint ptPos1 = ptPosArr.GetAt(n);
				CDPoint ptPos2 = ptPosArr.GetAt(n+1);
				str = COMMA(ptPos2.y - ptPos1.y);
				Dom.DimLineToExtend(ptPos2.y - ptPos1.y, nDimDan, n%2 ? str : "", n%2 ? "" : str, TRUE);	dLenTotal+=(ptPos2.y - ptPos1.y);
				dTLen += ptPos2.y - ptPos1.y;
			}
			dGap += pVr->m_uB*pVr->m_uC - dTLen;
		}
	}

	Dom.DimLineTo(dGap, nDimDan, COMMA(dGap), TRUE);	dLenTotal+=Round(dGap, 0);
	Dom.DimLineTo(A[2].y - xyLT_VStiff.y, nDimDan, COMMA(A[2].y - xyLT_VStiff.y), TRUE);	dLenTotal+=Round(A[2].y - xyLT_VStiff.y, 0);

	nDimDan = 1;
	Dom.DimMoveTo(A[1].x, vOrigin.y-dGap, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_duL1+pVr->m_duL2 + (A[2].y - xyLT_VStiff.y), nDimDan, COMMA(dLenTotal), TRUE);

	//하부치수선
	nDimDan = 0;
	Dom.SetDirection("BOTTOM");
	double dUpper = pVr==NULL ? dRatio*dHGir : pVr->m_dLH1+pVr->m_duL1+pVr->m_duL2;
	Dom.DimMoveTo(A[1].x, A[1].y - dUpper, nDimDan, "", TRUE);
	Dom.DimLineTo(dSharp-dWStiffL, nDimDan, COMMA(dSharp-dWStiffL), TRUE);
	if(dTStiffL>0)
		Dom.DimLineTo(dWStiffL, nDimDan, COMMA(dWStiffL), TRUE);
	Dom.DimLineToExtend(dTW, nDimDan, "", COMMA(dTW), TRUE);
	Dom.DimLineTo(dWStiff-pVr->m_duL3, nDimDan, COMMA(dWStiff-pVr->m_duL3), TRUE);
	Dom.DimLineTo(pVr->m_duE, nDimDan, COMMA(pVr->m_duE), TRUE);
	Dom.DimLineTo(pVr->m_duL3-pVr->m_duE, nDimDan, COMMA(pVr->m_duL3-pVr->m_duE), TRUE);
	Dom.DimLineTo(pVr->m_duL4, nDimDan, COMMA(pVr->m_duL4));	
	//수직보강재
	CVector vJ = pGir->GetJewonTotal(V_ST, pBx, 0, 1);//우측수직보강재
	CString szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	Dom.CircleArrow((A[1]+A[2]).x/2+dTW/2+dWStiff/2, (A[1]+A[2]).y/2-dWStiff/2, dWStiff, -dWStiff*2, 3, "수직보강재", szJewon);
	//용접선1
	CDPoint vAxis1 = pVr->GetVectorFrame(CVBracing::UPPER);
	CDPoint xyWeld1 = pVr->GetCenterSplice(CVBracing::LT) + vAxis1*pVr->m_duL4*0.8;
	Dom.WeldByValue(xyWeld1.x, xyWeld1.y, 6); 
	//용접선2
	CDPoint vAxis2 = pVr->GetVectorFrame(CVBracing::LEFT);
	if(pVr->GetType()==TYPE_VBRACING)
	{
		CDPoint xyWeld2 = pVr->GetPointFrame(CVBracing::LT) + vAxis2*(pVr->m_dG[2]+pVr->m_dC1LG/2) + vAxis2.Rotate90()*dHBeamCen;
		Dom.WeldByValue(xyWeld2.x, xyWeld2.y, 6, 0, TRUE, FALSE); 
		*pDom << Dom;
	}
	//용접길이
	dGap = pVr->m_duL4 - pVr->m_dULG*vAxis1.x;
	CDPoint xyWeldStt = pVr->GetPointFrame(CVBracing::LT) + vAxis1*(pVr->m_dG[0]-dGap)/vAxis1.x + vY*(dHBeam-dHBeamCen);
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xyWeldStt.x-pVr->m_duL3, xyWeldStt.y, 0);
	Dom.DimLineTo(pVr->m_duL3, 0, COMMA(pVr->m_duL3));
	Dom.DimLineToExtend(dGap, 0, "", COMMA(dGap)); 
	Dom.DimLineTo(pVr->m_dULG, 0, COMMA(pVr->m_dULG));
	*pDom << Dom;

	if(pVr->GetType()==TYPE_VBRACING)
	{
		xyWeldStt =  pVr->GetPointFrame(CVBracing::LT) + vAxis2*pVr->m_dG[2] + vAxis2.Rotate90()*dHBeamCen;
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyWeldStt.x, xyWeldStt.y, 0);
		Dom.DimLineTo(pVr->m_dC1LG, 0, COMMA(pVr->m_dC1LG));
		Dom.Rotate(xyWeldStt, vAxis2);
		*pDom << Dom;
	}
}
void CAPlateDrawCross::DimVBracingDetailTwo(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	CPlateGirderApp* pGir    = pBx->GetGirder();//0번거더
	CVBracing* pVr           = pBx->GetSection()->GetVBracing();
	CVStiff* pVStiff         = pBx->GetSection()->GetVStiff();
	double dSharp= pGir->GetLengthSharpOnBx(pBx, TRUE); 
	double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dWGirLo = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
	double dTW   = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dTFD  = pGir->GetThickFactByStation(pBx->GetStation(), 1);
	double dWStiffL= pVStiff->m_dWidth[0];
	double dWStiff = pVStiff->m_dWidth[1];
	double dTStiffL= pVStiff->m_dThick[0];
	long  nDimDan = 0;
	double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);
	if(!pOpt->GetInstallBeamAtCenter())
		dHBeamCen = dHBeam/2;
	CDPoint vLowerBr = pVr->GetVectorFrame(CVBracing::LOWER);
	CDPoint vX(1,0), vY(0,1);
	CString str;
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);

	CDomyun Dom(pDom);
	Dom.SetDirection("LEFT");//거더좌측치수선
	
	CDPoint xyStt = (A[1]+A[2])/2 - vX*dWGirLo/2 - vY*dHGir;
	CDPoint xyLD_VStiff = (A[1]+A[2])/2 - vY*(dHGir-pVr->m_dLH2) + vLowerBr*(dWStiff+dTW/2)/fabs(vLowerBr.x)
		                - vY*(pVr->m_ddL2-((dHBeam-dHBeamCen)/fabs(vLowerBr.x)));
	double dGap  = (pVr->m_ddL1+pVr->m_ddL2-(pVr->m_dB*pVr->m_dC))/2;
	double dLenTotal = 0;
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "", TRUE);
	Dom.DimLineTo(xyLD_VStiff.y-xyStt.y, nDimDan, COMMA(xyLD_VStiff.y-xyStt.y), TRUE);	dLenTotal += xyLD_VStiff.y-xyStt.y;
	Dom.DimLineTo(dGap, nDimDan, COMMA(dGap), TRUE);									dLenTotal += Round(dGap, 0);
	
	long n = 0;
	//str.Format("%d@%d=%d", long(pVr->m_dB), long(pVr->m_dC), long(pVr->m_dB*pVr->m_dC));
	CPlatePieceObject Obj1(pGir);
	pVr->CreatePieceObj(&Obj1, TRUE, TRUE, 1);
	if(Obj1.GetChildCount()>0 && Obj1.GetChild(0)->GetObjSize()==(pVr->m_dB+1))
	{	
		str.Format("%d@%d=%d", pVr->m_dB, pVr->m_dC, pVr->m_dB*pVr->m_dC);
		Dom.DimLineToExtend(pVr->m_dB*pVr->m_dC, nDimDan, "", str, TRUE);		dLenTotal += pVr->m_dB*pVr->m_dC;
	}
	else
	{
		double dTLen=0;
		CDPointArray ptPosArr;
		for(n=0; n<Obj1.GetChild(0)->GetObjSize(); n++)
			ptPosArr.Add(Obj1.GetChild(0)->GetPoint(n));
		// Cut
		ptPosArr.Add(Obj1.GetPoint(Obj1.GetObjSize()-1));
		ptPosArr.Add(Obj1.GetPoint(Obj1.GetObjSize()-8));
		ptPosArr.Sort(FALSE,TRUE);
		for(n=0; n<ptPosArr.GetSize()-1; n++)
		{
			CDPoint ptPos1 = ptPosArr.GetAt(n);
			CDPoint ptPos2 = ptPosArr.GetAt(n+1);
			str = COMMA(ptPos2.y - ptPos1.y);
			Dom.DimLineToExtend(ptPos2.y - ptPos1.y, nDimDan, n%2 ? str : "", n%2 ? "" : str, TRUE);	dLenTotal += ptPos2.y - ptPos1.y; 
			dTLen += ptPos2.y - ptPos1.y;
		}
		dGap += pVr->m_dB*pVr->m_dC - dTLen;	
	}	
	Dom.DimLineTo(dGap, nDimDan,COMMA(dGap), TRUE);	dLenTotal += Round(dGap, 0);
	nDimDan = 1;
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_ddL1+pVr->m_ddL2+xyLD_VStiff.y-xyStt.y, nDimDan, COMMA(dLenTotal), TRUE);

	nDimDan = 0;
	Dom.SetDirection("BOTTOM");//거더하부치수선
	xyStt = (A[1]+A[2])/2 - vY*(dHGir+dTFD) - vX*dWGirLo/2;
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "", TRUE);
	Dom.DimLineTo(dSharp-dWStiffL, nDimDan, COMMA(dSharp-dWStiffL), TRUE);
	if(dTStiffL>0)
		Dom.DimLineTo(dWStiffL, nDimDan, COMMA(dWStiffL), TRUE);
	Dom.DimLineToExtend(dTW, nDimDan, "", COMMA(dTW), TRUE);
	Dom.DimLineTo(dWStiff-pVr->m_ddL3, nDimDan, COMMA(dWStiff-pVr->m_ddL3), TRUE);
	Dom.DimLineTo(pVr->m_ddE, nDimDan, COMMA(pVr->m_ddE), TRUE);
	Dom.DimLineTo(pVr->m_ddL3-pVr->m_ddE, nDimDan, COMMA(pVr->m_ddL3-pVr->m_ddE), TRUE);
	Dom.DimLineTo(pVr->m_ddL4, nDimDan, COMMA(pVr->m_ddL4));	

	//용접선1
	CDPoint vAxis1 = pVr->GetVectorFrame(CVBracing::LOWER);
	CDPoint xyWeld1 = pVr->GetCenterSplice(CVBracing::LD) + vAxis1*pVr->m_ddL4*0.8;
	Dom.WeldByValue(xyWeld1.x, xyWeld1.y, 6, 0, FALSE, TRUE); 
	*pDom << Dom;
	if(pVr->GetType()==TYPE_INVVBRACING)
	{
		CDPoint vAxis2 = pVr->GetVectorFrame(CVBracing::LEFT);
		CDPoint xyWeld2 = pVr->GetPointFrame(CVBracing::LD) + vAxis2*(pVr->m_dG[2]+pVr->m_dC1LG*0.8) + vAxis2.Rotate90()*dHBeamCen;
		Dom.WeldByValue(xyWeld2.x, xyWeld2.y, 6, 0, FALSE, FALSE); 
		*pDom << Dom;
	}
	//용접길이
	dGap = pVr->m_ddL4 - pVr->m_dDLG*vAxis1.x;
	CDPoint xyWeldStt = pVr->GetPointFrame(CVBracing::LD) + vAxis1*(pVr->m_dG[6]-dGap)/vAxis1.x + vY*(dHBeam-dHBeamCen);
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyWeldStt.x-pVr->m_ddL3, xyWeldStt.y-pVr->m_ddL2, 0);
	Dom.DimLineTo(pVr->m_ddL3, 0, COMMA(pVr->m_ddL3));
	Dom.DimLineToExtend(dGap, 0, "", COMMA(dGap)); 
	Dom.DimLineTo(pVr->m_dDLG, 0, COMMA(pVr->m_dDLG));
	*pDom << Dom;
	if(pVr->GetType()==TYPE_INVVBRACING)
	{
		CDPoint vAxis2 = pVr->GetVectorFrame(CVBracing::LEFT);
		xyWeldStt =  pVr->GetPointFrame(CVBracing::LD) + vAxis2*pVr->m_dG[2] + vAxis2.Rotate90()*dHBeamCen;
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyWeldStt.x, xyWeldStt.y, 0);
		Dom.DimLineTo(pVr->m_dC1LG, 0, COMMA(pVr->m_dC1LG));
		Dom.Rotate(xyWeldStt, vAxis2);
		*pDom << Dom;
	}
	//수평브레이싱단면
	Dom.SetDirection("RIGHT");
	CHBracingGusset* pBrGu = pBx->GetHBracingGusset();
	if(pBrGu->IsReal())
	{
		double dWHBr = pBrGu->m_dH[1];
		CDPoint xyHBr = (A[1]+A[2])/2 + vX*(dTW/2+dWHBr) - vY*dHGir;
		Dom.DimMoveTo(xyHBr.x, xyHBr.y, 0, "", TRUE);
		Dom.DimLineTo(dWHBr, 0, COMMA(dWHBr));
		Dom.DimLineTo(pBrGu->m_dT, 0, COMMA(pBrGu->m_dT));
		CDPoint xyHBrGisi = xyHBr+vY*(dWHBr+pBrGu->m_dT/2);
		Dom.SetDirection("TOP");
		Dom.CircleArrow(xyHBrGisi.x, xyHBrGisi.y, Dom.Always(20), Dom.GetTextWidth("수평브레이싱 참조"), 1, "수평브레이싱 참조", "");
		*pDom << Dom;
	}

}
void CAPlateDrawCross::DimVBracingDetailThree(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir    = pBridge->GetGirder(pBridge->GetGirdersu()-2);
	CPlateGirderApp* pGirE   = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	pBx = pBx->GetBxMatchByCrossBeam(pGir);	//끝에서 두번째 거더의 BX
	CPlateBasicIndex* pBxE = pBx->GetBxMatchByCrossBeam(pGirE);	//끝에서 두번째 거더의 BX
	CVStiff*         pVStiff = pBxE->GetSection()->GetVStiff();
	CVBracing*       pVr     = pBx->GetSection()->GetVBracing();
	double dSharp  = pGirE->GetLengthSharpOnBx(pBxE, TRUE); 
	double dHGir   = pGirE->GetHeightGirderByStaAng(pBxE->GetStation());
	double dWGir   = pGirE->GetWidthOnStation(pBxE->GetStation(), TRUE);
	double dWGirLo = pGirE->GetWidthOnStation(pBxE->GetStation(), FALSE);
	double dTW     = pGirE->GetThickFactByStation(pBxE->GetStation(), 2);
	double dWStiff  = pVStiff->m_dWidth[0];
	double dWStiffR = pVStiff->m_dWidth[1];
	double dTStiffR = pVStiff->m_dThick[1];
	double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_UP, TRUE, TRUE);
	double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_UP, CSection::CENTERDX);
	if(!pOpt->GetInstallBeamAtCenter())
		dHBeamCen = dHBeam/2;

	double dRatio = 0.4;
	long  nDimDan = 0;
	double dGap   = (pVr->m_duR1+pVr->m_duR2-(pVr->m_uB*pVr->m_uC))/2;
	CDPoint vUpperBr = pVr->GetVectorFrame(0);
	CDPoint vX(1,0), vY(0,1);
	CString str;

	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);

	CDomyun Dom(pDom);
	//거더우측치수선
	Dom.SetDirection("RIGHT");
	//수직보강재 우측모서리와 LT이음판의 상단 끝단이 교차하는 점
	CDPoint xyRT_VStiff = A[7]+vX*dWGir/2 - vY*pVr->m_dRH1 - vUpperBr*(dWStiff+dTW/2)/fabs(vUpperBr.x)
		                 + vY*(pVr->m_duR1-((dHBeam-dHBeamCen)/fabs(vUpperBr.x)));
	CDPoint vOrigin = A[7]+vX*dWGir-vY*(pVr->m_dRH1+pVr->m_duR2+dHBeam-dHBeamCen);
	//
	double dLenTotal = 0;
	long	n = 0;
	Dom.DimMoveTo(vOrigin.x, vOrigin.y, nDimDan, "", TRUE);
	Dom.DimLineTo(dGap, nDimDan, COMMA(dGap), TRUE);	dLenTotal+=Round(dGap, 0);
	CPlatePieceObject Obj1(pGir);
	pVr->CreatePieceObj(&Obj1, TRUE, TRUE, 2);
	if(Obj1.GetChildCount()>0 && Obj1.GetChild(0)->GetObjSize()==(pVr->m_uB+1))
	{	
		str.Format("%d@%d=%d", pVr->m_uB, pVr->m_uC, pVr->m_uB*pVr->m_uC);	dLenTotal+=pVr->m_uB*pVr->m_uC;
		Dom.DimLineToExtend(pVr->m_uB*pVr->m_uC, nDimDan, "", str, TRUE);
	}
	else
	{
		double dTLen=0;
		CDPointArray ptPosArr;
		for(n=0; n<Obj1.GetChild(0)->GetObjSize(); n++)
			ptPosArr.Add(Obj1.GetChild(0)->GetPoint(n));
		// Cut
		ptPosArr.Add(Obj1.GetPoint(Obj1.GetObjSize()-1));
		ptPosArr.Add(Obj1.GetPoint(Obj1.GetObjSize()-8));
		ptPosArr.Sort(FALSE,TRUE);
		for(n=0; n<ptPosArr.GetSize()-1; n++)
		{
			CDPoint ptPos1 = ptPosArr.GetAt(n);
			CDPoint ptPos2 = ptPosArr.GetAt(n+1);
			str = COMMA(ptPos2.y - ptPos1.y);
			Dom.DimLineToExtend(ptPos2.y - ptPos1.y, nDimDan, n%2 ? str : "", n%2 ? "" : str, TRUE);	dLenTotal+=(ptPos2.y - ptPos1.y);
			dTLen += ptPos2.y - ptPos1.y;
		}
		dGap += pVr->m_uB*pVr->m_uC - dTLen;
	}

	Dom.DimLineTo(dGap, nDimDan, COMMA(dGap), TRUE);	dLenTotal+=Round(dGap, 0);
	Dom.DimLineTo(A[7].y - xyRT_VStiff.y, nDimDan, COMMA(A[7].y - xyRT_VStiff.y), TRUE);	dLenTotal+=A[7].y - xyRT_VStiff.y;
	nDimDan = 1;
	Dom.DimMoveTo(vOrigin.x, vOrigin.y, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_duR1+pVr->m_duR2+A[7].y - xyRT_VStiff.y, nDimDan, COMMA(dLenTotal), TRUE);

	nDimDan = 0;
	Dom.SetDirection("BOTTOM");
	//거더하부치수선
	double dUpper = pVr==NULL ? dRatio*dHGir : pVr->m_dRH1+pVr->m_duR1+pVr->m_duR2;
	CDPoint xy = A[7] + vX*(dWGir/2-dTW/2-dWStiff-pVr->m_duR4) - vY*dUpper;
	Dom.DimMoveTo(xy.x, xy.y, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_duR4, nDimDan, COMMA(pVr->m_duR4), TRUE);
	Dom.DimLineTo(pVr->m_duR3-pVr->m_duE, nDimDan, COMMA(pVr->m_duR3-pVr->m_duE), TRUE);
	Dom.DimLineTo(pVr->m_duE, nDimDan, COMMA(pVr->m_duE), TRUE);
	Dom.DimLineTo(dWStiff-pVr->m_duR3, nDimDan, COMMA(dWStiff-pVr->m_duR3), TRUE);
	Dom.DimLineToExtend(dTW, nDimDan, "", COMMA(dTW), TRUE);
	if(dTStiffR>0)
	{
		Dom.DimLineTo(dWStiffR, nDimDan, COMMA(dWStiffR), TRUE);
		Dom.DimLineTo(dSharp-dWStiffR, nDimDan, COMMA(dSharp-dWStiffR), TRUE);
	}
	else
		Dom.DimLineTo(dWGirLo/2, nDimDan, COMMA(dWGirLo/2), TRUE);

	//수직보강재 제원
	CVector vJ = pGir->GetJewonTotal(V_ST, pBxE, 0, 0);//좌측수직보강재
	CString szJewon = pOptStd->GetJewonString(1,vJ.x,vJ.y,vJ.z);
	Dom.CircleArrow(A[7].x+dWStiff/2, A[7].y-dWStiff/2, dWStiff, dWStiff*2, 1, "수직보강재", szJewon);
	//용접선1
	CDPoint vAxis1 = pVr->GetVectorFrame(CVBracing::UPPER);
	CDPoint xyWeld1 = pVr->GetCenterSplice(CVBracing::RT) - vAxis1*pVr->m_duR4/2;
	Dom.WeldByValue(xyWeld1.x, xyWeld1.y, 6); 
	//용접선2
	CDPoint vAxis2 = pVr->GetVectorFrame(CVBracing::RIGHT);
	if(pVr->GetType()==TYPE_VBRACING)
	{
		CDPoint xyWeld2 = pVr->GetPointFrame(CVBracing::RT) + vAxis2*(pVr->m_dG[4]+pVr->m_dC2RG/2) - vAxis2.Rotate90()*dHBeamCen;
		Dom.WeldByValue(xyWeld2.x, xyWeld2.y, 6, FALSE); 
		*pDom << Dom;
	}
	//용접길이1
	dGap = pVr->m_duR4 - pVr->m_dURG*vAxis1.x;
	CDPoint xyWeldStt = pVr->GetPointFrame(CVBracing::RT) - vAxis1*(pVr->m_dG[1]+pVr->m_dURG)/vAxis1.x + vY*dHBeam/2;
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xyWeldStt.x, xyWeldStt.y, 0);
	Dom.DimLineTo(pVr->m_dURG, 0, COMMA(pVr->m_dURG));
	Dom.DimLineToExtend(dGap, 0, "", COMMA(dGap)); 
	Dom.DimLineTo(pVr->m_duR3, 0, COMMA(pVr->m_duR3)); 
	*pDom << Dom;
	//용접길이2
	if(pVr->GetType()==TYPE_VBRACING)
	{
		xyWeldStt =  pVr->GetPointFrame(CVBracing::RT) + vAxis2*pVr->m_dG[4] + vAxis2.Rotate90()*(dHBeam-dHBeamCen);
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyWeldStt.x, xyWeldStt.y, 0);
		Dom.DimLineTo(pVr->m_dC2RG, 0, COMMA(pVr->m_dC2RG));
		Dom.Rotate(xyWeldStt, vAxis2);
		*pDom << Dom;
	}
}
void CAPlateDrawCross::DimVBracingDetailFour(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir    = pBridge->GetGirder(pBridge->GetGirdersu()-2);
	CPlateGirderApp* pGirE   = pBridge->GetGirder(pBridge->GetGirdersu()-1);
	pBx = pBx->GetBxMatchByCrossBeam(pGir);	//끝에서 두번째 거더의 BX
	CPlateBasicIndex* pBxE = pBx->GetBxMatchByCrossBeam(pGirE);	//끝에서 두번째 거더의 BX
	CVStiff*         pVStiff = pBxE->GetSection()->GetVStiff();
	CVBracing*       pVr     = pBx->GetSection()->GetVBracing();
	double dSharp  = pGirE->GetLengthSharpOnBx(pBxE, TRUE); 
	double dHGir   = pGirE->GetHeightGirderByStaAng(pBxE->GetStation());
	double dWGir   = pGirE->GetWidthOnStation(pBxE->GetStation(), TRUE);
	double dWGirLo = pGirE->GetWidthOnStation(pBxE->GetStation(), FALSE);
	double dTW     = pGirE->GetThickFactByStation(pBxE->GetStation(), 2);
	double dTFD    = pGirE->GetThickFactByStation(pBxE->GetStation(), 1);
	double dWStiff  = pVStiff->m_dWidth[0];
	double dWStiffR = pVStiff->m_dWidth[1];
	double dTStiffR = pVStiff->m_dThick[1];
	double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);
	if(!pOpt->GetInstallBeamAtCenter())
		dHBeamCen = dHBeam/2;
	long  nDimDan = 0;
	double dGap  = (pVr->m_ddR1+pVr->m_ddR2-(pVr->m_dB*pVr->m_dC))/2;
	CDPoint vLowerBr = pVr->GetVectorFrame(1);
	CDPoint vX(1,0), vY(0,1);
	CString str;

	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);

	CDomyun Dom(pDom);
	//거더우측치수선
	Dom.SetDirection("RIGHT");

	CDPoint xyStt = A[7]+vX*dWGir - vY*dHGir;
	CDPoint xyRD_VStiff = A[7]+vX*dWGir/2 - vY*(dHGir-pVr->m_dLH2) - vLowerBr*(dWStiff+dTW/2)/fabs(vLowerBr.x)
		                - vY*(pVr->m_ddL2-((dHBeam-dHBeamCen)/fabs(vLowerBr.x)));
	double dLenTotal = 0;
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "", TRUE);
	Dom.DimLineTo(xyRD_VStiff.y-xyStt.y, nDimDan, COMMA(xyRD_VStiff.y-xyStt.y), TRUE);	dLenTotal += xyRD_VStiff.y-xyStt.y;
	Dom.DimLineTo(dGap, nDimDan, COMMA(dGap), TRUE);									dLenTotal += Round(dGap, 0);		
	CPlatePieceObject Obj1(pGir);
	pVr->CreatePieceObj(&Obj1, TRUE, TRUE, 3);
	long n = 0;
	if(Obj1.GetChildCount()>0 && Obj1.GetChild(0)->GetObjSize()==(pVr->m_dB+1))
	{	
		str.Format("%d@%d=%d", pVr->m_dB, pVr->m_dC, pVr->m_dB*pVr->m_dC);
		Dom.DimLineToExtend(pVr->m_dB*pVr->m_dC, nDimDan, "", str, TRUE);		dLenTotal += pVr->m_dB*pVr->m_dC;
	}
	else
	{
		double dTLen=0;
		CDPointArray ptPosArr;
		for(n=0; n<Obj1.GetChild(0)->GetObjSize(); n++)
			ptPosArr.Add(Obj1.GetChild(0)->GetPoint(n));
		// Cut
		ptPosArr.Add(Obj1.GetPoint(1));
		ptPosArr.Add(Obj1.GetPoint(12));
		ptPosArr.Sort(FALSE,TRUE);
		for(n=0; n<ptPosArr.GetSize()-1; n++)
		{
			CDPoint ptPos1 = ptPosArr.GetAt(n);
			CDPoint ptPos2 = ptPosArr.GetAt(n+1);
			str = COMMA(ptPos2.y - ptPos1.y);
			Dom.DimLineToExtend(ptPos2.y - ptPos1.y, nDimDan, n%2 ? str : "", n%2 ? "" : str, TRUE);	dLenTotal += ptPos2.y - ptPos1.y; 
			dTLen += ptPos2.y - ptPos1.y;
		}
		dGap += pVr->m_dB*pVr->m_dC - dTLen;
	}
	Dom.DimLineTo(dGap, nDimDan, COMMA(dGap), TRUE);	dLenTotal += Round(dGap, 0);
	nDimDan = 1;
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_ddR1+pVr->m_ddR2+xyRD_VStiff.y-xyStt.y, nDimDan, COMMA(dLenTotal), TRUE);
	//거더하부치수선
	nDimDan = 0;
	Dom.SetDirection("BOTTOM");
	CDPoint xy = A[7] + vX*(dWGir/2-dTW/2-dWStiff-pVr->m_ddR4) - vY*(dHGir+dTFD);
	Dom.DimMoveTo(xy.x, xy.y, nDimDan, "", TRUE);
	Dom.DimLineTo(pVr->m_ddR4, nDimDan, COMMA(pVr->m_ddR4), TRUE);
	Dom.DimLineTo(pVr->m_ddR3-pVr->m_ddE, nDimDan, COMMA(pVr->m_ddR3-pVr->m_ddE), TRUE);
	Dom.DimLineTo(pVr->m_ddE, nDimDan, COMMA(pVr->m_ddE), TRUE);
	Dom.DimLineTo(dWStiff-pVr->m_ddR3, nDimDan, COMMA(dWStiff-pVr->m_ddR3), TRUE);
	Dom.DimLineToExtend(dTW, nDimDan, "", COMMA(dTW), TRUE);
	if(dTStiffR>0)
	{
		Dom.DimLineTo(dWStiffR, nDimDan, COMMA(dWStiffR), TRUE);
		Dom.DimLineTo(dSharp-dWStiffR, nDimDan, COMMA(dSharp-dWStiffR), TRUE);
	}
	else
		Dom.DimLineTo(dWGirLo/2, nDimDan, COMMA(dWGirLo/2), TRUE);

	//용접선1
	CDPoint vAxis1 = pVr->GetVectorFrame(CVBracing::LOWER);
	CDPoint vAxis2 = pVr->GetVectorFrame(CVBracing::RIGHT);
	CDPoint xyWeld1 = pVr->GetCenterSplice(CVBracing::RD) - vAxis1*pVr->m_ddR4*0.8;
	Dom.WeldByValue(xyWeld1.x, xyWeld1.y, 6, 0); 
	*pDom << Dom;
	//용접선2
	if(pVr->GetType()==TYPE_INVVBRACING)
	{
		CDPoint xyWeld2 = pVr->GetPointFrame(CVBracing::RD) + vAxis2*(pVr->m_dG[4]+pVr->m_dC2RG*0.8) + vAxis2.RotateInv90()*(dHBeamCen);
		Dom.WeldByValue(xyWeld2.x, xyWeld2.y, 6, 0, TRUE, FALSE); 
		*pDom << Dom;
	}
	//용접길이
	dGap = pVr->m_ddR4 - pVr->m_dDRG*vAxis1.x;
	CDPoint xyWeldStt = pVr->GetPointFrame(CVBracing::RD) - vAxis1*(pVr->m_dG[6]+pVr->m_dDRG)/vAxis1.x - vY*dHBeamCen;
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyWeldStt.x, xyWeldStt.y, 0);
	Dom.DimLineTo(pVr->m_dDLG, 0, COMMA(pVr->m_dDRG));
	Dom.DimLineToExtend(dGap, 0, "", COMMA(dGap)); 
	Dom.DimLineTo(pVr->m_ddR3, 0, COMMA(pVr->m_ddR3)); 
	*pDom << Dom;
	//용접길이2
	if(pVr->GetType()==TYPE_INVVBRACING)
	{
		xyWeldStt = pVr->GetPointFrame(CVBracing::RD) + vAxis2*(pVr->m_dG[4]+pVr->m_dC2RG) + vAxis2.RotateInv90()*(dHBeam-dHBeamCen);
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(xyWeldStt.x, xyWeldStt.y, 0);
		Dom.DimLineTo(pVr->m_dC2RG, 0, COMMA(pVr->m_dC2RG));
		Dom.Rotate(xyWeldStt, -vAxis2);
		*pDom << Dom;
	}
	//수평브레이싱단면
	Dom.SetDirection("LEFT");
	CHBracingGusset* pBrGu = pBxE->GetHBracingGusset();
	if(pBrGu->IsReal())
	{
		double dWHBr = pBrGu->m_dH[0];
		CDPoint xyHBr = A[7] + vX*(dWGir/2-dTW/2-dWHBr) - vY*dHGir;
		Dom.DimMoveTo(xyHBr.x, xyHBr.y, 0, "", TRUE);
		Dom.DimLineTo(dWHBr, 0, COMMA(dWHBr));
		Dom.DimLineTo(pBrGu->m_dT, 0, COMMA(pBrGu->m_dT));
		CDPoint xyHBrGisi = xyHBr+vY*(dWHBr+pBrGu->m_dT/2);
		Dom.SetDirection("TOP");
		Dom.CircleArrow(xyHBrGisi.x, xyHBrGisi.y, Dom.Always(20),- Dom.GetTextWidth("수평브레이싱 참조"), 3, "수평브레이싱 참조", "");
		*pDom << Dom;
	}
}
void CAPlateDrawCross::DimVBracingDetailFive(CDomyun* pDom, CPlateBasicIndex* pBx)
{
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();
	CPlateBridgeApp	*pBridge		= m_pDataMng->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(0);
	CPlateGirderApp* pGirR = pBridge->GetGirder(1);
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);	
	CVBracing* pVr   = pBx->GetSection()->GetVBracing();
	CVStiff *pVStiff = pBx->GetSection()->GetVStiff();

	double dHBeam = pBx->GetSection()->GetBeamBaseHeight(CSection::SEC_DOWN, TRUE, TRUE);
	double dHBeamCen = pBx->GetSection()->GetBeamJewon(CSection::SEC_DOWN, CSection::CENTERDX);
	if(!pOpt->GetInstallBeamAtCenter())
		dHBeamCen = dHBeam/2;

	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);
	CDPoint vX(1,0), vY(0,1);
	CString str;
	CDomyun Dom(pDom);

	CDPoint vDir = pVr->GetVectorFrame(CVBracing::LOWER);
	CDPoint xyStt;
	if(pVr->GetType()==TYPE_VBRACING)
	{
		xyStt = pVr->GetPointFrame(CVBracing::CEN) - vX*(pVr->m_dc3) - vY*(dHBeamCen)/vDir.x;
		Dom.SetDirection("BOTTOM");
	}
	else
	{
		xyStt = pVr->GetPointFrame(CVBracing::CEN) - vX*(pVr->m_dc3) + vY*(dHBeamCen)/vDir.x;
		Dom.SetDirection("TOP");
	}
	
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "", TRUE);
	Dom.DimLineTo(pVr->m_dc3, 0, COMMA(pVr->m_dc3));
	Dom.DimLineTo(pVr->m_dc4, 0, COMMA(pVr->m_dc4));

	Dom.SetDirection("RIGHT");
	if(pVr->GetType()==TYPE_VBRACING)
		xyStt = pVr->GetPointFrame(CVBracing::CEN) + vY*(dHBeam-dHBeamCen-pVr->m_dc2) + vX*pVr->m_dc4;
	else
		xyStt = pVr->GetPointFrame(CVBracing::CEN) - vY*(dHBeam-dHBeamCen+pVr->m_dc2) + vX*pVr->m_dc4;
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "", TRUE);
	Dom.DimLineTo(pVr->m_dc2, 0, COMMA(pVr->m_dc2));
	Dom.DimLineTo(pVr->m_dc1, 0, COMMA(pVr->m_dc1));
	*pDom << Dom;
	//용접선1
	CDPoint vAxis1 = vY;
	CDPoint xyWeld1;
	if(pVr->GetType()==TYPE_VBRACING)
		xyWeld1 = pVr->GetCenterSplice(CVBracing::CEN) - vAxis1*pVr->m_dc2;
	else
		xyWeld1 = pVr->GetCenterSplice(CVBracing::CEN) + vAxis1*pVr->m_dc1;
	Dom.WeldByValue(xyWeld1.x, xyWeld1.y, 6); 
	//용접선2
	CDPoint vAxis2 = pVr->GetVectorFrame(CVBracing::LEFT);
	CDPoint xyWeld2 = pVr->GetPointFrame(CVBracing::CEN) - vAxis2*(pVr->m_dG[3]+pVr->m_dC1RG/2) + vAxis2.Rotate90()*dHBeamCen;
	Dom.WeldByValue(xyWeld2.x, xyWeld2.y, 6, FALSE); 
	*pDom << Dom;

	Dom.SetDirection("TOP");
	vAxis1 = pVr->GetVectorFrame(CVBracing::LEFT);
	xyStt = pVr->GetPointFrame(CVBracing::CEN) - vAxis1*(pVr->m_dG[3]+pVr->m_dC1RG) + vAxis1.Rotate90()*(dHBeam-dHBeamCen);
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineTo(pVr->m_dC1RG, 0, COMMA(pVr->m_dC1RG));
	Dom.Rotate(xyStt, vAxis1);
	*pDom << Dom;


}

void CAPlateDrawCross::DrawVBracingSpliceBoltLT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe, BOOL bDim)
{
	CARoadOptionStd *pOptStd  = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir     = pBx->GetGirder();
	CVBracing       *pVr      = pBx->GetSection()->GetVBracing();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLS);

	CPlatePieceObject pObj(pGir);
	pVr->CreatePieceBoltObjLTSplice(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	if(bDim)
	{
		Dom.SetDirection("LEFT");
		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		double dLowerGap = (pVr->m_duL1+pVr->m_duL2-(pVr->m_uB*pVr->m_uC))/2;
		CDPoint xyStt  = pVr->xyBoltPos[0];
		Dom.DimMoveTo(xyStt.x, xyStt.y-dLowerGap, 0, "", TRUE);
		Dom.DimLineToExtend(dLowerGap, 0, "", COMMA(dLowerGap), TRUE);
		Dom.DimLineTo(pVr->m_uB*pVr->m_uC, 0, "uB@uC", TRUE);
		Dom.DimLineToExtend(dLowerGap, 0,"",  COMMA(dLowerGap), TRUE);

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xyStt.x-pVr->m_duE, xyStt.y, 0, "", TRUE);
		Dom.DimLineTo(pVr->m_duE, 0, "uE", TRUE);
	}
	*pDom << Dom;

}
void CAPlateDrawCross::DrawVBracingSpliceBoltLD(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe, BOOL bDim)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CVBracing       *pVr     = pBx->GetSection()->GetVBracing();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLS);

	CPlatePieceObject pObj(pGir);
	pVr->CreatePieceBoltObjLDSplice(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);

	if(bDim)
	{
		Dom.SetDirection("LEFT");
		pOptStd->SetEnvType(&Dom,HCAD_DIML);
		double dLowerGap = (pVr->m_ddL1+pVr->m_ddL2-(pVr->m_dB*pVr->m_dC))/2;
		CDPoint xyStt  = pVr->xyBoltPos[1];
		Dom.DimMoveTo(xyStt.x, xyStt.y-dLowerGap, 0, "", TRUE);
		Dom.DimLineToExtend(dLowerGap, 0, "", COMMA(dLowerGap), TRUE);
		Dom.DimLineTo(pVr->m_dB*pVr->m_dC, 0, "dB@dC", TRUE);
		Dom.DimLineToExtend(dLowerGap, 0,"",  COMMA(dLowerGap), TRUE);

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(xyStt.x-pVr->m_ddE, xyStt.y, 0, "", TRUE);
		Dom.DimLineTo(pVr->m_ddE, 0, "dE", TRUE);
	}
	*pDom << Dom;
}
void CAPlateDrawCross::DrawVBracingSpliceBoltRT(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe, BOOL bDim)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CVBracing       *pVr     = pBx->GetSection()->GetVBracing();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLS);

	CPlatePieceObject pObj(pGir);
	pVr->CreatePieceBoltObjRTSplice(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
	*pDom << Dom;

}
void CAPlateDrawCross::DrawVBracingSpliceBoltRD(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bSangSe, BOOL bDim)
{
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CVBracing       * pVr    = pBx->GetSection()->GetVBracing();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLS);

	CPlatePieceObject pObj(pGir);
	pVr->CreatePieceBoltObjRDSplice(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
	*pDom << Dom;

}

void CAPlateDrawCross::BoltHole(CDomyun *pDom, CDPoint vAxis, CDPoint xyCen, double dRad, double dRatio)
{
	CDomyun Dom(pDom);

	Dom.Circle(xyCen, dRad, FALSE);
	Dom.MoveTo(xyCen-vAxis.Rotate90()*(dRad+dRad*dRatio));
	Dom.LineTo(xyCen+vAxis.Rotate90()*(dRad+dRad*dRatio));
	Dom.MoveTo(xyCen-vAxis*(dRad+dRad*dRatio));
	Dom.LineTo(xyCen+vAxis*(dRad+dRad*dRatio));

	*pDom << Dom;
 
}

void CAPlateDrawCross::DstDrawBoltHole(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
									double dD, long nE, double dF, double BoltSize, 
									double SlopStt, double SlopEnd, double dF1,	double RadRotate, 
									BOOL bLeft, BOOL bRight, long nBoltArrange, CDPoint xy)
{
	if(nB == 0 && nE == 0) return;
	if(SlopEnd!=SlopStt && dF1>0)	return;

	CDomyun *pDom = new CDomyun(pDomP);

	double x,y;
	double SlopInc = (nE>0 && nBoltArrange==0) ? (SlopEnd-SlopStt)/nE : 0; // 기울기 증가분
	double R = (BoltSize+3)/2;
	//////////////////////////////////
	// 좌측구멍
	long i,j;
	long nV = dF1>0 ? (nE+1)*2 : nE+1;
	double yPos;
	if(bLeft) {
		for(i=0; i<nB+1; i++) {
			x = -(nB*dC+dC1/2) + dC*i;
			yPos = -dD;
			for(j=0; j<nV; j++) {
				if(dF1>0 && j==(long)(nV/2)) 
					yPos -= dF1;
				else if(j>0)
					yPos -= dF;
				y = yPos + x*(SlopStt+SlopInc*j); 
				if(j==nE)  // 볼트 일반배치
					y = yPos + x*SlopEnd;
				pDom->Circle(x,y,R,TRUE);
			}
		}
	}
	//////////////////////////////////
	// 우측구멍
	if(bRight) {
		for(i=0; i<nB+1; i++) {
			x = dC1/2 + dC*i;
			yPos = -dD;
			for(j=0; j<nV; j++) {
				if(dF1>0 && j==(long)(nV/2)) 
					yPos -= dF1;
				else if(j>0)
					yPos -= dF;
				y = yPos + x*(SlopStt+SlopInc*j);
				if(j==nE)  // 볼트 일반배치
					y = yPos + x*SlopEnd;
				pDom->Circle(x,y,R,TRUE);
			}
		}
	}

	pDom->Rotate(RadRotate,0,0);
	pDom->Move(xy);
	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawCross::DstDrawBoltLine(CDomyun *pDomP, double dA, long nB, double dC, double dC1,
									double dD, long nE, double dF, double BoltSize, 
									double SlopStt, double SlopEnd, double dF1,	double RadRotate, 
									BOOL bLeft, BOOL bRight, long nBoltArrange, CDPoint xy)
{
	if(nB == 0 && nE == 0) return;
	if(SlopEnd!=SlopStt && dF1>0)	return;

	CDomyun *pDom = new CDomyun(pDomP);

	double xStt,xEnd,yStt,yEnd;
	double SlopInc = (nE>0 && nBoltArrange==0) ? (SlopEnd-SlopStt)/nE : 0; // 기울기 증가분
	//////////////////////////////////
	// 수평라인(좌) 
	long i;
	long nV = dF1>0 ? (nE+1)*2 : nE+1;
	double yPos; // 볼트중심의 위치
	if(bLeft) {
		xStt = -(nB*dC+dC1/2);
		xEnd = bRight ? 0 : -dC1/2;
		yPos =  -dD;
		for(i=0; i<nV; i++) {
			if(dF1>0 && i==(long)(nV/2)) 
				yPos -= dF1;
			else if(i>0)
				yPos -= dF;
			yStt = yPos + xStt*(SlopStt+SlopInc*i);
			yEnd = yPos + xEnd*(SlopStt+SlopInc*i);
			if(i==nE) { // 볼트 일반배치
				yStt = yPos + xStt*SlopEnd;
				yEnd = yPos + xEnd*SlopEnd;
			}
			pDom->LineTo(xStt, yStt, xEnd, yEnd);
		}
	}
	// 수평라인(우) 
	if(bRight) {
		xStt = bLeft ? 0 : dC1/2;
		xEnd = (nB*dC+dC1/2);
		yPos =  -dD;
		for(i=0; i<nV; i++) {
			if(dF1>0 && i==(long)(nV/2)) 
				yPos -= dF1;
			else if(i>0)
				yPos -= dF;
			yStt = yPos + xStt*(SlopStt+SlopInc*i);
			yEnd = yPos + xEnd*(SlopStt+SlopInc*i);
			if(i==nE) { // 볼트 일반배치
				yStt = yPos + xStt*SlopEnd;
				yEnd = yPos + xEnd*SlopEnd;
			}
			pDom->LineTo(xStt, yStt, xEnd, yEnd);
		}
	}
	//////////////////////////////////
	// 수직라인(좌) 
	if(bLeft) {
		xStt = -(nB*dC+dC1/2) - dC;
		for(i=0; i<nB+1; i++) {
			xStt += dC;
			yStt  = -dD + xStt*SlopStt;
			if(dF1>0)
				yEnd  = -dD -(nE*dF)*2 - dF1 + xStt*SlopEnd;
			else
				yEnd  = -dD -nE*dF + xStt*SlopEnd;
			pDom->LineTo(xStt, yStt, xStt, yEnd);
		}
	}
	// 수직라인(우) 
	if(bRight) {
		xStt = dC1/2 - dC;
		for(i=0; i<nB+1; i++) {
			xStt += dC;
			yStt  = -dD + xStt*SlopStt;
			if(dF1>0)
				yEnd  = -dD -(nE*dF)*2 - dF1 + xStt*SlopEnd;
			else
				yEnd  = -dD -nE*dF + xStt*SlopEnd;
			pDom->LineTo(xStt, yStt, xStt, yEnd);
		}
	}

	pDom->Rotate(RadRotate,0,0);
	pDom->Move(xy);
	*pDomP << *pDom;
	delete pDom;
}

void CAPlateDrawCross::DstDrawGassetSupPlate(CDomyun *pDom, CPlateBasicIndex *pBx, double Len, double Thick, BOOL bLeft)
{
	if (pBx == NULL) return;
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirC= pDB->GetGirder(-1);
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun	Dom(pDom);
	CPlatePieceObject pObj(pGir);
	pObj.m_nCol = bLeft ? 0 : 1;
	pObj.m_nMarkBuje = pBx->IsJijum() ? CP_BR_GA_FA : CG_BR_GA_FA;
	pC->CreatePieceObjPlate(&pObj);
	pObj.DrawPieceOutline(&Dom, pOptStd, TRUE);
	
	long nCol = bLeft?0:1;
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음
	
	int nGen = (int)pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_GEN);
	double dB = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DB);
	double uB = nGen==0 ? pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UB) : 0;
	Dom.Move(0,-uB);
	*pDom << Dom;

	pOptStd->SetEnvType(&Dom, HCAD_STLH);

	Dom.LineTo(-Thick/2,dB,-Thick/2,Len-uB);
	Dom.LineTo(Thick/2,dB,Thick/2,Len-uB);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.LineTo(-Thick/2,0,-Thick/2,dB);
	Dom.LineTo(-Thick/2,0,Thick/2,0);
	Dom.LineTo(Thick/2,0,Thick/2,dB);

	Dom.LineTo(-Thick/2,Len,-Thick/2,Len-uB);
	Dom.LineTo(-Thick/2,Len,Thick/2,Len);
	Dom.LineTo(Thick/2,Len,Thick/2,Len-uB);

	Dom.Move(0,-Len);
	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamSpliceTotal(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bShop, long nDimDan)
{
	if (pBx == NULL) return;
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	
	CDomyun Dom(pDom);
	CDPoint A[10];
	pC->GetCrossXyDom(A);

	Dom.SetScaleDim(pDom->GetScaleDim());
	if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
		return;
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		DimCRBeamSpliceLeft(&Dom, pBx, BX, BY, bShop, nDimDan);
		DimCRBeamSpliceRight(&Dom, pBx, BX, BY, bShop, nDimDan);
		DimCRBeamSpliceShear(&Dom, pBx, BX, BY, nDimDan, FALSE);
		DimCRBeamSpliceMoment(&Dom, pBx, BX, BY, nDimDan, TRUE);
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		BX = A[5].x; BY = A[5].y;
		DimCRBeamSpliceLeft(&Dom, pBx, BX, BY, bShop, nDimDan);
		DimCRBeamSpliceRight(&Dom, pBx, BX, BY, bShop, nDimDan);
		DimCRBeamSpliceShear(&Dom, pBx, BX, BY, nDimDan, FALSE);
		DimCRBeamSpliceMoment(&Dom, pBx, BX, BY, nDimDan, TRUE);
	}
	else
	{
		DimCRBeamSpliceLeft(&Dom, pBx, BX, BY, bShop, nDimDan);
		DimCRBeamSpliceRight(&Dom, pBx, BX, BY, bShop, nDimDan);
		DimCRBeamSpliceShear(&Dom, pBx, BX, BY, nDimDan, FALSE);
		DimCRBeamSpliceMoment(&Dom, pBx, BX, BY, nDimDan, TRUE);
	}
	
	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamSpliceMoment(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, long nDimDan, BOOL bTop)
{
	if (pBx == NULL) return;
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	double GiulCrossGasset = pGir->GetGiulPlateCrossGusset(pBx);
	double AA = pC->m_mA;
	double B = pC->m_mB;
	double C = pC->m_mC;
	double C1 = pC->m_mC1;
	double E = pC->m_mE;
	double F = pC->m_mF;
	double W = pC->m_mW;
	double H = pC->m_H_CR;

	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();

	if (W ==0) return;

	//상부 기준점 이음판 상단 중심 
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	bTop ? Dom.SetDirection("TOP") : Dom.SetDirection("BOTTOM");
	CDPoint xy;
	long DimDan = nDimDan;
	//모멘트 이음판 
	if (pC->m_mW)
	{
		xy.x = -AA -(B * C) -C1/2;
		bTop ? xy.y = (AA + (B * C) +C1/2)*GiulCrossGasset : xy.y = (AA + (B * C) +C1/2)*GiulCrossGasset - H;
		CString str1,str2;
		str1.Format("%g@%g",E,F);
		str2.Format("=%g",E*F);
		Dom.DimMoveTo(xy,DimDan,FALSE,COMMA(AA));
		Dom.DimLineTo(AA, xy.y,"");
	if (GapCutting)
	{
		if (B > 1)
			Dom.DimLineTo(B*C, xy.y,COMMA(B)+"@"+COMMA(C),"="+COMMA(B*C));
		else
			Dom.DimLineTo(C, xy.y,COMMA(C));

		Dom.DimLineTo(C1/2, xy.y, COMMA(C1/2));
		Dom.DimLineTo(GapCutting, xy.y, "", COMMA(GapCutting));
		Dom.DimLineTo(C1/2, xy.y, COMMA(C1/2));

		if (B > 1)
			Dom.DimLineTo(B*C, xy.y,COMMA(B)+"@"+COMMA(C),"="+COMMA(B*C));
		else
			Dom.DimLineTo(C, xy.y,COMMA(C));

		Dom.DimLineTo(AA, xy.y,COMMA(AA));
		DimDan++;
		Dom.DimMoveTo(xy,DimDan);
		Dom.DimLineTo(AA*2+(B*C)*2+C1+GapCutting,xy.y,COMMA(AA*2+(B*C)*2+C1+GapCutting));
	}
	else
	{
		if (C == C1)
			Dom.DimLineTo((B*2+1)*C, xy.y,COMMA(B*2+1)+"@"+COMMA(C),"="+COMMA((B*2+1)*C));
		else {
			if(B>1)
				Dom.DimLineTo(B*C,xy.y, str1,str2);
			else
				Dom.DimLineTo(B*C,xy.y,COMMA(B*C),"",FALSE);
			Dom.DimLineTo(C1,xy.y,COMMA(C1));

			if(B>1)
				Dom.DimLineTo(B*C,xy.y, str1,str2);
			else
				Dom.DimLineTo(B*C,xy.y,COMMA(B*C),"",FALSE);
		}

		Dom.DimLineTo(AA, xy.y,COMMA(AA),"",FALSE,TRUE);
		DimDan++;
		Dom.DimMoveTo(xy,DimDan);
		Dom.DimLineTo(AA*2+(B*C)*2+C1,xy.y,COMMA(AA*2+(B*C)*2+C1));
	}
	}

	Dom.Move(BX, BY);
	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamSpliceShear(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, long nDimDan, BOOL bTop)
{
	if (pBx == NULL) return;
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	double GiulCrossGasset = pGir->GetGiulPlateCrossGusset(pBx);
	double AA = pC->m_sA;
	double B = pC->m_sB;
	double C = pC->m_sC;
	double C1 = pC->m_sC1;
	double E = pC->m_sE;
	double F = pC->m_sF;
	double H = pC->m_H_CR;

	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();

	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	//상부 기준점 이음판 상단 중심 
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	bTop ? Dom.SetDirection("TOP") : Dom.SetDirection("BOTTOM");

	long DimDan = nDimDan;
	CDPoint xy;
	xy.x = -AA -(B * C) -C1/2;
	xy.y = bTop ? (AA + (B * C) +C1/2)*GiulCrossGasset : (AA + (B * C) +C1/2)*GiulCrossGasset - H;
	CString str1,str2;
	str1.Format("%g@%g",E,F);
	str2.Format("=%g",E*F);
	Dom.DimMoveTo(xy,DimDan,FALSE,COMMA(AA));
	Dom.DimLineTo(AA, xy.y,"");
	if (GapCutting)
	{
		if (B > 1)
			Dom.DimLineTo(B*C, xy.y,COMMA(B)+"@"+COMMA(C),"="+COMMA(B*C));
		else
			Dom.DimLineTo(C, xy.y,COMMA(C));

		Dom.DimLineTo(C1/2, xy.y, COMMA(C1/2));
		Dom.DimLineTo(GapCutting, xy.y, "", COMMA(GapCutting));
		Dom.DimLineTo(C1/2, xy.y, COMMA(C1/2));

		if (B > 1)
			Dom.DimLineTo(B*C, xy.y,COMMA(B)+"@"+COMMA(C),"="+COMMA(B*C));
		else
			Dom.DimLineTo(C, xy.y,COMMA(C));

		Dom.DimLineTo(AA, xy.y,COMMA(AA));
		DimDan++;
		Dom.DimMoveTo(xy,DimDan);
		Dom.DimLineTo(AA*2+(B*C)*2+C1+GapCutting,xy.y,COMMA(AA*2+(B*C)*2+C1+GapCutting));
	}
	else
	{
		if (C == C1)
			Dom.DimLineTo((B*2+1)*C, xy.y,COMMA(B*2+1)+"@"+COMMA(C),"="+COMMA((B*2+1)*C));
		else {
			if(B>1)
				Dom.DimLineTo(B*C,xy.y, str1,str2);
			else
				Dom.DimLineTo(B*C,xy.y,COMMA(B*C),"",FALSE);
			Dom.DimLineTo(C1,xy.y,COMMA(C1));

			if(B>1)
				Dom.DimLineTo(B*C,xy.y, str1,str2);
			else
				Dom.DimLineTo(B*C,xy.y,COMMA(B*C),"",FALSE);
		}

		Dom.DimLineTo(AA, xy.y,COMMA(AA),"",FALSE,TRUE);
		DimDan++;
		Dom.DimMoveTo(xy,DimDan);
		Dom.DimLineTo(AA*2+(B*C)*2+C1,xy.y,COMMA(AA*2+(B*C)*2+C1));
	}

	Dom.Move(BX, BY);
	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamSpliceRight(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bShop, long nDimDan)
{
	if (pBx == NULL) return;
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption   *pGlopt  = pDB->m_pDataManage->GetGlobalOption();

	double B = pC->m_mB;
	double C = pC->m_mC;
	double C1 = pC->m_mC1;
	double E = pC->m_mE;
	double F = pC->m_mF;
	double W = pC->m_mW;
	double sA = pC->m_sA;
	double sW = pC->m_sW;
	double sE = pC->m_sE;
	double sF = pC->m_sF;
	double sB = pC->m_sB;
	double sC = pC->m_sC;
	double sC1 = pC->m_sC1;
	double H = pC->m_H_CR;
	
	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.SetDirection("RIGHT");
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double CrossJSlop = pGir->GetGiulPlateCrossGusset(pBx);
	if(CrossJSlop!=0 && pGlopt->GetDesignBoltHoleArrangeCrossBeam() != 2 && bShop)
		Dom.SetDimObq(-ToDegree(atan(CrossJSlop)));
	else if(pGlopt->GetDesignBoltHoleArrangeCrossBeam() == 2 || !bShop)
		CrossJSlop = 0;

	long DimDan = nDimDan;
	CDPoint xy;
	double G = pC->m_mG;
	double dis2 = (H - pC->m_mW*2-sW-G*2)/2;
	double sdis = (sW - sE * sF)/2;
	//모멘트
	double dis = (W - (E * F))/2;
	xy = CDPoint(sA+sB*sC+sC1/2,-H);
	xy.y += xy.x * CrossJSlop;
	if (W)
	{
		xy = CDPoint(B*C+C1/2,-H);
		xy.y += xy.x * CrossJSlop;
		Dom.DimMoveTo(xy,DimDan, FALSE, COMMA(G));
		Dom.DimLineTo(xy.x, G,"");
		Dom.DimLineTo(xy.x, dis,COMMA(dis));
		Dom.DimLineTo(xy.x, E*F,COMMA(E)+"@"+COMMA(F),"="+COMMA(E*F));
		Dom.DimLineTo(xy.x, dis,COMMA(dis));
	}
	else
	{
		Dom.DimMoveTo(xy,DimDan);
		dis2 = (H - sW)/2;
	}

	Dom.DimLineTo(xy.x, dis2,"",COMMA(dis2));
	Dom.DimLineTo(xy.x, sdis,COMMA(sdis));
	Dom.DimLineTo(xy.x, sE*sF,COMMA(sE)+"@"+COMMA(sF),"="+COMMA(sE*sF));
	Dom.DimLineTo(xy.x, sdis,COMMA(sdis));
	Dom.DimLineTo(xy.x, dis2,"",COMMA(dis2));

	if (W)
	{
		Dom.DimLineTo(xy.x, dis,COMMA(dis));
		Dom.DimLineTo(xy.x, E*F,COMMA(E)+"@"+COMMA(F),"="+COMMA(E*F));
		Dom.DimLineTo(xy.x, dis,COMMA(dis));
		Dom.DimLineTo(xy.x, G,COMMA(G),"",FALSE,TRUE);
	}

	Dom.Move(BX, BY);
	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamSpliceLeft(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY, BOOL bShop, long nDimDan)
{
	if (pBx == NULL) return;
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = m_pDataMng->GetBridge();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption   *pGlopt  = pDB->m_pDataManage->GetGlobalOption();

	double B = pC->m_mB;
	double C = pC->m_mC;
	double C1 = pC->m_mC1;
	double E = pC->m_mE;
	double F = pC->m_mF;
	double W = pC->m_mW;
	double sA = pC->m_sA;
	double sW = pC->m_sW;
	double sB = pC->m_sB;
	double sC = pC->m_sC;
	double sC1 = pC->m_sC1;
	double H = pC->m_H_CR;
	
	CDimDomyunEx Dom(pDom);
	Dom.SetScaleDim(pDom->GetScaleDim());
	Dom.SetDirection("LEFT");
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	double CrossJSlop = pGir->GetGiulPlateCrossGusset(pBx);
	if(CrossJSlop!=0 && pGlopt->GetDesignBoltHoleArrangeCrossBeam() != 2 && bShop)
		Dom.SetDimObq(ToDegree(atan(CrossJSlop)));
	else if(pGlopt->GetDesignBoltHoleArrangeCrossBeam() == 2 || !bShop)
		CrossJSlop = 0;

	long DimDan = nDimDan;
	CDPoint xy;
	double G = pC->m_mG;
	double dis2 = (H - pC->m_mW*2-pC->m_sW-G*2)/2;
	//모멘트
	xy = CDPoint(-(sA+sB*sC+sC1/2),-H);
	xy.y += xy.x * CrossJSlop;
	if (W)
	{
		xy = CDPoint(-B*C-C1/2,-H);
		xy.y += xy.x * CrossJSlop;
		Dom.DimMoveTo(xy,DimDan, FALSE, COMMA(G));
		Dom.DimLineTo(xy.x, G,"");
		Dom.DimLineTo(xy.x, W,COMMA(W));
	}
	else
	{
		Dom.DimMoveTo(xy,DimDan);
		dis2 = (H - sW)/2;
	}

	Dom.DimLineTo(xy.x, dis2,"",COMMA(dis2));
	Dom.DimLineTo(xy.x, sW,COMMA(sW));
	Dom.DimLineTo(xy.x, dis2,"",COMMA(dis2));
	
	if (W)
	{
		Dom.DimLineTo(xy.x, W,COMMA(W));
		Dom.DimLineTo(xy.x, G,COMMA(G),"",FALSE,TRUE);
	}

	Dom.Move(BX, BY);
	*pDom << Dom;
}



void CAPlateDrawCross::DimCRBeamFrontTop(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY, double angDeg)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	  = pBx->GetGirder();
	CPlateCrossBeam	 *pC	  = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;

	double webT = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	CDPoint A[10], B[10];
 	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	//  수직보강재 텍스트, 슬래브 앵커 치수
	long nA = pC->m_nAnchorsu_CR;
	CString str = _T(""), str1 = _T(""), str2 = _T("");
	Dom.SetDirection("TOP");
	double disSum = 0;
	double GTerm  = pGir->GetDisFrontPlateWebToWeb(pBx);
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	CDPoint vAng = CDPoint(0,1);
	CDPoint vCross = (A[2]-A[0]).Unit();
	CDPoint vBrk   = (A[3]-A[1]).Unit();
	CDPoint xyStt;
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	if(!pC->m_nLGirSpType)	xyStt = A[8]+CDPoint(1,0)*webT+vCross*pC->GetLengthGirWebToCrWeb(TRUE)/vCross.x;
	else					xyStt = A[2]+vBrk*dWSp/2;
	double	dDistVStiff	= 0;
	for(long n=0; n<pC->m_nVStiffsu_CR;n++)
	{
		if(n==3) break;
		if(pC->m_nVStiffsu_CR>3)dDistVStiff += pC->m_VStiffDis_CR[n];
		else					dDistVStiff = pC->m_VStiffDis_CR[0];
	}
	// 수직보강재 TEXT
	if(pC->m_nVStiffsu_CR>0)
	{
		Dom.SetTextAngle(90);

		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
			Dom.TextOut(A[8].x+webT+pC->GetLengthGirWebToCrWeb(TRUE)+dDistVStiff-Dom.GetTextHeight(),(A[5].y+A[2].y)/2-pC->m_H_CR/2, "PL-vWxvTxvL");
		else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
			Dom.TextOut(A[2].x+dWSp/2+dDistVStiff-Dom.GetTextHeight(),(A[5].y+A[2].y)/2-pC->m_H_CR/2, "PL-vWxvTxvL");
		else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
			Dom.TextOut(A[8].x+webT+pC->GetLengthGirWebToCrWeb(TRUE)+dDistVStiff-Dom.GetTextHeight(),(A[5].y+A[2].y)/2-pC->m_H_CR/2, "PL-vWxvTxvL");
		else
			Dom.TextOut(A[2].x+dWSp/2+dDistVStiff-Dom.GetTextHeight(),(A[5].y+A[2].y)/2-pC->m_H_CR/2, "PL-vWxvTxvL");
	}

	double dLen		= 0;
	double dCross	= fabs(pC->GetVectorPlan().RotateInv(vGir).y);
	BOOL   bSkew	= pC->IsSkew();
	
	if(pC->m_nAnchorsu_CR>0)
	{
		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			Dom.DimMoveTo(A[8].x+webT,  max(B[2].y, B[7].y), 0,"ANCHOR BAR");

			dLen = pC->GetLengthGirWebToCrWeb(TRUE);
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			for(long n=0; n<nA; n++) 
			{
				str.Format("A%d",n+1);
				disSum += pC->m_AnchorDis_CR[n];
				dLen = pC->m_AnchorDis_CR[n];
				if(bSkew)	str2.Format("%s (%s)", COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str,str2);
			}

			dLen = GTerm-disSum-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE));
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			dLen = pC->GetLengthGirWebToCrWeb(FALSE);
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
		else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			Dom.DimMoveTo(A[8].x+webT,  max(B[2].y, B[7].y), 0,"ANCHOR BAR");

			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			for(long n=0; n<nA; n++) 
			{
				str.Format("A%d",n+1);
				disSum += pC->m_AnchorDis_CR[n];
				dLen = pC->m_AnchorDis_CR[n];
				if(bSkew)	str2.Format("%s (%s)", COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str,str2);
			}

			dLen = GTerm-disSum-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE);
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			dLen = pC->GetLengthGirWebToCrWeb(FALSE);
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
		else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
		{
			Dom.DimMoveTo(A[8].x+webT,  max(B[2].y, B[7].y), 0,"ANCHOR BAR");

			dLen = pC->GetLengthGirWebToCrWeb(TRUE);
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			for(long n=0; n<nA; n++) 
			{
				str.Format("A%d",n+1);
				disSum += pC->m_AnchorDis_CR[n];
				dLen = pC->m_AnchorDis_CR[n];
				if(bSkew)	str2.Format("%s (%s)", COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str,str2);
			}

			dLen = GTerm-disSum-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(TRUE);
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
		else
		{
			Dom.DimMoveTo(A[8].x+webT,  max(B[2].y, B[7].y), 0,"ANCHOR BAR");

			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			for(long n=0; n<nA; n++) 
			{
				str.Format("A%d",n+1);
				disSum += pC->m_AnchorDis_CR[n];
				dLen = pC->m_AnchorDis_CR[n];
				if(bSkew)	str2.Format("%s (%s)", COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str,str2);
			}

			dLen = GTerm-disSum-pC->m_P_CR*2-dWSp;
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s", COMMA(dLen));
			if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
	}
	
	// 수평보강재
	Dom.SetTextAngle(0);
	Dom.SetTextAlignHorz(TA_LEFT);
	if(pC->m_nHStiffUppersu_CR)
	{
		Dom.TextOut((A[2].x+A[5].x)/2 + Dom.GetTextHeight(), A[2].y-pC->m_H_CR*0.14+pC->m_hT_CR,"PL-hW x hT");
	}

	// 모멘트이음판
	CDPoint xyBaseMo;
	if(pC->m_nLGirSpType==FALSE && pC->m_nRGirSpType)	//우측에만 이음판이 있는 경우
		xyBaseMo = A[5];
	else
		xyBaseMo = A[2];

	if((pC->m_mB && pC->m_mC) && (pC->m_nLGirSpType || pC->m_nRGirSpType))
	{
		Dom.DimMoveTo(xyBaseMo.x-(pC->m_uC1/2+pC->m_uB*pC->m_uC+pC->m_uA),
						xyBaseMo.y,0,"mA");
		Dom.DimLineTo(pC->m_mA,0,"");
		Dom.DimLineToExtend(pC->m_mB*pC->m_mC,0,"","mB@mC");
		Dom.DimLineTo(pC->m_mC1,0,"mC1");
	}

	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamFrontBottom(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY, double angDeg)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();
	CPlateGirderApp	 *pGirR  = pGir->m_pRight;
	CPlateCrossBeam	 *pC	 = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateCrossBeam  *pCR    = pBxSec->GetSection()->GetCrossBeam();	
	if(pCR == NULL)	return;
	
	CDPoint A[10], B[10], vGir(0,0);
 	pC->GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	double dHGir     = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	// 복부이음판의 치수
	double Len = pC->m_sC1/2+pC->m_sB*pC->m_sC+pC->m_sA; // 이음판의 1/2
	CDPoint dp;
	if(pC->m_nLGirSpType)	dp = CDPoint(A[2].x-Len, A[2].y-pC->m_H_CR);
	else					dp = CDPoint(A[5].x-Len, A[5].y-pC->m_H_CR);
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dp.x,dp.y,0,"sA");
		Dom.DimLineTo(pC->m_sA,0,"");
		Dom.DimLineToExtend(pC->m_sB*pC->m_sC,0,"","sB@sC");
		Dom.DimLineTo(pC->m_sC1,0,"sC1");
	}
	// 가로보 상면 치수 
	long nCol = 0;	
	if(!pC->m_nLGirSpType)
	{
		dp.x = A[0].x-pC->m_dbL4;
		dp.y = A[0].y;

		Dom.SetDirection("TOP");

		Dom.DimMoveTo(dp.x, dp.y ,nCol);
		Dom.DimLineTo(pC->m_dbL4, nCol, "bL4");
	}
	if(!pC->m_nRGirSpType)
	{
		dp.x = A[7].x;
		dp.y = A[7].y;

		Dom.SetDirection("TOP");

		Dom.DimMoveTo(dp.x, dp.y ,nCol);
		Dom.DimLineTo(pC->m_dbR4, nCol, "bR4");
	}

	// 가로보 하면 치수 (sharp,이음면,가로보 주플레이트)
	if(B[2].y>B[7].y) dp.y = B[7].y-dHGir;
	else			  dp.y = B[2].y-dHGir;

	if(!pC->m_nLGirSpType)
	{
		dp.x = A[2].x-pC->m_P_CR;
		Dom.SetDirection("BOTTOM");

		Dom.DimMoveTo(dp.x, dp.y ,nCol, Comma(pC->GetLengthGirWebToCrWeb(TRUE)));
		Dom.DimLineTo(pC->GetLengthGirWebToCrWeb(TRUE), nCol, "");
		Dom.DimLineTo(pC->m_dbL1, nCol, "bL1");
		Dom.DimLineToExtend(pC->m_dbL2, nCol, "", "bL2");
		Dom.DimLineToOver(pC->m_dbL3, nCol, "bL3");
	}
	if(!pC->m_nRGirSpType)
	{
		dp.x = A[5].x+pC->m_P_CR-pC->m_dbR1-pC->m_dbR2-pC->m_dbR3-pC->GetLengthGirWebToCrWeb(FALSE);
		Dom.SetDirection("BOTTOM");
		
		Dom.DimMoveTo(dp.x, dp.y, nCol, "bR3");
		Dom.DimLineTo(pC->m_dbR3, nCol, "");
		Dom.DimLineTo(pC->m_dbR2, nCol, "bR2");
		Dom.DimLineToExtend(pC->m_dbR1, nCol, "", "bR1");
		Dom.DimLineToOver(pC->GetLengthGirWebToCrWeb(FALSE), nCol, Comma(pC->GetLengthGirWebToCrWeb(FALSE)));
	}

	double dThickLower   = pGir->GetThickFactByStation(pBx->GetStation(), 1);
	double dThickWeb     = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dHeightGirN   = pGirR->GetHeightGirderByStaAng(pBxSec->GetStation());
	double dWidthGirN    = pGirR->GetWidthOnStation(pBxSec->GetStation(), TRUE, TRUE);
	double dThickLowerN  = pGirR->GetThickFactByStation(pBxSec->GetStation(), 1);
	double dThickWebN    = pGirR->GetThickFactByStation(pBxSec->GetStation(), 2);

	if(!pC->m_nLGirSpType || !pC->m_nRGirSpType)
		nCol++;
	//좌측 수직보강재 폭 *****************************************
	CDPoint vX(1, 0), vY(0, 1);
	CDPoint xyStt = (B[1]+B[2])/2-vY*(dHGir+dThickLower)+vX*(dThickWeb/2);
	if(pC->m_vW1_G>0 && pBridge->m_nInstallVStiffOnCrossBeam>0 && pBx->GetNumberGirder()==0)
	{
		Dom.DimMoveTo(xyStt.x-pC->m_vW1_G-dThickWeb, xyStt.y, nCol);
		Dom.DimLineTo(pC->m_vW1_G, nCol, "vW1G");
	}
//	if(pC->m_nLGirSpType==FALSE || pC->m_nType==2)	// 이음판이 없거나 수직보강재 연결타입일 때 
	if(pC->m_cGasset_CR==FALSE)						// 브라켓보강판이 있으면....
	{
		Dom.DimMoveTo(xyStt.x, xyStt.y, nCol);
		Dom.DimLineTo(pC->m_vW_G, nCol, "vWG");
		Dom.CircleArrow(xyStt.x+100, xyStt.y+100, Dom.Always(10), Dom.Always(10), 7, "vTG", "");
	}
	//우측 수직보강재 폭 *****************************************
	xyStt = B[7]+vX*(dWidthGirN/2)-vY*(dHeightGirN+dThickLowerN)-vX*(dThickWebN/2+pC->m_vW_G);
	if(pC->m_vW1_G>0 && pBridge->m_nInstallVStiffOnCrossBeam>0 && pBx->GetNumberGirder()==pBridge->GetGirdersu()-2)
	{
		Dom.DimMoveTo(xyStt.x+(pC->m_vW_G+dThickWeb), xyStt.y, nCol);
		Dom.DimLineTo(pC->m_vW1_G, nCol, "vW1G");
	}
//	if(pC->m_nRGirSpType==FALSE || pC->m_nType==2)	// 이음판이 없거나 수직보강재 연결타입일 때 
	if(pC->m_cGasset_CR==FALSE)						// 브라켓보강판이 있으면....
	{
		Dom.DimMoveTo(xyStt.x, xyStt.y, nCol);
		Dom.DimLineTo(pC->m_vW_G, nCol, "vWG");
	}
	//************************************************************
	if(pC->m_cGasset_CR || (pC->m_nLGirSpType || pC->m_nRGirSpType))
	{
		CString str1 = _T(""), str2 = _T("");
		double dCross	= fabs(pC->GetVectorPlan().RotateInv(vGir).y);
		BOOL   bSkew	= pC->IsSkew();
		double dLen = 0;

		nCol++;
		dp.x = A[2].x-pC->m_P_CR;
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dp.x, dp.y ,nCol);

		dLen = pC->m_P_CR;
		if(bSkew)	str1.Format("%s (%s)", COMMA(dLen), COMMA(dLen/dCross));
		else		str1.Format("%s", COMMA(dLen));

		dLen = A[5].x-A[2].x;
		if(bSkew)	str2.Format("(%s)", COMMA(dLen/dCross));
		else		str2.Format("", COMMA(dLen));
		
		Dom.DimLineToExtend(pC->m_P_CR,nCol, "P", str1);
		Dom.DimLineToExtend(A[5].x-A[2].x,nCol,COMMA(A[5].x-A[2].x), str2);
		Dom.DimLineToExtend(pC->m_P_CR,nCol, "P", str1);
	}

	*pDom << Dom;
}

// 입력화면 치수선 처리
// 정면도
void CAPlateDrawCross::DimCRBeamFrontLeft(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY, double angDeg)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateCrossBeam	 *pC	 = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;
	
	CDPoint A[10], B[10];
 	pC->GetCrossXyDom(A);
 	pGir->GetHunchXyDom(pBx, B);
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	// 복부이음판의 치수
	CDPoint dp; 
	if(pC->m_nLGirSpType)
		dp = CDPoint(A[2].x, A[2].y-pC->m_H_CR);	
	else
		dp = CDPoint(A[5].x, A[5].y-pC->m_H_CR);	

	Dom.SetDirection("LEFT");
	double Tip =0, sta, gH, gsH;
	if(pC->m_mW>0 && pC->m_mA !=0)
		Tip = ( pC->m_H_CR - (pC->m_mG*2+pC->m_mW*2+pC->m_sW) ) / 2;
	else
		Tip = (pC->m_H_CR - pC->m_sW) / 2;
	// 1단 : 복부이음판 
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		if(pC->m_nLGirSpType || (pC->m_nLGirSpType && pC->m_nRGirSpType))
		{
			dp.x = A[2].x-(pC->m_sC1/2+pC->m_sB*pC->m_sC+pC->m_sA);
			
			if(pC->m_mW>0 && pC->m_mA !=0) {
				Dom.DimMoveTo(dp.x,dp.y,0,"mG");
				Dom.DimLineToExtend(pC->m_mG,0,"","");
				Dom.DimLineToExtend(pC->m_mW,0,"mW",COMMA(pC->m_mW));
			}
			else
				Dom.DimMoveTo(dp.x,dp.y,0,COMMA(Tip));

			Dom.DimLineTo(Tip,0,"");
			Dom.DimLineToExtend(pC->m_sW,0,"sW",COMMA(pC->m_sW));
			if(pC->m_mW>0 && pC->m_mA !=0) {
				Dom.DimLineTo(Tip,0,COMMA(Tip));
				Dom.DimLineToExtend(pC->m_mW,0,"mW",COMMA(pC->m_mW));
				Dom.DimLineToOverExtend(pC->m_mG,0,"mG",COMMA(pC->m_mG));
			}
			else
				Dom.DimLineToOver(Tip,0,COMMA(Tip));

			sta  = pBx->GetStation();
			gH	= pGir->GetHeightGirderByStaAng(sta);	// DOM이면 각도는 무의미...
			gsH	= gH - (pC->m_T1_CR+pC->m_H_CR+pC->m_dLH1+pC->m_T1_CR);
			dp.x = A[8].x;
			dp.y = B[1].y-gH;
		}
		else
		{
			dp.x = A[5].x-(pC->m_sC1/2+pC->m_sB*pC->m_sC+pC->m_sA);
			
			if(pC->m_mW>0 && pC->m_mA !=0) {
				Dom.DimMoveTo(dp.x,dp.y,0,"mG");
				Dom.DimLineToExtend(pC->m_mG,0,"","");
				Dom.DimLineToExtend(pC->m_mW,0,"mW",COMMA(pC->m_mW));
			}
			else
				Dom.DimMoveTo(dp.x,dp.y,0,COMMA(Tip));

			Dom.DimLineTo(Tip,0,"");
			Dom.DimLineToExtend(pC->m_sW,0,"sW",COMMA(pC->m_sW));
			if(pC->m_mW>0 && pC->m_mA !=0) {
				Dom.DimLineTo(Tip,0,COMMA(Tip));
				Dom.DimLineToExtend(pC->m_mW,0,"mW",COMMA(pC->m_mW));
				Dom.DimLineToOverExtend(pC->m_mG,0,"mG",COMMA(pC->m_mG));
			}
			else
				Dom.DimLineToOver(Tip,0,COMMA(Tip));

			sta  = pBx->GetStation();
			gH	= pGir->GetHeightGirderByStaAng(sta);	// DOM이면 각도는 무의미...
			gsH	= gH - (pC->m_T1_CR+pC->m_H_CR+pC->m_dLH1+pC->m_T1_CR);
			dp.x = A[8].x;
			dp.y = B[1].y-gH;
		}		
	}
	sta  = pBx->GetStation();
	gH	= pGir->GetHeightGirderByStaAng(sta);	// DOM이면 각도는 무의미...
	gsH	= gH - (pC->m_T1_CR+pC->m_H_CR+pC->m_dLH1+(pC->m_dLH1==0 ? 0 : pC->m_T2_CR));
	// 2단 : 가로보 제원
	long nCol = 0;
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) || (pC->m_nLGirSpType && !pC->m_nRGirSpType))
	{
		dp.x = B[1].x;
		dp.y = B[1].y-pGir->GetHeightGirderByStaAng(pBx->GetStation());
		if(pC->m_H_CR == gH)
		{
			Dom.DimMoveTo(dp.x, dp.y - pC->m_T1_CR, nCol);
		}
		else
		{
			Dom.DimMoveTo(dp.x,dp.y ,nCol);
			Dom.DimLineTo(gsH,nCol,COMMA(gsH));	
		}
		Dom.DimLineToExtend(pC->m_T1_CR, nCol,"T1",COMMA(pC->m_T1_CR));
		Dom.DimLineToExtend(pC->m_H_CR, nCol,"H", COMMA(pC->m_H_CR));
		if(pC->m_dLH1>0)
		{
			Dom.DimLineToExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dLH1, nCol, "");
			Dom.DimLineToExtend(Dom.Always(10), nCol, "LH", COMMA(pC->m_dLH1), FALSE);
		}
		else
			Dom.DimLineToOverExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));
	}
	else 
	{
		CDPoint vDir = pC->GetVectorCross();
		double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
		double dLowerSp = dHGir - pC->m_dLH1 - pC->m_T1_CR - (pC->m_dLH1==0 ? 0 : pC->m_T2_CR) - pC->m_H_CR;
		dp.x = B[1].x;
		dp.y = B[1].y-dHGir;
		if(pC->m_H_CR == gH)
		{
			Dom.DimMoveTo(dp.x, dp.y - pC->m_T1_CR, nCol);
		}
		else
		{
			Dom.DimMoveTo(dp.x, dp.y ,nCol);
			Dom.DimLineTo(dLowerSp, nCol, COMMA(dLowerSp));
		}
		Dom.DimLineToExtend(pC->m_T1_CR,nCol,"T1", COMMA(pC->m_T1_CR));
		Dom.DimLineToExtend(pC->m_H_CR, nCol,"H",  COMMA(pC->m_H_CR));
		if(pC->m_dLH1>0)
		{
			Dom.DimLineToExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dLH1, nCol, "");
			Dom.DimLineToExtend(Dom.Always(10), nCol, "LH", COMMA(pC->m_dLH1), FALSE);
		}
		else
			Dom.DimLineToOverExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));

	}
	//H형강타입 볼트홀
	if(!pC->m_nRGirSpType && pC->m_nLGirSpType)
	{
		double dLowerSpace = (pC->m_H_CR-pC->m_dwH-pC->m_wE*pC->m_wF)/2;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(pC->xyBoltPos[2].x, pC->xyBoltPos[2].y-dLowerSpace, 0, "", TRUE);
		Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
		Dom.DimLineToExtend(pC->m_wE*pC->m_wF, 0, "wE@wF", COMMA(pC->m_wE*pC->m_wF));
		Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
	}

	*pDom << Dom;
}

// 도장상세도 치수 표기...
void CAPlateDrawCross::DimCRBeamFrontPaint(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();	
	CPlateCrossBeam	 *pC	 = pBx->GetSection()->GetCrossBeam();
	CPlateGirderApp	 *pGirR  = pGir->m_pRight;	
	if(pC == NULL)	return;	

	CDPoint A[10], B[10];
	CDPoint dp;
 	pC->GetCrossXyDom(A);
 	pGir->GetHunchXyDom(pBx, B);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	
	//Left
	double sta  = pBx->GetStation();
	double gH	= pGir->GetHeightGirderByStaAng(sta);	// DOM이면 각도는 무의미...
	double dThick = pGir->GetThickFactByStation(sta, G_W);
	double gsH	= gH - (pC->m_T1_CR+pC->m_H_CR+pC->m_dLH1+(pC->m_dLH1==0 ? 0 : pC->m_T2_CR));
		
	Dom.SetDirection("LEFT");	
	dp.x = B[1].x;
	dp.y = B[1].y-gH;
	Dom.DimMoveTo(dp.x,dp.y ,0);
	Dom.DimLineTo(gsH+pC->m_T1_CR,0,"GH2");		
	Dom.DimLineTo(pC->m_H_CR, 0,"GH1");		

	if(pC->m_nLGirSpType)
	{
		double  sX	 = 0;
		double  sY	 = 0;
		double dGW1  = 0;

		Dom.SetDirection("TOP");
		if(pC->m_bConnectionPlate)
		{
			sX = A[2].x - (pC->m_uA+pC->m_uB1*pC->m_uC+(pC->m_uC1/2));			
			dGW1 = pC->m_uA*2+pC->m_uB1*pC->m_uC+pC->m_uC1+pC->m_uB*pC->m_uC;
		}
		else
		{
			sX = A[2].x - (pC->m_uA+pC->m_uB*pC->m_uC+pC->m_uC1/2);			
			dGW1 = (pC->m_uA+pC->m_uB*pC->m_uC+pC->m_uC1/2)*2;			
		}
		sY = A[2].y;
		Dom.DimMoveTo(sX, sY, 0);
		Dom.DimLineTo(dGW1,0,"GW1");
	}

	if(pC->m_cGasset_CR==FALSE)						// 브라켓보강판이 없으면....
	{
		Dom.DimMoveTo(A[5].x, A[5].y, 0);
		Dom.DimLineTo(pC->m_vW_G, 0, "GW2");
	}

	//BOTTOM
	Dom.SetDirection("BOTTOM");
	if(pC->m_cGasset_CR || (pC->m_nLGirSpType || pC->m_nRGirSpType))
	{		
		
		dp.x = A[8].x+pC->m_vW_G + dThick;
		double dL = (A[9].x - pC->m_vW_G - dThick) - dp.x;
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dp.x, dp.y ,0);		
		Dom.DimLineTo(dL,0,"L");		
	}


	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamFrontRight(CDomyun *pDom, CPlateBasicIndex *pBx,	double BX, double BY, double angDeg)
{
	CARoadOptionStd  *pOptStd = m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	  = pBx->GetGirder();
	CPlateBridgeApp  *pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	 *pGirR   = pGir->m_pRight;
	CPlateCrossBeam	 *pC	  = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex *pBxR    = pBx->GetBxMatchByCrossBeam(pGirR);
	
	double dWidthGirN = pGir->GetWidthOnStation(pBxR->GetStation(), TRUE);
	if(pC == NULL)	return;

	CDPoint A[10], B[10], dp;
	double 	gH	= pGirR->GetHeightGirderByStaAng(pBxR->GetStation());	// DOM이면 각도는 무의미...
	double gsH	= gH - (pC->m_T1_CR+pC->m_H_CR+pC->m_dRH1+(pC->m_dRH1==0 ? 0 : pC->m_T2_CR));
	double dWSp = (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	double Tip;
 	pC->GetCrossXyDom(A);
 	pGir->GetHunchXyDom(pBx, B);
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	// 복부이음판의 치수
	Dom.SetDirection("RIGHT");
	// 1단 : 복부이음판 
	// 모멘트 이음판
	if((pC->m_mW>0 && pC->m_mF>0) && (pC->m_nLGirSpType || pC->m_nRGirSpType))	
	{
		if(pC->m_nLGirSpType || (pC->m_nLGirSpType && pC->m_nRGirSpType))
			dp = CDPoint(A[3].x, A[2].y-pC->m_H_CR);
		else
			dp = CDPoint(A[6].x, A[5].y-pC->m_H_CR);

		Tip = ( pC->m_H_CR - (pC->m_mD*2+pC->m_mE*pC->m_mF*2+pC->m_sE*pC->m_sF) ) / 2;

		Dom.DimMoveTo( dp.x,dp.y,0, "mD" );
		Dom.DimLineToExtend(pC->m_mD,0,"","");
		Dom.DimLineToExtend(pC->m_mE*pC->m_mF,0,"mE@mF","");
		Dom.DimLineToExtend(Tip,0,"",COMMA(Tip));
		Dom.DimLineToExtend(pC->m_sE*pC->m_sF,0,"sE@sF",COMMA(pC->m_sE*pC->m_sF));
		Dom.DimLineToExtend(Tip,0,"",COMMA(Tip));
		Dom.DimLineToExtend(pC->m_mE*pC->m_mF,0,"mE@mF","");
		Dom.DimLineToOver(pC->m_mD,0,"mD");
	}
	// 전단이음판	
	else if(pC->m_nLGirSpType || pC->m_nRGirSpType)		
	{
		if(pC->m_nLGirSpType || (pC->m_nLGirSpType && pC->m_nRGirSpType))
			dp = CDPoint(A[2].x+dWSp/2, A[2].y-pC->m_H_CR);
		else
			dp = CDPoint(A[5].x+dWSp/2, A[5].y-pC->m_H_CR);
	
		Tip = (pC->m_H_CR - pC->m_sE*pC->m_sF) / 2;
		
		Dom.DimMoveTo(dp.x,dp.y,0,COMMA(Tip));
		Dom.DimLineToExtend(Tip,0,"","");
		Dom.DimLineToExtend(pC->m_sE*pC->m_sF,0,"sE@sF",COMMA(pC->m_sE*pC->m_sF));
		Dom.DimLineToOver(Tip,0,COMMA(Tip));
	}
	CDPoint xy;
	if(pC->m_nLGirSpType || pC->m_nRGirSpType)
	{
		if(pC->m_nLGirSpType || (pC->m_nLGirSpType && pC->m_nRGirSpType))
			xy = A[2];
		else
			xy = A[5];
		dp.x = xy.x-(pC->m_dC1/2+pC->m_dB*pC->m_dC+pC->m_dA)/2;
		dp.y = xy.y-pC->m_H_CR-pGir->GetThickJewon(G_F_L,pBx)-Dom.Always(1);
		Dom.TextOut(dp.x,dp.y-Dom.GetTextHeight(),"dT");
		//
		dp.x = xy.x-(pC->m_uC1/2+pC->m_uB*pC->m_uC+pC->m_uA)/2;
		dp.y = xy.y+pGir->GetThickJewon(G_F_U,pBx)+pC->m_uT+Dom.Always(1);
		Dom.TextOut(dp.x,dp.y,"uT");

		dp = CDPoint(xy.x-(pC->m_sA+pC->m_sB*pC->m_sC+pC->m_sC1/2), 
					 xy.y-pC->m_H_CR/2);
		Dom.TextOut(dp.x-Dom.GetTextHeight(),dp.y,"sT");
		//
		dp = CDPoint(xy.x-(pC->m_mA+pC->m_mB*pC->m_mC+pC->m_mC1/2), 
					 xy.y-pC->m_H_CR+pC->m_mW/2);
		if(pC->m_mB && pC->m_mC)
			Dom.TextOut(dp.x-Dom.GetTextHeight(),dp.y,"mT");
	}
	long nCol = 0;
	if((pC->m_nLGirSpType && pC->m_nRGirSpType) || (!pC->m_nLGirSpType && pC->m_nRGirSpType))
	{
		dp.x = B[7].x+dWidthGirN;
		dp.y = B[7].y-pGir->GetHeightGirderByStaAng(pBx->GetStation());
		if(pC->m_H_CR == gH)
		{
			Dom.DimMoveTo(dp.x, dp.y -pC->m_T1_CR, nCol);
		}
		else
		{
			Dom.DimMoveTo(dp.x,dp.y ,nCol);
			Dom.DimLineTo(gsH,nCol,COMMA(gsH));
		}
		Dom.DimLineToExtend(pC->m_T1_CR, nCol,"T1",COMMA(pC->m_T1_CR));
		Dom.DimLineToExtend(pC->m_H_CR, nCol,"H", COMMA(pC->m_H_CR));
		if(pC->m_dRH1>0)
		{
			Dom.DimLineToExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dRH1, nCol, "");
			Dom.DimLineToExtend(Dom.Always(10), nCol, "RH", COMMA(pC->m_dRH1), FALSE);
		}
		else
			Dom.DimLineToOverExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));
	}
	else
	{
		double dLowerSp = pGirR->GetHeightGirderByStaAng(pBxR->GetStation()) 
			            - pC->m_dRH1 - pC->m_T1_CR - (pC->m_dRH1==0 ? 0 : pC->m_T2_CR)
						- pC->m_H_CR;
		dp.x = B[7].x+dWidthGirN;
		dp.y = B[7].y-pGir->GetHeightGirderByStaAng(pBx->GetStation());
		if(pC->m_H_CR == gH)
		{
			Dom.DimMoveTo(dp.x, dp.y -pC->m_T1_CR, nCol);
		}
		else
		{
			Dom.DimMoveTo(dp.x,dp.y ,nCol);
			Dom.DimLineTo(dLowerSp, nCol, COMMA(dLowerSp));
		}
		Dom.DimLineToExtend(pC->m_T1_CR,nCol,"T1", COMMA(pC->m_T1_CR));
		Dom.DimLineToExtend(pC->m_H_CR, nCol,"H",  COMMA(pC->m_H_CR));
		if(pC->m_dLH1>0)
		{
			Dom.DimLineToExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));
			Dom.DimLineTo(pC->m_dRH1, nCol, "");
			Dom.DimLineToExtend(Dom.Always(10), nCol, "RH", COMMA(pC->m_dRH1), FALSE);
		}
		else
			Dom.DimLineToOverExtend(pC->m_T2_CR,nCol,"T2",COMMA(pC->m_T2_CR));
	}
	//H형강타입 볼트홀
	if(!pC->m_nLGirSpType)
	{
		double dLowerSpace = (pC->m_H_CR-pC->m_wE*pC->m_wF)/2;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(pC->xyBoltPos[0].x, pC->xyBoltPos[0].y-dLowerSpace, 0, "", TRUE);
		Dom.DimLineToExtend(dLowerSpace, 0, COMMA(dLowerSpace), "");
		Dom.DimLineToExtend(pC->m_wE*pC->m_wF, 0, "", "wE@wF");
		Dom.DimLineToExtend(dLowerSpace, 0, COMMA(dLowerSpace), "");
	}	
	if(!pC->m_nLGirSpType && pC->m_cGasset_CR)
	{
		double dLowerSpace = (pC->m_dltH - pC->m_ltE*pC->m_ltF)/2;
		long nIdx = pC->m_ltE>0 ? 1 : 0;
		Dom.SetDirection("RIGHT");
		CDPoint xyGp1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), FALSE, TRUE);
		Dom.DimMoveTo(pC->xyBoltPos[nIdx].x, xyGp1.y, 0, "", TRUE);

		Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
		Dom.DimLineToExtend(pC->m_ltE*pC->m_ltF, 0, "tlE@tlF", COMMA(pC->m_ltE*pC->m_ltF));
		Dom.DimLineToExtend(dLowerSpace, 0, "", COMMA(dLowerSpace));
		Dom.DimMoveTo(pC->xyBoltPos[nIdx].x, xyGp1.y, 1, "", TRUE);
		Dom.DimLineToExtend(pC->m_dltH, 1, "tlH",COMMA(pC->m_dltH));
	}
	if(!pC->m_nRGirSpType && pC->m_cGasset_CR)
	{
		double dLowerSpace = (pC->m_drtH - pC->m_rtE*pC->m_rtF)/2;
		long nIdx = pC->m_rtE>0 ? 3 : 2;
		Dom.SetDirection("LEFT");
		CDPoint xyGp1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, CDPoint(0,1), FALSE, FALSE);
		Dom.DimMoveTo(pC->xyBoltPos[nIdx].x, xyGp1.y, 0, "", TRUE);

		Dom.DimLineTo(dLowerSpace, 0, COMMA(dLowerSpace));
		Dom.DimLineToExtend(pC->m_rtE*pC->m_rtF, 0, "trE@trF", COMMA(pC->m_rtE*pC->m_rtF));
		Dom.DimLineToExtend(dLowerSpace, 0, "", COMMA(dLowerSpace));
		Dom.DimMoveTo(pC->xyBoltPos[nIdx].x, xyGp1.y, 1, "", TRUE);
		Dom.DimLineToExtend(pC->m_drtH, 1, "trH", COMMA(pC->m_drtH));
	}

	*pDom << Dom;
}

// 평면도

void CAPlateDrawCross::DimCRBeamPlanFlSp(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper, BOOL bLeft, double BX, double BY)
{
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();
	CPlateCrossBeam	 *pC	 = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;

	CDomyun Dom(pDom);
	//
	CDPoint vAng = pBx->GetAngleSection().RotateInv90();
	double slope = vAng.y/vAng.x;
	double  sX	 = pC->m_P_CR + (pC->m_uC1/2+pC->m_uB*pC->m_uC+pC->m_uA);
	double  sY	 = sX*slope;
	double GTerm = pGir->GetDisFrontPlateWebToWeb(pBx);

	// 이음판 상면 치수 (상부이음판)
	if(bUpper)
	{
		Dom.SetDirection("BOTTOM");
		if(bLeft)
		{
			sX = pC->m_P_CR-(pC->m_uC1/2+pC->m_uB*pC->m_uC+pC->m_uA) + pC->m_uM_CR*vAng.y;
			sY = sX*slope - pC->m_uM_CR/vAng.x;
		}
		else
		{
			sX = GTerm-pC->m_P_CR-(pC->m_uC1/2+pC->m_uB*pC->m_uC+pC->m_uA)*vAng.x+pC->m_uM_CR*vAng.y;
			sY = sX*slope - pC->m_uM_CR/vAng.x;
		}
		Dom.DimMoveTo(sX,sY,0,"uA");
		Dom.DimLineTo(pC->m_uA,0,"");
		Dom.DimLineToExtend(pC->m_uB*pC->m_uC,0,"","uB@uC");
		Dom.DimLineToOver(pC->m_uC1,0,"uC1");
		Dom.Rotate(vAng.GetAngleRadian(),sX,sY);
		Dom.Move(BX, BY);
		*pDom << Dom;
	}
	else
	{
		// 이음판 하면 치수 (하부이음판)
		if(bLeft)
		{
			sX = pC->m_P_CR-(pC->m_dC1/2+pC->m_dB*pC->m_dC+pC->m_dA) + pC->m_dM_CR*vAng.y;
			sY = sX*slope - pC->m_dM_CR/vAng.x;
		}
		else
		{
			sX = GTerm-pC->m_P_CR-(pC->m_dC1/2+pC->m_dB*pC->m_dC+pC->m_dA)*vAng.x+pC->m_dM_CR*vAng.y;
			sY = sX*slope - pC->m_dM_CR/vAng.x;
		}
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(sX,sY,0,"dA");
		Dom.DimLineTo(pC->m_dA,0,"");
		Dom.DimLineToExtend(pC->m_dB*pC->m_dC,0,"","dB@dC");
		Dom.DimLineToOver(pC->m_dC1,0,"dC1");
		Dom.Rotate(vAng.GetAngleRadian(),sX,sY);
		Dom.Move(BX, BY);
		*pDom << Dom;
	}
}

// 좌측거더의 중심을 0,0으로 놓고 한다.
void CAPlateDrawCross::DimCRBeamPlanLeftRight(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY)
{
	CPlateGirderApp	 *pGir	  = pBx->GetGirder();
	CPlateGirderApp	 *pGirR	  = pGir->m_pRight;
	CPlateCrossBeam	 *pC	  = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;
	CPlateBasicIndex *pBxR	  = pBx->GetBxMatchByCrossBeam(pGirR);

	CDomyun Dom(pDom);
	Dom.SetDimExo(0);
	
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint	vCross	= pC->GetVectorPlan();
	double	dCross	= fabs(vCross.RotateInv(vGir).y);
	double	dWebT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double	dWebTR	= pGirR->GetThickFactByStation(pBxR->GetStation(), G_W);
	long	nCol	= 0;

	CDPoint xyCrL(0,0), xyCrR(0,0), xyBase(0,0), xyBaseL(0,0), xyBaseR(0,0), vX(1,0);
	pC->GetXyBaseLR(xyCrL, xyCrR);
	xyBase	= pGir->GetXyGirderDis(pBx->GetStation());
	xyBaseL	= xyBase+vX*(pC->m_P_CR+dWebT/2)/dCross;
	xyBaseR = xyBase+vX*(~(xyCrL-xyCrR)+dWebT/2/dCross+dWebTR/2/dCross);
	xyBaseR = xyBaseR-vX*(pC->m_P_CR+dWebT/2)/dCross;
	
	double dSpWU = pC->m_uA+pC->m_uB*pC->m_uC+pC->m_uC1/2;
	double dSpWD = pC->m_dA+pC->m_dB*pC->m_dC+pC->m_dC1/2;
		
	if(pC->m_nLGirSpType)
	{		
		if(pC->m_bConnectionPlate)
		{
			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyBaseL.x-dSpWU,xyBaseL.y-pC->m_uM1,nCol,"uD1");
			Dom.DimLineTo(pC->m_uD1,nCol,"");
			Dom.DimLineToExtend(pC->m_uE1*pC->m_uF1,nCol,"","uE1@uF1");
			Dom.DimLineToExtend(pC->m_uM1-(pC->m_uD1+pC->m_uE1*pC->m_uF1)-pC->m_T3_CR/2,nCol,COMMA(pC->m_uM1-(pC->m_uD1+pC->m_uE1*pC->m_uF1)-pC->m_T3_CR/2), "");
			Dom.DimLineToOver(pC->m_T3_CR,nCol,"T3");
			nCol++;
		}

		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(xyBaseL.x-dSpWU, xyBaseL.y+(pC->m_uM1-pC->m_uW), nCol);
		Dom.DimLineTo(pC->m_uW, nCol, "uW");
		
		Dom.SetDirection("RIGHT");
		Dom.SetDimObq(45);
		Dom.DimMoveTo(xyBaseL.x+dSpWU, xyBaseL.y, 0);
		Dom.DimLineTo(pC->m_uM_CR, 0, "uM");
		Dom.DimMoveTo(xyBaseL.x+dSpWU, xyBaseL.y-pC->m_uM_CR, 0, "uD");
		Dom.DimLineTo(pC->m_uD,0,"");
		if(pC->m_uF>0 && pC->m_uE>0)
			Dom.DimLineToExtend(pC->m_uE*pC->m_uF,0,"","uE@uF");		
		Dom.SetDimObq(0);

		// 이음판 상면 치수 (상부이음판)
		Dom.SetDirection("BOTTOM");
		if(pC->m_bConnectionPlate)
		{
			Dom.DimMoveTo(xyBaseL.x-pC->m_uC1/2,xyBaseL.y-pC->m_uM1,0,"uC1");
			Dom.DimLineTo(pC->m_uC1,0,"");
			Dom.DimLineToExtend(pC->m_uB*pC->m_uC,0,"","uB@uC");
			Dom.DimLineToOver(pC->m_uA,0,"uA");

			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyBaseL.x-pC->m_uA-pC->m_uB1*pC->m_uC-pC->m_uC1/2,xyBaseL.y+pC->m_uM1,0,"uA");
			Dom.DimLineTo(pC->m_uA,0,"");
			Dom.DimLineToExtend(pC->m_uB1*pC->m_uC,0,"","uB1@uC");			
		}
		else
		{
			Dom.DimMoveTo(xyBaseL.x-dSpWU,xyBaseL.y-pC->m_uM_CR,0,"uA");
			Dom.DimLineTo(pC->m_uA,0,"");
			Dom.DimLineToExtend(pC->m_uB*pC->m_uC,0,"","uB@uC");
			Dom.DimLineToOver(pC->m_uC1,0,"uC1");
		}
	}

	Dom.SetDirection("LEFT");	
	if(pC->m_nLGirSpType && pC->m_uN_CR)
	{
		Dom.DimMoveTo(xyBaseL.x-dSpWU,xyBaseL.y-pC->m_uM1-pC->m_uN_CR,nCol);
		Dom.DimLineTo(pC->m_uN_CR,nCol,"uN");
		Dom.DimLineToExtend(pC->m_uM1,nCol, "", "uM1");
	}
	else
	{
		if(pC->m_nLGirSpType)	Dom.DimMoveTo(xyBaseL.x-dSpWU,xyBaseL.y-pC->m_uM1,nCol);
		if(!pC->m_nLGirSpType)	Dom.DimMoveTo(xyBaseL.x-dSpWU,xyBaseL.y-pC->m_uM1-pC->m_T3_CR,nCol);
		Dom.DimLineTo(pC->m_uM_CR,nCol,"uM1");
	}
	nCol++;

	if(!pC->m_bConnectionPlate)
	{
		Dom.DimMoveTo(xyBaseL.x-dSpWU,xyBaseL.y-pC->m_T3_CR/2,nCol);
		Dom.DimLineTo(pC->m_T3_CR,nCol,"T3");
	}

	//////////////////////////////////////////////////////////////////////////
	nCol = 0;

	if(pC->m_nRGirSpType)
	{
		if(pC->m_bConnectionPlate)
		{
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xyBaseR.x+dSpWD,xyBaseR.y-pC->m_dM1,nCol,"dD1");
			Dom.DimLineTo(pC->m_dD1,nCol,"");
			Dom.DimLineToExtend(pC->m_dE1*pC->m_dF1,nCol,"","dE1@dF1");
			nCol++;
		}
		
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyBaseR.x+dSpWD, xyBaseR.y+(pC->m_dM1-pC->m_dW), nCol);
		Dom.DimLineTo(pC->m_dW, nCol, "dW");
		
		Dom.SetDirection("LEFT");
		Dom.SetDimObq(45);
		Dom.DimMoveTo(xyBaseR.x-dSpWD, xyBaseR.y, 0);
		Dom.DimLineTo(pC->m_dM_CR, 0, "dM");

		Dom.DimMoveTo(xyBaseR.x-dSpWD, xyBaseR.y-pC->m_dM_CR, 0, "dD");
		Dom.DimLineTo(pC->m_dD,0,"");
		if(pC->m_dF>0 && pC->m_dE>0)
			Dom.DimLineToExtend(pC->m_dE*pC->m_dF,0,"","dE@dF");
		Dom.SetDimObq(0);

		// 이음판 상면 치수 (상부이음판)
		Dom.SetDirection("BOTTOM");
		if(pC->m_bConnectionPlate)
		{
			Dom.DimMoveTo(xyBaseR.x-pC->m_dA-pC->m_dB*pC->m_dC-pC->m_dC1/2,xyBaseR.y-pC->m_dM1,0,"dA");
			Dom.DimLineTo(pC->m_dA,0,"");
			Dom.DimLineToExtend(pC->m_dB*pC->m_dC,0,"","dB@dC");
			Dom.DimLineToOver(pC->m_dC1,0,"dC1");

			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyBaseR.x+pC->m_dC1/2,xyBaseR.y+pC->m_dM1,0,"");
			Dom.DimLineToExtend(pC->m_dB1*pC->m_dC,0,"","dB1@dC");
			Dom.DimLineToOver(pC->m_dA,0,"dA");			
		}
		else
		{
			Dom.DimMoveTo(xyBaseR.x-dSpWD,xyBaseR.y-pC->m_dM_CR,0,"dA");
			Dom.DimLineTo(pC->m_dA,0,"");
			Dom.DimLineToExtend(pC->m_dB*pC->m_dC,0,"","dB@dC");
			Dom.DimLineToOver(pC->m_dC1,0,"dC1");
		}
	}

	Dom.SetDirection("RIGHT");
	if(pC->m_nRGirSpType && pC->m_dN_CR)
	{
		Dom.DimMoveTo(xyBaseR.x+dSpWD,xyBaseR.y-pC->m_dM1-pC->m_dN_CR,nCol);
		Dom.DimLineTo(pC->m_dN_CR, nCol, "dN");
		Dom.DimLineToExtend(pC->m_uM1, nCol, "", "dM1");
	}
	else
	{
		if(pC->m_nRGirSpType)	Dom.DimMoveTo(xyBaseR.x+dSpWD,xyBaseL.y-pC->m_dM1,nCol);
		if(!pC->m_nLGirSpType)	Dom.DimMoveTo(xyBaseR.x+dSpWD,xyBaseL.y-pC->m_T3_CR-pC->m_dM1,nCol);
		Dom.DimLineTo(pC->m_uM_CR,nCol,"dM1");
	}

	Dom.Rotate(vCross.RotateInv(vGir).Rotate90().GetAngleRadian(),xyBase.x,xyBase.y);
	Dom.Move(-xyBase);
	Dom.Move(BX,BY);
	
	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamPlanTop(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY)
{
	CARoadOptionStd  *pOptStd= m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();
	CPlateCrossBeam	 *pC	 = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;

	CDomyun Dom(pDom);

	double	GTerm	= pGir->GetDisFrontPlateWebToWeb(pBx);
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint	vCross	= pC->GetVectorPlan();
	CDPoint vAng	= vCross.Rotate90();
	double	dWebT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double	slope	= vAng.y/vAng.x;
	CString str		= _T(""), str1 = _T(""), str2 = _T("");
	double	dWSp	= (pC->m_sA+pC->m_sB*pC->m_sC)*2+pC->m_sC1;
	double	sX		= dWebT/2;
	double	sy		= pC->m_uM_CR+sX*slope;
	double	dCross	= fabs(pC->GetVectorPlan().RotateInv(vGir).y);
	BOOL	bSkew	= pC->IsSkew();
	double	dLen	= 0;
	CDPoint	vAng2	= -vCross.RotateInv(pGir->GetLine()->GetAngleAzimuth(pBx->GetStation()));

	Dom.SetDimExo(20);

	if(vAng2.y>0) sy += GTerm*(fabs(vAng2.x/vAng2.y));

	if(pC->m_nVStiffsu_CR>0)
	{
		if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			Dom.DimMoveTo(sX, sy,0,"V-STIFFNER");
			dLen = pC->GetLengthGirWebToCrWeb(TRUE);
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			double disSum = 0;
			for(long i = 0 ; i < pC->m_nVStiffsu_CR; i++) 
			{
				disSum += pC->m_VStiffDis_CR[i];
				str1.Format("V%d",i+1);

				dLen = pC->m_VStiffDis_CR[i];
				if(bSkew)	str2.Format("%s (%s)",	COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str1,str2);
			}

			dLen = GTerm-disSum-(pC->GetLengthGirWebToCrWeb(TRUE)+pC->GetLengthGirWebToCrWeb(FALSE));
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
			
			dLen = pC->GetLengthGirWebToCrWeb(FALSE);
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
		else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
		{
			Dom.DimMoveTo(sX, sy,0,"V-STIFFNER");
			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			double disSum = 0;
			for(long i = 0 ; i < pC->m_nVStiffsu_CR; i++)
			{
				disSum += pC->m_VStiffDis_CR[i];
				str1.Format("V%d",i+1);

				dLen = pC->m_VStiffDis_CR[i];
				if(bSkew)	str2.Format("%s (%s)",	COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str1,str2);
			}

			dLen = GTerm-disSum-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(FALSE);
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
			
			dLen = pC->GetLengthGirWebToCrWeb(FALSE);
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
		else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
		{
			Dom.DimMoveTo(sX, sy,0,"V-STIFFNER");
			dLen = pC->GetLengthGirWebToCrWeb(TRUE);
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			double disSum = 0;
			for(long i = 0 ; i < pC->m_nVStiffsu_CR; i++)
			{
				disSum += pC->m_VStiffDis_CR[i];
				str1.Format("V%d",i+1);

				dLen = pC->m_VStiffDis_CR[i];
				if(bSkew)	str2.Format("%s (%s)",	COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str1,str2);
			}

			dLen = GTerm-disSum-pC->m_P_CR-dWSp/2-pC->GetLengthGirWebToCrWeb(TRUE);
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
			
			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
		else
		{
			Dom.DimMoveTo(sX, sy,0,"V-STIFFNER");
			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			double disSum = 0;
			for(long i = 0 ; i < pC->m_nVStiffsu_CR; i++)
			{
				disSum += pC->m_VStiffDis_CR[i];
				str1.Format("V%d",i+1);

				dLen = pC->m_VStiffDis_CR[i];
				if(bSkew)	str2.Format("%s (%s)",	COMMA(dLen), COMMA(dLen/dCross));
				else		str2.Format("%s", COMMA(dLen));
				Dom.DimLineToExtend(dLen,0,str1,str2);
			}

			dLen = GTerm-disSum-pC->m_P_CR*2-dWSp;
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);

			dLen = pC->m_P_CR+dWSp/2;
			str1.Format("%s",		COMMA(dLen));
			if(bSkew)	str2.Format("(%s)",	COMMA(dLen/dCross));
			else		str2.Format("");
			Dom.DimLineToExtend(dLen, 0, str1, str2);
		}
	}

	Dom.Move(BX, BY);
	*pDom << Dom;

	// 상하면 표시
	CPlateGirderApp	*pGirR  = pGir->m_pRight;
	CPlateBasicIndex *pBxSec = (CPlateBasicIndex*)pGir->GetBxMatchByCrossBeam(pBx, pGirR);
	if(!pBxSec)
	{
		*pDom << Dom;
		return;
	}

	CDPoint DiaLxy(0,0), DiaRxy(0,0);
	pC->GetXyBaseLR(DiaLxy, DiaRxy);
	double Len = ~(DiaLxy-DiaRxy)/2 + dWebT/2/dCross;
	double dY  = max(pC->m_uM_CR+pC->m_uN_CR,pC->m_dM_CR+pC->m_dN_CR);
	Dom.SetDirection("TOP");

	Dom.GiSiArrow(Len,-dY-Dom.Always(3),-Dom.Always(10), Dom.Always(5),4,"상 면");
	Dom.GiSiArrow(Len,-dY-Dom.Always(3), Dom.Always(10), Dom.Always(5),0,"하 면");
	pOptStd->SetEnvType(&Dom,HCAD_CENT);
	Dom.LineTo(Len,dY+Dom.Always(1),Len,-dY-Dom.Always(5));
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.Rotate(CDPoint(0,0), vAng.RotateInv(vGir));
		
	Dom.Move(BX, BY);
	*pDom << Dom;
}

void CAPlateDrawCross::DimCRBeamPlanPaint(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY)
{
	CPlateGirderApp	 *pGir	  = pBx->GetGirder();
	CPlateGirderApp	 *pGirR	  = pGir->m_pRight;
	CPlateCrossBeam	 *pC	  = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL)	return;
	CPlateBasicIndex *pBxR	  = pBx->GetBxMatchByCrossBeam(pGirR);

	CDomyun Dom(pDom);
	Dom.SetDimExo(0);
	
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint	vCross	= pC->GetVectorPlan();
	double	dCross	= fabs(vCross.RotateInv(vGir).y);
	double	dWebT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double	dWebTR	= pGirR->GetThickFactByStation(pBxR->GetStation(), G_W);

	CDPoint xyCrL(0,0), xyCrR(0,0), xyBase(0,0), xyBaseL(0,0), xyBaseR(0,0), vX(1,0);
	pC->GetXyBaseLR(xyCrL, xyCrR);
	xyBase	= pGir->GetXyGirderDis(pBx->GetStation());
	xyBaseL	= xyBase+vX*(pC->m_P_CR+dWebT/2)/dCross;
	xyBaseR = xyBase+vX*(~(xyCrL-xyCrR)+dWebT/2/dCross+dWebTR/2/dCross);
	xyBaseR = xyBaseR-vX*(pC->m_P_CR+dWebT/2)/dCross;
	
	double dSpWU = pC->m_uA+pC->m_uB*pC->m_uC+pC->m_uC1/2;
	double dSpWD = pC->m_dA+pC->m_dB*pC->m_dC+pC->m_dC1/2;
		
	if(pC->m_nLGirSpType)
	{
		Dom.SetDirection("RIGHT");
		Dom.SetDimObq(45);
		Dom.DimMoveTo(xyBaseL.x+dSpWU, xyBaseL.y-pC->m_uM_CR, 0);
		Dom.DimLineTo(pC->m_uM_CR*2, 0, "GB1");
		Dom.SetDimObq(0);

		// 이음판 상면 치수 (상부이음판)
		Dom.SetDirection("BOTTOM");
		if(pC->m_bConnectionPlate)
		{
			Dom.DimMoveTo(xyBaseL.x-pC->m_uA-pC->m_uB1*pC->m_uC-pC->m_uC1/2,xyBaseL.y-pC->m_uM1,0);
			Dom.DimLineTo(pC->m_uA+pC->m_uB1*pC->m_uC+pC->m_uC1/2+dSpWU,0,"GW1");
		}
		else
		{
			Dom.DimMoveTo(xyBaseL.x-dSpWU*2,xyBaseL.y-pC->m_uM_CR,0);
			Dom.DimLineTo(dSpWU*2,0,"GW1");
		}
	}

	Dom.SetDirection("LEFT");	
	if(pC->m_nLGirSpType && pC->m_uN_CR)
	{
		Dom.DimMoveTo(xyBaseL.x-dSpWU,xyBaseL.y-pC->m_uM1-pC->m_uN_CR,0);
		Dom.DimLineTo((pC->m_uN_CR+pC->m_uM1)*2,0,"GB2");
	}

	//////////////////////////////////////////////////////////////////////////

	if(pC->m_nRGirSpType && pC->m_dN_CR)
	{
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xyBaseR.x+dSpWD, xyBaseR.y-pC->m_dM1-pC->m_dN_CR, 0);
		Dom.DimLineTo((pC->m_dN_CR+pC->m_uM1)*2,0,"GB3");
	}

	Dom.Rotate(vCross.RotateInv(vGir).Rotate90().GetAngleRadian(),xyBase.x,xyBase.y);
	Dom.Move(-xyBase);
	Dom.Move(BX,BY);
	
	*pDom << Dom;
}

void CAPlateDrawCross::DimVBracingPlanLeft(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd  *pOptStd= m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();
	CPlateGirderApp	 *pGirR  = pGir->m_pRight;
	CVBracing		 *pVr	 = pBx->GetSection()->GetVBracing();
	CSection *pSec = pBx->GetSection();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CVStiff* pVStiff = pSec->GetVStiff();
	if(pVr == NULL)	return;

	CDomyun Dom(pDom);
	CDPoint vAng = pBx->GetAngleSectionCrossVBracing();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vFrontUp = pVr->GetVectorFrame(0);
	CDPoint xyCenL = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint xyCenR = pGirR->GetXyGirderDis(pBxR->GetStation());
	double dWebT = pGir->GetThickFactByStation(pBx->GetStation(), 2);

	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xyCenL.x, xyCenL.y, 0);
	Dom.DimLineToExtend(pVr->m_dS, 0, "S", COMMA(pVr->m_dS));
	Dom.DimLineTo(pVr->m_dG[0]*fabs(vFrontUp.x), 0, COMMA(pVr->m_dG[0]));
	Dom.DimLineToExtend(pVr->m_dULG*fabs(vFrontUp.x), 0, "uLG", COMMA(pVr->m_dULG));
	Dom.SetDirection("BOTTOM");
	double dd = (pBx->GetAngleSection().y==0)? 0 : dWebT/2/fabs(pBx->GetAngleSection().y);
	CDPoint xyTemp = xyCenL+CDPoint(1,0)*(dd+pVStiff->m_dWidth[1]-pVr->m_duL3);
	
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0);
	Dom.DimLineToExtend(pVr->m_duL3, 0, "uL3", COMMA(pVr->m_duL3));
	Dom.DimLineToExtend(pVr->m_duL4, 0, "uL4", COMMA(pVr->m_duL4));
	
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 1);
	Dom.DimLineToExtend(pVr->m_ddL3, 1, "dL3", COMMA(pVr->m_ddL3));
	Dom.DimLineToExtend(pVr->m_ddL4, 1, "dL4", COMMA(pVr->m_ddL4));
	
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 2, "uE", (BOOL)pVr->m_duE);
	Dom.DimLineTo(pVr->m_duE, 2, "");

	Dom.Rotate(xyCenL, vAng);

	*pDom << Dom;
}

void CAPlateDrawCross::DimVBracingPlanRight(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd  *pOptStd= m_pDataMng->GetOptionStd();
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();
	CPlateGirderApp	 *pGirR  = pGir->m_pRight;
	CVBracing		 *pVr	 = pBx->GetSection()->GetVBracing();
	CSection *pSec = pBx->GetSection();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CSection *pSecR = pBxR->GetSection();
	if(pVr == NULL)	return;

	CDomyun Dom(pDom);
	CDPoint vAng = pBx->GetAngleSectionCrossVBracing();
	CDPoint vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vFrontUp = pVr->GetVectorFrame(0);
	CDPoint xyCenL = pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint xyCenR = pGirR->GetXyGirderDis(pBxR->GetStation());
	
	pOptStd->SetEnvType(&Dom,HCAD_DIML);
	if(!pVr->m_bSameLeftRight)
	{
		Dom.SetDirection("TOP");
		CDPoint xyStt = xyCenR+CDPoint(-1,0)*(pVr->m_dS+(pVr->m_dG[0]+pVr->m_dULG)*fabs(vFrontUp.x));
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
		Dom.DimLineToExtend(pVr->m_dURG*fabs(vFrontUp.x), 0, "uRG", COMMA(pVr->m_dURG));
		Dom.DimLineTo(pVr->m_dG[1]*fabs(vFrontUp.x), 0, COMMA(pVr->m_dG[1]));
		Dom.DimLineToExtend(pVr->m_dS, 0, "S", COMMA(pVr->m_dS));

		Dom.SetDirection("BOTTOM");
		
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
		Dom.DimLineToExtend(pVr->m_duR4, 0, "uR4", COMMA(pVr->m_duR4));
		Dom.DimLineToExtend(pVr->m_duR3, 0, "uR3", COMMA(pVr->m_duR3));
		
		Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
		Dom.DimLineToExtend(pVr->m_ddR4, 1, "dR4", COMMA(pVr->m_ddR4));
		Dom.DimLineToExtend(pVr->m_ddR3, 1, "dR3", COMMA(pVr->m_ddR3));
		
		Dom.Rotate(xyCenR, vAng);

		*pDom << Dom;
	}
}

void CAPlateDrawCross::DimVBracingPlanTop(CDomyun *pDom, CPlateBasicIndex *pBx, double BX, double BY)
{
	CPlateGirderApp	 *pGir	 = pBx->GetGirder();
	CVBracing		 *pVr	 = pBx->GetSection()->GetVBracing();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CSection *pSecR = pBxR->GetSection();
	if(pVr == NULL)	return;

	CVStiff *pVStiffL = pBx->GetSection()->GetVStiff();
	CVStiff *pVStiffR = pBxR->GetSection()->GetVStiff();

	CDomyun Dom(pDom);

	CDPoint A[10];
	pGir->GetHunchXyDom(pBx, A);

	CDPoint xyCenL		= pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint xyCenR		= pGir->m_pRight->GetXyGirderDis(pBxR->GetStation());
	CDPoint vCrossPlan	= pVr->GetVectorPlan();//(xyCenR-xyCenL).Unit();	// 가로보의 각도
	CDPoint vGir		= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double	dAng		= fabs(vCrossPlan.RotateInv(vGir).y);

	double dWebT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dWebTR	= pGir->m_pRight->GetThickFactByStation(pBxR->GetStation(), G_W);
	double dSteelH	= pVr->GetSteelMaterialData(CVBracing::UPPER)->m_dH;
	double dTStiffL = pVStiffL ? pVStiffL->m_dThick[1] : 0;
	double dTStiffR = pVStiffR ? pVStiffR->m_dThick[0] : 0;
	double GTerm	= ~(xyCenL-xyCenR)-(dWebT+dWebTR)/2;//pGir->GetDisFrontPlateWebToWeb(pBx);
	CDPoint vAng	= pBx->GetAngleSection().RotateInv90();
	
	double slope = vAng.y/vAng.x;
	CString str = _T(""), str1 = _T(""), str2 = _T("");

	double sX = dWebT/2;
	double sy = 300+sX*slope;

	if(vAng.y>0) sy += GTerm*(vAng.y/vAng.x);

	
	CDPoint vFrontUp	= pVr->GetVectorFrame(0);
	CDPoint xyBeamStt = xyCenL + vCrossPlan*(pVr->m_dS+pVr->m_dG[0]*fabs(vFrontUp.x)) + vCrossPlan.RotateInv90()*(dSteelH-dTStiffL)/2;
	CDPoint xyBeamEnd = xyCenR - vCrossPlan*(pVr->m_dS+pVr->m_dG[1]*fabs(vFrontUp.x)) + vCrossPlan.RotateInv90()*(dSteelH-dTStiffR)/2;
	xyCenL		= pGir->GetXyGirderDis(pBx->GetStation()-(dSteelH-dTStiffL)/2/dAng);
	xyCenR		= pGir->m_pRight->GetXyGirderDis(pBxR->GetStation()-(dSteelH-dTStiffR)/2/dAng);
	double dStt = ~(xyCenL-xyBeamStt)-dWebT/2/pBx->GetAngleSection().y;
	double dEnd = ~(xyCenR-xyBeamEnd)-dWebTR/2/pBx->GetAngleSection().y;
	dStt = dStt*fabs(pBx->GetAngleSection().y);
	dEnd = dEnd*fabs(pBx->GetAngleSection().y);

	double dLen = 0;
	BOOL bSkew = pVr->IsSkew();

	Dom.DimMoveTo(sX, sy, 0,"ANCHOR-BAR");
	
	dLen = dStt;
	str1.Format("%s", COMMA(dLen*dAng));
	if(bSkew)	str2.Format("(%s)", COMMA(dLen));
	else		str2.Format("");
	Dom.DimLineToExtend(dLen*dAng, 0, str1, str2);

	double disSum = 0;
	for(long i = 0 ; i < pVr->m_nAnchorSu; i++)
	{
		disSum += pVr->m_AnchorDis[i];

		dLen = pVr->m_AnchorDis[i];
		str1.Format("A%d",i+1);
		if(bSkew)	str2.Format("%s (%s)", COMMA(dLen*dAng), COMMA(dLen));
		else		str2.Format("");
		Dom.DimLineToExtend(dLen*dAng, 0, str1, str2);
	}

	dLen = GTerm-disSum-(dStt+dEnd);
	str1.Format("%s", COMMA(dLen*dAng));
	if(bSkew)	str2.Format("(%s)", COMMA(dLen));
	else		str2.Format("");
	Dom.DimLineToExtend(dLen*dAng, 0, str1, str2);

	dLen = dEnd;
	str1.Format("%s", COMMA(dLen*dAng));
	if(bSkew)	str2.Format("(%s)", COMMA(dLen));
	else		str2.Format("");
	Dom.DimLineToExtend(dLen*dAng, 0, str1, str2);

	Dom.Move(BX, BY);
	*pDom << Dom;

}


void CAPlateDrawCross::DrawHStiffFront(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(pBx==NULL) return;

	CPlateGirderApp *pGir    = pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);

	double dThickWeb = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dSta      = pBx->GetStation();
	
	CPlateBasicIndex *pBxHs = pGir->GetBxByStationDir(dSta, -1, BX_PLATEHSTIFF);
	CWebHStiff       *pHs   = pBx->GetWebHStiff();
	if(pHs==NULL)	return;

	long   nCol     = bLeft ? nCol=0 : nCol=1;
	double dTHStiff = pHs->m_dThickHStiff[nCol]; 
	double dWHStiff = pHs->m_dWidthHStiff[nCol]; 

	CDPoint A[10], vX(1,0), vY(0,1);
	pGir->GetHunchXyDom(pBx,A);
	CDPoint xyUpCen  = (A[1]+A[2])/2;

	if(bLeft) vX = CDPoint(-1,0);

	for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
	{
		if(pHs->IsEmpty(nDan, bLeft))	continue;

		double dHeightHStiff = pHs->GetHeightHStiffBySta(nDan, bLeft, dSta);
		
		Dom.Rectangle(xyUpCen + vX*dThickWeb/2            - vY*(dHeightHStiff-dTHStiff/2), 
					  xyUpCen + vX*(dThickWeb/2+dWHStiff) - vY*(dHeightHStiff+dTHStiff/2));

	}

	*pDom << Dom;
}

void CAPlateDrawCross::DrawInputHBracingFront(CDomyun *pDom, CPlateGirderApp *pGir, long nPos)
{
	CPlateBridgeApp *pBridge  = m_pDataMng->GetBridge();
	CPlateGirderApp *pGirPre  = pGir->m_pLeft;
	CPlateGirderApp *pGirNext = pGir->m_pRight;
	
	long nPosBr = pGir->GetBracingPosByHBrGussetPos(nPos);
	if(nPosBr==-1)	 return;

	CPlateBasicIndex *pBx1, *pBx2, *pBx3, *pBx4;
	long nG = pGir->GetBracingInTotal(nPosBr)->GetBx()->GetNumberGirder();
	//
	CDomyun Dom(pDom);
	Dom.SetScaleDim(20);
	if(nG<pBridge->GetGirdersu()-1)//마지막 거더가 아닌 경우
	{
		long nPlus = 1;
		if(pGir->GetQtyBracingAll()-1 == nPosBr)
			nPlus = 0;
		if(nPosBr+nPlus >= pGir->GetQtyBracingAll() || nPosBr+nPlus >= pGirNext->GetQtyBracingAll())
			return;

		CBracing* pBr1 = pGir->GetBracingInTotal(nPosBr);
		CBracing* pBr2 = pGir->GetBracingInTotal(nPosBr+nPlus);
		CBracing* pBr3 = pGirNext->GetBracingInTotal(nPosBr);
		CBracing* pBr4 = pGirNext->GetBracingInTotal(nPosBr+nPlus);

		pBx1   = (CPlateBasicIndex*)pBr1->GetBx();
		pBx2   = (CPlateBasicIndex*)pBr2->GetBx();
		pBx3   = (CPlateBasicIndex*)pBr3->GetBx();
		pBx4   = (CPlateBasicIndex*)pBr4->GetBx();
	}
	else                           //마지막 경우
	{
		long nPlus = 1;
		if(pGir->GetQtyBracingAll()-1 == nPosBr)
			nPlus = 0;
		CBracing* pBr1 = pGirPre->GetBracingInTotal(nPosBr);
		CBracing* pBr2 = pGirPre->GetBracingInTotal(nPosBr+nPlus);
		CBracing* pBr3 = pGir->GetBracingInTotal(nPosBr);
		CBracing* pBr4 = pGir->GetBracingInTotal(nPosBr+nPlus);

		pBx1   = (CPlateBasicIndex*)pBr1->GetBx();
		pBx2   = (CPlateBasicIndex*)pBr2->GetBx();
		pBx3   = (CPlateBasicIndex*)pBr3->GetBx();
		pBx4   = (CPlateBasicIndex*)pBr4->GetBx();
	}
	if(pBx1==NULL || pBx2==NULL || pBx3==NULL || pBx4==NULL) return; 
	//
	CHBracingGusset *pBrGu = pGir->GetHBracingGussetInTotal(nPos);
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)pBrGu->GetBx();
	if(pBx==NULL)
		pBx = pGir->GetBxByStation(pBx->GetStation(), BX_VSTIFF); 

	DrawGirderFront(&Dom, pBx);
	if(pBx)
		DrawFrontHGussetSplice(&Dom, pBx, /*bDim*/TRUE);

	
	Dom.SetCalcExtRect();	
	CDRect rect = Dom.GetExtRect();
	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()); 	
	Dom.Move(Dom.Always(100), 0);

	*pDom << Dom;

}

//수평브레이싱 가셋트이음판 정면모습
void CAPlateDrawCross::DrawFrontHGussetSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bDim)
{
	if(pBx==NULL) return;
	
	CPlateGirderApp *pGir	= pBx->GetGirder();
	CHBracingGusset *pBrGu	= pBx->GetHBracingGusset();
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateBasicIndex *pBxPre  = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);

	CPlateGirderApp  *pGirPre = NULL;
	if (pBxPre) pGirPre = pBxPre->GetGirder();


	if(pBrGu==NULL) return;
	CARoadOptionStd  *pOptStd= m_pDataMng->GetOptionStd();
	CGlobarOption *pOpt = m_pDataMng->GetGlobalOption();

	double dHeight  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dTWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dTSp		= pBrGu->m_dT;
	double dH1		= pBrGu->m_dH[0];
	double dH2		= pBrGu->m_dH[1];
	double dHDis	= pBx->GetOffsetFromLowerFlange();

	CDPoint A[10], xyU(0,0), xyL(0,0);
	pBx = ((CPlateBasicIndex*)pGir->GetBxByStation(pBx->GetStation(), BX_CROSSBEAM_VBRACING)); 
	pGir->GetHunchXyDom(pBx,A);
	CDPoint xyUpCen = (A[1]+A[2])/2;
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vCross	= pBx->GetAngleSectionCrossVBracing();
	double	dAng	= fabs(vCross.RotateInv(vGir).y);

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);	
	
	// 081128 기준 수평브레이싱은 하부설치만 지원하고있다
//	long i = pOpt->GetDesignHBracingInstall();
// 	if(pBrGu->IsReal(0) && dH1>0)
// 		DrawFrontHGussetSplice(&Dom, pBx, CDPoint(xyUpCen.x-dTWeb/2, xyUpCen.y-dHeight+dHDis), dTSp, dH1*dAng, TRUE);
// 	if(pBrGu->IsReal(1) && dH2>0)
// 		DrawFrontHGussetSplice(&Dom, pBx, CDPoint(xyUpCen.x+dTWeb/2, xyUpCen.y-dHeight+dHDis), dTSp, dH2*dAng, FALSE);


	// set left
	if (pGirPre)
	{
		CPlateCrossBeam	  *pC1 = pBxPre->GetSection()->GetCrossBeam();
		if (pC1)
		{
			CDPoint B[10];
			pC1->GetCrossXyDom(B);
			CDPoint xy1 = CDPoint(B[2].x, B[2].y-pC1->m_H_CR);
			CDPoint xy2 = CDPoint(B[3].x, B[3].y-pC1->m_H_CR);
			CDPoint vAng = (xy2-xy1).Unit();
			CDPoint xyTarget = CDPoint(xyUpCen.x-dTWeb/2, 0);
			CDPoint xyMatch;
			
			GetXyMatchLineAndLine(xy1, vAng, xyTarget, CDPoint(0,1), xyMatch);
			
			if(pBrGu->IsReal(0) && dH1>0)
				DrawFrontHGussetSplice(&Dom, pBx, xyMatch, dTSp, dH1*dAng, TRUE);
		}
	}

	if (pGir)
	{
		CPlateCrossBeam	  *pC1 = pBx->GetSection()->GetCrossBeam();	
		if (pC1)
		{
			CDPoint B[10];
			pC1->GetCrossXyDom(B);
			double Tl = (B[2].y<=B[3].y)?  0 : pC1->m_T1_CR;
			CDPoint xy1 = CDPoint(B[2].x, B[2].y-pC1->m_H_CR +Tl);
			CDPoint xy2 = CDPoint(B[3].x, B[3].y-pC1->m_H_CR +Tl);
			CDPoint vAng = (xy2-xy1).Unit();
			CDPoint xyTarget = CDPoint(xyUpCen.x+dTWeb/2, 0);
			CDPoint xyMatch;
			
			GetXyMatchLineAndLine(xy1, vAng, xyTarget, CDPoint(0,1), xyMatch);
			
			if(pBrGu->IsReal(1) && dH2>0)
				DrawFrontHGussetSplice(&Dom, pBx, xyMatch, dTSp, dH2*dAng, FALSE);
		}
	}


	//치수선
	if(bDim)
	{
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		CString str;

		if(dH1>0 && dH2<=0)	// 좌측만 있으면
		{
			Dom.SetDirection("LEFT");

			Dom.DimMoveTo(xyUpCen.x-dTWeb/2-dH1, xyUpCen.y-dHeight, 1);
			Dom.DimLineTo(dHDis-dTSp, 1, "vH");
			Dom.DimLineTo(dTSp,	1, "vT");
		
			Dom.SetDirection("BOTTOM");
			Dom.SetDimVLen(15);
			Dom.DimMoveTo(xyUpCen.x-dTWeb/2, xyUpCen.y-dHDis, 0);
			Dom.DimLineTo(-dH1*dAng, 0, COMMA(dH1*dAng));
		}

		if(dH2>0)
		{
			Dom.SetDirection("RIGHT");

			Dom.DimMoveTo(xyUpCen.x+dTWeb/2+dH2, xyUpCen.y-dHeight, 1);
			Dom.DimLineTo(dHDis-dTSp, 1, "vH");
			Dom.DimLineTo(dTSp,	1, "vT");
		
			Dom.SetDirection("BOTTOM");
			Dom.SetDimVLen(15);
			Dom.DimMoveTo(xyUpCen.x+dTWeb/2, xyUpCen.y-dHDis, 0);
			Dom.DimLineTo(dH2*dAng, 0, COMMA(dH2*dAng));

			Dom.Circle(xyUpCen, 10);
		}
	}

	*pDom << Dom;
}

void CAPlateDrawCross::DrawFrontHGussetSplice(CDomyun *pDom, CPlateBasicIndex *pBx, CDPoint xyU, double dHeight, double dWidth, BOOL bLeft)
{
	if(bLeft&& !pBx->GetGirder()->m_pLeft)	return;
	if(bLeft)	pBx = pBx->GetBxMatchByCrossBeam(pBx->GetGirder()->m_pLeft);
	CPlateCrossBeam	*pC		= pBx->GetSection()->GetCrossBeam();
	CVBracing		*pVr	= pBx->GetSection()->GetVBracing();
	CDPoint			vCross	= pC ? pC->GetVectorCross() : pVr->GetVectorFrame(LOWER);
	if(bLeft)		vCross	= vCross.Rotate(CDPoint(-1,0));

	CDomyun Dom(pDom);

	CDPoint xyL = xyU+CDPoint(0,-1)*dHeight;

	Dom.LineTo(xyL, xyU);
	Dom.LineTo(xyU+vCross*dWidth);
	Dom.LineTo(xyL+vCross*dWidth);
	Dom.LineTo(xyL);

	*pDom << Dom;	
}

void CAPlateDrawCross::DimCRBeamGassetKneeBracing(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGirCen = pBridge->GetGirder(-1);
	CPlateCrossBeam *pC      = pBx->GetSection()->GetCrossBeam();

	long nID=0;
	if(pC->m_nLGirSpType)
		nID = 0;		//용접이음
	if(!pC->m_nLGirSpType)
		nID = 1;		//볼트이음

	int nGen = (int)pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_GEN);
	double uB = nGen==0 ? pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UB) : 0;
	double dB = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DB);
	double Thick = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_T); 
	double dE = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DE);
	double uD = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UD);
	double uE = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UE);
	double sT = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_ST);
	double sR = sT<=16 ? pGirCen->m_pSangse->m_Scallop_R1 : pGirCen->m_pSangse->m_Scallop_R2;
	
	if(Thick==0) return;

	CDPoint vAng = CDPoint(0,1);
	CDPoint Gp1 = pC->GetPointGassetSupPlateEndBx(pBx,vAng, TRUE,TRUE);
	CDPoint Gp2 = pC->GetPointGassetSupPlateEndBx(pBx,vAng,FALSE,TRUE);

	CDPoint vGaAng = (Gp1-Gp2).Unit();
	Gp1 -= vGaAng * uB / (vGaAng.RotateInv90()*vGaAng);
	Gp2 += vGaAng * dB / (vGaAng.RotateInv90()*vGaAng);
	//
	CDPoint xy, xy0, xy1, xy2, Kb0, Kb1, Kb3, A[10];
	pC->GetCrossXyDom(A);
	CDPoint vCRAng = (A[3]-A[2]).Unit();
	xy.x = A[2].x;
	xy.y = A[2].y-pC->m_H_CR-pC->m_T1_CR;
	CDPoint vGaAngVert = vGaAng.RotateInv90(); 
	CDPoint Gp3 = Gp1 + vGaAngVert * Thick / fabs(vGaAng*vGaAngVert);

	GetXyMatchLineAndLine(Gp3, vGaAng, xy, vCRAng, xy0);	//하부플랜지와 가세트 교점
	xy1 = xy0 - dE*CDPoint(0, 1);
	GetXyMatchLineAndLine(Gp3, vGaAng, xy1, CDPoint(1, 0), Kb0);	//Knee Bracing과 가세트 교점
	xy2 = xy0 + uE*CDPoint(1, 0);
	GetXyMatchLineAndLine(xy2, CDPoint(0, 1), xy, vCRAng, Kb3);	//Knee Bracing과 하부플랜지 교점

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(10);

	Dom.SetDirection("TOP");
	Dom.DimMoveTo(xy0.x, xy0.y, 0);
	Dom.DimLineTo(uE, 0, COMMA(uE));
	Dom.GiSiArrowExt(xy0.x+sR*cos(ToRadian(45)),xy0.y-sR*sin(ToRadian(45)),
		Dom.Always(10),Dom.Always(5),7,COMMA(sR));

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(Kb0.x, Kb0.y, 0);
	Dom.DimLineTo(uD, 0, COMMA(uD));

	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(Kb0.x, xy0.y-dE, 0);
	Dom.DimLineTo(dE, 0, COMMA(dE));

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(Kb3.x, Kb3.y, 0);
	Dom.DimLineTo(-uD, 0, COMMA(uD));
	*pDom << Dom;
}

// 참고 : 도면은 정면이기 때문에 Dom형태(Shop이 아님)로 그림.
void CAPlateDrawCross::DrawFrontHStiffBx(CDomyun *pDomP, CPlateBasicIndex *pBx)
{
	if(pBx==NULL) return;

	CARoadOptionStd *pOptStd= m_pDataMng->GetOptionStd();
	CGlobarOption   *pOpt   = m_pDataMng->GetGlobalOption();
	CPlateGirderApp	*pGir   = pBx->GetGirder();
	CPlateCrossBeam	*pC     = pBx->GetCrossBeam();

	// 가로보가 없을 경우...
	if(pC==NULL) return;
	// 수평보강재가 없을 경우...
	if(pC->m_nHStiffUppersu_CR+pC->m_nHStiffLowersu_CR <= 0) return;

	CDomyun Dom(pDomP);

	pOptStd->SetEnvType(&Dom,HCAD_STLC);
	// 이음판과 수평보강재의 이격거리
	const double SpliceOffset = pGir->m_pSangse->m_HStiff_Splice_B;
	// 수직보강재와 수평보강재의 이격거리
	const double VStiffOffset = pGir->m_pSangse->m_HStiff_VStiff_B;
	// 수평보강재 개수 및 높이설정 
	double Hdis[4], hDisRate[2];
	long nHStiffUpper = pC->GetHStiffUpperDistRate(hDisRate);
	long i = 0;
	for(i = 0; i < nHStiffUpper; i++)	
		Hdis[i] = hDisRate[i];
	long nHStiffLower = pC->GetHStiffLowerDistRate(hDisRate);
	for(i = 0; i < nHStiffLower; i++)		
		Hdis[nHStiffUpper++] = hDisRate[i];

	CDPoint Po[18];	// 가로보상 수직부재의 중심포인트
	// 가로보 기울기
	double giulCross  = pGir->GetGiulCrossBody(pBx, FALSE);
	// 브라켓 기울기
	double guilGasset = pGir->GetGiulCrossGasset(pBx, FALSE, TRUE);
	// 브라켓에 수평보강재 적용
	BOOL bIncludeGasset = pOpt->GetDesignCrossBeamHStiffAtBracket()==0 ? TRUE : FALSE;
	// 거더복부판의 두께
	double dTWebGir = pGir->GetThickFactByStation(pBx->GetStation(), G_W);

	long PoEa = pGir->GetPointVStiffOnCrossBeamPrv(pBx, Po, FALSE, bIncludeGasset);	// 수직보강재 양면, 전면설치
	if (pC->m_cVSide_CR == 2)	// 후면
	{
		for (long n=0; n < 18; n++)
			Po[n] = CDPoint(-1,-1);
		PoEa = pGir->GetPointVStiffOnCrossBeam(pBx,Po, FALSE, bIncludeGasset);
	}
	// 가셋트와 이음판이 있을 경우
	// 이음판이 없을 경우는 bIncludeGasset에 상관없이 1 ~ PoEa-2 임.
	long nColStt = bIncludeGasset&&pC->m_nLGirSpType ? 0 : 1;
	long nColEnd = bIncludeGasset&&pC->m_nRGirSpType ? PoEa-1 : PoEa-2;

	CDPoint sp,ep;
	double HT  = pC->m_hT_CR/cos(atan(giulCross));	// 수평보강재 두께
	double VT  = pC->m_vT_CR;						// 수직보강재 두께
	if(pC->m_nVStiffsu_CR == 0)	VT = 0;				
	double Len = 0;		// 수평보강재의 길이
	double SW  = 0;		// 모멘트이음판과 전단이음판의 폭
	// 수평보강재의 시종점을 구해 그림
	for(long nH=0; nH<nHStiffUpper; nH++)
	{
		// 수평보강재가 모멘트이음판 높이에 있을 경우
		if(pC->m_mW>0 && Hdis[nH]< (pC->m_mD+pC->m_mW) || Hdis[nH]>(pC->m_H_CR-pC->m_mD-pC->m_mW))
			SW = pC->m_mA*2 + (pC->m_mB*pC->m_mC)*2 + pC->m_mC1;	// 모멘트 이음판 폭
		else	
			SW = pC->m_sA*2 + (pC->m_sB*pC->m_sC)*2 + pC->m_sC1;	// 전단 이음판 폭
	
		for(long n=nColStt; n<nColEnd; n++) 
		{
			if(Po[n].x==-1 && Po[n].y==-1) break;
			
			if(n == 0)				// 좌측 복부판와 좌측 이음판 사이(이음판이 있을 경우만 진입)
			{
				sp.x = Po[n].x + dTWebGir + VStiffOffset;
				sp.y = Po[n].y + VStiffOffset * guilGasset;
				Len  = Po[n+1].x - Po[n].x - (SW/2+SpliceOffset+VStiffOffset+dTWebGir);

				ep.x = sp.x + Len;
				ep.y = sp.y + Len * guilGasset;
			}
			else if(n == 1)			// 좌측 이음판과 첫번째 수직보강재
			{
				ep.x = Po[n+1].x -  VT/2 - VStiffOffset;
				ep.y = Po[n+1].y - (VT/2 + VStiffOffset) * giulCross;
				Len = Po[n+1].x - Po[n].x - (SW/2+SpliceOffset+VStiffOffset+VT/2);
				if(pC->m_nLGirSpType==FALSE)
					Len  = Po[n+1].x - Po[n].x - (VStiffOffset+VT/2);
				// 수직보강재가 없는 경우 ********************************
				if(PoEa == 4)		
				{
					ep.x = Po[n+1].x -  VT/2 - SpliceOffset - SW/2;
					ep.y = Po[n+1].y - (VT/2 + SpliceOffset + SW/2) * giulCross;
					if(pC->m_nRGirSpType==FALSE)	// 수평보강재의 종점에 이음판이 없을 경우	
					{
						ep.x = Po[n+1].x;			// 상부플랜지 끝단위치를 기준으로 함
						ep.y = Po[n+1].y;
					}
					// 수평보강재의 길이는 이음판의 부착여부에 따라 달라짐...
					// 좌측에만 있을 경우
					if(pC->m_nLGirSpType && pC->m_nRGirSpType==FALSE)
						Len = Po[n+1].x - Po[n].x - (SW/2 + SpliceOffset);
					// 우측에만 있을 경우
					if(pC->m_nLGirSpType==FALSE && pC->m_nRGirSpType)
						Len = Po[n+1].x - Po[n].x - (SW/2 + SpliceOffset);
					// 양측 모두 있을 경우
					if(pC->m_nLGirSpType && pC->m_nRGirSpType)
						Len = Po[n+1].x - Po[n].x - (SW + SpliceOffset*2);
					// 양측 모두 이음판이 없을 경우..
					else if(pC->m_nLGirSpType==FALSE && pC->m_nRGirSpType==FALSE)
						Len  = Po[n+1].x - Po[n].x;
				}
				sp.x = ep.x - Len;
				sp.y = ep.y - Len*giulCross;
			}		
			else if(n == (PoEa-3))	// 마지막 수직보강재와 우측 이음판
			{
				sp.x = Po[n].x + VT/2 + VStiffOffset;
				sp.y = Po[n].y + (VT/2 + VStiffOffset) * giulCross;
				Len  = Po[n+1].x - Po[n].x - (VT/2+VStiffOffset+SpliceOffset+SW/2);
				if(pC->m_nRGirSpType==FALSE)
						Len  = Po[n+1].x - Po[n].x - (VT/2+VStiffOffset);

				ep.x = sp.x + Len;
				ep.y = sp.y + Len*giulCross;
			}
			else if(n == (PoEa-2))	// 우측 이음판과 우측 복부판 사이(이음판이 있을 경우만 진입)
			{
				ep.x = Po[n+1].x - VStiffOffset;
				ep.y = Po[n+1].y - VStiffOffset * guilGasset;
				Len  = Po[n+1].x - Po[n].x - (SW/2+SpliceOffset+VStiffOffset+dTWebGir);
				sp.x = ep.x - Len;
				sp.y = ep.y - Len * guilGasset;
			}
			else					// 수직보강재와 수직 수강재 사이
			{
				sp.x = Po[n].x + (VT/2+VStiffOffset);
				sp.y = Po[n].y + (VT/2+VStiffOffset) * giulCross;
				Len  = Po[n+1].x - Po[n].x - (VStiffOffset+VT/2)*2;
				ep.x = sp.x + Len;
				ep.y = sp.y + Len*giulCross;
			}
			if(Len<=0) continue;

			Dom.LineTo(sp.x,sp.y-Hdis[nH]+HT/2 ,ep.x,ep.y-Hdis[nH]+HT/2);
			Dom.LineTo(sp.x,sp.y-Hdis[nH]-HT/2 ,ep.x,ep.y-Hdis[nH]-HT/2);
			Dom.LineTo(sp.x,sp.y-Hdis[nH]-HT/2 ,sp.x,sp.y-Hdis[nH]+HT/2);
			Dom.LineTo(ep.x,ep.y-Hdis[nH]-HT/2 ,ep.x,ep.y-Hdis[nH]+HT/2);
		}
	}

	*pDomP << Dom;
}

void CAPlateDrawCross::DrawTrimCRBeamOnBx(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CARoadOptionStd	*pOptStd	= m_pDataMng->GetOptionStd();
	CPlateBridgeApp	*pDB		= m_pDataMng->GetBridge();
	CPlateGirderApp *pGir		= pBx->GetGirder();	

	CDomyun Dom(pDom);

	CDPoint A[10]; // 거더의 포인트
	pGir->GetHunchXyDom(pBx,A);
	CPlateBasicIndex *pBxNext = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);

	CDPoint	ptMid	= (A[1] + A[2])/2;
	CDPoint xyTrim	= CDPoint(ptMid.x + pDB->m_BindConc.m_dWidth/2 + Dom.Always(10), ptMid.y);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	DrawGirderFront(&Dom, pBx);
	DrawCRBeamWebAndFlangeOnBx(&Dom, pBx);// 복부판, 상판, 하판
	DrawCRBeamSpliceOnBx(&Dom, pBx);      // 상하플랜지 이음판, 전단이음판, 모멘트이음판
	DrawCRBeamGassetOnBx(&Dom, pBx);      // 가셋트-상하플랜지판, 복부판, 보강판
	DrawCRBeamVStiffnerOnBx(&Dom, pBx);   // 수직보강재
	DrawCRBeamAnchorBar(&Dom, pBx);       // 슬래브앵커
	DrawFrontHStiffBx(&Dom, pBx);		  // 가로보 부착 수평보강재	
	
	DrawHStiffFront(&Dom, pBx, TRUE);	  // 거더 부착 수평보강재
	DrawHStiffFront(&Dom, pBx, FALSE);	  // 거더 부착 수평보강재
	DrawHStiffFront(&Dom, pBxNext, TRUE);	  // 수평보강재
	DrawHStiffFront(&Dom, pBxNext, FALSE);	  // 수평보강재
	
	DrawFrontHGussetSplice(&Dom, pBx, TRUE);
	DrawFrontHGussetSplice(&Dom, pBxNext, TRUE);

	Dom.TrimByLineOnSide(xyTrim, xyTrim-CDPoint(0,1)*pGir->GetHeightGirder(pBx, TRUE), -1, TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.CutLightningAnyDir(xyTrim, xyTrim-CDPoint(0,1)*pGir->GetHeightGirder(pBx, TRUE), -Dom.Always(5));

	*pDom << Dom;
}
