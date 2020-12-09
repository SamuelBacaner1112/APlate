// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignCondition.h: interface for the CXLDesignCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNCONDITION_H__9440445B_9EFA_4205_9BC0_C840DB5E44BE__INCLUDED_)
#define AFX_XLDESIGNCONDITION_H__9440445B_9EFA_4205_9BC0_C840DB5E44BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalcData;
class CDataManage;
class CAPlateDrawDanmyun;

class CXLDesignCondition : public CAPlateOutCalcXL
{
public:
	
	CXLDesignCondition(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignCondition();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};

	// 설계 조건
	// 1.1 일반사항
	void DesignCommon();
	// 1.2 하중조건
	void LoadCondition();
	// 1.3 사용재료
	void UseMaterial();
	// 1.5 설계방법
	void DesignMethod();
	// 1.6 참고문헌
	void RefLiterature();
	// 제작공정도
	void TU_ProgressOfWork();
	// 2.단면 가정
	void DesignSection();

protected:
	long m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	double	FindMaxMinGirderWidth(BOOL bUpper, BOOL bIsMaxWidth);
	double	FindMaxMinGirderHeight(BOOL bIsMaxHeight);
	void SetLines(int nLine);
};

#endif // !defined(AFX_XLDESIGNCONDITION_H__9440445B_9EFA_4205_9BC0_C840DB5E44BE__INCLUDED_)
