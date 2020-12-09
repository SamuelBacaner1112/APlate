// JijumStiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "JijumStiffDlg.h"

#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJijumStiffDlg dialog


CJijumStiffDlg::CJijumStiffDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CJijumStiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJijumStiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CJijumStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJijumStiffDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, ID_BUTTON_ALLSAMEAPPLY, m_btnAllSame);
}


BEGIN_MESSAGE_MAP(CJijumStiffDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CJijumStiffDlg)
	ON_BN_CLICKED(ID_BUTTON_ALLSAMEAPPLY, OnButtonAllsameapply)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJijumStiffDlg message handlers

void CJijumStiffDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	Draw.DrawPyung(&Dom, 4);
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CJijumStiffDlg::SetGridTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nJigan  = pDB->m_nQtyJigan;
	long nJiJum = JACKUPSUMAX;
	BOOL bInOut = pDB->GetQtyGirder() > 2 ? TRUE : FALSE;
	CString	szText	= _T("");

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nJigan+2);
	m_Grid.SetColumnCount(nJiJum+4+(bInOut?2:0));

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 80);
	m_Grid.SetColumnWidth(JACKUPSUMAX+2, 100);
	m_Grid.SetColumnWidth(JACKUPSUMAX+3, 100);
	if(bInOut)
	{
		m_Grid.SetColumnWidth(JACKUPSUMAX+4, 100);
		m_Grid.SetColumnWidth(JACKUPSUMAX+5, 100);
	}

	CString str;
	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetTextMatrix(0,1,"열수");
	for (long nCol = 2; nCol < nJiJum+2; nCol++)
	{
		str.Format("J %d", nCol-1);
		m_Grid.SetTextMatrix(0, nCol, str);
	}
	m_Grid.SetTextMatrix(0, nJiJum+2, "Wo (외측거더)");
	m_Grid.SetTextMatrix(0, nJiJum+3, "To (외측거더)");
	if(bInOut)
	{
		m_Grid.SetTextMatrix(0, nJiJum+4, "Wi (내측거더)");
		m_Grid.SetTextMatrix(0, nJiJum+5, "Ti (내측거더)");
	}

	long nRow = 1;
	for(long nJijum = 0; nJijum < nJigan+1; nJijum++)
		m_Grid.SetTextMatrix(nRow++, 0, pDB->m_strJijumName[nJijum]);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJijumStiffDlg::SetDataInit()
{
	SetGridTitle();

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pDB->GetGirder(0);
	BOOL bInOut = pDB->GetQtyGirder() > 2 ? TRUE : FALSE;
	
	for(long nJijum = 0; nJijum <= pDB->m_nQtyJigan; nJijum++)
	{
		long nQtyJack = pDB->GetQtyJijumStiff(nJijum);
		m_Grid.SetTextMatrix(nJijum+1, 1, "%g", (double)nQtyJack);
		for (long nJack = 0; nJack < JACKUPSUMAX; nJack++)
		{
			m_Grid.SetTextMatrix(nJijum+1, nJack+2, "%.lf", pDB->m_dJiJumTerm[nJijum][nJack]);
			if(nJack > nQtyJack-1)	m_Grid.SetItemState(nJijum+1, nJack+2, GVIS_READONLY);
			else					m_Grid.SetItemState(nJijum+1, nJack+2, GVIS_MODIFIED);
		}

		CVStiff *pVStiff = pGir->GetJijumVStiffByJijum(nJijum, 0);

		if(pVStiff == NULL)	
		{			
			for(long nCol=JACKUPSUMAX+2; nCol<m_Grid.GetColumnCount(); nCol++)
			{
				m_Grid.SetTextMatrix(nJijum+1, nCol, "0");   
				m_Grid.SetItemState(nJijum+1, nCol, GVIS_READONLY);
			}
			continue;
		}
		double dOutW = pVStiff->m_dWidth[0];
		double dOutT = pVStiff->m_dThick[0];

		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+2, "%.lf", dOutW);
		m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+3, "%.lf", dOutT);
		m_Grid.SetItemState(nJijum+1, JACKUPSUMAX+2, GVIS_MODIFIED);
		m_Grid.SetItemState(nJijum+1, JACKUPSUMAX+3, GVIS_MODIFIED);

		if(bInOut)
		{
			double dInW = pDB->GetGirder(1)->GetJijumVStiffByJijum(nJijum, 0)->m_dWidth[0];
			double dInT = pDB->GetGirder(1)->GetJijumVStiffByJijum(nJijum, 0)->m_dThick[0];

			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+4, "%.lf", dInW);
			m_Grid.SetTextMatrix(nJijum+1, JACKUPSUMAX+5, "%.lf", dInT);			
			m_Grid.SetItemState(nJijum+1, JACKUPSUMAX+4, GVIS_MODIFIED);
			m_Grid.SetItemState(nJijum+1, JACKUPSUMAX+5, GVIS_MODIFIED);
		}		
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJijumStiffDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	SetGridTitle();

	for(long nJijum = 0; nJijum <= pDB->m_nQtyJigan; nJijum++)
	{
		long nJack = 1;
		if(nJijum == 0 || nJijum == pDB->m_nQtyJigan)
			pDB->SetQtyJijumStiff(nJijum, 1);
		else
			pDB->SetQtyJijumStiff(nJijum, 3);

		long nQtyStiff = pDB->GetQtyJijumStiff(nJijum);
		m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", (double)nQtyStiff);

		if(nJijum == 0 || nJijum == pDB->m_nQtyJigan)
		{
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 0.0);
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 0.0);
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 0.0);
		}
		else
		{
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", -200.0);
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 0.0);
			m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 200.0);
		
		}
		
		m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 0.0);
		m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 0.0);
		m_Grid.SetTextMatrix(nJijum+1, nJack++, "%.lf", 0.0);
		
		for(long nCol=JACKUPSUMAX+2; nCol<m_Grid.GetColumnCount(); nCol+=2)
		{
			m_Grid.SetTextMatrix(nJijum+1, nCol, "%.lf", 200.0);
			m_Grid.SetTextMatrix(nJijum+1, nCol+1, "%.lf", 20.0);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
	
	SetDataSave();
}

void CJijumStiffDlg::SetDataSave()
{		
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CDoubleArray dDisArr;

	for(long nJijum = 0; nJijum <= pDB->m_nQtyJigan; nJijum++)
	{
		dDisArr.RemoveAll();
		long nQtyJijumStiff = m_Grid.GetTextMatrixLong(nJijum+1, 1);
		pDB->SetQtyJijumStiff(nJijum, nQtyJijumStiff);
		long nJack = 0;
		for (nJack = 0; nJack < nQtyJijumStiff; nJack++)
		{
			double dDis = atof(m_Grid.GetTextMatrix(nJijum+1, nJack+2));
			dDisArr.Add(dDis);
		}
		dDisArr.Sort();
		for(nJack=0; nJack<nQtyJijumStiff; nJack++)
			pDB->m_dJiJumTerm[nJijum][nJack] = dDisArr.GetAt(nJack);
		for(nJack; nJack<JACKUPSUMAX; nJack++)
			pDB->m_dJiJumTerm[nJijum][nJack] = 0;
	}
	
	SetDataBx();
}

void CJijumStiffDlg::SetDataBx()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir	= NULL;

	double dStaJijum = 0;
	double dSta = 0;
	CDPoint vAngJijum;	
	CDPoint vAng;
	CPtrList NList;

	for(long nG = -1; nG < pDB->GetGirdersu(); nG++)
	{
		pGir = pDB->GetGirder(nG);
		for(long nJijum = 0; nJijum <= pDB->m_nQtyJigan; nJijum++)
		{
			dStaJijum = pGir->GetStationOnJijum(nJijum);
			vAngJijum = pGir->GetAngleOnJijum(nJijum);
			long nQtyJijumStiff = m_Grid.GetTextMatrixLong(nJijum+1, 1);
			for(long nStiff = 0; nStiff < nQtyJijumStiff; nStiff++)
			{
				dSta = pGir->GetStationOffset(dStaJijum, vAngJijum, pDB->m_dJiJumTerm[nJijum][nStiff]);
				vAng = pGir->GetAngleOffset(dStaJijum, vAngJijum, pDB->m_dJiJumTerm[nJijum][nStiff]);
				CPlateBasicIndex* p = pGir->newBx();
				p->SetState(BX_JIJUMSTIFF,TRUE);
				p->SetStation(dSta);
				p->SetAngle(vAng);
				NList.AddTail(p);				
			}
		}
		// set BxList
		CDWordArray LKExceptionArr;		
		pGir->PressBxList(NList, BX_JIJUMSTIFF,&LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);

		// MakeJiJumStiff
		CPlateBxFinder Finder(pGir);
		Finder.GetBxFirst(BX_JIJUMSTIFF);
		long nCount = Finder.GetCountBx();
		pGir->MakeJiJumStiff(nCount);
		
		// Set LinkToBx In JACKUP
		pGir->SetLKPOS(CPlateBxObject::LK_JIJUM_VSTIFF);				
	}
	SetDataJewon();
}

void CJijumStiffDlg::SetDataJewon()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	for(long nG = -1; nG < pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp	* pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUMSTIFF);
		while(pBx)
		{
			long	nJijum	= pGir->GetNumberJijumByBx(pBx);
			double	dOutW	= m_Grid.GetTextMatrixDouble(nJijum+1, JACKUPSUMAX+2);
			double	dOutT	= m_Grid.GetTextMatrixDouble(nJijum+1, JACKUPSUMAX+3);
			double	dInW	= m_Grid.GetTextMatrixDouble(nJijum+1, m_Grid.GetColumnCount()-2);
			double	dInT	= m_Grid.GetTextMatrixDouble(nJijum+1, m_Grid.GetColumnCount()-1);

			CVStiff *pVStiff = pBx->GetJijumStiff();
			pVStiff->m_pSection = pBx->GetSection();

			if(nG < 1 || nG==pDB->GetQtyGirder()-1)
			{
				pVStiff->m_dWidth[0] = dOutW;
				pVStiff->m_dThick[0] = dOutT;
				pVStiff->m_dWidth[1] = dOutW;
				pVStiff->m_dThick[1] = dOutT;
			}
			else
			{
				pVStiff->m_dWidth[0] = dInW;
				pVStiff->m_dThick[0] = dInT;
				pVStiff->m_dWidth[1] = dInW;
				pVStiff->m_dThick[1] = dInT;
			}

			if(pDB->GetQtyGirder()<3)
			{
				pVStiff->m_dWidth[0] = dOutW;
				pVStiff->m_dThick[0] = dOutT;
				pVStiff->m_dWidth[1] = dOutW;
				pVStiff->m_dThick[1] = dOutT;
			}
	
			pVStiff->m_dH[LEFT][UP] = 0;
			pVStiff->m_dH[RIGHT][UP] = 0;
			pVStiff->m_dH[LEFT][DOWN] = 0;
			pVStiff->m_dH[RIGHT][DOWN] = 0;
		
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}
}

void CJijumStiffDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{			
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
		
	//DrawInputDomyunView(FALSE);
	m_pView->GetDomyun()->ClearEtt(FALSE);
	double presta, sta;
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->FixEtt(TRUE);
	for (long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		if(nCol>6 || m_Grid.GetTextMatrixDouble(nRow, nCol)==0)
			break;
		
		presta = GetStationMatrix(nRow, 0, nG);
		sta = GetStationMatrix(nRow, nCol, nG);
		Draw.SetCursorPosition(nG, presta, sta, pDom, TRUE, 40, -1, 0);
	}
	pDom->FixEtt(FALSE);
}

void CJijumStiffDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nCurRow = m_Grid.GetFocusCell().row;
	long nCurCol = m_Grid.GetFocusCell().col;

	if(nCurCol==1 && nCurRow!=0)
	{
		long nJack = m_Grid.GetTextMatrixLong(nCurRow, 1);
		long nOldJack = pDB->GetQtyJijumStiff(nCurRow-1);

		if(nJack<0)			nJack = 0;
		else if(nJack>6)	nJack = 6;		
		m_Grid.SetTextMatrix(nCurRow, 1, "%.lf", (double)nJack);
		long nCol = 0;
		for(nCol=2; nCol<JACKUPSUMAX+2; nCol++)
		{
			if(nCol<nJack+2)
				m_Grid.SetItemState(nCurRow, nCol, GVIS_MODIFIED);
			else
			{
				m_Grid.SetItemState(nCurRow, nCol, GVIS_READONLY);
				m_Grid.SetTextMatrix(nCurRow, nCol, "0");
			}
		}

		long nState = nJack==0 ? GVIS_READONLY : GVIS_MODIFIED;
		for(nCol=JACKUPSUMAX+2; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			m_Grid.SetItemState(nCurRow, nCol, nState);
			if(nJack == 0)
				m_Grid.SetTextMatrix(nCurRow, nCol, "0");
			else if(nOldJack == 0)
			{
				if(nCol%2)
					m_Grid.SetTextMatrix(nCurRow, nCol, "20");
				else
					m_Grid.SetTextMatrix(nCurRow, nCol, "200");
			}
		}
	}
	
	m_Grid.SetRedraw();
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

double CJijumStiffDlg::GetStationMatrix(long nRow, long nCol, long nG)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();		
	CPlateGirderApp *pGir = pDB->GetGirder(nG);

	double	dLenSum	= 0;
	double	dSta	= 0;
	double	dDist	= 0;

	if(nRow>1)
	{
		for(int i=2; i<=nRow; i++)
		{
			CPlateBasicIndex	*pBxCur	= pGir->GetBxOnJijum(i-1);
			CPlateBasicIndex	*pBxPre	= pGir->GetBxOnJijum(i-2);
			dDist += pBxCur->GetStation()-pBxPre->GetStation();
		}
	}
	if(nCol>1)
	{
		dLenSum = m_Grid.GetTextMatrixDouble(nRow, nCol);
		dSta = pDB->GetStationByLengthInGrid(TRUE, nG, dLenSum, 0, 0) + dDist;
	}
	else
		dSta = pGir->GetStationOnJijum(nRow-1);
	return dSta;
}

void CJijumStiffDlg::OnButtonAllsameapply() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nCurRow = m_Grid.GetFocusCell().row;
	double dText = 0;

	if(nCurRow<0) return;
	CString str;
	str.Format("  %s번 기준으로 동일적용을 합니다. \n    계속하시겠습니까?", m_Grid.GetTextMatrix(nCurRow,0));
	if (IDCANCEL == MessageBox(str, "모든지점 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	CDoubleArray dArrPos;
	long nJack = m_Grid.GetTextMatrixLong(nCurRow, 1);
	long nCol = 0;
	for(nCol=2; nCol<nJack+2; nCol++)
	{
		dText = m_Grid.GetTextMatrixDouble(nCurRow, nCol);
		dArrPos.Add(dText);
	}
	dArrPos.Sort();
	for(nCol; nCol<JACKUPSUMAX+6; nCol++)
	{
		dText = m_Grid.GetTextMatrixDouble(nCurRow, nCol);
		if(nCol<JACKUPSUMAX+2)
			dArrPos.Add(0);
		else			
			dArrPos.Add(dText);
	}

	for(long nRow=1; nRow<pDB->m_nQtyJigan+2; nRow++)
	{
		m_Grid.SetTextMatrix(nRow, 1, "%g", (double)nJack);
		for(long nCol=2; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			dText = dArrPos.GetAt(nCol-2);

			if((nCurRow==1 && nRow==pDB->m_nQtyJigan+1 && nCol<nJack+2) ||
				(nCurRow==pDB->m_nQtyJigan+1 && nRow==1 && nCol<nJack+2) ||
				(nCurRow!=1 && nCurRow!=pDB->m_nQtyJigan+1 && nRow==pDB->m_nQtyJigan+1 &&  nCol<nJack+2))
			{
				dText *= -1;
				m_Grid.SetTextMatrix(nRow, nJack+3-nCol, "%g", dText);
			}
			else
				m_Grid.SetTextMatrix(nRow, nCol, "%g", dText);
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
	
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView();
}

BOOL CJijumStiffDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	SetResize(ID_BUTTON_ALLSAMEAPPLY,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	return TRUE;
}
