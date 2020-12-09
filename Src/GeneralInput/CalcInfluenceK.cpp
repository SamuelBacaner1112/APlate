// CalcInfluenceK.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcInfluenceK dialog


CCalcInfluenceK::CCalcInfluenceK(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcInfluenceK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcInfluenceK)
	m_sKFileName   = _T("");
	//}}AFX_DATA_INIT
	m_nGir = 0;
}


void CCalcInfluenceK::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcInfluenceK)
	DDX_Text(pDX, IDC_EDIT_RESULT_PATH, m_sKFileName);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_EDIT_RESULT_PATH, m_ctrlEdit1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RECALC_K, m_btnReCalcCoeff);
	DDX_Control(pDX, IDC_BUTTON_MAKE_S2K, m__btnMakeInput);
}

BEGIN_MESSAGE_MAP(CCalcInfluenceK, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcInfluenceK)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_S2K, OnButtonMakeS2k)
	ON_BN_CLICKED(IDC_RECALC_K, OnRecalcInfluence)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcInfluenceK message handlers

void CCalcInfluenceK::SetGridTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	long nColSu = pDB->GetGirder(0)->GetQtyJoint()+1;

	m_Grid.DeleteAllItems();
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(pDB->GetGirdersu()+1);
	m_Grid.SetColumnCount(nColSu);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 70);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.EnableTitleTips(FALSE);	
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
//	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetEditable(FALSE);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetHeaderSort(FALSE);
	m_Grid.SetListMode(TRUE);

	m_Grid.SetTextMatrix(0, 0, "구분");
	CString str("");
	for(long i=0; i<pDB->GetGirdersu(); i++)	
	{
		str.Format("거더 %d", i+1);
		m_Grid.SetTextMatrix(i+1, 0, str);
	}
	for(long n=1; n<nColSu; n++)	
	{
		m_Grid.SetTextMatrix(0, n, "%.0f", double(n));
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CCalcInfluenceK::SetDataInit()
{
	CAPlateCalcStd  *pCalcStd = m_pStd->m_pCalcStd;
	CPlateBridgeApp *pDB      = (CPlateBridgeApp*)m_pStd->GetBridge();
	CCalcData       *pData    = m_pStd->GetDataManage()->GetCalcData();
	CFEMManage      *pFEMManage = NULL;

	UpdateData(FALSE);

	// 영향계수 계산
	if(m_pStd->m_pCalcStd->GetSizeInfluenceResult()==0)
	{
		pData->m_bRunInfluence = FALSE;
		CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

		HLLCalc.RunInfluenceAnalysis();	
	}

	SetGridTitle();

	m_Grid.SetRedraw(TRUE,TRUE);
	
	CString str("");
	for(long n=0; n<pDB->GetGirdersu(); n++)
	{
		pFEMManage = pCalcStd->GetInfluenceFemData(n);
		if(pFEMManage->GetElementSize()<=0) break;

		long nIndex = pFEMManage->GetLoadCaseIndex("LC1");

		for(long nE=0; nE< pFEMManage->GetElementSize(); nE++)
		{
			CElement *pElem = pFEMManage->GetElement((unsigned short)nE);
			CResultForceElement *pResult = pElem->GetResultForce((unsigned short)nIndex);
			if(pResult)
			{
				CResultForceElementData* pResultData = pResult->m_ResultForceElement[0];
				str.Format("%.3lf", pResult->m_ResultForceElement[0]->m_dForceOnElement[ELE_MOMENT2]);
				m_Grid.SetTextMatrix(n+1, nE+1, str);
			}
		}
		// 결과값이 열리면...
		if(pFEMManage->GetElement(0)->GetResultForce(0))
			m_Grid.SetTextMatrix(n+1, pFEMManage->GetElementSize()+1, "1.000");
	}
	UpdateData(FALSE);
}

void CCalcInfluenceK::SetDataSave()
{
}

void CCalcInfluenceK::OnCellChangedRow(NMHDR*nmgv,LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	m_nGir = GridView->iRow-1;    

	DrawInputDomyunView(TRUE);
}

void CCalcInfluenceK::DrawInputDomyunView(BOOL bZoomAll)
{
	CDataManage		*pDataManage	= m_pStd->GetDataManage();

	if(m_pStd->m_pCalcStd->GetSizeInfluenceResult()==0)
		return;

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	CDomyun Dom(pDom);
	Dom.SetScaleDim(400);
	DrawInfK(&Dom, m_nGir, FALSE);
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcInfluenceK::DrawInfK(CDomyun* pDom, long nG, BOOL bExcel)
{
	CDataManage     *pDataManage = m_pStd->GetDataManage();
	CARoadOptionStd *pOptStd	 = pDataManage->GetOptionStd();	
	CPlateBridgeApp *pDB		 = (CPlateBridgeApp*)m_pStd->GetBridge();
	CPlateGirderApp *pGir		 = pDB->GetGirder(nG);
	CAPlateCalcStd  *pCalcStd	 = m_pStd->m_pCalcStd;

	CFEMManage		 *pFEMManage = pCalcStd->GetInfluenceFemData(nG);
	CFemModelingCalc ModelCalc(pDataManage, m_pStd->m_pCalcStd);

	long nNodesu = pFEMManage->GetJointSize();

	CPlateBxFinder	 Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);

	double staStt	 = pBxStt->GetStation();
	double staEnd	 = pBxEnd->GetStation();
	double staGirStt = pGir->GetSSS(OFF_STT_SHOE);
	double sta		 = 0;
	double BaseY	 = 0.0;
	double Len		 = staEnd - staStt;
	double dPreSta=0;
	
	CDDWordArray DimSta, LenSta;
	CPlateBasicIndex *pBx = NULL;

	CString cs(""), cs2("");

	CDomyun Dom(pDom);
	double dSeparatorL = Dom.Always(1.5);

	for(long i = 0; i < 2; i++)	
	{
		double dDimDis = (pDom->GetDimVLen() + pDom->GetDimExo() + pDom->GetDimExe()) * pDom->GetScaleDim() + pDom->Always(50);
		if(i == 1) 		BaseY = - dDimDis;
		
		// 기준선 그림.
		
		pOptStd->SetEnvType(&Dom, HCAD_MIS2);
		
		Dom.MoveTo(staStt, BaseY);
		Dom.LineTo(staEnd, BaseY);
		
		// 절점번호 그림.
		if(bExcel)	Dom.SetTextHeight(5);
		else		Dom.SetTextHeight(3);

		Dom.SetTextAlignHorz(TA_CENTER);
		
		cs.Format("거더 %d 영향선", nG+1);

		if(i==1)	Dom.TextOut(staStt, BaseY+Dom.Always(20), cs);

		for(long nNodeIdx = 0; nNodeIdx < nNodesu; nNodeIdx++)
		{
			CJoint *pJ = pFEMManage->GetJoint((unsigned short)nNodeIdx);
			
			sta = pJ->m_vPointReal.x;

			Dom.MoveTo(sta, BaseY + dSeparatorL);
			Dom.LineTo(sta, BaseY - dSeparatorL);

			CDPoint P1(sta, BaseY + dSeparatorL*1.2);

			cs.Format("%d", nNodeIdx + 1);
			
			if(bExcel)
			{
				if(pJ->IsRestraintJoint()) 
					Dom.TextOut (P1, cs);
			}
			else
				Dom.TextOut (P1, cs);

			if(pJ->IsRestraintJoint())
			{
				if(bExcel)
					Dom.Triangle(sta, BaseY, 5);  // 지점 Symbol 그림.
				else
					Dom.Triangle(sta, BaseY, 3);
			}
		}
	}

	//모멘트하중표시
	double radius = Len*0.02;
	Dom.SetScaleDim(Dom.GetScaleDim()*1.3);
	pOptStd->SetEnvType(&Dom, HCAD_SYMB);

	Dom.TextOut(staStt, Len*0.05, "1N·m");
	Dom.TextOut(staEnd, Len*0.05, "1N·m");
	
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	double dRH = 0.03;
	Dom.Arc(staStt, Len*dRH, radius, 0, 180);
	Dom.Arc(staEnd, Len*dRH, radius, 0, 180);
	Dom.LineTo(staStt+radius, Len*dRH, staStt+radius+radius*0.15, Len*dRH+radius*0.4);
	Dom.LineTo(staStt+radius, Len*dRH, staStt+radius-radius*0.3, Len*dRH+radius*0.3);
	Dom.LineTo(staEnd-radius, Len*dRH, staEnd-radius-radius*0.15, Len*dRH+radius*0.4);
	Dom.LineTo(staEnd-radius, Len*dRH, staEnd-radius+radius*0.3, Len*dRH+radius*0.3);

	*pDom << Dom;

	//모멘트 Diagram
	CDomyun DomBM(pDom);		
	DomBM.SetScaleDim(Dom.GetScaleDim());
	
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	sta = pBx->GetStation();
	DomBM.MoveTo(sta, 0);
	double YScale   = Len*0.1;
	double BMValue  = 0, BMValueEnd = 0;

	for(long nElm = 0; nElm < pFEMManage->GetElementSize(); nElm++)
	{
		CElement *pElm  = pFEMManage->GetElement((unsigned short)nElm); 
		CJoint   *pJStt = pFEMManage->GetJoint(pElm->m_nJStt);
		CJoint   *pJEnd = pFEMManage->GetJoint(pElm->m_nJEnd);
		
		sta = pJStt->m_vPointReal.x;
		BMValue = pElm->GetResultForce(0)->m_ResultForceElement[0]->m_dForceOnElement[ELE_MOMENT2];

		if(nElm==pFEMManage->GetElementSize()-1)
		{
			staEnd = pJEnd->m_vPointReal.x;
			BMValueEnd = pElm->GetResultForce(0)->m_ResultForceElement[1]->m_dForceOnElement[ELE_MOMENT2];
			BMValueEnd = 1.0;
		}

		pOptStd->SetEnvType(&DomBM, HCAD_STLC);

		DomBM.LineTo(sta, -BMValue*YScale);
		if(nElm==pFEMManage->GetElementSize()-1)
			DomBM.LineTo(staEnd, -BMValueEnd*YScale);

		if(pJStt->IsRestraintJoint() || (nElm==pFEMManage->GetElementSize()-1 && pJEnd->IsRestraintJoint()))
		{
			pOptStd->SetEnvType(&DomBM, HCAD_SYMB);

			CDPoint P1(0,0);
			if(nElm==pFEMManage->GetElementSize()-1)
			{
				P1 = CDPoint(staEnd, BMValueEnd + Len*0.04);
				cs.Format("%.3f", BMValueEnd);
				DomBM.TextOut (P1, cs);
			}
			else
			{
				P1 = CDPoint(sta, BMValue + Len*0.04);
				cs.Format("%.3f", BMValue);
				DomBM.TextOut (P1, cs);
			}
		}
	}

	pOptStd->SetEnvType(&DomBM, HCAD_STLC);
	DomBM.LineTo(staEnd, 0);
	DomBM.Move(0, BaseY);
	*pDom << DomBM;

	// 지간구성 치수선 그림.	
	Dom.SetDirection("BOTTOM");
	pOptStd->SetEnvType(&Dom, HCAD_DIML);

	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING);
	dPreSta = staGirStt;
	while(pBx)   
	{
		if (!pBx->IsState(BX_CROSSBEAM_VBRACING) || pBx->IsJijum())   {
			sta = pBx->GetStation();
			if (staStt < sta && DimSta.GetSize() == 0)   {  // 치수선 첫부분이 중간에 걸림
				DimSta.Add(staStt);
				LenSta.Add(dPreSta);
			}
			if (staStt <= sta && sta <= staEnd)   {
				DimSta.Add(sta);
				LenSta.Add(sta);
			}
			if (sta > staEnd)   {  // 치수선 마지막이 중간에 걸림
				DimSta.Add(staEnd);
				LenSta.Add(sta);
			}
			if (sta >= staEnd)
				break;
			dPreSta = sta;
		}
		pBx = Finder.GetBxNext();
	}

	long nDan = 0;
	double staDim = DimSta.GetAt(0);
	double staLen = LenSta.GetAt(0);
	
	Dom.DimMoveTo(staDim, 0, nDan);

	double staDimPre = staDim;
	double staLenPre = staLen;
	long nSize = DimSta.GetSize();
	for (long nDim = 1; nDim < nSize; nDim++)   {
		staDim = DimSta.GetAt(nDim);
		staLen = LenSta.GetAt(nDim);
		CString cs = Comma(staLen - staLenPre);
		if (staDim == staLen)
			Dom.DimLineTo(staDim - staDimPre, nDan, cs);
		else
			Dom.DimLineToExtend(staDim - staDimPre, nDan, cs, "", FALSE);

		staDimPre = staDim;
		staLenPre = staLen;
	}

	DimSta.RemoveAll();
	LenSta.RemoveAll();

	*pDom << Dom;

	// 그림 제목
	CDomyun DomText1(pDom);
	DomText1.SetScaleDim(Dom.GetScaleDim()*1.2);
	DomText1.SetTextHeight(3);

	cs="  ⓐ INPUT";
	DomText1.TextOut(staStt, Dom.Always(12), cs);
	*pDom << DomText1;

	CDomyun DomText2(pDom);
	DomText2.SetScaleDim(Dom.GetScaleDim()*1.2);
	DomText2.SetTextHeight(3);
	
	cs="  ⓑ OUTPUT";
	DomText2.TextOut(staStt, BaseY + Dom.Always(20), cs);

	*pDom << DomText2;
}



void CCalcInfluenceK::PathToPathNum(CString & path,CString csExt, long Num) const
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



void CCalcInfluenceK::OnButtonMakeS2k() 
{
	UpdateData(TRUE);

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString strf    = _T("");

	for(int i=0; i<m_pStd->m_pDataManage->GetBridge()->GetGirdersu(); i++)
	{
		strf = m_sKFileName;
		PathToPathNum(strf, _T("s2k"), i+1);
	
		m_pStd->m_pCalcStd->GetInfluenceFemData(i)->SaveAsSap2000(strf);		
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



// 영향계수 재계산
void CCalcInfluenceK::OnRecalcInfluence() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	pData->m_bRunInfluence = FALSE;

	CFemHLLCalc HLLCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	HLLCalc.RunInfluenceAnalysis();	
	
	SetDataInit();
	DrawInputDomyunView(TRUE);
}


void CCalcInfluenceK::SetFileName()
{
	m_sKFileName = m_pStd->m_pDoc->GetPathName();
	int nFine = m_sKFileName.Find('.');
	if( nFine>=0)   m_sKFileName = m_sKFileName.Left(nFine);
	m_sKFileName += "영향계수";

	UpdateData(FALSE);
}


void CCalcInfluenceK::OnPreInitDialog()
{
	SetFileName();

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_RECALC_K, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_RESULT_PATH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MAKE_S2K, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}
