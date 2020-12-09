// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignStud.h: interface for the CXLDesignStud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNSTUD_H__62B43269_9855_4B2D_B871_B02830517C1B__INCLUDED_)
#define AFX_XLDESIGNSTUD_H__62B43269_9855_4B2D_B871_B02830517C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXLDesignStud : public CAPlateOutCalcXL  
{
public:
	CXLDesignStud(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignStud();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void ShearConnector();
	void SquareSHConnector();
	void HorSHSteel();

public:
	long				m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;

	///< (1) 합성후하중에 의한 수평전단력
	void HShearByPostLoad(long nSelectnG, long nSttJoint, long nEndJoint);
	///< (2) 건조수축 및 온도차에 의한 수평전단력
	void HShearOfShrinkage(long nSelectnG, long nSttJoint, long nEndJoint);
	///< (3) STUD의 설계
	void DesignOfStud(long nSelectnG);
	///< (4) 하중조합 및 STUD 의 배치
	void LoadComposeStudArrangement(long nSelectnG, long nSttJoint, long nEndJoint);
	///< (5) 전단연결재의 피로설계
	void FatigueDesignOfStud(long nSelectnG, long nSttJoint, long nEndJoint);
	///< (6) 전단연결재의 극한강도검토
	void UltimateStrengthOfStud(long nSelectnG, long nSttJoint, long nEndJoint);	

	void SetLines(int nLine);
};

#endif // !defined(AFX_XLDESIGNSTUD_H__62B43269_9855_4B2D_B871_B02830517C1B__INCLUDED_)
