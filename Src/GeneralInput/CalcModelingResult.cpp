// CalcModelingResult.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingResult dialog


CCalcModelingResult::CCalcModelingResult(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CCalcModelingResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcModelingResult)
	m_sSapInputFileName	= _T("");
	m_bShowJoint		= TRUE;
	m_bShowElement		= FALSE;
	m_bShowRestraint	= TRUE;
	m_bShowResultData	= FALSE;
	m_dTextSizeJoint	= 0.2;
	m_dTextSizeDataValue= 0.2;
	m_dTextSizeElement	= 0.2;
	m_nSolveType		= 0;
	m_cAxis				= 'd';
	m_bGL				= TRUE;
	m_nRadioForce		= 1;
	//}}AFX_DATA_INIT
}


void CCalcModelingResult::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcModelingResult)
	DDX_Control(pDX, IDC_COMBO_RESULT_TYPE, m_comboResultType);
	DDX_Control(pDX, IDC_BUTTON_BKCHANGE, m_btnBkChange);
	DDX_Control(pDX, IDC_F_OUTPUT_SLIDER2, m_ctrlSliderDispScale);
	DDX_Control(pDX, IDC_SLIDER_ALPHA, m_ctrlSliderAlpha);
	DDX_Control(pDX, IDC_F_OUTPUT_SLIDER1, m_ctrlSliderScale);
	DDX_Text(pDX, IDC_EDIT_RESULT_PATH, m_sSapInputFileName);
	DDX_Control(pDX, IDC_EDIT_RESULT_PATH, m_ctrlEdit1);
	DDX_Check(pDX, IDC_F_OUTPUT_CHECK1, m_bShowJoint);
	DDX_Check(pDX, IDC_F_OUTPUT_CHECK2, m_bShowElement);
	DDX_Check(pDX, IDC_F_OUTPUT_CHECK3, m_bShowRestraint);
	DDX_Check(pDX, IDC_F_OUTPUT_CHECK4, m_bShowResultData);
	DDX_Text(pDX, IDC_EDIT_JOINTNUMBER_SIZE, m_dTextSizeJoint);
	DDX_Text(pDX, IDC_EDIT_DATAVALUE_SIZE, m_dTextSizeDataValue);
	DDX_Text(pDX, IDC_EDIT_ELEMENT_SIZE, m_dTextSizeElement);
	DDX_CBIndex(pDX, IDC_COMBO_SAP_VERSION, m_nSolveType);
	DDX_Radio(pDX, IDC_RADIO_SHEAR22, m_nRadioForce);
	DDX_Control(pDX, IDC_COMBO_LOAD_TYPE, m_comboLoad);
	DDX_Control(pDX, IDC_COMBO_SAP_VERSION, m_comboVersion);
	DDX_Control(pDX, IDC_COMBO_LOADFORCE_TYPE, m_comboLoadType);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_BUTTON_SAVE_IMAGEFILE, m_btnSaveImage);
	DDX_Control(pDX, IDC_F_OUTPUT_FILEPRINT, m_btnMemberForce);
	DDX_Control(pDX, IDC_F_OUTPUT_FILEPRINT2, m_btnReaction);
	DDX_Control(pDX, IDC_F_OUTPUT_FILEPRINT3, m_btnRizing);
	DDX_Control(pDX, IDC_BUTTON_AUTORUN, m_btnSapRun);
	DDX_Control(pDX, IDC_BUTTON_MAKE_S2K, m_btnSapInput);
	DDX_Control(pDX, IDC_BUTTON_SAPIMPORT, m_btnFileRead);
	DDX_Control(pDX, IDC_BUTTON_SAP_DELETE, m_btnFileDelete);
	DDX_Control(pDX, IDC_BUTTON_JOINT_VIEW_DLG, m_btnBoundary);
	DDX_Control(pDX, IDC_BUTTON_SECTION_COEFF_DLG, m_btnElement);
}

BEGIN_MESSAGE_MAP(CCalcModelingResult, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CCalcModelingResult)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADFORCE_TYPE, OnSelchangeLoadCase)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_MAKE_S2K, OnButtonMakeSapS2k)
	ON_BN_CLICKED(IDC_BUTTON_AUTORUN, OnButtonAutoRun)
	ON_BN_CLICKED(IDC_F_OUTPUT_FILEPRINT, OnFilePrintElemForce)
	ON_BN_CLICKED(IDC_F_OUTPUT_FILEPRINT2, OnFilePrintReaction)
	ON_BN_CLICKED(IDC_F_OUTPUT_FILEPRINT3, OnFilePrintCamber)
	ON_BN_CLICKED(IDC_F_OUTPUT_CHECK1, OnShowJointNo)
	ON_BN_CLICKED(IDC_F_OUTPUT_CHECK2, OnShowElemNo)
	ON_BN_CLICKED(IDC_F_OUTPUT_CHECK3, OnShowRestraint)
	ON_BN_CLICKED(IDC_F_OUTPUT_CHECK4, OnShowData)
	ON_BN_CLICKED(IDC_BUTTON_SAPIMPORT, OnButtonSapAdd)
	ON_BN_CLICKED(IDC_BUTTON_SAP_DELETE, OnButtonSapDelete)
	ON_BN_CLICKED(IDC_RADIO_SHEAR22, OnRadioShear22)
	ON_BN_CLICKED(IDC_RADIO_MOMENT22, OnRadioMoment22)
	ON_BN_CLICKED(IDC_RADIO_AXIAL_FORCE, OnRadioAxialForce)
	ON_BN_CLICKED(IDC_RADIO_DISPLACEMENT, OnRadioDisplacement)
	ON_BN_CLICKED(IDC_RADIO_SHEAR33, OnRadioShear33)
	ON_BN_CLICKED(IDC_RADIO_MOMENT33, OnRadioMoment33)
	ON_BN_CLICKED(IDC_RADIO_AXIAL_TORSION, OnRadioAxialTorsion)
	ON_BN_CLICKED(IDC_RADIO_REACTION, OnRadioReaction)
	ON_CBN_SELCHANGE(IDC_COMBO_LOAD_TYPE, OnSelchangeComboLoadType)
	ON_BN_CLICKED(IDC_RADIO_VIEW_YZ, OnRadioViewYz)
	ON_BN_CLICKED(IDC_RADIO_VIEW_XZ, OnRadioViewXz)
	ON_BN_CLICKED(IDC_RADIO_VIEW_XY, OnRadioViewXy)
	ON_BN_CLICKED(IDC_RADIO_VIEW_3D, OnRadioView3d)
	ON_CPN_XT_SELENDOK(IDC_BUTTON_BKCHANGE, OnSelEndOkBackClr)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMAGEFILE, OnButtonSaveImagefile)
	ON_BN_CLICKED(IDC_BUTTON_SECTION_COEFF_DLG, OnButtonSectionCoeffDlg)
	ON_BN_CLICKED(IDC_BUTTON_JOINT_VIEW_DLG, OnButtonJointViewDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SAP_VERSION, OnSelchangeComboSapVersion)
	ON_CBN_SELCHANGE(IDC_COMBO_RESULT_TYPE, OnSelchangeComboResultType)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CALC_OK, OnCalcOK)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcModelingResult message handlers
// 3D View가 CGeneralBaseDlg::OnInitDialog()의 SetDataInit()이후에 생성이 되기 때문에 
// CGeneralBaseDlg::OnInitDialog()를 한번 돌린 후 3D화면을 설정해 줌. 
void CCalcModelingResult::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CAPlateCalcStd   *pCalcStd = m_pStd->m_pCalcStd;
	
	pCalcStd->m_nScaleGrade = m_ctrlSliderScale.GetPos();
	m_pStd->m_FEMDraw.SetDispScale(m_ctrlSliderDispScale.GetPos()*10);
	pCalcStd->m_dAlpha      = (double(m_ctrlSliderAlpha.GetPos())/10);

	DrawInputDomyunView(TRUE);

	CGeneralBaseDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCalcModelingResult::DrawInputDomyunView(BOOL bZoomAll)
{
	CFEMManage	*pFEM	= m_pStd->m_pCalcStd->GetModelingFemData(m_nFileNo);
	CDomyun		*pDom	= m_pView->GetDomyun();
	pDom->ClearEtt(TRUE);

	if(m_comboResultType.GetCurSel()!=0)
		pFEM = &m_FEMTotal;
	
	m_pStd->m_FEMDraw.ChangeFEM(pFEM);
	m_pStd->m_FEMDraw.InsertDomyunPoint();
	m_pStd->m_FEMDraw.SetLegendPow(FALSE);
	m_pStd->m_FEMDraw.SetRestraintScale(1);
	m_pStd->m_FEMDraw.SetJointSize(0.2);
	m_pStd->m_FEMDraw.SetGraientCountor(TRUE);

	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_JOINT_NUMBER, m_bShowJoint);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_NUMBER, m_bShowElement);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_RENDER_RESTRAINT, m_bShowRestraint);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE, m_bShowResultData);
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_LOADCASE, m_bCheckLoadType);
	
	long	nLoadCombo = max(m_comboLoadType.GetCurSel(), 0);
	if(m_comboResultType.GetCurSel()==1)
		nLoadCombo	= pFEM->GetLoadCaseIndex("STRESS_1");
	else if(m_comboResultType.GetCurSel()==2)
		nLoadCombo	= pFEM->GetLoadCaseIndex("STRESS_2");

	m_pStd->m_FEMDraw.SetLoadCase(nLoadCombo);	

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

void CCalcModelingResult::SetDataDefault()
{
}

void CCalcModelingResult::SetDataInit()
{
	MakeTotalData();
	SetGridTitle();

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	int nSize = m_pStd->m_pCalcStd->GetSizeModelingResult();
	m_Grid.SetRows(nSize+1);
	m_Grid.SetRowHeightAll(20);
	m_dTextSizeJoint		= 0.2;
	m_dTextSizeDataValue	= 0.2;
	m_dTextSizeElement		= 0.2;
	CString str = _T("");
	long nRow = 0;

	long	nFemSize = ModelCalc.GetFrameAnalysisSize();
	for(int i=0; i<nFemSize; i++)
	{
		CFEMManage *pFEM = m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(i));
		if(pFEM->GetTotalResultSize()==0) continue;
		str.Format("%d", i+1);
		m_Grid.SetTextMatrix(nRow+1, 0, str);
		str = m_pStd->m_pCalcStd->m_szArrModelingFilePath.GetAt(i);
		m_Grid.SetTextMatrix(nRow+1, 1, str, (UINT)DT_LEFT);
		
		int nType = pFEM->m_nExtraInfo;
		str = GetLoadName(nType);
		m_Grid.SetTextMatrix(nRow+1, 2, str);
		
		str.Format("%d", pFEM->GetLoadCaseSize());
		m_Grid.SetTextMatrix(nRow+1, 3, str);
		m_Grid.SetTextMatrix(nRow+1, 4, pFEM->m_szFEMName ,(UINT)DT_LEFT);
		nRow++;
	}	

	if(m_nSolveType==0)
	{
		GetDlgItem(IDC_BUTTON_AUTORUN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_MAKE_S2K)->SetWindowText("Sap Input 생성");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_AUTORUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MAKE_S2K)->SetWindowText("MIDAS Input 생성");
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CCalcModelingResult::SetDataSave()
{
	UpdateData(TRUE);

	CAPlateCalcStd   *pCalcStd = m_pStd->m_pCalcStd;

	pCalcStd->m_nScaleGrade         = m_ctrlSliderScale.GetPos();	
	pCalcStd->m_dAlpha              = (double(m_ctrlSliderAlpha.GetPos())/10);

//	pCalcStd->m_bViewRealSection    = m_bViewRealSection;
}

void CCalcModelingResult::SetGridTitle() 
{	
	m_Grid.DeleteAllItems();
	m_Grid.SetRows(1);
	m_Grid.SetCols(5);

	m_Grid.EnableTitleTips(FALSE);	
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.SetTextBkColor(RGBEDITABLE);	
	m_Grid.SetRowResize(TRUE);
	m_Grid.SetColumnResize(TRUE);
	m_Grid.SetGridLines(GVL_BOTH);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetEditable(FALSE);
	m_Grid.SetHeaderSort(FALSE);
	m_Grid.SetListMode(TRUE);
	
	m_Grid.SetColumnWidth (0, 50);
	m_Grid.SetColumnWidth (1, 300);
	m_Grid.SetColumnWidth (2, 150);
	m_Grid.SetColumnWidth (3, 70);
	m_Grid.SetColumnWidth (4, 350);

	m_Grid.SetTextMatrix (0, 0, "No.");
	m_Grid.SetTextMatrix (0, 1, "F i l e");
	m_Grid.SetTextMatrix (0, 2, "Type");
	m_Grid.SetTextMatrix (0, 3, "Case 수");
	m_Grid.SetTextMatrix (0, 4, "제   목");

	m_Grid.SetRedraw(TRUE,TRUE);

	UpdateData(FALSE);
}

void CCalcModelingResult::SetComboData(long nSAPIndex)
{
	CAPlateCalcStd *pCalcStd   = m_pStd->m_pCalcStd;
	CFEMManage     *pFEMManage = NULL;
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	
	m_comboLoadType.ResetContent ();

	if(pCalcStd->GetSizeModelingResult()==0)	return;
	
	pFEMManage = pCalcStd->GetModelingFemData(nSAPIndex);
	
	for (long n=0; n<pFEMManage->GetLoadCaseSize(); n++)
	{
		CString cs = pFEMManage->m_szLCName[n];
		m_comboLoadType.AddString(cs);
	}
	m_comboLoadType.SetCurSel(0);

	UpdateData(FALSE);
}

void CCalcModelingResult::OnSelchangeLoadCase() 
{
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	
	long nLoadCase  = m_comboLoadType.GetCurSel();
	m_pStd->m_FEMDraw.SetViewForceType(nLoadCase);
	DrawInputDomyunView(TRUE);
}

//모델링 설정 다이얼로그는 첫번째 CFEMManage로 생성해서 m_nViewLoadCase=0이 되어야 오류를 방지함
void CCalcModelingResult::OnButtonPrev()
{
	m_pStd->m_FEMDraw.SetViewForceType(0);

	CGeneralBaseDlg::OnButtonPrev();
}



void CCalcModelingResult::OnButtonMakeSapS2k() 
{
	CString szPath	= AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString szEdit	= AfxGetApp()->GetProfileString("Settings", "Editor");

	if(szEdit.Find(".exe")==-1)
		szEdit += "\\AEdit.exe";

	UpdateData(TRUE);
	
	CString strf    = _T("");
	int nSize = m_pStd->m_pCalcStd->GetSizeModelingFemData();

	for(int i=0; i<nSize; i++)
	{
		strf = m_sSapInputFileName;
		CFEMManage	*pFEM = m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(i));
		if(m_nSolveType==0)
		{
			PathToPathNum(strf,_T("s2k"), i+1);
			pFEM->SaveAsSap2000(strf);
			
			ShellExecute(NULL,NULL,szEdit,"\"" + strf + "\"",NULL,SW_SHOWNORMAL);
		}
		else if(m_nSolveType==1)
		{
			PathToPathNum(strf,_T("mct"), i+1);
			pFEM->SaveAsMidas(strf);
			ShellExecute(NULL,NULL,szEdit,"\"" + strf + "\"",NULL,SW_SHOWNORMAL);
		}
	
		if(i==0) 
		{
			// AEdit 실행되었는지 검사 - 실행이 안되었을 경우 최장 2초간 검사
			CWnd *pWnd;
			for(long n=0; n<4; n++)
			{			
				if(GetMyselfWindow(&pWnd)) break;
				Sleep(500);
			}
		}
	}	
}

void CCalcModelingResult::OnButtonAutoRun() 
{
	UpdateData(TRUE);

	if(MessageBox("SAP 자동실행을 하겠습니까?", "SAP 자동실행", MB_ICONQUESTION|MB_YESNO)==IDNO)
		return;

	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();	
	CString strAllf = _T("");
	CString strf    = _T("");
	int nFrameSize  = m_pStd->m_pCalcStd->GetSizeModelingFemData();
	for(int i=0; i<nFrameSize; i++)
	{
		strf = m_sSapInputFileName;
		PathToPathNum(strf,_T("s2k"), i+1);
		long nIdx = m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(i);
		CFEMManage *pFEM = m_pStd->m_pCalcStd->GetModelingFemData(nIdx);
		pFEM->RemoveResultData();
		for(long nLC=0; nLC<pFEM->GetLoadCaseSize(); nLC++)
		{
			if(nLC>=pFEM->GetLoadCaseSize()-pFEM->m_nOutputCaseAddSize)
			{
				pFEM->m_szLCName.RemoveAt(nLC);
				nLC--;
			}
		}
		pFEM->m_nOutputCaseAddSize = 0;
		pFEM->SaveAsSap2000(strf);
	}

	// 리다이렉트 - SAP2000 구동	
	CDocument* pDoc = m_pStd->m_pDoc;
    CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");	
	CString szSapPath = AfxGetApp()->GetProfileString("Settings", "Sap2000Directory");	
	CString szRunningPath = pDoc->GetPathName();
	CFileFind find;
	CString szS2kFile;
	CString szRunningDir;

	GetDirectoryName(szRunningDir,szRunningPath);
	if(szSapPath.IsEmpty() || !find.FindFile(szSapPath))
	{
		CString szText = "<< Sap2000 프로그램이 설치된 디렉토리를 선택하세요 >>";
		CCustomFolderDialog FolderDlg("",0,GetParent());		
		FolderDlg.m_bi.lpszTitle = szText.GetBuffer(0);
		if(FolderDlg.DoModal()==IDOK)
		{
			szSapPath = FolderDlg.GetPathName();
			AfxGetApp()->WriteProfileString("Settings", "Sap2000Directzory", szSapPath);										
		}
		else
			return;
	}

	BOOL bAnalysisComplete = FALSE;
	szS2kFile = pDoc->GetPathName();	
	if(find.FindFile(szSapPath + "\\Sapre.exe"))
	{						
		::SetCurrentDirectory(szRunningDir);

		// 다시 계산 초기화
		m_bErrorInclude = FALSE;			
		m_CalcDlg.m_pMainWnd = this;
		m_CalcDlg.m_bError = FALSE;
		m_CalcDlg.m_bWarning = FALSE;
		m_CalcDlg.m_szCurrPath = szRunningDir;
		if(!m_CalcDlg.GetSafeHwnd()) m_CalcDlg.Create(GetParent(),this);	
		m_CalcDlg.m_ctlList.ResetContent();

		for(long n=0; n<nFrameSize; n++)
		{

			CString szSapPre = szS2kFile;
			CString szSapGo = szS2kFile;
			PathToPathNum(szSapPre,_T("s2k"),n+1);
			PathToPathNum(szSapGo,_T(""),n+1);	
			szSapPre = szSapPre.Mid(szRunningDir.GetLength()+1); 
			szSapGo = szSapGo.Mid(szRunningDir.GetLength()+1);
			szSapPre.Replace(" ","_");
			szSapGo.Replace(" ","_");
			if(find.FindFile(szSapPre))
			{
				if(n==0)
				{
					CString sz = _T("");
					sz = szSapPath + "\\Sapre.exe " + szSapPre;
					m_CalcDlg.AddCommand(STDIO_OPEN, szSapPath + "\\Sapre.exe " + szSapPre);
				}
				else
				{
					CString sz = _T("");
					sz = szSapPath + "\\Sapre.exe " + szSapPre;
					m_CalcDlg.m_StrCmdList.AddTail(szSapPath + "\\Sapre.exe " + szSapPre);
				}
				m_CalcDlg.m_StrCmdList.AddTail(szSapPath + "\\Sapgo.exe " + szSapGo);
				bAnalysisComplete = TRUE;
			}				
			else
			{
				AfxMessageBox(szS2kFile + " 파일이 존재하지 않습니다 !\nInput 파일부터 생성후 실행하세요.", MB_ICONSTOP);
				bAnalysisComplete = FALSE;
				return;
			}
		}				
	}
	else
	{		
		CSap200RuntimeDlg RuntimeDlg;

		RuntimeDlg.m_szMsg = szSapPath + "\\Sapre.exe" +" 파일을 찾을 수 없습니다.\n\n자동 복사를 클릭하시면 Sap2000 설치된 디렉토리 하부에 위치한 Utility 디렉토리에서 Sapre.exe를 자동복사 합니다.\n\n자동복사 후 다시 실행하시기 바랍니다.";
		RuntimeDlg.DoModal();		
		return;
	}

	if (bAnalysisComplete) pData->m_bReadLiveDispOutput = TRUE;
}


void CCalcModelingResult::GetDirectoryName(CString &strDirName, const CString& strPathName)
{
  char drive[_MAX_DRIVE], dir[_MAX_DIR],
       fname[_MAX_FNAME], ext[_MAX_EXT];

  _SPLITPATH_S(strPathName, drive, dir, fname, ext);
  strDirName = drive;
  strDirName += dir;
  int len = strDirName.GetLength();
  if (len)
  {
    char* p = strDirName.GetBuffer(len);
    p = p + (char)(len - 1);
    if (*p == '\\') *p = '\0';
	strDirName.ReleaseBuffer();
  }
}

void CCalcModelingResult::PathToPathNum(CString & path,CString csExt, long Num) const
{
	int nFine1 = path.ReverseFind('.');
	int nFine2 = path.ReverseFind('\\');
	if( nFine1>=0 && nFine1>nFine2)   path = path.Left(nFine1);

	CString csNum;
	csNum.Format("%02ld",Num);
	path += csNum;
	if(csExt.GetLength() > 0) 
	{
		path += ".";
		path += csExt;
	}

}

long CCalcModelingResult::OnCalcOK(WPARAM, LPARAM)
{	
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	CString strf    = _T("");
	int nFrameSize = m_pStd->m_pCalcStd->GetSizeModelingFemData();
	for(int i=0; i<nFrameSize; i++)
	{
		// 불필요한 파일 삭제
		char *Ext[] = { "$2k", "$OG", "CMB", "EKO", "EV",  "F11", "F3",  "GUU", "H",   "H11", "H3", 
						"HSA", "ID",  "JCJ", "JOB", "K",   "K11", "K3",  "L3",  "LBL", "LBM", "LDS",
						"LIN", "LIS", "LOG", "LTS", "M11", "M3",  "MAS", "MOD", "MTL", "P1S", "P3",
						"P3X", "PAR", "PAT", "PDE", "PDF", "PPD", "SBK", "SEC", "SEL", "T3",  "TXT",
						"U",   "U11", "U3",  "V",   "V11", "V3",  "XMJ", "XYZ", "Y11", "Y3",  "Z11",
						"Z3",  "SDB", "HSC", "U1",	"SEV", "RSI", "JCT", "B3",  "BXU", "BXR", "BXP",
						"BRR", "BRC", "B11", "B1",  "B",   "LRD", "K1",  "F1",  "Y1",  "M1",  "LFC", "LCS"};
		long nCount = sizeof(Ext) / sizeof(Ext[0]);
		for(long s=0; s<nCount; s++)
		{
			strf = m_sSapInputFileName;
			PathToPathNum(strf,Ext[s], i+1);
			try{CFile::Remove(strf);} 
			catch(CFileException *pEx) {pEx->Delete();}
			catch(...){};
		}		
	}

	if(!m_CalcDlg.m_bError)
	{				
		CString sBuf = m_sSapInputFileName;		
		PathToPathNum(sBuf, m_nSolveType==0 ? _T("OUT") : _T("ANL"), 1);		

		ReadOutFile(sBuf);
		
		SetDataInit();
		//모렐링 파일을 요소와 지점별로 정리한다.
		CFemModelingCalc ModelCalc(m_pStd->m_pDataManage, m_pStd->m_pCalcStd);

		CFEMManage *pFEMManage = m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
		if(pFEMManage->GetTotalResultSize() > 0)
		{
			ModelCalc.MakeResultData();
			ModelCalc.MakeSummaryOfElementForce();	
			ModelCalc.MakeSummaryOfJointForce();
			ModelCalc.MakeMaxMinForceDB();
			ModelCalc.MakeMaxMinForceDL();
			ModelCalc.MakeMaxMinForceMTY();
			ModelCalc.MakeMaxMinForcePeople();
			if(pData->m_bWind)
				ModelCalc.MakeMaxMinForceWind();
			MakeTotalData();
		}
	}
	else
		AfxMessageBox("모델링 결과값정리중 문제가 발생하였습니다.\n정상적인 진행을 위해 [모델링 재생성]후 [SAP자동실행]을 하십시오");

	m_nFileNo = BEFORE_COMPOSITE_ANALYSIS;
	SetLoadTypeCombo();
	SetDataInit();
	SetComboData(0);
	DrawInputDomyunView(TRUE);

	AfxGetMainWnd()->SendMessage(WM_USER+15, 1);

	// set result fem index
	m_ResultIdx.RemoveAll();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	long nFemSize = ModelCalc.GetFrameAnalysisSize();	
	for (long n=0; n<nFemSize; n++)
	{
		CFEMManage *pFEM = m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(n));
		if(pFEM->GetTotalResultSize()==0) continue;	
		m_ResultIdx.Add(n);
	}

	return 0;
}

void CCalcModelingResult::OnFilePrintElemForce() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}
	CPlateStressOut	pStressOut(m_pStd);
 
	CStringArray Arr;
	pStressOut.ElementForceOutput(Arr);

	m_pStd->OutputViewOnAView(Arr);
}

void CCalcModelingResult::OnFilePrintReaction() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}
	
	CPlateStressOut	pStressOut(m_pStd);
 
	CStringArray Arr;
	pStressOut.NodeReactionOutput(Arr);
	m_pStd->OutputViewOnAView(Arr);
}

void CCalcModelingResult::OnFilePrintCamber() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}
	
	CPlateStressOut	pStressOut(m_pStd);
 
	CStringArray Arr;
	pStressOut.NodeCamberOutput(Arr);
	m_pStd->OutputViewOnAView(Arr);
}


void CCalcModelingResult::OnShowJointNo() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);	
}

void CCalcModelingResult::OnShowElemNo() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);	
}

void CCalcModelingResult::OnShowRestraint() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);	
}

void CCalcModelingResult::OnShowData() 
{
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnButtonSapAdd() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	int nFind = m_sSapInputFileName.ReverseFind('\\');
	CString sBuf = m_sSapInputFileName.Mid(nFind+1) + "01";

	CString sMessage =m_nSolveType==0 ? _T("Output Files (*.out) |*.OUT|All Files (*.*)|*.*||") : _T("Output Files (*.anl) |*.ANL|All Files (*.*)|*.*||");
	
	CString szDir = _T("");
	CString szPath = m_pStd->m_pDoc->GetPathName();
	GetDirectoryName(szDir, szPath);
	CFileDialog dlgFile(TRUE, "", sBuf, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, sMessage, GetParent());	
	dlgFile.m_ofn.lpstrInitialDir = szDir;
		
	if(dlgFile.DoModal()==IDOK)
	{
		if(!ReadOutFile(dlgFile.GetPathName())) return;
	}
	
	CFEMManage *pFEMManage = m_pStd->m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc ModelCalc(m_pStd->m_pDataManage, m_pStd->m_pCalcStd);
	if(pFEMManage->GetTotalResultSize() > 0)
	{
		ModelCalc.MakeResultData();
		ModelCalc.MakeSummaryOfElementForce();	
		ModelCalc.MakeSummaryOfJointForce();
		ModelCalc.MakeMaxMinForceDB();
		ModelCalc.MakeMaxMinForceDL();
		ModelCalc.MakeMaxMinForceMTY();
		ModelCalc.MakeMaxMinForcePeople();
		if(pData->m_bWind)
			ModelCalc.MakeMaxMinForceWind();
		MakeTotalData();
	}

	SetLoadTypeCombo();
	SetDataInit();
	SetComboData(0);
	
	// set result fem index
	m_ResultIdx.RemoveAll();
	long nFemSize = ModelCalc.GetFrameAnalysisSize();	
	for (long n=0; n<nFemSize; n++)
	{
		CFEMManage *pFEM = m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(n));
		if(pFEM->GetTotalResultSize()==0) continue;	
		m_ResultIdx.Add(n);
	}

	DrawInputDomyunView(TRUE);
}


void CCalcModelingResult::OnButtonSapDelete() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();

	if(m_pStd->m_pCalcStd->GetSizeModelingResult() < 1)
		return;

	CString sMessage = _T("");

	long nLower = m_Grid.GetSelectedCellRange().GetMinRow();
	long nUpper = m_Grid.GetSelectedCellRange().GetMaxRow();
	if(nLower<0 || nUpper<0) return;

	long nSttFile = atol(m_Grid.GetTextMatrix(nLower, 0))-1;
	long nEndFile = atol(m_Grid.GetTextMatrix(nUpper, 0))-1;
	long nGirdRow = m_Grid.GetRows()-2;
	if (nEndFile>nGirdRow) nEndFile = nGirdRow;
	
	pData->m_sz3DFrameOutFilePath.RemoveAll();
	sMessage.Format("  %d 결과를 삭제하시겠습니까?  ", nEndFile - nSttFile+1);

	if(IDOK == MessageBox((LPCSTR)sMessage, "결과 삭제 확인", MB_OKCANCEL | MB_ICONWARNING))   
	{
		long n = 0;
		// 선택된 Output Data 삭제
		for(n=nSttFile; n<=nEndFile; n++)
		{
			long nIdx = m_ResultIdx.GetAt(n);
			m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(nIdx))->RemoveResultData();
		}

		for (n=nSttFile; n<=nEndFile; n++)
		{
			m_comboLoad.DeleteString(n);
			m_ResultIdx.RemoveAt(n);
			nEndFile--;
			n--;
		}
	}

	SetDataInit();
	SetComboData(TRUE);
	
	DrawInputDomyunView(FALSE);		
}


BOOL CCalcModelingResult::ReadOutFile(CString szFile, BOOL bMessage)
{
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	//////////////////////////////////////////////////////////////////////////
	// 해석결과
//	m_pStd->m_pCalcStd->Remove3DFrameResultData();

	CString sPathName = szFile;
	CString sFileName = sPathName.Left(sPathName.ReverseFind ('.') - 2);

	pData->m_sz3DFrameOutFilePath.RemoveAll();

	m_pStd->m_pCalcStd->m_szArrModelingFilePath.RemoveAll();

	for(long n=0; n<ModelCalc.GetFrameAnalysisSize(); n++)
	{
		CFEMManage *pFEM = m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(n));
		//if(pFEM->GetLoadCaseSize()==0)	continue;	

		CStdioFile fFile;
		CString sFn = sFileName;
		PathToPathNum(sFn, m_nSolveType==0 ? _T("OUT") : _T("ANL"), n+1);
		// s2k의 내용과 결과내용이 일치하는지 체크 해야함!!!
		BOOL bSameNodeSize	  = TRUE;
		BOOL bSameElementSize = TRUE;
		if(fFile.Open(sFn, CFile::modeRead|CFile::typeText))
		{
			fFile.Close();

			m_pStd->m_pCalcStd->m_szArrModelingFilePath.Add(sFn);

			if(m_nSolveType==0)
			{
				CFEMLoadSAP2000Output	LoadOut(pFEM);
				if(LoadOut.LoadData(sFn))
				{
					m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
					m_pStd->m_FEMDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);	
				}
			}
			else
			{
				CLoadMidasOutput LoadOut(pFEM);
				if(LoadOut.LoadData(sFn))
				{
					m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
					m_pStd->m_FEMDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
					m_comboLoad.SetCurSel(0);
				}
			}
			
			pData->m_sz3DFrameOutFilePath.Add(sFn);
		}
		else
		{
			pData->m_sz3DFrameOutFilePath.RemoveAll();
			if(bMessage)
				MessageBox("  파일이 존재하지 않습니다.  ", NULL, MB_ICONWARNING);
			return FALSE;
		}

		if(!bSameElementSize || !bSameNodeSize)
		{
			pData->m_sz3DFrameOutFilePath.RemoveAll();
			if(bMessage)
				MessageBox("  현재 작업한 파일이 아닙니다.  ", NULL, MB_ICONWARNING);
			return FALSE;
		}		
	}

	// 솟음 데이타 확인
	((CDetCamberJewonDlg*)m_pStd->GetDlgOfIDD(IDD_DET_20_CAMBERJEWON))->SapRead();
	((CDetCamberJewonDlg*)m_pStd->GetDlgOfIDD(IDD_DET_20_CAMBERJEWON))->SetCamber();

	pData->m_bReadFrameModelingOutput = TRUE;

	return TRUE;
}



void CCalcModelingResult::SetFileName()
{	
	CString szPathWork	= AfxGetApp()->GetProfileString("Afx Directory", "Work Directory");

	m_sSapInputFileName = m_pStd->m_pDoc->GetPathName();
	int nFine = m_sSapInputFileName.ReverseFind('.');
	if( nFine>=0)   m_sSapInputFileName = m_sSapInputFileName.Left(nFine);

	nFine = m_sSapInputFileName.ReverseFind('\\');	
	CString szFileName = m_sSapInputFileName.Mid(nFine+1);
	szFileName.Replace(" ","_");
	m_sSapInputFileName = m_sSapInputFileName.Left(nFine+1) + szFileName;

	UpdateData(FALSE);	
}

void CCalcModelingResult::OnPreInitDialog()
{
	m_nFileNo			= BEFORE_COMPOSITE_ANALYSIS;
	m_bCheckLoadType	= FALSE;
	m_nRadioForce		= 1;
	m_nRadioForce		= 1;
	m_btnBkChange.SetColor(m_pView->GetDomBkColor());

	CAPlateCalcStd   *pCalcStd = m_pStd->m_pCalcStd;
	m_pStd->m_FEMDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
	
	m_ctrlSliderScale.SetRange(0, 10);
	m_ctrlSliderAlpha.SetRange(0, 10);
	m_ctrlSliderDispScale.SetRange(0, 20);

	m_ctrlSliderScale.SetPos(pCalcStd->m_nScaleGrade);
	m_ctrlSliderAlpha.SetPos(long(pCalcStd->m_dAlpha*10));
	m_ctrlSliderDispScale.SetPos((int)(m_pStd->m_FEMDraw.GetDispScale()/10));
	
	m_comboResultType.SetCurSel(0);
	m_pStd->m_FEMDraw.SetViewForceType(0);
	SetComboData(0);
	SetLoadTypeCombo();
	SetFileName();

	GetDlgItem(ID_BUTTON_RECOMMEND)->ShowWindow(SW_HIDE);

	SetResize(IDC_BUTTON_SAPIMPORT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAP_DELETE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_JOINT_VIEW_DLG, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SECTION_COEFF_DLG, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_ELEMENT_RESULT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_SHEAR22, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_MOMENT22, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_AXIAL_FORCE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_DISPLACEMENT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_SHEAR33, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_MOMENT33, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_AXIAL_TORSION, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_REACTION, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_GROUP, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TEXT1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TEXT3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TEXT2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_SLIDER1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_SLIDER2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_SLIDER_ALPHA, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TEXT5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TEXT9, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TEXT7, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC_TEXTSIZE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_JOINTNUMBER_SIZE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_ELEMENT_SIZE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_DATAVALUE_SIZE, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_CHECK1, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_CHECK2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_CHECK3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_CHECK4, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_VIEW_YZ, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_VIEW_XZ, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_VIEW_XY, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_RADIO_VIEW_3D, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC6, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_LOAD_TYPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_BRIDGE_OUT_VIEW_STATIC5, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_LOADFORCE_TYPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_RESULT_TYPE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_RESULT_TYPE, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetResize(IDC_STATIC7, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_FILEPRINT, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_FILEPRINT2, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_F_OUTPUT_FILEPRINT3, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_BKCHANGE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_BUTTON_SAVE_IMAGEFILE, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_EDIT_RESULT_PATH, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_SAP_VERSION, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_AUTORUN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MAKE_S2K, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	// set result fem index
	m_ResultIdx.RemoveAll();
	long nFemSize = ModelCalc.GetFrameAnalysisSize();	
	for (long n=0; n<nFemSize; n++)
	{
		CFEMManage *pFEM = m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(n));
		if(pFEM->GetTotalResultSize()==0) continue;	
		m_ResultIdx.Add(n);
	}

	UpdateData(FALSE);
}

void CCalcModelingResult::OnRadioShear22() 
{
	m_nRadioForce = 0;
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioMoment22() 
{
	m_nRadioForce	= 4;
	DrawInputDomyunView(TRUE);	
}

void CCalcModelingResult::OnRadioAxialForce() 
{
	m_nRadioForce = 2;
	DrawInputDomyunView(TRUE);		
}

void CCalcModelingResult::OnRadioDisplacement() 
{
	m_nRadioForce = 6;
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioShear33() 
{
	m_nRadioForce = 3;
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioMoment33() 
{
	m_nRadioForce	= 1;
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioAxialTorsion() 
{
	m_nRadioForce = 5;
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioReaction() 
{
	m_nRadioForce = 7;
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::SetLoadTypeCombo()
{
	m_comboLoad.ResetContent();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	for(int i=0; i<ModelCalc.GetFrameAnalysisSize(); i++)
	{
		CFEMManage *pFEM = m_pStd->m_pCalcStd->GetModelingFemData(m_pStd->m_pCalcStd->GetAnalysisTypeByIndex(i));
		if(pFEM->GetTotalResultSize()==0)		continue;
		
		long nType = pFEM->m_nExtraInfo;

		m_comboLoad.AddString(GetLoadName(nType));
	}
	m_comboLoad.SetCurSel(0);

	UpdateData(FALSE);
}

void CCalcModelingResult::OnSelchangeComboLoadType() 
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	
	UpdateData(TRUE);
	long    nIdx	= m_comboLoad.GetCurSel();
	long	nLoad	= m_ResultIdx.GetAt(nIdx);
	m_nFileNo = pCalcStd->GetAnalysisTypeByIndex(nLoad);

	CFEMManage *pFemManage = pCalcStd->GetModelingFemData(m_nFileNo);
	
	SetComboData(m_nFileNo);

	m_comboLoadType.SetCurSel(0);

	m_pStd->m_FEMDraw.ChangeFEM(pFemManage);

	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioViewYz() 
{
	m_cAxis = 'x';
	DrawInputDomyunView(TRUE);	
}

void CCalcModelingResult::OnRadioViewXz() 
{
	m_cAxis = 'y';
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioViewXy() 
{
	m_cAxis = 'z';
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnRadioView3d() 
{
	m_cAxis = 'd';
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::OnSelEndOkBackClr()
{
	COLORREF clr = m_btnBkChange.GetColor();
	m_pStd->m_FEMDraw.SetBkColor(clr);
	GetView()->Invalidate(FALSE);
}

void CCalcModelingResult::OnButtonSaveImagefile() 
{
	AfxGetMainWnd()->SendMessage(WM_USER+50);
	/*
	CDC *pDC;				
	CDC *pBitDC = new CDC;
	CRect rect;
	CBitmap bmp, *pOldBitmap;	
	CImage Image;

	CShFileDialogBase dlg(FALSE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "JPG 파일 (*.jpg)|*.jpg|BMP 파일 (*.bmp)|*.bmp||", m_pView);
	dlg.m_ofn.lpstrTitle = "파일 저장";
	if (dlg.DoModal() != IDOK) 
	{
		delete pBitDC;
		return;
	}
	
	ShowWindow(SW_HIDE);
	pDC = m_pView->GetDC();
	m_pView->OnDrawMain(pDC);
	CString szFileName = dlg.GetPathName();
	m_pView->GetClientRect(&rect);
	pBitDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);				
	pOldBitmap = pBitDC->SelectObject(&bmp);	
	m_pView->OnDrawMain(pDC);	// 이미지로 저장할 화면	
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
	*/
}

void CCalcModelingResult::OnButtonSectionCoeffDlg() 
{
	CCalcModelingFramePropty	Dlg;
	Dlg.m_pStd = m_pStd;
	Dlg.DoModal();
}

void CCalcModelingResult::OnButtonJointViewDlg() 
{
	CJointRestraintDlg	Dlg;
	Dlg.m_pStd = m_pStd;
	Dlg.DoModal();	
}

void CCalcModelingResult::OnCellChangedRow(NMHDR* nmgv, LRESULT*)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)nmgv;

	long	nRow = pGridView->iRow;
	if(nRow>0)
	{
		m_comboLoad.SetCurSel(nRow-1);
		UpdateData(FALSE);
		OnSelchangeComboLoadType();
	}
}
void CCalcModelingResult::OnSelchangeComboSapVersion() 
{
	UpdateData();
	SetDataInit();	
}

CString CCalcModelingResult::GetLoadName(long nAnalysisType)
{
	CPlateBridgeApp	*pDB = m_pStd->GetBridge();
	
	CString	szName;
	if(pDB->IsTUGir())
	{
		switch(nAnalysisType)
		{
		case BEFORE_COMPOSITE_ANALYSIS :		szName = "합성전 사하중 - 1";	break;
		case BEFORE_TURN_COMPOSITE_ANALYSIS :	szName = "합성전 사하중 - 2";	break;
		case AFTER_COMPOSITE_ANALYSIS :			szName = "합성후 사하중";		break;
		case LIVE_LOAD_ANALYSIS :				szName = "활하중";				break;
		case SETTLEMENT_ANALYSIS :				szName = "지점침하";			break;
		}
	}
	else
	{
		switch(nAnalysisType)
		{
		case BEFORE_COMPOSITE_ANALYSIS :		szName = "합성전 사하중";	break;
		case AFTER_COMPOSITE_ANALYSIS :			szName = "합성후 사하중";	break;
		case LIVE_LOAD_ANALYSIS :				szName = "활하중";			break;
		case SETTLEMENT_ANALYSIS :				szName = "지점침하";		break;
		}
	}

	return szName;
}

void CCalcModelingResult::OnSelchangeComboResultType() 
{
	UpdateData(TRUE);
	if(m_comboResultType.GetCurSel()==0)	GetDlgItem(IDC_COMBO_LOADFORCE_TYPE)->EnableWindow(TRUE);
	else									GetDlgItem(IDC_COMBO_LOADFORCE_TYPE)->EnableWindow(FALSE);
	DrawInputDomyunView(TRUE);
}

void CCalcModelingResult::MakeTotalData()
{
	m_FEMTotal.RemoveAllData();

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	long	nLoadCase	= 0;
	long	nElem		= 0;
	m_FEMTotal.SYSTEM_VAR.m_szUnitType = "KN";
	m_FEMTotal.SYSTEM_VAR.m_UX = m_FEMTotal.SYSTEM_VAR.m_UY = m_FEMTotal.SYSTEM_VAR.m_UZ = 0;
	m_FEMTotal.SYSTEM_VAR.m_RX = m_FEMTotal.SYSTEM_VAR.m_RY = m_FEMTotal.SYSTEM_VAR.m_RZ = 0;	
	m_FEMTotal.m_nExtraInfo		= 0;
	m_FEMTotal.m_szFEMName		= "";
	m_FEMTotal.m_szLCName.RemoveAll();
	m_FEMTotal.m_szTitle.RemoveAll();

	ModelCalc.MakeJoint(&m_FEMTotal);
	ModelCalc.MakeElement(&m_FEMTotal);

	m_FEMTotal.MakeElementLoadForce(nLoadCase+1);
	m_FEMTotal.m_szLCName.SetAt(nLoadCase, "STRESS_1");	// 상연응력

	CResultArray* pPreResult = m_pStd->m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	POSITION pos = m_FEMTotal.m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement		*pElm			= (CElement*)m_FEMTotal.m_ElementList.GetNext(pos);
		if(pPreResult->m_EleResultArr.GetSize()>0)
		{
			CElementResult	*pEPreResult	= (CElementResult*)pPreResult->m_EleResultArr.GetAt(nElem);

			double	pValue[FRAME_SEG_SIZE+1][7] = {0,};
			for(long nSeg=0; nSeg<=FRAME_SEG_SIZE; nSeg++)
			{
				double	dRatioElm	= nSeg* (1/double(FRAME_SEG_SIZE));
				pValue[nSeg][0]	= dRatioElm;
				pValue[nSeg][1]	= pEPreResult->m_dTotalMax[ELE_SHEAR_2][nSeg];
				pValue[nSeg][2]	= pEPreResult->m_dTotalMax[ELE_MOMENT3][nSeg];
				pValue[nSeg][3]	= pEPreResult->m_dTotalMax[ELE_AXFORCE][nSeg];
				pValue[nSeg][4]	= pEPreResult->m_dTotalMax[ELE_SHEAR_3][nSeg];
				pValue[nSeg][5]	= pEPreResult->m_dTotalMax[ELE_MOMENT2][nSeg];
				pValue[nSeg][6]	= pEPreResult->m_dTotalMax[ELE_AXTORSN][nSeg];
			}
			m_FEMTotal.MakeElementResult(nLoadCase, (unsigned short)nElem);
			m_FEMTotal.SetElementData(nLoadCase, (unsigned short)nElem, FRAME_SEG_SIZE+1, pValue);
		}
		nElem++;
	}
	++nLoadCase;
	nElem	= 0;
	m_FEMTotal.MakeElementLoadForce(nLoadCase+1);
	m_FEMTotal.m_szLCName.SetAt(nLoadCase, "STRESS_2");	// 상연응력
	pos = m_FEMTotal.m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement	*pElm	= (CElement*)m_FEMTotal.m_ElementList.GetNext(pos);
		if(pPreResult->m_EleResultArr.GetSize()>0)
		{
			CElementResult	*pEPreResult	= (CElementResult*)pPreResult->m_EleResultArr.GetAt(nElem);

			double	pValue[FRAME_SEG_SIZE+1][7] = {0,};
			for(long nSeg=0; nSeg<=FRAME_SEG_SIZE; nSeg++)
			{
				double	dRatioElm	= nSeg* (1/double(FRAME_SEG_SIZE));
				pValue[nSeg][0]	= dRatioElm;
				pValue[nSeg][1]	= pEPreResult->m_dTotalMin[ELE_SHEAR_2][nSeg];
				pValue[nSeg][2]	= pEPreResult->m_dTotalMin[ELE_MOMENT3][nSeg];
				pValue[nSeg][3]	= pEPreResult->m_dTotalMin[ELE_AXFORCE][nSeg];
				pValue[nSeg][4]	= pEPreResult->m_dTotalMin[ELE_SHEAR_3][nSeg];
				pValue[nSeg][5]	= pEPreResult->m_dTotalMin[ELE_MOMENT2][nSeg];
				pValue[nSeg][6]	= pEPreResult->m_dTotalMin[ELE_AXTORSN][nSeg];
			}
			m_FEMTotal.MakeElementResult(nLoadCase, (unsigned short)nElem);
			m_FEMTotal.SetElementData(nLoadCase, (unsigned short)nElem, FRAME_SEG_SIZE+1, pValue);
		}
		nElem++;
	}
}
