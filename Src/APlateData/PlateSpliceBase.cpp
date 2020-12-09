// SpliceBase.cpp: implementation of the CPlateSpliceBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateSpliceBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateSpliceBase::CPlateSpliceBase()
{
	m_nWebType = 0;
	m_uA = 0;
	m_uB = 0;	
	m_uC = 0;	
	m_uC1 = 0;
	m_uD = 0;	
	m_uG = 0;	
	m_uT = 0;	
	m_uL = 0;
	m_dA = 0;			//하부 이음판
	m_dB = 0;			//하부 이음판
	m_dC = 0;
	m_dC1 = 0;
	m_dD = 0;
	m_dG = 0;
	m_dT = 0;
	m_dL = 0;
	m_mA = 0;			//모멘트 이음판
	m_mB = 0;			//모멘트 이음판
	m_mC = 0;			//모멘트 이음판
	m_mC1 = 0;
	m_mG = 0;
	m_mT = 0;
	m_mL = 0;
	m_mE = 0;
	m_mF = 0;
	m_mW = 0;
	m_mD = 0;
	m_sA = 0;			//전단 이음판
	m_sB = 0;			//전단 이음판
	m_sC = 0;
	m_sC1 = 0;
	m_sT = 0;
	m_sL = 0;
	m_sL1 = 0; //채움판 관련..
	m_sE = 0;
	m_sF = 0;
	m_sE1 = 0;
	m_sF1 = 0;
	m_sW = 0;
	m_dBoltSize = 0;
	m_ZzsBoltSize = _T("M22");  // Bolt 규격 
	m_uE = 0;
	m_uF = 0;
	m_uW = 0;
	m_dE = 0;
	m_dF = 0;
	m_dW = 0;	
}

CPlateSpliceBase::~CPlateSpliceBase()
{

}


void CPlateSpliceBase::SerializeSpliceBase(CArchive& ar)
{
//	long nFlag = 1;
	long nFlag = 2;	// 복부판 타입추가

	CPlateBxObject::Serialize(ar);

	if(ar.IsStoring())//MAXJRIBSU
	{
		ar << nFlag;
		ar << m_uA						;		//상부 이음판
		ar << m_uB						;
		ar << m_uC						;
		ar << m_uC1						;
		ar << m_uD						;
		ar << m_uG						;
		ar << m_uT						;
		ar << m_uL						;
		ar << m_dA						;			//하부 이음판
		ar << m_dB						;			//하부 이음판
		ar << m_dC						;
		ar << m_dC1						;
		ar << m_dD						;
		ar << m_dG						;
		ar << m_dT						;
		ar << m_dL						;
		ar << m_mA						;			//모멘트 이음판
		ar << m_mB						;			//모멘트 이음판
		ar << m_mC						;			//모멘트 이음판
		ar << m_mC1						;
		ar << m_mD						;
		ar << m_mT						;
		ar << m_mL						;
		ar << m_mE						;
		ar << m_mF						;
		ar << m_mW						;
		ar << m_sA						;			//전단 이음판
		ar << m_sB						;			//전단 이음판
		ar << m_sC						;
		ar << m_sC1						;
		ar << m_sT						;
		ar << m_sL						;
		ar << m_sE						;
		ar << m_sF						;
		ar << m_sE1						;
		ar << m_sF1						;
		ar << m_sW						;
		ar << m_dBoltSize				;
		ar << m_ZzsBoltSize				;
		ar << m_uE;
		ar << m_uF;
		ar << m_uW;
		ar << m_dE;
		ar << m_dF;
		ar << m_dW;
		ar << m_uWidth;
		ar << m_dWidth;
		ar << m_mWidth;
		ar << m_mG;
		ar << m_sL1;
		ar << m_nWebType				;
	}
	else
	{
		ar >> nFlag;
		ar >> m_uA						;		//상부 이음판
		ar >> m_uB						;
		ar >> m_uC						;
		ar >> m_uC1						;
		ar >> m_uD						;
		ar >> m_uG						;
		ar >> m_uT						;
		ar >> m_uL						;
		ar >> m_dA						;			//하부 이음판
		ar >> m_dB						;			//하부 이음판
		ar >> m_dC						;
		ar >> m_dC1						;
		ar >> m_dD						;
		ar >> m_dG						;
		ar >> m_dT						;
		ar >> m_dL						;
		ar >> m_mA						;			//모멘트 이음판
		ar >> m_mB						;			//모멘트 이음판
		ar >> m_mC						;			//모멘트 이음판
		ar >> m_mC1						;
		ar >> m_mD						;
		ar >> m_mT						;
		ar >> m_mL						;
		ar >> m_mE						;
		ar >> m_mF						;
		ar >> m_mW						;
		ar >> m_sA						;			//전단 이음판
		ar >> m_sB						;			//전단 이음판
		ar >> m_sC						;
		ar >> m_sC1						;
		ar >> m_sT						;
		ar >> m_sL						;
		ar >> m_sE						;
		ar >> m_sF						;
		if(nFlag>1)
		{
			ar >> m_sE1;
			ar >> m_sF1;
		}
		ar >> m_sW						;
		ar >> m_dBoltSize				;
		ar >> m_ZzsBoltSize				;
		ar >> m_uE						;
		ar >> m_uF						;
		ar >> m_uW						;
		ar >> m_dE						;
		ar >> m_dF						;
		ar >> m_dW						;
		ar >> m_uWidth					;
		ar >> m_dWidth					;
		ar >> m_mWidth					;
		ar >> m_mG						;

		if(nFlag > 0)
			ar >> m_sL1					;

		if(nFlag>1)
			ar >> m_nWebType			;
		else if(m_mT>0)
			m_nWebType = 1;

//		m_mG = m_mD;
//		m_mD = 100;
	}
}
CPlateSpliceBase& CPlateSpliceBase::operator=(const CPlateSpliceBase& Obj)
{
	if( this == &Obj ) return *this;

	CPlateBxObject::operator = (Obj);

	m_nWebType = Obj.m_nWebType;
	m_uA  = Obj.m_uA;
	m_uB  = Obj.m_uB;
	m_uC  = Obj.m_uC;
	m_uC1 = Obj.m_uC1;
	m_uD  = Obj.m_uD;
	m_uG  = Obj.m_uG;
	m_uT  = Obj.m_uT;
	m_uL = Obj.m_uL;    
	m_dA = Obj.m_dA;			//하부 이음판
	m_dB = Obj.m_dB;			//하부 이음판
	m_dC = Obj.m_dC;
	m_dC1 = Obj.m_dC1;
	m_dD = Obj.m_dD;
	m_dG = Obj.m_dG;
	m_dT = Obj.m_dT;
	m_dL = Obj.m_dL;
	m_mA = Obj.m_mA;			//모멘트 이음판
	m_mB = Obj.m_mB;			//모멘트 이음판
	m_mC = Obj.m_mC;			//모멘트 이음판
	m_mC1 = Obj.m_mC1;
	m_mD = Obj.m_mD;
	m_mG = Obj.m_mG;
	m_mT = Obj.m_mT;
	m_mL = Obj.m_mL;
	m_mE = Obj.m_mE;
	m_mF = Obj.m_mF;
	m_mW = Obj.m_mW;
	m_sA = Obj.m_sA;			//전단 이음판
	m_sB = Obj.m_sB;			//전단 이음판
	m_sC = Obj.m_sC;
	m_sC1 = Obj.m_sC1;
	m_sT = Obj.m_sT;
	m_sL = Obj.m_sL;
	m_sL1 = Obj.m_sL1;
	m_sE = Obj.m_sE;
	m_sF = Obj.m_sF;
	m_sE1 = Obj.m_sE1;
	m_sF1 = Obj.m_sF1;
	m_sW = Obj.m_sW;
	m_dBoltSize = Obj.m_dBoltSize;
	m_uE = Obj.m_uE;      
	m_uF = Obj.m_uF;     
	m_uW = Obj.m_uW;   
	m_dE = Obj.m_dE;   
	m_dF = Obj.m_dF;   
	m_dW = Obj.m_dW;     
	m_ZzsBoltSize = Obj.m_ZzsBoltSize;

	return *this;
}
BOOL CPlateSpliceBase::operator ==(const CPlateSpliceBase & Obj)
{
	if(m_nWebType != Obj.m_nWebType)	return FALSE;
	if(m_uA  != Obj.m_uA)   return FALSE;
	if(m_uB  != Obj.m_uB)   return FALSE;
	if(m_uC  != Obj.m_uC)   return FALSE;
	if(m_uC1 != Obj.m_uC1)   return FALSE;
	if(m_uD  != Obj.m_uD)   return FALSE;
	if(m_uG  != Obj.m_uG)   return FALSE;
	if(m_uT  != Obj.m_uT)   return FALSE;
	if(m_uL != Obj.m_uL)   return FALSE;    
	if(m_dA != Obj.m_dA)   return FALSE;			//하부 이음판
	if(m_dB != Obj.m_dB)   return FALSE;			//하부 이음판
	if(m_dC != Obj.m_dC)   return FALSE;
	if(m_dC1 != Obj.m_dC1)   return FALSE;
	if(m_dD != Obj.m_dD)   return FALSE;
	if(m_dG != Obj.m_dG)   return FALSE;
	if(m_dT != Obj.m_dT)   return FALSE;
	if(m_dL != Obj.m_dL)   return FALSE;
	if(m_mA != Obj.m_mA)   return FALSE;			//모멘트 이음판
	if(m_mB != Obj.m_mB)   return FALSE;			//모멘트 이음판
	if(m_mC != Obj.m_mC)   return FALSE;			//모멘트 이음판
	if(m_mC1 != Obj.m_mC1)   return FALSE;
	if(m_mG != Obj.m_mG)   return FALSE;
	if(m_mT != Obj.m_mT)   return FALSE;
	if(m_mL != Obj.m_mL)   return FALSE;
	if(m_mE != Obj.m_mE)   return FALSE;
	if(m_mF != Obj.m_mF)   return FALSE;
	if(m_mW != Obj.m_mW)   return FALSE;
	if(m_mD != Obj.m_mD)   return FALSE;
	if(m_sA != Obj.m_sA)   return FALSE;			//전단 이음판
	if(m_sB != Obj.m_sB)   return FALSE;			//전단 이음판
	if(m_sC != Obj.m_sC)   return FALSE;
	if(m_sC1 != Obj.m_sC1)   return FALSE;
	if(m_sT != Obj.m_sT)   return FALSE;
	if(m_sL != Obj.m_sL)   return FALSE;
	if(m_sL1 != Obj.m_sL1)   return FALSE;
	if(m_sE != Obj.m_sE)   return FALSE;
	if(m_sF != Obj.m_sF)   return FALSE;
	if(m_sE1 != Obj.m_sE1)   return FALSE;
	if(m_sF1 != Obj.m_sF1)   return FALSE;
	if(m_sW != Obj.m_sW)   return FALSE;
	if(m_dBoltSize != Obj.m_dBoltSize)   return FALSE;
	if(m_uE != Obj.m_uE)   return FALSE;      
	if(m_uF != Obj.m_uF)   return FALSE;     
	if(m_uW != Obj.m_uW)   return FALSE;   
	if(m_dE != Obj.m_dE)   return FALSE;   
	if(m_dF != Obj.m_dF)   return FALSE;   
	if(m_dW != Obj.m_dW)   return FALSE;     
	if(m_ZzsBoltSize != Obj.m_ZzsBoltSize)   return FALSE;     
	

	return TRUE;
}

BOOL CPlateSpliceBase::IsTrue()
{
	if(m_uA == 0)	return FALSE;
	if(m_uB == 0)	return FALSE;
	if(m_dA == 0)	return FALSE;
	if(m_dB == 0)	return FALSE;

	return TRUE;
}

double CPlateSpliceBase::GetuL(double thickBase) const	
{
	if(m_uT <= 0)	return 0;
	
	CGlobarOption *pEtc = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetGlobalOption();

	BOOL bBoltLengthAuto = pEtc->GetBoltLengthAuto();
	if(!bBoltLengthAuto)
		return m_uL;

	CSteelBolt *pBolt = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetSteelBolt();	
	return pBolt->GetBoltLength(GetBx(), SP_U_BOLT, m_ZzsBoltSize, thickBase + m_uT*2);
}

double CPlateSpliceBase::GetdL(double thickBase) const
{
	if(m_dT <= 0)	return 0;
	CGlobarOption *pEtc = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetGlobalOption();
	BOOL bBoltLengthAuto = pEtc->GetBoltLengthAuto();
	if(!bBoltLengthAuto)
		return m_dL;

	CSteelBolt *pBolt = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetSteelBolt();	
	return pBolt->GetBoltLength(GetBx(), SP_L_BOLT, m_ZzsBoltSize, thickBase + m_dT*2);
}

double CPlateSpliceBase::GetmL(double thickBase) const
{
	if(m_mT <= 0 && m_sT <= 0)	return 0;
	CGlobarOption *pEtc = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetGlobalOption();
	BOOL bBoltLengthAuto = pEtc->GetBoltLengthAuto();
	if(!bBoltLengthAuto)
		return m_mL;

	CSteelBolt *pBolt = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetSteelBolt();	
	return pBolt->GetBoltLength(GetBx(), SP_W_BOLT, m_ZzsBoltSize, thickBase + m_mT*2);
}

double CPlateSpliceBase::GetsL(double thickBase) const
{
	if(m_sT <= 0)	return 0;
	CGlobarOption *pEtc = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetGlobalOption();
	BOOL bBoltLengthAuto = pEtc->GetBoltLengthAuto();
	if(!bBoltLengthAuto)
		return m_sL;

	CSteelBolt *pBolt = ((CPlateBridgeApp*)m_pGir->GetBridgeApp())->m_pDataManage->GetSteelBolt();	
	return pBolt->GetBoltLength(GetBx(), SP_W_BOLT, m_ZzsBoltSize, thickBase + m_sT*2);
}