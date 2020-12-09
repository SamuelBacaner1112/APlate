// CrossBeamSubInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "CrossBeamSubInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrossBeamSubInputDlg dialog


CCrossBeamSubInputDlg::CCrossBeamSubInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrossBeamSubInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCrossBeamSubInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCrossBeam = NULL;
	m_Grid.SetDefaultCharacterCount(2);
}


void CCrossBeamSubInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrossBeamSubInputDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrossBeamSubInputDlg, CDialog)
	//{{AFX_MSG_MAP(CCrossBeamSubInputDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_BUTTONCLICK,	IDC_GRID, OnCellButtonClickRecommend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrossBeamSubInputDlg message handlers

BOOL CCrossBeamSubInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetGridTitle();
	SetDataInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCrossBeamSubInputDlg::PreTranslateMessage(MSG* pMsg) 
{
	CStringArray strCombo;
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		long nRow = next.row;
		long nCol = next.col;
		// ���� ������
		if(nRow==1 && nCol==1)
		{
			strCombo.Add("����");
			strCombo.Add("1");		strCombo.Add("2");
			strCombo.Add("3");		strCombo.Add("4");
			strCombo.Add("5");		strCombo.Add("6");
			m_Grid.SetComboString(strCombo);
		}
		if(nRow==1 && nCol==2)
		{
			strCombo.Add("���");
			strCombo.Add("����");
			strCombo.Add("�ĸ�");
			m_Grid.SetComboString(strCombo);
		}
		// ������ ��Ŀ
		if(nRow==2)
		{
			strCombo.Add("����");
			strCombo.Add("1");		strCombo.Add("2");
			strCombo.Add("3");		strCombo.Add("4");
			strCombo.Add("5");		strCombo.Add("6");
			m_Grid.SetComboString(strCombo);
		}
		// ���򺸰���
		if((nRow==3 && nCol==2) || (nRow==4 && nCol==2))
		{
			strCombo.Add("����");
			strCombo.Add("1��");
			strCombo.Add("2��");
			m_Grid.SetComboString(strCombo);
		}
		// ������
		if(nRow==5)
		{
			strCombo.Add("����");
			strCombo.Add("��������");
			strCombo.Add("��������");
			strCombo.Add("��������");
			m_Grid.SetComboString(strCombo);
		}
		// ����� ������
		if(nRow==6)
		{
			strCombo.Add("����");
			strCombo.Add("������");
			m_Grid.SetComboString(strCombo);
		}
		// ä����
		if(nRow==7)
		{
			strCombo.Add("����");
			strCombo.Add("���ʸ�");
			strCombo.Add("���ʸ�");
			m_Grid.SetComboString(strCombo);
		}
		// ���Ʈ ������
		if(nRow==8)
		{
			strCombo.Add("����");
			strCombo.Add("������");
			m_Grid.SetComboString(strCombo);
		}
		// Ŀ�ؼ� �÷���Ʈ
		if(nRow==9)
		{
			strCombo.Add("������");
			strCombo.Add("����������");
			strCombo.Add("��������");
			m_Grid.SetComboString(strCombo);
		}
		// ��Ʈ����
		if(nRow==10 && nCol==1)
		{
			//	Bolt Size
			CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
			CPlateCrossBeam		*pC		= m_pCrossBeam;
			CPlateBasicIndex	*pBx	= pC->GetBx();
			CSteelBolt		*pSteelBolt	= pDB->m_pDataManage->GetSteelBolt();

			CString szBoltGrade = pDB->GetMaterial(CG_BOLT, pBx);

			for(long nIdx=0; nIdx<pSteelBolt->GetSize(); nIdx++)
			{
				if(pSteelBolt->GetStringData(nIdx, BDIB_GRADE) == szBoltGrade)
					strCombo.Add(pSteelBolt->GetStringData(nIdx, BDIB_SIZE));
			}
			m_Grid.SetComboString(strCombo);
		}
		// ��Ʈ�⺻����
		if(nRow==11 && nCol==1)
		{
			strCombo.Add("����");
			strCombo.Add("������");
			m_Grid.SetComboString(strCombo);
		}

		// ���κ���ġ�������
		if(nRow==12)
		{
			strCombo.Add("����");
			strCombo.Add("������");			
			m_Grid.SetComboString(strCombo);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCrossBeamSubInputDlg::SetGridTitle()
{
	long nRows = 13;//070322...���κ���ġ ��� ���� �߰�..
	long nCols = 3;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 95);
	m_Grid.SetColumnWidth(1, 60);
	m_Grid.SetColumnWidth(2, 68);
	m_Grid.ExpandColumnsToFit();

	for(long n=0;n<nRows;n++)			m_Grid.SetCellFixType(n, 0, GVN_ONECELL_COLFIX);
	m_Grid.SetCellFixType(0, 1, GVN_ONECELL_COLFIX);	m_Grid.SetCellFixType(0, 2, GVN_ONECELL_COLFIX);
	m_Grid.SetCellFixType(3, 1, GVN_ONECELL_COLFIX);	m_Grid.SetCellFixType(4, 1, GVN_ONECELL_COLFIX);

	m_Grid.SetTextMatrix(0, 0, "�� ��");
	m_Grid.SetTextMatrix(0, 1, "�� ��");			m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetTextMatrix(1, 0, "���� ������");		m_Grid.SetCellType(1, 1, CT_COMBO);	m_Grid.SetCellType(1, 2, CT_COMBO);
	m_Grid.SetTextMatrix(2, 0, "������ ��Ŀ");		m_Grid.SetCellType(2, 1, CT_COMBO);	m_Grid.SetMergeCol(2, 1, 2);
	m_Grid.SetTextMatrix(3, 0, "���� ������");		m_Grid.SetMergeRow(3, 4, 0);
	m_Grid.SetTextMatrix(3, 1, "���");				m_Grid.SetCellType(3, 2, CT_COMBO);
	m_Grid.SetTextMatrix(4, 1, "�Ϻ�");				m_Grid.SetCellType(4, 2, CT_COMBO);
	m_Grid.SetTextMatrix(5, 0, "������");			m_Grid.SetCellType(5, 1, CT_COMBO);	m_Grid.SetMergeCol(5, 1, 2);
	m_Grid.SetTextMatrix(6, 0, "����� ������");
	m_Grid.SetCellType(6, 1, CT_BUTTON);			m_Grid.SetTextMatrix(6, 1, "�⺻��");
	m_Grid.SetCellType(6, 2, CT_COMBO);	
	m_Grid.SetTextMatrix(7, 0, "ä����");			m_Grid.SetCellType(7, 1, CT_COMBO);	m_Grid.SetMergeCol(7, 1, 2);
	m_Grid.SetTextMatrix(8, 0, "���Ʈ ������");
	m_Grid.SetCellType(8, 1, CT_BUTTON);			m_Grid.SetTextMatrix(8, 1, "�⺻��");
	m_Grid.SetCellType(8, 2, CT_COMBO);
	m_Grid.SetTextMatrix(9, 0, "Ŀ�ؼ� �÷���Ʈ");
	m_Grid.SetCellType(9, 1, CT_BUTTON);			m_Grid.SetTextMatrix(9, 1, "�⺻��");
	m_Grid.SetCellType(9, 2, CT_COMBO);	
	m_Grid.SetTextMatrix(10, 0, "��Ʈ ����");		m_Grid.SetCellType(10,1, CT_COMBO);	m_Grid.SetMergeCol(10,1, 2);
	m_Grid.SetTextMatrix(10, 2, "��Ʈ���̰��");
	m_Grid.SetTextMatrix(11, 0, "��Ʈ �⺻����");	m_Grid.SetCellType(11,1, CT_COMBO);

	m_Grid.SetTextMatrix(12, 0, "��ġ ��� ����");
	m_Grid.SetCellType(12, 1, CT_BUTTON);			m_Grid.SetTextMatrix(12, 1, "�⺻��");
	m_Grid.SetCellType(12, 2, CT_COMBO);	
	
	

	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.ExpandToFit();
}

void CCrossBeamSubInputDlg::SetCellReadOnly(long nRow, long nCol, BOOL bReadOnly)
{
	if(bReadOnly)
	{
		m_Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
		m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
	}
	else
	{
		m_Grid.SetItemBkColour(nRow, nCol, RGB(225, 250, 250));
		m_Grid.SetItemState(nRow, nCol, GVIS_MODIFIED);
	}
}

void CCrossBeamSubInputDlg::SetDataInit()
{
	if(!m_pCrossBeam)	return;

	CString	sText;

	if(m_pCrossBeam->m_nVStiffsu_CR==0)
	{
		m_Grid.SetTextMatrix(1, 1, "����");
		SetCellReadOnly(1, 2, TRUE);
	}
	else
	{
		sText.Format("%d", m_pCrossBeam->m_nVStiffsu_CR);
		m_Grid.SetTextMatrix(1, 1, sText);
		SetCellReadOnly(1, 2, FALSE);
	}
	if(m_pCrossBeam->m_cVSide_CR==0)		m_Grid.SetTextMatrix(1, 2, "���");
	else if(m_pCrossBeam->m_cVSide_CR==1)	m_Grid.SetTextMatrix(1, 2, "����");
	else if(m_pCrossBeam->m_cVSide_CR==2)	m_Grid.SetTextMatrix(1, 2, "�ĸ�");

	if(m_pCrossBeam->m_nAnchorsu_CR==0)	m_Grid.SetTextMatrix(2, 1, "����");
	else								m_Grid.SetTextMatrixFormat(2, 1, "%d", m_pCrossBeam->m_nAnchorsu_CR);

	sText.Format("%d��", m_pCrossBeam->m_nHStiffUppersu_CR);
	if(m_pCrossBeam->m_nHStiffUppersu_CR==0)	m_Grid.SetTextMatrix(3, 2, "����");
	else										m_Grid.SetTextMatrix(3, 2, sText);
	sText.Format("%d��", m_pCrossBeam->m_nHStiffLowersu_CR);
	if(m_pCrossBeam->m_nHStiffLowersu_CR==0)	m_Grid.SetTextMatrix(4, 2, "����");
	else										m_Grid.SetTextMatrix(4, 2, sText);

	if(m_pCrossBeam->m_nLGirSpType==0&&m_pCrossBeam->m_nRGirSpType==0)			m_Grid.SetTextMatrix(5, 1, "����");
	else if(m_pCrossBeam->m_nLGirSpType==1&&m_pCrossBeam->m_nRGirSpType==0)		m_Grid.SetTextMatrix(5, 1, "��������");
	else if(m_pCrossBeam->m_nLGirSpType==0&&m_pCrossBeam->m_nRGirSpType==1)		m_Grid.SetTextMatrix(5, 1, "��������");
	else if(m_pCrossBeam->m_nLGirSpType==1&&m_pCrossBeam->m_nRGirSpType==1)		m_Grid.SetTextMatrix(5, 1, "��������");

	if(m_pCrossBeam->m_cGasset_CR==0)		m_Grid.SetTextMatrix(6, 2, "������");
	else if(m_pCrossBeam->m_cGasset_CR==1)	m_Grid.SetTextMatrix(6, 2, "����");
	if(m_pCrossBeam->GetType()==CR_TYPE_VSTIFF)	SetCellReadOnly(6, 2, TRUE);
	else										SetCellReadOnly(6, 2, FALSE);

	if(m_pCrossBeam->m_nFillerUseType==0)	m_Grid.SetTextMatrix(7, 1, "����");
	else if(m_pCrossBeam->m_nFillerUseType==1)	m_Grid.SetTextMatrix(7, 1, "���ʸ�");
	else if(m_pCrossBeam->m_nFillerUseType==2)	m_Grid.SetTextMatrix(7, 1, "���ʸ�");

	if(m_pCrossBeam->m_nLGirSpType || m_pCrossBeam->m_nRGirSpType)
		SetCellReadOnly(8, 2, FALSE);
	else
		SetCellReadOnly(8, 2, TRUE);
	if(m_pCrossBeam->m_bMomentPan)	m_Grid.SetTextMatrix(8, 2, "����");
	else							m_Grid.SetTextMatrix(8, 2, "������");

	if(!m_pCrossBeam->m_bConnectionPlate)	m_Grid.SetTextMatrix(9, 2, "������");
	else
	{
		if(m_pCrossBeam->m_nRadioConnectionPlateInOut==0)	m_Grid.SetTextMatrix(9, 2, "����������");
		else												m_Grid.SetTextMatrix(9, 2, "��������");
	}	

	m_Grid.SetTextMatrix(10, 1, m_pCrossBeam->m_ZzsBoltSize);

	if(m_pCrossBeam->m_bBoltInterver)
	{
		m_Grid.SetTextMatrix(11, 1, "����");
		SetCellReadOnly(11, 2, FALSE);
	}
	else
	{
		m_Grid.SetTextMatrix(11, 1, "������");
		SetCellReadOnly(11, 2, TRUE);
	}

	m_Grid.SetTextMatrix(11, 2, "%.0f", m_pCrossBeam->m_dBoltInterver);
		
	
	//���κ� ��ġ ��� ���� ����...
	if(m_pCrossBeam->m_bHunchTopConnect)	m_Grid.SetTextMatrix(12, 2, "����");
	else									m_Grid.SetTextMatrix(12, 2, "������");
	

	if(m_pCrossBeam->GetType()==CR_TYPE_VSTIFF)//���������� ���� Ÿ���̸� 
		SetCellReadOnly(12, 2, TRUE);
	else if(m_pCrossBeam->m_nLevelType == 0)//�ܸ��ġ �����̸�..
		SetCellReadOnly(12, 2, TRUE);
	else
		SetCellReadOnly(12, 2, FALSE);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CCrossBeamSubInputDlg::SetDataSave()
{
	CString	sText;

	sText	= m_Grid.GetTextMatrix(1, 1);
	if(sText=="����")
		m_pCrossBeam->m_nVStiffsu_CR = 0;
	else
	{
		sText = m_Grid.GetTextMatrix(1, 1);
		m_pCrossBeam->m_nVStiffsu_CR = atoi(sText);
		sText = m_Grid.GetTextMatrix(1, 2);
		if(sText=="���")		m_pCrossBeam->m_cVSide_CR = 0;
		else if(sText=="����")	m_pCrossBeam->m_cVSide_CR = 1;
		else if(sText=="�ĸ�")	m_pCrossBeam->m_cVSide_CR = 2;
	}

	m_pCrossBeam->m_nAnchorsu_CR		= GetlongValue(m_Grid.GetTextMatrix(2, 1));
	m_pCrossBeam->m_nHStiffUppersu_CR	= GetlongValue(m_Grid.GetTextMatrix(3, 2));
	m_pCrossBeam->m_nHStiffLowersu_CR	= GetlongValue(m_Grid.GetTextMatrix(4, 2));

	sText = m_Grid.GetTextMatrix(5, 1);
	if(sText=="����")
	{
		m_pCrossBeam->m_nLGirSpType = 0;
		m_pCrossBeam->m_nRGirSpType = 0;
	}
	else if(sText=="��������")
	{
		m_pCrossBeam->m_nLGirSpType = 1;
		m_pCrossBeam->m_nRGirSpType = 0;
	}
	else if(sText=="��������")
	{
		m_pCrossBeam->m_nLGirSpType = 0;
		m_pCrossBeam->m_nRGirSpType = 1;
	}
	else if(sText=="��������")
	{
		m_pCrossBeam->m_nLGirSpType = 1;
		m_pCrossBeam->m_nRGirSpType = 1;
	}

	m_pCrossBeam->m_cGasset_CR = GetBoolValue(m_Grid.GetTextMatrix(6, 2));

	sText = m_Grid.GetTextMatrix(7, 1);
	if(sText=="����")			m_pCrossBeam->m_nFillerUseType = 0;
	else if(sText=="���ʸ�")	m_pCrossBeam->m_nFillerUseType = 1;
	else if(sText=="���ʸ�")	m_pCrossBeam->m_nFillerUseType = 2;

	m_pCrossBeam->m_bMomentPan = GetBoolValue(m_Grid.GetTextMatrix(8, 2));

	sText = m_Grid.GetTextMatrix(9, 2);
	if(sText=="������")	m_pCrossBeam->m_bConnectionPlate = FALSE;
	else
	{
		m_pCrossBeam->m_bConnectionPlate = TRUE;
		if(sText=="����������")		m_pCrossBeam->m_nRadioConnectionPlateInOut = 0;
		else if(sText=="��������")	m_pCrossBeam->m_nRadioConnectionPlateInOut = 1;
	}

	m_pCrossBeam->m_ZzsBoltSize = m_Grid.GetTextMatrix(10, 1);
	m_pCrossBeam->m_bBoltInterver = GetBoolValue(m_Grid.GetTextMatrix(11, 1));
	m_pCrossBeam->m_dBoltInterver = m_Grid.GetTextMatrixDouble(11, 2);

	m_pCrossBeam->m_bHunchTopConnect = GetBoolValue(m_Grid.GetTextMatrix(12, 2));
}

void CCrossBeamSubInputDlg::OnCellChangedData(NMHDR* pNMHDR, LRESULT* pResult)
{
	CGeneralBaseDlg *pDlg = m_pStd->GetCurruntDlg();
	NM_GRIDVIEW *pCell = (NM_GRIDVIEW *)pNMHDR;

	long	nRow	= pCell->iRow;

	SetDataSave();
	SetDataInit();
	if(pDlg)
		pDlg->SendMessage(WM_DATA_CHANGED, (WPARAM)m_pCrossBeam, nRow);
}

long CCrossBeamSubInputDlg::GetlongValue(CString szValue)
{
	long nValue = 0;
	if(szValue=="����")	nValue = 0;
	else nValue = atoi(szValue);

	return nValue;
}

BOOL CCrossBeamSubInputDlg::GetBoolValue(CString szValue)
{
	if(szValue=="����")	return TRUE;
	else				return FALSE;
}
void CCrossBeamSubInputDlg::OnCellButtonClickRecommend(NMHDR* pNMHDR, LRESULT* pResult)
{
	CGeneralBaseDlg		*pDlg	= m_pStd->GetCurruntDlg();
	NM_GRIDVIEW			*pCell	= (NM_GRIDVIEW *)pNMHDR;

	long	nRow	= pCell->iRow;
	long	nCol	= pCell->iColumn;

	if(!m_pCrossBeam)	return;
	if(nRow==6 && nCol==1)
	{
		if(m_pCrossBeam->m_cGasset_CR)	RecommendBracketHeight();
		else
		{
			if(AfxMessageBox("����� �������� ������� �ʾҽ��ϴ�. ����� �������� ��ġ�Ͻðڽ��ϱ�?", MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				m_pCrossBeam->m_cGasset_CR	= 1;
				RecommendBracketHeight();
			}
		}
	}
	if(nRow==8 && nCol==1)
	{
		if(m_pCrossBeam->m_bMomentPan)	RecommendMoment();
		else
		{
			if(AfxMessageBox("���Ʈ �������� ������� �ʾҽ��ϴ�. ���Ʈ �������� ��ġ�Ͻðڽ��ϱ�?", MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				m_pCrossBeam->m_bMomentPan	=  TRUE;
				RecommendMoment();
			}
		}
	}
	if(nRow==9 && nCol==1)
	{
		if(m_pCrossBeam->m_bConnectionPlate)	RecommendConnectionPlate();
		else
		{
			if(AfxMessageBox("Ŀ�ؼ��÷���Ʈ�� ������� �ʾҽ��ϴ�. Ŀ�ؼ��÷���Ʈ�� ��ġ�Ͻðڽ��ϱ�?", MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				m_pCrossBeam->m_bConnectionPlate	= TRUE;
				RecommendConnectionPlate();
			}
		}
	}

	if(nRow==12 && nCol==1)
	{
		//�⺻���� �������� �ش� ���κ����� �������� ���������� ó��...		
		//���������� ���� Ÿ���̾ƴϰ� �ܸ��ġ ������ �ƴϸ�..
		if(m_pCrossBeam->GetType()!=CR_TYPE_VSTIFF && m_pCrossBeam->m_nLevelType != 0)
			RecommendHunchTopConnect();	
	}
	SetDataInit();

	if(pDlg) pDlg->SendMessage(WM_DATA_CHANGED, (WPARAM)m_pCrossBeam, 1);
}

void CCrossBeamSubInputDlg::RecommendBracketHeight()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateCrossBeam		*pC		= m_pCrossBeam;
	CPlateBasicIndex	*pBx	= pC->GetBx();
	CPlateGirderApp		*pGir	= pDB->GetGirder(pBx->GetNumberGirder());
	CPlateGirderApp		*pGirR	= pDB->GetGirder(pBx->GetNumberGirder()+1);
	CPlateBasicIndex	*pBxR	= pBx->GetBxMatchByCrossBeam(pGirR);

	double	dStation	= pBx->GetStation();
	double	dStationR	= pBxR->GetStation();
	double	dHeightGir  = pGir->GetHeightGirderByStaAng(dStation);
	double	dHeightGirR = pGirR->GetHeightGirderByStaAng(dStationR);

	//����������ǰ��
	double	dSharp		= pGir->GetLengthSharpOnBx(pBx, TRUE);
	CDPoint	vDir		= pC->GetVectorCross();
	double	dGussetH	= pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	pC->m_dltH    = dHeightGir-pC->m_T1_CR-pC->m_T2_CR-pC->m_dLH1-pC->m_H_CR+pC->GetLengthGirWebToCrWeb(TRUE)*vDir.y-dGussetH;
	if(pC->m_dLH1==0)
		pC->m_dltH = dHeightGir-pC->m_T1_CR-pC->m_H_CR-(pC->m_dbL2+pC->m_dbL3+pC->m_dbL1)*vDir.y-dGussetH;
	pC->m_ltF = 100;	
	pC->m_ltE = (long)((pC->m_dltH-2*100)/pC->m_ltF);
	//����������ǰ��
	dSharp = pGirR->GetLengthSharpOnBx(pBxR, TRUE);
	vDir  = pC->GetVectorCross();
	pC->m_drtH    = dHeightGir-pC->m_T1_CR-pC->m_T2_CR-pC->m_dRH1-pC->m_H_CR-pC->GetLengthGirWebToCrWeb(FALSE)*vDir.y-dGussetH;
	if(pC->m_dRH1==0)
		pC->m_drtH = dHeightGirR-pC->m_T1_CR-pC->m_H_CR+(pC->m_dbR2+pC->m_dbR3+pC->m_dbR1)*vDir.y-dGussetH;
	pC->m_rtF = 100;	
	pC->m_rtE = (long)((pC->m_drtH-2*100)/pC->m_rtF);
}

void CCrossBeamSubInputDlg::RecommendMoment()
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CPlateBasicIndex	*pBx		= m_pCrossBeam->GetBx();
	CSteelBolt			*pBolt		= m_pStd->GetDataManage()->GetSteelBolt();
	CPlateCrossBeam		*pC			= m_pCrossBeam;

	// ���� �����Ǹ� �ٽ� ����
	double Height	  = pC->m_H_CR;
	double PlateSpace = 15; // ���Ʈ�ǰ� �������� ����

    CString str;
	str.Format("%s %s", pDB->GetMaterial(CG_BOLT, pC->GetBx()), pC->m_ZzsBoltSize);
	long idx = pBolt->GetIndex(str);

	pC->m_mA	= pBolt->GetCalcEdgeDistance(idx);
	pC->m_mB	=  2;
	pC->m_mC	= 85;
	pC->m_mC1	= 85;
	pC->m_mG	= 50;
	pC->m_mD	= pC->m_mG + pC->m_mA;
	pC->m_mE	=  2;
	pC->m_mF	= 100;
	pC->m_mW	= (pC->m_mA*2 + pC->m_mE*pC->m_mF);

	pC->m_sF	= 100;
	pC->m_sE	= (long)((Height - (pC->m_mG+pC->m_mW+PlateSpace)*2)/pC->m_sF-1);
	pC->m_sW	= pC->m_sE*pC->m_sF+pC->m_sA*2;

	pC->m_mT	= pC->m_sT;
	double  T4  = pC->m_mT*2+pC->m_T3_CR;	// ����
	pC->m_mL = pBolt->GetBoltLength(pC->GetBx(), CG_BOLT, pC->m_ZzsBoltSize,T4);
}

//���κ� ��ġ �������..
//070322...KB...
//�����   1. LH,RH = 0��...,		2.LH, RH ��Ȱ��ȭ...3.������ �Ϲݵ� ���κ� ��ġ ����..
//������� 1. LH,RH = �ǰ�Ȱ�...,	2.LH, RH Ȱ��ȭ...	3.������ �Ϲݵ� ���κ� ��ġ ������..
//�ǰ�Ȱ�...�ܺ�,������ ����, �Ϲݺ� ������...
void CCrossBeamSubInputDlg::RecommendHunchTopConnect()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateCrossBeam		*pC		= m_pCrossBeam;
	CPlateBasicIndex	*pBx	= pC->GetBx();

	BOOL bCurrent	= m_pCrossBeam->m_bHunchTopConnect;//���� ����/������ ����...

	if(pC->GetType()==CR_TYPE_VSTIFF)//���������� ���� Ÿ���̸� 
		pC->m_bHunchTopConnect = FALSE;
	else if(pC->m_nLevelType == 0)//�ܸ��ġ �����̸�..
		pC->m_bHunchTopConnect = FALSE;
	else if (pBx->IsJijum() && !pDB->IsTUGir())//�ܺ�,�������̸�..
		pC->m_bHunchTopConnect = TRUE;
	else
		pC->m_bHunchTopConnect = FALSE;

	if (bCurrent != m_pCrossBeam->m_bHunchTopConnect)
		SetDataInit();
}
void CCrossBeamSubInputDlg::RecommendConnectionPlate()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateCrossBeam	*pC		= m_pCrossBeam;
	CSteelBolt		*pBolt	= m_pStd->GetDataManage()->GetSteelBolt();

	CString str;
	str.Format("%s %s", pDB->GetMaterial(CG_BOLT, pC->GetBx()), pC->m_ZzsBoltSize);
	long nIdx = pBolt->GetIndex(str);
	double dEdge	= pBolt->GetCalcEdgeDistance(nIdx);

	if(pC->GetType() == CR_TYPE_MANUFACTURE)
	{
		pC->m_P_CR	= 600;		pC->m_vW_CR	= 250;
		pC->m_vW1_G	= 0;		pC->m_uW	= 240;
		pC->m_uM1	= 280;		pC->m_uN_CR	= 100;
		pC->m_uE1	= 2;		pC->m_uF1	= 80;
		pC->m_uD1	= 40;		pC->m_uA	= dEdge;
		pC->m_uB1	= 2;		pC->m_uC	= 80;
		pC->m_uC1	= 100;		pC->m_uB	= 2;
		pC->m_uM_CR	= 200;		pC->m_uE	= 1;
		pC->m_uF	= 80;		pC->m_uD	= 40;

		pC->m_dW	= 240;		pC->m_dM1	= 280;
		pC->m_dN_CR	= 100;		pC->m_dE1	= 2;
		pC->m_dF1	= 80;		pC->m_dD1	= 40;
		pC->m_dA	= dEdge;	pC->m_dB1	= 2;
		pC->m_dC	= 80;		pC->m_dC1	= 100;
		pC->m_dB	= 2;		pC->m_dM_CR	= 200;
		pC->m_dE	= 1;		pC->m_dF	= 80;
		pC->m_dD	= 40;
	}
	else if(pC->GetType() == CR_TYPE_VSTIFF)
	{
		pC->m_P_CR	= 250;		pC->m_vW_CR	= 250;
		pC->m_vW1_G	= 0;		pC->m_uW	= 240;
		pC->m_uM1	= 280;		pC->m_uN_CR	= 100;
		pC->m_uE1	= 2;		pC->m_uF1	= 80;
		pC->m_uD1	= 40;		pC->m_uA	= dEdge;
		pC->m_uB1	= 1;		pC->m_uC	= 80;
		pC->m_uC1	= 100;		pC->m_uB	= 2;
		pC->m_uM_CR	= 200;		pC->m_uE	= 1;
		pC->m_uF	= 80;		pC->m_uD	= 40;

		pC->m_dW	= 240;		pC->m_dM1	= 280;
		pC->m_dN_CR	= 100;		pC->m_dE1	= 2;
		pC->m_dF1	= 80;		pC->m_dD1	= 40;
		pC->m_dA	= dEdge;	pC->m_dB1	= 1;
		pC->m_dC	= 80;		pC->m_dC1	= 100;
		pC->m_dB	= 2;		pC->m_dM_CR	= 200;
		pC->m_dE	= 1;		pC->m_dF	= 80;
		pC->m_dD	= 40;
	}
	else if(pC->GetType() == CR_TYPE_HSTEEL)
		pC->m_bConnectionPlate = FALSE;
}

void CCrossBeamSubInputDlg::OnClose() 
{
	CGeneralBaseDlg *pDlg = m_pStd->GetCurruntDlg();
	if(pDlg) pDlg->SendMessage(WM_SUBINPUT_CLOSE);
	
	CDialog::OnClose();
}
