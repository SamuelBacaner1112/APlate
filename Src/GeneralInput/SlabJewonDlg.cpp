// SlabJewonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "SubSlabHunchInputDlg.h"
#include "SubInputTensionBarDlg.h"
#include "SlabStructureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlabJewonDlg dialog
SHGRIDTITLE CSlabJewonDlg::m_gt[9] = 
{
	{"구분",						"%s",	70,	DT_CENTER,	TRUE},
	{"포장두께\n(A)",				"%.f",	80,	DT_CENTER,	FALSE},
	{"슬래브두께\n(T)",				"%.f",	80, DT_CENTER,	FALSE},
	{"헌치높이\n(H)",				"%.f",	80, DT_CENTER,	FALSE},
	{"헌치구배\n(1 : X)",			"%.f",	80, DT_CENTER,	FALSE},
	{"슬래브좌측\n두께(T1)",		"%.f",	80,	DT_CENTER,	FALSE},
	{"슬래브우측\n두께(T2)",		"%.f",	80,	DT_CENTER,	FALSE},
	{"슬래브돌출\n길이(W1)",		"%.f",	80,	DT_CENTER,	FALSE},
	{"슬래브두께\n기준위치(W)",		"%.f",	90,	DT_CENTER,	FALSE},	
};

CSlabJewonDlg::CSlabJewonDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CSlabJewonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSlabJewonDlg)
	m_dWidthGuardFenceL			=   0.0;
	m_dWidthGuardFenceR			=   0.0;
	m_dWidthMarginalStripeL		=   0.0;
	m_dWidthMarginalStripeR		=   0.0;
	m_dWidthShoulderL			=   0.0;
	m_dWidthShoulderR			=   0.0;
	m_dWidthLane				=   0.0;
	m_bLevelCenter				= TRUE;
	//}}AFX_DATA_INIT
	m_strLevel.Add("없  음");
	m_strLevel.Add("Level" );

	m_strType.Add("없  음");
	m_strType.Add("방호벽");
	m_strType.Add("중분대");
	m_strType.Add("보  도");

	m_strSwitch.Add("없  음");
	m_strSwitch.Add("있  음");
	// 정렬을 위한 정의
}


void CSlabJewonDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSlabJewonDlg)
 	DDX_Control(pDX, ID_BUTTON_SLABHUNCH, m_SlabHunch);
	DDX_Control(pDX, IDC_BUTTON_TENDON, m_Tendon);
	DDX_Control(pDX, IDC_COMBO_POS, m_ctlCombo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRIDN, m_GridT);
	DDX_Check(pDX, IDC_LEVEL_ROADCENTER, m_bLevelCenter);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SLAB_STRUCTURE, m_btnSlabStruct);
}

BEGIN_MESSAGE_MAP(CSlabJewonDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CSlabJewonDlg)
	ON_BN_CLICKED(IDC_LEVEL_ROADCENTER, OnLevelRoadcenter)
	ON_BN_CLICKED(ID_BUTTON_SLABHUNCH, OnButtonSlabhunch)
	ON_BN_CLICKED(IDC_BUTTON_TENDON, OnButtonTendon)
	ON_CBN_SELCHANGE(IDC_COMBO_POS, OnSelchangeComboPos)
	ON_BN_CLICKED(IDC_BUTTON_SLAB_STRUCTURE, OnButtonSlabStructure)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRIDN, OnCellChangedDataGridT)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRIDN, STMakeCursor)
	ON_MESSAGE(WM_USER_BUTTON_PUSH, OnReceive)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlabJewonDlg message handlers


void CSlabJewonDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	SetDataSave();
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CARoadOptionStd *pOpt    = m_pStd->GetDataManage()->GetOptionStd();

	CDomyun *pDom = m_pView->GetDomyun();	
	pDom->ClearEtt(TRUE);	

	CDomyun Dom(pDom);	
	pOpt->SetEnvType(&Dom, HCAD_STLC);

	CAPlateDrawDanmyun DrawDanmyun(m_pStd->GetDataManage());
	CAPlateDrawCross DrawCross(m_pStd->GetDataManage());
	// 상부 플래지의 두께가 가장 큰 위치의 공장이음 시점에서 교량진행방향으로 가장 가까운 가로보위치(nDir=1)
	CPlateBasicIndex *pBx = NULL;
	if(m_nTab==0)		pBx = GetPosBx();
	else if(m_nTab==1)	pBx = pDB->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	else				pBx = pDB->GetGirder(0)->GetCrossBxAtMinFlangeWidth();

	// 슬래브 중앙부를 그림(2번-가로보위치)
	pOpt->SetEnvType(&Dom, HCAD_STLC);
	DrawDanmyun.DrawGuardWall(&Dom, pBx);	// 방호벽	
	DrawDanmyun.DrawDanmyunOnBx(&Dom, pBx, FALSE, TRUE, CAPlateDrawCross::NOTDRAWVSTIFF);	// 슬래브 단면
	DrawDanmyun.DrawTensionBar(&Dom, pBx, TRUE);	// 지점부 슬래브 단면	
	///< 치수선
	Dom.SetCalcExtRect();
	CDRect BoundRect  = Dom.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 40*(dDiagonalL/8500);
	Dom.SetScaleDim(dScale);
	DrawDanmyun.DrawInputSlabDimDanmyun(&Dom, pBx);	
	// Slop
	pOpt->SetEnvType(&Dom, HCAD_DIML);
	DrawDanmyun.DimTextSlopeArrow(&Dom, pBx);

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}
	
void CSlabJewonDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateBxFinder Finder(m_pStd->GetBridge()->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CGlobarOption   *pGlopt  = m_pStd->GetDataManage()->GetGlobalOption();
	
	double dWidthFlange = pDB->GetGirder(0)->GetWidthFlangeUpper(pBx);
	double dWidthSlab	= pDB->GetWidthSlabDom(pBx);

	m_dWidthGuardFenceL		= 450.0;// 방호벽 Seat 폭
	m_dWidthGuardFenceR		= 450.0;
	m_dWidthMarginalStripeL = 50.0;// 측대 폭
	m_dWidthMarginalStripeR = 50.0;
	m_dWidthLane			= 3500.0;// 차선 폭
	m_dWidthShoulderL		= 1000.0;// 길어깨 폭
	m_dWidthShoulderR		= 1000.0;
	
	double dWidthShoulderEx = m_dWidthGuardFenceL + m_dWidthGuardFenceR + m_dWidthShoulderL + m_dWidthShoulderR;
	long   nQtyLane         = (long)((dWidthSlab - dWidthShoulderEx) / m_dWidthLane);
	if(nQtyLane%2 != 0.0) nQtyLane++;

// 6월 10일 발표용 TU 권고안 (by. 이종원부장님)
	if(pDB->IsTUGir())	m_nQtyHDanNode = nQtyLane + 3;
	else				m_nQtyHDanNode = nQtyLane + 4;

	// 차선 수, 폭에 의한 길어깨 재설정
	m_dWidthShoulderL = (dWidthSlab - (m_dWidthGuardFenceL+m_dWidthGuardFenceR+nQtyLane*m_dWidthLane)) / 2;
	m_dWidthShoulderR = (dWidthSlab - (m_dWidthGuardFenceL+m_dWidthGuardFenceR+nQtyLane*m_dWidthLane)) / 2;

	CString strWidth(_T("")), strLevel(_T("")), strType(_T("")), strSwitch(_T(""));

	for(long n = 1; n < m_nQtyHDanNode + 1; n++)
	{
		if(n == 1 || n == m_nQtyHDanNode)
		{
			double dWidthGuardFence = (n == 1)	? m_dWidthGuardFenceL
												: m_dWidthGuardFenceR;
			strWidth.Format("%g", dWidthGuardFence);
			strLevel  = "Level" ;
			strType	  = "방호벽";
			strSwitch = "없  음";			
		}
		else if(n == 2 || n == m_nQtyHDanNode - 1)
		{
			double dWidthShoulder = (n == 2)	? m_dWidthShoulderL
												: m_dWidthShoulderR;
			strWidth.Format("%g", dWidthShoulder);
			strLevel  = "없  음";
			strType	  = "없  음";
			strSwitch = "있  음";
		}
		else
		{
			strWidth.Format("%g", m_dWidthLane);
			strLevel  = "없  음";
			strType	  = "없  음";
			strSwitch = "없  음";
		}
// 6월 10일 발표용 TU 권고안 (by. 이종원부장님)
		if(pDB->IsTUGir() && (n==(m_nQtyHDanNode+1)/2))
		{
			strWidth.Format("%d", 570);
			strLevel  = "Level" ;
			strType	  = "중분대";
			strSwitch = "없  음";
		}
		pDB->SetLengthHDanByLine(atof(strWidth), n-1);
		pDB->SetValueTypeHDan(n-1, 0, GetStringValue(strLevel));
		pDB->SetValueTypeHDan(n-1, 1, GetStringValue(strType));
		pDB->SetValueTypeHDan(n-1, 2, GetStringValue(strSwitch));
	}

	pDB->m_cLevelRoadCenterToSlabEdge = TRUE;
	pDB->m_dThickSlabLeft			= 250.0;
	pDB->m_dThickSlabRight			= 250.0;
	if(!pDB->IsTUGir())	// TU거더는 구속콘크리트 제원에서 권고안
		pDB->m_dThickSlabBase		= 300.0;
	pDB->m_dWidthHunch				= 50;
	pDB->m_dSlopHunch				= 5;
	pDB->m_dHunchDisForBaseThick	= 50;

	if(pDB->IsTUGir())
	{
		double dWGir	 = pDB->GetGirder(-1)->GetWidthOnStation(pBx->GetStation(), TRUE);
		double dWMaxGir  = pDB->GetGirder(-1)->GetMaxWidthFlange(TRUE);
		double dWidthGap = (dWMaxGir -dWGir)/2; // 최대플랜지 폭과 현재 플랜지폭 차의 반 

		pDB->m_dWidthHunch = 0;
		pDB->m_dHunchDisForBaseThick = -dWidthGap-dWidthFlange/2;
	}

	pDB->m_dThickPave				= 80;
	pDB->m_dThickSlabHunch			= (pGlopt->IsLBDeck() || pDB->IsTUGir()) ? 0 : 50;
	pDB->SetQtyHDanNode(m_nQtyHDanNode);
	
	UpdateData(FALSE);

	//초기에 방음벽 그려주기 위해서..
	SetDataSaveGuard();
	for( long nGuard = 0; nGuard < pDB->GetGuardWallSu(); nGuard++ )
	{			
		SetDataDefaultGuard(nGuard);
	}

	//초기에 강선 처리 하기 위해...
	if(!pDB->IsPlateGir())
	{
		pDB->m_bTendonInputComplete = TRUE;
		m_Tendon.SetCheck(TRUE);
		UpdateData(FALSE);	
	
		OnCheckTendon();
		CSubInputTensionBarDlg TensinonInputDlg(m_pStd, pDB, GetParent());
		TensinonInputDlg.SetDataDefault();		

		//변곡점이 곡선 형태를 잡기 전에 필요한 횡단 정보를 
		//설정하기 위해  SetDataInit(), SetDataSave()를 수행함.

		SetDataInit();
		SetDataSave();	
	}

	if(!IsUsedFlag())
	{
		CSlabStructureDlg	Dlg;
		Dlg.m_pDB	= m_pStd->GetBridge();
		Dlg.SetDataDefault();
		
	}

}

//방음벽 권고안...
void CSlabJewonDlg::SetDataDefaultGuard(long nGuard)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CCentSeparation *pGuard = pDB->GetGuardWall(nGuard);

	long nHDan = pDB->GetNumHDanByGuardFence(pGuard);

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long bLeft = (nHDan < (pDB->GetQtyHDanNode()/2)) ? TRUE : FALSE;

	pGuard->SetDataDefault1(pGuard,nGuard,bLeft);
}

void CSlabJewonDlg::SetDataInit()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	m_bLevelCenter		= pDB->m_cLevelRoadCenterToSlabEdge;
	m_nQtyHDanNode		= pDB->GetQtyHDanNode();

	UpdateData(FALSE);

//	GetDlgItem(ID_BUTTON_SLABHUNCH)->ShowWindow(!pDB->IsTUGir());

	SetGridTitle();
	m_GridT.SetTextMatrix(1, 1, "%.0lf", pDB->m_dThickPave);		//	m_GridT.SetMemo(1, 1, "포장 두께");
	m_GridT.SetTextMatrix(1, 2, "%.0lf", pDB->m_dThickSlabBase);	//	m_GridT.SetMemo(1, 2, "기준 두께");
	m_GridT.SetTextMatrix(1, 3, "%.0lf", pDB->m_dThickSlabHunch);	//	m_GridT.SetMemo(1, 3, "헌치 높이");
	m_GridT.SetTextMatrix(1, 4, "%.0lf", pDB->m_dSlopHunch);		//	m_GridT.SetMemo(1, 3, "헌치 구배");
	m_GridT.SetTextMatrix(1, 5, "%.0lf", pDB->m_dThickSlabLeft);	//	m_GridT.SetMemo(1, 4, "좌측 두께");
	m_GridT.SetTextMatrix(1, 6, "%.0lf", pDB->m_dThickSlabRight);	//	m_GridT.SetMemo(1, 5, "우측 두께");
	m_GridT.SetTextMatrix(1, 7, "%.0lf", pDB->m_dWidthHunch);		//	m_GridT.SetMemo(1, 6, "돌출 길이");
	m_GridT.SetTextMatrix(1, 8, "%.0lf", pDB->m_dHunchDisForBaseThick);
	m_GridT.SetMemo(1, 8, "기준두께위치(상판 최대폭위치의 플랜지 끝단에서 거더 중앙으로[-], 거더 외측방향[+])");
	m_GridT.SetRedraw(TRUE,TRUE);

	CString str(_T(""));

	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

	for(long nNode = 0; nNode < m_nQtyHDanNode; nNode++)
	{
		str.Format("%g",pDB->GetLengthHDanDom(pBx, nNode));

		m_Grid.SetTextMatrix(1, nNode + 1, str);

		if(pDB->GetTypeLevelHDan(nNode) == 0)				
			m_Grid.SetTextMatrix(2, nNode + 1, "없  음");
		else													
			m_Grid.SetTextMatrix(2, nNode + 1, "Level" );

		if(pDB->GetWallGuardTypeHDan(nNode) == 0)			
			m_Grid.SetTextMatrix(3, nNode + 1, "없  음");
		else if(pDB->GetWallGuardTypeHDan(nNode) == 1)		
			m_Grid.SetTextMatrix(3, nNode + 1, "방호벽");
		else if(pDB->GetWallGuardTypeHDan(nNode) == 2)		
			m_Grid.SetTextMatrix(3, nNode + 1, "중분대");
		else													
			m_Grid.SetTextMatrix(3, nNode + 1, "보  도");

		if(pDB->GetTypeVariableLengthHDan(nNode) == 0)		
		{
			m_Grid.SetItemState(1, nNode + 1, m_Grid.GetItemState(1,nNode + 1) & ~GVIS_READONLY);
			m_Grid.SetTextMatrix(4, nNode + 1, "없  음");
		}
		else		
		{
			m_Grid.SetItemState(1, nNode + 1, m_Grid.GetItemState(1,nNode + 1) | GVIS_READONLY);
			m_Grid.SetTextMatrix(4, nNode + 1, "있  음");
		}
	}
	
	pDB->SetTendonArcType(pBx);
	
	m_Grid.SetRedraw(TRUE,TRUE);
	
	UpdateData(FALSE);
	
}

void CSlabJewonDlg::SetDataSave()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	UpdateData(TRUE);
	///< 데이터 저장
	pDB->m_cLevelRoadCenterToSlabEdge	= (BYTE)m_bLevelCenter;
	pDB->SetQtyHDanNode(m_nQtyHDanNode);

	pDB->m_dThickPave			= m_GridT.GetTextMatrixDouble(1, 1);
	pDB->m_dThickSlabBase		= m_GridT.GetTextMatrixDouble(1, 2);
	pDB->m_dThickSlabHunch		= m_GridT.GetTextMatrixDouble(1, 3);
	pDB->m_dSlopHunch			= m_GridT.GetTextMatrixDouble(1, 4);	
	pDB->m_dThickSlabLeft		= m_GridT.GetTextMatrixDouble(1, 5);
	pDB->m_dThickSlabRight		= m_GridT.GetTextMatrixDouble(1, 6);
	pDB->m_dWidthHunch			= m_GridT.GetTextMatrixDouble(1, 7);
	pDB->m_dHunchDisForBaseThick= m_GridT.GetTextMatrixDouble(1, 8);

	for(long nNode = 0; nNode < m_nQtyHDanNode+1; nNode++)
	{
		pDB->SetLengthHDanByLine(m_Grid.GetTextMatrixDouble(1,nNode+1), nNode);

		for(long nValueType = 0; nValueType < 3; nValueType++)
			pDB->SetValueTypeHDan(nNode, nValueType, GetStringValue(m_Grid.GetItemText(nValueType + 2, nNode + 1)));
	}

	for(long nLine = m_nQtyHDanNode + 1; nLine < HDANLINESU; nLine++)
	{
		pDB->SetLengthHDanByLine(0, nLine);

		for(long nValueType = 0; nValueType < 3; nValueType++)
			pDB->SetValueTypeHDan(nLine, nValueType, 0);
	}

//	if(pData->m_bPassedCalcFloorDlg)
//	{
//		for(long n=0; n<pData->m_HLsu; n++)
//			pFloor->SetFloorStatus(n);
//
//		pData->m_bPassedCalcFloorDlg = FALSE;
//	}

	// 방호벽 생성
	SetDataSaveGuard();

	m_pStd->m_pDoc->SetModifiedFlag(TRUE);		
}

void CSlabJewonDlg::SetDataSaveGuard()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();

	long nSu = 0;
	CStringArray strArr;

	for(int n = 0;n < m_nQtyHDanNode;n++)
	{
		CPlateBasicIndex	*pBx	= pDB->GetGirder(0)->GetBxOnJijum(0);

		long	nGuardType	= pDB->GetWallGuardTypeHDan(n);
		double	dLenGuard	= 0;
		double	dSlabWidth	= pDB->GetWidthSlabDom(pBx);		

		if(pDB->IsTypeGuardFenceDom(n)) 	// 방호벽 생성
		{
			dLenGuard	= 0;

			if(nGuardType == HT_GUARDFENCE)
			{
				for(long i=0; i<n; i++)
					dLenGuard += pDB->GetLengthHDanDom(pBx,i);
				dLenGuard += pDB->GetLengthHDanDom(pBx,n)/2;

				if(dLenGuard < dSlabWidth/2)
					strArr.Add(_T("방호벽(좌)"));
				else
					strArr.Add(_T("방호벽(우)"));
			}
			else if(nGuardType == HT_CENTERGUARDFENCE)
			{
				for(long i=0; i<n; i++)
					dLenGuard += pDB->GetLengthHDanDom(pBx,i);
				dLenGuard += pDB->GetLengthHDanDom(pBx,n)/2;

				if(dLenGuard < dSlabWidth/2)
					strArr.Add(_T("중분대(좌)"));
				else
					strArr.Add(_T("중분대(우)"));
			}
			nSu++;
		}
	}
	pDB->MakeGuardFence(nSu,strArr);
}

void CSlabJewonDlg::OnButtonNext()
{
	long nSu = 0;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	for(long n = 0; n < pDB->GetQtyHDanNode(); n++)
	{
		if(pDB->GetWallGuardTypeHDan(n) == 1)	// 방호벽 생성
			nSu++;
	}

	CGeneralBaseDlg::OnButtonNext();
}
///슬래브 제원을 저장하기 전에 슬래브 제원이 변경되었는지를 확인
BOOL CSlabJewonDlg::IsChangeSlabDataBeforeSave()
{
/*
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor      *pFloor  = m_pStd->m_pCalcStd->m_pCalcFloor;
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(pDB->m_dThickSlabLeft != m_dSlabT1)				return TRUE;
	if(pDB->m_dThickSlabBase != m_dBaseThick)			return TRUE;
	if(pDB->m_dThickSlabRight != m_dSlabT2)				return TRUE;
	if(pDB->m_dWidthHunch != m_dSlabW1)					return TRUE;
	if(pDB->m_dHunchDisForBaseThick != m_dBaseThickPos)	return TRUE;
	if(pDB->m_dThickPave != m_dPaveThick)				return TRUE;
	if(pDB->m_dThickSlabHunch != m_dHunchThick)			return TRUE;

	long nQtyH = pDB->GetQtyHDanNode();

	for(long nNode = 0; nNode < nQtyH+1; nNode++)
	{
		if(pDB->GetLengthHDanByLine(nNode) != m_Grid.GetTextMatrixDouble(1, nNode+1))
			return TRUE;

		for(long nValueType = 0; nValueType < 3; nValueType++)
		{
			if(pDB->GetValueTypeHDan(nNode, nValueType) != GetStringValue(m_Grid.GetItemText(nValueType + 2, nNode + 1)))
				return TRUE;
		}
	}
*/
	return FALSE;
}

void CSlabJewonDlg::SetGridTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(5);
	m_Grid.SetColumnCount(m_nQtyHDanNode+1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(0, 78);

	m_Grid.SetTextMatrix(0, 0, "구 분"   );
	m_Grid.SetTextMatrix(1, 0, "길이(mm)");
	m_Grid.SetTextMatrix(2, 0, "Level"   );
	m_Grid.SetTextMatrix(3, 0, "종류"    );
	m_Grid.SetTextMatrix(4, 0, "가변구간");

	long nColCount = m_Grid.GetColumnCount();

	CString str;

	for(long nCol = 1; nCol < nColCount; nCol++)
	{
		str.Format("L%d", nCol);
		m_Grid.SetTextMatrix(0, nCol, str);

		m_Grid.SetCellType(2, nCol, CT_COMBO);
		m_Grid.SetCellType(3, nCol, CT_COMBO);
		m_Grid.SetCellType(4, nCol, CT_COMBO);
	}
	m_Grid.SetRedraw(TRUE, TRUE);

	m_GridT.SetEditable(TRUE);
	m_GridT.SetListMode(FALSE);
	m_GridT.EnableDragAndDrop(TRUE);
	m_GridT.EnableInsertRow(FALSE);
	m_GridT.EnableInsertCol(FALSE);
	m_GridT.EnableReturnPass(TRUE);

	m_GridT.SetRowCount(2);
	m_GridT.SetColumnCount(9);
	m_GridT.SetFixedRowCount(1);
	m_GridT.SetFixedColumnCount(1);

	m_GridT.InputShGridTitle(m_gt, 9, TRUE, TRUE);
	m_GridT.SetRowHeightAll(20);
	m_GridT.SetRowHeight(0, 30);
	m_GridT.SetTextMatrix(1, 0, "설정값");

	if(pDB->IsTUGir())
	{
		m_GridT.SetItemState(1, 2, GVIS_READONLY);
//		m_GridT.SetItemState(1, 6, GVIS_READONLY);
		m_GridT.SetItemState(1, 7, GVIS_READONLY);
	}

	m_GridT.SetRedraw(TRUE,TRUE);
}

long CSlabJewonDlg::GetStringValue(const CString &str) const
{
	if(str == "없  음") return 0;
	if(str == "있  음") return 1;
	if(str == "Level" ) return 1;
	if(str == "방호벽") return 1;
	if(str == "중분대") return 2;
	if(str == "보  도") return 3;

	return 0;
}

BOOL CSlabJewonDlg::PreTranslateMessage(MSG* pMsg)
{	
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();

		switch(next.row)
		{
		case 2:
			m_Grid.SetComboString(m_strLevel);
			break;
		case 3:
			m_Grid.SetComboString(m_strType);
			break;
		case 4:
			m_Grid.SetComboString(m_strSwitch);
			break;
		}
	}
	
	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////

void CSlabJewonDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;	

	if(nRow==4)	// 가변구간 검사
	{
		BOOL bFind = FALSE;
		for(long col=1;col<m_Grid.GetColumnCount();col++)
		{
			if(m_Grid.GetTextMatrix(nRow,col)=="있  음") 
			{
				bFind = TRUE;
				break;
			}
		}
		if(!bFind) m_Grid.SetTextMatrix(nRow,nCol,"있  음");
	}

	SetDataSave();
	SetDataInit();
	SetDataSave(); // 도면 가변구간 때문에 한번더 저장	
	DrawInputDomyunView(TRUE);
}

void CSlabJewonDlg::OnCellChangedDataGridT(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;
	if(nCol==8)
	{
		CString	szText;
		szText	= m_GridT.GetTextMatrix(1, 8);
		if(szText==_T("강선적용"))
		{
			pDB->m_bTendonInputComplete = TRUE;
		}
		else
		{
			pDB->m_bTendonInputComplete = FALSE;
			pDB->m_dHeightLeftTension  = 0;
			pDB->m_dHeightRightTension = 0;
			pDB->m_dLengthGinjangdan   = 0;
			pDB->m_dLengthJungchakdan  = 0;
			pDB->m_nSlabTensionSize    = 0;
			pDB->m_nTendonFix          = 0;
			pDB->m_nTendonDomFix          = 0;
			for(long n=0; n<pDB->m_nSlabTensionSize; n++)
			{
				pDB->m_SlabTension[n].m_dL		= 0;
				pDB->m_SlabTension[n].m_dH		= 0;
				pDB->m_SlabTension[n].m_dR		= 0;
				pDB->m_SlabTension[n].m_nType	= 0;
			}
		}
	}
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CSlabJewonDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;

	m_nQtyHDanNode++;
	SetGridCellInsertedCol(nCol);
	SetDataSave();
	SetDataInit();
	SetGridTitle();

	DrawInputDomyunView(TRUE);
}

void CSlabJewonDlg::SetGridCellInsertedCol(int nCol)
{
	double dDis = atof(m_Grid.GetItemText(1, nCol+1))/2;
	m_Grid.SetTextMatrix(1, nCol, "%.0lf", dDis);
	m_Grid.SetTextMatrix(1, nCol+1,"%.0lf", dDis);
	m_Grid.SetTextMatrix(2, nCol, m_Grid.GetItemText(2, nCol+1));
	m_Grid.SetTextMatrix(3, nCol, m_Grid.GetItemText(3, nCol+1));
	m_Grid.SetTextMatrix(4, nCol, m_Grid.GetItemText(4, nCol+1));
}

void CSlabJewonDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	m_nQtyHDanNode--;
	SetDataSave();
	SetDataInit();
	SetGridTitle();

	DrawInputDomyunView(TRUE);
	m_Grid.SetFocus();
}

void CSlabJewonDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;

	double dDis = atof(m_Grid.GetItemText(1, nCol)) + atof(m_Grid.GetItemText(1, nCol+1));
	m_Grid.SetTextMatrix(1, nCol+1, "%.0lf", dDis);
}

void CSlabJewonDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}

void CSlabJewonDlg::OnLevelRoadcenter() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge(); 

	m_bLevelCenter = !m_bLevelCenter;
	pDB->m_cLevelRoadCenterToSlabEdge = (BYTE)m_bLevelCenter;

	DrawInputDomyunView(TRUE);
	UpdateData(FALSE);
}

void CSlabJewonDlg::OnButtonSlabhunch() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CSubSlabHunchInputDlg HunchInputDlg(m_pStd, pDB, GetParent());

	if(pDB->m_bUserSlabHunch)
		HunchInputDlg.DoModal();
	else
		pDB->m_dHunchArray.RemoveAll();

	SetDataSave();
	DrawInputDomyunView(FALSE);
}

BOOL CSlabJewonDlg::IsFocusItem(CString szTitle)
{
	if(szTitle.Find("L")!=-1)	return TRUE;

	if(GetDlgItem(IDC_GRIDN)) 
	{
		CGridCtrl *pGrid = (CGridCtrl *)GetDlgItem(IDC_GRIDN);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(0,nCol)==szTitle)
					return TRUE;
			}
		}
	}	
	return CGeneralBaseDlg::IsFocusItem(szTitle);
}

void CSlabJewonDlg::SetFocusItem(CString szTitle)
{
	if(szTitle=="A")	SetGridFocusCell(1, 1);
	if(szTitle=="T")	SetGridFocusCell(1, 2);
	if(szTitle=="H")	SetGridFocusCell(1, 3);
	if(szTitle=="T1")	SetGridFocusCell(1, 4);
	if(szTitle=="T2")	SetGridFocusCell(1, 5);
	if(szTitle=="W1")	SetGridFocusCell(1, 6);
	if(szTitle=="W")	SetGridFocusCell(1, 7);
}

void CSlabJewonDlg::SetGridFocusCell(long nRow, long nCol)
{
	m_GridT.ResetSelection();
	m_GridT.SetFocusCell(nRow,nCol);
	m_GridT.SendMessage(WM_KEYDOWN,VK_F2);
}

void CSlabJewonDlg::STMakeCursor(NMHDR* nmgv, LRESULT*)
{
	CDomyun *pDom = m_pView->GetDomyun();

	long	nCol	= m_GridT.GetFocusCell().col;
	CString	szTitle	= m_GridT.GetItemText(0,nCol);

	if(szTitle=="A")	szTitle="A";
	if(szTitle=="T")	szTitle="T";
	if(szTitle=="H")	szTitle="H";
	if(szTitle=="T1")	szTitle="T1";
	if(szTitle=="T2")	szTitle="T2";
	if(szTitle=="W1")	szTitle="W1";
	if(szTitle=="W")	szTitle="W";
	pDom->STMakeCursor(szTitle);
	return;
}

void CSlabJewonDlg::OnButtonTendon() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CSubInputTensionBarDlg TensinonInputDlg(m_pStd, pDB, GetParent());

	OnCheckTendon();
	if(m_Tendon.m_bOnOff)
		TensinonInputDlg.DoModal();
	SetDataInit();

	DrawInputDomyunView(FALSE);
}

void CSlabJewonDlg::OnCheckTendon() 
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	
	UpdateData(TRUE);

	BOOL bTendon = pDB->m_bTendonInputComplete;
	if(!bTendon && m_Tendon.m_bOnOff)		// 강선이 없다가 생겼을 경우
	{
		CSubInputTensionBarDlg TensinonInputDlg(m_pStd, pDB, GetParent());
		TensinonInputDlg.SetDataDefault();
	}
	else if(bTendon && !m_Tendon.m_bOnOff)	// 강선이 있다가 없앤경우
	{
		pDB->m_dHeightLeftTension  = 0;
		pDB->m_dHeightRightTension = 0;
		pDB->m_dLengthGinjangdan   = 0;
		pDB->m_dLengthJungchakdan  = 0;
		pDB->m_nSlabTensionSize    = 0;
		pDB->m_nTendonFix          = 0;
		for(long n=0; n<pDB->m_nSlabTensionSize; n++)
		{
			pDB->m_SlabTension[n].m_dL		= 0;
			pDB->m_SlabTension[n].m_dH		= 0;
			pDB->m_SlabTension[n].m_dR		= 0;
			//pDB->m_SlabTension[n].m_nType	= 0;
		}
	}
	pDB->m_bTendonInputComplete = m_Tendon.m_bOnOff;
}

void CSlabJewonDlg::OnPreInitDialog()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();

//	long	nShowWindow = SW_HIDE;
/*	if(!pDB->IsPlateGir())
	{
		nShowWindow	= SW_SHOW;
		m_Tendon.SetCheck(pDB->m_bTendonInputComplete); //프리스트레스 사용인지...여부...
	}
*/
	m_Tendon.SetCheck(pDB->m_bTendonInputComplete); //프리스트레스 사용인지...여부...

//	GetDlgItem(IDC_BUTTON_TENDON)->ShowWindow(nShowWindow);
	SetInitPositionCombo();

	SetResize(IDC_BUTTON_SLAB_STRUCTURE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_LEVEL_ROADCENTER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_TENDON, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_POS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_GRIDN, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
	SetResize(IDC_GRID, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
}

void CSlabJewonDlg::SetInitPositionCombo()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(0);	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_STT_SLAB|BX_END_SLAB);

	CString strText="";
	int nCount = 1;
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		if(pSec)
		{	
			if(pSec->GetenDetType()==SECDET_CROSSBEAM) // 가로보 타입
			{
				strText.Format("G1-G%d-C%d", pDB->GetQtyGirder(), nCount);
				m_ctlCombo.AddString(strText);
				nCount++;
			}
			else
			{
				strText.Format("G1-G%d-V%d", pDB->GetQtyGirder(), nCount);
				m_ctlCombo.AddString(strText);
				nCount++;
			}
		}
		else if(pBx->IsState(BX_STT_SLAB))
		{
			strText.Format("G1-G%d-%s", pDB->GetQtyGirder(), pDB->m_strJijumName[0]);
			m_ctlCombo.AddString(strText);				
		}
		else if(pBx->IsState(BX_END_SLAB))
		{
			strText.Format("G1-G%d-%s", pDB->GetQtyGirder(), pDB->m_strJijumName[pDB->m_nQtyJigan]);
			m_ctlCombo.AddString(strText);
		}
		pBx = Finder.GetBxNext();		
	}
	
	m_ctlCombo.SetCurSel(0);
}

CPlateBasicIndex* CSlabJewonDlg::GetPosBx()
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(0);	
	CPlateBxFinder Finder(pGir);
	Finder.GetBxFirst(BX_STT_SLAB|BX_CROSSBEAM_VBRACING|BX_END_SLAB);

	return Finder.GetBxNext(m_ctlCombo.GetCurSel());
}

void CSlabJewonDlg::OnSelchangeComboPos() 
{
	DrawInputDomyunView();
}

LRESULT CSlabJewonDlg::OnReceive(WPARAM wp,LPARAM lp)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	switch(wp)
	{
	case IDC_BUTTON_TENDON :
		OnButtonTendon();
		pDB->m_bTendonInputComplete = m_Tendon.m_bOnOff;		
		break;
	case ID_BUTTON_SLABHUNCH:
		pDB->m_bUserSlabHunch = m_SlabHunch.GetCheck();
		OnButtonSlabhunch();
		break;
	}
	return 0;
}

void CSlabJewonDlg::OnButtonSlabStructure() 
{
	CSlabStructureDlg	Dlg;

	Dlg.m_pDB			= m_pStd->GetBridge();
	Dlg.m_pDataManage	= m_pStd->m_pDataManage;

	if(!IsUsedFlag())
		Dlg.SetDataDefault();

	if(Dlg.DoModal()==IDOK)
	{
		DrawInputDomyunView(FALSE);
	}
}
