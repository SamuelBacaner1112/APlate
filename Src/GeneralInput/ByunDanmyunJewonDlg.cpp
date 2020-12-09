 // ByunDanmyunJewonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CByunDanmyunJewonDlg dialog


CByunDanmyunJewonDlg::CByunDanmyunJewonDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CByunDanmyunJewonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CByunDanmyunJewonDlg)
	m_bUseByunDan = FALSE;
	m_szArcInfo = _T("");
	m_nArcType = 0;
	//}}AFX_DATA_INIT
	m_nTab = 0;
	m_nByunDanType = 0;
}


void CByunDanmyunJewonDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CByunDanmyunJewonDlg)
	DDX_Control(pDX, IDC_TAB_SEL, m_ctlTab);
	DDX_Control(pDX, IDC_BUTTON_TYPE4, m_btnType4);
	DDX_Control(pDX, IDC_BUTTON_TYPE3, m_btnType3);
	DDX_Control(pDX, IDC_BUTTON_TYPE2, m_btnType2);
	DDX_Control(pDX, IDC_BUTTON_TYPE1, m_btnType1);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_USEBYUN, m_bUseByunDan);
	DDX_Text(pDX, IDC_STATIC_TITLE2, m_szArcInfo);
	DDX_Radio(pDX, IDC_RADIO_CIRCLETYPE1, m_nArcType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CByunDanmyunJewonDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CByunDanmyunJewonDlg)
	ON_BN_CLICKED(IDC_CHECK_USEBYUN, OnCheckUsebyun)
	ON_BN_CLICKED(IDC_BUTTON_TYPE1, OnButtonType1)
	ON_BN_CLICKED(IDC_BUTTON_TYPE2, OnButtonType2)
	ON_BN_CLICKED(IDC_BUTTON_TYPE3, OnButtonType3)
	ON_BN_CLICKED(IDC_BUTTON_TYPE4, OnButtonType4)
	ON_BN_CLICKED(IDC_RADIO_CIRCLETYPE1, OnRadioCircletype1)
	ON_BN_CLICKED(IDC_RADIO_CIRCLETYPE2, OnRadioCircletype2)
	ON_BN_CLICKED(IDC_RADIO_CIRCLETYPE3, OnRadioCircletype3)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommend)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SEL, &CByunDanmyunJewonDlg::OnSelchangeTabSel)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CByunDanmyunJewonDlg message handlers
void CByunDanmyunJewonDlg::OnPreInitDialog()
{
	SetResize(IDC_TAB_SEL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_USEBYUN, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TITLE2, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_CIRCLETYPE1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_CIRCLETYPE2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_CIRCLETYPE3, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_TYPE1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_TYPE2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_TYPE3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_TYPE4, SZ_TOP_LEFT, SZ_TOP_LEFT);

	if(!m_bitmap[0].GetSafeHandle())
		m_bitmap[0].LoadBitmap(IDB_BITMAP_BYUNTYPE1);	
	if(!m_bitmap[1].GetSafeHandle())
		m_bitmap[1].LoadBitmap(IDB_BITMAP_BYUNTYPE2);	
	if(!m_bitmap[2].GetSafeHandle())
		m_bitmap[2].LoadBitmap(IDB_BITMAP_BYUNTYPE3);	
	if(!m_bitmap[3].GetSafeHandle())
		m_bitmap[3].LoadBitmap(IDB_BITMAP_BYUNTYPE4);	
	//CSize(224,40)

	//m_btnType1.EnableMarkup();
	//m_btnType1.SetWindowText("<Image Source='res://bitmap_byuntype1.bmp' VerticalAlignment='Center'/>");
	m_btnType1.SetFlatStyle(TRUE);
	m_btnType2.SetFlatStyle(TRUE);
	m_btnType3.SetFlatStyle(TRUE);
	m_btnType4.SetFlatStyle(TRUE);

	m_btnType1.SetBitmap(CSize(224,40),IDB_BITMAP_BYUNTYPE1);
	m_btnType2.SetBitmap(CSize(224,40),IDB_BITMAP_BYUNTYPE2);
	m_btnType3.SetBitmap(CSize(224,40),IDB_BITMAP_BYUNTYPE3);
	m_btnType4.SetBitmap(CSize(224,40),IDB_BITMAP_BYUNTYPE4);
	CString szText;

	for(long i=0;i<m_pStd->GetBridge()->m_nQtyJigan; i++)
	{
		szText.Format("%d����",i+1);
		m_ctlTab.InsertItem(i,szText);
	}
} 

void CByunDanmyunJewonDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pBrige = m_pStd->GetBridge();
	CPlateGirderApp *pGir   = pBrige->GetGirder(-1);
	CARoadOptionStd *pOpt   = m_pStd->GetDataManage()->GetOptionStd();

	CAPlateDrawJong DrawJong(m_pStd->GetDataManage());
	CAPlateDrawDimJong DrawDimJong(m_pStd->GetDataManage());	

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER);
	CPlateBasicIndex *pBxStt = NULL;
	CPlateBasicIndex *pBxEnd = NULL;

	if(m_nTab==0)
	{
		pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		
		if(pBrige->m_nQtyJigan==1)
			pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
		else
			pBxEnd = pGir->GetBxOnJijum(1);
	}
	else if(m_nTab==pBrige->m_nQtyJigan-1)
	{
		pBxStt = pGir->GetBxOnJijum(pBrige->m_nQtyJigan-1);
		pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	}
	else
	{
		pBxStt = pGir->GetBxOnJijum(m_nTab);
		pBxEnd = pGir->GetBxOnJijum(m_nTab+1);
	}

	if(!pBxStt || !pBxEnd) return;
	//
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);

	///< ġ�����׸���
	pGir->SetEleLevel(TRUE);

	CDomyun DomTemp(pDom);		
	DrawJong.DrawJDanGirderForInput(&DomTemp, pBxStt, pBxEnd);	
	DomTemp.SetCalcExtRect();
	CDRect BoundRect  = DomTemp.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 80*(dDiagonalL/20000);
	//
	Dom.SetScaleDim(dScale);

	DrawJong.DrawJDanGirderForInput(&Dom, pBxStt, pBxEnd);	

	pOpt->SetEnvType(&Dom, HCAD_DIML);
	DrawDimJong.DimJongTotal(&Dom, pBxStt, pBxEnd, DIM_BASE_TOTAL, TRUE, TRUE, 1);
	DrawDimJong.DimByunDanmyunJong(&Dom, pBxStt, pBxEnd, m_nTab, m_nByunDanType);

	pGir->SetEleLevel(FALSE);
	
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CByunDanmyunJewonDlg::SetGridTitle()
{
	long	nCol1 = 0,
			nCol2 = 0,
			nCol3 = 0;		

	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
//	m_Grid.SetTextBkColor(RGBEDITABLE);
	m_Grid.EnableReturnPass(TRUE);

	switch(m_nByunDanType)
	{
	case 0: m_Grid.SetRowCount(2);
			m_Grid.SetColumnCount(4);
			m_Grid.SetTextMatrix(0,1,"H1");
			m_Grid.SetTextMatrix(0,2,"H3");
			m_Grid.SetTextMatrix(0,3,"L1");
			nCol1 = 1;
			nCol2 = 2;
			nCol3 = 3;
			break;
	case 1:	m_Grid.SetRowCount(2);
			m_Grid.SetColumnCount(9);
			m_Grid.SetTextMatrix(0,1,"H1");
			m_Grid.SetTextMatrix(0,2,"H2");
			m_Grid.SetTextMatrix(0,3,"H3");
			m_Grid.SetTextMatrix(0,4,"L1");
			m_Grid.SetTextMatrix(0,5,"L2");
			m_Grid.SetTextMatrix(0,6,"L3");
			m_Grid.SetTextMatrix(0,7,"L4");
			m_Grid.SetTextMatrix(0,8,"L5");
			nCol1 = 1;
			nCol2 = 3;
			nCol3 = 6;
			break;
	case 2:
	case 3:	m_Grid.SetRowCount(2);
			m_Grid.SetColumnCount(6);
			m_Grid.SetTextMatrix(0,1,"H1");
			m_Grid.SetTextMatrix(0,2,"H3");
			m_Grid.SetTextMatrix(0,3,"L1");
			m_Grid.SetTextMatrix(0,4,"L2");
			m_Grid.SetTextMatrix(0,5,"L3");
			nCol1 = 1;
			nCol2 = 2;
			nCol3 = m_nByunDanType == 2 ? 3 : 5;
			break;
	case 4:	m_Grid.SetRowCount(2);
			m_Grid.SetColumnCount(7);
			m_Grid.SetTextMatrix(0,1,"H1");
			m_Grid.SetTextMatrix(0,2,"H2");
			m_Grid.SetTextMatrix(0,3,"H3");
			m_Grid.SetTextMatrix(0,4,"L1");
			m_Grid.SetTextMatrix(0,5,"L2");
			m_Grid.SetTextMatrix(0,6,"L3");
			nCol1 = 1;
			nCol2 = 3;
			nCol3 = 5;
			break;
	default:break;
	}

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 78);

	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);

	m_Grid.SetTextMatrix(0,0,"�� ��");
	m_Grid.SetTextMatrix(1,0,"����(mm)");

	long nCols = m_Grid.GetColumnCount();
	for(long nCol = 1;nCol < nCols;nCol++)
	{
		if( nCol == nCol1 || nCol == nCol2 || nCol == nCol3 )
		{
			m_Grid.SetItemState(1, nCol, m_Grid.GetItemState(1,nCol) | GVIS_READONLY);
			m_Grid.SetItemBkColour(1, nCol, RGBREADONLY);
		}		
	}	

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CByunDanmyunJewonDlg::SetDataInit()
{	
	CPlateBridge *pBrige = m_pStd->GetBridge();

	long nJigan = m_nTab;
	m_nByunDanType = pBrige->m_nStyleByun[nJigan];
	m_nArcType = pBrige->m_nByunCurveType[nJigan];	
	m_bUseByunDan = pBrige->m_cByunDan;
	if(m_nArcType < 0) m_nArcType = 0;
	UpdateData(FALSE);

	SetGridTitle();
		
	for(long col=1; col<m_Grid.GetColumnCount(); col++)
		m_Grid.SetTextMatrix(1,col,"0");

	double H1 = pBrige->GetGirder(0)->GetHeightGirderByJijum(nJigan,FALSE);
	double H2 = pBrige->GetGirder(0)->GetHeightGirderByJijum(nJigan+1,FALSE);
	double Len = pBrige->m_dLengthJigan[nJigan];

	switch(m_nByunDanType)
	{
	case 0:	// ���� ����	
		m_Grid.SetTextMatrix(1,1,"%.0lf",H1);	
		m_Grid.SetTextMatrix(1,2,"%.0lf",H2);
		m_Grid.SetTextMatrix(1,3,"%.0lf",Len);		
		break;
	case 1:	// ���ܸ� ���� 1
		m_Grid.SetTextMatrix(1,1,"%.0lf",H1);
		m_Grid.SetTextMatrix(1,2,"%.0lf",pBrige->m_dHeightByun[nJigan]);		
		m_Grid.SetTextMatrix(1,3,"%.0lf",H2);
		m_Grid.SetTextMatrix(1,4,"%.0lf",pBrige->m_dLenLeftByun[nJigan]);		
		m_Grid.SetTextMatrix(1,5,"%.0lf",pBrige->m_dLenLeftCurve[nJigan]);		
		m_Grid.SetTextMatrix(1,7,"%.0lf",pBrige->m_dLenRightCurve[nJigan]);		
		m_Grid.SetTextMatrix(1,8,"%.0lf",pBrige->m_dLenRightByun[nJigan]);		
		break;
	case 2:	// ���ܸ� ���� 2			
		m_Grid.SetTextMatrix(1,1,"%.0lf",H1);
		m_Grid.SetTextMatrix(1,2,"%.0lf",H2);
		m_Grid.SetTextMatrix(1,3,"%.0lf",pBrige->m_dLenLeftByun[nJigan]);		
		m_Grid.SetTextMatrix(1,4,"%.0lf",pBrige->m_dLenRightCurve[nJigan]);		
		m_Grid.SetTextMatrix(1,5,"%.0lf",pBrige->m_dLenRightByun[nJigan]);		
		break;
	case 3: // ���ܸ� ���� 3		
		m_Grid.SetTextMatrix(1,1,"%.0lf",H1);
		m_Grid.SetTextMatrix(1,2,"%.0lf",H2);
		m_Grid.SetTextMatrix(1,3,"%.0lf",pBrige->m_dLenLeftByun[nJigan]);		
		m_Grid.SetTextMatrix(1,4,"%.0lf",pBrige->m_dLenLeftCurve[nJigan]);		
		m_Grid.SetTextMatrix(1,5,"%.0lf",pBrige->m_dLenRightByun[nJigan]);		
		break;
	case 4: // ���ܸ� ���� 4		
		m_Grid.SetTextMatrix(1,1,"%.0lf",H1);
		m_Grid.SetTextMatrix(1,3,"%.0lf",H2);
		m_Grid.SetTextMatrix(1,2,"%.0lf",pBrige->m_dHeightByun[nJigan]);	
		m_Grid.SetTextMatrix(1,4,"%.0lf",pBrige->m_dLenLeftByun[nJigan]);	
//		m_Grid.SetTextMatrix(1,5,"%.0lf",pBrige->m_dLenLeftCurve[nJigan]);	
		m_Grid.SetTextMatrix(1,6,"%.0lf",pBrige->m_dLenRightByun[nJigan]);		
		break;
	}
	m_ctlTab.SetCurSel(m_nTab);
	UpdateData(FALSE);
	EnableByunDan(m_nTab);	
	CalcArcInfo();
	InKyungTotal();
}

void CByunDanmyunJewonDlg::OnButtonRecommend()
{
	if(MessageBox("������ �Է��� �����Ͱ� ��� �������ϴ�.\n ��� �Ͻðڽ��ϱ�",
				  "���ܸ� ���� �⺻��", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;

	SetDataDefault();
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView();
}

void CByunDanmyunJewonDlg::InKyungTotal()
{
	CPlateBridge *pBrige = m_pStd->GetBridge();

	long	col,cols = m_Grid.GetColumnCount();
	double  klength = pBrige->m_dLengthJigan[m_nTab],inlength = 0;
	CString str;
	switch(m_nByunDanType)
	{
	case 1: for(col = 4;col < cols;col++)
			{
				if(col !=6 )inlength += atof(m_Grid.GetItemText(1,col));
			}
			str.Format("%.0lf",klength - inlength);
			m_Grid.SetTextMatrix(1,6,str);
			break;
	case 2: inlength = atof(m_Grid.GetItemText(1,4)) + atof(m_Grid.GetItemText(1,5));
			str.Format("%.0lf",klength - inlength);
			m_Grid.SetTextMatrix(1,3,str);
			break;
	case 3: inlength = atof(m_Grid.GetItemText(1,3)) + atof(m_Grid.GetItemText(1,4));
			str.Format("%.0lf",klength - inlength);
			m_Grid.SetTextMatrix(1,5,str);
			break;
	case 4: inlength = atof(m_Grid.GetItemText(1,4)) + atof(m_Grid.GetItemText(1,6));
			str.Format("%.0lf",klength - inlength);
			m_Grid.SetTextMatrix(1,5,str);
			break;
	default:break;
	}
	BOOL b = FALSE;
	for(col = 1;col < cols ;col++)
	{
		if(m_Grid.GetItemText(1,col) == _T(""))
		{
			b = FALSE;
			break;
		}
		else b = TRUE;
	}
	//if(b == TRUE) CalcArcInfo();	

	//if(m_nByunCurveType == -1 && m_bByunDan) m_nByunCurveType = 0;
	//UpdateData(FALSE);

	m_Grid.SetRedraw(TRUE,TRUE);
}
// ���� ������ ���ܸ� ������ ��ġ ��Ų��(default ��)
void CByunDanmyunJewonDlg::SetDataDefault()
{	
	CPlateBridge *pBrige = m_pStd->GetBridge();			
	
	double H1 = pBrige->GetGirder(0)->GetHeightGirderByJijum(m_nTab,FALSE);
	double H2 = pBrige->GetGirder(0)->GetHeightGirderByJijum(m_nTab+1,FALSE);
	if(H1 != H2)	//�糡 ���� ���̰� ���� �ٸ� ���
	{
		m_bUseByunDan = TRUE;
		pBrige->m_cByunDan = m_bUseByunDan;

		if(m_nTab == 0)	//�������� ���, m_nStyleByun = 2 �� �⺻���� ��.
		{
			double Len = pBrige->m_dLengthJigan[m_nTab];
			pBrige->m_nStyleByun[m_nTab] = 2;
			pBrige->m_nByunCurveType[m_nTab] = 0;
			pBrige->m_dHeightByun[m_nTab] = H1;
			pBrige->m_dLenLeftByun[m_nTab] = Len - (1250+12500);
			pBrige->m_dLenRightByun[m_nTab] = 1250;
			pBrige->m_dLenLeftCurve[m_nTab] = 0;
			pBrige->m_dLenRightCurve[m_nTab]= 12500;
		}
		else if(m_nTab == pBrige->m_nQtyJigan-1)	//�������� ���, m_nStyleByun = 3 �� �⺻���� ��.
		{
			double Len = pBrige->m_dLengthJigan[m_nTab];
			pBrige->m_nStyleByun[m_nTab] = 3;
			pBrige->m_nByunCurveType[m_nTab] = 0;
			pBrige->m_dHeightByun[m_nTab] = H2;//H1;
			pBrige->m_dLenLeftByun[m_nTab] = 1250;
			pBrige->m_dLenRightByun[m_nTab] = Len - (1250+12500);
			pBrige->m_dLenLeftCurve[m_nTab] = 12500;
			pBrige->m_dLenRightCurve[m_nTab]= 0;
		}
		else //�߾� �����δ� ���ܸ��� ��� ���� ���̰� ���ƾ� ��.
		{
			MessageBox("�߾��������� �ܸ� ���̰� �߸��ԷµǾ����ϴ�.\n�ٽ��ѹ� �Է��� Ȯ�����ֽʽÿ�",
				"���ܸ� �Է�",MB_ICONINFORMATION);
			return;
		}
	}
	else if(pBrige->m_cByunDan == FALSE)	//��ü������ ���ܸ��� �ƴ� ���
	{
		for(long nJ = 0;nJ < pBrige->m_nQtyJigan; nJ++)
		{
			pBrige->m_nStyleByun[nJ]		= 0;
			pBrige->m_nByunCurveType[nJ] = 0;
			pBrige->m_dHeightByun[nJ]	= 0;
			pBrige->m_dLenLeftByun[nJ]	= 0;
			pBrige->m_dLenLeftCurve[nJ]	= 0;
			pBrige->m_dLenRightCurve[nJ]	= 0;
			pBrige->m_dLenRightByun[nJ]	= 0;
		}
		m_szArcInfo = "* ���� Ÿ��";
		return;
	}
	else if(H1 == H2)	//�糡 ���� ���̰� ���� ���� ���, m_nStyleByun = 1 �� �⺻���� ��.
	{
		pBrige->m_nStyleByun[m_nTab] = 1;
		pBrige->m_nByunCurveType[m_nTab] = 0;
		pBrige->m_dHeightByun[m_nTab] = RoundDown(H1*0.7, -2);
		pBrige->m_dLenLeftByun[m_nTab] = 1250;
		pBrige->m_dLenRightByun[m_nTab] = 1250;
		pBrige->m_dLenLeftCurve[m_nTab] = 12500;
		pBrige->m_dLenRightCurve[m_nTab]= 12500;
	}
}

void CByunDanmyunJewonDlg::SetDataSave()
{
	CPlateBridge *pBrige = m_pStd->GetBridge();

	long nJigan = m_nTab;	
	pBrige->m_nStyleByun[nJigan]     = m_nByunDanType;
	pBrige->m_nByunCurveType[nJigan] = m_nArcType;
	pBrige->m_cByunDan = m_bUseByunDan;
	
	switch(m_nByunDanType)
	{
	case 0:
		pBrige->m_dHeightByun[nJigan]    = m_Grid.GetTextMatrixDouble(1,2);
		pBrige->m_dLenLeftByun[nJigan]	 = m_Grid.GetTextMatrixDouble(1,3);
		pBrige->m_dLenLeftCurve[nJigan]  = 0;
		pBrige->m_dLenRightCurve[nJigan] = 0;
		pBrige->m_dLenRightByun[nJigan]  = 0;
		break;
	case 1:	// Ÿ�� 1
		pBrige->m_dHeightByun[nJigan]    = m_Grid.GetTextMatrixDouble(1,2);
		pBrige->m_dLenLeftByun[nJigan]   = m_Grid.GetTextMatrixDouble(1,4);
		pBrige->m_dLenLeftCurve[nJigan]  = m_Grid.GetTextMatrixDouble(1,5);
		pBrige->m_dLenRightCurve[nJigan] = m_Grid.GetTextMatrixDouble(1,7);
		pBrige->m_dLenRightByun[nJigan]  = m_Grid.GetTextMatrixDouble(1,8);
		break;
	case 2:		
		pBrige->m_dHeightByun[nJigan]    = m_Grid.GetTextMatrixDouble(1,1);
		pBrige->m_dLenLeftByun[nJigan]	 = m_Grid.GetTextMatrixDouble(1,3);
		pBrige->m_dLenLeftCurve[nJigan]  = 0;
		pBrige->m_dLenRightCurve[nJigan] = m_Grid.GetTextMatrixDouble(1,4);
		pBrige->m_dLenRightByun[nJigan]  = m_Grid.GetTextMatrixDouble(1,5);
		break;
	case 3:		
		pBrige->m_dHeightByun[nJigan]    = m_Grid.GetTextMatrixDouble(1,2);
		pBrige->m_dLenLeftByun[nJigan]   = m_Grid.GetTextMatrixDouble(1,3);
		pBrige->m_dLenLeftCurve[nJigan]  = m_Grid.GetTextMatrixDouble(1,4);
		pBrige->m_dLenRightCurve[nJigan] = 0;
		pBrige->m_dLenRightByun[nJigan]  = m_Grid.GetTextMatrixDouble(1,5);
		break;
	case 4:		
		pBrige->m_dHeightByun[nJigan]    = m_Grid.GetTextMatrixDouble(1,2);
		pBrige->m_dLenLeftByun[nJigan]   = m_Grid.GetTextMatrixDouble(1,4);
		pBrige->m_dLenLeftCurve[nJigan]  = m_Grid.GetTextMatrixDouble(1,5)/2;
		pBrige->m_dLenRightCurve[nJigan] = m_Grid.GetTextMatrixDouble(1,5)/2;
		pBrige->m_dLenRightByun[nJigan]  = m_Grid.GetTextMatrixDouble(1,6);
		break;
	}
}

void CByunDanmyunJewonDlg::EnableByunDan(long nTab)
{
	CPlateBridge *pBrige = m_pStd->GetBridge();
	double dSttHJijum = pBrige->GetGirder(0)->GetHeightGirderByJijum(nTab,FALSE);	
	double dEndHJijum = pBrige->GetGirder(0)->GetHeightGirderByJijum(nTab+1,FALSE);
	if(m_bUseByunDan)
	{
		m_btnType1.EnableWindow(TRUE);
		m_btnType2.EnableWindow(TRUE);
		m_btnType3.EnableWindow(TRUE);
		m_btnType4.EnableWindow(TRUE);
		if(dSttHJijum == dEndHJijum)
		{
			m_btnType2.EnableWindow(FALSE);
			m_btnType3.EnableWindow(FALSE);
		}
		if(dSttHJijum > dEndHJijum)
			m_btnType2.EnableWindow(FALSE);
		if(dSttHJijum < dEndHJijum)
			m_btnType3.EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CIRCLETYPE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CIRCLETYPE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CIRCLETYPE3)->EnableWindow(TRUE);
				
		CheckSelectedButton();
		UpdateData(FALSE);
	}
	else
	{	
		m_btnType1.SetState(FALSE);
		m_btnType2.SetState(FALSE);
		m_btnType3.SetState(FALSE);
		m_btnType4.SetState(FALSE);

		m_btnType1.EnableWindow(FALSE);
		m_btnType2.EnableWindow(FALSE);
		m_btnType3.EnableWindow(FALSE);
		m_btnType4.EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CIRCLETYPE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CIRCLETYPE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CIRCLETYPE3)->EnableWindow(FALSE);

		m_nArcType = -1;
		UpdateData(FALSE);

		m_nByunDanType = 0;
		SetDataDefault();
		DrawInputDomyunView(FALSE);
	}
}

void CByunDanmyunJewonDlg::CheckSelectedButton()
{
	CPlateBridge *pBrige = m_pStd->GetBridge();
	if(pBrige->m_nStyleByun[m_nTab]==1)	m_btnType1.SetState(TRUE);
	else								m_btnType1.SetState(FALSE);
	if(pBrige->m_nStyleByun[m_nTab]==2)	m_btnType2.SetState(TRUE);
	else								m_btnType2.SetState(FALSE);
	if(pBrige->m_nStyleByun[m_nTab]==3)	m_btnType3.SetState(TRUE);
	else								m_btnType3.SetState(FALSE);
	if(pBrige->m_nStyleByun[m_nTab]==4)	m_btnType4.SetState(TRUE);
	else								m_btnType4.SetState(FALSE);
	
}

void CByunDanmyunJewonDlg::OnSelchangeTabSel(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetDataSave();
	CPlateBridge *pBrige = m_pStd->GetBridge();
	m_nTab = m_ctlTab.GetCurSel();	
	double dSttHJijum = pBrige->GetGirder(0)->GetHeightGirderByJijum(m_nTab,FALSE);	
	double dEndHJijum = pBrige->GetGirder(0)->GetHeightGirderByJijum(m_nTab+1,FALSE);
	if(dSttHJijum != dEndHJijum)
	{
		m_bUseByunDan = TRUE;
		UpdateData(FALSE);
	}
	SetDataInit();	

	DrawInputDomyunView();	
			
	m_Grid.SetFocus();
	CGeneralBaseDlg::OnCursorChange(0, 0);
	OnCursorChange(m_Grid.GetFocusCell().row,m_Grid.GetFocusCell().col);
	Invalidate();
}

void CByunDanmyunJewonDlg::OnCheckUsebyun() 
{
	CPlateBridge *pBrige = m_pStd->GetBridge();
	long nJigan = m_nTab;
	CString str;
	double dSttHJijum = pBrige->GetGirder(0)->GetHeightGirderByJijum(nJigan,FALSE);	
	double dEndHJijum = pBrige->GetGirder(0)->GetHeightGirderByJijum(nJigan+1,FALSE);
	if(dSttHJijum != dEndHJijum)
	{
		str.Format("H1�� H3�� ������ �ٸ��Ƿ� ���ܸ� ó���� �����ؾ� �մϴ�.");
		if (IDOK == MessageBox(str, "�˸�", MB_OK | MB_ICONINFORMATION))
		{
			m_bUseByunDan = TRUE;
			UpdateData(FALSE);
			return;
		}
	}
	UpdateData();

	if(!m_bUseByunDan)
	{
		m_nByunDanType = 0;
		m_szArcInfo = "* ���� Ÿ��";
	}
	else
	{
		if(m_nByunDanType==0)
			m_nByunDanType = 1;
	}
	SetDataSave();
	SetDataInit();
}

void CByunDanmyunJewonDlg::OnButtonType1() 
{	
	m_nByunDanType = 1;
	SetDataDefaultByStyleByun();
	//SetDataSave();
	SetDataInit();
	CalcArcInfo();

	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::OnButtonType2() 
{	
	m_nByunDanType = 2;
	SetDataDefaultByStyleByun();
	SetDataSave();
	SetDataInit();
	CalcArcInfo();	

	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::OnButtonType3() 
{	
	m_nByunDanType = 3;	
	SetDataDefaultByStyleByun();
	//SetDataSave();
	SetDataInit();
	CalcArcInfo();

	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::OnButtonType4() 
{	
	m_nByunDanType = 4;	
	SetDataDefaultByStyleByun();
	//SetDataSave();
	SetDataInit();
	CalcArcInfo();

	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::CalcArcInfo()
{
	if(!m_bUseByunDan)	return;
	m_szArcInfo.Empty();
	double len		= 0,
			height	= 0,
			height1 = 0,
			chasu	= 0;
	switch(m_nByunDanType)
	{
//	case 0:
//		m_szArcInfo = "* ���� Ÿ��";
//		break;
	case 1:	
		height1 = atof(m_Grid.GetItemText(1,1));
		len = atof(m_Grid.GetItemText(1,5));
		height = atof(m_Grid.GetItemText(1,2));
		m_szArcInfo = "* Ÿ�� 1\n\n";
		break;
	case 2:
		height1 = atof(m_Grid.GetItemText(1,2));
		len = atof(m_Grid.GetItemText(1,4));
		height = atof(m_Grid.GetItemText(1,1));
		m_szArcInfo = "* Ÿ�� 2\n\n";
		break;
	case 3:
		height1 = atof(m_Grid.GetItemText(1,1));
		len = atof(m_Grid.GetItemText(1,4));
		height = atof(m_Grid.GetItemText(1,2));
		m_szArcInfo = "* Ÿ�� 3\n\n";
		break;
	case 4:	
		height1 = atof(m_Grid.GetItemText(1,1));
		len = atof(m_Grid.GetItemText(1,5))/2;
		height = atof(m_Grid.GetItemText(1,2));
		m_szArcInfo = "* Ÿ�� 4\n\n";
	}
		
	CString szType = "";
	if(m_nArcType == 0)
	{
		double h = height1 - height;
		if(h==0) chasu = 0;
		else	 chasu = (len*len + h*h)/fabs(h*2);
		szType.Format("* ������ R (mm) : %.4lf\n\n* ���� :  y = �� (R��- x��)",chasu);		
	}
	else if( m_nArcType == 1 )
	{
		if(len==0) chasu = 0;
		else 	 chasu = (height1-height)/(len*len);
		szType.Format("* ��� A (1/mm) : %.10lf\n\n* ���� :  y = A x��",chasu);		
	}
	else if( m_nArcType == 2 )
	{
		if(log10(len)==0) chasu = 0;
		else 	   chasu = log10(height1-height)/log10(len);
		szType.Format("* � ���� n : %.7lf\n\n* ���� :  y = x��",chasu);
		
	}	
	m_szArcInfo += szType;
	UpdateData(FALSE);
}

void CByunDanmyunJewonDlg::OnRadioCircletype1() 
{
	UpdateData();
	SetDataSave();
	CalcArcInfo();

//	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::OnRadioCircletype2() 
{
	UpdateData();
	SetDataSave();
	CalcArcInfo();

//	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::OnRadioCircletype3() 
{
	UpdateData();
	SetDataSave();
	CalcArcInfo();

//	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::OnCellChangedData(NMHDR* nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	CString strNew = m_Grid.GetItemText(nRow,nCol);		
	InKyungTotal();
	SetDataSave();
	CalcArcInfo();

	DrawInputDomyunView(FALSE);
}

void CByunDanmyunJewonDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);
	
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

BOOL CByunDanmyunJewonDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	CPlateBridgeApp *pBrigeApp = m_pStd->GetBridge();
	double dSttHJijum = pBrigeApp->GetGirder(0)->GetHeightGirderByJijum(0,FALSE);	
	double dEndHJijum = pBrigeApp->GetGirder(0)->GetHeightGirderByJijum(1,FALSE);
	if(dSttHJijum != dEndHJijum) m_bUseByunDan = TRUE;

	m_nArcType = pBrigeApp->m_nByunCurveType[m_nTab];	
	
	UpdateData(FALSE);
	EnableByunDan(0);
	return TRUE;
}

void CByunDanmyunJewonDlg::SetDataDefaultByStyleByun()
{
	CPlateBridge *pBrige = m_pStd->GetBridge();

	pBrige->m_nStyleByun[m_nTab]     = m_nByunDanType;
	pBrige->m_nByunCurveType[m_nTab] = m_nArcType;
	pBrige->m_cByunDan = m_bUseByunDan;

	double H1 = pBrige->GetGirder(0)->GetHeightGirderByJijum(m_nTab,FALSE);
	double H2 = pBrige->GetGirder(0)->GetHeightGirderByJijum(m_nTab+1,FALSE);
	double Len = pBrige->m_dLengthJigan[m_nTab];
	if(m_nByunDanType == 0)	//���� Ÿ���� ���
	{
		pBrige->m_dHeightByun[m_nTab] = H1;
		pBrige->m_dLenLeftByun[m_nTab] = Len;
		pBrige->m_dLenRightByun[m_nTab] = 0;
		pBrige->m_dLenLeftCurve[m_nTab] = 0;
		pBrige->m_dLenRightCurve[m_nTab]= 0;
	}
	else if(m_nByunDanType == 1)	//�糡 ���� ���̰� ���� ���� ���, m_nStyleByun = 1 �� �⺻���� ��.
	{
		pBrige->m_dHeightByun[m_nTab] = RoundDown(H1*0.7, -2);
		pBrige->m_dLenLeftByun[m_nTab] = 1250;
		pBrige->m_dLenRightByun[m_nTab] = 1250;
		pBrige->m_dLenLeftCurve[m_nTab] = 12500;
		pBrige->m_dLenRightCurve[m_nTab]= 12500;
	}
	else if(m_nByunDanType == 2)	//�������� ���, m_nStyleByun = 2 �� �⺻���� ��.
	{
		pBrige->m_dHeightByun[m_nTab] = H1;
		pBrige->m_dLenLeftByun[m_nTab] = Len - (1250+12500);
		pBrige->m_dLenRightByun[m_nTab] = 1250;
		pBrige->m_dLenLeftCurve[m_nTab] = 0;
		pBrige->m_dLenRightCurve[m_nTab]= 12500;			
	}
	else if(m_nByunDanType == 3)	//�������� ���, m_nStyleByun = 3 �� �⺻���� ��.
	{
		pBrige->m_dHeightByun[m_nTab] = H2;//H1;
		pBrige->m_dLenLeftByun[m_nTab] = 1250;
		pBrige->m_dLenRightByun[m_nTab] = Len - (1250+12500);
		pBrige->m_dLenLeftCurve[m_nTab] = 12500;
		pBrige->m_dLenRightCurve[m_nTab]= 0;
	}
	else if(m_nByunDanType == 4)	//�糡 ���� ���̰� ���� ���� ���, m_nStyleByun = 4 �� �⺻���� ��.
	{
		pBrige->m_dHeightByun[m_nTab] = RoundDown(H1*0.7, -2);
		pBrige->m_dLenLeftByun[m_nTab] = 1250;
		pBrige->m_dLenRightByun[m_nTab] = 1250;
		pBrige->m_dLenLeftCurve[m_nTab] = 0;
		pBrige->m_dLenRightCurve[m_nTab]= 0;
	}
}
