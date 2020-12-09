// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DataManage.h: interface for the CDataManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMANAGE_H__50DCF5F4_AE1B_4452_AAB2_C5579A0080BD__INCLUDED_)
#define AFX_DATAMANAGE_H__50DCF5F4_AE1B_4452_AAB2_C5579A0080BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARoadOptionStd;
class CDomyunGLView;
class CLineInfoApp;
class CPlateBridgeApp;
class CSteel;
class CSteelMaterial;
class CSteelSection;
class CSteelDefine;
class CSteelRange;
class CSteelGrade;
class CSteelWeld;
class CSteelMark;
class CSteelDeduct;
class CSteelBolt;
class CSteelStud;
class CSteelScallop;
class CSteelUWeight;
class CSteelConc;
class CGlobarOption;
class CSteelPrestressPlate;

class CCalcData;

class CBMCreator;
class CPaintCreator;
class CWeldStandard;
class CADeckData;
class CADeckDataTU;

#define ANGLE90	CDPoint(0,1)
#define GL_OPT() CDataManage::GetGlobalOption()
#define GL_BUJE() CDataManage::GetSteelSection()
#define GL_BOLT() CDataManage::GetSteelBoltSize()
#define GL_DEDUCT() CDataManage::GetSteelDeduct()
#define GL_WELD() CDataManage::GetSteelWeld()
#define GL_STEEL() CDataManage::GetSteel()
#define GL_STUD() CDataManage::GetSteelStud()
#define GL_UWEIGHT() CDataManage::GetSteelUWeight()
#define GL_PUSH_METHOD_EL(nMethod) double dTempElMethod = CDataManage::GetGlobalOption()->GetMethodOfStationForElevation();	\
CDataManage::GetGlobalOption()->SetMethodOfStationForElevation(nMethod)
#define GL_POP_METHOD_EL()		   CDataManage::GetGlobalOption()->SetMethodOfStationForElevation(dTempElMethod)

class AFX_EXT_CLASS CDataManage 
{
public:
	CDataManage();
	virtual ~CDataManage();

	CBMCreator		*m_pBMStandard;		// Doc 俊辑 积己
	CPaintCreator	*m_pPaintCreator;	// Doc 俊辑 积己
	CWeldStandard	*m_pWeldStandard;   // Doc 俊辑 积己
protected:
	CDomyunGLView *m_pView;

	CARoadOptionStd *m_pConfig;
	CLineInfoApp	*m_pLineInfoApp;
	CPlateBridgeApp	*m_pBridge;	
	CCalcData		*m_pCalcData;
	CADeckData      *m_pDeckData;
	CADeckDataTU    *m_pDeckDataTU;

	CSteel			*m_pSteel;
	CSteelMaterial	*m_pSteelMaterial;
	CSteelSection	*m_pSteelSection;
	CSteelDefine	*m_pSteelDefine;
	CSteelRange		*m_pSteelRange;
	CSteelGrade		*m_pSteelGrade;
	CSteelWeld		*m_pSteelWeld;
	CSteelDeduct    *m_pSteelDeduct;
	CSteelBolt		*m_pSteelBolt;
	CSteelStud		*m_pSteelStud;
	CSteelScallop	*m_pSteelScallop;
	CSteelUWeight	*m_pSteelUWeight;
	CSteelConc		*m_pSteelConc;
	CSteelPrestressPlate	*m_pSteelPrestress;


	CSteelMark		*m_pSteelMark;


	static CGlobarOption	*m_pGlobarOption;

public:
	void Serialize(CArchive &ar);

public:
	void SetDomyunView(CDomyunGLView *pView) { m_pView = pView; }

	CDomyunGLView *GetDomyunView()	const	{ return m_pView; }
	CDomyun *GetDomyun()			const;
	CPlateBridgeApp *GetBridge()	const	{ return m_pBridge; }
	CCalcData *GetCalcData()		const	{ return m_pCalcData; }
	CADeckData *GetDeckData()		const	{ return m_pDeckData; }
	CADeckDataTU *GetDeckDataTU()		const	{ return m_pDeckDataTU; }
	CLineInfoApp *GetLineInfoApp()	const	{ return m_pLineInfoApp; }
	CARoadOptionStd *GetOptionStd()	const	{ return m_pConfig; }
	
	CBMCreator *GetBMStandard() const { return m_pBMStandard; }
	CPaintCreator *GetPaintStandard() const { return m_pPaintCreator; }
	CWeldStandard *GetWeldStandard() const { return m_pWeldStandard; }

	CSteel *GetSteel() const { return m_pSteel; }
	CSteelMaterial *GetSteelMaterial() const{ return m_pSteelMaterial; }
	CSteelSection *GetSteelSection() const { return m_pSteelSection; }
	CSteelDefine *GetSteelDefine() const { return m_pSteelDefine; }
	CSteelRange *GetSteelRange() const { return m_pSteelRange; }
	CSteelGrade *GetSteelGrade() const { return m_pSteelGrade; }
	CSteelWeld *GetSteelWeld() const { return m_pSteelWeld; }
	CSteelDeduct *GetSteelDeduct() const { return m_pSteelDeduct; }
	CSteelBolt *GetSteelBolt() const { return m_pSteelBolt; }
	CSteelStud *GetSteelStud() const { return m_pSteelStud;	}
	CSteelScallop *GetSteelScallop() const { return m_pSteelScallop; }
	CSteelUWeight *GetSteelUWeight() const { return m_pSteelUWeight; }
	CSteelConc *GetSteelConc() const { return m_pSteelConc; }
	CSteelPrestressPlate *GetSteelPrestress() const {return m_pSteelPrestress; }
	
	CSteelMark *GetSteelMark() const { return m_pSteelMark; }

	static CGlobarOption *GetGlobalOption();

	void SetCheckEmptyMaterial(BOOL bCheck);
	void SetCheckMap(CString szKey, CString szMessage);
	CString GetCheckMap(CString szKey);
	BOOL IsCheckEmptyMaterial() const { return m_bCheckEmptyMaterial; }
	
	double GetFilletWeldStandard(double dThick1, double dThick2);

private:
	BOOL m_bCheckEmptyMaterial;
	CMapStringToString m_MapCehckEmptyMaterial;
};

#endif // !defined(AFX_DATAMANAGE_H__50DCF5F4_AE1B_4452_AAB2_C5579A0080BD__INCLUDED_)
