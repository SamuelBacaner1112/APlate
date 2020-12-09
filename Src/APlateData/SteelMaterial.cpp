// SteelMaterial.cpp: implementation of the CSteelMaterial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelMaterial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSteelMaterialGroup::CSteelMaterialGroup()
{
}

void CSteelMaterialGroup::Serialize(CArchive& ar)
{
	long nFlag = 0, n;
	if(ar.IsStoring())
	{
		ar << nFlag;
		for(n=0; n<CSteelMaterialData::STEELCOUNT; n++)
		{
			ar << m_nSteelType[n];			
			ar << m_nJewonType[n];
		}
	}
	else
	{
		ar >> nFlag;
		for(n=0; n<CSteelMaterialData::STEELCOUNT; n++)
		{
			ar >> m_nSteelType[n];
			ar >> m_nJewonType[n];
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSteelMaterialData::CSteelMaterialData()
{
	m_nType						= 0;
	m_nJewonIdx					= 0;
	m_dH						= 0;
	m_dB						= 0;
	m_dT1						= 0;
	m_dT2						= 0;
	m_dR1						= 0;
	m_dR2						= 0;
	m_dSectionArea				= 0;
	m_dUnitWeight				= 0;
	m_dCenterOfGravity			= CDPoint(0,0);
	m_d2MomentOfInertia			= CDPoint(0,0);
	m_d2MomentOfInertiaMaxMin	= CDPoint(0,0);
	m_dRadiusOfGyration			= CDPoint(0,0);
	m_dRadiusOfGyrationMaxMin	= CDPoint(0,0);
	m_dModulusOfSection			= CDPoint(0,0);
	m_dTanAlpha					= 0;
}

CSteelMaterialData& CSteelMaterialData::operator<<=(const CSteelMaterialData& Obj)
{
	if(this == &Obj ) return *this;

	m_nType		= Obj.m_nType;
	m_nJewonIdx	= Obj.m_nJewonIdx;
	m_dH		= Obj.m_dH;
	m_dB		= Obj.m_dB;
	m_dT1		= Obj.m_dT1;
	m_dT2		= Obj.m_dT2;
	m_dR1						= Obj.m_dR1;
	m_dR2						= Obj.m_dR2;
	m_dSectionArea				= Obj.m_dSectionArea;
	m_dUnitWeight				= Obj.m_dUnitWeight;
	m_dCenterOfGravity			= Obj.m_dCenterOfGravity;	
	m_d2MomentOfInertia			= Obj.m_d2MomentOfInertia;	
	m_d2MomentOfInertiaMaxMin	= Obj.m_d2MomentOfInertiaMaxMin;	
	m_dRadiusOfGyration			= Obj.m_dRadiusOfGyration;
	m_dRadiusOfGyrationMaxMin	= Obj.m_dRadiusOfGyrationMaxMin;
	m_dModulusOfSection			= Obj.m_dModulusOfSection;
	m_dTanAlpha					= Obj.m_dTanAlpha;
	return *this;
}

void CSteelMaterialData::Serialize(CArchive& ar)
{
	long nFlag = 1;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nType;
		ar << m_dH;
		ar << m_dB;
		ar << m_dT1;
		ar << m_dT2;
		ar << m_dR1;
		ar << m_dR2;
		ar << m_dSectionArea;
		ar << m_dUnitWeight;
		ar << m_dCenterOfGravity;	
		ar << m_d2MomentOfInertia;	
		ar << m_d2MomentOfInertiaMaxMin;	
		ar << m_dRadiusOfGyration;
		ar << m_dRadiusOfGyrationMaxMin;
		ar << m_dModulusOfSection;
		ar << m_dTanAlpha;
		ar << m_nJewonIdx;
	}
	else
	{
		ar >> nFlag;	
		ar >> m_nType;
		ar >> m_dH;
		ar >> m_dB;
		ar >> m_dT1;
		ar >> m_dT2;
		ar >> m_dR1;
		ar >> m_dR2;
		ar >> m_dSectionArea;
		ar >> m_dUnitWeight;
		ar >> m_dCenterOfGravity;	
		ar >> m_d2MomentOfInertia;	
		ar >> m_d2MomentOfInertiaMaxMin;	
		ar >> m_dRadiusOfGyration;
		ar >> m_dRadiusOfGyrationMaxMin;
		ar >> m_dModulusOfSection;
		ar >> m_dTanAlpha;
		if(nFlag>0)
			ar >> m_nJewonIdx;
	}
}

void CSteelMaterialData::SetData(long nType, long nJewonIdx, double *pArray)
{	
	m_nType		= nType;
	m_nJewonIdx	= nJewonIdx;
	switch(m_nType)
	{
	case HSTEEL:
		m_dH = pArray[0];
		m_dB = pArray[1];
		m_dT1 = pArray[2];
		m_dT2 = pArray[3];
		m_dR1 = pArray[4];		
		m_dSectionArea = pArray[5];
		m_dUnitWeight = pArray[6];
		m_d2MomentOfInertia.x = pArray[7];	
		m_d2MomentOfInertia.y = pArray[8];	
		m_dRadiusOfGyration.x = pArray[9];
		m_dRadiusOfGyration.y = pArray[10];		
		m_dModulusOfSection.x = pArray[11];
		m_dModulusOfSection.y = pArray[12];
		break;
	case ISTEEL:
		m_dH = pArray[0];
		m_dB = pArray[1];
		m_dT1 = pArray[2];
		m_dT2 = pArray[3];
		m_dR1 = pArray[4];
		m_dR2 = pArray[5];
		m_dSectionArea = pArray[6];
		m_dUnitWeight = pArray[7];
		m_dCenterOfGravity.x = pArray[8];	
		m_dCenterOfGravity.y = pArray[9];	
		m_d2MomentOfInertia.x = pArray[10];	
		m_d2MomentOfInertia.y = pArray[11];			
		m_dRadiusOfGyration.x = pArray[12];
		m_dRadiusOfGyration.y = pArray[13];		
		m_dModulusOfSection.x = pArray[14];
		m_dModulusOfSection.y = pArray[15];
		break;
	case CSTEEL:
		m_dH = pArray[0];
		m_dB = pArray[1];
		m_dT1 = pArray[2];
		m_dT2 = pArray[3];
		m_dR1 = pArray[4];
		m_dR2 = pArray[5];
		m_dSectionArea = pArray[6];
		m_dUnitWeight = pArray[7];
		m_dCenterOfGravity.x = pArray[8];	
		m_dCenterOfGravity.y = pArray[9];
		m_d2MomentOfInertia.x = pArray[10];	
		m_d2MomentOfInertia.y = pArray[11];			
		m_dRadiusOfGyration.x = pArray[12];
		m_dRadiusOfGyration.y = pArray[13];		
		m_dModulusOfSection.x = pArray[14];
		m_dModulusOfSection.y = pArray[15];		
		break;
	case LSTEEL:
		m_dH = pArray[0];
		m_dB = pArray[1];
		m_dT1 = pArray[2];		
		m_dR1 = pArray[3];
		m_dR2 = pArray[4];
		m_dSectionArea = pArray[5];
		m_dUnitWeight = pArray[6];
		m_dCenterOfGravity.x = m_dCenterOfGravity.y = pArray[7];	
		m_d2MomentOfInertia.x = m_d2MomentOfInertia.y = pArray[8];	
		m_d2MomentOfInertiaMaxMin.x = pArray[9];	
		m_d2MomentOfInertiaMaxMin.y = pArray[10];	
		m_dRadiusOfGyration.x = m_dRadiusOfGyration.y = pArray[11];
		m_dRadiusOfGyrationMaxMin.x = pArray[12];
		m_dRadiusOfGyrationMaxMin.y = pArray[13];
		m_dModulusOfSection.x = m_dModulusOfSection.y = pArray[14];		
		break;
	case LVALSTEEL:		
		m_dH = pArray[0];
		m_dB = pArray[1];
		m_dT1 = pArray[2];		
		m_dR1 = pArray[3];
		m_dR2 = pArray[4];
		m_dSectionArea = pArray[5];
		m_dUnitWeight = pArray[6];
		m_dCenterOfGravity.x = pArray[7];
		m_dCenterOfGravity.y = pArray[8];	
		m_d2MomentOfInertia.x = pArray[9];
		m_d2MomentOfInertia.y = pArray[10];	
		m_d2MomentOfInertiaMaxMin.x = pArray[11];	
		m_d2MomentOfInertiaMaxMin.y = pArray[12];	
		m_dRadiusOfGyration.x = pArray[13];
		m_dRadiusOfGyration.y = pArray[14];
		m_dRadiusOfGyrationMaxMin.x = pArray[15];
		m_dRadiusOfGyrationMaxMin.y = pArray[16];
		m_dModulusOfSection.x = pArray[17];
		m_dModulusOfSection.y = pArray[18];
		m_dTanAlpha = pArray[19];	
		break;
	case CTSTEEL:
		m_dH = pArray[0];
		m_dB = pArray[1];
		m_dT1 = pArray[2];		
		m_dT2 = pArray[3];
		m_dR1 = pArray[4];
		m_dSectionArea = pArray[5];
		m_dUnitWeight = pArray[6];
		m_dCenterOfGravity.x = pArray[7];
		m_dCenterOfGravity.y = 0;	
		m_d2MomentOfInertia.x = pArray[8];
		m_d2MomentOfInertia.y = pArray[9];	
		m_dRadiusOfGyration.x = pArray[10];
		m_dRadiusOfGyration.y = pArray[11];
		m_dModulusOfSection.x = pArray[12];
		m_dModulusOfSection.y = pArray[13];		
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelMaterial::CSteelMaterial()
{
//	SetInit();
	SetSteelMaterialData();
}

CSteelMaterial::~CSteelMaterial()
{	
	for(long i=0; i<6; i++)
		AhTPADelete(&m_MatDataArray[i], (CSteelMaterialData *)0);
	StdMaterialRemoveAll();
}

void CSteelMaterial::Serialize(CArchive& ar)
{
	long nFlag = 2;
	long nSize = 0;
	long n = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;			
		for(n=0; n<BUJE_COUNT;n++) ar << m_nType[n];
		nSize = m_ptrArrStdSteel.GetSize();
		ar << nSize;
		for(n = 0 ; n < nSize ; n++)
			AhTPASerial(ar, &m_ptrArrStdSteel, (CStandardSteel*)0);
	}
	else
	{
		ar >> nFlag;		
		for(long ln=0; n<BUJE_COUNT;n++) ar << m_nType[n];
		if(nFlag>0)
		{
			ar >> nSize;
			StdMaterialRemoveAll();

			for(n = 0 ; n < nSize ; n++)
			{
				if(nFlag>1)
					AhTPASerial(ar, &m_ptrArrStdSteel, (CStandardSteel*)0);
			}
		}
	}

	for(n=0; n<CSteelMaterialData::STEELCOUNT; n++)
		AhTPASerial(ar, &m_MatDataArray[n], (CSteelMaterialData*)0);

}

CString CSteelMaterial::GetBujeMainString(long nIdx) const
{
	CHAR *szDataType[] = {
		"일반부", "단지점부", "중간지점부"
	};
	return szDataType[nIdx];
}

CString CSteelMaterial::GetBujeString(long nIdx) const
{
	CHAR *szDataType[] = {
		"가로보", "수직브레이싱 상현재", "수직브레이싱 하현재", "수직브레이싱 사재",
		"상부 수평브레이싱", "수평브레이싱"
	};
	return szDataType[nIdx];
}

CString CSteelMaterial::GetTypeName(long nType) const
{
	CHAR *szDataType[] = {
		"등변 ㄱ 형강", "부등변 ㄱ 형강", "ㄷ 형강", "CT 형강", "I 형강", "H 형강"
	};
	return szDataType[nType];
};

long CSteelMaterial::GetTypeNameIndex(const CString &szType) const
{
	long nIdx =-1;
	CString szData= _T("");

	long nSize = GetSizeType();
	for(long n=0; n<nSize;n++)
	{		
		if(GetTypeName(n)==szType)
		{
			nIdx = n;
			break;
		}
	}
	return nIdx;
}

CString CSteelMaterial::GetJewonDataString(long nType, long nIndex) const
{
	CString szRtn= _T("");
	if(m_MatDataArray->GetSize() <= nType) return szRtn;
	if(m_MatDataArray[nType].GetSize() <= nIndex) return szRtn;

	double dH = m_MatDataArray[nType].GetAt(nIndex)->m_dH;
	double dB = m_MatDataArray[nType].GetAt(nIndex)->m_dB;
	double dT1 = m_MatDataArray[nType].GetAt(nIndex)->m_dT1;
	double dT2 = m_MatDataArray[nType].GetAt(nIndex)->m_dT2;
	switch(nType)
	{
	case CSteelMaterialData::HSTEEL:
	case CSteelMaterialData::ISTEEL:
	case CSteelMaterialData::CTSTEEL:
	case CSteelMaterialData::CSTEEL:
		szRtn.Format("%gx%gx%gx%g",dH,dB,dT1,dT2);
		break;
	case CSteelMaterialData::LSTEEL:
	case CSteelMaterialData::LVALSTEEL:		
		szRtn.Format("%gx%gx%g",dH,dB,dT1);
		break;
	}
	return szRtn;
}

long CSteelMaterial::GetJewonIndex(long nType, const CString &szJewon) const
{
	long nIdx =-1;
	CString szData= _T("");

	long nSize = GetSizeJewon(nType);
	for(long n=0; n<nSize;n++)
	{		
		szData = GetJewonDataString(nType,n);
		if(szData==szJewon)
		{
			nIdx = n;
			break;
		}
	}
	return nIdx;
}

long CSteelMaterial::GetSizeType() const
{
	return CSteelMaterialData::STEELCOUNT;
}

long CSteelMaterial::GetSizeJewon(long nType) const
{
	return m_MatDataArray[nType].GetSize();
}

void CSteelMaterial::SetSteelMaterialData()
{
	long n=0;

	//판형교 설계요령(기준으로 정해놓음)	
//	m_nType[TYPE_GEN].m_nSteelType[BUJE_CROSSBEAM] = CSteelMaterialData::HSTEEL;
//	m_nType[TYPE_GEN].m_nSteelType[BUJE_UVBEAM] = CSteelMaterialData::LSTEEL;
//	m_nType[TYPE_GEN].m_nSteelType[BUJE_LVBEAM] = CSteelMaterialData::LSTEEL;
//	m_nType[TYPE_GEN].m_nSteelType[BUJE_CVBEAM] = CSteelMaterialData::LSTEEL;
	m_nType[TYPE_GEN].m_nSteelType[BUJE_UHBEAM] = CSteelMaterialData::CTSTEEL;
	m_nType[TYPE_GEN].m_nSteelType[BUJE_LHBEAM] = CSteelMaterialData::CTSTEEL;
	
//	m_nType[TYPE_ATYPE].m_nSteelType[BUJE_CROSSBEAM] = CSteelMaterialData::HSTEEL;
//	m_nType[TYPE_ATYPE].m_nSteelType[BUJE_UVBEAM] = CSteelMaterialData::LSTEEL;
//	m_nType[TYPE_ATYPE].m_nSteelType[BUJE_LVBEAM] = CSteelMaterialData::LSTEEL;
//	m_nType[TYPE_ATYPE].m_nSteelType[BUJE_CVBEAM] = CSteelMaterialData::LSTEEL;
	m_nType[TYPE_ATYPE].m_nSteelType[BUJE_UHBEAM] = CSteelMaterialData::CTSTEEL;
	m_nType[TYPE_ATYPE].m_nSteelType[BUJE_LHBEAM] = CSteelMaterialData::CTSTEEL;

//	m_nType[TYPE_PTYPE].m_nSteelType[BUJE_CROSSBEAM] = CSteelMaterialData::HSTEEL;
//	m_nType[TYPE_PTYPE].m_nSteelType[BUJE_UVBEAM] = CSteelMaterialData::LSTEEL;
//	m_nType[TYPE_PTYPE].m_nSteelType[BUJE_LVBEAM] = CSteelMaterialData::LSTEEL;
//	m_nType[TYPE_PTYPE].m_nSteelType[BUJE_CVBEAM] = CSteelMaterialData::LSTEEL;
	m_nType[TYPE_PTYPE].m_nSteelType[BUJE_UHBEAM] = CSteelMaterialData::CTSTEEL;
	m_nType[TYPE_PTYPE].m_nSteelType[BUJE_LHBEAM] = CSteelMaterialData::CTSTEEL;

	//제원
//	m_nType[TYPE_GEN].m_nJewonType[BUJE_CROSSBEAM] = 73;
//	m_nType[TYPE_GEN].m_nJewonType[BUJE_UVBEAM] = 32;
//	m_nType[TYPE_GEN].m_nJewonType[BUJE_LVBEAM] = 32;
//	m_nType[TYPE_GEN].m_nJewonType[BUJE_CVBEAM] = 32;
	m_nType[TYPE_GEN].m_nJewonType[BUJE_UHBEAM] = 18;
	m_nType[TYPE_GEN].m_nJewonType[BUJE_LHBEAM] = 18;
	
//	m_nType[TYPE_ATYPE].m_nJewonType[BUJE_CROSSBEAM] = 73;
//	m_nType[TYPE_ATYPE].m_nJewonType[BUJE_UVBEAM] = 32;
//	m_nType[TYPE_ATYPE].m_nJewonType[BUJE_LVBEAM] = 32;
//	m_nType[TYPE_ATYPE].m_nJewonType[BUJE_CVBEAM] = 32;
	m_nType[TYPE_ATYPE].m_nJewonType[BUJE_UHBEAM] = 18;
	m_nType[TYPE_ATYPE].m_nJewonType[BUJE_LHBEAM] = 18;

//	m_nType[TYPE_PTYPE].m_nJewonType[BUJE_CROSSBEAM] = 73;
//	m_nType[TYPE_PTYPE].m_nJewonType[BUJE_UVBEAM] = 32;
//	m_nType[TYPE_PTYPE].m_nJewonType[BUJE_LVBEAM] = 32;
//	m_nType[TYPE_PTYPE].m_nJewonType[BUJE_CVBEAM] = 32;
	m_nType[TYPE_PTYPE].m_nJewonType[BUJE_UHBEAM] = 18;
	m_nType[TYPE_PTYPE].m_nJewonType[BUJE_LHBEAM] = 18;	

	double nColumnCount[] = { 15, 20, 16, 14, 16, 13};
	for(long i=0; i<6; i++)
		AhTPADelete(&m_MatDataArray[i], (CSteelMaterialData *)0);

	double dArrDataLSTEEL[] = { 
		25,25	,3	,4		,2		,1.427	,1.12	,0.719	,0.797	,1.26	,0.332	,0.747	,0.94	,0.483	,0.448,
		30,30	,3	,4		,2		,1.727	,1.36	,0.844	,1.42	,2.26	,0.59	,0.908	,1.14	,0.585	,0.661,
		40,40	,5	,4.5	,3		,3.755	,2.95	,1.17	,5.42	,8.59	,2.25	,1.2	,1.51	,0.774	,1.91,
		45,45	,4	,6.5	,3		,3.492	,2.74	,1.24	,6.5	,10.3	,2.69	,1.36	,1.72	,0.88	,2,
		45,45	,5	,6.5	,3		,4.302	,3.38	,1.28	,7.91	,12.5	,3.29	,1.36	,1.71	,0.874	,2.46,
		50,50	,4	,6.5	,3		,3.892	,3.06	,1.37	,9.06	,14.4	,3.76	,1.53	,1.92	,0.983	,2.49,
		50,50	,5	,6.5	,3		,4.082	,3.77	,1.41	,11.1	,17.5	,4.58	,1.52	,1.91	,0.976	,3.08,
		50,50	,6	,6.5	,4.5	,5.644	,4.43	,1.44	,12.6	,20		,5.23	,1.5	,1.88	,0.963	,3.55,
		60,60	,4	,6.5	,3		,4.692	,3.68	,1.61	,16		,25.4	,6.62	,1.85	,2.33	,1.19	,3.66,
		60,60	,5	,6.5	,3		,5.802	,4.55	,1.66	,19.6	,31.2	,8.09	,1.84	,2.32	,1.18	,4.52,
		60,60	,6	,6.5	,4.5	,6.844	,5.37	,1.69	,22.6	,35.9	,9.3	,1.82	,2.29	,1.17	,5.24,
		65,65	,5	,8.5	,3		,6.367	,5		,1.77	,25.3	,40.1	,10.5	,1.99	,2.51	,1.28	,5.36,
		65,65	,6	,8.5	,4		,7.527	,5.91	,1.81	,29.4	,46.6	,12.2	,1.98	,2.49	,1.27	,6.26,
		65,65	,8	,8.5	,6		,9.761	,7.66	,1.88	,36.8	,58.3	,15.3	,1.94	,2.44	,1.25	,7.96,
		70,70	,6	,8.5	,4		,8.127	,6.38	,1.93	,37.1	,58.9	,15.3	,2.14	,2.69	,1.37	,7.33,
		75,75	,6	,8.5	,4		,8.727	,6.85	,2.06	,46.1	,73.2	,19		,2.3	,2.9	,1.48	,8.47,
		75,75	,9	,8.5	,6		,12.69	,9.96	,2.17	,64.4	,102	,26.7	,2.25	,2.84	,1.45	,12.1,
		75,75	,12	,8.5	,6		,16.56	,13		,2.29	,81.9	,129	,34.5	,2.22	,2.79	,1.44	,15.7,
		80,80	,6	,8.5	,4		,9.327	,7.32	,2.18	,56.4	,89.6	,23.2	,2.46	,3.1	,1.58	,9.7,
		80,80	,7	,8.5	,4		,10.797	,8.48	,2.23	,64.2	,102.4	,26.8	,2.44	,3.08	,1.58	,11.12,
		90,90	,6	,10		,5		,10.55	,8.28	,2.42	,80.7	,128	,33.4	,2.77	,3.48	,1.78	,12.3,
		90,90	,7	,10		,5		,12.22	,9.59	,2.46	,93		,148	,38.3	,2.76	,3.48	,1.77	,14.2,
		90,90	,8	,10		,7		,13.764	,10.8	,2.5	,104	,165	,42.8	,2.74	,3.46	,1.76	,16,
		90,90	,9	,10		,7		,15.394	,12.08	,2.54	,114	,182	,47.3	,2.72	,3.44	,1.75	,17.65,
		90,90	,10	,10		,7		,17		,13.3	,2.57	,125	,199	,51.7	,2.71	,3.42	,1.74	,19.5,
		90,90	,13	,10		,7		,21.71	,17		,2.69	,156	,248	,65.3	,2.68	,3.38	,1.73	,24.8,
		100,100	,7	,10		,5		,13.62	,10.7	,2.71	,129	,205	,53.2	,3.08	,3.88	,1.98	,17.7,
		100,100	,8	,10		,8		,15.36	,12.06	,2.75	,144	,229	,59.4	,3.06	,3.86	,1.97	,19.86,
		100,100	,10	,10		,7		,19		,14.9	,2.82	,175	,278	,72		,3.04	,3.83	,1.95	,24.4,
		100,100	,13	,10		,7		,24.31	,19.1	,2.94	,220	,348	,91.1	,3		,3.78	,1.94	,31.1,
		120,120	,8	,12		,5		,18.76	,14.7	,3.24	,258	,410	,106	,3.71	,4.67	,2.38	,29.5,
		130,130	,9	,12		,6		,22.74	,17.9	,3.53	,366	,583	,150	,4.01	,5.06	,2.57	,38.7,
		130,130	,10	,12		,6		,25.16	,19.75	,3.57	,403	,641	,165	,4		,5.05	,2.56	,42.8,
		130,130	,12	,12		,8.5	,29.76	,23.4	,3.64	,467	,743	,192	,3.96	,5		,2.54	,49.9,
		130,130	,15	,12		,8.5	,36.75	,28.8	,3.76	,568	,902	,234	,3.93	,4.95	,2.53	,61.5,
		150,150	,10	,14		,7		,29.21	,22.9	,4.06	,627	,997	,258	,4.63	,5.84	,2.97	,57.3,
		150,150	,12	,14		,7		,34.77	,27.3	,4.14	,740	,1.18	,304	,4.61	,5.82	,2.96	,68,
		150,150	,15	,14		,10		,42.74	,33.6	,4.24	,888	,1.14	,365	,4.56	,5.75	,2.92	,82.6,
		150,150	,19	,14		,10		,53.38	,41.9	,4.4	,1.09	,1.73	,451	,4.52	,5.69	,2.91	,103,
		175,175	,12	,15		,11		,40.52	,31.8	,4.73	,1.17	,1.86	,480	,5.38	,6.78	,3.44	,91.8,
		175,175	,15	,15		,11		,50.21	,39.4	,4.85	,1.44	,2.29	,589	,5.35	,6.75	,3.42	,114,
		200,200	,15	,17		,12		,57.75	,45.3	,5.46	,2.18	,3.47	,891	,6.14	,7.75	,3.93	,150,
		200,200	,20	,17		,12		,76		,59.7	,5.67	,2.82	,4.49	,1.16	,6.09	,7.68	,3.9	,197,
		200,200	,25	,17		,12		,93.75	,73.6	,5.86	,3.42	,5.42	,1.41	,6.04	,7.61	,3.88	,242,
		250,250	,25	,24		,12		,119.4	,93.7	,7.1	,6.95	,11.1	,2.86	,7.63	,9.62	,4.9	,388,
		250,250	,35	,24		,18		,162.6	,128	,7.45	,9.11	,14.4	,3.79	,7.49	,9.42	,4.83	,519,
	};

	double dArrDataVarLSTEEL[] = { 
		100,75	,7	,10	,5		,11.87	,9.32	,3.06	,1.83	,118	,56.9	,144	,30.8	,3.15	,2.19	,3.49	,1.61	,0.548	,17		,10,
		100,75	,10	,10	,7		,16.5	,13		,3.17	,1.94	,159	,76.1	,194	,41.3	,3.11	,2.15	,3.43	,1.58	,0.543	,23.3	,13.7,
		125,75	,7	,10	,5		,13.62	,10.7	,4.1	,1.64	,219	,60.4	,243	,36.4	,4.01	,2.11	,4.23	,1.64	,0.362	,26.1	,10.3,
		125,75	,10	,10	,7		,19		,14.9	,4.22	,1.75	,299	,80.9	,330	,49		,3.96	,2.06	,4.17	,1.61	,0.357	,36.1	,14.1,
		125,75	,13	,10	,7		,24.31	,19.1	,4.35	,1.87	,376	,101	,415	,61.9	,3.93	,2.04	,4.13	,1.6	,0.352	,46.1	,17.9,
		150,90	,9	,12	,6		,20.94	,16.4	,4.95	,1.99	,485	,133	,537	,80.4	,4.81	,2.52	,5.06	,1.96	,0.361	,48.2	,19,
		150,90	,12	,12	,8.5	,27.36	,215	,5.07	,2.01	,619	,167	,685	,102	,4.76	,2.47	,5		,1.93	,0.357	,62.3	,24.3,
	};	

	double dArrDataCSTEEL[] = { 
		75	,40		,5		,7		,8	,4	,8.818	,6.92	,0	,1.28	,75.3	,12.2	,2.92	,1.17	,20.1	,4.47,
		100	,50		,5		,7.5	,8	,4	,11.92	,9.36	,0	,1.54	,188	,26		,3.97	,1.48	,37.6	,7.52,
		125	,65		,6		,8		,8	,4	,17.11	,13.4	,0	,1.9	,424	,61.8	,4.98	,1.9	,67.8	,13.4,
		150	,75		,6.5	,10		,10	,5	,23.71	,18.6	,0	,2.28	,861	,117	,6.03	,2.22	,115	,22.4,
		200	,80		,7.5	,11		,12	,6	,31.33	,24.6	,0	,2.21	,1.95	,168	,7.88	,2.32	,195	,29.1,
		200	,90		,8		,13.5	,14	,7	,38.65	,30.3	,0	,2.74	,2.49	,277	,8.02	,2.68	,249	,44.2,
		250	,90		,9		,13		,14	,7	,44.07	,34.6	,0	,2.4	,4.18	,294	,9.74	,2.58	,334	,44.5,
		300	,90		,9		,13		,14	,7	,48.57	,38.1	,0	,2.22	,6.44	,309	,11.5	,2.52	,429	,45.7,
		380	,100	,10.5	,16		,18	,9	,69.69	,54.5	,0	,2.41	,14.5	,535	,14.5	,2.78	,763	,70.5,
		380	,100	,13		,20		,24	,12	,85.71	,67.3	,0	,2.54	,17.6	,655	,14.3	,2.76	,926	,87.8,
	};

	double dArrDataCTSTEEL[] = { 
		50	,100	,6		,8	,10		,10.95	,8.6	,1		,16.1	,66.9	,1.21	,2.47	,4.03	,13.4,
		62.5,125	,6.5	,9	,10		,15.16	,11.9	,1.19	,35		,147	,1.52	,3.11	,6.91	,23.5,
		75	,75		,5		,7	,8		,8.925	,7.01	,1.79	,42.6	,24.7	,2.18	,1.66	,7.46	,6.59,
		75	,100	,6		,9	,11		,13.42	,10.5	,1.55	,51.7	,75.3	,1.96	,2.37	,8.84	,15.1,
		75	,150	,7		,10	,11		,20.07	,15.8	,1.37	,66.4	,282	,1.82	,3.75	,10.8	,37.6,
		100	,100	,5.5	,8	,11		,13.58	,10.7	,2.29	,114	,67		,2.9	,2.22	,14.8	,13.4,
		97	,150	,6		,9	,13		,19.51	,15.3	,1.79	,125	,254	,2.53	,3.61	,15.8	,33.8,
		100	,200	,8		,12	,13		,31.77	,24.9	,1.73	,184	,801	,2.41	,5.02	,22.3	,80.1,
		100	,204	,12		,12	,13		,35.77	,28.1	,2.09	,256	,851	,2.67	,4.88	,32.4	,83.4,
		104	,202	,10		,16	,13		,41.85	,32.8	,1.91	,251	,1100	,2.45	,5.13	,29.5	,109,
		124	,124	,5		,8	,12		,16.34	,12.8	,2.63	,208	,127	,3.57	,2.79	,21.3	,20.5,
		125	,125	,6		,9	,12		,18.83	,12.8	,2.78	,248	,147	,3.63	,2.79	,25.6	,23.5,
		122	,175	,7		,11	,16		,28.12	,22.1	,2.27	,289	,492	,3.2	,4.18	,29.1	,56.3,
		122	,252	,11		,11	,16		,41.03	,32.2	,2.39	,445	,1470	,3.29	,5.98	,45.3	,117,
		124	,249	,8		,13	,16		,42.35	,33.2	,1.98	,364	,1670	,2.93	,6.29	,34.9	,134,
		125	,250	,9		,14	,16		,46.06	,36.2	,2.08	,412	,1820	,2.99	,6.29	,39.5	,146,
		125	,255	,14		,14	,16		,52.34	,41.1	,2.58	,589	,1940	,3.36	,6.09	,59.4	,152,
		149	,149	,5.5	,8	,13		,20.4	,16		,3.26	,393	,221	,4.39	,3.29	,33.8	,29.7,
		150	,150	,6.5	,9	,13		,23.39	,18.4	,3.41	,464	,254	,4.45	,3.29	,40		,33.8,
		147	,200	,8		,12	,18		,36.19	,28.4	,2.83	,572	,802	,3.97	,4.71	,48.2	,80.2,
		149	,201	,9		,14	,18		,41.68	,32.7	,2.91	,662	,949	,3.99	,4.71	,55.2	,94.4,
		147	,302	,12		,12	,18		,53.83	,42.3	,2.84	,858	,2760	,3.99	,7.16	,72.3	,183,
		149	,299	,9		,14	,18		,55.4	,43.5	,2.36	,715	,3120	,3.59	,7.51	,57		,209,
		150	,300	,10		,15	,18		,59.89	,47		,2.47	,798	,3380	,3.65	,7.51	,63.7	,225,
		150	,305	,15		,15	,18		,67.39	,52.9	,3.03	,1110	,3550	,4.05	,7.26	,92.5	,233,
		152	,301	,11		,17	,18		,67.41	,52.9	,2.55	,903	,3870	,3.66	,7.57	,71.4	,257,
		173	,174	,6		,9	,14		,26.34	,20.7	,3.71	,679	,396	,5.08	,3.88	,50		,45.5,
		175	,175	,7		,11	,14		,31.57	,24.8	,3.75	,815	,492	,5.08	,3.95	,59.3	,56.2,
		168	,249	,8		,12	,20		,44.08	,34.6	,3.02	,881	,1540	,4.47	,5.92	,64		,124,
		170	,250	,9		,14	,20		,50.76	,39.8	,3.09	,1020	,1830	,4.48	,6		,73.1	,146,
		172	,348	,10		,16	,20		,73		,57.3	,2.67	,1230	,5620	,4.11	,8.78	,84.7	,323,
		172	,354	,16		,16	,20		,83.32	,65.4	,3.4	,1800	,5920	,4.65	,8.43	,131	,335,
		175	,350	,12		,19	,20		,86.94	,68.2	,2.86	,1520	,6790	,4.18	,8.84	,104	,388,
		175	,357	,19		,19	,20		,99.19	,77.9	,3.59	,2200	,7220	,4.71	,8.53	,158	,404,
		169	,351	,13		,13	,20		,67.63	,53.1	,3.21	,1420	,4690	,4.59	,8.33	,104	,267,
		198	,199	,7		,11	,16		,36.08	,28.3	,4.17	,1190	,723	,5.76	,4.48	,76.4	,72.7,
		200	,200	,8		,13	,16		,42.06	,33		,4.23	,1400	,868	,5.76	,4.54	,88.6	,86.8,
		193	,299	,9		,14	,22		,60.05	,47.1	,3.33	,1530	,3120	,5.04	,7.21	,95.5	,209,
		195	,300	,10		,16	,22		,67.98	,53.4	,3.41	,1730	,3600	,5.05	,7.28	,108	,240,
		194	,402	,15		,15	,22		,89.23	,70		,3.7	,2480	,8130	,5.27	,9.54	,158	,404,
		197	,398	,11		,18	,22		,93.41	,73.3	,3.01	,2050	,9460	,4.68	,10.1	,123	,475,
		197	,405	,18		,18	,22		,107.2	,84.1	,3.89	,3050	,9980	,5.34	,9.65	,193	,493,
		200	,400	,13		,21	,22		,109.3	,85.8	,3.21	,2480	,11200	,4.76	,10.1	,147	,560,
		200	,408	,21		,21	,22		,125.3	,98.4	,4.07	,3650	,11900	,5.4	,9.75	,229	,584,
		203	,403	,16		,24	,22		,127.4	,100	,3.51	,3090	,13100	,4.92	,10		,184	,650,
		207	,405	,18		,28	,22		,147.7	,116	,3.68	,3360	,15500	,4.95	,10.2	,213	,766,
		214	,407	,20		,35	,22		,180.3	,142	,3.9	,4380	,19700	,4.93	,10.4	,250	,967,
		229	,417	,30		,50	,22		,264.3	,207	,4.85	,7470	,30200	,5.32	,10.7	,414	,1450,
		249	,432	,45		,70	,22		,385	,302	,6.13	,13200	,47100	,5.87	,11.1	,706	,2180,
		223	,199	,8		,12	,18		,42.15	,33.1	,5.1	,1880	,790	,6.67	,4.33	,109	,79.4,
		225	,200	,9		,14	,18		,43.38	,38		,5.15	,2160	,936	,6.68	,4.4	,124	,93.6,
		217	,299	,10		,15	,24		,67.52	,53		,4.04	,2350	,3350	,5.89	,7.04	,133	,224,
		220	,300	,11		,18	,24		,78.69	,61.8	,4.05	,2680	,4060	,5.84	,7.68	,149	,270,
		248	,199	,9		,14	,20		,50.64	,39.7	,5.9	,2840	,922	,7.49	,4.27	,150	,92.6,
		250	,200	,10		,16	,20		,57.12	,44.8	,5.96	,3210	,1070	,7.5	,4.33	,169	,107,
		253	,201	,11		,19	,20		,65.65	,51.5	,5.95	,3670	,1290	,7.48	,4.33	,190	,128,
		241	,300	,11		,15	,26		,72.76	,57.1	,4.92	,3420	,3380	,6.85	,6.82	,178	,225,
		244	,300	,11		,18	,26		,81.76	,64.2	,4.66	,3620	,4060	,6.66	,7.07	,184	,270,
		298	,199	,10		,15	,22		,60.23	,47.3	,7.79	,5190	,989	,9.29	,4.05	,236	,99.4,
		300	,200	,11		,17	,22		,67.21	,52.8	,7.84	,5810	,1140	,9.3	,4.12	,262	,114,
		303	,201	,12		,20	,22		,76.24	,59.8	,7.79	,6570	,1360	,9.28	,4.22	,292	,135,
		306	,202	,13		,23	,22		,85.33	,67		,7.79	,7340	,1590	,9.27	,4.31	,322	,157,
		291	,300	,12		,17	,28		,87.24	,68		,6.39	,6360	,3830	,8.54	,6.63	,280	,256,
		294	,300	,12		,20	,28		,96.24	,75.6	,6.08	,6170	,4510	,8.35	,6.85	,288	,301,
		297	,302	,14		,23	,28		,111.2	,87.3	,6.33	,7910	,5290	,8.44	,6.9	,339	,350,
		346	,300	,13		,20	,28		,105.7	,83		,7.99	,11300	,4510	,10.3	,6.53	,425	,301,
		350	,300	,13		,24	,28		,117.7	,92.4	,7.55	,12000	,5410	,10.1	,6.78	,438	,361,
		354	,302	,15		,28	,28		,136.8	,107	,7.78	,14200	,6440	,10.2	,6.86	,513	,426,
		396	,300	,14		,22	,28		,121.7	,95.6	,9.66	,17700	,4960	,12.1	,6.38	,593	,331,
		400	,300	,14		,26	,28		,133.7	,105	,9.18	,18800	,6860	,11.9	,6.62	,610	,391,
		404	,302	,16		,30	,28		,153.8	,121	,9.41	,21900	,6900	,11.9	,6.7	,705	,457,
	};	

	double dArrDataISTEEL[] = { 
		125	,75		,5.5	,9.5	,9		,4.5	,20.45	,16.1	,0	,0	,538	,57.5	,5.13	,1.68	,86		,15.3,
		150	,75		,5.5	,9.5	,9		,4.5	,21.83	,17.1	,0	,0	,819	,57.5	,6.12	,1.62	,109	,15.3,
		200	,100	,7		,10		,10		,5		,33.06	,26		,0	,0	,2170	,138	,8.11	,2.05	,217	,27.7,
		250	,125	,7.5	,12.5	,12		,6		,48.79	,38.3	,0	,0	,5180	,337	,10.3	,2.63	,414	,53.9,
		300	,150	,10		,18.5	,19		,9.5	,83.47	,65.5	,0	,0	,12700	,886	,12.3	,3.26	,849	,118,
		300	,150	,11.5	,22		,23		,11.5	,97.88	,76.8	,0	,0	,14700	,1080	,12.2	,3.32	,978	,149,
		350	,150	,9		,15		,13		,6.5	,74.58	,58.5	,0	,0	,15200	,702	,14.3	,3.17	,870	,93.5,
		350	,150	,12		,24		,25		,12.5	,111.1	,87.2	,0	,0	,22400	,1180	,14.2	,3.26	,1280	,158,
		400	,150	,10		,18		,17		,8.5	,91.73	,72		,0	,0	,24100	,864	,16.2	,3.07	,1200	,115,
		400	,150	,12.5	,25		,27		,13.5	,122.1	,95.8	,0	,0	,31700	,1240	,16.1	,3.18	,1580	,165,
		450	,175	,11		,20		,19		,9.5	,116.8	,91.7	,0	,0	,39200	,1510	,18.3	,3.6	,1740	,173,
		400	,175	,13		,26		,27		,13.5	,146.1	,115	,0	,0	,48800	,2020	,18.3	,3.72	,2170	,231,
		600	,190	,13		,25		,25		,12.5	,169.4	,133	,0	,0	,98400	,2460	,24.1	,3.81	,3280	,259,
		600	,190	,16		,35		,38		,19		,224.5	,176	,0	,0	,130000	,3540	,24.1	,3.97	,4330	,373,
	};

	double dArrDataHSTEEL[] = { 
		100 ,100	,6		,8	,10		,21.9	,17.2	,383	,134	,4.18	,2.47	,76.5	,26.7,
		125	,125	,6.5	,9	,10		,30.31	,23.8	,847	,293	,5.29	,3.11	,136	,47,
		150	,75		,5		,7	,8		,17.85	,14		,666	,49.5	,6.11	,1.66	,88.8	,13.2,
		148	,100	,6		,9	,11		,26.84	,21.1	,1020	,151	,6.17	,2.37	,138	,30.1,
		150	,150	,7		,10	,11		,40.14	,31.5	,1640	,563	,6.39	,3.75	,219	,75.1,
		198	,99		,4.5	,7	,11		,23.18	,18.2	,1580	,114	,8.26	,2.21	,160	,23,
		200	,100	,5.5	,8	,11		,27.16	,21.3	,1840	,134	,8.24	,2.22	,184	,26.8,
		194	,150	,6		,9	,13		,39.01	,30.6	,2690	,507	,8.3	,3.61	,277	,67.6,
		200	,200	,8		,12	,13		,63.53	,49.9	,4720	,1600	,8.62	,5.02	,472	,160,
		200	,204	,12		,12	,13		,71.53	,56.2	,4980	,1700	,8.35	,4.88	,498	,167,
		208	,202	,10		,16	,13		,83.69	,65.7	,6530	,2200	,8.83	,5.13	,628	,218,
		248	,124	,5		,8	,12		,32.68	,25.7	,3540	,255	,10.4	,2.79	,285	,41.1,
		250	,125	,6		,9	,12		,37.66	,29.6	,4050	,294	,10.4	,2.79	,324	,47,
		244	,175	,7		,11	,16		,56.24	,44.1	,6120	,984	,10.4	,4.18	,502	,113,
		244 ,252	,11		,11	,16		,82.06	,64.4	,8790	,2940	,10.3	,5.98	,720	,233,
		248	,249	,8		,13	,16		,84.7	,66.5	,9930	,3350	,10.8	,6.29	,801	,269,
		250	,250	,9		,14	,16		,92.18	,72.4	,10800	,3650	,10.8	,6.29	,867	,292,
		250	,255	,14		,14	,16		,104.7	,82.2	,11500	,3880	,10.5	,6.09	,919	,304,
		298	,149	,5.5	,8	,13		,40.8	,32		,6320	,442	,12.4	,3.29	,424	,59.3,
		300	,150	,6.5	,9	,13		,46.78	,36.7	,7210	,508	,12.4	,3.29	,481	,67.7,
		294	,200	,8		,12	,18		,72.38	,56.8	,11300	,1600	,12.5	,4.71	,771	,160,
		298	,201	,9		,14	,18		,83.36	,65.4	,13300	,1900	,12.6	,4.77	,893	,189,
		294	,302	,12		,12	,18		,107.7	,84.5	,16900	,5520	,12.5	,7.16	,1150	,365,
		298	,299	,9		,14	,18		,110.8	,87		,18800	,6240	,13		,7.5	,1270	,417,
		300	,300	,10		,15	,18		,119.8	,94		,20400	,6750	,13.1	,7.51	,1360	,450,
		300	,305	,15		,15	,18		,134.8	,106	,21500	,7100	,12.6	,7.26	,1440	,466,
		304	,301	,11		,17	,18		,134.8	,106	,23400	,7730	,13.2	,7.57	,1540	,514,
		310	,305	,15		,20	,18		,165.3	,130	,28150	,9460	,13.2	,7.6	,1810	,620,
		310	,310	,20		,20	,18		,180.8	,142	,29390	,9940	,12.8	,7.5	,1890	,642,
		346	,174	,6		,9	,14		,52.68	,41.4	,11100	,792	,14.5	,3.88	,641	,91,
		350	,175	,7		,11	,14		,63.14	,49.6	,13600	,984	,14.7	,3.95	,775	,112,
		354	,176	,8		,13	,14		,73.68	,57.8	,16100	,1180	,14.8	,4.01	,909	,134,
		336	,249	,8		,12	,20		,88.15	,69.2	,18500	,3090	,14.5	,5.92	,1100	,248,
		340	,250	,9		,14	,20		,101.5	,79.7	,21700	,3650	,14.6	,6		,1280	,292,
		344	,348	,10		,16	,20		,146	,115	,33300	,11200	,15.1	,8.78	,1940	,646,
		344	,354	,16		,16	,20		,166.6	,131	,35300	,11800	,14.6	,8.43	,2050	,669,
		350	,350	,12		,19	,20		,173.9	,137	,40300	,13600	,15.2	,8.84	,2300	,776,
		350	,357	,19		,19	,20		,191.4	,156	,42800	,14400	,14.7	,8.53	,2450	,809,
		396	,199	,7		,11	,16		,72.16	,56.6	,20000	,1450	,16.7	,4.48	,1010	,145,
		400	,200	,8		,13	,16		,84.12	,66		,23700	,1740	,16.8	,4.54	,1190	,174,
		404	,201	,9		,15	,16		,96.16	,75.5	,27500	,2030	,16.9	,4.6	,1360	,202,
		386	,299	,9		,14	,22		,120.1	,94.3	,33700	,6240	,16.7	,7.81	,1740	,418,
		390	,300	,10		,16	,22		,136	,107	,38700	,7210	,16.9	,7.28	,1980	,481,
		388	,402	,15		,15	,22		,178.5	,140	,49000	,16300	,16.6	,9.54	,2520	,809,
		394	,398	,11		,18	,22		,186.8	,147	,56100	,18900	,17.3	,10.1	,2850	,951,
		394	,405	,18		,18	,22		,214.4	,168	,59700	,20000	,16.7	,9.65	,3030	,985,
		400	,400	,13		,21	,22		,218.7	,172	,66600	,22400	,17.5	,10.1	,3330	,1120,
		400	,408	,21		,21	,22		,250.7	,197	,70900	,23800	,16.8	,9.75	,3540	,1170,
		406	,403	,16		,24	,22		,254.9	,200	,78000	,26200	,17.5	,10.1	,3840	,1300,
		414	,405	,18		,28	,22		,295.4	,232	,92800	,31000	,17.7	,10.2	,4480	,1530,
		428	,407	,20		,35	,22		,360.7	,283	,119000	,39400	,18.2	,10.4	,5570	,1930,
		458	,417	,30		,50	,22		,528.6	,415	,187000	,60500	,18.8	,10.7	,8170	,2900,
		498	,432	,45		,70	,22		,770.1	,605	,298000	,94000	,19.7	,11.1	,13000	,4370,
		446	,199	,8		,12	,18		,84.3	,66.2	,28700	,1580	,18.5	,4.33	,1290	,159,
		450	,200	,9		,14	,18		,96.76	,76		,33500	,1870	,18.6	,4.4	,1490	,187,
		434	,299	,10		,15	,24		,135	,106	,46800	,6690	,18.6	,7.04	,2160	,448,
		440	,300	,11		,18	,24		,157.4	,124	,56100	,8110	,18.9	,7.18	,2550	,541,
		496	,199	,9		,14	,20		,101.3	,79.5	,41900	,1840	,20.3	,4.27	,1690	,185,
		500	,200	,10		,16	,20		,114.2	,89.6	,47800	,2140	,20.5	,4.33	,1910	,214,
		506	,201	,11		,19	,20		,131.3	,103	,56500	,2580	,20.7	,4.43	,2230	,254,
		482	,300	,11		,15	,26		,145.5	,114	,60400	,6760	,20.4	,6.82	,2500	,451,
		488	,300	,11		,18	,26		,163.5	,128	,71000	,8110	,20.8	,7.04	,2910	,541,
		596	,199	,10		,15	,22		,120.5	,94.6	,68700	,1980	,23.9	,4.05	,2310	,199,
		600	,200	,11		,17	,22		,134.4	,106	,77600	,2280	,24		,4.12	,2590	,228,
		606	,201	,12		,20	,22		,152.5	,120	,90400	,2720	,24.3	,4.22	,2980	,271,
		612	,202	,13		,23	,22		,170.7	,134	,103000	,3180	,24.6	,4.31	,3380	,314,
		582	,300	,12		,17	,28		,174.5	,137	,103000	,7670	,24.3	,6.63	,3530	,511,
		588	,300	,12		,20	,28		,192.5	,151	,118000	,9020	,24.8	,6.85	,4020	,601,
		594	,302	,14		,23	,28		,222.4	,175	,137000	,10600	,24.9	,6.9	,4620	,701,
		692	,300	,13		,20	,28		,211.5	,166	,172000	,9020	,28.6	,6.53	,4980	,602,
		700	,300	,13		,24	,28		,235.5	,185	,201000	,10800	,29.3	,6.78	,5760	,722,
		708	,302	,15		,28	,28		,273.6	,215	,237000	,12900	,29.4	,6.86	,6700	,853,
		792	,300	,14		,22	,28		,243.4	,191	,254000	,9930	,32.3	,6.39	,6410	,662,
		800	,300	,14		,26	,28		,267.4	,210	,292000	,11700	,33		,6.62	,7290	,782,
		808	,302	,16		,30	,28		,307.6	,241	,339000	,13800	,33.2	,6.7	,8400	,915,
		890	,299	,15		,23	,18		,266.8	,210	,339000	,10300	,35.6	,6.2	,7610	,687,
		900	,300	,16		,28	,18		,305.8	,240	,404000	,12600	,36.4	,6.43	,8990	,842,
		912	,302	,18		,34	,18		,360.1	,283	,491000	,15700	,36.9	,6.56	,10800	,1040,
		918	,303	,19		,37	,18		,387.4	,304	,535000	,17200	,37.2	,6.67	,11700	,1140,
	};


	double nCountL = sizeof(dArrDataLSTEEL) / sizeof(double);	
	long nL = (long)nColumnCount[CSteelMaterialData::LSTEEL];
	for(n=0; n<nCountL/nL; n++)
	{
		CSteelMaterialData *pNewMat = new CSteelMaterialData;
		pNewMat->SetData(CSteelMaterialData::LSTEEL,n,&dArrDataLSTEEL[n*nL]);
		m_MatDataArray[CSteelMaterialData::LSTEEL].Add(pNewMat);
	}

	double nCountVarL = sizeof(dArrDataVarLSTEEL) / sizeof(double);	
	long nLVarC = (long)nColumnCount[CSteelMaterialData::LVALSTEEL];
	for(n=0; n<nCountVarL/nLVarC; n++)
	{
		CSteelMaterialData *pNewMat = new CSteelMaterialData;
		pNewMat->SetData(CSteelMaterialData::LVALSTEEL,n,&dArrDataVarLSTEEL[n*nLVarC]);
		m_MatDataArray[CSteelMaterialData::LVALSTEEL].Add(pNewMat);
	}
	
	double nCountC = sizeof(dArrDataCSTEEL) / sizeof(double);	
	long nLC = (long)nColumnCount[CSteelMaterialData::CSTEEL];
	for(n=0; n<nCountC/nLC; n++)
	{
		CSteelMaterialData *pNewMat = new CSteelMaterialData;
		pNewMat->SetData(CSteelMaterialData::CSTEEL,n,&dArrDataCSTEEL[n*nLC]);
		m_MatDataArray[CSteelMaterialData::CSTEEL].Add(pNewMat);
	}

	double nCountCT = sizeof(dArrDataCTSTEEL) / sizeof(double);	
	long nLCT = (long)nColumnCount[CSteelMaterialData::CTSTEEL];
	for(n=0; n<nCountCT/nLCT; n++)
	{
		CSteelMaterialData *pNewMat = new CSteelMaterialData;
		pNewMat->SetData(CSteelMaterialData::CTSTEEL,n,&dArrDataCTSTEEL[n*nLCT]);
		m_MatDataArray[CSteelMaterialData::CTSTEEL].Add(pNewMat);
	}

	double nCountI = sizeof(dArrDataISTEEL) / sizeof(double);	
	long nLI = (long)nColumnCount[CSteelMaterialData::ISTEEL];
	for(n=0; n<nCountI/nLI; n++)
	{
		CSteelMaterialData *pNewMat = new CSteelMaterialData;
		pNewMat->SetData(CSteelMaterialData::ISTEEL,n,&dArrDataISTEEL[n*nLI]);
		m_MatDataArray[CSteelMaterialData::ISTEEL].Add(pNewMat);
	}

	double nCountH = sizeof(dArrDataHSTEEL) / sizeof(double);	
	long nLH = (long)nColumnCount[CSteelMaterialData::HSTEEL];
	for(n=0; n<nCountH/nLH; n++)
	{
		CSteelMaterialData *pNewMat = new CSteelMaterialData;
		pNewMat->SetData(CSteelMaterialData::HSTEEL,n,&dArrDataHSTEEL[n*nLH]);
		m_MatDataArray[CSteelMaterialData::HSTEEL].Add(pNewMat);
	}	
}
/*
double CSteelMaterial::GetUWeight(long nType1, long nType2)
{
	long nSteelType = GetSteelType(nType1, nType2);
	long nJewonType = GetJewonType(nType1, nType2);

	return toTon(GetMatData(nSteelType, nJewonType)->m_dUnitWeight);
}

double CSteelMaterial::GetUWeight(CPlateBasicIndex *pBx, long nType2)
{
	long nSteelType = GetSteelType(pBx, nType2);
	long nJewonType = GetJewonType(pBx, nType2);

	return toTon(GetMatData(nSteelType, nJewonType)->m_dUnitWeight);
}

long CSteelMaterial::GetSteelType(CPlateBasicIndex *pBx, long nType2)
{
	CPlateBasicIndex *pBxPre  = pBx->GetBxPrev(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING);

	if(pBx->IsAbut() || (pBxNext && pBxNext->IsAbut()))
		return m_nType[TYPE_ATYPE].m_nSteelType[nType2];
	else if(pBx->IsPier() || (pBxNext && pBxNext->IsPier()))
		return m_nType[TYPE_PTYPE].m_nSteelType[nType2];
	else
		return m_nType[TYPE_GEN].m_nSteelType[nType2];

	return -1;
}

long CSteelMaterial::GetJewonType(CPlateBasicIndex *pBx, long nType2)
{
	CPlateBasicIndex *pBxPre  = pBx->GetBxPrev(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING);

	if(pBx->IsAbut() || (pBxNext && pBxNext->IsAbut()))
		return m_nType[TYPE_ATYPE].m_nJewonType[nType2];
	else if(pBx->IsPier() || (pBxNext && pBxNext->IsPier()))
		return m_nType[TYPE_PTYPE].m_nJewonType[nType2];
	else
		return m_nType[TYPE_GEN].m_nJewonType[nType2];

	return -1;

}
*/
void CSteelMaterial::SetSteelMaterialDefault(CSteelMaterialData *pSteelMat, long nSteelType)
{
	double	dArrDataHSTEEL[]	= {800, 300, 14, 26, 28, 267.4, 210, 292000, 11700, 33, 6.62, 7290, 782};
	double	dArrDataISTEEL[]	= {350, 150, 12, 24, 25, 12.5, 111.1, 87.2, 0, 0, 22400, 1180, 14.2, 3.26, 1280, 158};
	double	dArrDataLSTEEL[]	= {130, 130, 10, 12, 6, 25.16, 19.75, 3.5, 403, 641, 165, 4, 5.05, 2.56, 42.8};
	double	dArrDataCSTEEL[]	= {200, 80, 7.5, 11, 12, 6, 31.33, 24.6, 0, 2.21, 1.95, 168, 7.88, 2.32, 195,29.1};
	double	dArrDataCTSTEEL[]	= {50, 100, 6, 8, 10, 10.95, 8.6, 1, 16.1, 66.9, 1.21, 2.47, 4.03, 13.4};
	double	dArrDataVarLSTEEL[]	= {100, 75, 7, 10, 5, 11.87, 9.32, 3.06, 1.83, 118, 56.9, 144, 30.8, 3.15, 2.19, 3.49, 1.61, 0.548, 17, 10};

//	SetSteelMaterialData();

	switch(nSteelType)
	{
	case CSteelMaterialData::HSTEEL :
		pSteelMat->m_nType					= CSteelMaterialData::HSTEEL;
		pSteelMat->m_nJewonIdx				= 73;
		pSteelMat->m_dH						= dArrDataHSTEEL[0];
		pSteelMat->m_dB						= dArrDataHSTEEL[1];
		pSteelMat->m_dT1					= dArrDataHSTEEL[2];
		pSteelMat->m_dT2					= dArrDataHSTEEL[3];
		pSteelMat->m_dR1					= dArrDataHSTEEL[4];
		pSteelMat->m_dSectionArea			= dArrDataHSTEEL[5];
		pSteelMat->m_dUnitWeight			= dArrDataHSTEEL[6];
		pSteelMat->m_d2MomentOfInertia.x	= dArrDataHSTEEL[7];
		pSteelMat->m_d2MomentOfInertia.y	= dArrDataHSTEEL[8];
		pSteelMat->m_dRadiusOfGyration.x	= dArrDataHSTEEL[9];
		pSteelMat->m_dRadiusOfGyration.y	= dArrDataHSTEEL[10];
		pSteelMat->m_dModulusOfSection.x	= dArrDataHSTEEL[11];
		pSteelMat->m_dModulusOfSection.y	= dArrDataHSTEEL[12];
	break;

	case CSteelMaterialData::ISTEEL :
		pSteelMat->m_nType					= CSteelMaterialData::ISTEEL;
		pSteelMat->m_nJewonIdx				= 7;
		pSteelMat->m_dH						= dArrDataISTEEL[0];
		pSteelMat->m_dB						= dArrDataISTEEL[1];
		pSteelMat->m_dT1					= dArrDataISTEEL[2];
		pSteelMat->m_dT2					= dArrDataISTEEL[3];
		pSteelMat->m_dR1					= dArrDataISTEEL[4];
		pSteelMat->m_dR2					= dArrDataISTEEL[5];
		pSteelMat->m_dSectionArea			= dArrDataISTEEL[6];
		pSteelMat->m_dUnitWeight			= dArrDataISTEEL[7];
		pSteelMat->m_dCenterOfGravity.x		= dArrDataISTEEL[8];	
		pSteelMat->m_dCenterOfGravity.y		= dArrDataISTEEL[9];	
		pSteelMat->m_d2MomentOfInertia.x	= dArrDataISTEEL[10];	
		pSteelMat->m_d2MomentOfInertia.y	= dArrDataISTEEL[11];			
		pSteelMat->m_dRadiusOfGyration.x	= dArrDataISTEEL[12];
		pSteelMat->m_dRadiusOfGyration.y	= dArrDataISTEEL[13];		
		pSteelMat->m_dModulusOfSection.x	= dArrDataISTEEL[14];
		pSteelMat->m_dModulusOfSection.y	= dArrDataISTEEL[15];
	break;
	case CSteelMaterialData::CSTEEL:
		pSteelMat->m_nType					= CSteelMaterialData::CSTEEL;
		pSteelMat->m_nJewonIdx				= 4;
		pSteelMat->m_dH						= dArrDataCSTEEL[0];
		pSteelMat->m_dB						= dArrDataCSTEEL[1];
		pSteelMat->m_dT1					= dArrDataCSTEEL[2];
		pSteelMat->m_dT2					= dArrDataCSTEEL[3];
		pSteelMat->m_dR1					= dArrDataCSTEEL[4];
		pSteelMat->m_dR2					= dArrDataCSTEEL[5];
		pSteelMat->m_dSectionArea			= dArrDataCSTEEL[6];
		pSteelMat->m_dUnitWeight			= dArrDataCSTEEL[7];
		pSteelMat->m_dCenterOfGravity.x		= dArrDataCSTEEL[8];	
		pSteelMat->m_dCenterOfGravity.y		= dArrDataCSTEEL[9];
		pSteelMat->m_d2MomentOfInertia.x	= dArrDataCSTEEL[10];	
		pSteelMat->m_d2MomentOfInertia.y	= dArrDataCSTEEL[11];			
		pSteelMat->m_dRadiusOfGyration.x	= dArrDataCSTEEL[12];
		pSteelMat->m_dRadiusOfGyration.y	= dArrDataCSTEEL[13];		
		pSteelMat->m_dModulusOfSection.x	= dArrDataCSTEEL[14];
		pSteelMat->m_dModulusOfSection.y	= dArrDataCSTEEL[15];		
		break;
	case CSteelMaterialData::CTSTEEL:
		pSteelMat->m_nType					= CSteelMaterialData::CTSTEEL;
		pSteelMat->m_nJewonIdx				= 0;
		pSteelMat->m_dH						= dArrDataCTSTEEL[0];
		pSteelMat->m_dB						= dArrDataCTSTEEL[1];
		pSteelMat->m_dT1					= dArrDataCTSTEEL[2];		
		pSteelMat->m_dT2					= dArrDataCTSTEEL[3];
		pSteelMat->m_dR1					= dArrDataCTSTEEL[4];
		pSteelMat->m_dSectionArea			= dArrDataCTSTEEL[5];
		pSteelMat->m_dUnitWeight			= dArrDataCTSTEEL[6];
		pSteelMat->m_dCenterOfGravity.x		= dArrDataCTSTEEL[7];
		pSteelMat->m_dCenterOfGravity.y		= 0;	
		pSteelMat->m_d2MomentOfInertia.x	= dArrDataCTSTEEL[8];
		pSteelMat->m_d2MomentOfInertia.y	= dArrDataCTSTEEL[9];	
		pSteelMat->m_dRadiusOfGyration.x	= dArrDataCTSTEEL[10];
		pSteelMat->m_dRadiusOfGyration.y	= dArrDataCTSTEEL[11];
		pSteelMat->m_dModulusOfSection.x	= dArrDataCTSTEEL[12];
		pSteelMat->m_dModulusOfSection.y	= dArrDataCTSTEEL[13];
		break;
	case CSteelMaterialData::LSTEEL :
		pSteelMat->m_nType						= CSteelMaterialData::LSTEEL;
		pSteelMat->m_nJewonIdx					= 32;
		pSteelMat->m_dH							= dArrDataLSTEEL[0];
		pSteelMat->m_dB							= dArrDataLSTEEL[1];
		pSteelMat->m_dT1						= dArrDataLSTEEL[2];		
		pSteelMat->m_dR1						= dArrDataLSTEEL[3];
		pSteelMat->m_dR2						= dArrDataLSTEEL[4];
		pSteelMat->m_dSectionArea				= dArrDataLSTEEL[5];
		pSteelMat->m_dUnitWeight				= dArrDataLSTEEL[6];
		pSteelMat->m_dCenterOfGravity.x			= dArrDataLSTEEL[7];
		pSteelMat->m_dCenterOfGravity.y			= dArrDataLSTEEL[7];	
		pSteelMat->m_d2MomentOfInertia.x		= dArrDataLSTEEL[8];
		pSteelMat->m_d2MomentOfInertia.y		= dArrDataLSTEEL[8];
		pSteelMat->m_d2MomentOfInertiaMaxMin.x	= dArrDataLSTEEL[9];	
		pSteelMat->m_d2MomentOfInertiaMaxMin.y	= dArrDataLSTEEL[10];	
		pSteelMat->m_dRadiusOfGyration.x		= dArrDataLSTEEL[11];
		pSteelMat->m_dRadiusOfGyration.y		= dArrDataLSTEEL[11];
		pSteelMat->m_dRadiusOfGyrationMaxMin.x	= dArrDataLSTEEL[12];
		pSteelMat->m_dRadiusOfGyrationMaxMin.y	= dArrDataLSTEEL[13];
		pSteelMat->m_dModulusOfSection.x		= dArrDataLSTEEL[14];
		pSteelMat->m_dModulusOfSection.y		= dArrDataLSTEEL[14];
	break;
	case CSteelMaterialData::LVALSTEEL:
		pSteelMat->m_nType						= CSteelMaterialData::LVALSTEEL;
		pSteelMat->m_nJewonIdx					= 0;
		pSteelMat->m_dH							= dArrDataVarLSTEEL[0];
		pSteelMat->m_dB							= dArrDataVarLSTEEL[1];
		pSteelMat->m_dT1						= dArrDataVarLSTEEL[2];		
		pSteelMat->m_dR1						= dArrDataVarLSTEEL[3];
		pSteelMat->m_dR2						= dArrDataVarLSTEEL[4];
		pSteelMat->m_dSectionArea				= dArrDataVarLSTEEL[5];
		pSteelMat->m_dUnitWeight				= dArrDataVarLSTEEL[6];
		pSteelMat->m_dCenterOfGravity.x			= dArrDataVarLSTEEL[7];
		pSteelMat->m_dCenterOfGravity.y			= dArrDataVarLSTEEL[8];	
		pSteelMat->m_d2MomentOfInertia.x		= dArrDataVarLSTEEL[9];
		pSteelMat->m_d2MomentOfInertia.y		= dArrDataVarLSTEEL[10];	
		pSteelMat->m_d2MomentOfInertiaMaxMin.x	= dArrDataVarLSTEEL[11];	
		pSteelMat->m_d2MomentOfInertiaMaxMin.y	= dArrDataVarLSTEEL[12];	
		pSteelMat->m_dRadiusOfGyration.x		= dArrDataVarLSTEEL[13];
		pSteelMat->m_dRadiusOfGyration.y		= dArrDataVarLSTEEL[14];
		pSteelMat->m_dRadiusOfGyrationMaxMin.x	= dArrDataVarLSTEEL[15];
		pSteelMat->m_dRadiusOfGyrationMaxMin.y	= dArrDataVarLSTEEL[16];
		pSteelMat->m_dModulusOfSection.x		= dArrDataVarLSTEEL[17];
		pSteelMat->m_dModulusOfSection.y		= dArrDataVarLSTEEL[18];
		pSteelMat->m_dTanAlpha					= dArrDataVarLSTEEL[19];	
		break;
	}
}

void CSteelMaterial::AddStandardSteelMaterial(CStandardSteel *pStandardSteel)
{
	m_ptrArrStdSteel.Add(pStandardSteel);
}


void CSteelMaterial::StdMaterialRemoveAll()
{
	AhTPADelete(&m_ptrArrStdSteel, (CStandardSteel*)0);
}

long CSteelMaterial::GetStandardSteelIdx(CString szName)
{

	for(long n=0; n<m_ptrArrStdSteel.GetSize(); n++)
	{
		if(m_ptrArrStdSteel.GetAt(n)->m_szName == szName)
			return	n;
	}

	return -1;
}


//////////////////////////////////////////////////////////////////////////
// 대표강재
//////////////////////////////////////////////////////////////////////////
CStandardSteel::CStandardSteel()
{
	m_szName		= _T("");
	m_nSteelIdx		= 0;
	m_nMaterialIdx	= 0;
}

CStandardSteel::~CStandardSteel()
{

}

void CStandardSteel::Serialize(CArchive& ar)
{
	long	nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_szName;
		ar << m_nMaterialIdx;
		ar << m_nSteelIdx;
	}
	else
	{
		ar >> nFlag;
		ar >> m_szName;
		ar >> m_nMaterialIdx;
		ar >> m_nSteelIdx;
	}
}