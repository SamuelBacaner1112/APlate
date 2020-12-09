// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutCalcStd.h: interface for the CAPlateOutCalcStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTCALCSTD_H__0F47B84E_2D36_46B7_8A1C_18DCF5530725__INCLUDED_)
#define AFX_APLATEOUTPUT_H__0F47B84E_2D36_46B7_8A1C_18DCF5530725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////////
class CDataManage;
class CDomyun;
class CDataManage;
class CThreadWork;
class CAPlateOutExcel;
class CTStandard;
class CPlateBridgeApp;
class CAPlateOutExcelCheck;
class CAPlateOutCalcXL;
class CAPlateDrawDanmyun;
class CAPlateDrawCross;
class CAPlateCalcStd;
class CCalcFloor;
class CCalcGeneral;
class CFemModelingCalc;
class CFemStressCalc;
class CXLDesignFloor;
class CXLDesignCondition;
class CXLDesignOfMold;
class CXLSectionCheck;
class CXLSpliceCheck;
class CXLDesignStiff;
class CXLDesignCrossBeam;
class CXLDesignBracing;
class CXLDesignStringer;
class CXLDesignStud;
class CXLUsingCheck;
class CXLEtcCheck;
class CXLWeldingCheck;
class CFemModelingDraw;
class CAPlateOutputDlg;
class CXLCheckListFloor;
class CXLCheckListSection;
class CXLCheckListStiff;
class CXLCheckListCrossBeam;
class CXLCheckListUsing;
class CXLCheckListStud;

/////////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CAPlateOutCalcStd : public CObject
{
public:
	CAPlateOutCalcStd(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd, CDocument *pDoc);
	virtual ~CAPlateOutCalcStd();

public:

	CAPlateOutExcel*		GetAPlateOutExcel()		{return m_pAPlateOutExcel;		};
	CAPlateOutExcelCheck*	GetAPlateOutExcelCheck(){return m_pAPlateOutExcelCheck;	};
	CWnd*					GetCWnd()				{return m_pCWnd;				};
	CDataManage*			GetDataManage()			{return m_pDataManage;			};
	CPlateBridgeApp*		GetAPlateBridgeApp()	{return m_pBridgeApp;			};
	CDocument*				GetDocument()			{return m_pDoc;					};
	CTStandard*				GetTStandard()			{return m_pTStd;				};
	CAPlateOutCalcXL*		GetAPlateOutCalcXL()	{return m_pAPlateOutCalcXL;		};
	CAPlateDrawDanmyun*		GetAPlateDrawDanmyun()	{return m_pAPlateDrawDanmyun;	};
	CAPlateDrawCross*		GetAPlateDrawCross()	{return m_pAPlateDrawCross;		};
	CAPlateCalcStd*			GetAPlateCalcStd()		{return m_pCalcStd;				};
	CCalcFloor*				GetCalcFloor()			{return m_pCalcFloor;			};
	CCalcGeneral*			GetCalcGeneral()		{return m_pCalcGeneral;			};
	CFemModelingCalc*		GetFemModelingCalc()	{return m_pCalcFemModelingCalc;	};
	CFemStressCalc*			GetFemStressCalc()		{return m_pCalcFemStress;		};
	CGeneralBaseStd*		GetGeneralInputStd()	{return m_pGenInputStd;			};
	CAPlateOutputDlg*		GetAPlateOutputDlg()	{return m_pAPlateOutputDlg;		};

// 구조계산 Class
	CXLDesignCondition*		GetDesignCondiotion()	{return m_pXLDesignCondition;	};
	CXLDesignFloor*			GetDesignFloor()		{return m_pXLDesignFloor;		};
	CXLDesignOfMold*		GetDesignOfMold()		{return m_pXLDesignOfMold;		};
	CXLSectionCheck*		GetSectionCheck()		{return m_pXLSectionCheck;		};
	CXLSpliceCheck*			GetSpliceCheck()		{return m_pXLSpliceCheck;		};
	CXLWeldingCheck*		GetWeldingCheck()		{return m_pXLWeldingCheck;		};
	CXLDesignStiff*			GetDesignStiff()		{return m_pXLDesignStiff;		};
	CXLDesignCrossBeam*		GetDesignCrossBeam()	{return m_pXLDesignCrossBeam;	};
	CXLDesignBracing*		GetDesignBracing()		{return m_pXLDesignBracing;		};
	CXLDesignStringer*		GetDesignStringer()		{return m_pXLDesignStringer;	};
	CXLDesignStud*			GetDesignStud()			{return m_pXLDesignStud;		};
	CXLUsingCheck*			GetUsingCheck()			{return m_pXLUsingCheck;		};
	CXLEtcCheck*			GetEtcCheck()			{return m_pXLEtcCheck;			};

	CXLCheckListFloor*		GetXLCheckListFloor()	{return m_pXLCheckListFloor;	};
	CXLCheckListSection*	GetXLCheckListSection()	{return	m_pXLCheckListSection;	};
	CXLCheckListStiff*		GetXLCheckListStiff()	{return m_pXLCheckListStiff;	};
	CXLCheckListUsing*		GetXLCheckListUsing()	{return	m_pXLCheckListUsing;};
	CXLCheckListCrossBeam*	GetXLCheckListCrossBeam()	{return m_pXLCheckListCrossBeam;};
	CXLCheckListStud*		GetXLCheckListStud()	{return m_pXLCheckListStud; }

	void SetBridgeType(CString szBridgeType);
	long GetBridgeType() {return m_nBridgeType;};

	void					SetCWnd(CWnd *pCWnd);
	int						MakeAnalysisCal();

	CGeneralBaseStd        *m_pGenInputStd;
	double					m_dXLVersion;

protected:
	void APlateOutCalcStdInit(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd, CDocument *pDoc);
	void APlateOutCalcClose();
	CAPlateOutputDlg		*m_pAPlateOutputDlg;
	CPlateBridgeApp			*m_pBridgeApp;
	CDataManage				*m_pDataManage;
	CWnd					*m_pCWnd;
	CAPlateOutExcel			*m_pAPlateOutExcel;
	CAPlateOutExcelCheck	*m_pAPlateOutExcelCheck;
	CDocument				*m_pDoc;
	CTStandard				*m_pTStd;
	CAPlateOutCalcXL		*m_pAPlateOutCalcXL;
	CAPlateDrawDanmyun		*m_pAPlateDrawDanmyun;
	CAPlateDrawCross		*m_pAPlateDrawCross;
	CAPlateCalcStd			*m_pCalcStd;
	CCalcFloor				*m_pCalcFloor;
	CCalcGeneral			*m_pCalcGeneral;
	CFemModelingCalc		*m_pCalcFemModelingCalc;
	CFemStressCalc          *m_pCalcFemStress;

// 구조계산 Class
	CXLDesignCondition		*m_pXLDesignCondition;
	CXLDesignFloor			*m_pXLDesignFloor;
	CXLDesignOfMold			*m_pXLDesignOfMold;
	CXLSectionCheck			*m_pXLSectionCheck;
	CXLSpliceCheck			*m_pXLSpliceCheck;
	CXLWeldingCheck			*m_pXLWeldingCheck;
	CXLDesignStiff			*m_pXLDesignStiff;
	CXLDesignCrossBeam		*m_pXLDesignCrossBeam;
	CXLDesignBracing		*m_pXLDesignBracing;
	CXLDesignStringer		*m_pXLDesignStringer;
	CXLDesignStud			*m_pXLDesignStud;
	CXLUsingCheck			*m_pXLUsingCheck;
	CXLEtcCheck				*m_pXLEtcCheck;

	CXLCheckListFloor		*m_pXLCheckListFloor;
	CXLCheckListSection		*m_pXLCheckListSection;
	CXLCheckListStiff		*m_pXLCheckListStiff;
	CXLCheckListCrossBeam	*m_pXLCheckListCrossBeam;
	CXLCheckListUsing		*m_pXLCheckListUsing;

	CXLCheckListStud		*m_pXLCheckListStud;

	long m_nBridgeType;		///< 0 : H형강 교량   1 : 판형교   2 : 소수주형
};

#endif // !defined(AFX_APLATEOUTCALCSTD_H__0F47B84E_2D36_46B7_8A1C_18DCF5530725__INCLUDED_)
