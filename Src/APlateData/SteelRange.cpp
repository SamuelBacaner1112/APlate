// SteelRange.cpp: implementation of the CSteelRange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelRange.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define DATASU	3

CSteelRange::CSteelRange(CDataManage	*pManage)
{
	m_pDataManage = pManage;

	ClearKey();
	SetDataInit();
}

CSteelRange::~CSteelRange()
{
	RemoveAll();
}

void CSteelRange::RemoveAll()
{
	long size = m_SteelRangeArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_SteelRangeArr[n];
	m_SteelRangeArr.RemoveAll();
}

void CSteelRange::Serialize(CArchive& ar)
{
	long size = 0;
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		size = 	m_SteelRangeArr.GetSize();
		ar << size;
		for(long n = 0 ; n < size ; n++)
			m_SteelRangeArr[n]->Serialize(ar);
	}
	else
	{
		RemoveAll();
		ar >> nFlag;
		ar >> size;
		for(long n = 0 ; n < size ; n++)
		{
			CSteelRangeData *pData = new CSteelRangeData;
			pData->Serialize(ar);
			m_SteelRangeArr.Add(pData);
		}
	}
}

void CSteelRange::SetDataInit()
{
	CGlobarOption	*pOpt		= m_pDataManage->GetGlobalOption();
	CStringArray	strArrStandCode, strArrSteelCode,	StrArr;
	CString			strSteelRangeRow;	
	CDBAllowStressDataLoad DBAllowDataLoad(pOpt->GetSteelStandardYear(), ROADTYPE);
	
	DBAllowDataLoad.GetStandardCodeAll(strArrStandCode);
	DBAllowDataLoad.GetSteelCodeAll(strArrSteelCode);
	long nIdx = 0;
	for(nIdx = 0; nIdx < strArrSteelCode.GetSize(); nIdx++)
	{
		strSteelRangeRow.Format("±¸Á¶¿ë °­Àç, %s, %s", strArrStandCode.GetAt(nIdx), strArrSteelCode.GetAt(nIdx));
		StrArr.Add(strSteelRangeRow);
	}

	StrArr.Add("ºÀ°­,  KS D 3504,  SD30");
	StrArr.Add("ºÀ°­,  KS D 3504,  SD35");
	StrArr.Add("ºÀ°­,  KS D 3504,  SD40");
	StrArr.Add("ºÀ°­,  KS D 3005,  SBPR 785/930");
	StrArr.Add("ºÀ°­,  KS D 3005,  SBPR 785/1030");
	StrArr.Add("ºÀ°­,  KS D 3005,  SBPR 930/1080");
	StrArr.Add("ºÀ°­,  KS D 3005,  SBPR 930/1180");
	StrArr.Add("º¼Æ®,  KS B 1010,  F8T");
	StrArr.Add("º¼Æ®,  KS B 1010,  F10T");
	StrArr.Add("º¼Æ®,  KS B 1010,  S10T");
	StrArr.Add("º¼Æ®,  KS B 1010,  F13T");
	StrArr.Add("º¼Æ®,  KS B 1010,  S13T");
	StrArr.Add("°­°ü,  KS D 3566,  SPS41");
	StrArr.Add("°­°ü,  KS D 3566,  SPS50");
	
	RemoveAll();
	
	CStringArray	strArrResult;

	for(nIdx = 0; nIdx < StrArr.GetSize(); nIdx++)
	{
		if( DivideStr(strArrResult, StrArr.GetAt(nIdx),	',') == FALSE) continue;

		CSteelRangeData *pData = new CSteelRangeData;

		pData->m_strVarName		=	strArrResult[0];
		pData->m_strStandard	=	strArrResult[1];
		pData->m_strSymbol		=	strArrResult[2];

		m_SteelRangeArr.Add(pData);
	}
}

void CSteelRange::ClearKey()
{
	m_KeyRec.m_strVarName = "";
	m_KeyRec.m_strStandard= "";
	m_KeyRec.m_strSymbol  = "";
	m_idx	= 0;
	m_Key   = 0;
}

void CSteelRange::AllocKey(CSteelRangeData* pse, long KeyMode)
{
	ClearKey();
	m_Key = KeyMode;
	if(m_Key & BDISR_VARNAME)	m_KeyRec.m_strVarName	= pse->m_strVarName;
	if(m_Key & BDISR_STANDARD)	m_KeyRec.m_strStandard	= pse->m_strStandard;
	if(m_Key & BDISR_SYMBOL)	m_KeyRec.m_strSymbol	= pse->m_strSymbol;
}

BOOL	CSteelRange::StartKey()
{
	long size;
	m_idx = 0;
	
	size = GetSize();
	while(m_idx < size) {
		if(m_Key == 0)	return TRUE;
		if(m_Key & BDISR_VARNAME) {
			if(m_KeyRec.m_strVarName	!= m_SteelRangeArr[m_idx]->m_strVarName) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISR_STANDARD) {
			if(m_KeyRec.m_strStandard	!= m_SteelRangeArr[m_idx]->m_strStandard) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISR_SYMBOL) {
			if(m_KeyRec.m_strSymbol		!= m_SteelRangeArr[m_idx]->m_strSymbol) {
				m_idx++;	continue;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL	CSteelRange::NextKey()
{
	long size;
	
	m_idx++;
	size = GetSize();
	while(m_idx < size) {
		if(m_Key == 0)	return TRUE;
		if(m_Key & BDISR_VARNAME) {
			if(m_KeyRec.m_strVarName	!= m_SteelRangeArr[m_idx]->m_strVarName) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISR_STANDARD) {
			if(m_KeyRec.m_strStandard	!= m_SteelRangeArr[m_idx]->m_strStandard) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISR_SYMBOL) {
			if(m_KeyRec.m_strSymbol		!= m_SteelRangeArr[m_idx]->m_strSymbol) {
				m_idx++;	continue;
			}
		}
		return TRUE;
	}
	return FALSE;
}

CSteelRangeData*CSteelRange::GetRecord(CSteelRangeData* pse)
{
	long size;

	size = GetSize();
	if(m_idx >= size) {
		pse->m_strVarName  = "";
		pse->m_strStandard = "";
		pse->m_strSymbol   = "";
		return NULL;
	}
	pse->m_strVarName    = m_SteelRangeArr[m_idx]->m_strVarName;
	pse->m_strStandard	 = m_SteelRangeArr[m_idx]->m_strStandard;
	pse->m_strSymbol	 = m_SteelRangeArr[m_idx]->m_strSymbol;
	return(pse);
}

CString CSteelRange::GetStringData(long idx, STLRANGE_DATA_ID ID)
{
	CString str = "";
	if(idx >= GetSize())	return str;

	switch(ID) {
	case BDISR_VARNAME:		return m_SteelRangeArr[idx]->m_strVarName;
	case BDISR_STANDARD:	return m_SteelRangeArr[idx]->m_strStandard;
	case BDISR_SYMBOL:		return m_SteelRangeArr[idx]->m_strSymbol;
	default:	str = "Out of Range ID";
	}
	return str;
}