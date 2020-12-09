// DetSolePlateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetSolePlateDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetSolePlateDlg dialog

CDetSolePlateDlg::CDetSolePlateDlg(CGeneralBaseStd *pStd, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDetSolePlateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetSolePlateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTab		= 0;
	m_nJijum	= 0;	// 현재 지점
	m_nGirder	= 0;	// 현재 거더	// m_nCurIndex - 1;
	m_nJijumSu	= 0;
	m_nGirderSu	= 0;
	m_nCurIndex = 0;
	m_nCurrentShoeJewon = 0;
	m_pStd		= pStd;
}


void CDetSolePlateDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetSolePlateDlg)
	DDX_Control(pDX, IDC_TAB_SOLEPLATE, m_TabSelect);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SHOE_REF, m_btnShoe);
}

BEGIN_MESSAGE_MAP(CDetSolePlateDlg, TStackedPage)
	//{{AFX_MSG_MAP(CDetSolePlateDlg)
	ON_BN_CLICKED(IDC_BUTTON_SHOE_REF, OnButtonShoeRef)
	ON_CBN_DROPDOWN(IDC_BTN_BEARINGDB,	OnButtonSolePlateDropDown)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED_COL, IDC_GRID, OnCellChangedCol)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SOLEPLATE, &CDetSolePlateDlg::OnSelchangeTabSolePlate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetSolePlateDlg message handlers

///////////////////////////////////////////////////////////////////////////////

int CDetSolePlateDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TStackedPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.DeleteObject();
	// TODO: Add your specialized creation code here
	m_Font.CreateFont( 12,
		0,//int nWidth,
		0,//int nEscapement, 폰트의 각( degree, 직각인경우 900)
		0,//int nOrientation,한문자의 각 
		FW_NORMAL,// 폰트의 두께//int nWeight,
		FALSE,//BYTE bItalic, 
		FALSE,//BYTE bUnderline, 
		FALSE,//BYTE cStrikeOut, 
		DEFAULT_CHARSET,//BYTE nCharSet, DEFAULT_CHARSET,ANSI_CHARSET
		OUT_DEVICE_PRECIS,//BYTE nOutPrecision, OUT_DEVICE_PRECIS,OUT_DEFAULT_PRECIS
		CLIP_LH_ANGLES,//BYTE nClipPrecision, CLIP_LH_ANGLES,CLIP_DEFAULT_PRECIS
		DEFAULT_QUALITY,//BYTE nQuality, 
		VARIABLE_PITCH|FF_SWISS,//BYTE nPitchAndFamily, 
		"굴림체"//LPCTSTR lpszFacename 
		);
	CRect BRect(470,3,700,25);
	m_btnBearingDB.Create("교량받침 제원참조",WS_CHILD | WS_VISIBLE|BS_PUSHBUTTON, BRect,this,IDC_BTN_BEARINGDB);
	m_btnBearingDB.SetFont(&m_Font);
	m_btnBearingDB.ShowWindow(SW_SHOW);
	m_btnBearingDB.SetPushButtonStyle(xtpButtonDropDown);

	return 0;
}

void CDetSolePlateDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID,				SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_SOLEPLATE,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_BUTTON_SHOE_REF,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_BTN_BEARINGDB,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	m_nJijumSu = pBridge->m_nQtyJigan+1;

	m_TabSelect.DeleteAllItems();

	CString	str(_T(""));
	str.Format("A1");
	m_TabSelect.InsertItem(0, "A1");

	long nPierSu = m_nJijumSu - 2;

	for(long nPier = 1; nPier <= nPierSu; nPier++)
	{
		str.Format("P%d", nPier);
		m_TabSelect.InsertItem(nPier, str);
	}

	str.Format("A2");
	m_TabSelect.InsertItem(m_nJijumSu-1, "A2");

	m_TabSelect.SetCurSel(m_nJijum);

	#ifdef _DEBUG
		m_strDBRefPath					=	AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory") + "\\";
	#else
		m_strDBRefPath					=	AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory") + "\\DXF\\";
	#endif

	GetDlgItem(IDC_BUTTON_SHOE_REF)->EnableWindow(FALSE);
}

void CDetSolePlateDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pStd->GetDataManage()->GetDomyun();
	pDom->ClearEtt(TRUE);
	long nRow = m_Grid.GetFocusCell().row;
	if(nRow<0)	nRow = 1;

	CDomyun Dom(pDom);
	CAPlateDrawDanmyun Draw(m_pStd->GetDataManage());
	Draw.DrawInputSolePlate(&Dom, nRow, nRow-1, m_nJijum);
	*pDom << Dom;

	if(bZoomAll)
		m_pStd->GetDataManage()->GetDomyun()->ZoomAll();
}

void CDetSolePlateDlg::SetGridTitle()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long row = pBridge->GetGirdersu() + 1; // +1 is using for fixed...
	long col = 22;

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	//
	m_Grid.SetEditable(TRUE);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);	
//	m_Grid.SetTextBkColor(RGBEDITABLE);

	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(17,83);
	m_Grid.SetColumnWidth(18,83);
	m_Grid.SetRowHeightAll(20);

	m_Grid.SetTextMatrix(0,0,"구분");
	m_Grid.SetTextMatrix(0,1,"S(%)");
	m_Grid.SetTextMatrix(0,2,"W");
	m_Grid.SetTextMatrix(0,3,"W2");
	m_Grid.SetTextMatrix(0,4,"W3");
	m_Grid.SetTextMatrix(0,5,"H");
	m_Grid.SetTextMatrix(0,6,"H2");
	m_Grid.SetTextMatrix(0,7,"H3");
	m_Grid.SetTextMatrix(0,8,"Ф");
	m_Grid.SetTextMatrix(0,9,"T1");
	m_Grid.SetTextMatrix(0,10,"T");
	m_Grid.SetTextMatrix(0,11,"T2");
	m_Grid.SetTextMatrix(0,12,"반력(t)");
	m_Grid.SetTextMatrix(0,13,"sH1");
	m_Grid.SetTextMatrix(0,14,"sH2");
	m_Grid.SetTextMatrix(0,15,"sH3");
	m_Grid.SetTextMatrix(0,16,"sH4");	
	m_Grid.SetTextMatrix(0,17,"LEVEL 적용");
	m_Grid.SetTextMatrix(0,18,"Type");
	m_Grid.SetTextMatrix(0,19,"mW1");
	m_Grid.SetTextMatrix(0,20,"mW2");
	m_Grid.SetTextMatrix(0,21,"mW3");

	CString	str(_T(""));
	for (long i =1; i < row; i++)
	{
		str.Format("G%d", i);
		m_Grid.SetTextMatrix(i,0,str);	
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetSolePlateDlg::SetDataInit()
{
	UpdateData(FALSE);
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	SetGridTitle();

	CShoe* pShoe = NULL;
	
	long row = m_Grid.GetRows();
	long i = 0;
	for (i = 1; i < row; i++)
	{ 
		m_Grid.SetCellType(i,17,CT_COMBO);	
		m_Grid.SetCellType(i,18,CT_COMBO);	
	}

	long nRow=0;
	double JSlop = 0;
	CString	str = _T("");
	for(i = 1; i < row; i++)
	{
		nRow = i-1;
		CPlateGirderApp  *pGir = pBridge->GetGirder(nRow);
		CPlateBasicIndex *pBx  = pGir->GetBxOnJijum(m_nJijum);

		pGir->SetEleLevel(FALSE);
		JSlop = pBx ? pGir->GetGiulJongDanShoe(pBx) : 0;
		pShoe  =pGir->GetShoeByJijumNo(m_nJijum);

		str.Format("%.3f",JSlop*100);
		m_Grid.SetTextMatrix(i,1,str);
		m_Grid.SetItemState(i,1, m_Grid.GetItemState(i,1) | GVIS_READONLY);
		m_Grid.SetItemBkColour(i, 1, RGBREADONLY);

		str.Format("%g",pShoe->m_dW_SolePlate);
		m_Grid.SetTextMatrix(i,2,str);	

		str.Format("%g",pShoe->m_dW2_SolePlate);
		m_Grid.SetTextMatrix(i,3,str);	

		str.Format("%g",pShoe->m_dW3_SolePlate);
		m_Grid.SetTextMatrix(i,4,str);	

		str.Format("%g",pShoe->m_dH_SolePlate);
		m_Grid.SetTextMatrix(i,5,str);

		str.Format("%g",pShoe->m_dH2_SolePlate);
		m_Grid.SetTextMatrix(i,6,str);

		str.Format("%g",pShoe->m_dH3_SolePlate);
		m_Grid.SetTextMatrix(i,7,str);

		str.Format("%g",pShoe->m_dDia_SolePlateHole);
		m_Grid.SetTextMatrix(i,8,str);

		str.Format("%.3f",pShoe->m_dThick_SolePlate);
		m_Grid.SetTextMatrix(i,10,str);

		str.Format("%d",pShoe->m_dForceShoe);
		m_Grid.SetTextMatrix(i,12,str);
	
		str = m_Grid.GetItemText(i,10);
		m_Grid.SetTextMatrix(i,13,str);
		m_Grid.SetItemState(i,13, m_Grid.GetItemState(i,13) | GVIS_READONLY);
		m_Grid.SetItemBkColour(i, 13, RGBREADONLY);
		str.Format("%g",pShoe->m_dThick_Shoe);
		m_Grid.SetTextMatrix(i,14,str);
		str.Format("%g",pShoe->m_dThick_Mortar);
		m_Grid.SetTextMatrix(i,15,str);
		str.Format("%.3f",pShoe->m_dThick_Block);
		m_Grid.SetTextMatrix(i,16,str);
		
		if (pShoe->m_bLevelSolePlate)
		{
			str.Format("%.3f",atof(m_Grid.GetItemText(i,10)));
			m_Grid.SetTextMatrix(i,9,str);
			m_Grid.SetTextMatrix(i,11,str);
		}
		else
		{
			double W = pShoe->GetWidthTotalSolePlate();
			double T = pShoe->GetThickSolePlate();

			str.Format("%.3f",T-W/2*JSlop);
			m_Grid.SetTextMatrix(i,9,str);

			str.Format("%.3f",T+W/2*JSlop);
			m_Grid.SetTextMatrix(i,11,str);
		}
		m_Grid.SetItemState(i, 9,m_Grid.GetItemState(i, 9) | GVIS_READONLY);
		m_Grid.SetItemState(i,11,m_Grid.GetItemState(i,11) | GVIS_READONLY);
		m_Grid.SetItemBkColour(i, 9, RGBREADONLY);
		m_Grid.SetItemBkColour(i,11, RGBREADONLY);

		if(pShoe->m_bLevelSolePlate)
			str="적용";
		else
			str="적용안함";
		m_Grid.SetTextMatrix(i,17,str);

		if(pShoe->m_bHole_SolePlate)
			str="Hole";
		else
			str="Tap";
		m_Grid.SetTextMatrix(i,18,str);	
				
		str.Format("%g",pShoe->m_dW1_Mortal);
		m_Grid.SetTextMatrix(i,19,str);
		str.Format("%g",pShoe->m_dW2_Mortal);
		m_Grid.SetTextMatrix(i,20,str);
		str.Format("%g",pShoe->m_dW3_Mortal);
		m_Grid.SetTextMatrix(i,21,str);		
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);

	if(m_nJijum == 0 || m_nJijum == m_nJijumSu-1)
	{
		str = (m_nJijum == 0) ? "종점 동일적용" : "시점 동일적용";
		GetParent()->GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_PART)->SetWindowText(str);
	}
	else
		GetParent()->GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_PART)->SetWindowText("중간 지점 동일적용");
}

void CDetSolePlateDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	long nGirderSu = pBridge->GetGirdersu();

	for (long njijum = 0; njijum < pBridge->m_nQtyJigan+1; njijum++)
	{
		for (long i = 0; i <nGirderSu; i++)  //row = Quant of girder
		{
			SetDataDefaultSub(i,njijum);
		}
		
	}
	
	SetDlgText();
}

void CDetSolePlateDlg::SetDataDefaultSub(long row, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CPlateGirderApp* pGir = pBridge->GetGirder(row);
	CShoe          *pShoe = pGir->GetShoeByJijumNo(nJijum);
	///
	pShoe->m_bLevelSolePlate	= (BYTE)0;
	pShoe->m_dForceShoe			= 200;
	pShoe->m_dH_SolePlate		= 500;
	pShoe->m_dH2_SolePlate		= 50;
	pShoe->m_dH3_SolePlate		= 300;
	pShoe->m_dW_SolePlate		= 500;
	pShoe->m_dW2_SolePlate		= 50;
	pShoe->m_dW3_SolePlate		= 300;
	pShoe->m_dDia_SolePlateHole = 50;
	pShoe->m_bHole_SolePlate	= TRUE;

	pShoe->m_dW1_Mortal			= 250;		
	pShoe->m_dW2_Mortal			= 250;
	pShoe->m_dW3_Mortal			= 250;
	pShoe->m_dThick_Shoe		= 80;	
	pShoe->m_dThick_Mortar		= 50;

	double W = pShoe->GetWidthTotalSolePlate();  
	CPlateBasicIndex *pBxJijum = pGir->GetBxOnJijum(nJijum);
	double JSlop = pGir->GetGiulJongDanShoe(pBxJijum);
	pShoe->m_dThick_SolePlate = RoundUp(22 + W/2*fabs(JSlop),0);
	if((long)pShoe->m_dThick_SolePlate%2) pShoe->m_dThick_SolePlate += 1.0;
	pShoe->m_dThick_Block = 400-(pShoe->m_dThick_Mortar+pShoe->m_dThick_Shoe+pShoe->m_dThick_SolePlate);
}

void CDetSolePlateDlg::SetDlgText()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CString str,str1,str2;
	long	Ky = pBridge->m_nQtyJigan;
	if(m_nJijum == 0 || m_nJijum == Ky )
         str1.Format("시종점 동일적용");
	else str1.Format("중앙지점 동일적용");

	str.Format("%s - 교좌받침 상세",pBridge->m_sNameBridge);
	SetWindowText(str);
	GetParent()->GetDlgItem(IDC_BUTTON_APPLY_IDENTITY_PART)->SetWindowText(str1);
	str1 = pBridge->m_strJijumName[m_nJijum];

	long row = m_Grid.GetFocusCell().row;
	if (row < 1)
	{
		row =1;
	}
	
	long nG = row;
		
	GetWindowText(str2);
	str.Format("%s [G%d%s]",str2,nG,str1);
	SetWindowText(str);
	
}

void CDetSolePlateDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CShoe *pShoe = NULL;
	long nGirder; 
  
	for(long row = 1; row < m_Grid.GetRows(); row++)
 	{
		nGirder = row-1;
		CPlateGirderApp *pGir = pBridge->GetGirder(nGirder);
		pShoe =  pGir->GetShoeByJijumNo(m_nJijum);
	
		pShoe->m_dW_SolePlate = atof(m_Grid.GetItemText(row,2));
		pShoe->m_dW2_SolePlate = atof(m_Grid.GetItemText(row,3));
		pShoe->m_dW3_SolePlate = atof(m_Grid.GetItemText(row,4));
 		pShoe->m_dH_SolePlate = atof(m_Grid.GetItemText(row,5));
		pShoe->m_dH2_SolePlate = atof(m_Grid.GetItemText(row,6));
		pShoe->m_dH3_SolePlate = atof(m_Grid.GetItemText(row,7));
		pShoe->m_dDia_SolePlateHole = atof(m_Grid.GetItemText(row,8));
 		pShoe->m_dThick_SolePlate = atof(m_Grid.GetItemText(row,10));		
 		// REV_HA : 2000.6.14
 		pShoe->m_dForceShoe = (long)atof(m_Grid.GetItemText(row,12));
 		pShoe->m_dThick_Shoe  = atof(m_Grid.GetItemText(row,14));
 		pShoe->m_dThick_Mortar  = atof(m_Grid.GetItemText(row,15));
 		pShoe->m_dThick_Block  = atof(m_Grid.GetItemText(row,16));
		if (m_Grid.GetItemText(row,17) == "적용")
		{
			pShoe->m_bLevelSolePlate = (BYTE)1;
		}
		else if (m_Grid.GetItemText(row,17) == "적용안함")
		{
			pShoe->m_bLevelSolePlate = (BYTE)0;
		}

		if(m_Grid.GetItemText(row,18) == "Hole")
			pShoe->m_bHole_SolePlate = TRUE;
		else
			pShoe->m_bHole_SolePlate = FALSE;

		pShoe->m_dW1_Mortal = atof(m_Grid.GetItemText(row,19));		
		pShoe->m_dW2_Mortal = atof(m_Grid.GetItemText(row,20));
		pShoe->m_dW3_Mortal = atof(m_Grid.GetItemText(row,21));
	
 	}
}


void CDetSolePlateDlg::OnButtonRecommend()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
				  "교좌받침 상세 기본값", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	long i = m_Grid.GetFocusCell().row;
	if(i<0)	return;
	UpdateData(TRUE);
	SetDataDefaultSub(i-1,m_nJijum);
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView();
}
//2
void CDetSolePlateDlg::ApplyIdentityAllGirder()
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까", "교좌받침 상세 기본값", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	long row = m_Grid.GetFocusCell().row;
	if(row<0)
	{
		row = 1;
	}

	CString str;
	for (long irow = 1; irow < m_Grid.GetRows(); irow++)
	{
		for (long icol = 2 ; icol < m_Grid.GetCols(); icol++)
		{
			str = m_Grid.GetItemText(row,icol);
			m_Grid.SetTextMatrix(irow,icol,str);
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
	SetDataSave();

	DrawInputDomyunView();
}

void CDetSolePlateDlg::ApplyIdentityPart()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까", "교좌받침 상세 기본값", MB_ICONQUESTION|MB_YESNO)==IDNO)
			return;

	// 시종점부 동일적용

	if(m_nJijum==0 || m_nJijum==pBridge->m_nQtyJigan)
	{
		for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
		{
			// 1ST SHOE
			long nJijum = (m_nJijum==0) ? pBridge->m_nQtyJigan : 0;
			CPlateGirderApp *pGir	= pBridge->GetGirder(nG);
			CShoe			*pShoe	= pGir->GetShoe(m_nJijum);
			CShoe			*pShoe2	= pGir->GetShoe(nJijum);
			MakeSolePlateSameData(pShoe2, pShoe);
		}
	}
	// 중앙지점부 동일적용
	else
	{
		for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
		{
			// 1ST SHOE
			CPlateGirderApp *pGir  = pBridge->GetGirder(nG);
			CShoe      *pShoe = pGir->GetShoe(m_nJijum);
			for (long nJijum = 1; nJijum < pBridge->m_nQtyJigan; nJijum++)
			{
				CShoe *pShoe2 = pGir->GetShoe(nJijum);
				MakeSolePlateSameData(pShoe2,pShoe);
			}

		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView();
}

void CDetSolePlateDlg::ApplyIdentityAllJijum()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까", "교좌받침 상세 기본값", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		// 1ST SHOE
		CPlateGirderApp *pGir  = pBridge->GetGirder(nG);
		CShoe      *pShoe = pGir->GetShoe(m_nJijum);
		for (long nJijum = 0; nJijum < pBridge->m_nQtyJigan+1; nJijum++)
		{
			CShoe *pShoe2 = pGir->GetShoe(nJijum);
			MakeSolePlateSameData(pShoe2,pShoe);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView();
}

void CDetSolePlateDlg::ApplyCompleteInputAllSolePlate()
{
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

///////////////////////////////////////////////////////////////////////////////

BOOL CDetSolePlateDlg::PreTranslateMessage(MSG* pMsg)
{
	CStringArray szLevelArr;

	szLevelArr.Add("적용");
	szLevelArr.Add("적용안함");

	CStringArray szTypeArr;

	szTypeArr.Add("Hole");
	szTypeArr.Add("Tap");

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
	{
		char ClassName[30+1];
		::GetClassName(pMsg->hwnd,ClassName,30);
		if(strcmp(ClassName,"Edit")) return TRUE;
	}

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
	{
		if(m_Grid.GetFocusCell().col==m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col == 17) 	m_Grid.SetComboString(szLevelArr);
		if(next.col == 18) 	m_Grid.SetComboString(szTypeArr );
	}

	return TStackedPage::PreTranslateMessage(pMsg);
}

void CDetSolePlateDlg::OnSelchangeTabSolePlate(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetDataSave();

	m_nJijum = m_TabSelect.GetCurSel();

	SetDataInit();
	DrawInputDomyunView();

	m_Grid.SetFocus();

//	TStackedPage::OnCursorChange(0, 0);
	OnCursorChange(m_Grid.GetFocusCell().row,m_Grid.GetFocusCell().col);
}

void CDetSolePlateDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

    int iRow = GridView->iRow;
	int iCol = GridView->iColumn;

	CString	str(_T(""));

	if(iCol ==  9)
	{
		m_dT1 = m_Grid.GetTextMatrixDouble(iRow, iCol);

		if(m_bLevel) m_dT = m_dT1;
		else		 m_dT = m_dT1 + m_dSolePlateW / 2 * m_dVSlop;
	}

	if(iCol == 10)
	{
		m_dT = m_Grid.GetTextMatrixDouble(iRow, iCol);
	}

	if(iCol == 11)
	{
		m_dT2	= m_Grid.GetTextMatrixDouble(iRow, iCol);

		if(m_bLevel) m_dT = m_dT2;
		else         m_dT = m_dT2 - m_dSolePlateW / 2 * m_dVSlop;
	}

	if(iCol == 17)
	{
		if(m_bLevel)
			m_dT = m_Grid.GetTextMatrixDouble(iRow, 10);
	}

	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CDetSolePlateDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	m_nCurIndex = nRow;

	DrawInputDomyunView(TRUE);
	OnCursorChange(0, nCol);
}

void CDetSolePlateDlg::OnCursorChange(long nRow, long nCol)
{
//	TStackedPage::OnCursorChange(nRow, nCol);

	m_pStd->GetDataManage()->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CDetSolePlateDlg::MakeSolePlateSameData(CShoe *pShoe1, CShoe *pShoe2)
{
	pShoe1->m_bLevelSolePlate		= pShoe2->m_bLevelSolePlate;
	pShoe1->m_dForceShoe        	= pShoe2->m_dForceShoe;
	pShoe1->m_dH_SolePlate			= pShoe2->m_dH_SolePlate;
	pShoe1->m_dH2_SolePlate			= pShoe2->m_dH2_SolePlate;
	pShoe1->m_dH3_SolePlate			= pShoe2->m_dH3_SolePlate;
	pShoe1->m_dW_SolePlate			= pShoe2->m_dW_SolePlate;
	pShoe1->m_dW2_SolePlate			= pShoe2->m_dW2_SolePlate;
	pShoe1->m_dW3_SolePlate			= pShoe2->m_dW3_SolePlate;
	pShoe1->m_dDia_SolePlateHole	= pShoe2->m_dDia_SolePlateHole;
	pShoe1->m_bHole_SolePlate		= pShoe2->m_bHole_SolePlate;
	pShoe1->m_dThick_SolePlate		= pShoe2->m_dThick_SolePlate;
	pShoe1->m_dThick_Shoe			= pShoe2->m_dThick_Shoe;	
	pShoe1->m_dThick_Mortar			= pShoe2->m_dThick_Mortar;
	pShoe1->m_dThick_Block			= pShoe2->m_dThick_Block;
}

void CDetSolePlateDlg::OnButtonShoeRef() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	UpdateData(TRUE);
	
	SetDataSave();

	CShoe *pShoe, *pShoeOther;
	long nGirder = pBridge->GetNumberGirderByShoe(m_Grid.GetFocusCell().row-1,m_nJijum);
	pShoe = pBridge->GetGirder(nGirder)->GetShoe(m_nJijum);
	
	CShoeBox ShoeBox;
	
	long			nG, nRow;
	CBearingData	BearingData;
	// 데이타 차이로 인하여 변수 전달
	switch(pShoe->m_cDirectionShoe)
	{
	case 0: //고정단
		BearingData.m_nDir = 0; break;
	case 1: //양방향단
		BearingData.m_nDir = 3; break;
	case 2: //횡방향가동
		BearingData.m_nDir = 2; break;
	case 3: //종방향가동
		BearingData.m_nDir = 1; break;
	case 4: //고정단 45도
		BearingData.m_nDir = 0; break;
	}	
	
	ShoeBox.m_ForceVert = pShoe->m_dForceShoe;
	
	CString strShoeName = GetShoeName();
	CString	strShoeDir	= GetShoeDir();
	
	if( strShoeName.IsEmpty() || strShoeDir.IsEmpty() )
		return;
	
	CDlgBearingDB BearingDlg;
	
	BearingDlg.SetItemPath(m_strDBRefPath);
	BearingDlg.SetOpenFileInfo(strShoeName+ "_" + strShoeDir);
	BearingDlg.SetBearing(&BearingData);
	BearingDlg.SetStdHeightShoeSeat(0);
	
	if(BearingDlg.DoModal() == IDOK)
	{
		pShoe->m_nRefType		= BearingData.m_nIndexRef;
		pShoe->m_dForceShoe		= (long)BearingData.m_AllowForceVert;
		pShoe->m_dW_SolePlate	= BearingData.m_dSolePlateLength - pShoe->m_dW2_SolePlate*2;
		pShoe->m_dW3_SolePlate	= pShoe->m_dW_SolePlate - 40;
		pShoe->m_dH_SolePlate	= BearingData.m_dSolePlateWidth - pShoe->m_dH2_SolePlate*2;
		pShoe->m_dH3_SolePlate	= pShoe->m_dH_SolePlate - 40;
		pShoe->m_dThick_Shoe	= BearingData.m_H3;		
		
		if(pShoe->m_cDirectionShoe==0 && strShoeName.Find("유니슨 포트받침") > -1)		// 현재 유니슨 포트받침이고 고정단일때....
		{
			pShoe->m_dW_SolePlate	*=	sqrt(2.0);
			pShoe->m_dW_SolePlate	=	Round(pShoe->m_dW_SolePlate,-1);
			pShoe->m_dH_SolePlate	*=	sqrt(2.0);
			pShoe->m_dH_SolePlate	=	Round(pShoe->m_dH_SolePlate,-1);
		}
		
		if(AfxMessageBox("동일한 톤수,방향의 교량받침에 모두 적용하시겠습니까?",
			MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			for(long i = 1; i < m_Grid.GetRows(); i++)
			{
				nG		= pBridge->GetNumberGirderByShoe(i-1,m_nJijum);
				//nG		= m_pDB->GetNumberGirderByShoe();
				nRow	= i-1;
				
				CPlateGirderApp  *pGir	=	pBridge->GetGirder(nG);
				CPlateBasicIndex *pBx	=	pGir->GetBxOnJijum(m_nJijum);				
				
				pShoeOther = pBridge->GetShoeByRowCol(nRow,m_nJijum);
				
				if(pShoe==pShoeOther) continue;				
				
				if(pShoe->m_dForceShoe!=pShoeOther->m_dForceShoe) continue;
				
				if(pShoe->m_cDirectionShoe!=pShoeOther->m_cDirectionShoe) continue;
				
				pShoeOther->m_nRefType		= BearingData.m_nTypeBearing;
				pShoeOther->m_dForceShoe	= (long)BearingData.m_AllowForceVert;
				pShoeOther->m_dW_SolePlate	= BearingData.m_dSolePlateLength - pShoe->m_dW2_SolePlate*2;
				pShoeOther->m_dW3_SolePlate = pShoeOther->m_dW_SolePlate - 40;
				pShoeOther->m_dH_SolePlate	= BearingData.m_dSolePlateWidth - pShoe->m_dH2_SolePlate*2;
				pShoeOther->m_dH3_SolePlate = pShoeOther->m_dH_SolePlate - 40;
				pShoeOther->m_dThick_Shoe	= BearingData.m_H3;								
				
				if(pShoeOther->m_cDirectionShoe==0 && strShoeName.Find("유니슨 포트받침") > -1)
				{
					pShoeOther->m_dW_SolePlate	*= sqrt(2.0);
					pShoeOther->m_dW_SolePlate	=  Round(pShoeOther->m_dW_SolePlate,-1);
					pShoeOther->m_dH_SolePlate	*= sqrt(2.0);
					pShoeOther->m_dH_SolePlate	=  Round(pShoeOther->m_dH_SolePlate,-1);
				}
			}
		}
		
		SetDataInit();
		DrawInputDomyunView();
	}
}

void CDetSolePlateDlg::OnSelchangeComboCompany() 
{
	m_nCurrentShoeJewon = m_CompanyCombo.GetCurSel();		
}

void CDetSolePlateDlg::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	//if(!IsInitDialog())
	//{
	DrawInputDomyunView();
	//}
}

void CDetSolePlateDlg::OnCellChangedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    CString	str;

	int nCol = GridView->iColumn;
//	InitGrid();
	m_pStd->GetDataManage()->GetDomyun()->DeleteCursor();
	if (nCol ==1)
	{
		str = "S %";
	}
	else
	{
		str.Format("%s",m_Grid.GetItemText(0,nCol));					
	}
	m_pStd->GetDataManage()->GetDomyun()->STMakeCursor(str);
}

CString CDetSolePlateDlg::GetShoeName()
{
	CString sBearingName = _T("");
	m_btnBearingDB.GetWindowText(sBearingName);
	
	return sBearingName;
}

CString CDetSolePlateDlg::GetShoeDir()
{
	CPlateBridgeApp *pBridge	= m_pStd->GetBridge();
	CShoe			*pShoe;
	long			nGirder		= pBridge->GetNumberGirderByShoe(m_Grid.GetFocusCell().row-1,m_nJijum);
	
	pShoe = pBridge->GetGirder(nGirder)->GetShoe(m_nJijum);
	
	CString strShoeDir	=	_T("");
	int		nDir		=	pShoe->m_cDirectionShoe;
	
	switch(nDir)
	{
	case 0:		
		strShoeDir = _T("고정단"); break;
	case 1:		
		strShoeDir = _T("양방향"); break;
	case 2:		
		strShoeDir = _T("직각"); break;
	case 3:		
		strShoeDir = _T("교축"); break;
	case 4:
		strShoeDir = _T("고정단"); break;
	}
	
	return strShoeDir;
}

void CDetSolePlateDlg::OnButtonSolePlateDropDown()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_BEARINGDB);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CXTPButton* pButton = DYNAMIC_DOWNCAST(CXTPButton, GetDlgItem(IDC_BTN_BEARINGDB));
	ASSERT(pButton);
	
	DWORD dwStyle = pButton->GetPushButtonStyle();
	ASSERT(pButton->IsDropDownStyle());
	
	CXTPWindowRect rect(pButton);
	UINT	uFlag = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD;
	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(), uFlag, dwStyle == xtpButtonDropDownRight ? rect.right : rect.left,
		dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom,
		0, m_hWnd, 0);
	
	if(nCmd>0)
	{
		CString sBearingName = m_pStd->GetBearingDBName(nCmd);
		m_btnBearingDB.SetWindowText(sBearingName);
		if(sBearingName == UNREFERENCE)	GetDlgItem(IDC_BUTTON_SHOE_REF)->EnableWindow(FALSE);
		else							GetDlgItem(IDC_BUTTON_SHOE_REF)->EnableWindow(TRUE);
		UpdateData(FALSE);
	}
}