// HDanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "HDanDlg.h"
#include "BindConcJewonDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "RistModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHDanDlg dialog


CHDanDlg::CHDanDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CHDanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHDanDlg)
	m_nGirderSu		= 0;
	m_nCurGirder	= 0;
	m_nCurJijum		= 0;
	m_dMoveDist		= 0.0;
	m_bCheckJijumSame = TRUE;
	//}}AFX_DATA_INIT
}


void CHDanDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHDanDlg)	
	DDX_Control(pDX, IDC_EDIT_MOVEDIST, m_ctlMoveDist);
	DDX_Control(pDX, IDC_HDAN_GIRDERSU, m_ComboGirsu);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRIDN, m_GridN);
	DDX_CBIndex(pDX, IDC_HDAN_GIRDERSU, m_nGirderSu);			
	DDX_Text(pDX, IDC_EDIT_MOVEDIST, m_dMoveDist);
	DDX_Check(pDX, IDC_ALLJIJUM_SAME, m_bCheckJijumSame);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RIST_RECOMMEND, m_btnRecommend);
	DDX_Control(pDX, IDC_BUTTON_OFFSETONL1, m_btnOffset);
	DDX_Control(pDX, IDC_BUTTON_OFFSETONL2, m_btnMove);
	DDX_Control(pDX, IDC_ALLGIRGER_SAME, m_btnSameAll);
	DDX_Control(pDX, IDC_BINDCONC, m_btnConc);
	DDX_Control(pDX, IDC_BUTTON_CALC_AUTO, m_btnCalc);
}

BEGIN_MESSAGE_MAP(CHDanDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CHDanDlg)
	ON_CBN_SELCHANGE(IDC_HDAN_GIRDERSU, OnSelchangeHdanGirdersu)
	ON_BN_CLICKED(IDC_BUTTON_CALC_AUTO, OnButtonCalcAuto)	
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_BUTTON_OFFSETONL1, OnButtonOffsetonl1)
	ON_BN_CLICKED(IDC_BUTTON_OFFSETONL2, OnButtonOffsetonl2)
	ON_BN_CLICKED(IDC_ALLGIRGER_SAME, OnAllGirderSame)
	ON_BN_CLICKED(IDC_RIST_RECOMMEND, OnRistRecommend)
	ON_BN_CLICKED(IDC_BINDCONC, OnBindConc)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRIDN, OnCellChangedGridN)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRIDN, OnCellChangedDataGridN)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHDanDlg message handlers

BOOL CHDanDlg::OnInitDialog() 
{	
	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_RIST_RECOMMEND, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GRIDN, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_HDAN, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_GIRDER, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_ALLGIRGER_SAME,	SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_ALLJIJUM_SAME,	SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BINDCONC,			SZ_TOP_LEFT, SZ_TOP_LEFT);


 	// Button Draw Icon
	//CSize(30, 17)
	CBitmap bitRecommand;

	bitRecommand.LoadBitmap(IDB_MARK_RIST);
	m_btnRecommend.SetBitmap(bitRecommand);

	return TRUE;
}

void CHDanDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	CAPlateDrawDanmyun DrawDanmyun(m_pStd->GetDataManage());

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	if(m_nTab!=0)
	{
		CDomyun Dom(pDom);
		Dom.SetScaleDim(1);
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		CPlateBridgeApp *pDB = m_pStd->GetBridge();
		CString szMaterial = pDB->GetMaterial(G_F_U);	
		CRistModule RistModule;
		long nRow = m_GridN.GetFocusCell().row;
		long nCol = m_GridN.GetFocusCell().col;
		if(nRow<0) nRow=1;
		if(nCol<1) nCol=1;
		double dMaxJigan = toM(RistModule.GetLengthMaxJigan(pDB->m_nQtyJigan, pDB->m_dLengthJigan));
		if(m_pStd->GetDlgOfIDD(IDD_GEN_05_JIGANGUSENG)->IsUsedFlag())
			RistModule.DrawRistGraph(&Dom, dMaxJigan, toM(m_GridN.GetTextMatrixDouble(nRow,nCol)), szMaterial=="SM520C-TMC");
		else
			Dom.TextOut(0, 0, "         지간구성 입력을 먼저 하신 후 수행하시기 바랍니다.         ");
		Dom.SetCalcExtRect();
		CDRect BoundRect  = Dom.GetExtRect();

		// 화면의 중앙으로 정렬
		pOptStd->SetEnvType(&Dom, HCAD_HIDD);

		Dom.SetCalcExtRect();
		BoundRect = Dom.GetExtRect();
		BoundRect.top -= BoundRect.Height()*0.1;
		BoundRect.bottom +=BoundRect.Height() * 0.1;
		Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);
		*pDom << Dom;

		CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
		return;
	}
	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	DrawDanmyun.DrawHDanSlabForInput(&Dom);
	DrawDanmyun.DrawHDanGirderForInput(&Dom, m_nCurJijum);
	
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 75*(dDiagonalL/10000);
	Dom.SetScaleDim(dScale);

	CString strJijum = m_GridN.GetTextMatrix(m_nCurJijum+1, 0);
	DrawDanmyun.DrawHDanJewonDimForInput(&Dom, m_nCurGirder, m_nCurJijum, strJijum);
 	DrawDanmyun.DrawHDanDimForInput(&Dom, m_nCurJijum);
 	DrawDanmyun.DrawHDanBaseLineForInput(&Dom);

	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CHDanDlg::SetDataInit()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	GetDlgItem(IDC_BINDCONC)->ShowWindow(pDB->IsTUGir()?SW_SHOW:SW_HIDE);

	//////////////////////////////////////////////////////////////////////////
	// 횡단구성 그리드
	long nGirderSu = pDB->GetGirdersu();

	m_nGirderSu = nGirderSu-1;

	UpdateData(FALSE);

	SetGridTitle();

	m_Grid.SetTextMatrix(1, 1, "%g", pDB->GetSlabLeft()->m_dGirderCenter);
	m_Grid.SetTextMatrix(1, nGirderSu + 2, "%g", pDB->GetSlabRight()->m_dGirderCenter);
	long c = 0;
	for (c=2; c < nGirderSu+2; c++)	
	{		
		m_Grid.SetTextMatrix(1, c, "%g", pDB->GetGirder(c-2)->m_dGirderCenter);		
	}
	c++;
	m_Grid.SetTextMatrix(1, c++, pDB->GetSlabLeft()->GetLine()->GetNameLineEach());
	long nG=0;
	for ( ; c < nGirderSu*2+4; c++)
	{
		m_Grid.SetTextMatrix(1, c, pDB->GetGirder(nG++)->GetLine()->GetNameLineEach());
	}
	m_Grid.SetTextMatrix(1, c, pDB->GetSlabRight()->GetLine()->GetNameLineEach());
	m_Grid.SetRedraw(TRUE,TRUE);

	//////////////////////////////////////////////////////////////////////////
	// 거더높이 그리드
	CPlateGirder *pGir		= pDB->GetGirder(m_nCurGirder);

	CString szMaterial = pDB->GetMaterial(G_F_U);		
//	GetDlgItem(IDC_RIST_RECOMMEND)->ShowWindow(pDB->IsPlateGir() ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_RIST_RECOMMEND)->ShowWindow(SW_HIDE);	// #0031823 Rist 와 계약종료로 해당 기능 감춤
	
	SetGridTitle();

	CString str;
	
	m_UserDlg.m_bSM520 = szMaterial=="SM520C-TMC";
	m_UserDlg.m_pBridge = pDB;
	for(long k = 0; k < pDB->m_nQtyJigan + 1; k++)
	{
		m_GridN.SetTextMatrix(k + 1, 0, pDB->m_strJijumName[k]);
	}

	for(long nJijum=0; nJijum<pDB->m_nQtyJigan+1;nJijum++)
	{
		for(long nGir=0; nGir < pDB->GetGirdersu(); nGir++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nGir);

			m_GridN.SetTextMatrix(nJijum + 1, nGir + 1, "%g", pGir->GetHeightGirderByJijum(nJijum, FALSE));			
			if(!pDB->IsPlateGir() && m_pStd->GetDlgOfIDD(IDD_GEN_05_JIGANGUSENG)->IsUsedFlag() && (szMaterial=="SM520C-TMC" || szMaterial=="SM570-TMC"))
			{			
				m_GridN.SetUserComboDialog(nJijum + 1, nGir + 1, &m_UserDlg);
				m_GridN.SetCellType(nJijum + 1, nGir + 1,CT_USERDIALOGCOMBO_EDIT);
			}
			else
			{
				m_GridN.SetUserComboDialog(nJijum + 1, nGir + 1, NULL);
				m_GridN.SetCellType(nJijum + 1, nGir + 1,CT_EDIT);
			}
		}
	}

	m_GridN.SetRedraw(TRUE,TRUE);
}

//////////////////////////////////////////////////////////
///거더수가 0보다 작으면(데이터가 저장되지 않은 상태)거더수는 3
//////////////////////////////////////////////////////////
void CHDanDlg::SetDataDefault()
{					
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	//////////////////////////////////////////////////////////////////////////
	// 횡단구성 권고안
	long	nGsu		= 5;
	double	dHeightGir	= 2000;
	if(!pDB->IsPlateGir())	//소수주형, TU거더
		nGsu = 2;
	if(IsUsedFlag())	nGsu = m_nGirderSu+1;

	m_pStd->GetBridge()->MakeGirder(nGsu);	
	m_pStd->GetBridge()->MakeSlab();

	if(!pDB->IsPlateGir())		// 소수주형, TU거더
	{
		pDB->m_dGirderSpace        = pDB->IsTUGir() ? 6900 : 6400;
		pDB->m_dWidthSlabLeft      = pDB->IsTUGir() ? 2800 : 3100;
		pDB->m_dBridgeWidth        = pDB->m_dGirderSpace*(nGsu-1)+pDB->m_dWidthSlabLeft*2;//12000;
		pDB->m_dDisLineToBridgeCen = 0;
		pDB->GetSlabLeft()->m_dGirderCenter  = -(pDB->m_dGirderSpace*(nGsu-1)/2+pDB->m_dWidthSlabLeft);//-6000;
		pDB->GetSlabRight()->m_dGirderCenter = +(pDB->m_dGirderSpace*(nGsu-1)/2+pDB->m_dWidthSlabLeft);//6000;
	}
	else
	{
		pDB->m_dGirderSpace        = 2500;
		pDB->m_dWidthSlabLeft      = 1200;
		pDB->m_dBridgeWidth        = pDB->m_dGirderSpace*(nGsu-1)+pDB->m_dWidthSlabLeft*2;//12400;
		pDB->m_dDisLineToBridgeCen = 0;
		pDB->GetSlabLeft()->m_dGirderCenter  = -(pDB->m_dGirderSpace*(nGsu-1)/2+pDB->m_dWidthSlabLeft);//-6200;
		pDB->GetSlabRight()->m_dGirderCenter = +(pDB->m_dGirderSpace*(nGsu-1)/2+pDB->m_dWidthSlabLeft);//6200;
	}
	
	double dis = pDB->GetSlabLeft()->m_dGirderCenter+(pDB->m_dBridgeWidth-(nGsu-1)*pDB->m_dGirderSpace)/2;
	
	for(long nG=0; nG<nGsu; nG++)
	{
		pDB->GetGirder(nG)->m_dGirderCenter = dis;
		for(long nJ=0; nJ<pDB->m_nQtyJigan+1; nJ++)
		{
			if(pDB->GetGirder(nG)->GetHeightGirderByJijum(nJ)==0)
				pDB->GetGirder(nG)->SetHeightGirderAtJijum(nJ,dHeightGir);
		}
		dis += pDB->m_dGirderSpace;					
	}	

	for(long nJ=0; nJ<pDB->m_nQtyJigan+1; nJ++)
	{
		if(pDB->GetGirder(-1)->GetHeightGirderByJijum(nJ)==0)
			pDB->GetGirder(-1)->SetHeightGirderAtJijum(nJ,dHeightGir);
	}

	//////////////////////////////////////////////////////////////////////////
	// 거더높이 권고안

	long nK = 0;

	CString str="";
	// 지점명 설정
	for(long k = 0; k < pDB->m_nQtyJigan+1; k++)
	{
		if(k == 0 && pDB->m_nTypeAbutOrPier[STT_POS] == 0)						str = _T("A1");
		else if(k == pDB->m_nQtyJigan && pDB->m_nTypeAbutOrPier[END_POS] == 0)	str = _T("A2");
		else
		{
			if(k == 0 && pDB->m_nTypeAbutOrPier[STT_POS] == 1) nK++;

			str.Format("P%d",nK);
		}

		pDB->m_strJijumName[k] = str;

		nK++;
	}
	// 형고비를 적용하여 거더높이을 산정(현재 상태에서 지간길이가 설정되지 않기 때문에 
	// 전체교량길이를 지간수로 나눈값을 평균지간길이로 함(100자리에서 반올림)
	for(long nJijum=0; nJijum<pDB->m_nQtyJigan+1;nJijum++)
	{
		for(long nGir=0; nGir < pDB->GetGirdersu(); nGir++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nGir);
			pGir->SetHeightGirderAtJijum(nJijum, pGir->GetValueBasicDefault("WEB_H"));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 구속 콘크리트 권고안 (TU)

	CBindConcJewonDlg dlg(m_pStd, this);

	if(!IsUsedFlag())
		dlg.SetDataDefault();

//	SetModifyInputData(FALSE);
}

void CHDanDlg::SetDataSave()
{
	//////////////////////////////////////////////////////////////////////////
	// 횡단구성 저장
	UpdateData(TRUE);
	
	long nGsu = m_nGirderSu+1;

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CLineInfoApp    *pLine   = m_pStd->GetLineInfoApp();

	pDB->GetSlabLeft()->m_dGirderCenter  = m_Grid.GetTextMatrixDouble(1, 1);
	pDB->GetSlabRight()->m_dGirderCenter = m_Grid.GetTextMatrixDouble(1, nGsu+2);
	pDB->m_dWidthSlabLeft  = pDB->GetGirder(0)->m_dGirderCenter - pDB->GetSlabLeft()->m_dGirderCenter;
	pDB->m_dWidthSlabRight = pDB->GetSlabRight()->m_dGirderCenter - pDB->GetGirder(nGsu-1)->m_dGirderCenter;
	pDB->m_dBridgeWidth    = fabs(pDB->GetSlabRight()->m_dGirderCenter-pDB->GetSlabLeft()->m_dGirderCenter);
	long nG = 0;
	for(nG=0; nG<nGsu; nG++)
		pDB->GetGirder(nG)->m_dGirderCenter = m_Grid.GetTextMatrixDouble(1, nG+2);		

	for(nG=0; nG<nGsu; nG++)				///<거더 너비
	{										///<거더 센터에서 자기 선형까지의 이격 거리
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		
		for(long i=0; i<pLine->GetLinesu();i++)
		{
			CString str = pLine->GetLine(i)->GetNameLineEach();

			if(pLine->GetLine(i)->GetNameLineEach() == m_Grid.GetItemText(1, nGsu+3+nG))
			{
				pGir->m_nLineNum = i;
				pGir->SetLine(pLine->GetLine(i));
			}
		}
	}	

	long nK = 0;
	CString str;
	for(long k = 0; k < pDB->m_nQtyJigan+1; k++)
	{
		if(k == 0 && pDB->m_nTypeAbutOrPier[STT_POS] == 0)						str = _T("A1");
		else if(k == pDB->m_nQtyJigan && pDB->m_nTypeAbutOrPier[END_POS] == 0)	str = _T("A2");
		else
		{
			if(k == 0 && pDB->m_nTypeAbutOrPier[STT_POS] == 1) nK++;
			str.Format("P%d",nK);
		}
		pDB->m_strJijumName[k] = str;
		nK++;
	}

	//////////////////////////////////////////////////////////////////////////
	// 거더높이 저장

	for(long nJijum = 0; nJijum < pDB->m_nQtyJigan + 1; nJijum++)
	{
		pDB->m_strJijumName[nJijum]	= m_GridN.GetTextMatrix(nJijum + 1, 0);

		for(long nGir = -1; nGir < pDB->GetGirdersu(); nGir++)
		{			
			CPlateGirderApp *pGir = pDB->GetGirder(nGir);
			
			pGir->SetHeightGirderAtJijum(nJijum,m_GridN.GetTextMatrixDouble(nJijum + 1, ((nGir==-1) ? 0 : nGir) + 1));
		}
	}
}

void CHDanDlg::SetDataBx()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	CPtrList NList;
	long nG = 0;
	for(nG = -1; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		CLineInfo* pLine = pGir->GetLine();		
		for(double sta = pDB->GetStationBridgeStt(); sta <= pDB->GetStationBridgeEnd(); )
		{
			CLineSplit* pSplit = pLine->GetLineSplitByStation(sta);
			double dPrevSta = sta;
			sta = pSplit->GetStationNextLineType(sta,pLine->m_dStationBp,TRUE);

			// 에러이거나 범위를 벗어나는 경우			
			if(dPrevSta >= sta || sta > pDB->GetStationBridgeEnd()) break;

			CPlateBasicIndex* p = pGir->newBx();
			p->SetState(BX_LINE_P,TRUE);
			p->SetStation(sta);
			NList.AddTail(p);
		}
		
		// set BxList
		CDWordArray LKExceptionArr;
		pGir->PressBxList(NList, BX_LINE_P,&LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);		
	}

	for(nG = -1; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		CLineInfo* pLine = pGir->GetLine();

		double dSttSta = pDB->m_dStationBridgeStt;
		double dEndSta = dSttSta+pDB->m_dLengthTotalSlab;
		
		long nVIP_Prev = pLine->GetJongDanVIP(dSttSta) - 1, //교량 시점 이전의 n
			 nVIP_Next = pLine->GetJongDanVIP(dEndSta);		//교량 종점 이후의 n

		long nVIP = nVIP_Prev;

		double dBVC_Sta	   = 0;
		double dEVC_Sta	   = 0;		
		double dVIP_Sta    = pLine->m_VIPData[nVIP][0];		
		double dVIP_Length = pLine->m_VIPData[nVIP][2];

		if(dVIP_Length != 0) 		
		{
			dEVC_Sta = dVIP_Sta + dVIP_Length/2;		
			
			CPlateBasicIndex* p = pGir->newBx();
			p->SetState(BX_LINE_V,TRUE);
			p->SetStation(dEVC_Sta);
			NList.AddTail(p);
		}

		nVIP_Prev++;

		for(long n = nVIP_Prev; n <= nVIP_Next; n++)
		{
			if(n < nVIP_Next)
			{
				dVIP_Sta    = pLine->m_VIPData[n][0];			
				dVIP_Length = pLine->m_VIPData[n][2];

				// BVC
				dBVC_Sta = dVIP_Sta - pLine->m_VIPData[n][2]/2;			
				dEVC_Sta = dVIP_Sta + pLine->m_VIPData[n][2]/2;

				CPlateBasicIndex* p1 = pGir->newBx();
				p1->SetState(BX_LINE_V,TRUE);
				p1->SetStation(dBVC_Sta);
				NList.AddTail(p1);

				CPlateBasicIndex* p2 = pGir->newBx();
				p2->SetState(BX_LINE_V,TRUE);
				p2->SetStation(dEVC_Sta);
				NList.AddTail(p2);
			}
			else // if(n == nVIP_Next)
			{
				dVIP_Sta = pLine->m_VIPData[n][0];

				if(pLine->m_VIPData[n][2] != 0)
				{
					dBVC_Sta = dVIP_Sta - pLine->m_VIPData[n][2]/2;
					CPlateBasicIndex* p = pGir->newBx();
					p->SetState(BX_LINE_V,TRUE);
					p->SetStation(dBVC_Sta);
					NList.AddTail(p);
				}				
			}
		}		
		// set BxList
		CDWordArray LKExceptionArr;
		pGir->PressBxList(NList, BX_LINE_V,&LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);
	}
}

void CHDanDlg::SetGridTitle()
{	
	CPlateBridge *pDB = m_pStd->GetBridge();
	//////////////////////////////////////////////////////////////////////////
	// 횡단구성 타이틀
	long nGirderSu = m_nGirderSu+1;
	
	if(nGirderSu<1) return;	
	CString str;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(2);
	m_Grid.SetColumnCount(2*nGirderSu+3+2); // 슬래브 2개 포함

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	m_Grid.SetColumnWidthAll(60);		

	m_Grid.SetColumnWidth(0,65);		
	m_Grid.SetTextMatrix(0,0,"구 분");	
	m_Grid.SetTextMatrix(1,0,"길이(㎜)");	
				
	str = "SL";
	m_Grid.SetTextMatrix(0,1,str);
	m_Grid.SetCellType(1,1, CT_EDIT);
	str = "SR";
	m_Grid.SetTextMatrix(0,nGirderSu+2,str);
	m_Grid.SetCellType(1,nGirderSu+2, CT_EDIT);	
	long col = 0;
	for(col = 0;col < nGirderSu; col++)
	{	
		str.Format("L%ld",col+1);
		m_Grid.SetTextMatrix(0,col+2,str);
		m_Grid.SetCellType(1,col+2, CT_EDIT);
	}	

	m_Grid.SetTextMatrix(0,2+nGirderSu+1,"SL선형");
	m_Grid.SetColumnWidth(2+nGirderSu+1, 0);			// 임시로 막음(확폭시 적용할 것)
	for(col = 0; col < nGirderSu; col++)
	{
		str.Format("G%ld선형",col+1);
		m_Grid.SetTextMatrix(0, col+2+nGirderSu+2, str);
		m_Grid.SetColumnWidth(col+2+nGirderSu+2, 0);	// 임시로 막음(확폭시 적용할 것)
	}
	m_Grid.SetTextMatrix(0, 2+nGirderSu*2+2, "SR선형");
	m_Grid.SetColumnWidth(2+nGirderSu*2+2, 0);			// 임시로 막음(확폭시 적용할 것)

	CStringArray szArr;
	CLineInfoApp*	pLineInfoApp = m_pStd->GetLineInfoApp();
	for(long n = 0; n < nGirderSu;n++)
	{		
		CLineInfo* pLine = pLineInfoApp->GetLine(n);
		BOOL bFind=FALSE;
		for(long i=0; i<szArr.GetSize();i++) 
		{
			if(szArr.GetAt(i) == pLine->GetNameLineEach()) 
			{
				bFind = TRUE;
				break;
			}
		}
		if(n == 0 || !bFind)
			szArr.Add(pLine->GetNameLineEach());				
	}
	m_Grid.SetComboString(szArr);
	m_Grid.SetRedraw(TRUE,TRUE);

	//////////////////////////////////////////////////////////////////////////
	// 거더높이 타이틀
	
	m_GridN.SetEditable(TRUE);
	m_GridN.SetListMode(FALSE);
	m_GridN.EnableDragAndDrop(TRUE);
	m_GridN.EnableInsertRow(FALSE);
	m_GridN.EnableInsertCol(FALSE);	
	m_GridN.SetFixedRowCount(1);
	m_GridN.SetFixedColumnCount(0);
	m_GridN.EnableReturnPass(TRUE);

	m_GridN.SetRowCount(pDB->m_nQtyJigan + 2);
	m_GridN.SetColumnCount(pDB->GetGirdersu() + 1);

	m_GridN.SetRowHeightAll(20);
	m_GridN.SetRowHeight(0, 20);
	
	m_GridN.SetColumnWidthAll(70);		

	m_GridN.SetTextMatrix(0,0,"지점명");

	CString szData;

	for(long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		szData.Format("G%d",nG+1);
		m_GridN.SetTextMatrix(0, nG+1, szData);
	}
}

void CHDanDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);
	
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CHDanDlg::OnSelchangeHdanGirdersu() 
{	
	UpdateData(TRUE);

	//CalcGirderPosAuto();
	SetDataDefault();
	SetDataInit(); 
	SetDataSave();

	m_pStd->GetDlgOfIDD(IDD_CALC_SECFORCE_DIV_CALC)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_SECFORCE_DIV_RESULT)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_WINDFORCE)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_MODELING_SETUP)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_FRAME_COMBO)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_MODELING_RESULT)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_INF_K)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_SECTION_STRESS)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_SECTION_OUTLINE)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_WARPING)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_DISPLACEMENT)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_FATIGUE)->SetUsedFlag(FALSE);
	m_pStd->GetDlgOfIDD(IDD_CALC_WELD)->SetUsedFlag(FALSE);

	DrawInputDomyunView(TRUE);
}

void CHDanDlg::OnCellChangedGridN(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    long nRow = GridView->iRow;
	long nCol = GridView->iColumn;

	if(nCol>0)
		m_nCurGirder = nCol-1;
	m_nCurJijum = nRow-1;

	DrawInputDomyunView(TRUE);
	CGeneralBaseDlg::OnCellChangedGridN(nmgv, 0);	
	m_pView->GetDomyun()->STMakeCursor(m_GridN.GetItemText(nRow,0));
}

void CHDanDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CGeneralBaseDlg::OnCellChangedData(nmgv, 0);

	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CHDanDlg::OnCellChangedDataGridN(NMHDR* nmgv, LRESULT*)
{
	UpdateData(TRUE);
	if(m_bCheckJijumSame)
	{
		long nCurRow = m_GridN.GetFocusCell().row;
		long nCurCol = m_GridN.GetFocusCell().col;
		if(nCurRow>0 && nCurCol>0)
		{
			for(long n=1; n<m_GridN.GetRowCount(); n++)
				m_GridN.SetTextMatrix(n, nCurCol, "%g", m_GridN.GetTextMatrixDouble(nCurRow, nCurCol));
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	SetGridTitle();
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CHDanDlg::OnButtonCalcAuto() 
{
	UpdateData(TRUE);

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(FALSE);

}

BOOL CHDanDlg::IsFocusItem(CString szTitle)
{
	for(long nRow=1; nRow<m_GridN.GetRowCount(); nRow++)
	{
		if(m_GridN.GetTextMatrix(nRow, 0) == szTitle)
			return TRUE;
	}

	for(long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
	{
		if(m_Grid.GetTextMatrix(0, nCol) == szTitle)
			return TRUE;
	}

	return CGeneralBaseDlg::IsFocusItem(szTitle);
}

void CHDanDlg::SetFocusItem(CString szTitle)
{
	for(long nRow=1; nRow<m_GridN.GetRowCount(); nRow++)
	{
		if(m_GridN.GetTextMatrix(nRow, 0) == szTitle)
		{
			m_GridN.SetFocusCell(nRow, m_GridN.GetFocusCell().col);
			m_GridN.SendMessage(WM_KEYDOWN,VK_F2);
			return;
		}
	}
	
	for(long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
	{
		if(m_Grid.GetTextMatrix(0, nCol) == szTitle)
		{
			m_Grid.SetFocusCell(1, nCol);
			m_Grid.SendMessage(WM_KEYDOWN,VK_F2);
			return;
		}
	}
}

/*
// 거더의 위치를 교량중심을 기준으로 양측으로 거더배치를 균일하게 생성함.
void CHDanDlg::CalcGirderPosAuto()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	long nGsu = m_nGirderSu+1;
	// 형고비를 적용하여 거더높이을 산정(현재 상태에서 지간길이가 설정되지 않기 때문에 
	// 전체교량길이를 지간수로 나눈값을 평균지간길이로 함(100자리에서 반올림)
	double dLengthJigan = pBridge->m_dLengthTotalSlab/pBridge->m_nQtyJigan; 
	for(long nG=0; nG<nGsu; nG++)
	{
		pBridge->GetGirder(nG)->m_dGirderCenter = 0;
		for(long nJijum=0; nJijum<pBridge->m_nQtyJigan+1;nJijum++)
		{
			for(long nGir=0; nGir < pBridge->GetGirdersu(); nGir++)
			{
				if(pBridge->GetGirder(nG)->GetHeightGirderByJijum(nJijum)==0)
					pBridge->GetGirder(nGir)->SetHeightGirderAtJijum(nJijum, Round(dLengthJigan/22, -2));
			}
		}
	}
	
	if(nGsu%2==0)//짝수거더
	{
		//좌측
		long nGCen = nGsu/2-1;
		long n=0;
		for(nG=nGCen; nG>=0; nG--)
		{
			if(nG==nGCen)
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->m_dDisLineToBridgeCen-pBridge->m_dGirderSpace/2;
			else
			{
				long Dis = (n+1)*pBridge->m_dGirderSpace;
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->GetGirder(nGCen)->m_dGirderCenter - Dis;
				n++;
			}
		}
		//우측
		nGCen = nGsu/2;
		n=0;
		for(nG=nGCen; nG<nGsu; nG++)
		{
			if(nG==nGCen)
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->m_dDisLineToBridgeCen+pBridge->m_dGirderSpace/2;
			else
			{
				long Dis = (n+1)*pBridge->m_dGirderSpace;
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->GetGirder(nGCen)->m_dGirderCenter + Dis;
				n++;
			}
		}
	}
	else
	{
		//좌측
		long nGCen = nGsu/2;
		long n=0;
		for(nG=nGCen; nG>=0; nG--)
		{
			if(nG==nGCen)
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->m_dDisLineToBridgeCen;
			else
			{
				long Dis = (n+1)*pBridge->m_dGirderSpace;
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->GetGirder(nGCen)->m_dGirderCenter - Dis;
				n++;
			}
		}
		//우측
		nGCen = nGsu/2;
		n=0;
		for(nG=nGCen; nG<nGsu; nG++)
		{
			if(nG==nGCen)
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->m_dDisLineToBridgeCen;
			else
			{
				long Dis = (n+1)*pBridge->m_dGirderSpace;
				pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->GetGirder(nGCen)->m_dGirderCenter + Dis;
				n++;
			}
		}
	}

	pBridge->GetSlabLeft()->m_dGirderCenter  = pBridge->m_dDisLineToBridgeCen-pBridge->m_dBridgeWidth/2;
	pBridge->GetSlabRight()->m_dGirderCenter = pBridge->m_dDisLineToBridgeCen+pBridge->m_dBridgeWidth/2;
	pBridge->m_dWidthSlabLeft				 = pBridge->GetGirder(0)->m_dGirderCenter - pBridge->GetSlabLeft()->m_dGirderCenter;
	pBridge->m_dWidthSlabRight				 = pBridge->GetSlabRight()->m_dGirderCenter - pBridge->GetGirder(nGsu-1)->m_dGirderCenter;
	pBridge->GetGirder(-1)->m_dGirderCenter  = 0;

}


// 거더의 위치를 좌측캔틸레버길이를 기준으로 계산함.
void CHDanDlg::CalcGirderPosAutoByCantilever()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	long nGsu = m_nGirderSu+1;
	// 형고비를 적용하여 거더높이을 산정(현재 상태에서 지간길이가 설정되지 않기 때문에 
	// 전체교량길이를 지간수로 나눈값을 평균지간길이로 함(100자리에서 반올림)
	double dLengthJigan = pBridge->m_dLengthTotalSlab/pBridge->m_nQtyJigan; 
	for(long nG=0; nG<nGsu; nG++)
	{
		pBridge->GetGirder(nG)->m_dGirderCenter = 0;
		for(long nJijum=0; nJijum<pBridge->m_nQtyJigan+1;nJijum++)
		{
			for(long nGir=0; nGir < pBridge->GetGirdersu(); nGir++)
			{
				if(pBridge->GetGirder(nG)->GetHeightGirderByJijum(nJijum)==0)
					pBridge->GetGirder(nGir)->SetHeightGirderAtJijum(nJijum,Round(dLengthJigan/22, -2));
			}
		}
	}

	// nG=-1 : 좌측슬래브
	// nG=pBridge->GetGirdersu() : 우측슬래브
	double dLeftCant = frM(atof(m_strLengthCantilever));
	for(nG=-1; nG<=pBridge->GetGirdersu(); nG++)
	{
		if(nG==-1)	// 좌측슬래브
			pBridge->GetSlabLeft()->m_dGirderCenter = pBridge->m_dDisLineToBridgeCen-pBridge->m_dBridgeWidth/2;
		else if(nG>=0 && nG<pBridge->GetGirdersu())	// 거더
			pBridge->GetGirder(nG)->m_dGirderCenter = pBridge->GetSlabLeft()->m_dGirderCenter+dLeftCant+pBridge->m_dGirderSpace*nG;
		else
			pBridge->GetSlabRight()->m_dGirderCenter= (pBridge->m_dDisLineToBridgeCen+pBridge->m_dBridgeWidth/2);
	}

	pBridge->m_dWidthSlabLeft				 = pBridge->GetGirder(0)->m_dGirderCenter - pBridge->GetSlabLeft()->m_dGirderCenter;
	pBridge->m_dWidthSlabRight				 = pBridge->GetSlabRight()->m_dGirderCenter - pBridge->GetGirder(nGsu-1)->m_dGirderCenter;
	pBridge->GetGirder(-1)->m_dGirderCenter  = 0;
}
*/

void CHDanDlg::OnButtonRecommend() 
{
	SetDataDefault();
	SetDataInit();
	SetDataSave();

	UpdateData(FALSE);

	DrawInputDomyunView(TRUE);
}

void CHDanDlg::OnButtonOffsetonl1() 
{
	UpdateData();
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	double dBaseD=0;
	long nGSu = pDB->GetGirdersu();
	dBaseD = pDB->GetGirder(0)->m_dGirderCenter;
	for(long nG=1; nG<nGSu; nG++)	
	{
		dBaseD += m_dMoveDist;
		pDB->GetGirder(nG)->m_dGirderCenter = dBaseD;					
	}
	SetDataInit();

	DrawInputDomyunView(TRUE);
}

void CHDanDlg::OnButtonOffsetonl2() 
{
	UpdateData();
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nGSu = pDB->GetGirdersu();
	for(long nG=0; nG<nGSu; nG++)	
	{		
		pDB->GetGirder(nG)->m_dGirderCenter += m_dMoveDist;					
	}
	pDB->GetSlabLeft()->m_dGirderCenter += m_dMoveDist;
	pDB->GetSlabRight()->m_dGirderCenter += m_dMoveDist;
	SetDataInit();

	DrawInputDomyunView(TRUE);
}

void CHDanDlg::OnAllGirderSame() 
{
	long nCurCol = m_GridN.GetFocusCell().col;
	if(nCurCol<=0) nCurCol = 1;

	CString str;
	str.Format("  %d번 거더 기준으로 제원을 배치합니다. \n    계속하시겠습니까?", nCurCol);
	if (IDCANCEL == MessageBox(str, "모든 거더 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	long nCols = m_GridN.GetCols();
	double dText;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pCurGir = pDB->GetGirder(nCurCol);

	for (long nCol = 1; nCol < nCols; nCol++)
	{
		if (nCol == nCurCol)
			continue;
		CPlateGirderApp *pGir = pDB->GetGirder(nCol);
		for(long nRow=1; nRow<pDB->m_nQtyJigan+2; nRow++)
		{
			dText = m_GridN.GetTextMatrixDouble(nRow, nCurCol);
			m_GridN.SetTextMatrix(nRow, nCol, "%g", dText);
		}
	}
	m_GridN.SetRedraw(TRUE,TRUE);

	SetGridTitle();
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CHDanDlg::OnRistRecommend() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	if(!m_pStd->GetDlgOfIDD(IDD_GEN_05_JIGANGUSENG)->IsUsedFlag())
	{
		AfxMessageBox("지간구성 입력을 먼저 하신 후 수행하시기 바랍니다.",MB_ICONWARNING);
		return;
	}

	CString szMaterialU = pDB->GetMaterial(G_F_U);
	CString szMaterialL = pDB->GetMaterial(G_F_L);
	if((szMaterialU!="SM520C-TMC" && szMaterialU!="SM570-TMC") || (szMaterialL!="SM520C-TMC" && szMaterialL!="SM570-TMC"))
	{
		AfxMessageBox("상하판 강재재질이 SM520C-TMC 또는 SM570-TMC일때만 지원됩니다.",MB_ICONWARNING);
		return;
	}
	
	CRistModule Rist;
	double dH = Rist.GetHeightGirder(pDB->m_nQtyJigan, pDB->m_dLengthJigan,szMaterialU=="SM520C-TMC");	

	
	for(long nJijum = 0; nJijum < pDB->m_nQtyJigan + 1; nJijum++)
	{
		for(long nGir = -1; nGir < pDB->GetGirdersu(); nGir++)
		{			
			CPlateGirderApp *pGir = pDB->GetGirder(nGir);			
			pGir->SetHeightGirderAtJijum(nJijum,dH);
		}
	}
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

long CHDanDlg::GetTabCount()
{
// #0031824 Rist 와 계약종료로 해당 기능 감춤
//	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
//	CString szMaterial = pDB->GetMaterial(G_F_U);
//	if ((szMaterial=="SM520C-TMC" || szMaterial=="SM570-TMC") && !pDB->IsPlateGir()) return 2;	
	return 1;
}

CString CHDanDlg::GetTabTitle(long nIdx)
{
	if(nIdx==0) return "입력";
	return "그래프 표기";
}

// 구속콘크리트 제원
void CHDanDlg::OnBindConc()
{
	CBindConcJewonDlg dlg(m_pStd, this);
	dlg.DoModal();
}

//void CHDanDlg::OnAllJijumSame() 
//{
//	CPlateBridgeApp *pDB = m_pStd->GetBridge();
//
//	long nCurRow = m_GridN.GetFocusCell().row;
//	long nCurCol = m_GridN.GetFocusCell().col;
//	if(nCurRow<0 || nCurCol <1) return;
//
//	CString str;
//	str.Format("  %s 기준으로 모든지점을 동일적용 합니다. \n    계속하시겠습니까?", pDB->m_strJijumName[nCurRow-1]);
//	if (IDCANCEL == MessageBox(str, "모든 지점 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
//		return;
//
//	for(long n=1; n<m_GridN.GetRowCount(); n++)
//	{
//		m_GridN.SetTextMatrix(n, nCurCol, "%g", m_GridN.GetTextMatrixDouble(nCurRow, nCurCol));
//	}
//
//	m_Grid.SetRedraw(TRUE,TRUE);
//
//	SetGridTitle();
//	SetDataSave();
//	DrawInputDomyunView(FALSE);	
//}

