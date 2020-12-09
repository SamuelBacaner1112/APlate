// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WeldRecord.h: interface for the CWeldRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WELDRECORD_H__46D3D499_650A_4954_B42E_66C12E271BC5__INCLUDED_)
#define AFX_WELDRECORD_H__46D3D499_650A_4954_B42E_66C12E271BC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_WELD_COLUMN 40

class AFX_EXT_CLASS CWeldRecord  
{
public:
	CWeldRecord(CWeldRecord* pRec = NULL);
	virtual ~CWeldRecord();
 
protected:
	long m_nKindName;
	long m_nEleName;
	long m_nSubEleName;
	long m_nEleNameConnect; // 접속부재
	long m_nSubEleNameConnect;

public:
	long m_nGirder;
	long m_nSplice;
	long m_nMarkGrandAssem;		// 대조립마크
	long m_nMarkMidAssem;		// 중조립마크
	long m_nMarkAssem;			// 소조립마크
	long m_nMarkSection;		// 단면마크
	long m_nMarkSectionBasic;	// 단면마크
	long m_nMarkBuje;			// 부재마크
	long m_nSectionNumber;
	long m_nSectionNumberConnect;

	CString m_sMaterial;
	CString m_sMarkGrandAssem;
	CString m_sMarkMidAssem;
	CString m_sMarkAssem;
	CString m_sMarkSection;
	CString m_sMarkSectionBasic;
	CString m_sMarkBuje;

	CString m_sRefDWG;
	CString m_sUsage;

	// 제원
	CVector m_vJewon;		// 제원
	long	m_nQty;			// 수량
	double  m_dLengthWeld;	// 용접장
	double  m_dThickWeld;	// 용접목두께

	// WELD SET
	CString m_sWeldType;
	CString m_sImproveType;
	CString m_sLayType;
	long	m_nWeldType;
	long	m_nImproveType;
	long	m_nLayType;

	// 접속부재
	long	m_nMarkBujeConnect;
	double  m_dThickConnect;

	int CompareSang(const CWeldRecord* pRec) const;
	int CompareAssembly(const CWeldRecord* pRec) const;
	int CompareAssemblySang(const CWeldRecord* pRec) const;
	int CompareAssemblyTotal(const CWeldRecord* pRec) const;
	int CompareWeldType(const CWeldRecord* pRec) const;
	int CompareWeldThick(const CWeldRecord* pRec) const;
	BOOL IsSameRecord(const CWeldRecord *pRec,DWORD exceptionFlag = 0) const;

	// 
	CString GetStrGirder(long nStart=0) const;
	CString GetStrSplice(long nStart=0) const;
	CString GetStrSectionType() const;
	CString GetStrEleName() const;
	CString GetStrSubEleName() const;
	CString GetStrWidth(long nRound=0) const;
	CString GetStrThick(long nRound=0) const;
	CString GetStrLength(long nRound=0) const;
	void SetStrSectionType(const CString& sName);
	void SetStrEleName(const CString& sEleName,long nSecTypeNumber);
	void SetStrEleName(const CString& sEleName);
	void SetStrSubEleName(const CString& sName);
	void SetStrEleConnectName(const CString& sEleName);
	void SetStrSubEleConnectName(const CString& sName);
	CString GetStrEleConnectName() const;
	CString GetStrSubEleConnectName() const;

	long GetDivideEleNameNum(const CString& sName) const;
	CString GetDivideEleNameStr(const CString& sName) const;
};

#endif // !defined(AFX_WELDRECORD_H__46D3D499_650A_4954_B42E_66C12E271BC5__INCLUDED_)
