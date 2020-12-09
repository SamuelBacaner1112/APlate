// ShearBarTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "ShearBarTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShearBarTypeDlg dialog


CShearBarTypeDlg::CShearBarTypeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CShearBarTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearBarTypeDlg)
	m_nRadio = -1;
	//}}AFX_DATA_INIT
}


void CShearBarTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearBarTypeDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShearBarTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CShearBarTypeDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShearBarTypeDlg message handlers

BOOL CShearBarTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nType = atoi(m_ReturnValue);
	if(nType < 0 || nType > 3)
		nType = 0;
	SetType(nType);
	m_nRadio = nType - 1;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShearBarTypeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CBitmap bitmap[3];
	CDC memDC;
	//현재 DC와 호환적인 메모리 DC를 만든다.
	memDC.CreateCompatibleDC(&dc);	
	int nWidth = 118;
	int nHeight = 128;
	int nStt = 20;
	int nSpace = 10;

	//1. 
	bitmap[0].LoadBitmap(IDB_BITMAP_SHEAR1);	
	//메모리 DC에 비트맵을 설정한다.
	memDC.SelectObject(&bitmap[0]);
	dc.BitBlt(nStt, 30, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//2.
	bitmap[1].LoadBitmap(IDB_BITMAP_SHEAR2);	
	memDC.SelectObject(&bitmap[1]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 30, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//3.
	bitmap[2].LoadBitmap(IDB_BITMAP_SHEAR3);	
	memDC.SelectObject(&bitmap[2]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 30, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);	
	// Do not call CDialog::OnPaint() for painting messages
}


void CShearBarTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	int nType = m_nRadio + 1;	
	SetType(nType);
	CString str;
	str.Format("%d", nType);
	m_ReturnValue = str;
	CDialog::OnOK();
}



void CShearBarTypeDlg::SetType(int nType)
{
	m_nType = nType;
}

int CShearBarTypeDlg::GetType()
{
	return m_nType;
}
