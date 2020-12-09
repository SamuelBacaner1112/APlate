// JackupPositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "JackupPositionDlg.h"

#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJackupPositionDlg dialog


CJackupPositionDlg::CJackupPositionDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CJackupPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJackupPositionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CJackupPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJackupPositionDlg)
	DDX_Control(pDX, IDC_ALLPIER_SAME, m_ctrlPierSame);
	DDX_Control(pDX, IDC_ALLABUT_SAME, m_ctrlAbutSame);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ALLPIER_SAME, m_ctrlPierSame);
	DDX_Control(pDX, IDC_ALLABUT_SAME, m_ctrlAbutSame);
	DDX_Control(pDX, IDC_BUTTON_JACKUPSOLEPLATE, m_btnPosition);
}


BEGIN_MESSAGE_MAP(CJackupPositionDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CJackupPositionDlg)
	ON_BN_CLICKED(IDC_ALLABUT_SAME, OnAllabutSame)
	ON_BN_CLICKED(IDC_ALLPIER_SAME, OnAllpierSame)
	ON_BN_CLICKED(IDC_BUTTON_JACKUPSOLEPLATE, OnButtonJackupsoleplate)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJackupPositionDlg message handlers

void CJackupPositionDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(0);
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);

	if(m_nTab==0)
	{	
		CDomyun Dom(pDom);
		CAPlateDrawPyung Draw(m_pStd->GetDataManage());
		Draw.DrawPyung(&Dom, 2);
		*pDom << Dom;	
	}
	else
	{
		CDomyun Dom(pDom);
		Dom.SetScaleDim(20);
		CAPlateDrawDanmyun DrawDan(m_pStd->GetDataManage());
		long nJ = m_Grid.GetFocusCell().row - 1;
		if(nJ<0) nJ=0;
		CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nJ);
		DrawDan.DrawGirderOnBx(&Dom, pBx);		
		DrawDan.DrawJackUpStiffOnBx(&Dom, pBx);
		DrawDan.DimJackUpStiffOnBx(&Dom, pBx);
		*pDom << Dom;	
	}

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CJackupPositionDlg::SetGridTitle()
{
	CPlateBridge *pBridge = m_pStd->GetBridge();

	long nJigan  = pBridge->m_nQtyJigan;
	long nJackup = JACKUPSUMAX;		

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nJigan+2);
	m_Grid.SetColumnCount(nJackup+1+6);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(50);

	CString str;
	m_Grid.SetTextMatrix(0,0,"구 분");
	for (long nCol = 1; nCol < nJackup+1; nCol++)
	{
		str.Format("잭업 %d", nCol);
		m_Grid.SetTextMatrix(0, nCol, str);
	}
	m_Grid.SetTextMatrix(0, nJackup+1, "WL");
	m_Grid.SetTextMatrix(0, nJackup+2, "TL");
	m_Grid.SetTextMatrix(0, nJackup+3, "WR");
	m_Grid.SetTextMatrix(0, nJackup+4, "TR");
	m_Grid.SetTextMatrix(0, nJackup+5, "VL");
	m_Grid.SetTextMatrix(0, nJackup+6, "VG");

	long nRow = 1;
	for(long nJijum = 0; nJijum < nJigan+1; nJijum++)
		m_Grid.SetTextMatrix(nRow++, 0, pBridge->m_strJijumName[nJijum]);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJackupPositionDlg::SetDataInit()
{
	SetGridTitle();

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(0);
	//BOOL bError = FALSE;
	for(long nJijum = 0; nJijum <= pBridge->m_nQtyJigan; nJijum++)
	{
		for (long nJack = 0; nJack < JACKUPSUMAX; nJack++)
			m_Grid.SetTextMatrix(nJijum+1, nJack+1, "%.lf", pBridge->m_dJackUpTerm[nJijum][nJack]);

		CJackupStiff *pJack = pGir->GetJackupByJijum(nJijum, 0);
		if(pJack == NULL)	
		{			
			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+1, "%.lf", 180.0);
			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+2, "%.lf", 20.0);
			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+3, "%.lf", 180.0);
			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+4, "%.lf", 20.0);
			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+5, "%.lf", pGir->GetHeightGirderByStaAng(pGir->GetStationOnJijum(nJijum))/2);
			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+6, "%.lf", 10.0);
			//bError = TRUE;
			continue;
		}
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+1, "%.lf", pJack->m_vJewon[0].x);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+2, "%.lf", pJack->m_vJewon[0].y);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+3, "%.lf", pJack->m_vJewon[1].x);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+4, "%.lf", pJack->m_vJewon[1].y);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+5, "%.lf", pJack->m_dVL);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+6, "%.lf", pJack->m_dVG);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJackupPositionDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pDB->GetGirder(0);

	for(long nJijum = 0; nJijum <= pDB->m_nQtyJigan; nJijum++)
	{
		long nJack = 1;
		if(nJijum == 0)
		{
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 600.0);
		}				
		else if(nJijum == pDB->m_nQtyJigan)
		{			
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", -600.0);
		}
		else
		{
			m_Grid.SetTextMatrix(nJijum+1,nJack++,"%.lf",-600.0);
			m_Grid.SetTextMatrix(nJijum+1,nJack++,"%.lf",600.0);
		}
		for(long nJack1 = nJack; nJack1 <= JACKUPSUMAX;nJack1++)
		{
			m_Grid.SetTextMatrix(nJijum+1,nJack1,"%.lf",0.0);
		}
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+1, "%.lf", 200.0);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+2, "%.lf", 20.0);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+3, "%.lf", 200.0);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+4, "%.lf", 20.0);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+5, "%.lf", pGir->GetHeightGirderByStaAng(pGir->GetStationOnJijum(nJijum))/2);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+6, "%.lf", 10.0);
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	SetDataSave();
	SetDataBx();

}

void CJackupPositionDlg::SetDataSave()
{		
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long nJijum = 0; nJijum <= pBridge->m_nQtyJigan; nJijum++)
	{
		for (long nJack = 0; nJack < JACKUPSUMAX; nJack++)
		{
			double dDis = atof(m_Grid.GetTextMatrix(nJijum+1, nJack+1));
			pBridge->m_dJackUpTerm[nJijum][nJack] = dDis;
		}
	}
	SetDataBx();
}

void CJackupPositionDlg::SetDataBx()
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CPlateGirderApp* pGir;
	double dStaJijum = 0;
	CDPoint vAngJijum;
	double dSta = 0;
	CDPoint vAng;
	CPtrList NList;

	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		pGir = pBridge->GetGirder(nG);
		for(long nJijum = 0; nJijum <= pBridge->m_nQtyJigan; nJijum++)
		{
			dStaJijum = pGir->GetStationOnJijum(nJijum);
			vAngJijum = pGir->GetAngleOnJijum(nJijum);
			for(long nJack = 0; nJack < JACKUPSUMAX; nJack++)
			{
				if(pBridge->m_dJackUpTerm[nJijum][nJack] == 0)	continue;
				dSta = pGir->GetStationOffset(dStaJijum, vAngJijum, pBridge->m_dJackUpTerm[nJijum][nJack]);
				vAng = pGir->GetAngleOffset(dStaJijum, vAngJijum, pBridge->m_dJackUpTerm[nJijum][nJack]);
				CPlateBasicIndex* p = pGir->newBx();
				p->SetState(BX_JACKUP,TRUE);
				p->SetStation(dSta);
				p->SetAngle(vAng);
				NList.AddTail(p);
			}
		}
		// set BxList		
		CDWordArray LKExceptionArr;
		LKExceptionArr.Add(CPlateBxObject::LK_JACKUP);
		pGir->PressBxList(NList, BX_JACKUP,&LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);		

		// MakeJackupStiff
		CPlateBxFinder Finder(pGir);
		Finder.GetBxFirst(BX_JACKUP);
		long nCount = Finder.GetCountBx();
		pGir->MakeJackupStiff(nCount);				
		pGir->SetLKPOS(CPlateBxObject::LK_JACKUP);
		pGir->SetBxPosLeftRight(CPlateBxObject::LK_JACKUP);
	}
	SetDataJewon();
}

void CJackupPositionDlg::SetDataJewon()
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		CJackupStiff *pJack;

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JACKUP);
		while (pBx)
		{
			long nJijum = pGir->GetNumberJijumByBx(pBx);
			if (pBx->IsState(BX_JACKUP))
			{
				pJack = pBx->GetJackupStiff();
				pJack->m_vJewon[0].x = atof(m_Grid.GetTextMatrix(nJijum+1, JACKUPSUMAX+1));
				pJack->m_vJewon[0].y = atof(m_Grid.GetTextMatrix(nJijum+1, JACKUPSUMAX+2));				
				pJack->m_vJewon[0].z = pGir->GetHeightGirderByStaAng(pBx->GetStation());				
				pJack->m_vJewon[1].x = atof(m_Grid.GetTextMatrix(nJijum+1, JACKUPSUMAX+3));
				pJack->m_vJewon[1].y = atof(m_Grid.GetTextMatrix(nJijum+1, JACKUPSUMAX+4));				
				pJack->m_vJewon[1].z = pGir->GetHeightGirderByStaAng(pBx->GetStation());				
				pJack->m_dVL = atof(m_Grid.GetTextMatrix(nJijum+1, JACKUPSUMAX+5));
				pJack->m_dVG = atof(m_Grid.GetTextMatrix(nJijum+1, JACKUPSUMAX+6));
			}
			pBx = Finder.GetBxNext();
		}
	}
}

double CJackupPositionDlg::GetStationMatrix(long nRow, long nCol, long nG)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();		
	CPlateGirderApp *pGir = pBridge->GetGirder(nG);

	double lenSum, sta;
	double dist = 0.0;
	if(nRow>1)
		for(int i=2; i<=nRow; i++)
			dist += pGir->GetBxOnJijum(i-1)->GetStation()-pGir->GetBxOnJijum(i-2)->GetStation();//pBridge->m_dLengthJigan[i-2];
	if(nCol!=0)
	{
		lenSum = m_Grid.GetTextMatrixDouble(nRow, nCol);
		sta = pBridge->GetStationByLengthInGrid(TRUE, nG, lenSum, 0, 0) + dist;
	}
	else
		sta = pGir->GetStationOnJijum(nRow-1);
	return sta;
}

void CJackupPositionDlg::OnCellChanged(WPARAM nmgv, LPARAM)
{			
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	
	char ch = m_Grid.GetTextMatrix(nRow,0).GetAt(0);

	if(ch=='A')
	{
		m_ctrlAbutSame.EnableWindow(TRUE);
		m_ctrlPierSame.EnableWindow(FALSE);
	}
	else if(ch=='P')
	{
		m_ctrlAbutSame.EnableWindow(FALSE);
		m_ctrlPierSame.EnableWindow(TRUE);
	}

	if(m_nTab==0)
	{	
		m_pView->GetDomyun()->ClearEtt(FALSE);
		double presta, sta;
		CAPlateDrawPyung Draw(m_pStd->GetDataManage());
		CDomyun *pDom = m_pView->GetDomyun();
		pDom->FixEtt(TRUE);
		for (long nG = 0; nG < pBridge->GetGirdersu(); nG++)   {
			if(nCol>6 || m_Grid.GetTextMatrixDouble(nRow, nCol)==0)
				break;
			presta = GetStationMatrix(nRow, 0, nG);
			sta = GetStationMatrix(nRow, nCol, nG);
			Draw.SetCursorPosition(nG, presta, sta, pDom, TRUE, 100, -1, 2);
		}
		pDom->FixEtt(FALSE);
	}
	else
	{
		DrawInputDomyunView(FALSE);
	}
}

void CJackupPositionDlg::OnAllabutSame() 
{
	// TODO: Add your control notification handler code here
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	double dText;
	long nCurRow = m_Grid.GetFocusCell().row;
	long nSign = -1;
	char chAbut = m_Grid.GetTextMatrix(nCurRow,0).GetAt(1);

	CString str;
	str.Format("  %c번 교대 기준으로 잭업 보강재를 배치합니다. \n    계속하시겠습니까?", chAbut);
	if (IDCANCEL == MessageBox(str, "모든 교대 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	for(long nRow=1; nRow<pBridge->m_nQtyJigan+2; nRow++)
	{
		if (nRow == nCurRow)
			continue;
		if(m_Grid.GetTextMatrix(nRow,0).GetAt(0)=='A')
		{
			for(long nCol=1; nCol<JACKUPSUMAX+5; nCol++)
			{
				if(nCol==JACKUPSUMAX+1 ||nCol==JACKUPSUMAX+2 || nCol==JACKUPSUMAX+3 || nCol==JACKUPSUMAX+4)
					nSign = 1;
				dText = m_Grid.GetTextMatrixDouble(nCurRow, nCol);
				m_Grid.SetTextMatrix(nRow, nCol, "%g", dText*nSign);
			}
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	SetGridTitle();
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CJackupPositionDlg::OnAllpierSame() 
{
	// TODO: Add your control notification handler code here
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	double dText;
	long nCurRow = m_Grid.GetFocusCell().row;
	char chPier = m_Grid.GetTextMatrix(nCurRow,0).GetAt(1);

	CString str;
	str.Format("  %c번 교각 기준으로 잭업 보강재를 배치합니다. \n    계속하시겠습니까?", chPier);
	if (IDCANCEL == MessageBox(str, "모든 교각 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	for(long nRow=1; nRow<pBridge->m_nQtyJigan+2; nRow++)
	{
		if (nRow == nCurRow)
			continue;
		if(m_Grid.GetTextMatrix(nRow,0).GetAt(0)=='P')
		{
			for(long nCol=1; nCol<JACKUPSUMAX+5; nCol++)
			{
				dText = m_Grid.GetTextMatrixDouble(nCurRow, nCol);
				m_Grid.SetTextMatrix(nRow, nCol, "%g", dText);
			}
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	SetGridTitle();
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

BOOL CJackupPositionDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	long nCurRow = m_Grid.GetFocusCell().row;
	char ch = m_Grid.GetTextMatrix(nCurRow,0).GetAt(0);

	if(ch=='A')
		m_ctrlPierSame.EnableWindow(FALSE);
	else if(ch=='P')
		m_ctrlAbutSame.EnableWindow(FALSE);
	
	SetResize(IDC_ALLPIER_SAME,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_ALLABUT_SAME,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_JACKUPSOLEPLATE,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);	
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;
}

long CJackupPositionDlg::GetTabCount()
{ 	
	return m_pStd->GetDlgOfIDD(IDD_GEN_18_FIELDJOINPOSITION)->IsUsedFlag() ? 2 : 1; 
}

void CJackupPositionDlg::OnButtonJackupsoleplate() 
{
	CJackupSolePlateDlg	Dlg(m_pStd);

	Dlg.DoModal();
	DrawInputDomyunView(FALSE);
}

void CJackupPositionDlg::OnButtonRecommend() 
{
	SetDataDefault();
	
}
