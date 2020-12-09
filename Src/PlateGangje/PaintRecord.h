// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PaintRecord.h: interface for the CPaintRecord class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_PAINTRECORD_H__E2672109_10EA_468B_B9F2_F269BAA294C1__INCLUDED_)
#define AFX_PAINTRECORD_H__E2672109_10EA_468B_B9F2_F269BAA294C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const long OUTSIDE_FACT			=	0;	// �ܺε��� (����)
const long OUTSIDE_FIELD		=	1;	// �ܺε��� (����)
//const long INSIDE_FACT			=	2;	// ���ε��� (����)
const long SPLICE_FACT			=	2;	// �����ǵ���
const long BOLT_FIELD_OUTSIDE	=	3;	// �ܺκ�Ʈ����
//const long BOLT_FIELD_INSIDE	=	5;	// ���κ�Ʈ����
const long PAVE_FACT			=	4;	// ����鵵��
const long PAINT_ITEM_MAX		=   5;

#define MAX_PAINT_COLUMN	40

class AFX_EXT_CLASS CPaintRecord  
{
protected:
		
	long m_nEleName;		// ���� ��ȣ 
	long m_nSubEleName;		// ���� ��ȣ 
	long m_nProcessBig;
	long m_nProcessMid;

public:
	double GetAreaAllSide();
	CPaintRecord(CPaintRecord *pRec=NULL);
	virtual ~CPaintRecord();

// DATA SET
	long m_nKindName;
	long m_nMarkBuje;			// ���縶ũ
	long m_nMarkGrandAssem;		// ��������ũ
	long m_nMarkMidAssem;		// ��������ũ
	long m_nMarkAssem;			// ��������ũ
	long m_nMarkSection;		// �ܸ鸶ũ
	long m_nMarkSectionBasic;	// �ܸ鸶ũ
	long m_nGirder;				// �Ŵ���ȣ 
	long m_nSplice;				// �������� ��ȣ 
	long m_nSectionNumber;

	CString m_sMarkBuje;
	CString m_sMarkGrandAssem;
	CString m_sMarkMidAssem;
	CString m_sMarkAssem;
	CString m_sMarkSection;
	CString m_sMarkSectionBasic;
	//
	CString m_sMaterial;
	CString m_sRefDWG;
	CString m_sUsage;

	CVector m_vJewon;		// ����
	CVector m_vJewonAngle;		// dB, dT2
	double  m_dPlateArea;	// ����(W,L)
	double	m_dPerimeter;	// �ѷ�(W,L)
	double  m_dPaintArea[PAINT_ITEM_MAX];
	double  m_dUWeight;
	double  m_dTWeight;
	long	m_nQty;			// ����
	long	m_nCountMake;

	/////////////////////////////////////
	int CompareSang(const CPaintRecord* pRec) const;
	int CompareBujeSang(const CPaintRecord* pRec) const;
	int CompareBujeTotal(const CPaintRecord* pRec) const;
	int CompareAssembly(const CPaintRecord* pRec) const;
	int CompareAssemblyTotal(const CPaintRecord* pRec) const;
	int CompareGrandAssembly(const CPaintRecord* pRec) const;
	int CompareGrandAssemblyTotal(const CPaintRecord* pRec) const;
	int CompareMatAndThick(const CPaintRecord* pRec) const;
	int CompareBoltSang(const CPaintRecord* pRec) const;
	int CompareBoltTotal(const CPaintRecord* pRec) const;
	BOOL IsSameRecord(const CPaintRecord* pRec,DWORD exceptionFlag = 0) const;

	/////////////////////////////////////
	CString GetStrGirder(long nStart=0) const;
	CString GetStrSplice(long nStart=0) const;
	CString GetStrEleName() const;
	CString GetStrSubEleName() const;
	CString GetStrSectionType() const;
	CString GetStrProcessBig() const;
	CString GetStrProcessMid() const;

	void SetStrEleName(const CString& sEleName);
	void SetStrEleName(const CString& sName,long nSecTypeNumber);
	void SetStrSubEleName(const CString& sName);
	void SetStrSectionType(const CString& sName);
	void SetStrProcessBig(const CString& sName);
	void SetStrProcessMid(const CString& sName);

	long GetDivideEleNameNum(const CString& sName) const;
	CString GetDivideEleNameStr(const CString& sName) const;
	CString GetStrWidth(long nRound=0) const;
	CString GetStrThick(long nRound=0) const;
	CString GetStrLength(long nRound=0) const;
};
 
#endif // !defined(AFX_PAINTRECORD_H__E2672109_10EA_468B_B9F2_F269BAA294C1__INCLUDED_)
