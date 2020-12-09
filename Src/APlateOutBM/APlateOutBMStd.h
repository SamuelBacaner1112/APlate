// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutBMStd.h: interface for the CAPlateOutBMStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTBMSTD_H__E0509B28_7BF0_43B5_B6A0_3EB9AA91EB96__INCLUDED_)
#define AFX_APLATEOUTBMSTD_H__E0509B28_7BF0_43B5_B6A0_3EB9AA91EB96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class COptionStd;
class CAPlateOutBMDlg;
class CAPlateOutNonDestructionDlg;
class CAPlateNonDestructionXL;
class CAPlateOutWeldChangeDlg;
class CAPlateWeldChangeXL;
class CAPlateDrawDanmyun;
class CAPlateBMXL;
class CAPlateOutBMFormat;

class AFX_EXT_CLASS CAPlateOutBMStd
{
public:
	CAPlateOutBMStd(CDataManage *pDataManage, CGeneralBaseStd *pGenInputStd, COptionStd *pOptionStd);
	virtual ~CAPlateOutBMStd();

public:
	void CAPlateOutBMStdInit(CDataManage *pDataManage, CGeneralBaseStd *pGenInputStd, COptionStd *pOptionStd);
	void CAPlateOutBMStdClose();
	
	CAPlateOutBMDlg*				GetAPlateOutBMDlg()		{return m_pAPlateOutBMDlg;};
	CAPlateOutNonDestructionDlg*	GetAPlateOutNonDesDlg()	{return m_pAPlateOutNonDesDlg;};
	CAPlateOutWeldChangeDlg*		GetAPlateOutWeldChangeDlg() { return m_pAPlateWeldChangeDlg; }

	CPlateBridgeApp*				GetBridgeApp()			{ return m_pDB;	};	
	COptionStd*						GetOptionStd()			{ return m_pOptionStd;};
	CDataManage*					GetDataManage()			{ return m_pDataManage;};
	CAPlateBMXL*					GetAPlateBMXL()			{ return m_pAPlateBMXL;};
	CAPlateNonDestructionXL*		GetAPlateNonDestructionXL()	{ return m_pAPlateNonDesXL;	};
	CAPlateWeldChangeXL*			GetAPlateWeldChangeXL()	{ return m_pAPlateWeldChangeXL; };

	CAPlateOutBMFormat*				GetAPlateOutBMFormat()	{ return m_pAPlateOutBMFormat;};
	CGeneralBaseStd*				GetGeneralInputStd()	{ return m_pGenInputStd;};	
	
	CWnd*							GetWnd()				{ return m_pWnd;};
	void							SetWnd(CWnd *pWnd)		{ m_pWnd = pWnd;};
	CString							GetCircleText(long nNumber);
	
	double							m_dXLVersion;

protected:
	CAPlateOutBMDlg				*m_pAPlateOutBMDlg;
	CAPlateBMXL					*m_pAPlateBMXL;
	CAPlateOutNonDestructionDlg	*m_pAPlateOutNonDesDlg;
	CAPlateNonDestructionXL		*m_pAPlateNonDesXL;
	CAPlateOutWeldChangeDlg		*m_pAPlateWeldChangeDlg;
	CAPlateWeldChangeXL			*m_pAPlateWeldChangeXL;
	CAPlateOutBMFormat			*m_pAPlateOutBMFormat;
	
	CDataManage					*m_pDataManage;	
	COptionStd					*m_pOptionStd;
	CPlateBridgeApp				*m_pDB;	
	CGeneralBaseStd				*m_pGenInputStd;
	
	CWnd						*m_pWnd;
};

#endif // !defined(AFX_APLATEOUTBMSTD_H__E0509B28_7BF0_43B5_B6A0_3EB9AA91EB96__INCLUDED_)
