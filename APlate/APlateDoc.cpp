// APlateDoc.cpp : implementation of the CAPlateDoc class
//

#include "stdafx.h"
#include "APlate.h"
#include "APlateDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "Splash.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPlateDoc
#define HEADER_BUF_SIZE	512

IMPLEMENT_DYNCREATE(CAPlateDoc, CDocument)

BEGIN_MESSAGE_MAP(CAPlateDoc, CDocument)
	//{{AFX_MSG_MAP(CAPlateDoc)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_OUT_LINE, OnOutLine)
END_MESSAGE_MAP()

long nBrx_Version = 0;
///////////////////
/////////////////////////////////////////////////////////////////////////////
// CAPlateDoc construction/destruction

CString CAPlateDoc::DomTitle[DOMTYPENUM] = { 
	"�Ŵ� �Ϲݵ�","�Ŵ� �󼼵�", "����극�̽� �󼼵�", "�������� �󼼵�", "���κ� �󼼵�",
		"���� �극�̽� �󼼵�","��Ÿ �󼼵�", "���� ���ǥ", "��൵", "������ħ ��ġ��", "������", "���� �󼼵�",
		"�Ϲ� ��鵵", "���� ������", "������ �Ϲݵ�", "Ⱦ���� TENDON ��ġ��", "������ ��ٵ�"/*, "Turnover �Ŵ�" APLATE-631 */
};

BOOL CAPlateDoc::m_bFirstDocument = FALSE;
CString CAPlateDoc::m_szSerial = _T("");

CAPlateDoc::CAPlateDoc()
{
	AfxInitRichEdit();

	m_bBrpFileOpen			= FALSE;
	m_bMoveCursor			= FALSE;
	m_bEnableCrossCursor	= FALSE;
	m_nOptionCur			= 0;
	m_bOptModifyCheck		= TRUE;
	m_nActiveIDD			= 0;

	//==============================================================================================
	//
	//  LOCK CHECK MODULE - "Lock" �ʱ�ȭ (���������� ������ �� 1ȸ�� �ʱ�ȭ �˴ϴ�)
	//
	//  !! �Ʒ� ��õ� Lock �Լ�(RNBO_xxx)����, �ݵ�� ���� ȣ������� ������ �ּž� �մϴ� !!
	//
	//==============================================================================================
	//APlate������, ����, �Է»���
	CAPlateApp *pApp = (CAPlateApp*)AfxGetApp();	
	
	if(!CAPlateDoc::m_bFirstDocument)	// ���� ������ ���� Lock�� üũ��
	{
		if (aIsLockOK() || LDK::aIsLogIn()) // 2015. 07. 28. Software Lock �۾�, ������
		{		
			if(LDK::aCheckLicenseHGKey ())
			{	
				//==========================================================================================
				//  STEP5 : ��ǰ Ȱ��ȭ �˻�
				//==========================================================================================
				//[ Lock ������ ���� Ȯ�� ��� ��� ���� ]==================================================		
				switch (LDK::aGetModuleNumber())
				{
				case SPLOCK_APLATE_LID:
					CGeneralBaseStd::SetModuleDeck(TRUE);
					CGeneralBaseStd::SetModuleTUGirder(FALSE);
					break;
				case SPLOCK_MASTER_LID:
					CGeneralBaseStd::SetModuleTUGirder(TRUE);
					CGeneralBaseStd::SetModuleDeck(TRUE);
					CGeneralBaseStd::SetModuleDesignStd(TRUE);
					break;
				case SPLOCK_APLATE_TU_LID:
					CGeneralBaseStd::SetModuleTUGirder(TRUE);
					CGeneralBaseStd::SetModuleDeck(TRUE);
					break;			
				}

				// Spash ========================================================================
				// CG: The following line was added by the Splash Screen component.
				CEnvRegist env;		
				if(env.LoadEnvInt(CEnvRegist::SET_PRGOPT))
					CSplashWnd::ShowSplashScreen(AfxGetMainWnd());

				CString szPath = ((CAPlateApp*)AfxGetApp())->GetFixedDir();
				if(CSplashWnd::c_pSplashWnd->GetSafeHwnd())
					CHgBaseDrawStd::GetHCadSymbolDom()->SetProgress(&CSplashWnd::c_pSplashWnd->m_ProcessBar, ((CMainFrame*)AfxGetMainWnd())->GetMessageBar());

				CHgBaseDrawStd::GetHCadSymbolDom()->DxfIn(szPath+"\\BlockDomyun\\HCAD_Symbol.dxf");
				CAPlateDoc::m_bFirstDocument	= TRUE;
			}
		}		
	}	
	
	CreateMember();
	CProgramSettingMng::m_bARoadTypeComma = TRUE;
	CProgramSettingMng::m_bUseComma		  = FALSE;

	// ExportApierInfo�Լ��� flag�� ���⼭ ���� ��
	// AAbutPier2007������ ����ؾ� �ϹǷ�..(added by jaeho 2007.06.01)
	m_nFlagExportApierInfo	= 15;
}

void CAPlateDoc::CreateMember()
{
	//�����Ͱ���
	m_pDataManage = new CDataManage;
	//���ǥ����
	m_pDataManage->m_pBMStandard    = new CBMCreator(m_pDataManage);
	m_pDataManage->m_pBMStandard->m_pTStandard = new CTStandard;
	m_pDataManage->m_pBMStandard->m_pTStandard->m_pDB = m_pDataManage->GetBridge();
	m_pDataManage->GetBMStandard()->m_pTStandard->m_pDoc = this;

	m_pDataManage->m_pPaintCreator = new CPaintCreator(m_pDataManage);	
	m_pDataManage->m_pPaintCreator->SetTStandard(new CTStandard);
	m_pDataManage->m_pPaintCreator->GetTStandard()->m_pDB = m_pDataManage->GetBridge();
	m_pDataManage->m_pPaintCreator->GetTStandard()->m_pDoc = this;

	m_pDataManage->m_pWeldStandard  = new CWeldStandard(m_pDataManage);		
	m_pDataManage->m_pWeldStandard->SetTStandard(new CTStandard);
	m_pDataManage->m_pWeldStandard->GetTStandard()->m_pDB = m_pDataManage->GetBridge();
	m_pDataManage->m_pWeldStandard->GetTStandard()->m_pDoc = this;

	m_pCalcStd	= new CAPlateCalcStd(m_pDataManage);

	//�Է°���
	m_pGenInputStd = new CGeneralBaseStd(m_pDataManage, m_pCalcStd);		
	m_pGenInputStd->m_pDoc = this; 
	m_pGenInputStd->m_pDeckData = m_pDataManage->GetDeckData();
	m_pGenInputStd->m_pDeckData->m_pDB =  m_pDataManage->GetBridge();
	m_pGenInputStd->m_pDeckDataTU = m_pDataManage->GetDeckDataTU();
	m_pGenInputStd->m_pDeckDataTU->m_pDB =  m_pDataManage->GetBridge();

	//�������
	m_pPlateDrawStd	= new CAPlateDrawStd(m_pDataManage);
	//CDrawStd�� m_pDB
	m_pPlateDrawStd->m_pDB = m_pDataManage->GetBridge();
	m_pPlateDrawStd->m_pCalcStd = m_pCalcStd;
	//CDrawStd�� m_pDoc 
	m_pPlateDrawStd->m_pDoc = this;
	m_pDomOptionStd = new COptionStd;
	m_pPlateDrawStd->m_pDomOptionStd = m_pDomOptionStd;

	//������꼭
	m_pAPlateOutCalcStd	= new CAPlateOutCalcStd(m_pDataManage, m_pCalcStd, this);
	m_pAPlateOutCalcStd->m_pGenInputStd = m_pGenInputStd;
	
	m_pPlateDrawStd->GetDogak()->m_pNameBridge	= &m_pDataManage->GetBridge()->m_sNameBridge;	
	m_pPlateDrawStd->GetDogak()->m_pNameProject	= &m_pDataManage->GetLineInfoApp()->m_sNameProject;	
	
	
	
	CHgBaseDrawStd::m_pARoadOptionStd	= m_pDataManage->GetOptionStd();
	
	//�÷���Ʈ �� ���� ...
	m_pAPlateOutBMStd	= new CAPlateOutBMStd(m_pDataManage, m_pGenInputStd, m_pDomOptionStd);
	
	for(long i=0;i<DOMTYPENUM;i++)	
		DomTitleArr.Add(DomTitle[i]);
	m_pPlateDrawStd->SetDomyunSize(DOMTYPENUM);
	m_pPlateDrawStd->SetDomyunTitleArray(DomTitleArr);
	
	m_pShareBridgeInfoStd	= new CShareBridgeInfoStd;
	
	m_pShareWithADManagerStd = new CShareWidthADManagerStd;
	m_pShareWithADManagerStd->m_pARoadOptionStd	= m_pDataManage->GetOptionStd();
	m_pShareWithADManagerStd->m_pDrawStd		= m_pPlateDrawStd;
}

void CAPlateDoc::ClearMember()
{
	delete m_pDomOptionStd;
	delete m_pPlateDrawStd;
	delete m_pGenInputStd;
	delete m_pCalcStd;
	delete m_pDataManage->m_pBMStandard->m_pTStandard;
	delete m_pDataManage->m_pBMStandard;
	delete m_pDataManage->m_pPaintCreator->GetTStandard();
	delete m_pDataManage->m_pPaintCreator;
	delete m_pDataManage->m_pWeldStandard->GetTStandard();
	delete m_pDataManage->m_pWeldStandard;	
	delete m_pDataManage;
	delete m_pAPlateOutCalcStd;
	delete m_pAPlateOutBMStd;
	delete m_pShareBridgeInfoStd;
	delete m_pShareWithADManagerStd;
	InitSteelBox_SapOut();
}

CAPlateDoc::~CAPlateDoc()
{
	ClearMember();
}

BOOL CAPlateDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CMainFrame* pMFrm = (CMainFrame*)AfxGetMainWnd();
	
// 	if(pMFrm)	pMFrm->m_wndPageViewBar.m_pIn = NULL;

	m_nActiveIDD	= 0;
	m_bBrpFileOpen	= FALSE;

	return TRUE;
}

// ���� �� ���� Serialize ���� ������� ����
BOOL CAPlateDoc::MakeBackUp(CString szPathName, BOOL bOpen)
{
	BOOL bResult = FALSE;
	if(szPathName == "") return bResult;
	
	CString szPathNameBackUp = szPathName;
	if(bOpen)	szPathNameBackUp += ".bak1";
	else		szPathNameBackUp += ".bak2";
	
	CFileFind pFind;
	if(pFind.FindFile(szPathName))
	{
		CEnvRegist reg;
		BOOL bMakeBackUpFileDate = reg.LoadEnvInt(CEnvRegist::MAKE_BACKUPFILE_DATE);
		if(bMakeBackUpFileDate && bOpen)
			bResult = ((CMSDIWinApp*)AfxGetApp())->MakeBackUpFileByDate((CDocument*)this, szPathName, FALSE);
		else
			bResult = CopyFile(szPathName, szPathNameBackUp, FALSE);
	}
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// CAPlateDoc serialization

void CAPlateDoc::Serialize(CArchive& ar)
{
	CWaitCursor Wait;
	CFileCompress FCmp;
	CFile tmpf,tmpf2;
	CFile * pMf = ar.GetFile();	

	CString csTmp = "C:\\_$ash.tmp";	
	char NullBuf[BRPHEADERBUFSIZE];
	char cValueFile = 1;

	memset(NullBuf,0,BRPHEADERBUFSIZE);
	CString csVer;
	csVer.Format("Hangil IT BRP v%-8.4lf Build v%-05ld",(double)BRPFILEVER,(long)BRPBUILDVER);
	memcpy(NullBuf,csVer.GetBuffer(csVer.GetLength()),csVer.GetLength());	// 38 byte
	NullBuf[38] = cValueFile;
	
	MakeBackUp(pMf->GetFilePath(), !ar.IsStoring());
	if (ar.IsStoring())
	{			
		pMf->Write(&BRPFILEVER,sizeof BRPFILEVER);
		pMf->Write(NullBuf,BRPHEADERBUFSIZE);	
		if(tmpf.Open(csTmp,CFile::modeCreate|CFile::modeWrite)==FALSE)
			return;
		CArchive artmp(&tmpf,CArchive::store);
		SerializeMain(artmp);
		artmp.Close();
		tmpf.Close();

		if(tmpf2.Open(csTmp,CFile::modeRead)==FALSE)
			return;
		
		FCmp.EncodeLZ(&tmpf2,pMf,1024);
		tmpf2.Close();

		CFile::Remove(csTmp);						
	}
	else
	{	
		double BrpFileVer;
		pMf->Read(&BrpFileVer,sizeof BrpFileVer);
		pMf->Read(NullBuf,BRPHEADERBUFSIZE);	
			
		{// CArchiveException
			CString s1("Hangil IT"), s2(NullBuf);
			s2 = s2.Left(9);
			if(s1 != s2)
				AfxThrowArchiveException(CArchiveException::badIndex,""); 
		}
		
		// ������������ ���۵� ������ ���� �ʴ´�
		if(BrpFileVer > (double)BRPFILEVER)
		{
			AfxMessageBox(OVER_VERSION);
			DeleteContents();
			return;
		}
		
		cValueFile = NullBuf[38];
		if(tmpf.Open(csTmp,CFile::modeCreate|CFile::modeWrite)==FALSE)
			return;
		
		FCmp.DecodeLZ(pMf,&tmpf);
		tmpf.Close();

		if(tmpf2.Open(csTmp,CFile::modeRead)==FALSE)
			return;

		CArchive artmp(&tmpf2,CArchive::load);					
		SerializeMain(artmp);			
		artmp.Close();
		tmpf2.Close();

		CFile::Remove(csTmp);
		
		// �ӵ� ������ ���Ͽ� IP ������ ����
		m_pDataManage->GetBridge()->SetGirderSearchIP();
		// ������ ������ �����Ϳ� ARoad�� ������ �ּҸ� ����
		m_pDataManage->GetGlobalOption()->SetExpansionBlockJewon(m_pCalcStd->m_pDesignExpanJoint);
	}

	((CAPlateApp*)AfxGetApp())->m_bFirFileOpen = FALSE;
}

void CAPlateDoc::SerializeMain(CArchive& ar)
{	
//	long nFlag = 1;
	long nFlag = 2;	// #28172 PS�������� ����
	
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_bOptModifyCheck;
	}
	else
	{		
		m_bBrpFileOpen = TRUE;
		
		ar >> nFlag;
		//		if(IsOverVersion(ar, nFlag)) return;	// ������������ ���۵� ������ ���� �ʴ´�
		
		if(nFlag>0)
			ar >> m_bOptModifyCheck;
	}

	m_pDataManage->Serialize(ar);
	m_pGenInputStd->Serialize(ar);
	m_pPlateDrawStd->Serialize(ar);
	m_pDomOptionStd->Serialize(ar);
	m_pCalcStd->Serialize(ar);
	
	for(long nG=0; nG<m_pDataManage->GetBridge()->GetQtyGirder(); nG++)
	{
		m_pDataManage->GetBridge()->GetGirder(nG)->ReStandVStiff();
	}
	if(nFlag<2)
	{
		CCalcData			*pData			= m_pDataManage->GetCalcData();
		CSteelPrestressPlate		*pPrestress		= m_pDataManage->GetSteelPrestress();
		long				nPrestressIdx	= pPrestress->GetIndexByPrestressName(pData->DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName);
		CSteelPrestressData *pSteelPSData	= pPrestress->GetSteelPrestressData(nPrestressIdx);
		pData->DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx	= nPrestressIdx;
		pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength	= GetValueUnitChange(pSteelPSData->GetTensionStrength(), _T("kgf/cm��"), _T("MPa"));
		pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength		= GetValueUnitChange(pSteelPSData->GetYieldStrength(), _T("kgf/cm��"), _T("MPa"));
	}
	CProgramSettingMng::m_bARoadTypeComma = TRUE;
	CProgramSettingMng::m_bUseComma		  = FALSE;		
}

/////////////////////////////////////////////////////////////////////////////
// CAPlateDoc diagnostics

#ifdef _DEBUG
void CAPlateDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAPlateDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAPlateDoc commands


BOOL CAPlateDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/)
{
	CString newName = pszPathName;
	BOOL bModified = IsModified();
	
	BOOL bSaveAs = FALSE;
	if (newName.IsEmpty())
		bSaveAs = TRUE;
	
	if (bSaveAs)
	{
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			int iBad = newName.FindOneOf(_T(" #%;/\\"));    // dubious filename
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);
			
			// append the default suffix if there is one
			newName += ".brp";
		}
		CBitmap LogoBitmap;
		LogoBitmap.LoadBitmap(IDB_BITMAP_LOGO); 
		CFileDialog dlg(FALSE, "���� ����", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
			"APlate Files (.brp)|*.brp||");	
	
		dlg.m_ofn.lpstrFile = newName.GetBuffer(_MAX_PATH);
		CString sPath = ((CAPlateApp *)AfxGetApp())->GetWorkDir();				
		if (dlg.DoModal() != IDOK) return FALSE;	
		CString szPathName = dlg.GetPathName();
		newName = szPathName;
	}
	
	BeginWaitCursor();
	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			// be sure to delete the file
			try
			{
				CFile::Remove(newName);
			}
			catch(...)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}			
		}		
		EndWaitCursor();
		return FALSE;
	}
	EndWaitCursor();
	
	if(bReplace)
	{
		SetPathName(newName, TRUE);
		ASSERT(m_strPathName == newName);       // must be set
	}
	else
	{
		SetModifiedFlag(bModified);
	}
	return TRUE;
}


void CAPlateDoc::OnOutLine() 
{
	// ���� ���� ��������
	CString csName = GetTitle();
	if( csName.Find('.') >=0) { int pos = csName.Find('.'); csName = csName.Left(pos);}
	CFileDialog dlg(FALSE, "",csName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"���� ������ (.lin)|*.lin|");
	dlg.m_ofn.lpstrTitle = "���� ���� ��������";
	
	if (dlg.DoModal() == IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&f,CArchive::store);
		m_pDataManage->GetLineInfoApp()->Serialize(ar);
		ar.Close();
		f.Close();
		
		CString szLineExe = ((CAPlateApp *)AfxGetApp())->GetFixedDir() + "\\ALine.exe " + "\"" + dlg.GetPathName() + "\"";				
		
		PROCESS_INFORMATION pi;
		// Set up the start up info struct.
		STARTUPINFO si;
		::ZeroMemory(&si, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;		
		// Use this if you want to show the child.
		si.wShowWindow = SW_SHOW;
		BOOL bResult = ::CreateProcess(NULL, szLineExe.GetBuffer(0), NULL, NULL, TRUE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if(bResult && pi.hProcess)
		{		
			m_WaitForALineDlg.m_bCancel = FALSE;
			if(!m_WaitForALineDlg.GetSafeHwnd()) m_WaitForALineDlg.Create(CWaitForSubProgram::IDD);
			m_WaitForALineDlg.CenterWindow();
			m_WaitForALineDlg.ShowWindow(SW_SHOW);
			
			DWORD dwExitCode;
			::GetExitCodeProcess(pi.hProcess, &dwExitCode);
			while(dwExitCode == STILL_ACTIVE)
			{
				MSG msg;	
				while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
				{
					AfxGetThread()->PumpMessage();							
				}	
				::GetExitCodeProcess(pi.hProcess, &dwExitCode);
				if(m_WaitForALineDlg.m_bCancel) break;
			}
			if(!m_WaitForALineDlg.m_bCancel) 
			{
				m_WaitForALineDlg.DestroyWindow();
				CString szMsg;
				szMsg.Format("ALine���� ������ %s ������ ���� ���Ͽ� �ݿ��Ͻðڽ��ϱ� ?", dlg.GetPathName());
				if(AfxMessageBox(szMsg, MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					CFile f;
					if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;					
					CArchive ar(&f,CArchive::load);
					m_pDataManage->GetLineInfoApp()->Serialize(ar);
					ar.Close();
					f.Close();
					
					CString csErrorMsg(_T(""));
					if(!m_pDataManage->GetLineInfoApp()->SetDataIp(csErrorMsg))
					{
						AfxMessageBox(csErrorMsg,MB_OK | MB_ICONEXCLAMATION);
					}
				}
			}
		}
	}
}


void CAPlateDoc::ImportAPierInfo(CArchive &ar)
{
	CPlateBridgeApp* pBridge = m_pDataManage->GetBridge();
	CLineInfoApp *pLineApp = (CLineInfoApp *)pBridge->m_pLine;	
	long nFlag=0;
	CString szBoxType;
	CHAR sInfo[30+1];		
	CDoubleArray m_GirderLengthArray;
	long nGirderSu;
	
	BOOL vBool;	
	double vDouble;
	long vLong;
	CString vString;
	BYTE vByte;
	CDPoint vDPoint;
	if(!ar.IsStoring())
	{
		m_pGenInputStd->GetDlgOfIDD(IDD_GEN_05_JIGANGUSENG)->SetDataDefault();		
		
		ar >> nFlag;
		ar.Read(sInfo,30);
		// ��������
		if(strstr(sInfo,"ABeam Export Info File")!=NULL)
		{
			AfxMessageBox("���� Export ������ ���� �� �����ϴ� !");
			return;
		}
		pLineApp->Serialize(ar);

		// ���� �ʱ�ȭ
		CString csErrorMsg(_T(""));
		if( pLineApp->SetDataIp(csErrorMsg) == FALSE )
		{
			if(csErrorMsg!="")	AfxMessageBox(csErrorMsg);						
			return;
		}

		// ��������
		double dUganStt=1000, dUganEnd=1000;
		if(nFlag>1)
		{
			CString szDummy;
			if(nFlag>12) 			
				ar >> vBool/*pLD->m_bRailDesign*/;							
			ar >> pBridge->m_sNameBridge;
			ar >> pBridge->m_sNameOwner;		// ����ó
			ar >> pBridge->m_sNameConstructor;	// �ð���		
			ar >> szDummy;						// ���ۻ�
			ar >> pBridge->m_sNameSite;			// �����
			ar >> pBridge->m_sNameEngineering;	// �����
			ar >> pBridge->m_dLengthUganStt;	// ���� ���� �Ÿ�
			ar >> pBridge->m_dLengthUganEnd;	// ���� ���� �Ÿ�
		}		
		if(nFlag>6)
		{
			ar >> szBoxType;
		}

		ar >> pBridge->m_dStationBridgeStt;		// ���� ������
		ar >> vDouble;	// ������ �ѱ���
		pBridge->m_dLengthTotalSlab = vDouble - pBridge->m_dLengthUganStt - pBridge->m_dLengthUganEnd;

		ar >> vByte; pBridge->m_nTypeAbutOrPier[0] = vByte; // ������ ���� ���� Ÿ��
		ar >> vByte; pBridge->m_nTypeAbutOrPier[1] = vByte; // ������ ���� ���� Ÿ��
		ar >> pBridge->m_nQtyJigan;			 // ������		

		ar >> vDouble; pBridge->m_dLengthGirderShoeStt = vDouble - pBridge->m_dLengthUganStt - pBridge->m_dLengthSlabGirderStt; //m_pDB->m_dLengthSlabGirderStt ���� ������
		ar >> vDouble; pBridge->m_dLengthGirderShoeEnd = vDouble - pBridge->m_dLengthUganEnd - pBridge->m_dLengthSlabGirderEnd; //m_pDB->m_dLengthSlabGirderEnd ���� ������
		long n = 0;
		for(n=0; n<pBridge->m_nQtyJigan; n++)	ar >> pBridge->m_dLengthJigan[n];	// ��������					
		for(n=0; n<pBridge->m_nQtyJigan+1; n++)	ar >> vDouble;	pBridge->m_vAngJijum[n] = ToDPointFrDegree(vDouble); // �������� ���� (DEGREE)
		for(n=0; n<pBridge->m_nQtyJigan+1; n++)	vString;	
			
		if(nFlag>0)
		{			
			for(n=0; n<pBridge->m_nQtyJigan+1; n++)
			{			
				ar >> pBridge->m_strJijumName[n];	// �������̸�				
			}
		}

		ar >> nGirderSu;
		pBridge->MakeGirder(nGirderSu);		
		pBridge->MakeSlab();
		ar >> pBridge->GetSlabLeft()->m_dGirderCenter;
		ar >> pBridge->GetSlabRight()->m_dGirderCenter;
		long nJ = 0;
		for(nJ=0; nJ<pBridge->m_nQtyJigan+1; nJ++)
		{
			long nShoe = 0;
			for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
			{
				ar >> pBridge->GetGirder(nG)->m_dGirderCenter;//pG->m_dGirderCenter;										// �Ŵ� �߽� ����
				ar >> vDouble;//pG->GetWidUp(pBx) + pG->m_dLeftSharp + pG->m_dRighSharp;		// �Ŵ� ��� �÷��� �ʺ�
				ar >> vDouble; pBridge->GetGirder(nG)->SetHeightGirderAtJijum(nJ,vDouble);//pG->GetHeightBox(pBx->GetStation()) + dLowerFlangeThick;	// �Ŵ� ����				
				ar >> vLong;//pG->m_nLineNum;
				// ������
				double dTest;
				ar >> dTest;//ShoeFir.GetDisShoeFrWeb();			// �Ŵ� ���������� ����
				ar >> vDouble;//pG->m_dLeftSharp;
				if(dTest != 0)
				{					
					ar >> vLong;//ShoeFir.m_nRefType;
					ar >> vDouble;//ShoeFir.GetWidthTotalSolePlate();		// ��ü ��
					ar >> vDouble;//ShoeFir.GetHeigthTotalSolePlate();	// ��ü ����					
					ar >> vDouble;//ShoeFir.m_dThick_Block;				// Block �β�
					ar >> vDouble;//ShoeFir.m_dW1_Mortal;					// ������� ���� ��
					ar >> vDouble;//ShoeFir.m_dW2_Mortal;					// ������� ���� ��
					ar >> vDouble;//ShoeFir.m_dThick_Mortar;				// Moltal �β�
					ar >> vDouble;//ShoeFir.m_dThick_Shoe;				// �� ����					
					ar >> vByte;//ShoeFir.m_cDirectionShoe;				// �� ����	
					ar >> vLong;//ShoeFir.m_dForceShoe;					// �� ���
					ar >> vDouble;//ShoeFir.m_dThick_SolePlate;			// ���÷���Ʈ �β�					
					ar >> vDPoint;//m_pDB->GetXyShoe(nShoe++,nJ);
				}
				ar >> dTest;			// �Ŵ� ���������� ����
				ar >> vDouble;
				if(dTest != 0)
				{				
					ar >> vLong;//ShoeNxt.m_nRefType;
					ar >> vDouble;//ShoeNxt.GetWidthTotalSolePlate();		// ��ü ��
					ar >> vDouble;//ShoeNxt.GetHeigthTotalSolePlate();	// ��ü ����					
					ar >> vDouble;//ShoeNxt.m_dThick_Block;				// Block �β�
					ar >> vDouble;//ShoeNxt.m_dW1_Mortal;					// ������� ���� ��
					ar >> vDouble;//ShoeNxt.m_dW2_Mortal;					// ������� ���� ��
					ar >> vDouble;//ShoeNxt.m_dThick_Mortar;				// Moltal �β�
					ar >> vDouble;//ShoeNxt.m_dThick_Shoe;				// �� ����					
					ar >> vByte;//ShoeNxt.m_cDirectionShoe;				// �� ����
					ar >> vLong;//ShoeNxt.m_dForceShoe;					// �� ���
					ar >> vDouble;//ShoeNxt.m_dThick_SolePlate;			// ���÷���Ʈ �β�
					ar >> vDPoint;//m_pDB->GetXyShoe(nShoe++,nJ);
				}
			}
		}	
		//������ġ ���⼳��
		ar >> pBridge->m_cDirShoe;
		// Ⱦ������						
		pBridge->m_dThickSlabLeft = 250.0;
		pBridge->m_dThickSlabRight= 250.0;
		pBridge->m_dWidthHunch = 50;
		pBridge->m_dThickSlabHunch=50;
		if(FALSE)	//=================================> ��������
			ar >> vDouble;//m_pDB->GetMortalProtectedRail();		// ��ȣ����
		else
			ar >> pBridge->m_dThickPave;//m_pDB->GetThickPaveDoro();			// �ƽ���
		ar >> pBridge->m_dThickSlabBase;				// ���� ������ �β�
		ar >> pBridge->m_dHunchDisForBaseThick;		// ���� ������ �β� ��ġ��		
		ar >> vLong; pBridge->SetQtyHDanNode(vLong);//m_pDB->GetQtyHDan();					// Ⱦ�ܳ���		
		ar >> pBridge->m_cLevelRoadCenterToSlabEdge;	// ������ ���������� ��������
		for(nJ=0; nJ<pBridge->m_nQtyJigan+1; nJ++) // ������ Ȯ�� ����
		{
			//CBasicIndex *pBx = m_pDB->GetGirder(0)->GetBxOnJijum(nJ);	
			ar >> vDouble;//m_pDB->GetGirder(0)->GetHeightTaper(pBx->GetStation());
		}
		for(n=0; n<pBridge->GetQtyHDanNode(); n++)
		{
			ar >> vDouble; pBridge->SetLengthHDanByLine(vDouble, n); //GetLengthHDanDom(pBx, n);					// Ⱦ�ܱ���
			ar >> vDouble;// m_pDB->GetSlopeRailByLine(n);					// ö�����ϰ�� Slop
			ar >> vLong; pBridge->SetValueTypeHDan(n, 0, vLong); // m_pDB->GetTypeLevelHDan(n);					// Ⱦ�� ����Ÿ��
			ar >> vLong; pBridge->SetValueTypeHDan(n, 2, vLong);//m_pDB->GetTypeVariableLengthHDan(n);			// ��ȭ���� ���翩��
			ar >> vLong; pBridge->SetValueTypeHDan(n, 1, vLong);
		}
		
		// ��ȣ�� ���� Flag 15
		CStringArray strArr;
		ar >> vLong; 
		strArr.SetSize(vLong);
		pBridge->MakeGuardFence(vLong,strArr);//m_pDB->GetGuardFenceSu();
		for(long nGuard=0; nGuard<pBridge->GetGuardWallSu(); nGuard++)
		{
			CCentSeparation *pCentSeparation = pBridge->GetGuardWall(nGuard);
			ar >> pCentSeparation->m_D1;			
			ar >> pCentSeparation->m_W1;			
			ar >> pCentSeparation->m_W2;			
			ar >> pCentSeparation->m_W3;			
			ar >> pCentSeparation->m_W4;			
			ar >> pCentSeparation->m_W5;			
			ar >> pCentSeparation->m_H1;			
			ar >> pCentSeparation->m_H2;			
			ar >> pCentSeparation->m_H3;			
		}

		m_pGenInputStd->GetDlgOfIDD(IDD_GEN_02_BASEINFO)->SetUsedFlag(TRUE);
		m_pGenInputStd->GetDlgOfIDD(IDD_GEN_03_HDAN)->SetUsedFlag(TRUE);				
		m_pGenInputStd->GetDlgOfIDD(IDD_GEN_05_JIGANGUSENG)->SetUsedFlag(TRUE);		
		m_pGenInputStd->GetDlgOfIDD(IDD_GEN_06_JIJUMANGLE)->SetUsedFlag(TRUE);
		m_pGenInputStd->GetDlgOfIDD(IDD_GEN_20_SLABJEWON)->SetUsedFlag(TRUE);	
		((CMainFrame*)AfxGetMainWnd())->SendMessage(WM_CHANGE_MENUDLG, ID_GNINPUT_1);
	}		
}

void CAPlateDoc::ExportAPierInfo(CArchive &ar)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	long nFlag=m_nFlagExportApierInfo;
	CHAR sInfo[30+1] = "APier Export Info File 1.0\x0a";
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar.Write(sInfo,30);
		// ��������
		m_pDataManage->GetLineInfoApp()->Serialize(ar);
		// ��������
		ar << (BOOL)FALSE;//pDB->IsRailBridgeType();
		ar << pDB->m_sNameBridge;			// [�⺻ �Է�]
		ar << pDB->m_sNameOwner;			// ����ó
		ar << pDB->m_sNameConstructor;	// �ð���		
		ar << pDB->m_sNameAssembler;		// ���ۻ�
		ar << pDB->m_sNameSite;			// �����
		ar << pDB->m_sNameEngineering;	// �����
		ar << pDB->m_dLengthUganStt;		// ���� ���� �Ÿ�
		ar << pDB->m_dLengthUganEnd;		// ���� ���� �Ÿ�		
		// �ڽ� ����
		ar << CString("APLATE");//m_pAData->m_pBaseChief->m_BaseEtcCtl.GetStrDesignBoxType();

		ar << pDB->m_dStationBridgeStt;	// ���� ������
		ar << pDB->m_dLengthTotalSlab + pDB->m_dLengthUganStt + pDB->m_dLengthUganEnd;	// ���� �ѱ���
		ar << (BYTE)pDB->m_nTypeAbutOrPier[0];	// ������ ���� ���� Ÿ��
		ar << (BYTE)pDB->m_nTypeAbutOrPier[1];	// ������ ���� ���� Ÿ��
		ar << pDB->m_nQtyJigan;									// ������
		ar << pDB->m_dLengthUganStt + pDB->m_dLengthSlabGirderStt + pDB->m_dLengthGirderShoeStt;	// ������ ����ġ
		ar << pDB->m_dLengthUganEnd + pDB->m_dLengthSlabGirderEnd + pDB->m_dLengthGirderShoeEnd;	// ������ ����ġ		
		
		long n  = 0;
		long nJ = 0;

		for(n=0; n<pDB->m_nQtyJigan; n++)	ar << pDB->m_dLengthJigan[n];						// ��������
		for(n=0; n<pDB->m_nQtyJigan+1; n++)		ar << pDB->m_vAngJijum[n].GetAngleDegree();			// �������� ���� (DEGREE)
		for(n=0; n<pDB->m_nQtyJigan+1; n++)		ar << pDB->m_strJijumName[n];						// ���� �̸�
		
		// �Ŵ��� ������
		ar << pDB->GetGirdersu();	
		// ������ �¿��� �Ÿ�����
		ar << pDB->GetGirder(0)->m_dGirderCenter - pDB->m_dWidthSlabLeft;								// ���� ������ ����
		ar << pDB->GetGirder(pDB->GetGirdersu()-1)->m_dGirderCenter + pDB->m_dWidthSlabRight;	// ���� ������ ����										
		for(nJ=0; nJ<pDB->m_nQtyJigan+1; nJ++)
		{
			long nShoe = 0;
			for(long nG=0; nG<pDB->GetGirdersu(); nG++)
			{
				CPlateGirderApp *pG = pDB->GetGirder(nG);
				CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
				double dUpperFlangeThick = pG->GetThickFlangeByBase(pBx->GetStation(), TRUE);
				double dLowerFlangeThick = pG->GetThickFlangeByBase(pBx->GetStation(), FALSE);
				double dWebHeight = pG->GetHeightGirderByStaAng(pBx->GetStation());
				if(pDB->IsTUGir())
				{
					dUpperFlangeThick	= pDB->m_BindConc.m_dHeight;
					dWebHeight	= pG->GetHeightGirderByStaAng(pBx->GetStation()) + pG->GetThickFlangeByBase(pBx->GetStation(), TRUE) - pDB->m_BindConc.m_dDis;
				}
				ar << pG->m_dGirderCenter;											// �Ŵ� �߽� ����
				ar << pG->GetMaxWidthFlange(TRUE);									// �Ŵ� ��� �÷��� �ʺ�
				ar << dWebHeight + dUpperFlangeThick + dLowerFlangeThick;			// �Ŵ� ����
				ar << pG->m_nLineNum;
				// ������
				CShoe ShoeFir = pG->m_Shoe[nJ];				
				ar << pG->GetWidthFlangeUpper(pBx)/2;//ShoeFir.GetDisShoeFrWeb();	// �Ŵ� ���������� ����
				ar << (double)0;//pG->m_dLeftSharp;
				//if(ShoeFir.GetDisShoeFrWeb() != 0)
				{					
					ar << ShoeFir.m_nRefType;
					ar << ShoeFir.GetWidthTotalSolePlate();		// ��ü ��
					ar << ShoeFir.GetHeigthTotalSolePlate();	// ��ü ����					
					ar << ShoeFir.m_dThick_Block;				// Block �β�
					ar << ShoeFir.m_dW1_Mortal;					// ������� ���� ��
					ar << ShoeFir.m_dW2_Mortal;					// ������� ���� ��
					ar << ShoeFir.m_dThick_Mortar;				// Mortar �β�
					ar << ShoeFir.m_dThick_Shoe;				// �� ����					
					ar << ShoeFir.m_cDirectionShoe;				// �� ����	
					ar << ShoeFir.m_dForceShoe;					// �� ���
					ar << ShoeFir.m_dThick_SolePlate;			// ���÷���Ʈ �β�					
					ar << pDB->GetXyShoe(nShoe++,nJ);
				}
				ar << (double)0;//ShoeNxt.GetDisShoeFrWeb();			// �Ŵ� ���������� ����
				ar << (double)0;//pG->m_dRighSharp;				
			}
		}			
		//������ġ ���⼳��
		ar << pDB->m_cDirShoe;
		// Ⱦ������						
		//if(pDB->IsRailBridgeType())
		//	ar << pDB->GetMortalProtectedRail();		// ��ȣ����
		//else
		double	dThickSlab	= pDB->m_dThickSlabBase;
		if(pDB->IsTUGir())	dThickSlab	= pDB->m_dThickSlabBase - pDB->m_BindConc.m_dDeep;
		ar << pDB->GetThickPave();						// �ƽ���
		ar << dThickSlab;								// ���� ������ �β�
		ar << pDB->m_dHunchDisForBaseThick;				// ���� ������ �β� ��ġ��		
		ar << pDB->GetQtyHDanNode();					// Ⱦ�ܳ���		
		ar << pDB->m_cLevelRoadCenterToSlabEdge;		// ������ ���������� ��������
		for(nJ=0; nJ<pDB->m_nQtyJigan+1; nJ++)			// ������ Ȯ�� ����
		{
			CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetBxOnJijum(nJ);	
			ar << pDB->GetGirder(0)->GetHeightTaper(pBx->GetStation());
		}		
		CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetBxOnJijum(0);
		for(n=0; n<pDB->GetQtyHDanNode(); n++)
		{
			ar << pDB->GetLengthHDanDom(pBx, n);				// Ⱦ�ܱ���
			ar << (double)0;//pDB->GetSlopeRailByLine(n);		// ö�����ϰ�� Slop
			ar << pDB->GetTypeLevelHDan(n);						// Ⱦ�� ����Ÿ��
			ar << pDB->GetTypeVariableLengthHDan(n);			// ��ȭ���� ���翩��
			ar << pDB->GetWallGuardTypeHDan(n);					// Flag 15
		}		
		// ��ȣ�� ���� Flag 15
		ar << pDB->GetGuardWallSu();
		for(long nGuard=0; nGuard<pDB->GetGuardWallSu(); nGuard++)
		{
			CCentSeparation *pCentSeparation = pDB->GetGuardWall(nGuard);
			ar << pCentSeparation->m_D1;			
			ar << pCentSeparation->m_W1;			
			ar << pCentSeparation->m_W2;			
			ar << pCentSeparation->m_W3;			
			ar << pCentSeparation->m_W4;			
			ar << pCentSeparation->m_W5;			
			ar << pCentSeparation->m_H1;			
			ar << pCentSeparation->m_H2;			
			ar << pCentSeparation->m_H3;			
		}
		for(long nG=-1; nG<pDB->GetGirdersu(); nG++)
		{			
			CPlateGirderApp *pGir = pDB->GetGirder(nG);

			double dGirderLen = pGir->GetSSS(OFF_END_GIRDER) - pGir->GetSSS(OFF_STT_GIRDER);
			ar << dGirderLen;

			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			ar << Finder.GetCountBx();			
			while(pBx)
			{			
				CPlateBasicIndex *pBxMatch =	nG<pDB->GetGirdersu()-1 ?  pGir->GetBxMatchByCrossBeam(pBx, pDB->GetGirder(nG+1)) : NULL;
				double dWidth = pGir->GetWidthFlangeUpper(pBx);
				double dCurSta = pBx->GetStation();
				CDPoint vAng = pBx->GetAngle();				
				CDPoint xyLeft = pGir->GetXyGirderDis(dCurSta, 0, vAng);
				CDPoint xyRight = pBxMatch ? pBxMatch->GetGirder()->GetXyGirderDis(pBxMatch->GetStation(),  0, pBxMatch->GetAngle()) : pGir->GetXyGirderDis(dCurSta,  0, vAng);					
				CDPoint xyMid = (xyLeft + xyRight) / 2;
				double dStaMatch;				
				pGir->GetLine()->GetStationMatchVerticalByXy(xyMid, dStaMatch);
				CDPoint vAngLine = pGir->GetLine()->GetAngleAzimuth(dStaMatch);
				CDPoint vAngCross = (xyLeft - xyRight).Unit();				
				if(xyLeft == xyRight)
					vAngCross = CDPoint(0,1);
				else
					vAngCross = vAngCross.RotateInv(vAngLine);				
				double dLen=0;
				CPlateBasicIndex *pBxPrev = Finder.GetBxFixPrev();
				if(pBxPrev)
				{
					double dPreSta = pBxPrev->GetStation();
					dLen = pDB->GetGirder(-1)->GetLine()->GetLengthByDis(dPreSta, dCurSta, 0);
				}
				ar << nG;				
				ar << dCurSta;
				ar << vAngCross;
				ar << xyLeft;
				ar << xyRight;
				ar << dLen;						
				// �÷���Ʈ ����				
				//CDesignCalc *pCalc = m_pDesignDataStd->m_pDesignCalc;
				//CNEMCalc* pNem = m_pEnvironment->m_pFB->m_pNemCalc;
				CFemModelingCalc *pFemModelingCalc = m_pAPlateOutCalcStd->GetFemModelingCalc();
				ar << toM(pGir->GetHeightGirderByStaAng(dCurSta)+pGir->GetThickFlangeByBase(dCurSta) + pGir->GetThickFactByStation(dCurSta, G_F_L));
				ar << toM(pFemModelingCalc->GetWidthSlab(nG,dCurSta));
				ar << toM(pGir->GetWidthFlangeLower(pBx));
				ar << toM(pGir->GetWidthFlangeUpper(pBx));
				ar << toM(pDB->m_dThickSlabBase);
				ar << toM(pDB->m_dThickSlabHunch);
				ar << toM(pGir->GetThickJewon(G_F_U, pBx));
				ar << toM(pGir->GetThickJewon(G_F_L, pBx));
				ar << toM(pGir->GetThickJewon(G_W, pBx));
				ar << (double)0;//toM(pGir->m_dULeftSharp);
				ar << (double)0;//toM(pGir->m_dURightSharp);
				ar << (double)0;//pBx->GetJRib(TRUE)->m_dWidthJRib;
				ar << (double)0;//pBx->GetJRib(TRUE)->m_Thick;
				ar << (long)0;//pBx->GetJRib(TRUE)->Getsu();					
				ar << (double)0;//pBx->GetJRib(FALSE)->m_dWidthJRib;
				ar << (double)0;//pBx->GetJRib(FALSE)->m_Thick;
				ar << (long)0;//pBx->GetJRib(FALSE)->Getsu();				
				// ���� m �� �Ŵ����� 
				//CFMoldData*	 pFBri = m_pEnvironment->m_pFB->m_pFMoldData;
				//CFemDataEx*	 pData = pFBri->m_pFDArr[1];  // �ռ��� ������

				// ������
				double dBonusRate = m_pDataManage->GetCalcData()->m_BonusRate;
				ar << dBonusRate;

				pFemModelingCalc->GetSectionCoefficient(pBx);
				double As = toM2(pFemModelingCalc->GetMapValueSecCo("As_SUM")); // ��ȿ�� ������ As
				double dLoad = As * tokNPM3(m_pDataManage->GetCalcData()->DESIGN_UNIT_WEIGHT.m_dGangjae) * dBonusRate; 
				ar << dLoad;
				// ���� m �� ����
				double dWidthAsphalt = pDB->GetWidthSlabDom(pBx);
				for(n=0; n<pDB->GetQtyHDanNode(); n++)
				{
					if(pDB->GetValueTypeHDan(n, 1)==0) continue;
					dWidthAsphalt -= pDB->GetLengthHDanDom(pBx, n);
				}
				//if(pDB->IsRailBridgeType())
				//	dLoad = toM(pDB->GetMortalProtectedRail())*tokNPM3(m_pAData->m_pBaseChief->m_BaseEtcCtl.m_UWeightMugunConcrete)*toM(dWidthAsphalt) / pDB->GetGirdersu();
				//else
				dLoad = toM(pDB->GetThickPave())*tokNPM3(m_pDataManage->GetCalcData()->DESIGN_UNIT_WEIGHT.m_dAsphalt)*toM(dWidthAsphalt) / pDB->GetGirdersu();
				/*
				if(pDB->IsRailBridgeType())
				{					
					long nQtyHDan = pDB->GetQtyHDan();
					for(long n = 0;n < nQtyHDan;n++)
					{
						long nType = pDB->GetGuardFenceType(n);
						if(nType==HT_VERDOSANG || nType==HT_GRAVELDOSANG)
						{
							CDPoint A[6];
							pDB->GetXyDosangDom(pDB->GetGirder(0)->GetBxOnJijum(0), n, A);					
							double dWidthBottom = toM(fabs(A[0].x - A[5].x));
							double dWidthTop = toM(fabs(A[2].x - A[3].x));
							double dHeight = toM(pDB->GetHeightDosangAvg(pDB->GetGirder(0)->GetBxOnJijum(0), n));
							double dArea = (dWidthTop + dWidthBottom) * dHeight * 1./2.;
							double dUnit = nType==HT_VERDOSANG ? tokNPM3(m_pAData->m_pBaseChief->m_BaseEtcCtl.m_UWeightGravel) :  tokNPM3(m_pAData->m_pBaseChief->m_BaseEtcCtl.m_UWeightDosang);
							dLoad += (dArea * dUnit) / pDB->GetGirdersu();
						}
					}
				}
				*/
				ar << dLoad;

				// ���� m �� ��ȣ��			
				dLoad = 0;
				for(n=0; n<pDB->GetGuardWallSu(); n++)
				{
					CCentSeparation *pGuard = pDB->GetGuardWall(n);
					pGuard->InitCalculate(tokNPM3(m_pDataManage->GetCalcData()->DESIGN_UNIT_WEIGHT.m_dChulgun));
					dLoad += pGuard->GetArea()/1000000.0 * tokNPM3(m_pDataManage->GetCalcData()->DESIGN_UNIT_WEIGHT.m_dChulgun);					
				}
				//CARailFloorData *pFD  = m_pDesignDataStd->GetRailData()->GetRailFloorData(CENTER_FLOOR);
				struct CCalcData::_DESIGN_FLOOR_DATA FDL = m_pDataManage->GetCalcData()->DESIGN_FLOOR_DATA[FLOOR_LEFT];
				struct CCalcData::_DESIGN_FLOOR_DATA FDR = m_pDataManage->GetCalcData()->DESIGN_FLOOR_DATA[FLOOR_RIGHT];
				if(FDL.m_bBangEm) dLoad += FDL.m_dBangEmWeight;
				if(FDR.m_bBangEm) dLoad += FDR.m_dBangEmWeight;
				if(FDL.m_bWalkLoad) 
				{
					double dSttLen = 0, dEndLen = 0;
					for(long nHDan=0; nHDan<pDB->GetQtyHDanNode(); nHDan++)
					{
						long nType	= pDB->GetValueTypeHDan(nHDan, 1);      // m_nHDanValue[������][1=����]
						double dLen = toM(pDB->GetLengthHDanDom(pBx, nHDan));
						dEndLen += dLen;

						if(nType==3)
						{	
							BOOL bLeft = TRUE;							
							double dWidth = toM(pDB->GetWidthSlabDom(pBx));
							if(dSttLen < dWidth/2)
							{
								dLoad += (FDL.m_dWalkLoad1 + FDL.m_dWalkLoad2) * toM(dEndLen - dSttLen) * 1./2.;
							}							
							dSttLen += dLen;
						}											
					}
				}
				if(FDR.m_bWalkLoad) 
				{
					double dSttLen = 0, dEndLen = 0;
					for(long nHDan=0; nHDan<pDB->GetQtyHDanNode(); nHDan++)
					{
						long nType	= pDB->GetValueTypeHDan(nHDan, 1);      // m_nHDanValue[������][1=����]
						double dLen = toM(pDB->GetLengthHDanDom(pBx, nHDan));
						dEndLen += dLen;

						if(nType==3)
						{	
							BOOL bLeft = TRUE;							
							double dWidth = toM(pDB->GetWidthSlabDom(pBx));
							if(dSttLen > dWidth/2)
							{								
								dLoad += (FDR.m_dWalkLoad1 + FDR.m_dWalkLoad2) * toM(dEndLen - dSttLen) * 1./2.;
							}							
							dSttLen += dLen;
						}											
					}					
				}
				if(m_pDataManage->GetCalcData()->m_bExtraLoad)
				{				
					for(long m = 0; m < m_pDataManage->GetCalcData()->m_ELoadTitleArr.GetSize(); m++)
					{
						double LoadWeight = m_pDataManage->GetCalcData()->m_ELoadWeightArr[m];
						if(LoadWeight>0) dLoad += LoadWeight; 
					}
				}
				dLoad /= pDB->GetGirdersu();				
				ar << dLoad;				
				// ���κ� ����				
				CPlateCrossBeam *pCr = pBx->GetCrossBeam();
				if(pCr && nG!=pDB->GetGirdersu()-1)
				{										
					ar << TRUE; // ���κ� ����
					ar << toM(pCr->m_H_CR);
					ar << toM(pCr->m_dM_CR * 2);
					ar << toM(pCr->m_uM_CR * 2);
					ar << toM(pCr->m_T2_CR);
					ar << toM(pCr->m_T1_CR);
					ar << toM(pCr->m_T3_CR);
					//CMap<CString, LPCSTR, double, double> Map;
					pFemModelingCalc->GetSectionCoefficient(pBx);
					double dA = pFemModelingCalc->GetMapValueSecCoCR("As_SUM");
					double dLoad = toM2(dA) * tokNPM3(m_pDataManage->GetCalcData()->DESIGN_UNIT_WEIGHT.m_dGangjae) * dBonusRate; 
					ar << dLoad;
				}
				else
				{
					ar << FALSE;
				}				
				pBx = Finder.GetBxNext();
			}			
		}
				
		// ����� ���� ���ݷ� ��������
		// ��� ���������� �˻�			
		ar << m_pDataManage->GetCalcData()->m_bReadFrameModelingOutput;
		if( m_pDataManage->GetCalcData()->m_bReadFrameModelingOutput )
		{			
			SapReadReaction(ar);
		}							
	}
}

void CAPlateDoc::SapReadReaction(CArchive &ar)
{
	CPlateBridgeApp		*pDB = m_pDataManage->GetBridge();
	CFemModelingCalc	*pFemModelingCalc = m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc		*pStessCalc = m_pAPlateOutCalcStd->GetFemStressCalc();
	CFEMManage		*pFEMManage = m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	
	if(ar.IsStoring())
	{
		long nGirderSu = pDB->GetGirdersu ();
		nGirderSu = max (nGirderSu, 2);

		BOOL bHapSung = TRUE;//!m_pEnvironment->m_NumHaiseokCombo;
		
		ar << nGirderSu;
		ar << bHapSung;
		for (long nLoadGir = 0; nLoadGir < nGirderSu; nLoadGir++)   
		{
			/* �̷���� ����
			if (pDB->GetGirdersu () == 1)   
			{
				BOOL bExist;	
				pFemModelingCalc->
				pDesignCalc->GetLiveLoadReaction (1, 1, 1, 0, bExist);   // �Ŵ�1���϶� �¿��� ���� Case���� ����				
			}
			*/
			long nLoadTypesu = 4;
			if (nLoadGir == nGirderSu - 1)	nLoadTypesu = 6;
			ar << nLoadTypesu;
			for (long nLoadType = 1; nLoadType <= nLoadTypesu; nLoadType++)   
			{			
				BOOL bExist;
				pFemModelingCalc->GetLiveLoadReaction(nLoadType, nLoadGir, 0, 0, bExist);   // �ش� LC �ִ��� ����.
				ar << bExist;
				if (!bExist)   
				{
					pFemModelingCalc->GetLiveLoadReaction(nLoadType, nLoadGir, 1, 0, bExist);   // �ش� LC �ִ��� ����.
					ar << bExist;
					if (!bExist) continue;
				}
				ar << nLoadType;
				
				long nMaxLanesu = 20;
				ar << nMaxLanesu;
				for (long nLane = 0; nLane <= nMaxLanesu; nLane++)   
				{
					pFemModelingCalc->GetLiveLoadReaction(nLoadType, nLoadGir, nLane, 0, bExist);   // �ش� ���� �ִ��� ����.
					ar << bExist;
					if (!bExist) continue;
					//if (nLoadType <= 2 && nLane > 0)   
					//{
						//ar << nLane;
				
					//}
					ar << pDB->m_nQtyJigan;
					for (long nK = 0; nK <= pDB->m_nQtyJigan; nK++)   
					{																	
						ar << pFEMManage->GetJointSize();						
						for (int i = pFEMManage->GetJointSize()-1; i >=0; i--)   
						{
							CJoint *pJ = pFEMManage->GetJoint(i);
							ar << (BYTE)pJ->m_UZ;
							ar << m_pCalcStd->GetBxByJoint(pJ)->GetNumberJijum();
							ar << (BYTE)0;//pFemData->m_vJoint[i].m_bMain;
							if (!(pJ->m_UZ) || m_pCalcStd->GetBxByJoint(pJ)->GetNumberJijum() != nK) continue;
							//if (pFemData->m_vJoint[i].m_bMain != 5)   
							{
								ar << pJ->m_LongAttrib[JOINT_GIR] + 1;
							}							

							double dValue, dValue2, dSumMax = 0.0, dSumMin = 0.0;
							 
							//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_PREV_S, i, 'Z', bExist)/10;
							dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSteel[JOINT_REACTION].z,3)/10;
							dSumMax += dValue;
							dSumMin += dValue;
							if(pDB->IsTUContinuousSpan())
							{
								dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadConf2[JOINT_REACTION].z,3)/10;
								dSumMax += dValue;
								dSumMin += dValue;
								dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSteelConf[JOINT_REACTION].z,3)/10;
								dSumMax += dValue;
								dSumMin += dValue;
							}
							ar << dValue;
														
							//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_PREV_C, i, 'Z', bExist)/10;
							if(pDB->IsTUGir())
							{
								dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSlab[JOINT_REACTION].z,3)/10;
								dValue += Round(m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadConf[JOINT_REACTION].z,3)/10;
								if(!pDB->IsTUContinuousSpan())
									dValue += Round(m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadTurning[JOINT_REACTION].z,3)/10;
							}
							else
							{
								dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSlab[JOINT_REACTION].z,3)/10;
							}
							
							dSumMax += dValue;
							dSumMin += dValue;
							ar << dValue;

							//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_NEXT, i, 'Z', bExist)/10;
							dValue = Round(m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPostDead[JOINT_REACTION].z,3)/10;
							dSumMax += dValue;
							dSumMin += dValue;
							ar << dValue;
							
							dValue = pFemModelingCalc->GetLiveLoadReaction(nLoadType, nLoadGir, nLane, i, bExist)/10;
							ar << dValue;

							dValue2 = pFemModelingCalc->GetLiveLoadReaction(-nLoadType, nLoadGir, nLane, i, bExist)/10;
							ar << dValue2;

							if(nLoadType==5)
							{
								double dValueExt;
								dValueExt = pFemModelingCalc->GetLiveLoadReaction(9, nLoadGir, nLane, i, bExist)/10;
								ar << dValueExt;
								dValueExt = pFemModelingCalc->GetLiveLoadReaction(10, nLoadGir, nLane, i, bExist)/10;
								ar << dValueExt;
							}

							dSumMax += max(dValue, dValue2);
							dSumMin += min(dValue, dValue2);

							//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_DISP_MAX, i, 'Z', bExist)/10;
							dValue = Round(m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vUnSettleMax[JOINT_REACTION].z,3)/10;
							dSumMax += dValue;
							ar << dValue;

							//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_DISP_MIN, i, 'Z', bExist)/10;
							dValue = Round(m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vUnSettleMin[JOINT_REACTION].z,3)/10;
							dSumMin += dValue;
							ar << dValue;

							ar << dSumMax;
							ar << dSumMin;
							
							ar << i;							
						}						
					}
					if (nLoadType >= 3)  // ��ũ�� Ʈ���Ϸ��� ������ ����.
						break;
				}
			}
		}

		for (long nK = 0; nK <= pFEMManage->GetJointSize(); nK++)   
		{				
			long nJointSize	= pFEMManage->GetJointSize();
			ar << nJointSize;
			for (int i = pFEMManage->GetJointSize()-1; i >=0 ; i--)   
			{
				CJoint *pJ = pFEMManage->GetJoint(i);
				long nKK	= m_pCalcStd->GetBxByJoint(pJ)->GetNumberJijum();
				ar << (BYTE)pJ->m_UZ;
				ar << nKK;
				ar << (BYTE)0;//pFemData->m_vJoint[i].m_bMain;
				if (!(pJ->m_UZ) || nKK != nK) continue;																		
				CString sType = "";
				//if (pFemData->m_vJoint[i].m_bMain != 5)   
				{
					ar << pJ->m_LongAttrib[JOINT_GIR] + 1;
				}
				
				double dValue;
				//BOOL bExist;				

				double dSumMax = 0;
				double dSumMin = 0;
				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_PREV_S, i, 'Z', bExist)/10;
				dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSteel[JOINT_REACTION].z,3)/10;
				dSumMax += dValue;
				dSumMin += dValue;
				if(pDB->IsTUContinuousSpan())
				{
					dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadConf2[JOINT_REACTION].z,3)/10;
					dSumMax += dValue;
					dSumMin += dValue;
					dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSteelConf[JOINT_REACTION].z,3)/10;
					dSumMax += dValue;
					dSumMin += dValue;
				}
				ar << dValue;
				
				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_PREV_C, i, 'Z', bExist);
				if(pDB->IsTUGir())
				{
					dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSlab[JOINT_REACTION].z,3)/10;
					dValue += Round(m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadConf[JOINT_REACTION].z,3)/10;
					dValue += Round(m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadTurning[JOINT_REACTION].z,3)/10;
				}
				else
				{
					dValue = Round(m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPreDeadSlab[JOINT_REACTION].z,3)/10;
				}
				dSumMax += dValue;
				dSumMin += dValue;
				ar << dValue;

				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_NEXT, i, 'Z', bExist)/10;
				dValue = Round(m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vPostDead[JOINT_REACTION].z,3)/10;
				dSumMax += dValue;
				dSumMin += dValue;
				ar << dValue;

				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_LIVE_MAX, i, 'Z', bExist)/10;
				dValue = Round(m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vLiveMax[JOINT_REACTION].z,3)/10;
				dSumMax += dValue;				
				ar << dValue;

				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_LIVE_MIN, i, 'Z', bExist)/10;
				dValue = Round(m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vLiveMin[JOINT_REACTION].z,3)/10;				
				dSumMin += dValue;
				ar << dValue;

				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_DISP_MAX, i, 'Z', bExist)/10;
				dValue = Round(m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vUnSettleMax[JOINT_REACTION].z,3)/10;
				dSumMax += dValue;				
				ar << dValue;

				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_DISP_MIN, i, 'Z', bExist)/10;
				dValue = Round(m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(i)->m_vUnSettleMin[JOINT_REACTION].z,3)/10;				
				dSumMin += dValue;
				ar << dValue;

				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_SUM_MAX, i, 'Z', bExist)/10;
				ar << dSumMax;

				//dValue = pStessCalc->GetOneNodForce(NOD_REACTION, LC_SUM_MIN, i, 'Z', bExist)/10;
				ar << dSumMin;
				
				ar << i;				
			}			
		}		
	}
}

long _nTransIndex=0;
BOOL CAPlateDoc::TransBrtString(const CString &str, long &nStep)
{
	CPlateBridgeApp* pBridge		= m_pDataManage->GetBridge();
	CCalcData		*pCalcData		= m_pDataManage->GetCalcData();

	long nUnitType=0;
	char strBuff[10000];
	double dTemp, dTemp2;
	long nTemp, nTemp2, nTemp3, n;
	CStringArray szTest;
	try 
	{	
		switch(nStep)
		{
		case 0:	_SCANF_S(str, "%d", &nBrx_Version); ++nStep; break;
		case 1: nUnitType = str=="SI" ? 1 : 0; ++nStep; break;
		case 2:	pBridge->m_sNameBridge = str; ++nStep; break;
		case 3:	pBridge->m_sNameOwner = str; ++nStep; break;
		case 4:	pBridge->m_sNameConstructor = str; ++nStep; break;
		case 5:	pBridge->m_sNameAssembler = str; ++nStep; break;
		case 6:	pBridge->m_sNameSite = str; ++nStep; break;
		case 7:	pBridge->m_sNameEngineering = str; ++nStep; break;
		case 8:	_SCANF_S(str, "%lf %lf %lf %lf %lf %lf %lf %lf", &pBridge->m_dStationBridgeStt, &pBridge->m_dLengthTotalSlab, &pBridge->m_dLengthUganStt, &pBridge->m_dLengthUganEnd,
					   &pBridge->m_dLengthGirderShoeStt, &pBridge->m_dLengthGirderShoeEnd, &pBridge->m_dLengthSlabGirderStt, &pBridge->m_dLengthSlabGirderEnd); ++nStep; break;			
		case 9: _SCANF_S(str, "%u %u", &pBridge->m_nTypeAbutOrPier[0], &pBridge->m_nTypeAbutOrPier[1]); ++nStep; break;
		case 10: _SCANF_S(str, "%d", &pBridge->m_nQtyJigan); _nTransIndex = 0; ++nStep; break;				
		case 11: _SCANF_S(str, "%lf", &pBridge->m_dLengthJigan[_nTransIndex++]); if(_nTransIndex==pBridge->m_nQtyJigan) { _nTransIndex=0; ++nStep; } break;
		case 12: _SCANF_S(str, "%s %lf", _SCANF_PARAM_S(&strBuff, sizeof(strBuff)), &dTemp); 
				pBridge->m_strJijumName[_nTransIndex] = strBuff;
				pBridge->m_vAngJijum[_nTransIndex] = ToDPointFrDegree(dTemp);
				if(++_nTransIndex==pBridge->m_nQtyJigan+1) ++nStep; 
				break;
		case 13: _SCANF_S(str, "%d", &nTemp); pBridge->MakeGirder(nTemp); _nTransIndex = 0; ++nStep; break;
		case 14: _SCANF_S(str, "%lf %lf %lf %d", &pBridge->GetGirder(_nTransIndex)->m_dGirderCenter,&dTemp,&dTemp2,&pBridge->GetGirder(_nTransIndex)->m_nLineNum); 			
			//pBridge->GetGirder(_nTransIndex)->SetWidBaseLower(dTemp);
			//pBridge->GetGirder(_nTransIndex)->SetWidBaseUpperLeft(dTemp/2);
			//pBridge->GetGirder(_nTransIndex)->SetWidBaseUpperRigh(dTemp/2);
			if(++_nTransIndex==pBridge->GetGirdersu()) ++nStep; 
			break; //���� ���̴� �����ϱ�� �Ѵ�.
		case 15: pBridge->MakeSlab(); _SCANF_S(str, "%lf %lf",&pBridge->GetSlabLeft()->m_dGirderCenter,&pBridge->GetSlabRight()->m_dGirderCenter); 			
			++nStep;
			break;		
		case 16:
			_SCANF_S(str, "%lf %lf %lf %lf %lf %lf %lf", &pBridge->m_dThickSlabLeft, &pBridge->m_dThickSlabRight, &pBridge->m_dThickSlabBase,
					&pBridge->m_dHunchDisForBaseThick, &pBridge->m_dHunchDisForBaseThick, &pBridge->m_dThickSlabHunch, &pBridge->m_dThickPave);						
			++nStep;
			break;
		case 17: _SCANF_S(str, "%d", &pBridge->m_cLevelRoadCenterToSlabEdge); ++nStep; break;
		case 18: _SCANF_S(str, "%d", &nTemp); pBridge->SetQtyHDanNode(nTemp); _nTransIndex=0;  ++nStep; break;
		case 19: _SCANF_S(str, "%lf %d %d %d", &dTemp, &nTemp, &nTemp2, &nTemp3); 
			pBridge->SetLengthHDanByLine(dTemp,_nTransIndex);			
			pBridge->SetValueTypeHDan(_nTransIndex, 1,nTemp);
			pBridge->SetValueTypeHDan(_nTransIndex, 0,nTemp2);
			pBridge->SetValueTypeHDan(_nTransIndex, 2,nTemp3);
			if(++_nTransIndex == pBridge->GetQtyHDanNode()) ++nStep;
			break;
		case 20: _SCANF_S(str, "%d", &nTemp); 
			for(n=0; n<nTemp; n++) szTest.Add("");
			pBridge->MakeGuardFence(nTemp,szTest);
			_nTransIndex=0; ++nStep; 
			break;
		case 21:
			_SCANF_S(str, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &pBridge->GetGuardWall(_nTransIndex)->m_D1, &pBridge->GetGuardWall(_nTransIndex)->m_W1, &pBridge->GetGuardWall(_nTransIndex)->m_W2,
				&pBridge->GetGuardWall(_nTransIndex)->m_W3, &pBridge->GetGuardWall(_nTransIndex)->m_W4, &pBridge->GetGuardWall(_nTransIndex)->m_W5, &pBridge->GetGuardWall(_nTransIndex)->m_H1,
				&pBridge->GetGuardWall(_nTransIndex)->m_H2, &pBridge->GetGuardWall(_nTransIndex)->m_H3);
			if(++_nTransIndex == pBridge->GetGuardWallSu()) ++nStep;
			break;
		case 22: pBridge->m_sDeungeub = str; ++nStep; break;
		case 23: _SCANF_S(str, "%lf %lf", &pCalcData->m_dSpeed, &pCalcData->CALC_CANTILEVER_DATA[2].m_Cent_R); ++nStep; break;
		case 24: _SCANF_S(str, "%d %lf %lf %lf", &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight);  ++nStep; 
			if(nUnitType==0)	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight *=10*1000;
			break;
		case 25: _SCANF_S(str, "%d %lf %lf %lf", &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight);  ++nStep; 
			if(nUnitType==0)	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight *=10*1000;
			break;
		case 26: _SCANF_S(str, "%d %lf %lf %lf", &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis);  ++nStep; 
			if(nUnitType==0)	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight *=10*1000;
			break;
		case 27: _SCANF_S(str, "%d %lf %lf %lf", &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanHeight, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis);  ++nStep; 
			if(nUnitType==0)	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanHeight *=10*1000;
			break;
		case 28: _SCANF_S(str, "%d %lf %lf %lf %lf %lf", &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH, &pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_HoriF, &pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_VerF);  ++nStep; 
			if(nUnitType==0)	{ pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1 *=.01; pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2 *=.01; pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_HoriF *= 10; pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_VerF *= 10; }
			break;
		case 29: _SCANF_S(str, "%d %lf %lf %lf %lf %lf", &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH, &pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_HoriF, &pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_VerF);  ++nStep; 
			if(nUnitType==0)	{ pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1 *=.01; pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2 *=.01;  pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_HoriF *= 10; pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_VerF *= 10; }
			break;
		case 30: _SCANF_S(str, "%d %d %d %d %d %d", &pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bPeopleLoad, &pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bPeopleLoad, &pCalcData->m_bTank, &pCalcData->m_bTrailer, &pCalcData->m_bMugun, &pCalcData->m_bJungbunMugun);  ++nStep; break;
		case 31: _SCANF_S(str, "%d %d", &pCalcData->m_bExtraLoad, &nTemp); _nTransIndex=0; pCalcData->m_ELoadTitleArr.SetSize(nTemp); pCalcData->m_ELoadDisArr.SetSize(nTemp); pCalcData->m_ELoadWeightArr.SetSize(nTemp); ++nStep; if(nTemp==0) ++nStep; 
			
			break;
		case 32: _SCANF_S(str, "%s %lf %lf", &strBuff, &dTemp, &dTemp2);
				 if(nUnitType==0)	{ dTemp2 *= 10*1000; }
				 pCalcData->m_ELoadTitleArr.SetAt(_nTransIndex,strBuff);
				 pCalcData->m_ELoadDisArr.SetAt(_nTransIndex,dTemp);
				 pCalcData->m_ELoadWeightArr.SetAt(_nTransIndex,dTemp2);
				 if(++_nTransIndex == pCalcData->m_ELoadTitleArr.GetSize()) ++nStep;				 
				 break;
		case 33: _SCANF_S(str, "%d %d %lf %lf %d %d", &pCalcData->m_HLType, &pCalcData->m_nHLiveLoadType, &pCalcData->m_dUserLaneWidth, &pCalcData->m_DBFromEdge, &pCalcData->m_bNoDistOnNoGuard, &pCalcData->m_bWind); ++nStep; break;
		case 34: _SCANF_S(str, "%d", &pCalcData->m_AddLoad); ++nStep; break;			
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAPlateDoc::ImportBridgeInfo(CArchive &ar)
{
	CString szBuff;
	long nStep=0;	
	if(!ar.IsStoring())
	{
		while(TRUE)
		{		
			if(!ar.ReadString(szBuff)) break;
			szBuff.TrimLeft(); szBuff.TrimRight();
			if(szBuff=="END") break;
			if(!szBuff.IsEmpty() && szBuff[0]==';') continue;
			if(!TransBrtString(szBuff, nStep))
			{
				AfxMessageBox("�� �������� ����Ÿ�� �����մϴ� !",MB_ICONERROR);
				return FALSE;
			}			
		}
	}
	return TRUE;
}

void CAPlateDoc::ExportBridgeInfo(ofstream &out, CString szFileName)
{
	CPlateBridgeApp* pBridge		= m_pDataManage->GetBridge();
	CCalcData		*pCalcData		= m_pDataManage->GetCalcData();
	
	long nVersion=0;
	out << "; ARoad �������� - FileName : " << (LPCSTR)szFileName << "\n";
	out << "; ����\n";
	out << nVersion << "\n";
	out << "; ����\n";
	out << "SI\n";
	out << "; ���� �⺻���� (������, ����ó, �ð���, ���ۻ�, �����, �����)\n";	
	out << (LPCSTR)pBridge->m_sNameBridge << "\n";			// [�⺻ �Է�]
	out << (LPCSTR)pBridge->m_sNameOwner << "\n";				// ����ó
	out << (LPCSTR)pBridge->m_sNameConstructor << "\n";		// �ð���		
	out << (LPCSTR)pBridge->m_sNameAssembler << "\n";			// ���ۻ�
	out << (LPCSTR)pBridge->m_sNameSite << "\n";				// �����
	out << (LPCSTR)pBridge->m_sNameEngineering << "\n";		// �����
	out << "; ���� ��ġ����\n; ���� Station, ������ ����, ������ ����, �������� ����ġ, ������������� �Ŵ����ܰ��� �Ÿ� \n";
	out << pBridge->m_dStationBridgeStt << " " << pBridge->m_dLengthTotalSlab << " " << pBridge->m_dLengthUganStt << " " << pBridge->m_dLengthUganEnd;
	out << " " << pBridge->m_dLengthGirderShoeStt << " " << pBridge->m_dLengthGirderShoeEnd << " " << pBridge->m_dLengthSlabGirderStt << " " << pBridge->m_dLengthSlabGirderEnd << "\n";
	out << "; ������ ���� ���� ���� 0 : ����, 1 : ����\n";
	out << pBridge->m_nTypeAbutOrPier[0] << " " << pBridge->m_nTypeAbutOrPier[1] << "\n";
	out << "; ������ �� ��������\n";
	out << pBridge->m_nQtyJigan << "\n";	

	long n = 0;
	for(n=0; n<pBridge->m_nQtyJigan; n++) out << pBridge->m_dLengthJigan[n] << "\n";
	out << "; �������� - �����̸�, ��������\n";	
	for(n=0; n<pBridge->m_nQtyJigan+1; n++) 
	{
		CDPoint vAngJijum = pBridge->m_vAngJijum[n];
		out << (LPCSTR)pBridge->m_strJijumName[n] << " " << vAngJijum.GetAngleDegree() << "\n";
	}
	out << "; �Ŵ�����\n; �Ŵ�����\n";
	out << pBridge->GetGirdersu() << "\n";
	
	out << "; �Ŵ����� - �����߽ɿ��� �Ÿ�, �Ŵ������, �Ŵ�����, ������ȣ\n";
	for(n=0; n<pBridge->GetGirdersu(); n++)
	{
		CPlateGirderApp *pG = pBridge->GetGirder(n);
		out << pG->m_dGirderCenter << " " << pG->GetWidthFlangeUpper(pG->GetBxOnJijum(0)) << " " << pG->GetHeightGirderByStaAng(pG->GetBxOnJijum(0)->GetStation(), 0, CDPoint(0,1), FALSE) << " " << pG->m_nLineNum << "\n";
	}
	out << "; ������ ����\n; ������ �¿��� �Ÿ�\n";
	out << pBridge->GetSlabLeft()->m_dGirderCenter << " " << pBridge->GetSlabRight()->m_dGirderCenter << "\n";
	out << "; ������ ��,���� ����, ���� ������ �β�, ������ �������, ���صβ� ��ġ, ��ġ����, �ƽ��� �β�\n";
	out << pBridge->m_dThickSlabLeft << " " << pBridge->m_dThickSlabRight << " " << pBridge->m_dThickSlabBase << " ";
	out << pBridge->m_dHunchDisForBaseThick << " " << pBridge->m_dHunchDisForBaseThick << " " << pBridge->m_dThickSlabHunch << " " << pBridge->m_dThickPave << "\n";
	out << "; �����߽ɰ��� ��������\n";
	out << (long)pBridge->m_cLevelRoadCenterToSlabEdge << "\n";
	
	out << "; Ⱦ�� ����\n; Ⱦ�ܳ���\n";
	out << pBridge->GetQtyHDanNode() << "\n";					// Ⱦ�ܳ���		
	out << "; Ⱦ�� ��庰 ���� - Ⱦ�ܱ���, ��ȣ�� Ÿ��, Ⱦ�� ����Ÿ��, ��ȭ���� ���翩��\n";	
	for(n=0; n<pBridge->GetQtyHDanNode(); n++)	
		out << pBridge->GetLengthHDanDom(pBridge->GetGirder(0)->GetBxOnJijum(0), n) << " " << pBridge->GetWallGuardTypeHDan(n) << " " << pBridge->GetTypeLevelHDan(n) << " " << pBridge->GetTypeVariableLengthHDan(n) << "\n";			
	out << "; ��ȣ�� ����\n; ��ȣ����\n";
	out << pBridge->GetGuardWallSu() << "\n";
	out << "; ��ȣ�� ����Ÿ - D1, W1, W2, W3, W4, W5, H1, H2, H3\n";
	for(long nGuard=0; nGuard<pBridge->GetGuardWallSu(); nGuard++)
	{
		CCentSeparation *pCentSeparation = pBridge->GetGuardWall(nGuard);
		out << pCentSeparation->m_D1 << " " << pCentSeparation->m_W1 << " " << pCentSeparation->m_W2 << " " << pCentSeparation->m_W3 << " ";
		out << pCentSeparation->m_W4 << " " << pCentSeparation->m_W5 << " " << pCentSeparation->m_H1 << " " << pCentSeparation->m_H2 << " " << pCentSeparation->m_H3 << "\n";
	}		
	out << "; ������ ����\n; ������ ���\n";
	out << (LPCSTR) pBridge->m_sDeungeub << "\n";
	out << "; ����ӵ�, �����߽ɹݰ�\n";
	out << pCalcData->m_dSpeed << " " << pCalcData->CALC_CANTILEVER_DATA[2].m_Cent_R << "\n";
	out << "; ���� ������ ���� - ���翩��, �����߷�, ����, �Ÿ�\n";
	out << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis << "\n";
	out << "; ���� ������ ���� - ���翩��, �����߷�, ����, �Ÿ�\n";
	out << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmHeight << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis << "\n";
	out << "; ���� ���� ���� - ���翩��, �����߷�, ����, �Ÿ�\n";
	out << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanHeight << "\n";
	out << "; ���� ���� ���� - ���翩��, �����߷�, ����, �Ÿ�\n";
	out << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanHeight << "\n";
	out << "; ���� ���� ���� - ���翩��, ���� �����߷�, ���� �����߷�, �Ÿ�, �������� ����ۿ� �����, �������� ����ۿ� ������\n";
	out << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1 << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2 << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadH << " " << pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_HoriF << " " << pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Nangan_VerF << "\n";
	out << "; ���� ���� ���� - ���翩��, ���� �����߷�, ���� �����߷�, �Ÿ�, �������� ����ۿ� �����, �������� ����ۿ� ������\n";
	out << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1 << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2 << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadH << " " << pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_HoriF << " " << pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Nangan_VerF << "\n";
	out << "; ��Ÿ �ɼ� - ������������, ������������, ��ũ����, Ʈ���Ϸ�����, ��ȣ�� ������ũ��Ʈ, �ߺд� ������ũ��Ʈ\n";
	out << pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bPeopleLoad << " " << pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bPeopleLoad << " " << pCalcData->m_bTank << " " << pCalcData->m_bTrailer << " " << pCalcData->m_bMugun << " " << pCalcData->m_bMugun << "\n";
	out << "; �ΰ����� ���翩��, ����\n";
	out << pCalcData->m_bExtraLoad << " " << pCalcData->m_ELoadTitleArr.GetSize() << "\n";
	out << "; �ΰ����� ����,�Ÿ�,�����߷�\n";
	for(n=0; n<pCalcData->m_ELoadTitleArr.GetSize(); n++)	
		out << (LPCSTR)pCalcData->m_ELoadTitleArr.GetAt(n)	<< " " << pCalcData->m_ELoadDisArr.GetAt(n) << " " << pCalcData->m_ELoadWeightArr.GetAt(n) << "\n";	
	out << "; Ⱦ���� ���� ���� - Ⱦ�й� ������ġ, Ȱ���� ���Ϲ��, ���������� ����ڱ���, DB���� �������� �Ÿ�, �������±��� �ܺ�����, ǳ���� ����\n";
	out << pCalcData->m_HLType << " " << pCalcData->m_nHLiveLoadType << " " << pCalcData->m_dUserLaneWidth << " " << pCalcData->m_DBFromEdge << " " << pCalcData->m_bNoDistOnNoGuard << " " << pCalcData->m_bWind << "\n";
	out << "; �𵨸��� �μӼ��� ����\n";
	out << pCalcData->m_AddLoad << "\n";
	out << "END";	
}


void CAPlateDoc::ExportAAbutPier2007Info()
{
	// TODO: Add your command handler code here
	if(!m_pDataManage) return;
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	if(!pDB) return;
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	if(!pOpt) return;

	// ���� ���� ����
	m_pShareBridgeInfoStd->InitShareBridgeInfoStd(0);

	// ������Ʈ ���� ////////////
	m_pShareBridgeInfoStd->SetProjectName(m_pDataManage->GetLineInfoApp()->m_sNameProject);
	m_pShareBridgeInfoStd->SetOwnerName(pDB->m_sNameOwner);
	m_pShareBridgeInfoStd->SetConstructorName(pDB->m_sNameConstructor);
	m_pShareBridgeInfoStd->SetSpotName(pDB->m_sNameSite);
	m_pShareBridgeInfoStd->SetEngName(pDB->m_sNameEngineering);
	m_pShareBridgeInfoStd->SetLineDirName("��������");
	m_pShareBridgeInfoStd->SetLineRevDirName("�����ݴ����");
	////////////////////////////

	// ������ �и�
	m_pShareBridgeInfoStd->SetLineCount(0);
	
	CConvertCommonBridge *pConvertCommonBridge	= m_pShareBridgeInfoStd->m_pConvertCommonBridge[0];
	m_pShareBridgeInfoStd->m_bIsCommonBridge[0]	= TRUE;
	pConvertCommonBridge->SetLineInfoApp(m_pDataManage->GetLineInfoApp());
	pConvertCommonBridge->SetLineBase(0);
	pConvertCommonBridge->SetStaBridge(pDB->m_dStationBridgeStt);
	pConvertCommonBridge->SetLenBridge(pDB->GetLengthBridge());
	pConvertCommonBridge->SetTypeUpDown(0);
	pConvertCommonBridge->InitJijumData(pDB->m_nQtyJigan+1);
	pConvertCommonBridge->InitSegData(1);
	pConvertCommonBridge->InitSpanData(0, pDB->m_nQtyJigan);
	
	long nJ = 0;

	for(nJ = 0; nJ < pConvertCommonBridge->m_pBri->m_nQtyJigan; nJ++)
	{
		pConvertCommonBridge->InitBeamData(0, nJ, pDB->GetGirdersu(), BRIDGE_SECTION_RCSLAB);
	}

	// jijum
	double dSta	= pDB->m_dStationBridgeStt;
	for(nJ = 0; nJ <= pConvertCommonBridge->m_pBri->m_nQtyJigan; nJ++)
	{
		CConvertJijumData *pConvertJijumData	= pConvertCommonBridge->GetConvertJijumData(nJ);
		if(!pConvertJijumData) continue;
		long nVerPos  = (nJ == 0) ? JIJUM_VPOS_END : JIJUM_VPOS_STT;

		pConvertJijumData->SetQtyGirder(pDB->GetGirdersu(), pDB->GetGirdersu());

		long nCountShoe	= 0;
		long girder		= 0;
		for(girder = 0; girder < pDB->GetGirdersu(); girder++)
		{
			CPlateGirderApp *pG	= pDB->GetGirder(girder);
			if(pG)
			{
				pConvertJijumData->SetDisGirderFrLine(girder, pG->m_dGirderCenter, pG->m_dGirderCenter);
				if(girder == 0)
					pConvertJijumData->m_pJ->m_nNumLineSlabLeft[nVerPos]	= pG->m_nLineNum;
				else if(girder == pDB->GetGirdersu()-1)
					pConvertJijumData->m_pJ->m_nNumLineSlabRight[nVerPos]	= pG->m_nLineNum;
				
				pConvertJijumData->m_pJ->m_nNumLineInfo[nVerPos][girder]	= pG->m_nLineNum;

				nCountShoe ++;

				pConvertJijumData->SetXyBearing(girder, 0, pDB->GetXyShoe(girder, nJ), pDB->GetXyShoe(girder, nJ));
			}
		}

		if(nJ == 0)
			pConvertJijumData->SetQtyBearing(0, nCountShoe);
		else
			pConvertJijumData->SetQtyBearing(nCountShoe, 0);

		long nShoe	= 0;
		for(girder = 0; girder < pDB->GetGirdersu(); girder++)
		{
			CPlateGirderApp *pG = pDB->GetGirder(girder);
			CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
			
			CShoe *pShoe	= pG->GetShoe(nJ);
			
			long nCount	= 0;
			CDPoint xyShoe	= pDB->GetXyShoe(nShoe, nJ);
			pConvertJijumData->SetXyBearing(girder, 0, xyShoe, xyShoe);

			// ��� ���� ////
			CBearingData *pBearing	= pConvertJijumData->m_pJ->GetBearing(nVerPos, nShoe);
			pBearing->m_nIndexRef	= pShoe->m_nRefType;
			pBearing->m_H1			= pShoe->m_dThick_Block;
			pBearing->m_H2			= pShoe->m_dThick_Mortar;
			pBearing->m_H3			= pShoe->m_dThick_Shoe;
			pBearing->m_H4			= pShoe->m_dThick_SolePlate;
			pBearing->m_AllowForceVert	= pShoe->m_dForceShoe;
			pBearing->m_dSolePlateWidth	= pShoe->GetWidthTotalSolePlate();
			pBearing->m_dSolePlateLength	= pShoe->GetHeigthTotalSolePlate();
			pBearing->m_sDia1			= pShoe->GetHeightTotalShoe();
			pBearing->m_sDia2			= pShoe->GetHeightTotalShoe();
			pBearing->m_DL1				= pShoe->GetWidthTotalSolePlate() + pShoe->m_dW1_Mortal + pShoe->m_dW2_Mortal;
			pBearing->m_B = pBearing->m_C = pBearing->m_DL1 = pBearing->m_DL2 = pBearing->m_DL1/2;
			if(pShoe->m_cDirectionShoe==0)	pBearing->m_nDir = 0;
			else if(pShoe->m_cDirectionShoe==1) pBearing->m_nDir = 3;
			else if(pShoe->m_cDirectionShoe==2) pBearing->m_nDir = 2;
			else if(pShoe->m_cDirectionShoe==3) pBearing->m_nDir = 1;
			else if(pShoe->m_cDirectionShoe==4) pBearing->m_nDir = 0;
			///////////////////

			nShoe++;
			nCount++;

		
			pConvertJijumData->m_pJ->m_nCountShoe[nVerPos][girder]	= nCount;
			pConvertJijumData->m_pJ->m_dDistShoe[nVerPos][girder][0]	= 0;
		}
		pConvertCommonBridge->m_pBri->m_nTypeShoeAngle	= pDB->m_cDirShoe;


		pConvertJijumData->SetNameJijum(pDB->m_strJijumName[nJ]);
		pConvertJijumData->SetAngleJijum(pDB->m_vAngJijum[nJ].GetAngleDegree());
		if(nJ == 0)
			pConvertJijumData->SetdLenUgan(0, pDB->m_dLengthUganStt);
		else if(nJ == pDB->m_nQtyJigan)
			pConvertJijumData->SetdLenUgan(pDB->m_dLengthUganEnd, 0);
		else
			pConvertJijumData->SetdLenUgan(0, 0);
		pConvertJijumData->SetTypeGirder(BRIDGE_SECTION_STPLATE, BRIDGE_SECTION_STPLATE);

		if(nJ == 0)
		{
			pConvertJijumData->SetLenSlabToGirder(0, pDB->m_dLengthSlabGirderStt);
			pConvertJijumData->SetLenGirderToShoe(0, pDB->m_dLengthGirderShoeStt);
		}
		else if(nJ == pDB->m_nQtyJigan)
		{
			pConvertJijumData->SetLenSlabToGirder(pDB->m_dLengthSlabGirderEnd, 0);
			pConvertJijumData->SetLenGirderToShoe(pDB->m_dLengthGirderShoeEnd, 0);
		}
		else 
		{
			pConvertJijumData->SetLenSlabToGirder(0, 0);
			pConvertJijumData->SetLenGirderToShoe(0, 0);
			
		}

		CPlateBasicIndex *pBx	= pDB->GetGirder(0)->GetBxOnJijum(nJ);
		pConvertJijumData->SetStationJijum(pBx->GetStation());
		pConvertJijumData->SetLenShoeToShoeUseOnlyInput(pDB->m_dLengthJigan[nJ]);

		if(nJ == 0)
		{
			pConvertJijumData->SetTypeAbutPier(pDB->m_nTypeAbutOrPier[0] == 0 ? TYPE_ABUT : TYPE_PIER);
			pConvertJijumData->SetTypeConnect(2);
		}
		else if(nJ == pConvertCommonBridge->m_pBri->m_nQtyJigan)
		{
			pConvertJijumData->SetTypeAbutPier(pDB->m_nTypeAbutOrPier[1] == 0 ? TYPE_ABUT : TYPE_PIER);
			pConvertJijumData->SetTypeConnect(3);
		}
		else
		{
			pConvertJijumData->SetTypeAbutPier(TYPE_PIER);
			pConvertJijumData->SetTypeConnect(4);
		}
		

		double dDistLeft	= pDB->GetSlabLeft()->m_dGirderCenter;
		double dDistRight	= pDB->GetSlabRight()->m_dGirderCenter;
		pConvertJijumData->SetDisSlabLeftFrLine(dDistLeft, dDistLeft);
		pConvertJijumData->SetDistSlabRightFrLine(dDistRight, dDistRight);
	}

	// seg
	CConvertSegData *pConvertSegData	= pConvertCommonBridge->GetConvertSegData(0);
	if(pConvertSegData)
	{
		pConvertSegData->SetBri(0);
		pConvertSegData->SetJijumStt(0);
		pConvertSegData->SetJijumEnd(pDB->m_nQtyJigan);
		pConvertSegData->SetNameSeg("APlate");
		pConvertSegData->SetTypeGirder(BRIDGE_SECTION_STPLATE);
	}


	CSegData *pSeg	= pConvertCommonBridge->m_pBri->GetSegData(0);
	double	dThickSlab	= pDB->m_dThickSlabBase;
	if(pDB->IsTUGir())	dThickSlab	= pDB->m_dThickSlabBase - pDB->m_BindConc.m_dDeep;
	if(pSeg)
	{
		// span
		for(long span = 0; span < pSeg->GetSizeSpan(); span++)
		{
			CConvertSpanData *pConvertSpanData	= pConvertCommonBridge->GetConvertSpanData(0, span);
			if(!pConvertSpanData) continue;
			CSpanData *pSpan	= pSeg->GetSpan(span);
			if(!pSpan) continue;

			long nVerPos  = (span==0) ? JIJUM_VPOS_END : JIJUM_VPOS_STT;
			long nJ1	= span;
			long nJ2	= span+1;
			CConvertJijumData *pConvertJijumData	= pConvertCommonBridge->GetConvertJijumData(nJ1);

			// beam �ǰ��(rc������ beam������ �Է¾ȹ����Ƿ�)
			for(long beam = 0; beam < pSpan->GetSizeBeam(); beam++)
			{
				CBeamData *pBeam	= pSpan->GetBeamData(beam);
				if(!pBeam) continue;
				pBeam->m_pBeamMid->SetDataDefault(0);
				pBeam->m_pBeamEnd->SetDataDefault(1);
				pBeam->SetDataDefault();

				CPlateGirderApp *pGir	= pDB->GetGirder(beam);
				CPlateBasicIndex *pBx	= pGir->GetBxOnJijum(nJ1);
				CPlateBasicIndex *pBx2	= pGir->GetBxOnJijum(nJ2);
				double dSta	= pBx->GetStation();
				
				CPlateFactChain *pFact = pGir->GetFactChain(0, 0);
				double	dHeightGir	= pGir->GetHeightGirderByJijum(nJ1, FALSE);
				double  dHeightGir2	= pGir->GetHeightGirderByJijum(nJ2, FALSE);
				double	dWidFlange	= pGir->GetWidthFlangeUpper(pBx);
				double	dThickFlange	= pGir->GetThickJewon(G_F_L, pBx);
				double	dThickFlange2	= pGir->GetThickJewon(G_F_L, pBx2);

				double	dThickFlangeU	= pGir->GetThickJewon(G_F_U, pBx);
				double	dThickFlangeU2	= pGir->GetThickJewon(G_F_U, pBx2);

				switch(pOpt->GetDesignHeigtBase())
				{
				case 0: break;
				case 1: 
					dHeightGir -= dThickFlange; 
					dHeightGir2 -= dThickFlange2;
					break;
				case 2: 
					dHeightGir -= dThickFlange; 
					dHeightGir2 -= dThickFlange2;
					break;
				case 3: break;
				}
				
				if(pDB->IsTUGir())
				{
					dHeightGir	= pGir->GetHeightGirderByStaAng(pBx->GetStation()) + dThickFlangeU - pDB->m_BindConc.m_dDis
									+ dThickFlange;
					dHeightGir2	= pGir->GetHeightGirderByStaAng(pBx->GetStation()) + dThickFlangeU2 - pDB->m_BindConc.m_dDis
									+ dThickFlange2;

					dWidFlange		= pGir->GetWidthFlangeUpper(pBx);
					dThickFlange	= pDB->m_BindConc.m_dHeight;
					dThickFlange2	= pDB->m_BindConc.m_dHeight;

				}
				pBeam->m_pBeamMid->m_dHeightGirder_NotBeam	= dHeightGir;
				pBeam->m_pBeamMid->m_dWidthGirder_NotBeam	= dWidFlange;
				pBeam->m_pBeamMid->m_dWidthExtGirder_NotBeam	= 0;
				pBeam->m_pBeamMid->m_dThickGirder_NotBeam	= dThickFlange;

				pBeam->m_pBeamEnd->m_dHeightGirder_NotBeam	= dHeightGir2;
				pBeam->m_pBeamEnd->m_dWidthGirder_NotBeam	= dWidFlange;
				pBeam->m_pBeamEnd->m_dWidthExtGirder_NotBeam	= 0;
				pBeam->m_pBeamEnd->m_dThickGirder_NotBeam	= dThickFlange2;
			}
		}

		// hDan
		CConvertHDanData *pConvertHDanData	= pConvertCommonBridge->GetConvertHDanData(0);
		
		if(pConvertHDanData)
		{
			CPlateBasicIndex *pBx	= pDB->GetGirder(0)->GetBxOnJijum(0);
			
			pConvertHDanData->SetQtyHDan(pDB->GetQtyHDanNode());	// Ⱦ�ܰ� ��ȣ�� ���� ����
			pConvertHDanData->SetQtyRail(pDB->GetQtyHDanNode());
			//pConvertHDanData->SetDisBaseElevation(pDB->GetDisBaseElevation());
			//pConvertHDanData->SetDisBaseElevationRL(pDB->GetDisBaseElevation());
			pConvertHDanData->SetHeightDosangUnderTie(pDB->m_dHeightDosang);
			//pConvertHDanData->SetHeightRail(pDB->GetHeightRail());
			pConvertHDanData->SetLevelFrRoadCenterToEdge(pDB->m_cLevelRoadCenterToSlabEdge);
			//pConvertHDanData->SetSlopeDosang(pDB->GetSlopeDosang());
			pConvertHDanData->SetThickPavement(pDB->GetThickPave());
			pConvertHDanData->SetThickSlabStd(dThickSlab);
			pConvertHDanData->SetDistThickSlabStd(pDB->m_dHunchDisForBaseThick);

			CHDanData *pHDan	= pSeg->m_pHDan;
			pConvertHDanData->SetApplyLineSlope(TRUE);	
			long nIdxGuardWall	= 0;
			for(long d = 0; d < pHDan->m_nQtyHDan; d++)
			{
				pConvertHDanData->SetLengthHDan(d, pDB->GetLengthHDanDom(pBx, d));
				pConvertHDanData->SetLevelHDan(d, pDB->GetTypeLevelHDan(d)==1);
				pConvertHDanData->SetSlopeHDan(d, 0); //GetLine()->GetPyungubae(StaCen,Dist);
				pConvertHDanData->SetTypeHDan(d, pDB->GetWallGuardTypeHDan(d));

				// GuardWall
				CGuardWall* pGW = pConvertCommonBridge->m_pBri->GetGuardWallByHDan(0, d);
				if(pGW)
				{
					pGW->m_bRailRoad = FALSE;
					pGW->m_bLeftGuardFence = d == 0;

					if(pGW->m_nTypeGuardWall != 0)
					{
						CCentSeparation *pCentSeparation = pDB->GetGuardWall(nIdxGuardWall++);
						if(pCentSeparation)
						{
							pGW->m_dDistFrLeft	= pCentSeparation->m_D1;			
							pGW->m_dW1			= pCentSeparation->m_W1;			
							pGW->m_dW2			= pCentSeparation->m_W2;			
							pGW->m_dW3			= pCentSeparation->m_W3;			
							pGW->m_dW4			= pCentSeparation->m_W4;			
							pGW->m_dW5			= pCentSeparation->m_W5;			
							pGW->m_dH1			= pCentSeparation->m_H1;			
							pGW->m_dH2			= pCentSeparation->m_H2;			
							pGW->m_dH3			= pCentSeparation->m_H3;			
						}
					}
				}
			}			
		}
	}

	
	// ���¸� �ݷ�//////////////////////////////////////////////
	// ����� ���� ���ݷ� ��������
	// ��� ���������� �˻�		
	if(m_pDataManage->GetCalcData()->m_bReadFrameModelingOutput)
	{
		CString sFileName	= "c:\\AAbutPier2007Export.tmp";
		// �Ǿ�1�� ����� Ȱ���ؼ� �ݷºκи� ���� �����ؼ� ����Ѵ�.
		// �Ǿ�1�� �ڵ� �� ���
		CFile f;
		if(f.Open(sFileName, CFile::modeCreate|CFile::modeWrite)==TRUE)
		{
			CArchive ar(&f,CArchive::store);

			SapReadReaction(ar);

			ar.Close();
			f.Close();
		}

		CFile ff;
		if(ff.Open(sFileName, CFile::modeRead)==TRUE)
		{
			CArchive ar(&ff,CArchive::load);
			SetReactionToShareBridgeInfo(ar, pDB->m_nQtyJigan, m_nFlagExportApierInfo);
			ar.Close();
			ff.Close();

			//
			m_bExistBodoInBrx	= FALSE;
			m_bExistWindInBrx	= FALSE;
			for(long nJ = 0; nJ <= pDB->m_nQtyJigan; nJ++)
				ExportReactionByJijum(nJ);
		}
	}

	/////////////////////////////////////////////////////////////////////
	// �ܸ� �� ���κ� ���� => �����ؼ�  : START !!
	AhTPADelete(&m_pShareBridgeInfoStd->m_arrDiaphragm[0], (CEarthQuakeDiaphram*)0);
	AhTPADelete(&m_pShareBridgeInfoStd->m_arrCrossBeam[0], (CEarthQuakeCrossBeam*)0);
	long nQtyGirder = pDB->GetGirdersu();
	for(long nG=0; nG<nQtyGirder; nG++)
	{			
		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_JOINT|BX_END_JOINT|BX_FLANGE|BX_BYUN|BX_CROSSBEAM_VBRACING);
		while(pBx)
		{			
			CEarthQuakeDiaphram *pDiap = new CEarthQuakeDiaphram;

			// �ڽ� ����				
			pDiap->m_nG = nG;
			pDiap->m_nTypeCalc = pDB->GetNumHaiseokCombo();
			pDiap->m_dFck = toKgPCm2(m_pDataManage->GetDeckData()->m_SigmaCK);

			double dSta = pBx->GetStation();
			pDiap->m_dStation = dSta;

			CFemModelingCalc *pFemModelingCalc = m_pAPlateOutCalcStd->GetFemModelingCalc();
			
			__int64 bxFlag = pBx->GetBxFlagState();
			long nJijum = pGir->GetNumberJijumByBx(pBx);
			if(bxFlag&BX_STT_JOINT || bxFlag&BX_END_JOINT || (pBx->IsPier() && (nJijum!=pDB->m_nQtyJigan && nJijum!=0)))
			{
				pDiap->m_nNumberJijum = pGir->GetNumberJijumByBx(pBx);
			}
			double dUpperFlangeThick = pGir->GetThickFlangeByBase(pBx->GetStation(), TRUE);
			double dLowerFlangeThick = pGir->GetThickFlangeByBase(pBx->GetStation(), FALSE);
			double dWebHeight = pGir->GetHeightGirderByStaAng(pBx->GetStation());
			double dWidUpperFlange = pGir->GetWidthFlangeUpper(pBx);
			if(pDB->IsTUGir())
			{
				dUpperFlangeThick	= pDB->m_BindConc.m_dHeight;
				dWebHeight	= pGir->GetHeightGirderByStaAng(pBx->GetStation()) + pGir->GetThickFlangeByBase(pBx->GetStation(), TRUE) - pDB->m_BindConc.m_dDis;
				dWidUpperFlange = pDB->m_BindConc.m_dWidth;
			}
			pDiap->m_dWidthOnSlab		= toM(pFemModelingCalc->GetWidthSlab(nG,pDiap->m_dStation));
			pDiap->m_dHeight			= toM(dWebHeight+dUpperFlangeThick+dLowerFlangeThick);
			pDiap->m_dWidthUpper		= toM(dWidUpperFlange);
			pDiap->m_dWidthLower		= toM(pGir->GetWidthFlangeLower(pBx));
			pDiap->m_dThickSlabBase		= toM(dThickSlab);
			pDiap->m_dThickSlabHunch	= toM(pDB->m_dThickSlabHunch);
			pDiap->m_dThickFactUpper	= toM(pGir->GetThickJewon(G_F_U, pBx));
			pDiap->m_dThickFactLower	= toM(pGir->GetThickJewon(G_F_L, pBx));
			pDiap->m_dThickFactWeb		= toM(pGir->GetThickJewon(G_W, pBx));

			double	dTotalGirderLen	= 0;
			double	dTotalWeight	= 0;
			double	dWeightGir		= 0;
			long	nQtyJigan		= pDB->m_nQtyJigan;
			if(m_pArrSteelBox_SapOut.GetSize()>0)
			{
				for(long nG=0; nG<nQtyGirder; nG++)
				{	
					CPlateGirderApp *pGir = m_pAPlateOutCalcStd->GetAPlateBridgeApp()->GetGirder(nG);
					double	dSttGir	= pGir->GetSSS(OFF_STT_GIRDER);
					double	dEndGir	= pGir->GetSSS(OFF_END_GIRDER);
	//				double dGirderLen = pGir->GetLengthDimType(pGir->GetStationBySc(pGir->GetSSS(OFF_STT_GIRDER),0,pGir->GetSSS(OFF_STT_GIRDER)), pGir->GetStationBySc(pGir->GetSSS(OFF_END_GIRDER),0,pGir->GetAAA(OFF_END_GIRDER)));
					double dGirderLen = pGir->GetLengthDimType(dSttGir, dEndGir, 0);
					double dW=0, nVPos=2;				

					for(long nJ=0; nJ<nQtyJigan+1; nJ++)
					{				
			//			CJijum *pJ = pConvertCommonBridge->m_pBri->GetJijum(nJ);
						CSteelBox_SapOut *pSB_SO	= m_pArrSteelBox_SapOut.GetAt(nJ);
						CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nJ);
						CShoe ShoeFir = pGir->m_Shoe[nJ];
						CShoe ShoeNxt = pGir->m_Shoe[nJ+1];
						if(ShoeNxt.GetDisShoeFrWeb() != 0)
							nVPos = 2;
						else
							nVPos = 1;

						for(long nSide=0; nSide<nVPos; nSide++)
						{					
							if(pConvertCommonBridge->m_pBri->m_bRailDesign)
							{						
								dW += pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadConcBeforeCombine[nSide] +
									  pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadSteelBeforeCombine[nSide] +
									  pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadAfterCombine[nSide];
							}
							else
							{
								dW += pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadConcBeforeCombine[nSide] +
									  pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadSteelBeforeCombine[nSide] +
									  pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadAfterCombine[nSide];
							}
						}
					}
					dTotalWeight	+= dW;
					dTotalGirderLen	+= dGirderLen;
				}
				dWeightGir = dTotalWeight/toM(dTotalGirderLen);	// // ������ü �������� �ݷ� / �Ŵ����� ��ü��;
			}

			pDiap->m_WeightGir = dWeightGir;
			pDiap->m_WeightBangHo = 0;
			pDiap->m_WeightPave = 0;

			m_pShareBridgeInfoStd->m_arrDiaphragm[0][0].Add(pDiap);
			//////////////////////////////////////////////////////////////////////////
			
			// ���κ� ����				
			if(pBx->IsState(BX_CROSSBEAM_VBRACING))
			{
				BOOL bCrossLeft  = pBx->IsCrossBeamLeft();
				BOOL bCrossRight = pBx->IsCrossBeamRight();
				if(bCrossLeft==FALSE && bCrossRight==FALSE)
				{
					pBx = Finder.GetBxNext();
					continue;
				}

				CPlateCrossBeam *pCr = bCrossLeft ? pBx->GetCrossBeamLeft() : pBx->GetCrossBeam();
				if(pCr!=NULL)
				{
					CEarthQuakeCrossBeam *pCross = new CEarthQuakeCrossBeam;
					pCross->m_nG = nG;
					pCross->m_dStation = pBx->GetStation(); // ??????
					if(bxFlag&BX_STT_JOINT || bxFlag&BX_END_JOINT || (pBx->IsPier() && (nJijum!=pDB->m_nQtyJigan && nJijum!=0)))
					{
						pCross->m_nNumberJijum = pGir->GetNumberJijumByBx(pBx);
					}
					pCross->m_bLeft  = bCrossLeft;
					pCross->m_bRight = bCrossRight;
					pCross->m_dH = toM(pCr->m_H_CR);
					pCross->m_dUWidth = toM(pCr->m_dM_CR * 2);
					pCross->m_dLWidth = toM(pCr->m_uM_CR * 2);
					pCross->m_dUThick = toM(pCr->m_T2_CR);
					pCross->m_dLThick = toM(pCr->m_T1_CR);
					pCross->m_dWThick = toM(pCr->m_T3_CR);
					pCross->m_WeightCB = 0;

					m_pShareBridgeInfoStd->m_arrCrossBeam[0][0].Add(pCross);
				}
			}

			pBx = Finder.GetBxNext();
		}			
	}


	// �ܸ� �� ���κ� ���� => �����ؼ�  : END !!
	//////////////////////////////////////////////////////////////////////////

	// BasicConcInfo
	//m_pShareBridgeInfoStd->SetBasicConcInfo();

	CString sTitle	= _T("");//pDB->m_sNameBridge;
	m_pShareBridgeInfoStd->SetNameSoftWareExport("APlate");
	m_pShareBridgeInfoStd->SetNameBridgeExport(sTitle);

	sTitle	= sTitle + "�� ���� ���� ���� ��������";
	m_pShareBridgeInfoStd->Export(sTitle);
}

// �� �ҽ��� ��ġ���� ARoad���� ���ľ� ��.
// ARoad�� ��ġ�� APlate�� �����ؼ� ��� ��
void CAPlateDoc::SetReactionToShareBridgeInfo(CArchive &ar, long nQJ, long nFlag)
{
	if(!m_pDataManage) return;
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	if(!pDB) return;
	
	CCommonBridgeApp *pBri	= m_pShareBridgeInfoStd->m_pConvertCommonBridge[0]->m_pBri;
	InitSteelBox_SapOut();
	AhTPAMake(nQJ+1, &m_pArrSteelBox_SapOut, (CSteelBox_SapOut*)0);

	m_nCalcLaneSu = 0;
	BOOL bFirst = TRUE;
	if(!ar.IsStoring())
	{
		//�ݷ� ����
		long nGirderSu = 0;
		BOOL bHapSung = TRUE;
		long nNodesu = 0, nQtyJigan = 0;
		
		ar >> nGirderSu;
		ar >> bHapSung;
				
		for (long nLoadGir = 0; nLoadGir < nGirderSu; nLoadGir++)   
		{
			long nLoadTypesu, nLoadType;
			ar >> nLoadTypesu;			
			for (long nLT = 1; nLT <= nLoadTypesu; nLT++)   
			{			
				BOOL bExist;
				ar >> bExist;					
				if (!bExist)   
				{
					ar >> bExist;					
					if (!bExist) continue;
				}
				ar >> nLoadType;
				/* LoadType ������ ���� �Ʒ� ����				
					1 : DB ����
					2 : DL ����
					3 : TANK ����
					4 : TANK TRAILER ����
					5 : ���� ����
					6 : ǳ����
				}*/
				if(nLoadType == 5) m_bExistBodoInBrx = TRUE;	// �������� ����
				if(nLoadType == 6) m_bExistWindInBrx = TRUE;	// ǳ���� ����
				
				long nMaxLanesu;
				long nKK, nCurG;
				BYTE Uz, bMain;
				ar >> nMaxLanesu;
				for (long nLane = 0; nLane <= nMaxLanesu; nLane++)		
				{					
					ar >> bExist;
					if (!bExist) continue;
					if(bFirst && bExist) ++m_nCalcLaneSu;
					ar >> nQtyJigan;							
					for (long nK = 0; nK <= nQtyJigan; nK++)   
					{	
						CJijumData *pJ	= pBri->GetJijum(nK);
						CSteelBox_SapOut *pSB_SO	= m_pArrSteelBox_SapOut.GetAt(nK);

						long nShoePos=0, nRealShoe=0;
						ar >> nNodesu;
						for (int i = 0; i < nNodesu; i++)   
						{
							ar >> Uz;
							ar >> nKK;
							ar >> bMain;
							if (!Uz || nKK != nK) continue;
							if (bMain != 5) ar >> nCurG;					

							//if(pLD->GetJijum(nK)->GetGirderData(nCurG-1, 0)->m_dDist[1] != 0)
							CPlateGirderApp *pG = pDB->GetGirder(nCurG-1);
							CPlateBasicIndex *pBx = pG->GetBxOnJijum(nK);
							CShoe ShoeFir = pG->m_Shoe[nK];
							nShoePos = 0;
							
							pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_nG = nCurG-1;
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dDeadSteelBeforeCombine[nShoePos];
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dDeadConcBeforeCombine[nShoePos];
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dDeadAfterCombine[nShoePos];							
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dDBMaxMin[nShoePos][0];
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dDBMaxMin[nShoePos][1];
							if(nLoadType==5)
							{
								if(nFlag>8)
								{
									ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane+1][nCurG-1].m_dDBMaxMin[nShoePos][0];
									ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane+2][nCurG-1].m_dDBMaxMin[nShoePos][0];
								}
								else
								{
									AfxMessageBox("���� ���������� ����Ǿ� ������ �ݿ��� ���� ���Ͽ����ϴ�.\nARoad2002 �ֽŹ������� brx ������ �ٽ� �ҷ����ñ� �ٶ��ϴ�.", MB_ICONSTOP);
								}
							}
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dSinkingMaxMin[nShoePos][0];
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dSinkingMaxMin[nShoePos][1];
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dTotalMaxMin[nShoePos][0];
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_dTotalMaxMin[nShoePos][1];							
							ar >> pSB_SO->STEEL_SAPOUT[nLoadGir+1][nLoadType][nLane][nCurG-1].m_nIdx[nShoePos];							
							++nRealShoe;
							if(nRealShoe==2) nRealShoe=0;
						}						
					}
					if (nLoadType >= 3)  // ��ũ�� Ʈ���Ϸ��� ������ ����.
						break;
				}
				bFirst = FALSE;
			}
		}
		
		for (long nK = 0; nK <= nQJ; nK++)   
		{	
			CJijumData *pJ	= pBri->GetJijum(nK);
			CSteelBox_SapOut *pSB_SO	= m_pArrSteelBox_SapOut.GetAt(nK);
			long nShoePos=0, nRealShoe=0;
			long nKK, nCurG;
			BYTE Uz, bMain;
			if(nFlag > 7)	ar >> nNodesu;
			for (int i = 0; i < nNodesu; i++)   
			{
				ar >> Uz;
				ar >> nKK;
				ar >> bMain;
				if (!Uz || nKK != nK) continue;				
				if (bMain != 5) ar >> nCurG;
				//pLD->GetJijum(nK)->GetGirderData(nCurG-1, 0)->m_dDist[1] != 0)
				CPlateGirderApp *pG = pDB->GetGirder(nCurG-1);
				CPlateBasicIndex *pBx = pG->GetBxOnJijum(nK);
				CShoe ShoeFir = pG->m_Shoe[nK];
				nShoePos = 0;
				
				pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_nG = nCurG-1;
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dDeadSteelBeforeCombine[nShoePos];
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dDeadConcBeforeCombine[nShoePos];
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dDeadAfterCombine[nShoePos];							
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dDBMaxMin[nShoePos][0];							
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dDBMaxMin[nShoePos][1];
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dSinkingMaxMin[nShoePos][0];
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dSinkingMaxMin[nShoePos][1];
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dTotalMaxMin[nShoePos][0];
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_dTotalMaxMin[nShoePos][1];							
				ar >> pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nCurG-1].m_nIdx[nShoePos];

				++nRealShoe;
				if(nRealShoe==2) nRealShoe=0;				
			}			
		}				
	}
}

// �� �ҽ��� ��ġ���� ARoad���� ���ľ� ��.
// ARoad�� ��ġ�� APlate�� �����ؼ� ��� ��
void CAPlateDoc::ExportReactionByJijum(long nJ)
{
	if(!m_pDataManage) return;
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	if(!pDB) return;
	
	CSteelBox_SapOut *pSB_SO	= m_pArrSteelBox_SapOut.GetAt(nJ);

	// �ݷ� ��������� �ʿ��� ������//////
	long nVPos	= nJ==0 ? 1 : 0;
	long nIdx	= 0;
	double dLoadDead	= 0;
	double dLoadDeadDW = 0;
	double dLoadLive[5]	= {0, 0, 0, 0, 0};
	double dLoadMax	= 0;
	// �ݷ� ��������� �ʿ��� ������//////

	BOOL bRailDesign	= FALSE;
	CConvertCommonBridge *pConvertCommonBridge	= m_pShareBridgeInfoStd->m_pConvertCommonBridge[0];
	CCommonBridgeApp *pBri	= pConvertCommonBridge->m_pBri;
	CConvertJijumData *pConvertJijumData	= pConvertCommonBridge->GetConvertJijumData(nJ);
	CJijumData *pJ	= pConvertJijumData->m_pJ;
	long nQtyBearingV1(0), nQtyBearingV2(0);
	pConvertJijumData->GetQtyBearing(nQtyBearingV1, nQtyBearingV2);
	long nTypeAbutPier	= TYPE_PIER;
	long i				= 0;
	long n				= 0;
	long nL				= 0;
	long nUpDn = 0;
	pConvertJijumData->GetTypeAbutPier(nTypeAbutPier);

	if(nTypeAbutPier == TYPE_ABUT)
	{
		long nGMaxGirder = pJ->GetQtyGirder(0)-1;
		long nQtyBearing	= nJ == 0 ? nQtyBearingV2 : nQtyBearingV1;
		if(bRailDesign)
		{
			for(i = 0; i<nQtyBearing; i++)
			{
				//long nG = pLD->GetNumberGirderByShoe(i, pAbut->m_nJNO, 0), nSide;			
				long nG	= pDB->GetNumberGirderByShoe(i, nJ), nSide;
				nSide	= 0;

				// �ʱ�ȭ //
				dLoadDead	= 0;
				dLoadLive[0]	= 0;
				dLoadLive[1]	= 0;
				dLoadLive[2]	= 0;
				dLoadLive[3]	= 0;
				dLoadLive[4]	= 0;
				dLoadMax		= 0;
				// �ʱ�ȭ //

				dLoadDead    = pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadConcBeforeCombine[nSide] +
									   pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadSteelBeforeCombine[nSide] +
									   pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadAfterCombine[nSide];					
				dLoadLive[0] = max(fabs(pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDBMaxMin[nSide][0]),
										   fabs(pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDBMaxMin[nSide][1]));									

				dLoadMax = pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dTotalMaxMin[nSide][0];

				m_pShareBridgeInfoStd->SetBearingReAction(nJ, nVPos, i, dLoadDead, dLoadDeadDW, dLoadLive[0], dLoadLive[1], dLoadLive[2], dLoadLive[3], dLoadLive[4], dLoadMax, nUpDn);
			}
		}
		else
		{			
			for(i = 0; i<nQtyBearing; i++)
			{
				//long nG = pLD->GetNumberGirderByShoe(i, pAbut->m_nJNO, 0), nSide;			
				long nG	= pDB->GetNumberGirderByShoe(i, nJ), nSide;
				nSide	= 0;

				// �ʱ�ȭ //
				dLoadDead	= 0;
				dLoadLive[0]	= 0;
				dLoadLive[1]	= 0;
				dLoadLive[2]	= 0;
				dLoadLive[3]	= 0;
				dLoadLive[4]	= 0;
				dLoadMax		= 0;
				// �ʱ�ȭ //
				
				dLoadDead    = pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadConcBeforeCombine[nSide] +
									   pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadSteelBeforeCombine[nSide] +
									   pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadAfterCombine[nSide];
				
				double dMax		 = 0;
				long nMaxLane	 = 0;			
				long nGMaxCase	 = 0;			
				long nMaxLoadCase= 0;				
				for(nL=1; nL<LOADTYPE-1; nL++) 
				{				
					for(n=1; n<MAXLANE-1; n++)
					{								
						for(long nGMax=0; nGMax<pJ->GetQtyGirder(0); nGMax++)
						{
							double dMaxData = 0;
							for(long nGCur=0; nGCur<pJ->GetQtyGirder(0); nGCur++)
							{										
								double dMaxSub = pSB_SO->STEEL_SAPOUT[nGMax+1][nL][n][nGCur].m_dTotalMaxMin[0][0];
								//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
								CPlateGirderApp *pG = pDB->GetGirder(nGCur);
								CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
								CShoe ShoeFir = pG->m_Shoe[nJ];
								dMaxData += dMaxSub;								
							}
							if(dMax < dMaxData)
							{
								nMaxLane = n;					
								dMax = dMaxData;
								nGMaxCase = nGMax;
								nMaxLoadCase=nL;
							}
						}
					}
				}
				if(nGMaxGirder==0) // �Ŵ��� 1���� ��� ���������Ͻÿ����� �˻�
				{
					for(nL=1; nL<LOADTYPE-1; nL++) 
					{				
						for(n=1; n<MAXLANE-1; n++)
						{								
							double dMaxData = 0;
							for(long nGCur=0; nGCur<pJ->GetQtyGirder(0); nGCur++)
							{										
								double dMaxSub = pSB_SO->STEEL_SAPOUT[2][nL][n][0].m_dTotalMaxMin[0][0];
								//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
								CPlateGirderApp *pG = pDB->GetGirder(nGCur);
								CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
								CShoe ShoeFir = pG->m_Shoe[nJ];
								dMaxData += dMaxSub;								
							}
							if(dMax < dMaxData)
							{
								nMaxLane = n;					
								dMax = dMaxData;
								nGMaxCase = 1;
								nMaxLoadCase=nL;								
							}							
						}
					}
				}
				double dMaxDB = 0;			
				//if(m_pIn->m_pAPierDataStd->m_bABeamDeckImport && m_pIn->m_pAPierDataStd->m_nABeamDeckOutTypeReaction==1)
				//	dMaxDB = pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDBMaxMin[nSide][0];
				//else
				dMaxDB = pSB_SO->STEEL_SAPOUT[nGMaxCase+1][nMaxLoadCase][nMaxLane][nG].m_dDBMaxMin[nSide][0];
				
				dLoadLive[0] = dMaxDB;

				if(m_bExistBodoInBrx) // �������� �����
				{
					dLoadLive[0] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][5][1][nG].m_dDBMaxMin[nSide][0];					
					dLoadLive[0] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][5][2][nG].m_dDBMaxMin[nSide][0];					
				}
				// �����Ͻ� �������
				//if(m_pIn->m_pAPierDataStd->m_bExistWindInBrx) // ǳ���� �����
				//{					
				//	dLoadLive[0] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][6][0][nG].m_dDBMaxMin[nSide][0];					
				//}

				dLoadMax = pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dTotalMaxMin[nSide][0];

				m_pShareBridgeInfoStd->SetBearingReAction(nJ, nVPos, i, dLoadDead, dLoadDeadDW, dLoadLive[0], dLoadLive[1], dLoadLive[2], dLoadLive[3], dLoadLive[4], dLoadMax, nUpDn);
			}
		}
	}
	else 
	{
		long nGMaxGirder = pJ->GetQtyGirder(nVPos)-1;
		if(bRailDesign)
		{
			long nQtyBearing	= nJ > 0 ? nQtyBearingV1 : nQtyBearingV2;
			for(i = 0; i<nQtyBearing; i++)
			{
				// �ʱ�ȭ //
				dLoadDead	= 0;
				dLoadLive[0]	= 0;
				dLoadLive[1]	= 0;
				dLoadLive[2]	= 0;
				dLoadLive[3]	= 0;
				dLoadLive[4]	= 0;
				dLoadMax		= 0;
				// �ʱ�ȭ //

				long nG	= pDB->GetNumberGirderByShoe(i, nJ), nSide;
				nSide	= 0;
				dLoadDead    = pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadConcBeforeCombine[nSide] +
									   pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadSteelBeforeCombine[nSide] +
									   pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDeadAfterCombine[nSide];					
				dLoadLive[0] = max(fabs(pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDBMaxMin[nSide][0]),
						                   fabs(pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dDBMaxMin[nSide][1]));									
				for(long nType=0; nType<2; nType++)
				{					
					double dMax=0;	
					for(nL=nType; nL<4; nL+=2) 
					{				
						for(n=0; n<MAXLANE; n++)
						{	
							double dMaxData = 0, dMinData = 0;
							long nQtyGirder	= pJ->GetQtyGirder(nVPos);
							for(long nGMax=0; nGMax<nQtyGirder; nGMax++)
							{								
								double dMaxSub = pSB_SO->STEEL_SAPOUT[nGMax][nL][n][nGMax].m_dDBMaxMin[nSide][0];
								double dMinSub = pSB_SO->STEEL_SAPOUT[nGMax][nL][n][nGMax].m_dDBMaxMin[nSide][1];
								dMax = max(max(dMax, fabs(dMaxSub)),fabs(dMinSub));
							}
						}							
					}
					dLoadLive[nType+1] = dMax;
					dLoadMax = pSB_SO->STEEL_SAPOUT[nG][LOADTYPE-1][MAXLANE-1][nG].m_dTotalMaxMin[nSide][0];
				}

				m_pShareBridgeInfoStd->SetBearingReAction(nJ, nVPos, i, dLoadDead, dLoadDeadDW, dLoadLive[0], dLoadLive[1], dLoadLive[2], dLoadLive[3], dLoadLive[4], dLoadMax, nUpDn);
			}
		}
		else
		{			
			long nShoeRow = 2;
			for(long k=0; k<nShoeRow; k++)
			{				
				//if(k == 1) continue;	// aroad �� �Ŀ� ����
				// aroad�� �Ŀ��� ������ ������ ������ ���� �Ŀ��� ����
				if(nJ == 0 && k == 0) continue;
				if(nJ > 0 && k == 1) continue;	// aroad �� �Ŀ� ����

				long nQtyBearing	= k == 0 ? nQtyBearingV1 : nQtyBearingV2;
				for(i=0; i<nQtyBearing; i++)
				{
					// �ʱ�ȭ //
					dLoadDead	= 0;
					dLoadLive[0]	= 0;
					dLoadLive[1]	= 0;
					dLoadLive[2]	= 0;
					dLoadLive[3]	= 0;
					dLoadLive[4]	= 0;
					dLoadMax		= 0;
					// �ʱ�ȭ //

					long nG	= pDB->GetNumberGirderByShoe(i, nJ), nSide;
					nSide	= 0;
					
					dLoadDead    = pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadConcBeforeCombine[nSide] +
										   pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadSteelBeforeCombine[nSide] +
										   pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dDeadAfterCombine[nSide];
									
					double dMax=0;
					long nMaxLane=0;			
					long nGMaxCase=0;			
					long nMaxLoadCase=0;
					for(nL=1; nL<LOADTYPE-1; nL++) 
					{				
						for(n=1; n<MAXLANE-1; n++)
						{								
							long nQtyGirder	= pJ->GetQtyGirder(nVPos);
							for(long nGMax=0; nGMax<nQtyGirder; nGMax++)
							{
								double dMaxData = 0;
								for(long nGCur=0; nGCur<nQtyGirder; nGCur++)
								{										
									double dMaxSub = pSB_SO->STEEL_SAPOUT[nGMax+1][nL][n][nGCur].m_dTotalMaxMin[0][0];
									//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
									CPlateGirderApp *pG = pDB->GetGirder(nGCur);
									CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
									CShoe ShoeFir = pG->m_Shoe[nJ];
									dMaxData += dMaxSub;								
								}
								if(dMax < dMaxData)
								{
									nMaxLane = n;					
									dMax = dMaxData;
									nGMaxCase = nGMax;
									nMaxLoadCase=nL;
								}
							}
						}
					}
					if(nGMaxGirder==0) // �Ŵ��� 1���� ��� ���������Ͻÿ����� �˻� - �ټ� ���ذ� ??
					{
						for(nL=1; nL<LOADTYPE-1; nL++) 
						{				
							for(long n=1; n<MAXLANE-1; n++)
							{								
								double dMaxData = 0;
								long nQtyGirder	= pJ->GetQtyGirder(nVPos);
								for(long nGCur=0; nGCur<nQtyGirder; nGCur++)
								{										
									double dMaxSub = pSB_SO->STEEL_SAPOUT[2][nL][n][0].m_dTotalMaxMin[0][0];
									//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
									CPlateGirderApp *pG = pDB->GetGirder(nGCur);
									CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
									CShoe ShoeFir = pG->m_Shoe[nJ];
									dMaxData += dMaxSub;								
								}
								if(dMax < dMaxData)
								{
									nMaxLane = n;					
									dMax = dMaxData;
									nGMaxCase = 1;
									nMaxLoadCase=nL;								
								}							
							}
						}
					}
					double dMaxDB = 0;			
					dMaxDB = pSB_SO->STEEL_SAPOUT[nGMaxCase+1][nMaxLoadCase][nMaxLane][nG].m_dDBMaxMin[nSide][0];							
					
					dLoadLive[0] = dMaxDB;

					if(m_bExistBodoInBrx) // �������� �����
					{
						dLoadLive[0] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][5][1][nG].m_dDBMaxMin[nSide][0];					
						dLoadLive[0] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][5][2][nG].m_dDBMaxMin[nSide][0];					
					}
					// �����Ͻ� �������
					//if(m_pIn->m_pAPierDataStd->m_bExistWindInBrx) // ǳ���� �����
					//{					
					//	dLoadLive[0] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][6][0][nG].m_dDBMaxMin[nSide][0];					
					//}

					dLoadMax = pSB_SO->STEEL_SAPOUT[0][LOADTYPE-1][MAXLANE-1][nG].m_dTotalMaxMin[nSide][0];

					CBearingData *pBearing	= pJ->GetBearing(nVPos, i);
					pBearing->m_LoadDead	= dLoadDead;
					pBearing->m_LoadLive[0]	= dLoadLive[0];
					pBearing->m_LoadMax		= dLoadMax;
				}
				
				nQtyBearing	= k==0 ? nQtyBearingV1 : nQtyBearingV2;
				for(i=0; i<nQtyBearing; i++)
				{
					// �ʱ�ȭ //
					dLoadDead	= 0;
					dLoadLive[0]	= 0;
					dLoadLive[1]	= 0;
					dLoadLive[2]	= 0;
					dLoadLive[3]	= 0;
					dLoadLive[4]	= 0;
					dLoadMax		= 0;
					// �ʱ�ȭ //

					long nG	= pDB->GetNumberGirderByShoe(i, nJ), nSide;
					nSide	= 0;
					
					// ���� �� ���Ͻ�
					double dMax=0; 
					long nMaxLane=0;
					long nMaxLoadCase=0;
					long nLGM = 1;
					for(nL=1; nL<LOADTYPE-1; nL++) 
					{				
						if(nL==5 || nL==6) continue; // ��������, ǳ������ ����
						for(long n=1; n<max(1,m_nCalcLaneSu/2)+1; n++)
						{								
							double dMaxData = 0;
							long nQtyGirder	= pJ->GetQtyGirder(nVPos);
							for(long nGCur=0; nGCur<nQtyGirder; nGCur++)
							{										
								double dMaxSub = pSB_SO->STEEL_SAPOUT[1][nL][n][nGCur].m_dTotalMaxMin[0][0];
								//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
								CPlateGirderApp *pG = pDB->GetGirder(nGCur);
								CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
								CShoe ShoeFir = pG->m_Shoe[nJ];
								dMaxData += dMaxSub;								
							}
							if(dMax < dMaxData)
							{
								nMaxLane = n;					
								dMax = dMaxData;						
								nMaxLoadCase=nL;
							}
						}				
					}		
					if(nGMaxGirder==0) // �Ŵ��� 1���� ��� ���������Ͻÿ����� �˻� - �ټ� ���ذ� ??
					{				
						for(nL=1; nL<LOADTYPE-1; nL++) 
						{				
							if(nL==5 || nL==6) continue; // ��������, ǳ������ ����
							for(n=1; n<max(1,m_nCalcLaneSu/2)+1; n++)
							{								
								double dMaxData = 0;
								long nQtyGirder	= pJ->GetQtyGirder(nVPos);
								for(long nGCur=0; nGCur<nQtyGirder; nGCur++)
								{										
									double dMaxSub = pSB_SO->STEEL_SAPOUT[2][nL][n][nGCur].m_dTotalMaxMin[0][0];
									//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
									CPlateGirderApp *pG = pDB->GetGirder(nGCur);
									CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
									CShoe ShoeFir = pG->m_Shoe[nJ];
									dMaxData += dMaxSub;								
								}
								if(dMax < dMaxData)
								{
									nMaxLane = n;					
									dMax = dMaxData;						
									nMaxLoadCase=nL;
									nLGM = 2;
								}
							}				
						}	
					}
					dLoadLive[1] = pSB_SO->STEEL_SAPOUT[nLGM][nMaxLoadCase][nMaxLane][nG].m_dDBMaxMin[nSide][0];

					
					if(m_bExistBodoInBrx) // �������� �����
					{					
						dLoadLive[1] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][5][1][nG].m_dDBMaxMin[nSide][0];					
					}
	//-				if(m_pIn->m_pAPierDataStd->m_bExistWindInBrx) // ǳ���� �����
	//-				{
	//-					dLoadLive[1] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][6][0][nG].m_dDBMaxMin[nSide][1];					
	//-				}

					// ���� �� ���Ͻ�				
					dMax=0; 
					nMaxLane=0;
					nMaxLoadCase=0;
					nLGM = nGMaxGirder;
					for(nL=1; nL<LOADTYPE-1; nL++) 
					{				
						for(n=1; n<max(1,m_nCalcLaneSu/2)+1; n++)
						{								
							double dMaxData = 0;
							long nQtyGirder	= pJ->GetQtyGirder(nVPos);
							for(long nGCur=0; nGCur<nQtyGirder; nGCur++)
							{									
								strSTEELBOX_SAPOUT stSteelSapOut = pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][nL][n][nGCur];
								double dMaxSub = stSteelSapOut.m_dTotalMaxMin[0][0];
								//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
								CPlateGirderApp *pG = pDB->GetGirder(nGCur);
								CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
								CShoe ShoeFir = pG->m_Shoe[nJ];
								dMaxData += dMaxSub;								
							}
							if(dMax < dMaxData)
							{
								nMaxLane = n;					
								dMax = dMaxData;						
								nMaxLoadCase=nL;
							}
						}				
					}			
					if(nGMaxGirder==0) // �Ŵ��� 1���� ��� ���������Ͻÿ����� �˻� - �ټ� ���ذ� ??
					{		
						nLGM = 1;
						for(nL=1; nL<LOADTYPE-1; nL++) 
						{				
							for(n=1; n<max(1,m_nCalcLaneSu/2)+1; n++)
							{	
								
								double dMaxData = 0;
								long nQtyGirder	= pJ->GetQtyGirder(nVPos);
								for(long nGCur=0; nGCur<nQtyGirder; nGCur++)
								{
									strSTEELBOX_SAPOUT stSteelSapOut = pSB_SO->STEEL_SAPOUT[2][nL][n][nGCur];
									double dMaxSub = stSteelSapOut.m_dTotalMaxMin[0][0];
									//if(pJ->GetGirderData(nGCur,0)->m_dDist[1]!=0)
									CPlateGirderApp *pG = pDB->GetGirder(nGCur);
									CPlateBasicIndex *pBx = pG->GetBxOnJijum(nJ);
									CShoe ShoeFir = pG->m_Shoe[nJ];
									dMaxData += dMaxSub;								
								}
								if(dMax < dMaxData)
								{
									nMaxLane = n;					
									dMax = dMaxData;						
									nMaxLoadCase=nL;
									nLGM = 1;
								}
							}				
						}		
					}
					dLoadLive[2] = pSB_SO->STEEL_SAPOUT[nLGM+1][nMaxLoadCase][nMaxLane][nG].m_dDBMaxMin[nSide][0];
					if(m_bExistBodoInBrx) // �������� �����
					{					
						dLoadLive[2] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][5][2][nG].m_dDBMaxMin[nSide][0];					
					}
	//-				if(m_pIn->m_pAPierDataStd->m_bExistWindInBrx) // ǳ���� �����
	//-				{
	//-					dLoadLive[2] += pSB_SO->STEEL_SAPOUT[nGMaxGirder+1][6][0][nG].m_dDBMaxMin[nSide][0];					
	//-				}

					CBearingData *pBearing	= pJ->GetBearing(nVPos, i);
					pBearing->m_LoadLive[1]	= dLoadLive[1];
					pBearing->m_LoadLive[2]	= dLoadLive[2];

					m_pShareBridgeInfoStd->SetBearingReAction(nJ, nVPos, i, pBearing->m_LoadDead, pBearing->m_LoadDeadDW, pBearing->m_LoadLive[0], pBearing->m_LoadLive[1], pBearing->m_LoadLive[2], pBearing->m_LoadLive[3], pBearing->m_LoadLive[4], pBearing->m_LoadMax, nUpDn);
				}			
			}
		}
	}
}


void CAPlateDoc::InitSteelBox_SapOut()
{
	AhTPADelete(&m_pArrSteelBox_SapOut, (CSteelBox_SapOut*) 0);
}

void CAPlateDoc::OnCloseDocument() 
{			
	CMainFrame* pMFrm =(CMainFrame*)AfxGetMainWnd();
	pMFrm->m_MTIClientWnd.Detach();
	pMFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);	
	pMFrm->m_wndShortcutBar.SelectItem(pMFrm->m_wndShortcutBar.GetItem(0));	
	CDocument::OnCloseDocument();
}

BOOL CAPlateDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(!LDK ::aCheckLicenseHGKey())
		return 0;

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;		
	
	return TRUE;
}


