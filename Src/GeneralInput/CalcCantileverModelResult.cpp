// CalcCantileverModelResult.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "FileProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverModelResult dialog


CCalcCantileverModelResult::CCalcCantileverModelResult(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcCantileverModelResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcCantileverModelResult)
	//}}AFX_DATA_INIT
	m_bCheckLoadType		= FALSE;
	m_bCheckViewJoint		= TRUE;
	m_bChceckViewElement	= FALSE;
	m_bCheckViewOutValue	= FALSE;
	m_bCheckViewRestraint	= TRUE;
	m_nRadioView			= 1;
	m_nRadioForce			= 1;

	m_cAxis					= 'y';
	m_bGL					= TRUE;
	m_dTextSizeJoint		= 0.2;
	m_dTextSizeDataValue	= 0.2;
	m_dTextSizeElement		= 0.2;
}


void CCalcCantileverModelResult::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcCantileverModelResult)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_BKCHANGE, m_btnBkChange);
	DDX_Control(pDX, IDC_BUTTON_SAVEIMG, m_btnSaveImage);
	DDX_Control(pDX, IDC_BUTTON_FILE_READ, m_btnFileRead);
	DDX_Control(pDX, IDC_BUTTON_MAKE, m_btnMakeData);
	DDX_Control(pDX, IDC_BUTTON_RUN, m_btnMidasRun);
	DDX_Control(pDX, IDC_BUTTON_VIEW_CALC, m_btnViewCalc);

	DDX_Control(pDX, IDC_SLIDER_SCALE,    m_ctlSlider);
	DDX_Control(pDX, IDC_SLIDER_SCALE2,   m_ctlDispSlider);
	DDX_Control(pDX, IDC_COMBO_LOADCOMBO, m_ComboLoadCombo);
	DDX_Control(pDX, IDC_COMBO_LOADCASE,  m_ComboLoadCase);

	DDX_Check(pDX, IDC_CHECK_LOADTYPE,	m_bCheckLoadType);
	DDX_Check(pDX, IDC_CHECK_JOINT,		m_bCheckViewJoint);
	DDX_Check(pDX, IDC_CHECK_ELEMENT,	m_bChceckViewElement);
	DDX_Check(pDX, IDC_CHECK_OUTVALUE,	m_bCheckViewOutValue);
	DDX_Check(pDX, IDC_CHECK_RESTRAINT, m_bCheckViewRestraint);	
	DDX_Radio(pDX, IDC_RADIO_V22,		m_nRadioForce);
	DDX_Radio(pDX, IDC_RADIO_VIEW_YZ,	m_nRadioView);

	DDX_Text(pDX, IDC_FILE_PATH, m_szPath);
	DDX_Text(pDX, IDC_EDIT_JOINTNUMBER_SIZE, m_dTextSizeJoint);
	DDX_Text(pDX, IDC_EDIT_DATAVALUE_SIZE, m_dTextSizeDataValue);
	DDX_Text(pDX, IDC_EDIT_ELEMENT_SIZE, m_dTextSizeElement);
}

BEGIN_MESSAGE_MAP(CCalcCantileverModelResult, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcCantileverModelResult)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_CPN_XT_SELENDOK(IDC_BUTTON_BKCHANGE, OnButtonBackGroundColr)
	ON_BN_CLICKED(IDC_BUTTON_SAVEIMG, OnButtonSaveImageFile)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADCOMBO, OnSelchangeComboLoadcase)
	ON_BN_CLICKED(IDC_CHECK_LOADTYPE, OnCheckLoadtype)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADCASE, OnSelchangeComboLoadtype)
	
	ON_BN_CLICKED(IDC_RADIO_V22       , OnCheck_Shear22)
	ON_BN_CLICKED(IDC_RADIO_M22       , OnCheck_Moment22)
	ON_BN_CLICKED(IDC_RADIO_AXIAL     , OnCheck_AxialForce)
	ON_BN_CLICKED(IDC_RADIO_DISP      , OnCheck_Displace)
	ON_BN_CLICKED(IDC_RADIO_V33       , OnCheck_Shear33)
	ON_BN_CLICKED(IDC_RADIO_M33       , OnCheck_Moment33)
	ON_BN_CLICKED(IDC_RADIO_TORSION   , OnCheck_AxialTorsion)
	ON_BN_CLICKED(IDC_RADIO_REAC	  , OnCheck_Reaction)

	ON_BN_CLICKED(IDC_CHECK_JOINT    , OnCheck_NodeNum)
	ON_BN_CLICKED(IDC_CHECK_ELEMENT  , OnCheck_MemberNum)
	ON_BN_CLICKED(IDC_CHECK_RESTRAINT, OnCheck_Boundary)
	ON_BN_CLICKED(IDC_CHECK_OUTVALUE , OnCheck_ResultValue)

	ON_BN_CLICKED(IDC_RADIO_VIEW_YZ, OnCheck_Plan_YZ)
	ON_BN_CLICKED(IDC_RADIO_VIEW_XZ, OnCheck_Plan_XZ)
	ON_BN_CLICKED(IDC_RADIO_VIEW_XY, OnCheck_Plan_XY)
	ON_BN_CLICKED(IDC_RADIO_VIEW_3D, OnCheck_Plan_3D)

	ON_BN_CLICKED(IDC_BUTTON_FILE_READ, OnButtonReadOutputFile)
	ON_BN_CLICKED(IDC_BUTTON_MAKE,		OnButtonMidasFileMake)
	ON_BN_CLICKED(IDC_BUTTON_RUN,		OnButtonMiaasRun)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_CALC, OnButtonResultViewTable)

	ON_MESSAGE(WM_RUNTHREAD, OnRunThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcCantileverModelResult message handlers

BOOL CCalcCantileverModelResult::IsValid()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	BOOL bValid = pDB->m_bTendonInputComplete;

	if (!bValid)
		m_bUsedFlag = FALSE;

	return bValid;
}

void CCalcCantileverModelResult::OnPreInitDialog()
{
	m_btnBkChange.SetColor(m_pView->GetDomBkColor());
/*
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_MIDDLE_CENTER);
	SetResize(IDC_RADIO_V22, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_M22, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_AXIAL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_DISP, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_V33, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_M33, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_TORSION, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_REAC, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_CHECK_JOINT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_ELEMENT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_RESTRAINT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_OUTVALUE, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_RADIO_VIEW_YZ, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_RADIO_VIEW_XZ, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_RADIO_VIEW_XY, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);
	SetResize(IDC_RADIO_VIEW_3D, SZ_MIDDLE_CENTER, SZ_MIDDLE_CENTER);

	SetResize(IDC_STATIC9, SZ_TOP_CENTER, SZ_MIDDLE_RIGHT);
	SetResize(IDC_STATIC10, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC11, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_STATIC12, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_JOINTNUMBER_SIZE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_ELEMENT_SIZE, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_EDIT_DATAVALUE_SIZE, SZ_TOP_CENTER, SZ_TOP_CENTER);

	SetResize(IDC_BUTTON_BKCHANGE , SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SAVEIMG , SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC7, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC8, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SLIDER_SCALE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SLIDER_SCALE2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_FILE_PATH, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FILE_READ , SZ_TOP_RIGHT, SZ_TOP_RIGHT);
*/

	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC9, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_BUTTON_MAKE , SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_RUN  , SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_VIEW_CALC  , SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetLoadComboCombo();
	SetLoadCaseCombo();
	SetFileName();
}

void CCalcCantileverModelResult::DrawInputDomyunView(BOOL bZoomAll)
{
	UpdateData(TRUE);

	CFEMManage	*pFEM	= m_pStd->m_pCalcStd->GetCantileverFemData(0);
	CDomyun		*pDom	= m_pView->GetDomyun();
	long	nLoadCombo	= m_ComboLoadCombo.GetCurSel();
	long	nLoadCase	= m_ComboLoadCase.GetCurSel();


	pDom->ClearEtt(TRUE);
	
	
	m_pStd->m_FEMDraw.ChangeFEM(pFEM);
	m_pStd->m_FEMDraw.InsertDomyunPoint();
	m_pStd->m_FEMDraw.SetLegendPow(FALSE);
	m_pStd->m_FEMDraw.SetRestraintScale(0.5);
	m_pStd->m_FEMDraw.SetJointSize(0.05);

	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_NUMBER, m_bCheckViewJoint);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_NUMBER, m_bChceckViewElement);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_RENDER_RESTRAINT, m_bCheckViewRestraint);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE, m_bCheckViewOutValue);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_LOADCASE, m_bCheckLoadType);


	if(m_bCheckLoadType && nLoadCase>-1)	m_pStd->m_FEMDraw.SetLoadCase(nLoadCase);	
	if(!m_bCheckLoadType && nLoadCombo>-1)	m_pStd->m_FEMDraw.SetLoadCase(nLoadCombo);	
	
	if(m_nRadioForce==6)
	{
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_DISPLACEMENT,TRUE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_DISPLACEMENT,TRUE);	
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_REACTION,FALSE);				
		
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE,FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE,FALSE);
	}
	else if(m_nRadioForce==7)
	{
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_DISPLACEMENT,FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_DISPLACEMENT,FALSE);	
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_REACTION,TRUE);
		m_pStd->m_FEMDraw.SetViewForceType(m_nRadioForce);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE,FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE,FALSE);
	}
	else
	{
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_DISPLACEMENT,FALSE);
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_DISPLACEMENT,FALSE);	
		m_pStd->m_FEMDraw.SetFlag(FEMVIEW_REACTION,FALSE);				
		CFEMManage *pFEM = m_pStd->m_FEMDraw.GetFEMManage();
		if(pFEM && pFEM->IsOutputLoaded())
		{		
			m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE,TRUE);
		}
		m_pStd->m_FEMDraw.SetViewForceType(m_nRadioForce);
	}		
	m_pView->SetAxis(m_cAxis);
	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}

void CCalcCantileverModelResult::SetDataSave()
{
	UpdateData(TRUE);
	
	CAPlateCalcStd   *pCalcStd = m_pStd->m_pCalcStd;
	pCalcStd->m_nScaleGrade = m_ctlSlider.GetPos();	
}

void CCalcCantileverModelResult::SetDataDefault()
{
}

void CCalcCantileverModelResult::SetDataInit()
{
	if(m_bCheckLoadType) 
	{
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LOADCOMBO)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LOADCOMBO)->EnableWindow(TRUE);
	}

	m_ctlSlider.SetPos(m_pStd->m_FEMDraw.GetScaleGrade());
	m_ctlDispSlider.SetPos((int)m_pStd->m_FEMDraw.GetDispScale());

	SetLoadComboCombo();
	SetLoadCaseCombo();

	UpdateData(FALSE);
}

void CCalcCantileverModelResult::SetLoadComboCombo()
{
	long nComboSelect = 0;
	m_ComboLoadCombo.ResetContent();

	CFEMManage *pFEM = m_pStd->m_pCalcStd->GetCantileverFemData(0);
	if(!pFEM) return;

	for(long n=0; n<pFEM->m_szLCName.GetSize(); n++)
	{
		m_ComboLoadCombo.AddString(pFEM->m_szLCName[n]);
		if(pFEM->m_szLCName[n]=="ENVULT MAX")
			nComboSelect = n;
	}
	m_ComboLoadCombo.SetCurSel(nComboSelect);
	
	
	UpdateData(FALSE);
}

void CCalcCantileverModelResult::SetLoadCaseCombo()
{
	CFEMManage *pFEM = m_pStd->m_pCalcStd->GetCantileverFemData(0);
	if(!pFEM) return;
	
	m_ComboLoadCase.ResetContent();	
	for(long n=0; n<pFEM->m_szLCName.GetSize() - pFEM->m_nOutputCaseAddSize; n++)
	{
		m_ComboLoadCase.AddString(pFEM->m_szLCName[n]);
	}
	m_ComboLoadCase.SetCurSel(0);	
}

void CCalcCantileverModelResult::SetFileName()
{
	CString szPathWork	= AfxGetApp()->GetProfileString("Afx Directory", "Work Directory");
	
	m_szPath = m_pStd->m_pDoc->GetPathName();
	int nFine = m_szPath.ReverseFind('.');
	if( nFine>=0)   m_szPath = m_szPath.Left(nFine);
	
	nFine = m_szPath.ReverseFind('\\');	
	CString szFileName = m_szPath.Mid(nFine+1);
	szFileName.Replace(" ","_");
	m_szPath = m_szPath.Left(nFine+1) + szFileName;
	
	UpdateData(FALSE);
}

void CCalcCantileverModelResult::OnSelchangeComboLoadcase()
{
	DrawInputDomyunView(TRUE);	
}

void CCalcCantileverModelResult::OnButtonBackGroundColr()
{
	COLORREF clr = m_btnBkChange.GetColor();
	m_pStd->m_FEMDraw.SetBkColor(clr);
	GetView()->Invalidate(FALSE);
}

void CCalcCantileverModelResult::OnButtonSaveImageFile()
{
	CDC *pDC;				
	CDC *pBitDC = new CDC;
	CRect rect;
	CBitmap bmp, *pOldBitmap;	
	CImage Image;
	
	CFileDialog dlg(FALSE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"JPG 파일 (*.jpg)|*.jpg|BMP 파일 (*.bmp)|*.bmp||", m_pView);
	dlg.m_ofn.lpstrTitle = "파일 저장";
	if (dlg.DoModal() != IDOK) 
	{
		delete pBitDC;
		return;
	}
	
	ShowWindow(SW_HIDE);
	pDC = m_pView->GetDC();
	//m_pView->OnDrawMain(pDC);
	CString szFileName = dlg.GetPathName();
	m_pView->GetClientRect(&rect);
	pBitDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);				
	pOldBitmap = pBitDC->SelectObject(&bmp);	
	//m_pView->OnDrawMain(pDC);	// 이미지로 저장할 화면	
	pBitDC->BitBlt(0,0,rect.right,rect.bottom,pDC,0,0,SRCCOPY);	
	if(!Image.CopyFromBmp(&bmp))
	{
		AfxMessageBox("지원할수 없는 형식입니다 !");
		delete pBitDC;
		return;
	}	
	Image.SaveImage(szFileName.GetBuffer(szFileName.GetLength()));	
	pBitDC->SelectObject(pOldBitmap);
	
	ReleaseDC(pDC);
	delete pBitDC;	
	ShowWindow(SW_SHOW);
}

void CCalcCantileverModelResult::OnCheckLoadtype() 
{
	UpdateData();
	if(m_bCheckLoadType) 
	{
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LOADCOMBO)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LOADCOMBO)->EnableWindow(TRUE);
	}	
	DrawInputDomyunView(TRUE);	
}

void CCalcCantileverModelResult::OnSelchangeComboLoadtype() 
{
	DrawInputDomyunView(TRUE);	
}

void CCalcCantileverModelResult::OnCheck_Shear22()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Moment22()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_AxialForce()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Displace()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Shear33()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Moment33()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_AxialTorsion()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Reaction()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_NodeNum()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_MemberNum()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Boundary()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_ResultValue()
{
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Plan_YZ()
{
	m_cAxis = 'x';
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Plan_XZ()
{
	m_cAxis = 'y';
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Plan_XY()
{
	m_cAxis = 'z';
	DrawInputDomyunView(TRUE);
}
void CCalcCantileverModelResult::OnCheck_Plan_3D()
{
	m_cAxis = 'd';
	DrawInputDomyunView(TRUE);
}

void CCalcCantileverModelResult::OnButtonReadOutputFile()
{
	CString	szOutExt  = "*.anl|*.*";
	CString	szComment = "MIDAS Output File(*.anl)|*.anl|모든파일 (*.*)|*.*||";
	
	CString szPath = "";
	CFileDialog dlg(TRUE, szOutExt, "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szComment, m_pView);
	dlg.m_ofn.lpstrTitle = "MIDAS 출력파일 불러오기";
	
	if (dlg.DoModal() == IDOK) szPath = dlg.GetPathName();
	else return;
	
	CString szInExt = ".mct";
	
	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];		  	
	char	inputname[_MAX_EXT];		  	
	_SPLITPATH_S(szPath,drive,dir,fname,ext);
	_STRCPY_S(inputname, sizeof(inputname), drive);
	_STRCAT_S(inputname, sizeof(inputname), dir);
	_STRCAT_S(inputname, sizeof(inputname), fname);
	_STRCAT_S(inputname, sizeof(inputname), szInExt);

	// strcpy(inputname,drive);
	// strcat(inputname,dir);
	// strcat(inputname,fname);
	// strcat(inputname,szInExt);
	CString szInputPath(inputname);
	m_szPath = szPath;
	
	CFileProgressDlg Dlg(m_pView);
	Dlg.m_pParent = this;
	Dlg.m_nType = 1;
	Dlg.DoModal();
	m_pProgressDlg = NULL;
}

LRESULT CCalcCantileverModelResult::OnRunThread(WPARAM wp, LPARAM lp)
{
	CAPlateCalcStd  *pCalcStd	= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM		= pCalcStd->GetCantileverFemData(0);
	CLoadMidasOutput LoadOut(pFEM, &m_pProgressDlg->m_ctlProgress);	

	try
	{
		if(LoadOut.LoadData(m_szPath))
		{
			m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
			m_pStd->m_FEMDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
			m_ComboLoadCombo.SetCurSel(0);
	
			SetLoadComboCombo();
			SetLoadCaseCombo();
			DrawInputDomyunView(TRUE);
			UpdateData(FALSE);
		}
	}
	catch(...)
	{
		AfxMessageBox(m_szPath + "파일이 비정상적이거나 존재하지 않습니다.", MB_ICONSTOP);	
	}
	return 0;
}

void CCalcCantileverModelResult::OnButtonMidasFileMake()
{
	CAPlateCalcStd	*pCalcStd		= m_pStd->m_pCalcStd;
	
	UpdateData();
	SetDataSave();
	
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString szAEdit = AfxGetApp()->GetProfileString("Settings", "Editor");
	CFileFind finder;
	if ( FALSE == finder.FindFile(szAEdit) )
	{
		// 설정 파일을 못찾으면 AEDIT.EXE 파일로 오픈한다.
		szAEdit = szPath + "\\AEdit.exe";
	}

	CString szFileName	= m_pStd->m_pDoc->GetPathName();
	szFileName  = szFileName.Left(szFileName.ReverseFind('.'));
	
	szFileName += "_바닥판";
	
	CFEMManage	*pFEM	= pCalcStd->GetCantileverFemData(0);
	
	CString	szNameTemp	= szFileName + ".mct";
	pFEM->SaveAsMidas(szNameTemp);
	ShellExecute(NULL,NULL,szAEdit, "\"" + szNameTemp + "\"",NULL,SW_SHOWNORMAL);
	
	CWnd *pWnd;
	for(long k=0; k<4; k++)
	{			
		if(GetMyselfWindow(&pWnd)) break;
		Sleep(500);
	}
}

BOOL CCalcCantileverModelResult::FileExist(LPCTSTR lpPathName)
{
	HANDLE hFind;	
    WIN32_FIND_DATA fd;	
    if ((hFind = ::FindFirstFile (lpPathName, &fd)) != INVALID_HANDLE_VALUE) 
	{	
        FindClose (hFind);
        return TRUE;
    }	
    return FALSE;
}

void CCalcCantileverModelResult::OnButtonMiaasRun()
{
	CString szPath		= AfxGetApp()->GetProfileString("Settings", "MIDAS Civil Directory");		
	szPath += "\\CVLw.exe";
	
	BOOL bIsFile = FileExist(szPath);
	
	if (bIsFile) ShellExecute(NULL,"open", "\"" + szPath + "\"", NULL, NULL,SW_SHOWNORMAL);
	else MessageBox("현재 디렉토리에 Midas Civil 프로그램이 없습니다.", "Midas Civil 프로그램 에러", MB_OK);
}

void CCalcCantileverModelResult::OnButtonResultViewTable()
{
	CDataManage		*pDataManage	= m_pStd->m_pDataManage;
	CAPlateCalcStd	*pCalcStd		= m_pStd->m_pCalcStd;
	CFEMManage		*pFEM			= pCalcStd->GetCantileverFemData(0);	
	CStringArray	Arr;
	
	
	if(!pFEM->IsOutputLoaded())
	{
		AfxMessageBox("계산 결과가 존재하지 않습니다", MB_OK | MB_ICONSTOP);
		return;
	}
	
	pCalcStd->GetCantileverElementForceOutput(Arr);
	pCalcStd->GetCantileverNodeReactionOutput(Arr);
 	m_pStd->OutputViewOnAView(Arr);
}

void CCalcCantileverModelResult::OnDestroy() 
{
	CGeneralBaseDlg::OnDestroy();
	
	m_pView->SetGL(FALSE);	
}