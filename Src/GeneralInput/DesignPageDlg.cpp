// DesignPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GeneralInput.h"
#include "DesignPage.h"
#include "DesignLSteelPage.h"
#include "DesignLVarSteelPage.h"
#include "DesignCSteelPage.h"
#include "DesignCTSteelPage.h"
#include "DesignISteelPage.h"
#include "DesignHSteelPage.h"
#include "DesignSteelMaterialPage.h"
#include "DesignBMPage.h"
#include "DesignDeductPage.h"
#include "DesignWeldPage.h"
#include "DesignScallopPage.h"
#include "DesignBoltPage.h"
#include "DesignStudPage.h"
#include "DesignSteelDimPage.h"
#include "DesignUWeightPage.h"
#include "DesignConcretePage.h"
#include "DesignGradePage.h"
#include "DesignMaterialPropertyPage.h"
#include "DesignPrestressPage.h"
#include "DesignFrictionCoeffPage.h"
#include "DesignPrestressPropertyPage.h"
#include "DesignSpecialMovingLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignPageDlg dialog


CDesignPageDlg::CDesignPageDlg(CWnd* pParent /*=NULL*/)
: TStackedDialog(IDC_PLACEHOLDER, CDesignPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDesignPageDlg)
	//}}AFX_DATA_INIT
	m_nBridgeType = 0;
}

void CDesignPageDlg::DoDataExchange(CDataExchange* pDX)
{
	TStackedDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignPageDlg)
	DDX_Control(pDX, IDC_TREE_MENU, m_ctlMenuTree);
	DDX_Control(pDX, IDC_PLACEHOLDER, m_PlaceHolder);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_OPENOPT, m_btnOptSave);
	DDX_Control(pDX, IDC_BUTTON_SAVEOPT, m_btnOptOpen);
	DDX_Control(pDX, IDC_BUTTON_F10T, m_btnF10T);
	DDX_Control(pDX, IDC_BUTTON_F13T, m_btnF13T);
	DDX_Control(pDX, IDC_COMBO_BOLTLENGTH, m_ComboLengthCalc);
}


BEGIN_MESSAGE_MAP(CDesignPageDlg, TStackedDialog)
	//{{AFX_MSG_MAP(CDesignPageDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MENU, OnSelchangedTreeMenu)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_BUTTON_RECOMMEND, OnButtonRecommendEachDlg)
	ON_BN_CLICKED(IDC_BUTTON_OPENOPT, OnButtonOpenopt)
	ON_BN_CLICKED(IDC_BUTTON_SAVEOPT, OnButtonSaveopt)
	ON_BN_CLICKED(IDC_BUTTON_F10T, OnButtonF10T)
	ON_BN_CLICKED(IDC_BUTTON_F13T, OnButtonF13t)
	ON_CBN_SELCHANGE(IDC_COMBO_BOLTLENGTH, OnSelchangeComboBoltlength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignPageDlg message handlers

void CDesignPageDlg::SetDataDefault()
{
	GetPage(IDD_DESIGN_BASIC_CONDITION)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_MEMEBER_SET)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_MATERIAL_SET)->GetPage()->SetDataDefault();
	if(m_nBridgeType != PLATE_GIR)
		GetPage(IDD_DESIGN_PS_MEMBER)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_UNIT_WEIGHT)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_BM_SET)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_DEDUCT_SET)->GetPage()->SetDataDefault();	
	GetPage(IDD_DESIGN_WELD_SET)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_SCALLUP_SET)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_STEEL_MEMBER)->GetPage()->SetDataDefault();
	if(m_nBridgeType != PLATE_GIR)
	{
		GetPage(IDD_DESIGN_PS_PROPERTY)->GetPage()->SetDataDefault();
		GetPage(IDD_DESIGN_FRICTION_COEFF)->GetPage()->SetDataDefault();
	}
	GetPage(IDD_DESIGN_BOLT)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_STUD)->GetPage()->SetDataDefault();
	//GetPage(IDD_DESIGN_CONCRETE)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_GRADE_LOAD)->GetPage()->SetDataDefault();
	//GetPage(IDD_DESIGN_SPECIAL_LOAD)->GetPage()->SetDataDefault();

	// 형강제원
	GetPage(IDD_DESIGN_STEEL_L1)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_STEEL_L2)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_STEEL_C)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_STEEL_CT)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_STEEL_I)->GetPage()->SetDataDefault();
	GetPage(IDD_DESIGN_STEEL_H)->GetPage()->SetDataDefault();
}
// m_nBridgeType : 0 판형교, 1 소수주형교, 2 TU거더교
void CDesignPageDlg::SetDataInit()
{
	GetPage(IDD_DESIGN_BASIC_CONDITION)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_MEMEBER_SET)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_MATERIAL_SET)->GetPage()->SetDataInit();
	if(m_nBridgeType != PLATE_GIR)
		GetPage(IDD_DESIGN_PS_MEMBER)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_UNIT_WEIGHT)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_BM_SET)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_DEDUCT_SET)->GetPage()->SetDataInit();	
	GetPage(IDD_DESIGN_WELD_SET)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_SCALLUP_SET)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_STEEL_MEMBER)->GetPage()->SetDataInit();
	if(m_nBridgeType != PLATE_GIR)
	{
		GetPage(IDD_DESIGN_PS_PROPERTY)->GetPage()->SetDataInit();
		GetPage(IDD_DESIGN_FRICTION_COEFF)->GetPage()->SetDataInit();
	}
	GetPage(IDD_DESIGN_BOLT)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_STUD)->GetPage()->SetDataInit();
	//GetPage(IDD_DESIGN_CONCRETE)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_GRADE_LOAD)->GetPage()->SetDataInit();
	//GetPage(IDD_DESIGN_SPECIAL_LOAD)->GetPage()->SetDataInit();

	// 형강제원
	GetPage(IDD_DESIGN_STEEL_L1)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_STEEL_L2)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_STEEL_C)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_STEEL_CT)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_STEEL_I)->GetPage()->SetDataInit();
	GetPage(IDD_DESIGN_STEEL_H)->GetPage()->SetDataInit();

//	m_ComboLengthCalc.SetCurSel(m_pStd->GetDataManage()->GetSteelBolt()->m_nLengthCalcType);
}

void CDesignPageDlg::SetDataSave()
{
	CDesignSteelMaterialPage* pSteel;
	pSteel =  (CDesignSteelMaterialPage*) GetPage(IDD_DESIGN_MEMEBER_SET)->GetPage();
					
	if(pSteel->m_bChange && m_pStd->GetDlgOfIDD(IDD_GEN_19_FACTJOINPOSITION)->IsUsedFlag())
	{
		AfxMessageBox("변경된 강종을 적용하실려면 단면제원 변화구간 입력에서 '기본강종 가져오기' 버튼을 눌러 현재 입력에 반영을 하셔야 합니다.", MB_ICONINFORMATION );
	    pSteel->m_bChange = FALSE;
	}
	else
		pSteel->m_bChange = FALSE;

	GetPage(IDD_DESIGN_BASIC_CONDITION)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_MEMEBER_SET)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_MATERIAL_SET)->GetPage()->SetDataSave();
	if(m_nBridgeType != PLATE_GIR)
		GetPage(IDD_DESIGN_PS_MEMBER)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_UNIT_WEIGHT)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_BM_SET)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_DEDUCT_SET)->GetPage()->SetDataSave();	
	GetPage(IDD_DESIGN_WELD_SET)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_SCALLUP_SET)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_STEEL_MEMBER)->GetPage()->SetDataSave();
	if(m_nBridgeType != PLATE_GIR)
	{
		GetPage(IDD_DESIGN_PS_PROPERTY)->GetPage()->SetDataSave();
		GetPage(IDD_DESIGN_FRICTION_COEFF)->GetPage()->SetDataSave();
	}
	GetPage(IDD_DESIGN_BOLT)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_STUD)->GetPage()->SetDataSave();
	//GetPage(IDD_DESIGN_CONCRETE)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_GRADE_LOAD)->GetPage()->SetDataSave();
	//GetPage(IDD_DESIGN_SPECIAL_LOAD)->GetPage()->SetDataSave();

	// 형강제원
	GetPage(IDD_DESIGN_STEEL_L1)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_STEEL_L2)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_STEEL_C)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_STEEL_CT)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_STEEL_I)->GetPage()->SetDataSave();
	GetPage(IDD_DESIGN_STEEL_H)->GetPage()->SetDataSave();
	
	TStackedPageHelper *pPageHlper = GetPage();
	if(pPageHlper) pPageHlper->GetPage()->SetDataSave();
	
	GetPage()->GetPage()->SetDataSave();
	
	if(m_pStd->m_pDoc)
		m_pStd->m_pDoc->UpdateFrameCounts();
}

void CDesignPageDlg::SetShowButton(BOOL bShow)
{
	BOOL bActive = (bShow)? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_BUTTON_F10T)->ShowWindow(bActive);
	GetDlgItem(IDC_BUTTON_F13T)->ShowWindow(bActive);
}

void CDesignPageDlg::SetShowEdit(BOOL bShow)
{
	BOOL bActive = (bShow)? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_COMBO_BOLTLENGTH)->ShowWindow(bActive);
	GetDlgItem(IDC_STATIC_BOLTCALC)->ShowWindow(bActive);
}

void CDesignPageDlg::OnPreInitDialog()
{
	m_ctlMenuTree.DeleteAllItems();
	m_MenuStr.RemoveAll();
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	m_nBridgeType = pBridge->GetBridgeType();
	m_ctlMenuTree.SetImageList(&m_TreeImageList,TVSIL_NORMAL);
	//설계조건
	m_MenuStr.Add("기본조건");			m_nIdArray.Add(IDD_DESIGN_BASIC_CONDITION);
	m_MenuStr.Add("부재설정");			m_nIdArray.Add(IDD_DESIGN_MEMEBER_SET);	
	m_MenuStr.Add("재료물리상수");		m_nIdArray.Add(IDD_DESIGN_MATERIAL_SET);
	if(m_nBridgeType != PLATE_GIR)
	{
		m_MenuStr.Add("PS강재설정");			m_nIdArray.Add(IDD_DESIGN_PS_MEMBER);
	}
	m_MenuStr.Add("단위중량");			m_nIdArray.Add(IDD_DESIGN_UNIT_WEIGHT);
	m_MenuStr.Add("집계설정");			m_nIdArray.Add(IDD_DESIGN_BM_SET);
	m_MenuStr.Add("공제설정");			m_nIdArray.Add(IDD_DESIGN_DEDUCT_SET);	
	m_MenuStr.Add("용접설정");			m_nIdArray.Add(IDD_DESIGN_WELD_SET);
	m_MenuStr.Add("스켈럽설정");		m_nIdArray.Add(IDD_DESIGN_SCALLUP_SET);
	m_MenuStr.Add("강재제원");			m_nIdArray.Add(IDD_DESIGN_STEEL_MEMBER);
	if(m_nBridgeType != PLATE_GIR)
	{
		m_MenuStr.Add("PS강재제원");	m_nIdArray.Add(IDD_DESIGN_PS_PROPERTY);
		m_MenuStr.Add("마찰계수");		m_nIdArray.Add(IDD_DESIGN_FRICTION_COEFF);
	}
	m_MenuStr.Add("볼트제원");			m_nIdArray.Add(IDD_DESIGN_BOLT);
	m_MenuStr.Add("스터드제원");		m_nIdArray.Add(IDD_DESIGN_STUD);
	//m_MenuStr.Add("콘크리트제원");		m_nIdArray.Add(IDD_DESIGN_CONCRETE);
	m_MenuStr.Add("활하중제원");		m_nIdArray.Add(IDD_DESIGN_GRADE_LOAD);
	//m_MenuStr.Add("특수하중제원");		m_nIdArray.Add(IDD_DESIGN_SPECIAL_LOAD);
		
	//형강단면
	m_MenuStr.Add("등변  ㄱ형강");		m_nIdArray.Add(IDD_DESIGN_STEEL_L1);	
	m_MenuStr.Add("부등변ㄱ형강");		m_nIdArray.Add(IDD_DESIGN_STEEL_L2);	
	m_MenuStr.Add("ㄷ형강");			m_nIdArray.Add(IDD_DESIGN_STEEL_C);	
	m_MenuStr.Add("CT형강");			m_nIdArray.Add(IDD_DESIGN_STEEL_CT);	
	m_MenuStr.Add("I 형강");			m_nIdArray.Add(IDD_DESIGN_STEEL_I);	
	m_MenuStr.Add("H 형강");			m_nIdArray.Add(IDD_DESIGN_STEEL_H);	
	//////////////////////////////////////////////////////////////////
	long nIdx = 0;

	HTREEITEM hItem = m_ctlMenuTree.InsertItem("설계조건",1,1,TVI_ROOT);
	HTREEITEM hSubItem = m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	if(m_nBridgeType != PLATE_GIR)
		m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	if(m_nBridgeType != PLATE_GIR)
	{
		m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
		m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	}	
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	//m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	//m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);	
	
	m_ctlMenuTree.Expand(hItem,TVE_EXPAND);
	m_ctlMenuTree.SelectItem(hSubItem);
	hItem = m_ctlMenuTree.InsertItem("형강제원",1,1,TVI_ROOT);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.InsertItem(m_MenuStr.GetAt(nIdx++),0,0,hItem);
	m_ctlMenuTree.Expand(hItem,TVE_EXPAND);	

	AddPage(IDD_DESIGN_BASIC_CONDITION);
	AddPage(IDD_DESIGN_MEMEBER_SET);
	AddPage(IDD_DESIGN_MATERIAL_SET);
	if(m_nBridgeType != PLATE_GIR)
		AddPage(IDD_DESIGN_PS_MEMBER);
	AddPage(IDD_DESIGN_UNIT_WEIGHT);
	AddPage(IDD_DESIGN_BM_SET);
	AddPage(IDD_DESIGN_DEDUCT_SET);
	AddPage(IDD_DESIGN_WELD_SET);
	AddPage(IDD_DESIGN_SCALLUP_SET);
	AddPage(IDD_DESIGN_STEEL_MEMBER);
	if(m_nBridgeType != PLATE_GIR)
	{
		AddPage(IDD_DESIGN_PS_PROPERTY);
		AddPage(IDD_DESIGN_FRICTION_COEFF);
	}
	AddPage(IDD_DESIGN_BOLT);
	AddPage(IDD_DESIGN_STUD);
	//AddPage(IDD_DESIGN_CONCRETE);
	AddPage(IDD_DESIGN_GRADE_LOAD);
	//AddPage(IDD_DESIGN_SPECIAL_LOAD);
	AddPage(IDD_DESIGN_STEEL_L1);
	AddPage(IDD_DESIGN_STEEL_L2);
	AddPage(IDD_DESIGN_STEEL_C);
	AddPage(IDD_DESIGN_STEEL_CT);
	AddPage(IDD_DESIGN_STEEL_I);
	AddPage(IDD_DESIGN_STEEL_H);

	SetPage(IDD_DESIGN_BASIC_CONDITION);

	// set button and combo style
	SetResize(IDC_TREE_MENU,		SZ_TOP_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_BOLTCALC,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_OPENOPT,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAVEOPT,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_F10T,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_F13T,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_BOLTLENGTH,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_PLACEHOLDER,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetWindowText("기초설정 - 설계조건 - [" + m_MenuStr.GetAt(0) + "]");
}

TStackedPage *CDesignPageDlg::CreatePage(UINT nId)
{
	CDataManage *pDataMng = m_pStd->GetDataManage();

	switch (nId)
	{
		case IDD_DESIGN_BASIC_CONDITION:  return new CDesignPage(pDataMng, this);
		case IDD_DESIGN_MEMEBER_SET:  return new CDesignSteelMaterialPage(pDataMng);
		case IDD_DESIGN_MATERIAL_SET:  return new CDesignMaterialPropertyPage(pDataMng, this);
		case IDD_DESIGN_PS_MEMBER: return new CDesignPrestressPropertyPage(pDataMng, this);
		case IDD_DESIGN_UNIT_WEIGHT: return new CDesignUWeightPage(pDataMng);
		case IDD_DESIGN_BM_SET: return new CDesignBMPage(pDataMng);
		case IDD_DESIGN_DEDUCT_SET: return new CDesignDeductPage(pDataMng);
		case IDD_DESIGN_WELD_SET: return new CDesignWeldPage(pDataMng);
		case IDD_DESIGN_SCALLUP_SET: return new CDesignScallopPage(pDataMng);
		case IDD_DESIGN_STEEL_MEMBER: return new CDesignSteelDimPage(pDataMng);
		case IDD_DESIGN_PS_PROPERTY: return new CDesignPrestressPage(pDataMng);
		case IDD_DESIGN_FRICTION_COEFF: return new CDesignFrictionCoeffPage(pDataMng);
		case IDD_DESIGN_BOLT: return new CDesignBoltPage(pDataMng);
		case IDD_DESIGN_STUD: return new CDesignStudPage(pDataMng);
		//case IDD_DESIGN_CONCRETE: return new CDesignConcretePage(pDataMng);	//2009
		case IDD_DESIGN_GRADE_LOAD: return new CDesignGradePage(pDataMng);
		//case IDD_DESIGN_SPECIAL_LOAD: return new CDesignSpecialMovingLoad(pDataMng, this);	//2009
		case IDD_DESIGN_STEEL_L1:  return new CDesignLSteelPage(pDataMng);
		case IDD_DESIGN_STEEL_L2:  return new CDesignLVarSteelPage(pDataMng);
		case IDD_DESIGN_STEEL_C:  return new CDesignCSteelPage(pDataMng);
		case IDD_DESIGN_STEEL_CT:  return new CDesignCTSteelPage(pDataMng);
		case IDD_DESIGN_STEEL_I:  return new CDesignISteelPage(pDataMng);
		case IDD_DESIGN_STEEL_H:  return new CDesignHSteelPage(pDataMng);		
	}

	return NULL;    // this will assert
}

void CDesignPageDlg::OnPageChanged(UINT nId, BOOL bActivated)
{
	m_pStd->m_pDoc->SetModifiedFlag(TRUE);
	SetDataInit();
	RedrawWindow();

}

void CDesignPageDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		
	TStackedDialog::DrawInputDomyunView(bZoomAll);
}

void CDesignPageDlg::OnSelchangedTreeMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hitem = m_ctlMenuTree.GetSelectedItem();
	if(m_ctlMenuTree.ItemHasChildren(hitem)) return;	
	
	TV_ITEM tv;
	char text[80],ptext[80];
	memset(text,0,80);
	memset(ptext,0,80);
	tv.hItem = hitem;
	tv.mask = TVIF_HANDLE | TVIF_TEXT;
	tv.pszText = text;
	tv.cchTextMax = 30;
	m_ctlMenuTree.GetItem(&tv);
	
	hitem = m_ctlMenuTree.GetParentItem(hitem);
	tv.hItem = hitem;
	tv.mask = TVIF_HANDLE | TVIF_TEXT;
	tv.pszText = ptext;
	tv.cchTextMax = 30;
	m_ctlMenuTree.GetItem(&tv);

	for(long n=0;n<m_MenuStr.GetSize();n++)
	{
		if(m_MenuStr.GetAt(n) == text)	
		{
			SetPage(m_nIdArray.GetAt(n));			
			SetWindowText("기초설정 - " + CString(ptext) + " - [" + m_MenuStr.GetAt(n) + "]");
			break;
		}
	}

	MovePageWindow();

// 	if(!TStackedDialog::m_bPressPin) DrawTitleButton();
	*pResult = 0;

	// set show button and combo
	BOOL	bShow = FALSE;
	CString szText = _T("부재설정");

	if(szText == (LPCSTR)text) bShow = TRUE;
	SetShowButton(bShow);
	bShow = FALSE;

	szText = _T("볼트제원");
	if(szText == (LPCSTR)text) bShow = TRUE;
	SetShowEdit(bShow);
}

int CDesignPageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (TStackedDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_TreeImageList.GetSafeHandle())
		m_TreeImageList.Create(IDB_TREEBAR_TAB,16,1,RGB(255,0,0));

	return 0;
}

void CDesignPageDlg::OnSize(UINT nType, int cx, int cy) 
{
	TStackedDialog::OnSize(nType, cx, cy);
	MovePageWindow();

	CRect Rect;
	const long ButtonWidth		=  67;
	const long ButtonHeight		=  22;
	const CRect ButtonRect(0,0,ButtonWidth,ButtonHeight);
}

void CDesignPageDlg::OnButtonRecommendEachDlg() 
{
	GetPage()->GetPage()->SetDataDefault();
}

void CDesignPageDlg::MovePageWindow()
{
	CRect rect;
	if(GetDlgItem(IDC_PLACEHOLDER)->GetSafeHwnd())
	{
		GetDlgItem(IDC_PLACEHOLDER)->GetWindowRect(&rect);		
		ScreenToClient(rect);
		TStackedPageHelperList::iterator it;
		for (it = m_PageList.begin(); it != m_PageList.end(); it ++) 					
			(*it).GetPage()->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	}
	RedrawWindow();
}

void CDesignPageDlg::ResetDesignPage(long nBridgeType)
{
	m_nBridgeType = nBridgeType;
	m_MenuStr.RemoveAll();
	m_nIdArray.RemoveAll();
	m_ctlMenuTree.DeleteAllItems();
	OnPreInitDialog();
	SetDataInit();
}

void CDesignPageDlg::OnButtonOpenopt() 
{
	//m_pBaseChief->LoadBaseFile(m_pDB->IsRailBridgeType(),m_pDB->IsHL());

	CFileDialog dlg(TRUE, "*.pdt|*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		              "기초설정 (*.pdt)|*.pdt|", AfxGetMainWnd());
	dlg.m_ofn.lpstrTitle = "기초설정 옵션 파일 불러오기";

	if (dlg.DoModal() == IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;
		CArchive ar(&f,CArchive::load);
		m_pStd->m_pDataManage->Serialize(ar);
		ar.Close();
		f.Close();

		SetDataInit();				
		m_pStd->m_pDoc->SetModifiedFlag(TRUE);
   }
}

void CDesignPageDlg::OnButtonSaveopt() 
{
	CString csName = m_pStd->m_pDoc->GetTitle();
	if( csName.Find('.') >=0) { int pos = csName.Find('.'); csName = csName.Left(pos);}
	CFileDialog dlg(FALSE, "*.pdt|*.*",csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		              "기초설정 (*.pdt)|*.pdt|모든파일 (*.*)|*.*||", AfxGetMainWnd());
	dlg.m_ofn.lpstrTitle = "기초설정 옵션 파일 내보내기";

	if (dlg.DoModal() == IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&f,CArchive::store);
		m_pStd->m_pDataManage->Serialize(ar);
		ar.Close();
		f.Close();
	}	
}

void CDesignPageDlg::OnButtonF10T() 
{
	CDesignSteelMaterialPage *pPage = (CDesignSteelMaterialPage *)GetPage()->GetPage();
	pPage->OnButtonF10T();
}

void CDesignPageDlg::OnButtonF13t() 
{
	CDesignSteelMaterialPage *pPage = (CDesignSteelMaterialPage *)GetPage()->GetPage();
	pPage->OnButtonF13T();
}

void CDesignPageDlg::OnSelchangeComboBoltlength() 
{
	UpdateData(TRUE);
	m_pStd->GetDataManage()->GetSteelBolt()->m_nLengthCalcType = m_ComboLengthCalc.GetCurSel();
}
