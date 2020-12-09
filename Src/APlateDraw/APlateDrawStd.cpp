// APlateDrawStd.cpp: implementation of the CAPlateDrawStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
// #include "APlateOutGeneral.h"
// #include "APlateOutDetail.h"
// #include "APlateOutHBracing.h"
// #include "APlateOutSplice.h"
// #include "APlateOutCrossBeam.h"
// #include "APlateOutVBracing.h"
// #include "APlateOutEtcDetail.h"
// #include "APlateOutBM.h"
// #include "APlateOutShoeCoord.h"
// #include "APlateOutCamber.h"
// #include "APlateOutWeldMap.h"
// #include "APlateOutPlanGen.h"
// #include "APlateOutLineInfo.h"
// #include "APlateOutPaintMap.h"
// #include "APlateOutSlabGen.h"
// #include "APlateOutTendon.h"
// #include "APlateDrawDeckRebar.h"
// #include "APlateOutSummary.h"
// #include "APlateOutTUGirder.h"
// #include "APlateDrawStd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateDrawStd::CAPlateDrawStd(CDataManage *pAData)
{
	CDimDomyunEx::SetCutLineOn(TRUE);

	m_pDataManage = pAData;
	m_pOptStd = pAData->GetOptionStd();
	m_pDrawPageMng->AddOnePage(new CAPlateOutGeneral, "�Ŵ� �Ϲݵ�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutDetail, "�Ŵ� �󼼵�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutHBracing, "����극�̽� �󼼵�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutSplice, "�������� �󼼵�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutCrossBeam, "���κ� �󼼵�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutVBracing, "���� �극�̽� �󼼵�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutEtcDetail, "��Ÿ �󼼵�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutBM, "���� ���ǥ", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutSummary, "��൵", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutShoeCoord, "������ħ ��ġ��", this);	
	m_pDrawPageMng->AddOnePage(new CAPlateOutCamber, "������", this);	 
	m_pDrawPageMng->AddOnePage(new CAPlateOutPaintMap, "���� �󼼵�", this);	 
	m_pDrawPageMng->AddOnePage(new CAPlateOutPlanGen, "�Ϲ� ��鵵", this);	 
	m_pDrawPageMng->AddOnePage(new CAPlateOutLineInfo, "���� ������", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutSlabGen, "������ �Ϲݵ�", this);
	m_pDrawPageMng->AddOnePage(new CAPlateOutTendon, "Ⱦ���� TENDON ��ġ��", this);
	m_pDrawPageMng->AddOnePage(new CAPlateDrawDeckRebar, "������ ��ٵ�", this);
// 	APLATE-631 ���� Ʈ�� �޴� ��� ������� �־� �ϴ� ���⸸ ����� �Ѵ�..
//	m_pDrawPageMng->AddOnePage(new CAPlateOutTUGirder, "Turnover �Ŵ�", this);
}
 
CAPlateDrawStd::~CAPlateDrawStd()
{
	m_pDrawPageMng->ResetAll();
}


void CAPlateDrawStd::SetDomyunSize(long nSize)
{
	m_nDomyunSize = nSize;
}

long CAPlateDrawStd::GetDomyunSize()
{
	return m_nDomyunSize;
}

void CAPlateDrawStd::SetDomyunTitleArray(CStringArray &strArrTitle)
{
	m_strArrDomyunTitle.RemoveAll();

	for(long nIdx = 0; nIdx < strArrTitle.GetSize(); nIdx++)
		m_strArrDomyunTitle.Add(strArrTitle.GetAt(nIdx));
}

void CAPlateDrawStd::SetAtDomyunTitle(long nIdx, CString &strTitle)
{
	if(nIdx < 0 || nIdx > m_strArrDomyunTitle.GetSize() - 1)
		return;
	m_strArrDomyunTitle.SetAt(nIdx, strTitle);
}

CString CAPlateDrawStd::GetAtDomyunTitle(long nIdx)
{
	if(nIdx < 0 || nIdx > m_strArrDomyunTitle.GetSize() - 1)
		return _T("");

	return m_strArrDomyunTitle.GetAt(nIdx);
}
	
void CAPlateDrawStd::AddMaterial(CDomyun * pAdd, double Scale)
{	
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pDrawPageMng->m_pDrawStd;
	CARoadOptionStd *pOpt		= GetDataManage()->GetOptionStd();	
	CSteelSection	*pSteelSec  = GetDataManage()->GetSteelSection();
	CSteelGrade		*pSteelGrade= GetDataManage()->GetSteelGrade();	
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->GetCurPage());

	CGridDomyunEx Dom(pAdd);
	pOpt->SetEnvType(&Dom,HCAD_TBL1);
		
	CDRect gr = GetDogak()->GetRectDraw(Scale);
	double x  = gr.right - Dom.Always(207);
	double y  = gr.bottom;
	
	CString csFmt, cs;

	Dom.SetBasePoint(CDPoint(x,y));
	Dom.SetColumnCount(2);	
	Dom.SetRowCount(6);
	Dom.SetRowHeightAll(Dom.GetTextHeight()*1.5);
	Dom.SetColumnWidth(0,Dom.Always(27));	
	Dom.SetColumnWidth(1,Dom.Always(38));	

	Dom.SetTextMatrix(0, 0, "�� �� �� :", GRID_ALIGN_RIGHT);
	Dom.SetTextMatrix(0, 1, "��� ���� �����", GRID_ALIGN_LEFT);
	Dom.SetTextMatrix(2, 0, "��   �� :", GRID_ALIGN_RIGHT);
	long TotalBujemarkSu = TOTAL_PIECE_VALUE;
	long nBujeMark = 0;
	CString	strBuje = "�ֺ���";
	long	n		= 0;
	for (n=0; n < TotalBujemarkSu; n++)
	{
		if (strBuje == pSteelSec->GetClass(n))
		{
			nBujeMark = n;
			break;
		}
	}
	Dom.SetTextMatrix(2, 1, "�ֺ���-"+pSteelSec->GetMaterialByMarkBuje(nBujeMark), GRID_ALIGN_LEFT);
	strBuje = "�κ���";
	for (n=0; n < TotalBujemarkSu; n++)
	{
		if (strBuje == pSteelSec->GetClass(n))
		{
			nBujeMark = n;
			break;
		}
	}
	Dom.SetTextMatrix(3, 1, "�κ���-"+pSteelSec->GetMaterialByMarkBuje(nBujeMark), GRID_ALIGN_LEFT);
	Dom.SetTextMatrix(5, 0, "���� ���� :", GRID_ALIGN_RIGHT);
	csFmt = GetDataManage()->GetBridge()->m_sDeungeub.Left(1) + "���";
	long nSteelGrade = pSteelGrade->GetIndex(csFmt);
	double DB = pSteelGrade->GetDB( nSteelGrade < 0 ? 0 : nSteelGrade ) / 10000 ;
	cs.Format("DB-%.0lf, DL-%.0lf",DB,DB);
	Dom.SetTextMatrix(5, 1, cs, GRID_ALIGN_LEFT);
	Dom.SetGridLines(GVL_NONE);
	Dom.Draw();
	Dom.DrawGirdOutLine(TRUE);

	Dom.SetCalcExtRect();		
	Dom.Move(pMove->GetPoint(1100));
	pMove->AddMPE(&Dom,Dom.GetExtRect(),1100,"�����");
	*pAdd << Dom;
}

void CAPlateDrawStd::AddJubujebubuje(CDomyun* pAdd, double Scale)
{
	CAPlateDrawStd	*pStd		= (CAPlateDrawStd*)m_pDrawPageMng->m_pDrawStd;
	CARoadOptionStd *pOpt		= GetDataManage()->GetOptionStd();	
	CSteelSection	*pSteelSec  = GetDataManage()->GetSteelSection();
	COutMovePoint	*pMove = pStd->GetMovePoint(pStd->GetCurPage());	

	CGridDomyunEx Dom(pAdd);
	pOpt->SetEnvType(&Dom,HCAD_TBL1);

	CDRect gr = GetDogak()->GetRectDraw(Scale);
	double x  = gr.right - Dom.Always(200);
	double y  = gr.bottom;
					
	CStringArray m_szarrMain;
	CStringArray m_szarrSub;

	typedef struct _DOMMAT
	{		
		long	nMarkBuje;	// �����
		char*	szName;		// ���� ǥ��
	} DOMMAT;

	DOMMAT mat[] = {	{G_F_U,		"��� �÷���"},
						{G_F_L,		"�Ϻ� �÷���"},
						{G_W,		"������"},						
						{V_ST,		"���� ������"},
						{G_H_L,		"���� ������"},
						{SP_U_U,	"�ֺ��� ������"},
						{CG_W,		"�Ϲݺ� ���κ�"},						
						{CP_W,		"������ ���κ�"},
						{GI_SO,		"SOLE PLATE"},
						{CG_SP_W_SH,"�κ��� ������"},
						{V_J_ST,	"���� ������"},
						{CG_V,		"���κ� ������"},
						{GI_JA_L,	"�������� ��� ������"},						
					};
	
	const long c_ntotal	= sizeof(mat)/sizeof(DOMMAT);
	
	for( int i = 0; i < c_ntotal; i++ )
	{
		CString cs = pSteelSec->GetBujeTypeByMarkBuje( mat[i].nMarkBuje );
					
		if(cs == _T("�ֺ���") )
					m_szarrMain.Add( mat[i].szName );
		else		m_szarrSub.Add ( mat[i].szName );		
	}
	
	// const string
	//Main & Sub Material
	m_szarrMain.Add( "�� ���¼����� ���� ����" );
	m_szarrSub.Add ( "�� 2���� ����� ���� ���� ����");

	long nRow = 0;
	Dom.SetBasePoint(CDPoint(x+Dom.Always(70),y - Dom.GetTextHeight() * 0.5));
	Dom.SetGridLines(GVL_NONE);
	Dom.SetColumnCount(2);
	// ���� SetRowCount���� 1...���� SetRowHeightAll ���� 1������ ����..Draw�ϱ� ���� �ѹ���..
	Dom.SetRowCount(nRow+1);
	Dom.SetColumnWidth(0,Dom.Always(10));	
	Dom.SetColumnWidth(1,Dom.Always(90));	
	CString szText = "";

	Dom.SetTextMatrix(0,0,"�ֺ��� :",GRID_ALIGN_RIGHT);
	long nSize = m_szarrMain.GetSize();
	long iIdx = 0;
	for(iIdx=0;iIdx<nSize;iIdx++)
	{
		CString szOne = m_szarrMain.GetAt(iIdx);
		while(!szOne.IsEmpty())
		{
			long nPos = szOne.Find(' ');
			if(nPos!=-1) 
				szText += szOne.Left(nPos) + " ";
			else
				szText += szOne + (iIdx < nSize - 2 ? ", " : " ");
			if(Dom.GetTextWidth(szText) > Dom.Always(100))
			{
				Dom.SetTextMatrix(nRow,1,szText,GRID_ALIGN_LEFT);
				Dom.SetRowCount(++nRow+1);
				szText = "";
			}
			if(nPos != -1)	
				szOne = szOne.Mid(nPos+1);
			else 
				szOne = "";
		}
			
	}
	if(!szText.IsEmpty())
	{
		Dom.SetTextMatrix(nRow,1,szText,GRID_ALIGN_LEFT);		
		Dom.SetRowCount(++nRow+1);
		szText = "";
	}

	Dom.SetRowCount(nRow+1);
	Dom.SetTextMatrix(nRow,0,"�κ��� :",GRID_ALIGN_RIGHT);
	nSize = m_szarrSub.GetSize();

	for(iIdx=0;iIdx<nSize;iIdx++)
	{
		CString szOne = m_szarrSub.GetAt(iIdx);
		while(!szOne.IsEmpty())
		{
			long nPos = szOne.Find(' ');
			if(nPos!=-1) 
				szText += szOne.Left(nPos) + " ";
			else
				szText += szOne + (iIdx < nSize - 2 ? ", " : " ");
			if(Dom.GetTextWidth(szText) > Dom.Always(100))
			{
				Dom.SetTextMatrix(nRow,1,szText,GRID_ALIGN_LEFT);
				Dom.SetRowCount(++nRow+1);
				szText = "";
			}
			if(nPos != -1)	
				szOne = szOne.Mid(nPos+1);
			else 
				szOne = "";
		}
	}
	if(!szText.IsEmpty())
	{
		Dom.SetTextMatrix(nRow,1,szText,GRID_ALIGN_LEFT);		
		szText = "";
	}
	Dom.SetRowHeightAll(Dom.GetTextHeight()*1.3);	
	Dom.Draw();	

	Dom.SetCalcExtRect();		
	Dom.Move(pMove->GetPoint(1200));
	pMove->AddMPE(&Dom,Dom.GetExtRect(),1200,"���籸��");
	*pAdd << Dom;
}


void CAPlateDrawStd::ChangeSeperateLayer(CDomyun *pDom)
{				
	CARoadOptionStd	*pOpt	= (CARoadOptionStd*)m_pOptStd;
	POSITION pos = pDom->m_DrawObjMap.GetStartPosition(),p2;
	try
	{
		while(pos)
		{	
			p2 = pos;					
			CDrawObject *Key, *Value;
			pDom->m_DrawObjMap.GetNextAssoc(pos,(void *&)Key,(void *&)Value);				
			/*
			if(Value->GetEttKind()==ETT_ARC && CHgBaseDrawStd::GetHCadSymbolDom()->GetSize()!=0)
			{
				CString szLayer = Value->GetsLayer();				
				CString szCadLayer = pOpt->m_LayerList[HCAD_DIML]->m_szCadLayername;
				if(szLayer.Right(szCadLayer.GetLength()) == szCadLayer)
				{
					CObArc *pArc = (CObArc *)Value;					
					if(!pArc->m_bCircle || !pArc->m_bFillstyle) continue;
					CDPoint ptCen = pArc->m_SttPoint;
					double dAng = pArc->m_dRotate;
					pDom->RemoveObject(pArc);					
					CDomyun Dom(pDom);
					CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGDIMDT2", ptCen.x, ptCen.y, dMainScale * pOpt->m_DimSet.ArrowSize);																				
					if(dAng!=0) Dom.Rotate(dAng, ptCen.x, ptCen.y);
					*pDom << Dom;
					continue;
				}
			}
			*/
			if(Value->GetEttKind()==ETT_TEXT)
			{
				CObText *pText = (CObText *)Value;
				CString szLayer = Value->GetsLayer();
				CString szCadLayer = pOpt->m_LayerList[HCAD_DIML]->m_szCadLayername;
				CString szCadLayer2 = pOpt->m_LayerList[HCAD_TBL1]->m_szCadLayername;
				CString szCadLayer3 = pOpt->m_LayerList[HCAD_WELD]->m_szCadLayername;
				CString szCadLayer4 = pOpt->m_LayerList[HCADEX_GISI]->m_szCadLayername;
				if(szLayer.Right(szCadLayer.GetLength()) == szCadLayer)
				{
					szLayer.Replace(pOpt->m_LayerList[HCAD_DIML]->m_szCadLayername, pOpt->m_LayerList[HCAD_DIMT]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if(szLayer.Right(szCadLayer4.GetLength()) == szCadLayer4)
				{
					szLayer.Replace(pOpt->m_LayerList[HCADEX_GISI]->m_szCadLayername, pOpt->m_LayerList[HCADEX_GIST]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if(szLayer.Right(szCadLayer2.GetLength()) == szCadLayer2)
				{
					szLayer.Replace(pOpt->m_LayerList[HCAD_TBL1]->m_szCadLayername, pOpt->m_LayerList[HCAD_TBTC]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if(szLayer.Right(szCadLayer3.GetLength()) == szCadLayer3)
				{
					szLayer.Replace(pOpt->m_LayerList[HCAD_WELD]->m_szCadLayername, pOpt->m_LayerList[HCAD_DIMT]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
			}
			else if(Value->GetEttKind()==ETT_MTEXT)
			{
				CObMText *pText = (CObMText *)Value;
				CString szLayer = Value->GetsLayer();
				CString szCadLayer = pOpt->m_LayerList[HCAD_DIML]->m_szCadLayername;
				CString szCadLayer2 = pOpt->m_LayerList[HCAD_TBL1]->m_szCadLayername;
				CString szCadLayer3 = pOpt->m_LayerList[HCAD_WELD]->m_szCadLayername;
				CString szCadLayer4 = pOpt->m_LayerList[HCADEX_GISI]->m_szCadLayername;
				if(szLayer.Right(szCadLayer.GetLength()) == szCadLayer)
				{
					szLayer.Replace(pOpt->m_LayerList[HCAD_DIML]->m_szCadLayername, pOpt->m_LayerList[HCAD_DIMT]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if(szLayer.Right(szCadLayer4.GetLength()) == szCadLayer4)
				{
					szLayer.Replace(pOpt->m_LayerList[HCADEX_GISI]->m_szCadLayername, pOpt->m_LayerList[HCADEX_GIST]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if(szLayer.Right(szCadLayer2.GetLength()) == szCadLayer2)
				{
					szLayer.Replace(pOpt->m_LayerList[HCAD_TBL1]->m_szCadLayername, pOpt->m_LayerList[HCAD_TBTC]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if(szLayer.Right(szCadLayer3.GetLength()) == szCadLayer3)
				{
					szLayer.Replace(pOpt->m_LayerList[HCAD_WELD]->m_szCadLayername, pOpt->m_LayerList[HCAD_DIMT]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
			}			
		}
	}
	catch(...) {}
}