// JackupSolePlateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "JackupSolePlateDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJackupSolePlateDlg dialog

  
CJackupSolePlateDlg::CJackupSolePlateDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: CDialog(CJackupSolePlateDlg::IDD, pParent)
{
	m_pStd = pStd;
	//{{AFX_DATA_INIT(CJackupSolePlateDlg)
	m_bUseJackupSolePlate = TRUE;
	//}}AFX_DATA_INIT
}

void CJackupSolePlateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJackupSolePlateDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_USE_JACKUP_SOLEPLATE, m_bUseJackupSolePlate);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_JACKUPSOLEPLATE, m_btnDefault);
	DDX_Control(pDX, ID_BUTTON_APPLY, m_btnApply);
}

BEGIN_MESSAGE_MAP(CJackupSolePlateDlg, CDialog)
	//{{AFX_MSG_MAP(CJackupSolePlateDlg)
	ON_BN_CLICKED(IDC_CHECK_USE_JACKUP_SOLEPLATE, OnCheckUseJackupSoleplate)
	ON_BN_CLICKED(IDC_BUTTON_JACKUPSOLEPLATE, OnButtonRecommend)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnDataChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJackupSolePlateDlg message handlers

void CJackupSolePlateDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	/*
	CDomyun *pDom = m_pView->GetDomyun();

	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	Draw.DrawPyung(&Dom, 3);
	*pDom << Dom;	

	CDialog::DrawInputDomyunView(bZoomAll);
*/
}

void CJackupSolePlateDlg::SetGridTitle()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long nJigan  = pBridge->m_nQtyJigan;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount((nJigan+1)*2+1);
	m_Grid.SetColumnCount(9);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(60);

	char *szArr[] = { "구배(%)", "L", "H", "W", "T1", "T", "T2" };
	CString str;
	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetMergeCol(0,0,1);
	for (long nCol = 0; nCol < 7; nCol++) 
		m_Grid.SetTextMatrix(0, nCol+2, szArr[nCol]);
	long nRow = 1;
	for (long nJijum = 0; nJijum < nJigan+1; nJijum++)
	{
		m_Grid.SetTextMatrix(nRow, 0, pBridge->m_strJijumName[nJijum]);
		m_Grid.SetMergeRow(nRow, nRow+1, 0);
		m_Grid.SetTextMatrix(nRow++, 1, "전");		
		m_Grid.SetTextMatrix(nRow++, 1, "후");
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJackupSolePlateDlg::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);
	double m_dVSlop=0, dSta=0, dStaPos=0;

	SetGridTitle();
	m_bUseJackupSolePlate = pBridge->m_bUseJackupSolePlate;
	UpdateData(FALSE);
	if(pBridge->m_bUseJackupSolePlate)
	{
		SetDataNotUse();
		return;
	}

	for(long nJijum = 0; nJijum <= pBridge->m_nQtyJigan; nJijum++)
	{
		for(long i=0; i<2; i++)
		{
			dSta	= pGir->GetStationOnJijum(nJijum);
			dStaPos	= pBridge->m_structJackupJewon[nJijum][i].m_dL;
			
			if(i == 0)	dSta -= dStaPos;
			else		dSta += dStaPos;
			
			m_dVSlop = pGir->GetGiulJongDanOnlyLine(dSta);
			double dT = pBridge->m_structJackupJewon[nJijum][i].m_dT;
			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 2, "%.3f", m_dVSlop*100);
			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 3, "%g", pBridge->m_structJackupJewon[nJijum][i].m_dL);
			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 4, "%g", pBridge->m_structJackupJewon[nJijum][i].m_dH);
			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 5, "%g", pBridge->m_structJackupJewon[nJijum][i].m_dW);
			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 6, "%.3f", dT-pBridge->m_structJackupJewon[nJijum][i].m_dW/2*m_dVSlop);
			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 7, "%g", dT);
			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 8, "%.3f", dT+pBridge->m_structJackupJewon[nJijum][i].m_dW/2*m_dVSlop);

			m_Grid.SetItemState(nJijum * 2 + i+1, 2, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY); 
			m_Grid.SetItemBkColour(nJijum * 2 + i+1, 2, RGBREADONLY);
			m_Grid.SetItemState(nJijum * 2 + i+1, 6, m_Grid.GetItemState(nJijum * 2 + i+1, 6) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nJijum * 2 + i+1, 6, RGBREADONLY);
			m_Grid.SetItemState(nJijum * 2 + i+1, 8, m_Grid.GetItemState(nJijum * 2 + i+1, 8) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nJijum * 2 + i+1, 8, RGBREADONLY);		
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJackupSolePlateDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(-1);

	double dSta=0, Giul=0;
	pBridge->m_bUseJackupSolePlate	= FALSE;

	for(long nJijum = 0; nJijum <= pBridge->m_nQtyJigan; nJijum++)
	{
		dSta = pGir->GetStationOnJijum(nJijum);
		Giul = pGir->GetGiulFlangeUpperLowerCamber(dSta);

		long nNumL = 0, nNumR = 0;
		double dLRight = 0;
		double dLLeft  = 0;
		for(long nJ=0;nJ<JACKUPSUMAX;nJ++)
		{
			double len = pBridge->m_dJackUpTerm[nJijum][nJ];
			if(len==0) continue;
			if(len > 0) 
			{
				dLRight += len;
				nNumR++;
			}
			else		
			{
				dLLeft  += len;
				nNumL++;
			}
			
		}
		if(nNumR>0)		dLRight /= nNumR;
		else dLRight = 0;
		if(nNumL>0) 	dLLeft /= nNumL;
		else dLLeft = 0;

		for(long i=0; i<2; i++)
		{
			pBridge->m_structJackupJewon[nJijum][i].m_dL = i==0 ? -dLLeft : dLRight;
			pBridge->m_structJackupJewon[nJijum][i].m_dW = 500;
			pBridge->m_structJackupJewon[nJijum][i].m_dH = 600;
			pBridge->m_structJackupJewon[nJijum][i].m_dT = 22;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJackupSolePlateDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(-1);

	pBridge->m_bUseJackupSolePlate = m_bUseJackupSolePlate;

	for(long nJijum = 0; nJijum <= pBridge->m_nQtyJigan; nJijum++)
	{
		double dSta = pGir->GetStationOnJijum(nJijum);
		double Giul = pGir->GetGiulFlangeUpperLower(dSta);

		for(long i=0; i<2; i++)
		{			
			pBridge->m_structJackupJewon[nJijum][i].m_dL = m_Grid.GetTextMatrixDouble(nJijum * 2 + i+1, 3);
			pBridge->m_structJackupJewon[nJijum][i].m_dH = m_Grid.GetTextMatrixDouble(nJijum * 2 + i+1, 4);
			pBridge->m_structJackupJewon[nJijum][i].m_dW = m_Grid.GetTextMatrixDouble(nJijum * 2 + i+1, 5);			
			pBridge->m_structJackupJewon[nJijum][i].m_dT = m_Grid.GetTextMatrixDouble(nJijum * 2 + i+1, 7);			
		}		
	}
}

double CJackupSolePlateDlg::GetStationMatrix(long nRow, long nCol, long nG)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();		
	CPlateGirderApp *pGir = pBridge->GetGirder(nG);

	double lenSum, sta;
	if(nCol!=0)
	{
		if(nRow%2==1)
			lenSum = -m_Grid.GetTextMatrixDouble(nRow, nCol);
		else
			lenSum = m_Grid.GetTextMatrixDouble(nRow, nCol);
		sta = pGir->GetStationOnJijum((nRow-1)/2) + lenSum;
	}
	else
		sta = pGir->GetStationOnJijum((nRow-1)/2);
	return sta;
}

void CJackupSolePlateDlg::OnCellChanged(NMHDR* pNMHDR, LRESULT* pResult)
{			
//	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();
//
//	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)pNMHDR;	
//	int nRow = GridView->iRow;
//    int nCol = GridView->iColumn;
	
	//DrawInputDomyunView(FALSE);
/*
	m_pView->GetDomyun()->ClearEtt(FALSE);
	double presta, sta;
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->FixEtt(TRUE);
	for (long nG = 0; nG < pBridge->GetGirdersu(); nG++) 
	{
		if(nCol!=3 || m_Grid.GetTextMatrixDouble(nRow, nCol)==0)
			break;
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		presta = GetStationMatrix(nRow, 0, nG);
		sta = GetStationMatrix(nRow, nCol, nG);
		Draw.SetCursorPosition(nG, presta, sta, pDom, TRUE, 100, -1, 2);
	}
	pDom->FixEtt(FALSE);
	*/
}


void CJackupSolePlateDlg::OnDataChanged(NMHDR* pNMHDR, LRESULT* pResult)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)pNMHDR;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	
	if(nCol==7)
	{
		double Giul = m_Grid.GetTextMatrixDouble(nRow, 2) / 100;
		double dDefaultW = m_Grid.GetTextMatrixDouble(nRow, 5);
		m_Grid.SetTextMatrix(nRow, 6, "%.3f", m_Grid.GetTextMatrixDouble(nRow, nCol)-dDefaultW/2*Giul);		
		m_Grid.SetTextMatrix(nRow, 8, "%.3f", m_Grid.GetTextMatrixDouble(nRow, nCol)+dDefaultW/2*Giul);
		m_Grid.SetRedraw(TRUE,TRUE);
	}	
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CJackupSolePlateDlg::OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult)
{
//	CDialog::OnCellChangedData(nmgv, 0);

	DrawInputDomyunView(TRUE);
}

void CJackupSolePlateDlg::OnCheckUseJackupSoleplate()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	UpdateData(TRUE);

	pBridge->m_bUseJackupSolePlate = m_bUseJackupSolePlate;

	if(m_bUseJackupSolePlate)
	{
		SetDataNotUse();
		SetDataSave();
	}
	else
	{
		SetDataDefault();
		SetDataInit();
		SetDataSave();
	}
	UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE,TRUE);
	DrawInputDomyunView(FALSE);
}

void CJackupSolePlateDlg::SetDataNotUse()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(-1);

	for(long nJijum = 0; nJijum <= pBridge->m_nQtyJigan; nJijum++)
	{
		for(long nJ=0;nJ<JACKUPSUMAX;nJ++)
		{
			double dLen = pBridge->m_dJackUpTerm[nJijum][nJ];
			if(dLen==0) continue;			
		}

		for(long i=0; i<2; i++)
		{
			pBridge->m_structJackupJewon[nJijum][i].m_dL = 0;
			pBridge->m_structJackupJewon[nJijum][i].m_dW = 0;
			pBridge->m_structJackupJewon[nJijum][i].m_dH = 0;
			pBridge->m_structJackupJewon[nJijum][i].m_dT = 0;

			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 2, "-");
			m_Grid.SetItemState(nJijum * 2 + i+1, 2, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY); 

			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 3, "-");
			m_Grid.SetItemState(nJijum * 2 + i+1, 3, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY); 

			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 4, "-");
			m_Grid.SetItemState(nJijum * 2 + i+1, 2, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY); 

			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 5, "-");
			m_Grid.SetItemState(nJijum * 2 + i+1, 2, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY); 

			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 6, "-");
			m_Grid.SetItemState(nJijum * 2 + i+1, 2, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY); 

			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 7, "-");
			m_Grid.SetItemState(nJijum * 2 + i+1, 2, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY); 

			m_Grid.SetTextMatrix(nJijum * 2 + i+1, 8, "-");
			m_Grid.SetItemState(nJijum * 2 + i+1, 2, m_Grid.GetItemState(nJijum * 2 + i+1, 2) | GVIS_READONLY);
		}
	}


}

BOOL CJackupSolePlateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetDataInit();
	SetDataSave();

	return TRUE;
}

void CJackupSolePlateDlg::OnButtonRecommend()
{
	if(!m_bUseJackupSolePlate)
	{
		SetDataDefault();
		SetDataInit();
		SetDataSave();
	}
}