// APlateOutHBracing.cpp: implementation of the CAPlateOutHBracing class.
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
#define MPE_HBRACING_PLAN        100
#define MPE_HBRACING_SANGSE      150
#define MPE_HBRACING_SANGSE_TITLE 160

#define MPE_HBRACING_COORD       110
#define MPE_HBRACING_TABLE       120
#define MPE_HBRACING_LIST        130
#define MPE_HBRACING_LIST_TITLE	 131
#define MPE_HBRACING_LEGEND      140
#define MPE_HBRACING_LEGEND_TITLE 141
#define MPE_HBRACING_TABLE_TITLE  120

CAPlateOutHBracing::CAPlateOutHBracing()
{
	nSangseSu = 8;
}
  
CAPlateOutHBracing::~CAPlateOutHBracing()
{

}

void CAPlateOutHBracing::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CDrawPageMng    *pPageMng	= pStd->GetDrawPageMng();	
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	COutMovePoint	*pMove	    = pStd->GetMovePoint(pStd->m_nCurPage);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
		
	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// �Ϲݵ������� �����ȣ
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	

	// ����
	pPageMng->SetProgressData("���� �����...",0);
	pStd->GetDogak()->Add(&Dom,nPage,Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	CString str;
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);	
	pStd->AddMaterial(&Dom,Scale);
	pStd->AddJubujebubuje(&Dom,Scale);
	*pDom << Dom;			

	pDom->SetCalcExtRect();
	CDRect	DgkBound = pDom->GetExtRect();
	// ��鵵
	if(nSubPage <= GetPagesuGeneralTotal())
	{
		if(pDomOpt->m_HBracing_Plan)
		{
			CDPoint xyMove = CDPoint(Dom.Always(80), DgkBound.Height()-Dom.Always(80));
//			EXIT_RTN;
			pPageMng->SetProgressData("��鵵 ����� ...",10);
			AddPlan(&Dom,nSubPage);

			Dom.SetCalcExtRect();	
			CDRect rect = Dom.GetExtRect();
			Dom.Move(xyMove);
			Dom.Move(-rect.left, -rect.top-rect.Height()); 	

			Dom.Move(pMove->GetPoint(MPE_HBRACING_PLAN));
			Dom.SetCalcExtRect();	
			rect = Dom.GetExtRect();	
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,rect,MPE_HBRACING_PLAN,"��鵵");		
			*pDom << Dom;	

			DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "�� �� ��",TRUE, TRUE, 0);
			Dom.Move(pMove->GetPoint(MPE_HBRACING_PLAN+1));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
			pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_HBRACING_PLAN+1,"��鵵 ����");	
			*pDom << Dom;
		}
	}
	else
	{
		if(pDomOpt->m_HBracing_Detail)
		{
//			EXIT_RTN;
			pPageMng->SetProgressData("�󼼵� ����� ...",10);
			AddPlanDetail(&Dom, nSubPage);
		}
		*pDom << Dom;
	}	

	pPageMng->SetProgressData("��¿Ϸ�",100);
}

long CAPlateOutHBracing::GetPageSu()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(0);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	m_nPage = 0;
	long nTotal  = 0;

	switch(pOpt->m_Gen_nDivPage)
	{
		case 0:
			nTotal = pBridge->m_nQtyJigan;
			m_nPage = nTotal/pOpt->m_Gen_nUnitEaDraw;
			if(nTotal%pOpt->m_Gen_nUnitEaDraw)
				m_nPage++;
			break;
		case 1:
			nTotal  = pGir->GetQtySplice()+1;
			m_nPage = nTotal/pOpt->m_Gen_nUnitEaDraw;
			if(nTotal%pOpt->m_Gen_nUnitEaDraw)
				m_nPage++;
			break;
		case 2:
			nTotal = pGir->GetQtyCrossBeamVBracing()+1; 
			m_nPage = nTotal/pOpt->m_Gen_nUnitEaDraw;
			if(nTotal%pOpt->m_Gen_nUnitEaDraw)
				m_nPage++;
			break;
		case 3:
			m_nPage = 1;
			break;
	}
	//
	MakeHBracingGussetList();

	if (m_HBracingGussetList.GetSize() == 0)
	{
		m_nPage = 0;
		return m_nPage;
	}
	else
	{
		if(m_HBracingGussetList.GetSize()%nSangseSu==0)
			m_nPage += m_HBracingGussetList.GetSize()/nSangseSu;
		else
			m_nPage += long(m_HBracingGussetList.GetSize()/nSangseSu) + 1;

		return m_nPage;
	}

	return m_nPage;
}


void CAPlateOutHBracing::MakeHBracingGussetList()
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge  = pStd->GetDataManage()->GetBridge();
	BOOL bAllGirder = TRUE;//pDomOpt->->m_Cross_AllGirder;

	CTypedPtrArray	<CObArray, CHBracingGusset*> crListTemp;
	crListTemp.RemoveAll();

	m_HBracingGussetList.RemoveAll();

	long GirderSu = pBridge->GetGirdersu();
	long n = 0;
	for (n = 0; n < GirderSu; n++)
	{
		CPlateGirderApp	*pGir = pBridge->GetGirder(n);
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);

		while(pBx)
		{
			CHBracingGusset* pBrGu = pBx->GetHBracingGusset();
			if(pBrGu->IsReal())
			{
				long nGNum1 = pBx->GetNumberGirder();
				long nSize = crListTemp.GetSize();
				long i = 0;
				for (i = 0; i < nSize; i++)
				{
					CHBracingGusset *pBrGuComp = crListTemp.GetAt(i);
					long nGNum2 = pBrGuComp->GetBx()->GetNumberGirder();
					if (bAllGirder)
					{
						if(pBrGu->IsSamePlateJewon(pBrGuComp) && nGNum1 == nGNum2)
							break;
					}
					else
					{
						if(pBrGu->IsSamePlateJewon(pBrGuComp))
							break;
					}
				}
				if (i == nSize) {
					crListTemp.Add(pBrGu);
				}
			}

			pBx = Finder.GetBxNext();
		}
	}

	//sort ������ - �Ϲݺ�
	
	for (n = 0; n < crListTemp.GetSize(); n++)
	{
		if (crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_HBracingGussetList.Add(crListTemp.GetAt(n));
	}
	for (n = 0; n < crListTemp.GetSize(); n++)
	{
		if (!crListTemp.GetAt(n)->GetBx()->IsJijum())
			m_HBracingGussetList.Add(crListTemp.GetAt(n));
	}
	

}


double CAPlateOutHBracing::GetScale()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd *pOpt       = pStd->GetDomOptionStd();

	return pOpt->m_HBracing_Scale.GetAt(0);
}

CString CAPlateOutHBracing::GetTitle()
{
	CString szTitle;
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;

	long nPageSu = m_nPage;
	if(nPageSu>1)
		szTitle.Format("����극�̽� �󼼵� (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("����극�̽� �󼼵�");

	return szTitle;	
}

// �Ϲݵ� ��� ���� �׸���
void CAPlateOutHBracing::AddPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd		*pOpt		= pStd->GetDataManage()->GetOptionStd();
	COptionStd			*pDomOpt    = pStd->GetDomOptionStd();
	CPlateBridgeApp		*pBridge	= pStd->GetDataManage()->GetBridge();
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateBasicIndex	*pBxStt		= GetPageBxStt(CENTERGIRDER,nSubPage);
	CPlateBasicIndex	*pBxEnd		= GetPageBxEnd(CENTERGIRDER,nSubPage);
	CString str = _T("");
	
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	long nG = 0;
	CPlateBasicIndex *pBxLeftStt = GetPageBxStt(nG, nSubPage,FALSE);
	CPlateBasicIndex *pBxLeftEnd = GetPageBxEnd(nG, nSubPage,FALSE);
	nG = pBridge->GetGirdersu()-1;
	CPlateBasicIndex *pBxRightStt = GetPageBxStt(nG, nSubPage,FALSE);
	CPlateBasicIndex *pBxRightEnd = GetPageBxEnd(nG, nSubPage,FALSE);
	
	DrawPyung.DrawSlabByBx(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);
	DrawPyung.DrawCenterLine(&Dom,-1,pBxStt->GetStation(),pBxEnd->GetStation());		

	for( nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		DrawPyung.DrawHBracingCLWithenBx(&Dom, pBxStt, pBxEnd);
		DrawPyung.DrawCRBeamCLWithenBx(&Dom, pBxStt, pBxEnd);//���κ� �߽ɼ����׸�		
	}
	
	//�������� ��� �׷����� ������ ������
	for(nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		DrawPyung.DrawHBracingSpliceWithenBx(&Dom, pBxStt, pBxEnd, TRUE);	
	}
	for(nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		DrawPyung.DrawHBracingBeamWithenBx(&Dom, pBxStt, pBxEnd, TRUE);	
	}

	for(nG = 0; nG < pBridge->GetGirdersu()-1; nG++)
	{
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		DrawPyung.DrawCRBeamVBracingFlangeWithenBx(&Dom, pBxStt, pBxEnd, FALSE, FALSE, TRUE);
	}

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);				

	for(nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp	*pGirCur = pBridge->GetGirder(nG);
		CPlateBxFinder	Finder(pGirCur);
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | BX_UPPERFLANGE, pBxStt, pBxEnd);				
		if(pBx != pBxStt)
		{
			//DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBx, TRUE, 0);	
			DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, TRUE, 0);					
		}
		while(Finder.GetBxFixNext())
		{					
			//DrawPyung.DrawGirderWithenBx(&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0);	
			DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0);	
			pBx = Finder.GetBxNext();
		}			
		if(pBx != pBxEnd)
		{
			//DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, TRUE, 0);	
			DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, TRUE, 0);	
		}

//		DrawPyung.DrawSpliceCut(&Dom, pBxStt, pBxEnd, 0, TRUE, FALSE);
//		DrawPyung.DrawSplicePyung(&Dom, pBxStt, pBxEnd, 0, TRUE, FALSE);
		DrawPyung.DrawSpliceWebPyung(&Dom, pBxStt, pBxEnd, 0, FALSE, FALSE);
		DrawPyung.DrawVStiffWithenBx(&Dom, pBxStt, pBxEnd);
		DrawPyung.DrawJackUpWithenBx(&Dom, pBxStt, pBxEnd);
		DrawPyung.DrawJiJumStiffWithenBx(&Dom, pBxStt, pBxEnd);

		//////////////////////////////////////////////////////////////////////////
		double dStaStt = pBxStt->GetStation();
		double dStaEnd = pBxEnd->GetStation();
		double dTWeb = pGirCur->GetThickFactByStation(dStaStt, G_W);
		
		CDPoint vAngStt = pBxStt->GetAngleSection();
		CDPoint vAngEnd = pBxEnd->GetAngleSection();
		
		CDPoint ptLeftUp	= pGirCur->GetXyGirderDis(dStaStt, -dTWeb/2, vAngStt);
		CDPoint ptLeftDn	= pGirCur->GetXyGirderDis(dStaStt,  dTWeb/2, vAngStt);
		CDPoint ptRightUp	= pGirCur->GetXyGirderDis(dStaEnd, -dTWeb/2, vAngEnd);
		CDPoint ptRightDn	= pGirCur->GetXyGirderDis(dStaEnd,  dTWeb/2, vAngEnd);
		//////////////////////////////////////////////////////////////////////////

		Dom.LineTo(ptLeftUp, ptLeftDn);
		Dom.LineTo(ptRightUp, ptRightDn);		
	}				
	
	pBxStt = GetPageBxStt(0,nSubPage);
	pBxEnd = GetPageBxEnd(0,nSubPage);	
	
	pOpt->SetEnvType(&Dom,HCAD_DIML);		
//	if(pDomOpt->m_HBracing_bDimension)
	{
		DrawDimPyung.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_BASE_FACTUPPER, TRUE, TRUE,  "��� �÷��� ", 2, TRUE);
		DrawDimPyung.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_BASE_SPLICE, TRUE, TRUE,  "�� �� �� �� ", 3, TRUE);
		DrawDimPyung.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_BASE_TOTAL, TRUE, TRUE,  "�� �� ��", 4, TRUE);

		pBxStt = GetPageBxStt(pBridge->GetGirdersu()-1,nSubPage);
		pBxEnd = GetPageBxEnd(pBridge->GetGirdersu()-1,nSubPage);	

		DrawDimPyung.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_BASE_DIAP, FALSE, TRUE,  "���κ�&�����극�̽� ", 0, TRUE);	
		DrawDimPyung.DimPyungTotal(&Dom, pBxStt, pBxEnd, DIM_BASE_FACTLOWER, FALSE, TRUE,  "�Ϻ� �÷��� ", 1, TRUE);

		pBxStt = GetPageBxStt(0,nSubPage);
		pBxEnd = GetPageBxEnd(0,nSubPage);	

		DrawDimPyung.DrawDimHDan(&Dom,pBxStt,TRUE);
		DrawDimPyung.DrawDimHDan(&Dom,pBxEnd,FALSE);
	}

	pBxStt = GetPageBxStt(CENTERGIRDER,nSubPage);	
	pBxEnd = GetPageBxEnd(CENTERGIRDER,nSubPage);	

	double rotAng  = DrawPyung.GetRotateAnglePyung(pBxStt,pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	Dom.Move(-MoveXY);
	Dom.Rotate(rotAng,0,0);			

	*pDom << Dom;
	//�� �ɺ�
	long nCount = m_HBracingGussetList.GetSize();
	CDPoint xyCen;
	
	CPlateGirderApp* pGir = NULL;
	CPlateBasicIndex* pBx = NULL;
	double dRad = 700;
	CDPoint vGir;
	for(long n=0; n<nCount; n++)
	{
		pBx = m_HBracingGussetList.GetAt(n)->GetBx();
		long nG = pBx->GetNumberGirder();
		pBxStt = GetPageBxStt(nG,nSubPage);
		pBxEnd = GetPageBxEnd(nG,nSubPage);	
		pGir = (CPlateGirderApp*)pBx->GetGirder();
		xyCen = pGir->GetXyGirderDis(pBx->GetStation());
		vGir = pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		if( pBxStt->GetStation()<=pBx->GetStation() && pBx->GetStation()<=pBxEnd->GetStation())
		{
			Dom.Circle(xyCen, dRad);
			str.Format("�� %d", n+1);	
			Dom.SetTextAngle(vGir.GetAngleDegree());
			Dom.TextOut(xyCen+vGir.Rotate90()*(dRad+50), str);
		}
	}
	Dom.Move(-MoveXY);
	Dom.Rotate(rotAng,0,0);			
	*pDom << Dom; 
}

CPlateBasicIndex* CAPlateOutHBracing::GetPageBxStt(long nG, long nSubPage, BOOL bFrontMatch, BOOL bNextMatch)
{	 
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	CPlateBasicIndex *pBx = NULL;
	if(nSubPage==1)
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxFirst(BX_STT_GIRDER);
		return pBx;
	}
	long nDivDom = pOpt->m_Gen_nDivPage;
	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum((nSubPage-1)*pOpt->m_Gen_nUnitEaDraw);
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter((nSubPage-1)*pOpt->m_Gen_nUnitEaDraw-1);			
			break;
		case 2 :
			if(nG==-1)
			{
				nG = 0;//pStd->m_pDrawDBStandard->GetNumberGirderNearestToCenterLine();
				pGir = pBridge->GetGirder(nG);
			}
			if(nG>pBridge->GetGirdersu()-2)
			{
				pBx = pGir->m_pLeft->GetBxByCrossNo((nSubPage-1)*pOpt->m_Gen_nUnitEaDraw);
				pBx = pBx ? pBx->GetBxMatchByCrossBeam(pGir) : NULL;
			}
			else
				pBx = pGir->GetBxByCrossNo((nSubPage-1)*pOpt->m_Gen_nUnitEaDraw);
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxFirst(BX_STT_GIRDER);
			break;
	}
	
	if(pBx==NULL)
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxFirst(BX_STT_GIRDER);
	}
	return pBx;
}


CPlateBasicIndex* CAPlateOutHBracing::GetPageBxEnd(long nG, long nSubPage, BOOL bFrontMatch, BOOL bNextMatch)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir		= pBridge->GetGirder(nG);
	COptionStd	    *pOpt       = pStd->GetDomOptionStd();

	long nPageSuGenTotal = GetPagesuGeneralTotal();

	CPlateBasicIndex *pBx = NULL;
	if(nSubPage >= nPageSuGenTotal)
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxLast(BX_END_GIRDER);

		return pBx;
	}
	
	switch(pOpt->m_Gen_nDivPage)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum(nSubPage * pOpt->m_Gen_nUnitEaDraw);
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter(nSubPage * pOpt->m_Gen_nUnitEaDraw-1);
			break;
		case 2 :
			if(nG==-1)
			{
				nG = 0;
				pGir = pBridge->GetGirder(nG);
			}
			if(nG>pBridge->GetGirdersu()-2)
			{
				pBx = pGir->m_pLeft->GetBxByCrossNo(nSubPage*pOpt->m_Gen_nUnitEaDraw);
				pBx = pBx ? pBx->GetBxMatchByCrossBeam(pGir) : NULL;
			}
			else
				pBx = pGir->GetBxByCrossNo(nSubPage*pOpt->m_Gen_nUnitEaDraw);
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxLast(BX_END_GIRDER);
			break;
	}

	if(pBx==NULL)
	{
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxLast(BX_END_GIRDER);
	}
	
	return pBx;
}

void CAPlateOutHBracing::AddPlanDetail(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	 *pStd	 = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd  *pOpt	 = pStd->GetDataManage()->GetOptionStd();
	COptionStd	     *pDomOpt= pStd->GetDomOptionStd();
	COutMovePoint	 *pMove	 = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateBridgeApp  *pBridge= pStd->GetDataManage()->GetBridge();		
	CHBracingGusset  *pBrGu  = NULL;
	CPlateBasicIndex *pBx    = NULL;
	CPlateGirderApp  *pGir   = NULL; 

	CString str = _T("");
	CDPoint xyCen, vGir;	
	CAPlateDrawHBracing DrawHBracing(pStd->GetDataManage());	
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	double dScale = pDomOpt->m_HBracing_Scale.GetAt(1);
	long   nSp    = nSubPage-GetPagesuGeneralTotal()-1;
	CDPoint xyMove;
	CDRect	DgkBound = pDom->GetExtRect();

	for(long n=0; n<nSangseSu; n++)
	{
		CDomyun	Dom(pDom);
		Dom.SetScaleDim(dScale);

		if(nSp*nSangseSu+n > m_HBracingGussetList.GetSize()-1)	return;

		pBrGu = m_HBracingGussetList.GetAt(nSp*nSangseSu+n);
		pBx   = pBrGu->GetBx();
		pGir  = (CPlateGirderApp*)pBx->GetGirder();
		xyCen = pBx->GetGirder()->GetXyGirderDis(pBx->GetStation());
		vGir  =  pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
		
		DrawHBracing.DrawInputHBracingBySangseType(&Dom, pGir, pBrGu->GetSelfCount(), TRUE,FALSE,FALSE, TRUE);
		CDPoint xyCen = pGir->GetXyGirderDis(pBrGu->GetBx()->GetStation());

		Dom.SetScaleDim(GetScale());
		Dom.RedrawByScale(GetScale()/dScale);//Ȯ��
		Dom.Move(-xyCen);//������ǥ�� �������� �̵�

		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();
		Dom.Move(-rect.left, -rect.top-rect.Height()); 	
		double dY = n<4 ? 0 : rect.Height()+Dom.Always(40);
		long nn = n<4 ? n : n-4;
		xyMove.x = Dom.Always(80) + nn*(rect.Width()+Dom.Always(20));
		xyMove.y = DgkBound.Height()-Dom.Always(10)-dY;
		Dom.Move(xyMove);
		//MPE�߰�����
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_HBRACING_SANGSE+n));
		Dom.SetCalcExtRect();	//������ �̵����� ���� ���� ����
		str.Format("�� %d", nSp*nSangseSu+n+1);
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_HBRACING_SANGSE+n, str);		
		*pDom << Dom;
		// TITLE
		DrawStd.AddTitleOnDom(&Dom, dScale, rect, str, TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_HBRACING_SANGSE_TITLE+n));
		Dom.SetCalcExtRect();	//������ �̵����� ���� ���� ����
		str.Format("�� %d ����", nSp*nSangseSu+n+1);
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_HBRACING_SANGSE_TITLE+n, str);
		*pDom << Dom;	
	}
}
	
long CAPlateOutHBracing::GetPagesuGeneralTotal()
{
	long nSangPage = 0;
	if (m_HBracingGussetList.GetSize() == 0)
	{
		nSangPage = 0;
		return nSangPage;
	}
	else
	{
		if(m_HBracingGussetList.GetSize()%nSangseSu==0)
			nSangPage += m_HBracingGussetList.GetSize()/nSangseSu;
		else
			nSangPage += long(m_HBracingGussetList.GetSize()/nSangseSu) + 1;

	}

	return GetPageSu()-nSangPage;
}
