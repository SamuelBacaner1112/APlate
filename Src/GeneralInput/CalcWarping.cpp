// CalcWarping.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcWarping.h"
#include "../APlateCalc/APlateCalc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcWarping dialog


CCalcWarping::CCalcWarping(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcWarping::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcWarping)
	m_CheckOKNG = _T("");
	m_WeakSectionType = _T("");
	m_strKFactor = _T("");
	m_strIFactor = _T("");
	//}}AFX_DATA_INIT
}


void CCalcWarping::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcWarping)
	DDX_Control(pDX, IDC_S_WARPING_CHECK_COMBO1, m_TypeCombo);
	DDX_Text(pDX, IDC_S_WARPING_CHECK_OKNG, m_CheckOKNG);
	DDX_Text(pDX, IDC_S_WARPING_Type, m_WeakSectionType);
	DDX_Text(pDX, IDC_S_WARPING_CHECK_EDIT1, m_strKFactor);
	DDX_Text(pDX, IDC_S_WARPING_CHECK_EDIT2, m_strIFactor);
	DDX_Control(pDX, IDC_S_WARPING_CHECK_EDIT1, m_ctrlEdit1);
	DDX_Control(pDX, IDC_S_WARPING_CHECK_EDIT2, m_ctrlEdit2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcWarping, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcWarping)
	ON_CBN_SELCHANGE(IDC_S_WARPING_CHECK_COMBO1, OnSelchangeWarpingGirderCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcWarping message handlers
void CCalcWarping::OnPreInitDialog()
{
	// Init Combobox
	CAPlateCalcStd  *pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage		*pFEMManage	= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	
	CString cs       = "";

	m_TypeCombo.ResetContent();
	
	long nSizeSec = pFEMManage->GetFrameSectionSize();
	for(long nSec=0; nSec<nSizeSec; nSec++)
	{
		cs.Format("�ܸ� TYPE %d", nSec+1);
		m_TypeCombo.AddString(cs);
	}

	SetResize(IDC_S_WARPING_CHECK_STATIC, SZ_TOP_LEFT, SZ_BOTTOM_CENTER);
	SetResize(IDC_S_WARPING_CHECK_STATIC5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_WARPING_CHECK_COMBO1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_S_WARPING_Type, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_S_WARPING_CHECK_STATIC2, SZ_TOP_CENTER, SZ_BOTTOM_RIGHT);	
	SetResize(IDC_S_WARPING_CHECK_STATIC3, SZ_TOP_CENTER, SZ_TOP_CENTER);	
	SetResize(IDC_S_WARPING_CHECK_STATIC4, SZ_TOP_CENTER, SZ_TOP_CENTER);	
	SetResize(IDC_S_WARPING_CHECK_EDIT1, SZ_TOP_CENTER, SZ_TOP_CENTER);	
	SetResize(IDC_S_WARPING_CHECK_EDIT2, SZ_TOP_CENTER, SZ_TOP_CENTER);	

	SetResize(IDC_S_WARPING_CHECK_OKNG, SZ_BOTTOM_CENTER, SZ_BOTTOM_CENTER);	
			  
	UpdateData(FALSE);
}

void CCalcWarping::SetDataInit()
{
	CAPlateCalcStd		*pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage			*pFEMManage	= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc	ModelCalc(m_pStd->GetDataManage(), pCalcStd);
	CFemStressCalc		StressCalc(m_pStd->GetDataManage(), pCalcStd);

	///< ��ü �ܸ� �� ������Ʋ�� ����� ���� ���� �ܸ� ã��
	double dSta        = 0;	
	long   nG          = 0;
	double dAlphaRatio = 10000, dAlphaRatio_Old=0;
	long   nSizeSec    = pFEMManage->GetFrameSectionSize();

	for(long nMat=0; nMat<nSizeSec; nMat++)
	{
		ModelCalc.GetMaxForcePositionOfSection(nMat+1, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
		
		double dStaCheck = StressCalc.GetStationCheckPos(nG, dSta);

		dAlphaRatio_Old = Calc_WarpingRatio(nG, dStaCheck);

		if(dAlphaRatio > dAlphaRatio_Old)
		{
			dAlphaRatio       = dAlphaRatio_Old;
			m_nCurSectionType = nMat;
		}
	}

	m_WeakSectionType.Format("���� ����� �ܸ� :  �ܸ� TYPE %d", m_nCurSectionType+1);

	m_TypeCombo.SetCurSel(long(m_nCurSectionType));

	UpdateData(FALSE);
}

void CCalcWarping::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);		
	CDomyun Dom(pDom);
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcWarping::SetDataDefault()
{

}

void CCalcWarping::SetDataSave()
{

}

double CCalcWarping::Calc_WarpingRatio(long nG, double dSta)
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	ModelCalc.GetSectionCoefficient(nG, dSta);

	double dK  = ModelCalc.GetMapValueSecCo("K");///< mm4
	double dIw = ModelCalc.GetMapValueSecCo("WarpingIw");///<mm6
	double dWarpingAlpha = ModelCalc.GetMapValueSecCo("WarpingAlpha");

	m_strKFactor.Format("%.3f", dK);
	m_strIFactor.Format("%.3f", dIw);
	
	if(dWarpingAlpha <0.4) 
		m_CheckOKNG.Format("   ����,  ��Ʋ�������(��) = L ����(GK/EIw) = %6.3f               �� < 0.4 �̹Ƿ�  \n\n         ���� ��Ʋ���� ���� ����� ���� �Ҽ��ִ�.!",dWarpingAlpha);
	
	if((dWarpingAlpha >=0.4)&&(dWarpingAlpha <=10)) 
		m_CheckOKNG.Format("   ����,  ��Ʋ�������(��) = L ����(GK/EIw) = %6.3f         0.4 < �� < 10 �̹Ƿ�  \n\n         ���� ��Ʋ���� ����Ʋ���� ��� ����ؾ��Ѵ�!",dWarpingAlpha);
	
	if(dWarpingAlpha >10) 
		m_CheckOKNG.Format("   ����,  ��Ʋ�������(��) = L ����(GK/EIw) = %6.3f               �� > 10 �̹Ƿ�  \n\n         ������Ʋ���� ���� ���丸�ϸ� �Ǹ� WARPING�� ���� ����� �����ʾƵ� �ȴ�.",dWarpingAlpha);
	UpdateData(FALSE);
	return dWarpingAlpha;
}

void CCalcWarping::OnSelchangeWarpingGirderCombo() 
{
	CAPlateCalcStd		*pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage			*pFEMManage	= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc	ModelCalc(m_pStd->GetDataManage(), pCalcStd);
	CFemStressCalc		StressCalc(m_pStd->GetDataManage(), pCalcStd);

	m_nCurSectionType	= m_TypeCombo.GetCurSel();

	long	nG		= 0;
	double	dSta	= 0;
	ModelCalc.GetMaxForcePositionOfSection(m_nCurSectionType+1, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
	
	double dStaCheck = StressCalc.GetStationCheckPos(nG, dSta);
	Calc_WarpingRatio(nG, dStaCheck);

	UpdateData(FALSE);
}
