// ExpansionJointTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Generalinput.h"
#include "ExpansionJointTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpansionJointTypeDlg dialog

CExpansionJointTypeDlg::CExpansionJointTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExpansionJointTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpansionJointTypeDlg)
	m_nIndex	 = 0;	
	//}}AFX_DATA_INIT
	m_Grid.SetDefaultCharacterCount(2);
}


void CExpansionJointTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpansionJointTypeDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpansionJointTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CExpansionJointTypeDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpansionJointTypeDlg message handlers

BOOL CExpansionJointTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->SetWindowText(m_strDlgTitle);
	m_nIndex = GetExpansionJointType(m_szName);
	
	SetGridTitle();
	SetDataInit();
	SetDataSave();
	SetCellPaint(m_nIndex+2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExpansionJointTypeDlg::SetGridTitle()
{
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(EXPANSIONTYPE_SIZE+2);
	m_Grid.SetColumnCount(7);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(0, 90);
	m_Grid.SetColumnWidth(1, 100);
	
	for(long nRow=0; nRow<m_Grid.GetRowCount(); nRow++)
		for(long nCol=0; nCol<m_Grid.GetColumnCount(); nCol++)
			m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);

	m_Grid.SetTextMatrix(0, 0, "모델명");			m_Grid.SetMergeRow(0, 1, 0);
	m_Grid.SetTextMatrix(0, 1, "최대허용신축량");	m_Grid.SetMergeRow(0, 1, 1);
	m_Grid.SetTextMatrix(0, 2, "BLOCK OUT");		m_Grid.SetMergeCol(0, 2, 3);
	m_Grid.SetTextMatrix(0, 4, "유간");				m_Grid.SetMergeCol(0, 4, 6);
	m_Grid.SetTextMatrix(1, 2, "폭");
	m_Grid.SetTextMatrix(1, 3, "높이");
	m_Grid.SetTextMatrix(1, 4, "최소");
	m_Grid.SetTextMatrix(1, 5, "평균");
	m_Grid.SetTextMatrix(1, 6, "최대");

	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.ExpandColumnsToFit();
}

void CExpansionJointTypeDlg::SetDataInit()
{
	long nIdx = 0;
	for(long nRow=0; nRow<EXPANSIONTYPE_SIZE; nRow++)
	{
		m_Grid.SetTextMatrix(nRow+2, 0, ExpJointRef[nIdx]);
		m_Grid.SetTextMatrix(nRow+2, 1, ExpJointRef[nIdx+1]);
		m_Grid.SetTextMatrix(nRow+2, 2, ExpJointRef[nIdx+2]);
		m_Grid.SetTextMatrix(nRow+2, 3, ExpJointRef[nIdx+3]);
		m_Grid.SetTextMatrix(nRow+2, 4, ExpJointRef[nIdx+4]);
		m_Grid.SetTextMatrix(nRow+2, 5, ExpJointRef[nIdx+5]);
		m_Grid.SetTextMatrix(nRow+2, 6, ExpJointRef[nIdx+6]);
//		m_Grid.SetTextMatrix(nRow+2, 7, ExpJointRef[nRow+7]);
//		m_Grid.SetTextMatrix(nRow+2, 8, ExpJointRef[nRow+8]);
//		m_Grid.SetTextMatrix(nRow+2, 9, ExpJointRef[nRow+9]);
//		m_Grid.SetTextMatrix(nRow+2,10, ExpJointRef[nRow+10]);

		nIdx+=11;
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CExpansionJointTypeDlg::SetDataSave()
{
	long nCurRow	= m_Grid.GetFocusCell().row;
	if(nCurRow<1)	nCurRow = m_nIndex+2;
	m_szName			= m_Grid.GetTextMatrix(nCurRow, 0);
	m_dMaxAllowExpL		= atof(m_Grid.GetTextMatrix(nCurRow, 1));
	m_dBlockW			= atof(m_Grid.GetTextMatrix(nCurRow, 2));
	m_dBlockH			= atof(m_Grid.GetTextMatrix(nCurRow, 3));
	m_dUganMin			= atof(m_Grid.GetTextMatrix(nCurRow, 4));
	m_dUganAve			= atof(m_Grid.GetTextMatrix(nCurRow, 5));
	m_dUganMax			= atof(m_Grid.GetTextMatrix(nCurRow, 6));
}

void CExpansionJointTypeDlg::OnCellChangedRow(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)pNMHDR;

    long	nCurRow	= GridView->iRow;

	if(nCurRow>1)
	{
		SetCellPaint(nCurRow);
		m_Grid.SetRedraw(TRUE, TRUE);
		SetDataSave();
		m_nIndex = nCurRow-2;
	}
}

void CExpansionJointTypeDlg::SetCellPaint(long nIndex)
{
	long	nQtyCol	= m_Grid.GetColumnCount();

	for(long nRow=2; nRow<EXPANSIONTYPE_SIZE+2; nRow++)
	{
		for(long nCol=1; nCol<nQtyCol; nCol++)
		{
			if(nRow==nIndex)
			{
				COLORREF	color = 0xfffaf0;
				m_Grid.SetItemState(nRow, nCol, GVIS_MODIFIED);
				m_Grid.SetItemBkColour(nRow, nCol, color);
			}
			else
			{
				m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
			}
		}
	}
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CExpansionJointTypeDlg::OnOK() 
{
	SetDataSave();
	CDialog::OnOK();
}

long CExpansionJointTypeDlg::GetExpansionJointType(CString strExpansionJoint)
{
	long nIdx = 0;
	for(long nRow=0; nRow<EXPANSIONTYPE_SIZE; nRow++)
	{
		if(ExpJointRef[nIdx] == strExpansionJoint)
			return nRow;
		nIdx+=11;
	}

	return 0;
}
