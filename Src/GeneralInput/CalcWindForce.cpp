// CalcWindForce.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcWindForce.h"
#include "GeneralInput.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcWindForce dialog


CCalcWindForce::CCalcWindForce(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcWindForce::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcWindForce)
	m_strStation   = _T("");
	m_sWindFileName = _T("");
	//}}AFX_DATA_INIT
	m_nCase = 0;
}


void CCalcWindForce::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcWindForce)
	DDX_Control(pDX, IDC_COMBO_JIJUM, m_comboJijumPos);
	DDX_Text(pDX, IDC_STATIC_STA, m_strStation);
	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_sWindFileName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB_WIND, m_TabWind);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_BUTTON_SAPEXPORT, m_btnSapInput);
}


BEGIN_MESSAGE_MAP(CCalcWindForce, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcWindForce)
	ON_BN_CLICKED(IDC_BUTTON_SAPEXPORT, OnButtonMakeSapS2k)
	ON_CBN_SELCHANGE(IDC_COMBO_JIJUM, OnSelchangeComboJijum)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_WIND, OnSelchangeTabWind)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcWindForce message handlers

void CCalcWindForce::OnPreInitDialog()
{
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();
	CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	if(pData->m_bRunCrossWind == FALSE)
	{
		HLLCalc.RunWindAnalysis();	
		pData->m_bRunCrossWind = TRUE;
	}

	SetComboData();
	SetFileName();

	SetResize(IDC_TAB_WIND, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_EDIT_SAVE_PATH, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAPEXPORT, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	m_TabWind.InsertItem(0, "하중");
	m_TabWind.InsertItem(1, "결과");
	m_TabWind.SetCurSel(0);

	UpdateData(FALSE);
}

void CCalcWindForce::SetDataInit()
{
	UpdateData(TRUE);

	SetGridTitle();

	if(m_TabWind.GetCurSel()==PAGE_LOAD)
	{
		SetDataInitWindLoad();
	}
	else
	{
		SetDataInitResult();
	}

	UpdateData(FALSE);
}

void CCalcWindForce::SetDataInitWindLoad()
{
	CAPlateCalcStd   *pCalcStd = m_pStd->m_pCalcStd;
	CCalcData        *pData    = m_pStd->GetDataManage()->GetCalcData();
	CPlateBasicIndex *pBx      = m_pStd->GetBridge()->GetGirder(-1)->GetBxOnJijum(m_comboJijumPos.GetCurSel()<0?0:m_comboJijumPos.GetCurSel());	 
	CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	long           nJijum    = m_comboJijumPos.GetCurSel();
	if(nJijum<0)   nJijum    = 0;
	CFEMManage     *pFEM  = pCalcStd->GetWindFemData(nJijum);
	
	if(pCalcStd->GetSizeWindFemData() == 0 ) return;
		
	m_strStation.Format("%s", GetStationForStringOut(frM(pBx->GetStation()),3,TRUE,TRUE));
	
	BOOL bBangEmL = pData->DESIGN_FLOOR_DATA[0].m_bBangEm;
	BOOL bBangEmR = pData->DESIGN_FLOOR_DATA[1].m_bBangEm;
	
	CString szNum, szTemp, szForce[2], szMoment;
	
	////////////////////////////////////////////////////////////////////////
	// 재하하중
	long ns = pFEM->m_szLCName.GetSize();
	for(long n=0; n<ns; n++)
	{
		CString szLCTitle = pFEM->m_szLCName[n];
		CString szSpecify = _T("");
	
		CJoint *pJoint = NULL;

		long nRow = n+2;

		if(bBangEmL && bBangEmR)
		{
			if(n==0)
			{
				szSpecify= "좌측방음벽(좌->우)";
				m_Grid.SetTextMatrix(nRow, 0, szSpecify);
				m_Grid.SetTextMatrix(nRow, 1, szLCTitle);
				m_Grid.SetTextMatrix(nRow, 2, "1");
				pJoint = pFEM->GetJoint(0);
			}
			else
			{
				szSpecify= "우측방호벽(우->좌)";
				szNum.Format("%d", pFEM->GetElementSize()+1);
				m_Grid.SetTextMatrix(nRow, 0, szSpecify);
				m_Grid.SetTextMatrix(nRow, 1, szLCTitle);			
				m_Grid.SetTextMatrix(nRow, 2, szNum);
				long nJ = HLLCalc.GetLoadApplyMemberCount(pFEM);
				pJoint = pFEM->GetJoint((unsigned short)nJ);
			}
		}
		else if(bBangEmL && !bBangEmR)
		{
			szSpecify= (n==0)? "좌측방음벽(좌->우)" : "좌측방음벽(우->좌)";
			m_Grid.SetTextMatrix(nRow, 0, szSpecify);
			m_Grid.SetTextMatrix(nRow, 1, szLCTitle);
			m_Grid.SetTextMatrix(nRow, 2, "1");
			pJoint = pFEM->GetJoint(0);
		}
		else if(!bBangEmL && bBangEmR)
		{
			szSpecify= (n==0)? "우측방호벽(우->좌)" : "우측방호벽(좌->우)";
			szNum.Format("%d", pFEM->GetElementSize()+1);
			m_Grid.SetTextMatrix(nRow, 0, szSpecify);
			m_Grid.SetTextMatrix(nRow, 1, szLCTitle);		
			m_Grid.SetTextMatrix(nRow, 2, szNum);
			long nJ = HLLCalc.GetLoadApplyMemberCount(pFEM);
			pJoint = pFEM->GetJoint((unsigned short)nJ);
		}

		if(pJoint)
		{
			CTypedPtrArray <CObArray, CLoadForceJoint*> m_LoadForceArray; 
			
			CLoadForceJoint *Force = pJoint->GetLoadForce(n);
			
			CVector vForce = Force->m_vForce;
			CVector vMoment= Force->m_vMoment;
			
			szForce[0].Format("%.3f", vForce.z);
			szForce[1].Format("%.3f", vForce.x);
			szMoment.Format("%.3f", vMoment.y);
			
			m_Grid.SetTextMatrix(nRow, 3, szForce[0]);
			m_Grid.SetTextMatrix(nRow, 4, szForce[1]);
			m_Grid.SetTextMatrix(nRow, 5, szMoment);

			m_Grid.SetTextMatrix(nRow, 6, "-");	
			m_Grid.SetTextMatrix(nRow, 7, "-");	
			m_Grid.SetTextMatrix(nRow, 8, "-");	
			m_Grid.SetTextMatrix(nRow, 9, "-");	
			
			m_Grid.SetCellType(nRow,	10,	CT_CHECKBOX);
			m_Grid.SetTextMatrix(nRow, 10, "적용");			
			m_Grid.SetCheck(nRow, 10, Force->m_bApply);
			
			m_Grid.SetItemState(nRow, 2, GVIS_READONLY);	m_Grid.SetItemBkColour(nRow, 2, RGBREADONLY);
			m_Grid.SetItemState(nRow, 6, GVIS_READONLY);	m_Grid.SetItemBkColour(nRow, 6, RGBREADONLY);
			m_Grid.SetItemState(nRow, 7, GVIS_READONLY);	m_Grid.SetItemBkColour(nRow, 7, RGBREADONLY);
			m_Grid.SetItemState(nRow, 8, GVIS_READONLY);	m_Grid.SetItemBkColour(nRow, 8, RGBREADONLY);
			m_Grid.SetItemState(nRow, 9, GVIS_READONLY);	m_Grid.SetItemBkColour(nRow, 9, RGBREADONLY);
		}
	}

	UpdateData(FALSE);
}

void CCalcWindForce::SetDataInitResult()
{
	CAPlateCalcStd		*pCalcStd = m_pStd->m_pCalcStd;
	CCalcData			*pData    = m_pStd->GetDataManage()->GetCalcData();
	long				nJijum    = m_comboJijumPos.GetCurSel();
	
	if(nJijum<0)   nJijum    = 0;

	CFEMManage     *pFEM  = pCalcStd->GetWindFemData(nJijum);
	
	if(pCalcStd->GetSizeWindFemData() == 0 ) return;
	
	CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), pCalcStd);
	unsigned short nJointL	= (unsigned short)0;
	unsigned short nJointR	= (unsigned short)HLLCalc.GetLoadApplyMemberCount(pFEM);

	BOOL	bBangEmL	= pData->DESIGN_FLOOR_DATA[0].m_bBangEm;
	BOOL	bBangEmR	= pData->DESIGN_FLOOR_DATA[1].m_bBangEm;
	CJoint	*pJoint		= NULL;

	CString szNum, szTemp;

	for(long nC=0; nC<pFEM->m_szLCName.GetSize(); nC++)
	{
		CString szLCTitle	=	pFEM->m_szLCName[nC];
		long	nRow		=	nC+1;

		if(bBangEmL && bBangEmR)
		{
			if(nC == 0)	pJoint = pFEM->GetJoint(nJointL);
			else		pJoint = pFEM->GetJoint(nJointR);
		}
		
		else if(bBangEmL && !bBangEmR)	pJoint = pFEM->GetJoint(nJointL);
		else if(!bBangEmL && bBangEmR)	pJoint = pFEM->GetJoint(nJointR);
		
		CLoadForceJoint *Force = pJoint->GetLoadForce(nC);
		
		if(!Force->m_bApply) m_Grid.SetRowHeight(nRow,	0);
		
		m_Grid.SetTextMatrix(nRow, 0, szLCTitle);

		long nCnt = 0;

		for (long i = 0; i < pFEM->GetJointSize(); i++)
		{
			long	nCol	= nCnt*3;
			double	dRes[3] = { 0, 0, 0 };
			CString szRes[3];

			CJoint			*pJoint		= pFEM->GetJoint((unsigned short)i);
			CResultForce	*pResult	= pJoint->GetResultForce(nC);

			dRes[0] = Round(pResult->m_Force.z, 3);
			dRes[1] = Round(pResult->m_Moment.y, 3);
			dRes[2] = Round(pResult->m_Force.x, 3);

			if (dRes[0]==dRes[1] && dRes[1]==dRes[2] && dRes[2]==0) continue;
			nCnt++;

			szRes[0].Format("%.3f", dRes[0]);
			szRes[1].Format("%.3f", dRes[1]);
			szRes[2].Format("%.3f", dRes[2]);

			m_Grid.SetTextMatrix(nRow, nCol+1, szRes[0]);
			m_Grid.SetTextMatrix(nRow, nCol+2, szRes[1]);
			m_Grid.SetTextMatrix(nRow, nCol+3, szRes[2]);

			m_Grid.SetItemState(nRow, nCol+1, GVIS_READONLY);	
			m_Grid.SetItemState(nRow, nCol+2, GVIS_READONLY);	
			m_Grid.SetItemState(nRow, nCol+3, GVIS_READONLY);	
		}
	}

	UpdateData(FALSE);
}

// 풍하중 재계산
void CCalcWindForce::SetDataDefault()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData(); 
	pData->m_bRunCrossWind = FALSE;

	CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	HLLCalc.RunWindAnalysis();
}

void CCalcWindForce::SetDataSave()
{
	if (m_TabWind.GetCurSel()==PAGE_RESULT) return;
	UpdateData(TRUE);

	long           nJijum    = m_comboJijumPos.GetCurSel();
	if(nJijum<0)   nJijum    = 0;
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CCalcData      *pData    = m_pStd->GetDataManage()->GetCalcData();
	CFEMManage     *pFEM  = pCalcStd->GetWindFemData(nJijum);
	CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), pCalcStd);

	BOOL bBangEmL = pData->DESIGN_FLOOR_DATA[0].m_bBangEm;
	BOOL bBangEmR = pData->DESIGN_FLOOR_DATA[1].m_bBangEm;

	unsigned short nJointL	= (unsigned short)0;
	unsigned short nJointR	= (unsigned short)HLLCalc.GetLoadApplyMemberCount(pFEM);

	for (long n=0; n<pFEM->m_szLCName.GetSize(); n++)
	{
		long nRow = n+2;
		CJoint *pJoint = NULL;
		CString szLoad	= m_Grid.GetTextMatrix(nRow, 1);
		double  dLoadUz = m_Grid.GetTextMatrixDouble(nRow, 3);
		double  dLoadUx = m_Grid.GetTextMatrixDouble(nRow, 4);
		double  dLoadRy = m_Grid.GetTextMatrixDouble(nRow, 5);
		BOOL	bApply	= m_Grid.IsCheck(nRow,	10);
	
		pFEM->m_szLCName[n] = szLoad;
		
		if (bBangEmL && bBangEmR)
		{
			pJoint = (n==0)? pFEM->GetJoint(nJointL) : pFEM->GetJoint(nJointR);
		}
		else if (bBangEmL && !bBangEmR)
		{
			pJoint = pFEM->GetJoint(nJointL);
		}
		else if (!bBangEmL && bBangEmR)
		{
			pJoint = pFEM->GetJoint(nJointR);
		}

		if (pJoint)
		{
			CLoadForceJoint *pForce = pJoint->GetLoadForce(n);
			pForce->m_vForce.z  = dLoadUz;
			pForce->m_vForce.x  = dLoadUx;
			pForce->m_vMoment.y = dLoadRy;
			pForce->m_bApply	= bApply;
		}
	}
}

void CCalcWindForce::SetComboData()
{
	CPlateBridgeApp *pDB       = (CPlateBridgeApp*)m_pStd->GetBridge();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	
	m_comboJijumPos.ResetContent ();
	
	if(pDB->IsByunDanInBridge())
	{
		for (long n=0; n<pDB->m_nQtyJigan+1; n++)
		{
			CString cs = pDB->m_strJijumName[n];
			m_comboJijumPos.AddString(cs);
		}
	}
	else
		m_comboJijumPos.AddString(pDB->m_strJijumName[0]);
	
	m_comboJijumPos.SetCurSel(0);

	UpdateData(FALSE);
}

void CCalcWindForce::SetGridTitle()
{
	UpdateData(TRUE);
	if(m_TabWind.GetCurSel()==PAGE_LOAD)		SetGridTitleWindLoad();
	else if(m_TabWind.GetCurSel()==PAGE_RESULT)	SetGridTitleResult();
}

void CCalcWindForce::SetGridTitleWindLoad()
{
	long	nRows	= 4;
	long	nCols	= 11;

	UpdateData(TRUE);

	m_Grid.DeleteAllItems();
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.DeleteUnusedColumn();
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetRowCount(nRows);

	m_Grid.SetEditable(TRUE);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 100);
	
	m_Grid.SetTextMatrixEx(0, 0, "구분", 1, 0);
	m_Grid.SetTextMatrixEx(0, 1, "하중종류", 1, 0);
	m_Grid.SetTextMatrixEx(0, 2, "위치①", 1, 0);
	m_Grid.SetTextMatrixEx(0, 3, "하중크기①", 0, 2);
	m_Grid.SetTextMatrixEx(0, 6, "위치②", 1, 0);
	m_Grid.SetTextMatrixEx(0, 7, "하중크기②", 0, 2);
	m_Grid.SetTextMatrixEx(0, 10, "적용", 1,0);

	m_Grid.SetTextMatrixEx(1, 3, "Uz");
	m_Grid.SetTextMatrixEx(1, 4, "Ux");
	m_Grid.SetTextMatrixEx(1, 5, "Ry");
	m_Grid.SetTextMatrixEx(1, 7, "Uz");
	m_Grid.SetTextMatrixEx(1, 8, "Ux");
	m_Grid.SetTextMatrixEx(1, 9, "Ry");

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CCalcWindForce::SetGridTitleResult()
{
	CPlateBridgeApp *pDB      = (CPlateBridgeApp*)m_pStd->GetBridge();
	
	long	nRows	= 3;
	long	nCols	= pDB->GetGirdersu()*3+1;
	
	UpdateData(TRUE);
	
	m_Grid.DeleteAllItems();
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.DeleteUnusedColumn();
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetRowCount(nRows);
	
	
	m_Grid.SetEditable(TRUE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(0, 100);

	m_Grid.SetTextMatrix(0,0,"Load Case");

	CString str ="";
	for(long n=1, nG=1; n<=(pDB->GetGirdersu()-1)*3+1; n+=3, nG++)
	{
		str.Format("P%d", nG);
		m_Grid.SetTextMatrix(0, n, str);
		str.Format("M%d", nG);
		m_Grid.SetTextMatrix(0, n+1, str);
		str.Format("H%d", nG);
		m_Grid.SetTextMatrix(0, n+2, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CCalcWindForce::DrawInputDomyunView(BOOL bZoomAll)
{
	CFEMManage	*pFEM	= m_pStd->m_pCalcStd->GetWindFemData(m_comboJijumPos.GetCurSel());
	CDomyun		*pDom	= m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);
	Dom.SetScaleDim(0.1);
	Dom.SetTextHeight(2.5);

	long	nLoadCase = 0;
    CCellID Cell = m_Grid.GetFocusCell();

	nLoadCase = m_nCase;

	CForceDiagram ForceDom(pFEM);

	ForceDom.SetScaleDim(0.1);
	ForceDom.SetTextHeight(1.5);
	ForceDom.DrawFrameModel(TRUE, FALSE, TRUE);
	ForceDom.DrawLoadCase(nLoadCase, FALSE, TRUE, 0.02, FALSE, 1);
	ForceDom.Move(0,Dom.GetExtRect().Height());
	Dom << ForceDom;

	Dom.SetCalcExtRect();
	CDRect Bound = Dom.GetExtRect();
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_HIDD);
	Dom.Rectangle(Bound.left-Dom.Always(8), 
	Bound.top-Dom.Always(3),
	Bound.right+Dom.Always(8), 
	Bound.bottom+Dom.Always(7));

	DrawDimJoint(&Dom,pFEM,1.3);
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}
void CCalcWindForce::DrawDimJoint(CDomyun *pDom, CFEMManage *pFEM, double dTextHeight)
{
	CDomyun			Dom(pDom);
	CARoadOptionStd	*pOptStd= m_pStd->GetDataManage()->GetOptionStd();
	
	Dom.SetDirection("BOTTOM");
	
	double	dSum		= 0;
	
	for(long nElem = 0; nElem < pFEM->GetElementSize(); nElem++)
	{
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		CJoint		*pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		CJoint		*pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);
		
		double dX	  =	pJ_J->m_vPoint.x-pJ_I->m_vPoint.x ;
		//double dXHalf = -dX/2;
			
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		
		Dom.SetTextHeight(dTextHeight);

		Dom.DimMoveTo(dSum, pJ_I->m_vPoint.y-Dom.Always(8),0);

		dSum += dX;

		Dom.DimLineTo(dX,0,COMMA(frM(dX)));
		
		if(nElem==pFEM->GetElementSize() - 1) //맨 마지막 턴을 출력하는 곳
		{
			Dom.DimMoveTo(dSum, pJ_I->m_vPoint.y-Dom.Always(8), 0);
			Dom.DimMoveTo(0,	pJ_I->m_vPoint.y-Dom.Always(8), 1);
			Dom.DimLineTo(dSum, 1, COMMA(frM(dSum)));
		}
		
	/*	if(bElemNum)
		{
			Dom.SetTextHeight(dTextHeight*2/3);
			Dom.TextCircle(dSum+dXHalf,-(pJ_I->m_vPoint.y+Dom.Always(5)),COMMA(nElem+1), 10);
		}		
	*/

	}

	*pDom << Dom;
	
}
void CCalcWindForce::OnCellChangedData(NMHDR*nmgv,LRESULT*)
{
	long           nJijum    = m_comboJijumPos.GetCurSel();
	if(nJijum<0)   nJijum    = 0;
	CFEMManage     *pFEM  = m_pStd->m_pCalcStd->GetWindFemData(nJijum);

	SetDataSave();
	SetDataInit();

	CFEMAnalysis Analysis(NULL, pFEM);
	Analysis.SetWriteM3t(m_pStd->m_pCalcStd->m_bMasterLock);	
	if(!Analysis.RunFemAnalysisOnASolver())
	{
		AfxMessageBox("계산 수행 중 오류가 있습니다 !");
		return;
	}
}

void CCalcWindForce::OnCellChangedRow(NMHDR*nmgv,LRESULT*)
{
	UpdateData(TRUE);

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	long	nRow		= GridView->iRow;

	if(m_TabWind.GetCurSel()==PAGE_RESULT)
	{
		m_nCase = nRow-1;
		DrawInputDomyunView(FALSE);
		return;
	}


	m_nCase = nRow-2;

	DrawInputDomyunView(FALSE);
}

void CCalcWindForce::OnButtonMakeSapS2k()
{
	UpdateData(TRUE);

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString strf    = _T("");

	int nSize = m_pStd->m_pCalcStd->GetBridge()->m_nQtyJigan;
	if(m_pStd->m_pCalcStd->GetBridge()->IsByunDanInBridge()==FALSE)
		nSize = 1;

	for(int i=0; i<nSize; i++)
	{
		strf = m_sWindFileName;
		PathToPathNum(strf,_T("s2k"), i+1);
	
		m_pStd->m_pCalcStd->GetWindFemData(i)->SaveAsSap2000(strf);		
		ShellExecute(NULL,NULL,szPath+"\\AEdit.exe","\"" + strf + "\"",NULL,SW_SHOWNORMAL);
		if(i==0) 
		{
			// AEdit 실행되었는지 검사 - 실행이 안되었을 경우 최장 2초간 검사
			CWnd *pWnd;
			for(long n=0; n<4; n++)
			{			
				if(GetMyselfWindow(&pWnd)) break;
				Sleep(500);
			}
		}
	}	
}

void CCalcWindForce::PathToPathNum(CString & path,CString csExt, long Num) const
{
	int nFine1 = path.ReverseFind('.');
	int nFine2 = path.ReverseFind('\\');
	if( nFine1>=0 && nFine1>nFine2)   path = path.Left(nFine1);

	CString csNum;
	csNum.Format("%02ld",Num);
	path += csNum;
	if(csExt.GetLength() > 0) 
	{
		path += ".";
		path += csExt;
	}
}

void CCalcWindForce::OnSelchangeComboJijum() 
{
	UpdateData(TRUE);

	SetDataInit();

	long nJijum = m_comboJijumPos.GetCurSel();
	CPlateBasicIndex *pBx = m_pStd->GetBridge()->GetGirder(-1)->GetBxOnJijum(nJijum);	 

	m_strStation.Format("%s", GetStationForStringOut(frM(pBx->GetStation()),3,TRUE,TRUE));
	UpdateData(FALSE);

	DrawInputDomyunView(TRUE);
}


BOOL CCalcWindForce::IsValid()
{	
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	return pData->m_bWind?TRUE:FALSE;
}


void CCalcWindForce::SetFileName()
{
	CDocument* pDoc = m_pStd->m_pDoc;

	m_sWindFileName = pDoc->GetPathName();
	int nFine = m_sWindFileName.Find('.');
	if( nFine>=0)   m_sWindFileName = m_sWindFileName.Left(nFine);
	m_sWindFileName += "풍하중";

	UpdateData(FALSE);	
}

void CCalcWindForce::OnSelchangeTabWind(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetDataInit();

	if(m_TabWind.GetCurSel()==PAGE_LOAD)	m_Grid.SetFocusCell(2, 3);
	else									m_Grid.SetFocusCell(1, 1);

	DrawInputDomyunView(TRUE);
}