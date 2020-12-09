// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Camber.h: interface for the CCamber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMBER_H__B5B76534_91E7_4C23_9A31_A9A0A60A64C3__INCLUDED_)
#define AFX_CAMBER_H__B5B76534_91E7_4C23_9A31_A9A0A60A64C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CCamber : public CObject
{
// Construtor
public:
	CCamber();
	virtual ~CCamber();
// Implementation
public:
	void Serialize(CArchive& ar);
	void SerializeForOldVersion(CArchive& ar);
	CCamber& operator=(const CCamber& obj);
	void SetCamberByType(long nBridgeType, long nType, double dValue);
	double GetCamberByType(long nBridgeType, long nType);
	void SetCamberStation(double dSta) { m_dStationCamber = dSta; }
	double GetCamberStation() const { return m_dStationCamber; }

	void SetCamberByType_GenGirder(long nType, double dValue);
	void SetCamberByType_TUGirder(long nType, double dValue);
	double GetCamberByType_GenGirder(long nType);
	double GetCamberByType_TUGirder(long nType);
// Attributes
public:
	double		m_User[10];			//5~6
	short m_nFlag;
	void SetOldStationCamber(double staJijum);

protected:
	double		m_dStationCamber;
	double		m_dCamberJongDan;			//0	종단곡선
	double		m_dCamberPyungubae;			//1 편경사
	double		m_dCamberSteelOnly;			//2 강재자중
	double		m_dCamberFixLoad;			//3 
	double		m_dCamberVividLoad;			//4
	double		m_dCamberUser1;			//5
	double		m_dCamberUser2;			//6
	double		m_dCamberFactory;				//7
	double		m_dCamberField;			//8
	double		m_dCamberSteelRemove;		//9
	double		m_dCamberVividRemove;		//10

	double	m_dCamberConf;	// 구속콘크리트 자중
	double	m_dCamberTurning;		// 180도 터닝하중
};

#endif // !defined(AFX_CAMBER_H__B5B76534_91E7_4C23_9A31_A9A0A60A64C3__INCLUDED_)
