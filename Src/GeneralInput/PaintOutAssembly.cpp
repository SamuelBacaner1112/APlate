// PaintOutAssembly.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "../PlateGangje\Gangje.h"
#include "PaintOutAssembly.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
/////////////////////////////////////////////////////////////////////////////
// CPaintOutAssembly property page

IMPLEMENT_DYNCREATE(CPaintOutAssembly, CPropertyPage)

CPaintOutAssembly::CPaintOutAssembly() : CPropertyPage(CPaintOutAssembly::IDD)
{
	//{{AFX_DATA_INIT(CPaintOutAssembly)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = new CGridCtrl;
}

CPaintOutAssembly::~CPaintOutAssembly()
{
	delete m_pGrid;
}

void CPaintOutAssembly::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaintOutAssembly)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaintOutAssembly, CPropertyPage)
	//{{AFX_MSG_MAP(CPaintOutAssembly)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintOutAssembly message handlers

int CPaintOutAssembly::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

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

void CPaintOutAssembly::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	m_pPaintOutSheet->GetClientRect(&rect);
	SetWindowPos(NULL,0,0,rect.Width()-23,rect.Height()-60,SWP_NOMOVE);

	GetClientRect(&rect);	
	rect.left += 5;
	rect.right -= 5;
	rect.top += 5;	
	rect.bottom -= 5;	
	if(m_pGrid->GetSafeHwnd())
		m_pGrid->SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOMOVE);			
}

UINT CPaintOutAssembly::DoWork()
{
	m_pGrid->SetRunningThread(TRUE);
	InitGrid();
	DrawGrid();	
	m_pGrid->SetRunningThread(FALSE);
	return 0;
}

BOOL CPaintOutAssembly::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_pPaintOutSheet->m_nARoadFlag=0;//�ӽ÷�..
	
//	InitGrid();
//	DrawGrid();

	Begin();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPaintOutAssembly::InitGrid()
{
	switch(m_nMakeFlag)
	{
		case PAINTMAKE_SANG:					InitGridSang();					break;
		case PAINTMAKE_BUJE_SANG:				InitGridBujeSang();				break;
		case PAINTMAKE_BUJE_TOTAL:				InitGridBujeTotal();			break;
		case PAINTMAKE_ASSEMBLY:				InitGridAssembly();				break;
		case PAINTMAKE_ASSEMBLY_TOTAL:			InitGridAssemblyTotal();		break;
		case PAINTMAKE_GRAND_ASSEMBLY:			InitGridGrandAssembly();		break;
		case PAINTMAKE_GRAND_ASSEMBLY_TOTAL:	InitGridGrandAssemblyTotal();	break;
		case PAINTMAKE_MAT_THICK:				InitGridMatAndThick();			break;
		case PAINTMAKE_BOLT_SANG:				InitGridBoltSang();				break;
//		case PAINTMAKE_BOLT_TOTAL:				InitGridBoltTotal();			break;
		case PAINTMAKE_SUMMARY:					InitGridSummary();				break;
	}
}

void CPaintOutAssembly::DrawGrid()
{

	switch(m_nMakeFlag)
	{
		case PAINTMAKE_SANG:					DrawGridSang();					break;
		case PAINTMAKE_BUJE_SANG:				DrawGridBujeSang();				break;
		case PAINTMAKE_BUJE_TOTAL:				DrawGridBujeTotal();			break;
		case PAINTMAKE_ASSEMBLY:				DrawGridAssembly();				break;
		case PAINTMAKE_ASSEMBLY_TOTAL:			DrawGridAssemblyTotal();		break;
		case PAINTMAKE_GRAND_ASSEMBLY:			DrawGridGrandAssembly();		break;
		case PAINTMAKE_GRAND_ASSEMBLY_TOTAL:	DrawGridGrandAssemblyTotal();	break;
		case PAINTMAKE_MAT_THICK:				DrawGridMatAndThick();			break;
		case PAINTMAKE_BOLT_SANG:				DrawGridBoltSang();				break;
//		case PAINTMAKE_BOLT_TOTAL:				DrawGridBoltTotal();			break;
		case PAINTMAKE_SUMMARY:					DrawGridSummary();				break;
	}
	//���� ������ 
	double row = m_pGrid->GetRowCount();
	double col = m_pGrid->GetColumnCount();
	
	for (long nRow = 1; nRow < row; nRow++) 
	{
		for (long nCol = 0; nCol < col; nCol++) 
		{
			if ( nRow % 2 == 0) 
			{
				COLORREF	color = 0xfffaf0;
				m_pGrid->SetItemBkColour(nRow,nCol, color);
			}
			else 
			{
				COLORREF color = 16777215;
				m_pGrid->SetItemBkColour(nRow,nCol, color);
			}
		}
	}
}

void CPaintOutAssembly::InitGridAssemblyTotal()
{
	long nRows = 0;
	long nCols = 8;
	char *Title[] = {"\n��������ũ", 
					 "\n�����ܺ�\n(����)",
					 "\n�����ܺ�\n(����)",
					 "\n��������\n(����)",
					 "\n�� �� ��\n(����)",
					 "\n�ܺ� ��Ʈ ��\n������(����)",
					 "\n���� ��Ʈ ��\n������(����)",
					 "\n�ܺ������\n(����)"};

	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(90);
	m_pGrid->SetColumnWidth(0, 80);

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

void CPaintOutAssembly::InitGridAssembly()
{
	long nRows =  0;
	long nCols = 13;
	char *Title[] = {"\n��������ũ", 
		             "\n���縶ũ", 
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",	// ������� ��������ǥ�� ����
					 "\n�����ܺ�\n(����)",
					 "\n�����ܺ�\n(����)",
					 "\n��������\n(����)",
					 "\n�� �� ��\n(����)",
					 "\n�ܺ� ��Ʈ ��\n������(����)",
					 "\n���� ��Ʈ ��\n������(����)",
					 "\n�ܺ������\n(����)"};

	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 80);
	long i = 0;
	for(i=6; i<nCols; i++)
		m_pGrid->SetColumnWidth(i,90);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	for(i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText = Title[i];
		m_pGrid->SetItem(&Item);
	}
}

void CPaintOutAssembly::InitGridGrandAssembly()
{
	long nRows =  0;
	long nCols = 14;
	char *Title[] = {"\n��������ũ", 
					 "\n��������ũ", 
		             "\n���縶ũ", 
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",	// ������� ��������ǥ�� ����
					 "\n�����ܺ�\n(����)",
					 "\n�����ܺ�\n(����)",
					 "\n��������\n(����)",
					 "\n�� �� ��\n(����)",
					 "\n�ܺ� ��Ʈ ��\n������(����)",
					 "\n���� ��Ʈ ��\n������(����)",
					 "\n�ܺ������\n(����)"};

	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 80);
	m_pGrid->SetColumnWidth(1, 80);
	long i = 0;
	for(i=7; i<nCols; i++)
		m_pGrid->SetColumnWidth(i,90);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	for(i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText = Title[i];
		m_pGrid->SetItem(&Item);
	}
}

void CPaintOutAssembly::InitGridGrandAssemblyTotal()
{
	long nRows = 0;
	long nCols = 8;
	char *Title[] = {"\n��������ũ", 
					 "\n�����ܺ�\n(����)",
					 "\n�����ܺ�\n(����)",
					 "\n��������\n(����)",
					 "\n�� �� ��\n(����)",
					 "\n�ܺ� ��Ʈ ��\n������(����)",
					 "\n���� ��Ʈ ��\n������(����)",
					 "\n�ܺ������\n(����)"};

	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(90);
	m_pGrid->SetColumnWidth(0, 80);

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

void CPaintOutAssembly::InitGridMatAndThick()
{
	long nRows =  0;
	long nCols = 14;
	char *Title[] = {"\n��з�",
					 "\n�ߺз�",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",
					 "\n�����ܺ�\n(����)",
					 "\n�����ܺ�\n(����)",
					 "\n��������\n(����)",
					 "\n�� �� ��\n(����)",
					 "\n�ܺ� ��Ʈ ��\n������(����)",
					 "\n���� ��Ʈ ��\n������(����)",
					 "\n�ܺ������\n(����)"};

	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0,120);
	m_pGrid->SetColumnWidth(1,120);
	long i = 0;
	for(i=7; i<nCols; i++)
		m_pGrid->SetColumnWidth(i,90);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	for(i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText = Title[i];
		m_pGrid->SetItem(&Item);
	}
}

void CPaintOutAssembly::InitGridSummary()
{
	long nRows = 0;
	long nCols = 2;
	char *CTitle[] = {"\n�� ��", "\n����\n(��)"};

 	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(80);
	m_pGrid->SetColumnWidth(0,150);


	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	for(long i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText = CTitle[i];
		m_pGrid->SetItem(&Item);
	}
}

void CPaintOutAssembly::InitGridSang()
{
	long nRows =0;
	char *Title[] = {"\n�Ŵ�",
					 "\n����\n����",
					 "\n����",
					 "\n�����",
					 "\n�⺻�ܸ�",
					 "\n�ܸ鸶ũ",
					 "\n��������ũ",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",	// ������� ��������ǥ�� ����
					 "\n�����ܺ�\n(����)",
					 "\n�����ܺ�\n(����)",
					 //"\n��������\n(����)",
					 "\n�� �� ��\n(����)",
					 "\n�ܺ� ��Ʈ ��\n������(����)",
					 //"\n���� ��Ʈ ��\n������(����)",
					 "\n�ܺ������\n(����)",
					 "\n��������",
					 "\n���뵵",
					 "\n���������ũ",	//  ���ϴ� HIDDEN COLUMN
					 "\n�߼�������ũ",
					 "\n�����ȣ",		// ���縶ũ m_nMarkBuje
					 "\n������ȣ",		// ��������ũ m_nMarkAssem
					 "\n��������ȣ",
					 "\n��������ȣ",
					 "\n�⺻�ܸ��ȣ",
					 "\n�ܸ��ȣ",		// �ܸ鸶ũ m_nMarkSection
					 "\n������ȣ",		// ������ȣ m_nSectionNumber
				};
 	long nCols = 28;
	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth( 0, 50);
	m_pGrid->SetColumnWidth( 1, 50);
	m_pGrid->SetColumnWidth( 2,120);
	m_pGrid->SetColumnWidth( 3,100);
	m_pGrid->SetColumnWidth( 6, 80);
/*
	if(!m_pPaintOutSheet->m_pDataManage->m_pBMStandard->IsMakeMark(CMarkSerial:MARK_SECTION))
		m_pGrid->SetColumnWidth(5,0);
	if(!m_pPaintOutSheet->m_pDataManage->m_pBMStandard->IsMakeMark(CMarkSerial::MARK_ASSEM))
		m_pGrid->SetColumnWidth(6,0);
	if(!m_pPaintOutSheet->m_pDataManage->m_pBMStandard->IsMakeMark(CMarkSerial::MARK_BUJE))
		m_pGrid->SetColumnWidth(7,0);
*/
	long i = 0;
	for(i=12; i<19; i++)
		m_pGrid->SetColumnWidth(i,90);
	
	if(m_pPaintOutSheet->m_nARoadFlag==0)
	{
		m_pGrid->SetColumnWidth(4,0);
		m_pGrid->SetColumnWidth(5,0);
	}

	for(i=19; i<nCols; i++)
		m_pGrid->SetColumnWidth(i,0);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	for(i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText  = Title[i];
		m_pGrid->SetItem(&Item);
	}
}

void CPaintOutAssembly::InitGridBoltSang()
{
	long nRows = 0;
	long nCols = 8;
	char *Title[] = {"\n��������ũ", 
					 "\n���縶ũ",
					 "\n����",
					 "\n�β�\n(mm)",
					 "\n����\n(mm)",
					 "\n�����߷�\n(kgf/mm��)",
					 "\n����\n(��)",
					 "\n���߷�\n(kgf)",
				};
 	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0,80);
	m_pGrid->SetColumnWidth(7,100);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;

	for(long i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText  = Title[i];
		m_pGrid->SetItem(&Item);
	}
}

void CPaintOutAssembly::InitGridBoltTotal()
{
	long nRows = 0;
	long nCols = 6;
	char *Title[] = {"\n����",
					 "\n�β�\n(mm)",
					 "\n����\n(mm)",
					 "\n�����߷�\n(kgf/mm��)",//(kgf/��)
					 "\n����\n(��)",
					 "\n���߷�\n(kgf)",
				};
 	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(5,100);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;

	for(long i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText  = Title[i];
		m_pGrid->SetItem(&Item);
	}

}

void CPaintOutAssembly::InitGridBujeTotal()
{
}

void CPaintOutAssembly::InitGridBujeSang()
{
	long nRows =  0;
	long nCols = 10;
	char *Title[] = {"\n�����",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",	// ������� ��������ǥ�� ����
					 "\n�����ܺ�\n(����)",
					 "\n�����ܺ�\n(����)",
//					 "\n��������\n(����)",
					 "\n�� �� ��\n(����)",
					 "\n�ܺ� ��Ʈ ��\n������(����)",
//					 "\n���� ��Ʈ ��\n������(����)",
					 "\n�ܺ������\n(����)",
				};
 	DWORD dwTextStyle = DT_CENTER|DT_VCENTER;//|DT_SINGLELINE;
	CString vDigit;	
	
	m_pGrid->SetEditable(TRUE);
	m_pGrid->SetListMode(TRUE);
	m_pGrid->EnableDragAndDrop(TRUE);
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->EnableInsertRow(FALSE);	
	m_pGrid->EnableInsertCol(FALSE);	
	m_pGrid->SetTextBkColor(RGBEDITABLE);
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
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth( 0,100);
	m_pGrid->SetColumnWidth( 3, 80);
	long i = 0;
	for(i=3; i<10; i++)
		m_pGrid->SetColumnWidth(i,90);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	for(i = 0; i < nCols; i++) 
	{
		Item.row = 0;
		Item.col = i;	
		Item.nFormat = dwTextStyle;
		Item.szText  = Title[i];
		m_pGrid->SetItem(&Item);
	}
}

void CPaintOutAssembly::DrawGridAssemblyTotal()
{
	if(!GetSafeHwnd()) return;

	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();	
	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList		*pList	= &pPaStd->m_ListAssemblyTotal;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow	= 0; 
	long nCol	= 0;
	long nItem	= PAINT_ITEM_MAX;
	CString cs	= _T("");
	CString str = _T("");

	CPaintRecord *pRecPre = NULL;

	long nRound = pGOpt->GetPointValueOfUnitGangJae(); // �����߷� ���� �ݿø� �ڸ���
	double AreaTotal[PAINT_ITEM_MAX];
	long  i = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
			
		nCol = 0;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkAssem,(UINT)DT_LEFT);		// ��������ũ
		//	
		long i = 0;
		for(i=0; i<nItem; i++)										
		{																		
			if(pRec->m_dPaintArea[i]!=0)
				cs = GetStringOutValue(toM2(pRec->m_dPaintArea[i]),nRound);
			else
				cs = _T("");
			m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);					
		}
		
		for(i=0; i<nItem; i++)	AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPre = pRec;
	}	

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 0;
	m_pGrid->SetTextMatrix(nRow,nCol++, "�Ѹ���",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		
	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridAssembly()
{
	if(!GetSafeHwnd()) return;

	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();
	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList		*pList	= &pPaStd->m_ListAssembly;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow	= 0; 
	long nCol	= 0;
	long nItem	= PAINT_ITEM_MAX;
	CString cs	= _T("");
	CString str = _T("");

	CPaintRecord *pRecPre = NULL;

	long nRound = pGOpt->GetPointValueOfUnitGangJae(); // �����߷� ���� �ݿø� �ڸ���
	double AreaSub[PAINT_ITEM_MAX],AreaTotal[PAINT_ITEM_MAX];
	long i = 0;
	for(i=0; i<nItem; i++)	AreaSub[i]   = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		//sub total
		if( pRecPre && pRecPre->m_sMarkAssem != pRec->m_sMarkAssem) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			nCol = 5;
			m_pGrid->SetTextMatrix(nRow,nCol++,"SUB TOTAL",(UINT)DT_RIGHT);
			long i = 0;
			for(i=0; i<nItem; i++)
			{			
				cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
				if(AreaSub[i]==0) cs = _T("");
				m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
			}		
			for(i=0; i<nItem; i++)	
				AreaSub[i] = 0;
		}
		for(i=0; i<nItem; i++)	AreaSub[i] += pRec->m_dPaintArea[i];

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
			
		// ��������ũ
		nCol = 0;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkAssem,(UINT)DT_LEFT);		// ��������ũ
		if(m_nMakeFlag==PAINTMAKE_ASSEMBLY)
		{
			m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkBuje,(UINT)DT_LEFT);	// ���縶ũ
			m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
			m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
			m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
			m_pGrid->SetTextMatrix(nRow,nCol++,atofRound(pRec->m_nQty,0),(UINT)DT_RIGHT);		// ����
		}
		//		
		for(i=0; i<nItem; i++)										
		{																		
			if(pRec->m_dPaintArea[i]!=0)
				cs = GetStringOutValue(toM2(pRec->m_dPaintArea[i]),nRound);
			else
				cs = _T("");
			m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);					
		}
		
		for(i=0; i<nItem; i++)	
			AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPre = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 5;
	m_pGrid->SetTextMatrix(nRow,nCol++, "SUB TOTAL",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
		if(AreaSub[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 5;
	m_pGrid->SetTextMatrix(nRow,nCol++, "�Ѹ���",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		
	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridGrandAssemblyTotal()
{
	if(!GetSafeHwnd()) return;

	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();
	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList *pList = &pPaStd->m_ListGrandAssembly;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow = 0; CString str = _T("");
	long nCol = 0;
	long nItem = PAINT_ITEM_MAX;
	CString cs = _T("");

	long nRound = pGOpt->GetPointValueOfUnitGangJae(); // �����߷� ���� �ݿø� �ڸ���
	double AreaTotal[PAINT_ITEM_MAX];
	long i = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
			
		nCol = 0;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);	// ��������ũ
		//	
		long i = 0;
		for(i=0; i<nItem; i++)										
		{																		
			if(pRec->m_dPaintArea[i]!=0)
				cs = GetStringOutValue(toM2(pRec->m_dPaintArea[i]),nRound);
			else
				cs = _T("");
			m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);					
		}
		
		for(i=0; i<nItem; i++)	
			AreaTotal[i] += pRec->m_dPaintArea[i];
	}	

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 0;
	m_pGrid->SetTextMatrix(nRow,nCol++, "�Ѹ���",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		
	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridGrandAssembly()
{
	if(!GetSafeHwnd()) return;

	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();
	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList		*pList	= &pPaStd->m_ListGrandAssembly;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow  = 0; CString str = _T("");
	long nCol  = 0;
	long nItem = PAINT_ITEM_MAX;
	CString cs = _T("");

	CPaintRecord *pRecPre = NULL;
	long nRound = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	double AreaSub[PAINT_ITEM_MAX],AreaTotal[PAINT_ITEM_MAX];
	long i = 0;
	for(i=0; i<nItem; i++)	AreaSub[i]   = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		//sub total
		if( pRecPre && pRecPre->m_sMarkGrandAssem != pRec->m_sMarkGrandAssem) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			nCol = 6;
			m_pGrid->SetTextMatrix(nRow,nCol++,"SUB TOTAL",(UINT)DT_RIGHT);
			long i = 0;
			for(i=0; i<nItem; i++)
			{			
				cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
				if(AreaSub[i]==0) cs = _T("");
				m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
			}		
			for(i=0; i<nItem; i++)	AreaSub[i] = 0;
		}
		for(i=0; i<nItem; i++)	AreaSub[i] += pRec->m_dPaintArea[i];

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
			
		nCol = 0;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkAssem,(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkBuje,(UINT)DT_LEFT);	// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);	// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);	// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,atofRound(pRec->m_nQty,0),(UINT)DT_RIGHT);	// ����
		//		
		for(i=0; i<nItem; i++)										
		{																		
			if(pRec->m_dPaintArea[i]!=0)
				cs = GetStringOutValue(toM2(pRec->m_dPaintArea[i]),nRound);
			else
				cs = _T("");
			m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);					
		}
		
		for(i=0; i<nItem; i++)	AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPre = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 6;
	m_pGrid->SetTextMatrix(nRow,nCol++, "SUB TOTAL",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
		if(AreaSub[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 6;
	m_pGrid->SetTextMatrix(nRow,nCol++, "�Ѹ���",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		
	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridMatAndThick()
{
	if(!GetSafeHwnd()) return;

	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();
	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList		*pList	= &pPaStd->m_ListMatAndThick;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow	= 0; CString str = _T("");
	long nCol	= 0;
	long nItem	= PAINT_ITEM_MAX;
	CString cs	= _T("");

	CPaintRecord *pRecPre = NULL;
	long nRound = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	double AreaSub[PAINT_ITEM_MAX],AreaTotal[PAINT_ITEM_MAX];
	long i = 0;
	for(i=0; i<nItem; i++)	AreaSub[i]   = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		//sub total
		if( pRecPre && (pRecPre->GetStrProcessBig() != pRec->GetStrProcessBig() || 
			            pRecPre->m_vJewon.y != pRec->m_vJewon.y)) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			nCol = 6;
			m_pGrid->SetTextMatrix(nRow,nCol++,"SUB TOTAL",(UINT)DT_RIGHT);
			long i = 0;
			for(i=0; i<nItem; i++)
			{			
				cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
				if(AreaSub[i]==0) cs = _T("");
				m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
			}		
			for(i=0; i<nItem; i++)	AreaSub[i] = 0;
		}
		for(i=0; i<nItem; i++)	AreaSub[i] += pRec->m_dPaintArea[i];

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
			
		// ��������ũ
		nCol = 0;
		if(pRec->GetStrProcessBig()=="MAIN P/N")
			m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);
		else
			m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrProcessBig(),(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrProcessMid(),(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkBuje,(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,atofRound(pRec->m_nQty,0),(UINT)DT_RIGHT);		// ����
		//		
		for(i=0; i<nItem; i++)										
		{																		
			if(pRec->m_dPaintArea[i]!=0)
				cs = GetStringOutValue(toM2(pRec->m_dPaintArea[i]),nRound);
			else
				cs = _T("");
			m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);					
		}
		
		for(i=0; i<nItem; i++)	AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPre = pRec;
	}	
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 6;
	m_pGrid->SetTextMatrix(nRow,nCol++, "SUB TOTAL",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
		if(AreaSub[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 6;
	m_pGrid->SetTextMatrix(nRow,nCol++, "�Ѹ���",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		
	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridSummary()
{
	if(!GetSafeHwnd()) return;

	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList		*pList	= &pPaStd->m_ListSang;

	CPaintRecord pRec;

	if(pList->GetCount() == 0) return;

	double dSumPaintArea[PAINT_ITEM_MAX];
	for(long i=0; i<PAINT_ITEM_MAX; i++) dSumPaintArea[i]=0;

	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		for(long i=0; i<PAINT_ITEM_MAX; i++)
			dSumPaintArea[i] += pRec->m_dPaintArea[i];
	}
	long nRow = 1;
	long col = 1;
	CString str = _T("");
	m_pGrid->SetRowCount(6);

	m_pGrid->SetTextMatrix(1,0,"���� �ܺ�(����)");
	m_pGrid->SetTextMatrix(2,0,"���� �ܺ�(����)");
//	m_pGrid->SetTextMatrix(3,0,"���� ����(����)");
	m_pGrid->SetTextMatrix(3,0,"�� �� �� (����)");
	m_pGrid->SetTextMatrix(4,0,"�ܺ� ��Ʈ(����)");
//	m_pGrid->SetTextMatrix(6,0,"���� ��Ʈ(����)");
	m_pGrid->SetTextMatrix(5,0,"�ܺ������(����)");

	str.Format("%10.3f",toM2(dSumPaintArea[OUTSIDE_FACT]));
	m_pGrid->SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[OUTSIDE_FIELD]));
	m_pGrid->SetTextMatrix(nRow++,col,str);
//	str.Format("%10.3f",toM2(dSumPaintArea[INSIDE_FACT]));
//	m_pGrid->SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[SPLICE_FACT]));
	m_pGrid->SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[BOLT_FIELD_OUTSIDE]));
	m_pGrid->SetTextMatrix(nRow++,col,str);
//	str.Format("%10.3f",toM2(dSumPaintArea[BOLT_FIELD_INSIDE]));
//	m_pGrid->SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[PAVE_FACT]));
	m_pGrid->SetTextMatrix(nRow++,col,str);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : ");
}

void CPaintOutAssembly::DrawGridSang()
{
	if(!GetSafeHwnd()) return;

	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();
	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList		*pList	= &pPaStd->m_ListSang;

	if(pList->GetCount() <= 0) return;
	m_pPaintOutSheet->m_CountText.ShowWindow(SW_HIDE);
	m_pPaintOutSheet->m_Progress.ShowWindow(SW_SHOW);
	m_pPaintOutSheet->m_Progress.SetRange(0,pList->GetCount());
	m_pPaintOutSheet->m_Progress.SetPos(0);

	POSITION pos = pList->GetHeadPosition();

	CString cs = _T("");
	long nRow   = 0; 
	long nCol	= 0;
	long nItem  = PAINT_ITEM_MAX;
	long nGStt  = m_pPaintOutSheet->m_pDataManage->GetBridge()->GetNumberStartGirder();
	long nSpStt = m_pPaintOutSheet->m_pDataManage->GetBridge()->GetNumberStartSplice();

	CPaintRecord *pRecPre = NULL;
	double AreaSub[PAINT_ITEM_MAX],AreaTotal[PAINT_ITEM_MAX];
	long nRound = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���

	long i = 0;
	for(i=0; i<nItem; i++)	AreaSub[i]   = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		//sub total
		if( pRecPre && pRecPre->m_nSplice != pRec->m_nSplice) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			nCol = 11;
			m_pGrid->SetTextMatrix(nRow,nCol++,"SUB TOTAL",(UINT)DT_RIGHT);
			for(i=0; i<nItem; i++)
			{			
				cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
				if(AreaSub[i]==0) cs = _T("");
				m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
			}		
			for(i=0; i<nItem; i++)	AreaSub[i] = 0;
		}
		for(i=0; i<nItem; i++)	AreaSub[i] += pRec->m_dPaintArea[i];

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);
			
		nCol = 0;
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrGirder(nGStt));				// �Ŵ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSplice(nSpStt));				// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrEleName(),(UINT)DT_LEFT);	// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);	// �����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkSectionBasic,(UINT)DT_LEFT);// �⺻�ܸ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkSection,(UINT)DT_LEFT);		// �ܸ鸶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkBuje,(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,atofRound(pRec->m_nQty,0),(UINT)DT_RIGHT);		// ����
		//		
		for(i=0; i<nItem; i++)										
		{								 										
			if(pRec->m_dPaintArea[i]!=0)
				cs = GetStringOutValue(toM2(pRec->m_dPaintArea[i]),nRound);
			else
				cs = _T("");
			m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);					
		}
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sRefDWG,(UINT)DT_LEFT);	// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sUsage,(UINT)DT_LEFT);	// ���뵵
		//////// HIDDEN  //////////////
		// ��������ũ
		m_pGrid->SetTextMatrix(nRow, nCol++, pRec->m_sMarkGrandAssem, (UINT)DT_RIGHT);
		// ��������ũ
		m_pGrid->SetTextMatrix(nRow, nCol++, pRec->m_sMarkMidAssem, (UINT)DT_RIGHT);
		//
		cs.Format("%ld",pRec->m_nMarkBuje);
		m_pGrid->SetTextMatrix(nRow, nCol++, cs, (UINT)DT_RIGHT);		// �����ȣ
		cs.Format("%ld",pRec->m_nMarkAssem);
		m_pGrid->SetTextMatrix(nRow, nCol++, cs, (UINT)DT_RIGHT);		// ������ȣ
		cs.Format("%ld",pRec->m_nMarkMidAssem);
		m_pGrid->SetTextMatrix(nRow, nCol++, cs, (UINT)DT_RIGHT);		// ��������ȣ
		cs.Format("%ld",pRec->m_nMarkGrandAssem);
		m_pGrid->SetTextMatrix(nRow, nCol++, cs, (UINT)DT_RIGHT);		// ��������ȣ
		cs.Format("%ld",pRec->m_nMarkSectionBasic);
		m_pGrid->SetTextMatrix(nRow, nCol++, cs, (UINT)DT_RIGHT);		// �⺻�ܸ��ȣ
		cs.Format("%ld",pRec->m_nMarkSection);
		m_pGrid->SetTextMatrix(nRow, nCol++, cs, (UINT)DT_RIGHT);		// �ܸ��ȣ
		cs.Format("%ld",pRec->m_nSectionNumber);
		m_pGrid->SetTextMatrix(nRow, nCol++, cs, (UINT)DT_RIGHT);		// ���� ��ȣ
		
		for(i=0; i<nItem; i++)	AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPre = pRec;
		m_pPaintOutSheet->m_Progress.StepIt();
	}	
	m_pPaintOutSheet->m_Progress.ShowWindow(SW_HIDE);
	m_pPaintOutSheet->m_CountText.ShowWindow(SW_SHOW);
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 11;
	m_pGrid->SetTextMatrix(nRow,nCol++, "SUB TOTAL",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
		if(AreaSub[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 11;
	m_pGrid->SetTextMatrix(nRow,nCol++, "�Ѹ���",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridBoltSang()
{
	if(!GetSafeHwnd()) return;
	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();
	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CPtrList		*pList	= &pPaStd->m_ListBoltSang;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	CString cs = _T("");
	long nRow   = 0; 
	long nCol   = 0;	

	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		nCol = 0;	
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);	

		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMarkBuje,(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMaterial,(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);	
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);
		cs = GetStringOutValue(pRec->m_dUWeight,nRoundUnit);
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
		cs.Format("%d",pRec->m_nQty);
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
		cs = GetStringOutValue(pRec->m_dTWeight,nRoundTot);
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridBoltTotal()
{
	if(!GetSafeHwnd()) return;
	CPaintCreator		*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CGlobarOption		*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();
	CPtrList			*pList	= &pPaStd->m_ListBoltTotal;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	CString cs = _T("");
	long nRow   = 0; 
	long nCol   = 0;	

	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// ���߷� ���� �ݿø� �ڸ���
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// �����߷� ���� �ݿø� �ڸ���

	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		nCol = 0;	
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);	

		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sMaterial,(UINT)DT_LEFT);
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);	
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);
		cs = GetStringOutValue(pRec->m_dUWeight,nRoundUnit);
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
		cs.Format("%d",pRec->m_nQty);
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
		cs = GetStringOutValue(pRec->m_dTWeight,nRoundTot);
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CPaintOutAssembly::DrawGridBujeTotal()
{
}

void CPaintOutAssembly::DrawGridBujeSang()
{
	if(!GetSafeHwnd()) return;

	CPaintCreator	*pPaStd	= m_pPaintOutSheet->m_pDataManage->m_pPaintCreator;
	CGlobarOption	*pGOpt	= m_pPaintOutSheet->m_pDataManage->GetGlobalOption();	
	CPtrList		*pList	= &pPaStd->m_ListBujeSang;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	CString cs = _T("");
	long nRow   = 0; 
	long nCol   = 0;	
	long nItem  = PAINT_ITEM_MAX;
	long nRound = pGOpt->GetPointValueOfUnitGangJae();

	CPaintRecord *pRecPre = NULL;
	double AreaSub[PAINT_ITEM_MAX],AreaTotal[PAINT_ITEM_MAX];

	long i = 0;
	for(i=0; i<nItem; i++)	AreaSub[i]   = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		//sub total
		if( pRecPre && pRecPre->GetStrSubEleName() != pRec->GetStrSubEleName()) 
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);
			nCol = 4;
			m_pGrid->SetTextMatrix(nRow,nCol++,"SUB TOTAL",(UINT)DT_RIGHT);
			for(i=0; i<nItem; i++)
			{			
				cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
				if(AreaSub[i]==0) cs = _T("");
				m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
			}		
			for(i=0; i<nItem; i++)	AreaSub[i] = 0;
		}
		for(i=0; i<nItem; i++)	AreaSub[i] += pRec->m_dPaintArea[i];

		nCol = 0;	
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);	

		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);	// �����
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrThick(),(UINT)DT_RIGHT);		// �β�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrWidth(),(UINT)DT_RIGHT);		// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
		m_pGrid->SetTextMatrix(nRow,nCol++,atofRound(pRec->m_nQty,0),(UINT)DT_RIGHT);		// ����
		//�������		
		for(i=0; i<nItem; i++)
		{																		
			cs = GetStringOutValue(toM2(pRec->m_dPaintArea[i]),nRound);
			if(pRec->m_dPaintArea[i]==0) cs = _T("");
			m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);					
		}
		//
		for(i=0; i<nItem; i++)	AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPre = pRec;
	}
	//last row sub weight
	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 4;
	m_pGrid->SetTextMatrix(nRow,nCol++, "SUB TOTAL",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
		if(AreaSub[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);
	nCol = 4;
	m_pGrid->SetTextMatrix(nRow,nCol++, "�Ѹ���",(UINT)DT_RIGHT);
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_pGrid->SetTextMatrix(nRow,nCol++,cs,(UINT)DT_RIGHT);
	}		

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());

}

BOOL CPaintOutAssembly::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_sTotalRecord.GetLength() && m_pPaintOutSheet->m_CountText.GetSafeHwnd())
	{
		m_pPaintOutSheet->m_CountText.SetWindowText(m_sTotalRecord);
		m_pPaintOutSheet->m_CountText.ShowWindow(SW_SHOW);
	}
	
	return CPropertyPage::OnSetActive();
}
