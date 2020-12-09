// CalcSecForceDivResult.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcSecForceDivResult.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcSecForceDivResult dialog


CCalcSecForceDivResult::CCalcSecForceDivResult(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcSecForceDivResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcSecForceDivResult)
	m_sHLLFileName = _T("");
	//}}AFX_DATA_INIT
	m_nCase = 0;	
}


void CCalcSecForceDivResult::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcSecForceDivResult)
	DDX_Control(pDX, IDC_COMBO_HDANTYPE, m_ctlHLCombo);
	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_sHLLFileName);
	DDX_Control(pDX, IDC_EDIT_SAVE_PATH, m_flatSavePath);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_BUTTON_SAPEXPORT, m_btnSapInput);
}


BEGIN_MESSAGE_MAP(CCalcSecForceDivResult, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcSecForceDivResult)
	ON_CBN_SELCHANGE(IDC_COMBO_HDANTYPE, OnSelchangeComboHdantype)
	ON_BN_CLICKED(IDC_BUTTON_SAPEXPORT, OnButtonMakeSapS2k)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCalcSecForceDivResult message handlers
void CCalcSecForceDivResult::SetGridTitle()
{
	CPlateBridgeApp *pDB      = (CPlateBridgeApp*)m_pStd->GetBridge();
	CAPlateCalcStd  *pCalcStd = m_pStd->m_pCalcStd;
	long            nSel      = m_ctlHLCombo.GetCurSel();

	CFEMManage *pManage  = pCalcStd->GetHLLFemData(nSel);
	long       nLoadCase = pManage->GetLoadCaseSize();
	long       nCol      = pDB->GetGirdersu()+2;

	CString str ="";

	m_Grid.DeleteAllItems();

	m_Grid.SetRowCount(nLoadCase+1);
	m_Grid.SetColumnCount(nCol);

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);

	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 200);

	m_Grid.SetTextMatrix(0,0,"Load Case");

	for(long n=1; n<=pDB->GetGirdersu(); n++)
	{
		str.Format("G%d", n);
		m_Grid.SetTextMatrix(0, n, str);
	}
	m_Grid.SetTextMatrix(0, nCol-1, "sf");
	m_Grid.SetRedraw(TRUE,TRUE);
}


////////////////////////////////////////////////////////////////////////////////////
///시.종점 절점은 자유단이어서 반력이 0임
////////////////////////////////////////////////////////////////////////////////////
void CCalcSecForceDivResult::SetDataInit()
{
	CDataManage     *pDataManage = m_pStd->GetDataManage();
	CAPlateCalcStd  *pCalcStd    = m_pStd->m_pCalcStd;
	long	         nSel		 = m_ctlHLCombo.GetCurSel();
	CFEMManage      *pManage     = pCalcStd->GetHLLFemData(nSel);
	CCalcData       *pData       = pDataManage->GetCalcData();
	CPlateBridgeApp *pDB	     = (CPlateBridgeApp*)m_pStd->GetBridge();
	long		    nLoadCase	 = pManage->GetLoadCaseSize();
	CFemHLLCalc HLLCalc(pDataManage, pCalcStd);

	SetGridTitle();	
	
	/////////////////////////////////////////////////////////////////////////////////////
	// LoadCase
	long n = 0;
	for(n=0; n<pManage->GetLoadCaseSize(); n++)
		m_Grid.SetTextMatrix(n+1, 0, pManage->m_szLCName.GetAt(n));

	CString strCell = "";
	double dReactionForce = 0;
	/////////////////////////////////////////////////////////////////////////////////////
	// 절점별 반력
	POSITION pos = pManage->m_JointList.GetHeadPosition();
	long nCol=1;
	while(pos)
	{
		CJoint *pJ = (CJoint *)pManage->m_JointList.GetNext(pos);
		if(pJ->m_nIdx==0 || pJ->m_nIdx==pManage->GetJointSize()-1) continue;
		if(pJ->GetResultSize()==0)		break; 

		for(long n=0; n<nLoadCase; n++)	// 상->하
		{
			dReactionForce = Round(pJ->GetResultForce(n)->m_Force.z, 4);
			strCell.Format("%.3lf", dReactionForce);	
			m_Grid.SetTextMatrix(n+1, nCol, strCell);
		}
		nCol++;
	}	
	/////////////////////////////////////////////////////////////////////////////////////
	// 감소율, 비교
	CString str = "";
	for(n=0; n<pManage->GetLoadCaseSize(); n++)
	{
		CString szLCName = pManage->m_szLCName[n];
		long nFind = szLCName.Find("DB");
		
		CCalcGeneral CalcGeneral(m_pStd->GetDataManage());
		if(nFind!=-1 && nFind==0)
		{
			CString szLane = szLCName.Mid(nFind+4);
		
			double	dFactor = CalcGeneral.GetScaceFactor(atoi(szLane));
			
			str.Format("%.2f",CalcGeneral.GetScaceFactor(atoi(szLane)));

			m_Grid.SetTextMatrix(n+1, pDB->GetGirdersu()+1, str);		// 콤보에 포함된 하중
		}
		
		nFind = szLCName.Find("DL");
		if(nFind!=-1 && nFind==0)
		{
			CString szLane = szLCName.Mid(nFind+4);
			
			double	dFactor = CalcGeneral.GetScaceFactor(atol(szLane));

			str.Format("%.2f",CalcGeneral.GetScaceFactor(atol(szLane)));
			
			m_Grid.SetTextMatrix(n+1, pDB->GetGirdersu()+1, str);		// 콤보에 포함된 하중
		}
	}	

	UpdateData(FALSE);
}



void CCalcSecForceDivResult::SetDataSave()
{

}



void CCalcSecForceDivResult::SetPreComboData()
{
	CDataManage     *pDataManage = m_pStd->GetDataManage();
	CPlateBridgeApp *pBridge     = pDataManage->GetBridge();		
	CCalcData       *pData       = pDataManage->GetCalcData();
	CAPlateCalcStd  *pCalcStd    = m_pStd->m_pCalcStd;

	if(pCalcStd->GetSizeHLLFemData()<=0)
		AfxMessageBox("하중분배계산이 수행되지 않았습니다.");
	
	CString szText="";
	m_ctlHLCombo.ResetContent();
	double dStation = 0;
	for(long n=0; n<pCalcStd->GetSizeHLLFemData(); n++)
	{		
		dStation = pData->m_dStationHLLPos[n];
		szText.Format("횡분배 %d - %s",n+1, GetStationForStringOut(frM(dStation),3,TRUE,TRUE));
		m_ctlHLCombo.InsertString(n,szText);
	}
	m_ctlHLCombo.SetCurSel(0);
}



void CCalcSecForceDivResult::DrawInputDomyunView(BOOL bZoomAll)
{
	CARoadOptionStd	*pOptStd= m_pStd->GetDataManage()->GetOptionStd();
	long	nSel	= max(m_ctlHLCombo.GetCurSel(),0);
	CFEMManage	*pFEM  = m_pStd->m_pCalcStd->GetHLLFemData(nSel);
	CDomyun		*pDom	= m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);

	long	nRow		= max(m_Grid.GetFocusCell().row,0);
	CString szCell		= m_Grid.GetTextMatrix(nRow, 0);
	long	nLoadCase	= m_pStd->GetLoadCase(pFEM, szCell);
	if(nLoadCase<0)	nLoadCase = 0;
	
	if(szCell==_T(""))
	{
		CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
		return;
	}
	
	// 부제 및 하중표현
	CForceDiagram ForceDom(pFEM);

	ForceDom.SetScaleDim(0.1);
	ForceDom.SetTextHeight(1.5);
	ForceDom.DrawFrameModel(TRUE, FALSE, TRUE);
	ForceDom.DrawLoadCase(nLoadCase, FALSE, TRUE, 0.02, FALSE, 1);
	ForceDom.DrawResultForce(nLoadCase);

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

void CCalcSecForceDivResult::OnPreInitDialog()
{
	SetPreComboData();
	m_nCase = 0;
	SetFileName();

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_SAVE_PATH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAPEXPORT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	UpdateData(FALSE);
}

void CCalcSecForceDivResult::OnSelchangeComboHdantype() 
{
	UpdateData(TRUE);

	SetDataInit();

	DrawInputDomyunView(FALSE);	
}

void CCalcSecForceDivResult::OnCellChangedRow(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	m_nCase = GridView->iRow-1;    
	if(m_nCase<0) m_nCase = 0; 

	DrawInputDomyunView(TRUE);
}

void CCalcSecForceDivResult::OnButtonMakeSapS2k() 
{
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");

	UpdateData(TRUE);
	
	CString strf    = _T("");
	int nSize = m_pStd->m_pCalcStd->GetSizeHLLFemData();

	for(int i=0; i<nSize; i++)
	{
		strf = m_sHLLFileName;
		PathToPathNum(strf,_T("s2k"), i+1);
	
		m_pStd->m_pCalcStd->GetHLLFemData(i)->SaveAsSap2000(strf);		
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

void CCalcSecForceDivResult::PathToPathNum(CString & path,CString csExt, long Num) const
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

void CCalcSecForceDivResult::SetFileName()
{
	m_sHLLFileName = m_pStd->m_pDoc->GetPathName();

	int nFine = m_sHLLFileName.Find('.');
	if( nFine>=0)   m_sHLLFileName = m_sHLLFileName.Left(nFine);
	m_sHLLFileName += "_LAT";

	UpdateData(FALSE);
}
