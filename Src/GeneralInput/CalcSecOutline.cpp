// CalcSecOutline.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcSecOutline.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcSecOutline dialog


CCalcSecOutline::CCalcSecOutline(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcSecOutline::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcSecOutline)
	m_dMomentY = 5;
	m_dHeightY = 2.5;
	m_dFlangeY = 10.0;
	m_Static = _T("");
	m_bPic = FALSE;
	//}}AFX_DATA_INIT
}


void CCalcSecOutline::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcSecOutline)
	DDX_Control(pDX, IDC_S_SUMMARY_COMBO1, m_Girder);
	DDX_Text(pDX, IDC_S_SUMMARY_EDIT1, m_dMomentY);
	DDX_Text(pDX, IDC_S_SUMMARY_EDIT2, m_dHeightY);
	DDX_Text(pDX, IDC_S_SUMMARY_EDIT3, m_dFlangeY);
	DDX_GridControl(pDX, IDC_S_SUMMARY_GRID, m_Grid);
	DDX_Text(pDX, IDC_S_SUMMARY_STATIC, m_Static);
	DDX_Check(pDX, IDC_S_SUMMARY_PIC, m_bPic);
	DDX_Control(pDX, IDC_S_SUMMARY_EDIT1, m_ctrlEdit1);
	DDX_Control(pDX, IDC_S_SUMMARY_EDIT2, m_ctrlEdit2);
	DDX_Control(pDX, IDC_S_SUMMARY_EDIT3, m_ctrlEdit3);
	DDX_Control(pDX, IDC_S_SUMMARY_EDIT4, m_ctrlEdit4);
	DDX_Control(pDX, IDC_S_SUMMARY_EDIT5, m_ctrlEdit5);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_S_SUMMARY_DATA, m_btnMoment);
	DDX_Control(pDX, IDC_BUTTON_SECTION_CHECK, m_btnCheckSec);
	DDX_Control(pDX, IDC_S_SUMMARY_DEL, m_btnDelete);

}

BEGIN_MESSAGE_MAP(CCalcSecOutline, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcSecOutline)
	ON_BN_CLICKED(IDC_S_SUMMARY_DATA, OnSSummaryData)
	ON_BN_CLICKED(IDC_S_SUMMARY_DEL, OnSSummaryDel)
	ON_BN_CLICKED(IDC_S_SUMMARY_PIC, OnSSummaryPic)
	ON_CBN_SELCHANGE(IDC_S_SUMMARY_COMBO1, OnSelchangeSSummaryCombo1)
	ON_BN_CLICKED(IDC_BUTTON_SECTION_CHECK, OnButtonSectionCheck)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_S_SUMMARY_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcSecOutline message handlers
void CCalcSecOutline::SetGridTitle()
{
	m_Grid.DeleteAllItems();
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);

	m_Grid.SetRedraw(TRUE,TRUE);

}

void CCalcSecOutline::OnPreInitDialog()
{
	CPlateBridgeApp *pDB   = m_pStd->GetBridge();
	CFemStressCalc  StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

//	if(m_pStd->m_pCalcStd->m_vByunDanInfo.size()==0)
	{
		StressCalc.SetSectionGugan();
		StressCalc.SetSectionSplice();
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
			StressCalc.MakeDanmyunMomentData(nG);
	}

	CString cs="";
	m_Girder.ResetContent();

	for (long nG=0; nG<pDB->GetGirdersu(); nG++)  
	{
		cs.Format("%d", nG+1);
		m_Girder.AddString(cs);
	}
	m_nG = 0;
	m_Girder.SetCurSel(0);
}

BOOL CCalcSecOutline::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	if(pData->m_ArrExtraSecGir.GetSize()<1)
	{
		GetDlgItem(IDC_BUTTON_SECTION_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_S_SUMMARY_DEL)->EnableWindow(FALSE);
	}
	m_nIdxSelectedSec = 0;

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_STATIC_GROUPBOX1, SZ_TOP_LEFT, SZ_BOTTOM_CENTER);
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_SUMMARY_COMBO1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_SUMMARY_EDIT1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_SUMMARY_EDIT2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_SUMMARY_EDIT3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_SUMMARY_EDIT4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_SUMMARY_EDIT5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_S_SUMMARY_GROUP, SZ_TOP_CENTER, SZ_BOTTOM_RIGHT);
	SetResize(IDC_S_SUMMARY_GRID, SZ_TOP_CENTER, SZ_BOTTOM_RIGHT);
	SetResize(IDC_S_SUMMARY_STATIC, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_S_SUMMARY_PIC, SZ_TOP_CENTER, SZ_TOP_CENTER);

	SetResize(IDC_S_SUMMARY_DATA, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SECTION_CHECK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_S_SUMMARY_DEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;
}

void CCalcSecOutline::SetDataDefault()
{
	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp *pDB   = m_pStd->GetBridge();

	m_nG = pDB->GetGirdersu()-1;
	pData->m_dMatMinSpace = 1000;
	pData->m_dMatMaxSpace = 100000;
}

void CCalcSecOutline::SetDataInit()
{
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();
	CFEMManage      *pManage = m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	CString cs       = "";
	long   nG        = 0;
	long   nStrsu    = pData->m_ArrExtraSecGir.GetSize(); ///< 거더별 추가 단면수
	double dMinSpace = pData->m_dMatMinSpace;
	double dMaxSpace = pData->m_dMatMaxSpace;
	long   nElm=0;
	double dSta = 0;
	///////////////////////////////////////////////////////////////////////////
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_S_SUMMARY_EDIT4);
	cs.Format("%.1f", dMinSpace);
	pEdit->SetWindowText(cs);

	pEdit = (CEdit*)GetDlgItem(IDC_S_SUMMARY_EDIT5);
	cs.Format("%.1f", dMaxSpace);
	pEdit->SetWindowText(cs);
	///////////////////////////////////////////////////////////////////////////
	// Grid생성
	if(nStrsu > 0)	//추가 단면이 있을 경우...
	{
		GetDlgItem(IDC_BUTTON_SECTION_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_S_SUMMARY_DEL)->EnableWindow(TRUE);
		m_Grid.SetRows(nStrsu+1);
		m_Grid.SetCols(3);

		for(long i=0; i<nStrsu; i++)
		{
			cs.Format("%s", pData->m_ArrExtraSecTitle[i]);	// 타이틀
			m_Grid.SetTextMatrix(i+1, 0, cs);

			nG   = pData->m_ArrExtraSecGir[i];
			dSta = pData->m_ArrExtraSecSta[i];

			double dRatio  = 0;
			nElm = ModelCalc.GetElemNoAndRatio(nG, dSta, dRatio);
			double dLengthElm     = ModelCalc.GetElemLength(nElm, FALSE);
			double dLengthFromStt = dRatio*dLengthElm;
			CElement *pElm = pManage->GetElement((unsigned short)nElm);
			
			cs.Format("절점 %d 부터 %.3f m", pElm->m_nJStt+1, dLengthFromStt);
			m_Grid.SetTextMatrix(i+1, 1, cs);
			cs.Format("%s", GetStationForStringOut(frM(dSta),3,TRUE,TRUE));
			m_Grid.SetTextMatrix(i+1, 2, cs);
		}
		cs.Format("(전체 %d개)", nStrsu);
		m_Static = cs;
	}
	else
	{
		// 초기화
		m_Grid.SetRows(1);
		m_Grid.SetCols(3);
		m_Static = "(전체 0개)";
	}

	m_Grid.SetTextMatrix(0, 0, "TITLE ");
	m_Grid.SetTextMatrix(0, 1, "위  치 ");
	m_Grid.SetTextMatrix(0, 2, "STATION");
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 150);
	m_Grid.SetColumnWidth(2, 150);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRedraw(TRUE,TRUE);
	UpdateData(FALSE);
}

void CCalcSecOutline::SetDataSave()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	pData->m_dMatMinSpace = GetMinSpace();
	pData->m_dMatMaxSpace = GetMaxSpace();
}

double CCalcSecOutline::GetMinSpace()
{
	CString str="";
	GetDlgItemText(IDC_S_SUMMARY_EDIT4,str);

	return atof(str);
}

double CCalcSecOutline::GetMaxSpace()
{
	CString str="";
	GetDlgItemText(IDC_S_SUMMARY_EDIT5,str);

	return atof(str);
}

#pragma optimize("", off)
void CCalcSecOutline::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB	     = m_pStd->GetDataManage()->GetBridge();
	CDataManage     *pDataManage = m_pStd->GetDataManage();
	CARoadOptionStd *pOptStd     = pDataManage->GetOptionStd();	
	CAPlateDrawPyung DrawPyung(pDataManage);

	CDomyun *pDom = m_pView->GetDomyun();	
	pDom->ClearEtt(TRUE);		

	CDomyun Dom(pDom);
	Dom.ClearEtt(TRUE);
	CFemStressCalc  StressCalc(pDataManage, m_pStd->m_pCalcStd);
	m_nG = m_Girder.GetCurSel();

	CDRect	BoundRect;
	StressCalc.OutputOfMomentDiagram(&Dom, m_nG, m_dMomentY);
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();

	StressCalc.OutputOfExtraSection(&Dom, BoundRect);
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();

	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 150*(dDiagonalL/35000);
	if(dDiagonalL>200000)	dScale = 300;

	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetScaleDim(dScale*1.5);
	Dom.TextOut(BoundRect.CenterPoint().x, BoundRect.bottom + Dom.Always(5), "- 부재력도 -");
	Dom.Move(0, -BoundRect.top);

	*pDom << Dom;	

	StressCalc.OutputOfSectionSummary(&Dom, m_nG, m_dHeightY, m_dFlangeY, dScale);
	Dom.SetCalcExtRect();
	BoundRect  = Dom.GetExtRect();

	Dom.TextOut(BoundRect.CenterPoint().x, BoundRect.bottom + Dom.Always(5), "- 단면요약도 -");
	Dom.Move(0, -BoundRect.bottom - Dom.Always(30));

	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}
#pragma optimize("", on)

void CCalcSecOutline::OnSSummaryData() 
{
	CPlateBridgeApp *pDB     = m_pStd->GetBridge();

	// 상태바
	CTStandard *pTS = m_pStd->GetDataManage()->GetBMStandard()->m_pTStandard;
	CRect rect = CRect(200, 3, 400, 18);
	if(!m_pStd->m_ctlProgress.GetSafeHwnd())
		m_pStd->m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_pStd->m_ctlProgress.ShowWindow(SW_SHOW);	
	pTS->SetPaneText(0, "단면검토 계산중...", TRUE);
		m_pStd->m_ctlProgress.SetPos(0);
	//
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CFemStressCalc StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemModelingCalc ModelingCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	if(!pData->m_bReadFrameModelingOutput)
	{
		int nResult = MessageBox("  해석결과 파일이 열리지 않았습니다. \n\n  해석결과 파일을 여신 후 실행하시기 바랍니다.",
			"Output File UnLoaded", MB_OK | MB_ICONEXCLAMATION);
		if(nResult == IDOK) 
			return;
	}

	// 아래부분을 확인 요망합니다.
	int nResult = IDOK;
//	if (pData->m_ArrMaxM[0].GetSize ())
//		nResult = MessageBox ("  이미 실행되었습니다.\n  계속 하시겠습니까?",
//			" Data 생성 확인", MB_OKCANCEL | MB_ICONQUESTION);
//	else nResult = IDOK;

	if (nResult == IDOK)   
	{
		m_pStd->m_ctlProgress.SetRange(0, 6);
		m_pStd->m_ctlProgress.ShowWindow(TRUE);
		pData->m_dMatMinSpace = GetMinSpace();
		pData->m_dMatMaxSpace = GetMaxSpace();
		m_pStd->m_ctlProgress.SetPos(3);
		StressCalc.SetSectionGugan();
		StressCalc.SetSectionSplice();
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
			StressCalc.MakeDanmyunMomentData(nG);
		m_pStd->m_ctlProgress.SetPos(4);
		ModelingCalc.MakeMaxMinForceDB();
		m_pStd->m_ctlProgress.SetPos(4);
		ModelingCalc.MakeMaxMinForceDL();
		m_pStd->m_ctlProgress.SetPos(5);
		ModelingCalc.MakeMaxMinForceMTY();
		m_pStd->m_ctlProgress.SetPos(5);
		ModelingCalc.MakeMaxMinForcePeople();
		m_pStd->m_ctlProgress.SetPos(6);
		ModelingCalc.MakeMaxMinForceWind();
		m_pStd->m_ctlProgress.SetPos(6);
		m_pStd->m_ctlProgress.ShowWindow(FALSE);
		UpdateData(FALSE);
		pTS->SetPaneText(0, "", TRUE);
	}

	SetDataSave();
	SetDataInit();
	pTS->SetPaneText(0, "", TRUE);

	DrawInputDomyunView(TRUE);
}


void CCalcSecOutline::SetLButtonDblClk(UINT nFlags, CPoint uXy)
{
	CPlateBridgeApp *pDB     = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	if(!pData->m_bReadFrameModelingOutput)
	{
		int nResult = MessageBox ("  해석결과 파일이 열리지 않았습니다. \n\n  해석결과 파일을 여신 후 실행하시기 바랍니다.",
			"OutputFile UnLoaded", MB_OK | MB_ICONEXCLAMATION);
		if(nResult == IDOK) 
			return;
	}

	CDomyun *pDom = m_pView->GetDomyun();
	CDPoint  Pick = pDom->ClientToExt(uXy);

	CStringArray Arr;
	double dRatio=0;
	long   nElm =0;
	double dSta = Pick.x + pGir->GetSSS(OFF_STT_GIRDER);
	// Bx 근접위치 찾음
	GetStationNearBx(m_nG, dSta);

	CCalcSecStressPos StressPosionDlg(m_pStd);
	StressPosionDlg.InitDlgData(m_nG, dSta);

	if(StressPosionDlg.DoModal()==IDOK)
	{		
		nElm = ModelCalc.GetElemNoAndRatio(m_nG, dSta, dRatio);	
		double sta = ModelCalc.GetStationByElmAndRatio(nElm, 0);  // 요소의 시점
				
		dSta  = sta + StressPosionDlg.m_dCheckLength*1000;
		
		long nResult = MessageBox("  구조계산서 출력시 \n 현재 선택 위치를 추가 하시겠습니까?",
			" Excel 출력 추가 확인", MB_YESNOCANCEL | MB_ICONQUESTION);
		if(nResult == IDYES)
		{
			CString cs;
			pData->m_ArrExtraSecGir.Add(m_nG);
			pData->m_ArrExtraSecSta.Add(dSta);
			cs.Format("    추가단면%d", pData->m_ArrExtraSecGir.GetSize());
			pData->m_ArrExtraSecTitle.Add(cs);
			SetDataInit();
		}
		if(nResult != IDCANCEL)
		{
			CPlateStressOut	pStressOut(m_pStd);
 
			if(pDB->IsTUGir())
			{
				CXLControl	XL;
//				#ifdef _DEBUG
//					XL.SetXLVisible(TRUE);
//				#endif
				CString szPath	= AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
				szPath += "\\Analyxl4.dll";
				XL.OpenXL(szPath);

//				CPlateBasicIndex *pBx = pDB->GetGirder(m_nG)->GetBxByStation(dSta);
				CDPoint vBx(m_nG, dSta);
				CDPoint *pBx;
				pBx = &vBx;
				long nEndRow = AfxGetMainWnd()->SendMessage(WM_USER+200, (LPARAM)&XL, (WPARAM)pBx);

				CUserDesignCheckDlg UserCheckDlg(&XL, "단면검토 사용자 설계점검");
				UserCheckDlg.SetXLRange(1, 1, nEndRow, 27);
				UserCheckDlg.DoModal();
			}
			else
			{
				CStringArray Arr;
				pStressOut.StressCheckCoefOutput(Arr, m_nG, dSta, TRUE);
				m_pStd->OutputViewOnAView(Arr);
			}
		}
	}

	DrawInputDomyunView(FALSE);
}

// 더블클릭시 Bx에 근접한 위치가 선택되면 Bx의 Sta를 리턴
void CCalcSecOutline::GetStationNearBx(long nG, double &dSta)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir= pDB->GetGirder(nG);
	CPlateBasicIndex *pBx = pGir->GetBxByStation(dSta);
	
	if(fabs(dSta - pBx->GetStation()) < 20)
		dSta = pBx->GetStation();
	
}


void CCalcSecOutline::OnSSummaryDel() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	long nRowSel = m_Grid.GetFocusCell().row;
	if(nRowSel <= 0 || m_Grid.GetRowCount() <= 1) return;
	
	CString cs;
	cs.Format("%s를 삭제 하시겠습니까?", m_Grid.GetTextMatrix(nRowSel, 0));
	cs.TrimLeft();
	long nResult = MessageBox(cs, " 삭제 확인", MB_OKCANCEL | MB_ICONQUESTION);
	if(nResult != IDOK) return;
	
	long nRow = m_Grid.GetRows();
	long nCol = m_Grid.GetCols();
	
	CMatrixStr SMat;
	SMat.Resize(nRow, nCol);
	for(long r = 0; r < nRow; r++)
		for(long c = 0; c < nCol; c++)
			SMat.SetTextMatrix(r, c, m_Grid.GetTextMatrix(r, c));
		
	SMat.DeleteRow(nRowSel);
	m_Grid = SMat;
	
	pData->m_ArrExtraSecTitle.RemoveAt(nRowSel - 1);
	pData->m_ArrExtraSecGir.RemoveAt(nRowSel - 1);
	pData->m_ArrExtraSecSta.RemoveAt(nRowSel - 1);
	
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 150);
	
	cs.Format("(전체 %d개)", m_Grid.GetRows() - 1);
	m_Static = cs;
	if(pData->m_ArrExtraSecGir.GetSize()>0)
	{
		GetDlgItem(IDC_BUTTON_SECTION_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_S_SUMMARY_DEL)->EnableWindow(TRUE);
	}
	else
	{	
		GetDlgItem(IDC_BUTTON_SECTION_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_S_SUMMARY_DEL)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);

	DrawInputDomyunView(FALSE);
}

void CCalcSecOutline::OnSSummaryPic() 
{
	m_bPic = !m_bPic;
	UpdateData(FALSE);
}


void CCalcSecOutline::OnSelchangeSSummaryCombo1() 
{
	UpdateData(TRUE);
	SetDataSave();
	SetDataInit();
	
	DrawInputDomyunView(FALSE);
}

void CCalcSecOutline::OnCellChangedRow(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    m_nIdxSelectedSec = GridView->iRow-1;
}

void CCalcSecOutline::OnButtonSectionCheck() 
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();
	CPlateStressOut	pStressOut(m_pStd);

	long nG = pData->m_ArrExtraSecGir.GetAt(m_nIdxSelectedSec);
	double dSta = pData->m_ArrExtraSecSta.GetAt(m_nIdxSelectedSec);

	if(pDB->IsTUGir())
	{
		CXLControl	XL;
#ifdef _DEBUG
	XL.SetXLVisible(TRUE);
#endif
		CString szPath	= AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
		szPath += "\\Analyxl4.dll";
		XL.OpenXL(szPath);

		CDPoint vBx(nG, dSta);
		CDPoint *pBx;
		pBx = &vBx;
		
		long nEndRow = AfxGetMainWnd()->SendMessage(WM_USER+200, (LPARAM)&XL, (WPARAM)pBx);

		CUserDesignCheckDlg UserCheckDlg(&XL, "단면검토 사용자 설계점검");
		UserCheckDlg.SetXLRange(1, 1, nEndRow, 27);
		UserCheckDlg.DoModal();
	}
	else
	{
		CStringArray Arr;
		pStressOut.StressCheckCoefOutput(Arr, m_nG, dSta, TRUE);
		m_pStd->OutputViewOnAView(Arr);
	}
}
