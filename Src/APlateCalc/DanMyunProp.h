// DanMyunProp.h: interface for the CDanMyunProp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DANMYUNPROP_H__98658AD7_C2C4_483C_84BD_F1255FAD7D57__INCLUDED_)
#define AFX_DANMYUNPROP_H__98658AD7_C2C4_483C_84BD_F1255FAD7D57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateBasicIndex;
class AFX_EXT_CLASS CDanMyunProp  
{
public:
	CDanMyunProp(CAPlateCalcStd *pCalcStd, CPlateBasicIndex *pBx);
	CDanMyunProp(const CDanMyunProp& rhs);
	virtual ~CDanMyunProp();

	CDanMyunProp& operator=(const CDanMyunProp& rhs);

	CDataManage* m_pDataManage;
	CAPlateCalcStd *m_pCalcStd;
	CPlateBasicIndex *m_pBx;

	enum { EQUAL_HIGH, BYUNDAN_TYPE } m_nByunType;
	
	CSectionJewon m_MatLeft;
	CSectionJewon m_MatRight;	

	// 단면 저항 모멘트
	double m_dMaxMoment_Left;
	double m_dMinMoment_Left;
	double m_dMaxSta_Left;		//	각각의 모멘트 위치 스테이션
	double m_dMinSta_Left;

	double m_dMaxMoment_Right;
	double m_dMinMoment_Right;
	double m_dMaxSta_Right;
	double m_dMinSta_Right;
	
public:
	BOOL IsByunDanMyun();
	void SetMaterialIndex();
	//int GetJiganByStation(int nG, double dSta);
	void InitCalc();
	long GetIndexUsedMaterial();

};

#endif // !defined(AFX_DANMYUNPROP_H__98658AD7_C2C4_483C_84BD_F1255FAD7D57__INCLUDED_)
