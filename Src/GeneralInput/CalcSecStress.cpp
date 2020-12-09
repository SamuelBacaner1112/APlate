// CalcSecStress.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../PlateGangje\Gangje.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcSecStress dialog


CCalcSecStress::CCalcSecStress(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcSecStress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcSecStress)
	m_bBuckling			= FALSE;
	m_bRenderingStress	= TRUE;
	m_nViewStress		= 0;
	//}}AFX_DATA_INIT
	m_bGL = TRUE;
}


void CCalcSecStress::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcSecStress)
	DDX_Control(pDX, IDC_BUTTON_CALCSTRESS, m_btnCalcSec);
	DDX_Check(pDX, IDC_CHECK_BUCKLING, m_bBuckling);
	DDX_Check(pDX, IDC_CHECK_VIEW_STRESS, m_bRenderingStress);
	DDX_Radio(pDX, IDC_RADIO_UPPER_STRESS, m_nViewStress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCalcSecStress, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcSecStress)
	ON_BN_CLICKED(IDC_RADIO_DAN_STRESS, OnRadioDanStress)
	ON_BN_CLICKED(IDC_RADIO_LOWER_COMPSTRESS, OnRadioLowerCompStress)
	ON_BN_CLICKED(IDC_RADIO_LOWER_STRESS, OnRadioLowerStress)
	ON_BN_CLICKED(IDC_RADIO_SHEARSTRESS, OnRadioShearStress)
	ON_BN_CLICKED(IDC_RADIO_UPPER_COMPSTRESS, OnRadioUpperCompStress)
	ON_BN_CLICKED(IDC_RADIO_UPPER_STRESS, OnRadioUpperStress)
	ON_BN_CLICKED(IDC_BUTTON_CALCSTRESS, OnButtonCalcStress)
	ON_BN_CLICKED(IDC_CHECK_VIEW_STRESS, OnCheckViewStress)
	ON_BN_CLICKED(IDC_CHECK_BUCKLING, OnCheckBuckling)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcSecStress message handlers
void CCalcSecStress::OnPreInitDialog()
{
	m_nViewStress = 0;

	UpdateData(TRUE);

	if(m_pStd->GetBridge()->IsTUGir())	GetDlgItem(IDC_CHECK_BUCKLING)->ShowWindow(SW_SHOW);
	else								GetDlgItem(IDC_CHECK_BUCKLING)->ShowWindow(SW_HIDE);
	
	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_CHECK_VIEW_STRESS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_GROUPBOX, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_UPPER_STRESS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_UPPER_COMPSTRESS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_SHEARSTRESS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_LOWER_STRESS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_LOWER_COMPSTRESS, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_DAN_STRESS, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_CALCSTRESS, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);	
	SetResize(IDC_CHECK_BUCKLING, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	MakeSectionStressFrame();

	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
	m_pStd->m_FEMDraw.SetViewForceType(0);
}

void CCalcSecStress::MakeSectionStressFrame()
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), pCalcStd);

	if(pCalcStd->GetSizeSectionStressData()==0)
	{
		pCalcStd->RemoveSectionStressData();
		pCalcStd->MakeSectionStressData(1);
		CFEMManage	*pFEM	= pCalcStd->GetSectionStressData(SECTION_STRESS);

		pFEM->SYSTEM_VAR.m_szUnitType = "KN";
		pFEM->SYSTEM_VAR.m_UX = pFEM->SYSTEM_VAR.m_UY = pFEM->SYSTEM_VAR.m_UZ = 0;
		pFEM->SYSTEM_VAR.m_RX = pFEM->SYSTEM_VAR.m_RY = pFEM->SYSTEM_VAR.m_RZ = 0;	
		pFEM->m_szLCName.RemoveAll();
		pFEM->m_szTitle.RemoveAll();
		ModelCalc.MakeJoint(pFEM);
		ModelCalc.MakeElement(pFEM);
	}
}

void CCalcSecStress::SetDataInit()
{
	CCalcData  *pCalcData = m_pStd->GetDataManage()->GetCalcData();
	m_bBuckling	= pCalcData->m_bBucklingStress;
	UpdateData(FALSE);
}

void CCalcSecStress::SetDataSave()
{
	CCalcData  *pCalcData = m_pStd->GetDataManage()->GetCalcData();
	if(m_bBuckling)	pCalcData->m_bBucklingStress = TRUE;
	else			pCalcData->m_bBucklingStress = FALSE;
}

void CCalcSecStress::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun		*pDom	= m_pView->GetDomyun();
	m_pView->Clear();
	pDom->ClearEtt(TRUE);

	if(m_pStd->m_pCalcStd->GetSizeSectionStressData()>0)
	{
		CFEMManage	*pFEM	= m_pStd->m_pCalcStd->GetSectionStressData(SECTION_STRESS);

		m_pStd->m_FEMDraw.ChangeFEM(pFEM);
		m_pStd->m_FEMDraw.InsertDomyunPoint();
		m_pStd->m_FEMDraw.SetLegendPow(FALSE);
		m_pStd->m_FEMDraw.SetRestraintScale(1);
		m_pStd->m_FEMDraw.SetJointSize(0.2);
		m_pStd->m_FEMDraw.SetGraientCountor(TRUE);

		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_NUMBER, TRUE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_NUMBER, FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_RENDER_RESTRAINT, FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE, FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_LOADCASE, FALSE);
		m_pStd->m_FEMDraw.SetLoadCase(0);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_DISPLACEMENT,FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_DISPLACEMENT,FALSE);	
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_REACTION,FALSE);				
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE,TRUE);
		m_pStd->m_FEMDraw.SetViewForceType(m_nViewStress);
		m_pView->SetAxis('d');
	}
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

///단면저항모멘트
void CCalcSecStress::OnRadioDanStress() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

/// 하연합성응력
void CCalcSecStress::OnRadioLowerCompStress() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

///하연응력	
void CCalcSecStress::OnRadioLowerStress() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}
///전단응력
void CCalcSecStress::OnRadioShearStress() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

///상연합성응력
void CCalcSecStress::OnRadioUpperCompStress() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}
/// 상연응력
void CCalcSecStress::OnRadioUpperStress() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

void CCalcSecStress::OnCheckViewStress() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

void CCalcSecStress::SetDataDefault()
{
	MakeSectionStressFrame();
}

void CCalcSecStress::OnButtonCalcStress() 
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	CTStandard		*pTS		= m_pStd->GetDataManage()->GetBMStandard()->m_pTStandard;
	
	CRect rect = CRect(200, 3, 400, 18);
	if(!m_pStd->m_ctlProgress.GetSafeHwnd())
		m_pStd->m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_pStd->m_ctlProgress.ShowWindow(SW_SHOW);	
	pTS->SetPaneText(0, "단면응력 계산중...", TRUE);
	m_pStd->m_ctlProgress.SetPos(0);

	if(pCalcStd->GetSizeModelingResult()==0)
	{
		AfxMessageBox("모델링 결과값을 읽지 않았습니다.\n[모델링결과]다이얼로그에서 SAP자동실행이나 파일읽기를 하십시오");
		return;
	}

	pCalcStd->RemoveSectionStressData();
	pCalcStd->MakeSectionStressData(1);

	CFEMManage		*pFEM	= pCalcStd->GetSectionStressData(SECTION_STRESS);
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();

	double dStressRatio=0, dSigma_up=0, dSigma_lo=0, dComp_up=0, dComp_lo=0, dTau=0, dValue=0;
	long   nStt=0, nEnd=0;
	long	nLoadCase	= 0;
	CString szTemp="";

	pFEM->SYSTEM_VAR.m_szUnitType = "KN";
	pFEM->SYSTEM_VAR.m_UX = pFEM->SYSTEM_VAR.m_UY = pFEM->SYSTEM_VAR.m_UZ = 0;
	pFEM->SYSTEM_VAR.m_RX = pFEM->SYSTEM_VAR.m_RY = pFEM->SYSTEM_VAR.m_RZ = 0;	
	pFEM->m_szLCName.RemoveAll();
	pFEM->m_szTitle.RemoveAll();
	ModelCalc.MakeJoint(pFEM);
	ModelCalc.MakeElement(pFEM);
	pFEM->MakeElementLoadForce(nLoadCase+1);
	pFEM->m_szLCName.SetAt(nLoadCase, "STRESS");	// 상연응력

	ModelCalc.GetElemRangeByGirder(pDB->GetGirdersu()-1, nStt, nEnd); 
	m_pStd->m_ctlProgress.SetRange(0, (short)nEnd);
	m_pStd->m_ctlProgress.SetStep(0);
	m_pStd->m_ctlProgress.ShowWindow(TRUE);
	
	UpdateData(TRUE);

	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	long	nElem	= 0;
	while(pos)
	{
		CElement	*pElm	= (CElement*)pFEM->m_ElementList.GetNext(pos);
		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			double	pValue[FRAME_SEG_SIZE+1][7] = {0,};
			for(long nSeg=0; nSeg <= FRAME_SEG_SIZE; nSeg++)
			{
				m_pStd->m_ctlProgress.StepIt();

				double	dRatioElm	= nSeg* (1/double(FRAME_SEG_SIZE));
				double	dSta		= ModelCalc.GetStationByElmAndRatio(pElm->m_nIndex, dRatioElm);
				long	nG			= ModelCalc.GetGirderNoByElemID(pElm->m_nIndex);

				dStressRatio	= StressCalc.GetMaxStressRatio(nG, dSta, dSigma_up, dSigma_lo, dComp_up, dComp_lo, dTau);
				dValue			= StressCalc.GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, dSta,4);

				if(!StressCalc.IsPositive(ELE_SHEAR_2, nG, dSta))
					dTau = -dTau;
				pValue[nSeg][0]	= dRatioElm;
				pValue[nSeg][1]	= dSigma_up;
				pValue[nSeg][2]	= dSigma_lo;
				pValue[nSeg][3]	= dTau;
				pValue[nSeg][4]	= dComp_up;
				pValue[nSeg][5]	= dComp_lo;
				pValue[nSeg][6]	= dStressRatio*dValue;
				
				m_pStd->m_ctlProgress.SetPos((short)pElm->m_nIndex);
				szTemp.Format("거더 : %d   %s", nG+1, GetStationForStringOut(frM(dSta),3,TRUE,TRUE));
				pTS->SetPaneText(0, szTemp, TRUE);
				UpdateData(FALSE);
			}
			pFEM->MakeElementResult(nLoadCase, (unsigned short)nElem);
			pFEM->SetElementData(nLoadCase, (unsigned short)nElem, FRAME_SEG_SIZE+1, pValue);
		}
		nElem++;
	}

	pTS->SetPaneText(0, "", TRUE);
	m_pStd->m_ctlProgress.ShowWindow(FALSE); 
	UpdateData(FALSE);
	DrawInputDomyunView(TRUE);
}


void CCalcSecStress::OnCheckBuckling() 
{
	UpdateData(TRUE);
	SetDataSave();
	OnButtonCalcStress();
}
