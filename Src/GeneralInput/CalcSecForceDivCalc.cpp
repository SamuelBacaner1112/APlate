// CalcSecForceDivCalc.cpp : implementation file
//
#include "stdafx.h"
#include "generalinput.h"
#include "CalcSecForceDivCalc.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLOOR_LOAD_DEAD					"DEAD"
#define FLOOR_LOAD_PREDEAD				"PREDEAD"
#define FLOOR_LOAD_POSTDEAD				"POSTDEAD"

#define FLOOR_LOAD_SLAB_STR				"슬래브"
#define FLOOR_LOAD_SLAB_TU_STR			"구속콘크리트"
#define FLOOR_LOAD_WALK_STR 			"보도하중"
#define FLOOR_LOAD_PAVE_STR 			"포장하중"
#define FLOOR_LOAD_GUARD_STR 			"방호벽자중"
#define FLOOR_LOAD_HANDRAIL_LEFT_STR 	"난간하중(좌측)"
#define FLOOR_LOAD_HANDRAIL_RIGHT_STR 	"난간하중(우측)"
#define FLOOR_LOAD_SOUNDWALL_LEFT_STR 	"방음벽(좌측)"
#define FLOOR_LOAD_SOUNDWALL_RIGHT_STR 	"방음벽(우측)"
#define FLOOR_LOAD_EXTRA_STR			"EXTRA"
#define FLOOR_LOAD_PEOPLE_STR			"PEOPLE"
#define FLOOR_LOAD_LIVE_DB_STR			"DB"
#define FLOOR_LOAD_LIVE_DL_STR 			"DL"
#define FLOOR_LOAD_LIVE_TANK_STR		"TANK"
#define FLOOR_LOAD_LIVE_TRAILER_STR		"TRAIL"
#define EPSILON							0.0000000001

/////////////////////////////////////////////////////////////////////////////
// CCalcSecForceDivCalc dialog


CCalcSecForceDivCalc::CCalcSecForceDivCalc(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcSecForceDivCalc::IDD, pParent)
{
 	m_bSetHLLCurSel = FALSE;
	//}}AFX_DATA_INIT
	m_nHLoadExePos	= 0;
	m_nSelectElem	= -1;
}

void CCalcSecForceDivCalc::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcSecForceDivCalc)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_APPLY_FORCE, m_ApplyForce);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_BUTTON_ADDLOAD, m_btnDelLoad);
	DDX_Control(pDX, IDC_BUTTON_DELLOAD, m_btnAddLoad);
	DDX_Control(pDX, IDC_LIVELOADPRE_SELECT, m_btnLiveLoadPre);
	DDX_Control(pDX, IDC_COMBO_SPAN, m_ctrlHLLStation);
	DDX_Radio(pDX,	IDC_RADIO_PREDEAD, m_nLoadCase);
	DDX_Control(pDX, IDC_TAB_HL, m_ctlTab);
	
}


BEGIN_MESSAGE_MAP(CCalcSecForceDivCalc, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcSecForceDivCalc)
	ON_BN_CLICKED(IDC_LIVELOADPRE_SELECT, OnLiveloadpreSelect)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_PREDEAD, OnRadioPredead)
	ON_BN_CLICKED(IDC_RADIO_POSTDEAD, OnRadioPostdead)
	ON_BN_CLICKED(IDC_RADIO_LIVE, OnRadioLive)
	ON_BN_CLICKED(IDC_BUTTON_ADDLOAD, OnButtonAddload)
	ON_BN_CLICKED(IDC_BUTTON_DELLOAD, OnButtonDelload)
	ON_BN_CLICKED(IDC_CHECK_APPLY_FORCE, OnCheckApplyForce)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_HL, OnTabSelchange)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_CBN_SELCHANGE(IDC_COMBO_SPAN, &CCalcSecForceDivCalc::OnSelchangeComboSpan)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPAN, &CCalcSecForceDivCalc::OnDeltaposSpinSpan)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcSecForceDivCalc message handlers

// 횡분배 재계산
void CCalcSecForceDivCalc::SetDataDefault() 
{
	CCalcData  *pData = m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor *pFloor= m_pStd->m_pCalcStd->m_pCalcFloor;	
	CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
//	for(long n=0; n<pData->m_HLsu; n++)
//		pFloor->SetFloorStatus(n);

	
	pData->m_bRunCross = FALSE;

	pData->m_CurLiveHL = 0;
	
	HLLCalc.RunCrossAnalysis();

	SetLoadName();
	InitTabCtrl();
	SetComboData();
	DrawInputDomyunView();

	UpdateData(FALSE);
}

void CCalcSecForceDivCalc::SetDataInit()
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	long			nSel	= m_ctrlHLLStation.GetCurSel()<0 ? 0 : m_ctrlHLLStation.GetCurSel();
	CFEMManage		*pFEM	= pCalcStd->GetHLLFemData(nSel);

	// Set Grid
	m_Grid.SetGridDefault(2, 0, 2, 0);

	long	nRow			= 0;
	long	nCol			= 0;
	UINT	nFormat			= DT_CENTER | DT_VCENTER;
	CString	szText			= _T("");

	m_Grid.AddCell(nRow, nCol++, "하중명\n");
	m_Grid.AddCell(nRow, nCol++, "하중종류\n");
	m_Grid.AddCell(nRow, nCol++, "부재명\n");
	m_Grid.AddCell(nRow, nCol++, "위치①\n");
	m_Grid.AddCell(nRow, nCol++, "하중크기①");
	m_Grid.AddCell(nRow, nCol++, "하중크기①");
	m_Grid.AddCell(nRow, nCol++, "하중크기①");
	m_Grid.AddCell(nRow, nCol++, "부재명\n");
	m_Grid.AddCell(nRow, nCol++, "위치②\n");
	m_Grid.AddCell(nRow, nCol++, "하중크기②");
	m_Grid.AddCell(nRow, nCol++, "하중크기②");
	m_Grid.AddCell(nRow, nCol++, "하중크기②");
	m_Grid.AddCell(nRow, nCol++, "적용\n");
	nRow++; nCol=0;

	m_Grid.AddCell(nRow, nCol++, "하중명\n");
	m_Grid.AddCell(nRow, nCol++, "하중종류\n");
	m_Grid.AddCell(nRow, nCol++, "부재명\n");
	m_Grid.AddCell(nRow, nCol++, "위치①\n");
	m_Grid.AddCell(nRow, nCol++, "Uz");
	m_Grid.AddCell(nRow, nCol++, "Ux");
	m_Grid.AddCell(nRow, nCol++, "Ry");
	m_Grid.AddCell(nRow, nCol++, "부재명\n");
	m_Grid.AddCell(nRow, nCol++, "위치②\n");
	m_Grid.AddCell(nRow, nCol++, "Uz");
	m_Grid.AddCell(nRow, nCol++, "Ux");
	m_Grid.AddCell(nRow, nCol++, "Ry");
	m_Grid.AddCell(nRow, nCol++, "적용\n");
	
	nRow++;	nCol=0;

	CString	szLoad		= GetTabName();
	long nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;

	for(long n=0; n<nLoadCaseSize; n++)
	{
		CString	szLoadCase	= pFEM->GetLoadCaseString(n);
		if(szLoadCase!=szLoad) continue;
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
					UINT nReadOnly	= pTrap->m_bApply ? CELL_LINE_NO : CELL_READONLY;
					
					m_Grid.AddCell(nRow, nCol++, &pTrap->m_strName, nFormat, CELL_READONLY);
					m_Grid.AddCell(nRow, nCol, "분포하중", nFormat, CELL_READONLY);nCol++;
					szText.Format("%d", nElem+1);
					m_Grid.AddCell(nRow, nCol, szText, nFormat, nReadOnly);							m_Grid.SetCellDec(1);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_dRStt, nFormat, nReadOnly);				m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapStt.z, nFormat, nReadOnly);			m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapStt.x, nFormat, nReadOnly);			m_Grid.SetCellDec(4);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_vTrapRotateStt.y, nFormat, nReadOnly);		m_Grid.SetCellDec(4);	nCol++;
					
					m_Grid.AddCell(nRow, nCol, szText, nFormat, nReadOnly);							m_Grid.SetCellDec(1);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pTrap->m_dREnd, nFormat, nReadOnly);				m_Grid.SetCellDec(4);	nCol++;
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
					
					szText.Format("%d", nElem+1);
					m_Grid.AddCell(nRow, nCol, &pPoint->m_strName, DT_CENTER|DT_VCENTER, CELL_READONLY);		nCol++;
					m_Grid.AddCell(nRow, nCol, "집중하중", nFormat, CELL_READONLY);nCol++;
					m_Grid.AddCell(nRow, nCol, szText, nFormat, nReadOnly);					m_Grid.SetCellDec(1);	nCol++;
					m_Grid.AddCell(nRow, nCol, &pPoint->m_dDist, nFormat, nReadOnly);		m_Grid.SetCellDec(4);	nCol++;
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
		}
	}

	m_Grid.UpdateData(FALSE);
	m_Grid.MergeGrid(0, 1, 0, 12);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 110);
	m_Grid.SetRedraw(TRUE, TRUE);

	UpdateData(FALSE);
}

void CCalcSecForceDivCalc::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CCalcSecForceDivCalc::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd	*pOptStd	=	m_pStd->GetDataManage()->GetOptionStd();
	CFEMManage		*pFEM		=	m_pStd->m_pCalcStd->GetHLLFemData(m_ctrlHLLStation.GetCurSel());
	CDomyun			*pDom		=	m_pView->GetDomyun();

	CAPlateDrawCalc		DrawCalc(m_pStd->m_pDataManage);

	pDom->ClearEtt(TRUE);
	
 	CString	szCell		= GetTabName();//m_Grid.GetTextMatrix(2, 0);
	long	nLoadCase	= m_pStd->GetLoadCase(pFEM, szCell);

	if(nLoadCase<0)		nLoadCase = m_nLoadCase;
	
	// 부재 및 하중표현
	CForceDiagram ForceDom(pFEM);
	ForceDom.CopyAttrWnd(pDom);
	ForceDom.ClearEtt(TRUE);
	ForceDom.SetScaleDim(0.1);
	ForceDom.SetTextHeight(1.5);

	ForceDom.DrawFrameModel(TRUE, FALSE, TRUE);
	ForceDom.DrawLoadCase(nLoadCase, FALSE, TRUE, 0.02, FALSE, 1);

	// 	방호벽 삽입
	m_pStd->DrawWallGuard(&ForceDom);
	m_pStd->DrawDimJoint(&ForceDom, pFEM, 1.5);
	m_pStd->DrawDimLoadCase(&ForceDom, pFEM, nLoadCase, 1.5);

	ForceDom.SetCalcExtRect();
	CDRect Bound = ForceDom.GetExtRect();
	pOptStd->SetEnvType(&ForceDom, HCAD_HIDD);
	ForceDom.Rectangle(Bound.left-ForceDom.Always(8), Bound.top-ForceDom.Always(3), Bound.right+ForceDom.Always(8), Bound.bottom+ForceDom.Always(7));

	*pDom << ForceDom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcSecForceDivCalc::SetLoadName()
{
	// 합성전 고정하중 이름 없는 부분 수정
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;

	long	nSel = max(m_ctrlHLLStation.GetCurSel(), 0);
	CFEMManage		*pFEM		= pCalcStd->GetHLLFemData(nSel);

	// 활하중 이름 없는 부분 수정
	long nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;

	for (long n=0; n<nLoadCaseSize; n++)
	{
		BOOL bMatch = FALSE;
		if(pFEM->GetLoadCaseString(n)==FLOOR_LOAD_PREDEAD || pFEM->GetLoadCaseString(n)==FLOOR_LOAD_POSTDEAD) continue;		
		CString szLoadName	= pFEM->GetLoadCaseString(n);
		CString szLC		= pFEM->m_szLCName[n];

		if (szLoadName.Find(FLOOR_LOAD_LIVE_DB_STR)>-1)			bMatch = TRUE;
		if (szLoadName.Find(FLOOR_LOAD_LIVE_DL_STR)>-1)			bMatch = TRUE;
		if (szLoadName.Find(FLOOR_LOAD_LIVE_TANK_STR)>-1)		bMatch = TRUE;
		if (szLoadName.Find(FLOOR_LOAD_PEOPLE_STR)>-1)			bMatch = TRUE;
		if (szLoadName.Find(FLOOR_LOAD_LIVE_TRAILER_STR)>-1)	bMatch = TRUE;
		if (!bMatch) continue;

		for(long nEle=0; nEle<pFEM->GetElementSize(); nEle++)
		{
			CElement	*pElem	= pFEM->GetElement((unsigned short)nEle);
			CLoadForce	*pForce	= pElem->GetLoadForce(n);

			
			if(pForce->IsFlag(EXIST_TRAP))
			{
				for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
				{
					CString& szName = pForce->m_TrapForceArr.GetAt(nLoad)->m_strName;
					szName = szLoadName;
				}
			}
			if(pForce->IsFlag(EXIST_DISTFORCE))
			{
				for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
				{
					CString& szName = pForce->m_PointForceArr.GetAt(nLoad)->m_strName;
					szName = szLoadName;
				}
			}
		}
	}
}

void CCalcSecForceDivCalc::OnPreInitDialog()
{
 	m_bSetHLLCurSel=FALSE;

	SetResize(IDC_RADIO_PREDEAD, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_POSTDEAD, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_LIVE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_SPAN, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_SPIN_SPAN, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_TAB_HL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_ADDLOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELLOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_LIVELOADPRE_SELECT, SZ_TOP_LEFT, SZ_TOP_LEFT);

	if(!IsUsedFlag())
	{
		CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
		HLLCalc.RunCrossAnalysis();
	}

	// Set Data
	m_nLoadCase = 0;
	m_LoadPoint.clear();
	m_LoadPoint_Dead.clear();

	SetLoadName();
	InitTabCtrl();
	SetComboData();

	long nCur	= m_ctlTab.GetCurSel();
	BOOL bApply = m_bCheckArr.GetAt(nCur);
	CString szCase = _T("");
	szCase.Format("%s 적용", GetTabName());
	m_ApplyForce.SetCheck(bApply);
	m_ApplyForce.SetWindowText(szCase);	

 	UpdateData(FALSE);
}

void CCalcSecForceDivCalc::InitTabCtrl()
{
	long nHLLCase = m_ctrlHLLStation.GetCurSel()<0 ? 0 : m_ctrlHLLStation.GetCurSel();
	CAPlateCalcStd	*pCalcStd		= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM			= pCalcStd->GetHLLFemData(nHLLCase);	
	
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
		CString szLoadName = pFEM->GetLoadCaseString(n);
		switch(m_nLoadCase)
		{
		case PREDEAD:
			{
				if(szLoadName.Find("BEFORE")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//FLOOR_LOAD_SLAB_STR);
					m_bCheckArr.Add(TRUE);
				}
				if (szLoadName.Find("BEF_TU")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//FLOOR_LOAD_SLAB_TU_STR);
					m_bCheckArr.Add(TRUE);
				}
			}
			break;
		case POSTDEAD:
			{				
				if(szLoadName.Find("WALK")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//FLOOR_LOAD_WALK_STR);
					m_bCheckArr.Add(TRUE);
				}
				if (szLoadName.Find("PAVE")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//FLOOR_LOAD_PAVE_STR);
					m_bCheckArr.Add(TRUE);
					continue;
				}
				if (szLoadName.Find("GUARD")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//FLOOR_LOAD_GUARD_STR);
					m_bCheckArr.Add(TRUE);
					continue;
				}
				if (szLoadName.Find("SOUNDL")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//"SOUNDL");
					m_bCheckArr.Add(TRUE);
					continue;
				}
				if (szLoadName.Find("SOUNDR")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//"SOUNDR");
					m_bCheckArr.Add(TRUE);
					continue;
				}
				if (szLoadName.Find("HANDL")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//"HANDL");
					m_bCheckArr.Add(TRUE);
					continue;
				}
				if (szLoadName.Find("HANDR")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//"HANDR");
					m_bCheckArr.Add(TRUE);
					continue;
				}
				if (szLoadName.Find("EXTRA")>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);//FLOOR_LOAD_EXTRA_STR);
					m_bCheckArr.Add(TRUE);
					continue;
				}
			}
			break;
		case LIVE:
			{
				if(pFEM->GetLoadCaseString(n)==FLOOR_LOAD_PREDEAD || pFEM->GetLoadCaseString(n)==FLOOR_LOAD_POSTDEAD) continue;

				CString szLoadName = pFEM->GetLoadCaseString(n);				
				if (szLoadName.Find(FLOOR_LOAD_LIVE_DB_STR)==0)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(TRUE);
				}				
				if (szLoadName.Find(FLOOR_LOAD_LIVE_DL_STR)==0)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(TRUE);
				}			
				if (szLoadName.Find(FLOOR_LOAD_LIVE_TANK_STR)>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(TRUE);
				}				
				if (szLoadName.Find(FLOOR_LOAD_PEOPLE_STR)>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(TRUE);
				}
				if (szLoadName.Find(FLOOR_LOAD_LIVE_TRAILER_STR)>-1)
				{
					m_ctlTab.InsertItem(nItem++, szLoadName);
					m_bCheckArr.Add(TRUE);
				}	
			}
			break;
		}
	}
	m_ctlTab.SetCurSel(0);
}

void CCalcSecForceDivCalc::InsertTabItem(long nItem, CString szLoadCase)
{
	m_ctlTab.InsertItem(nItem, szLoadCase);
}

BOOL CCalcSecForceDivCalc::ExistLoadCase(CString szLoadCase)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	long				nSel = m_ctrlHLLStation.GetCurSel()<0 ? 0 : m_ctrlHLLStation.GetCurSel();
	CFEMManage		   *pFEM = pCalcStd->GetHLLFemData(nSel);


	for (long nCase=0; nCase<2; nCase++)
	{
		if (nCase==1 && !pDB->IsTUGir()) continue;

		for(long nEle=0; nEle<pFEM->GetElementSize(); nEle++)
		{
			CElement	*pElem	= pFEM->GetElement((unsigned short)nEle);
			CLoadForce	*pForce	= pElem->GetLoadForce(m_nLoadCase+nCase);
			
			if(pForce->IsFlag(EXIST_TRAP))
			{
				for(long nLoad=0; nLoad<pForce->m_TrapForceArr.GetSize(); nLoad++)
				{
					CString szName = pForce->m_TrapForceArr.GetAt(nLoad)->m_strName;
					long nCnt = szName.Find(szLoadCase);
					if(szName.Find(szLoadCase)>-1) return TRUE;
				}
			}
			if(pForce->IsFlag(EXIST_DISTFORCE))
			{
				for(long nLoad=0; nLoad<pForce->m_PointForceArr.GetSize(); nLoad++)
				{
					CString szName = pForce->m_PointForceArr.GetAt(nLoad)->m_strName;
					long nCnt = szName.Find(szLoadCase);
					if(szName.Find(szLoadCase)>-1) return TRUE;
				}
			}
		}	
	}
	
	return FALSE;
}

void CCalcSecForceDivCalc::SetComboData()
{
	CDataManage *pDataManage = m_pStd->GetDataManage();
	CCalcData *pData = pDataManage->GetCalcData();
	
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CString szText="";
	m_ctrlHLLStation.ResetContent();
	
	double dStation = 0;
	for(long n=0; n<pCalcStd->GetSizeHLLFemData(); n++)
	{		
		dStation = pData->m_dStationHLLPos[n];
		szText.Format("횡분배 %d - %s",n+1, GetStationForStringOut(frM(dStation),3,TRUE,TRUE));
		m_ctrlHLLStation.InsertString(n,szText);
	}
	m_ctrlHLLStation.SetCurSel(0);
	
	UpdateData(FALSE);
}

CString CCalcSecForceDivCalc::GetTabName()
{
	UpdateData(TRUE);

	CString	szTabTitle	= _T("");
	TCITEM	TabCtrlItem;
	TabCtrlItem.mask		= TCIF_TEXT;
	TabCtrlItem.pszText		= szTabTitle.GetBufferSetLength(255);
	TabCtrlItem.cchTextMax	= 255;
	m_ctlTab.GetItem(m_ctlTab.GetCurSel(), &TabCtrlItem);
	
	return szTabTitle;
}

void CCalcSecForceDivCalc::OnRadioPredead()
{
	m_nLoadCase	= PREDEAD;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();
	
	CString szCase = GetTabName();
	szCase += _T("적용");
	m_ApplyForce.SetWindowText(szCase);	
}

void CCalcSecForceDivCalc::OnRadioPostdead()
{
	m_nLoadCase	= POSTDEAD;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();

	CString szCase = GetTabName();
	szCase += _T("적용");
	m_ApplyForce.SetWindowText(szCase);	
}

void CCalcSecForceDivCalc::OnRadioLive()
{
	m_nLoadCase	= LIVE;
	InitTabCtrl();
	SetDataInit();
	DrawInputDomyunView();
	
	CString szCase = GetTabName();
	szCase += _T("적용");
	m_ApplyForce.SetWindowText(szCase);	
}

void CCalcSecForceDivCalc::OnButtonAddload()
{
	CAPlateCalcStd	*pCalcStd = m_pStd->m_pCalcStd;
	long				nSel = m_ctrlHLLStation.GetCurSel()<0 ? 0 : m_ctrlHLLStation.GetCurSel();
	CFEMManage		*pFEM = pCalcStd->GetHLLFemData(nSel);

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
		pForceAdd->SetDistForce(0, vZero, szName, FALSE, "");
	}
	SetDataInit();
	DrawInputDomyunView();
}

void CCalcSecForceDivCalc::OnButtonDelload()
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	long				nSel = m_ctrlHLLStation.GetCurSel()<0 ? 0 : m_ctrlHLLStation.GetCurSel();
	CFEMManage		   *pFEM = pCalcStd->GetHLLFemData(nSel);

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

void CCalcSecForceDivCalc::OnCheckApplyForce()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	long				nSel = m_ctrlHLLStation.GetCurSel()<0 ? 0 : m_ctrlHLLStation.GetCurSel();
	CFEMManage		   *pFEM = pCalcStd->GetHLLFemData(nSel);

	long nCur = m_ctlTab.GetCurSel();
	m_bCheckArr.GetAt(nCur) = m_ApplyForce.GetCheck();
	
	CString	szLoad		= GetTabName();
	long	nLoadCase	= m_nLoadCase;
	if (pDB->IsTUGir() && szLoad!=FLOOR_LOAD_SLAB_STR) nLoadCase++;
	if(nLoadCase<0)	nLoadCase = m_nLoadCase;
	long nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;
		
	for (long n=0; n<nLoadCaseSize; n++)
	{
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
		}
	}

	SetDataInit();
	DrawInputDomyunView();

	CFEMAnalysis Analysis(NULL, pFEM);
	Analysis.SetWriteM3t(pCalcStd->m_bMasterLock);	
	if(!Analysis.RunFemAnalysisOnASolver())
	{
		AfxMessageBox("계산 수행 중 오류가 있습니다 !");
		return;
	}
}

void CCalcSecForceDivCalc::OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCalcSecForceDivCalc::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	if (!IsInitDialog())
		DrawInputDomyunView(TRUE);
	CCellID	cell = m_Grid.GetFocusCell();
	m_nSelectElem	= m_Grid.GetTextMatrixLong(cell.row, 2)-1;
}

void CCalcSecForceDivCalc::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	long			nSel		= max(m_ctrlHLLStation.GetCurSel(),0);
	CFEMManage		 *pFEM		= pCalcStd->GetHLLFemData(nSel);
	
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
	
	CFEMAnalysis Analysis(NULL, pFEM);
	Analysis.SetWriteM3t(pCalcStd->m_bMasterLock);	
	if(!Analysis.RunFemAnalysisOnASolver())
	{
		AfxMessageBox("계산 수행 중 오류가 있습니다 !");
		return;
	}
}

long CCalcSecForceDivCalc::GetForceIndex(long nElemCur, long nCurRow, BOOL bTrap)
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	long			nSel		= m_ctrlHLLStation.GetCurSel()<0 ? 0 : m_ctrlHLLStation.GetCurSel();
	CFEMManage		*pFEM		= pCalcStd->GetHLLFemData(nSel);
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
			bMatch[0] = (fabs(pPoint->m_dDist-dDist)<EPSILON)?	TRUE : FALSE;
			bMatch[1] = (pPoint->m_vForce==vForce)?				TRUE : FALSE;

			if(bMatch[0]&&bMatch[1])	return nIndex;		
			nIndex++;
		}
	}

	return nIndex;
}

void CCalcSecForceDivCalc::DeleteForcePoint(CLoadForce *pForce, long nIndex, BOOL bDelete)
{
	if(pForce->IsFlag(EXIST_DISTFORCE))
	{
		CLoadForcePoint *pPoint	= pForce->m_PointForceArr.GetAt(nIndex);
		
		pForce->m_PointForceArr.RemoveAt(nIndex);
		if(bDelete)
			delete pPoint;
	}	
}

void CCalcSecForceDivCalc::DeleteForceTrap(CLoadForce *pForce, long nIndex, BOOL bDelete)
{
	if(pForce->IsFlag(EXIST_TRAP))
	{
		CLoadForceTrap	*pTrap	= pForce->m_TrapForceArr.GetAt(nIndex);
		
		pForce->m_TrapForceArr.RemoveAt(nIndex);
		if(bDelete)
			delete pTrap;
	}
}

void CCalcSecForceDivCalc::OnLiveloadpreSelect() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	
	pData->m_CurLiveHL = m_ctrlHLLStation.GetCurSel();
}



void CCalcSecForceDivCalc::OnSelchangeComboSpan()
{
	SetDataInit();
	DrawInputDomyunView();
}


void CCalcSecForceDivCalc::OnDeltaposSpinSpan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;

	BOOL bMovePrev = pNMUpDown->iDelta == 1 ? TRUE : FALSE ;
	
	long nSelect = 0;
	long nSelcetPrev = m_ctrlHLLStation.GetCurSel();

	if(bMovePrev)
		nSelect = nSelcetPrev - 1 < 0 ? 0 : nSelcetPrev - 1;
	else
		nSelect = nSelcetPrev + 1 > pCalcStd->GetSizeHLLFemData() - 1 ? pCalcStd->GetSizeHLLFemData()-1 : nSelcetPrev + 1;

	m_ctrlHLLStation.SetCurSel(nSelect);
	SetDataInit();
	DrawInputDomyunView();
}



