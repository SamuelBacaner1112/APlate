// DetVBeamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "DetVBeamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetVBeamDlg dialog


CDetVBeamDlg::CDetVBeamDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetVBeamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetVBeamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDetVBeamDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetVBeamDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetVBeamDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetVBeamDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetVBeamDlg message handlers

void CDetVBeamDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetVBeamDlg::SetGridTitle()
{
}
	
void CDetVBeamDlg::SetDataInit()
{
	SetGridTitle();
}

void CDetVBeamDlg::SetDataDefault()
{
}

void CDetVBeamDlg::SetDataSave()
{
}

void CDetVBeamDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,nCol));
}

BOOL CDetVBeamDlg::IsValid()
{
	return FALSE;
}