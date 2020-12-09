// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#ifndef  __PLATEGIRDER_H__
#define  __PLATEGIRDER_H__

typedef CTypedPtrArray<CObArray, CPlateBxObject*> CPlateLinkToBxArray;

#include "Shoe.h"

class CCamber;
class CPlateFactChain;
class CPlateCrossBeam;
class CWebHStiff;
class CJackupStiff;
class CJRib;
class CPlateSplice;
class CBracing;
class CSection;
class CHBracingGusset;
class CVStiff;
class CPlateFactChain;
class CRangePress;
///////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS  CPlateGirder : public CObject
{
// Constructor
public:
	CPlateGirder();
	CPlateGirder(CPlateBridge* pDB,CLineInfo* pLine,CPlateGirderApp* pLeft = NULL);
	virtual ~CPlateGirder();
	friend class CPlateBasicIndex;
	friend class CPlateBridge;

public:	
	enum SP_TYPE { SPLICE=0, WELD=1, TOTAL=2};	

	CLineInfo	    *GetLine()      const { return m_pLine; }
	CPlateBridge    *GetBridgeApp() const{ 	return m_pBridgeApp; }

	BOOL IsJustModified() const;
	BOOL IsModify() const;
	BOOL IsDataFilled() const;
	void SetJustModified(BOOL bModify);
	void SetEleLevel(BOOL bVal)	{ m_bEleLevel = bVal; }
	BOOL GetEleLevel()	{ return m_bEleLevel; }
	BOOL IsLineSameLeftOrRight() const ;

	void ReStandVStiff();

// Attributes
public:
	CPlateBridge *m_pBridgeApp;
	CLineInfo    *m_pLine;
	long m_nBoundLineInfo;
	BOOL m_bEleLevel;

	long m_SectSu[10];			
	//////////////////////////////////////////
	CTypedPtrArray <CObArray, CBracing*>		m_BracingArr;		// ����극�̽� 
	CTypedPtrArray <CObArray, CSection*>		m_VStiffSectionArr;	// �ܸ� �� ���������� ��ġ
	CTypedPtrArray <CObArray, CSection*>		m_CrossSectionArr;	// �ܸ� �� ���κ�, �����극�̽� ��ġ
	CTypedPtrArray <CObArray, CHBracingGusset*>	m_HGussetArr;		// ����극�̽� ������
	CTypedPtrArray <CObArray, CVStiff*>			m_JiJumVStiff;		// ����������	
	CTypedPtrArray <CObArray, CWebHStiff*>		m_aWebHStiffArr;	// �Ŵ� ���򺸰���
	CTypedPtrArray <CObArray, CJackupStiff*>	m_aJackupArr;		// ���������
	CTypedPtrArray <CObArray, CPlateFactChain*>	m_aFactChain[3];	// ��������
	CTypedPtrArray <CObArray, CCamber*>			m_pCamber;			// ķ��
	CTypedPtrArray <CObArray, CPlateSplice*>	m_pSplice;			// ����������
	//	
	CShoe m_Shoe[JIJUMSUMAX];										// ��
	
	CPtrList	m_BxList;	

protected:
	double	m_dHeightOnJijum[JIJUMSUMAX]	;	// �Ŵ�����

public:
	CRangePress		*m_pRangePress;		// ����, ���屸��
	CPlateGirderApp *m_pLeft;			// ����Ŵ����� �����Ŵ�
	CPlateGirderApp *m_pRight;			// ����Ŵ����� �����Ŵ�
	CPlateSangSeDo  *m_pSangse;			// �󼼵��� �ʿ��� ���� �� ���̽�
	
	double	m_dGirderCenter					;	// �Ŵ��߽���ġ(��������)	
	long	m_nLineNum						;	// ������ȣ
	long    m_nInstallVStiff;					// ����, ����, ����

// Implementation
	void SetPOSAndEachPointer();
protected:
	void SetLKGirderPointAndSelfCount(void* pTPA);
	void SetLKSideFactChain();		
	void SetEnTypeNumSplice();

public:
	CPlateGirder& operator=(const CPlateGirder& obj);

public:	
	virtual void Serialize(CArchive& ar);		
		//���� �ÿ��� ���
	CPlateLinkToBxArray * GetLKArr(long nLK) const;
	void SetLKEndPOS(void* pTPA);
	void SetLKClearPOS(long nLK);
	BOOL CheckSelfData(BOOL bAssertUse);
	void SetBxPosLeftRight(__int64 bxLeftFlag); 
	CPlateBasicIndex* GetBx(POS pos) const{ if(!pos) return NULL; return (CPlateBasicIndex*)m_BxList.GetAt(pos);}	
	CPtrList& GetBxList()  { return m_BxList; }
	virtual CPlateBasicIndex* newBx(CPlateBasicIndex *pBx = NULL);
	void SetLine(CLineInfo* pLine) { m_pLine = pLine; }	
	void SetRightGirder(CPlateGirderApp* pRight) {m_pRight = pRight;}	
};

#endif  // __PLATEGIRDER_H__
