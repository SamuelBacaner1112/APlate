// CalcCantileverModeling.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON							0.0001

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverModeling dialog


CCalcCantileverModeling::CCalcCantileverModeling(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcCantileverModeling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverModeling)
	m_nLoadCase = 0;
	//}}AFX_DATA_INIT
	m_nSelectElem	= -1;
}


void CCalcCantileverModeling::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverModeling)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_DEL_LOAD, m_btnDelLoad);
	DDX_Control(pDX, IDC_BUTTON_ADD_LOAD, m_btnAddLoad);
	DDX_Control(pDX, IDC_TAB_LOADCASE, m_ctlTab);
	DDX_Radio(pDX, IDC_RADIO_SLAB, m_nLoadCase);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_CHECK_APPLY_FORCE, m_ApplyForce);

}


BEGIN_MESSAGE_MAP(CCalcCantileverModeling, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverModeling)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_SLAB, OnRadioPreLoad)
	ON_BN_CLICKED(IDC_RADIO_POST, OnRadioPostLoad)
	ON_BN_CLICKED(IDC_RADIO_LIVE, OnRadioLiveLoad)
	ON_BN_CLICKED(IDC_RADIO_WIND, OnRadioWindLoad)
	ON_BN_CLICKED(IDC_RADIO_COLLISION, OnRadioCrashLoad)
	ON_BN_CLICKED(IDC_RADIO_CENTROID, OnRadioCentrifugalLoad)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LOAD, OnButtonAddload)
	ON_BN_CLICKED(IDC_BUTTON_DEL_LOAD, OnButtonDelload)
	ON_BN_CLICKED(IDC_CHECK_APPLY_FORCE, OnCheckApplyForce)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOADCASE, OnTabSelChange)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverModeling message handlers

BOOL CCalcCantileverModeling::IsValid()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	BOOL bValid = pDB->m_bTendonInputComplete;

	if (!bValid)
		m_bUsedFlag = FALSE;

	return bValid;
}

void CCalcCantileverModeling::SetDataDefault()
{	
	CCalcData  *pData = m_pStd->GetDataManage()->GetCalcData();
	CFemCantileverCalc FemCantCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	pData->m_bRunCross = FALSE;
	FemCantCalc.MakeCantileverModeling(0);

	m_nLoadCase = 0;
	InitTabCtrl();
	m_ApplyForce.SetCheck(m_bCheckArr.GetAt(0));
	SetDataInit();
	DrawInputDomyunView();

	UpdateData(FALSE);

}

void CCalcCantileverModeling::SetDataInit()
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		= pCalcStd->GetCantileverFemData(0);

	// Set Grid
	m_Grid.SetGridDefault(2, 0, 2, 0);

	long    nApplyElement   = m_pStd->GetLoadApplyMemberCount(pFEM);
	long	nRow			= 0;
	long	nCol			= 0;
	UINT	nFormat			= DT_CENTER | DT_VCENTER;
	CString	szText			= _T("");
	CString szType1			= (m_nLoadCase==WIND||m_nLoadCase==CRASH)?  _T("절점명\n") : _T("부재명\n"); 
	CString szType2			= (m_nLoadCase==WIND||m_nLoadCase==CRASH)? _T("거리") : _T("비율");

	m_Grid.AddCell(nRow, nCol++, "하중명\n");
	m_Grid.AddCell(nRow, nCol++, "하중종류\n");
	m_Grid.AddCell(nRow, nCol++, szType1);
	m_Grid.AddCell(nRow, nCol++, "위치①");
	m_Grid.AddCell(nRow, nCol++, "하중크기①");
	m_Grid.AddCell(nRow, nCol++, "하중크기①");
	m_Grid.AddCell(nRow, nCol++, "하중크기①");
	m_Grid.AddCell(nRow, nCol++, szType1);
	m_Grid.AddCell(nRow, nCol++, "위치②");
	m_Grid.AddCell(nRow, nCol++, "하중크기②");
	m_Grid.AddCell(nRow, nCol++, "하중크기②");
	m_Grid.AddCell(nRow, nCol++, "하중크기②");
	m_Grid.AddCell(nRow, nCol++, "적용\n");
	nRow++; nCol=0;

	m_Grid.AddCell(nRow, nCol++, "하중명\n");
	m_Grid.AddCell(nRow, nCol++, "하중종류\n");
	m_Grid.AddCell(nRow, nCol++, szType1);
	m_Grid.AddCell(nRow, nCol++, szType2);
	m_Grid.AddCell(nRow, nCol++, "Uz");
	m_Grid.AddCell(nRow, nCol++, "Ux");
	m_Grid.AddCell(nRow, nCol++, "Ry");
	m_Grid.AddCell(nRow, nCol++, szType1);
	m_Grid.AddCell(nRow, nCol++, szType2);
	m_Grid.AddCell(nRow, nCol++, "Uz");
	m_Grid.AddCell(nRow, nCol++, "Ux");
	m_Grid.AddCell(nRow, nCol++, "Ry");
	m_Grid.AddCell(nRow, nCol++, "적용\n");
	
	nRow++;	nCol=0;

	CString	szLoad		= GetTabName();
	long nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;
	
	for(long n=0; n<nLoadCaseSize; n++)
	{
		CString szLCName = pFEM->m_szLCName[n];
		if(szLoad=="")	continue;

		for (long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
		{
			CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
			CLoadForce	*pForce	= pElem->GetLoadForce(n);
			if(!pForce)	continue;

			if(pForce->IsFlag(EXIST_TRAP))
			{
				for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
				{
					CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
					UINT nReadOnly	= pTrap->m_bApply ? CELL_LINE_NO : CELL_READONLY;
					CString szLoadName = pTrap->m_strName;
					long	nCnt = szLoadName.Find(szLoad);				
					if (nCnt<0) continue;
					
					m_Grid.AddCell(nRow, nCol++, &pTrap->m_strName, nFormat, CELL_READONLY);
					m_Grid.AddCell(nRow, nCol, "분포하중", nFormat, CELL_READONLY);nCol++;
					szText.Format("%d", nElem+1);
					m_Grid.AddCell(nRow, nCol, szText, nFormat, nReadOnly);							m_Grid.SetCellDec(1);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_dRStt, nFormat, nReadOnly);				m_Grid.SetCellDec(6);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapStt.z, nFormat, nReadOnly);			m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapStt.x, nFormat, nReadOnly);			m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapRotateStt.y, nFormat, nReadOnly);		m_Grid.SetCellDec(4);	nCol++;
					
					m_Grid.AddCell(nRow, nCol, szText, nFormat, nReadOnly);							m_Grid.SetCellDec(1);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_dREnd, nFormat, nReadOnly);				m_Grid.SetCellDec(6);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapEnd.z, nFormat, nReadOnly);			m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapEnd.x, nFormat, nReadOnly);			m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapRotateEnd.y, nFormat, nReadOnly);		m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_bApply, nFormat, -1, "적용", CELL_TYPE_CHECK);					
					m_bCheckArr.GetAt(m_ctlTab.GetCurSel()) = pTrap->m_bApply;

					nRow++;nCol=0;
				}
			}

			if(pForce->IsFlag(EXIST_DISTFORCE))
			{
				for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
				{
					CLoadForcePoint	*pPoint	= pForce->m_PointForceArr[nLoad];
					UINT nReadOnly	= pPoint->m_bApply ? CELL_LINE_NO : CELL_READONLY;
					CString szLoadName = pPoint->m_strName;
					long	nCnt = szLoadName.Find(szLoad);				
					if (nCnt<0) continue;
					
					szText.Format("%d", nElem+1);
					m_Grid.AddCell(nRow, nCol, &pPoint->m_strName, DT_CENTER|DT_VCENTER, CELL_READONLY);		nCol++;
					m_Grid.AddCell(nRow, nCol, "집중하중", nFormat, CELL_READONLY);nCol++;
					m_Grid.AddCell(nRow, nCol, szText, nFormat, nReadOnly);					m_Grid.SetCellDec(1);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pPoint->m_dDist, nFormat, nReadOnly);		m_Grid.SetCellDec(6);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pPoint->m_vForce.z, nFormat, nReadOnly);	m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pPoint->m_vForce.x, nFormat, nReadOnly);	m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pPoint->m_vMoment.y, nFormat, nReadOnly);	m_Grid.SetCellDec(4);	nCol++;
					
					szText.Format("-");
					m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
					m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
					m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
					m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
					m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
					m_Grid.AddCell(nRow, nCol, &pPoint->m_bApply, DT_CENTER|DT_VCENTER, -1, "적용", CELL_TYPE_CHECK);
					
					m_bCheckArr.GetAt(m_ctlTab.GetCurSel()) = pPoint->m_bApply;
					nRow++;nCol=0;
				}
			}

			if ( szLCName.Find(szLoad)>-1 && szLCName.Find("WIND")>-1 && nElem==0 )
			{
				long nIdx = (szLoad.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);
				
				szText.Format("%d", nIdx+1);
				m_Grid.AddCell(nRow, nCol++, &Force->m_szName, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, "집중하중", DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol, szText, nFormat, CELL_READONLY);				m_Grid.SetCellDec(1);	nCol++;
				m_Grid.AddCell(nRow, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);	m_Grid.SetCellDec(6);	nCol++;
				m_Grid.AddCell(nRow, nCol, &Force->m_vForce.z, nFormat, CELL_LINE_NO);	m_Grid.SetCellDec(4);	nCol++;
				m_Grid.AddCell(nRow, nCol, &Force->m_vForce.x, nFormat, CELL_LINE_NO);	m_Grid.SetCellDec(4);	nCol++;
				m_Grid.AddCell(nRow, nCol, &Force->m_vMoment.y, nFormat, CELL_LINE_NO);	m_Grid.SetCellDec(4);	nCol++;
				szText.Format("-");
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol, &Force->m_bApply, DT_CENTER|DT_VCENTER, -1, "적용", CELL_TYPE_CHECK);						
				m_bCheckArr.GetAt(m_ctlTab.GetCurSel()) = Force->m_bApply;
				nRow++;nCol=0;
			}

			if ( szLCName.Find(szLoad)>-1 && szLCName.Find("CRASH")>-1 && nElem==0 )
			{
				long nIdx = (szLoad.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);
				
				szText.Format("%d", nIdx+1);
				m_Grid.AddCell(nRow, nCol++, &Force->m_szName, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, "집중하중", DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol, szText, nFormat, CELL_READONLY);				m_Grid.SetCellDec(1);	nCol++;
				m_Grid.AddCell(nRow, nCol, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);	m_Grid.SetCellDec(6);	nCol++;
				m_Grid.AddCell(nRow, nCol, &Force->m_vForce.z, nFormat, CELL_LINE_NO);	m_Grid.SetCellDec(4);	nCol++;
				m_Grid.AddCell(nRow, nCol, &Force->m_vForce.x, nFormat, CELL_LINE_NO);	m_Grid.SetCellDec(4);	nCol++;
				m_Grid.AddCell(nRow, nCol, &Force->m_vMoment.y, nFormat, CELL_LINE_NO);	m_Grid.SetCellDec(4);	nCol++;
				szText = _T("-");
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol++, szText, DT_CENTER|DT_VCENTER, CELL_READONLY);
				m_Grid.AddCell(nRow, nCol, &Force->m_bApply, DT_CENTER|DT_VCENTER, -1, "적용", CELL_TYPE_CHECK);
				m_bCheckArr.GetAt(m_ctlTab.GetCurSel()) = Force->m_bApply;	
				nRow++; nCol=0;
			}
		}
	}

	m_Grid.UpdateData(FALSE);
	m_Grid.MergeGrid(0, 1, 0, 12);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 110);
	m_Grid.SetRedraw(TRUE, TRUE);

	UpdateData(FALSE);
}

void CCalcCantileverModeling::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CCalcCantileverModeling::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd		*pOptStd= m_pStd->GetDataManage()->GetOptionStd();
	CFEMManage			*pFEM	= m_pStd->m_pCalcStd->GetCantileverFemData(0);
	CDomyun				*pDom	= m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);
	
	CString szCell		= GetTabName();
	long	nLoadCase	= m_pStd->GetLoadCase(pFEM, szCell);
	if(nLoadCase<0)	nLoadCase = 0;

	CForceDiagram ForceDom(pFEM);
	ForceDom.SetScaleDim(0.1);
	ForceDom.SetTextHeight(1.5);

	ForceDom.DrawFrameModel(TRUE, FALSE, TRUE);
	if(szCell != "")
		ForceDom.DrawLoadCase(nLoadCase, FALSE, TRUE, 0.02, FALSE, 1);

	// 	방호벽 삽입
	m_pStd->DrawWallGuard(&ForceDom);
	m_pStd->DrawDimJoint(&ForceDom, pFEM, 1.5);
	m_pStd->DrawDimLoadCase(&ForceDom, pFEM, nLoadCase, 1.5);

	ForceDom.SetCalcExtRect();
	CDRect Bound = ForceDom.GetExtRect();
	pOptStd->SetEnvType(&ForceDom,HCAD_HIDD);
	ForceDom.Rectangle(Bound.left-ForceDom.Always(8), Bound.top-ForceDom.Always(3), Bound.right+ForceDom.Always(8), Bound.bottom+ForceDom.Always(7));

	*pDom << ForceDom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcCantileverModeling::OnPreInitDialog()
{
	SetResize(IDC_RADIO_SLAB, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_POST, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_LIVE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_WIND, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_CENTROID, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_COLLISION, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_TAB_LOADCASE, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_BUTTON_ADD_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DEL_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		
	if(!IsUsedFlag())
	{
		CFemCantileverCalc FemCantCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
		FemCantCalc.MakeCantileverModeling(0);
	}
	
	// Set Data
	m_nLoadCase = 0;
	InitTabCtrl();

	long nCur	= m_ctlTab.GetCurSel();
	BOOL bApply = m_bCheckArr.GetAt(nCur);
	CString szCase = _T("");
	CString szLoad = GetTabName();

	if (szLoad==_T(""))
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(TRUE);
		szCase.Format("%s 적용", GetTabName());
		m_ApplyForce.SetCheck(bApply);
		m_ApplyForce.SetWindowText(szCase);
	}
	
	UpdateData(FALSE);
}

void CCalcCantileverModeling::InitTabCtrl()
{
	CAPlateCalcStd	*pCalcStd		= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM			= pCalcStd->GetCantileverFemData(0);	
	
	m_ctlTab.DeleteAllItems();
	m_ctlTab.SendInitialUpdate(TRUE);
	m_ctlTab.SetPadding(CSize(6, 6));
	m_ctlTab.m_bXPBorder = FALSE;
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long	nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;
	long	nItem			= 0;
	m_bCheckArr.RemoveAll();

	for(long n=0; n<nLoadCaseSize; n++)
	{
		switch(m_nLoadCase)
		{
		case PREDEAD:
			{
				CString szLoadName = pFEM->GetLoadCaseString(n);
				
				if (szLoadName.Find("WEIGHT")>-1)
				{
					m_ctlTab.InsertItem(nItem++, "WEIGHT");
					m_bCheckArr.Add(FALSE);
					continue;
				}
			}
			break;
		case POSTDEAD:
			{
				CString szLoadName = pFEM->GetLoadCaseString(n);
				
				if (szLoadName.Find("CURB")>-1)
				{
					m_ctlTab.InsertItem(nItem++, "CURB");
					m_bCheckArr.Add(FALSE);
					continue;
				}
				if (szLoadName.Find("S-WALL")>-1)
				{
					m_ctlTab.InsertItem(nItem++, "S-WALL");
					m_bCheckArr.Add(FALSE);
					continue;
				}
				if (szLoadName.Find("FENCE")>-1)
				{
					m_ctlTab.InsertItem(nItem++, "FENCE");
					m_bCheckArr.Add(FALSE);
					continue;
				}
				if (szLoadName.Find("PAVE")>-1)
				{
					m_ctlTab.InsertItem(nItem++, "PAVE");
					m_bCheckArr.Add(FALSE);
					continue;
				}
				if (szLoadName.Find("WALKWAY")>-1)
				{
					m_ctlTab.InsertItem(nItem++, "WALKWAY");
					m_bCheckArr.Add(FALSE);
					continue;
				}
				if (szLoadName.Find("PEOPLE")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(FALSE);
					continue;
				}
			}
			break;
		case LIVE:
			{				
				CString szLoadName = pFEM->GetLoadCaseString(n);
				
				if (szLoadName.Find("DB")==0)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(FALSE);
					continue;
				}				
				if (szLoadName.Find("TANK")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(FALSE);
					continue;
				}				
				if (szLoadName.Find("TRAILER")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(FALSE);
					continue;
				}				
			}
			break;
		case WIND:
			{
				CString szLoadName = pFEM->GetLoadCaseString(n);
				if (szLoadName.Find("WIND")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(FALSE);
					continue;
				}
			}
			break;
		case CRASH:
			{
				CString szLoadName = pFEM->GetLoadCaseString(n);
				if (szLoadName.Find("CRASH")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(FALSE);
					continue;
				}
			}
			break;
		case CENTRI:
			{
				CString szLoadName = pFEM->GetLoadCaseString(n);
				if (szLoadName.Find("CF")==1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(FALSE);
					continue;
				}
			}
			break;
		}
	}

	CheckLoadApply(m_bCheckArr);
	m_ctlTab.SetCurSel(0);
}

CString CCalcCantileverModeling::GetTabName()
{
	long nCurrent = m_nLoadCase;
	UpdateData(TRUE);
	
	CString	szTabTitle	= _T("");
	TCITEM	TabCtrlItem;
	TabCtrlItem.mask		= TCIF_TEXT;
	TabCtrlItem.pszText		= szTabTitle.GetBufferSetLength(255);
	TabCtrlItem.cchTextMax	= 255;
	m_ctlTab.GetItem(m_ctlTab.GetCurSel(), &TabCtrlItem);
	
	m_nLoadCase = nCurrent;
	return szTabTitle;
}

void CCalcCantileverModeling::OnRadioPreLoad()
{
	m_nLoadCase	= PREDEAD;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();

	CString szCase = GetTabName();
	if (szCase==_T(""))
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(TRUE);
		szCase += _T("적용");
		m_ApplyForce.SetWindowText(szCase);	
		m_ApplyForce.SetCheck(m_bCheckArr.GetAt(0));
	}
}

void CCalcCantileverModeling::OnRadioPostLoad()
{
	m_nLoadCase	= POSTDEAD;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();

	CString szCase = GetTabName();
	if (szCase==_T(""))
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(TRUE);
		szCase += _T("적용");
		m_ApplyForce.SetWindowText(szCase);	
		m_ApplyForce.SetCheck(m_bCheckArr.GetAt(0));
	}
}

void CCalcCantileverModeling::OnRadioLiveLoad()
{
	m_nLoadCase	= LIVE;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();

	CString szCase = GetTabName();
	if (szCase==_T(""))
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(TRUE);
		szCase += _T("적용");
		m_ApplyForce.SetWindowText(szCase);
		m_ApplyForce.SetCheck(m_bCheckArr.GetAt(0));
	}
}


void CCalcCantileverModeling::OnRadioWindLoad()
{
	m_nLoadCase	= WIND;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();
	
	CString szCase = GetTabName();
	if (szCase==_T(""))
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(TRUE);
		szCase += _T("적용");
		m_ApplyForce.SetWindowText(szCase);	
		m_ApplyForce.SetCheck(m_bCheckArr.GetAt(0));
	}
}

void CCalcCantileverModeling::OnRadioCrashLoad()
{
	m_nLoadCase	= CRASH;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();

	CString szCase = GetTabName();
	if (szCase==_T(""))
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(TRUE);
		szCase += _T("적용");
		m_ApplyForce.SetWindowText(szCase);	
		m_ApplyForce.SetCheck(m_bCheckArr.GetAt(0));
	}
}

void CCalcCantileverModeling::OnRadioCentrifugalLoad()
{
	m_nLoadCase	= CENTRI;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();

	CString szCase = GetTabName();
	if (szCase==_T(""))
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPLY_FORCE)->ShowWindow(TRUE);
		szCase += _T("적용");
		m_ApplyForce.SetWindowText(szCase);	
		m_ApplyForce.SetCheck(m_bCheckArr.GetAt(0));
	}
}

void CCalcCantileverModeling::OnButtonAddload()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);
	
	// Check of Select Cell
	long	nRow = m_Grid.GetFocusCell().row;	
	if (nRow<0)	nRow = m_Grid.GetRowCount()-1;
	
	CString szCell		= m_Grid.GetTextMatrix(nRow, 0);
	long	nLoadCase	= m_pStd->GetLoadCase(pFEM, szCell);
	if(nLoadCase<0)	nLoadCase = m_nLoadCase;
	long	nElem	= m_Grid.GetTextMatrixLong(nRow, 2)-1;
	if(nElem<0)	nElem = 0;
	
	CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
	CLoadForce	*pForce	= pElem->GetLoadForce(nLoadCase);
	CString		szName	= m_Grid.GetTextMatrix(nRow, 0);
	CVector		vZero(0,0,0);
	
	if(pForce->IsFlag(EXIST_TRAP))
	{
		CElement	*pElemAdd	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForceAdd	= pElemAdd->GetLoadForce(nLoadCase);
		pForceAdd->SetLoadForce(0, 1, vZero, vZero, vZero, vZero, szName, vZero, FALSE);
	}
	else if(pForce->IsFlag(EXIST_DISTFORCE))
	{
		CElement	*pElemAdd	= pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pForceAdd	= pElemAdd->GetLoadForce(nLoadCase);
		pForceAdd->SetDistForce(0, vZero, szName, FALSE,  "");
	}
	SetDataInit();
	DrawInputDomyunView();
}


void CCalcCantileverModeling::OnButtonDelload()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);

	long	nRowFocus	= m_Grid.GetFocusCell().row;
	if(nRowFocus<0)	return;
	long	nElemCur	= m_Grid.GetTextMatrixLong(nRowFocus, 2)-1;
	if(nElemCur<0)	return;
	BOOL	bTrap		= m_Grid.GetTextMatrix(nRowFocus, 1)==_T("분포하중") ? TRUE : FALSE;
	long	nIndex		= GetForceIndex(nElemCur, nRowFocus, bTrap);

	CString szCell		= m_Grid.GetTextMatrix(nRowFocus, 0);
	long	nLoadCase	= m_pStd->GetLoadCase(pFEM, szCell);
	if(nLoadCase<0)	nLoadCase = m_nLoadCase;	
	if(nElemCur<0 || nIndex<0)
	{
		AfxMessageBox("삭제할 하중을 선택하십시요.");
		return;
	}
	
	CElement	*pElem	= pFEM->GetElement((unsigned short)nElemCur);
	CLoadForce	*pForce	= pElem->GetLoadForce(nLoadCase);
	if(bTrap)	DeleteForceTrap(pForce, nIndex, TRUE);
	else		DeleteForcePoint(pForce, nIndex, TRUE);

	SetDataInit();
	DrawInputDomyunView();

}

void CCalcCantileverModeling::OnCheckApplyForce()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);

	long nCur = m_ctlTab.GetCurSel();
	m_bCheckArr.GetAt(nCur) = m_ApplyForce.GetCheck();
	
	CString	szLoad		= GetTabName();
	long nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;
		
	for (long n=0; n<nLoadCaseSize; n++)
	{
		CString szLCName = pFEM->m_szLCName[n];
		for (long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
		{
			CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
			CLoadForce	*pForce	= pElem->GetLoadForce(n);
			if(!pForce)	return;
			
			if(pForce->IsFlag(EXIST_TRAP))
			{
				for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
				{
					CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
					CString szLoadName = pTrap->m_strName;
					long	nCnt = szLoadName.Find(szLoad);				
					if (nCnt<0) continue;
					
					pTrap->m_bApply = m_ApplyForce.GetCheck();					
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
					pPoint->m_bApply = m_ApplyForce.GetCheck();				
				}
			}

			if ( szLCName.Find(szLoad)>-1 && szLCName.Find("WIND")>-1 && nElem==0 )
			{
				long nApplyElement   = m_pStd->GetLoadApplyMemberCount(pFEM);
				long nIdx = (szLoad.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);			
				Force->m_bApply = m_ApplyForce.GetCheck();	
			}
			
			if ( szLCName.Find(szLoad)>-1 && szLCName.Find("CRASH")>-1 && nElem==0 )
			{
				long nApplyElement   = m_pStd->GetLoadApplyMemberCount(pFEM);
				long nIdx = (szLoad.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(n);			
				Force->m_bApply = m_ApplyForce.GetCheck();	
			}
		}
	}

	SetDataInit();
	UpdateComboData();
	DrawInputDomyunView();
}

void CCalcCantileverModeling::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataInit();
	DrawInputDomyunView();	
	m_Grid.SetFocusCell(1, 2);
	
	long nCur	= m_ctlTab.GetCurSel();
	BOOL bApply = m_bCheckArr.GetAt(nCur);
	CString szCase = _T("");
	szCase.Format("%s 적용", GetTabName());
	
	m_ApplyForce.SetCheck(bApply);
	m_ApplyForce.SetWindowText(szCase);	
}

long CCalcCantileverModeling::GetForceIndex(long nElemCur, long nCurRow, BOOL bTrap)
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		= pCalcStd->GetCantileverFemData(0);
	CElement		*pEle		= pFEM->GetElement((unsigned short)nElemCur);
	
	long	nRow = m_Grid.GetFocusCell().row;
	if (nRow<0) return 0;
	
	CString szCell		= m_Grid.GetTextMatrix(nRow, 0);
	long	nLoadCase	= m_pStd->GetLoadCase(pFEM, szCell);
	if(nLoadCase<0)	nLoadCase = m_nLoadCase;
	CLoadForce	*pForce	= pEle->GetLoadForce(nLoadCase);
	long nIndex = 0;
	
	if(bTrap)
	{
		double	dStt	= m_Grid.GetTextMatrixDouble(nCurRow, 3);
		double	dEnd	= m_Grid.GetTextMatrixDouble(nCurRow, 8);
		CVector	vTrapStt(0, 0, m_Grid.GetTextMatrixDouble(nCurRow, 4));
		CVector vTrapEnd(0, 0, m_Grid.GetTextMatrixDouble(nCurRow, 9));
		for(long n=0; n<pForce->m_TrapForceArr.GetSize(); n++)
		{
			CLoadForceTrap *pTrap = pForce->m_TrapForceArr.GetAt(n);
			
			BOOL bMatch[4] = { FALSE, FALSE, FALSE, FALSE };		
			bMatch[0] = (fabs(pTrap->m_dRStt-dStt)<EPSILON)?	TRUE : FALSE;
			bMatch[1] = (fabs(pTrap->m_dREnd-dEnd)<EPSILON)?	TRUE : FALSE;
			bMatch[2] = (pTrap->m_vTrapStt==vTrapStt)?			TRUE : FALSE;
			bMatch[3] = (pTrap->m_vTrapEnd==vTrapEnd)?			TRUE : FALSE;
			
			if (bMatch[0]&&bMatch[1]&&bMatch[2]&&bMatch[3])	return nIndex;		
			nIndex++;
		}
	}
	else
	{
		double	dDist	= m_Grid.GetTextMatrixDouble(nCurRow, 3);
		CVector	vForce(0, 0, m_Grid.GetTextMatrixDouble(nCurRow, 4));
		
		for(long n=0; n<pForce->m_PointForceArr.GetSize(); n++)
		{
			CLoadForcePoint *pPoint = pForce->m_PointForceArr.GetAt(n);
			
			BOOL bMatch[2] = { FALSE, FALSE };

			double dx = fabs(pPoint->m_vForce.x - vForce.x);
			double dy = fabs(pPoint->m_vForce.y - vForce.y);
			double dz = fabs(pPoint->m_vForce.z - vForce.z);

			bMatch[0] = (fabs(pPoint->m_dDist-dDist)<EPSILON)?		TRUE : FALSE;
			bMatch[1] = (dx<EPSILON && dy<EPSILON && dz<EPSILON)?	TRUE : FALSE;
			
			if(bMatch[0]&&bMatch[1])	return nIndex;		
			nIndex++;
		}
	}
	
	return nIndex;
}

void CCalcCantileverModeling::DeleteForcePoint(CLoadForce *pForce, long nIndex, BOOL bDelete)
{
	if(pForce->IsFlag(EXIST_DISTFORCE))
	{
		CLoadForcePoint *pPoint	= pForce->m_PointForceArr.GetAt(nIndex);
		
		pForce->m_PointForceArr.RemoveAt(nIndex);
		if(bDelete)
			delete pPoint;
	}
}

void CCalcCantileverModeling::DeleteForceTrap(CLoadForce *pForce, long nIndex, BOOL bDelete)
{
	if(pForce->IsFlag(EXIST_TRAP))
	{
		CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr.GetAt(nIndex);
		
		pForce->m_TrapForceArr.RemoveAt(nIndex);
		if(bDelete)
			delete pTrap;
	}
}

void CCalcCantileverModeling::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	if (!IsInitDialog())
		DrawInputDomyunView(TRUE);
	CCellID	cell = m_Grid.GetFocusCell();
	m_nSelectElem	= m_Grid.GetTextMatrixLong(cell.row, 2)-1;
}

void CCalcCantileverModeling::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;
	
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage		 *pFEM		= pCalcStd->GetCantileverFemData(0);
	
	if(nCol==2)
	{
		
		if(m_nSelectElem<0)	return;
		CString	szText		= _T("");
		CString szCell		= m_Grid.GetTextMatrix(nRow, 0);
		long nLoadCase		= m_pStd->GetLoadCase(pFEM, szCell);
		
		if(nLoadCase<0)	nLoadCase = m_nLoadCase;
		BOOL	bTrap		= m_Grid.GetTextMatrix(nRow, 1)==_T("분포하중") ? TRUE : FALSE;
		long	nElemTag	= m_Grid.GetTextMatrixLong(nRow, nCol)-1;
		long	nIdx		= GetForceIndex(m_nSelectElem, nRow, bTrap);
		
		if(nElemTag>pFEM->GetElementSize()-1)
		{
			nElemTag = pFEM->GetElementSize()-1;
			szText.Format("%d", nElemTag+1);
			m_Grid.SetTextMatrix(nRow, nCol, szText);
		}
		
		CElement	*pElem		= pFEM->GetElement((unsigned short)m_nSelectElem);
		CLoadForce	*pForce		= pElem->GetLoadForce(nLoadCase);
		
		CElement	*pElemTag	= pFEM->GetElement((unsigned short)nElemTag);
		CLoadForce	*pForceTag	= pElemTag->GetLoadForce(nLoadCase);
		
		if(bTrap)
		{
			if(nIdx>=pForce->m_TrapForceArr.GetSize())	return;
			CLoadForceTrap *pTrap	= pForce->m_TrapForceArr.GetAt(nIdx);
			pForceTag->m_TrapForceArr.Add(pTrap);
			pForceTag->SetEnableFlag(EXIST_TRAP);
			DeleteForceTrap(pForce, nIdx, FALSE);
		}
		else
		{
			if(nIdx>=pForce->m_PointForceArr.GetSize())	return;
			CLoadForcePoint *pPoint	= pForce->m_PointForceArr.GetAt(nIdx);
			pForceTag->m_PointForceArr.Add(pPoint);
			pForceTag->SetEnableFlag(EXIST_DISTFORCE);
			DeleteForcePoint(pForce, nIdx, FALSE);
		}
		m_nSelectElem = nElemTag;		
	}
	
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView();
}

void CCalcCantileverModeling::CheckLoadApply(CBOOLArray& bCheckArr)
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		= pCalcStd->GetCantileverFemData(0);

	for (long i=0; i<bCheckArr.GetSize(); i++)
	{
		// Seek Load Case
		CString szFemLCName   = _T("");
		CString szTabLCName = _T("");
		long	nLoadCase  =  0;
		TCITEM  TabCtrlItem;

		TabCtrlItem.mask		= TCIF_TEXT;
		TabCtrlItem.pszText		= szTabLCName.GetBufferSetLength(255);
		TabCtrlItem.cchTextMax	= 255;
		m_ctlTab.GetItem(i, &TabCtrlItem);
		nLoadCase  = m_pStd->GetLoadCase(pFEM, szTabLCName);
		szFemLCName= pFEM->m_szLCName[nLoadCase];

		// Check All Load Apply
		BOOL bAll_Apply = FALSE;
		
		for (long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
		{
			CElement*	pElem = pFEM->GetElement((unsigned short)nElem);
			CLoadForce* pForce= pElem->GetLoadForce(nLoadCase);
			if (!pForce) continue;

			if (bAll_Apply) break;
			if(pForce->IsFlag(EXIST_TRAP))
			{
				for (long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
				{
					if (bAll_Apply) continue;

					CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr[nLoad];
					CString szLoadName = pTrap->m_strName;
					long	nCnt = szLoadName.Find(szTabLCName);				
					if (nCnt<0) continue;
					if (pTrap->m_bApply) bAll_Apply = TRUE;
				}
			}

			if(pForce->IsFlag(EXIST_DISTFORCE))
			{
				for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
				{
					if (bAll_Apply) break;

					CLoadForcePoint	*pPoint	= pForce->m_PointForceArr[nLoad];
					CString szLoadName = pPoint->m_strName;
					long	nCnt = szLoadName.Find(szTabLCName);				
					if (nCnt<0) continue;
					if (pPoint->m_bApply) bAll_Apply = TRUE;
				}
			}

			if ( szFemLCName.Find(szTabLCName)>-1 && szFemLCName.Find("WIND")>-1 && nElem==0 )
			{

				if (bAll_Apply) break;
				long nApplyElement   = m_pStd->GetLoadApplyMemberCount(pFEM);
				long nIdx = (szTabLCName.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(nLoadCase);

				if (Force->m_bApply) bAll_Apply = TRUE;
			}

			if ( szFemLCName.Find(szTabLCName)>-1 && szFemLCName.Find("CRASH")>-1 && nElem==0 )
			{
				if (bAll_Apply) break;
				long nApplyElement   = m_pStd->GetLoadApplyMemberCount(pFEM);
				long nIdx = (szTabLCName.Find("L")==0)? 0 : nApplyElement;
				CJoint *pNode = pFEM->GetJoint((unsigned short)nIdx);
				CLoadForceJoint* Force = pNode->GetLoadForce(nLoadCase);
				
				if (Force->m_bApply) bAll_Apply = TRUE;
			}
		}
		bCheckArr.GetAt(i) = bAll_Apply;
	}
}

void CCalcCantileverModeling::UpdateComboData()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CFEMManage		   *pFEM = pCalcStd->GetCantileverFemData(0);

	BOOL	bMatch= FALSE;
	CCombo* pComb = NULL;
	CString szTab = GetTabName();

	// 1. seek load combination
	for (long i=0; i<pFEM->m_ComboList.GetCount(); i++)
	{
		if (bMatch) break;
		POSITION pos  = pFEM->m_ComboList.FindIndex(i);
		pComb = (CCombo*)pFEM->m_ComboList.GetAt(pos);

		for (long j=0; j<pComb->m_szLoadName.GetSize(); j++)
		{
			if (bMatch) break;
			if (szTab==pComb->m_szLoadName.GetAt(j))
				bMatch = TRUE;
		}
	}
	if (!bMatch) return;

	// 2. reset load combination
	BOOL bCombo_Apply = FALSE;

	for (long n=0; n<m_bCheckArr.GetSize(); n++)
	{
		if (bCombo_Apply) continue;
		bCombo_Apply = m_bCheckArr.GetAt(n);
	}
	pComb->m_bApply = bCombo_Apply;
}
