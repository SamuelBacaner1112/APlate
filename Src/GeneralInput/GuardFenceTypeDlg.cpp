// GuardFenceTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "GuardFenceTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuardFenceTypeDlg dialog


CGuardFenceTypeDlg::CGuardFenceTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGuardFenceTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuardFenceTypeDlg)
	m_nRadio = -1;
	//}}AFX_DATA_INIT
	m_nType = 0;
}


void CGuardFenceTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuardFenceTypeDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CGuardFenceTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CGuardFenceTypeDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuardFenceTypeDlg message handlers

BOOL CGuardFenceTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nType = GetType();
	m_nRadio = nType - 1;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGuardFenceTypeDlg::DrawRebarType()
{


}

void CGuardFenceTypeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBitmap bitmap[7];
	CDC memDC;
	//현재 DC와 호환적인 메모리 DC를 만든다.
	memDC.CreateCompatibleDC(&dc);	
	int nWidth = 118;
	int nHeight = 178;
	int nStt = 10;
	int nSpace = 10;

	//1. 
	bitmap[0].LoadBitmap(IDB_BITMAP_GUARD1);	
	//메모리 DC에 비트맵을 설정한다.
	memDC.SelectObject(&bitmap[0]);
	dc.BitBlt(nStt, 20, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//2.
	bitmap[1].LoadBitmap(IDB_BITMAP_GUARD2);	
	memDC.SelectObject(&bitmap[1]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 20, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//3.
	bitmap[2].LoadBitmap(IDB_BITMAP_GUARD3);	
	memDC.SelectObject(&bitmap[2]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 20, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//4
	bitmap[3].LoadBitmap(IDB_BITMAP_GUARD4);	
	memDC.SelectObject(&bitmap[3]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 20, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//5
	bitmap[4].LoadBitmap(IDB_BITMAP_GUARD5);	
	memDC.SelectObject(&bitmap[4]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 20, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//6
	bitmap[5].LoadBitmap(IDB_BITMAP_GUARD6);	
	memDC.SelectObject(&bitmap[5]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 20, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	//7
	bitmap[6].LoadBitmap(IDB_BITMAP_GUARD7);	
	memDC.SelectObject(&bitmap[6]);
	nStt += nWidth + nSpace;
	dc.BitBlt(nStt, 20, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
	// Do not call CDialog::OnPaint() for painting messages
}


void CGuardFenceTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	int nBarType = GetType();

	if(nBarType == 3)
	{
		int nType = m_nRadio + 1;
		if(nType != 3)
		{
			AfxMessageBox("선택할수 없는 타입입니다.");
			m_nRadio = nBarType - 1;
			UpdateData(FALSE);
			return;
		}
	}
	else if(nBarType == 4)
	{
		int nType = m_nRadio + 1;
		if(nType != 4)
		{
			AfxMessageBox("선택할수 없는 타입입니다.");
			m_nRadio = nBarType - 1;
			UpdateData(FALSE);
			return;
		}
	}
	else if(nBarType == 1 || nBarType == 2 || nBarType == 5 || nBarType == 6 || nBarType == 7)
	{
		int nType = m_nRadio + 1;
		if(nType == 3 || nType == 4)
		{
			AfxMessageBox("선택할수 없는 타입입니다.");
			m_nRadio = nBarType - 1;
			UpdateData(FALSE);
			return;
		}
	}

	nBarType = m_nRadio + 1;
	SetType(nBarType);
	
	CDialog::OnOK();
}

void CGuardFenceTypeDlg::SetGuardFenceType(int nType)
{
	m_nRadio = nType - 1;
	UpdateData(FALSE);
}

void CGuardFenceTypeDlg::SetType(int nType)
{
	m_nType = nType;
}

int CGuardFenceTypeDlg::GetType()
{
	return m_nType;
}
