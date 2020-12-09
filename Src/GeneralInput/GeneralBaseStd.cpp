// PPCInputStd.cpp: implementation of the CGeneralBaseStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeneralInput.h"
#include "GeneralBaseStd.h"
#include "../APlateOutCalc\APlateOutCalcStd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL CGeneralBaseStd::m_bInclude_Module_Deck	 = TRUE;
BOOL CGeneralBaseStd::m_bInclude_Module_TUGirder = FALSE;
BOOL CGeneralBaseStd::m_bInclude_Module_DesignStd = FALSE;

BOOL CGeneralBaseStd::IsModuleDeck() { return m_bInclude_Module_Deck; }
BOOL CGeneralBaseStd::IsModuleTUGirder() { return m_bInclude_Module_TUGirder; }
BOOL CGeneralBaseStd::IsModuleDesignStd() { return m_bInclude_Module_DesignStd; }
void CGeneralBaseStd::SetModuleDeck(BOOL bInclude) { m_bInclude_Module_Deck = bInclude; }
void CGeneralBaseStd::SetModuleTUGirder(BOOL bInclude)	{ m_bInclude_Module_TUGirder = bInclude; }
void CGeneralBaseStd::SetModuleDesignStd(BOOL bInclude) { m_bInclude_Module_DesignStd = bInclude; }


CGeneralBaseStd::CGeneralBaseStd(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd) 
	: m_pDataManage(pDataManage), m_pCalcStd(pCalcStd)
{	
	m_pCurruntDlg		= NULL;		// 현재 다이얼로그	
	m_nSaveMenuIDD		= -1;		// 저장된 메뉴 ID
	m_nHistoryIdx		= -1;		

	InsertDialog(ID_GNINPUT_0 , &m_SelLineDlg				,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_1 , &m_BaseInputDlg				,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_2 , &m_BaseInfoDlg				,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_3 , &m_HDanDlg					,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_4 , &m_JiganGusengDlg			,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_5 , &m_JijumAngleDlg			,IDD_MENU_GEN);	
	InsertDialog(ID_GNINPUT_6 , &m_JijumStiffDlg			,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_7 , &m_JackupPositionDlg		,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_8, &m_StartPointJewonDlg		,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_9, &m_ByunDanmyunJewonDlg		,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_10, &m_VBracingPositionDlg		,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_11, &m_VStiffPositionDlg		,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_12, &m_HBracingPositionDlg		,IDD_MENU_GEN);	
	InsertDialog(ID_GNINPUT_13, &m_JRibPositionDlg			,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_14, &m_DanmyunTypeDlg			,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_15, &m_FieldJoinPositionDlg		,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_16, &m_FactJoinPositionDlg		,IDD_MENU_GEN);
	InsertDialog(ID_GNINPUT_17, &m_SlabJewonDlg				,IDD_MENU_GEN);

	// 상세입력
	InsertDialog(ID_DETINPUT_1, &m_DetEtcInputDlg			,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_2, &m_DetFieldJoinDlg			,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_3, &m_DetCrossBeamTypeDlg		,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_4, &m_DetVBracingTypeDlg		,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_5, &m_DetHBracingDlg			,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_6, &m_DetVStiffJewonDlg		,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_7, &m_DetCamberJewonDlg		,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_8, &m_DetWebHStiffPosDlg		,IDD_MENU_DETAIL);
	InsertDialog(ID_DETINPUT_9, &m_DetSHConnectorVPosDlg	,IDD_MENU_DETAIL);

	// 계산입력
	InsertDialog(ID_CLINPUT_1, &m_DesignBase      			,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_2, &m_CalcSlabSetupDlg			,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_3, &m_CalcCantileverDeadLoadDlg	,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_4, &m_CalcCantileverModeling	,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_5, &m_CalcCantileverLoadCombo	,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_6, &m_CalcCantileverModelResult	,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_7, &m_CalcCantileverLoadDlg		,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_8, &m_CalcLossOfPrestress       ,IDD_MENU_CALC);	///< 프리스트레스 손실
	InsertDialog(ID_CLINPUT_9, &m_CalcFloorSlabStress		,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_10, &m_CalcCantileverRebarDlg   ,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_11, &m_CalcUltimateLoadDlg		,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_12, &m_CalcCantileverDroopDlg   ,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_13, &m_CalcSectionConceptDlg	,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_14, &m_CalcSecForceDivCalcDlg   ,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_15, &m_CalcSecForceDivResultDlg	,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_16, &m_CalcWindForceDlg			,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_17, &m_CalcModelingSetupDlg		,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_18, &m_CalcFrameComboLoadDlg	,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_19, &m_CalcModelingResultDlg    ,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_20, &m_CalcInfluenceKDlg        ,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_21, &m_CalcSecStressDlg         ,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_22, &m_CalcSecOutlineDlg		,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_23, &m_CalcWarping              ,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_24, &m_CalcDisplacement  		,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_25, &m_CalcFatigue       		,IDD_MENU_CALC);
	InsertDialog(ID_CLINPUT_26, &m_CalcWeld          		,IDD_MENU_CALC);

	// 배근도입력
	InsertDialog(ID_DECK_1,		&m_DeckBasicDataDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_2,		&m_DeckRebarInfoDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_3,		&m_DeckMainRebarDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_4,		&m_DeckDistriRebarCTCDlg,	IDD_MENU_DECK);
	InsertDialog(ID_DECK_5,		&m_DeckDistriRebarDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_6,		&m_DeckMainRebarCTCDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_7,		&m_DeckMainSkewRebarCTCDlg,	IDD_MENU_DECK);
	InsertDialog(ID_DECK_8,		&m_DeckSabogangCTCDlg,		IDD_MENU_DECK);
	if(IsModuleTUGirder())
		InsertDialog(ID_DECK_9,		&m_DeckBindConcRebarDlg,	IDD_MENU_DECK);
	InsertDialog(ID_DECK_10,	&m_DeckCrossStringerDlg,	IDD_MENU_DECK);		
	InsertDialog(ID_DECK_11,	&m_DeckGuardFenceDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_12,	&m_DeckPlanRebarDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_13,	&m_DeckEtcRebarDlg,			IDD_MENU_DECK);
	InsertDialog(ID_DECK_14,	&m_DeckRebarJewonDlg,		IDD_MENU_DECK);
	InsertDialog(ID_DECK_15,	&m_DeckBMOptionDlg,			IDD_MENU_DECK);
	
	m_BMOutSheet.m_pDataManage = m_pDataManage;
	m_PaintOutSheet.m_pDataManage = m_pDataManage;
	m_WeldOutSheet.m_pDataManage = m_pDataManage;

	if(IsModuleTUGirder())	m_menuInput.LoadMenu(IDR_MENU_INPUT_FULL);
	else					m_menuInput.LoadMenu(IDR_MENU_INPUT_NOT_TU);

	m_TotalViewDlg.m_pStd = this;
	
	m_MenuBearingDB.LoadMenu(IDR_MENU_BEARINGDB);

}

CGeneralBaseStd::~CGeneralBaseStd()
{
	m_DialogList.RemoveAll();

	for(long n=0; n<IDD_MENU_SIZE; n++)
		m_IDDArray[n].RemoveAll();
}


CLineInfoApp *CGeneralBaseStd::GetLineInfoApp() const
{ 
	return m_pDataManage->GetLineInfoApp(); 
}

CPlateBridgeApp *CGeneralBaseStd::GetBridge() const
{
	return m_pDataManage->GetBridge();
}

//<summary>
//다이얼로그 초기화
//</summary>
void CGeneralBaseStd::InsertDialog(UINT nID, CGeneralBaseDlg *pDlg, long nType)
{
	m_IDDArray[nType].Add(pDlg->GetIDD());
	pDlg->SetParentStd(this);
	pDlg->SetMenuID(nID);	
	m_DialogList.Add(pDlg);	
}


void CGeneralBaseStd::HideCurrentDialog()
{
	if(m_pCurruntDlg && m_pCurruntDlg->IsWindowVisible())
	{
		m_pCurruntDlg->m_pView->SetGL(FALSE);	
		m_pCurruntDlg->m_pView->SetDomBkColor(0);	
		m_pCurruntDlg->m_pView->SetOwnerMouseMode(TRUE);
		m_pCurruntDlg	= NULL;
	}
}

void CGeneralBaseStd::InsertHistory(UINT nIDD)
{
	if(m_nHistoryIdx==-1 || m_nHistoryIdx == m_DlgHistroyArray.GetUpperBound())
	{
		if(m_DlgHistroyArray.GetSize()>0 && m_DlgHistroyArray.GetAt(m_DlgHistroyArray.GetUpperBound()) == nIDD)
			return;
		m_DlgHistroyArray.Add(nIDD);
		m_nHistoryIdx = m_DlgHistroyArray.GetUpperBound();
	}
	else
	{	
		m_DlgHistroyArray.SetSize(m_nHistoryIdx+1);
		if(m_DlgHistroyArray.GetSize()>0 && m_DlgHistroyArray.GetAt(m_DlgHistroyArray.GetUpperBound()) == nIDD)
			return;
		m_DlgHistroyArray.Add(nIDD);
		m_nHistoryIdx = m_DlgHistroyArray.GetUpperBound();
	}
}

BOOL CGeneralBaseStd::IsNextHistroy()
{
	return (m_nHistoryIdx==-1 || m_nHistoryIdx>=m_DlgHistroyArray.GetUpperBound()) ? FALSE : TRUE;
}

BOOL CGeneralBaseStd::IsPrevHistory()
{
	return (m_nHistoryIdx==-1 || m_nHistoryIdx==0) ? FALSE : TRUE;
}

void CGeneralBaseStd::GoNextHistory()
{
	if(m_nHistoryIdx>=m_DlgHistroyArray.GetUpperBound()) return;
	UINT nIDD = m_DlgHistroyArray.GetAt(++m_nHistoryIdx);	
	
	if(m_pCurruntDlg)
	{
		CGeneralBaseDlg *pDlg = GetDlgOfIDD(nIDD);
		OnShowDialog(pDlg,m_pCurruntDlg->GetView(),FALSE,NEXT_PROCESS,FALSE);
	}
}

void CGeneralBaseStd::GoPrevHistory()
{
	if(m_nHistoryIdx<=0) return;
	UINT nIDD = m_DlgHistroyArray.GetAt(--m_nHistoryIdx);

	if(m_pCurruntDlg)
	{
		CGeneralBaseDlg *pDlg = GetDlgOfIDD(nIDD);
		OnShowDialog(pDlg,m_pCurruntDlg->GetView(),FALSE,NEXT_PROCESS,FALSE);
	}
}

CGeneralBaseDlg *CGeneralBaseStd::FindValidDlg(CGeneralBaseDlg *pBaseDlg, BOOL bNext)
{	
	BOOL bStart = FALSE;
	if(bNext)
	{		
		for(long n=0; n<m_DialogList.GetSize(); n++)
		{
			if(m_DialogList[n] == pBaseDlg) bStart = TRUE;
			if(bStart && m_DialogList[n]->IsValid()) return m_DialogList[n];
		}
	}
	else
	{
		for(long n=m_DialogList.GetSize()-1; n>=0; n--)
		{
			if(m_DialogList[n] == pBaseDlg) bStart = TRUE;
			if(bStart && m_DialogList[n]->IsValid()) return m_DialogList[n];
		}
	}
	ASSERT(TRUE);
	return NULL;
}

UINT CGeneralBaseStd::OnShowDialog(CGeneralBaseDlg *pDlg, CWnd *pParent, BOOL bCenter, int nFlag, BOOL bAddHis)
{
	HideCurrentDialog();	// 현재 윈도우 감추기

	if(pDlg)
	{
		if(!pDlg->IsValid())
		{
			pDlg->SetUsedFlag(FALSE);
			AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
		}
	}

	pDlg = FindValidDlg(pDlg,nFlag == NEXT_PROCESS ? TRUE : FALSE);

	if(pDlg)
	{
		SetCurruntDialog(pDlg);
		AfxGetMainWnd()->SendMessage(WM_USER+2, pDlg->GetMenuID(), 1);

		return pDlg->GetMenuID();
	}
	else
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, 6008);
	}

	return 0;
}

BOOL CGeneralBaseStd::IsEnableDialog(UINT nID)
{
	CGeneralBaseDlg *pDlg = GetDlgOfMenuID(nID);	
	if(!pDlg) return FALSE;	

	return GetDlgOfMenuID(nID)->IsUsedFlag();	
}

void CGeneralBaseStd::MoveNextDialog() 
{	
	if(m_pCurruntDlg)
	{				
		CGeneralBaseDlg *pDlg = GetDialogByDir(TRUE);
		OnShowDialog(pDlg,m_pCurruntDlg->GetView(),FALSE,NEXT_PROCESS);
	}
}

void CGeneralBaseStd::MovePrevDialog()
{
	if(m_pCurruntDlg)
	{		
		CGeneralBaseDlg *pDlg = GetDialogByDir(FALSE);
		OnShowDialog(pDlg,m_pCurruntDlg->GetView(),FALSE,PREV_PROCESS);
	}
}

void CGeneralBaseStd::GoCurrentDlg()
{
	if(m_nSaveMenuIDD != -1)
	{
		CGeneralBaseDlg *pDlg = GetDlgOfIDD(m_nSaveMenuIDD);
		if(pDlg)
		{
			pDlg = FindValidDlg(pDlg, TRUE);
			AfxGetMainWnd()->SendMessage(WM_COMMAND, pDlg->GetMenuID(), 0);
		}
		else
			AfxGetMainWnd()->SendMessage(WM_COMMAND, m_nSaveMenuIDD, 0);
	}
}

UINT CGeneralBaseStd::ConvertResource(UINT nID)
{
	switch (nID)
	{
			case	25000:	return IDD_GEN_01_SELECTLINE;
			case	20029:	return IDD_DIALOG_MAINDLG;
			case	25001:	return IDD_GEN_02_BASEINFO;
			case	25002:	return IDD_GEN_03_HDAN;
			case	25004:	return IDD_GEN_05_JIGANGUSENG;
			case	25005:	return IDD_GEN_06_JIJUMANGLE;
			case	25008:	return IDD_GEN_09_JIJUMSTIFFPOSITION;
			case	25007:	return IDD_GEN_08_JACKUPPOSITION;
			case	25010:	return IDD_GEN_11_STARTENDPOINTJEWON;
			case	25011:	return IDD_GEN_12_BYUNDANMYUNJEWON;
			case	25012:	return IDD_GEN_13_VBRACINGPOSITION;
			case	25013:	return IDD_GEN_14_VSTIFFPOSITION;
			case	25014:	return IDD_GEN_15_HBRACINGPOSITION;
			case	25015:	return IDD_GEN_16_JRIBPOSITION;
			case	25016:	return IDD_GEN_17_DANMYUNTYPE;
			case	25017:	return IDD_GEN_18_FIELDJOINPOSITION;
			case	25018:	return IDD_GEN_19_FACTJOINPOSITION;
			case	25019:	return IDD_GEN_20_SLABJEWON;
			case	20175:	return IDD_DET_01_ECT_INPUT;
			case	2033:	return IDD_DET_12_FIELDJOIN;
			case	2034:	return IDD_DET_13_CROSSBEAM_TYPE;
			case	2036:	return IDD_DET_15_VBRACING_TYPE;
			case	2039:	return IDD_DET_18_HORIBRACING;
			case	2040:	return IDD_DET_19_VSTIFFJEWON;
			case	2041:	return IDD_DET_20_CAMBERJEWON;
			case	2042:	return IDD_DET_21_WEBHSTIFFPOSITION;
			case	2044:	return IDD_DET_23_SHEARCONNECTORVPOSITION;
			case	9001:	return IDD_CALC_DESIGNBASE;
			case	20164:	return IDD_CALC_CANTILEVER_SETUP;
			case	2046:	return IDD_CALC_CANTILEVER_DEADLOAD;
			case	20185:	return IDC_JRIBDETAIL_RECOMMEND_LOWER3;
			case	20186:	return IDC_HSTIFF_CHECK_LENGTH;
			case	20187:	return IDD_CALC_CANTILEVER_MODELRESULT;
			case	20156:	return IDD_CALC_CANTILEVER_LOAD;
			case	20138:	return IDC_VSTIFF_INPUTCOMPLETE;
			case	20139:	return IDB_BITMAP_CURVETYPE;
			case	2047:	return IDD_CALC_CANTILEVER_REBAR;
			case	20157:	return IDD_CALC_ULTIMATELOAD_DLG;
			case	2048:	return IDD_CALC_CANTILEVER_DROOP;
			case	2050:	return IDD_CALC_SECTIONCONCEPT;
			case	2052:	return IDD_CALC_SECFORCE_DIV_CALC;
			case	2053:	return IDD_CALC_SECFORCE_DIV_RESULT;
			case	20119:	return IDD_CALC_WINDFORCE;
			case	2054:	return IDD_CALC_MODELING_SETUP;
			case	2055:	return IDD_CALC_FRAME_COMBO;
			case	2058:	return IDD_CALC_MODELING_RESULT;
			case	2059:	return IDD_CALC_INF_K;
			case	2060:	return IDD_CALC_SECTION_STRESS;
			case	2061:	return IDD_CALC_SECTION_OUTLINE;
			case	20121:	return IDD_CALC_WARPING;
			case	20125:	return IDD_CALC_DISPLACEMENT;
			case	20124:	return IDD_CALC_FATIGUE;
			case	20123:	return IDD_CALC_WELD;
			case	21025:	return IDD_DECK_01_BASICDATA;
			case	12021:	return IDD_DECK_02_REBARINFO;
			case	11999:	return IDD_DECK_03_MAINREINFORCE;
			case	21037:	return IDD_DECK_04_DISTRI_REABR_CTC;
			case	12030:	return IDD_DECK_05_DISTRI_REBAR;
			case	12024:	return IDD_DECK_060_MAINREBAR_CTC;
			case	16021:	return IDD_DECK_061_MAINSKEWREBAR_CTC;
			case	16022:	return IDD_DECK_062_SABOGANG_CTC;
			case	30175:	return IDD_DECK_063_BINDCONC;
			case	12076:	return IDD_DECK_065_CROSSSTRINGER; 
			case	21038:	return IDD_DECK_10_GUARDFENCE;
			case	12025:	return IDD_DECK_07_PLANREBAR;
			case	12031:	return IDD_DECK_08_ETCREBAR;
			case	12032:	return IDD_DECK_09_REBARJEWON;
			case	12165:	return IDD_DECK_11_BM_OPTION;
	}

	return 0;
}

void CGeneralBaseStd::Serialize(CArchive &ar)
{	
	long nMenuIDSize = m_DialogList.GetSize();
	long nFlag = 2;
	UINT nIDD  = 0;			
	BOOL bValue    = FALSE;
	BOOL bUsedFlag = FALSE;;
	
	if(m_pCurruntDlg) 
		m_nSaveMenuIDD = m_pCurruntDlg->GetIDD();

	BOOL bBool = FALSE;

	if( ar.IsStoring() )
	{		
		ar << nFlag;	
		ar << m_nSaveMenuIDD;		
		ar << bBool;
		
		nMenuIDSize = m_DialogList.GetSize();
		ar << nMenuIDSize;
		long n = 0;
		for(n=0; n<nMenuIDSize; n++)
		{					
			ar << m_DialogList[n]->GetIDD();
			ar << m_DialogList[n]->IsUsedFlag();
			ar << m_DialogList[n]->IsErrorInclude();
		}
		for(n=0; n<nMenuIDSize; n++)
		{
			BOOL bModify = m_DialogList[n]->IsModifyData();
			ar << m_DialogList[n]->IsModifyData();
		}
	}
	else
	{		
		int nLastMenuID=-1;
		ar >> nFlag;
		ar >> m_nSaveMenuIDD;

		if (nFlag<2)
		{
			m_nSaveMenuIDD = ConvertResource(m_nSaveMenuIDD);
		}


		ar >> bBool;
		ar >> nMenuIDSize;
		for(long n=0; n<nMenuIDSize; n++)
		{				
			ar >> nIDD;

			if (nFlag<2)
			{
				nIDD = ConvertResource(nIDD);
			}

			ar >> bUsedFlag;
			ar >> bValue;

			if(nIDD == IDD_CALC_SECFORCE_DIV_CALC)
				long n = 1;

			if(GetDlgOfIDD(nIDD))
			{			
				GetDlgOfIDD(nIDD)->SetErrorInclude(bValue);
				GetDlgOfIDD(nIDD)->SetUsedFlag(bUsedFlag);
			}
		}
		if(nFlag>0)
		{
			for(long n=0; n<nMenuIDSize; n++)
			{
				ar >> bValue;
				if(n<m_DialogList.GetSize())
				{
					CGeneralBaseDlg *pDlg = GetDlgOfMenuID(m_DialogList[n]->GetMenuID());
					pDlg->SetModifyData(bValue);
				}
			}
		}
	}
}


CGeneralBaseDlg* CGeneralBaseStd::GetDlgOfMenuID(UINT nMenuID)
{	
	for(long n=0; n<m_DialogList.GetSize(); n++)
	{
		if(m_DialogList[n]->GetMenuID() == nMenuID) 
			return m_DialogList[n];
	}
	return NULL;
}

CGeneralBaseDlg* CGeneralBaseStd::GetDlgOfIDD(UINT nIDD)
{
	for(long n=0; n<m_DialogList.GetSize(); n++)
	{
		long nID = m_DialogList[n]->GetIDD();
		if(m_DialogList[n]->GetIDD() == nIDD) return m_DialogList[n];
	}
	return NULL;	
}

UINT CGeneralBaseStd::GetLastInputMenuID()
{
	for(long n=0; n<m_DialogList.GetSize(); n++)
	{
		CGeneralBaseDlg *pDlg = m_DialogList[n];
		if(!pDlg->IsUsedFlag() && pDlg->IsValid())
			return pDlg->GetMenuID();
	}
	return ID_DECK_15;
}

CGeneralBaseDlg* CGeneralBaseStd::GetDialogByDir(BOOL bNext)
{
	for(long n=0; n<m_DialogList.GetSize(); n++)
	{
		if(m_DialogList[n] == m_pCurruntDlg) 
		{
			if(n==m_DialogList.GetSize()-1 && bNext) return NULL;
			if(n==0 && !bNext) return NULL;
			if(bNext) return m_DialogList[n+1];
			else	  return m_DialogList[n-1];
		}		
	}
	return NULL;
}

long CGeneralBaseStd::GetMenuTypeOnIDD(UINT nIDD)
{
	for(long n=0; n<IDD_MENU_SIZE; n++)
	{
		for(long i=0; i<m_IDDArray[n].GetSize(); i++)
		{
			if(m_IDDArray[n].GetAt(i)==nIDD) return n;
		}
	}
	ASSERT(TRUE);
	return 0;
}

void CGeneralBaseStd::ShowTotalView(CWnd *pWnd)
{
	if(!m_TotalViewDlg.GetSafeHwnd())
		m_TotalViewDlg.Create(IDD_DIALOG_TOTALVIEW,pWnd);
	
	// 영역 다시 설정
	m_TotalViewDlg.m_DomView.GetDomyun()->SetCalcExtRect();
	m_TotalViewDlg.m_DomView.ZoomAll();

	m_TotalViewDlg.ShowWindow(SW_SHOW);
}

void CGeneralBaseStd::HideTotalView()
{
	if(m_TotalViewDlg.GetSafeHwnd())
		m_TotalViewDlg.ShowWindow(SW_HIDE);
}

CString CGeneralBaseStd::GetStrSaveFileName()
{
	CString szFileName= m_pDoc->GetTitle();

	int nFind = szFileName.ReverseFind('.');
	if(nFind != -1)
	{
		szFileName = szFileName.Mid(0, nFind);
	}

	// 파일명이 설정되지 않을 경우
	if(szFileName==" - H 형강교" || szFileName==" - 판형교"	|| szFileName==" - 소수주형")
		szFileName = "임시";

	return szFileName;

}

void CGeneralBaseStd::OutputViewOnAView(CStringArray &Arr)
{
	CString szPath  = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString szOutput= szPath + "\\Result.Out";
	CString szAEdit = szPath + "\\AEdit.exe";
	
	CFile file;
	if(!file.Open(szOutput, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox("파일저장 에러 !",MB_ICONSTOP | MB_OK);
		return;
	}
	CArchive ar(&file, CArchive::store);
	for(long n=0; n<Arr.GetSize(); n++)
		ar.WriteString(Arr.GetAt(n)+"\r\n");
	ar.Close();
	file.Close();
	
	ShellExecute(NULL,NULL,szAEdit,"\"\tNEW" + szOutput + "\"",NULL,SW_SHOWNORMAL);	
}

void CGeneralBaseStd::InsertInputMenu(CXTPMenuBar* pMenuBar)
{
	CXTPControls* pControls = pMenuBar->GetControls();
	if(pControls->GetCount()>IDD_MENU_APLATETYPE_FULL)
		return;
	
	pMenuBar->LoadMenu(&m_menuInput, FALSE);
	for(int i=0; i<IDD_MENU_SIZE; i++)
	{
		CXTPControl* pControl = pControls->GetAt(i+IDD_MENU_APLATETYPE_FULL);
		pControls->MoveBefore(pControl, i+1);
	}
}

// 입력창 활성 비활성 셋팅
void CGeneralBaseStd::SetDialogEnable()
{
	long nCount	= m_DialogList.GetSize();
	for(long i = 0; i < nCount; i++)
	{
		CGeneralBaseDlg *pDlg	= m_DialogList.GetAt(i);
		if(!pDlg->IsValid())
			pDlg->SetUsedFlag(FALSE);
	}
	
// 	AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
// 	AfxGetMainWnd()->SendMessage(WM_USER+13);
}

CString CGeneralBaseStd::GetBearingDBName(int nMenuID)
{
	CString sName = _T("");
	switch(nMenuID)
	{
	case ID_MENUITEM_UNREFERENCE:						sName = UNREFERENCE;	break;
	case ID_MENUITEM_UNISON_PORT:						sName = UNISON_PORT;	break;
	case ID_MENUITEM_UNISON_SPRING_918:					sName = UNISON_SPRING_918;	break;
	case ID_MENUITEM_UNISON_SPRING_1173:				sName = UNISON_SPRING_1173;	break;
	case ID_MENUITEM_MACROAD_ENTIRE_SPRING_09:			sName = MACROAD_ENTIRE_SPRING_09;	break;
	case ID_MENUITEM_MACROAD_ENTIRE_SPRING_115:			sName = MACROAD_ENTIRE_SPRING_115;	break;
	case ID_MENUITEM_AASHTO_SPRING:						sName = AASHTO_SPRING;	break;
	case ID_MENUITEM_BUHUNG_GAHWANG_ENTIRE_SPRING_09:	sName = BUHUNG_GAHWANG_ENTIRE_SPRING_09;	break;
	case ID_MENUITEM_BUHUNG_GAHWANG_ENTIRE_SPRING_115:	sName = BUHUNG_GAHWANG_ENTIRE_SPRING_115;	break;
	case ID_MENUITEM_DAEKYUNG_ERB_SPRING_09:			sName = DAEKYUNG_ERB_SPRING_09;	break;
	case ID_MENUITEM_DAEKYUNG_ERB_SPRING_115:			sName = DAEKYUNG_ERB_SPRING_115;	break;
	case ID_MENUITEM_HS_NRB_SPRING_09:					sName = HS_NRB_SPRING_09;	break;
	case ID_MENUITEM_HS_NRB_SPRING_115:					sName = HS_NRB_SPRING_115;	break;
	case ID_MENUITEM_HS_ENTIRE_SPRING_09:				sName = HS_ENTIRE_SPRING_09;	break;
	case ID_MENUITEM_HS_ENTIRE_SPRING_115:				sName = HS_ENTIRE_SPRING_115;	break;
	case ID_MENUITEM_UNISON_LRB:						sName = UNISON_LRB;	break;
	case ID_MENUITEM_UNISON_FPB:						sName = UNISON_FPB;	break;
	case ID_MENUITEM_ESCO_RTS_DRB_AASHTO:				sName = ESCO_RTS_DRB_AASHTO;	break;
	case ID_MENUITEM_ESCO_RTS_DRB_G1173:				sName = ESCO_RTS_DRB_G1173;	break;
	case ID_MENUITEM_ESCO_RTS_DRB_G918:					sName = ESCO_RTS_DRB_G918;	break;
	case ID_MENUITEM_ESCO_RTS_DISK:						sName = ESCO_RTS_DISK;	break;
	case ID_MENUITEM_ESCO_RTS_AASHTO:					sName = ESCO_RTS_AASHTO;	break;
	case ID_MENUITEM_ESCO_RTS_G1173:					sName = ESCO_RTS_G1173;	break;
	case ID_MENUITEM_ESCO_RTS_G918:						sName = ESCO_RTS_G918;	break;
	case ID_MENUITEM_ESCO_RTS_POT:						sName = ESCO_RTS_POT;	break;
	case ID_MENUITEM_ESCO_RTS_EQS:						sName = ESCO_RTS_EQS;	break;
	case ID_MENUITEM_ESCO_RTS_EQS_PLUS:					sName = ESCO_RTS_EQS_PLUS;	break;
	case ID_MENUITEM_ESCO_RTS_OMEQS:					sName = ESCO_RTS_OMEQS;	break;
	case ID_MENUITEM_ESCO_RTS_FDB:						sName = ESCO_RTS_FDB;	break;
	case ID_MENUITEM_ESCO_RTS_EQS_LIGHT:				sName = ESCO_RTS_EQS_LIGHT;	break;
	case ID_MENUITEM_ESCO_RTS_EQS_LIGHT_OM:				sName = ESCO_RTS_EQS_LIGHT_OM;	break;
	case ID_MENUITEM_KR_PORT:							sName = KR_PORT;	break;
	case ID_MENUITEM_KR_PORT_EXTEND:					sName = KR_PORT_EXTEND;	break;
	case ID_MENUITEM_KR_SPRING_09:						sName = KR_SPRING_09;	break;
	case ID_MENUITEM_KR_SPRING_115:						sName = KR_SPRING_115;	break;
	case ID_MENUITEM_KR_DISC:							sName = KR_DISC;	break;
	}
	
	return sName;
}

void CGeneralBaseStd::DrawWallGuard(CForceDiagram *pDom, double dTextHeight)
{
	CPlateBridgeApp	*pDB		= GetBridge();	
	CARoadOptionStd	*pOptStd	= GetDataManage()->GetOptionStd();

	CDomyun Dom(pDom);

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	Dom.SetScaleDim(0.1);
	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	CAPlateDrawDanmyun DrawDanmyun(GetDataManage());
	
	for (long n=0; n<pDB->GetQtyHDanNode(); n++)
	{
		if(pDB->IsTypeGuardFenceDom(n))  //방호벽이면
		{
			CDPoint A[9];
			pDB->GetXyGuardFenceDom(pBx, n, A, TRUE);
			CDPointArray xyWall = GetWallGuardPoint(A);
			for (long i=0; i<xyWall.GetSize()-1; i++)
				Dom.LineTo(xyWall[i+0], xyWall[i+1]);

		}
	}
	*pDom << Dom;
}

long CGeneralBaseStd::GetLoadApplyMemberCount(CFEMManage *pFEM)
{
	long nCnt = 0;
	
	for (long nE=0; nE<pFEM->GetElementSize(); nE++)
	{
		CElement* pElem = pFEM->GetElement((unsigned short)nE);
		CVector vStt    = pFEM->GetJoint(pElem->m_nJStt)->m_vPoint;
		CVector vEnd    = pFEM->GetJoint(pElem->m_nJEnd)->m_vPoint;
		
		if (vStt.z==0 && vEnd.z==0) nCnt++; 
	}
	
	return nCnt;
}

long CGeneralBaseStd::GetLoadCase(CFEMManage *pFEM, CString szLoadCase)
{
	long nLoadCaseSize	= pFEM->GetLoadCaseSize() - pFEM->m_nOutputCaseAddSize;

	if (szLoadCase ==_T("")) return -1;
	
	for (long n=0; n<nLoadCaseSize; n++)
	{
		CString szCase = pFEM->GetLoadCaseString(n);
		if (szLoadCase == szCase)
		{
			return n;
		}
	}
	
	return -1;
}

void CGeneralBaseStd::DrawDimJoint(CForceDiagram *pDom, CFEMManage *pFEM, double dTextHeight, BOOL bElemNum)
{
	CDomyun Dom(pDom);
	CARoadOptionStd		*pOptStd= GetDataManage()->GetOptionStd();
	CElement* pElem = NULL;
	CJoint	* pJ_I	= NULL;
	CJoint	* pJ_J	= NULL;
	long	nApplyElem = GetLoadApplyMemberCount(pFEM);

	// seek bottom level
	double dBottom = 0;
	long nElem = 0;
	for (nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		pElem	= pFEM->GetElement((unsigned short)nElem);
		pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);

		double dPos = min( pJ_I->m_vPoint.z, pJ_J->m_vPoint.z );
		dBottom = min( dPos, dBottom );
	}


	Dom.SetDirection("BOTTOM");
	
	double	dSum		= 0;
	for(nElem=0; nElem < nApplyElem; nElem++)
	{
		pElem	= pFEM->GetElement((unsigned short)nElem);
		pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);
		
		double dX	  =  pJ_J->m_vPoint.x-pJ_I->m_vPoint.x ;
		
		pOptStd->SetEnvType(&Dom, HCAD_STLC);
		if(pJ_I->m_UX==1 || pJ_I->m_UY==1 || pJ_I->m_UZ==1)
		{
			CDPointArray ptArr;
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x, dBottom));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x-Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x+Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_I->m_vPoint.x, dBottom));
			Dom.HatchingByCDPointArray(ptArr, 45, Dom.Always(1), TRUE);
		}
		
		if(pJ_J->m_UX==1 || pJ_J->m_UY==1 || pJ_J->m_UZ==1)
		{
			CDPointArray ptArr;
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x, dBottom));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x-Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x+Dom.Always(1.5), dBottom-Dom.Always(3)));
			ptArr.Add(CDPoint(pJ_J->m_vPoint.x, pJ_J->m_vPoint.y));
			Dom.HatchingByCDPointArray(ptArr, 45, Dom.Always(1), TRUE);
		}
		
		pOptStd->SetEnvType(&Dom, HCAD_DIML);
		
		Dom.SetTextHeight(dTextHeight);
		Dom.DimMoveTo(dSum, dBottom,0);
		dSum += dX;
		Dom.DimLineTo(dX,0,COMMA(frM(dX)));
		
		if(nElem==nApplyElem-1) //맨 마지막 턴을 출력하는 곳
		{
			Dom.DimMoveTo(dSum, dBottom, 0);
			Dom.DimMoveTo(0, dBottom, 1);
			Dom.DimLineTo(dSum, 1, COMMA(frM(dSum)));
		}		
	}
	
	*pDom << Dom;
}

void CGeneralBaseStd::DrawDimLoadCase(CForceDiagram *pDom, CFEMManage *pFEM, long nCase, double dTextHeight)
{
	// 1. seek Distance
	CDoubleArray ArrDis;

	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pE = (CElement *)pFEM->m_ElementList.GetNext(pos);		
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);
		CLoadForce *pLoadForce = pE->GetLoadForce(nCase);
		
		if(pLoadForce && pLoadForce->IsFlag(EXIST_TRAP))
		{
			for(long n=0; n<pLoadForce->m_TrapForceArr.GetSize(); n++)
			{
				if(!pLoadForce->m_TrapForceArr[n]->m_bApply) continue;
				ArrDis.Add(pJ_I->m_vPoint.x);
				ArrDis.Add(pJ_J->m_vPoint.x);
			}
		}
		if(pLoadForce && pLoadForce->IsFlag(EXIST_DISTFORCE))
		{
			CString szData;
			for(long n=0; n<pLoadForce->m_PointForceArr.GetSize(); n++)				
			{
				if(!pLoadForce->m_PointForceArr[n]->m_bApply)	continue;
				CDPoint ptStt = CDPoint(pJ_I->m_vPoint.x, pJ_I->m_vPoint.z);
				CDPoint ptEnd = CDPoint(pJ_J->m_vPoint.x, pJ_J->m_vPoint.z);
				if(pLoadForce->m_PointForceArr[n]->m_vForce.x!=0)
				{				
					CDPoint vPos = ptStt + (ptEnd - ptStt).Unit() * ~(ptEnd - ptStt) * pLoadForce->m_PointForceArr[n]->m_dDist;
					if(!pLoadForce->m_PointForceArr[n]->m_bRateDist)
						vPos = CDPoint(ptStt.x+pLoadForce->m_PointForceArr[n]->m_dDist, ptStt.y);
					
					ArrDis.Add(vPos.x);
				}
				if(pLoadForce->m_PointForceArr[n]->m_vForce.z!=0)
				{				
					CDPoint vPos = ptStt + (ptEnd - ptStt).Unit() * ~(ptEnd - ptStt) * pLoadForce->m_PointForceArr[n]->m_dDist;				
					if(!pLoadForce->m_PointForceArr[n]->m_bRateDist)
						vPos = CDPoint(ptStt.x+pLoadForce->m_PointForceArr[n]->m_dDist, ptStt.y);

					ArrDis.Add(vPos.x);
				}
				if ( ~(pLoadForce->m_PointForceArr[n]->m_vMoment)!=0 )
				{
					CDPoint vPos = ptStt + (ptEnd - ptStt).Unit() * ~(ptEnd - ptStt) * pLoadForce->m_PointForceArr[n]->m_dDist;
					ArrDis.Add(vPos.x);
				}
			}
		}	
	}

	double dLen = 0;
	pos = pFEM->m_JointList.GetHeadPosition();
	while(pos)
	{
		CJoint *pJ = (CJoint *)pFEM->m_JointList.GetNext(pos);
		
		dLen = max(dLen, pJ->m_vPoint.x);
		CLoadForceJoint *pLoadForce = pJ->GetLoadForce(nCase);
		if(pLoadForce && pLoadForce->IsFlag(EXIST_JOINT_FORCE))
		{
			CDPoint ptPos = CDPoint(pJ->m_vPoint.x, pJ->m_vPoint.z);		
			if(!pLoadForce->m_bApply) continue;
			
			if(pLoadForce->m_vForce.x!=0)
			{
				ArrDis.Add(ptPos.x);
			}
			if(pLoadForce->m_vForce.z!=0)
			{							
				ArrDis.Add(ptPos.x);
			}		
			if(~(pLoadForce->m_vMoment)!=0)
			{
				ArrDis.Add(ptPos.x);
			}
		}
	}
	ArrDis.Add(0);
	ArrDis.Add(dLen);
	ArrDis.RemoveSameValue();
	ArrDis.Sort();
		

	// 2. Set Draw
	CDomyun			Dom(pDom);
	CARoadOptionStd	*pOptStd		= GetDataManage()->GetOptionStd();
	double	dY = Dom.Always(10);
	double	dSum	= 0.0;
	
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("TOP");
	Dom.SetTextHeight(dTextHeight);
	
	for(long n = 0; n < ArrDis.GetSize() - 1; n++)
	{	
		Dom.DimMoveTo(dSum,	dY,	0);
		Dom.DimLineTo(ArrDis[n+1] - ArrDis[n],	0,	COMMA(frM(ArrDis[n+1] - ArrDis[n])));
		
		dSum += (ArrDis[n+1] - ArrDis[n]);
	}
	
	*pDom << Dom;
}

CDPointArray CGeneralBaseStd::GetWallGuardPoint(CDPoint* pPoint)
{
	CPlateBridgeApp *pDB = GetBridge();

	CDPointArray xyArr;
	double dLimitBottom = pPoint[8].y;
	double dGirderCenter = pDB->GetGirder(0)->m_dGirderCenter;
	double dWidthSlabLeft = pDB->GetWidthSlabLeft();
	double dLen = dGirderCenter - dWidthSlabLeft;

	for (long n=0; n<9; n++)
	{
		if (pPoint[n].y < dLimitBottom)
			pPoint[n].y = dLimitBottom;
		pPoint[n].x -= dLen;
		pPoint[n].x  = toM(pPoint[n].x);
		pPoint[n].y -= dLimitBottom;
		pPoint[n].y = toM(pPoint[n].y);
		
		xyArr.Add(pPoint[n]);
	}
	xyArr.Add(xyArr.GetAt(0));

	return xyArr;
}
