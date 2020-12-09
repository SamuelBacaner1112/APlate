// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SteelDeduct.h: interface for the CSteelDeduct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELDEDUCT_H__EFE71779_DD34_4984_83BF_ECE15662B1E5__INCLUDED_)
#define AFX_STEELDEDUCT_H__EFE71779_DD34_4984_83BF_ECE15662B1E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelDeduct  
{
public:
	CSteelDeduct();
	virtual ~CSteelDeduct();

	long m_nMatApplyType;
	CStringArray m_MatApplyArr;

	long m_bDeductUpperFlange;		// 1. 상판
	long m_bDeductLowerFlange;		// 2. 하판
	long m_bDeductWeb;				// 3. 복부판
	long m_bDeductHStiff;			// 5. 수평보강재
	long m_bDeductVerStiff;			// 6. 수직보강재
	long m_bDeductCrossBeamWebPlate;// 7. 가로보 복부판
	long m_bDeductCrossBeamBracketWebPlate;	// 8. 가로보 브라켓 복부판
	long m_bDeductCrossBeamBracketUDFlange;	// 9. 가로보 브라켓 상.하판
	long m_bDeductCrossBeamBracketStiffner;	// 10. 가로보 브라켓 보강재
	long m_bDeductCrossBeamBracketStiffPlate;//11. 가로보 브라켓 보강판		
	long m_bDeductDongBari;			// 12. 유지보수형 동바리	
	long m_bDeductGeneralScallop;	// 13. 일반 스캘럽
	long m_bDeductEdgeCut;			// 14. 모다기 공제
	long m_bDeductBoltHole;			// 15. 볼트홀 공제
	long m_bDeductSplice;			// 16. 현장이음 공제
	long m_bDeductSolePlate;		// 17. 솔플레이트 공제
		
	long m_bSolePlateForJackUp; // 잭업용 솔플레이트
	long m_bBSAWeightApply; // 볼트, 전단연결재, 슬래브 앵커 중량적용
	long m_bSectionWeightApply; // 단면 기본 중량 기준적용
	long m_nFloatNum;			// 반올림 자리수
	

// Implementation
public:
	CString GetMatApplyType();
	void SetMatApplyType(CString rtStr);
	void InitData();

public:	
	void Serialize(CArchive& ar);
};

#endif // !defined(AFX_STEELDEDUCT_H__EFE71779_DD34_4984_83BF_ECE15662B1E5__INCLUDED_)
