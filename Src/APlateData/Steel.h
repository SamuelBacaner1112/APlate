// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Steel.h: interface for the CSteel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEEL_H__79F097B8_0A37_4C96_B5E0_138B29393BB7__INCLUDED_)
#define AFX_STEEL_H__79F097B8_0A37_4C96_B5E0_138B29393BB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum STEEL_DATA_ID {
//	BDIST_VARNAME,
	BDIST_STANDARD,						// 0. 규격
	BDIST_SYMBOL,						// 1. 강재기호
	BDIST_STRETCHSTRESSALLOWMIN,		// 2. 허용인장응력 40이하
	BDIST_STRETCHSTRESSALLOWMID,		// 3. 허용인장응력 40-75
	BDIST_STRETCHSTRESSALLOWMAX,		// 4. 허용인장응력 75-100
	BDIST_STRETCHSTRESSALLOWFACTOR,		// 5. 허용인장 응력계수
	BDIST_BOLTSTRESSFACTOR,				// 6. 볼트응력 계수
	BDIST_SHEARSTRESSALLOWMIN,			// 7. 허용전단응력 40이하
	BDIST_SHEARSTRESSALLOWMID,			// 8. 허용전단응력 40-75
	BDIST_SHEARSTRESSALLOWMAX,			// 9. 허용전단응력 75-100
	BDIST_SHEARSTRESSALLOWFACTOR,		// 10. 허용전단 응력계수
	BDIST_TOTALSTRESSFACTOR,			// 11. 합성응력계수
	BDIST_UNITWEIGHT,					// 12. 단위중량
	BDIST_BOARDTHICKARROW,				// 13. 허용판 두께
	BDIST_MARK,							// 14. 재질마크
	BDIST_ALPHA,						// 15. 선팽창계수
	BDIST_ELASTICMODULUS,				// 16. 탄성계수
	BDIST_TEMPERATURE,					// 17. 온도
	BDIST_MINFACTOR,					// 18. 허용응력 최소계수
	BDIST_MAXFACTOR,					// 19. 허용응력 최대계수
	BDIST_STRESSFACTOR,					// 20. 허용응력 계수
	BDIST_AXISMIN,						// 21. 허용축방향인장응력 40이하
	BDIST_AXISMID,						// 22. 허용축방향인장응력 40-75이하
	BDIST_AXISMAX,						// 23. 허용축방향인장응력 75-100이하
	BDIST_BASEYIELDPOINTMIN,			// 24. 허용응력 기준항복점 40이하
	BDIST_BASEYIELDPOINTMID,			// 25. 허용응력 기준항복점 40-75이하
	BDIST_BASEYIELDPOINTMAX,			// 26. 허용응력 기준항복점 40-100이하
	BDIST_SAFETYRATE,					// 27. 허용응력 안전율
	BDIST_BOLTSTRESSALLOWMIN,			// 28. 허용지압응력 40이하
	BDIST_BOLTSTRESSALLOWMID,			// 29. 허용지압응력 40-75
	BDIST_BOLTSTRESSALLOWMAX,			// 30. 허용지압응력 75-100
	BDIST_BOLTSTRESSALLOWFACTOR,		// 31. 허용지압응력 계수
	BDIST_YIELDPOINTMIN,				// 32. 항복점 16이하
	BDIST_YIELDPOINTMID,				// 33. 항복점 16-40
	BDIST_YIELDPOINTMAX,				// 34. 항복점 40-75
	BDIST_YIELDPOINTLMAX,				// 35. 항복점 75초과
	BDIST_STRETCHSTRENGTHMIN,			// 36. 인장강도 MIN
	BDIST_STRETCHSTRENGTHMAX,			// 37. 인장강도 MAX
	BDIST_X1,							// 38. 11,12호 실험
	BDIST_X2,							// 39. 5호실험
	BDIST_BENDINGANGLE,					// 40. 굽힘강도
	BDIST_INNERRADIUS,					// 41. 안쪽반지름
	BDIST_STRAIGHTBOARDDISTANCE,		// 42. 편평시험
	BDIST_LENGTHPERWEIGHT,				// 43. 길이당중량
};

class CHGBaseSteel;
class CFactoryBaseSteel;

class AFX_EXT_CLASS CSteel  
{
public:
	CSteel();
	virtual ~CSteel();
// Attributes	
public:
	vector<CHGBaseSteel*> m_SteelArr;

// Implementation
public:

	double	GetUnitWeightByMaterial(const CString& sMaterial) const;
	void	Serialize(CArchive& ar);
	void	SetDataInit();

	void	RemoveAll();
	CString GetStringData(long idx, STEEL_DATA_ID ID);
	long	GetIndex(CString strSymbol);
	long	GetSize()	{ return m_SteelArr.size();	}
	long	GetApplyYear() {return m_nApplyYear;}
	// Set
	void	SetSteelAllowValue(long nAllowType, long nIdx, double dAllowValue, long nThickIdx, long nScopeIdx = 0);
	void	SetApplyYear(long nApplyYearIdx);
	void	SetBridgeType(long nBridgeTypeIdx);

	// Get
	CString GetSymbol(long nIdx);
	CString GetStandard(long nIdx);
	double	GetSteelAllowValue(long nAllowType, long nIdx, long nThickIdx, long nSubIdx = 0);
	double	GetUnitWeight() {return 0.00000785;}

private:
	long	m_nApplyYear;
	long	m_nBridgeType;
};


#endif // !defined(AFX_STEEL_H__79F097B8_0A37_4C96_B5E0_138B29393BB7__INCLUDED_)
