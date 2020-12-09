// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// 수평브레이싱 대표강재
#if !defined(_AFX_STANDARD_STEEL_)
#define _AFX_STANDARD_STEEL_

class AFX_EXT_CLASS CStandardSteel : public CObject
{
public:
	CStandardSteel();
	virtual ~CStandardSteel();

	CString	m_szName;
	long	m_nSteelIdx;
	long	m_nMaterialIdx;

	virtual void Serialize(CArchive& ar);
};
#endif // !defined(_AFX_STANDARD_STEEL_)

// SteelMaterial.h: interface for the CSteelMaterial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELMATERIAL_H__373C7418_8C77_4036_94CE_0E1AE92ECD98__INCLUDED_)
#define AFX_STEELMATERIAL_H__373C7418_8C77_4036_94CE_0E1AE92ECD98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelMaterialData : public CObject  
{
public:
	CSteelMaterialData();
	virtual ~CSteelMaterialData() {};

public:
	//형강설정  
	//LSTEEL:		등변ㄱ형강 
	//LVALSTEEL:	부등변ㄱ형강 
	//CSTEEL:		ㄷ형강 
	//CTSTEEL:		CT형강 
	//ISTEEL:		I형강 
	//HSTEEL:		H형강 
	enum { LSTEEL, LVALSTEEL, CSTEEL, CTSTEEL, ISTEEL, HSTEEL, STEELCOUNT };
	long m_nType;
	long m_nJewonIdx;
	double m_dH;
	double m_dB;
	double m_dT1;
	double m_dT2;
	double m_dR1;
	double m_dR2;
	double m_dSectionArea;
	double m_dUnitWeight;
	CDPoint m_dCenterOfGravity;	
	CDPoint m_d2MomentOfInertia;	
	CDPoint m_d2MomentOfInertiaMaxMin;	
	CDPoint m_dRadiusOfGyration;
	CDPoint m_dRadiusOfGyrationMaxMin;
	CDPoint m_dModulusOfSection;
	double m_dTanAlpha;

	CSteelMaterialData& operator<<=(const CSteelMaterialData& Obj);
	
public:
	friend CArchive& operator <<( CArchive& ar, CSteelMaterialData& v)
	{	v.Serialize(ar); return ar;	}
	friend CArchive& operator >>( CArchive& ar, CSteelMaterialData& v)
	{	v.Serialize(ar); return ar;	}
	virtual void Serialize(CArchive& ar);
	void SetData(long nType, long nJewonIdx, double *pArray);			
};

class AFX_EXT_CLASS CSteelMaterialGroup : public CObject  
{
public:	
	CSteelMaterialGroup();

public:
	long m_nSteelType[CSteelMaterialData::STEELCOUNT];
	long m_nJewonType[CSteelMaterialData::STEELCOUNT];

public:
	friend CArchive& operator <<( CArchive& ar, CSteelMaterialGroup& v)
	{	v.Serialize(ar); return ar;	}
	friend CArchive& operator >>( CArchive& ar, CSteelMaterialGroup& v)
	{	v.Serialize(ar); return ar;	}
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CSteelMaterial
{
public:
	CSteelMaterial();
	virtual ~CSteelMaterial();

public:
	enum { TYPE_GEN, TYPE_ATYPE, TYPE_PTYPE, TYPE_COUNT };
	enum { BUJE_CROSSBEAM, BUJE_UVBEAM, BUJE_LVBEAM, BUJE_CVBEAM, BUJE_UHBEAM, BUJE_LHBEAM, BUJE_COUNT };
	CSteelMaterialGroup m_nType[BUJE_COUNT]; 

public:
	virtual void Serialize(CArchive& ar);
	CTypedPtrArray<CObArray, CSteelMaterialData*> m_MatDataArray[CSteelMaterialData::STEELCOUNT];
	void SetSteelMaterialData();
	CString GetBujeMainString(long nIdx) const;
	CString GetBujeString(long nIdx) const;	
	CString GetTypeName(long nType) const;
	CString GetJewonDataString(long nType, long nIndex) const;	
	long GetTypeNameIndex(const CString &szType) const;
	long GetJewonIndex(long nType, const CString &szJewon) const;
	long GetSizeType() const;
	long GetSizeJewon(long nType) const;
	//
	long GetSteelType(long nType1, long nType2) const { return m_nType[nType1].m_nSteelType[nType2]; }
	long GetJewonType(long nType1, long nType2) const { return m_nType[nType1].m_nJewonType[nType2]; }
//	long GetSteelType(CPlateBasicIndex *pBx, long nType2);
//	long GetJewonType(CPlateBasicIndex *pBx, long nType2);
	void SetSteelType(long nType1, long nType2, long nVal) { m_nType[nType1].m_nSteelType[nType2] = nVal; }
	void SetJewonType(long nType1, long nType2, long nVal) { m_nType[nType1].m_nJewonType[nType2] = nVal; }
	long GetStandardSteelIdx(CString szName);
	//
//	double GetUWeight(long nType1, long nType2);
//	double GetUWeight(CPlateBasicIndex *pBx, long nType2);

	CSteelMaterialData* GetMatData(long nType1, long nType2) const { return m_MatDataArray[nType1].GetAt(nType2);}
	void SetSteelMaterialDefault(CSteelMaterialData *pSteelMat, long nSteelType);

	CTypedPtrArray <CObArray, CStandardSteel*> m_ptrArrStdSteel;
	void AddStandardSteelMaterial(CStandardSteel *pStandardSteel);
	void StdMaterialRemoveAll();
};

#endif // !defined(AFX_STEELMATERIAL_H__373C7418_8C77_4036_94CE_0E1AE92ECD98__INCLUDED_)
