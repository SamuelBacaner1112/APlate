// CalcModelingFramePropty.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcModelingFramePropty.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"
#include "../APlateDBDraw/APlateDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingFramePropty dialog


CCalcModelingFramePropty::CCalcModelingFramePropty(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcModelingFramePropty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcModelingFramePropty)
	m_nPrePost = -1;
	//}}AFX_DATA_INIT
	m_Grid.SetDefaultCharacterCount(2);
}


void CCalcModelingFramePropty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcModelingFramePropty)
	DDX_Radio(pDX, IDC_RADIO_PRE, m_nPrePost);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDOK, m_btnOk);
}


BEGIN_MESSAGE_MAP(CCalcModelingFramePropty, CDialog)
	//{{AFX_MSG_MAP(CCalcModelingFramePropty)
	ON_BN_CLICKED(IDC_RADIO_PRE, OnRadioPre)
	ON_BN_CLICKED(IDC_RADIO_POST, OnRadioPost)
	ON_BN_CLICKED(IDC_RADIO_PRE_STEEL, OnRadioPreSteel)
	ON_BN_CLICKED(IDC_RADIO_PRE_CONC, OnRadioPreConc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingFramePropty message handlers
void CCalcModelingFramePropty::SetGridTitle()
{
	CAPlateCalcStd  *pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage      *pFemManage = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CString szTemp ="";

	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetEditable(FALSE);
	m_Grid.SetListMode(FALSE);

	m_Grid.SetRowCount(pFemManage->GetElementSize()*2+1);
	m_Grid.SetColumnCount(8);
	m_Grid.SetColumnWidthAll(110);
	m_Grid.SetColumnWidth(0, 70);
	m_Grid.SetColumnWidth(1, 70);

	m_Grid.SetTextMatrix(0, 0, "부재");
	m_Grid.SetTextMatrix(0, 1, "절점");
	m_Grid.SetTextMatrix(0, 2, "단면적(As)");
	m_Grid.SetTextMatrix(0, 3, "유효폭(플랜지)");
	m_Grid.SetTextMatrix(0, 4, "슬래브 전폭");
	m_Grid.SetTextMatrix(0, 5, "I33(강축)");
	m_Grid.SetTextMatrix(0, 6, "I22(약축)");
	m_Grid.SetTextMatrix(0, 7, "J(비틀림상수)");
	long n = 0;
	for(n=0; n<pFemManage->GetElementSize()*2; n=n+2)
		m_Grid.SetMergeRow(n+1, n+2, 0);

	for(n=0; n<pFemManage->GetElementSize()*2; n=n+2)
	{
		szTemp.Format("%d", long(n/2+1));
		m_Grid.SetTextMatrix(n+1, 0, szTemp);
	}
	POSITION pos = pFemManage->m_ElementList.GetHeadPosition();
	
	n = 1;

	while(pos)
	{
		CElement *pE = (CElement *)pFemManage->m_ElementList.GetNext(pos);		
		CJoint *pJ_I = pFemManage->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFemManage->GetJoint(pE->m_nJEnd);

		szTemp.Format("%d", pJ_I->m_nIdx+1);
		m_Grid.SetTextMatrix(n++, 1, szTemp);
		szTemp.Format("%d", pJ_J->m_nIdx+1);
		m_Grid.SetTextMatrix(n++, 1, szTemp);
	}
	m_Grid.SetRedraw(TRUE, TRUE);
	m_Grid.ExpandColumnsToFit();
}

void CCalcModelingFramePropty::SetDataInit()
{
	///
	CDataManage     *pDataManage = m_pStd->GetDataManage();
	CAPlateCalcStd  *pCalcStd    = m_pStd->m_pCalcStd;
	CPlateBridgeApp *pDB	     = (CPlateBridgeApp*)m_pStd->GetBridge();
	CFEMManage      *pFemManage  = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	

	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, FALSE);
//	m_pStd->m_FEMDraw.SetRenderingStress(FALSE);

	POSITION pos = pFemManage->m_ElementList.GetHeadPosition();
	long n=1;
	CString szTemp="";
	CPlateBasicIndex *pBxStt = NULL;
	CPlateBasicIndex *pBxEnd = NULL; 
	CPlateBasicIndex *pBxCr  = NULL;

	while(pos)
	{
		CElement *pE = (CElement *)pFemManage->m_ElementList.GetNext(pos);
		double	dArea=0, dEFStt=0, dEFEnd=0, dESStt=0, dESEnd=0, dI33=0, dI22=0, dK=0;
		if(pE->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			CJoint *pJ_I = pFemManage->GetJoint(pE->m_nJStt);
			CJoint *pJ_J = pFemManage->GetJoint(pE->m_nJEnd);
			
			pBxStt = pCalcStd->GetBxByJoint(pJ_I);
			pBxEnd = pCalcStd->GetBxByJoint(pJ_J);

			ModelCalc.GetSectionCoefficient(pBxStt);
			dEFStt	= toM(ModelCalc.GetEffectWidthFlange(pBxStt->GetNumberGirder(), pBxStt->GetStation()));	// 플랜지유효폭Stt
			dEFEnd	= toM(ModelCalc.GetEffectWidthFlange(pBxEnd->GetNumberGirder(), pBxEnd->GetStation()));	// 플랜지유효폭End
			if(pDB->IsTUGir())
			{
				switch(m_nPrePost)
				{
				case 1:	// 합성전(강재)
					dArea	= toM2(ModelCalc.GetMapValueSecCo("As_SUM"));
					dI33	= toM4(ModelCalc.GetMapValueSecCo("I33"));
					dI22	= toM4(ModelCalc.GetMapValueSecCo("I22"));
					dK		= toM4(ModelCalc.GetMapValueSecCo("K"));
					break;
				case 2:	// 합성전(구속콘크리트)
					dArea	= toM2(ModelCalc.GetMapValueSecCo("Avc"));
					dI33	= toM4(ModelCalc.GetMapValueSecCo("Ivc33"));
					dI22	= toM4(ModelCalc.GetMapValueSecCo("Ivc22"));
					dK		= toM4(ModelCalc.GetMapValueSecCo("Kvc"));
					break;
				case 3:	// 합성후
					dArea	= toM2(ModelCalc.GetMapValueSecCo("Avd"));				
					dESStt	= toM(ModelCalc.GetWidthSlab(pBxStt->GetNumberGirder(), pBxStt->GetStation()));	// 슬래브유효폭Stt
					dI33	= toM4(ModelCalc.GetMapValueSecCo("Ivd33"));
					dI22	= toM4(ModelCalc.GetMapValueSecCo("Ivd22"));
					dK		= toM4(ModelCalc.GetMapValueSecCo("Kvd"));				
					dESEnd	= toM(ModelCalc.GetWidthSlab(pBxEnd->GetNumberGirder(), pBxEnd->GetStation()));	// 슬래브유효폭End
					break;
				}
			}
			else
			{
				BOOL bPre	= (m_nPrePost== 0) ? TRUE : FALSE;
				dArea	= toM2(ModelCalc.GetMapValueSecCo(bPre ? "As_SUM" : "Av_SUM"));				
				dESStt	= bPre ? 0 : toM(ModelCalc.GetWidthSlab(pBxStt->GetNumberGirder(), pBxStt->GetStation()));	// 슬래브유효폭Stt
				dI33	= toM4(ModelCalc.GetMapValueSecCo(bPre ? "I33"	  : "Iv33"));
				dI22	= toM4(ModelCalc.GetMapValueSecCo(bPre ? "I22"	  : "Iv22"));
				dK		= toM4(ModelCalc.GetMapValueSecCo(bPre ? "K"	  : "Kv"));				
				dESEnd	= bPre ? 0 : toM(ModelCalc.GetWidthSlab(pBxEnd->GetNumberGirder(), pBxEnd->GetStation()));	// 슬래브유효폭End
			}
			
			///<단면적
			szTemp.Format("%.4lf", dArea);
			m_Grid.SetTextMatrix(n, 2, szTemp);
			///<플랜지 유효폭
			szTemp.Format("%.2lf", dEFStt);
			m_Grid.SetTextMatrix(n, 3, szTemp);
			///<슬래브 유효폭
			szTemp.Format("%.2lf", dESStt);
			m_Grid.SetTextMatrix(n, 4, szTemp);
			///<I33
			szTemp.Format("%.8lf", dI33);
			m_Grid.SetTextMatrix(n, 5, szTemp);
			///<I22
			szTemp.Format("%.8lf", dI22);
			m_Grid.SetTextMatrix(n, 6, szTemp);
			///<K
			szTemp.Format("%.8lf", dK);
			m_Grid.SetTextMatrix(n++, 7, szTemp);
			///<단면적/////////////////////////////////////////////////////////////////////////////////////////////////////
			szTemp.Format("%.4lf", dArea);
			m_Grid.SetTextMatrix(n, 2, szTemp);
			///<플랜지 유효폭
			szTemp.Format("%.2lf", dEFEnd);
			m_Grid.SetTextMatrix(n, 3, szTemp);
			///<슬래브 유효폭
			szTemp.Format("%.2lf", dESEnd);
			m_Grid.SetTextMatrix(n, 4, szTemp);
			///<I33
			szTemp.Format("%.8lf", dI33);
			m_Grid.SetTextMatrix(n, 5, szTemp);
			///<I22
			szTemp.Format("%.8lf", dI22);
			m_Grid.SetTextMatrix(n, 6, szTemp);
			///<J
			szTemp.Format("%.8lf", dK);
			m_Grid.SetTextMatrix(n++, 7, szTemp);
		}
		else if(pE->m_LongAttrib[ELEMENT_TYPE] & FRAME_SHOE_ELEMENT)
		{			
			POSITION posSec = pFemManage->m_FrameSectionList.GetHeadPosition();
			CFrameSection *pFrSec = NULL;
			while(posSec)
			{
				pFrSec =  (CFrameSection*)pFemManage->m_FrameSectionList.GetNext(posSec);  
				if(pFrSec->m_nFlag==POS_SHOE)
					break;
			}

			///<단면적			
			szTemp.Format("%.4lf", toM2(pFrSec->m_dA));
			m_Grid.SetTextMatrix(n, 2, szTemp);
			m_Grid.SetTextMatrix(n, 3, "-");
			m_Grid.SetTextMatrix(n, 4, "-");
			///<I33
			szTemp.Format("%.8lf", toM4(pFrSec->m_dIx));
			m_Grid.SetTextMatrix(n, 5, szTemp);
			///<I22
			szTemp.Format("%.8lf", toM4(pFrSec->m_dIy));
			m_Grid.SetTextMatrix(n, 6, szTemp);
			///<J
			szTemp.Format("%.8lf", toM4(pFrSec->m_dJ));
			m_Grid.SetTextMatrix(n++, 7, szTemp);
			///<단면적			
			szTemp.Format("%.4lf", toM2(pFrSec->m_dA));

			m_Grid.SetTextMatrix(n, 2, szTemp);
			m_Grid.SetTextMatrix(n, 3, "-");
			m_Grid.SetTextMatrix(n, 4, "-");
			m_Grid.SetTextMatrix(n, 5, "동일");
			m_Grid.SetTextMatrix(n, 6, "동일");
			m_Grid.SetTextMatrix(n++, 7, "동일");
		}
		else
		{
			pBxCr = pCalcStd->GetBxByJoint(pFemManage->GetJoint(pE->m_nJStt));
			ModelCalc.GetSectionCoefficientCR(pBxCr);			
			///<단면적			
			szTemp.Format("%.4lf", toM2(ModelCalc.GetMapValueSecCoCR("Sum_As")));
			m_Grid.SetTextMatrix(n, 2, szTemp);
			m_Grid.SetTextMatrix(n, 3, "-");
			m_Grid.SetTextMatrix(n, 4, "-");
			///<I22
			szTemp.Format("%.8lf", toM4(ModelCalc.GetMapValueSecCoCR("I33")));
			m_Grid.SetTextMatrix(n, 5, szTemp);
			///<I33
			szTemp.Format("%.8lf", toM4(ModelCalc.GetMapValueSecCoCR("I22")));
			m_Grid.SetTextMatrix(n, 6, szTemp);
			///<J
			szTemp.Format("%.8lf", toM4(ModelCalc.GetMapValueSecCoCR("K")));
			m_Grid.SetTextMatrix(n++, 7, szTemp);
			///<단면적			
			szTemp.Format("%.4lf", toM2(ModelCalc.GetMapValueSecCoCR("Sum_As")));

			m_Grid.SetTextMatrix(n, 2, szTemp);
			m_Grid.SetTextMatrix(n, 3, "-");
			m_Grid.SetTextMatrix(n, 4, "-");
			m_Grid.SetTextMatrix(n, 5, "동일");
			m_Grid.SetTextMatrix(n, 6, "동일");
			m_Grid.SetTextMatrix(n++, 7, "동일");
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);

	UpdateData(FALSE);
}
void CCalcModelingFramePropty::OnRadioPre() 
{
	UpdateData(TRUE);
	SetDataInit();
}

void CCalcModelingFramePropty::OnRadioPost() 
{
	UpdateData(TRUE);
	SetDataInit();
}

void CCalcModelingFramePropty::OnRadioPreSteel() 
{
	UpdateData(TRUE);
	SetDataInit();
}

void CCalcModelingFramePropty::OnRadioPreConc()
{
	UpdateData(TRUE);
	SetDataInit();
}

BOOL CCalcModelingFramePropty::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CPlateBridgeApp *pDB = (CPlateBridgeApp*)m_pStd->GetBridge();
	if(pDB->IsTUGir())
	{
		m_nPrePost = 1;
		GetDlgItem(IDC_RADIO_PRE)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_PRE_STEEL)->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_PRE_CONC)->ShowWindow(TRUE);
	}
	else
	{
		m_nPrePost = 0;
		GetDlgItem(IDC_RADIO_PRE)->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_PRE_STEEL)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_PRE_CONC)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_POST)->SetWindowPos(m_pParentWnd,65,5,77,10,1);
	}
	
	UpdateData(FALSE);
	SetGridTitle();
	SetDataInit();
	
	return TRUE;
}
