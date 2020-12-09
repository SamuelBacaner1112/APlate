// APlate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "APlate.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "APlateDoc.h"
#include "APlateView.h"
#include "Splash.h"

#include <afxinet.h>
#include <direct.h> 
#include "Shlwapi.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAPlateApp
const TCHAR szAPlateClass[] = _T("APlateClass");
const CString szAPlateName	= _T("APlate2010");
 
BEGIN_MESSAGE_MAP(CAPlateApp, CMSDIWinApp)
	//{{AFX_MSG_MAP(CAPlateApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CMSDIWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CMSDIWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CMSDIWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPlateApp construction

#define LOCK_FEATURE_ID 4200
CAPlateApp::CAPlateApp()
{
	m_strFixedDir = _T("");	
	m_bFirFileOpen = TRUE;
	
	::aLockInit(UNIQ_ID_APLATE, SPLOCK_APLATE_DID, SPADDR_SL_APLATE, NULL, 70, BUILDDATE, "login_APlate.swf");

	LDK::SLockInitInfo LockInfo = {
	"APlate",
	UNIQ_ID_APLATE,
	PID_APLATE,
	LDK::aLoginDlgProc ,                          
	BUILDDATE,
	LICENSE_NULL};

	LDK::aLockInit (LockInfo);
}

CAPlateApp::~CAPlateApp()
{	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAPlateApp object

CAPlateApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAPlateApp initialization

BOOL CAPlateApp::InitInstance()
{
	CString errMsg = _T("오류가 발생했습니다.");
	BEGIN_AEXCEPTION();

	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
	AfxEnableControlContainer();

	AfxGetModuleState()->m_dwVersion = 0x0601;
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("HangilIT"));

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)
	//SetFixDirectory();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

//	unsigned short int uModuleValue;
//	if(!aCheckLock(HANGIL_APLATE, SPADDR_MF_APLATE, SPLOCK_APLATE_RCODE, SPLOCK_APLATE_RCODE, uModuleValue, TRUE, FALSE))
//	{                              
//		if (!::aLockCheckResultAndNotice (NULL, FALSE))
//			return FALSE ;
//	}
//	LDK::aSetLockSerial (aGetLockProductSerial());

	unsigned short int uModuleValue; // 2015. 07. 28. Software Lock 작업, 김형준
	if(aCheckLock(HANGIL_APLATE, SPADDR_MF_APLATE, SPLOCK_APLATE_RCODE, SPLOCK_APLATE_RCODE, uModuleValue, TRUE, FALSE))
	{		
		if (!::aLockCheckResultAndNotice(NULL, FALSE))
			return FALSE;

		LDK::aSetLockSerial(aGetLockProductSerial());
	}
	else if (LDK::aLockLType(LDK::eSLnRegistration) && !LDK::aLockLogin(LOCK_FEATURE_ID))
	{
		if (!LDK::aLDKNoticeLock(LDK::aGetLockInfo(), LDK::aGetLockResult()))
			return FALSE;
	}

	CMultiDocTemplate* pDocTemplate;

//	pDocTemplate = new CMultiDocTemplate(CGeneralBaseStd::IsModuleTUGirder() ? IDR_APLATETYPE_FULL : IDR_APLATETYPE_NOT_TU,
	pDocTemplate = new CMultiDocTemplate(IDR_APLATETYPE_FULL,
		RUNTIME_CLASS(CAPlateDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAPlateView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(FALSE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (aIsLockOK () || LDK::aIsLogIn()) // 2015. 07. 28. Software Lock 작업, 김형준
	{
		if(LDK ::aCheckLicenseHGKey())
		{
			if (!ProcessShellCommand (cmdInfo))
				return FALSE ;
		}
	}
	
	// The main window has been initialized, so show and update it.	
	int bMax = GetProfileInt("Size","Zoomed",-1);
	if(bMax != 0)	// APlate 처음실행시 or Maximized로 종료된 경우	
		pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);					
	else
		pMainFrame->ShowWindow(m_nCmdShow);			// default
	pMainFrame->UpdateWindow();

	//==============================================================================================
	//
	//  Lock Check Module
	//
	//==============================================================================================
	CDocument*  pDoc = pMainFrame->GetActiveDocument();
	
	//----------------------------------------------------------------------------------------------
	//  STEP1: Lock Check 실패시, 종료
	//----------------------------------------------------------------------------------------------
	if (LDK::aIsLogIn()) // 2015. 07. 28. Software Lock 작업, 김형준
	{
		if (!LDK::aLockCheckResultAndNotice(GetDocument(), FALSE))
			return FALSE;
	}
	else
	{
		if (!::aLockCheckResultAndNotice(GetDocument(), FALSE))
			return FALSE;
	}
	
	//----------------------------------------------------------------------------------------------	
	//  STEP2: Lock 검사 Timer 가동	
	//----------------------------------------------------------------------------------------------	
	pMainFrame->SetTimer(SPLOCK_TIMER_ID, SPLOCK_TIMER_INTERVAL, NULL);	
		
	CString szRunTime ;
	if(LDK::aCheckLicenseHGKey()) // 2015. 07. 28. Software Lock 작업, 김형준
	{
		if (LDK::aIsLogIn())
		{
			LDK::aUpdateCheck();
		}
		else
		{
			aUpdateCheck("APlate" , atoi( GetProfileString("Program Option" , "CheckVersion" ,"")), szRunTime);
			if(!szRunTime .IsEmpty())
				WriteProfileString("Settings" , "LastRunDate", szRunTime);
		}
	}

	LDK::aCheckLoseLockAndDestoryKey();

	if(!LDK ::aCheckLicenseHGKey())
		pMainFrame->PostMessage (WM_COMMAND, ID_CREATE_LICENSE);

	AfxBeginThread(aSendRunInfo,NULL);	
	///////////////////////////////////////////////////////////////////////////
	
	END_AEXCEPTION(errMsg);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAPlateApp message handlers

void CAPlateApp::PreLoadState()
{
}

// App command to run the dialog
void CAPlateApp::OnAppAbout()
{
	CString sVersion = GetStringVersion();
	CString sLockInfo = "";		// ABeamDeck는 별도의 Edition이 없음(Standard, Basic 구분 없음)

	CDialogAbout	Dlg;
	Dlg.SetProgramName(szAPlateName);
	Dlg.SetLockInfo(sLockInfo);
	Dlg.SetEtcInfo("");
	Dlg.SetVersion(sVersion);
	Dlg.DoModal();
}

CString CAPlateApp::GetStringVersion()
{
	CString sVersion = VERSION;
	// Master Lock일 경우 빌드넘버도 보여준다. 
	if(LDK::aGetModuleNumber() == SPLOCK_MASTER_LID)
	{
		sVersion.Format("%s(%s)", VERSION, BUILDNUMBER);
	}

	return sVersion;
}

BOOL CAPlateApp::PreTranslateMessage(MSG* pMsg) 
{
	// CG: The following line was added by the Splash Screen component.
	if(CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;
	return CMSDIWinApp::PreTranslateMessage(pMsg);
}

int CAPlateApp::ExitInstance() 
{
	aLockExit();
	LDK::aLockLogout();

	//SHCBCleanUp ();
	return CMSDIWinApp::ExitInstance();
}

void CAPlateApp::SetFixDirectory()
{
	CWinApp* pApp = AfxGetApp();
	CString csSection       = "Afx Directory";
	CString csFixedItem    = "Fixed Directory";
	CString csWorkItem    = "Work Directory";
	
	////////////////////////////////////////////////////////////////
	// Set AShop Directory(Fixed Directory) 
	// Only Write Current Path in Registy
	CString strFixedDir = pApp->GetProfileString(csSection, csFixedItem);

	::PathRemoveBackslash(strFixedDir.GetBufferSetLength(strFixedDir.GetLength()));
	
	pApp->WriteProfileString(csSection,	csFixedItem,	strFixedDir);

	BOOL bFirst = strFixedDir.GetLength()==0 ? TRUE : FALSE;
	if(bFirst)
	{
		char Buf[1024];
		::GetCurrentDirectory( 1024,Buf);
		pApp->WriteProfileString(csSection, csFixedItem, Buf);
		strFixedDir = Buf;
	}
	
	//////////////////////////////////////////////////////////////////
	// Set Work Directory
	
	CString csWork = pApp->GetProfileString(csSection, csWorkItem);
	if(csWork=="")
	{
		CString cs = "C:\\";
		_chdrive( 3 );
		_chdir((const char *)cs.GetBuffer(cs.GetLength()));		
		cs = "APlateWork";
		_mkdir(cs);
		csWork = (CString)"C:\\" + "APlateWork";
		pApp->WriteProfileString(csSection, csWorkItem, csWork);
	}
	_chdrive( 3 );
	int v = _chdir((const char *)csWork.GetBuffer(csWork.GetLength()));
	
    if( IsRegFileExt()==FALSE )
	{
		CString sValue[4];
		sValue[0] = ".brp";
		sValue[1] = "Steel Plate 파일";
		sValue[2] = "교량파일";
		sValue[3] = strFixedDir;
		SetRegFileExt(sValue, 1);
	}
}

void CAPlateApp::CutLastBackSlash(CString& cs)
{
	long nLen = cs.GetLength();
	if(nLen < 1) return;
	if(	cs[nLen-1] == '\\')
		cs = cs.Left(nLen-1);
}

void CAPlateApp::OnFileOpen() 
{
	CString sPath;
	
	if(m_bFirFileOpen)
	{
		sPath = GetProfileString("Afx Directory", "Work Directory");
		CutLastBackSlash(sPath);
		sPath += "\\";
		SetCurrentDirectory(sPath);
	}
	
	CBitmap LogoBitmap;
	LogoBitmap.LoadBitmap(IDB_BITMAP_LOGO);
	CFileDialog dlg(TRUE, "교량 파일 불러오기","", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "APlate Files (.brp)|*.brp|All Files (*.*)|*.*||", NULL);

	//CShFileDialog dlg("교량 파일 불러오기",FALSE, TRUE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
//		"APlate Files (.brp)|*.brp|All Files (*.*)|*.*||");	

	
//	dlg.SetLogoBitmap(&LogoBitmap);
	if(m_bFirFileOpen)
		dlg.m_ofn.lpstrInitialDir = sPath.GetBuffer( sPath.GetLength() );
	if (dlg.DoModal() != IDOK) return;
	m_bFirFileOpen = FALSE;
	
	CString szPathName = dlg.GetPathName();
	
//	if(dlg.GetPage() == CShFileDialog::BCGFileRecent)
//	{
//		szPathName = dlg.GetRecentFilePath();
//	}
	
	OpenDocumentFile(szPathName);
	
	DeleteWmfFile();	
}

BOOL CAPlateApp::IsRegFileExt() const
{	
	HKEY hSecKey = HKEY_CLASSES_ROOT;
	HKEY nExtKey = NULL;
	CString strValue;
	DWORD dwType, dwCount,dw;
	
	if( RegCreateKeyEx(hSecKey,"APlateFile", 0,REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_WRITE|KEY_READ,NULL,&nExtKey,&dw) == ERROR_SUCCESS)
	{
		LONG lResult = RegQueryValueEx(nExtKey, "",
			NULL, &dwType,	NULL, &dwCount);
		
		if (lResult == ERROR_SUCCESS)
		{
			lResult = RegQueryValueEx(nExtKey, "", NULL, &dwType,
				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
	}
	
	if(hSecKey != NULL) RegCloseKey(hSecKey);
	
	if(	strValue.GetLength() > 0) return TRUE;
	return FALSE;
}

void CAPlateApp::SetRegFileExt(CString sValue[4], long nIcon, BOOL bOpen)
{
	CWinApp* pApp = AfxGetApp();
	HKEY nExtKey = NULL;
	HKEY nDefaultIconkey = NULL;
	HKEY nShellKey = NULL;
	HKEY nOpenKey = NULL;
	HKEY nCommandKey = NULL;
	HKEY nPrintKey = NULL;
	
	DWORD dw;
	if( RegCreateKeyEx(HKEY_CLASSES_ROOT,sValue[0], 0,REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_WRITE|KEY_READ,NULL,&nExtKey,&dw) == ERROR_SUCCESS)
	{
		LPCTSTR lpszValue = (LPCTSTR)sValue[1];
		RegSetValueEx(nExtKey,NULL,	0,REG_SZ,(LPBYTE)lpszValue,
			(lstrlen(lpszValue)+1)*sizeof(TCHAR));
	}
	if (nExtKey != NULL)	RegCloseKey(nExtKey);
	
	nExtKey = NULL;
	CString szIconKey="";
	if(nIcon!=0) szIconKey.Format(",%d",nIcon);
	if( RegCreateKeyEx(HKEY_CLASSES_ROOT,sValue[1], 0,REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_WRITE|KEY_READ,NULL,&nExtKey,&dw) == ERROR_SUCCESS)
	{
		LPCTSTR lpszValue = (LPCTSTR)sValue[2];
		RegSetValueEx(nExtKey,NULL,	0,REG_SZ,(LPBYTE)lpszValue,
			(lstrlen(lpszValue)+1)*sizeof(TCHAR));
		
		if( RegCreateKeyEx(nExtKey,"DefaultIcon", 0,REG_NONE,REG_OPTION_NON_VOLATILE,
			KEY_WRITE|KEY_READ,NULL,&nDefaultIconkey,&dw) == ERROR_SUCCESS)
		{
			CString szDir = GetFixedDir()+"\\APlate2010.exe";
			szDir += szIconKey;
			LPCTSTR lpszValue = (LPCTSTR)szDir;
			RegSetValueEx(nDefaultIconkey,NULL,	0,REG_SZ,(LPBYTE)lpszValue,
				(lstrlen(lpszValue)+1)*sizeof(TCHAR));
		}
		if (nDefaultIconkey != NULL)	RegCloseKey(nDefaultIconkey);
		
		nDefaultIconkey = NULL;
		if(bOpen)
		{			
			if(sValue[3].GetLength() > 0)
				if( RegCreateKeyEx(nExtKey,"shell", 0,REG_NONE,REG_OPTION_NON_VOLATILE,
					KEY_WRITE|KEY_READ,NULL,&nShellKey,&dw) == ERROR_SUCCESS)
				{
					if( RegCreateKeyEx(nShellKey,"open", 0,REG_NONE,REG_OPTION_NON_VOLATILE,
						KEY_WRITE|KEY_READ,NULL,&nOpenKey,&dw) == ERROR_SUCCESS)
						if( RegCreateKeyEx(nOpenKey,"command", 0,REG_NONE,REG_OPTION_NON_VOLATILE,
							KEY_WRITE|KEY_READ,NULL,&nCommandKey,&dw) == ERROR_SUCCESS)
						{
							CString sPath(sValue[3]),sFmt;
							sFmt.Format(".EXE\" %c%%1%c",'"','"');
							sPath = "\"" + sPath + (CString)"\\"+pApp->m_pszAppName + sFmt;
							LPCTSTR lpszValue = (LPCTSTR)sPath;
							RegSetValueEx(nCommandKey,NULL,	0,REG_SZ,(LPBYTE)lpszValue,
								(lstrlen(lpszValue)+1)*sizeof(TCHAR));
						}
						
						if( RegCreateKeyEx(nShellKey,"print", 0,REG_NONE,REG_OPTION_NON_VOLATILE,
							KEY_WRITE|KEY_READ,NULL,&nPrintKey,&dw) == ERROR_SUCCESS)
							if( RegCreateKeyEx(nPrintKey,"command", 0,REG_NONE,REG_OPTION_NON_VOLATILE,
								KEY_WRITE|KEY_READ,NULL,&nCommandKey,&dw) == ERROR_SUCCESS)
							{
								CString sPath(sValue[3]),sFmt;
								sFmt.Format(".EXE /P %c%%1%c",'"','"');
								sPath += (CString)"\\"+pApp->m_pszAppName + sFmt;
								LPCTSTR lpszValue = (LPCTSTR)sPath;
								RegSetValueEx(nCommandKey,NULL,	0,REG_SZ,(LPBYTE)lpszValue,
									(lstrlen(lpszValue)+1)*sizeof(TCHAR));
							}
							
				}
		}
	}
	
	if (nExtKey != NULL)	RegCloseKey(nExtKey);
	if (nDefaultIconkey != NULL)	RegCloseKey(nDefaultIconkey);
	if (nShellKey != NULL)	RegCloseKey(nShellKey);
	if (nOpenKey != NULL)	RegCloseKey(nOpenKey);
	if (nCommandKey != NULL)	RegCloseKey(nCommandKey);
	if (nPrintKey != NULL)	RegCloseKey(nPrintKey);
}

void CAPlateApp::DeleteWmfFile()
{
	CString strPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	SetCurrentDirectory(strPath);
	
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	BOOL bResult = TRUE;
	strPath += "\\*.wmf";
	
	hFind = FindFirstFile(strPath, &wfd);
	while(bResult)
	{
		BOOL bDelete = DeleteFile(wfd.cFileName);		
		bResult = FindNextFile(hFind,&wfd);
	}
	FindClose(hFind);
}

UINT CAPlateApp::GetMRUListSize()
{
	UINT nListSize = 0;
	UINT nExistSize = 0;
	
	CString strName = _T("");
	if(m_pRecentFileList==NULL)
		return nExistSize;
	nListSize = m_pRecentFileList->GetSize();
	
	for(UINT i=0; i<nListSize; i++)
	{
		strName = GetMRUName(i, FALSE);
		
		if(strName!=_T(""))
			nExistSize++;
	}
	
	return nExistSize;
}

CString CAPlateApp::GetMRUName(UINT nIndex, BOOL bFullPath)
{
	CString strName = _T("");
	if(m_pRecentFileList==NULL)
		return strName;
	
	if(bFullPath)
	{
		strName = (*m_pRecentFileList)[nIndex];
	}
	else
	{
		m_pRecentFileList->m_nMaxDisplayLength = 0;
		m_pRecentFileList->GetDisplayName(strName, nIndex, _T(""), 0);
	}
	
	return strName;
}