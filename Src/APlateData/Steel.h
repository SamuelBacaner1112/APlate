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
	BDIST_STANDARD,						// 0. �԰�
	BDIST_SYMBOL,						// 1. �����ȣ
	BDIST_STRETCHSTRESSALLOWMIN,		// 2. ����������� 40����
	BDIST_STRETCHSTRESSALLOWMID,		// 3. ����������� 40-75
	BDIST_STRETCHSTRESSALLOWMAX,		// 4. ����������� 75-100
	BDIST_STRETCHSTRESSALLOWFACTOR,		// 5. ������� ���°��
	BDIST_BOLTSTRESSFACTOR,				// 6. ��Ʈ���� ���
	BDIST_SHEARSTRESSALLOWMIN,			// 7. ����������� 40����
	BDIST_SHEARSTRESSALLOWMID,			// 8. ����������� 40-75
	BDIST_SHEARSTRESSALLOWMAX,			// 9. ����������� 75-100
	BDIST_SHEARSTRESSALLOWFACTOR,		// 10. ������� ���°��
	BDIST_TOTALSTRESSFACTOR,			// 11. �ռ����°��
	BDIST_UNITWEIGHT,					// 12. �����߷�
	BDIST_BOARDTHICKARROW,				// 13. ����� �β�
	BDIST_MARK,							// 14. ������ũ
	BDIST_ALPHA,						// 15. ����â���
	BDIST_ELASTICMODULUS,				// 16. ź�����
	BDIST_TEMPERATURE,					// 17. �µ�
	BDIST_MINFACTOR,					// 18. ������� �ּҰ��
	BDIST_MAXFACTOR,					// 19. ������� �ִ���
	BDIST_STRESSFACTOR,					// 20. ������� ���
	BDIST_AXISMIN,						// 21. ���������������� 40����
	BDIST_AXISMID,						// 22. ���������������� 40-75����
	BDIST_AXISMAX,						// 23. ���������������� 75-100����
	BDIST_BASEYIELDPOINTMIN,			// 24. ������� �����׺��� 40����
	BDIST_BASEYIELDPOINTMID,			// 25. ������� �����׺��� 40-75����
	BDIST_BASEYIELDPOINTMAX,			// 26. ������� �����׺��� 40-100����
	BDIST_SAFETYRATE,					// 27. ������� ������
	BDIST_BOLTSTRESSALLOWMIN,			// 28. ����������� 40����
	BDIST_BOLTSTRESSALLOWMID,			// 29. ����������� 40-75
	BDIST_BOLTSTRESSALLOWMAX,			// 30. ����������� 75-100
	BDIST_BOLTSTRESSALLOWFACTOR,		// 31. ����������� ���
	BDIST_YIELDPOINTMIN,				// 32. �׺��� 16����
	BDIST_YIELDPOINTMID,				// 33. �׺��� 16-40
	BDIST_YIELDPOINTMAX,				// 34. �׺��� 40-75
	BDIST_YIELDPOINTLMAX,				// 35. �׺��� 75�ʰ�
	BDIST_STRETCHSTRENGTHMIN,			// 36. ���尭�� MIN
	BDIST_STRETCHSTRENGTHMAX,			// 37. ���尭�� MAX
	BDIST_X1,							// 38. 11,12ȣ ����
	BDIST_X2,							// 39. 5ȣ����
	BDIST_BENDINGANGLE,					// 40. ��������
	BDIST_INNERRADIUS,					// 41. ���ʹ�����
	BDIST_STRAIGHTBOARDDISTANCE,		// 42. �������
	BDIST_LENGTHPERWEIGHT,				// 43. ���̴��߷�
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
