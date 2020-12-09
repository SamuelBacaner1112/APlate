// DataManage.cpp: implementation of the CDataManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "DataManage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGlobarOption* CDataManage::m_pGlobarOption = NULL;

CDataManage::CDataManage()
{
	m_pGlobarOption = new CGlobarOption; 
	m_pConfig = new CARoadOptionStd(DRW_APIER);
	m_pLineInfoApp = new CLineInfoApp;
	m_pBridge = new CPlateBridgeApp(m_pLineInfoApp);
	m_pBridge->m_pDataManage = this;
	m_pBridge->m_pLine = m_pLineInfoApp;	

	m_pSteelConc = new CSteelConc; //CCalcData생성자에서 CSteelConc를 먼저 생성해서
	
	m_pCalcData = new CCalcData(this);
	m_pDeckData = new CADeckData(this);
	m_pDeckDataTU = new CADeckDataTU(this);

	m_pSteel = new CSteel;
	m_pSteelMaterial = new CSteelMaterial;
	m_pSteelSection = new CSteelSection;
	m_pSteelDefine = new CSteelDefine;
	m_pSteelRange = new CSteelRange(this);
	m_pSteelGrade = new CSteelGrade;
	m_pSteelWeld = new CSteelWeld;
	m_pSteelDeduct = new CSteelDeduct;
	m_pSteelBolt = new CSteelBolt(this);
	m_pSteelStud = new CSteelStud;
	m_pSteelScallop = new CSteelScallop;
	m_pSteelUWeight = new CSteelUWeight;
	m_pSteelPrestress = new CSteelPrestressPlate;

	m_pSteelMark = new CSteelMark;
}

CDataManage::~CDataManage()
{	
	delete m_pSteelMark;

	delete m_pSteelConc;
	delete m_pSteelUWeight;
	delete m_pSteelScallop;
	delete m_pSteelBolt;
	delete m_pSteelStud;
	delete m_pSteelDeduct;
	delete m_pSteelWeld;
	delete m_pSteelGrade;
	delete m_pSteelRange;
	delete m_pSteelDefine;
	delete m_pSteelSection;
	delete m_pSteelMaterial;
	delete m_pSteel;
	delete m_pSteelPrestress;

	delete m_pLineInfoApp;
	delete m_pBridge;
	delete m_pConfig;
	delete m_pGlobarOption;

	delete m_pCalcData;
	delete m_pDeckData; 
	delete m_pDeckDataTU; 


}

CGlobarOption *CDataManage::GetGlobalOption()
{
	return m_pGlobarOption;
}

void CDataManage::Serialize(CArchive &ar)
{
//	long nFlag = 2;
//	long nFlag = 3;	// SteelBoltSize Class 삭제
//	long nFlag = 4; // 강재DB 추가
	long nFlag = 5;	// CSteelPrestress Engin 사용에 따른 변경

	if(ar.IsStoring())
	{
		ar << nFlag;			
	}
	else
	{
		ar >> nFlag;		
	}	

	m_pLineInfoApp->Serialize(ar);
	m_pBridge->Serialize(ar);
	m_pConfig->Serialize(ar);		
	m_pGlobarOption->Serialize(ar);	
	
	if(nFlag < 4 && ar.IsLoading())
	{
		long	nFlag1	= 0;
		long	nSize	= 0;
		long	nTemp	= 0;
		double	dTemp	= 0.0;
		CString	strTemp	= _T("");

		ar >> nTemp;
		ar >> nSize;
		
		for(long n = 0; n < nSize; n++)
		{
			ar >> nFlag1;

			for(long nSIdx = 0; nSIdx < 3; nSIdx++)
				ar >> strTemp;
			for(long nDIdx = 0; nDIdx < 35; nDIdx++)
				ar >> dTemp;
			
			if(nFlag1 > 0)
			{
				for(long nDIdx = 0; nDIdx < 16; nDIdx++)
					ar >> dTemp;
			}
		}
		
		long nApplyYear = m_pGlobarOption->GetSteelStandardYear();
		
		m_pSteel->SetApplyYear(nApplyYear);
		m_pSteel->SetDataInit();
	}

	if(nFlag > 3)
	{
		long nApplyYear = m_pGlobarOption->GetSteelStandardYear();
		m_pSteel->SetBridgeType(ROADTYPE);
		m_pSteel->SetApplyYear(nApplyYear);
		m_pSteel->Serialize(ar);
	}

	m_pSteelMaterial->Serialize(ar);
	m_pSteelSection->Serialize(ar);
	m_pSteelDefine->Serialize(ar);
	
	m_pSteelRange->Serialize(ar);


	m_pSteelGrade->Serialize(ar);
	m_pSteelWeld->Serialize(ar);
	m_pSteelDeduct->Serialize(ar);
	m_pSteelBolt->Serialize(ar);
	if(nFlag < 3)
	{
		long	nTemp, nSize;
		double	dTemp;

		ar >> nTemp;
		ar >> nSize;
		for(long n = 0 ; n < nSize ; n++) 
		{
			ar >> nTemp;
			ar >> dTemp;	ar >> dTemp;	ar >> dTemp;
			ar >> dTemp;	ar >> dTemp;	ar >> dTemp;
		}
	}
	else
		m_pSteelStud->Serialize(ar);
	
	m_pSteelScallop->Serialize(ar);
	m_pSteelUWeight->Serialize(ar);
	m_pSteelConc->Serialize(ar);

	m_pCalcData->Serialize(ar);
	if (nFlag>0)
		m_pDeckData->Serialize(ar);

	if(nFlag>0)
	{
		if(nFlag > 4)
		{
			m_pSteelPrestress->Serialize(ar);
		}
		else
		{
			m_pSteelPrestress->SerializeOld(ar);
		}
	}

	if(!ar.IsStoring())
	{
		CString csErrorMsg(_T(""));
		if( m_pLineInfoApp->SetDataIp(csErrorMsg) == FALSE )
		{
			if(csErrorMsg!="")	AfxMessageBox(csErrorMsg);
			return;
		}
	}
	if(nFlag>1)
		m_pDeckDataTU->Serialize(ar);

}

CDomyun *CDataManage::GetDomyun() const
{ 
	return m_pView->GetDomyun(); 
}

void CDataManage::SetCheckEmptyMaterial(BOOL bCheck)
{
	m_bCheckEmptyMaterial = bCheck;
	m_MapCehckEmptyMaterial.RemoveAll();
}

void CDataManage::SetCheckMap(CString szKey, CString szMessage)
{
	m_MapCehckEmptyMaterial.SetAt(LPCTSTR(szKey), LPCTSTR(szMessage));
}

CString CDataManage::GetCheckMap(CString szKey)
{
	CString szMessage = _T("");
	m_MapCehckEmptyMaterial.Lookup(LPCTSTR(szKey), (CString &)szMessage);
	return szMessage;
}

double CDataManage::GetFilletWeldStandard(double dThick1, double dThick2)
{
	double dS = 6.0;
	
	//도설 2.5.2.4
	if(m_pGlobarOption->GetSteelStandardYear() == APPLYYEAR2010)
	{
		double dMaxThick = max(dThick1, dThick2);
		if(dMaxThick > 20.0)
			dS = 8.0;
	}
	else
	{
		dS = max(RoundUp (sqrt (2 * max (dThick1, dThick2)), 0), 6.0);
	}

	
	return dS;
}