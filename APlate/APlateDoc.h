// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDoc.h : interface of the CAPlateDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDOC_H__6826997C_826D_4069_B8B5_0DBD84C9E12D__INCLUDED_)
#define AFX_APLATEDOC_H__6826997C_826D_4069_B8B5_0DBD84C9E12D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WaitForSubProgram.h"
#define DOMTYPENUM 17	// APLATE-631 (18->17)


class CDataManage;
class CGeneralBaseStd;
class CAPlateDrawStd;
class COptionStd;
class CMegaLock;
class CAPlateCalcStd;
class CFEMManage;
class CAPlateOutCalcStd;
class CAPlateOccView;
class CAPlateOutBMStd;
class CAPlateOutNonDestructionDlg;
class CShareBridgeInfoStd;
class CShareWidthADManagerStd;

#define MAXGIRDER_AROAD	20
#define LOADTYPE		6+1
#define MAXLANE			20 + 1
#define STEEL_SHOEMAX	2

typedef struct _STEELBOXSAPOUT {
	long m_nG;
	long m_nIdx[STEEL_SHOEMAX];
	double m_dDeadSteelBeforeCombine[STEEL_SHOEMAX];
	double m_dDeadConcBeforeCombine[STEEL_SHOEMAX];
	double m_dDeadAfterCombine[STEEL_SHOEMAX];
	double m_dDBMaxMin[STEEL_SHOEMAX][2];
	double m_dSinkingMaxMin[STEEL_SHOEMAX][2];
	double m_dTotalMaxMin[STEEL_SHOEMAX][2];	
} strSTEELBOX_SAPOUT;

class CSteelBox_SapOut : public CObject
{
public:
	strSTEELBOX_SAPOUT STEEL_SAPOUT[MAXGIRDER_AROAD][LOADTYPE][MAXLANE][MAXGIRDER_AROAD];
};


class CAPlateDoc : public CDocument
{
protected: // create from serialization only
	CAPlateDoc();
	DECLARE_DYNCREATE(CAPlateDoc)

// Attributes
public:
	CStringArray DomTitleArr;	
	static CString DomTitle[DOMTYPENUM];

	// Operations
public:	
	CDataManage			*m_pDataManage;		//APlateData 전체 관리	
	CGeneralBaseStd		*m_pGenInputStd;	//입력다이얼로그(일반, 상세, 계산)
	CAPlateCalcStd		*m_pCalcStd;		//계산모델링
	CAPlateDrawStd		*m_pPlateDrawStd;	//도면생성관리
	COptionStd			*m_pDomOptionStd;   //도면옵션관리
	CShareWidthADManagerStd	*m_pShareWithADManagerStd;

	static BOOL		m_bFirstDocument;

	BOOL	m_bOptModifyCheck;
	BOOL	m_bBrpFileOpen;
	BOOL	m_bMasterLock;
	long	m_nActiveIDD;
	long	m_nCurDeckNoOfSettingDomyun;	// 도면환경설정에서 현재 작업중이던 교량 인덱스 보관
	BOOL	m_bEnableCrossCursor;
	BOOL	m_bMoveCursor;	

	static CString	m_szSerial;

	void ImportAPierInfo(CArchive &ar);
	void ExportAPierInfo(CArchive &ar);
	void SapReadReaction(CArchive &ar);
	BOOL TransBrtString(const CString &str, long &nStep);
	BOOL ImportBridgeInfo(CArchive &ar);	
	void ExportBridgeInfo(ofstream &out, CString szFileName);
	BOOL MakeBackUp(CString szPathName, BOOL bOpen);
	
	CGeneralBaseStd*	GetPlateInputStd()		{ return m_pGenInputStd; };
	CAPlateOutCalcStd*	GetAPlateOutCalcStd()	{ return m_pAPlateOutCalcStd; };
	CAPlateOutBMStd*	GetAPlateOutBMStd()		{ return m_pAPlateOutBMStd; };
	
	void InitSteelBox_SapOut();
	// added by jaeho 2007.06.01 ///
	CTypedPtrArray <CObArray, CSteelBox_SapOut*> m_pArrSteelBox_SapOut;	//임시적인 스틸박스 SapOut 데이타
	BOOL m_bExistBodoInBrx;
	BOOL m_bExistWindInBrx;
	long m_nOptionCur;
	long m_nCalcLaneSu;
	long m_nFlagExportApierInfo;
	void ExportReactionByJijum(long nJ);
	void SetReactionToShareBridgeInfo(CArchive &ar, long nQJ, long nFlag);
	CShareBridgeInfoStd* m_pShareBridgeInfoStd;

private:
	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;	//구조계산서 출력
	CAPlateOutBMStd		*m_pAPlateOutBMStd;		// DECK 수량 산출 관련...070214...KB...추가...	
	CWaitForSubProgram  m_WaitForALineDlg;
	void CreateMember();
	void ClearMember();

// Operations
public:
	BOOL IsFileOpen()	{return	m_bBrpFileOpen;};
	void SetFileOpen(BOOL bIsFileOpen)	{m_bBrpFileOpen= bIsFileOpen;};


// Overrides
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace = TRUE);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPlateDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
	void SerializeMain(CArchive& ar);

// Implementation
public:
	void ExportAAbutPier2007Info();
	virtual ~CAPlateDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CAPlateDoc)
	//}}AFX_MSG
	afx_msg void OnOutLine();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APLATEDOC_H__6826997C_826D_4069_B8B5_0DBD84C9E12D__INCLUDED_)
