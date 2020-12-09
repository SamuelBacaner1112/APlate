// TStandard.cpp: implementation of the CTStandard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "VAbortPgsDlg.h"
#include "TStandard.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTStandard::CTStandard()
{

	m_bUseSimpleThread = FALSE;
}

CTStandard::~CTStandard()
{
}

BOOL CTStandard::CreatePgs(int SPos)
{	// ID_INDICATOR_PROGRESS 59142 : String Table ID
	if(m_StatusProgress.GetSafeHwnd()) return FALSE;
	int Pos = SPos;
	CRect r;
	m_pWndStatusBar->GetClientRect(&r);
	r = CRect(Pos,1,Pos+203,r.Height()-2);
	return m_StatusProgress.Create( WS_VISIBLE, r, m_pWndStatusBar, 59142 );	
}
void CTStandard::SetPosPgs(int Pos)
{
	if(GetSafePgs()==FALSE) return;
	m_StatusProgress.SetPos(Pos);
}
void CTStandard::DestroyPgs()
{
	if(GetSafePgs()==FALSE) return;
	m_StatusProgress.DestroyWindow();
}

void CTStandard::SetRange( int nLower, int nUpper )
{
	if(GetSafePgs()==FALSE) return;
	m_StatusProgress.SetRange(nLower,nUpper);
}

BOOL* CTStandard::GetAbort() const
{ 
	CVAbortPgsDlg * pADlg = (CVAbortPgsDlg *)m_pAbortPgsDlg;

	return (BOOL* const)&pADlg->m_bAbort; 
}


void CTStandard::SetStatusBar(CXTPStatusBar *pStatusBar)
{
	m_pWndStatusBar = pStatusBar; 
	m_hStatusWnd = pStatusBar->GetSafeHwnd();
}

BOOL CTStandard::SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate/* = TRUE*/)
{
	CVAbortPgsDlg * pADlg = (CVAbortPgsDlg *)m_pAbortPgsDlg;

//-	if(pADlg->GetSafeHwnd())
//-		pADlg->SetPaneText(lpszNewText);
//-	else


	
	CXTPStatusBarPane* pPane = m_pWndStatusBar->GetPane(nIndex);
	pPane->SetText(lpszNewText);

	return TRUE;


//  ÀÌÀü
//	if(m_bUseSimpleThread)
//		return m_pWndStatusBar->SetPaneText(nIndex, lpszNewText,bUpdate);

//	return FALSE;
}

BOOL CTStandard::Abort() const
{
	CVAbortPgsDlg * pADlg = (CVAbortPgsDlg *)m_pAbortPgsDlg;

	if(m_bUseSimpleThread)
	{
		MSG msg;
		while(::PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE))
			AfxGetThread()->PumpMessage();
	}
	return pADlg->m_bAbort; 
}

void CTStandard::BeginSimpleThread()
{ 
	CVAbortPgsDlg * pADlg = (CVAbortPgsDlg *)m_pAbortPgsDlg;

	pADlg->m_bAbort = FALSE; 
	m_bUseSimpleThread = TRUE;
}
void CTStandard::EndSimpleThread()
{
	m_bUseSimpleThread = FALSE; 
}


void CTStandard::GoProgress(long lw)
{
	CVAbortPgsDlg * pADlg = (CVAbortPgsDlg *)m_pAbortPgsDlg;

	if(lw > 100 ) lw = 100;

	pADlg->SetPosAll(lw,FALSE);

}
