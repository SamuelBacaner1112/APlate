// VStiffPositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "VStiffPositionDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVStiffPositionDlg dialog

CVStiffPositionDlg::CVStiffPositionDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CVStiffPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVStiffPositionDlg)
	m_dBaseDist = 0.0;
	m_dDisCenterOnVst = 0.0;
	m_bSameApplySameCol = TRUE;
	m_nLengthType = 1;
	m_bInputAngleVStiff = FALSE;
	m_bCheckNotInstall = FALSE;
	//}}AFX_DATA_INIT
	m_bRowColChange = TRUE;
}
 
CVStiffPositionDlg::~CVStiffPositionDlg()
{
	AhPtrArrayDelete(m_bSellArr, (CPtrArray*)0 );
	
}
void CVStiffPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVStiffPositionDlg)
// 	DDX_Control(pDX, IDC_VSTIFF_BRIDGELEN, m_Radio1);
// 	DDX_Control(pDX, IDC_VSTIFF_GIRDERLEN, m_Radio2);
	DDX_Control(pDX, IDC_VSTIFF_DISTANCE_CENTER, m_Edit1);
	DDX_Control(pDX, IDC_EDIT_BASEDIST, m_Edit2);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_BASEDIST, m_dBaseDist);
	DDX_Text(pDX, IDC_VSTIFF_DISTANCE_CENTER, m_dDisCenterOnVst);
	DDX_Check(pDX, IDC_CHECK_SAMETYPE, m_bSameApplySameCol);
	DDX_Radio(pDX, IDC_VSTIFF_GIRDERLEN, m_nLengthType);
	DDX_Check(pDX, IDC_VSTIFF_INPUTANGLE, m_bInputAngleVStiff);
	DDX_Check(pDX, IDC_CHECK_NOTINSTALL, m_bCheckNotInstall);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_VSTIFF_ARRANGE_THISGIRDER, m_btnSameAll);
	DDX_Control(pDX, IDC_VSTIFF_AUTO_SYMMETRY, m_btnSymetry);
	DDX_Control(pDX, IDC_VSTIFF_SETTING1, m_btnSetting1);
	DDX_Control(pDX, IDC_VSTIFF_SETTING2, m_btnSetting2);
}

BEGIN_MESSAGE_MAP(CVStiffPositionDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CVStiffPositionDlg)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	ON_BN_CLICKED(IDC_CHECK_SAMETYPE, OnCheckSametype)
	ON_BN_CLICKED(IDC_VSTIFF_ARRANGE_THISGIRDER, OnVstiffArrangeThisgirder)
	ON_BN_CLICKED(IDC_VSTIFF_BRIDGELEN, OnVstiffBridgelen)
	ON_BN_CLICKED(IDC_VSTIFF_GIRDERLEN, OnVstiffGirderlen)
	ON_BN_CLICKED(IDC_VSTIFF_INPUTANGLE, OnVstiffInputangle)
	ON_BN_CLICKED(IDC_VSTIFF_SETTING1, OnVstiffSetting1)
	ON_BN_CLICKED(IDC_VSTIFF_SETTING2, OnVstiffSetting2)
	ON_BN_CLICKED(IDC_VSTIFF_AUTO_SYMMETRY, OnSetDataSymmetry)
	ON_BN_CLICKED(IDC_CHECK_NOTINSTALL, OnCheckNotinstall)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)	
	ON_NOTIFY(GVN_CELL_INSERT_COL, IDC_GRID, OnCellInsertCol)	
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)	
	ON_NOTIFY(GVN_CELL_DELETE_COL, IDC_GRID, OnCellDeleteCol)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVStiffPositionDlg message handlers

void CVStiffPositionDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		

	CDomyun Dom(pDom);
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	Draw.DrawPyung(&Dom, 7);
	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CVStiffPositionDlg::SetGridTitle()
{
	long cols = m_Grid.GetCols();
	long rows = m_Grid.GetRows();
	
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(TRUE);
	m_Grid.EnableTitleTips(FALSE);
	m_Grid.SetTextBkColor(RGB(225, 250, 250));
	
	m_Grid.SetColumnCount(cols);
	m_Grid.SetRowCount(rows);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,65);	

	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetFixedRowCount(1);


	CString str, str2;	
	for(long col = 1;col < cols;col++)
	{		
		str.Format("%d",col);
		m_Grid.SetTextMatrix(0,col,str);
	}
	
	m_Grid.SetTextMatrix(0,0,"�� ��");

	long	nG = 0;
	for(long r = 1; r < rows; r+=2)
	{
		if( r == 1 )
		{
			str.Format("����(CR)");
			str2.Format("����(CR)");
		}
		else
		{
			str.Format("����(G%d)",nG/*+pBridge->GetNumberStartGirder()*/);
			str2.Format("����(G%d)",nG/*+pBridge->GetNumberStartGirder()*/);
		}
		m_Grid.SetTextMatrix(r, 0, str);
		m_Grid.SetTextMatrix(r+1, 0, str2);
		if (!m_bInputAngleVStiff)
			m_Grid.SetRowHeight(r+1, 0);
		nG++;
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CVStiffPositionDlg::SetDataInit()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CGlobarOption   *pOpt    = m_pStd->GetDataManage()->GetGlobalOption();

	m_dBaseDist			= pBridge->m_dLengthBaseVStiff;
	m_dDisCenterOnVst	= pBridge->m_dDisCenterOnVst;
	m_nLengthType		= pBridge->m_nLengthTypeOnVst;
	m_bInputAngleVStiff	= pBridge->m_bInputAngleVStiff;
	m_bSameApplySameCol	= (BOOL)pBridge->m_cSameApplySameColVStiff;
	m_bCheckNotInstall  = pOpt->GetDesignVStiffInstall(); //0�̸� ��ġ�̹Ƿ� üũǥ�� ���ϰ� 1�̸� �̼�ġ �̹Ƿ� üũǥ�� �Ѵ�...
	EnableCheckInstall(!m_bCheckNotInstall);

	CString str;
	long nRows = pBridge->m_VStiffGrid.GetRows();
	long nNewRows = pBridge->GetGirdersu()*2 + 3;
	long nNewCols = 0;
	long nG = 0;
	for(nG = -1;nG < pBridge->GetGirdersu(); nG++)
	{
		long nQty = pBridge->GetGirder(nG)->GetQtyVStiffSectionTotal()+pBridge->GetGirder(nG)->GetQtyCrossSectionTotal();
		nNewCols = max(nQty, nNewCols);
	}
	m_Grid.SetRowCount(nNewRows);
	m_Grid.SetColumnCount(nNewCols);

	for(nG = -1;nG < pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		double staStt = pGir->GetSSS(OFF_STT_SHOE);
		double staEnd = pGir->GetSSS(OFF_END_SHOE);
		long nCol = 1;
		long nRow = nG*2+3;
		long nOldGsu = nRows/2-1;
		if (nG >= nOldGsu)  // �Ŵ����� �þ (������ �Ŵ� Data�� �۾�)
			pGir = pBridge->GetGirder(nOldGsu-1);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF);
		while (pBx)
		{
			double sta = pBx->GetStation();
			if (staStt < sta && sta <= staEnd)
			{
				SetMatrixByStation(nRow, nCol, sta);
				str = "-";
				if (pBx->IsState(BX_VSTIFF))
					str.Format("%.lf", pBx->GetAngle().GetAngleDegree());
				m_Grid.SetTextMatrix(nRow+1, nCol, str);
				nCol++;				
			}
			pBx = Finder.GetBxNext();
		}
	}	
	SetDataSaveGrid();
	DataInitGridData();
	UpdateData(FALSE);

	SetGridTitle();

	m_Grid.SetRedraw(TRUE,TRUE);	
}

void CVStiffPositionDlg::DataInitGridData()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	m_bRowColChange = FALSE;

	CPlateGirderApp *pGir;
	CString str;
	long nChange = 0;
	AhPtrArrayMake(pBridge->GetGirdersu()+1, m_bSellArr, (CPtrArray*)0);	// CR Girder����
	CPtrArray* pDA;

	for(long g = -1;g < pBridge->GetGirdersu();g++)
	{
		long nRow = g * 2 + 3;
		pDA = (CPtrArray*)m_bSellArr.GetAt(g+1);
		pDA->RemoveAll();
		pGir = pBridge->GetGirder(g);
		long nCol = 1;
		double term = 0, dis = 0, disCross;

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF);
		CPlateBasicIndex *pBxCross = pBx;
		while (pBx = Finder.GetBxNext())
		{
			pDA->Add(pBxCross);
			if (pBx->IsState(BX_CROSSBEAM_VBRACING))
			{
				disCross = pBx->GetStation() - pBxCross->GetStation();
				dis = disCross - term;
				str.Format("%.lf", dis);
				
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGBREADONLY);
				m_Grid.SetItemState(nRow + 1, nCol, m_Grid.GetItemState(nRow + 1, nCol) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow + 1, nCol, RGBREADONLY);
				
//				if (str != m_Grid.GetTextMatrix(nRow, nCol))
				if (fabs(dis - m_Grid.GetTextMatrixDouble(nRow, nCol)) > 1.0)
				{
					nChange++;
					m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);
					m_Grid.SetItemBkColour(nRow, nCol, RGB(225, 250, 250));					
				}

				term = 0.0;
				pBxCross = pBx;
			}
			else   {
				term += m_Grid.GetTextMatrixDouble(nRow, nCol);				
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGB(225, 250, 250));
				m_Grid.SetItemState(nRow + 1, nCol, m_Grid.GetItemState(nRow + 1, nCol) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow + 1, nCol, RGB(225, 250, 250));
			}
			nCol++;			
		}
	}

	m_bRowColChange = TRUE;
}


void CVStiffPositionDlg::SetDataSaveGrid()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long rows = m_Grid.GetRows();
	long cols = m_Grid.GetCols();
	pBridge->m_VStiffGrid.Resize(rows, cols);

	for (long r = 0; r < rows; r++)
		for (long c = 0; c < cols; c++)
			pBridge->m_VStiffGrid.SetTextMatrix(r, c, m_Grid.GetItemText(r, c));

	pBridge->m_dDisCenterOnVst	= m_dDisCenterOnVst;
	pBridge->m_nLengthTypeOnVst	= m_nLengthType;
}


void CVStiffPositionDlg::SetDataDefault()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();		
	CGlobarOption   *pOpt    = m_pStd->GetDataManage()->GetGlobalOption();

	if(pOpt->GetDesignVStiffInstall()==0)	// ��ġ
	{
		if (pBridge->IsPlateGir())
			pBridge->m_dLengthBaseVStiff = 1250.0;
		else
			pBridge->m_dLengthBaseVStiff = 2500.0;
	}
	else//��ġ���� ����...
	{
		pBridge->m_dLengthBaseVStiff = 0;
	}

	m_dBaseDist = pBridge->m_dLengthBaseVStiff;
	m_dDisCenterOnVst = pBridge->m_dDisCenterOnVst;
	m_nLengthType     = pBridge->m_nLengthTypeOnVst;
	UpdateData(FALSE);

	SetGridByBaseLen(0);
	SetSellAttrUnitCross();	// ���̾����� ������ �� ������ ������ �迭�ʱ�ȭ
	SetGridAttrUnit();	

	SetDataSaveGrid();	
	
	long cols = GetMaxVStiffsuForGridsu();
	long rows = pBridge->GetGirdersu()*2 + 3;
	m_Grid.SetRowCount(rows);
	m_Grid.SetColumnCount(cols);

	SetDataBx();
	SetDataSave();
}

void CVStiffPositionDlg::SetDataBx()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	long nCols = m_Grid.GetColumnCount();

	CDPoint vAng90(0,1);
	CDPoint vAng;
	long g = 0;
	for(g=-1; g < pBridge->GetGirdersu(); g++)
	{
		CPlateGirderApp* pGir = pBridge->GetGirder(g);

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBxS = Finder.GetBxFirst(BX_VSTIFF);
		while(pBxS)
		{
			pBxS->SetAngle(CDPoint(0,1));
			pBxS = Finder.GetBxNext();
		}

		CPlateBasicIndex *pBxCross = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		pBxCross = Finder.GetBxNext();	// �ι�° ���̾�����
		CPtrList NList;
		
		double sta = Round(pGir->GetSSS(OFF_STT_SHOE), 0);		
		long nCount = 0;
		for(long nCol = 1; nCol < nCols; nCol++)	//Index xx1~xx9���� ����
		{
			double dis = pBridge->m_VStiffGrid.GetTextMatrixDouble(g*2+3, nCol);
			if(dis <= 0)	continue;

			sta = GetStationMatrix(g*2+3, nCol);
			
			if(pBxCross && sta + 5.0 < pBxCross->GetStation())
			{
				//BX_VSTIFF
				CPlateBasicIndex* p = pGir->newBx();
				double angDeg = pBridge->m_VStiffGrid.GetTextMatrixDouble(g*2+4, nCol);
				CDPoint vAng = (angDeg == 90) ? 
					vAng90 : ToDPointFrRadian(ToRadian(angDeg));
				NList.AddTail(p);
				p->SetState(BX_VSTIFF,TRUE);
				CPlateBasicIndex *pBxBending = pGir->GetBxByStation(sta, BX_BENDING);
				if(pBxBending && pBxBending->GetStation() == sta)
					sta -= 0.000001;

				CPlateBasicIndex *pBxByun = pGir->GetBxByStation(sta, BX_BYUN);
				if(pBxByun && pBxByun->GetStation() == sta)
					sta -= 0.000001;

				CPlateBasicIndex *pBxJackup = pGir->GetBxByStation(sta, BX_JACKUP);
				if(pBxJackup && pBxJackup->GetStation() == sta)
					sta -= 0.000001;

				p->SetStation(sta);					// ¦�� �ε���
				p->SetMatchCol(nCol-1);
				p->SetAngle(vAng);
				++nCount;
			
			}
			else		//���̾������̸�
			{
				pBxCross = Finder.GetBxNext();	// �ι�° ���̾�����
			}
		}

		pGir->MakeVStiffSection(nCount);	

		CDWordArray LKExceptionArr;
		LKExceptionArr.Add(CPlateBxObject::LK_VSTIFF);	
		pGir->PressBxList(NList, BX_VSTIFF,&LKExceptionArr);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);		
				
		pGir->SetLKPOS(CPlateBxObject::LK_VSTIFF);		
	}
	
	
	for(g=-1; g < pBridge->GetGirdersu(); g++)
	{
		CPlateGirderApp* pGir = pBridge->GetGirder(g);

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBxS = Finder.GetBxFirst(BX_VSTIFF);
		while(pBxS)
		{
			pBxS->GetSection()->SetenType(SECTION_V);	
			pBxS->GetSection()->SetenDetType(SECDET_VSITFF);

			pBxS = Finder.GetBxNext();
		}
	}
}



void CVStiffPositionDlg::SetDataSave()
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	CGlobarOption   *pOpt    = m_pStd->GetDataManage()->GetGlobalOption();

	UpdateData();
	pBridge->m_dLengthBaseVStiff = m_dBaseDist;
	if(pBridge->m_dLengthBaseVStiff==0)
		pOpt->SetDesignVStiffInstall(1);//��ġ�ϴ� ����...
	pBridge->m_dDisCenterOnVst  = m_dDisCenterOnVst;
	pBridge->m_nLengthTypeOnVst = m_nLengthType;
	pBridge->m_bInputAngleVStiff= m_bInputAngleVStiff;
	pBridge->m_cSameApplySameColVStiff = (BYTE)m_bSameApplySameCol;
	
	SetDataBx();
}

long CVStiffPositionDlg::GetMaxVStiffsuForGridsu() const
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	long nResult = 0;

	for(long g = -1; g < pBridge->GetGirdersu(); g++)
	{
		CPlateGirderApp* pGir = pBridge->GetGirder(g);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		long nCurSum = 0;
		while (pBx = Finder.GetBxNext())
		{
			double dis = pGir->GetLine()->GetLengthByDis(Finder.GetBxFixPrev()->GetStation(),
														pBx->GetStation(), m_dDisCenterOnVst);
			long nQty = (long)RoundUp(dis / m_dBaseDist, 0);
			if (nQty == 0)	nQty++;
			nCurSum += nQty;
		}
		if (nResult < nCurSum)
			nResult = nCurSum;
	}

	return nResult+1;	//�׸��� �÷� ���񶧹��� +1�� ��.
}

// m_dLengthBaseVStiff�� ���� �������� m_Grid�� �� ������
// nSetType = 0 : �⺻���� ����
//            1 : ����� ����
void CVStiffPositionDlg::SetGridByBaseLen(long nSetType)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	CPlateGirderApp *pGir;
	long cols = GetMaxVStiffsuForGridsu();
	long rows = pBridge->GetGirdersu()*2 + 3;
	m_Grid.SetRowCount(rows);
	m_Grid.SetColumnCount(cols);

	CString str;
	double angDeg = 90;
	for(long g = -1;g < pBridge->GetGirdersu();g++)
	{
		pGir = pBridge->GetGirder(g);
		long nCol = 0;
		long nRow = g*2+3;
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while (Finder.GetBxFixNext())
		{
			double curSta = pBx->GetStation();
			double nextSta = Finder.GetBxFixNext()->GetStation();
			double dis = pGir->GetLine()->GetLengthByDis(curSta, nextSta, m_dDisCenterOnVst);
			long nQty = (long) RoundUp(dis / m_dBaseDist, 0);
			if (nQty == 0)	nQty++;
			for(long nVStiffsu = 0; nVStiffsu < nQty; nVStiffsu++)
			{
				angDeg = 90;
				double lenBase = m_dBaseDist;
				if (nSetType == 1)
					lenBase = dis / nQty;

				if (pBx == Finder.GetBxFixHead() && nVStiffsu == 0)		//�������� ���
					curSta = pGir->GetStationByActLength(nextSta, -(nQty-1)*lenBase, m_dDisCenterOnVst - pGir->m_dGirderCenter);
				else if (nVStiffsu == nQty-1)   //���̾����� ������ �������� ���
					curSta = nextSta;
				else
					curSta = pGir->GetStationByActLength(curSta, lenBase, m_dDisCenterOnVst - pGir->m_dGirderCenter);

				if (nVStiffsu == nQty-2 && (nextSta-curSta)<1)
				{
					curSta = nextSta;
					nVStiffsu++;					
				}

				if (nVStiffsu == nQty-1)
					angDeg = 0;

				SetMatrixByStation(nRow, nCol+1, curSta);
				if (angDeg == 0)
					str.Format("-");
				else
					str.Format("%.lf", angDeg);
				m_Grid.SetTextMatrix(nRow+1, nCol+1, str);

				nCol++;
			}
			pBx = Finder.GetBxNext();
		}
		// �׸��� ���� ���� Data ����.
		for ( ; nCol < cols-1; nCol++)   {
			m_Grid.SetTextMatrix(nRow, nCol+1, "");
			m_Grid.SetTextMatrix(nRow+1, nCol+1, "");
		}
	}
	SetDataSaveGrid();
	m_Grid.Invalidate();
}

void CVStiffPositionDlg::SetMatrixByStation(long nRow, long nCol, double sta)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	if (nCol >= m_Grid.GetColumnCount())
		m_Grid.SetColumnCount(nCol + 1);
	double lenSum = 0;
	for(long c = 1; c < nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);
	if(nCol == m_Grid.GetColumnCount()-1)
		sta = pBridge->GetGirder(nRow/2-1)->GetSSS(OFF_END_SHOE);
	double len = pBridge->GetLengthByStationInGrid(TRUE, nRow/2-1, sta, m_nLengthType, m_dDisCenterOnVst) - lenSum;	
	m_Grid.SetTextMatrix(nRow, nCol, "%.1f", len);
}

void CVStiffPositionDlg::SetSellAttrUnitCross()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	long nGsu = pBridge->GetGirdersu();
	AhPtrArrayMake(nGsu+1,m_bSellArr, (CPtrArray*)0);	// CR Girder����
	CPlateGirderApp* pGir;
	CPtrArray* pDA;
	long cols = m_Grid.GetColumnCount();
	double dMiss = 5.0;

	for(long g = -1; g < nGsu; g++)
	{
		pDA = (CPtrArray*)m_bSellArr.GetAt(g+1);
		pDA->RemoveAll();
		pGir = pBridge->GetGirder(g);

		long nCol = 1;
		long nRow = g*2+3;
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while (Finder.GetBxFixNext())
		{
			double staDiap = Finder.GetBxFixNext()->GetStation(), staVStf;
			do	{
				if (nCol >= cols)
					break;
				pDA->Add(pBx);	// ������ �ȵǴ� ��
				staVStf = GetStationMatrix(nRow,nCol++);
			}
			while(staVStf + dMiss < staDiap);
			pBx = Finder.GetBxNext();
		}
	}
}

double CVStiffPositionDlg::GetStationMatrix(long nRow, long nCol)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	double lenSum = 0;
	for(long c = 1; c <= nCol; c++)
		lenSum += m_Grid.GetTextMatrixDouble(nRow, c);
	double sta = pBridge->GetStationByLengthInGrid(TRUE, nRow/2-1, lenSum, m_nLengthType, m_dDisCenterOnVst);
	return sta;
}

void CVStiffPositionDlg::SetGridAttrUnit()
{
	m_bRowColChange = FALSE;
	long cols = m_Grid.GetCols();
	long rows = m_Grid.GetRows();

	for(long r = 1; r < rows; r++)
	{		
		for(long c = 1; c < cols; c++)
		{			
			if(IsCellAttrFix(r,c))
			{
				m_Grid.SetItemState(r, c, m_Grid.GetItemState(r, c) | GVIS_READONLY);
				m_Grid.SetItemBkColour(r, c, RGBREADONLY);
			}
			else
			{
				m_Grid.SetItemState(r, c, m_Grid.GetItemState(r, c) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(r, c, RGB(225, 250, 250));
			}				
		}
	}

	m_bRowColChange = TRUE;

}

BOOL CVStiffPositionDlg::IsCellAttrFix(long row,long col) const
{
	if(row <= 0 || col <=0 ) return FALSE;
	row--; col--;
	long nGBound = row / 2;

	if(m_bSellArr.GetUpperBound() < nGBound)
		return FALSE;

	CPtrArray* pDA = (CPtrArray*)m_bSellArr.GetAt(nGBound);

	if(pDA->GetUpperBound() < col)
		return FALSE;

	return (pDA->GetUpperBound() == col || pDA->GetAt(col) != pDA->GetAt(col+1));
}

void CVStiffPositionDlg::OnButtonRecommend() 
{
	SetDataDefault();
	//SetDataSave();
	SetDataInit();
	SetDataBx();

	DrawInputDomyunView(TRUE);	
}

void CVStiffPositionDlg::SetGridCellInsertedCol(long row,long col) 
{	
	long rows = m_Grid.GetRowCount();	
	long currow = row;	

	CDWordArray tcArr;
	long idx = 0;

	long r = 0;
	for (r = 1; r < rows; r+=2)
	{
		if (!m_bSameApplySameCol && r != row)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;
		tcArr.Add(GetColMatchGirder(row, col, r));
	}
	for (r = 1; r < rows; r+=2)
	{
		if (!m_bSameApplySameCol && r != row)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;
		long nG = (r - 1) / 2 - 1;
		long tc = tcArr.GetAt(idx++);
		if (tc < 1)
			continue;
		for (long n = 0; n < 2; n++)
			if (r + n != currow)
				m_Grid.InsertColumn("", DT_CENTER|DT_VCENTER|DT_SINGLELINE, tc, r + n);
			
			double Dis = atof(m_Grid.GetItemText(r, tc+1));
			double DisPrev = Round(Dis/2, 0);
			double DisNext = Dis- DisPrev;			//Ȧ���� ������ ���Ͽ� ����.
			m_Grid.SetTextMatrix(r, tc, "%.0lf", DisPrev);
			m_Grid.SetTextMatrix(r, tc+1,"%.0lf", DisNext);
			m_Grid.SetTextMatrix(r+1, tc, (Dis == 0) ? "-" : "90");
			
			// �迭����
			CPtrArray* pDA = (CPtrArray*)m_bSellArr.GetAt(nG+1);
			pDA->InsertAt(tc-1, pDA->GetAt(tc-1));
	}

	m_bRowColChange = TRUE;
	SetGridTitle();
	DataSaveGrid();
	DrawInputDomyunView();

/*	for (long r = 1; r < rows; r++)   {
		if (!m_bSameApplySameCol && r != row)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;
		tcArr.Add(GetColMatchGirder(row, col, r));
	}
	for (r = 1; r < rows; r+=2)   {
		if (!m_bSameApplySameCol && r != row)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;
		long nG = (r - 1) / 2 - 1;
		long tc = tcArr.GetAt(idx++);
		if (tc < 1)
			continue;
		for (long n = 0; n < 2; n++)
			if (r + n != currow)
				m_Grid.InsertColumn("", DT_CENTER|DT_VCENTER|DT_SINGLELINE, tc, r + n);

		double Dis = atof(m_Grid.GetItemText(r, tc+1));
		double DisPrev = Round(Dis/2, 0);
		double DisNext = Dis- DisPrev;			//Ȧ���� ������ ���Ͽ� ����.
		m_Grid.SetTextMatrix(r, tc, "%.0lf", DisPrev);
		m_Grid.SetTextMatrix(r, tc+1,"%.0lf", DisNext);
		m_Grid.SetTextMatrix(r+1, tc, (Dis == 0) ? "-" : "90");

		// �迭����
		CPtrArray* pDA = (CPtrArray*)m_bSellArr.GetAt(nG+1);
		pDA->InsertAt(tc-1, pDA->GetAt(tc-1));
	}

	m_bRowColChange = TRUE;
	SetGridTitle();
	SetDataSaveGrid();
	DrawTotal();*/
}

void CVStiffPositionDlg::DataSaveGrid()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	
	long rows = m_Grid.GetRows();
	long cols = m_Grid.GetCols();
	pBridge->m_VStiffGrid.Resize(rows, cols);
	
	for (long r = 0; r < rows; r++)
	{
		long c = 0;
		for (c = 0; c < cols-1; c++)
			pBridge->m_VStiffGrid.SetTextMatrix(r, c, m_Grid.GetItemText(r, c));
		
		long nG = (r / 2)-1;
		if(r%2==1)
		{
			CString szCombo = m_Grid.GetTextMatrix(r, c);
			if(szCombo=="����")	pBridge->GetGirder(nG)->m_nInstallVStiff = 0;
			if(szCombo=="����")	pBridge->GetGirder(nG)->m_nInstallVStiff = 1;
			if(szCombo=="����")	pBridge->GetGirder(nG)->m_nInstallVStiff = 2;
		}
	}
	
	pBridge->m_dDisCenterOnVst	= m_dDisCenterOnVst;
	pBridge->m_nLengthTypeOnVst	= m_nLengthType;
}

void CVStiffPositionDlg::OnCellInsertedCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	
	SetGridCellInsertedCol(nRow,nCol);
	
	m_Grid.SetInsertThisOnly(!m_bSameApplySameCol);
	m_Grid.SetDeleteThisOnly(!m_bSameApplySameCol);	
	
	SetGridAttrUnit();
}

void CVStiffPositionDlg::OnCellInsertCol(NMHDR* nmgv, LRESULT*)
{
	m_Grid.SetInsertThisOnly(TRUE);
	m_Grid.SetDeleteThisOnly(TRUE);	
}

void CVStiffPositionDlg::OnCellDeleteCol(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	nRow -= (nRow - 1) % 2;  // ����Row�� �ٲ�.

	long rows = m_Grid.GetRows();
	CDWordArray tcArr;
	long idx = 0;
	long r = 0;
	for (r = 1; r < rows; r+=2)   
	{
		if (!m_bSameApplySameCol && r != nRow)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;
		tcArr.Add(GetColMatchGirder(nRow, nCol, r));
	}
	for (r = 1; r < rows; r+=2)   
	{
		if (!m_bSameApplySameCol && r != nRow)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;

		long nG = (r - 1) / 2 - 1;
		long tc = tcArr.GetAt(idx++);
		if (tc < 1 || IsCellAttrFix(r, tc))
			continue;
		
		double Dis1 = m_Grid.GetTextMatrixDouble(r, tc);
		double Dis2 = m_Grid.GetTextMatrixDouble(r, tc+1);
		m_Grid.SetTextMatrix(r, tc+1, "%.1lf", Dis1+Dis2);
				
		// �迭����
		CPtrArray* pDA = (CPtrArray*)m_bSellArr.GetAt(nG+1);
		pDA->RemoveAt(tc-1);		

		for (long n = 0; n < 2; n++)
			if (r + n != GridView->iRow)
				m_Grid.DeleteColumn(tc, r + n);
	}	
	m_Grid.DeleteUnusedColumn();

	m_Grid.SetInsertThisOnly(TRUE);
	m_Grid.SetDeleteThisOnly(TRUE);

	return;
}

void CVStiffPositionDlg::OnCellDeletedCol(NMHDR* nmgv, LRESULT*)
{
	m_Grid.SetInsertThisOnly(!m_bSameApplySameCol);
	m_Grid.SetDeleteThisOnly(!m_bSameApplySameCol);

	SetDataSaveGrid();
	DrawInputDomyunView(FALSE);

	return;
}

void CVStiffPositionDlg::OnCheckSametype() 
{
	UpdateData();
	m_Grid.SetInsertThisOnly(!m_bSameApplySameCol);
	m_Grid.SetDeleteThisOnly(!m_bSameApplySameCol);		
}

// ���� Cell��ġ�� ����������� ����(Match)�Ǵ� �ٸ��Ŵ��� ���������� ��ġ(Col) ���ϱ�.
long CVStiffPositionDlg::GetColMatchGirder(long nCurRow, long nCurCol, long nTarRow)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	
	if (nCurRow == nTarRow)
		return nCurCol;
	long nCols = m_Grid.GetColumnCount();
	long nCurGir = (nCurRow - 3) / 2;
	long nTarGir = (nTarRow - 3) / 2;
	double dMiss = 5.0;

	// ���� ���������� ������ �ִ� ���̾����� ã�� (Column, Bx)
	CPlateGirderApp *pCurGir = pBridge->GetGirder(nCurGir);
	long i = 0;
	for (i = nCurCol - 1; i > 0; i--)   {
		if (IsCellAttrFix(nCurRow, i))
			break;
	}
	CPtrArray* pDA = (CPtrArray *)m_bSellArr.GetAt(nCurGir + 1);
	long nCurCrossCol = i;
	CPlateBasicIndex *pCurCrossBx = (CPlateBasicIndex *)pDA->GetAt(nCurCol-1);

	// ���� ���̾������� �����Ǵ� �ٸ� �Ŵ��� ���̾����� ã�� (Bx)
	CPlateGirderApp *pTarGir = pBridge->GetGirder(nTarGir);
	CPlateBasicIndex *pTarCrossBx = pCurGir->GetBxMatchByCrossBeam(pCurCrossBx, pTarGir);
	if (pTarCrossBx == NULL)
		return -1;

	// ���� ���̾������� �ش�Ǵ� Column ã��
	double dTarCrossSta = pTarCrossBx->GetStation();
	double dTarSta;
	long	nTarCrossCol = 0;
	for (nTarCrossCol = 0; nTarCrossCol < nCols; nTarCrossCol++)   {
		dTarSta = GetStationMatrix(nTarRow, nTarCrossCol);
		if (dTarSta + dMiss >= dTarCrossSta)
			break;
	}
	// ���� Column ��ġ�κ��� ���ϴ� ���������� ���� (Column)
	long nTarCol = nTarCrossCol + (nCurCol - nCurCrossCol);
	// ���� ���������簡 �����ϴ��� ����
	for (i = nTarCrossCol + 1; i < nCols; i++)
		if (i == nTarCol || IsCellAttrFix(nTarRow, i))
			break;
	if (i == nTarCol)
		return nTarCol;
	return -1;
}

void CVStiffPositionDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{			
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;
	
	m_pView->GetDomyun()->ClearEtt(FALSE);

	long nG2 = -2;
	double presta2, sta2;
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->FixEtt(TRUE);

	for (long r = 1; r < m_Grid.GetRowCount(); r+=2)   
	{
		if (!m_bSameApplySameCol && r != nRow)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;
		long nG = (r-3) / 2;
		double presta = GetStationMatrix(r, nCol-1);
		double sta    = GetStationMatrix(r, nCol);
		
		if (r == nRow)   
		{
			nG2 = nG;
			presta2 = presta;
			sta2 = sta;
		}
		else
			Draw.SetCursorPosition(nG, presta, sta, pDom, TRUE, 50, 2, 0);
	}
	// �ڱ� �Ŵ��� �� ��������...
	if (nG2 >= -1)
	{
		Draw.SetCursorPosition(nG2, presta2, sta2, pDom, TRUE, 50, 2, 0);
	}

	pDom->FixEtt(FALSE);
}

void CVStiffPositionDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	if(nRow %2 == 0)
	{
		SetDataSaveGrid();
		SetDataSave();
		SetDataBx();
		SetDataInit();
		return;	//���� ��ġ�̸�, ��� �Ŵ� �������� ��ư ����� �������.
	}
	CString str = m_Grid.GetItemText(nRow,nCol);	
	double dPrevDis = atof( m_Grid.GetLastChangeData() );
	double dDifDis = atof(str) - dPrevDis;
	double dDifSta;

	if (m_nLengthType == 0)   {  // dDifDis�� �������̷� ��ȯ.
		double staNew = GetStationMatrix(nRow, nCol);
		m_Grid.SetTextMatrix(nRow,nCol,"%.1lf",dPrevDis);
		dDifSta = staNew - GetStationMatrix(nRow, nCol);
	}
	else
		m_Grid.SetTextMatrix(nRow,nCol,"%.1lf",dPrevDis);

	for (long r = 1; r < m_Grid.GetRows(); r+=2)   {
		if (!m_bSameApplySameCol && r != nRow)   // ���Ŵ����� �ƴҰ�� �ڱ� �Ŵ��� �۾�
			continue;
		long nMatchCol = GetColMatchGirder(nRow, nCol, r);
		if (m_nLengthType == 0)   {
			CPlateGirderApp *pGir = pBridge->GetGirder(r/2-1);
			double staStt = pGir->GetSSS(OFF_STT_SHOE), len = 0;
			for (long c = 1; c <= nMatchCol; c++)
				len += atof(m_Grid.GetItemText(r,c));
			double sta = pGir->GetStationByActLength(staStt, len);
			dDifDis = Round(pGir->GetLengthDimType(sta, sta + dDifSta), 0) * (dDifSta > 0 ? 1 : -1);
		}
		SetOneStepSel(r, nMatchCol, dDifDis, dPrevDis);
	}

//	if(GetKeyState(VK_RETURN) < 0)
//		GetDlgItem(IDC_VSTIFF_GRID)->PostMessage(WM_KEYDOWN,VK_RIGHT);

	SetDataSaveGrid();	
}

//	double dDifDis = dDis - dPrevDis;
void CVStiffPositionDlg::SetOneStepSel(long row, long col, double dDifDis, double dPrevDis)
{
	long nRow = row, nCol = col;
	if (dDifDis == 0 || col < 1 || IsCellAttrFix(row, col))
		return;

	CString str;
	double Len = m_Grid.GetTextMatrixDouble(row, col);
	Len += dDifDis;
	str.Format("%.lf",Len);
	m_Grid.SetTextMatrix(row, col, str);
	col--;

	long nGBound = (row-1) / 2;
	CPtrArray* pDA = (CPtrArray*)m_bSellArr.GetAt(nGBound);
	if(pDA->GetUpperBound() < col)// �米�� ��� ������ �������ε��� �ڿ� �Է��Ѱ��
	{
		CString sFmt; sFmt.Format("%.lf",dPrevDis);
		m_Grid.SetTextMatrix(nRow,nCol,sFmt);
		return;
	}

	CPlateBasicIndex *pBx = (CPlateBasicIndex *)pDA->GetAt(col);

	while(1) // search fix col
	{
		if(pDA->GetUpperBound() < col) break;
		if(pBx != pDA->GetAt(col)) break;
		col++;
	}

	Len = m_Grid.GetTextMatrixDouble(row, col);
	Len -= dDifDis;
	str.Format("%.lf", Len);
	m_Grid.SetTextMatrix(row, col, str);

	m_Grid.Invalidate();
}

void CVStiffPositionDlg::OnVstiffArrangeThisgirder() 
{
	// TODO: Add your control notification handler code here
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	long nCurRow = m_Grid.GetFocusCell().row;
	if(nCurRow<=0) nCurRow = 1;
	nCurRow -= (nCurRow % 2) ? 0 : 1;

	CString str;
	str.Format("  %d�� �Ŵ� �������� ���������縦  \n ������ġ�� �����մϴ�. ����Ͻðڽ��ϱ�?", nCurRow/2);
	if (IDCANCEL == MessageBox(str, "��� �Ŵ� ��������", MB_OKCANCEL | MB_ICONQUESTION))
		return;

	long nRows = m_Grid.GetRows();
	long nCols = m_Grid.GetCols();
	long nEndCol = m_Grid.GetValidCol(nCurRow, FALSE);
	double dMiss = 200;

	for (long nRow = 1; nRow < nRows; nRow += 2)   {
		if (nRow == nCurRow)
			continue;
		long nCol = 1;
		CPlateGirderApp *pGir = pBridge->GetGirder(nRow/2-1);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		double staSttDiap = pBx->GetStation();
		double staStt = staSttDiap;
		pBx = Finder.GetBxNext();
		double staEndDiap = pBx->GetStation();
		long nCurCol = 0;
		for (nCurCol = 0; nCurCol <= nEndCol; nCurCol++)
		{
			double sta = GetStationMatrix(nCurRow, nCurCol);
			if (sta <= staStt)
				continue;
			if (sta >= staEndDiap)   {  // ���̾����� ��ġ
				SetMatrixByStation(nRow, nCol, staEndDiap);
				m_Grid.SetTextMatrix(nRow+1, nCol++, "-");
				pBx = Finder.GetBxNext();
				if (!pBx)	break;
				staSttDiap = staEndDiap;
				staEndDiap = pBx->GetStation();
			}
			if (sta - staSttDiap < dMiss || staEndDiap - sta < dMiss)
				continue;
			SetMatrixByStation(nRow, nCol, sta);
			m_Grid.SetTextMatrix(nRow+1, nCol++, "90");
		}
		if (nCurCol > nEndCol)   {
			while (pBx)   {
				SetMatrixByStation(nRow, nCol, pBx->GetStation());
				m_Grid.SetTextMatrix(nRow+1, nCol++, "-");
				pBx = Finder.GetBxNext();
			}
		}
		for ( ; nCol < nCols; nCol++)   {
			m_Grid.SetTextMatrix(nRow, nCol, "");
			m_Grid.SetTextMatrix(nRow+1, nCol, "");
		}
	}
	m_Grid.DeleteUnusedColumn();
	SetSellAttrUnitCross();	// ���̾����� ������ �� ������ ������ �迭�ʱ�ȭ
	SetGridAttrUnit();
	SetDataSaveGrid();
	SetDataSave();

	m_Grid.Invalidate();
	DrawInputDomyunView(FALSE);
}

void CVStiffPositionDlg::OnVstiffBridgelen() 
{
	// TODO: Add your control notification handler code here
	long nOldType = m_nLengthType;
	double dOldDisCenter = m_dDisCenterOnVst;
	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
	SetDataSaveGrid();
}

void CVStiffPositionDlg::OnVstiffGirderlen() 
{
	// TODO: Add your control notification handler code here
	long nOldType = m_nLengthType;
	double dOldDisCenter = m_dDisCenterOnVst;
	UpdateData();
	SetGridDataByChangeLengthType(nOldType, dOldDisCenter);
	SetDataSaveGrid();
}

void CVStiffPositionDlg::OnVstiffInputangle() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SetGridTitle();
}

void CVStiffPositionDlg::OnVstiffSetting1() 
{
	CGlobarOption   *pOpt    = m_pStd->GetDataManage()->GetGlobalOption();
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	SetGridByBaseLen(0);
	SetSellAttrUnitCross();	// ���̾����� ������ �� ������ ������ �迭�ʱ�ȭ
	SetGridAttrUnit();
	SetGridTitle();
	DrawInputDomyunView(FALSE);

	if(m_dBaseDist ==0)
	{
		pOpt->SetDesignVStiffInstall(1);//��ġ���� ����...
		EnableCheckInstall(FALSE);
		m_bCheckNotInstall = TRUE;
		UpdateData(FALSE);
	}
}

void CVStiffPositionDlg::OnVstiffSetting2() 
{
	// TODO: Add your control notification handler code here
	SetGridByBaseLen(1);
	SetSellAttrUnitCross();
	SetGridAttrUnit();
	SetGridTitle();
	DrawInputDomyunView(FALSE);
}

void CVStiffPositionDlg::SetGridDataByChangeLengthType(int nOldType, double dOldDisCenter)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();	

	long nRows = m_Grid.GetRows();
	long nCols = m_Grid.GetCols();

	for (long nRow = 1; nRow < nRows; nRow += 2)   {
		CPlateGirderApp *pGir = pBridge->GetGirder(nRow/2 - 1);
		double staStt = pGir->GetSSS(OFF_STT_SHOE);
		double sta, lenSum = 0;
		for (long nCol = 1; nCol < nCols; nCol++)   {
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
			SetMatrixByStation(nRow, nCol, sta);
		}
	}

	m_Grid.Invalidate();
}

BOOL CVStiffPositionDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();

	SetResize(IDC_VSTIFF_ARRANGE_THISGIRDER,	SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_VSTIFF_AUTO_SYMMETRY,			SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_GROUPBOX,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_VSTIFF_GIRDERLEN,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_VSTIFF_BRIDGELEN,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_VSTIFF_DISTANCE_CENTER,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_UNIT,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_VSTIFF_INPUTANGLE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_TITLE1,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_BASEDIST,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_UNIT1,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_VSTIFF_SETTING1 ,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_VSTIFF_SETTING2,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	SetResize(IDC_CHECK_NOTINSTALL,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SAMETYPE,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_GRID,  SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;
}

// �ڵ� ��Ī ��ư
void CVStiffPositionDlg::OnSetDataSymmetry() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	UpdateData(TRUE);

	if( m_bSameApplySameCol )
	{
		for (long nG = -1; nG < pBridge->GetGirdersu(); nG++)
			VStiffAutoSymmetry(nG);
	}
	else
		VStiffAutoSymmetry((m_Grid.GetFocusCell().row-1)/2-1);

	m_Grid.DeleteUnusedColumn();
	SetGridTitle();
}

void CVStiffPositionDlg::VStiffAutoSymmetry(long nG)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	long nCols = m_Grid.GetCols();
	long nRow = nG*2+3;
	long nPier=1;
	CPlateGirderApp* pGir = pBridge->GetGirder(nG);
	double SttSta = pGir->GetSSS(OFF_STT_SHOE);
	double EndSta = pGir->GetSSS(OFF_END_SHOE);
	double HalfSta = (SttSta + EndSta) / 2.0;  // ���� ����� ���� �߰� == ��Ī��(������)
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
		if (sta == HalfSta)   {  // �̷� ���� ��¥ �幰�ž�...
			nCenCol++;
			break;
		}
		if (sta > HalfSta)
			break;
		staMid = pGir->GetStationByOneStepMid(sta, 0);
		staSym = pGir->GetStationByOneStepMid(HalfSta + (HalfSta - staMid), 0);  // �̵����� ������.
		staSym -= sta - staMid;  // �̵��� ����.
		StaArr.Add(staSym);
	}
	long nNewCols = nCenCol + StaArr.GetSize() + 1;
	if (m_Grid.GetCols() < nNewCols)
		m_Grid.SetColumnCount(nNewCols);

	long nSymCol = StaArr.GetSize();
	for ( ; nCenCol < nNewCols - 1; nCenCol++)
	{
		staSym = StaArr.GetAt(nSymCol - 1);
		SetMatrixByStation(nRow, nCenCol, staSym);
		SetMatrixByStation(nRow+1, nCenCol, pGir->GetStationByOneStepMid(staSym, 0));
		nSymCol--;
	}
	// ������ ���� ����
	SetMatrixByStation(nRow, nCenCol, pGir->GetSSS(OFF_END_SHOE));
	m_Grid.SetTextMatrix(nRow+1, nCenCol, "%.0f", 0.0);

	// �ڿ� ���� Data ����.
	for (nCenCol++; nCenCol < m_Grid.GetCols(); nCenCol++)
	{
		m_Grid.SetTextMatrix (nRow, nCenCol, "");
		m_Grid.SetTextMatrix (nRow+1, nCenCol, "");
	}
}

void CVStiffPositionDlg::OnCheckNotinstall() 
{
	CGlobarOption   *pOpt    = m_pStd->GetDataManage()->GetGlobalOption();

	UpdateData();

	if(m_bCheckNotInstall)//��ġ���� �����̸�..
	{
		pOpt->SetDesignVStiffInstall(1);//��ġ���� ����...
		m_dBaseDist = 0;
		UpdateData(FALSE);

		OnVstiffSetting1();		
		EnableCheckInstall(FALSE);
	}
	else//��ġ�ϸ�...
	{
		EnableCheckInstall(TRUE);	
		pOpt->SetDesignVStiffInstall(0);//��ġ
		OnButtonRecommend();
	}	
}

void CVStiffPositionDlg::EnableCheckInstall(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_BASEDIST)->EnableWindow(bEnable);
	GetDlgItem(IDC_VSTIFF_SETTING1)->EnableWindow(bEnable);	
}
