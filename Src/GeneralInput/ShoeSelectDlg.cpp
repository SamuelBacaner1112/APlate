// ShoeSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "ShoeSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShoeSelectDlg dialog


CShoeSelectDlg::CShoeSelectDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: CDialog(CShoeSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShoeSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStd = pStd;

	m_StrShoe.Add("고정단");
	m_StrShoe.Add("고정단 45도");
	m_StrShoe.Add("양방향단");
	m_StrShoe.Add("횡방향가동");
	m_StrShoe.Add("종방향가동");
}


void CShoeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShoeSelectDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btnReset);
	DDX_Control(pDX, IDOK, m_btnOK);
}

BEGIN_MESSAGE_MAP(CShoeSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CShoeSelectDlg)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_DBCLK,IDC_GRID,OnCellDblClk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShoeSelectDlg message handlers
BOOL CShoeSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Dynamic Combo	
	SetGridTitle();
	DataInit();

	return TRUE;
}

void CShoeSelectDlg::SetGridTitle()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long cols = pBridge->m_nQtyJigan+2;
	long rows = GetMaxShoesu()+1;
	CString str;

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetRowCount(rows);
	m_Grid.SetColumnCount(cols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));

	m_Grid.SetTextMatrix(0,0,"구 분");
	
	for(long col = 1;col < cols;col++)
	{
		str = pBridge->m_strJijumName[col-1];
		m_Grid.SetTextMatrix(0,col,str);
	}

	for(long row = 1;row < rows;row++)
	{
		str.Format("%d",row);
		m_Grid.SetTextMatrix(row,0,str);
	}
	
	m_Grid.SetComboString(m_StrShoe);
	m_Grid.SetRedraw(TRUE,TRUE);
}

long CShoeSelectDlg::GetMaxShoesu()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	
	long	Shoesu	= 0;
	long	Biksu	= 0;
	for(long n = 0;n <= pBridge->m_nQtyJigan;n++)
	{
		for(long g = 0;g < pBridge->GetGirdersu();g++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(g);
			CShoe *pShoe = pGir->GetShoe(n);
			if( pShoe ) Biksu++;			
		}
		if( Shoesu < Biksu ) Shoesu = Biksu;
		Biksu = 0;
	}

	return Shoesu;
}

BOOL CShoeSelectDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CShoeSelectDlg::OnOK() 
{
	DataSave();	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////
void CShoeSelectDlg::DataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	
	long	row = 1;
	long	col = 1;
	CString str;
	CString strCombo = _T("고정단!양방향단!횡방향가동!종방향가동!고정단 45도");
	for(long n = 0;n <= pBridge->m_nQtyJigan;n++)
	{
		for(long g = 0;g < pBridge->GetGirdersu();g++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(g);
			CShoe *pShoe = pGir->GetShoe(n);			
			
			str = GetShoeDirection(pShoe->m_cDirectionShoe);
			m_Grid.SetTextMatrix(row,col,str);
			SET_GRID_RCOMBO(m_Grid,row,col,strCombo,&pShoe->m_cDirectionShoe);
			row++;					
		}
		col++;
		row = 1;
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CShoeSelectDlg::DataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	
	long	nRow = 1,
			nCol = 1,
			Dir = -1;
	CString str;
	for(long n = 0;n <= pBridge->m_nQtyJigan;n++)
	{
		for(long nG = 0;nG < pBridge->GetGirdersu();nG++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(nG);
			CShoe *pShoe = pGir->GetShoe(n);
						
			str = m_Grid.GetItemText(nRow,nCol);
			Dir = GetShoeDirection(str);
			pShoe->m_cDirectionShoe = (BYTE)Dir;
			nRow++;					
		}
		nCol++;
		nRow = 1;
	}
}

long CShoeSelectDlg::GetShoeDirection(CString str)
{
	long Dir = -1;

	if( str == "고정단" )	  return 0;
	if( str == "양방향단" )   return 1;
	if( str == "횡방향가동" ) return 2;
	if( str == "종방향가동" ) return 3;
	if( str == "고정단 45도" )return 4;	

	return Dir;
}

CString CShoeSelectDlg::GetShoeDirection(long Dir)
// Dir값에 따른 슈방향 텍스트 리턴
{
	CString str;
	switch(Dir)
	{
		case 0: str = "고정단";		break;
		case 1: str = "양방향단";	break;
		case 2: str = "횡방향가동";	break;
		case 3: str = "종방향가동";	break;
		case 4: str = "고정단 45도";break;
	}

	return str;
}

void CShoeSelectDlg::SetShoeDirection(CString oStr,CString cStr)
// 어느 한 지점이 고정단 -> 방향 또는 방향 -> 고정단으로 변화시 나머지 지점 슈 방향 결정
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if( oStr.Left(6) != "고정단" && cStr.Left(6) != "고정단" ) return;
	
	long	Dir    = 0;
	BOOL	bLRFix = FALSE,		// 좌우 고정단 찾기
			bUDFix = FALSE;		// 상하 고정단 찾기

	for(long k = 0;k <= pBridge->m_nQtyJigan;k++)
	{
		for(long nG = 0;nG < pBridge->GetGirdersu();nG++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(nG);
			CShoe *pShoe = pGir->GetShoe(k);
			
			if(pShoe->m_cDirectionShoe != 0 && pShoe->m_cDirectionShoe != 4)
			{
				bLRFix = GetLRFixShoe(nG,k);
				bUDFix = GetUDFixShoe(nG,k);
				Dir	   = GetShoeDirection(bLRFix,bUDFix);

				pShoe->m_cDirectionShoe = (BYTE)Dir;
				bLRFix = bUDFix = FALSE;
			}			
		}
	}

}

BOOL CShoeSelectDlg::GetLRFixShoe(long nG,long k)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	
	long Dir	= -1;
	BOOL bLRFix = FALSE;
	
	for(long n = 0;n <= pBridge->m_nQtyJigan;n++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CShoe *pShoe = pGir->GetShoe(n);
		
		Dir = (long)pShoe->m_cDirectionShoe;
		if( Dir == 0 || Dir == 4) 
		{
			bLRFix = TRUE;
			break;
		}
	}		
	return bLRFix;
}

BOOL CShoeSelectDlg::GetUDFixShoe(long nG,long k)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	
	long Dir	= -1;
	BOOL bUDFix = FALSE;

	for(long g = 0;g < pBridge->GetGirdersu();g++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(g);
		CShoe *pShoe = pGir->GetShoe(k);

		Dir = (long)pShoe->m_cDirectionShoe;
		if( Dir == 0 || Dir == 4)
		{
			bUDFix = TRUE;
			break;
		}	
	}

	return bUDFix;
}

long CShoeSelectDlg::GetShoeDirection(BOOL bLRFix,BOOL bUDFix)
{
	long	Dir = 1;
	if( bLRFix && bUDFix ) return 1;
	if( bUDFix ) return 2;
	if( bLRFix ) return 3;

	return Dir;
}

void CShoeSelectDlg::OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)pNMHDR;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	DataSave();
	SetShoeDirection(m_Grid.GetItemText(nRow,nCol),m_Grid.GetItemText(nRow,nCol));
	DataInit();
	
}

void CShoeSelectDlg::OnCellChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	//CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	//long nJigan = nCol-1;
	//long nShoePerGirder = GetMaxShoesu() / pBridge->GetGirdersu();
	//long nG = (nRow-1)/nShoePerGirder;
	//long nPos = (nRow-1) % nShoePerGirder;
	//CPlateGirderApp *pGir = pBridge->GetGirder(nG);
	//CShoe *pShoe = pGir->GetShoe(nJigan);
	//double dDis = -pBridge->GetGirder(nG)->m_dWidth/2 + pShoe->GetDisShoeFrWeb();	

	//CPlateGirderApp *pGir = pBridge->GetGirder(nG);	
	//double dSta = pGir->GetStationCenterLineXxx(pGir->GetStationOnJijum(nJigan), dDis, pGir->GetAngleOnJijum(nJigan));
	//m_pDDBGen->SetCursorPosition(nG, 0, dSta, dDis, NULL);
}

void CShoeSelectDlg::OnButtonReset() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long n = 0;n <= pBridge->m_nQtyJigan;n++)
	{
		for(long g = 0;g < pBridge->GetGirdersu();g++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(g);
			CShoe *pShoe = pGir->GetShoe(n);			
			pShoe->m_cDirectionShoe = 1;
		}
	}

	SetGridTitle();
	DataInit();
	DataSave();
}
void CShoeSelectDlg::OnCellDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW*)pNMHDR;

	int nRow = GridView->iRow;
	int nCol = GridView->iColumn;

	if(m_Grid.GetTextMatrix(nRow,nCol) != "양방향단") return;

	m_Grid.SetTextMatrix(nRow,nCol,"고정단 45도");
	m_Grid.SetRedraw(TRUE,TRUE);

}
