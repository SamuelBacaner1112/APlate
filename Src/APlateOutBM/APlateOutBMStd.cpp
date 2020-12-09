// APlateOutBMStd.cpp: implementation of the CAPlateOutBMStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutBM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPlateOutBMStd::CAPlateOutBMStd(CDataManage *pDataManage, CGeneralBaseStd *pGenInputStd, COptionStd *pOptionStd)
{
	CAPlateOutBMStdInit(pDataManage, pGenInputStd, pOptionStd);
}

CAPlateOutBMStd::~CAPlateOutBMStd()
{
	CAPlateOutBMStdClose();
}

void CAPlateOutBMStd::CAPlateOutBMStdInit(CDataManage *pDataManage, CGeneralBaseStd *pGenInputStd, COptionStd *pOptionStd)
{
	m_pDataManage		= pDataManage;
	m_pGenInputStd		= pGenInputStd;	
	m_pOptionStd		= pOptionStd;
	m_pDB				= m_pDataManage->GetBridge();
		
	m_pAPlateOutBMDlg		= new CAPlateOutBMDlg(this);
	m_pAPlateBMXL			= new CAPlateBMXL(this);
	m_pAPlateOutBMFormat	= new CAPlateOutBMFormat(this);
	m_pAPlateOutNonDesDlg	= new CAPlateOutNonDestructionDlg(this);
	m_pAPlateNonDesXL		= new CAPlateNonDestructionXL(this);
	m_pAPlateWeldChangeDlg	= new CAPlateOutWeldChangeDlg(this);
	m_pAPlateWeldChangeXL	= new CAPlateWeldChangeXL(this);
	m_dXLVersion			= 0.0;
}

void CAPlateOutBMStd::CAPlateOutBMStdClose()
{
	delete m_pAPlateOutBMDlg;
	delete m_pAPlateBMXL;
	delete m_pAPlateOutBMFormat;
	delete m_pAPlateOutNonDesDlg;
	delete m_pAPlateNonDesXL;	
	delete m_pAPlateWeldChangeDlg;
	delete m_pAPlateWeldChangeXL;
}

CString CAPlateOutBMStd::GetCircleText(long nNumber)
{
	const long nCount = 41;
	CString sCircleText[nCount] = 
	{
		"¨ç", "¨è", "¨é", "¨ê", "¨ë", "¨ì", "¨í", "¨î", "¨ï", "¨ð",
		"¨ñ", "¨ò", "¨ó", "¨ô", "¨õ", "¨Í", "¨Î", "¨Ï", "¨Ð", "¨Ñ",
		"¨Ò", "¨Ó", "¨Ô", "¨Õ", "¨Ö", "¨×", "¨Ø", "¨Ù", "¨Ú", "¨Û",
		"¨Ü", "¨Ý", "¨Þ", "¨ß", "¨à", "¨á", "¨â", "¨ã", "¨ä", "¨å", 
		"¨æ"
	};

	if(nNumber <= 0 || nNumber >= nCount) return "";
	return sCircleText[nNumber-1];
}