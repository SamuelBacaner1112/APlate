// ShopSymbol.cpp: implementation of the CSymbolDom class.
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

CSymbolDom::CSymbolDom()
{
	m_pDBStd = NULL; 
}

CSymbolDom::CSymbolDom(CDomyun *pDom, CARoadOptionStd* pStd, CAPlateDrawStandard* pDBStd)
{
	CopyAttrWnd(pDom);
	m_pOptStd = pStd;
	m_pDBStd = pDBStd;
}

CSymbolDom::~CSymbolDom()
{

}

//////////////////////////////////////////////////////////////////////
// Function
//////////////////////////////////////////////////////////////////////
void CSymbolDom::DrawWeldMark(CDPoint xy,long nMarkBuje1, long nMarkBuje2, CPlateBasicIndex *pBx, double dWeldSize, double dHeight1, double dHeight2, BOOL bLeft, BOOL bUpper, double dAngle, BOOL bFrontFace, CString szAddStr, BOOL bGrind, BOOL bSttSplice)
{
	/*
	VERIFY(m_pDBStd);
	VERIFY(pBx);
	if(!pBx) return;
	CPlateGirderApp *pGir = pBx->GetGirder(); 	

	long nIdx = m_pDBStd->m_pDB->GetIndexBaseWeld(nMarkBuje1,nMarkBuje2,pBx);
	if(nIdx==-1)
		nIdx = m_pDBStd->m_pDB->GetIndexBaseWeld(nMarkBuje2,nMarkBuje1,pBx);	
	int nType;
	BOOL bWeldByValue = TRUE;
	BOOL bOnDule = FALSE;

	double T1, T2;
	double dThickXWeld = 12;

	if(dWeldSize<0) dWeldSize = pGir->GetSizeWeldFillet(pBx,nMarkBuje1,nMarkBuje2,bSttSplice);
	
	switch(pWeld->m_nWeldType)
	{
		case 0:
			if(pWeld->m_nLayType==1)		
				nType = FILLET;
			else if(bFrontFace)	
				nType = FILLET_L;
			else							
				nType = FILLET_U;
			break;	
		case 4:
			nType = ROUNDWELD;
			break;
	}
	switch(pWeld->m_nImproveType)
	{
		case 0:
			if(pWeld->m_nLayType==1)		
				nType = ITYPE;
			else if(bFrontFace)	
				nType = ITYPE_L;
			else							
				nType = ITYPE_U;
			break;
		case 1:
//			if(pWeld->m_nLayType==0)	
			if(pWeld->m_nLayType==1)		
				nType = KWELD;
			else if(bFrontFace)	
				nType = KWELD_L;
			else						
				nType = KWELD_U;
			break;		
		case 2:
			nType = KWELD;
			T1 = pGir->GetThickJewon(nMarkBuje1,pBx);
			dWeldSize = (T1-2)/2;
			break;
		case 3:			
			if(pWeld->m_nLayType==0)	
				nType = XWELD_L;
			else						
				nType = XWELD_U;
			break;		
		case 4:
			nType = XWELD_UF;			
			break;		
		case 5:
			if((nMarkBuje1==G_F_U || nMarkBuje1==G_F_L || nMarkBuje1==G_W || nMarkBuje1==G_W_R)
				&& (nMarkBuje2==G_F_U || nMarkBuje2==G_F_L || nMarkBuje2==G_W || nMarkBuje2==G_W_R))			
			{
				T1 = pBx->GetFactChain(nMarkBuje1,FALSE)->m_dFactChainThick;
				T2 = pBx->GetFactChain(nMarkBuje2,TRUE)->m_dFactChainThick;	
				bWeldByValue = FALSE;
				dWeldSize = min(T1, T2);
			}
			if(dWeldSize > dThickXWeld)
				nType = XWELD_UF;
			else if(bFrontFace)
				nType = XWELD_L;
			else
				nType = XWELD_U;
			break;
		case 6:
			nType = LWELD_EXT;
			break;
		case 7:
			nType = KWELD_EXT;
			break;
	}

	if(pWeld->m_nLayType == 3) bOnDule = TRUE;

	// 무개선용접에 해당하는 경우 : 맞대기이음+기준두께이하+X개선이아니경우
	double dThickNoImprove = 12;
	if(dThickNoImprove>0 && pWeld->m_nWeldType==1 && fabs(T1-T2)<=dThickNoImprove && dWeldSize < dThickXWeld)
	{
		if((nMarkBuje1==G_F_U || nMarkBuje1==G_F_L) && (nMarkBuje2==G_F_U || nMarkBuje2==G_F_L))
		{
			if(pWeld->m_nLayType==1)		
				nType = ITYPE;
			else if(bFrontFace)	
				nType = ITYPE_L;
			else							
				nType = ITYPE_U;
		}
	}

	CDomyun DomSub(this);		

	if(bWeldByValue)
		DomSub.WeldByValue(xy.x,xy.y,dWeldSize,nType,bLeft,bUpper,dHeight1,dHeight2,bGrind,bOnDule,szAddStr);
	else
		DomSub.Weld(xy.x,xy.y,T1,T2,nType,bLeft,bUpper,dHeight1,dHeight2,bGrind,bOnDule,szAddStr);

	DomSub.Rotate(dAngle,xy.x,xy.y);	
	*this << DomSub;
	*/
}

/*        strTitle1                                         strTitle2
             |                                                 |
            | |                 -->                           | |  
           |---|              -                              |---| 
             |               -    dRotAng : 회전각             | 
             |                -                                |
			 |			                                       | 
			 |		        |<-------dDis-------|              |
			 ---------------x         X xy      x---------------
*/
void CSymbolDom::DrawSymbolSectionGisiArrow(CDPoint xy, double dDis, CString strTitle1, CString strTitle2, double dRotAng, double dScale, BOOL bLeft, BOOL bRight)
{
	double dXLen = 1000, dYLen = 1000, dTriXLen = 300, dTriYLen = 500, dMar = 50;
	if(dScale!=100)
	{
		dXLen	 *= dScale/100;
		dYLen	 *= dScale/100;
		dTriXLen *= dScale/100;
		dTriYLen *= dScale/100;
		dMar	 *= dScale/100;
	}
	CDomyun Dom(this);
	m_pOptStd->SetEnvType(&Dom,HCAD_SYMB);
	Dom.SetTextHeight(5);
	if(fabs(dRotAng) == 90)	// 거더상세도 일때만..
		Dom.SetTextAngle(-dRotAng);

	CDPoint xy1((xy.x-dDis/2),xy.y), xy2((xy.x+dDis/2), xy.y);
	// Left
	if(bLeft)
	{
		Dom.MoveTo(xy1);
		Dom.LineToEx(-dXLen, 0);
		Dom.LineToEx(0, dYLen);
		Dom.LineToEx(dTriXLen,0);
		Dom.LineToEx(-dTriXLen, dTriYLen);
		Dom.LineToEx(-dTriXLen, -dTriYLen);
		Dom.LineToEx(dTriXLen, 0);
		Dom.Solid(xy1.x-dXLen, xy1.y+dYLen, xy1.x-dXLen,xy1.y+dYLen+dTriYLen,
			xy1.x-dXLen-dTriXLen, xy1.y+dYLen, xy1.x-dXLen, xy1.y+dYLen);
		
		Dom.TextOut(xy1.x-dXLen, xy1.y+ dYLen + dTriYLen + dMar, strTitle1);		
	}

	// Right
	if(bRight)
	{
		Dom.MoveTo(xy2);
		Dom.LineToEx(dXLen, 0);
		Dom.LineToEx(0, dYLen);
		Dom.LineToEx(dTriXLen,0);
		Dom.LineToEx(-dTriXLen, dTriYLen);
		Dom.LineToEx(-dTriXLen, -dTriYLen);
		Dom.LineToEx(dTriXLen, 0);
		Dom.Solid(xy2.x+dXLen, xy2.y+dYLen, xy2.x+dXLen, xy2.y+dYLen+dTriYLen,
			xy2.x+dXLen+dTriXLen, xy2.y+dYLen, xy2.x+dXLen, xy2.y+dYLen);

		Dom.TextOut(xy2.x+dXLen, xy2.y + dYLen + dTriYLen + dMar, strTitle1);
	}

	Dom.Rotate(dRotAng*ConstPi/180, xy.x, xy.y);
	*this << Dom;
}

void CSymbolDom::DrawSymbolTitle(double xpos, double ypos, CString Title, CString UpStr, CString DnStr, CString DnTitle)
{
	ASSERT_VALID(m_pOptStd);	

	double Len = GetTextWidth(Title);
	double dCircleRadius = this->GetTextHeight()*1.5;
	CDomyun Dom(this);	

	Dom.MoveTo(xpos-(Len+dCircleRadius*2)/2,ypos);
	Dom.LineTo(xpos+(Len+dCircleRadius*2)/2,ypos);

	Dom.MoveTo(xpos-(Len+dCircleRadius*2)/2,ypos - Always(0.5));
	Dom.LineTo(xpos+(Len)/2-dCircleRadius,ypos - Always(0.5));

//	pOStd->SetEnvType((CDomyun *)&Dom,HCAD_DIML);
	Dom.Circle(xpos+Len/2,ypos,dCircleRadius);	
	Dom.TextOut(xpos-dCircleRadius,ypos+Always(1),Title);
	Dom.TextOut(xpos+(Len)/2,ypos+Always(1),UpStr);
	Dom.TextOut(xpos+(Len)/2,ypos-Dom.GetTextHeight()-Always(1),DnStr);

	*this << Dom;
}

// AngDegRotate : 선형기준으로 회전
void CSymbolDom::DrawSymbolSPMark(double xpos, double ypos, double Height, CString Str, double AngDegRotate, BOOL bBlock, BOOL bUpper, BOOL bSmall)
{
	CDomyun *pDom = new CDomyun(this);
	pDom->SetTextAlignHorz(TA_CENTER);

	pDom->LineTo(0,0,0,Height);	
	double radius = bSmall ? pDom->GetTextHeight()*0.8
		                   : pDom->GetTextHeight()*1.3;
	if(bUpper) {
		pDom->TextCircle(0,Height,Str,2,TRUE,bSmall);
		pDom->Rotate(AngDegRotate,0,0);
	}
	else {
		pDom->TextCircle(0,Height-radius*2,Str,2,TRUE,bSmall);
		pDom->Rotate(AngDegRotate,0,0);
	}

	if(bBlock)
	{		
		CString BlockName;
		BlockName.Format("SYMBLOCK_%s",Str);		
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(xpos,ypos));		
	}
	else
	{
		pDom->Move(xpos,ypos);
		*this << *pDom;
		delete pDom;	
	}	
}

// AngDegRotate : 선형기준으로 회전
void CSymbolDom::DrawSymbolJijumMark(double xpos, double ypos, CString Str, double AngDegRotate, BOOL bBlock)
{
	CDomyun *pDom = new CDomyun(this);
	pDom->SetTextAlignHorz(TA_CENTER);

	double Len = pDom->GetTextHeight()*1.5;
	double Hgt = pDom->GetTextHeight()*1.5;

	pDom->Solid(0,0 ,Len/2, -Hgt, -Len/2, -Hgt, 0,0);
	pDom->TextOut(0,-Hgt-pDom->GetTextHeight()*1.5,Str);
	pDom->Rotate(AngDegRotate,0,0);

	pDom->Move(xpos,ypos);
	*this << *pDom;
	delete pDom;
}

void CSymbolDom::SymbolTextCircle(CDPoint xy, CString str, CString sBlockName, CDPoint vAngRot, BOOL bSameSize)
{
	CDomyun *pDom = new CDomyun(this);
	pDom->SetTextAlignHorz(TA_CENTER);

	long nEaStr = bSameSize ? -1 : 4;
	if(bSameSize && str.GetLength()>2)
	{
		if(str.GetLength()==3)	pDom->SetTextWidthFactor(0.80);
		if(str.GetLength()==4)	pDom->SetTextWidthFactor(0.65);
		if(str.GetLength()>4)	pDom->SetTextWidthFactor(0.55);
	}
	else
		pDom->SetTextWidthFactor(1.0);
	pDom->TextCircle(0,0,str,nEaStr);
	pDom->Rotate(CDPoint(0,0),vAngRot);

	pDom->SetBlockName(sBlockName);
	AddBlockArr(pDom);				
	Insert(sBlockName,xy);
}

void CSymbolDom::SymbolGiSiArrowExtend(CDPoint xy, double Dis1, double Dis2, long nDirArrow, 
									   CString str1, CString str2, CString sBlockName, 
									   CDPoint vAngRot)
{
	CDomyun *pDom = new CDomyun(this);
	pDom->SetTextAlignHorz(TA_CENTER);

	pDom->GiSiArrowExtend(0,0,Dis1,Dis2,nDirArrow,str1,str2);
	pDom->Rotate(CDPoint(0,0),vAngRot);

	pDom->SetBlockName(sBlockName);
	AddBlockArr(pDom);				
	Insert(sBlockName,xy);
}


// AngDegRotate : 선형기준으로 회전
void CSymbolDom::DrawSymbolGirderMark(CDomyun *pCheckDom, double xpos, double ypos, CString Str, double AngDegRotate, BOOL bBlock, long nInit /* = 0 */)
{	
	CDomyun *pDom = new CDomyun(this);
	pDom->SetTextAlignHorz(TA_CENTER);

	double Len1 = pDom->GetScaleDim()*5.0;
	double Len2 = Len1*cos(ToRadian(60));
	double Hgt  = Len1*sin(ToRadian(60));
	CDPoint po[6];

	// 육각형 포인트
	po[0] = CDPoint(-Len1/2,Hgt);
	po[1] = CDPoint( Len1/2,Hgt);
	po[2] = CDPoint((Len1/2+Len2),0);
	po[3] = CDPoint( Len1/2,-Hgt);
	po[4] = CDPoint(-Len1/2,-Hgt);
	po[5] = CDPoint(-(Len1/2+Len2),0);

	for(long n=0; n<5; n++)
		pDom->LineTo(po[n],po[n+1]);
	pDom->LineTo(po[5],po[0]);
	//
	pDom->TextOut(0,-pDom->GetTextHeight()/2,Str);

	pDom->Rotate(AngDegRotate,0,0);

	if(bBlock)
	{		
		CString BlockName;
		BlockName.Format("SYMBLOCK_%s_%2d",Str,nInit);
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(xpos,ypos));		
	}
	else
	{
		pDom->Move(xpos,ypos);
		*this << *pDom;
		delete pDom;
	}
}

void CSymbolDom::DrawSymbolShopMark(double xpos, double ypos, CString Str, double AngDegRotate, double dWidth, BOOL bBlock, BOOL bEllipse)
{
	CDomyun Dom(this);
	Dom.SetTextAlignHorz(TA_CENTER);

	double Len = (dWidth == 0) ? Dom.GetTextWidth(Str) + Dom.Always(4): dWidth;
	double Hgt = Dom.GetTextHeight()*2.5;

	if(bEllipse)
		Dom.Ellipse(0,0,CDPoint(0,Hgt/2),CDPoint(-Len/2,0),0,360);
	Dom.TextOut(0,-Dom.GetTextHeight()/2,Str);
	Dom.Rotate(AngDegRotate+ConstPi*2,0,0);

	Dom.Move(xpos,ypos);
	*this << Dom;	
}

void CSymbolDom::DrawSymbolDetailMark(double xpos, double ypos, double Height, CString UpStr, 
									   CString DnStr, CString csText, BOOL bUpper, BOOL bLeft, double Ang)
{
	CDomyun ArrowDom(this), TextDom(this);

	double dCircleRadius = Always(4.2);
	double Len1 = ArrowDom.GetTextWidth(UpStr)  + dCircleRadius + ArrowDom.Always(2);
	double Len2 = ArrowDom.GetTextWidth(csText) + ArrowDom.Always(3);
	ArrowDom.CircleArrow(xpos,ypos,Height,Len1+Len2,1,"","");

	CDPoint cen = CDPoint(Height*cos(ConstPi/4)+Len1/2,Height);
	if(bLeft)	cen.x *= -1;
	if(!bUpper) cen.y *= -1;
	TextDom.Circle(cen.x,cen.y,dCircleRadius);
	TextDom.TextOut(cen.x,cen.y+TextDom.GetTextHeight()*0.2,UpStr);
	TextDom.TextOut(cen.x,cen.y-TextDom.GetTextHeight()*1.2,DnStr);
	//
	bLeft ? TextDom.SetTextAlignHorz(TA_LEFT) : TextDom.SetTextAlignHorz(TA_RIGHT);
	cen.x = Height*cos(ConstPi/4)+Len1+Len2 - TextDom.Always(1);
	if(bLeft)	cen.x *= -1;
	TextDom.TextOut(cen.x,cen.y+TextDom.GetTextHeight()*0.2,csText);

	if(bLeft)	ArrowDom.Mirror(xpos,TRUE);
	if(!bUpper) ArrowDom.Mirror(ypos,FALSE);
	//
	ArrowDom.Rotate(Ang,xpos,ypos);
	TextDom.Rotate(Ang,0,0);
	TextDom.Move(xpos,ypos);
	*this << ArrowDom;
	*this << TextDom;
}

void CSymbolDom::DrawSymbolNote1(double xpos, double ypos)
{	
	// 도면 이동, 스케일조정
	CGridDomyun p;				
	m_pOptStd->SetEnvType(&p,HCAD_TBL1);
	p.SetScaleDim(GetScaleDim());	

	p.SetRowCol(4,2);	
	p.SetSelWidth(p.Always(50),0);
	p.SetSelWidth(p.Always(30),1);
	p.SetSelHeight(p.Always(5));
	p.SetTextMatrix(0,0,"부재 THK.");
	p.SetTextMatrix(0,1,"용접각장");
	//
	p.SetTextMatrix(1,0,"T < 18");
	p.SetTextMatrix(1,1,"6");
	//
	p.SetTextMatrix(2,0,"18 < T < 24");
	p.SetTextMatrix(2,1,"7");
	//
	p.SetTextMatrix(3,0,"24 < T < 32");
	p.SetTextMatrix(3,1,"8");

	p.AddGrid();
	p.Move(xpos,ypos);
	
	*this << p;
}

void CSymbolDom::DrawSymbolNote2(double xpos, double ypos)
{	
	CDomyun p(this);				
	m_pOptStd->SetEnvType(&p,HCAD_STLC);	
	
	p.MoveTo(0,100);
	p.LineTo(500,100);
	p.LineTo(510,90);
	p.LineTo(510,-90);
	p.LineTo(500,-100);
	p.LineTo(0,-100);

	m_pOptStd->SetEnvType(&p,HCAD_CUTL);	
	p.LineTo(0,-150, 0, 150);

	m_pOptStd->SetEnvType(&p,HCAD_DIML);
	p.GiSiArrowExt(505,95,200,300,1,"C1");
	p.GiSiArrowExt(505,-95,200,0,7,"");
	p.LineTo(705,295,705,-295);

	// 최초 기준점 이동
	p.Move(xpos,ypos);
	*this << p;
}

// xy : 기준점, rvAng : 회전Vector
void CSymbolDom::DrawSymbolVentPlan(CDPoint xy, CDPoint rvAng, BOOL bText, BOOL bUpText)
{
	CDomyun Dom(this);
	m_pOptStd->SetEnvType(&Dom,HCAD_SYMB);
	double Len=150,Wid=100;
	Dom.LineTo(-Len, Wid, Len,-Wid);
	Dom.LineTo(-Len,-Wid, Len, Wid);
	Dom.LineTo(-Len, Wid,-Len,-Wid);
	Dom.LineTo( Len, Wid, Len,-Wid);

	m_pOptStd->SetEnvType(&Dom,HCAD_CENT);
	Dom.LineTo(0, Wid, 0,-Wid);

	if(bText) {
		long nDir = bUpText ? 1 : 7;
		m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
		Len = Dom.GetTextWidth("VENT HOLE");
		Dom.GiSiArrowExtend(0,0,Dom.Always(7),Len,nDir,"VENT HOLE","(TYP.)");
	}
	
	Dom.Rotate(CDPoint(0,0),rvAng);
	Dom.Move(xy.x,xy.y);

	*this << Dom;
}
// CL(중심선)마크 xpos, ypos 위치 
// VerDirection = TRUE 기본값 (상향)
// VerDirection = FALSE (왼쪽 방향) 좌우측 모두 사용 
void CSymbolDom::DrawSymbolCLMark(double xpos, double ypos, BOOL VerDirection, BOOL bBlock)
{	
	CDomyun *pDom = new CDomyun(this);
	m_pOptStd->SetEnvType(pDom,HCAD_SYMB);

	pDom->SetTextAlignHorz(TA_CENTER);
	double TextW = pDom->GetTextWidth("C");

	pDom->TextOut(0, 0, "C");
	pDom->TextOut(TextW/4, -TextW/4,"L");
	
	if (VerDirection == FALSE)
	{
		pDom->Rotate(ToRadian(90),0,0);
	}
	
	pDom->Move(xpos,ypos);
	*this << *pDom;
	delete pDom;

}

void CSymbolDom::DrawSymbolDanMyunMark(double xpos, double ypos, CString cs, long Dir, BOOL bBlock)
{	
	CDomyun *pDom = new CDomyun(this);
	m_pOptStd->SetEnvType(pDom,HCAD_SYMB);

	pDom->SetTextAlignHorz(TA_CENTER);
	m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CString str;
	str.Format("\"%s\"",cs);

	double TextH = pDom->GetTextHeight();
	double TextW = pDom->GetTextWidth(str);
	
	pDom->LineTo(0,0,0,TextH*4);
	pDom->LineTo(0,TextH*4,-TextW*1.2 ,TextH*4);
	pDom->LineTo(-TextW*1.2,TextH*4,0,TextW*1.2);
	
	if (Dir == 0)
	{
		pDom->TextOut(-TextW*1.2/2,TextH,str);
	}
	else if (Dir == 1)
	{
		pDom->Mirror(0,TRUE);
		pDom->TextOut(TextW*1.2/2,TextH,str);
	}
	else if (Dir == 2)
	{
		pDom->Rotate(-ToRadian(90),0,0);
		pDom->TextOut(TextW*1.2/2,TextH,str);
	}
	else if (Dir == 3)
	{
		pDom->Rotate(-ToRadian(90),0,0);
		pDom->Mirror(0,TRUE);
		pDom->TextOut(-TextW*1.2/2,TextH,str);
	}
	else
	{
		pDom->Move(xpos,ypos);
		delete pDom;
		return;
	}

	pDom->Move(xpos,ypos);
	*this << *pDom;
	delete pDom;

}

void CSymbolDom::DrawSymbolThickMark(double xpos, double ypos, CString cs, BOOL bBlock)
{
	CDomyun *pDom = new CDomyun(this);
	m_pOptStd->SetEnvType(pDom,HCAD_DIML);

	double TextH = pDom->GetTextHeight();
	double TextW = pDom->GetTextWidth(cs);
	double R = TextW*3/4;
	double angle = ToDegree(asin((TextW/4)/R));
	double hun = R-R*cos(ToRadian(angle));

	pDom->TextOut(0,0,cs);

	pDom->Arc(-TextW/4,R-hun - TextH/2,R,270-angle,angle*2);

	pDom->Arc(TextW/4,-R+hun- TextH/2,R,90-angle,angle*2);

	pDom->Move(xpos,ypos);
	*this << *pDom;
	delete pDom;

}

void CSymbolDom::DrawSymbolCuttingS(double xpos, double ypos, double Angle)
{
	CDomyun *pDom = new CDomyun(this);

	pDom->SetTextHeight(10);
	pDom->SetTextWidthFactor(0.3);
	pDom->TextOut(0,-pDom->GetTextHeight()/2,"S");

	/*
	double Height = 200;
	double Width  = 50;

	pDom->Ellipse(0,Height,CDPoint(0,Height),CDPoint(-Width,0),40,270);
	pDom->Ellipse(0,-Height,CDPoint(0,Height),CDPoint(-Width,0),220,360);	
	pDom->Ellipse(0,-Height,CDPoint(0,Height),CDPoint(-Width,0),0,90);	
	*/

	pDom->Rotate(Angle,0,0);
	pDom->Move(xpos,ypos);
	*this << *pDom;
	delete pDom;
}

void CSymbolDom::DrawSymbolCircleMark(double xpos, double ypos, double Height, CString UpStr, 
									   CString DnStr, CString csText, BOOL bUpper, BOOL bLeft, double Ang)
{
	CDomyun ArrowDom(this), TextDom(this);

	double dCircleRadius = Always(6);
	double Len1 = ArrowDom.GetTextWidth(UpStr)  + dCircleRadius + ArrowDom.Always(2);
	double Len2 = ArrowDom.GetTextWidth(csText);
	ArrowDom.CircleArrow(xpos,ypos,Height,Len1+Len2,1,"","");

	CDPoint cen = CDPoint(Height*cos(ConstPi/4)+Len1/2,Height);
	if(bLeft)	cen.x *= -1;
	if(!bUpper) cen.y *= -1;
	if(TextDom.GetTextWidth(UpStr) < dCircleRadius * 2)
	{
		TextDom.Circle(cen.x,cen.y,dCircleRadius);		
		TextDom.TextOut(cen.x,cen.y+TextDom.GetTextHeight()*0.2,UpStr);		
		TextDom.TextOut(cen.x,cen.y-TextDom.GetTextHeight()*1.2,DnStr);
	}
	else
	{
		//TextDom.Arc(cen.x,cen.y,dCircleRadius,180,180);		
		if(bLeft)	
		{
			TextDom.Arc(cen.x,cen.y,dCircleRadius,60,300);
			TextDom.SetTextAlignHorz(TA_LEFT);
			TextDom.TextOut(cen.x - dCircleRadius + Always(1),cen.y+TextDom.GetTextHeight()*0.2,UpStr);
		}
		else		
		{
			TextDom.Arc(cen.x,cen.y,dCircleRadius,180,300);
			TextDom.SetTextAlignHorz(TA_RIGHT);
			TextDom.TextOut(cen.x + dCircleRadius - Always(1),cen.y+TextDom.GetTextHeight()*0.2,UpStr);
		}		
		TextDom.SetTextAlignHorz(TA_CENTER);
		TextDom.TextOut(cen.x,cen.y-TextDom.GetTextHeight()*1.2,DnStr);
	}
	//
	bLeft ? TextDom.SetTextAlignHorz(TA_LEFT) : TextDom.SetTextAlignHorz(TA_RIGHT);
	cen.x = Height*cos(ConstPi/4)+Len1+Len2 - TextDom.Always(1);
	if(bLeft)	cen.x *= -1;
	TextDom.TextOut(cen.x,cen.y+TextDom.GetTextHeight()*0.2,csText);

	if(bLeft)	ArrowDom.Mirror(xpos,TRUE);
	if(!bUpper) ArrowDom.Mirror(ypos,FALSE);
	//
	ArrowDom.Rotate(Ang,xpos,ypos);
	TextDom.Rotate(Ang,0,0);
	TextDom.Move(xpos,ypos);
	*this << ArrowDom;
	*this << TextDom;
}

//          -
//        -   -
//      -   1   -
//        -   -
//          -
void CSymbolDom::DrawSymbolDiamondMark(CDPoint xy, CDPoint rAng, CString Str, BOOL bBlock)
{
	long nID = 0;
	CDomyun *pDom = new CDomyun(this);
	CString BlockName = "SYMBOL_DIAMOND";

	double Len = pDom->GetTextWidth(Str)*4;

	pDom->TextOut(0,-pDom->GetTextHeight()/2,Str);
	pDom->LineTo(Len/2,  0,  0, Len/2);
	pDom->LineTo(0,  Len/2, -Len/2, 0);
	pDom->LineTo(-Len/2, 0, 0, -Len/2);
	pDom->LineTo(0, -Len/2,  Len/2, 0);

	if(bBlock)
	{
		while(IsExistBlock(BlockName))
		{
			nID ++;
			BlockName.Format("SYMBOL_HEXAGON%d",nID);
		}
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(xy.x,xy.y));
	}
	else
	{
		pDom->Move(xy.x,xy.y);
		*this << *pDom;
		delete pDom;
	}		
}

void CSymbolDom::DrawSymbolHexagonStr(CDPoint xy, CDPoint rAng, CString str, BOOL bBlock)
{
	long nID = 0;
	CDomyun *pDom = new CDomyun(this);
	CString BlockName = "SYMBOL_HEXAGON0";

	double Len = pDom->GetTextWidth(str);
	double Hgt = pDom->GetTextHeight()*2.2;

	pDom->TextOut(0,-pDom->GetTextHeight()/2,str);
	pDom->LineTo(Len/2, Hgt/2, -Len/2, Hgt/2);
	pDom->LineTo(Len/2,-Hgt/2, -Len/2,-Hgt/2);
	pDom->LineTo(-Len/2, Hgt/2,-Len/2-Len/2*cos(ConstPi/3),0);
	pDom->LineTo(-Len/2,-Hgt/2,-Len/2-Len/2*cos(ConstPi/3),0);
	pDom->LineTo( Len/2, Hgt/2,  Len/2+Len/2*cos(ConstPi/3),0);
	pDom->LineTo( Len/2,-Hgt/2,  Len/2+Len/2*cos(ConstPi/3),0);

	pDom->Rotate(CDPoint(0,0),rAng);

	if(bBlock)
	{
		while(IsExistBlock(BlockName))
		{
			nID ++;
			BlockName.Format("SYMBOL_HEXAGON%d",nID);
		}
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(xy.x,xy.y));
	}
	else
	{
		pDom->Move(xy.x,xy.y);
		*this << *pDom;
		delete pDom;
	}		
}

void CSymbolDom::DrawSymbolGiSiLine(double X , double Y, double Dis, BOOL bLeft, BOOL bUpper, double Height1, double Height2, CString cs)
{
	double H1			= (bUpper) ? Always(Height1) : - Always(Height1);
	double UpperAng	= (bUpper) ? 0 : ConstPi/3;

	double Angle1		= (bLeft) ? ConstPi/3 + UpperAng : - ConstPi/3 - UpperAng;		
	double Dis1 = H1 / tan(Angle1);

	if(Height2!=0) {
		double H2		= (bUpper) ? Always(Height2) : - Always(Height2);
		double UpperAng	= (bUpper) ? 0 : -ConstPi/3;
		double Angle2	= (bLeft) ? ConstPi*2/3 + UpperAng : - ConstPi*2/3 - UpperAng;		
		double Dis2	= H2 / tan(Angle2);
		LineTo(X,Y,X-Dis2,Y+H2);

		if(!bUpper) Angle2 += ConstPi;
		if(Height2<0) Angle2 += ConstPi;
		if(bLeft) Arrow(X,Y,-Angle2);
		else	  Arrow(X,Y,-Angle2 + ConstPi);

		X = X-Dis2;
		Y = Y+H2;
	}
	else {
		if(!bUpper) Angle1 += ConstPi;
		if(bLeft) Arrow(X,Y,-Angle1);
		else	  Arrow(X,Y,-Angle1 + ConstPi);
	}

	LineTo(X,Y,X-Dis1,Y+H1);	
	LineTo(X-Dis1-Dis,Y+H1);
	X = X-Dis1-Dis/2;
	Y = Y+H1;
	TextOut(X,Y+Always(0.5),cs);
}

void CSymbolDom::DrawSymbolBoltBox(double xpos, double ypos, CString str1, CString str2)
{
	double L1 = GetTextWidth(str1);
	double L2 = GetTextWidth(str2);
	double H  = GetTextHeight()+10;

	MoveTo(xpos,ypos);
	LineTo(xpos+L1+L2,ypos);
	LineTo(xpos+L1+L2,ypos+H);
	LineTo(xpos,ypos+H);
	LineTo(xpos,ypos);
	LineTo(xpos+L1,ypos,xpos+L1,ypos+H);

	SetTextAlignHorz(TA_CENTER);
	TextOut(xpos+L1/2,ypos+5,str1);
	TextOut(xpos+L1+L2/2,ypos+5,str2);
}

void CSymbolDom::DrawSymbolTitleExt(double xpos, double ypos, long nSu, CString Title, CString sAssem, CString UpStr, CString DnStr)
{
	ASSERT_VALID(m_pOptStd);	

	CDomyun Dom(this);	
	CString sTitle;
	sTitle.Format("%d - %s",nSu, Title);
	Dom.SetScaleDim(this->GetScaleDim());
	double dCircleRadius = this->GetTextHeight()*2;
	double sTilteLen = GetTextWidth(sTitle);
	double sAssemlen = GetTextWidth(sAssem);
	double DnStrLen  = GetTextWidth(DnStr);
	double Gap = Always(5);
	double TotalLen = 0;
	if (sAssemlen == 0)
		TotalLen = sTilteLen + dCircleRadius*2 + sAssemlen + Gap*2;
	else
		TotalLen = sTilteLen + dCircleRadius*3 + sAssemlen + Gap * 3;
	double OneTextLen = GetTextWidth("A");
	
	Dom.MoveTo(xpos-TotalLen/2,ypos);
	Dom.LineTo(xpos+TotalLen/2,ypos);

	Dom.MoveTo(xpos-TotalLen/2,ypos - Always(0.5));
	Dom.LineTo(xpos+TotalLen/2,ypos - Always(0.5));

	double Cxpos = xpos-TotalLen/2+Gap*2+sTilteLen+dCircleRadius;
	double Cypos = ypos;

	if (sAssemlen == 0)
		Cxpos = xpos+TotalLen/2-dCircleRadius;

	if (DnStrLen > dCircleRadius)
		Dom.Arc(Cxpos,Cypos,dCircleRadius,0,315);
	else
		Dom.Circle(Cxpos,Cypos,dCircleRadius);	

	double dEllipsMajorLen = sAssemlen+Always(1.2);
	double dEllipsMinorLen = this->GetTextHeight()+Always(1.2);
	if (sAssemlen !=0)
		Dom.Ellipse(xpos + TotalLen/2 - dEllipsMajorLen/2,ypos + dEllipsMinorLen,CDPoint(dEllipsMajorLen/2,0)
					,CDPoint(0,dEllipsMinorLen),0,360);

	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(xpos-TotalLen/2,ypos+Always(1),sTitle);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.TextOut(Cxpos,ypos+Always(1.2),UpStr);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(Cxpos-OneTextLen/2,ypos-Dom.GetTextHeight()-Always(1.5),DnStr);
	
	Dom.SetTextAlignHorz(TA_CENTER);
	if (sAssemlen !=0)
		Dom.TextOut(xpos+TotalLen/2-dEllipsMajorLen/2,ypos+dEllipsMinorLen/2+Always(1.2),sAssem);
	
	*this << Dom;
}

void CSymbolDom::DrawSymbolShoeMarkPlan(long nRow, long nCol, BOOL bBlock, BOOL bMoveAndRotate, BOOL bScaleApply)
{
//	CDomyun *pMainDom = m_DrawStd->GetDomyun(m_DrawStd->m_nCurPage);
/*
	CString BlockName = "SHOEPLAN0";
	long nID = (long)(nCol*100+nRow);
	CDomyun *pDom = new CDomyun(this);

	CPlateBridgeApp *pDB  = m_pDBStd->m_pDB;

	long nG = pDB->GetNumberGirderByShoe(nRow,nCol);
	CDiaphragm *pDia = pDB->GetDiaphragmByJijumNo(nG,nCol);

	CShoe *pShoe = pDB->IsFirstShoe(nRow,nCol) ? &pDia->m_ShoeFir : &pDia->m_ShoeSec;

	///////////////////////////////////////////////////////////////////////////
	double radius = bScaleApply ? pDom->Always(3) : 180;	// 원의 반지름
	double Length = bScaleApply ? pDom->Always(7) : 420;	// 원의 중심 ~ 삼각형의 꼭지점
	double triLen = bScaleApply ? pDom->Always(3) : 180;	// 삼각형의 높이
	
	if(pShoe->m_cDirectionShoe==0) radius *= 1.30;
	pDom->Circle(0,0,radius,!pShoe->m_cDirectionShoe);

	if(pShoe->m_cDirectionShoe==1 || pShoe->m_cDirectionShoe==2) // 상하로 그리기
	{
		pDom->LineTo(0,-Length, 0,Length);
		pDom->Solid( 0, Length, -triLen/2, Length-triLen, triLen/2, Length-triLen, 0, Length);
		pDom->Solid( 0,-Length, -triLen/2,-Length+triLen, triLen/2,-Length+triLen, 0,-Length);
	}

	if(pShoe->m_cDirectionShoe==1 || pShoe->m_cDirectionShoe==3) // 좌우로 그리기
	{
		pDom->LineTo(-Length,0,  Length,0);
		pDom->Solid( -Length,0, -Length+triLen, -triLen/2, -Length+triLen, triLen/2, -Length,0);
		pDom->Solid(  Length,0,  Length-triLen, -triLen/2,  Length-triLen, triLen/2,  Length,0);
	}

	CDPoint xy   = pDB->GetXyShoe(nRow,nCol);
	CDPoint rAng = pDB->GetGirder(nG)->GetLine()->GetAngleAzimuth(pDB->GetStationShoeOnGirCenter(nRow,nCol));

	if(bMoveAndRotate)
	{
		pDom->Rotate(CDPoint(0,0),rAng);
		double Angle = pDB->GetAngleShoeOnGirCenter(nRow,nCol).GetAngleRadian() - ConstPi/2;
		pDom->Rotate(Angle,0,0);
	}
	else
		xy = CDPoint(0,0);

	///////////////////////////////////////////////////////////////////////////
	if(bBlock)
	{
		CString BlockName;
		BlockName.Format("SHOEPLAN%d",nID);
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(xy.x,xy.y));
	}
	else
	{
		pDom->Move(xy.x,xy.y);
		*this << *pDom;
		delete pDom;
	}
*/
}

void CSymbolDom::DrawSymbolShoeMarkPlanGen(double x, double y, long nType, double DegAng)
{
//	CDomyun *pMainDom = m_DrawStd->GetDomyun(m_DrawStd->m_nCurPage);

	CString BlockName = "SHOEPLAN0";
	CDomyun *pDom = new CDomyun(this);

	///////////////////////////////////////////////////////////////////////////
	double radius = pDom->Always(3);	// 원의 반지름
	double Length = pDom->Always(7);	// 원의 중심 ~ 삼각형의 꼭지점
	double triLen = pDom->Always(3);	// 삼각형의 높이
	
	BOOL FillInCircle = FALSE;
	if (nType==0) 
		FillInCircle = TRUE;

	if(nType==0 || nType==1 || nType==2 || nType==3)
	{
		if (nType==0)
			radius *= 1.30;

		pDom->Circle(0,0,radius,FillInCircle);
	}
	if(nType==1 || nType==3) // 상하로 그리기
	{
		pDom->LineTo(0,-Length, 0,Length);
		pDom->Solid( 0, Length, -triLen/2, Length-triLen, triLen/2, Length-triLen, 0, Length);
		pDom->Solid( 0,-Length, -triLen/2,-Length+triLen, triLen/2,-Length+triLen, 0,-Length);
	}
	if(nType==2 || nType==3) // 좌우로 그리기
	{
		pDom->LineTo(-Length,0,  Length,0);
		pDom->Solid( -Length,0, -Length+triLen, -triLen/2, -Length+triLen, triLen/2, -Length,0);
		pDom->Solid(  Length,0,  Length-triLen, -triLen/2,  Length-triLen, triLen/2,  Length,0);
	}
	Length = pDom->Always(5); //LRB TYPE
	if(nType==4)
	{
		pDom->Solid( -Length, triLen/2, -Length, -triLen/2, -Length+triLen, 0, -Length+triLen,0);
		pDom->Solid(  Length, triLen/2,  Length, -triLen/2,  Length-triLen, 0,  Length-triLen,0);
		pDom->Solid( triLen/2,  Length, -triLen/2,  Length, 0,  Length-triLen, 0,  Length-triLen);
		pDom->Solid( triLen/2, -Length, -triLen/2, -Length, 0, -Length+triLen, 0, -Length+triLen);
	}

	CDPoint xy;
	if(DegAng !=0)
		pDom->Rotate(ToRadian(DegAng),0,0);
	///////////////////////////////////////////////////////////////////////////

	pDom->Move(x,y);
	*this << *pDom;
	delete pDom;
}

// Length=-1 : 자동산정
void CSymbolDom::DrawSymbolStationFlag(CDPoint xy, CString strUpper, CString strLower, 
									   double Height, double Length, CDPoint rotAngle)
{
	double Hgt = Height;
	double Len = Length;
	if(Length==-1 || Length==1)
		Len = max(GetTextWidth(strUpper,3),GetTextWidth(strLower,3)) * Length;

	CDomyun Dom(this);

	double TH = Dom.GetTextHeight();
	Dom.LineTo(xy.x,xy.y+Hgt, xy.x+Len,xy.y+Hgt);
	Dom.TextOut(xy.x+Len/2, xy.y+Hgt+Always(Dom.GetDimTvp()), strUpper);
	Dom.TextOut(xy.x+Len/2, xy.y+Hgt-TH-Always(Dom.GetDimTvp()), strLower);
	if(Height>0)
	{
		if(Len<0)
			Dom.Rotate(-ConstPi/6,xy.x,xy.y+Hgt);
		else
			Dom.Rotate( ConstPi/6,xy.x,xy.y+Hgt);
	}
	else
	{
		if(Len<0)
			Dom.Rotate( ConstPi/6,xy.x,xy.y+Hgt);
		else
			Dom.Rotate(-ConstPi/6,xy.x,xy.y+Hgt);
	}
	
	Dom.LineTo(xy.x,xy.y+Hgt, xy.x,xy.y);

	Dom.Rotate(xy,rotAngle);

	*this << Dom;
}

void CSymbolDom::DrawSymbolCuttingMark(CDPoint Point, double dRadius, double Angle)
{
	CDomyun Dom(this);		
	Dom.Circle(Point.x,Point.y,dRadius);
	// 솔리드 해치
	CDDWordArray BoundArea, BoundArea2;
	BoundArea.Add(0); BoundArea.Add(Point.x); BoundArea.Add(Point.y-dRadius); BoundArea.Add(Point.x); BoundArea.Add(Point.y);
	BoundArea.Add(0); BoundArea.Add(Point.x); BoundArea.Add(Point.y); BoundArea.Add(Point.x-dRadius); BoundArea.Add(Point.y);
	BoundArea.Add(1); BoundArea.Add(Point.x); BoundArea.Add(Point.y); BoundArea.Add(dRadius); BoundArea.Add(180); BoundArea.Add(270); BoundArea.Add(1);
	Dom.HatchSolid(&BoundArea);
		
	BoundArea2.Add(0); BoundArea2.Add(Point.x); BoundArea2.Add(Point.y+dRadius); BoundArea2.Add(Point.x); BoundArea2.Add(Point.y);
	BoundArea2.Add(0); BoundArea2.Add(Point.x); BoundArea2.Add(Point.y); BoundArea2.Add(Point.x+dRadius); BoundArea2.Add(Point.y);	
	BoundArea2.Add(1); BoundArea2.Add(Point.x); BoundArea2.Add(Point.y); BoundArea2.Add(dRadius); BoundArea2.Add(0); BoundArea2.Add(90); BoundArea2.Add(1);
	Dom.HatchSolid(&BoundArea2);
	
	Dom.Rotate(Angle,Point.x,Point.y);
	*this << Dom;
}

void CSymbolDom::DrawTitleOnTable(double xp, double yp, CString sTitle, long nLayerType, BOOL bTopPos, long nQtyLine)
{ 		
	CDomyun* pDom = new CDomyun(this);		
	pDom->SetTextAlignHorz(TA_CENTER);

	m_pOptStd->SetEnvType(pDom,HCAD_HIDD);
	pDom->Point(0,pDom->GetTextHeight());

	m_pOptStd->SetEnvType(pDom,nLayerType);
	double Len = pDom->GetTextWidth(sTitle);
	double TH  = pDom->GetTextHeight();
	pDom->TextOut(0,0,sTitle);
	for(long n=0; n<nQtyLine; n++)
		pDom->LineTo(-Len/2-pDom->Always(1),-pDom->Always(1)-pDom->Always(0.5)*n,
		              Len/2+pDom->Always(1),-pDom->Always(1)-pDom->Always(0.5)*n);

	pDom->SetCalcExtRect();
	CDRect Bound = pDom->GetExtRect();

	if(bTopPos)
		pDom->Move(xp,yp+TH*1.5);
	else
		pDom->Move(xp,yp-TH*2.5);	

	*this << *pDom;
	delete pDom;
}

// TextHeight : SCALE이 고려된 값
void CSymbolDom::SymbolTextCircle(CDPoint xy, CString str, double Radius, double TextHeight)
{
	CDomyun* pDom = new CDomyun(this);		
	pDom->SetTextAlignHorz(TA_CENTER);

	double R  = (Radius>0)	   ? Radius : pDom->GetScaleDim()*5.0;
	double TH = (TextHeight>0) ? TextHeight : pDom->GetScaleDim()*3.0;;
	double Scale = pDom->GetScaleDim();

	pDom->SetTextHeight(TH/Scale);
	pDom->Circle(xy.x,xy.y,R);
	pDom->TextOut(xy.x,xy.y-TH/2,str);

	*this << *pDom;
	delete pDom;
}

void CSymbolDom::SymbolTextCircleWithLine(CDPoint xy, CString str, CDPoint LineStt, double Radius, double TextHeight)
{
	CDomyun* pDom = new CDomyun(this);		
	pDom->SetTextAlignHorz(TA_CENTER);

	double R  = (Radius>0)	   ? Radius : pDom->GetScaleDim()*5.0;
	double TH = (TextHeight>0) ? TextHeight : pDom->GetScaleDim()*3.0;;
	double Scale = pDom->GetScaleDim();

	pDom->SetTextHeight(TH/Scale);
	pDom->Circle(xy.x,xy.y,R);
	if(fabs(xy.y) > fabs(LineStt.y))
		pDom->LineTo(xy.x,xy.y-R,LineStt.x,LineStt.y);
	else
		pDom->LineTo(xy.x,xy.y+R,LineStt.x,LineStt.y);
	pDom->TextOut(xy.x,xy.y-TH/2,str);

	*this << *pDom;
	delete pDom;
}

// 단면 CUTTING 표시
// 입력 : bLeftText - 단면표시를 왼쪽으로 할 것인가?
void CSymbolDom::DrawSymbolSectionMark(double BX, double BY, CString csText, BOOL bLeftText/*TRUE*/, double AngleDeg, double TextAngleDeg)
{
	CDomyun *pDom = new CDomyun(this);

	double Len = pDom->Always(10);
	double Hgt = pDom->Always(10);
	double Bet = pDom->Always(0.8);

	pDom->MoveTo(0,  0);
	pDom->LineTo(0,Hgt);

	if(bLeftText) {
			pDom->MoveTo(-Bet,  0);
			pDom->LineTo(-Bet,Hgt);
			pDom->GiSiArrowExt(-Len,Hgt,Len,0,0,"");
	}
	else {
			pDom->MoveTo(Bet,  0);
			pDom->LineTo(Bet,Hgt);
			pDom->GiSiArrowExt(Len,Hgt,Len,0,4,"");
	}
	pDom->Rotate(ToRadian(AngleDeg),0,0);
	pDom->Move(BX,BY);
	*this << *pDom;
	
	//text
	CDPoint Rxy(bLeftText ? (-Len/2) : (Len/2), Hgt/2);
	Rxy = ::GetXyRotateRadian(CDPoint(0,0), Rxy, ToRadian(AngleDeg));
	pDom->TextOut(Rxy,csText);
	pDom->Rotate(ToRadian(TextAngleDeg),Rxy.x,Rxy.y);
	pDom->Move(BX,BY);
	*this << *pDom;
	delete pDom;
}


// CDPoint cp : 사각형 중심 좌표
void CSymbolDom::DrawSymbolDirectionArrow(CDPoint cp, CString cs, BOOL bFront)
{
	CDomyun *pDom = new CDomyun(this);
	pDom->SetTextAlignHorz(TA_CENTER);
	double LengthRect = pDom->GetTextWidth(cs)+pDom->Always(3);
	double LengthTri  = pDom->Always(4);
	double HeightRect = pDom->Always(0.5);
	double HeightTri  = pDom->Always(1.5);

	pDom->Solid(-LengthRect/2-LengthTri, 0, -LengthRect/2, -HeightTri, -LengthRect/2, HeightTri, -LengthRect/2-LengthTri, 0);
	pDom->Solid(-LengthRect/2, -HeightRect, LengthRect/2, -HeightRect, 
					LengthRect/2, HeightRect, -LengthRect/2, HeightRect);

	// 회전
	double yT = pDom->Always(1);
	double Ang = 180;
	if(!bFront)
		pDom->Rotate(ToRadian(Ang), 0, 0);

	pDom->TextOut(0, yT, cs);
	cp.x = bFront ? cp.x+LengthRect/2+LengthTri : cp.x = cp.x-LengthRect/2-LengthTri;
	pDom->Move(cp);
	
	*this << *pDom;
	delete pDom;
}

/*
    |
   |----- sText ----|
                   |
*/
void CSymbolDom::DrawSymbolPropName(double xpos, double ypos, CString sText, double Angle)
{
	CDomyun *pDom = new CDomyun(this);
	m_pOptStd->SetEnvType(pDom,HCAD_SYMB);

	pDom->SetTextAlignHorz(TA_CENTER);
	double TextH = pDom->GetTextHeight();
	double TextW = pDom->GetTextWidth(sText);
	double Len = pDom->Always(5);

	// Right Line
	pDom->GiSiArrow(xpos + TextW / 1.7, ypos, Len, Len*0.7, 5, "");
	// Left Line
	pDom->GiSiArrow(xpos - TextW / 1.7, ypos, -Len, Len*0.7, 1, "");
	
	pDom->TextOut(xpos,ypos-TextH / 2,sText);

	if(Angle!=0) pDom->Rotate(Angle * ConstPi / 180, xpos, ypos);

	*this << *pDom;
	delete pDom;
}

void CSymbolDom::DrawSymbolLinePointMark(CDPoint bp, CDPoint ep, CString szSta, CString szTypeText, CString szLeftType, CString szRightType)
{
	CDomyun Dom(this);
	Dom.SetTextAlignHorz(TA_CENTER);

	CDPoint vAng = (ep - bp).Unit();;
	double Len = Dom.GetTextWidth(szTypeText,5);
	double Hgt = Dom.GetTextHeight()*2.5;
	CDPoint ptEllipse, ptLeft, ptRight;
	CDPoint ptStaMark;
	
	ptEllipse = ep + vAng * Hgt/2;
	Dom.Ellipse(ptEllipse,CDPoint(0,Hgt/2),CDPoint(-Len/2,0),0,360);
	Dom.TextOut(ptEllipse.x,ptEllipse.y-Dom.GetTextHeight()/2,szTypeText);		
	Dom.GiSiArrow(ptEllipse.x + Len/2,ptEllipse.y,Dom.Always(15),Dom.Always(2),0,szRightType);
	Dom.GiSiArrow(ptEllipse.x - Len/2,ptEllipse.y,-Dom.Always(15),Dom.Always(2),4,szLeftType);
	Dom.Rotate(ptEllipse, vAng.RotateInv90());
	
	Dom.LineTo(bp, ep);			

	CDomyun SubDom(this);
	SubDom.SetTextAlignHorz(TA_LEFT);
	ptStaMark = ep - vAng * SubDom.Always(Dom.GetDimLevelDis());
	SubDom.MoveTo(ptStaMark);
	SubDom.LineTo(ptStaMark.x + SubDom.GetTextWidth(szSta,2),ptStaMark.y);
	SubDom.TextOut(ptStaMark.x + SubDom.Always(1), ptStaMark.y + SubDom.Always(1), szSta);
	SubDom.Rotate(ToRadian(15) + vAng.RotateInv90().GetAngleRadian(),ptStaMark.x,ptStaMark.y);
	Dom << SubDom;

	*this << Dom;	
}

void CSymbolDom::DrawSymbolTitleExt(double xpos, double ypos, CString Title, CStringArray &strArr)
{
	ASSERT_VALID(m_pOptStd);	

	long nSu = strArr.GetSize();
	double dCircleRadius = this->GetTextHeight()*1.5;
	double Gap = Always(2);
	double Len = GetTextWidth(Title) + dCircleRadius * nSu * 2 + Gap;
	double Len2 = dCircleRadius * nSu * 2 + Gap;
	
	CDomyun Dom(this);	

	Dom.MoveTo(xpos-Len/2,ypos);
	Dom.LineTo(xpos+Len/2,ypos);

	Dom.MoveTo(xpos-Len/2,ypos - Always(0.5));
	Dom.LineTo(xpos+Len/2,ypos - Always(0.5));

	Dom.TextOut(xpos+(Len/2-Len2) - (Len - Len2)/2,ypos + dCircleRadius - Dom.GetTextHeight()/2,Title);

	CDPoint xy;
	for (long n =0; n < nSu; n++)
	{
		xy.x = xpos+Len/2 - Len2 + Gap + dCircleRadius + dCircleRadius*2*n;
		xy.y = ypos + dCircleRadius + Always(0.5);
		Dom.Circle(xy,dCircleRadius);	
		Dom.TextOut(xy.x,xy.y - Dom.GetTextHeight()/2,strArr.GetAt(n));
	}
	*this << Dom;
}

// bSameSize : 같은 크기의 타이틀을 적기위함 
void CSymbolDom::SymbolTextCircle(CDPoint xy, CString str, BOOL bSameSize)
{
	ASSERT_VALID(m_pOptStd);	
	
	CDomyun* pDom = new CDomyun(this);		

	long nEaStr = bSameSize ? -1 : 4;
	if(bSameSize && str.GetLength()>2)
	{
		if(str.GetLength()==3)	pDom->SetTextWidthFactor(0.80);
		if(str.GetLength()==4)	pDom->SetTextWidthFactor(0.65);
		if(str.GetLength()>4)	pDom->SetTextWidthFactor(0.55);
	}
	else
		pDom->SetTextWidthFactor(1.0);

	pDom->TextCircle(xy.x, xy.y, str,nEaStr);
	*this << *pDom;
	delete pDom;
}