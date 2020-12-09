  // APlateOutEtcDetail.cpp: implementation of the CAPlateOutEtcDetail class.
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

CAPlateOutEtcDetail::CAPlateOutEtcDetail()
{

}

CAPlateOutEtcDetail::~CAPlateOutEtcDetail()
{

}

void CAPlateOutEtcDetail::DrawOnePage()
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CDrawPageMng *pPageMng		= pStd->GetDrawPageMng();
	COptionStd	    *pDomOpt    = pStd->GetDomOptionStd();
	CGlobarOption	*pOpt		= pStd->GetDataManage()->GetGlobalOption();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();	
	CPlateGirderApp *pGir		= pBridge->GetGirder(-1);	
		
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
	pStd->GetDogak()->AddTitle(&Dom, pBridge->m_sNameBridge, GetTitle(), _T(""),FALSE);	
	CDRect dgkRect = Dom.GetExtRect(), DomBound;
	
	pPageMng->SetProgressData("��Ÿ�� �����...",40);
	InsertDomyun(&Dom, nSubPage);
	ReplaceData(&Dom, nSubPage);
	*pDom << Dom;

	if(nSubPage==2)
	{
		DrawDetailShearConnect(&Dom);
		TableSolePlateDetail(&Dom);
		DimTextNote(&Dom);
	}

	if(nSubPage==3)
	{
		DrawJackupDetail(&Dom);
		Dom.Move(Dom.Always(-140), 0);
		
		DrawJackupDanmyun(&Dom);
		Dom.Move(Dom.Always(260), Dom.Always(430));
		*pDom << Dom;

		CAPlateDrawDanmyun Draw(pStd->GetDataManage());
		Draw.DrawInputStagingRing(&Dom, pGir->m_pSangse->m_Dongbari_Type, FALSE);
		Dom.Move(Dom.Always(650), Dom.Always(430));
		
	}

	if(nSubPage==4)
	{		
		DimTextWeldNote1(&Dom);
		DimTextWeldNote2(&Dom);
		DimTextWeldThick(&Dom);
	}
	pPageMng->SetProgressData("��¿Ϸ�",100);
	*pDom << Dom;

	/*
	PageTable(&Dom);	
	AddScallopDetail(&Dom);
	AddVStiffDetail1(&Dom);
	AddTaperDetail(&Dom);
	AddHStiffSpDetail(&Dom);
	AddHStiffVDetail(&Dom);
	*/
}

long CAPlateOutEtcDetail::GetPageSu()
{
	m_nPage = 5;
	return m_nPage;
}

double CAPlateOutEtcDetail::GetScale()
{
	return 20;
}

CString CAPlateOutEtcDetail::GetTitle()
{
 	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nPageSu = m_nPage;
	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("�� Ÿ �� �� �� (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("�� Ÿ �� �� ��");

	return szTitle;
}

void CAPlateOutEtcDetail::InsertDomyun(CDomyun *pDom, long nSubPage)
{
	CString csSection       = "Afx Directory";
	CString csWorkItem		= "Fixed Directory";
	CString szPath = AfxGetApp()->GetProfileString(csSection, csWorkItem);
	
	CString szName;	
	szName.Format("%s\\BlockDomyun\\Etc_Detail(%d).dxf",szPath,nSubPage);

	CDomyun Dom(pDom);
	
	Dom.DxfIn(szName);
	Dom.Move(Dom.Always(0), Dom.Always(0));
	*pDom << Dom;
}
void CAPlateOutEtcDetail::ReplaceData(CDomyun *pDom, long nSubPage)
{
	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp		*pBridge	= pStd->GetDataManage()->GetBridge();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	CPlateBasicIndex	*pBx		= pGir->GetBxOnJijum(0);
	CSection			*pSec		= ((CPlateBasicIndex *)pBx)->GetSection();
	CVStiff				*pVStiff	= pSec->GetVStiff();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxHs = Finder.GetBxFirst(BX_PLATEHSTIFF,NULL,NULL);
	CWebHStiff  *pHs = pBxHs->GetWebHStiff();

	//��Ÿ�󼼵�(1)
	switch(nSubPage)
	{
	case 1:
		{
			pDom->ReplaceString("R1",atofRound(pBridge->GetGirder(-1)->m_pSangse->m_Scallop_R1,0));
			pDom->ReplaceString("R2",atofRound(pBridge->GetGirder(-1)->m_pSangse->m_Scallop_R2,0));

			pDom->ReplaceString("TH1",atofRound(pBridge->GetGirder(-1)->m_pSangse->m_Taper_H,0));
			pDom->ReplaceString("TW1",atofRound(pBridge->GetGirder(-1)->m_pSangse->m_Taper_W,1));
			pDom->ReplaceString("TH2",atofRound(pBridge->m_HTaper_H,0));
			pDom->ReplaceString("TW2",atofRound(pBridge->m_HTaper_W,1));

			pDom->ReplaceString("VW",atofRound(pVStiff->m_dWidth[0],0));//���������� �β�
			pDom->ReplaceString("VD",atofRound(pVStiff->m_dGap[0],0));//���������� ��
			pDom->ReplaceString("VA","45");//���������� �𼭸� ����

			double HA = 45; //���򺸰��� �𼭸� ����
			double HB1 = pBridge->GetGirder(-1)->m_pSangse->m_HStiff_VStiff_B;
			double HB2 = HB1 * 2 + pVStiff->m_dThick[0];
			double HD = pBridge->GetGirder(-1)->m_pSangse->m_HStiff_VStiff_D;
			double HW1 = max(pHs->m_dWidthHStiff[0],pHs->m_dWidthHStiff[1]); //���򺸰��� ��
			double HW2 = HW1 - HD;
			pDom->ReplaceString("HB1",atofRound(HB1,0));
			pDom->ReplaceString("HB2",atofRound(HB2,0));
			pDom->ReplaceString("HD",atofRound(HD,0));
			pDom->ReplaceString("HA",atofRound(HA,0));
			pDom->ReplaceString("HW1",atofRound(HW1,0));
			pDom->ReplaceString("HW2",atofRound(HW2,0));

			pDom->ReplaceString("HS",atofRound(pBridge->GetGirder(-1)->m_pSangse->m_HStiff_Splice_B,0));			
		}

		break;
	case 2:
		{
			double SD1 = pBridge->GetGirder(0)->m_pSangse->m_StudHori_D;
			double SD2 = SD1 + 10;
			double SH1 = pBridge->GetGirder(0)->m_pSangse->m_StudHori_E;
			double SH2 = pBridge->GetGirder(0)->m_pSangse->m_StudHori_H;
			double SH0 = SH1 + SH2;
			pDom->ReplaceString("SD1", COMMA(SD1));
			pDom->ReplaceString("SD2", COMMA(SD2));
			pDom->ReplaceString("SH1", COMMA(SH1));
			pDom->ReplaceString("SH2", COMMA(SH2));
			pDom->ReplaceString("SH0", COMMA(SH0));
		}
		break;
	case 3:
		{
			double AA0 = pBridge->GetGirder(-1)->m_pSangse->m_SlabAnchor_A;
			double AAn = 45; //����
			double AB0 = pBridge->GetGirder(-1)->m_pSangse->m_SlabAnchor_B;
			double AB1 = AA0/sqrt(2.0);
			double AC = pBridge->GetGirder(-1)->m_pSangse->m_SlabAnchor_C;
			double AD = pBridge->GetGirder(-1)->m_pSangse->m_SlabAnchor_T;
			pDom->ReplaceString("AA0",COMMA(AA0));
			pDom->ReplaceString("AAn",COMMA(AAn));
			pDom->ReplaceString("AB0",COMMA(AB0));
			pDom->ReplaceString("AB1",COMMA(AB1));
			pDom->ReplaceString("AC",COMMA(AC));
			pDom->ReplaceString("AD","D"+COMMA(AD));
		}
		break;
	case 4:
		break;
	case 5:
		break;
	}
	
	//pDom->ReplaceString("%%d","��");
	
/*
	
		//��Ÿ�󼼵�(2)
		double SD1 = pBridge->GetGirder(0)->m_pSangse->m_StudHori_D;
		double SD2 = SD1 + 10;
		double SH1 = pBridge->GetGirder(0)->m_pSangse->m_StudHori_E;
		double SH2 = pBridge->GetGirder(0)->m_pSangse->m_StudHori_H;
		double SH0 = SH1 + SH2;
		pDom->ReplaceString("SD1", COMMA(SD1));
		pDom->ReplaceString("SD2", COMMA(SD2));
		pDom->ReplaceString("SH1", COMMA(SH1));
		pDom->ReplaceString("SH2", COMMA(SH2));
		pDom->ReplaceString("SH0", COMMA(SH0));

		//��Ÿ�󼼵�(3)
		double DW = pGir->m_pSangse->m_Dongbari_W;
		double DC = pGir->m_pSangse->m_Dongbari_C;
		double DF = pGir->m_pSangse->m_Dongbari_F;
		double DT = pGir->m_pSangse->m_Dongbari_T; //���ٸ��� �β�
		double DA0 = pGir->m_pSangse->m_Dongbari_A;
		double DA1 = DW - DA0;
		double DD0 = pGir->m_pSangse->m_Dongbari_D;
		double DD1 = DA0 - DD0/2;
		double DD2 = DW - (DD0 + DD1);
		double DE0 = pGir->m_pSangse->m_Dongbari_E;
		double DE1 = DE0*2;
		double DB0 = pGir->m_pSangse->m_Dongbari_B + DD0;
		double DB1 = (DE1 - DB0)/2;
		pDom->ReplaceString("DW",COMMA(DW));
		pDom->ReplaceString("DC",COMMA(DC));
		pDom->ReplaceString("DF",COMMA(DF));
		pDom->ReplaceString("DT",COMMA(DT));
		pDom->ReplaceString("DA0",COMMA(DA0));
		pDom->ReplaceString("DA1",COMMA(DA1));
		pDom->ReplaceString("DD0",COMMA(DD0));
		pDom->ReplaceString("DD1",COMMA(DD1));
		pDom->ReplaceString("DD2",COMMA(DD2));
		pDom->ReplaceString("DE0",COMMA(DE0));
		pDom->ReplaceString("DE1",COMMA(DE1));
		pDom->ReplaceString("DB0",COMMA(DB0));
		pDom->ReplaceString("DB1",COMMA(DB1));

		
	*/
}

void CAPlateOutEtcDetail::DrawScallopDetail(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CPlateGirderApp* pGirCen = pStd->GetDataManage()->GetBridge()->GetGirder(-1);
	double R = pGirCen->m_pSangse->m_Scallop_R1;
	CAPlateDrawDanmyun Draw(pBridge->m_pDataManage);
	Draw.DrawScallopGen(&Dom, R);
	Draw.DimScallopGen(&Dom, R);
	*pDom << Dom;
}

void CAPlateOutEtcDetail::AddScallopDetail(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt    = pStd->m_pDomOptionStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp  *pGir = pStd->m_pDB->GetGirder(-1);
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);
 
	CDomyun Dom(pDom);
	double Scale = 15.0;  ////////////
	Dom.SetScaleDim(20);
	DrawScallopDetail(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(155),pDom->Always(395));
	*pDom << Dom;
	//��Ʈ
	Dom.SetScaleDim(20);
	NoteScallop(&Dom);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(138),pDom->Always(365));
	*pDom << Dom;
	//���̺�
	Dom.SetScaleDim(20);
	TableScallop(&Dom);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(160),pDom->Always(426));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::TableScallop(CDomyun *pDom)
{
 	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp    *pGir    = pStd->m_pDB->GetGirder(-1);

	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	long nRows = 2;
	long nCols = 3;
	double Height = Dom.Always(8);
	double Width = Dom.Always(20);
	CString str;
	Dom.SetRowCount(nRows);
	Dom.SetColumnCount(nCols);
	Dom.SetColumnWidthAll(Width);
	Dom.SetRowHeightAll(Height);
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetTextMatrix(0,0,"����");
	Dom.SetTextMatrix(0,1,"t<=16");
	Dom.SetTextMatrix(0,2,"t>16");
	Dom.SetTextMatrix(1,0,"R");
	str.Format("%.f",pGir->m_pSangse->m_Scallop_R1);
	Dom.SetTextMatrix(1,1,str);
	str.Format("%.f",pGir->m_pSangse->m_Scallop_R2);
	Dom.SetTextMatrix(1,2,str);
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();
	*pDom << Dom;
}

void CAPlateOutEtcDetail::NoteScallop(CDomyun *pDom)
{
 	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp    *pGir    = pStd->m_pDB->GetGirder(-1);

	double R1 = pGir->m_pSangse->m_Scallop_R1;
	double R2 = pGir->m_pSangse->m_Scallop_R2;
	CString str1, str2, str3, str4, str5;
	str1.Format("(1) ���忬�� �������� ���Ϻ� �÷����� WEB�����鿡�� ��Ķ���� ����Ͽ�");
	str2.Format("    ��ġ�Ͽ��� �Ѵ�.");
	str3.Format("(2) ��Ķ���� ũ��� WEB���� �β��� �������� ��ġ�Ѵ�.");
	str4.Format("(3) WEB�β��� 16mm������ ��쿡�� %.fmm, 16mm�� �ʰ��ϴ� ��쿡��", R1);
	str5.Format("    %.fmm�� �������� �Ѵ�.", R2);

	CGridDomyunEx Dom(pDom);	

	CDPoint Base(0,0);
	Dom.SetBasePoint(Base);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(5);
	Dom.SetColumnCount(2);
	Dom.SetRowHeightAll(Dom.Always(5));

	Dom.SetTextMatrix(0,1,str1,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(1,1,str2,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(2,1,str3,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(3,1,str4,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(4,1,str5,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.Draw();	

	*pDom << Dom;
}

void CAPlateOutEtcDetail::DrawTaperDetail(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	
	double Scale =  2;
	double t1	 = 12;
	double t2	 = 18;
	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CAPlateDrawDanmyun Draw(pBridge->m_pDataManage);
	Draw.DrawFactDetail_VType(&Dom,t1,t2,Scale,TRUE, 2, 2, 60, FALSE, TRUE, FALSE, FALSE);

//	Dom.DimMoveTo(
	*pDom << Dom;
}

void CAPlateOutEtcDetail::AddTaperDetail(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt    = pStd->m_pDomOptionStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp  *pGir = pStd->m_pDB->GetGirder(-1);
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);
 
	CDomyun Dom(pDom);
	double Scale = 1.0;  //////////////
	DrawTaperDetail(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(141),pDom->Always(137));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::DrawHStiffSpDetail(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1); 
	CPlateBxFinder FinderSp(pGir);
	CPlateBasicIndex *pBxSp = FinderSp.GetBxFirst(BX_SPLICE);
	CPlateBxFinder FinderSc(pGir);
	CPlateBasicIndex *pBxSc = FinderSc.GetBxFirst(BX_CROSSBEAM_VBRACING);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CAPlateDrawDanmyun Draw(pBridge->m_pDataManage);
	Draw.DrawHStiff_withSplice(&Dom, pBxSp, 20);
	*pDom << Dom;
}

void CAPlateOutEtcDetail::AddHStiffSpDetail(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt    = pStd->m_pDomOptionStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp  *pGir = pStd->m_pDB->GetGirder(-1);
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);

	CDomyun Dom(pDom);
	double Scale = 10;  //////////////
	Dom.SetScaleDim(20);
	DrawHStiffSpDetail(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(671),pDom->Always(177));
	*pDom << Dom;
	//��Ʈ
	Dom.SetScaleDim(20);
	NoteHStiffSp(&Dom);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(560),pDom->Always(107));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::DrawHStiffVDetail(CDomyun *pDom, CPlateBasicIndex *pBx)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();		
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1); 
	CPlateBxFinder FinderSp(pGir);
	CPlateBasicIndex *pBxSp = FinderSp.GetBxFirst(BX_SPLICE);
	CPlateBxFinder FinderSc(pGir);
	CPlateBasicIndex *pBxSc = FinderSc.GetBxFirst(BX_CROSSBEAM_VBRACING);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	CAPlateDrawDanmyun Draw(pBridge->m_pDataManage);
	Draw.DrawHStiff_withVStiff(&Dom, pBxSp, 20);
	*pDom << Dom;
}

void CAPlateOutEtcDetail::AddHStiffVDetail(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt    = pStd->m_pDomOptionStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp  *pGir = pStd->m_pDB->GetGirder(-1);
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);

	CDomyun Dom(pDom);
	double Scale = 10;  //////////////
	Dom.SetScaleDim(20);
	DrawHStiffVDetail(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(421),pDom->Always(177));
	*pDom << Dom;
	//��Ʈ
	Dom.SetScaleDim(20);
	NoteHStiffV(&Dom);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(310),pDom->Always(107));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::NoteHStiffV(CDomyun *pDom)
{
 	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp    *pGir    = pStd->m_pDB->GetGirder(-1);

	double VB = pGir->m_pSangse->m_HStiff_VStiff_B;
	CString strV1, strV2;
	strV1.Format("* ������������ �����ο� ��ġ�Ǵ� ���򺸰���� �ڵ������� ����Ͽ�");
	strV2.Format("  ����������� ���� �������� %.fmm �̰ݽ��� ��ġ�Ͽ��� �Ѵ�.", VB);

	CGridDomyunEx Dom(pDom);	

	CDPoint Base(0,0);
	Dom.SetBasePoint(Base);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(2);
	Dom.SetColumnCount(2);
	Dom.SetRowHeightAll(Dom.Always(5));

	Dom.SetTextMatrix(0,1,strV1,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(1,1,strV2,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.Draw();	

	*pDom << Dom;
}

void CAPlateOutEtcDetail::NoteHStiffSp(CDomyun *pDom)
{
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp    *pGir    = pStd->m_pDB->GetGirder(-1);

	double SB = pGir->m_pSangse->m_HStiff_Splice_B;
	CString strSp1, strSp2;
	strSp1.Format("* ���忬�� ������(WEB PLATE)�� ���򺸰������ �������� %.fmm", SB);
	strSp2.Format("  �̰ݽ��Ѽ�ġ�Ͽ��� �Ѵ�.");

	CGridDomyunEx Dom(pDom);	

	CDPoint Base(0,0);
	Dom.SetBasePoint(Base);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(2);
	Dom.SetColumnCount(2);
	Dom.SetRowHeightAll(Dom.Always(5));

	Dom.SetTextMatrix(0,1,strSp1,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(1,1,strSp2,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.Draw();	

	*pDom << Dom;
}

void CAPlateOutEtcDetail::PageTable(CDomyun *pDom)
{
 	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp    *pGir    = pStd->m_pDB->GetGirder(0);

	CGridDomyunEx Dom(pDom);
	long nRows = 4;
	long nCols = 3;
	double Height1 = Dom.Always(15);
	double Height2 = Dom.Always(230);
	double Width = Dom.Always(250);
	CString str;
	Dom.SetRowCount(nRows);
	Dom.SetColumnCount(nCols);
	Dom.SetColumnWidthAll(Width);
	Dom.SetRowHeight(0,Height1);
	Dom.SetRowHeight(1,Height2);
	Dom.SetRowHeight(2,Height1);
	Dom.SetRowHeight(3,Height2);
	Dom.SetBasePoint(CDPoint(Dom.Always(46), Dom.Always(532)));
	Dom.Draw(FALSE);
	Dom.DrawOutLine();
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(125), Dom.GetBasePoint().y - Dom.Always(10), "��Ķ�� ��");
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(375), Dom.GetBasePoint().y - Dom.Always(10), "���������� ������<1>");
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(625), Dom.GetBasePoint().y - Dom.Always(10), "���������� ������<2>");
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(125), Dom.GetBasePoint().y - Dom.Always(255), "�ܸ� ��ȭ�� TAPER ó�� ��");
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(375), Dom.GetBasePoint().y - Dom.Always(255), "���򺸰��� ������");
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(625), Dom.GetBasePoint().y - Dom.Always(255), "���򺸰���� ������ ���Ӻ� ��");
	*pDom << Dom;
}

void CAPlateOutEtcDetail::TableVStiff(CDomyun *pDom)
{
 	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateGirderApp    *pGir    = pStd->m_pDB->GetGirder(0);

	CGridDomyunEx Dom(pDom);
	long nRows1 = 3;
	long nCols1 = 4;
	long nRows2 = 2;
	long nCols2 = 6;
	double Height1 = Dom.Always(5);
	double Height2 = Dom.Always(10);
	double Height3 = Dom.Always(25);
	double Width = Dom.Always(20);
	CString str;
	Dom.SetRowCount(nRows1);
	Dom.SetColumnCount(nCols1);
	Dom.SetRowHeightAll(Height1);
	Dom.SetColumnWidth(0,Width);
	Dom.SetColumnWidth(1,Width);
	Dom.SetColumnWidth(2,Width*2);
	Dom.SetColumnWidth(3,Width*2);
	Dom.SetBasePoint(CDPoint(0,0));

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.TextOut(Dom.Always(60), Dom.Always(2), "������������ �������");

	Dom.SetGridLines(GVL_VERT);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	Dom.SetTextMatrix(1,0,"��    ġ");
	Dom.SetTextMatrix(1,1,"�� �� ��");
	Dom.SetTextMatrix(0,2,"SWAY BRACING ");
	Dom.SetTextMatrix(1,2,"�� CROSS BEAM");
	Dom.SetTextMatrix(2,2,"�� ��ġ�Ǵ� �κ�");
	Dom.SetTextMatrix(1,3,"��    Ÿ");
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();
	Dom.Draw(FALSE);

	Dom.SetRowCount(nRows2);
	Dom.SetColumnCount(nCols2);
	Dom.SetColumnWidthAll(Width);
	Dom.SetRowHeight(0,Height2);
	Dom.SetRowHeight(1,Height3);
	Dom.SetBasePoint(CDPoint(Dom.Always(0),Dom.Always(-15)));
	Dom.SetGridLines(GVL_BOTH);

	for(int i=0; i<nRows2; i++)
		for(int j=0; j<nCols2; j++)
			Dom.SetTextMatrix(i, j, "");

	Dom.SetTextMatrix(0,0,"�ڸ��Ʈ��ȣ");
	Dom.SetTextMatrix(0,1,"��");
	Dom.SetTextMatrix(0,2,"��");
	Dom.SetTextMatrix(0,3,"��, ������");
	Dom.SetTextMatrix(0,4,"��");
	Dom.SetTextMatrix(0,5,"��, ������");
	Dom.SetTextMatrix(1,0,"���䵵");
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();
	*pDom << Dom;
}

void CAPlateOutEtcDetail::NoteVStiff(CDomyun *pDom)
{
 	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;

	CString str1, str2, str3, str4, str5;
	str1.Format("(1) ���������� �޴� �κ��� ����������� ��, �� �÷��� ���պδ� ����");
	str2.Format("(2) ����������� �����÷����� ����");
	str3.Format("(3) ����� ����������� ���� �� ���� �÷����� ����");
	str4.Format("(4) �����ο� ���� ���������� �޴� ��ġ�� ��ġ�Ǵ� ����������� ��������");
	str5.Format("    Ȩ�������� ���� �÷����� ���ս�Ų��.");

	CGridDomyunEx Dom(pDom);	

	CDPoint Base(0,0);
	Dom.SetBasePoint(Base);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(5);
	Dom.SetColumnCount(2);
	Dom.SetRowHeightAll(Dom.Always(5));

	Dom.SetTextMatrix(0,1,str1,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(1,1,str2,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(2,1,str3,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(3,1,str4,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(4,1,str5,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_DIML);
	Dom.Draw();	

	*pDom << Dom;
}

void CAPlateOutEtcDetail::AddVStiffDetail1(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt    = pStd->m_pDomOptionStd;
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->m_nCurPage);
	CPlateGirderApp  *pGir = pStd->m_pDB->GetGirder(-1);
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);
 
	CDomyun Dom(pDom);
	double Scale = 10.0;  ////////////
	Dom.SetScaleDim(20);
	DrawVStiffDetail1(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(351),pDom->Always(435));
	*pDom << Dom;

	DrawVStiffDetail1(&Dom, pBx, TRUE, TRUE);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(387),pDom->Always(435));
	*pDom << Dom;

	DrawVStiffDetail1(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(431),pDom->Always(435));
	*pDom << Dom;

	DrawVStiffDetail1(&Dom, pBx, FALSE);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(471),pDom->Always(435));
	*pDom << Dom;

	DrawVStiffDetail1(&Dom, pBx);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(511),pDom->Always(435));
	*pDom << Dom;
	//��Ʈ
	Dom.SetScaleDim(20);
	NoteVStiff(&Dom);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(290),pDom->Always(365));
	*pDom << Dom;
	//���̺�
	Dom.SetScaleDim(20);
	TableVStiff(&Dom);
	Dom.RedrawByScale(GetScale()/Scale);
	Dom.Move(pDom->Always(301),pDom->Always(494));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::DrawVStiffDetail1(CDomyun *pDom, CPlateBasicIndex *pBx, BOOL bVStiff, BOOL bWeld)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();

	CDomyun Dom(pDom);
	double flangeH = Dom.Always(1);
	double flangeW = Dom.Always(10);
	double webH = Dom.Always(15);
	double webW = Dom.Always(1.5);
	Dom.SetScaleDim(15);
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	Dom.Rectangle(0, 0, flangeW, flangeH);
	Dom.Rectangle((flangeW - webW)/2, 0, (flangeW + webW)/2, -webH);
	Dom.Rectangle(0, -webH, flangeW, -(webH + flangeH));
	Dom.MoveTo(-Dom.Always(3), Dom.Always(2));
	Dom.LineTo(-Dom.Always(1), 0);
	Dom.LineTo(flangeW + Dom.Always(1), 0);
	Dom.LineTo(flangeW + Dom.Always(3), Dom.Always(2));

	CDPoint VStiff1((flangeW + webW)/2, 0);
	CDPoint VStiff2((flangeW + webW)/2 + flangeW/4, -webH);
	CDPoint weld1 = VStiff1;
	CDPoint weld2(VStiff2.x, VStiff1.y);
	CDPoint weld3 = VStiff2;
	CDPoint weld4(VStiff1.x, VStiff2.y);

	if(bVStiff)
	{
		Dom.Rectangle(VStiff1, VStiff2);
		pOpt->SetEnvType(&Dom, HCAD_HATH);
		Dom.Solid(weld1.x, weld1.y, weld1.x + Dom.Always(0.75), weld1.y, weld1.x, 
			-Dom.Always(0.75), weld1.x, weld1.y);
		Dom.Solid(weld2.x, weld2.y, weld2.x - Dom.Always(0.75), weld2.y, weld2.x, 
			-Dom.Always(0.75), weld2.x, weld2.y);
		Dom.Solid(weld3.x, weld3.y, weld3.x - Dom.Always(0.75), weld3.y, weld3.x, 
			weld3.y + Dom.Always(0.75), weld3.x, weld3.y);
		Dom.Solid(weld4.x, weld4.y, weld4.x + Dom.Always(0.75), weld4.y, weld4.x, 
			weld4.y + Dom.Always(0.75), weld4.x, weld4.y);
		*pDom << Dom;
		pOpt->SetEnvType(&Dom, HCAD_DIML);
		Dom.GiSiArrowExt(weld1.x + Dom.Always(0.5), weld1.y - Dom.Always(0.5), Dom.Always(8), 0, 0, "");
		Dom.Rotate(weld1+CDPoint(Dom.Always(0.5), -Dom.Always(0.5)), CDPoint(1, -1.7));
		*pDom << Dom;
		Dom.GiSiArrowExt(weld4.x + Dom.Always(0.5), weld4.y + Dom.Always(0.5), Dom.Always(8), 0, 0, "");
		Dom.Rotate(weld4+CDPoint(Dom.Always(0.5), Dom.Always(0.5)), CDPoint(1, 1.7));
		Dom.TextOut(flangeW + Dom.Always(2), -(webH/2 + Dom.Always(1)), "����");
		if(bWeld)
		{
			Dom.TextOut(flangeW + Dom.Always(4), -(webH/2 + Dom.Always(5)), "MATAL");
			Dom.TextOut(flangeW + Dom.Always(4), -(webH/2 + Dom.Always(9)), "TOUCH");
		}
	}
	else
	{
		Dom.Rectangle(VStiff1.x, VStiff1.y, VStiff2.x, VStiff2.y + Dom.Always(2));
		pOpt->SetEnvType(&Dom, HCAD_HATH);
		Dom.Solid(weld1.x, weld1.y, weld1.x + Dom.Always(0.75), weld1.y, weld1.x, 
			-Dom.Always(0.75), weld1.x, weld1.y);
		Dom.Solid(weld2.x, weld2.y, weld2.x - Dom.Always(0.75), weld2.y, weld2.x, 
			-Dom.Always(0.75), weld2.x, weld2.y);
		*pDom << Dom;
		pOpt->SetEnvType(&Dom, HCAD_DIML);
		Dom.GiSiArrowExt(weld1.x + Dom.Always(0.5), weld1.y - Dom.Always(0.5), Dom.Always(8), 0, 0, "");
		Dom.Rotate(weld1+CDPoint(Dom.Always(0.5), -Dom.Always(0.5)), CDPoint(1, -1.7));
		Dom.TextOut(flangeW + Dom.Always(2), -(webH/2 + Dom.Always(1)), "����");
	}
	*pDom << Dom;
}


void CAPlateOutEtcDetail::DrawDetailShearConnect(CDomyun *pDom)
{
 	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(0);
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(0);

	CDomyun Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_STLC);
	CAPlateDrawDanmyun DrawDanmyun(pStd->GetDataManage());
	DrawDanmyun.DrawGirderOnBx(&Dom, pBx);
	DrawDanmyun.DrawFrontUpperStudOrAnchorBx(&Dom, pBx, pBx->GetGirder()->m_pSangse->m_StudHori_Type, FALSE);

	double dScale = 20;
	Dom.SetScaleDim(dScale);

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DrawDanmyun.DrawDimFrontUpperStudOrAnchorBx(&Dom, pBx, pBx->GetGirder()->m_pSangse->m_StudHori_Type, FALSE);

	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();

	Dom.Move(-rect.left - rect.Width()/2, -rect.top - rect.Height()/2);
	Dom.Move(pDom->Always(650),pDom->Always(400));
	
	*pDom << Dom;
}


void CAPlateOutEtcDetail::TableSolePlateDetail(CDomyun *pDom)
{
	CAPlateDrawStd *pStd = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB = pStd->GetDataManage()->GetBridge();

	// Data
	CGridDomyunEx Dom(pDom);
	CStringArray strArr[9];
	CString Str = _T("");
	long i=0;
	
	for(long nPier=0; nPier< pDB->m_nQtyJigan+1; nPier++)
	{
		for(long nS=0; nS<pDB->GetQtyShoeOnJijum(nPier); nS++)
		{
			long nGir = pDB->GetNumberGirderByShoe(nS, nPier);
			CShoe *pShoe = pDB->GetShoeByRowCol(nS, nPier);
			CPlateGirderApp  *pGir = pDB->GetGirder(nGir);
			CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nPier);
			CPlateGirderApp  *pGir2 = pDB->GetGirder(-1);

			double slope = pGir->GetGiulJongDanShoe(pBx);			

			Str = pDB->m_strJijumName[nPier];			
			strArr[0].Add(Str);

			Str.Format("%d", nS+1);
			strArr[1].Add(Str);
			switch(pShoe->m_cDirectionShoe)
			{
			case 0:
			case 4:
				Str = "������";
				break;
			case 1:
				Str = "�����";
				break;
			case 2:
			case 3:
				Str = "�Ϲ���";
				break;
			}
			strArr[2].Add(Str);
			double L = pShoe->m_dW_SolePlate + 2*pShoe->m_dW2_SolePlate;
			double H = pShoe->m_dH_SolePlate + 2*pShoe->m_dH2_SolePlate;
			double T1 = pShoe->m_dThick_SolePlate - L/2*slope;
			double T2 = pShoe->m_dThick_SolePlate + L/2*slope;
			Str.Format("%.3f",slope*100);
			strArr[3].Add(Str);
			strArr[4].Add(COMMA(L));
			strArr[5].Add(COMMA(H));
			strArr[6].Add(COMMA(pShoe->m_dThick_SolePlate));
			strArr[7].Add(COMMA(RoundUp(T1,0)));
			strArr[8].Add(COMMA(RoundUp(T2,0)));
		}
	}
	
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);
	CDPoint Base(0,0);
	long nColsu = 9;
	long nRowsu = strArr[0].GetSize()+1;

	Dom.SetColumnCount(nColsu);
	Dom.SetRowCount(nRowsu);
	double Height = Dom.Always(8);
	double Width[] = { Dom.Always(15), Dom.Always(20), Dom.Always(20), Dom.Always(30), Dom.Always(15), 
					Dom.Always(15), Dom.Always(15), Dom.Always(15), Dom.Always(15) };
	Dom.SetRowHeightAll(Height);
	for(i=0; i<nColsu; i++)
		Dom.SetColumnWidth(i, Width[i]);
	Dom.SetBasePoint(Base);
	
	// Title
	Dom.SetTextMatrix(0,0,"����");
	Dom.SetTextMatrix(0,1,"����ȣ");
	Dom.SetTextMatrix(0,2,"������");
	Dom.SetTextMatrix(0,3,"S(%)");
	Dom.SetTextMatrix(0,4,"W");
	Dom.SetTextMatrix(0,5,"H");
	Dom.SetTextMatrix(0,6,"T");
	Dom.SetTextMatrix(0,7,"T1");
	Dom.SetTextMatrix(0,8,"T2");
	
	// 
	long PierMergeRow = 1;
	for(i=0; i<nRowsu-1; i++)
	{
		Dom.SetTextMatrix(i+1, 0, strArr[0].GetAt(i));
		Dom.SetTextMatrixCircle(i+1, 1, strArr[1].GetAt(i));
		Dom.SetTextMatrix(i+1, 2, strArr[2].GetAt(i));
		Dom.SetTextMatrix(i+1, 3, strArr[3].GetAt(i));
		Dom.SetTextMatrix(i+1, 4, strArr[4].GetAt(i), GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 5, strArr[5].GetAt(i), GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 6, strArr[6].GetAt(i), GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 7, strArr[7].GetAt(i), GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i+1, 8, strArr[8].GetAt(i), GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER);

		if(i==nRowsu-2 || strArr[0].GetAt(i)!=strArr[0].GetAt(i+1))
		{
			Dom.SetMergeRow(PierMergeRow, i+1, 0);
			PierMergeRow = i+2;
		}
	}

	Dom.Draw(FALSE);
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();
/*
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	Base = rect.TopLeft() + CDPoint(0,-1)*Dom.Always(20);
//
	char *szNoteArr[] = {
		"SOLE PLATE ���۽� ������ħ �԰��� ����Ͽ� ����ġ���� Ȯ���� �� �����Ͽ��� �Ѵ�.",
		"SOLE PLATE ���۽� ���ܰ�� ������ ����Ͽ� SOLE PLATE�� �����Ͽ��� �Ѵ�.",
		"SOLE PLATE �԰��� ������ħ ��翡 ���� ������(������)�� ������ ���� �� ������ �� �ִ�.",
		"SOLE PLATE  �� �β��� ���ܰ�簡 1%  �̸��϶����� �β���ȭ�� ���� �ʰ� 1% �̻��϶�����",
		"�Ʒ����� ���¸鿡 ������ �ǰ� ��ġ�Ͽ� �� �β��� 22mm �̻����� �Ѵ�.",
		"SOLE PLATE �� ������ FLANGE���ܿ��� �ּ� 15mm �̻� �̰��Ѵ�." };

	long nSize = 6;//sizeof(szNoteArr) / sizeof(szNoteArr[0]);
	
//	CGridDomyunEx Dom(pDom);	

	//CDPoint Base(0,0);
	Dom.SetBasePoint(Base);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(nSize);
	Dom.SetColumnCount(2);

	Dom.SetRowHeightAll(Dom.Always(7));
	Dom.SetColumnWidth(0,Dom.Always(10));
	Dom.SetColumnWidth(1,Dom.Always(30));

	Dom.SetTextMatrixCircle(0,0,"1");
	Dom.SetTextMatrixCircle(1,0,"2");
	Dom.SetTextMatrixCircle(2,0,"3");
	Dom.SetTextMatrixCircle(3,0,"4");
	Dom.SetTextMatrixCircle(5,0,"5");

	for(i=0;i<nSize;i++)
		Dom.SetTextMatrix(i,1,szNoteArr[i],GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(2), Dom.GetBasePoint().y + Dom.Always(6), "NOTE");

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TXTG);
	Dom.Draw(FALSE);
	*/
	Dom.Move(Dom.Always(570),Dom.Always(270));
	*pDom << Dom;
	
}

void CAPlateOutEtcDetail::DimTextNote(CDomyun *pDom)
{
 	CAPlateDrawStd		*pStd		= (CAPlateDrawStd*)m_pStd;

	char *szNoteArr[] = {
		"SOLE PLATE ���۽� ������ħ �԰��� ����Ͽ� ����ġ���� Ȯ���� �� �����Ͽ��� �Ѵ�.",
		"SOLE PLATE ���۽� ���ܰ�� ������ ����Ͽ� SOLE PLATE�� �����Ͽ��� �Ѵ�.",
		"SOLE PLATE �԰��� ������ħ ��翡 ���� ������(������)�� ������ ���� �� ������ �� �ִ�.",
		"SOLE PLATE  �� �β��� ���ܰ�簡 1%  �̸��϶����� �β���ȭ�� ���� �ʰ� 1% �̻��϶�����",
		"�Ʒ����� ���¸鿡 ������ �ǰ� ��ġ�Ͽ� �� �β��� 22mm �̻����� �Ѵ�.",
		"SOLE PLATE �� ������ FLANGE���ܿ��� �ּ� 15mm �̻� �̰��Ѵ�." };

	long nSize = sizeof(szNoteArr) / sizeof(szNoteArr[0]);
	
	CGridDomyunEx Dom(pDom);	

	CDPoint Base(0,0);
	Dom.SetBasePoint(Base);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(nSize);
	Dom.SetColumnCount(2);

	Dom.SetRowHeightAll(Dom.Always(7));
	Dom.SetColumnWidth(0,Dom.Always(10));
	Dom.SetColumnWidth(1,Dom.Always(30));

	Dom.SetTextMatrixCircle(0,0,"1");
	Dom.SetTextMatrixCircle(1,0,"2");
	Dom.SetTextMatrixCircle(2,0,"3");
	Dom.SetTextMatrixCircle(3,0,"4");
	Dom.SetTextMatrixCircle(5,0,"5");

	for(long i=0;i<nSize;i++)
		Dom.SetTextMatrix(i,1,szNoteArr[i],GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(2), Dom.GetBasePoint().y + Dom.Always(6), "NOTE");

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TXTG);
	Dom.Draw();	

	Dom.Move(Dom.Always(570),Dom.Always(150));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::DimTextWeldNote1(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CGlobarOption	*pOpt	= pStd->GetDataManage()->GetGlobalOption();

	CString szText = _T("");
	CStringArray szNoteArr;
	szNoteArr.Add("FLANGE �� WEB�� ����δ� �ݵ�� ���ܸ� ����Ȩ�������� V�� �Ǵ� X�� ������ ���. (�´����� : BUTT WELDING)");
	szNoteArr.Add("�ܸ��� ���� �ٸ� �ֿ������ �´�� ������ �־�� �β� �� ���� ������ ��ȭ���� ���̹����� ��簡 1:5���ϰ� �ǵ��� �Ѵ�.");
	szNoteArr.Add("�´�� �������� �β��� ������ 3mm�̸��� ��쿡�� ������ taper�� ��ġ���� �ʰ� ������Ʈ�� ó���ϸ�,");
	szText.Format("   �������� ������ �β��� %.0fmm �ʰ��Ҷ��� X�� ������ �ϰ�, %.0fmm ������ ���� V�� ������ �Ѵ�."
		, pOpt->GetDesignThickXWeldApply(), pOpt->GetDesignThickXWeldApply());
	szNoteArr.Add(szText);
	szNoteArr.Add("   ��, T-2-t1=3mm �̻��� ��� 1:5 taper�� ��ġ�Ѵ�.");
	szNoteArr.Add("   T-2-t1=3mm �̸��� ��쿡�� taper�� ������Ʈ�θ� ��ġ�Ѵ�.");
	szNoteArr.Add("�ϸ� �÷����� WEB�÷����� BUTT�����δ� �ݵ�� FACING �� �׶��ε� ó���Ұ�.");
	
	long nSize = szNoteArr.GetSize();
	CGridDomyunEx Dom(pDom);	

	CDPoint xyBase(0,0);
	Dom.SetBasePoint(xyBase);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(nSize);
	Dom.SetColumnCount(2);

	Dom.SetRowHeightAll(Dom.Always(7));
	Dom.SetColumnWidth(0,Dom.Always(5));
	Dom.SetColumnWidth(1,Dom.Always(30));

	Dom.SetTextMatrix(0, 0, "1.",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(1, 0, "2.",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(2, 0, "3.",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(6, 0, "4.",GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);

	for(long i=0;i<nSize;i++)
		Dom.SetTextMatrix(i,1,szNoteArr.GetAt(i),GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(2), Dom.GetBasePoint().y + Dom.Always(6), "NOTE");

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TXTG);
	Dom.SetTextColor(4);
	Dom.Draw();	

	Dom.Move(Dom.Always(100),Dom.Always(128));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::DimTextWeldNote2(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CGlobarOption	*pOpt	= pStd->GetDataManage()->GetGlobalOption();

	CString szText = _T("");
	CStringArray szNoteArr;
	szNoteArr.Add("FLANGE �� WEB�� ��������Ȩ���� (X��, V������)");
	szNoteArr.Add("CROSS BEAM BUESSET PLATE(I��)�� ��������Ȩ���� (V�� ����)");
	szNoteArr.Add("DIAPHRAGM�� ���������� �� JACK UP ������� ��������Ȩ���� (K�� ����)");
	szNoteArr.Add("������ DIAPHRAGM�� �ϸ� �÷����� ��������Ȩ���� (K�� ����)");
		
	long nSize = szNoteArr.GetSize();
	CGridDomyunEx Dom(pDom);	

	CDPoint xyBase(0,0);
	Dom.SetBasePoint(xyBase);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(nSize);
	Dom.SetColumnCount(2);

	Dom.SetRowHeightAll(Dom.Always(7));
	Dom.SetColumnWidth(0,Dom.Always(5));
	Dom.SetColumnWidth(1,Dom.Always(30));

	for(long i=0;i<nSize;i++)
	{
		szText.Format("%d.", i+1);
		Dom.SetTextMatrix(i,0,szText,GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
		Dom.SetTextMatrix(i,1,szNoteArr.GetAt(i),GRID_ALIGN_LEFT | GRID_ALIGN_VCENTER);
	}
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(Dom.GetBasePoint().x + Dom.Always(2), Dom.GetBasePoint().y + Dom.Always(6), "NOTE");

	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TXTG);
	Dom.SetTextColor(4);
	Dom.Draw();	

	Dom.Move(Dom.Always(380),Dom.Always(107));
	*pDom << Dom;
}

void CAPlateOutEtcDetail::DimTextWeldThick(CDomyun *pDom)
{
	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	CGlobarOption	*pOpt	= pStd->GetDataManage()->GetGlobalOption();

	CString szText1 = _T(""), szText2 = _T("");
	szText1.Format("T��%.0fmm", pOpt->GetDesignThickXWeldApply());
	szText2.Format("T>%.0fmm", pOpt->GetDesignThickXWeldApply());

	CGridDomyunEx Dom(pDom);	

	CDPoint xyBase(0,0);
	Dom.SetBasePoint(xyBase);
	Dom.SetGridLines(GVL_NONE);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(2);

	Dom.SetRowHeightAll(Dom.Always(7));
	Dom.SetColumnWidthAll(Dom.Always(47));
	Dom.SetTextMatrix(0, 0, szText1, GRID_ALIGN_LEFT|GRID_ALIGN_VCENTER);
	Dom.SetTextMatrix(0, 1, szText2, GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER);
	
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TXTG);
	Dom.SetTextColor(4);
	Dom.Draw();	

	Dom.Rotate(xyBase, CDPoint(0,1));
	Dom.Move(Dom.Always(113),Dom.Always(378.5));
	*pDom << Dom;
}


void CAPlateOutEtcDetail::DrawJackupDetail(CDomyun *pDom)
{
	CAPlateDrawStd  *pStd		= (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pBridge	= pStd->GetDataManage()->GetBridge();
	CARoadOptionStd *pOpt		= pStd->GetDataManage()->GetOptionStd();
	COptionStd		*pDomOpt	= pStd->m_pDomOptionStd;	
	COutMovePoint	*pMove		= pStd->GetMovePoint(pStd->m_nCurPage);		
	CPlateGirderApp *pGir		= pBridge->GetGirder(-1);
	CJackupStiff	*pJack		= pGir->GetJackupByJijum(0, 0);
	if(pJack == NULL)
		return;

	CDomyun	Dom(pDom);
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	CPlateBxFinder Finder(pGir);
	Finder.GetBxFirst(BX_STT_GIRDER | BX_SPLICE | BX_END_GIRDER);
	CPlateBasicIndex *pBxStt = Finder.GetBxNext(0);
	CPlateBasicIndex *pBxEnd = Finder.GetBxNext(1);

	pGir->SetEleLevel(TRUE);

	CAPlateDrawStandard DrawStd(pStd->GetDataManage());	
	CAPlateDrawJong DrawJong(pStd->GetDataManage());	
	CAPlateDrawDetailDim  DetailDim(pStd,&Dom,pBxStt, pBxEnd);

	long nLine  = 2;
	CPlateBasicIndex *pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_VSTIFF | BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	DrawJong.DrawGirderLineBx(&Dom, pBxStt, pBxEnd, nLine);
	DrawJong.DrawJackUpBx(&Dom,pBxStt,pBxEnd,nLine, 4);
	DrawJong.DrawJiJumVStiffBx(&Dom,pBxStt,pBxEnd,nLine, 4);
	DrawJong.DrawInOutGateBx(&Dom, pBxStt, pBxEnd, TRUE);

	double GTU	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	double GTL	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double GH = pGir->GetHeightGirderByStaAng(pGir->GetBxOnJijum(0)->GetStation(), 0, CDPoint(0,1), FALSE);
	CShoe *pShoe = pBridge->GetGirder(0)->GetShoeByJijumNo(0);
	double dShoeW = pShoe->m_dW_SolePlate;
	double dShoeT = pShoe->m_dThick_SolePlate;
	double JPW = pBridge->m_structJackupJewon[0][1].m_dW;
	double JPT = pBridge->m_structJackupJewon[0][1].m_dT;
	
	CDPoint sp = CDPoint(0,0);
	double sttSta = pBxStt->GetStation();
	double CurSta = pBx->GetStation();
	CDPoint vAngle = pBx->GetAngle();
	sp.x = sttSta + pGir->GetLengthDimType(sttSta,CurSta,0);
	sp.y = pGir->GetElevationFlangeCamber(CurSta, 0, vAngle, FALSE) - GTL;
	Dom.Rectangle(sp.x-dShoeW/2,sp.y, sp.x+dShoeW/2,sp.y-dShoeT);
	double L = 150;
	double H = 120;
	Dom.Solid(sp.x ,sp.y-dShoeT, sp.x+L/2, sp.y-H, sp.x-L/2,sp.y-H, sp.x, sp.y-dShoeT);
	Dom.TextOut(sp.x, sp.y-H*2, "Shoe");

	pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_JACKUP, pBxStt, pBxEnd);
	if(pBx)
	{
		sp.x = pBx->GetStation();
		Dom.Rectangle(sp.x-JPW/2,sp.y, sp.x+JPW/2,sp.y-JPT);
		Dom.Solid(sp.x ,sp.y-JPT, sp.x+L/2, sp.y-H, sp.x-L/2,sp.y-H, sp.x, sp.y-JPT);
		Dom.TextOut(sp.x, sp.y-H*2, "Jack-Up");
	}

	//���� �ڸ���
	pOpt->SetEnvType(&Dom, HCAD_CUTL);
	CObLayer Lay;
	CVector Stt = CVector(sp.x+400, sp.y+GTU+GTL+GH+300);
	CVector End = CVector(sp.x+400, sp.y-300);
	CObLine Line(Stt, End, &Lay);
	Dom.TrimByLine(&Line, Stt-CVector(1,0)*10000);
	Dom.CutLightning(End.x, End.y, Stt.x, Stt.y, TRUE, 1);
	CDRect rectErase(Stt.x, Stt.y, End.x+20000, End.y);
	Dom.EraseRect(rectErase);

	// ���� �̵�
	CDPoint MoveXY;
	MoveXY.x = -pBxStt->GetStation();
	MoveXY.y = -pGir->GetElevationFlangeCamber(pBxStt->GetStation());
	Dom.Move(MoveXY);

	*pDom << Dom;

	//ġ����
	pOpt->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("TOP");
	Dom.SetDimExo(3);
	Dom.DimMoveTo(0,0,0);
	pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_JIJUMSTIFF, pBxStt, pBxEnd);
	double dPJijumSta = pBxStt->GetStation();
	double dNJijumSta;
	double dNJackSta;
	while(pBx)
	{
		dNJijumSta = pBx->GetStation();
		Dom.DimLineTo(dNJijumSta-dPJijumSta, 0, COMMA(dNJijumSta-dPJijumSta));
		dPJijumSta = dNJijumSta;
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
	pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_JACKUP, pBxStt, pBxEnd);
	while(pBx)
	{
		dNJackSta = pBx->GetStation();
		Dom.DimLineTo(dNJackSta-dPJijumSta, 0, COMMA(dNJackSta-dPJijumSta));
		dPJijumSta = dNJackSta;
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
	
	Dom.SetDirection("BOTTOM");
	if(pBridge->m_dWidthInOutStt > 0)
	{
		Dom.DimMoveTo(0,-pGir->GetHeightInOutUpper(TRUE)-400,0);
		Dom.DimLineTo(pBridge->m_dWidthInOutStt,0,COMMA(pBridge->m_dWidthInOutStt));
		Dom.GiSiArrowExt(pBridge->m_dWidthInOutStt-pBridge->m_dRadiusInOutStt*.5,
			-(pGir->GetHeightInOutUpper(TRUE)+pBridge->m_dRadiusInOutStt*.5),
			pBridge->m_dRadiusInOutStt,-150,5,COMMA(pBridge->m_dRadiusInOutStt));
	}
	
	*pDom << Dom;
	Dom.SetDirection("LEFT");
	sp.x = 0;//sttSta + pGir->GetLengthDimType(sttSta,CurSta,0);
	sp.y = pGir->GetElevationFlangeCamber(CurSta, 0, vAngle, FALSE);

	Dom.DimMoveTo(sp.x, sp.y+GH, 0);
	Dom.DimLineTo(-GH,0,COMMA(GH));

	MoveXY.x = 0;//-pBxStt->GetStation();
	MoveXY.y = -pGir->GetElevationFlangeCamber(pBxStt->GetStation());
	Dom.Move(MoveXY);

	pGir->SetEleLevel(FALSE);
	*pDom << Dom;
}

void CAPlateOutEtcDetail::DrawJackupDanmyun(CDomyun *pDom)
{
//	/*
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CARoadOptionStd *pOpt	= pStd->GetDataManage()->GetOptionStd();
	CPlateBridgeApp *pBridge = pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(-1);
	CJackupStiff *pJack = pGir->GetJackupByJijum(0, 0);
	CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(0);
	
	if(pJack == NULL)
		return;
// 	CPlateBxFinder Finder(pGir);
// 	Finder.GetBxFirst(BX_STT_GIRDER | BX_SPLICE | BX_END_GIRDER);
// 	CPlateBasicIndex *pBxStt = Finder.GetBxNext(0);
// 	CPlateBasicIndex *pBxEnd = Finder.GetBxNext(1);
	
	double GH = pGir->GetHeightGirderByStaAng(pBx->GetStation());//, 0, CDPoint(0,1), FALSE);	
	double GTW = pBx->GetFactChain(G_W)->m_dFactChainThick;
	double GTU	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
	double GTL	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
	double dGirUWidth = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dGirLWidth = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
	double JWL = pJack->m_vJewon[0].x;
	double JWR = pJack->m_vJewon[1].x;
	double JPW = pBridge->m_structJackupJewon[0][1].m_dW;
	double JPT = pBridge->m_structJackupJewon[0][1].m_dT;
	double JPH = pBridge->m_structJackupJewon[0][1].m_dH;
	
	double dVG	 = pJack->m_dVG;
	double dVL   = pJack->m_dVL;	

	CDomyun Dom(pDom);

	pOpt->SetEnvType(&Dom, HCAD_STLC);
	Dom.Rectangle(-dGirUWidth/2,GTU, dGirUWidth/2,0);		//����÷���
	Dom.Rectangle(-GTW/2,0, GTW/2,-GH);						//����
	Dom.Rectangle(-dGirLWidth/2,-GH, dGirLWidth/2,-GH-GTL);	//�Ϻ��÷���
	

	if (dVL <= 0) 
	{
		dVL = GH;
		Dom.LineTo(-GTW/2-JWL,0-GH, -GTW/2-JWL,0-GH+dVL);
		Dom.LineTo(GTW/2+JWR,0-GH, GTW/2+JWR,0-GH+dVL);
	}
	else
	{
		//�������������
		Dom.LineTo(-GTW/2-JWL,0-GH, -GTW/2-JWL,0-GH+dVL-(JWL-dVG));
		Dom.LineTo(-GTW/2-dVG,0-GH+dVL,-GTW/2,0-GH+dVL);
		Dom.LineTo(-GTW/2-JWL,0-GH+dVL-(JWL-dVG),-GTW/2-dVG,0-GH+dVL);
		
		//�������������
		Dom.LineTo(GTW/2+JWR,0-GH, GTW/2+JWR,0-GH+dVL-(JWR-dVG));
		Dom.LineTo(GTW/2+dVG,0-GH+dVL,GTW/2,0-GH+dVL);
		Dom.LineTo(GTW/2+JWR,0-GH+dVL-(JWR-dVG),GTW/2+dVG,0-GH+dVL);
	}
	
	//Dom.LineTo(-GTW/2-JWL,0, -GTW/2-JWL,-GH);					//�������������
	//Dom.LineTo(GTW/2+JW,0, GTW/2+JW,-GH);					//�������������
	Dom.Rectangle(-JPH/2,-GH-GTL, JPH/2,-GH-GTL-JPT);		//������÷���Ʈ
	double L = 150;
	double H = 120;
	Dom.Solid(0 ,-GH-GTL-JPT, L/2, -GH-GTL-H, -L/2,-GH-GTL-H, 0, -GH-GTL-JPT);
	Dom.TextOut(0, -GH-GTL-H*2, "Jack-Up");

// 		// ���� �̵�
// 	CDPoint MoveXY;
// 	MoveXY.x = 0;//-pBxStt->GetStation();
// 	MoveXY.y = -pGir->GetElevationFlangeCamber(pBxStt->GetStation());
// 	Dom.Move(MoveXY);
	
	*pDom << Dom;

	//ġ����
	pOpt->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDimExo(3);
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(-GTW/2-JWL,0, 0);
	Dom.DimLineTo(JWL, 0, COMMA(JWL));
	Dom.DimLineToExtend(GTW, 0, "", COMMA(GTW));
	Dom.DimLineTo(JWR, 0, COMMA(JWR));
	
	if (dVG > 0 && dVL > 0)
	{
		Dom.DimMoveTo(-GTW/2-dVG,0-GH+dVL, 0);
		Dom.DimLineTo(dVG, 0, COMMA(dVG));
	}

	Dom.GiSiArrowExtend(JWL/3*2, -GH-GTL-JPT, Dom.Always(8), Dom.Always(24), 7, "��� �÷���Ʈ",
		COMMA(JPH)+"X"+COMMA(JPT)+"X"+COMMA(JPW));

	Dom.SetDirection("LEFT");
	
	if (dVL > 0)
	{
		Dom.DimMoveTo(-dGirUWidth/2, -GH, 0);
		Dom.DimLineTo(dVL, 0, COMMA(dVL));
		Dom.DimLineTo(GH-dVL, 0, COMMA(GH-dVL));
	}	

	//Dom.DimMoveTo(-dGirUWidth, -GH, 0);
	//Dom.DimLineTo(GH, 0, COMMA(GH));

// 	���� �̵�	
// 		MoveXY.x = 0;//-pBxStt->GetStation();
// 		MoveXY.y = -pGir->GetElevationFlangeCamber(pBxStt->GetStation());
// 		Dom.Move(MoveXY);
		
	*pDom << Dom;
//	*/
}