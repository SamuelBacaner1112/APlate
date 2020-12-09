// DimDomyunEx.cpp: implementation of the CDimDomyunEx class.
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
BOOL CDimDomyunEx::m_CutExtLine = FALSE;

CDimDomyunEx::CDimDomyunEx(CDomyun *pDom):CDomyun(pDom),m_BasePoint(0,0)
{
	m_nDir = DIR_CENTER;	
	m_bTextDir = FALSE;
	m_SideMarkHeight = 0;
	m_nDan = 0;
	m_bBlock = FALSE;
	m_pMainDom = NULL;	
}

CDimDomyunEx::CDimDomyunEx(CPlateBridgeApp *pDB, CDomyun *pDom):CDomyun(pDom),m_BasePoint(0,0)
{
	m_nDir = DIR_CENTER;	
	m_bTextDir = FALSE;
	m_SideMarkHeight = 0;
	m_pDB = pDB;
	m_nDan = 0;
	m_bBlock = FALSE;   
	m_pMainDom = NULL;	
}

CDimDomyunEx::~CDimDomyunEx()
{

}

void CDimDomyunEx::DimBasePoint(CDPoint BasePoint, long nDan)		
{ 
	if(nDan == 0) 
	{
		m_BasePoint = BasePoint; 
		return;
	}
	double dDis = (Always(m_DIMEXO) + Always(m_DIMVLEN) + Always(m_DIMLEVELDIS)*(nDan-1));
	switch(m_CurDirection)
	{
	case DIM_TOP:	
		m_BasePoint = BasePoint + CDPoint(0,+dDis); 
		break;
	case DIM_BOTTOM:
		m_BasePoint = BasePoint + CDPoint(0,-dDis); 
		break;
	case DIM_LEFT:
		m_BasePoint = BasePoint + CDPoint(-dDis,0); 
		break;
	case DIM_RIGHT:
		m_BasePoint = BasePoint + CDPoint(+dDis,0); 
		break;
	}
}

void CDimDomyunEx::DimBaseCurvePoint(CDPoint BasePoint, long nDan)	
{ 
	m_BaseStaionPoint = BasePoint; 	
}

CDPoint CDimDomyunEx::DimHorMoveTo(CDPoint xy, CString Str, double nLen, BOOL bExtend, BOOL bArrow)
{
	CDPoint OldDimPos = m_CurDimPos;

	double StartLineLen = (nLen == 0) ? GetTextWidth(Str) : nLen;

	//중심일경우
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1);	// 일단 단 때문에 보류
	if(m_CurDirection==DIM_BOTTOM) 
	{
		ExoLine *= -1;
		ExeLine *= -1;
	}

	//각도에 따른 x변화량 계산
	double ExoXByAng = fabs(ExoLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAngExe = fabs(m_BasePoint.y-xy.y+ExeLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAng = fabs(m_BasePoint.y-xy.y) * tan(ToRadian(m_DIMOBQ));

	if(Str != "") 
	{
		LineTo(xy.x-ExoXByAng-StartLineLen, m_BasePoint.y,xy.x-ExoXByAng,m_BasePoint.y);			
		
		double textAngle = GetTextAngle();			
		long   oldHor = GetTextAlignHorz();		
		long nDir = (m_bTextDir) ? -1 : 1;

		//SetTextAngle(m_bTextDir ? 180 : 0);		

		if(nLen==0)
			BlockText(0,Always(m_DIMTVP)*nDir,xy.x-ExoXByAng - StartLineLen/2,m_BasePoint.y, Str);
			//TextOut(xy.x-ExoXByAng - StartLineLen/2,m_BasePoint.y + Always(m_DIMTVP)*nDir,Str);				
		else
		{
			long Horz = GetTextAlignHorz();
			if(nLen==0)
			{
				SetTextAlignHorz(TA_CENTER);
				//TextOut(xy.x-ExoXByAng - StartLineLen + GetTextHeight(),m_BasePoint.y + Always(m_DIMTVP)*nDir,Str);				
				BlockText(0, Always(m_DIMTVP)*nDir, xy.x-ExoXByAng - StartLineLen/2,m_BasePoint.y, Str);
			}
			else
			{
				SetTextAlignHorz(TA_LEFT);
				//TextOut(xy.x-ExoXByAng - StartLineLen + GetTextHeight(),m_BasePoint.y + Always(m_DIMTVP)*nDir,Str);				
				BlockText(0, Always(m_DIMTVP)*nDir, xy.x-ExoXByAng - StartLineLen + Always(1),m_BasePoint.y, Str);
			}			
			SetTextAlignHorz(Horz);
		}

		SetTextAngle(textAngle);	
		SetTextAlignHorz(oldHor);
	}
		
	// 첫번째 치수선 기입
	if(!bExtend)
	{
		LineTo(xy.x-ExoXByAng,xy.y+ExoLine,xy.x+DisXByAngExe,m_BasePoint.y+ExeLine);						
	}
	else
	{
		if(bArrow) Arrow(xy.x+DisXByAng,m_BasePoint.y,ConstPi);
	}

	// 시작 포인터 저장
	m_CurDimPos = xy;

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimHorLineTo(double xDis,double y,const CString &cs, const CString &cs2, BOOL bOver, double nLen, BOOL bExtend, double dAddLen /* = 0*/, BOOL bEdgeArrow /*= TRUE*/)
{
	CDPoint OldDimPos = m_CurDimPos, NewPos;
	
	double StartLineLen;
	double nTopBottom = 1;

	CString str = cs.GetLength() > cs2.GetLength() ? cs: cs2;
	StartLineLen = (nLen == 0) ? GetTextWidth(str) : nLen;

	if(!bOver) StartLineLen = 0;

	NewPos = CDPoint(xDis + m_CurDimPos.x + dAddLen, y);

	//중심일경우
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1);	// 일단 단 때문에 보류
	if(m_CurDirection==DIM_BOTTOM) nTopBottom = -1;	
	ExoLine *= nTopBottom;
	ExeLine *= nTopBottom;	

	//각도에 따른 x변화량 계산
	double ExoXByAng = fabs(ExoLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAngExe = fabs(m_BasePoint.y-y+ExeLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAng = fabs(m_BasePoint.y-y) * tan(ToRadian(m_DIMOBQ));

	// 첫번째 치수선 기입	
	if(dAddLen) 
	{
		ExoLine += Always(10) * nTopBottom;
		CDPoint S1 = CDPoint(NewPos.x-ExoXByAng,y+ExoLine);
		CDPoint S2 = CDPoint(NewPos.x-dAddLen,y);
		CDPoint vVec = (S2 - S1).Unit();
		double dVerg = ~(S1 - S2) / 10;
		CDPoint ArcM = S1 + vVec * ~(S2-S1)/2 + ((m_CurDirection == DIM_TOP) ? vVec.Rotate90() : vVec.RotateInv90()) * dVerg * ((xDis < 0) ? -1 : 1);
		Arc(S1.x,S1.y,ArcM.x,ArcM.y,S2.x,S2.y);		
	}
	if(!bExtend)
	{
		LineTo(NewPos.x-ExoXByAng,y+ExoLine,NewPos.x+DisXByAngExe,m_BasePoint.y+ExeLine);
		if(m_CurDimPos.x < NewPos.x)
		{
			if(xDis>0) BlockDot(m_CurDimPos.x+DisXByAng,m_BasePoint.y,ConstPi,FALSE);
			BlockDot(NewPos.x+DisXByAng,m_BasePoint.y,ConstPi,TRUE);				
		}
		else
		{
			if(xDis>0) BlockDot(m_CurDimPos.x+DisXByAng,m_BasePoint.y,ConstPi,TRUE);
			BlockDot(NewPos.x+DisXByAng,m_BasePoint.y,ConstPi,FALSE);				
		}
	}
	else	
	{
		if(bEdgeArrow) Arrow(NewPos.x+DisXByAng,m_BasePoint.y,0);	
	}

	LineTo(m_CurDimPos.x+DisXByAng,m_BasePoint.y,NewPos.x+DisXByAng+StartLineLen,m_BasePoint.y);

	long nDir = (m_bTextDir) ? -1 : 1;	
	if(bOver)
	{				
		BlockText(0,Always(m_DIMTVP) * nDir,NewPos.x+DisXByAng+StartLineLen/2, m_BasePoint.y, cs);
		if(!cs2.IsEmpty())			
			BlockText(0, - GetTextHeight() - Always(m_DIMTVP) * nDir,NewPos.x+DisXByAng+StartLineLen/2, m_BasePoint.y, cs2);
	}
	else
	{		
		BlockText(0, Always(m_DIMTVP) * nDir,(m_CurDimPos.x+NewPos.x)/2+DisXByAng, m_BasePoint.y, cs);
		if(!cs2.IsEmpty())			
			BlockText(0, - GetTextHeight() - Always(m_DIMTVP) * nDir,(m_CurDimPos.x+NewPos.x)/2+DisXByAng, m_BasePoint.y, cs2);
	}
	
	m_CurDimPos = NewPos;

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimVerMoveTo(CDPoint xy, CString Str, double nLen, BOOL bExtend, BOOL bArrow)
{
	CDPoint OldDimPos = m_CurDimPos;
	
	double StartLineLen = (nLen == 0) ? GetTextWidth(Str) : nLen;

	//중심일경우
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);
	//if(m_nDir == DIR_CENTER) ExoLine = Always(1);	// 일단 단 때문에 보류
	if(m_CurDirection==DIM_RIGHT) 
	{
		ExoLine *= -1;
		ExeLine *= -1;
	}
	
	//각도에 따른 x변화량 계산
	double ExoYByAng = fabs(ExoLine) * tan(ToRadian(m_DIMOBQ));
	double DisYByAngExe = fabs(m_BasePoint.x - xy.x - ExeLine) * tan(ToRadian(m_DIMOBQ));
	double DisYByAng = fabs(m_BasePoint.x - xy.x) * tan(ToRadian(m_DIMOBQ));

	if(Str != "") 
	{
		LineTo(m_BasePoint.x,xy.y - ExoYByAng - StartLineLen,m_BasePoint.x,xy.y - DisYByAngExe);	
		
		double textAngle = GetTextAngle();			
		long   oldHor = GetTextAlignHorz();		
		long nDir = (m_bTextDir) ? -1 : 1;

		SetTextAngle(m_bTextDir ? -90 : 90);		
	
		if(nLen==0)
			//TextOut(m_BasePoint.x - Always(m_DIMTVP)*nDir,xy.y - ExoYByAng - StartLineLen/2,Str);
			BlockText( - Always(m_DIMTVP)*nDir,0,m_BasePoint.x,xy.y - ExoYByAng - StartLineLen/2,Str);
		else
		{
			long Horz = GetTextAlignHorz();
			SetTextAlignHorz(TA_LEFT);
			//TextOut(m_BasePoint.x - Always(m_DIMTVP)*nDir,xy.y - ExoYByAng - StartLineLen/2 + GetTextHeight(),Str);
			BlockText( - Always(m_DIMTVP)*nDir,0,m_BasePoint.x,xy.y - ExoYByAng - StartLineLen/2 + GetTextHeight(),Str);
			SetTextAlignHorz(Horz);	
		}
		SetTextAngle(textAngle);	
		SetTextAlignHorz(oldHor);
	}

	// 첫번째 치수선 기입
	LineTo(xy.x - ExoLine,xy.y - ExoYByAng,m_BasePoint.x - ExeLine,xy.y - DisYByAngExe);	
	if(bArrow) Arrow(m_BasePoint.x,xy.y-DisYByAng,ConstPi/2);	

	// 시작 포인터 저장
	m_CurDimPos = xy;

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimVerLineTo(double x, double yDis,const CString &cs, const CString &cs2, BOOL bOver, double nLen, BOOL bExtend, double dAddLen, BOOL bEdgeArrow/* = TRUE*/)
{
	CDPoint OldDimPos = m_CurDimPos, NewPos;
	double StartLineLen;
	CString str = cs.GetLength() > cs2.GetLength() ? cs: cs2;
	StartLineLen = (nLen == 0) ? GetTextWidth(str) : nLen;
	if(!bOver) StartLineLen = 0;

	NewPos = CDPoint(x, m_CurDimPos.y + yDis + dAddLen);

	//중심일경우
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1); // 일단 단 때문에 보류
	if(m_CurDirection==DIM_RIGHT) 
	{
		ExoLine *= -1;
		ExeLine *= -1;
	}

	//각도에 따른 x변화량 계산
	double ExoYByAng = fabs(ExoLine) * tan(ToRadian(m_DIMOBQ));
	double DisYByAngExe = fabs(m_BasePoint.x - x - ExeLine) * tan(ToRadian(m_DIMOBQ));
	double DisYByAng = fabs(m_BasePoint.x - x) * tan(ToRadian(m_DIMOBQ));

	// 첫번째 치수선 기입
	if(!bExtend)
	{				
		if(dAddLen) 
		{
			CDPoint S1 = CDPoint(NewPos.x,NewPos.y - dAddLen);
			CDPoint S2 = CDPoint(NewPos.x - ExoLine,NewPos.y - ExoYByAng);
			CDPoint vVec = (S2 - S1).Unit();
			double dVerg = ~(S1 - S2) / 10;
			CDPoint ArcM = S1 + vVec * ~(S2-S1)/2 + ((m_CurDirection == DIM_LEFT) ? vVec.Rotate90() : vVec.RotateInv90()) * dVerg * ((yDis > 0) ? -1 : 1);
			Arc(S1.x,S1.y,ArcM.x,ArcM.y,S2.x,S2.y);			
		}
		LineTo(NewPos.x - ExoLine,NewPos.y - ExoYByAng,m_BasePoint.x - ExeLine,NewPos.y - DisYByAngExe);	
		if(m_CurDimPos.y >= NewPos.y)
		{
			BlockDot(m_BasePoint.x,m_CurDimPos.y-DisYByAng,ConstPi/2,FALSE);
			BlockDot(m_BasePoint.x,NewPos.y-DisYByAng,ConstPi/2,TRUE);
		}
		else
		{
			BlockDot(m_BasePoint.x,m_CurDimPos.y-DisYByAng,ConstPi/2,TRUE);
			BlockDot(m_BasePoint.x,NewPos.y-DisYByAng,ConstPi/2,FALSE);
		}
		LineTo(m_BasePoint.x,m_CurDimPos.y-DisYByAng,m_BasePoint.x,NewPos.y-DisYByAng + StartLineLen);
	}
	else	
	{
		if(bEdgeArrow)	Arrow(m_BasePoint.x,NewPos.y-DisYByAng,ConstPi/2);	
	}
	
	long nDir = (m_bTextDir) ? -1 : 1;
	double textAngle = GetTextAngle();			
	SetTextAngle(90 * nDir);	
	if(bOver)
	{		
		//TextOut(m_BasePoint.x - Always(m_DIMTVP) * nDir,NewPos.y-DisYByAng + StartLineLen/2,cs);
		BlockText( - Always(m_DIMTVP) * nDir,0,m_BasePoint.x,NewPos.y-DisYByAng + StartLineLen/2,cs);
		if(!cs2.IsEmpty())
			//TextOut(m_BasePoint.x + GetTextHeight() + Always(m_DIMTVP) * nDir,NewPos.y-DisYByAng + StartLineLen/2,cs2);		
			BlockText(+ GetTextHeight() + Always(m_DIMTVP) * nDir,0,m_BasePoint.x ,NewPos.y-DisYByAng + StartLineLen/2,cs2);
	}
	else
	{
		//TextOut(m_BasePoint.x - Always(m_DIMTVP) * nDir,(m_CurDimPos.y+NewPos.y)/2-DisYByAng,cs);
		BlockText( - Always(m_DIMTVP) * nDir, 0, m_BasePoint.x,(m_CurDimPos.y+NewPos.y)/2-DisYByAng, cs);
		if(!cs2.IsEmpty())
			//TextOut(m_BasePoint.x + GetTextHeight() + Always(m_DIMTVP) * nDir,(m_CurDimPos.y+NewPos.y)/2-DisYByAng,cs2);		
			BlockText(+ GetTextHeight() + Always(m_DIMTVP) * nDir,0,m_BasePoint.x,(m_CurDimPos.y+NewPos.y)/2-DisYByAng,cs2);
	}

	m_CurDimPos = NewPos;
	SetTextAngle(textAngle);

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimAlignMoveTo(CDPoint xy, BOOL bInitSet, CString Str, double nLen)
{
	CDPoint OldDimPos = m_CurDimPos;

	double StartLineLen = (nLen == 0) ? GetTextWidth(Str) : nLen;
	if(bInitSet) m_InitPoint = xy;

	//중심일경우
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1);	// 일단 단 때문에 보류
	if(m_CurDirection==DIM_BOTTOM) 
	{
		ExoLine *= -1;
		ExeLine *= -1;
	}

	//각도에 따른 x변화량 계산
	double ExoXByAng = fabs(ExoLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAngExe = fabs(m_BasePoint.y-xy.y+ExeLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAng = fabs(m_BasePoint.y-xy.y) * tan(ToRadian(m_DIMOBQ));
	CDPoint SttP, EndP, Pos;

	if(Str != "") 
	{			
		SttP  = CDPoint(xy.x-ExoXByAng-StartLineLen, m_BasePoint.y);
		EndP  = CDPoint(xy.x-ExoXByAng,m_BasePoint.y);

		SttP = GetXyRotateVector(m_InitPoint, SttP, m_VAng);
		EndP = GetXyRotateVector(m_InitPoint, EndP, m_VAng);
		LineTo(SttP, EndP);			
		
		double OldTextAng = GetTextAngle();			
		long   oldHor = GetTextAlignHorz();		
		long nDir = (m_bTextDir) ? -1 : 1;
		SetTextAngle(m_VAng.GetAngleDegree());	

		if(nLen==0)
		{
			Pos = CDPoint(xy.x-ExoXByAng - StartLineLen/2,m_BasePoint.y + Always(m_DIMTVP)*nDir);
			Pos = GetXyRotateVector(m_InitPoint, Pos, m_VAng);
			TextOut(Pos.x,Pos.y,Str);												  			
			//BlockText(Pos.x,Pos.y,Str);
		}
		else
		{
			long Horz = GetTextAlignHorz();
			if(nLen==0)
			{
				SetTextAlignHorz(TA_LEFT);
				Pos = CDPoint(xy.x-ExoXByAng - StartLineLen + GetTextHeight(),m_BasePoint.y + Always(m_DIMTVP)*nDir);
			}
			else
			{
				SetTextAlignHorz(TA_CENTER);
				Pos = CDPoint(xy.x-ExoXByAng - StartLineLen/2,m_BasePoint.y + Always(m_DIMTVP)*nDir);
			}
			Pos = GetXyRotateVector(m_InitPoint, Pos, m_VAng);
			TextOut(Pos.x,Pos.y,Str);				
			//BlockText(Pos.x,Pos.y,Str);
			SetTextAlignHorz(Horz);
		}
		
		SetTextAlignHorz(oldHor);
		SetTextAngle(OldTextAng);
	}

	SttP = CDPoint(xy.x-ExoXByAng,xy.y+ExoLine);
	EndP = CDPoint(xy.x+DisXByAngExe,m_BasePoint.y+ExeLine);
	SttP = GetXyRotateVector(m_InitPoint, SttP, m_VAng);
	EndP = GetXyRotateVector(m_InitPoint, EndP, m_VAng);
	// 첫번째 치수선 기입
	LineTo(SttP,EndP);	

	Pos = CDPoint(xy.x+DisXByAng,m_BasePoint.y);
	Pos = GetXyRotateVector(m_InitPoint, Pos, m_VAng);

	BlockDot(Pos.x,Pos.y);
	
	// 시작 포인터 저장
	m_CurDimPos = xy;	

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimAlignLineTo(double xDis,double y,const CString &cs, const CString &cs2, BOOL bOver, double nLen)
{
	CDPoint OldDimPos = m_CurDimPos, NewPos;
	
	double StartLineLen;

	CString str = cs.GetLength() > cs2.GetLength() ? cs: cs2;
	StartLineLen = (nLen == 0) ? GetTextWidth(str) : nLen;

	if(!bOver) StartLineLen = 0;

	NewPos = CDPoint(xDis + m_CurDimPos.x, y);

	//중심일경우
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1);	// 일단 단 때문에 보류
	if(m_CurDirection==DIM_BOTTOM) 
	{
		ExoLine *= -1;
		ExeLine *= -1;
	}

	//각도에 따른 x변화량 계산
	double ExoXByAng = fabs(ExoLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAngExe = fabs(m_BasePoint.y-y+ExeLine) * tan(ToRadian(m_DIMOBQ));
	double DisXByAng = fabs(m_BasePoint.y-y) * tan(ToRadian(m_DIMOBQ));
	CDPoint SttP, EndP, Pos;
	double OldTextAng;

	// 첫번째 치수선 기입
	SttP = CDPoint(NewPos.x-ExoXByAng,y+ExoLine);
	EndP = CDPoint(NewPos.x+DisXByAngExe,m_BasePoint.y+ExeLine);
	SttP = GetXyRotateVector(m_InitPoint, SttP, m_VAng);
	EndP = GetXyRotateVector(m_InitPoint, EndP, m_VAng);
	LineTo(SttP,EndP);	

	Pos = CDPoint(NewPos.x+DisXByAng,m_BasePoint.y);
	Pos = GetXyRotateVector(m_InitPoint, Pos, m_VAng);	
	BlockDot(Pos.x,Pos.y);
	
	SttP = CDPoint(m_CurDimPos.x+DisXByAng,m_BasePoint.y);
	EndP = CDPoint(NewPos.x+DisXByAng+StartLineLen,m_BasePoint.y);
	SttP = GetXyRotateVector(m_InitPoint, SttP, m_VAng);
	EndP = GetXyRotateVector(m_InitPoint, EndP, m_VAng);
	LineTo(SttP, EndP);

	long nDir = (m_bTextDir) ? -1 : 1;	
	OldTextAng = GetTextAngle();
	SetTextAngle(m_VAng.GetAngleDegree());	
	if(bOver)
	{		
		Pos = CDPoint(NewPos.x+DisXByAng+StartLineLen/2, m_BasePoint.y);
		DimTextOut(Pos,cs,m_VAng.GetAngleDegree(),nDir);
		if(!cs2.IsEmpty())
		{
			Pos = CDPoint(NewPos.x+DisXByAng+StartLineLen/2, m_BasePoint.y - GetTextHeight());			
			DimTextOut(Pos,cs2,m_VAng.GetAngleDegree(),nDir*(-1));			
		}
	}
	else
	{
		Pos = CDPoint((m_CurDimPos.x+NewPos.x)/2+DisXByAng, m_BasePoint.y);
		DimTextOut(Pos,cs,m_VAng.GetAngleDegree(),nDir);
		if(!cs2.IsEmpty())
		{
			Pos = CDPoint((m_CurDimPos.x+NewPos.x)/2+DisXByAng, m_BasePoint.y - GetTextHeight());
			DimTextOut(Pos,cs2,m_VAng.GetAngleDegree(),nDir*(-1));
		}
	}

	m_CurDimPos = NewPos;
	SetTextAngle(OldTextAng);

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimMoveTo(CDPoint xy, long nDan, BOOL bDrawMark, CString Str, double nLen, BOOL bExtend, BOOL bArrow)
{
	CDPoint OldDimPos;
	
	if(m_CurDirection == DIM_TOP || m_CurDirection == DIM_BOTTOM)	
	{
		long nDir = (m_CurDirection==DIM_BOTTOM) ? 1 : -1;
		if(nDan > 0)
		{			
			m_BasePoint.y = xy.y - (Always(m_DIMEXO) + Always(m_DIMVLEN) + Always(m_DIMLEVELDIS)*(nDan-1)) * nDir;
			m_nDan = nDan;
		}
		
		if(m_CutExtLine) xy.y = m_BasePoint.y + Always(m_DIMVLEN) * nDir;

		OldDimPos = DimHorMoveTo(xy,Str,nLen,bExtend,bArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMark(0, xy.y, DIM_HOR);		
	}
	else
	{
		long nDir = (m_CurDirection==DIM_RIGHT) ? -1 : 1;
		if(nDan > 0)
		{
			m_BasePoint.x = xy.x - (Always(m_DIMEXO) + Always(m_DIMVLEN) + Always(m_DIMLEVELDIS)*(nDan-1)) * nDir;
			m_nDan = nDan;
		}

		if(m_CutExtLine) xy.x = m_BasePoint.x + Always(m_DIMVLEN) * nDir;

		OldDimPos = DimVerMoveTo(xy,Str,nLen,bExtend,bArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMark(xy.x, 0, DIM_VER);		
	}
	
	return OldDimPos;
}

CDPoint CDimDomyunEx::DimLineTo(double x, double y,const CString &cs,const CString &cs2, BOOL bDrawMark, BOOL bOver, double nLen, BOOL bExtend, BOOL bEdgeArrow)
{
	CDPoint OldDimPos;

	if(m_CurDirection == DIM_TOP || m_CurDirection == DIM_BOTTOM)	
	{
		long nDir = (m_CurDirection==DIM_BOTTOM) ? 1 : -1;
		if(m_CutExtLine) y = m_BasePoint.y + Always(m_DIMVLEN) * nDir;
		OldDimPos = DimHorLineTo(x,y,cs,cs2,bOver,nLen,bExtend,0,bEdgeArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMark(0,y,DIM_HOR);
	}
	else
	{
		long nDir = (m_CurDirection==DIM_RIGHT) ? -1 : 1;
		if(m_CutExtLine) x = m_BasePoint.x + Always(m_DIMVLEN) * nDir;
		OldDimPos = DimVerLineTo(x,y,cs,cs2,bOver,nLen,bExtend,0,bEdgeArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMark(x,0,DIM_VER);
	}

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimLineToPiece(double x, double y, double dAddLen, const CString &cs, const CString &cs2, BOOL bDrawMark, BOOL bOver, double nLen, BOOL bExtend, BOOL bEdgeArrow)
{
	CDPoint OldDimPos;

	if(m_CurDirection == DIM_TOP || m_CurDirection == DIM_BOTTOM)	
	{
		OldDimPos = DimHorLineTo(x,y,cs,cs2,bOver,nLen,bExtend,dAddLen,bEdgeArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMark(0,y,DIM_HOR);

	}
	else
	{
		OldDimPos = DimVerLineTo(x,y,cs,cs2,bOver,nLen,bExtend,dAddLen,bEdgeArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMark(x,0,DIM_VER);
	}

	return OldDimPos;
}

void CDimDomyunEx::DrawSideMark(double x, double y, BOOL bHor)
{
	if(m_nDir == DIR_NONE) return;

	double ExtLine = Always(m_DIMEXO) + Always(3);	
	if(m_CurDirection==DIM_BOTTOM || m_CurDirection==DIM_RIGHT) ExtLine = -(Always(m_DIMEXO) + Always(7));

	CDomyun Dom(this);
	if(bHor)
	{
		CDPoint xy = CDPoint(m_CurDimPos.x + x, y);
		if(m_nDir == DIR_CENTER)
		{
			Dom.LineTo(xy.x+Always(1),xy.y+ExtLine-Always(1),xy.x+Always(1),xy.y+ExtLine+Always(3));
			Dom.LineTo(xy.x-Always(1),xy.y+ExtLine,xy.x-Always(1),xy.y+ExtLine+Always(4));	
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x+Always(1),xy.y+ExtLine+Always(i)-Always(0.5),
				           xy.x-Always(1),xy.y+ExtLine+Always(i)+Always(0.5));
		}
		if(m_nDir == DIR_RIGHT)
		{
			Dom.LineTo(xy.x+Always(1),xy.y+ExtLine,xy.x+Always(1),xy.y+ExtLine+Always(4));
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x+Always(1),xy.y+ExtLine+Always(i)+Always(0.5),xy.x,xy.y+ExtLine+Always(i));
		}
		if(m_nDir == DIR_LEFT)
		{
			Dom.LineTo(xy.x-Always(1),xy.y+ExtLine,xy.x-Always(1),xy.y+ExtLine+Always(4));	
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x-Always(1),xy.y+ExtLine+Always(i)+Always(0.5),xy.x,xy.y+ExtLine+Always(i));
		}
	}
	else
	{
		CDPoint xy = CDPoint(x, m_CurDimPos.y + y);
		if(m_nDir == DIR_CENTER)
		{
			Dom.LineTo(xy.x-ExtLine,xy.y-Always(1),xy.x-ExtLine-Always(4),xy.y-Always(1));
			Dom.LineTo(xy.x-ExtLine+Always(1),xy.y+Always(1),xy.x-ExtLine-Always(3),xy.y+Always(1));			
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x-ExtLine-Always(i)-Always(0.5),xy.y-Always(1),
				           xy.x-ExtLine-Always(i)+Always(0.5),xy.y+Always(1));
		}
		if(m_nDir == DIR_RIGHT)
		{
			Dom.LineTo(xy.x-ExtLine,xy.y-Always(1),xy.x-ExtLine-Always(4),xy.y-Always(1));
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x-ExtLine-Always(i)-Always(0.5),xy.y-Always(1),xy.x-ExtLine-Always(i),xy.y);			
		}
		if(m_nDir == DIR_LEFT)
		{
			Dom.LineTo(xy.x-ExtLine,xy.y+Always(1),xy.x-ExtLine-Always(4),xy.y+Always(1));			
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x-ExtLine-Always(i)-Always(0.5),xy.y+Always(1),xy.x-ExtLine-Always(i),xy.y);						
		}
	}

	if(m_CurDirection==DIM_BOTTOM || m_CurDirection==DIM_LEFT)
	{
		if(bHor)
			Dom.Rotate(ToRadian(GetDimObq()),m_CurDimPos.x,y);
		else		
			Dom.Rotate(ToRadian(GetDimObq()),x,m_CurDimPos.y);
	}
	else
	{
		if(bHor)
			Dom.Rotate(-ToRadian(GetDimObq()),m_CurDimPos.x,y);
		else
			Dom.Rotate(-ToRadian(GetDimObq()),x,m_CurDimPos.y);
	}

	*this << Dom;
}

////////////////////////////////////// Curve Domyun ///////////////////////////////////////////////

CDPoint CDimDomyunEx::DimHorCurveMoveTo(double xSta, double y, long nG, CString Str, double nLen, BOOL bArrow, BOOL bExtend)
{		
	CPlateGirderApp		*pGir = m_pDB->GetGirder(nG);
	CLineInfo		*pLine = pGir->GetLine();	

	CDPoint OldDimPos = m_CurDimPos;
	double StartLineLen = (nLen == 0) ? GetTextWidth(Str) : nLen;

	//중심일경우
	int nDir = 1;
	
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);

	//if(m_nDir == DIR_CENTER) ExtLine = Always(1);
	//if(m_CurDirection==DIM_TOP) ExtLine *= -1;
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1) * nDir;	
	if(m_CurDirection==DIM_BOTTOM) 
	{
		ExoLine *= -1;
		ExeLine *= -1;
	}

	if(Str != "") 
	{
		CDPoint LineXy = Getxy(xSta - StartLineLen, y + ExoLine, 90, nG);
		CDPoint LineHd = Getxy(xSta,y + ExoLine,90,nG);		

		double dLen = pGir->GetStationByActLength(xSta, StartLineLen, y-pGir->m_dGirderCenter) - xSta;		

		DrawLS(this,nG,xSta - dLen,xSta,m_BaseStaionPoint.y);
		
		double textAngle = GetTextAngle();			
		long   oldHor = GetTextAlignHorz();
		long nTextDir = (m_bTextDir) ? 1 : -1;
		double Angle = pGir->GetLine()->GetAngleAzimuth(xSta - StartLineLen/2).GetAngleRadian();// + ConstPi * nTextDir;

		SetTextAngle(ToDegree(Angle));
		if(nLen==0)
		{
			SetTextAlignHorz(TA_CENTER);
			TextOut(Getxy(xSta - dLen/2,m_BaseStaionPoint.y + Always(m_DIMTVP)*nDir*nTextDir,90,nG),Str);				
		}
		else
		{
			SetTextAlignHorz(TA_LEFT);
			TextOut(Getxy(xSta - dLen + Always(1),m_BaseStaionPoint.y + Always(m_DIMTVP)*nDir*nTextDir,90,nG),Str);				
		}
		//CDPoint Pos = Getxy(xSta - StartLineLen + GetTextHeight(),m_BaseStaionPoint.y + Always(m_DIMTVP)*nDir*nTextDir,90,nG);
		//BlockText(Pos.x,Pos.y,Str);
		SetTextAngle(textAngle);	
		SetTextAlignHorz(oldHor);
	}

	CDPoint CurveXy = Getxy(xSta,y + ExoLine,90,nG);
	CDPoint CurveHd = Getxy(xSta,m_BaseStaionPoint.y,90,nG);
	CDPoint CurveEd = Getxy(xSta,m_BaseStaionPoint.y + ExeLine,90,nG);
		
	if(!bArrow)
	{
		if(!bExtend)
		{
			LineTo(CurveXy.x,CurveXy.y,CurveEd.x,CurveEd.y);				
		}
	}
	else 
	{
		double LastAngle = pGir->GetLine()->GetAngleAzimuth(xSta).GetAngleRadian();		
		BlockDot(CurveHd.x,CurveHd.y, LastAngle);		
	}		

	// 시작 포인터 저장
	m_CurDimPos = CDPoint(xSta,y);
  
	return OldDimPos;	
}

CDPoint CDimDomyunEx::DimHorCurveLineTo(double xDis,double y,const CString &cs,const CString &cs2,long nG,BOOL bExtend,BOOL bOver,double nLen, BOOL bEdgeArrow)
{	
	if(xDis==0) return m_CurDimPos;
	CPlateGirderApp		*pGir = m_pDB->GetGirder(nG);
	CLineInfo		*pLine = pGir->GetLine();	

	CDPoint OldDimPos = m_CurDimPos, NewPos;

	CString csMax = cs.GetLength() > cs2.GetLength() ? cs : cs2;
	double StartLineLen = (nLen == 0) ? GetTextWidth(csMax) : nLen;

	NewPos = CDPoint(xDis + m_CurDimPos.x, y);

	//중심일경우
	//int nDir = 1;
	
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	double ExeLine = Always(m_DIMEXE);

	if(m_CurDirection==DIM_BOTTOM) 
	{
		ExoLine *= -1;
		ExeLine *= -1;
	}
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1) * nDir;		
	CDPoint CurveXy = Getxy(NewPos.x,y + ExoLine,90,nG);
	CDPoint StartHd = Getxy(m_CurDimPos.x,m_BaseStaionPoint.y,90,nG);	
	CDPoint CurveHd = Getxy(NewPos.x,m_BaseStaionPoint.y,90,nG);	
	CDPoint CurveEd = Getxy(NewPos.x,m_BaseStaionPoint.y + ExeLine,90,nG);

	// 첫번째 치수선 기입		
	if(!bExtend)
	{				
		if(!bEdgeArrow)
		{
			LineTo(CurveXy.x,CurveXy.y,CurveEd.x,CurveEd.y);
			if(m_CurDimPos.x < NewPos.x)
			{
				BlockDot(StartHd.x,StartHd.y, pGir->GetLine()->GetAngleAzimuth(m_CurDimPos.x).GetAngleRadian(),TRUE);	
				BlockDot(CurveHd.x,CurveHd.y, pGir->GetLine()->GetAngleAzimuth(NewPos.x).GetAngleRadian(),FALSE);
			}
			else
			{
				BlockDot(StartHd.x,StartHd.y, pGir->GetLine()->GetAngleAzimuth(m_CurDimPos.x).GetAngleRadian(),FALSE);	
				BlockDot(CurveHd.x,CurveHd.y, pGir->GetLine()->GetAngleAzimuth(NewPos.x).GetAngleRadian(),TRUE);
			}
		}
		else 
		{
			double LastAngle = pGir->GetLine()->GetAngleAzimuth(NewPos.x).GetAngleRadian();		
			Arrow(CurveHd,LastAngle);
		}						
	}
	else if(bEdgeArrow)
	{				
		double LastAngle = pGir->GetLine()->GetAngleAzimuth(NewPos.x).GetAngleRadian();		
		Arrow(CurveHd,LastAngle);
	}
	else
	{
		if(m_CurDimPos.x < NewPos.x)
			BlockDot(StartHd.x,StartHd.y, pGir->GetLine()->GetAngleAzimuth(m_CurDimPos.x).GetAngleRadian(),TRUE);	
		else
			BlockDot(StartHd.x,StartHd.y, pGir->GetLine()->GetAngleAzimuth(m_CurDimPos.x).GetAngleRadian(),FALSE);	
	}
	if(bOver)	
	{				
		//NewPos.x += StartLineLen;		
		NewPos.x += pGir->GetStationByActLength(m_CurDimPos.x, StartLineLen, y-pGir->m_dGirderCenter) - m_CurDimPos.x;		
	}

	DrawLS(this,nG,m_CurDimPos.x,NewPos.x,m_BaseStaionPoint.y);

	double textAngle = GetTextAngle();	
	double MiddleSta = (m_CurDimPos.x+NewPos.x)/2;
	double Angle = pGir->GetLine()->GetAngleAzimuth(MiddleSta).GetAngleRadian();
	long nTDir = (m_bTextDir) ? -1 : 1;	
	long oldHor = GetTextAlignHorz();

	SetTextAngle(ToDegree(Angle)+textAngle);	
	CDPoint  TextPos1, TextPos2;

	if(!bOver)
	{
		TextPos1 = Getxy(MiddleSta,m_BaseStaionPoint.y - Always(m_DIMTVP)*nTDir,90,nG);
		TextPos2 = Getxy(MiddleSta,m_BaseStaionPoint.y + GetTextHeight() + Always(m_DIMTVP)*nTDir,90,nG);	
	}
	else
	{
		SetTextAlignHorz(TA_CENTER);
		TextPos1 = Getxy(MiddleSta,m_BaseStaionPoint.y - Always(m_DIMTVP)*nTDir,90,nG);
		TextPos2 = Getxy(MiddleSta,m_BaseStaionPoint.y + GetTextHeight() + Always(m_DIMTVP)*nTDir,90,nG);	
	}

	TextOut(TextPos1,cs);		
	if(!cs2.IsEmpty())	TextOut(TextPos2,cs2);			

	SetTextAngle(textAngle);	
	SetTextAlignHorz(oldHor);

	m_CurDimPos = NewPos;

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimVerCurveMoveTo(double xSta, double y, long nG, CString Str, double nLen,BOOL bArrow, BOOL bExtend, BOOL bOver)
{	
	CPlateGirderApp	*pGir = m_pDB->GetGirder(nG);
	CLineInfo		*pLine = pGir->GetLine();	

	CDPoint OldDimPos = m_CurDimPos;
	
	if(bOver) nLen = 0;
	double StartLineLen = (nLen == 0) ? GetTextWidth(Str) : nLen;
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	//double ExeLine = Always(m_DIMEXE);	
	//double ExtLine = 0;

	int nDir = 1;
	if(m_CurDirection==DIM_LEFT) nDir = -1;
	//if(m_nDir == DIR_CENTER) ExtLine = Always(1);
	//if(m_CurDirection==DIM_RIGHT) ExtLine *= -1;

	CDPoint CurveXy = Getxy(xSta /*+ ExtLine * nDir*/,y,90,nG);
	CDPoint CurveHd = Getxy(xSta + m_BaseStaionPoint.x, y, 90,nG);

	if(Str != "") 
	{
		CDPoint LineOv = Getxy(xSta + m_BaseStaionPoint.x, y - StartLineLen - 100, 90, nG);
		CDPoint LineXy = Getxy(xSta + m_BaseStaionPoint.x, y + StartLineLen, 90, nG);
		CDPoint LineHd = Getxy(xSta + m_BaseStaionPoint.x, y,90,nG);

		if(bOver)
			LineTo(LineOv,LineXy);
		LineTo(LineXy,LineHd);
		
		double textAngle = GetTextAngle();			
		long   oldHor = GetTextAlignHorz();
		long nTextDir = (m_bTextDir) ? 1 : -1;
		double Angle = pGir->GetLine()->GetAngleAzimuth(xSta - StartLineLen/2).GetAngleRadian() - ConstPi/2 * nTextDir;

		SetTextAngle(ToDegree(Angle));

		if(bOver)
		{
			SetTextAlignHorz(TA_LEFT);
			TextOut(Getxy(xSta + m_BaseStaionPoint.x + Always(m_DIMTVP)*nTextDir,y + StartLineLen - GetTextWidth(Str) - 100,90,nG),Str);
		}
		else if(nLen==0)
		{
			SetTextAlignHorz(TA_CENTER);
			TextOut(Getxy(xSta + m_BaseStaionPoint.x + Always(m_DIMTVP)*nTextDir,y + StartLineLen/2,90,nG),Str);				
		}
		else
		{
			SetTextAlignHorz(TA_LEFT);
			TextOut(Getxy(xSta + m_BaseStaionPoint.x + Always(m_DIMTVP)*nTextDir,y + StartLineLen - GetTextHeight(),90,nG),Str);
		}
		//CDPoint Pos = Getxy(xSta + m_BaseStaionPoint.x + Always(m_DIMTVP)*nTextDir,y + StartLineLen - GetTextHeight(),90,nG);
		//BlockText(Pos.x,Pos.y,Str);
		SetTextAngle(textAngle);	
		SetTextAlignHorz(oldHor);
	}

	if(!bArrow)
	{
		if(!bExtend)
		{
			DrawLS(this,nG,xSta + ExoLine*nDir,xSta + m_BaseStaionPoint.x,y);				
		}
	}
	else
	{
		double LastAngle = pGir->GetLine()->GetAngleAzimuth(xSta).GetAngleRadian() - ConstPi/2;		
		Arrow(CurveHd,LastAngle);
	}

	// 시작 포인터 저장
	m_CurDimPos = CDPoint(xSta,y);

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimVerCurveLineTo(double x, double yDis,const CString &cs,const CString &cs2,long nG,BOOL bOver,double nLen, BOOL bEdgeArrow)
{	
	CPlateGirderApp		*pGir = m_pDB->GetGirder(nG);
	CLineInfo		*pLine = pGir->GetLine();	

	CDPoint OldDimPos = m_CurDimPos, NewPos;

	CString csMax = cs.GetLength() > cs2.GetLength() ? cs : cs2;
	double StartLineLen = (nLen == 0) ? GetTextWidth(csMax) : nLen;

	NewPos = CDPoint(x, m_CurDimPos.y+yDis);

	//중심일경우
	int nDir = 1;
	
	double ExoLine = m_CutExtLine ? 0 : Always(m_DIMEXO);
	
	if(m_CurDirection==DIM_LEFT) nDir = -1;
	
	CDPoint CurveXy = Getxy(x + m_BaseStaionPoint.x, m_CurDimPos.y,90,nG);	
	CDPoint CurveHd = Getxy(x + m_BaseStaionPoint.x, NewPos.y,90,nG);	

	// 첫번째 치수선 기입
	DrawLS(this,nG,x + ExoLine * nDir,x + m_BaseStaionPoint.x,NewPos.y);
	double LastAngle = pGir->GetLine()->GetAngleAzimuth(NewPos.x).GetAngleRadian() - ConstPi/2;		
	if(m_CurDimPos.y < NewPos.y)
	{
		BlockDot(CurveXy.x,CurveXy.y,LastAngle,FALSE);
		BlockDot(CurveHd.x,CurveHd.y,LastAngle,TRUE);
	}
	else
	{
		BlockDot(CurveXy.x,CurveXy.y,LastAngle,TRUE);
		BlockDot(CurveHd.x,CurveHd.y,LastAngle,FALSE);
	}
	
	if(bOver)
		CurveHd = Getxy(x + m_BaseStaionPoint.x, NewPos.y + StartLineLen*2,90,nG);
	LineTo(CurveXy.x,CurveXy.y,CurveHd.x,CurveHd.y);

	double textAngle = GetTextAngle();	
	long nTextDir = (m_bTextDir) ? 1 : -1;
	double Angle = pGir->GetLine()->GetAngleAzimuth(x).GetAngleRadian() - ConstPi/2 * nTextDir;
	long oldHor = GetTextAlignHorz();

	SetTextAngle(ToDegree(Angle));
	CDPoint  TextPos1, TextPos2;
	if(!bOver)
	{
		TextPos1 = Getxy(x + m_BaseStaionPoint.x + Always(m_DIMTVP)*nTextDir,m_CurDimPos.y + yDis/2,90,nG);
		TextPos2 = Getxy(x + m_BaseStaionPoint.x - Always(m_DIMTVP)*nTextDir + GetTextHeight(),m_CurDimPos.y + yDis/2,90,nG);	
	}
	else
	{
//		SetTextAlignHorz();
		TextPos1 = Getxy(x + m_BaseStaionPoint.x + Always(m_DIMTVP)*nTextDir,NewPos.y + StartLineLen,90,nG);
		TextPos2 = Getxy(x + m_BaseStaionPoint.x - Always(m_DIMTVP)*nTextDir - GetTextHeight(),NewPos.y + StartLineLen,90,nG);	
	}

	TextOut(TextPos1,cs);		
	//BlockText(TextPos1.x,TextPos1.y,cs);
	if(!cs2.IsEmpty()) TextOut(TextPos2,cs2);		
		//BlockText(TextPos2.x,TextPos2.y,cs2);

	SetTextAngle(textAngle);	
	SetTextAlignHorz(oldHor);
	m_CurDimPos = NewPos;

	return OldDimPos;
}

CDPoint CDimDomyunEx::DimCurveMoveTo(double xSta, double y, long nG, BOOL bDrawMark, CString Str, double nLen, BOOL bArrow, BOOL bExtend, BOOL bOver)
{
	CDPoint OldDimPos;
	
	if(m_CurDirection == DIM_TOP || m_CurDirection == DIM_BOTTOM)	
	{		
		long nDir = (m_CurDirection == DIM_TOP) ? -1 : 1;
		if(m_CutExtLine) y = m_BaseStaionPoint.y + Always(m_DIMLEVELDIS) * nDir;
		OldDimPos = DimHorCurveMoveTo(xSta,y,nG, Str, nLen, bArrow, bExtend);
		if(bDrawMark && !m_CutExtLine) DrawSideMarkCurve(0,y,nG,DIM_HOR);
	}
	else
	{
		long nDir = (m_CurDirection == DIM_LEFT) ? -1 : 1;		
		if(m_CutExtLine) 
		{
			m_BaseStaionPoint.y = m_BaseStaionPoint.x;
			xSta += m_BaseStaionPoint.x - Always(m_DIMLEVELDIS) * nDir;			
			m_BaseStaionPoint.x = Always(m_DIMLEVELDIS) * nDir;			
		}
		OldDimPos = DimVerCurveMoveTo(xSta,y,nG, Str, nLen, bArrow, bExtend, bOver);
		if(bDrawMark && !m_CutExtLine) DrawSideMark(xSta, 0, DIM_VER);
	}
	
	return OldDimPos;
}

CDPoint CDimDomyunEx::DimCurveLineTo(double x, double y,const CString &cs,const CString &cs2, long nG, BOOL bDrawMark, BOOL bExtend, BOOL bOver, double nLen, BOOL bEdgeArrow)
{
	CDPoint OldDimPos;

	if(m_CurDirection == DIM_TOP || m_CurDirection == DIM_BOTTOM)	
	{
		long nDir = (m_CurDirection == DIM_TOP) ? -1 : 1;
		if(m_CutExtLine) y = m_BaseStaionPoint.y + Always(m_DIMLEVELDIS) * nDir;
		OldDimPos = DimHorCurveLineTo(x,y,cs,cs2,nG,bExtend,bOver,nLen,bEdgeArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMarkCurve(0,y,nG,DIM_HOR);
	}
	else
	{
		long nDir = (m_CurDirection == DIM_LEFT) ? -1 : 1;
		if(m_CutExtLine) 
		{			
			x += m_BaseStaionPoint.y - Always(m_DIMLEVELDIS) * nDir;
			m_BaseStaionPoint.x = Always(m_DIMLEVELDIS) * nDir;
		}
		OldDimPos = DimVerCurveLineTo(x,y,cs,cs2,nG,bOver,nLen,bEdgeArrow);
		if(bDrawMark && !m_CutExtLine) DrawSideMarkCurve(x,0,nG,DIM_VER);
	}

	return OldDimPos;
}

void CDimDomyunEx::DrawSideMarkCurve(double x, double y, long nG, BOOL bHor)
{	
	CPlateGirderApp		*pGir = m_pDB->GetGirder(nG);
	CDPoint xy;
	double Angle;

	if(m_nDir == DIR_NONE) return;

	double ExtLine = (m_SideMarkHeight==0) ? Always(10) : m_SideMarkHeight;	
	if(m_CurDirection==DIM_BOTTOM || m_CurDirection==DIM_RIGHT) ExtLine *= -1;	

	CDomyun Dom(this);
	if(bHor)
	{		
		xy = Getxy(x + m_CurDimPos.x, y,90,nG);
		Angle = pGir->GetLine()->GetAngleAzimuth(x + m_CurDimPos.x).GetAngleRadian();	
		if(m_nDir == DIR_CENTER)
		{
			Dom.LineTo(xy.x+Always(1),xy.y-ExtLine-Always(1),xy.x+Always(1),xy.y-ExtLine+Always(3));
			Dom.LineTo(xy.x-Always(1),xy.y-ExtLine,xy.x-Always(1),xy.y-ExtLine+Always(4));	
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x+Always(1),xy.y-ExtLine+Always(i)-Always(0.5),
				           xy.x-Always(1),xy.y-ExtLine+Always(i)+Always(0.5));
		}
		if(m_nDir == DIR_RIGHT)
		{
			Dom.LineTo(xy.x+Always(1),xy.y-ExtLine,xy.x+Always(1),xy.y-ExtLine+Always(4));			
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x+Always(1),xy.y-ExtLine+Always(i)+Always(0.5),xy.x,xy.y-ExtLine+Always(i));
		}
		else
		{
			Dom.LineTo(xy.x-Always(1),xy.y-ExtLine,xy.x-Always(1),xy.y-ExtLine+Always(4));	
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x-Always(1),xy.y-ExtLine+Always(i)+Always(0.5),xy.x,xy.y-ExtLine+Always(i));
		}
	}
	else
	{
		xy = Getxy(x, y + m_CurDimPos.y, 90,nG);		
		Angle = pGir->GetLine()->GetAngleAzimuth(x).GetAngleRadian();	
		if(m_nDir == DIR_RIGHT)
		{
			Dom.LineTo(xy.x-ExtLine,xy.y-Always(1),xy.x-ExtLine-Always(4),xy.y-Always(1));			
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x-ExtLine-Always(i)-Always(0.5),xy.y-Always(1),xy.x-ExtLine-Always(i),xy.y);
		}
		else
		{
			Dom.LineTo(xy.x-ExtLine,xy.y+Always(1),xy.x-ExtLine-Always(4),xy.y+Always(1));			
			for(long i=0;i<4;i++)
				Dom.LineTo(xy.x-ExtLine-Always(i)-Always(0.5),xy.y+Always(1),xy.x-ExtLine-Always(i),xy.y);			
		}
	}
	
	Dom.Rotate(Angle,xy.x,xy.y);
	*this << Dom;
}

void CDimDomyunEx::DimArray(CDPointArray &Arr, BOOL bUpper, double nDan, BOOL bTextRot, BOOL bTotal, double dLeft, CDPoint LeftP, double dRight, CDPoint RightP)
{
	CDPoint BaseP1, UpperP1, ExtP1;
	CDPoint BaseP2, UpperP2, ExtP2;

	double dPos = bUpper ? -1.0 : 1.0;	
	double nUpper = 1;
	double dLen = GetDimVLen() + GetDimLevelDis() * (nDan-1.0);

	double dTotal = 0;

	if(m_CurDirection == DIM_BOTTOM) nUpper = -1;
	else						 	 nUpper = 1;
	long n = 0;
	if(bTotal)	
	{
		BOOL bDrawText = FALSE;
		double dTotCheck = 0;
		for(n=0;n<Arr.GetSize()-1;n++)		dTotal += ~(Arr.GetAt(n+1) - Arr.GetAt(n));	
		for(n=0;n<Arr.GetSize()-1;n++)
		{
			CDPoint Po1 = Arr.GetAt(n);
			CDPoint Pre1 = (n == 0) ? Arr.GetAt(n) : Arr.GetAt(n-1);
			CDPoint Next1 = (n+1 < Arr.GetSize()) ? Arr.GetAt(n+1) : Arr.GetAt(Arr.GetUpperBound());
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			CDPoint Po2 = Arr.GetAt(n+1);
			CDPoint Pre2 = Arr.GetAt(n);
			CDPoint Next2 = (n+2 < Arr.GetSize()) ? Arr.GetAt(n+2) : Arr.GetAt(n+1);
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));
			
			if(n==0 && dLeft !=0) 
			{
				CDPoint vVec = (BaseP2 - BaseP1).Unit();
				CDPoint BaseP3 = BaseP1 - vVec * dLeft;
				CDPoint UpperP3 = UpperP1 - vVec * dLeft;
				CDPoint ExtP3  = ExtP1 - vVec * dLeft;
				LineTo(BaseP3,ExtP3); BlockDot(UpperP3.x,UpperP3.y);								
				LineTo(UpperP3,UpperP1);
				
				CDPoint vPoVec = (Po2-Po1).Unit();
				CDPoint NewP;
				GetXyMatchLineAndLine(Po1,vPoVec,LeftP,vPoVec.Rotate90(),NewP);

				CDPoint MiddleP = (UpperP3+UpperP1)/2;
				CString szLen = ~(Po1-NewP) == 0 ? "0" : COMMA(~(Po1-NewP));
				CDPoint vVecT = (UpperP1 - UpperP3).Unit();
				SetTextAngle(ToDegree(vVecT.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				BlockText(Always(m_DIMTVP) * sin(vVecT.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVecT.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, szLen);		

				DrawExtLine(LeftP,Po1,Po2,dLeft,TRUE,bUpper);							
			}
			else if(n==Arr.GetSize()-2 && dRight!=0)
			{
				CDPoint vVec = (BaseP2 - BaseP1).Unit();
				CDPoint BaseP3 = BaseP2 + vVec * dRight;
				CDPoint UpperP3 = UpperP2 + vVec * dRight;
				CDPoint ExtP3  = ExtP2 + vVec * dRight;				
				LineTo(BaseP3,ExtP3); BlockDot(UpperP3.x,UpperP3.y);								
				LineTo(UpperP2,UpperP3);

				CDPoint vPoVec = (Po2-Po1).Unit();
				CDPoint NewP;
				GetXyMatchLineAndLine(Po2,vPoVec,RightP,vPoVec.Rotate90(),NewP);

				CDPoint MiddleP = (UpperP3+UpperP2)/2;
				CString szLen = ~(NewP-Po2) == 0 ? "0" : COMMA(~(NewP-Po2));
				CDPoint vVecT = (UpperP3 - UpperP2).Unit();
				SetTextAngle(ToDegree(vVecT.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				BlockText(Always(m_DIMTVP) * sin(vVecT.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVecT.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, szLen);		

				DrawExtLine(RightP,Po2,Po1,dRight,FALSE,bUpper);				
			}						
			if(n==0)	
			{ 
				LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			}
			if(n==Arr.GetSize()-2)	
			{
				LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			}
			LineTo(UpperP1,UpperP2);
			dTotCheck += ~(Arr.GetAt(n+1) - Arr.GetAt(n));
			if(dTotCheck > dTotal / 2 && !bDrawText)
			{
				double dL = dTotCheck - dTotal / 2;
				CDPoint MiddleP = UpperP2 - (UpperP2 - UpperP1).Unit() * dL;
				CDPoint vVec = (UpperP2 - UpperP1).Unit();
				SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, COMMA(dTotal));		
				bDrawText = TRUE;
			}
		}
	}
	else
	{
		for(long n=0;n<Arr.GetSize()-1;n++)
		{
			CDPoint Po1 = Arr.GetAt(n);
			CDPoint Pre1 = (n == 0) ? Arr.GetAt(n) : Arr.GetAt(n-1);
			CDPoint Next1 = (n+1 < Arr.GetSize()) ? Arr.GetAt(n+1) : Arr.GetAt(Arr.GetUpperBound());
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			CDPoint Po2 = Arr.GetAt(n+1);
			CDPoint Pre2 = Arr.GetAt(n);
			CDPoint Next2 = (n+2 < Arr.GetSize()) ? Arr.GetAt(n+2) : Arr.GetAt(n+1);
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			if(n==0 && dLeft !=0) 
			{
				CDPoint vVec = (BaseP2 - BaseP1).Unit();
				CDPoint BaseP3 = BaseP1 - vVec * dLeft;
				CDPoint UpperP3 = UpperP1 - vVec * dLeft;
				CDPoint ExtP3  = ExtP1 - vVec * dLeft;
				LineTo(BaseP3,ExtP3); BlockDot(UpperP3.x,UpperP3.y);				
				LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
				LineTo(UpperP3,UpperP2);
				
				CDPoint vPoVec = (Po2-Po1).Unit();
				CDPoint NewP;
				GetXyMatchLineAndLine(Po1,vPoVec,LeftP,vPoVec.Rotate90(),NewP);

				CDPoint MiddleP = (UpperP3+UpperP1)/2;
				CString szLen = ~(Po1-NewP) == 0 ? "0" : COMMA(~(Po1-NewP));
				CDPoint vVecT = (UpperP3 - UpperP1).Unit();
				SetTextAngle(ToDegree(vVecT.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				BlockText(Always(m_DIMTVP) * sin(vVecT.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVecT.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, szLen);						

				DrawExtLine(LeftP,Po1,Po2,dLeft,TRUE,bUpper);
			}
			else if(n==Arr.GetSize()-2 && dRight!=0)
			{
				CDPoint vVec = (BaseP2 - BaseP1).Unit();
				CDPoint BaseP3 = BaseP2 + vVec * dRight;
				CDPoint UpperP3 = UpperP2 + vVec * dRight;
				CDPoint ExtP3  = ExtP2 + vVec * dRight;
				LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
				LineTo(BaseP3,ExtP3); BlockDot(UpperP3.x,UpperP3.y);								
				LineTo(UpperP1,UpperP3);

				CDPoint vPoVec = (Po2-Po1).Unit();
				CDPoint NewP;
				GetXyMatchLineAndLine(Po2,vPoVec,RightP,vPoVec.Rotate90(),NewP);

				CDPoint MiddleP = (UpperP3+UpperP1)/2;
				CString szLen = ~(NewP-Po2) == 0 ? "0" : COMMA(~(NewP-Po2));
				CDPoint vVecT = (UpperP1 - UpperP3).Unit();
				SetTextAngle(ToDegree(vVecT.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				BlockText(Always(m_DIMTVP) * sin(vVecT.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVecT.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, szLen);						

				DrawExtLine(RightP,Po1,Po2,dRight,FALSE,bUpper);
			}
			LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			LineTo(UpperP1,UpperP2);
			CDPoint MiddleP = (UpperP1+UpperP2)/2;
			CDPoint vVec = (UpperP2 - UpperP1).Unit();
			SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
			BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, COMMA(~(Po1-Po2)));		
		}
	}
}

void CDimDomyunEx::DimTextOut(CDPoint xy, CString Str, double RotAng, long nDir, long nAlign)
{
	double OldTextAng = GetTextAngle();
	SetTextAngle(RotAng);
	SetTextAlignHorz(nAlign);
	
	CString ftext = Str;
	CStringArray findStr;
	int FindPoint=0,i,TextNum;
	double AddSpace=0;
	
	while(FindPoint!=-1) {
		FindPoint = ftext.Find("\n",0);
		if(FindPoint!=-1) {
			findStr.Add(ftext.Left(FindPoint));
			ftext = ftext.Mid(FindPoint+1);
		} else {
			if(ftext.GetLength() != 0) findStr.Add(ftext);
		}
	}
	TextNum = findStr.GetSize() - 1;	
	for(i=0;i<findStr.GetSize();i++) 
	{		
		if(findStr.GetAt(TextNum-i).Find("(")!=-1) AddSpace += Always(0.3);
		CDPoint Pos = CDPoint(xy.x,xy.y+(Always(GetDimTvp())+GetTextHeight()*i+AddSpace)*nDir);
		Pos = GetXyRotateVector(m_InitPoint, Pos, m_VAng);
		TextOut(Pos.x,Pos.y,findStr.GetAt(TextNum-i));
		//BlockText(Pos.x,Pos.y,findStr.GetAt(TextNum-i));
	}
	SetTextAngle(OldTextAng);
}

// sta  : 도로중심 스테이션
// dist : 도로중심에서 이격된거리
CDPoint CDimDomyunEx::Getxy(long nG, double sta, double dist)
{
	CPlateGirderApp*	 pGir = m_pDB->GetGirder(nG);

	CDPoint po = pGir->GetLine()->GetXyLineDis(sta,dist);

	return po;
}

//
CDPoint CDimDomyunEx::Getxy(double DisSTA, double H, double Angle,long nG)
{
 	CDPoint Point;

	CPlateGirderApp*	 pGir = m_pDB->GetGirder(nG);

	Point = pGir->GetLine()->GetXyLineDis(DisSTA,H);

	return Point;
}

void CDimDomyunEx::DrawLS(CDomyun* p, long nG, double sttSta, double endSta, double dist, double RepLen)
{ 		
	m_pDB->GetGirder(nG)->GetLine()->DrawLS(p,sttSta, endSta, dist ,RepLen);
}

void CDimDomyunEx::BlockDot(double x, double y, double dAng, BOOL bStart)
{	
	if(m_bBlock)
	{
		CString BlockName = "DOT0";
		long nID = 0;
		CDomyun *pDom = new CDomyun(this);	
		if(m_nDimDotValue==0)
		{
			pDom->Circle(0,0,Always(m_ArrowHeadSize/2),TRUE);
		}
		else if(m_nDimDotValue==1)
		{
			// 화살표를 반대방향으로 그리기
			double Ang = bStart ? ConstPi : 0;			
			Arrow(x,y,Ang+dAng);			
		}
		else
		{
			pDom->Circle(0,0,Always(m_ArrowHeadSize/2),FALSE);
		}

		if(m_pMainDom)
		{
			while(m_pMainDom->IsExistBlock(BlockName) || IsExistBlock(BlockName))
			{
				nID ++;
				BlockName.Format("DOT%d",nID);
			}
		}
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(x,y));		
	}
	else				
	{
		if(m_nDimDotValue==0)
		{
			Circle(x,y,Always(m_ArrowHeadSize/2),TRUE);	
		}
		else if(m_nDimDotValue==1)
		{
			// 화살표를 반대방향으로 그리기
			double Ang = bStart ? ConstPi : 0;				
			Arrow(x,y,Ang+dAng);		
		}
		else
		{
			Circle(x,y,Always(m_ArrowHeadSize/2),FALSE);
		}
	}
}

void CDimDomyunEx::BlockText(double DisX, double DisY, double x, double y, CString Text)
{
	if(m_bBlock)
	{
		CString BlockName = "TEXT0";
		long nID = 0;
		CDomyun *pDom = new CDomyun(this);	
		pDom->TextOut(DisX,DisY,Text);
		if(m_pMainDom)
		{
			while(m_pMainDom->IsExistBlock(BlockName) || IsExistBlock(BlockName))
			{
				nID ++;
				BlockName.Format("TEXT%d",nID);
			}
		}
		pDom->SetBlockName(BlockName);
		AddBlockArr(pDom);				
		Insert(BlockName,CDPoint(x,y));		
	}
	else				
		TextOut(DisX+x,DisY+y,Text);
}

void CDimDomyunEx::DrawExtLine(CDPoint BaseP, CDPoint vPo1, CDPoint vPo2, double dLen, BOOL bLeft, BOOL bUpper)
{
	CDPoint vBaseP = (BaseP - vPo1).Unit();
	CDPoint vAng = (vPo2 - vPo1).Unit();
	double dPos = Always(10);
	double Angle = bLeft ? 90 : -90;
	if(bUpper) Angle *= -1;
	
	Arc(BaseP - vBaseP * dPos, dPos, vBaseP.GetAngleDegree(),Angle);
	LineTo(BaseP - vAng * dLen - vBaseP * dPos, vPo1 - vAng * dLen - vBaseP * Always(GetDimExo()));
}

void CDimDomyunEx::DimPieceObj(CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace, BOOL bTextRot, BOOL bTotal, BOOL bMove, BOOL bFlgUpper)
{
	CDPoint BaseP1, UpperP1, ExtP1;
	CDPoint BaseP2, UpperP2, ExtP2;

	long nStt = nFace < 0 ? 0 : pObj->GetSttFace(nFace);
	long nEnd = nFace < 0 ? pObj->GetUpperBound() : pObj->GetEndFace(nFace);
	double dPos = bUpper ? -1.0 : 1.0;	
	double nUpper = 1;
	double dLen = GetDimVLen() + GetDimLevelDis() * (nDan-1.0);	

	if(m_CurDirection == DIM_BOTTOM) nUpper = -1;
	else						 	 nUpper = 1;

	if(bTotal)	
	{
		BOOL bDrawText = FALSE;
		long nPos = nStt;		
		long nFixPos;
		double dTotal = pObj->GetDistance(nStt,nEnd);
		double dTotCheck = 0;
		while(nPos < nEnd)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,type));
			CDPoint Next1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,type));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			nFixPos =  pObj->GetNextIdx(nPos, type);

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,type));
			CDPoint Next2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,type));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			if(nPos == nStt)
			{
				LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			}
			if(nPos == pObj->GetPrevIdx(nEnd,type))
			{
				LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			}
			LineTo(UpperP1,UpperP2);
			dTotCheck += pObj->GetDistance(nPos, nFixPos);
			if(dTotCheck > dTotal / 2 && !bDrawText)
			{
				double dL = dTotCheck - dTotal / 2;
				CDPoint MiddleP = UpperP1 - (UpperP2 - UpperP1).Unit() * dL/2;
				CDPoint vVec = (UpperP2 - UpperP1).Unit();
				SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				CString strDist;
				if(bMove)
					strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nStt,nEnd)), pObj->GetFlangeLMove(nStt,nEnd,bFlgUpper));
				else
					strDist = COMMA(pObj->GetDistance(nStt,nEnd));
				BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		
				bDrawText = TRUE;
			}			

			nPos = pObj->GetNextIdx(nPos, type);
		}			
	}
	else
	{
		long nPos = nStt;		
		long nFixPos;
		while(nPos < nEnd)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,type));
			CDPoint Next1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,type));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			nFixPos =  pObj->GetNextIdx(nPos, type);

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,type));
			CDPoint Next2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,type));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			LineTo(UpperP1,UpperP2);
			CDPoint MiddleP = (UpperP1+UpperP2)/2;
			CDPoint vVec = (UpperP2 - UpperP1).Unit();
			SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
			CString strDist;
			if(bMove)
				strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nPos,nFixPos)), pObj->GetFlangeLMove(nPos,nFixPos,bFlgUpper));
			else
				strDist = COMMA(pObj->GetDistance(nPos,nFixPos));
			BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		

			nPos = pObj->GetNextIdx(nPos, type);						
		}		
	}	
}

void CDimDomyunEx::DimPieceObjOnBx(CPlatePieceObject *pObj, __int64 type, P_TYPE ptype, BOOL bUpper, double nDan, long nFace, BOOL bTextRot, BOOL bTotal, BOOL bMove, BOOL bFlgUpper)
{
	CDPoint BaseP1, UpperP1, ExtP1;
	CDPoint BaseP2, UpperP2, ExtP2;

	long nStt = nFace < 0 ? 0 : pObj->GetSttFace(nFace);
	long nEnd = nFace < 0 ? pObj->GetUpperBound() : pObj->GetEndFace(nFace);
	double dPos = bUpper ? -1.0 : 1.0;	
	double nUpper = 1;
	double dLen = GetDimVLen() + GetDimLevelDis() * (nDan-1.0);	

	if(m_CurDirection == DIM_BOTTOM) nUpper = -1;
	else						 	 nUpper = 1;
	long n = 0;
	if(bTotal)	
	{
		BOOL bDrawText = FALSE;
		long nPos = nStt;		
		long nFixPos;
		double dTotal = 0;
		double dTotCheck = 0;
		CDPoint OldP;
		for(n=nStt;n<=nEnd;n++)
		{
			CDPoint Po	= pObj->GetPoint(n);
			long nPre	= (n == nStt) ? n : pObj->GetPrevIdx(n,ptype);
			long nNext	= (n == nEnd) ? n : pObj->GetNextIdx(n,ptype);
			CDPoint Pre = pObj->GetPoint(nPre);
			CDPoint Next = pObj->GetPoint(nNext);

			CDPoint UpperP	= GetOffsetPos(Po, Pre, Next,Always(dLen * dPos));	
			if(n!=nPos) dTotal += ~(OldP - UpperP);
			OldP = UpperP;
		}
		while(nPos < nEnd)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,ptype));
			CDPoint Next1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,ptype));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			nFixPos =  pObj->GetNextIdxOnBx(nPos, ptype, type);

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,ptype));
			CDPoint Next2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,ptype));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			if(nPos == nStt)
			{
				LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			}
			if(nPos == pObj->GetPrevIdx(nEnd,type))
			{
				LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			}
			LineTo(UpperP1,UpperP2);
			dTotCheck += ~(UpperP1 - UpperP2);
			if(dTotCheck > dTotal / 2 && !bDrawText)
			{				
				double dL = dTotal/2 - (dTotCheck -~(UpperP1 - UpperP2));
				CDPoint MiddleP = UpperP1 - (UpperP2 - UpperP1).Unit() * dL;
				CDPoint vVec = (UpperP2 - UpperP1).Unit();
				SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				CString strDist;
				if(bMove)
					strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nStt,nEnd)), pObj->GetFlangeLMove(nStt,nEnd,bFlgUpper));
				else
					strDist = COMMA(pObj->GetDistance(nStt,nEnd));
				BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		
				bDrawText = TRUE;
			}			

			nPos = pObj->GetNextIdx(nPos, ptype);
		}			
	}
	else
	{
		long nPos = nStt;		
		long nFixPos;
		CDPoint OldP = CDPoint(0,0), MiddleP = CDPoint(0,0),vVec = CDPoint(0,0);			
		while(nPos < nEnd)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,ptype));
			CDPoint Next1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,ptype));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			nFixPos =  pObj->GetNextIdxOnBx(nPos, ptype, type);

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,ptype));
			CDPoint Next2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,ptype));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);			
			double dTotal = 0;
			double dTotCheck = 0;
			BOOL bCh = TRUE;
			for(n=nPos;n<=nFixPos;n++)
			{
				CDPoint Po	= pObj->GetPoint(n);
				long nPre	= (n == nStt) ? n : pObj->GetPrevIdx(n,ptype);
				long nNext	= (n == nEnd) ? n : pObj->GetNextIdx(n,ptype);
				CDPoint Pre = pObj->GetPoint(nPre);
				CDPoint Next = pObj->GetPoint(nNext);

				CDPoint UpperP	= GetOffsetPos(Po, Pre, Next,Always(dLen * dPos));	
				if(n!=nPos) dTotal += ~(OldP - UpperP);
				OldP = UpperP;
			}

			for(n=nPos;n<=nFixPos;n++)
			{
				CDPoint Po	= pObj->GetPoint(n);
				long nPre	= (n == nStt) ? n : pObj->GetPrevIdx(n,ptype);
				long nNext	= (n == nEnd) ? n : pObj->GetNextIdx(n,ptype);
				CDPoint Pre = pObj->GetPoint(nPre);
				CDPoint Next = pObj->GetPoint(nNext);
							
				CDPoint UpperP	= GetOffsetPos(Po, Pre, Next,Always(dLen * dPos));			
				if(n!=nPos)	dTotCheck += ~(UpperP - OldP);
				if(dTotCheck > dTotal / 2 && bCh && n!=nPos)
				{							
					double dL = dTotal/2 - (dTotCheck - ~(UpperP - OldP));					
					MiddleP = OldP + (UpperP - OldP).Unit() * dL;
					vVec = (UpperP - OldP).Unit();
					bCh = FALSE;
				}
								
				OldP = UpperP;
				if(n==nPos) MoveTo(UpperP);
				else		LineTo(UpperP);
			}						
			SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
			CString strDist;
			if(bMove)
				strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nPos,nFixPos)), pObj->GetFlangeLMove(nPos,nFixPos,bFlgUpper));
			else
				strDist = COMMA(pObj->GetDistance(nPos,nFixPos));
			BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		

			nPos = pObj->GetNextIdxOnBx(nPos,ptype,type);						
		}		
	}	
}

void CDimDomyunEx::DimPieceObjReverse(CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace, BOOL bTextRot, BOOL bTotal, BOOL bMove, BOOL bFlgUpper)
{
	CDPoint BaseP1, UpperP1, ExtP1;
	CDPoint BaseP2, UpperP2, ExtP2;

	long nStt = nFace < 0 ? 0 : pObj->GetSttFace(nFace);
	long nEnd = nFace < 0 ? pObj->GetUpperBound() : pObj->GetEndFace(nFace);
	double dPos = bUpper ? -1.0 : 1.0;	
	double nUpper = 1;
	double dLen = GetDimVLen() + GetDimLevelDis() * (nDan-1.0);	

	if(m_CurDirection == DIM_BOTTOM) nUpper = -1;
	else						 	 nUpper = 1;

	if(bTotal)	
	{
		BOOL bDrawText = FALSE;
		long nPos = nEnd;		
		long nFixPos;
		double dTotal = pObj->GetDistance(nStt,nEnd);
		double dTotCheck = 0;
		while(nPos > nStt)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,type));
			CDPoint Next1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,type));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			nFixPos =  pObj->GetPrevIdx(nPos, type);

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,type));
			CDPoint Next2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,type));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			if(nPos == nEnd)
			{
				LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			}
			if(nPos == pObj->GetNextIdx(nStt,type))
			{
				LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			}
			LineTo(UpperP1,UpperP2);
			dTotCheck += pObj->GetDistance(nPos, nFixPos);
			if(dTotCheck > dTotal / 2 && !bDrawText)
			{
				double dL = dTotCheck - dTotal / 2;
				CDPoint MiddleP = UpperP1 - (UpperP2 - UpperP1).Unit() * dL/2;
				CDPoint vVec = (UpperP2 - UpperP1).Unit();
				SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				CString strDist;
				if(bMove)
					strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nStt,nEnd)), pObj->GetFlangeLMove(nEnd,nStt,bFlgUpper));
				else
					strDist = COMMA(pObj->GetDistance(nStt,nEnd));
				BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		
				bDrawText = TRUE;
			}			

			nPos = pObj->GetPrevIdx(nPos, type);
		}			
	}
	else
	{
		long nPos = nEnd;		
		long nFixPos;
		while(nPos > nStt)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,type));
			CDPoint Next1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,type));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			nFixPos =  pObj->GetPrevIdx(nPos, type);

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,type));
			CDPoint Next2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,type));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			LineTo(UpperP1,UpperP2);
			CDPoint MiddleP = (UpperP1+UpperP2)/2;
			CDPoint vVec = (UpperP2 - UpperP1).Unit();
			SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
			CString strDist;
			if(bMove)
				strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nPos,nFixPos)), pObj->GetFlangeLMove(nPos,nFixPos,bFlgUpper));
			else
				strDist = COMMA(pObj->GetDistance(nFixPos,nPos));
			BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		

			nPos = pObj->GetPrevIdx(nPos, type);						
		}		
	}	
}

void CDimDomyunEx::DimPieceObjReverseOnBx(CPlatePieceObject *pObj, __int64 type, P_TYPE ptype, BOOL bUpper, double nDan, long nFace, BOOL bTextRot, BOOL bTotal, BOOL bMove, BOOL bFlgUpper)
{
	CDPoint BaseP1, UpperP1, ExtP1;
	CDPoint BaseP2, UpperP2, ExtP2;

	long nStt = nFace < 0 ? 0 : pObj->GetSttFace(nFace);
	long nEnd = nFace < 0 ? pObj->GetUpperBound() : pObj->GetEndFace(nFace);
	double dPos = bUpper ? -1.0 : 1.0;	
	double nUpper = 1;
	double dLen = GetDimVLen() + GetDimLevelDis() * (nDan-1.0);

	if(m_CurDirection == DIM_BOTTOM) nUpper = -1;
	else						 	 nUpper = 1;
	
	if(bTotal)	
	{
		BOOL bDrawText = FALSE;
		long nPos = nEnd;		
		long nFixPos;
		double dTotal = 0;
		double dTotCheck = 0;
		CDPoint OldP;
		for(long n=nStt;n>=nEnd;n--)
		{
			CDPoint Po	= pObj->GetPoint(n);
			long nPre	= (n == nEnd) ? n : pObj->GetNextIdx(n,ptype);
			long nNext	= (n == nStt) ? n : pObj->GetPrevIdx(n,ptype);
			CDPoint Pre = pObj->GetPoint(nPre);
			CDPoint Next = pObj->GetPoint(nNext);
						
			CDPoint UpperP	= GetOffsetPos(Po, Pre, Next,Always(dLen * dPos));	
			if(n!=nPos) dTotal += ~(OldP - UpperP);
			OldP = UpperP;
		}
		while(nPos > nStt)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,ptype));
			CDPoint Next1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,ptype));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			nFixPos =  pObj->GetPrevIdxOnBx(nPos, ptype, type);

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,ptype));
			CDPoint Next2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,ptype));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			if(nPos == nEnd)
			{
				LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			}
			if(nPos == pObj->GetNextIdx(nStt,type))
			{
				LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			}
			LineTo(UpperP1,UpperP2);
			dTotCheck += ~(UpperP2 - UpperP1);
			if(dTotCheck > dTotal / 2 && !bDrawText)
			{
				//double dL = (dTotCheck - dTotal) / 2;
				double dL = dTotal/2 - (dTotCheck - ~(UpperP2 - UpperP1));					
				CDPoint MiddleP = UpperP1 - (UpperP2 - UpperP1).Unit() * dL;
				CDPoint vVec = (UpperP2 - UpperP1).Unit();
				SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
				CString strDist;
				if(bMove)
					strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nStt,nEnd)), pObj->GetFlangeLMove(nStt,nEnd,bFlgUpper));
				else
					strDist = COMMA(pObj->GetDistance(nEnd,nStt));
				BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		
				bDrawText = TRUE;
			}			

			nPos = pObj->GetPrevIdxOnBx(nPos,ptype,type);
		}			
	}
	else
	{
		long nPos = nEnd;		
		long nFixPos = nStt;
		CDPoint OldP = CDPoint(0,0), MiddleP = CDPoint(0,0),vVec = CDPoint(0,0);			
		while(nPos > nStt)
		{			
			CDPoint Po1		= pObj->GetPoint(nPos);
			CDPoint Pre1	= (nPos == nEnd) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetNextIdx(nPos,ptype));
			CDPoint Next1	= (nPos == nStt) ? pObj->GetPoint(nPos) : pObj->GetPoint(pObj->GetPrevIdx(nPos,ptype));
			
			BaseP1	= GetOffsetPos(Po1, Pre1, Next1,Always(GetDimExo() * dPos));
			UpperP1	= GetOffsetPos(Po1, Pre1, Next1,Always(dLen * dPos));
			ExtP1	= GetOffsetPos(Po1, Pre1, Next1,Always((dLen + GetDimExe()) * dPos));

			if(nPos != nFixPos && pObj->GetBx(nPos)->IsState((bFlgUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE) | BX_SPLICE))
			{
				SetTextAngle(0);
				if(pObj->GetBx(nPos)->IsState(BX_SPLICE))
				{
					if(pObj->GetBx(nPos)->GetSplice()->m_uD==0)
					{
						CDPoint SPoint = GetOffsetPos(Po1, Pre1, Next1,Always(20 * dPos));
						CSymbolDom Dom(this,NULL);
						Dom.SetScaleDim(GetScaleDim()/2);
						Dom.DrawSymbolCuttingS(SPoint.x,SPoint.y,(UpperP1-BaseP1).Unit().RotateInv90().GetAngleRadian());
						*this << Dom;
					}
				}
				else
				{
					CDPoint SPoint = GetOffsetPos(Po1, Pre1, Next1,Always(20 * dPos));
					CSymbolDom Dom(this,NULL);
					Dom.SetScaleDim(GetScaleDim()/2);
					Dom.DrawSymbolCuttingS(SPoint.x,SPoint.y,(UpperP1-BaseP1).Unit().RotateInv90().GetAngleRadian());
					*this << Dom;					
				}
			}

			nFixPos =  pObj->GetPrevIdxOnBx(nPos, ptype, type);			

			CDPoint Po2		= pObj->GetPoint(nFixPos);
			CDPoint Pre2	= (nFixPos == nEnd) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetNextIdx(nFixPos,ptype));
			CDPoint Next2	= (nFixPos == nStt) ? pObj->GetPoint(nFixPos) : pObj->GetPoint(pObj->GetPrevIdx(nFixPos,ptype));
			
			BaseP2	= GetOffsetPos(Po2, Pre2, Next2,Always(GetDimExo() * dPos));
			UpperP2	= GetOffsetPos(Po2, Pre2, Next2,Always(dLen * dPos));
			ExtP2	= GetOffsetPos(Po2, Pre2, Next2,Always((dLen + GetDimExe()) * dPos));

			if(pObj->GetBx(nFixPos)->IsState((bFlgUpper ? BX_UPPERFLANGE : BX_LOWERFLANGE) | BX_SPLICE))
			{
				SetTextAngle(0);
				if(pObj->GetBx(nFixPos)->IsState(BX_SPLICE))
				{
					if(pObj->GetBx(nFixPos)->GetSplice()->m_uD==0)
					{
						CDPoint SPoint = GetOffsetPos(Po2, Pre2, Next2,Always(20 * dPos));						
						CSymbolDom Dom(this,NULL);
						Dom.SetScaleDim(GetScaleDim()/2);
						Dom.DrawSymbolCuttingS(SPoint.x,SPoint.y,(UpperP2-BaseP2).Unit().RotateInv90().GetAngleRadian());
						*this << Dom;						
					}
				}
				else
				{
					CDPoint SPoint = GetOffsetPos(Po2, Pre2, Next2,Always(20 * dPos));
					CSymbolDom Dom(this,NULL);
					Dom.SetScaleDim(GetScaleDim()/2);
					Dom.DrawSymbolCuttingS(SPoint.x,SPoint.y,(UpperP2-BaseP2).Unit().RotateInv90().GetAngleRadian());
					*this << Dom;					
				}
			}

			LineTo(BaseP1,ExtP1); BlockDot(UpperP1.x,UpperP1.y);
			LineTo(BaseP2,ExtP2); BlockDot(UpperP2.x,UpperP2.y);
			double dTotal = 0;
			double dTotCheck = 0;
			BOOL bCh = TRUE;
			long n = 0;
			for(n=nPos;n>=nFixPos;n--)
			{
				CDPoint Po	= pObj->GetPoint(n);
				long nPre	= (n == nEnd) ? n : pObj->GetNextIdx(n,ptype);
				long nNext	= (n == nStt) ? n : pObj->GetPrevIdx(n,ptype);
				CDPoint Pre = pObj->GetPoint(nPre);
				CDPoint Next = pObj->GetPoint(nNext);
							
				CDPoint UpperP	= GetOffsetPos(Po, Pre, Next,Always(dLen * dPos));	
				if(n!=nPos) dTotal += ~(OldP - UpperP);
				OldP = UpperP;
			}
			for(n=nPos;n>=nFixPos;n--)
			{
				CDPoint Po	= pObj->GetPoint(n);
				long nPre	= (n == nEnd) ? n : pObj->GetNextIdx(n,ptype);
				long nNext	= (n == nStt) ? n : pObj->GetPrevIdx(n,ptype);
				CDPoint Pre = pObj->GetPoint(nPre);
				CDPoint Next = pObj->GetPoint(nNext);
							
				CDPoint UpperP	= GetOffsetPos(Po, Pre, Next,Always(dLen * dPos));			
				if(n!=nPos)	dTotCheck += ~(OldP-UpperP);
				if(dTotCheck > dTotal / 2 && bCh && n!=nPos)
				{							
					double dL = dTotal/2 - (dTotCheck - ~(UpperP - OldP));					
					MiddleP = OldP + (UpperP - OldP).Unit() * dL;
					vVec = (UpperP - OldP).Unit();
					bCh = FALSE;
				}
								
				OldP = UpperP;
				if(n==nPos) MoveTo(UpperP);
				else		LineTo(UpperP);
			}				
			SetTextAngle(ToDegree(vVec.GetAngleRadian() + (bTextRot ? ConstPi : 0)));
			CString strDist;
			if(bMove)
				strDist.Format("%s(%+.1f)",COMMA(pObj->GetDistance(nPos,nFixPos)), pObj->GetFlangeLMove(nPos,nFixPos,bFlgUpper));
			else
				strDist = COMMA(pObj->GetDistance(nFixPos,nPos));
			BlockText(Always(m_DIMTVP) * sin(vVec.GetAngleRadian()) * nUpper, Always(m_DIMTVP) * cos(vVec.GetAngleRadian()) * nUpper, MiddleP.x, MiddleP.y, strDist);		

			nPos = pObj->GetPrevIdxOnBx(nPos,ptype,type);						
		}		
	}	
}

void CDimDomyunEx::DimPieceObjLevelPos(CDPoint InitBaseP, CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace, BOOL bStart, BOOL bDrawLine)
{	
	CDPoint BaseP, UpperP, ExtP;

	long nStt = nFace < 0 ? 0 : pObj->GetSttFace(nFace);
	long nEnd = nFace < 0 ? pObj->GetUpperBound() : pObj->GetEndFace(nFace);	
	double nUpper = 1;
	double dLen = GetDimVLen() + GetDimLevelDis() * (nDan-1.0);

	double dTotal = 0;

	if(bUpper) nUpper = -1;
	else	   nUpper = 1;
	
	long nPos = bStart ? pObj->GetPFirst(type,nStt,nEnd) : pObj->GetPLast(type,nStt,nEnd);		
	while(nPos != -1)
	{						
		CDPoint Po	= pObj->GetPoint(nPos);
		long nPrev = pObj->GetPFixPrev();
		long nNext = pObj->GetPFixNext();		
		CDPoint Pre	= nPrev == -1 ? Po : pObj->GetPoint(nPrev);
		CDPoint Next= nNext == -1 ? Po : pObj->GetPoint(nNext);			
		
		CString Text1,Text2;
		Text1.Format("%.1f",pObj->GetPoint(nPos).x - InitBaseP.x);		
		Text2.Format("(%.1f)",pObj->GetPoint(nPos).y - InitBaseP.y);
			
		BaseP	= GetOffsetPos(Po, Pre, Next,Always(GetDimExo() * nUpper));
		UpperP	= GetOffsetPos(Po, Pre, Next,Always(dLen * nUpper) + Always(5) * nUpper);		
		
		CDPoint vAng = UpperP.y > BaseP.y ? (UpperP - BaseP).Unit() : (BaseP - UpperP).Unit();
		SetTextAngle(vAng.GetAngleDegree());
		CDPoint TextPos1 = UpperP - vAng.RotateInv90() * Always(1);
		CDPoint TextPos2 = UpperP + vAng.RotateInv90() * (GetTextHeight() + Always(1));		
		dTotal += ~(Pre-Po);
		//if(Po != InitBaseP)
		{						
			//Text1.Format("%.1f",dTotal);		
			TextOut(TextPos1,Text1);
			TextOut(TextPos2,Text2);
			if(bDrawLine)
			{
				ExtP	= GetOffsetPos(Po, Pre, Next,Always(dLen * nUpper) + Always(15) * nUpper);
				LineTo(BaseP,ExtP);
			}
		}
		
		nPos =  pObj->GetPNext();
	}		
}

void CDimDomyunEx::DimPieceObjLevelPosHor(CDPoint BaseP, CPlatePieceObject *pObj, P_TYPE type, BOOL bUpper, double nDan, long nFace, BOOL bStart)
{
	long nStt = nFace < 0 ? 0 : pObj->GetSttFace(nFace);
	long nEnd = nFace < 0 ? pObj->GetUpperBound() : pObj->GetEndFace(nFace);
	double dPos = bUpper ? 1.0 : -1.0;		
	double dLen = (GetDimExo() + GetDimVLen() + GetDimLevelDis() * nDan) * dPos;	

	SetTextAlignHorz(TA_LEFT);
	double dTLen = bUpper ? GetTextWidth("",4) : GetTextWidth("",0);
	long nPos = bStart ? pObj->GetPFirst(type,nStt,nEnd) : pObj->GetPLast(type,nStt,nEnd);		
	while(nPos != -1)
	{						
		CString Text1,Text2;
		CDPoint Po		= pObj->GetPoint(nPos);				
		SetTextAngle(90);
		CDPoint TextPos1 = Po + CDPoint(-Always(1),Always(dLen) - dTLen);
		CDPoint TextPos2 = Po + CDPoint(GetTextHeight() + Always(1),Always(dLen) - dTLen);
		//if(Po != BaseP)
		{
			Text1.Format("%.1f",Po.x - BaseP.x);		
			Text2.Format("%.1f",pObj->GetPoint(nPos).y - BaseP.y);		
			TextOut(TextPos1,Text1 == "0.0" ? "0" : Text1);
			TextOut(TextPos2,Text2 == "0.0" ? "0" : Text2);
		}
		if(!bUpper) LineTo(Po.x,Po.y-Always(1),Po.x,Po.y-Always(22));
		
		nPos =  pObj->GetPNext();
	}	
}

void CDimDomyunEx::DimPieceFace(CPlatePieceObject *pObj, long nFace, long nDimDan, CString sDir, P_TYPE type, BOOL bTotal)
{
	SetDirection(sDir);
	long nStt = pObj->GetSttFace(nFace);
	long nEnd = pObj->GetEndFace(nFace);
	long nIdx = (nStt<nEnd || nEnd==0) ? pObj->GetPFirst(type,nStt,nEnd)
								       : pObj->GetPLast(type,nEnd,nStt);
	CDPoint DimXY[60];

	for(long n=0; n<60; n++) DimXY[n] = CDPoint(-1,-1);

	long i=0;
	CDPoint xy	  = pObj->GetPoint(nIdx);
	CDPoint xyStt = pObj->GetPoint(nIdx);
	CDPoint xyPre = pObj->GetPoint(nIdx);
	DimMoveTo(xy,nDimDan);
	while(nIdx != -1)
	{		
		if(nIdx != nStt)
			DimXY[i++] = pObj->GetPoint(nIdx);
		nIdx = pObj->GetPNext();
	}

	double CurDis,NextDis,Total=0;
	long Ea = 1;
	CString str;
	for(i=0; i<60; i++) {
		xy = DimXY[i];
		if(xy.x == -1) break;
		if(sDir=="LEFT"||sDir=="RIGHT") {
			CurDis  = xy.y - xyPre.y;
			if(CurDis==0) continue;
			NextDis = DimXY[i+1].y - xy.y;
			if(CurDis!=NextDis)
			{
				if(Ea==1)
					DimLineTo(xy.x,CurDis,COMMA(fabs(CurDis)));
				else
				{
					str.Format("%d@%s=%s",Ea,COMMA(fabs(CurDis)),COMMA(fabs(CurDis*Ea)));
					DimLineTo(xy.x,CurDis*Ea,str);
				}
				Ea = 1;
			}
			else
				Ea++;
		}
		if(sDir=="TOP"||sDir=="BOTTOM") {
			CurDis  = Round(xy.x - xyPre.x,10);
			if(CurDis==0) continue;
			NextDis = Round(DimXY[i+1].x - xy.x,10);
			if(CurDis!=NextDis) 
			{
				if(Ea==1 && CurDis!=0)
					DimLineTo(CurDis,xy.y,COMMA(fabs(CurDis)));
				else
				{
					str.Format("%d@%s=%s",Ea,COMMA(fabs(CurDis)),COMMA(fabs(CurDis*Ea)));
					DimLineTo(CurDis*Ea,xy.y,str);
				}
				Ea = 1;
			}
			else 
				Ea++;
		}
		Total += CurDis;
		xyPre = xy;
	}

	if(!bTotal) return;	
	DimMoveTo(xyStt,++nDimDan);
	if(sDir=="LEFT"||sDir=="RIGHT")
		DimLineTo(xyStt.x,Total,COMMA(fabs(Total)));
	if(sDir=="TOP"||sDir=="BOTTOM")
		DimLineTo(Total,xyStt.y,COMMA(fabs(Total)));
}

void CDimDomyunEx::DimPieceFaceExt(CPlatePieceObject *pObj, long nFace, long nDimDan, CString sDir, P_TYPE type, BOOL bTotal,BOOL bEqu)
{
	SetDirection(sDir);
	long nStt = pObj->GetSttFace(nFace);
	long nEnd = pObj->GetEndFace(nFace);
	long nIdx = (nStt<nEnd) ? pObj->GetPFirst(type,nStt,nEnd)
	                        : pObj->GetPLast(type,nEnd,nStt);
	CDPointArray DimXY;
	
	double CurDis,NextDis,Total=0;
	long i=0;
	CDPoint xy	  = pObj->GetPoint(nIdx);
	CDPoint xyStt = pObj->GetPoint(nIdx);
	CDPoint xyPre = pObj->GetPoint(nIdx);

	while(nIdx != -1)
	{		
		if(nIdx != nStt)
			DimXY.Add(pObj->GetPoint(nIdx));		
		nIdx = pObj->GetPNext();		
		if(nIdx == nStt) break;
	}	
	
	if(sDir=="LEFT"||sDir=="RIGHT")
	{
		CurDis  = Round(DimXY[0].y - xyPre.y,2);
		DimMoveTo(DimXY[0].x,xy.y,nDimDan);		
	}
	else
	{
		CurDis  = Round(DimXY[0].x - xyPre.x,2);				
		DimMoveTo(xy.x,DimXY[0].y,nDimDan);
	}
		
	long Ea = 1;
	CString str,str2;
	for(i=0; i<DimXY.GetSize()-1; i++) {
		xy = DimXY[i];
		if(xy.x == -1) break;
		if(sDir=="LEFT"||sDir=="RIGHT") {
			CurDis  = Round(xy.y - xyPre.y,2);
			NextDis = Round(DimXY[i+1].y - xy.y,2);
			if(i==0 || CurDis!=NextDis || !bEqu || i==DimXY.GetSize()-2)
			{
				if(Ea==1 || !bEqu)
				{		
					if(i!=0 && bEqu)
					{						
						DimMoveTo(m_CurDimPos,(nDimDan+1));
						DimLineTo(xy.x,CurDis,COMMA(fabs(CurDis)));
						DimBasePoint(m_CurDimPos,nDimDan);
					}
					else
						DimLineTo(xy.x,CurDis,COMMA(fabs(CurDis)));
				}
				else
				{
					str.Format("%d@%s=%s",Ea,COMMA(fabs(CurDis)),COMMA(fabs(CurDis*Ea)));
					if(GetTextWidth(str) < fabs(CurDis*Ea))
						DimLineTo(xy.x,CurDis*Ea,str);
					else
					{
						str.Format("%d@%s",Ea,COMMA(fabs(CurDis)));
						str2.Format("=%s",COMMA(fabs(CurDis*Ea)));
						DimLineTo(xy.x,CurDis*Ea,str,str2);
					}
				}
				Ea = 1;
			}
			else
				Ea++;
		}
		if(sDir=="TOP"||sDir=="BOTTOM") {
			CurDis  = Round(xy.x - xyPre.x,2);
			NextDis = Round(DimXY[i+1].x - xy.x,2);
			if(i==0 || CurDis!=NextDis || !bEqu || i==DimXY.GetSize()-2)
			{
				if(Ea==1 || !bEqu)
				{
					if(i!=0 && bEqu)
					{
						DimMoveTo(m_CurDimPos,(nDimDan+1));
						DimLineTo(CurDis,xy.y,COMMA(fabs(CurDis)));
						DimBasePoint(m_CurDimPos,nDimDan);
					}
					else
						DimLineTo(CurDis,xy.y,COMMA(fabs(CurDis)));
				}
				else
				{
					str.Format("%d@%s=%s",Ea,COMMA(fabs(CurDis)),COMMA(fabs(CurDis*Ea)));
					if(GetTextWidth(str) < fabs(CurDis*Ea))
						DimLineTo(CurDis*Ea,xy.y,str);
					else
					{
						str.Format("%d@%s",Ea,COMMA(fabs(CurDis)));
						str2.Format("=%s",COMMA(fabs(CurDis*Ea)));
						DimLineTo(CurDis*Ea,xy.y,str,str2);
					}
				}
				Ea = 1;
			}
			else 
				Ea++;
		}
		Total += CurDis;
		xyPre = xy;
	}
	Total += NextDis;
	if(sDir=="LEFT"||sDir=="RIGHT") {
		DimMoveTo(m_CurDimPos,nDimDan);
		DimLineTo(xy.x,NextDis,COMMA(fabs(NextDis)));
	}

	if(!bTotal) return;	
	if(bEqu) nDimDan ++;	
	DimMoveTo(xyStt,++nDimDan);
	if(sDir=="LEFT"||sDir=="RIGHT")
		DimLineTo(xyStt.x,Total,COMMA(fabs(Total)));
	if(sDir=="TOP"||sDir=="BOTTOM")
		DimLineTo(Total,xyStt.y,COMMA(fabs(Total)));

}

void CDimDomyunEx::DimPieceChamfer(CPlatePieceObject *pObj, long nIdx, long nDir, double R)
{
	BOOL bChamfer = FALSE;
	if (pObj->GetPTType(nIdx) & P_TYPE_CHAMFER)  bChamfer = TRUE;
	//
	CString str;
	long nEa = 0;
	for(long i=0; i<pObj->GetObjSize(); i++) 
	{
		if(pObj->GetPTType(i) & P_TYPE_CHAMFER && bChamfer)
			nEa++;
		if(pObj->GetPTType(i) & P_TYPE_SCALLOP && !bChamfer)
			nEa++;
	}

	CDPoint xy1 = pObj->GetPoint(nIdx+1);
	CDPoint xy2 = pObj->GetPoint(nIdx+2);
	R = (R == 0) ? pObj->GetRadius(nIdx) : R;
	CDPoint mid = (xy2+xy1)/2 + (xy2-xy1).Unit().Rotate90() * pObj->GetRadius(nIdx+2);
	if (nEa > 1)
		str.Format("%d-%c%.0f",nEa,bChamfer?'C':'R',R);
	else
		str.Format("%c%.0f",bChamfer?'C':'R',R);
	
	double Dis1 = Always(8);
	double Dis2 = GetTextWidth(str);
	if(nDir==3 || nDir==5) Dis2 *= -1;
	GiSiArrowExt(mid.x,mid.y,Dis1,Dis2,nDir,str);
}

void CDimDomyunEx::DimPieceChamferEx(CPlatePieceObject *pObj, long nIdx, BOOL bStt, BOOL bInSide, double R)
{
	BOOL bChamfer = FALSE;
	if (pObj->GetPTType(nIdx) & P_TYPE_CHAMFER)  bChamfer = TRUE;
	//
	CString str;
	long nEa = 0;
	for(long i=0; i<pObj->GetObjSize(); i++) 
	{
		if(pObj->GetPTType(i) & P_TYPE_CHAMFER && bChamfer)
			nEa++;
		if(pObj->GetPTType(i) & P_TYPE_SCALLOP && !bChamfer)
			nEa++;
	}

	CDPoint xy1 = pObj->GetPoint(nIdx+1);
	CDPoint xy2 = pObj->GetPoint(nIdx+2);
	CDPoint vVec = (xy2-xy1).Unit();
	R = (R == 0) ? pObj->GetRadius(nIdx) : R;
	CDPoint mid = (xy2+xy1)/2 + vVec.Rotate90() * pObj->GetRadius(nIdx+2);
	if (nEa > 1)
		str.Format("%d-%c%.0f",nEa,bChamfer?'C':'R',R);
	else
		str.Format("%c%.0f",bChamfer?'C':'R',R);
		
	double Dis = GetTextWidth(str,2);
	CDPoint vVec90 = bInSide ? vVec.RotateInv90() : vVec.Rotate90();
	CDPoint EndP = mid + vVec90 * (GetArrowHeadSize() + Dis);
	CDPoint TextP = mid + vVec90 * (GetArrowHeadSize() + Dis/2 + Always(2)) + (bStt ? vVec90.Rotate90() : vVec90.RotateInv90()) * Always(1);
	
	Arrow(mid,bInSide ? vVec.Rotate90().GetAngleRadian() : vVec.RotateInv90().GetAngleRadian());
	LineTo(mid,EndP);
	double OldAng = GetTextAngle();
	if(!bStt) vVec90 *= -1;
	SetTextAngle(vVec90.GetAngleDegree());
	TextOut(TextP,str);
	SetTextAngle(OldAng);
}

void CDimDomyunEx::DimPieceObjEx(CPlatePieceObject *pObj, P_TYPE type, __int64 BxFlag, CString sDir, long nDan, long nFace, BOOL bReverse, BOOL bAt /* = FALSE */ , BOOL bTotal /* = FALSE */)
{
	CDPointArray DimXYArr;
	BOOL bNextTextDraw = TRUE;	
	BOOL bPrevTop = TRUE;

	SetDirection(sDir);	
	long nStt = bReverse ? pObj->GetEndFace(-nFace) : pObj->GetSttFace(nFace);
	long nEnd = bReverse ? pObj->GetSttFace(-nFace) : pObj->GetEndFace(nFace);	
	long nIdx = (nStt<nEnd) ? pObj->GetPFirst(type,nStt,nEnd)
	                        : pObj->GetPLast(type,nEnd,nStt);		
	while(nIdx != -1)
	{		
		if(BxFlag==0 || pObj->GetBx(nIdx)->IsState(BxFlag))
			DimXYArr.Add(pObj->GetPoint(nIdx));
		nIdx = pObj->GetPNext();
	}

	long nSize = DimXYArr.GetSize();
	for(long n=0; n<nSize; n++)
	{
		if(n==0)	// Start Point
		{
			if(nSize > 1)
			{
				double dDis = GetDist(DimXYArr[0],DimXYArr[1],sDir);
				CString szText = COMMA(dDis);
				double dTextWidth = GetTextWidth(szText);				
				if(dTextWidth > dDis) 
				{
					DimMoveTo(DimXYArr[n],nDan,FALSE,szText);
					bNextTextDraw = FALSE;
				}
				else							
					DimMoveTo(DimXYArr[n],nDan);
			}
			else
				DimMoveTo(DimXYArr[n],nDan);
		}		
		else if(n==nSize-1 && n!=0)	// End Point
		{		
			if(nSize > 0)
			{		
				double dDis = GetDist(DimXYArr[n],DimXYArr[n-1],sDir);
				CString szText = COMMA(dDis);
				double dTextWidth = GetTextWidth(szText);	
				double dX, dY;
				
				if(sDir == "TOP" || sDir == "BOTTOM")
					dX = dDis, dY = DimXYArr[n].y;
				else
					dX = DimXYArr[n].x, dY = dDis;

				if(dTextWidth > dDis) 
				{
					//if(!bPrevTop)	
					//	DimLineTo(dX,dY,szText);
					//else			
					DimLineTo(dX,dY,szText,"",FALSE,TRUE);
				}
				else							
					DimLineTo(dX,dY,szText);
			}			
		}
		else	// General Point
		{	
			double dDis = GetDist(DimXYArr[n],DimXYArr[n-1],sDir);
			CString szText = COMMA(dDis);
			double dTextWidth = GetTextWidth(szText);			
			double dX, dY;
			if(sDir == "TOP" || sDir == "BOTTOM")
				dX = dDis, dY = DimXYArr[n].y;
			else
				dX = DimXYArr[n].x, dY = dDis;

			if(bNextTextDraw)
			{
				if(dTextWidth > dDis) 
				{
					if(bPrevTop)
					{
						DimLineTo(dX,dY,"",szText);
						bPrevTop = FALSE;
					}
					else
					{
						DimLineTo(dX,dY,szText);
						bPrevTop = TRUE;
					}
				}
				else
				{						
					DimLineTo(dX,dY,szText);
					bPrevTop = TRUE;
				}												
			}
			else
			{
				DimLineTo(dX,dY,"");
				bNextTextDraw = TRUE;
				bPrevTop = FALSE;
			}
		}
	}
}

void CDimDomyunEx::SetCutLineOn(BOOL bTrue) 
{ 
	m_CutExtLine = bTrue; 
}