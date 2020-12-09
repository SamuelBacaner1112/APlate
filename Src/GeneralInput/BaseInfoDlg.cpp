// BaseInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseInfoDlg dialog


CBaseInfoDlg::CBaseInfoDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CBaseInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBaseInfoDlg)
	//}}AFX_DATA_INIT
}

void CBaseInfoDlg::DoDataExchange(CDataExchange* pDX) 
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseInfoDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseInfoDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CBaseInfoDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA	, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseInfoDlg message handlers
void CBaseInfoDlg::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetGridTitle();
}

void CBaseInfoDlg::SetGridTitle()
{
	long nBaseRow = 11;
	long nBaseCol = 6;
	m_Grid.ResetAttrib();
	m_Grid.SetRowCount(nBaseRow);
	m_Grid.SetColumnCount(nBaseCol);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0,150);
	m_Grid.SetColumnWidth(3,150);
	for(long nRow=0; nRow<nBaseRow; nRow++)
		m_Grid.SetCellFixType(nRow, 0, GVN_ONECELL_COLFIX);
	for(long nCol=0; nCol<nBaseCol; nCol++)
		m_Grid.SetCellFixType(0, nCol, GVN_ONECELL_COLFIX);

	m_Grid.SetTextMatrix(0, 0, "�׸�");
	m_Grid.SetTextMatrix(0, 1, "����");				m_Grid.SetMergeCol(0, 1, 5);
	m_Grid.SetTextMatrix(1, 0, "�����");			m_Grid.SetMergeCol(1, 1, 5);
//	m_Grid.SetItemState(1, 1, GVIS_READONLY);
	m_Grid.SetTextMatrix(2, 0, "������");			m_Grid.SetMergeCol(2, 1, 5);
	m_Grid.SetTextMatrix(3, 0, "����ó");			m_Grid.SetMergeCol(3, 1, 5);
	m_Grid.SetTextMatrix(4, 0, "�ð���");			m_Grid.SetMergeCol(4, 1, 5);
	m_Grid.SetTextMatrix(5, 0, "���ۻ�");			m_Grid.SetMergeCol(5, 1, 5);
	m_Grid.SetTextMatrix(6, 0, "�����");			m_Grid.SetMergeCol(6, 1, 5);
	m_Grid.SetTextMatrix(7, 0, "�����");			m_Grid.SetMergeCol(7, 1, 5);
	m_Grid.SetTextMatrix(8, 0, "����Station(m)");	m_Grid.SetMergeCol(8, 1, 5);
	m_Grid.SetTextMatrix(9, 0, "���������(m)");	m_Grid.SetMergeCol(9, 1, 2);	m_Grid.SetCellFixType(9, 3, GVN_ONECELL_COLFIX);
	m_Grid.SetTextMatrix(9, 3, "������");			m_Grid.SetMergeCol(9, 4, 5);
	m_Grid.SetTextMatrix(10, 0, "������");			
	m_Grid.SetRadioAndRange(10, 1, 10, 1, 10, 2);	m_Grid.SetRadioAndRange(10, 2, 10, 1, 10, 2);
	m_Grid.SetTextMatrix(10, 1, "����");			m_Grid.SetTextMatrix(10, 2, "����");	m_Grid.SetCellFixType(10, 3, GVN_ONECELL_COLFIX);
	m_Grid.SetTextMatrix(10, 3, "������");
	m_Grid.SetRadioAndRange(10, 4, 10, 4, 10, 5);	m_Grid.SetRadioAndRange(10, 5, 10, 4, 10, 5);
	m_Grid.SetTextMatrix(10, 4, "����");			m_Grid.SetTextMatrix(10, 5, "����");

	m_Grid.SetRedraw(TRUE, TRUE);	
}

BOOL CBaseInfoDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBaseInfoDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
	CAPlateDrawDimPyung DrawDimPyung(m_pStd->GetDataManage());

	if( m_pStd->GetLineInfoApp()->IsValidLineInuptData()==FALSE) 
	{
//		MessageBox("���� ������ �����ϴ�.\n��������/���� ���� �������� �޴��� ���� �����Ͻʽÿ�.");
		return;
	}

	CDomyun	*pDom	= m_pView->GetDomyun();
	CDomyun	Dom(pDom);

	CString csErrorMsg(_T(""));
	if(m_pStd->GetLineInfoApp()->SetDataIp(csErrorMsg) )  // m_prjIP ������ ����
	{
		Dom.SetScaleDim(1);
		m_pView->GetDomyun()->ClearEtt(TRUE);
		if(m_nTab==0)	// ��鵵
		{
			double dLenClothoidLine = m_pStd->GetLineInfoApp()->m_dLenUnitClothoid;
			m_pStd->GetLineInfoApp()->DrawLineOne(&Dom, 0, (long)dLenClothoidLine);

			// Slab �׸���
			CPlateBxFinder FinderSlabL(pDB->GetGirder(0));
			CPlateBasicIndex *pBxLeftStt = FinderSlabL.GetBxFirst(BX_STT_GIRDER);
			CPlateBasicIndex *pBxLeftEnd = FinderSlabL.GetBxFirst(BX_END_GIRDER);
			CPlateBxFinder FinderSlabR(pDB->GetGirder(pDB->GetGirdersu()-1));
			CPlateBasicIndex *pBxRightStt = FinderSlabR.GetBxFirst(BX_STT_GIRDER);
			CPlateBasicIndex *pBxRightEnd = FinderSlabR.GetBxFirst(BX_END_GIRDER);
			if(pBxLeftStt && pBxLeftEnd && pBxRightStt && pBxRightEnd)
				DrawPyung.DrawSlabByBx(&Dom, pBxLeftStt, pBxLeftEnd, pBxRightStt, pBxRightEnd);
			else
				DrawPyung.DrawSlabVirtual(&Dom);

			DrawDimPyung.DimXyCenter(&Dom);
		}
		else	// ���ܸ鵵		
		{
			DrawJongDan();
		}
	}
	if(csErrorMsg!="") MessageBox(csErrorMsg);	

	*pDom << Dom;
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CBaseInfoDlg::DrawJongDan()
{
	if(m_pStd->GetBridge()->GetLengthBridge() <= 0)
	{
		m_pView->GetDomyun()->SetLineColor(1);
		m_pView->GetDomyun()->SetTextColor(4);
		m_pView->GetDomyun()->SetTextHeight(m_pStd->GetLineInfoApp()->GetTextHeightIp());

		m_pStd->GetLineInfoApp()->DrawJongDan(m_pView->GetDomyun()); // ���� ����
	}
	else
		DrawJongGuBaeDo();

}

void CBaseInfoDlg::DrawJongGuBaeDo()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	double dStaStt = 0;
	double dStaEnd = 0;

	double dStaBridgeStt = pDB->m_dStationBridgeStt;
	double dStaBridgeEnd = dStaBridgeStt + pDB->GetLengthBridge();

	BOOL bUser = FALSE;

	m_pView->GetDomyun()->SetScaleDim(1);
	m_pView->GetDomyun()->SetTextHeight(m_pStd->GetLineInfoApp()->GetTextHeightIp());

	m_pStd->GetLineInfoApp()->DrawJongDan(m_pView->GetDomyun());

	m_pView->GetDomyun()->SetLineColor(7);
	m_pStd->GetLineInfoApp()->DrawJongDanCurveOnlyBridge(m_pView->GetDomyun(), dStaStt, dStaEnd, dStaBridgeStt, dStaBridgeEnd, 5, bUser);

	m_pView->GetDomyun()->SetLineColor(2);
	m_pView->GetDomyun()->SetTextColor(4);
	m_pStd->GetLineInfoApp()->DrawJongDanBridgeTextOut(m_pView->GetDomyun(), dStaStt, dStaEnd, dStaBridgeStt, dStaBridgeEnd, 5, bUser);
}


void CBaseInfoDlg::SetDataInit()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CString	strText	= _T("");
	m_Grid.SetTextMatrix(1, 1, m_pStd->GetLineInfoApp()->m_sNameProject, (UINT)DT_LEFT);	// �����
	m_Grid.SetTextMatrix(2, 1, pDB->m_sNameBridge, (UINT)DT_LEFT);			// ������
	m_Grid.SetTextMatrix(3, 1, pDB->m_sNameOwner, (UINT)DT_LEFT);			// ����ó
	m_Grid.SetTextMatrix(4, 1, pDB->m_sNameConstructor, (UINT)DT_LEFT);		// �ð���
	m_Grid.SetTextMatrix(5, 1, pDB->m_sNameAssembler, (UINT)DT_LEFT);		// ���ۻ�
	m_Grid.SetTextMatrix(6, 1, pDB->m_sNameSite, (UINT)DT_LEFT);			// �����
	m_Grid.SetTextMatrix(7, 1, pDB->m_sNameEngineering, (UINT)DT_LEFT);		// �����
	strText.Format("%.6lf",toM(pDB->m_dStationBridgeStt));					// ���� Station
	m_Grid.SetTextMatrix(8, 1, strText, (UINT)DT_LEFT);
	strText.Format("%.6lf",toM(pDB->m_dLengthTotalSlab));					// ������ ����
	m_Grid.SetTextMatrix(9, 1, strText, (UINT)DT_LEFT);
	strText.Format("%d", pDB->m_nQtyJigan);
	m_Grid.SetTextMatrix(9, 4, strText, (UINT)DT_LEFT);						// ������
	m_Grid.SetCheck(10, 1, pDB->m_nTypeAbutOrPier[STT_POS]==0?TRUE:FALSE);	// ���� ����
	m_Grid.SetCheck(10, 2, pDB->m_nTypeAbutOrPier[STT_POS]!=0?TRUE:FALSE);	// ���� ����
	m_Grid.SetCheck(10, 4, pDB->m_nTypeAbutOrPier[END_POS]==0?TRUE:FALSE);	// ���� ����
	m_Grid.SetCheck(10, 5, pDB->m_nTypeAbutOrPier[END_POS]!=0?TRUE:FALSE);	// ���� ����
	m_Grid.SetRedraw(TRUE, TRUE);
	UpdateData(FALSE);
}

void CBaseInfoDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	
	pDB->m_sNameBridge       = "�׽�Ʈ ����";	
	pDB->m_sNameEngineering  = "�ѱ濣���Ͼ";
	pDB->m_dStationBridgeStt = max(m_pStd->GetLineInfoApp()->m_dStationBp, 0) + 210000.0;	
	if( m_pStd->GetBridge()->m_nLineType == 1 )//����
		pDB->m_dStationBridgeStt += 800000.0;

	if(pDB->IsPlateGir())
	{
		pDB->m_dLengthTotalSlab  = 100000.0;
		pDB->m_nQtyJigan         = 2;
	}
	else if(pDB->IsSosuGir())
	{
		pDB->m_dLengthTotalSlab  = 130000.0;
		pDB->m_nQtyJigan         = 3;
	}
	else	// TU
	{
		pDB->m_dLengthTotalSlab  = 50000.0;
		pDB->m_nQtyJigan         = 1;
	}
	pDB->m_bQuickLineSearch = TRUE;
	
	pDB->m_nTypeAbutOrPier[STT_POS] = 0;
	pDB->m_nTypeAbutOrPier[END_POS] = 0;
}

void CBaseInfoDlg::SetDataSave()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	UpdateData(TRUE);

	CString strText	= _T("");
	m_pStd->GetLineInfoApp()->m_sNameProject = m_Grid.GetTextMatrix(1, 1);
	pDB->m_sNameBridge			= m_Grid.GetTextMatrix(2, 1);
	pDB->m_sNameOwner			= m_Grid.GetTextMatrix(3, 1);
	pDB->m_sNameConstructor		= m_Grid.GetTextMatrix(4, 1);
	pDB->m_sNameAssembler		= m_Grid.GetTextMatrix(5, 1);
	pDB->m_sNameSite			= m_Grid.GetTextMatrix(6, 1);
	pDB->m_sNameEngineering		= m_Grid.GetTextMatrix(7, 1);
	pDB->m_dStationBridgeStt	= frM(m_Grid.GetTextMatrixDouble(8, 1));
	pDB->m_dLengthTotalSlab		= frM(m_Grid.GetTextMatrixDouble(9, 1));
	pDB->m_nQtyJigan			= m_Grid.GetTextMatrixLong(9, 4);
	pDB->m_nTypeAbutOrPier[STT_POS] = m_Grid.IsCheck(10, 1)? 0 : 1;
	pDB->m_nTypeAbutOrPier[END_POS] = m_Grid.IsCheck(10, 4)? 0 : 1;
	pDB->SetGirderSearchIP();
}

BOOL CBaseInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray	strArray;
	UpdateData(TRUE);
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		long nRow = next.row;
		long nCol = next.col;
		if(nRow==10 && m_Grid.GetCellType(nRow,nCol)==CT_COMBO)
		{
			strArray.Add("����");
			strArray.Add("����");
			m_Grid.SetComboString(strArray);
		}
	}
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CBaseInfoDlg::OnCellChangedData(NMHDR* nmgv,LRESULT*)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CLineInfoApp	*pLine	= m_pStd->GetLineInfoApp();

// 	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
// 	int nRow = GridView->iRow;
//     int nCol = GridView->iColumn;
	CString strText = "";				

	//������ �Է� ���� üũ...
	double staStt = m_Grid.GetTextMatrixDouble(8, 1);
	double endStt = toM(pLine->GetStationEpLine());

	if(staStt < toM(pLine->m_dStationBp) )
	{
		AfxMessageBox("������ ������ ������ ���� ���� �۽��ϴ�.\n�ٽ� Ȯ���� �ֽʽÿ�.", MB_OK | MB_ICONSTOP);
		strText.Format("%.6lf",toM(pDB->m_dStationBridgeStt));								// ���� Station
		m_Grid.SetTextMatrix(8, 1, strText, (UINT)DT_LEFT);		
	}
	else if(staStt > endStt)
	{
		AfxMessageBox("������ ������ ������ ���� ���� Ů�ϴ�.\n�ٽ� Ȯ���� �ֽʽÿ�.", MB_OK | MB_ICONSTOP);
		strText.Format("%.6lf",toM(pDB->m_dStationBridgeStt));								// ���� Station
		m_Grid.SetTextMatrix(8, 1, strText, (UINT)DT_LEFT);	
	}else if(staStt != toM(pDB->m_dStationBridgeStt))      //���� �����̼��� �����Ǿ��ٸ�,
	{
		double staSttGap = frM(staStt) - pDB->m_dStationBridgeStt;
		for(long nG = -1; nG < pDB->GetGirdersu(); nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			CPtrList& ptList = pGir->GetBxList();
			POSITION pos = ptList.GetHeadPosition();
			while(pos)
			{
				CPlateBasicIndex *pBx = (CPlateBasicIndex *)ptList.GetNext(pos);
				pBx->SetStation(pBx->GetStation() + staSttGap);
			}
		}
		SetDataSave();
	}

	//������ üũ
	if(m_Grid.GetTextMatrixDouble(9, 1)<=0)
	{		
		AfxMessageBox("�������� ���̴� '0' ���� �������� �����ϴ�.\n�ٽ� Ȯ���� �ֽʽÿ�.", MB_OK | MB_ICONSTOP);
		strText.Format("%.6lf",toM(pDB->m_dLengthTotalSlab));								// ������ ����
		m_Grid.SetTextMatrix(9, 1, strText, (UINT)DT_LEFT);
	}
	
	//���� üũ..
	if(m_Grid.GetTextMatrixLong(9, 4)>KYUNGGANSUMAX)
	{	strText.Format("%s���� ���ϱ��� �Է°����մϴ�.", KYUNGGANSUMAX);
		AfxMessageBox(strText, MB_OK | MB_ICONSTOP);
		strText.Format("%d", pDB->m_nQtyJigan);
		m_Grid.SetTextMatrix(9, 4, strText, (UINT)DT_LEFT);
	}

	SetDataSave();
	m_Grid.SetRedraw(TRUE, TRUE);
}
