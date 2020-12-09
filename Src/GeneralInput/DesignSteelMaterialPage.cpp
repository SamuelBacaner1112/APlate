// DesignSteelMaterialPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignSteelMaterialPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignSteelMaterialPage dialog

SHGRIDTITLE CDesignSteelMaterialPage::m_gt[COLUMN_COUNT_SECTION] = 
	{	
		{"단면형식",	"%s",		120,	DT_CENTER,	TRUE}, // 0
		{"부재명",		"%s",		150,	DT_LEFT,	TRUE}, // 1
		{"부재형태",	"%s",		70,		DT_LEFT,	TRUE}, // 2
		{"부재대분류",	"%s",		80,		DT_LEFT,	FALSE}, // 3
		{"재질",		"%s",		100,	DT_LEFT,	FALSE}, // 4
		{"부재종류",	"%s",		100,	DT_LEFT,	FALSE}, // 5
	};

CDesignSteelMaterialPage::CDesignSteelMaterialPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignSteelMaterialPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignSteelMaterialPage)
	m_bSameApply = TRUE;
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_bChange = FALSE;
}


void CDesignSteelMaterialPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignSteelMaterialPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_SAMEAPPLY, m_bSameApply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignSteelMaterialPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignSteelMaterialPage)
	ON_BN_CLICKED(IDC_CHECK_SAMEAPPLY, OnCheckSameapply)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignSteelMaterialPage message handlers
void CDesignSteelMaterialPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_SAMEAPPLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}

void CDesignSteelMaterialPage::SetGridTitle()
{	
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(1);
	m_Grid.SetColumnCount(COLUMN_COUNT_SECTION);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(3);
	m_Grid.InputShGridTitle(m_gt, COLUMN_COUNT_SECTION,FALSE);
	m_Grid.SetTextBkColor(RGBEDITABLE);
}

void CDesignSteelMaterialPage::SetDataDefault()
{
	CSteelRange *pSteelRange = m_pDataManage->GetSteelRange();
	pSteelRange->SetDataInit();
	m_pDataManage->GetSteelSection()->DataInit();
	SetDataInit();
	SetFactDataInit();//공장이음이 생성되어 있으면 다시 초기화...
}

void CDesignSteelMaterialPage::SetDataInit()
{
	SetGridTitle();

	CSteelSection *pSteelSection = m_pDataManage->GetSteelSection();
	long	size = pSteelSection->GetSize();
	m_bSameApply = pSteelSection->m_bSameBuje;
	if(size < 1) return;

	CString	str	= "", strType	= "";	

	m_Grid.SetRowCount(size+1);
	m_Grid.SetRowHeightAll(20);				
	m_Grid.SetRowHeight(0, 20);	
	
	for(long nRow = 1; nRow < size+1 ; nRow++) 
	{
		CString sz = pSteelSection->GetType(nRow-1);
		m_Grid.SetGridData(m_gt ,nRow, 0, pSteelSection->GetType(nRow-1));
		m_Grid.SetGridData(m_gt ,nRow, 1, pSteelSection->GetName(nRow-1));
		m_Grid.SetGridData(m_gt ,nRow, 2, pSteelSection->GetShape(nRow-1));
		m_Grid.SetGridData(m_gt ,nRow, 3, pSteelSection->GetClass(nRow-1));
		m_Grid.SetCellType(nRow,3,CT_COMBO);
		m_Grid.SetGridData(m_gt ,nRow, 4, pSteelSection->GetMaterialByMarkBuje(nRow-1));		
		m_Grid.SetCellType(nRow,4,CT_COMBO);
		m_Grid.SetGridData(m_gt ,nRow, 5, pSteelSection->GetBujeSizeType(nRow-1));
		m_Grid.SetCellType(nRow,5,CT_COMBO);


		if(pSteelSection->GetClass(nRow-1)=="주부재")
		{
			m_Grid.SetItemBkColour(nRow, 3, RGB(0, 151, 233));
			m_Grid.SetItemBkColour(nRow, 4, RGB(0, 151, 233));
		}
		else if(pSteelSection->GetClass(nRow-1)=="부부재")
		{
			m_Grid.SetItemBkColour(nRow, 3, RGB(1, 222, 34));
			m_Grid.SetItemBkColour(nRow, 4, RGB(1, 222, 34));
		}
		else
		{
			m_Grid.SetItemBkColour(nRow, 3, RGB(211, 117, 2));
			m_Grid.SetItemBkColour(nRow, 4, RGB(211, 117, 2));
		}
	}

	for(long n=ST_W+1; n<EN_BOLT+2; n++)
		m_Grid.SetRowHeight(n, 0);		

	m_Grid.SetRedraw(TRUE,TRUE);
//	UpdateData(FALSE);
}

void CDesignSteelMaterialPage::SetDataSave()
{
	CSteelSection *pSteelSection = m_pDataManage->GetSteelSection();
	CCalcData     *pData         = m_pDataManage->GetCalcData();

	long	size = pSteelSection->GetSize();
	if(size < 1) return;

	long cRow = m_Grid.GetRowCount();
	if((size+1) > cRow)	size = cRow-1;

	for(long nRow = 1 ; nRow < size+1 ; nRow++) 
	{
		pSteelSection->SetClass (nRow-1, m_Grid.GetItemText(nRow, 3));
		pSteelSection->SetSymbol(nRow-1, m_Grid.GetItemText(nRow, 4));
		pSteelSection->SetBujeSizeType(nRow-1, m_Grid.GetItemText(nRow, 5));
	}
	pData->DESIGN_MATERIAL.m_szSwsJu = pSteelSection->GetMaterialByMarkBuje(G_F_U);
	pData->DESIGN_MATERIAL.m_szSwsBu = pSteelSection->GetMaterialByMarkBuje(CP_W);		
	pSteelSection->m_bSameBuje		 = m_bSameApply;
}

BOOL CDesignSteelMaterialPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDesignSteelMaterialPage::PreTranslateMessage(MSG* pMsg) 
{		
	CStringArray m_strCombo;
	CString strClass, str;
	CSteelDefine *pSteelDefine = m_pDataManage->GetSteelDefine();
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		CString szSelText = m_Grid.GetTextMatrix(next.row,next.col);

		switch(next.col)
		{
			case 3 :
				{			
					BOOL bFind = FALSE;
					for(int idx = 0 ; idx < pSteelDefine->GetSize() ; idx++)
					{
						CString szData = pSteelDefine->GetStringData(idx, BDISD_CLASS);
						if(szData == szSelText) bFind = TRUE;
						m_strCombo.Add(szData);		
					}
					
					if(!bFind) 
					{
						m_strCombo.RemoveAll();
						m_strCombo.Add(szSelText);
					}
					m_Grid.SetComboString(m_strCombo);			
				}
				break;
			case 4 :
				{
					strClass = m_Grid.GetItemText(next.row, 3);
					for(int idx = 0 ; idx < pSteelDefine->GetSize() ; idx++) 
					{
						str = pSteelDefine->GetStringData(idx, BDISD_CLASS);
						if(str == strClass) 
						{
							str = pSteelDefine->GetStringData(idx, BDISD_VARNAME);
							if((str == "ALL") || str == "")  // 모든 Symbol
								InsertSymbol(&m_strCombo,"");					
							else
								InsertSymbol(&m_strCombo,str);					
						}
					}
					if(!m_strCombo.GetSize()) m_strCombo.Add(m_Grid.GetTextMatrix(next.row,next.col));
					m_Grid.SetComboString(m_strCombo);		
				}
				break;
			case 5 :
			{
				//CStringArray strCombo;
				m_strCombo.Add("대형부재");
				m_strCombo.Add("소형부재");
				m_Grid.SetComboString(m_strCombo);
			}
			break;

		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
		
	return TStackedPage::PreTranslateMessage(pMsg);
}

void CDesignSteelMaterialPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{		
	UpdateData();
	m_bChange = TRUE;
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	CSteelDefine *pSteelDefine = m_pDataManage->GetSteelDefine();

	CString strDef;
	CString strNew = m_Grid.GetItemText(nRow,nCol);	
	//	Get 동종부재 동일적용
	BOOL	bSymbolAllApply = m_bSameApply;
	long idx, size;
	if(nCol == 3) 
	{ // Class
		size = pSteelDefine->GetSize();
		for(idx = 0 ; idx < size ; idx++) 
		{
			strDef = pSteelDefine->GetClass(idx);			
			if(strNew == strDef) 
			{
				CString data = pSteelDefine->GetSymbol(idx);
				if(data != m_Grid.GetItemText(nRow, nCol+1))
				{
					m_Grid.SetItemText(nRow, nCol+1, pSteelDefine->GetSymbol(idx));
					m_Grid.Invalidate();
				}
				break;
			}
		}
	}
	//	재질이 바뀌면 동일부재 동일적용인지 Check
	if(nCol == 4)
	{
		CString			strYetSteelCode = m_Grid.GetItemText(nRow, nCol);
		CGlobarOption	*pOpt			= m_pDataManage->GetGlobalOption();
		long			nApplyYear		= pOpt->GetSteelStandardYear();

		CDBAllowStressDataLoad *pDBAllowStressDataLoad = new CDBAllowStressDataLoad(nApplyYear, ROADTYPE);
		CStringArray strArrSteelCode;
		CStringArray strArrDBSteelCodeAll;

		pDBAllowStressDataLoad->GetSteelCode(strArrSteelCode);
		
		BOOL bExistCode = FALSE;
		for(long nIdx = 0; nIdx < strArrSteelCode.GetSize(); nIdx++)
		{
			if(strYetSteelCode == strArrSteelCode.GetAt(nIdx))
			{
				bExistCode = TRUE;
				break;
			}
		}
		if(m_Grid.GetTextMatrix(nRow, nCol-1) == "볼트")
			bExistCode = TRUE;

		if(!bExistCode)
		{
			CString strMsg = _T("");
			strMsg.Format("[%s]에서 지원하지 않는 강종입니다.!", pOpt->GetStrSteelStandardYear());
			AfxMessageBox(strMsg, MB_ICONWARNING);
			CSteelSection *pSteelSection = m_pDataManage->GetSteelSection();
			m_Grid.SetTextMatrix(nRow, nCol, pSteelSection->GetMaterialByMarkBuje(nRow-1), (UINT)DT_LEFT);
			delete pDBAllowStressDataLoad;
			return;
		}

		if(bSymbolAllApply)
		{
			CString strClass, strClassDes;
			//	부재 대분류(3), 재질(4)
			strClass = m_Grid.GetItemText(nRow, nCol-1);
			// 재질(Symbol)은 앞에서 strGrid가 가지고 있음
			int cRow = m_Grid.GetRowCount();
			for(nRow = 1 ; nRow < cRow ; nRow++) 
			{
				strClassDes = m_Grid.GetItemText(nRow, nCol-1);
				strDef		= m_Grid.GetItemText(nRow, nCol);
				if(strClass == strClassDes) { // 같은 대분류이면 Change
					if(m_Grid.GetItemText(nRow, nCol) != strNew)
					{
						m_Grid.SetItemText(nRow, nCol, strNew);
						m_Grid.Invalidate();
					}
				}
			}
		}
		
		delete pDBAllowStressDataLoad;
	}

	//이미 공장이음까지 진행된 상태이면 부재를 바꿨을때 적용되게 해줘야 한다...
	//070406....KB...
	//nMarkBujeFact 일반 상판 - G_F_U , 하판, - G_F_L, 복부판 - G_W
	
	/*if((GridView->iRow>0 && GridView->iRow<4) && (GridView->iColumn==4||GridView->iColumn==3))
	{
		CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
		CPlateGirderApp	*pGir	= pDB->GetGirder(0);

		if(pGir->GetSizeFactChain(G_F_U)==0)	return 0;

		//동일부재 동일적용이면 동일부재인지 체크 후 동일부재인것만 같이 변경
		//동일부재 동일적용이 아니면 해당 부재만 변경...
		long nType = GridView->iRow-1;//부재 타입...줄수 -1 
		long nCount = 1;//반복횟수
		long nArrType[3];
		long nCnt=0;
		long nArrCnt = 0;
		nArrType[0] = GridView->iRow-1;
		CString strClass, strClassDes;
		strClass = m_Grid.GetItemText(GridView->iRow, 3);//부재 대분류
		if (bSymbolAllApply)
		{
			for (nCnt=1;nCnt<4;nCnt++)
			{
				if(nCnt == GridView->iRow) 
				{
					nArrType[nArrCnt++] = nCnt-1;//현재 선택된 부제 타입...
					continue;
				}
				strClassDes = m_Grid.GetItemText(nCnt, 3);//부재 대분류
				if(strClass == strClassDes)
				{
					nArrType[nArrCnt++] = nCnt-1;
					nCount++;
				}
			}
		}	
		//일괄 세팅 해제..
		for (nCnt=0; nCnt<nCount; nCnt++)
		{
			nType = nArrType[nCnt];
			for(long nG=-1; nG<pDB->GetGirdersu(); nG++)
			{
				pGir = pDB->GetGirder(nG);
				
				for(long n=0; n<pGir->GetSizeFactChain(nType); n++)
				{
					CPlateFactChain	*pFactChain = pGir->GetFactChain(nType, n);
					pFactChain->m_strSymbol = m_Grid.GetTextMatrix(GridView->iRow, 4);
				}
			}			
		}			
	}*/
	SetDataSave();
	
	return;
}

void CDesignSteelMaterialPage::SetFactDataInit()
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(0);
	
	if(pGir->GetSizeFactChain(G_F_U)==0)	return;	

	for(long nType=0; nType<3; nType++)
	{
		for(long nG=-1; nG<pDB->GetGirdersu(); nG++)
		{
			pGir = pDB->GetGirder(nG);
			
			for(long n=0; n<pGir->GetSizeFactChain(nType); n++)
			{
				CPlateFactChain	*pFactChain = pGir->GetFactChain(nType, n);
				pFactChain->m_strSymbol = m_Grid.GetTextMatrix(nType+1, 4);
			}
		}		

	}	
}
BOOL CDesignSteelMaterialPage::InsertSymbol(CStringArray* pStrArr, CString pKey)
{
	long	ret, size;

	CSteelRange *pSteelRange = m_pDataManage->GetSteelRange();

	CString			str;
	CSteelRangeData	bsr;	
	
	size = pSteelRange->GetSize();
	if(size < 1) return TRUE;

	bsr.m_strVarName	= "";
	bsr.m_strStandard	= "";
	bsr.m_strSymbol		= "";
	if(pKey == "")
	{
		pSteelRange->AllocKey(&bsr, 0);
	}
	else
	{
		bsr.m_strVarName = pKey;
		pSteelRange->AllocKey(&bsr, BDISR_VARNAME);
	}
	ret = pSteelRange->StartKey();
	while(ret)
	{
		pSteelRange->GetRecord(&bsr);
		if(bsr.m_strSymbol == "" ||	bsr.m_strSymbol.Left(1) == " ")
		{
			;
		}
		else
		{
			pStrArr->Add(bsr.m_strSymbol);
		}
		ret = pSteelRange->NextKey();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignSteelMaterialPage::OnButtonF10T()
{
	for(long nRow=2; nRow<m_Grid.GetRowCount(); nRow++)
	{
		if(m_Grid.GetTextMatrix(nRow, 3) == "볼트")
			m_Grid.SetTextMatrix(nRow, 4, "F10T", (UINT)DT_LEFT);		
	}
	m_Grid.SetRedraw(TRUE, TRUE);
	SetDataSave();
}
void CDesignSteelMaterialPage::OnButtonF13T()
{
	for(long nRow=2; nRow<m_Grid.GetRowCount(); nRow++)
	{
		if(m_Grid.GetTextMatrix(nRow, 3) == "볼트")
			m_Grid.SetTextMatrix(nRow, 4, "F13T", (UINT)DT_LEFT);		
	}
	m_Grid.SetRedraw(TRUE, TRUE);
	SetDataSave();
}

void CDesignSteelMaterialPage::OnCheckSameapply() 
{
	UpdateData(TRUE);
	m_pDataManage->GetSteelSection()->m_bSameBuje = m_bSameApply;
}
