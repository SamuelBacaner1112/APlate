// SpliceBoltPositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "SpliceBoltPositionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpliceBoltPositionDlg dialog

CSpliceBoltPositionDlg::CSpliceBoltPositionDlg(CGeneralBaseStd *pStd, long nG, long nSp, CWnd* pParent /*=NULL*/)
	: CDialog(CSpliceBoltPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpliceBoltPositionDlg)
	//}}AFX_DATA_INIT
	m_pStd			= pStd;
	m_pParent		= (CDetFieldJoinDlg *)pParent;
	m_nG			= nG;
	m_nSp			= nSp;
	
	//////////////////////////////////////////////////////////////////////////	
	CSteelBolt		*pBolt	= m_pStd->GetDataManage()->GetSteelBolt();
	CPlateSplice	*pSp	= m_pStd->GetBridge()->GetGirder(nG)->GetBxBySpliceNoSPType(nSp)->GetSplice();
	CString str;
	str.Format("%s %s", m_pStd->GetBridge()->GetMaterial(SP_U_BOLT, pSp->GetBx()), pSp->m_ZzsBoltSize);
	long nBolt	= pBolt->GetIndex(str);
	m_dDan		= pBolt->GetCalcEdgeDistance(nBolt);

	m_nSpliceType	= pSp->m_nWebType;
	m_dHorUL	= pSp->m_uC;	// 상하판 ↔
	m_dVerUL	= pSp->m_uF;	// 상하판 ↕
	m_dHorMS	= pSp->m_sC;	// 모멘트,복부판 ↔	
	m_dVerMS	= pSp->m_sF;	// 모멘트,복부판 ↕
	m_dDan		= pSp->m_sA		> 0 ? pSp->m_sA		: m_dDan;
	m_dCen		= pSp->m_sC1	> 0 ? pSp->m_sC1	: 100;
}


void CSpliceBoltPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpliceBoltPositionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Radio(pDX,IDC_BOARD,m_nSpliceType);
}


BEGIN_MESSAGE_MAP(CSpliceBoltPositionDlg, CDialog)
	//{{AFX_MSG_MAP(CSpliceBoltPositionDlg)
	ON_BN_CLICKED(IDC_SELECTPOS, OnSelectPos)
	ON_BN_CLICKED(IDC_ALLPOS, OnAllPos)
	ON_BN_CLICKED(IDC_DEFAULT, OnRecommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpliceBoltPositionDlg message handlers

BOOL CSpliceBoltPositionDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSpliceBoltPositionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDataInit();
	
	return TRUE;
}

void CSpliceBoltPositionDlg::SetGridTitle()
{
	long nRow = 0, nCol =0;
	m_Grid.SetColumnCount(6);
	m_Grid.SetRowCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0,35);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetFiexedRows(1);
	
	m_Grid.SetTextMatrix(nRow,nCol,"상하판\n↔");		m_Grid.SetItemFormat(nRow,nCol++,DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(nRow,nCol,"상하판\n↕");		m_Grid.SetItemFormat(nRow,nCol++,DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(nRow,nCol,"복부판\n↔");		m_Grid.SetItemFormat(nRow,nCol++,DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(nRow,nCol,"복부판\n↕");		m_Grid.SetItemFormat(nRow,nCol++,DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(nRow,nCol,"연단거리\nΓ．");	m_Grid.SetItemFormat(nRow,nCol++,DT_VANDHCENTER_AUTO);
	m_Grid.SetTextMatrix(nRow,nCol,"중앙거리\n·|·");	m_Grid.SetItemFormat(nRow,nCol++,DT_VANDHCENTER_AUTO);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CSpliceBoltPositionDlg::SetDataInit()
{
	long nRow = 1, nCol = 0;
	SetGridTitle();
	m_Grid.SetTextMatrix(nRow,nCol++,"%g",m_dHorUL);
	m_Grid.SetTextMatrix(nRow,nCol++,"%g",m_dVerUL);
	m_Grid.SetTextMatrix(nRow,nCol++,"%g",m_dHorMS);
	m_Grid.SetTextMatrix(nRow,nCol++,"%g",m_dVerMS);
	m_Grid.SetTextMatrix(nRow,nCol++,"%g",m_dDan);
	m_Grid.SetTextMatrix(nRow,nCol,"%g",m_dCen);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CSpliceBoltPositionDlg::SetDataSave()
{
	long nRow = 1, nCol = 0;
	m_dHorUL	= m_Grid.GetTextMatrixDouble(nRow,nCol++);
	m_dVerUL	= m_Grid.GetTextMatrixDouble(nRow,nCol++);
	m_dHorMS	= m_Grid.GetTextMatrixDouble(nRow,nCol++);
	m_dVerMS	= m_Grid.GetTextMatrixDouble(nRow,nCol++);
	m_dDan		= m_Grid.GetTextMatrixDouble(nRow,nCol++);
	m_dCen		= m_Grid.GetTextMatrixDouble(nRow,nCol);
}

void CSpliceBoltPositionDlg::OnSelectPos() 
{
	SetDataSave();
	SetAutoCalcPos(m_nG, m_nSp);	
	m_pParent->SetDataInit();
	m_pParent->DrawInputDomyunView();
}

void CSpliceBoltPositionDlg::OnAllPos() 
{
	SetDataSave();

	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		for(long nSp=0; nSp<pGir->GetQtySplice(); nSp++)
			SetAutoCalcPos(nG, nSp);
	}

	m_pParent->SetDataInit();
	m_pParent->DrawInputDomyunView();
}

void CSpliceBoltPositionDlg::SetAutoCalcPos(long nG, long nSp)
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxBySpliceNoSPType(nSp);
	CPlateSplice		*pSp	= pBx ? pBx->GetSplice() : NULL;
	CGlobarOption		*pOpt	= pDB->m_pDataManage->GetGlobalOption();
	
	if(!pSp) return;
	CSteelBoltData *pBoltData	= pDB->m_pDataManage->GetSteelBolt()->GetBolt(pDB->GetMaterial(SP_W_BOLT, pBx), pSp->m_ZzsBoltSize);

	double dFlangUW	= min(pBx->GetFactChain(G_F_U, FALSE)->m_dWidth, pBx->GetFactChain(G_F_U, TRUE)->m_dWidth);
	double dFlangLW	= min(pBx->GetFactChain(G_F_L, FALSE)->m_dWidth, pBx->GetFactChain(G_F_L, TRUE)->m_dWidth);
	double dWebT	= min(pBx->GetFactChain(G_W, FALSE)->m_dFactChainThick, pBx->GetFactChain(G_W, TRUE)->m_dFactChainThick);

	double upperTp	= pBx->GetFactChain(G_F_U, FALSE)->m_dFactChainThick;
	double upperTn	= pBx->GetFactChain(G_F_U, TRUE)->m_dFactChainThick;
	double lowerTp	= pBx->GetFactChain(G_F_L, FALSE)->m_dFactChainThick;
	double lowerTn	= pBx->GetFactChain(G_F_L, TRUE)->m_dFactChainThick;
	double dWebH	= pGir->GetHeightGirderByStaAng(pBx->GetStation());
	if(!pOpt->GetDesignHeigtBaseUpper())
		dWebH -= max(0, upperTn-upperTp);
	if(pOpt->GetDesignHeigtBaseLower())
		dWebH -= max(0, lowerTn-lowerTp);	// Filler 고려된 최소웹길이

	UpdateData();
	pSp->m_nWebType = m_nSpliceType;
	
	// 상하플랜지 ////////////////////////////////////////////////////////////
	pSp->m_uF	= pSp->m_dF		= m_dVerUL;
	pSp->m_uC	= pSp->m_dC		= m_dHorUL;
	pSp->m_uC1	= pSp->m_dC1	= m_dCen;
	pSp->m_uA	= pSp->m_dA		= m_dDan;

	pSp->m_uE	= (long)max(0, RoundDown((dFlangUW/2 - pSp->m_uA*2 - 20) / pSp->m_uF, 0));
	pSp->m_dE	= (long)max(0, RoundDown((dFlangLW/2 - pSp->m_dA*2 - 20) / pSp->m_dF, 0));	

	if(pSp->m_uE < 1)	pSp->m_uF = 0;
	if(pSp->m_dE < 1)	pSp->m_dF = 0;
	pSp->m_uW   = pSp->m_uE * pSp->m_uF + pSp->m_uA*2;
	pSp->m_dW   = pSp->m_dE * pSp->m_dF + pSp->m_dA*2;

	// 전단이음&모멘트판 /////////////////////////////////////////////////////
	pSp->m_sE1 = 0;
	pSp->m_sF1 = 0.0;
	pSp->m_sF	= pSp->m_mF		= m_dVerMS;
	pSp->m_sC	= pSp->m_mC		= m_dHorMS;
	pSp->m_sC1	= pSp->m_mC1	= m_dCen;
	pSp->m_sA	= pSp->m_mA		= m_dDan;	

	switch(pSp->m_nWebType)
	{
	case SP_WEB_FL:		
		pSp->m_mB	= 0;
		pSp->m_mC	= 0;
		pSp->m_mC1	= 0;
		pSp->m_mA	= 0;
		pSp->m_mT	= 0;
		pSp->m_mD	= 0;
		pSp->m_mG	= 0;
		pSp->m_mW	= 0;
		pSp->m_mF	= 0;
		pSp->m_mE	= 0;
		pSp->m_sE	= (long)max(0, RoundDown((dWebH - pSp->m_sA*2 - 100)/pSp->m_sF, 0));
		if(pSp->m_sE < 1)	pSp->m_sF = 0;
		pSp->m_sW	= pSp->m_sE*pSp->m_sF+pSp->m_sE1*pSp->m_sF1*2+pSp->m_sA*2;
		break;
	case SP_WEB_MO:		
	case SP_WEB_FLMO:
		pSp->m_mB	= pSp->m_mB > 0 ? pSp->m_mB : pSp->m_sB + 1;
		pSp->m_mE	= 2;
		pSp->m_mW	= pSp->m_mE*pSp->m_mF+pSp->m_mA*2;
		
		if(pSp->m_nWebType==SP_WEB_MO)
		{
			pSp->m_mT	= pSp->m_mT > 0 ? pSp->m_mT : pSp->m_sT;
			pSp->m_sE	= (long)max(0, RoundDown((dWebH-pSp->m_mW*2-120-pSp->m_sA*2)/pSp->m_sF,0));
			pSp->m_sW	= pSp->m_sE*pSp->m_sF+pSp->m_sE1*pSp->m_sF1*2+pSp->m_sA*2;
			pSp->m_mG	= RoundDown((dWebH-pSp->m_sW-pSp->m_mW*2-20)/2,0);
		}
		else
		{
			pSp->m_mT	= pSp->m_sT;
			pSp->m_sE	= (long)max(0, RoundDown((dWebH-pSp->m_mW*2-100-pSp->m_sA*2)/pSp->m_sF,0));			
			pSp->m_sW	= pSp->m_sE*pSp->m_sF+pSp->m_sE1*pSp->m_sF1*2+(pSp->m_sF-pSp->m_sA)*2;
			pSp->m_mG	= (dWebH-pSp->m_sW-pSp->m_mW*2)/2;
		}
		pSp->m_mD	= pSp->m_mG+pSp->m_mA;
		break;
	}
}

void CSpliceBoltPositionDlg::OnRecommand()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateBasicIndex	*pBx	= pDB->GetGirder(m_nG)->GetBxBySpliceNoSPType(m_nSp);
	CPlateSplice		*pSp	= pBx ? pBx->GetSplice() : NULL;
	CSteelBolt			*pBolt	= m_pStd->GetDataManage()->GetSteelBolt();

	CString strSymbol;
	long nBolt = 0;
	if(pSp)
	{
		strSymbol.Format("%s %s", pDB->GetMaterial(SP_W_BOLT, pBx), pSp->m_ZzsBoltSize);
		nBolt	= pBolt->GetIndex(strSymbol);
		m_dDan	= pBolt->GetCalcEdgeDistance(nBolt);
	}
	else
		m_dDan	= 40;

	m_dHorUL	= 100;	// 상하판 ↔
	m_dVerUL	= 100;	// 상하판 ↕
	m_dHorMS	= 100;	// 모멘트,복부판 ↔	
	m_dVerMS	= 100;	// 모멘트,복부판 ↕	
	m_dCen		= 100;

	SetDataInit();
}
