// AbutPierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbutPierDlg dialog


CAbutPierDlg::CAbutPierDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CAbutPierDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbutPierDlg)	
	//}}AFX_DATA_INIT	
	m_nCurPos = 0;
	m_nTab = 0;
}


void CAbutPierDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbutPierDlg)	
	DDX_Control(pDX, IDC_TAB_ABUTPIER, m_tabSelect);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbutPierDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CAbutPierDlg)	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ABUTPIER, OnSelchangeTabAbutpier)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/// CAbutPierDlg message handlers
/// 테스트
/////////////////////////////////////////////////////////////////////////////

void CAbutPierDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		
	CDomyun Dom(pDom);
	CAPlateDrawAbut Draw(m_pStd->GetDataManage());
	if(m_nTab==0)
		Draw.DrawAbutOrPier(&Dom,m_nCurPos==0 ? 0 : pBridge->m_nQtyJigan);	
	else
		Draw.DrawAbutOrPier(&Dom, pBridge->m_nTypeAbutOrPier[STT_POS]==0  ? m_nCurPos+1 : m_nCurPos);

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}
/////////////////////////////////////////////////////////////////////////////
/// 그리드에 제목 및 열과 행수를 정의한다.
/////////////////////////////////////////////////////////////////////////////
void CAbutPierDlg::SetGridTitle()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
//	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	if(m_nTab==0)	
		m_Grid.SetRowCount(pBridge->GetAbutSu()+1);
	else				
		m_Grid.SetRowCount(pBridge->GetPierSu()+1);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	if(m_nTab==0)
	{
		m_Grid.SetColumnCount(23);
		m_Grid.SetColumnWidthAll(50);
		m_Grid.SetColumnWidth(0, 65);
		m_Grid.SetTextMatrix(0,0,"구분");
		m_Grid.SetTextMatrix(0,1,"T1");
		m_Grid.SetTextMatrix(0,2,"T2");
		m_Grid.SetTextMatrix(0,3,"T3");
		m_Grid.SetTextMatrix(0,4,"T4");
		m_Grid.SetTextMatrix(0,5,"H1");
		m_Grid.SetTextMatrix(0,6,"H2");
		m_Grid.SetTextMatrix(0,7,"H3");
		m_Grid.SetTextMatrix(0,8,"H4");
		m_Grid.SetTextMatrix(0,9,"H5");
		m_Grid.SetTextMatrix(0,10,"H6");
		m_Grid.SetTextMatrix(0,11,"H7");
		m_Grid.SetTextMatrix(0,12,"H8");
		m_Grid.SetTextMatrix(0,13,"B1");
		m_Grid.SetTextMatrix(0,14,"B2");
		m_Grid.SetTextMatrix(0,15,"B3");
		m_Grid.SetTextMatrix(0,16,"K1");
		m_Grid.SetTextMatrix(0,17,"K2");
		m_Grid.SetTextMatrix(0,18,"W");
		m_Grid.SetTextMatrix(0,19,"W1");
		m_Grid.SetTextMatrix(0,20,"W2");
		m_Grid.SetTextMatrix(0,21,"WL");
		m_Grid.SetTextMatrix(0,22,"WT");

		for(long nJ=0, nRow=0;nJ<pBridge->m_nQtyJigan+1;nJ++)
		{
			if(m_nTab==0 && pBridge->IsAbutOnJiJum(nJ))
			{
				m_Grid.SetTextMatrix(nRow+1,0,pBridge->m_strJijumName[nJ]);
				nRow++;
			}			
		}	
	}
	else
	{
		m_Grid.SetColumnCount(16);
		m_Grid.SetColumnWidthAll(50);
		m_Grid.SetColumnWidth(0, 65);

		m_Grid.SetTextMatrix(0,0,"구분");
		m_Grid.SetTextMatrix(0,1,"W");
		m_Grid.SetTextMatrix(0,2,"H1");
		m_Grid.SetTextMatrix(0,3,"H2");
		m_Grid.SetTextMatrix(0,4,"H3");
		m_Grid.SetTextMatrix(0,5,"H4");
		m_Grid.SetTextMatrix(0,6,"B1");
		m_Grid.SetTextMatrix(0,7,"B2");
		m_Grid.SetTextMatrix(0,8,"B3");
		m_Grid.SetTextMatrix(0,9,"B4");
		m_Grid.SetTextMatrix(0,10,"D");
		m_Grid.SetTextMatrix(0,11,"D2_P");
		m_Grid.SetTextMatrix(0,12,"D2_N");
		m_Grid.SetTextMatrix(0,13,"K1");
		m_Grid.SetTextMatrix(0,14,"K2");
		m_Grid.SetTextMatrix(0,15,"X");

		m_Grid.SetTextMatrix(1,0,"길이(mm)");

		for(long nJ=0, nRow=0;nJ<pBridge->m_nQtyJigan+1;nJ++)
		{
			if(m_nTab==1 && !pBridge->IsAbutOnJiJum(nJ))
			{
				m_Grid.SetTextMatrix(nRow+1,0,pBridge->m_strJijumName[nJ]);				
				nRow++;
			}
		}	
	}
		
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CAbutPierDlg::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	SetGridTitle();
	
	if(m_nTab==0)
	{	
		for(long nJ=0, nRow=0;nJ<pBridge->m_nQtyJigan+1;nJ++)
		{
			if(pBridge->IsAbutOnJiJum(nJ))
			{		
				CPierInfo *pAbut = &pBridge->m_PierInfo[nJ];
				m_Grid.SetTextMatrix(nRow+1,1,"%g",pAbut->m_daT1);
				m_Grid.SetTextMatrix(nRow+1,2,"%g",pAbut->m_daT2);
				m_Grid.SetTextMatrix(nRow+1,3,"%g",pAbut->m_daT3);
				m_Grid.SetTextMatrix(nRow+1,4,"%g",pAbut->m_daT4);
				m_Grid.SetTextMatrix(nRow+1,5,"%g",pAbut->m_daH1);
				m_Grid.SetTextMatrix(nRow+1,6,"%g",pAbut->m_daH2);
				m_Grid.SetTextMatrix(nRow+1,7,"%g",pAbut->m_daH3);
				m_Grid.SetTextMatrix(nRow+1,8,"%g",pAbut->m_daH4);
				m_Grid.SetTextMatrix(nRow+1,9,"%g",pAbut->m_daH5);
				m_Grid.SetTextMatrix(nRow+1,10,"%g",pAbut->m_daH6); 
				m_Grid.SetTextMatrix(nRow+1,11,"%g",pAbut->m_daH7); 
				m_Grid.SetTextMatrix(nRow+1,12,"%g",pAbut->m_daH8);
				m_Grid.SetTextMatrix(nRow+1,13,"%g",pAbut->m_daB1); 
				m_Grid.SetTextMatrix(nRow+1,14,"%g",pAbut->m_daB2); 
				m_Grid.SetTextMatrix(nRow+1,15,"%g",pAbut->m_daB3); 
				m_Grid.SetTextMatrix(nRow+1,16,"%g",pAbut->m_daK1); 
				m_Grid.SetTextMatrix(nRow+1,17,"%g",pAbut->m_daK2); 
				m_Grid.SetTextMatrix(nRow+1,18,"%g",pAbut->m_daW); 
				m_Grid.SetTextMatrix(nRow+1,19,"%g",pAbut->m_daW1); 
				m_Grid.SetTextMatrix(nRow+1,20,"%g",pAbut->m_daW2); 
				m_Grid.SetTextMatrix(nRow+1,21,"%g",pAbut->m_daWL); 
				m_Grid.SetTextMatrix(nRow+1,22,"%g",pAbut->m_daWT);

				nRow++;
			}
		}
	}
	else
	{
		for(long nJ=0, nRow=0;nJ<pBridge->m_nQtyJigan+1;nJ++)
		{
			if(!pBridge->IsAbutOnJiJum(nJ))
			{		
				CPierInfo *pPier = &pBridge->m_PierInfo[nJ];
				m_Grid.SetTextMatrix(nRow+1,1,"%g",pPier->m_dpW);
				m_Grid.SetTextMatrix(nRow+1,2,"%g",pPier->m_dpH1);
				m_Grid.SetTextMatrix(nRow+1,3,"%g",pPier->m_dpH2);
				m_Grid.SetTextMatrix(nRow+1,4,"%g",pPier->m_dpH3);
				m_Grid.SetTextMatrix(nRow+1,5,"%g",pPier->m_dpH4);
				m_Grid.SetTextMatrix(nRow+1,6,"%g",pPier->m_dpB1);
				m_Grid.SetTextMatrix(nRow+1,7,"%g",pPier->m_dpB2);
				m_Grid.SetTextMatrix(nRow+1,8,"%g",pPier->m_dpB3);
				m_Grid.SetTextMatrix(nRow+1,9,"%g",pPier->m_dpB4);
				m_Grid.SetTextMatrix(nRow+1,10,"%g",pPier->m_dpD);
				m_Grid.SetTextMatrix(nRow+1,11,"%g",pPier->m_dpP);
				m_Grid.SetTextMatrix(nRow+1,12,"%g",pPier->m_dpN);
				m_Grid.SetTextMatrix(nRow+1,13,"%g",pPier->m_dpK1); 
				m_Grid.SetTextMatrix(nRow+1,14,"%g",pPier->m_dpK2); 
				m_Grid.SetTextMatrix(nRow+1,15,"%g",pPier->m_dpR);
				nRow++;
			}
		}
	}
}

void CAbutPierDlg::SetDataDefault()
{		
	CPlateBridgeApp	*pBridge	= m_pStd->GetBridge();

	for(long nJ=0; nJ<pBridge->m_nQtyJigan+1; nJ++)
	{
		CPierInfo * pInfo = &pBridge->m_PierInfo[nJ];
		if(pBridge->IsAbutOnJiJum(nJ))
		{
			pInfo->SetAbut(TRUE);
			pInfo->m_daT1 = 300;
			pInfo->m_daT2 = 500;
			pInfo->m_daT3 = 250;
			pInfo->m_daT4 = 1450;
			pInfo->m_daH1 = 400;
			pInfo->m_daH2 = 230;
			pInfo->m_daH3 = 2000;
			pInfo->m_daH4 = 2500;
			pInfo->m_daH5 = 1100;
			pInfo->m_daH6 = 880;
			pInfo->m_daH7 = 2700;
			pInfo->m_daH8 = 800;
			pInfo->m_daB1 = 1600;
			pInfo->m_daB2 = 1450;
			pInfo->m_daB3 = 1000;
			pInfo->m_daK1 = 100;
			pInfo->m_daK2 = 100;
			pInfo->m_daW  = 4950;
			pInfo->m_daW1 = 1500;
			pInfo->m_daW2 = 3150;
			pInfo->m_daWL = pBridge->m_dBridgeWidth;			
			pInfo->m_daWT = 500;
		}
		else
		{
			pInfo->SetAbut(FALSE);
			pInfo->m_dpW = pBridge->m_dBridgeWidth;
			pInfo->m_dpR = 0;				
			pInfo->m_dpP = 1250;	
			pInfo->m_dpN = 1250;
			pInfo->m_dpH1 = 1100;	
			pInfo->m_dpH2 = 900;	
			pInfo->m_dpH3 = 10000;	
			pInfo->m_dpB1 = 2750;	
			pInfo->m_dpB2 = 2750;	
			pInfo->m_dpB3 = 2750;	
			pInfo->m_dpB4 = 2750;	
			pInfo->m_dpD  = 2500;	
			pInfo->m_dpK1 = 100;	
			pInfo->m_dpK2 = 100;		
		}
	}
}

void CAbutPierDlg::SetDataSave()
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if(m_nTab==0)
	{
		for(long nJ=0, nRow=0;nJ<pBridge->m_nQtyJigan+1;nJ++)
		{
			CPierInfo * pInfo = &pBridge->m_PierInfo[nJ];
			if(pBridge->IsAbutOnJiJum(nJ))
			{
				pInfo->SetAbut(TRUE);
				pInfo->m_daT1 = m_Grid.GetTextMatrixDouble(nRow+1,1);
				pInfo->m_daT2 = m_Grid.GetTextMatrixDouble(nRow+1,2);
				pInfo->m_daT3 = m_Grid.GetTextMatrixDouble(nRow+1,3);
				pInfo->m_daT4 = m_Grid.GetTextMatrixDouble(nRow+1,4);
				pInfo->m_daH1 = m_Grid.GetTextMatrixDouble(nRow+1,5);
				pInfo->m_daH2 = m_Grid.GetTextMatrixDouble(nRow+1,6);
				pInfo->m_daH3 = m_Grid.GetTextMatrixDouble(nRow+1,7);
				pInfo->m_daH4 = m_Grid.GetTextMatrixDouble(nRow+1,8);
				pInfo->m_daH5 = m_Grid.GetTextMatrixDouble(nRow+1,9);
				pInfo->m_daH6 = m_Grid.GetTextMatrixDouble(nRow+1,10);
				pInfo->m_daH7 = m_Grid.GetTextMatrixDouble(nRow+1,11);
				pInfo->m_daH8 = m_Grid.GetTextMatrixDouble(nRow+1,12);
				pInfo->m_daB1 = m_Grid.GetTextMatrixDouble(nRow+1,13);
				pInfo->m_daB2 = m_Grid.GetTextMatrixDouble(nRow+1,14);
				pInfo->m_daB3 = m_Grid.GetTextMatrixDouble(nRow+1,15);
				pInfo->m_daK1 = m_Grid.GetTextMatrixDouble(nRow+1,16);
				pInfo->m_daK2 = m_Grid.GetTextMatrixDouble(nRow+1,17);
				pInfo->m_daW  = m_Grid.GetTextMatrixDouble(nRow+1,18);
				pInfo->m_daW1 = m_Grid.GetTextMatrixDouble(nRow+1,19);
				pInfo->m_daW2 = m_Grid.GetTextMatrixDouble(nRow+1,20);
				pInfo->m_daWL = m_Grid.GetTextMatrixDouble(nRow+1,21);			
				pInfo->m_daWT = m_Grid.GetTextMatrixDouble(nRow+1,22);
		
				nRow++;
			}		
		}
	}
	else
	{
		for(long nJ=0, nRow=0;nJ<pBridge->m_nQtyJigan+1;nJ++)
		{
			CPierInfo * pInfo = &pBridge->m_PierInfo[nJ];
			if(!pBridge->IsAbutOnJiJum(nJ))
			{
				pInfo->SetAbut(FALSE);
				pInfo->m_dpW = m_Grid.GetTextMatrixDouble(nRow+1,1);
				pInfo->m_dpH1 = m_Grid.GetTextMatrixDouble(nRow+1,2);
				pInfo->m_dpH2 = m_Grid.GetTextMatrixDouble(nRow+1,3);
				pInfo->m_dpH3 = m_Grid.GetTextMatrixDouble(nRow+1,4);
				pInfo->m_dpH4 = m_Grid.GetTextMatrixDouble(nRow+1,5);
				pInfo->m_dpB1 = m_Grid.GetTextMatrixDouble(nRow+1,6);
				pInfo->m_dpB2 = m_Grid.GetTextMatrixDouble(nRow+1,7);
				pInfo->m_dpB3 = m_Grid.GetTextMatrixDouble(nRow+1,8);
				pInfo->m_dpB4 = m_Grid.GetTextMatrixDouble(nRow+1,9);
				pInfo->m_dpD = m_Grid.GetTextMatrixDouble(nRow+1,10);
				pInfo->m_dpP = m_Grid.GetTextMatrixDouble(nRow+1,11);
				pInfo->m_dpN = m_Grid.GetTextMatrixDouble(nRow+1,12);
				pInfo->m_dpK1 = m_Grid.GetTextMatrixDouble(nRow+1,13);	
				pInfo->m_dpK2 = m_Grid.GetTextMatrixDouble(nRow+1,14);	
				pInfo->m_dpR = m_Grid.GetTextMatrixDouble(nRow+1,15);
				nRow++;
			}				
		}
	}	
}

void CAbutPierDlg::OnSelchangeTabAbutpier(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();
	m_nTab = m_tabSelect.GetCurSel();
	SetDataInit();
	DrawInputDomyunView();
	m_nCurPos = 0;	
			
	m_Grid.SetFocus();

	CGeneralBaseDlg::OnCursorChange(0, 0);
	OnCursorChange(m_Grid.GetFocusCell().row,m_Grid.GetFocusCell().col);
		
	*pResult = 0;
}

void CAbutPierDlg::OnPreInitDialog()
{
	m_tabSelect.InsertItem(0, "교대");
	m_tabSelect.InsertItem(1, "교각");
	m_tabSelect.SetCurSel(m_nTab);	
}

void CAbutPierDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);
	
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CAbutPierDlg::OnCellChangedRow(NMHDR* nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nRow = GridView->iRow;
	
	m_nCurPos = nRow - m_Grid.GetFixedRowCount();
	DrawInputDomyunView(TRUE);
}

void CAbutPierDlg::OnButtonRecommend() 
{
	CPlateBridgeApp *pBridge	= m_pStd->GetBridge();

	if(m_nTab==0)
	{
		for(long nJ=0; nJ<pBridge->m_nQtyJigan+1; nJ++)
		{
			CPierInfo * pInfo = &pBridge->m_PierInfo[nJ];
			if(pBridge->IsAbutOnJiJum(nJ))
			{
				pInfo->SetAbut(TRUE);
				pInfo->m_daT1 = 300;
				pInfo->m_daT2 = 500;
				pInfo->m_daT3 = 250;
				pInfo->m_daT4 = 1450;
				pInfo->m_daH1 = 400;
				pInfo->m_daH2 = 230;
				pInfo->m_daH3 = 2000;
				pInfo->m_daH4 = 2500;
				pInfo->m_daH5 = 1100;
				pInfo->m_daH6 = 880;
				pInfo->m_daH7 = 2700;
				pInfo->m_daH8 = 800;
				pInfo->m_daB1 = 1600;
				pInfo->m_daB2 = 1450;
				pInfo->m_daB3 = 1000;
				pInfo->m_daK1 = 100;
				pInfo->m_daK2 = 100;
				pInfo->m_daW  = 4950;
				pInfo->m_daW1 = 1500;
				pInfo->m_daW2 = 3150;
				pInfo->m_daWL = pBridge->m_dBridgeWidth;			
				pInfo->m_daWT = 500;
			}		
		}
	}
	else
	{
		for(long nJ=0; nJ<pBridge->m_nQtyJigan+1; nJ++)
		{
			CPierInfo * pInfo = &pBridge->m_PierInfo[nJ];
			if(!pBridge->IsAbutOnJiJum(nJ))
			{
				pInfo->SetAbut(FALSE);
				pInfo->m_dpW = pBridge->m_dBridgeWidth;
				pInfo->m_dpR = 0;				
				pInfo->m_dpP = 1250;	
				pInfo->m_dpN = 1250;
				pInfo->m_dpH1 = 1100;	
				pInfo->m_dpH2 = 900;	
				pInfo->m_dpH3 = 10000;	
				pInfo->m_dpB1 = 2750;	
				pInfo->m_dpB2 = 2750;	
				pInfo->m_dpB3 = 2750;	
				pInfo->m_dpB4 = 2750;	
				pInfo->m_dpD  = 2500;	
				pInfo->m_dpK1 = 100;	
				pInfo->m_dpK2 = 100;
			}				
		}
	}
	SetDataInit();
	DrawInputDomyunView(TRUE);
}
