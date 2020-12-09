// FieldJoinPositionDlg.cpp : implementation file
// 현장이음 위치

#include "stdafx.h"
#include "GeneralInput.h"
#include "FieldJoinPositionDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
  
/////////////////////////////////////////////////////////////////////////////
// CFieldJoinPositionDlg dialog
const double BASE_LENGTH = 13750;
const double BASE_JIJUM_LENGTH = 5625;  

CFieldJoinPositionDlg::CFieldJoinPositionDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CFieldJoinPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFieldJoinPositionDlg)
	m_bSameApplySameCol = TRUE;
	m_nLengthType = 1;
	m_dDisCenterOnSplice = 0.0;
	m_bCheckMove = FALSE;
	m_dMoveTerm = 0.0;
	//}}AFX_DATA_INIT
	m_nCount = 0;
	m_bFirstRun = TRUE;
}
  

void CFieldJoinPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFieldJoinPositionDlg)
/*	DDX_Control(pDX, IDC_SPLICE_DISTANCE_CENTER, m_Edit1);*/
	DDX_Control(pDX, IDC_SPLICE_MOVEDIS, m_Edit2);
// 	DDX_Control(pDX, IDC_SPLICE_GIRDERLEN, m_Radio1);
// 	DDX_Control(pDX, IDC_SPLICE_BRIDGELEN, m_Radio2);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_SPLICE_ALL_GIRDER, m_bSameApplySameCol);
	DDX_Radio(pDX, IDC_SPLICE_GIRDERLEN, m_nLengthType);
	DDX_Text(pDX, IDC_SPLICE_DISTANCE_CENTER, m_dDisCenterOnSplice);
	DDX_Check(pDX, IDC_SPLICE_CHECKMOVE, m_bCheckMove);
	DDX_Text(pDX, IDC_SPLICE_MOVEDIS, m_dMoveTerm);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SPLICE_LEFT_SHIFT, m_btnMoveLeft);
	DDX_Control(pDX, IDC_SPLICE_RIGHT_SHIFT, m_btnMoveRight);
	DDX_Control(pDX, IDC_BUTTON_APPLY_ALL_SPLICE, m_btnAllSplice);
	DDX_Control(pDX, IDC_BUTTON_APPLY_ALL_WELD, m_btnAllWeld);
	DDX_Control(pDX, IDC_SPLICE_MOVE1, m_btnMove1);
	DDX_Control(pDX, IDC_SPLICE_MOVE2, m_btnMove2);
	DDX_Control(pDX, IDC_SPLICE_ALLEQUAL, m_btnAllSame);
	DDX_Control(pDX, IDC_SPLICE_AUTO_SYMMETRY, m_btnSymmetry);
}

BEGIN_MESSAGE_MAP(CFieldJoinPositionDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CFieldJoinPositionDlg)
	ON_BN_CLICKED(IDC_SPLICE_ALL_GIRDER, OnSpliceAllGirder)
	ON_BN_CLICKED(IDC_SPLICE_GIRDERLEN, OnSpliceGirderlen)
	ON_BN_CLICKED(IDC_SPLICE_BRIDGELEN, OnSpliceBridgelen)
	ON_BN_CLICKED(IDC_SPLICE_ALLEQUAL, OnSpliceAllequal)
	ON_BN_CLICKED(IDC_SPLICE_RIGHT_SHIFT, OnSpliceRightShift)
	ON_BN_CLICKED(IDC_SPLICE_LEFT_SHIFT, OnSpliceLeftShift)
	ON_BN_CLICKED(IDC_SPLICE_CHECKMOVE, OnSpliceCheckmove)
	ON_BN_CLICKED(IDC_SPLICE_AUTO_SYMMETRY, OnSetDataSymmetry)
	ON_BN_CLICKED(IDC_SPLICE_MOVE1, MoveByMoment)
	ON_BN_CLICKED(IDC_SPLICE_MOVE2, MoveByStress)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_ALL_SPLICE, OnButtonApplyAllSplice)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_ALL_WELD, OnButtonApplyAllWeld)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnButtonNext)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_COPIED_DATA, IDC_GRID, OnCellCopiedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldJoinPositionDlg message handlers
BOOL CFieldJoinPositionDlg::OnInitDialog() 
{	
	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_SPLICE_LEFT_SHIFT,		SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_SPLICE_RIGHT_SHIFT,		SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_APPLY_ALL_SPLICE,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_APPLY_ALL_WELD,	SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_SPLICE_ALLEQUAL,			SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SPLICE_AUTO_SYMMETRY,		SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_GROUPBOX,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICE_GIRDERLEN,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICE_BRIDGELEN,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICE_DISTANCE_CENTER,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICE_ALL_GIRDER,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SPLICE_CHECKMOVE,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;
}

void CFieldJoinPositionDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);

	if(m_nTab==0)
	{
		CAPlateDrawPyung Draw(m_pStd->GetDataManage());
		Draw.DrawPyung(&Dom, 8);
	}
	else
	{
		CAPlateDrawJong Draw(m_pStd->GetDataManage());
		Draw.DrawJongGirderTotal(&Dom);
	}

	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CFieldJoinPositionDlg::SetGridTitle()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CStringArray szArr;
	szArr.Add("이음판");
	szArr.Add("용접");

	long nColumnW = pBridge->IsTUContinuousSpan() ? 110 : 80;

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);
	m_Grid.EnableTitleTips(FALSE);
//	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);		
	m_Grid.SetColumnWidthAll(60);	
	m_Grid.SetColumnWidth(0, nColumnW);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);
	m_Grid.SetComboString(szArr);

	CString str;
	long cols = m_Grid.GetColumnCount();
	long rows = m_Grid.GetRowCount();//타이틀, 도로중심 포함.
	
	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetTextMatrix(1,0,"이음종류");
	m_Grid.SetTextMatrix(2,0,"길이(CR)");
	m_Grid.SetTextMatrix(3,0,"이동(CR)");
	m_Grid.SetTextMatrix(4,0,"교번부지정(CR)");
	for(long nCol = 1;nCol < cols;nCol++)
	{		
		str.Format(" SP %d",nCol);
		m_Grid.SetTextMatrix(0,nCol,str);		
	}
	long nRow = 0;
	for(nRow = 5;nRow < rows;nRow+=3)
	{
		str.Format("길이(G%d)",GetNumberGirder(nRow)+pBridge->GetNumberStartGirder()+1);
		m_Grid.SetTextMatrix(nRow,0,str);
		str.Format("이동(G%d)",GetNumberGirder(nRow)+pBridge->GetNumberStartGirder()+1);
		m_Grid.SetTextMatrix(nRow+1,0,str);
		str.Format("교번부지정(G%d)",GetNumberGirder(nRow)+pBridge->GetNumberStartGirder()+1);
		m_Grid.SetTextMatrix(nRow+2,0,str);		
	}			

	for(nRow = 2;nRow < rows;nRow+=3)
	{
		long nLastCol = m_Grid.GetValidCol(nRow, FALSE);
		for(long nCol = 1;nCol <= nLastCol;nCol++)
		{
			m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) & ~GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow, nCol, RGB(225, 250, 250));
			m_Grid.SetItemState(nRow+1,nCol, m_Grid.GetItemState(nRow+1,nCol) | GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow+1, nCol, RGBREADONLY);
			m_Grid.SetItemState(nRow+2, nCol, m_Grid.GetItemState(nRow, nCol) & ~GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow+2, nCol, RGB(225, 250, 250));
		}
		m_Grid.SetItemState(nRow,nLastCol, m_Grid.GetItemState(nRow,nLastCol) | GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nLastCol, RGBREADONLY);

		if (!m_bCheckMove)
			m_Grid.SetRowHeight(nRow+1, 0);
		if(!pBridge->IsTUContinuousSpan())
			m_Grid.SetRowHeight(nRow+2, 0);
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CFieldJoinPositionDlg::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	m_bSameApplySameCol  = (BOOL)pBridge->m_cSameApplySameColSplice;
	m_nLengthType        = pBridge->m_nLengthTypeOnSplice;
	m_dDisCenterOnSplice = pBridge->m_dDisCenterOnSplice;
	m_dMoveTerm          = pBridge->m_dMoveTermOnSplice;
	
	long nCols     = 0;
	long nSplicesu = 0;
	CPlateGirderApp *pGir = NULL;
	long nG = 0;
	for(nG=-1; nG<pBridge->GetGirdersu(); nG++)	//그리드 컬럼 수를 구함.
	{
		pGir      = pBridge->GetGirder(nG);
		nSplicesu = pGir->GetQtySplice() + 1;

		if(nSplicesu>nCols)	nCols = nSplicesu;
	}
	m_Grid.SetColumnCount(nCols+1);
	m_Grid.SetRowCount(pBridge->GetGirdersu()*3 + 5);
	
	for(nG=-1; nG<pBridge->GetGirdersu(); nG++)
	{
		pGir      = pBridge->GetGirder(nG);
		long nCol = 1;
		long nRow = GetRow(nG);

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE | BX_END_GIRDER);
		while (pBx)
		{
			double sta    = pBx->GetStation();
			double staMid = pGir->GetStationByOneStepMid(sta, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
			if (pBx->IsState(BX_END_GIRDER))
				staMid = sta;
			if(nG==-1)	
			{
				if(Finder.GetBxFixNext())
				{
					m_Grid.SetCellType(1, nCol, CT_COMBO);
					m_Grid.SetItemState   (1, nCol, GVIS_MODIFIED);
					m_Grid.SetItemBkColour(1, nCol, RGB(225, 250, 250));
				}
				else
				{
					m_Grid.SetItemState   (1, nCol, m_Grid.GetItemState(1, nCol) | GVIS_READONLY);
					m_Grid.SetItemBkColour(1, nCol, RGBREADONLY);
				}
			}

			SetMatrixByStation(nRow, nCol, sta);
			SetMatrixByStation(nRow+1, nCol, staMid);
			m_Grid.SetCellType(nRow+2, nCol, CT_CHECKBOX);
			m_Grid.SetTextMatrix(nRow+2, nCol, "");
			m_Grid.SetCheck(nRow+2, nCol++, pBx->IsShift());
			pBx = Finder.GetBxNext();
		}
	}
	// 이음종류 설정 그리드
	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	long nCol = 1;
	while (pBx)
	{
		CPlateSplice *pSp = pBx->GetSplice();
		if(pSp->m_bSpliceType)			m_Grid.SetTextMatrix(1, nCol++, "이음판");
		else							m_Grid.SetTextMatrix(1, nCol++, "용접");

		pBx = Finder.GetBxNext();
	}	

	UpdateData(FALSE);

	m_Grid.SetInsertThisOnly(!m_bSameApplySameCol);
	m_Grid.SetDeleteThisOnly(!m_bSameApplySameCol);

	SetGridTitle();
}

void CFieldJoinPositionDlg::SetDataDefault()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CGlobarOption   *pGloOpt = m_pStd->GetDataManage()->GetGlobalOption();

	m_bSameApplySameCol  = (BOOL)pDB->m_cSameApplySameColSplice;
	m_nLengthType        = pDB->m_nLengthTypeOnSplice;
	m_dDisCenterOnSplice = pDB->m_dDisCenterOnSplice;
	m_dMoveTerm          = pDB->m_dMoveTermOnSplice;

	UpdateData(FALSE);
	SetDataSave();

	double dLengthMax   = pGloOpt->GetDesignMaxLengthOfFieldSplice()-625;
	double dLengthJijum = 11250;
	long nG, nK, nDivide, n;
	long nGsu = pDB->GetGirdersu();
	long nKsu = pDB->m_nQtyJigan;
	long nSize;

	double PreLength;
	double SttSpOrg, EndSpOrg, LengthOrg;
	double SttSp, EndSp;
	double Sta, SpSta, NewSta;
	double SttGir, EndGir;

	CDDWordArray DArSpSta; // 시작슈,끝슈,지점부에 위치한 현장이음 Sta.
	CDDWordArray DArSpStaAll;  // 구한 현장이음 Sta.

	for(nG = -1; nG < nGsu; nG++)
	{
		DArSpSta.RemoveAll();
		DArSpStaAll.RemoveAll();
		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		SttGir = pGir->GetSSS(OFF_STT_GIRDER);
		EndGir = pGir->GetSSS(OFF_END_GIRDER);
		
		DArSpSta.Add(SttGir);
		PreLength = SttGir;

		// 지점부
		for(nK = 0; nK < nKsu-1; nK++)
		{
			Sta = pGir->GetStationOnJijum(nK+1);

			// 지점부 좌측 현장이음
			SpSta = Sta - dLengthJijum / 2.0;
			NewSta = pGir->GetStationByOneStepMid(SpSta, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
			DArSpSta.Add(NewSta);

			// 지점부 좌측 현장이음
			SpSta = Sta + dLengthJijum / 2.0;
			NewSta = pGir->GetStationByOneStepMid(SpSta, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
			DArSpSta.Add(NewSta);
		}

		DArSpSta.Add(EndGir);

		// 지간 중앙부
		for(nK = 0; nK < nKsu; nK++)
		{
			SttSpOrg  = DArSpSta[2 * nK];
			EndSpOrg  = DArSpSta[2 * nK + 1];
			LengthOrg = EndSpOrg - SttSpOrg;

			DArSpStaAll.Add(SttSpOrg);  // 현장이음1

			nDivide = (long)Round((LengthOrg / dLengthMax),0) + 1;
			if(nDivide % 2 == 0) nDivide++;  // nDivide는 홀수

			SttSp = SttSpOrg;
			EndSp = EndSpOrg;

			for(n = 0; n < nDivide; n++)
			{
				if(n % 2 == 0)
				{
					SttSp += LengthOrg / nDivide;

					if(pDB->IsTUGir() && n==0)
						NewSta = pGir->GetStationByOneStepMid(SttSp, -1, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
					else
						NewSta = pGir->GetStationByOneStepMid(SttSp, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
					SttSp = NewSta;
					DArSpStaAll.Add(SttSp);  // 현장이음2
				}
				else
				{
					EndSp -= LengthOrg / nDivide;

					if(pDB->IsTUGir() && n==1)
						NewSta = pGir->GetStationByOneStepMid(EndSp, 1, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
					else
						NewSta = pGir->GetStationByOneStepMid(EndSp, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
					EndSp = NewSta;
					DArSpStaAll.Add(EndSp);  // 현장이음3

					if(EndSp - SttSp <= dLengthMax) break;
				}
			}

			DArSpStaAll.Add(EndSpOrg); // 현장이음4
		}

		double dTest = 0;
		for(long nK=0; nK<DArSpStaAll.GetSize(); nK++)
		{
			dTest = DArSpStaAll.GetAt(nK);			
		}

		// 정렬하여 그리드에 써넣기
		long i, j;
		nSize = DArSpStaAll.GetSize();
		for (i = 1; i < nSize; i++)   {
			double temp = DArSpStaAll[i];
			j = i;
			while (j > 0 && DArSpStaAll[j - 1] > temp)   {
				DArSpStaAll.SetAt(j, DArSpStaAll[j - 1]);
				j--;
			}
			DArSpStaAll.SetAt(j, temp);
		}

		if(nG == -1)
		{
			m_Grid.SetRowCount(nGsu*3+5);
			m_Grid.SetColumnCount(nSize);
		}
		else if (m_Grid.GetCols() < nSize)
		{
			m_Grid.SetColumnCount(nSize);
		}

		for(n=1; n<nSize; n++)
		{
			SetMatrixByStation(GetRow(nG), n, DArSpStaAll[n]);
			m_Grid.SetTextMatrix(GetRow(nG)+1, n, "0");
		}
		for(n ; n<m_Grid.GetCols(); n++)
		{
			m_Grid.SetTextMatrix(GetRow(nG), n, "");
			m_Grid.SetTextMatrix(GetRow(nG)+1, n, "");
		}
	}
	
	// 이음종류 설정 그리드(현장이음판)
	CPlateBxFinder Finder(pDB->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	while (pBx)
	{
		CPlateSplice *pSp = pBx->GetSplice();
		pSp->m_bSpliceType = TRUE;

		pBx = Finder.GetBxNext();
	}

	// 교번부설정 초기화
	for(nG=-1; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE|BX_END_GIRDER);
		while (pBx)
		{
			pBx->SetShift(FALSE);
			pBx = Finder.GetBxNext();
		}
	}
	
	SetDataBx();

}
	
void CFieldJoinPositionDlg::SetDataSave()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	UpdateData();
	pBridge->m_cSameApplySameColSplice = (BYTE)m_bSameApplySameCol;
	pBridge->m_nLengthTypeOnSplice     = m_nLengthType;
	pBridge->m_dDisCenterOnSplice      = m_dDisCenterOnSplice;
	pBridge->m_dMoveTermOnSplice       = m_dMoveTerm;

	// 이음종류 설정 그리드
	for(long nG=-1; nG<pBridge->GetGirdersu(); nG++)
	{
		CPlateBxFinder Finder(pBridge->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
		long nCol = 1;
		while (pBx)
		{
			CString szSpType = m_Grid.GetTextMatrix(1, nCol);
			if(szSpType=="이음판")
				pBx->GetSplice()->m_bSpliceType = TRUE;
			else
				pBx->GetSplice()->m_bSpliceType = FALSE;

			if(nCol<m_Grid.GetColumnCount())
			{
				pBx->SetShift(m_Grid.IsCheck(GetRow(nG)+2, nCol));
			}
			
			nCol++;
			
			pBx = Finder.GetBxNext();
		}
	}
}

long CFieldJoinPositionDlg::GetAutoCalcSpliceNum()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(CENTERGIRDER);
	CGlobarOption   *pGloOpt = m_pStd->GetDataManage()->GetGlobalOption();

	double dLength;
	long nCount = 0;
	double dLengthMax = pGloOpt->GetDesignMaxLengthOfFieldSplice()-625;

	if(pBridge->m_nQtyJigan == 1)
	{
		dLength = pGir->GetStationEnd() - pGir->GetStationStt();
		nCount = (long)(dLength / dLengthMax);
	}
	else
	{
		for(long nJ=0; nJ<pBridge->m_nQtyJigan;nJ++)
		{
			dLength = pGir->GetStationOnJijum(nJ+1) - pGir->GetStationOnJijum(nJ);
			if(nJ==0 || nJ==pBridge->m_nQtyJigan-1)
			{
				nCount += (long)((dLength - 5625)/ dLengthMax) + 2;
			}
			else
			{
				nCount += (long)((dLength - 5625*2)/ dLengthMax) + 3;
			}
		}
	}

	return nCount;
}

void CFieldJoinPositionDlg::SetDataBx()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	pBridge->m_cSameApplySameColSplice	 = (BYTE)m_bSameApplySameCol;

	long nCols = m_Grid.GetCols();
	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)		
	{
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		long nSplicesu = 0;
		double dis = 0;
		double sta = pGir->GetSSS(OFF_STT_GIRDER);
		CPtrList NList;

		for(long nCol = 1; nCol < nCols-1; nCol++)
		{
			if(m_Grid.GetTextMatrixDouble(GetRow(nG), nCol+1) > 0)
			{
				nSplicesu++;
				dis  = m_Grid.GetTextMatrixDouble(GetRow(nG), nCol);
				if(dis <= 0)	continue;
				sta = GetStationMatrix(GetRow(nG), nCol);

				// BX_SPLICE
				CPlateBasicIndex* p = pGir->newBx();
				p->SetState(BX_SPLICE,TRUE);
				p->SetStation(sta);
				p->SetShift(m_Grid.IsCheck((GetRow(nG)+2), nCol));
				NList.AddTail(p);
			}
		}

		// set BxList
		CDWordArray LKExceptionArr;
		LKExceptionArr.Add(CPlateBxObject::LK_SPLICE);
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_UPPER);
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_LOWER);
		LKExceptionArr.Add(CPlateBxObject::LK_FACTCHAIN_WEB);		
		pGir->PressBxList(NList,BX_SPLICE,&LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);

		// MakeSplice
		CPlateBxFinder Finder(pGir);
		Finder.GetBxFirst(BX_SPLICE);
		pGir->MakeSplice(Finder.GetCountBx());

		// set LinkToBx
		pGir->SetLKPOS(CPlateBxObject::LK_SPLICE);

		// 현장이음위치에서 분할
		/*
		pGir->SetJRibByDivideSplice(TRUE);		// JRib
		pGir->SetJRibByDivideSplice(FALSE);		// JRib
		pGir->SetFactChainByDivideSplice(0);	// FactChain
		pGir->SetFactChainByDivideSplice(1);	// FactChain
		pGir->SetFactChainByDivideSplice(2);	// FactChain
		pGir->SetFactChainByDivideSplice(3);	// FactChain
		*/
	}
}


void CFieldJoinPositionDlg::OnCellCopiedData(NMHDR* nmgv, LRESULT*)
{

	SetDataSave();
	SetDataBx();
	SetDataInit();

	return;
}

void CFieldJoinPositionDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	if(nRow==1)
	{
		SetDataSave();
		DrawInputDomyunView(FALSE);
		return;
	}

	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	double staNew = GetStationMatrix(nRow, nCol);
	if(nRow==1 || nRow%3!=1)
		m_Grid.SetTextMatrix(nRow, nCol, m_Grid.GetLastChangeData());
	double staDif = staNew - GetStationMatrix(nRow, nCol);

	for (long r = 2; r < m_Grid.GetRows(); r+=3)   {
		if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		double sta = GetStationMatrix(r, nCol) + staDif;
		double staNext = GetStationMatrix(r, nCol+1);
		SetMatrixByStation(r, nCol, sta, TRUE);
		SetMatrixByStation(r, nCol+1, staNext, TRUE);
		CPlateGirderApp* pGir = pBridge->GetGirder(GetNumberGirder(r));
		SetMatrixByStation(r+1, nCol, pGir->GetStationByOneStepMid(sta, 0), TRUE);
	}
	m_Grid.Invalidate();

	DrawInputDomyunView(FALSE);
}

void CFieldJoinPositionDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	
	if(m_nTab==1) return;	// 기능죽임

	long nG2 = -2;
	double presta2, sta2;
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(FALSE);
	pDom->FixEtt(TRUE);

	for (long r = 2; r < m_Grid.GetRowCount(); r+=3) 
	{
		if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		long nG = GetNumberGirder(r);
		double presta, sta;
		presta = GetStationMatrix(r, nCol-1);
		sta = GetStationMatrix(r, nCol);
		if (r == nRow)   {
			nG2 = nG;
			presta2 = presta;
			sta2 = sta;
		}
		else
			Draw.SetCursorPosition(nG, presta, sta, pDom, TRUE, 40, 2, 0);
	}
	// 자기 거더는 맨 마지막에...
	if (nG2 >= -1)
		Draw.SetCursorPosition(nG2, presta2, sta2, pDom, TRUE, 40, 2, 0);
}

void CFieldJoinPositionDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	if(nCol == m_Grid.GetColumnCount()-1)	return;

	UpdateData();

	for(long row = 2; row < m_Grid.GetRowCount(); row++)
	{
		if (!(m_bSameApplySameCol || row == nRow))		continue;
		if(nRow!=1 && nRow%3==1)						continue;
		double dist1 = m_Grid.GetTextMatrixDouble(row, nCol);
		double dist2 = m_Grid.GetTextMatrixDouble(row, nCol + 1);
		m_Grid.SetTextMatrix(row, nCol+1, "%.0f", dist1 + dist2);
	}

	return;
}

void CFieldJoinPositionDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{
	SetGridTitle();
	m_Grid.SetFocus();

	return;
}

void CFieldJoinPositionDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	UpdateData();

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	
	if(m_Grid.GetColumnCount() > nCol+1)
	{
		CString str = m_Grid.GetTextMatrix(1,nCol+1);
		if(str.IsEmpty()) str = "이음판";
		m_Grid.SetTextMatrix(1, nCol, str);
		m_Grid.SetCellType(1, nCol, CT_COMBO);		
	}

	for (long row = 2; row < m_Grid.GetRowCount(); row++)
	{
		if (!(m_bSameApplySameCol || row == nRow))
			continue;
		long nG = GetNumberGirder(row);
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		double sta = GetStationMatrix(row, nCol);
		double staNext = GetStationMatrix(row, nCol+1);
		double staNew = pGir->GetStationByOneStepMid((sta + staNext) / 2, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
		SetMatrixByStation(row, nCol, staNew, TRUE);
		SetMatrixByStation(row, nCol+1, staNext, TRUE);
	}

	SetGridTitle();

	m_Grid.SetFocus();

	return;
}

double CFieldJoinPositionDlg::GetStationMatrix(long nRow, long nCol)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	double lenSum = 0;
	for(long c = 1; c <= nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);
	double sta = pBridge->GetStationByLengthInGrid(FALSE, GetNumberGirder(nRow), lenSum, m_nLengthType, m_dDisCenterOnSplice);
	return sta;
}

void CFieldJoinPositionDlg::SetMatrixByStation(long nRow, long nCol, double sta, BOOL bHistory)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	double lenSum = 0, len;
	
	if (nRow == GetRow(GetNumberGirder(nRow)))
	{  // 길이
		for(long c = 1; c < nCol; c++)
			lenSum += m_Grid.GetTextMatrixDouble(nRow, c);
		len = pBridge->GetLengthByStationInGrid(FALSE, GetNumberGirder(nRow), sta, m_nLengthType, m_dDisCenterOnSplice) - lenSum;
	}
	else
	{  // 이동
		for(long c = 1; c <= nCol; c++)
			lenSum += m_Grid.GetTextMatrixDouble(nRow-1, c);
		len = -(pBridge->GetLengthByStationInGrid(FALSE, GetNumberGirder(nRow), sta, m_nLengthType, m_dDisCenterOnSplice) - lenSum);
		if (fabs(len) <= 1)
			len = 0;
	}
	m_Grid.SetTextMatrix(nRow, nCol, "%.0f", len, DT_CENTER, bHistory);
}

void CFieldJoinPositionDlg::OnSpliceAllGirder() 
{
	UpdateData();
	m_Grid.SetInsertThisOnly(!m_bSameApplySameCol);
	m_Grid.SetDeleteThisOnly(!m_bSameApplySameCol);
}

void CFieldJoinPositionDlg::SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter)
{
	long nRows = m_Grid.GetRows();
	long nCols = m_Grid.GetCols();

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	for (long nRow = 2; nRow < nRows; nRow += 3)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(GetNumberGirder(nRow));
		double staStt = pGir->GetSSS(OFF_STT_GIRDER);
		double sta, lenSum = 0;
		long nEndCol = m_Grid.GetValidCol(nRow, FALSE);
		for (long nCol = 1; nCol < nCols; nCol++)
		{
			lenSum += m_Grid.GetTextMatrixDouble(nRow, nCol);
			if(nCol == nCols-1) 
			{
				sta = pGir->GetSSS(OFF_END_SHOE);
			}
			else
			{
				if (nOldType == 0)
					sta = pGir->GetStationByActLength(staStt, lenSum);
				else
					sta = pGir->GetLine()->GetStationByDisLength(staStt, lenSum, dOldDisCenter);
			}
			SetMatrixByStation(nRow, nCol, sta, TRUE);
			if (nCol < nEndCol)
				SetMatrixByStation(nRow + 1, nCol, pGir->GetStationByOneStepMid(sta, 0), TRUE);
			else
				m_Grid.SetTextMatrix(nRow + 1, nCol, "0");
		}
	}
	m_Grid.Invalidate();
}

void CFieldJoinPositionDlg::OnSpliceGirderlen() 
{
	long nOldType = m_nLengthType;
	double dOldDisCenter = m_dDisCenterOnSplice;
	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);

	SetDataSave();
	SetDataBx();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CFieldJoinPositionDlg::OnSpliceBridgelen() 
{
	long nOldType = m_nLengthType;
	double dOldDisCenter = m_dDisCenterOnSplice;
	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);

	SetDataSave();
	SetDataBx();
	SetDataInit();
	DrawInputDomyunView(FALSE);

}

void CFieldJoinPositionDlg::OnSpliceAllequal() 
{
	long nCurRow = m_Grid.GetFocusCell().row;
	if(nCurRow<=0) nCurRow = 2;

	CString str;
	str.Format("  %d번 거더 기준으로 현장이음을 배치합니다. \n    계속하시겠습니까?", (nCurRow-1)/2);
	if (IDCANCEL == MessageBox(str, "모든 거더 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	long nRows = m_Grid.GetRows();
	long nCols = m_Grid.GetCols();
	long nEndCol = m_Grid.GetValidCol(nCurRow, FALSE);
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pCurGir = pBridge->GetGirder(GetNumberGirder(nCurRow));

	for (long nRow = 2; nRow < nRows; nRow++)   {
		if (nRow == nCurRow)
			continue;
		CPlateGirderApp *pGir = pBridge->GetGirder(GetNumberGirder(nRow));
		long nJijum = 0;
		double staCurStt = pCurGir->GetSSS(OFF_STT_GIRDER);
		double staCurEnd = staCurStt;
		double staStt = pGir->GetSSS(OFF_STT_GIRDER);
		double staEnd = staStt;
		long nCol = 0;
		for (nCol = 1; nCol < nEndCol; nCol++)
		{
			double staCur = GetStationMatrix(nCurRow, nCol);
			double staCurMid = pCurGir->GetStationByOneStepMid(staCur, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
			if (staCur > staCurEnd)   {
				nJijum++;
				staCurStt = staCurEnd;
				staCurEnd = pCurGir->GetStationOnJijum(nJijum);
				staStt = staEnd;
				staEnd = pGir->GetStationOnJijum(nJijum);
			}
			double staMid = (staCurMid - staCurStt) / (staCurEnd - staCurStt) * (staEnd - staStt) + staStt;
			staMid = pGir->GetStationByOneStepMid(staMid, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
			double sta = staMid + (staCur - staCurMid);
			SetMatrixByStation(nRow, nCol, sta, TRUE);
//			SetMatrixByStation(nRow+1, nCol, staMid, TRUE);
		}
		SetMatrixByStation(nRow, nCol, pGir->GetSSS(OFF_END_GIRDER), TRUE);
//		m_Grid.SetTextMatrix(nRow+1, nCol, "0");
		for (nCol++ ; nCol < nCols; nCol++)   {
			m_Grid.SetTextMatrix(nRow, nCol, "");
//			m_Grid.SetTextMatrix(nRow+1, nCol, "");
		}
	}

//	m_Grid.DeleteUnusedColumn();
	SetGridTitle();
	SetDataSave();
	SetDataBx();
	DrawInputDomyunView(FALSE);
}

void CFieldJoinPositionDlg::OnSpliceRightShift() 
{
	MoveLeftRight(1);
	m_Grid.Invalidate();
	m_Grid.SetFocus();	
//	SetDataSave();
//	SetDataBx();
//	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CFieldJoinPositionDlg::OnSpliceLeftShift() 
{
	MoveLeftRight(-1);
	m_Grid.Invalidate();
	m_Grid.SetFocus();	
//	SetDataSave();
//	SetDataBx();
//	SetDataInit();
	DrawInputDomyunView(FALSE);
}

//<summary>
//
//</summary>
//<param name="nDir">항상 -1 또는 +1</param>
void CFieldJoinPositionDlg::MoveLeftRight(long nDir)
{
	UpdateData(TRUE);

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = NULL;
	long	nG = 0,
			nCurRow = m_Grid.GetFocusCell().row,			
			nCurCol = m_Grid.GetFocusCell().col,
			nMoveType = (m_Grid.GetFocusCell().row-1) % 2;
	if(nCurRow==1)	return;

	nCurRow -= (nCurRow - 2) % 2;
	double dSta, dNextSta, dMoveLen;

	for(long nRow = 2; nRow < m_Grid.GetRowCount(); nRow+=3)
	{
		if (!(m_bSameApplySameCol || nRow == nCurRow))
			continue;
		nG = GetNumberGirder(nRow);
		pGir = pBridge->GetGirder(nG);
		dSta = GetStationMatrix(nRow, nCurCol);
		dNextSta = GetStationMatrix(nRow, nCurCol+1);
		if (nMoveType == 1)  // 거리
			dMoveLen = pGir->GetStationByOneStepMid(dSta, nDir, BX_CROSSBEAM_VBRACING | BX_VSTIFF) - pGir->GetStationByOneStepMid(dSta, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF);
		else  // 이동량
			dMoveLen = nDir * 50;
		SetMatrixByStation(nRow, nCurCol, dSta + dMoveLen, TRUE);
		SetMatrixByStation(nRow+1, nCurCol, pGir->GetStationByOneStepMid(dSta + dMoveLen, 0, BX_CROSSBEAM_VBRACING | BX_VSTIFF), TRUE);
		SetMatrixByStation(nRow, nCurCol+1, dNextSta, TRUE);
	}
}

void CFieldJoinPositionDlg::OnSpliceCheckmove() 
{
	UpdateData();
	SetGridTitle();
}
// 자동 대칭 버튼
void CFieldJoinPositionDlg::OnSetDataSymmetry() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	UpdateData(TRUE);

	if( m_bSameApplySameCol )
	{
		for (long nG = -1; nG < pBridge->GetGirdersu(); nG++)
			SpliceAutoSymmetry(nG);
	}
	else
		SpliceAutoSymmetry(GetNumberGirder(m_Grid.GetFocusCell().row));

	m_Grid.DeleteUnusedColumn();
	SetGridTitle();
}

void CFieldJoinPositionDlg::SpliceAutoSymmetry(long nG)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long nCols = m_Grid.GetCols();
	long nRow = GetRow(nG);
	long nPier=1;
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);
	double SttSta = pGir->GetSSS(OFF_STT_GIRDER);
	double EndSta = pGir->GetSSS(OFF_END_GIRDER);
	double HalfSta = (SttSta + EndSta) / 2.0;  // 교량 교대부 사이 중간 == 대칭점(기준점)
	double sta, staMid, staSym;
	CDDWordArray StaArr;
	long nCenCol = 0;
	for (nCenCol = 1; nCenCol < nCols; nCenCol++)
	{
		sta = GetStationMatrix(nRow, nCenCol);
		if(sta > pGir->GetStationOnJijum(nPier))
		{						
			if(nPier >= pBridge->m_nQtyJigan) break;
			SttSta = pGir->GetStationOnJijum(nPier);
			EndSta = pGir->GetStationOnJijum(pBridge->m_nQtyJigan-nPier);
			HalfSta = (SttSta + EndSta) / 2.0;
			++nPier;
		}
		if (sta == HalfSta)   {  // 이런 경우는 진짜 드물거야...
			nCenCol++;
			break;
		}
		if (sta > HalfSta)
			break;
		staMid = pGir->GetStationByOneStepMid(sta, 0);
		staSym = pGir->GetStationByOneStepMid(HalfSta + (HalfSta - staMid), 0);  // 이동전의 상태임.
		staSym -= sta - staMid;  // 이동량 보정.
		StaArr.Add(staSym);
	}
	long nNewCols = nCenCol + StaArr.GetSize() + 1;
	if (m_Grid.GetCols() < nNewCols)
		m_Grid.SetColumnCount(nNewCols);

	long nSymCol = StaArr.GetSize();
	for ( ; nCenCol < nNewCols - 1; nCenCol++)
	{
		staSym = StaArr.GetAt(nSymCol - 1);
		SetMatrixByStation(nRow, nCenCol, staSym, TRUE);
		SetMatrixByStation(nRow+1, nCenCol, pGir->GetStationByOneStepMid(staSym, 0), TRUE);
		nSymCol--;
	}
	// 마지막 구간 저장
	SetMatrixByStation(nRow, nCenCol, pGir->GetSSS(OFF_END_GIRDER), TRUE);
	m_Grid.SetTextMatrix(nRow+1, nCenCol, "%.0f", 0.0);

	// 뒤에 남은 Data 삭제.
	for (nCenCol++; nCenCol < m_Grid.GetCols(); nCenCol++)
	{
		m_Grid.SetTextMatrix (nRow, nCenCol, "");
		m_Grid.SetTextMatrix (nRow+1, nCenCol, "");
	}
}

void CFieldJoinPositionDlg::MoveByMoment() 
{
	UpdateData(TRUE);
	MoveByMomentStress(m_dMoveTerm);
	SetDataSave();
	SetDataBx();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CFieldJoinPositionDlg::MoveByStress() 
{
	UpdateData(TRUE);
	MoveByMomentStress(-m_dMoveTerm);
	SetDataSave();
	SetDataBx();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CFieldJoinPositionDlg::MoveByMomentStress(double dBaseTerm)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	int nRow = m_Grid.GetFocusCell().row;
	long nG  = GetNumberGirder(nRow);

	for (long g = -1; g < pBridge->GetGirdersu(); g++)
	{
		if (!(m_bSameApplySameCol || g == nG))
			continue;
		CPlateGirderApp* pGir = pBridge->GetGirder(g);
		long row = GetRow(g);
		long nEndCol = m_Grid.GetValidCol(row, FALSE);
		for (long col = 1; col < nEndCol; col++)
		{
			double staMid = pGir->GetStationByOneStepMid(GetStationMatrix(row, col), 0);
			double dTerm = GetDirInKyunggan(g, staMid) * dBaseTerm;
			SetMatrixByStation(row, col, staMid + dTerm, TRUE);
			SetMatrixByStation(row+1, col, staMid, TRUE);
		}
	}
	m_Grid.Invalidate();
}
// 현재 스테이션에 위치한 경간에서 최대 정모멘트 기준으로 앞뒤 여부를 구함.
// -1 : 앞쪽,  1 : 뒤쪽
long CFieldJoinPositionDlg::GetDirInKyunggan(long nG, double sta)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CPlateGirderApp *pGir = pBridge->GetGirder(nG);
	long nK = pGir->GetNumberJiganByStation(sta);
	double staStt = pGir->GetStationOnJijum(nK);
	double staEnd = pGir->GetStationOnJijum(nK+1);
	double Ratio = 0.5;
	if (pBridge->m_nQtyJigan > 1)   {
		if (nK == 0)  // 첫경간
			Ratio = 3.0/7.0;
		if (nK+1 == pBridge->m_nQtyJigan)  // 끝경간
			Ratio = 4.0/7.0;
	}
	double staMax = staStt + (staEnd - staStt) * Ratio;
	if (sta < staMax)
		return -1;
	return 1;
}

// 이음종류 설정 그리드(현장이음판)
void CFieldJoinPositionDlg::OnButtonApplyAllSplice() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	while (pBx)
	{
		CPlateSplice *pSp = pBx->GetSplice();
		pSp->m_bSpliceType = TRUE;		

		pBx = Finder.GetBxNext();
	}

	SetDataInit();
	SetDataSave();
	SetDataBx();

	DrawInputDomyunView(FALSE);

}

// 이음종류 설정 그리드(현장이음판)
void CFieldJoinPositionDlg::OnButtonApplyAllWeld() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CPlateBxFinder Finder(pBridge->GetGirder(-1));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
	while (pBx)
	{
		CPlateSplice *pSp = pBx->GetSplice();
		pSp->m_bSpliceType = FALSE;		

		pBx = Finder.GetBxNext();
	}
	SetDataInit();
	SetDataSave();
	SetDataBx();

	DrawInputDomyunView(FALSE);

}

void CFieldJoinPositionDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	if(!m_pStd->GetDlgOfIDD(IDD_DET_12_FIELDJOIN)->IsValid())
	{
		m_pStd->GetDlgOfIDD(IDD_DET_12_FIELDJOIN)->SetUsedFlag(FALSE);
		AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	}

	m_pStd->MoveNextDialog();
}
