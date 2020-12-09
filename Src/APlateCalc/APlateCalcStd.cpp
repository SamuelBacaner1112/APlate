// APlateCalcStd.cpp: implementation of the CAPlateCalcStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CResultArray::CResultArray()
{
}

CResultArray::~CResultArray()
{
	AhTPADelete(&m_EleResultArr, (CElementResult*)0);
	AhTPADelete(&m_JointResultArr, (CJointResult*)0);
}

void CResultArray::Serialize(CArchive& ar)
{
	long nFlag = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
	}
	else
	{
		ar >> nFlag;
	}
	AhTPASerial(ar, &m_EleResultArr, (CElementResult*)0);
	AhTPASerial(ar, &m_JointResultArr, (CJointResult*)0);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAPlateCalcStd::CAPlateCalcStd(CDataManage *pDataManage)
{
	m_pDataManage		= pDataManage;
	m_pCalcFloor		= new CCalcFloor(pDataManage);
	m_pCalcGeneral		= new CCalcGeneral(pDataManage);
	m_pDesignExpanJoint	= new CDesignExpansionJoint;

	m_nScaleGrade      = 1;
	m_dAlpha           = 1;
	m_bMasterLock		= TRUE;
}

CAPlateCalcStd::~CAPlateCalcStd()
{
	AhTPADelete(&m_HLLFemDataArr, (CFEMManage*)0);
	AhTPADelete(&m_ModelingFemDataArr, (CFEMManage*)0);
	AhTPADelete(&m_WindFemDataArr, (CFEMManage*)0);
	AhTPADelete(&m_InfluenceFemDataArr, (CFEMManage*)0);
	AhTPADelete(&m_SectionStressDataArr, (CFEMManage*)0);
	AhTPADelete(&m_CantileverFemDataArr, (CFEMManage*)0);
	AhTPADelete(&m_ModelingResultArr, (CResultArray*)0);

	delete m_pCalcFloor;
	delete m_pCalcGeneral;
	delete m_pDesignExpanJoint;
}

void CAPlateCalcStd::RemoveHLLFemData()
{
	AhTPADelete(&m_HLLFemDataArr, (CFEMManage*)0);
}

void CAPlateCalcStd::MakeResultArray(long nMake)
{
	AhTPAMake(nMake, &m_ModelingResultArr, (CResultArray*)0);
}

void CAPlateCalcStd::RemoveResultArray()
{
	AhTPADelete(&m_ModelingResultArr, (CResultArray*)0);
}

void CAPlateCalcStd::RemoveModelingFemData()
{
	AhTPADelete(&m_ModelingFemDataArr, (CFEMManage*)0);
	AhTPADelete(&m_ModelingResultArr, (CResultArray*)0);
}

void CAPlateCalcStd::RemoveWindFemData()
{
	AhTPADelete(&m_WindFemDataArr, (CFEMManage*)0);
}

void CAPlateCalcStd::RemoveInfluenceFemData()
{
	AhTPADelete(&m_InfluenceFemDataArr, (CFEMManage*)0);
}

void CAPlateCalcStd::MakeCantileverFemData(long nMake)
{
	AhTPAMake(nMake, &m_CantileverFemDataArr, (CFEMManage*)0);
}

void CAPlateCalcStd::RemoveCantileverFemData()
{
	AhTPADelete(&m_CantileverFemDataArr, (CFEMManage*)0);
}

long CAPlateCalcStd::GetSizeCantileverFemResult()
{
	long nCount = 0;
	long nFrame = m_CantileverFemDataArr.GetSize();
	for(long n=0; n<nFrame; n++)
	{
		if(m_CantileverFemDataArr.GetAt(n)->GetTotalResultSize()>0)
			nCount++;
	}
	return nCount;
}

void CAPlateCalcStd::GetCantileverElementForceOutput(CStringArray& Arr)
{
	CFEMManage* pFEM = GetCantileverFemData(0);

	char sBuf[500];
	Arr.Add ("  << 부 재 력 >>\r\n");

	for (long nTempForce = 0; nTempForce < 3; nTempForce++) 
	{
		long nForce;
		switch (nTempForce)   
		{
		case 0 :
			Arr.Add (" < BENDING MOMENT >");
			nForce = ELE_MOMENT2;
			break;
		case 1 :
			Arr.Add (" < SHEAR FORCE >");
			nForce = ELE_SHEAR_2;
			break;
		case 2 :
			Arr.Add (" < TORSION MOMENT >");
			nForce = ELE_AXTORSN;
			break;
		}
		Arr.Add ("┏━━┳━━┳━━━━━━┳━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━┓");
		Arr.Add ("┃부재┃절점┃   합성전   ┃   합성후   ┃      활  하  중      ┃       풍 하 중       ┃     원 심 하 중      ┃     충 돌 하 중      ┃      합      계      ┃비고┃");
		Arr.Add ("┃번호┃번호┃  고정하중  ┃  고정하중  ┃    Max        Min    ┃    Max        Min    ┃    Max        Min    ┃    Max        Min    ┃    Max        Min    ┃    ┃");
		Arr.Add ("┣━━╋━━╋━━━━━━╋━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━┫");
		// 결과 Data Line 출력
		CString szPreDead, szPreDeadMin, szPostDead, szLiveMax, szLiveMin, szWindMax, szWindMin, 
				szCFMax, szCFMin, szCrashMax, szCrashMin, szTotalMax, szTotalMin, szName;
		CResultForceElement	*pPreDead = NULL, *pPostDead=NULL, *pLiveMax=NULL, *pLiveMin=NULL, *pWindMax=NULL, *pWindMin=NULL, 
							*pCFMax=NULL, *pCFMin=NULL, *pCrashMax=NULL, *pCrashMin=NULL;

		for (long i = 0; i < pFEM->GetElementSize(); i++)  
		{			
			CElement *pEBase = pFEM->GetElement((unsigned short)i);
			if(pEBase->m_szFrameSectionName != "CROSS")
			{
				long nPreDead = pFEM->GetLoadCaseIndex("BEFORE");
				if (nPreDead>-1) pPreDead = pFEM->GetElement((unsigned short)i)->GetResultForce(nPreDead);
				
				long nPostDead	= pFEM->GetLoadCaseIndex("AFTER");
				if (nPostDead>-1) pPostDead = pFEM->GetElement((unsigned short)i)->GetResultForce(nPostDead);
			
				long nLiveMax	= pFEM->GetLoadCaseIndex("LIVE MAX");
				if (nLiveMax>-1) pLiveMax = pFEM->GetElement((unsigned short)i)->GetResultForce(nLiveMax);

				long nLiveMin	= pFEM->GetLoadCaseIndex("LIVE MIN");
				if (nLiveMin>-1) pLiveMin = pFEM->GetElement((unsigned short)i)->GetResultForce(nLiveMin);

				long nWindMax	= pFEM->GetLoadCaseIndex("WIND MAX");
				if (nWindMax>-1) pWindMax = pFEM->GetElement((unsigned short)i)->GetResultForce(nWindMax);
				
				long nWindMin	= pFEM->GetLoadCaseIndex("WIND MIN");
				if (nWindMin>-1) pWindMin = pFEM->GetElement((unsigned short)i)->GetResultForce(nWindMin);		
				
				long nCFMax	= pFEM->GetLoadCaseIndex("CF MAX");
				if (nCFMax>-1) pCFMax = pFEM->GetElement((unsigned short)i)->GetResultForce(nCFMax);
				
				long nCFMin	= pFEM->GetLoadCaseIndex("CF MIN");
				if (nCFMin>-1) pCFMin = pFEM->GetElement((unsigned short)i)->GetResultForce(nCFMin);			
				
				long nCrashMax = pFEM->GetLoadCaseIndex("CRASH MAX");
				if (nCrashMax>-1) pCrashMax = pFEM->GetElement((unsigned short)i)->GetResultForce(nCrashMax);

				long nCrashMin = pFEM->GetLoadCaseIndex("CRASH MIN");
				if (nCrashMin>-1) pCrashMin = pFEM->GetElement((unsigned short)i)->GetResultForce(nCrashMin);
				
				for (long nPoint = 0; nPoint < 2; nPoint++) 
				{
					long nIdx[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
					double dPreDead=0, dPostDead=0, dLiveMax=0, dLiveMin=0, dWindMax=0, dWindMin=0, dCFMax=0, dCFMin=0, dCrashMax=0, dCrashMin=0;
					if (pPreDead && nPoint!=0)	nIdx[0] = pPreDead->m_ResultForceElement.GetUpperBound();
					if (pPostDead && nPoint!=0) nIdx[1] = pPostDead->m_ResultForceElement.GetUpperBound();
					if (pLiveMax && nPoint!=0)	nIdx[2] = pLiveMax->m_ResultForceElement.GetUpperBound();
					if (pLiveMin && nPoint!=0)	nIdx[3] = pLiveMin->m_ResultForceElement.GetUpperBound();
					if (pWindMax && nPoint!=0)	nIdx[4] = pWindMax->m_ResultForceElement.GetUpperBound();
					if (pWindMin && nPoint!=0)	nIdx[5] = pWindMin->m_ResultForceElement.GetUpperBound();
					if (pCFMax && nPoint!=0)	nIdx[6] = pCFMax->m_ResultForceElement.GetUpperBound();
					if (pCFMin && nPoint!=0)	nIdx[7] = pCFMin->m_ResultForceElement.GetUpperBound();
					if (pCrashMax && nPoint!=0) nIdx[8] = pCrashMax->m_ResultForceElement.GetUpperBound();
					if (pCrashMin && nPoint!=0) nIdx[9] = pCrashMin->m_ResultForceElement.GetUpperBound();

					if (pPreDead)	dPreDead	= pPreDead->m_ResultForceElement[nIdx[0]]->m_dForceOnElement[nForce];
					if (pPostDead)	dPostDead	= pPostDead->m_ResultForceElement[nIdx[1]]->m_dForceOnElement[nForce];
					if (pLiveMax)	dLiveMax	= pLiveMax->m_ResultForceElement[nIdx[2]]->m_dForceOnElement[nForce];
					if (pLiveMin)	dLiveMin	= pLiveMin->m_ResultForceElement[nIdx[3]]->m_dForceOnElement[nForce];
					if (pWindMax)	dWindMax	= pWindMax->m_ResultForceElement[nIdx[4]]->m_dForceOnElement[nForce];
					if (pWindMin)	dWindMin	= pWindMin->m_ResultForceElement[nIdx[5]]->m_dForceOnElement[nForce];
					if (pCFMax)		dCFMax		= pCFMax->m_ResultForceElement[nIdx[6]]->m_dForceOnElement[nForce];
					if (pCFMin)		dCFMin		= pCFMin->m_ResultForceElement[nIdx[7]]->m_dForceOnElement[nForce];
					if (pCrashMax)	dCrashMax	= pCrashMax->m_ResultForceElement[nIdx[8]]->m_dForceOnElement[nForce];
					if (pCrashMin)	dCrashMin	= pCrashMin->m_ResultForceElement[nIdx[9]]->m_dForceOnElement[nForce];

					szPreDead.Format("%10.3lf", dPreDead);
					szPostDead.Format("%10.3lf", dPostDead);
					szLiveMax.Format("%10.3lf", dLiveMax);
					szLiveMin.Format("%10.3lf", dLiveMin);
					szWindMax.Format("%10.3lf", dWindMax);
					szWindMin.Format("%10.3lf", dWindMin);
					szCFMax.Format("%10.3lf", dCFMax);
					szCFMin.Format("%10.3lf", dCFMin);
					szCrashMax.Format("%10.3lf", dCrashMax);
					szCrashMin.Format("%10.3lf", dCrashMin);

					double dTotalMax = dPreDead+dPostDead+dLiveMax+dWindMax+dCFMax+dCrashMax;
					double dTotalMin = dPreDead+dPostDead+dLiveMin+dWindMin+dCFMin+dCrashMin;
					szTotalMax.Format("%10.3lf", dTotalMax);
					szTotalMin.Format("%10.3lf", dTotalMin);
					sprintf(sBuf, "┃%4d┃%4d┃ %s ┃ %s ┃%s %s ┃%s %s ┃%s %s ┃%s %s ┃%s %s ┃ %2s ┃", 
						pEBase->m_nIndex+1, pEBase->m_nJStt+1, 
						szPreDead, szPostDead, szLiveMax, szLiveMin, szWindMax, szWindMin, szCFMax, szCFMin, szCrashMax, szCrashMin, szTotalMax, szTotalMin, "");

					Arr.Add(sBuf);
				}
 			}
		}
		Arr.Add ("┗━━┻━━┻━━━━━━┻━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━┛\r\n\r\n");
	}
}

void CAPlateCalcStd::GetCantileverNodeReactionOutput(CStringArray& Arr)
{
	CFEMManage* pFEM = GetCantileverFemData(0);
		
	long nJointSize = pFEM->GetJointSize();
	char sBuf [200];

	Arr.Add ("  << 최대반력집계 >>\r\n");
	// 결과 Data Line 출력
	CString szPreDead, szPostDead, szLiveMax, szLiveMin, szWindMax, szWindMin, 
			szCFMax, szCFMin, szCrashMax, szCrashMin, szTotalMax, szTotalMin;

	for(long nJoint=0; nJoint<nJointSize; nJoint++)
	{
		CJoint* pJoint = pFEM->GetJoint((unsigned short)nJoint);
		if (!pJoint->IsRestraintJoint()) continue;

		Arr.Add ("┏━━┳━━━━━━┳━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━━━━━━━━━━┳━━┓");
		Arr.Add ("┃절점┃   합성전   ┃   합성후   ┃      활  하  중      ┃      풍  하  중      ┃    윈 심  하  중     ┃     충 돌  하 중     ┃      합      계      ┃비고┃");
		Arr.Add ("┃번호┃  고정하중  ┃  고정하중  ┃    Max        Min    ┃    Max        Min    ┃    Max        Min    ┃    Max        Min    ┃    Max        Min    ┃    ┃");
		Arr.Add ("┣━━╋━━━━━━╋━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━━━━━━━━━━╋━━┫");

		double dPreDead=0, dPostDead=0, dLiveMax=0, dLiveMin=0, dWindMax=0, dWindMin=0, dCFMax=0, 
				dCFMin=0, dCrashMax=0, dCrashMin=0, dTotalMax=0, dTotalMin=0;

		long nPreDead = pFEM->GetLoadCaseIndex("BEFORE");
		if (nPreDead>-1) dPreDead = pJoint->GetResultForce(nPreDead)->m_Force.z + pJoint->GetResultForce(nPreDead)->m_SpringForce.z;	
		long nPostDead	= pFEM->GetLoadCaseIndex("AFTER");
		if (nPostDead>-1) dPostDead = pJoint->GetResultForce(nPostDead)->m_Force.z + pJoint->GetResultForce(nPostDead)->m_SpringForce.z;		
		long nLiveMax	= pFEM->GetLoadCaseIndex("LIVE MAX");
		if (nLiveMax>-1) dLiveMax = pJoint->GetResultForce(nLiveMax)->m_Force.z + pJoint->GetResultForce(nLiveMax)->m_SpringForce.z;		
		long nLiveMin	= pFEM->GetLoadCaseIndex("LIVE MIN");
		if (nLiveMin>-1) dLiveMin = pJoint->GetResultForce(nLiveMin)->m_Force.z + pJoint->GetResultForce(nLiveMin)->m_SpringForce.z;		
		long nWindMax	= pFEM->GetLoadCaseIndex("WIND MAX");
		if (nWindMax>-1) dWindMax = pJoint->GetResultForce(nWindMax)->m_Force.z + pJoint->GetResultForce(nWindMax)->m_SpringForce.z;		
		long nWindMin	= pFEM->GetLoadCaseIndex("WIND MIN");
		if (nWindMin>-1) dWindMin = pJoint->GetResultForce(nWindMin)->m_Force.z + pJoint->GetResultForce(nWindMin)->m_SpringForce.z;			
		long nCFMax	= pFEM->GetLoadCaseIndex("CF MAX");
		if (nCFMax>-1) dCFMax = pJoint->GetResultForce(nCFMax)->m_Force.z + pJoint->GetResultForce(nCFMax)->m_SpringForce.z;		
		long nCFMin	= pFEM->GetLoadCaseIndex("CF MIN");
		if (nCFMin>-1) dCFMin = pJoint->GetResultForce(nCFMin)->m_Force.z + pJoint->GetResultForce(nCFMin)->m_SpringForce.z;		
		long nCrashMax = pFEM->GetLoadCaseIndex("CRASH MAX");
		if (nCrashMax>-1) dCrashMax = pJoint->GetResultForce(nCrashMax)->m_Force.z + pJoint->GetResultForce(nCrashMax)->m_SpringForce.z;		
		long nCrashMin = pFEM->GetLoadCaseIndex("CRASH MIN");
		if (nCrashMin>-1) dCrashMin = pJoint->GetResultForce(nCrashMin)->m_Force.z + pJoint->GetResultForce(nCrashMin)->m_SpringForce.z;
		
		dTotalMax = dPreDead+dPostDead+dLiveMax+dWindMax+dCFMax+dCrashMax;
		dTotalMin = dPreDead+dPostDead+dLiveMin+dWindMin+dCFMin+dCrashMin;

		szPreDead.Format("%10.3lf", dPreDead);
		szPostDead.Format("%10.3lf", dPostDead);
		szLiveMax.Format("%10.3lf", dLiveMax);
		szLiveMin.Format("%10.3lf", dLiveMin);
		szWindMax.Format("%10.3lf", dWindMax);
		szWindMin.Format("%10.3lf", dWindMin);
		szCFMax.Format("%10.3lf", dCFMax);
		szCFMin.Format("%10.3lf", dCFMin);
		szCrashMax.Format("%10.3lf", dCrashMax);
		szCrashMin.Format("%10.3lf", dCrashMin);
		szTotalMax.Format("%10.3lf", dTotalMax);
		szTotalMin.Format("%10.3lf", dTotalMin);

		sprintf(sBuf, "┃%4d┃ %s ┃ %s ┃%s %s ┃%s %s ┃%s %s ┃%s %s ┃%s %s ┃    ┃", 
			pJoint->m_nIdx+1, 
			szPreDead, szPostDead, szLiveMax, szLiveMin, szWindMax, szWindMin, szCFMax, szCFMin, szCrashMax, szCrashMin, szTotalMax, szTotalMin);
		
		if(ABS(atof(szTotalMax)) > 0.1)
			Arr.Add(sBuf);

		Arr.Add ("┗━━┻━━━━━━┻━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━━━━━━━━━━┻━━┛\r\n\r\n");
	}	
}

void CAPlateCalcStd::MakeHLLFemData(long nMake)
{
	AhTPAMake(nMake, &m_HLLFemDataArr, (CFEMManage*)0);
}

void CAPlateCalcStd::MakeModelingFemData(long nMake)
{
	AhTPAMake(nMake, &m_ModelingFemDataArr, (CFEMManage*)0);
	AhTPAMake(nMake, &m_ModelingResultArr, (CResultArray*)0);
}

void CAPlateCalcStd::MakeWindFemData(long nMake)
{
	AhTPAMake(nMake, &m_WindFemDataArr, (CFEMManage*)0);
}


void CAPlateCalcStd::MakeInfluenceFemData(long nMake)
{
	AhTPAMake(nMake, &m_InfluenceFemDataArr, (CFEMManage*)0);
}

void CAPlateCalcStd::MakeSectionStressData(long nMake)
{
	AhTPAMake(nMake, &m_SectionStressDataArr, (CFEMManage*)0);
}

void CAPlateCalcStd::RemoveSectionStressData()
{
	AhTPADelete(&m_SectionStressDataArr, (CFEMManage*)0);
}

long CAPlateCalcStd::GetSizeSectionStressResult()
{
	long nCount = 0;
	long nFrame = m_SectionStressDataArr.GetSize();
	for(long n=0; n<nFrame; n++)
	{
		if(m_SectionStressDataArr.GetAt(n)->GetTotalResultSize()>0)
			nCount++;
	}
	return nCount;
}

void CAPlateCalcStd::Serialize(CArchive &ar)
{
	long nVal = 0;
	long nFlag = 4;	// 박병구 APlate2010
	BOOL bVal = 0;
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << nVal;//m_nViewForceType;
		ar << m_nScaleGrade   ;
		ar << m_dAlpha        ;
		ar << bVal;
		ar << bVal;
	}
	else
	{
		ar >> nFlag;
		ar >> nVal;//m_nViewForceType;
		ar >> m_nScaleGrade   ;
		ar >> m_dAlpha        ;
		ar >> bVal;
		ar >> bVal;
	}
	if(nFlag>0)
		m_pDesignExpanJoint->Serialize(ar);
	if(nFlag>2)
	{
		m_szArrModelingFilePath.Serialize(ar);
		AhTPASerial(ar, &m_HLLFemDataArr, (CFEMManage*)0);
		AhTPASerial(ar, &m_ModelingFemDataArr, (CFEMManage*)0);
		AhTPASerial(ar, &m_WindFemDataArr, (CFEMManage*)0);
		AhTPASerial(ar, &m_InfluenceFemDataArr, (CFEMManage*)0);
		AhTPASerial(ar, &m_CantileverFemDataArr, (CFEMManage*)0);
	}
}

void CAPlateCalcStd::Remove3DFrameResultData()
{
	long nFrame = m_ModelingFemDataArr.GetSize();
	for(long n=0; n<nFrame; n++)
		m_ModelingFemDataArr.GetAt(n)->RemoveResultData();

}


long CAPlateCalcStd::GetSizeHLLResult()
{
	long nCount = 0;
	long nFrame = m_HLLFemDataArr.GetSize();
	for(long n=0; n<nFrame; n++)
	{
		if(m_HLLFemDataArr.GetAt(n)->GetTotalResultSize()>0)
			nCount++;
	}
	return nCount;
}

long CAPlateCalcStd::GetSizeModelingResult()
{
	long nCount = 0;
	long nFrame = m_ModelingFemDataArr.GetSize();
	for(long n=0; n<nFrame; n++)
	{
		if(m_ModelingFemDataArr.GetAt(n)->GetTotalResultSize()>0)
			nCount++;
	}
	return nCount;
}

long CAPlateCalcStd::GetSizeWindResult()
{
	long nCount = 0;
	long nFrame = m_WindFemDataArr.GetSize();
	for(long n=0; n<nFrame; n++)
	{
		if(m_WindFemDataArr.GetAt(n)->GetTotalResultSize()>0)
			nCount++;
	}
	return nCount;
}

long CAPlateCalcStd::GetSizeInfluenceResult()
{
	long nCount = 0;
	long nFrame = m_InfluenceFemDataArr.GetSize();
	for(long n=0; n<nFrame; n++)
	{
		if(m_InfluenceFemDataArr.GetAt(n)->GetTotalResultSize()>0)
			nCount++;
	}
	return nCount;
}

CFEMManage *CAPlateCalcStd::GetModelingFemData(long nAnalysisType)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	BOOL	bTuGir	= pDB->IsTUGir();
	long	nFemIdx	= 0;
	switch(nAnalysisType)
	{
	case BEFORE_COMPOSITE_ANALYSIS :
		nFemIdx = 0;
		break;
	case BEFORE_TURN_COMPOSITE_ANALYSIS :
		nFemIdx = 1;
		break;
	case AFTER_COMPOSITE_ANALYSIS :
		nFemIdx = bTuGir ? 2 : 1;
		break;
	case LIVE_LOAD_ANALYSIS :
		nFemIdx = bTuGir ? 3 : 2;
		break;
	case SETTLEMENT_ANALYSIS :
		nFemIdx = bTuGir ? 4 : 3;
		break;
	}
	return m_ModelingFemDataArr.GetAt(nFemIdx);
}

CResultArray *CAPlateCalcStd::GetModelingResult(long nAnalysisType)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	BOOL	bTuGir	= pDB->IsTUGir();
	long	nFemIdx	= 0;
	switch(nAnalysisType)
	{
	case BEFORE_COMPOSITE_ANALYSIS :
		nFemIdx = 0;
		break;
	case BEFORE_TURN_COMPOSITE_ANALYSIS :
		nFemIdx = 1;
		break;
	case AFTER_COMPOSITE_ANALYSIS :
		nFemIdx = bTuGir ? 2 : 1;
		break;
	case LIVE_LOAD_ANALYSIS :
		nFemIdx = bTuGir ? 3 : 2;
		break;
	case SETTLEMENT_ANALYSIS :
		nFemIdx = bTuGir ? 4 : 3;
		break;
	}
	return m_ModelingResultArr.GetAt(nFemIdx);
}

long CAPlateCalcStd::GetAnalysisTypeByIndex(long nIdx)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	BOOL bTuGir = pDB->IsTUGir();
	long nAnalysisType = 0;

	switch(nIdx)
	{
	case 0 :
		nAnalysisType = BEFORE_COMPOSITE_ANALYSIS;
		break;
	case 1 :
		nAnalysisType = bTuGir ? BEFORE_TURN_COMPOSITE_ANALYSIS : AFTER_COMPOSITE_ANALYSIS;
		break;
	case 2 :
		nAnalysisType = bTuGir ? AFTER_COMPOSITE_ANALYSIS : LIVE_LOAD_ANALYSIS;
		break;
	case 3 : 
		nAnalysisType = bTuGir ? LIVE_LOAD_ANALYSIS : SETTLEMENT_ANALYSIS;
		break;
	case 4 :
		nAnalysisType = SETTLEMENT_ANALYSIS;
		break;
	}

	return nAnalysisType;
}

CPlateBasicIndex* CAPlateCalcStd::GetBxByJoint(CJoint *pJ)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(pJ->m_LongAttrib[JOINT_GIR]);
	CPlateBasicIndex	*pBx	= pGir->GetBxByStation(pJ->m_vPointReal.x, BX_CROSSBEAM_VBRACING|BX_SPLICE);

	return pBx;
}
/*
void CAPlateCalcStd::SetExtraDanmyunTotal(CSectionJewon &Mat)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CFEMManage			*pManage	= GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	
	int nSize = m_vExtraDanmyunTotal.size();
	for(int i=0; i<nSize; i++)
	{
		if(Mat == m_vExtraDanmyunTotal[i])
		{
			Mat.m_nIdx = m_vExtraDanmyunTotal[i].m_nIdx;
			return;
		}
	}

	long nMatSttID = 0;
	POSITION pos = pManage->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pManage->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag==POS_GIRDER)
			nMatSttID++;
	}
	Mat.m_nIdx = nMatSttID + m_vExtraDanmyunTotal.size();

	m_vExtraDanmyunTotal.push_back(Mat);
}
*/
void CAPlateCalcStd::SetExtraDanmyun(CSectionJewon &Mat)
{
	CFEMManage			*pManage	= GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	
	int nSize = m_vExtraDanmyun.size();
	for(int i=0; i<nSize; i++)
	{
		if(Mat == m_vExtraDanmyun[i])
		{
			Mat.m_nIdx = m_vExtraDanmyun[i].m_nIdx;
			return;
		}
	}

	long nMatSttID = 0;
	POSITION pos = pManage->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pManage->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag==POS_GIRDER)
			nMatSttID++;
	}
	Mat.m_nIdx = nMatSttID + m_vExtraDanmyun.size();

	m_vExtraDanmyun.push_back(Mat);
}

CCentSeparation* CAPlateCalcStd::GetGuardWall(BOOL bLeft)
{
	CPlateBridgeApp		*pDB			= m_pDataManage->GetBridge();
	CCentSeparation	*pGuard	= NULL;

	long nQtyGuardFence = pDB->GetGuardFencesu();

	for (long nGuard=0; nGuard<nQtyGuardFence; nGuard++)
	{
		pGuard = pDB->GetGuardFence(nGuard);
		if( bLeft == TRUE && (pGuard->m_strName==_T("방호벽(좌)") || pGuard->m_strName==_T("중분대(좌)")))
		{
			break;
		}
		if( bLeft == FALSE && (pGuard->m_strName==_T("방호벽(우)") || pGuard->m_strName==_T("중분대(우)")))
		{
			break;
		}
	}

	return pGuard;
}