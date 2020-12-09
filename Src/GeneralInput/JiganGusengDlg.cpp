// JiganGusengDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "JiganGusengDlg.h"
#include "ShoeSelectDlg.h"
#include "ExpansionJointTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJiganGusengDlg dialog


CJiganGusengDlg::CJiganGusengDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CJiganGusengDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJiganGusengDlg)
	//}}AFX_DATA_INIT
	m_nIdxExp	= 0;
}


void CJiganGusengDlg::DoDataExchange(CDataExchange* pDX)
{  
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJiganGusengDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_EXPANSION, m_GridExp);
	DDX_Control(pDX, IDC_EDIT_BINDCONC_STT, m_dBindConcLen_Stt);
	DDX_Control(pDX, IDC_EDIT_BINDCONC_END, m_dBindConcLen_End);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SHOEFIX, m_btnShoeFix);
	DDX_Control(pDX, IDC_BUTTON_RECOMMEND, m_btnRecommend);
	DDX_Control(pDX, IDC_BUTTON_EXPANSIONJOINT_TYPE, m_btnExpType);
	DDX_Control(pDX, IDC_EXPANSIONJOINT_CHECK, m_btnExpCheck);
	DDX_Control(pDX, IDC_BUTTON_AUTOCALC, m_btnAutoCalc);
}


BEGIN_MESSAGE_MAP(CJiganGusengDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CJiganGusengDlg)
	ON_BN_CLICKED(IDC_BUTTON_SHOEFIX, OnButtonShoefix)
	ON_BN_CLICKED(IDC_BUTTON_AUTOCALC, OnButtonAutocalc)
	ON_BN_CLICKED(IDC_BUTTON_EXPANSIONJOINT_TYPE, OnButtonExpansionjoitnType)
	ON_BN_CLICKED(IDC_EXPANSIONJOINT_CHECK, OnExpansionjointCheck)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMEND, OnButtonRecommendExpansionJoint)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnButtonNext)
	ON_EN_KILLFOCUS(IDC_EDIT_BINDCONC_STT, OnKillfocusEditBindconcStt)
	ON_EN_KILLFOCUS(IDC_EDIT_BINDCONC_END, OnKillfocusEditBindconcEnd)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_MESSAGE(EXPANSION_RECOMMEND, OnExpansionRecommend)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJiganGusengDlg message handlers

BOOL CJiganGusengDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_BUTTON_SHOEFIX,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_RECOMMEND,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EXPANSIONJOINT_TYPE,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EXPANSIONJOINT_CHECK,				SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_AUTOCALC,					SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_GRID_EXPANSION, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_PICTURE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_TEXT, SZ_TOP_LEFT, SZ_TOP_RIGHT);

	return TRUE;
}

void CJiganGusengDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
 	Draw.DrawPyung(&Dom);
	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CJiganGusengDlg::SetGridTitle()
{
	CPlateBridge *pBridge = m_pStd->GetBridge();

	long nJigan = pBridge->m_nQtyJigan;

	m_Grid.ResetAttrib(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(2);
	m_Grid.SetColumnCount(nJigan + 7);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);		
	m_Grid.SetColumnWidth(0,65);

	m_Grid.SetTextMatrix(0,0,"구 분");	
	m_Grid.SetTextMatrix(1,0,"길이(㎜)");	
	m_Grid.SetTextMatrix(0,1,"J1");
	m_Grid.SetTextMatrix(0,2,"S1");
	m_Grid.SetTextMatrix(0,3,"G1");

	CString str;

	for(long nCol = 4; nCol < m_Grid.GetCols()-3; nCol++)
	{
		str.Format("L%d",nCol - 3);
		m_Grid.SetTextMatrix(0,nCol,str);
	}

	m_Grid.SetTextMatrix(0,nJigan + 4,"G2");
	m_Grid.SetTextMatrix(0,nJigan + 5,"S2");
	m_Grid.SetTextMatrix(0,nJigan + 6,"J2");
	m_Grid.SetRedraw(TRUE, TRUE);

////////////////////////////////////////////////
	m_GridExp.SetEditable(TRUE);
	m_GridExp.SetListMode(FALSE);
	m_GridExp.EnableDragAndDrop(TRUE);
	m_GridExp.EnableInsertRow(FALSE);
	m_GridExp.EnableInsertCol(FALSE);
	m_GridExp.EnableReturnPass(TRUE);
	m_GridExp.SetRowCount(4);
	m_GridExp.SetColumnCount(9);
	m_GridExp.SetFixedRowCount(2);
	m_GridExp.SetFixedColumnCount(1);
	m_GridExp.SetRowHeightAll(20);
	m_GridExp.SetColumnWidthAll(70);
	m_GridExp.SetColumnWidth(1, 100);

	m_GridExp.SetTextMatrix(0, 0, "구분");				m_GridExp.SetMergeRow(0, 1, 0);
	m_GridExp.SetTextMatrix(0, 1, "최대허용신축량");	m_GridExp.SetMergeRow(0, 1, 1);
	m_GridExp.SetTextMatrix(0, 2, "BLOCK OUT");			m_GridExp.SetMergeCol(0, 2, 3);
	m_GridExp.SetTextMatrix(0, 4, "유간");				m_GridExp.SetMergeCol(0, 4, 6);
	m_GridExp.SetTextMatrix(0, 7, "모델명");			m_GridExp.SetMergeRow(0, 1, 7);
	m_GridExp.SetTextMatrix(0, 8, "검토");				m_GridExp.SetMergeRow(0, 1, 8);
	m_GridExp.SetTextMatrix(1, 2, "폭");
	m_GridExp.SetTextMatrix(1, 3, "높이");
	m_GridExp.SetTextMatrix(1, 4, "최소");
	m_GridExp.SetTextMatrix(1, 5, "평균");
	m_GridExp.SetTextMatrix(1, 6, "최대");

	m_GridExp.SetRedraw(TRUE, TRUE);
}

void CJiganGusengDlg::SetDataInit()
{
	SetGridTitle();

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	long nJigan = pBridge->m_nQtyJigan;


	m_Grid.SetTextMatrix(1,1,"%g", pBridge->m_dLengthUganStt);
	m_Grid.SetTextMatrix(1,nJigan+6, "%g", pBridge->m_dLengthUganEnd);	

	m_Grid.SetTextMatrix(1,2,"%g",pBridge->m_dLengthSlabGirderStt);
	m_Grid.SetTextMatrix(1,nJigan+5,"%g",pBridge->m_dLengthSlabGirderEnd);

	m_Grid.SetTextMatrix(1,3,"%g",pBridge->m_dLengthGirderShoeStt);
	m_Grid.SetTextMatrix(1,nJigan+4,"%g",pBridge->m_dLengthGirderShoeEnd);

	m_Grid.SetItemBkColour(1, nJigan+3, RGBREADONLY);
	m_Grid.SetItemState(1, nJigan+3, GVIS_READONLY);

	for(long nJ=0; nJ<nJigan; nJ++)
		m_Grid.SetTextMatrix(1,4+nJ,"%g",pBridge->m_dLengthJigan[nJ]);

	CGlobarOption			*pOpt		= m_pStd->m_pDataManage->GetGlobalOption();
	CDesignExpansionJoint	*pDesignExp	= m_pStd->m_pCalcStd->m_pDesignExpanJoint;
	CExpansionJointData		*pExpData	= NULL;

	if(pDesignExp->GetSize()>0)
	{
		pExpData	= pDesignExp->GetExpansionJointData(0);
		m_GridExp.SetTextMatrix(2, 0, pBridge->m_strJijumName[0]);
		m_GridExp.SetTextMatrix(2, 1, "%.0f", pExpData->m_dMaxAllowExp);
		m_GridExp.SetTextMatrix(2, 2, "%.0f", pOpt->GetSttExpansionJointWidth());
		m_GridExp.SetTextMatrix(2, 3, "%.0f", pOpt->GetSttExpansionJointHeight());
		m_GridExp.SetTextMatrix(2, 4, "%.0f", pExpData->m_dMinLayingGap);
		m_GridExp.SetTextMatrix(2, 5, "%.0f", (pExpData->m_dMinLayingGap+pExpData->m_dMaxLayingGap)/2.0);
		m_GridExp.SetTextMatrix(2, 6, "%.0f", pExpData->m_dMaxLayingGap);
		m_GridExp.SetTextMatrix(2, 7, pExpData->m_strExpanJoint);
		m_GridExp.SetTextMatrix(2, 8, pDesignExp->GetCheckSuccess(0));

		pExpData	= pDesignExp->GetExpansionJointData(1);
		m_GridExp.SetTextMatrix(3, 0, pBridge->m_strJijumName[nJigan]);
		m_GridExp.SetTextMatrix(3, 1, "%.0f", pExpData->m_dMaxAllowExp);
		m_GridExp.SetTextMatrix(3, 2, "%.0f", pOpt->GetEndExpansionJointWidth());
		m_GridExp.SetTextMatrix(3, 3, "%.0f", pOpt->GetEndExpansionJointHeight());
		m_GridExp.SetTextMatrix(3, 4, "%.0f", pExpData->m_dMinLayingGap);
		m_GridExp.SetTextMatrix(3, 5, "%.0f", (pExpData->m_dMinLayingGap+pExpData->m_dMaxLayingGap)/2.0);
		m_GridExp.SetTextMatrix(3, 6, "%.0f", pExpData->m_dMaxLayingGap);
		m_GridExp.SetTextMatrix(3, 7, pExpData->m_strExpanJoint);
		m_GridExp.SetTextMatrix(3, 8, pDesignExp->GetCheckSuccess(1));
	}

	if(pBridge->IsTUGir())
	{
		GetDlgItem(IDC_STATIC_TEXT_BIND1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT_BIND2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_BINDCONC_STT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_BINDCONC_END)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TEXT_BIND1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT_BIND2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_BINDCONC_STT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_BINDCONC_END)->ShowWindow(SW_HIDE);
	}

	CString	sText	= _T("");

	sText.Format("%g",	pBridge->m_dLengthTUBindConc_Stt);
	m_dBindConcLen_Stt.SetWindowText(sText);

	sText.Format("%g",	pBridge->m_dLengthTUBindConc_End);
	m_dBindConcLen_End.SetWindowText(sText);

	UpdateData(FALSE);	
}

void CJiganGusengDlg::SetDataDefault()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();

	long	nJigan		= pDB->m_nQtyJigan;
	double	dSlabLen	= pDB->m_dLengthTotalSlab;

	pDB->m_dLengthUganStt = 100;
	pDB->m_dLengthUganEnd = 100;	
	pDB->m_dLengthGirderShoeStt = 500;
	pDB->m_dLengthGirderShoeEnd = 500;


	if(pDB->IsTUGir())
	{
		pDB->m_dLengthSlabGirderStt		= 150;
		pDB->m_dLengthSlabGirderEnd		= 150;

		pDB->m_dLengthTUBindConc_Stt	= 100;
		pDB->m_dLengthTUBindConc_End	= 100;
	}
	else
	{
		pDB->m_dLengthSlabGirderStt		= 50;
		pDB->m_dLengthSlabGirderEnd		= 50;

		pDB->m_dLengthTUBindConc_Stt	= 0;
		pDB->m_dLengthTUBindConc_End	= 0;
	}

	CString szData;
	double dSlabOneLen = 
		(dSlabLen - pDB->m_dLengthSlabGirderStt - pDB->m_dLengthSlabGirderEnd
					- pDB->m_dLengthGirderShoeStt - pDB->m_dLengthGirderShoeEnd ) / (double)nJigan;

	for(long nJ=0; nJ<nJigan; nJ++)
		pDB->m_dLengthJigan[nJ] = dSlabOneLen;

	if(!IsUsedFlag())
		OnExpansionRecommend(-1,-1);

	SetModifyInputData(FALSE);
}

void CJiganGusengDlg::SetDataSave()
{
	CPlateBridgeApp			*pBridge	= m_pStd->GetBridge();
	CCalcData				*pData		= m_pStd->GetDataManage()->GetCalcData();
	CDesignExpansionJoint	*pDesignExp	= m_pStd->m_pCalcStd->m_pDesignExpanJoint;
	CGlobarOption			*pOpt		= m_pStd->m_pDataManage->GetGlobalOption();

	UpdateData(TRUE);

	long nJigan = pBridge->m_nQtyJigan;

	pBridge->m_dLengthUganStt = m_Grid.GetTextMatrixDouble(1,1);
	pBridge->m_dLengthUganEnd = m_Grid.GetTextMatrixDouble(1,nJigan+6);	

	pBridge->m_dLengthSlabGirderStt = m_Grid.GetTextMatrixDouble(1,2);
	pBridge->m_dLengthSlabGirderEnd = m_Grid.GetTextMatrixDouble(1,nJigan+5);

	pBridge->m_dLengthGirderShoeStt	= m_Grid.GetTextMatrixDouble(1,3);
	pBridge->m_dLengthGirderShoeEnd = m_Grid.GetTextMatrixDouble(1,nJigan+4);

	for(long nJ=0; nJ<nJigan; nJ++)
		pBridge->m_dLengthJigan[nJ] = m_Grid.GetTextMatrixDouble(1,4+nJ);

	CString str = "";
	pData->DESIGN_BASE.m_szBridgeLength.Format("%s = %.3f",str, toM(pBridge->GetLengthBridge()));

	if(pDesignExp->GetSize()>0)
	{
		pOpt->SetSttExpansionJointWidth(m_GridExp.GetTextMatrixDouble(2, 2));
		pOpt->SetSttExpansionJointHeight(m_GridExp.GetTextMatrixDouble(2, 3));
		pOpt->SetEndExpansionJointWidth(m_GridExp.GetTextMatrixDouble(3, 2));
		pOpt->SetEndExpansionJointHeight(m_GridExp.GetTextMatrixDouble(3, 3));

		for(long n=0; n<2; n++)
		{
			CExpansionJointData	*pExpData = pDesignExp->GetExpansionJointData(n);

			pExpData->m_bIsAbut				= pBridge->IsAbutOnJiJum(n==0?0:pBridge->m_nQtyJigan);

			pExpData->m_dMaxAllowExp		= m_GridExp.GetTextMatrixDouble(n+2, 1);
			pExpData->m_dMinLayingGap		= m_GridExp.GetTextMatrixDouble(n+2, 4);
			pExpData->m_dMaxLayingGap		= m_GridExp.GetTextMatrixDouble(n+2, 6);
			pExpData->m_strExpanJoint		= m_GridExp.GetTextMatrix(n+2, 7);
			pExpData->m_dEstablishLayingGap	= n==0 ? pBridge->m_dLengthUganStt : pBridge->m_dLengthUganEnd;
		}
	}

	CString	sText	= _T("");

	m_dBindConcLen_Stt.GetWindowText(sText);
	pBridge->m_dLengthTUBindConc_Stt	= atof(sText);

	m_dBindConcLen_End.GetWindowText(sText);
	pBridge->m_dLengthTUBindConc_End	= atof(sText);
}


void CJiganGusengDlg::SetJiganLastLen()
{
	CPlateBridge *pBridge = m_pStd->GetBridge();

	double total = 0;
	double dValue;
	for(long col = 2; col < pBridge->m_nQtyJigan + 6; col++)
	{
		dValue = m_Grid.GetTextMatrixDouble(1,col);
		if(dValue <= 0 || col == pBridge->m_nQtyJigan+3) continue;
		total += dValue;
	}
	
	m_Grid.SetTextMatrix(1,pBridge->m_nQtyJigan+3,"%.0lf",pBridge->m_dLengthTotalSlab - total);	
	m_Grid.Invalidate();
}

void CJiganGusengDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{	
	CGeneralBaseDlg::OnCellChangedData(nmgv, 0);	
	
	SetJiganLastLen();
}

void CJiganGusengDlg::SetDataBx()
{		
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		CPtrList NList;
		CPlateBasicIndex * p;
		double sta = 0;
		CDPoint vAng(0,1);

		CPlateGirderApp* pGir = pBridge->GetGirder(nG);

		sta = pGir->GetSSS(OFF_STT_GIRDER);
		vAng = pGir->GetAAA(OFF_STT_GIRDER);
		p = pGir->newBx();
		p->SetState(BX_STT_GIRDER,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );

		sta = pGir->GetSSS(OFF_END_GIRDER);
		vAng = pGir->GetAAA(OFF_END_GIRDER);
		p = pGir->newBx();
		p->SetState(BX_END_GIRDER,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );

		pGir->PressBxList(NList);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);
	}

}	

void CJiganGusengDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);
	
	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CJiganGusengDlg::OnButtonShoefix() 
{
	CShoeSelectDlg dlg(m_pStd);	
	if(dlg.DoModal()==IDOK)
	{
		OnExpansionRecommend(-1,-1);
	}

	SetDataSave();
	DrawInputDomyunView(FALSE);	
}

void CJiganGusengDlg::OnButtonAutocalc() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if(MessageBox("기존의 입력한 데이터가 삭제 됩니다.\n계속 하시겠습니까","지간구성",
			MB_ICONQUESTION | MB_YESNO) == IDNO)
			return;
	long	k = 0; 
	BOOL	bFix = FALSE;
	double Len = pBridge->m_dLengthGirderShoeStt;	
	long g = 0;
	for(g = 0;g < pBridge->GetGirdersu();g++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(g);
		for(k = 0;k < pBridge->m_nQtyJigan+1 ;k++)
		{			
			CShoe *pShoe = pGir->GetShoe(k);
			if(pShoe->m_cDirectionShoe == 0)
			{
				bFix = TRUE;
				break;
			}
		}
		if(bFix == TRUE) break;
	}
	if(k >= pBridge->m_nQtyJigan+1){
		MessageBox("먼저 슈 고정점을 선택하십시오","지간구성",MB_ICONINFORMATION);
		return;
	}
	CPlateGirderApp* pG = pBridge->GetGirder(g-1);
	if( k != 0)
	{
		for(long n = 0; n < k;n++)
			Len += pBridge->m_dLengthJigan[n];
	}
	if(k == pBridge->m_nQtyJigan) 
		Len += pBridge->m_dLengthGirderShoeEnd;


	SetSttEndUgan(Len);
	DrawInputDomyunView(TRUE);	
}

void CJiganGusengDlg::SetSttEndUgan(double shoeX)
// 고정슈를 선택하였을때 시종점의 유간을 구한다.
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	if(shoeX <= 0) return;
	CString str;
	long	col,cols = m_Grid.GetColumnCount(),ugan = 0;
	double	glength = shoeX ;

	for(col = 0;col < 2;col++)
	{
		if(col == 1)
			glength = (pBridge->m_dLengthTotalSlab - shoeX) ;
	
		if     (glength < 35)                     ugan =  35;
		else if(glength >= 35  && glength <  45)  ugan =  45;
		else if(glength >= 45  && glength <  55)  ugan =  50;
		else if(glength >= 55  && glength <  85)  ugan =  70;
		else if(glength >= 85  && glength <  95)  ugan =  80;
		else if(glength >= 95  && glength < 125)  ugan = 100;
		else if(glength >= 125 && glength < 155)  ugan = 120;
		else if(glength >= 155 && glength < 205)  ugan = 160;
		else if(glength >= 205 && glength < 305)  ugan = 240;
		else if(glength >= 305 && glength < 405)  ugan = 320;
		else if(glength >= 405 && glength < 555)  ugan = 400;
		else									  ugan = 500;

		str.Format("%d",ugan);
		if(col == 0) m_Grid.SetTextMatrix(1,1,str);
		else		 m_Grid.SetTextMatrix(1,cols - 1,str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CJiganGusengDlg::OnButtonNext()
{
	if(CheckShoeDirFixExist()==FALSE)	//고정단이 없으면
	{
		int ret;
		ret = MessageBox("고정단이 없습니다.\n\n교량받침 고정점을 선택 하시겠습니까 ?",
							"교량받침 고정점 선택 여부", MB_YESNO);
		if(ret == IDYES)
			OnButtonShoefix();

		return;
	}

	CGeneralBaseDlg::OnButtonNext();
}

BOOL CJiganGusengDlg::CheckShoeDirFixExist()
{
	BOOL m_bExistFix=FALSE;
	CPlateBridge *pBridge = m_pStd->GetBridge();

	for(long n = 0;n <= pBridge->m_nQtyJigan;n++)
	{
		for(long g = 0;g < pBridge->GetGirdersu();g++)
		{
			CPlateGirderApp *pGir = pBridge->GetGirder(g);
			CShoe *pShoe = pGir->GetShoe(n);			
			if(pShoe->m_cDirectionShoe==0)//고정단
			{
				m_bExistFix = TRUE;
				return m_bExistFix;
			}
			else
				m_bExistFix = FALSE;
		}
	}
	return m_bExistFix;
}

// lp : -1 지간구성 다이얼로그 자체에서의 권고안
long CJiganGusengDlg::OnExpansionRecommend(WPARAM wp, LPARAM lp)
{
	CPlateBridgeApp			*pDB		= m_pStd->GetBridge();
	CDesignExpansionJoint	*pDesignExp	= m_pStd->m_pCalcStd->m_pDesignExpanJoint;
	CPlateGirderApp			*pGir		= pDB->GetGirder(0);
	CGlobarOption			*pOpt		= m_pStd->m_pDataManage->GetGlobalOption();

	long	nJijum				= 0;
	long	nSection			= 0;
	double	dEstablishLayingGap	= 0;
	CString	strText				= _T("");

	if(wp==-1)
	{
		if(pDesignExp->GetSize()>0)
			pDesignExp->RemoveAll();
		for(long n=0; n<2; n++)
		{
			CExpansionJointData	*pExpData = new CExpansionJointData;
			if(n==0)
			{
				nJijum				= 0;
				nSection			= pDB->m_nTypeAbutOrPier[STT_POS]==1 ? BOTHSIDE_SECTION : POST_SECTION;
				dEstablishLayingGap	= pDB->m_dLengthUganStt;
			}
			else
			{
				nJijum				= pDB->m_nQtyJigan;
				nSection			= pDB->m_nTypeAbutOrPier[END_POS]==1 ? BOTHSIDE_SECTION : PRE_SECTION;
				dEstablishLayingGap	= pDB->m_dLengthUganEnd;
			}
			pExpData->m_strName					= pDB->m_strJijumName[nJijum];
			pExpData->m_bIsAbut					= pDB->IsAbutOnJiJum(nJijum);
			pExpData->m_nRegion					= NORMAL_REGION;
			pExpData->m_nPosition				= nSection;
			pExpData->m_dEstablishLayingGap		= dEstablishLayingGap;

			if(lp==-1)
			{
				pExpData->m_strExpanJoint		= _T("UCF-150S");
				pExpData->m_dMaxLayingGap		= 180.0;
				pExpData->m_dMinLayingGap		= 30.0;
				pExpData->m_dMaxAllowExp		= 150.0;
			}
			else
			{
				pExpData->m_dMaxAllowExp		= m_GridExp.GetTextMatrixDouble(n+2, 1);
				pExpData->m_dMinLayingGap		= m_GridExp.GetTextMatrixDouble(n+2, 4);
				pExpData->m_dMaxLayingGap		= m_GridExp.GetTextMatrixDouble(n+2, 6);
				pExpData->m_strExpanJoint		= m_GridExp.GetTextMatrix(n+2, 7);
			}

			if(nSection==BOTHSIDE_SECTION)
			{
				pExpData->m_nTypeBridge[PRE_SECTION]		= STEEL_DECK_BRIDGE;
				pExpData->m_nTypeBridge[POST_SECTION]		= STEEL_DECK_BRIDGE;

				if(n==0)
				{
					pExpData->m_dLengthGirder[POST_SECTION]		= GetDistGirder(nJijum, TRUE);
					pExpData->m_dHeightGirder[POST_SECTION]		= pGir->GetHeightGirderByJijum(nJijum);
					pExpData->m_dDispEarthQuake[POST_SECTION]	= 0;					
				}
				else
				{
					pExpData->m_dLengthGirder[PRE_SECTION]		= GetDistGirder(nJijum, FALSE);
					pExpData->m_dHeightGirder[PRE_SECTION]		= pGir->GetHeightGirderByJijum(nJijum);
					pExpData->m_dDispEarthQuake[PRE_SECTION]	= 0;
				}
			}
			else
			{
				pExpData->m_nTypeBridge[nSection]		= STEEL_DECK_BRIDGE;
				pExpData->m_dLengthGirder[nSection]		= GetDistGirder(nJijum, n == 0 ? TRUE : FALSE);
				pExpData->m_dHeightGirder[nSection]		= pGir->GetHeightGirderByJijum(nJijum);
				pExpData->m_dDispEarthQuake[nSection]	= 0;
			}			
			pDesignExp->InsertBridge(pExpData);
		}
	}
	else
	{
		CExpansionJointData	*pExpData	= pDesignExp->GetExpansionJointData(wp);

		if(wp==0)
		{
			nJijum				= 0;
			nSection			= pDB->m_nTypeAbutOrPier[STT_POS]==1 ? BOTHSIDE_SECTION : POST_SECTION;
			dEstablishLayingGap	= pDB->m_dLengthUganStt;
		}
		else
		{
			nJijum				= pDB->m_nQtyJigan;
			nSection			= pDB->m_nTypeAbutOrPier[END_POS]==1 ? BOTHSIDE_SECTION : PRE_SECTION;
			dEstablishLayingGap	= pDB->m_dLengthUganEnd;
		}
		pExpData->m_nPosition					= nSection;
		pExpData->m_strName						= pDB->m_strJijumName[nJijum];
		pExpData->m_bIsAbut						= pDB->IsAbutOnJiJum(nJijum);
		pExpData->m_nRegion						= NORMAL_REGION;
		pExpData->m_dEstablishLayingGap			= dEstablishLayingGap;

		if(lp==-1)
		{
			pExpData->m_strExpanJoint				= _T("UCF-150S");
			pExpData->m_dMaxLayingGap				= 180.0;
			pExpData->m_dMinLayingGap				= 30.0;
			pExpData->m_dMaxAllowExp				= 150.0;
		}

		if(pExpData->m_nPosition==BOTHSIDE_SECTION)
		{
			pExpData->m_nTypeBridge[PRE_SECTION]		= STEEL_DECK_BRIDGE;
			pExpData->m_nTypeBridge[POST_SECTION]		= STEEL_DECK_BRIDGE;
			if(wp==0)
			{
				pExpData->m_dLengthGirder[PRE_SECTION]		= 0;
				pExpData->m_dHeightGirder[PRE_SECTION]		= 0;
				pExpData->m_dDispEarthQuake[PRE_SECTION]	= 0;
				pExpData->m_dLengthGirder[POST_SECTION]		= GetDistGirder(nJijum, TRUE);
				pExpData->m_dHeightGirder[POST_SECTION]		= pGir->GetHeightGirderByJijum(nJijum);
				pExpData->m_dDispEarthQuake[POST_SECTION]	= 0;
			}
			else
			{
				pExpData->m_dLengthGirder[PRE_SECTION]		= GetDistGirder(nJijum, FALSE);
				pExpData->m_dHeightGirder[PRE_SECTION]		= pGir->GetHeightGirderByJijum(nJijum);
				pExpData->m_dDispEarthQuake[PRE_SECTION]	= 0;
				pExpData->m_dLengthGirder[POST_SECTION]		= 0;
				pExpData->m_dHeightGirder[POST_SECTION]		= 0;
				pExpData->m_dDispEarthQuake[POST_SECTION]	= 0;
			}
		}
		else
		{
			pExpData->m_nTypeBridge[nSection]		= STEEL_DECK_BRIDGE;
			pExpData->m_dLengthGirder[nSection]		= GetDistGirder(nJijum, wp==0 ? TRUE : FALSE);
			pExpData->m_dHeightGirder[nSection]		= pGir->GetHeightGirderByJijum(nJijum);
			pExpData->m_dDispEarthQuake[nSection]	= 0;
		}
		pDesignExp->SetExpansionJoint(wp);
	}

	// 엔진의 블럭제원 포인터에 APlate의 블럭제원 주소를 연결
	pOpt->SetExpansionBlockJewon(pDesignExp);

	return 0;
}

void CJiganGusengDlg::OnButtonExpansionjoitnType() 
{
	CGlobarOption			*pOpt	= m_pStd->m_pDataManage->GetGlobalOption();
	CExpansionJointTypeDlg	ExpDlg;

	long nIndex	= m_GridExp.GetFocusCell().row-2;

	if(nIndex<0)	
	{
		AfxMessageBox("선택된 항목이 없습니다.");
		return;
	}

	CExpansionJointData	*pExpData	= m_pStd->m_pCalcStd->m_pDesignExpanJoint->GetExpansionJointData(nIndex);
	ExpDlg.m_szName = pExpData->m_strExpanJoint;
	ExpDlg.m_strDlgTitle = "신축이음 제원(" + m_GridExp.GetTextMatrix(m_GridExp.GetFocusCell().row, 0) + ")";

	if(ExpDlg.DoModal()==IDOK)
	{
		if(nIndex==0)
		{
			pOpt->SetSttExpansionJointWidth(ExpDlg.m_dBlockW);
			pOpt->SetSttExpansionJointHeight(ExpDlg.m_dBlockH);
		}
		else if(nIndex==1)
		{
			pOpt->SetEndExpansionJointWidth(ExpDlg.m_dBlockW);
			pOpt->SetEndExpansionJointHeight(ExpDlg.m_dBlockH);
		}
		m_nIdxExp					= ExpDlg.m_nIndex;
		pExpData->m_strExpanJoint	= ExpDlg.m_szName;
		pExpData->m_dMaxAllowExp	= ExpDlg.m_dMaxAllowExpL;
		pExpData->m_dMinLayingGap	= ExpDlg.m_dUganMin;
		pExpData->m_dMaxLayingGap	= ExpDlg.m_dUganMax;
		SetDataInit();
	}
}

void CJiganGusengDlg::OnExpansionjointCheck() 
{
	CExpansionJointDlg	ExpJointDlg(this);

	if(CheckShoeDirFixExist()==FALSE)	//고정단이 없으면
	{
		if(MessageBox("고정단이 없습니다.\n\n교량받침 고정점을 선택 하시겠습니까 ?", "교량받침 고정점 선택 여부", MB_YESNO) == IDYES)
			OnButtonShoefix();
		return;
	}

	SetDataSave();
	ExpJointDlg.SetExpansionJoint(m_pStd->m_pCalcStd->m_pDesignExpanJoint);
	if(ExpJointDlg.DoModal()==IDOK)
		SetDataInit();
}

double CJiganGusengDlg::GetDistGirder(long nJijum, BOOL bPre)
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();

	double	dDist	= 0;
	long	nJ		= 0;
	BOOL	bFix	= FALSE;

	for(nJ=0;nJ <= pDB->m_nQtyJigan;nJ++)
	{
		if(bFix)	break;
		for(long nG = 0;nG < pDB->GetGirdersu();nG++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			CShoe *pShoe = pGir->GetShoe(nJ);

			if(pShoe->m_cDirectionShoe==0)
				bFix = TRUE;
		}
	}

	if(bPre)
	{
		for(long n=0; n<nJ-1; n++)
			dDist += pDB->m_dLengthJigan[n];
	}
	else
	{
		for(long n=nJ-1; n<nJijum; n++)
			dDist += pDB->m_dLengthJigan[n];
	}

	return dDist;
}


void CJiganGusengDlg::OnButtonRecommendExpansionJoint() 
{
	CGlobarOption	*pOpt		= m_pStd->m_pDataManage->GetGlobalOption();
	pOpt->SetSttExpansionJointWidth(300.0);
	pOpt->SetSttExpansionJointHeight(150.0);
	pOpt->SetEndExpansionJointWidth(300.0);
	pOpt->SetEndExpansionJointHeight(150.0);
	m_pStd->m_pCalcStd->m_pDesignExpanJoint->RemoveAll();
	OnExpansionRecommend(-1,-1);
	SetDataInit();
	SetDataSave();
}

void CJiganGusengDlg::OnKillfocusEditBindconcEnd() 
{
	SetDataSave();
	DrawInputDomyunView(FALSE);
}

void CJiganGusengDlg::OnKillfocusEditBindconcStt() 
{
	SetDataSave();
	DrawInputDomyunView(FALSE);
}
