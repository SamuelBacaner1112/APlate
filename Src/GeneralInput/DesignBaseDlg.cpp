// DesignBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignBaseDlg dialog


CDesignBaseDlg::CDesignBaseDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDesignBaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignBaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDesignBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignBaseDlg)
	DDX_Control(pDX, IDC_TAB_DESIGN, m_ctlTab);
	DDX_Control(pDX, IDC_LIST_ALLOWSTRESS, m_richeditAllowStress);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDesignBaseDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDesignBaseDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DESIGN, OnSelchangeTabDesign)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignBaseDlg message handlers
void CDesignBaseDlg::SetShow()
{
	long nSel = m_ctlTab.GetCurSel();
	if(nSel==0 || nSel==1 || nSel==2 || nSel==3 || nSel == 4|| nSel==5)
	{
		GetDlgItem(IDC_GRID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_LIST_ALLOWSTRESS)->ShowWindow(SW_HIDE);
	}
	else
	{
//		GetDlgItem(IDC_GRID)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_LIST_ALLOWSTRESS)->ShowWindow(SW_SHOW);
		m_Grid.ShowWindow(SW_HIDE);
		m_richeditAllowStress.ShowWindow(SW_SHOW);

		m_richeditAllowStress.SetSel(0, -1);
		m_richeditAllowStress.ReplaceSel("");
		m_richeditAllowStress.SetReadOnly(TRUE);
		CXLControl	XL;
		CWinApp	*pApp		= AfxGetApp();
		CString	szsSection	= "Afx Directory";
		CString	szsFixedDir	= "Fixed Directory";
		CString	szAPlate	= pApp->GetProfileString(szsSection, szsFixedDir); 
		szAPlate += _T("\\DlgObject.dll");

		XL.OpenXL(szAPlate);
		m_richeditAllowStress.AddText("\t");
		if(nSel==4)		XL.SetActiveSheet("Allow_Comp");
		if(nSel==5)		XL.SetActiveSheet("Allow_Tension");
		if(nSel==6)		XL.SetActiveSheet("Allow_Shear");

		m_richeditAllowStress.InsetExcelObjectFromFile(&XL);
		m_richeditAllowStress.LineScroll(-m_richeditAllowStress.GetFirstVisibleLine());

		XL.QuitXL();
		RedrawWindow();
	}
}

void CDesignBaseDlg::SetDataInitDesignBase()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CGlobarOption *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	m_Grid.SetTextMatrix(1,1, pOpt->GetStrDesignBridgeType());
	m_Grid.SetTextMatrix(2,1,pData->DESIGN_BASE.m_szBridgeLength);
	m_Grid.SetTextMatrix(3,1,pData->DESIGN_BASE.m_szBridgeLineType);
	m_Grid.SetTextMatrix(4,1,"%.3f",pData->DESIGN_BASE.m_dBridgeWidth);
	m_Grid.SetTextMatrix(5,1,"%g",pData->DESIGN_BASE.m_dBridgeSkew);
}

void CDesignBaseDlg::SetDataInitDesignCondition()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	m_Grid.SetTextMatrix(1,3,pData->DESIGN_CONDITION.m_szBridgeGrade);
	m_Grid.SetTextMatrix(2,3,"%g", tokN(pData->DESIGN_CONDITION.m_dPf));
	m_Grid.SetTextMatrix(3,3,"%g", tokN(pData->DESIGN_CONDITION.m_dPr));
	m_Grid.SetTextMatrix(4,3,"%g", tokNPM(pData->DESIGN_CONDITION.m_dW));
	m_Grid.SetTextMatrix(5,3,"%g", tokN(pData->DESIGN_CONDITION.m_dPm));
	m_Grid.SetTextMatrix(6,3,"%g", tokN(pData->DESIGN_CONDITION.m_dPs));
}

void CDesignBaseDlg::SetDataInitDesignMaterial()
{	
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();	
	long n = 1;

	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dSigmaCK);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dSigmaCi);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dSigmaY);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dSigmaSA);
	m_Grid.SetTextMatrix(n++,2,pData->DESIGN_MATERIAL.m_szSwsJu);
	m_Grid.SetTextMatrix(n++,2,pData->DESIGN_MATERIAL.m_szSwsBu);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dEc);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dEs);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dShearEs);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dEst);
	m_Grid.SetTextMatrix(n++,2,"%g",(double)pData->DESIGN_MATERIAL.m_nN);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dEpsilonS);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dAlpha);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dCreep);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dCreep2);
	m_Grid.SetTextMatrix(n++,2,"%g",pData->DESIGN_MATERIAL.m_dTemparature);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignBaseDlg::SetDataInitDesignUnitWeight()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	m_Grid.SetTextMatrix(1,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dGangjae));
	m_Grid.SetTextMatrix(2,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dAluminum));
	m_Grid.SetTextMatrix(3,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dChulgun));
	m_Grid.SetTextMatrix(4,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dGravel));
	m_Grid.SetTextMatrix(5,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dCement));
	m_Grid.SetTextMatrix(6,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dMokjae));
	m_Grid.SetTextMatrix(7,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dYukchung));
	m_Grid.SetTextMatrix(8,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dAsphalt));
	m_Grid.SetTextMatrix(9,1,"%g",tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dMugun));
}

void CDesignBaseDlg::SetDataInitDesignRef()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	for(long n=0; n<pData->m_szRefArray.GetSize(); n++)
	{
		m_Grid.SetTextMatrix(n+1,0,"%.0f",(double)(n+1));
		m_Grid.SetTextMatrix(n+1,1,pData->m_szRefArray[n]);
	}
}

void CDesignBaseDlg::SetDataInit()
{
	SetDataDefault();

	SetShow();		
	SetGridTitle();
	SetDataDefaultBridgeJewon();//���������� ������ �Ǵ��� �ڵ����� ����Ǿ���.

	long nSel = m_ctlTab.GetCurSel();
	if(nSel==0) SetDataInitDesignBase();
	else if(nSel==1) SetDataInitDesignCondition();
	else if(nSel==2) SetDataInitDesignMaterial();
	else if(nSel==3) SetDataInitDesignUnitWeight();
	else if(nSel==4) SetDataInitDesignRef();
	else if(nSel==5) SetDataInitPrestress();
}

void CDesignBaseDlg::SetDataInitPrestress()
{
	CDataManage	*pDataManage	= m_pStd->GetDataManage();
	CCalcData	*pCalcData		= m_pStd->GetDataManage()->GetCalcData();

	long nPrestressIdx	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData	*pSteelPSData = pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);
	if(!pSteelPSData) return;

	m_Grid.SetTextMatrix(1,		1,			pCalcData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName);
	m_Grid.SetTextMatrix(2,		1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea);
	m_Grid.SetTextMatrix(3,		1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter);
//	m_Grid.SetTextMatrix(4,		1,	"%g",	(double)pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty);
//	m_Grid.SetTextMatrix(5,		1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC);
	m_Grid.SetTextMatrix(4,		1,			GetStrDuctType(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType));
	m_Grid.SetTextMatrix(5,		1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter);
	m_Grid.SetTextMatrix(6,		1,	"%.4f",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction);
	m_Grid.SetTextMatrix(7,		1,	"%.2f",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction);
	m_Grid.SetTextMatrix(8,		1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	//m_Grid.SetTextMatrix(9,		1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEndAnchorageSlip);
	m_Grid.SetTextMatrix(9,	1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs);
	m_Grid.SetTextMatrix(10,	1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength);
	m_Grid.SetTextMatrix(11,	1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength);
	m_Grid.SetTextMatrix(12,	1,	"%g",	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity);
}

CString CDesignBaseDlg::GetStrDuctType(long nType)
{
	if(nType==0) return _T("�ݼӽ���");
	if(nType==1) return _T("�ƿ����� �ݼӽ���");
	if(nType==2) return _T("�ƽ���Ʈ �Ǵ� �׸���������");
	if(nType==3) return _T("�ƿ����ݵ� ������Ʈ");
	return _T("");
}

void CDesignBaseDlg::SetDataDefaultBridgeJewon()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CPlateGirderApp *pGirCR= pBridge->GetGirder(-1);	//���� �߽� �Ŵ�
	CPlateBxFinder Finder(pGirCR);

	double dJigan = 0.0;
	CString str, str1;

	// ��������
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	if(pBridge->GetNumHaiseokCombo() == 0)
		pData->DESIGN_BASE.m_szBridgeType = "�ռ��� ���� ����";
	else
		pData->DESIGN_BASE.m_szBridgeType = "���ռ��� ���� ����";

	for(long n = 0;n < pBridge->m_nQtyJigan;n++)
	{
		dJigan = pBridge->m_dLengthJigan[n];
		if(n == 0)	dJigan += pBridge->m_dLengthUganStt+pBridge->m_dLengthSlabGirderStt+pBridge->m_dLengthGirderShoeStt;
		if(n ==	pBridge->m_nQtyJigan-1)	dJigan += pBridge->m_dLengthUganEnd+pBridge->m_dLengthSlabGirderEnd+pBridge->m_dLengthGirderShoeEnd;
		str1.Format("%.3f", toM(dJigan));
		str += str1;
		if( n != pBridge->m_nQtyJigan-1 ) str += " + ";
	}

	pData->DESIGN_BASE.m_szBridgeLength.Format("%s = %.3f",str, toM(pBridge->GetLengthBridge()));
	pData->DESIGN_BASE.m_dBridgeWidth = toM(pBridge->GetWidthSlabDom(pBx));
//	pData->DESIGN_BASE.m_szBridgeGirderJewon = pBridge->GetGirder(0)->GetJewonHBeam(G_H, NULL);
	pData->DESIGN_BASE.m_dBridgeSkew = pBridge->m_vAngJijum[0].GetAngleDegree();
	pData->DESIGN_BASE.m_szBridgeLineType = GetLineInfo();

}

void CDesignBaseDlg::SetDataDefaultDesignCondition()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CSteelGrade *pGrade = m_pStd->GetDataManage()->GetSteelGrade();
	long nCombo = atol(pBridge->m_sDeungeub)-1;
	if(nCombo< 0 || nCombo>2) nCombo= 0;
	pData->DESIGN_CONDITION.m_dPf		= pGrade->GetDBPf(nCombo);
	pData->DESIGN_CONDITION.m_dPr		= pGrade->GetDBPr(nCombo);
	pData->DESIGN_CONDITION.m_dW		= pGrade->GetDL(nCombo);	
	pData->DESIGN_CONDITION.m_dPm		= pGrade->GetDLPm(nCombo);
	pData->DESIGN_CONDITION.m_dPs		= pGrade->GetDLPs(nCombo);
	pData->DESIGN_CONDITION.m_szBridgeGrade = pBridge->m_sDeungeub;	
}

void CDesignBaseDlg::SetDataDefaultDesignRef()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CGlobarOption *pOpt = m_pStd->GetDataManage()->GetGlobalOption();

	pData->m_szRefArray.RemoveAll();

	long nStandardYear = pOpt->GetSteelStandardYear();

	if(nStandardYear == APPLYYEAR2010)
	{
		pData->m_szRefArray.Add("���α� �������(2010)");
		pData->m_szRefArray.Add("���α� ������� �ؼ�(2008)");
		pData->m_szRefArray.Add("��ũ��Ʈ ���� �������(2007)");
	}
	else // 2008
	{
		pData->m_szRefArray.Add("���α� �������(2005)");
		pData->m_szRefArray.Add("���α� ������� �ؼ�(2008)");
		pData->m_szRefArray.Add("��ũ��Ʈ ���� �������(2003)");
	}
	pData->m_szRefArray.Add("���μ��� ���(2008)");
	pData->m_szRefArray.Add("���μ��� ���(2001)");
	pData->m_szRefArray.Add("������ ���(1995)");
	pData->m_szRefArray.Add("�����α� �󼼺� ������ħ(2006)");
}

void CDesignBaseDlg::SetDataDefaultDesignUnitWeight()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	CSteelUWeight *pUWeight = m_pStd->GetDataManage()->GetSteelUWeight();

	pData->DESIGN_UNIT_WEIGHT.m_dGangjae	= pUWeight->m_dUWeightGangjae;//->GetUnitWeight(0);
	pData->DESIGN_UNIT_WEIGHT.m_dAluminum	= pUWeight->m_dUWeightAluminum;
	pData->DESIGN_UNIT_WEIGHT.m_dAsphalt	= pUWeight->m_dUWeightAsphalt;
	pData->DESIGN_UNIT_WEIGHT.m_dCement		= pUWeight->m_dUWeightCement;
	pData->DESIGN_UNIT_WEIGHT.m_dChulgun	= pUWeight->m_dUWeightChulgunCon;
//	pData->DESIGN_UNIT_WEIGHT.m_dChulgun	= pUWeight->m_m_dUWeightJochul;
	pData->DESIGN_UNIT_WEIGHT.m_dMokjae		= pUWeight->m_dUWeightMokjae;
	pData->DESIGN_UNIT_WEIGHT.m_dMugun		= pUWeight->m_dUWeightMugunCon;
	pData->DESIGN_UNIT_WEIGHT.m_dYukchung	= pUWeight->m_dUWeightYukchung;
}

void CDesignBaseDlg::SetDataDefault()
{			

	SetDataDefaultBridgeJewon();
	SetDataDefaultDesignCondition();
	SetDataDefaultDesignRef();
	SetDataDefaultDesignUnitWeight();

}

void CDesignBaseDlg::SetDataSave()
{
}

void CDesignBaseDlg::SetGridTitleDesignBase()
{
	long nRows = 6;
	long nCols = 3;
	
	m_Grid.ResetAttrib();
	m_Grid.SetEditable(FALSE);	
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
		
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 200);
	m_Grid.SetColumnWidth(2, 50);

	CString str;
	m_Grid.SetTextMatrix(0,0,"�� ��");
	m_Grid.SetTextMatrix(0,1,"�� ��");
	m_Grid.SetTextMatrix(0,2,"�� ��");

	char *szTitle[] = { "��������", "��������", "��鼱��", "��������", "�簢" };
	char *szUnit[] = { "", "m", "", "m", "��" };
	for(long n=0; n<5; n++)
	{
		m_Grid.SetTextMatrix(n+1,0,szTitle[n]);
		m_Grid.SetTextMatrix(n+1,2,szUnit[n]);		 
		//m_Grid.SetItemState(n+1,2, m_Grid.GetItemState(n+1,2) | GVIS_READONLY); 
	}
	for(long nRow=1; nRow<nRows; nRow++)
	{
		//m_Grid.SetItemBkColour(nRow,1,RGBREADONLY);
		//m_Grid.SetItemBkColour(nRow,2,RGBREADONLY);
		m_Grid.SetItemState(nRow, 1, GVIS_READONLY);
		m_Grid.SetItemState(nRow, 2, GVIS_READONLY);
	}
}

void CDesignBaseDlg::SetGridTitleDesignCondition()
{
	long nRows = 7;
	long nCols = 5;
	
	m_Grid.ResetAttrib();
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
		
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(3);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	m_Grid.SetColumnWidthAll(100);	
	m_Grid.SetColumnWidth(3, 250);
	m_Grid.SetColumnWidth(4, 50);

	CString str;
	m_Grid.SetTextMatrix(0,0,"�� ��");
	m_Grid.SetTextMatrix(0,3,"�� ��");
	m_Grid.SetTextMatrix(0,4,"�� ��");
	m_Grid.SetMergeCol(0,0,2);

	m_Grid.SetTextMatrix(1,0,"�������");	
	m_Grid.SetMergeCol(1,0,2);
	m_Grid.SetTextMatrix(2,0,"��������");
	m_Grid.SetMergeRow(2,6,0);
	m_Grid.SetTextMatrix(2,1,"DB ����");
	m_Grid.SetMergeRow(2,3,1);
	m_Grid.SetTextMatrix(4,1,"DL ����");
	m_Grid.SetMergeRow(4,6,1);


	char *szTitle[] = { "Pf", "Pr", "W", "Pm", "Ps" };
	char *szUnit[] = { "kN", "kN", "kN/m", "kN", "kN" };
	for(long n=0; n<5; n++)
	{
		m_Grid.SetTextMatrix(n+2,2,szTitle[n]);
		m_Grid.SetTextMatrix(n+2,4,szUnit[n]);		
		//m_Grid.SetItemState(n+2,4, m_Grid.GetItemState(n+1,4) | GVIS_READONLY); 
	}

	for(long nRow=1; nRow<nRows; nRow++)
	{
		//m_Grid.SetItemBkColour(nRow,3,RGBREADONLY);
		//m_Grid.SetItemBkColour(nRow,4,RGBREADONLY);
		m_Grid.SetItemState(nRow, 3, GVIS_READONLY);
		m_Grid.SetItemState(nRow, 4, GVIS_READONLY);
	}
}

void CDesignBaseDlg::SetGridTitleDesignMaterial()
{
	long nRows = 17;
	long nCols = 4;
	
	m_Grid.ResetAttrib();
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(2);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 220);
	m_Grid.SetColumnWidth(2, 150);
	m_Grid.SetColumnWidth(3, 50);

	CString str;
	m_Grid.SetTextMatrix(0,0,"�� ��");
	m_Grid.SetTextMatrix(0,2,"�� ��");
	m_Grid.SetTextMatrix(0,3,"�� ��");
	m_Grid.SetMergeCol(0,0,1);

	m_Grid.SetTextMatrix(1,0,"��ũ��Ʈ");
	m_Grid.SetMergeRow(1,2,0);

	m_Grid.SetTextMatrix(3,0,"ö��");
	m_Grid.SetMergeRow(3,4,0);	
	m_Grid.SetTextMatrix(5,0,"����");
	m_Grid.SetMergeRow(5,6,0);	
	m_Grid.SetTextMatrix(7,0,"ź�����");
	m_Grid.SetMergeRow(7,11,0);	
	m_Grid.SetTextMatrix(12,0,"����� ������ Ư��");
	m_Grid.SetMergeRow(12,16,0);	
	
	char *szTitle[] = { "Fck", "Fci", "Fy", "Fsa", "�ֺ���", "�κ���", "��ũ��Ʈ Ec", "ö�� Es", "����ź����� G", "���� Est", "ź������� n",
						"���������� ��s", "�� ��â��� ��", "�ٴ��� ��ũ��Ʈ�� ũ���� ��1", "�ٴ��� ��ũ��Ʈ�� �������� ��2",
						"�ٴ��� ��ũ��Ʈ�� �������� �µ���"};
	char *szUnit[] = {	"MPa", "MPa", "MPa", "MPa", "", "", "MPa", "MPa", "MPa", "MPa", "", "", "", "", "", "��"};
	for(long n=0; n<16; n++)
	{
		m_Grid.SetTextMatrix(n+1,1,szTitle[n]);
		m_Grid.SetTextMatrix(n+1,3,szUnit[n]);		
		//m_Grid.SetItemState(n+1,3, m_Grid.GetItemState(n+1,3) | GVIS_READONLY); 
	}

	for(long nRow=1; nRow<nRows; nRow++)
	{
		//m_Grid.SetItemBkColour(nRow,2,RGBREADONLY);		
		m_Grid.SetItemState(nRow, 2, GVIS_READONLY);
		//m_Grid.SetItemBkColour(nRow,3,RGBREADONLY);		
		m_Grid.SetItemState(nRow, 3, GVIS_READONLY);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignBaseDlg::SetGridTitleDesignUnitWeight()
{
	long nRows = 10;
	long nCols = 3;
	
	m_Grid.ResetAttrib();
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
		
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 200);
	m_Grid.SetColumnWidth(2, 50);

	CString str;
	m_Grid.SetTextMatrix(0,0,"��    ��");
	m_Grid.SetTextMatrix(0,1,"�����߷�");
	m_Grid.SetTextMatrix(0,2,"��    ��");	
	

	char *szTitle[] = { "�� ��", "�˹̴�", "ö����ũ��Ʈ", "�ڰ�.�⼮", "�ø�Ʈ ����", 
		                "����", "��û��(�����)", "����", "���� ��ũ��Ʈ"};
	char *szUnit[] = {	"kN/��","kN/��","kN/��","kN/��","kN/��","kN/��","kN/��"
						,"kN/��","kN/��"};
	for(long n=0; n<9; n++)
	{
		m_Grid.SetTextMatrix(n+1,0,szTitle[n]);
		m_Grid.SetTextMatrix(n+1,2,szUnit[n]);
		//m_Grid.SetItemState(n+1,2, m_Grid.GetItemState(n+1,2) | GVIS_READONLY); 
	}

	for(long nRow=1; nRow<nRows; nRow++)
	{
		//m_Grid.SetItemBkColour(nRow,1,RGBREADONLY);
		//m_Grid.SetItemBkColour(nRow,2,RGBREADONLY);
		m_Grid.SetItemState(nRow, 1, GVIS_READONLY);
		m_Grid.SetItemState(nRow, 2, GVIS_READONLY);
	}
}

void CDesignBaseDlg::SetGridTitleDesignRef()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();	

	long nRows = pData->m_szRefArray.GetSize()+1;
	long nCols = 2;
	
	m_Grid.ResetAttrib();
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);		
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 400);	
		
	m_Grid.SetTextMatrix(0,0,"�� ȣ");
	m_Grid.SetTextMatrix(0,1,"���� �ڷ�");	
	
	for(long nRow=1; nRow<nRows; nRow++)
	{
		//m_Grid.SetItemBkColour(nRow,1,RGBREADONLY);
		m_Grid.SetItemState(nRow, 1, GVIS_READONLY);
		
	}
}

void CDesignBaseDlg::SetGridTitleDesignPrestress()
{
//	long nRows = 16;	
	long nRows = 13;
	long nCols = 3;
	m_Grid.ResetAttrib();
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);		
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 170);
	m_Grid.SetColumnWidth(1, 200);
	m_Grid.SetColumnWidth(2, 100);

//	char *szTitle[] = { "PS���� ȣĪ", "PS���� ��Ī�ܸ���", "PS���� ����", "PS���� ����", "PS���� ����",
//							"��Ʈ�� ����", "��Ʈ ����", "�Ļ� ���� ���", "��� ���� ���", "������ Ȱ����(������)", "������ Ȱ����(������)",
//							"ź����� Es", "���Ѱ���", "�׺�����", "��ջ�����"};
//	char *szUnit[] = { "", "mm��", "mm", "", "mm", "", "mm", "1/m", "", "mm", "mm", "MPa", "MPa", "MPa", "%"};

	char *szTitle[] = { "PS���� ȣĪ", "PS���� ��Ī�ܸ���", "PS���� ����", "��Ʈ�� ����", "��Ʈ ����", 
						"�Ļ� ���� ���", "��� ���� ���", "������ Ȱ����(������)", "������ Ȱ����(������)",
						"ź����� Es", "���Ѱ���", "�׺�����", "��ջ�����"};
	char *szUnit[] = { "", "mm��", "mm", "", "mm", "1/m", "1/m", "", "mm", "mm", "MPa", "MPa", "MPa", "%"};

	for(long n=0; n<nRows; n++)
	{
		m_Grid.SetTextMatrix(n+1, 0, szTitle[n]);
		m_Grid.SetTextMatrix(n+1, 2, szUnit[n]);
		//m_Grid.SetItemState(n, 2, m_Grid.GetItemState(n,2) | GVIS_READONLY);
	}
	for(long nRow=1; nRow<nRows; nRow++)
	{
		m_Grid.SetItemState(nRow, 1, GVIS_READONLY);
		m_Grid.SetItemState(nRow, 2, GVIS_READONLY);
		//m_Grid.SetItemBkColour(nRow,1,RGBREADONLY);
		//m_Grid.SetItemBkColour(nRow,2,RGBREADONLY);
	}

	m_Grid.SetTextMatrix(0, 0, "�� ��");
	m_Grid.SetTextMatrix(0, 1, "�� �� ��");
	m_Grid.SetTextMatrix(0, 2, "�� ��");
	m_Grid.SetRowHeight(7, 0);
}

void CDesignBaseDlg::SetGridTitle()
{
	long nSel = m_ctlTab.GetCurSel();			
	if(nSel==0) SetGridTitleDesignBase();
	else if(nSel==1) SetGridTitleDesignCondition();	
	else if(nSel==2) SetGridTitleDesignMaterial();
	else if(nSel==3) SetGridTitleDesignUnitWeight();
	else if(nSel==4) SetGridTitleDesignRef();
	else if(nSel==5) SetGridTitleDesignPrestress();
}

void CDesignBaseDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDesignBaseDlg::OnCursorChange(long nRow, long nCol)
{
}

void CDesignBaseDlg::OnPreInitDialog()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	m_ctlTab.InsertItem(0, "���� ����");
	m_ctlTab.InsertItem(1, "���� ����");
	m_ctlTab.InsertItem(2, "����� �������");
	m_ctlTab.InsertItem(3, "����� �����߷�");
//	m_ctlTab.InsertItem(4, "��� ��������");
//	m_ctlTab.InsertItem(5, "��� ��������");
//	m_ctlTab.InsertItem(6, "��� ��������");
	m_ctlTab.InsertItem(4, "������");	
	if(!pBridge->IsPlateGir())
		m_ctlTab.InsertItem(5, "PS ����");

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_DESIGN, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_LIST_ALLOWSTRESS,SZ_TOP_LEFT, SZ_TOP_RIGHT);
}

void CDesignBaseDlg::OnSelchangeTabDesign(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	m_ctlTab.SetFocus();
	SetDataInit();
}

int CDesignBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGeneralBaseDlg::OnCreate(lpCreateStruct) == -1)
		return -1;		
	
	return 0;
}

CString CDesignBaseDlg::GetLineInfo()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir= pBridge->GetGirder(-1);	//���� �߽� �Ŵ�
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_LINE_P);
	CLineInfo	*pLine = pGir->GetLine();

	CString sLineInfo, sRadius, sLeft, sRadius2, sLeft2, sLineType;

	double staStt = pBxStt->GetStation();		//�������� station.
	double staEnd = pBxEnd->GetStation();		//�������� station.

	if(pBx!=NULL)
	{
		double sta  = pBx->GetStation();

		sLineInfo	= pLine->GetStringPoint(sta);

		if(sLineInfo=="BTC")
		{
			pBx = Finder.GetBxNext();	//BC

			if(pBx==NULL)
			{
				if(sta>staStt)
					sLineType="���� + Ŭ�μ��̵�";
				else
					sLineType="Ŭ�μ��̵�";
			}
			else
			{

				sta = pBx->GetStation();	//BC station

				sRadius = pGir->GetRadiusString(staStt,0);
				sLeft	= sRadius.Left(1);
				sRadius2 = pGir->GetRadiusString(staEnd,0);
				sLeft2	= sRadius2.Left(1);

				if(sta>staStt)
				{
					if(sLeft2=="")
						sLineType="���� + Ŭ�μ��̵� + ��� + Ŭ�μ��̵� + ����";
					else if(sLeft2=="A")
						sLineType="���� + Ŭ�μ��̵� + ��� + Ŭ�μ��̵�";
					else if(sLeft2=="R")
						sLineType="���� + Ŭ�μ��̵� + ���";
				}
				else
				{
					if(sLeft2=="")
						sLineType="Ŭ�μ��̵� + ��� + Ŭ�μ��̵� + ����";
					else if(sLeft2=="A")
						sLineType="Ŭ�μ��̵� + ��� + Ŭ�μ��̵�";
					else if(sLeft2=="R")
						sLineType="Ŭ�μ��̵� + ���";
				}
			}
		}
		else if(sLineInfo=="BC")
		{
			pBx = Finder.GetBxNext();	//EC

			sRadius = pGir->GetRadiusString(staStt,0);
			sLeft	= sRadius.Left(1);
			sRadius2 = pGir->GetRadiusString(staEnd,0);
			sLeft2	= sRadius2.Left(1);

			if(pBx==NULL)
			{
				if(sta>staStt)
				{
					if(sLeft=="A")
						sLineType="Ŭ�μ��̵� + ���";
					else
						sLineType="���� + ���";
				}
				else
						sLineType="���";
			}
			else
			{
				sta = pBx->GetStation();	//EC station

				if(sta>staStt)
				{
					if(staEnd>sta)
					{
						if(sLeft=="A" && sLeft2=="A")		//Stt:Clothoid    End:Clothoid
						{
							if(staEnd>sta)
								sLineType="Ŭ�μ��̵� + ��� + Ŭ�μ��̵�";
							else
								sLineType="Ŭ�μ��̵� + ���";
						}
						else if(sLeft=="A" && sLeft2=="")		//Stt:Clothoid    End:Line
						{
							pBx = Finder.GetBxNext();

							if(pBx != NULL)	//ETC�� ������
							{
								sta = pBx->GetStation();	//ETC station
								if(staEnd > sta)
									sLineType="Ŭ�μ��̵� + ��� + Ŭ�μ��̵� + ����";
								else
									sLineType="Ŭ�μ��̵� + ��� + Ŭ�μ��̵�";
							}
							else
							{
								if(staEnd > sta)
									sLineType = "Ŭ�μ��̵� + ��� + ����";
								else
									sLineType = "Ŭ�μ��̵� + ���";
							}
						}
						else if(sLeft==""&&sLeft2=="A")		//Stt:Line    End:Clothoid
						{
							if(staEnd>sta)
								sLineType="���� + ��� + Ŭ�μ��̵�";
							else
								sLineType="���� + ���";
						}
						else		//Stt:Line    End:Line
							sLineType="���� + ��� + ����";
					}
					else
					{
						if(sLeft=="A")
							sLineType="Ŭ�μ��̵� + ���";
						else
							sLineType="���� + ���";
					}
				}
				else
				{
					if(staEnd>sta)
					{
						if(sLeft=="A"&&sLeft2=="A")
						{
							if(staEnd>sta)
								sLineType="��� + Ŭ�μ��̵�";
							else
								sLineType="���";
						}
						else if(sLeft=="A"&&sLeft2=="")
						{
							pBx = Finder.GetBxNext();
							sta = pBx->GetStation();	//ETC station

							if(pBx!=NULL)	//ETC�� ������
							{
								if(staEnd>sta)
									sLineType="��� + Ŭ�μ��̵� + ����";
								else
									sLineType="��� + Ŭ�μ��̵�";
							}
							else
							{
								if(staEnd>sta)
									sLineType="��� + ����";
								else
									sLineType="���";
							}
						}
						else if(sLeft==""&&sLeft2=="A")
						{
							if(staEnd>sta)
								sLineType="��� + Ŭ�μ��̵�";
							else
								sLineType="���";
						}
						else
							sLineType="��� + ����";
					}
					else
						sLineType="���";
				}
			}
		}
		else if(sLineInfo=="EC")
		{
			pBx = Finder.GetBxNext();	//ETC

			if(sta>staStt)
			{
				if(pBx!=NULL)	//ETC�� ������
				{
					sta = pBx->GetStation();	//ETC station

					if(staEnd>sta)
						sLineType="��� + Ŭ�μ��̵� + ����";
					else
						sLineType="��� + Ŭ�μ��̵�";
				}
				else	//ETC�� ������
				{
					if(staEnd>sta)
						sLineType="��� + ����";
					else
						sLineType="���";
				}
			}
			else
			{
				if(pBx!=NULL)	//ETC�� ������
				{
					sta = pBx->GetStation();	//ETC station

					if(staEnd>sta)
						sLineType="Ŭ�μ��̵� + ����";
					else
						sLineType="Ŭ�μ��̵�";
				}
				else	//ETC�� ������
					sLineType="����";
			}
		}
		else if(sLineInfo=="ETC")
		{
			if(sta>staStt)
			{
				if(staEnd>sta)
					sLineType="Ŭ�μ��̵� + ����";
				else
					sLineType="Ŭ�μ��̵�";
			}
			else
				sLineType="����";
		}
	}
	else
	{
		double staStt  = pBxStt->GetStation();

		sRadius = pGir->GetRadiusString(staStt,0);
		sLeft	= sRadius.Left(1);

		if(sRadius=="")
			sLineType = "����";
		else
		{
			if(sLeft=="R")
				sLineType = "���";
			else if(sLeft=="A")
				sLineType = "Ŭ�μ��̵�";
		}
	}

	return sLineType;

	return "";
}
