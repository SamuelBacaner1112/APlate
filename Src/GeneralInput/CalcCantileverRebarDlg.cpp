// CalcCantileverRebarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Generalinput.h"
#include "CalcCantileverRebarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverRebarDlg dialog


CCalcCantileverRebarDlg::CCalcCantileverRebarDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcCantileverRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverRebarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCalcCantileverRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverRebarDlg)
	DDX_Control(pDX, IDC_TAB_REBAR, m_TabRebar);
	DDX_Control(pDX, IDC_RICHEDIT_REBAR, m_richeditRebar);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_REBAR_COMBO, m_comboRebar);
}


BEGIN_MESSAGE_MAP(CCalcCantileverRebarDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverRebarDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_REBAR, OnSelchangeTabRebar)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_CBN_SELCHANGE(IDC_REBAR_COMBO, &CCalcCantileverRebarDlg::OnSelchangeRebarCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverRebarDlg message handlers

void CCalcCantileverRebarDlg::OnPreInitDialog()
{
 	SetResize(IDC_RICHEDIT_REBAR,		SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_TAB_REBAR,			SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_STATIC_REBAR_TITLE,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_GRID,					SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_REBAR_COMBO,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_REBAR_STATIC,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_REBAR_GROUPBOX,	SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);


	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());
	CGlobarOption	*pOpt	= m_pStd->GetDataManage()->GetGlobalOption();
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();

	m_sRebar = pData->DESIGN_MATERIAL.m_sSigmaY;
	
	long nTab	= 0;
	m_TabRebar.InsertItem(nTab++, "���� ĵƿ������");
	m_TabRebar.InsertItem(nTab++, "���� ĵƿ������");
	if(pOpt->GetSlabCentDesignMethod() == 0 || pOpt->GetSlabCentDesignMethod() == 2)
	{
		// ���������ϰ��
		m_TabRebar.InsertItem(nTab++, "�߾Ӻ� �ٴ���(���������)");
	}
	else
	{
		// ������ �����ϰ��� ���� ����
		if(!CalcGeneral.CheckExperienceDesign(TRUE) || !CalcGeneral.CheckExperienceDesign(FALSE))
			m_TabRebar.InsertItem(nTab++, "�߾Ӻ� �ٴ���(���������)");
		m_TabRebar.InsertItem(nTab++, "�߾Ӻ� �ٴ���(����������)");
	}
	if(pDB->m_nQtyJigan > 1)
		m_TabRebar.InsertItem(nTab++, "�߰�������");
	m_TabRebar.SetCurSel(0);

	CString strList = _T("");
	for(long ix = 0; ix < 2; ix++)
	{
		strList.Format(_T("%d ��"), ix+1);
		m_comboRebar.AddString(strList);
	}

	SetTextFont();
	m_richeditRebar.SetWordCharFormat(m_cfDefault);	

	reCalculate();

	SetGridTitle();
}

//////////////////////////////////////////////
// From : CColorizerDemoDlg
DWORD CALLBACK readFunction(DWORD dwCookie,
	LPBYTE lpBuf,	//the buffer to fill
	LONG nCount,	//the no. of bytes to read
	LONG* nRead) // no. of bytes read
{
	CFile* fp = (CFile *)dwCookie;
	*nRead = fp->Read(lpBuf,nCount);
	return 0;
}

void CCalcCantileverRebarDlg::reCalculate()
{
	CDataManage		*pDataManage	= m_pStd->GetDataManage();
	CCalcFloor		*pCalcFloor		= m_pStd->m_pCalcStd->m_pCalcFloor;
	CPlateStressOut	StressOut(m_pStd);
	
	// Ȱ���� ���
	pCalcFloor->SetLiveLoad(FLOOR_LEFT);
	pCalcFloor->SetLiveLoad(FLOOR_RIGHT);
	pCalcFloor->SetLiveLoadCenter();
	// ���� �浹���� ���
	pCalcFloor->SetCollisionLoad(FLOOR_LEFT);
	pCalcFloor->SetCollisionLoad(FLOOR_RIGHT);
	// ǳ���� ���
	pCalcFloor->SetWindLoad(FLOOR_LEFT);
	pCalcFloor->SetWindLoad(FLOOR_RIGHT);
	//		pCalcFloor->SetWindLoad(FLOOR_MID);
	// �������� ���
	pCalcFloor->SetCentriodLoad(FLOOR_LEFT);
	pCalcFloor->SetCentriodLoad(FLOOR_RIGHT);
	pCalcFloor->SetCentriodLoad(FLOOR_MID);
	// ��������
	pCalcFloor->SetLoadComposite(FLOOR_LEFT);
	pCalcFloor->SetLoadComposite(FLOOR_RIGHT);
	pCalcFloor->SetLoadComposite(FLOOR_MID);
}

CString CCalcCantileverRebarDlg::GetCheck(BOOL bCheck)
{
	if(bCheck)	return _T("O.K");
	else		return _T("N.G");

	return _T("");
}

void CCalcCantileverRebarDlg::SetDataInit()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CCalcData		*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor		*pCalcFloor	= m_pStd->m_pCalcStd->m_pCalcFloor;
	CGlobarOption	*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());
	CPlateStressOut	StressOut(m_pStd);

	CString	sText		= _T("");
	long	nQtyJigan	= pDB->m_nQtyJigan;
	long	nRow		= 2;
	long	nMethod		= pOpt->GetSlabCentDesignMethod();
	long	nFloorIdx	= 0;
	CString	szText		= GetTitle();

	m_richeditRebar.SetSel(0, -1);
	m_richeditRebar.ReplaceSel("");
	m_richeditRebar.SetReadOnly(TRUE);

	if(szText.Find("��")!=-1)		nFloorIdx = FLOOR_LEFT;
	else if(szText.Find("��")!=-1)	nFloorIdx = FLOOR_RIGHT;
	else							nFloorIdx = FLOOR_MID;


	if(szText=="�߾Ӻ� �ٴ���(����������)")
		pCalcFloor->SetCantileverRebar(nFloorIdx, FLOOR_EXPERIENCE_DESIGN);
	else
		pCalcFloor->SetCantileverRebar(nFloorIdx, FLOOR_STRENGTH_DESIGN);


	// ĵƿ������ �¿���
	for(long nIdx=0; nIdx<2; nIdx++)
	{
		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc1);					// �Ϲݺ� ��ö�� C.T.C 
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo1));		// �Ϲݺ� ��ö�� ���� 1-Cycle
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo1_2Cy));	// �Ϲݺ� ��ö�� ���� 2-Cycle
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_bCheckGen1));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB);					// �Ϲݺ� ���ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo3));		// �Ϲݺ� ���ö�� ����
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_bCheckGen2));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc2);					// �ܺ� ��ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2));		// �ܺ� ��ö�� ���� 1-Cycle
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2_2Cy));	// �ܺ� ��ö�� ���� 2-Cycle
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_bCheckDan1));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB1);				// �ܺ� ���ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo4));		// �ܺ� ���ö�� ����
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		m_Grid.SetTextMatrix(nRow,		5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_bCheckDan2));
		nRow+=3;
	}

	if(nMethod==1)	//������ �����
	{
		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1);					// �Ϲݺ� ��� ��ö�� C.T.C 
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy));	// �Ϲݺ� ��� ��ö�� ���� 1-Cycle
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy));	// �Ϲݺ� ��� ��ö�� ���� 2-Cycle
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck((BOOL)pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheck_Up));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1);					// �Ϲݺ� ��ö�� �ϸ� C.T.C 
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1));		// �Ϲݺ� ��ö�� �ϸ� ���� 1-Cycle
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy));	// �Ϲݺ� ��ö�� �ϸ� ���� 2-Cycle
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen1));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2);					// �Ϲݺ� ��� ���ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2));		// �Ϲݺ� ��� ���ö�� ����
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck((BOOL)pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheck_UpB));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB);					// �Ϲݺ� �ϸ� ���ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3));		// �Ϲݺ� �ϸ� ���ö�� ����
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen2));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2);					// �ܺ� ��ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2));		// �ܺ� ��ö�� ���� 1-Cycle
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy));	// �ܺ� ��ö�� ���� 2-Cycle
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan1));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1);					// �ܺ� ���ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4));		// �ܺ� ���ö�� ����
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		m_Grid.SetTextMatrix(nRow,		5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan2));
	
	}
	else
	{
		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1);					// �Ϲݺ� ��ö�� C.T.C 
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1));		// �Ϲݺ� ��ö�� ���� 1-Cycle
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy));	// �Ϲݺ� ��ö�� ���� 2-Cycle
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen1));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB);					// �Ϲݺ� ���ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3));		// �Ϲݺ� ���ö�� ����
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen2));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2);					// �ܺ� ��ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2));		// �ܺ� ��ö�� ���� 1-Cycle
		m_Grid.SetTextMatrix(nRow,		4,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy));	// �ܺ� ��ö�� ���� 2-Cycle
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan1));

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1);					// �ܺ� ���ö�� C.T.C
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4));		// �ܺ� ���ö�� ����
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		
		m_Grid.SetTextMatrix(nRow,		5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan2));
		
	}

	nRow+=3;

	CCalcFloor			CalcFloor(m_pStd->GetDataManage());
	
	CMap <CString, LPCTSTR, double, double> Map;
	
	CalcFloor.CalcCantileverInnerPositionCheck(Map);
	


	nRow = 20;
	long nAddRow = 0;
	if(nMethod == 2)
	{
		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->m_dLBDeckCTC);
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->m_nLBDeckDiaIdx));
		m_Grid.SetTextMatrix(nRow,		4,	"%.0f",	pCalcData->m_dLBDeckCover);
		m_Grid.SetTextMatrix(nRow,		5,			GetCheck(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen1));
		nAddRow = 3;
	}

	double	dUr		 =	Map["Ur"];
	double	dBar_Area =	Map["Bar_Area"];
	double	dReq_As	 =	Map["Req_As"];
	BOOL	bOK = FALSE;

	if (dUr>CalcFloor.GetFloorJujangStd() && dBar_Area>dReq_As)
		bOK = TRUE;

	if(nQtyJigan>1)
	{
		if(nMethod==1 )		nRow += 2;
		else				nRow += nAddRow;

		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->m_Ctc1);
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->m_ComboInnerJijumSlab1));
		m_Grid.SetTextMatrix(nRow,		4,			_T("-"));
		m_Grid.SetTextMatrix(nRow++,	5,			GetCheck(bOK));
		m_Grid.SetTextMatrix(nRow,		2,	"%.0f",	pCalcData->m_Ctc2);
		m_Grid.SetTextMatrix(nRow,		3,			GetRebar(pCalcData->m_ComboInnerJijumSlab2));
		m_Grid.SetTextMatrix(nRow++,	4,			_T("-"));
	}

	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	long nIdx = pData->m_nEndPartRebarConditionIdx;
	long nComboCnt = m_comboRebar.GetCount();
	if(nIdx < nComboCnt)
		m_comboRebar.SetCurSel(nIdx);
	else
	{
		m_comboRebar.SetCurSel(1);
		pData->m_nEndPartRebarConditionIdx = 1;
	}


// 	if(szText=="�߾Ӻ� �ٴ���(���������)")
// 		StressOut.Output_RebarQtyCheck(FLOOR_MID, m_richeditRebar, 0);
// 	else if(szText=="�߾Ӻ� �ٴ���(����������)")
// 		StressOut.Output_RebarQtyCheck(FLOOR_MID, m_richeditRebar, 1);
// 	else if(szText=="���� ĵƿ������")	
// 		StressOut.Output_RebarQtyCheck(FLOOR_LEFT, m_richeditRebar, 0);
// 	else if(szText=="���� ĵƿ������")
// 		StressOut.Output_RebarQtyCheck(FLOOR_RIGHT, m_richeditRebar, 0);
// 	else if(szText=="�߰�������")
// 		StressOut.Output_InnerFloor(m_richeditRebar);
// 
// 	m_richeditRebar.LineScroll(-m_richeditRebar.GetFirstVisibleLine());

	CString	strTabName = GetTitle();
	FloorType eType = GetPosition(strTabName);
	if(eType == eFloor_Center)
	{
		GetDlgItem(IDC_REBAR_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_REBAR_GROUPBOX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_REBAR_STATIC)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_REBAR_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REBAR_GROUPBOX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REBAR_STATIC)->ShowWindow(SW_HIDE);
	}

	SetRichEditRebarDesignCheck(eType, m_richeditRebar);

	m_Grid.RedrawWindow();
}

void CCalcCantileverRebarDlg::SetDataSave()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CCalcData		*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CGlobarOption	*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();

	CString	sText		= _T("");
	long	nRow		= 2;
	long	nQtyJigan	= pDB->m_nQtyJigan;
	long	nMethod		= pOpt->GetSlabCentDesignMethod();

	for(long nIdx=0; nIdx<2; nIdx++)
	{
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc1		= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ��ö�� C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH1		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �Ϲݺ� ��ö�� ���� 1-Cycle
		//pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_dia		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// //�տ��� ���信�� C����꿡 �ʿ�
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH1_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// �Ϲݺ� ��ö�� ���� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo1		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH1);
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo1_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH1_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB		= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ���ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH3		= GetRebar(m_Grid.GetTextMatrix(nRow++, 3));	// �Ϲݺ� ���ö�� ����
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo3		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH3);

		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc2		= m_Grid.GetTextMatrixDouble(nRow, 2);			// �ܺ� ��ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �ܺ� ��ö�� ���� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// �ܺ� ��ö�� ���� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2);
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB1		= m_Grid.GetTextMatrixDouble(nRow, 2);			// �ܺ� ���ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH4		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �ܺ� ���ö�� ����
		pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo4		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH4);

		nRow+=3;
	}

	if(nMethod==1)
	{
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1		= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ��ö�� ��� C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH1_1Cy	= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �Ϲݺ� ��ö�� ���� ��� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH1_2Cy = GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// �Ϲݺ� ��ö�� ���� ��� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH1_1Cy);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH1_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ��ö�� �ϸ� C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �Ϲݺ� ��ö�� ���� �ϸ� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// �Ϲݺ� ��ö�� ���� �ϸ� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2		= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ���ö�� ��� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH2		= GetRebar(m_Grid.GetTextMatrix(nRow++, 3));	// �Ϲݺ� ���ö�� ��� ����
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH2);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ���ö�� �ϸ� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH3		= GetRebar(m_Grid.GetTextMatrix(nRow++, 3));	// �Ϲݺ� ���ö�� �ϸ� ����
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH3);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �ܺ� ��ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �ܺ� ��ö�� ���� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// �ܺ� ��ö�� ���� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �ܺ� ���ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH4		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �ܺ� ���ö�� ����
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH4);
	}
	else
	{
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ��ö�� C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �Ϲݺ� ��ö�� ���� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// �Ϲݺ� ��ö�� ���� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �Ϲݺ� ���ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH3		= GetRebar(m_Grid.GetTextMatrix(nRow++, 3));	// �Ϲݺ� ���ö�� ����
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH3);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �ܺ� ��ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �ܺ� ��ö�� ���� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2_2Cy	= GetRebar(m_Grid.GetTextMatrix(nRow++, 4));	// �ܺ� ��ö�� ���� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy	= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2_2Cy);

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1			= m_Grid.GetTextMatrixDouble(nRow, 2);			// �ܺ� ���ö�� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH4		= GetRebar(m_Grid.GetTextMatrix(nRow, 3));		// �ܺ� ���ö�� ����
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4		= GetRebarDiaIdx(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH4);
	}
	nRow+=3;

	long nAddRow = 0;
	if(nMethod == 2)
	{
		pCalcData->m_dLBDeckCTC		= m_Grid.GetTextMatrixDouble(nRow, 2);						// LB-Deck ö�ٰ���1
		pCalcData->m_nLBDeckDiaIdx	= GetRebarDiaIdx(GetRebar(m_Grid.GetTextMatrix(nRow, 3)));	// LB-Deck ���� index
		pCalcData->m_dLBDeckCover	= m_Grid.GetTextMatrixDouble(nRow, 4);						// LB-Deck �Ǻ�
		nAddRow = 3;
	}

	nRow += nAddRow;
	if(nQtyJigan>1)
	{
		pCalcData->m_Ctc1					= m_Grid.GetTextMatrixDouble(nRow, 2);		// ö�ٰ���1
		pCalcData->m_CaseH1					= GetRebar(m_Grid.GetTextMatrix(nRow++, 3));// ö��ġ��1
		pCalcData->m_ComboInnerJijumSlab1	= GetRebarDiaIdx(pCalcData->m_CaseH1);		// ö�� ȣĪ�޺�1
		pCalcData->m_dia1					= atof(pCalcData->m_CaseH1);				// ö������1

		pCalcData->m_Ctc2					= m_Grid.GetTextMatrixDouble(nRow, 2);		// ö������2
		pCalcData->m_CaseH2					= GetRebar(m_Grid.GetTextMatrix(nRow, 3));	// ö��ġ��2
		pCalcData->m_ComboInnerJijumSlab2	= GetRebarDiaIdx(pCalcData->m_CaseH2);		// ö�� ȣĪ�޺�2
		pCalcData->m_dia2					= atof(pCalcData->m_CaseH2);				// ö������2
	}
}

long CCalcCantileverRebarDlg::GetRebarDiaIdx(CString szDia)
{
	if(szDia == _T("13"))	return 0;
	if(szDia == _T("16"))	return 1;
	if(szDia == _T("19"))	return 2;
	if(szDia == _T("22"))	return 3;
	if(szDia == _T("25"))	return 4;
	if(szDia == _T("29"))	return 5;
	if(szDia == _T("32"))	return 6;
	if(szDia == _T("35"))	return 7;

	return -1;
}

CString CCalcCantileverRebarDlg::GetRebar(long nIdx)
{
	if(nIdx == 0)	return _T(m_sRebar+"13");
	if(nIdx == 1)	return _T(m_sRebar+"16");
	if(nIdx == 2)	return _T(m_sRebar+"19");
	if(nIdx == 3)	return _T(m_sRebar+"22");
	if(nIdx == 4)	return _T(m_sRebar+"25");
	if(nIdx == 5)	return _T(m_sRebar+"29");
	if(nIdx == 6)	return _T(m_sRebar+"32");
	if(nIdx == 7)	return _T(m_sRebar+"35");

	return _T("");
}

CString CCalcCantileverRebarDlg::GetRebar(CString szDia)
{
	if(szDia == _T(m_sRebar+"13"))	return _T("13");
	if(szDia == _T(m_sRebar+"16"))	return _T("16");
	if(szDia == _T(m_sRebar+"19"))	return _T("19");
	if(szDia == _T(m_sRebar+"22"))	return _T("22");
	if(szDia == _T(m_sRebar+"25"))	return _T("25");
	if(szDia == _T(m_sRebar+"29"))	return _T("29");
	if(szDia == _T(m_sRebar+"32"))	return _T("32");
	if(szDia == _T(m_sRebar+"35"))	return _T("35");

	return _T("");
}


void CCalcCantileverRebarDlg::SetDataDefault()
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CCalcData		*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor		*pCalcFloor	= m_pStd->m_pCalcStd->m_pCalcFloor;
	CGlobarOption	*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());

	long	nPos	= m_TabRebar.GetCurSel();
	long	nStt	= IsUsedFlag() ? nPos : 0;
	long	nEnd	= IsUsedFlag() ? nPos+1 : FLOOR_SIZE;
	long	nMethod	= pOpt->GetSlabCentDesignMethod();

	if((IsUsedFlag() && nPos<3) || !IsUsedFlag())
	{
		for(long nIdx=nStt; nIdx<nEnd; nIdx++)
		{
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc1		= 250.0;	// �Ϲݺ� ��ö�� C.T.C 
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH1		= _T("19");	// �Ϲݺ� ��ö�� ���� 1-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH1_2Cy	= _T("19");	// �Ϲݺ� ��ö�� ���� 2-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo1		= 2;		// �Ϲݺ� ��ö��Index 1-Cycle 
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo1_2Cy	= 2;		// �Ϲݺ� ��ö��Index 2-Cycle 

			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB		= 125.0;	// �Ϲݺ� ���ö�� C.T.C
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH3		= _T("22");	// �Ϲݺ� ���ö�� ����
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo3		= 3;		// �Ϲݺ� ���ö�� Index

			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Ctc2		= 250;		// �ܺ� ��ö�� C.T.C
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2		= _T("16");	// �ܺ� ��ö�� ���� 1-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH2_2Cy	= _T("16");	// �ܺ� ��ö�� ���� 2-Cycle
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2		= 1;		// �ܺ� ��ö�� ���� Index
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo2_2Cy	= 1;		// �ܺ� ��ö�� ���� Index

			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CtcB1		= 125.0;	// �ܺ� ���ö�� C.T.C
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_CaseH4		= _T("19");	// �ܺ� ���ö�� ����
			pCalcData->CALC_CANTILEVER_DATA[nIdx].m_Rein_Combo4		= 2;		// �ܺ� ���ö�� Index

			pCalcFloor->SetCantileverRebar(nIdx, FLOOR_STRENGTH_DESIGN);
		}

		if(nMethod == 2)
		{
			pCalcData->m_dLBDeckCTC = 250.0;		// LB-Deck ���� index
			pCalcData->m_nLBDeckDiaIdx = 0;			// LB-Deck CTC
			pCalcData->m_dLBDeckCover = 85.0;		// LB-Deck �Ǻ�
		}
	}

	// ������ ����
	if(nMethod==1)
	{
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1			= 250.0;	// �Ϲݺ� ��ö�� ��� C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1		= _T("19");	// �Ϲݺ� ��ö�� ���� ��� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH1_2Cy	= _T("19");	// �Ϲݺ� ��ö�� ���� ��� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1		= 2;		// �Ϲݺ� ��ö��Index ��� 1-Cycle 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy	= 2;		// �Ϲݺ� ��ö��Index ��� 2-Cycle 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB			= 125.0;	// �Ϲݺ� ���ö�� ��� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH3		= _T("19");	// �Ϲݺ� ���ö�� ��� ����
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3		= 2;		// �Ϲݺ� ���ö�� ��� Index

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH1_1Cy	= _T("19");	// �Ϲݺ� ��ö�� �ϸ� C.T.C 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH1_2Cy	= _T("19");	// �Ϲݺ� ��ö�� ���� �ϸ� 1-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy	= 2;		// �Ϲݺ� ��ö�� ���� �ϸ� 2-Cycle
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy	= 2;		// �Ϲݺ� ��ö��Index �ϸ� 1-Cycle 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1		= 250;		// �Ϲݺ� ��ö��Index �ϸ� 2-Cycle 
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_CaseH2		= _T("19");	// �Ϲݺ� ���ö�� �ϸ� C.T.C
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2		= 2;		// �Ϲݺ� ���ö�� �ϸ� ����
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2		= 125;		// �Ϲݺ� ���ö�� �ϸ� Index
		
		pCalcFloor->SetCantileverRebar(FLOOR_MID, FLOOR_STRENGTH_DESIGN);	// ���� �����
		pCalcFloor->SetCantileverRebar(FLOOR_MID, FLOOR_EXPERIENCE_DESIGN);	// ������ �����
	}

	if((IsUsedFlag() && nPos == 3) || !IsUsedFlag())
	{
		if(pDB->m_nQtyJigan>1)
		{
			pCalcData->m_dia1					= 25.0;		// ö������1
			pCalcData->m_dia2					= 25.0;		// ö������2
			pCalcData->m_Ctc1					= 125.0;	// ö�ٰ���1
			pCalcData->m_Ctc2					= 125.0;	// ö������2
			pCalcData->m_CaseH1					= _T("25");	// ö��ġ��1
			pCalcData->m_CaseH2					= _T("25");	// ö��ġ��2
			pCalcData->m_ComboInnerJijumSlab1	= 4;		// ö�� ȣĪ�޺�1
			pCalcData->m_ComboInnerJijumSlab2	= 4;		// ö�� ȣĪ�޺�2
			pCalcFloor->SetInnerJijumRebar();
		}
	}
	SetGridTitle();
}

void CCalcCantileverRebarDlg::SetGridTitle()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CGlobarOption	*pOpt	= m_pStd->GetDataManage()->GetGlobalOption();

	long	nQtyJigan	= pDB->m_nQtyJigan;
	long	nRows		= 18;
	long	nCols		= 6;
	long	nMethod		= pOpt->GetSlabCentDesignMethod();
	int     n			= 0;
	int     i			= 0;	

	long nAddRow = 0;

	if(nQtyJigan > 1)	nRows += 4;
	if(nMethod == 1)	nRows += 2;
	if(nMethod == 2)	
	{
		nAddRow = 3;
		nRows += nAddRow;
	}

	m_Grid.SetCols(0);//�ʱ�ȭ...
	m_Grid.SetRows(0);

	m_Grid.SetCols(nCols);//�缳��...
	m_Grid.SetRows(nRows);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(49);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(2, 37);
	m_Grid.SetColumnWidth(5, 37);

	m_Grid.SetTextMatrix(0, 0, "< ���� ĵƿ������ >");	m_Grid.SetMergeCol(0, 0, 5);
	m_Grid.SetTextMatrix(1, 0, "�����ġ");				m_Grid.SetMergeCol(1, 0, 1);
	m_Grid.SetTextMatrix(1, 2, "����");
	m_Grid.SetTextMatrix(1, 3, "�� ����");
	m_Grid.SetTextMatrix(1, 4, "�� ����");
	m_Grid.SetTextMatrix(1, 5, "����");
	m_Grid.SetTextMatrix(2, 0, "�Ϲݺ�");				m_Grid.SetMergeRow(2, 3, 0);
	m_Grid.SetTextMatrix(2, 1, "��ö��");
	m_Grid.SetTextMatrix(3, 1, "���ö��");
	m_Grid.SetTextMatrix(4, 0, "�ܺ�");					m_Grid.SetMergeRow(4, 5, 0);
	m_Grid.SetTextMatrix(4, 1, "��ö��");
	m_Grid.SetTextMatrix(5, 1, "���ö��");

	m_Grid.SetTextMatrix(6, 0, "< ���� ĵƿ������ >");	m_Grid.SetMergeCol(6, 0, 5);
	m_Grid.SetTextMatrix(7, 0, "�����ġ");				m_Grid.SetMergeCol(7, 0, 1);
	m_Grid.SetTextMatrix(7, 2, "����");
	m_Grid.SetTextMatrix(7, 3, "�� ����");
	m_Grid.SetTextMatrix(7, 4, "�� ����");
	m_Grid.SetTextMatrix(7, 5, "����");
	m_Grid.SetTextMatrix(8, 0, "�Ϲݺ�");				m_Grid.SetMergeRow(8, 9, 0);
	m_Grid.SetTextMatrix(8, 1, "��ö��");
	m_Grid.SetTextMatrix(9, 1, "���ö��");
	m_Grid.SetTextMatrix(10, 0, "�ܺ�");				m_Grid.SetMergeRow(10,11, 0);
	m_Grid.SetTextMatrix(10, 1, "��ö��");
	m_Grid.SetTextMatrix(11, 1, "���ö��");

	m_Grid.SetTextMatrix(12, 0, "< �߾Ӻ� >");			m_Grid.SetMergeCol(12, 0, 5);
	m_Grid.SetTextMatrix(13, 0, "�����ġ");			m_Grid.SetMergeCol(13, 0, 1);
	m_Grid.SetTextMatrix(13, 2, "����");
	m_Grid.SetTextMatrix(13, 3, "�� ����");
	m_Grid.SetTextMatrix(13, 4, "�� ����");
	m_Grid.SetTextMatrix(13, 5, "����");

	m_Grid.SetCellType(2, 3, CT_COMBO);	m_Grid.SetCellType(2, 4, CT_COMBO);
	m_Grid.SetCellType(3, 3, CT_COMBO);
	m_Grid.SetCellType(4, 3, CT_COMBO);	m_Grid.SetCellType(4, 4, CT_COMBO);
	m_Grid.SetCellType(5, 3, CT_COMBO);

	m_Grid.SetCellType(8, 3, CT_COMBO);	m_Grid.SetCellType(8, 4, CT_COMBO);
	m_Grid.SetCellType(9, 3, CT_COMBO);
	m_Grid.SetCellType(10,3, CT_COMBO);	m_Grid.SetCellType(10,4, CT_COMBO);
	m_Grid.SetCellType(11,3, CT_COMBO);
		
	for(i = 0; i < 2; i++)
		for(n = 0; n < 6; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
	for(i = 2; i < 6; i++)
		for(n = 0; n < 2; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);

	for(i = 6; i < 8; i++)
		for(n = 0; n < 6; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
	for(i = 8; i < 12; i++)
		for(n = 0; n < 2; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
		
	for(i = 12; i < 14; i++)
		for(n = 0; n < 6; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);

	if(nMethod==0 || nMethod == 2)
	{
		m_Grid.SetTextMatrix(14, 0, "�Ϲݺ�");				m_Grid.SetMergeRow(14, 15, 0);
		m_Grid.SetTextMatrix(14, 1, "��ö��");
		m_Grid.SetTextMatrix(15, 1, "���ö��");
		m_Grid.SetTextMatrix(16, 0, "�ܺ�");				m_Grid.SetMergeRow(16, 17, 0);
		m_Grid.SetTextMatrix(16, 1, "��ö��");
		m_Grid.SetTextMatrix(17, 1, "���ö��");

		for(i = 14; i < 18; i++)
			for(n = 0; n < 2; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);

		
		if(nMethod == 2)
		{
			m_Grid.SetTextMatrix(18, 0, "< �߾Ӻ�(LB-Deck) >");	m_Grid.SetMergeCol(18, 0, 5);
			m_Grid.SetTextMatrix(19, 0, "�����ġ");				m_Grid.SetMergeCol(19, 0, 1);
			m_Grid.SetTextMatrix(19, 2, "����");
			m_Grid.SetTextMatrix(19, 3, "�� ����");
			m_Grid.SetTextMatrix(19, 4, "�Ǻ�");
			m_Grid.SetTextMatrix(19, 5, "����");
			m_Grid.SetTextMatrix(20, 0, "��ö��");				m_Grid.SetMergeCol(20, 0, 1);

			m_Grid.SetCellType(20 , 3, CT_COMBO);

			for(i = 18; i < 20; i++)
				for(n = 0; n < 6; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
			
			for(n = 0; n < 2; n++)	m_Grid.SetCellFixType(20, n, GVN_ONECELL_COLFIX);
		}

		if(nQtyJigan>1)
		{
			m_Grid.SetTextMatrix(18 + nAddRow, 0, "< �߰������� >");	m_Grid.SetMergeCol(18 + nAddRow, 0, 5);
			m_Grid.SetTextMatrix(19 + nAddRow, 0, "�����ġ");		m_Grid.SetMergeCol(19 + nAddRow, 0, 1);
			m_Grid.SetTextMatrix(19 + nAddRow, 2, "����");
			m_Grid.SetTextMatrix(19 + nAddRow, 3, "�� ����");
			m_Grid.SetTextMatrix(19 + nAddRow, 4, "�� ����");
			m_Grid.SetTextMatrix(19 + nAddRow, 5, "����");
			m_Grid.SetTextMatrix(20 + nAddRow, 0, "���ö��");		m_Grid.SetMergeCol(20 + nAddRow, 0, 1);
			m_Grid.SetTextMatrix(21 + nAddRow, 0, "�ϸ�ö��");		m_Grid.SetMergeCol(21 + nAddRow, 0, 1);

			m_Grid.SetMergeRow(20 + nAddRow, 21, 5);
			m_Grid.SetCellType(20 + nAddRow, 3, CT_COMBO);			m_Grid.SetCellType(21 + nAddRow, 3, CT_COMBO);

			for(i = 18 + nAddRow; i < 20 + nAddRow; i++)
				for(n = 0; n < 6; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
			for(i = 20 + nAddRow; i < 22 + nAddRow; i++)
				for(n = 0; n < 2; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
		}

		m_Grid.SetCellType(14,3, CT_COMBO);	m_Grid.SetCellType(14,4, CT_COMBO);
		m_Grid.SetCellType(15,3, CT_COMBO);
		m_Grid.SetCellType(16,3, CT_COMBO);	m_Grid.SetCellType(16, 4, CT_COMBO);
		m_Grid.SetCellType(17,3, CT_COMBO);
	}
	else
	{
		m_Grid.SetTextMatrix(14, 0, "�Ϲݺ�");				m_Grid.SetMergeRow(14, 17, 0);
		m_Grid.SetTextMatrix(14, 1, "��ö�ٻ��");
		m_Grid.SetTextMatrix(15, 1, "��ö���ϸ�");
		m_Grid.SetTextMatrix(16, 1, "���ö�ٻ��");
		m_Grid.SetTextMatrix(17, 1, "���ö���ϸ�");

		m_Grid.SetTextMatrix(18, 0, "�ܺ�");				m_Grid.SetMergeRow(18, 19, 0);
		m_Grid.SetTextMatrix(18, 1, "��ö��");
		m_Grid.SetTextMatrix(19, 1, "���ö��");

		for(i = 14; i < 20; i++)
			for(n = 0; n < 2; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);

		if(nQtyJigan>1)
		{
			m_Grid.SetTextMatrix(20, 0, "< �߰������� >");	m_Grid.SetMergeCol(20, 0, 5);
			m_Grid.SetTextMatrix(21, 0, "�����ġ");		m_Grid.SetMergeCol(21, 0, 1);
			m_Grid.SetTextMatrix(21, 2, "����");
			m_Grid.SetTextMatrix(21, 3, "�� ����");
			m_Grid.SetTextMatrix(21, 4, "�� ����");
			m_Grid.SetTextMatrix(21, 5, "����");
			m_Grid.SetTextMatrix(22, 0, "���ö��");		m_Grid.SetMergeCol(22, 0, 1);
			m_Grid.SetTextMatrix(23, 0, "�ϸ�ö��");		m_Grid.SetMergeCol(23, 0, 1);

			m_Grid.SetMergeRow(22,23, 5);
			m_Grid.SetCellType(22, 3, CT_COMBO);			m_Grid.SetCellType(23,3, CT_COMBO);

			for(i = 20; i < 22; i++)
				for(n = 0; n < 6; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
			for(i = 22; i < 24; i++)
				for(n = 0; n < 2; n++)	m_Grid.SetCellFixType(i, n, GVN_ONECELL_COLFIX);
		}

		m_Grid.SetCellType(14,3, CT_COMBO);	m_Grid.SetCellType(14, 4, CT_COMBO);
		m_Grid.SetCellType(15,3, CT_COMBO);	m_Grid.SetCellType(15, 4, CT_COMBO);

		m_Grid.SetCellType(16,3, CT_COMBO);	
		m_Grid.SetCellType(17,3, CT_COMBO);
		m_Grid.SetCellType(18,3, CT_COMBO);	m_Grid.SetCellType(18, 4, CT_COMBO);
		m_Grid.SetCellType(19,3, CT_COMBO);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
	SetActiveGrid();
}

void CCalcCantileverRebarDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	CDomyun Dom(pDom);

	*pDom << Dom;
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

BOOL CCalcCantileverRebarDlg::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;
	

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		long	nRow	= m_Grid.GetFocusCell().row;
		long	nCol	= m_Grid.GetFocusCell().col;

		if(m_Grid.GetCellType(nRow, nCol)==CT_COMBO)
		{
			strCombo.Add(m_sRebar+"13");
			strCombo.Add(m_sRebar+"16");
			strCombo.Add(m_sRebar+"19");
			strCombo.Add(m_sRebar+"22");
			strCombo.Add(m_sRebar+"25");
			strCombo.Add(m_sRebar+"29");
			strCombo.Add(m_sRebar+"32");
			strCombo.Add(m_sRebar+"35");
			m_Grid.SetComboString(strCombo);
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CCalcCantileverRebarDlg::OnSelchangeTabRebar(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetGridTitle();	
	SetDataInit();
}

void CCalcCantileverRebarDlg::OnCellChangedData(NMHDR*nmgv,LRESULT*)
{
	CCalcFloor	*pCalcFloor	= m_pStd->m_pCalcStd->m_pCalcFloor;
	CString	szTabTitle	= GetTitle();

	SetDataSave();
	if(szTabTitle=="���� ĵƿ������")					pCalcFloor->SetCantileverRebar(FLOOR_LEFT, FLOOR_STRENGTH_DESIGN);
	else if(szTabTitle=="���� ĵƿ������")				pCalcFloor->SetCantileverRebar(FLOOR_RIGHT, FLOOR_STRENGTH_DESIGN);
	else if(szTabTitle=="�߾Ӻ� �ٴ���(���������)")		pCalcFloor->SetCantileverRebar(FLOOR_MID, FLOOR_STRENGTH_DESIGN);
	else if(szTabTitle=="�߾Ӻ� �ٴ���(����������)")		pCalcFloor->SetCantileverRebar(FLOOR_MID, FLOOR_EXPERIENCE_DESIGN);
	else if(szTabTitle=="�߰�������")					pCalcFloor->SetInnerJijumRebar();
	SetDataInit();
}

void CCalcCantileverRebarDlg::OnCellChangedRow(NMHDR*nmgv,LRESULT*)
{
	CGlobarOption	*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;

    long	nRow	= GridView->iRow;
	long	nSide	= m_TabRebar.GetCurSel();
	long	nTab	= 0;
	long	nMethod	= pOpt->GetSlabCentDesignMethod();
	BOOL	bChange	= FALSE;
	BOOL	bLeft	= CalcGeneral.CheckExperienceDesign(TRUE);
	BOOL	bRight	= CalcGeneral.CheckExperienceDesign(FALSE);

	if(nRow>=0 && nRow<=5)		nTab = 0;
	if(nRow>=6 && nRow<=11)		nTab = 1;
	if(nMethod == 1)
	{
		if(nRow>=12 && nRow<=19)
		{
			if(bLeft && bRight)
				nTab = 2;
			else if((!bLeft && bRight) || (bLeft && !bRight))
				nTab = 3;
		}
		else if(nRow>=20 && nRow<=23)
		{
			if(bLeft && bRight)
				nTab = 3;
			else if((!bLeft && bRight) || (bLeft && !bRight))
				nTab = 4;
		}
	}
	else
	{
		long nAddRow = 0;
		if(nMethod == 2)
			nAddRow = 3;

		if(nRow>=12 && nRow <= 17 + nAddRow)	
			nTab = 2;
		if(nRow >= 18 + nAddRow && nRow <= 21 + nAddRow)	
			nTab = 3;
	}

	if(nSide != nTab)	bChange	= TRUE;
	else				bChange	= FALSE;
	if(bChange)
	{
		m_TabRebar.SetCurSel(nTab);
		SetActiveGrid();
		SetDataInit();
	}
}

void CCalcCantileverRebarDlg::SetActiveGrid()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CGlobarOption	*pOpt	= m_pStd->GetDataManage()->GetGlobalOption();

	long	nPos		= m_TabRebar.GetCurSel();
	long	nRowsu		= m_Grid.GetRowCount();
	long	nColsu		= m_Grid.GetColumnCount();
	long	nAddRow		= 0;
	long	nMethod		= pOpt->GetSlabCentDesignMethod();
	long	nFloorIdx	= 0;
	CString	szText		= _T("");
	CString	szTabTitle	= GetTitle();

	if(szTabTitle=="�߾Ӻ� �ٴ���(���������)" || szTabTitle=="�߾Ӻ� �ٴ���(����������)")
		nFloorIdx = FLOOR_MID;
	else if(szTabTitle=="���� ĵƿ������")
		nFloorIdx = FLOOR_LEFT;
	else if(szTabTitle=="���� ĵƿ������")
		nFloorIdx = FLOOR_RIGHT;
	else
		nFloorIdx = -1;
	long nRow = 0;
	for(nRow=0; nRow<nRowsu; nRow++)
	{
		
		szText.Format("%s", m_Grid.GetTextMatrix(nRow, 0));
		if(nFloorIdx==FLOOR_LEFT && szText == _T("< ���� ĵƿ������ >"))
		{
			nAddRow	= 5;
			break;
		}
		if(nFloorIdx==FLOOR_RIGHT && szText == _T("< ���� ĵƿ������ >"))
		{
			nAddRow	= 5;
			break;
		}
		if(nFloorIdx==FLOOR_MID && szText == _T("< �߾Ӻ� >"))
		{
			if(nMethod==1)	
				nAddRow	= 7;
			else	
				nAddRow	= 5;
			break;
		}
		if(szTabTitle=="�߰�������" && szText == _T("< �߰������� >"))
		{
			nAddRow	= 3;
			break;
		}
	}

	for(long nR=0; nR<nRowsu; nR++)//��ü�� �б��������� �ϰ�...
	{
		for(long nCol=0; nCol<nColsu; nCol++)
		{
			if(!m_Grid.GetCellFixType(nR,nCol))//��Ÿ���� Fix�� �ƴϸ�...
			{
				m_Grid.SetItemBkColour(nR, nCol, RGB(255,255,200));//RGB(255,255,200));
				m_Grid.SetItemState(nR, nCol, GVIS_READONLY);
			}
			
		}
	}

	if(nPos==2 && m_TabRebar.GetItemCount()==5)
	{

	}
	else
	{
		for(long nR=nRow+2; nR<=nRow+nAddRow; nR++)
		{
			long nCol = 0;
			for(nCol=2; nCol<nColsu; nCol++)
				m_Grid.SetItemBkColour(nR, nCol, RGB(255,255,255));
			for(nCol=2; nCol<nColsu-1; nCol++)
				m_Grid.SetItemState(nR, nCol, GVIS_MODIFIED);
		}

		if(nFloorIdx==FLOOR_MID && szText == _T("< �߾Ӻ� >") && nMethod == 2)
		{
			long nCol = 0;
			for(nCol=2; nCol<nColsu; nCol++)
				m_Grid.SetItemBkColour(nRow+nAddRow+3, nCol, RGB(255,255,255));
			for(nCol=2; nCol<nColsu-1; nCol++)
				m_Grid.SetItemState(nRow+nAddRow+3, nCol, GVIS_MODIFIED);
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

CString CCalcCantileverRebarDlg::GetTitle()
{
	CString	szTabTitle	= _T("");
	TCITEM	TabCtrlItem;
	TabCtrlItem.mask		= TCIF_TEXT;
	TabCtrlItem.pszText		= szTabTitle.GetBufferSetLength(255);
	TabCtrlItem.cchTextMax	= 255;
	m_TabRebar.GetItem(m_TabRebar.GetCurSel(), &TabCtrlItem);

	return szTabTitle;
}

BOOL CCalcCantileverRebarDlg::IsValid()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	BOOL bValid = pDB->m_bTendonInputComplete;

	if (bValid)
		m_bUsedFlag = FALSE;

	return !bValid;
}

FloorType CCalcCantileverRebarDlg::GetPosition( CString strTabName )
{
	FloorType eType;

	if(strTabName=="���� ĵƿ������")
		eType = eFloor_LeftCantilever;
	else if(strTabName=="���� ĵƿ������")
		eType = eFloor_RightCantilever;	
	else if(strTabName=="�߰�������")
		eType = eFloor_MiddleJijum;
	else if(strTabName=="�߾Ӻ� �ٴ���(����������)")
		eType = eFloor_CenterExperience;
	else
		eType = eFloor_Center;

	return eType;
}

void CCalcCantileverRebarDlg::SetTextFont()
{
	m_cfDefault.crTextColor		= RGB_BLACK;
	m_cfDefault.bCharSet		= HANGUL_CHARSET;
	m_cfDefault.bPitchAndFamily = HANGUL_CHARSET;
	m_cfDefault.dwEffects		= NULL;
	m_cfDefault.dwMask			= CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR | CFM_FACE | CFM_SIZE;
	m_cfDefault.yHeight			= 9*20;
	strcpy(m_cfDefault.szFaceName,_T("����ü"));

	m_cfNG.crTextColor		= RGB_LRED;
	m_cfNG.bCharSet			= HANGUL_CHARSET;
	m_cfNG.bPitchAndFamily  = HANGUL_CHARSET;
	m_cfNG.dwEffects		= NULL;
	m_cfNG.dwMask			= CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR | CFM_FACE | CFM_SIZE;
	m_cfNG.yHeight			= 9*20;
	strcpy(m_cfNG.szFaceName,_T("����ü"));
}

void CCalcCantileverRebarDlg::parse()
{
	//turn off response to onchange events
	long mask = m_richeditRebar.GetEventMask();
	m_richeditRebar.SetEventMask(mask ^= ENM_CHANGE );

	//set redraw to false to reduce flicker, and to speed things up
	m_richeditRebar.SetRedraw(FALSE);

	//do some cleanup
	m_richeditRebar.SetSel(0,0);
	m_richeditRebar.SetRedraw(TRUE);
	m_richeditRebar.RedrawWindow();

	m_richeditRebar.SetEventMask(mask |= ENM_CHANGE );
}

void CCalcCantileverRebarDlg::SetRichEditRebarDesignCheck( enum FloorType eType, CRichEditCtrl &RichEdit )
{
	RichEdit.SetRedraw(FALSE);

	CCalcFloor *pCalcFloor = m_pStd->m_pCalcStd->m_pCalcFloor;
	pCalcFloor->MakeCalculateOutputFile(eType);

	CFile file(pCalcFloor->GetFileNameResult(),CFile::modeReadWrite|CFile::typeBinary);

	EDITSTREAM es;
	es.dwCookie = (DWORD)&file;
	es.pfnCallback = readFunction;
	RichEdit.StreamIn(SF_TEXT,es);
	file.Close();

	parse();

	/////////
	FINDTEXTEX fText;
	fText.chrg.cpMin =  0;
	fText.chrg.cpMax = -1;

	long nLines   = RichEdit.GetLineCount();

	for(long row=0;row<nLines-1;row++)
	{
		char strLine[150] = {'a',};
		RichEdit.GetLine(row,strLine);

		CString csLine = _T("");	
		csLine.Append(strLine);

		if(csLine.Find("N.G")!=-1)
		{
			fText.lpstrText = strLine;			
			RichEdit.FindText(0,&fText);	
			RichEdit.SetSel(fText.chrgText);
			RichEdit.SetSelectionCharFormat(m_cfNG);
			fText.chrg.cpMin = fText.chrgText.cpMax;
		}
	}
	/////////
	UpdateData(FALSE);
}


void CCalcCantileverRebarDlg::OnSelchangeRebarCombo()
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	pData->m_nEndPartRebarConditionIdx = m_comboRebar.GetCurSel();

	SetDataInit();
}
