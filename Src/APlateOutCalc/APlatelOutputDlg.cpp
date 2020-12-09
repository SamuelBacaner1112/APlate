// APlateOutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "APlateOutCalc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutputDlg dialog
#define WINDOW_ORGWIDTH			592
#define WINDOW_ORGHEIGHT		290
#define WINDOW_FIRSTHEIGHT		380
#define WINDOW_SECONDHEIGHT		380
#define TOTOUTXLSU				8
#define TOTCHECKLISTSU			12


CAPlateOutputDlg::CAPlateOutputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAPlateOutputDlg::IDD, pParent)
{
	m_pProgress = new CXTPMarqueeCtrl;	
}

CAPlateOutputDlg::~CAPlateOutputDlg()
{
	delete	m_pProgress;
}

void CAPlateOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAPlateOutputDlg)
	DDX_Control(pDX, IDC_CHECK_PIC, m_buttonOpt);
	DDX_Control(pDX, IDC_PROGRESS_2, m_ctrlProgress2);
	DDX_Control(pDX, IDC_PROGRESS_1, m_ctrlProgress1);
	DDX_Control(pDX, IDC_LIST_OUTXL, m_ctrlList);
	DDX_Control(pDX, IDC_TAB_OUTXL, m_ctrlTab);
	DDX_Control(pDX, IDC_TREE_OPTION, m_ListOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAPlateOutputDlg, CDialog)
	//{{AFX_MSG_MAP(CAPlateOutputDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OUTXL, OnSelchangeTabOutxl)
	ON_BN_CLICKED(IDC_BUTTON_OUT, OnButtonOut)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_FILE_OPEN, OnButtonFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_DOMYUN_OPEN, OnButtonDomyunOpen)
	ON_BN_CLICKED(IDC_BUTTON_ALLCHECK, OnButtonAllcheck)
	ON_BN_CLICKED(IDC_BUTTON_ALLUNCHECK, OnButtonAlluncheck)
	ON_BN_CLICKED(IDC_CHECK_PIC, OnCheckPic)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FOLDER, OnButtonOpenFolder)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_MAKEANALYSIS_XL, OnReceive)
	ON_MESSAGE(WM_USER_MAKEANALYSIS_XLCHECK, OnReceiveCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPlateOutputDlg message handlers

void CAPlateOutputDlg::OnSelchangeTabOutxl(NMHDR* pNMHDR, LRESULT* pResult) 
{

	UpdateData(TRUE);
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_buttonOpt.SetWindowText("�׸�����");
		m_buttonOpt.SetCheck(m_bPic);
		m_ctrlList.ShowWindow(SW_HIDE);
		m_ListOption.ShowWindow(SW_SHOW);
	}
	else
	{
		m_buttonOpt.SetWindowText("1�����Ϸ� ���");
		m_buttonOpt.SetCheck(m_bOneFiles);
		m_ctrlList.ShowWindow(SW_SHOW);
		m_ListOption.ShowWindow(SW_HIDE);
	}
	InitListBox();

	*pResult = 0;
}

BOOL CAPlateOutputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_sXLSaveFile			= _T("");
	m_sXLCheckSaveFile		= _T("");
	m_szFileName			= _T("");
	m_nType					= 0;
	m_nFlagXLFileOut		= 0;
	m_nFlagXLCheckFileOut	= 0;
	m_bPic					= TRUE;
	m_bOneFiles				= TRUE;

	m_ctrlTab.InsertItem(0, "������꼭");
	m_ctrlTab.InsertItem(1, "�������� ���� ����Ʈ");

	long ExtStyle = LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP |	LVS_EX_FLATSB | LVS_EX_CHECKBOXES;
	m_ctrlList.SetExtendedStyle(ExtStyle);
	m_ctrlList.InsertColumn(0, "����׸�", LVCFMT_LEFT, 420);

	m_buttonOpt.SetCheck(TRUE);

	m_ctrlTab.SetCurSel(0);
	m_ctrlList.ShowWindow(SW_HIDE);
	m_ListOption.ShowWindow(SW_SHOW);

	RECT rect, rect2;
	UINT IDC_PROGRESS_MARQUEE1 = 100;
	GetDlgItem(IDC_TAB_OUTXL)->GetClientRect(&rect);
	rect2.top	 = rect.bottom + 4;
	rect2.bottom = rect.bottom + 19;
	rect2.left	 = rect.right + 10;
	rect2.right  = rect.right + 112;
	
	m_pProgress->Create(WS_CHILD | PBS_SMOOTH | WS_OVERLAPPED, rect2, this, IDC_PROGRESS_MARQUEE1);
	m_pProgress->SetPos(0);
	m_pProgress->SetMarquee(FALSE, 0);

	InitListBox();
	InitTreeData();
	SetWindowPos(NULL,0,0, WINDOW_ORGWIDTH, WINDOW_ORGHEIGHT, SWP_NOMOVE);
	CenterWindow();
	CXLControl::GetWmfViewer().CreateOrShowWindow(this,FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAPlateOutputDlg::InitListBox()
{
	CStringArray	ArrXLOut;
	LV_ITEM lvitem;

	if(m_ctrlTab.GetCurSel()==0)
	{
		// ������꼭
		ArrXLOut.Add("������� �� �ٴ���");
		ArrXLOut.Add("�𵨸� �� ��������");
		ArrXLOut.Add("����� �� �ݷ�����");
		ArrXLOut.Add("�ܸ����� ����");
		ArrXLOut.Add("�������� ����");
		ArrXLOut.Add("�������� ����");
		ArrXLOut.Add("���κ� �� �극�̽� ����");
		ArrXLOut.Add("���ܿ����� ���� �� ��뼺 ����");
	}
	else if(m_ctrlTab.GetCurSel()==1)
	{
		//������� ���� ����Ʈ
		ArrXLOut.Add("�ٴ���");
		ArrXLOut.Add("�ݷ°���");
		ArrXLOut.Add("����Ʋ��");
		ArrXLOut.Add("�ܸ����");
		ArrXLOut.Add("�ܸ��൵");
		ArrXLOut.Add("��������");
		ArrXLOut.Add("��������");
		ArrXLOut.Add("���������");
		ArrXLOut.Add("���κ� �� �극�̽� ����");
		ArrXLOut.Add("���ܿ��������");
		ArrXLOut.Add("��뼺����");
		ArrXLOut.Add("������������");
	}

	m_ctrlList.DeleteAllItems();

	for(int i=0; i < ArrXLOut.GetSize(); i++)
	{	
		CString strNum  = _T("");
		strNum.Format(" %2d. ", i+1);
		CString strItem = strNum + ArrXLOut.GetAt(i);

		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		lvitem.pszText = strItem.GetBuffer(strItem.GetLength());
		m_ctrlList.InsertItem(&lvitem);

		m_ctrlList.SetCheck(i, FALSE);
	}

}

void CAPlateOutputDlg::InitTreeData()
{
	CPlateBridgeApp	*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CGlobarOption	*pOpt			= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CDataManage		*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd	*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData		*pCalcData		= m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CFemStressCalc	StressCalc(pDataManage, pCalcStd);

	if(pCalcData->m_bReadFrameModelingOutput)
		StressCalc.SetSectionCrossBeamPos();

	CStringArray	szArrCr;
	CStringArray	szArrBr;
	CString			szText	= _T("");
	long			nNum	= 0;
	for(long nG=0; nG<pDB->GetQtyGirder()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			CSection	*pSec	= pBx->GetSection();
			if(pSec)
			{
				CPlateCrossBeam	*pCross	= pSec->GetCrossBeam();
				CVBracing		*pVBr	= pSec->GetVBracing();
				nNum	= pBx->GetNumberCrossBeam();
				if(pCross && pCross->IsSectionCheck())
				{
					szText.Format("G%d-G%d C%d", nG+1, nG+2, nNum+1);
					szArrCr.Add(szText);
				}
				if(pVBr && pVBr->IsSectionCheck())
				{
					szText.Format("G%d-G%d B%d", nG+1, nG+2, nNum+1);
					szArrBr.Add(szText);
				}
			}
			pBx = Finder.GetBxNext();
		}
	}
	
	m_ListOption.m_pOptions = &m_Options;
	m_ListOption.m_pOptions->m_Items.RemoveAll();

	pOpt->m_nCalcDocSub[1] = pOpt->GetDesignDanmyunCheck();//�ܸ����� �Ŵ�����


	m_Options.SetOption		("SH1ST_FLOOR_", pOpt->m_bCalcDocCheck[0] ?				CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH1ST_FLOOR_",	"\t1. ������� �� �ٴ���",			CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
	
	m_Options.SetOption		("SH2ND_MODEL_", pOpt->m_bCalcDocCheck[1] ?				CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH2ND_MODEL_",	"\t2. �𵨸� �� ��������",			CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption		("SH2NDGIR"	,	pOpt->m_nCalcDocSub[1]);
		m_Options.SetOptionItem	("SH2NDGIR"	,	"\t\t�𵨸�:�����Ŵ�, �����Ŵ� ��������",	0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem	("SH2NDGIR"	,	"\t\t�𵨸�:�Ŵ� �������� ����",			1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem	("SH2NDGIR"	,	"\t\t�𵨸�:��� �Ŵ� ��������",			2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOption		("SH2NDLOAD"	,	pOpt->m_nCalcDocSub[2]);
		m_Options.SetOptionItem	("SH2NDLOAD"	,	"\t\t����:������ġ ���",	0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem	("SH2NDLOAD"	,	"\t\t����:��ü��ġ ���",	1, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOption		("SH3TH_FORCE_", pOpt->m_bCalcDocCheck[2] ?				CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH3TH_FORCE_",	"\t3. ����� �� �ݷ�����",			CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("SH3TH_MEMBER_FORCE",		pOpt->m_bCalcDocSub[0]? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SH3TH_MEMBER_FORCE",	"\t\t �����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("SH3TH_REACTION",		pOpt->m_bCalcDocSub[1]? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SH3TH_REACTION",	"\t\t �ݷ�����",  CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOption		("SH4TH_STRESS_",pOpt->m_bCalcDocCheck[3] ?				CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH4TH_STRESS_",	"\t4. �ܸ����� ����",				CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption		("SH4THGIR"	,	pOpt->m_nCalcDocSub[3]);
		m_Options.SetOptionItem	("SH4THGIR"	,	"\t\t�Ŵ� �������� ����",		0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem	("SH4THGIR"	,	"\t\t��� �Ŵ� ��������",		1, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOption		("SH5TH_JOINT_",	pOpt->m_bCalcDocCheck[4] ?			CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH5TH_JOINT_",	"\t5. �������� ����",				CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption("SH5TH_JOINT_GROUND",		pOpt->m_bCalcDocSub[2]? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SH5TH_JOINT_GROUND",	"\t\t �������� ����", CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("SH5TH_JOINT_WELD",		pOpt->m_bCalcDocSub[3]? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem("SH5TH_JOINT_WELD",	"\t\t ���� ����",  CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOption		("SH6TH_REINFOR_",	pOpt->m_bCalcDocCheck[5] ?			CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH6TH_REINFOR_",	"\t6. �������� ����",				CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOption		("SH7TH_CROSS_",	pOpt->m_bCalcDocCheck[6] ?			CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH7TH_CROSS_",	"\t7. ���κ� �� �극�̽� ����",		CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
	long n = 0;
	for(n=0; n<szArrCr.GetSize(); n++)
		m_Options.SetOptionItem("\t\t"+szArrCr.GetAt(n), IDB_RADIOON, IDB_RADIOON);
	for(n=0; n<szArrBr.GetSize(); n++)
		m_Options.SetOptionItem("\t\t"+szArrBr.GetAt(n), IDB_RADIOON, IDB_RADIOON);

	m_Options.SetOption		("SH8TH_DIAPSTUD_",	pOpt->m_bCalcDocCheck[7] ?			CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
	m_Options.SetOptionItem	("SH8TH_DIAPSTUD_",	"\t8. ���ܿ�����, ��뼺���� �� �������� ���",CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	//////////////////////////////////////////////////////////////////////////
	
	m_Options.BeginTrans();
	m_ListOption.Initialize(NULL);

	HTREEITEM hItem = m_ListOption.GetRootItem();
	while(hItem)
	{
		m_ListOption.Expand(hItem,TVE_COLLAPSE);
		hItem = m_ListOption.GetNextSiblingItem(hItem);
	}	
}

void CAPlateOutputDlg::SaveTreeData()
{
	CGlobarOption	*pOpt	= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();

	pOpt->m_bCalcDocCheck[0] = m_Options.GetOption("SH1ST_FLOOR_");
	pOpt->m_bCalcDocCheck[1] = m_Options.GetOption("SH2ND_MODEL_");
	pOpt->m_bCalcDocCheck[2] = m_Options.GetOption("SH3TH_FORCE_");
	pOpt->m_bCalcDocCheck[3] = m_Options.GetOption("SH4TH_STRESS_");
	pOpt->m_bCalcDocCheck[4] = m_Options.GetOption("SH5TH_JOINT_");
	pOpt->m_bCalcDocCheck[5] = m_Options.GetOption("SH6TH_REINFOR_");
	pOpt->m_bCalcDocCheck[6] = m_Options.GetOption("SH7TH_CROSS_");
	pOpt->m_bCalcDocCheck[7] = m_Options.GetOption("SH8TH_DIAPSTUD_");

	pOpt->m_nCalcDocSub[1]	= m_Options.GetOption("SH2NDGIR");	// 2. �ܸ鰡��
	pOpt->m_nCalcDocSub[2]	= m_Options.GetOption("SH2NDLOAD");	// 2. �ܸ鰡��
	pOpt->m_nCalcDocSub[3]	= m_Options.GetOption("SH4THGIR");	// 4. �ܸ����

	pOpt->SetDesignDanmyunCheck(pOpt->m_nCalcDocSub[1]);//�ܸ����� �Ŵ�����

	pOpt->m_bCalcDocSub[0] = m_Options.GetOption("SH3TH_MEMBER_FORCE");
	pOpt->m_bCalcDocSub[1] = m_Options.GetOption("SH3TH_REACTION");
	pOpt->m_bCalcDocSub[2] = m_Options.GetOption("SH5TH_JOINT_GROUND");
	pOpt->m_bCalcDocSub[3] = m_Options.GetOption("SH5TH_JOINT_WELD");

	if (!pOpt->m_bCalcDocSub[0] && !pOpt->m_bCalcDocSub[1]) pOpt->m_bCalcDocCheck[2] = FALSE;
	if (!pOpt->m_bCalcDocSub[2] && !pOpt->m_bCalcDocSub[3]) pOpt->m_bCalcDocCheck[4] = FALSE;
}

void CAPlateOutputDlg::SaveFileXLFormat()
{
	CAPlateOutExcel	*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CTStandard		*pTStd			= m_pAPlateOutCalcStd->GetTStandard();

	CString cs;
	CString	csName	= m_pAPlateOutCalcStd->GetDocument()->GetTitle();
	int		nPos	= 0;

	CXLControl excel;
	double dVersion = excel.GetVersion();

	if(csName.Find('.') >=0 )
	{
		nPos	= csName.Find('.');
		csName	= csName.Left(nPos);
	}

	cs.Format("-%ld", GetCurXLFile()+1);
	csName += cs;

	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel ���� ���� (*.xls)|*.xls|������� (*.*)|*.*||") : _T("Microsoft Excel ���� ���� (*.xlsx)|*.xlsx|������� (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	dlg.m_ofn.lpstrTitle = "���� ���� ����";
	if (dlg.DoModal() != IDOK)
	{
		SetDlgEnable(FALSE);
		SetWindowPos(NULL, 0, 0, WINDOW_ORGWIDTH, WINDOW_ORGHEIGHT, SWP_NOMOVE);
		return;
	}
	m_sXLSaveFile	= dlg.GetPathName();
	m_szFileName	= dlg.GetFileName();

	pTStd->SetPaneText(0,"�������� ��� �غ��� �Դϴ�...",TRUE);

	//  Ready Thread
	pAPlateOutXL->m_sXLSaveFile[0] = m_sXLSaveFile;	// 
	GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText("");
	GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText("");
	pAPlateOutXL->Begin(this, WM_USER_MAKEANALYSIS_XL);

	pTStd->SetPaneText(0,"",TRUE);
}

void CAPlateOutputDlg::SetXLOutput()
{
	CGlobarOption	*pOpt		= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CAPlateOutExcel			*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutExcelCheck	*pOutXLCheck	= m_pAPlateOutCalcStd->GetAPlateOutExcelCheck();

	CString	szText;
	long	nTab	= m_ctrlTab.GetCurSel();
	long	nTotOut	= 0;

	if(nTab == 0)	nTotOut = TOTOUTXLSU;
	else			nTotOut = TOTCHECKLISTSU;

	m_nPageArr.RemoveAll();

	if(nTab==0)
	{
		for(long i = 0; i < nTotOut; i++)
		{
			if(pOpt->m_bCalcDocCheck[i])
				m_nPageArr.Add(i+1);
		}
		if(m_nPageArr.GetSize() == 1)
		{
			pAPlateOutXL->SetAllFiles(FALSE);
			m_nType = 0;
			m_nFlagXLFileOut = m_nPageArr.GetAt(0)-1;
		}
		else if(m_nPageArr.GetSize() > 1)
		{
			pAPlateOutXL->SetAllFiles(TRUE);
			m_nType = 1;
		}
		else if(m_nPageArr.GetSize() == TOTOUTXLSU)
		{
			pAPlateOutXL->SetAllFiles(TRUE);
			m_nType = 2;
		}
		else
		{
			pAPlateOutXL->SetAllFiles(FALSE);
			m_nType = 99;
		}
	}
	else
	{
		for(long i = 0; i < nTotOut; i++)
		{
			if(m_ctrlList.GetCheck(i))
				m_nPageArr.Add(i+1);
		}
		if(m_nPageArr.GetSize() == 1)
		{
			m_nType = 3;
			pOutXLCheck->SetAllFiles(FALSE);
			m_nFlagXLCheckFileOut = m_nPageArr.GetAt(0)-1;
		}
		else if(m_nPageArr.GetSize() > 1)
		{
			pOutXLCheck->SetAllFiles(TRUE);
			m_nType = 4;
		}
		else if(m_nPageArr.GetSize() == TOTOUTXLSU)
		{
			pOutXLCheck->SetAllFiles(TRUE);
			m_nType = 5;
		}
		else
		{
			pOutXLCheck->SetAllFiles(FALSE);
			m_nType = 99;
		}
	}
}

void CAPlateOutputDlg::OnButtonOut() 
{
	CAPlateOutExcel			*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutExcelCheck	*pOutXLCheck	= m_pAPlateOutCalcStd->GetAPlateOutExcelCheck();

	CXLControl excel;
	double dVersion = excel.GetVersion();

	pAPlateOutXL->Abort(FALSE);

	UpdateData(TRUE);
	SetDlgEnable(FALSE);
	SaveTreeData();	
	SetXLOutput();
	
	if(m_nType > 6)
	{	
		MessageBox("����׸��� �����Ͻʽÿ�.", "����");
		return;
	}

	switch(m_nType)
	{
		case 0:
		{
			SetWindowPos(NULL, 0, 0, WINDOW_ORGWIDTH, WINDOW_FIRSTHEIGHT, SWP_NOMOVE);
			SaveFileXLFormat();
			break;
		}
		case 1:
		{

			SetWindowPos(NULL,0,0, WINDOW_ORGWIDTH, WINDOW_SECONDHEIGHT, SWP_NOMOVE);

			CString csName = m_pAPlateOutCalcStd->GetDocument()->GetTitle();
			if(csName.Find('.') >=0)
			{
				int pos = csName.Find('.'); 
				csName = csName.Left(pos);
			}
			
			if(m_nPageArr.GetSize()==1)
			{
				CString cs;
				long nPage = m_nPageArr.GetAt(0);
				cs.Format("%d", nPage);
				csName += cs;
			}

			CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel ���� ���� (*.xls)|*.xls|������� (*.*)|*.*||") : _T("Microsoft Excel ���� ���� (*.xlsx)|*.xlsx|������� (*.*)|*.*||");
			CFileDialog dlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

			dlg.m_ofn.lpstrTitle = "�׼� ���� ����";
			if(dlg.DoModal() != IDOK)
			{
				SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);
				return;
			}
			m_sXLSaveFile	= dlg.GetPathName();
			m_szFileName	= dlg.GetFileName();

			pAPlateOutXL->m_sXLSaveFile[0] = m_sXLSaveFile;
			GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText("");
			GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText("");
			pAPlateOutXL->Begin(this, WM_USER_MAKEANALYSIS_XL);

			break;
		}
		case 2:
		{
			SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_SECONDHEIGHT,SWP_NOMOVE);
			m_nPageArr.SetSize(TOTOUTXLSU);

			CString csName = m_pAPlateOutCalcStd->GetDocument()->GetTitle();
			if(csName.Find('.') >=0)
			{
				int pos = csName.Find('.'); 
				csName = csName.Left(pos);
			}
			
			CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel ���� ���� (*.xls)|*.xls|������� (*.*)|*.*||") : _T("Microsoft Excel ���� ���� (*.xlsx)|*.xlsx|������� (*.*)|*.*||");
			CFileDialog dlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

			dlg.m_ofn.lpstrTitle = "�׼� ���� ����";
			if (dlg.DoModal() != IDOK)
			{
				SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);
				return;
			}
			m_sXLSaveFile	= dlg.GetPathName();
			m_szFileName	= dlg.GetFileName();

			pAPlateOutXL->m_sXLSaveFile[0] = m_sXLSaveFile;
			GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText("");
			GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText("");
			pAPlateOutXL->Begin(this,WM_USER_MAKEANALYSIS_XL);
			break;
		}
		case 3:
		{
			SetWindowPos(NULL, 0, 0, WINDOW_ORGWIDTH, WINDOW_FIRSTHEIGHT, SWP_NOMOVE);
			SaveFileXLCheckFormat();
			break;
		}
		case 4:
		{
			SetWindowPos(NULL, 0, 0, WINDOW_ORGWIDTH, WINDOW_SECONDHEIGHT, SWP_NOMOVE);

			CString csName = m_pAPlateOutCalcStd->GetDocument()->GetTitle();
			if(csName.Find('.') >=0)
			{
				int pos = csName.Find('.'); 
				csName = csName.Left(pos)+"-Check";
			}
			
			if(m_nPageArr.GetSize()==1)
			{
				CString cs;
				long nPage = m_nPageArr.GetAt(0);
				cs.Format("%d", nPage);
				csName += cs;
			}

			CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel ���� ���� (*.xls)|*.xls|������� (*.*)|*.*||") : _T("Microsoft Excel ���� ���� (*.xlsx)|*.xlsx|������� (*.*)|*.*||");
			CFileDialog dlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

			dlg.m_ofn.lpstrTitle = "�׼� ���� ����";
			if(dlg.DoModal() != IDOK)
			{
				SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);
				return;
			}
			m_sXLCheckSaveFile	= dlg.GetPathName();
			m_szFileName		= dlg.GetFileName();

			pOutXLCheck->SetAllFiles(TRUE);
			pOutXLCheck->m_sXLCheckSaveFile[0] = m_sXLCheckSaveFile;
			GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText("");
			GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText("");
			pOutXLCheck->Begin(this, WM_USER_MAKEANALYSIS_XLCHECK);
			break;
		}
		case 5:
		{
			SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_SECONDHEIGHT,SWP_NOMOVE);
			m_nPageArr.SetSize(TOTCHECKLISTSU);

			CString csName = m_pAPlateOutCalcStd->GetDocument()->GetTitle();
			if(csName.Find('.') >=0)
			{
				int pos = csName.Find('.'); 
				csName = csName.Left(pos)+"-Check";
			}
			
			CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel ���� ���� (*.xls)|*.xls|������� (*.*)|*.*||") : _T("Microsoft Excel ���� ���� (*.xlsx)|*.xlsx|������� (*.*)|*.*||");
			CFileDialog dlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

			dlg.m_ofn.lpstrTitle = "�׼� ���� ����";
			if (dlg.DoModal() != IDOK)
			{
				SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);
				return;
			}
			m_sXLCheckSaveFile	= dlg.GetPathName();
			m_szFileName		= dlg.GetFileName();

			pOutXLCheck->SetAllFiles(TRUE);
			pOutXLCheck->m_sXLCheckSaveFile[0] = m_sXLCheckSaveFile;
			GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText("");
			GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText("");
			pOutXLCheck->Begin(this,WM_USER_MAKEANALYSIS_XLCHECK);
			break;
		}
	}
}

void CAPlateOutputDlg::SaveFileXLCheckFormat()
{
	CAPlateOutExcelCheck	*pOutXLCheck	= m_pAPlateOutCalcStd->GetAPlateOutExcelCheck();
	CXLControl excel;
	double dVersion = excel.GetVersion();

	CString csName = m_pAPlateOutCalcStd->GetDocument()->GetTitle();

	if( csName.Find('.') >=0) { int pos = csName.Find('.'); csName = csName.Left(pos);}
	CString cs;	cs.Format("-Check%ld",GetCurXLCheckFile()+1);		csName += cs;
	
	CString sFileForm = (dVersion < 12) ? _T("Microsoft Excel ���� ���� (*.xls)|*.xls|������� (*.*)|*.*||") : _T("Microsoft Excel ���� ���� (*.xlsx)|*.xlsx|������� (*.*)|*.*||");
	CFileDialog dlg(FALSE, "*.xls|*.*", csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);

	
	dlg.m_ofn.lpstrTitle = "�׼� ���� ����";
	if (dlg.DoModal() != IDOK)
	{
		SetWindowPos(NULL,0,0,WINDOW_ORGWIDTH,WINDOW_ORGHEIGHT,SWP_NOMOVE);
		return;
	}
	m_sXLCheckSaveFile	= dlg.GetPathName();
	m_szFileName		= dlg.GetFileName();

	//  Ready Thread
	pOutXLCheck->m_sXLCheckSaveFile[0] = m_sXLCheckSaveFile;
	GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText("");
	GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText("");
	pOutXLCheck->Begin(this, WM_USER_MAKEANALYSIS_XLCHECK);
}

void CAPlateOutputDlg::SetDlgEnable(BOOL bEnable)
{
	if(bEnable)
	{
		GetDlgItem(IDC_BUTTON_OUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FILE_OPEN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FILE_OPEN)->EnableWindow(TRUE);
	}
}

void CAPlateOutputDlg::OnCancel() 
{
	CAPlateOutExcel			*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutExcelCheck	*pOutXLCheck	= m_pAPlateOutCalcStd->GetAPlateOutExcelCheck();

	if(pAPlateOutXL->GetWinThread() || pOutXLCheck->GetWinThread())
	{
		pAPlateOutXL->Abort(TRUE);
		m_pProgress->SetMarquee(FALSE, 0);
		return;
	}
	
	SaveTreeData();
	CDialog::OnCancel();
}

void CAPlateOutputDlg::OnClose() 
{
	CAPlateOutExcel			*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutExcelCheck	*pOutXLCheck	= m_pAPlateOutCalcStd->GetAPlateOutExcelCheck();

	if(pAPlateOutXL->GetWinThread() || pOutXLCheck->GetWinThread())
	{
		MessageBox ("���� �۾��� �������Դϴ�.", "�˸�", MB_OK | MB_ICONINFORMATION);
		return;
	}

	SaveTreeData();

	CDialog::OnClose();
}



UINT CAPlateOutputDlg::OnMakeAnalysisCal()
{

//	CTStandard		*pTS	= m_pAPlateOutCalcStd->GetTStandard();
//	CVAbortPgsDlg	*pADlg	= (CVAbortPgsDlg*)pTS->m_pAbortPgsDlg;
	UINT nErrResult = 0;
	try
	{
//		m_pAPlateOutCalcStd->GetAPlateOutExcelCheck()->MakeAnalysisCal();
	}
	catch(...)
	{
		nErrResult = 2;
	}

//	pADlg->OnOK();

	return nErrResult;
}

HHOOK m_hHook = NULL;

LRESULT CALLBACK MessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode==HCBT_ACTIVATE)
		keybd_event((unsigned char)VkKeyScan('R'),0,KEYEVENTF_EXTENDEDKEY,0);

	return CallNextHookEx(m_hHook,nCode,wParam,lParam);

}

void CAPlateOutputDlg::OnButtonFileOpen() 
{
	CString	szFile	= m_ctrlTab.GetCurSel() == 0 ? m_sXLSaveFile : m_sXLCheckSaveFile;

	//if(m_pAPlateOutCalcStd->m_dXLVersion>=12.0)
	//	szFile += _T("x");

	if(m_ctrlTab.GetCurSel() == 0)
		ShellExecute(NULL, "open", szFile, NULL,NULL, SW_SHOWNORMAL);
	else if(m_ctrlTab.GetCurSel() == 1)
		ShellExecute(NULL, "open", szFile, NULL, NULL, SW_SHOWNORMAL);
}

void CAPlateOutputDlg::OnButtonDomyunOpen() 
{
	CXLControl::GetWmfViewer().CreateOrShowWindow(this,TRUE);
}

void CAPlateOutputDlg::OnButtonAllcheck() 
{
	UpdateData(TRUE);
	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		AllCheckCalcSheet(TRUE);
		SaveTreeData();
	}
	else							
	{
		for(int i=0; i < TOTCHECKLISTSU; i++)
			m_ctrlList.SetCheck(i, TRUE);	
	}
}

void CAPlateOutputDlg::OnButtonAlluncheck() 
{
	UpdateData(TRUE);
	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		AllCheckCalcSheet(FALSE);
		SaveTreeData();
	}
	else
	{
		for(int i=0; i < TOTCHECKLISTSU; i++)
			m_ctrlList.SetCheck(i, FALSE);	
	}
}

LRESULT CAPlateOutputDlg::OnReceive(WPARAM wp,LPARAM lp)
{
	switch( wp )
	{
	case 0 :	m_ctrlProgress1.SetPos((int)lp);
		break;
	case 1:		GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText((LPCTSTR)lp);
		break;
	case 2 :	m_ctrlProgress2.SetPos((int)lp);
		break;
	case 3:		GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText((LPCTSTR)lp);
		break;
	}

	CAPlateOutExcel	*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	
	int iInterval = 50;
	if(pAPlateOutXL->m_bAbort == FALSE && GetDlgItem(IDC_BUTTON_OUT)->IsWindowEnabled() == FALSE)	
	{
		m_pProgress->ShowWindow(SW_SHOW);
		m_pProgress->SetPos(0);
		m_pProgress->SetMarquee(TRUE, iInterval);
		
	}
	else
	{						
		m_pProgress->ShowWindow(SW_HIDE);
		m_pProgress->SetMarquee(FALSE, iInterval);
		m_pProgress->SetPos(0);
	}

	return 0;
}

LRESULT CAPlateOutputDlg::OnReceiveCheck(WPARAM wp,LPARAM lp)
{
	switch( wp )
	{
	case 0 :	m_ctrlProgress1.SetPos((int)lp);
		break;
	case 1:		GetDlgItem(IDC_STATIC_TEXT_OUT)->SetWindowText((LPCTSTR)lp);
		break;
	case 2 :	m_ctrlProgress2.SetPos((int)lp);
		break;
	case 3:		GetDlgItem(IDC_STATIC_TOTTEXT_OUT)->SetWindowText((LPCTSTR)lp);
		break;
	}

	return 0;
}

BOOL CAPlateOutputDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ) return FALSE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==13 )
	{
		if(	m_hWnd == pMsg->hwnd ) return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CAPlateOutputDlg::OnMakeXLFile() 
{
	CAPlateOutExcel			*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	//CAPlateOutExcelCheck	*pOutXLCheck	= m_pAPlateOutCalcStd->GetAPlateOutExcelCheck();

	CXLControl::GetWmfViewer().ClearAll();
	pAPlateOutXL->DoWork();
}

void CAPlateOutputDlg::OnCheckPic() 
{
	UpdateData(TRUE);
	if(m_ctrlTab.GetCurSel()==0)
	{
		if(m_buttonOpt.GetCheck())
			m_bPic = TRUE;
		else
			m_bPic = FALSE;
	}
	else
	{
		if(m_buttonOpt.GetCheck())
			m_bOneFiles = TRUE;
		else
			m_bOneFiles = FALSE;
	}
}

void CAPlateOutputDlg::AllCheckCalcSheet(BOOL bEnable)
{
	CGlobarOption	*pOpt		= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();

	for(long n=0; n<OPT_CHKSHEET; n++)
	{
		pOpt->m_bCalcDocCheck[n]			= bEnable;		// �ܱ�����꼭 ���
		pOpt->m_bCalcDocSub[n]				= bEnable;
	}

	m_ListOption.DeleteAllItems();
	InitTreeData();
}

void CAPlateOutputDlg::OnButtonOpenFolder() 
{
	CString	szPath			= _T("");
	CString	szSaveFileName	= _T("");
	long	nIndex			= 0;
	if(m_ctrlTab.GetCurSel()==0)	szSaveFileName = m_sXLSaveFile;
	else							szSaveFileName = m_sXLCheckSaveFile;

	nIndex	= szSaveFileName.Find(m_szFileName);
	szPath	= szSaveFileName.Mid(0, nIndex);

	if(nIndex!=-1)
		ShellExecute(NULL, "open", szPath, NULL, NULL, SW_SHOWNORMAL);
}
