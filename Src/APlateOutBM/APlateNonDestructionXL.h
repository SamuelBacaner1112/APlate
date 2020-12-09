// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateNonDestructionXL.h: interface for the CAPlateNonDestructionXL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATENONDESTRUCTIONXL_H__E09929A0_DE0C_4D1B_825B_085A927F46C3__INCLUDED_)
#define AFX_APLATENONDESTRUCTIONXL_H__E09929A0_DE0C_4D1B_825B_085A927F46C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_MAKEANALYSIS_NONDES (WM_APP + 1100) // ���ı��˻缭

class CAPlateOutBMBrInfo;


class CAPlateNonDestructionXL : public CThreadWork
{
public:
	CAPlateNonDestructionXL(CAPlateOutBMStd *pAPlateOutBMStd);
	virtual ~CAPlateNonDestructionXL();

	virtual	UINT DoWork();
	void KillTheXL();
	void SetProgress(CString strText, long nlW);
	
	CAPlateOutBMStd		*m_pAPlateOutBMStd;
	CAPlateOutBMBrInfo	*m_pAPlateOutBMBrInfo;		
	
	CAPlateOutBMBrInfo* GetBridgeInfo(long nBridgeIdx);
	
	CString			m_szReadFileName;		// �о �� dll ���� �̸�
	CString			m_szSaveFilePath;		// ���� ���� �̸�
	long			m_nTotSheetSu;			// �� ��� Sheet ��
	long			m_nBMSheetIdx[255];		// ����� Sheet�� ID�� ���ʵ��� ����
	
	CString GetNamePrintSheet(long nSheet);
	void APlateOutNonDes(CXLControl	*pXL, long nOutIdx, CStringArray &ArrDelSheet);
	void NonDestructionStandard(CXLControl *pXL, CStringArray &ArrDelSheet);
	void NonDestructionCalcFoundation(CXLControl *pXL, CStringArray &ArrDelSheet);
	void NonDestructionSummary(CXLControl *pXL, CStringArray &ArrDelSheet);
	
private:
	CString GetAlphabet(long nCol);		
	CMap <CString, LPCTSTR, CString, CString> m_MapRefCell;
};

#endif // !defined(AFX_APLATENONDESTRUCTIONXL_H__E09929A0_DE0C_4D1B_825B_085A927F46C3__INCLUDED_)
