#if !defined(AFX_PLATEWELDCHANGEXL_H__A189D141_54B8_45B3_919D_25F47E488002__INCLUDED_)
#define AFX_PLATEWELDCHANGEXL_H__A189D141_54B8_45B3_919D_25F47E488002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlateWeldChangeXL.h : header file
//
#define WM_USER_MAKEANALYSIS_WELDCHANGE (WM_APP + 1200) // 표준 제작 공수
class CAPlateOutBMBrInfo;

/////////////////////////////////////////////////////////////////////////////
// CAPlateWeldChangeXL window

class CAPlateWeldChangeXL : public CThreadWork
{
// Construction
public:
	CAPlateWeldChangeXL(CAPlateOutBMStd *pAPlateOutBMStd);
	virtual ~CAPlateWeldChangeXL();

// Attributes
public:
	CAPlateOutBMStd		*m_pAPlateOutBMStd;
	CAPlateOutBMBrInfo	*m_pAPlateOutBMBrInfo;
	
	CString			m_szReadFileName;		// 읽어서 쓸 dll 파일 이름
	CString			m_szSaveFilePath;		// 저장 파일 이름	

// Operations
public:
	virtual	UINT DoWork();
	void KillTheXL();
	void SetProgress(CString strText, long nlW);
	CAPlateOutBMBrInfo* GetBridgeInfo(long nBridgeIdx);
	void APlateOutWeldChangeDes(CXLControl *pXL);

private:


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATEWELDCHANGEXL_H__A189D141_54B8_45B3_919D_25F47E488002__INCLUDED_)
