// SlabStructureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "SlabStructureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlabStructureDlg dialog


CSlabStructureDlg::CSlabStructureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSlabStructureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSlabStructureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSlabStructureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSlabStructureDlg)
	DDX_DomControl(pDX, IDC_DOMYUN_VIEW, m_DomView);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STRUCTURE_DEFAULT, m_btnBaseType);
	DDX_Control(pDX, IDC_GUARDWALLBANG_RECOMMEND, m_btnRectType);
	DDX_Control(pDX, IDC_BUTTON_RECOMMEND_MEDIBARR, m_btnCenterType);
	DDX_Control(pDX, IDC_BUTTON_NANGAN, m_btnSideType);
	DDX_Control(pDX, IDC_BUTTON_SYMMETRY, m_btnSymmetriType);
	DDX_Control(pDX, IDC_BUTTON_RECOMMEND_MEDIBARR_HALF, m_btnCenterHalfType);
	DDX_Control(pDX, IDOK, m_btnOK);
}

BEGIN_MESSAGE_MAP(CSlabStructureDlg, CDialog)
	//{{AFX_MSG_MAP(CSlabStructureDlg)
	ON_BN_CLICKED(IDC_STRUCTURE_DEFAULT, OnStructureDefault)
	ON_BN_CLICKED(IDC_GUARDWALLBANG_RECOMMEND, OnGuardwallbangRecommend)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND_MEDIBARR_HALF, OnButtonRecommendMedibarrHalf)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND_MEDIBARR, OnButtonRecommendMedibarr)
	ON_BN_CLICKED(IDC_BUTTON_NANGAN, OnButtonNangan)
	ON_BN_CLICKED(IDC_BUTTON_SYMMETRY, OnButtonSymmetry)
	ON_BN_CLICKED(IDC_BUTTON_GUARDWALL, OnButtonGuardwall)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlabStructureDlg message handlers
void CSlabStructureDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pDB;
	for( long nGuard = 0; nGuard < pDB->GetGuardWallSu(); nGuard++ )
	{			
		CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);
		long nHDan = pDB->GetNumHDanByGuardFence(pGuard);

		if(pDB->GetWallGuardTypeHDan(nHDan) == HT_GUARDFENCE )
			SetDataDefault1(nGuard);
		else if(pDB->GetWallGuardTypeHDan(nHDan) == HT_CENTERGUARDFENCE)
			SetDataDefault3(nGuard);		
	}
}

//방호벽 권고안...
void CSlabStructureDlg::SetDataDefault1(long nGuard)
{
	CPlateBridgeApp *pDB = m_pDB;
	CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);

	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	double lenHDan = pDB->GetLengthHDanDom(pBx, nHDan);
	long bLeft = (nHDan < (pDB->GetQtyHDanNode()/2)) ? TRUE : FALSE;

	pGuard->SetDataDefault1(pGuard,nGuard,bLeft);
}

void CSlabStructureDlg::SetDataDefault2(long nGuard)
// 중분대 (1/2) 권고안
{
	CPlateBridgeApp *pDB = m_pDB;
	CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);
	long bLeft = (nHDan < (pDB->GetQtyHDanNode()/2)) ? TRUE : FALSE;
	
	double width = 320;//280;	// 280을 기준으로	
	double R_W1 = 125/width;
	double R_W2 = 85/width;
	double R_W3 = 110/width;
	double R_W4 = 85/width;
	double R_W5 = 125/width;

	long count = 0;
	double LEN = 0;
	for(long n = 0;n < pDB->GetQtyHDanNode(); n++)
	{
		if(pDB->IsTypeGuardFenceDom(n)) //방호벽
		{
			if( count == nGuard )
			{
				LEN = pDB->GetLengthHDanDom(pBx, n);
				break;
			}
			else count++;
		}
	}

	double W = 0;

	pGuard->m_D1 = 0;

	if(bLeft)
	{				
		pGuard->m_W1 = 0;
		pGuard->m_W2 = 0;
		W	 = Round(R_W4*LEN, 0);
		if((long)W%10 != 5) W = W - (long)W%10;
		pGuard->m_W4 = W;
		W	 = Round(R_W5*LEN, 0);
		if((long)W%10 != 5) W = W - (long)W%10;
		pGuard->m_W5 = W;
		pGuard->m_W3 = LEN - (pGuard-> m_W4+pGuard->m_W5);
	}
	else
	{
		pGuard->m_W4 = 0;
		pGuard->m_W5 = 0;
		W	 = Round(R_W4*LEN, 0);
		if((long)W%10 != 5) W = W - (long)W%10;
		pGuard->m_W2 = W;
		W	 = Round(R_W5*LEN, 0);
		if((long)W%10 != 5) W = W - (long)W%10;
		pGuard->m_W1 = W;
		pGuard->m_W3 = LEN - (pGuard-> m_W2+pGuard->m_W1);

	}

	pGuard->m_H1 = 155;
	pGuard->m_H2 = 175;
	pGuard->m_H3 = 1020;	
}

// 중분대 권고안
void CSlabStructureDlg::SetDataDefault3(long nGuard)
{
	CPlateBridgeApp *pDB = m_pDB;
	CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);

	double width = 610;//570;	// 570을 기준으로
	double R_W1 = 125/width;
	double R_W2 = 105/width;
	double R_W3 = 150/width;
	double R_W4 = 105/width;
	double R_W5 = 125/width;

	double LEN = 0;

	LEN = pDB->GetLengthHDanDom(pBx, nHDan);
	double W = 0;
	pGuard->m_D1 = 0;
	W	 = Round(R_W1*LEN, 0);
	if((long)W%10 != 5) W = W - (long)W%10;
	pGuard->m_W1 = W;
	W	 = Round(R_W2*LEN, 0);
	if((long)W%10 != 5) W = W - (long)W%10;
	pGuard->m_W2 = W;
	W	 = Round(R_W4*LEN, 0);
	if((long)W%10 != 5) W = W - (long)W%10;
	pGuard->m_W4 = W;
	W	 = Round(R_W5*LEN, 0);
	if((long)W%10 != 5) W = W - (long)W%10;
	pGuard->m_W5 = W;
	pGuard->m_W3 = LEN - ( pGuard->m_W1+pGuard->m_W2+pGuard->m_W4+pGuard->m_W5);
	
	pGuard->m_H1 = 155;
	pGuard->m_H2 = 175;
// 6월 10일 발표용 TU 권고안 (by. 이종원부장님)
	pGuard->m_H3 = 600;
}

// 난간 타입
void CSlabStructureDlg::SetDataDefault4(long nGuard)
{
	CPlateBridgeApp *pDB = m_pDB;
	CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);
	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	double lenHDan = pDB->GetLengthHDanDom(pBx, nHDan);
	long bLeft = (nHDan < (pDB->GetQtyHDanNode()/2)) ? TRUE : FALSE;

	double width = 450;
	double R_W3 = 230/width;
	double R_W4 = 70/width;
	double R_W5 = 120/width;
	double Height = 1000;

	if(bLeft)
	{
		pGuard->m_D1 = 0;
		pGuard->m_W1 = 0;
		pGuard->m_W2 = 0;
		pGuard->m_W3 = lenHDan-50;
		pGuard->m_W4 = 0;
		pGuard->m_W5 = 50;
	}
	else
	{
		pGuard->m_D1 = 0;
		pGuard->m_W1 = 50;
		pGuard->m_W2 = 0;
		pGuard->m_W3 = Round(R_W3*lenHDan, 0) + Round(R_W4*lenHDan, 0) + Round(R_W5*lenHDan, 0);
		pGuard->m_W4 = 0;
		pGuard->m_W5 = 0;
	}

	pGuard->m_H1 = 0;
	pGuard->m_H2 = 400;
	pGuard->m_H3 = 0;
}

void CSlabStructureDlg::SetDataDefault5(long nGuard)
{
	CPlateBridgeApp *pDB = m_pDB;
	CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);
	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);
	
	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	double lenHDan = pDB->GetLengthHDanDom(pBx, nHDan);
	
	pGuard->m_D1 = 0;
	pGuard->m_W1 = 0;
	pGuard->m_W2 = 0;
	pGuard->m_W3 = lenHDan;
	pGuard->m_W4 = 0;
	pGuard->m_W5 = 0;
	
	pGuard->m_H1 = 350;
	pGuard->m_H2 = 0;
	pGuard->m_H3 = 300;
}

void CSlabStructureDlg::SetDataDefault6(long nGuard)
{
	CPlateBxFinder		Finder(m_pDB->GetGirder(-1));
	CCentSeparation		*pGuard	= m_pDB->GetGuardFence(nGuard);	
	CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
	long	nHDan	= m_pDB->GetNumHDanByGuardFence(pGuard);	
	double	lenHDan	= m_pDB->GetLengthHDanDom(pBx, nHDan);
	BOOL	bLeft	= (nHDan < (m_pDB->GetQtyHDanAct(pBx->GetStation())/2)) ? TRUE : FALSE;
	
	if( pGuard->m_nSepType == 0 || pGuard->m_nSepType == 1 )
	{
		pGuard->m_D1 = bLeft ? 30	: 0;		
		pGuard->m_W1 = bLeft ? 0	: 120;
		pGuard->m_W2 = bLeft ? 0	: 70;
		pGuard->m_W3 = 230;
		pGuard->m_W4 = bLeft ? 70	: 0;
		pGuard->m_W5 = bLeft ? 120	: 0;
		pGuard->m_H1 = 205;
		pGuard->m_H2 = 175;
		pGuard->m_H3 = 700;
	}
	else if(pGuard->m_nSepType == 2 || pGuard->m_nSepType == 3)
	{
		pGuard->m_W1 = 300;
		pGuard->m_H1 = 300;
	}
	else
	{
		pGuard->m_W1 = 230;
		pGuard->m_W2 = 110;
		pGuard->m_W3 = 370;
		pGuard->m_W4 = 110;
		pGuard->m_W5 = 230;
		pGuard->m_H1 = 115;
		pGuard->m_H2 = 175;
		pGuard->m_H3 = 560;
	}
}

void CSlabStructureDlg::SetDataSave()
{
	for(long nGuard = 0; nGuard < m_pDB->GetGuardFencesu(); nGuard++)
	{
		CCentSeparation *pCentSeparation = m_pDB->GetGuardFence(nGuard);

		long nCols = 1;				
		pCentSeparation->m_D1 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_W1 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_W2 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_W3 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_W4 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_W5 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_H1 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_H2 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);
		pCentSeparation->m_H3 = m_Grid.GetTextMatrixDouble(nGuard+1,nCols++);						
	}
}

void CSlabStructureDlg::SetGridTitle()
{
	long nRows = m_pDB->GetGuardFencesu() + 1;

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetColumnCount(10);
	m_Grid.SetRowCount(nRows);	
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);		
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0,110);
	m_Grid.SetRowHeightAll(20);

	for(long i=1; i<nRows; i++ )
	{
		CCentSeparation *pGuard = m_pDB->GetGuardFence(i-1);
		m_Grid.SetTextMatrix(i,0, pGuard->m_strName);
	}

	// draw cols
	long nCols = 0;
	m_Grid.SetTextMatrix(0, nCols++,"구 분");
	m_Grid.SetTextMatrix(0, nCols++,"D1");
	m_Grid.SetTextMatrix(0, nCols++,"W1");
	m_Grid.SetTextMatrix(0, nCols++,"W2");
	m_Grid.SetTextMatrix(0, nCols++,"W3");
	m_Grid.SetTextMatrix(0, nCols++,"W4");
	m_Grid.SetTextMatrix(0, nCols++,"W5");
	m_Grid.SetTextMatrix(0, nCols++,"H1");
	m_Grid.SetTextMatrix(0, nCols++,"H2");
	m_Grid.SetTextMatrix(0, nCols++,"H3");

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CSlabStructureDlg::SetDataInit()
{
	SetGridTitle();

	for( long nGuard = 0; nGuard < m_pDB->GetGuardFencesu(); nGuard++)
	{
		CCentSeparation *pCentSeparation = m_pDB->GetGuardFence(nGuard);

		long nCols = 1;
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_D1);
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_W1);
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_W2);	
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_W3);	
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_W4);
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_W5);
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_H1);	
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_H2);	
		m_Grid.SetTextMatrix(nGuard+1, nCols++, "%g", pCentSeparation->m_H3);
	}
	m_Grid.SetRedraw(TRUE,TRUE);	
}

void CSlabStructureDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CAPlateDrawDanmyun Draw(m_pDataManage);

	long nGuard = max(m_Grid.GetFocusCell().row-1,0);

	CPlateBridgeApp *pDB = m_pDB;
	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);
	if(!pGuard) return;
	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);

	CDomyun *pDom = m_DomView.GetDomyun();
	pDom->ClearEtt(TRUE);
	double dDimScale = 20;
	pDom->SetScaleDim(dDimScale);
	
	Draw.DstDrawGuardFence(pDom,pBx,nHDan);
	Draw.DstDimGuardFenceTop(pDom, pBx, nHDan, TRUE);
	Draw.DstDimGuardFenceSide(pDom, pBx, nHDan, TRUE);

	m_DomView.RedrawAll();
	if(bZoomAll)
	{
		m_DomView.ZoomAll();
		m_DomView.ZoomPlusNegaExt(TRUE, TRUE, (BOOL)1.1);
	}
}

BOOL CSlabStructureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DomView.SetOwnerMouseMode(FALSE);
	m_DomView.m_bEnablePopupMenu = TRUE;
	m_DomView.GetDomyun()->SetCWnd(&m_DomView);
	m_DomView.SetCrossLength(0);

	SetDataInit();
	DrawInputDomyunView();
	m_Grid.SetFocusCell(1, 1);

	return TRUE;
}

void CSlabStructureDlg::OnCellChangedRow(NMHDR* pNMHDR, LRESULT* pResult)
{
	DrawInputDomyunView(TRUE);
}

void CSlabStructureDlg::OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CSlabStructureDlg::OnOK() 
{
	SetDataSave();
	CDialog::OnOK();
}

void CSlabStructureDlg::OnStructureDefault() 
{
	SetDataDefault();
	SetDataInit();
	DrawInputDomyunView();	
}

void CSlabStructureDlg::OnGuardwallbangRecommend() 
{
	long nIdx = max(m_Grid.GetFocusCell().row-1,0);
	SetDataDefault5(nIdx);
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CSlabStructureDlg::OnButtonRecommendMedibarrHalf() 
{
	long nIdx = max(m_Grid.GetFocusCell().row-1,0);
	SetDataDefault2(nIdx);
	SetDataInit();
	DrawInputDomyunView(FALSE);	
}

void CSlabStructureDlg::OnButtonRecommendMedibarr() 
{
	long nIdx = max(m_Grid.GetFocusCell().row-1,0);
	SetDataDefault3(nIdx);
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CSlabStructureDlg::OnButtonNangan() 
{
	long nIdx = max(m_Grid.GetFocusCell().row-1,0);
	SetDataDefault4(nIdx);	
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

BOOL CSlabStructureDlg::IsFocusItem(CString &szTitle)
{
	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(0,nCol)==szTitle)
				{
					return TRUE;					
				}
			}
		}
	}
	return FALSE;
}

void CSlabStructureDlg::SetFocusItem(CString szTitle)
{
	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			long nRow = pGrid->GetFocusCell().row;
			if(nRow < 0) nRow=1;
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(0,nCol)==szTitle)
				{
					GetFocus();
					//pGrid->ResetSelection();
					
					pGrid->SetSelectedRange(-1,-1,-1,-1);
					pGrid->SetFocusCell(nRow,nCol);					
					pGrid->SendMessage(WM_KEYDOWN,VK_F2);
					break;
				}
			}
		}
	}
}

void CSlabStructureDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	POINT pt;
	pt = point;
	CRect rect(pt.x-5,pt.y-5,pt.x+5,pt.y+5);	
	CObArray obArr;	
	m_DomView.SetStopFind(FALSE);
	if(m_DomView.Intersects(&obArr,rect,INTER_TEXT))
	{			
		CDrawObject* pSelObj = ((CDrawObject*)obArr.GetAt(0));
		if(pSelObj->GetEttKind()==ETT_TEXT)
		{									
			CObText *pText = (CObText *)pSelObj;	
			CString szText = pText->m_TextString;
			if(IsFocusItem(szText))// && !m_bHasFocus)					
			{					
				CDRect rectText = m_DomView.GetDomyun()->GetTextRect(pText, szText);						
				
				rectText.left += pText->m_SttPoint.x;
				rectText.top  += pText->m_SttPoint.y;
				rectText.right += pText->m_SttPoint.x;
				rectText.bottom += pText->m_SttPoint.y;

				SetFocusItem(szText);								
			}			
		}
	}		
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CSlabStructureDlg::OnButtonSymmetry() 
{
	CPlateBridgeApp		*pDB = m_pDB;

	long nIdx = max(m_Grid.GetFocusCell().row-1,0);

	CCentSeparation *pGuard = pDB->GetGuardWall(nIdx);

	long	nHDan	= pDB->GetNumHDanByGuardFence(pGuard);
	BOOL	bLeft	= (nHDan < (pDB->GetQtyHDanNode()/2)) ? TRUE : FALSE;

	if(bLeft)
	{
		if(pDB->GetGuardFencesu()>1)
		{
			CCentSeparation *pGuardR = pDB->GetGuardWall(pDB->GetGuardFencesu()-1);
			pGuardR->m_D1	= 30 - pGuard->m_D1;
			pGuardR->m_H1	= pGuard->m_H1;
			pGuardR->m_H2	= pGuard->m_H2;
			pGuardR->m_H3	= pGuard->m_H3;

			pGuardR->m_W1	= pGuard->m_W5;
			pGuardR->m_W2	= pGuard->m_W4;
			pGuardR->m_W3	= pGuard->m_W3;
			pGuardR->m_W4	= pGuard->m_W2;
			pGuardR->m_W5	= pGuard->m_W1;
		}
	}
	else
	{
		CCentSeparation *pGuardL = pDB->GetGuardWall(0);
		pGuardL->m_D1	= 30 - pGuard->m_D1;
		pGuardL->m_H1	= pGuard->m_H1;
		pGuardL->m_H2	= pGuard->m_H2;
		pGuardL->m_H3	= pGuard->m_H3;

		pGuardL->m_W1	= pGuard->m_W5;
		pGuardL->m_W2	= pGuard->m_W4;
		pGuardL->m_W3	= pGuard->m_W3;
		pGuardL->m_W4	= pGuard->m_W2;
		pGuardL->m_W5	= pGuard->m_W1;
	}
	SetDataInit();
	DrawInputDomyunView();
}

BOOL CSlabStructureDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSlabStructureDlg::OnButtonGuardwall() 
{
	long nIdx = max(m_Grid.GetFocusCell().row-1,0);
	SetDataDefault6(nIdx);	
	SetDataInit();
	DrawInputDomyunView(FALSE);
}
