// CalcCantileverLoadCombo.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverLoadCombo dialog


CCalcCantileverLoadCombo::CCalcCantileverLoadCombo(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcCantileverLoadCombo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverLoadCombo)
	//}}AFX_DATA_INIT
	m_nSelectedCombo = 0;
}


void CCalcCantileverLoadCombo::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverLoadCombo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_COMOB,	m_GridCombo);
	DDX_GridControl(pDX, IDC_GRID_LOAD,		m_GridLoad);
	DDX_GridControl(pDX, IDC_GRID_STDLOAD,	m_GridStd);
	
	DDX_Control(pDX, IDC_BUTTON_ADD_COMBO,		m_btnAddCombo);
	DDX_Control(pDX, IDC_BUTTON_DELETE_COMBO,	m_btnDelCombo);
	DDX_Control(pDX, IDC_BUTTON_UPPER,			m_btnUpper);
	DDX_Control(pDX, IDC_BUTTON_LOWER,			m_btnLower);
	DDX_Control(pDX, IDC_BUTTON_INSERTCOMBO,	m_btnInsertCombo);
	DDX_Control(pDX, IDC_BUTTON_REMOVECOMBO,	m_btnRemoveCombo);
}


BEGIN_MESSAGE_MAP(CCalcCantileverLoadCombo, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverLoadCombo)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ADD_COMBO, OnButtonAddCombo)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_COMBO, OnButtonDeleteCombo)
	ON_BN_CLICKED(IDC_BUTTON_INSERTCOMBO, OnButtonInsertcombo)
	ON_BN_CLICKED(IDC_BUTTON_REMOVECOMBO, OnButtonRemovecombo)
	ON_BN_CLICKED(IDC_BUTTON_UPPER, OnButtonUpper)
	ON_BN_CLICKED(IDC_BUTTON_LOWER, OnButtonLower)

	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID_COMOB,  OnCellChangedRowCombo)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_LOAD,	 OnCellChangedDataLoad)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_COMOB, OnCellChangedDataCombo)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverLoadCombo message handlers

BOOL CCalcCantileverLoadCombo::IsValid()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	BOOL bValid = pDB->m_bTendonInputComplete;

	if (!bValid)
		m_bUsedFlag = FALSE;
	
	return bValid;
}

void CCalcCantileverLoadCombo::OnPreInitDialog()
{
	
	SetResize(IDC_GRID_COMOB,			SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_LOAD,			SZ_TOP_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID_STDLOAD,			SZ_MIDDLE_RIGHT,SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_UPPER,			SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON_LOWER,			SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON_INSERTCOMBO,	SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON_REMOVECOMBO,	SZ_MIDDLE_RIGHT,	SZ_MIDDLE_RIGHT);

	SetResize(IDC_BUTTON_ADD_COMBO,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE_COMBO,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);

	CheckLoadCaseApply();
	CheckLoadComboApply();
	SetOriginalComboData();
}

BOOL CCalcCantileverLoadCombo::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	return TRUE;  
}

void CCalcCantileverLoadCombo::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun* pDom = m_pView->GetDomyun();
	pDom->ClearEtt();
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcCantileverLoadCombo::GridCommon(CGridCtrl *pGrid, long nQtyRow, long nQtyCol)
{
	pGrid->DeleteAllItems();
	
	pGrid->EnableReturnPass(TRUE);
	pGrid->EnableInsertCol(FALSE);
	pGrid->EnableInsertRow(FALSE);
	pGrid->SetTextBkColor(RGBEDITABLE);	
	pGrid->SetRowResize(TRUE);
	pGrid->SetColumnResize(TRUE);
	pGrid->SetGridLines(GVL_BOTH);
	pGrid->SetFixedRowCount(1);
	pGrid->SetFixedColumnCount(1);
	pGrid->SetListMode(FALSE);
	pGrid->EnableTitleTips(FALSE);	
	pGrid->SetRowCount(nQtyRow);
	pGrid->SetColumnCount(nQtyCol);	
	pGrid->SetRedraw(TRUE,TRUE);
}

long CCalcCantileverLoadCombo::GetStaticLoadCase()
{
	long nCase = 0;
	long nRow = m_GridStd.GetRows();
	if (nRow<2) return 0;

	for (int i=1; i<nRow; i++)
	{
		CString sz = m_GridStd.GetTextMatrix(i, 1);
		if (sz=="BEFORE") break;
		nCase++;
	}

	return nCase;
}

void CCalcCantileverLoadCombo::SetDataSave()
{

}

void CCalcCantileverLoadCombo::SetDataDefault()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage	*pFEM =	pCalcStd->GetCantileverFemData(0);
	long i, j;
	long nStt, nEnd;

	for (i=0; i<(long)m_bCurrentLC.size(); i++)
		m_bCurrentLC[i] = m_bOriginalLC[i];

	for (i=0; i<pFEM->m_ComboList.GetCount(); i++)
	{
		POSITION	pos		= pFEM->m_ComboList.FindIndex(i);

		if (i>=(long)m_Comb_bApply.size())
		{
			pFEM->m_ComboList.RemoveAt(pos);
			i--;
			continue;
		}
		
		CCombo*		pCombo	= (CCombo*)pFEM->m_ComboList.GetAt(pos);


		pCombo->m_bApply		= m_Comb_bApply[i];
		pCombo->m_szName		= m_Comb_szName[i];
		pCombo->m_szDescription	= m_Comb_szDescription[i];
		pCombo->m_nEnvType		= m_Comb_nEnvType[i];
		pCombo->m_bLive			= m_Comb_bLive[i];

		nStt = m_Comb_Idx1[i+0];
		nEnd = m_Comb_Idx1[i+1];
		pCombo->m_nType.RemoveAll();
		for (j=nStt; j<nEnd; j++)
		{
			DWORD nn = m_Comb_Arr1.GetAt(j);
			pCombo->m_nType.Add(m_Comb_Arr1.GetAt(j));
		}

		nStt = m_Comb_Idx2[i+0];
		nEnd = m_Comb_Idx2[i+1];
		pCombo->m_szLoadName.RemoveAll();
		for (j=nStt; j<nEnd; j++)
		{
			CString sz = m_Comb_Arr2.GetAt(j);
			pCombo->m_szLoadName.Add(m_Comb_Arr2.GetAt(j));
		}

		nStt = m_Comb_Idx3[i+0];
		nEnd = m_Comb_Idx3[i+1];
		pCombo->m_dSafe.RemoveAll();
		for (j=nStt; j<nEnd; j++)
		{
			double dd = m_Comb_Arr3.GetAt(j);
			pCombo->m_dSafe.Add(m_Comb_Arr3.GetAt(j));
		}
	}

	SetLoadCaseApply();
	SetGridTitleCombo();
	SetDataInitCombo();
	SetGridTitleLoad();
	SetDataInitLoad();
	SetGridTitleStd();
	SetDataInitStd();
}

void CCalcCantileverLoadCombo::SetDataInit()
{
	SetGridTitleCombo();
	SetDataInitCombo();
	
	SetGridTitleLoad();
	SetDataInitLoad();
	
	SetGridTitleStd();
	SetDataInitStd();
}

void CCalcCantileverLoadCombo::SetGridTitleCombo()
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage      *pFEM = pCalcStd->GetCantileverFemData(0);
		
	long	nColSu	=	5;
	long	nRowSu	=	pFEM->m_ComboList.GetCount() == 0 ? 1 : pFEM->m_ComboList.GetCount()+1;
	
	GridCommon(&m_GridCombo, nRowSu, nColSu);
	
	m_GridCombo.SetColumnWidthAll(40);
	m_GridCombo.SetColumnWidth(1, 120);
	m_GridCombo.SetColumnWidth(2, 120);
	m_GridCombo.SetColumnWidth(3, 320);
	m_GridCombo.SetRowHeightAll(20);
	
	m_GridCombo.SetTextMatrix(0, 0, "순번");
	m_GridCombo.SetTextMatrix(0, 1, "조합명");
	m_GridCombo.SetTextMatrix(0, 2, "조합");
	m_GridCombo.SetTextMatrix(0, 3, "설명");
	m_GridCombo.SetTextMatrix(0, 4, "적용");
	
	if(pFEM->m_ComboList.GetCount() == 0)	return;
	
	for(long nRow=1; nRow<nRowSu; nRow++)
	{
		for(long nCol=1; nCol<nColSu; nCol++)
		{
			if(nCol==2)			m_GridCombo.SetCellType(nRow, nCol, CT_COMBO);
			else if(nCol==4)	m_GridCombo.SetCellType(nRow, nCol, CT_CHECKBOX);
		}
	}
	
	m_GridCombo.SetRedraw(TRUE,TRUE);	
}

void CCalcCantileverLoadCombo::SetGridTitleLoad()
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage	*pFEM = pCalcStd->GetCantileverFemData(0);
	CString		szText		= _T("");
	
	POSITION	pos		=	pFEM->m_ComboList.GetHeadPosition();
	CCombo		*pComb	=	NULL;
	long		nSize	=	0;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb)	nSize = 0;
	else		nSize = pComb->m_szLoadName.GetSize();
	
	long	nRowSu	=	nSize == 0 ? 1 : nSize+1;
	long	nColSu	=	3;
	
	GridCommon(&m_GridLoad, nRowSu, nColSu);
	
	m_GridLoad.SetColumnWidthAll(40);
	m_GridLoad.SetColumnWidth(1, 100);
	m_GridLoad.SetColumnWidth(2, 100);
	m_GridLoad.SetRowHeightAll(20);
	
	m_GridLoad.SetTextMatrix(0, 0, "순번");
	m_GridLoad.SetTextMatrix(0, 1, "하중명");
	m_GridLoad.SetTextMatrix(0, 2, "가중치");
	
	if(pFEM->m_ComboList.GetCount()==0)	return;
	
	m_GridLoad.SetRedraw(TRUE,TRUE);
	m_GridLoad.ExpandColumnsToFit();	
}

void CCalcCantileverLoadCombo::SetGridTitleStd()
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		=	pCalcStd->GetCantileverFemData(0);
	long			nColSu		=	3;
	long			nRowSu		=	0;
	
	for(long nLC = 0; nLC < pFEM->m_szLCName.GetSize(); nLC++)
	{
		if(pFEM->m_szLCName[nLC].Find("MAX") > -1 || pFEM->m_szLCName[nLC].Find("MIN") > -1)
			continue;
		nRowSu++;
	}
	
	nRowSu += m_nSelectedCombo + 1 + pFEM->m_MovingLoadList.GetCount();
	
	GridCommon(&m_GridStd, nRowSu, nColSu);
	
	m_GridStd.SetColumnWidthAll(40);
	m_GridStd.SetColumnWidth(1, 80);
	m_GridStd.SetColumnWidth(2, 120);
	m_GridStd.SetRowHeightAll(20);
	
	m_GridStd.SetTextMatrix(0, 0, "순번");
	m_GridStd.SetTextMatrix(0, 1, "하중명");
	m_GridStd.SetTextMatrix(0, 2, "설명");
	
	m_GridStd.SetRedraw(TRUE,TRUE);
	m_GridStd.ExpandColumnsToFit();	
}

void CCalcCantileverLoadCombo::SetDataInitCombo()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage	*pFEM =	pCalcStd->GetCantileverFemData(0);
	
	long	nRow	=	1;
	CString	szText	=	_T("");
	
	if(pFEM->m_ComboList.GetCount()==0)	return;
	
	POSITION pos = pFEM->m_ComboList.GetHeadPosition();
			 
	while(pos)
	{
		CCombo *pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		szText.Format("%d", nRow);
		
		m_GridCombo.SetTextMatrix(nRow, 0, szText);
		m_GridCombo.SetTextMatrix(nRow, 1, pComb->m_szName);
		m_GridCombo.SetTextMatrix(nRow, 2, pComb->m_nEnvType ==0 ? "ADD" : "ENVE");
		m_GridCombo.SetTextMatrix(nRow, 3, pComb->m_szDescription);
		m_GridCombo.SetCheck(nRow, 4, pComb->m_bApply);
		
		nRow++;
	}
	
	m_GridCombo.SetRedraw(TRUE, TRUE);	
}

void CCalcCantileverLoadCombo::SetDataInitLoad()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage      *pFEM =	pCalcStd->GetCantileverFemData(0);
	
	long		nRow	=	1;
	long		nSize	=	0;
	CString		szText	=	_T("");
	POSITION	pos		=	pFEM->m_ComboList.GetHeadPosition();
	CCombo		*pComb	=	NULL;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb)	nSize = 0;
	else		nSize = pComb->m_szLoadName.GetSize();
	
	for(long n=0; n<nSize; n++)
	{
		szText.Format("%d", nRow);
		m_GridLoad.SetTextMatrix(nRow, 0, szText);
		szText.Format("%s", pComb->m_szLoadName[n]);
		m_GridLoad.SetTextMatrix(nRow, 1, szText);
		szText.Format("%.3f", pComb->m_dSafe[n]);
		m_GridLoad.SetTextMatrix(nRow, 2, szText);
		
		m_GridLoad.SetItemState(nRow, 1, GVIS_READONLY);
		m_GridLoad.SetItemBkColour(nRow, 1, RGBREADONLY);
		nRow++;
	}
	
	m_GridLoad.SetRedraw(TRUE, TRUE);	
}

void CCalcCantileverLoadCombo::SetDataInitStd()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		=	pCalcStd->GetCantileverFemData(0);
	CString			szText		=	_T("");
	long			nRow		=	1;
	
	POSITION pos ;
	
	nRow = 1;
	
	for(long nLC = 0; nLC < pFEM->m_szLCName.GetSize(); nLC++)
	{
		if(pFEM->m_szLCName[nLC].Find("MAX") > -1 || pFEM->m_szLCName[nLC].Find("MIN") > -1)
			continue;
		
		szText.Format("%d", nRow);
		
		m_GridStd.SetTextMatrix(nRow, 0, szText);
		m_GridStd.SetTextMatrix(nRow, 1, pFEM->m_szLCName[nLC]);
		
		if(nLC < pFEM->m_szLCComment.GetSize())
			m_GridStd.SetTextMatrix(nRow, 2, pFEM->m_szLCComment[nLC]);
		
		m_GridStd.SetItemState(nRow, 1, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 1, RGBREADONLY);
		m_GridStd.SetItemState(nRow, 2, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 2, RGBREADONLY);
		
		nRow++;
	}
	
	pos = pFEM->m_MovingLoadList.GetHeadPosition();
	
	while(pos)
	{
		CMovingLoad *pMove = (CMovingLoad*) pFEM->m_MovingLoadList.GetNext(pos);
		
		if(!pMove) return;
		
		szText.Format("%d", nRow);
		
		m_GridStd.SetTextMatrix(nRow, 0, szText);
		m_GridStd.SetTextMatrix(nRow, 1, pMove->m_szName);
		m_GridStd.SetTextMatrix(nRow, 2, "");
		
		m_GridStd.SetItemState(nRow, 1, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 1, RGBREADONLY);
		m_GridStd.SetItemState(nRow, 2, GVIS_READONLY);		m_GridStd.SetItemBkColour(nRow, 2, RGBREADONLY);
		
		nRow++;
	}
	if(pFEM->m_ComboList.GetCount() <= 0 )  return;
	
	pos = pFEM->m_ComboList.GetHeadPosition();
	
	for(long nComb = 0; nComb < m_nSelectedCombo; nComb++)
	{	
		CCombo *pComb = (CCombo *)pFEM->m_ComboList.GetNext(pos);
		
		if(!pComb)	return;
		
		szText.Format("%d", nRow);
		
		m_GridStd.SetTextMatrix(nRow,	0,	szText);
		m_GridStd.SetTextMatrix(nRow,	1,	pComb->m_szName);
		m_GridStd.SetTextMatrix(nRow,	2, pComb->m_szDescription);
		
		m_GridStd.SetItemState(nRow,	1,	GVIS_READONLY); m_GridStd.SetItemBkColour(nRow,	1,	RGBREADONLY);
		m_GridStd.SetItemState(nRow,	2,	GVIS_READONLY); m_GridStd.SetItemBkColour(nRow,	2,	RGBREADONLY);
		
		nRow++;	
	}
	
	m_GridStd.SetRedraw(TRUE, TRUE);	
}

BOOL CCalcCantileverLoadCombo::SwapCombo(BOOL bUpper)
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		=	pCalcStd->GetCantileverFemData(0);
	
	if(bUpper	&& m_nSelectedCombo	< 1)										return FALSE;
	if(!bUpper	&& m_nSelectedCombo	> pFEM->m_ComboList.GetCount() - 2)	return FALSE;	
	
	long	nIdx1		=	m_nSelectedCombo + 1;
	long	nIdx2		=	nIdx1 + (bUpper? -1 : 1);	
	CString	szText		=	_T("");
	CString	szText2		=	_T("");
	CCombo	*pComb		=	NULL;
	CCombo	*pComb2		=	NULL;	
	CCombo	*pComb3		=	NULL;
	POSITION pos		=	pFEM->m_ComboList.GetHeadPosition();
	POSITION posBack	=	NULL;
	
	szText	=	m_GridCombo.GetTextMatrix(nIdx1,	1);
	
	//bUpper가 true이면 이전의 행을, false이면 다음의 행의 키 값을 가져 옴.
	szText2	=	m_GridCombo.GetTextMatrix(nIdx2,	1);	
	
	while(pos)
	{
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	pos = pFEM->m_ComboList.GetHeadPosition();
	
	//Key 값 찾기 작업
	while(pos)
	{
		pComb2 = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText2 == pComb2->m_szName)
			break;
	}
	
	if(bUpper)
	{
		for(long n = 0; n < pComb->m_szLoadName.GetSize(); n++)
		{
			if(pComb->m_szLoadName[n] == pComb2->m_szName)
				return FALSE;
		}
	}
	else
	{
		for(long n = 0; n < pComb2->m_szLoadName.GetSize(); n++)
		{
			if(pComb2->m_szLoadName[n] == pComb->m_szName)
				return FALSE;
		}
		
	}
	
	pos = pFEM->m_ComboList.GetHeadPosition();
	
	//위치 변경 작업
	while(pos)
	{
		posBack = pos;
		pComb3 = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb3->m_szName)
		{
			pFEM->m_ComboList.SetAt(posBack,pComb2);
			
			if(bUpper)
			{
				pFEM->m_ComboList.GetPrev(posBack);
				pFEM->m_ComboList.SetAt(posBack, pComb);
			}
			else
			{
				pFEM->m_ComboList.GetNext(posBack);
				pFEM->m_ComboList.SetAt(posBack, pComb);
			}
			break;
		}		
	}
	
	return TRUE;
}

BOOL CCalcCantileverLoadCombo::SwapLoad(CCombo* pCombo, long nIdx1, BOOL bUpper)
{
	if(bUpper	&& nIdx1 < 1)									return FALSE;
	if(!bUpper	&& nIdx1 > pCombo->m_szLoadName.GetSize()-2)	return FALSE;
	
	return TRUE;

}

BOOL CCalcCantileverLoadCombo::ErrorMessage(long nErr, long nVal)
{
	BOOL bError	= FALSE;
	switch(nErr)
	{
	case 0 :	// 하중조합 선택이 항목이 없는경우
		if(nVal>=0)	bError = FALSE;
		else
		{
			AfxMessageBox("하중조합을 선택하여 주십시요.");
			bError = TRUE;
		}
		break;
	case 1 :	// 추가할 하중 선택이 없는경우
		if(nVal>=0)	bError = FALSE;
		else
		{
			AfxMessageBox("추가할 하중을 선택하여 주십시요.");
			bError = TRUE;
		}
		break;
	case 2 :	// 삭제할 comboLoad를 선택하지 않은경우
		if(nVal>=0)	bError = FALSE;
		else
		{
			AfxMessageBox("제거할 하중을 선택하여 주십시요.");
			bError = TRUE;
		}
		break;
	case 3 :
		if(nVal>0)	bError = FALSE;
		else
		{
			AfxMessageBox("하중조합이 존재하지 않습니다.");
			bError = TRUE;
		}
		break;
	}
	
	return bError;
}

void CCalcCantileverLoadCombo::OnCellChangedRowCombo(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW		*GridView	=	(NM_GRIDVIEW *)nmgv;
	int				nRow		=	GridView->iRow;
	
	m_GridLoad.ResetSelection();
	m_GridStd.ResetSelection();	
	m_nSelectedCombo = nRow-1;
	
	SetGridTitleLoad();
	SetGridTitleStd();
	SetDataInitStd();	
	SetDataInitLoad();

	m_GridCombo.SetFocus();
}

void CCalcCantileverLoadCombo::OnCellChangedDataLoad(NMHDR* nmgv, LRESULT*)
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		=	pCalcStd->GetCantileverFemData(0);
	CString			szText		=	_T("");
	
	szText = m_GridStd.GetTextMatrix(m_nSelectedCombo+1,1);
	
	POSITION pos	=	pFEM->m_ComboList.GetHeadPosition();
	CCombo *pComb	=	NULL;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);	
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb) return;
	
	CCellID CellID = m_GridLoad.GetFocusCell();
	
	pComb->m_dSafe.GetAt(CellID.row-1) = m_GridLoad.GetTextMatrixDouble(CellID.row,	2);
}

void CCalcCantileverLoadCombo::OnCellChangedDataCombo(NMHDR* nmgv, LRESULT*)
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		=	pCalcStd->GetCantileverFemData(0);
	NM_GRIDVIEW*	pGrid		= (NM_GRIDVIEW*)nmgv;
	
	long	nRow	= pGrid->iRow;
	long	nCol	= pGrid->iColumn;
	CString	szText	= _T("");	
	if(pFEM->m_ComboList.GetCount()==0)	return;
	
	POSITION pos	= pFEM->m_ComboList.FindIndex(nRow-1);
	CCombo*	 pComb	= (CCombo*)pFEM->m_ComboList.GetAt(pos);

	pComb->m_szName = m_GridCombo.GetTextMatrix(nRow, 1);
	if ( m_GridCombo.GetTextMatrix(nRow, 2) == "ADD")
		pComb->m_nEnvType	=	0;
	else
		pComb->m_nEnvType	=	1;
	pComb->m_szDescription	=	m_GridCombo.GetTextMatrix(nRow, 3);

	if (nCol==4)
	{
		if (m_GridCombo.IsCheck(nRow, 4))
		{
			CString szTitle = _T("Load Combination 변경");
			CString szText	= _T("");
			szText.Format("해당 Load Cobination %s에 해당하는 모든 Load Case 활성화합니다.", pComb->m_szName);
			if (IDOK == MessageBox(szText, szTitle, MB_OKCANCEL | MB_ICONQUESTION))
			{
				pComb->m_bApply = m_GridCombo.IsCheck(nRow, nCol);
				SetComboToLoadCaseApply(pComb);
			}
			else
				m_GridCombo.SetCheck(nRow, 4, pComb->m_bApply);
		}
		else
		{
			CString szTitle = _T("Load Combination 변경");
			CString szText	= _T("");
			szText.Format("해당 Load Cobination -  %s에 해당하는 모든 Load Case 비활성화합니다.", pComb->m_szName);
			if (IDOK == MessageBox(szText, szTitle, MB_OKCANCEL | MB_ICONQUESTION))
			{
				pComb->m_bApply = m_GridCombo.IsCheck(nRow, nCol);
				SetComboToLoadCaseApply(pComb);
			}
			else
				m_GridCombo.SetCheck(nRow, 4, pComb->m_bApply);
		}
	}
}

void CCalcCantileverLoadCombo::OnButtonAddCombo()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage      *pFEM		=	pCalcStd->GetCantileverFemData(0);
	CString			sText		=	_T("");
	
	sText.Format("USER%d",pFEM->m_ComboList.GetCount()+1);
	
	CCombo *pComb = new CCombo(sText, CCombo::ENEV, 1, FALSE);
	
	pFEM->m_ComboList.AddTail(pComb);
	
	SetDataInit();
}

void CCalcCantileverLoadCombo::OnButtonDeleteCombo()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		=	pCalcStd->GetCantileverFemData(0);
	long			nSize		=	(long)0.0;
	CString			szText		=	_T("");
	CString			szText2		=	_T("");
	CStringArray	szArr;	

	POSITION	pos;
	CCombo		*pComb;
	CCombo		*pComb2;
	
	szText	=	m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,1);
	pos		=	pFEM->m_ComboList.GetHeadPosition();

	while(pos)
	{
		pComb2	=	(CCombo*)pFEM->m_ComboList.GetNext(pos);
		if(szText == pComb2->m_szName)
			break;
	}

	pos	=	pFEM->m_ComboList.GetHeadPosition();

	long nCount = 1;

	//현재 조합이 다른 곳에서 참조 되었는지를 체크함.
	while(pos)
	{
		pComb	= (CCombo*)pFEM->m_ComboList.GetNext(pos);

		nCount++;	
		
		if(m_nSelectedCombo > nCount) continue;
		
		nSize	= pComb->m_szLoadName.GetSize();
		
		for(long nLoad = 0; nLoad < nSize; nLoad++)
		{
			BOOL	bExist		=	FALSE;
			CString szTitle1	=	pComb->m_szLoadName[nLoad];
			CString szTitle2	=	pComb2->m_szName;
			CString	szCombo		=	pComb->m_szName;

			if(szTitle1 == szTitle2)
				bExist = TRUE;

			if(bExist)
				szArr.Add(szCombo);
		}
		
	}
	
	if(szArr.GetSize()>0)
	{
		CString	sz = _T("");

		for(long n=0; n<szArr.GetSize(); n++)
		{
			sz.Format("%s, ", szArr.GetAt(n));
			szText2 += sz;
		}
		
		sz.Format("\n%s는 위의 하중조합에서 사용중입니다. 먼저 위의 조합에서 제거해야 합니다.", pComb2->m_szName);
		
		szText2 += sz;

		AfxMessageBox(szText2);

		return;
	}

	if(m_nSelectedCombo!=0)
		m_nSelectedCombo--;

	m_GridCombo.SetFocusCell(m_nSelectedCombo+1, 1);
	
	
	if(ErrorMessage(3, pFEM->m_ComboList.GetCount())) return;

	POSITION PosBack;

	pos = pFEM->m_ComboList.GetHeadPosition();

	while(pos)
	{
		pComb2	=	(CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb2->m_szName)
			break;

		PosBack = pos;
	}
	
	//자료구조의 첫번째 pos는 GetNext실행 후 얻어 올 수 없음.
	if(pFEM->m_ComboList.GetCount()==1 || m_nSelectedCombo==0)
		PosBack = pFEM->m_ComboList.GetHeadPosition();

	if(PosBack == NULL) return;

	pFEM->m_ComboList.RemoveAt(PosBack);
	
	if(m_nSelectedCombo!=0)
		m_nSelectedCombo--;

	m_GridCombo.SetFocusCell(m_nSelectedCombo+1, 1);

	SetDataInit();
}

void CCalcCantileverLoadCombo::OnButtonUpper()
{
	if(m_nSelectedCombo < 0 ) return;
	
	CCellID CellLoad	=	m_GridLoad.GetFocusCell();
	CCellID	CellCombo	=	m_GridCombo.GetFocusCell();
	
	if(CellLoad.row < 1)	//Combo
	{
		if( SwapCombo(TRUE) )
			m_GridCombo.SetFocusCell(CellCombo.row-1, CellCombo.col);
		SetDataInitCombo();
	}
}

void CCalcCantileverLoadCombo::OnButtonLower()
{
	CCellID CellLoad	=	m_GridLoad.GetFocusCell();
	CCellID	CellCombo	=	m_GridCombo.GetFocusCell();
	
	if(CellLoad.row<1)	// Combo
	{
		if(SwapCombo(FALSE))
			m_GridCombo.SetFocusCell(CellCombo.row+1, CellCombo.col);
		SetDataInitCombo();
	}
}

void CCalcCantileverLoadCombo::OnButtonInsertcombo()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage	*pFEM			=	pCalcStd->GetCantileverFemData(0);
	CString		szText			=	_T("");
	
	szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,1);
	
	POSITION pos	=	pFEM->m_ComboList.GetHeadPosition();
	CCombo *pComb	=	NULL;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb) return;
	
	CCellID CellID = m_GridStd.GetFocusCell();
	szText = m_GridStd.GetTextMatrix(CellID.row,1);
	
	if(szText.GetLength()<=0) return;
	long nStatic = GetStaticLoadCase();
	long nType = (CellID.row>nStatic)? 1 : 0;
	
	pComb->m_szLoadName.Add(szText);
	pComb->m_nType.Add(nType);
	pComb->m_dSafe.Add(1.0);


	SetGridTitleLoad();
	SetDataInitLoad();
}

void CCalcCantileverLoadCombo::OnButtonRemovecombo()
{
	CAPlateCalcStd  *pCalcStd   =	m_pStd->m_pCalcStd;
	CFEMManage	*pFEM			=	pCalcStd->GetCantileverFemData(0);
	CCombo		*pComb			=	NULL;
	POSITION	pos				=	pFEM->m_ComboList.GetHeadPosition();
	CString		szText			=	_T("");
	long		nCombo			=	m_nSelectedCombo;
	long		nComboLoad		=	m_GridLoad.GetFocusCell().row-1;
	
	if(ErrorMessage(0, nCombo))	return;
	if(ErrorMessage(2, nComboLoad))	return;
	
	while(pos)
	{
		szText = m_GridCombo.GetTextMatrix(m_nSelectedCombo+1,	1);
		
		pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		if(szText == pComb->m_szName)
			break;
	}
	
	if(!pComb) return;
	
	CCellID CellID	=	m_GridLoad.GetFocusCell();
	long	nCurRow	=	CellID.row;
	
	pComb->m_szLoadName.RemoveAt(nCurRow-1);
	pComb->m_nType.RemoveAt(nCurRow-1);
	pComb->m_dSafe.RemoveAt(nCurRow-1);
	
	
	SetGridTitleLoad();
	SetDataInitLoad();
}

long CCalcCantileverLoadCombo::GetLoadCaseIndex(CString szLoad)
{
	long nIdx = 0;

	for (long n=0; n<m_szNameLC.GetSize(); n++)
	{
		if (szLoad==m_szNameLC.GetAt(n))
			return n;
	}

	return nIdx;
}

long CCalcCantileverLoadCombo::GetLoadApplyMemberCount()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);
	
	long nCnt = 0;
	
	for (long nE=0; nE<pFEM->GetElementSize(); nE++)
	{
		CElement* pElem = pFEM->GetElement((unsigned short)nE);
		CVector vStt    = pFEM->GetJoint(pElem->m_nJStt)->m_vPoint;
		CVector vEnd    = pFEM->GetJoint(pElem->m_nJEnd)->m_vPoint;
		
		if (vStt.z==0 && vEnd.z==0) nCnt++; 
	}
	
	return nCnt;
}

void CCalcCantileverLoadCombo::SetComboToLoadCaseApply(CCombo* pComb)
{
	for (long nCombo=0; nCombo<pComb->m_szLoadName.GetSize(); nCombo++)
	{
		CString szCombo = pComb->m_szLoadName.GetAt(nCombo);
		BOOL bAll_Apply = pComb->m_bApply;

		for (long n=0; n<(long)m_bCurrentLC.size(); n++)
		{
			CString szLoad = m_szNameLC.GetAt(n);
			if (szCombo!=szLoad) continue;
			m_bCurrentLC[n] = bAll_Apply;
		}
	}

	SetLoadCaseApply();
}

void CCalcCantileverLoadCombo::SetLoadCaseApply()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CCalcData		  *pData = m_pStd->GetDataManage()->GetCalcData();
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);

	long nApplyElement  = GetLoadApplyMemberCount();

	for (long n=0; n<(long)m_bCurrentLC.size(); n++)
	{
		CString szLoad = m_szNameLC.GetAt(n);
		BOOL	bApply = m_bCurrentLC.at(n);	

		for (long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
		{
			CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
			CLoadForce	*pForce	= pElem->GetLoadForce(n);
			if (!pForce)	continue;
			
			if(pForce->IsFlag(EXIST_TRAP))
			{
				for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
				{
					CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
					CString szLoadName = pTrap->m_strName;
					long	nCnt = szLoadName.Find(szLoad);				
					if (nCnt<0) continue;					
					pTrap->m_bApply = bApply;
				}
			}
			
			if(pForce->IsFlag(EXIST_DISTFORCE))
			{
				for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
				{
					CLoadForcePoint	*pPoint	= pForce->m_PointForceArr[nLoad];
					CString szLoadName = pPoint->m_strName;
					long	nCnt = szLoadName.Find(szLoad);				
					if (nCnt<0) continue;
					pPoint->m_bApply = bApply;
				}
			}
			
			if ( szLoad.Find("WIND")>-1 && nElem==0 )
			{
				long nIdx = (szLoad.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);			
				Force->m_bApply = bApply;
			}
			
			if ( szLoad.Find("CRASH")>-1 && nElem==0 )
			{
				long nIdx = (szLoad.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);
				Force->m_bApply = bApply;	
			}
		}
	}
}

void CCalcCantileverLoadCombo::CheckLoadCaseApply()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CCalcData		  *pData = m_pStd->GetDataManage()->GetCalcData();
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);

	m_bCurrentLC.clear();
	m_bOriginalLC.clear();
	m_szNameLC.RemoveAll();

	long nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;
	long nApplyElement  = GetLoadApplyMemberCount();
	long n = 0;
	for (n=0; n<nLoadCaseSize; n++)
	{
		BOOL	bApply	 = FALSE;
		CString szLCName = pFEM->m_szLCName[n];
		m_szNameLC.Add(szLCName);
		
		for (long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
		{
			CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
			CLoadForce	*pForce	= pElem->GetLoadForce(n);
			if (!pForce)	continue;
			if (bApply)		continue;

			if(pForce->IsFlag(EXIST_TRAP) && !bApply)
			{
				for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
				{
					CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
					CString szLoadName = pTrap->m_strName;
					long	nCnt = szLoadName.Find(szLCName);				
					if (nCnt<0) continue;					
					if (bApply) continue;
					bApply = pTrap->m_bApply;
				}
			}

			if(pForce->IsFlag(EXIST_DISTFORCE) && !bApply)
			{
				for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
				{
					CLoadForcePoint	*pPoint	= pForce->m_PointForceArr[nLoad];
					CString szLoadName = pPoint->m_strName;
					long	nCnt = szLoadName.Find(szLCName);				
					if (nCnt<0) continue;
					if (bApply) continue;
					bApply = pPoint->m_bApply;
				}
			}

			if ( szLCName.Find("WIND")>-1 && nElem==0 && !bApply )
			{
				long nIdx = (szLCName.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);			
				bApply = Force->m_bApply;
				if (bApply) continue;
			}

			if ( szLCName.Find("CRASH")>-1 && nElem==0 && !bApply )
			{
				long nIdx = (szLCName.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);
				bApply = Force->m_bApply;	
			}
		}

		m_bCurrentLC.push_back(bApply);
	}
	
	for (n=0; n<(long)m_bCurrentLC.size(); n++)
		m_bOriginalLC.push_back(m_bCurrentLC[n]);
}

void CCalcCantileverLoadCombo::CheckLoadComboApply()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);

	if (pFEM->m_ComboList.GetCount()<1) return;
	POSITION pos = pFEM->m_ComboList.GetHeadPosition();

	while(pos)
	{
		BOOL bComboApply = FALSE;
		CCombo* pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);

		for (long n=0; n<pComb->m_szLoadName.GetSize(); n++)
		{
			if (bComboApply) continue;
			CString szCombLC = pComb->m_szLoadName.GetAt(n);
			long	nLoad	 = GetLoadCaseIndex(szCombLC);
			bComboApply		 = m_bCurrentLC.at(nLoad); 
		}
		pComb->m_bApply = bComboApply;
	}
}

void CCalcCantileverLoadCombo::SetOriginalComboData()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);
	
	if (pFEM->m_ComboList.GetCount()<1) return;
	
	POSITION pos = pFEM->m_ComboList.GetHeadPosition();
	long	 nPre[3] = { 0, 0, 0 };
	long	 nCur[3] = { 0, 0, 0 };
	
	m_Comb_bApply.clear();
	m_Comb_szName.clear();
	m_Comb_szDescription.clear();
	m_Comb_nEnvType.clear();
	m_Comb_bLive.clear();
	m_Comb_Idx1.RemoveAll();
	m_Comb_Idx2.RemoveAll();
	m_Comb_Idx3.RemoveAll();
	m_Comb_Arr1.RemoveAll();
	m_Comb_Arr2.RemoveAll();
	m_Comb_Arr3.RemoveAll();
	
	while(pos)
	{
		CCombo* pComb = (CCombo*)pFEM->m_ComboList.GetNext(pos);
		
		m_Comb_bApply.push_back(pComb->m_bApply);
		m_Comb_szName.push_back(pComb->m_szName);
		m_Comb_szDescription.push_back(pComb->m_szDescription);
		m_Comb_nEnvType.push_back(pComb->m_nEnvType);
		m_Comb_bLive.push_back(pComb->m_bLive);
		
		CDWordArray  Arr1;
		CStringArray Arr2;
		CDoubleArray Arr3;
		long n = 0;
		for (n=0; n<pComb->m_nType.GetSize(); n++)
			Arr1.Add(pComb->m_nType.GetAt(n));
		
		for (n=0; n<pComb->m_szLoadName.GetSize(); n++)
			Arr2.Add(pComb->m_szLoadName.GetAt(n));
		
		for (n=0; n<pComb->m_dSafe.GetSize(); n++)
			Arr3.Add(pComb->m_dSafe.GetAt(n));
		
		m_Comb_Idx1.Add(nPre[0]);
		nCur[0] = Arr1.GetSize();
		nPre[0] += nCur[0];
		for (n=0; n<Arr1.GetSize(); n++)
			m_Comb_Arr1.Add(Arr1.GetAt(n));
		
		m_Comb_Idx2.Add(nPre[1]);
		nCur[1] = Arr2.GetSize();
		nPre[1] += nCur[1];
		for (n=0; n<Arr2.GetSize(); n++)
			m_Comb_Arr2.Add(Arr2.GetAt(n));
		
		m_Comb_Idx3.Add(nPre[2]);
		nCur[2] = Arr3.GetSize();
		nPre[2] += nCur[2];
		for (n=0; n<Arr3.GetSize(); n++)
			m_Comb_Arr3.Add(Arr3.GetAt(n));
	}
	m_Comb_Idx1.Add(nPre[0]);
	m_Comb_Idx2.Add(nPre[1]);
	m_Comb_Idx3.Add(nPre[2]);
}

BOOL CCalcCantileverLoadCombo::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID CellCombo	= m_GridCombo.GetFocusCell();
		if(CellCombo.col==2)
		{
			strCombo.Add("ADD");
			strCombo.Add("ENVE");
			
			m_GridCombo.SetComboString(strCombo);
		}
	}
	
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}
