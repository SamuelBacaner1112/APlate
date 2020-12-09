// FemStressCalc.cpp: implementation of the CFemStressCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

#define SQ(v)	((v)*(v))
#define maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFemStressCalc::CFemStressCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd) : m_pDataManage(pDataManage)
{
	m_pCalcStd = pCalcStd;
}

CFemStressCalc::~CFemStressCalc()
{

}

double CFemStressCalc::GetOneEleMaxForce(int nForceType, int nLC, int nEleIdx)
{
	double dRatio    = 0;
	double dMax      = 0;
	double dEleForce = 0;

	for(long n=0; n<=4; n++)
	{
		dRatio = n * 0.25;
		dEleForce = GetOneEleForce(nForceType, nLC, nEleIdx, dRatio);

		if(n==0 || dMax<dEleForce)
			dMax = dEleForce;
	}
	return dMax;
}

double CFemStressCalc::GetOneNodForceSub(CFEMManage *pFEM, int nForceType, long nLC, int nNodIdx, char cAxis)
{
	CVector vReturn;
	//long nLC = pFEM->GetLoadCaseIndex((LPCSTR)szLC);
	ASSERT(nLC>=0);
	if(nLC<0) return 0;
	if(nForceType==NOD_DISPLACE)	// 처짐량
	{
		vReturn.x = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Disp.x;
		vReturn.y = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Disp.y;
		vReturn.z = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Disp.z;		
	}
	else if(nForceType==NOD_ROTATE)		// 회전량
	{
		vReturn.x = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Rotate.x;
		vReturn.y = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Rotate.y;
		vReturn.z = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Rotate.z;		
	}	
	else if(nForceType==NOD_REACTION)	// 반력
	{
		vReturn.x = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Force.x;
		vReturn.y = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Force.y;
		vReturn.z = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Force.z;		
	}
	else if(nForceType==NOD_MOMENT)		// 모멘트
	{
		vReturn.x = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Moment.x;
		vReturn.y = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Moment.y;
		vReturn.z = pFEM->GetJoint(nNodIdx)->GetResultForce(nLC)->m_Moment.z;		
	}

	double dRetValue = 0;

	if(cAxis== 'X') dRetValue = vReturn.x;
	if(cAxis== 'Y') dRetValue = vReturn.y;
	if(cAxis== 'Z') dRetValue = vReturn.z;

	if (nForceType == NOD_DISPLACE)
		return dRetValue;
	if (nForceType == NOD_REACTION)
		return dRetValue;
	if (nForceType == NOD_MOMENT)
		return dRetValue;

	return 0;
}

double CFemStressCalc::GetOneNodForce(int nForceType, int nLC, int nNodIdx, char cAxis, BOOL &bExist)
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CCalcData *pData = m_pDataManage->GetCalcData();
	CPlateBridgeApp	*pDB = m_pDataManage->GetBridge();

	double dForce=0;
	if(pDB->IsTUGir())
	{
		if(nLC & LC_PREV_S || nLC & LC_PREV_ST_CON)
		{
			CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);		

			if(nLC & LC_PREV_S)			dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
			if(nLC & LC_PREV_ST_CON)	dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
		}
		if(nLC & LC_PREV_CONF || nLC & LC_PREV_TURN || nLC & LC_PREV_C)
		{
			CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);		

			if(nLC & LC_PREV_CONF)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
			if(nLC & LC_PREV_TURN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
			if(nLC & LC_PREV_C)			dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
		}
	}
	else
	{
		if(nLC & LC_PREV_S || nLC & LC_PREV_C)
		{
			CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);		

			if(nLC & LC_PREV_S)			dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
			if(nLC & LC_PREV_C)			dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
		}
	}

	if(nLC & LC_NEXT)///< 합성후 사하중
	{
		CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);		
		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
	}
	if( nLC & LC_LIVE_MAX	|| nLC & LC_LIVE_MIN   || nLC & LC_DB_MAX      || nLC & LC_DB_MIN   ||
	    nLC & LC_DL_MAX		|| nLC & LC_DL_MIN     || nLC & LC_MTY_MAX     || nLC & LC_MTY_MIN  ||
		nLC & LC_PEOPLE_MAX	|| nLC & LC_PEOPLE_MIN || nLC & LC_WIND_MAX    || nLC & LC_WIND_MIN ||
		nLC & LC_TANK_MAX	|| nLC & LC_TANK_MIN   || nLC & LC_TRAILER_MAX || nLC & LC_TRAILER_MIN )
	{
		CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);	

		if(nLC & LC_LIVE_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
		if(nLC & LC_LIVE_MIN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC &  LC_DB_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC &  LC_DB_MIN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC &  LC_DL_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_DL_MIN)			dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_MTY_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_MTY_MIN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_PEOPLE_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_PEOPLE_MIN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_WIND_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_WIND_MIN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_TANK_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_TANK_MIN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_TRAILER_MAX)	dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
		if(nLC & LC_TRAILER_MIN)	dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);		
	}
	if(nLC & LC_DISP_MAX || nLC & LC_DISP_MIN)///< 지점침하
	{
		if(pData->m_bCheckSettlement)
		{
			CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS);			

			if(nLC & LC_DISP_MAX)	dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
			if(nLC & LC_DISP_MIN)	dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
		}
		else
		{
			dForce = 0;			
		}
	}
	if(nLC & LC_SUM_MAX || nLC & LC_SUM_MIN)
	{
		CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);		

		///< 합계하중은 GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS)에 정의 됨.
		if(nLC & LC_SUM_MAX)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
		if(nLC & LC_SUM_MIN)		dForce += GetOneNodForceSub(pFEM, nForceType, nLC, nNodIdx, cAxis);
	}

	return dForce;
}

double CFemStressCalc::GetOneEleForce(int nForceType, int nLC, int nEleIdx, double dRatio)
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CCalcData		*pData	= m_pDataManage->GetCalcData();
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	long nSeg = 0;
	if	   (0<=dRatio && dRatio<0.25)		nSeg=0;
	else if(0.25<=dRatio && dRatio<0.5)		nSeg=1;
	else if(0.5 <=dRatio && dRatio<0.75)	nSeg=2;
	else if(0.75<=dRatio && dRatio<1)		nSeg=3;
	else if(dRatio==1.0)					nSeg=4;	

	double dForceStt=0;
	double dForceEnd=0;

	if(pDB->IsTUGir())
	{
		if(nLC & LC_PREV_S || nLC & LC_PREV_CONF2 || nLC & LC_PREV_ST_CON)
		{
			CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
			if (pResult->m_EleResultArr.GetSize()<=nEleIdx)	return 0;
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nEleIdx);

			if(nLC & LC_PREV_S)
			{
				dForceStt += pEleResult->m_dPreDeadSteel[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadSteel[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadSteel[nForceType][nSeg+1];
			}
			if(nLC & LC_PREV_CONF2)
			{
				dForceStt += pEleResult->m_dPreDeadConf2[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadConf2[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadConf2[nForceType][nSeg+1];
			}
			if(nLC & LC_PREV_ST_CON)
			{
				dForceStt += pEleResult->m_dPreDeadSteelConf[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadSteelConf[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadSteelConf[nForceType][nSeg+1];
			}
		}
		if(nLC & LC_PREV_CONF || nLC & LC_PREV_TURN || nLC & LC_PREV_C)
		{
			CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS);
			if (pResult->m_EleResultArr.GetSize()<=nEleIdx)	return 0;
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nEleIdx);

			if(nLC & LC_PREV_CONF)
			{
				dForceStt += pEleResult->m_dPreDeadConf[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadConf[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadConf[nForceType][nSeg+1];
			}
			if(nLC & LC_PREV_TURN)
			{
				dForceStt += pEleResult->m_dPreDeadTurning[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadTurning[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadTurning[nForceType][nSeg+1];
			}
			if(nLC & LC_PREV_C)
			{
				dForceStt += pEleResult->m_dPreDeadSlab[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadSlab[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadSlab[nForceType][nSeg+1];
			}
		}
	}
	else
	{
		if(nLC & LC_PREV_S || nLC & LC_PREV_C)
		{
			CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
			if (pResult->m_EleResultArr.GetSize()<=nEleIdx)	return 0;
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nEleIdx);

			if(nLC & LC_PREV_S)
			{
				dForceStt += pEleResult->m_dPreDeadSteel[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadSteel[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadSteel[nForceType][nSeg+1];
			}
			if(nLC & LC_PREV_C)
			{
				dForceStt += pEleResult->m_dPreDeadSlab[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dPreDeadSlab[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dPreDeadSlab[nForceType][nSeg+1];				
			}
		}
	}

	if(nLC & LC_NEXT)///< 합성후 사하중
	{
		CFEMManage *pFEM	= m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
		CResultArray	*pResult	= m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
		if (pResult->m_EleResultArr.GetSize()<=nEleIdx)	return 0;
		CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nEleIdx);

		dForceStt += pEleResult->m_dPostDead[nForceType][nSeg];
		if(nSeg==4)		dForceEnd += pEleResult->m_dPostDead[nForceType][nSeg];
		else			dForceEnd += pEleResult->m_dPostDead[nForceType][nSeg+1];
	}
	if( nLC & LC_LIVE_MAX	|| nLC & LC_LIVE_MIN  || nLC & LC_ONLYLIVE_MAX	|| nLC & LC_ONLYLIVE_MIN ||
		nLC & LC_DB_MAX      || nLC & LC_DB_MIN   ||
	    nLC & LC_DL_MAX		|| nLC & LC_DL_MIN     || nLC & LC_MTY_MAX     || nLC & LC_MTY_MIN  ||
		nLC & LC_PEOPLE_MAX	|| nLC & LC_PEOPLE_MIN || nLC & LC_WIND_MAX    || nLC & LC_WIND_MIN ||
		nLC & LC_TANK_MAX	|| nLC & LC_TANK_MIN   || nLC & LC_TRAILER_MAX || nLC & LC_TRAILER_MIN )
	{
		CResultArray	*pResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
		if (pResult->m_EleResultArr.GetSize()<=nEleIdx)	return 0;
		CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nEleIdx);

		if(nLC & LC_LIVE_MAX)
		{
			dForceStt += pEleResult->m_dLiveMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dLiveMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dLiveMax[nForceType][nSeg+1];
		}
		if(nLC & LC_LIVE_MIN)
		{
			dForceStt += pEleResult->m_dLiveMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dLiveMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dLiveMin[nForceType][nSeg+1];
		}
		if(nLC & LC_ONLYLIVE_MAX)
		{
			dForceStt += pEleResult->m_dOnlyLiveMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dOnlyLiveMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dOnlyLiveMax[nForceType][nSeg+1];
		}
		if(nLC & LC_ONLYLIVE_MIN)
		{
			dForceStt += pEleResult->m_dOnlyLiveMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dOnlyLiveMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dOnlyLiveMin[nForceType][nSeg+1];
		}
		if(nLC &  LC_DB_MAX)
		{
			dForceStt += pEleResult->m_dDBMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dDBMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dDBMax[nForceType][nSeg+1];
		}
		if(nLC &  LC_DB_MIN)
		{
			dForceStt += pEleResult->m_dDBMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dDBMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dDBMin[nForceType][nSeg+1];
		}
		if(nLC &  LC_DL_MAX)
		{
			dForceStt += pEleResult->m_dDLMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dDLMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dDLMax[nForceType][nSeg+1];
		}
		if(nLC & LC_DL_MIN)
		{
			dForceStt += pEleResult->m_dDLMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dDLMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dDLMin[nForceType][nSeg+1];
		}
		if(nLC & LC_MTY_MAX)
		{
			dForceStt += pEleResult->m_dMTYMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dMTYMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dMTYMax[nForceType][nSeg+1];
		}
		if(nLC & LC_MTY_MIN)
		{
			dForceStt += pEleResult->m_dMTYMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dMTYMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dMTYMin[nForceType][nSeg+1];
		}
		if(nLC & LC_PEOPLE_MAX)
		{
			dForceStt += pEleResult->m_dPeopleMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dPeopleMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dPeopleMax[nForceType][nSeg+1];
		}
		if(nLC & LC_PEOPLE_MIN)
		{
			dForceStt += pEleResult->m_dPeopleMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dPeopleMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dPeopleMin[nForceType][nSeg+1];
		}
		if(nLC & LC_WIND_MAX)
		{
			dForceStt += pEleResult->m_dWindMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dWindMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dWindMax[nForceType][nSeg+1];
		}
		if(nLC & LC_WIND_MIN)
		{
			dForceStt += pEleResult->m_dWindMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dWindMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dWindMin[nForceType][nSeg+1];
		}
		if(nLC & LC_TANK_MAX)
		{
			dForceStt += pEleResult->m_dTankMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dTankMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dTankMax[nForceType][nSeg+1];
		}
		if(nLC & LC_TANK_MIN)
		{
			dForceStt += pEleResult->m_dTankMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dTankMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dTankMin[nForceType][nSeg+1];
		}
		if(nLC & LC_TRAILER_MAX)
		{
			dForceStt += pEleResult->m_dTrailerMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dTrailerMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dTrailerMax[nForceType][nSeg+1];
		}
		if(nLC & LC_TRAILER_MIN)
		{
			dForceStt += pEleResult->m_dTrailerMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dTrailerMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dTrailerMin[nForceType][nSeg+1];
		}
	}
	if(nLC & LC_DISP_MAX || nLC & LC_DISP_MIN)///< 지점침하
	{
		if(pData->m_bCheckSettlement)
		{
			CResultArray	*pResult	= m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);
			if (pResult->m_EleResultArr.GetSize()<=nEleIdx)	return 0;
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nEleIdx);

			if(nLC & LC_DISP_MAX)
			{
				dForceStt += pEleResult->m_dUnSettleMax[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dUnSettleMax[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dUnSettleMax[nForceType][nSeg+1];
			}
			if(nLC & LC_DISP_MIN)
			{
				dForceStt += pEleResult->m_dUnSettleMin[nForceType][nSeg];
				if(nSeg==4)		dForceEnd += pEleResult->m_dUnSettleMin[nForceType][nSeg];
				else			dForceEnd += pEleResult->m_dUnSettleMin[nForceType][nSeg+1];
			}
		}
		else
		{
			dForceStt = 0;
			dForceEnd = 0;
		}
	}
	if(nLC & LC_SUM_MAX || nLC & LC_SUM_MIN)
	{
		CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
		if (pResult->m_EleResultArr.GetSize()<=nEleIdx)	return 0;
		CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nEleIdx);

		if(nLC & LC_SUM_MAX)  ///< 합계하중은 GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS)에 정의 됨.
		{
			dForceStt += pEleResult->m_dTotalMax[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dTotalMax[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dTotalMax[nForceType][nSeg+1];
		}
		if(nLC & LC_SUM_MIN)
		{
			dForceStt += pEleResult->m_dTotalMin[nForceType][nSeg];
			if(nSeg==4)		dForceEnd += pEleResult->m_dTotalMin[nForceType][nSeg];
			else			dForceEnd += pEleResult->m_dTotalMin[nForceType][nSeg+1];
		}
	}

	double dLengthElem    = ModelCalc.GetElemLength(nEleIdx);
	double dLengthFromStt = dRatio*dLengthElem;
	double dLengthSeg     = dLengthElem / 4;

	return dForceStt + (dForceEnd-dForceStt)/dLengthSeg * (dLengthFromStt-nSeg*dLengthSeg);
}

 
///////////////////////////////////////////////////////////////////////////////////////////
// 목적 : SAP의 부재 결과 Data중 원하는 값(kgf, MM)을 구함.
// 입력 :
//	  nForceType ==  ELE_SHEAR_2   Shear 1-2   
//	             ==  ELE_MOMENT2   Moment 1-2
//               ==  ELE_AXFORCE   Axial Force
//               ==  ELE_SHEAR_3   Shear 1-3
//               ==  ELE_MOMENT3   Moment 1-3
//               ==  ELE_AXTORSN   Axial Torsion
//
//  LC_PREV_S			0   // 합성전 사하중 (Steel)
//  LC_PREV_C			1   // 합성전 사하중 (Concrete)
//  LC_NEXT				2   // 합성후 사하중
//  LC_LIVE_MAX			3   // 활하중 Max
//  LC_LIVE_MIN			4   // 활하중 Min
//  LC_DISP_MAX			5   // 지점 침하 Max
//  LC_DISP_MIN			6   // 지점 침하 Min
//  LC_SUM_MAX			7   // 합계 Max
//  LC_SUM_MIN			8   // 합계 Min

//   nResultType ==  0   지정된 값. (Girder와 Basic Index가 지정되어야 함.)
//               ==  1   최대값. (+로 제일 큰값)
//               ==  2   최소값. (-로 제일 큰값)
//               ==  3   최대 절대값. (절대값으로 제일 큰값. 단, 부호는 그대로 리턴됨.)
//               ==  4   합계 Max와 Min중 절대값이 큰값. (단, 부호는 그대로 리턴됨.)
//                       (이 경우는 LoadCase가 무시되고 무조건 합계 Max Min값으로 계산.)
//               ==  5   합계 Max값. (단, 부호는 그대로 리턴됨.)
//               ==  6   합계 Min값. (단, 부호는 그대로 리턴됨.)
/////////////////////////////////////////////////////////////////////////////////////////////
double CFemStressCalc::GetElementForce(int nForceType, int nLC, long &nG, double &dSta, int nResultType)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	double dResult = 0.0;
	BOOL bFirst = TRUE;
	int nTempGir;

	if (dSta != -9999999999999.0)   // 지정된 위치임.
		return GetElementForceTerm(nForceType, nLC, nG, dSta, dSta, nResultType);

	for (long nCurGir = 0; nCurGir < pDB->GetGirdersu(); nCurGir++)   
	{
		if (nG != -1 && nG != nCurGir)   // 해당 거더가 아니면
			continue;
		CPlateGirderApp* pGir = pDB->GetGirder(nCurGir);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);

		double dStaStt = pBxStt->GetStation();
		double dStaEnd = pBxEnd->GetStation();
		double dValue = GetElementForceTerm(nForceType, nLC, nCurGir, dStaStt, dStaEnd, nResultType);
		BOOL bUpdate = FALSE;
		switch (nResultType)
		{
		case 1 :
			if (dValue > dResult)
				bUpdate = TRUE;
			break;
		case 2 :
			if (dValue < dResult)
				bUpdate = TRUE;
			break;
		case 0 :
		case 3 :
		case 4 :
			if (fabs(dValue) > fabs(dResult))
				bUpdate = TRUE;
			break;
		}
		if (bFirst || bUpdate)
		{
			bFirst = FALSE;
			nTempGir = nCurGir;
			dSta = dStaStt;
			dResult = dValue;
		}
	}
	nG = nTempGir;
	
	return dResult;

}
///////////////////////////////////////////////////////////////////////////
// 목적 : SAP의 부재 결과 Data중 원하는 값(kgf, MM)을 구함.
// 입력 :
//	  nForceType ==  ELE_SHEAR_2   Shear 1-2    (ForceType.h에 정의되어 있음.)
//	             ==  ELE_MOMENT2   Moment 1-2
//               ==  ELE_AXFORCE   Axial Force
//               ==  ELE_SHEAR_3   Shear 1-3
//               ==  ELE_MOMENT3   Moment 1-3
//               ==  ELE_AXTORSN   Axial Torsion
//
//           nLC ==  LC_PREV_S     합성전 사하중 (Steel)    (SAPData.h에 정의되어 있음.)
//               ==  LC_PREV_C     합성전 사하중 (Concrete)
//               ==  LC_NEXT       합성후 사하중
//               ==  LC_LIVE_MAX   활하중 Max
//               ==  LC_LIVE_MIN   활하중 Min
//               ==  LC_DISP_MAX   지점 침하 Max
//               ==  LC_DISP_MIN   지점 침하 Min
//               ==  LC_SUM_MAX    합계 Max 
//               ==  LC_SUM_MIN    합계 Min 
//
//           &nG == -1   모든 거더에서 찾음. (나중에 해당 거더번호 리턴됨.)
//               >=  0   지정된 거더에서 찾음.
//
//       &nIndex == -1   해당 거더내의 모든 Basic Index에서 찾음. (나중에 해당 Basic Index 리턴됨.)
//               >=  0   지정된 Basic Index에서 찾음.
//
//   nResultType ==  0   지정된 값. (Girder와 Basic Index가 지정되어야 함.)
//               ==  1   최대값. (+로 제일 큰값)
//               ==  2   최소값. (-로 제일 큰값)
//               ==  3   최대 절대값. (절대값으로 제일 큰값. 단, 부호는 그대로 리턴됨.)
//               ==  4   합계 Max와 Min중 절대값이 큰값. (단, 부호는 그대로 리턴됨.)
//                       (이 경우는 LoadCase가 무시되고 무조건 합계 Max Min값으로 계산.)
//               ==  5   합계 Max값. (단, 부호는 그대로 리턴됨.)
//               ==  6   합계 Min값. (단, 부호는 그대로 리턴됨.)
///////////////////////////////////////////////////////////////////////////
double CFemStressCalc::GetElementForceTerm(int nForceType, int nLC, long nG, double &dStaStt, double dStaEnd, int nResultType)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateBxFinder Finder(pDB->GetGirder(nG));
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);

	double dValue = 0, dResult = 0.0, dMaxRatio = 0;
	double dRatioStt = 0, dRatioEnd = 0;
	long nEleStt = 0, nEleEnd = 0, nMaxEle = 0;
	BOOL bFirst = TRUE;

	nEleStt = ModelCalc.GetElemNoAndRatio(nG, dStaStt, dRatioStt);
	///< nEleStt이 거더의 시작요소 일때
	long nStt=0, nEnd=0;
	BOOL bGirSttEle = FALSE;
	for(long n=0; n<pDB->GetGirdersu(); n++)
	{
		ModelCalc.GetElemRangeByGirder(n, nStt, nEnd);
		if(nEleStt == nStt)		bGirSttEle = TRUE;
	}
	///< *거더의 시작요소인 경우는 제외되고 요소의 시점부분일 경우
	///<  이 위치에는 맞물린 Element 결과값 존재
	if(dRatioStt == 0.0 && !bGirSttEle)
	{  
		double dValue1, dValue2, IValue, JValue;
		dValue1 = GetOneEleForce(nForceType, LC_SUM_MAX, nEleStt, 0.0);
		dValue2 = GetOneEleForce(nForceType, LC_SUM_MIN, nEleStt, 0.0);
		IValue  = maxabs(dValue1, dValue2);
		dValue1 = GetOneEleForce(nForceType, LC_SUM_MAX, nEleStt-1, 1.0);
		dValue2 = GetOneEleForce(nForceType, LC_SUM_MIN, nEleStt-1, 1.0);
		JValue  = maxabs(dValue1, dValue2);
		///< 요소의 시작점을 기준으로 전요소의 끝점과 현재요소의 시점의 값을 비교하여 
		///< 전요소의 끝점값이 클 경우는 요소번호를 뒤로 빼고 dRatio를 1로 함.
		if(fabs(JValue) > fabs(IValue))   
		{
			nEleStt--;
			dRatioStt = 1.0;
		}
	}
	///< 시작스테이션과 끝스테이션이 반대로 되있으면 끝스테이션을 시작스테이션으로 함
	if(dStaStt < dStaEnd)
		nEleEnd = ModelCalc.GetElemNoAndRatio(nG, dStaEnd, dRatioEnd);
	else
	{
		nEleEnd   = nEleStt;
		dRatioEnd = dRatioStt;
	}

	for(long nElm=nEleStt; nElm<= nEleEnd; nElm++)
	{
		for(long nPoint = 0; nPoint < MAXMIN_POINT; nPoint++)  
		{
			double dRatio = (double)nPoint / (MAXMIN_POINT-1);
			if (nElm == nEleStt && nPoint == 0)  ///< 시작 스테이션 위치
			{
				dRatio = dRatioStt;
				nPoint = (long)(dRatio*(MAXMIN_POINT-1));
			}
			if (nElm == nEleEnd && dRatio >= dRatioEnd)
			{
				dRatio = dRatioEnd;
				nPoint = MAXMIN_POINT - 1;  // 종료조건
			}
			dValue = 0.0;
			BOOL bUpdate = FALSE;
			if(nPoint < 0) return 0.0; 
			if(nResultType < 4)
			{
				// 일반 Load Case 순환 루틴.
//				for (int nCurLC = LC_PREV_S; nCurLC <= LC_TRAILER_MIN; nCurLC++) 
//				{
//					if(nLC != nCurLC)   // 해당 Load Case가 아니면
//						continue;
//
//					dValue += GetOneEleForce(nForceType, nCurLC, nElm, dRatio);
//				}
				dValue += GetOneEleForce(nForceType, nLC, nElm, dRatio);
				if (bFirst)  
				{
					bUpdate = TRUE;
					bFirst  = FALSE;
				}
				switch (nResultType) 
				{
				case 1 :
					if (dValue > dResult)
						bUpdate = TRUE;
					break;
				case 2 :
					if (dValue < dResult)
						bUpdate = TRUE;
					break;
				case 0 :
				case 3 :
					if (fabs(dValue) > fabs(dResult))
						bUpdate = TRUE;
					break;
				}
			}
			else	// nLC가 사용안됨
			{
				double dSumMax = GetOneEleForce(nForceType, LC_SUM_MAX, nElm, dRatio);
				double dSumMin = GetOneEleForce(nForceType, LC_SUM_MIN, nElm, dRatio);
				double dMax = max(dSumMax, dSumMin);
				double dMin = min(dSumMax, dSumMin);
				//시종점에서 부모멘트로 저장되는 것을 정모멘트 저장하는 걸로 변경
				if(nForceType==ELE_MOMENT3 && ((pBxStt->GetStation()==dStaStt && dRatio==0) || (pBxEnd->GetStation()==dStaEnd && dRatio==1)))
				{
					dMax = 0;
					dMin = 0;
				}

				if	   (nResultType==4)	//절대값 최대 모멘트
					dValue = maxabs(dMax, dMin);
				else if(nResultType==5)	//최대 정모멘트
				{
					if(dMax>0)		dValue = dMax;
//					else			dValue = -999;
				}
				else if(nResultType==6)	//최대 부모멘트
				{
					if(dMin<0)		dValue = dMin;
//					else			dValue = -999;
				}
				
				if (fabs(dValue) >= fabs(dResult))
					bUpdate = TRUE;
			}
			if (bUpdate) 
			{
				dResult   = dValue;
				nMaxEle   = nElm;
				dMaxRatio = dRatio;
			}
		}
	}
	if(dStaStt < dStaEnd)
		dStaStt = ModelCalc.GetStationByElmAndRatio(nMaxEle, dMaxRatio);

	return dResult;
}

double CFemStressCalc::GetMaxStressRatio(long nG, double dSta, double &Max_up, double &Max_lo, double &Comp_up, double &Comp_lo, double &Tau, double dStaSec, long nPosSec)
{
	double Max_upA=0, Max_loA=0, Tau_up=0, Tau_lo=0, Tau_upA=0, Tau_loA=0;
	///< 상,하부 응력값 및 허용응력값 구하기
	GetFlangeStress(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA, dStaSec, nPosSec);

	Comp_up = SQ (Max_up / Max_upA) + SQ (Tau_up / Tau_upA);
	Comp_lo = SQ (Max_lo / Max_loA) + SQ (Tau_lo / Tau_loA);

	Tau = Tau_up;
	if (Comp_up == 0.0 && Comp_lo == 0.0)
		return 0.0;
	
	double Ratio = sqrt (1.2 / max (Comp_up, Comp_lo));

	double dRatio_Up	= min(Ratio, fabs (Max_upA / Max_up));
	double dRatio_lo	= min(Ratio, fabs (Max_loA / Max_lo));
	double dRatio_min	= min(dRatio_Up, dRatio_lo);
	
	return dRatio_min;
}

void CFemStressCalc::GetFlangeStress(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec, long nPosSec)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	if(pDB->IsTUGir())	GetFlangeStress_TUGirder(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA, dStaSec, nPosSec);
	else				GetFlangeStress_GenGirder(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA, dStaSec, nPosSec);
}

void CFemStressCalc::GetFlangeStress_TUGirder(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec, long nPosSec)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx     = pGir->GetBxByStationDir(dSta, 0);
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CSteel           *pSteelCtl  = m_pDataManage->GetSteel();
	CCalcGeneral	CalcGen(m_pDataManage);

	GetSectionStress(nG, dSta, TRUE, dStaSec, nPosSec);

	BOOL	bWind		= pData->m_bWind;
	BOOL	bBuckling	= m_pDataManage->GetCalcData()->m_bBucklingStress;
	double	dSigmaCA	= 0;
	double	dSigmaTA	= 0;
	double	dSigmaTurn	= 0;
// 	double	dTu			= pGir->GetThickFactByStation(dSta, G_F_U);
// 	double	dTl			= pGir->GetThickFactByStation(dSta, G_F_L);
// 	double	dTw			= pGir->GetThickFactByStation(dSta, G_W);
	BOOL	bPLUS_M		= IsPositive(ELE_MOMENT3, nG, dSta);
	CString	strMatLeft	= _T("");
	CString	strMatRight	= _T("");

	if(dStaSec!=0)	ModelCalc.GetSectionCoefficient(nG, dStaSec, FALSE, 0, nPosSec);
	else			ModelCalc.GetSectionCoefficient(nG, dSta);
	double	dUpFlangeW	= ModelCalc.GetMapValueSecCo("FLANGE_W_UP");
	double	dUpFlangeT	= ModelCalc.GetMapValueSecCo("FLANGE_T_UP");
	double	dWebT		= ModelCalc.GetMapValueSecCo("WEB_T");
	double	dDnFlangeW	= ModelCalc.GetMapValueSecCo("FLANGE_W_DN");
	double	dDnFlangeT	= ModelCalc.GetMapValueSecCo("FLANGE_T_DN");
	double	dWebH		= ModelCalc.GetMapValueSecCo("WEB_H");
	double	dTu			= dUpFlangeT;
	double	dTl			= dDnFlangeT;
	double	dTw			= dWebH;
	double	dAw			= (dWebH-dUpFlangeT-dDnFlangeT)*dWebT;
	double	dAc			= dDnFlangeW*dDnFlangeT;
	double	dDist		= pDB->m_dLengthBaseCrossBeam;
	double	dWidth		= (dUpFlangeW-dWebT)/2;//(pGir->GetWidthOnStation(dSta, bPLUS_M) - pGir->GetThickFactByStation(dSta, G_W))/2;
	CHGBaseSteel	*pSteel  = NULL;
	if(bPLUS_M)  ///< 정모멘트
	{
		CalcGen.GetMaterial(pBx, G_F_L, strMatLeft, strMatRight);

		pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		CHGAllowStress	AllowTension(pSteel);
		CHGAllowStress	AllowBuckling(pSteel);
		CHGAllowStress	AllowBendingStress(pSteel);			
		AllowBuckling.SetAllowBuckling(dTu, dWidth);
		AllowBendingStress.SetAllowBendingStress(dAw, dAc, dDist, dDnFlangeW);		
		
		dSigmaTA	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dTl);
		CalcGen.GetMaterial(pBx, G_F_U, strMatLeft, strMatRight);
		pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		AllowBendingStress.SetBaseSteel(pSteel);
		AllowBuckling.SetBaseSteel(pSteel);

		if(bBuckling)	dSigmaCA	= AllowBendingStress.GetAllowStress(bBuckling ? ALLOW_BENDINGCOMP_CASE1 : ALLOW_BENDINGCOMP_CASE2, dTu);
		else			dSigmaCA	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dTu);
	}
	else		///< 부모멘트
	{
		CalcGen.GetMaterial(pBx, G_F_U, strMatLeft, strMatRight);
		pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		CHGAllowStress	AllowTension(pSteel);
		CHGAllowStress	AllowBuckling(pSteel);				
		CHGAllowStress	AllowBendingStress(pSteel);	
		AllowBuckling.SetAllowBuckling(dTl, dWidth);
		AllowBendingStress.SetAllowBendingStress(dAw, dAc, dDist, dDnFlangeW);		
		

		dSigmaTA	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dTu);
		CalcGen.GetMaterial(pBx, G_F_L, strMatLeft, strMatRight);
		pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		AllowBuckling.SetBaseSteel(pSteel);
		AllowBendingStress.SetBaseSteel(pSteel);

		dSigmaTurn	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dTl);
		dSigmaCA	= min(AllowBendingStress.GetAllowStress(ALLOW_BENDINGCOMP_CASE2, dTl), AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dTl));		
	}

	m_MapSecStress["SigmaCA"]	= dSigmaCA;
	m_MapSecStress["SigmaTA"]	= dSigmaTA;
	CalcGen.GetMaterial(pBx, G_W, strMatLeft, strMatRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
	CHGAllowStress	AllowTension(pSteel);

	double	dFu[20]		= {0,};	// 상연 작용응력
	double	dFl[20]		= {0,};	// 하연 작용응력
	double	dFua[20]	= {0,};	// 상연 허용응력
	double	dFla[20]	= {0,};	// 하연 허용응력
	double	dTau[20]	= {0,};
	double	dTauA[20]	= {0,};

	dFu[0]	= m_MapSecStress["Fsu1"];				dFl[0]	= m_MapSecStress["Fsl1"];			// 강재자중
	dFu[1]	= dFu[0] + m_MapSecStress["Fsu2"];		dFl[1]	= dFl[0] + m_MapSecStress["Fsl2"];	// 구속콘크리트
	dFu[2]	= dFu[1] + m_MapSecStress["Fsu3"];		dFl[2]	= dFl[1] + m_MapSecStress["Fsl3"];	// 180도 터닝
	if(bPLUS_M)
	{
		dFu[3]	= dFu[2] + m_MapSecStress["Fsu_cc"];	dFl[3]	= dFl[2] + m_MapSecStress["Fsl_cc"];// 구속큰크리트 크리프
		dFu[4]	= dFu[3] + m_MapSecStress["Fsu_cs"];	dFl[4]	= dFl[3] + m_MapSecStress["Fsl_cs"];// 구속콘크리트 건조수축
		dFu[5]	= dFu[4] + m_MapSecStress["Fsu4"];		dFl[5]	= dFl[4] + m_MapSecStress["Fsl4"];	// 바닥판 자중
		dFu[6]	= dFu[5] + m_MapSecStress["Fsu5"];		dFl[6]	= dFl[5] + m_MapSecStress["Fsl5"];	// 2차 고정하중
		dFu[7]	= dFu[6] + m_MapSecStress["Fsu6"];		dFl[7]	= dFl[6] + m_MapSecStress["Fsl6"];	// 활하중
		dFu[8]	= dFu[7] + m_MapSecStress["Fsu_dc"];	dFl[8]	= dFl[7] + m_MapSecStress["Fsl_dc"];// 바닥판 크리프
		dFu[9]	= dFu[8] + m_MapSecStress["Fsu_ds"];	dFl[9]	= dFl[8] + m_MapSecStress["Fsl_ds"];// 바닥판 건조수축
		dFu[10]	= dFu[9] + m_MapSecStress["Fsu_dt"];	dFl[10]	= dFl[9] + m_MapSecStress["Fsl_dt"];// +바닥판 온도차
		dFu[11]	= dFu[9] - m_MapSecStress["Fsu_dt"];	dFl[11]	= dFl[9] - m_MapSecStress["Fsl_dt"];// -바닥판 온도차
		if(bWind)
		{
			dFu[12]	= dFu[4] + m_MapSecStress["Fsuw"];		dFl[12]	= dFl[4] + m_MapSecStress["Fslw"];		// 2차 고정하중(풍)
			dFu[13]	= dFu[5] + 0.5*m_MapSecStress["Fsuw"];	dFl[13]	= dFl[5] + 0.5*m_MapSecStress["Fslw"];	// 활하중(풍)
			dFu[14]	= dFu[13] + m_MapSecStress["Fsu_cc"];	dFl[14]	= dFl[13] + m_MapSecStress["Fsl_cc"];	// 구속콘크리트 크리프(풍)
			dFu[15]	= dFu[14] + m_MapSecStress["Fsu_cs"];	dFl[15]	= dFl[14] + m_MapSecStress["Fsl_cs"];	// 구속콘크리트 건조수축(풍)
			dFu[16]	= dFu[15] + m_MapSecStress["Fsu_dc"];	dFl[16]	= dFl[15] + m_MapSecStress["Fsl_dc"];	// 바닥판 크리프(풍)
			dFu[17] = dFu[16] + m_MapSecStress["Fsu_ds"];	dFl[17] = dFl[16] + m_MapSecStress["Fsl_ds"];	// 바닥판 건조수축(풍)
			dFu[18] = dFu[17] + m_MapSecStress["Fsu_dt"];	dFl[18] = dFl[17] + m_MapSecStress["Fsl_dt"];	// +바닥판 온도차(풍)
			dFu[19] = dFu[17] - m_MapSecStress["Fsu_dt"];	dFl[19] = dFl[17] - m_MapSecStress["Fsl_dt"];	// -바닥판 온도차(풍)
		}
	}
	else
	{
		dFu[3] = dFu[2] + m_MapSecStress["Fvsu_Deck"];		dFl[3] = dFl[2] + m_MapSecStress["Fvsl_Deck"];
		dFu[4] = dFu[3] + m_MapSecStress["Fvsu_Live"];		dFl[4] = dFl[3] + m_MapSecStress["Fvsl_Live"];
		dFu[5] = dFu[4] + m_MapSecStress["Fsu_cc"];			dFl[5] = dFl[4] + m_MapSecStress["Fsl_cc"];
		dFu[6] = dFu[5] + m_MapSecStress["Fsu_cs"];			dFl[6] = dFl[5] + m_MapSecStress["Fsl_cs"];
		dFu[7] = dFu[6] + m_MapSecStress["Fsu_dc"];			dFl[7] = dFl[6] + m_MapSecStress["Fsl_dc"];
		dFu[8] = dFu[7] + m_MapSecStress["Fsu_ds"];			dFl[8] = dFl[7] + m_MapSecStress["Fsl_ds"];
		dFu[9] = dFu[8] + m_MapSecStress["Fsu_dt"];			dFl[9] = dFl[8] + m_MapSecStress["Fsl_dt"];
		dFu[10] = dFu[8] - m_MapSecStress["Fsu_dt"];		dFl[10] = dFl[8] - m_MapSecStress["Fsl_dt"];
		if(bWind)
		{
			dFu[11]	= dFu[3] + m_MapSecStress["Fsuw"];		dFl[11] = dFl[3] + m_MapSecStress["Fslw"];
			dFu[12]	= dFu[4] + 0.5*m_MapSecStress["Fsuw"];	dFl[12] = dFl[4] + m_MapSecStress["Fslw"];
			dFu[13]	= dFu[12] + m_MapSecStress["Fsu_cc"];	dFl[13] = dFl[12] + m_MapSecStress["Fsu_cc"];
			dFu[14] = dFu[13] + m_MapSecStress["Fsu_cs"];	dFl[14] = dFl[13] + m_MapSecStress["Fsl_cs"];
			dFu[15] = dFu[14] + m_MapSecStress["Fsu_dc"];	dFl[15] = dFl[14] + m_MapSecStress["Fsl_dc"];
			dFu[16] = dFu[15] + m_MapSecStress["Fsu_ds"];	dFl[16] = dFl[15] + m_MapSecStress["Fsl_ds"];
			dFu[17] = dFu[16] + m_MapSecStress["Fsu_dt"];	dFl[17] = dFl[16] + m_MapSecStress["Fsl_dt"];
			dFu[18] = dFu[17] - m_MapSecStress["Fsu_dt"];	dFl[18] = dFl[17] - m_MapSecStress["Fsl_dt"];
		}
	}

	if(bPLUS_M)
	{
		dFua[0]		= dSigmaCA;				dFla[0]		= dSigmaTA;
		dFua[1]		= dSigmaCA;				dFla[1]		= dSigmaTA;
		dFua[2]		= dSigmaCA;				dFla[2]		= dSigmaTA;
		dFua[3]		= dSigmaCA*1.15;		dFla[3]		= dSigmaTA;
		dFua[4]		= dSigmaCA*1.15;		dFla[4]		= dSigmaTA;
		dFua[5]		= dSigmaCA;				dFla[5]		= dSigmaTA;
		dFua[6]		= dSigmaCA;				dFla[6]		= dSigmaTA;
		dFua[7]		= dSigmaCA;				dFla[7]		= dSigmaTA;
		dFua[8]		= dSigmaCA*1.15;		dFla[8]		= dSigmaTA;
		dFua[9]		= dSigmaCA*1.15;		dFla[9]		= dSigmaTA;
		dFua[10]	= dSigmaCA*1.30;		dFla[10]	= dSigmaTA*1.15;
		dFua[11]	= dSigmaCA*1.30;		dFla[11]	= dSigmaTA*1.15;

		dFua[12]	= dSigmaCA*1.25;		dFla[12]	= dSigmaTA*1.25;
		dFua[13]	= dSigmaCA*1.25;		dFla[13]	= dSigmaTA*1.25;
		dFua[14]	= dSigmaCA*1.25;		dFla[14]	= dSigmaTA*1.25;
		dFua[15]	= dSigmaCA*1.25;		dFla[15]	= dSigmaTA*1.25;
		dFua[16]	= dSigmaCA*1.25;		dFla[16]	= dSigmaTA*1.25;
		dFua[17]	= dSigmaCA*1.25;		dFla[17]	= dSigmaTA*1.25; 
		dFua[18]	= dSigmaCA*1.35;		dFla[18]	= dSigmaTA*1.35;
		dFua[19]	= dSigmaCA*1.35;		dFla[19]	= dSigmaTA*1.35;
	}
	else
	{
		// 2008.09.30 부모멘트 허용응력 자료 리스트에서 미수령(심동윤)
		dFua[0]	= dSigmaTA;		dFla[0]	= dSigmaCA;
		dFua[1]	= dSigmaTurn;	dFla[1]	= dSigmaTA;
		dFua[2]	= dSigmaTurn;	dFla[2]	= dSigmaTA;
		dFua[3]	= dSigmaTurn;	dFla[3]	= dSigmaTA;
		dFua[4]	= dSigmaTurn;	dFla[4]	= dSigmaTA;
		dFua[5]	= dSigmaTurn;	dFla[5]	= dSigmaTA;

		dFua[6]		= dSigmaTurn*1.15;		dFla[6]		= dSigmaTA;
		dFua[7]		= dSigmaTurn*1.15;		dFla[7]		= dSigmaTA;
		dFua[8]		= dSigmaTurn*1.15;		dFla[8]		= dSigmaTA;
		dFua[9]		= dSigmaTurn*1.30;		dFla[9]		= dSigmaTA*1.15;
		dFua[10]	= dSigmaTurn*1.30;		dFla[10]	= dSigmaTA*1.15;
		dFua[11]	= dSigmaTurn*1.20;		dFla[11]	= dSigmaTA*1.20;
		dFua[12]	= dSigmaTurn*1.25;		dFla[12]	= dSigmaTA*1.25;
		dFua[13]	= dSigmaTurn*1.25;		dFla[13]	= dSigmaTA*1.25;
		dFua[14]	= dSigmaTurn*1.25;		dFla[14]	= dSigmaTA*1.25;
		dFua[15]	= dSigmaTurn*1.25;		dFla[15]	= dSigmaTA*1.25;
		dFua[16]	= dSigmaTurn*1.25;		dFla[16]	= dSigmaTA*1.25;
		dFua[17]	= dSigmaTurn*1.35;		dFla[17]	= dSigmaTA*1.35;
		dFua[18]	= dSigmaTurn*1.35;		dFla[18]	= dSigmaTA*1.35;
	}
	dTau[0]	= m_MapSecStress["Tau_shear"];	dTauA[0] = AllowTension.GetAllowStress(ALLOW_SHEARSTRESS, dTw);// 강재자중
	dTau[1]	= dTau[0];						dTauA[1] = dTauA[0];
	dTau[2]	= dTau[0];						dTauA[2] = dTauA[0];
	dTau[3]	= dTau[0];						dTauA[3] = dTauA[0];
	dTau[4]	= dTau[0];						dTauA[4] = dTauA[0];
	dTau[5]	= dTau[0];						dTauA[5] = dTauA[0];
	dTau[6]	= dTau[0];						dTauA[6] = dTauA[0];
	dTau[7]	= dTau[0];						dTauA[7] = dTauA[0];
	dTau[8]	= dTau[0];						dTauA[8] = dTauA[0];
	dTau[9]	= dTau[0];						dTauA[9] = dTauA[0];
	dTau[10] = dTau[0];						dTauA[10] = dTauA[0];
	dTau[11] = dTau[0];						dTauA[11] = dTauA[0];

	if(bWind)
	{
		dTau[12]	= m_MapSecStress["Tau_shear_w"];	dTauA[12] = dTauA[0] * 1.2;
		dTau[13]	= m_MapSecStress["Tau_shear_wl"];	dTauA[13] = dTauA[0] * 1.25;
		dTau[14]	= dTau[13];							dTauA[14] = dTauA[0] * 1.25;
		dTau[15]	= dTau[13];							dTauA[15] = dTauA[0] * 1.25;
		dTau[16]	= dTau[13];							dTauA[16] = dTauA[0] * 1.25;
		dTau[17]	= dTau[13];							dTauA[17] = dTauA[0] * 1.25;
		dTau[18]	= dTau[13];							dTauA[18] = dTauA[0] * 1.25;
		dTau[19]	= dTau[13];							dTauA[19] = dTauA[0] * 1.25;
	}

	long	nMaxCaseup	= 0;
	long	nMaxCaselo	= 0;
	long	nCaseSize	= bWind ? 20 : 12;
	double	dRate		= 0;
	double	dRateMaxup	= 0;
	double	dRateMaxlo	= 0;
	if(bPLUS_M)	nCaseSize = bWind ? 20 : 12;
	else		nCaseSize = bWind ? 19 : 11;

	for(long nCase=0; nCase<nCaseSize; nCase++)
	{
		dRate	= pow(dFu[nCase]/dFua[nCase], 2);
		if(dRateMaxup < dRate)
		{
			dRateMaxup	= dRate;
			nMaxCaseup	= nCase;
		}

		dRate	= pow(dFl[nCase]/dFla[nCase], 2);
		if(dRateMaxlo < dRate)
		{
			dRateMaxlo	= dRate;
			nMaxCaselo	= nCase;
		}
	}

	Max_up	= dFu[nMaxCaseup];
	Max_lo	= dFl[nMaxCaselo];
	Max_upA	= dFua[nMaxCaseup];
	Max_loA	= dFla[nMaxCaselo];
	Tau_up	= dTau[nMaxCaseup];
	Tau_lo	= dTau[nMaxCaselo];
	Tau_upA	= dTauA[nMaxCaseup];
	Tau_loA	= dTauA[nMaxCaselo];

	m_MapSecStress["Max_up"]	= Max_up;
	m_MapSecStress["Max_lo"]	= Max_lo;
	m_MapSecStress["Max_upA"]	= Max_upA;
	m_MapSecStress["Max_loA"]	= Max_loA;
	m_MapSecStress["Tau_up"]	= Tau_up;
	m_MapSecStress["Tau_lo"]	= Tau_lo;
	m_MapSecStress["Tau_upA"]	= Tau_upA;
	m_MapSecStress["Tau_loA"]	= Tau_loA;
	m_MapSecStress["MaxRate_up"]	= dRateMaxup + pow(Tau_up/Tau_upA,2);
	m_MapSecStress["MaxRate_lo"]	= dRateMaxlo + pow(Tau_lo/Tau_loA,2);
}
/////////////////////////////////////////////////////////////////////////////
/// * nG거더의 dSta위치에서 상하부최대응력값과 허용응력, 최대전단응력과 허용전단응력을 구함
///   - 모든 계산은 N.mm 단위
/////////////////////////////////////////////////////////////////////////////
void CFemStressCalc::GetFlangeStress_GenGirder(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec, long nPosSec)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx     = pGir->GetBxByStationDir(dSta, 0);
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CSteel           *pSteelCtl  = m_pDataManage->GetSteel();
	CCalcGeneral	CalcGen(m_pDataManage);

	GetSectionStress(nG, dSta, TRUE, dStaSec, nPosSec);
	CPlateBasicIndex	*pBxSec	= NULL;
	if(dStaSec!=0)	pBxSec	= pGir->GetBxByStation(dStaSec);
	else			pBxSec	= pBx;

	BOOL	bWind		= pData->m_bWind;
	BOOL	bBuckling	= m_pDataManage->GetCalcData()->m_bBucklingStress;
	double	dSigmaCA	= 0;
	double	dSigmaCAPre	= 0;
	double	dSigmaCAPost= 0;
	double	dSigmaTA	= 0;
	BOOL	bPLUS_M		= IsPositive(ELE_MOMENT3, nG, dSta);
	CString	strMatLeft	= _T("");
	CString	strMatRight	= _T("");

	// (APLATE-680) 두개의 Sta가 잘못 들어가면 안된다. 인자로 들어온 Sta는 부재력위치, 
	// 이함수에는 첫번째 sta로 단면위치가 들어가야 되므로 dStaSec가 들어가는게 맞다.
	ModelCalc.GetSectionCoefficient(nG, (dStaSec == 0)? dSta : dStaSec, FALSE, dStaSec, nPosSec);
	double	dUpFlangeW	= ModelCalc.GetMapValueSecCo("FLANGE_W_UP");
	double	dUpFlangeT	= ModelCalc.GetMapValueSecCo("FLANGE_T_UP");
	double	dWebT		= ModelCalc.GetMapValueSecCo("WEB_T");
	double	dDnFlangeW	= ModelCalc.GetMapValueSecCo("FLANGE_W_DN");
	double	dDnFlangeT	= ModelCalc.GetMapValueSecCo("FLANGE_T_DN");
	double	dWebH		= ModelCalc.GetMapValueSecCo("WEB_H");
	double	dAw			= dWebH*dWebT;
	double	dAc			= dDnFlangeW*dDnFlangeT;
	double	dDist		= pDB->m_dLengthBaseCrossBeam;
	double	dHy_RPre	= m_MapSecStress["HyR_Pre"];
	double	dHy_RPos	= m_MapSecStress["HyR_Pos"];
	CHGBaseSteel	*pSteel = NULL;
	if(bPLUS_M)  ///< 정모멘트
	{
		CalcGen.GetMaterial(pBxSec, G_F_L, strMatLeft, strMatRight);
		pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		CHGAllowStress	AllowTension(pSteel);

		CalcGen.GetMaterial(pBxSec, G_F_U, strMatLeft, strMatRight);
		CHGBaseSteel *pSteelUp = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		CHGAllowStress	AllowBuckling(pSteelUp);
		CHGAllowStress	AllowBendingStress(pSteelUp);			

		double	dWidth		= (dUpFlangeW-dWebT)/2;
		AllowBuckling.SetAllowBuckling(dUpFlangeT, dWidth);
		AllowBendingStress.SetAllowBendingStress(dAw, dAc, dDist, dDnFlangeW);		
		
		dSigmaTA		= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dDnFlangeT);
		dSigmaCAPre		= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dUpFlangeT);		// 합성전 허용응력
		dSigmaCAPost	= AllowBendingStress.GetAllowStress(bBuckling ? ALLOW_BENDINGCOMP_CASE1 : ALLOW_BENDINGCOMP_CASE2, dUpFlangeT);	// 합성후 허용응력
	}
	else		///< 부모멘트
	{
		CalcGen.GetMaterial(pBxSec, G_F_U, strMatLeft, strMatRight);
		pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		CHGAllowStress	AllowTension(pSteel);
		
		CalcGen.GetMaterial(pBxSec, G_F_L, strMatLeft, strMatRight);
		CHGBaseSteel *pSteelLo = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
		
		double	dWidth		= (dDnFlangeW-dWebT)/2;
		CHGAllowStress	AllowBuckling(pSteelLo);				
		CHGAllowStress	AllowBendingStress(pSteelLo);	
		AllowBuckling.SetAllowBuckling(dDnFlangeT, dWidth);
		AllowBendingStress.SetAllowBendingStress(dAw, dAc, dDist, dDnFlangeW);		
		dSigmaTA	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dUpFlangeT);
		CalcGen.GetMaterial(pBxSec, G_F_L, strMatLeft, strMatRight);		
		dSigmaCA	= min(AllowBendingStress.GetAllowStress(ALLOW_BENDINGCOMP_CASE2, dDnFlangeT), AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dDnFlangeT));		
	}

	CalcGen.GetMaterial(pBxSec, G_W, strMatLeft, strMatRight);
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
	CHGAllowStress	AllowTension(pSteel);
	double TauA1 = AllowTension.GetAllowStress(ALLOW_SHEARSTRESS, dWebT);
	double TauA2 = TauA1 * 1.2;
	double TauA3 = TauA1 * 1.25;
	double Tau1  = m_MapSecStress["Tau_shear"];
	double Tau2  = m_MapSecStress["Tau_shear_w"];
	double Tau3  = m_MapSecStress["Tau_shear_wl"];

	Tau_up = Tau1;
	Tau_lo = Tau1;
	Tau_upA = TauA1;
	Tau_loA = TauA1;

	// 풍하중 제외
	// 1. 합성전
	// 2. 합성전 + 합성후 + 활하중
	// 3. 합성전 + 합성후 + 활하중 + 크리이프
	// 4. 합성전 + 합성후 + 활하중 + 크리이프 + 건조수축
	// 5. 합성전 + 합성후 + 활하중 + 크리이프 + 건조수축 + 온도차
	// 6. 합성전 + 합성후 + 활하중 + 크리이프 + 건조수축 - 온도차

	// 풍하중 포함
	// 7. 합성전 + 합성후 + 풍하중
	// 8. 합성전 + 합성후 + 활하중 + (1/2)풍하중
	// 9. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프
	//10. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프 + 건조수축
	//11. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프 + 건조수축 + 온도차
	//12. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프 + 건조수축 - 온도차

	double Sigma_upA1, Sigma_upA2, Sigma_upA3, Sigma_upA4, Sigma_upA5, Sigma_upA6, Sigma_upA7, Sigma_upA8, Sigma_upA9, Sigma_upA10, Sigma_upA11, Sigma_upA12;
	double Sigma_loA1, Sigma_loA2, Sigma_loA3, Sigma_loA4, Sigma_loA5, Sigma_loA6, Sigma_loA7, Sigma_loA8, Sigma_loA9, Sigma_loA10, Sigma_loA11, Sigma_loA12;

	if(bPLUS_M)	// 정모멘트
	{  
		Sigma_upA1 = dSigmaCAPre * dHy_RPre;
		Sigma_upA2 = dSigmaCAPost * dHy_RPos;
		Sigma_upA3 = dSigmaCAPost * 1.15 * dHy_RPos;
		Sigma_upA4 = dSigmaCAPost * 1.15 * dHy_RPos;
		Sigma_upA5 = dSigmaCAPost * 1.30 * dHy_RPos;
		Sigma_upA6 = dSigmaCAPost * 1.30 * dHy_RPos;

		Sigma_loA1 = dSigmaTA * dHy_RPre;
		Sigma_loA2 = dSigmaTA * dHy_RPos;
		Sigma_loA3 = dSigmaTA * dHy_RPos;
		Sigma_loA4 = dSigmaTA * dHy_RPos;
		Sigma_loA5 = dSigmaTA * 1.15 * dHy_RPos;
		Sigma_loA6 = dSigmaTA * 1.15 * dHy_RPos;
	}
	else   
	{
		Sigma_upA1 = dSigmaTA * dHy_RPre;
		Sigma_upA2 = dSigmaTA * dHy_RPos;
		Sigma_upA3 = dSigmaTA * dHy_RPos;
		Sigma_upA4 = dSigmaTA * dHy_RPos;
		Sigma_upA5 = dSigmaTA * 1.15 * dHy_RPos;
		Sigma_upA6 = dSigmaTA * 1.15 * dHy_RPos;

		Sigma_loA1 = dSigmaCA * dHy_RPre;
		Sigma_loA2 = dSigmaCA * dHy_RPos;
		Sigma_loA3 = dSigmaCA * dHy_RPos;
		Sigma_loA4 = dSigmaCA * dHy_RPos;
		Sigma_loA5 = dSigmaCA * 1.15 * dHy_RPos;
		Sigma_loA6 = dSigmaCA * 1.15 * dHy_RPos;
	}

	if(bWind)
	{
		if(bPLUS_M)	// 정모멘트
		{  
			Sigma_upA7  = dSigmaCAPre * 1.2 * dHy_RPre;
			Sigma_upA8  = dSigmaCAPost* 1.25 * dHy_RPos;
			Sigma_upA9  = dSigmaCAPost * 1.25 * dHy_RPos;
			Sigma_upA10 = dSigmaCAPost * 1.25 * dHy_RPos;
			Sigma_upA11 = dSigmaCAPost * 1.35 * dHy_RPos;
			Sigma_upA12 = dSigmaCAPost * 1.35 * dHy_RPos;

			Sigma_loA7  = dSigmaTA * 1.2 * dHy_RPre;
			Sigma_loA8  = dSigmaTA * 1.25 * dHy_RPos;
			Sigma_loA9  = dSigmaTA * 1.25 * dHy_RPos;
			Sigma_loA10 = dSigmaTA * 1.25 * dHy_RPos;
			Sigma_loA11 = dSigmaTA * 1.35 * dHy_RPos;
			Sigma_loA12 = dSigmaTA * 1.35 * dHy_RPos;
		}
		else   
		{
			Sigma_upA7  = dSigmaTA * 1.25 * dHy_RPre;
			Sigma_upA8  = dSigmaTA * 1.25 * dHy_RPos;
			Sigma_upA9  = dSigmaTA * 1.25 * dHy_RPos;
			Sigma_upA10 = dSigmaTA * 1.25 * dHy_RPos;
			Sigma_upA11 = dSigmaTA * 1.35 * dHy_RPos;
			Sigma_upA12 = dSigmaTA * 1.35 * dHy_RPos;

			Sigma_loA7  = dSigmaCA * 1.25 * dHy_RPre;
			Sigma_loA8  = dSigmaCA * 1.25 * dHy_RPos;
			Sigma_loA9  = dSigmaCA * 1.25 * dHy_RPos;
			Sigma_loA10 = dSigmaCA * 1.25 * dHy_RPos;
			Sigma_loA11 = dSigmaCA * 1.35 * dHy_RPos;
			Sigma_loA12 = dSigmaCA * 1.35 * dHy_RPos;
		}
	}

	double Sigma_up_prev, Sigma_lo_prev;	// 합성전
	double Sigma_up_post, Sigma_lo_post;	// 합성후
	double Sigma_up_live, Sigma_lo_live;	// 활하중
	double Sigma_up_wind, Sigma_lo_wind;	// 풍하중
	double Sigma_up_cree, Sigma_lo_cree;	// 크리이프
	double Sigma_up_shri, Sigma_lo_shri;	// 건조수축
	double Sigma_up_temp, Sigma_lo_temp;	// 온도차

	double Sigma_up, Sigma_lo, Sigma_upA, Sigma_loA; // 상, 하, 허용 상, 허용 하

	// 합성전
	Sigma_up_prev	= m_MapSecStress["Sigma_su"];
	Sigma_lo_prev	= m_MapSecStress["Sigma_sl"];
	// 합성후
	Sigma_up_post = m_MapSecStress["Sigma_vsu"];
	Sigma_lo_post = m_MapSecStress["Sigma_vsl"];
	// 활하중
	Sigma_up_live = m_MapSecStress["Sigma_vsu_l"];
	Sigma_lo_live = m_MapSecStress["Sigma_vsl_l"];
	// 풍하중
	Sigma_up_wind = m_MapSecStress["Sigma_vsu_w"];
	Sigma_lo_wind = m_MapSecStress["Sigma_vsl_w"];
	// 크리이프
	Sigma_up_cree = m_MapSecStress["Sigma_vsu1"];
	Sigma_lo_cree = m_MapSecStress["Sigma_vsl1"];
	// 건조수축
	Sigma_up_shri = m_MapSecStress["Sigma_vsu2"];
	Sigma_lo_shri = m_MapSecStress["Sigma_vsl2"];
	// 온도차
	Sigma_up_temp = m_MapSecStress["Sigma_sut"];
	Sigma_lo_temp = m_MapSecStress["Sigma_slt"];

	// 1. 합성전
	Sigma_upA = Sigma_upA1;
	Sigma_loA = Sigma_loA1;

	Max_up  = Sigma_up_prev;
	Max_lo  = Sigma_lo_prev;
	Max_upA = Sigma_upA;
	Max_loA = Sigma_loA;

	// 2. 합성전 + 합성후 + 활하중
	double	dAlpha	= m_MapSecStress["Alpha"];
	double	dFw_up	= 0;
	double	dFw_lo	= 0;
	if(dAlpha < 10 && Sigma_up_post+Sigma_up_live > 0)		dFw_up = m_MapSecStress["Fw"];
	else if(dAlpha < 10 && Sigma_up_post+Sigma_up_live < 0)	dFw_up = -m_MapSecStress["Fw"];
	if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live > 0)		dFw_lo = m_MapSecStress["Fw"];
	else if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live < 0)	dFw_lo = -m_MapSecStress["Fw"];

	Sigma_upA = Sigma_upA2;
	Sigma_loA = Sigma_loA2;
	Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + dFw_up;
	Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + dFw_lo;

	if (fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
	{
		Max_up  = Sigma_up;
		Max_upA = Sigma_upA;
	}
	if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
	{
		Max_lo  = Sigma_lo;
		Max_loA = Sigma_loA;
	}

	// 3. 합성전 + 합성후 + 활하중 + 크리이프
	Sigma_upA = Sigma_upA3;
	Sigma_loA = Sigma_loA3;
	Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_cree + dFw_up;
	Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_cree + dFw_lo;

	if (fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
	{
		Max_up  = Sigma_up;
		Max_upA = Sigma_upA;
	}
	if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
	{
		Max_lo  = Sigma_lo;
		Max_loA = Sigma_loA;
	}

	// 4. 합성전 + 합성후 + 활하중 + 크리이프 + 건조수축
	Sigma_upA = Sigma_upA4;
	Sigma_loA = Sigma_loA4;
	Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_cree + Sigma_up_shri + dFw_up;
	Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_cree + Sigma_lo_shri + dFw_lo;

	if (fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
	{
		Max_up  = Sigma_up;
		Max_upA = Sigma_upA;
	}
	if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
	{
		Max_lo  = Sigma_lo;
		Max_loA = Sigma_loA;
	}

	// 5. 합성전 + 합성후 + 활하중 + 크리이프 + 건조수축 + 온도차
	Sigma_upA = Sigma_upA5;
	Sigma_loA = Sigma_loA5;
	Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_cree + Sigma_up_shri + Sigma_up_temp + dFw_up;
	Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_cree + Sigma_lo_shri + Sigma_lo_temp + dFw_lo;

	if (fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
	{
		Max_up  = Sigma_up;
		Max_upA = Sigma_upA;
	}
	if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
	{
		Max_lo  = Sigma_lo;
		Max_loA = Sigma_loA;
	}

	// 6. 합성전 + 합성후 + 활하중 + 크리이프 + 건조수축 - 온도차
	Sigma_upA = Sigma_upA6;
	Sigma_loA = Sigma_loA6;
	Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_cree + Sigma_up_shri - Sigma_up_temp + dFw_up;
	Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_cree + Sigma_lo_shri - Sigma_lo_temp + dFw_lo;

	if (fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
	{
		Max_up  = Sigma_up;
		Max_upA = Sigma_upA;
	}
	if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
	{
		Max_lo  = Sigma_lo;
		Max_loA = Sigma_loA;
	}

	if(bWind)
	{
		// 7. 합성전 + 합성후 + 풍하중
		Sigma_upA = Sigma_upA7;
		Sigma_loA = Sigma_loA7;
		Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_wind + dFw_up;
		Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_wind + dFw_lo;

		if (fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
		{
			Max_up  = Sigma_up;
			Max_upA = Sigma_upA;
			Tau_up = Tau2;
			Tau_upA = TauA2;
		}
		if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
		{
			Max_lo  = Sigma_lo;
			Max_loA = Sigma_loA;
			Tau_lo = Tau2;
			Tau_loA = TauA2;
		}
		// 8. 합성전 + 합성후 + 활하중 + (1/2)풍하중
		Sigma_upA = Sigma_upA8;
		Sigma_loA = Sigma_loA8;
		Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_wind / 2.0 + dFw_up;
		Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_wind / 2.0 + dFw_lo;

		if(fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
		{
			Max_up  = Sigma_up;
			Max_upA = Sigma_upA;
			Tau_up  = Tau3;
			Tau_upA = TauA3;
		}
		if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
		{
			Max_lo  = Sigma_lo;
			Max_loA = Sigma_loA;
			Tau_lo  = Tau3;
			Tau_loA = TauA3;
		}
		// 9. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프
		Sigma_upA = Sigma_upA9;
		Sigma_loA = Sigma_loA9;
		Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_wind / 2.0 + Sigma_up_cree + dFw_up;
		Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_wind / 2.0 + Sigma_lo_cree + dFw_lo;

		if (fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
		{
			Max_up  = Sigma_up;
			Max_upA = Sigma_upA;
			Tau_up  = Tau3;
			Tau_upA = TauA3;
		}
		if (fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
		{
			Max_lo  = Sigma_lo;
			Max_loA = Sigma_loA;
			Tau_lo  = Tau3;
			Tau_loA = TauA3;
		}
		//10. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프 + 건조수축
		Sigma_upA = Sigma_upA10;
		Sigma_loA = Sigma_loA10;
		Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_wind / 2.0 + Sigma_up_cree + Sigma_up_shri + dFw_up;
		Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_wind / 2.0 + Sigma_lo_cree + Sigma_lo_shri + dFw_lo;

		if(fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
		{
			Max_up  = Sigma_up;
			Max_upA = Sigma_upA;
			Tau_up  = Tau3;
			Tau_upA = TauA3;
		}
		if(fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
		{
			Max_lo  = Sigma_lo;
			Max_loA = Sigma_loA;
			Tau_lo  = Tau3;
			Tau_loA = TauA3;
		}
		//11. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프 + 건조수축 + 온도차
		Sigma_upA = Sigma_upA11;
		Sigma_loA = Sigma_loA11;

		Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_wind / 2.0 + Sigma_up_cree + Sigma_up_shri + Sigma_up_temp + dFw_up;
		Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_wind / 2.0 + Sigma_lo_cree + Sigma_lo_shri + Sigma_lo_temp + dFw_lo;

		if(fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
		{
			Max_up  = Sigma_up;
			Max_upA = Sigma_upA;
			Tau_up  = Tau3;
			Tau_upA = TauA3;
		}
		if(fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
		{
			Max_lo  = Sigma_lo;
			Max_loA = Sigma_loA;
			Tau_lo  = Tau3;
			Tau_loA = TauA3;
		}
		//12. 합성전 + 합성후 + 활하중 + (1/2)풍하중 + 크리이프 + 건조수축 - 온도차
		Sigma_upA = Sigma_upA12;
		Sigma_loA = Sigma_loA12;
		Sigma_up = Sigma_up_prev + Sigma_up_post + Sigma_up_live + Sigma_up_wind / 2.0 + Sigma_up_cree + Sigma_up_shri - Sigma_up_temp + dFw_up;
		Sigma_lo = Sigma_lo_prev + Sigma_lo_post + Sigma_lo_live + Sigma_lo_wind / 2.0 + Sigma_lo_cree + Sigma_lo_shri - Sigma_lo_temp + dFw_lo;

		if(fabs (Sigma_up / Sigma_upA) > fabs (Max_up / Max_upA))
		{
			Max_up  = Sigma_up;
			Max_upA = Sigma_upA;
			Tau_up  = Tau3;
			Tau_upA = TauA3;
		}
		if(fabs (Sigma_lo / Sigma_loA) > fabs (Max_lo / Max_loA))
		{
			Max_lo  = Sigma_lo;
			Max_loA = Sigma_loA;
			Tau_lo  = Tau3;
			Tau_loA = TauA3;
		}
	}
}

BOOL CFemStressCalc::IsPositive(int nForceType, long nG, double dSta)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);

	if(nForceType==ELE_MOMENT3 || nForceType==ELE_MOMENT2)
	{
		CPlateBasicIndex *pBxStt = pGir->GetBxOnJijum(0);
		CPlateBasicIndex *pBxEnd = pGir->GetBxOnJijum(pDB->m_nQtyJigan);
		if(pDB->m_nQtyJigan == 1)
			return TRUE;
		if(pBxStt->IsDanbuJijumStt() && pBxStt->GetStation()==dSta)
			return TRUE;
		if(pBxEnd->IsDanbuJijumEnd() && pBxEnd->GetStation()==dSta)
			return TRUE;
	}
	
	BOOL bTUContinuousSpan	= pDB->IsTUContinuousSpan();
	double	dValuePre		= GetElementForce(nForceType, bTUContinuousSpan ? FRAME_PRE_DEADLOAD_2 : FRAME_PRE_DEADLOAD, nG, dSta, 0);
	double	dValuePost		= GetElementForce(nForceType, FRAME_NEXT_DEADLOAD, nG, dSta, 0);
	double	dValueLiveMax	= GetElementForce(nForceType, LC_LIVE_MAX, nG, dSta, 0);
	double	dValueLiveMin	= GetElementForce(nForceType, LC_LIVE_MIN, nG, dSta, 0);
	
	double dComp1 = dValuePre+dValuePost+dValueLiveMax;
	double dComp2 = dValuePre+dValuePost+dValueLiveMin;
	
	if(dComp1 > 0.0 && dComp2 > 0.0)
		return TRUE;
	else if(dComp1 < 0.0 && dComp2 < 0.0)
		return FALSE;
	
	if(ABS(dComp1) >= ABS(dComp2))
		return TRUE;
	else
		return FALSE;
}

void CFemStressCalc::GetSectionStress(long nG, double dSta, BOOL bHAPSUNG, double dStaSec, long nPosSec)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	m_MapSecStress.clear();

	if(pDB->IsTUGir())	GetSectionStress_TUGirder(nG, dSta, bHAPSUNG, dStaSec, nPosSec);
	else				GetSectionStress_GenGirder(nG, dSta, bHAPSUNG, dStaSec, nPosSec);
}

void CFemStressCalc::GetSectionStress_TUGirder(long nG, double dSta, BOOL bHAPSUNG, double dStaSec, long nPosSec)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);
	CCalcData			*pData	= m_pDataManage->GetCalcData();
	CFemModelingCalc	ModelCalc(m_pDataManage, m_pCalcStd);

	BOOL	bWind		= pData->m_bWind;
	BOOL	bPositiveM	= IsPositive(ELE_MOMENT3, nG, dSta);
	BOOL	bPositiveS	= IsPositive(ELE_SHEAR_2, nG, dSta);
	BOOL	bPositiveT	= IsPositive(ELE_AXTORSN, nG, dSta);
	double	dMph1 = 0, dMph2 = 0, dMph3 = 0, dMph4 = 0, dMph5 = 0, dMph6 = 0, dMw = 0, dMphT = 0;
	double	dSph1 = 0, dSph2 = 0, dSph3 = 0, dSph4 = 0, dSph5 = 0, dSph6 = 0, dSw = 0, dSph6Apos = 0, dSphT = 0;
	double	dMst = 0, dMsct = 0, dMvt = 0, dMwt = 0;
	double	dMomentSumMax = 0, dMomentSumMin = 0, dShearSumMax = 0, dShearSumMin = 0, dTorsionSumMax = 0, dTorsionSumMin = 0, dTorsionSum = 0;	

	//////////
	m_MapSecStress["HyR_Pre"] = m_MapSecStress["HyR_Pos"] = 1;
	if(dStaSec!=0)	ModelCalc.GetSectionCoefficient(nG, dStaSec, FALSE, 0, nPosSec);
	else			ModelCalc.GetSectionCoefficient(nG, dSta, FALSE, 0, nPosSec);
	double	dTd			= ModelCalc.m_MapSecCoe["SLAB_THICK"];
	double	dBc			= ModelCalc.m_MapSecCoe["BindConWidth"];
	double	dTc			= ModelCalc.m_MapSecCoe["BindConThick"];
	double	dDist		= ModelCalc.m_MapSecCoe["BindConDist"];	
	double	dBu			= ModelCalc.m_MapSecCoe["FLANGE_W_UP"];
	double	dTu			= ModelCalc.m_MapSecCoe["FLANGE_T_UP"];
	double	dTw			= ModelCalc.m_MapSecCoe["WEB_T"];
	double	dHw			= ModelCalc.m_MapSecCoe["WEB_H"];
	double	dBl			= ModelCalc.m_MapSecCoe["FLANGE_W_DN"];
	double	dTl			= ModelCalc.m_MapSecCoe["FLANGE_T_DN"];
	
	if(bPositiveM)
	{
		if(pDB->IsTUContinuousSpan())	// 연속교
		{
			CSteelUWeight *pUWeight = m_pDataManage->GetSteelUWeight();
			double dX = 0;
			double dL = pGir->GetLengthPositiveSection(dStaSec==0?dSta:dStaSec, dX);
			double dW = (dBu*dTu+dTw*dHw+dBl*dTl)*pUWeight->m_dUWeightGangjae;
			double dC = (dBc*dTc)*pUWeight->m_dUWeightChulgunCon;
			dMph1	= ((-dW*dX/2)*(dL-dX));																// 강 자중에 의한 모멘트
			dMph2	= ((-dC*dX/2)*(dL-dX));																// 구속콘크리트 자중에 의한 모멘트
			dMph3	= (((dC+dW)*dX/2)*(dL-dX));
			dMphT	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_ST_CON, nG, dSta,0));					// 턴오버 하중 및 하중 재분배에 의한 모멘트
			dMph3	+= dMphT;																			// 턴오버 하중 및 하중 재분배에 의한 모멘트
			dMph4	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta,0));						// 바닥판 자중에 의한 모멘트
			dMph5	= frkNM(GetElementForce(ELE_MOMENT3, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 모멘트
			dMph6	= frkNM(GetElementForce(ELE_MOMENT3, LC_ONLYLIVE_MAX | LC_DISP_MAX, nG, dSta,0));	// 활하중에 의한 모멘트
			
			dSph1	= ((-dW*dX)-(-dW*dL/2));															// 강 자중에 의한 전단력
			dSph2	= ((-dC*dX)-(-dC*dL/2));															// 구속콘크리트 자중에 의한 전단력
			dSph3	= (dW+dC)*dX-(dW+dC)*dL/2;
			dSphT	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_ST_CON, nG, dSta,0));					// 턴오버 하중 및 하중 재분배에 의한 전단력
			dSph3	+= dSphT;																			// 턴오버 하중 및 하중 재분배에 의한 전단력
			dSph4	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_C, nG, dSta,0));						// 바닥판 자중에 의한 전단력
			dSph5	= frkN(GetElementForce(ELE_SHEAR_2, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 전단력
			dSph6	= frkN(GetElementForce(ELE_SHEAR_2, LC_ONLYLIVE_MAX | LC_DISP_MAX, nG, dSta,0));	// 활하중에 의한 전단력
			dSph6Apos	= frkN(GetElementForce(ELE_SHEAR_2, LC_LIVE_MAX | LC_DISP_MAX, nG, dSta,0));
		}
		else	// 단순교
		{
			dMph1	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_S,  nG, dSta,0));						// 강 자중에 의한 모멘트
			dMph2	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_CONF, nG, dSta,0));					// 구속콘크리트 자중에 의한 모멘트
			dMph3	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_TURN, nG, dSta,0));					// 턴오버 하중에 의한 모멘트
			dMph4	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta,0));						// 바닥판 자중에 의한 모멘트
			dMph5	= frkNM(GetElementForce(ELE_MOMENT3, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 모멘트
			dMph6	= frkNM(GetElementForce(ELE_MOMENT3, LC_ONLYLIVE_MAX | LC_DISP_MAX, nG, dSta,0));	// 활하중에 의한 모멘트

			dSph1	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_S, nG, dSta,0));						// 강 자중에 의한 전단력
			dSph2	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_CONF, nG, dSta,0));						// 구속콘크리트 자중에 의한 전단력
			dSph3	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_TURN, nG, dSta,0));						// 턴오버 하중에 의한 전단력
			dSph4	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_C, nG, dSta,0));						// 바닥판 자중에 의한 전단력
			dSph5	= frkN(GetElementForce(ELE_SHEAR_2, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 전단력
			dSph6	= frkN(GetElementForce(ELE_SHEAR_2, LC_ONLYLIVE_MAX | LC_DISP_MAX, nG, dSta,0));	// 활하중에 의한 전단력
			dSph6Apos	= frkN(GetElementForce(ELE_SHEAR_2, LC_LIVE_MAX | LC_DISP_MAX, nG, dSta,0));
		}
	}
	else
	{
		if(pDB->IsTUContinuousSpan())	// 연속교
		{
			dMph1	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_ST_CON,  nG, dSta,0));					// 강 자중에 의한 모멘트
			dMph3	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_CONF | LC_PREV_C, nG, dSta,0));		// 구속콘크리트 자중+바닥판 자중에 의한 모멘트
			dMph4	= frkNM(GetElementForce(ELE_MOMENT3, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 모멘트
			dMph5	= frkNM(GetElementForce(ELE_MOMENT3, LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));	// 활하중에 의한 모멘트
			
			dSph1	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_ST_CON, nG, dSta,0));					// 강 자중에 의한 전단력
			dSph3	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_CONF | LC_PREV_C, nG, dSta,0));			// 구속콘크리트 자중+바닥판 자중에 의한 전단력
			dSph4	= frkN(GetElementForce(ELE_SHEAR_2, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 전단력
			dSph5	= frkN(GetElementForce(ELE_SHEAR_2, LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));	// 활하중에 의한 전단력
			dSph6Apos	= frkN(GetElementForce(ELE_SHEAR_2, LC_LIVE_MIN | LC_DISP_MIN, nG, dSta,0));
		}
		else
		{
			dMph1	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_S,  nG, dSta,0));						// 강 자중에 의한 모멘트
			dMph2	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_CONF, nG, dSta,0));					// 구속콘크리트 자중에 의한 모멘트
			dMph3	= frkNM(GetElementForce(ELE_MOMENT3, LC_PREV_C, nG, dSta,0));						// 바닥판 자중에 의한 모멘트
			dMph4	= frkNM(GetElementForce(ELE_MOMENT3, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 모멘트
			dMph5	= frkNM(GetElementForce(ELE_MOMENT3, LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));	// 활하중에 의한 모멘트

			dSph1	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_S, nG, dSta,0));						// 강 자중에 의한 전단력
			dSph2	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_CONF, nG, dSta,0));						// 구속콘크리트 자중에 의한 전단력
			dSph3	= frkN(GetElementForce(ELE_SHEAR_2, LC_PREV_C, nG, dSta,0));						// 바닥판 자중에 의한 전단력
			dSph4	= frkN(GetElementForce(ELE_SHEAR_2, LC_NEXT, nG, dSta,0));							// 2차 고정하중에 의한 전단력
			dSph5	= frkN(GetElementForce(ELE_SHEAR_2, LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));	// 활하중에 의한 전단력
			dSph6Apos	= frkN(GetElementForce(ELE_SHEAR_2, LC_LIVE_MIN | LC_DISP_MIN, nG, dSta,0));
		}
	}
	dMst		= frkNM(GetElementForce(ELE_AXTORSN, LC_PREV_C, nG, dSta,0));						// 바닥판 합성전 고정하중에의한 비틀림모멘트
	dMsct		= frkNM(GetElementForce(ELE_AXTORSN, LC_NEXT, nG, dSta,0));							// 바닥판 합성후 고정하중에의한 비틀림모멘트
	dMvt		= frkNM(GetElementForce(ELE_AXTORSN, bPositiveT ? LC_ONLYLIVE_MAX | LC_DISP_MAX : LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));	// 활하중에의한 비틀림모멘트

	dMw			= frkNM(GetElementForce(ELE_MOMENT3, bPositiveM ? LC_WIND_MAX : LC_WIND_MIN, nG, dSta,0));	// 풍하중에 의한 모멘트
	dSw			= frkN(GetElementForce(ELE_SHEAR_2, bPositiveS ? LC_WIND_MAX : LC_WIND_MIN, nG, dSta,0));	// 풍하중에 의한 전단력
	dMwt		= frkNM(GetElementForce(ELE_AXTORSN, bPositiveT ? LC_WIND_MAX : LC_WIND_MIN, nG, dSta,0));	// 풍하중에의한 비틀림모멘트

	dMomentSumMax	= frkNM(GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, dSta,0));///< 합계 최대모멘트
	dMomentSumMin	= frkNM(GetElementForce(ELE_MOMENT3, LC_SUM_MIN, nG, dSta,0));///< 합계 최소모멘트
	dShearSumMax	= frkN(GetElementForce(ELE_SHEAR_2, LC_SUM_MAX, nG, dSta,0));
	dShearSumMin	= frkN(GetElementForce(ELE_SHEAR_2, LC_SUM_MIN, nG, dSta,0));
	dTorsionSumMax	= frkNM(GetElementForce(ELE_AXTORSN, LC_SUM_MAX, nG, dSta,0));
	dTorsionSumMin	= frkNM(GetElementForce(ELE_AXTORSN, LC_SUM_MIN, nG, dSta,0));
	dTorsionSum		= bPositiveT ? dTorsionSumMax : dTorsionSumMin;

	m_MapSecStress["Mph1"]	= dMph1;
	m_MapSecStress["Mph2"]	= dMph2;
	m_MapSecStress["Mph3"]	= dMph3;
	m_MapSecStress["Mph4"]	= dMph4;
	m_MapSecStress["Mph5"]	= dMph5;
	m_MapSecStress["Mph6"]	= dMph6;
	m_MapSecStress["Mw"]	= dMw;
	m_MapSecStress["MphT"]	= dMphT;

	m_MapSecStress["Sph1"]	= dSph1;
	m_MapSecStress["Sph2"]	= dSph2;
	m_MapSecStress["Sph3"]	= dSph3;
	m_MapSecStress["Sph4"]	= dSph4;
	m_MapSecStress["Sph5"]	= dSph5;
	m_MapSecStress["Sph6"]	= dSph6;
	m_MapSecStress["Sph6_1"]	= dSph6Apos;
	m_MapSecStress["Sw"]	= dSw;
	m_MapSecStress["SphT"]	= dSphT;

	m_MapSecStress["Mst"]	= dMst;
	m_MapSecStress["Msct"]	= dMsct;
	m_MapSecStress["Mvt"]	= dMvt;
	m_MapSecStress["Mwt"]	= dMwt;

	m_MapSecStress["MomentPreDead"]	= dMph3 + dMph4;
	m_MapSecStress["MomentPostDead"]= dMph5;
	m_MapSecStress["MomentLive"]	= dMph6;
	m_MapSecStress["MomentWind"]	= dMw;
	m_MapSecStress["ShearPreDead"]	= dSph3 + dSph4;
	m_MapSecStress["ShearPostDead"]	= dSph5;
	m_MapSecStress["ShearLive"]		= dSph6;
	m_MapSecStress["ShearWind"]		= dSw;
	m_MapSecStress["MomentSumMax"]	= dMomentSumMax;   
	m_MapSecStress["MomentSumMin"]	= dMomentSumMin;
	m_MapSecStress["ShearSumMax"]	= dShearSumMax;   
	m_MapSecStress["ShearSumMin"]	= dShearSumMin;
	m_MapSecStress["TorsionSum"]	= dTorsionSum;

	double	dK			= ModelCalc.GetValueOfInfK(nG, dSta);
	double	dEs			= pData->DESIGN_MATERIAL.m_dEst;			// 강의 탄성계수
	double	dEc			= pData->DESIGN_MATERIAL.m_dBindEc;			// 구속콘크리트의 탄성계수
	double	dEd			= pData->DESIGN_MATERIAL.m_dEc;				// 바닥판 콘크리트의 탄성계수
	double	dNc			= (double)pData->DESIGN_MATERIAL.m_nBindNc;	// 강과 구속콘크리트의 탄성계수비
	double	dNd			= (double)pData->DESIGN_MATERIAL.m_nN;		// 강과 바닥판 콘크리트의 탄성계수비
	double	dEpsilon	= pData->DESIGN_MATERIAL.m_dEpsilonS;		// 건조수축계수
	double	dAlpha		= pData->DESIGN_MATERIAL.m_dAlpha;			// 선팽창계수
	double	dCreep1		= pData->DESIGN_MATERIAL.m_dCreep;			// 크리프1 
	double	dCreep2		= pData->DESIGN_MATERIAL.m_dCreep2;			// 크리프2
	double	dTemp		= pData->DESIGN_MATERIAL.m_dTemparature;	// 강과 콘크리트의 온도차 
	//double	dL			= pDB->m_dLengthBaseCrossBeam;				// 가로보 간격
	//double	dG			= pData->DESIGN_MATERIAL.m_dShearEs;		// 전단탄성계수
	double	dCu			= pData->m_dCoverUp;
	double	dCl			= pData->m_dCoverDn;

	double	dAsWeb		= ModelCalc.m_MapSecCoe["As_WEB"];
	double	dSumAs		= ModelCalc.m_MapSecCoe["As_SUM"];
	double	dSumAsY		= ModelCalc.m_MapSecCoe["Asy_SUM"];
	double	dSumAsY2	= ModelCalc.m_MapSecCoe["Asy2_SUM"];
	double	dSumIx		= ModelCalc.m_MapSecCoe["Iyy_SUM"];
	double	dConfAs		= dBc * dTc;
	//double	dConfY		= dHw/2 + dTu;
	double	dConfIx		= dBc * pow(dTc, 3) / 12;

	// Phase 1 - 강 자중에 의한 응력
	double	dIsApo		= dSumAsY2 + dSumIx;
	double	dDeltaS		= dSumAsY /	dSumAs;
	double	dIs			= dIsApo - dSumAs * pow(dDeltaS, 2);
	double	dYsu1		= dHw/2 + dTu - dDeltaS;
	double	dYsl1		= -(dHw/2 + dTl + dDeltaS);
	double	dFsu1		= dMph1 * dYsu1 / dIs;
	double	dFsl1		= dMph1 * dYsl1 / dIs;
	double	dK1			= (dBu*pow(dTu,3) + dHw*pow(dTw,3) + dBl*pow(dTl,3)) / 3;

	if(dStaSec!=0)	ModelCalc.GetSectionCoefficient(nG, dStaSec, TRUE, 0, nPosSec);
	else			ModelCalc.GetSectionCoefficient(nG, dSta, TRUE, 0, nPosSec);
	double	dSlabAs		= ModelCalc.m_MapSecCoe["As_SLAB"];
	double	dSlabIx		= ModelCalc.m_MapSecCoe["Iyy_SLAB"];
	double	dBeff		= ModelCalc.m_MapSecCoe["SlabWidth"];

	m_MapSecStress["Td"]	= dTd;
	m_MapSecStress["Bc"]	= dBc;
	m_MapSecStress["Tc"]	= dTc;
	m_MapSecStress["Dist"]	= dDist;
	m_MapSecStress["Bu"]	= dBu;
	m_MapSecStress["Tu"]	= dTu;
	m_MapSecStress["Tw"]	= dTw;
	m_MapSecStress["Hw"]	= dHw;
	m_MapSecStress["Bl"]	= dBl;
	m_MapSecStress["Tl"]	= dTl;
	m_MapSecStress["Ysu"]	= dYsu1;
	m_MapSecStress["Ysl"]	= dYsl1;
	m_MapSecStress["Beff"]	= dBeff;
	m_MapSecStress["K"]		= dK;

	if(bPositiveM)
	{
		// Phase 2 - 구속콘크리트 자중에 의한 응력
		double	dAv2		= dSumAs + dConfAs / dNc;
		double	dIcPnc		= dConfIx / dNc;
		double	dD2			= dYsu1;
		double	dDc2		= dD2 * dSumAs / dAv2;
		double	dDs2		= dD2 - dDc2;
		double	dYcu2		= dDc2 + dTc/2;
		double	dYcl2		= dYcu2 - dTc;
		double	dYsu2		= dDc2;
		double	dYsl2		= dYsu2 - (dHw + dTu + dTl);
		double	dIv2		= dIs + dIcPnc + dSumAs * pow(dDs2,2) + dConfAs/dNc * pow(dDc2,2);
		double	dFcu2		= dMph2 * dYcu2 / (dNc * dIv2);
		double	dFcl2		= dMph2 * dYcl2 / (dNc * dIv2);
		double	dFsu2		= dMph2 * dYsu2 / dIv2;
		double	dFsl2		= dMph2 * dYsl2 / dIv2;
		double	dK2			= dK1 + dBc*pow(dTc,3) / 3 / dNc;
		//double	dTau2		= dSph2 / dAsWeb;
		// Phase 3 - 턴오버 하중에 의한 응력
		double	dFcu3		= dMph3 * dYcu2 / (dNc * dIv2);
		double	dFcl3		= dMph3 * dYcl2 / (dNc * dIv2);
		double	dFsu3		= dMph3 * dYsu2 / dIv2;
		double	dFsl3		= dMph3 * dYsl2 / dIv2;
		// Phase 4 - 바닥판 콘크리트 자중에 의한 응력
		double	dFsu4		= dMph4 * dYsu2 / dIv2;
		double	dFsl4		= dMph4 * dYsl2 / dIv2;
		// Phase 5 - 2차 고정하중에 의한 응력
		double	dAv3		= dSumAs + dConfAs/dNc + dSlabAs/dNd;
		double	dIdPnd		= dSlabIx / dNd;
		double	dD3			= dYcu2 - dDist + dTd/2;
		double	dDd3		= dD3 * dAv2 / dAv3;
		double	dDsc3		= dD3 - dDd3;
		double	dYdu3		= dDd3 + dTd/2;
		double	dYdl3		= dYdu3 - dTd;
		double	dYcu3		= dYdl3 + dDist;
		double	dYcl3		= dYcu3 - dTc;
		double	dYsu3		= dYcu3 - dTc/2;
		double	dYsl3		= dYsu3 - (dHw + dTu + dTl);
		double	dIv3		= dIv2 + dAv2 * pow(dDsc3,2) + dIdPnd + dSlabAs/dNd * pow(dDd3,2);
		double	dFdu5		= dMph5 * dYdu3 / (dNd * dIv3);
		double	dFdl5		= dMph5 * dYdl3 / (dNd * dIv3);
		double	dFsu5		= dMph5 * dYsu3 / dIv3;
		double	dFsl5		= dMph5 * dYsl3 / dIv3;
		double	dK3			= dK2 + dBeff * pow(dTd,3) / 3 / dNd;

		m_MapSecStress["Yvcu2"]	= dYcu2;
		m_MapSecStress["Yvcl2"]	= dYcl2;
		m_MapSecStress["Yvsu2"]	= dYsu2;
		m_MapSecStress["Yvsl2"]	= dYsl2;

		m_MapSecStress["Yvcu"]	= dYcu3;
		m_MapSecStress["Yvcl"]	= dYcl3;
		m_MapSecStress["Yvsu"]	= dYsu3;
		m_MapSecStress["Yvsl"]	= dYsl3;
		m_MapSecStress["Yvdu"]	= dYdu3;
		m_MapSecStress["Yvdl"]	= dYdl3;

		// Phase 6 - 활 하 중
		double	dFsu6		= dMph6 * dYsu3 / dIv3;
		double	dFsl6		= dMph6 * dYsl3 / dIv3;
		// 풍하중
		double	dFsuw		= dMw * dYsu3 / dIv3;
		double	dFslw		= dMw * dYsl3 / dIv3;
		// 구속콘크리트의 크리프
		double	dNc_c		= dNc * (1.0 + dCreep1/2);
		double	dAv_cc		= dSumAs + dConfAs/dNc_c;
		double	dIcPnc_c	= dConfIx / dNc_c;
		double	dDc_c		= dYsu1;
		double	dDelta_c	= (dConfAs/dNc_c * dDc_c) / dAv_cc;
		double	dDs_c		= dDelta_c;
		double	ddc_c		= dYsu1 - dDelta_c;
		double	dYcu_cc		= ddc_c + dTc/2;
		//double	dYcl_cc		= dYcu_cc - dTc;
		double	dYsu_cc		= ddc_c;
		double	dYsl_cc		= dYsu_cc - (dHw + dTu + dTl);
		double	dIvc_c		= dIs + dSumAs*pow(dDs_c,2) + dIcPnc_c + dConfAs/dNc_c*pow(ddc_c,2);
		double	dSumFcu		= dFcu2 + dFcu3;
		double	dSumFcl		= dFcl2 + dFcl3;
		double	dN_c		= (dSumFcu+dSumFcl)/2 * dConfAs;
		double	dPpi		= dN_c * 2 * dCreep1 / (2+dCreep1);
		double	dMpi		= dPpi * ddc_c;
		//double	dEc_c		= dEc / (1 + dCreep1/2);
		double	dFsu_cc		= dPpi/dAv_cc + dK*dMpi*dYsu_cc/dIvc_c;
		double	dFsl_cc		= dPpi/dAv_cc + dK*dMpi*dYsl_cc/dIvc_c;
		// 구속콘크리트의 건조수축
		double	dNc_s		= dNc * (1+dCreep2/2);
		double	dAv_cs		= dSumAs + dConfAs/dNc_s;
		double	dIcPnc_s	= dConfIx / dNc_s;
		double	dDc_s		= dYsu1;
		double	dDeltacs	= (dConfAs/dNc_s * dDc_s) / dAv_cs;
		double	dDs_s		= dDeltacs;
		double	ddc_s		= dYsu1 - dDeltacs;
		double	dYcu_cs		= ddc_s + dTc/2;
		//double	dYcl_cs		= dYcu_cs - dTc;
		double	dYsu_cs		= ddc_s;
		double	dYsl_cs		= dYsu_cs - (dHw + dTu + dTl);
		double	dIvc_s		= dIs + dSumAs*pow(dDs_s,2) + dIcPnc_s + dConfAs/dNc_s*pow(ddc_s,2);
		double	dPc_s		= dEs * dEpsilon * dConfAs/dNc_s;
		double	dMv_s		= dPc_s * ddc_s;
		//double	dEc_s		= dEs / dNc_s;
		double	dFsu_cs		= dPc_s/dAv_cs + dK * dMv_s * dYsu_cs/dIvc_s;
		double	dFsl_cs		= dPc_s/dAv_cs + dK * dMv_s * dYsl_cs/dIvc_s;
		// 바닥판 콘크리트의 크리프
		double	dNd_c		= dNd * (1 + dCreep1/2);
		double	dAv_dc		= dSumAs + dConfAs/dNc + dSlabAs/dNd_c;
		double	dIdPnd_c	= dSlabIx/dNd_c;
		double	dDd_dc		= dD3;
		double	ddd_dc		= dDd_dc * dAv2 / dAv_dc;
		double	dDsc3_c		= dDd_dc - ddd_dc;
		double	dYdu_dc		= ddd_dc + dTd/2;
		double	dYdl_dc		= dYdu_dc - dTd;
		double	dYcu_dc		= dYdl_dc + dDist;
		double	dYsu_dc		= dYcu_dc - dTc/2;
		double	dYsl_dc		= dYsu_dc - (dHw + dTu + dTl);
		double	dIv_dc		= dIv2 + dAv2 * pow(dDsc3_c,2) + dIdPnd_c + dSlabAs/dNd_c*pow(ddd_dc,2);
		//double	dSumFdu		= dFdu5;
		//double	dSumFdl		= dFdl5;
		double	dNc_dc		= dMph5 * dDd3 * dSlabAs / (dNd * dIv3);
		double	dPpi_dc		= dNc_dc * 2 * dCreep1 / (2 + dCreep1);
		double	dMpi_dc		= dPpi_dc * ddd_dc;
		//double	dEd1		= dEd / (1 + dCreep1/2);
		double	dFsu_dc		= dPpi_dc / dAv_dc + dK * dMpi_dc * dYsu_dc/dIv_dc;
		double	dFsl_dc		= dPpi_dc / dAv_dc + dK * dMpi_dc * dYsl_dc/dIv_dc;
		// 바닥판 콘크리트의 건조수축
		double	dNd_s		= dNd * (1 + dCreep2/2);
		double	dAv_ds		= dSumAs + dConfAs/dNc + dSlabAs/dNd_s;
		double	dIdPnd_s	= dSlabIx/dNd_s;
		double	dDd_ds		= dD3;
		double	ddd_ds		= dDd_ds * dAv2/dAv_ds;
		double	dDsc3_s		= dDd_ds - ddd_ds;
		double	dYdu_ds		= ddd_ds +dTd/2;
		double	dYdl_ds		= dYdu_ds - dTd;
		double	dYcu_ds		= dYdl_ds + dDist;
		double	dYsu_ds		= dYcu_ds - dTc/2;
		double	dYsl_ds		= dYsu_ds - (dHw + dTu + dTl);
		double	dIv_ds		= dIv2 + dAv2 * pow(dDsc3_s,2) + dIdPnd_s + dSlabAs/dNd_s*pow(ddd_ds,2);
		double	dP2			= dEs * dEpsilon * dSlabAs/dNd_s;
		double	dMv2		= dP2 * ddd_ds;
		//double	dEc2		= dEs/dNd_s;
		double	dFsu_ds		= dP2 / dAv_ds + dK * dMv2 * dYsu_ds / dIv_ds;
		double	dFsl_ds		= dP2 / dAv_ds + dK * dMv2 * dYsl_ds / dIv_ds;
		// 바닥판 콘크리트의 온도차
		double	dEpsT		= dAlpha * dTemp;
		double	dP1			= dEs * dEpsT * dSlabAs / dNd;
		double	dMv_t		= dP1 * dDd3;
		double	dFsu_dt		= dP1/dAv3 + dK * dMv_t * dYsu3 / dIv3;
		double	dFsl_dt		= dP1/dAv3 + dK * dMv_t * dYsl3 / dIv3;
		// 전단 및 비틀림
		double	dShear		= (ABS(dSph1) + ABS(dSph2) + ABS(dSph3) + ABS(dSph4) + ABS(dSph5) + ABS(dSph6))/dAsWeb;
		double	dSheart		= ABS(dMst+dMsct+dMvt) * dTw / dK3;
		double	dShearMax	= MAX(dShear, dSheart);
		double	dShearWind	= (ABS(dSph1) + ABS(dSph2) + ABS(dSph3) + ABS(dSph4) + ABS(dSph5))*dAsWeb + (dMst+dMsct+dMwt)*dTw/dK3;	// 활하중 비재하시
		double	dShearWindL	= (ABS(dSph1) + ABS(dSph2) + ABS(dSph3) + ABS(dSph4) + ABS(dSph5/2) + ABS(dSph6))*dAsWeb + (dMst+dMsct+dMvt+dMwt/2)*dTw/dK3;	// 활하중 재하시

		m_MapSecStress["Tau_shear"]		= dShearMax;//dShear;
		m_MapSecStress["Tau_shear_w"]	= dShearWind;
		m_MapSecStress["Tau_shear_wl"]	= dShearWindL;

		m_MapSecStress["Fsu1"]			= dFsu1;	m_MapSecStress["Fsl1"]			= dFsl1;
		m_MapSecStress["Fsu2"]			= dFsu2;	m_MapSecStress["Fsl2"]			= dFsl2;
		m_MapSecStress["Fsu3"]			= dFsu3;	m_MapSecStress["Fsl3"]			= dFsl3;
		m_MapSecStress["Fsu4"]			= dFsu4;	m_MapSecStress["Fsl4"]			= dFsl4;
		m_MapSecStress["Fsu5"]			= dFsu5;	m_MapSecStress["Fsl5"]			= dFsl5;
		m_MapSecStress["Fsu6"]			= dFsu6;	m_MapSecStress["Fsl6"]			= dFsl6;
		m_MapSecStress["Fsu_cc"]		= dFsu_cc;	m_MapSecStress["Fsl_cc"]		= dFsl_cc;
		m_MapSecStress["Fsu_cs"]		= dFsu_cs;	m_MapSecStress["Fsl_cs"]		= dFsl_cs;
		m_MapSecStress["Fsu_dc"]		= dFsu_dc;	m_MapSecStress["Fsl_dc"]		= dFsl_dc;
		m_MapSecStress["Fsu_ds"]		= dFsu_ds;	m_MapSecStress["Fsl_ds"]		= dFsl_ds;
		m_MapSecStress["Fsu_dt"]		= dFsu_dt;	m_MapSecStress["Fsl_dt"]		= dFsl_dt;
		if(bWind)
		{
			m_MapSecStress["Fsuw"]		= dFsuw;
			m_MapSecStress["Fslw"]		= dFslw;
		}
	}
	else
	{
		// Phase 2 - 구속콘크리트 자중에 의한 응력 - TU연속교에서는 사용하지 않는다 (Phase 3에 합산)
		double	dFsu2	= dMph2 * dYsu1 / dIs;
		double	dFsl2	= dMph2 * dYsl1 / dIs;
		// Phase 3 - 바닥판 콘크리트 자중에 의한 응력
		double	dFsu3	= dMph3 * dYsu1 / dIs;
		double	dFsl3	= dMph3 * dYsl1 / dIs;
		//double	dK2		= dK1 + dBc * pow(dTc,3)/3/dNc;
		// Phase 4 - 2차 고정하중에 의한 응력
		double	dAru	= pData->m_Asuse1 * dBeff / 1000;
		double	dArl	= pData->m_Asuse2 * dBeff / 1000;
		double	dAs		= dSumAs;
		double	dAr		= dAru + dArl;
		double	dAv		= dAs + dAr;
		double	dD		= dYsu1 + dTc/2 - dDist + dCl + (dTd-dCu-dCl) * dAru / dAr;
		double	dDr		= dD * dAs / (dAs + dAr);
		double	dDs		= dD - dDr;
		double	dYvru	= dDr + (dTd - dCu - dCl) * dArl / dAr;
		double	dYvrl	= dDr - (dTd - dCu - dCl) * dAru / dAr;
		double	dYvsu	= dYvrl - dCl + dDist - dTc/2;
		double	dYvsl	= dYvsu - (dHw + dTl + dTu);
		double	dIrv	= dIs + dAs * pow(dDs,2) + dAr * pow(dDr,2);
		double	dFvsu4	= dMph4 * dYvsu / dIrv;
		double	dFvsl4	= dMph4 * dYvsl / dIrv;
		double	dK3		= dK1 + dBc * pow(dTc,3)/3/dNc + dBeff*pow(dTd,3)/3/dNd;
	
		m_MapSecStress["Aru"]	= dAru;
		m_MapSecStress["Arl"]	= dArl;
		m_MapSecStress["Ar"]	= dAr;
		m_MapSecStress["ds"]	= dDs;
		m_MapSecStress["Yvru"]	= dYvru;
		m_MapSecStress["Yvrl"]	= dYvrl;
		m_MapSecStress["Yvsu"]	= dYvsu;
		m_MapSecStress["Yvsl"]	= dYvsl;
		m_MapSecStress["Irv"]	= dIrv;
		// Phase 5 - 활 하 중 + 지 점 침 하
		double	dFvsu5	= dMph5 * dYvsu / dIrv;
		double	dFvsl5	= dMph5 * dYvsl / dIrv;
		// 풍 하 중
		double	dFsuw	= dMw * dYvsu / dIrv;
		double	dFslw	= dMw * dYvsl / dIrv;
		// 구속콘크리트의 크리프
		double	dAc			= dConfAs;
		double	dAd			= dSlabAs;
		double	dAv2		= dAs + dAc/dNc;
		double	dIcPnc		= dConfIx / dNc;
		double	dD2			= dYsu1;
		double	dDc2		= dD2 * dAs / (dAc/dNc + dAs);
		double	dDs2		= dD2 - dDc2;
		double	dYcu2		= dDc2 + dTc/2;
		//double	dYcl2		= dYcu2 - dTc;
		double	dIv2		= dIs + dIcPnc + dAs*pow(dDs2,2) + dAc/dNc*pow(dDc2,2);
/*		double	dNc_c		= dNc * (1 + dCreep1/2);
		double	dAv_cc		= dAs + dAc/dNc_c + dAd/dNd;
		double	dIcPnc_c	= dConfIx / dNc_c;
		double	dDc_c		= dYsu1;
		double	dDd_c		= dDc_c + dTc/2 - dDist + dTd/2;
		double	dDeltac		= (dAd/dNd*dDd_c + dAc/dNc_c*dDc_c)/dAv_cc;
		double	dDs_c		= -dDeltac;
		double	ddc_c		= dYsu1 - dDeltac;
		double	ddd_c		= ddc_c + dTc/2 - dDist + dTd/2;
		double	dYsu_cc		= ddc_c;
		double	dYsl_cc		= dYsu_cc - (dHw + dTu + dTl);
		double	dIvc_c		= dIs + dAs*pow(dDs_c,2) + dIcPnc_c + dAc/dNc_c*pow(ddc_c,2) + dSlabIx/dNd + dAd/dNd*pow(ddd_c,2);
		double	dNc_cc		= (dSumFcu+dSumFcl)/2 * dAc;
		double	dPpi		= dNc_cc * 2 * dCreep1 / (2+dCreep1);
		double	dMpi		= dPpi * ddc_c;
		double	dEc_c		= dEc / (1+dCreep1/2);
		double	dFru_cc		= dPpi / dAv + dK*dMpi*dYvru/dIrv;
		double	dFrl_cc		= dPpi / dAv + dK*dMpi*dYvrl/dIrv;
		double	dFsu_cc		= dPpi / dAv + dK*dMpi*dYvsu/dIrv;
		double	dFsl_cc		= dPpi / dAv + dK*dMpi*dYvsl/dIrv;
		// 구속콘크리트의 건조수축
		double	dNc_s		= dNc*(1+dCreep2/2);
		double	dAv_cs		= dAs + dAc/dNc_s + dAd/dNd;
		double	dIcPnc_s	= dConfIx / dNc_s;
		double	dDc_s		= dYsu1;
		double	dDd_s		= dDc_s + dTc/2 - dDist + dTd/2;
		double	dDelta_s	= (dAd/dNd*dDd_s + dAc/dNc_s*dDc_s) / dAv_cs;
		double	dds_s		= -dDelta_s;
		double	ddc_s		= dYsu1 - dDelta_s;
		double	ddd_s		= ddc_s + dTc/2 - dDist + dTd/2;
		double	dYcu_cs		= ddc_s + dTc/2;
		double	dYcl_cs		= dYcu_cs - dTc;
		double	dYsu_cs		= ddc_s;
		double	dYsl_cs		= dYsu_cs - (dHw + dTu + dTl);
		double	dIvc_s		= dIs + dAs*pow(dds_s,2) + dIcPnc_s + dAc/dNc_s*pow(ddc_s,2) + dSlabIx/dNd + dAd/dNd*pow(ddd_s,2);
		double	dPc_s		= dEs * dEpsilon * dAc/dNc_s;
		double	dMv_s		= dPc_s * ddc_s;
		double	dEc_s		= dEs/dNc_s;
		double	dFru_cs		= dPc_s/dAv + dK * dMv_s * dYvru/dIrv;
		double	dFrl_cs		= dPc_s/dAv + dK * dMv_s * dYvrl/dIrv;
		double	dFsu_cs		= dPc_s/dAv + dK * dMv_s * dYvsu/dIrv;
		double	dFsl_cs		= dPc_s/dAv + dK * dMv_s * dYvsl/dIrv;
*/		// 바닥판 콘크리트의 크리프
		double	dAv3		= dAv2 + dAd/dNd;
		double	dIdPnd		= dSlabIx / dNd;
		double	dD3			= dYcu2 - dDist + dTd/2;
		double	ddd3		= dD3 * dAv2 / dAv3;
		double	ddsc3		= dD3 - ddd3;
		double	dYdu3		= ddd3 + dTd/2;
		//double	dYdl3		= dYdu3 - dTd;
		double	dIv3		= dIv2 + dAv2 * pow(ddsc3,2) + dIdPnd + dAd/dNd * pow(ddd3,2);
		double	dNd_c		= dNd*(1+dCreep1/2);
		double	dAv_dc		= dAs + dAc/dNc + dAd/dNd_c;
		double	dDd_dc		= dD3;
		double	ddd_dc		= dDd_dc * dAv2 / dAv_dc;
		double	dYdu_dc		= ddd_dc + dTd/2;
		double	dYdl_dc		= dYdu_dc - dTd;
		double	dYcu_dc		= dYdl_dc + dDist;
		//double	dYsu_dc		= dYcu_dc - dTc/2;
		double	dNc_dc		= dMph5 * ddd3 * dAd/(dNd*dIv3);
		double	dPpi_dc		= dNc_dc * 2 * dCreep1 / (2+dCreep1);
		double	dMpi_dc		= dPpi_dc * ddd_dc;
		double	dFsu_dc		= dPpi_dc / dAv + dK * dMpi_dc * dYvsu / dIrv;
		double	dFsl_dc		= dPpi_dc / dAv + dK * dMpi_dc * dYvsl / dIrv;
		// 바닥판 콘크리트의 건조수축
		double	dNd_s		= dNd * (1 + dCreep2/2);
		double	dAv_ds		= dAs + dAc/dNc + dAd/dNd_s;
		//double	dIdPnd_s	= dSlabIx / dNd_s;
		double	dDd_ds		= dD3;
		double	ddd_ds		= dDd_ds * dAv2 / dAv_ds;
		//double	dDsc3_ds	= dDd_ds - ddd_ds;
		double	dYdu_ds		= ddd_ds + dTd/2;
		double	dYdl_ds		= dYdu_ds - dTd;
		double	dYcu_ds		= dYdl_ds + dDist;
		//double	dYsu_ds		= dYcu_ds - dTc/2;
		double	dP2			= dEs * dEpsilon * dAd / dNd_s;
		double	dMv2		= dP2 * ddd_ds;
		double	dFsu_ds		= dP2 / dAv + dK * dMv2 * dYvsu / dIrv;
		double	dFsl_ds		= dP2 / dAv + dK * dMv2 * dYvsl / dIrv;
		// 바닥판 콘크리트의 온도차
		double	dEpsT		= dAlpha * dTemp;
		double	dP1			= dEs * dEpsT * dAd / dNd;
		double	dMv_t		= dP1 * ddd3;
		//double	dErt		= dEs * dEpsT;
		double	dFsu_dt		= dP1 / dAv + dK * dMv_t * dYvsu/dIrv;
		double	dFsl_dt		= dP1 / dAv + dK * dMv_t * dYvsl/dIrv;
		// 전단 및 비틀림
		double	dMt			= dMst + dMsct + dMvt;
		double	dShear		= (ABS(dSph1) + ABS(dSph2) + ABS(dSph3) + ABS(dSph4) + ABS(dSph5) + ABS(dSph6))/dAsWeb;
		double	dSheart		= ABS(dMt) * dTw / dK3;
		double	dShearMax	= MAX(dShear, dSheart);
		double	dShearWind	= (ABS(dSph1) + ABS(dSph2) + ABS(dSph3) + ABS(dSph4))*dAsWeb + (dMst+dMsct+dMwt)*dTw/dK3;	// 활하중 비재하시
		double	dShearWindL	= (ABS(dSph1) + ABS(dSph2) + ABS(dSph3) + ABS(dSph4) + ABS(dSph5/2))*dAsWeb + (dMst+dMsct+dMvt+dMwt/2)*dTw/dK3;	// 활하중 재하시

		m_MapSecStress["Tau_shear"]		= dShearMax;//dShear;
		m_MapSecStress["Tau_shear_w"]	= dShearWind;
		m_MapSecStress["Tau_shear_wl"]	= dShearWindL;

		m_MapSecStress["Fsu1"]			= dFsu1;	m_MapSecStress["Fsl1"]			= dFsl1;
		m_MapSecStress["Fsu2"]			= dFsu2;	m_MapSecStress["Fsl2"]			= dFsl2;
		m_MapSecStress["Fsu3"]			= dFsu3;	m_MapSecStress["Fsl3"]			= dFsl3;
		m_MapSecStress["Fvsu_Deck"]		= dFvsu4;	m_MapSecStress["Fvsl_Deck"]		= dFvsl4;
		m_MapSecStress["Fvsu_Live"]		= dFvsu5;	m_MapSecStress["Fvsl_Live"]		= dFvsl5;
		m_MapSecStress["Fsu_dc"]		= dFsu_dc;	m_MapSecStress["Fsl_dc"]		= dFsl_dc;
		m_MapSecStress["Fsu_ds"]		= dFsu_ds;	m_MapSecStress["Fsl_ds"]		= dFsl_ds;
		m_MapSecStress["Fsu_dt"]		= dFsu_dt;	m_MapSecStress["Fsl_dt"]		= dFsl_dt;

		if(bWind)
		{
			m_MapSecStress["Fsuw"]		= dFsuw;
			m_MapSecStress["Fslw"]		= dFslw;
		}
	}
}
///////////////////////////////////////////////////////////////////////
///  단면 검토를 위한 각종 응력값들 구하기
///		sType  : 계산서 응력 기호    (예) "Sigma_su"
///////////////////////////////////////////////////////////////////////
///  1) 정모멘트부 : 합계하중 최대, 최소중 절대값이 큰 값이 양수인경우
///  2) 부모멘트부 : 합계하중 최대, 최소중 절대값이 큰 값이 음수인경우  
///  3) 활하중 검토 모멘트 : 정 모멘트시 활하중 MAX값
///					 		 부 모멘트시 활하중 MIN값			         
///  4) 풍하중 검토 모멘트 : 정 모멘트시 풍하중 MAX값
///                          부 모멘트시 풍하중 MIN값			         
//////////////////////////////////////////////////////////////////////
///  5) 활하중 검토 전단력 : 절대값이 큰 값을 사용함
///  6) 풍하중 검토 전단력 : 활하중 검토 전단력이 정 값일 경우 +값 중 큰값
///							 활하중 검토 전단력이 부 값일 경우 -값 중 큰값	
///////////////////////////////////////////////////////////////////////
void CFemStressCalc::GetSectionStress_GenGirder(long nG, double dSta, BOOL bHAPSUNG, double dStaSec, long nPosSec)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	m_MapSecStress.clear();

	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx     = pGir->GetBxByStationDir(dSta, 0);
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CCalcGeneral	 CalcGen(m_pDataManage);
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();

	BOOL bUpperInner = (pOpt->GetDesignHeigtBaseUpper() == 1); //상판내측
	///<   kN.m --> N.mm
	///< 하중별 모멘트 **********************************************************************************
	BOOL	bPositiveM	= IsPositive(ELE_MOMENT3, nG, dSta);
	BOOL	bPositiveS	= IsPositive(ELE_SHEAR_2, nG, dSta);
	BOOL	bPositiveT	= IsPositive(ELE_AXTORSN, nG, dSta);

	double	dMomentPreDead	= frkNM(GetElementForce(ELE_MOMENT3, FRAME_PRE_DEADLOAD,  nG, dSta,0));
	double	dMomentPostDead	= frkNM(GetElementForce(ELE_MOMENT3, FRAME_NEXT_DEADLOAD, nG, dSta,0));///< 합성후 사하중 휨모멘트
	double	dMomentSumMax	= frkNM(GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, dSta,0));///< 합계 최대모멘트
	double	dMomentSumMin	= frkNM(GetElementForce(ELE_MOMENT3, LC_SUM_MIN, nG, dSta,0));///< 합계 최소모멘트
	double	dMomentWind		= frkNM(GetElementForce(ELE_MOMENT3, bPositiveM ? LC_WIND_MAX : LC_WIND_MIN, nG, dSta,0));
	double	dMomentLive		= frkNM(GetElementForce(ELE_MOMENT3, bPositiveM ? LC_ONLYLIVE_MAX | LC_DISP_MAX : LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));

	///< 하중별 전단력 **********************************************************************************
	///< kN ---> N
	double	dShearPreDead	= frkN(GetElementForce(ELE_SHEAR_2, FRAME_PRE_DEADLOAD, nG, dSta,0));		// 합성전 사하중 전단력
	double	dShearPostDead	= frkN(GetElementForce(ELE_SHEAR_2, FRAME_NEXT_DEADLOAD, nG, dSta,0));		// 합성후 사하중 전단력
	double	dShearSumMax	= frkN(GetElementForce(ELE_SHEAR_2, LC_SUM_MAX, nG, dSta,0));
	double	dShearSumMin	= frkN(GetElementForce(ELE_SHEAR_2, LC_SUM_MIN, nG, dSta,0));	
	double	dShearLive		= frkN(GetElementForce(ELE_SHEAR_2, bPositiveS ? LC_ONLYLIVE_MAX | LC_DISP_MAX : LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));
	double	dShearWind      = frkN(GetElementForce(ELE_SHEAR_2, bPositiveS ? LC_WIND_MAX : LC_WIND_MIN, nG, dSta,0));

	///<   kN.m --> N.mm
	///< 하중별 비틀림 모멘트 **********************************************************************************
	double	dTorsionPreDead		= frkNM(GetElementForce(ELE_AXTORSN, FRAME_PRE_DEADLOAD, nG, dSta,0));		// 합성전 사하중 비틀림 모멘트
	double	dTorsionPostDead	= frkNM(GetElementForce(ELE_AXTORSN, FRAME_NEXT_DEADLOAD, nG, dSta,0));	// 합성후 사하중 비틀림 모멘트
	double	dTorsionSumMax		= frkNM(GetElementForce(ELE_AXTORSN, LC_SUM_MAX, nG, dSta,0));
	double	dTorsionSumMin		= frkNM(GetElementForce(ELE_AXTORSN, LC_SUM_MIN, nG, dSta,0));
	double	dTorsionSum			= bPositiveT ? dTorsionSumMax : dTorsionSumMin;
	double	dTorsionLive		= frkNM(GetElementForce(ELE_AXTORSN, bPositiveT ? LC_ONLYLIVE_MAX | LC_DISP_MAX : LC_ONLYLIVE_MIN | LC_DISP_MIN, nG, dSta,0));
	double	dTorsionWind		= frkNM(GetElementForce(ELE_AXTORSN, bPositiveT ? LC_WIND_MAX : LC_WIND_MIN, nG, dSta,0));

	///< 영향계수 및 기타        **********************************************************************************
	double dK			= ModelCalc.GetValueOfInfK(nG, dSta);		///< 영향계수
	double dEst			= pData->DESIGN_MATERIAL.m_dEst;            ///< 강재의 탄성계수
	double dEc			= pData->DESIGN_MATERIAL.m_dEc;             ///< 콘크리트의 탄성계수 
	double dN			= pData->DESIGN_MATERIAL.m_nN;                    ///< 탄성계수비(철근/콘크리트)
	double dEpsilon		= pData->DESIGN_MATERIAL.m_dEpsilonS;      ///< 건조수축계수
	double dAlpha		= pData->DESIGN_MATERIAL.m_dAlpha;         ///< 선팽창계수
	double dTHunch		= pDB->m_dThickSlabHunch;		         ///< 헌치 높이
	double dTSlabBase	= pDB->m_dThickSlabBase;		         ///< 슬래브 기본두께
	double dCreepCoef1	= pData->DESIGN_MATERIAL.m_dCreep;         ///< 크리프
	double dCreepCoef2	= pData->DESIGN_MATERIAL.m_dCreep2;
	double dTemperature	= pData->DESIGN_MATERIAL.m_dTemparature;  ///< 강과 콘크리트의 온도차 

	// 합성전
	// (APLATE-680) 두개의 Sta가 잘못 들어가면 안된다. 인자로 들어온 Sta는 부재력위치, 
	// 이함수에는 첫번째 sta로 단면위치가 들어가야 되므로 dStaSec가 들어가는게 맞다.
	double	dStaCoeff	= dStaSec!=0 ? dStaSec : dSta;
	ModelCalc.GetSectionCoefficient(nG, dStaCoeff, FALSE, dSta, nPosSec);

	double dI33A       = ModelCalc.m_MapSecCoe["I33"];
	double dYsuPreA    = ModelCalc.m_MapSecCoe["Ysu"];
	double dYslPreA    = ModelCalc.m_MapSecCoe["Ysl"];
	double dAsWebA     = ModelCalc.m_MapSecCoe["As_WEB"];
	double dThickSlab	= ModelCalc.m_MapSecCoe["SLAB_THICK"];
	double dUp_f_w		= ModelCalc.m_MapSecCoe["FLANGE_W_UP"];
	double dUp_f_t		= ModelCalc.m_MapSecCoe["FLANGE_T_UP"];
	double dLo_f_w		= ModelCalc.m_MapSecCoe["FLANGE_W_DN"];
	double dLo_f_t		= ModelCalc.m_MapSecCoe["FLANGE_T_DN"];
	double dWeb_h		= ModelCalc.m_MapSecCoe["WEB_H"];
	double dWeb_t		= ModelCalc.m_MapSecCoe["WEB_T"];
	double dThickHunch	= ModelCalc.m_MapSecCoe["HUNCH"];
	double dTSlab		= ModelCalc.m_MapSecCoe["SLAB_THICK"];
	// 합성후  - 슬래브 유효폭 고려
	ModelCalc.GetSectionCoefficient(nG, dStaCoeff, TRUE, dSta, nPosSec);
	double dI33       = ModelCalc.m_MapSecCoe["I33"];
	double dYsuPre    = ModelCalc.m_MapSecCoe["Ysu"];
	double dYslPre    = ModelCalc.m_MapSecCoe["Ysl"];
	double dAsWeb     = ModelCalc.m_MapSecCoe["As_WEB"];
	double dWEffectSlab		= ModelCalc.GetEffectWidthSlab(nG, dSta);
	double dAsSlab    = ModelCalc.m_MapSecCoe["As_SLAB"]; ///<콘크리트단면
	double dAsGirder  = ModelCalc.m_MapSecCoe["As_SUM"];  ///<강재단면
	double dAsSumPost = ModelCalc.m_MapSecCoe["Av_SUM"]; ///<강재단면+콘크리트단면/n
	double dIv33      = ModelCalc.m_MapSecCoe["Iv33"];
	double dYcuPost   = ModelCalc.m_MapSecCoe["Yvcu"]; 
	double dYclPost   = ModelCalc.m_MapSecCoe["Yvcl"];
	double dYsuPost   = ModelCalc.m_MapSecCoe["Yvsu"];
	double dYslPost   = ModelCalc.m_MapSecCoe["Yvsl"];
	double ddV        = ModelCalc.m_MapSecCoe["dv"];
	double ddVc       = ModelCalc.m_MapSecCoe["dvc"];
	double dIgc       = ModelCalc.m_MapSecCoe["Igc"];
	double dKK        = ModelCalc.m_MapSecCoe["K"];  ///< 합성전
	double dKKv       = ModelCalc.m_MapSecCoe["Kv"]; ///< 합성후
	
	m_MapSecStress["MomentPreDead"]  = dMomentPreDead;  
	m_MapSecStress["MomentPostDead"] = dMomentPostDead; 
	m_MapSecStress["MomentSumMax"]   = dMomentSumMax;   
	m_MapSecStress["MomentSumMin"]   = dMomentSumMin;   
	m_MapSecStress["MomentWind"]	 = dMomentWind;     
	m_MapSecStress["MomentLive"]	 = dMomentLive;     
	m_MapSecStress["ShearPreDead"]   = dShearPreDead;   
	m_MapSecStress["ShearPostDead"]  = dShearPostDead;  
	m_MapSecStress["ShearSumMax"]	 = dShearSumMax;    
	m_MapSecStress["ShearSumMin"]	 = dShearSumMin;    
	m_MapSecStress["ShearWind"]		 = dShearWind;      
	m_MapSecStress["ShearLive"]		 = dShearLive;      
	m_MapSecStress["TorsionPreDead"] = dTorsionPreDead; 
	m_MapSecStress["TorsionPostDead"]= dTorsionPostDead;
	m_MapSecStress["TorsionSumMax"]  = dTorsionSumMax;  
	m_MapSecStress["TorsionSumMin"]  = dTorsionSumMin;  
	m_MapSecStress["TorsionSum"]	= dTorsionSum;

	m_MapSecStress["TorsionWind"]	 = dTorsionWind;    
	m_MapSecStress["TorsionLive"]	 = dTorsionLive;    
	m_MapSecStress["K"]				 = dK;
	m_MapSecStress["N"]				 = dN;

	// 하이브리드 계수 계산
	double dHy_RPre = 1, dHy_RPos = 1;
	///< 비틀림에 대한 추가 전단응력은 순수비틀림의 경우만 생각함.
	if(bHAPSUNG)		
	{
		///< 합성전 사하중은 항상 비합성단면임.
		double Sigma_su = dMomentPreDead / dI33A * dYsuPreA;	m_MapSecStress["Sigma_su"] = Sigma_su;// 합성전 상판 응력
		double Sigma_sl = dMomentPreDead / dI33A * dYslPreA;	m_MapSecStress["Sigma_sl"] = Sigma_sl;// 합성전 하판 응력
		double Tau_s    = fabs(dShearPreDead)/ dAsWebA;			m_MapSecStress["Tau_s"]    = Tau_s+fabs(dTorsionPreDead)*dWeb_t/dKK;// 합성전 전단응력

		if(bPositiveM) // 정모멘트
		{
//			double	dAs = dAsGirder;
//			double	dAc = dWEffectSlab*dThickSlab;
//			double	dAv = dAs + dAc/dN;
//			double	dIc	= dAc * pow(dThickSlab,3)/12;
//			double	dIcPerN	= dIc/dN;
//			double	dDv	= dYsuPre - dUp_f_t + dThickHunch + dThickSlab/2;
//			double	dDvc = dDv * dAs/dAv;
//			double	dDvs = dDv - dDvc;
//			double dYvcu = dDvc + dThickSlab/2;								m_MapSecStress["Yvcu"]	= dYvcu;
			double dYvcu = ddVc + dThickSlab/2;								m_MapSecStress["Yvcu"]	= dYvcu;
			double dYvcl = dYvcu - dThickSlab;								m_MapSecStress["Yvcl"]	= dYvcl;
			double dYvsu = bUpperInner ? dYvcu - (dThickSlab+dThickHunch) + dUp_f_t : dYvcu - (dThickSlab+dThickHunch); m_MapSecStress["Yvsu"]	= dYvsu;
			double dYvsl = dYvsu - (dWeb_h + dUp_f_t + dLo_f_t);			m_MapSecStress["Yvsl"]	= dYvsl;
//			double dIv	= dI33+dIcPerN+dAs*pow(dDvs,2)+dAc/dN*pow(dDvc,2);	m_MapSecStress["Iv"]	= dIv;

			double Sigma_vcu	= dMomentPostDead / dIv33 * dYcuPost / dN;		m_MapSecStress["Sigma_vcu"] = Sigma_vcu;// 상판 응력(콘크리트 포함)
			double Sigma_vcl	= dMomentPostDead / dIv33 * dYclPost / dN;		m_MapSecStress["Sigma_vcl"] = Sigma_vcl;// 하판 응력(콘크리트 포함)
			double Sigma_vsu	= dMomentPostDead / dIv33 * dYsuPost;			m_MapSecStress["Sigma_vsu"] = Sigma_vsu;// 상판 응력
			double Sigma_vsl	= dMomentPostDead / dIv33 * dYslPost;			m_MapSecStress["Sigma_vsl"] = Sigma_vsl;// 하판 응력
			double Tau_sv		= fabs(dShearPostDead) / dAsWeb;				m_MapSecStress["Tau_sv"]    = Tau_sv+fabs(dTorsionPostDead)*dWeb_t/dKKv;// 전단 응력
			// < 활하중에 의한 응력 >	
			double Sigma_vcu_l = (dMomentLive) / dIv33 * dYcuPost / dN;		m_MapSecStress["Sigma_vcu_l"] = Sigma_vcu_l;// 상판 응력(콘크리트 포함)
			double Sigma_vcl_l = (dMomentLive) / dIv33 * dYclPost / dN;		m_MapSecStress["Sigma_vcl_l"] = Sigma_vcl_l;// 하판 응력(콘크리트 포함)
			double Sigma_vsu_l = (dMomentLive) / dIv33 * dYsuPost;			m_MapSecStress["Sigma_vsu_l"] = Sigma_vsu_l;// 상판 응력
			double Sigma_vsl_l = (dMomentLive) / dIv33 * dYslPost;			m_MapSecStress["Sigma_vsl_l"] = Sigma_vsl_l;// 하판 응력
			double Tau_sv_l    = fabs(dShearLive) / dAsWeb;					m_MapSecStress["Tau_sv_l"]    = Tau_sv_l+fabs(dTorsionLive)*dWeb_t/dKKv;// 전단 응력
			/*
			// < 지점침하에 의한 응력 >	
			double Sigma_vcu_j = dMomentDisp / dIv33 * dYcuPost / dN;		m_MapSecStress["Sigma_vcu_j"] = Sigma_vcu_j;// 상판 응력(콘크리트 포함)
			double Sigma_vcl_j = dMomentDisp / dIv33 * dYclPost / dN;		m_MapSecStress["Sigma_vcl_j"] = Sigma_vcl_j;// 하판 응력(콘크리트 포함)
			double Sigma_vsu_j = dMomentDisp / dIv33 * dYsuPost;			m_MapSecStress["Sigma_vsu_j"] = Sigma_vsu_j;// 상판 응력
			double Sigma_vsl_j = dMomentDisp / dIv33 * dYslPost;			m_MapSecStress["Sigma_vsl_j"] = Sigma_vsl_j;// 하판 응력
			double Tau_sv_j    = fabs(dShearDisp) / dAsWeb;					m_MapSecStress["Tau_sv_j"]    = Tau_sv_j+fabs(dTorsionDisp)*dWeb_t/dKKv;// 전단 응력
			*/
			// < 풍하중에 의한 응력 >
			double Sigma_vcu_w = dMomentWind / dIv33 * dYcuPost / dN;		m_MapSecStress["Sigma_vcu_w"] = Sigma_vcu_w;// 상판 응력(콘크리트 포함)
			double Sigma_vcl_w = dMomentWind / dIv33 * dYclPost / dN;		m_MapSecStress["Sigma_vcl_w"] = Sigma_vcl_w;// 하판 응력(콘크리트 포함)
			double Sigma_vsu_w = dMomentWind / dIv33 * dYsuPost;			m_MapSecStress["Sigma_vsu_w"] = Sigma_vsu_w;// 상판 응력
			double Sigma_vsl_w = dMomentWind / dIv33 * dYslPost;			m_MapSecStress["Sigma_vsl_w"] = Sigma_vsl_w;// 하판 응력
			double Tau_sv_w    = fabs(dShearWind) / dAsWeb;					m_MapSecStress["Tau_sv_w"]    = Tau_sv_w+fabs(dTorsionWind)*dWeb_t/dKKv;// 전단 응력
			// < Creep에 의한 응력 >
			m_MapSecStress["CreepCoef1"] = dCreepCoef1;
			m_MapSecStress["Ec"] = dEc;
			double dEc1  = dEc / ( 1 + dCreepCoef1 / 2 );					m_MapSecStress["Ec1"] = dEc1;
			double dN1   = dN * ( 1 + dCreepCoef1 / 2 );					m_MapSecStress["n1"]  = dN1;///<탄성계수비
			double Av1   = dAsGirder + dAsSlab/dN1;							m_MapSecStress["Av1"] = Av1;
			double Dc1   = ddV*dAsGirder/(dAsSlab/dN1+dAsGirder);			m_MapSecStress["Dc1"] = Dc1;
			double Ds1   = ddV - Dc1;										m_MapSecStress["Ds1"] = Ds1;
			double Yv1cu = Dc1 + dTSlab/2;									m_MapSecStress["Yv1cu"] = Yv1cu;
			double Yv1cl = Yv1cu - dTSlab;									m_MapSecStress["Yv1cl"] = Yv1cl;
			double Yv1su = bUpperInner ? (Yv1cu - dTSlabBase + dUp_f_t) : (Yv1cu - dTSlabBase);	m_MapSecStress["Yv1su"] = Yv1su;
			double Yv1sl = Yv1su - (dWeb_h + dUp_f_t + dLo_f_t);		m_MapSecStress["Yv1sl"] = Yv1sl;
			double Iv1 = dI33 + dIgc/dN1	+ dAsGirder*Ds1*Ds1 + dAsSlab/dN1*Dc1*Dc1;	m_MapSecStress["Iv1"] = Iv1;
			double D   = dYsuPre + dTHunch + dTSlab /2;						m_MapSecStress["D"]  = D;	
			double Dc  = dYcuPost - dTSlab/2;								m_MapSecStress["Dc"] = Dc;	
			double Nc  = dMomentPostDead * ddVc * dAsSlab/ (dN*dIv33);		m_MapSecStress["Nc"] = Nc;
			double PCreep = Nc * 2 * dCreepCoef1 / (2 + dCreepCoef1);		m_MapSecStress["PCreep"] = PCreep;
			double MCreep = PCreep * Dc1;									m_MapSecStress["MCreep"] = MCreep;
			double Sigma_cu = dMomentPostDead*dYcuPost/(dN*dIv33);			m_MapSecStress["Sigma_cu"] = Sigma_cu;
			double Sigma_cl = dMomentPostDead*dYclPost/(dN*dIv33);			m_MapSecStress["Sigma_cl"] = Sigma_cl;
			double Sigma_cu1  = (PCreep/Av1 + dK*MCreep*Yv1cu/Iv1)/dN1 - dEc1*Sigma_cu*dCreepCoef1/dEc;	m_MapSecStress["Sigma_cu1"] = Sigma_cu1;
			double Sigma_cl1  = (PCreep/Av1 + dK*MCreep*Yv1cl/Iv1)/dN1 - dEc1*Sigma_cl*dCreepCoef1/dEc;	m_MapSecStress["Sigma_cl1"] = Sigma_cl1;
			double Sigma_vsu1 =  PCreep/Av1 + dK*MCreep*Yv1su/Iv1;					m_MapSecStress["Sigma_vsu1"] = Sigma_vsu1;
			double Sigma_vsl1 =  PCreep/Av1 + dK*MCreep*Yv1sl/Iv1;					m_MapSecStress["Sigma_vsl1"] = Sigma_vsl1;
			// < 건조 수축 >
			m_MapSecStress["Epsilon"] = dEpsilon;
			m_MapSecStress["CreepCoef2"] = dCreepCoef2;
			double dN2 = dN * ( 1 + dCreepCoef2/2);										m_MapSecStress["n2"]  = dN2;
			double dEc2= dEst / dN2;													m_MapSecStress["Ec2"] = dEc2;	
			double Av2 = dAsGirder + dAsSlab/dN2;										m_MapSecStress["Av2"] = Av2;
			double Dc2 = ddV*dAsGirder / (dAsSlab/dN2+dAsGirder);						m_MapSecStress["Dc2"] = Dc2;
			double Ds2 = ddV - Dc2;														m_MapSecStress["Ds2"] = Ds2;	
			double Iv2 = dI33 + dIgc/dN2 + dAsGirder*Ds2*Ds2 + dAsSlab/dN2*Dc2*Dc2;		m_MapSecStress["Iv2"] = Iv2;
			double Yv2cu = Dc2 + dTSlab/2;												m_MapSecStress["Yv2cu"] = Yv2cu;
			double Yv2cl = Yv2cu - dTSlab;												m_MapSecStress["Yv2cl"] = Yv2cl;
			double Yv2su = bUpperInner ? Yv2cu - dTSlabBase + dUp_f_t : Yv2cu - dTSlabBase;	m_MapSecStress["Yv2su"] = Yv2su;
			double Yv2sl = Yv2su - (dWeb_h + dUp_f_t + dLo_f_t);					m_MapSecStress["Yv2sl"] = Yv2sl;
			double PShrink = dEst * dEpsilon * dAsSlab / dN2;							m_MapSecStress["PShrink"] = PShrink;
			double MShrink = PShrink * Dc2;												m_MapSecStress["MShrink"] = MShrink;
			double Sigma_cu2 = (PShrink/Av2 + dK*MShrink*Yv2cu / Iv2) / dN2  - dEc2*dEpsilon;	m_MapSecStress["Sigma_cu2"] = Sigma_cu2;
			double Sigma_cl2 = (PShrink/Av2 + dK*MShrink*Yv2cl / Iv2) / dN2  - dEc2*dEpsilon;	m_MapSecStress["Sigma_cl2"] = Sigma_cl2;
			double Sigma_vsu2 = PShrink/Av2 + dK*MShrink*Yv2su / Iv2;					m_MapSecStress["Sigma_vsu2"] = Sigma_vsu2;
			double Sigma_vsl2 = PShrink/Av2 + dK*MShrink*Yv2sl / Iv2;					m_MapSecStress["Sigma_vsl2"] = Sigma_vsl2;
			// < 온도차 > 
			m_MapSecStress["Alpha"] = dAlpha;
			m_MapSecStress["Temperature"] = dTemperature;
			double Et  = dAlpha * dTemperature;											m_MapSecStress["Et"] = Et;
			double Dct = dYcuPost - dTSlab /2;											m_MapSecStress["Dct"] = Dct;
			double Ect = dEc * Et;														m_MapSecStress["Ect"] = Ect;
			double P1  = dEst * Et * dAsSlab / dN;										m_MapSecStress["P1"] = P1;			
			double Mt  = P1 * ddVc;														m_MapSecStress["Mt"] = Mt;
			double Sigma_cut = (P1/dAsSumPost + dK*Mt/dIv33* dYcuPost)/dN  - Ect;		m_MapSecStress["Sigma_cut"] = Sigma_cut;
			double Sigma_clt = (P1/dAsSumPost + dK*Mt/dIv33* dYclPost)/dN  - Ect;		m_MapSecStress["Sigma_clt"] = Sigma_clt;
			double Sigma_sut =  P1/dAsSumPost + dK*Mt/dIv33* dYsuPost;					m_MapSecStress["Sigma_sut"] = Sigma_sut;
			double Sigma_slt =  P1/dAsSumPost + dK*Mt/dIv33* dYslPost;					m_MapSecStress["Sigma_slt"] = Sigma_slt;
//			m_MapSecStress["Tau_shear"]		= Tau_shear;	// 전단 응력-풍하중 비재하시
//			m_MapSecStress["Tau_shear_w"]	= Tau_shear_w;	// 전단 응력(풍하중 재하시+활하중 비재하시)
//			m_MapSecStress["Tau_shear_wl"]	= Tau_shear_wl;	// 전단 응력(풍하중 재하시+활하중 재하시)
			double	dL		= pDB->m_dLengthBaseCrossBeam;	// 가로보 간격
			m_MapSecStress["CrossBeamTerm"]	= dL;
			// < 전단 및 비틀림 > - 전단 응력(풍하중 비재하시)
			double	dMt		= dTorsionPreDead+dTorsionPostDead+dTorsionLive;
			double	dWn		= dWeb_h*dUp_f_w/4.0;
			double	dH2		= dWeb_h+(dUp_f_t+dLo_f_t)/2.0;
			double	dIw		= (pow(dH2,2)*(dUp_f_t*pow(dUp_f_w,3)*dLo_f_t*pow(dLo_f_w,3)))/(12*(dUp_f_t*pow(dUp_f_w,3)+dLo_f_t*pow(dLo_f_w,3)));
			double	dA		= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			double	dMwa	= (dMt/dL)/pow(dA/dL,2) * (2*sinh(dA/2)/sinh(dA)-1);
			double	dTwa	= -(dMt/dL)/(dA/dL) * (1-cosh(dA))/sinh(dA);
			double	dSwa	= (dWeb_h*dUp_f_t*pow(max(dUp_f_w, dLo_f_w),2))/16;
			double	dNu		= (fabs(dShearPreDead)+fabs(dShearPostDead)+fabs(dShearLive))/dAsWeb;
			double	dNutWeb	= fabs(dMt)*dWeb_t/dKKv;
			double	dNutFL	= fabs(dMt)*dUp_f_t/dKKv;
			double	dFw		= fabs(dMwa) * dWn / dIw;
			double	dNuw	= fabs(dTwa)*dSwa / (dIw*dUp_f_t);
			m_MapSecStress["Mt"]		= dMt;
			m_MapSecStress["Wn"]		= dWn;
			m_MapSecStress["lw"]		= dIw;
			m_MapSecStress["Mwa"]		= dMwa;
			m_MapSecStress["Twa"]		= dTwa;
			m_MapSecStress["Swa"]		= dSwa;
			m_MapSecStress["Nu"]		= dNu;
			m_MapSecStress["Nut_Web"]	= dNutWeb;
			m_MapSecStress["Nut_FL"]	= dNutFL;
			m_MapSecStress["Nuw"]		= dNuw;
			m_MapSecStress["Nu_Web"]	= dNu+dNutWeb;
			m_MapSecStress["Nu_FL"]		= dNutFL+dNuw;			
//			m_MapSecStress["Tau_shear"]	= max(max(fabs(dNu), dNutWeb), dNuw);
			m_MapSecStress["Tau_shear"]	= dNu + dNutWeb;
			m_MapSecStress["Fw"]		= dFw;
			m_MapSecStress["Alpha"]		= dA;
			// < 전단 및 비틀림 > - 전단 응력(풍하중 재하시+활하중 비재하시)
			dMt		= dTorsionPreDead+dTorsionPostDead+dTorsionWind;
			dWn		= dWeb_h*dUp_f_w/4;
			dH2		= dWeb_h+(dUp_f_t+dLo_f_t)/2;
			dIw		= (pow(dH2,2)*(dUp_f_t*pow(dUp_f_w,3)*dLo_f_t*pow(dLo_f_w,3)))/(12*(dUp_f_t*pow(dUp_f_w,3)+dLo_f_t*pow(dLo_f_w,3)));
			dA		= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			dMwa	= (dMt/dL)/pow(dA/dL,2) * (2*sinh(dA/2)/sinh(dA)-1);
			dTwa	= -(dMt/dL)/(dA/dL) * (1-cosh(dA))/sinh(dA);
			dSwa	= (dWeb_h*dUp_f_t*pow(max(dUp_f_w, dLo_f_w),2))/16;
			dNu		= (fabs(dShearPreDead)+fabs(dShearPostDead)+fabs(dShearWind))/dAsWeb;
			dNutWeb	= dMt*dWeb_t/dKKv;
			dNutFL	= dMt*dUp_f_t/dKKv;
			dFw		= fabs(dMwa) * dWn / dIw;
			dNuw	= dTwa*dSwa / (dIw*dUp_f_t);
			m_MapSecStress["Mt_w"]			= dMt;
			m_MapSecStress["Wh_w"]			= dWn;
			m_MapSecStress["lw_w"]			= dIw;
			m_MapSecStress["Mwa_w"]			= dMwa;
			m_MapSecStress["Twa_w"]			= dTwa;
			m_MapSecStress["Swa_w"]			= dSwa;
			m_MapSecStress["Nu_w"]			= dNu;
			m_MapSecStress["Nut_Web_w"]		= dNutWeb;
			m_MapSecStress["Nut_FL_w"]		= dNutFL;
			m_MapSecStress["Nuw_w"]			= dNuw;
			m_MapSecStress["Nu_Web_w"]		= dNu+dNutWeb;
			m_MapSecStress["Nu_FL_w"]		= dNutFL+dNuw;
			m_MapSecStress["Tau_shear_w"]	= max(max(fabs(dNu), dNutWeb), dNuw);
			m_MapSecStress["Fw_w"]			= dFw;
			m_MapSecStress["Alpha_w"]		= dA;
			// < 전단 및 비틀림 > - 전단 응력(풍하중 재하시+활하중 재하시)
			dMt		= dTorsionPreDead+dTorsionPostDead+dTorsionWind/2+dTorsionLive;
			dWn		= dWeb_h*dUp_f_w/4;
			dH2		= dWeb_h+(dUp_f_t+dLo_f_t)/2;
			dIw		= (pow(dH2,2)*(dUp_f_t*pow(dUp_f_w,3)*dLo_f_t*pow(dLo_f_w,3)))/(12*(dUp_f_t*pow(dUp_f_w,3)+dLo_f_t*pow(dLo_f_w,3)));
			dA		= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			dMwa	= (dMt/dL)/pow(dA/dL,2) * (2*sinh(dA/2)/sinh(dA)-1);
			dTwa	= -(dMt/dL)/(dA/dL) * (1-cosh(dA))/sinh(dA);
			dSwa	= (dWeb_h*dUp_f_t*pow(max(dUp_f_w, dLo_f_w),2))/16;
			dNu		= (fabs(dShearPreDead)+fabs(dShearPostDead)+fabs(dShearWind/2)+fabs(dShearLive))/dAsWeb;
			dNutWeb	= dMt*dWeb_t/dKKv;
			dNutFL	= dMt*dUp_f_t/dKKv;
			dFw		= fabs(dMwa) * dWn / dIw;
			dNuw	= dTwa*dSwa / (dIw*dUp_f_t);
			m_MapSecStress["Mt_wl"]			= dMt;
			m_MapSecStress["Wh_wl"]			= dWn;
			m_MapSecStress["lw_wl"]			= dIw;
			m_MapSecStress["Mwa_wl"]		= dMwa;
			m_MapSecStress["Twa_wl"]		= dTwa;
			m_MapSecStress["Swa_wl"]		= dSwa;
			m_MapSecStress["Nu_wl"]			= dNu;
			m_MapSecStress["Nut_Web_wl"]	= dNutWeb;
			m_MapSecStress["Nut_FL_wl"]		= dNutFL;
			m_MapSecStress["Nuw_wl"]		= dNuw;
			m_MapSecStress["Nu_Web_wl"]		= dNu+dNutWeb;
			m_MapSecStress["Nu_FL_wl"]		= dNutFL+dNuw;
			m_MapSecStress["Tau_shear_wl"]	= max(max(fabs(dNu), dNutWeb), dNuw);
			m_MapSecStress["Fw_wl"]			= dFw;
			m_MapSecStress["Alpha_wl"]		= dA;

//			Tau_shear = fabs(dShearPreDead+dShearPostDead+dShearLive)/dAsWeb + fabs(dTorsionPreDead+dTorsionPostDead+dTorsionLive)*dWeb_t/dKKv;
//			m_MapSecStress["Tau_shear"] = Tau_shear;		// 전단 응력-풍하중 비재하시
//			Tau_shear_w = fabs(dShearPreDead+dShearPostDead+dShearWind) / dAsWeb + fabs(dTorsionPreDead+dTorsionPostDead+dTorsionWind)*dWeb_t/dKKv;
//			m_MapSecStress["Tau_shear_w"] = Tau_shear_w;	// 전단 응력(풍하중 재하시+활하중 비재하시)
//			Tau_shear_wl = fabs(dShearPreDead+dShearPostDead+dShearWind/2+dShearLive)/dAsWeb + fabs(dTorsionPreDead+dTorsionPostDead+dTorsionWind/2+dTorsionLive)*dWeb_t/dKKv;
//			m_MapSecStress["Tau_shear_wl"] = Tau_shear_wl;	// 전단 응력(풍하중 재하시+활하중 재하시)

			// < 조합 응력 >
			double Sigma1 = Sigma_su + Sigma_vsu + Sigma_vsu1 + Sigma_vsu2;				m_MapSecStress["Sigma1"] = Sigma1;
			double Sigma2 = Sigma_sl + Sigma_vsl + Sigma_vsl1 + Sigma_vsl2;				m_MapSecStress["Sigma2"] = Sigma2;
			double StressPi = ( Sigma1 - Sigma2 ) / Sigma1;								m_MapSecStress["StressPi"] = StressPi;// 응력 구배
		}
		else    // 부모멘트
		{
			//// 부모멘트 구간시 철근에 관련된 사항
			double tcu = pData->m_dCoverUp;		m_MapSecStress["tcu"] = tcu;
			double tcl = pData->m_dCoverDn;     m_MapSecStress["tcl"] = tcl;
			double asuse1 = pData->m_Asuse1;
			double asuse2 = pData->m_Asuse2;
			double Aru = asuse1*dWEffectSlab/1000;									m_MapSecStress["Aru"] = Aru;
			double Arl = asuse2*dWEffectSlab/1000;									m_MapSecStress["Arl"] = Arl;
			double Ar  = Aru + Arl;														m_MapSecStress["Ar"]  = Ar;
			double Av  = dAsGirder + Ar;												m_MapSecStress["Av"]  = Av;
			// < 합성후 응력 >
			double d = 0;
			if(bUpperInner)
				d = dYsuPre - dUp_f_t + dTHunch + tcl+ (dTSlab-tcu-tcl)*Aru/Ar;
			else
				d = dYsuPre + dTHunch + tcl+ (dTSlab-tcu-tcl)*Aru/Ar;
			m_MapSecStress["d"]    = d;
			double dr		= d*dAsGirder/Av;											m_MapSecStress["dr"]   = dr;
			double ds		= d-dr;														m_MapSecStress["ds"]   = ds;
			double Yvru		= dr+(dTSlab-tcu-tcl)*Arl/Ar;								m_MapSecStress["Yvru"] = Yvru;
			double Yvrl		= dr-(dTSlab-tcu-tcl)*Aru/Ar;								m_MapSecStress["Yvrl"] = Yvrl;
			double dYsuPost = bUpperInner ? Yvrl-tcl-dTHunch+dUp_f_t : Yvrl-tcl-dTHunch; m_MapSecStress["Yvsu"] = dYsuPost;
			double dYslPost = dYsuPost - (dWeb_h+dUp_f_t+dLo_f_t);					m_MapSecStress["Yvsl"] = dYslPost;
			double Irv      = dI33 + dAsGirder*ds*ds + Ar*dr*dr;						m_MapSecStress["Irv"]  = Irv;
			
			double Sigma_vcu = dMomentPostDead / Irv * Yvru;							m_MapSecStress["Sigma_vcu"] = Sigma_vcu;
			double Sigma_vcl = dMomentPostDead / Irv * Yvrl;							m_MapSecStress["Sigma_vcl"] = Sigma_vcl;
			double Sigma_vsu = dMomentPostDead / Irv * dYsuPost;						m_MapSecStress["Sigma_vsu"] = Sigma_vsu;
			double Sigma_vsl = dMomentPostDead / Irv * dYslPost;						m_MapSecStress["Sigma_vsl"] = Sigma_vsl;
			double Tau_sv    = fabs(dShearPostDead) / dAsWeb + fabs(dTorsionPostDead)*dWeb_t/dKKv;
			m_MapSecStress["Tau_sv"] = Tau_sv;
			// < 활하중에 의한 응력 >
			double Sigma_vcu_l = (dMomentLive) / Irv * Yvru;				m_MapSecStress["Sigma_vcu_l"] = Sigma_vcu_l;
			double Sigma_vcl_l = (dMomentLive) / Irv * Yvrl;				m_MapSecStress["Sigma_vcl_l"] = Sigma_vcl_l;
			double Sigma_vsu_l = (dMomentLive) / Irv * dYsuPost;			m_MapSecStress["Sigma_vsu_l"] = Sigma_vsu_l;
			double Sigma_vsl_l = (dMomentLive) / Irv * dYslPost;			m_MapSecStress["Sigma_vsl_l"] = Sigma_vsl_l;
			double Tau_sv_l = fabs(dShearLive) / dAsWeb	+ fabs(dTorsionLive)*dWeb_t/dKKv;
			m_MapSecStress["Tau_sv_l"] = Tau_sv_l;
			// < 풍하중에 의한 응력 >
			double Sigma_vcu_w = dMomentWind / Irv * Yvru;			m_MapSecStress["Sigma_vcu_w"] = Sigma_vcu_w;
			double Sigma_vcl_w = dMomentWind / Irv * Yvrl;			m_MapSecStress["Sigma_vcl_w"] = Sigma_vcl_w;
			double Sigma_vsu_w = dMomentWind / Irv * dYsuPost;		m_MapSecStress["Sigma_vsu_w"] = Sigma_vsu_w;
			double Sigma_vsl_w = dMomentWind / Irv * dYslPost;		m_MapSecStress["Sigma_vsl_w"] = Sigma_vsl_w;
			double Tau_sv_w = fabs(dShearWind) / dAsWeb + fabs(dTorsionWind)*dWeb_t/dKKv;
			m_MapSecStress["Tau_sv_w"] = Tau_sv_w;
			// < Creep에 의한 응력 >
			double Avc = dAsGirder+dAsSlab/dN;						m_MapSecStress["Avc"] = Avc;
			double Dv = bUpperInner ? (dYsuPre - dUp_f_t + dTHunch + dTSlab /2) : (dYsuPre + dTHunch + dTSlab /2);	m_MapSecStress["Dv"] = Dv;
			double Dvc = Dv*dAsGirder/Avc;							m_MapSecStress["Dvc"] = Dvc;
			double Dvs = Dv-Dvc;									m_MapSecStress["Dvs"] = Dvs;
			double Ec1 = dEc / ( 1 + dCreepCoef1 / 2 );				m_MapSecStress["Ec1"] = Ec1;
			double dN1	= dN * ( 1 + dCreepCoef1 / 2 );				m_MapSecStress["n1"] = dN1;
			double Av1 = dAsGirder + dAsSlab / dN1;					m_MapSecStress["Av1"] = Av1;
			double Dc1 = Dv*dAsGirder/(dAsSlab/dN1+dAsGirder);		m_MapSecStress["Dc1"] = Dc1;
			double Ds1 = Dv - Dc1;									m_MapSecStress["Ds1"] = Ds1;
			double Icv = dI33 + ModelCalc.m_MapSecCoe["Igcs"] + dAsGirder*Dvs*Dvs + dAsSlab/dN*Dvc*Dvc;	m_MapSecStress["Icv"] = Icv;
			double Nc = dMomentPostDead * Dvc * dAsSlab	/ (dN*Icv);						m_MapSecStress["Nc"] = Nc;
			double PCreep = Nc * 2 * dCreepCoef1 / ( 2 + dCreepCoef1 );					m_MapSecStress["PCreep"] = PCreep;
			double MCreep = PCreep * Dc1;					  							m_MapSecStress["MCreep"] = MCreep;
			double Sigma_cu = dMomentPostDead*Yvru/Irv;		  							m_MapSecStress["Sigma_cu"] = Sigma_cu;
			double Sigma_cl = dMomentPostDead*Yvrl/Irv;   								m_MapSecStress["Sigma_cl"] = Sigma_cl;
			double Sigma_cu1 = (PCreep / Av + dK * MCreep * Yvru / Irv );				m_MapSecStress["Sigma_cu1"] = Sigma_cu1;
			double Sigma_cl1 = (PCreep / Av + dK * MCreep * Yvrl / Irv );				m_MapSecStress["Sigma_cl1"] = Sigma_cl1;
			double Sigma_vsu1 = PCreep / Av + dK * MCreep * dYsuPost / Irv;				m_MapSecStress["Sigma_vsu1"] = Sigma_vsu1;
			double Sigma_vsl1 = PCreep / Av + dK * MCreep * dYslPost / Irv;				m_MapSecStress["Sigma_vsl1"] = Sigma_vsl1;
			// < 건조 수축 >
			double dEpsilon = pData->DESIGN_MATERIAL.m_dEpsilonS;						m_MapSecStress["Epsilon"] = dEpsilon;
			double dCreepCoef2= pData->DESIGN_MATERIAL.m_dCreep2;						m_MapSecStress["CreepCoef2"] = dCreepCoef2;
			double dN2 = dN * ( 1 + dCreepCoef2 /2 );									m_MapSecStress["n2"] = dN2;
			double Av2 = dAsGirder + dAsSlab / dN2;										m_MapSecStress["Av2"] = Av2;
			double Dc2 = ddV*dAsGirder/(dAsSlab/dN2+dAsGirder);							m_MapSecStress["Dc2"] = Dc2;
			double Ds2 = ddV - Dc2;														m_MapSecStress["Ds2"] = Ds2;
			double Iv2 = dI33 + dIgc/dN2+ dAsGirder * Ds2 * Ds2 + dAsSlab/dN2*Dc2*Dc2;	m_MapSecStress["Iv2"] = Iv2;
			double PShrink = dEst * dEpsilon * dAsSlab / dN2;							m_MapSecStress["PShrink"] = PShrink;
			double MShrink = PShrink * Dc2;												m_MapSecStress["MShrink"] = MShrink;
			double Sigma_cu2 = (PShrink / Av + dK * MShrink * Yvru / Irv );				m_MapSecStress["Sigma_cu2"] = Sigma_cu2;
			double Sigma_cl2 = (PShrink / Av + dK * MShrink * Yvrl / Irv );				m_MapSecStress["Sigma_cl2"] = Sigma_cl2;
			double Sigma_vsu2 = PShrink / Av + dK * MShrink * dYsuPost / Irv;			m_MapSecStress["Sigma_vsu2"] = Sigma_vsu2;
			double Sigma_vsl2 = PShrink / Av + dK * MShrink * dYslPost / Irv;			m_MapSecStress["Sigma_vsl2"] = Sigma_vsl2;
			// < 온도차 > 
			double Alpha = pData->DESIGN_MATERIAL.m_dAlpha;								m_MapSecStress["Alpha"] = Alpha;
			double Temperature  = pData->DESIGN_MATERIAL.m_dTemparature;				m_MapSecStress["Temperature"] = Temperature;
			double Et = Alpha * Temperature;											m_MapSecStress["Et"] = Et;
			double Ect = dEc * Et;														m_MapSecStress["Ect"] = Ect;
			double P1 = dEst * Et * dAsSlab / dN;										m_MapSecStress["P1"] = P1;
			double Mt = P1 * Dvc;														m_MapSecStress["Mt"] = Mt;
			double Ert = dEst * Et;														m_MapSecStress["Ert"] = Ert;
			double Sigma_cut = ( P1 / Av + dK * Mt / Irv * Yvru ) - Ert;				m_MapSecStress["Sigma_cut"] = Sigma_cut;
			double Sigma_clt = ( P1 / Av + dK * Mt / Irv * Yvrl ) - Ert;				m_MapSecStress["Sigma_clt"] = Sigma_clt;
			double Sigma_sut = P1 / Av + dK * Mt / Irv * dYsuPost;						m_MapSecStress["Sigma_sut"] = Sigma_sut;
			double Sigma_slt = P1 / Av + dK * Mt / Irv * dYslPost;						m_MapSecStress["Sigma_slt"] = Sigma_slt;
//			m_MapSecStress["Tau_shear"] = Tau_shear;		// 전단 응력(풍하중 비재하시)
//			m_MapSecStress["Tau_shear_w"] = Tau_shear_w;	// 전단 응력(풍하중 재하시 + 활하중 비재하시)
//			m_MapSecStress["Tau_shear_wl"] = Tau_shear_wl;	// 전단 응력(풍하중 재하시 + 활하중 재하시)
			double	dL		= pDB->m_dLengthBaseCrossBeam;	// 가로보 간격
			m_MapSecStress["CrossBeamTerm"]	= dL;
			// < 전단 및 비틀림 > - 전단 응력(풍하중 비재하시)
			double	dMt		= dTorsionPreDead+dTorsionPostDead+dTorsionLive;
			double	dWn		= dWeb_h*dUp_f_w/4.0;
			double	dH2		= dWeb_h+(dUp_f_t+dLo_f_t)/2.0;
			double	dIw		= (pow(dH2,2)*(dUp_f_t*pow(dUp_f_w,3)*dLo_f_t*pow(dLo_f_w,3)))/(12*(dUp_f_t*pow(dUp_f_w,3)+dLo_f_t*pow(dLo_f_w,3)));
			double	dA		= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			double	dMwa	= (dMt/dL)/pow(dA/dL,2) * (2*sinh(dA/2)/sinh(dA)-1);
			double	dTwa	= -(dMt/dL)/(dA/dL) * (1-cosh(dA))/sinh(dA);
			double	dSwa	= (dWeb_h*dUp_f_t*pow(max(dUp_f_w, dLo_f_w),2))/16;
			double	dNu		= (fabs(dShearPreDead)+fabs(dShearPostDead)+fabs(dShearLive))/dAsWeb;
			double	dNutWeb	= dMt*dWeb_t/dKKv;
			double	dNutFL	= dMt*dUp_f_t/dKKv;
			double	dFw		= fabs(dMwa) * dWn / dIw;
			double	dNuw	= dTwa*dSwa / (dIw*dUp_f_t);
			m_MapSecStress["Mt"]		= dMt;
			m_MapSecStress["Wn"]		= dWn;
			m_MapSecStress["lw"]		= dIw;
			m_MapSecStress["Mwa"]		= dMwa;
			m_MapSecStress["Twa"]		= dTwa;
			m_MapSecStress["Swa"]		= dSwa;
			m_MapSecStress["Nu"]		= dNu;
			m_MapSecStress["Nut_Web"]	= dNutWeb;
			m_MapSecStress["Nut_FL"]	= dNutFL;
			m_MapSecStress["Nuw"]		= dNuw;
			m_MapSecStress["Nu_Web"]	= dNu+dNutWeb;
			m_MapSecStress["Nu_FL"]		= dNutFL+dNuw;
//			m_MapSecStress["Tau_shear"]	= max(max(fabs(dNu), dNutWeb), dNuw);
			m_MapSecStress["Tau_shear"]	= fabs(dNu)+fabs(dNutWeb);
			m_MapSecStress["Fw"]		= dFw;
			m_MapSecStress["Alpha"]		= dA;
			// < 전단 및 비틀림 > - 전단 응력(풍하중 재하시+활하중 비재하시)
			dMt		= dTorsionPreDead+dTorsionPostDead+dTorsionWind;
			dWn		= dWeb_h*dUp_f_w/4;
			dH2		= dWeb_h+(dUp_f_t+dLo_f_t)/2;
			dIw		= (dH2*(dUp_f_t*pow(dUp_f_w,3)*dLo_f_t*pow(dLo_f_w,3)))/(12*(dUp_f_t*pow(dUp_f_w,3)+dLo_f_t*pow(dLo_f_w,3)));
			dA		= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			dMwa	= (dMt/dL)/pow(dA/dL,2) * (2*sinh(dA/2)/sinh(dA)-1);
			dTwa	= -(dMt/dL)/(dA/dL) * (1-cosh(dA))/sinh(dA);
			dSwa	= (dWeb_h*dUp_f_t*pow(max(dUp_f_w, dLo_f_w),2))/16;
			dNu		= (fabs(dShearPreDead)+fabs(dShearPostDead)+fabs(dShearWind))/dAsWeb;
			dNutWeb	= dMt*dWeb_t/dKKv;
			dNutFL	= dMt*dUp_f_t/dKKv;
			dFw		= dMwa * dWn / dIw;
			dNuw	= dTwa*dSwa / (dIw*dUp_f_t);
			m_MapSecStress["Mt_w"]			= dMt;
			m_MapSecStress["Wh_w"]			= dWn;
			m_MapSecStress["lw_w"]			= dIw;
			m_MapSecStress["Mwa_w"]			= dMwa;
			m_MapSecStress["Twa_w"]			= dTwa;
			m_MapSecStress["Swa_w"]			= dSwa;
			m_MapSecStress["Nu_w"]			= dNu;
			m_MapSecStress["Nut_Web_w"]		= dNutWeb;
			m_MapSecStress["Nut_FL_w"]		= dNutFL;
			m_MapSecStress["Nuw_w"]			= dNuw;
			m_MapSecStress["Nu_Web_w"]		= dNu+dNutWeb;
			m_MapSecStress["Nu_FL_w"]		= dNutFL+dNuw;
//			m_MapSecStress["Tau_shear_w"]	= max(max(fabs(dNu), dNutWeb), dNuw);
			m_MapSecStress["Tau_shear_w"]	= fabs(dNu)+fabs(dNutWeb);
			m_MapSecStress["Fw_w"]			= dFw;
			m_MapSecStress["Alpha_w"]		= dA;
			// < 전단 및 비틀림 > - 전단 응력(풍하중 재하시+활하중 재하시)
			dMt		= dTorsionPreDead+dTorsionPostDead+dTorsionWind/2+dTorsionLive;
			dWn		= dWeb_h*dUp_f_w/4;
			dH2		= dWeb_h+(dUp_f_t+dLo_f_t)/2;
			dIw		= (dH2*(dUp_f_t*pow(dUp_f_w,3)*dLo_f_t*pow(dLo_f_w,3)))/(12*(dUp_f_t*pow(dUp_f_w,3)+dLo_f_t*pow(dLo_f_w,3)));
			dA		= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			dMwa	= (dMt/dL)/pow(dA/dL,2) * (2*sinh(dA/2)/sinh(dA)-1);
			dTwa	= -(dMt/dL)/(dA/dL) * (1-cosh(dA))/sinh(dA);
			dSwa	= (dWeb_h*dUp_f_t*pow(max(dUp_f_w, dLo_f_w),2))/16;
			dNu		= (fabs(dShearPreDead)+fabs(dShearPostDead)+fabs(dShearWind/2)+fabs(dShearLive))/dAsWeb;
			dNutWeb	= dMt*dWeb_t/dKKv;
			dNutFL	= dMt*dUp_f_t/dKKv;
			dFw		= fabs(dMwa) * dWn / dIw;
			dNuw	= dTwa*dSwa / (dIw*dUp_f_t);
			m_MapSecStress["Mt_wl"]			= dMt;
			m_MapSecStress["Wh_wl"]			= dWn;
			m_MapSecStress["lw_wl"]			= dIw;
			m_MapSecStress["Mwa_wl"]		= dMwa;
			m_MapSecStress["Twa_wl"]		= dTwa;
			m_MapSecStress["Swa_wl"]		= dSwa;
			m_MapSecStress["Nu_wl"]			= dNu;
			m_MapSecStress["Nut_Web_wl"]	= dNutWeb;
			m_MapSecStress["Nut_FL_wl"]		= dNutFL;
			m_MapSecStress["Nuw_wl"]		= dNuw;
			m_MapSecStress["Nu_Web_wl"]		= dNu+dNutWeb;
			m_MapSecStress["Nu_FL_wl"]		= dNutFL+dNuw;
//			m_MapSecStress["Tau_shear_wl"]	= max(max(fabs(dNu), dNutWeb), dNuw);
			m_MapSecStress["Tau_shear_wl"]	= fabs(dNu)+fabs(dNutWeb);
			m_MapSecStress["Fw_wl"]			= dFw;
			m_MapSecStress["Alpha_wl"]		= dA;

//			Tau_shear = fabs(dShearPreDead+dShearPostDead+dShearLive)/dAsWeb + fabs(dTorsionPreDead+dTorsionPostDead+dTorsionLive)*dWeb_t/dKKv;
//			m_MapSecStress["Tau_shear"] = Tau_shear;		// 전단 응력-풍하중 비재하시
//			Tau_shear_w = fabs(dShearPreDead+dShearPostDead+dShearWind) / dAsWeb + fabs(dTorsionPreDead+dTorsionPostDead+dTorsionWind)*dWeb_t/dKKv;
//			m_MapSecStress["Tau_shear_w"] = Tau_shear_w;	// 전단 응력(풍하중 재하시+활하중 비재하시)
//			Tau_shear_wl = fabs(dShearPreDead+dShearPostDead+dShearWind/2+dShearLive)/dAsWeb + fabs(dTorsionPreDead+dTorsionPostDead+dTorsionWind/2+dTorsionLive)*dWeb_t/dKKv;
//			m_MapSecStress["Tau_shear_wl"] = Tau_shear_wl;	// 전단 응력(풍하중 재하시+활하중 재하시)
			// < 조합 응력 >
			double Sigma1 = Sigma_su + Sigma_vsu + Sigma_vsu1 + Sigma_vsu2;			m_MapSecStress["Sigma1"] = Sigma1;
			double Sigma2 = Sigma_sl + Sigma_vsl + Sigma_vsl1 + Sigma_vsl2;			m_MapSecStress["Sigma2"] = Sigma2;
			double StressPi = ( Sigma1 - Sigma2 ) / Sigma1;							m_MapSecStress["StressPi"] = StressPi;
		}

		// 하이브리드 계수 ///////////////////////////////////////////////////////
		
		pBx = pGir->GetBxByStation(dStaCoeff);
		CString szMaterialL, szMaterialW, szTemp;		
		
		CSteel			*pSteelCtl = m_pDataManage->GetSteel();
		CalcGen.GetMaterial(pBx, G_F_L, szMaterialL, szTemp);
		CHGBaseSteel	*pSteelL = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szMaterialL + szTemp)];

		CalcGen.GetMaterial(pBx, G_W,	szMaterialW, szTemp);
		CHGBaseSteel	*pSteelW = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szMaterialW + szTemp)];
		CHGAllowStress AllowStressL(pSteelL);
		CHGAllowStress AllowStressW(pSteelW);
		double dBxT = 0, dHy_beta = 0, dHy_Phi = 0;
		double dYvsu	= m_MapSecStress["Yvsu"];
		double dYvsl	= m_MapSecStress["Yvsl"];
		double dHy_alpha	= min(1, AllowStressW.GetAllowStress(ALLOW_STDYIELDPOINT_WEB, dWeb_t)/AllowStressL.GetAllowStress(ALLOW_STDYIELDPOINT_LO, dLo_f_t));	// 최대 1
		// 하이브리드 합성전
		dBxT			= fabs(dYsuPre)>fabs(dYslPre) ? (dUp_f_w*dUp_f_t) : (dLo_f_w*dLo_f_t);
		dHy_beta		= (dWeb_h*dWeb_t)/dBxT;
		dHy_Phi			= max(fabs(dYsuPre),fabs(dYslPre))/(dUp_f_t+dLo_f_t+dWeb_h);
		dHy_RPre		= 1-(dHy_Phi*dHy_beta*pow(1-dHy_alpha,2)*(3-dHy_Phi+dHy_Phi*dHy_alpha))/(6+dHy_beta*dHy_Phi*(3-dHy_Phi));

		// 하이브리드 합성후
		dBxT			= fabs(dYvsu)>fabs(dYvsl) ? (dUp_f_w*dUp_f_t) : (dLo_f_w*dLo_f_t);
		dHy_beta		= (dWeb_h*dWeb_t)/dBxT;
		dHy_Phi			= max(fabs(dYvsu),fabs(dYvsl))/(dUp_f_t+dLo_f_t+dWeb_h);
		dHy_RPos		= 1-(dHy_Phi*dHy_beta*pow(1-dHy_alpha,2)*(3-dHy_Phi+dHy_Phi*dHy_alpha))/(6+dHy_beta*dHy_Phi*(3-dHy_Phi));

		if(!pBx->IsHybrid())
			dHy_RPre = dHy_RPos		= 1;		// 하이브리드 아닐때..
	}
	///< 비합성일 경우 응력 **************************************************************************************
	else    
	{
		///< 합성전 사하중(강재자중)에 대한 응력검토
		double Sigma_su = dMomentPreDead / dI33 * dYsuPre;				m_MapSecStress["Sigma_su"] = Sigma_su;// 상판 응력
		double Sigma_sl = dMomentPreDead / dI33 * dYslPre;				m_MapSecStress["Sigma_sl"] = Sigma_sl;// 하판 응력
		double Tau_s    = fabs(dShearPreDead) / dAsWeb	+ fabs(dTorsionPreDead)*dWeb_t/dKK;// 전단응력
		m_MapSecStress["Tau_s"] = Tau_s;
		///< 합성전 사하중(콘크리트 및 포장하중)에 대한 응력검토
		double Sigma_vcu = dMomentPostDead / dI33 * dYsuPre;			m_MapSecStress["Sigma_vcu"] = Sigma_vcu;
		double Sigma_vcl = dMomentPostDead / dI33 * dYslPre;			m_MapSecStress["Sigma_vcl"] = Sigma_vcl;
		///< 전단 응력
		double Tau_sc    = fabs(dShearPostDead) / dAsWeb + fabs(dTorsionPostDead)*dWeb_t/dKK;
		m_MapSecStress["Tau_sc"] = Tau_sc;
		// <(활하중 + 지점침하)에 대한 응력검토>
		double Sigma_vsu = (dMomentLive) / dI33 * dYsuPre;		m_MapSecStress["Sigma_vsu"] = Sigma_vsu;
		double Sigma_vsl = (dMomentLive) / dI33 * dYslPre;		m_MapSecStress["Sigma_vsl"] = Sigma_vsl;
		/*
		// 전단 응력
		double Tau_sv    = fabs(dShearDisp) / dAsWeb + fabs(dTorsionLive)*dWeb_t/dKK;
		m_MapSecStress["Tau_sv"] = Tau_sv;
		*/
		// < 전단 및 비틀림 >
		double Tau_shear = (fabs(dShearPreDead)+fabs(dShearPostDead)+fabs(dShearLive)+fabs(dShearLive)) / dAsWeb + fabs(dTorsionPreDead+dTorsionPostDead+dTorsionLive)*dWeb_t/dKK;
		m_MapSecStress["Tau_shear"] = Tau_shear;
	}
	m_MapSecStress["HyR_Pre"]		= dHy_RPre;
	m_MapSecStress["HyR_Pos"]		= dHy_RPos;
}

BOOL CFemStressCalc::InsertMomentPos(long nG)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);

	BOOL	bInsert	= TRUE;
	double	dThick	= 0;

	dThick	= pGir->GetFactChain(G_F_U, 0)->m_dFactChainThick;
	long n = 0;
	for(n=1; n<pGir->GetSizeFactChain(G_F_U); n++)
	{
		if(dThick != pGir->GetFactChain(G_F_U, n)->m_dFactChainThick)
			return FALSE;
	}

	dThick	= pGir->GetFactChain(G_F_L, 0)->m_dFactChainThick;
	for(n=1; n<pGir->GetSizeFactChain(G_F_L); n++)
	{
		CPlateFactChain *pFact =pGir->GetFactChain(G_F_L, n);
		if(dThick != pFact->m_dFactChainThick)
			return FALSE;
	}

	dThick	= pGir->GetFactChain(G_W, 0)->m_dFactChainThick;
	for(n=1; n<pGir->GetSizeFactChain(G_W); n++)
	{
		CPlateFactChain *pFact =pGir->GetFactChain(G_W, n);
		if(dThick != pFact->m_dFactChainThick)
			return FALSE;
	}
	return bInsert;
}

BOOL CFemStressCalc::IsMomentPos(long nG, double dSta)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);

	double	dStaMomentup	= pGir->GetSSS(OFF_STT_SLAB);
	long n = 0;
	for(n=0; n<pGir->m_pRangePress->m_ArrayUpper.GetSize(); n++)
	{
		dStaMomentup += pGir->m_pRangePress->m_ArrayUpper.GetAt(n)->m_dDistRange;
		if(Round(dSta,3)==Round(dStaMomentup,3))
			return TRUE;
	}
	dStaMomentup	= pGir->GetSSS(OFF_STT_SLAB);
	for(n=0; n<pGir->m_pRangePress->m_ArrayLower.GetSize(); n++)
	{
		CRangePressData *pRangPressData = pGir->m_pRangePress->m_ArrayLower.GetAt(n);
		dStaMomentup += pRangPressData->m_dDistRange;
		if(Round(dSta,3)==Round(dStaMomentup,3))
			return TRUE;
	}

	return FALSE;
}


// !! 하이브리드 추가단면 체크를 위한 함수 !!
// FALSE : 현재 위치의 전/후 제원이 같으면서 부재명이 다른경우
BOOL CFemStressCalc::IsSameSteelMaterial(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CPlateFactChain *pFUPre = pBx->GetFactChain(G_F_U, FALSE);
	CPlateFactChain *pFUNxt = pBx->GetFactChain(G_F_U, TRUE);
	CPlateFactChain *pFLPre = pBx->GetFactChain(G_F_L, FALSE);
	CPlateFactChain *pFLNxt = pBx->GetFactChain(G_F_L, TRUE);
	CPlateFactChain *pFWPre = pBx->GetFactChain(G_W, FALSE);
	CPlateFactChain *pFWNxt = pBx->GetFactChain(G_W, TRUE);

	CString szPre, szNext, szTemp;
	
	// 제원이 다른경우, 기본적인 단면구성이므로 고려대상 아님
	if(pFUPre->m_dWidth != pFUNxt->m_dWidth)					return TRUE;
	if(pFLPre->m_dWidth != pFLNxt->m_dWidth)					return TRUE;
	if(pFWPre->m_dWidth != pFWNxt->m_dWidth)					return TRUE;
	if(pFUPre->m_dFactChainThick != pFUNxt->m_dFactChainThick)	return TRUE;	
	if(pFLPre->m_dFactChainThick != pFLNxt->m_dFactChainThick)	return TRUE;	
	if(pFWPre->m_dFactChainThick != pFWNxt->m_dFactChainThick)	return TRUE;

	if(pFUPre && pFUNxt)
	{
		pDB->GetRegulationSteelMaterial(G_F_U, pFUPre->m_strSymbol, szPre,	szTemp);
		pDB->GetRegulationSteelMaterial(G_F_U, pFUNxt->m_strSymbol, szNext, szTemp);
		if(szPre != szNext)	return FALSE;
	}

	if(pFLPre && pFLNxt)
	{
		pDB->GetRegulationSteelMaterial(G_F_L, pFLPre->m_strSymbol, szPre,	szTemp);
		pDB->GetRegulationSteelMaterial(G_F_L, pFLNxt->m_strSymbol, szNext, szTemp);
		if(szPre != szNext)	return FALSE;
	}

	if(pFWPre && pFWNxt)
	{
		pDB->GetRegulationSteelMaterial(G_W, pFWPre->m_strSymbol, szPre,	szTemp);
		pDB->GetRegulationSteelMaterial(G_W, pFWNxt->m_strSymbol, szNext,	szTemp);
		if(szPre != szNext)	return FALSE;
	}

	return TRUE;	
}

BOOL CFemStressCalc::IsSameSteelMaterial(long nG, double dSta)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);

	return IsSameSteelMaterial(pBx);
}

bool AscendStation(CDanMyunProp p1, CDanMyunProp p2)
{
	return p1.m_pBx->GetStation() < p2.m_pBx->GetStation();
}

bool AscendMatIndex(CSectionJewon p1, CSectionJewon p2)
{
	return p1.m_nIdx < p2.m_nIdx;
}

bool IsSameStation(CDanMyunProp p1, CDanMyunProp p2)
{
	return p1.m_pBx->GetStation() == p2.m_pBx->GetStation();
}

bool IsSameMatIndex(CSectionJewon p1, CSectionJewon p2)
{
	if(p1.m_nIdx == p2.m_nIdx)
		return true;
	else
		return false;
}

void CFemStressCalc::SetSectionGugan()
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();  
	CFEMManage      *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	
	m_pCalcStd->m_vByunDanInfo.clear();
	m_pCalcStd->m_vExtraDanmyun.clear();

	for (long nG = 0; nG<pDB->GetGirdersu(); nG++)   
	{
		vector<CDanMyunProp> vByunDan;
		
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder FinderNode(pGir);
		CPlateBxFinder FinderTerm(pGir);

		CPlateBasicIndex *pBxTemp = NULL;
		CPlateBasicIndex *pBxINode = FinderNode.GetBxFirst(BX_CROSSBEAM_VBRACING);
		CPlateBasicIndex *pBxJNode = FinderNode.GetBxNext();
		
		double	dPreUThick	= 0;
		double	dPreLThick	= 0;
		double	dPreUWidth	= 0;
		double	dPreLWidth	= 0;
		double	dPreWebT	= 0;
		///< 각각의 Bx에 대하여 절점 사이에 두개씩 검색...		
		///< 테이터부분과 공장이음부에서 두께와 플랜지 폭이 다른 경우...

		CDanMyunProp SectI(m_pCalcStd, pBxINode);
		vByunDan.push_back(SectI);

		while(pBxJNode)	
		{
			CDanMyunProp SectJ(m_pCalcStd, pBxJNode);

			double dUThick = pBxJNode->GetFactChain(G_F_U)->m_dFactChainThick;
			double dLThick = pBxJNode->GetFactChain(G_F_L)->m_dFactChainThick;
			double dUWidth = pBxJNode->GetFactChain(G_F_U)->m_dWidth;
			double dLWidth = pBxJNode->GetFactChain(G_F_L)->m_dWidth;
			double dWebT   = pBxJNode->GetFactChain(G_W)->m_dFactChainThick;

			if(!SectI.IsByunDanMyun() || !SectJ.IsByunDanMyun())
			{
				// 각각의 Bx에 대하여 절점 사이에 세개씩 검색...				
				__int64 BxFlag;
				
				//////////////////////////////////////////////////////////////////////////
				// 강종변화
				BxFlag	= BX_UPPERFLANGE | BX_LOWERFLANGE | BX_WEB;			// 상부-공장이음
				pBxTemp	= FinderTerm.GetBxFirst(BxFlag, pBxINode, pBxJNode, TRUE, TRUE);
				while(pBxTemp)
				{
					if(!IsSameSteelMaterial(pBxTemp))	
					{
						CDanMyunProp Sect(m_pCalcStd, pBxTemp);
						vByunDan.push_back(Sect);
					}
					pBxTemp = FinderTerm.GetBxNext();
				}

				////////////////////////////////////////////////////
				BxFlag	= BX_UPPERFLANGE;			// 상부-공장이음
				pBxTemp	= FinderTerm.GetBxFirst(BxFlag, pBxINode, pBxJNode, TRUE, TRUE);
				while(pBxTemp)
				{
					dUWidth		= pBxTemp->GetFactChain(G_F_U)->m_dWidth;
					dUThick		= pBxTemp->GetFactChain(G_F_U)->m_dFactChainThick;
					if(!pBxINode->GetGirder()->IsByunByStation(pBxINode->GetStation()) && !pBxJNode->GetGirder()->IsByunByStation(pBxJNode->GetStation()))
					{
						if((dPreUThick!=dUThick) || (dPreUWidth!=dUWidth))
						{
							CDanMyunProp Sect(m_pCalcStd, pBxTemp);
							vByunDan.push_back(Sect);
							dPreUThick	= dUThick;
							dPreUWidth	= dUWidth;
						}
					}
					pBxTemp = FinderTerm.GetBxNext();
				}
								
				////////////////////////////////////////////////////
				BxFlag	= BX_LOWERFLANGE;	// 하부-공장이음
				pBxTemp	= FinderTerm.GetBxFirst(BxFlag, pBxINode, pBxJNode, TRUE, TRUE);
				while(pBxTemp)
				{
					dLWidth		= pBxTemp->GetFactChain(G_F_L)->m_dWidth;
					dLThick		= pBxTemp->GetFactChain(G_F_L)->m_dFactChainThick;
					if(!pBxINode->GetGirder()->IsByunByStation(pBxINode->GetStation()) &&
					   !pBxJNode->GetGirder()->IsByunByStation(pBxJNode->GetStation()))
					{
						if((dPreLThick!=dLThick) || (dPreLWidth!=dLWidth))
						{
							CDanMyunProp Sect(m_pCalcStd, pBxTemp);
							vByunDan.push_back(Sect);
							dPreLThick	= dLThick;
							dPreLWidth	= dLWidth;
						}
					}
					pBxTemp = FinderTerm.GetBxNext();
				}

				////////////////////////////////////////////////////
				BxFlag = BX_WEB;					// 복부-공장이음
				pBxTemp	= FinderTerm.GetBxFirst(BxFlag, pBxINode, pBxJNode, TRUE, TRUE);
				while(pBxTemp)
				{
					dWebT = pBxTemp->GetFactChain(G_W)->m_dFactChainThick;
					if(!pBxINode->GetGirder()->IsByunByStation(pBxINode->GetStation()) &&
					   !pBxJNode->GetGirder()->IsByunByStation(pBxJNode->GetStation()))
					{
						if(dWebT!=dPreWebT)
						{
							CDanMyunProp Sect(m_pCalcStd, pBxTemp);
							vByunDan.push_back(Sect);
							dPreWebT = dWebT;
						}
					}
					pBxTemp = FinderTerm.GetBxNext();
				}
				
				////////////////////////////////////////////////////
				if(!SectI.IsByunDanMyun() && !SectJ.IsByunDanMyun())
				{
					BxFlag = BX_STT_BENDING_UPPER;		// Taper
					pBxTemp = FinderTerm.GetBxFirst(BxFlag, pBxINode, pBxJNode, TRUE, TRUE);
					if(pBxTemp)
					{
						CDanMyunProp Sect(m_pCalcStd, pBxTemp);
						vByunDan.push_back(Sect);
					}
					
					BxFlag = BX_END_BENDING_UPPER;
					pBxTemp = FinderTerm.GetBxFirst(BxFlag, pBxINode, pBxJNode, TRUE, TRUE);
					if(pBxTemp)
					{
						CDanMyunProp Sect(m_pCalcStd, pBxTemp);
						vByunDan.push_back(Sect);
					}
				}
			}			
			vByunDan.push_back(SectJ);
			dPreUThick = dUThick;
			dPreLThick = dLThick;
			dPreUWidth = dUWidth;
			dPreLWidth = dLWidth;
			dPreWebT   = dWebT;

			pBxINode = pBxJNode;
			pBxJNode = FinderNode.GetBxNext();				
		}

		// Station 오름차순 순서로 정렬
		sort(vByunDan.begin(), vByunDan.end(), AscendStation);
		
		// 중복된 위치 정보 제거
		vByunDan.erase(unique(vByunDan.begin(), vByunDan.end(), IsSameStation), vByunDan.end());		

		// 단면 번호 결정
		long j = 0;
		for(j=0; j<(long)vByunDan.size(); j++)
		{
			vByunDan[j].SetMaterialIndex();
		}

		// 연속된 같은 단면번호 제거
		for(j=0; j<(long)vByunDan.size(); j++)
		{
			CDanMyunProp CurMat = vByunDan[j];

			for(int k=j+1; k<(long)vByunDan.size()-1; k++)
			{
				CDanMyunProp	Prop	= vByunDan[k];
				CSectionJewon	MatLeft = vByunDan[k].m_MatLeft;
				CSectionJewon	MatRight= vByunDan[k].m_MatRight;

				if((CurMat.m_MatRight == MatLeft) && (CurMat.m_MatRight == MatRight)
					&& !vByunDan[k-1].IsByunDanMyun()	&& !vByunDan[k+1].IsByunDanMyun())
				{
					// 최대구간 삭제방지 적용
					double dLength	= vByunDan[k].m_pBx->GetStation()-vByunDan[k-1].m_pBx->GetStation();
					if(k!=0 && dLength > pData->m_dMatMaxSpace) continue;

					vByunDan.erase(vByunDan.begin()+k);
					k--;					
				}
				else
					break;
			}			
		}

			// 최소간격보다 작을 경우 제거
		for(j=0; j<(long)vByunDan.size()-1; j++)
		{
			CDanMyunProp	*pCurMat	= &vByunDan[j];
			CDanMyunProp	*pNxtMat	= &vByunDan[j+1];

			double	dStaCur	= pCurMat->m_pBx->GetStation();
			double	dStaNxt	= pNxtMat->m_pBx->GetStation();
			double	dLen	= dStaNxt - dStaCur;

			if(dLen>pData->m_dMatMinSpace)	continue;			

			//2008.10.15 simdongyoun
			if(pCurMat->m_MatLeft.m_dI33<pNxtMat->m_MatLeft.m_dI33)
			{
				if(pCurMat->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER | BX_CROSSBEAM_VBRACING))	continue;
				if(!pNxtMat->IsByunDanMyun())
					pNxtMat->m_MatLeft = pCurMat->m_MatLeft;

				vByunDan.erase(vByunDan.begin()+j);
				--j;
			}
			else if(pCurMat->m_MatRight.m_dI33 > pNxtMat->m_MatRight.m_dI33)
			{
				if(pNxtMat->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER | BX_CROSSBEAM_VBRACING))	continue;
				if(!pCurMat->IsByunDanMyun())
					pCurMat->m_MatRight = pNxtMat->m_MatRight;

				vByunDan.erase(vByunDan.begin()+j+1);
				--j;
			}
			else
			{
				// 단면2차모멘트가 같은경우(대부분 변단면의경우 다이아프램에 의해서 나누어 지는 단면)
				// 조건에 걸리는 않는 위치를 삭제(2008.10.15 simdongyoun)
				if(!pCurMat->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER | BX_CROSSBEAM_VBRACING))
				{
					if(!pNxtMat->IsByunDanMyun())
						pNxtMat->m_MatLeft = pCurMat->m_MatLeft;
					vByunDan.erase(vByunDan.begin()+j);
					--j;
				}
				else
				{
					if(!pCurMat->IsByunDanMyun())
						pCurMat->m_MatRight = pNxtMat->m_MatRight;

					vByunDan.erase(vByunDan.begin()+j+1);
					--j;
				}
			}
		}

		// ================================================ 추가 단면 번호 다시 조정
		long nMatGir = 0;
		POSITION pos = pManage->m_FrameSectionList.GetHeadPosition();
		while(pos)
		{
			CFrameSection *pFrameSec = (CFrameSection *)pManage->m_FrameSectionList.GetNext(pos);
			if(pFrameSec->m_nFlag==POS_GIRDER)
				nMatGir++;
		}

		for(j=0; j<(long)vByunDan.size(); j++)
		{
			CDanMyunProp DanMyunProp = vByunDan[j];
			if(DanMyunProp.m_MatLeft.m_nIdx > nMatGir-1) 
				m_pCalcStd->SetExtraDanmyun(DanMyunProp.m_MatLeft);
			if(DanMyunProp.m_MatRight.m_nIdx > nMatGir-1) 
				m_pCalcStd->SetExtraDanmyun(DanMyunProp.m_MatRight);
		}
		// ================================================
		//for(j=0; j<vByunDan.size(); j++)
		//{
		//	TRACE2("\n[%d] [%d]",vByunDan[j].m_MatLeft.m_nIdx, vByunDan[j].m_MatRight.m_nIdx);
		//}

		// 삽입
		 
		m_pCalcStd->m_vByunDanInfo.push_back(vByunDan);
		
		// 사용자 단면위치 추가(단면 검토시 더블클릭으로 추가된 단면)
		for(long n=0; n<pData->m_ArrExtraSecTitle.GetSize(); n++)		
		{		
			CPlateGirderApp *pGirCur = pDB->GetGirder(pData->m_ArrExtraSecGir[n]);
			CPlateBasicIndex *pBxMatch = pGirCur->GetBxByStation(pData->m_ArrExtraSecSta[n]);

			CDanMyunProp SectUser(m_pCalcStd, pBxMatch); 
			vByunDan.push_back(SectUser);
		}
		if(pData->m_ArrExtraSecTitle.GetSize()>0)
		{
			// 한번더 정렬 - Station 오름차순 순서로 정렬
			sort(vByunDan.begin(), vByunDan.end(), AscendStation);
			// 한번더 단면 번호 결정
			for(int j=0; j<(long)vByunDan.size(); j++)
				vByunDan[j].SetMaterialIndex();
		}
	}
}

void CFemStressCalc::SetSectionSplice()
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();  
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);

	CJewonArray vArrJewon, vArrJewonNew;
	CJewon      vTemp;
	
	for (long n = 0; n<pDB->GetGirdersu(); n++)   
	{
		CPlateGirderApp *pGir = pDB->GetGirder(n);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_SPLICE);
		while(pBx)
		{
			GetSectionStress(n, pBx->GetStation(), FALSE);
			ModelCalc.GetSectionCoefficient(pBx);
			double dSta = pBx->GetStation();
			BOOL bPositiveM = IsPositive(ELE_MOMENT3, n, dSta);
			vTemp.h  = dSta;
			vTemp.b  = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBx);//m_MapSecCoe["I33"];
			vTemp.t1 = m_MapSecStress[bPositiveM?"MomentSumMax":"MomentSumMin"];
			vTemp.t2 = n;
			vTemp.r  = double(bPositiveM);
			vTemp.t1 = ABS(m_MapSecStress[bPositiveM?"MomentSumMax":"MomentSumMin"]);

			vArrJewon.Add(vTemp);

			pBx = Finder.GetBxNext();
		}
	}
	///< 각 이음부 단면당 최대모멘트가 발생하는 순서로 sort함 : ABS(모멘트)
	vArrJewon.Sort(2, FALSE);   

	///< 이음판의 제원이 같은 것을 제거함.
	long nSize = vArrJewon.GetSize();
	CPlateBasicIndex *pBx=NULL, *pBx2=NULL;
	CPlateSplice     *pSp=NULL, *pSp2=NULL;
	long   nG=0;
	double dSta=0, dSta2=0;
	long k = 0;
	for(k=0; k<vArrJewon.GetSize()-1; k++)
	{
		nG   = (long)vArrJewon.GetAt(k).t2;
		dSta = vArrJewon.GetAt(k).h;
		pBx  = pDB->GetGirder(nG)->GetBxByStation(dSta, BX_SPLICE);
		pSp  = pBx->GetSplice();

		for(long l=k+1; l<nSize; l++)
		{
			dSta2 = vArrJewon.GetAt(l).h;
			pBx2  = pDB->GetGirder(nG)->GetBxByStation(dSta2, BX_SPLICE);
			pSp2  = pBx2->GetSplice();
			if(pSp == pSp2)
			{
				vArrJewon.RemoveAt(l--);
				nSize--;
			}
		}
	}
	//  이음판을 거더1번부터 재배열함
	CDPointArray xy;
	for(k=0; k<vArrJewon.GetSize(); k++)
	{
		CJewon Jewon =  vArrJewon.GetAt(k);
		xy.Add(CDPoint(k,Jewon.t2*100000000 + Jewon.h));
	}

	xy.Sort(FALSE, TRUE);   
	for(k=0; k<vArrJewon.GetSize(); k++)
		vArrJewonNew.Add(vArrJewon[(int)xy[k].x]);

	//  m_ArrSecSpDesign 생성
	pData->m_ArrSecSpDesign.RemoveAll();
	CVector vPos;
	for(k=0; k<vArrJewonNew.GetSize(); k++)
	{
		CJewon Jewon =vArrJewonNew.GetAt(k);
		vPos.x = Jewon.t2;	// 거더 
		vPos.y = Jewon.h;	// 스테이션
		vPos.z = 0;
		pData->m_ArrSecSpDesign.Add(vPos);
	}
}
/*
/////////////////////////////////////////////////////////////////////////////////
// * 현재 위치의 Materials이 합성전 사하중의 Materials와 같은 것이 있는지 검사
//   - 동일 단면이 있으면 제거
//   - 추가 단면이 있으면 꼬리에 추가함.
/////////////////////////////////////////////////////////////////////////////////
void CFemStressCalc::SetSectionNameAndRemoveMinSpace()
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();  
	CFEMManage      *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CCalcGeneral	CalcGen(m_pDataManage);

	long nMatSttID = 0;
	POSITION pos = pManage->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pManage->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag==POS_GIRDER)
			nMatSttID++;
	}
	long   nMaterialsu = nMatSttID;
	long   nJointSu  = pManage->GetJointSize();
	long   nMatNo    = 0;
	double dSttI33   = 0;
	double dSttI33_2 = 0;
	double dSttI33_3 = 0;
	double dStaMid   = 0;
	double dMI33     = 0;
	double dI33 = 0;
	CString szJewon[3], szTemp;
	// 합성전 단면..				
	for (long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		pData->m_ArrSecIndex[nG].RemoveAll();

		BOOL bInsertMoment	= InsertMomentPos(nG);
		long nSize = pData->m_ArrSecSta[nG].GetSize();
		long nCheckByun = 0;
		for(long i=0; i<nSize-1; i++)
		{
			long nByun     = pData->m_ArrSecByunType[nG].GetAt(i);
			long nByunNext = pData->m_ArrSecByunType[nG].GetAt(i+1);
			long nByunPre  = i==0 ? nByun : pData->m_ArrSecByunType[nG].GetAt(i-1);
			double	dStaStt	= pData->m_ArrSecSta[nG].GetAt(i);
			double	dStaEnd	= pData->m_ArrSecSta[nG].GetAt(i+1);
			
			if(nByun==0)			// 현재 단면이 등단면 일때 
			{
				if(nByunNext==0)	// 다음 단면이 등단면 일때
				{
					CPlateBasicIndex *pBxStt = pGir->GetBxByStation(dStaStt);
					if(pBxStt->IsState(BX_STT_BENDING_UPPER))
						dStaMid = GetStationCheckPos(pBxStt);
					else if(pBxStt->IsJijum() && EQUAL(pBxStt->GetStation(), (pGir->GetSSS(OFF_STT_GIRDER)+pDB->m_dLengthGirderShoeStt), 1))
						dStaMid = GetStationCheckPos(pBxStt);
					else if(pBxStt->IsState(BX_END_BENDING_UPPER))
						dStaMid = pGir->GetSSS(OFF_END_GIRDER)-pDB->m_dLengthGirderShoeEnd;
					else
						dStaMid = (dStaStt+dStaEnd)/2;
				}
				else
					dStaMid = dStaStt+1;
				
				ModelCalc.GetSectionCoefficient(nG, dStaMid);
			}
			else				// 현재 단면이 변단면 일때 
			{
				ModelCalc.GetSectionCoefficient(nG, dStaStt+10);
				dSttI33_2 = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS);
				ModelCalc.GetSectionCoefficient(nG, dStaStt-10);
				dSttI33_3 = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS);
				ModelCalc.GetSectionCoefficient(nG, dStaStt);
				dSttI33   = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS);
				
				// 지점이 아니고 100mm이격된 곳에서와 같은 I33 일 때
				if(IsJijumByStation(dStaStt)==FALSE && (ModelCalc.IsSameI33(dSttI33,dSttI33_2) || ModelCalc.IsSameI33(dSttI33,dSttI33_3)) && nByunNext==0)
					pData->m_ArrSecByunType[nG].SetAt(i,0);
			}
			CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxByStation(dStaStt);
			CalcGen.GetMaterial(pBx, G_F_U, szJewon[FRM_UPPER], szTemp);
			CalcGen.GetMaterial(pBx, G_F_L, szJewon[FRM_LOWER], szTemp);
			CalcGen.GetMaterial(pBx, G_W,   szJewon[FRM_WEB], szTemp);
			CDPoint Propty[3];
			Propty[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
			Propty[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
			Propty[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);

			CFrameSection *pFrameSec	= (CFrameSection*)ModelCalc.GetFrameSectionByPrpty(pManage, Propty, szJewon);
			// 변단면 고려 ///////////////////////////////////////////////////////////
//			if(i<nSize-2)
//			{
//				CDPoint ProptyMid[3], ProptyEnd[3];
//
//				ModelCalc.GetSectionCoefficient(nG, (dStaStt+dStaEnd)/2);
//				ProptyMid[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
//				ProptyMid[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
//				ProptyMid[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);
//
//				ModelCalc.GetSectionCoefficient(nG, dStaEnd);
//				ProptyEnd[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
//				ProptyEnd[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
//				ProptyEnd[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);
//
//				CFrameSection *pFrameEnd	= (CFrameSection*)ModelCalc.GetFrameSectionByPrpty(pManage, ProptyEnd, szJewon);
				// 단면 진행중에 제원이 변경된 경우 *변단면*
//				if(pFrameEnd && (Propty[0]!=ProptyMid[0] || Propty[1]!=ProptyMid[1] || Propty[2]!=ProptyMid[2]))
//				{
//					if(!IsJijumByStation(dStaStt) && nByun!=nByunPre)	// 이전 단면과 단면타입이 다를경우
//						pFrameSec = pFrameEnd;		
//				}
//			}
			//////////////////////////////////////////////////////////////////////////			

			if(pFrameSec==NULL)
				pData->m_ArrSecIndex[nG].Add(++nMatSttID);				// 단면이 기존에 없는 경우
			else if(bInsertMoment && IsMomentPos(nG, dStaStt) && IsMomentPos(nG, dStaEnd))
				pData->m_ArrSecIndex[nG].Add(++nMatSttID);
			else
				pData->m_ArrSecIndex[nG].Add(atol(pFrameSec->m_szName));// 단면이 FrameSection에 있는 경우 기존 단면의 이름을 설정
		}

		// 맨 마지막 단면. 등단면 일경우 : 앞 단면을 그대로 입력. 변단면일 경우 체크
		long nByun = pData->m_ArrSecByunType[nG].GetAt(nSize-1);
		if(nByun==0)	// 등단면일경우
		{
			double dStt = pData->m_ArrSecSta[nG].GetAt(nSize-2);
			double dEnd = pData->m_ArrSecSta[nG].GetAt(nSize-1);
			dStaMid = (dStt+dEnd)/2;	// 현재단면과 다음단면의 중앙스테이션

			ModelCalc.GetSectionCoefficient(nG, dStaMid);

			CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxByStation(dStaMid);
			CalcGen.GetMaterial(pBx, G_F_U, szJewon[FRM_UPPER], szTemp);
			CalcGen.GetMaterial(pBx, G_F_L, szJewon[FRM_LOWER], szTemp);
			CalcGen.GetMaterial(pBx, G_W,   szJewon[FRM_WEB], szTemp);
			CDPoint Propty[3];
			Propty[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
			Propty[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
			Propty[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);

			CFrameSection *pFrameSec	= (CFrameSection*)ModelCalc.GetFrameSectionByPrpty(pManage, Propty, szJewon);

			if(pFrameSec==NULL)
				pData->m_ArrSecIndex[nG].Add(++nMatSttID);				// 단면이 기존에 없는 경우
			else if(bInsertMoment && IsMomentPos(nG, dStt) && IsMomentPos(nG, dEnd))
				pData->m_ArrSecIndex[nG].Add(++nMatSttID);
			else
				pData->m_ArrSecIndex[nG].Add(atol(pFrameSec->m_szName));// 단면이 FrameSection에 있는 경우 기존 단면의 이름을 설정
		}
		else
		{
			CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxByStation(pData->m_ArrSecSta[nG].GetAt(nSize-1));
			ModelCalc.GetSectionCoefficient(nG, pData->m_ArrSecSta[nG].GetAt(nSize-1));
			dSttI33 = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS);

			CDPoint Propty[3];
			CalcGen.GetMaterial(pBx, G_F_U, szJewon[FRM_UPPER], szTemp);
			CalcGen.GetMaterial(pBx, G_F_L, szJewon[FRM_LOWER], szTemp);
			CalcGen.GetMaterial(pBx, G_W,   szJewon[FRM_WEB], szTemp);
			Propty[0] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_UP"], ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
			Propty[1] = CDPoint(ModelCalc.m_MapSecCoe["FLANGE_W_DN"], ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
			Propty[2] = CDPoint(ModelCalc.m_MapSecCoe["WEB_H"], ModelCalc.m_MapSecCoe["WEB_T"]);
			
			CFrameSection *pFrameSec = ModelCalc.GetFrameSectionByPrpty(pManage, Propty, szJewon);
			if(pFrameSec==NULL)		// 단면이 기존에 없는 경우
				pData->m_ArrSecIndex[nG].Add(nMatSttID++);
			else
				pData->m_ArrSecIndex[nG].Add(atol(pFrameSec->m_szName));
		}

		// m_dMatMinSpace(단면검토 최소 간격) 이하의 단면 변화에 대해서 무시(제거)
		for(long k=1; k<pData->m_ArrSecSta[nG].GetSize()-1; k++)
		{	
			long nMatId = pData->m_ArrSecIndex[nG].GetAt(k);
			double dSta = pData->m_ArrSecSta[nG].GetAt(k);
			double dEnd = pData->m_ArrSecSta[nG].GetAt(k+1);
			// 단면과 단면의 간격...
			if((dEnd-dSta)<pData->m_dMatMinSpace) 
			{
				// 거더의 단면(가정단면B)은 반드시 나와야 한다.
				if(nMaterialsu+1 > nMatId)	continue;
	  
				ModelCalc.GetSectionCoefficient(nG, dSta);
				double	dI33Stt	= ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS);
				ModelCalc.GetSectionCoefficient(nG, dEnd);
				double	dI33End	= ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS);
				nSize--;
				if(dI33Stt>dI33End)
				{
					pData->m_ArrSecIndex[nG].RemoveAt(k+1);
					pData->m_ArrSecSta[nG].RemoveAt(k+1);
					pData->m_ArrSecByunType[nG].RemoveAt(k+1);
				}
				else
				{
					pData->m_ArrSecIndex[nG].RemoveAt(k);
					pData->m_ArrSecSta[nG].RemoveAt(k);
					pData->m_ArrSecByunType[nG].RemoveAt(k);
				}
				k--;
			}
		}

		// 같은 단면 번호 제거
		// 등단면에서 등단면으로에서만 제거 해야한다.
		long nCount;
		long nSizeSec = 0;
		do
		{
			long nSize = pData->m_ArrSecIndex[nG].GetSize();
			nCount = 0;
			for(long k=0; k<nSize-2; k++)
			{
//				if(!pData->m_ArrSecByunType[nG].GetAt(k) && !pData->m_ArrSecByunType[nG].GetAt(k+1))
				if(pData->m_ArrSecByunType[nG].GetAt(k)==0 && pData->m_ArrSecByunType[nG].GetAt(k+1)==0)
				{
					long SM = pData->m_ArrSecIndex[nG].GetAt(k);
					long EM = pData->m_ArrSecIndex[nG].GetAt(k+1);
					
					if(EM == SM)
					{					
						pData->m_ArrSecSta[nG].RemoveAt(k+1);
						pData->m_ArrSecIndex[nG].RemoveAt(k+1);
						pData->m_ArrSecByunType[nG].RemoveAt(k+1);
						nCount++; nSize--;
					}
				}
			}
		} while(nCount>0);
	}
}

// 추가 단면(sap input단면과 틀린단면 - 절점 사이에 존재하는 단면)이
// 여러개 일때 비교하여 같은 단면은 같은 단면 번호로...
void CFemStressCalc::SetSameSectionToOne()
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CFEMManage      *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CString sBujeU, sBujeL, sBujeW, sTemp, sBujeU2, sBujeL2, sBujeW2;
	CStringArray sFlangeUp;
	CStringArray sFlangeLo;
	CStringArray sFlangeWeb;
	CDoubleArray nMatI33;
	CDoubleArray nFlangeTUp;
	CDoubleArray nFlangeTLo;
	CDWordArray  nMatid;
	CDWordArray  nGid;
	CDWordArray  nMatNum;
	CDWordArray  nCheck;

	// material num 중에서 최대 번호 찾기.
	long nJointSu = pFEM->GetJointSize();
	long ngirder  = pDB->GetGirdersu();
	// Material Num 중에서 최대 번호 찾기(추가 단면은 번호를 현재 단면의 마지막 다음부터 정의함)
	long   nMatSttID = 0;
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag==POS_GIRDER)
			nMatSttID++;
	}
	
	long nCount = 0;
	sFlangeUp.RemoveAll();
	sFlangeLo.RemoveAll();
	sFlangeWeb.RemoveAll();
	nMatI33.RemoveAll();
	nMatid.RemoveAll();
	nGid.RemoveAll();
	nMatNum.RemoveAll();
	nCheck.RemoveAll();
	nFlangeTUp.RemoveAll();
	nFlangeTLo.RemoveAll();
	
	for (long nG = 0; nG<ngirder; nG++)
	{			
		long	nSize			= pData->m_ArrSecSta[nG].GetSize();
		BOOL	bInsertMoment	= InsertMomentPos(nG);
		
		// sap input 이외의 단면에 대한 이차모멘트 저장.
		for(long i=0; i<nSize-1; i++)  
		{
			long matid = pData->m_ArrSecIndex[nG].GetAt(i);
			double dStaStt = pData->m_ArrSecSta[nG].GetAt(i);
			double dStaEnd = pData->m_ArrSecSta[nG].GetAt(i+1);
			if(bInsertMoment && IsMomentPos(nG, dStaStt) && IsMomentPos(nG, dStaEnd))
				continue;

			if(matid>nMatSttID)
			{
				double dStaCheck = 0;
				long nByun     = pData->m_ArrSecByunType[nG].GetAt(i);
				long nByunNext = pData->m_ArrSecByunType[nG].GetAt(i+1);
				if(nByun==0)			// 현재 단면이 등단면 일때 
				{
					double dStt = pData->m_ArrSecSta[nG].GetAt(i);
					double dEnd = pData->m_ArrSecSta[nG].GetAt(i+1);
					
					if(!nByunNext)	// 다음 단면이 등단면 일때
						dStaCheck = (dStt+dEnd)/2;	// 현재단면과 다음단면의 중앙스테이션
					else
						dStaCheck = dStt+1;			// 현재단면의 스테이션
				}
				else				// 현재 단면이 변단면 일때 
					dStaCheck = pData->m_ArrSecSta[nG].GetAt(i);
				
				ModelCalc.GetSectionCoefficient(nG, dStaCheck);
				double dSttI33 = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS);//ModelCalc.m_MapSecCoe["I33"];
				CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxByStation(dStaCheck);

				pDB->GetRegulationSteelMaterial(G_F_U,	pBx->GetFactChain(G_F_U)->m_strSymbol,	sBujeU, sTemp);
				pDB->GetRegulationSteelMaterial(G_F_L,	pBx->GetFactChain(G_F_L)->m_strSymbol,	sBujeL, sTemp);
				pDB->GetRegulationSteelMaterial(G_W,	pBx->GetFactChain(G_W)->m_strSymbol,	sBujeW, sTemp);
				sFlangeUp.Add(sBujeU);
				sFlangeLo.Add(sBujeL);
				sFlangeWeb.Add(sBujeW);
				nMatI33.Add(dSttI33);
				nFlangeTUp.Add(ModelCalc.m_MapSecCoe["FLANGE_T_UP"]);
				nFlangeTLo.Add(ModelCalc.m_MapSecCoe["FLANGE_T_DN"]);
				nMatid.Add(i);
				nMatNum.Add(matid);
				nGid.Add(nG);
				nCheck.Add(0);

				//TRACE3("extra nMatI33[%d] : I33 =  %f,,,, %d \n", matid,dSttI33, i);				
			}			
		}
	}
		
	long nSize = nMatI33.GetSize();
	if(nSize<2) return;
	long nNum = nMatNum.GetAt(0);
	for(long i=0; i<nSize; i++)
	{
		long lCheck = nCheck.GetAt(i);
		if(lCheck) continue;
		
		double dI33_1 = nMatI33.GetAt(i);
		double dTup_1 = nFlangeTUp.GetAt(i);
		double dTlo_1 = nFlangeTLo.GetAt(i);
		sBujeU = sFlangeUp.GetAt(i);
		sBujeL = sFlangeLo.GetAt(i);
		sBujeW = sFlangeWeb.GetAt(i);
		for(long j=i+1; j<nSize; j++)
		{
			long nId = nMatid.GetAt(j);
			double dI33_2 = nMatI33.GetAt(j);
			double dTup_2 = nFlangeTUp.GetAt(j);
			double dTlo_2 = nFlangeTLo.GetAt(j);
			sBujeU2 = sFlangeUp.GetAt(j);
			sBujeL2 = sFlangeLo.GetAt(j);
			sBujeW2 = sFlangeWeb.GetAt(j);
			long nGir = nGid.GetAt(j);
			if(ModelCalc.IsSameI33(dI33_2,dI33_1) && dTup_1==dTup_2 && dTlo_1==dTlo_2 && 
				sBujeU==sBujeU2 && sBujeL==sBujeL2 && sBujeW==sBujeW2)
			{
				pData->m_ArrSecIndex[nGir].SetAt(nId,nNum);
				nCheck.SetAt(j,(long)1);
			}
		}
		long nId = nMatid.GetAt(i);
		long nGir = nGid.GetAt(i);
		
		pData->m_ArrSecIndex[nGir].SetAt(nId,nNum);
		nNum++;		
	}
}

//////////////////////////////////////////////////////////////////////////////////
// 일정구간(가로보 간격) 사이에 변단면 여부 구함.
// return  0 : 변단면 아님
//        -1 : ↙ 방향으로 기울어짐
//         1 : ↘ 방향으로 기울어짐
//////////////////////////////////////////////////////////////////////////////////
long CFemStressCalc::GetByunDanStyle(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CPlateBasicIndex *&pBxTerm)
{
	long nG = pBxStt->GetNumberGirder();

	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
		
	CPlateBxFinder Finder(pGir);

	double	dStaStt	= pBxStt->GetStation();
	double	dStaEnd	= pBxEnd->GetStation();

	for(long nK = 0; nK<pDB->m_nQtyJigan; nK++)
	{
		pBxTerm = NULL;
		CPlateBasicIndex *pBxKyungGanStt = pGir->GetBxOnJijum(nK);
		CPlateBasicIndex *pBxKyungGanEnd = pGir->GetBxOnJijum(nK+1);
		// ↙방향
		CPlateBasicIndex *pBxSTTLOW = Finder.GetBxFirst(BX_STT_BYUN_LOWER, pBxKyungGanStt, pBxKyungGanEnd, FALSE, FALSE);
		CPlateBasicIndex *pBxSTTUPP = Finder.GetBxFirst(BX_STT_BYUN_UPPER, pBxKyungGanStt, pBxKyungGanEnd, FALSE, FALSE);

		if(pBxSTTLOW != NULL && pBxSTTUPP != NULL)
		{
			pBxTerm = Finder.GetBxFirst(BX_STT_BYUN_LOWER | BX_STT_BYUN_UPPER, pBxStt, pBxEnd);
			if(dStaEnd > pBxSTTLOW->GetStation() && dStaStt < pBxSTTUPP->GetStation())
				return -1;
		}
		// ↘방향
		CPlateBasicIndex *pBxENDUPP = Finder.GetBxFirst(BX_END_BYUN_UPPER, pBxKyungGanStt, pBxKyungGanEnd, FALSE, FALSE);
		CPlateBasicIndex *pBxENDLOW = Finder.GetBxFirst(BX_END_BYUN_LOWER, pBxKyungGanStt, pBxKyungGanEnd, FALSE, FALSE);
		
		if(pBxENDUPP != NULL && pBxENDLOW != NULL)
		{
			pBxTerm = Finder.GetBxFirst(BX_END_BYUN_UPPER | BX_END_BYUN_LOWER, pBxStt, pBxEnd);
			if(dStaEnd > pBxENDUPP->GetStation() && dStaStt < pBxENDLOW->GetStation())
				return 1;
		}
	}
	return 0;
}

// 일정구간(다이아프램 간격) 사이에 변단면 여부 구함.
// return  0 : 변단면 아님
//        -1 : ↙ 방향으로 기울어짐
//         1 : ↘ 방향으로 기울어짐
long CFemStressCalc::GetByunDanStyle(long nStt, long nEnd, double &dStaByun)
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);

	long nG = ModelCalc.GetGirderNoByJointID(nStt);
	CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);

	double dStaI = pFEM->GetJoint(nStt)->m_vPointReal.x;
	double dStaJ = pFEM->GetJoint(nEnd)->m_vPointReal.x;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_BYUN_UPPER | BX_END_BYUN_UPPER | BX_END_GIRDER);
	BOOL bByun = pBx->IsState(BX_STT_BYUN_UPPER);

	long nIState = -1, nJState = -1;
	while (pBx)
	{
		double sta = pBx->GetStation();
		if (sta == dStaI)
			nIState = 0;
		if (sta == dStaJ)
			nJState = 0;
		if (nIState < 0 && sta > dStaI)
			nIState = (long)bByun;
		if (nJState < 0 && sta > dStaJ)
			nJState = (long)bByun;
		if (nIState >= 0 && nJState >= 0)
			break;
		if (pBx->IsState(BX_END_BYUN_UPPER))
			bByun = TRUE;
		if (pBx->IsState(BX_STT_BYUN_UPPER))
			bByun = FALSE;
		pBx = Finder.GetBxNext();
	}

	if(nIState >= 0 && nJState < 0)			dStaByun = Finder.GetBxFirst(BX_STT_BYUN_UPPER)->GetStation();
	else if(nJState >= 0 && nIState < 0)	dStaByun = Finder.GetBxFirst(BX_END_BYUN_UPPER)->GetStation();
	else									dStaByun = -1;
		
	if (nIState || nJState)   
	{
		double staMid = (dStaI + dStaJ) / 2;
		long nJijum = pGir->GetNumberJijumByStation(staMid);
		long nJijan = pGir->GetNumberJiganByStation(staMid);
		return (nJijum == nJijan) ? -1 : 1;
	}
	return 0;
}

int CFemStressCalc::IsByunDanStyle(long nG, double dSttSta, double dEndSta)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	double dCurSta = 0, dSttStaByun = 0, dEndStaByun = 0;

	long nMaxNodeIdx=0, nMinNodeIdx=0;
	ModelCalc.GetJointRangeByGirder(nG, nMinNodeIdx, nMaxNodeIdx);
	long nRestraintZ = 0;

	for(long nNodeIdx = nMinNodeIdx + 1; nNodeIdx < nMaxNodeIdx; nNodeIdx++)
	{
		nRestraintZ = pFEM->GetJoint(nNodeIdx)->m_UZ;
		if(!nRestraintZ) continue;

		dCurSta = pFEM->GetJoint(nNodeIdx)->m_vPointReal.x;

		if(dCurSta > dSttSta && dCurSta < dEndSta) return FALSE;
	}

	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_BYUN_LOWER | BX_STT_BYUN_UPPER);
	while(pBx)
	{
		dCurSta = pBx->GetStation();

		if(pBx->IsState(BX_STT_BYUN_LOWER))			dSttStaByun = dCurSta;
		if(pBx->IsState(BX_STT_BYUN_UPPER))			dEndStaByun = dCurSta;

		if(dSttStaByun >= dSttSta && dSttStaByun <  dEndSta) return TRUE;
		if(dEndStaByun >  dSttSta && dEndStaByun <= dEndSta) return TRUE;
		if(dSttStaByun <= dSttSta && dEndStaByun >  dEndSta) return TRUE;

		pBx = Finder.GetBxNext();
	}

	pBx = Finder.GetBxFirst(BX_END_BYUN_LOWER | BX_END_BYUN_UPPER);
	while(pBx)
	{
		dCurSta = pBx->GetStation();

		if(pBx->IsState(BX_END_BYUN_UPPER))			dSttStaByun = dCurSta;
		if(pBx->IsState(BX_END_BYUN_LOWER))			dEndStaByun = dCurSta;

		if(dSttStaByun >= dSttSta && dSttStaByun <  dEndSta) return TRUE;
		if(dEndStaByun >  dSttSta && dEndStaByun <= dEndSta) return TRUE;
		if(dSttStaByun <= dSttSta && dEndStaByun >  dEndSta) return TRUE;

		pBx = Finder.GetBxNext();
	}

	return FALSE;
}
*/
// 특정위치의 지점이 변단면 사이에 존재하는지 여부를 구함.
// return  0 : 변단면 아님
//        -1 : ↙ 방향으로 기울어짐
//         1 : ↘ 방향으로 기울어짐
long CFemStressCalc::GetByunDanStyle(CPlateBasicIndex *pBx)
{
	long nG = pBx->GetNumberGirder();

	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CPlateBxFinder Finder(pGir);
		
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
	
	BOOL bStt = pBxStt->GetStation()==pBx->GetStation() ? TRUE : FALSE;
	BOOL bEnd = pBxEnd->GetStation()==pBx->GetStation() ? TRUE : FALSE;

	double SttSta = pBx->GetStation() - 50;
	double EndSta = pBx->GetStation() + 50;
	if(pBx->IsState(BX_STT_BENDING_UPPER|BX_END_BENDING_UPPER))
		return 0;

	double dH_Inode = pGir->GetHeightGirderByStaAng(SttSta,0,CDPoint(0,1),FALSE);
	double dH_Jnode = pGir->GetHeightGirderByStaAng(EndSta,0,CDPoint(0,1),FALSE);

	if(pBx->IsJijum() && !bStt && !bEnd)
	{
		double SttINode  = pBx->GetStation();
		double FrontISta = pBx->GetStation() - 5000;
		double BackISta  = pBx->GetStation() + 5000;
		
		double dH_Front = pGir->GetHeightGirderByStaAng(FrontISta,0,CDPoint(0,1),FALSE);
		double dH_Back  = pGir->GetHeightGirderByStaAng(BackISta,0,CDPoint(0,1),FALSE);
		double dH_IPos  = pGir->GetHeightGirderByStaAng(SttINode,0,CDPoint(0,1),FALSE);

		if(dH_Front == dH_IPos && dH_Back==dH_IPos) 
			return 0;
		else
			return -1;
	}

	if(dH_Inode > dH_Jnode) return -1;
	if(dH_Inode < dH_Jnode) return  1;
	
	return 0;
}

void CFemStressCalc::SetSectionCrossBeamPos()
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CPlateBridgeApp	*pDB			= m_pDataManage->GetBridge();
	CPlateCrossBeam	*pCrMaxEnd		= NULL;
	CVBracing		*pVBrMaxEnd		= NULL;
	CPlateCrossBeam	*pCrMaxCent		= NULL;
	CVBracing		*pVBrMaxCent	= NULL;

	double	dCrMaxEnd	= 0;
	double	dCrMaxCent	= 0;
	double  dVBrMaxEnd	= 0;
	double	dVBrMaxCent	= 0;
	double	dValue		= 0;
	double	dSumMax		= 0;
	double	dSumMin		= 0;
	long	nEle		= 0;

	// 단부 가로보
	for(long nG=0; nG<pDB->GetQtyGirder()-1; nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			CSection *pSec = pBx->GetSection();
			nEle	= (long)ModelCalc.GetElemByCrossBx(pBx);
			if(nEle<0)
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			dSumMax	= fabs(GetOneEleMaxForce(ELE_MOMENT3, LC_SUM_MAX, nEle));
			dSumMin	= fabs(GetOneEleMaxForce(ELE_MOMENT3, LC_SUM_MIN, nEle));
			dValue	= MAX(dSumMax, dSumMin);

			if(pSec->IsenType(SECTION_A))
			{
				if(pSec->GetenDetType()==SECDET_VBRACING)
				{	
					if(dVBrMaxEnd < dValue)
					{
						dVBrMaxEnd	= dValue;
						pVBrMaxEnd	= pSec->GetVBracing();
					}
				}
				else
				{
					if(dCrMaxEnd < dValue)
					{
						dCrMaxEnd	= dValue;
						pCrMaxEnd	= pSec->GetCrossBeam();
					}
				}
			}
			else
			{
				if(pSec->GetenDetType()==SECDET_VBRACING)
				{	
					if(dVBrMaxCent < dValue)
					{
						dVBrMaxCent	= dValue;
						pVBrMaxCent	= pSec->GetVBracing();
					}
				}
				else
				{
					if(dCrMaxCent < dValue)
					{
						dCrMaxCent	= dValue;
						pCrMaxCent	= pSec->GetCrossBeam();
					}
				}
			}

			pBx = Finder.GetBxNext();
		}
	}

	if(pVBrMaxEnd)	pVBrMaxEnd->SetSectionCheck(TRUE);
	if(pCrMaxEnd)	pCrMaxEnd->SetSectionCheck(TRUE);
	if(pVBrMaxCent)	pVBrMaxCent->SetSectionCheck(TRUE);
	if(pCrMaxCent)	pCrMaxCent->SetSectionCheck(TRUE);
}

void CFemStressCalc::SetSectionVBracingPos()
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CCalcData       *pData   = m_pDataManage->GetCalcData();

	CTypedPtrArray < CObArray, CPlateBasicIndex* > DArrVBracingBx;
	// 출력 부재 인덱스 구하기
	ModelCalc.GetBxArrayOfVBracing(DArrVBracingBx);
	// 중복 인덱스 제거
	ModelCalc.RemoveDupTypedArray(DArrVBracingBx);

	CVector vTemp;
	pData->m_ArrSecVBracing.RemoveAll();

	for(long n=0; n<DArrVBracingBx.GetSize(); n++)
	{
		CPlateBasicIndex *pBx = DArrVBracingBx.GetAt(n);	
		vTemp.x = pBx->GetNumberGirder();
		vTemp.y = pBx->GetStation();

		pData->m_ArrSecVBracing.Add(vTemp);
	}
}

/// 일단 임시로 첫번재 두번째 수평브레이싱을 적용함(1번거더)
void CFemStressCalc::SetSectionHBracingPos()
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CCalcData       *pData   = m_pDataManage->GetCalcData();
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();  

	CTypedPtrArray < CObArray, CPlateBasicIndex* > DArrHBracingBx;

	CPlateBxFinder Finder(pDB->GetGirder(0));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nCount = 0;
	while(pBx)
	{
		CBracing *pBr = pBx->GetBracing();
		if(nCount==0 || nCount==1)
			DArrHBracingBx.Add(pBx);
		pBx = Finder.GetBxNext();
		nCount++;
	}

	CPlateBxFinder FinderRight(pDB->GetGirder(pDB->GetGirdersu()-1));
	pBx = FinderRight.GetBxFirst(BX_CROSSBEAM_VBRACING);
	nCount = 0;
	while(pBx)
	{
		CBracing *pBr = pBx->GetBracing();
		if(nCount==0 || nCount==1)
			DArrHBracingBx.Add(pBx);
		pBx = FinderRight.GetBxNext();
		nCount++;
	}
	
	CVector vTemp;
	pData->m_ArrSecHBracing.RemoveAll();

	for(long n=0; n<DArrHBracingBx.GetSize(); n++)
	{
		CPlateBasicIndex *pBx = DArrHBracingBx.GetAt(n);	
		vTemp.x = pBx->GetNumberGirder();
		vTemp.y = pBx->GetStation();

		pData->m_ArrSecHBracing.Add(vTemp);
	}
}


void CFemStressCalc::DeleteOnNotUsedExtSection(int nG)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);

	if(m_pCalcStd->m_vByunDanInfo.size()<=0)	return;

	int nSize = m_pCalcStd->m_vByunDanInfo[nG].size();
	for(int i=0; i<nSize-1; i++)
	{
		CDanMyunProp *pSectI = &m_pCalcStd->m_vByunDanInfo[nG][i];
		CDanMyunProp *pSectJ = &m_pCalcStd->m_vByunDanInfo[nG][i+1];
				
		double SectIMaxM_L =pSectI->m_dMaxMoment_Left;
		double SectIMaxM_R =pSectI->m_dMaxMoment_Right;
		double SectJMaxM_L =pSectJ->m_dMaxMoment_Left;
		
		double SectIMinM_L =pSectI->m_dMinMoment_Left;
		double SectIMinM_R =pSectI->m_dMinMoment_Right;
		double SectJMinM_L =pSectJ->m_dMinMoment_Left;

		// 교각 위치에서 모멘트도 그리기
		if( (pSectI->m_pBx->IsPier())	|| (pSectJ->m_pBx->IsPier()) )
		{
			if(pSectI->m_pBx->IsPier())	pSectI->m_MatLeft.m_bUsedCalc = TRUE;
			else						pSectJ->m_MatLeft.m_bUsedCalc = TRUE;
		}
		else
		{
			// MaxMoment
			double dStaSecI  = pSectI->m_pBx->GetStation();
			double dValSecI  = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, 0, dStaSecI, dStaSecI, 0);
			
			double dStaSecJ  = pSectJ->m_pBx->GetStation();
			double dValSecJ  = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, 0, dStaSecJ, dStaSecJ, 0);		

			if(dValSecI > 0 || dValSecJ > 0)
			{			
				if(!pSectI->IsByunDanMyun() && ! pSectJ->IsByunDanMyun())
				{
					if(pSectI->m_MatRight.m_dI33 < pSectJ->m_MatLeft.m_dI33)	pSectI->m_MatRight.m_bUsedCalc = TRUE;
					else														pSectJ->m_MatLeft.m_bUsedCalc = TRUE;					
				}	
				if(pSectI->IsByunDanMyun() && SectIMaxM_L>0) pSectI->m_MatRight.m_bUsedCalc = TRUE;
				if(pSectJ->IsByunDanMyun() && SectJMaxM_L>0) pSectJ->m_MatRight.m_bUsedCalc = TRUE;
				if(SectIMaxM_R<0 && SectJMaxM_L>0) pSectI->m_MatLeft.m_bUsedCalc = TRUE;
				else if(SectJMaxM_L<0 && SectIMaxM_R>0) pSectJ->m_MatLeft.m_bUsedCalc = TRUE;				
			}
			
			// MinMoment
			long nPierI = pGir->GetNumberJijumByBx(pSectI->m_pBx);
			long nPierJ = pGir->GetNumberJijumByBx(pSectJ->m_pBx);
			if ( (pSectI->m_pBx->IsJijum() && (nPierI == 0 && dValSecJ > dValSecI)) || 
				 (pSectJ->m_pBx->IsJijum() && (nPierJ == pDB->m_nQtyJigan && dValSecJ < dValSecI)))
			{
				continue;   // 거더의 첫 부분과 끝부분에서 발생하는 - Moment는 처리안함.
			}
			if(SectIMinM_R < 0 || SectJMinM_L < 0)
			{								
				if(!pSectI->IsByunDanMyun() && ! pSectJ->IsByunDanMyun())
				{
					if(pSectI->m_MatRight.m_dI33 < pSectJ->m_MatLeft.m_dI33)	pSectI->m_MatRight.m_bUsedCalc = TRUE;
					else														pSectJ->m_MatLeft.m_bUsedCalc = TRUE;					
				}
				if(pSectI->IsByunDanMyun() && SectIMinM_L<0)	pSectI->m_MatRight.m_bUsedCalc = TRUE;
				if(pSectJ->IsByunDanMyun() && SectJMinM_L<0)	pSectJ->m_MatRight.m_bUsedCalc = TRUE;					
			}						
		}		
	}	
}

/////////////////////////////////////////////////////////////////////////
///  * 단면구성도에 있는 단면저항모멘트를 생성함
///     - m_ArrSecSta은 SetSectionGugan()에서 계산됨
/////////////////////////////////////////////////////////////////////////
void CFemStressCalc::MakeDanmyunMomentData(long nG)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();		

	CPlateGirderApp* pSGir = pDB->GetGirder(0);
	double staJi = pSGir->GetStationOnJijum(1);
	double dAbsMax = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MIN, 0, staJi, staJi, 1);  // 합계중 최소값.
	double dAbsMin = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MIN, 0, staJi, staJi, 2);  // 합계중 최소값.
	double dValue = fabs(dAbsMax) > fabs(dAbsMin) ? dAbsMax : dAbsMin;
	double dBound = fabs(dValue/8);

	long nSize = m_pCalcStd->m_vByunDanInfo[nG].size();
	for(int i=0; i<nSize; i++)
	{
		BOOL bPreByun = FALSE;
		BOOL bCurByun = FALSE;
		BOOL bNextByun = FALSE;

		CDanMyunProp* pPreSect = NULL;
		if(i > 0)
		{
			pPreSect = &m_pCalcStd->m_vByunDanInfo[nG][i-1];
			bPreByun = pPreSect->IsByunDanMyun();
		}

		CDanMyunProp* pCurSect = &m_pCalcStd->m_vByunDanInfo[nG][i];
		bCurByun = pCurSect->IsByunDanMyun();

		CDanMyunProp* pNextSect = NULL;
		if(i < nSize-1)
		{
			pNextSect = &m_pCalcStd->m_vByunDanInfo[nG][i+1];
			bNextByun = pNextSect->IsByunDanMyun();
		}

		// 현재 절점이 변단면 구간에 속할 때
		if(bCurByun)
			SetDanmyunMoment(NULL, pCurSect, NULL, CURRENT, dBound);
		else
		{
			// 시작 절점
			if(pPreSect == NULL)
			{
				if(bNextByun)	SetDanmyunMoment(NULL, pCurSect, NULL, CURRENT, dBound);
				else			SetDanmyunMoment(NULL, pCurSect, pNextSect, RIGHT, dBound);
			}
			else if(pNextSect == NULL) // 끝 절점
			{
				if(bPreByun)	SetDanmyunMoment(NULL, pCurSect, NULL, CURRENT, dBound);
				else			SetDanmyunMoment(pPreSect, pCurSect, NULL, LEFT, dBound);			
			}
			else
			{
				if(bPreByun && !bNextByun)			SetDanmyunMoment(NULL, pCurSect, pNextSect, RIGHT, dBound);
				else if(!bPreByun && bNextByun)		SetDanmyunMoment(pPreSect, pCurSect, NULL, LEFT, dBound);
				else if(bPreByun && bNextByun)		SetDanmyunMoment(NULL, pCurSect, NULL, CURRENT, dBound);
				else								SetDanmyunMoment(pPreSect, pCurSect, pNextSect, BOTH, dBound);
			}
		}
	}

	DeleteOnNotUsedExtSection(nG);
}

// nType = CURRENT : pSectI == pSectJ이며, 현재 위치에서의 모멘트 저장
//       = LEFT  : 단면의 Left  모멘트 저장
//       = RIGHT : 단면의 Right 모멘트 저장
//		 = BOTH  : 단면의 Left, Right 모멘트를 서로 다르게 저장
void CFemStressCalc::SetDanmyunMoment(CDanMyunProp *pPreSect, CDanMyunProp *pCurSect, CDanMyunProp *pNextSect, int nType, double dBound)
{
	double dMaxUp=0, dMaxLo=0, dCompUp=0, dCompLo=0, dTau=0;
	long nG = (int)pCurSect->m_pBx->GetNumberGirder();

	if(nType == CURRENT)
	{
		ASSERT(pCurSect != NULL);

		double dPointSta = pCurSect->m_pBx->GetStation();
				
		double dPointMinValue = GetElementForce(ELE_MOMENT3, LC_SUM_MIN, nG, dPointSta, 0);  // 지정된 위치의 값
		double dPointMin = dPointMinValue * GetMaxStressRatio(nG, dPointSta, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau);

		double dPointMaxValue = GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, dPointSta, 0);  // 지정된 위치의 값
		double dPointMax = dPointMaxValue * GetMaxStressRatio(nG, dPointSta, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau);
		
		pCurSect->m_dMinMoment_Left  = dPointMin;
		pCurSect->m_dMinMoment_Right = dPointMin;	
		
		pCurSect->m_dMaxMoment_Left  = dPointMax;
		pCurSect->m_dMaxMoment_Right = dPointMax;
	}
	else if(nType == LEFT)
	{
		ASSERT(pPreSect != NULL);
		ASSERT(pCurSect != NULL);

		double dSttStaMax = pPreSect->m_MatRight.m_pBx->GetStation();				
		double dSttStaMin = dSttStaMax;
		double dEndSta = pCurSect->m_MatLeft.m_pBx->GetStation();
		if(pPreSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dSttStaMax = pPreSect->m_pBx->GetStation();
		if(pCurSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dEndSta = pCurSect->m_pBx->GetStation();			

		double	dStaSec = dSttStaMax;
		long	nPosSec	= 1;
		double dTermMaxValue = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, nG, dSttStaMax, dEndSta, 1);  // 지정된 구간에서 최대값.
		double dTermMax  = dTermMaxValue * GetMaxStressRatio(nG, dSttStaMax, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);		
		
		double dTermMinValue = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MIN, nG, dSttStaMin, dEndSta, 2);  // 지정된 구간에서 최소값.
		double dTermMin  = dTermMinValue * GetMaxStressRatio(nG, dSttStaMin, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);		
		
		pCurSect->m_dMaxMoment_Left  = dTermMax;
		pCurSect->m_dMaxMoment_Right = dTermMax;
		
		pCurSect->m_dMinMoment_Left  = dTermMin;
		pCurSect->m_dMinMoment_Right = dTermMin;
	}
	else if(nType == RIGHT)
	{
		ASSERT(pCurSect  != NULL);
		ASSERT(pNextSect != NULL);
		
		double dSttStaMax = pCurSect->m_MatRight.m_pBx->GetStation();		
		double dSttStaMin = dSttStaMax;
		double dEndSta = pNextSect->m_MatLeft.m_pBx->GetStation();		

		if(pCurSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dSttStaMax = pCurSect->m_pBx->GetStation();	
		if(pNextSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dEndSta = pNextSect->m_pBx->GetStation();			
		

		// (APLATE-680) 우측단 단면 Station은 dEndSta 이고 Pos는 -1
		double	dStaSec = dEndSta;
		long	nPosSec	= -1;
		double dTermMaxValue = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, nG, dSttStaMax, dEndSta, 1);  // 지정된 구간에서 최대값.
		double dMaxRatio = GetMaxStressRatio(nG, dSttStaMax, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);
		double dTermMax = dTermMaxValue * dMaxRatio;
		
		double dTermMinValue = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MIN, nG, dSttStaMin, dEndSta, 2);  // 지정된 구간에서 최소값.
		double dMindRatio = GetMaxStressRatio(nG, dSttStaMin, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);
		double dTermMin  = dTermMinValue * dMindRatio;
		
		pCurSect->m_dMaxMoment_Left  = dTermMax;
		pCurSect->m_dMaxMoment_Right = dTermMax;
		
		pCurSect->m_dMinMoment_Left  = dTermMin;
		pCurSect->m_dMinMoment_Right = dTermMin;
	}
	else
	{
		ASSERT(pPreSect != NULL);
		ASSERT(pCurSect != NULL);
		ASSERT(pNextSect != NULL);

		// (APLATE-680) 단면 기준 좌측 단면 Station은 dEndSta 이고 Pos는 -1, 우측단면은 SttSta 이고 Pos-1;
		// Stt/End Station은 구간 기준으로 Section 좌우측은 다른 구간이다.
		// GetMaxStressRatio()에 들어가는 첫번째 Sta는 부재력위치, 두번째 Sta는 단면위치 Staion이다.

		//////////////////////////////////////////////////////////////////////////
		//  단면좌측 (최소간격으로 인해 삭제된 단면을 고려해야 함.
		//////////////////////////////////////////////////////////////////////////

		double dSttStaMax = pPreSect->m_MatRight.m_pBx->GetStation();		
		double dSttStaMin = dSttStaMax;
		double dEndSta    = pCurSect->m_MatLeft.m_pBx->GetStation();
		
		if(pPreSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dSttStaMax = pPreSect->m_pBx->GetStation();			
		if(pCurSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dEndSta = pCurSect->m_pBx->GetStation();			
		
		double	dStaSec = dEndSta;
		long	nPosSec	= -1;
		// 최대모멘트
		double dTermMaxValue	= GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, nG, dSttStaMax, dEndSta, 1);  // 지정된 구간에서 최대값.
		double dMaxRatio		= GetMaxStressRatio(nG, dSttStaMax, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);
		double dTermMax			= dTermMaxValue * dMaxRatio;
		
		// 최소모멘트
		double dTermMinValue	= GetElementForceTerm(ELE_MOMENT3, LC_SUM_MIN, nG, dSttStaMin, dEndSta, 2);  // 지정된 구간에서 최소값.
		double dMinRatio		= GetMaxStressRatio(nG, dSttStaMin, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);
		double dTermMin			= dTermMinValue * dMinRatio;

		pCurSect->m_dMaxMoment_Left  = dTermMax;
		pCurSect->m_dMinMoment_Left  = dTermMin;		
		//////////////////////////////////////////////////////////////////////////
		//  단면우측 (최소간격으로 인해 삭제된 단면을 고려해야 함.
		//////////////////////////////////////////////////////////////////////////
		dSttStaMax	= pCurSect->m_MatRight.m_pBx->GetStation();		
		dSttStaMin	= dSttStaMax;
		dEndSta		= pNextSect->m_MatLeft.m_pBx->GetStation();

		if(pCurSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dSttStaMax = pCurSect->m_pBx->GetStation();
		if(pNextSect->m_pBx->IsState(BX_STT_BENDING_UPPER | BX_END_BENDING_UPPER))		
			dEndSta = pNextSect->m_pBx->GetStation();
		
		// 최대모멘트
		dStaSec = dSttStaMax;
		nPosSec	= 1;
		dTermMaxValue	= GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, nG, dSttStaMax, dEndSta, 1);  // 지정된 구간에서 최대값.
		dMaxRatio		= GetMaxStressRatio(nG, dSttStaMax, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);
		dTermMax		= dTermMaxValue * dMaxRatio;

		// 최소모멘트
		dTermMinValue	= GetElementForceTerm(ELE_MOMENT3, LC_SUM_MIN, nG, dSttStaMin, dEndSta, 2);  // 지정된 구간에서 최소값.
		dMinRatio		= GetMaxStressRatio(nG, dSttStaMin, dMaxUp, dMaxLo, dCompUp, dCompLo, dTau, dStaSec, nPosSec);
		dTermMin		= dTermMinValue * dMinRatio;

		pCurSect->m_dMaxMoment_Right = dTermMax;
		pCurSect->m_dMinMoment_Right = dTermMin;
	}
}

// station이 지점인지 아닌지 판단
BOOL CFemStressCalc::IsJijumByStation(double dSta)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

	for(long nJijum=0; nJijum<pDB->m_nQtyJigan+1; nJijum++)
	{
		CPlateBasicIndex *pBx = pDB->GetGirder(-1)->GetBxOnJijum(nJijum);
		if(pBx->GetStation() == dSta)
			return TRUE;
	}
	return FALSE;
}

void CFemStressCalc::OutputOfMomentDiagram(CDomyun *pDom, long nG, double dScaleMoment, double Scale, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nML, BOOL bXL)
{	
	CPlateBridgeApp *pDB     = m_pDataManage->GetBridge(); 
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CARoadOptionStd *pOption = m_pDataManage->GetOptionStd();
	CCalcData		*pData	 = m_pDataManage->GetCalcData();
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);

	CPlateBxFinder Finder(pGir);
	if (!pBxStt)		pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	if (!pBxEnd)		pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

	double staStt    = pBxStt->GetStation();
	double staEnd    = pBxEnd->GetStation();
	double sta   = 0;
	double BaseY = 0.0;
	CString cs(""), cs2("");
	double YScale = dScaleMoment/5;
	double dPreValue = 0, dPreSta  = 0, dValue = 0;
	double staINode  = 0, staJNode = 0; 
	double dStressRatio = 0, dMax_up = 0, dMax_lo = 0, dComp_up = 0, dComp_lo = 0, dTau = 0;

	pData->m_ArrMomentZeroSta.RemoveAll();	
	pData->m_ArrMinZeroSta.RemoveAll();
	pData->m_ArrMaxZeroSta.RemoveAll();

	// 기준선 그림.
	CDomyun Dom(pDom);
	Dom.SetScaleDim(Scale);

	pOption->SetEnvType(&Dom, HCAD_TXTG);
	Dom.MoveTo(staStt, BaseY);
	Dom.LineTo(staEnd, BaseY);

	// 절점번호 그림.
	Dom.SetTextHeight(3.5);
	Dom.SetTextAlignHorz(TA_CENTER);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE, pBxStt, pBxEnd);

	int nNode = ModelCalc.GetJointNoByBx(pBx)+1;
	while (pBx)   
	{
		if(pBx->IsJoint())
		{
			sta = pBx->GetStation();
			Dom.MoveTo(sta, BaseY + 400);
			Dom.LineTo(sta, BaseY - 400);
			CDPoint P1(sta, BaseY + 500);
			cs.Format("%d", nNode++);
			Dom.TextOut (P1, cs);
			if (pBx->IsJijum())
				Dom.Triangle(sta, BaseY, 6);  // 지점 Symbol 그림.
		}
		pBx = Finder.GetBxNext();
	}

	// Max Moment 그림
	pOption->SetEnvType(&Dom, HCAD_PLTC);
	Dom.SetTextAlignHorz (TA_LEFT);
	
	BOOL bMaxSearch = FALSE;
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while (!pBx->IsJoint())
		pBx = Finder.GetBxNext();
	
	staINode  = pBx->GetStation();
	dPreValue = GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, staINode, 0);
	dPreSta = staINode;

	Dom.MoveTo(dPreSta, -dPreValue * YScale);

	BOOL bFirst = TRUE;
	while (pBx = Finder.GetBxNext())  
	{
		//if(!pBx->IsJoint())	continue;

		staJNode = pBx->GetStation();
		for (long i = 1; i < MAXMIN_POINT; i++)
		{
			sta = staINode + (staJNode - staINode) * i / (MAXMIN_POINT - 1);
			dValue = GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, sta, 0);
			if (dValue > 0.0)
			{
				if (dPreValue <= 0.0 || bFirst)
				{
					dPreSta = Bogan(dPreValue, dValue, dPreSta, sta, 0.0);
					Dom.MoveTo(dPreSta, 0.0);
					if (pGir->GetNumberJiganByStation(dPreSta) >= 1)
					{
						pData->m_ArrMomentZeroSta.Add((DWORD)dPreSta);
						pData->m_ArrMaxZeroSta.Add((DWORD)dPreSta);
					}
					bMaxSearch = TRUE;
					bFirst = FALSE;
				}
				if(bMaxSearch && dValue < dPreValue)
				{  // 첫 감소구간
					if(sta >= staStt && sta <= staEnd)
					{
						Dom.MoveTo(dPreSta, 0.0);
						Dom.LineTo(dPreSta, -dPreValue * YScale);
						CDPoint P1(dPreSta, -dPreValue * YScale * 0.4);
						cs.Format("Max(+) = %.1f kN.m", dPreValue);
						Dom.TextOut(P1, cs);

						if(pDB->IsTUGir())
						{
							dStressRatio = GetMaxStressRatio(nG, dPreSta, dMax_up, dMax_lo, dComp_up, dComp_lo, dTau);
							P1.y -= Dom.GetTextHeight()*1.5;
							cs.Format("저항모멘트 = %.3f kN.m", dStressRatio * dPreValue);
							Dom.TextOut(P1.x, P1.y, cs);
						}
					}
					bMaxSearch = FALSE;
				}
				if (sta <= staStt)
					Dom.MoveTo(sta, -dValue * YScale);
				if (sta > staStt && sta <= staEnd)
					Dom.LineTo(sta, -dValue * YScale);
			}
			else if (dPreValue > 0.0)
			{
				dPreSta = Bogan(dPreValue, dValue, dPreSta, sta, 0.0);
				if (sta > staStt && sta <= staEnd)
					Dom.LineTo(dPreSta, 0.0);
				if (pGir->GetNumberJiganByStation(sta) < pDB->m_nQtyJigan - 1)
				{
					pData->m_ArrMomentZeroSta.Add((DWORD)dPreSta);
					pData->m_ArrMaxZeroSta.Add((DWORD)dPreSta);
				}
			}
			dPreValue = dValue;
			dPreSta  = sta;
		}
		staINode = staJNode;
	}
	
	// Min Moment 그림
	bFirst = TRUE;
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while (!pBx->IsJoint())
		pBx = Finder.GetBxNext();

	staINode = pBx->GetStation(), staJNode;
	dPreValue = GetElementForce(ELE_MOMENT3, LC_SUM_MIN, nG, staINode, 0);
	dPreSta = staINode;
	while (pBx = Finder.GetBxNext())
	{
		if(!pBx->IsJoint())	continue;

		staJNode = pBx->GetStation();
		for(long i = 1; i < MAXMIN_POINT; i++)
		{
			sta = staINode + (staJNode - staINode) * i / (MAXMIN_POINT - 1);
			dValue = GetElementForce(ELE_MOMENT3, LC_SUM_MIN, nG, sta, 0);
			long nPier = pGir->GetNumberJiganByStation(dPreSta);
			if ((nPier == 0 && dValue > dPreValue) || (nPier == pDB->m_nQtyJigan - 1 && dValue < dPreValue))
			{
				dPreValue = dValue;
				dPreSta   = sta;
				continue;   // 거더의 첫 부분과 끝부분에서 발생하는 - Moment는 처리안함.
			}
			if(dValue < 0.0)
			{
				if(dPreValue >= 0.0 || bFirst)
				{
					dPreSta = Bogan(dPreValue, dValue, dPreSta, sta, 0.0);
					Dom.MoveTo(dPreSta, 0.0);
					pData->m_ArrMomentZeroSta.Add((DWORD)dPreSta);
					pData->m_ArrMinZeroSta.Add((DWORD)dPreSta);
					bFirst =FALSE;
				}
				if (sta <= staStt)
					Dom.MoveTo(sta, -dValue * YScale);
				if (sta > staStt && sta <= staEnd)
					Dom.LineTo(sta, -dValue * YScale);
				if(i == MAXMIN_POINT-1 && pBx->IsJijum())
				{
					if(sta >= staStt && sta <= staEnd)
					{
						Dom.MoveTo(sta, 0.0);
						Dom.LineTo(sta, -dValue * YScale);
						CDPoint P1(sta, -dValue * YScale * 0.4);
						cs.Format("Max(-) = %.1f kN.m", dValue);
						Dom.TextOut(P1, cs);

						if(pDB->IsTUGir())
						{
							dStressRatio = GetMaxStressRatio(nG, sta, dMax_up, dMax_lo, dComp_up, dComp_lo, dTau);
							P1.y -= Dom.GetTextHeight()*1.5;
							cs.Format("저항모멘트 = %.3f kN.m", dStressRatio * dValue);
							Dom.TextOut(P1.x, P1.y, cs);
						}
					}
				}
			}
			else if(dPreValue < 0.0)
			{
				dPreSta = Bogan(dPreValue, dValue, dPreSta, sta, 0.0);
				if (sta > staStt && sta <= staEnd)
					Dom.LineTo(dPreSta, 0.0);
				pData->m_ArrMomentZeroSta.Add((DWORD)dPreSta);
				pData->m_ArrMinZeroSta.Add((DWORD)dPreSta);
			}
			dPreValue = dValue;
			dPreSta   = sta;
		}
		staINode = staJNode;
	}

	CPlateGirderApp* pSGir = pDB->GetGirder(0);
	double staJi = pSGir->GetStationOnJijum(1);
	double dSMin = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MIN, 0, staJi, staJi, 0);  // 합계중 최소값.
	double dBound = fabs(dSMin/8);
	
	///////////////////////////////////////////
	// 단면 저항 Moment (Max) 그림
	pOption->SetEnvType(&Dom, HCAD_PAPR);
	Dom.SetTextAlignHorz(TA_CENTER);

	int nSize = (long)m_pCalcStd->m_vByunDanInfo.size() > nG ? (int)m_pCalcStd->m_vByunDanInfo[nG].size() : 0;
	for(int i=0; i<nSize-1; i++)
	{
		CDanMyunProp SectI = m_pCalcStd->m_vByunDanInfo[nG][i];
		CDanMyunProp SectJ = m_pCalcStd->m_vByunDanInfo[nG][i+1];
				
		double SectIMaxM_L = SectI.m_dMaxMoment_Left * YScale;
		double SectIMaxM_R = SectI.m_dMaxMoment_Right * YScale;
		double SectJMaxM_L = SectJ.m_dMaxMoment_Left * YScale;
		double SectJMaxM_R = SectJ.m_dMaxMoment_Right * YScale;
		
		double SectIMinM_L = SectI.m_dMinMoment_Left * YScale;
		double SectIMinM_R = SectI.m_dMinMoment_Right * YScale;
		double SectJMinM_L = SectJ.m_dMinMoment_Left * YScale;
		double SectJMinM_R = SectJ.m_dMinMoment_Right * YScale;
		
		double SectISta = SectI.m_pBx->GetStation();
		double SectJSta = SectJ.m_pBx->GetStation();

		// 교각 위치에서 모멘트도 그리기
		BOOL bIPier = SectI.m_pBx->IsPier() && (SectI.m_pBx->GetNumberJijum()>0&&SectI.m_pBx->GetNumberJijum()<pDB->m_nQtyJigan);
		BOOL bJPier = SectJ.m_pBx->IsPier() && (SectJ.m_pBx->GetNumberJijum()>0&&SectJ.m_pBx->GetNumberJijum()<pDB->m_nQtyJigan);
		if(bIPier || bJPier)
		{
			if(bIPier)
			{
				CPlateBasicIndex *pBxL = pGir->GetBxByStationDir(SectISta, -1, BX_END_BYUN_LOWER);
				CPlateBasicIndex *pBxR = pGir->GetBxByStationDir(SectISta,  1, BX_STT_BYUN_LOWER);
				double PierL = pBxL && fabs(pBxL->GetStation()-SectISta) < 10000 ? pBxL->GetStation() : 0.;
				double PierR = pBxR && fabs(pBxR->GetStation()-SectISta) < 10000 ? pBxR->GetStation() : 0.;
				double dLen = PierR - PierL; 
				
				// 교차점				
				Dom.MoveTo(SectISta, -SectIMinM_L);
				if(PierL!=0 && PierR!=0 && pBxR)
					Dom.LineTo(SectISta + dLen/2, -SectIMinM_L);
				else
					Dom.LineTo(SectJSta, -SectIMinM_L);
				Dom.LineTo(SectJSta, -SectJMinM_L);				
							
				cs.Format("%d", SectI.m_MatLeft.m_nIdx+1);
				Dom.TextCircle(SectISta, -SectIMinM_L + 6*Scale, cs, -1);
			}
			else
			{
				CPlateBasicIndex *pBxL = pGir->GetBxByStationDir(SectJSta, -1, BX_END_BYUN_LOWER);
				CPlateBasicIndex *pBxR = pGir->GetBxByStationDir(SectJSta,  1, BX_STT_BYUN_LOWER);
				// 2007. 5. 18. 변단면과 아닌 구간이 혼용될때 생기는 오류 방지를 위하여 고침 (fabs(pBxL->GetStation()-SectISta) < 10000) 추가
				double PierL = pBxL && fabs(pBxL->GetStation()-SectJSta) < 10000 ? pBxL->GetStation() : 0.;
				double PierR = pBxR && fabs(pBxR->GetStation()-SectJSta) < 10000 ? pBxR->GetStation() : 0.;
				//double PierL = pBxL ? pBxL->GetStation() : 0.;
				//double PierR = pBxR ? pBxR->GetStation() : 0.;
				double dLen = PierR - PierL; 
				
				Dom.MoveTo(SectISta, -SectIMinM_L);
				if(PierL!=0 && PierR!=0 && pBxR)
					Dom.LineTo(SectJSta - dLen/2, -SectJMinM_L);
				else
					Dom.LineTo(SectISta, -SectJMinM_L);
				Dom.LineTo(SectJSta, -SectJMinM_L);
				
				cs.Format("%d", SectJ.m_MatLeft.m_nIdx+1);
				Dom.TextCircle(SectJSta, -SectJMinM_L + 6*Scale, cs, -1);
			}
		}
		else
		{

			// MaxMoment
			double dStaSecI  = SectI.m_pBx->GetStation();
			double dValSecI  = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, nG, dStaSecI, dStaSecI, 0);				
			double dStaSecJ  = SectJ.m_pBx->GetStation();
			double dValSecJ  = GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, nG, dStaSecJ, dStaSecJ, 0);		

			if(dValSecI > 0 || dValSecJ > 0)
			{			
				if( (SectIMaxM_L != SectIMaxM_R) && (SectIMaxM_L > 0) && (SectIMaxM_R > 0) )
				{
					Dom.MoveTo(SectISta, min(-SectIMaxM_R,-dBound * YScale));
					Dom.LineTo(SectISta, min(-SectIMaxM_L,-dBound * YScale));									
				}
				if( (SectJMaxM_L != SectJMaxM_R) && (SectJMaxM_L > 0) && (SectJMaxM_R > 0) )
				{
					Dom.MoveTo(SectJSta, min(-SectJMaxM_R,-dBound * YScale));
					Dom.LineTo(SectJSta, min(-SectJMaxM_L,-dBound * YScale));
				}
				// 단면 번호
				if(!SectI.IsByunDanMyun() && ! SectJ.IsByunDanMyun())
				{
					if(SectI.m_MatRight.m_dI33 < SectJ.m_MatLeft.m_dI33)
						cs.Format("%d", SectI.m_MatRight.m_nIdx+1);
					else
						cs.Format("%d", SectJ.m_MatLeft.m_nIdx+1);
					if(SectIMaxM_R>0)
						Dom.TextCircle((SectISta + SectJSta)/2, min(-SectIMaxM_R,-dBound * YScale) - 6*Scale, cs, -1);
				}	
				if(SectI.IsByunDanMyun() && SectIMaxM_L>0)
				{
					cs.Format("%d", SectI.m_MatRight.m_nIdx+1);
					Dom.TextCircle(SectISta, min(-SectIMaxM_L,-dBound * YScale) - 6*Scale, cs, -1);
				}
				if(SectJ.IsByunDanMyun() && SectJMaxM_L>0)
				{
					cs.Format("%d", SectJ.m_MatRight.m_nIdx+1);
					Dom.TextCircle(SectJSta, min(-SectJMaxM_L,-dBound * YScale) - 6*Scale, cs, -1);
				}
				if(SectIMaxM_R<0 && SectJMaxM_L>0)
				{
					CDPoint xyResult;
					Dom.MoveTo(SectISta, -dBound * YScale);
					Dom.LineTo(SectJSta, min(-SectJMaxM_L,-dBound * YScale));					

					cs.Format("%d", SectI.m_MatLeft.m_nIdx+1);
					Dom.TextCircle(SectISta, -dBound * YScale - 6*Scale, cs, -1);					
				}
				else if(SectJMaxM_L<0 && SectIMaxM_R>0)
				{
					CDPoint xyResult;
					Dom.MoveTo(SectISta, min(-SectIMaxM_R,-dBound * YScale));					
					Dom.LineTo(SectJSta, -dBound * YScale);	
					
					cs.Format("%d", SectJ.m_MatLeft.m_nIdx+1);
					Dom.TextCircle(SectJSta, -dBound * YScale - 6*Scale, cs, -1);					
				}				
				else
				{				
					Dom.MoveTo(SectISta, min(-SectIMaxM_R,-dBound * YScale));
					Dom.LineTo(SectJSta, min(-SectJMaxM_L,-dBound * YScale));					
				}
			}

			// MinMoment
			long nPierI = pGir->GetNumberJijumByBx(SectI.m_pBx);
			long nPierJ = pGir->GetNumberJijumByBx(SectJ.m_pBx);
			if ( (SectI.m_pBx->IsJijum() && (nPierI == 0 && dValSecJ > dValSecI)) || 
				 (SectJ.m_pBx->IsJijum() && (nPierJ == pDB->m_nQtyJigan && dValSecJ < dValSecI)))
			{
				continue;   // 거더의 첫 부분과 끝부분에서 발생하는 - Moment는 처리안함.
			}
			if(SectIMinM_R < 0 || SectJMinM_L < 0)
			{				
				if( (SectIMinM_L != SectIMinM_R) && (SectIMinM_L < 0) && (SectIMinM_R < 0) )
				{
					Dom.MoveTo(SectISta, max(-SectIMinM_R, dBound * YScale));
					Dom.LineTo(SectISta, max(-SectIMinM_L, dBound * YScale));
				}
				if( (SectJMinM_L != SectJMinM_R) && (SectJMinM_L < 0) && (SectJMinM_R < 0) )
				{
					Dom.MoveTo(SectJSta, max(-SectJMinM_R, dBound * YScale));
					Dom.LineTo(SectJSta, max(-SectJMinM_L, dBound * YScale));
				}
				if(SectIMinM_R>0 && SectJMinM_L<0)
				{
					CDPoint xyResult;
					Dom.MoveTo(SectISta, dBound * YScale);
					Dom.LineTo(SectJSta, max(-SectJMinM_L, dBound * YScale));					
				}
				else if(SectIMinM_R<0 && SectJMinM_L>0)
				{
					CDPoint xyResult;
					GetXyMatchSegAndLine(CDPoint(SectISta, -SectIMinM_R), CDPoint(SectJSta, -SectJMinM_L), CDPoint(0,0), CDPoint(1,0), xyResult);
					Dom.MoveTo(SectISta, max(-SectIMinM_R, dBound * YScale));					
					Dom.LineTo(SectJSta, dBound * YScale);					
				}
				else
				{				
					Dom.MoveTo(SectISta, max(-SectIMinM_R, dBound * YScale));
					Dom.LineTo(SectJSta, max(-SectJMinM_L, dBound * YScale));
				}

				// 단면 번호
				if(!SectI.IsByunDanMyun() && ! SectJ.IsByunDanMyun())
				{
					if(SectI.m_MatRight.m_dI33 < SectJ.m_MatLeft.m_dI33)
						cs.Format("%d", SectI.m_MatRight.m_nIdx+1);
					else
						cs.Format("%d", SectJ.m_MatLeft.m_nIdx+1);		
					if(SectIMinM_R<0)
						Dom.TextCircle((SectISta + SectJSta)/2, max(-SectIMinM_R, dBound * YScale) + 6*Scale, cs, -1);
				}
				if(SectI.IsByunDanMyun() && SectIMinM_L<0)
				{
					cs.Format("%d", SectI.m_MatRight.m_nIdx+1);
					Dom.TextCircle(SectISta, max(-SectIMinM_L, dBound * YScale) + 6*Scale, cs, -1);					
				}
				if(SectJ.IsByunDanMyun() && SectJMinM_L<0)
				{
					cs.Format("%d", SectJ.m_MatRight.m_nIdx+1);
					Dom.TextCircle(SectJSta, max(-SectJMinM_L, dBound * YScale) + 6*Scale, cs, -1);					
				}
			}						
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// Grid Line 그림.
	// Dom.SetLineColor(19);
	double MaxM = 0;
	double MinM = 0;

	pOption->SetEnvType(&Dom, HCAD_CENT);
	Dom.SetTextHeight(3.5);
	Dom.SetTextAlignHorz(TA_LEFT);	

	pOption->SetEnvType(&Dom, HCAD_CENT);
	//Dom.SetTextHeight(3.5);..KB.
	Dom.SetTextAlignHorz(TA_LEFT);
	nG = -1;
	sta = -9999999999999.0;
	MaxM = max(MaxM, GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, sta, 1));
	nG = -1;
	sta = -9999999999999.0;
	MinM = min(MinM, GetElementForce(ELE_MOMENT3, LC_SUM_MIN, nG, sta, 2));
	double nInc = 5000;
	for (long nGrid = (long) (MinM / nInc) - 1; nGrid < MaxM / nInc + 1; nGrid++)   
	{
		if (nGrid == 0)
			continue;
		if(!bXL)
		{
			CDPoint P1(staStt - 30 * Scale, -nGrid * nInc * YScale + 0.7 * Scale);
			cs.Format("%g kN.m", nInc * nGrid);
			Dom.TextOut(P1, cs);

			Dom.MoveTo(staStt - 30 * Scale, -nGrid * nInc * YScale);
			Dom.LineTo(staEnd, -nGrid * nInc * YScale);
		}
	}
	Dom.Move (-staStt, 0);
	
	*pDom << Dom;		
}

void CFemStressCalc::OutputOfSectionSummary(CDomyun *pDom, long nG, double GFactor, double scaleH, double scaleV, CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nML)
{	
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);
	CARoadOptionStd	*pOptStd	= m_pDataManage->GetOptionStd();
	CCalcData		*pData		= m_pDataManage->GetCalcData();
	CGlobarOption	*pGlopt		= m_pDataManage->GetGlobalOption();

	CDomyun Dom(pDom);
	Dom.SetScaleDim(scaleV);

	CPlateBxFinder Finder(pGir);
	if (!pBxStt)		pBxStt = Finder.GetBxFirst(BX_STT_GIRDER);
	if (!pBxEnd)		pBxEnd = Finder.GetBxLast(BX_END_GIRDER);

	double BaseY  = 0.0;
	double dHeight= 0.0;
	double sta    = 0.0;
	double staStt = pBxStt->GetStation();
	double staEnd = pBxEnd->GetStation();
	double staGirStt = pGir->GetSSS(OFF_STT_GIRDER);
	double staGirEnd = pGir->GetSSS(OFF_END_GIRDER);

	CString cs(""), cs2("");

	double YHeight = GFactor;   // 거더 높이 확대비율
	double YFlange = scaleH;   // 플랜지 두께 확대비율	

// 단면 변화 구간
#ifdef _DEBUG
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextHeight(0.5);
	int nSize = (long)m_pCalcStd->m_vByunDanInfo.size() > nG ? (int)m_pCalcStd->m_vByunDanInfo[nG].size() : 0;

	for(long i=0; i<nSize; i++)
	{
		double dSta = m_pCalcStd->m_vByunDanInfo[nG][i].m_pBx->GetStation();
		long nMatId1 = m_pCalcStd->m_vByunDanInfo[nG][i].m_MatLeft.m_nIdx;
		long nMatId2 = m_pCalcStd->m_vByunDanInfo[nG][i].m_MatRight.m_nIdx;
		int nByun = m_pCalcStd->m_vByunDanInfo[nG][i].m_nByunType;

		double dTextHeight = Dom.GetTextHeight(FALSE);

		Dom.LineTo(dSta, BaseY + 24 * scaleV, dSta, BaseY - 80 * scaleV);

		cs.Format("%.0f", dSta);
		if(i%2==0)	Dom.TextOut(dSta, BaseY - (80+dTextHeight/2) * scaleV, cs);
		else		Dom.TextOut(dSta, BaseY - (80-dTextHeight/2) * scaleV, cs);
		cs.Format("%2d, %2d", nMatId1+1, nMatId2+1);
		Dom.TextOut(dSta, BaseY - (80+dTextHeight+1) * scaleV, cs);

		cs.Format("%d", nByun);
		Dom.TextOut(dSta, BaseY - (80+2*dTextHeight+1) * scaleV,cs);
	}
#endif

	// 교번 구간 표시
	Dom.SetTextAlignHorz(TA_CENTER);
	pOptStd->SetEnvType(&Dom, HCAD_CENT);

	InsertionSort(pData->m_ArrMomentZeroSta);
	for (long nIdx = 0; nIdx < pData->m_ArrMomentZeroSta.GetSize() - 1; nIdx += 2)   
	{
		pOptStd->SetEnvType(&Dom, HCAD_CENT);
		if (pData->m_ArrMomentZeroSta[nIdx] >= staStt && pData->m_ArrMomentZeroSta[nIdx] <= staEnd)
		{
			Dom.MoveTo(pData->m_ArrMomentZeroSta[nIdx], BaseY + 12 * scaleV);
			Dom.LineTo(pData->m_ArrMomentZeroSta[nIdx], BaseY - (pGir->GetHeightMaxByJijum() * YHeight + 12 * scaleV));
		}
		if (pData->m_ArrMomentZeroSta[nIdx + 1] >= staStt && pData->m_ArrMomentZeroSta[nIdx + 1] <= staEnd)
		{
			Dom.MoveTo(pData->m_ArrMomentZeroSta[nIdx + 1], BaseY + 12 * scaleV);
			Dom.LineTo(pData->m_ArrMomentZeroSta[nIdx + 1], BaseY - (pGir->GetHeightMaxByJijum() * YHeight + 12 * scaleV));
		}
		double dSttTerm = max(staStt, pData->m_ArrMomentZeroSta[nIdx]);
		double dEndTerm = min(staEnd, pData->m_ArrMomentZeroSta[nIdx + 1]);
		if (dSttTerm < dEndTerm)
		{
			CDPoint P1((dSttTerm + dEndTerm) * 0.5, BaseY + 14 * scaleV);
			Dom.TextOut (P1, "- 교번 구간 -");
		}
	}

	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	// 상판 밑면 그림.
	double dThick=0, dPreThick=0;
	long nBaseUpper = pGlopt->GetDesignHeigtBaseUpper();
	long nBaseLower = pGlopt->GetDesignHeigtBaseLower();
	CPlateBasicIndex *pBx = NULL;//Finder.GetBxFirst(BX_UPPER, pBxStt, pBxEnd, FALSE, FALSE);
	double	dDirUpper	= nBaseUpper ? -1 : 1;
	double	dDirLower	= nBaseLower ? -1 : 1;

	// 상판
	dHeight = pDB->IsTUGir() ? 0 : pGir->GetHeightTaper(staStt);
	dThick = pBxStt->GetFactChain(G_F_U)->m_dFactChainThick;
	dPreThick = dThick;
	Dom.MoveTo(staStt, BaseY - dHeight*YHeight);
	pBx = Finder.GetBxFirst(BX_UPPER, pBxStt, pBxEnd, TRUE, TRUE);

	while(pBx)
	{
		sta = pBx->GetStation();
		dHeight	= pDB->IsTUGir() ? 0 : pGir->GetHeightTaper(sta);
		Dom.LineTo(sta, BaseY - dHeight*YHeight);
		pBx = Finder.GetBxNext();
	}
	// 상판
	Dom.MoveTo(staStt, BaseY - dHeight*YHeight - dThick*YFlange*dDirUpper);
	pBx = Finder.GetBxFirst(BX_UPPER, pBxStt, pBxEnd, TRUE, TRUE);
	while(pBx)
	{
		sta = pBx->GetStation();
		dHeight	= pDB->IsTUGir() ? 0 : pGir->GetHeightTaper(sta);
		dThick = pBx->GetFactChain(G_F_U)->m_dFactChainThick;
		if(pBx->IsState(BX_UPPERFLANGE))
		{
			if(dThick!=dPreThick)
			{
				Dom.LineTo(sta, BaseY - dHeight*YHeight - dPreThick*YFlange*dDirUpper);
				Dom.LineTo(sta, BaseY - dHeight*YHeight);
				Dom.LineTo(sta, BaseY - dHeight*YHeight - dThick* YFlange*dDirUpper);
			}
			dPreThick = dThick;
		}
		else
		{
			Dom.LineTo(sta, BaseY - dHeight*YHeight - dThick*YFlange*dDirUpper);
		}
		pBx = Finder.GetBxNext();
	}

	// 하판
	dHeight	= pGir->GetHeightFlangeLower(staStt);
	dThick	= pBxStt->GetFactChain(G_F_L)->m_dFactChainThick;
	dPreThick = dThick;
	Dom.MoveTo(staStt, BaseY - dHeight*YHeight);
	pBx = Finder.GetBxFirst(BX_UPPER, pBxStt, pBxEnd, TRUE, TRUE);
	while(pBx)
	{
		sta = pBx->GetStation();
		dHeight	=  pGir->GetHeightFlangeLower(sta);
		Dom.LineTo(sta, BaseY - dHeight*YHeight);
		pBx = Finder.GetBxNext();
	}
	// 하판
	Dom.MoveTo(staStt, BaseY - dHeight*YHeight - dThick*YFlange*dDirLower);
	pBx = Finder.GetBxFirst(BX_LOWER, pBxStt, pBxEnd, TRUE, TRUE);
	while(pBx)
	{
		sta = pBx->GetStation();
		dHeight	= pGir->GetHeightFlangeLower(sta);
		dThick = pBx->GetFactChain(G_F_L)->m_dFactChainThick;
		if(pBx->IsState(BX_LOWERFLANGE))
		{
			if(dThick!=dPreThick)
			{
				Dom.LineTo(sta, BaseY - dHeight*YHeight - dPreThick*YFlange*dDirLower);
				Dom.LineTo(sta, BaseY - dHeight*YHeight);
				Dom.LineTo(sta, BaseY - dHeight*YHeight - dThick* YFlange*dDirLower);
			}
			dPreThick = dThick;
		}
		else
		{
			Dom.LineTo(sta, BaseY - dHeight*YHeight - dThick*YFlange*dDirLower);
		}
		pBx = Finder.GetBxNext();
	}
	// 거더 양끝선 그림.
	if (pBxStt->IsState(BX_STT_GIRDER))
	{
		dHeight = pDB->IsTUGir() ? 0 : pGir->GetHeightTaper(staStt);
		dThick	= pBxStt->GetFactChain(G_F_U)->m_dFactChainThick;
		if(nBaseUpper==0)	Dom.MoveTo(staStt, BaseY - dHeight * YHeight);
		else				Dom.MoveTo(staStt, BaseY - dHeight * YHeight - dThick * YFlange * dDirUpper);
		dHeight = pGir->GetHeightFlangeLower(staStt);
		dThick	= pBxStt->GetFactChain(G_F_L)->m_dFactChainThick;
		if(nBaseLower==0)	Dom.LineTo(staStt, BaseY - dHeight * YHeight - dThick * YFlange * dDirLower);
		else				Dom.LineTo(staStt, BaseY - dHeight * YHeight);
	}
	if (pBxEnd->IsState(BX_END_GIRDER))
	{
		dHeight = pDB->IsTUGir() ? 0 : pGir->GetHeightTaper(staEnd);
		dThick	= pBxEnd->GetFactChain(G_F_U)->m_dFactChainThick;
		if(nBaseUpper==0)	Dom.MoveTo(staEnd, BaseY - dHeight * YHeight);
		else				Dom.MoveTo(staEnd, BaseY - dHeight * YHeight - dThick * YFlange * dDirUpper);
		dHeight = pGir->GetHeightFlangeLower(staEnd);
		dThick	= pBxEnd->GetFactChain(G_F_L)->m_dFactChainThick;
		if(nBaseLower==0)	Dom.LineTo(staEnd, BaseY - dHeight * YHeight - dThick * YFlange * dDirLower);
		else				Dom.LineTo(staEnd, BaseY - dHeight * YHeight);
	}

	// 지점 Symbol 그림.
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	long nJijum=0;
	while(pBx)
	{
		if (pBx->IsJijum())
		{
			cs	= pDB->m_strJijumName[nJijum];
			sta = pBx->GetStation();
			dHeight = pGir->GetHeightFlangeLower(sta);
			dThick  = pBx->GetFactChain(G_F_L)->m_dFactChainThick;
			
			if(nBaseLower==0)	Dom.Triangle(sta, BaseY - dHeight * YHeight - dThick * YFlange * dDirLower, 4);
			else				Dom.Triangle(sta, BaseY - dHeight * YHeight, 4);
			Dom.TextCircle(sta, BaseY - dHeight * YHeight - dThick * YFlange * dDirLower - 10 * scaleV, cs, -1);
			nJijum++;
		}
		pBx = Finder.GetBxNext();
	}

	// 복부판 공장이음 그림.
	pOptStd->SetEnvType(&Dom, HCAD_PLTC);
	dPreThick = dThick = pGir->GetThickJewon(G_W, pBxStt);
	pBx = Finder.GetBxFirst(BX_WEB, pBxStt, pBxEnd);
	while(pBx)   
	{
		sta = pBx->GetStation();
		dThick = pGir->GetThickJewon(G_W, pBx);
		if (dThick != dPreThick)
		{  // 두께가 같으면 수행하지 않음.
			dHeight = pGir->GetHeightTaper(sta);
			Dom.MoveTo(sta, BaseY - dHeight * YHeight);
			dHeight = pGir->GetHeightFlangeLower(sta);
			Dom.LineTo(sta, BaseY - dHeight * YHeight);
		}
		dPreThick = dThick;
		pBx = Finder.GetBxNext();
	}

	// 현장이음 그림.
	double dWidth1 = 800;
	double dWidth2 = 500;
	double dHeight1 = 500;
	double dGap = 100;
	
	pBx = Finder.GetBxFirst(BX_SPLICE, pBxStt, pBxEnd);
	while(pBx)
	{
		CPlateSplice *pSp = pBx->GetSplice();
		sta = pBx->GetStation();
		// 중심선
		dThick	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
		dHeight = pGir->GetHeightTaper(sta);
		if(nBaseUpper==0)	Dom.MoveTo(sta, BaseY - dHeight * YHeight);
		else				Dom.MoveTo(sta, BaseY - dThick * YFlange * dDirUpper);

		dHeight = pGir->GetHeightFlangeLower(sta);
		dThick	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
		if(pSp->m_bSpliceType)	pOptStd->SetEnvType(&Dom, HCAD_PLTC);
		else					Dom.SetLineColor(2);
		if(nBaseLower==0)	Dom.LineTo(sta, BaseY -  dHeight * YHeight - dThick * YFlange * dDirLower);
		else				Dom.LineTo(sta, BaseY -  dHeight * YHeight);
		pOptStd->SetEnvType(&Dom, HCAD_PLTC);
		// 이음판
		CDRect rect;
		if(pSp->m_bSpliceType)
		{
			if(pSp->m_mT>0)
			{
				if(nBaseUpper==0)	rect.top = BaseY - dThick * YFlange * dDirUpper - dGap;
				else				rect.top = BaseY - dGap;
				rect.bottom = rect.top - dHeight1;
				rect.left   = sta - dWidth1 / 2;
				rect.right  = sta + dWidth1 / 2;
				Dom.Rectangle(rect);   // 상부 모멘트 이음판
				if(nBaseLower==0)	rect.bottom = BaseY - dHeight * YHeight + dGap;
				else				rect.bottom = BaseY - dHeight * YHeight - dThick * YFlange * dDirLower + dGap;
				rect.top    = rect.bottom + dHeight1;
				Dom.Rectangle(rect);   // 하부 모멘트 이음판
				rect.bottom = rect.top;
				if(nBaseUpper==0)	rect.top = BaseY - dThick * YFlange * dDirUpper - dGap - dHeight1;
				else				rect.top = BaseY - dGap - dHeight1;
				rect.left   = sta - dWidth2 / 2;
				rect.right  = sta + dWidth2 / 2;
				Dom.Rectangle(rect);   // 전단 이음판
			}
			else
			{
				if(nBaseUpper==0)	rect.top = BaseY - dThick * YFlange * dDirUpper - dGap;
				else				rect.top = BaseY - dGap;
				if(nBaseLower==0)	rect.bottom = BaseY - dHeight * YHeight + dGap;
				else				rect.bottom = BaseY - dHeight * YHeight - dThick * YFlange * dDirLower + dGap;
				rect.left   = sta - dWidth2 / 2;
				rect.right  = sta + dWidth2 / 2;
				Dom.Rectangle(rect);   // 전단 이음판
			}
		}
		cs = pBx->GetSplice()->GetMarkstring();
		Dom.RoundText(sta, BaseY + 900 + 2 * scaleV, cs);   // 현장이음 Label.
		pBx = Finder.GetBxNext();
	}

	// 수평보강재 그림.
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	pBx = Finder.GetBxFirst(BX_PLATEHSTIFF | BX_END_GIRDER, pBxStt, pBxEnd);
	while(pBx)
	{
		sta = pBx->GetStation();

		CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();

		if(!pBxNext)		break;

		double		staNext = pBxNext->GetStation();
		CWebHStiff	*pHs	= pBx->GetWebHStiff();

		if(!pHs) break;

		BOOL bLeft = FALSE;

		if(pHs->m_nPos==WEB_SIDE_BOTH || pHs->m_nPos==WEB_SIDE_RIGHT)	//양측, 우측
			bLeft = FALSE;
		else
			bLeft = TRUE;
	
		for(long nDan = 0; nDan < WEBHSTIFFSUMAX; nDan++)
		{
			if (pHs->IsEmpty(nDan, bLeft))		continue;
			if (pBx->IsState(BX_SPLICE)) 		continue;
			if (pBxNext->IsState(BX_SPLICE))	continue;

			dHeight	= pHs->GetHeightHStiffOnStt(nDan, bLeft, TRUE) + pGir->GetHeightTaper(sta);

			double dHeightNext = pHs->GetHeightHStiffOnStt(nDan, bLeft, FALSE) + pGir->GetHeightTaper(staNext);

			dThick	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;

			Dom.SetLineColor(2);
			Dom.MoveTo(sta, BaseY - dHeight * YHeight/* *  dDirUpper*/);
			Dom.LineTo(staNext, BaseY - dHeightNext * YHeight/* * dDirUpper*/);
		}
		pBx = Finder.GetBxNext();
	}

	Dom.SetDirection("TOP");
	pOptStd->SetEnvType(&Dom, HCAD_DIML);
	CDDWordArray DimSta, LenSta, ArrThick;

	// 거더길이 치수선 그림.
	DimSta.Add(staStt);
	LenSta.Add(staGirStt);
	DimSta.Add(staEnd);
	LenSta.Add(staGirEnd);
	OutputOfDimSectionSummary(&Dom, BaseY, 4, "거 더  길 이", DimSta, LenSta, ArrThick);
	DimSta.RemoveAll();
	LenSta.RemoveAll();
	ArrThick.RemoveAll();

	// 지간구성 치수선 그림.
	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);
	double dPreSta = staGirStt;
	while(pBx)
	{
		if (!pBx->IsState(BX_CROSSBEAM_VBRACING) || pBx->IsJijum())
		{
			sta = pBx->GetStation();
			if (staStt < sta && DimSta.GetSize() == 0)
			{  // 치수선 첫부분이 중간에 걸림
				DimSta.Add(staStt);
				LenSta.Add(dPreSta);
			}
			if (staStt <= sta && sta <= staEnd)
			{
				DimSta.Add(sta);
				LenSta.Add(sta);
			}
			if (sta > staEnd)
			{  // 치수선 마지막이 중간에 걸림
				DimSta.Add(staEnd);
				LenSta.Add(sta);
			}
			if (sta >= staEnd)
				break;
			dPreSta = sta;
		}
		pBx = Finder.GetBxNext();
	}
	OutputOfDimSectionSummary(&Dom, BaseY, 3, "경 간  구 성", DimSta, LenSta, ArrThick);
	DimSta.RemoveAll();
	LenSta.RemoveAll();
	ArrThick.RemoveAll();

	// 상판 공장이음 치수선 그림.
	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE, pBxStt, pBxEnd);
	dPreSta = staGirStt;
	dPreThick = 0;
	while(pBx)
	{
		dThick	= pBx->GetFactChain(G_F_U)->m_dFactChainThick * (pGlopt->GetDesignHeigtBaseUpper()==0 ? 1 : 1);
		if (!pBx->IsState(BX_UPPERFLANGE) || dThick != dPreThick)
		{
			sta = pBx->GetStation();
			if (staStt < sta && DimSta.GetSize() == 0)
			{  // 치수선 첫부분이 중간에 걸림
				DimSta.Add(staStt);
				LenSta.Add(dPreSta);
			}
			if (staStt <= sta && sta <= staEnd)
			{
				DimSta.Add(sta);
				LenSta.Add(sta);
			}
			if (sta > staEnd)
			{  // 치수선 마지막이 중간에 걸림
				DimSta.Add(staEnd);
				LenSta.Add(sta);
			}
			ArrThick.Add(dThick);

			if (sta >= staEnd)
				break;
			dPreSta = sta;
			dPreThick = dThick;
		}
		pBx = Finder.GetBxNext();
	}
	OutputOfDimSectionSummary(&Dom, BaseY, 2, "상  판(두께)", DimSta, LenSta, ArrThick);
	DimSta.RemoveAll();
	LenSta.RemoveAll();
	ArrThick.RemoveAll();

	// 현장이음 치수선 그림.
	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_SPLICE, pBxStt, pBxEnd);
	dPreSta = staGirStt;
	while(pBx)
	{
		sta = pBx->GetStation();
		if (staStt < sta && DimSta.GetSize() == 0)
		{  // 치수선 첫부분이 중간에 걸림
			DimSta.Add(staStt);
			LenSta.Add(dPreSta);
		}
		if (staStt <= sta && sta <= staEnd)
		{
			DimSta.Add(sta);
			LenSta.Add(sta);
		}
		if (sta > staEnd)
		{  // 치수선 마지막이 중간에 걸림
			DimSta.Add(staEnd);
			LenSta.Add(sta);
		}
		if (sta >= staEnd)
			break;
		dPreSta = sta;
		pBx = Finder.GetBxNext();
	}
	OutputOfDimSectionSummary(&Dom, BaseY, 1, "현 장  이 음", DimSta, LenSta, ArrThick);
	DimSta.RemoveAll();
	LenSta.RemoveAll();
	ArrThick.RemoveAll();


	Dom.SetDirection("BOTTOM");
	double MaxH = pGir->GetHeightMaxByJijum() * YHeight;

	// 하판 공장이음 치수선 그림.
	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE, pBxStt, pBxEnd);
	dPreSta = staGirStt;
	dPreThick = 0;
	while(pBx)
	{
		dThick	= pBx->GetFactChain(G_F_L)->m_dFactChainThick * (pGlopt->GetDesignHeigtBaseLower()==0 ? 1 : 1);

		if (!pBx->IsState(BX_LOWERFLANGE) || dThick != dPreThick)
		{
			sta = pBx->GetStation();
			if (staStt < sta && DimSta.GetSize() == 0)
			{  // 치수선 첫부분이 중간에 걸림
				DimSta.Add(staStt);
				LenSta.Add(dPreSta);
			}
			if (staStt <= sta && sta <= staEnd)
			{
				DimSta.Add(sta);
				LenSta.Add(sta);
			}
			if (sta > staEnd)
			{  // 치수선 마지막이 중간에 걸림
				DimSta.Add(staEnd);
				LenSta.Add(sta);
			}
			ArrThick.Add(dThick);

			if (sta >= staEnd)
				break;
			dPreSta = sta;
			dPreThick = dThick;
		}
		pBx = Finder.GetBxNext();
	}
	OutputOfDimSectionSummary(&Dom, BaseY - MaxH, 1, "하  판(두께)", DimSta, LenSta, ArrThick);
	DimSta.RemoveAll();
	LenSta.RemoveAll();
	ArrThick.RemoveAll();

	// 복부판 치수선 그림.
	pBx = Finder.GetBxFirst(BX_STT_GIRDER | BX_END_GIRDER | BX_WEB, pBxStt, pBxEnd);
	dPreSta = staGirStt;
	dPreThick = 0;
	while(pBx)
	{
		dThick = pGir->GetThickJewon(G_W, pBx);
		if (!pBx->IsState(BX_WEB) || dThick != dPreThick)
		{
			sta = pBx->GetStation();
			if (staStt < sta && DimSta.GetSize() == 0)
			{  // 치수선 첫부분이 중간에 걸림
				DimSta.Add(staStt);
				LenSta.Add(dPreSta);
			}
			if (staStt <= sta && sta <= staEnd)
			{
				DimSta.Add(sta);
				LenSta.Add(sta);
			}
			if (sta > staEnd)
			{  // 치수선 마지막이 중간에 걸림
				DimSta.Add(staEnd);
				LenSta.Add(sta);
			}
			ArrThick.Add(dThick);

			if (sta >= staEnd)
				break;
			dPreSta = sta;
			dPreThick = dThick;
		}
		pBx = Finder.GetBxNext();
	}
	OutputOfDimSectionSummary(&Dom, BaseY - MaxH, 2, "복부판  두께", DimSta, LenSta, ArrThick);
	DimSta.RemoveAll();
	LenSta.RemoveAll();
	ArrThick.RemoveAll();

	Dom.Move (-staStt, 0);

	*pDom << Dom;	
}

void CFemStressCalc::OutputOfDimSectionSummary(CDomyun *pDom, double BaseY, long nDan, CString sTitle,
										  CDDWordArray &DimSta, CDDWordArray &LenSta, CDDWordArray &ArrThick)
{
	double staDim = DimSta.GetAt(0);
	double staLen = LenSta.GetAt(0);
	double thick  = 0;

	if (staDim == staLen)
		pDom->DimMoveTo(staDim, BaseY, nDan, sTitle);
	else
		pDom->DimMoveTo(staDim, BaseY, nDan, sTitle, FALSE);

	double staDimPre = staDim;
	double staLenPre = staLen;

	long nSize = DimSta.GetSize();
	for (long nDim = 1; nDim < nSize; nDim++)
	{
		staDim = DimSta.GetAt(nDim);
		staLen = LenSta.GetAt(nDim);

		CString cs = COMMA(staLen - staLenPre);
		CString cs2;							
		if(sTitle=="상  판  (폭)"||sTitle=="상  판(두께)"||sTitle=="하  판  (폭)"||sTitle=="하  판(두께)"||sTitle=="복부판  두께")
		{
			thick  = ArrThick.GetAt(nDim-1);
			if(thick==0)
				cs2.Format("");
			else
				cs2.Format("(%.fmm)", thick);
		}

		if (staDim == staLen)
		{
			if(sTitle=="상  판  (폭)"||sTitle=="상  판(두께)"||sTitle=="하  판  (폭)"||sTitle=="하  판(두께)"||sTitle=="복부판  두께")
				pDom->DimLineToExtend(staDim - staDimPre, nDan, cs, cs2);
			else
				pDom->DimLineTo(staDim - staDimPre, nDan, cs);
		}
		else
		{
			if(sTitle=="상  판  (폭)"||sTitle=="상  판(두께)"||sTitle=="하  판  (폭)"||sTitle=="하  판(두께)"||sTitle=="복부판  두께")
				pDom->DimLineToExtend(staDim - staDimPre, nDan, cs, cs2, FALSE);
			else
				pDom->DimLineToExtend(staDim - staDimPre, nDan, cs, "", FALSE);
		}

		staDimPre = staDim;
		staLenPre = staLen;
	}

}

///< x1: 시작데이터위치
///< y1: 시작데이터값
///< x2: 끝데이터위치
///< y2: 끝데이터값
///< x : 보간할 위치값
double CFemStressCalc::Bogan(double x1, double x2, double y1, double y2, double x)
{
	if(fabs(x1-x2) < 0.001)
		return (y1 + y2) / 2;
	if(fabs(x2-x) < 0.001)
		return y2;
	return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
}

void CFemStressCalc::InsertionSort(CDWordArray &Data)
{
	// 추가 정렬
	int i, j, nCount = Data.GetSize();
	for (i = 1; i < nCount; i++)   {
		double temp = Data [i];
		j = i;
		while (j > 0 && Data [j - 1] > temp)   {
			Data.SetAt (j, Data [j - 1]);
			j--;
		}
		Data.SetAt (j, (DWORD)temp);
	}
}



void CFemStressCalc::OutputOfFatigueCheckPos(CDomyun *pDom, BOOL bTitle, double TextHeight)
{
	CFemModelingCalc ModelCalc(m_pDataManage, m_pCalcStd);
	CARoadOptionStd *pOptStd = m_pDataManage->GetOptionStd();
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = NULL;
	CPlateBasicIndex *pBx = NULL;
	CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	double BaseX = 0;
	double BaseY = 0;
	double sta;
	double dRatio;
	long nEleIdx, nNodeIdx;
	CString cs;

	CDomyun Dom(pDom);


	// 단면 위치 그림.
	pOptStd->SetEnvType(&Dom, HCAD_STLC);
	for (long nDan = 1; nDan <= 3; nDan++)   
	{
		nNodeIdx = ModelCalc.GetMaxFatigueIndex(nDan, nEleIdx, dRatio, 1);  // DB일때 : 현재는 절점 위치만 나옴.
		if (nNodeIdx < 0)
			continue;
		pBx = m_pCalcStd->GetBxByJoint(pFEM->GetJoint((unsigned short)nNodeIdx));
		sta = pBx->GetStation();

		if(nDan==1)
		{
			pGir = pBx->GetGirder();

			BaseX = pGir->GetSSS(OFF_STT_SHOE);
			BaseY = 0;//100 * Scale;

			// 기준선 그림.
			pOptStd->SetEnvType(&Dom, HCAD_MIS2);
			Dom.MoveTo(BaseX, BaseY);
			Dom.LineTo(pGir->GetSSS(OFF_END_SHOE), BaseY);

			// 지점 심볼 그림.
			Dom.SetTextAlignHorz(TA_CENTER);
			for (long nK = 0; nK <= pDB->m_nQtyJigan; nK++)   
			{
				CPlateBasicIndex* pBx2 = pGir->GetBxOnJijum(nK);
				double sta2 = pBx2->GetStation();
				cs = pDB->m_strJijumName[nK];
				Dom.SetTextHeight(TextHeight);
				Dom.TextOut(CDPoint(sta2, BaseY - Dom.Always(10)), cs);
				Dom.Triangle(sta2, BaseY, 6);  // 지점 Symbol 그림.
			}
		}

		Dom.MoveTo(sta, BaseY + Dom.Always(15));
		Dom.LineTo(sta, BaseY - Dom.Always(15));
		cs.Format("단면 %d", nDan);
		Dom.TextOut(CDPoint(sta, BaseY + Dom.Always(17)), cs);
		cs.Format("절점 %d (G%d)", nNodeIdx + 1, pBx->GetNumberGirder() + 1);
		Dom.TextOut(CDPoint(sta, BaseY - Dom.Always(20)), cs);
	}
	if (bTitle)   {
		pOptStd->SetEnvType(&Dom, HCAD_SUB_TITLE);
		Dom.SetTextAlignHorz(TA_LEFT);
		Dom.TextOut(CDPoint(BaseX, BaseY + Dom.Always(25)), "< 피로조사 단면위치 >");
	}

	Dom.Move(-BaseX, 0);

//	Dom.RedrawByScale(25.0 / Scale);  // 아래 함수의 DimScale을 참조해야함.

	*pDom << Dom;
}

void CFemStressCalc::OutputOfExtraSection(CDomyun *pDom, CDRect rect)
{
	CPlateBridgeApp *pDB     = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();

	CDomyun Dom(pDom);
	Dom.SetTextAlignHorz (TA_CENTER);
	Dom.SetLineColor(6);

	long   nG   = 0;
	double dSta = 0;
	CString szTitle;
	double BaseX=0;
	for(long n=0; n<pData->m_ArrExtraSecTitle.GetSize(); n++)
	{
		nG   = pData->m_ArrExtraSecGir[n];
		dSta = pData->m_ArrExtraSecSta[n];
		szTitle = pData->m_ArrExtraSecTitle[n];
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		BaseX   = dSta - pGir->GetSSS(OFF_STT_GIRDER);

		Dom.MoveTo (BaseX, rect.top);
		Dom.LineTo (BaseX, rect.bottom);
		Dom.TextOut(BaseX, rect.bottom+Dom.Always(3), szTitle);
	}

	*pDom << Dom;
}

double CFemStressCalc::GetMapValueSecStress(CString sz)
{
	return m_MapSecStress[sz];
}


// 가로보 검토를 위한 영향선 종거의 총합 구하기
// 인자1 : 가로보 지간									= Length(L)
// 인자2 : 가로보 설치 간격								= Distance(B)
// 인자3 : 차륜하중(1),탱크하중(2),탱크트레일러하중(3)	= nLoadType
// 인자4 : 활하중(1),전단력(2),모멘트(3)				= nForceType
// 인자5 : 영향선 종거값의 배열(리턴)					= (CDDWordArray)DArrY 
// 리턴값 : 영향선 종거값의 합							= double

double CFemStressCalc::CalcInfluenceLineCross(double Length,double Distance,long nLoadType,long nForceType,CDDWordArray &DArrY)
{
	CCalcData *pData   = m_pDataManage->GetCalcData();
	double Btk=0;					// 탱크 및 탱크트레일러 궤도간 거리
	double L1=0,L2=0;
	long nCount = 0;
	double y1,y2,y3;				// 종거값
	double Sum = 0;				// 종거값들의 합
	double x = 0, y = 0;

	// 차륜하중(DB)
	if(nLoadType == 1) 
	{
		if(nForceType == 1) // 활하중
		{
			if(Distance < 4.2)
			{
				DArrY.Add(0.0);
				DArrY.Add(1.0);
				DArrY.Add(0.0);
			}
			else
			{
				y1 = (Distance - 4.2) / Distance;
				y2 = 1.0;
				y3 = y1;
				DArrY.Add(y1);
				DArrY.Add(y2);
				DArrY.Add(y3);
			}
		}
		if(nForceType == 2) // 전단력
		{
			nCount++;
			DArrY.Add(1.0);

			if(Length >= 1.2)
			{
				x = 1.2;
				while(Length >= x)
				{
					y = 1.0 - x / Length;
					DArrY.Add(y);
					if(nCount%2 == 0)	 x += 1.2;
					else    			 x += 1.8;
					nCount++;

					if(nCount==3) break;
				}
			}
		}
		if(nForceType == 3) // 모멘트
		{
			if(Length < 2.4)
			{
				y = Length / 4 * (1 - 2 * 0 / Length);
				DArrY.Add(y);
			}
			if(Length < 3.6) 
			{
				y = Length / 4 * (1 - 2 * 0 / Length);
				DArrY.Add(y);
				y = Length / 4 * (1 - 2 * 1.2 / Length);
				DArrY.Add(y);				
			}
			else
			{
				y = Length / 4 * (1 - 2 * 1.8 / Length);
				DArrY.Add(y);
				y = Length / 4 * (1 - 2 * 0 / Length);
				DArrY.Add(y);
				y = Length / 4 * (1 - 2 * 1.2 / Length);
				DArrY.Add(y);
			}
			
		}
	}

	// 탱크하중
	if(nLoadType == 2) 
	{
		L1  = pData->m_TankEdit1;
		if(nForceType == 1) // 활하중
		{
			if(Distance < L1 / 2.0) DArrY.Add(1.0);
			else
			{
				y1 = (Distance - L1 / 2.0) / Distance;
				y2 = 1.0;
				y3 = y1;
				DArrY.Add(y1);
				DArrY.Add(y2);
				DArrY.Add(y3);
			}
		}
		if(nForceType == 2) // 전단력
		{
			Btk = pData->m_TankEdit2 + pData->m_TankEdit3;
			DArrY.Add(1.0);
			if(Length >= Btk) 
			{
				y = 1.0 - Btk / Length;
				DArrY.Add(y);
			}
		}
		if(nForceType == 3) // 모멘트
		{
			Btk = pData->m_TankEdit2 + pData->m_TankEdit3;
			y = Length / 4 * (1 - 2 * 0 / Length);
			DArrY.Add(y);
			if(Length >= Btk * 2)
			{
				y = Length / 4 * (1 - 2 * Btk / Length);
				DArrY.Add(y);
			}
		}
	}

	// 탱크트레일러 하중
	if(nLoadType == 3) 
	{
		L1  = pData->m_TrailEdit1;
		L2  = pData->m_TrailEdit2;
		if(nForceType == 1) // 활하중
		{
			if(Distance < L1) DArrY.Add(1.0);
			else
			{
				y1 = (Distance - L1) / Distance;
				y2 = 1.0;
				y3 = (Distance - L2) / Distance;
				DArrY.Add(y1);
				DArrY.Add(y2);
				DArrY.Add(y3);
			}
		}
		if(nForceType == 2) // 전단력
		{
			Btk = pData->m_TrailEdit5 + pData->m_TrailEdit6;
			DArrY.Add(1.0);
			if(Length >= Btk) 
			{
				y = 1.0 - Btk / Length;
				DArrY.Add(y);
			}
		}
		if(nForceType == 3) // 모멘트
		{
			Btk = pData->m_TrailEdit5 + pData->m_TrailEdit6;
			y = Length / 4 * (1 - 2 * 0 / Length);
			DArrY.Add(y);
			if(Length >= Btk * 2)
			{
				y = Length / 4 * (1 - 2 * Btk / Length);
				DArrY.Add(y);
			}
		}
	}

	long nDArrYSize = DArrY.GetSize();
	for(long i=0; i<nDArrYSize; i++)
	{
		y = DArrY[i];
		Sum += y;
	}

	return Round(Sum,3);
}
 
// 세로보 검토를 위한 영향선 종거의 총합 구하기
// 인자1 : 가로보 지간									= Length(L)
// 인자2 : 가로보 설치 간격								= Distance(B)
// 인자3 : 차륜하중(1),탱크하중(2),탱크트레일러하중(3)	= nLoadType
// 인자4 : 활하중(1),전단력(2),모멘트(3)				= nForceType
// 인자5 : 영향선 종거값의 배열(리턴)					= (CDDWordArray)DArrY 
// 리턴값 : 영향선 종거값의 합							= double

double CFemStressCalc::CalcInfluenceLineStg(double Length,double Distance,long nLoadType,long nForceType,CDDWordArray &DArrY)
{
	CCalcData        *pData   = m_pDataManage->GetCalcData();
	double Btk = 0;  // 탱크 및 탱크트레일러 궤도간 거리
	double L1=0,L2=0;
	long   nChasu=0;  // 작용 하중 최대 수
	double y=0;	  // 종거값
	double Sum = 0;  // 종거값들의 합

	if(nLoadType == 1) // 차륜하중(DB)
	{
		if(nForceType == 1) // 활하중
		{
			if(Length <= 2.4)	   nChasu = 1;
			else if(2.4 < Length && Length <= 3.6) nChasu = 2;
			else if(3.6< Length && Length <= 6)   nChasu = 3;
			else if(6.0< Length && Length <= 8.4) nChasu = 5;
			else if(8.4< Length && Length <= 9.6) nChasu = 6;
			else				   nChasu = 7;
			
			if(nChasu == 1)
			{
				y = 1.0;
				DArrY.Add(y);
			}
			if(nChasu == 2)
			{
				y = 1.0;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.2 / Length;
				DArrY.Add(y);
			}
			if(nChasu == 3)
			{
				y = 1.0;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.2 / Length;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.8 / Length;
				DArrY.Add(y);
			}
			if(nChasu == 5)
			{
				y = 1.0;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.2 / Length;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.8 / Length;
				DArrY.Add(y);
				y = (1.0 - 2 * 3.0 / Length);
				DArrY.Add(y);
				y = (1.0 - 2 * 3.0 / Length);
				DArrY.Add(y);
			}
			if(nChasu == 6)
			{
				y = 1.0;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.2 / Length;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.8 / Length;
				DArrY.Add(y);
				y = (1.0 - 2 * 3.0 / Length);
				DArrY.Add(y);
				y = (1.0 - 2 * 3.0 / Length);
				DArrY.Add(y);
				y = 1.0 - 2 * 4.2 / Length;
				DArrY.Add(y);
			}
			if(nChasu == 7)
			{
				y = 1.0;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.2 / Length;
				DArrY.Add(y);
				y = 1.0 - 2 * 1.8 / Length;
				DArrY.Add(y);
				y = (1.0 - 2 * 3.0 / Length);
				DArrY.Add(y);
				y = (1.0 - 2 * 3.0 / Length);
				DArrY.Add(y);
				y = 1.0 - 2 * 4.2 / Length;
				DArrY.Add(y);
				y = 1.0 - 2 * 4.8 / Length;
				DArrY.Add(y);
			}
		}
		if(nForceType == 2) // 전단력
		{
			DArrY.Add(1.0);
			if(Distance >= 4.2)
			{
				y = 1.0 - 4.2 / Distance;
				DArrY.Add(y);
			}
			if(Distance >= 8.4)
			{
				y = 1.0 - 8.4 / Distance;
				DArrY.Add(y);
			}
		}
		if(nForceType == 3) // 모멘트
		{
			DArrY.Add(1.0);
			if(Distance >= 4.2)
			{
				y = 1.0 - 4.2 / Distance;
				DArrY.Add(y);
			}
			if(Distance >= 8.4)
			{
				y = 1.0 - 8.4 / Distance;
				DArrY.Add(y);
			}
		}
	}

	if(nLoadType == 2) // 탱크하중
	{
		if(nForceType == 1) // 활하중
		{
			Btk = pData->m_TankEdit2 + pData->m_TankEdit3;
			DArrY.Add(1.0);
			if(Length >= Btk*2) 
			{
				y = 1.0 - 2 * Btk / Length;
				DArrY.Add(y);
			}
		}
		if(nForceType == 2) // 전단력
		{
			L1  = pData->m_TankEdit1;
			DArrY.Add(1.0);
			if(Distance >= L1) 
			{
				y = 1.0 - L1 / Distance;
				DArrY.Add(y);
			}
		}
		if(nForceType == 3) // 모멘트
		{
			L1  = pData->m_TankEdit1;
			DArrY.Add(1.0);
			if(Distance >= L1*2)
			{
				y = 1.0 - L1 * 2 / Distance;
				DArrY.Add(y);
			}
		}
	}

	if(nLoadType == 3) // 탱크트레일러 하중
	{
		if(nForceType == 1) // 활하중
		{
			Btk = pData->m_TrailEdit5 + pData->m_TrailEdit6;
			DArrY.Add(1.0);
			if(Length >= Btk*2) 
			{
				y = 1.0 - Btk * 2 / Length; 
				DArrY.Add(y);
			}
		}
		if(nForceType == 2) // 전단력
		{
			L2  = pData->m_TrailEdit2;
			DArrY.Add(1.0);
			if(Distance >= L2) 
			{
				y = 1.0 - L2 / Distance;
				DArrY.Add(y);
			}
		}
		if(nForceType == 3) // 모멘트
		{
			L2  = pData->m_TrailEdit2;
			DArrY.Add(1.0);
			if(Distance >= L2*2)
			{
				y = 1.0 - L2 * 2 / Distance;
				DArrY.Add(y);
			}
		}
	}

	long nDArrYSize = DArrY.GetSize();
	for(long i=0; i<nDArrYSize; i++)
	{
		y = DArrY[i];
		Sum += y;
	}

	return Round(Sum,3);
}

// dSta의 단면 면적을 구함.
double CFemStressCalc::GetAreaOfSectionByStation(long nG, double dSta)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);

	double dWidthUp = pGir->GetWidthOnStation(dSta, TRUE);
	double dThickUp = pGir->GetThickFactByStation(dSta, G_F_U);
	double dWidthDn = pGir->GetWidthOnStation(dSta, FALSE);
	double dThickDn = pGir->GetThickFactByStation(dSta, G_F_L);
	double dHeightWeb= pGir->GetHeightGirderByStaAng(dSta);
	double dThickWeb = pGir->GetThickFactByStation(dSta, G_W);

	return dWidthUp*dThickUp + dWidthDn*dThickDn + dHeightWeb*dThickWeb;
}


// 현재 단면의 I33값을 가져올때 스테이션의 위치
double CFemStressCalc::GetStationCheckPos(CPlateBasicIndex *pBx)
{
	double dStaCheck = 0;
	long nByun = GetByunDanStyle(pBx);
	if(nByun==0)
	{
		if(pBx->IsDanbuJijumStt() || pBx->IsState(BX_STT_BENDING_UPPER))
			dStaCheck = pBx->GetStation()+1;
		else if(pBx->IsDanbuJijumEnd() || pBx->IsState(BX_END_BENDING_UPPER))
			dStaCheck = pBx->GetStation()-1;
		else
			dStaCheck = pBx->GetStation()+1;
	}
	else
		dStaCheck = pBx->GetStation();

	return dStaCheck;
}


// 현재 단면의 I33값을 가져올때 스테이션의 위치
double CFemStressCalc::GetStationCheckPos(long nG, double dSta)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp  *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx     = pGir->GetBxByStation(dSta);
	
	return	GetStationCheckPos(pBx);
}



void CFemStressCalc::GetAddMaxForceSect(CDWordArray &DArrnG, CDDWordArray &DArrSta)
{
	CPlateBridgeApp  *pDB     = m_pDataManage->GetBridge();
	long	nGsu = pDB->GetGirdersu();
	long	nKsu = pDB->m_nQtyJigan;
	double	dSttSta = 0, dEndSta = 0;
	
	for(long nG = 0; nG < nGsu; nG++)
	{
		for(long nK = 0; nK < nKsu; nK++)
		{
			dSttSta = pDB->GetGirder(nG)->GetBxOnJijum(nK)->GetStation();
			dEndSta = pDB->GetGirder(nG)->GetBxOnJijum(nK+1)->GetStation();

			GetElementForceTerm(ELE_MOMENT3, LC_SUM_MAX, nG, dSttSta, dEndSta, 1);

			DArrnG.Add(nG);
			DArrSta.Add(dSttSta);
		}
	}
}


void CFemStressCalc::GetAddMinSect(CDWordArray &DArrnG, CDDWordArray &DArrSta)
{
	CPlateBridgeApp  *pDB     = m_pDataManage->GetBridge();
	long nGsu = pDB->GetGirdersu();
	long nKsu = pDB->m_nQtyJigan;
	double dSttSta = 0, dEndSta = 0;
	double dStaMinSection = 0;
	
	BOOL bFindStt=FALSE, bFindEnd=FALSE;
	for(long nG = 0; nG < nGsu; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);

		for(long nK = 0; nK < nKsu; nK++)
		{
			dSttSta = pDB->GetGirder(nG)->GetBxOnJijum(nK)->GetStation();
			dEndSta = pDB->GetGirder(nG)->GetBxOnJijum(nK+1)->GetStation();

			bFindStt = bFindEnd = FALSE;

			// 최소 단면 찾기
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_STT_BYUN_UPPER| BX_END_BYUN_UPPER);
			while(pBx)
			{
				double dCurSta = pBx->GetStation();
				if(dCurSta >= dSttSta && dCurSta <= dEndSta)
				{
					if(pBx->IsState(BX_STT_BYUN_UPPER))
					{
						bFindStt = TRUE;
						dSttSta = dCurSta;
					}
					if(pBx->IsState(BX_END_BYUN_UPPER))
					{
						bFindEnd = TRUE;
						dEndSta = dCurSta;
					}
				}
				pBx = Finder.GetBxNext();
			}
			// 변단면 종류 파악 - 유형2와 유형3에 대비
			if(bFindStt && !bFindEnd) dSttSta = dEndSta;
			if(!bFindStt && bFindEnd) dEndSta = dSttSta;

			dStaMinSection = GetStationMinHeight(nG, dSttSta, dEndSta);

			DArrnG.Add(nG);
			DArrSta.Add(dStaMinSection);
		}
	}
}



// 구간사이에서 최소 웨브 높이의 위치 구하기
double CFemStressCalc::GetStationMinHeight(long nG, double dSttSta, double dEndSta)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir= pDB->GetGirder(nG);

	if(dEndSta < dSttSta) return 0; // error
	if(dEndSta == dSttSta) return dSttSta;	// 위치가 한군데 일때
	
	long   nCount = 0;
	double dSta   = 0;
	double dStep  = 100; // 100mm
	double dMinHeight = 100000, dHeight = 0;
	double dStaMinHeight = 0;
	BOOL bOver = FALSE;

	while(dSta < dEndSta)
	{
		dSta = dSttSta + dStep * nCount++;
		if(dSta > dEndSta) 
		{
			dSta = dEndSta;
			bOver = TRUE;
		}
		dHeight = pGir->GetHeightGirderByStaAng(dSta);

		if(dHeight < dMinHeight)
		{
			dMinHeight = dHeight;
			dStaMinHeight = dSta; 
		}
		if(bOver) break;
	}

	return dStaMinHeight;
}

CFrameSection* CFemStressCalc::GetFrameSectionByIndex(CFEMManage *pFEM, long nIndex)
{
	long nMatch = 0;
	POSITION posSec = pFEM->m_FrameSectionList.GetHeadPosition();
	while(posSec)
	{
		CFrameSection *pFrSec =  (CFrameSection*)pFEM->m_FrameSectionList.GetNext(posSec);  
		if(nIndex==nMatch)
			return pFrSec;
	
		nMatch++;
	}

	return NULL;
}

void CFemStressCalc::CompareMaterialRemoveDup(CDWordArray &DArnG, CDDWordArray &DArSta, CDWordArray &DArMat)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CFEMManage		*pFEM	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	long nG = 0;
	double dSta = 0;
	double up_f_t = 0, lo_f_t = 0, w_t = 0, up_f_l = 0, lo_f_l = 0, w_h = 0;
	BOOL bSameMat = FALSE;
	CDWordArray DArDeleteIdx;
	long n = 0;
	for(n = 0; n < DArnG.GetSize(); n++)
	{
		nG   = DArnG[n];
		dSta = DArSta[n];

		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		up_f_t		= GetThickFlangeBySta(G_F_U, nG, dSta);
		lo_f_t		= GetThickFlangeBySta(G_F_L, nG, dSta);
		w_t			= GetThickFlangeBySta(G_W, nG, dSta);
		w_h			= Round(pGir->GetHeightGirderByStaAng(dSta), 0);		
		up_f_l		= pGir->GetWidthOnStation(dSta, TRUE);
		lo_f_l		= pGir->GetWidthOnStation(dSta, FALSE);

		// 기존의 Materials
		for(long m = 0; m < DArMat.GetSize(); m++)
		{
			bSameMat    = FALSE;
			CFrameSection *pFrSec = GetFrameSectionByIndex(pFEM, DArMat[m]-1);

			if(up_f_t   == pFrSec->m_dJewon[SEC_TU]	&&	lo_f_t == pFrSec->m_dJewon[SEC_TL]
			&& w_t	    == pFrSec->m_dJewon[SEC_TW] &&	w_h    == pFrSec->m_dJewon[SEC_HW]
			&& up_f_l   == pFrSec->m_dJewon[SEC_BU] &&  lo_f_l == pFrSec->m_dJewon[SEC_BL])
				bSameMat = TRUE;	

			if(bSameMat) break;
		}
		if(bSameMat)	DArDeleteIdx.Add(n);
	}

	// 같은 Material 지우기
	long nDeletesu = DArDeleteIdx.GetSize();
	for(n = nDeletesu - 1; n >= 0; n--)
	{
		long nIdx = DArDeleteIdx[n];
		DArnG.RemoveAt(nIdx);
		DArSta.RemoveAt(nIdx);
	}
}


double CFemStressCalc::GetThickFlangeBySta(long nMarkBuje, long nG, double dSta, BOOL bSmall/*TRUE*/)
{
	CPlateBridgeApp *pDB  = m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);

	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex *pBxGirStt = Finder.GetBxFirst(BX_STT_GIRDER);

	double dThickBx = 0, dStaBx = 0, dThick = 0;
	double dThickBxPre = pBxGirStt->GetFactChain(nMarkBuje)->m_dFactChainThick;

	__int64 BxFlag;
	switch (nMarkBuje)  
	{
		case G_F_U : BxFlag = BX_UPPERFLANGE;	break;		
		case G_F_L : BxFlag = BX_LOWERFLANGE;	break;		
		case G_W :	 BxFlag = BX_WEB;			break;
	}
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag);

	while(pBx)
	{
		dThickBx = pBx->GetFactChain(nMarkBuje)->m_dFactChainThick;	
		dStaBx = pBx->GetStation();

		if(dStaBx == dSta)
		{
			if(bSmall)	dThick = (dThickBxPre >= dThickBx) ? dThickBx : dThickBxPre;
			else    	dThick = dThickBx;
			return		dThick;
		}

		if(dStaBx > dSta)	break;

		pBx = Finder.GetBxNext();
		dThickBxPre = dThickBx;
	}

	return dThickBxPre;
}

// 최소단면 중복 검사및 추가
void CFemStressCalc::CompareHeightAddSect(CDWordArray &DArnG, CDDWordArray &DArSta, CDWordArray &DArnG2, CDDWordArray &DArSta2, CDWordArray &DArType)
{
	CPlateBridgeApp		*pDB    = m_pDataManage->GetBridge();
	CPlateBasicIndex	*pBx	= NULL;
	CPlateBasicIndex	*pBx2	= NULL;
	CPlateGirderApp		*pGir	= NULL;
	CPlateGirderApp		*pGir2	= NULL;

	long nG = 0, nG2 = 0;
	double dSta = 0, dSta2 = 0;
	double up_f_t = 0, lo_f_t = 0, w_t = 0, up_f_l = 0, lo_f_l = 0, w_h = 0;
	double up_f_t2 = 0, lo_f_t2 = 0, w_t2 = 0, up_f_l2 = 0, lo_f_l2 = 0, w_h2 = 0;
	BOOL bSameMat = FALSE;
	CDWordArray DArDeleteIdx;
	long n = 0;
	for(n = 0; n < DArnG2.GetSize(); n++)
	{
		nG2		= DArnG2[n];
		dSta2	= DArSta2[n];

		pGir2	= pDB->GetGirder(nG2);
		pBx2	= pGir2->GetBxByStationDir(dSta2, 0);

		up_f_t2	= pGir2->GetThickJewon(G_F_U, pBx2);
		lo_f_t2	= pGir2->GetThickJewon(G_F_L, pBx2);
		w_t2	= pGir2->GetThickJewon(G_W, pBx2);
		w_h2	= Round(pBx2->GetHeightGirderByBx(), 0);
		up_f_l2	= pGir2->GetWidthOnStation(dSta2, TRUE);
		lo_f_l2	= pGir2->GetWidthOnStation(dSta2, FALSE);

		// 부재력 최대 단면과 비교
		for(long m = 0; m < DArnG.GetSize(); m++)
		{
			nG = DArnG[m];
			dSta = DArSta[m];
			bSameMat = FALSE;

			pGir = pDB->GetGirder(nG);
			pBx = pGir->GetBxByStationDir(dSta, 0);

			up_f_t		= GetThickFlangeBySta(G_F_U, nG, dSta);
			lo_f_t		= GetThickFlangeBySta(G_F_L, nG, dSta);
			w_t			= GetThickFlangeBySta(G_W, nG, dSta);
			w_h			= Round(pGir->GetHeightGirderByStaAng(dSta), 0);		
			up_f_l		= pGir2->GetWidthOnStation(dSta, TRUE);
			lo_f_l		= pGir2->GetWidthOnStation(dSta, FALSE);

			if(up_f_t   == up_f_t2		&&	lo_f_t   == lo_f_t2
			&& w_t	    == w_t2			&&	w_h      == w_h2		
			&& up_f_l   == up_f_l2		&&  lo_f_l   == lo_f_l2)	
				bSameMat = TRUE;	

			if(bSameMat) break;
		}
		if(bSameMat)	DArDeleteIdx.Add(n);
	}

	// 같은 Material 지우기
	long nDeletesu = DArDeleteIdx.GetSize();
	for(n = nDeletesu - 1; n >= 0; n--)
	{
		long nIdx = DArDeleteIdx[n];
		DArnG2.RemoveAt(nIdx);
		DArSta2.RemoveAt(nIdx);
	}

	DArType.RemoveAll();
	for(long i = 0; i < DArnG.GetSize(); i++)
	{
		DArType.Add(1);	//1:변단면시 부재력 최대
	}

	// 부재력이 제일 큰 최소단면 찾기
	long nGMin = 0;
	double dStaMin = 0;
	double dMinHeight = 0, dHeight = 0;
	double dMaxMoment = 0, dMoment = 0;
	for(n = 0; n < DArnG2.GetSize(); n++)
	{
		nG = DArnG2[n];
		dSta = DArSta2[n];
		pGir = pDB->GetGirder(nG);
		pBx = pGir->GetBxByStationDir(dSta, 0);
		dMinHeight = pBx->GetHeightGirderByBx();
		nGMin = nG;
		dStaMin = dSta;

		for(long m = 0; m < DArnG2.GetSize(); m++)
		{
			if(m == n) continue;
			nG2 = DArnG2[m];
			dSta2 = DArSta2[m];
			pGir2 = pDB->GetGirder(nG2);
			pBx2 = pGir->GetBxByStationDir(dSta2, 0);
			dHeight = pBx2->GetHeightGirderByBx();
			
			if(dMinHeight > dHeight)
			{
				dMinHeight = dHeight;
				nGMin = nG2;
				dStaMin = dSta2;
			}
			if(fabs(dMinHeight - dHeight) < 0.00001)	// 같을때
			{
				dMaxMoment = GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nGMin, dStaMin, 1);
				dMoment    = GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG2, dSta2, 1);
				if(dMaxMoment < dMoment)
				{
					dMinHeight = dHeight;
					nGMin = nG2;
					dStaMin = dSta2;
				}
			}
		}
	}

	// DArnG, DArSta 에 nGMin, dStaMin 추가
	if(DArnG2.GetSize() > 0)
	{
		DArnG.Add(nGMin);
		DArSta.Add(dStaMin);
		DArType.Add(2);	//2:변단면시 최소단면
	}

}
