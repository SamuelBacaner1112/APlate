// DeckCrossStringerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckCrossStringerDlg dialog


CDeckCrossStringerDlg::CDeckCrossStringerDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckCrossStringerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckCrossStringerDlg)
	m_bMakeC3 = FALSE;
	m_bMakeS3 = FALSE;
	m_bMakeOB3 = FALSE;
	m_bShowSlabHunch = FALSE;
	//}}AFX_DATA_INIT
}


void CDeckCrossStringerDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckCrossStringerDlg)
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Check(pDX, IDC_CHECK_C3, m_bMakeC3);
	DDX_Check(pDX, IDC_CHECK_S3, m_bMakeS3);
	DDX_Check(pDX, IDC_CHECK_OB3, m_bMakeOB3);
	DDX_Check(pDX, IDC_CHECK_SHOW_SLABHUNCH, m_bShowSlabHunch);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDel);
}


BEGIN_MESSAGE_MAP(CDeckCrossStringerDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckCrossStringerDlg)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)	
 	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDeleteRebar)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAddRebar)
	ON_BN_CLICKED(IDC_CHECK_C3, OnCheckC3)
	ON_BN_CLICKED(IDC_CHECK_S3, OnCheckS3)
	ON_BN_CLICKED(IDC_CHECK_OB3, OnCheckOB3)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SLABHUNCH, OnCheckShowSlabHunch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckCrossStringerDlg message handlers

void CDeckCrossStringerDlg::OnPreInitDialog()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;
	
	m_ctlTab.DeleteAllItems();
	
	m_ctlTab.InsertItem(0, "�����Է�");

	if(pDeckData->IsBridgeHasCrossBeamHunch())
		m_ctlTab.InsertItem(1, "���κ��� ����ö��");

	if(pDeckData->IsBridgeHasStringer())
		m_ctlTab.InsertItem(2, "���κ��� ����ö��");

	if(pDeckData->IsBridgeHasOutBracket())
		m_ctlTab.InsertItem(3, "�������κ��� ����ö��");

	m_ctlTab.SetCurSel(0);

	SetResize(IDC_CHECK_SHOW_SLABHUNCH, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON_ADD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DEL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_C3, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_S3, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_OB3, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}


void CDeckCrossStringerDlg::SetGridTitle()
{
	if(GetCurTabName()=="�����Է�")
		SetGridTitleBasic();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetGridTitleCrSangse();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetGridTitleStSangse();
	else if(GetCurTabName()=="�������κ��� ����ö��")
		SetGridTitleObkSangse();

}


void CDeckCrossStringerDlg::SetGridTitleBasic()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	m_Grid.SetRowCount(3);
	m_Grid.SetColumnCount(18);
	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.SetFixedRowCount(2);	

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);		
	m_Grid.SetColumnWidth(5, 130);
	m_Grid.SetColumnWidth(6, 100);
	m_Grid.SetColumnWidth(11, 100);
	m_Grid.SetColumnWidth(16, 130);
	m_Grid.SetColumnWidth(17, 100);
		
	m_Grid.SetTextMatrix(0, 0, "����");
	m_Grid.SetMergeRow(0, 1, 0);
	m_Grid.SetTextMatrix(2, 0, "ö��");
	
	m_Grid.SetTextMatrix(0, 1, "���κ� ����ö��");
	m_Grid.SetMergeCol(0, 1, 6);
	m_Grid.SetTextMatrix(0, 7, "���κ� ����ö��");
	m_Grid.SetMergeCol(0, 7, 11);
	m_Grid.SetTextMatrix(0, 12, "�������κ� ����ö��");
	m_Grid.SetMergeCol(0, 12, 17);

	m_Grid.SetTextMatrix(1, 1, "C1");
	m_Grid.SetTextMatrix(1, 2, "C2");
	m_Grid.SetTextMatrix(1, 3, "C3");
	m_Grid.SetTextMatrix(1, 4, "CTC");
	m_Grid.SetTextMatrix(1, 5, "C2��������");
	m_Grid.SetTextMatrix(1, 6, "C3����");
	m_Grid.SetTextMatrix(1, 7, "S1");
	m_Grid.SetTextMatrix(1, 8, "S2");
	m_Grid.SetTextMatrix(1, 9, "S3");
	m_Grid.SetTextMatrix(1, 10, "CTC");
	m_Grid.SetTextMatrix(1, 11, "S3����");
	m_Grid.SetTextMatrix(1, 12, "OB1");
	m_Grid.SetTextMatrix(1, 13, "OB2");
	m_Grid.SetTextMatrix(1, 14, "OB3");
	m_Grid.SetTextMatrix(1, 15, "CTC");
	m_Grid.SetTextMatrix(1, 16, "OB2��������");
	m_Grid.SetTextMatrix(1, 17, "OB3����");
	long n = 0;
	for(n=1; n<=3; n++)
		m_Grid.SetCellType(2, n, CT_COMBO);	
	
	for(n=7; n<=9; n++)
		m_Grid.SetCellType(2, n, CT_COMBO);	

	for(n=12; n<=14; n++)
		m_Grid.SetCellType(2, n, CT_COMBO);	

	if(pDeckData->m_bInstallCrossBeamUpperBogangRebar==FALSE)
	{	
		m_Grid.SetColumnWidth(3, 0);
		m_Grid.SetColumnWidth(6, 0);
	}
	
	if(pDeckData->m_bInstallStringerUpperBogangRebar==FALSE)
	{
		m_Grid.SetColumnWidth(9, 0);
		m_Grid.SetColumnWidth(11, 0);
	}

	if(pDeckData->m_bInstallOutBrkUpperBogangRebar==FALSE)
	{
		m_Grid.SetColumnWidth(14, 0);
		m_Grid.SetColumnWidth(17, 0);
	}
	
	if(!pDeckData->IsBridgeHasCrossBeamHunch())
	{
		for(long n=1; n<=6; n++)
			m_Grid.SetColumnWidth(n, 0);
	}

	if(!pDeckData->IsBridgeHasStringer())
	{
		for(long n=7; n<=11; n++)
			m_Grid.SetColumnWidth(n, 0);
	}

	if(!pDeckData->IsBridgeHasOutBracket())
	{
		for(long n=12; n<=17; n++)
			m_Grid.SetColumnWidth(n, 0);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}


void CDeckCrossStringerDlg::SetGridTitleCrSangse()
{
	CPlateBridgeApp	*pDB	= m_pStd->m_pDataManage->GetBridge();

	m_Grid.SetTextBkColor(RGBEDITABLE);	

	long nGsu = pDB->GetGirdersu();
	long nMaxCr = 0;
	long nG		= 0;
	for (nG=0; nG<nGsu-1; nG++)
		nMaxCr = max(nMaxCr, pDB->GetGirder(nG)->GetQtyCrossBeamVBracingHunch());

	m_Grid.SetRowCount(3*(nGsu-1));
	m_Grid.SetColumnCount(nMaxCr + 2);
	m_Grid.SetFixedColumnCount(2);	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(35);		
	m_Grid.SetColumnWidth(0, 70);	
	m_Grid.SetMergeCol(0, 0, 1);
	m_Grid.SetTextMatrix(0, 0, "�� ��");

	long nRow = 0;
	CString str;
	for(nG = 0; nG < nGsu-1; nG++)
	{
		m_Grid.SetTextMatrix(nRow, 1, "����");
		m_Grid.SetTextMatrix(nRow+1, 1, "����");
		m_Grid.SetTextMatrix(nRow+2, 1, "CTC");
		for(long n = 0; n < nMaxCr; n++)
		{
			m_Grid.SetMergeRow(nRow, nRow+2, 0);
			str.Format("C%d", n+1);
			m_Grid.SetTextMatrix(nRow, n+2, str);
		}
		str.Format("G%d", nG+1);
		m_Grid.SetTextMatrix(nRow+1, 0, str);
		nRow+=2;
	}

	for(nG = 0; nG < nMaxCr; nG++)
	{	
		str.Format("C%d", nG+1);
		m_Grid.SetTextMatrix(0, nG+2, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}


void CDeckCrossStringerDlg::SetGridTitleStSangse()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetRowCount(3);
	m_Grid.SetColumnCount(pDeckData->m_arrCTCBogangRebarAtStringer.GetSize()+1);
	m_Grid.SetFixedColumnCount(1);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(35);		
	m_Grid.SetColumnWidth(0, 70);	
	
	m_Grid.SetTextMatrix(0, 0, "����");
	m_Grid.SetTextMatrix(1, 0, "����");
	m_Grid.SetTextMatrix(2, 0, "CTC");
	
	CString str;
	for (long nStg=0; nStg<pDeckData->m_arrCTCBogangRebarAtStringer.GetSize(); nStg++)
	{
		str.Format("S%d", nStg+1);
		m_Grid.SetTextMatrix(0, nStg+1, str);
			
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}



void CDeckCrossStringerDlg::SetGridTitleObkSangse()
{
/*	CPlateBridgeApp	*pDB		= m_pStd->m_pDataManage->GetBridge();
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	m_Grid.SetTextBkColor(RGBEDITABLE);	

	long nObkLinesu = 2;
	long nMaxObk    = max(pDB->GetLBracketsu(), pDB->GetRBracketsu());

	m_Grid.SetRowCount(2*nObkLinesu + 1);
	m_Grid.SetColumnCount(nMaxObk + 2);
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(2);	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(40);		
	m_Grid.SetColumnWidth(0, 70);	
	m_Grid.SetMergeCol(0, 0, 1);
	m_Grid.SetTextMatrix(0, 0, "�� ��");

	long nRow = 1;
	CString str;
	for(long n = 0; n < nObkLinesu; n++)
	{
		m_Grid.SetTextMatrix(nRow, 1, "����");
		m_Grid.SetTextMatrix(nRow+1, 1, "CTC");
		m_Grid.SetMergeRow(nRow, nRow+1, 0);
		str = n==0 ? "����" : "����";
		m_Grid.SetTextMatrix(nRow, 0, str);
		nRow+=2;
	}

	for(n = 0; n < nMaxObk; n++)
	{	
		str.Format("OB%d", n+1);
		m_Grid.SetTextMatrix(0, n+2, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);*/
}


void CDeckCrossStringerDlg::SetDataDefault()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	long nPos = 0;	// ����ĵƿ������
	
	pDeckData->m_dDiaCrossBeam1	= pDeckData->m_dDiaBeryukRebarLower_Cent[nPos];	// �Ϲݺ� �ϸ� ���ö��
	pDeckData->m_dDiaCrossBeam2	= pDeckData->m_dDiaMainRebarLower_Cent[nPos];	// �Ϲݺ� �ϸ� ��ö��
	pDeckData->m_dDiaCrossBeam3	= pDeckData->m_dDiaMainRebar_Cent[nPos];		// �Ϲݺ� ��� ��ö��
	pDeckData->m_dDiaStringer1	= pDeckData->m_dDiaMainRebarLower_Cent[nPos];	// �Ϲݺ� �ϸ� ��ö��
	pDeckData->m_dDiaStringer2	= pDeckData->m_dDiaBeryukRebarLower_Cent[nPos];	// �Ϲݺ� �ϸ� ���ö��
	pDeckData->m_dDiaStringer3	= pDeckData->m_dDiaBeryukRebarLower_Cent[nPos];	// �Ϲݺ� ��� �ֹ��ö��	
	pDeckData->m_dDiaOutBrk1	= pDeckData->m_dDiaBeryukRebarLower_Cent[nPos];	// �Ϲݺ� �ϸ� ���ö��
	pDeckData->m_dDiaOutBrk2	= pDeckData->m_dDiaMainRebarLower_Cent[nPos];	// �Ϲݺ� �ϸ� ��ö��
	pDeckData->m_dDiaOutBrk3	= pDeckData->m_dDiaMainRebar_Cent[nPos];		// �Ϲݺ� ��� ��ö��

	pDeckData->m_dLengthCrossBeamBogang	 	 = 3000;
	pDeckData->m_dLengthCrossBeamBogangLower = 600;
	pDeckData->m_dLengthStringerBogang		 = 3000;
	pDeckData->m_dLengthOutBrkBogang	 	 = 2000;
	pDeckData->m_dLengthOutBrkBogangLower    = 300;
	BOOL bFirst = !IsUsedFlag();
	if(bFirst)
	{
		pDeckData->m_bInstallCrossBeamUpperBogangRebar = FALSE;
		pDeckData->m_bInstallStringerUpperBogangRebar  = FALSE;
		pDeckData->m_bInstallOutBrkUpperBogangRebar    = FALSE;
	}

//	if(bFirst || GetCurTabName()=="���κ��� ����ö��")
	{
		// �� ���κ����� ����ö�� ���� ����
		pDeckData->m_CTCBasicCrossBeam.y = pDeckData->m_dGenSupportRebarCTC[nPos];
		pDeckData->CalcCTCBogangRebarAtCrossBeam();
	}
//	else if(bFirst || GetCurTabName()=="���κ��� ����ö��")
//	{
//		// �� ���κ����� ����ö�� ���� ����
//		pDeckData->m_CTCBasicStringer.y = pDeckData->m_dGenMainRebarCTC[nPos]/2;
//		pDeckData->CalcCTCBogangRebarAtStringer();
//	}
//	else if(bFirst || GetCurTabName()=="�������κ��� ����ö��")
//	{
//		// �� �������κ����� ����ö�� ���� ����
//		pDeckData->m_CTCBasicOutBrk.y	 = pDeckData->m_dGenSupportRebarCTC[nPos];
//		pDeckData->CalcCTCBogangRebarAtOutBrk();
//	}
}


void CDeckCrossStringerDlg::SetDataInit()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	if(GetCurTabName()=="�����Է�")
		SetDataInitBasic();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataInitCrSangse();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataInitStSangse();
	else if(GetCurTabName()=="�������κ��� ����ö��")
		SetDataInitObkSangse();


	if(GetCurTabName()=="�����Է�")
	{
		GetDlgItem(IDC_CHECK_SHOW_SLABHUNCH)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_C3)->ShowWindow(TRUE);

		if(pDeckData->IsBridgeHasCrossBeamHunch())	GetDlgItem(IDC_CHECK_C3)->ShowWindow(TRUE);
		else									GetDlgItem(IDC_CHECK_C3)->ShowWindow(FALSE);
		if(pDeckData->IsBridgeHasStringer())	GetDlgItem(IDC_CHECK_S3)->ShowWindow(TRUE);
		else									GetDlgItem(IDC_CHECK_S3)->ShowWindow(FALSE);
		if(pDeckData->IsBridgeHasOutBracket())	GetDlgItem(IDC_CHECK_OB3)->ShowWindow(TRUE);
		else									GetDlgItem(IDC_CHECK_OB3)->ShowWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_SHOW_SLABHUNCH)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_C3)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_S3)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_OB3)->ShowWindow(FALSE);
	}

}


void CDeckCrossStringerDlg::SetDataInitBasic()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;
	
	SetGridTitleBasic();

	m_Grid.SetTextMatrix(2, 1, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaCrossBeam1)));
	m_Grid.SetTextMatrix(2, 2, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaCrossBeam2)));
	m_Grid.SetTextMatrix(2, 3, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaCrossBeam3)));
	m_Grid.SetTextMatrix(2, 4, Comma(pDeckData->m_CTCBasicCrossBeam.y));
	m_Grid.SetTextMatrix(2, 5, Comma(pDeckData->m_dLengthCrossBeamBogangLower));
	m_Grid.SetTextMatrix(2, 6, Comma(pDeckData->m_dLengthCrossBeamBogang));
	
	m_Grid.SetTextMatrix(2, 7, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaStringer1)));
	m_Grid.SetTextMatrix(2, 8, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaStringer2)));
	m_Grid.SetTextMatrix(2, 9, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaStringer3)));
	m_Grid.SetTextMatrix(2, 10, Comma(pDeckData->m_CTCBasicStringer.y));
	m_Grid.SetTextMatrix(2, 11, Comma(pDeckData->m_dLengthStringerBogang));

	m_Grid.SetTextMatrix(2, 12, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaOutBrk1)));
	m_Grid.SetTextMatrix(2, 13, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaOutBrk2)));
	m_Grid.SetTextMatrix(2, 14, GetRebarMarkByIndexARoad(GetRebarIndexBySizeARoad(pDeckData->m_dDiaOutBrk3)));
	m_Grid.SetTextMatrix(2, 15, Comma(pDeckData->m_CTCBasicOutBrk.y));
	m_Grid.SetTextMatrix(2, 16, Comma(pDeckData->m_dLengthOutBrkBogangLower));
	m_Grid.SetTextMatrix(2, 17, Comma(pDeckData->m_dLengthOutBrkBogang));

	m_Grid.Invalidate();

	m_bMakeC3  = pDeckData->m_bInstallCrossBeamUpperBogangRebar;
	m_bMakeS3  = pDeckData->m_bInstallStringerUpperBogangRebar;
	m_bMakeOB3 = pDeckData->m_bInstallOutBrkUpperBogangRebar;

	UpdateData(FALSE);
}


void CDeckCrossStringerDlg::SetDataInitCrSangse()
{
	CPlateBridgeApp	*pDB	= m_pStd->m_pDataManage->GetBridge();
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	m_Grid.SetTextBkColor(RGBEDITABLE);	

	long nGsu = pDB->GetGirdersu();
	long nMaxCr = 0;
	long nG		= 0;
	for (nG=0; nG<nGsu-1; nG++)
		nMaxCr = max(nMaxCr, pDB->GetGirder(nG)->GetQtyCrossBeamVBracingHunch());

	m_Grid.SetRowCount(3*(nGsu-1));
	m_Grid.SetColumnCount(nMaxCr + 2);
	m_Grid.SetFixedColumnCount(2);	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(35);		
	m_Grid.SetColumnWidth(0, 70);	

	long nRow = 0;
	CString str;
	for(nG = 0; nG < nGsu-1; nG++)
	{
		str.Format("G%d", nG+1);
		m_Grid.SetMergeRow(nRow, nRow+2, 0);
		m_Grid.SetTextMatrix(nRow, 0, str);

		m_Grid.SetTextMatrix(nRow, 1, "����");
		m_Grid.SetTextMatrix(nRow+1, 1, "����");
		m_Grid.SetTextMatrix(nRow+2, 1, "CTC");

		CPlateBxFinder Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		long nCol = 2;
		while(pBx)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);
			if(pGir->IsCrossBeamBxHasHunch(pBx))
			{
				long nCount = 0;
				CDPoint xyDim = pDeckData->GetCTCRebarAtCB(pBx, nCount);

				m_Grid.SetCellFixType(nRow, nCol, GVN_ONECELL_COLFIX);
				m_Grid.SetTextMatrix(nRow, nCol, "C"+COMMA(nCount+1));
				m_Grid.SetTextMatrix(nRow+1, nCol, "%.0f", xyDim.x);
				m_Grid.SetTextMatrix(nRow+2, nCol, "%.0f", xyDim.y);
				nCol++;
			}

			pBx = Finder.GetBxNext();
		}
		nRow+=3;
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}



void CDeckCrossStringerDlg::SetDataInitStSangse()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	SetGridTitleStSangse();

	for(long nStg=0; nStg<pDeckData->m_arrCTCBogangRebarAtStringer.GetSize(); nStg++)
	{
		m_Grid.SetTextMatrix(1, nStg+1, "%.0f", pDeckData->m_arrCTCBogangRebarAtStringer[nStg].x);
		m_Grid.SetTextMatrix(2, nStg+1, "%.0f", pDeckData->m_arrCTCBogangRebarAtStringer[nStg].y);
	}

	m_Grid.SetRedraw(TRUE,TRUE);	
}





void CDeckCrossStringerDlg::SetDataInitObkSangse()
{
//	CPlateBridgeApp	*pDB		= m_pStd->m_pDataManage->GetBridge();
//	CADeckData	*pDeckData  = m_pStd->m_pDeckData;
//
//	SetGridTitleObkSangse();
//
//	long nRow = 1;
//	long nNum = 0;
//
//	for(long n = 0; n < 2 ; n++)
//	{
//		long nObkSu = n==0 ? pDB->GetLBracketsu() : pDB->GetRBracketsu();
//		for(long nObk = 0; nObk < nObkSu; nObk++)
//		{
//			if(pDeckData->m_arrCTCBogangRebarAtOutBrk[nObk].x==0)
//			{
//				m_Grid.SetTextMatrix(nRow, nObk+2, "0");
//				m_Grid.SetTextMatrix(nRow+1, nObk+2, "0");
//				//  RGBREADONLY
//				m_Grid.SetItemState(nRow, nObk+2, m_Grid.GetItemState(nRow, nObk+2) | GVIS_READONLY);
//				m_Grid.SetItemBkColour(nRow, nObk+2, RGBREADONLY);
//				m_Grid.SetItemState(nRow+1, nObk+2, m_Grid.GetItemState(nRow+1, nObk+2) | GVIS_READONLY);
//				m_Grid.SetItemBkColour(nRow+1, nObk+2, RGBREADONLY);
//			}
//			else
//			{
//				m_Grid.SetTextMatrix(nRow, nObk+2, "%.0f", pDeckData->m_arrCTCBogangRebarAtOutBrk[nObk].x);
//				m_Grid.SetTextMatrix(nRow+1, nObk+2, "%.0f", pDeckData->m_arrCTCBogangRebarAtOutBrk[nObk].y);
//			}
//		}
//		nRow += 2;
//	}
//
//	m_Grid.SetRedraw(TRUE,TRUE);
}



void CDeckCrossStringerDlg::SetDataSave()
{
	if(GetCurTabName()=="�����Է�")
		SetDataSaveBasic();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveCrSangse();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveStSangse();
	else if(GetCurTabName()=="�������κ��� ����ö��")
		SetDataSaveObkSangse();

	CADeckData	*pDeckData  = m_pStd->m_pDeckData;
	if(pDeckData->m_bRecalcJewon && m_pStd->GetDlgOfMenuID(ID_DECK_14)->IsUsedFlag())
		pDeckData->MakeRebarJewon();
}



void CDeckCrossStringerDlg::SetDataSaveBasic()
{
	CADeckData	*pDeckData  = m_pStd->m_pDeckData;

	CString szTemp = m_Grid.GetTextMatrix(2, 1);	szTemp.Delete(0);
	pDeckData->m_dDiaCrossBeam1 = atof(szTemp);

	szTemp = m_Grid.GetTextMatrix(2, 2);	szTemp.Delete(0);
	pDeckData->m_dDiaCrossBeam2 = atof(szTemp);

	szTemp = m_Grid.GetTextMatrix(2, 3);	szTemp.Delete(0);
	pDeckData->m_dDiaCrossBeam3 = atof(szTemp);

	pDeckData->m_CTCBasicCrossBeam.y = m_Grid.GetTextMatrixDouble(2, 4);
	pDeckData->m_dLengthCrossBeamBogangLower = m_Grid.GetTextMatrixDouble(2, 5);
	pDeckData->m_dLengthCrossBeamBogang = m_Grid.GetTextMatrixDouble(2, 6);

	szTemp = m_Grid.GetTextMatrix(2, 7);	szTemp.Delete(0);
	pDeckData->m_dDiaStringer1 = atof(szTemp);

	szTemp = m_Grid.GetTextMatrix(2, 8);	szTemp.Delete(0);
	pDeckData->m_dDiaStringer2 = atof(szTemp);	

	szTemp = m_Grid.GetTextMatrix(2, 9);	szTemp.Delete(0);
	pDeckData->m_dDiaStringer3 = atof(szTemp);	

	pDeckData->m_CTCBasicStringer.y = m_Grid.GetTextMatrixDouble(2, 10);
	pDeckData->m_dLengthStringerBogang = m_Grid.GetTextMatrixDouble(2, 11);


	szTemp = m_Grid.GetTextMatrix(2, 12);	szTemp.Delete(0);
	pDeckData->m_dDiaOutBrk1 = atof(szTemp);

	szTemp = m_Grid.GetTextMatrix(2, 13);	szTemp.Delete(0);
	pDeckData->m_dDiaOutBrk2 = atof(szTemp);

	szTemp = m_Grid.GetTextMatrix(2, 14);	szTemp.Delete(0);
	pDeckData->m_dDiaOutBrk3 = atof(szTemp);

	pDeckData->m_CTCBasicOutBrk.y		  = m_Grid.GetTextMatrixDouble(2, 15);

	pDeckData->m_dLengthOutBrkBogangLower = m_Grid.GetTextMatrixDouble(2, 16);
	pDeckData->m_dLengthOutBrkBogang	  = m_Grid.GetTextMatrixDouble(2, 17);

	pDeckData->m_bInstallCrossBeamUpperBogangRebar = m_bMakeC3;
	pDeckData->m_bInstallStringerUpperBogangRebar  = m_bMakeS3;
	pDeckData->m_bInstallOutBrkUpperBogangRebar    = m_bMakeOB3;
}



void CDeckCrossStringerDlg::SetDataSaveCrSangse()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	for (long nRow=0; nRow<m_Grid.GetRowCount(); nRow+=3)
	{
		for (long nCol=2; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			CString sText = m_Grid.GetTextMatrix(nRow, nCol);
			if(sText.Find("C")!=-1)
			{
				sText.Delete(0);
				long nCount = atol(sText)-1;
				long nRebar = m_Grid.GetTextMatrixLong(nRow+1, nCol);
				long nCTC   = m_Grid.GetTextMatrixLong(nRow+2, nCol);
				pDeckData->m_arrCTCBogangRebarAtCrossBeam[nCount] = CDPoint(nRebar, nCTC);
			}
		}
	}
}


void CDeckCrossStringerDlg::SetDataSaveObkSangse()
{
//	CPlateBridgeApp* pDB = m_pStd->m_pDataManage->GetBridge();
//	CADeckData *pDeckData = m_pStd->m_pDeckData;
//	pDeckData->m_arrCTCBogangRebarAtOutBrk.RemoveAll();
//
//	if (!pDeckData->IsBridgeHasOutBracket())	return;
//
//	//CrossBeam
//	long nObkSu = max(pDB->GetLBracketsu(), pDB->GetRBracketsu());
//	long nRow = 1;
//
//	for(long n = 0; n < 2 ; n++)
//	{
//		CDPoint ptDim;
//		for(long nObk = 0; nObk < nObkSu; nObk++)
//		{
//			ptDim.x = m_Grid.GetTextMatrixDouble(nRow, nObk+2);
//			ptDim.y = m_Grid.GetTextMatrixDouble(nRow+1, nObk+2);
//			pDeckData->m_arrCTCBogangRebarAtOutBrk.Add(ptDim);
//		}
//		nRow+=2;
//	}
}


void CDeckCrossStringerDlg::SetDataSaveStSangse()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	pDeckData->m_arrCTCBogangRebarAtStringer.RemoveAll();

	for(long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
	{
		double x = m_Grid.GetTextMatrixDouble(1, nCol);
		double y = m_Grid.GetTextMatrixDouble(2, nCol);
		pDeckData->m_arrCTCBogangRebarAtStringer.Add(CDPoint(x, y));
	}

}


void CDeckCrossStringerDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDrawDBFrontRebar   DBFrontRebar(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	double dScaleDim = 0;
	CString sText;
	if(GetCurTabName()=="�����Է�")
	{
		dScaleDim = 50;
		sText = "";
		Dom.SetScaleDim(dScaleDim);
		DrawSangseRebar(&Dom);
	}
	else if(GetCurTabName()=="���κ��� ����ö��")
	{
		dScaleDim = 100;
		sText = "���κ� ����ö��";
		Dom.SetScaleDim(dScaleDim);
		DrawPlanRebarCr(&Dom, m_dScaleDim/100);
	}
	else if(GetCurTabName()=="���κ��� ����ö��")
	{
		dScaleDim = 100;
		sText = "���κ� ����ö��";
		Dom.SetScaleDim(dScaleDim);
		DrawPlanRebarSt(&Dom, m_dScaleDim/100);
	}
	else if(GetCurTabName()=="�������κ��� ����ö��")
	{
		dScaleDim = 100;
		sText = "�������κ� ����ö��";
		Dom.SetScaleDim(dScaleDim);
		DrawPlanRebarObk(&Dom, m_dScaleDim/100);
	}
	
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	DrawStd.AddTitleOnDom(&Dom, dScaleDim, rect, sText, TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
	*pDom << Dom;
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}


void CDeckCrossStringerDlg::DrawSangseRebar(CDomyun* pDom)
{	
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CDrawDBFrontRebar   DBFrontRebar(m_pStd->m_pDataManage);
	CAPlateDrawStandard DrawStd(m_pStd->m_pDataManage);
	
	CDomyun Dom(pDom);
	Dom.SetDirection("TOP");	
	m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetScaleDim(35);
	
	CDRect rect1, rect2, rect3;

	if(pDeckData->IsBridgeHasCrossBeamHunch())
	{
		DBFrontRebar.DrawSangseCrossBeam(&Dom);
		Dom.SetCalcExtRect();
		rect1 = Dom.GetExtRect();
		DrawStd.AddTitleOnDom(&Dom, 50, rect1, "���κ� ������", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
		*pDom << Dom;
	}

	if(pDeckData->IsBridgeHasStringer())
	{
		DBFrontRebar.DrawSangseStringer(&Dom);
		Dom.SetCalcExtRect();
		rect2 = Dom.GetExtRect();
		DrawStd.AddTitleOnDom(&Dom, 50, rect2, "���κ� ������", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
		double dMoveX = rect1.Width()*1.2;
		Dom.Move(dMoveX, 0);
	
		*pDom << Dom;
	}

	if(pDeckData->IsBridgeHasOutBracket())
	{
		DBFrontRebar.DrawSangseOutBracket(&Dom);
		Dom.SetCalcExtRect();
		rect3 = Dom.GetExtRect();
		DrawStd.AddTitleOnDom(&Dom, 50, rect3, "�������κ� ������", TRUE, FALSE, 0, FALSE, "", HCAD_STRESS_TEXT);
		double dMoveX = (rect1.Width()+rect2.Width())*1.2;
		Dom.Move(dMoveX, 0);

		*pDom << Dom;
	}

}


void CDeckCrossStringerDlg::DrawPlanRebarCr(CDomyun* pDom, double dTextRatio)
{
	CPlateBridgeApp		 *pDB			= m_pStd->m_pDataManage->GetBridge();		
	
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	
	CDomyun Dom(pDom);

	CPlateBxFinder Finder(pDB->GetGirder(-1));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_SLAB);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_SLAB);

	Dom.SetDirection("TOP");
	DBPlan.DrawSlabTotal(&Dom);
	DBPlan.DrawAbutPierCenterLine(&Dom);               //���뱳���߽ɼ�
	for(long g=0; g<pDB->GetGirdersu(); g++)
		DBPlan.DrawAbutPierSymbol(&Dom, g);     //����, �����ɺ���ũ
	DBPlan.DrawCenterLine(&Dom, -1, pBxStt->GetStation(), pBxEnd->GetStation());
	DBPlan.DrawCenterLine(&Dom, -1, pBxStt->GetStation(), pBxEnd->GetStation());
	DBPlan.DrawTaperLinePyung(&Dom);			// Taper �� �׸���
	DBPlan.DrawCRBeamCLTotalGirder(&Dom); //�߽ɼ����׸�
	
	long nGirderSu  = pDB->GetGirdersu();
	for(long nG=0; nG<nGirderSu-1; nG++)
	{
		CPlateGirderApp	*pGir   = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);	
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
		// �Ŵ�����
		DBPlan.DrawGirder(&Dom, TRUE);		
		if(m_bShowSlabHunch)
		{
			m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
			DBPlan.DrawPlanCrossBeam(&Dom, pBxStt, pBxEnd);				
		}

		// ũ�ν� ��
		m_pStd->m_pDataManage->GetOptionStd()->SetEnvLType(&Dom,LT_DASH_LINE);

		if(nG < nGirderSu-1)
		{
			CPlateBxFinder FinderSub(pGir);			
			CPlateBasicIndex *pBx = FinderSub.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
			while(pBx)
			{
				if(pGir->IsCrossBeamBxHasHunch(pBx))
				{					
					m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);			
					DBDeckPlan.DrawBogangRebarAtCrossBeamBx(&Dom, pBx, 0, TRUE);
					m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
					if(dTextRatio!=1.0)
						Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);
					DBDeckPlan.DimBogangRebarAtCrossBeamBx(&Dom, pBx, 0, TRUE);
				}

				pBx = FinderSub.GetBxNext();
			}
		}		
	}
	
//	// 3. ���� ���κ�.....���� ���� �����Ѵ�
//	if(pDB->GetLBracketsu()>0 && m_bShowSlabHunch)
//	{			
//		CPlateBxFinder Finder(pDB->GetGirder(0));
//		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_LEFT, NULL, NULL);						
//		long nUpLow = 1;
//		while(pBx)
//		{
//			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
//			if(pBxNext)
//				DBPlan.DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, TRUE);
//			pBx = Finder.GetBxNext();
//		}
//	}		
//	
//	
//	if(pDB->GetRBracketsu()>0 && m_bShowSlabHunch)
//	{			
//		CPlateBxFinder Finder(pDB->GetGirder(pDB->GetGirdersu()-1));	
//		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_RIGHT, NULL, NULL);						
//		long nUpLow = 1;
//		while(pBx)
//		{
//			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
//			if(pBxNext)
//				DBPlan.DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, FALSE);
//			pBx = Finder.GetBxNext();
//		}
//	}	
//
//	CPlateBxFinder Finder(pDB->GetGirder(-1));	
//	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
//	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
//	
//	double rotAng  = pDBStd->GetRotateAnglePyung(pBxStt,pBxEnd);
//	if(CDrawDBGen::IsInputDomRotate())
//		Dom.Rotate(rotAng,0,0);

	*pDom << Dom;
}


void CDeckCrossStringerDlg::DrawPlanRebarSt(CDomyun* pDom, double dTextRatio)
{
/*	CPlateBridgeApp			 *pDB			= m_pStd->m_pDataManage->GetBridge();		
	CADeckData			 *pDeckData		= m_pStd->m_pDeckData;
	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);

	CDomyun Dom(pDom);
	Dom.SetDirection("TOP");
	DBPlan.DrawSlabTotal(&Dom);
	DBGen.DrawPlaneSttEndLine(&Dom);			// ������ ���� �� �׸���
	DBGen.DrawPlaneCenterLineEachGider(&Dom);	// �Ŵ����� �߽ɼ�
	DBGen.DrawPlaneSlabSttEnd(&Dom);			// ������ ������ ���� �������� ����
	DBGen.DrawTaperLinePyung(&Dom);			// Taper �� �׸���
	for(long n=0; n<m_pDB->m_nQtyJigan-1; n++)	// ������ �׸���
		DBGen.DrawPlanePierLine(&Dom, n);	
	DBGen.PierNumDraw(&Dom,0);				// ���� �� ���� ��ȣ  Text ����ϱ�	
	DBGen.DimXySlab(&Dom);					// ������ x.y��
	DBGen.DimXyCenter(&Dom);					// ���������̼�
	
	long nGirderSu  = pDB->GetGirdersu();
	for(long nG=0; nG<nGirderSu; nG++)
	{
		CPlateGirderApp	*pGir   = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);	
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
		// �Ŵ�����
		DBPlan.DrawGirderLineOnSlab(&Dom,pBxStt,pBxEnd,1,1);		

		m_pStd->m_pDataManage->GetOptionStd()->SetEnvLType(&Dom,LT_DASH_LINE);

		if(nG < nGirderSu-1)
		{
			CPlateBxFinder FinderSub(pGir);			
			CPlateBasicIndex *pBx = FinderSub.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
			while(pBx)
			{
				if(m_bShowSlabHunch)	// ��ġ����
					m_pStd->m_pDDBGen->DrawPlanSlabCrossbeam(&Dom, pBx);		

				pBx = FinderSub.GetBxNext();
			}
		}		
	}		
					
	for (long nStg=0; nStg<pDeckData->m_arrStringer.GetSize(); nStg++)
	{
		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_BARC);			
		pDBDeckPlan->DrawBogangRebarAtStringer(&Dom, nStg, 0, TRUE);	

		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_DIML);
		if(dTextRatio!=1.0)
			Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*dTextRatio);
		pDBDeckPlan->DimBogangRebarAtStringer(&Dom, nStg, 0, TRUE);	
	}
	

	// 3. ���� ���κ�.....���� ���� �����Ѵ�
	if(pDB->GetLBracketsu()>0 && m_bShowSlabHunch)
	{			
		CPlateBxFinder Finder(pDB->GetGirder(0));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_LEFT, NULL, NULL);						
		long nUpLow = 1;
		while(pBx)
		{
			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
			if(pBxNext)
				DBPlan.DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, TRUE);
			pBx = Finder.GetBxNext();
		}
	}		
	
	
	if(pDB->GetRBracketsu()>0 && m_bShowSlabHunch)
	{			
		CPlateBxFinder Finder(pDB->GetGirder(pDB->GetGirdersu()-1));	
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_RIGHT, NULL, NULL);						
		long nUpLow = 1;
		while(pBx)
		{
			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
			if(pBxNext)
				DBPlan.DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, FALSE);
			pBx = Finder.GetBxNext();
		}
	}	

	CPlateBxFinder Finder(pDB->GetGirder(-1));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	
	double rotAng  = pDBStd->GetRotateAnglePyung(pBxStt,pBxEnd);
	if(CDrawDBGen::IsInputDomRotate())
		Dom.Rotate(rotAng,0,0);

	*pDom << Dom;*/
}




void CDeckCrossStringerDlg::DrawPlanRebarObk(CDomyun* pDom, double dTextRatio)
{
/*	CPlateBridgeApp		 *pDB			= m_pStd->m_pDataManage->GetBridge();		
	CADeckData			 *pDeckData		= m_pStd->m_pDeckData;

	CAPlateDrawPyung	 DBPlan(m_pStd->m_pDataManage);
	CDrawDBBaseRebarPlan DBDeckPlan(m_pStd->m_pDataManage);
	
	CDomyun Dom(pDom);

	Dom.SetDirection("TOP");
	DBPlan.DrawSlabTotal(&Dom);
	DBGen.DrawPlaneSttEndLine(&Dom);			// ������ ���� �� �׸���
	DBGen.DrawPlaneCenterLineEachGider(&Dom);	// �Ŵ����� �߽ɼ�
	DBGen.DrawPlaneSlabSttEnd(&Dom);			// ������ ������ ���� �������� ����
	DBGen.DrawTaperLinePyung(&Dom);			// Taper �� �׸���
	for(long n=0; n<m_pDB->m_nQtyJigan-1; n++)	// ������ �׸���
		DBGen.DrawPlanePierLine(&Dom, n);	
	DBGen.PierNumDraw(&Dom,0);				// ���� �� ���� ��ȣ  Text ����ϱ�	
	DBGen.DimXyCenter(&Dom);					// ���������̼�
	
	// �Ŵ�����
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp	*pGir   = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);	
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);

		m_pStd->m_pDataManage->GetOptionStd()->SetEnvType(&Dom,HCAD_STLC);
		DBPlan.DrawGirderLineOnSlab(&Dom,pBxStt,pBxEnd,1,1);		
		m_pStd->m_pDataManage->GetOptionStd()->SetEnvLType(&Dom,LT_DASH_LINE);

		if(nG < pDB->GetGirdersu()-1)
		{
			CPlateBxFinder FinderSub(pGir);			
			CPlateBasicIndex *pBx = FinderSub.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
			while(pBx)
			{
				if(m_bShowSlabHunch)	// ��ġ����
					m_pStd->m_pDDBGen->DrawPlanSlabCrossbeam(&Dom, pBx);		

				pBx = FinderSub.GetBxNext();
			}
		}		
	}

	// 3. ���� ���κ�.....���� ���� �����Ѵ�
	if(pDB->GetLBracketsu()>0 && m_bShowSlabHunch)
	{			
		CPlateBxFinder Finder(pDB->GetGirder(0));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_LEFT, NULL, NULL);						
		long nUpLow = 1;
		while(pBx)
		{
			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
			if(pBxNext)
				DBPlan.DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, TRUE);
			pBx = Finder.GetBxNext();
		}
	}		
	
	
	if(pDB->GetRBracketsu()>0 && m_bShowSlabHunch)
	{			
		CPlateBxFinder Finder(pDB->GetGirder(pDB->GetGirdersu()-1));	
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_RIGHT, NULL, NULL);						
		long nUpLow = 1;
		while(pBx)
		{
			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
			if(pBxNext)
				DBPlan.DrawPlanOutSidePlateBxOnSlab(&Dom, pBx, pBxNext, nUpLow, FALSE);
			pBx = Finder.GetBxNext();
		}
	}	

	if(pDB->GetLBracketsu()>0 && pDB->m_dThickSlabHunch > 0 )
	{
		CPlateGirderApp *pGir = pDB->GetGirder(0);
		CPlateBxFinder Finder(pGir);	
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_LEFT);
		while (pBx)
		{
			pDBDeckPlan->DrawBogangRebarAtOutBrkBx(&Dom, pBx, TRUE, 0, TRUE);
			pBx = Finder.GetBxNext();
		}
	}

	if(pDB->GetRBracketsu()>0 && pDB->m_dThickSlabHunch > 0 )
	{
		CPlateGirderApp *pGir = pDB->GetGirder(pDB->GetGirdersu()-1);
		CPlateBxFinder Finder(pGir);	
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_OUTSIDE_CROSS_RIGHT);
		while (pBx)
		{
			pDBDeckPlan->DrawBogangRebarAtOutBrkBx(&Dom, pBx, FALSE, 0, TRUE);
			pBx = Finder.GetBxNext();
		}
	}

	
	CPlateBxFinder Finder(pDB->GetGirder(-1));	
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex *pBxEnd = Finder.GetBxFirst(BX_END_GIRDER);
	
	double rotAng  = pDBStd->GetRotateAnglePyung(pBxStt,pBxEnd);
	if(CDrawDBGen::IsInputDomRotate())
		Dom.Rotate(rotAng,0,0);

	*pDom << Dom;*/
}



BOOL CDeckCrossStringerDlg::PreTranslateMessage(MSG* pMsg) 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CStringArray strComboArr;
	CString szFy;

	if(GetCurTabName()=="�����Է�")
	{
		szFy = GetCharRebarMark(toKgPCm2(pDeckData->m_SigmaY));
		
		strComboArr.Add(szFy+"13");
		strComboArr.Add(szFy+"16");
		strComboArr.Add(szFy+"19");
		strComboArr.Add(szFy+"22");
		strComboArr.Add(szFy+"25");
		strComboArr.Add(szFy+"29");
		strComboArr.Add(szFy+"32");
		strComboArr.Add(szFy+"35");
		
		if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
		{
			CCellID next = m_Grid.GetFocusCell();
			if(m_Grid.GetCellType(next.row, next.col) == CT_COMBO)
			{
				if(next.row == 2)
					m_Grid.SetComboString(strComboArr);		
			}
		}
	}

	if(m_Grid.TranslateMsg(pMsg))
		return m_Grid.PreTranslateMessage(pMsg);
	
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}



void CDeckCrossStringerDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData();	

	m_Grid.DeleteAllItems();

	SetDataInit();	
	DrawInputDomyunView();
}

void CDeckCrossStringerDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(GetCurTabName()=="�����Է�")
		SetDataSaveBasic();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveCrSangse();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveStSangse();
	else if(GetCurTabName()=="�������κ��� ����ö��")
		SetDataSaveObkSangse();
}

BOOL CDeckCrossStringerDlg::IsValid()
{
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();		
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	if(!pDB->m_bApplyHunchAtCB)	return FALSE;

	BOOL bCB     = pDeckData->IsBridgeHasCrossBeamHunch();
	BOOL bOutBrk = pDeckData->IsBridgeHasOutBracket();
	BOOL bCBBrk  = bCB||bOutBrk ? TRUE : FALSE;

	if((bCBBrk && m_pStd->m_bInclude_Module_Deck) && m_pStd->m_pDataManage->GetBridge()->m_dThickSlabHunch>0)
		return TRUE;
	else
		return FALSE;

	return FALSE;
}

void CDeckCrossStringerDlg::OnButtonDeleteRebar() 
{
	CADeckData			 *pDeckData		= m_pStd->m_pDeckData;

	if(GetCurTabName()=="�����Է�")
	{
	}
	else if(GetCurTabName()=="���κ��� ����ö��")
	{
		for(long n=0; n < pDeckData->m_arrCTCBogangRebarAtCrossBeam.GetSize(); n++)
		{	
			if(pDeckData->m_arrCTCBogangRebarAtCrossBeam[n].x > 0)
				pDeckData->m_arrCTCBogangRebarAtCrossBeam[n].x -= 1;
		}	
	}
	else if(GetCurTabName()=="���κ��� ����ö��")
	{
		for(long n=0; n < pDeckData->m_arrCTCBogangRebarAtStringer.GetSize(); n++)
		{	
			if(pDeckData->m_arrCTCBogangRebarAtStringer[n].x > 0)
				pDeckData->m_arrCTCBogangRebarAtStringer[n].x -= 1;
		}	
	}
	else if(GetCurTabName()=="�������κ��� ����ö��")
	{
		for(long n=0; n < pDeckData->m_arrCTCBogangRebarAtOutBrk.GetSize(); n++)
		{	
			if(pDeckData->m_arrCTCBogangRebarAtOutBrk[n].x > 0)
				pDeckData->m_arrCTCBogangRebarAtOutBrk[n].x -= 1;
		}	
	}

	SetDataInit();
	if(GetCurTabName()=="�����Է�")
		SetDataSaveBasic();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveCrSangse();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveStSangse();
	else if(GetCurTabName()=="�������κ��� ����ö��")
		SetDataSaveObkSangse();

	DrawInputDomyunView();	
}



void CDeckCrossStringerDlg::OnButtonAddRebar() 
{
	CADeckData			 *pDeckData		= m_pStd->m_pDeckData;

	if(GetCurTabName()=="�����Է�")
	{
	}
	else if(GetCurTabName()=="���κ��� ����ö��")
	{
		for(long n=0; n < pDeckData->m_arrCTCBogangRebarAtCrossBeam.GetSize(); n++)
		{	
			if(pDeckData->m_arrCTCBogangRebarAtCrossBeam[n].x > 0)
				pDeckData->m_arrCTCBogangRebarAtCrossBeam[n].x += 1;
		}	
	}
	else if(GetCurTabName()=="���κ��� ����ö��")
	{
		for(long n=0; n < pDeckData->m_arrCTCBogangRebarAtStringer.GetSize(); n++)
		{	
			if(pDeckData->m_arrCTCBogangRebarAtStringer[n].x > 0)
				pDeckData->m_arrCTCBogangRebarAtStringer[n].x += 1;
		}	
	}
	else if(GetCurTabName()=="�������κ��� ����ö��")
	{
		for(long n=0; n < pDeckData->m_arrCTCBogangRebarAtOutBrk.GetSize(); n++)
		{	
			if(pDeckData->m_arrCTCBogangRebarAtOutBrk[n].x > 0)
				pDeckData->m_arrCTCBogangRebarAtOutBrk[n].x += 1;
		}	
	}

	SetDataInit();
	if(GetCurTabName()=="�����Է�")
		SetDataSaveBasic();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveCrSangse();
	else if(GetCurTabName()=="���κ��� ����ö��")
		SetDataSaveStSangse();
	else if(GetCurTabName()=="�������κ��� ����ö��")
		SetDataSaveObkSangse();

	DrawInputDomyunView();
}


void CDeckCrossStringerDlg::OnCheckC3() 
{
	UpdateData();

	SetDataSaveBasic();
	SetDataInitBasic();

	DrawInputDomyunView();
	
}

void CDeckCrossStringerDlg::OnCheckS3() 
{
	UpdateData();

	SetDataSaveBasic();
	SetDataInitBasic();

	DrawInputDomyunView();
	
}


void CDeckCrossStringerDlg::OnCheckOB3() 
{
	UpdateData();

	SetDataSaveBasic();
	SetDataInitBasic();

	DrawInputDomyunView();	
}



void CDeckCrossStringerDlg::OnCheckShowSlabHunch() 
{
	UpdateData();

	DrawInputDomyunView();
	
}




CString CDeckCrossStringerDlg::GetCurTabName()
{
	CString sText;
	TCITEM item ;
	char cTableName[256];
	item.mask = TCIF_TEXT;
	item.pszText = cTableName;
	item.cchTextMax = 255;

	m_ctlTab.GetItem(m_ctlTab.GetCurSel(), &item);
	sText = item.pszText;

	return sText;
	
}
