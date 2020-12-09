// APlateOutTendon.cpp: implementation of the CAPlateOutTendon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
#include "../PlateGangje\Gangje.h"
#include "../APlateDBDraw/DimDomyunEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// MPE
const int MPE_TENDON_GEN	 = 10;
const int MPE_SECTION_A		 = 100;
const int MPE_NOTE			 = 200;
const int MPE_TABLE			 = 300;
const int MPE_FIXSANG		 = 400;

CAPlateOutTendon::CAPlateOutTendon()
{
	m_Scale  = 100;
	m_nG     = 0;
	m_pBxStt = NULL;
	m_pBxEnd = NULL;
	m_dLastY = 0;

}

CAPlateOutTendon::~CAPlateOutTendon()
{

}

void CAPlateOutTendon::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB		= pStd->GetDataManage()->GetBridge();
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();	
	COptionStd		*pDomOpt	= pStd->GetDomOptionStd();
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// �Ϲݵ������� �����ȣ
	long nSubPage = nPage - nprePage;

	//�������� ���� Bx ����
	if(pDB->GetGirdersu() >=2)
		m_nG = pDB->GetGirdersu() - 2; 
	else
		m_nG = 0;

	m_pBxStt = GetBxSttPage(nSubPage, m_nG);
	m_pBxEnd = GetBxEndPage(nSubPage, m_nG);

	CDomyun *pDomP = pStd->GetDomyun(nPage);
	pDomP->SetScaleDim(GetScale());
	CDomyun Dom(pDomP);
	// ����
	double dScale = GetScale();
	CDRect rtPlan(0,0,0,0);

	pPageMng->SetProgressData("���� �����...",0);
	//long nSttPage = pDomOpt->m_nDomPageArray.GetAt(PAGE_TENDON) == TRUE ? 0 : pDomOpt->m_nDomPageArray.GetAt(PAGE_TENDON) + nSubPage - 1;	
	//pStd->GetDogak()->Add(&Dom, nPage, dScale, pDomOpt->m_szDGKName, pDomOpt->m_sDomNumHead, "", nSttPage);
	pStd->GetDogak()->Add(&Dom,nPage,dScale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""), FALSE);	
	pStd->AddMaterial(&Dom, dScale);
	pStd->AddJubujebubuje(&Dom, dScale);

	Dom.SetCalcExtRect();
	CDRect dgkRect = Dom.GetExtRect(), DomBound;
	CDPoint xyMove = CDPoint(Dom.Always(100), dgkRect.Height()-Dom.Always(60));

	*pDomP << Dom;

	if(pDomOpt->m_bTendon_PlanView)
	{
		pPageMng->SetProgressData("��鵵 �����...",30);
		AddPlan(&Dom, nSubPage);

		double dY = pDB->GetGirder(0)->m_dGirderCenter;
		Dom.Move(0, dY);

		Dom.SetCalcExtRect();
		CDRect rtPlan = Dom.GetExtRect();
		double MoveX = Dom.Always(220);
		double MoveY = (dgkRect.bottom - rtPlan.bottom) - Dom.Always(90);
		Dom.Move(MoveX, MoveY);
		Dom.Move(pMove->GetPoint(MPE_TENDON_GEN));
		Dom.SetCalcExtRect();	
		CDRect rect = Dom.GetExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, rect, MPE_TENDON_GEN,"��鵵");		

		*pDomP << Dom;

		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "�� �� ��",TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_TENDON_GEN+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(),MPE_TENDON_GEN+1,"��鵵 ����");

		*pDomP << Dom;
	}
	if(pDomOpt->m_Gen_bBasicNote)
	{
//		EXIT_RTN;
		pPageMng->SetProgressData("��Ʈ���� �����...",40);
		InitNote();
		AddNote(&Dom,nSubPage);

		Dom.Move(Dom.Always(-10), Dom.Always(-25));
		Dom.Move(pMove->GetPoint(MPE_NOTE));
		
		Dom.SetCalcExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_NOTE,"��Ʈ����");

		*pDomP << Dom;	
	}

	if(pDomOpt->m_bTendon_SectionAA)
	{
		pPageMng->SetProgressData("�ܸ� A-A �����...",80);
		AddFrontAA(&Dom, nSubPage);
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		double MoveX = 0;
		if (nSubPage==1)		
			MoveX = Dom.Always(250);
		else
			MoveX = Dom.Always(420);

		double MoveY = Dom.Always(200);
		Dom.Move(MoveX, MoveY);
		Dom.Move(pMove->GetPoint(MPE_SECTION_A));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,rect,MPE_SECTION_A,"�ܸ� A-A");
		*pDomP << Dom;

		DrawStd.AddTitleOnDom(&Dom, pDomOpt->m_dTendon_ScaleSectionAA, rect, "�ܸ� A-A", TRUE, TRUE, 0);
		Dom.Move(pMove->GetPoint(MPE_SECTION_A+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SECTION_A+1, "�ܸ� A-A ����");

		*pDomP << Dom;
	}

	if(nSubPage==1)
	{
		pPageMng->SetProgressData("������ �󼼵� �����...",90);
		InsertDomyun(&Dom, nSubPage);
		ReplaceData(&Dom);

		Dom.Move(Dom.Always(80), Dom.Always(0));
		Dom.Move(pMove->GetPoint(MPE_FIXSANG));
		Dom.SetCalcExtRect();	
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),MPE_FIXSANG	,"������ ��");
		*pDomP << Dom;

		//////////////////////////////////////////////////////////////////////////
		pPageMng->SetProgressData("Ⱦ���� TENDON ���ǥ �����...", 98);
		AddTable(&Dom);
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(MPE_TABLE));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,rect,MPE_TABLE,"Ⱦ���� TENDON ���ǥ");
		*pDomP << Dom;

		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "Ⱦ���� TENDON ���ǥ", TRUE, FALSE, 0);
		Dom.Move(pMove->GetPoint(MPE_TABLE+1));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TABLE+1, "Ⱦ���� TENDON ���ǥ");

		*pDomP << Dom;
	}

	pPageMng->SetProgressData("��¿Ϸ�",100);

	*pDomP << Dom;
}

long CAPlateOutTendon::GetPageSu()
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(0);
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	if(pDB->IsPlateGir()) return 0;

	long nTotal  = 0;

	switch(pOpt->m_nTendon_DivType)
	{
		case 0:
			nTotal  = pDB->m_nQtyJigan;
			m_nPage = nTotal/pOpt->m_nTendon_DivNum;
			if(nTotal%pOpt->m_nTendon_DivNum)
				m_nPage++;
			break;
		case 1:
			nTotal  = pGir->GetQtySplice();
			m_nPage = nTotal/pOpt->m_nTendon_DivNum;
			if(nTotal%pOpt->m_nTendon_DivNum)
				m_nPage++;
			break;
		case 2:
			nTotal = pGir->GetQtyCrossBeamVBracing(); 
			m_nPage = nTotal/pOpt->m_nTendon_DivNum;
			if(nTotal%pOpt->m_nTendon_DivNum)
				m_nPage++;
			break;
		case 3:
			m_nPage = 1;
			break;
	}

	if(!pDB->m_bTendonInputComplete)	m_nPage = 0;

	return m_nPage;
}

CString CAPlateOutTendon::GetTitle()
{
	if(m_nPage == 0) return _T("");

	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	long nPageSu = m_nPage;
	CString szTitle;

	if(nPageSu>1)
		szTitle.Format("Ⱦ���� TENDON ��ġ�� (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("Ⱦ���� TENDON ��ġ��");

	return szTitle;
}

double CAPlateOutTendon::GetScale()
{	
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	m_Scale = pOpt->m_dTendon_ScaleGen;

	return m_Scale;
}

// �������� ���� BX�� BX_STT_SLAB
CPlateBasicIndex* CAPlateOutTendon::GetBxSttPage(long nSubPage, long nG, BOOL bSttSlab)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	long nPageSuOrg = GetPageSu();
	long nPageOrg	= nSubPage%nPageSuOrg;
	if((nSubPage%nPageSuOrg)==0)
		nPageOrg = nPageSuOrg;

	CPlateBasicIndex *pBx = NULL;
	long nDivDom   = pOpt->m_nTendon_DivType;
	__int64 BxFlag = bSttSlab ? BX_STT_SLAB : BX_STT_GIRDER;
	
	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum((nPageOrg-1)*pOpt->m_nTendon_DivNum);
			if(nPageOrg==1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BxFlag);
			}
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter((nPageOrg-1)*pOpt->m_nTendon_DivNum);
			if(nPageOrg==1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BxFlag);
			}
			break;
		case 2 :
			if(nPageOrg==1)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxFirst(BxFlag);
			}
			else
			{
				if(nG==-1){
					nG   = pDB->GetNumberGirderNearestToCenterLine();
					pGir = pDB->GetGirder(nG);
				}
				if(nG>pDB->GetGirdersu()-2)
					pBx = pGir->m_pLeft->GetBxByCrossNo((nPageOrg-1)*pOpt->m_nTendon_DivNum)->GetBxMatchByCrossBeam(pGir);
				else
					pBx = pGir->GetBxByCrossNo((nPageOrg-1)*pOpt->m_nTendon_DivNum);
			}
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxFirst(BxFlag);
			break;
	}
	
	return pBx;
}

// �������� ���� BX�� BX_END_SLAB
CPlateBasicIndex* CAPlateOutTendon::GetBxEndPage(long nSubPage, long nG, BOOL bEndSlab)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	COptionStd	    *pOpt = pStd->GetDomOptionStd();

	long nPageSuOrg = GetPageSu();
	long nPageOrg	= nSubPage%nPageSuOrg;
	if((nSubPage%nPageSuOrg)==0)
		nPageOrg = nPageSuOrg;

	CPlateBasicIndex *pBx = NULL;
	long nDivDom   = pOpt->m_nTendon_DivType;
	__int64 BxFlag = bEndSlab ? BX_END_SLAB : BX_END_GIRDER;

	switch(nDivDom)
	{
		case 0 :
			pBx = pGir->GetBxOnJijum(nPageOrg * pOpt->m_nTendon_DivNum);
			if(nPageOrg==nPageSuOrg)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BxFlag);
			}			
			break;
		case 1 :
			pBx = pGir->GetBxBySpliceNoCenter(nPageOrg * pOpt->m_nTendon_DivNum);
			if(nPageOrg==nPageSuOrg)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BxFlag);
			}			
			break;
		case 2 :
			if(nPageOrg==nPageSuOrg)
			{
				CPlateBxFinder Finder(pGir);
				pBx = Finder.GetBxLast(BxFlag);
			}			
			else
			{
				if(nG==-1){
					nG   = pDB->GetNumberGirderNearestToCenterLine();
					pGir = pDB->GetGirder(nG);
				}
				if(nG>pDB->GetGirdersu()-2)
					pBx = pGir->m_pLeft->GetBxByCrossNo(nPageOrg*pOpt->m_nTendon_DivNum)->GetBxMatchByCrossBeam(pGir);
				else
					pBx = pGir->GetBxByCrossNo(nPageOrg*pOpt->m_nTendon_DivNum);
			}
			break;
		case 3 :
			CPlateBxFinder Finder(pGir);
			pBx = Finder.GetBxLast(BxFlag);
			break;
	}
	
	return pBx;
}

void CAPlateOutTendon::AddPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COutMovePoint	*pMove	 = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();
	CSlabApp		*pSlabLeft = pDB->GetSlabLeft();
	CSlabApp		*pSlabRight= pDB->GetSlabRight();

	CAPlateDrawPyung    DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_STLC);

	// ������ �ܰ���
	DrawSlabByPage(&Dom, nSubPage);
	AddPlanGirderLine(&Dom, nSubPage);
	DrawPyung.DrawTendonArr(&Dom, m_pBxStt, m_pBxEnd);

	// ���� BX������ �ι�° ���κ� ��ġ --------------------------------	
	CPlateGirderApp *pGir = m_pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, m_pBxStt, m_pBxEnd);
	pBx = Finder.GetBxNext();
	//------------------------------------------------------------------
	CDPoint xyLeftStt = pSlabLeft->GetXyGirderDis(pBx->GetStation());
	CDPoint xyRightStt= pSlabRight->GetXyGirderDis(pBx->GetStation());

	CSymbolDom SymDom(&Dom, pStd->m_pOptStd);
	SymDom.DrawSymbolSectionGisiArrow((xyLeftStt+xyRightStt)/2, ~(xyLeftStt-xyRightStt), "A", "A", (xyRightStt-xyLeftStt).GetAngleDegree());

	// ġ����
	AddDimPlan(&Dom, nSubPage);

	// �����߽ɼ� (MAIN ����)
	DrawPyung.DrawCenterLine(&Dom, -1, m_pBxStt->GetStation(), m_pBxEnd->GetStation());

	//  ����ȸ��
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, CENTERGIRDER);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, CENTERGIRDER);

	double rotAng  = DrawPyung.GetRotateAnglePyung(pBxStt, pBxEnd);
	CDPoint MoveXY = pBxStt->GetGirder()->GetXyGirderDis(pBxStt->GetStation());
	Dom.Move(-MoveXY);
	Dom.Rotate(rotAng,0,0);
	SymDom.Move(-MoveXY);
	SymDom.Rotate(rotAng,0,0);
	
	*pDom << SymDom;
	*pDom << Dom;
}

void CAPlateOutTendon::AddDimPlan(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOptStd = pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp	*pDB     = pStd->m_pDB;
	COutMovePoint	*pMove	 = pStd->GetMovePoint(pStd->m_nCurPage);
	COptionStd	    *pOpt    = pStd->GetDomOptionStd();

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	
	CAPlateDrawDimPyung DrawDimPyung(pStd->GetDataManage());	

	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	long nPlanDan = 1;

	BOOL bSttPage = (nSubPage==1) ? TRUE : FALSE;
	BOOL bEndPage = (nSubPage==GetPageSu()) ? TRUE : FALSE;
	// ���ġ�� -------------------------------------------
	long nGir = 0;
	CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nGir);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nGir);

	CAPlateDrawDetailDim DetailDim(pStd, &Dom, pBxStt, pBxEnd);

	__int64 DimTypeUpper = 0x0000;
	DimTypeUpper |= DIM_BASE_SPAN;
	DimTypeUpper |= DIM_BASE_TENDON;
	
	DetailDim.SetPyungDimDan(nPlanDan,TRUE,TRUE);
	DetailDim.DimTotal(&Dom, DimTypeUpper, TRUE, TRUE, TRUE, TRUE, bSttPage, bEndPage, FALSE);

	// �ϸ�ġ�� -------------------------------------------
	nGir    = pDB->GetGirdersu()-1;
	pBxStt  = GetBxSttPage(nSubPage,nGir);
	pBxEnd  = GetBxEndPage(nSubPage,nGir);

	CAPlateDrawDetailDim DetailDimLower(pStd, &Dom, pBxStt, pBxEnd);

//	__int64 DimTypeLower = 0x0000;
//	DimTypeLower |= DIM_BASE_SPAN;
//	DimTypeLower |= DIM_BASE_TENDON;
	
	DetailDimLower.SetPyungDimDan(nPlanDan,FALSE,TRUE);
//	DetailDimLower.DimTotal(&Dom, DimTypeLower, FALSE, FALSE, TRUE, TRUE,bSttPage, bEndPage, FALSE);

	// Ⱦ�� ġ�� -------------------------------------------
	pBxStt = GetBxSttPage(nSubPage, -1);
	pBxEnd = GetBxEndPage(nSubPage, -1);
	
	if(pBxStt)
		DrawDimPyung.DrawDimHDan(&Dom, pBxStt, TRUE);
	if(pBxEnd)
		DrawDimPyung.DrawDimHDan(&Dom, pBxEnd, FALSE);

	// Match Line -------------------------------------------
	if(pOpt->m_bTendon_MatchLine)
	{
		long nG = 0;
		pBxStt = GetBxSttPage(nSubPage, nG);
		pBxEnd = GetBxEndPage(nSubPage, nG);
		long nMLDan	   = 5;
		long nRealPage = nSubPage;
		if(!pBxStt->IsState(BX_STT_GIRDER|BX_STT_JOINT))	// �����곪 �Ŵ��� �����δ� ��ġ������ �������� ����
			DimTextPyungMatchLine(&Dom, pBxStt, nRealPage-1, nMLDan);
		if(!pBxEnd->IsState(BX_END_GIRDER|BX_END_JOINT))	// �����곪 �Ŵ��� �����δ� ��ġ������ �������� ����
			DimTextPyungMatchLine(&Dom, pBxEnd, nRealPage  , nMLDan);
	}
	//  -----------------------------------------------------
	*pDom << Dom;
}

void CAPlateOutTendon::InitNote()
{
	m_sNoteArr.RemoveAll();
	m_sNoteArr.Add("1. DUCT�� ö�� �� ��Ӽ����� �������Ѽ� ��ũ��Ʈ");
	m_sNoteArr.Add("   Ÿ���� ������ ����� �Ѵ�.   (�������� 1.0M ����)");
	m_sNoteArr.Add("2. ö�� ��ٰ� DUCT�� ���������� DUCT�� �켱�ð��Ѵ�.");
	m_sNoteArr.Add("3. DUCT�� ������ ����Ǵ� ������ ö�� ����� ������");
	m_sNoteArr.Add("   ������ �տ��� ����� �Ѵ�.");
	m_sNoteArr.Add("4. ������ġ�� ������ P.S��Ʈ���尡 ������ ���� ������");
	m_sNoteArr.Add("   ���� �Ҽ� �ֵ��� �ı��Ǵ� ���� ���� ������ ������ �ʴ�");
	m_sNoteArr.Add("   ������ ������ ���߾�� �ϸ� �ִ� Ȱ���� 6mm�̳�����");
	m_sNoteArr.Add("   ����� ������ �̷������ �Ѵ�.");
	m_sNoteArr.Add("5. ������ ����ö�� ��ġ�ô� ������ ��ٵ��� �����Ͽ��� �Ѵ�. ");
	m_sNoteArr.Add("6. Ⱦ���� �ٴ��� ���ºл��� ���� ������׷� �����Ѵ�.");

}

void CAPlateOutTendon::AddTable(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd		*pOptStd	= pStd->m_pOptStd;	
	CPlateBridgeApp		*pDB		= pStd->GetDataManage()->GetBridge();
	CCalcFloor			CalcFloor(pStd->GetDataManage());

	double Height = 8.0;
	double Width = 30.0;

	CGridDomyunEx Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	Dom.SetColumnCount(8);
	Dom.SetRowCount(3);	
	Dom.SetMargin(3);
	Dom.SetRowHeightAll(Dom.Always(Height));
	Dom.SetColumnWidthAll(Dom.Always(Width));
	Dom.SetColumnWidth(0, Dom.Always(40.0));
	Dom.SetColumnWidth(5, Dom.Always(20));
	Dom.SetColumnWidth(6, Dom.Always(20));
	
	Dom.SetMergeRow(0,1,0);
	Dom.SetMergeRow(0,1,1);
	Dom.SetMergeRow(0,1,2);
	Dom.SetMergeRow(0,1,3);
	Dom.SetMergeRow(0,1,4);
	Dom.SetMergeRow(0,1,7);
	Dom.SetMergeCol(0,5,6);

	Dom.SetTextMatrix(0,0,"Ⱦ���� ��������\n(m)");
	Dom.SetTextMatrix(0,1,"��Ʈ����\n(m)");
	Dom.SetTextMatrix(0,2,"��������\n(ea)");
	Dom.SetTextMatrix(0,3,"���� �� ����\n(m)");
	Dom.SetTextMatrix(0,4,"��Ʈ �� ����\n(m)");
	Dom.SetTextMatrix(0,5,"������ ���� (ea)");
	Dom.SetTextMatrix(0,7,"AIR VENT\n(ea)");

	Dom.SetTextMatrix(1,5,"�����");
	Dom.SetTextMatrix(1,6,"������");

	double	dLength		= CalcFloor.CalcPrestressLength(3);
	double	dDuctLen	= dLength - pDB->m_dLengthDuctOffset;
	long	nTendonQty	= pDB->m_arrTendon.GetSize();

	Dom.SetTextMatrix(2, 0, COMMA(dLength));
	Dom.SetTextMatrix(2, 1, COMMA(dDuctLen));
	Dom.SetTextMatrix(2, 2, COMMA(nTendonQty));
	Dom.SetTextMatrix(2, 3, COMMA(dLength*nTendonQty));
	Dom.SetTextMatrix(2, 4, COMMA(dDuctLen*nTendonQty));
	Dom.SetTextMatrix(2, 5, COMMA(nTendonQty));
	Dom.SetTextMatrix(2, 6, COMMA(nTendonQty));
	Dom.SetTextMatrix(2, 7, COMMA(nTendonQty*2));

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	Dom.Move(Dom.Always(200), Dom.Always(50));

	*pDom << Dom;
}

void CAPlateOutTendon::AddNote(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();

	CDomyun Dom(pDomP);

	DimNoteTypeVertical(&Dom);
	
	CDRect rect;
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	Dom.Move(-rect.left,-rect.top);
	Dom.Move(Dom.Always(50),Dom.Always(50));

	*pDomP << Dom;
}

void CAPlateOutTendon::DimNoteTypeVertical(CDomyun *pDomP)
{
 	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;

	long nSize = m_sNoteArr.GetSize();
	CGridDomyunEx Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetGridLines(GVL_NONE);
	Dom.SetColumnCount(2);
	Dom.SetRowCount(nSize+2);
	Dom.SetColumnWidth(0,Dom.Always(12));
	Dom.SetColumnWidth(1,Dom.Always(130));
	Dom.SetRowHeightAll(Dom.GetTextHeight()*1.4);
	Dom.SetRowHeight(0,Dom.Always(5));
	Dom.SetRowHeight(nSize+1,Dom.Always(5));

	CString str = _T("");
	long nIdxNote = 0;
	for(long i=1; i<nSize+1; i++)
	{
		str = m_sNoteArr.GetAt(i-1);
		if(str.Find(".")==0)
		{
			nIdxNote++;
			Dom.SetTextMatrix(i,0,COMMA(nIdxNote)+". ", GRID_ALIGN_RIGHT);
			str = str.Right(str.GetLength()-1);
		}
		Dom.SetTextMatrix(i,1,str,GRID_ALIGN_LEFT);
	}
	
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL2);
	Dom.DrawOutLine();
	Dom.LineTo(0,0,0,Dom.Always(10));
	Dom.LineTo(0,Dom.Always(10),Dom.Always(30),Dom.Always(10));
	Dom.LineTo(Dom.Always(30),Dom.Always(10),Dom.Always(40),0);

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.TextOut(Dom.Always(15), Dom.Always(3.5), "��Ʈ ����");

	*pDomP << Dom;
}

void CAPlateOutTendon::AddFrontAA(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	COptionStd	    *pOpt = pStd->GetDomOptionStd();
	COutMovePoint   *pMove= pStd->GetMovePoint(pStd->m_nCurPage);
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	

	double dFrontDetScale = pOpt->m_dTendon_ScaleSectionAA;
	double dCurPageScale  = GetScale();

	// Ⱦ�ܸ鵵
	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);

	Dom.SetScaleDim(dFrontDetScale);
	// ���� BX������ �ι�° ���κ� ��ġ --------------------------------	
	CPlateGirderApp *pGir = m_pBxStt->GetGirder();
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, m_pBxStt, m_pBxEnd);
	pBx = Finder.GetBxNext();
	//------------------------------------------------------------------

	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());	
	// �ܸ�׸�
	DrawDanmyun.DrawDanmyunEttOnBx(&Dom, pBx, FALSE, TRUE, FALSE);			
	DrawDanmyun.DrawTensionBar    (&Dom, pBx);	// ������ ������ �ܸ�	
	DrawDanmyun.DrawGuardWall     (&Dom, pBx);	
	
	// Ⱦ�ܸ鵵 ġ����
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);	

	DrawDanmyun.DimTensionBarOnLineInfo(&Dom, pBx);	//  ġ��
	DrawDanmyun.DimTensionBar(&Dom, pBx);
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);
	DrawDanmyun.DimTypicalSectionBottom(&Dom, pBx);	
	
	// �󼼵� �����Ϸ� ���� ũ�� ����
	Dom.RedrawByScale(dCurPageScale/dFrontDetScale);
/*
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();	
	double MoveX = pDomP->Always(200);
	double MoveY = pDomP->Always(200);

	Dom.Move(MoveX, MoveY);
	Dom.Move(pMove->GetPoint(MPE_SECTION_A));
	Dom.SetCalcExtRect();
	rect = Dom.GetExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom,rect,MPE_SECTION_A,"�ܸ� A-A");
	*pDomP << Dom;

	Dom.SetScaleDim(dCurPageScale);
	DrawStd.AddTitleOnDom(&Dom, dFrontDetScale, rect, "�ܸ� A-A", TRUE, TRUE, 0);
	
	Dom.Move(pMove->GetPoint(MPE_SECTION_A+1));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SECTION_A+1, "�ܸ� A-A ����");
*/
	*pDomP << Dom;
}

void CAPlateOutTendon::InsertDomyun(CDomyun *pDom, long nSubPage)
{
	CString csSection       = "Afx Directory";
	CString csWorkItem		= "Fixed Directory";
	CString szPath = AfxGetApp()->GetProfileString(csSection, csWorkItem);
	
	CString szName;	
	szName.Format("%s\\BlockDomyun\\FixingDetail.dxf",szPath, nSubPage);	

	CDomyun Dom(pDom);
	
	Dom.DxfIn(szName);
	Dom.Move(Dom.Always(450), Dom.Always(100));

	*pDom << Dom;
}

void CAPlateOutTendon::ReplaceData(CDomyun *pDom)
{
/*	//��Ÿ�󼼵�(1)
	double R1 = pGir->m_pSangse->m_Scallop_R1;
	double R2 = pGir->m_pSangse->m_Scallop_R2;	
	pDom->ReplaceString("R1",COMMA(R1));
	pDom->ReplaceString("R2",COMMA(R2));
*/
}

void CAPlateOutTendon::DrawSlabByPage(CDomyun *pDomP, long nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;
	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	CDomyun Dom(pDomP);

	long nG = 0;
	CPlateBasicIndex *pBxLeftStt  = GetBxSttPage(nSubPage,nG,FALSE);
	CPlateBasicIndex *pBxLeftEnd  = GetBxEndPage(nSubPage,nG,FALSE);
	
	nG = pDB->GetGirdersu()-1;
	CPlateBasicIndex *pBxRightStt = GetBxSttPage(nSubPage,nG,FALSE);
	CPlateBasicIndex *pBxRightEnd = GetBxEndPage(nSubPage,nG,FALSE);
	
	DrawPyung.DrawSlabByBx(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);

	// ����������
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_CENT);
	if(nSubPage==1)
		DrawPyung.DrawJointLine(&Dom,TRUE);
	if(nSubPage==GetPageSu())
		DrawPyung.DrawJointLine(&Dom,FALSE);

	*pDomP << Dom;
}

void CAPlateOutTendon::DimTextPyungMatchLine(CDomyun *pDom, CPlateBasicIndex *pBx, long nSubPage, long nDan)
{
 	CAPlateDrawStd	 *pStd    = (CAPlateDrawStd*)m_pStd;
	COptionStd	     *pOpt    = pStd->GetDomOptionStd();
	CPlateBridgeApp	 *pDB     = pStd->m_pDB;
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirSec = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBxSec  = pBx->GetBxMatchByCrossBeam(pGirSec);	// pBx�� ���κ� ��ġ�� ��츸 ���� �۵���.
	
	// pBx �� �������� ��ġ �� ���...
	if(pBx->IsState(BX_SPLICE))
	{
		long nSp = pBx->GetSplice()->GetSelfCount();
		pBxSec = pGirSec->GetBxBySpliceNo(nSp);
	}

	if(pBxSec==NULL) return;

	CSymbolDom SymDom(pDom, pStd->m_pOptStd);
	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_SYMB);

	CString str;
	double sta = pBx->GetStation();
	CDPoint XY(0,0);
	
	double Dis = pGir->GetMaxWidthFlange(TRUE);

	double Scale   = SymDom.GetScaleDim();
	double dExo	   = SymDom.GetDimExo();
	double vLen	   = SymDom.GetDimVLen();
	double dLevel  = SymDom.GetDimLevelDis();
	double distExo = Dis + (dExo+vLen+dLevel*(nDan-1))*Scale;

	XY = pGir->GetXyGirderDis(sta, -distExo, pBx->GetAngle());
	str.Format("%s%d","M.L", nSubPage);
	CDPoint rAng = pGir->GetLine()->GetAngleAzimuth(sta);
	SymDom.DrawSymbolHexagonStr(XY,rAng,str);

	distExo -= SymDom.Always(5);
	pStd->m_pOptStd->SetEnvType(&SymDom, HCAD_CUTL);
	XY = pGir->GetXyGirderDis(sta, -distExo, pBx->GetAngle());
	SymDom.MoveTo(XY.x, XY.y);
	Dis = pDB->GetWidthAct(sta);//pDB->GetSlabRight()->m_dGirderCenter + distExo - pGir->m_dGirderCenter;
	XY = pGir->GetXyGirderDis(sta, Dis, pBx->GetAngle());
	SymDom.LineTo(XY.x, XY.y);

	*pDom << SymDom;
}


// �Ŵ� ���Ϻ� �÷��� ������ �׸�.
	void CAPlateOutTendon::AddPlanGirderLine(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	BOOL bWebLine = TRUE;
	long nGsu = pDB->GetGirdersu();
	// nSide:-1 �߽ɰŴ����� �����Ŵ�
	// nSide:-1 �߽ɰŴ����� �����Ŵ�

	CDomyun Dom(pDom);

	for(long nG=0; nG<nGsu; nG++)
	{
		pStd->m_pOptStd->SetEnvLType(&Dom,LT_DOT_LINE);

		CPlateBxFinder	Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBxStt = GetBxSttPage(nSubPage, nG, FALSE);
		CPlateBasicIndex *pBxEnd = GetBxEndPage(nSubPage, nG, FALSE);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING | BX_UPPERFLANGE, pBxStt, pBxEnd);				
		if(pBx != pBxStt)
		{
			DrawPyung.DrawGirderWithenBx(&Dom, pBxStt, pBx, TRUE, 0, 0);	
			if(bWebLine)
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBxStt, pBx, TRUE, 0, 0);					
		}
		while(Finder.GetBxFixNext())
		{					
			DrawPyung.DrawGirderWithenBx   (&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0, 0);	
			if(bWebLine)
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, Finder.GetBxFixNext(), TRUE, 0, 0);	
			pBx = Finder.GetBxNext();
		}			
		if(pBx != pBxEnd)
		{
			DrawPyung.DrawGirderWithenBx(&Dom, pBx, pBxEnd, TRUE, 0, 0);	
			if(bWebLine)
				DrawPyung.DrawGirderWebWithenBx(&Dom, pBx, pBxEnd, TRUE, 0, 0);
		}
	}		
	*pDom << Dom;
	
}

void CAPlateOutTendon::DrawCenterLineAndMark(CDomyun *pDomP, long subPage)
{
	CAPlateDrawStd	*pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->m_pDB;

	CAPlateDrawPyung DrawPyung(pStd->GetDataManage());	

	CDomyun* pDom = new CDomyun(pDomP);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_CENT);
	
	CPlateBasicIndex *pBxStt = GetBxSttPage(subPage,-1);
	CPlateBasicIndex *pBxEnd = GetBxEndPage(subPage,-1);


	// �����߽ɼ� (MAIN ����)
	double  sttStation = pBxStt->GetStation();
	double  endStation = pBxEnd->GetStation();
	DrawPyung.DrawCenterLine(pDom,-1,sttStation,endStation);

	*pDomP << *pDom;
	delete pDom;
}



