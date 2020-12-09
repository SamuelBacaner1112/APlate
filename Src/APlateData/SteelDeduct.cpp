// SteelDeduct.cpp: implementation of the CSteelDeduct class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelDeduct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelDeduct::CSteelDeduct()
{
	InitData();

	m_MatApplyArr.Add("평면치수 좌.우 기준");
	m_MatApplyArr.Add("평면치수 거더 중앙 기준");
	m_MatApplyArr.Add("전개치수 좌우 적용");
	m_MatApplyArr.Add("전개치수 직사각형 적용");
	m_MatApplyArr.Add("전개치수 수축량, 좌우 적용");
	m_MatApplyArr.Add("전개치수 수축량, 직사각형 적용");
	m_MatApplyArr.Add("실중량 기준");
}

CSteelDeduct::~CSteelDeduct()
{
	
}
void CSteelDeduct::InitData()
{
	m_bDeductUpperFlange = TRUE;	// 1. 상판
	m_bDeductLowerFlange = TRUE;	// 2. 하판
	m_bDeductWeb = FALSE;			// 3. 복부판
	m_bDeductHStiff = FALSE;		// 5. 수평보강재
	m_bDeductVerStiff = FALSE;		// 6. 수직보강재
	m_bDeductCrossBeamWebPlate			= FALSE; // 7. 가로보 복부판
	m_bDeductCrossBeamBracketWebPlate	= FALSE; // 8. 가로보 브라켓 복부판
	m_bDeductCrossBeamBracketUDFlange	= FALSE; // 9. 가로보 브라켓 상.하판
	m_bDeductCrossBeamBracketStiffner	= FALSE; // 10. 가로보 브라켓 보강재
	m_bDeductCrossBeamBracketStiffPlate = FALSE; // 11. 가로보 브라켓 보강판		
	m_bDeductDongBari		= FALSE;	// 12. 유지보수형 동바리		
	m_bDeductGeneralScallop = FALSE;	// 13. 일반 스캘럽
	m_bDeductEdgeCut		= FALSE;	// 14. 모따기 공제
	m_bDeductBoltHole		= FALSE;	// 15. 볼트홀 공제
	m_bDeductSplice			= FALSE;	// 16. 현장이음 공제
	m_bDeductSolePlate		= FALSE;	// 17. 솔플레이트 공제
	m_nMatApplyType = 1;				//강재집계표 적용기준						

	m_bSolePlateForJackUp = 1;
	m_bBSAWeightApply = 1;				// 볼트, 전단연결재, 슬래브 앵커 중량적용
	m_bSectionWeightApply = 0;			// 단면 기본 중량 기준적용
	m_nFloatNum = 4;					// 반올림 자리수	
}

CString CSteelDeduct::GetMatApplyType()
{	
	return m_MatApplyArr.GetAt(m_nMatApplyType);		
}

void CSteelDeduct::SetMatApplyType(CString rtStr)
{
	for(long i=0;i<m_MatApplyArr.GetSize();i++)
	{
		if(m_MatApplyArr.GetAt(i) == rtStr)
		{
			m_nMatApplyType = i;
			break;
		}
	}		
}

void CSteelDeduct::Serialize(CArchive& ar)
{
	long nLong = 0;
//	long nFlag = 0;
	long nFlag = 2;	// 현장이음 타입추가로 인한 현장이음공제 추가 ..081002 lina

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nMatApplyType;
		ar << nLong;	// 개구부 공제(YES : 0 )
		ar << nLong;
		ar << nLong;		// 맨홀 공제(YES : 0 )
		ar << nLong;
		ar << nLong; // 외측가로보 공제
		ar << m_bDeductCrossBeamWebPlate;
		ar << m_bDeductWeb;		// 복부판 공제
		ar << m_bDeductUpperFlange;
		ar << m_bDeductCrossBeamBracketStiffPlate;// 브라켓 보강판
		ar << m_bDeductCrossBeamBracketStiffner;
		ar << m_bDeductCrossBeamBracketUDFlange;
		ar << m_bDeductVerStiff;
		ar << m_bDeductHStiff;
		ar << nLong;
		ar << nLong;
		ar << nLong;
		ar << nLong;
		ar << m_bSolePlateForJackUp;	// 잭업용 솔플레이트 포함
		ar << m_bBSAWeightApply; // 볼트, 전단연결재, 슬래브 앵커 중량적용
		ar << m_bSectionWeightApply; // 단면 기본 중량 기준적용
		ar << m_nFloatNum;			// 반올림 자리수
		ar << m_bDeductBoltHole;		

		ar << m_bDeductLowerFlange;
		ar << nLong;
		ar << m_bDeductCrossBeamBracketWebPlate;
		ar << nLong;
		ar << m_bDeductDongBari;
		ar << m_bDeductSplice;
		ar << m_bDeductSolePlate;
	}
	else
	{		
		ar >> nFlag;
		ar >> m_nMatApplyType;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> m_bDeductCrossBeamWebPlate;
		ar >> m_bDeductWeb;		// 복부판 공제
		ar >> m_bDeductUpperFlange;
		ar >> m_bDeductCrossBeamBracketStiffPlate;// 브라켓 보강판
		ar >> m_bDeductCrossBeamBracketStiffner;
		ar >> m_bDeductCrossBeamBracketUDFlange;
		ar >> m_bDeductVerStiff;
		ar >> m_bDeductHStiff;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;
		ar >> nLong;

		ar >> m_bSolePlateForJackUp;	// 잭업용 솔플레이트 공제
		ar >> m_bBSAWeightApply;		// 볼트, 전단연결재, 슬래브 앵커 중량적용
		ar >> m_bSectionWeightApply;	// 단면 기본 중량 기준적용
		ar >> m_nFloatNum;				// 반올림 자리수
		ar >> m_bDeductBoltHole;		

		ar >> m_bDeductLowerFlange;
		ar >> nLong;
		ar >> m_bDeductCrossBeamBracketWebPlate;
		ar >> nLong;
		ar >> m_bDeductDongBari;
		
		if(nFlag > 0)	ar >> m_bDeductSplice;
		if(nFlag > 1)	ar >> m_bDeductSolePlate;
	}
}