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
		// 수직 보강재
		if(nRow==1 && nCol==1)
		{
			strCombo.Add("없음");
			strCombo.Add("1");		strCombo.Add("2");
			strCombo.Add("3");		strCombo.Add("4");
			strCombo.Add("5");		strCombo.Add("6");
			m_Grid.SetComboString(strCombo);
		}
		if(nRow==1 && nCol==2)
		{
			strCombo.Add("양면");
			strCombo.Add("전면");
			strCombo.Add("후면");
			m_Grid.SetComboString(strCombo);
		}
		// 슬래브 앵커
		if(nRow==2)
		{
			strCombo.Add("없음");
			strCombo.Add("1");		strCombo.Add("2");
			strCombo.Add("3");		strCombo.Add("4");
			strCombo.Add("5");		strCombo.Add("6");
			m_Grid.SetComboString(strCombo);
		}
		// 수평보강재
		if((nRow==3 && nCol==2) || (nRow==4 && nCol==2))
		{
			strCombo.Add("없음");
			strCombo.Add("1단");
			strCombo.Add("2단");
			m_Grid.SetComboString(strCombo);
		}
		// 이음판
		if(nRow==5)
		{
			strCombo.Add("없음");
			strCombo.Add("좌측적용");
			strCombo.Add("우측적용");
			strCombo.Add("양측적용");
			m_Grid.SetComboString(strCombo);
		}
		// 브라켓 보강판
		if(nRow==6)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");
			m_Grid.SetComboString(strCombo);
		}
		// 채움판
		if(nRow==7)
		{
			strCombo.Add("없음");
			strCombo.Add("한쪽면");
			strCombo.Add("양쪽면");
			m_Grid.SetComboString(strCombo);
		}
		// 모멘트 이음판
		if(nRow==8)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");
			m_Grid.SetComboString(strCombo);
		}
		// 커넥션 플레이트
		if(nRow==9)
		{
			strCombo.Add("미적용");
			strCombo.Add("내외측적용");
			strCombo.Add("외측적용");
			m_Grid.SetComboString(strCombo);
		}
		// 볼트설정
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
		// 볼트기본간격
		if(nRow==11 && nCol==1)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");
			m_Grid.SetComboString(strCombo);
		}

		// 가로보헌치상면접합
		if(nRow==12)
		{
			strCombo.Add("적용");
			strCombo.Add("미적용");			
			m_Grid.SetComboString(strCombo);
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCrossBeamSubInputDlg::SetGridTitle()
{
	long nRows = 13;//070322...가로보헌치 상면 접합 추가..
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

	m_Grid.SetTextMatrix(0, 0, "항 목");
	m_Grid.SetTextMatrix(0, 1, "설 정");			m_Grid.SetMergeCol(0, 1, 2);
	m_Grid.SetTextMatrix(1, 0, "수직 보강재");		m_Grid.SetCellType(1, 1, CT_COMBO);	m_Grid.SetCellType(1, 2, CT_COMBO);
	m_Grid.SetTextMatrix(2, 0, "슬래브 앵커");		m_Grid.SetCellType(2, 1, CT_COMBO);	m_Grid.SetMergeCol(2, 1, 2);
	m_Grid.SetTextMatrix(3, 0, "수평 보강재");		m_Grid.SetMergeRow(3, 4, 0);
	m_Grid.SetTextMatrix(3, 1, "상부");				m_Grid.SetCellType(3, 2, CT_COMBO);
	m_Grid.SetTextMatrix(4, 1, "하부");				m_Grid.SetCellType(4, 2, CT_COMBO);
	m_Grid.SetTextMatrix(5, 0, "이음판");			m_Grid.SetCellType(5, 1, CT_COMBO);	m_Grid.SetMergeCol(5, 1, 2);
	m_Grid.SetTextMatrix(6, 0, "브라켓 보강판");
	m_Grid.SetCellType(6, 1, CT_BUTTON);			m_Grid.SetTextMatrix(6, 1, "기본값");
	m_Grid.SetCellType(6, 2, CT_COMBO);	
	m_Grid.SetTextMatrix(7, 0, "채움판");			m_Grid.SetCellType(7, 1, CT_COMBO);	m_Grid.SetMergeCol(7, 1, 2);
	m_Grid.SetTextMatrix(8, 0, "모멘트 이음판");
	m_Grid.SetCellType(8, 1, CT_BUTTON);			m_Grid.SetTextMatrix(8, 1, "기본값");
	m_Grid.SetCellType(8, 2, CT_COMBO);
	m_Grid.SetTextMatrix(9, 0, "커넥션 플레이트");
	m_Grid.SetCellType(9, 1, CT_BUTTON);			m_Grid.SetTextMatrix(9, 1, "기본값");
	m_Grid.SetCellType(9, 2, CT_COMBO);	
	m_Grid.SetTextMatrix(10, 0, "볼트 설정");		m_Grid.SetCellType(10,1, CT_COMBO);	m_Grid.SetMergeCol(10,1, 2);
	m_Grid.SetTextMatrix(10, 2, "볼트길이계산");
	m_Grid.SetTextMatrix(11, 0, "볼트 기본간격");	m_Grid.SetCellType(11,1, CT_COMBO);

	m_Grid.SetTextMatrix(12, 0, "헌치 상면 접합");
	m_Grid.SetCellType(12, 1, CT_BUTTON);			m_Grid.SetTextMatrix(12, 1, "기본값");
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
		m_Grid.SetTextMatrix(1, 1, "없음");
		SetCellReadOnly(1, 2, TRUE);
	}
	else
	{
		sText.Format("%d", m_pCrossBeam->m_nVStiffsu_CR);
		m_Grid.SetTextMatrix(1, 1, sText);
		SetCellReadOnly(1, 2, FALSE);
	}
	if(m_pCrossBeam->m_cVSide_CR==0)		m_Grid.SetTextMatrix(1, 2, "양면");
	else if(m_pCrossBeam->m_cVSide_CR==1)	m_Grid.SetTextMatrix(1, 2, "전면");
	else if(m_pCrossBeam->m_cVSide_CR==2)	m_Grid.SetTextMatrix(1, 2, "후면");

	if(m_pCrossBeam->m_nAnchorsu_CR==0)	m_Grid.SetTextMatrix(2, 1, "없음");
	else								m_Grid.SetTextMatrixFormat(2, 1, "%d", m_pCrossBeam->m_nAnchorsu_CR);

	sText.Format("%d단", m_pCrossBeam->m_nHStiffUppersu_CR);
	if(m_pCrossBeam->m_nHStiffUppersu_CR==0)	m_Grid.SetTextMatrix(3, 2, "없음");
	else										m_Grid.SetTextMatrix(3, 2, sText);
	sText.Format("%d단", m_pCrossBeam->m_nHStiffLowersu_CR);
	if(m_pCrossBeam->m_nHStiffLowersu_CR==0)	m_Grid.SetTextMatrix(4, 2, "없음");
	else										m_Grid.SetTextMatrix(4, 2, sText);

	if(m_pCrossBeam->m_nLGirSpType==0&&m_pCrossBeam->m_nRGirSpType==0)			m_Grid.SetTextMatrix(5, 1, "없음");
	else if(m_pCrossBeam->m_nLGirSpType==1&&m_pCrossBeam->m_nRGirSpType==0)		m_Grid.SetTextMatrix(5, 1, "좌측적용");
	else if(m_pCrossBeam->m_nLGirSpType==0&&m_pCrossBeam->m_nRGirSpType==1)		m_Grid.SetTextMatrix(5, 1, "우측적용");
	else if(m_pCrossBeam->m_nLGirSpType==1&&m_pCrossBeam->m_nRGirSpType==1)		m_Grid.SetTextMatrix(5, 1, "양측적용");

	if(m_pCrossBeam->m_cGasset_CR==0)		m_Grid.SetTextMatrix(6, 2, "미적용");
	else if(m_pCrossBeam->m_cGasset_CR==1)	m_Grid.SetTextMatrix(6, 2, "적용");
	if(m_pCrossBeam->GetType()==CR_TYPE_VSTIFF)	SetCellReadOnly(6, 2, TRUE);
	else										SetCellReadOnly(6, 2, FALSE);

	if(m_pCrossBeam->m_nFillerUseType==0)	m_Grid.SetTextMatrix(7, 1, "없음");
	else if(m_pCrossBeam->m_nFillerUseType==1)	m_Grid.SetTextMatrix(7, 1, "한쪽면");
	else if(m_pCrossBeam->m_nFillerUseType==2)	m_Grid.SetTextMatrix(7, 1, "양쪽면");

	if(m_pCrossBeam->m_nLGirSpType || m_pCrossBeam->m_nRGirSpType)
		SetCellReadOnly(8, 2, FALSE);
	else
		SetCellReadOnly(8, 2, TRUE);
	if(m_pCrossBeam->m_bMomentPan)	m_Grid.SetTextMatrix(8, 2, "적용");
	else							m_Grid.SetTextMatrix(8, 2, "미적용");

	if(!m_pCrossBeam->m_bConnectionPlate)	m_Grid.SetTextMatrix(9, 2, "미적용");
	else
	{
		if(m_pCrossBeam->m_nRadioConnectionPlateInOut==0)	m_Grid.SetTextMatrix(9, 2, "내외측적용");
		else												m_Grid.SetTextMatrix(9, 2, "외측적용");
	}	

	m_Grid.SetTextMatrix(10, 1, m_pCrossBeam->m_ZzsBoltSize);

	if(m_pCrossBeam->m_bBoltInterver)
	{
		m_Grid.SetTextMatrix(11, 1, "적용");
		SetCellReadOnly(11, 2, FALSE);
	}
	else
	{
		m_Grid.SetTextMatrix(11, 1, "미적용");
		SetCellReadOnly(11, 2, TRUE);
	}

	m_Grid.SetTextMatrix(11, 2, "%.0f", m_pCrossBeam->m_dBoltInterver);
		
	
	//가로보 헌치 상면 접합 여부...
	if(m_pCrossBeam->m_bHunchTopConnect)	m_Grid.SetTextMatrix(12, 2, "적용");
	else									m_Grid.SetTextMatrix(12, 2, "미적용");
	

	if(m_pCrossBeam->GetType()==CR_TYPE_VSTIFF)//수직보강재 연결 타입이면 
		SetCellReadOnly(12, 2, TRUE);
	else if(m_pCrossBeam->m_nLevelType == 0)//단면배치 수평이면..
		SetCellReadOnly(12, 2, TRUE);
	else
		SetCellReadOnly(12, 2, FALSE);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CCrossBeamSubInputDlg::SetDataSave()
{
	CString	sText;

	sText	= m_Grid.GetTextMatrix(1, 1);
	if(sText=="없음")
		m_pCrossBeam->m_nVStiffsu_CR = 0;
	else
	{
		sText = m_Grid.GetTextMatrix(1, 1);
		m_pCrossBeam->m_nVStiffsu_CR = atoi(sText);
		sText = m_Grid.GetTextMatrix(1, 2);
		if(sText=="양면")		m_pCrossBeam->m_cVSide_CR = 0;
		else if(sText=="전면")	m_pCrossBeam->m_cVSide_CR = 1;
		else if(sText=="후면")	m_pCrossBeam->m_cVSide_CR = 2;
	}

	m_pCrossBeam->m_nAnchorsu_CR		= GetlongValue(m_Grid.GetTextMatrix(2, 1));
	m_pCrossBeam->m_nHStiffUppersu_CR	= GetlongValue(m_Grid.GetTextMatrix(3, 2));
	m_pCrossBeam->m_nHStiffLowersu_CR	= GetlongValue(m_Grid.GetTextMatrix(4, 2));

	sText = m_Grid.GetTextMatrix(5, 1);
	if(sText=="없음")
	{
		m_pCrossBeam->m_nLGirSpType = 0;
		m_pCrossBeam->m_nRGirSpType = 0;
	}
	else if(sText=="좌측적용")
	{
		m_pCrossBeam->m_nLGirSpType = 1;
		m_pCrossBeam->m_nRGirSpType = 0;
	}
	else if(sText=="우측적용")
	{
		m_pCrossBeam->m_nLGirSpType = 0;
		m_pCrossBeam->m_nRGirSpType = 1;
	}
	else if(sText=="양측적용")
	{
		m_pCrossBeam->m_nLGirSpType = 1;
		m_pCrossBeam->m_nRGirSpType = 1;
	}

	m_pCrossBeam->m_cGasset_CR = GetBoolValue(m_Grid.GetTextMatrix(6, 2));

	sText = m_Grid.GetTextMatrix(7, 1);
	if(sText=="없음")			m_pCrossBeam->m_nFillerUseType = 0;
	else if(sText=="한쪽면")	m_pCrossBeam->m_nFillerUseType = 1;
	else if(sText=="양쪽면")	m_pCrossBeam->m_nFillerUseType = 2;

	m_pCrossBeam->m_bMomentPan = GetBoolValue(m_Grid.GetTextMatrix(8, 2));

	sText = m_Grid.GetTextMatrix(9, 2);
	if(sText=="미적용")	m_pCrossBeam->m_bConnectionPlate = FALSE;
	else
	{
		m_pCrossBeam->m_bConnectionPlate = TRUE;
		if(sText=="내외측적용")		m_pCrossBeam->m_nRadioConnectionPlateInOut = 0;
		else if(sText=="외측적용")	m_pCrossBeam->m_nRadioConnectionPlateInOut = 1;
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
	if(szValue=="없음")	nValue = 0;
	else nValue = atoi(szValue);

	return nValue;
}

BOOL CCrossBeamSubInputDlg::GetBoolValue(CString szValue)
{
	if(szValue=="적용")	return TRUE;
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
			if(AfxMessageBox("브라켓 보강판이 적용되지 않았습니다. 브라켓 보강판을 설치하시겠습니까?", MB_YESNO | MB_ICONQUESTION)==IDYES)
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
			if(AfxMessageBox("모멘트 이음판이 적용되지 않았습니다. 모멘트 이음판을 설치하시겠습니까?", MB_YESNO | MB_ICONQUESTION)==IDYES)
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
			if(AfxMessageBox("커넥션플레이트가 적용되지 않았습니다. 커넥션플레이트를 설치하시겠습니까?", MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				m_pCrossBeam->m_bConnectionPlate	= TRUE;
				RecommendConnectionPlate();
			}
		}
	}

	if(nRow==12 && nCol==1)
	{
		//기본값이 눌렸을땐 해당 가로보에서 적용인지 비적용인지 처리...		
		//수직보강재 연결 타입이아니고 단면배치 수평이 아니면..
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

	//좌측보강재권고안
	double	dSharp		= pGir->GetLengthSharpOnBx(pBx, TRUE);
	CDPoint	vDir		= pC->GetVectorCross();
	double	dGussetH	= pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	pC->m_dltH    = dHeightGir-pC->m_T1_CR-pC->m_T2_CR-pC->m_dLH1-pC->m_H_CR+pC->GetLengthGirWebToCrWeb(TRUE)*vDir.y-dGussetH;
	if(pC->m_dLH1==0)
		pC->m_dltH = dHeightGir-pC->m_T1_CR-pC->m_H_CR-(pC->m_dbL2+pC->m_dbL3+pC->m_dbL1)*vDir.y-dGussetH;
	pC->m_ltF = 100;	
	pC->m_ltE = (long)((pC->m_dltH-2*100)/pC->m_ltF);
	//우측보강재권고안
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

	// 복부 이음판만 다시 설정
	double Height	  = pC->m_H_CR;
	double PlateSpace = 15; // 모멘트판과 전단판의 간격

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
	double  T4  = pC->m_mT*2+pC->m_T3_CR;	// 복부
	pC->m_mL = pBolt->GetBoltLength(pC->GetBx(), CG_BOLT, pC->m_ZzsBoltSize,T4);
}

//가로보 헌치 상면접합..
//070322...KB...
//적용시   1. LH,RH = 0값...,		2.LH, RH 비활성화...3.슬래브 일반도 가로보 헌치 적용..
//비적용시 1. LH,RH = 권고안값...,	2.LH, RH 활성화...	3.슬래브 일반도 가로보 헌치 미적용..
//권고안값...단부,지점부 적용, 일반부 미적용...
void CCrossBeamSubInputDlg::RecommendHunchTopConnect()
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateCrossBeam		*pC		= m_pCrossBeam;
	CPlateBasicIndex	*pBx	= pC->GetBx();

	BOOL bCurrent	= m_pCrossBeam->m_bHunchTopConnect;//현재 적용/비적용 여부...

	if(pC->GetType()==CR_TYPE_VSTIFF)//수직보강재 연결 타입이면 
		pC->m_bHunchTopConnect = FALSE;
	else if(pC->m_nLevelType == 0)//단면배치 수평이면..
		pC->m_bHunchTopConnect = FALSE;
	else if (pBx->IsJijum() && !pDB->IsTUGir())//단부,지점부이면..
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
