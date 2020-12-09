// CalcDisplacement.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcDisplacement dialog


CCalcDisplacement::CCalcDisplacement(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcDisplacement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcDisplacement)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nLoadComb = 0;
	m_nLoadCase = 0;
}


void CCalcDisplacement::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcDisplacement)
	DDX_Control(pDX, IDC_S_DISPLACE_COMBO1, m_comboGirder);
	DDX_Control(pDX, IDC_S_DISPLACE_COMBO2, m_comboJigan);
	DDX_Control(pDX, IDC_S_DISPLACE_COMBO3, m_comboLoadComb);
	DDX_Control(pDX, IDC_S_DISPLACE_EDIT1, m_ctrlEdit1);
	DDX_Control(pDX, IDC_S_DISPLACE_EDIT2, m_ctrlEdit2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_S_DISPLACE_READFILE, m_btnReadFile);
	DDX_Control(pDX, IDC_S_DISPLACE_TOTEXT, m_btnOutText);
}


BEGIN_MESSAGE_MAP(CCalcDisplacement, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcDisplacement)
	ON_BN_CLICKED(IDC_S_DISPLACE_TOTEXT, OnDisplaceTotext)
	ON_CBN_SELCHANGE(IDC_S_DISPLACE_COMBO1, OnSelchangeDisplaceCombo1)
	ON_CBN_SELCHANGE(IDC_S_DISPLACE_COMBO2, OnSelchangeDisplaceCombo2)
	ON_CBN_SELCHANGE(IDC_S_DISPLACE_COMBO3, OnSelchangeDisplaceCombo3)
	ON_BN_CLICKED(IDC_S_DISPLACE_READFILE, OnSDisplaceReadfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcDisplacement message handlers
void CCalcDisplacement::OnPreInitDialog()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	if(pData->m_bReadFrameModelingOutput==FALSE)	return;  

	SetComboData();
	Calc_Displacement();
}

void CCalcDisplacement::SetDataInit()
{


}

void CCalcDisplacement::SetDataDefault()
{
	CCalcData       *pData      = m_pStd->GetDataManage()->GetCalcData();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	if(pData->m_bReadFrameModelingOutput==FALSE)  
	{
		MessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.", "WARNING", MB_OK | MB_ICONINFORMATION);
		return;
	}
}

void CCalcDisplacement::SetDataSave()
{


}



void CCalcDisplacement::OnSelchangeDisplaceCombo1() 
{
	Calc_Displacement();
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(TRUE);
}

void CCalcDisplacement::OnSelchangeDisplaceCombo2() 
{
	Calc_Displacement();
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(TRUE);
}

void CCalcDisplacement::OnSelchangeDisplaceCombo3() 
{
	Calc_Displacement();
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(TRUE);
}

void CCalcDisplacement::OnDisplaceTotext() 
{
}

void CCalcDisplacement::SetComboData()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	CString sValue="";
	m_comboGirder.ResetContent ();
	m_comboJigan.ResetContent();
	long n = 0;
	for (n=0; n<pDB->GetGirdersu(); n++)  
	{
		sValue.Format ("거더 %d", n+1);
		m_comboGirder.AddString (sValue);
	}

	for (n=0; n<pDB->m_nQtyJigan; n++)
	{
		sValue.Format ("지간 %d", n+1);
		m_comboJigan.AddString (sValue);
	}

	m_comboGirder.SetCurSel (0);
	m_comboJigan.SetCurSel (0);

	SetComboLoadComb();
}

void CCalcDisplacement::SetComboLoadComb()
{
	CAPlateCalcStd  *pCalcStd   = m_pStd->m_pCalcStd;
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	m_comboLoadComb.ResetContent ();
	CFEMManage *pFEM = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	
	if (pFEM == NULL)		return;
	
/*	for (int nLC = 0; nLC < pFEM->GetLoadCaseSize(); nLC++)   
	{
		CString sName = pFEM->m_szLCName[nLC];
		if (sName == "" || sName.Right (4) == " MAX")
			continue;
		if (ModelCalc.GetLiveLoadDisplace (nLC, -1) == 0.0)
			continue;
		if (sName.Right (4) == " MIN")
			sName = sName.Left (sName.GetLength () - 4);
		m_comboLoadComb.AddString (sName);
	}
	m_nLoadComb = min(m_nLoadComb, m_comboLoadComb.GetCount ()-1);
	if (m_comboLoadComb.GetCount () == 0)
		return;
	if (m_nLoadComb < 0)   {  // 처음 실행.
		if ((m_nLoadComb = m_comboLoadComb.SelectString (0, "DEFORM")) == CB_ERR)   {
			if ((m_nLoadComb = m_comboLoadComb.SelectString (0, "DESIGN")) == CB_ERR)
				m_nLoadComb = m_comboLoadComb.GetCount () - 1;   // DEFORM LC가 없으면 마지막 LC 선택.
		}
	}	
*/
	
	for (long n=0; n<pFEM->m_szLCName.GetSize(); n++)
	{
		CString cs = pFEM->m_szLCName[n];
		if(cs.Find("MAX")!=-1 || cs.Find("MIN")!=-1)
			m_comboLoadComb.AddString(cs);
	}
	
	m_comboLoadComb.SetCurSel(m_comboLoadComb.GetCount()-1);

	GetLoadCase();
}

int CCalcDisplacement::GetLoadCase()
{
	CAPlateCalcStd *pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage     *pFEM = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);

	long nLoadCase = 0;
	for (long n=0; n<pFEM->GetLoadCaseSize(); n++)
	{
		CString cs = pFEM->m_szLCName[n];
		if(cs.Find("MAX")==-1 && cs.Find("MIN")==-1)
			nLoadCase++;
	}
	
	return m_comboLoadComb.GetCurSel()+nLoadCase;
}

void CCalcDisplacement::DrawInputDomyunView(BOOL bZoomAll)
{
	CPlateBridgeApp *pDB     = m_pStd->GetDataManage()->GetBridge();	
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();

	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	if(pData->m_bReadLiveDispOutput==FALSE)	return;  
		//
	long nG  = m_comboGirder.GetCurSel();
	long nLC = GetLoadCase();
	if (nG == CB_ERR) 	nG = 0;
	if (nLC== -1)		nLC = GetLoadCase();

	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	double SttSta = pGir->GetStationStt();
	double EndSta = pGir->GetStationEnd();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 입력그림	
	CDomyun Dom(pDom);
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 스케일 조정
	CDomyun DomTemp(pDom);		
	DomTemp.SetScaleDim(1);	// 텍스트 크기를 무시함.
	DisplacementOutput(&DomTemp, nG, nLC);
	DomTemp.SetCalcExtRect();
	CDRect BoundRect  = DomTemp.GetExtRect();
	double dDiagonalL = sqrt(BoundRect.Height()*BoundRect.Height()+BoundRect.Width()*BoundRect.Width());
	double dScale     = 350*(dDiagonalL/70000);
	Dom.SetScaleDim(dScale);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 그림
	DisplacementOutput(&Dom, nG, nLC);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 제목 쓰기
	CString cs;
	cs.Format ("< Girder %d >", nG + 1);
	Dom.SetLineColor(4);
	Dom.SetTextAlignHorz (TA_LEFT);
	Dom.TextOut(CDPoint(0, Dom.GetTextHeight()*5), cs);
	Dom.SetLineColor(-1);
	Dom.MoveTo(-6000, 0.0);
	Dom.LineTo(EndSta-SttSta+6000, 0);

	*pDom << Dom;	
	
	// 화면의 중앙으로 정렬
	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	CRect   DlgRect;  
 
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	BoundRect.top -= BoundRect.Height()*0.5;
	BoundRect.bottom +=BoundRect.Height() * 0.2;
	Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);	
	Dom.Invalidate();
	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcDisplacement::Calc_Displacement()
{
	CPlateBridgeApp *pDB   = m_pStd->GetDataManage()->GetBridge();
	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	if(pData->m_bReadLiveDispOutput==FALSE)	return;  

	UpdateData (TRUE);
	long nG = m_comboGirder.GetCurSel();
	long nK = m_comboJigan.GetCurSel();
	long nLC= GetLoadCase();

	if (nG == CB_ERR) 		nG = 0;
	if (nK == CB_ERR) 		nK = 0;
	if (nLC== -1)			nLC= GetLoadCase();

	if (ModelCalc.GetLiveLoadDisplace (nLC, -1) == 0.0)
		return;

	CPlateGirderApp *pGir = pDB->GetGirder(nG);

	double dLength = toM (pDB->m_dLengthJigan [nK]);   // 지간 거리.
	double dDa     = 0;
	CString sValue, sDa;
	if(dLength > 40)
	{
		dDa = dLength / 500;
		sDa = "L / 500";
	}
	else if (dLength > 10)
	{
		dDa = dLength * dLength / 20000;
		sDa = "L²/ 20000";
	}
	else   {
		dDa = dLength / 2000;
		sDa = "L / 2000";
	}
	dDa = frM(dDa);
	double dDmax = GetMaxDisplace(nG, nK, nLC);
	if (dDmax < dDa)
		sValue.Format (">>  δ=  %.3f mm   <   δa = %s =  %.3f mm   -->   O.K.", dDmax, sDa, dDa);
	else
		sValue.Format (">>  δ=  %.3f mm   >   δa = %s =  %.3f mm   -->   N.G.", dDmax, sDa, dDa);

	CStatic *pStatic = (CStatic *) GetDlgItem(IDC_S_DISPLACE_STATIC);
	pStatic->SetWindowText (sValue);

	CEdit *pEdit;
	sValue.Format ("%.3f", dDmax);
	pEdit = (CEdit *) GetDlgItem (IDC_S_DISPLACE_EDIT1);
	pEdit->SetWindowText (sValue);
	sValue.Format ("%.3f", dDa);
	pEdit = (CEdit *) GetDlgItem (IDC_S_DISPLACE_EDIT2);
	pEdit->SetWindowText (sValue);

}

double CCalcDisplacement::GetMaxDisplace(long nG, long nK, long nLC)
{
	CPlateBridgeApp *pDB  = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CAPlateCalcStd  *pCalcStd = m_pStd->m_pCalcStd;

	CPlateBxFinder Finder(pDB->GetGirder(nG));
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBx    = NULL;
	CFEMManage       *pFEM   = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	int nJigan = 0;
	double dValue, dMaxValue = 0.0;
	int nNodeSu = pFEM->GetJointSize();
	for (int nNode = 0; nNode < nNodeSu; nNode++)
	{
		CJoint *pJ = pFEM->GetJoint(nNode);
		if (ModelCalc.GetGirderNoByJointID(nNode) != nG)
			continue;
		pBx    = pCalcStd->GetBxByJoint(pFEM->GetJoint(nNode));
		dValue = frM(fabs(ModelCalc.GetLiveLoadDisplace(nLC, nNode)));
		dMaxValue = max(dValue, dMaxValue);

		if (pBx != pBxStt && pBx->IsJijum()) 
		{
			if (nJigan == nK)
				return dMaxValue;
			dMaxValue = 0.0;
			nJigan ++;
		}
	}

	return 0.0;
}

void CCalcDisplacement::DisplacementOutput(CDomyun *pDom, long nG, long nLC)
{
	CPlateBridgeApp *pDB  = m_pStd->GetDataManage()->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CAPlateCalcStd  *pCalcStd = m_pStd->m_pCalcStd;
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFEMManage		*pFEM = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CARoadOptionStd *pOptStd = m_pStd->GetDataManage()->GetOptionStd();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);

	double BaseX = pBxStt->GetStation();
	double BaseY = 0.0;
	double sta, dValue;
	CString cs;

	double YScale = 5000;
	dValue = fabs(ModelCalc.GetLiveLoadDisplace(nLC, -1));
	if(fabs(dValue) > 0.000001)
		YScale = (pDB->GetLengthBridge() * 0.7 + 30000) / (dValue * 1000);
	YScale/=10.0;

	CDomyun Dom(pDom);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 기준선 그림.
	Dom.SetLineColor(0);
	Dom.MoveTo (BaseX, BaseY);
	Dom.LineTo (pBxEnd->GetStation(), BaseY);
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 절점번호 그림.
	Dom.SetTextAlignHorz (TA_CENTER);
	long nIdxPrev = 0;
	long nIdxNext = 0;
	ModelCalc.GetJointRangeByGirder(nG, nIdxPrev, nIdxNext);
	long nNode = 0;
	for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++)   
	{
		CJoint *pJ = pFEM->GetJoint((unsigned short)nNode);
		sta = pJ->m_vPointReal.x;

		Dom.MoveTo (sta, BaseY + 400);
		Dom.LineTo (sta, BaseY - 400);
		cs.Format ("%d", nNode + 1);
		Dom.TextOut (CDPoint (sta, BaseY + 500), cs);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 지점구속부 그림.
	for (long nJijum=0; nJijum < pDB->m_nQtyJigan+1; nJijum++)   
	{
		sta = pGir->GetBxOnJijum(nJijum)->GetStation();
		cs  = pDB->m_strJijumName[nJijum];

		pOptStd->SetEnvType(&Dom, HCAD_TXTG);
		Dom.TextOut(CDPoint (sta, BaseY+Dom.GetTextHeight()*3), cs);
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		Dom.Triangle(sta, BaseY, 5 * 1.3);  // 지점 Symbol 그림.
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//
	if (dValue == 0.0)
		return;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 처짐선 그림
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetLineColor(2);
	for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++)   
	{
		CJoint *pJ = pFEM->GetJoint((unsigned short)nNode);
		sta = pJ->m_vPointReal.x;

		dValue = frM(ModelCalc.GetLiveLoadDisplace(nLC, nNode));

		if (ModelCalc.IsRestraintJoint(nNode))		dValue = 0.0;

		if (nNode == nIdxPrev)
			Dom.MoveTo (sta, 0.0);
		else
			Dom.LineTo (sta, dValue * YScale);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 최대값 그림
	Dom.SetLineColor(3);
	Dom.SetTextAlignHorz (TA_LEFT);
	int nJigan = 0;
	double dMaxValue = 0.0, dMaxDist = 0.0;
	for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++) 
	{
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		CJoint *pJ = pFEM->GetJoint((unsigned short)nNode);
		sta = pJ->m_vPointReal.x;
		dValue = frM(ModelCalc.GetLiveLoadDisplace(nLC, nNode));

		if (fabs (dValue) > fabs (dMaxValue))  
		{
			dMaxValue = dValue;
			dMaxDist  = sta;
		}
		if (nNode != nIdxPrev && ModelCalc.IsRestraintJoint(nNode))
		{
			if(fabs(dMaxValue)>0.000001)
			{
				Dom.MoveTo (dMaxDist, 0.0);
				Dom.LineTo (dMaxDist, dMaxValue * YScale);
				CDPoint P1 (dMaxDist, dMaxValue * YScale * 0.6);
				cs.Format (" δ%d = %.3f mm", ++nJigan, fabs (dMaxValue));
								Dom.MoveTo (dMaxDist, 0.0);

				Dom.TextOut (P1, cs);
				dMaxValue = 0.0;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	Dom.Move(-BaseX, 0);

	*pDom << Dom;
}

BOOL CCalcDisplacement::IsValid()
{
	return TRUE;
}

void CCalcDisplacement::OnSDisplaceReadfile() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CAPlateCalcStd  *pCalcStd = m_pStd->m_pCalcStd;
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("계산결과를 먼저 불러오셔야 합니다",MB_OK | MB_ICONWARNING);
		return;
	}
	/*
	if(ModelCalc.GetLiveLoadDisplace(-1, -1) != 0.0)
	{
		if (MessageBox("이미 Load되어 있습니다. 다시 읽으시겠습니까?", "WARNING",
			MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
			return;
	}
	*/

	CString sMessage = "Displace Files (*.TXT)|*.TXT|All Files (*.*)|*.*||";
	CFileDialog dlgFile (TRUE, "", "활하중 처짐 Data", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sMessage);
	CFEMManage *pFEM = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	if(IDOK == dlgFile.DoModal())
	{			
		CPlateSAP2000Disp SapOutput(pFEM);
		CString sName = dlgFile.GetFileName();

		if(SapOutput.LoadSAP2000Disp(sName))   // 파일 읽기 성공하면
		{  
			SetComboData();
			Calc_Displacement();
			
			pData->m_bReadLiveDispOutput = TRUE;
			DrawInputDomyunView(TRUE);
		}
		else
			MessageBox ("File의 내용이 현재 Data와 일치하지 않습니다.", "ERROR", MB_OK | MB_ICONWARNING);
	}
	Calc_Displacement();
}

BOOL CCalcDisplacement::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);
	
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_S_DISPLACE_COMBO1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_DISPLACE_COMBO2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_DISPLACE_COMBO3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC5, SZ_TOP_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_S_DISPLACE_EDIT1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_DISPLACE_EDIT2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	SetResize(IDC_S_DISPLACE_READFILE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_S_DISPLACE_TOTEXT, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);	
	SetResize(IDC_S_DISPLACE_STATIC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	return TRUE;
}
