// OptionSheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "APlateUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

IMPLEMENT_DYNAMIC(COptionSheet, CPropertySheet)

COptionSheet::COptionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_bSetOk = FALSE;  
}

COptionSheet::COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_PageGen); 
	AddPage(&m_PageGeneral);
	AddPage(&m_PageGeneralDim);
	AddPage(&m_PageDetail);
	AddPage(&m_PageDetailDim);
	AddPage(&m_PageSplice);
	AddPage(&m_PageCrossBeam);
	AddPage(&m_PageHVracing);
	AddPage(&m_PageVBracing);
	AddPage(&m_OptionBMPage);
	AddPage(&m_OptionShoe);
	AddPage(&m_OptionCamber);
	AddPage(&m_OptionEtcDetail);
	AddPage(&m_OptionSummary);// 070116....ADD...KB...
//	AddPage(&m_OptionWeldMap);
	AddPage(&m_OptionPlanGen);
	AddPage(&m_OptionLineInfo);
	AddPage(&m_OptionPaint);
	AddPage(&m_OptionSlabGen);
	AddPage(&m_OptionTendon);
	AddPage(&m_OptionDeckRebar);
	AddPage(&m_OptionTUGirder);
	m_bSetOk = FALSE;
}

COptionSheet::~COptionSheet()
{
	CAPlateDrawStd *pStd = m_pPlateDrawStd;
	//LSH_ZZ : CPlateView::OnOption()에서 처리함
	//pDoc->m_nOptionCur = pStd->m_pDomOptionStd->m_nOptionCur;
}

BEGIN_MESSAGE_MAP(COptionSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COptionSheet)
	ON_BN_CLICKED(ID_PLATEOPT_SAVEBUTTON, OnSave)
	ON_BN_CLICKED(ID_PLATEOPT_LOADBUTTON, OnLoad)
	ON_WM_CREATE()
	ON_WM_CLOSE()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSheet message handlers

int COptionSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_PageGen.m_pPlateDrawStd			= m_pPlateDrawStd;
	m_PageGeneral.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_PageGeneralDim.m_pPlateDrawStd	= m_pPlateDrawStd;
	m_PageDetail.m_pPlateDrawStd		= m_pPlateDrawStd;		
	m_PageDetailDim.m_pPlateDrawStd		= m_pPlateDrawStd;		
	m_PageSplice.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_PageCrossBeam.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_PageHVracing.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_PageVBracing.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionBMPage.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionShoe.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionCamber.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionEtcDetail.m_pPlateDrawStd	= m_pPlateDrawStd;
	m_OptionSummary.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionPlanGen.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionLineInfo.m_pPlateDrawStd	= m_pPlateDrawStd;
	m_OptionPaint.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionSlabGen.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionTendon.m_pPlateDrawStd		= m_pPlateDrawStd;
	m_OptionDeckRebar.m_pPlateDrawStd	= m_pPlateDrawStd;	
	m_OptionTUGirder.m_pPlateDrawStd	= m_pPlateDrawStd;

	return 0;
}


void COptionSheet::OnClose() 
{
	CPropertySheet::OnClose();
}

BOOL COptionSheet::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return FALSE;
	}
	return CPropertySheet::PreTranslateMessage(pMsg);
}

BOOL COptionSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	CAPlateDrawStd	*pStd = m_pPlateDrawStd;
	CPlateBridgeApp	*pDB = pStd->GetDataManage()->GetBridge();

	if(!pDB->IsTUGir())
		RemovePage(&m_OptionTUGirder);

	//LSH_ZZ : CPlateView::OnOption()에서 처리함
	//pStd->m_pDomOptionStd->m_nOptionCur = pDoc->m_nOptionCur;
	SetActivePage(pStd->m_pDomOptionStd->m_nOptionCur);
	m_bSetOk = TRUE;

	CRect rect;
	GetClientRect(&rect);
	HFONT font;
	font = CreateFontSize("굴림", 12);	
	m_buttonLoad.Create("불러오기",WS_CHILD|WS_VISIBLE,CRect(10,rect.bottom-27,90,rect.bottom-3),this,ID_PLATEOPT_LOADBUTTON);
	m_buttonLoad.SendMessage(WM_SETFONT, (WPARAM)font, 0);
	m_buttonSave.Create("저장",WS_CHILD|WS_VISIBLE,CRect(100,rect.bottom-27,180,rect.bottom-3),this,ID_PLATEOPT_SAVEBUTTON);
	m_buttonSave.SendMessage(WM_SETFONT, (WPARAM)font, 0);
	m_buttonDefault.Create("기본값",WS_CHILD|WS_VISIBLE,CRect(190,rect.bottom-27,260,rect.bottom-3),this,ID_PLATEOPT_DEFAULTBUTTON);
	m_buttonDefault.SendMessage(WM_SETFONT, (WPARAM)font, 0);	
	return bResult;
}

void COptionSheet::OnSave()
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	CFileDialog dlg(FALSE, "*.apt",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		              "Plate 환경 옵션파일(*.apt)|*.apt||",NULL);
	dlg.m_ofn.lpstrTitle = "옵션 저장";
	if(dlg.DoModal()==IDOK)
	{
		UpdateData();
		CFile file;
		if(!file.Open(dlg.GetPathName(),CFile::modeWrite | CFile::modeCreate)) return;
		CArchive ar(&file,CArchive::store);		
		pOpt->Serialize(ar);
		ar.Close();
		file.Close();
	}
}

void COptionSheet::OnLoad()
{
	CAPlateDrawStd	*pStd	= m_pPlateDrawStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	CFileDialog dlg(TRUE, "*.apt",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		              "Plate 환경 옵션파일(*.apt)|*.apt||",NULL);
	dlg.m_ofn.lpstrTitle = "옵션 불러오기";
	if(dlg.DoModal()==IDOK)
	{
		CFile file;
		if(!file.Open(dlg.GetPathName(),CFile::modeRead)) return;
		CArchive ar(&file,CArchive::load);
		pOpt->Serialize(ar);
		ar.Close();
		file.Close();
		EndDialog(0);		
	}
}