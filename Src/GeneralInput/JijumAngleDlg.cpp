// JijumAngleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "JijumAngleDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateData/APlateData.h"

#include "ShoeZapyoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJijumAngleDlg dialog


CJijumAngleDlg::CJijumAngleDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CJijumAngleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJijumAngleDlg)
	m_nShoeDir = 0;	
	m_bStartOffset = TRUE;
	m_bEndOffset = TRUE;
	m_nGirEndDir = 1;
	m_dGirStndCuttingAngle = 0.0;
	//}}AFX_DATA_INIT
}


void CJijumAngleDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJijumAngleDlg)
	DDX_Control(pDX, IDC_EDIT_GIR_STND_ANGLE, m_ctrlGirStndCuttingAngle);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Radio(pDX, IDC_RADIO_SHOEDIR, m_nShoeDir);	
	DDX_Check(pDX, IDC_CHECK_STTOFFSET, m_bStartOffset);
	DDX_Check(pDX, IDC_CHECK_ENDOFFSET, m_bEndOffset);
	DDX_Radio(pDX, IDC_RADIO_GIREND_CT_VER, m_nGirEndDir);
	DDX_Text(pDX, IDC_EDIT_GIR_STND_ANGLE, m_dGirStndCuttingAngle);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ANGLEDLG_RIGHTANG, m_btnAngRight);
	DDX_Control(pDX, IDC_ANGLEDLG_A1EQUAL, m_btnAngQual);
	DDX_Control(pDX, IDC_ANGLEDLG_A1PARALLEL, m_btnAngParallel);
	DDX_Control(pDX, IDC_SHOE_COORDIANTE, m_btnShoeCordinate);

}


BEGIN_MESSAGE_MAP(CJijumAngleDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CJijumAngleDlg)
	ON_BN_CLICKED(IDC_CHECK_STTOFFSET, OnCheckSttoffset)
	ON_BN_CLICKED(IDC_CHECK_ENDOFFSET, OnCheckEndoffset)
	ON_BN_CLICKED(IDC_RADIO_SHOEDIR, OnShoeDir1)
	ON_BN_CLICKED(IDC_RADIO_SHOEDIR2, OnShoeDir2)
	ON_BN_CLICKED(IDC_RADIO_SHOEDIR3, OnShoeDir3)
	ON_BN_CLICKED(IDC_RADIO_SHOEDIR4, OnShoeDir4)
	ON_BN_CLICKED(IDC_ANGLEDLG_A1EQUAL, OnAngleA1Equal)
	ON_BN_CLICKED(IDC_ANGLEDLG_A1PARALLEL, OnAngleA1Parallel)
	ON_BN_CLICKED(IDC_ANGLEDLG_RIGHTANG, OnAngleRighTang)
	ON_BN_CLICKED(IDC_SHOE_COORDIANTE, OnShoeCoordiante)
	ON_BN_CLICKED(IDC_RADIO_GIREND_CT_VER, OnRadioGirendCtVer)
	ON_BN_CLICKED(IDC_RADIO_GIREND_SA_PYUNG, OnRadioGirendSaPyung)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJijumAngleDlg message handlers

void CJijumAngleDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());
	Draw.DrawPyung(&Dom, 1);
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CJijumAngleDlg::SetGridTitle()
{
	CPlateBridge *pBridge = m_pStd->GetBridge();

	long nJigan  = pBridge->m_nQtyJigan;
	long nColumn = nJigan + 4;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(2);
	m_Grid.SetColumnCount(nColumn);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(60);		
	m_Grid.SetColumnWidth(0, 70);

	m_Grid.SetTextMatrix(0, 0, "구   분");
	m_Grid.SetTextMatrix(1, 0, "각도(˚)");
	m_Grid.SetTextMatrix(0, 1, "시 점");

	CString str;
	for(long nCol = 2; nCol < nColumn-1; nCol++)
	{
		str = pBridge->m_strJijumName[nCol - 2];
		m_Grid.SetTextMatrix(0,nCol,str);
	}

	m_Grid.SetTextMatrix(0, nColumn - 1, "종 점");

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CJijumAngleDlg::SetDataInit()
{
	CPlateBridge *pBridge = m_pStd->GetBridge();

	SetGridTitle();

	m_nShoeDir  = (BYTE)pBridge->m_cDirShoe;
	m_bStartOffset = pBridge->m_cOffSetStt;
	m_bEndOffset = pBridge->m_cOffSetEnd;

	CString str;

	long nCols = m_Grid.GetColumnCount();	
	CPlateGirderApp*	pGirCR = pBridge->GetGirder(-1);
	
	double angDeg = pBridge->m_vAngJijum[0].GetAngleDegree();	
	m_Grid.SetTextMatrix(1,1,"%.4lf",angDeg);   // 시점

	angDeg = pGirCR->GetAAA(OFF_STT_SHOE).GetAngleDegree();	
	m_Grid.SetTextMatrix(1, 2,"%.4lf",angDeg);   // A1	
	m_Grid.SetItemBkColour(1, 2, RGBREADONLY);
	m_Grid.SetItemState(1, 2, GVIS_READONLY);

	for(long nCol = 3;nCol < nCols-2;nCol++)
	{		
		m_Grid.SetTextMatrix(1,nCol,"%.4lf",pBridge->m_vAngJijum[nCol-2].GetAngleDegree());   // Pier
		m_Grid.SetItemState(1, nCol, GVIS_MODIFIED);
	}

	m_Grid.SetTextMatrix(1, nCols-2,"%.4lf", pGirCR->GetAAA(OFF_END_SHOE).GetAngleDegree());   // A2	
	m_Grid.SetItemBkColour(1, nCols-2, RGBREADONLY);
	m_Grid.SetItemState(1, nCols-2, GVIS_READONLY);
	m_Grid.SetTextMatrix(1,nCols-1,"%.4lf", pBridge->m_vAngJijum[pBridge->m_nQtyJigan].GetAngleDegree());   // 종점
	

	// 사교시 거더 끝단형태
	CGlobarOption* pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	m_nGirEndDir = pOpt->GetDesignGirderEndType();

	m_dGirStndCuttingAngle	= pBridge->m_dGirEndStndGuttingAngle;

	UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE,TRUE);
}
//사용안함.
void CJijumAngleDlg::SetDataDefault()
{	
	CPlateBridge *pBridge = m_pStd->GetBridge();
	CGlobarOption* pOpt = m_pStd->GetDataManage()->GetGlobalOption();

	pOpt->SetDesignGirderEndType(1);
	
	pBridge->m_cDirShoe = 0;
	pBridge->m_cOffSetStt = FALSE;
	pBridge->m_cOffSetEnd = FALSE;

	for(long nJ = 1;nJ < pBridge->m_nQtyJigan+1;nJ++)
	{
		pBridge->m_vAngJijum[nJ] = ANGLE90;		
	}	
}

void CJijumAngleDlg::SetDataSave(BOOL bSttAbut, BOOL bEndAbut)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CCalcData       *pData   = m_pStd->GetDataManage()->GetCalcData();

	UpdateData();

	pBridge->m_cDirShoe = m_nShoeDir;
	pBridge->m_cOffSetStt = m_bStartOffset;
	pBridge->m_cOffSetEnd = m_bEndOffset;

	CPlateGirderApp* pGirCR = pBridge->GetGirder(-1);
	
	long	nKy  = pBridge->m_nQtyJigan;
	long	cols = m_Grid.GetColumnCount();
	double  ang;
	CDPoint vNormal;  // 지점 각도의 수직방향. (정밀도 위해 사용. 특히 90도인 경우)	
	if(bSttAbut)	
	{		
		if(m_bStartOffset)
		{
			double dAngle = atof(m_Grid.GetItemText(1, 2)) - 90;
			CDPoint vAng = ToDPointFrRadian(ToRadian(dAngle));
			vNormal = pGirCR->GetAngleOffset(pGirCR->GetSSS(OFF_STT_SHOE),vAng,-pBridge->m_dLengthUganStt-pBridge->m_dLengthGirderShoeStt-pBridge->m_dLengthSlabGirderStt);
		}
		else
		{
			ang = atof(m_Grid.GetItemText(1, 2)) - 90;				
			vNormal = ToDPointFrRadian(ToRadian(ang));
		}
	}
	else
	{
		ang = atof(m_Grid.GetItemText(1, 1)) - 90;				
		vNormal = ToDPointFrRadian(ToRadian(ang));
	}
	pBridge->m_vAngJijum[0] = vNormal.Rotate90();		
	for(long col = 3; col < cols-2; col++)
	{
		ang = atof(m_Grid.GetItemText(1,col)) - 90;
		vNormal = ToDPointFrRadian(ToRadian(ang));
		pBridge->m_vAngJijum[col-2] = vNormal.Rotate90();
	}

	if(bEndAbut)	
	{
		if(m_bEndOffset)
		{
			double dAngle = atof(m_Grid.GetItemText(1, cols-2)) - 90;
			CDPoint vAng = ToDPointFrRadian(ToRadian(dAngle));
			vNormal = pGirCR->GetAngleOffset(pGirCR->GetSSS(OFF_END_SHOE),vAng,+pBridge->m_dLengthUganStt+pBridge->m_dLengthGirderShoeStt+pBridge->m_dLengthSlabGirderStt);		
		}
		else
		{
			ang = atof(m_Grid.GetItemText(1, cols-2)) - 90;				
			vNormal = ToDPointFrRadian(ToRadian(ang));
		}
	}
	else
	{
		ang = atof(m_Grid.GetItemText(1,cols-1)) - 90;
		vNormal = ToDPointFrRadian(ToRadian(ang));
	}
	pBridge->m_vAngJijum[nKy]		= vNormal.Rotate90();
	pBridge->m_cOffSetStt			= (BYTE)m_bStartOffset;
	pBridge->m_cOffSetEnd			= (BYTE)m_bEndOffset;

	pData->DESIGN_BASE.m_dBridgeSkew = pBridge->m_vAngJijum[0].GetAngleDegree();


	// 사교시 거더 끝단형태
	CGlobarOption* pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	pOpt->SetDesignGirderEndType(m_nGirEndDir);

	pBridge->m_dGirEndStndGuttingAngle	= m_dGirStndCuttingAngle;
}

BOOL CJijumAngleDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(FALSE);
	
	SetResize(IDC_ANGLEDLG_RIGHTANG,	   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_ANGLEDLG_A1EQUAL,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_ANGLEDLG_A1PARALLEL,	   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_SHOE_COORDIANTE,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
 	SetResize(IDC_STATIC1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_TITLE1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_SHOEDIR, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_SHOEDIR2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_SHOEDIR3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_SHOEDIR4, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_GIREND_CT_VER, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_GIREND_SA_PYUNG, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_GIR_STND_ANGLE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);


	return TRUE;
}

void CJijumAngleDlg::OnCheckSttoffset() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	UpdateData();

	m_bStartOffset = !m_bStartOffset;
	pBridge->m_cOffSetStt = (BYTE)m_bStartOffset;

	SetDataSave();	
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CJijumAngleDlg::OnCheckEndoffset() 
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	UpdateData();

	m_bEndOffset = !m_bEndOffset;
	pBridge->m_cOffSetEnd = (BYTE)m_bEndOffset;

	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

////////////////////////////////////////////////////////////////////////
///BX_STT_GIRDER(거더시점)
///BX_END_GIRDER(거더종점)
///BX_STT_JOINT (신축이음부시점)
///BX_END_JOINT (신축이음부종점)
///BX_STT_SLAB(거더선에서 슬래브 시점)
///BX_END_SLAB(거더선에서 슬래브 종점)
////////////////////////////////////////////////////////////////////////
void CJijumAngleDlg::SetDataBx()
{	
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();

	for(long nG = -1; nG < pBridge->GetGirdersu(); nG++)
	{
		CPtrList NList;
		CPlateBasicIndex *p = NULL;
		double sta = 0;
		CDPoint vAng(0,1);

		CPlateGirderApp* pGir = pBridge->GetGirder(nG);
		///<BX_STT_GIRDER
		sta = pGir->GetSSS(OFF_STT_GIRDER);
		vAng = pGir->GetAAA(OFF_STT_GIRDER);
		p = pGir->newBx();
		p->SetState(BX_STT_GIRDER,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );
		///<BX_END_GIRDER
		sta = pGir->GetSSS(OFF_END_GIRDER);
		vAng = pGir->GetAAA(OFF_END_GIRDER);
		p = pGir->newBx();
		p->SetState(BX_END_GIRDER,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );
		///<BX_STT_JOINT
		sta = pGir->GetStationStt();
		vAng = pGir->GetAngleStt();
		p = pGir->newBx();
		p->SetState(BX_STT_JOINT,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );
		///<BX_END_JOINT
		sta = pGir->GetStationEnd();
		vAng = pGir->GetAngleEnd();
		p = pGir->newBx();
		p->SetState(BX_END_JOINT,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );
		///<BX_STT_SLAB
		sta = pGir->GetSSS(OFF_STT_SLAB);
		vAng = pGir->GetAAA(OFF_STT_SLAB);
		p = pGir->newBx();
		p->SetState(BX_STT_SLAB,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );
		///<BX_END_SLAB
		sta = pGir->GetSSS(OFF_END_SLAB);
		vAng = pGir->GetAAA(OFF_END_SLAB);
		p = pGir->newBx();
		p->SetState(BX_END_SLAB,TRUE);
		p->SetStation(sta);
		p->SetAngle(vAng);
		NList.AddTail( p );

		for(long nJijum=0; nJijum<=pBridge->m_nQtyJigan; nJijum++)
		{
			CPlateBasicIndex * p;
			CDPoint vAng(0,1);

			CPlateGirderApp* pGir = pBridge->GetGirder(nG);

			sta  = pGir->GetStationOnJijum(nJijum);
			vAng = pGir->GetAngleOnJijum(nJijum);
			p = pGir->newBx();
			p->SetState(BX_JIJUM, TRUE);
			p->SetStation(sta);
			p->SetAngle(vAng);
			NList.AddTail( p );
		}
	
		pGir->PressBxList(NList);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);
	}

}

void CJijumAngleDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{	
	CGeneralBaseDlg::OnCellChangedData(nmgv, 0);	
	
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
    int nCol = GridView->iColumn;

	if(nCol==2)									//시점 슈 각도에 입력이 되었을 경우
		SetDataSave(TRUE,FALSE);
	else if(nCol==m_Grid.GetColumnCount() - 2)	//종점 슈 각도에 입력이 되었을 경우
		SetDataSave(FALSE,TRUE);
	else
		SetDataSave();								//일반 각도 입력을 하였을 경우

	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CJijumAngleDlg::OnShoeDir1() 
{
	m_nShoeDir = 0;
	SetDataSave();	
	DrawInputDomyunView(FALSE);
}

void CJijumAngleDlg::OnShoeDir2() 
{
	m_nShoeDir = 1;
	SetDataSave();	
	DrawInputDomyunView(FALSE);
}

void CJijumAngleDlg::OnShoeDir3() 
{
	m_nShoeDir = 2;
	SetDataSave();	
	DrawInputDomyunView(FALSE);
}

void CJijumAngleDlg::OnShoeDir4() 
{
	m_nShoeDir = 3;
	SetDataSave();	
	DrawInputDomyunView(FALSE);
	
}

void CJijumAngleDlg::OnAngleA1Equal() 
{
	m_bStartOffset = TRUE;
	m_bEndOffset   = TRUE;
	UpdateData(FALSE);
	CString str;
	str = m_Grid.GetItemText(1,1);

	for(long col = 2;col < m_Grid.GetColumnCount() ;col++)
	{
		m_Grid.SetTextMatrix(1,col,str);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);	
	SetDataSave();
	SetDataBx();
	DrawInputDomyunView();
}

void CJijumAngleDlg::OnAngleA1Parallel() 
{
	m_bStartOffset = TRUE;
	m_bEndOffset   = TRUE;

	CPlateBridge *pBridge = m_pStd->GetBridge();
	CPlateGirderApp *pGirCR = pBridge->GetGirder(-1);

	UpdateData(FALSE);
	double sttSta = pBridge->m_dStationBridgeStt;
	double  ang = atof(m_Grid.GetItemText(1,1)) - 90;
	CDPoint vSttAng = ToDPointFrRadian(ToRadian(ang));
	vSttAng = vSttAng.Rotate90();
	CString str;

	for(long col = 2;col < m_Grid.GetColumnCount() ;col++)
	{
		double sta;
		if (col < m_Grid.GetColumnCount() - 1)
			sta = pGirCR->GetStationOnJijum(col - 2);
		else
			sta = pGirCR->GetStationEnd();
		ang = pGirCR->GetLine()->GetAngleByOffset(sttSta, sta, vSttAng).GetAngleRadian();
		str.Format("%.4lf", ToDegree(ang));
		m_Grid.SetTextMatrix(1,col,str);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);	
	SetDataSave();
	SetDataBx();
	DrawInputDomyunView();
}

void CJijumAngleDlg::OnAngleRighTang() 
{
	m_bStartOffset = TRUE;
	m_bEndOffset   = TRUE;

	UpdateData(FALSE);
	CString str;
	str = "90.0000";

	for(long col = 1;col < m_Grid.GetColumnCount() ;col++)
	{
		m_Grid.SetTextMatrix(1,col,str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);	
	SetDataSave();
	SetDataBx();
	DrawInputDomyunView();
}

void CJijumAngleDlg::OnShoeCoordiante() 
{
	CPlateBridge *pBridge = m_pStd->GetBridge();
	SetDataSave();

	CShoeZapyoDlg Dlg(this);
	
	Dlg.m_pDB = pBridge;
	Dlg.m_pLineInfoApp = pBridge->m_pLine;

	Dlg.DoModal();	
}

void CJijumAngleDlg::OnRadioGirendCtVer() 
{
	UpdateData();
	SetDataSave();
}

void CJijumAngleDlg::OnRadioGirendSaPyung() 
{
	UpdateData();
	SetDataSave();	
}

void CJijumAngleDlg::OnButtonApply()
{
	UpdateData();
	SetDataSave();
	SetDataInit();

	DrawInputDomyunView(FALSE);
}
