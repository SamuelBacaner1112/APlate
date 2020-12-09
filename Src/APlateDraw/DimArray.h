// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(__DIM_ARRAY_H__)
#define __DIM_ARRAY_H__
 
class CDimData : public CObject
{
public:
	CDimData() 
	{ 		
		m_StaMain		= 0;
		m_StaSub		= 0;
		m_StaCenter		= 0;
		m_StaLeft		= 0;
		m_StaRight		= 0;
		m_pBx = NULL;
		m_nMarkBuje = -1;
	}

public:		
	double	m_StaMain;
	double	m_StaSub;	
	double	m_StaCenter;
	double  m_StaLeft;
	double  m_StaRight;
	CPlateBasicIndex *m_pBx;
	long	m_nMarkBuje;
};

class DimDataArr
{
public:
	DimDataArr(long nType) 
	{ 
		m_nType = nType;
		m_bUseMulti = FALSE;
	}

	DimDataArr() 
	{ 
		m_nType = 0;
		m_bUseMulti = FALSE;
	}
	~DimDataArr() { AhTPADelete(&m_DimData,(CDimData*)0); }
	
	void Add(long nIndex, double StaMain, double StaSub=0, long nMarkBuje=-1) 
	{ 
		CDimData *pData = new CDimData;
		pData->m_StaMain	   = StaMain;
		pData->m_StaCenter     = StaMain;
		pData->m_StaLeft	   = StaMain;
		pData->m_StaRight	   = StaMain;
		pData->m_StaSub		   = StaSub;	
		pData->m_nMarkBuje	   = nMarkBuje;
		m_DimData.Add(pData);
	}
	void Add(long nIndex, double StaMain, double StaCenter, double StaLeft, double StaRight, double StaSub=0, long nMarkBuje=-1) 
	{ 
		CDimData *pData = new CDimData;
		pData->m_StaMain	   = StaMain;
		pData->m_StaCenter     = StaCenter;
		pData->m_StaLeft	   = StaLeft;
		pData->m_StaRight	   = StaRight;
		pData->m_StaSub		   = StaSub;	
		pData->m_nMarkBuje	   = nMarkBuje;
		m_DimData.Add(pData);
	}
	void Add(CPlateBasicIndex *pBx, double StaMain, double StaSub=0, long nMarkBuje=-1) 
	{
		CDimData *pData = new CDimData;
		pData->m_pBx = pBx;
		pData->m_StaMain	   = StaMain;
		pData->m_StaCenter     = StaMain;
		pData->m_StaLeft	   = StaMain;
		pData->m_StaRight	   = StaMain;
		pData->m_StaSub		   = StaSub;	
		pData->m_nMarkBuje	   = nMarkBuje;
		m_DimData.Add(pData);
	}	
	void Add(CPlateBasicIndex *pBx, double StaMain, double StaCenter, double StaLeft, double StaRight, double StaSub=0, long nMarkBuje=-1) 
	{
		CDimData *pData = new CDimData;
		pData->m_pBx = pBx;
		pData->m_StaMain	   = StaMain;
		pData->m_StaCenter     = StaCenter;
		pData->m_StaLeft	   = StaLeft;
		pData->m_StaRight	   = StaRight;
		pData->m_StaSub		   = StaSub;	
		pData->m_nMarkBuje	   = nMarkBuje;
		m_DimData.Add(pData);
	}

	long		GetSize()		{ return m_DimData.GetSize(); }
	CDimData*	GetAt(long i)	{ return m_DimData[i]; }
	long		GetUpperBound() { return GetSize() < 0 ? 0 : GetSize()-1; }
	void		RemoveAll()		{AhTPADelete(&m_DimData,(CDimData*)0);}
	double		GetBxSta(long i, BOOL bSlab=FALSE)
	{ 
		if(m_DimData[i]->m_pBx==NULL) return GetDataSta(i);
		if(!bSlab) return m_DimData[i]->m_pBx->GetStation();
		CPlateBasicIndex *pBx  = m_DimData[i]->m_pBx;
		CPlateGirderApp  *pGir = pBx->GetGirder();
		CPlateBridgeApp  *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
		long   nG    = pBx->GetNumberGirder();
		double Dist  = 0;
		if(nG==0)
			Dist = -pDB->m_dWidthSlabLeft;
		if(nG==pDB->GetGirdersu()-1)
			Dist =  pDB->m_dWidthSlabRight;
		return pGir->GetStationBySc(pBx->GetStation(),Dist,pBx->GetAngle());
	}
	double GetDataSta(long i) 	
	{
		return m_DimData[i]->m_StaMain;
	}
	double GetDataSta(long i, long nDir) 	
	{
		if(nDir==-1) return m_DimData[i]->m_StaLeft;
		if(nDir== 1) return m_DimData[i]->m_StaRight;
		return m_DimData[i]->m_StaCenter;
	}
	double		GetSubSta(long i) 
								{ return m_DimData[i]->m_StaSub; }
	double		GetThickFact(long i)
	{
		double dThickFact = 0;
		if(i<0 || i>=GetSize())	return 0;

		long nMarkBuje = m_DimData[i]->m_nMarkBuje;
		if(nMarkBuje==-1) return 0;

		CPlateBasicIndex *pBx = GetBx(i);

		dThickFact = pBx->GetFactChain(nMarkBuje,TRUE)->m_dFactChainThick;

		return dThickFact;
	} 
 
	double		GetWidthFact(long i)
	{
		double dWidthFact = 0;
		if(i<0 || i>=GetSize())	return 0;

		long nMarkBuje = m_DimData[i]->m_nMarkBuje;
		if(nMarkBuje==-1) return 0;

		CPlateBasicIndex *pBx = GetBx(i);

		dWidthFact = pBx->GetFactChain(nMarkBuje,TRUE)->m_dWidth;

		return dWidthFact;
	} 
	
	CPlateBasicIndex *GetBx(long i)
								{  return m_DimData[i]->m_pBx; }
	void		SetType(long nType)
								{ m_nType = nType; }
	void		SetMulti()		{ m_bUseMulti = TRUE; }

	BOOL		IsMulti()		{ return m_bUseMulti; }

public:
	long	m_nType;
	BOOL	m_bUseMulti;
	CTypedPtrArray <CObArray, CDimData*> m_DimData;
};

#endif