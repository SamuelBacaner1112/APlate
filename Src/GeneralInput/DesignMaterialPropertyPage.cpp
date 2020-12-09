// DesignMaterialPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "DesignMaterialPropertyPage.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignMaterialPropertyPage dialog

CDesignMaterialPropertyPage::CDesignMaterialPropertyPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignMaterialPropertyPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignMaterialPropertyPage)
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
	m_pDesignPageDlg = (CDesignPageDlg*)pParent;
}

void CDesignMaterialPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignMaterialPropertyPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignMaterialPropertyPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignMaterialPropertyPage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)	
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignMaterialPropertyPage message handlers
void CDesignMaterialPropertyPage::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignMaterialPropertyPage::SetGridTitle()
{
	long nBridgeType = 0;
	
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - ���� ��ũ��Ʈ ź������� nc �� TU�Ŵ� �ɼǿ����� ������ ��

	long	nRows	=	bTUType ? 30 : 29;
	long	nCols	=	5;
	CString str		=	_T("");

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(3);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 220);
	m_Grid.SetColumnWidth(2, 130);
	m_Grid.SetColumnWidth(3, 100);

	
	m_Grid.SetTextMatrix(0,0,"�� ��");
	m_Grid.SetTextMatrix(0,3,"�� ��");
	m_Grid.SetTextMatrix(0,4,"�� ��");
	m_Grid.SetMergeCol(0,0,2);

	//��з�
	m_Grid.SetTextMatrix(1,0,"��ũ��Ʈ");
	m_Grid.SetMergeRow(1,3,0);

	m_Grid.SetTextMatrix(4,0,"ö��");
	m_Grid.SetMergeRow(4,6,0);

	m_Grid.SetTextMatrix(7,0,"����");
	m_Grid.SetMergeRow(7,8,0);

	if(bTUType)
	{
		m_Grid.SetTextMatrix(9, 0,"ź�����");
		m_Grid.SetMergeRow(9,17,0);

		m_Grid.SetTextMatrix(18,0,"����� ������ Ư��");
		m_Grid.SetMergeRow(18,23,0);

		m_Grid.SetTextMatrix(24,0,"�������");
		m_Grid.SetMergeRow(24,29,0);	
	}
	else
	{
		m_Grid.SetTextMatrix(9, 0,"ź�����");
		m_Grid.SetMergeRow(9,16,0);

		m_Grid.SetTextMatrix(17,0,"����� ������ Ư��");
		m_Grid.SetMergeRow(17,22,0);

		m_Grid.SetTextMatrix(23,0,"�������");
		m_Grid.SetMergeRow(23,28,0);	
	}


	//�� ����

	long	nRow	=	1;
	long	nUntil	=	24;	//������� �������� �� ����

	for(long nR=1;nR < nUntil; nR++)
	{
		m_Grid.SetMergeCol(nR,1,2);
	}
	
	//��ũ��Ʈ �ߺз�
	m_Grid.SetTextMatrix(nRow++,	1,	"fck");
	m_Grid.SetTextMatrix(nRow++,	1,	"fci");
	
	//ö�� �ߺз�
	m_Grid.SetTextMatrix(nRow++,	1,	"������ũ��Ʈ fck");
	m_Grid.SetTextMatrix(nRow++,	1,	"fy");
	m_Grid.SetTextMatrix(nRow++,	1,	"������ ����� fsa");
	m_Grid.SetTextMatrix(nRow++,	1,	"�ٴ��� ����� fsa");

	//���� �ߺз�
	m_Grid.SetTextMatrix(nRow++,	1,	"�ֺ���");
	m_Grid.SetTextMatrix(nRow++,	1,	"�κ���");

	//ź����� �ߺз�
	m_Grid.SetTextMatrix(nRow++,	1,	"��ũ��Ʈ Ec");
	m_Grid.SetTextMatrix(nRow++,	1,	"��ũ��Ʈ Eci");
	m_Grid.SetTextMatrix(nRow++,	1,	"������ũ��Ʈ Ec");
	m_Grid.SetTextMatrix(nRow++,	1,	"ö�� Es");
	m_Grid.SetTextMatrix(nRow++,	1,	"����ź����� G");
	m_Grid.SetTextMatrix(nRow++,	1,	"���� Est");
	m_Grid.SetTextMatrix(nRow++,	1,	"���̺��� Est");
	m_Grid.SetTextMatrix(nRow++,	1,	"�ٴ��� ź������� n");
	if(bTUType == TRUE)
	{
		m_Grid.SetTextMatrix(nRow++,	1,	"������ũ��Ʈ ź������� nc");
	}

	//����� ������ Ư�� �ߺз�
	m_Grid.SetTextMatrix(nRow++,	1,	"���������� ��s");
	m_Grid.SetTextMatrix(nRow++,	1,	"�� ��â��� ��");
	m_Grid.SetTextMatrix(nRow++,	1,	"�ٴ��� ��ũ��Ʈ�� ũ���� ��1");
	m_Grid.SetTextMatrix(nRow++,	1,	"�ٴ��� ��ũ��Ʈ�� �������� ��2");
	m_Grid.SetTextMatrix(nRow++,	1,	"��ũ��Ʈ ���Ƽۺ�");
	m_Grid.SetTextMatrix(nRow++,	1,	"�ٴ��� ��ũ��Ʈ�� �������� �µ���");

	//������� �ߺз�
	long nRowTemp = nRow;
	m_Grid.SetTextMatrix(nRow, 1, "������Ʈ���� �������� \n (ũ����, �������� �ս���)");
	m_Grid.SetItemFormat(nRow, 1, DT_CENTER);
	m_Grid.SetMergeRow(nRow, nRow+1, 1);
	nRow++;

	//������Ʈ���� �Һз�
	m_Grid.SetTextMatrix(nRowTemp,		2,	"��������");
	m_Grid.SetCellType(nRowTemp+1,		3, CT_COMBO);
	m_Grid.SetTextMatrix(nRowTemp+1,	2,	"��������");
	
	nRow++;
	nRowTemp	=	nRow;
	m_Grid.SetMergeRow(nRow,nRow+1,1);
	m_Grid.SetTextMatrix(nRow++,1,"���� ���� �ۿ�� (�ս���)");
	
	//���� ���� �ۿ�� �Һз�
	m_Grid.SetTextMatrix(nRowTemp,		2,	"��������");

	m_Grid.SetCellType(nRowTemp+1,		3, CT_COMBO);
	m_Grid.SetTextMatrix(nRowTemp+1,	2,	"��������");

	nRow++;
	//��ũ��Ʈ ��� �� ��������
	m_Grid.SetMergeCol(nRow,1,2);
	m_Grid.SetTextMatrix(nRow++,1,"��ũ��Ʈ ��� �� ��������");
	
	//�տ�����
	m_Grid.SetMergeCol(nRow,1,2);
	m_Grid.SetCellType(nRow,	3, CT_COMBO);
	m_Grid.SetTextMatrix(nRow,1,"�տ�����");
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDesignMaterialPropertyPage::SetDataInit()
{
	SetGridTitle();

	CCalcData			*pData			= m_pDataManage->GetCalcData();
	long				nPrestressIdx	= pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	= m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);
	long				nRow			= 1;	

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - ���� ��ũ��Ʈ ź������� nc �� TU�Ŵ� �ɼǿ����� ������ ��
	
	CString szComboData = _T("");

	//��ũ��Ʈ(��з�)
	
	////fck
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaCK);//1
	
	////fci
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaCi);//2

	////������ũ��Ʈ fck(TU)
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dBindSigmaCK);	// TU

	//ö��(��з�)
	
	////fy
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaY);

	////������ ���� �� fsa
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaSA);

	////�ٴ��� ���� �� fsa
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dSigmaDeckSA);

	//����(��з�)
	
	////�ֺ���
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_szSwsJu);
	
	////�κ���
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);	
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_szSwsBu);	
	
	//ź�����(��з�)

	////��ũ��Ʈ Ec
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEc);

	////��ũ��Ʈ Eci
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEci);

	////������ũ��Ʈ Ec(TU)
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dBindEc);	

	////ö�� Es
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEs);

	////���� ź����� G
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dShearEs);

	////���� Est
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEst);

	////���̺��� Est
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dDummyEst);

	////�ٴ��� ź������� n
	m_Grid.SetTextMatrix(nRow++,3,"%g",(double)pData->DESIGN_MATERIAL.m_nN);

	////������ũ��Ʈ ź������� nc(TU)
	if(bTUType == TRUE)
	{
		m_Grid.SetTextMatrix(nRow++,3,"%g",(double)pData->DESIGN_MATERIAL.m_nBindNc);	// TU
	}
	
	//����� ������ Ư��(��з�)

	////���������� 
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dEpsilonS);
	
	////�� ��â ���	
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dAlpha);
	
	////�ٴ��� ��ũ��Ʈ�� ũ���� 
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dCreep);
	
	////�ٴ��� ��ũ��Ʈ�� ��������
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dCreep2);

	////��ũ��Ʈ ���Ƽۺ�
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_RatePoasong);

	////�ٴ��� ��ũ��Ʈ�� �������� �µ���
	m_Grid.SetTextMatrix(nRow++,3,"%g",pData->DESIGN_MATERIAL.m_dTemparature);

	//�������(��з�)
	
	////������Ʈ���� ��������(�ߺз�)

	//////��������
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);					
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow);

	//////��������
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow);

	////���� ���� �ۿ��(�ߺз�)

	//////��������
	m_Grid.SetItemBkColour(nRow,3, RGBREADONLY);					
	m_Grid.SetItemState(nRow,3, m_Grid.GetItemState(nRow,3) | GVIS_READONLY);
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow );

	//////��������
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow);

	////��ũ��Ʈ ��� �� ��������
	m_Grid.SetTextMatrix(nRow++,3,pData->DESIGN_MATERIAL.m_strAllow_DeckConcStretchPow );

	////�տ�����
	m_Grid.SetTextMatrix(nRow,3,pData->DESIGN_MATERIAL.m_strCrackPow);

	//���� ����
	SetUnit();

	m_Grid.SetRedraw(TRUE,TRUE);
}
void CDesignMaterialPropertyPage::SetUnit()
{
	long nRow	=	0;
	long nMaxRow = 29;

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - ���� ��ũ��Ʈ ź������� nc �� TU�Ŵ� �ɼǿ����� ������ ��

	if(bTUType == TRUE)
	{
		nMaxRow = 30;
	}
	for(nRow = 1; nRow < nMaxRow; nRow++)
	{
		m_Grid.SetItemBkColour(nRow,4, RGBREADONLY);					
		m_Grid.SetItemState(nRow,4, m_Grid.GetItemState(nRow,4) | GVIS_READONLY);
	}
	
	//fck ~ �ٴ��� ����� fsa
	for(nRow = 1; nRow < 7; nRow++)
	{
		m_Grid.SetTextMatrix(nRow,	4,	"MPa");
	}
	
	//��ũ��Ʈ Ec ~ ���̺��� Est
	for(nRow = 9; nRow < 16; nRow++)
	{
		m_Grid.SetTextMatrix(nRow,		4,	"MPa");
	}

	long nRowChange = bTUType ? 23 : 22;
	m_Grid.SetTextMatrix( nRowChange,	4,"��");	//�ٴ��� ��ũ��Ʈ�� �������� �µ���

	//�������
	nRowChange = bTUType ? 24 : 23;
	for(nRow = nRowChange; nRow < nMaxRow; nRow++)
	{
		m_Grid.SetTextMatrix(nRow,	4,	"MPa");
	}
	
}
BOOL CDesignMaterialPropertyPage::OnInitDialog() 
{
	TStackedPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDesignMaterialPropertyPage::OnCellChanged(NMHDR* nmgv, LRESULT*)
{

}

void CDesignMaterialPropertyPage::SetConcreteEcSigmaCKByFck(double dFck)
{
	CCalcData *pCalcData				= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight				= m_pDataManage->GetSteelUWeight();	
	long				nPrestressIdx	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	= m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);

	double dWc = tokNPM3(pUWeight->m_dUWeightMugunCon)/10;
	double dEc = GetValueEcConc(dFck*10, dWc, TRUE);
	double dEs = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	
	pCalcData->DESIGN_MATERIAL.m_dEc		= dEc/10;
	pCalcData->DESIGN_MATERIAL.m_dSigmaCK	= dFck;

	double dn  = dEs/(dEc/10);
	pCalcData->DESIGN_MATERIAL.m_nN = (long)dn;
}

void CDesignMaterialPropertyPage::SetConcreteEcSigmaCKByFci(double dFci)
{
	CCalcData *pCalcData	= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight = m_pDataManage->GetSteelUWeight();	
	
	double dWc = tokNPM3(pUWeight->m_dUWeightMugunCon)/10;
	double dEc = GetValueEcConc(dFci*10, dWc, TRUE);
	
	pCalcData->DESIGN_MATERIAL.m_dEci		= dEc/10;
	pCalcData->DESIGN_MATERIAL.m_dSigmaCi	= dFci;
}

void CDesignMaterialPropertyPage::SetConcreteEcSigmaCKByFck2(double dFck)
{
	CCalcData *pCalcData				= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight				= m_pDataManage->GetSteelUWeight();	
	long				nPrestressIdx	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	= m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);
	
	double dWc = tokNPM3(pUWeight->m_dUWeightMugunCon)/10;
	double dEc = GetValueEcConc(dFck*10, dWc, TRUE);
	double dEs = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	
	pCalcData->DESIGN_MATERIAL.m_dBindEc		= dEc/10;
	pCalcData->DESIGN_MATERIAL.m_dBindSigmaCK	= dFck;
	
	double dbn = dEs/(dEc/10);
	pCalcData->DESIGN_MATERIAL.m_nBindNc = (long)dbn;
}

void CDesignMaterialPropertyPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	CCalcData *pCalcData = m_pDataManage->GetCalcData();
	long				nPrestressIdx	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData   = m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);

	if (nRow == 1 && nCol == 3)
	{
		double dFck = m_Grid.GetTextMatrixDouble(nRow, nCol);
		SetConcreteEcSigmaCKByFck(dFck);
		SetDataInit();
	}
	if (nRow == 2 && nCol == 3)
	{
		double dFci = m_Grid.GetTextMatrixDouble(nRow, nCol);
		SetConcreteEcSigmaCKByFci(dFci);
		SetDataInit();
	}
	if (nRow == 3 && nCol == 3)
	{
		double dFck = m_Grid.GetTextMatrixDouble(nRow, nCol);
		SetConcreteEcSigmaCKByFck2(dFck);
		SetDataInit();
	}
	if (nRow==9 && nCol==3)
	{
		double dEc = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dEs = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
		double dn  = dEs/(dEc);
		
		pCalcData->DESIGN_MATERIAL.m_dEc = dEc;
		pCalcData->DESIGN_MATERIAL.m_nN = (long) dn;
		SetDataInit();
	}
	if (nRow==11 && nCol==3)
	{
		double dEc = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dEst = pCalcData->DESIGN_MATERIAL.m_dEst;
		double dn  = dEst/(dEc);
		
		pCalcData->DESIGN_MATERIAL.m_dBindEc = dEc;
		pCalcData->DESIGN_MATERIAL.m_nBindNc = (long) dn;
		SetDataInit();
	}
	if (nRow==13 && nCol==3)
	{
		double dShearEs = m_Grid.GetTextMatrixDouble(nRow, nCol);
		pCalcData->DESIGN_MATERIAL.m_dShearEs = dShearEs;
		SetDataInit();
	}
	if (nRow==14 && nCol==3)
	{
		double dEc  = pCalcData->DESIGN_MATERIAL.m_dBindEc;
		double dEst = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dn	= dEst / dEc;
		pCalcData->DESIGN_MATERIAL.m_dEst	 = dEst;
		pCalcData->DESIGN_MATERIAL.m_nBindNc = (long)dn;
		SetDataInit();
	}
	if (nRow==15 && nCol==3)
	{
		double m_dDummyEst = m_Grid.GetTextMatrixDouble(nRow, nCol);
		pCalcData->DESIGN_MATERIAL.m_dDummyEst = m_dDummyEst;
		SetDataInit();
	}
}

void CDesignMaterialPropertyPage::SetDataSave()
{
	CCalcData		*pData		= m_pDataManage->GetCalcData();
	CADeckData		*pDeck		= m_pDataManage->GetDeckData();
	long			nRow		= 1;

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - ���� ��ũ��Ʈ ź������� nc �� TU�Ŵ� �ɼǿ����� ������ ��

	pData->DESIGN_MATERIAL.m_dSigmaCK		= m_Grid.GetTextMatrixDouble(nRow++,3);	//fck
	pData->DESIGN_MATERIAL.m_dSigmaCi		= m_Grid.GetTextMatrixDouble(nRow++,3);	//fci
	pData->DESIGN_MATERIAL.m_dBindSigmaCK	= m_Grid.GetTextMatrixDouble(nRow++,3);	//������ũ��Ʈ fck
	pData->DESIGN_MATERIAL.m_dSigmaY		= m_Grid.GetTextMatrixDouble(nRow++,3); //fy
	pData->DESIGN_MATERIAL.m_sSigmaY		= GetCharRebarMark(pData->DESIGN_MATERIAL.m_dSigmaY, FALSE);
	pData->DESIGN_MATERIAL.m_dSigmaSA		= m_Grid.GetTextMatrixDouble(nRow++,3); //������ ����� fsa
	pData->DESIGN_MATERIAL.m_dSigmaDeckSA	= m_Grid.GetTextMatrixDouble(nRow++,3); //�ٴ��� ����� fsa
	pData->DESIGN_MATERIAL.m_szSwsJu		= m_Grid.GetTextMatrix(nRow++,3);		//�ֺ���
	pData->DESIGN_MATERIAL.m_szSwsBu		= m_Grid.GetTextMatrix(nRow++,3);		//�κ���
	pData->DESIGN_MATERIAL.m_dEc			= m_Grid.GetTextMatrixDouble(nRow++,3);	//��ũ��Ʈ Ec
	pData->DESIGN_MATERIAL.m_dEci			= m_Grid.GetTextMatrixDouble(nRow++,3);	//��ũ��Ʈ Eci
	pData->DESIGN_MATERIAL.m_dBindEc		= m_Grid.GetTextMatrixDouble(nRow++,3);	//������ũ��Ʈ Ec
	pData->DESIGN_MATERIAL.m_dEs			= m_Grid.GetTextMatrixDouble(nRow++,3);	//ö�� Es
	pData->DESIGN_MATERIAL.m_dShearEs		= m_Grid.GetTextMatrixDouble(nRow++,3);	//����ź����� G
	pData->DESIGN_MATERIAL.m_dEst			= m_Grid.GetTextMatrixDouble(nRow++,3);	//���� Est
	pData->DESIGN_MATERIAL.m_dDummyEst		= m_Grid.GetTextMatrixDouble(nRow++,3);	//���̺��� Est
	pData->DESIGN_MATERIAL.m_nN				= m_Grid.GetTextMatrixLong(nRow++,3);	//�ٴ��� ź������� n
	if(bTUType == TRUE)
	{
		pData->DESIGN_MATERIAL.m_nBindNc		= m_Grid.GetTextMatrixLong(nRow++,3);	//������ũ��Ʈ ź������� nc
	}
	
	pData->DESIGN_MATERIAL.m_dEpsilonS		= m_Grid.GetTextMatrixDouble(nRow++,3);	//����������
	pData->DESIGN_MATERIAL.m_dAlpha			= m_Grid.GetTextMatrixDouble(nRow++,3);	//�� ��â ���
	pData->DESIGN_MATERIAL.m_dCreep			= m_Grid.GetTextMatrixDouble(nRow++,3);	//�ٴ��� ��ũ��Ʈ�� ũ����
	pData->DESIGN_MATERIAL.m_dCreep2		= m_Grid.GetTextMatrixDouble(nRow++,3);	//�ٴ��� ��ũ��Ʈ�� ��������
	pData->DESIGN_MATERIAL.m_RatePoasong	= m_Grid.GetTextMatrixDouble(nRow++,3);	//��ũ��Ʈ ���Ƽۺ�
	pData->DESIGN_MATERIAL.m_dTemparature	= m_Grid.GetTextMatrixDouble(nRow++,3);	//�ٴ��� ��ũ��Ʈ�� �������� �µ���

	pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow		=	m_Grid.GetTextMatrix(nRow++,3);	//��������(������Ʈ���� ���� ��)
	pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow	=	m_Grid.GetTextMatrix(nRow++,3);	//��������(������Ʈ���� ���� ��)	
	pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow		=	m_Grid.GetTextMatrix(nRow++,3);	//��������(���� ���� �ۿ��)
	pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow	=	m_Grid.GetTextMatrix(nRow++,3);	//��������(���� ������ �ۿ��)
	pData->DESIGN_MATERIAL.m_strAllow_DeckConcStretchPow	=	m_Grid.GetTextMatrix(nRow++,3);	//�ٴ��� ��ũ��Ʈ�� �������� �µ���
	pData->DESIGN_MATERIAL.m_strCrackPow					=	m_Grid.GetTextMatrix(nRow++,3);	//�տ�����

	// �ڵ��� ���ǻ� ��¿�� ���� �̷����� �Ͽ���.(KSG)
	pDeck->m_SigmaCK = pData->DESIGN_MATERIAL.m_dSigmaCK/10;
	pDeck->m_SigmaY  = pData->DESIGN_MATERIAL.m_dSigmaY/10;
	pDeck->m_SigmaSA = pData->DESIGN_MATERIAL.m_dSigmaSA/10;
}

void CDesignMaterialPropertyPage::SetDataDefault()
{
	CCalcData *pData = m_pDataManage->GetCalcData();
	CSteel *pSteel = m_pDataManage->GetSteel();
	CSteelConc *pConc = m_pDataManage->GetSteelConc();
	CSteelSection *pSteelSection = m_pDataManage->GetSteelSection();

	long nIdx = pConc->GetIndex("S27");	//��ũ��Ʈ
	if (m_pDataManage->GetGlobalOption()->GetDesignBridgeType()!=PLATE_GIR)
		nIdx = pConc->GetIndex("S40");

//	m_pDataManage->GetDeckData()->m_UWeightConcrete

	double	Wc = (m_pDataManage->GetDeckData()->m_UWeightConcrete>0) ? m_pDataManage->GetDeckData()->m_UWeightConcrete/1000 : 2.350;

	long nApplyYear = pSteel->GetApplyYear();

	pData->DESIGN_MATERIAL.m_dSigmaCK		= pConc->GetBasicStrength(nIdx);//m_pDataManage->GetGlobalOption()->GetDesignBridgeType()==0 ? 27.0 : 40.0;
	pData->DESIGN_MATERIAL.m_dSigmaCi		= 28;
	pData->DESIGN_MATERIAL.m_dSigmaY		= 400.0;
	pData->DESIGN_MATERIAL.m_sSigmaY		= "H";
	pData->DESIGN_MATERIAL.m_dSigmaSA		= 180;
	pData->DESIGN_MATERIAL.m_dSigmaDeckSA	= 160;
	pData->DESIGN_MATERIAL.m_szSwsJu		= pSteelSection->GetMaterialByMarkBuje(G_F_U);	//�ֺ���
	pData->DESIGN_MATERIAL.m_szSwsBu		= pSteelSection->GetMaterialByMarkBuje(CP_W);	//�κ���
	pData->DESIGN_MATERIAL.m_dEc			= GetValueEcConc(pData->DESIGN_MATERIAL.m_dSigmaCK/10,Wc,TRUE);
	pData->DESIGN_MATERIAL.m_dEci			= GetValueEcConc(pData->DESIGN_MATERIAL.m_dSigmaCi/10,Wc,TRUE);
	pData->DESIGN_MATERIAL.m_dBindEc		= 34000;
	pData->DESIGN_MATERIAL.m_dEs			= 200000.0;									//ö��ź�����
	pData->DESIGN_MATERIAL.m_dShearEs		= nApplyYear == APPLYYEAR2008 ? 81000.0 : 79000.0;			///< ���� ź�����
	pData->DESIGN_MATERIAL.m_dEpsilonS		= pConc->GetEpsilon(nIdx);						//����������
	pData->DESIGN_MATERIAL.m_dAlpha			= pConc->GetAlpha(nIdx);						//����â���
	pData->DESIGN_MATERIAL.m_dCreep			= pConc->GetCreep1(nIdx);						//ũ����1 
	pData->DESIGN_MATERIAL.m_dCreep2		= pConc->GetCreep2(nIdx);						//ũ����2
	pData->DESIGN_MATERIAL.m_dTemparature	= pConc->GetTemperature(nIdx);				//�µ���
	pData->DESIGN_MATERIAL.m_RatePoasong	= 0.167;
	nIdx = pSteel->GetIndex(pData->DESIGN_MATERIAL.m_szSwsJu);
	
	pData->DESIGN_MATERIAL.m_dEst         = nApplyYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//����ź�����
	pData->DESIGN_MATERIAL.m_dDummyEst    = nApplyYear == APPLYYEAR2008 ? 210000.0 : 205000.0;//pSteel->GetElasticModulus(nIdx);//����ź�����
	pData->DESIGN_MATERIAL.m_nN           = 8;

	// TU�Ŵ��� �߰�
	pData->DESIGN_MATERIAL.m_dBindSigmaCK = 50;
	pData->DESIGN_MATERIAL.m_nBindNc      = 6;
	
	//�������
	pData->SetDefaultAllowDesignMaterials();
	SetDataInit();
}

double CDesignMaterialPropertyPage::GetConcreteEcByFck(double dFck)
{
	CSteelConc *pConc = m_pDataManage->GetSteelConc();

	for (long n=0; n< pConc->GetSize(); n++)
	{
		double fc = pConc->GetBasicStrength(n);

		if(dFck == pConc->GetBasicStrength(n))
		{
			return pConc->GetElasticModulus(n);
		}
	}

	return 0;
}

double CDesignMaterialPropertyPage::GetConcreteFckByEc(double dEc)
{
	if(dEc == 22000.0)		return 21.0;
	else if(dEc == 23500.0)	return 24.0;
	else if(dEc == 25000.0)	return 27.0;
	else if(dEc == 26000.0)	return 30.0;
	else if(dEc == 30000.0)	return 40.0;
	else if(dEc == 34000.0)	return 50.0;

	return 0;
}

BOOL CDesignMaterialPropertyPage::PreTranslateMessage(MSG* pMsg)
{
	CStringArray strCombo;
	CString strLength = _T("");

	long nBridgeType = 0;
	if(m_pDesignPageDlg) 
		nBridgeType = m_pDataManage->GetBridge()->GetBridgeType();
	BOOL bTUType = nBridgeType == 2 ? TRUE : FALSE;		// 0022974 - ���� ��ũ��Ʈ ź������� nc �� TU�Ŵ� �ɼǿ����� ������ ��

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		long nSelectedRow = next.row;
		if(bTUType == FALSE)
		{
			nSelectedRow +=1;
		}
		switch(nSelectedRow)
		{
		case 25:	//��������(������Ʈ���� ��������)
			{
				strCombo.Add("1.4");
				strCombo.Add("0.25 ��(fci)");
				strCombo.Add("0.50 ��(fci)");
				m_Grid.SetComboString(strCombo);
				break;
			}
		case 27:	//��������(���� ���� �ۿ� ��)
			{
				CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
				long nStandardYear = pOpt->GetSteelStandardYear();

				if(nStandardYear == APPLYYEAR2010)	// #0028192 �̽� - ���α� ������� 2010 ���ý� 0.63 �Է� ������� �߰�
					strCombo.Add("0.63 ��(fck)");

				strCombo.Add("0.50 ��(fck)");
				strCombo.Add("0.25 ��(fck)");
				strCombo.Add("0");
				m_Grid.SetComboString(strCombo);
				break;
			}
		case 29:	//�տ�����
			{
				strCombo.Add("0.63 ��(fck)");
				strCombo.Add("0.54 ��(fck)");
				strCombo.Add("0.47 ��(fci)");
				m_Grid.SetComboString(strCombo);
				break;
			}
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	return TStackedPage::PreTranslateMessage(pMsg);
}
