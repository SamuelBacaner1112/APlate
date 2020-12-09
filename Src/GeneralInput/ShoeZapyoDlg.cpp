// ShoeZapyoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "ShoeZapyoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShoeZapyoDlg dialog


CShoeZapyoDlg::CShoeZapyoDlg(CWnd*pParent /*=NULL*/)
	: CXTResizeDialog(CShoeZapyoDlg::IDD, pParent)
{
		// NOTE: the ClassWizard will add member initialization here
	//{{AFX_DATA_INIT
	//}}AFX_DATA_INIT(CShoeZapyoDlg)
}


void CShoeZapyoDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShoeZapyoDlg)
	DDX_GridControl(pDX, IDC_GRID_SHOEZAPYO, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SHOE_FILEOUT, m_btnOutExcel);
	DDX_Control(pDX, IDC_SHOE_PRINT, m_btnOutPrint);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
}

BEGIN_MESSAGE_MAP(CShoeZapyoDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CShoeZapyoDlg)
	ON_BN_CLICKED(IDC_SHOE_FILEOUT, OnShoeFileout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShoeZapyoDlg message handlers

BEGIN_EVENTSINK_MAP(CShoeZapyoDlg, CXTResizeDialog)
    //{{AFX_EVENTSINK_MAP(CShoeZapyoDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CShoeZapyoDlg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	SetGrid();
	SetGridAlign();
	SetGridText();

	m_Grid.AutoSizeColumns();
	m_Grid.SetRedraw(TRUE,TRUE);

	return TRUE;
}

void CShoeZapyoDlg::SetGrid()			//hw0918 start
{
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)m_pDB;

	long	l,
			cols = (pDB->m_nQtyJigan+1)*2 + 1, // 5 �̻���
			rows = 0,			// ���� �� 1�� ���� , ���� ���� ���� ���� �� ������ ǥ���ϴ� ����
			maxRows = 0,			//�ְ� ��
			nGsu = pDB->GetGirdersu(), // �� �Ŵ��� ��
			nG = 0,
			Jigansu = pDB->m_nQtyJigan-1;

	for(long i=0; i< 2; i++) {		//A1, A2�� ���� ������ ����.
		for(nG = 0; nG < nGsu; nG++)
			rows++;
		if(maxRows < rows) maxRows = rows;
		rows = 0;
	}

	for(long j=0; j<Jigansu; j++) {		//P1 ~ Pn�� ���� ������ ����.
		for(nG = 0; nG < nGsu; nG++)
			rows++;
		if(maxRows < rows) maxRows = rows;
		rows = 0;
	}

	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));

	rows = maxRows + 1;
	m_Grid.SetColumnCount(cols);
	m_Grid.SetRowCount(rows);

	m_Grid.SetTextMatrix(0,0,"����");
	m_Grid.SetTextMatrix(0,1,((CPlateBridgeApp *)m_pDB)->m_strJijumName[0] + "(x)");
	m_Grid.SetTextMatrix(0,2,((CPlateBridgeApp *)m_pDB)->m_strJijumName[0] + "(y)");
	
	CString csFmt;
	for(l=0;l<Jigansu;l++)
	{
		csFmt = ((CPlateBridgeApp *)m_pDB)->m_strJijumName[l+1] + "(x)";
		m_Grid.SetTextMatrix(0,3+l*2,csFmt);
		csFmt = ((CPlateBridgeApp *)m_pDB)->m_strJijumName[l+1] + "(y)";
		m_Grid.SetTextMatrix(0,4+l*2,csFmt);
	}
	m_Grid.SetTextMatrix(0,3+l*2,((CPlateBridgeApp *)m_pDB)->m_strJijumName[l+1] + "(x)");
	m_Grid.SetTextMatrix(0,4+l*2,((CPlateBridgeApp *)m_pDB)->m_strJijumName[l+1] + "(y)");


	for(l=1;l<rows;l++)
	{
		csFmt.Format("%ld",l);
		m_Grid.SetTextMatrix(l,0,csFmt);
	}	
}							//hw0918 end

void CShoeZapyoDlg::SetGridAlign()
{
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0,50);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
}
// �׸��忡 �� ��ǥ �� ����ϴ� �Լ�
// ��ü �Ŵ��� A1, P1,... A2�� ����� (ó�� ����ϴ� ���� A1�� 1,2,3,... x,y ��ǥ��)
void CShoeZapyoDlg::SetGridText()
{
	CPlateBridgeApp*	pDB = (CPlateBridgeApp*)m_pDB;

	long	ShoeQuangityForWidth = 0,
			nJijum = 0,
			s = 0,
			nQtyJijum = pDB->m_nQtyJigan+1;
	CDPoint xy;
	CString str;

	for(nJijum=0; nJijum<nQtyJijum; nJijum++)
	{
		ShoeQuangityForWidth = pDB->GetQtyShoeOnJijum(nJijum);
		for(s=0; s<ShoeQuangityForWidth; s++)
		{
			xy = pDB->GetXyShoe(s, nJijum);
			str.Format("%.1f",xy.y);
			m_Grid.SetTextMatrix(s+1, (nJijum*2)+1, str);
			str.Format("%.1f",xy.x);
			m_Grid.SetTextMatrix(s+1, (nJijum*2)+2, str);
		}
	}
}



void CShoeZapyoDlg::OnShoeFileout() 
{		
	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel ���� ���� (*.xls)|*.xls|������� (*.*)|*.*||") : _T("Microsoft Excel ���� ���� (*.xlsx)|*.xlsx|������� (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	dlg.m_ofn.lpstrTitle = "���� ��������";

	if(dlg.DoModal()==IDOK)
	{
		CMsgDlg xlDlg;
		xlDlg.AddGridCtrl(&m_Grid);
		xlDlg.SetSaveFileName(dlg.GetPathName());
		xlDlg.AddSaveSheetName("����ǥ��");
		xlDlg.SetCmdType(CMD_SAVE);
		xlDlg.Run(this);
	}
}
