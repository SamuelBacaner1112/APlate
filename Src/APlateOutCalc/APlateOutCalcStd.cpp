// APlateOutCalcStd.cpp: implementation of the CAPlateOutCalcStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "APlateOutCalc.h"
#include "../PlateGangje\Gangje.h"
#include "APlateOutCalcStd.h"
#include "../APlateCalc/APlateCalc.h"
#include "../APlateData/APlateData.h"
#include "../APlateDBDraw/APlateDBDraw.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutCalcStd::CAPlateOutCalcStd(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd, CDocument *pDoc)
{
	APlateOutCalcStdInit(pDataManage, pCalcStd, pDoc);
	m_nBridgeType = 1;
	m_pCWnd	= NULL;
}

CAPlateOutCalcStd::~CAPlateOutCalcStd()
{
	APlateOutCalcClose();
}

void CAPlateOutCalcStd::APlateOutCalcStdInit(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd, CDocument *pDoc)
{
	m_pDataManage		= pDataManage;
	m_pDoc				= pDoc;
	m_pCalcStd			= pCalcStd;
	m_pTStd				= m_pDataManage->GetBMStandard()->m_pTStandard;
	
	m_pAPlateOutputDlg	= new CAPlateOutputDlg;
	m_pAPlateOutputDlg->m_pAPlateOutCalcStd = this;	
	m_pAPlateOutExcel	= new CAPlateOutExcel;
	m_pAPlateOutExcel->m_pAPlateOutCalcStd = this;	
	m_pAPlateOutExcelCheck	= new CAPlateOutExcelCheck;
	m_pAPlateOutExcelCheck->m_pAPlateOutCalcStd = this;	
	m_pAPlateOutCalcXL	= new CAPlateOutCalcXL;
	m_pAPlateOutCalcXL->m_pAPlateOutCalcStd = this;	
	m_pAPlateDrawDanmyun	= new CAPlateDrawDanmyun(pDataManage);	
	m_pAPlateDrawCross	= new CAPlateDrawCross(pDataManage);
	m_pBridgeApp		= m_pDataManage->GetBridge();	
	m_pCalcFloor		= new CCalcFloor(pDataManage);	
	m_pCalcGeneral		= new CCalcGeneral(pDataManage);	
	m_pCalcFemModelingCalc = new CFemModelingCalc(pDataManage, m_pCalcStd);	
	m_pCalcFemStress    = new CFemStressCalc(pDataManage, m_pCalcStd);
	// 구조계산 Class	
	m_pXLDesignCondition = new CXLDesignCondition(this, m_pAPlateOutCalcXL);	
	m_pXLDesignFloor	= new CXLDesignFloor(this, m_pAPlateOutCalcXL);	
	m_pXLDesignOfMold	= new CXLDesignOfMold(this, m_pAPlateOutCalcXL);
	m_pXLSectionCheck	= new CXLSectionCheck(this, m_pAPlateOutCalcXL);	
	m_pXLSpliceCheck	= new CXLSpliceCheck(this, m_pAPlateOutCalcXL);	
	m_pXLWeldingCheck	= new CXLWeldingCheck(this, m_pAPlateOutCalcXL);	
	m_pXLDesignStiff	= new CXLDesignStiff(this, m_pAPlateOutCalcXL);	
	m_pXLDesignCrossBeam= new CXLDesignCrossBeam(this, m_pAPlateOutCalcXL);	
	m_pXLDesignBracing	= new CXLDesignBracing(this, m_pAPlateOutCalcXL);	
	m_pXLDesignStringer	= new CXLDesignStringer(this, m_pAPlateOutCalcXL);	
	m_pXLDesignStud		= new CXLDesignStud(this, m_pAPlateOutCalcXL);	
	m_pXLUsingCheck		= new CXLUsingCheck(this, m_pAPlateOutCalcXL);	
	m_pXLEtcCheck		= new CXLEtcCheck(this, m_pAPlateOutCalcXL);	
	m_pXLCheckListFloor	= new CXLCheckListFloor(this, m_pAPlateOutCalcXL);	
	m_pXLCheckListSection	= new CXLCheckListSection(this, m_pAPlateOutCalcXL);	
	m_pXLCheckListStiff	= new CXLCheckListStiff(this, m_pAPlateOutCalcXL);	
	m_pXLCheckListUsing	= new CXLCheckListUsing(this, m_pAPlateOutCalcXL);	
	m_pXLCheckListCrossBeam	= new CXLCheckListCrossBeam(this, m_pAPlateOutCalcXL);
	m_pXLCheckListStud	= new CXLCheckListStud(this, m_pAPlateOutCalcXL);
	m_dXLVersion	= 0;
}

int CAPlateOutCalcStd::MakeAnalysisCal()
{
	return 0;
}

void CAPlateOutCalcStd::APlateOutCalcClose()
{
	delete m_pAPlateOutputDlg;
	delete m_pAPlateOutExcel;
	delete m_pAPlateOutExcelCheck;
	delete m_pAPlateOutCalcXL;
	delete m_pAPlateDrawDanmyun;
	delete m_pAPlateDrawCross;
	delete m_pCalcFloor;
	delete m_pCalcGeneral;
	delete m_pCalcFemModelingCalc;
	delete m_pCalcFemStress;

// 구조계산 Class
	delete m_pXLDesignCondition;
	delete m_pXLDesignFloor;
	delete m_pXLDesignOfMold;
	delete m_pXLSectionCheck;
	delete m_pXLSpliceCheck;
	delete m_pXLDesignStiff;
	delete m_pXLDesignCrossBeam;
	delete m_pXLDesignBracing;
	delete m_pXLDesignStringer;
	delete m_pXLDesignStud;
	delete m_pXLUsingCheck;
	delete m_pXLEtcCheck;
	delete m_pXLWeldingCheck;
	delete m_pXLCheckListFloor;
	delete m_pXLCheckListSection;
	delete m_pXLCheckListStiff;
	delete m_pXLCheckListUsing;
	delete m_pXLCheckListCrossBeam;
	delete m_pXLCheckListStud;
}

void CAPlateOutCalcStd::SetCWnd(CWnd *pCWnd)
{
	m_pCWnd = pCWnd;
}

void CAPlateOutCalcStd::SetBridgeType(CString szBridgeType)
{
	if(szBridgeType == _T("판형교"))	m_nBridgeType = 0;
	if(szBridgeType == _T("소수주거더교"))	m_nBridgeType = 1;
}
