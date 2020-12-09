// SpliceDesignResult.cpp : implementation file
//

#include "stdafx.h"
#include "generalinput.h"
#include "SpliceDesignResult.h"
#include "LabelControl.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpliceDesignResult dialog

CSpliceDesignResult::CSpliceDesignResult(CGeneralBaseStd *pStd, long nG, long nSp, CWnd* pParent /*=NULL*/)
	: CDialog(CSpliceDesignResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpliceDesignResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStd = pStd;
	m_nG  = nG;
	m_nSp = nSp;
}


void CSpliceDesignResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpliceDesignResult)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SPLICEDESIGNRESULT_TOTEXT, m_btnOutText);
	DDX_Control(pDX, IDOK, m_btnOk);
}


BEGIN_MESSAGE_MAP(CSpliceDesignResult, CDialog)
	//{{AFX_MSG_MAP(CSpliceDesignResult)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SPLICEDESIGNRESULT_TOTEXT, OnSpliceDesignResultToText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpliceDesignResult message handlers

void CSpliceDesignResult::SetDataInit()
{
	m_nSp = 0;	// splice number
	
	// 상부
	m_n = 0;		// 필요 볼트 수
	m_na = 0;		// 현재 볼트 수
	m_As = 0;		// 이음판 필요 단면적
	m_Ass = 0;		// 이음판 실제 단면적
	m_Su = 0;		// 이음판 설계 응력
	m_Ss = 0;		// 이음판 실제 응력
	m_R = 0;		// 실제 합성 전단력

	// 하부
	m_nl = 0;
	m_nal = 0;
	m_Asl = 0;
	m_Assl = 0;
	m_Sl = 0;
	m_Ssl = 0;
	m_Rl = 0;

	// 복부판 현장 이음 해석
	//m_SaAr[6];	// 볼트의 실제 전단력 1열~6열
	m_Rwh = 0;		// 볼트 1개의 실제 전단력
	m_Rw = 0;		// 실제 합성 전단력
	m_Swu = 0;		// 상부 이음판 실제 응력
	m_Swl = 0;		// 하부 이음판 실제 응력

	m_Sft = 0;		// 인장부 플랜지 응력

	m_Ra = 0;			// 허용 합성 전단력
	m_Sa = 0;			// 이음판 허용 능력
}


BOOL CSpliceDesignResult::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	strTitle.Format("현장 이음 [SP%d] 해석 결과", m_nSp+1);
	SetWindowText(strTitle);
	SetSpliceDesignData();
	DrawResult();

	return TRUE;
}

void CSpliceDesignResult::SetSplicePoint(long nG, long nType)
{
	m_nG = nG;
	m_nSp = nType;
}

void CSpliceDesignResult::DrawResult() const
{
	CString str(_T("")), str1(_T(""));

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_UP_PLATE_DESIGNFORCE); //m_Su
	str.Format("%.1f", m_Su);
	pStatic->SetWindowText(str);

	CStatic *p1 = (CStatic *)GetDlgItem(IDC_UP_BOLT_NEEDNO); //m_n
	CStatic *p2 = (CStatic *)GetDlgItem(IDC_UP_BOLT_REALNO); //m_na
	str.Format("%.1f", m_n);
	str1.Format("%g", m_na);
	p1->SetWindowText(str);
	p2->SetWindowText(str1);

	CStatic *p4 = (CStatic *)GetDlgItem(IDC_UP_PLATE_NEEDAREA); //m_As
	CStatic *p5 = (CStatic *)GetDlgItem(IDC_UP_PLATE_REALAREA); //m_Ass
	str.Format("%.2f", m_As);
	str1.Format("%.2f", m_Ass);
	p4->SetWindowText(str);
	p5->SetWindowText(str1);

	CStatic *p7 = (CStatic *)GetDlgItem(IDC_UP_PLATE_ALLOWFORCE); //m_Sa
	CStatic *p8 = (CStatic *)GetDlgItem(IDC_UP_PLATE_REALFORCE);  //m_Ss
	str.Format("%.2f", m_Sa);
	str1.Format("%.2f", m_Ss);
	p7->SetWindowText(str);
	p8->SetWindowText(str1);

	CStatic *p10 = (CStatic *)GetDlgItem(IDC_UP_PLATE_ALLOW_SUMFORCE); // m_Ra
	CStatic *p11 = (CStatic *)GetDlgItem(IDC_UP_PLATE_REAL_SUMFORCE); // m_R
	str.Format("%.2f", m_Ra);
	str1.Format("%.2f", m_R);
	p10->SetWindowText(str);
	p11->SetWindowText(str1);

	CStatic *p12 = (CStatic *)GetDlgItem(IDC_LO_PLATE_DESIGNFORCE); //m_Sl
	str.Format("%.1f", m_Sl);
	p12->SetWindowText(str);

	CStatic *p13 = (CStatic *)GetDlgItem(IDC_LO_BOLT_NEEDNO); //m_nl
	CStatic *p14 = (CStatic *)GetDlgItem(IDC_LO_BOLT_REALNO); //m_nal
	str.Format("%.1f", m_nl);
	str1.Format("%g", m_nal);
	p13->SetWindowText(str);
	p14->SetWindowText(str1);

	CStatic *p16 = (CStatic *)GetDlgItem(IDC_LO_PLATE_NEEDAREA); //m_Asl
	CStatic *p17 = (CStatic *)GetDlgItem(IDC_LO_PLATE_REALAREA); //m_Assl
	str.Format("%.2f", m_Asl);
	str1.Format("%.2f", m_Assl);
	p16->SetWindowText(str);
	p17->SetWindowText(str1);

	CStatic *p19 = (CStatic *)GetDlgItem(IDC_LO_PLATE_ALLOWFORCE); //m_Sa
	CStatic *p20 = (CStatic *)GetDlgItem(IDC_LO_PLATE_REALFORCE); //m_Ssl
	str.Format("%.2f", m_Sa);
	str1.Format("%.2f", m_Ssl);
	p19->SetWindowText(str);
	p20->SetWindowText(str1);

	CStatic *p22 = (CStatic *)GetDlgItem(IDC_LO_PLATE_ALLOW_SUMFORCE); // m_Ra
	CStatic *p23 = (CStatic *)GetDlgItem(IDC_LO_PLATE_REAL_SUMFORCE); // m_Rl
	str.Format("%.2f", m_Ra);
	str1.Format("%.2f", m_Rl);
	p22->SetWindowText(str);
	p23->SetWindowText(str1);

	// web splice
	CStatic *p42 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_ALLOW_FORCE); //m_Ra
	str.Format("%.2f", m_Ra);
	p42->SetWindowText(str);

	CStatic *p44 = (CStatic *)GetDlgItem(IDC_WEB_BOLT_REAL_FORCE1); //m_SaAr[0]
	if (m_SaAr[0] >= 0.0)
	{
		str1.Format("%.2f", m_SaAr[0]);
		p44->SetWindowText(str1);
	}
	else   {
		p44 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_TEXT1);
		p44->EnableWindow (FALSE);
		p44 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_UNIT1);
		p44->EnableWindow (FALSE);
	}
	CStatic *p46 = (CStatic *)GetDlgItem(IDC_WEB_BOLT_REAL_FORCE2); //m_SaAr[1]
	if (m_SaAr[1] >= 0.0)   {
		str1.Format("%.2f", m_SaAr[1]);
		p46->SetWindowText(str1);
	}
	else   {
		p46 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_TEXT2);
		p46->EnableWindow (FALSE);
		p46 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_UNIT2);
		p46->EnableWindow (FALSE);
	}
	CStatic *p48 = (CStatic *)GetDlgItem(IDC_WEB_BOLT_REAL_FORCE3); //m_SaAr[2]
	if (m_SaAr[2] >= 0.0)   {
		str1.Format("%.2f", m_SaAr[2]);
		p48->SetWindowText(str1);
	}
	else   {
		p48 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_TEXT3);
		p48->EnableWindow (FALSE);
		p48 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_UNIT3);
		p48->EnableWindow (FALSE);
	}
	CStatic *p50 = (CStatic *)GetDlgItem(IDC_WEB_BOLT_REAL_FORCE4); //m_SaAr[3]
	if (m_SaAr[3] >= 0.0)   {
		str1.Format("%.2f", m_SaAr[3]);
		p50->SetWindowText(str1);
	}
	else   {
		p50 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_TEXT4);
		p50->EnableWindow (FALSE);
		p50 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_UNIT4);
		p50->EnableWindow (FALSE);
	}
	CStatic *p52 = (CStatic *)GetDlgItem(IDC_WEB_BOLT_REAL_FORCE5); //m_SaAr[4]
	if (m_SaAr[4] >= 0.0)   {
		str1.Format("%.2f", m_SaAr[4]);
		p52->SetWindowText(str1);
	}
	else   {
		p52 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_TEXT5);
		p52->EnableWindow (FALSE);
		p52 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_UNIT5);
		p52->EnableWindow (FALSE);
	}
	CStatic *p54 = (CStatic *)GetDlgItem(IDC_WEB_BOLT_REAL_FORCE6); //m_SaAr[5]
	if (m_SaAr[5] >= 0.0)   {
		str1.Format("%.2f", m_SaAr[5]);
		p54->SetWindowText(str1);
	}
	else   {
		p54 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_TEXT6);
		p54->EnableWindow (FALSE);
		p54 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_UNIT6);
		p54->EnableWindow (FALSE);
	}

	CStatic *p57 = (CStatic *)GetDlgItem(IDC_WEB_SHEAR_REAL_FORCE); //m_Rwh
	str1.Format("%.2f", m_Rwh);
	p57->SetWindowText(str1);

	CStatic *p60 = (CStatic *)GetDlgItem(IDC_WEB_REAL_SUMFORCE); //m_Rw
	str1.Format("%.2f", m_Rw);
	p60->SetWindowText(str1);

	CStatic *p62 = (CStatic *)GetDlgItem(IDC_WEB_PLATE_ALLOWFORCE); // m_Sa
	CStatic *p63 = (CStatic *)GetDlgItem(IDC_WEB_UPPLATE_REALFORCE); //m_Swu
	str.Format("%.2f", m_Sa);
	str1.Format("%.2f", m_Swu);
	p62->SetWindowText(str);
	p63->SetWindowText(str1);

	CStatic *p65 = (CStatic *)GetDlgItem(IDC_WEB_LOPLATE_REALFORCE); //m_Swl
	str1.Format("%.2f", m_Swl);
	p65->SetWindowText(str1);

	// 모재 응력검토
	CStatic *p70 = (CStatic *)GetDlgItem(IDC_FLANGE_ALLOWFORCE); //m_Sa
	str1.Format("%.2f", m_Sa);
	p70->SetWindowText(str1);

	CStatic *p71 = (CStatic *)GetDlgItem(IDC_FLANGE_REALFORCE); //m_Sft
	str1.Format("%.2f", m_Sft);
	p71->SetWindowText(str1);
}

void CSpliceDesignResult::SetSpliceDesignData()
{
	CPlateStressOut	pStressOut(m_pStd);
 
	CMap<CString, LPCTSTR, double, double> MapSplice;
	pStressOut.SpliceCheckOutput(m_CalcArr, m_nG, m_nSp, MapSplice);

	int nRow, nCol;
	nRow = nCol = 0;
//------------------------------------------------------------------
	// 공통
	if (!FindString (m_CalcArr, "검토단면 :", nRow, nCol))  return;
	BOOL bIsPlus = m_CalcArr.GetAt (nRow).Find ("정") >= 0;

	m_Sa = MapSplice["Safu"];  // 허용 합성 전단력
	m_Ra = MapSplice["BOLT_ALLOW_STRESS"];  // 이음판 허용 응력 (볼트의 허용 전단력)
//------------------------------------------------------------------
	// 상판
	m_Su = MapSplice["Su"];						// 이음판 설계 응력
	m_Ss = MapSplice["SPLICE_STRESS_UP"];		// 이음판 실제 응력
	m_n  = MapSplice["REQ_BOLTSU_UP"];			// 필요 볼트 수
	m_na = MapSplice["USE_BOLTSU_UP"];			// 현재 볼트 수
	m_Ass= MapSplice["REQ_AS_UP"];				// 이음판 필요 단면적
	m_As = MapSplice["USE_AS_UP"];				// 이음판 실제 단면적
	m_R  = MapSplice["BOLT_ACTING_STRESS_UP"];	// 실제 합성 전단력

	if (!bIsPlus)   m_Sft = MapSplice["인장모재작용응력_UP"];

	// 하판
	m_Sl  = MapSplice["Sl"];					// 이음판 설계 응력
	m_Ssl = MapSplice["SPLICE_STRESS_DN"];		// 이음판 실제 응력
	m_nl  = MapSplice["REQ_BOLTSU_DN"];			// 필요 볼트 수
	m_nal = MapSplice["USE_BOLTSU_DN"];			// 현재 볼트 수
	m_Assl= MapSplice["REQ_AS_DN"];				// 이음판 필요 단면적
	m_Asl = MapSplice["USE_AS_DN"];				// 이음판 실제 단면적
	m_Rl  = MapSplice["BOLT_ACTING_STRESS_DN"];	// 실제 합성 전단력

	if (bIsPlus)   	m_Sft = MapSplice["인장모재작용응력_DN"];

	// 복부판-여긴 종전방식으로 그냥함...
	int nRow2 = nRow;
	for (int nLine = 0; nLine < 6; nLine++)  
	{
		if (!FindString (m_CalcArr, "열 :", nRow2, nCol))   {
			m_SaAr [nLine] = -1.0;
			continue;
		}
		if (!FindString (m_CalcArr, "N/EA", nRow2, nCol))  return;
		m_SaAr [nLine] = atof ((LPCTSTR) m_CalcArr.GetAt (nRow2) + nCol - 10);
	}
	if (!FindString (m_CalcArr, "ρs =", nRow, nCol))  return;
	if (!FindString (m_CalcArr, "EA  =", nRow, nCol))  return;
	m_Rwh = atof ((LPCTSTR) m_CalcArr.GetAt (nRow) + nCol + 7);

	if (!FindString (m_CalcArr, "√( ρP²+ ρs ²)", nRow, nCol))  return;
	if (!FindString (m_CalcArr, ") =", nRow, nCol))  return;
	m_Rw = atof ((LPCTSTR) m_CalcArr.GetAt (nRow) + nCol + 5);

	if (!FindString (m_CalcArr, "f sa", nRow, nCol))  return;
	m_Swu = atof ((LPCTSTR) m_CalcArr.GetAt (nRow) + nCol - 16);

	if (!FindString (m_CalcArr, "f sa", nRow, ++nCol))  return;
	m_Swl = atof ((LPCTSTR) m_CalcArr.GetAt (nRow) + nCol - 16);

	m_Rw = MapSplice["WEB_COMPOSE_STRESS"];
	m_Swu = MapSplice["UP_WEB_STRESS"];
	m_Swl = MapSplice["DN_WEB_STRESS"];
}

void CSpliceDesignResult::OnSpliceDesignResultToText() 
{
	CCalcData *pData = m_pStd->GetDataManage()->GetCalcData();
	if(pData->m_bReadFrameModelingOutput==FALSE)
	{
		AfxMessageBox("구조해석 Data가 없습니다.\n모델링 실행 또는 결과파일을 읽어주십시오.");
		return;
	}
	
	CPlateStressOut	pStressOut(m_pStd);
 
	CMap<CString, LPCTSTR, double, double> MapSplice;
	CStringArray Arr;
	pStressOut.SpliceCheckOutput(Arr, m_nG, m_nSp, MapSplice);
	m_pStd->OutputViewOnAView(Arr);
	
}

int CSpliceDesignResult::FindString(CStringArray &Arr, char *pFindStr, int &nRow, int &nCol)
{
	int nRowSu = Arr.GetSize ();
	while (nRow < nRowSu && (nCol = Arr.GetAt (nRow).Find (pFindStr, nCol)) < 0)   {
		nRow++;
		nCol = 0;
	}
	if (nRow >= nRowSu)   // 못찾음.
		return 0;
	return 1;
}

CSpliceDesignResult::SP_RESULT CSpliceDesignResult::GetDesignResult(UINT nID)
{
	SP_RESULT nResult = NONE;

	switch(nID)
	{
	case IDC_UP_BOLT_REALNO_OK:
		{
			if(m_n <= m_na)
				return OK;
			else
				return NG;
		}
	case IDC_UP_PLATE_REALAREA_OK :
		{
			if(m_As <= m_Ass)
				return OK;
			else
				return NG;
		}
	case IDC_UP_PLATE_REALFORCE_OK :
		{
			if(m_Ss <= m_Sa)
				return OK;
			else
				return NG;
		}
	case IDC_UP_PLATE_REAL_SUMFORCE_OK :
		{
			if(m_R <= m_Ra)
				return OK;
			else
				return NG;
		}
	case IDC_LO_BOLT_REALNO_OK :
		{
			if(m_nl <= m_nal)
				return OK;
			else
				return NG;
		}
	case IDC_LO_PLATE_REALAREA_OK :
		{
			if(m_Asl <= m_Assl)
				return OK;
			else
				return NG;
		}
	case IDC_LO_PLATE_REALFORCE_OK :
		{
			if(m_Ssl <= m_Sa)
				return OK;
			else
				return NG;
		}
	case IDC_LO_PLATE_REAL_SUMFORCE_OK :
		{
			if(m_Rl <= m_Ra)
				return OK;
			else
				return NG;
		}
	case IDC_WEB_BOLT_REAL_FORCE1_OK :
		{
			if(m_SaAr[0] >= 0.0)
			{
				if(m_Ra > m_SaAr[0])
					return OK;
				else
					return NG;
			}
		}
	case IDC_WEB_BOLT_REAL_FORCE2_OK :
		{
			if(m_SaAr[1] >= 0.0)
			{
				if(m_Ra > m_SaAr[1])
					return OK;
				else
					return NG;
			}
		}
	case IDC_WEB_BOLT_REAL_FORCE3_OK :
		{
			if(m_SaAr[2] >= 0.0) 
			{
				if(m_Ra > m_SaAr[2])
					return OK;
				else
					return NG;
			}
		}
	case IDC_WEB_BOLT_REAL_FORCE4_OK :
		{
			if(m_SaAr[3] >= 0.0)
			{
				if(m_Ra > m_SaAr[3])
					return OK;
				else
					return NG;
			}
		}
	case IDC_WEB_BOLT_REAL_FORCE5_OK :
		{
			if(m_SaAr[4] >= 0.0)
			{
				if(m_Ra > m_SaAr[4])
					return OK;
				else
					return NG;
			}
		}
	case IDC_WEB_BOLT_REAL_FORCE6_OK :
		{
			if(m_SaAr[5] >= 0.0)
			{
				if(m_Ra > m_SaAr[5])
					return OK;
				else
					return NG;
			}
		}
	case IDC_WEB_SHEAR_REAL_FORCE_OK :
		{
			if(m_Ra >= m_Rwh)
				return OK;
			else 
				return NG;
		}
	case IDC_WEB_REAL_SUMFORCE_OK :
		{
			if(m_Ra >= m_Rw)
				return OK;
			else
				return NG;
		}
	case IDC_WEB_UPPLATE_REALFORCE_OK :
		{
			if(m_Sa >= m_Swu)
				return OK;
			else
				return NG;
		}
	case IDC_WEB_LOPLATE_REALFORCE_OK :
		{
			if(m_Sa >= m_Swl)
				return OK;
			else
				return NG;
		}
	case IDC_FLANGE_REALFORCE_OK :
		{
			if(m_Sa >= m_Sft)
				return OK;
			else
				return NG;
		}
	}
	
	return nResult;
}

HBRUSH CSpliceDesignResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
			{
				int nID = pWnd->GetDlgCtrlID();
				SP_RESULT nSpResult = GetDesignResult(nID);
			
				if(nSpResult == OK)	
				{ 
					pDC->SetTextColor(RGB(0, 0, 128));
					pWnd->SetWindowText(_T("0.K."));
				}
				else if( nSpResult == NG)
				{ 
					pDC->SetTextColor(RGB(255,0,0));
					pWnd->SetWindowText(_T("N.G."));
				}
			}
		break;
	}

    return hbr;
}

void CSpliceDesignResult::OnPaint() 
{
	CPaintDC dc(this);  
	DrawResult();
}
