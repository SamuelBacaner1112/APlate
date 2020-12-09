// BMOutSheet.cpp : implementation file 
// 

#include "stdafx.h"
#include "../PlateGangje\Gangje.h"
#include "BMOutSangPage.h"
#include "BMOutSheet.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

/////////////////////////////////////////////////////////////////////////////
// CBMOutSheet

#define WM_PAGE_RESET WM_USER+7
IMPLEMENT_DYNAMIC(CBMOutSheet, CPropertySheet)

CBMOutSheet::CBMOutSheet()	
	: CPropertySheet("", NULL, 0)
{
	// [01]기본도의 강재집계표
	m_pBMOutSangPage = new CBMOutSangPage;
	m_pBMOutSangPage->m_pBMOutSheet = this;
	m_pBMOutSangPage->m_nMakeFlag	= MAKE_BM_SANG;
	m_pBMOutSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutSangPage->m_psp.pszTitle = "상세 집계표";
	// [02]표준집계표
	m_pBMOutStandardSangPage = new CBMOutSangPage;
	m_pBMOutStandardSangPage->m_pBMOutSheet = this;
	m_pBMOutStandardSangPage->m_nMakeFlag	= MAKE_BM_STDANDARD_SANG;
	m_pBMOutStandardSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutStandardSangPage->m_psp.pszTitle = "표준 집계표";
	// [03]표준집계표(제작용)
	m_pBMOutStandardShopPage = new CBMOutSangPage;
	m_pBMOutStandardShopPage->m_pBMOutSheet = this;
	m_pBMOutStandardShopPage->m_nMakeFlag	= MAKE_BM_STDANDARD_SHOP;
	m_pBMOutStandardShopPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutStandardShopPage->m_psp.pszTitle = "표준 집계표(제작용)";
	// [03]표준집계표(제작용)
	m_pBMOutStandardShopType2Page = new CBMOutSangPage;
	m_pBMOutStandardShopType2Page->m_pBMOutSheet = this;
	m_pBMOutStandardShopType2Page->m_nMakeFlag	= MAKE_BM_STDANDARD_SHOP_TYPE2;
	m_pBMOutStandardShopType2Page->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutStandardShopType2Page->m_psp.pszTitle = "표준 집계표(제작용II)";
	// [04]강종별 두께별집계표
	m_pBMOutThickPage = new CBMOutSangPage;
	m_pBMOutThickPage->m_pBMOutSheet = this;
	m_pBMOutThickPage->m_nMakeFlag	= MAKE_BM_THICK;
	m_pBMOutThickPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutThickPage->m_psp.pszTitle = "강종별 두께별";
	// [04]강종별 두께별집계표
	m_pBMOutThickSangPage = new CBMOutSangPage;
	m_pBMOutThickSangPage->m_pBMOutSheet = this;
	m_pBMOutThickSangPage->m_nMakeFlag	= MAKE_BM_THICK_SANG;
	m_pBMOutThickSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutThickSangPage->m_psp.pszTitle = "공정별 두께별 상세";
	// [05]부재마크별 상세집계표
	m_pBMOutBujeMarkSangPage = new CBMOutSangPage;
	m_pBMOutBujeMarkSangPage->m_pBMOutSheet = this;
	m_pBMOutBujeMarkSangPage->m_nMakeFlag	= MAKE_BM_BUJEMARK_SANG;
	m_pBMOutBujeMarkSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBujeMarkSangPage->m_psp.pszTitle = "부재마크별 상세";
	// [06]부재마크별 총괄집계표
	m_pBMOutBujeMarkTotalPage = new CBMOutSangPage;
	m_pBMOutBujeMarkTotalPage->m_pBMOutSheet = this;
	m_pBMOutBujeMarkTotalPage->m_nMakeFlag	= MAKE_BM_BUJEMARK_TOTAL;
	m_pBMOutBujeMarkTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBujeMarkTotalPage->m_psp.pszTitle = "부재마크별 총괄";
	// [07]부재마크별 공제집계표
	m_pBMOutBujeMarkDeductPage = new CBMOutSangPage;
	m_pBMOutBujeMarkDeductPage->m_pBMOutSheet = this;
	m_pBMOutBujeMarkDeductPage->m_nMakeFlag	= MAKE_BM_BUJEMARK_DEDUCT;
	m_pBMOutBujeMarkDeductPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBujeMarkDeductPage->m_psp.pszTitle = "부재마크별 공제";
	// [08]차량적재 상세집계표
	m_pBMOutCarPage = new CBMOutSangPage;
	m_pBMOutCarPage->m_pBMOutSheet = this;
	m_pBMOutCarPage->m_nMakeFlag	= MAKE_BM_CAR;
	m_pBMOutCarPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutCarPage->m_psp.pszTitle = "차량적재";
	// [09]차량적재 총괄집계표
	m_pBMOutCarTotalPage = new CBMOutSangPage;
	m_pBMOutCarTotalPage->m_pBMOutSheet = this;
	m_pBMOutCarTotalPage->m_nMakeFlag	= MAKE_BM_CARTOTAL;
	m_pBMOutCarTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutCarTotalPage->m_psp.pszTitle = "차량적재 총괄";
	// [10]대조립별 상세집계표
	m_pBMOutGrandAssemSangPage = new CBMOutSangPage;
	m_pBMOutGrandAssemSangPage->m_pBMOutSheet = this;
	m_pBMOutGrandAssemSangPage->m_nMakeFlag	= MAKE_BM_GRAND_ASSEMBLY_SANG;
	m_pBMOutGrandAssemSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutGrandAssemSangPage->m_psp.pszTitle = "대조립마크별 상세";
	// [11]대조립별 총괄집계표
	m_pBMOutGrandAssemTotalPage = new CBMOutSangPage;
	m_pBMOutGrandAssemTotalPage->m_pBMOutSheet = this;
	m_pBMOutGrandAssemTotalPage->m_nMakeFlag	= MAKE_BM_GRAND_ASSEMBLY_TOTAL;
	m_pBMOutGrandAssemTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutGrandAssemTotalPage->m_psp.pszTitle = "대조립마크별 총괄";
	// [12]중조립별 상세집계표
	m_pBMOutMidAssemSangPage = new CBMOutSangPage;
	m_pBMOutMidAssemSangPage->m_pBMOutSheet = this;
	m_pBMOutMidAssemSangPage->m_nMakeFlag	= MAKE_BM_MID_ASSEMBLY_SANG;
	m_pBMOutMidAssemSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutMidAssemSangPage->m_psp.pszTitle = "중조립마크별 상세";
	// [13]중조립별 총괄집계표
	m_pBMOutMidAssemTotalPage = new CBMOutSangPage;
	m_pBMOutMidAssemTotalPage->m_pBMOutSheet = this;
	m_pBMOutMidAssemTotalPage->m_nMakeFlag	= MAKE_BM_MID_ASSEMBLY_TOTAL;
	m_pBMOutMidAssemTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutMidAssemTotalPage->m_psp.pszTitle = "중조립마크별 총괄";
	// [14]소조립별 상세집계표
	m_pBMOutAssemSangPage = new CBMOutSangPage;
	m_pBMOutAssemSangPage->m_pBMOutSheet = this;
	m_pBMOutAssemSangPage->m_nMakeFlag	= MAKE_BM_ASSEMBLY_SANG;
	m_pBMOutAssemSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutAssemSangPage->m_psp.pszTitle = "소조립마크별 상세";
	// [15]소조립별 총괄집계표
	m_pBMOutAssemTotalPage = new CBMOutSangPage;
	m_pBMOutAssemTotalPage->m_pBMOutSheet = this;
	m_pBMOutAssemTotalPage->m_nMakeFlag	= MAKE_BM_ASSEMBLY_TOTAL;
	m_pBMOutAssemTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutAssemTotalPage->m_psp.pszTitle = "소조립마크별 총괄";
	// [16]소조립별 공제집계표
	m_pBMOutAssemDeductPage = new CBMOutSangPage;
	m_pBMOutAssemDeductPage->m_pBMOutSheet = this;
	m_pBMOutAssemDeductPage->m_nMakeFlag	= MAKE_BM_ASSEMBLY_DEDUCT;
	m_pBMOutAssemDeductPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutAssemDeductPage->m_psp.pszTitle = "소조립마크별 공제";
	// [17]소조립별 공장제작용집계표
	m_pBMOutAssemFactPage = new CBMOutSangPage;
	m_pBMOutAssemFactPage->m_pBMOutSheet = this;
	m_pBMOutAssemFactPage->m_nMakeFlag	= MAKE_BM_ASSEMBLY_FACT;
	m_pBMOutAssemFactPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutAssemFactPage->m_psp.pszTitle = "소조립마크별 공장제작";
	// [18]소조립별 현장설치용집계표
	m_pBMOutAssemFieldPage = new CBMOutSangPage;
	m_pBMOutAssemFieldPage->m_pBMOutSheet = this;
	m_pBMOutAssemFieldPage->m_nMakeFlag	= MAKE_BM_ASSEMBLY_FIELD;
	m_pBMOutAssemFieldPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutAssemFieldPage->m_psp.pszTitle = "소조립마크별 현장설치";
	// [19]단면별 집계표(기본도정산용)
	m_pBMOutSectionCompPage = new CBMOutSangPage;
	m_pBMOutSectionCompPage->m_pBMOutSheet = this;
	m_pBMOutSectionCompPage->m_nMakeFlag	= MAKE_BM_SECTION_COMP;
	m_pBMOutSectionCompPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutSectionCompPage->m_psp.pszTitle = "단면타입별 정산용";
	// [20]부재별 집계표(기본도정산용)
	m_pBMOutBujeCompPage = new CBMOutSangPage;
	m_pBMOutBujeCompPage->m_pBMOutSheet = this;
	m_pBMOutBujeCompPage->m_nMakeFlag	= MAKE_BM_BUJE_COMP;
	m_pBMOutBujeCompPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBujeCompPage->m_psp.pszTitle = "부재별 정산용";
	// [21]기본도 정산용 집계표
	m_pBMOutBasicCompPage = new CBMOutSangPage;
	m_pBMOutBasicCompPage->m_pBMOutSheet = this;
	m_pBMOutBasicCompPage->m_nMakeFlag	= MAKE_BM_BASIC_COMP;
	m_pBMOutBasicCompPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBasicCompPage->m_psp.pszTitle = "기본도 정산용";

	////////////////////////////////////////////////////////////
	m_pBMOutSectionTypePage = new CBMOutSangPage;
	m_pBMOutSectionTypePage->m_pBMOutSheet = this;
	m_pBMOutSectionTypePage->m_nMakeFlag	= MAKE_BM_SECTIONTYPE;
	m_pBMOutSectionTypePage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutSectionTypePage->m_psp.pszTitle = "단면타입별";

	m_pBMOutBujePage = new CBMOutSangPage;
	m_pBMOutBujePage->m_pBMOutSheet = this;
	m_pBMOutBujePage->m_nMakeFlag	= MAKE_BM_BUJE;
	m_pBMOutBujePage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBujePage->m_psp.pszTitle = "부재별";

	m_pBMOutBujeTotalPage = new CBMOutSangPage;
	m_pBMOutBujeTotalPage->m_pBMOutSheet = this;
	m_pBMOutBujeTotalPage->m_nMakeFlag	= MAKE_BM_BUJE_TOTAL;
	m_pBMOutBujeTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBujeTotalPage->m_psp.pszTitle = "부재별 총괄";

	m_pBMOutGirderTotalPage = new CBMOutSangPage;
	m_pBMOutGirderTotalPage->m_pBMOutSheet = this;
	m_pBMOutGirderTotalPage->m_nMakeFlag	= MAKE_BM_GIRDER_TOTAL;
	m_pBMOutGirderTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutGirderTotalPage->m_psp.pszTitle = "거더별 총괄";

	m_pBMOutBujeKindTotalPage = new CBMOutSangPage;
	m_pBMOutBujeKindTotalPage->m_pBMOutSheet = this;
	m_pBMOutBujeKindTotalPage->m_nMakeFlag	= MAKE_BM_BUJEKIND_TOTAL;
	m_pBMOutBujeKindTotalPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutBujeKindTotalPage->m_psp.pszTitle = "부재종류별 총괄";

	m_pBMOutDeductSangPage = new CBMOutSangPage;
	m_pBMOutDeductSangPage->m_pBMOutSheet = this;
	m_pBMOutDeductSangPage->m_nMakeFlag	= MAKE_BM_DEDUCT_SANG;
	m_pBMOutDeductSangPage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutDeductSangPage->m_psp.pszTitle = "상세별 공제";

	m_pBMOutDeductBujePage = new CBMOutSangPage;
	m_pBMOutDeductBujePage->m_pBMOutSheet = this;
	m_pBMOutDeductBujePage->m_nMakeFlag	= MAKE_BM_DEDUCT_BUJE;
	m_pBMOutDeductBujePage->m_psp.dwFlags |= PSP_USETITLE;
	m_pBMOutDeductBujePage->m_psp.pszTitle = "부재별 공제";

	m_bNoSize = FALSE;
	m_MenuMakeButton.LoadMenu (IDR_MENUBUT_BMMAKE);	
	m_sXLFileName = _T("");
	m_nDimType	= DIM_BASIC;;
}

CBMOutSheet::~CBMOutSheet()
{
	// 강제집계표 제작용
	delete	m_pBMOutSangPage;				// [01]기본도의 강재집계표
	delete	m_pBMOutStandardSangPage;		// [02]표준집계표
	delete	m_pBMOutStandardShopPage;		// [03]표준집계표(제작용)
	delete	m_pBMOutStandardShopType2Page;	// [03]표준집계표(제작용)
	delete	m_pBMOutThickPage;				// [04]강종별 두께별집계표
	delete	m_pBMOutThickSangPage;			// [04]강종별 두께별집계표
	delete	m_pBMOutBujeMarkSangPage;		// [05]부재마크별 상세집계표
	delete	m_pBMOutBujeMarkTotalPage;		// [06]부재마크별 총괄집계표
	delete	m_pBMOutBujeMarkDeductPage;		// [07]부재마크별 공제집계표
	delete	m_pBMOutCarPage;				// [08]차량적재 상세집계표
	delete	m_pBMOutCarTotalPage;			// [09]차량적재 총괄집계표
	delete	m_pBMOutGrandAssemSangPage;		// [10]대조립별 상세집계표
	delete	m_pBMOutGrandAssemTotalPage;	// [11]대조립별 총괄집계표
	delete	m_pBMOutMidAssemSangPage;		// [12]중조립별 상세집계표
	delete	m_pBMOutMidAssemTotalPage;		// [13]중조립별 총괄집계표
	delete	m_pBMOutAssemSangPage;			// [14]소조립별 상세집계표
	delete	m_pBMOutAssemTotalPage;			// [15]소조립별 총괄집계표
	delete	m_pBMOutAssemDeductPage;		// [16]소조립별 공제집계표
	delete	m_pBMOutAssemFactPage;			// [17]소조립별 공장제작용집계표
	delete	m_pBMOutAssemFieldPage;			// [18]소조립별 현장설치용집계표
	delete	m_pBMOutSectionCompPage;		// [19]단면별 집계표(기본도정산용)
	delete	m_pBMOutBujeCompPage;			// [20]부재별 집계표(기본도정산용)
	delete	m_pBMOutBasicCompPage;			// [21]기본도 정산용 집계표
	// 강제집계표 설계용
	delete	m_pBMOutSectionTypePage;
	delete	m_pBMOutBujePage;
	delete	m_pBMOutBujeTotalPage;
	delete  m_pBMOutGirderTotalPage;
	delete  m_pBMOutBujeKindTotalPage;
	delete	m_pBMOutDeductSangPage;
	delete	m_pBMOutDeductBujePage;
	m_MenuMakeButton.DestroyMenu();
}


BEGIN_MESSAGE_MAP(CBMOutSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CBMOutSheet)
	ON_CBN_DROPDOWN(IDC_BMSHEET_MAKEGANGJE, OnButtonMakeCreatorDropDown)
	ON_CBN_DROPDOWN(IDC_BMSHEET_SAVE, OnButtonSaveDropDown)
	ON_CBN_DROPDOWN(IDC_BMSHEET_PRINT, OnButtonPrintDropDown)

	ON_BN_CLICKED(IDC_BMSHEET_MAKEGANGJE, OnButtonMakeCreator)
	ON_BN_CLICKED(IDC_BMSHEET_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDR_PRINT_PREVIEW, OnButtonPrintPreview)
	ON_BN_CLICKED(IDR_PRINT_THIS, OnButtonPrintThis)

	ON_WM_DESTROY()
	ON_MESSAGE(WM_BM_PROGRESS,OnProgress)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMOutSheet message handlers

BOOL CBMOutSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();	
    SetWindowText("강재집계표");

	return bResult;
}

BOOL CBMOutSheet::Create(CWnd* pParentWnd,long nMakeFlag, long nARoadFlag, DIMTYPE nDimType)
{
	DWORD flag = WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_THICKFRAME
				| WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	m_nMakeBMFlag = nMakeFlag;
	m_nARoadFlag  = nARoadFlag/*nARoadFlag*/;
	m_nDimType    = nDimType;

	if(GetSafeHwnd())// 이미 로드시
		ShowWindow(SW_SHOWNORMAL);
	else	
	{
		if(!GetPageCount())
		{
			if(nMakeFlag & MAKE_BM_SANG)					AddPage(m_pBMOutSangPage);
			if(nMakeFlag & MAKE_BM_BASIC_COMP)				AddPage(m_pBMOutBasicCompPage);
//			if(nMakeFlag & MAKE_BM_STDANDARD_SANG)			AddPage(m_pBMOutStandardSangPage);
			if(nMakeFlag & MAKE_BM_STDANDARD_SHOP)			AddPage(m_pBMOutStandardShopPage);
//			if(nMakeFlag & MAKE_BM_STDANDARD_SHOP_TYPE2)	AddPage(m_pBMOutStandardShopType2Page);
			if(nMakeFlag & MAKE_BM_THICK_SANG)				AddPage(m_pBMOutThickSangPage);
			if(nMakeFlag & MAKE_BM_THICK)					AddPage(m_pBMOutThickPage);
//			if(nMakeFlag & MAKE_BM_BUJEMARK_SANG)			AddPage(m_pBMOutBujeMarkSangPage);
//			if(nMakeFlag & MAKE_BM_BUJEMARK_TOTAL)			AddPage(m_pBMOutBujeMarkTotalPage);
//			if(nMakeFlag & MAKE_BM_BUJEMARK_DEDUCT)			AddPage(m_pBMOutBujeMarkDeductPage);
			if(nMakeFlag & MAKE_BM_CAR)						AddPage(m_pBMOutCarPage);
			if(nMakeFlag & MAKE_BM_CARTOTAL)				AddPage(m_pBMOutCarTotalPage);
//			if(nMakeFlag & MAKE_BM_GRAND_ASSEMBLY_SANG)		AddPage(m_pBMOutGrandAssemSangPage);
//			if(nMakeFlag & MAKE_BM_GRAND_ASSEMBLY_TOTAL)	AddPage(m_pBMOutGrandAssemTotalPage);
//			if(nMakeFlag & MAKE_BM_MID_ASSEMBLY_SANG) 		AddPage(m_pBMOutMidAssemSangPage);
//			if(nMakeFlag & MAKE_BM_MID_ASSEMBLY_TOTAL)		AddPage(m_pBMOutMidAssemTotalPage);
//			if(nMakeFlag & MAKE_BM_ASSEMBLY_SANG)			AddPage(m_pBMOutAssemSangPage);
//			if(nMakeFlag & MAKE_BM_ASSEMBLY_TOTAL)			AddPage(m_pBMOutAssemTotalPage);
//			if(nMakeFlag & MAKE_BM_ASSEMBLY_DEDUCT)			AddPage(m_pBMOutAssemDeductPage);
//			if(nMakeFlag & MAKE_BM_ASSEMBLY_FACT)			AddPage(m_pBMOutAssemFactPage);
//			if(nMakeFlag & MAKE_BM_ASSEMBLY_FIELD)			AddPage(m_pBMOutAssemFieldPage);
//			if(nMakeFlag & MAKE_BM_SECTION_COMP)			AddPage(m_pBMOutSectionCompPage);
			if(nMakeFlag & MAKE_BM_BUJE_COMP)				AddPage(m_pBMOutBujeCompPage);

			//
			if(nMakeFlag & MAKE_BM_SECTIONTYPE)				AddPage(m_pBMOutSectionTypePage);
			if(nMakeFlag & MAKE_BM_BUJE)					AddPage(m_pBMOutBujePage);
			if(nMakeFlag & MAKE_BM_BUJE_TOTAL)				AddPage(m_pBMOutBujeTotalPage);
			if(nMakeFlag & MAKE_BM_GIRDER_TOTAL)			AddPage(m_pBMOutGirderTotalPage);
			if(nMakeFlag & MAKE_BM_BUJEKIND_TOTAL)			AddPage(m_pBMOutBujeKindTotalPage);
			if(nMakeFlag & MAKE_BM_DEDUCT_SANG)				AddPage(m_pBMOutDeductSangPage);
			if(nMakeFlag & MAKE_BM_DEDUCT_BUJE)				AddPage(m_pBMOutDeductBujePage);
		}
		CPropertySheet::Create(pParentWnd,flag);
	}

	return TRUE;
}


void CBMOutSheet::Start(long nMakeType)
{
	CRect rect;
	GetClientRect(&rect);
	if(nMakeType==IDR_BMMAKE_CREATOR)
	{
		m_Progress.ShowWindow(SW_SHOW);
		m_ProgressText.MoveWindow(rect.left+560,rect.bottom-17,190,20);
		m_ProgressText.ShowWindow(SW_SHOW);
	}
	if(nMakeType==IDR_BMMAKE_FILEOPEN_XL)
	{
		m_ProgressText.MoveWindow(rect.left+310,rect.bottom-17,440,20);
		m_ProgressText.ShowWindow(SW_SHOW);
		CString str1,str2;
		str1 = m_sXLFileName.Right(m_sXLFileName.GetLength()-m_sXLFileName.ReverseFind('\\')-1);
		str2.Format("%s 파일을 여는중....",str1);
		m_ProgressText.SetWindowText(str2);
	}
	m_CloseButton.SetWindowText("취소(&S)");

	m_MakeButton.EnableWindow(FALSE);
	m_SaveButton.EnableWindow(FALSE);
	m_PrintButton.EnableWindow(FALSE);
	m_CountText.ShowWindow(SW_HIDE);
	m_WeightText.ShowWindow(SW_HIDE);
	m_UnitWeightText.ShowWindow(SW_HIDE);
}


void CBMOutSheet::Finish()
{
	m_Progress.ShowWindow(SW_HIDE);
	m_ProgressText.ShowWindow(SW_HIDE);
	m_CloseButton.SetWindowText("닫기(&C)");

	m_MakeButton.EnableWindow(TRUE);
	m_SaveButton.EnableWindow(TRUE);
	m_PrintButton.EnableWindow(TRUE);

	for(long n = 0; n < GetPageCount(); n++)
		((CBMOutSangPage*)GetPage(n))->DrawGrid();
	m_CountText.ShowWindow(SW_SHOW);
	m_WeightText.ShowWindow(SW_SHOW);
	m_UnitWeightText.ShowWindow(SW_SHOW);
	GetActivePage()->OnSetActive();
}

// 생성버튼
void CBMOutSheet::OnButtonMakeCreator()
{	
	m_pDataManage->m_pBMStandard->SetWorkValue(0,"",m_nDimType,m_nMakeBMFlag);
	
	Start(IDR_BMMAKE_CREATOR);
	
	m_pDataManage->m_pBMStandard->Begin(this,WM_BM_PROGRESS);
}

void CBMOutSheet::OnButtonMakeCreatorDropDown()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENUBUT_BMMAKE);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CXTPButton* pButton = DYNAMIC_DOWNCAST(CXTPButton, GetDlgItem(IDC_BMSHEET_MAKEGANGJE));
	ASSERT(pButton);
	
	DWORD dwStyle = pButton->GetPushButtonStyle();
	ASSERT(pButton->IsDropDownStyle());
	
	CXTPWindowRect rect(pButton);
	
	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(),
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left,
		dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom,
		0, m_hWnd, 0);
	
	switch (nCmd)
	{
	case IDR_BMMAKE_CREATOR:
		OnButtonMakeCreator();
		break;
		
	case IDR_BMMAKE_FILEOPEN_XL:
		OnButtonMakeFileOpenXL();
		break;
	}
}

//불러오기
void CBMOutSheet::OnButtonMakeFileOpenXL()
{
	CXLControl excel;
	double dVersion = excel.GetVersion();

	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);


	dlg.m_ofn.lpstrTitle = "엑셀 파일열기";

	if(IDOK == dlg.DoModal())
	{
		m_sXLFileName = dlg.GetPathName();

		m_pDataManage->m_pBMStandard->SetWorkValue(1, m_sXLFileName, m_nDimType, m_nMakeBMFlag);

		Start(IDR_BMMAKE_FILEOPEN_XL);

		m_pDataManage->m_pBMStandard->Begin(this,WM_BM_PROGRESS);
	}
}

// 저장 버튼
void CBMOutSheet::OnButtonSaveDropDown()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_BMOUT_SAVE);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CXTPButton* pButton = DYNAMIC_DOWNCAST(CXTPButton, GetDlgItem(IDC_BMSHEET_SAVE));
	ASSERT(pButton);
	
	DWORD dwStyle = pButton->GetPushButtonStyle();
	ASSERT(pButton->IsDropDownStyle());
	
	CXTPWindowRect rect(pButton);
	
	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(),
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left,
		dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom,
		0, m_hWnd, 0);
	
	switch (nCmd)
	{
	case IDR_BMMAKE_FILESAVE_XL:
		OnButtonFileSaveXL();
		break;
		
	case IDR_BMMAKE_FILESAVEALL_XL:
		OnButtonFileSaveAllXL();
		break;
	case IDR_BMMAKE_OUTFILEOPEN_XL:
		//OnButtonope
		break;
	}
}

void CBMOutSheet::OnButtonFileSaveXL()
{
	CBMOutSangPage* pPage = (CBMOutSangPage*)GetActivePage();
	
	CXLControl excel;
	double dVersion = excel.GetVersion();

	CDocument* pDoc = m_pDataManage->m_pBMStandard->m_pTStandard->m_pDoc;
	CString sFileName = pDoc->GetTitle();
	if( sFileName.Find('.') >= 0 )
	{
		int pos = sFileName.Find('.');
		sFileName = sFileName.Left(pos);
	}
	sFileName += "_강재_";
	sFileName += pPage->m_psp.pszTitle;

	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);



	dlg.m_ofn.lpstrTitle = "엑셀 파일저장";
	if(IDOK == dlg.DoModal())
	{
		m_sXLFileName = dlg.GetPathName();
		CMsgDlg Dlg(this);
		Dlg.SetCmdType(CMD_SAVE);
		Dlg.AddGridCtrl(pPage->m_pGrid);
		Dlg.AddSaveSheetName(pPage->m_psp.pszTitle);
		Dlg.SetSaveFileName(m_sXLFileName);
		Dlg.Run();					
	}
}

void CBMOutSheet::OnButtonFileSaveAllXL()
{
	CDocument* pDoc = m_pDataManage->m_pBMStandard->m_pTStandard->m_pDoc;
	CString sFileName = pDoc->GetTitle()+"_강재";
	if( sFileName.Find('.') >= 0 )
	{
		int pos = sFileName.Find('.');
		sFileName = sFileName.Left(pos);
	}
	sFileName += "_강재집계표";

	CXLControl excel;
	double dVersion = excel.GetVersion();
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel 통합 문서 (*.xls)|*.xls|모든파일 (*.*)|*.*||") : _T("Microsoft Excel 통합 문서 (*.xlsx)|*.xlsx|모든파일 (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	dlg.m_ofn.lpstrTitle = "엑셀 파일저장";
	if(IDOK == dlg.DoModal())
	{
		m_sXLFileName = dlg.GetPathName();
		CMsgDlg Dlg(this);
		Dlg.SetCmdType(CMD_SAVE);
		for(long i=0; i<GetPageCount(); i++)
		{
			CBMOutSangPage* pPage = (CBMOutSangPage*)GetPage(i);
			if(!pPage->m_pGrid->GetSafeHwnd())
			{
				SetActivePage(pPage);			
				while(pPage->GetWinThread())
				{
					MSG msg;	
					while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
					{
						AfxGetThread()->PumpMessage();							
					}			
				}
			}
			Dlg.AddGridCtrl(pPage->m_pGrid);
			Dlg.AddSaveSheetName(pPage->m_psp.pszTitle);
		}
		Dlg.SetSaveFileName(m_sXLFileName);
		Dlg.Run();
	}
}

// 출력버튼 
void CBMOutSheet::OnButtonPrintDropDown()
{
	CMenu menu;
	menu.LoadMenu(IDR_MENU_BMOUT_PRINT);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	
	CXTPButton* pButton = DYNAMIC_DOWNCAST(CXTPButton, GetDlgItem(IDC_BMSHEET_PRINT));
	ASSERT(pButton);
	
	DWORD dwStyle = pButton->GetPushButtonStyle();
	ASSERT(pButton->IsDropDownStyle());
	
	CXTPWindowRect rect(pButton);
	
	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(),
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left,
		dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom,
		0, m_hWnd, 0);
	
	switch (nCmd)
	{
	case IDR_PRINT_PREVIEW:
		OnButtonPrintPreview();
		break;
	case IDR_PRINT_THIS:
		OnButtonPrintThis();
		break;
	}
}

void CBMOutSheet::OnButtonPrintPreview()
{
	CBMOutSangPage* pPage = (CBMOutSangPage*)GetActivePage();
	pPage->m_pGrid->PrintPreview();
}

void CBMOutSheet::OnButtonPrintThis()
{
	CBMOutSangPage* pPage = (CBMOutSangPage*)GetActivePage();	
	pPage->m_pGrid->Print();
}

// 닫기 버튼
void CBMOutSheet::OnButtonClose()
{
	// 스레드가 진행중인 경우	
	if(m_pDataManage->GetBMStandard()->GetWinThread())	
	{	m_pDataManage->GetBMStandard()->Abort();	return;	}
	
	if(m_pDataManage->GetBMStandard()->m_BMList_Sang.GetCount())
		AfxGetMainWnd()->SendMessage(WM_PAGE_RESET,7);
	DestroyWindow();	
}


void CBMOutSheet::OnDestroy() 
{
	CPropertySheet::OnDestroy();

	m_SheetFont.DeleteObject();
}

LRESULT CBMOutSheet::OnProgress(WPARAM wp, LPARAM lp)
{
	int nValue = (int)wp;

	if(nValue == 0) // progress
	{
		CString sFmt;
		int nPos = (int)lp;
		sFmt.Format("%ld%%",nPos);
		m_Progress.SetPos(nPos);
	}
	else if(nValue == 1)	// PaneText
	{
		LPCTSTR lpstr = (LPCTSTR)(lp);
		m_ProgressText.SetWindowText(lpstr);
	}
	else if(nValue == 4)	// 종료 or 정지
	{
		Finish();
		AfxGetMainWnd()->SendMessage(WM_USER+44,/*PAGE_BM*/7,0);
	}

	return FALSE;
}

BOOL CBMOutSheet::DestroyWindow() 
{
	// 스레드가 진행중인 경우
	if(m_pDataManage->GetBMStandard()->GetWinThread())	
	{	m_pDataManage->GetBMStandard()->Abort();	return TRUE;	}

	return CPropertySheet::DestroyWindow();	
}



void CBMOutSheet::OnSize(UINT nType, int cx, int cy) 
{
	CPropertySheet::OnSize(nType, cx, cy);

	if(m_bNoSize || cx==0 || cy==0) return;

	CRect Rect;
	const long ButtonWidth		=  90;
	const long ButtonHeight		=  21;
	const long ProgressWidth	= 200;
	const long ProgressTxtWidth = 200;
	const CRect ButtonRect(0,0,ButtonWidth,ButtonHeight);
	const CRect ProgressRect(0,0,ProgressWidth,ButtonHeight);
	const CRect ProgressTxtRect(0,0,ProgressTxtWidth,ButtonHeight);
	const CRect TextRect(0,0,ProgressTxtWidth*2/3,ButtonHeight);

	//  Create
	CRect BRect;
	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(10,cy - ButtonHeight - 2);
	if(m_MakeButton.GetSafeHwnd())
		m_MakeButton.MoveWindow(&BRect);

	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(10 + ButtonWidth + 10,cy - ButtonHeight - 2);
	if(m_SaveButton.GetSafeHwnd())
		m_SaveButton.MoveWindow(&BRect);

	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(10 + ButtonWidth*2 + 20,cy - ButtonHeight - 2);
	if(m_PrintButton.GetSafeHwnd())
		m_PrintButton.MoveWindow(&BRect);

	BRect.CopyRect(&ButtonRect);
	BRect.OffsetRect(cx - ButtonWidth - 2,cy - ButtonHeight - 2);
	if(m_CloseButton.GetSafeHwnd())
		m_CloseButton.MoveWindow(&BRect);

	// Progress
	BRect.CopyRect(&ProgressRect);
	BRect.OffsetRect(50 +3*ButtonWidth+30,cy - ButtonHeight - 2);
	if(m_Progress.GetSafeHwnd())
		m_Progress.MoveWindow(&BRect);

	// Progress text
	BRect.CopyRect(&ProgressTxtRect);
	BRect.OffsetRect(50 + 3*ButtonWidth+ ProgressWidth + 40,cy - ButtonHeight - 2);
	if(m_ProgressText.GetSafeHwnd())
		m_ProgressText.MoveWindow(&BRect);

	// m_CountText
	BRect.CopyRect(&TextRect);
	BRect.OffsetRect(50 +3*ButtonWidth, cy - ButtonHeight + 3);
	if(m_CountText.GetSafeHwnd())
		m_CountText.MoveWindow(&BRect);

	// m_WeightText
	BRect.CopyRect(&TextRect);
	BRect.OffsetRect(50 +3*ButtonWidth+130, cy - ButtonHeight + 3);
	if(m_WeightText.GetSafeHwnd())
		m_WeightText.MoveWindow(&BRect);

	// m_UnitWeightText
	BRect = CRect(0,0,ProgressTxtWidth*3/4,ButtonHeight);
	BRect.OffsetRect(50 +3*ButtonWidth+260, cy - ButtonHeight + 3);
	if(m_WeightText.GetSafeHwnd())
		m_UnitWeightText.MoveWindow(&BRect);

	CTabCtrl* pTab = GetTabControl();
	if(pTab->GetSafeHwnd())		
		pTab->SetWindowPos(NULL,0,0,cx-13,cy-32,SWP_NOMOVE);		

	if(GetSafeHwnd() && pTab->GetSafeHwnd())
	for(long n = 0; n < GetPageCount(); n++)
	{
		CBMOutSangPage* pPage = (CBMOutSangPage*)GetPage(n);
		if(pPage->GetSafeHwnd())
			pPage->PostMessage(WM_SIZE);
	}
}

int CBMOutSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 폰트 생성
	m_SheetFont.CreateFont( 12,
		0,//int nWidth,
		0,//int nEscapement, 폰트의 각( degree, 직각인경우 900)
		0,//int nOrientation,한문자의 각 
		FW_NORMAL,// 폰트의 두께//int nWeight,
		FALSE,//BYTE bItalic, 
		FALSE,//BYTE bUnderline, 
		FALSE,//BYTE cStrikeOut, 
		DEFAULT_CHARSET,//BYTE nCharSet, DEFAULT_CHARSET,ANSI_CHARSET
		OUT_DEVICE_PRECIS,//BYTE nOutPrecision, OUT_DEVICE_PRECIS,OUT_DEFAULT_PRECIS
		CLIP_LH_ANGLES,//BYTE nClipPrecision, CLIP_LH_ANGLES,CLIP_DEFAULT_PRECIS
		DEFAULT_QUALITY,//BYTE nQuality, 
		VARIABLE_PITCH|FF_SWISS,//BYTE nPitchAndFamily, 
		"굴림체"//LPCTSTR lpszFacename 
		);

	///////////////////////////////////////////////////////////////////
	// 동적 버튼 
	DWORD BFlag = WS_CHILD | WS_VISIBLE|BS_PUSHBUTTON;
	CRect rc;
	GetClientRect(&rc);
	rc.bottom += 100;
	m_bNoSize = TRUE;
	SetWindowPos(NULL,0,0,rc.Width(),rc.Height(),SWP_NOMOVE);
	m_bNoSize = FALSE;
	GetClientRect(&rc);

	//  Create
	CRect BRect(0,0,0,0);
	m_MakeButton.Create("생성(&R)",BFlag,BRect,this,IDC_BMSHEET_MAKEGANGJE);
	m_MakeButton.SetFont(&m_SheetFont);
	m_MakeButton.ShowWindow(SW_SHOW);
	m_MakeButton.SetPushButtonStyle(xtpButtonDropDown);


	m_SaveButton.Create("저장(&S)",BFlag,BRect,this,IDC_BMSHEET_SAVE);
	m_SaveButton.SetFont(&m_SheetFont);
	m_SaveButton.ShowWindow(SW_SHOW);
	m_SaveButton.ShowWindow(SW_SHOW);
	m_SaveButton.SetPushButtonStyle(xtpButtonDropDown);

	m_PrintButton.Create("인쇄(&P)",BFlag,BRect,this,IDC_BMSHEET_PRINT);
	m_PrintButton.SetFont(&m_SheetFont);
	m_PrintButton.ShowWindow(SW_SHOW);
	m_PrintButton.ShowWindow(SW_SHOW);
	m_PrintButton.SetPushButtonStyle(xtpButtonDropDown);

	m_CloseButton.Create("닫기(&C)",BFlag,BRect,this,IDC_BMSHEET_CLOSE);
	m_CloseButton.SetFont(&m_SheetFont);
	m_CloseButton.ShowWindow(SW_SHOW);

	// Progress
	BFlag = WS_CHILD | WS_VISIBLE;
	m_Progress.Create(BFlag,BRect,this,IDC_BMSHEET_PRGRESS);
	m_Progress.SetFont(&m_SheetFont);
	m_Progress.ShowWindow(SW_HIDE);//SW_HIDE

	// Progress text
	m_ProgressText.Create("",BFlag,BRect,this,IDC_BMSHEET_PRGRESSTXT);
	m_ProgressText.SetFont(&m_SheetFont);
	m_ProgressText.ShowWindow(SW_HIDE);

	// Count text
	m_CountText.Create("",BFlag,BRect,this,IDC_DET_BMRECCOUNT);
	m_CountText.SetFont(&m_SheetFont);
	m_CountText.ShowWindow(SW_HIDE);

	// Weight text
	m_WeightText.Create("",BFlag,BRect,this,IDC_DET_BMWEIGHT);
	m_WeightText.SetFont(&m_SheetFont);
	m_WeightText.ShowWindow(SW_HIDE);

	// Unit Weight text
	m_UnitWeightText.Create("",BFlag,BRect,this,IDC_DET_BMUNITWEIGHT);
	m_UnitWeightText.SetFont(&m_SheetFont);
	m_UnitWeightText.ShowWindow(SW_HIDE);
	
	CenterWindow( GetParent() );
	DWORD lp = MAKELONG(rc.Width(),rc.Height());

	EnableStackedTabs(FALSE);	
	return 0;
}

