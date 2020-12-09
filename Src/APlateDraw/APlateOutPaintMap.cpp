// APlateOutPaintMap.cpp: implementation of the CAPlateOutPaintMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DanmyunGen		10
#define Danmyun			20
#define Splice			30
#define CrossBeam		40

#define TABLE_GEN		1
#define TABLE_CROSS		2
#define TABLE_SEC		3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutPaintMap::CAPlateOutPaintMap()
{

}

CAPlateOutPaintMap::~CAPlateOutPaintMap()
{

}

void CAPlateOutPaintMap::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CDrawPageMng	*pPageMng	= pStd->GetDrawPageMng();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
		
	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;		// �Ϲݵ������� �����ȣ
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();

	CDomyun *pDom = m_pStd->GetDomyun(nPage);
	pDom->SetScaleDim(Scale);	
	CDomyun Dom(pDom);	
	// ����
	pPageMng->SetProgressData("���� �����...",0);
	pStd->GetDogak()->Add(&Dom,nPage,m_Scale,pDomOpt->m_szDGKName,pDomOpt->m_sHead_DomyunNumber,pDomOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pDomOpt->m_szXRefPath : "");
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);
	CDRect dgkRect = Dom.GetExtRect(), DomBound;

	switch(nSubPage)
	{
		case 1:
			if(pDomOpt->m_bPaint_GirGen)
				AddGeneral(&Dom);
			if(pDomOpt->m_bPaint_Section)
				AddDanmyun(&Dom);
			break;
		case 2:
			if(pDomOpt->m_bPaint_Splice)
				AddSplice(&Dom);
			if(pDomOpt->m_bPaint_Cross)
				AddCrossBeam(&Dom);
			break;
	}

	*pDom << Dom;

	pPageMng->SetProgressData("��¿Ϸ�",100);
}

long CAPlateOutPaintMap::GetPageSu()
{
	m_nPage = 2;
	return m_nPage;
}

double CAPlateOutPaintMap::GetScale()
{
	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;
	
	m_Scale = pOpt->m_dPaint_Scale_GirGen;
	return m_Scale;
	//return 25;
}

CString CAPlateOutPaintMap::GetTitle()
{
 	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nPageSu = m_nPage;
	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("�� �� �� �� �� (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("�� �� �� �� ��");
//	csTitle.Format("�� �� �� �� �� (%d)",nSubPage);

	return szTitle;
}

//����� �Ϲݵ�...
void CAPlateOutPaintMap::AddGeneral(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);	

	double dyPos = 450;
	double dxPos = 100;
	double Scale = pOpt->m_dPaint_Scale_GirGen;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);
	// �Ϲ�
	if(pOpt->m_bPaint_GirGen)
	{
		pPageMng->SetProgressData("���鵵(�Ϲ�) �����...",30);
		DrawGeneral(&Dom,pBx);
		Dom.SetCalcExtRect();
		CDPoint Base(0,0);
		CDRect rect = Dom.GetExtRect();
		Base.x = rect.left;
		Base.y = rect.top-pDom->Always(5);
		DimTextNoteGen(&Dom,Base, TABLE_GEN);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+140),pDom->Always(dyPos));
		rect = Dom.GetExtRect();
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen,"���鵵(�Ϲ�)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "�� �� ��",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen+1,"���鵵(�Ϲ�) ����");
		*pDom << Dom;
	}
	// ǥ
	if(pOpt->m_bPaint_TableBox)
	{
		pPageMng->SetProgressData("�������ٰ�(�Ϲ�) �����...",50);
		Dom.SetScaleDim(GetScale());
		DrawTableGen(&Dom);
		Dom.Move(pDom->Always(dxPos+400),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen+2,"�������ٰ�(�Ϲ�)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		Dom.SetTextAlignHorz(TA_LEFT);
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "���� ���� �ٰ� : �Ϲ�",TRUE,FALSE,0);
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(DanmyunGen+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),DanmyunGen+3,"�������ٰ�(�Ϲ�) ����");
		*pDom << Dom;
	}	
}

//����� �ܸ鵵...
void CAPlateOutPaintMap::AddDanmyun(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);	

	double dyPos = 200;
	double dxPos = 150;
	double Scale = pOpt->m_dPaint_Scale_Section;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);
	// �Ϲ�
	if(pOpt->m_bPaint_Section)
	{
		pPageMng->SetProgressData("���鵵(�ܸ�) �����...",70);
		DrawSection(&Dom,pBx, 1);		
		Dom.SetCalcExtRect();
		CDPoint Base(0,0);
		CDRect rect = Dom.GetExtRect();
		Base.x = rect.left;
		Base.y = rect.top-pDom->Always(5);
		DimTextNoteGen(&Dom,Base, TABLE_SEC);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos),pDom->Always(dyPos));
		rect = Dom.GetExtRect();
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun,"���鵵(�ܸ�)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "�� �� ��",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+1,"���鵵(�ܸ�) ����");
		*pDom << Dom;

		Dom.SetScaleDim(Scale);
		DrawSection(&Dom,pBx, 2);		
		Dom.SetCalcExtRect();	
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+200),pDom->Always(dyPos));
		rect = Dom.GetExtRect();
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+2,"���鵵(�ܸ�)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "�� �� ��",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+3,"���鵵(�ܸ�) ����");
		*pDom << Dom;
	}
	// ǥ
	if(pOpt->m_bPaint_TableSection)
	{
		pPageMng->SetProgressData("�������ٰ�(�ܸ�) �����...",90);
		Dom.SetScaleDim(GetScale());
		DrawTableSec(&Dom);
		Dom.Move(pDom->Always(dxPos+350),pDom->Always(dyPos+5));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+4,"�������ٰ�(�ܸ�)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		Dom.SetTextAlignHorz(TA_LEFT);
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "���� ���� �ٰ� : �ܸ�",TRUE,FALSE,0);
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Danmyun+5));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Danmyun+5,"�������ٰ�(�ܸ�) ����");
		*pDom << Dom;
	}

}

//����� ��������...
void CAPlateOutPaintMap::AddSplice(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());

	CPlateBasicIndex	*pBx = pGir->GetBxBySpliceNoSPType(0);

	//��� ���� ������ ���....070320...KB...
	if (pGir->GetQtySplice(CPlateGirder::SPLICE) < 1) return;

	double dyPos = 480;
	double dxPos = 100;

	double Scale = pOpt->m_dPaint_Scale_Splice;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);
	if(pOpt->m_bPaint_SpliceUp)
	{
		pPageMng->SetProgressData("���������� �����...",60);
		DrawFlangeSplice(&Dom,pBx,TRUE);		
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos-20),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice,"����������");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "��  ��",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+1,"���������� ����");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_SpliceWeb)
	{
		pPageMng->SetProgressData("���������� �����...",70);
		DrawWebSplice(&Dom,pBx);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+100),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+2,"����������");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "��  ��",TRUE,TRUE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+3,"���������� ����");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_SpliceDown)
	{
		pPageMng->SetProgressData("���������� �����...",80);
		DrawFlangeSplice(&Dom,pBx,FALSE);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos+220),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice+4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+4,"����������");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "��  ��",TRUE,TRUE,0);
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+5));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+5,"���������� ����");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_TableSplice)
	{
		pPageMng->SetProgressData("�������ٰ�(��������) �����...",90);
		Dom.SetScaleDim(GetScale());
		DrawTableSplice(&Dom);
		Dom.Move(pDom->Always(500),pDom->Always(dyPos+25));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(Splice+6));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+6,"�������ٰ�(��������)");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "���� ���� �ٰ� : ��������",TRUE,FALSE,0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(Splice+7));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),Splice+7,"�������ٰ�(��������) ����");
		*pDom << Dom;
	}

}

//����� ���κ�..
void CAPlateOutPaintMap::AddCrossBeam(CDomyun *pDom)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd			*pOpt		= pStd->m_pDomOptionStd;
	CPlateBridgeApp		*pDB		= pStd->m_pDB;
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	COutMovePoint		*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);	
	CDrawPageMng		*pPageMng	= pStd->GetDrawPageMng();
	CAPlateDrawStandard DrawStd(pStd->GetDataManage());
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	while(pBx)
	{
		CPlateCrossBeam	*pC	= pBx->GetCrossBeam();
		if(pC)
		{
			break;
		}
		pBx = Finder.GetBxNext();
	}

	if(pBx==NULL)
		return;

	double dyPos = 260;
	double dxPos = 60;

	double Scale = pOpt->m_dPaint_Scale_Cross;
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	if(pOpt->m_bPaint_CrossPlan)
	{
		pPageMng->SetProgressData("���κ� ��鵵 �����...",70);
		DrawCrossBeamPlan(&Dom,pBx);		
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam,"���κ� ��鵵");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "��鵵", TRUE, TRUE, 0);		
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+1));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+1,"���κ� ��鵵 ����");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_CrossFront)
	{
		pPageMng->SetProgressData("���κ� ���鵵 �����...",80);
		DrawCrossBeamFront(&Dom,pBx);		
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(dxPos),pDom->Always(dyPos-100));
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+2));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+2,"���κ� ���鵵");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, Scale, rect, "���鵵", TRUE, TRUE, 0);				
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+3));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+3,"���κ� ���鵵 ����");
		*pDom << Dom;
	}

	if(pOpt->m_bPaint_TableCross)
	{
		pPageMng->SetProgressData("���κ� �������ٰ� �����...",90);
		Dom.SetScaleDim(GetScale());
		DrawTableCROSSBEAM(&Dom);
		Dom.SetCalcExtRect();
		CDPoint Base(0,0);
		CDRect rect = Dom.GetExtRect();
		Base.x = rect.left;
		Base.y = rect.top-pDom->Always(5);
		DimTextNoteGen(&Dom,Base, TABLE_CROSS);
		Dom.RedrawByScale(GetScale()/Scale);
		Dom.Move(pDom->Always(500),pDom->Always(dyPos));
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+4));
		pStd->ChangeSeperateLayer(&Dom); // HCAD ���� ���� �۾�
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+4,"���κ� �������ٰ�");
		*pDom << Dom;

		Dom.SetScaleDim(GetScale());
		DrawStd.AddTitleOnDom(&Dom, GetScale(), rect, "���� ���� �ٰ� : ���κ�", TRUE, FALSE, 0);			
		Dom.SetCalcExtRect();
		Dom.Move(pMove->GetPoint(CrossBeam+5));
		pMove->AddMPE(&Dom,Dom.GetExtRect(),CrossBeam+5,"���κ� �������ٰ� ����");
		*pDom << Dom;
	}
}

void CAPlateOutPaintMap::DrawCrossBeamPlan(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;	
	CAPlateDrawCross DrawCross(pStd->GetDataManage());
	
	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);	
	DrawCross.DrawInputCRBeamPlan(&Dom, pBx, TRUE);	

	*pDom << Dom;	
}

void CAPlateOutPaintMap::DrawCrossBeamFront(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawCross DrawCross(pStd->GetDataManage());		

	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
		
	DrawCross.DrawInputCRBeamFront(&Dom, pBx, TRUE);

	*pDom << Dom;	
}

void CAPlateOutPaintMap::InsertDomyun(CDomyun *pDom, long nSubPage)
{
	CString csSection       = "Afx Directory";
	CString csWorkItem		= "Fixed Directory";
	CString szPath = AfxGetApp()->GetProfileString(csSection, csWorkItem);
	
	CString szName;	
	szName.Format("%s\\BlockDomyun\\Painter_Detail(%d).dxf",szPath,nSubPage);	

	CDomyun Dom(pDom);
	
	Dom.DxfIn(szName);
	Dom.Move(Dom.Always(80), Dom.Always(80));
	*pDom << Dom;
}

void CAPlateOutPaintMap::DrawFlangeSplice(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bUpper)
{
	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;		
	CAPlateDrawPyung	DrawPyung(pStd->GetDataManage());

	CDomyun Dom(pDom);
	if(bUpper)
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
		DrawPyung.DrawUpperFlangeSplice(&Dom, pBx, 0, 0, FALSE, FALSE, TRUE);
	}
	else
	{
		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLH);
		DrawPyung.DrawLowerFlangeSplice(&Dom, pBx, 0, 0, FALSE, FALSE, TRUE);
	}
		
	*pDom << Dom;
}

void CAPlateOutPaintMap::DrawWebSplice(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CAPlateDrawJong		DrawJong(pStd->GetDataManage());
	
	CDomyun Dom(pDom);
	DrawJong.DrawInputDetSpliceWeb(&Dom, 0, 0, FALSE, TRUE);
	
	*pDom << Dom;
}

void CAPlateOutPaintMap::DrawGeneral(CDomyun *pDom, CPlateBasicIndex *pBx)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp		*pDB	= pStd->m_pDB;
	CPlateGirderApp		*pGir	= pBx->GetGirder();	

	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());
	DrawDanmyun.DrawGirderOnBx(&Dom, pBx, TRUE);
	DrawDanmyun.DrawFrontUpperStudOrAnchorBx(&Dom, pBx, pBx->GetGirder()->m_pSangse->m_StudHori_Type);
	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	Dom.Move(-A[2].x, -A[2].y);
	*pDom << Dom;	
}
void CAPlateOutPaintMap::DrawSection(CDomyun *pDom, CPlateBasicIndex *pBx, long nMode)
{
 	CAPlateDrawStd		*pStd	= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp		*pDB	= pStd->m_pDB;
	CPlateGirderApp		*pGir	= pBx->GetGirder();	

	CDomyun Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	
	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());
	DrawDanmyun.DrawGirderOnBx(&Dom, pBx, TRUE, nMode);//���������� �׸���...	
	DrawDanmyun.DrawFrontUpperStudOrAnchorBx(&Dom, pBx, pBx->GetGirder()->m_pSangse->m_StudHori_Type);
	
	CDPoint A[10];
	pGir->GetHunchXyDom(pBx,A);
	Dom.Move(-A[2].x, -A[2].y);
	*pDom << Dom;	
}

void CAPlateOutPaintMap::DimTextNoteGen(CDomyun *pDom, CDPoint Base, long nMode)
{
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TXTG);
	Dom.SetBasePoint(Base);
	long nRow = 1;
	long nCol = 3;
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(5));
	Dom.SetColumnWidthAll(Dom.Always(14));
	Dom.SetColumnWidth(1,Dom.Always(5));
	Dom.SetTextMatrix(0,0,"NOTE)");
	long nAlign = GRID_ALIGN_LEFT|GRID_ALIGN_VCENTER;
	Dom.SetTextMatrix(0,1,"1.",nAlign);	
	switch(nMode)
	{
		case TABLE_GEN:
			Dom.SetTextMatrix(0,2,"���ܿ����� : HB=STUD�� ����, D=STUD�� ����",nAlign);	
			break;
		case TABLE_CROSS:
			Dom.SetTextMatrix(0,2,"D : ANCHOR BAR�� ����, HB : ANCHOR BAR�� ����",nAlign);	
			break;
		case TABLE_SEC:
			Dom.SetTextMatrix(0,2,"TV : ���������� �β�",nAlign);	
			break;
	}
	
	Dom.SetGridLines(GVL_NONE);

	Dom.Draw();
	*pDom << Dom;	
}

void CAPlateOutPaintMap::DrawTableSec(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 4;
	long nCol = 6;	
	CString sData[] = {
			"����������",  "WV*HV*2+HV*TV",       "WV*HV*2+HV*TV",       "", "", "",
			"����������*", "WV-HV-WV*H2/2+HV*TV", "WV-HV-WV*H2/2+HV*TV", "", "", "",
			"���ٸ����","L*W*2+(L+W*2)*T",     "L*W*2+(L+W*2)*T",     "", "", ""			
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"����",
							"1) �ܺ� ����\n  (�� ��)",
							"2) �ܺ� ����\n  (�� ��)",
							"3) ������ ����\n (�� ��)",
							"4) �ܺκ�Ʈ��\n  ���ᵵ��(����)",
							"5) �ܺ������\n  ����(����)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;

}
void CAPlateOutPaintMap::DrawTableSplice(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 10;
	long nCol = 6;	
	CString sData[] = {
			"��������� (����)",	"",                "",					"W*L*2+(W+L)*T*2", "",				"",
			"��������� (����)",	"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"�ϸ� ������",			"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"���Ʈ ������",		"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"���� ������",			"",                "-W*L",				"W*L*2+(W+L)*T*2", "W*L+(W+L)*2*T",	"",
			"���� ������ (������)",	"",                "-W1*L1*2-W2*L2",	"W*L-(W1-W2)*L2\n+(W1*2-W2+L)*2*T", "W*L+(W+L)*2*T",	"",
			"�극�̽� ������",		"W*L*2+(W+L)*T*2", "W*L*2+(W+L)*T*2", "", "", "",
			"ä����",				"",                "",                  "",                "(W/2+L)*2*T",     "",
			"ä���� (������)",		"",                "",                  "",                "(W+L)*2*T\n+(W1-W2)*T",     ""
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"����",
							"1) �ܺ� ����\n  (�� ��)",
							"2) �ܺ� ����\n  (�� ��)",
							"3) ������ ����\n (�� ��)",
							"4) �ܺκ�Ʈ��\n  ���ᵵ��(����)",
							"5) �ܺ������\n  ����(����)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;

}
void CAPlateOutPaintMap::DrawTableCROSSBEAM(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 17;
	long nCol = 6;	
	CString sData[] = {
			"���÷���Ʈ",       "GH1*L*2",                 "GH1*L*2",                 "",                    "",                "",
			"���÷���Ʈ",       "GB1*L",                   "GB1*L",                   "",                    "",                "GB1*L+L*T*2",
			"���÷���Ʈ",       "GB1*L*2+L*T*2",           "GB1*L*2+L*T*2",           "",                    "",                "",
			"����������",       "GB1*GH1*2+GH1*T",         "GB1*GH1*2+GH1*T",         "",                    "",                "",
			"���򺸰���",       "GB1*L*2+L*T",             "GB1*L*2+L*T",             "",                    "",                "",
			"���������(���)", "",                        "",                        "GB1*L*2+(GB1+L)*T*2", "",                "",
			"�Ϻ�������(�ϸ�)", "",                        "-GB1*L",                  "GB1*L*2+(GB1+L)*T*2", "GB1*L+(GB1+L)*T*2", "",
			"�Ϻ�������",       "",                        "-GB1*L",                  "GB1*L*2+(GB1+L)*T*2", "GB1*L+(GB1+L)*T*2", "",
			"����������",       "",                        "-GB1*L",                  "GB1*L*2+(GB1+L)*T*2", "GB1*L+(GB1+L)*T*2", "",
			"ANCHOR BAR",       "",                        "",                        "",                    "",                "PI*D*HB+PI*(D/2)^2*2",
			"�ż�Ʈ �÷���Ʈ",  "GH1*GW2*2",               "GH1*GW2*2",               "",                    "",                "",
			"�ż�Ʈ ����",      "(GB1+GB2)/2*GW1",         "(GB1+GB2)/2*GW1",         "",                    "",                "(GB1*GB2)/2*GW1+GL1*T*2",
			"�ż�Ʈ ����",      "(GB1+GB3)/2*GW2+GL2*T*2", "(GB1+GB3)/2*GW2+GL2*T*2", "",                    "",                "",
			"�ż�Ʈ ������",    "GH2*GW2*2",               "GH2*GW2*2",               "",                    "",                "",
			"�ż�Ʈ ������",    "W*L*2+(W+L)*T*2",         "GB1*L*2+(W+L)*T*2",       "",                    "",                "",
			"�ż�Ʈ ����",      "GH2*GW3/2*2",             "GH2*GW3/2*2",             "",                    "",                ""			
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"����",
							"1) �ܺ� ����\n  (�� ��)",
							"2) �ܺ� ����\n  (�� ��)",
							"3) ������ ����\n (�� ��)",
							"4) �ܺκ�Ʈ��\n  ���ᵵ��(����)",
							"5) �ܺ������\n  ����(����)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;	
}
void CAPlateOutPaintMap::DrawTableGen(CDomyun *pDom)
{
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetBasePoint(CDPoint(0,0));
	
	long nRow = 8;
	long nCol = 6;	
	CString sData[] = {
			"����÷���",  "(WU-T)*LA",        "(WU-T)*LA",        "", "", "(WU+TU*2)*LA",
			"�Ϻ��÷���",  "(WL*2-t+TL*2)*LA", "(WL*2-t+TL*2)*LA", "", "", "",
			"�����÷���",  "LA*H*2",           "LA*H*2",           "", "", "",
			"���򺸰���",  "WH*LA*2+LA*TH",    "",                 "", "", "",
			"SOLE PLATE",  "WP*LP+(WP+LP)*T*2","WP*LP+(WP+LP)*T*2","", "", "",
			"���� ������", "",                 "",                 "", "", "PI*D*HB+PI*(D/2)^2",
			"FACE PLATE",  "W*L*2+(W+L)*T*2",  "W*L*2+(W+L)*T*2",  "", "", ""
			};
	
	Dom.SetRowCount(nRow);
	Dom.SetColumnCount(nCol);
	Dom.SetRowHeightAll(Dom.Always(10));
	Dom.SetRowHeight(0, Dom.Always(15));
	Dom.SetColumnWidthAll(Dom.Always(40));
	
	/// Title		
	CString sTopTitle[] = {	"����",
							"1) �ܺ� ����\n  (�� ��)",
							"2) �ܺ� ����\n  (�� ��)",
							"3) ������ ����\n (�� ��)",
							"4) �ܺκ�Ʈ��\n  ���ᵵ��(����)",
							"5) �ܺ������\n  ����(����)"};

	long i = 0;
	for(i=0; i<nCol; i++)
		Dom.SetTextMatrix(0,i,sTopTitle[i]);	

	long nIdx = 1;
	long nSize = sizeof(sData)/ sizeof(sData[0]);
	
	for(i=0;i<nSize; i++)
	{
		for(long j=0; j<nCol; j++)
		{
			Dom.SetTextMatrix(nIdx,j,sData[i+j]);
		}
		nIdx++;
		i += nCol-1;
	}		

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	*pDom << Dom;
}
void CAPlateOutPaintMap::ReplaceData(CDomyun *pDom)
{

}
