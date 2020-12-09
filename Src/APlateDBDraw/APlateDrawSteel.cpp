// APlateDrawSteel.cpp: implementation of the CAPlateDrawSteel class.
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

CAPlateDrawSteel::CAPlateDrawSteel(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{

}

CAPlateDrawSteel::~CAPlateDrawSteel()
{

}

// pBx         = 가로보 또는 브레이싱BX
// installDir  = 빔의 설치방향(문서참조)
// eyePos      = 빔을 보는 시야의 방향(0:정면, 1:평면)
// xyStt       = 골조의 시점
// xyEnd       = 골조의 종점
// nUsingType  = 수직브레이싱인지 수평브레이싱인가?
// nDrawingType= 부재의 시,종점위치결정
// bJewon      = 제원 표시유무
// bEndArrow   = 상세도에서 CuttingLine 처리 하기 위해서
// nInstallPos = 부재의 설치위치(수직브레이싱: UP, DOWN, LEFT, RIGHT)
//								(수평브레이싱: LEFT, RIGHT)
void CAPlateDrawSteel::DrawBeamL   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden)

{
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CSection *pSec = pBx->GetSection();
	CAPlateDrawCross DrawCross(m_pDataMng);
	CGlobarOption	*pOpt = m_pDataMng->GetGlobalOption(); 

	double dT1, dHeight;
	double dCenter;
	if(nUsingType==VBRACING)
	{
		dT1 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T1);
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, eyePos==0?CSection::CENTERDX:CSection::CENTERDY);		
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, TRUE, eyePos==0 ? TRUE : FALSE); // 양측중 긴부분
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;

	}
	else
	{
		dT1 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T1);
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::CENTERDX);		
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, FALSE, eyePos==0 ? TRUE : FALSE); // 양측중 작은 부분
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;
	}
		
	double dLength = 400;
	CString szJewon = "";
	if(bJewon)
	{
		if(nUsingType==VBRACING)
			szJewon.Format("1-L %s", pGir->GetStringVBracingBeamJewon(pBx, nInstallPos));
		else if(nUsingType==HBRACING)
			szJewon.Format("1-L %s", pGir->GetStringHBracingBeamJewon(pBx, nInstallPos));
	}
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	CDPoint xyLT, xyRT, xyRD, xyLD;
	CDPoint xyCen = (xyStt+xyEnd)/2;

	if(nDrawingType==FROM_STT)
		xyEnd = xyStt+vDir*dLength;
	else if(nDrawingType==FROM_END)
		xyStt = xyEnd-vDir*dLength;	
	else if(nDrawingType==FROM_MID)
	{
		xyStt = xyCen-vDir*dLength;	
		xyEnd = xyCen+vDir*dLength;
	}
	
	xyLT = xyStt + vDir.Rotate90() * (installDir==1 ? dHeight-dCenter:dCenter);
	xyRT = xyLT + vDir*~(xyEnd-xyStt);
	xyRD = xyRT + vDir.RotateInv90() * dHeight;
	xyLD = xyRD - vDir*~(xyEnd-xyStt);
	if(eyePos==0)
	{
		Dom.LineTo(xyLT, xyRT);
		Dom.LineTo(xyLD, xyRD);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyRT, xyRD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyLT, xyLD);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyLT, xyLD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyRT, xyRD);
			}
			else if(nDrawingType==FROM_MID)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyLT, xyLD, Dom.Always(2));
				Dom.CutLightningAnyDir(xyRT, xyRD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(xyLT, xyLD);
			Dom.LineTo(xyRT, xyRD);
		}

		if(installDir==0 || installDir==1)  //정면설치
			pOptStd->SetEnvType(&Dom,  HCAD_STLC);		
		if(installDir==2 || installDir==3)   //반대설치
			pOptStd->SetEnvType(&Dom, HCAD_STLH);		

		if(installDir==1 || installDir==3)
		{
			Dom.MoveTo(xyLD + vDir.Rotate90()*dT1);
			Dom.LineTo(xyRD + vDir.Rotate90()*dT1);
		}
		if(installDir==0 || installDir==2)
		{
			Dom.MoveTo(xyLT + vDir.RotateInv90()*dT1);
			Dom.LineTo(xyRT + vDir.RotateInv90()*dT1);
		}
		
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xyRT-xyLT).Unit();
			CDPoint xyPos = xyLT + vAng * ~(xyRT-xyLT)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	else
	{
		pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
		Dom.LineTo(xyLT, xyRT);
		Dom.LineTo(xyLD, xyRD);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyRT, xyRD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyLT, xyLD);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyLT, xyLD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyRT, xyRD);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
			Dom.LineTo(xyLT, xyLD);
			Dom.LineTo(xyRT, xyRD);
		}

		if(installDir==0 || installDir==1)
		{
			if(installDir==0 || installDir==2)
				pOptStd->SetEnvType(&Dom, HCAD_STLH);
			else
				pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);

			Dom.MoveTo(xyLT + vDir.RotateInv90()*dT1);
			Dom.LineTo(xyRT + vDir.RotateInv90()*dT1);
		}
		else
		{
			if(installDir==0 || installDir==2)
				pOptStd->SetEnvType(&Dom, HCAD_STLH);
			else
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
			
			Dom.MoveTo(xyLD + vDir.Rotate90()*dT1);
			Dom.LineTo(xyRD + vDir.Rotate90()*dT1);
		}
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xyRT-xyLT).Unit();

			CDPoint xyPos(0,0);
//			if(vAng.x<0)	// 글자가 부재 밑에 뒤집어 써질경우
//			{
//				vAng = (xyLD-xyRD).Unit();
//				xyPos = xyRD + vAng * ~(xyLD-xyRD)/2 + vAng.Rotate90() * Dom.Always(1);
//			}
//			else
				xyPos = xyLT + vAng * ~(xyRT-xyLT)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	*pDom << Dom;		
}

// pBx         = 가로보 또는 브레이싱BX
// installDir  = 빔의 설치방향(문서참조)
// eyePos      = 빔을 보는 시야의 방향(0:정면, 1:평면)
// xyStt       = 골조의 시점
// xyEnd       = 골조의 종점
// nUsingType  = 수직브레이싱인지 수평브레이싱인가?
// nDrawingType= 부재의 시,종점위치결정
// bJewon      = 제원 표시유무
// bEndArrow   = 상세도에서 CuttingLine 처리 하기 위해서
// nInstallPos = 부재의 설치위치(수직브레이싱: UP, DOWN, LEFT, RIGHT)
//								(수평브레이싱: LEFT, RIGHT)
void CAPlateDrawSteel::DrawBeamLVal   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden)

{
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CGlobarOption	*pOpt = m_pDataMng->GetGlobalOption(); 
	CSection *pSec = pBx->GetSection();
	CAPlateDrawCross DrawCross(m_pDataMng);
	double dT1, dCenter, dHeight;
	if(nUsingType==VBRACING)
	{
		dT1 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T1);
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, eyePos==0?CSection::CENTERDX:CSection::CENTERDY);		
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, TRUE, eyePos==0 ? TRUE : FALSE); // 양측중 긴부분
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;
	}
	else
	{
		dT1 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T1);
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::CENTERDX);		
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, FALSE, eyePos==0 ? TRUE : FALSE); // 양측중 작은 부분
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;
	}
	
	double dLength = 400;
	
	CString szJewon = "";
	if(bJewon)
	{
		if(nUsingType==VBRACING)
			szJewon.Format("1-L %s", pGir->GetStringVBracingBeamJewon(pBx, nInstallPos));
		else if(nUsingType==HBRACING)
			szJewon.Format("1-L %s", pGir->GetStringHBracingBeamJewon(pBx, nInstallPos));
	}
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	CDPoint xyLT, xyRT, xyRD, xyLD;
	CDPoint xyCen = (xyStt+xyEnd)/2;

	if(nDrawingType==FROM_STT)
		xyEnd = xyStt+vDir*dLength;
	else if(nDrawingType==FROM_END)
		xyStt = xyEnd-vDir*dLength;	
	else if(nDrawingType==FROM_MID)
	{
		xyStt = xyCen-vDir*dLength;	
		xyEnd = xyCen+vDir*dLength;
	}
	else
	{
		xyStt = xyStt;
		xyEnd = xyEnd;
	}
	

	if(eyePos==0)
	{
		xyLT = xyStt + vDir.Rotate90() * (installDir==1 ? dHeight-dCenter:dCenter);
		xyRT = xyLT + vDir*~(xyEnd-xyStt);
		xyRD = xyRT + vDir.RotateInv90() * dHeight;
		xyLD = xyRD - vDir*~(xyEnd-xyStt);

		Dom.LineTo(xyLT, xyRT);
		Dom.LineTo(xyLD, xyRD);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyRT, xyRD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyLT, xyLD);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyLT, xyLD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyRT, xyRD);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(xyLT, xyLD);
			Dom.LineTo(xyRT, xyRD);
		}

		if(installDir==0 || installDir==1)  //정면설치
			pOptStd->SetEnvType(&Dom, HCAD_STLC);		
		if(installDir==2 || installDir==3)   //반대설치
			pOptStd->SetEnvType(&Dom, HCAD_STLH);		

		if(installDir==1 || installDir==3)
		{
			Dom.MoveTo(xyLD + vDir.Rotate90()*dT1);
			Dom.LineTo(xyRD + vDir.Rotate90()*dT1);
		}
		if(installDir==0 || installDir==2)
		{
			Dom.MoveTo(xyLT + vDir.RotateInv90()*dT1);
			Dom.LineTo(xyRT + vDir.RotateInv90()*dT1);
		}
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xyRT-xyLT).Unit();
			CDPoint xyPos = xyLT + vAng * ~(xyRT-xyLT)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	else	// 양단면이 다를 경우는 eyePos에 따라 빔의 길이가 서로 반대가 된다.
	{
		xyLT = xyStt + vDir.Rotate90() * (installDir==1 ? dHeight-dCenter:dCenter);
		xyRT = xyLT + vDir*~(xyEnd-xyStt);
		xyRD = xyRT + vDir.RotateInv90() * dHeight;
		xyLD = xyRD - vDir*~(xyEnd-xyStt);
		
		pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
		Dom.LineTo(xyLT, xyRT);
		Dom.LineTo(xyLD, xyRD);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyRT, xyRD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyLT, xyLD);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyLT, xyLD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyRT, xyRD);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
			Dom.LineTo(xyLT, xyLD);
			Dom.LineTo(xyRT, xyRD);
		}
		if(installDir==0 || installDir==1)
		{
			if(installDir==0 || installDir==2)
				pOptStd->SetEnvType(&Dom, HCAD_STLH);
			else
				pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);

			Dom.MoveTo(xyLT + vDir.RotateInv90()*dT1);
			Dom.LineTo(xyRT + vDir.RotateInv90()*dT1);
		}
		else
		{
			if(installDir==0 || installDir==2)
				pOptStd->SetEnvType(&Dom, HCAD_STLH);
			else
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
			
			Dom.MoveTo(xyLD + vDir.Rotate90()*dT1);
			Dom.LineTo(xyRD + vDir.Rotate90()*dT1);
		}
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xyRT-xyLT).Unit();
			CDPoint xyPos = xyLT + vAng * ~(xyRT-xyLT)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	*pDom << Dom;		
}

// pBx         = 가로보 또는 브레이싱BX
// installDir  = 빔의 설치방향(문서참조)
// eyePos      = 빔을 보는 시야의 방향(0:정면, 1:평면)
// xyStt       = 골조의 시점
// xyEnd       = 골조의 종점
// nUsingType  = 수직브레이싱인지 수평브레이싱인가?
// nDrawingType= 부재의 시,종점위치결정
// bJewon      = 제원 표시유무
// bEndArrow   = 상세도에서 CuttingLine 처리 하기 위해서
// nInstallPos = 부재의 설치위치(수직브레이싱: UP, DOWN, LEFT, RIGHT)
//								(수평브레이싱: LEFT, RIGHT)
void CAPlateDrawSteel::DrawBeamC   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden)

{
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd *pOptStd = m_pDataMng->GetOptionStd();
	CSection *pSec = pBx->GetSection();
	CAPlateDrawCross DrawCross(m_pDataMng);
	CGlobarOption	*pOpt = m_pDataMng->GetGlobalOption(); 

	double dT1, dT2, dCenter, dHeight;
	if(nUsingType==VBRACING)
	{
		dT1 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T1);
		dT2 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T2);
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, eyePos==0?CSection::CENTERDX:CSection::CENTERDY);		
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, TRUE, eyePos==0 ? TRUE : FALSE); // 양측중 긴부분
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;
	}
	else
	{
		dT1 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T1);
		dT2 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T2);
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::CENTERDX);		
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, FALSE, eyePos==0 ? TRUE : FALSE); // 양측중 작은 부분
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;
	}
	
	double dLength = 400;
	
	CString szJewon = "";
	if(bJewon)
	{
		if(nUsingType==VBRACING)
			szJewon.Format("1-C %s", pGir->GetStringVBracingBeamJewon(pBx, nInstallPos));
		else if(nUsingType==HBRACING)
			szJewon.Format("1-C %s", pGir->GetStringHBracingBeamJewon(pBx, nInstallPos));
	}
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	CDPoint xyLT, xyRT, xyRD, xyLD;
	CDPoint xyCen = (xyStt+xyEnd)/2;
	
	if(nDrawingType==FROM_STT)
		xyEnd = xyStt+vDir*dLength;
	else if(nDrawingType==FROM_END)
		xyStt = xyEnd-vDir*dLength;	
	else if(nDrawingType==FROM_MID)
	{
		xyStt = xyCen-vDir*dLength;	
		xyEnd = xyCen+vDir*dLength;
	}
	else
	{
		xyStt = xyStt;
		xyEnd = xyEnd;
	}
	
	//연직 절단 적용(Issue:6686)
	CDPoint vDirCurrent = vDir;
	
	//정면도
	if(!eyePos)
		vDir = CDPoint(1,0);
	//

	xyLT = xyStt + vDir.Rotate90() * (installDir==1 ? dHeight-dCenter:dCenter);
	xyRT = xyLT  + vDirCurrent*~(xyEnd-xyStt);
	xyRD = xyRT  + vDir.RotateInv90() * dHeight;
	xyLD = xyRD  - vDirCurrent*~(xyEnd-xyStt);

	if(eyePos==0)  //정면도
	{
		
		Dom.LineTo(xyLT, xyRT);
		Dom.LineTo(xyLD, xyRD);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyRT, xyRD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyLT, xyLD);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyLT, xyLD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyRT, xyRD);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(xyLT, xyLD);
			Dom.LineTo(xyRT, xyRD);
		}

		if(installDir==0)  //정면설치(거더방향)
			pOptStd->SetEnvType(&Dom,HCAD_STLC);		
		else  //반대설치(거더반대방향)
			pOptStd->SetEnvType(&Dom, HCAD_STLH);		

		Dom.MoveTo(xyLT + vDir.RotateInv90()*dT2);
		Dom.LineTo(xyRT + vDir.RotateInv90()*dT2);
		Dom.MoveTo(xyLD + vDir.Rotate90()*dT2);
		Dom.LineTo(xyRD + vDir.Rotate90()*dT2);
	}
	else   //평면도
	{
		Dom.LineTo(xyLT, xyRT);
		Dom.LineTo(xyLD, xyRD);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyRT, xyRD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyLT, xyLD);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xyLT, xyLD, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xyRT, xyRD);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
			Dom.LineTo(xyLT, xyLD);
			Dom.LineTo(xyRT, xyRD);
		}

		pOptStd->SetEnvType(&Dom, HCAD_STLH);		
		if(installDir==0)  //정면설치(거더방향)
		{
			Dom.MoveTo(xyLT + vDir.RotateInv90()*dT1);
			Dom.LineTo(xyRT + vDir.RotateInv90()*dT1);
		}
		else//반대설치(거더반대방향)
		{
			Dom.MoveTo(xyLD + vDir.Rotate90()*dT1);
			Dom.LineTo(xyRD + vDir.Rotate90()*dT1);
		}
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xyRT-xyLT).Unit();
			CDPoint xyPos = xyLT + vAng * ~(xyRT-xyLT)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	*pDom << Dom;	
}

// pBx         = 가로보 또는 브레이싱BX
// installDir  = 빔의 설치방향(문서참조)
// eyePos      = 빔을 보는 시야의 방향(0:정면, 1:평면)
// xyStt       = 골조의 시점
// xyEnd       = 골조의 종점
// nUsingType  = 수직브레이싱인지 수평브레이싱인가?
// nDrawingType= 부재의 시,종점위치결정
// bJewon      = 제원 표시유무
// bEndArrow   = 상세도에서 CuttingLine 처리 하기 위해서
// nInstallPos = 부재의 설치위치(수직브레이싱: UP, DOWN, LEFT, RIGHT)
//								(수평브레이싱: LEFT, RIGHT)
void CAPlateDrawSteel::DrawBeamCT   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden)

{
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();
	CSection *pSec = pBx->GetSection();
	CAPlateDrawCross DrawCross(m_pDataMng);
	CGlobarOption	*pOpt = m_pDataMng->GetGlobalOption(); 

	double dT1, dT2, dHeight, dCenter;
	if(nUsingType==VBRACING)
	{
		dT1 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T1);
		dT2 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T2);
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, TRUE, eyePos==0 ? TRUE : FALSE); // 양측중 긴부분
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, eyePos==0?CSection::CENTERDX:CSection::CENTERDY);		
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;
	}
	else
	{
		dT1 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T1);
		dT2 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T2);
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, FALSE, eyePos==0 ? TRUE : FALSE); // 양측중 작은 부분
		dCenter = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::CENTERDX);		
		if(!pOpt->GetInstallBeamAtCenter())
			dCenter = dHeight/2;
	}
	
	double dLength = 400;

	CString szJewon = "";
	if(bJewon)
	{
		if(nUsingType==VBRACING)
			szJewon.Format("1-CT %s", pGir->GetStringVBracingBeamJewon(pBx, nInstallPos));
		else if(nUsingType==HBRACING)
			szJewon.Format("1-CT %s", pGir->GetStringHBracingBeamJewon(pBx, nInstallPos));
	}
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	CDPoint xyLT, xyRT, xyRD, xyLD;
	CDPoint xyCen = (xyStt+xyEnd)/2;

	if(nDrawingType==FROM_STT)
		xyEnd = xyStt+vDir*dLength;
	else if(nDrawingType==FROM_END)
		xyStt = xyEnd-vDir*dLength;	
	else if(nDrawingType==FROM_MID)
	{
		xyStt = xyCen-vDir*dLength;	
		xyEnd = xyCen+vDir*dLength;
	}
	else
	{
		xyStt = xyStt;
		xyEnd = xyEnd;
	}

	CDPoint xy1, xy2, xy3, xy4, xy5, xy6, xy7, xy8;
	
	if(eyePos==0) //정면도
	{
		if(installDir==0)
		{
			xy1 = xyStt + vDir.Rotate90()*(dCenter);
			xy2 = xyStt + vDir.RotateInv90()*(dHeight-dCenter);
			xy3 = xyEnd + vDir.RotateInv90()*(dHeight-dCenter);
			xy4 = xyEnd + vDir.Rotate90()*(dCenter);
		}
		else
		{
			xy1 = xyStt + vDir.Rotate90()*(dHeight-dCenter);
			xy2 = xyStt + vDir.RotateInv90()*(dCenter);
			xy3 = xyEnd + vDir.RotateInv90()*(dCenter);
			xy4 = xyEnd + vDir.Rotate90()*(dHeight-dCenter);
		}
		Dom.LineTo(xy1, xy4);
		Dom.LineTo(xy2, xy3);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy4, xy3, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy1, xy2);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy1, xy2, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy4, xy3);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(xy1, xy2);
			Dom.LineTo(xy4, xy3);
		}
		if(installDir==0)
		{
			xy5 = xyStt + vDir.Rotate90()*(dCenter-dT2);
			xy6 = xyEnd + vDir.Rotate90()*(dCenter-dT2);
		}
		else
		{
			xy5 = xyStt + vDir.RotateInv90()*(dCenter-dT2);
			xy6 = xyEnd + vDir.RotateInv90()*(dCenter-dT2);
		}
		Dom.LineTo(xy5, xy6);

		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xy4-xy1).Unit();
			CDPoint xyPos = xy1 + vAng * ~(xy4-xy1)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}	
	}
	else  		//평면도
	{
		xy1 = xyStt + vDir.Rotate90()*(dHeight/2);
		xy2 = xyStt + vDir.RotateInv90()*(dHeight/2);
		xy3 = xyEnd + vDir.RotateInv90()*(dHeight/2);
		xy4 = xyEnd + vDir.Rotate90()*(dHeight/2);
		xy5 = xyStt + vDir.Rotate90()*(dT1/2);
		xy6 = xyStt + vDir.RotateInv90()*(dT1/2);
		xy7 = xyEnd + vDir.RotateInv90()*(dT1/2);
		xy8 = xyEnd + vDir.Rotate90()*(dT1/2);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy4, xy3, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy1, xy2);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy1, xy2, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy4, xy3);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
			Dom.LineTo(xy1, xy2);
			Dom.LineTo(xy4, xy3);
		}	

		pOptStd->SetEnvType(&Dom,bHidden ? HCAD_STLH : HCAD_STLC);//
		Dom.LineTo(xy1, xy4);					//  수평브레이싱 
		Dom.LineTo(xy2, xy3);					//  수평브레이싱 
		if(installDir==0)
			pOptStd->SetEnvType(&Dom, HCAD_STLH);
		else//반대설치(거더반대방향)
			pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
		Dom.LineTo(xy5, xy8);
		Dom.LineTo(xy6, xy7);


		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xy4-xy1).Unit();
			CDPoint xyPos = xy1 + vAng * ~(xy4-xy1)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	*pDom << Dom;	
}

// pBx         = 가로보 또는 브레이싱BX
// installDir  = 빔의 설치방향(문서참조)
// eyePos      = 빔을 보는 시야의 방향(0:정면, 1:평면)
// xyStt       = 골조의 시점
// xyEnd       = 골조의 종점
// nUsingType  = 수직브레이싱인지 수평브레이싱인가?
// nDrawingType= 부재의 시,종점위치결정
// bJewon      = 제원 표시유무
// bEndArrow   = 상세도에서 CuttingLine 처리 하기 위해서
// nInstallPos = 부재의 설치위치(수직브레이싱: UP, DOWN, LEFT, RIGHT)
//								(수평브레이싱: LEFT, RIGHT)
void CAPlateDrawSteel::DrawBeamI   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden)

{
	
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();
	CAPlateDrawCross DrawCross(m_pDataMng);
	CSection *pSec = pBx->GetSection();
	CGlobarOption	*pOpt = m_pDataMng->GetGlobalOption(); 

	double dT1, dT2, dHeight;
	if(nUsingType==VBRACING)
	{
		dT1 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T1);
		dT2 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T2);
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, TRUE, eyePos==0 ? TRUE : FALSE); // 양측중 긴부분
	}
	else
	{
		dT1 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T1);
		dT2 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T2);
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, FALSE, eyePos==0 ? TRUE : FALSE); // 양측중 작은 부분
	}
	
	double dLength = 400;

	CString szJewon = "";
	if(bJewon)
	{
		if(nUsingType==VBRACING)
			szJewon.Format("1-I %s", pGir->GetStringVBracingBeamJewon(pBx, nInstallPos));
		else if(nUsingType==HBRACING)
			szJewon.Format("1-I %s", pGir->GetStringHBracingBeamJewon(pBx, nInstallPos));
	}
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	CDPoint xyLT, xyRT, xyRD, xyLD;
	CDPoint xyCen = (xyStt+xyEnd)/2;

	if(nDrawingType==FROM_STT)
		xyEnd = xyStt+vDir*dLength;
	else if(nDrawingType==FROM_END)
		xyStt = xyEnd-vDir*dLength;	
	else if(nDrawingType==FROM_MID)
	{
		xyStt = xyCen-vDir*dLength;	
		xyEnd = xyCen+vDir*dLength;
	}
	else
	{
		xyStt = xyStt;
		xyEnd = xyEnd;
	}

	CDPoint xy1, xy2, xy3, xy4, xy5, xy6, xy7, xy8;
	if(eyePos==0)		//정면도
	{
		xy1 = xyStt + vDir.Rotate90()*(dHeight/2);
		xy2 = xyStt + vDir.RotateInv90()*(dHeight/2);
		xy3 = xyEnd + vDir.RotateInv90()*(dHeight/2);
		xy4 = xyEnd + vDir.Rotate90()*(dHeight/2);
		xy5 = xyStt + vDir.Rotate90()*(dHeight/2-dT2);
		xy6 = xyStt + vDir.RotateInv90()*(dHeight/2-dT2);
		xy7 = xyEnd + vDir.RotateInv90()*(dHeight/2-dT2);
		xy8 = xyEnd + vDir.Rotate90()*(dHeight/2-dT2);
		Dom.LineTo(xy1, xy4);
		Dom.LineTo(xy2, xy3);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy4, xy3, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy1, xy2);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy1, xy2, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy4, xy3);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(xy1, xy2);
			Dom.LineTo(xy4, xy3);
		}
		pOptStd->SetEnvType(&Dom,HCAD_STLC);

		Dom.LineTo(xy6, xy7);
		Dom.LineTo(xy5, xy8);
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xy4-xy1).Unit();
			CDPoint xyPos = xy1 + vAng * ~(xy4-xy1)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	else		//평면도
	{
		xy1 = xyStt + vDir.Rotate90()*(dHeight/2);
		xy2 = xyStt + vDir.RotateInv90()*(dHeight/2);
		xy3 = xyEnd + vDir.RotateInv90()*(dHeight/2);
		xy4 = xyEnd + vDir.Rotate90()*(dHeight/2);
		xy5 = xyStt + vDir.Rotate90()*(dT1/2);
		xy6 = xyStt + vDir.RotateInv90()*(dT1/2);
		xy7 = xyEnd + vDir.RotateInv90()*(dT1/2);
		xy8 = xyEnd + vDir.Rotate90()*(dT1/2);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy4, xy3, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy1, xy2);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy1, xy2, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy4, xy3);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
			Dom.LineTo(xy1, xy2);
			Dom.LineTo(xy4, xy3);
		}		
		//평면도
		pOptStd->SetEnvType(&Dom,bHidden ? HCAD_STLH : HCAD_STLC);
		Dom.LineTo(xy1, xy4);
		Dom.LineTo(xy2, xy3);

		pOptStd->SetEnvType(&Dom, HCAD_STLH);
		Dom.LineTo(xy5, xy8);
		Dom.LineTo(xy6, xy7);
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xy4-xy1).Unit();
			CDPoint xyPos = xy1 + vAng * ~(xy4-xy1)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	*pDom << Dom;		
}

// pBx         = 가로보 또는 브레이싱BX
// installDir  = 빔의 설치방향(문서참조)
// eyePos      = 빔을 보는 시야의 방향(0:정면, 1:평면)
// xyStt       = 골조의 시점
// xyEnd       = 골조의 종점
// nUsingType  = 수직브레이싱인지 수평브레이싱인가?
// nDrawingType= 부재의 시,종점위치결정
// bJewon      = 제원 표시유무
// bEndArrow   = 상세도에서 CuttingLine 처리 하기 위해서
// nInstallPos = 부재의 설치위치(수직브레이싱: UP, DOWN, LEFT, RIGHT)
//								(수평브레이싱: LEFT, RIGHT)
void CAPlateDrawSteel::DrawBeamH   (CDomyun *pDom, CPlateBasicIndex* pBx, long installDir, 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden)

{
	CPlateGirderApp *pGir = (CPlateGirderApp *)pBx->GetGirder();
	CARoadOptionStd  *pOptStd  = m_pDataMng->GetOptionStd();
	CAPlateDrawCross DrawCross(m_pDataMng);
	CSection *pSec = pBx->GetSection();
	CGlobarOption	*pOpt = m_pDataMng->GetGlobalOption(); 

	double dT1, dT2, dHeight;
	if(nUsingType==VBRACING)
	{
		dT1 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T1);
		dT2 = pBx->GetSection()->GetBeamJewon(nInstallPos, CSection::T2);
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, TRUE, eyePos==0 ? TRUE : FALSE); // 양측중 긴부분
	}
	else
	{
		dT1 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T1);
		dT2 = pBx->GetBracing()->GetBeamJewon(nInstallPos, FALSE, CBracing::T2);
		dHeight	= pSec->GetBeamBaseHeight(nInstallPos, FALSE, eyePos==0 ? TRUE : FALSE); // 양측중 작은 부분
	}
	
	double dLength = 400;

	CString szJewon = "";
	if(bJewon)
	{
		if(nUsingType==VBRACING)
			szJewon.Format("1-H %s", pGir->GetStringVBracingBeamJewon(pBx, nInstallPos));
		else if(nUsingType==HBRACING)
			szJewon.Format("1-H %s", pGir->GetStringHBracingBeamJewon(pBx, nInstallPos));
	}
		
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CDPoint vDir = (xyEnd - xyStt).Unit();
	CDPoint xyLT, xyRT, xyRD, xyLD;
	CDPoint xyCen = (xyStt+xyEnd)/2;
	
	if(nDrawingType==FROM_STT)
		xyEnd = xyStt+vDir*dLength;
	else if(nDrawingType==FROM_END)
		xyStt = xyEnd-vDir*dLength;	
	else if(nDrawingType==FROM_MID)
	{
		xyStt = xyCen-vDir*dLength;	
		xyEnd = xyCen+vDir*dLength;
	}
	else
	{
		xyStt = xyStt;
		xyEnd = xyEnd;
	}


	CDPoint xy1, xy2, xy3, xy4, xy5, xy6, xy7, xy8;
	if(eyePos==0)		//정면도
	{
		xy1 = xyStt + vDir.Rotate90()*(dHeight/2);
		xy2 = xyStt + vDir.RotateInv90()*(dHeight/2);
		xy3 = xyEnd + vDir.RotateInv90()*(dHeight/2);
		xy4 = xyEnd + vDir.Rotate90()*(dHeight/2);
		xy5 = xyStt + vDir.Rotate90()*(dHeight/2-dT2);
		xy6 = xyStt + vDir.RotateInv90()*(dHeight/2-dT2);
		xy7 = xyEnd + vDir.RotateInv90()*(dHeight/2-dT2);
		xy8 = xyEnd + vDir.Rotate90()*(dHeight/2-dT2);
		Dom.LineTo(xy1, xy4);
		Dom.LineTo(xy2, xy3);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy4, xy3, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy1, xy2);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy1, xy2, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy4, xy3);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			Dom.LineTo(xy1, xy2);
			Dom.LineTo(xy4, xy3);
		}
		pOptStd->SetEnvType(&Dom,HCAD_STLC);

		Dom.LineTo(xy6, xy7);
		Dom.LineTo(xy5, xy8);
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xy4-xy1).Unit();
			CDPoint xyPos = xy1 + vAng * ~(xy4-xy1)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	else		//평면도
	{
		xy1 = xyStt + vDir.Rotate90()*(dHeight/2);
		xy2 = xyStt + vDir.RotateInv90()*(dHeight/2);
		xy3 = xyEnd + vDir.RotateInv90()*(dHeight/2);
		xy4 = xyEnd + vDir.Rotate90()*(dHeight/2);
		xy5 = xyStt + vDir.Rotate90()*(dT1/2);
		xy6 = xyStt + vDir.RotateInv90()*(dT1/2);
		xy7 = xyEnd + vDir.RotateInv90()*(dT1/2);
		xy8 = xyEnd + vDir.Rotate90()*(dT1/2);
		if(bEndArrow)
		{
			if(nDrawingType==FROM_STT || nDrawingType==ALL)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy4, xy3, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy1, xy2);
			}
			else if(nDrawingType==FROM_END)
			{
				pOptStd->SetEnvType(&Dom,HCAD_CUTL);
				Dom.CutLightningAnyDir(xy1, xy2, Dom.Always(2));
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				Dom.LineTo(xy4, xy3);
			}
		}
		else
		{
			pOptStd->SetEnvType(&Dom, bHidden ? HCAD_STLH : HCAD_STLC);
			Dom.LineTo(xy1, xy2);
			Dom.LineTo(xy4, xy3);
		}		
		//평면도
		pOptStd->SetEnvType(&Dom,bHidden ? HCAD_STLH : HCAD_STLC);
		Dom.LineTo(xy1, xy4);
		Dom.LineTo(xy2, xy3);

		pOptStd->SetEnvType(&Dom, HCAD_STLH);
		Dom.LineTo(xy5, xy8);
		Dom.LineTo(xy6, xy7);
		if(bJewon && !szJewon.IsEmpty())
		{
			pOptStd->SetEnvType(&Dom,HCAD_DIML);
			CDPoint vAng = (xy4-xy1).Unit();
			CDPoint xyPos = xy1 + vAng * ~(xy4-xy1)/2 + vAng.Rotate90() * Dom.Always(1);
			Dom.SetTextAngle(vAng.GetAngleDegree());
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(xyPos, szJewon);
		}
	}
	*pDom << Dom;		
}
	
// pBx         = 가로보 또는 브레이싱BX
// nBeamType   = 빔의 종류
// installDir[4] = 빔의 설치방향(문서참조) => 0:L형강, 1:부등변L형강, 2: ㄷ형강, 3: CT형강
// eyePos      = 빔을 보는 시야의 방향(0:정면, 1:평면)
// xyStt       = 골조의 시점
// xyEnd       = 골조의 종점
// nUsingType  = 수직브레이싱인지 수평브레이싱인가?
// nDrawingType= 부재의 시,종점위치결정
// bJewon      = 제원 표시유무
// bEndArrow   = 상세도에서 CuttingLine 처리 하기 위해서
// nInstallPos = 부재의 설치위치(수직브레이싱: UP, DOWN, LEFT, RIGHT)
//								(수평브레이싱: LEFT, RIGHT)
void CAPlateDrawSteel::DrawBeam   (CDomyun *pDom, CPlateBasicIndex* pBx, long nBeamType, long installDir[4], 
		              long eyePos, CDPoint xyStt, CDPoint xyEnd, long nUsingType, 
					  long nDrawingType, BOOL bJewon, BOOL bEndArrow, long nInstallPos, BOOL bHidden)
{
	if(xyStt == xyEnd) return;

	CDomyun Dom(pDom);
	switch(nBeamType)
	{
	case CSteelMaterialData::LSTEEL:
		DrawBeamL(&Dom, pBx, installDir[0], eyePos, xyStt, xyEnd, nUsingType, nDrawingType, bJewon, bEndArrow, nInstallPos, bHidden); 
		break;
	case CSteelMaterialData::LVALSTEEL:
		DrawBeamLVal(&Dom, pBx, installDir[1], eyePos, xyStt, xyEnd, nUsingType, nDrawingType, bJewon, bEndArrow, nInstallPos, bHidden); 
		break;
	case CSteelMaterialData::CSTEEL:
		DrawBeamC(&Dom, pBx, installDir[2], eyePos, xyStt, xyEnd, nUsingType, nDrawingType, bJewon, bEndArrow, nInstallPos, bHidden); 
		break;
	case CSteelMaterialData::CTSTEEL:
		DrawBeamCT(&Dom, pBx, installDir[3], eyePos, xyStt, xyEnd, nUsingType, nDrawingType, bJewon, bEndArrow, nInstallPos, bHidden); 
		break;
	case CSteelMaterialData::ISTEEL:
		DrawBeamI(&Dom, pBx, 0, eyePos, xyStt, xyEnd, nUsingType, nDrawingType, bJewon, bEndArrow, nInstallPos, bHidden); 
		break;
	case CSteelMaterialData::HSTEEL:
		DrawBeamH(&Dom, pBx, 0, eyePos, xyStt, xyEnd, nUsingType, nDrawingType, bJewon, bEndArrow, nInstallPos, bHidden); 
		break;
	}

	*pDom << Dom;
}
