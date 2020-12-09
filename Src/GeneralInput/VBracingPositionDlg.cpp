// VBracingPositionDlg.cpp : implementation file
// 다이얼로그 제목 : 가로보 및 수직브레이싱 위치

#include "stdafx.h"
#include "GeneralInput.h"
#include "VBracingPositionDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVBracingPositionDlg dialog

CVBracingPositionDlg::CVBracingPositionDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CVBracingPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVBracingPositionDlg)
	m_dBaseDist = 5000.0;
	m_bSameApplySameCol = TRUE;
	m_cCross90 = 0;
	m_dDisCenterOnDiap = 0.0;
	m_nLengthType = 1;
	//}}AFX_DATA_INIT
}


void CVBracingPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVBracingPositionDlg)
	DDX_Control(pDX, IDC_EDIT_BASEDIST, m_btnBaseDist);
	DDX_Control(pDX, IDC_CROSS_DISTANCE_CENTER, m_ctrlEdit1);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_BASEDIST, m_dBaseDist);
	DDX_Check(pDX, IDC_CHECK_SAMETYPE, m_bSameApplySameCol);
	DDX_Radio(pDX, IDC_RADIO_POSTYPE1, m_cCross90);
	DDX_Text(pDX, IDC_CROSS_DISTANCE_CENTER, m_dDisCenterOnDiap);
	DDX_Radio(pDX, IDC_CROSS_GIRDERLEN, m_nLengthType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_GIRDERSAME_APPLY, m_btnAllSame);
}


BEGIN_MESSAGE_MAP(CVBracingPositionDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CVBracingPositionDlg)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_CHECK_SAMETYPE, OnCheckSametype)
	ON_BN_CLICKED(IDC_BUTTON_GIRDERSAME_APPLY, OnButtonGirderSameApply)
	ON_BN_CLICKED(IDC_CROSS_GIRDERLEN, OnCrossGirderlen)
	ON_BN_CLICKED(IDC_CROSS_BRIDGELEN, OnCrossBridgelen)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_INSERT_COL, IDC_GRID, OnCellInsertCol)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)	
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

BOOL CVBracingPositionDlg::OnInitDialog() 
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();		
	if(pBridge->m_vAngJijum[1].y==1)		m_cCross90 = 0;
	else                              		m_cCross90 = 1;

	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_BUTTON_GIRDERSAME_APPLY,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_TITLE1,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_BASEDIST,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_UNIT,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SAMETYPE,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_GRID,  SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_GROUPBOX,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_POSTYPE1,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_POSTYPE2,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_POSTYPE3,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_GROUPBOX2,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CROSS_GIRDERLEN,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CROSS_BRIDGELEN,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CROSS_DISTANCE_CENTER,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CVBracingPositionDlg message handlers

void CVBracingPositionDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	Dom.SetScaleDim(70);
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	Draw.DrawPyung(&Dom, 6);
	*pDom << Dom;

	// 화면의 중앙으로 정렬
/*	pOptStd->SetEnvType(&Dom, HCAD_HIDD);
	CDRect  BoundRect;
 
	Dom.SetCalcExtRect();
	BoundRect = Dom.GetExtRect();
	BoundRect.left += BoundRect.Width()*0.2;
	BoundRect.top -= BoundRect.Height()*0.5;
	BoundRect.bottom +=BoundRect.Height() * 0.2;
	Dom.Rectangle(BoundRect.left, BoundRect.top, BoundRect.right, BoundRect.bottom);	
	*pDom << Dom;	
	Dom.Invalidate();
*/
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CVBracingPositionDlg::SetGridTitle()
{
	CString str;
	long rows = m_Grid.GetRows();
	long cols = m_Grid.GetCols();

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(rows);
	m_Grid.SetColumnCount(cols);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));

	for(long nCol=1; nCol<m_Grid.GetCols(); nCol++)
	{
		str.Format("%d",nCol);
		m_Grid.SetTextMatrix(0,nCol,str);
	}	
	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,65);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetTextMatrix(0,0,"구 분");

	CString str1;
	long nG = 0;
	for (long r=1; r<rows; r+=2)
	{
		if(r == 1)
		{
			str = "길이(CR)";
			str1= "각도(CR)";
		}
		else   
		{
			str.Format ("길이(G%d)", nG);
			str1.Format("각도(G%d)", nG);
		}
		m_Grid.SetTextMatrix(r,0,str);
		m_Grid.SetTextMatrix(r+1,0,str1);
		m_Grid.SetRowHeight(r+1, 0);
		nG++;
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CVBracingPositionDlg::SetDataInit()
{
	m_Grid.SetRedraw();

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	m_dBaseDist			= pBridge->m_dLengthBaseCrossBeam;
	m_cCross90			= (BYTE)pBridge->m_nCrossArrangeType;
	m_bSameApplySameCol	= (BOOL)pBridge->m_cSameApplySameColCross;		
	m_nLengthType       = pBridge->m_nLengthTypeOnCrBeam;

	if(m_dBaseDist == 0) m_dBaseDist = 5000;

	CString str			= _T("");
	long	rows		= pBridge->m_CrossAngleGrid.GetRows();
	long	cols		= pBridge->m_CrossAngleGrid.GetCols();
	long	nNewRows	= pBridge->GetGirdersu()*2 + 3;

	m_Grid.SetRowCount(nNewRows);
	m_Grid.SetColumnCount(cols);

	// 그리드 복제==> 거더수가 변경시 변경전과 변경후 그리드중 작은것을 저장
	long r = 0;
	for(r = 0; r < min(rows, nNewRows); r++)
	{
		for (long c = 0; c < cols; c++)
		{
			if(atof(pBridge->m_CrossAngleGrid.GetTextMatrix(r, c)) < 0)
			{
				m_Grid.SetTextMatrix(r, c, "0");
			}
			else
			{
				m_Grid.SetTextMatrix(r, c, pBridge->m_CrossAngleGrid.GetTextMatrix(r, c));
				if(atol(pBridge->m_CrossAngleGrid.GetTextMatrix(r, c))==0 && c!=0 && r!=0)
				{
					m_Grid.SetItemState(r, c, m_Grid.GetItemState(r, c) | GVIS_READONLY);
					m_Grid.SetItemBkColour(r, c, RGBREADONLY);
				}
				else if(c!=0 && r!=0)
				{
					m_Grid.SetItemState(r, c, m_Grid.GetItemState(r, c) & ~GVIS_READONLY);
					m_Grid.SetItemBkColour(r, c, RGB(225, 250, 250));
				}
			}
		}
	}
	// 거더수 증가면 마지막 거더 복사.
	for(r = rows; r < nNewRows; r += 2)
	{
		for (long c = 0; c < cols; c++)   
		{
			m_Grid.SetTextMatrix(r, c, pBridge->m_CrossAngleGrid.GetTextMatrix(rows-2, c));
			m_Grid.SetTextMatrix(r+1, c, pBridge->m_CrossAngleGrid.GetTextMatrix(rows-1, c));
		}
	}
	// 거더의 길이가 바뀌었을 경우 그리드 뒤쪽의 값을 수정.
	double dMiss = 10.0;
	for(r=1; r<nNewRows; r+= 2)   
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(r/2-1);
		double staEnd  = pGir->GetSSS(OFF_END_SHOE);
		long   nEndCol = m_Grid.GetValidCol(r, FALSE);
		long c = 0;
		for (c = 1; c < nEndCol; c++)
			if (GetStationMatrix(r, c) + dMiss >= staEnd)
				break;
		SetMatrixByStation(r, c, staEnd);
		m_Grid.SetTextMatrix(r+1, c, "90");
		for (c++ ; c < cols; c++)   
		{
			m_Grid.SetTextMatrix(r, c, "0");
			m_Grid.SetTextMatrix(r+1, c, "-");
		}
	}
	m_Grid.DeleteUnusedColumn();
	m_Grid.SetRedraw(TRUE,TRUE);
	// 수정된 그리드 값을 pBridge->m_CrossAngleGrid 에 저장함(갱신)
	SetDataSaveGrid();

	UpdateData(FALSE);

	m_Grid.SetInsertThisOnly(!m_bSameApplySameCol);
	m_Grid.SetDeleteThisOnly(!m_bSameApplySameCol);

	SetGridTitle();
}

void CVBracingPositionDlg::SetDataDefault()
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();		
	
	UpdateData();
	
	long cols = GetCrossGridSuInBridge()+1;
	long rows = (pBridge->GetGirdersu()+1)*2 + 1;

	m_Grid.SetRowCount(rows);
	m_Grid.SetColumnCount(cols);

	CDDWordArray dDisArr;
	CString str;

	double angDeg = 90;
	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		if(m_cCross90 == 0)					//교량 중앙 기준
		{
			GetDisArr(nG, dDisArr);
		}
		else if(m_cCross90 == 1)			//다이아프램 동일 간격
		{
			GetDisArrDiap(nG, dDisArr);
		}
		else if(m_cCross90 == 2)			//가로보 동일 각도
		{
			GetDisArrCross(nG, dDisArr);
		}

		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		double sta = pGir->GetSSS(OFF_STT_SHOE);

		long nSize = dDisArr.GetSize();
		long r = nG*2 + 3;
		for(long n = 0; n < nSize; n++)
		{
			angDeg = 90;
			sta += dDisArr[n];
			SetMatrixByStation(r, n+1, sta);		//그리드에 나타나는 거리를 세팅함
			str.Format("%.lf", angDeg);
			if (dDisArr[n] == 0)
				str = "-";
			m_Grid.SetTextMatrix(r+1, n+1, str);	//그리드에 나타나는 각도를 세팅함
		}
	}
	m_Grid.DeleteUnusedColumn();
	SetDataSaveGrid();

	m_Grid.SetRedraw(TRUE,TRUE);
	SetDataBx();
	DrawInputDomyunView(FALSE);

	UpdateData(FALSE);
	SetDataSave();
}

//<summary>
//현재 거더의 다이아프램 배치 간격을 구함.(실제 
//</summary>
//<param name="nG">거더 번호</param>
//<param name="&dDisArr">현재 거더의 다이아프램 배치 간격</param>
void CVBracingPositionDlg::GetDisArr(long nG, CDDWordArray &dDisArr)
{
	dDisArr.RemoveAll();

	CDDWordArray dDisKyungArr;
	long nQtyJigan = m_pStd->GetBridge()->m_nQtyJigan;
	for(long nJigan = 0; nJigan<nQtyJigan; nJigan++)
	{
		if(nQtyJigan == 1)	GetDisArrJiganCenter(nG, nJigan, dDisKyungArr);	//단지간 일 경우
		else
		{
			if(nJigan==0)					GetDisArrMultiJiganFirst(nG, nJigan, dDisKyungArr);  //첫번째 지간
			else if(nJigan==nQtyJigan-1)	GetDisArrMultiJiganLast(nG, nJigan, dDisKyungArr);	 //마지막 지간
			else							GetDisArrJiganCenter(nG, nJigan, dDisKyungArr);		 //중앙부 지간
		}		
		long dDisKyungArrSize = dDisKyungArr.GetSize();
		
		for(long n=0; n<dDisKyungArrSize; n++)		//0번 지간일 경우
			dDisArr.Add(dDisKyungArr[n]);

		dDisKyungArr.RemoveAll();
	}

}

void CVBracingPositionDlg::SetDataSaveGrid()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long rows = m_Grid.GetRows();
	long cols = m_Grid.GetCols();
	pBridge->m_CrossAngleGrid.Resize(rows, cols);

	for (long r = 0; r < rows; r++)
		for (long c = 0; c < cols; c++)
			pBridge->m_CrossAngleGrid.SetTextMatrix(r, c, m_Grid.GetItemText(r, c));

}

void CVBracingPositionDlg::SetDataSave()
{		
	UpdateData(TRUE);

	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();

	pBridge->m_dLengthBaseCrossBeam  = m_dBaseDist;
	pBridge->m_nCrossArrangeType     = m_cCross90;
	pBridge->m_cSameApplySameColCross= (BYTE)m_bSameApplySameCol;
	pBridge->m_nLengthTypeOnCrBeam   = m_nLengthType;

	m_Grid.DeleteUnusedColumn();
	SetDataSaveGrid();
	SetDataBx();

	pData->DESIGN_BASE.m_szBridgeLineType = GetLineInfo();
	CPlateBxFinder Finder(pBridge->GetGirder(0));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	pData->DESIGN_BASE.m_dBridgeWidth = toM(pBridge->GetWidthSlabDom(pBx));
	
}

void CVBracingPositionDlg::SetDataBx()
{
	SetBxBending();
	SetBxCross();	
	SetBxJackupHeight();
	SetBxJijumVStiffHeight();
	SetBxByun();
}


void CVBracingPositionDlg::SetBxBending()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	double dSta = 0;
	CDPoint vAng;
	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		CPtrList NList;
		CPlateBasicIndex * p;
		CPlateGirderApp* pGir = pBridge->GetGirder(nG);

		if(pBridge->m_dLengthTaperToTaperStt > 0 && pBridge->m_dHeightTaperStt > 0)
		{
			dSta = pGir->GetSSS(OFF_STT_HUNCHSTT);
			vAng = pGir->GetAAA(OFF_STT_HUNCHSTT);
			p = pGir->newBx();
			p->SetState(BX_STT_BENDING_LOWER,TRUE);
			p->SetStation(dSta);
			p->SetAngle(vAng);
			NList.AddTail( p );

			dSta = pGir->GetSSS(OFF_STT_HUNCHEND);
			vAng = pGir->GetAAA(OFF_STT_HUNCHEND);
			p = pGir->newBx();
			p->SetState(BX_STT_BENDING_UPPER,TRUE);
			p->SetStation(dSta);
			p->SetAngle(vAng);
			NList.AddTail( p );
		}
		if(pBridge->m_dLengthTaperToTaperEnd > 0 && pBridge->m_dHeightTaperEnd > 0)
		{
			dSta = pGir->GetSSS(OFF_END_HUNCHSTT);
			vAng = pGir->GetAAA(OFF_END_HUNCHSTT);
			p = pGir->newBx();
			p->SetState(BX_END_BENDING_LOWER,TRUE);
			p->SetStation(dSta);
			p->SetAngle(vAng);
			NList.AddTail( p );

			dSta = pGir->GetSSS(OFF_END_HUNCHEND);
			vAng = pGir->GetAAA(OFF_END_HUNCHEND);
			p = pGir->newBx();
			p->SetState(BX_END_BENDING_UPPER,TRUE);
			p->SetStation(dSta);
			p->SetAngle(vAng);
			NList.AddTail( p );
		}
			// set BxList
//			CDWordArray LKExceptionArr;
//			LKExceptionArr.Add(CBxObject::LK_JACKUP);
			pGir->PressBxList(NList,BX_BENDING);
			AhPtrListDelete(NList,(CPlateBasicIndex*)0);

			pGir->SetBxPosLeftRight(BX_STT_BENDING_LOWER);
			pGir->SetBxPosLeftRight(BX_STT_BENDING_UPPER);
			pGir->SetBxPosLeftRight(BX_END_BENDING_LOWER);
			pGir->SetBxPosLeftRight(BX_END_BENDING_UPPER);
	}
}

void CVBracingPositionDlg::SetBxCross()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	long nG = 0;
	for(nG = -1; nG < pBridge->GetGirdersu(); nG++)
		SetBxCrossSub(nG);

	for(nG = -1; nG < pBridge->GetGirdersu(); nG++)
		pBridge->GetGirder(nG)->SetBxPosMatchCross();

}


///////////////////////////////////////////////////////////////////////////////
/// 기능 : 가로보 Bx 세팅함
///////////////////////////////////////////////////////////////////////////////
void CVBracingPositionDlg::SetBxCrossSub(long nG)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);

	

	long   nJijum  = 0;
	double staPier = pGir->GetStationOnJijum(nJijum);
	double sta     = staPier;
	long   cols    = pBridge->m_CrossAngleGrid.GetCols();

	CPtrList NList;

	long nCount = 0;
	for(long nCol = 0; nCol < cols; nCol++)
	{
		if (nCol > 0)
		{
			double dis = pBridge->m_CrossAngleGrid.GetTextMatrixDouble(nG*2+3, nCol);
			if( dis <= 0 )  continue;
			sta = GetStationMatrix(nG*2+3, nCol);
		}
		///<set 지점(마지막 지점[A2]의 스테이션 점검 때문에 +10 처리
		///<지점부가로보설정
		if(sta > staPier-10)	
		{
			CVector vAng;
			//BX_CROSSBEAM_VBRACING
			CPlateBasicIndex* p = pGir->newBx();
			NList.AddTail(p);
			p->SetState(BX_CROSSBEAM_VBRACING,TRUE);
			p->SetStation(staPier);
			vAng = pGir->GetAngleOnJijum(nJijum);
			p->SetAngle(vAng);
			p->SetMatchCol(-(nJijum+1)*10);	// -10, -20, -30 ...			
			
			if(nJijum > pBridge->m_nQtyJigan)		break;
			staPier = pGir->GetStationOnJijum(++nJijum);
			++nCount;
		}			
		///<지점부이외의 가로보설정
		if (pBridge->IsSafeCross(nG*2+3, nCol))// set Cross
		{
			//BX_CROSSBEAM_VBRACING
			CPlateBasicIndex* p = pGir->newBx();
			NList.AddTail(p);
			p->SetState(BX_CROSSBEAM_VBRACING,TRUE);
			p->SetStation(sta);
			p->SetMatchCol(nCol);			
			double ang = ToRadian(pBridge->m_CrossAngleGrid.GetTextMatrixDouble(nG*2+4, nCol) - 90);
			CDPoint vAng(cos(ang),sin(ang));
			vAng = vAng.Rotate90();
			++nCount;
		}
	}
	pGir->MakeBracing(nCount);	
	pGir->MakeCrossSection(nCount);

	// set BxList
	CDWordArray LKExceptionArr;			
	LKExceptionArr.Add(CPlateBxObject::LK_BRACING);	
	pGir->PressBxList(NList,BX_CROSSBEAM_VBRACING,&LKExceptionArr);
	AhPtrListDelete(NList,(CPlateBasicIndex*)0);

	pGir->SetLKPOS(CPlateBxObject::LK_BRACING);		
	pGir->SetLKPOS(CPlateBxObject::LK_CROSSBEAM);

	//일반부 가로보도 각도 정보가 필요함
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxS = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	while(pBxS)
	{
		if(pBxS->IsJijum())
		{
			pBxS = Finder.GetBxNext();
			continue;
		}
		CDPoint xyAng = (nG == pBridge->GetGirdersu()-1) ? pGir->FindAngleSectionLeft(pBxS) : pGir->FindAngleSectionRight(pBxS);
		pBxS->SetAngle(xyAng);

		pBxS = Finder.GetBxNext();
	}

}

//기능 : 전체 교량의 크로스빔 그리드 수를 산정하여 리턴-모든 거더의 그리드수(컬럼 수)는 꼭 같음
long CVBracingPositionDlg::GetCrossGridSuInBridge()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	long nGridSu = 0;
	for(long nJigan = 0; nJigan < pBridge->m_nQtyJigan; nJigan++)
		nGridSu += GetCrossGridSuInOneJigan(nJigan);

	return nGridSu;
}

//입력 : nJigan   크로스빔 그리드 수를 산정하려는 지간
//기능 : 해당 지간의 크로스빔 그리드 수를 산정하여 리턴-모든 거더의 그리드수(컬럼 수)는 꼭 같음
long CVBracingPositionDlg::GetCrossGridSuInOneJigan(long nJigan)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	if(nJigan > pBridge->m_nQtyJigan)	return -1;
	double sttSta = GetStationOnCRLineByJijum(nJigan);
	double endSta = GetStationOnCRLineByJijum(nJigan+1);
	double len = endSta - sttSta;
	len = pBridge->m_pLine->GetLengthByDis(sttSta, endSta, 0);

	long nGridSu = (long)((len - 1)/m_dBaseDist) + 1;

	return nGridSu;
}

//<summary>
//항상 기본 선형 스테이션을 리턴함.
//  시점일 경우 - 모든 거더(기준선형 거더 제외)의 시점 스테이션중에서 가장 작은 기본선형 스테이션
//  종점일 경우 - 모든 거더(기준선형 거더 제외)의 시점 스테이션중에서 가장 큰 기본선형 스테이션
//  중앙 지점일 경우 - 가장 좌측 거더와 우측거더 중앙의 좌표에서 기본선형에 수선을 내린 스테이션
//</summary>
//<param name="nJijum">교대,교각 번호 0, 1, 2...</param>
//<return></return>
double CVBracingPositionDlg::GetStationOnCRLineByJijum(long nJijum)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	if(nJijum > pBridge->m_nQtyJigan)		return -1;
	if(nJijum == 0)						return GetStationOnCRLineStt();
	if(nJijum == pBridge->m_nQtyJigan)	return GetStationOnCRLineEnd();

	CPlateGirderApp* pGirLeft = pBridge->GetGirder(0);
	CPlateGirderApp* pGirRight = pBridge->GetGirder(pBridge->GetGirdersu()-1);

	CDPoint sttXY = pGirLeft->GetXyGirderDis(pGirLeft->GetStationOnJijum(nJijum), 0, pGirLeft->GetAngleOnJijum(nJijum));
	CDPoint endXY = pGirRight->GetXyGirderDis(pGirRight->GetStationOnJijum(nJijum), 0, pGirRight->GetAngleOnJijum(nJijum));
	CDPoint midXY = (sttXY + endXY) / 2;

	double staReturn = 0;
	pBridge->GetGirder(-1)->GetLine()->GetStationMatchVerticalByXy(midXY, staReturn);

	return staReturn;
}

//<summary>
//모든 거더(기준선형 거더 제외)의 시점 스테이션중에서 가장 작은 기본선형 스테이션
//</summary>
//<return></return>
double CVBracingPositionDlg::GetStationOnCRLineStt()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	double sta = 0;
	double staResult = 0;
	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CDPoint xy = pGir->GetXyGirderDis(pGir->GetSSS(OFF_STT_SHOE));
		pBridge->GetGirder(-1)->GetLine()->GetStationMatchVerticalByXy(xy, sta);
		if(nG == 0)	staResult = sta;
		staResult = min(staResult, sta);
	}
	return staResult;
}

//<summary>
//모든 거더(기준선형 거더 제외)의 시점 스테이션중에서 가장 큰 기본선형 스테이션
//</summary>
//<return></return>
double CVBracingPositionDlg::GetStationOnCRLineEnd()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	double sta = 0;
	double staResult = 0;
	for(long nG = 0; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CDPoint xy = pGir->GetXyGirderDis(pGir->GetSSS(OFF_END_SHOE));
		pBridge->GetGirder(-1)->GetLine()->GetStationMatchVerticalByXy(xy, sta);
		if(nG == 0)	staResult = sta;
		staResult = max(staResult, sta);
	}
	return staResult;
}
// 가로보 길이 Setting Button
void CVBracingPositionDlg::OnButtonRecommend() 
{
	if(m_pStd->GetDlgOfIDD(IDD_GEN_17_DANMYUNTYPE)->IsUsedFlag())
		AfxMessageBox("가로보 및 수직브레이싱의 배치가 변경되어 단면타입을 초기화 합니다.",MB_ICONWARNING);
	SetDataDefault();
	SetDataSave();
	SetDataInit();
	SetDataBx();

	DrawInputDomyunView(TRUE);
}

void CVBracingPositionDlg::SetGridCellInsertedCol(int nRow, int nCol)
{	
	long rows = m_Grid.GetRowCount();
	long curRow = nRow;
	nRow -= (nRow - 1) % 2;  // 길이Row로 바꿈.

	for (long r = 1; r < rows; r+=2)   
	{
		if (!m_bSameApplySameCol && r != nRow)  // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		if (!m_bSameApplySameCol)   {
			for (long n = 0; n < 2; n++)
				if (r + n != curRow)
					m_Grid.InsertColumn("", DT_CENTER|DT_VCENTER|DT_SINGLELINE, nCol, r + n);
		}
		double Dis = atof(m_Grid.GetItemText(r, nCol+1));
		m_Grid.SetTextMatrix(r, nCol, "%.1lf",(double)(Dis/2));
		m_Grid.SetTextMatrix(r, nCol+1,"%.1lf",(double)(Dis/2));
		m_Grid.SetTextMatrix(r+1,nCol, (Dis == 0) ? "-" : "90");
	}
}

void CVBracingPositionDlg::OnCellInsertCol(NMHDR* nmgv, LRESULT*)
{
	if(m_pStd->GetDlgOfIDD(IDD_GEN_17_DANMYUNTYPE)->IsUsedFlag())
		AfxMessageBox("가로보 및 수직브레이싱의 배치가 변경되어 단면타입을 초기화 합니다.",MB_ICONWARNING);
	return;
}

void CVBracingPositionDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{			
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	UpdateData();
	SetGridCellInsertedCol(nRow, nCol);
	SetDataSave();
	SetDataBx();
	
	SetGridTitle();
	DrawInputDomyunView(FALSE);
	return;
}

void CVBracingPositionDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{			
	SetDataSave();
	SetDataBx();
	if(m_pStd->GetDlgOfIDD(IDD_GEN_17_DANMYUNTYPE)->IsUsedFlag())
		m_pStd->GetDlgOfIDD(IDD_GEN_17_DANMYUNTYPE)->SetDataDefault();
	SetGridTitle();
	DrawInputDomyunView(FALSE);
	return;
}

void CVBracingPositionDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{	
	if(m_pStd->GetDlgOfIDD(IDD_GEN_17_DANMYUNTYPE)->IsUsedFlag())
		AfxMessageBox("가로보 및 수직브레이싱의 배치가 변경되어 단면타입을 초기화 합니다.",MB_ICONWARNING);
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	long rows = m_Grid.GetRowCount();
	for (long r = 1; r < rows; r += 2)
	{
		if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		double Dis1 = m_Grid.GetTextMatrixDouble(r, nCol);
		double Dis2 = m_Grid.GetTextMatrixDouble(r, nCol+1);
		m_Grid.SetTextMatrix(r, nCol+1, "%.1lf", Dis1+Dis2);
		if (!m_bSameApplySameCol)
			for (long n = 0; n < 2; n++)
				if (r + n != GridView->iRow)
					m_Grid.DeleteColumn(nCol, r + n);			
	}
	m_Grid.DeleteUnusedColumn();
	m_Grid.SetRedraw(TRUE,TRUE);
	return;
}

void CVBracingPositionDlg::OnCheckSametype() 
{
	UpdateData();
	m_Grid.SetInsertThisOnly(!m_bSameApplySameCol);
	m_Grid.SetDeleteThisOnly(!m_bSameApplySameCol);	
}

double CVBracingPositionDlg::GetStationMatrix(long nRow, long nCol)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	double lenSum = 0;
	for(long c = 1; c <= nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);
	double sta = pBridge->GetStationByLengthInGrid(TRUE, nRow/2-1, lenSum, 1, pBridge->m_dDisCenterOnVst);
	return sta;
}


//<summary>
//해당 그리드의 거리를 세팅함
//</summary>
//<param name="nRow">가로보 다이얼로그 그리드의 행</param>
//<param name="nCol">가로보 다이얼로그 그리드의 열</param>
//<param name="sta">해당 그리드의 스테이션</param>
void CVBracingPositionDlg::SetMatrixByStation(long nRow, long nCol, double sta)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	double			lenSum	= 0;
	long			nG		= nRow/2 - 1;
	CPlateGirderApp *pGir	= pBridge->GetGirder(nG);
	double			dLen	= 0;

	if(sta <= pGir->GetStationOnJijum(0) || sta > pGir->GetStationOnJijum(pBridge->m_nQtyJigan))
	{
		m_Grid.SetTextMatrix(nRow, nCol, "%.1f",dLen);
		return;
	}
	
	for(long c = 1; c < nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);
	
	dLen = pBridge->GetLengthByStationInGrid(TRUE, nG, sta, m_nLengthType, m_dDisCenterOnDiap)-lenSum;	
	m_Grid.SetTextMatrix(nRow, nCol, "%.1f", dLen);
}

//	double dDifDis = dDis - dPrevDis;
void CVBracingPositionDlg::SetSelEditByUser(long row,double dDifDis, BOOL m_bNext)
{
	if(dDifDis == 0) return;	// 값이 변하지 않은 경우
//	if(row % 2 == 0) return;	// 각도 셀의 경우
	long col = m_Grid.GetFocusCell().col;
	long cols = m_Grid.GetColumnCount();

	if(m_bNext) col++;
	// 수정된 Column위치 작업
	CString str;
	double DifLen = dDifDis, Len, MoveLen;
	long n = 0;
	for (n = col; n < cols; n++)   {
		Len = atof(m_Grid.GetItemText(row,n));
		if (Len == 0)
			continue;
		if (DifLen > 0)   // 뒤로 밀림.
			MoveLen = DifLen;
		else   // 앞으로 당겨짐.
			MoveLen = -min(Len, -DifLen);
		DifLen -= MoveLen;
		Len += MoveLen;

		str.Format("%.1lf",Len);
		m_Grid.SetTextMatrix(row,n,str);
		if (DifLen == 0.0)
			break;
	}
	// 마지막 Column위치 작업
	DifLen = dDifDis;
	for (n = cols - 1; n > col; n--)   {
		Len = atof(m_Grid.GetItemText(row,n));
		if (Len == 0)
			continue;
		if (DifLen > 0)   // 뒤로 밀림.
			MoveLen = min(Len, DifLen);
		else   // 앞으로 당겨짐.
			MoveLen = DifLen;
		DifLen -= MoveLen;
		Len -= MoveLen;

		str.Format("%.1lf",Len);
		m_Grid.SetTextMatrix(row,n,str);
		if (DifLen == 0.0)
			break;
	}
}

void CVBracingPositionDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{			
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	
	//DrawInputDomyunView(FALSE);
	m_pView->GetDomyun()->ClearEtt(FALSE);

	long nG2 = -2;
	double presta2, sta2;
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->FixEtt(TRUE);
	for (long r = 1; r < m_Grid.GetRowCount(); r+=2)   {
		if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		long nG = (r-3) / 2;
		double presta, sta;
		presta = GetStationMatrix(r, nCol-1);
		sta = GetStationMatrix(r, nCol);
		if (r == nRow)   {
			nG2 = nG;
			presta2 = presta;
			sta2 = sta;
		}
		else
			Draw.SetCursorPosition(nG, presta, sta, pDom, TRUE, 50, 2, 0);
	}
	// 자기 거더는 맨 마지막에...
	if (nG2 >= -1)
		Draw.SetCursorPosition(nG2, presta2, sta2, pDom, TRUE, 50, 2, 0);
	pDom->FixEtt(FALSE);
}

void CVBracingPositionDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{			
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	if (nRow % 2 == 0) 
	{
		SetDataSaveGrid();
		return;
	}
	double dPrevDis = atof(m_Grid.GetLastChangeData());
	double dDifDis = atof(m_Grid.GetItemText(nRow,nCol)) - dPrevDis;
	double dDifSta;

	if (m_nLengthType == 0)   {  // dDifDis를 선형길이로 변환.
		double staNew = GetStationMatrix(nRow, nCol);
		m_Grid.SetTextMatrix(nRow,nCol,"%.1lf",dPrevDis);
		dDifSta = staNew - GetStationMatrix(nRow, nCol);
	}
	else
		m_Grid.SetTextMatrix(nRow,nCol,"%.1lf",dPrevDis);

	for (long r = 1; r < m_Grid.GetRows(); r+=2)   {
		if (!m_bSameApplySameCol && r != nRow)   // 모든거더적용 아닐경우 자기 거더만 작업
			continue;
		if (m_nLengthType == 0)   {
			CPlateGirderApp *pGir = pBridge->GetGirder(r/2-1);
			double staStt = pGir->GetSSS(OFF_STT_SHOE), len = 0;
			for (long c = 1; c <= nCol; c++)
				len += atof(m_Grid.GetItemText(r,c));
			double sta = pGir->GetStationByActLength(staStt, len);
			dDifDis = Round(pGir->GetLengthDimType(sta, sta + dDifSta), 0) * (dDifSta > 0 ? 1 : -1);
		}

		SetSelEditByUser(r,dDifDis);
	}
	SetDataSaveGrid();	
	m_Grid.SetRedraw(TRUE,TRUE);
	SetDataBx();
	DrawInputDomyunView(FALSE);
}

//지간이 2개 이상일 경우, 시점부
void CVBracingPositionDlg::GetDisArrMultiJiganFirst(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
	dDisArr.RemoveAll();
	CDDWordArray dDisKyungArr;
	double staDiff = 0;
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	double staSttGir = GetStationGirderLineByJijum(nG, nJigan);		//거더 슈 시점 스테이션
	double staEndGir = GetStationGirderLineByJijum(nG, nJigan+1);	//거더 슈 종점 스테이션
	double sta = staEndGir;//종방향 교랑 중앙 스테이션

	long nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	long n = 0;
	for(n = 0; n<nKyungGridSu; n++)//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		staDiff = sta - pGir->GetLine()->GetStationByDisLength(sta, -dLengthBase, m_dDisCenterOnDiap);
		if(n == nKyungGridSu-1 && sta >= staSttGir){
			staDiff = sta-staSttGir;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}else if(sta-staDiff >= staSttGir){
			sta -= staDiff;
		}else if(bFirst && sta-staDiff < staSttGir){
			staDiff = sta - staSttGir;
			sta -= staDiff;
			bFirst = FALSE;
		}else{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=dDisKyungArrSize-1; n>=0; n--)		//0번 지간일 경우
		dDisArr.Add(dDisKyungArr[n]);
}
//지간이 2개 이상일 경우, 종점부
void CVBracingPositionDlg::GetDisArrMultiJiganLast(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
	dDisArr.RemoveAll();
	CDDWordArray dDisKyungArr;
	double staDiff = 0;
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	double staSttGir = GetStationGirderLineByJijum(nG, nJigan);//거더 시점 스테이션
	double staEndGir = GetStationGirderLineByJijum(nG, nJigan+1);//거더 종점 스테이션
	double sta = staSttGir;//종방향 교랑 중앙 스테이션

	long nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	long n = 0;
	for(n = 0; n<nKyungGridSu; n++)
	{
		staDiff = pGir->GetLine()->GetStationByDisLength(sta, dLengthBase, m_dDisCenterOnDiap) - sta;
		if(n == nKyungGridSu-1 && sta <= staEndGir){
			staDiff = staEndGir - sta;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}else if(sta+staDiff <= staEndGir){
			sta += staDiff;
		}else if(bFirst && sta+staDiff > staEndGir){
			staDiff = staEndGir-sta;
			sta += staDiff;
			bFirst = FALSE;
		}else{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=0; n<dDisKyungArrSize; n++)
		dDisArr.Add(dDisKyungArr[n]);
}


//<summary>
//현재 거더의 다이아프램 배치 간격을 구함.
//가로보 동일각도
//</summary>
//<param name="nG">거더 번호</param>
//<param name="&dDisArr">현재 거더의 다이아프램 배치 간격</param>
void CVBracingPositionDlg::GetDisArrCross(long nG, CDDWordArray &dDisArr)
{
	dDisArr.RemoveAll();

	CDDWordArray dDisKyungArr;
	long nQtyJigan = m_pStd->GetBridge()->m_nQtyJigan;
	for(long nJigan = 0; nJigan<nQtyJigan; nJigan++)
	{
//		if(nQtyJigan == 1)	GetDisArrJiganCenter(nG, nJigan, dDisKyungArr);	//단지간 일 경우
//		else
		{
			if(nJigan==0)					GetDisArrCrossMultiJiganFirst(nG, nJigan, dDisKyungArr);  //첫번째 지간
			else if(nJigan==nQtyJigan-1)	GetDisArrCrossMultiJiganLast(nG, nJigan, dDisKyungArr);	 //마지막 지간
			else							GetDisArrCrossJiganCenter(nG, nJigan, dDisKyungArr);		 //중앙부 지간
		}		
		long dDisKyungArrSize = dDisKyungArr.GetSize();
		
		for(long n=0; n<dDisKyungArrSize; n++)		//0번 지간일 경우
			dDisArr.Add(dDisKyungArr[n]);

		dDisKyungArr.RemoveAll();
	}

}

//단지간, 다지간의 중앙부 경우
//입력 : nG
//Bridge 의 중앙 스테이션(sttStaBri+endStaBri)/2에서 좌우로 구분하여 정렬
//
void CVBracingPositionDlg::GetDisArrCrossJiganCenter(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
	CLineInfo  *pLine = pGir->GetLine();
	double dLengthBase = m_dBaseDist;
	long   nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	long   nKyungGridSuHalf = (long)nKyungGridSu/2;
	double staDiff = 0;
	double staSttGir = pGir->GetStationOnJijum(nJigan);//거더선형 스테이션,거더 시점 스테이션
	double staEndGir = pGir->GetStationOnJijum(nJigan+1);//거더선형 스테이션,거더 종점 스테이션
	double staCR = (GetStationOnCRLineByJijum(nJigan)+GetStationOnCRLineByJijum(nJigan+1))/2;//기준선형 스테이션, 교랑 중앙 스테이션
	double sta = (staSttGir + staEndGir)/2;	//기준선형 스테이션, 교랑 중앙 스테이션
	
	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	CDDWordArray dDisKyungArr;
	long n = 0;
	for(n = 0; n<nKyungGridSuHalf; n++)	//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		if(n==0 && nKyungGridSu%2==1)//가로보 구간이 홀수 개이고 중앙부 첫번째일 경우
		{
			staDiff = sta - pLine->GetStationByDisLength(sta, -dLengthBase/2, m_dDisCenterOnDiap);
			sta -= staDiff;
		}
		if(n == nKyungGridSuHalf-1 && sta > staSttGir)//마지막 그리드이고 현재 스테이션이 거더 스테이션보다 클 경우
		{
			staDiff = sta-staSttGir;		
		}
		else if(sta-staDiff > staSttGir)
		{
			staDiff = sta - pLine->GetStationByDisLength(sta, -dLengthBase, m_dDisCenterOnDiap);
			sta -= staDiff;
		}
		else if(bFirst && sta-staDiff < staSttGir)
		{
			staDiff = sta - staSttGir;
			sta -= staDiff;
			bFirst = FALSE;
		}
		else
		{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=dDisKyungArrSize-1; n>=0; n--)		//0번 지간일 경우
		dDisArr.Add(dDisKyungArr[n]);

	dDisKyungArr.RemoveAll();

	sta = (staSttGir + staEndGir)/2;//종방향 교랑 중앙 스테이션
	bFirst = TRUE;
	for(n = 0; n<nKyungGridSuHalf; n++)//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		if(n==0 && nKyungGridSu%2==1)
		{
			staDiff = pLine->GetStationByDisLength(sta, dLengthBase/2, m_dDisCenterOnDiap) - sta;
			sta += staDiff;
			dDisArr.Add(dLengthBase);//중앙부에 가로보 없음.
		}

		if(n == nKyungGridSuHalf-1 && sta < staEndGir)
		{
			staDiff = staEndGir - sta;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}
		else if(sta+staDiff < staEndGir)
		{
			staDiff = pLine->GetStationByDisLength(sta, dLengthBase, m_dDisCenterOnDiap) - sta;
			sta += staDiff;
		}
		else if(bFirst && sta+staDiff > staEndGir)
		{
			staDiff = staEndGir-sta;
			sta += staDiff;
			bFirst = FALSE;
		}
		else
		{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=0; n<dDisKyungArrSize; n++)
		dDisArr.Add(dDisKyungArr[n]);
}
//지간이 2개 이상일 경우, 시점부
void CVBracingPositionDlg::GetDisArrCrossMultiJiganFirst(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
	dDisArr.RemoveAll();
	CDDWordArray dDisKyungArr;
	double staDiff = 0;
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	double staSttGir = pGir->GetStationOnJijum(nJigan);		//거더 슈 시점 스테이션
	double staEndGir = pGir->GetStationOnJijum(nJigan+1);	//거더 슈 종점 스테이션
	double sta = staEndGir;//종방향 교랑 중앙 스테이션

	long nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	long n = 0;
	for(n = 0; n<nKyungGridSu; n++)//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		staDiff = sta - pGir->GetLine()->GetStationByDisLength(sta, -dLengthBase, m_dDisCenterOnDiap);
		if(n == nKyungGridSu-1 && sta >= staSttGir){
			staDiff = sta-staSttGir;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}else if(sta-staDiff >= staSttGir){
			sta -= staDiff;
		}else if(bFirst && sta-staDiff < staSttGir){
			staDiff = sta - staSttGir;
			sta -= staDiff;
			bFirst = FALSE;
		}else{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=dDisKyungArrSize-1; n>=0; n--)		//0번 지간일 경우
		dDisArr.Add(dDisKyungArr[n]);
}
//지간이 2개 이상일 경우, 종점부
void CVBracingPositionDlg::GetDisArrCrossMultiJiganLast(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateGirderApp* pGir = m_pStd->GetBridge()->GetGirder(nG);
	dDisArr.RemoveAll();
	CDDWordArray dDisKyungArr;
	double staDiff = 0;
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	double staSttGir = pGir->GetStationOnJijum(nJigan);			//거더 시점 스테이션
	double staEndGir = pGir->GetStationOnJijum(nJigan+1);	//거더 종점 스테이션
	double sta = staSttGir;//종방향 교랑 중앙 스테이션

	long nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	long n = 0;
	for(n = 0; n<nKyungGridSu; n++)
	{
		staDiff = pGir->GetLine()->GetStationByDisLength(sta, dLengthBase, m_dDisCenterOnDiap) - sta;
		if(n == nKyungGridSu-1 && sta <= staEndGir){
			staDiff = staEndGir - sta;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}else if(sta+staDiff <= staEndGir){
			sta += staDiff;
		}else if(bFirst && sta+staDiff > staEndGir){
			staDiff = staEndGir-sta;
			sta += staDiff;
			bFirst = FALSE;
		}else{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=0; n<dDisKyungArrSize; n++)
		dDisArr.Add(dDisKyungArr[n]);
}

//<summary>
//현재 거더의 다이아프램 배치 간격을 구함.
//사교, 확폭일 경우 각 교각 기준하여(교량 중심 기준 아님) 거더 별로 5m씩 이동됨.
//</summary>
//<param name="nG">거더 번호</param>
//<param name="&dDisArr">현재 거더의 다이아프램 배치 간격</param>
void CVBracingPositionDlg::GetDisArrDiap(long nG, CDDWordArray &dDisArr)
{
	dDisArr.RemoveAll();

	CDDWordArray dDisKyungArr;
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	long nQtyJigan = pBridge->m_nQtyJigan;
	for(long nJigan = 0; nJigan<nQtyJigan; nJigan++)
	{
//		if(nQtyJigan == 1)	GetDisArrJiganCenter(nG, nJigan, dDisKyungArr);	//단지간 일 경우
//		else
		{
			if(nJigan==0)					GetDisArrDiapMultiJiganFirst(nG, nJigan, dDisKyungArr);  //첫번째 지간
			else if(nJigan==nQtyJigan-1)	GetDisArrDiapMultiJiganLast(nG, nJigan, dDisKyungArr);	 //마지막 지간
			else							GetDisArrDiapJiganCenter(nG, nJigan, dDisKyungArr);		 //중앙부 지간
		}		
		long dDisKyungArrSize = dDisKyungArr.GetSize();
		
		for(long n=0; n<dDisKyungArrSize; n++)		//0번 지간일 경우
			dDisArr.Add(dDisKyungArr[n]);

		dDisKyungArr.RemoveAll();
	}

}

//단지간일 경우
//입력 : nG
//Bridge 의 중앙 스테이션(sttStaBri+endStaBri)/2에서 좌우로 구분하여 정렬
//
void CVBracingPositionDlg::GetDisArrJiganCenter(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateGirderApp *pGir = m_pStd->GetBridge()->GetGirder(nG);
	CLineInfo  *pLine = pGir->GetLine();
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	long   nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	long   nKyungGridSuHalf = (long)nKyungGridSu/2;
	double staDiff		= 0;
	double staSttGir	= GetStationGirderLineByJijum(nG, nJigan);//거더선형 스테이션,거더 시점 스테이션
	double staEndGir	= GetStationGirderLineByJijum(nG, nJigan+1);//거더선형 스테이션,거더 종점 스테이션
	double staCR		= (GetStationOnCRLineByJijum(nJigan)+GetStationOnCRLineByJijum(nJigan+1))/2;//기준선형 스테이션, 교랑 중앙 스테이션
	double sta			= pGir->GetStationMatchByCRLine(staCR, CDPoint(0, 1));	//기준선형 스테이션, 교랑 중앙 스테이션

	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	CDDWordArray dDisKyungArr;
	long n = 0;
	for(n = 0; n<nKyungGridSuHalf; n++)	//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		if(n==0 && nKyungGridSu%2==1)//가로보 구간이 홀수 개이고 중앙부 첫번째일 경우
		{
			staDiff = sta - pLine->GetStationByDisLength(sta, -dLengthBase/2, m_dDisCenterOnDiap);
			sta -= staDiff;
		}
		if(n == nKyungGridSuHalf-1 && sta > staSttGir)//마지막 그리드이고 현재 스테이션이 거더 스테이션보다 클 경우
		{
			staDiff = sta-staSttGir;		
		}
		else if(sta-staDiff > staSttGir)
		{
			staDiff = sta - pLine->GetStationByDisLength(sta, -dLengthBase, m_dDisCenterOnDiap);
			sta -= staDiff;
		}
		else if(bFirst && sta-staDiff < staSttGir)
		{
			staDiff = sta - staSttGir;
			sta -= staDiff;
			bFirst = FALSE;
		}
		else
		{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=dDisKyungArrSize-1; n>=0; n--)		//0번 지간일 경우
		dDisArr.Add(dDisKyungArr[n]);

	dDisKyungArr.RemoveAll();

	sta = pGir->GetStationMatchByCRLine(staCR, CDPoint(0, 1));//종방향 교랑 중앙 스테이션
	bFirst = TRUE;
	for(n = 0; n<nKyungGridSuHalf; n++)//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		if(n==0 && nKyungGridSu%2==1)
		{
			staDiff = pLine->GetStationByDisLength(sta, dLengthBase/2, m_dDisCenterOnDiap) - sta;
			sta += staDiff;
			dDisArr.Add(dLengthBase);//중앙부에 가로보 없음.
		}

		if(n == nKyungGridSuHalf-1 && sta < staEndGir)
		{
			staDiff = staEndGir - sta;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}
		else if(sta+staDiff < staEndGir)
		{
			staDiff = pLine->GetStationByDisLength(sta, dLengthBase, m_dDisCenterOnDiap) - sta;
			sta += staDiff;
		}
		else if(bFirst && sta+staDiff > staEndGir)
		{
			staDiff = staEndGir-sta;
			sta += staDiff;
			bFirst = FALSE;
		}
		else
		{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=0; n<dDisKyungArrSize; n++)
		dDisArr.Add(dDisKyungArr[n]);
}

//<summary>
//가로보 기본간격을 재 설정함
//	확폭이 아닐 경우	: 재설정하지 않음.
//	확폭일 경구			: 거더 지간 길이와 기준선형 지간 길이와 비례식으로 계산
//</summary>
//<param name="nG"></param>
//<param name="nJigan"></param>
//<return></return>
double CVBracingPositionDlg::GetLengthBaseReal(long nG, long nJigan)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	if(nJigan > pBridge->m_nQtyJigan) return -1;	
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);

	if(!pGir->IsExtLineGirder())  return m_dBaseDist;//확폭이 아니면
	
//	double sttStaBri = GetStationOnCRLineByJijum(nJigan);
//	double endStaBri = GetStationOnCRLineByJijum(nJigan+1);
	double sttStaBri = GetStationGirderLineByJijum(-1, nJigan);
	double endStaBri = GetStationGirderLineByJijum(-1, nJigan+1);
	double staSttGir = GetStationGirderLineByJijum(nG, nJigan);//거더 시점 스테이션
	double staEndGir = GetStationGirderLineByJijum(nG, nJigan+1);//거더 종점 스테이션

	double dLengthBase = Round(m_dBaseDist*(staEndGir-staSttGir)/(endStaBri-sttStaBri), 0);
	return dLengthBase;
}


//<summary>
//nG 거더 선형의 스테이션을 리턴
//  시종점 경우 - 자기 거더 시.종점 슈 스테이션(기준선형, 본선이 아닐수도 있음)
//  중앙 지점일 경우 - 가장 좌측 거더와 우측거더 중앙의 좌표에서 기본선형에 수선을 내린 기본 선형의 스테이션에서
//					   거더 중앙과 만나는 nG 거더 선형의 스테이션
//</summary>
//<param name="nG"></param>
//<param name="nJijum"></param>
//<return></return>
double CVBracingPositionDlg::GetStationGirderLineByJijum(long nG, long nJijum)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	if(nJijum > pBridge->m_nQtyJigan) return -1;
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);

	if(nJijum == 0 || nJijum == pBridge->m_nQtyJigan || m_cCross90 != 0)
		return pGir->GetStationOnJijum(nJijum);	//시. 종점 슈 일 경우 

	double staCR = GetStationOnCRLineByJijum(nJijum);//도로 중심 스테이션
	CDPoint vAngCR(0, 1);
	double staReturn = pGir->GetStationMatchByCRLine(staCR, vAngCR);

	return staReturn;
}

//지간이 2개 이상일 경우, 시점부
void CVBracingPositionDlg::GetDisArrDiapMultiJiganFirst(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);
	dDisArr.RemoveAll();
	CDDWordArray dDisKyungArr;
	double staDiff = 0;
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	double staSttGir = pGir->GetStationOnJijum(nJigan);		//거더 슈 시점 스테이션
	double staEndGir = pGir->GetStationOnJijum(nJigan+1);	//거더 슈 종점 스테이션
	double sta = staEndGir;//종방향 교랑 중앙 스테이션

	long nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	long n = 0;
	for(n = 0; n<nKyungGridSu; n++)//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		staDiff = sta - pGir->GetLine()->GetStationByDisLength(sta, -dLengthBase, 0);
		if(n == nKyungGridSu-1 && sta >= staSttGir){
			staDiff = sta-staSttGir;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}else if(sta-staDiff >= staSttGir){
			sta -= staDiff;
		}else if(bFirst && sta-staDiff < staSttGir){
			staDiff = sta - staSttGir;
			sta -= staDiff;
			bFirst = FALSE;
		}else{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=dDisKyungArrSize-1; n>=0; n--)		//0번 지간일 경우
		dDisArr.Add(dDisKyungArr[n]);
}
//지간이 2개 이상일 경우, 종점부
void CVBracingPositionDlg::GetDisArrDiapMultiJiganLast(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(nG);
	dDisArr.RemoveAll();
	CDDWordArray dDisKyungArr;
	double staDiff = 0;
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	double staSttGir = pGir->GetStationOnJijum(nJigan);			//거더 시점 스테이션
	double staEndGir = pGir->GetStationOnJijum(nJigan+1);	//거더 종점 스테이션
	double sta = staSttGir;//종방향 교랑 중앙 스테이션

	long nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	long n = 0;
	for(n = 0; n<nKyungGridSu; n++)
	{
		staDiff = pGir->GetLine()->GetStationByDisLength(sta, dLengthBase, 0) - sta;
		if(n == nKyungGridSu-1 && sta <= staEndGir){
			staDiff = staEndGir - sta;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}else if(sta+staDiff <= staEndGir){
			sta += staDiff;
		}else if(bFirst && sta+staDiff > staEndGir){
			staDiff = staEndGir-sta;
			sta += staDiff;
			bFirst = FALSE;
		}else{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=0; n<dDisKyungArrSize; n++)
		dDisArr.Add(dDisKyungArr[n]);
}

//단지간, 다지간의 중앙부 경우
//입력 : nG
//Bridge 의 중앙 스테이션(sttStaBri+endStaBri)/2에서 좌우로 구분하여 정렬
//
void CVBracingPositionDlg::GetDisArrDiapJiganCenter(long nG, long nJigan, CDDWordArray &dDisArr)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pBridge->GetGirder(nG);
	CLineInfo  *pLine = pGir->GetLine();
	double dLengthBase = GetLengthBaseReal(nG, nJigan);
	long   nKyungGridSu = GetCrossGridSuInOneJigan(nJigan);//개소수는 오직 지간 그리드수로만 점검
	long   nKyungGridSuHalf = (long)nKyungGridSu/2;
	double staDiff = 0;
	double staSttGir = pGir->GetStationOnJijum(nJigan);//거더선형 스테이션,거더 시점 스테이션
	double staEndGir = pGir->GetStationOnJijum(nJigan+1);//거더선형 스테이션,거더 종점 스테이션
	double staCR = (GetStationOnCRLineByJijum(nJigan)+GetStationOnCRLineByJijum(nJigan+1))/2;//기준선형 스테이션, 교랑 중앙 스테이션
	double sta = (staSttGir + staEndGir)/2;	//기준선형 스테이션, 교랑 중앙 스테이션

	BOOL bFirst = TRUE;	//staSttGir을 넘어서는 첫번째 그리드(시.종점부)
	CDDWordArray dDisKyungArr;
	long n = 0;
	for(n = 0; n<nKyungGridSuHalf; n++)	//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		if(n==0 && nKyungGridSu%2==1)//가로보 구간이 홀수 개이고 중앙부 첫번째일 경우
		{
			staDiff = sta - pLine->GetStationByDisLength(sta, -dLengthBase/2, 0);
			sta -= staDiff;
		}
		if(n == nKyungGridSuHalf-1 && sta > staSttGir)//마지막 그리드이고 현재 스테이션이 거더 스테이션보다 클 경우
		{
			staDiff = sta-staSttGir;		
		}
		else if(sta-staDiff > staSttGir)
		{
			staDiff = sta - pLine->GetStationByDisLength(sta, -dLengthBase, 0);
			sta -= staDiff;
		}
		else if(bFirst && sta-staDiff < staSttGir)
		{
			staDiff = sta - staSttGir;
			sta -= staDiff;
			bFirst = FALSE;
		}
		else
		{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	long dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=dDisKyungArrSize-1; n>=0; n--)		//0번 지간일 경우
		dDisArr.Add(dDisKyungArr[n]);

	dDisKyungArr.RemoveAll();

	sta = (staSttGir + staEndGir)/2;//종방향 교랑 중앙 스테이션
	bFirst = TRUE;
	for(n = 0; n<nKyungGridSuHalf; n++)//교량 중앙 스테이션에서 시점 거더 방향으로 배치
	{
		if(n==0 && nKyungGridSu%2==1)
		{
			staDiff = pLine->GetStationByDisLength(sta, dLengthBase/2, 0) - sta;
			sta += staDiff;
			dDisArr.Add(dLengthBase);//중앙부에 가로보 없음.
		}

		if(n == nKyungGridSuHalf-1 && sta < staEndGir)
		{
			staDiff = staEndGir - sta;		//마지막 그리드이고 staDiff가 0이 아닐 경우
		}
		else if(sta+staDiff < staEndGir)
		{
			staDiff = pLine->GetStationByDisLength(sta, dLengthBase, 0) - sta;
			sta += staDiff;
		}
		else if(bFirst && sta+staDiff > staEndGir)
		{
			staDiff = staEndGir-sta;
			sta += staDiff;
			bFirst = FALSE;
		}
		else
		{
			staDiff = 0;
		}

		dDisKyungArr.Add(staDiff);
	}
	dDisKyungArrSize = dDisKyungArr.GetSize();
	for(n=0; n<dDisKyungArrSize; n++)
		dDisArr.Add(dDisKyungArr[n]);
}

void CVBracingPositionDlg::SetBxJackupHeight()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx;

		pBx = Finder.GetBxFirst(BX_JACKUP);
		while( pBx )
		{
			CJackupStiff* pJack = pBx->GetJackupStiff();
			pJack->m_vJewon[0].z = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			pBx = Finder.GetBxNext();
		}
		pBx = Finder.GetBxFirst(BX_JACKUP);
		while( pBx )
		{
			CJackupStiff* pJack = pBx->GetJackupStiff();
			pJack->m_vJewon[0].z = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			pBx = Finder.GetBxNext();
		}
		pBx = Finder.GetBxFirst(BX_JACKUP);
		while( pBx )
		{
			CJackupStiff* pJack = pBx->GetJackupStiff();
			pJack->m_vJewon[0].z = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			pBx = Finder.GetBxNext();
		}	
	}
}

void CVBracingPositionDlg::SetBxJijumVStiffHeight()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx;

		pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_JIJUMSTIFF);
		while( pBx )
		{
			CVStiff* pVStiff = pBx->GetJijumStiff();
			pVStiff->m_dH[LEFT][UP] = pVStiff->m_dH[LEFT][DOWN] = 0;
			pVStiff->m_dH[RIGHT][UP] = pVStiff->m_dH[RIGHT][DOWN] = 0;
			pBx = (CPlateBasicIndex*)Finder.GetBxNext();
		}	
	}
}

void CVBracingPositionDlg::SetBxByun()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	CDPoint vAng = CDPoint(0,1);
//	if(pBridge->m_cByunDan==FALSE)	return;	// 변단면인 경우만 ==> 누구짓이야~~

	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		CPtrList NList;
		CPlateBasicIndex * p;
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);

		for(long nK = 0; nK < pBridge->m_nQtyJigan; nK++)
		{
			double dStaJijum = pGir->GetStationOnJijum(nK);
			double dStaJijum2 = pGir->GetStationOnJijum(nK+1);	//지간은 지점보다 1개 작음.
			if(pGir->IsByunDanMyunInJigan(nK) == FALSE)	continue;

			if(pBridge->m_dLenLeftByun[nK] > 0 && pBridge->m_nStyleByun[nK] != 2)
			{
				p = pGir->newBx();
				p->SetState(BX_STT_BYUN_LOWER,TRUE);
				p->SetStation(dStaJijum + pBridge->m_dLenLeftByun[nK]);
				p->SetAngle(vAng);
				NList.AddTail( p );
			}
			if(pBridge->m_dLenLeftCurve[nK] > 0)	
			{
				p = pGir->newBx();
				p->SetState(BX_STT_BYUN_UPPER,TRUE);
				p->SetStation(dStaJijum + pBridge->m_dLenLeftByun[nK] + pBridge->m_dLenLeftCurve[nK]);
				p->SetAngle(vAng);
				NList.AddTail( p );
			}
			if(pBridge->m_dLenRightCurve[nK] > 0)	
			{
				p = pGir->newBx();
				p->SetState(BX_END_BYUN_UPPER,TRUE);
				p->SetStation(dStaJijum2 - pBridge->m_dLenRightCurve[nK] - pBridge->m_dLenRightByun[nK]);
				p->SetAngle(vAng);
				NList.AddTail( p );
			}
			if(pBridge->m_dLenRightByun[nK] > 0 && pBridge->m_nStyleByun[nK] != 3)
			{
				p = pGir->newBx();
				p->SetState(BX_END_BYUN_LOWER,TRUE);
				p->SetStation(dStaJijum2 - pBridge->m_dLenRightByun[nK]);
				p->SetAngle(vAng);
				NList.AddTail( p );
			}
		}
		// set BxList
		CDWordArray LKExceptionArr;
		LKExceptionArr.Add(CPlateBxObject::LK_CROSSBEAM);
		pGir->PressBxList(NList,BX_BYUN, &LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);
	}
}


CString CVBracingPositionDlg::GetLineInfo()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();		
	CPlateGirderApp *pGir    = pBridge->GetGirder(-1);	//도로 중심 거더
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_LINE_P);
	CLineInfo	     *pLine  = pGir->GetLine();

	CString sLineInfo, sRadius, sLeft, sRadius2, sLeft2, sLineType;

	double staStt = pBxStt->GetStation();		//교량시점 station.
	double staEnd = pBxEnd->GetStation();		//교량종점 station.

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
					sLineType="직선 + 클로소이드";
				else
					sLineType="클로소이드";
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
						sLineType="직선 + 클로소이드 + 원곡선 + 클로소이드 + 직선";
					else if(sLeft2=="A")
						sLineType="직선 + 클로소이드 + 원곡선 + 클로소이드";
					else if(sLeft2=="R")
						sLineType="직선 + 클로소이드 + 원곡선";
				}
				else
				{
					if(sLeft2=="")
						sLineType="클로소이드 + 원곡선 + 클로소이드 + 직선";
					else if(sLeft2=="A")
						sLineType="클로소이드 + 원곡선 + 클로소이드";
					else if(sLeft2=="R")
						sLineType="클로소이드 + 원곡선";
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
						sLineType="클로소이드 + 원곡선";
					else
						sLineType="직선 + 원곡선";
				}
				else
						sLineType="원곡선";
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
								sLineType="클로소이드 + 원곡선 + 클로소이드";
							else
								sLineType="클로소이드 + 원곡선";
						}
						else if(sLeft=="A" && sLeft2=="")		//Stt:Clothoid    End:Line
						{
							pBx = Finder.GetBxNext();

							if(pBx != NULL)	//ETC가 있으면
							{
								sta = pBx->GetStation();	//ETC station
								if(staEnd > sta)
									sLineType="클로소이드 + 원곡선 + 클로소이드 + 직선";
								else
									sLineType="클로소이드 + 원곡선 + 클로소이드";
							}
							else
							{
								if(staEnd > sta)
									sLineType = "클로소이드 + 원곡선 + 직선";
								else
									sLineType = "클로소이드 + 원곡선";
							}
						}
						else if(sLeft==""&&sLeft2=="A")		//Stt:Line    End:Clothoid
						{
							if(staEnd>sta)
								sLineType="직선 + 원곡선 + 클로소이드";
							else
								sLineType="직선 + 원곡선";
						}
						else		//Stt:Line    End:Line
							sLineType="직선 + 원곡선 + 직선";
					}
					else
					{
						if(sLeft=="A")
							sLineType="클로소이드 + 원곡선";
						else
							sLineType="직선 + 원곡선";
					}
				}
				else
				{
					if(staEnd>sta)
					{
						if(sLeft=="A"&&sLeft2=="A")
						{
							if(staEnd>sta)
								sLineType="원곡선 + 클로소이드";
							else
								sLineType="원곡선";
						}
						else if(sLeft=="A"&&sLeft2=="")
						{
							pBx = Finder.GetBxNext();
							sta = pBx->GetStation();	//ETC station

							if(pBx!=NULL)	//ETC가 있으면
							{
								if(staEnd>sta)
									sLineType="원곡선 + 클로소이드 + 직선";
								else
									sLineType="원곡선 + 클로소이드";
							}
							else
							{
								if(staEnd>sta)
									sLineType="원곡선 + 직선";
								else
									sLineType="원곡선";
							}
						}
						else if(sLeft==""&&sLeft2=="A")
						{
							if(staEnd>sta)
								sLineType="원곡선 + 클로소이드";
							else
								sLineType="원곡선";
						}
						else
							sLineType="원곡선 + 직선";
					}
					else
						sLineType="원곡선";
				}
			}
		}
		else if(sLineInfo=="EC")
		{
			pBx = Finder.GetBxNext();	//ETC

			if(sta>staStt)
			{
				if(pBx!=NULL)	//ETC가 있으면
				{
					sta = pBx->GetStation();	//ETC station

					if(staEnd>sta)
						sLineType="원곡선 + 클로소이드 + 직선";
					else
						sLineType="원곡선 + 클로소이드";
				}
				else	//ETC가 없으면
				{
					if(staEnd>sta)
						sLineType="원곡선 + 직선";
					else
						sLineType="원곡선";
				}
			}
			else
			{
				if(pBx!=NULL)	//ETC가 있으면
				{
					sta = pBx->GetStation();	//ETC station

					if(staEnd>sta)
						sLineType="클로소이드 + 직선";
					else
						sLineType="클로소이드";
				}
				else	//ETC가 없으면
					sLineType="직선";
			}
		}
		else if(sLineInfo=="ETC")
		{
			if(sta>staStt)
			{
				if(staEnd>sta)
					sLineType="클로소이드 + 직선";
				else
					sLineType="클로소이드";
			}
			else
				sLineType="직선";
		}
	}
	else
	{
		double staStt  = pBxStt->GetStation();

		sRadius = pGir->GetRadiusString(staStt,0);
		sLeft	= sRadius.Left(1);

		if(sRadius=="")
			sLineType = "직선";
		else
		{
			if(sLeft=="R")
				sLineType = "원곡선";
			else if(sLeft=="A")
				sLineType = "클로소이드";
		}
	}

	return sLineType;

}


void CVBracingPositionDlg::OnButtonGirderSameApply() 
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nCurRow = m_Grid.GetFocusCell().row;
	if(nCurRow<=0) nCurRow = 1;
	nCurRow -= (nCurRow % 2) ? 0 : 1;

	CString str;
	str.Format("  %d번 거더 기준으로 가로보를  \n  직각으로 설정합니다.  계속하시겠습니까?", nCurRow/2);
	if (IDCANCEL == MessageBox(str, "모든 거더 동일적용", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	long nRows = m_Grid.GetRows();
	long nCols = m_Grid.GetCols();
	long nEndCol = m_Grid.GetValidCol(nCurRow, FALSE);
	for (long nRow = 1; nRow < nRows; nRow += 2)
	{
		if (nRow == nCurRow)
			continue;
		CPlateGirderApp *pGir = pDB->GetGirder(nRow/2-1);
		for (long nCol = 1; nCol < nCols; nCol++)
		{
			double staBase = GetStationMatrix(nCurRow, nCol);//현재 기준 거더의 스테이션
			double staEndShoe = pGir->GetSSS(OFF_END_SHOE);//현재 기준 거더의 스테이션
			if (nCol == nCols-1 || staBase > staEndShoe)
				staBase = staEndShoe;

			SetMatrixByStation(nRow, nCol, staBase);
			m_Grid.SetTextMatrix(nRow+1, nCol, m_Grid.GetTextMatrixDouble(nRow, nCol) == 0 ? "-" : "90");
		}
	}
	m_Grid.DeleteUnusedColumn();
	SetDataSaveGrid();
	m_Grid.SetRedraw(TRUE,TRUE);
	
	DrawInputDomyunView(FALSE);	
}

void CVBracingPositionDlg::OnCrossGirderlen() 
{
	long nOldType = m_nLengthType;
	double dDisCenterOld = m_dDisCenterOnDiap;
	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dDisCenterOld);
	SetDataSaveGrid();
	DrawInputDomyunView(FALSE);

}

void CVBracingPositionDlg::OnCrossBridgelen() 
{
	long nLengthTypeOld = m_nLengthType;
	double dDisCenterOld = m_dDisCenterOnDiap;
	UpdateData();
	SetGridDataByChangeLengthType(nLengthTypeOld, dDisCenterOld);
//	SetDataSaveGrid();
	SetDataSave();
	DrawInputDomyunView(FALSE);

}

//<summary>
//m_nLengthType 과 m_dDisCenterOnDiap에 따라서 SetMatrix함.
//</summary>
//<param name="nLengthType">0 = 거더선형 기준, 1=거더 중앙 기준</param>
//<param name="dDisCenter">거더선형 기준 일 경우에만 적용됨</param>
//SetMatrixByLengthTypeDisCenter()
void CVBracingPositionDlg::SetGridDataByChangeLengthType(int nLengthType, double dDisCenter)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	long nRows = m_Grid.GetRows();
	long nCols = m_Grid.GetCols();

	for (long nRow = 1; nRow < nRows; nRow += 2) 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nRow/2 - 1);
		double staStt = pGir->GetSSS(OFF_STT_SHOE);
		double sta, lenSum = 0;
		for (long nCol = 1; nCol < nCols; nCol++)   
		{
			lenSum += m_Grid.GetTextMatrixDouble(nRow, nCol);
			if(nCol == nCols-1) 
			{
				sta = pGir->GetSSS(OFF_END_SHOE);
			}
			else
			{
				if (nLengthType == 0)
					sta = pGir->GetStationByActLength(staStt, lenSum);
				else
					sta = pGir->GetLine()->GetStationByDisLength(staStt, lenSum, dDisCenter);
			}
			SetMatrixByStation(nRow, nCol, sta);
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}
