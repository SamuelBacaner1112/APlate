// CalcCantileverLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Generalinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverLoadDlg dialog


CCalcCantileverLoadDlg::CCalcCantileverLoadDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcCantileverLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverLoadDlg)
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}


void CCalcCantileverLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverLoadDlg)
	DDX_Control(pDX, IDC_RICHEDIT_CANTILEVER_LOAD, m_richeditLoad);
	DDX_Control(pDX, IDC_TAB_LOAD_POSITION, m_TabPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcCantileverLoadDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverLoadDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOAD_POSITION, OnSelchangeTabLoadPosition)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverLoadDlg message handlers

BOOL CCalcCantileverLoadDlg::IsValid()
{
// 	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
// 	BOOL bValid = pDB->m_bTendonInputComplete;
// 
// 	if (!bValid)
// 		m_bUsedFlag = FALSE;
	
	return TRUE;
}


void CCalcCantileverLoadDlg::OnPreInitDialog()
{
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());
	CGlobarOption	*pOpt	= m_pStd->GetDataManage()->GetGlobalOption();

	m_TabPos.InsertItem(0, "���� ĵƿ������");
	m_TabPos.InsertItem(1, "���� ĵƿ������");

	if(pOpt->GetSlabCentDesignMethod() == 0 || pOpt->GetSlabCentDesignMethod() == 2)
	{
		// ���������ϰ��
		m_TabPos.InsertItem(2, "�߾Ӻ� �ٴ���");
	}
	else
	{
		// ������ �����ϰ��� ���� ����
		if(!CalcGeneral.CheckExperienceDesign(TRUE) || !CalcGeneral.CheckExperienceDesign(FALSE))
			m_TabPos.InsertItem(2, "�߾Ӻ� �ٴ���");
	}
	m_TabPos.SetCurSel(0);
	
	
	m_richeditLoad.SetSel(0, -1);
	m_richeditLoad.ReplaceSel("");
	m_richeditLoad.SetReadOnly(TRUE);
	// ���� ���� �Ǻ�
	CPlateBridgeApp* pDB = m_pStd->GetBridge();
	m_bTendon = pDB->m_bTendonInputComplete;

	reCalculate();

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);
	
}

void CCalcCantileverLoadDlg::SetDataDefault()
{

}

void CCalcCantileverLoadDlg::reCalculate()
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
	pCalcFloor->SetWindLoad(FLOOR_MID);
	// �������� ���
	pCalcFloor->SetCentriodLoad(FLOOR_LEFT);
	pCalcFloor->SetCentriodLoad(FLOOR_RIGHT);
	pCalcFloor->SetCentriodLoad(FLOOR_MID);
	// ��������
	pCalcFloor->SetLoadComposite(FLOOR_LEFT);
	pCalcFloor->SetLoadComposite(FLOOR_RIGHT);
	pCalcFloor->SetLoadComposite(FLOOR_MID);
}

void CCalcCantileverLoadDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcCantileverLoadDlg::OnSelchangeTabLoadPosition(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetDataInit();
	UpdateData(FALSE);
}

void CCalcCantileverLoadDlg::SetDataInit()
{
	CDataManage		*pDataManage	= m_pStd->GetDataManage();
	CPlateStressOut	StressOut(m_pStd);

	long	nPos	= m_TabPos.GetCurSel();
	BOOL	bLeft	= TRUE;
	if(nPos==0)	bLeft	= TRUE;
	else		bLeft	= FALSE;

	m_richeditLoad.SetSel(0, -1);
	m_richeditLoad.ReplaceSel("");
	m_richeditLoad.SetReadOnly(TRUE);
	
	if (m_bTendon)
	{
		// �ٴ��� �ּ� �β�
 		StressOut.Output_SlabThickness_Tendon(m_richeditLoad);
		m_richeditLoad.AddText("\n\n");
	}
	else
	{
		// �ٴ��� �ּ� �β�
		StressOut.Output_SlabThickness(nPos, m_richeditLoad);
		m_richeditLoad.AddText("\n\n");
		// Ȱ����
		if(nPos != FLOOR_MID)	StressOut.Output_MoveLoad(bLeft, m_richeditLoad);
		else					StressOut.Output_MoveLoadCenter(m_richeditLoad);
		m_richeditLoad.AddText("\n\n");
		// ǳ����
		StressOut.Output_WindLoad(nPos, m_richeditLoad);
		m_richeditLoad.AddText("\n\n");
		// ��������
		if(nPos == FLOOR_MID)	StressOut.OutPut_CentriodLoadCenter(m_richeditLoad);
		else					StressOut.Output_CentriodLoad(nPos, m_richeditLoad);
		m_richeditLoad.AddText("\n\n");
		// ���� �浹����
		StressOut.Output_CollisionLoad(nPos, m_richeditLoad);
		m_richeditLoad.AddText("\n\n");
		// ��������
		StressOut.Output_TotalCombinationLoad(nPos, m_richeditLoad);
		m_richeditLoad.AddText("\n\n");
	}
	m_richeditLoad.LineScroll(-m_richeditLoad.GetFirstVisibleLine());
	
	UpdateData(FALSE);
}

BOOL CCalcCantileverLoadDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	SetResize(IDC_RICHEDIT_CANTILEVER_LOAD, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_LOAD_POSITION, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;
}

void CCalcCantileverLoadDlg::SetDataSave()
{

}