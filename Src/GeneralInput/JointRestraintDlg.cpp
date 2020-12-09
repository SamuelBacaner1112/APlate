// JointRestraintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "JointRestraintDlg.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJointRestraintDlg dialog


CJointRestraintDlg::CJointRestraintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJointRestraintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJointRestraintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Grid.SetDefaultCharacterCount(2);
}


void CJointRestraintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJointRestraintDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDOK, m_btnOk);
}


BEGIN_MESSAGE_MAP(CJointRestraintDlg, CDialog)
	//{{AFX_MSG_MAP(CJointRestraintDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJointRestraintDlg message handlers

BOOL CJointRestraintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetGridTitle();
	SetDataInit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJointRestraintDlg::SetGridTitle()
{
	CAPlateCalcStd	*pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage	*pFemManage = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CString cs;

	int TotalNodeSu = pFemManage->GetJointSize();
	int Row = TotalNodeSu+1, Col=11;

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);

	m_Grid.SetRows(Row);
	m_Grid.SetCols(Col);

	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(1,70);
	m_Grid.SetColumnWidth(2,70);
	m_Grid.SetColumnWidth(3,70);

	m_Grid.SetTextMatrix(0,0,"ÀýÁ¡");
	m_Grid.SetTextMatrix(0,1,"X");
	m_Grid.SetTextMatrix(0,2,"Y");
	m_Grid.SetTextMatrix(0,3,"Z");
	m_Grid.SetTextMatrix(0,4,"Ux");
	m_Grid.SetTextMatrix(0,5,"Uy");
	m_Grid.SetTextMatrix(0,6,"Uz");
	m_Grid.SetTextMatrix(0,7,"Rx");
	m_Grid.SetTextMatrix(0,8,"Ry");
	m_Grid.SetTextMatrix(0,9,"Rz");
	m_Grid.SetTextMatrix(0,10,"Name");
	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.ExpandColumnsToFit();
}

void CJointRestraintDlg::SetDataInit()
{
	CAPlateCalcStd	*pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage	*pFemManage = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CString cs;

	long TotalNodeSu = pFemManage->GetJointSize();
	for(long i=0; i<TotalNodeSu; i++)
	{
		// Node reading
		CJoint* pJoint= pFemManage->GetJoint((unsigned short)i);

		cs.Format("%d",    pJoint->m_nIdx+1);		m_Grid.SetTextMatrix(i + 1,0,cs);
		cs.Format("%.4lf", toM(pJoint->m_vPoint.x));		m_Grid.SetTextMatrix(i + 1,1,cs);
		cs.Format("%.4lf", toM(pJoint->m_vPoint.y));		m_Grid.SetTextMatrix(i + 1,2,cs);
		cs.Format("%.4lf", toM(pJoint->m_vPoint.z));		m_Grid.SetTextMatrix(i + 1,3,cs);

		m_Grid.SetTextMatrix(i + 1, 4, pJoint->m_UX ? "1" : "0");
		m_Grid.SetTextMatrix(i + 1, 5, pJoint->m_UY ? "1" : "0");
		m_Grid.SetTextMatrix(i + 1, 6, pJoint->m_UZ ? "1" : "0");
		m_Grid.SetTextMatrix(i + 1, 7, pJoint->m_RX ? "1" : "0");
		m_Grid.SetTextMatrix(i + 1, 8, pJoint->m_RY ? "1" : "0");
		m_Grid.SetTextMatrix(i + 1, 9, pJoint->m_RZ ? "1" : "0");
		m_Grid.SetTextMatrix(i + 1,10,"");
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}