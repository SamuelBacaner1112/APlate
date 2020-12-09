// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BMRecord.h: interface for the CBMRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMRECORD_H__BC968069_2BE4_4B54_B1C8_34C97869A803__INCLUDED_)
#define AFX_BMRECORD_H__BC968069_2BE4_4B54_B1C8_34C97869A803__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#define MAX_BM_COLUMN		45
#define MAX_COUNT_ELEMAIN	24
#define MAX_COUNT_ELESUB	63
#define MAX_COUNT_KINDNAME	25

class AFX_EXT_CLASS CBMRecord 
{
// Constructor 
public:
	CBMRecord(CBMRecord* pBM = NULL);
	virtual ~CBMRecord();

// Attributes
protected:
	long m_nEleName;			// ���� ��ȣ(�Ϲ�, ���κ�...)
	long m_nSubEleName;			// �����   ��ȣ(����÷���, �Ϻ��÷���...)
	long m_nProcessBig;			// ������ ��ȣ
	long m_nProcessMid;			// ������ ��ȣ

public:
	DWORD	m_dwSummaryFlag;	// ���� RECORD�� �ƴ� �հ� DATA (������,������,�ܸ�Ÿ��)
	CString m_sSummaryMark;		// ���۵��� ���踦 ���� ���и�ũ
	DWORD m_dwFlag;				//
	DWORD m_dwDisableFlag;		//
	long m_nKindName;			// ���纰 ����(MATERIAL_KIND_PLATE=0, MATERIAL_KIND_BOLT=1, MATERIAL_KIND_BOLT2NUT=2 ��
	long m_nMarkBuje;			// ���縶ũ
	long m_nCountMake;			//
	long m_nGirder;				// �Ŵ���ȣ
	long m_nSplice;				// ����������ȣ
	long m_nQty;				// ����
	long m_nFirstClass;			// ���ǥ���� 1���з� (���Ÿ���� ���ǥ���� ���... -> ��������)
	long m_nSectionNumber;		// 
	
	CVector m_vJewon;			// ��������(������ �� dH, dT1, dT2)
	CVector m_vJewonAngle;		// dB, dT2
	CVector m_vJewonBasic;		//
	double	m_dAreaBuje;		// �������
	double  m_dUWeight;			// �����߷�
	double  m_dTWeight;			// ��ü����
	double  m_dTWeightBasic;	//
	double	m_dStationRec;		//	

	CString m_sMaterial;		// ��������
	// NEW ADD
	CString m_sKindName;		// ���纰 ����
	CString m_sRefDWG;			// ���������
	CString m_sUsage;			// ���뵵
	double  m_dFactorArea;		//
	double  m_dFactorAreaBasic; //
	double  m_dThickSecond;		//
	double  m_dWidthSecond;		//
	long m_nSizeType;			// ������ ���� : 1:����,2:����,3:��Ÿ 2006.5.24

// Operation
public:
	CBMRecord& operator=(const CBMRecord &obj);

// Implementation
public:
	CString GetStrGirder(long nStart=0) const;	// �Ŵ��̸�
	CString GetStrSplice(long nStart=0) const;	// ���������̸�
	CString GetStrSectionType() const;			// �ܸ��̸�
	CString GetStrEleName() const;				// �����̸�
	CString GetStrSubEleName() const;			// �����̸�
	CString GetStrProcessBig() const;			//
	CString GetStrProcessMid() const;			//

	CString GetStrQty() const;					// ������Ī
	CString GetStrWidth(long nRound=0) const;	// ��
	CString GetStrThick(long nRound=0) const;	// �β�
	CString GetStrLength(long nRound=0) const;	// ����
	CString GetStrUWeight(long nRound=3) const;		// �����߷�
	CString GetStrTWeight(BOOL bUnitTon=FALSE, long nRound=3) const;	// ��ü�߷�
	CString GetStrBujeKindName() const;			// ���纰 �����̸�	
	CString GetStrWidthBasic(long nRound=0) const;	//	
	CString GetStrThickBasic(long nRound=0) const;	//
	CString GetStrLengthBasic(long nRound=0) const;	//
	CString GetStrTWeightBasic(BOOL bUnitTon=FALSE, long nRound=3) const;	//
	//
	CString GetStrMaterial() const;		// ������Ī
	
	void SetStrSectionType(const CString& sName);	// �ܸ��̸� ����
	void SetStrEleName(const CString& sEleName,long nSecTypeNumber);	// 	��������
	void SetStrEleName(const CString& sEleName);						// ��������
	void SetStrSubEleName(const CString& sName);	// �������
	void SetStrProcessBig(const CString& sName);	// 
	void SetStrProcessMid(const CString& sName);	//

	void SetNormal(BOOL bNormal);
	BOOL IsNormal() const;
	BOOL IsSameRecord(const CBMRecord* pRec,DWORD exceptionFlag = 0) const;
	void InitData();

	int CompareSang(const CBMRecord* pRec) const;				// [01]�⺻���� ��������ǥ
	int CompareStandardSang(const CBMRecord* pRec) const;		// [02]ǥ������ǥ	
	int CompareStandardShopType2(const CBMRecord* pRec) const;	// [03]ǥ������ǥ(���ۿ�)
	int CompareThick(const CBMRecord* pRec) const;				// [04]������ �β�������ǥ
	int CompareThickSang(const CBMRecord* pRec) const;			// [04]������ �β�������ǥ
	int CompareBujeMarkSang(const CBMRecord* pRec) const;		// [05]���縶ũ�� ������ǥ	
	int CompareBujeMarkDeduct(const CBMRecord* pRec) const;		// [07]���縶ũ�� ��������ǥ
	int CompareCar(const CBMRecord* pRec) const;				// [08]�������� ������ǥ
	int CompareCarSection(const CBMRecord* pRec) const;			// [08]�������� ������ǥ
	int CompareCarAssembly(const CBMRecord* pRec) const;		// [08]�������� ������ǥ
	int CompareCarTotal(const CBMRecord* pRec) const;			// [09]�������� �Ѱ�����ǥ
	int CompareGrandAssemblySang(const CBMRecord* pRec) const;	// [10]�������� ������ǥ			
	int CompareAssemblyTotal(const CBMRecord* pRec) const;		// [15]�������� �Ѱ�����ǥ	
	int CompareAssemblyFact(const CBMRecord* pRec) const;		// [17]�������� �������ۿ�����ǥ
	int CompareAssemblyField(const CBMRecord* pRec) const;		// [18]�������� ���弳ġ������ǥ
	int CompareSectionComp(const CBMRecord* pRec) const;		// [19]�ܸ麰 ����ǥ(�⺻�������)
	int CompareBujeComp(const CBMRecord* pRec) const;			// [20]���纰 ����ǥ(�⺻�������)
	int CompareBasicComp(const CBMRecord* pRec) const;			// [21]�⺻�� ����� ����ǥ
	//
	int CompareSectionType(const CBMRecord* pRec) const;
	int CompareBuje(const CBMRecord* pRec) const;
	int CompareSizeType(const CBMRecord* pRec) const;

	long GetDivideEleNameNum(const CString& sName) const;
	CString GetDivideEleNameStr(const CString& sName) const;
	CString GetStrSteelBujeKindName()	const;					// ����Ÿ�� �����̸� ����

};

#endif // !defined(AFX_BMRECORD_H__BC968069_2BE4_4B54_B1C8_34C97869A803__INCLUDED_)
