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

#define WM_USER_MAKEANALYSIS_NONDES (WM_APP + 1100) // 비파괴검사서

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
	
	CString			m_szReadFileName;		// 읽어서 쓸 dll 파일 이름
	CString			m_szSaveFilePath;		// 저장 파일 이름
	long			m_nTotSheetSu;			// 총 출력 Sheet 수
	long			m_nBMSheetIdx[255];		// 출력할 Sheet의 ID를 차례데로 저장
	
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
