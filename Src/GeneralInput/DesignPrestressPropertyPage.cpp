// DesignPrestressPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "../APlateCalc/APlateCalc.h"
#include "DesignPrestressPropertyPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPropertyPage dialog
SHGRIDTITLE CDesignPrestressPropertyPage::m_gt[COL] = 
{
	{"��    ��",							"%s",		170,	DT_RIGHT,	TRUE},
	{"��    ��",							"%s",		170,	DT_RIGHT,	TRUE},
	{"�� �� ��",							"%s",		200,	DT_RIGHT,	TRUE},
	{"��    ��",							"%s",		100,	DT_RIGHT,	TRUE},
};

CDesignPrestressPropertyPage::CDesignPrestressPropertyPage(CDataManage *pMng, CWnd* pParent /*=NULL*/)
	: TStackedPage(CDesignPrestressPropertyPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignPrestressPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataManage = pMng;
}


void CDesignPrestressPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	TStackedPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignPrestressPropertyPage)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignPrestressPropertyPage, TStackedPage)
	//{{AFX_MSG_MAP(CDesignPrestressPropertyPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignPrestressPropertyPage message handlers
void CDesignPrestressPropertyPage::OnPreInitDialog()
{
	CXTResizeDialog::SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
}

void CDesignPrestressPropertyPage::SetGridTitle()
{
	if(!GetSafeHwnd()) return;

	// �׸��� �⺻ ���� 
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(ROW);
	m_Grid.SetColumnCount(COL);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.InputShGridTitle(m_gt, COL, TRUE);
	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.SetRedraw(TRUE,TRUE);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 120);

	char *szUnit[] = { "", "", "mm��", "mm", "", "mm", "1/m", "1/rad", "mm", "MPa", "MPa", "MPa", "%", "", "mm", "mm", "MPa", "Mpa"};

	for(long n=0; n<ROW; n++)
	{
		m_Grid.SetTextMatrix(n, 3, szUnit[n]);
		m_Grid.SetItemState(n, 3, GVIS_READONLY);
		m_Grid.SetItemBkColour(n, 3, RGBREADONLY);
	}

	long	nRow		=	0;
	long	nRowMiddle	=	0;
	
	//��з�
	m_Grid.SetMergeCol(nRow,	0,	1);
	nRow++;
	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS���� ȣĪ");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS���� ��Ī�ܸ���");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS���� ����");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"��Ʈ�� ����");

	m_Grid.SetMergeCol(nRow	,	0	,1);
	m_Grid.SetTextMatrix(nRow++,	0,	"��Ʈ�� ����");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"�Ļ� ���� ���");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"��� ���� ���");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"������ Ȱ����");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"ź����� Es");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"���尭��(fpu)");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"�׺�����(fpy)");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"��ջ�����");

	m_Grid.SetMergeCol(nRow,	0,	1);
	m_Grid.SetTextMatrix(nRow++,	0,	"PS ������ ������ ���� ���(��p)");

	m_Grid.SetMergeRow(nRow,nRow+1,0);
	
	nRowMiddle	=	nRow;

	m_Grid.SetTextMatrix(nRow++,	0,	"������");
	
	++nRow;
	m_Grid.SetTextMatrix(nRow,		1,	"�����Է�");
	m_Grid.SetTextMatrix(nRow++,	0,	"Jack�� ���� �ִ� ��������");
	m_Grid.SetTextMatrix(nRow,		1,	"�����Է�");
	m_Grid.SetTextMatrix(nRow,		0,	"Jack�� ���� �ִ� ��������");
	
	m_Grid.SetMergeRow(nRow-1, nRow, 0, TRUE);
	//�ߺз�
	m_Grid.SetTextMatrix(nRowMiddle++,	1,	"�� (B)");
	m_Grid.SetTextMatrix(nRowMiddle,	1,	"���� (H)");

	m_Grid.SetCellType(1,	2,	CT_COMBO);
	m_Grid.SetCellType(4,	2,	CT_COMBO);
	m_Grid.SetCellType(13,	2,	CT_COMBO);
	m_Grid.SetCellType(16,	2,	CT_COMBO);

	//HIDDEN
	//m_Grid.SetRowHeight(5, 0);	

}

void CDesignPrestressPropertyPage::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CCalcData	*pData	= m_pDataManage->GetCalcData();
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	if(nRow == 1 && nCol == 2)
	{
		CString strType = m_Grid.GetTextMatrix(1, 2);
		CSteelPrestressPlate *pPreStress	= m_pDataManage->GetSteelPrestress();
		CSteelPrestressData *pPSData= pPreStress->GetSteelPrestressData(strType);
		if(!pPSData) return;
		pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName = strType;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dArea				= pPSData->m_dArea;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter			= pPSData->m_dDiameter;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength	= GetValueUnitChange(pPSData->GetTensionStrength(), _T("kgf/cm��"), _T("MPa"));	///< ���尭��
		pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength		= GetValueUnitChange(pPSData->GetYieldStrength(), _T("kgf/cm��"), _T("MPa"));	///< �׺�����
		SetDataInit();
	}
	else if(nRow == 4 && nCol == 2)
	{
		CString strType = m_Grid.GetTextMatrix(4, 2);
		long nIndex = GetLondDuctType(strType);
		pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType = nIndex;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction = GetWobbleFriction(nIndex);
		pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction = GetCurvatureFriction(nIndex);
		SetDataInit();
	}
	else if(nRow == 16)
	{
		SetDataSave();
		SetDataInit();
	}
	else if (nRow==9 && nCol==2)
	{
		double dEs = m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dEc = pData->DESIGN_MATERIAL.m_dEc;
		double dn  = dEs / dEc;	

		pData->DESIGN_STEELPRESTRESS_DATA.m_dEs = dEs;
		pData->DESIGN_MATERIAL.m_nN = (long)dn;
		SetDataInit();
	}
	else if(nRow == 17)
	{
		CString strValue = m_Grid.GetTextMatrix(nRow, 2);
		
		for(long nIdx = 0; nIdx < strValue.GetLength(); nIdx++)
		{
			if(!isdigit(strValue.GetAt(nIdx)) && strValue.GetAt(nIdx) != '.')
			{
				AfxMessageBox("���ڿ��� �Է����ּ���!", MB_OK, MB_ICONWARNING);
				m_Grid.SetTextMatrix(nRow, 2, "0");
				return;
			}
		}
	}
	

	SetDataSave();

	
}

void CDesignPrestressPropertyPage::SetDataDefault()
{
	CCalcData			*pData = m_pDataManage->GetCalcData();
	CSteelPrestressPlate		*pPrestress	= m_pDataManage->GetSteelPrestress();
	CSteelPrestressData	*pPSData	= pPrestress->GetSteelPrestressData(12);

	//��� Pressstress ������ ��ȣ
	pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx	= 12;												

	//��� Pressstress ������ ȣĪ
	pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName	= pPSData->m_szSymbol + " " + pPSData->m_szName;	

	//��Ī �ܸ���
	pData->DESIGN_STEELPRESTRESS_DATA.m_dArea	= pPSData->m_dArea;
	
	//����			
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter	= pPSData->m_dDiameter;			
	
	//��Ʈ���� - �ƿ����� �ݼӽ���
	pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType	= 1;							
	
	//��Ʈ ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter	= 22.4;							
	
	//�Ļ������
	pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction	= 0.0050;						
	
	//����������
	pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction	= 0.25;							
	
	//����ġ Ȱ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip	= 6.0;							
	
	//ź�����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dEs	= 200000.0;			

	//��������
	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionLoad	=  GetValueUnitChange(pPSData->m_dTensionLoad, _T("kgf"), _T("N"));

	//��������
	pData->DESIGN_STEELPRESTRESS_DATA.m_dLoadOfElongation	= GetValueUnitChange(pPSData->m_dLoadOfElongation, _T("kgf"), _T("N"));

	//���尭��
	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength	= GetValueUnitChange(pPSData->GetTensionStrength(), _T("kgf/cm��"), _T("MPa"));

	//�׺�����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength	= GetValueUnitChange(pPSData->GetYieldStrength(), _T("kgf/cm��"), _T("MPa"));
	
	//��ջ�����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity	= 70.0;

	//PS ������ ������ ���� ���
	pData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP	=	0.28;

	//������ ��
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW	=	190;

	//������ ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH	=	70;

	//Jack�� ���� �ִ� ���� ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax	=	"0.90��fpy";

	SetDataInit();
}

void CDesignPrestressPropertyPage::SetDataInit()
{
	SetGridTitle();

	CCalcData			*pData			=	m_pDataManage->GetCalcData();
	long				nPrestressIdx	=	pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
	CSteelPrestressData *pSteelPSData	=	m_pDataManage->GetSteelPrestress()->GetSteelPrestressData(nPrestressIdx);

	if(!pSteelPSData) return;

	m_Grid.SetTextMatrix(1,		2,			pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName);
	m_Grid.SetTextMatrix(2,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dArea);
	m_Grid.SetTextMatrix(3,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter);
	m_Grid.SetTextMatrix(4,		2,			GetStrDuctType(pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType));
	m_Grid.SetTextMatrix(5,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter);
	m_Grid.SetTextMatrix(6,		2,	"%.4f",	pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction);
	m_Grid.SetTextMatrix(7,		2,	"%.2f",	pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction);
	m_Grid.SetTextMatrix(8,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	m_Grid.SetTextMatrix(9,		2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dEs);
	m_Grid.SetTextMatrix(10,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength);
	m_Grid.SetTextMatrix(11,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength);
	m_Grid.SetTextMatrix(12,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity);
	
	m_Grid.SetTextMatrix(13,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP);
	m_Grid.SetTextMatrix(14,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW);
	m_Grid.SetTextMatrix(15,	2,	"%g",	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH);
	
	CString strJackMax = pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax;
	
	if(strJackMax.Find("fpu") < 0 && strJackMax.Find("fpy") < 0) //����� ���� 
	{
		m_Grid.SetTextMatrix(16, 2, "����� ����");
		m_Grid.SetTextMatrix(17, 2, pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax);
		m_Grid.SetItemState(17, 2, m_Grid.GetItemState(17, 2) & ~GVIS_READONLY);
	}
	else
	{
		m_Grid.SetTextMatrix(16, 2, pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax);
		m_Grid.SetTextMatrix(17, 2, "");
		m_Grid.SetItemState(17, 2, m_Grid.GetItemState(17, 2) |  GVIS_READONLY);
	}

	m_Grid.SetRedraw(TRUE,TRUE);

}

void CDesignPrestressPropertyPage::SetDataSave()
{
	CCalcData			*pData			= m_pDataManage->GetCalcData();
	CSteelPrestressPlate		*pPrestress		= m_pDataManage->GetSteelPrestress();
	CString				szText			= m_Grid.GetTextMatrix(1, 2);
	long				nPrestressIdx	= pPrestress->GetIndexByPrestressName(szText);
	CSteelPrestressData *pSteelPSData	= pPrestress->GetSteelPrestressData(nPrestressIdx);

	if(!pSteelPSData) return;

	
	pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx	= nPrestressIdx;
	
	//PS���� ȣĪ
	pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName	= m_Grid.GetTextMatrix(1, 2);

	//PS���� ��Ī�ܸ���
	pData->DESIGN_STEELPRESTRESS_DATA.m_dArea	= m_Grid.GetTextMatrixDouble(2, 2);
	
	//PS���� ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter	= m_Grid.GetTextMatrixDouble(3, 2);

	//��Ʈ�� ����
	CString strType = m_Grid.GetTextMatrix(4, 2);
	pData->DESIGN_STEELPRESTRESS_DATA.m_nDuctType	= GetLondDuctType(strType);

	//��Ʈ�� ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter	= m_Grid.GetTextMatrixDouble(5, 2);

	//�Ļ� ���� ���
	pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction	= m_Grid.GetTextMatrixDouble(6, 2);

	//��� ���� ���
	pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction	= m_Grid.GetTextMatrixDouble(7, 2);

	//������ Ȱ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip	= m_Grid.GetTextMatrixDouble(8, 2);

	//ź����� Es
	pData->DESIGN_STEELPRESTRESS_DATA.m_dEs	= m_Grid.GetTextMatrixDouble(9, 2);

	//���� ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength	= m_Grid.GetTextMatrixDouble(10, 2);

	//�׺�����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength	= m_Grid.GetTextMatrixDouble(11, 2);

	//��ջ�����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity	= m_Grid.GetTextMatrixDouble(12, 2);

	//PS ������ ������ ���� ���
	pData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP	= m_Grid.GetTextMatrixDouble(13, 2);

	//������ ��
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW	= m_Grid.GetTextMatrixDouble(14, 2);

	//������ ����
	pData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH	= m_Grid.GetTextMatrixDouble(15, 2);

	//Jack�� ���� �ִ� ��������
	CString strJackMax = m_Grid.GetTextMatrix(16, 2);

	pData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax	= m_Grid.GetTextMatrix(strJackMax == "����� ����" ? 17 : 16, 2);
}

BOOL CDesignPrestressPropertyPage::PreTranslateMessage(MSG* pMsg)
{
	CStringArray strCombo;
	CCalcData *pData = m_pDataManage->GetCalcData();
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		switch(next.row)
		{			
		case 1:
			{
				CSteelPrestressPlate *pSteelPS = m_pDataManage->GetSteelPrestress();
				if(!pSteelPS) break;
				for(long n=0; n<pSteelPS->GetSizePsGangsun(); n++)
				{
					CSteelPrestressData *pSteelPSData = pSteelPS->GetSteelPrestressData(n);
					strCombo.Add(pSteelPSData->m_szSymbol + " " + pSteelPSData->m_szName);
				}

				m_Grid.SetComboString(strCombo);
				break;
			}
		case 4:
			{
				strCombo.Add("�ݼӽ���");
				strCombo.Add("�ƿ����� �ݼӽ���");
				strCombo.Add("�ƽ���Ʈ �Ǵ� �׸���������");
				strCombo.Add("�ƿ����ݵ� ������Ʈ");

				m_Grid.SetComboString(strCombo);
				break;
			}
		case 13:
			{
				strCombo.Add("0.4");
				strCombo.Add("0.28");
				strCombo.Add("0.55");

				m_Grid.SetComboString(strCombo);
				break;

			}
		case 16:
			{
				pData->GetDefaultConstPSf(strCombo);
				m_Grid.SetComboString(strCombo);
				break;
			}
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);

	return TStackedPage::PreTranslateMessage(pMsg);
}

CString CDesignPrestressPropertyPage::GetStrDuctType(long nType)
{
	if(nType==0) return _T("�ݼӽ���");
	if(nType==1) return _T("�ƿ����� �ݼӽ���");
	if(nType==2) return _T("�ƽ���Ʈ �Ǵ� �׸���������");
	if(nType==3) return _T("�ƿ����ݵ� ������Ʈ");
	return _T("");
}

long CDesignPrestressPropertyPage::GetLondDuctType(CString strDuctType)
{
	if(strDuctType == _T("�ݼӽ���"))						return 0;
	if(strDuctType == _T("�ƿ����� �ݼӽ���"))				return 1;
	if(strDuctType == _T("�ƽ���Ʈ �Ǵ� �׸���������"))		return 2;
	if(strDuctType == _T("�ƿ����ݵ� ������Ʈ"))			return 3;

	return -1;
}

///< ����������
double CDesignPrestressPropertyPage::GetCurvatureFriction(long nIndex)
{
	if(nIndex == 0) return 0.30;
	if(nIndex == 1) return 0.25;
	if(nIndex == 2) return 0.30;
	if(nIndex == 3) return 0.25;
	return 0;
}

///< �Ļ������
double CDesignPrestressPropertyPage::GetWobbleFriction(long nIndex)
{
	if(nIndex == 0) return 0.0066;
	if(nIndex == 1) return 0.0050;
	if(nIndex == 2) return 0.0066;
	if(nIndex == 3) return 0.0007;
	return 0;
}
