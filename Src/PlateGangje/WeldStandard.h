// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// WeldStandard.h: interface for the CWeldStandard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WELDSTANDARD_H__0ED3D2E5_099B_461E_B970_5558EEB1C461__INCLUDED_)
#define AFX_WELDSTANDARD_H__0ED3D2E5_099B_461E_B970_5558EEB1C461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WeldRecord.h"
class CTStandard;

class AFX_EXT_CLASS CWeldStandard : public CWeldCreator  
{
public:
	CWeldStandard(CDataManage* pMng);
	virtual ~CWeldStandard();

public:
	UINT MakeToCreator();
	UINT MakeToLoadFileXL();
	virtual UINT DoWork(); 

	void SetWorkValue(long nWorkValue,const CString sPath,DIMTYPE nDimType,long nMakeFlag);

	void SetTStandard(CTStandard *pTStandard);
	CTStandard* GetTStandard();
private:
	void AddWeldXLFileLine(const CStringArray& sArr);
	void ShellSortRecord(CPtrList* pList, int (CWeldRecord::*fcmp)(const CWeldRecord*) const) const;
	void MakeListSang();
	void MakeListAssembly();
	void MakeListAssemblyTotal();
	void MakeListAssemblySang();
	void MakeListWeldType();
	void MakeListWeldThick();

private:
	CTStandard* m_pTStandard;
	BOOL m_bAlertWeld;

};

#endif // !defined(AFX_WELDSTANDARD_H__0ED3D2E5_099B_461E_B970_5558EEB1C461__INCLUDED_)
