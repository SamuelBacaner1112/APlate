// CuttingOuWeld.cpp : implementation file
//
#include "stdafx.h"
#include "GeneralInput.h"
#include "../PlateGangje\Gangje.h"
#include "WeldOutSheet.h"
#include "WeldOutAssembly.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeldOutAssembly property page

IMPLEMENT_DYNCREATE(CWeldOutAssembly, CPropertyPage)

CWeldOutAssembly::CWeldOutAssembly() : CPropertyPage(CWeldOutAssembly::IDD)
{
	//{{AFX_DATA_INIT(CWeldOutAssembly)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = new CGridCtrl;
	m_sTotalRecord = _T("");
}

CWeldOutAssembly::~CWeldOutAssembly()
{
	delete m_pGrid;
}

void CWeldOutAssembly::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeldOutAssembly)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeldOutAssembly, CPropertyPage)
	//{{AFX_MSG_MAP(CWeldOutAssembly)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeldOutAssembly message handlers

UINT CWeldOutAssembly::DoWork()
{
	m_pGrid->SetRunningThread(TRUE);
	InitGrid();
	DrawGrid();	
	m_pGrid->SetRunningThread(FALSE);
	return 0;
}

BOOL CWeldOutAssembly::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	SendMessage(WM_SIZE);
//	InitGrid();
//	DrawGrid();
	
	Begin();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CWeldOutAssembly::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

void CWeldOutAssembly::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	m_pWeldOutSheet->GetClientRect(&rect);
	SetWindowPos(NULL,0,0,rect.Width()-23,rect.Height()-60,SWP_NOMOVE);

	GetClientRect(&rect);	
	rect.left += 5;
	rect.right -= 5;
	rect.top += 5;	
	rect.bottom -= 5;	
	if(m_pGrid->GetSafeHwnd())
		m_pGrid->SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),SWP_NOMOVE);			
}

void CWeldOutAssembly::InitGrid()
{
	switch(m_nMakeFlag)
	{
		case WELDMAKE_SANG:
			InitGridSang();
			break;
		case WELDMAKE_ASSEMBLY:
			InitGridAssembly();
			break;
		case WELDMAKE_ASSEMBLY_SANG:
			InitGridAssemblySang();
			break;
		case WELDMAKE_ASSEMBLY_TOTAL:
			InitGridAssemblyTotal();
			break;
		case WELDMAKE_WELDTYPE:
			InitGridWeldType();
			break;
		case WELDMAKE_WELDTHICK:
			InitGridWeldThick();
			break;
	}
}

void CWeldOutAssembly::DrawGrid()
{
	if(!GetSafeHwnd()) return;

	switch(m_nMakeFlag)
	{
		case WELDMAKE_SANG:
			DrawGridSang();
			break;
		case WELDMAKE_ASSEMBLY:
			DrawGridAssembly();
			break;
		case WELDMAKE_ASSEMBLY_SANG:
			DrawGridAssemblySang();
			break;
		case WELDMAKE_ASSEMBLY_TOTAL:
			DrawGridAssemblyTotal();
			break;
		case WELDMAKE_WELDTYPE:
			DrawGridWeldType();
			break;
		case WELDMAKE_WELDTHICK:
			DrawGridWeldThick();
			break;
	}
	//
//���� ������ 
	double row = m_pGrid->GetRowCount();
	double col = m_pGrid->GetColumnCount();
	
	for (long nRow = 1; nRow < row; nRow++) {
		for (long nCol = 0; nCol < col; nCol++) {
			if ( nRow % 2 == 0) {
				COLORREF	color = 0xfffaf0;
				m_pGrid->SetItemBkColour(nRow,nCol, color);
			}
			else {
				COLORREF color = 16777215;
				m_pGrid->SetItemBkColour(nRow,nCol, color);
			}
			if (m_pGrid->GetTextMatrix(nRow, nCol) == "SUB TOTAL") {
					COLORREF color = 8187132;
					m_pGrid->SetItemBkColour(nRow,nCol++, color);
					m_pGrid->SetItemBkColour(nRow,nCol, color);
			}
		}
	}
}

void CWeldOutAssembly::InitGridAssembly()
{
	long nRows =  0;
	long nCols = 11;
	char *Title[] = {"\n��������ũ",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",	// ������� ��������ǥ�� ����
					 "\n��������",
					 "\n�����β�\n(mm)",
					 "\n������\n(mm)",
					 "\n�������",
					 "\n�������",
				};
	InitGridFirst(nRows,nCols);
	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(2,50);
	m_pGrid->SetColumnWidth(5,50);
	m_pGrid->SetColumnWidth(6,150);// ��������
	m_pGrid->SetColumnWidth(9,150);
	m_pGrid->SetColumnWidth(10,200);

	InitGridTitle(nCols,Title);
}

void CWeldOutAssembly::InitGridSang()
{
	long nRows =  0;
	long nCols = 31;

	char *Title[] = {"\n�Ŵ�",
					 "\n����\n����",
					 "\n����",
					 "\n�����",
					 "\n�⺻�ܸ�",		//
					 "\n�ܸ鸶ũ",		//
					 "\n��������ũ",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",	// ������� ��������ǥ�� ����
					 "\n��������",
					 "\n�����β�\n(mm)",
					 "\n������\n(mm)",
					 "\n�������",
					 "\n�������",
					 "\n���Ӻ���\n����",
					 "\n���Ӻ���\n�����",
					 "\n���Ӻ���\n�β�(mm)",
					 "\n��������",
					 "\n���뵵",
					 "\n��������ũ",	//  ���ϴ� HIDDEN COLUMN
					 "\n��������ũ",
					 "\n�����ȣ",		// ���縶ũ m_nMarkBuje
					 "\n������ȣ",		// ��������ũ m_nMarkAssem
					 "\n��������ȣ",
					 "\n��������ȣ",
					 "\n�⺻�ܸ��ȣ",
					 "\n�ܸ��ȣ",		// �ܸ鸶ũ m_nMarkSection
					 "\n������ȣ",		// ������ȣ m_nSectionNumber
				};
		
	InitGridFirst(nRows,nCols);
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);			
	m_pGrid->SetColumnWidthAll(70);

	m_pGrid->SetColumnWidth( 0, 50);
	m_pGrid->SetColumnWidth( 1, 50);
	m_pGrid->SetColumnWidth( 2,120);
	m_pGrid->SetColumnWidth( 3,100);
	m_pGrid->SetColumnWidth( 6, 80);
	m_pGrid->SetColumnWidth(12,120);
	m_pGrid->SetColumnWidth(15,120);
	m_pGrid->SetColumnWidth(16,120);
	m_pGrid->SetColumnWidth(17,120);
	m_pGrid->SetColumnWidth(19,100);

	if(m_pWeldOutSheet->m_nARoadFlag==0)	// AROAD�ϰ��
	{
		m_pGrid->SetColumnWidth( 4,0);
		m_pGrid->SetColumnWidth( 5,0);
		m_pGrid->SetColumnWidth( 6,0);
		m_pGrid->SetColumnWidth( 7,0);
		m_pGrid->SetColumnWidth(20,0);
		m_pGrid->SetColumnWidth(21,0);
	}
/*
	if(!m_pWeldOutSheet->m_pDataManage->m_pBMStandard->IsMakeMark(CMarkSerial::MARK_SECTION))
		m_pGrid->SetColumnWidth(5,0);
	if(!m_pWeldOutSheet->m_pDataManage->m_pBMStandard->IsMakeMark(CMarkSerial::MARK_ASSEM))
		m_pGrid->SetColumnWidth(6,0);
	if(!m_pWeldOutSheet->m_pDataManage->m_pBMStandard->IsMakeMark(CMarkSerial::MARK_BUJE))
		m_pGrid->SetColumnWidth(7,0);
*/
	InitGridTitle(nCols,Title);	
	// Hidden
	if(m_nMakeFlag==MAKE_BM_SANG)
		for(long i=22; i<31; i++)
			m_pGrid->SetColumnWidth(i,0);
}

void CWeldOutAssembly::InitGridAssemblySang()
{
	long nRows =  0;
	long nCols = 13;
	char *Title[] = {"\n��������ũ",
					 "\n��������ũ",
					 "\n��������ũ",
					 "\n���縶ũ",
					 "\n�β�\n(mm)",
					 "\n�ʺ�\n(mm)",
					 "\n����\n(mm)",
					 "\n����\n(��)",	// ������� ��������ǥ�� ����
					 "\n��������",
					 "\n�����β�\n(mm)",
					 "\n������\n(mm)",
					 "\n�������",
					 "\n�������"
				};
	InitGridFirst(nRows,nCols);
	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0,80);
	m_pGrid->SetColumnWidth(1,80);
	m_pGrid->SetColumnWidth(2,80);
	m_pGrid->SetColumnWidth(4,50);
	m_pGrid->SetColumnWidth(7,50);
	m_pGrid->SetColumnWidth(8,120);// ��������
	m_pGrid->SetColumnWidth(11,120);
	m_pGrid->SetColumnWidth(12,120);

	InitGridTitle(nCols,Title);
}

void CWeldOutAssembly::InitGridAssemblyTotal()
{
	long nRows = 0;
	long nCols = 6;
	char *Title[] = {"\n��������ũ",
					 "\n��������",
					 "\n�����β�\n(mm)",
					 "\n������\n(mm)",
					 "\n�������",
					 "\n�������",
				};
	InitGridFirst(nRows,nCols);
	
	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(70);
	m_pGrid->SetColumnWidth(0, 80);
	m_pGrid->SetColumnWidth(1,120);// ��������
	m_pGrid->SetColumnWidth(4,120);
	m_pGrid->SetColumnWidth(5,120);

	InitGridTitle(nCols,Title);
}

void CWeldOutAssembly::InitGridWeldType()
{
	long nRows = 0;
	long nCols = 7;
	char *Title[] = {"\n��������", "\n��������", "\n�������",
					 "\n������\n(mm)",
					 "\n�����β�\n(mm)",
					 "\n����з�", 
					 "\n�����"};

	InitGridFirst(nRows,nCols);

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(80);
	m_pGrid->SetColumnWidth( 0,120);
	m_pGrid->SetColumnWidth( 1,120);
	m_pGrid->SetColumnWidth( 2,120);
	m_pGrid->SetColumnWidth( 5, 90);// �����
	m_pGrid->SetColumnWidth( 6,120);

	InitGridTitle(nCols,Title);
}

void CWeldOutAssembly::InitGridWeldThick()
{
	long nRows = 0;
	long nCols = 5;
	char *Title[] = {"\n��������", 
		             "\n��������", 
					 "\n�������",
					 "\n�����β�\n(mm)",
					 "\n������\n(mm)"
					 };

	InitGridFirst(nRows,nCols);

	m_pGrid->SetRowHeightAll(18);
	m_pGrid->SetRowHeight(0,50);
	//
	m_pGrid->SetColumnWidthAll(80);
	m_pGrid->SetColumnWidth(0,120);
	m_pGrid->SetColumnWidth(1,120);
	m_pGrid->SetColumnWidth(2,120);

	InitGridTitle(nCols,Title);
}

void CWeldOutAssembly::DrawGridSang()
{
	CWeldStandard* pWeldStd = m_pWeldOutSheet->m_pDataManage->m_pWeldStandard;
	CPtrList *pList = &pWeldStd->m_ListSang;

	if(pList->GetCount() <= 0) return;
	m_pWeldOutSheet->m_CountText.ShowWindow(SW_HIDE);
	m_pWeldOutSheet->m_Progress.ShowWindow(SW_SHOW);
	m_pWeldOutSheet->m_Progress.SetRange(0,pList->GetCount());
	m_pWeldOutSheet->m_Progress.SetPos(0);

	CWeldRecord* pRecPrev = NULL;
	POSITION pos = pList->GetHeadPosition();

	long nRow = 0;
	double dSubTotal = 0, dTotal = 0;
	CString cs = _T("");
	long nGStt  = m_pWeldOutSheet->m_pDataManage->GetBridge()->GetNumberStartGirder();
	long nSpStt = m_pWeldOutSheet->m_pDataManage->GetBridge()->GetNumberStartSplice();
	while( pos )
	{	
		CWeldRecord * pRec = (CWeldRecord *)pList->GetNext(pos);

		if(pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice)
		{
			nRow++;
			m_pGrid->SetRowCount(nRow+1);
			m_pGrid->SetRowHeight(nRow,18);

			cs.Format("%5.0lf",dSubTotal);
			dTotal += dSubTotal;
			dSubTotal = Round(pRec->m_dLengthWeld,0);

			m_pGrid->SetTextMatrix(nRow, 13, "SUB TOTAL", (UINT)DT_RIGHT);
			m_pGrid->SetTextMatrix(nRow, 14, cs, (UINT)DT_RIGHT);
			//SetColorSubTotal(nRow);
		}
		else
			dSubTotal += Round(pRec->m_dLengthWeld,0);

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrGirder(nGStt));							// �Ŵ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSplice(nSpStt));						// ��������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrEleName(),(UINT)DT_LEFT);				// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSubEleName(),(UINT)DT_LEFT);			// �����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkSectionBasic,(UINT)DT_LEFT);			// �⺻�ܸ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkSection,(UINT)DT_LEFT);				// �ܸ鸶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkAssem,(UINT)DT_LEFT);					// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkBuje,(UINT)DT_LEFT);					// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);				// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);				// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);				// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,COMMA(pRec->m_nQty),(UINT)DT_RIGHT);				// ����
		//		
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sWeldType,(UINT)DT_RIGHT);					//��������
		cs.Format("%.0f",pRec->m_dThickWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);									//�����β�
		cs.Format("%.0f",pRec->m_dLengthWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);									//������\n(mm)
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sImproveType,(UINT)DT_RIGHT);				//�������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sLayType,(UINT)DT_RIGHT);					//�������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrEleConnectName(),(UINT)DT_RIGHT);		//���Ӻ���\n����
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrSubEleConnectName(),(UINT)DT_RIGHT);	//���Ӻ���\n�����
		cs.Format("%.0f",pRec->m_dThickConnect);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);									//���Ӻ���\n�β�(mm)
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sRefDWG,(UINT)DT_LEFT);						// ��������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sUsage,(UINT)DT_LEFT);						// ���뵵
		//////// HIDDEN  //////////////
		// ��������ũ
		m_pGrid->SetTextMatrix(nRow, nColCount++, pRec->m_sMarkGrandAssem, (UINT)DT_RIGHT);
		// ��������ũ
		m_pGrid->SetTextMatrix(nRow, nColCount++, pRec->m_sMarkMidAssem, (UINT)DT_RIGHT);
		//
		cs.Format("%ld",pRec->m_nMarkBuje);
		m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// �����ȣ
		cs.Format("%ld",pRec->m_nMarkAssem);
		m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ������ȣ
		cs.Format("%ld",pRec->m_nMarkMidAssem);
		m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ��������ȣ
		cs.Format("%ld",pRec->m_nMarkGrandAssem);
		m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ��������ȣ
		cs.Format("%ld",pRec->m_nMarkSectionBasic);
		m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// �⺻�ܸ��ȣ
		cs.Format("%ld",pRec->m_nMarkSection);
		m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// �ܸ��ȣ
		cs.Format("%ld",pRec->m_nSectionNumber);
		m_pGrid->SetTextMatrix(nRow, nColCount++, cs, (UINT)DT_RIGHT);		// ���� ��ȣ

		pRecPrev = pRec;
		m_pWeldOutSheet->m_Progress.StepIt();
	}

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);

	cs.Format("%5.0lf",dSubTotal);
	dTotal += dSubTotal;

	m_pGrid->SetTextMatrix(nRow, 13, "SUB TOTAL", (UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow, 14, cs, (UINT)DT_RIGHT);
	//SetColorSubTotal(nRow);

	nRow++;
	m_pGrid->SetRowCount(nRow+1);
	m_pGrid->SetRowHeight(nRow,18);

	cs.Format("%5.0lf",dTotal);
	m_pGrid->SetTextMatrix(nRow, 13, "�ѱ���", (UINT)DT_RIGHT);
	m_pGrid->SetTextMatrix(nRow, 14, cs, (UINT)DT_RIGHT);
	//SetColorSubTotal(nRow);

	m_pWeldOutSheet->m_Progress.ShowWindow(SW_HIDE);
	m_pWeldOutSheet->m_CountText.ShowWindow(SW_SHOW);

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CWeldOutAssembly::DrawGridAssembly()
{
	CWeldStandard* pWeldStd = m_pWeldOutSheet->m_pDataManage->m_pWeldStandard;
	CPtrList *pList = &pWeldStd->m_ListAssembly;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow = 0; CString str = _T("");

	CWeldRecord *pRecPre = NULL;
	CString cs;
	while( pos )
	{	
		CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkBuje,(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);	// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);	// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,COMMA(pRec->m_nQty),(UINT)DT_RIGHT);	// ����
		//		
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sWeldType,(UINT)DT_RIGHT);	//��������
		cs.Format("%.0f",pRec->m_dThickWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);					//�����β�\n(mm)
		cs.Format("%.0f",pRec->m_dLengthWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);					//������\n(mm)
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sImproveType,(UINT)DT_RIGHT);//�������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sLayType,(UINT)DT_RIGHT);	//�������

		pRecPre = pRec;
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CWeldOutAssembly::DrawGridAssemblySang()
{
	CWeldStandard* pWeldStd = m_pWeldOutSheet->m_pDataManage->m_pWeldStandard;
	CPtrList *pList = &pWeldStd->m_ListAssemblySang;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow = 0; CString str = _T("");

	CWeldRecord *pRecPre = NULL;
	CString cs;
	while( pos )
	{	
		CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkGrandAssem,(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkMidAssem,(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkAssem,(UINT)DT_LEFT);		// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkBuje,(UINT)DT_LEFT);		// ���縶ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrThick(),(UINT)DT_RIGHT);	// �β�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrWidth(),(UINT)DT_RIGHT);	// �ʺ�
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->GetStrLength(),(UINT)DT_RIGHT);	// ����
		m_pGrid->SetTextMatrix(nRow,nColCount++,COMMA(pRec->m_nQty),(UINT)DT_RIGHT);	// ����
		//		
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sWeldType,(UINT)DT_RIGHT);	//��������
		cs.Format("%.0f",pRec->m_dThickWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);					//�����β�\n(mm)
		cs.Format("%.0f",pRec->m_dLengthWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);					//������\n(mm)
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sImproveType,(UINT)DT_RIGHT);//�������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sLayType,(UINT)DT_RIGHT);	//�������

		pRecPre = pRec;
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CWeldOutAssembly::DrawGridAssemblyTotal()
{
	CWeldStandard* pWeldStd = m_pWeldOutSheet->m_pDataManage->m_pWeldStandard;
	CPtrList *pList = &pWeldStd->m_ListAssemblyTotal;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow = 0; CString str = _T("");

	CWeldRecord *pRecPre = NULL;
	CString cs;
	while( pos )
	{	
		CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

		long nColCount = 0;
		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sMarkAssem,(UINT)DT_LEFT);	// ��������ũ
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sWeldType,(UINT)DT_RIGHT);	//��������
		cs.Format("%.0f",pRec->m_dThickWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);					//�����β�\n(mm)
		cs.Format("%.0f",pRec->m_dLengthWeld);
		m_pGrid->SetTextMatrix(nRow,nColCount++,cs,(UINT)DT_RIGHT);					//������\n(mm)
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sImproveType,(UINT)DT_RIGHT);//�������
		m_pGrid->SetTextMatrix(nRow,nColCount++,pRec->m_sLayType,(UINT)DT_RIGHT);	//�������

		pRecPre = pRec;
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CWeldOutAssembly::DrawGridWeldType()
{
	CWeldStandard* pWeldStd = m_pWeldOutSheet->m_pDataManage->m_pWeldStandard;
	CPtrList *pList = &pWeldStd->m_ListWeldType;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow = 0; CString str = _T("");

	CWeldRecord *pRecPre = NULL;
	while( pos )
	{	
		CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		long nCol = 0;
		// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sWeldType);
		// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sImproveType);
		// �������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sLayType);
		// ������	
		str.Format("%7.1f",pRec->m_dLengthWeld);
		m_pGrid->SetTextMatrix(nRow,nCol++,str);
		// ��β�
		str.Format("%4.1f",pRec->m_dThickWeld);
		m_pGrid->SetTextMatrix(nRow,nCol++,str);

		// 
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrEleName());
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->GetStrSubEleName());

		pRecPre = pRec;
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}

void CWeldOutAssembly::DrawGridWeldThick()
{
	CWeldStandard* pWeldStd = m_pWeldOutSheet->m_pDataManage->m_pWeldStandard;
	CPtrList *pList = &pWeldStd->m_ListWeldThick;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	long nRow = 0; CString str = _T("");

	CWeldRecord *pRecPre = NULL;
	while( pos )
	{	
		CWeldRecord *pRec = (CWeldRecord *)pList->GetNext(pos);

		nRow++;
		m_pGrid->SetRowCount(nRow+1);
		m_pGrid->SetRowHeight(nRow,18);

		// ����׷�
		long nCol = 0;
		// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sWeldType);
		// ��������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sImproveType);
		// �������
		m_pGrid->SetTextMatrix(nRow,nCol++,pRec->m_sLayType);
		// ��β�
		str.Format("%4.1f",pRec->m_dThickWeld);
		m_pGrid->SetTextMatrix(nRow,nCol++,str);
		// ������	
		str.Format("%10.1f",pRec->m_dLengthWeld);
		m_pGrid->SetTextMatrix(nRow,nCol++,str);

		pRecPre = pRec;
	}	

	m_sTotalRecord.Format("�ѷ��ڵ� �� : %ld",pList->GetCount());
}


void CWeldOutAssembly::InitGridFirst(long nRows, long nCols)
{
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
}

void CWeldOutAssembly::InitGridTitle(long nCols, char *Title[])
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

BOOL CWeldOutAssembly::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_sTotalRecord.GetLength() && m_pWeldOutSheet->m_CountText.GetSafeHwnd())
	{
		m_pWeldOutSheet->m_CountText.SetWindowText(m_sTotalRecord);
		m_pWeldOutSheet->m_CountText.ShowWindow(SW_SHOW);
	}

	return CPropertyPage::OnSetActive();
}
