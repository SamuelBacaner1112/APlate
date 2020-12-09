// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// BMCreator.h: interface for the CBMCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMCREATOR_H__2C813849_F3E1_41CA_9673_27BB3A3273CB__INCLUDED_)
#define AFX_BMCREATOR_H__2C813849_F3E1_41CA_9673_27BB3A3273CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000

// ��������ǥ ���ۿ�    
const long MAKE_BM_SANG						=	0x00000001;	// [01]�⺻���� ��������ǥ
const long MAKE_BM_STDANDARD_SANG			=	0x00000002;	// [02]ǥ������ǥ
const long MAKE_BM_STDANDARD_SHOP			=	0x00000004;	// [03]ǥ������ǥ(���ۿ�)
const long MAKE_BM_STDANDARD_SHOP_TYPE2		=	0x00000008;	// [03]ǥ������ǥ(���ۿ�)
const long MAKE_BM_THICK					=	0x00000010;	// [04]������ �β�������ǥ
const long MAKE_BM_THICK_SANG				=	0x00000020;	// [05]������ �β�������ǥ
const long MAKE_BM_BUJEMARK_SANG			=	0x00000040;	// [06]���縶ũ�� ������ǥ
const long MAKE_BM_BUJEMARK_TOTAL			=	0x00000080; // [07]���縶ũ�� �Ѱ�����ǥ
const long MAKE_BM_BUJEMARK_DEDUCT			=	0x00000100; // [08]���縶ũ�� ��������ǥ
const long MAKE_BM_CAR						=	0x00000200;	// [09]�������� ������ǥ
const long MAKE_BM_CARTOTAL					=	0x00000400;	// [10]�������� �Ѱ�����ǥ
const long MAKE_BM_GRAND_ASSEMBLY_SANG 		=	0x00000800;	// [11]�������� ������ǥ
const long MAKE_BM_GRAND_ASSEMBLY_TOTAL		=	0x00001000;	// [12]�������� �Ѱ�����ǥ
const long MAKE_BM_MID_ASSEMBLY_SANG 		=	0x00002000;	// [13]�������� ������ǥ
const long MAKE_BM_MID_ASSEMBLY_TOTAL		=	0x00004000;	// [14]�������� �Ѱ�����ǥ
const long MAKE_BM_ASSEMBLY_SANG			=	0x00008000;	// [15]�������� ������ǥ
const long MAKE_BM_ASSEMBLY_TOTAL			=	0x00010000; // [16]�������� �Ѱ�����ǥ
const long MAKE_BM_ASSEMBLY_DEDUCT			=	0x00020000; // [17]�������� ��������ǥ
const long MAKE_BM_ASSEMBLY_FACT			=	0x00040000; // [18]�������� �������ۿ�����ǥ
const long MAKE_BM_ASSEMBLY_FIELD			=	0x00080000; // [19]�������� ���弳ġ������ǥ
const long MAKE_BM_SECTION_COMP				=	0x00100000; // [20]�ܸ麰 ����ǥ(�⺻�������)
const long MAKE_BM_BUJE_COMP				=	0x00200000; // [21]���纰 ����ǥ(�⺻�������)
const long MAKE_BM_BASIC_COMP				=	0x00400000;	// [22]�⺻�� ����� ����ǥ
// ��������ǥ �����(????)
const long MAKE_BM_SECTIONTYPE				=	0x00800000;
const long MAKE_BM_BUJE						=	0x01000000;
const long MAKE_BM_BUJE_TOTAL				=	0x02000000;
const long MAKE_BM_DEDUCT_SANG				=	0x04000000; 
const long MAKE_BM_DEDUCT_BUJE				=	0x08000000; 
const long MAKE_BM_GIRDER_TOTAL				=	0x10000000; 
const long MAKE_BM_BUJEKIND_TOTAL			=	0x20000000; 

// TOTAL;
const long BM_SUMMARY_ASSEM		= 0x00000001;
const long BM_SUMMARY_ASSEM_MID	= 0x00000002;
const long BM_SUMMARY_ASSEM_BIG	= 0x00000004;
const long BM_SUMMARY_SECTION	= 0x00000008;

const DWORD WM_BM_PROGRESS	 =	WM_APP+2200;
class CDataManage;
class CBMRecord;
class CDataManage;
class CTStandard;


class AFX_EXT_CLASS CBMCreator  : public CThreadWork
{	
// Constructor
public:
	CBMCreator(CDataManage *pMng);
	virtual ~CBMCreator();

// Attributes
protected:
	CDataManage* m_pDataManage;
	CPlateBridgeApp* m_pDB;

	CPtrList m_BMStardardList;	// ǥ�ظ���Ʈ

	long	m_nWorkValue;	// 0:����, 1:tbl���Ϸε�, 2:�׼����Ϸε�
	CString	m_sPath;		// 
	DIMTYPE	m_nDimType;		// DimType
	long	m_nMakeFlag;	// MakeFlag

	DWORD	m_dwMakeMarkFlag;	// Mark Flag
	BOOL	m_bUnitMode;

public:
	// ��������ǥ ���ۿ�
	CPtrList m_BMList_Sang;					// [01]�⺻���� ��������ǥ
	CPtrList m_BMList_StandardSang;			// [02]ǥ������ǥ
	CPtrList m_BMList_StandardShop;			// [03]ǥ������ǥ(���ۿ�)
	CPtrList m_BMList_StandardShopType2;	// [03]ǥ������ǥ(���ۿ뵿��)(2002.09.23)
	CPtrList m_BMList_Thick;				// [04]������ �β�������ǥ
	CPtrList m_BMList_ThickSang;			// [04]�β��� ������ǥ (2002.09.23)
	CPtrList m_BMList_BujeMarkSang;			// [05]���縶ũ�� ������ǥ
	CPtrList m_BMList_BujeMarkTotal;		// [06]���縶ũ�� �Ѱ�����ǥ
	CPtrList m_BMList_BujeMarkDeduct;		// [07]���縶ũ�� ��������ǥ
	CPtrList m_BMList_Car;					// [08]�������� ������ǥ
	CPtrList m_BMList_CarTotal;				// [09]�������� �Ѱ�����ǥ
	CPtrList m_BMList_GrandAssemblySang;	// [10]�������� ������ǥ
	CPtrList m_BMList_GrandAssemblyTotal;	// [11]�������� �Ѱ�����ǥ
	CPtrList m_BMList_MidAssemblySang;		// [12]�������� ������ǥ
	CPtrList m_BMList_MidAssemblyTotal;		// [13]�������� �Ѱ�����ǥ
	CPtrList m_BMList_AssemblySang;			// [14]�������� ������ǥ
	CPtrList m_BMList_AssemblyTotal;		// [15]�������� �Ѱ�����ǥ
	CPtrList m_BMList_AssemblyDeduct;		// [16]�������� ��������ǥ
	CPtrList m_BMList_AssemblyFact;			// [17]�������� �������ۿ�����ǥ
	CPtrList m_BMList_AssemblyField;		// [18]�������� ���弳ġ������ǥ
	CPtrList m_BMList_SectionComp;			// [19]�ܸ麰 ����ǥ(�⺻�������)
	CPtrList m_BMList_BujeComp;				// [20]���纰 ����ǥ(�⺻�������)
	CPtrList m_BMList_BasicComp;			// [21]�⺻�� ����� ����ǥ
											
	// ��������ǥ �����	
	CPtrList m_BMList_SectionType;			// �ܸ�Ÿ�Ժ� ����ǥ
	CPtrList m_BMList_Buje;					// ���纰 ����ǥ
	CPtrList m_BMList_BujeTotal;			// ���纰 �Ѱ�����ǥ
	CPtrList m_BMList_BujeKindTotal;		// ���������� �Ѱ�����ǥ
	CPtrList m_BMList_DeductSang;			// �󼼺�����
	CPtrList m_BMList_DeductBuje;			// ���纰����
	CPtrList m_BMList_Girder;				// �Ŵ�������

	////////////////////////////////////////////////////////////////////////
	CPtrList m_BMUnitList;				// �������ǥ ������ ���� LIST
	CPtrList m_BMGangjeList;			// �ܸ麰,SET���� ����

// Implementation
protected:
	// ����
	void DeductBMFactChain(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,long nMarkFactType);
	void DeductBMFlangeSkew(CPlateBasicIndex* pBx, BOOL bUpper);
	void DeductBMUpperFlangeBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx);
	void DeductBMLowerFlangeBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx);	
	void DeductBMWebTaper(CPlateBasicIndex* pBx);
	void DeductBMWebBoltHole(CPlateBasicIndex* pBx, BOOL bEndBx);		
	// ����������,���򺸰���
	void DeductBMVStiffner(CPlateBasicIndex* pBx);
	void DeductBMHStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,BOOL bLeft);
	// ��������
	void DeductBMSpliceWeb(CPlateBasicIndex* pBx);
	void DeductBMSpliceUpperBoltHole(CPlateBasicIndex* pBx);
	void DeductBMSpliceLowerBoltHole(CPlateBasicIndex* pBx);
	void DeductBMSpliceWebBoltHole(CPlateBasicIndex* pBx);
	// ���κ�
	void DeductBMCrossBeamMain(CPlateBasicIndex* pBx);
	void DeductBMCrossBeamBracket(CPlateBasicIndex* pBx, BOOL bLeft);
	void DeductBMCrossBeamBracketGasset(CPlateBasicIndex* pBx, BOOL bLeft);
	void DeductBMCrossBeamSplice(CPlateBasicIndex* pBx, BOOL bLeft);
	
// Implementation
protected:
	void AddBMFactChain(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,long nMarkFactType);	
	void AddBMHStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd,BOOL bLeft);
	void AddBMSplice(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, BOOL bIncludeStt=TRUE, BOOL bIncludeEnd=TRUE);
	void AddBMSpliceUpper(CPlateBasicIndex* pBx);
	void AddBMSpliceLower(CPlateBasicIndex* pBx);
	void AddBMSpliceWeb(CPlateBasicIndex* pBx);
	
	// ���� �극�̽�
	void AddBMVBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMVBracingAngle(CPlateBasicIndex* pBx);
	void AddBMVBracingGusset(CPlateBasicIndex* pBx);
	void AddBMVBracingBolt(CPlateBasicIndex* pBx);
	// ���� �극�̽�
	void AddBMHBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMHBracingAngle(CPlateBasicIndex* pBx);
	void AddBMHBracingGusset(CPlateBasicIndex* pBx);
	void AddBMHBracingBolt(CPlateBasicIndex* pBx);

	// ���� ������
	void AddBMVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	
	// ���κ�
	void AddBMCrossBeam(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMCrossBeamMain(CPlateBasicIndex* pBx);
	void AddBMCrossBeamBracket(CPlateBasicIndex* pBx, BOOL bLeft);
	void AddBMCrossBeamBracketGusset(CPlateBasicIndex* pBx, BOOL bLeft);
	void AddBMCrossBeamGirderVStiff(CPlateBasicIndex* pBx, CPlateBasicIndex* pBxEnd);
	void AddBMCrossBeamSplice(CPlateBasicIndex* pBx, BOOL bLeft);
	void AddBMVBracingGirderVStiff(CPlateBasicIndex* pBx, CPlateBasicIndex* pBxEnd);
	//
	void AddBMGitaBuje(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd);
	void AddBMGitaBuje_Dongbari(CPlateBasicIndex* pBx);
	void AddBMGitaBuje_JackUpPlate(CPlateBasicIndex* pBx);
	void AddBMGitaBuje_SolePlate(CPlateBasicIndex* pBx);
	void AddBMWeldStud(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	// ���ǿ� ����
	void AddBMJackUpStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, __int64 BxFlag=BX_JACKUP);
	void AddBMJiJumStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd, __int64 BxFlag=BX_JIJUMSTIFF);
	void AddBMJackUpStiffSub(CPlateBasicIndex* pBx);	
	void AddBMFacePlate(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);

	void ClearBase();
	void ClearAll();
	BOOL SameAndIncludeRecord(CPtrList* pList, const CBMRecord* pRec,DWORD exceptionFlag=0);

	CBMRecord* AddBMRecord(CBMRecord* pRec =NULL);
	CBMRecord* AddSimpleBMRecord(CBMRecord* pRec,long nMarkPiece,CPlateBasicIndex* pBx,const CString& sSubEleName);
	CString GetMarkBx(long nMarkType,long nMark,CPlateBasicIndex* pBx,long nRow=0,long nCol=0, BOOL bFront=TRUE) const;

public:
	void SetMakeMark(long nMAKE_MARK,BOOL bApply);
	BOOL IsMakeMark(long nMAKE_MARK) const;

private:
	void SetBMSpliceRecord(CBMRecord *pRec, CPlateBasicIndex* pBx);	
	void SetBMCrossBeamRecord(CBMRecord *pRec, CPlateBasicIndex* pBx);	
	void SetBMVStiffRecord(CBMRecord *pRec, CPlateBasicIndex* pBx);
	void SetJewonBMRecord(CBMRecord *pRec, CPlateBasicIndex* pBx, long nMarkBuje, long nRow=0, long nCol=0, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE, CVector *pJewon=NULL, CVector *pJewonBasic=NULL);
	void SetDeductRecord(CBMRecord *pRec, CPlateBasicIndex* pBx, long nMarkBuje, long nRow=0, long nCol=0, BOOL bFront=TRUE, BOOL bLeftBeam=TRUE);

	/////////////////////////////////////////////////////////
	// BMStandard
public:
	CTStandard* m_pTStandard;
 
	// AROAD
	void GetBMListBoxGangje(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bSpaceTerm=FALSE);
	void GetBMListCrossBeam(CPtrList* pList, CPlateBasicIndex *pBx);	
	void GetBMListVBracing(CPtrList* pList, CPlateBasicIndex *pBx);
	void GetBMListByBxAndFlagSang(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 BxFlag, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bCrossBeam=FALSE);
	// ASHOP
	void GetBMListGangjeAllGirder(CPtrList* pList,  __int64 BxFlag, long nMarkMidAssem, long nMarkAssem, long nMarkSection);
	void GetBMListByBxAndFlag(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 BxFlag, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bCrossBeam=FALSE);
	void GetBMListAssemblyBox(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, BOOL bSpaceTerm=FALSE);
	void GetBMListAssemblySection(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeSttBx=TRUE, BOOL bIncludeEndBx=TRUE, __int64 BxFlag=0x00);
	void GetBMListAssemblyCrossBeam(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bUnitWeightBracket, BOOL bUnitWeightSplice);
	void GetBMListAssemblyCrossBeamGasset(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bLeft=TRUE);
	void GetBMListAssemblyCrossBeamSplice(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void GetBMListAssemblySerobo(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd);
	void GetBMListAssemblySeroboGasset(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bStt=TRUE);
	void GetBMListAssemblySeroboSplice(CPtrList* pList, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bStt=TRUE);
	// ���κ��󼼵��� ������	
	void MakeListCrossBeamJewon();
	CPtrList m_BMCrossBeamList;

// Implementation
protected:
	UINT AddAllCreator();
	UINT MakeToLoadFileXL();

	void ShellSortBMRecord(CPtrList* pList, int (CBMRecord::*fcmp)(const CBMRecord*) const) const;
	void MakeUnitList(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, __int64 BxFlag, BOOL bIncludeSttBx, BOOL bIncludeEndBx, BOOL bCrossBeam);

	// ��������ǥ ���ۿ�
	void MakeListSang();				// [01]�⺻���� ��������ǥ : 2002.06.10
	void MakeListStandardSang();		// [02]ǥ������ǥ
	void MakeListStandardShop();		// [03]ǥ������ǥ(���ۿ�)
	void MakeListStandardShopType2();	// [03]ǥ������ǥ(���ۿ�) - 2002.09.23
	void MakeListThick();				// [04]������ �β�������ǥ
	void MakeListThickSang();			// [04]�β��� ������ǥ  - 2002.09.23
	void MakeListBujeMarkSang();		// [05]���縶ũ�� ������ǥ
	void MakeListBujeMarkTotal();		// [06]���縶ũ�� �Ѱ�����ǥ
	void MakeListBujeMarkDeduct();		// [07]���縶ũ�� ��������ǥ
	void MakeListCar();					// [08]�������� ������ǥ
	void MakeListCarTotal();			// [09]�������� �Ѱ�����ǥ
	void MakeListGrandAssemblySang();	// [10]�������� ������ǥ
	void MakeListGrandAssemblyTotal();	// [11]�������� �Ѱ�����ǥ
	void MakeListMidAssemblySang();		// [12]�������� ������ǥ
	void MakeListMidAssemblyTotal();	// [13]�������� �Ѱ�����ǥ
	void MakeListAssemblySang();		// [14]�������� ������ǥ
	void MakeListAssemblyTotal();		// [15]�������� �Ѱ�����ǥ
	void MakeListAssemblyDeduct();		// [16]�������� ��������ǥ
	void MakeListAssemblyFact();		// [17]�������� �������ۿ�����ǥ
	void MakeListAssemblyField();		// [18]�������� ���弳ġ������ǥ
	void MakeListSectionComp();			// [19]�ܸ麰 ����ǥ(�⺻�������)
	void MakeListBujeComp();			// [20]���纰 ����ǥ(�⺻�������)
	void MakeListBasicComp();			// [21]�⺻�� ����� ����ǥ

	// ��������ǥ �����
	void MakeListSectionType();		// �ܸ�Ÿ�Ժ� ����ǥ
	void MakeListBuje();			// ���纰 ����ǥ
	void MakeListBujeTotal();		// ���纰 �Ѱ�����ǥ
	void MakeListBujeKindTotal();	// ���������� �Ѱ�����ǥ
	void MakeListDeductSang();		// �󼼺�����
	void MakeListDeductBuje();		// ���纰����
	void MakeListGirder();			// �Ŵ��� ����ǥ

	// ��������Ʈ ����
	void MakeListGangje();
	void AddBMXLFileLine(const CStringArray& sArr);

public:
	virtual UINT DoWork();
	double GetTotalWeight() {return m_TotalWeight;}
	void SetWorkValue(long nWorkValue,const CString sPath,DIMTYPE nDimType,long nMakeFlag);
	void SetDimType(DIMTYPE nDimType) {m_nDimType = nDimType;}	
	BOOL IsCarBuje(CBMRecord *pRec);

private:
	double m_TotalWeight;	
	void SetPlateTypeInStandardList();



};

#endif // !defined(AFX_BMCREATOR_H__2C813849_F3E1_41CA_9673_27BB3A3273CB__INCLUDED_)
