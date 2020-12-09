// DanmyunTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DanmyunTypeDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDanmyunTypeDlg dialog


CDanmyunTypeDlg::CDanmyunTypeDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDanmyunTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDanmyunTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDanmyunTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDanmyunTypeDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DANMYUNTYPE_ALLGIRGER_SAME, m_btnSameAll);
	DDX_Control(pDX, IDC_DANMYUNTYPE_ALLGIRGER_CTYPE, m_btnCtype);
}
 

BEGIN_MESSAGE_MAP(CDanmyunTypeDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDanmyunTypeDlg)
	ON_BN_CLICKED(IDC_DANMYUNTYPE_ALLGIRGER_SAME, OnDanmyuntypeAllgirgerSame)
	ON_BN_CLICKED(IDC_DANMYUNTYPE_ALLGIRGER_CTYPE, OnDanmyuntypeAllgirgerCtype)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHBracingPositionDlg message handlers

void CDanmyunTypeDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		
	
	CDomyun Dom(pDom);
	if(m_nTab==0)
	{
		CAPlateDrawPyung Draw(m_pStd->GetDataManage());
		Draw.DrawPyung(&Dom, 5);
	}
	else
	{
		CAPlateDrawJong Draw(m_pStd->GetDataManage());
		Draw.DrawJongGirderTotal(&Dom);
	}
	
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDanmyunTypeDlg::SetGridTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	GetDlgItem(IDC_DANMYUNTYPE_ALLGIRGER_SAME)->EnableWindow(pDB->IsPlateGir());	

	long nRows = pDB->GetGirdersu()+1;
	long nCols = 0;

	for(long n=0; n<pDB->GetGirdersu()-1;n++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(n));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		nCols = max(Finder.GetCountBx() + 1, nCols);
	}
	
	m_Grid.DeleteAllItems();
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(pDB->IsPlateGir() ? 50 : 35);		
	m_Grid.SetColumnWidth(0, 90);

	CString str;
	m_Grid.SetTextMatrix(0,0,"구 분");
	for(long nRow=1; nRow<nRows; nRow++)
	{
		if(nRow==1)	str = "타입(CR)";
		else		str.Format("타입(G%d~G%d)", nRow-1, nRow);
		m_Grid.SetTextMatrix(nRow, 0, str);
	}
	for(long nCol = 1; nCol < nCols;nCol++)
	{
		str.Format("%d",nCol);
		m_Grid.SetTextMatrix(0, nCol, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDanmyunTypeDlg::SetDataInit()
{	
	char *szEnType[] = { "A", "P", "B", "C"};

	CPlateBridgeApp *pDB = m_pStd->GetBridge();		
	long nRows = pDB->GetGirdersu()+1;

	for(long nRow=1; nRow<nRows; nRow++)
	{
		long nG = nRow-2;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		long nCol=1;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec)
			{	
				long nType = pSec->GetenType();
				if(nType == SECTION_P || nType == SECTION_A)
				{
					if(pDB->IsPlateGir())
					{
						m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
						m_Grid.SetCellType(nRow,nCol,CT_COMBO);
					}
					else
						m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
					
					CString sType = pSec->GetenDetType() == SECDET_CROSSBEAM ?
								((CString)szEnType[nType] + "-C") : ((CString)szEnType[nType] + "-B");
					m_Grid.SetTextMatrix(nRow,nCol, sType);
					// C B
				}
				else if(nType != -1)
				{
					if(pDB->IsPlateGir())
					{
						m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
						m_Grid.SetCellType(nRow,nCol,CT_COMBO);
					}
					else
						m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
					
					m_Grid.SetTextMatrix(nRow,nCol,szEnType[nType]);
				}				
			}

			nCol++;
			
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}		
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}


//이전지점부터 ~ 현재지점까지 단면 타입 셋
void CDanmyunTypeDlg::SetDanmyunTypeByJijum(CPlateBasicIndex* pBxPrevJijum, CPlateBasicIndex* pBxJijum)
{
	if(!pBxPrevJijum || !pBxJijum) return;
	if(pBxPrevJijum == pBxJijum) return;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	CPlateGirderApp *pGir = pBxPrevJijum->GetGirder();
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxPrevJijum,pBxJijum);
	int nSectionDistance = 0;
	while(pBx)
	{
		nSectionDistance++;
		pBx = Finder.GetBxNext();
	}

	//간격이고 시작과끝을 포함하므로  -1을 한다.
	nSectionDistance--;

	// 3N  => ■ B B ⓒ B B ⓒ B B ■
	// 3N+1=> ■ B ⓒ B B C B B ⓒ B■
	// 3N+2=> ■ ⓒ B B ⓒ B B ⓒ B B ⓒ■


	int nStartPos = nSectionDistance % 3;
	//   B B C 의 반복으로 셋팅하되 시작위치(nStartPos)에 따라서 밀려서 시작하면 됨
	
	CByteArray ArrBBC;
	long n = 0;
	for(n = 0; n < nSectionDistance+6; n += 3)
	{
		ArrBBC.Add('B');
		ArrBBC.Add('B');
		ArrBBC.Add('C');
	}
	
	for(n = 0; n < nStartPos; n++ )
		ArrBBC.RemoveAt(0);


	// Set 단면 타입
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxPrevJijum,pBxJijum,FALSE,FALSE);
	n = 0;
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();

		ASSERT(pSec->GetenType() != SECTION_A);
		ASSERT(pSec->GetenType() != SECTION_P);

		if(ArrBBC[n++] == 'B')
		{
			if(pDB->GetBridgeType()==PLATE_GIR)
			{
				pSec->SetenType(SECTION_B);
				pSec->SetenDetType(SECDET_VBRACING);		
			}
			else
			{
				pSec->SetenType(SECTION_C);
				pSec->SetenDetType(SECDET_CROSSBEAM);
			}
		}
		else
		{
			pSec->SetenType(SECTION_C);
			pSec->SetenDetType(SECDET_CROSSBEAM);					
		}

		pBx = Finder.GetBxNext();
	}
}

void CDanmyunTypeDlg::SetDataDefault()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	
	long nType	= pDB->GetBridgeType();	// 2007.10.15 이종원차장 수정요청 - 모두 SECDET_CROSSBEAM 타입으로 설정
	for(long nG=-1; nG<pDB->GetGirdersu()+1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		CPlateBasicIndex* pBxPrevJijum = NULL;
		long nJiJum=0;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			if(pSec)
			{	
				if(pBx->GetMatchCol() <= -10)//지점인 경우
				{
					if((nJiJum==0 && pDB->m_nTypeAbutOrPier[0]==0) || (nJiJum==pDB->m_nQtyJigan && pDB->m_nTypeAbutOrPier[1]==0))
						pSec->SetenType(SECTION_A);
					else
						pSec->SetenType(SECTION_P);
					++nJiJum;

					pSec->SetenDetType(SECDET_CROSSBEAM);
					SetDanmyunTypeByJijum(pBxPrevJijum, pBx);//이전지점부터 ~ 현재지점까지
					pBxPrevJijum = pBx;

				}
				else
				{
					pSec->SetenType(SECTION_B);
					if(nType==0)	pSec->SetenDetType(SECDET_VBRACING);
					else			pSec->SetenDetType(SECDET_CROSSBEAM);
				}

				pSec->SetenTypeNum(0);
			}						
			pBx = Finder.GetBxNext();
		}		
	}
}


void CDanmyunTypeDlg::SetDataSave()
{		
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= NULL;
	CPlateBasicIndex	*pBx	= NULL;
	CSection			*pSec	= NULL;

	long	nG		= 0;
	long	nCol	= 0;
	CString	szType	= _T("");
	long	nRow =0;
	for(nRow=1; nRow<pDB->GetGirdersu()+1; nRow++)
	{
		nG	= nRow-2;
		nCol= 1;
		pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			CString szType = m_Grid.GetTextMatrix(nRow,nCol);
			if(pSec && szType.GetLength() > 0)
			{
				switch(szType[0])	
				{
				case 'A':
					pSec->SetenType(SECTION_A);
					if(szType=="A-B")
					{
						pSec->SetenDetType(SECDET_VBRACING);
						pSec->GetVBracing()->SetType(TYPE_INVVBRACING);
					}
					else
						pSec->SetenDetType(SECDET_CROSSBEAM);
					break;
				case 'P':
					pSec->SetenType(SECTION_P);
					if(szType=="P-B")
					{
						pSec->SetenDetType(SECDET_VBRACING);
						pSec->GetVBracing()->SetType(TYPE_VBRACING);				
						
					}
					else
						pSec->SetenDetType(SECDET_CROSSBEAM);
					break;
				default :
					if(szType=="B")
					{
						pSec->SetenType(SECTION_B);	
						pSec->SetenDetType(SECDET_VBRACING);
						pSec->GetVBracing()->SetType(TYPE_VBRACING);
					}
					else if(szType=="C")
					{
						pSec->SetenType(SECTION_C);
						pSec->SetenDetType(SECDET_CROSSBEAM);
					}
				}
				pSec->SetenTypeNum(m_Grid.GetTextMatrixLong(nRow+1,nCol++));
			}
			else
				nCol++;
			
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}

	// 마지막 거더의 단면 저장
	nRow	= pDB->GetGirdersu()+1;
	nG		= pDB->GetGirdersu()-1;
	nCol	= 1;
	pGir	= pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	while(pBx)
	{
		pSec	= pBx->GetSection();
		szType	= m_Grid.GetTextMatrix(nRow,nCol);

		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft, 1);
		if(pBxMatch==NULL)
		{
			// APLATE-717
			// 실제로 가로보가 없는 경우
			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
			nCol++;
			continue;
		}

		if(pSec && szType.GetLength() > 0)
		{
			switch(szType[0])	
			{
			case 'A':
				pSec->SetenType(SECTION_A);
				if(szType=="A-B")
				{
					pSec->SetenDetType(SECDET_VBRACING);
					pSec->GetVBracing()->SetType(TYPE_INVVBRACING);
				}
				else
					pSec->SetenDetType(SECDET_CROSSBEAM);
				break;
			case 'P':
				pSec->SetenType(SECTION_P);
				if(szType=="P-B")
				{
					pSec->SetenDetType(SECDET_VBRACING);
					pSec->GetVBracing()->SetType(TYPE_VBRACING);
				}
				else
					pSec->SetenDetType(SECDET_CROSSBEAM);
				break;
			default :
				if(szType=="B")
				{
					pSec->SetenType(SECTION_B);	
					pSec->SetenDetType(SECDET_VBRACING);
					pSec->GetVBracing()->SetType(TYPE_VBRACING);
				}
				else if(szType=="C")
				{
					pSec->SetenType(SECTION_C);
					pSec->SetenDetType(SECDET_CROSSBEAM);
				}
			}
			pSec->SetenTypeNum(m_Grid.GetTextMatrixLong(nRow,nCol++));
		}
		else
			nCol++;
		
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();
	}

	// APLATE-735
	// 마지막거더의 종점위치가 지점으로 저장되지 못한 경우
	pBx	= Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
	if(pBx->IsJijum()==FALSE)
	{
		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft, 1);
		if(pBxMatch)
		{
			pSec = pBx->GetSection();
			CSection *pSecMatch = pBxMatch->GetSection();
			if(pSec && pSecMatch)
			{
				pSec->SetenType(SECTION_A);
				if(pSecMatch->GetenDetType()==SECDET_VBRACING)
				{
					pSec->SetenDetType(SECDET_VBRACING);
					pSec->GetVBracing()->SetType(TYPE_INVVBRACING);
				}
				else
					pSec->SetenDetType(SECDET_CROSSBEAM);
			}
		}
	}
}

void CDanmyunTypeDlg::OnCellChangedData(NMHDR*nmgv, LRESULT*)
{			
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;
	CGeneralBaseDlg::OnCellChangedData(nmgv, 0);	
}

void CDanmyunTypeDlg::OnCellChanged(NMHDR*nmgv,LRESULT*)
{			
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	int c = 1;
	
	m_pView->GetDomyun()->ClearEtt(FALSE);

	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->FixEtt(TRUE);
	for (long r = 1; r < m_Grid.GetRowCount(); r++)
	{
		if (r != nRow)
			continue;
		long nG = r-2;
		double sta;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING );
		while(pBx)
		{
			sta = pBx->GetStation();
			if(c==nCol)
				break;
			pBx = Finder.GetBxNext();
			c++;
		}
		Draw.SetCursorPosition(nG, -1, sta, pDom, TRUE, 40, 2, 0);
	}
	pDom->FixEtt(FALSE);
}

// 모든 거더 동일적용
void CDanmyunTypeDlg::OnDanmyuntypeAllgirgerSame() 
{
	long nCurRow = m_Grid.GetFocusCell().row;
	if(nCurRow<=0) nCurRow = 1;

	CString str;
	str.Format("  %d번 거더 기준으로 제원을 배치합니다. \n    계속하시겠습니까?", nCurRow-1);
	if (IDCANCEL == MessageBox(str, "모든 거더 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	for(long nRow=1; nRow< m_Grid.GetRowCount(); nRow++)
	{
		if (nRow == nCurRow)
			continue;
		for (long nCol = 1; nCol < m_Grid.GetColumnCount(); nCol++)
		{
			CString sOrg = m_Grid.GetTextMatrix(nCurRow, nCol);
			CString sDest = m_Grid.GetTextMatrix(nRow, nCol);
			if(!sOrg.IsEmpty() && !sDest.IsEmpty())
				m_Grid.SetTextMatrix(nRow, nCol, sOrg);
			/*
			if(sOrg[0] != 'A' &&  sOrg[0] != 'P' && sDest[0] != 'A' &&  sDest != 'P')
				m_Grid.SetTextMatrix(nRow, nCol, sOrg); */ //지점부는 안바뀌게 한 이유를 아무도 모름....070115...KB...
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	SetDataSave();
	DrawInputDomyunView(FALSE);
	
}

BOOL CDanmyunTypeDlg::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		CString sTxt = m_Grid.GetTextMatrix(next.row, next.col);
		if(!sTxt.IsEmpty())
		{
			switch( sTxt[0] )
			{			
			case 'A': 
			    strCombo.Add("A-B");
				strCombo.Add("A-C");
				m_Grid.SetComboString(strCombo);
				break;
			case 'P' :
			    strCombo.Add("P-B");
				strCombo.Add("P-C");
				m_Grid.SetComboString(strCombo);
				break;
			default :
				strCombo.Add("B");
				strCombo.Add("C");
				m_Grid.SetComboString(strCombo);
				break;
			}
		}
	} 

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CDanmyunTypeDlg::OnPreInitDialog()
{
	SetGridTitle();
	
	SetResize(IDC_DANMYUNTYPE_ALLGIRGER_SAME,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_DANMYUNTYPE_ALLGIRGER_CTYPE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDanmyunTypeDlg::OnDanmyuntypeAllgirgerCtype() 
{
	long nCurRow = m_Grid.GetFocusCell().row;
	if(nCurRow<=0) nCurRow = 1;

	CString str;
	str.Format("  %d번 거더 기준으로 제원을 배치합니다. \n    계속하시겠습니까?", nCurRow-1);
	if (IDCANCEL == MessageBox(str, "모든 거더 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	for(long nRow=1; nRow< m_Grid.GetRowCount(); nRow++)
	{
		for (long nCol = 1; nCol < m_Grid.GetColumnCount(); nCol++)
		{
			CString szType = _T("");
			szType	= m_Grid.GetTextMatrix(nRow,nCol);
			if(szType[0] == 'A')	
				m_Grid.SetTextMatrix(nRow,nCol,"A-C");
			else if(szType[0] == 'P')
				m_Grid.SetTextMatrix(nRow,nCol,"P-C");
			else
				m_Grid.SetTextMatrix(nRow,nCol,"C");
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	SetDataSave();
	DrawInputDomyunView(FALSE);
}
