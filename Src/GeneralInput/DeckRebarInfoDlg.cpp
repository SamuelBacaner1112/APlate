// DeckRebarInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateCalc/APlateCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeckRebarInfoDlg dialog


CDeckRebarInfoDlg::CDeckRebarInfoDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CDeckRebarInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckRebarInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDeckRebarInfoDlg::~CDeckRebarInfoDlg()
{
	
}

void CDeckRebarInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckRebarInfoDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_APPLY_SAME_LRC, m_btnSameAll);
	DDX_Control(pDX, IDC_BUTTON_APPLY_CURRENT, m_btnReport);
}

BEGIN_MESSAGE_MAP(CDeckRebarInfoDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CDeckRebarInfoDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_SAME_LRC, OnButtonApplySameLRC)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_CURRENT, OnButtonApplyAtCalcData)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)	
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckRebarInfoDlg message handlers
void CDeckRebarInfoDlg::OnPreInitDialog()
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;	

	m_Grid.InitGrid(toKgPCm2(pDeckData->m_SigmaY));

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_APPLY_SAME_LRC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_CURRENT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}



void CDeckRebarInfoDlg::SetGridData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	UINT nFormat = DT_CENTER | DT_VCENTER;

	CString str = _T("");
	long nLine = 0;
	BOOL bOneJigan = pDB->m_nQtyJigan==1 ? TRUE : FALSE;
	BOOL bSkewSeg  = pDeckData->IsSkew();

	m_Grid.SetGridDefault(bSkewSeg?18:17, 10, 2, 3, FALSE);
	m_Grid.AddCell(0, 0, "구분\n");	m_Grid.AddCell(0, 1, "배근위치\n");	m_Grid.AddCell(0, 2, "배근위치\n");
	m_Grid.AddCell(1, 0, "구분\n");	m_Grid.AddCell(1, 1, "배근위치\n");	m_Grid.AddCell(1, 2, "배근위치\n");
	
	m_Grid.AddCell(0, 3, "상면");	m_Grid.AddCell(0, 4, "상면");		m_Grid.AddCell(0, 5, "상면");
	m_Grid.AddCell(0, 6, "하면");	m_Grid.AddCell(0, 7, "하면");		m_Grid.AddCell(0, 8, "하면");		m_Grid.AddCell(0, 9, "검토\n");	
	
	m_Grid.AddCell(1, 3, "간격");	m_Grid.AddCell(1, 4, "①직경");		m_Grid.AddCell(1, 5, "②직경");
	m_Grid.AddCell(1, 6, "간격");	m_Grid.AddCell(1, 7, "①직경");		m_Grid.AddCell(1, 8, "②직경");		m_Grid.AddCell(1, 9, "검토\n");

	m_Grid.AddCell(2, 0, "주철근\n");		m_Grid.AddCell(2, 1, "일반부\n");		m_Grid.AddCell(2, 2, "좌측캔틸레버부");
	m_Grid.AddCell(3, 0, "주철근\n");		m_Grid.AddCell(3, 1, "일반부\n");		m_Grid.AddCell(3, 2, "우측캔틸레버부");
	m_Grid.AddCell(4, 0, "주철근\n");		m_Grid.AddCell(4, 1, "일반부\n");		m_Grid.AddCell(4, 2, "슬래브중앙부");
	m_Grid.AddCell(5, 0, "주철근\n");		m_Grid.AddCell(5, 1, "단부보강부\n");	m_Grid.AddCell(5, 2, "좌측캔틸레버부");
	m_Grid.AddCell(6, 0, "주철근\n");		m_Grid.AddCell(6, 1, "단부보강부\n");	m_Grid.AddCell(6, 2, "우측캔틸레버부");
	m_Grid.AddCell(7, 0, "주철근\n");		m_Grid.AddCell(7, 1, "단부보강부\n");	m_Grid.AddCell(7, 2, "슬래브중앙부");
	if (bOneJigan)	m_Grid.AddCell(8, 0, "배력철근\n\n");
	else			m_Grid.AddCell(8, 0, "배력철근\n");		m_Grid.AddCell(8, 1, "일반부\n");		m_Grid.AddCell(8, 2, "좌측캔틸레버부");	
	if (bOneJigan)	m_Grid.AddCell(9, 0, "배력철근\n\n");
	else			m_Grid.AddCell(9, 0, "배력철근\n");		m_Grid.AddCell(9, 1, "일반부\n");		m_Grid.AddCell(9, 2, "우측캔틸레버부");
	if (bOneJigan)	m_Grid.AddCell(10,0, "배력철근\n\n");
	else			m_Grid.AddCell(10,0, "배력철근\n");		m_Grid.AddCell(10,1, "일반부\n");		m_Grid.AddCell(10,2, "슬래브중앙부");
	if (bOneJigan)	m_Grid.AddCell(11,0, "배력철근\n\n");
	else			m_Grid.AddCell(11,0, "배력철근\n");		m_Grid.AddCell(11, 1, "단부보강부\n");	m_Grid.AddCell(11, 2, "좌측캔틸레버부");
	if (bOneJigan)	m_Grid.AddCell(12,0, "배력철근\n\n");
	else			m_Grid.AddCell(12,0, "배력철근\n");		m_Grid.AddCell(12, 1, "단부보강부\n");	m_Grid.AddCell(12, 2, "우측캔틸레버부");
	if (bOneJigan)	m_Grid.AddCell(13,0, "배력철근\n\n");
	else			m_Grid.AddCell(13,0, "배력철근\n");		m_Grid.AddCell(13, 1, "단부보강부\n");	m_Grid.AddCell(13, 2, "슬래브중앙부");
	m_Grid.AddCell(14,0, "배력철근\n");	m_Grid.AddCell(14, 1, "중간지점부\n");	m_Grid.AddCell(14, 2, "좌측캔틸레버부");
	m_Grid.AddCell(15,0, "배력철근\n");	m_Grid.AddCell(15, 1, "중간지점부\n");	m_Grid.AddCell(15, 2, "우측캔틸레버부");
	m_Grid.AddCell(16,0, "배력철근\n");	m_Grid.AddCell(16, 1, "중간지점부\n");	m_Grid.AddCell(16, 2, "슬래브중앙부");
	if(bSkewSeg){	m_Grid.AddCell(17,0, "사보강철근");	m_Grid.AddCell(17, 1, "사보강철근");	m_Grid.AddCell(17, 2, "사보강철근");}
	//////////////////////////////////////////////////////////////////////////
	// 상면 간격
	m_Grid.AddCell(2, 3, &pDeckData->m_dGenMainRebarCTC[0]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(3, 3, &pDeckData->m_dGenMainRebarCTC[1]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(4, 3, &pDeckData->m_dGenMainRebarCTC[2]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(5, 3, &pDeckData->m_dEndMainRebarCTC[0]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(6, 3, &pDeckData->m_dEndMainRebarCTC[1]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(7, 3, &pDeckData->m_dEndMainRebarCTC[2]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(8, 3, &pDeckData->m_dGenSupportRebarCTC[0]); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(9, 3, &pDeckData->m_dGenSupportRebarCTC[1]); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(10,3, &pDeckData->m_dGenSupportRebarCTC[2]); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(11,3, &pDeckData->m_dEndSupportRebarCTC[0]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(12,3, &pDeckData->m_dEndSupportRebarCTC[1]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(13,3, &pDeckData->m_dEndSupportRebarCTC[2]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(14,3, &pDeckData->m_dJijumRebarCTCUp[0]);		m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(15,3, &pDeckData->m_dJijumRebarCTCUp[1]);		m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(16,3, &pDeckData->m_dJijumRebarCTCUp[2]);		m_Grid.SetCellDec(CELL_DEC_0);
	if(bSkewSeg)	m_Grid.AddCell(17,3, &pDeckData->m_dSabogangRebarCTCUp);	m_Grid.SetCellDec(CELL_DEC_0);
	//////////////////////////////////////////////////////////////////////////
	// 상면 직경1Cycle
	m_Grid.AddCell(2, 4, &pDeckData->m_dDiaMainRebar_Cent[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(3, 4, &pDeckData->m_dDiaMainRebar_Cent[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(4, 4, &pDeckData->m_dDiaMainRebar_Cent[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(5, 4, &pDeckData->m_dDiaMainRebar_Dan[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(6, 4, &pDeckData->m_dDiaMainRebar_Dan[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(7, 4, &pDeckData->m_dDiaMainRebar_Dan[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(8, 4, &pDeckData->m_dDiaBeryukRebar_Cent[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(9, 4, &pDeckData->m_dDiaBeryukRebar_Cent[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(10,4, &pDeckData->m_dDiaBeryukRebar_Cent[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(11,4, &pDeckData->m_dDiaBeryukRebar_Dan[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(12,4, &pDeckData->m_dDiaBeryukRebar_Dan[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(13,4, &pDeckData->m_dDiaBeryukRebar_Dan[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(14,4, &pDeckData->m_dDiaJijumRebarUp[0], nFormat, CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(15,4, &pDeckData->m_dDiaJijumRebarUp[1], nFormat, CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(16,4, &pDeckData->m_dDiaJijumRebarUp[2], nFormat, nLine, "", CELL_TYPE_DIA);
	if(bSkewSeg)	m_Grid.AddCell(17,4, &pDeckData->m_dDiaSabogangUp, nFormat, nLine, "", CELL_TYPE_DIA);

	//////////////////////////////////////////////////////////////////////////
	// 상면 직경2Cycle
	m_Grid.AddCell(2, 5, &pDeckData->m_dDiaMainRebar_Cent2nd[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(3, 5, &pDeckData->m_dDiaMainRebar_Cent2nd[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(4, 5, &pDeckData->m_dDiaMainRebar_Cent2nd[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(5, 5, &pDeckData->m_dDiaMainRebar_Dan2nd[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(6, 5, &pDeckData->m_dDiaMainRebar_Dan2nd[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(7, 5, &pDeckData->m_dDiaMainRebar_Dan2nd[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(8, 5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(9, 5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(10,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(11,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(12,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(13,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(14,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(15,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(16,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(16,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	if(bSkewSeg)	m_Grid.AddCell(17,5, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	//////////////////////////////////////////////////////////////////////////
	// 하면 간격
	m_Grid.AddCell(2, 6, &pDeckData->m_dGenMainRebarCTCLower[0]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(3, 6, &pDeckData->m_dGenMainRebarCTCLower[1]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(4, 6, &pDeckData->m_dGenMainRebarCTCLower[2]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(5, 6, &pDeckData->m_dEndMainRebarCTCLower[0]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(6, 6, &pDeckData->m_dEndMainRebarCTCLower[1]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(7, 6, &pDeckData->m_dEndMainRebarCTCLower[2]);	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(8, 6, &pDeckData->m_dGenSupportRebarCTCLower[0]);m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(9, 6, &pDeckData->m_dGenSupportRebarCTCLower[1]);m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(10,6, &pDeckData->m_dGenSupportRebarCTCLower[2]);m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(11,6, &pDeckData->m_dEndSupportRebarCTCLower[0]);m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(12,6, &pDeckData->m_dEndSupportRebarCTCLower[1]);m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(13,6, &pDeckData->m_dEndSupportRebarCTCLower[2]);m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(14,6, &pDeckData->m_dJijumRebarCTCDn[0]);			m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(15,6, &pDeckData->m_dJijumRebarCTCDn[1]);			m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(16,6, &pDeckData->m_dJijumRebarCTCDn[2]);			m_Grid.SetCellDec(CELL_DEC_0);
	if(bSkewSeg)	m_Grid.AddCell(17,6, &pDeckData->m_dSabogangRebarCTCDn);		m_Grid.SetCellDec(CELL_DEC_0);
	//////////////////////////////////////////////////////////////////////////
	// 하면 직경1Cycle
	m_Grid.AddCell(2, 7, &pDeckData->m_dDiaMainRebarLower_Cent[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(3, 7, &pDeckData->m_dDiaMainRebarLower_Cent[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(4, 7, &pDeckData->m_dDiaMainRebarLower_Cent[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(5, 7, &pDeckData->m_dDiaMainRebarLower_Dan[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(6, 7, &pDeckData->m_dDiaMainRebarLower_Dan[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(7, 7, &pDeckData->m_dDiaMainRebarLower_Dan[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(8, 7, &pDeckData->m_dDiaBeryukRebarLower_Cent[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(9, 7, &pDeckData->m_dDiaBeryukRebarLower_Cent[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(10,7, &pDeckData->m_dDiaBeryukRebarLower_Cent[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(11,7, &pDeckData->m_dDiaBeryukRebarLower_Dan[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(12,7, &pDeckData->m_dDiaBeryukRebarLower_Dan[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(13,7, &pDeckData->m_dDiaBeryukRebarLower_Dan[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(14,7, &pDeckData->m_dDiaJijumRebarDn[0], nFormat, CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(15,7, &pDeckData->m_dDiaJijumRebarDn[1], nFormat, CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(16,7, &pDeckData->m_dDiaJijumRebarDn[2], nFormat, nLine, "", CELL_TYPE_DIA);
	if(bSkewSeg)	m_Grid.AddCell(17,7, &pDeckData->m_dDiaSabogangDn, nFormat, nLine, "", CELL_TYPE_DIA);
	//////////////////////////////////////////////////////////////////////////
	// 하면 직경1Cycle
	m_Grid.AddCell(2, 8, &pDeckData->m_dDiaMainRebarLower_Cent2nd[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(3, 8, &pDeckData->m_dDiaMainRebarLower_Cent2nd[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(4, 8, &pDeckData->m_dDiaMainRebarLower_Cent2nd[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(5, 8, &pDeckData->m_dDiaMainRebarLower_Dan2nd[0], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(6, 8, &pDeckData->m_dDiaMainRebarLower_Dan2nd[1], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(7, 8, &pDeckData->m_dDiaMainRebarLower_Dan2nd[2], nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(8, 8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(9, 8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(10,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(11,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(12,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(13,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(14,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(15,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(16,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	if(bSkewSeg)	m_Grid.AddCell(17,8, "-", DT_CENTER|DT_VCENTER, CELL_READONLY);
	
	if(bSkewSeg)
		m_Grid.AddCell(17, 9, "-", nFormat, CELL_READONLY);

	// 검토(O.K, N.G)는 일단 유보
	m_Grid.SetColumnWidth(9, 0);
}



void CDeckRebarInfoDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	long nCRow = m_Grid.GetFocusCell().row;
	long nCCol = m_Grid.GetFocusCell().col;
	if(nCRow<1) nCRow = 1;
	if(nCCol<1) nCCol = 1;

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	InsertDomyun(nCRow, nCCol);

	CGeneralBaseDlg::DrawInputDomyunView(TRUE);
}



void CDeckRebarInfoDlg::InsertDomyun(long nCRow, long nCCol)
{
	if(nCRow<2)	nCRow = 2;
	if(nCCol<4)	nCCol = 4;
	
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);
	
	CDomyun Dom(pDom);

	CString	csSection	= "Afx Directory";
	CString	csWorkItem	= "Fixed Directory";
	CString	szPath		= AfxGetApp()->GetProfileString(csSection, csWorkItem);
	
	CString sText1 = m_Grid.GetTextMatrix(nCRow, 0);
	CString sText2 = m_Grid.GetTextMatrix(nCRow, 1);
	CString sText3 = m_Grid.GetTextMatrix(nCRow, 2);
	CString sText4 = m_Grid.GetTextMatrix(0, nCCol);
	if(sText1=="사보강철근")	return;
/*	
	if(sText4.Find("검토")!=-1)
		sText4 = "상면";
	
	if(sText1.Find("주철근")!=-1)
		sText1 = "주철근";
	else if(sText1.Find("배력철근")!=-1)
		sText1 = "배력철근";

	if(sText2.Find("일반부")!=-1)
		sText2 = "일반부";
	else if(sText2.Find("단부보강부")!=-1)
		sText2 = "단부보강부";
	else if(sText2.Find("중간지점부")!=-1)
		sText2 = "중간지점부";
*/
	if(sText4.Find("검토")!=-1)
		sText4 = "Upper";
	if(sText4.Find("상면")!=-1)
		sText4 = "Upper";
	if(sText4.Find("하면")!=-1)
		sText4 = "Lower";

	if(sText3.Find("좌측캔틸레버부")!=-1)
		sText3 = "LeftCant";
	if(sText3.Find("우측캔틸레버부")!=-1)
		sText3 = "RightCant";
	if(sText3.Find("슬래브중앙부")!=-1)
		sText3 = "SlabCen";

	if(sText1.Find("주철근")!=-1)
		sText1 = "Main";
	else if(sText1.Find("배력철근")!=-1)
		sText1 = "Distri";
	if(sText2.Find("일반부")!=-1)
		sText2 = "Gen";
	else if(sText2.Find("단부보강부")!=-1)
		sText2 = "End";
	else if(sText2.Find("중간지점부")!=-1)
		sText2 = "Mid";

	CString sTextTot = sText1+sText2+sText3+sText4;
	CString szName;
	szName.Format("%s\\BlockDomyun\\%s.gif", szPath, sTextTot);

	Dom.DrawPicture(CDPoint(0,0), szName);

	*pDom << Dom;
}



void CDeckRebarInfoDlg::ConvertFloorDataToRebarData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CGlobarOption *pGlopt = m_pStd->m_pDataManage->GetGlobalOption(); 
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	
	CCalcData *pCalcData = m_pStd->m_pDataManage->GetCalcData();

	CCalcData::_CALC_CANTILEVER_DATA stCantL= m_pStd->m_pDataManage->GetCalcData()->CALC_CANTILEVER_DATA[FLOOR_LEFT];
	CCalcData::_CALC_CANTILEVER_DATA stCantR= m_pStd->m_pDataManage->GetCalcData()->CALC_CANTILEVER_DATA[FLOOR_RIGHT];
	CCalcData::_CALC_CANTILEVER_DATA stCantC= m_pStd->m_pDataManage->GetCalcData()->CALC_CANTILEVER_DATA[FLOOR_MID];

//		// 소수주형교량 철근
//		CString	m_strGenReinTDia_1Cy;		// 철근지름 일반부 주철근 인장부 1Cycle
//		CString	m_strGenReinTDia_2Cy;		// 철근지름 일반부 주철근 인장부 2Cycle
//		long	m_nGenReinTDiaIdx_1Cy;		// 철근지름 일반부 주철근 인장부 1Cycle
//		long	m_nGenReinTDiaIdx_2Cy;		// 철근지름 일반부 주철근 인장부 2Cycle
//		double	m_dGenReinTCTC;				// 철근간격 일반부 주철근 인장부
//		double	m_dGenReinTAsreq;			// 철근지름 일반부 주철근 인장부 필요철근량
//		double	m_dGenReinTAsuse;			// 철근지름 일반부 주철근 인장부 사용철근량
//		double	m_dGenReinTAsuse_1Cy;		// 철근지름 일반부 주철근 인장부 사용철근량 1Cycle
//		double	m_dGenReinTAsuse_2Cy;		// 철근지름 일반부 주철근 인장부 사용철근량 2Cylcle
//
//		CString	m_strGenReinCDia_1Cy;		// 철근지름 일반부 주철근 압축부 1Cycle
//		CString	m_strGenReinCDia_2Cy;		// 철근지름 일반부 주철근 압축부 2Cycle
//		long	m_nGenReinCDiaIdx_1Cy;		// 철근지름 일반부 주철근 압축부 1Cycle
//		long	m_nGenReinCDiaIdx_2Cy;		// 철근지름 일반부 주철근 압축부 2Cycle
//		double	m_dGenReinCCTC;				// 철근간격 일반부 주철근 압축부
//		double	m_dGenReinCAsreq;			// 철근지름 일반부 주철근 압축부 필요철근량
//		double	m_dGenReinCAsuse;			// 철근지름 일반부 주철근 압축부 사용철근량
//		double	m_dGenReinCAsuse_1Cy;		// 철근지름 일반부 주철근 압축부 사용철근량 1Cycle
//		double	m_dGenReinCAsuse_2Cy;		// 철근지름 일반부 주철근 압축부 사용철근량 2Cylcle

	// CARailFloorData의 철근 제원값을 ADeckData의 철근 제원값에 복사함.    
	if(pDB->m_nSlabTensionSize==0)
	{
		// 상부	(좌측)
		pDeckData->m_dDiaMainRebar_Cent[0]			= _dRebarDia[stCantL.m_Rein_Combo1];	
		pDeckData->m_dDiaMainRebar_Cent2nd[0]		= _dRebarDia[stCantL.m_Rein_Combo1_2Cy];		
		pDeckData->m_dDiaBeryukRebar_Cent[0]		= _dRebarDia[stCantL.m_Rein_Combo3];			
		pDeckData->m_dDiaMainRebar_Dan[0]			= _dRebarDia[stCantL.m_Rein_Combo2];		
		pDeckData->m_dDiaMainRebar_Dan2nd[0]		= _dRebarDia[stCantL.m_Rein_Combo2_2Cy];		
		pDeckData->m_dDiaBeryukRebar_Dan[0]			= _dRebarDia[stCantL.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTC[0]			= stCantL.m_Rein_Ctc1;		
		pDeckData->m_dGenSupportRebarCTC[0]			= stCantL.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTC[0]			= stCantL.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTC[0]			= stCantL.m_Rein_CtcB1;	
		// 하부
		pDeckData->m_dDiaMainRebarLower_Cent[0]		= _dRebarDia[stCantL.m_Rein_Combo1];	
		pDeckData->m_dDiaMainRebarLower_Cent2nd[0]	= _dRebarDia[stCantL.m_Rein_Combo1_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Cent[0]	= _dRebarDia[stCantL.m_Rein_Combo3];		
		pDeckData->m_dDiaMainRebarLower_Dan[0]		= _dRebarDia[stCantL.m_Rein_Combo2];			
		pDeckData->m_dDiaMainRebarLower_Dan2nd[0]	= _dRebarDia[stCantL.m_Rein_Combo2_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Dan[0]	= _dRebarDia[stCantL.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTCLower[0]		= stCantL.m_Rein_Ctc1;		
		pDeckData->m_dGenSupportRebarCTCLower[0]	= stCantL.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTCLower[0]		= stCantL.m_Rein_Ctc2;		
		pDeckData->m_dEndSupportRebarCTCLower[0]	= stCantL.m_Rein_CtcB1;	

		// 상부	(우측)
		pDeckData->m_dDiaMainRebar_Cent[1]			= _dRebarDia[stCantR.m_Rein_Combo1];	
		pDeckData->m_dDiaMainRebar_Cent2nd[1]		= _dRebarDia[stCantR.m_Rein_Combo1_2Cy];	
		pDeckData->m_dDiaBeryukRebar_Cent[1]		= _dRebarDia[stCantR.m_Rein_Combo3];			
		pDeckData->m_dDiaMainRebar_Dan[1]			= _dRebarDia[stCantR.m_Rein_Combo2];		
		pDeckData->m_dDiaMainRebar_Dan2nd[1]		= _dRebarDia[stCantR.m_Rein_Combo2_2Cy];	
		pDeckData->m_dDiaBeryukRebar_Dan[1]			= _dRebarDia[stCantR.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTC[1]			= stCantR.m_Rein_Ctc1;		
		pDeckData->m_dGenSupportRebarCTC[1]			= stCantR.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTC[1]			= stCantR.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTC[1]			= stCantR.m_Rein_CtcB1;	
		// 하부
		pDeckData->m_dDiaMainRebarLower_Cent[1]		= _dRebarDia[stCantR.m_Rein_Combo1];	
		pDeckData->m_dDiaMainRebarLower_Cent2nd[1]	= _dRebarDia[stCantR.m_Rein_Combo1_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Cent[1]	= _dRebarDia[stCantR.m_Rein_Combo3];		
		pDeckData->m_dDiaMainRebarLower_Dan[1]		= _dRebarDia[stCantR.m_Rein_Combo2];			
		pDeckData->m_dDiaMainRebarLower_Dan2nd[1]	= _dRebarDia[stCantR.m_Rein_Combo2_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Dan[1]	= _dRebarDia[stCantR.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTCLower[1]		= stCantR.m_Rein_Ctc1;	
		pDeckData->m_dGenSupportRebarCTCLower[1]	= stCantR.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTCLower[1]		= stCantR.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTCLower[1]	= stCantR.m_Rein_CtcB1;	

		// 중앙부
		if(pGlopt->GetSlabCentDesignMethod()!=1)	// 경험적이 아니면...
		{
			// 상부	
			pDeckData->m_dDiaMainRebar_Cent[2]			= _dRebarDia[stCantC.m_Rein_Combo1];
			pDeckData->m_dDiaMainRebar_Cent2nd[2]		= _dRebarDia[stCantC.m_Rein_Combo1_2Cy];
			pDeckData->m_dDiaBeryukRebar_Cent[2]		= _dRebarDia[stCantC.m_Rein_Combo3];
			pDeckData->m_dDiaMainRebar_Dan[2]			= _dRebarDia[stCantC.m_Rein_Combo2];
			pDeckData->m_dDiaMainRebar_Dan2nd[2]		= _dRebarDia[stCantC.m_Rein_Combo2_2Cy];		
			pDeckData->m_dDiaBeryukRebar_Dan[2]			= _dRebarDia[stCantC.m_Rein_Combo4];		

			pDeckData->m_dGenMainRebarCTC[2]			= stCantC.m_Rein_Ctc1;		
			pDeckData->m_dGenSupportRebarCTC[2]			= stCantC.m_Rein_CtcB;	
			pDeckData->m_dEndMainRebarCTC[2]			= stCantC.m_Rein_Ctc2;	
			pDeckData->m_dEndSupportRebarCTC[2]			= stCantC.m_Rein_CtcB1;	
			// 하부
			pDeckData->m_dDiaMainRebarLower_Cent[2]		= _dRebarDia[stCantC.m_Rein_Combo1];
			pDeckData->m_dDiaMainRebarLower_Cent2nd[2]	= _dRebarDia[stCantC.m_Rein_Combo1_2Cy];
			pDeckData->m_dDiaBeryukRebarLower_Cent[2]	= _dRebarDia[stCantC.m_Rein_Combo3];
			pDeckData->m_dDiaMainRebarLower_Dan[2]		= _dRebarDia[stCantC.m_Rein_Combo2];
			pDeckData->m_dDiaMainRebarLower_Dan2nd[2]	= _dRebarDia[stCantC.m_Rein_Combo2_2Cy];	
			pDeckData->m_dDiaBeryukRebarLower_Dan[2]	= _dRebarDia[stCantC.m_Rein_Combo4];		

			pDeckData->m_dGenMainRebarCTCLower[2]		= stCantC.m_Rein_Ctc1;		
			pDeckData->m_dGenSupportRebarCTCLower[2]	= stCantC.m_Rein_CtcB;	
			pDeckData->m_dEndMainRebarCTCLower[2]		= stCantC.m_Rein_Ctc2;	
			pDeckData->m_dEndSupportRebarCTCLower[2]	= stCantC.m_Rein_CtcB1;	
		}
		else	// 경험적
		{
			// 상부	
			pDeckData->m_dDiaMainRebar_Cent[2]			= _dRebarDia[stCantC.m_Rein_Up_Combo1_1Cy];
			pDeckData->m_dDiaMainRebar_Cent2nd[2]		= _dRebarDia[stCantC.m_Rein_Up_Combo1_2Cy];
			pDeckData->m_dDiaBeryukRebar_Cent[2]		= _dRebarDia[stCantC.m_Rein_Up_Combo2];
			pDeckData->m_dDiaMainRebar_Dan[2]			= _dRebarDia[stCantC.m_Rein_Combo2];
			pDeckData->m_dDiaMainRebar_Dan2nd[2]		= _dRebarDia[stCantC.m_Rein_Combo2_2Cy];		
			pDeckData->m_dDiaBeryukRebar_Dan[2]			= _dRebarDia[stCantC.m_Rein_Combo4];		

			pDeckData->m_dGenMainRebarCTC[2]			= stCantC.m_Rein_Up_Ctc1;		
			pDeckData->m_dGenSupportRebarCTC[2]			= stCantC.m_Rein_Up_Ctc2;	
			pDeckData->m_dEndMainRebarCTC[2]			= stCantC.m_Rein_Ctc2;	
			pDeckData->m_dEndSupportRebarCTC[2]			= stCantC.m_Rein_CtcB1;	
			// 하부
			pDeckData->m_dDiaMainRebarLower_Cent[2]		= _dRebarDia[stCantC.m_Rein_Up_Combo1_1Cy];
			pDeckData->m_dDiaMainRebarLower_Cent2nd[2]	= _dRebarDia[stCantC.m_Rein_Up_Combo1_2Cy];
			pDeckData->m_dDiaBeryukRebarLower_Cent[2]	= _dRebarDia[stCantC.m_Rein_Up_Combo2];
			pDeckData->m_dDiaMainRebarLower_Dan[2]		= _dRebarDia[stCantC.m_Rein_Combo2];
			pDeckData->m_dDiaMainRebarLower_Dan2nd[2]	= _dRebarDia[stCantC.m_Rein_Combo2_2Cy];		
			pDeckData->m_dDiaBeryukRebarLower_Dan[2]	= _dRebarDia[stCantC.m_Rein_Combo4];		

			pDeckData->m_dGenMainRebarCTCLower[2]		= stCantC.m_Rein_Up_Ctc1;		
			pDeckData->m_dGenSupportRebarCTCLower[2]	= stCantC.m_Rein_Up_Ctc2;	
			pDeckData->m_dEndMainRebarCTCLower[2]		= stCantC.m_Rein_Ctc2;	
			pDeckData->m_dEndSupportRebarCTCLower[2]	= stCantC.m_Rein_CtcB1;	
		}

		// 지점부 바닥판
		pDeckData->m_dDiaJijumRebarUp[0]			= pCalcData->m_dia1;	
		pDeckData->m_dDiaJijumRebarDn[0]			= pCalcData->m_dia2;	
		pDeckData->m_dJijumRebarCTCUp[0]			= pCalcData->m_Ctc1;
		pDeckData->m_dJijumRebarCTCDn[0]			= pCalcData->m_Ctc2;
		pDeckData->m_dDiaJijumRebarUp[1]			= pCalcData->m_dia1;	
		pDeckData->m_dDiaJijumRebarDn[1]			= pCalcData->m_dia2;	
		pDeckData->m_dJijumRebarCTCUp[1]			= pCalcData->m_Ctc1;
		pDeckData->m_dJijumRebarCTCDn[1]			= pCalcData->m_Ctc2;
		pDeckData->m_dDiaJijumRebarUp[2]			= pCalcData->m_dia1;
		pDeckData->m_dDiaJijumRebarDn[2]			= pCalcData->m_dia2;
		pDeckData->m_dJijumRebarCTCUp[2]			= pCalcData->m_Ctc1;
		pDeckData->m_dJijumRebarCTCDn[2]			= pCalcData->m_Ctc2;
	}
	else
	{
		// 상부	(좌측)
		pDeckData->m_dDiaMainRebar_Cent[0]			= _dRebarDia[stCantL.m_nGenReinTDiaIdx_1Cy];	
		pDeckData->m_dDiaMainRebar_Cent2nd[0]		= _dRebarDia[stCantL.m_nGenReinTDiaIdx_2Cy];		
		pDeckData->m_dDiaBeryukRebar_Cent[0]		= _dRebarDia[stCantL.m_Rein_Combo3];			
		pDeckData->m_dDiaMainRebar_Dan[0]			= _dRebarDia[stCantL.m_Rein_Combo2];		
		pDeckData->m_dDiaMainRebar_Dan2nd[0]		= _dRebarDia[stCantL.m_Rein_Combo2_2Cy];		
		pDeckData->m_dDiaBeryukRebar_Dan[0]			= _dRebarDia[stCantL.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTC[0]			= stCantL.m_dGenReinTCTC;;		
		pDeckData->m_dGenSupportRebarCTC[0]			= stCantL.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTC[0]			= stCantL.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTC[0]			= stCantL.m_Rein_CtcB1;	
		// 하부
		pDeckData->m_dDiaMainRebarLower_Cent[0]		= _dRebarDia[stCantL.m_nGenReinCDiaIdx_1Cy];	
		pDeckData->m_dDiaMainRebarLower_Cent2nd[0]	= _dRebarDia[stCantL.m_nGenReinCDiaIdx_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Cent[0]	= _dRebarDia[stCantL.m_Rein_Combo3];		
		pDeckData->m_dDiaMainRebarLower_Dan[0]		= _dRebarDia[stCantL.m_Rein_Combo2];			
		pDeckData->m_dDiaMainRebarLower_Dan2nd[0]	= _dRebarDia[stCantL.m_Rein_Combo2_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Dan[0]	= _dRebarDia[stCantL.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTCLower[0]		= stCantL.m_dGenReinCCTC;		
		pDeckData->m_dGenSupportRebarCTCLower[0]	= stCantL.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTCLower[0]		= stCantL.m_Rein_Ctc2;		
		pDeckData->m_dEndSupportRebarCTCLower[0]	= stCantL.m_Rein_CtcB1;	

		// 상부	(우측)
		pDeckData->m_dDiaMainRebar_Cent[1]			= _dRebarDia[stCantR.m_nGenReinTDiaIdx_1Cy];	
		pDeckData->m_dDiaMainRebar_Cent2nd[1]		= _dRebarDia[stCantR.m_nGenReinTDiaIdx_2Cy];	
		pDeckData->m_dDiaBeryukRebar_Cent[1]		= _dRebarDia[stCantR.m_Rein_Combo3];			
		pDeckData->m_dDiaMainRebar_Dan[1]			= _dRebarDia[stCantR.m_Rein_Combo2];		
		pDeckData->m_dDiaMainRebar_Dan2nd[1]		= _dRebarDia[stCantR.m_Rein_Combo2_2Cy];	
		pDeckData->m_dDiaBeryukRebar_Dan[1]			= _dRebarDia[stCantR.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTC[1]			= stCantR.m_dGenReinTCTC;		
		pDeckData->m_dGenSupportRebarCTC[1]			= stCantR.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTC[1]			= stCantR.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTC[1]			= stCantR.m_Rein_CtcB1;	
		// 하부
		pDeckData->m_dDiaMainRebarLower_Cent[1]		= _dRebarDia[stCantR.m_nGenReinCDiaIdx_1Cy];	
		pDeckData->m_dDiaMainRebarLower_Cent2nd[1]	= _dRebarDia[stCantR.m_nGenReinCDiaIdx_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Cent[1]	= _dRebarDia[stCantR.m_Rein_Combo3];		
		pDeckData->m_dDiaMainRebarLower_Dan[1]		= _dRebarDia[stCantR.m_Rein_Combo2];			
		pDeckData->m_dDiaMainRebarLower_Dan2nd[1]	= _dRebarDia[stCantR.m_Rein_Combo2_2Cy];		
		pDeckData->m_dDiaBeryukRebarLower_Dan[1]	= _dRebarDia[stCantR.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTCLower[1]		= stCantR.m_dGenReinCCTC;	
		pDeckData->m_dGenSupportRebarCTCLower[1]	= stCantR.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTCLower[1]		= stCantR.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTCLower[1]	= stCantR.m_Rein_CtcB1;	

		// 상부(중앙)
		pDeckData->m_dDiaMainRebar_Cent[2]			= _dRebarDia[stCantC.m_nGenReinTDiaIdx_1Cy];
		pDeckData->m_dDiaMainRebar_Cent2nd[2]		= _dRebarDia[stCantC.m_nGenReinTDiaIdx_2Cy];
		pDeckData->m_dDiaBeryukRebar_Cent[2]		= _dRebarDia[stCantC.m_Rein_Combo3];
		pDeckData->m_dDiaMainRebar_Dan[2]			= _dRebarDia[stCantC.m_Rein_Combo2];
		pDeckData->m_dDiaMainRebar_Dan2nd[2]		= _dRebarDia[stCantC.m_Rein_Combo2_2Cy];		
		pDeckData->m_dDiaBeryukRebar_Dan[2]			= _dRebarDia[stCantC.m_Rein_Combo4];		
		
		pDeckData->m_dGenMainRebarCTC[2]			= stCantC.m_dGenReinTCTC;		
		pDeckData->m_dGenSupportRebarCTC[2]			= stCantC.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTC[2]			= stCantC.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTC[2]			= stCantC.m_Rein_CtcB1;	
		// 하부
		pDeckData->m_dDiaMainRebarLower_Cent[2]		= _dRebarDia[stCantC.m_nGenReinCDiaIdx_1Cy];
		pDeckData->m_dDiaMainRebarLower_Cent2nd[2]	= _dRebarDia[stCantC.m_nGenReinCDiaIdx_2Cy];
		pDeckData->m_dDiaBeryukRebarLower_Cent[2]	= _dRebarDia[stCantC.m_Rein_Combo3];
		pDeckData->m_dDiaMainRebarLower_Dan[2]		= _dRebarDia[stCantC.m_Rein_Combo2];
		pDeckData->m_dDiaMainRebarLower_Dan2nd[2]	= _dRebarDia[stCantC.m_Rein_Combo2_2Cy];	
		pDeckData->m_dDiaBeryukRebarLower_Dan[2]	= _dRebarDia[stCantC.m_Rein_Combo4];		

		pDeckData->m_dGenMainRebarCTCLower[2]		= stCantC.m_dGenReinCCTC;		
		pDeckData->m_dGenSupportRebarCTCLower[2]	= stCantC.m_Rein_CtcB;	
		pDeckData->m_dEndMainRebarCTCLower[2]		= stCantC.m_Rein_Ctc2;	
		pDeckData->m_dEndSupportRebarCTCLower[2]	= stCantC.m_Rein_CtcB1;	

		// 지점부 바닥판
		pDeckData->m_dDiaJijumRebarUp[0]			= pCalcData->m_dia1;	
		pDeckData->m_dDiaJijumRebarDn[0]			= pCalcData->m_dia2;	
		pDeckData->m_dJijumRebarCTCUp[0]			= pCalcData->m_Ctc1;
		pDeckData->m_dJijumRebarCTCDn[0]			= pCalcData->m_Ctc2;
		pDeckData->m_dDiaJijumRebarUp[1]			= pCalcData->m_dia1;	
		pDeckData->m_dDiaJijumRebarDn[1]			= pCalcData->m_dia2;	
		pDeckData->m_dJijumRebarCTCUp[1]			= pCalcData->m_Ctc1;
		pDeckData->m_dJijumRebarCTCDn[1]			= pCalcData->m_Ctc2;
		pDeckData->m_dDiaJijumRebarUp[2]			= pCalcData->m_dia1;
		pDeckData->m_dDiaJijumRebarDn[2]			= pCalcData->m_dia2;
		pDeckData->m_dJijumRebarCTCUp[2]			= pCalcData->m_Ctc1;
		pDeckData->m_dJijumRebarCTCDn[2]			= pCalcData->m_Ctc2;
	}
	
	pDeckData->m_dDiaSabogangUp					= _dRebarDia[stCantL.m_Rein_Combo2];	
	pDeckData->m_dDiaSabogangDn					= _dRebarDia[stCantL.m_Rein_Combo2];	
	pDeckData->m_dSabogangRebarCTCUp			= 100;
	pDeckData->m_dSabogangRebarCTCDn			= 100;
	pDeckData->m_dDiaDanbuHunch					= _dRebarDia[stCantL.m_Rein_Combo2];	 
}


void CDeckRebarInfoDlg::SetDataDefault() 
{
	// 경험적설계법이 아닐경우 바닥판 철근정보를 가져와서 상하철근을 동일값을 사용한다
	// 경험적설계법일 경우 일반부 주철근, 배근철근의 상하철근값이 다르기 때문에 단부 철근만 상하 동일값을 사용한다
	// ConvertFloorDataToRebarData()함수에 위사항 적용됨
	ConvertFloorDataToRebarData();

	m_pStd->m_pDoc->SetModifiedFlag();
}

void CDeckRebarInfoDlg::SetDataInit() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CPlateBridgeApp *pDB = m_pStd->m_pDataManage->GetBridge();	

	BOOL bSkewSeg	= pDeckData->IsSkew();
	BOOL bOneJigan	= pDB->m_nQtyJigan==1 ? TRUE : FALSE;

	SetGridData();
	m_Grid.UpdateData(FALSE);
	
	long row(0), col(0);
	m_Grid.GetCellCount(row, col);
	m_Grid.EnableInsertRow(FALSE);	
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);

	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(7,85);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 120);
	m_Grid.SetColumnWidth(3, 70);
	m_Grid.SetColumnWidth(4, 70);
	m_Grid.SetColumnWidth(5, 70);
	m_Grid.SetColumnWidth(6, 70);
	m_Grid.SetColumnWidth(7, 70);
	m_Grid.SetColumnWidth(8, 70);
	m_Grid.SetColumnWidth(9, 0);
	
	if(bOneJigan)
	{
		m_Grid.SetRowHeight(14, 0);
		m_Grid.SetRowHeight(15, 0);
		m_Grid.SetRowHeight(16, 0);
	}
	m_Grid.MergeGrid(0, 1, 0, 2);
	m_Grid.MergeGrid(2, bSkewSeg?18:17, 0, 2);
	m_Grid.MergeGrid(0, 1, 3, 9);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDeckRebarInfoDlg::SetDataSave() 
{
	m_Grid.UpdateData(TRUE);
}


BOOL CDeckRebarInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	
	m_Grid.ViewGridCellForCombo(pMsg);
	
	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CDeckRebarInfoDlg::OnCellChangedData(NMHDR* nmgv, LRESULT*)
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = pGridView->iRow;
    int nCol = pGridView->iColumn;

	SetDataSave();

	if(nRow==16 && nCol==4)
	{
		CString sText = m_Grid.GetTextMatrix(nRow, nCol);
		sText.Delete(0);
		pDeckData->m_dDiaJijumRebarUp[0] =	atof(sText);
		pDeckData->m_dDiaJijumRebarUp[1] =	atof(sText);
		pDeckData->m_dDiaJijumRebarUp[2] =	atof(sText);
	}

	if(nRow==16 && nCol==7)
	{
		CString sText = m_Grid.GetTextMatrix(nRow, nCol);
		sText.Delete(0);
		pDeckData->m_dDiaJijumRebarDn[0] =	atof(sText);
		pDeckData->m_dDiaJijumRebarDn[1] =	atof(sText);
		pDeckData->m_dDiaJijumRebarDn[2] =	atof(sText);
	}

	SetDataInit();
	SetDataSave();
	
	DrawInputDomyunView(FALSE);
}

void CDeckRebarInfoDlg::OnCellChanged(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;	
	
	int nRow = pGridView->iRow;

	CString str = m_Grid.GetTextMatrix(nRow, 1);
	
	m_pView->GetDomyun()->STMakeCursor(str);

	DrawInputDomyunView();
}



void CDeckRebarInfoDlg::OnButtonApplySameLRC() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;

	//////////////////////////////////////////////////////////////////////////
	// 상면 간격
	pDeckData->m_dGenMainRebarCTC[2] = pDeckData->m_dGenMainRebarCTC[1] = pDeckData->m_dGenMainRebarCTC[0] = m_Grid.GetTextMatrixDouble(2, 3);
	pDeckData->m_dEndMainRebarCTC[2] = pDeckData->m_dEndMainRebarCTC[1]= pDeckData->m_dEndMainRebarCTC[0] = m_Grid.GetTextMatrixDouble(5, 3);
	pDeckData->m_dGenSupportRebarCTC[2] = pDeckData->m_dGenSupportRebarCTC[1] = pDeckData->m_dGenSupportRebarCTC[0] = m_Grid.GetTextMatrixDouble(8, 3);
	pDeckData->m_dEndSupportRebarCTC[2] = pDeckData->m_dEndSupportRebarCTC[1] = pDeckData->m_dEndSupportRebarCTC[0] = m_Grid.GetTextMatrixDouble(11, 3);
	pDeckData->m_dJijumRebarCTCUp[2] = pDeckData->m_dJijumRebarCTCUp[1] = pDeckData->m_dJijumRebarCTCUp[0] = m_Grid.GetTextMatrixDouble(14, 3);
	//////////////////////////////////////////////////////////////////////////
	// 상면 직경1Cycle
	CString sz = m_Grid.GetTextMatrix(2, 4);	sz.Delete(0);
	pDeckData->m_dDiaMainRebar_Cent[2] = pDeckData->m_dDiaMainRebar_Cent[1] = pDeckData->m_dDiaMainRebar_Cent[0] = atof(sz);
	sz = m_Grid.GetTextMatrix(5, 4);	sz.Delete(0);
	pDeckData->m_dDiaMainRebar_Dan[2] = pDeckData->m_dDiaMainRebar_Dan[1] = pDeckData->m_dDiaMainRebar_Dan[0] = atof(sz);
	sz = m_Grid.GetTextMatrix(8, 4);	sz.Delete(0);
	pDeckData->m_dDiaBeryukRebar_Cent[2] = pDeckData->m_dDiaBeryukRebar_Cent[1] = pDeckData->m_dDiaBeryukRebar_Cent[0] = atof(sz);
	sz = m_Grid.GetTextMatrix(11, 4);	sz.Delete(0);
	pDeckData->m_dDiaBeryukRebar_Dan[2] = pDeckData->m_dDiaBeryukRebar_Dan[1] = pDeckData->m_dDiaBeryukRebar_Dan[0] = atof(sz);
	sz = m_Grid.GetTextMatrix(14, 4);	sz.Delete(0);
	pDeckData->m_dDiaJijumRebarUp[2] = pDeckData->m_dDiaJijumRebarUp[1] = pDeckData->m_dDiaJijumRebarUp[0] = atof(sz);
	//////////////////////////////////////////////////////////////////////////
	// 상면 직경2Cycle
	sz = m_Grid.GetTextMatrix(2, 5);	sz.Delete(0);
	pDeckData->m_dDiaMainRebar_Cent2nd[2] = pDeckData->m_dDiaMainRebar_Cent2nd[1] = pDeckData->m_dDiaMainRebar_Cent2nd[0] = atof(sz);
	sz = m_Grid.GetTextMatrix(5, 5);	sz.Delete(0);
	pDeckData->m_dDiaMainRebar_Dan2nd[2] = pDeckData->m_dDiaMainRebar_Dan2nd[1] = pDeckData->m_dDiaMainRebar_Dan2nd[0] = atof(sz);
	//////////////////////////////////////////////////////////////////////////
	// 하면 간격
	pDeckData->m_dGenMainRebarCTCLower[2] = pDeckData->m_dGenMainRebarCTCLower[1] = pDeckData->m_dGenMainRebarCTCLower[0] = m_Grid.GetTextMatrixDouble(2, 6);
	pDeckData->m_dEndMainRebarCTCLower[2] = pDeckData->m_dEndMainRebarCTCLower[1] = pDeckData->m_dEndMainRebarCTCLower[0] = m_Grid.GetTextMatrixDouble(5, 6);
	pDeckData->m_dGenSupportRebarCTCLower[2] = pDeckData->m_dGenSupportRebarCTCLower[1] = pDeckData->m_dGenSupportRebarCTCLower[0] = m_Grid.GetTextMatrixDouble(8, 6);
	pDeckData->m_dEndSupportRebarCTCLower[2] = pDeckData->m_dEndSupportRebarCTCLower[1] = pDeckData->m_dEndSupportRebarCTCLower[0] = m_Grid.GetTextMatrixDouble(11, 6);
	pDeckData->m_dJijumRebarCTCDn[2] = pDeckData->m_dJijumRebarCTCDn[1] = pDeckData->m_dJijumRebarCTCDn[0] = m_Grid.GetTextMatrixDouble(14,6);	
	//////////////////////////////////////////////////////////////////////////
	// 하면 직경1Cycle
	sz = m_Grid.GetTextMatrix(2, 7);	sz.Delete(0);
	pDeckData->m_dDiaMainRebarLower_Cent[2] = pDeckData->m_dDiaMainRebarLower_Cent[1] = pDeckData->m_dDiaMainRebarLower_Cent[0] = atof(sz);		
	sz = m_Grid.GetTextMatrix(5, 7);	sz.Delete(0);
	pDeckData->m_dDiaMainRebarLower_Dan[2] = pDeckData->m_dDiaMainRebarLower_Dan[1] = pDeckData->m_dDiaMainRebarLower_Dan[0] = atof(sz);			
	sz = m_Grid.GetTextMatrix(8, 7);	sz.Delete(0);
	pDeckData->m_dDiaBeryukRebarLower_Cent[2] = pDeckData->m_dDiaBeryukRebarLower_Cent[1] = pDeckData->m_dDiaBeryukRebarLower_Cent[0] = atof(sz);			
	sz = m_Grid.GetTextMatrix(11, 7);	sz.Delete(0);
	pDeckData->m_dDiaBeryukRebarLower_Dan[2] = pDeckData->m_dDiaBeryukRebarLower_Dan[1] = pDeckData->m_dDiaBeryukRebarLower_Dan[0] = atof(sz);			
	sz = m_Grid.GetTextMatrix(14, 7);	sz.Delete(0);
	pDeckData->m_dDiaJijumRebarDn[2] = pDeckData->m_dDiaJijumRebarDn[1] = pDeckData->m_dDiaJijumRebarDn[0] = atof(sz);			
	//////////////////////////////////////////////////////////////////////////
	// 하면 직경2Cycle
	sz = m_Grid.GetTextMatrix(2, 8);	sz.Delete(0);
	pDeckData->m_dDiaMainRebarLower_Cent2nd[2] = pDeckData->m_dDiaMainRebarLower_Cent2nd[1] = pDeckData->m_dDiaMainRebarLower_Cent2nd[0] = atof(sz);	
	sz = m_Grid.GetTextMatrix(5, 8);	sz.Delete(0);
	pDeckData->m_dDiaMainRebarLower_Dan2nd[2] = pDeckData->m_dDiaMainRebarLower_Dan2nd[1] = pDeckData->m_dDiaMainRebarLower_Dan2nd[0] = atof(sz);			

	SetDataInit();
	SetDataSave();	
}


BOOL CDeckRebarInfoDlg::IsValid()
{
	return m_pStd->m_bInclude_Module_Deck;
}


void CDeckRebarInfoDlg::OnButtonApplyAtCalcData() 
{
	CADeckData *pDeckData = m_pStd->m_pDeckData;
	CGlobarOption *pGlopt = m_pStd->m_pDataManage->GetGlobalOption(); 
	CCalcData *pCalcData = m_pStd->m_pDataManage->GetCalcData();

	CCalcData::_CALC_CANTILEVER_DATA *pStCantL = &m_pStd->m_pDataManage->GetCalcData()->CALC_CANTILEVER_DATA[FLOOR_LEFT];
	CCalcData::_CALC_CANTILEVER_DATA *pStCantR = &m_pStd->m_pDataManage->GetCalcData()->CALC_CANTILEVER_DATA[FLOOR_RIGHT];
	CCalcData::_CALC_CANTILEVER_DATA *pStCantC = &m_pStd->m_pDataManage->GetCalcData()->CALC_CANTILEVER_DATA[FLOOR_MID];

	// CARailFloorData의 철근 제원값을 ADeckData의 철근 제원값에 복사함.    
	// 상부	(좌측)
	pStCantL->m_Rein_Combo1	    = GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent[0]);	
	pStCantL->m_Rein_Combo1_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent2nd[0]);	
	pStCantL->m_Rein_Combo3		= GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Cent[0]);	
	pStCantL->m_Rein_Combo2		= GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan[0]);	
	pStCantL->m_Rein_Combo2_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan2nd[0]);	
	pStCantL->m_Rein_Combo4		= GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Dan[0]);	

	pStCantL->m_Rein_Ctc1 = pDeckData->m_dGenMainRebarCTC[0];
	pStCantL->m_Rein_CtcB = pDeckData->m_dGenSupportRebarCTC[0];
	pStCantL->m_Rein_Ctc2 = pDeckData->m_dEndMainRebarCTC[0];
	pStCantL->m_Rein_CtcB1= pDeckData->m_dEndSupportRebarCTC[0];
	// 하부
	pStCantL->m_Rein_Combo1		= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent[0]);	
	pStCantL->m_Rein_Combo1_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent2nd[0]);	
	pStCantL->m_Rein_Combo3		= GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Cent[0]);	
	pStCantL->m_Rein_Combo2		= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan[0]);	
	pStCantL->m_Rein_Combo2_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan2nd[0]);		
	pStCantL->m_Rein_Combo4		= GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Dan[0]);	

	pStCantL->m_Rein_Ctc1	= pDeckData->m_dGenMainRebarCTCLower[0];	
	pStCantL->m_Rein_CtcB = pDeckData->m_dGenSupportRebarCTCLower[0];
	pStCantL->m_Rein_Ctc2 = pDeckData->m_dEndMainRebarCTCLower[0];	
	pStCantL->m_Rein_CtcB1= pDeckData->m_dEndSupportRebarCTCLower[0];

	// 상부	(우측)
	pStCantR->m_Rein_Combo1	    = GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent[1]);	
	pStCantR->m_Rein_Combo1_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent2nd[1]);	
	pStCantR->m_Rein_Combo3		= GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Cent[1]);	
	pStCantR->m_Rein_Combo2		= GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan[1]);	
	pStCantR->m_Rein_Combo2_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan2nd[1]);	
	pStCantR->m_Rein_Combo4		= GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Dan[1]);	

	pStCantR->m_Rein_Ctc1 = pDeckData->m_dGenMainRebarCTC[1];
	pStCantR->m_Rein_CtcB = pDeckData->m_dGenSupportRebarCTC[1];
	pStCantR->m_Rein_Ctc2 = pDeckData->m_dEndMainRebarCTC[1];
	pStCantR->m_Rein_CtcB1= pDeckData->m_dEndSupportRebarCTC[1];
	// 하부
	pStCantR->m_Rein_Combo1		= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent[1]);	
	pStCantR->m_Rein_Combo1_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent2nd[1]);	
	pStCantR->m_Rein_Combo3		= GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Cent[1]);	
	pStCantR->m_Rein_Combo2		= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan[1]);	
	pStCantR->m_Rein_Combo2_2Cy	= GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan2nd[1]);		
	pStCantR->m_Rein_Combo4		= GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Dan[1]);	

	pStCantR->m_Rein_Ctc1	= pDeckData->m_dGenMainRebarCTCLower[1];	
	pStCantR->m_Rein_CtcB = pDeckData->m_dGenSupportRebarCTCLower[1];
	pStCantR->m_Rein_Ctc2 = pDeckData->m_dEndMainRebarCTCLower[1];	
	pStCantR->m_Rein_CtcB1= pDeckData->m_dEndSupportRebarCTCLower[1];

	// 중앙부
	if(pGlopt->GetSlabCentDesignMethod()!=1)	// 경험적이 아니면...
	{
		CString strText = _T("");

		// 상부	
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent[2]));
		pStCantC->m_Rein_CaseH1		= strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent2nd[2]));
		pStCantC->m_Rein_CaseH1_2Cy	= strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Cent[2]));
		pStCantC->m_Rein_CaseH3		= strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan[2]));
		pStCantC->m_Rein_CaseH2		= strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan2nd[2]));
		pStCantC->m_Rein_CaseH2_2Cy	= strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Dan[2]));
		pStCantC->m_Rein_CaseH4		= strText;

		pStCantC->m_Rein_Ctc1  = pDeckData->m_dGenMainRebarCTC[2];
		pStCantC->m_Rein_CtcB  = pDeckData->m_dGenSupportRebarCTC[2];
		pStCantC->m_Rein_Ctc2  = pDeckData->m_dEndMainRebarCTC[2];
		pStCantC->m_Rein_CtcB1 = pDeckData->m_dEndSupportRebarCTC[2];
		// 하부
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent[2]));
		pStCantC->m_Rein_CaseH1	  = strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent2nd[2]));
		pStCantC->m_Rein_CaseH1_2Cy = strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Cent[2]));
		pStCantC->m_Rein_CaseH3	  = strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan[2]));
		pStCantC->m_Rein_CaseH2	  = strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan2nd[2]));
		pStCantC->m_Rein_CaseH2_2Cy = strText;
		strText.Format("%f", GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Dan[2]));
		pStCantC->m_Rein_CaseH4	  = strText;

		pStCantC->m_Rein_Ctc1 = pDeckData->m_dGenMainRebarCTCLower[2];
		pStCantC->m_Rein_CtcB = pDeckData->m_dGenSupportRebarCTCLower[2];
		pStCantC->m_Rein_Ctc2 = pDeckData->m_dEndMainRebarCTCLower[2];
		pStCantC->m_Rein_CtcB1= pDeckData->m_dEndSupportRebarCTCLower[2];
	}
	else	// 경험적
	{
		CString strText = _T("");
		// 상부	
		pStCantC->m_Rein_Up_Combo1_1Cy = GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent[2]);
		pStCantC->m_Rein_Up_Combo1_2Cy = GetRebarIdx(pDeckData->m_dDiaMainRebar_Cent2nd[2]);
		pStCantC->m_Rein_Up_Combo2	 = GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Cent[2]);
		strText.Format("%f",GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan[2]));
		pStCantC->m_Rein_CaseH2		 = strText;
		strText.Format("%f",GetRebarIdx(pDeckData->m_dDiaMainRebar_Dan2nd[2]));
		pStCantC->m_Rein_CaseH2_2Cy	 = strText;
		strText.Format("%f",GetRebarIdx(pDeckData->m_dDiaBeryukRebar_Dan[2]));
		pStCantC->m_Rein_CaseH4		 = strText;

		pStCantC->m_Rein_Up_Ctc1 = pDeckData->m_dGenMainRebarCTC[2];
		pStCantC->m_Rein_Up_Ctc2 = pDeckData->m_dGenSupportRebarCTC[2];
		pStCantC->m_Rein_Ctc2    = pDeckData->m_dEndMainRebarCTC[2];
		pStCantC->m_Rein_CtcB1   = pDeckData->m_dEndSupportRebarCTC[2];
		// 하부
		pStCantC->m_Rein_Up_Combo1_1Cy = GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent[2]);
		pStCantC->m_Rein_Up_Combo1_2Cy = GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Cent2nd[2]);
		pStCantC->m_Rein_Up_Combo2	 = GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Cent[2]);
		strText.Format("%f",GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan[2]));
		pStCantC->m_Rein_CaseH2		 = strText;
		strText.Format("%f",GetRebarIdx(pDeckData->m_dDiaMainRebarLower_Dan2nd[2]));
		pStCantC->m_Rein_CaseH2_2Cy	 = strText;
		strText.Format("%f",GetRebarIdx(pDeckData->m_dDiaBeryukRebarLower_Dan[2]));
		pStCantC->m_Rein_CaseH4		 = strText;

		pStCantC->m_Rein_Up_Ctc1 = pDeckData->m_dGenMainRebarCTCLower[2];
		pStCantC->m_Rein_Up_Ctc2 = pDeckData->m_dGenSupportRebarCTCLower[2];
		pStCantC->m_Rein_Ctc2    = pDeckData->m_dEndMainRebarCTCLower[2];
		pStCantC->m_Rein_CtcB1   = pDeckData->m_dEndSupportRebarCTCLower[2];
	}

	// 지점부 바닥판
	pCalcData->m_dia1 = pDeckData->m_dDiaJijumRebarUp[0];
	pCalcData->m_dia2 = pDeckData->m_dDiaJijumRebarDn[0];
	pCalcData->m_Ctc1 = pDeckData->m_dJijumRebarCTCUp[0];
	pCalcData->m_Ctc2 = pDeckData->m_dJijumRebarCTCDn[0];
	
}

long CDeckRebarInfoDlg::GetRebarIdx(double dDia)
{
	long nIdx	= 0;
	switch(long(dDia)) 
	{
	case 13: nIdx = 0;
		break;
	case 16: nIdx = 1;
		break;
	case 19: nIdx = 2;
		break;
	case 22: nIdx = 3;
		break;
	case 25: nIdx = 4;
		break;
	case 29: nIdx = 5;
		break;
	case 32: nIdx = 6;
		break;
	case 35: nIdx = 7;
		break;
	default: break;
	}

	return nIdx;
}
