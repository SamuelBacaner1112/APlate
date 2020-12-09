// APlateDrawEtc.cpp: implementation of the CAPlateDrawEtc class.
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

CAPlateDrawEtc::CAPlateDrawEtc(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}
CAPlateDrawEtc::~CAPlateDrawEtc()
{

}
void CAPlateDrawEtc::DrawInputSlabAnchor(CDomyun *pDom)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(-1);
	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();
	CPlateSangSeDo* pSang = pGir->m_pSangse;

	double A = pSang->m_SlabAnchor_A;
	double B = pSang->m_SlabAnchor_B;
	double T = pSang->m_SlabAnchor_T;
	BYTE   Type = pSang->m_SlabAnchorType;
	double uwidth = 300;
	double dAng = pGir->GetAngleSlabAnchor(T, uwidth/2);
	//
	CDomyun Dom(pDom);
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(10);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	// 정면도
	DrawSlabAnchorFront(&Dom);
	DimSlabAnchorFront(&Dom);
	CDRect  BoundRect;
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	//중심선
	CDPoint BasePo1(0, BoundRect.bottom);
	CDPoint BasePo2(0, BoundRect.top-BoundRect.Height());	

	pOptStd->SetEnvType(pDom, HCAD_CENT);
	Dom.LineTo(BasePo1, BasePo2);
	if (Type == 2 || Type == 3)
		dAng *=-1;
	Dom.Rotate(ToRadian(dAng), BasePo2.x, BasePo2.y);
	*pDom << Dom;
	// 평면도
	DrawSlabAnchorPlan(&Dom);
	DimSlabAnchorPlan(&Dom);
	Dom.Move(0,BasePo2.y);//정면도와 평면도 사이의 레이어 간격
	*pDom << Dom;
}

void CAPlateDrawEtc::DrawSlabAnchorFront(CDomyun *pDom)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(-1);
	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();
	CPlateSangSeDo* pSang = pGir->m_pSangse;

	double A = pSang->m_SlabAnchor_A;
	double B = pSang->m_SlabAnchor_B;
	double T = pSang->m_SlabAnchor_T;//두께(D10,D13...)
	double MoThick  = 12;//위점선과 아래점선 사이의 간격
	double wing		= B+A;//점선길이

	CDomyun Dom(pDom);
	Dom.ClearEtt(TRUE);
	Dom.SetScaleDim(15);
	pOptStd->SetEnvType(&Dom, HCAD_MIS1);
	CDPoint xy, xy1, xy2, xyR, xyR2;
	//하부점선 
	xy  = CDPoint(-wing/2,0);
	xy1 = CDPoint( wing/2,0);
	Dom.LineTo(xy, xy1);
	xy  = CDPoint(-wing/2,-MoThick);
	xy1 = CDPoint( wing/2,-MoThick);
	Dom.LineTo(xy, xy1);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.AnchorBarFront(0,0, T, 0, A, B);
	*pDom << Dom;
}
void CAPlateDrawEtc::DimSlabAnchorFront(CDomyun *pDom)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(-1);
	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();
	CPlateSangSeDo *pSang  = pBridge->GetGirder(-1)->m_pSangse;

	double A = pSang->m_SlabAnchor_A;
	double B = pSang->m_SlabAnchor_B;
	
	double MoThick  = 12;//점선간의 간격(y좌표에 적용)
	double wing		= B;//점선,B는 GAP과 입력 값 두 의미로 적용
	double minus_wind = -(B/2);//치수선의 시작점
	double Diameter = 5;//앵커의 기둥과 기둥사이의 사각형 간격 

	double	a		= Diameter*tan(ConstPi/8);
	
	CDimDomyunEx DimDom(pDom);
	DimDom.SetScaleDim(10);
	
	// 치수선
	pOptStd->SetEnvType(&DimDom, HCAD_DIML);
	long DimDan = 1;

	DimDom.SetDirection("BOTTOM");
		
	DimDom.DimMoveTo(-B/2,0,DimDan);
	DimDom.DimLineTo(-A,0, "A", COMMA(A));
	DimDom.Rotate(-ToRadian(45), -B/2, 0);//각도, x좌표, y좌표
	
	*pDom << DimDom;

	DimDom.DimMoveTo(B/2,0,DimDan);
	DimDom.DimLineTo(A,0 , "A", COMMA(A));
	DimDom.Rotate(ToRadian(45), B/2, 0);//각도, x좌표, y좌표

	DimDom.DimMoveTo(minus_wind,0,DimDan);//입력한값을 반으로 나누어 기준점을 잡음
	DimDom.DimLineTo(wing,0,"B",COMMA(B));

	*pDom << DimDom;

	// 각도
	DimDom.SetDirection("TOP");
	CString str1,str2;

	CDPoint vAng(1,1);  //45도일 경우
	vAng = vAng.Unit();
	str1.Format("%.0f", vAng.GetAngleDegree());
	str1 += "%%d";

	double R  = B;// 정점부터 각도표현까지의 길이
	CDPoint XY = CDPoint(wing, MoThick);
	CDPoint Prev = CDPoint(B+100, A+100);
	CDPoint Next = CDPoint(A/2+wing, Diameter+wing);

	CDPoint BP = CDPoint(B/2, 0);
	CDPoint staxy = CDPoint(BP.x+R, BP.y);
	CDPoint staxy2 = BP + staxy.Rotate(vAng);
	DimDom.DimArcMoveTo(BP.x, BP.y, R, staxy.x, staxy.y, DimDan);
	DimDom.DimArcLineTo(staxy2.x, staxy2.y, DimDan, str1);

	*pDom << DimDom;
}
void CAPlateDrawEtc::DrawSlabAnchorPlan(CDomyun *pDom)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(-1);
	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();
	CPlateSangSeDo *pSang  = pBridge->GetGirder(-1)->m_pSangse;

	double A = pSang->m_SlabAnchor_A;
	double B = pSang->m_SlabAnchor_B;
	double C = pSang->m_SlabAnchor_C;
	double T = pSang->m_SlabAnchor_T;//두께(D10,D13...)
	BYTE	 nType = pSang->m_SlabAnchorType;
	double MoThick  = 12;//위점선과 아래점선 사이의 간격
	double wing		= (400*(A+B))/383;//점선길이
	double uwidth = 300;

	CDomyun Dom(pDom);
	Dom.SetScaleDim(15);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	double dAng = pGir->GetAngleSlabAnchor(T, uwidth/2);
	Dom.AnchorBarPlan(0,0,T,ToRadian(dAng), A, B, C, (int)nType);
	
	CDPoint xy, xy1, xy2, xyR, xyR2;
	
	// 플랜지라인(상하면)
	xy  = CDPoint(-wing,-uwidth/2);
	xy1 = CDPoint( wing,-uwidth/2);
	Dom.LineTo(xy, xy1);
	xy  = CDPoint(-wing,uwidth/2);
	xy1 = CDPoint( wing,uwidth/2);
	Dom.LineTo(xy, xy1);	

	// 웨브라인(중앙)
	pOptStd->SetEnvType(&Dom, HCAD_STLH);
	xy  = CDPoint(-wing,-MoThick/2);
	xy1 = CDPoint( wing,-MoThick/2);
	Dom.LineTo(xy, xy1);
	xy  = CDPoint(-wing,MoThick/2);
	xy1 = CDPoint( wing,MoThick/2);
	Dom.LineTo(xy, xy1);	

	// 절단선(컷라이트닝)
	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(-wing,(-uwidth/2)-Dom.Always(1),-wing, (uwidth/2)+Dom.Always(1));//왼쪽
	Dom.CutLightning( wing,(-uwidth/2)-Dom.Always(1), wing, (uwidth/2)+Dom.Always(1));//오른쪽

	*pDom << Dom;
}
void CAPlateDrawEtc::DimSlabAnchorPlan(CDomyun *pDom)
{
	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(-1);
	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();
	CPlateSangSeDo *pSang  = pBridge->GetGirder(-1)->m_pSangse;

	BYTE	 nType = pSang->m_SlabAnchorType;
	double uwidth = 300;
	double A = pSang->m_SlabAnchor_A;
	double B = pSang->m_SlabAnchor_B;
	double C = pSang->m_SlabAnchor_C;
	double T = pSang->m_SlabAnchor_T;//두께(D10,D13...)
	double dAng = pGir->GetAngleSlabAnchor(T, uwidth/2);

	double Length	= A*cos(ConstPi/4)+B/2;
	BOOL bUpDir = (nType==1||nType==2) ? TRUE : FALSE;

	CDimDomyunEx Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("RIGHT");
	
	// 치수선
	long DimDan = 1;
	Dom.DimMoveTo(Length,bUpDir?-T/2:T/2,DimDan);
	Dom.DimLineTo(Length,bUpDir?C:-C,"C",COMMA(C));

	if(nType==2||nType==3) dAng *= -1.0;
	Dom.Rotate(ToRadian(dAng),0,0);

	*pDom << Dom;
}

void CAPlateDrawEtc::AddAnchorBarDetailCrBeam (CDomyun *pDom, CPlateBasicIndex *pBx, double Scale, double AnchorDia)
{
	double BX, BY;
	BX = 0;
	BY = 0;

	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBx->GetGirder();
	CPlateCrossBeam	*pC	  = pBx->GetSection()->GetCrossBeam();
	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	double Ang = pGir->GetAngleSlabAnchor(AnchorDia, 150/*pC->m_uM_CR*/);

	DrawAnchorBarDetailSide(&Dom, pBx, BX, BY, Scale, AnchorDia);
	CDRect  BoundRect;
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();

	// 중심선
	CDPoint BasePo(0, -600);
	CDPoint BasePo1(0, AnchorDia*2);
	CDPoint BasePo2(0, -600 -AnchorDia*2);	

	pOptStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.LineTo(BasePo1, BasePo2);
	long nType = pGir->m_pSangse->m_SlabAnchorType;
	if (nType == 2 || nType == 3)
		Ang *=-1;
	Dom.Rotate(ToRadian(Ang), BasePo.x, BasePo.y);
	*pDom << Dom;

	DrawAnchorBarDetailPlan(&Dom, pBx, BX, BY, Scale, AnchorDia);
	Dom.Move(0,-600);
	*pDom << Dom;

	CAPlateSymbolDom sDom(pDom, m_pDataMng);			
	pOptStd->SetEnvType(&sDom, HCAD_DIML);
	sDom.SetScaleDim(Dom.GetScaleDim());
	CDPoint xy;
	xy.x = 0;
	xy.y = -AnchorDia/2 - AnchorDia/4;
	sDom.DrawWeldMark(xy, GI_SA, GI_SA, pBx, -1, 20, 0, TRUE, FALSE, 0);
	Dom << sDom;

	double Length = 300;
	double M  = pC ? pC->m_uM_CR : 150;
	double T3 = pC ? pC->m_T3_CR : 12;
	pOptStd->SetEnvType(&Dom, HCAD_MIS1);
	Dom.LineTo(-Length, T3/2, Length, T3/2);
	Dom.LineTo(-Length, -T3/2, Length, -T3/2);
	Dom.LineTo(-Length, M, Length, M);
	Dom.LineTo(-Length, -M, Length, -M);
	
	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(-Length, -M, -Length, M,TRUE);
	Dom.CutLightning( Length, -M,  Length, M,TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	if (nType == 2 || nType == 3)
	{
		Dom.DimArcMoveTo(0,0,10,50,Ang,3);
		Dom.DimArcLineTo(50,0,3,COMMA(fabs(Ang)) + "%%d");
	}
	else
	{
		Dom.DimArcMoveTo(0,0,10,50,0,3);
		Dom.DimArcLineTo(50,Ang,3,COMMA(fabs(Ang)) + "%%d");
	}

//
	CString str1 = _T(""), str2 = _T("");
	CVector vJ = pGir->GetJewonTotal(GI_SA, pBx);
	str1.Format("A.BAR D%.0fx%.0f-%s",AnchorDia,vJ.z, pBridge->GetMarkBuje(GI_SA, pBx));
	double Len = Dom.GetTextWidth(str1);
	double Dis = -150 ;
	double slop = tan(ToRadian(Ang));
	xy.x = Dis;
	xy.y = Dis*slop + AnchorDia/2;
	Dom.GiSiArrowExtend(xy.x, xy.y, Dom.Always(20),-Len,3,str1,str2);

	Dom.Move(0,-600);
	*pDom << Dom;
}

void CAPlateDrawEtc::AddAnchorBarDetailVBracing(CDomyun *pDom, CPlateBasicIndex *pBx, double Scale, double AnchorDia)
{
	double BX, BY;
	BX = 0;
	BY = 0;

	CPlateBridgeApp* pBridge = m_pDataMng->GetBridge();
	CPlateGirderApp	*pGir    = (CPlateGirderApp*)pBx->GetGirder();
	CARoadOptionStd* pOptStd = m_pDataMng->GetOptionStd();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	double Ang = pGir->GetAngleSlabAnchor(AnchorDia, 150);

	DrawAnchorBarDetailSide(&Dom, pBx, BX, BY, Scale, AnchorDia);
	CDRect  BoundRect;
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();

	// 중심선
	CDPoint BasePo(0, -600);
	CDPoint BasePo1(0, AnchorDia*2);
	CDPoint BasePo2(0, -600 -AnchorDia*2);	

	pOptStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.LineTo(BasePo1, BasePo2);
	long nType = pGir->m_pSangse->m_SlabAnchorType;
	if (nType == 2 || nType == 3)
		Ang *=-1;
	Dom.Rotate(ToRadian(Ang), BasePo.x, BasePo.y);
	*pDom << Dom;

	DrawAnchorBarDetailPlan(&Dom, pBx, BX, BY, Scale, AnchorDia);
	Dom.Move(0,-600);
	*pDom << Dom;

	CAPlateSymbolDom sDom(pDom, m_pDataMng);			
	pOptStd->SetEnvType(&sDom, HCAD_DIML);
	sDom.SetScaleDim(Dom.GetScaleDim());
	CDPoint xy;
	xy.x = 0;
	xy.y = -AnchorDia/2 - AnchorDia/4;
//	sDom.DrawWeldMark(xy, GI_SA, ST_F_U, pBx, -1, 20, 0, TRUE, FALSE, 0);
	Dom << sDom;

	double Length = 300;
	double M  = pBx->GetSection()->GetBeamJewon(CSection::SEC_UP, CSection::B);

	pOptStd->SetEnvType(&Dom, HCAD_MIS1);
	Dom.LineTo(-Length, M, Length, M);
	Dom.LineTo(-Length, -M, Length, -M);
	
	pOptStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(-Length, -M, -Length, M,TRUE);
	Dom.CutLightning( Length, -M,  Length, M,TRUE);

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	if (nType == 2 || nType == 3)
	{
		Dom.DimArcMoveTo(0,0,10,50,Ang,3);
		Dom.DimArcLineTo(50,0,3,COMMA(fabs(Ang)) + "%%d");
	}
	else
	{
		Dom.DimArcMoveTo(0,0,10,50,0,3);
		Dom.DimArcLineTo(50,Ang,3,COMMA(fabs(Ang)) + "%%d");
	}

	CString str1 = _T(""), str2 = _T("");
	CVector vJ = pGir->GetJewonTotal(GI_SA, pBx);
	str1.Format("A.BAR D%.0fx%.0f-%s",AnchorDia,vJ.z, pBridge->GetMarkBuje(GI_SA, pBx));
	double Len = Dom.GetTextWidth(str1);
	double Dis = -150 ;
	double slop = tan(ToRadian(Ang));
	xy.x = Dis;
	xy.y = Dis*slop + AnchorDia/2;
	Dom.GiSiArrowExtend(xy.x, xy.y, Dom.Always(20),-Len,3,str1,str2);

	Dom.Move(0,-600);
	*pDom << Dom;
}


void CAPlateDrawEtc::DrawAnchorBarDetailPlan(CDomyun *pDomP, CPlateBasicIndex *pBx,
									double BX, double BY,
									double Scale, double AnchorDia)
{
	CPlateGirderApp	*pGir = (CPlateGirderApp*)pBx->GetGirder();
	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();

	CDimDomyunEx* p = new CDimDomyunEx(pDomP);
	p->SetScaleDim(Scale);

	// 슬래브 앵커
	pOptStd->SetEnvType(p,HCAD_STLC);

	double A = pBx->GetGirder()->m_pSangse->m_SlabAnchor_A;
	double B = pBx->GetGirder()->m_pSangse->m_SlabAnchor_B;
	double C = pBx->GetGirder()->m_pSangse->m_SlabAnchor_C;
	long nType = pBx->GetGirder()->m_pSangse->m_SlabAnchorType;
	double dAng = pGir->GetAngleSlabAnchor(AnchorDia, 150/*pC->m_uM_CR*/);
	if (nType == 2 || nType == 3)
		dAng *=-1;

	p->AnchorBarPlan(0,0,AnchorDia,0,A,B,C,nType);

	// 치수선
	pOptStd->SetEnvType(p,HCAD_DIML);
	BOOL bUpDir = (nType==1||nType==2) ? TRUE : FALSE;
	double Length	= A*cos(ConstPi/4)+B/2;
	long DimDan = 1;
	p->SetDirection("LEFT");
	p->DimMoveTo(-Length,bUpDir ? AnchorDia/2 : -AnchorDia/2,DimDan);
	p->DimLineTo(-Length, bUpDir ? -C : C,COMMA(C));

	p->SetDirection("RIGHT");
	p->DimMoveTo(Length,bUpDir ? -AnchorDia/2 : AnchorDia/2,DimDan);
	p->DimLineTo(Length,bUpDir ? C : -C ,COMMA(C));

	p->Rotate(ToRadian(dAng),0,0);

	*pDomP << *p;
	delete p;
}

//	정면도
void CAPlateDrawEtc::DrawAnchorBarDetailSide(CDomyun *pDomP, CPlateBasicIndex *pBx,
									double BX, double BY,
									double Scale, double AnchorDia)
{

	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();

	// 슬래브 앵커제원은 고정된 것으로 가정 : 도면의 그리기를 사용
	double MoThick  = 12;
	double wing		= 200;
	double MidL		= pBx->GetGirder()->m_pSangse->m_SlabAnchor_B;
	double SideL	= pBx->GetGirder()->m_pSangse->m_SlabAnchor_A;

	CDimDomyunEx* p = new CDimDomyunEx(pDomP);
	p->SetScaleDim(Scale);

	// 모제판 (가로보, 세로보)
	pOptStd->SetEnvType(p,HCAD_MIS1);
	p->LineTo(-wing, 0, wing, 0);
	p->LineTo(-wing, -MoThick, wing, -MoThick);

	// 슬래브 앵커
	pOptStd->SetEnvType(p,HCAD_STLC);
	p->AnchorBarFront(0,0,AnchorDia,0,SideL,MidL);

	// 치수선
	pOptStd->SetEnvType(p,HCAD_DIML);
	long DimDan = 2;
	p->SetDirection("TOP");
	p->DimMoveTo(-MidL/2,AnchorDia,DimDan);
	p->DimLineTo(MidL,AnchorDia,COMMA(MidL));
	*pDomP << *p;
	//
	p->SetDirection("BOTTOM");
	p->DimMoveTo( -MidL/2,0,DimDan);
	p->DimLineTo(-SideL,0,COMMA(SideL));
	p->Rotate(-ConstPi/4,-MidL/2,0);
	*pDomP << *p;

	p->DimMoveTo( MidL/2,0,DimDan);
	p->DimLineTo(SideL,0,COMMA(SideL));
	p->Rotate(ConstPi/4,MidL/2,0);
	*pDomP << *p;

	// 제원
	p->SetDirection("TOP");
	// 용접
//	long nWeld = ROUNDWELD;
//	p->Weld(-MidL/3,AnchorDia,AnchorDia,MoThick,nWeld,FALSE,TRUE, 15);

	// 각도
	CDPoint vAng(1,1);  //45도일 경우
	vAng = vAng.Unit();
	CString str = _T("");
	str.Format("%.0f", vAng.GetAngleDegree());
	str += "%%d";

	double R  = 30;
//	CDPoint XY = CDPoint(-MidL/2, AnchorDia);
//	CDPoint Prev = CDPoint(0, AnchorDia);
//	CDPoint Next = CDPoint(-MidL/2-wing, AnchorDia+wing);

//	CDPoint BP   = GetOffsetPos(XY, Prev, Next, AnchorDia);  //구부러진 점의 좌표
	CDPoint BP(-MidL/2, 0);
	p->DimArcMoveTo(BP.x, BP.y, SideL, BP.x-vAng.x*R, vAng.y*R, 1);
	p->DimArcLineTo(BP.x-SideL, BP.y, 1, str);

	*pDomP << *p;
	delete p;
}

void CAPlateDrawEtc::DrawFillerSpliceRect(CDomyun *pDom, CPlateBasicIndex *pBx, __int64 nFlag, CPlatePieceObject *pObj, BOOL bFull)
{
	BOOL bNextFiller = TRUE;
	if(!bFull && !pBx->IsFiller(nFlag, bNextFiller))	return;
	if(pObj->GetObjSize() != 5)							return;

	CARoadOptionStd* pOptStd  = m_pDataMng->GetOptionStd();
	CDomyun Dom(pDom);

	CDPoint xyCU, xyCD;
	CDPointArray xyArr;

	xyCU = (pObj->GetPoint(3)+pObj->GetPoint(0))/2;
	xyCD = (pObj->GetPoint(2)+pObj->GetPoint(1))/2;

	if(bFull)
	{
		xyArr.Add(pObj->GetPoint(3));
		xyArr.Add(pObj->GetPoint(0));
		xyArr.Add(pObj->GetPoint(1));
		xyArr.Add(pObj->GetPoint(2));
		xyArr.Add(pObj->GetPoint(3));
	}
	else if(bNextFiller)
	{	
		xyArr.Add(xyCU);
		xyArr.Add(pObj->GetPoint(0));
		xyArr.Add(pObj->GetPoint(1));
		xyArr.Add(xyCD);			
		xyArr.Add(xyCU);
	}
	else
	{
		xyArr.Add(pObj->GetPoint(3));
		xyArr.Add(xyCU);
		xyArr.Add(xyCD);
		xyArr.Add(pObj->GetPoint(2));
		xyArr.Add(pObj->GetPoint(3));
	}

	CDRect rectHatch;
	rectHatch.left		= xyArr.GetAt(0).x;
	rectHatch.right		= xyArr.GetAt(1).x;
	rectHatch.bottom	= xyArr.GetAt(0).y;
	rectHatch.top		= xyArr.GetAt(2).y;
	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	if(rectHatch.Height() < 10)		// Filler 두께 표현
	{
		Dom.HatchRect(rectHatch, "ANSI31");
		Dom.Rectangle(rectHatch);
	}
	else
	{	// Filler 면적 표현
		Dom.MoveTo(xyArr.GetAt(0));
		Dom.LineTo(xyArr.GetAt(1));
		Dom.LineTo(xyArr.GetAt(2));
		Dom.LineTo(xyArr.GetAt(3));
		Dom.LineTo(xyArr.GetAt(0));

		pOptStd->SetEnvType(&Dom, HCAD_STLC, LT_HIDDEN_LINE);
		Dom.HatchingByCDPointArray(xyArr, 45, 30, FALSE);		
	}
		
	*pDom << Dom;
}
