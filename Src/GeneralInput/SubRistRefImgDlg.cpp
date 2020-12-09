// SubRistRefImgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "SubRistRefImgDlg.h"
#include "RistModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubRistRefImgDlg dialog


CSubRistRefImgDlg::CSubRistRefImgDlg(long nType, CWnd* pParent /*=NULL*/)
	: CDialog(CSubRistRefImgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubRistRefImgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nType = nType;
	m_Grid.SetDefaultCharacterCount(2);
}


void CSubRistRefImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubRistRefImgDlg)
	DDX_Control(pDX, IDC_TAB_REFTYPE, m_ctlTab);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDOK, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CSubRistRefImgDlg, CDialog)
	//{{AFX_MSG_MAP(CSubRistRefImgDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_REFTYPE, OnSelchangeTabReftype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubRistRefImgDlg message handlers
void CSubRistRefImgDlg::SetGridTitleFlangeLength()
{
	m_Grid.SetColumnCount(14);		
	m_Grid.SetRowCount(12);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetEditable(FALSE);	

	m_Grid.SetColumnWidthAll(48);
	m_Grid.SetColumnWidth(0, 120);
	m_Grid.SetColumnWidth(1, 100);	
	
	m_Grid.SetTextMatrix(0,0,"구분");						m_Grid.SetMergeCol(0,0,1);
	m_Grid.SetTextMatrix(0,2,"중앙경간 (L1/a1)");			m_Grid.SetMergeCol(0,2,8);
	m_Grid.SetTextMatrix(0,9,"측경간  (L2/a2)");			m_Grid.SetMergeCol(0,9,13);

	m_Grid.SetTextMatrix(1,0,"강종");
	m_Grid.SetTextMatrix(1,1,"경간구성");

	CString szText;
	long nC = 0;
	for(nC=0; nC<7; nC++)
	{		
		szText.Format("%d",nC+1);
		m_Grid.SetTextMatrix(1,2+nC,szText);
	}
	for(nC=7; nC<7+6; nC++)
	{		
		szText.Format("%d",nC-6);
		m_Grid.SetTextMatrix(1,2+nC,szText);
	}
	
	m_Grid.SetTextMatrix(2,0,"SM520C-TMC");					m_Grid.SetMergeRow(2,6,0);
	m_Grid.SetTextMatrix(7,0,"SM570-TMC");					m_Grid.SetMergeRow(7,11,0);

	long nRow=0;
	for(long s=0; s<2; s++)
	{	
		for(long n=0; n<5; n++)
		{
			m_Grid.SetTextMatrix(nRow+2,1,CRistModule::m_gFlangeWidthTitle[s][n]);
			for(long m=0; m<12; m++)
			{			
				if(CRistModule::m_gFlangeLengthValue[s][n][m]==0)				
					m_Grid.SetItemBkColour(nRow+2,m+2,RGBREADONLY);				
				else
					m_Grid.SetTextMatrixFormat(nRow+2,m+2,"%.3f",CRistModule::m_gFlangeLengthValue[s][n][m]);				
			}
			++nRow;
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);	
	m_Grid.ExpandToFit();
}

void CSubRistRefImgDlg::SetGridTitleFlangeThick()
{
	m_Grid.SetColumnCount(14);		
	m_Grid.SetRowCount(12);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetEditable(FALSE);

	m_Grid.SetColumnWidthAll(48);
	m_Grid.SetColumnWidth(0, 120);
	m_Grid.SetColumnWidth(1, 100);	
	
	m_Grid.SetTextMatrix(0,0,"구분");						m_Grid.SetMergeCol(0,0,1);
	m_Grid.SetTextMatrix(0,2,"중앙경간 (L1/a1)");			m_Grid.SetMergeCol(0,2,8);
	m_Grid.SetTextMatrix(0,9,"측경간  (L2/a2)");			m_Grid.SetMergeCol(0,9,13);

	m_Grid.SetTextMatrix(1,0,"강종");
	m_Grid.SetTextMatrix(1,1,"경간구성");

	CString szText;
	long nC = 0;
	for(nC=0; nC<7; nC++)
	{		
		szText.Format("%d",nC+1);
		m_Grid.SetTextMatrix(1,2+nC,szText);
	}
	for(nC=7; nC<7+6; nC++)
	{		
		szText.Format("%d",nC-6);
		m_Grid.SetTextMatrix(1,2+nC,szText);
	}
	
	m_Grid.SetTextMatrix(2,0,"SM520C-TMC");					m_Grid.SetMergeRow(2,6,0);
	m_Grid.SetTextMatrix(7,0,"SM570-TMC");					m_Grid.SetMergeRow(7,11,0);

	long nRow=0;	
	for(long s=0; s<2; s++)
	{	
		for(long n=0; n<5; n++)
		{
			m_Grid.SetTextMatrix(nRow+2,1,CRistModule::m_gFlangeWidthTitle[s][n]);
			for(long m=0; m<12; m++)
			{	
				if(m_ctlTab.GetCurSel()==0)
				{
					if(CRistModule::m_gFlangeUpperThickValue[s][n][m]==0)				
						m_Grid.SetItemBkColour(nRow+2,m+2,RGBREADONLY);				
					else
					{
						m_Grid.SetItemBkColour(nRow+2,m+2,RGB(225, 250, 250));	
						m_Grid.SetTextMatrixFormat(nRow+2,m+2,"%.0f",CRistModule::m_gFlangeUpperThickValue[s][n][m]);				
					}
				}
				else if(m_ctlTab.GetCurSel()==1)
				{
					if(CRistModule::m_gFlangeLowerThickValue[s][n][m]==0)				
						m_Grid.SetItemBkColour(nRow+2,m+2,RGBREADONLY);				
					else
					{
						m_Grid.SetItemBkColour(nRow+2,m+2,RGB(225, 250, 250));	
						m_Grid.SetTextMatrixFormat(nRow+2,m+2,"%.0f",CRistModule::m_gFlangeLowerThickValue[s][n][m]);				
					}
				}
				else
				{
					if(CRistModule::m_gWebThickValue[s][n][m]==0)										
						m_Grid.SetItemBkColour(nRow+2,m+2,RGBREADONLY);				
					else
					{
						m_Grid.SetItemBkColour(nRow+2,m+2,RGB(225, 250, 250));	
						m_Grid.SetTextMatrixFormat(nRow+2,m+2,"%.0f",CRistModule::m_gWebThickValue[s][n][m]);				
					}
				}
			}
			++nRow;
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);	
	m_Grid.ExpandToFit();
}

void CSubRistRefImgDlg::SetGridTitleFlangeWidth()
{	
	m_Grid.SetColumnCount(4);		
	m_Grid.SetRowCount(12);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetEditable(FALSE);

	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 200);
	m_Grid.SetColumnWidth(2, 100);
	m_Grid.SetColumnWidth(3, 100);
	
	m_Grid.SetTextMatrix(0,0,"강종");						m_Grid.SetMergeRow(0,1,0);
	m_Grid.SetTextMatrix(0,1,"경간구성");					m_Grid.SetMergeRow(0,1,1);
	m_Grid.SetTextMatrix(0,2,"플랜지폭 공식 : B=(K a-C)");  m_Grid.SetMergeCol(0,2,3);
	m_Grid.SetTextMatrix(1,2,"K");							
	m_Grid.SetTextMatrix(1,3,"C");							

	m_Grid.SetTextMatrix(2,0,"SM570-TMC");					m_Grid.SetMergeRow(2,6,0);//순서가 바뀌어 있음.. 수정 ..070125..KB
	m_Grid.SetTextMatrix(7,0,"SM520C-TMC");					m_Grid.SetMergeRow(7,11,0);
	
	long nRow=0;
	for(long s=0; s<2; s++)
	{	
		for(long n=0; n<5; n++)
		{
			m_Grid.SetTextMatrix(nRow+2,1,CRistModule::m_gFlangeWidthTitle[s][n]);
			m_Grid.SetTextMatrixFormat(nRow+2,2,"%g",CRistModule::m_gFlangeWidthValue[s][0][n]);
			m_Grid.SetTextMatrixFormat(nRow+2,3,"%g",CRistModule::m_gFlangeWidthValue[s][1][n]);
			++nRow;
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);	
	m_Grid.ExpandToFit();
}

void CSubRistRefImgDlg::SetDataInit()
{
	switch(m_nType)
	{
	case 0:	SetGridTitleFlangeLength(); break;
	case 1:	SetGridTitleFlangeThick(); break;
	case 2:	SetGridTitleFlangeWidth(); break;
	}
}

BOOL CSubRistRefImgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	switch(m_nType)
	{
	case 0: m_ctlTab.InsertItem(0,"구간길이"); break;
	case 1: m_ctlTab.InsertItem(0, "상부플랜지 두께"); m_ctlTab.InsertItem(1, "하부플랜지 두께"); m_ctlTab.InsertItem(2, "복부판 두께"); break;
	case 2: m_ctlTab.InsertItem(0, "플랜지 폭원"); break;
	}
	SetDataInit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSubRistRefImgDlg::OnSelchangeTabReftype(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataInit();
	
	*pResult = 0;
}
