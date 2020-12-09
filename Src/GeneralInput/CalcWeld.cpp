// CalcWeld.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "CalcWeld.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcWeld dialog


CCalcWeld::CCalcWeld(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcWeld::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcWeld)	
	//}}AFX_DATA_INIT
}


void CCalcWeld::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcWeld)
	DDX_Control(pDX, IDC_RICHEDIT_WELD, m_richWeld);	
	DDX_Control(pDX, IDC_S_YONGJUB_COMBO1,		m_ComboBox1);	
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_S_YONGJUB_TOTEXT, m_btnOutText);
}


BEGIN_MESSAGE_MAP(CCalcWeld, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcWeld)
	ON_CBN_SELCHANGE(IDC_S_YONGJUB_COMBO1, OnSelchangeWeldCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcWeld message handlers
void CCalcWeld::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);		
	CDomyun Dom(pDom);
	*pDom << Dom;

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcWeld::SetDataInit()
{	
	CPlateBridgeApp *pDB      = m_pStd->GetBridge();
	CCalcData       *pData    = m_pStd->m_pCalcStd->GetCalcData();
	CFemStressCalc StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CPlateStressOut	pStressOut(m_pStd);


	m_richWeld.SetSel(0, -1);
	m_richWeld.ReplaceSel("");
	m_richWeld.SetReadOnly(TRUE);
	UpdateData(TRUE);

	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		m_richWeld.AddText("\n< 모델링 결과파일을 읽지 않았습니다. >\n");
		return;
	}
	
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_S_YONGJUB_COMBO1);
	long nType = pCombo->GetCurSel();


	if(nType < 0)	
	{	
		nType = 0;
		pCombo->SetCurSel(0);
	}

	if(m_pStd->m_pCalcStd->GetSizeByundanInfo(0) == 0)
	{
		StressCalc.SetSectionGugan();
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
			StressCalc.MakeDanmyunMomentData(nG);
	}
	
	BOOL bJijum = (nType == pCombo->GetCount()-1) ? TRUE : FALSE;
	pStressOut.Output_WeldReview(nType, m_richWeld, bJijum);

	m_richWeld.LineScroll(-m_richWeld.GetFirstVisibleLine());
	//UpdateData(FALSE);

}

void CCalcWeld::SetDataSave()
{

}

void CCalcWeld::SetDataDefault()
{

}

void CCalcWeld::OnPreInitDialog()
{
	// 합성후 단면에 대해 용접검토
	CFEMManage		*pFEM = m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CPlateBridgeApp *pDB	 = m_pStd->GetBridge();
	CFemStressCalc StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_S_YONGJUB_COMBO1);
	pCombo->ResetContent();

	CString cs;
	if(m_pStd->m_pCalcStd->GetSizeByundanInfo(0) == 0)
	{
		StressCalc.SetSectionGugan();
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
			StressCalc.MakeDanmyunMomentData(nG);
	}

	long	nIdx	= 1;
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag == POS_GIRDER)
		{
			cs.Format("단면 - %d", nIdx);
			pCombo->AddString(cs);
			nIdx++;
		}
	}
		
	pCombo->AddString("지점부");

	if(nIdx > 0)
		pCombo->SetCurSel(0);

	SetResize(IDC_RICHEDIT_WELD, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_S_YONGJUB_TOTEXT, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}

void CCalcWeld::OnSelchangeWeldCombo() 
{
	UpdateData(TRUE);
	SetDataInit();
}

void CCalcWeld::EditValueOut(int nID, double dValue, char *pFormat)
{
	CString sValue;
	sValue.Format (pFormat, dValue);
	CEdit *pEdit = (CEdit *) GetDlgItem (nID);
	pEdit->SetWindowText (sValue);
}

void CCalcWeld::StaticTextOut(int nID, CString &sText)
{
	CStatic *pStatic = (CStatic *) GetDlgItem (nID);
	pStatic->SetWindowText (sText);
}

void CCalcWeld::GetJijumVStiffBxByMaxReaction(BOOL bPier)
{
	CPlateBridgeApp	*pBridgeApp	= m_pStd->GetBridge();
	CDataManage		*pDataManage= m_pStd->GetDataManage();
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage	*pFEM    = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CResultArray	*pResult	= pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);

	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	m_ArrBxAbut.RemoveAll();
	m_ArrBxPier.RemoveAll();

	for(long nG=0; nG<pBridgeApp->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridgeApp->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			if(pBx->IsDanbuJijum())
				m_ArrBxAbut.Add(pBx);
			else if(pBx->IsPier())
				m_ArrBxPier.Add(pBx);
				
			pBx = Finder.GetBxNext();
		}
	}

	m_dMaxReaction	= 0;
	m_nMaxJoint		= 0;

	if(bPier)
	{
		for(long n=0; n<m_ArrBxPier.GetSize(); n++)
		{
			CPlateBasicIndex *pBx = m_ArrBxPier.GetAt(n);
			long nJoint = ModelCalc.GetJointNoByBx(pBx);
			CJoint		*pJ			= pFEM->GetJoint((unsigned short)nJoint);
			CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(nJoint);
			if(m_dMaxReaction < pJResult->m_vTotalMax[JOINT_REACTION].z)
			{
				m_dMaxReaction = pJResult->m_vTotalMax[JOINT_REACTION].z;
				m_nMaxJoint = nJoint;
			}
		}
	}
	else
	{
		for(long n=0; n<m_ArrBxAbut.GetSize(); n++)
		{
			CPlateBasicIndex *pBx = m_ArrBxAbut.GetAt(n);
			long nJoint = ModelCalc.GetJointNoByBx(pBx);
			CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(nJoint);

			if(m_dMaxReaction < pJResult->m_vTotalMax[JOINT_REACTION].z)
			{
				m_dMaxReaction	= pJResult->m_vTotalMax[JOINT_REACTION].z;
				m_nMaxJoint		= nJoint;
			}
		}
	}
}
