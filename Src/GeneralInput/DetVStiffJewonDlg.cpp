// DetVStiffJewonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../APlateCalc/APlateCalc.h"
#include "generalinput.h"
#include "../APlateData/APlateData.h"
#include "DetVStiffJewonDlg.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "VStiffStdInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetVStiffJewonDlg dialog
 

CDetVStiffJewonDlg::CDetVStiffJewonDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDetVStiffJewonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetVStiffJewonDlg)
	m_nCmpPos = 0;
	m_szStation = _T("");
	m_bTotalView = FALSE;
	m_radioArr = -1;
	m_cCarryUpper = FALSE;
	m_cCarryLower = FALSE;
	//}}AFX_DATA_INIT
	m_bArr = FALSE;
	m_nG = 0;	
}


void CDetVStiffJewonDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetVStiffJewonDlg)
	DDX_Control(pDX, IDC_CHECK_VSTIFF_ARRANGEMENT, m_checkArr);
	DDX_Control(pDX, IDC_COMBO_VSTIFF_POSITION, m_ctlComboType);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_CBIndex(pDX, IDC_COMBO_VSTIFF_POSITION, m_nCmpPos);
	DDX_Text(pDX, IDC_STATIC_STATION, m_szStation);
	DDX_Check(pDX, IDC_CHECK_TOTALVIEW, m_bTotalView);
	DDX_Radio(pDX, IDC_RADIO_LEFT, m_radioArr);
	DDX_Check(pDX, IDC_CHECK_DONGBALI_UP, m_cCarryUpper);
	DDX_Check(pDX, IDC_CHECK_DONGBALI_DOWN, m_cCarryLower);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_DESIGN_CHECK, m_btnDesignCheck);
	DDX_Control(pDX, IDC_BUTTON_AUTOCALC, m_btnAutoCalc);
	DDX_Control(pDX, IDC_BUTTON_INPUT_BASICTYPE, m_btnInputBasicType);
	DDX_Control(pDX, IDC_BUTTON_INPUT_ALLGIRSAME, m_btnAllGirder);
	DDX_Control(pDX, IDC_BUTTON_INPUT_ALLSAME, m_btnAllVStiff);
	DDX_Control(pDX, IDC_VSTIFF_INPUTCOMPLETE, m_btnAllVStiffInput);
}

BEGIN_MESSAGE_MAP(CDetVStiffJewonDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDetVStiffJewonDlg)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnDialogNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnDialogPrev)
	ON_CBN_SELCHANGE(IDC_COMBO_VSTIFF_POSITION, OnSelchangeComboPosition)
	ON_BN_CLICKED(IDC_VSTIFF_INPUTCOMPLETE, OnVstiffInputcomplete)
	ON_BN_CLICKED(IDC_BUTTON_AUTOCALC, OnButtonAutocalc)
	ON_BN_CLICKED(IDC_CHECK_TOTALVIEW, OnCheckTotalview)
	ON_BN_CLICKED(IDC_CHECK_VSTIFF_ARRANGEMENT, OnCheckVstiffArrangement)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_BASICTYPE, OnButtonInputStandardType)
	ON_BN_CLICKED(IDC_RADIO_LEFT, OnRadioLeftOnlyVStiff)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, OnRadioRightOnlyVStiff)
	ON_BN_CLICKED(IDC_RADIO_SYMMETRY, OnRadioSymmetryOnlyVStiff)
	ON_BN_CLICKED(IDC_CHECK_DONGBALI_DOWN, OnCheckDongbaliDown)
	ON_BN_CLICKED(IDC_CHECK_DONGBALI_UP, OnCheckDongbaliUp)
	ON_BN_CLICKED(IDC_BUTTON_DESIGN_CHECK, OnButtonDesignCheck)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_ALLGIRSAME, OnButtonInputAllgirsame)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_ALLSAME, OnButtonInputAllsame)
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommend)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_MESSAGE(WM_GET_NEARBX, OnClickTotalView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetVStiffJewonDlg message handlers
void CDetVStiffJewonDlg::OnPreInitDialog()
{
	SetComboString();
	SetVStiffSetting();
	SetControl();
}

void CDetVStiffJewonDlg::SetVStiffSetting(long nMode)
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	if(pDB->m_nInstallVStiff==0) //양측배치
	{
		m_checkArr.SetCheck(FALSE);
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SYMMETRY)->EnableWindow(FALSE);
		m_bArr = FALSE;
	}
	else
	{
		m_checkArr.SetCheck(TRUE);
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SYMMETRY)->EnableWindow(TRUE);
		m_bArr = TRUE;

		if(pDB->m_nInstallVStiff==1)	// 좌측
		{
			m_radioArr = 0;
		}
		else if(pDB->m_nInstallVStiff==2) // 우측
		{
			m_radioArr = 1;
		}
		else if(pDB->m_nInstallVStiff==3)	// 양측
		{
			m_radioArr = 2;
		}
	}


	UpdateData(FALSE);
}

void CDetVStiffJewonDlg::OnDialogPrev() 
{
	SetDataSave();		

	--m_nCmpPos;
	if(m_nCmpPos < 0) {
		m_pStd->GetDetHBracing()->m_nPos = 0;
		m_pStd->GetDetVBracingTypeDlg()->m_nPos = 0;
		m_pStd->MovePrevDialog();
		return;
	}	
	UpdateData(FALSE);
	SetDataInit();
	DrawInputDomyunView(TRUE);	
}

void CDetVStiffJewonDlg::OnDialogNext() 
{
	SetDataSave();

	++m_nCmpPos;	
	if(m_nCmpPos >= m_ctlComboType.GetCount()) 
	{
		m_pStd->MoveNextDialog();
		return;
	}	
	UpdateData(FALSE);
	SetDataInit();


	DrawInputDomyunView(TRUE);	
}

void CDetVStiffJewonDlg::SetComboString(long nG)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();	


	m_ctlComboType.ResetContent();//초기화..

	CString sMid = "";
	long nCount = 0;
	long nVSCount = 0;
	BOOL bWrite = FALSE;

	CPlateBxFinder Finder(pDB->GetGirder(nG));
	CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF);
	
	while(pBx)
	{
		CSection *pSec = pBx->GetSection();
		//ASSERT(pSec);
		if(pSec)
		{	
			nCount++;
			nVSCount++;
			bWrite = FALSE;
			if(pSec->GetenDetType()==SECDET_CROSSBEAM)// 가로보 타입	
			{
				sMid = "C";					
				nVSCount = 0;
			}
			else if(pSec->GetenDetType()==SECDET_VBRACING)// 브레이싱 타입			
			{
				sMid = "B";
				nVSCount = 0;
			}
			else//두 타입이 아닌것도 있네..
			{
				nCount--;
				bWrite = TRUE;
			}
		}
		else
		{
			nVSCount++;
			bWrite = TRUE;
		}

		if (bWrite)
		{
			CString szType;			
			szType.Format("수직보강재-%s%d-%d", sMid, nCount, nVSCount);		
			m_ctlComboType.AddString(szType);			
		}
		
		pBx = (CPlateBasicIndex *)Finder.GetBxNext();		
	}	
}

void CDetVStiffJewonDlg::SetDataInit()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();		
	UpdateData();
	if(m_nCmpPos<0) 
	{
		m_nCmpPos=0;
		UpdateData(FALSE);
	}

	SetGridTitle();

	for(long i=0; i<pDB->GetGirdersu();i++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(i);
		if(m_nCmpPos >= pGir->GetQtyVStiff()) continue;

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		pBx = (CPlateBasicIndex *)Finder.GetBxNext(m_nCmpPos);
		CSection *pSec = pBx->GetSection();
		ASSERT(pSec);
		CVStiff *pVStiff = pSec->GetVStiff();
		ASSERT(pVStiff);	
		if(pVStiff)
		{
			//left
			m_Grid.SetTextMatrix(i+1,1,"%.0lf", pVStiff->m_dWidth[0]); 
			m_Grid.SetTextMatrix(i+1,2,"%.0lf", pVStiff->m_dThick[0]); 		
			m_Grid.SetTextMatrix(i+1,3,"%.0lf", pVStiff->m_dGap[0]);
			if(pVStiff->m_dThick[0]!=0)
				m_Grid.SetTextMatrix(i+1,4,"%.0lf", pVStiff->m_dH[LEFT][DOWN]);
			else
				m_Grid.SetTextMatrix(i+1,4,"%.0lf", 0.0);
			//right
			m_Grid.SetTextMatrix(i+1,5,"%.0lf",  pVStiff->m_dWidth[1]); 
			m_Grid.SetTextMatrix(i+1,6,"%.0lf", pVStiff->m_dThick[1]);
			m_Grid.SetTextMatrix(i+1,7,"%.0lf", pVStiff->m_dGap[1]);
			if(pVStiff->m_dThick[1]!=0)
				m_Grid.SetTextMatrix(i+1,8,"%.0lf", pVStiff->m_dH[RIGHT][DOWN]);
			else 
				m_Grid.SetTextMatrix(i+1,8,"%.0lf", 0.0);
		}

		// 수평브레이싱 연결판 
		m_Grid.SetTextMatrix(i+1, 9, _T(""));//"dTL"
		m_Grid.SetTextMatrix(i+1, 10,  _T(""));//"dTR"
		//CHBracingGusset *pBrRu = pGir->GetHBracingGussetInTotal(m_nCmpPos);
		CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
		if(pBrRu)	
		{
			if(pBrRu->IsReal())
			{
				CString str;
				str.Format("%g", pBrRu->m_dT);	m_Grid.SetTextMatrix(i+1, 9, str);//"dTL"
				str.Format("%g", pBrRu->m_ddH);	m_Grid.SetTextMatrix(i+1, 10, str);//"dTR"
				m_Grid.SetItemBkColour(i+1, 9, RGB(225, 250, 250)/*RGBEDITABLE*/);
				m_Grid.SetItemState(i+1, 9, GVIS_MODIFIED);
				m_Grid.SetItemBkColour(i+1, 10, RGB(225, 250, 250)/*RGBEDITABLE*/);
				m_Grid.SetItemState(i+1, 10, GVIS_MODIFIED);
			}
			else
			{
				m_Grid.SetItemBkColour(i+1, 9, RGBREADONLY);
				m_Grid.SetItemState(i+1, 9, GVIS_READONLY);
				m_Grid.SetItemBkColour(i+1, 10, RGBREADONLY);
				m_Grid.SetItemState(i+1, 10, GVIS_READONLY);
			}
		}	
		else
		{
			m_Grid.SetItemBkColour(i+1, 9, RGBREADONLY);
			m_Grid.SetItemState(i+1, 9, GVIS_READONLY);
			m_Grid.SetItemBkColour(i+1, 10, RGBREADONLY);
			m_Grid.SetItemState(i+1, 10, GVIS_READONLY);
		}
	}
	m_Grid.SetRedraw(TRUE, TRUE);
}


void CDetVStiffJewonDlg::SetDataDefaultDepyoDanmyun()
{
	CPlateBridgeApp	*pDB  = m_pStd->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(0);  
	// 대표단면을 위한 기본제원은 거더1번의 시작지점
	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetBxOnJijum(0);
	double dWebT = pGir->GetThickJewon(G_W, pBx);

	if(pDB->IsPlateGir())
	{
		if(pDB->m_dA_p==0)		pDB->m_dA_p = dWebT*5;
		if(pDB->m_dB_p==0)		pDB->m_dB_p = 20;
		if(pDB->m_dC_p==0)		pDB->m_dC_p = 150;
		if(pDB->m_dT_p==0)		pDB->m_dT_p = 12;

		if(pDB->m_dC_n==0)		pDB->m_dC_n = 150;
		if(pDB->m_dT_n==0)		pDB->m_dT_n = 12;
	}
	else
	{
		if(pDB->m_dA_p==0)		pDB->m_dA_p = dWebT*5;
		if(pDB->m_dB_p==0)		pDB->m_dB_p = 20;
		if(pDB->m_dC_p==0)		pDB->m_dC_p = 250;
		if(pDB->m_dT_p==0)		pDB->m_dT_p = 22;

		if(pDB->m_dC_n==0)		pDB->m_dC_n = 250;
		if(pDB->m_dT_n==0)		pDB->m_dT_n = 22;		
	}
}


void CDetVStiffJewonDlg::SetDataDefault()///초기에 한번만 실행...
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();		
	SetDataDefaultDepyoDanmyun();

	for(long i=0; i<pDB->GetGirdersu();i++)
	{
		CPlateGirderApp *pGir  = (CPlateGirderApp*)pDB->GetGirder(i);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		while(pBx)
		{
			double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			CSection *pSec    = pBx->GetSection();
			CVStiff  *pVStiff = pSec->GetVStiff();

			if(!pVStiff) 
			{
				pBx = (CPlateBasicIndex *)Finder.GetBxNext();
				continue;
			}

			RecommandData(pVStiff, dHGir);

			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}

	if (!pDB->IsPlateGir()) //소수주거더, TU거더교이면...
	{
		pDB->m_nInstallVStiff = 3;
	
		SetVStiffSetting();
		OnCheckVstiffArrangementSub(FALSE);	
	}
}

void CDetVStiffJewonDlg::SetDataSave()
{
	CPlateBridgeApp *pDB  = m_pStd->GetBridge();		
	CGlobarOption   *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	UpdateData();		
	for(long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		if(m_nCmpPos >= pGir->GetQtyVStiff()) continue;

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		if(pBx==NULL)	break;

		pBx = Finder.GetBxNext(m_nCmpPos);
		CSection *pSec    = pBx->GetSection();		
		CVStiff  *pVStiff = pSec->GetVStiff();

		pVStiff->m_pSection = pSec;	
		ASSERT(pVStiff);		
		if(pVStiff)
		{
			pVStiff->m_dWidth[0]		= m_Grid.GetTextMatrixDouble(nG + 1,1);
			pVStiff->m_dThick[0]		= m_Grid.GetTextMatrixDouble(nG + 1,2);
			pVStiff->m_dGap[0]			= m_Grid.GetTextMatrixDouble(nG + 1,3);
			pVStiff->m_dH[LEFT][DOWN]	= m_Grid.GetTextMatrixDouble(nG + 1,4);
			pVStiff->m_dH[LEFT][UP]		= pDB->IsTUGir() ? pOpt->GetInsulationVStiffUF() : 0;

			pVStiff->m_dWidth[1]		= m_Grid.GetTextMatrixDouble(nG + 1,5);
			pVStiff->m_dThick[1]		= m_Grid.GetTextMatrixDouble(nG + 1,6);
			pVStiff->m_dGap[1]			= m_Grid.GetTextMatrixDouble(nG + 1,7);
			pVStiff->m_dH[RIGHT][DOWN]	= m_Grid.GetTextMatrixDouble(nG + 1,8);
			pVStiff->m_dH[RIGHT][UP]	= pDB->IsTUGir() ? pOpt->GetInsulationVStiffUF() : 0;
			double dWL	= pVStiff->m_dWidth[0];
			DOUBLE dWR	= pVStiff->m_dWidth[1];

			if((dWL>0 && dWR>0) || (dWL==0 && dWR>0))
			{
				if(dWL>0 && dWR>0)
					pDB->GetGirder(nG)->m_nInstallVStiff = 0;
				else
					pDB->GetGirder(nG)->m_nInstallVStiff = 2;
			}
			else
				pDB->GetGirder(nG)->m_nInstallVStiff = 1;
		}

		// 수평브레이싱 연결판 
		//CHBracingGusset *pBrRu = pGir->GetHBracingGussetInTotal(m_nCmpPos);
		CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
		if(pBrRu)	
		{
			pBrRu->m_dT = atof(m_Grid.GetTextMatrix(nG + 1, 9));//"dTL"
			pBrRu->m_ddH = atof(m_Grid.GetTextMatrix(nG + 1, 10));//"dHL"
		}

	}
	UpdateData();

/*	if(m_bArr==FALSE)	//양측배치
		pDB->m_nInstallVStiff = 0;
	else
	{
		if(m_radioArr==0)
			pDB->m_nInstallVStiff = 1;
		else if(m_radioArr==1)
			pDB->m_nInstallVStiff = 2;
		else if(m_radioArr==2)
			pDB->m_nInstallVStiff = 3;
	}
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		if(pDB->m_nInstallVStiff==0)		// 양측
			pDB->GetGirder(nG)->m_nInstallVStiff = 0; 
		else if(pDB->m_nInstallVStiff==1)	// 좌측
			pDB->GetGirder(nG)->m_nInstallVStiff = 1; 
		else if(pDB->m_nInstallVStiff==2)	// 우측
			pDB->GetGirder(nG)->m_nInstallVStiff = 2; 
		else if(pDB->m_nInstallVStiff==3)	// 편측
		{
			if(nG<pDB->GetGirdersu()/2)	//우측
				pDB->GetGirder(nG)->m_nInstallVStiff = 2; 
			else
				pDB->GetGirder(nG)->m_nInstallVStiff = 1; 
		}
	}
	*/
}

void CDetVStiffJewonDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();	

	CPlateBridgeApp *pDB = m_pStd->GetBridge();	
	CPlateGirderApp *pGir    = m_pStd->GetBridge()->GetGirder(m_nG); 	
	CARoadOptionStd *pOptStd = m_pStd->m_pDataManage->GetOptionStd();
	CPlateGirderApp *pGirMatch = NULL;
    CPlateBasicIndex *pBxMatch = NULL;
	CAPlateDrawDanmyun DrawDan(m_pStd->GetDataManage());
	CAPlateDrawCross DrawCross(m_pStd->GetDataManage());	

	pDom->ClearEtt(TRUE);	
	// 현재 m_G거더의 m_nCmpPos위치의 수직보강재 BX
	CPlateBxFinder Finder(pDB->GetGirder(m_nG));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);
	long nCount=0;
	while(pBx)
	{
		if(m_nCmpPos == nCount) break;
		++nCount;		
		pBx = Finder.GetBxNext();
	}
	//
	CDomyun Dom(pDom);	
	if(m_nTab==0)
	{
		if(pBx) 
		{
			m_szStation.Format("%s", GetStationForStringOut(frM(pBx->GetStation()),3,TRUE,TRUE));
			UpdateData(FALSE);
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			DrawDan.DrawGirderOnBx(&Dom, pBx);		
			DrawDan.DrawVStiffOnBx(&Dom, pBx);	
			DrawDan.DimVStiffOnBx(&Dom, pBx);	

			Dom.SetScaleDim(20);
			CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
			if(pBrRu)
				DrawCross.DrawFrontHGussetSplice(&Dom, pBrRu->GetBx(), TRUE);
		}	
	}
	else if(m_nTab==1)	// 정모멘트부
	{
		if(pBx)
		{
			pOptStd->SetEnvType(&Dom, HCAD_STLC);
			DrawDan.DrawGuardWall(&Dom, pBx);	// 방호벽
			DrawDan.DrawSlabUpper(&Dom, pBx);
			DrawDan.DrawSlabHunch(&Dom, pBx);
			for(long nG=0; nG<pDB->GetGirdersu();nG++)
			{
				pGirMatch = pDB->GetGirder(nG);	
				
				if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) || pBx->GetSection()->IsenDetType(SECDET_VBRACING))
					pBxMatch = pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirMatch);
				else
					pBxMatch = pBx->GetGirder()->GetBxMatchByVStiff(pBx, pGirMatch);

				if(pBxMatch)
				{
					DrawDan.DrawGirderOnBx(&Dom, pBxMatch);		
					DrawDan.DrawVStiffOnBx(&Dom, pBxMatch);	
					DrawDan.DimVStiffOnBx(&Dom, pBx);
					if(pDB->IsTUGir())
						DrawDan.DrawGirderBindConc(&Dom, pBxMatch);
				}
			}
		}
	}
	else if(m_nTab==2)	// 부모멘트부
	{
		if(pDB->m_nQtyJigan > 1)	// 2경간 이상시만...
		{
			pBx = Finder.GetBxFirst(BX_VSTIFF, pGir->GetBxOnJijum(1), NULL);
			if(pBx)
			{
				pOptStd->SetEnvType(&Dom, HCAD_STLC);
				DrawDan.DrawGuardWall(&Dom, pBx);	// 방호벽
				DrawDan.DrawSlabUpper(&Dom, pBx);
				DrawDan.DrawSlabHunch(&Dom, pBx);
				for(long nG=0; nG<pDB->GetGirdersu();nG++)
				{
					pGirMatch = pDB->GetGirder(nG);	
					
					if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) || pBx->GetSection()->IsenDetType(SECDET_VBRACING))
						pBxMatch = pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirMatch);
					else
						pBxMatch = pBx->GetGirder()->GetBxMatchByVStiff(pBx, pGirMatch);

					if(pBxMatch)
					{
						DrawDan.DrawGirderOnBx(&Dom, pBxMatch);		
						DrawDan.DrawVStiffOnBx(&Dom, pBxMatch);	
						DrawDan.DimVStiffOnBx(&Dom, pBx);	
						if(pDB->IsTUGir())
							DrawDan.DrawGirderBindConc(&Dom, pBxMatch);
					}
				}
			}
		}
	}

	*pDom << Dom;
	
	if(m_bTotalView)
	{
		m_pStd->GetTotalView()->m_nFlag = BX_VSTIFF;

		CDomyun Dom;
		CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
		CDomyun *pDomDlg = m_pStd->GetTotalView()->m_DomView.GetDomyun();
		m_pStd->GetTotalView()->Clear();
		DrawPyung.DrawPyungOnAirialView(&Dom);
		DrawPyung.DrawPyungVStiffPosMark(&Dom, m_nG, m_nCmpPos);
		Dom.Rotate(CDPoint (0,0), pDB->GetAngleBridgeCenter().GetMirrorVert());
		if(m_pStd->GetTotalView()->GetSafeHwnd()) m_pStd->GetTotalView()->m_DomView.RedrawAll();
		*pDomDlg << Dom;

		m_pStd->ShowTotalView(m_pView);
	}	
	else
		m_pStd->HideTotalView();

	OnCursorChange(1,1);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CDetVStiffJewonDlg::OnCursorChange(long nRow, long nCol)
{
	CGeneralBaseDlg::OnCursorChange(nRow, nCol);

	m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,nCol));
}
// Grid 설정
void CDetVStiffJewonDlg::SetGridTitle()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();		
	char *sTitle[] = { "LW", "LT", "LG", "LH",
		               "RW", "RT", "RG", "RH",
					   "vT","vH"};// 수평브레이싱 연결판 		


	long nRows = pDB->GetGirdersu()+1;
	long nCols = 11;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(50);		
	m_Grid.SetColumnWidth(0, 65);

	CString str;
	m_Grid.SetTextMatrix(0,0,"구 분");

	long nG=0;
	for(long nRow=1; nRow < pDB->GetGirdersu()+1; nRow++)
	{
		str.Format("G%d",nRow);
		m_Grid.SetTextMatrix(nG+1, 0, str);		
		nG++;
	}
	for(long nCol = 1; nCol < nCols; nCol++)
	{		
		m_Grid.SetTextMatrix(0,nCol,sTitle[nCol-1]);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CDetVStiffJewonDlg::OnSelchangeComboPosition() 
{
	m_nCmpPos = m_ctlComboType.GetCurSel();
	SetDataInit();
	SetDataSave();

	SetControl();

	DrawInputDomyunView(TRUE);
	m_ctlComboType.SetFocus();
}

void CDetVStiffJewonDlg::OnVstiffInputcomplete() 
{
	SetDataSave();
	m_pStd->MoveNextDialog();	
}

void CDetVStiffJewonDlg::OnButtonAutocalc() 
{	
/*	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	UpdateData();		

	for(long i=0; i<pDB->GetGirdersu();i++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(i);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			ASSERT(pSec);
			if(pSec->IsenDetType(SECDET_VSITFF))
			{
				CVStiff *pVStiff = pSec->GetVStiff();	
				if(pGir->GetLine()->GetTypeLine(pBx->GetStation()) == 0 && !pBx->IsState(BX_CROSSBEAM_VBRACING))
				{
					if(pVStiff->m_dWidth[0]!=0)	pVStiff->m_dH[LEFT][UP] = pVStiff->m_dH[LEFT][UP]-60;
					if(pVStiff->m_dWidth[1]!=0)	pVStiff->m_dH[RIGHT][UP] = pVStiff->m_dH[RIGHT][UP]-60;
				}
				else
				{
					if(pVStiff->m_dWidth[0]!=0)	pVStiff->m_dH[LEFT][UP] = pVStiff->m_dH[LEFT][UP]; 
					if(pVStiff->m_dWidth[1]!=0)	pVStiff->m_dH[RIGHT][UP] = pVStiff->m_dH[RIGHT][UP]; 			
				}
			}
			pBx = Finder.GetBxNext();
		}
	}	

	SetDataInit();
*/
}

void CDetVStiffJewonDlg::OnCheckTotalview() 
{
	UpdateData();
	CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_TOTALVIEW);
	CAPlateDrawPyung DrawPyung(m_pStd->GetDataManage());
	BOOL bTotalView = (pButton) ? pButton->GetCheck() : FALSE;
	if(bTotalView)
	{
		m_pStd->GetTotalView()->m_nFlag = BX_CROSSBEAM_VBRACING;
		CDomyun *pDomDlg = m_pStd->GetTotalView()->m_DomView.GetDomyun();
		pDomDlg->SetCWnd(m_pView->GetDomyun()->GetCWnd());
		m_pStd->GetTotalView()->Clear();
		DrawPyung.DrawPyungOnAirialView(pDomDlg);
		m_pStd->ShowTotalView(m_pView);
	}
	else
		m_pStd->HideTotalView();
	DrawInputDomyunView(TRUE);
}


void CDetVStiffJewonDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	SetDataSave();
	SetDataInit();

	DrawInputDomyunView(TRUE);
}

void CDetVStiffJewonDlg::SetControl()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	if(m_nG < 0 || m_nG >= pDB->GetGirdersu()) return;

	CPlateGirderApp *pGir = pDB->GetGirder(m_nG);
	CVStiff *pVStiff = pGir->GetVStiffByPos(m_nCmpPos);
	if(pVStiff == NULL) return;

	m_cCarryUpper = pVStiff->m_cCarryUpper != 0;
	m_cCarryLower = pVStiff->m_cCarryLower != 0;
}

void CDetVStiffJewonDlg::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;
	int nRow  = GridView->iRow;    
	m_nG      = nRow-1;

	SetControl();
	
	DrawInputDomyunView(TRUE);
}

// 대표단면의 값을 이용하여 수직보강재 전부를 설정함
// 기본적으로 양측기준임.
void CDetVStiffJewonDlg::SetStandardInput()
{
	UpdateData();	

	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	for(long i=0; i<pDB->GetGirdersu();i++)
	{
		CPlateGirderApp  *pGir = (CPlateGirderApp*)pDB->GetGirder(i);
		CPlateBxFinder	 Finder(pGir);
		CPlateBasicIndex *pBx  = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		CGlobarOption    *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
		while(pBx)
		{
			CVStiff  *pVStiff = pBx->GetSection()->GetVStiff();
			BOOL   bIsPositive = IsPositiveRange(pBx);

			if(!pVStiff) 
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			pVStiff->m_dH[LEFT][UP] = pDB->IsTUGir()&&!pBx->IsJijum() ?  pOpt->GetInsulationVStiffUF() : 0;
			pVStiff->m_dH[LEFT][DOWN] = bIsPositive ? pDB->m_dA_p : 0;
			pVStiff->m_dH[RIGHT][UP] = pDB->IsTUGir()&&!pBx->IsJijum() ? pOpt->GetInsulationVStiffUF() : 0;
			pVStiff->m_dH[RIGHT][DOWN] = bIsPositive ? pDB->m_dA_p : 0;
			pVStiff->m_dWidth[0]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
			pVStiff->m_dThick[0]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
			pVStiff->m_dWidth[1]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
			pVStiff->m_dThick[1]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
			pVStiff->m_dGap[0] =  pDB->m_dB_p;
			pVStiff->m_dGap[1] =  pDB->m_dB_p;

			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}
}

void CDetVStiffJewonDlg::OnCheckVstiffArrangement() 
{
	if(m_checkArr.GetCheck())
	{		
		m_radioArr = 2;
		
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SYMMETRY)->EnableWindow(TRUE);
		m_bArr = TRUE;
	}
	else
	{
		GetDlgItem(IDC_RADIO_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SYMMETRY)->EnableWindow(FALSE);
		m_bArr = FALSE;
	}

	UpdateData(FALSE);
	OnCheckVstiffArrangementSub();
}

void CDetVStiffJewonDlg::OnCheckVstiffArrangementSub(BOOL bMode)
{	
	if(m_bArr == FALSE)	//양측배치를 할경우
	{
		CPlateBridgeApp *pDB = m_pStd->GetBridge();

		for(long i=0; i<pDB->GetGirdersu();i++)
		{
			CPlateGirderApp  *pGir = (CPlateGirderApp*)pDB->GetGirder(i);
			CPlateBxFinder	 Finder(pGir);
			CPlateBasicIndex *pBx  = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
			CGlobarOption    *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
			while(pBx)
			{
				CVStiff  *pVStiff = pBx->GetSection()->GetVStiff();				
				BOOL   bIsPositive = IsPositiveRange(pBx);

				if(!pVStiff) 
				{
					pBx = Finder.GetBxNext();
					continue;
				}
				pVStiff->m_dH[LEFT][UP] =  pDB->IsTUGir()&&!pBx->IsJijum() ? pOpt->GetInsulationVStiffUF() : 0;// 좌측상단
				pVStiff->m_dH[LEFT][DOWN]= bIsPositive ? pDB->m_dA_p : 0;
				pVStiff->m_dH[RIGHT][UP] =  pDB->IsTUGir()&&!pBx->IsJijum() ? pOpt->GetInsulationVStiffUF() : 0;
				pVStiff->m_dH[RIGHT][DOWN] = bIsPositive ? pDB->m_dA_p : 0;
				pVStiff->m_dWidth[0]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
				pVStiff->m_dThick[0]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
				pVStiff->m_dWidth[1]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
				pVStiff->m_dThick[1]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
				pVStiff->m_dGap[0]  = pDB->m_dB_p;
				pVStiff->m_dGap[1]  = pDB->m_dB_p;

				pBx = (CPlateBasicIndex *)Finder.GetBxNext();
			}
		}	

		if (bMode)
		{
			SetDataInit();
			SetDataSave();
			DrawInputDomyunView(TRUE);
		}		
	}
	else
	{
		if(m_radioArr==0)
			OnRadioLeftOnlyVStiff();
		else if(m_radioArr==1)
			OnRadioRightOnlyVStiff();
		else
			OnRadioSymmetryOnlyVStiff();
	}
	m_dRH = 0;
	m_dLH = 0;
}

void CDetVStiffJewonDlg::OnButtonInputStandardType() 
{
	CPlateBridgeApp		*pDB = m_pStd->GetBridge();
	CVStiffStdInputDlg	StdDlg;

	StdDlg.m_pStd	= m_pStd;
	StdDlg.m_nG		= m_nG;

	if(StdDlg.DoModal() == IDOK)
	{
		if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
					  "대표단면 입력", MB_ICONQUESTION | MB_YESNO) == IDNO)
			return;

		SetStandardInput();
	}

	if (pDB->IsPlateGir())//판형교이면..
		pDB->m_nInstallVStiff = 0;
	else//소수주거더교이면...
		pDB->m_nInstallVStiff = 3;
	
	SetVStiffSetting();
	OnCheckVstiffArrangementSub(FALSE);		

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(TRUE);

}

// 수평보강재 배치범위 중 하부배치범위를 기준으로 일반부와 부모멘트부를 구분함
// 이유 : 하부배치범위가 교번구간을 부모멘트범위로 포함하고 있음.
// return TRUE : 일반부에 배치된 수직보강재임
// return FALSE: 부모멘트부에 배치된 수직보강재임
BOOL CDetVStiffJewonDlg::IsPositiveRange(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir    = pBx->GetGirder();

	long nRangeUp = pGir->m_pRangePress->m_ArrayUpper.GetSize();
	double dStaStt = pGir->GetSSS(OFF_STT_GIRDER);
	double dRangeStt=dStaStt, dRangeEnd=0;
	for(long n=0; n<nRangeUp; n++)
	{
		dRangeEnd = dRangeStt+pGir->m_pRangePress->m_ArrayLower.GetAt(n)->m_dDistRange;
		if(dRangeStt < pBx->GetStation() && pBx->GetStation() < dRangeEnd)
			return !pGir->m_pRangePress->m_ArrayLower.GetAt(n)->m_bCompress;

		dRangeStt = dRangeEnd; 
	}
	return TRUE;
}

void CDetVStiffJewonDlg::OnRadioLeftOnlyVStiff() 
{
	UpdateData();	

	CPlateBridgeApp  *pDB  = m_pStd->GetBridge();
	CGlobarOption    *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	for(long i=0; i<pDB->GetGirdersu();i++)
	{
		CPlateGirderApp *pGir  = (CPlateGirderApp*)pDB->GetGirder(i);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		while(pBx)
		{
			CVStiff  *pVStiff = pBx->GetSection()->GetVStiff();			
			double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			BOOL   bIsPositive = IsPositiveRange(pBx);

			if(!pVStiff) 
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			
			pVStiff->m_dH[LEFT][UP] = pDB->IsTUGir()&&!pBx->IsJijum() ? pOpt->GetInsulationVStiffUF() : 0;
			pVStiff->m_dH[LEFT][DOWN] = bIsPositive ? pDB->m_dA_p : 0;
			pVStiff->m_dH[RIGHT][UP] = dHGir;
			pVStiff->m_dH[RIGHT][DOWN] = 0;

			pVStiff->m_dWidth[0]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
			pVStiff->m_dThick[0]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
			pVStiff->m_dWidth[1]= 0;
			pVStiff->m_dThick[1]= 0;
			pVStiff->m_dGap[0]  = pDB->m_dB_p;
			pVStiff->m_dGap[1]  = 0;

			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}

	m_dRH = 0;
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetVStiffJewonDlg::OnRadioRightOnlyVStiff() 
{
	UpdateData();	

	CPlateBridgeApp  *pDB  = m_pStd->GetBridge();
	CGlobarOption    *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	for(long i=0; i<pDB->GetGirdersu();i++)
	{
		CPlateGirderApp *pGir  = (CPlateGirderApp*)pDB->GetGirder(i);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		while(pBx)
		{
			CVStiff  *pVStiff = pBx->GetSection()->GetVStiff();
			double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			BOOL   bIsPositive = IsPositiveRange(pBx);

			if(!pVStiff) 
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			pVStiff->m_dH[LEFT][UP] = dHGir;
			pVStiff->m_dH[LEFT][DOWN] = 0;
			pVStiff->m_dH[RIGHT][UP] =  pDB->IsTUGir()&&!pBx->IsJijum() ? pOpt->GetInsulationVStiffUF() : 0;
			pVStiff->m_dH[RIGHT][DOWN] = bIsPositive ? pDB->m_dA_p : 0;
			pVStiff->m_dWidth[0]= 0;
			pVStiff->m_dThick[0]= 0;
			pVStiff->m_dWidth[1]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
			pVStiff->m_dThick[1]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
			pVStiff->m_dGap[0]  = 0;
			pVStiff->m_dGap[1]  = pDB->m_dB_p;

			pBx = (CPlateBasicIndex *)Finder.GetBxNext();
		}
	}
	m_dLH = 0;
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

void CDetVStiffJewonDlg::OnRadioSymmetryOnlyVStiff() 
{
	UpdateData();	

	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CGlobarOption    *pOpt = m_pStd->GetDataManage()->GetGlobalOption();
	for(long i=0; i<pDB->GetGirdersu();i++)
	{
		CPlateGirderApp *pGir  = (CPlateGirderApp*)pDB->GetGirder(i);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
		
		
		if(i<pDB->GetGirdersu()/2)		// 교량중심 좌측방향거더들(우측배치)-거더수가 홀수개 일 경우는 중앙거더는 우측으로 포함됨.
		{
			while(pBx)
			{
				CVStiff  *pVStiff = pBx->GetSection()->GetVStiff();
				double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
				BOOL   bIsPositive = IsPositiveRange(pBx);

				if(!pVStiff) 
				{
					pBx = Finder.GetBxNext();
					continue;
				}
				pVStiff->m_dH[LEFT][UP] = dHGir;
				pVStiff->m_dH[LEFT][DOWN] = 0;
				pVStiff->m_dH[RIGHT][UP] =  pDB->IsTUGir()&&!pBx->IsJijum() ? pOpt->GetInsulationVStiffUF() : 0;
				pVStiff->m_dH[RIGHT][DOWN] = bIsPositive ? pDB->m_dA_p : 0;
				pVStiff->m_dWidth[0]= 0;
				pVStiff->m_dThick[0]= 0;
				pVStiff->m_dWidth[1]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
				pVStiff->m_dThick[1]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
				pVStiff->m_dGap[0]  = 0;
				pVStiff->m_dGap[1]  = pDB->m_dB_p;

				pBx = (CPlateBasicIndex *)Finder.GetBxNext();
			}
		}
		else	// 우측거더(좌측에 수직보강재 배치)
		{
			while(pBx)
			{
				CVStiff  *pVStiff = pBx->GetSection()->GetVStiff();
				double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
				BOOL   bIsPositive = IsPositiveRange(pBx);

				if(!pVStiff) 
				{
					pBx = Finder.GetBxNext();
					continue;
				}
				pVStiff->m_dH[LEFT][UP] = pDB->IsTUGir()&&!pBx->IsJijum() ? pOpt->GetInsulationVStiffUF() : 0;
				pVStiff->m_dH[LEFT][DOWN] = bIsPositive ? pDB->m_dA_p : 0;
				pVStiff->m_dH[RIGHT][UP] =  dHGir;
				pVStiff->m_dH[RIGHT][DOWN] = 0;
				pVStiff->m_dWidth[0]= bIsPositive ? pDB->m_dC_p : pDB->m_dC_n;
				pVStiff->m_dThick[0]= bIsPositive ? pDB->m_dT_p : pDB->m_dT_n;
				pVStiff->m_dWidth[1]= 0;
				pVStiff->m_dThick[1]= 0;
				pVStiff->m_dGap[0]  = pDB->m_dB_p;
				pVStiff->m_dGap[1]  = 0;

				pBx = (CPlateBasicIndex *)Finder.GetBxNext();
			}
		}
	}
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView(TRUE);
}

long CDetVStiffJewonDlg::OnClickTotalView(WPARAM wp, LPARAM lp) 
{
	CPlateBasicIndex *pBx  = (CPlateBasicIndex *)wp;
	CPlateGirderApp  *pGir = pBx->GetGirder();
	SetDataSave();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxComp = Finder.GetBxFirst(BX_VSTIFF);
	long nVStiff = 0;
	while(pBxComp)
	{
		if(pBx==pBxComp)
		{
			m_nCmpPos = nVStiff;
			break;
		}
		
		nVStiff++;
		pBxComp = Finder.GetBxNext();
	}
	UpdateData(FALSE);

	SetDataInit();
	m_Grid.ResetSelection();
	m_Grid.SetFocusCell(pBx->GetNumberGirder()+1, 1);
	DrawInputDomyunView(TRUE);

	return 0;
}

void CDetVStiffJewonDlg::OnCheckDongbaliUp() 
{
	UpdateData(TRUE);

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);

	CVStiff *pVStiff = pGir->GetVStiffByPos(m_nCmpPos);
	pVStiff->m_cCarryUpper = m_cCarryUpper;	

	DrawInputDomyunView(TRUE);
}

void CDetVStiffJewonDlg::OnCheckDongbaliDown() 
{
	UpdateData(TRUE);

	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(m_nG);

	CVStiff *pVStiff = pGir->GetVStiffByPos(m_nCmpPos);
	pVStiff->m_cCarryLower= m_cCarryLower;		

	DrawInputDomyunView(TRUE);
}

void CDetVStiffJewonDlg::OnButtonDesignCheck() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}

	CPlateStressOut	pStressOut(m_pStd);

	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(m_nG);
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex	*pBx		= (CPlateBasicIndex *)Finder.GetBxFirst(BX_VSTIFF);
	pBx = (CPlateBasicIndex *)Finder.GetBxNext(m_nCmpPos);

	CStringArray Arr;
	pStressOut.CheckVStiff(Arr, pBx);
	m_pStd->OutputViewOnAView(Arr);
}

CString CDetVStiffJewonDlg::GetTabTitle(long nIdx) 
{
	if(nIdx==0)
		return "입력";
	else if(nIdx==1)
		return "횡단면도";
	//else if(nIdx==2)
		//return "횡단면도(부)";

	return "";
}

BOOL CDetVStiffJewonDlg::IsValid()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CGlobarOption   *pOpt    = m_pStd->GetDataManage()->GetGlobalOption();

	if(pOpt->GetDesignVStiffInstall()==0)
	{
		for(long i=0; i<pDB->GetGirdersu();i++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(i);
			if(pGir->GetQtyVStiff() > 0) 
				return TRUE;
		}
	}
	
	return FALSE;
}

// 모든 거더 동일적용
void CDetVStiffJewonDlg::OnButtonInputAllgirsame() 
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까",
		"수직보강재 상세 입력 기본값", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;
	
	SetDataAllGirderSame();
	SetDataInit();
	DrawInputDomyunView(TRUE);			
	
}

// 모든 수직보강재 동일적용
void CDetVStiffJewonDlg::OnButtonInputAllsame() 
{
	if(MessageBox("기존의 입력한 데이터가 모두 지워집니다.\n 계속 하시겠습니까", 
		"수직보강재 상세 입력 기본값", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	SetDataAllSame();

	SetDataInit();
	DrawInputDomyunView(TRUE);		
	
}

void CDetVStiffJewonDlg::SetDataAllGirderSame()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir  = pDB->GetGirder(m_nG);
	CPlateBxFinder FinderStd(pGir);
	CPlateBasicIndex *pBxStd = FinderStd.GetBxFirst(BX_VSTIFF);
	pBxStd = FinderStd.GetBxNext(m_nCmpPos);
	CVStiff *pVStiffStd = pBxStd->GetSection()->GetVStiff();
	CHBracingGusset* pBrRuStd = pBxStd->GetHBracingGusset();

	for(long nG = 0; nG < pDB->GetGirdersu(); nG++) 
	{
		pGir = pDB->GetGirder(nG);
		if(m_nCmpPos >= pGir->GetQtyVStiff()) continue;

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);
		pBx = Finder.GetBxNext(m_nCmpPos);
		CVStiff *pVStiff = pBx->GetSection()->GetVStiff();

		*pVStiff = *pVStiffStd;  

		// 수평브레이싱 연결판 
		CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
		if(pBrRuStd && pBrRu)	
		{
			pBrRu->m_dT = pBrRuStd->m_dT;//"dTL"
			pBrRu->m_ddH = pBrRuStd->m_ddH;//"dHL"
		}

	}
}

void CDetVStiffJewonDlg::SetDataAllSame()
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir  = pDB->GetGirder(m_nG);
	CPlateBxFinder FinderStd(pGir);
	CPlateBasicIndex *pBxStd = FinderStd.GetBxFirst(BX_VSTIFF);
	pBxStd = FinderStd.GetBxNext(m_nCmpPos);
	CVStiff *pVStiffStd = pBxStd->GetSection()->GetVStiff();
	CHBracingGusset* pBrRuStd = pBxStd->GetHBracingGusset();


	for(long nG = 0; nG < pDB->GetGirdersu(); nG++) 
	{
		pGir = pDB->GetGirder(nG);
		if(m_nCmpPos >= pGir->GetQtyVStiff()) continue;

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);
		while(pBx)
		{
			if(pBx->GetSection())
			{
				CVStiff *pVStiff = pBx->GetSection()->GetVStiff();
				if(pVStiff)
				*pVStiff = *pVStiffStd;  

					// 수평브레이싱 연결판 
					CHBracingGusset* pBrRu = pBx->GetHBracingGusset();
					if(pBrRuStd && pBrRu)	
					{
						pBrRu->m_dT = pBrRuStd->m_dT;//"dTL"
						pBrRu->m_ddH = pBrRuStd->m_ddH;//"dHL"
					}
			}
			pBx = Finder.GetBxNext();
		}
	}
}

//현재 선택된 Row의 값만 권고안값으로 바꾼다...
void CDetVStiffJewonDlg::OnButtonRecommend() 
{	
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir  = pDB->GetGirder(m_nG);
	CPlateBxFinder FinderStd(pGir);
	CPlateBasicIndex *pBxStd = FinderStd.GetBxFirst(BX_VSTIFF);
	pBxStd = FinderStd.GetBxNext(m_nCmpPos);
	CVStiff *pVStiff = pBxStd->GetSection()->GetVStiff();
	CHBracingGusset* pBrRuStd = pBxStd->GetHBracingGusset();
	
	// 현재 m_G거더의 m_nCmpPos위치의 수직보강재 BX
	CPlateBxFinder Finder(pDB->GetGirder(m_nG));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF);
	long nCount=0;
	while(pBx)
	{
		if(m_nCmpPos == nCount) break;
		++nCount;		
		pBx = Finder.GetBxNext();
	}

	double dHGir = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	
	RecommandData(pVStiff, dHGir);

	if (!pDB->IsPlateGir()) //소수주거더교, TU거더교
	{
		pDB->m_nInstallVStiff = 3;

		SetVStiffSetting();
		OnCheckVstiffArrangementSub(FALSE);	
	}	
	
	SetDataInit();
	DrawInputDomyunView(TRUE);	
}

void CDetVStiffJewonDlg::RecommandData(CVStiff *pVStiff, double dHGir)
{
	CPlateBridgeApp *pDB = m_pStd->GetBridge();

	pVStiff->m_dH[LEFT][UP] = 0;
	pVStiff->m_dH[LEFT][DOWN] = 0;
	pVStiff->m_dH[RIGHT][UP] = 0;
	pVStiff->m_dH[RIGHT][DOWN] = 0;

	if(pDB->IsPlateGir())
	{
		// 수직보강재의 돌출폭은 복부판 높이의 1/30에 50mm더한 값보다 크게 잡고, 
		// 보강재의 두께는 돌출폭의 1/13이싱이어야 한다.  -강도로교 상세부 설계지침 p28
		pVStiff->m_dWidth[0]= RoundUp(dHGir/30 + 50, -1);									  
		pVStiff->m_dWidth[1]= RoundUp(dHGir/30 + 50, -1);
		pVStiff->m_dThick[0]= RoundUp(pVStiff->m_dWidth[0]/13, 0);
		pVStiff->m_dThick[1]= RoundUp(pVStiff->m_dWidth[1]/13, 0);
		pVStiff->m_dGap[0]  = 20;
		pVStiff->m_dGap[1]  = 20;
	}
	else//소수주형교이면...
	{
		pVStiff->m_dWidth[0]= 250;
		pVStiff->m_dWidth[1]= 250;
		pVStiff->m_dThick[0]= 22;
		pVStiff->m_dThick[1]= 22;
		pVStiff->m_dGap[0]  = 20;
		pVStiff->m_dGap[1]  = 20;
	}
}

long CDetVStiffJewonDlg::GetTabCount()
{
	return 2;
	//CPlateBridgeApp *pDB = m_pStd->GetBridge();

	//if(pDB->m_nQtyJigan > 1)	return 3;
	//else						return 2;
}

BOOL CDetVStiffJewonDlg::OnInitDialog() 
{
	CGeneralBaseDlg::OnInitDialog();
	
	SetResize(IDC_BUTTON_DESIGN_CHECK, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_AUTOCALC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_INPUT_BASICTYPE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_INPUT_ALLGIRSAME, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_INPUT_ALLSAME, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_VSTIFF_INPUTCOMPLETE, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_CHECK_TOTALVIEW, SZ_TOP_CENTER, SZ_TOP_CENTER);
	
	SetResize(IDC_STATIC_GROUP, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_LEFT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_RIGHT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_SYMMETRY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	SetResize(IDC_STATIC_GROUP5, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VSTIFF_ARRANGEMENT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_DONGBALI_UP, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_DONGBALI_DOWN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC1, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	return TRUE;
}
