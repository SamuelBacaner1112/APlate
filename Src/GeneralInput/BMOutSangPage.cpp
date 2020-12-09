// BMOutSangPage.cpp : implementation file 
// 

#include "stdafx.h"
#include "../PlateGangje\Gangje.h"
#include "BMOutSheet.h"
#include "BMOutSangPage.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 
 
/////////////////////////////////////////////////////////////////////////////
// CBMOutSangPage property page

IMPLEMENT_DYNCREATE(CBMOutSangPage, CPropertyPage)

CBMOutSangPage::CBMOutSangPage() : CPropertyPage(CBMOutSangPage::IDD)
{
	//{{AFX_DATA_INIT(CBMOutSangPage)
	//}}AFX_DATA_INIT
	m_pGrid = new CGridCtrl;
	m_sTotalRecord = _T("");
	m_sTotalWeight = _T(""); 
	m_sUnitWeight  = _T("");
}

CBMOutSangPage::~CBMOutSangPage()
{
	delete m_pGrid;
}

void CBMOutSangPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMOutSangPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBMOutSangPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBMOutSangPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMOutSangPage message handlers

int CBMOutSangPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect(0,0,0,0);
	GetClientRect(&rect);	
	rect.left += 5;
	rect.right -= 5;
	rect.top += 5;	
	rect.bottom -= 5;	

	if(!m_pGrid->GetSafeHwnd())
	{
		m_pGrid->Create(rect,this,IDC_GRID_BM,WS_BORDER|WS_CHILD|WS_VISIBLE);
		CFont Font;
		Font.CreatePointFont(90,"����ü");
		m_pGrid->SendMessage(WM_SETFONT,(WPARAM)(HFONT)Font,0);
	}
	
	
	return 0;
}

UINT CBMOutSangPage::DoWork()
{
	m_pGrid->SetRunningThread(TRUE);
	SetGrid();
	DrawGrid();	
	m_pGrid->SetRunningThread(FALSE);
	return 0;
}

BOOL CBMOutSangPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SendMessage(WM_SIZE);
//	SetGrid();
//	DrawGrid();
	Begin();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBMOutSangPage::SetGrid()
{
	if(!GetSafeHwnd()) return;
	switch(m_nMakeFlag) 
	{
		case MAKE_BM_SANG :						SetGridSangPage();				break;// [01]�⺻���� ��������ǥ
		case MAKE_BM_STDANDARD_SANG :			SetGridStandardSangPage();		break;// [02]ǥ������ǥ
		case MAKE_BM_STDANDARD_SHOP :			SetGridStandardShopPage();		break;// [03]ǥ������ǥ(���ۿ�)
		case MAKE_BM_STDANDARD_SHOP_TYPE2 :		SetGridStandardShopType2Page();	break;// [03]ǥ������ǥ(���ۿ�)
		case MAKE_BM_THICK :					SetGridThickPage();				break;// [04]������ �β�������ǥ
		case MAKE_BM_THICK_SANG :				SetGridThickSangPage();			break;// [04]������ �β�������ǥ
		case MAKE_BM_BUJEMARK_SANG :			SetGridBujeMarkSangPage();		break;// [05]���縶ũ�� ������ǥ
		case MAKE_BM_BUJEMARK_TOTAL :			SetGridBujeMarkTotalPage();		break;// [06]���縶ũ�� �Ѱ�����ǥ
		case MAKE_BM_BUJEMARK_DEDUCT :			SetGridBujeMarkDeductPage();	break;// [07]���縶ũ�� ��������ǥ
		case MAKE_BM_CAR :						SetGridCarPage();				break;// [08]�������� ������ǥ
		case MAKE_BM_CARTOTAL :					SetGridCarTotalPage();			break;// [09]�������� �Ѱ�����ǥ
		case MAKE_BM_GRAND_ASSEMBLY_SANG :		SetGridGrandAssemSangPage();	break;// [10]�������� ������ǥ
		case MAKE_BM_GRAND_ASSEMBLY_TOTAL :		SetGridGrandAssemTotalPage();	break;// [11]�������� �Ѱ�����ǥ
		case MAKE_BM_MID_ASSEMBLY_SANG :		SetGridMidAssemSangPage();		break;// [12]�������� ������ǥ
		case MAKE_BM_MID_ASSEMBLY_TOTAL :		SetGridMidAssemTotalPage();		break;// [13]�������� �Ѱ�����ǥ
		case MAKE_BM_ASSEMBLY_SANG :			SetGridAssemSangPage();			break;// [14]�������� ������ǥ
		case MAKE_BM_ASSEMBLY_TOTAL :			SetGridAssemTotalPage();		break;// [15]�������� �Ѱ�����ǥ
		case MAKE_BM_ASSEMBLY_DEDUCT :			SetGridAssemDeductPage();		break;// [16]�������� ��������ǥ
		case MAKE_BM_ASSEMBLY_FACT :			SetGridAssemFactPage();			break;// [17]�������� �������ۿ�����ǥ
		case MAKE_BM_ASSEMBLY_FIELD :			SetGridAssemFieldPage();		break;// [18]�������� ���弳ġ������ǥ
		case MAKE_BM_SECTION_COMP :				SetGridSectionCompPage();		break;// [19]�ܸ麰 ����ǥ(�⺻�������)
		case MAKE_BM_BUJE_COMP :				SetGridBujeCompPage();			break;// [20]���纰 ����ǥ(�⺻�������)
		case MAKE_BM_BASIC_COMP :				SetGridBasicCompPage();			break;// [21]�⺻�� ����� ����ǥ
		//
		case MAKE_BM_SECTIONTYPE :				SetGridSectionTypePage();		break;
		case MAKE_BM_BUJE :						SetGridBujePage();				break;
		case MAKE_BM_BUJE_TOTAL :				SetGridBujeTotalPage();			break;
		case MAKE_BM_GIRDER_TOTAL :				SetGridGirderTotalPage();		break;
		case MAKE_BM_BUJEKIND_TOTAL :			SetGridBujeKindTotalPage();		break;
		case MAKE_BM_DEDUCT_SANG :				SetGridDeductSangPage();		break;
		case MAKE_BM_DEDUCT_BUJE :				SetGridDeductBujePage();		break;
	}

}

void CBMOutSangPage::SetGridCarPage()			
{
	long nRows = 0;
	long nCols = 16;

	char *Title[] = {"\n�Ŵ�",
					 "\n����\n����",
					 "\n����",
					 "\n�����",
					 "\n���縶ũ",//e4
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n������\n��ũ",//e12
					 "\n������\n��ũ",//e13
					 "\n������\n��ũ",//e14
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
//	if(bUnitTon) Title[11] = "\n���߷�\n(Tonf)";
	if(bUnitTon) Title[10] = "\n���߷�\n(Tonf)";
		
	InitGridFirst(nRows,nCols);		
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth( 0, 45);
	m_pGrid->SetColumnWidth( 1, 45);
	m_pGrid->SetColumnWidth( 2,120);
	m_pGrid->SetColumnWidth( 3,100);
	m_pGrid->SetColumnWidth(12,100);

	// AROAD�� ���
	if(m_pBMOutSheet->m_nARoadFlag==0)
	{
		m_pGrid->SetColumnWidth(4,0);
		m_pGrid->SetColumnWidth(12,0);
		m_pGrid->SetColumnWidth(13,0);
		m_pGrid->SetColumnWidth(14,0);
	}
/*	else
	{
		m_pGrid->SetColumnWidth(0,0);
		m_pGrid->SetColumnWidth(1,0);
		m_pGrid->SetColumnWidth(2,0);
		m_pGrid->SetColumnWidth(3,0);
	}*/

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridDeductSangPage()	{	SetGridSangPage(); }
//�� ����ǥ
void CBMOutSangPage::SetGridSangPage()
{	
	long nRows = 0;
	long nCols = 37;

	char *Title[] = {"\n�Ŵ�",
					 "\n����\n����",
					 "\n����",
					 "\n�����",
					 "\n�ܸ鸶ũ",//erase at aroad
					 "\n���縶ũ",//e
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n������\n��ũ",//e
					 "\n������\n��ũ",//e
					 "\n������\n��ũ",//e
					 "\n�⺻�ܸ�",  //e16
					 "\n��������",  //e17
					 "\n����\n(��)",
					 "\n��������",  //e19
					 "\n���뵵",  //e20
					 "\n�������",
					 "\n�β�2\n(mm)",  ///hide from here
					 "\n�ʺ�2\n(mm)",
					 "�߷��Լ�",		// �߷���� <====== ���ϴ� HIDDEN COLUMN
					 "�����ȣ",		// ���縶ũ m_nMarkBuje
					 "��������ȣ",		// ��������ũ m_nMarkAssem
					 "��������ȣ",
					 "��������ȣ",
					 "�⺻�ܸ��ȣ",
					 "�ܸ��ȣ",		// �ܸ鸶ũ m_nMarkSection
					 "1���з�",			// ���ǥ���� 1���з� m_nFirstClass
					 "������ȣ",		// ������ȣ m_nEleNameOnlySectionNumber
					 "m_dStationRec",	// m_dStationRec
					 "m_dwFlag",		// m_dwFlag
					 "m_dwDisableFlag",	// m_dwDisableFlag
					 "m_nKindName",	// m_nKindName
				};
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[11] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth( 0, 45);
	m_pGrid->SetColumnWidth( 1, 45);
	m_pGrid->SetColumnWidth( 2,120);
	m_pGrid->SetColumnWidth( 3,100);
	m_pGrid->SetColumnWidth(13,100);

	InitGridTitle(nCols,Title);	
	// Hidden
	if(m_nMakeFlag==MAKE_BM_SANG || m_nMakeFlag==MAKE_BM_DEDUCT_SANG)
		for(long i=22; i<37; i++)
			m_pGrid->SetColumnWidth(i,0);

	if(m_pBMOutSheet->m_nARoadFlag==0)
	{
		m_pGrid->SetColumnWidth(4,0);
		m_pGrid->SetColumnWidth(5,0);
		m_pGrid->SetColumnWidth(13,0);
		m_pGrid->SetColumnWidth(14,0);
		m_pGrid->SetColumnWidth(15,0);
		m_pGrid->SetColumnWidth(16,0);
		m_pGrid->SetColumnWidth(17,0);
		m_pGrid->SetColumnWidth(19,0);
		m_pGrid->SetColumnWidth(20,0);
	}
}

//�ܸ�Ÿ�Ժ� ����ǥ
void CBMOutSangPage::SetGridSectionTypePage()
{
	long nRows = 0;
	long nCols = 9;
	char *Title[] = {"\n�ܸ�Ÿ��",
		             "\n�����",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[7] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(60);

	m_pGrid->SetColumnWidth(0,100);
	m_pGrid->SetColumnWidth(1,100);
	m_pGrid->SetColumnWidth(4,70);
	m_pGrid->SetColumnWidth(5,40);
	m_pGrid->SetColumnWidth(6,80);
	m_pGrid->SetColumnWidth(7,100);

	InitGridTitle(nCols,Title);	
}

//�β��� ����ǥ
void CBMOutSangPage::SetGridThickPage()
{
	long nRows = 0;
	long nCols = 7;
	char *Title[] = {"\n����",
					 "\n�β�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n��������"
					};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[5] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(1,100);
	m_pGrid->SetColumnWidth(5,100);
	m_pGrid->SetColumnWidth(6,80);
	
	if(m_pBMOutSheet->m_nARoadFlag==0)
		m_pGrid->SetColumnWidth(6,0);
	InitGridTitle(nCols,Title);	
}

//�β��� ����ǥ
void CBMOutSangPage::SetGridThickSangPage()
{
	long nRows =  0;
	long nCols = 11;
	char *Title[] = {"\n��з�",
					 "\n�ߺз�",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[8] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0,120);
	m_pGrid->SetColumnWidth(1,120);
	m_pGrid->SetColumnWidth(2,100);
	m_pGrid->SetColumnWidth(9,100);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridBujeTotalPage()	 
{		
	long nRows = 0;
	long nCols = 3;
	char *Title[] = {"\n�����",
					 "\n���߷�\n(kgf)",
					 "\n����"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[1] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(60);

	m_pGrid->SetColumnWidth(0,100);
	m_pGrid->SetColumnWidth(1,80);
	m_pGrid->SetColumnWidth(2,100);

	InitGridTitle(nCols,Title);		
}

void CBMOutSangPage::SetGridDeductBujePage() {	SetGridBujePage();}
//���纰 ����ǥ
void CBMOutSangPage::SetGridBujePage()
{	
	long nRows = 0;
	long nCols = 8;
	char *Title[] = {"\n�����",
		             "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[6] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(60);

	m_pGrid->SetColumnWidth(0,100);
	m_pGrid->SetColumnWidth(3,70);
	m_pGrid->SetColumnWidth(4,40);
	m_pGrid->SetColumnWidth(5,80);
	m_pGrid->SetColumnWidth(6,100);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::DrawGrid()
{
	if(!GetSafeHwnd()) return;
	switch(m_nMakeFlag) 
	{
		case MAKE_BM_SANG :						DrawGridSangPage();					break;// [01]�⺻���� ��������ǥ
		case MAKE_BM_STDANDARD_SANG :			DrawGridStandardSangPage();			break;// [02]ǥ������ǥ
		case MAKE_BM_STDANDARD_SHOP :			DrawGridStandardShopPage();			break;// [03]ǥ������ǥ(���ۿ�)
		case MAKE_BM_STDANDARD_SHOP_TYPE2 :		DrawGridStandardShopType2Page();	break;// [03]ǥ������ǥ(���ۿ�)
		case MAKE_BM_THICK :					DrawGridThickPage();				break;// [04]������ �β�������ǥ
		case MAKE_BM_THICK_SANG :				DrawGridThickSangPage();			break;// [04]������ �β�������ǥ
		case MAKE_BM_BUJEMARK_SANG :			DrawGridBujeMarkSangPage();			break;// [05]���縶ũ�� ������ǥ
		case MAKE_BM_BUJEMARK_TOTAL :			DrawGridBujeMarkTotalPage();		break;// [06]���縶ũ�� �Ѱ�����ǥ
		case MAKE_BM_BUJEMARK_DEDUCT :			DrawGridBujeMarkDeductPage();		break;// [07]���縶ũ�� ��������ǥ
		case MAKE_BM_CAR :						DrawGridCarPage();					break;// [08]�������� ������ǥ
		case MAKE_BM_CARTOTAL :					DrawGridCarTotalPage();				break;// [09]�������� �Ѱ�����ǥ
		case MAKE_BM_GRAND_ASSEMBLY_SANG :		DrawGridGrandAssemSangPage();		break;// [10]�������� ������ǥ
		case MAKE_BM_GRAND_ASSEMBLY_TOTAL :		DrawGridGrandAssemTotalPage();		break;// [11]�������� �Ѱ�����ǥ
		case MAKE_BM_MID_ASSEMBLY_SANG :		DrawGridMidAssemSangPage();			break;// [12]�������� ������ǥ
		case MAKE_BM_MID_ASSEMBLY_TOTAL :		DrawGridMidAssemTotalPage();		break;// [13]�������� �Ѱ�����ǥ
		case MAKE_BM_ASSEMBLY_SANG :			DrawGridAssemSangPage();			break;// [14]�������� ������ǥ
		case MAKE_BM_ASSEMBLY_TOTAL :			DrawGridAssemTotalPage();			break;// [15]�������� �Ѱ�����ǥ
		case MAKE_BM_ASSEMBLY_DEDUCT :			DrawGridAssemDeductPage();			break;// [16]�������� ��������ǥ
		case MAKE_BM_ASSEMBLY_FACT :			DrawGridAssemFactPage();			break;// [17]�������� �������ۿ�����ǥ
		case MAKE_BM_ASSEMBLY_FIELD :			DrawGridAssemFieldPage();			break;// [18]�������� ���弳ġ������ǥ
		case MAKE_BM_SECTION_COMP :				DrawGridSectionCompPage();			break;// [19]�ܸ麰 ����ǥ(�⺻�������)
		case MAKE_BM_BUJE_COMP :				DrawGridBujeCompPage();				break;// [20]���纰 ����ǥ(�⺻�������)
		case MAKE_BM_BASIC_COMP :				DrawGridBasicCompPage();			break;// [21]�⺻�� ����� ����ǥ
		//
		case MAKE_BM_SECTIONTYPE :				DrawGridSectionTypePage();			break;
		case MAKE_BM_BUJE :						DrawGridBujePage();					break;
		case MAKE_BM_BUJE_TOTAL :				DrawGridBujeTotalPage();			break;
		case MAKE_BM_GIRDER_TOTAL :				DrawGridGirderTotalPage();			break;
		case MAKE_BM_BUJEKIND_TOTAL :			DrawGridBujeKindTotalPage();			break;
			
		case MAKE_BM_DEDUCT_SANG :				DrawGridDeductSangPage();			break;
		case MAKE_BM_DEDUCT_BUJE :				DrawGridDeductBujePage();			break;
	}
	//���� ������ 
	double row = m_pGrid->GetRowCount();
	double col = m_pGrid->GetColumnCount();
	
	long nRow = (m_nMakeFlag==MAKE_BM_BASIC_COMP) ? 2 : 1;
	for (; nRow < row; nRow++) 
	{
		for (long nCol = 0; nCol < col; nCol++) 
		{
			if ( nRow % 2 == 0) {
				COLORREF	color = 0xfffaf0;
				m_pGrid->SetItemBkColour(nRow,nCol, color);
			}
			else {
				COLORREF color = 16777215;
				m_pGrid->SetItemBkColour(nRow,nCol, color);
			}
			if ((m_pGrid->GetTextMatrix(nRow, nCol) == "SUB TOTAL") || (m_pGrid->GetTextMatrix(nRow, nCol) == "���߷�")) 
			{
					COLORREF color = 8187132;
					m_pGrid->SetItemBkColour(nRow,nCol,   color);
					m_pGrid->SetItemBkColour(nRow,nCol+1, color);
			}
		}
	}
	m_pGrid->SetRedraw();
}


void CBMOutSangPage::DrawGridCarPage()			
{		
	CString cs;
	CBMCreator	    *pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPlateBridgeApp *pDB    = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CPtrList		*pList  = &pBMStd->m_BMList_Car;
	// set Rows
	if(pList->GetCount() <= 0) return;

	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);

	long nRoundTot  = pGOpt->GetPointValueOfGangJae(); // ���� �ݿø� �ڸ���
	long nRoundUnit = pGOpt->GetPointValueOfUnitGangJae();

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		BOOL bSubTotal = FALSE;

		if(pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice && pBMStd->IsCarBuje(pRec)) 
			bSubTotal = TRUE;
		if(pRecPrev && pBMStd->IsCarBuje(pRecPrev) && !pBMStd->IsCarBuje(pRec)) 
			bSubTotal = TRUE;
		if(pRecPrev && !pBMStd->IsCarBuje(pRecPrev) && pRecPrev->GetStrEleName() != pRec->GetStrEleName()) 
			bSubTotal = TRUE;
		if(pRecPrev && !pBMStd->IsCarBuje(pRecPrev)/* && pRecPrev->m_sMarkSection != pRec->m_sMarkSection*/) 
			bSubTotal = TRUE;

		if(bSubTotal)
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,9,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,10,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;			
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

//		if(pBMStd->IsCarBuje(pRec))
//		{
			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrGirder(m_pBMOutSheet->m_pDataManage->GetBridge()->GetNumberStartGirder()));// �Ŵ�
			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSplice(m_pBMOutSheet->m_pDataManage->GetBridge()->GetNumberStartSplice()));// ��������
//		}
		nColCount = 2;
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrEleName(),(UINT)DT_LEFT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);		// �����
		m_pGrid->SetTextMatrix(nRow,nColCount++,""/*pRec->GetStrMarkPiece()*/,(UINT)DT_LEFT);			// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);			// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);			// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);				// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);	// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,""/*pRec->m_sMarkGrandAssem*/,(UINT)DT_LEFT);			// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,""/*pRec->m_sMarkMidAssem*/,(UINT)DT_LEFT);			// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,""/*pRec->GetStrMarkAssem()*/,(UINT)DT_LEFT);			// ��������ũ

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+2);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,9,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,10,cs,(UINT)DT_RIGHT);
	nRow++;
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,9,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,10,cs,(UINT)DT_RIGHT);

	double dWidthAvg	= 0.00;
	for(long i=0; i<pDB->m_nQtyJigan+1;i++)
		dWidthAvg += pDB->GetWidthAct(pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (pDB->m_nQtyJigan+1);
	double dAreaTotal	= dWidthAvg*(pDB->GetGirder(-1)->GetStationEnd()-pDB->GetGirder(-1)->GetStationStt());
	double dUnitWeight	= TotalWeight / dFactorUnit / toM2(dAreaTotal);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight.Format("�������߷� : %.1f kgf/��",dUnitWeight);
}

void CBMOutSangPage::DrawGridDeductSangPage()
{
	DrawGridSangPage();
}

//�� ����ǥ
void CBMOutSangPage::DrawGridSangPage()
{	
	CString cs;
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();	
	
	if(m_nMakeFlag==MAKE_BM_SANG)			pList = &pBMStd->m_BMList_Sang;
	if(m_nMakeFlag==MAKE_BM_DEDUCT_SANG)	pList = &pBMStd->m_BMList_DeductSang;

	// set Rows
	if(pList->GetCount() <= 0) return;
	m_pBMOutSheet->m_CountText.ShowWindow(SW_HIDE);
	m_pBMOutSheet->m_WeightText.ShowWindow(SW_HIDE);
	m_pBMOutSheet->m_UnitWeightText.ShowWindow(SW_HIDE);
	m_pBMOutSheet->m_Progress.ShowWindow(SW_SHOW);
	m_pBMOutSheet->m_Progress.SetRange(0,pList->GetCount());
	m_pBMOutSheet->m_Progress.SetPos(0);

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight   = 0;

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if(pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice)
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,10,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,11,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrGirder(m_pBMOutSheet->m_pDataManage->GetBridge()->GetNumberStartGirder()));// �Ŵ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSplice(m_pBMOutSheet->m_pDataManage->GetBridge()->GetNumberStartSplice()));// ��������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrEleName(),(UINT)DT_LEFT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);	// �����
//		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkSection,(UINT)DT_LEFT);		// �ܸ鸶ũ
//		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);		// ���縶ũ
		nColCount++;
		nColCount++;
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon,nRoundTot),(UINT)DT_RIGHT);		// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);		// ����
//		cs.Format("%s", pRec->m_sMarkGrandAssem);
//		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);		// ��������ũ
//		cs.Format("%s", pRec->m_sMarkMidAssem);
//		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);		// ��������ũ
//		cs.Format("%s", pRec->m_sMarkAssem);
//		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);		// ��������ũ
		nColCount++;
		nColCount++;
		nColCount++;
		if(m_nMakeFlag==MAKE_BM_SANG)
		{
	//		cs.Format("%s", pRec->m_sMarkSectionBasic);
	//		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);	// �⺻�ܸ�
			nColCount++;
			cs.Format("%s", pRec->GetStrBujeKindName());
			m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);			// ��������
			cs.Format("%.4f",toM2(pRec->m_dAreaBuje));
			m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);				// ����
			cs.Format("%s", pRec->m_sRefDWG);
			m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);	// ��������
			cs.Format("%s", pRec->m_sUsage);
			m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);	// ���뵵
			cs.Format("%7.3f",pRec->m_dFactorArea);
			m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);	// �������
			cs.Format("%7.2f",pRec->m_dThickSecond);
			m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);	// �β�2
			cs.Format("%7.2f",pRec->m_dWidthSecond);
			m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_LEFT);	// �ʺ�2
			////////
			cs.Format("");
			long nRowExcel = nRow + 1;
			switch(pRec->m_nKindName)
			{
				case MATERIAL_KIND_PLATE:
				case MATERIAL_KIND_ELBOW:
				case MATERIAL_KIND_ELBOWEQUAL:
				case MATERIAL_KIND_SLEEVEFORPIPE:
				case MATERIAL_KIND_WIREMESH:
					{
						if(bUnitTon)
							cs.Format("=ROUND(G%d*H%d*I%d*J%d*K%d/1000000000*V%d/1000,%d)",nRowExcel,nRowExcel
								,nRowExcel,nRowExcel,nRowExcel,nRowExcel,nRoundTot);
						else
							cs.Format("=ROUND(G%d*H%d*I%d*J%d*K%d/1000000000*V%d,%d)",nRowExcel,nRowExcel
								,nRowExcel,nRowExcel,nRowExcel,nRowExcel,nRoundTot);
					}
					break;
				case MATERIAL_KIND_ROUNDBAR:
				case MATERIAL_KIND_PIPE:
				case MATERIAL_KIND_REBAR:
					{
						if(bUnitTon)
							cs.Format("=ROUND(I%d*J%d*K%d/1000/1000,%d)",nRowExcel,nRowExcel,nRowExcel,nRoundTot);
						else
							cs.Format("=ROUND(I%d*J%d*K%d/1000,%d)",nRowExcel,nRowExcel,nRowExcel,nRoundTot);
					}
					break;
				case MATERIAL_KIND_BOLT :
				case MATERIAL_KIND_BOLT2NUT :
				case MATERIAL_KIND_STUDWELD :
				case MATERIAL_KIND_STUDBOLT :
					{
						if(bUnitTon)
							cs.Format("=ROUND(J%d*K%d/1000,%d)",nRowExcel,nRowExcel,nRoundTot);
						else
							cs.Format("=ROUND(J%d*K%d,%d)",nRowExcel,nRowExcel,nRoundTot);
					}
					break;
				case MATERIAL_KIND_BEAM:
					{
						if(bUnitTon)
							cs.Format("=ROUND(I%d*J%d*K%d/1000000,%d)",nRowExcel,nRowExcel,nRowExcel,nRoundTot);
						else
							cs.Format("=ROUND(I%d*J%d*K%d/1000,%d)",nRowExcel,nRowExcel,nRowExcel,nRoundTot);
					}
					break;
			}
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// �߷����
			cs.Format("%ld",pRec->m_nMarkBuje);
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ���縶ũ
//			cs.Format("%ld",pRec->m_nMarkAssem);
//			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ��������ũ
//			cs.Format("%ld",pRec->m_nMarkMidAssem);
//			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ��������ũ
//			cs.Format("%ld",pRec->m_nMarkGrandAssem);
//			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ��������ũ
//			cs.Format("%ld",pRec->m_nMarkSectionBasic);
//			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// �⺻�ܸ鸶ũ
//			cs.Format("%ld",pRec->m_nMarkSection);
//			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// �ܸ鸶ũ
			nColCount++;
			nColCount++;
			nColCount++;
			nColCount++;
			nColCount++;

			cs.Format("%ld",pRec->m_nFirstClass);
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ���ǥ���� 1���з�
			cs.Format("%ld",pRec->m_nSectionNumber);
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ���� ��ȣ
			cs.Format("%.3f",pRec->m_dStationRec);
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// m_dStationRec
			cs.Format("%ld",pRec->m_dwFlag);
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// m_dwFlag
			cs.Format("%ld",pRec->m_dwDisableFlag);
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// m_dwDisableFlag
			cs.Format("%ld",pRec->m_nKindName);
			m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// m_nKindName
		}

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
		m_pBMOutSheet->m_Progress.StepIt();
	}	
	m_pBMOutSheet->m_Progress.ShowWindow(SW_HIDE);
	m_pBMOutSheet->m_CountText.ShowWindow(SW_SHOW);
	m_pBMOutSheet->m_WeightText.ShowWindow(SW_SHOW);
	m_pBMOutSheet->m_UnitWeightText.ShowWindow(SW_SHOW);
	nRow++;
	m_pGrid->SetRowCount(nRow+2);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,10,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,11,cs,(UINT)DT_RIGHT);
	nRow++;
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,10,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,11,cs,(UINT)DT_RIGHT);

	double dWidthAvg	= 0.00;
	for(long i=0; i<pDB->m_nQtyJigan+1;i++)
		dWidthAvg += pDB->GetWidthAct(pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (pDB->m_nQtyJigan+1);
	double dAreaTotal	= dWidthAvg*(pDB->GetGirder(-1)->GetStationEnd()-pDB->GetGirder(-1)->GetStationStt());
	double dUnitWeight	= TotalWeight / toM2(dAreaTotal) / dFactorUnit;

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight.Format("�������߷� : %.1f kgf/��",dUnitWeight);
}

//�ܸ�Ÿ�Ժ� ����ǥ
void CBMOutSangPage::DrawGridSectionTypePage()
{	
	CString cs;
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	pList = &pBMStd->m_BMList_SectionType;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight=0, SubWeight=0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		if( pRecPrev && ( pRecPrev->GetStrEleName() != pRec->GetStrEleName() 
 			             || ( pRecPrev->GetStrEleName()=="�Ϲ�" && 
						      pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName()) )  ) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

//		if(!pRecPrev || pRecPrev->GetStrEleName() != pRec->GetStrEleName())
			m_pGrid->SetTextMatrix(nRow,nColCount,pRec->GetStrEleName(),(UINT)DT_LEFT);// ����
		nColCount++;
//		if(!pRecPrev || pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName())
			m_pGrid->SetTextMatrix(nRow,nColCount,pRec->GetStrSubEleName(),(UINT)DT_LEFT);// �����
		nColCount++;

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	//sub total
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);
	//���߷�
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow, 6, "���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow, 7, cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
}

//�β��� ����ǥ
void CBMOutSangPage::DrawGridThickPage()
{
	CPlateBridgeApp *pDB    = m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption   *pGOpt  = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CBMCreator     *pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList        *pList  = &pBMStd->m_BMList_Thick;

	if(pList->GetCount() <= 0) return;

	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	// set Rows
	if(pList->GetCount() >= 0)
		m_pGrid->SetRowCount(pList->GetCount()+1);

	CBMRecord *pRecPrev= NULL;
	double TotalWeight = 0;
	BOOL   bUnitTon    = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	long n    = 0;
	long nn   = 0;
	CString str, str2;
	CString szTemp;

	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowHeight(nRow,18);
		// ���� *******************************************************
		str  = m_pGrid->GetTextMatrix(n, 0);
		str2 = pRec->GetStrMaterial();// ����
		if (str != str2)
		{
			m_pGrid->SetTextMatrix(nRow,nColCount++,str2,(UINT)DT_LEFT);// ����
			n = nRow;
		}
		else
			m_pGrid->SetTextMatrix(nRow,nColCount++,"",(UINT)DT_LEFT);// ����
		// �β� *******************************************************
		if(pRec->m_nKindName==MATERIAL_KIND_BEAM)
			szTemp.Format("%s-%sX%s", pRec->GetStrSteelBujeKindName(), pRec->GetStrWidth(), pRec->GetStrThick());
		else
			szTemp = pRec->GetStrThick();
		m_pGrid->SetTextMatrix(nRow, nColCount++, szTemp,(UINT)DT_RIGHT);	// �β�
		// ����, ���� *******************************************************
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);		// ����
		// �����߷�, ���߷� *******************************************************
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		// ��������	***************************************************************
		str  = m_pGrid->GetTextMatrix(nn, 6);
		str2 = pRec->GetStrBujeKindName();// ����
		if (str != str2)
		{
			m_pGrid->SetTextMatrix(nRow, nColCount++,str2,(UINT)DT_LEFT);
			nn = nRow;
		}
		else
			m_pGrid->SetTextMatrix(nRow, nColCount++,"",(UINT)DT_LEFT);
		// ************************************************************************
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	 
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	CString cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,4,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,5,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
}

//�β��� ����ǥ
void CBMOutSangPage::DrawGridThickSangPage()
{	
	/*
	CString cs;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList* pList = &pBMStd->m_BMList_ThickSang;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight   = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && (pRecPrev->GetStrProcessBig() != pRec->GetStrProcessBig() || 
			             pRecPrev->m_vJewon.y != pRec->m_vJewon.y)) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,7,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,8,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

//		if(pRec->GetStrProcessBig()=="MAIN P/N")
//			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);
//		else
//			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrProcessBig(),(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrProcessMid(),(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkBuje,(UINT)DT_LEFT);			// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);			// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);	// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);						// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,7,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,8,cs,(UINT)DT_RIGHT);

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,7,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,8,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridBujeTotalPage()  {	DrawGridBujePage();}
void CBMOutSangPage::DrawGridDeductBujePage() {	DrawGridBujePage();}
//���纰 ����ǥ
void CBMOutSangPage::DrawGridBujePage()
{	
	CString cs;
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();

	if(m_nMakeFlag==MAKE_BM_BUJE)			pList = &pBMStd->m_BMList_Buje;
	if(m_nMakeFlag==MAKE_BM_DEDUCT_BUJE)	pList = &pBMStd->m_BMList_DeductBuje;	
	if(m_nMakeFlag==MAKE_BM_BUJE_TOTAL)		pList = &pBMStd->m_BMList_BujeTotal;	
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	// set Rows
	if(pList->GetCount() >= 0)
		m_pGrid->SetRowCount(pList->GetCount()+1);

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight=0,SubWeight=0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	long nColSub = (m_nMakeFlag!=MAKE_BM_BUJE_TOTAL) ? 5 : 0;
	long nColWgt = (m_nMakeFlag!=MAKE_BM_BUJE_TOTAL) ? 6 : 1;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		if( pRecPrev && pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName()
			&& m_nMakeFlag!=MAKE_BM_BUJE_TOTAL) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,nColSub,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,nColWgt,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
		CString sxxx = pRec->GetStrSubEleName();
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);	// �����
		if(m_nMakeFlag!=MAKE_BM_BUJE_TOTAL)
		{
			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);	// �β�
			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);	// �ʺ�
			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);		// ����
			m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);// �����߷�
		}
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	//sub total
	if(m_nMakeFlag!=MAKE_BM_BUJE_TOTAL)
	{
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
		cs = pBase.GetStringWeight(SubWeight,nRoundTot);
		m_pGrid->SetTextMatrix(nRow,nColSub,"SUB TOTAL",(UINT)DT_RIGHT);
		m_pGrid->SetTextMatrix(nRow,nColWgt,cs,(UINT)DT_RIGHT);
	}
	//���߷�
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow, nColSub, "���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow, nColWgt, cs,(UINT)DT_RIGHT);

	double dWidthAvg	= 0.00;
	for(long i=0; i<pDB->m_nQtyJigan+1;i++)
		dWidthAvg += pDB->GetWidthAct(pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (pDB->m_nQtyJigan+1);
	double dAreaTotal	= dWidthAvg*(pDB->GetGirder(-1)->GetStationEnd()-pDB->GetGirder(-1)->GetStationStt());
	double dUnitWeight	= TotalWeight / toM2(dAreaTotal) / dFactorUnit;

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight.Format("�������߷� : %.1f kgf/��",dUnitWeight);
}



void CBMOutSangPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);

	CRect rect;
	m_pBMOutSheet->GetClientRect(&rect);
	SetWindowPos(NULL,0,0,rect.Width()-23,rect.Height()-60,SWP_NOMOVE);

	GetClientRect(&rect);	
	rect.left += 5;
	rect.right -= 5;
	rect.top += 5;	
	rect.bottom -= 5;	
	if(m_pGrid->GetSafeHwnd())
		m_pGrid->SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOMOVE);			
}
 


void CBMOutSangPage::SetGridCarTotalPage()
{
	long nRows = 0;
	long nCols = 4;
	char *Title[] = {"\n�Ŵ�",
					 "\n��������",
					 "\n��������ũ",
					 "\n���߷�\n(kgf)",
					};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[3] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth(2,120);
	m_pGrid->SetColumnWidth(3,100);

/*	if(!m_pBMOutSheet->m_pDataManage->GetBMStandard()->IsMakeMark(CMarkSerial::MARK_ASSEM))
	{
		m_pGrid->SetColumnWidth(1,120);
		m_pGrid->SetColumnWidth(2,0);
	}
*/
	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::DrawGridCarTotalPage()
{
	CString cs;
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	pList = &pBMStd->m_BMList_CarTotal;

	// set Rows
	if(pList->GetCount() >= 0)
		m_pGrid->SetRowCount(pList->GetCount()+1);

	if(pList->GetCount() == 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nColWeight = 3;
	long nColName   = m_pBMOutSheet->m_nARoadFlag==0 ? 1 : 2;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		long nCol = 0;
		nRow++;
		m_pGrid->SetRowHeight(nRow,18);
		
		CString str;
		if(pBMStd->IsCarBuje(pRec))
		{
			str.Format("G%d",pRec->m_nGirder+1+m_pBMOutSheet->m_pDataManage->GetBridge()->GetNumberStartGirder());
			m_pGrid->SetTextMatrix(nRow,nCol++,str,(UINT)DT_RIGHT);
			str.Format("S%d",pRec->m_nSplice+1+m_pBMOutSheet->m_pDataManage->GetBridge()->GetNumberStartSplice());
			m_pGrid->SetTextMatrix(nRow,nCol++,str,(UINT)DT_RIGHT);
		}
		nCol = 2;
		if(pBMStd->IsCarBuje(pRec) && m_pBMOutSheet->m_nARoadFlag)
			m_pGrid->SetTextMatrix(nRow,nColName,""/*pRec->m_sMarkGrandAssem*/,(UINT)DT_RIGHT);
		if(!pBMStd->IsCarBuje(pRec))
			m_pGrid->SetTextMatrix(nRow,nColName,pRec->GetStrEleName(),(UINT)DT_RIGHT);
		m_pGrid->SetTextMatrix(nRow,nColWeight,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	nRow++;
	CString str;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,nColName,"���߷�",(UINT)DT_RIGHT);
	if(m_pBMOutSheet->m_nARoadFlag==0)
		m_pGrid->SetTextMatrix(nRow,nColName+1,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,nColWeight, str,(UINT)DT_RIGHT);
	
	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
}


BOOL CBMOutSangPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class	
	if(m_sTotalRecord.GetLength() && m_pBMOutSheet->m_CountText.GetSafeHwnd())
	{
		m_pBMOutSheet->m_CountText.SetWindowText(m_sTotalRecord);
		m_pBMOutSheet->m_CountText.ShowWindow(SW_SHOW);
	}

	if(m_sTotalWeight.GetLength() && m_pBMOutSheet->m_WeightText.GetSafeHwnd())
	{
		m_pBMOutSheet->m_WeightText.SetWindowText(m_sTotalWeight);
		m_pBMOutSheet->m_WeightText.ShowWindow(SW_SHOW);
	}

	if(m_pBMOutSheet->m_UnitWeightText.GetSafeHwnd())
	{
		m_pBMOutSheet->m_UnitWeightText.SetWindowText(m_sUnitWeight);
		m_pBMOutSheet->m_UnitWeightText.ShowWindow(SW_SHOW);
	}

	return CPropertyPage::OnSetActive();
}

void CBMOutSangPage::SetGridAssemSangPage()
{
	long nRows = 0;
	long nCols = 7;
	char *Title[] = {"\n��������ũ",
					 "\n��������ũ",
					 "\n��������ũ",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n���",
				};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[5] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0,80);
	m_pGrid->SetColumnWidth(1,80);
	m_pGrid->SetColumnWidth(2,80);
	m_pGrid->SetColumnWidth(4,100);


	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridAssemDeductPage()	{	SetGridAssemFactPage();}
void CBMOutSangPage::SetGridAssemFieldPage()	{	SetGridAssemFactPage();}
void CBMOutSangPage::SetGridAssemTotalPage()	{	SetGridAssemFactPage();}
void CBMOutSangPage::SetGridAssemFactPage()
{
	long nRows = 0;
	long nCols = 5;
	char *Title[] = {"\n��������ũ",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n���",
				};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[3] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0,80);
	m_pGrid->SetColumnWidth(2,100);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridBujeMarkDeductPage() { SetGridBujeMarkSangPage(); }
void CBMOutSangPage::SetGridBujeMarkSangPage()
{
	long nRows = 0;
	long nCols = 12;

	char *Title[] = {"\n��������ũ",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n��������",
					 "\n����\n(��)",
					 "\n���",
				};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[7] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth(0,80);
	m_pGrid->SetColumnWidth(7,100);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridBujeMarkTotalPage()
{
	long nRows = 0;
	long nCols = 11;

	char *Title[] = {"\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n��������",
					 "\n����\n(��)",
					 "\n���",
				};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[6] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(6,100);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridGrandAssemSangPage()
{
	long nRows =  0;
	long nCols = 12;
	char *Title[] = {"\n��������ũ",
					 "\n��������ũ",
					 "\n��������ũ",
//					 "\n�ܸ鸶ũ",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[9] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 80);
	m_pGrid->SetColumnWidth(1, 80);
	m_pGrid->SetColumnWidth(2, 80);
	m_pGrid->SetColumnWidth(9,100);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridGrandAssemTotalPage()
{
	long nRows = 0;
	long nCols = 3;
	char *Title[] = {"\n��������ũ",
					 "\n���߷�\n(kgf)",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[1] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 80);
	m_pGrid->SetColumnWidth(1,100);

/*	if(!m_pBMOutSheet->m_pDataManage->GetBMStandard()->IsMakeMark(CMarkSerial::MARK_ASSEM))
		m_pGrid->SetColumnWidth(0,0);
	*/
	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridBasicCompPage()
{
	long nRows =  0;
	long nCols = 20;
	char *Title[] = {"\n\n�Ŵ�",
					 "\n\n����\n����",
					 "\n\n����",
					 "\n\n�����",
					 "\n\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n\n�߷���\n(����-�⺻)",
					 "\n\n����",
//					 "\n��������ũ",
//					 "\n��������ũ",
//					 "\n��������ũ",
					 "\n\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[10] = "\n���߷�\n(Tonf)";

	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(FALSE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	COLORREF color = 16777215;
//	if(nRow%2==0) color = 0xfffaf0;
	m_pGrid->SetTextBkColor(color);
	TRY 
	{
		if(m_pGrid->GetRowCount() <= 1)
			m_pGrid->SetRowCount(nRows+2);
		m_pGrid->SetColumnCount(nCols);
		m_pGrid->SetFixedRowCount(2);
		m_pGrid->SetFixedColumnCount(0);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
	}
    END_CATCH

	m_pGrid->SetRowHeight(0,25);
	m_pGrid->SetRowHeight(1,50);
	m_pGrid->SetColumnWidthAll(80);
	m_pGrid->SetColumnWidth( 0, 45);
	m_pGrid->SetColumnWidth( 1, 45);
	m_pGrid->SetColumnWidth( 2,120);
	m_pGrid->SetColumnWidth( 3,100);
	m_pGrid->SetColumnWidth( 9,100);
	m_pGrid->SetColumnWidth(10,100);
	m_pGrid->SetColumnWidth(15,100);
	m_pGrid->SetColumnWidth(16,100);
	m_pGrid->SetColumnWidth(17,100);
//	m_pGrid->SetColumnWidth(19,100);
//	m_pGrid->SetColumnWidth(20,100);
//	m_pGrid->SetColumnWidth(21,100);

	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	long i = 0;
	for(i = 0; i < nCols; i++) 
	{
		if(i<5 || i>16)
			Item.row = 0;
		else 
			Item.row = 1;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText = Title[i];
		m_pGrid->SetItem(&Item);
	}
	m_pGrid->SetTextMatrix(0,5,"�� �� ��");
	m_pGrid->SetTextMatrix(0,11,"�� �� ��");
	//
	for(i=0; i<5; i++)
		m_pGrid->SetMergeRow(0,1,i);
	m_pGrid->SetMergeRow(0, 1,17);
	m_pGrid->SetMergeRow(0, 1,18);
	m_pGrid->SetMergeRow(0, 1,19);
	m_pGrid->SetMergeCol(0, 5,10);
	m_pGrid->SetMergeCol(0,11,16);
}


void CBMOutSangPage::DrawGridAssemSangPage()
{
	/*
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	
	pList = &pBMStd->m_BMList_AssemblySang;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CString cs = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_sMarkAssem != pRec->m_sMarkAssem) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,4,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,5,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMarkAssem(),(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);	// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot)+" ",(UINT)DT_RIGHT);

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,4,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,5,cs,(UINT)DT_RIGHT);

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,4,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,5,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridAssemDeductPage()	{ DrawGridAssemFactPage(); }
void CBMOutSangPage::DrawGridAssemFieldPage()	{ DrawGridAssemFactPage(); }
void CBMOutSangPage::DrawGridAssemTotalPage()	{ DrawGridAssemFactPage(); }
void CBMOutSangPage::DrawGridAssemFactPage()
{
	/*
	if(!GetSafeHwnd()) return;

	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();

	if(m_nMakeFlag==MAKE_BM_ASSEMBLY_TOTAL)		pList = &pBMStd->m_BMList_AssemblyTotal;
	if(m_nMakeFlag==MAKE_BM_ASSEMBLY_FACT)		pList = &pBMStd->m_BMList_AssemblyFact;
	if(m_nMakeFlag==MAKE_BM_ASSEMBLY_FIELD)		pList = &pBMStd->m_BMList_AssemblyField;
	if(m_nMakeFlag==MAKE_BM_ASSEMBLY_DEDUCT)	pList = &pBMStd->m_BMList_AssemblyDeduct;

	if(pList->GetCount() <= 0) return;
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;

	double TotalWeight = 0, SubWeight = 0, Area = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0; CString str = _T("");
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		long nCol = 0;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkAssem());
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty()+" ",(UINT)DT_RIGHT);
		m_pGrid->SetTextMatrix(nRow,nCol++, pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot)+" ",(UINT)DT_RIGHT);

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,10,"SUB TOTAL",(UINT)DT_RIGHT);	m_pGrid->SetTextMatrix(nRow,2,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,3,str,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridBujeMarkDeductPage(){	DrawGridBujeMarkSangPage();}
void CBMOutSangPage::DrawGridBujeMarkSangPage()
{	
	/*
	CString cs;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList* pList;

	if(m_nMakeFlag==MAKE_BM_BUJEMARK_SANG)		pList = &pBMStd->m_BMList_BujeMarkSang;
	if(m_nMakeFlag==MAKE_BM_BUJEMARK_DEDUCT)	pList = &pBMStd->m_BMList_BujeMarkDeduct;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_sMarkGrandAssem != pRec->m_sMarkGrandAssem) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrBujeKindName(),(UINT)DT_LEFT);			// ��������
		cs.Format("%.4f",toM2(pRec->m_dAreaBuje));
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);				// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	//sub weight
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);
	//���߷�
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,6,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridBujeMarkTotalPage()
{
	/*
	CString cs;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList* pList = &pBMStd->m_BMList_BujeMarkTotal;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrBujeKindName(),(UINT)DT_LEFT);			// ��������
		cs.Format("%.4f",toM2(pRec->m_dAreaBuje));
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);				// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	nRow++;
	CString str;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,5,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,6,str,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridGrandAssemSangPage()
{
	/*
	CString cs;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList* pList = &pBMStd->m_BMList_GrandAssemblySang;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_sMarkGrandAssem != pRec->m_sMarkGrandAssem) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,8,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,9,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkAssem,(UINT)DT_LEFT);			// ��������ũ
//		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMarkSection(),(UINT)DT_LEFT);	// �ܸ鸶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);		// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,8,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,9,cs,(UINT)DT_RIGHT);

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,8,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,9,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridGrandAssemTotalPage()
{
	/*
	if(!GetSafeHwnd()) return;

	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	pList = &pBMStd->m_BMList_GrandAssemblyTotal;

	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	double  TotalWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0; CString str = _T("");
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		// ��������ũ
		long nCol = 0;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot)+" ",(UINT)DT_RIGHT);
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,0,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,1,str,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridBasicCompPage()
{
	/*
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	
	pList = &pBMStd->m_BMList_StandardSang;
	// set Rows
	if(pList->GetCount() <= 0) return;
	CString cs;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	POSITION pos = pList->GetHeadPosition();

	long nRow = 1;
	CString strEleName,strSubName; 
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		long nCol = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

//		if(pRec->m_dTWeight==0 && pRec->m_nGirder==-1 && pRec->m_nSplice==-1)	continue;
		if(pRec->m_dwSummaryFlag==BM_SUMMARY_ASSEM_BIG) SetColorSubTotal(nRow);

		strEleName = pRec->GetStrEleName();
		strSubName = pRec->GetStrSubEleName();
		if(strEleName=="<->") strEleName = _T("");
		if(strSubName=="<->") strSubName = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrGirder(pDB->GetNumberStartGirder()));	// �Ŵ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSplice(pDB->GetNumberStartSplice()));	// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,strEleName,(UINT)DT_LEFT);							// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,strSubName,(UINT)DT_LEFT);							// �����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);				// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThickBasic(),(UINT)DT_RIGHT);			// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidthBasic(),(UINT)DT_RIGHT);			// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLengthBasic(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty(),(UINT)DT_RIGHT);					// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);				// �����߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeightBasic(bUnitTon, nRoundTot),(UINT)DT_RIGHT);	// ���߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);					// �β�(����)
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);					// �ʺ�(����)
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);				// ����(����)
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty(),(UINT)DT_RIGHT);					// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);				// �����߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);			// ���߷�(����)
		double dDiffWeight = pRec->m_dTWeight*dFactorUnit - pRec->m_dTWeightBasic*dFactorUnit;
		if(pRec->m_dTWeight && pRec->m_dTWeightBasic)
			cs = pBase.GetStringWeight(dDiffWeight,nRoundTot);
		else
			cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);								// �߷���
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMaterial(),(UINT)DT_LEFT);				// ����
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);				// ��������ũ
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);				// ��������ũ
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkAssem(),(UINT)DT_LEFT);				// ��������ũ
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : ");
	m_sTotalWeight.Format("���߷� : ");
	m_sUnitWeight  = _T("");
	*/
}


void CBMOutSangPage::InitGridFirst(long nRows, long nCols)
{	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetHeaderSort(FALSE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	COLORREF color = 16777215;
//	if(nRow%2==0) color = 0xfffaf0;
	m_pGrid->SetTextBkColor(color);
	TRY 
	{
		if(m_pGrid->GetRowCount() <= 1)
			m_pGrid->SetRowCount(nRows+1);
		m_pGrid->SetColumnCount(nCols);
		m_pGrid->SetFixedRowCount(1);
		m_pGrid->SetFixedColumnCount(0);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
	}
    END_CATCH
}

void CBMOutSangPage::InitGridTitle(long nCols, char *Title[])
{
	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	for(long i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText = Title[i];
		m_pGrid->SetItem(&Item);
	}							
}

void CBMOutSangPage::SetGridSectionCompPage()
{
	long nRows = 0;
	long nCols = 12;
	char *Title[] = {"\n����",
					 "\n�����",
		             "\n�ʺ�\n(mm)",
					 "\n�β�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n���縶ũ",
					 "\n��������ũ",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[7] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 120);
	m_pGrid->SetColumnWidth(1, 100);
	m_pGrid->SetColumnWidth(2, 80);
	m_pGrid->SetColumnWidth(7,100);
	m_pGrid->SetColumnWidth(10,80);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridBujeCompPage()
{
	long nRows = 0;
	long nCols = 11;
	char *Title[] = {"\n�����",
		             "\n�ʺ�\n(mm)",
					 "\n�β�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n���縶ũ",
					 "\n��������ũ",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[6] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0,100);
	m_pGrid->SetColumnWidth(6,100);
	m_pGrid->SetColumnWidth(9,80);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridMidAssemSangPage()
{
	long nRows = 0;
	long nCols = 7;
	char *Title[] = {"\n��������ũ",
					 "\n��������ũ",
					 "\n��������ũ",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[5] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 80);
	m_pGrid->SetColumnWidth(1, 80);
	m_pGrid->SetColumnWidth(2, 80);
	m_pGrid->SetColumnWidth(5,100);

	InitGridTitle(nCols,Title);	
}

void CBMOutSangPage::SetGridMidAssemTotalPage()
{
	long nRows = 0;
	long nCols = 5;
	char *Title[] = {"\n��������ũ",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[3] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 80);
	m_pGrid->SetColumnWidth(3,100);

	InitGridTitle(nCols,Title);	
}

// [03]ǥ������ǥ(���ۿ�)
void CBMOutSangPage::SetGridStandardShopPage()
{
	long nRows = 0;
	long nCols = 13;
	char *Title[] = {"\n��������ũ",
					 "\n��������ũ",
					 "\n��������ũ",
					 "\n�ܸ鸶ũ",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n���"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[10] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(10,100);

	for(long i=0; i<5; i++)
		m_pGrid->SetColumnWidth(i,80);
	m_pGrid->SetColumnWidth(2,0);

/*	if(!m_pBMOutSheet->m_pDataManage->GetBMStandard()->IsMakeMark(CMarkSerial::MARK_SECTION))
		m_pGrid->SetColumnWidth(2,0);
	if(!m_pBMOutSheet->m_pDataManage->GetBMStandard()->IsMakeMark(CMarkSerial::MARK_ASSEM))
	{
		m_pGrid->SetColumnWidth(0,0);
		m_pGrid->SetColumnWidth(1,0);
		m_pGrid->SetColumnWidth(3,0);
	}
	if(!m_pBMOutSheet->m_pDataManage->GetBMStandard()->IsMakeMark(CMarkSerial::MARK_BUJE))
		m_pGrid->SetColumnWidth(4,0);
	*/
	InitGridTitle(nCols,Title);	
}

// [03]ǥ������ǥ(���ۿ�) - ����Ÿ��
void CBMOutSangPage::DrawGridStandardShopType2Page()
{
	/*
	CString cs;
	CBMCreator	*pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList	*pList  = &pBMStd->m_BMList_StandardShopType2;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight   = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	CString strEleName,strSubName; 
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if(pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice)
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,5,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,6,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nCol = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);				// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);					// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);					// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);				// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty(),(UINT)DT_RIGHT);					// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMaterial(),(UINT)DT_LEFT);				// ����

		pRecPrev = pRec;
	}	

	nRow++;
	m_pGrid->SetRowCount(nRow+2);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,5,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,6,cs,(UINT)DT_RIGHT);
	nRow++;
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,5,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,6,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

// [03]ǥ������ǥ(���ۿ�)
void CBMOutSangPage::DrawGridStandardShopPage()
{	
	/*
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	
	pList = &pBMStd->m_BMList_StandardShop;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	POSITION pos = pList->GetHeadPosition();
	CString cs = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		long nCol = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		if(pRec->m_dwSummaryFlag==BM_SUMMARY_ASSEM_BIG) SetColorSubTotal(nRow);

		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkAssem(),(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkSection(),(UINT)DT_LEFT);	// �ܸ鸶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);			// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);			// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);	// ���߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMaterial(),(UINT)DT_LEFT);		// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : ");
	m_sTotalWeight.Format("���߷� : "); 
	m_sUnitWeight  = _T("");
	*/
}

// [03]ǥ������ǥ(���ۿ�) - ����Ÿ��
void CBMOutSangPage::SetGridStandardShopType2Page()
{	
	long nRows = 0;
	long nCols = 9;
	
	char *Title[] = {"\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n���"};
		
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[6] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth(0,100);
 
	InitGridTitle(nCols,Title);	
}

// [02]ǥ������ǥ
void CBMOutSangPage::SetGridStandardSangPage()
{
	long nRows = 0;
	long nCols = 13;
	
	char *Title[] = {"\n�Ŵ�",
					 "\n����\n����",
					 "\n����",
					 "\n�����",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����߷�\n(kgf/��)",
					 "\n���߷�\n(kgf)",
					 "\n����",
					 "\n���"};
		
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[10] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);

	// ���縶ũ�� ������� �ʴ� ���
/*	if(!m_pBMOutSheet->m_pDataManage->GetBMStandard()->IsMakeMark(CMarkSerial::MARK_BUJE))
		m_pGrid->SetColumnWidth(4,0);
*/
	m_pGrid->SetColumnWidth( 0, 45);
	m_pGrid->SetColumnWidth( 1, 45);
	m_pGrid->SetColumnWidth( 2,120);
	m_pGrid->SetColumnWidth( 3,100);
 
	InitGridTitle(nCols,Title);	
}

// [02]ǥ������ǥ
void CBMOutSangPage::DrawGridStandardSangPage()
{
	CString cs;
	CBMCreator	*pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList	*pList  = &pBMStd->m_BMList_StandardSang;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();

	POSITION pos = pList->GetHeadPosition();

	long nRow = 0;
	CString strEleName,strSubName; 
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		long nCol = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

//		if(pRec->m_dTWeight==0 && pRec->m_nGirder==-1 && pRec->m_nSplice==-1)	continue;
//		if(pRec->m_dwSummaryFlag==BM_SUMMARY_ASSEM_BIG) SetColorSubTotal(nRow);

		strEleName = pRec->GetStrEleName();
		strSubName = pRec->GetStrSubEleName();
		if(strEleName=="<->") strEleName = _T("");
		if(strSubName=="<->") strSubName = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrGirder(pDB->GetNumberStartGirder()));	// �Ŵ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSplice(pDB->GetNumberStartSplice()));	// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,strEleName,(UINT)DT_LEFT);							// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,strSubName,(UINT)DT_LEFT);							// �����
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkSection,(UINT)DT_LEFT);					// �ܸ鸶ũ
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);				// ���縶ũ
		nCol++;
		nCol++;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);					// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);					// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);				// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty(),(UINT)DT_RIGHT);					// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);				// �����߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);			// ���߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMaterial(),(UINT)DT_LEFT);				// ����
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);				// ��������ũ
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);				// ��������ũ
//		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkAssem(),(UINT)DT_LEFT);				// ��������ũ
		nCol++;
		nCol++;
		nCol++;
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : ");
	m_sTotalWeight.Format("���߷� : ");
	m_sUnitWeight  = _T("");
}

void CBMOutSangPage::DrawGridSectionCompPage()
{
	/*
	CString cs;
	CBMCreator	*pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList	*pList  = &pBMStd->m_BMList_SectionComp;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName()) 
		{
			nRow++;			
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);			
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nCol = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrEleName(),(UINT)DT_LEFT);		// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);		// �����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);			// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);			// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);	// ���߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMaterial(),(UINT)DT_LEFT);		// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);		// ��������ũ

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+2);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);

	nRow++;
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,6,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);

	double dWidthAvg	= 0.00;
	for(long i=0; i<pDB->m_nQtyJigan+1;i++)
		dWidthAvg += pDB->GetWidthAct(pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (pDB->m_nQtyJigan+1);
	double dAreaTotal	= dWidthAvg*(pDB->GetGirder(-1)->GetStationEnd()-pDB->GetGirder(-1)->GetStationStt());
	double dUnitWeight	= TotalWeight / toM2(dAreaTotal) / dFactorUnit;

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight.Format("�������߷� : %.1f kgf/��",dUnitWeight);
	*/
}

void CBMOutSangPage::DrawGridBujeCompPage()
{
	/*
	CString cs;
	CBMCreator	*pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList	*pList  = &pBMStd->m_BMList_BujeComp;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName()) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nCol = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);		// �����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);			// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);			// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);		// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);	// ���߷�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMaterial(),(UINT)DT_LEFT);		// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrMarkPiece(),(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);		// ��������ũ

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+2);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,6,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);

	nRow++;
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,6,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,7,cs,(UINT)DT_RIGHT);

	double dWidthAvg	= 0.00;
	for(long i=0; i<pDB->m_nQtyJigan+1;i++)
		dWidthAvg += pDB->GetWidthAct(pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (pDB->m_nQtyJigan+1);
	double dAreaTotal	= dWidthAvg*(pDB->GetGirder(-1)->GetStationEnd()-pDB->GetGirder(-1)->GetStationStt());
	double dUnitWeight	= TotalWeight / toM2(dAreaTotal) / dFactorUnit;

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight.Format("�������߷� : %.1f kgf/��",dUnitWeight);
	*/
}

void CBMOutSangPage::DrawGridMidAssemSangPage()
{
	/*
	CString cs;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList* pList = &pBMStd->m_BMList_MidAssemblySang;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_sMarkMidAssem != pRec->m_sMarkMidAssem) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			SetColorSubTotal(nRow);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,4,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,5,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkAssem,(UINT)DT_LEFT);			// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,4,"SUB TOTAL",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,5,cs,(UINT)DT_RIGHT);

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,4,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,5,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}

void CBMOutSangPage::DrawGridMidAssemTotalPage()
{
	/*
	CString cs;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	CPtrList* pList = &pBMStd->m_BMList_MidAssemblyTotal;

	// set Rows
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrQty(),(UINT)DT_RIGHT);			// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrUWeight(nRoundUnit),(UINT)DT_RIGHT);		// �����߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}	

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow,0,"���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow,3,cs,(UINT)DT_RIGHT);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight = _T("");
	*/
}


void CBMOutSangPage::SetColorSubTotal(long nRow)
{
	COLORREF color = 8187132;
	long col = m_pGrid->GetColumnCount();

	for(long i=0; i<col; i++)
		m_pGrid->SetItemBkColour(nRow,i, color);
}


void CBMOutSangPage::DrawGridGirderTotalPage()
{	
	CString cs;
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();
	pList = &pBMStd->m_BMList_Girder;

	// set Rows
	if(pList->GetCount() >= 0)
		m_pGrid->SetRowCount(pList->GetCount()+1);

	if(pList->GetCount() == 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	long nRound = pGOpt->GetPointValueOfGangJae(); // ���� �ݿø� �ڸ���

	POSITION pos = pList->GetHeadPosition();
	double TotalWeight = 0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	CString str = _T("");
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		long nCol = 0;
		nRow++;
		m_pGrid->SetRowHeight(nRow,18);
		
		str.Format("G%d",pRec->m_nGirder+1+m_pBMOutSheet->m_pDataManage->GetBridge()->GetNumberStartGirder());
		m_pGrid->SetTextMatrix(nRow,nCol++,str,(UINT)DT_RIGHT);
		str = GetStringOutValue(pRec->m_dTWeight*dFactorUnit,nRound);
		m_pGrid->SetTextMatrix(nRow,nCol,str,(UINT)DT_RIGHT);

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
	}	
	//���߷�
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	str = GetStringOutValue(TotalWeight,nRound);
	m_pGrid->SetTextMatrix(nRow, 0, "���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow, 1, str,(UINT)DT_RIGHT);

	double dWidthAvg	= 0.00;
	for(long i=0; i<pDB->m_nQtyJigan+1;i++)
		dWidthAvg += pDB->GetWidthAct(pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (pDB->m_nQtyJigan+1);
	double dAreaTotal	= dWidthAvg*(pDB->GetGirder(-1)->GetStationEnd()-pDB->GetGirder(-1)->GetStationStt());
	double dUnitWeight	= TotalWeight / toM2(dAreaTotal) / dFactorUnit;

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight.Format("�������߷� : %.1f kgf/��",dUnitWeight);
}

void CBMOutSangPage::SetGridGirderTotalPage()
{
	long nRows = 0;
	long nCols = 2;
	char *Title[] = {"\n�Ŵ�","\n���߷�\n(kgf)"};

	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[1] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			

	m_pGrid->SetColumnWidth(0,80);
	m_pGrid->SetColumnWidth(1,100);

	InitGridTitle(nCols,Title);	
}


//MAKE_BM_BUJEKIND_TOTAL
void CBMOutSangPage::DrawGridBujeKindTotalPage()
{
	CString cs;
	CPtrList* pList;
	CBMCreator* pBMStd = m_pBMOutSheet->m_pDataManage->GetBMStandard();

	pList = &pBMStd->m_BMList_BujeKindTotal;
	if(pList->GetCount() <= 0) return;

	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)m_pBMOutSheet->m_pDataManage->GetBridge();
	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	CGangjeBase pBase(pDB);
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���

	// set Rows
	if(pList->GetCount() >= 0)
		m_pGrid->SetRowCount(pList->GetCount()+1);

	POSITION pos = pList->GetHeadPosition();
	CBMRecord* pRecPrev = NULL;
	double TotalWeight=0,SubWeight=0;
	BOOL bUnitTon = m_pBMOutSheet->m_pDataManage->GetGlobalOption()->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	long nRow = 0;
	const long nColSub = 1;
	const long nColWgt = 2;
	while( pos )
	{	
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		if( pRecPrev && pRecPrev->m_nSizeType != pRec->m_nSizeType) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			cs = pBase.GetStringWeight(SubWeight,nRoundTot);
			m_pGrid->SetTextMatrix(nRow,nColSub,"SUB TOTAL",(UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow,nColWgt,cs,(UINT)DT_RIGHT);
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		}
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_nSizeType==1 ? "��������" : pRec->m_nSizeType==2 ? "��������" : "��Ÿ����",(UINT)DT_CENTER);
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);	// �����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrTWeight(bUnitTon, nRoundTot),(UINT)DT_RIGHT);// ���߷�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrMaterial(),(UINT)DT_LEFT);// ����

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	//sub total
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
		cs = pBase.GetStringWeight(SubWeight,nRoundTot);
		m_pGrid->SetTextMatrix(nRow,nColSub,"SUB TOTAL",(UINT)DT_RIGHT);
		m_pGrid->SetTextMatrix(nRow,nColWgt,cs,(UINT)DT_RIGHT);

	//���߷�
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	SetColorSubTotal(nRow);
	cs = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_pGrid->SetTextMatrix(nRow, nColSub, "���߷�",(UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow, nColWgt, cs,(UINT)DT_RIGHT);

	double dWidthAvg	= 0.00;
	for(long i=0; i<pDB->m_nQtyJigan+1;i++)
		dWidthAvg += pDB->GetWidthAct(pDB->GetGirder(-1)->GetStationOnJijum(i));

	dWidthAvg /= (pDB->m_nQtyJigan+1);
	double dAreaTotal	= dWidthAvg*(pDB->GetGirder(-1)->GetStationEnd()-pDB->GetGirder(-1)->GetStationStt());
	double dUnitWeight	= TotalWeight / toM2(dAreaTotal) / dFactorUnit;

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
	m_sTotalWeight.Format("���߷� : %.3f tonf",(TotalWeight/1000.0)/dFactorUnit);
	m_sUnitWeight.Format("�������߷� : %.1f kgf/��",dUnitWeight);


}

void CBMOutSangPage::SetGridBujeKindTotalPage()
{
	long nRows = 0;
	long nCols = 4;
	char *Title[] = {"\n��������","\n�����","\n���߷�\n(kgf)","\n����"};

	CGlobarOption *pGOpt   = m_pBMOutSheet->m_pDataManage->GetGlobalOption();	
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	if(bUnitTon) Title[2] = "\n���߷�\n(Tonf)";

	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(20);
	m_pGrid->SetRowHeight(0,50);			

	m_pGrid->SetColumnWidth(0,80);
	m_pGrid->SetColumnWidth(1,150);
	m_pGrid->SetColumnWidth(2,100);
	m_pGrid->SetColumnWidth(3,100);

	InitGridTitle(nCols,Title);	


}
