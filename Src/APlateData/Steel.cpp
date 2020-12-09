// Steel.cpp: implementation of the CSteel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Steel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteel::CSteel()
{	
	m_nApplyYear	= APPLYYEAR2008;
	m_nBridgeType	= ROADTYPE;
}

CSteel::~CSteel()
{
	RemoveAll();
}

double CSteel::GetUnitWeightByMaterial(const CString& sMaterial) const
{
// 	for(long n = 0; n < m_SteelArr.size(); n++)
// 	{
// 		if(m_SteelArr[n]->m_strSymbol == sMaterial)
// 			return m_SteelArr[n]->m_UnitWeight;
// 	}

	return 0;
}

void CSteel::RemoveAll()
{
	long size = m_SteelArr.size();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_SteelArr[n];
	m_SteelArr.clear();
}

void CSteel::Serialize(CArchive& ar)
{
	long size = 0;
	long nFlag = 0;
	if(ar.IsStoring()) 
	{
		ar << nFlag;
		size = 	m_SteelArr.size();
		ar << size;
		for(long n = 0 ; n < size ; n++)
			m_SteelArr[n]->Serialize(ar);
	}
	else 
	{
		ar >> nFlag;
		RemoveAll();
		ar >> size;
		for(long n = 0 ; n < size ; n++) 
		{
			CHGBaseSteel *pBaseSteel = CFactoryBaseSteel::CreateSteel(m_nApplyYear, m_nBridgeType);
			pBaseSteel->Serialize(ar);
			m_SteelArr.push_back(pBaseSteel);
		}
	}
}


void CSteel::SetDataInit()
{	

	CDBAllowStressDataLoad DBAllowStress(m_nApplyYear, m_nBridgeType);
	CStringArray			SArr;
	CStringArray			StrArr;
	
	DBAllowStress.GetAllowStressData(SArr);
	
	RemoveAll();
	long nSize = SArr.GetSize();

	for(long n = 0; n < nSize; n++)
	{
		if( DivideStr(StrArr, SArr.GetAt(n),',') ==FALSE) continue;		

		CHGBaseSteel *pBaseSteel =  CFactoryBaseSteel::CreateSteel(m_nApplyYear, m_nBridgeType);
		pBaseSteel->LoadBaseFile(StrArr, 0);
		m_SteelArr.push_back(pBaseSteel);
	}
}

long CSteel::GetIndex(CString strSymbol)
{
	long	nIdx	= 0;
	long	nSize	= GetSize();

	while(nIdx < nSize) 
	{
		CString	sz = GetSymbol(nIdx);
		if(strSymbol == sz)
			return nIdx;
		nIdx++;
	}

	return -1;
}

CString CSteel::GetStringData(long idx, STEEL_DATA_ID ID)
{
// 	CString str = "";
// 	if(idx >= GetSize())	return str;
// 	switch(ID) {
// //	case BDIST_VARNAME:
// 	case BDIST_STANDARD:					str = m_SteelArr[idx]->m_strStandard;							break;
// 	case BDIST_SYMBOL:						str = m_SteelArr[idx]->m_strSymbol;								break;
// 	case BDIST_STRETCHSTRESSALLOWMIN:		str.Format("%lf", m_SteelArr[idx]->m_StretchStressAllowMin);	break;
// 	case BDIST_STRETCHSTRESSALLOWMID:		str.Format("%lf", m_SteelArr[idx]->m_StretchStressAllowMid);	break;
// 	case BDIST_STRETCHSTRESSALLOWMAX:		str.Format("%lf", m_SteelArr[idx]->m_StretchStressAllowMax);	break;
// 	case BDIST_STRETCHSTRESSALLOWFACTOR:	str.Format("%lf", m_SteelArr[idx]->m_StretchStressAllowFactor);	break;	
// 	case BDIST_BOLTSTRESSFACTOR:			str.Format("%lf", m_SteelArr[idx]->m_BoltStressFactor);			break;
// 	case BDIST_SHEARSTRESSALLOWMIN:			str.Format("%lf", m_SteelArr[idx]->m_ShearStressAllowMin);		break;
// 	case BDIST_SHEARSTRESSALLOWMID:			str.Format("%lf", m_SteelArr[idx]->m_ShearStressAllowMid);		break;
// 	case BDIST_SHEARSTRESSALLOWMAX:			str.Format("%lf", m_SteelArr[idx]->m_ShearStressAllowMax);		break;
// 	case BDIST_SHEARSTRESSALLOWFACTOR:		str.Format("%lf", m_SteelArr[idx]->m_ShearStressAllowFactor);	break;
// 	case BDIST_TOTALSTRESSFACTOR:			str.Format("%lf", m_SteelArr[idx]->m_TotalStressFactor);		break;
// 	case BDIST_UNITWEIGHT:					str.Format("%lf", m_SteelArr[idx]->m_UnitWeight);				break;
// 	case BDIST_BOARDTHICKARROW:				str.Format("%lf", m_SteelArr[idx]->m_dThickAllowMax);			break;
// 	case BDIST_MARK:						str = m_SteelArr[idx]->m_strMark;								break;
// 	case BDIST_ALPHA:						str.Format("%lf", m_SteelArr[idx]->m_Alpha);					break;
// 	case BDIST_ELASTICMODULUS:				str.Format("%lf", m_SteelArr[idx]->m_ElasticModulus);			break;
// 	case BDIST_TEMPERATURE:					str.Format("%lf", m_SteelArr[idx]->m_Temperature);				break;
// 	case BDIST_MINFACTOR:					str.Format("%lf", m_SteelArr[idx]->m_MinFactor);				break;
// 	case BDIST_MAXFACTOR:					str.Format("%lf", m_SteelArr[idx]->m_MaxFactor);				break;
// 	case BDIST_STRESSFACTOR:				str.Format("%lf", m_SteelArr[idx]->m_StressFactor);				break;
// 	case BDIST_AXISMIN:						str.Format("%lf", m_SteelArr[idx]->m_AxisMin);					break;
// 	case BDIST_AXISMID:						str.Format("%lf", m_SteelArr[idx]->m_AxisMid);					break;
// 	case BDIST_AXISMAX:						str.Format("%lf", m_SteelArr[idx]->m_AxisMax);					break;
// 	case BDIST_BASEYIELDPOINTMIN:			str.Format("%lf", m_SteelArr[idx]->m_BaseYieldPointMin);		break;
// 	case BDIST_BASEYIELDPOINTMID:			str.Format("%lf", m_SteelArr[idx]->m_BaseYieldPointMid);		break;
// 	case BDIST_BASEYIELDPOINTMAX:			str.Format("%lf", m_SteelArr[idx]->m_BaseYieldPointMax);		break;
// 	case BDIST_SAFETYRATE:					str.Format("%lf", m_SteelArr[idx]->m_SafetyRate);				break;
// 	case BDIST_BOLTSTRESSALLOWMIN:			str.Format("%lf", m_SteelArr[idx]->m_BoltStressAllowMin);		break;
// 	case BDIST_BOLTSTRESSALLOWMID:			str.Format("%lf", m_SteelArr[idx]->m_BoltStressAllowMid);		break;
// 	case BDIST_BOLTSTRESSALLOWMAX:			str.Format("%lf", m_SteelArr[idx]->m_BoltStressAllowMax);		break;
// 	case BDIST_BOLTSTRESSALLOWFACTOR:		str.Format("%lf", m_SteelArr[idx]->m_BoltStressAllowFactor);	break;
// 	case BDIST_YIELDPOINTMIN:				str.Format("%lf", m_SteelArr[idx]->m_YieldPointMin);			break;
// 	case BDIST_YIELDPOINTMID:				str.Format("%lf", m_SteelArr[idx]->m_YieldPointMid);			break;
// 	case BDIST_YIELDPOINTMAX:				str.Format("%lf", m_SteelArr[idx]->m_YieldPointMax);			break;
// 	case BDIST_YIELDPOINTLMAX:				str.Format("%lf", m_SteelArr[idx]->m_YieldPointLMax);			break;
// 	case BDIST_STRETCHSTRENGTHMIN:			str.Format("%lf", m_SteelArr[idx]->m_StretchStrengthMin);		break;
// 	case BDIST_STRETCHSTRENGTHMAX:			str.Format("%lf", m_SteelArr[idx]->m_StretchStrengthMax);		break;
// 	case BDIST_X1:							str.Format("%lf", m_SteelArr[idx]->m_X1);						break;
// 	case BDIST_X2:							str.Format("%lf", m_SteelArr[idx]->m_X2);						break;
// 	case BDIST_BENDINGANGLE:				str.Format("%lf", m_SteelArr[idx]->m_BendingAngle);				break;
// 	case BDIST_INNERRADIUS:					str.Format("%lf", m_SteelArr[idx]->m_InnerRadius);				break;
// 	case BDIST_STRAIGHTBOARDDISTANCE:		str.Format("%lf", m_SteelArr[idx]->m_StraightBoardDistance);	break;
// 	case BDIST_LENGTHPERWEIGHT:				str.Format("%lf", m_SteelArr[idx]->m_LengthPerWeight);			break;
// 
// 	default:
// 		str = "Out of Range ID";
// 	}
	return "";
}

CString CSteel::GetSymbol(long nIdx)
{
	return m_SteelArr[nIdx]->GetSymbol();
}

CString CSteel::GetStandard(long nIdx)
{
	return m_SteelArr[nIdx]->GetStandard();
}

void CSteel::SetApplyYear(long nApplyYearIdx)
{
	m_nApplyYear = nApplyYearIdx;
}

void CSteel::SetBridgeType(long nBridgeTypeIdx)
{
	m_nBridgeType = nBridgeTypeIdx;
}

double CSteel::GetSteelAllowValue(long nAllowType, long nIdx, long nThickIdx, long nSubIdx)
{
	CHGBaseSteel *pSteel = m_SteelArr[nIdx];
	
	return	pSteel->GetSteelAllowValue(nAllowType, nThickIdx,	nSubIdx);
	
}

void CSteel::SetSteelAllowValue(long nAllowType, long nIdx, double dAllowValue, long nThickIdx, long nSubIdx)
{
	CHGBaseSteel *pSteel = m_SteelArr[nIdx];
	
	pSteel->SetSteelAllowValue(nAllowType, dAllowValue, nThickIdx, nSubIdx);
}