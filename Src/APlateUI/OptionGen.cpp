// OptionGen.cpp : implementation file
//

#include "stdafx.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionGen property page

IMPLEMENT_DYNCREATE(COptionGen, CPropertyPage)

COptionGen::COptionGen() : CPropertyPage(COptionGen::IDD)
{
	//{{AFX_DATA_INIT(COptionGen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptionGen::~COptionGen()
{
}

void COptionGen::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionGen)
	DDX_GridControl(pDX, IDC_GRID_UI, m_Grid);
	DDX_GridControl(pDX, IDC_GRID1_UI, m_GridPage);
	DDX_GridControl(pDX, IDC_GRID2_UI, m_GridTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionGen, CPropertyPage)
	//{{AFX_MSG_MAP(COptionGen)
	ON_BN_CLICKED(ID_PLATEOPT_DEFAULTBUTTON, OnDefault)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionGen message handlers
BOOL COptionGen::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	InitGridData();
	InitGridDataPage();
	InitGridDataTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionGen::InitGridData(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	if(bDefaultLoad)
	{		
		pOpt->m_szDGKName = "C:\\APlateWork\\DGK_Sample\\Default.dgk";
		pOpt->m_szXRefPath = "";
		pOpt->m_sTextBridgeDirection  = "";
		pOpt->m_sTextCounterDirection = "";		
		pOpt->m_bTitleDogak			  = TRUE;	//도각 타이틀 부분 페이지 넣기
		pOpt->m_sHead_DomyunNumber	  = "";
		pOpt->m_bUsedHCad			  = TRUE;
//		pOpt->m_bDesign				  = TRUE;
//		pOpt->m_bBujeGuBun			  = TRUE;
//		pOpt->m_KeyPlanbOutLine		  = FALSE;	//요약도 테두리선 적용
	}

	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(8);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(18);				
	m_Grid.SetColumnWidthAll(Rect.Width()/2);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetBkColor(RGB(255,255,255));
	m_Grid.SetFixedBkColor(RGB(255,255,255));	
	m_Grid.SetTextBkColor(RGB(255,255,255));

	m_Grid.SetTextMatrix(0,0,"설정항목");
	m_Grid.SetTextMatrix(1,0,"도각 파일",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(2,0,"XRef 도각 파일",(UINT)DT_LEFT);	
	m_Grid.SetTextMatrix(3,0,"교량진행방향 문자열",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,0,"교량반대방향 문자열",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,0,"도각 제목에 번호 표현",(UINT)DT_LEFT);
	m_Grid.SetCellType(5,1,CT_COMBO);
	m_Grid.SetTextMatrix(6,0,"도면 번호 첫머리 문자",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(7,0,"HCAD Block",(UINT)DT_LEFT);
	m_Grid.SetCellType(7,1,CT_COMBO);
	
	m_Grid.SetTextMatrix(0,1,"설정값");
	m_Grid.SetTextMatrix(1,1,pOpt->m_szDGKName,(UINT)DT_LEFT);
	m_Grid.SetCellType(1,1,CT_FILEBUTTON);
	m_Grid.SetInitFileDlgText("도각파일 (.dgk)|*.dgk|XRef DWG 파일 (.dwg)|*.dwg||");
	m_Grid.SetTextMatrix(2,1,pOpt->m_szXRefPath,(UINT)DT_LEFT);
	m_Grid.SetCellType(2,1,CT_FILEBUTTON);	
	m_Grid.SetTextMatrix(3,1,pOpt->m_sTextBridgeDirection,(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(4,1,pOpt->m_sTextCounterDirection,(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(5,1,pOpt->m_bTitleDogak ? "적용" : "적용 안함",(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(6,1,pOpt->m_sHead_DomyunNumber,(UINT)DT_LEFT);
	m_Grid.SetTextMatrix(7,1,pOpt->m_bUsedHCad ? "사용" : "사용 안함", (UINT)DT_LEFT);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void COptionGen::InitGridDataPage(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	COptionStd		*pOpt = pStd->m_pDomOptionStd;

	long nDomuynSize = pStd->GetDomyunSize();

	if(bDefaultLoad)
	{			
		pOpt->m_nDomPageArray.RemoveAll();		
		for(long i=0;i< nDomuynSize;i++)		
			pOpt->m_nDomPageArray.Add(0);					
	}

	CRect Rect;
	m_GridPage.GetClientRect(&Rect);
	m_GridPage.EnableReturnPass(TRUE);
	m_GridPage.SetFixedRowCount(1);	
	m_GridPage.SetFixedColumnCount(1);
	m_GridPage.SetRowCount(nDomuynSize+1);
	m_GridPage.SetColumnCount(2);
	m_GridPage.SetRowHeightAll(18);				
	m_GridPage.SetColumnWidth(0,Rect.Width()/2);
	m_GridPage.SetColumnWidth(1,Rect.Width()/2-20);
	m_GridPage.EnableInsertRow(FALSE);	
	m_GridPage.EnableInsertCol(FALSE);	
	m_GridPage.SetBkColor(RGB(255,255,255));
	m_GridPage.SetFixedBkColor(RGB(255,255,255));	
	m_GridPage.SetTextBkColor(RGB(255,255,255));

	CString szData;
	m_GridPage.SetTextMatrix(0,0,"도면종류");
	
	for(long nRow=1;nRow<= nDomuynSize;nRow++)
	{
		CString strDomyunTitle = pStd->GetAtDomyunTitle(nRow - 1);
		m_GridPage.SetTextMatrix(nRow,0, strDomyunTitle);
		szData.Format("%d",pOpt->m_nDomPageArray.GetAt(nRow-1));
		m_GridPage.SetTextMatrix(nRow,1,szData,(UINT)DT_RIGHT);		
	}
	
	m_GridPage.SetTextMatrix(0,1,"도면번호");
	
	m_GridPage.SetRedraw(TRUE,TRUE);
}

void COptionGen::InitGridDataTitle(BOOL bDefaultLoad)
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

//	if(bDefaultLoad)
	{					
		pOpt->m_nDomTitleArray.RemoveAll();
		for(long i = 0; i < pStd->GetDomyunSize(); i++)			
			pOpt->m_nDomTitleArray.Add(1);		
	}

	CRect Rect;
	m_GridTitle.GetClientRect(&Rect);
	m_GridTitle.EnableReturnPass(TRUE);
	m_GridTitle.SetFixedRowCount(1);	
	m_GridTitle.SetFixedColumnCount(1);
	m_GridTitle.SetRowCount(pStd->GetDomyunSize() + 1);
	m_GridTitle.SetColumnCount(2);
	m_GridTitle.SetRowHeightAll(18);				
	m_GridTitle.SetColumnWidth(0,Rect.Width()/2);
	m_GridTitle.SetColumnWidth(1,Rect.Width()/2-20);
	m_GridTitle.EnableInsertRow(FALSE);	
	m_GridTitle.EnableInsertCol(FALSE);	
	m_GridTitle.SetBkColor(RGB(255,255,255));
	m_GridTitle.SetFixedBkColor(RGB(255,255,255));	
	m_GridTitle.SetTextBkColor(RGB(255,255,255));

	CString szData;
	m_GridTitle.SetTextMatrix(0,0,"도면종류");

	for(long nRow=0; nRow < pStd->GetDomyunSize(); nRow++)	
	{
		CString strDomyunTitle = pStd->GetAtDomyunTitle(nRow);
		m_GridTitle.SetTextMatrix(nRow+1, 0, strDomyunTitle);		
		szData.Format("%d",pOpt->m_nDomTitleArray.GetAt(nRow));
		m_GridTitle.SetTextMatrix(nRow+1, 1, szData, (UINT)DT_RIGHT);		
	}

	m_GridTitle.SetTextMatrix(0,1,"도면 타이틀 번호");
	m_GridTitle.SetRedraw(TRUE,TRUE);
}

void COptionGen::OnDestroy() 
{
	CPropertyPage::OnDestroy();			
}

void COptionGen::OnOK() 
{
// 	// TODO: Add your specialized code here and/or call the base class
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;

	pOpt->m_szDGKName = m_Grid.GetTextMatrix(1,1);
	pStd->GetDogak()->Initialize(TRUE,pOpt->m_szDGKName,"");
	pOpt->m_szXRefPath = m_Grid.GetTextMatrix(2,1);
	pOpt->m_sTextBridgeDirection = m_Grid.GetTextMatrix(3,1);
	pOpt->m_sTextCounterDirection = m_Grid.GetTextMatrix(4,1);	
	CString str = m_Grid.GetTextMatrix(5,1);
	pOpt->m_bTitleDogak		   = (str.Compare("적용")==0) ? TRUE : FALSE;
	pOpt->m_sHead_DomyunNumber = m_Grid.GetTextMatrix(6,1);
	pOpt->m_bUsedHCad = m_Grid.GetTextMatrix(7,1)=="사용" ? TRUE : FALSE;
	CHgBaseDrawStd::SetUseHCadBlock(pOpt->m_bUsedHCad);

	for(long nRow=1;nRow <= pStd->GetDomyunSize();nRow++)
	{
		pOpt->m_nDomTitleArray.SetAt(nRow-1,(WORD)m_GridTitle.GetTextMatrixLong(nRow,1));
		pOpt->m_nDomPageArray.SetAt(nRow-1,(WORD)m_GridPage.GetTextMatrixLong(nRow,1));
	}

	pStd->SetModifiedAll();
	CPropertyPage::OnOK();
}



BOOL COptionGen::OnSetActive() 
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;
	CPlateBridgeApp	*pDB	= pStd->m_pDB;

	//if(((COptionSheet *)GetParent())->m_bSetOk)
	//	pOpt->m_nOptionCur = 0;	

	pOpt->m_nOptionCur = OPTION_PAGE_AROAD_GEN;
	
	return CPropertyPage::OnSetActive();
}

void COptionGen::OnDefault()
{
	InitGridData(TRUE);
	InitGridDataPage(TRUE);
	InitGridDataTitle(TRUE);
}

BOOL COptionGen::PreTranslateMessage(MSG* pMsg) 
{	
	CStringArray szTitle, szHCad;

	szTitle.Add("적용");
	szTitle.Add("적용 안함");

	szHCad.Add("사용");
	szHCad.Add("사용 안함");

	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col == 1 && next.row == 5)
			m_Grid.SetComboString(szTitle);
		if(next.col == 1 && next.row == 7)
			m_Grid.SetComboString(szHCad);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}
