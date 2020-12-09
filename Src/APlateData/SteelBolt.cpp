// SteelBolt.cpp: implementation of the CSteelBolt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelBolt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelBoltData::CSteelBoltData()
{
	m_ForceAllow = 0.0;

	m_CenterSpaceMin = 0.0;
	m_CenterSpaceMax = 0.0;
	m_ShearFaceDistanceMin = 0.0;
	m_StressFaceDistanceMin = 0.0;

	m_HoleMargin = 0.0;
	m_BaseWeight = 0.0;
	m_LengthPerWeight = 0.0;
	m_NutWeight = 0.0;
	m_WasherWeight = 0.0;
	m_Length = 0.0;
}

CSteelBoltData::~CSteelBoltData()
{

}

void CSteelBoltData::Serialize(CArchive& ar)
{
	//long nFlag = 0;
	long nFlag = 1;	// F13T 추가..0825 lina
	double dTemp;
	CString szTemp;

	if(ar.IsStoring()) 
	{
		ar << nFlag;
		ar << m_strStandard;
		ar << m_strGrade;
		ar << m_strSize;
		ar << m_ForceAllow;
		ar << m_CenterSpaceMin;
		ar << m_CenterSpaceMax;
		ar << m_ShearFaceDistanceMin;
		ar << m_StressFaceDistanceMin;
		ar << m_HoleMargin;
		ar << m_BaseWeight;
		ar << m_LengthPerWeight;
		ar << m_NutWeight;
		ar << m_WasherWeight;
		ar << m_Length;
	}
	else
	{
		ar >> nFlag;
		ar >> m_strStandard;
		if(nFlag<1)
		{
			ar >> szTemp;
			long nIdx = szTemp.Find("M");
			m_strGrade	= szTemp.Left(nIdx-1);
			m_strSize	= szTemp.Right(szTemp.GetLength()-nIdx);
		}
		else
		{
			ar >> m_strGrade;
			ar >> m_strSize;
		}

		ar >> m_ForceAllow;

		if(nFlag<1)
		{
			ar >> dTemp;
			ar >> dTemp;
			ar >> dTemp;
		}

		ar >> m_CenterSpaceMin;
		ar >> m_CenterSpaceMax;
		ar >> m_ShearFaceDistanceMin;
		ar >> m_StressFaceDistanceMin;

		if(nFlag<1)			m_HoleMargin = 3;
		else				ar >> m_HoleMargin;

		ar >> m_BaseWeight;
		ar >> m_LengthPerWeight;
		ar >> m_NutWeight;
		ar >> m_WasherWeight;

		if(nFlag<1)			m_Length = m_ShearFaceDistanceMin;
		else				ar >> m_Length;

		if(nFlag<1)
		{
			ar >> dTemp;
			ar >> dTemp;
			ar >> dTemp;
			ar >> dTemp;
			ar >> dTemp;
			ar >> dTemp;
		}		
	}
}

double	CSteelBoltData::GetBoltSize()
{
	CString szBoltSize = m_strSize;
	szBoltSize.Delete(0);
	return atof(szBoltSize);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelBolt::CSteelBolt(CDataManage *pDataManage)
{
	m_pDataManage		= pDataManage;
	m_nLengthCalcType	= 1;
}

CSteelBolt::~CSteelBolt()
{
	RemoveAll();
}

void CSteelBolt::RemoveAll()
{
	long size = m_BoltArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_BoltArr[n];
	m_BoltArr.RemoveAll();
}

void CSteelBolt::Serialize(CArchive& ar)
{
	long size = 0;
	//long nFlag = 1;
	long nFlag = 2;		// 볼트길이 산정방식 옵션 추가 ..080917 lina (APMS ARoad12008개발 1965)
	if(ar.IsStoring()) 
	{
		ar << nFlag;
		size = 	m_BoltArr.GetSize();
		ar << size;
		for(long n = 0 ; n < size ; n++)
			m_BoltArr[n]->Serialize(ar);
		ar << m_nLengthCalcType;
	}
	else 
	{
		ar >> nFlag;
		RemoveAll();
		ar >> size;
		for(long n = 0 ; n < size ; n++) 
		{
			CSteelBoltData*pData = new CSteelBoltData;
			pData->Serialize(ar);
			m_BoltArr.Add(pData);
		}
		if(nFlag>1)
			ar >> m_nLengthCalcType;
		else
		{
			// F13T작업 이전 파일 불러올경우 강제 초기화 080916 lina(APMS 리스트 프로젝트 1936)
			SetDataInit();
			m_nLengthCalcType = 0;
		}
	}
}

void CSteelBolt::SetDataInit()
{
	RemoveAll();
	char* szBaseStr[] =
	{
		"KS B 1010,  F8T,	M20,  31000.00,  65,  130,  32,  28,  3, 0.0762, 	0.00247, 	0.0970, 	0.0320, 	35",
		"KS B 1010,  F8T,	M22,  39000.00,  75,  150,  37,  32,  3, 0.1060, 	0.00298, 	0.1370, 	0.0520, 	40",
		"KS B 1010,  F8T,	M24,  45000.00,  85,  170,  42,  37,  3, 0.1450, 	0.00355, 	0.2010, 	0.0620, 	45",
		"KS B 1010,  F8T,	M27,  58000.00,  95,  190,  48,  43,  4, 0.2142, 	0.00448, 	0.2630, 	0.0930, 	50",
		"KS B 1010,  F8T,	M30,  71000.00, 105,  210,  55,  50,  4, 0.2606, 	0.00542, 	0.3400, 	0.1340, 	55",
		"KS B 1010,  F10T,	M20,  39000.00,  65,  130,  32,  28,  3, 0.0762, 	0.00247, 	0.0970, 	0.0320, 	35",
		"KS B 1010,  F10T,	M22,  48000.00,  75,  150,  37,  32,  3, 0.1060, 	0.00298, 	0.1370, 	0.0520, 	40",
		"KS B 1010,  F10T,	M24,  56000.00,  85,  170,  42,  37,  3, 0.1450, 	0.00355, 	0.2010, 	0.0620, 	45",
		"KS B 1010,  F10T,	M27,  73000.00,  95,  190,  48,  43,  4, 0.2142, 	0.00448, 	0.2630, 	0.0930, 	50",
		"KS B 1010,  F10T,	M30,  89000.00, 105,  210,  55,  50,  4, 0.2606, 	0.00542, 	0.3400, 	0.1340, 	55",
		"KS B 1010,  S10T,	M20,  39000.00,  65,  130,  32,  28,  3, 0.0762, 	0.00247, 	0.0970, 	0.0320, 	30",
		"KS B 1010,  S10T,	M22,  48000.00,  75,  150,  37,  32,  3, 0.1060, 	0.00298, 	0.1370, 	0.0520, 	35",
		"KS B 1010,  S10T,	M24,  56000.00,  85,  170,  42,  37,  3, 0.1450, 	0.00355, 	0.2010, 	0.0620, 	40",
		"KS B 1010,  S10T,	M27,  73000.00,  95,  190,  48,  43,  4, 0.2142, 	0.00448, 	0.2630, 	0.0930, 	45",
		"KS B 1010,  S10T,	M30,  89000.00, 105,  210,  55,  50,  4, 0.2606, 	0.00542, 	0.3400, 	0.1340, 	50",
		"KS B 1010,  F13T,	M20,  50000.00,  65,  130,  32,  28,  3, 0.0762, 	0.00247, 	0.0970, 	0.0320, 	35",
		"KS B 1010,  F13T,	M22,  63000.00,  75,  150,  37,  32,  3, 0.1060, 	0.00298, 	0.1370, 	0.0520, 	40",
		"KS B 1010,  F13T,	M24,  73000.00,  85,  170,  42,  37,  3, 0.1450, 	0.00355, 	0.2010, 	0.0620, 	45",
		"KS B 1010,  S13T,	M20,  50000.00,  65,  130,  32,  28,  3, 0.0762, 	0.00247, 	0.0970, 	0.0320, 	30",
		"KS B 1010,  S13T,	M22,  63000.00,  75,  150,  37,  32,  3, 0.1060, 	0.00298, 	0.1370, 	0.0520, 	35",
		"KS B 1010,  S13T,	M24,  73000.00,  85,  170,  42,  37,  3, 0.1450, 	0.00355, 	0.2010, 	0.0620, 	40"
		};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;
	for(long n = 0; n < nCount; n++)
	{
		if( DivideStr(SArr, szBaseStr[n],',') ==FALSE) continue;

		// File에서 입력한 데이터가 적은 경우 처리(ASSERT 등 방지)
		CSteelBoltData*pData = new CSteelBoltData;
		long nIdx = 0;
		pData->m_strStandard				= SArr[nIdx++];
		pData->m_strGrade					= SArr[nIdx++];
		pData->m_strSize					= SArr[nIdx++];
		pData->m_ForceAllow					= atof(SArr[nIdx++]);
		pData->m_CenterSpaceMin				= atof(SArr[nIdx++]);
		pData->m_CenterSpaceMax				= atof(SArr[nIdx++]);
		pData->m_ShearFaceDistanceMin		= atof(SArr[nIdx++]);
		pData->m_StressFaceDistanceMin		= atof(SArr[nIdx++]);
		pData->m_HoleMargin					= atof(SArr[nIdx++]);
		pData->m_BaseWeight					= atof(SArr[nIdx++]);
		pData->m_LengthPerWeight			= atof(SArr[nIdx++]);
		pData->m_NutWeight					= atof(SArr[nIdx++]);
		pData->m_WasherWeight				= atof(SArr[nIdx++]);
		pData->m_Length						= atof(SArr[nIdx++]);
		m_BoltArr.Add(pData);
	}
	m_nLengthCalcType = 1;
}

CString CSteelBolt::GetStringData(long idx, BOLT_DATA_ID ID)
{
	CString str = "";
	if(idx >= GetSize())	return str;
	switch(ID) {
	case BDIB_STANDARD:
		str = m_BoltArr[idx]->m_strStandard;								break;
	case BDIB_GRADE:
		str = m_BoltArr[idx]->m_strGrade;									break;
	case BDIB_SIZE:
		str = m_BoltArr[idx]->m_strSize;									break;
	case BDIB_SYMBOL:
		str	= m_BoltArr[idx]->m_strGrade + " " + m_BoltArr[idx]->m_strSize;	break;
	case BDIB_FORCEALLOW:
		str.Format("%g", m_BoltArr[idx]->m_ForceAllow);						break;
	case BDIB_CENTERSPACEMIN:
		str.Format("%g", m_BoltArr[idx]->m_CenterSpaceMin);					break;
	case BDIB_CENTERSPACEMAX:
		str.Format("%g", m_BoltArr[idx]->m_CenterSpaceMax);					break;
	case BDIB_SHEARFACEDISTANCEMIN:
		str.Format("%g", m_BoltArr[idx]->m_ShearFaceDistanceMin);			break;
	case BDIB_STRESSFACEDISTANCEMIN:
		str.Format("%g", m_BoltArr[idx]->m_StressFaceDistanceMin);			break;
	case BDIB_HOLEMARGIN:
		str.Format("%g", m_BoltArr[idx]->m_HoleMargin);						break;
	case BDIB_BASEWEIGHT:
		str.Format("%.4f", m_BoltArr[idx]->m_BaseWeight);					break;
	case BDIB_LENGTHPERWEIGHT:
		str.Format("%.5f", m_BoltArr[idx]->m_LengthPerWeight);				break;
	case BDIB_NUTWEIGHT:
		str.Format("%.4f", m_BoltArr[idx]->m_NutWeight);					break;
	case BDIB_WASHERWEIGHT:	
		str.Format("%.4f", m_BoltArr[idx]->m_WasherWeight);					break;
	case BDIB_LENGTH:
		str.Format("%g", m_BoltArr[idx]->m_Length);							break;
	default:
		str = "Out of Range ID";
	}
	return str;
}

double CSteelBolt::GetBoltLength(CPlateBasicIndex *pBx, long nMaterial, CString strSize, double dThick)
{
	if(m_nLengthCalcType==1)	// 2사 3입
	{
		if(long(dThick)%5 < 3)	dThick -= long(dThick)%5;
		else					dThick -= long(dThick)%5 - 5;
	}
	else						// 5단위 RoundUp
	{
		if(long(dThick)%5 != 0)	dThick -= long(dThick)%5 - 5;
	}	

	if(!pBx) return dThick + 35;

	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CString strMaterial = pDB->GetMaterial(nMaterial, pBx);

	CSteelBoltData *pBoltData = GetBolt(strMaterial, strSize);	
	
	if(!pBoltData) return 0.0;

	return pBoltData->m_Length + dThick;
}

// 볼트 연단거리의 올림 계산
double CSteelBolt::GetCalcEdgeDistance(long nBoltIdx)
{
	if(nBoltIdx<0 || nBoltIdx>=GetSize())	return 0;

	double dBaseDan	= GetShearFaceDistanceMin(nBoltIdx);
	long nDiv		= (long)dBaseDan/5;
	double dExt		= dBaseDan - nDiv*5;

	if(dExt>0)
		++nDiv;

	return max(40, nDiv*5.0);
}

CSteelBoltData* CSteelBolt::GetBolt(CString strMaterial, double dDia)
{
	CString strDia = _T("");
	strDia.Format("M%d", long(dDia));
	CSteelBoltData *pBoltData = GetBolt(strMaterial, strDia);
	if(!pBoltData)
		return	NULL;
	else
		return pBoltData;
}

CSteelBoltData* CSteelBolt::GetBolt(CString strMaterial, CString strSize)
{	
	long nIdx = GetIndex(strMaterial + " " + strSize);
	
	if(nIdx<0)
		nIdx = GetIndex(strMaterial + " M22");

	if(nIdx <  0)
		return NULL;

	return m_BoltArr[nIdx];
}

long CSteelBolt::GetIndex(CString &strSymbol)
{
	long idx, size;

	idx = 0;
	size = GetSize();
	while(idx < size)
	{
		if(strSymbol == GetStringData(idx, BDIB_SYMBOL))
			return idx;
		idx++;
	}
	return -1;
}
