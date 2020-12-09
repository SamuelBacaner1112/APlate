// PlateStressOut.cpp: implementation of the CPlateStressOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateCalc/APlateCalc.h"
#include "generalinput.h"
#include "../APlateDBDraw/APlateDBDraw.h"
#include "../APlateData/APlateData.h"
#include "PlateStressOut.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define RE_RED		RGB(255, 0, 0)
#define RE_BLUE		RGB(0, 0, 255)
#define RE_BLACK	RGB(0, 0, 0)
#define RE_WHITE	RGB(255, 255, 255)
#define maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateStressOut::CPlateStressOut(CGeneralBaseStd* pStd)
{
	m_pStd = pStd;
}

CPlateStressOut::~CPlateStressOut()
{


}

void CPlateStressOut::ElementForceOutput(CStringArray &Arr)
{
	CAPlateCalcStd	*pCalcStd	= m_pStd->m_pCalcStd;
	CCalcData		*pData		= m_pStd->GetDataManage()->GetCalcData();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CPlateBridgeApp	*pDB		= (CPlateBridgeApp*)m_pStd->GetBridge();
	
	char sBuf [500];
	Arr.Add ("  << 부 재 력 >>\r\n");
	for (int nTempForce = 0; nTempForce < 3; nTempForce++) 
	{
		int nForce;
		switch (nTempForce)
		{
		case 0 :
			Arr.Add (" < BENDING MOMENT (kN.m) >");
			nForce = ELE_MOMENT3;
			break;
		case 1 :
			Arr.Add (" < SHEAR FORCE (kN) >");
			nForce = ELE_SHEAR_2;
			break;
		case 2 :
			Arr.Add (" < TORSION MOMENT (kN.m) >");
			nForce = ELE_AXTORSN;
			break;
		}
		if(pDB->IsTUGir())
		{
			Arr.Add ("┏━━┳━━┳━━━━━━━━━━━━━━━━━━━━┳━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━┳━━┓");
			Arr.Add ("┃부재┃절점┃             합성전 고정하중            ┃  합성후  ┃      활 하 중      ┃     지점 침하      ┃         합    계         ┃비고┃");
			Arr.Add ("┃번호┃번호┃ 강재자중 구속콘자중  180˚터닝  바닥판 ┃ 고정하중 ┃   Max        Min   ┃   Max        Min   ┃      Max        Min      ┃    ┃");
			Arr.Add ("┣━━╋━━╋━━━━━━━━━━━━━━━━━━━━╋━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━━━━╋━━┫");
		}
		else
		{
			Arr.Add ("┏━━┳━━┳━━━━━━━━━━┳━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━┳━━┓");
			Arr.Add ("┃부재┃절점┃   합성전 고정하중  ┃  합성후  ┃      활 하 중      ┃     지점 침하      ┃         합    계         ┃비고┃");
			Arr.Add ("┃번호┃번호┃  Steel    Concrete ┃ 고정하중 ┃   Max        Min   ┃   Max        Min   ┃      Max        Min      ┃    ┃");
			Arr.Add ("┣━━╋━━╋━━━━━━━━━━╋━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━━━━╋━━┫");
		}
		
		// 결과 Data Line 출력
		CString szBujeNum, szJointNum, szDeadSteel, szDeadBind, szDeadTurn, szDeadSlab, szPostDead;
		CString szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szName;
		double dDeadSteel, dDeadBind, dDeadTurn, dDeadSlab, dPostDead;
		double dLiveMax, dLiveMin, dUnSettleMax, dUnSettleMin, dTotalMax, dTotalMin;
		
		CFEMManage	*pFemPre = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);	///< 합성전 고정하중
		CFEMManage	*pFemPos = pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);	///< 합성후 고정하중
		CFEMManage	*pFemLiv = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);		///< 활하중
		CFEMManage	*pFemSet = pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS);		///< 지점침하
		CFEMManage	*pFemCon = NULL;
		CResultArray	*pPreResult = pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);	///< 합성전 고정하중
		CResultArray	*pPosResult = pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);	///< 합성후 고정하중
		CResultArray	*pLivResult = pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);		///< 활하중
		CResultArray	*pSetResult = pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);		///< 지점침하
		CResultArray	*pConResult = NULL;

		if(pDB->IsTUGir())
		{
			pFemCon = pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);		///< 합성전-180도 터닝 + 슬래브(+가로보)
			pConResult	= pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS);
		}

		for (long i = 0; i < pFemPre->GetElementSize(); i++)  
		{
			CElement *pEPre	= pFemPre->GetElement((unsigned short)i);
			CElement *pECon = pFemCon ? pFemCon->GetElement((unsigned short)i) : NULL;

			CElementResult *pEPreResult	= pPreResult->m_EleResultArr.GetAt(i);
			CElementResult *pEPosResult	= pPosResult->m_EleResultArr.GetAt(i);
			CElementResult *pELivResult	= pLivResult->m_EleResultArr.GetAt(i);
			CElementResult *pESetResult	= pSetResult->m_EleResultArr.GetAt(i);
			CElementResult *pEConResult = pFemCon ? pConResult->m_EleResultArr.GetAt(i) : NULL;

			if(pEPre->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
			{
				for (long nPoint = 0; nPoint < 2; nPoint++) 
				{
					if (nPoint == 0)  
					{
						ModelCalc.GetJijumNameOfJoint(pEPre->m_nJStt, szName);
						dDeadSteel	= Round(pEPreResult->m_dPreDeadSteel[nForce][0],3);
						dDeadBind	= pECon ? Round(pEConResult->m_dPreDeadConf[nForce][0], 3)	: 0;
						dDeadTurn	= pECon ? Round(pEConResult->m_dPreDeadTurning[nForce][0],3)	: 0;
						dDeadSlab	= pECon ? Round(pEConResult->m_dPreDeadSlab[nForce][0],3)		: Round(pEPreResult->m_dPreDeadSlab[nForce][0],3);
						dPostDead	= Round(pEPosResult->m_dPostDead[nForce][0],3);
						dLiveMax	= Round(pELivResult->m_dLiveMax[nForce][0],3);
						dLiveMin	= Round(pELivResult->m_dLiveMin[nForce][0],3);
						if(pData->m_bCheckSettlement)
						{
							dUnSettleMax = Round(pESetResult->m_dUnSettleMax[nForce][0],3);
							dUnSettleMin = Round(pESetResult->m_dUnSettleMin[nForce][0],3);
						}
						else
						{
							dUnSettleMax = 0;
							dUnSettleMin = 0;
						}
					}
					else
					{
						ModelCalc.GetJijumNameOfJoint(pEPre->m_nJEnd, szName);
						dDeadSteel	= Round(pEPreResult->m_dPreDeadSteel[nForce][FRAME_SEG_SIZE],3);
						dDeadBind	= pECon ? Round(pEConResult->m_dPreDeadConf[nForce][FRAME_SEG_SIZE],3)		: 0;
						dDeadTurn	= pECon ? Round(pEConResult->m_dPreDeadTurning[nForce][FRAME_SEG_SIZE],3)	: 0;
						dDeadSlab	= pECon ? Round(pEConResult->m_dPreDeadSlab[nForce][FRAME_SEG_SIZE],3)		:
											  Round(pEPreResult->m_dPreDeadSlab[nForce][FRAME_SEG_SIZE],3);
						dPostDead	= Round(pEPosResult->m_dPostDead[nForce][FRAME_SEG_SIZE],3);
						dLiveMax	= Round(pELivResult->m_dLiveMax[nForce][FRAME_SEG_SIZE],3);
						dLiveMin	= Round(pELivResult->m_dLiveMin[nForce][FRAME_SEG_SIZE],3);
						if(pData->m_bCheckSettlement)
						{
							dUnSettleMax = Round(pESetResult->m_dUnSettleMax[nForce][FRAME_SEG_SIZE],3);
							dUnSettleMin = Round(pESetResult->m_dUnSettleMin[nForce][FRAME_SEG_SIZE],3);
						}
						else
						{
							dUnSettleMax = 0;
							dUnSettleMin = 0;
						}
					}
					
					szDeadSteel.Format  ("%9.3lf", dDeadSteel);
					szDeadBind.Format	("%9.3lf", dDeadBind);
					szDeadTurn.Format	("%9.3lf", dDeadTurn);
					szDeadSlab.Format   ("%9.3lf", dDeadSlab);
					szPostDead.Format   ("%9.3lf", dPostDead);
					szLiveMax.Format    ("%9.3lf", dLiveMax);
					szLiveMin.Format    ("%9.3lf", dLiveMin);					

					szUnSettleMax.Format("%9.3lf", dUnSettleMax);
					szUnSettleMin.Format("%9.3lf", dUnSettleMin);

					dTotalMax = Round(pEPreResult->m_dTotalMax[nForce][nPoint==0 ? 0 : FRAME_SEG_SIZE],3);
					dTotalMin = Round(pEPreResult->m_dTotalMin[nForce][nPoint==0 ? 0 : FRAME_SEG_SIZE],3);

					szTotalMax.Format   ("%11.3lf", dTotalMax);
					szTotalMin.Format   ("%11.3lf", dTotalMin);

					if(nPoint == 0)
					{
						szBujeNum.Format("%4d", pEPre->m_nIndex+1);
						szJointNum.Format("%4d", pEPre->m_nJStt+1);
					}
					else
					{
						szBujeNum.Format("    ");
						szJointNum.Format("%4d", pEPre->m_nJEnd+1);
					}

					if(pDB->IsTUGir())
					{
						sprintf(sBuf, "┃%s┃%s┃%s %s %s %s ┃%s ┃%s %s ┃%s %s ┃%s  %s  ┃ %s ┃", 
									szBujeNum, szJointNum, szDeadSteel, szDeadBind, szDeadTurn, szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szName);
					}
					else
					{
						sprintf(sBuf, "┃%s┃%s┃%s %s ┃%s ┃%s %s ┃%s %s ┃%s  %s  ┃ %s ┃", 
									szBujeNum, szJointNum, szDeadSteel,	szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szName);
					}

					Arr.Add(sBuf);
				}
			}
		}
		if(pDB->IsTUGir())
			Arr.Add ("┗━━┻━━┻━━━━━━━━━━━━━━━━━━━━┻━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━━┻━━┛\r\n\r\n");
		else
			Arr.Add ("┗━━┻━━┻━━━━━━━━━━┻━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━━┻━━┛\r\n\r\n");
	}
}

void CPlateStressOut::NodeReactionOutput(CStringArray &Arr)
{
	CAPlateCalcStd		*pCalcStd	= m_pStd->m_pCalcStd;
	CCalcData			*pData		= m_pStd->GetDataManage()->GetCalcData();
	CFemModelingCalc	pModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CPlateBridgeApp		*pDB		= (CPlateBridgeApp*)m_pStd->GetBridge();
	CFEMManage			*pFEMManage	= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	char sBuf [512];
	CString sText;
	CString szDeadSteel, szDeadConc, szDeadTurn, szDeadSlab, szPostDead;
	CString szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szShoeForce, szName, szTemp;
	double dDeadSteel, dDeadConc, dDeadTurn, dDeadSlab, dPostDead;
	double dLiveMax, dLiveMin, dUnSettleMax, dUnSettleMin, dTotalMax, dTotalMin;
	long	nForce		= JOINT_REACTION;
	char	nTitle		= 'A';
	BOOL	bExist		= FALSE;
	long	nGirdersu	= pDB->GetGirdersu();

//	for(long nLoadGir=0; nLoadGir < nGirdersu; nLoadGir++)
	for(long nLoadGir=0; nLoadGir < 2; nLoadGir++)
	{		
		if(nLoadGir==0)	sText.Format("%c. 좌측 최대인 경우\r\n", nTitle++);
		else			sText.Format("%c. 우측 최대인 경우\r\n", nTitle++);
//		sText.Format("%c. G%d 최대인 경우\r\n", nTitle++, nLoadGir + 1);
		Arr.Add (sText);
		
		long nLoadTypesu = 4;
		if(nLoadGir == nGirdersu - 1)	nLoadTypesu = 6;

		for(long nLoadType = 1; nLoadType <= nLoadTypesu; nLoadType++)
		{						
			pModelCalc.GetLiveLoadReaction(nLoadType, nLoadGir, 0, 0, bExist);   // 해당 LC 있는지 조사.
			if (!bExist)   
			{
				pModelCalc.GetLiveLoadReaction(nLoadType, nLoadGir, 1, 0, bExist);   // 해당 LC 있는지 조사.				
				if (!bExist) continue;
			}
			
			switch (nLoadType)
			{
				case 1 :	sText =" 1) DB 하중\r\n";break;
				case 2 :	sText =" 2) DL 하중\r\n";break;
				case 3 :	sText =" 3) TANK 하중\r\n";break;
				case 4 :	sText =" 4) TANK TRAILER 하중\r\n";break;
				case 5 :	sText.Format("%c. 보도 하중\r\n", nTitle++);break;
				case 6 :	sText.Format("%c. 풍하중\r\n", nTitle++);	break;
			}
			Arr.Add(sText);

			long nMaxLanesu = 20;
			for(long nLane = 0; nLane <= nMaxLanesu; nLane++)
			{
				pModelCalc.GetLiveLoadReaction(nLoadType, nLoadGir, nLane, 0, bExist);   // 해당 차선 있는지 조사.
				if(!bExist) continue;

				if(nLoadType <= 2)
				{
					sText.Format(" %c. %d차선 재하\r\n", nLane+'a', nLane+1);
					Arr.Add(sText);
				}
				for(long nK = 0; nK <=  pDB->m_nQtyJigan; nK++)
				{															
					szTemp.Format("   < %s >\r\n", pDB->m_strJijumName[nK]);
					Arr.Add (szTemp);

					if(pDB->IsTUGir())
					{
						Arr.Add ("┏━━┳━━┳━━━━━━━━━━━━━━━━━━━━┳━━━━━┳━━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━━┓");
						Arr.Add ("┃지점┃절점┃             합성전 고정하중            ┃  합성후  ┃      활 하 중      ┃     지점 침하    ┃      합    계      ┃");
						Arr.Add ("┃위치┃번호┃ 강재자중 구속콘자중 180˚터닝   바닥판 ┃ 고정하중 ┃   Max        Min   ┃   Max       Min  ┃   Max        Min   ┃");
						Arr.Add ("┣━━╋━━╋━━━━━━━━━━━━━━━━━━━━╋━━━━━╋━━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━━┫");
					}
					else
					{
						Arr.Add ("┏━━┳━━┳━━━━━━━━━━┳━━━━━┳━━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━━┓");
						Arr.Add ("┃지점┃절점┃   합성전 고정하중  ┃  합성후  ┃      활 하 중      ┃     지점 침하    ┃      합    계      ┃");
						Arr.Add ("┃위치┃번호┃  Steel    Concrete ┃ 고정하중 ┃   Max        Min   ┃   Max       Min  ┃   Max        Min   ┃");
						Arr.Add ("┣━━╋━━╋━━━━━━━━━━╋━━━━━╋━━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━━┫");
					}

					for(int nJ = 0; nJ < pFEMManage->GetJointSize(); nJ++)
					{
						CJoint* pJ = pFEMManage->GetJoint(nJ);			
						CPlateBasicIndex	*pBx	= pCalcStd->GetBxByJoint(pJ);
						long	nJijum = pBx->GetNumberJijum();

						if(!(pJ->m_UZ))	continue;

						if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && nJijum == nK)
						{
							CJoint *pPreJ, *pPosJ, *pSetJ, *pConJ = NULL;
							CJointResult *pPreJResult, *pPosJResult, *pSetJResult, *pConJResult = NULL;
							pPreJ = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
							pPosJ = pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
							pSetJ = pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS)->GetJoint(pJ->m_nIdx);

							pPreJResult	= pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
							pPosJResult	= pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
							pSetJResult	= pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);

							if(pDB->IsTUGir())
							{
								pConJ		= pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
								pConJResult	= pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
							}

							szName.Format("G%d", pJ->m_LongAttrib[JOINT_GIR]+1);
							
							dDeadSteel	= Round(pPreJResult->m_vPreDeadSteel[nForce].z, 3);							
							if(pConJ)
							{
								dDeadConc = Round(pConJResult->m_vPreDeadConf[nForce].z, 3);
								dDeadTurn = Round(pConJResult->m_vPreDeadTurning[nForce].z, 3);
								dDeadSlab = Round(pConJResult->m_vPreDeadSlab[nForce].z,3);
							}
							else
							{
								dDeadConc = 0;
								dDeadTurn = 0;
								dDeadSlab = Round(pPreJResult->m_vPreDeadSlab[nForce].z, 3);
							}
							
							dPostDead = Round(pPosJResult->m_vPostDead[nForce].z,3);
							dLiveMax = Round(pModelCalc.GetLiveLoadReaction(nLoadType, nLoadGir, nLane, nJ, bExist),3);
							dLiveMin = Round(pModelCalc.GetLiveLoadReaction(-nLoadType, nLoadGir, nLane, nJ, bExist),3);

							szDeadSteel.Format  ("%8.3lf", dDeadSteel);
							szDeadConc.Format	("%8.3lf", dDeadConc);
							szDeadTurn.Format	("%8.3lf", dDeadTurn);
							szDeadSlab.Format   ("%8.3lf", dDeadSlab);
							szPostDead.Format   ("%8.3lf", dPostDead);
							szLiveMax.Format    ("%8.3lf", dLiveMax);
							szLiveMin.Format    ("%8.3lf", dLiveMin);

							if(pData->m_bCheckSettlement)
							{
								dUnSettleMax = Round(pSetJResult->m_vUnSettleMax[nForce].z,3);
								dUnSettleMin = Round(pSetJResult->m_vUnSettleMin[nForce].z,3);
							}
							else
							{
								dUnSettleMax = 0;
								dUnSettleMin = 0;
							}

							szUnSettleMax.Format("%8.3lf", dUnSettleMax);
							szUnSettleMin.Format("%8.3lf", dUnSettleMin);

							dTotalMax = Round(dDeadSteel+dDeadConc+dDeadTurn+dDeadSlab+dPostDead+dLiveMax+dUnSettleMax,3);
							dTotalMin = Round(dDeadSteel+dDeadConc+dDeadTurn+dDeadSlab+dPostDead+dLiveMin+dUnSettleMin,3);

							szTotalMax.Format   ("%8.3lf", dTotalMax);
							szTotalMin.Format   ("%8.3lf", dTotalMin);
						
							if(pDB->IsTUGir())
							{
								sprintf(sBuf, "┃ %s ┃%4d┃%s  %s   %s  %s ┃%s  ┃%s  %s  ┃%s %s ┃ %s  %s ┃", 
									szName, pJ->m_nIdx+1, szDeadSteel, szDeadConc, szDeadTurn, szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin);
							}
							else
							{
								sprintf(sBuf, "┃ %s ┃%4d┃%s  %s  ┃%s  ┃%s  %s  ┃%s %s ┃ %s  %s ┃", 
									szName, pJ->m_nIdx+1, szDeadSteel, szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin);
							}
							

							if(ABS(atof(szTotalMax)) > 0.1)
								Arr.Add(sBuf);
						}
					}

					if(pDB->IsTUGir())
						Arr.Add ("┗━━┻━━┻━━━━━━━━━━━━━━━━━━━━┻━━━━━┻━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━━┛\r\n\r\n");
					else
						Arr.Add ("┗━━┻━━┻━━━━━━━━━━┻━━━━━┻━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━━┛\r\n\r\n");
				}			

				if(nLoadType >= 3)  // 탱크와 트레일러는 차선이 없음.
					break;
			}
		}
	}

	sText.Format("%c. 최대 반력 집계\r\n", nTitle++);
	Arr.Add (sText);
	// 결과 Data Line 출력
	for(long nK = 0; nK <= pDB->m_nQtyJigan; nK++)
	{		
		szTemp.Format("   < %s >\r\n", pDB->m_strJijumName[nK]);
		Arr.Add (szTemp);
		if(pDB->IsTUGir())
		{
			Arr.Add ("┏━━┳━━┳━━━━━━━━━━━━━━━━━━━━┳━━━━━┳━━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━━┳━━━┓");
			Arr.Add ("┃지점┃절점┃             합성전 고정하중            ┃  합성후  ┃      활 하 중      ┃     지점 침하    ┃      합    계      ┃ 사용 ┃");
			Arr.Add ("┃위치┃번호┃ 강재자중 구속콘자중 180˚터닝   바닥판 ┃ 고정하중 ┃   Max        Min   ┃   Max       Min  ┃   Max        Min   ┃ Shoe ┃");
			Arr.Add ("┣━━╋━━╋━━━━━━━━━━━━━━━━━━━━╋━━━━━╋━━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━━╋━━━┫");
		}
		else
		{
			Arr.Add ("┏━━┳━━┳━━━━━━━━━━┳━━━━━┳━━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━━┳━━━┓");
			Arr.Add ("┃지점┃절점┃   합성전 고정하중  ┃  합성후  ┃      활 하 중      ┃     지점 침하    ┃      합    계      ┃ 사용 ┃");
			Arr.Add ("┃위치┃번호┃  Steel    Concrete ┃ 고정하중 ┃   Max        Min   ┃   Max       Min  ┃   Max        Min   ┃ Shoe ┃");
			Arr.Add ("┣━━╋━━╋━━━━━━━━━━╋━━━━━╋━━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━━╋━━━┫");
		}	

		for(int nJ = 0; nJ < pFEMManage->GetJointSize(); nJ++)
		{
			CJoint* pJ = pFEMManage->GetJoint(nJ);			
			CPlateBasicIndex	*pBx	= pCalcStd->GetBxByJoint(pJ);
			long	nJijum = pBx->GetNumberJijum();

			if(!(pJ->m_UZ))	continue;

			if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && nJijum == nK)
			{
				CPlateGirderApp *pGir = pDB->GetGirder(pJ->m_LongAttrib[JOINT_GIR]);
				CShoe* pShoe =  pGir->GetShoeByJijumNo(nJijum);
				CJoint	*pPreJ, *pPosJ, *pLivJ, *pSetJ, *pConJ = NULL;
				CJointResult *pPreJResult, *pPosJResult, *pLivJResult, *pSetJResult, *pConJResult = NULL;

				pPreJ = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
				pPosJ = pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
				pSetJ = pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS)->GetJoint(pJ->m_nIdx);
				pLivJ = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS)->GetJoint(pJ->m_nIdx);

				pPreJResult = pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				pPosJResult = pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				pSetJResult = pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				pLivJResult = pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);

				if(pDB->IsTUGir())
				{
					pConJ = pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS)->GetJoint(pJ->m_nIdx);
					pConJResult = pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx);
				}

				szName.Format("G%d", pJ->m_LongAttrib[JOINT_GIR]+1);
				dDeadSteel	= Round(pPreJResult->m_vPreDeadSteel[nForce].z, 3);							
				dDeadConc	= 0;
				dDeadTurn	= 0;
				dDeadSlab	= 0;
				if(pConJ)
				{
					dDeadConc = Round(pConJResult->m_vPreDeadConf[nForce].z, 3);
					dDeadTurn = Round(pConJResult->m_vPreDeadTurning[nForce].z, 3);
					dDeadSlab = Round(pConJResult->m_vPreDeadSlab[nForce].z,3);
				}
				else
					dDeadSlab	= Round(pPreJResult->m_vPreDeadSlab[nForce].z, 3);
				
				dPostDead = Round(pPosJResult->m_vPostDead[nForce].z,3);
				dLiveMax = Round(pLivJResult->m_vLiveMax[nForce].z,3);
				dLiveMin = Round(pLivJResult->m_vLiveMin[nForce].z,3);

				szDeadSteel.Format  ("%8.3lf", dDeadSteel);
				szDeadConc.Format	("%8.3lf", dDeadConc);
				szDeadTurn.Format	("%8.3lf", dDeadTurn);
				szDeadSlab.Format   ("%8.3lf", dDeadSlab);
				szPostDead.Format   ("%8.3lf", dPostDead);
				szLiveMax.Format    ("%8.3lf", dLiveMax);
				szLiveMin.Format    ("%8.3lf", dLiveMin);
				
				if(pData->m_bCheckSettlement)
				{
					dUnSettleMax = Round(pSetJResult->m_vUnSettleMax[nForce].z,3);
					dUnSettleMin = Round(pSetJResult->m_vUnSettleMin[nForce].z,3);
				}
				else
				{
					dUnSettleMax = 0;
					dUnSettleMin = 0;
				}

				szUnSettleMax.Format("%8.3lf", dUnSettleMax);
				szUnSettleMin.Format("%8.3lf", dUnSettleMin);

				dTotalMax = Round(dDeadSteel+dDeadConc+dDeadTurn+dDeadSlab+dPostDead+dLiveMax+dUnSettleMax,3);
				dTotalMin = Round(dDeadSteel+dDeadConc+dDeadTurn+dDeadSlab+dPostDead+dLiveMin+dUnSettleMin,3);

				szTotalMax.Format   ("%8.3lf", dTotalMax);
				szTotalMin.Format   ("%8.3lf", dTotalMin);				
				szShoeForce.Format  ("%5d", pShoe->m_dForceShoe*10);
				
				if(pDB->IsTUGir())
				{
					sprintf(sBuf, "┃ %s ┃%4d┃%s  %s   %s  %s ┃%s  ┃%s  %s  ┃%s %s ┃ %s  %s ┃%s ┃", 
						szName, pJ->m_nIdx+1, szDeadSteel, szDeadConc, szDeadTurn, szDeadSlab, szPostDead,
						szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szShoeForce);
				}
				else
				{
					sprintf(sBuf, "┃ %s ┃%4d┃%s  %s  ┃%s  ┃%s  %s  ┃%s %s ┃ %s  %s ┃%s ┃", 
						szName, pJ->m_nIdx+1, szDeadSteel, szDeadSlab, szPostDead,
						szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szShoeForce);
				}

				if(ABS(atof(szTotalMax)) > 0.1)
					Arr.Add(sBuf);
			}
		}
		if(pDB->IsTUGir())
			Arr.Add ("┗━━┻━━┻━━━━━━━━━━━━━━━━━━━━┻━━━━━┻━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━━┻━━━┛\r\n\r\n");
		else
			Arr.Add ("┗━━┻━━┻━━━━━━━━━━┻━━━━━┻━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━━┻━━━┛\r\n\r\n");
	}			
}

void CPlateStressOut::NodeDisplaceOutput(CStringArray &Arr)
{
	CAPlateCalcStd *pCalcStd = m_pStd->m_pCalcStd;
	CCalcData      *pData    = m_pStd->GetDataManage()->GetCalcData();
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CPlateBridgeApp *pDB     = (CPlateBridgeApp*)m_pStd->GetBridge();
	CFEMManage*   pFEMManage = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	
	char sBuf [200];

	Arr.Add ("  << 처짐량집계 >>\r\n");
	// 결과 Data Line 출력
	double dLengthJigan = 0;
	double dDroop       = 0;
	long nForce = JOINT_DISPLACEMENT;
	long nStt = 0; 
	long nEnd = 0;
	CString szDeadSteel, szDeadSlab, szPostDead, szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szAllowDroop, szName, szTemp, szCheck;

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		szTemp.Format("   < 거더%d >\r\n", nG+1);
		Arr.Add (szTemp);
		Arr.Add ("┏━━┳━━┳━━━━━━━━━━┳━━━━━┳━━━━━━━━━━┳━━━━━━━━━┳━━━━━━━━━━┳━━━━┳━━┓");
		Arr.Add ("┃지점┃절점┃   합성전 고정하중  ┃  합성후  ┃      활 하 중      ┃     지점 침하    ┃      합    계      ┃ 허  용 ┃비고┃");
		Arr.Add ("┃위치┃번호┃  Steel    Concrete ┃ 고정하중 ┃   Max        Min   ┃   Max       Min  ┃   Max        Min   ┃ 처  짐 ┃    ┃");
		Arr.Add ("┣━━╋━━╋━━━━━━━━━━╋━━━━━╋━━━━━━━━━━╋━━━━━━━━━╋━━━━━━━━━━╋━━━━╋━━┫");
		ModelCalc.GetJointRangeByGirder(nG, nStt, nEnd);

		for (int i = nStt; i < nEnd+1; i++)  
		{
			CJoint* pJ = pFEMManage->GetJoint(i);
			ModelCalc.GetJijumNameOfJoint(i, szName);
						
			szDeadSteel.Format  ("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPreDeadSteel[nForce].z);
			szDeadSlab.Format   ("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPreDeadSlab[nForce].z);
			szPostDead.Format   ("%8.3lf", pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPostDead[nForce].z);
			szLiveMax.Format    ("%8.3lf", pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vLiveMax[nForce].z);
			szLiveMin.Format    ("%8.3lf", pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vLiveMin[nForce].z);
			if(pData->m_bCheckSettlement)
			{
				szUnSettleMax.Format("%8.3lf", pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vUnSettleMax[nForce].z);
				szUnSettleMin.Format("%8.3lf", pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vUnSettleMin[nForce].z);
			}
			else
			{
				szUnSettleMax.Format("%8.3lf", 0);
				szUnSettleMin.Format("%8.3lf", 0);
			}
			szTotalMax.Format   ("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMax[nForce].z);
			szTotalMin.Format   ("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMin[nForce].z);
			long nJigan = ModelCalc.GetJiganNoByJointID(i);
			dLengthJigan = toM (pDB->m_dLengthJigan[nJigan]);   // 지간 거리.
			if(dLengthJigan > 40)   	dDroop = dLengthJigan / 500;
			else if(dLengthJigan > 10)  dDroop = dLengthJigan * dLengthJigan / 20000;
			else						dDroop = dLengthJigan / 2000;

			szAllowDroop.Format ("%8.3lf", dDroop);
			if(dDroop>fabs(pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMin[nForce].z))
				szCheck = "O.K.";
			else
				szCheck = "N.G.";
			
			sprintf(sBuf, "┃ %s ┃%4d┃%s  %s  ┃%s  ┃%s  %s  ┃%s %s ┃ %s  %s ┃%s┃%s┃ ", 
					szName, pJ->m_nIdx+1, 
					szDeadSteel, szDeadSlab, szPostDead, szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szAllowDroop, szCheck);

			Arr.Add(sBuf);
		}
		Arr.Add ("┗━━┻━━┻━━━━━━━━━━┻━━━━━┻━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━━━━┻━━━━┻━━┛\r\n\r\n");
	}

}

void CPlateStressOut::NodeCamberOutput(CStringArray &Arr)
{
	CAPlateCalcStd  *pCalcStd   = m_pStd->m_pCalcStd;
	CDataManage		*pDataManage = m_pStd->GetDataManage();
	CCalcData       *pData      = pDataManage->GetCalcData();
	CPlateBridgeApp *pDB        = (CPlateBridgeApp*)m_pStd->GetBridge();
	CFEMManage      *pFEMManage = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFEMManage		*pFEMManageTu = pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);
	CFemStressCalc	StressCalc(pDataManage, pCalcStd);
	CPlateGirderApp	*pGir	    = NULL;
	CJoint			*pJ			= NULL;

	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	

	CString szText = _T("");
	int nCurGir = -1, nCamberIdx = 0;
	long nUnit		= pDB->m_cCamberUnitToMM;

	Arr.Add ("  << 솟 음 (CAMBER) >>\r\n");

	for (int i = 0; i < pFEMManage->GetJointSize(); i++)   
	{
		pJ = pFEMManage->GetJoint(i);
		//if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE))	
		//	continue;

		if (pFEMManage->GetJoint(i)->m_LongAttrib[JOINT_GIR] != nCurGir)   // Girder가 바뀌면
		{  
			// Header 출력
			nCurGir = pFEMManage->GetJoint(i)->m_LongAttrib[JOINT_GIR];
			pGir  = pDB->GetGirder(nCurGir);
			nCamberIdx = 0;
			szText.Format(" < Girder %d >", nCurGir+1);
			Arr.Add (szText);

			if(pDB->IsTUGir())
			{
				Arr.Add ("┏━━━┳━━━━━━━━━━━━━━━━━━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━┓");
				Arr.Add ("┃ 절점 ┃             합성전 고정하중            ┃  합성후  ┃  종곡선  ┃  편경사  ┃  솟음량  ┃  비고  ┃");
				Arr.Add ("┃ 번호 ┃ 강재자중 구속콘자중 180˚터닝   바닥판 ┃ 고정하중 ┃  보정량  ┃  보정량  ┃          ┃        ┃");
				Arr.Add ("┣━━━╋━━━━━━━━━━━━━━━━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━┫");
			}
			else
			{
				Arr.Add ("┏━━━┳━━━━━━━━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━━┳━━━━┓");
				Arr.Add ("┃ 절점 ┃   합성전 고정하중  ┃  합성후  ┃  종곡선  ┃  편경사  ┃  솟음량  ┃  비고  ┃");
				Arr.Add ("┃ 번호 ┃  Steel    Concrete ┃ 고정하중 ┃  보정량  ┃  보정량  ┃          ┃        ┃");
				Arr.Add ("┣━━━╋━━━━━━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━━╋━━━━┫");
			}
		}

		double dSta = pJ->m_vPointReal.x;
		nCamberIdx = pGir->GetCamberNodeByStation(dSta);
		CCamber	*pCamber	= pGir->GetCamberByNode(nCamberIdx);

		double dJong		= pCamber->GetCamberByType(pDB->GetBridgeType(), 0);	// 종곡선 보정량
		double dSlop		= pCamber->GetCamberByType(pDB->GetBridgeType(), 1);	// 편경사 보정량
		double dPreSteel	= pCamber->GetCamberByType(pDB->GetBridgeType(), 2);
		double dPreConf		= pCamber->GetCamberByType(pDB->GetBridgeType(), 3);
		double dPreTurn		= 0;
		double dPreSlab		= 0;
		double dPos			= pCamber->GetCamberByType(pDB->GetBridgeType(), 4);
		double dCamber		= dPreSteel+dPreConf+dPos;

		if(pDB->IsTUGir())
		{
			dPreTurn = pCamber->GetCamberByType(pDB->GetBridgeType(), 4);
			dPreSlab = pCamber->GetCamberByType(pDB->GetBridgeType(), 5);
			dPos	 = pCamber->GetCamberByType(pDB->GetBridgeType(), 6);

			//Issue Number: 26311
			if(pDB->m_nQtyJigan > 1)
			{
				long nCaseSteelIdx	 = pFEMManage->GetLoadCaseIndex("CONSTEEL");
				long nCaseSteelTUIdx = pFEMManageTu->GetLoadCaseIndex("CONSTEEL");
				long nCaseConcTUIdx	 = pFEMManageTu->GetLoadCaseIndex("CONCRETE");

				double dConSteel	= frM(StressCalc.GetOneNodForceSub(pFEMManage,		NOD_DISPLACE,	nCaseSteelIdx,		i, 'Z'));
				double dConSteelTU	= frM(StressCalc.GetOneNodForceSub(pFEMManageTu,	NOD_DISPLACE,	nCaseSteelTUIdx,	i, 'Z'));
				double dConcTU		= frM(StressCalc.GetOneNodForceSub(pFEMManageTu,	NOD_DISPLACE,	nCaseConcTUIdx,		i, 'Z'));

				dPreConf = dConSteel;
				dPreSlab = fabs(dConSteelTU + dConcTU);
			}
			dCamber	 = dPreSteel+dPreConf+dPreTurn+dPreSlab+dPos;
		}

		if(nUnit != 1)
		{
			dJong		= toCm(dJong);
			dSlop		= toCm(dSlop);
			dPreSteel	= toCm(dPreSteel);
			dPreConf	= toCm(dPreConf);
			dPreTurn	= toCm(dPreTurn);
			dPreSlab	= toCm(dPreSlab);
			dPos		= toCm(dPos);
			dCamber		= toCm(dCamber);
		}

		CString szNode, szJong, szSlop, szPreSteel, szPreConf, szPreTurn, szPreSlab, szPos, szCamber, szJijum, szTemp;
		szNode.Format("%5d", i+1);
		szJong.Format		("%8.3lf", dJong);
		szSlop.Format		("%8.3lf", dSlop);
		szPreSteel.Format	("%8.3lf", dPreSteel);
		szPreConf.Format	("%8.3lf", dPreConf);
		szPreTurn.Format	("%8.3lf", dPreTurn);
		szPreSlab.Format    ("%8.3lf", dPreSlab);
		szPos.Format		("%8.3lf", dPos);
		szCamber.Format		("%8.3lf", dCamber);
		ModelCalc.GetJijumNameOfJoint(i, szJijum);

		if(pDB->IsTUGir())
		{
			szTemp.Format("┃%s ┃ %s  %s  %s  %s ┃ %s ┃ %s ┃ %s ┃ %s ┃   %s   ┃",
				szNode, szPreSteel, szPreConf, szPreTurn, szPreSlab, szPos, szJong, szSlop, szCamber, szJijum);
		}
		else
		{
			szTemp.Format("┃%s ┃ %s  %s ┃ %s ┃ %s ┃ %s ┃ %s ┃   %s   ┃",
				szNode, szPreSteel, szPreConf, szPos, szJong, szSlop, szCamber, szJijum);
		}
		Arr.Add(szTemp);

		nCamberIdx++;
		
		long nSttJoint=0;
		long nEndJoint=0;
		ModelCalc.GetJointRangeByGirder(nCurGir, nSttJoint, nEndJoint);

		if (nEndJoint == i)	// Footer 출력
		{
			if(pDB->IsTUGir())
				Arr.Add ("┗━━━┻━━━━━━━━━━━━━━━━━━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━┛\r\n");
			else
				Arr.Add ("┗━━━┻━━━━━━━━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━━┻━━━━┛\r\n");
		}
	}

}


void CPlateStressOut::SlabSectionOutput(CStringArray &Arr)
{
	SlabSectionOutputLeft(Arr);
	SlabSectionOutputRight(Arr);
	SlabSectionOutputCenter(Arr);
}

void CPlateStressOut::SlabSectionPrestress(long nSide, CShRichEdit &RichEdit)
{
	CPlateBridgeApp		*pDB			= m_pStd->GetBridge();
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pStd->m_pCalcStd;

	CCalcFloor			CalcFloor(pDataManage);
	CMap <CString, LPCTSTR, double, double> Map;
	CalcFloor.CalcPreStressLoss(pCalcStd, Map, nSide);


	// 단면제원
	double	dHeight[3]	= { Map["Height_Left"], Map["Height_Middle"], Map["Height_Right"] };
	double	dArea[3]	= { Map["Area_Left"], Map["Area_Middle"], Map["Area_Right"] };
	double	dYb[3]		= { Map["Yb_Left"], Map["Yb_Middle"], Map["Yb_Right"] };
	double	dIx[3]		= { Map["I_Left"], Map["I_Middle"], Map["I_Right"] };
	double	dZb[3]		= { Map["Zb_Left"], Map["Zb_Middle"], Map["Zb_Right"] };
	double	dEp[3]		= { Map["Ep_Left"], Map["Ep_Middle"], Map["Ep_Right"] };

	CString szText(_T(""));
	CString szSub(_T(""));
	CString szSign(_T(""));
	CString szOk(_T(""));

	szText.Format("    < 단면제원 >\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);

	for (long n=0; n<3; n++)
	{
		szSub = (n==0)? _T("    - 좌측 캔틸레버부 : ") : (n==1)? _T("    - 중       앙      부 : ") : _T("    - 우측 캔틸레버부 : ");
		szText.Format("%s ㆍ높이H = %.0f mm, ㆍA = %.0f mm², ㆍyb = %.0f mm, ㆍI = %.0f mm⁴, Zb = %.0f mm³, ep = %.0f mm \n", 
			szSub, dHeight[n], dArea[n], dYb[n], dIx[n], dZb[n], dEp[n]);
		RichEdit.AddText(szText);
	}
	RichEdit.AddText("\n");

	// PS강재제원
	double dTendon_Dia = Map["Tendon_Dia"];
	double dTendon_Area= Map["Tendon_Area"];
	double dTendon_Qty = Map["Tendon_Qty"];

	szText.Format("    < PS강재 제원 >\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);

	szText = _T("    ㆍ사용 긴장재\n");
	RichEdit.AddText(szText);
	szText.Format("      φ %.2f mm   x   %.0f   =   %.2f   x   %.0f   =   %.2f mm²\n\n", 
			dTendon_Dia, dTendon_Qty, dTendon_Area, dTendon_Qty, dTendon_Area*dTendon_Qty );
	RichEdit.AddText(szText);

	// 부위별 응력검토
	// 프리스트레싱 직후 긴장재 응력 및 허용응력 검토
	double dStress_Fj = Map["Stress_Fj"];
	double dLoss_Fric = Map["Loss_Fric"];
	double dLoss_Act  = Map["Loss_Act"];
	double dLoss_Elec = Map["Loss_E"];
	double dLoss_Total= Map["Loss_Total"];
	double dStress_Total = Map["Stress_Total"];

	szText = (nSide==0)? _T("    좌측 캔틸레버부") : (nSide==1)? _T("    우측 캔틸레버부") : _T("    슬래브 중앙부");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText = (nSide==0 && pDB->m_nTendonFix<1)? _T(" (고정단)") : (nSide==0 && pDB->m_nTendonFix==1)?  _T(" (긴장단)") : _T(""); 
	szText += (nSide==1 && pDB->m_nTendonFix<1)? _T(" (긴장단)") : (nSide==1 && pDB->m_nTendonFix==1)?  _T(" (고정단)") : _T("");
	szText += _T("\n\n");
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	szText = _T("    ① 프리스트레싱 직후 긴장재 응력 및 허용응력 검토   ( ☞ 도.설.해 4.6.3.3 )		(단위 : MPa)\n");
	RichEdit.AddText(szText);
	szText.Format("     ㆍ긴장응력 (fj) : %.3f ㆍ마찰손실(ΔfpF) : %.3f ㆍ활동손실(Δfan) : %.3f ㆍ탄성손실(ΔfpEL) : %.3f ㆍ총손실량(Δfpt) : %.3f ㆍ긴장재응력(fpt) : %.3f \n\n",
					dStress_Fj, dLoss_Fric, dLoss_Act, dLoss_Elec, dLoss_Total, dStress_Total);
	RichEdit.AddText(szText);

	// 허용응력검토-1
	double dStrength_Ultimate	= Map["Str_Ulimate"];
	double dStrength_Yield		= Map["Str_Yield"];
	double dFactored_Ultimate	= Map["Fact_Ulimate"];
	double dFactored_Yield		= Map["Fact_Yield"];

	szText = "     - 허용응력검토\n";
	RichEdit.AddText(szText);
	
	if (dStress_Total<=dFactored_Ultimate)	{ szSign=_T("≤"); szOk=_T("∴ O.K"); }
	if (dStress_Total<dFactored_Ultimate)	{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (dStress_Total>=dFactored_Ultimate)	{ szSign=_T("≥"); szOk=_T("∴ N.G"); }
	if (dStress_Total>dFactored_Ultimate)	{ szSign=_T("＞"); szOk=_T("∴ N.G"); }

	szText = "     ㆍfpt    =    ";
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dStress_Total);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    0.70 fpu    =    0.70 * %.2f    =    ", szSign, dStrength_Ultimate);
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dFactored_Ultimate);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	if (dStress_Total<=dFactored_Yield)	{ szSign=_T("≤"); szOk=_T("∴ O.K"); }
	if (dStress_Total<dFactored_Yield)	{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (dStress_Total>=dFactored_Yield)	{ szSign=_T("≥"); szOk=_T("∴ N.G"); }
	if (dStress_Total>dFactored_Yield)	{ szSign=_T("＞"); szOk=_T("∴ N.G"); }

	szText = "     ㆍfpt    =    ";
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dStress_Total);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    0.83 fy      =    0.83 * %.2f    =    ", szSign, dStrength_Yield);
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dFactored_Yield);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s\n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 시간에 따른 총 손실을 고려한 긴장재 응력 산정 및 허용응력 검토
	double dTime_FJ = Map["Time_FJ"];	// 긴장재응력
	double dTime_Sh = Map["Time_Sh"];	// 건조수축
	double dTime_Cr = Map["Time_Cr"];	// 크리프
	double dTime_Re = Map["Time_Re"];	// 릴렉세이션
	double dTime_To = Map["Time_To"];	// 총손실량
	double dTime_EFJ = Map["Time_EFJ"];	// 유효긴장재응력


	szText = _T("    ② 시간에 따른 총 손실을 고려한 긴장재 응력 산정 및 허용응력 검토   ( ☞ 도.설 4.6.3.3 )	\n");
	RichEdit.AddText(szText);
	szText.Format("     ㆍ긴장재응력 (fpt) : %.3f ㆍ건조수축(ΔfpSH) : %.3f ㆍ크리프손실(ΔfpCR) : %.3f ㆍ릴랙세이션(ΔfpR) : %.3f ㆍ총손실량(Δfpe) : %.3f ㆍ유효긴장재응력(fpe) : %.3f \n\n",
		dTime_FJ, dTime_Sh, dTime_Cr, dTime_Re, dTime_To, dTime_EFJ);
	RichEdit.AddText(szText);
	
	// 허용응력검토-2
	double dStrength_Yield2		= Map["Str_Yield2"];
	double dFactored_Yield2		= Map["Fact_Yield2"];

	szText = "     - 허용응력검토\n";
	RichEdit.AddText(szText);

	if (dTime_EFJ<=dFactored_Yield2)	{ szSign=_T("≤"); szOk=_T("∴ O.K"); }
	if (dTime_EFJ<dFactored_Yield2)		{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (dTime_EFJ>=dFactored_Yield2)	{ szSign=_T("≥"); szOk=_T("∴ N.G"); }
	if (dTime_EFJ>dFactored_Yield2)		{ szSign=_T("＞"); szOk=_T("∴ N.G"); }

	szText = "     ㆍfpe    =    ";
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dTime_EFJ);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    0.80 fy      =    0.80 * %.2f    =    ", szSign, dStrength_Yield2);
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dFactored_Yield2);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s\n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
}

void CPlateStressOut::SlabPrestresscheck(long nSide, CShRichEdit &RichEdit)
{
	CPlateBridgeApp		*pDB			= m_pStd->GetBridge();
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pStd->m_pCalcStd;
	CCalcData			*pData			= pDataManage->GetCalcData();

	CString szText(_T(""));
	CString szSign(_T(""));
	CString szOk(_T(""));
	CString szStress(_T(""));
	CString szStr1(_T(""));
	CString szStr2(_T(""));

	CCalcFloor			CalcFloor(pDataManage);
	CMap <CString, LPCTSTR, double, double> Map;
	CalcFloor.CalcPreStressCheck(pCalcStd, Map, nSide);

	// 0028262  이슈에 의해 절대값을 취하지 않음 - 검토 기호 비교시에만 절대값 사용.
	double dForced_Top_Before =	Map["Force_Top_Before"];	// fabs(Map["Force_Top_Before"]);
	double dForced_Top_After  =	Map["Force_Top_After"];		// fabs(Map["Force_Top_After"]);
	double dForced_Bot_Before =	Map["Force_Bot_Before"];	// fabs(Map["Force_Bot_Before"]);
	double dForced_Bot_After  =	Map["Force_Bot_After"];		// fabs(Map["Force_Bot_After"]);

	double dAllow_Top_Before  =	Map["Allow_Top_Before"];	// fabs(Map["Allow_Top_Before"]);
	double dAllow_Top_After   =	Map["Allow_Top_After"];		// fabs(Map["Allow_Top_After"]);
	double dAllow_Bot_Before  =	Map["Allow_Bot_Before"];	// fabs(Map["Allow_Bot_Before"]);
	double dAllow_Bot_After   =	Map["Allow_Bot_After"];		// fabs(Map["Allow_Bot_After"]);

	// 위치
	szText = (nSide==0)? _T("    좌측 캔틸레버부") : (nSide==1)? _T("    우측 캔틸레버부") : _T("    슬래브 중앙부");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText = (nSide==0 && pDB->m_nTendonFix<1)? _T(" (고정단)") : (nSide==0 && pDB->m_nTendonFix==1)?  _T(" (긴장단)") : _T(""); 
	szText += (nSide==1 && pDB->m_nTendonFix<1)? _T(" (긴장단)") : (nSide==1 && pDB->m_nTendonFix==1)?  _T(" (고정단)") : _T("");
	szText += _T("\n\n");
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ① 프리스트레싱 도입 직후의 응력 검토								
	szText = _T("    ① 프리스트레싱 도입 직후의 응력 검토\n\n");
	RichEdit.AddText(szText);

	// 상연
	if (fabs(dForced_Top_Before) <= fabs(dAllow_Top_Before))	{ szSign=_T("≤"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Top_Before) < fabs(dAllow_Top_Before))		{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Top_Before) >= fabs(dAllow_Top_Before))	{ szSign=_T("≥"); szOk=_T("∴ N.G"); }
	if (fabs(dForced_Top_Before) > fabs(dAllow_Top_Before))		{ szSign=_T("＞"); szOk=_T("∴ N.G"); }
	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow;
	szStress =  (dForced_Top_Before < 0) ? szStr2 : szStr1;

	szText = _T("     - 상연 허용 응력 검토\n");
	RichEdit.AddText(szText);
	szText = _T("        fc,top    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dForced_Top_Before);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    %s    =    ", szSign, szStress);
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dAllow_Top_Before);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	
	// 하연
	if (fabs(dForced_Bot_Before) <= fabs(dAllow_Bot_Before))	{ szSign=_T("≤"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Bot_Before) < fabs(dAllow_Bot_Before))		{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Bot_Before) >= fabs(dAllow_Bot_Before))	{ szSign=_T("≥"); szOk=_T("∴ N.G"); }
	if (fabs(dForced_Bot_Before) > fabs(dAllow_Bot_Before))		{ szSign=_T("＞"); szOk=_T("∴ N.G"); }
	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow;
	szStress =  (dForced_Bot_Before < 0) ? szStr2 : szStr1;

	szText = _T("     - 하연 허용 응력 검토\n");
	RichEdit.AddText(szText);
	szText = _T("        fc,bot    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dForced_Bot_Before);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    %s    =    ", szSign, szStress);
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dAllow_Bot_Before);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ② 설계하중 작용시의 응력 검토						
	szText = _T("    ② 설계하중 작용시의 응력 검토\n\n");
	RichEdit.AddText(szText);

	// 상연
	if (fabs(dForced_Top_After) <= fabs(dAllow_Top_After))	{ szSign=_T("≤"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Top_After) < fabs(dAllow_Top_After))		{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Top_After) >= fabs(dAllow_Top_After))	{ szSign=_T("≥"); szOk=_T("∴ N.G"); }
	if (fabs(dForced_Top_After) > fabs(dAllow_Top_After))		{ szSign=_T("＞"); szOk=_T("∴ N.G"); }

	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow;
	szStress =  (dForced_Top_After < 0) ?  szStr2 : szStr1;
	
	szText = _T("     - 상연 허용 응력 검토\n");
	RichEdit.AddText(szText);
	szText = _T("        fc,top    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dForced_Top_After);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    %s    =    ", szSign, szStress);
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dAllow_Top_After);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 하연
	if (fabs(dForced_Bot_After) <= fabs(dAllow_Bot_After))		{ szSign=_T("≤"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Bot_After) < fabs(dAllow_Bot_After))		{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (fabs(dForced_Bot_After) >= fabs(dAllow_Bot_After))		{ szSign=_T("≥"); szOk=_T("∴ N.G"); }
	if (fabs(dForced_Bot_After) > fabs(dAllow_Bot_After))		{ szSign=_T("＞"); szOk=_T("∴ N.G"); }

	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow;
	szStress =  (dForced_Bot_After < 0) ? szStr2 :szStr1;
	
	szText = _T("     - 하연 허용 응력 검토\n");
	RichEdit.AddText(szText);
	szText = _T("        fc,bot    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dForced_Bot_After);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    %s    =    ", szSign, szStress);
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa    ", dAllow_Bot_After);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
}

void CPlateStressOut::SlabRebarOutput(CStringArray &Arr, long nPos)
{
//	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
//	Output_RebarQtyCheck(Arr, nPos);
}

void CPlateStressOut::SlabSectionOutputLeft(CStringArray &Arr)
{

	CString newElement(_T(""));	

	newElement.Format("3. 바닥판 설계");
	Arr.Add(newElement+"\r\n");
	newElement.Format(" 3.1 켄틸레버부");
	Arr.Add(newElement+"\r\n");
	newElement.Format("  1) 좌측켄틸레버부");
	Arr.Add(newElement+"\r\n");

	Output_DeadLoad(Arr, 0);				//사하중
//	Output_MoveLoad(Arr, TRUE);				//활하중
//	Output_EtcLoad(Arr, TRUE);				//기타하중	
//	Output_TotalCombinationLoad(Arr, 0);	//하중조합
//	Output_SlabThickness(Arr, 0);		//슬래브 최소두께검토
}

void CPlateStressOut::SlabSectionOutputRight(CStringArray &Arr)
{

	CString newElement(_T(""));	

	newElement.Format("  2) 우측켄틸레버부");
	Arr.Add(newElement+"\r\n");

	Output_DeadLoad(Arr, 1);				//사하중
//	Output_MoveLoad(Arr, FALSE);			//활하중
//	Output_EtcLoad(Arr, FALSE);				//기타하중	
//	Output_TotalCombinationLoad(Arr, 1);	//하중조합
//	Output_SlabThickness(Arr, 1);		//슬래브 최소두께검토
}

void CPlateStressOut::SlabSectionOutputCenter(CStringArray &Arr)
{
//	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();
//	CPlateBridgeApp *pDB   = m_pStd->GetBridge();

	CString newElement(_T(""));	

	newElement.Format("  3) 슬래브 중앙부");
	Arr.Add(newElement+"\r\n");

	Output_DeadLoad(Arr, 2);			    //사하중
//	Output_MoveLoadCenter(Arr);		    	//활하중
//	if(pDB->IsStraightLineTotalBridge())
//		OutPut_CentriodLoad(Arr);				//원심하중
//	Output_TotalCombinationLoad(Arr, 2);	//하중조합
//	Output_SlabThickness(Arr, 2);		//슬래브 최소두께검토
}

void CPlateStressOut::SlabSectionOutputReview(CStringArray &Arr)
{
	CString newElement(_T(""));		

//	Output_DroopReview(Arr);			//처짐검토

	newElement.Format("    <<균열 및 균열폭 검토>>         (콘크리트 시방서   P139)");
	Arr.Add(newElement+"\r\n");	 
//	Output_CrackReview(Arr, 0);			//좌측켄틸레버부 균열검토
//	Output_CrackReview(Arr, 1);			//우측켄틸레버부 균열검토
//	Output_CrackReview(Arr, 2);			//중앙부 균열검토
//	Output_CrackReview(Arr, 3);			//외측가로보 중앙부 - 좌측
//	Output_CrackReview(Arr, 4);			//외측가로보 중앙부 - 우측
//	Output_GiJumReview(Arr);			//지점부바닥판 검토
}


void CPlateStressOut::Output_DeadLoad(CStringArray& Arr,  long nPos) 
{
//	CCalcData       *pData  = m_pStd->GetDataManage()->GetCalcData();
//	CPlateBridgeApp *pDB    = m_pStd->GetBridge();
	CCalcFloor     *pFCalc = m_pStd->m_pCalcStd->m_pCalcFloor;	
	CCalcGeneral        CalcGeneral(m_pStd->GetDataManage());

	CStringArray ArrSrc;	
	if(nPos==0)		 pFCalc->CalcSlabLeftDeadLoad(ArrSrc);
	else if(nPos==1) pFCalc->CalcSlabRightDeadLoad(ArrSrc);	
	else if(nPos==2) pFCalc->CalcSlabCenterDeadLoad(ArrSrc);
	
	long nArrsu = ArrSrc.GetSize();
	long nRow   = (int)((nArrsu-2)/4);

	CString newElement(_T(""));	
	long index  = 0;
	newElement.Format("    <<사하중>>");
	Arr.Add(newElement+"\r\n");
	if(nPos==2)
	{
		newElement.Format("  ┏━━┳━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━┳━━━━━━━┓");
		Arr.Add(newElement);
		newElement.Format("  ┃ NO ┃         산                   식              ┃ 자중(tonf) ┃모멘트(tonf.m)┃");
		Arr.Add(newElement);
		newElement.Format("  ┣━━╋━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━╋━━━━━━━┫");
		Arr.Add(newElement);
	}
	else
	{
		newElement.Format("  ┏━━┳━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━┳━━━━━━┳━━━━━━━┓");
		Arr.Add(newElement);
		newElement.Format("  ┃ NO ┃         산                   식                ┃ 자중(tonf) ┃   거리(m)  ┃모멘트(tonf.m)┃");
		Arr.Add(newElement);
		newElement.Format("  ┣━━╋━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━━┫");
		Arr.Add(newElement);
	}

//	if(pData->DESIGN_FLOOR_DATA[nPos].m_bBangEm && pData->DESIGN_FLOOR_DATA[nPos].m_dBangEmWeight != 0)  nRow++;

	for( long i = 0; i < nRow; i++ )
	{
		CString str1 = ArrSrc.GetAt(index++);
		CString str2 = ArrSrc.GetAt(index++);
		CString str3 = ArrSrc.GetAt(index++);
		CString str4 = ArrSrc.GetAt(index++);
	
		if(nPos==2)
			newElement.Format("  ┃  %2d┃%-44s  ┃ %-11s┃%-13s ┃", i+1, str1, str2, str4 );
		else
			newElement.Format("  ┃  %2d┃ %-44s   ┃   %s   ┃   %s   ┃    %s  ┃", i+1, str1, str2, str3, str4 );

		Arr.Add(newElement);
	}

	if(nPos==2)
	{
		newElement.Format("  ┗━━┻━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━┻━━━━━━━┛");
		Arr.Add(newElement+"\r\n\r\n\r\n");
	}
	else
	{
		CString str1 = ArrSrc.GetAt(index++);
		CString str2 = ArrSrc.GetAt(index++);

		newElement.Format("  ┣━━╋━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━━┫");
		Arr.Add(newElement);
		newElement.Format("  ┃ 계 ┃                                                ┃   %5s   ┃            ┃   %8s  ┃", str1, str2 );
		Arr.Add(newElement);
		newElement.Format("  ┗━━┻━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━━┛");
		Arr.Add(newElement+"\r\n\r\n\r\n");
	}
}

// 활하중
void CPlateStressOut::Output_MoveLoad(BOOL bLeft, CShRichEdit &RichEdit) 
{
	CCalcData			*pCalcData		= m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CCalcGeneral		CalcGeneral(m_pStd->GetDataManage());
	CPlateBxFinder		Finder(pDB->GetGirder(0));
	CPlateBasicIndex	*pBx		= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	if(!bLeft)			pGir		= pDB->GetGirder(pDB->GetGirdersu()-1);
	CCalcFloor			CalcFloor(m_pStd->GetDataManage());

	long	nIndex		= bLeft ? FLOOR_LEFT : FLOOR_RIGHT;
	double	dPr			= pCalcData->DESIGN_CONDITION.m_dPr;
	double	dMl			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml);
	double	dii			= 0;
	double	dL			= 0;
	double	diT			= 0.15;
	CStringArray arr;
	CString sText	= _T("");
	CString sign	= _T("");
	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nIndex].m_bWalkLoad;

	if(bWalkLoad)	sText.Format("    (2) 활 하 중  ( 군중하중 : 5 kN/m²)	(☞도.설.기 2.1.3)\n\n");
	else			sText.Format("    (2) 활 하 중  ( DB-24 : Pr = %.1f kN )	(☞도.설.기 2.1.3)\n\n", tokN(dPr));
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	if(pCalcData->DESIGN_FLOOR_DATA[nIndex].m_bWalkLoad)
	{
		dL	= toM(CalcGeneral.GetWidthBodoDeadLoad(bLeft, pBx));
		if(dL > 0)
		{
			sText.Format("     Ml = 5 kN/㎡ × L²/ 2 = %.3f × %.3f²/ 2\n", 5.0, dL);
			RichEdit.AddText(sText);
			sText.Format("        = %.3f kN.m/m (보도 5 kN/㎡ 등분포하중 재하)\n", dMl);
			RichEdit.AddText(sText);
		}
		else
		{
			sText.Format("보도하중 계산 필요없음\n");
			RichEdit.AddText(sText);
		}

		return;
	}

	dL = toM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_L);
	if(CalcFloor.IsLaneLoad(nIndex) && dL > 0)
	{
		// 일반활하중
		double di			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_i;
		double dE			= toM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_E);
		double dMl			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml);
		double dMl_			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml_);

		// 탱크궤도하중
		double dTankWeight	= pCalcData->m_TankWeight;
		double dL2			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Tank_L;
		double dP2			= dTankWeight / 2 / pCalcData->m_TankEdit1;
		double dMl2			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml_Tank);
		
		// 탱크트레일러하중
		double dTrailer		= max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
		double dL3			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Trailer_L;
		double dE3			= CalcGeneral.CalcMovingLoad_E(dL3);
		double dMl3			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml_Trailer);

		CalcGeneral.CalcMovingLoad_I(dL, di, dii);
		if( dii > 0.3 )	sign = _T(">");
		else			sign = _T("<");

		sText.Format("      ① 충 격 계 수\n");
		RichEdit.AddText(sText);
		sText.Format("       i = 15 / (40 + L) = 15 / (40 + %.3f) = %.3f  %s  0.30      ∴ i = %.3f\n\n", dL, dii, sign, di);
		RichEdit.AddText(sText);
		sText.Format("      ② 윤하중 분포폭\n");
		RichEdit.AddText(sText);
		sText.Format("        E  = 0.8 ×L + 1.14 = 0.8 × %.3f + 1.14 = %.3f m   여기서,  L = %.3f m\n\n", dL, 0.8*dL+1.14, dL);
		RichEdit.AddText(sText);
		sText.Format("      ③ 휨 모 멘 트\n");
		RichEdit.AddText(sText);
		sText.Format("       - 일반활하중 적용시\n");
		RichEdit.AddText(sText);
		// lina
		sText.Format("       Ml  =  (Pr/E)  ×  L  × (1+i)  = (%.3f/%.3f)  × %.3f  × (1+%.3f)  = %.3f  kN.m/m\n\n", tokN(dPr), dE, dL, di, (tokN(dPr)/dE)*dL*(1+di));
		RichEdit.AddText(sText);
		if(dL-1.8>0)
		{
			CalcGeneral.CalcMovingLoad_I(dL-1.8, di, dii);
			if( dii > 0.3 )	sign = _T(">");
			else			sign = _T("<");

			sText.Format("      ④ 충 격 계 수\n");
			RichEdit.AddText(sText);
			sText.Format("       i = 15 / (40 + L) = 15 / (40 + %.3f) = %.3f  %s  0.30      ∴ i = %.3f\n\n", dL-1.8, dii, sign, di);
			RichEdit.AddText(sText);
			sText.Format("      ⑤ 윤하중 분포폭\n");
			RichEdit.AddText(sText);
			sText.Format("        E2  = 0.8 ×L + 1.14 = 0.8 × %.3f + 1.14 = %.3f m   여기서,  L = %.3f m\n\n", dL-1.8, dE, dL-1.8);
			RichEdit.AddText(sText);
			sText.Format("      ⑥ 휨 모 멘 트\n");
			RichEdit.AddText(sText);
			sText.Format("       - 일반활하중 적용시\n");
			RichEdit.AddText(sText);
			sText.Format("       Ml2  =  (Pr/E)  ×  L  × (1+i)  = (%.3f/%.3f)  × %.3f  × (1+%.3f)  = %.3f  kN.m/m\n\n", tokN(dPr), dE, dL-1.8, di, dMl_);
			RichEdit.AddText(sText);
			sText.Format("       Ml Total = Ml + Ml2 = %.3f + %.3f = %.3f kN.m/m\n\n", dMl-dMl_,dMl_,dMl);
			RichEdit.AddText(sText);
		}

		if(pCalcData->m_bTank)			// 탱크하중
		{
			sText.Format("       - 탱크궤도하중 작용시\n");
			RichEdit.AddText(sText);
			sText.Format("       Pr  =  W / 2 / L ( 탱크접지길이 ) =  %.1f / 2 / %.3f   =   %.3f kN/m\n", dTankWeight, pCalcData->m_TankEdit1, dP2);
			RichEdit.AddText(sText);
			sText.Format("       Ml  =  Pr  ×  L ( 모멘트길이 )  × (1 + i)  = %.3f  × %.3f  × (1+%.3f)  = %.3f  kN.m/m\n", dP2, dL2, diT, dMl2);
			RichEdit.AddText(sText);
		}

		if(pCalcData->m_bTrailer)
		{
			if(dL3 > 0)
			{
				sText.Format("       - 탱크트레일러하중 작용시\n");
				RichEdit.AddText(sText);
				sText.Format("       E  =  0.8 × L + 1.14  =  0.8 × %.3f + 1.14   =   %.3f m\n", dL3, dE3);
				RichEdit.AddText(sText);
				sText.Format("       Ml =  (Pr/E)  ×  L ( 모멘트길이 )  × (1+i)  = (%.3f/%.3f)  × %.3f  × (1+%.3f)  = %.3f  kN.m/m\n", dTrailer, dE3, dL3, diT, dMl3);
				RichEdit.AddText(sText);
			}
			if(dL3<=0)
			{
				sText.Format("     탱크트레일러하중 계산 필요없음\n");
				RichEdit.AddText(sText);
			}
		}
	}
	else
	{
		sText.Format("     활하중이 재하되지 않음\n");
		RichEdit.AddText(sText);
	}
}

// 활하중 계산 - 중앙부
void CPlateStressOut::Output_MoveLoadCenter(CShRichEdit &RichEdit)
{
	CCalcData       *pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());
	CCalcFloor		CalcFloor(m_pStd->GetDataManage());

	CString sText(_T(""));
	CString sign(_T(""));
	double	dMl				= tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml);
	double	dMl_Tank		= tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Tank);
	double	dMl_Trailer		= tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Trailer);
	double	dL				= toM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_L);
	double	dPr				= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dTankWeight		= pCalcData->m_TankWeight / 2 / pCalcData->m_TankEdit1;
	double	dTrailerWeight	= max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
	double	dII				= 0;
	double	dI				= 0;
	double	dComp_i			= 0.3;
	double	diT				= 0.15;

	CalcGeneral.CalcMovingLoad_I(dL, dI, dII);

	sText.Format("    (2) 활하중     ( DB-24 : Pr = %.1f kN)     (☞도.설.기 2.1.3)\n\n", dPr);
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	if(CalcFloor.IsLaneLoad(FLOOR_MID))
	{
		sText.Format("      계산지간 L = %.3f m      \r\n", dL);						RichEdit.AddText(sText);
		sText.Format("      단순지지의 경우 바닥판의 지간은 지지거더의 중심간격으로 설정한다.\n");;	RichEdit.AddText(sText);
		sText.Format("      이때 지간은 바닥판의 순지간에 바닥판의 두께를 더한 길이를 초과할 필요는 없다.\n\n");
		RichEdit.AddText(sText);

		if( dII > dComp_i ) sign = _T(" >");
		else 	sign = _T("<");

		sText.Format("      ① 충 격 계 수  i = 15 / (40 + L) = 15 / (40 + %.3f) = %.3f   %s   0.3    ∴  i = %.3f\n\n", dL, dII, sign, dI);
		RichEdit.AddText(sText);
		sText.Format("      ② 휨 모 멘 트\n");
		RichEdit.AddText(sText);
		sText.Format("       - 일반활하중 적용시\n");
		RichEdit.AddText(sText);
		sText.Format("         Ml  =  ( L + 0.6 ) / 9.6 × Pr × ( 1 + i )  (바닥판이 3개 이상의 지점을 가진 연속판일때는 0.8배를 취함)\n");
		RichEdit.AddText(sText);
		sText.Format("              =  ( %.3f + 0.6 ) / 9.6 × %.3f × ( 1 + i ) = %.3f  kN/m\n", dL, dPr, dMl);
		RichEdit.AddText(sText);

		if(pCalcData->m_bTank)
		{
			sText.Format("       - 탱크궤도하중 적용시\n");
			RichEdit.AddText(sText);
			RichEdit.AddText("");
			sText.Format("         Pr  =  W / 2 / L(탱크접지길이) =  %.1f / 2 / %.3f   =   %.3f kN/m\n", pCalcData->m_TankWeight, pCalcData->m_TankEdit1, dTankWeight);
			RichEdit.AddText(sText);
			sText.Format("         Ml  =  ( %.3f + 0.6 ) / 9.6 × %.3f × (1 + %.3f) = %.3f kN/m\n", dL, dTankWeight, diT, dMl_Tank);
			RichEdit.AddText(sText);
		}

		if(pCalcData->m_bTrailer)
		{
			sText.Format("       - 탱크트레일러하중 적용시\n");
			RichEdit.AddText(sText);
			RichEdit.AddText("");
			sText.Format("         Pr  =  90 kN/m\n");
			RichEdit.AddText(sText);
			sText.Format("         Ml  =  ( %.3f + 0.6 ) / 9.6 × %.3f × (1 + %.3f) = %.3f kN/m (탱크트레일러 하중 작용시)\n", dL, dTrailerWeight, diT, dMl_Trailer);
			RichEdit.AddText(sText);
		}
	}
	else
	{
		sText.Format("     활하중이 재하되지 않음\n");
		RichEdit.AddText(sText);
	}

}

void CPlateStressOut::Output_WindLoad(long nSide, CShRichEdit &RichEdit)
{
	CPlateBridge	*pDB		= m_pStd->GetBridge();
	CCalcData       *pCalcData	= m_pStd->GetDataManage()->GetCalcData();

	double	dPw			= tokNPM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_Pw);
	double	dMw			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_Mw);
	double	dHH			= toM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_H);
	double	dWalkLoadH	= pCalcData->DESIGN_FLOOR_DATA[nSide].m_dWalkLoadH;
	double dWindLoadForce = pCalcData->m_dWindLoadForce;
	double	dPaveT		= toM(pDB->m_dThickPave);
	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad;
	CString	sText		= _T("");

	sText.Format("    (3) 풍 하 중 \n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	long nHDan = 0;

	if(nSide == FLOOR_LEFT)
		nHDan = 0;
	else if(nSide == FLOOR_RIGHT)
		nHDan = m_pStd->GetDataManage()->GetBridge()->GetQtyHDanNode() - 1;

	if(nSide==FLOOR_MID || !(pDB->GetValueTypeHDan(nHDan, 1) == 1))
	{
		sText.Format("      ※ 풍하중에 대한 내용이 없습니다.\n");
		RichEdit.AddText(sText);
		return;
	}

	if(dHH > 0)
	{
		sText.Format("      Pw  =  %.1f  × H  =  %.1f  ×  %.3f\n", dWindLoadForce, dWindLoadForce, dHH);	RichEdit.AddText(sText);
		sText.Format("            =  %.3f  kN/m\n", dPw);					RichEdit.AddText(sText);
		if(bWalkLoad)
		{
			sText.Format("      Mw  =  Pw × ( H / 2 + t )  =  %.3f  × ( %.3f / 2 + %.3f )\n", dPw, dHH, dWalkLoadH);	RichEdit.AddText(sText);
			sText.Format("             =  %.3f    kN·m/m\n", dMw);														RichEdit.AddText(sText);
		}
		else
		{
			sText.Format("      Mw  =  Pw × ( H / 2 + t ) = %.3f  × ( %.3f / 2 + %.3f )\n", dPw, dHH, dPaveT);	RichEdit.AddText(sText);
			sText.Format("            =  %.3f    kN·m/m\n", dMw);													RichEdit.AddText(sText);
		}
	}
}

// 바닥판 중앙부 원심하중 검토
void CPlateStressOut::OutPut_CentriodLoadCenter(CShRichEdit &RichEdit)
{
	CCalcData       *pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp *pDB	= m_pStd->GetBridge();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());

	CString newElement(_T(""));
	double	dL				= CalcGeneral.CalcCentPart_L() + pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;
	double	dPf				= (pCalcData->DESIGN_CONDITION.m_dPf)/10000;
	double	dPr				= pCalcData->DESIGN_CONDITION.m_dPr;
	double	dV				= pCalcData->m_dSpeed;
	double	dR				= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R;
	double	dMcf			= tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf);
	double	dMcf_Tank		= tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Tank);
	double	dMcf_Trailer	= tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Trailer);
	double	dE				= dPf*toM(dL)/(toM(dL)+0.6);
						//	2.4 ×  5.75  /  ( 5.75 + 0.6 ) 								
	double	dTankV			= 40.0;
	double	dCf				= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
	double	dCf_Tank		= CalcGeneral.CalcCentriodLoad_CF(dTankV, dR);
	double	dCf_Trailer		= CalcGeneral.CalcCentriodLoad_CF(dTankV, dR);

	newElement.Format("    (4) 원 심 하 중\n\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);

	if(dR == 0.0)
	{
		newElement.Format("      ※ 원심하중에 대한 내용이 없습니다.\n");
		RichEdit.AddText(newElement);
		return;
	}

	newElement.Format("      ※ 설계속도  V  =  %.3f       R  = %.3f m\n", dV, dR);
	RichEdit.AddText(newElement);

	newElement.Format("         CF  =  0.79  ×  V² /   R  (%%) =  0.79  ×  %.3f² /   %.3f  =  %.3f %%\n", dV, dR, dCf);
	RichEdit.AddText(newElement);
	newElement.Format("        Mcf  =  Pcf×1.80  =  (9.6/E)×CF/100×1.8  =  (9.6/%.3f) × %.3f / 100× 1.8  =  %.3f kN.m/m\n", dE, dCf, dMcf);
	RichEdit.AddText(newElement);

	if(pCalcData->m_bTank)
	{
		dPr = pCalcData->m_TankWeight / 2 / pCalcData->m_TankEdit1;
		newElement.Format("      - 탱크궤도하중 작용시");
		newElement.Format("        Mcf  =  Pr × CF/100 ×1.80  =  %.3f × %.3f/100 × 1.80  =  %.3f kN·m (탱크궤도하중작용시)\n", dPr, dCf_Tank, dMcf_Tank);
		RichEdit.AddText(newElement);
	}

	if(pCalcData->m_bTrailer)
	{
		dPr = max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));

		newElement.Format("        Mcf  =  (Pr/E) × CF/100 ×1.80  =  (%.3f/%.3f) × %.3f/100 × 1.80 = %.3f kN·m (탱크트레일러하중작용시)\n", dPr, dE, dCf_Trailer, dMcf_Trailer);
		RichEdit.AddText(newElement);
	}
}

void CPlateStressOut::Output_CollisionLoad(long nSide, CShRichEdit &RichEdit)
{
	CCalcData			*pCalcData		= m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CCentSeparation		*pSep			= NULL;
	CPlateGirderApp		*pGir			= NULL;
	CCalcGeneral        CalcGeneral(m_pStd->GetDataManage());

	double	dMoveL		= toM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_L);
	double	dH			= 0;
	double	dH2			= 1.0 + toM(pDB->m_dThickPave);
	double	dVelocity	= 0;
	double	dHoriForce	= 0;
	double	dMoment		= 0;
	double	dMoment1	= 0;
	double	dMoment2	= 0;
	double	dR			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_R;
	BOOL	bNanGan		= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bNanGan;
	long	nQtyGir		= pDB->GetGirdersu();

	CStringArray arr;	
	CString newElement(_T(""));	
	CString sign(_T(""));
	if(nSide == FLOOR_LEFT)
	{
		pGir	= pDB->GetGirder(0);
		pSep	= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	}
	else if(nSide == FLOOR_RIGHT)
	{
		pGir	= pDB->GetGirder(nQtyGir-1);
		pSep	= pDB->GetCurGuardWall(CCentSeparation::RIGHT);
	}
	if(!pSep)
	{
		newElement.Format("    (5) 차량 충돌 하중        (☞ 도.설.해 2.4.3.5)\n\n");
		RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
		newElement.Format("       ※ 차량 충돌 하중 대한 내용이 없습니다.");
		RichEdit.AddText(newElement);
		return;
	}

	if(bNanGan)			dH		= toM(pSep->GetHeight() + pCalcData->DESIGN_FLOOR_DATA[nSide].m_dNanGanHeight);
	else				dH		= toM(pSep->GetHeight());

	if(pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad)
	{
		double dHoriF	= tokNPM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_HoriF);
		double dVeriF	= tokNPM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_VerF);
		double dH		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_dH;
		double dL		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_dL;

		double	dMCo = tokNM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Mo);//dHoriF * dH;

		newElement.Format("    (5) 군중에 의한 추력 또는 차량의 2차 충돌하중	       \n\n");
		RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
		newElement.Format("      · 수평충돌력 H와 수직력 V가 상단부와 난간 정상부에 작용하는 것으로 한다.\n");
		RichEdit.AddText(newElement);
		newElement.Format("            수평 충돌력   H  =  %.3f  kN/m\n", dHoriF);
		RichEdit.AddText(newElement);
		newElement.Format("            수직력   V  =  %.3f  kN/m\n", dVeriF);
		RichEdit.AddText(newElement);
		newElement.Format("            작용 모멘트  Mco = %.3f × %.3f + %.3f × %.3f =  %.3f kN·m/m\n", dHoriF, dH, dVeriF, dL, dMCo);
		RichEdit.AddText(newElement);	
	}
	else
	{
		newElement.Format("    (5) 차량 충돌 하중        (☞ 도.설.해 2.4.3.5)\n\n");
		RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
		dVelocity	= pCalcData->m_dSpeed;
		dHoriForce	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Co;
		dMoment1	= dHoriForce*dH2;
		dMoment2	= 10.0 * dH;
		dMoment		= max(dMoment1,dMoment2);
			
		newElement.Format("    ① 수평충돌력  H가 노면상 1m 높이에 교축을 따라 1m 마다 작용하는 것으로 한다.\n" );
		RichEdit.AddText(newElement);
		newElement.Format("      H = (V/60)² × 750 + 250 = (%.3f/60)² × 750 + 250 = %.3f N/m = %.3f kN/m\n", dVelocity, frkN(dHoriForce), dHoriForce);
		RichEdit.AddText(newElement);
		newElement.Format("      작용  높이   h = %.3f m\n ", dH2);
		RichEdit.AddText(newElement);
		newElement.Format("      작용모멘트 Mco = H × h = %.3f × %.3f = %.3f kN·m\n", dHoriForce, dH2, dMoment1);
		RichEdit.AddText(newElement+"\r\n");
		newElement.Format("    ② 방호벽 상단에 횡방향으로 10kN/m 의 수평하중을 재하한다.\n");
		RichEdit.AddText(newElement);

		newElement.Format("      작용  높이   h = %.3f m \n", dH);
		RichEdit.AddText(newElement);
		newElement.Format("      작용모멘트 Mco = H × h = 1.000 × %.3f = %.3f kN·m\n", dH, dMoment2);
		RichEdit.AddText(newElement);

		if(dR>200 || dR==0)
		{
			if(dR==0)
			{
				newElement.Format("      곡률반지름  R = ∞ m ＞ 200.0 m 이므로, ①과 ②의 값 중 큰 값을 적용한다.\n", dR);
				RichEdit.AddText(newElement);
			}
			else
			{
				newElement.Format("     곡률반지름  R = %.1f m ＞ 200.0 m 이므로, ①과 ②의 값 중 큰 값을 적용한다.\n", dR);
				RichEdit.AddText(newElement);
			}
				
			newElement.Format("     Mco = %.3f kN·m",dMoment);
			RichEdit.AddText(newElement);
		}
		else
		{
			newElement.Format("     곡률반지름  R = %.1f m ≤ 200.0 m 이므로, ①과 ②의 값 중 큰 값의 2배를 적용한다.\n", dR);
			RichEdit.AddText(newElement);
			newElement.Format("     Mco = %.3f kN·m\n",2.0*dMoment);
			RichEdit.AddText(newElement);
		}
	}
}

void CPlateStressOut::Output_CentriodLoad(long nSide, CShRichEdit &RichEdit)
{
	CCalcData			*pCalcData		= m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CCentSeparation		*pSep			= NULL;
	CPlateGirderApp		*pGir			= NULL;
	CCalcGeneral        CalcGeneral(m_pStd->GetDataManage());

	CString	sText	= _T("");

	if(nSide==FLOOR_LEFT)	pSep = pDB->GetCurGuardWall(CCentSeparation::LEFT);
	else					pSep = pDB->GetCurGuardWall(CCentSeparation::RIGHT);

	CPlateBxFinder		Finder(pGir);
	double	dL				= toM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_L);
	double	dE				= toM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_E);
	double	dV				= pCalcData->m_dSpeed;//pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Coll_V;
	double	dMcf			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf);
	double	dPcf			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf;
	double	dPr_Tank		= pCalcData->m_TankWeight / 2.0 / pCalcData->m_TankEdit1;
	double	dCf_Tank		= 0;
	double	dPcf_Tank		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf_Tank;
	double	dMcf_Tank		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Tank;
	double	dPr_Trailer		= max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
	double	dCf_Trailer		= 0;
	double	dPcf_Trailer	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf_Trailer;
	double	dMcf_Trailer	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Trailer;
	//double	dWidth			= pSep ? pSep->GetWidth() : 0;
	//double	dSharp			= pGir->GetLengthSharpOnBx(pBx, TRUE);
	//double	dD1				= pSep ? pSep->m_D1 : 0;
	double	dCf				= 0;	
	double  dE3				= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_E3;
	BOOL	bWalkLoad		= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad;
	BOOL	bTank			= pCalcData->m_bTank;
	BOOL	bTrailer		= pCalcData->m_bTrailer;
	double	dTerm			= 0;
	double	dR				= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_R;
	double	dPr				= tokN(pCalcData->DESIGN_CONDITION.m_dPr);

	sText.Format("    (4) 원 심 하 중 \n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	if(!bWalkLoad && dL>0 && dR>0)
	{
		sText.Format("      ※  설계속도  V  =  %.3f km/hr       R  = %.3f\n", dV, dR);
		RichEdit.AddText(sText);
		dCf	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
		if(dR == 0) dCf = 0;

		if(nSide == FLOOR_LEFT || nSide == FLOOR_RIGHT)
		{
			sText.Format("      CF  =  0.79  ×  V²/  R  (%%) =  0.79  ×  %.3f²  /  %.3f  =  %.3f %%\n", dV, dR, dCf);
			RichEdit.AddText(sText);
			sText.Format("      Pcf  =  (Pr/E) × CF/100  =  (%.3f/%.3f) × %.3f/100  =  %.3f kN·m\n", dPr, dE, dCf, dPcf);
			RichEdit.AddText(sText);

			sText.Format("      - 일반활하중 작용시\n");
			RichEdit.AddText(sText);
			if(bTank || bTrailer)
				sText.Format("      Mcf  =  Pcf × 1.80  =  %.3f × 1.80  =  %.3f kN·m\n", dPcf, dMcf);
			else
				sText.Format("      Mcf  =  Pcf × 1.80  =  %.3f × 1.80  =  %.3f kN·m\n", dPcf, dMcf);
			RichEdit.AddText(sText);

			if(bTank)		// 탱크궤도하중 작용시
			{
				dV			= 40;//탱크나 트레일러는 속도 40 고정..
				if(dR ==0)
				{
					dCf_Tank	= 0;					
				}
				else
				{
					dCf_Tank	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
				}
				
				sText.Format("      - 탱크궤도하중 작용시\n");
				RichEdit.AddText(sText);
				sText.Format("      CF  =  0.79  ×  V²/  R  (%%) =  0.79  ×  %.3f²   /   %.3f  =  %.3f %%\n", dV, dR, dCf_Tank);
				RichEdit.AddText(sText);
				sText.Format("      Pcf  =  Pr × CF/100  =  %.3f × %.3f/100  =  %.3f kN·m\n", dPr_Tank, dCf_Tank, dPcf_Tank);
				RichEdit.AddText(sText);
				sText.Format("      Mcf  =  Pcf × 1.80  =  %.3f × 1.80  =  %.3f kN·m\n", dPcf_Tank, dMcf_Tank);
				RichEdit.AddText(sText);
			}
			if(bTrailer)	// 탱크트레일러 궤도하중작용시
			{
				dV			= 40;//탱크나 트레일러는 속도 40 고정..
				if(dR ==0)
				{					
					dCf_Trailer = 0;
				}
				else
				{
					dCf_Trailer	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
				}
				if(nSide == FLOOR_LEFT)	dTerm = pDB->m_dWidthSlabLeft;
				else					dTerm = pDB->m_dWidthSlabRight;
				
				//dL		= dTerm - dSharp/2 - (487.5 + dD1 + dWidth);
				//dE		= CalcGeneral.CalcMovingLoad_E(dL);
				sText.Format("      - 탱크트레일러하중 작용시\n");
				RichEdit.AddText(sText);
				sText.Format("      CF  =  0.79  ×  V²/  R  (%%) =  0.79  ×  %.3f²   /   %.3f  =  %.3f %%\n", dV, dR, dCf_Trailer);
				RichEdit.AddText(sText);
				sText.Format("      Pcf  =  (Pr/E) × CF/100  =  (%.3f/%.3f) × %.3f/100  =  %.3f kN·m\n", dPr_Trailer, dE3, dCf_Trailer, dPcf_Trailer);
				RichEdit.AddText(sText);
				sText.Format("      Mcf  =  Pcf × 1.80  =  %.3f × 1.80  =  %.3f kN·m\n", dPcf_Trailer, dMcf_Trailer);
				RichEdit.AddText(sText);
			}
		}
		else
		{
			sText.Format("      CF  =  0.79  ×  V²/  R  (%%) =  0.79  ×  %.3f²  /  %.3f  =  %.3f %%\n", dV, dR, dCf);
			RichEdit.AddText(sText);
			sText.Format("      Mcf  =  Pcf × 1.8  =  (%.3f / E)	× CF/100 × 1.8\n", dPr);
			RichEdit.AddText(sText);
			sText.Format("           =  ( %.3f / %.3f ) × %.3f/100 × 1.8  =  %.3f kN·m\n", dPr, dE, dCf, dMcf);
			RichEdit.AddText(sText);
		}
	}
	else
	{
		sText.Format("      ※ 원심하중에 대한 내용이 없습니다.");
		RichEdit.AddText(sText);
	}
}

void CPlateStressOut::Output_TotalCombinationLoad(long nPos, CShRichEdit &RichEdit) 
{
	CCalcData			*pCalcData	= m_pStd->GetDataManage()->GetCalcData();

	CString newElement(_T(""));	

	double	dD   = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Stop_M);			// 사하중
	double	dCo  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Coll_Mo);			// 충돌하중
	double	dCF  = tokNM((pCalcData->CALC_CANTILEVER_DATA[nPos].m_Cent_Mcf));				// 원심하중
	double	dCF2 = (pCalcData->CALC_CANTILEVER_DATA[nPos].m_Cent_Mcf_Tank);			// 원심하중(탱크궤도하중일때)
	double	dCF3 = (pCalcData->CALC_CANTILEVER_DATA[nPos].m_Cent_Mcf_Trailer);		// 원심하중(탱크트레일러하중일때)
	double	dLi  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_Ml);			// 활하중
	double	dLi2 = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_Ml_Tank);		// 활하중(탱크궤도하중일때)
	double	dLi3 = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_Ml_Trailer);	// 활하중(탱크트레일러하중일때)
	double	dW   = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Wind_Mw);			// 풍하중
	double	dMu  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Rein_Mu);

	newElement.Format("    (6) 하중 조합    ( ☞ 도.설 2.2.3.2 )\n\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);

	newElement.Format("    - 최대 계수 모멘트 계산\n");	
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, FALSE);

	if(nPos == 0 || nPos == 1) // 하중조합 좌측. 우측
	{
		if(pCalcData->DESIGN_FLOOR_DATA[nPos].m_bWalkLoad) // 보도하중이 있는 경우
		{
			// #APLATE-660, 계산식의 오류를 없애기 위해 계산식을 수정한다.
			double	dMu_1 = Round(1.3 * dD + 2.15 * dLi, 3);
			dMu = dMu > dMu_1 ? dMu : dMu_1;
			double	dMu_2 = Round(1.3 * dD + 1.3 * dLi + 1.3 * dCo, 3);
			dMu = dMu > dMu_2 ? dMu : dMu_2;
			double	dMu_3 = Round(1.3 * dD + 1.3 * dLi + 0.65 * dW, 3);
			dMu = dMu > dMu_3 ? dMu : dMu_3;
			double	dMu_4 = Round(1.2 * dD + 1.2 * dW + 1.2 * dCo, 3);
			dMu = dMu > dMu_4 ? dMu : dMu_4;
			double	dMu_5 = Round(1.3 * dD + 1.3 * dW, 3);
			dMu = dMu > dMu_5 ? dMu : dMu_5;

			newElement.Format("     ① Mu = 1.3D + 2.15(L+i)          = 1.3 × %.3f + 2.15× %.3f                 = %.3f kN.m/m\n", dD, dLi, dMu_1);
			RichEdit.AddText(newElement);
			newElement.Format("     ② Mu = 1.3D +  1.3(L+i) + 1.3CO  = 1.3 × %.3f + 1.3 × %.3f + 1.3  × %.3f  = %.3f kN.m/m\n", dD, dLi, dCo, dMu_2);
			RichEdit.AddText(newElement);
			newElement.Format("     ③ Mu = 1.3D +  1.3(L+i) + 0.65W  = 1.3 × %.3f + 1.3 × %.3f + 0.65 × %.3f  = %.3f kN.m/m\n", dD, dLi, dW, dMu_3);
			RichEdit.AddText(newElement);
			newElement.Format("     ④ Mu = 1.2D + 1.2W + 1.2CO       = 1.2 × %.3f + 1.2 × %.3f + 1.2  × %.3f  = %.3f kN.m/m\n", dD, dW, dCo, dMu_4);
			RichEdit.AddText(newElement);
			newElement.Format("     ⑤ Mu = 1.3D + 1.3W               = 1.3 × %.3f + 1.3 × %.3f                 = %.3f kN.m/m\n", dD, dW, dMu_5);
			RichEdit.AddText(newElement);
		}
		else
		{
			// #APLATE-660, 계산식의 오류를 없애기 위해 선형의 상태에 대한 분기를 삭제한다.
			double	dMu_1 = Round(1.3 * dD + 2.15 * dLi + 1.3 * dCF, 3);
			dMu = dMu > dMu_1 ? dMu : dMu_1;
			double	dMu_2 = Round(1.3 * dD + 1.3 * dLi + 1.3 * dCF + 1.3 * dCo, 3);
			dMu = dMu > dMu_2 ? dMu : dMu_2;
			double	dMu_3 = Round(1.3 * dD + 1.3 * dLi + 1.3 * dCF + 0.65 * dW, 3);
			dMu = dMu > dMu_3 ? dMu : dMu_3;
			double	dMu_4 = Round(1.2 * dD + 1.2 * dW + 1.2 * dCo, 3);
			dMu = dMu > dMu_4 ? dMu : dMu_4;
			double	dMu_5 = Round(1.3 * dD + 1.3 * dW, 3);
			dMu = dMu > dMu_5 ? dMu : dMu_5;

			newElement.Format("     ① Mu = 1.3D + 2.15(L+i) + 1.3CF           = 1.3 × %.3f + 2.15 × %.3f + 1.3 × %.3f                 = %.3f kN.m/m\n", dD, dLi, dCF, dMu_1);
			RichEdit.AddText(newElement);
			newElement.Format("     ② Mu = 1.3D +  1.3(L+i) + 1.3CF + 1.3CO   = 1.3 × %.3f + 1.3  × %.3f + 1.3 × %.3f + 1.3  × %.3f  = %.3f kN.m/m\n", dD, dLi, dCF, dCo, dMu_2);
			RichEdit.AddText(newElement);
			newElement.Format("     ③ Mu = 1.3D +  1.3(L+i) + 1.3CF + 0.65W   = 1.3 × %.3f + 1.3  × %.3f + 1.3 × %.3f + 0.65 × %.3f  = %.3f kN.m/m\n", dD, dLi, dCF, dW, dMu_3);
			RichEdit.AddText(newElement);
			newElement.Format("     ④ Mu = 1.2D + 1.2W + 1.2CO                = 1.2 × %.3f + 1.2  × %.3f + 1.2 × %.3f                 = %.3f kN.m/m\n", dD, dW, dCo, dMu_4);
			RichEdit.AddText(newElement);
			newElement.Format("     ⑤ Mu = 1.3D + 1.3W                        = 1.3 × %.3f + 1.3  × %.3f                               = %.3f kN.m/m\n", dD, dW, dMu_5);
			RichEdit.AddText(newElement);

			if(pCalcData->m_bTank)
			{
				double dMu_6 = 1.3 * dD + 1.3 * dLi2 + 1.3 * dCF2;
				dMu = dMu > dMu_6 ? dMu : dMu_6;

				newElement.Format("     ⑥ Mu = 1.3D + 1.3(L+i) + 1.3CF (탱크궤도하중작용시)      = 1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f   = %.3f kN.m/m\n", dD, dLi2, dCF2, dMu_6);
				RichEdit.AddText(newElement);
			}

			if(pCalcData->m_bTrailer)
			{
				double dMu_7 = 1.3 * dD + 1.3 * dLi3 + 1.3 * dCF3;
				dMu = dMu > dMu_7 ? dMu : dMu_7;

				if(!pCalcData->m_bTank)
				{
					newElement.Format("     ⑥ Mu = 1.3D + 1.3(L+i) + 1.3CF (탱크트레일러하중작용시)  = 1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f   = %.3f kN.m/m\n", dD, dLi3, dCF3, dMu_7);
					RichEdit.AddText(newElement);
				}
				else
				{
					newElement.Format("     ⑦ Mu = 1.3D + 1.3(L+i) + 1.3CF (탱크트레일러하중작용시)  = 1.3 × %.3f + 1.3 × %.3f + 1.3 × %.3f   = %.3f kN.m/m\n", dD, dLi3, dCF3, dMu_7);
					RichEdit.AddText(newElement);
				}
			}
		}
		newElement.Format("   ㆍ 최대 계수 모멘트는 하중조합 중에서 가장 큰 Mu = %.3f kN.m/m 로 단면 검토한다. \n\n", dMu);
		RichEdit.AddText(newElement);
	}
	else if(nPos == 1)
	{
		newElement.Format("    < 캔틸레버판 단부설계 >\n\n");
		RichEdit.AddText(newElement);
		newElement.Format("   ① 주철근량 : 들보 단부 이외의 켄틸레버부 바닥판에서 필요한 주철근량의 2배를 주철근으로 배근한다.\n");
		RichEdit.AddText(newElement);
		newElement.Format("   ② 배력철근량 : 들보 단부 이외의 켄틸레버부 바닥판의 상축에 배력철근량의 2배를 배력철근으로 배근한다.\n");
		RichEdit.AddText(newElement);
		newElement.Format("   ③ 도로교 시방서 강교편 바닥판에 배치범위가 명시되어 있지 않으므로 콘크리트편\n");
		RichEdit.AddText(newElement);
		newElement.Format("      ☞도.시 P453 을 적용하여 켄틸레버판의 사하중에 대한 지간을 배치범위로 적용한다.\n");
		RichEdit.AddText(newElement);	
	}
	else if(nPos == 2)		// 하중조합 중앙부
	{
		dD		= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Stop_M),3);
		double	dML1	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml),3);
		double	dML2	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Tank),3);
		double	dML3	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Trailer),3);
		double	dMcf1	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf),3);
		double	dMcf2	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Tank),3);
		double	dMcf3	= Round(tokNM(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Trailer),3);
		double	dMu_1	= Round(1.3 * dD + 2.15 * dML1 + 1.3 * dMcf1,3);
		double	dMu_2	= Round(1.3 * dD +  1.3 * dML2 + 1.3 * dMcf2,3);
		double	dMu_3	= Round(1.3 * dD +  1.3 * dML3 + 1.3 * dMcf3,3);

		// #APLATE-660, 계산식의 오류를 없애기 위해 선형의 상태에 대한 분기를 삭제한다.
		newElement.Format("     ① Mu = 1.3D + 2.15(L+i) + 1.3CF  = 1.3 × %.3f + 2.15 × %.3f + 1.3 × %.3f   = %.3f  kN.m/m\n",	dD, dML1, dMcf1, dMu_1);
		RichEdit.AddText(newElement);

		if(pCalcData->m_bTank)
		{
			dMu_1 = dMu_1 > dMu_2 ? dMu_1 : dMu_2;

			newElement.Format("     ② Mu = 1.3D + 1.3(L+i) + 1.3CF  = 1.3 × %.3f +  1.3 × %.3f + 1.3 × %.3f   = %.3f  kN.m/m( 탱크궤도하중작용시 )\n", dD, dML2, dMcf2, dMu_2);
			RichEdit.AddText(newElement);
		}

		if(pCalcData->m_bTrailer)
		{
			dMu_1 = dMu_1 > dMu_3 ? dMu_1 : dMu_3;

			if(!pCalcData->m_bTank)
				newElement.Format("     ② Mu = 1.3D + 1.3(L+i) + 1.3CF  = 1.3 × %.3f +  1.3 × %.3f + 1.3 × %.3f   = %.3f   kN.m/m( 탱크트레일러하중작용시 )\n", dD, dML3, dMcf3, dMu_3);
			else
				newElement.Format("     ③ Mu = 1.3D + 1.3(L+i) + 1.3CF  = 1.3 × %.3f +  1.3 × %.3f + 1.3 × %.3f   = %.3f   kN.m/m( 탱크트레일러하중작용시 )\n", dD, dML3, dMcf3, dMu_3);
			RichEdit.AddText(newElement);
		}

		newElement.Format("   ㆍ 최대 계수 모멘트는 하중조합 중에서 가장 큰 Mu = %.3f kN.m/m 로 단면 검토한다. \n\n", dMu_1);
		RichEdit.AddText(newElement);
	}

	// 2014. 12. 26 #APLATE-660, 최대 사용 모멘트 하중 조합을 추가한다.
	newElement.Format("    - 최대 사용 모멘트 계산\n");	
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, FALSE);

	double	dMu_Use	= Round(dD + dLi + 0.3 * dW, 3);

	newElement.Format("     ① Ms = D + Li + 0.3W = %.3f + %.3f + 0.3 × %.3f = %.3f kN.m/m\n", dD, dLi, dW, dMu_Use);
	RichEdit.AddText(newElement);

	newElement.Format("   ㆍ 최대 사용 모멘트는 하중조합 중에서 가장 큰 Ms = %.3f kN.m/m 로 균열 검토한다. \n\n", dMu_Use);
	RichEdit.AddText(newElement);
}

//슬래브최소두께검토
void CPlateStressOut::Output_SlabThickness(long nPos, CShRichEdit &RichEdit) 
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CCalcData			*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	BOOL bUsePreStress  = pDB->m_nSlabTensionSize==0 ? FALSE : TRUE;
	BOOL bWalkLoad		= pCalcData->DESIGN_FLOOR_DATA[nPos].m_bWalkLoad;	

	double  dLastCheck	= 220;//차도부 바닥판의 최소두께는 220
	if(bUsePreStress)	dLastCheck = 200;//프리스트레스 적용이면...(소수주거더교)

	CString sText	= _T("");
	CString sign(_T("")), sign2(_T("")), signDet(_T(""));
	double	dL	= toM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_L);	// 활하중에 의한 지간
	double	dT	= pCalcData->CALC_CANTILEVER_DATA[nPos].m_Req_T;		// 최바닥판 최소두께
	double	dmT	= 0.25;
	long	nGirdersu	= pDB->GetGirdersu();

	RichEdit.AddText("\n");
	sText.Format("    (1) 바닥판의 최소 두께             (☞ 도.설.기 3.6.1.5)\n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	if(bWalkLoad)
	{
		sText = "       보도부 바닥판의 최소 두께는 140 mm를 표준으로 한다.\n";
		RichEdit.AddText(sText);
	}
	else if(dL > 0)
	{
	
		double dTB	= pDB->m_dThickSlabBase;

		//씁 이걸 빼먹다니..끙...070307....KB...
		if(nPos == FLOOR_MID) dTB = (pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch);

		if(dT > dTB )	
		{
			signDet	= _T("N.G\n");
			sign2	= ">";
		}
		else
		{
			signDet	= _T("O.K\n");
			sign2	= "<";
		}

		sText.Format("      ① 활하중에 의한 지간 길이 ( L )\n");
		RichEdit.AddText(sText);

		if(nPos == FLOOR_LEFT || nPos== FLOOR_RIGHT)	// 좌.우측 최소두께 검토
		{
			if(dL > dmT)	
			{
				sign = _T(">");
				sText.Format("       L  =  %.3f  %s  0.25    지간길이가 0.25보다 크므로\n", dL, sign);
				RichEdit.AddText(sText);
				sText.Format("      ∴ T  =  80 ×  L  +  230\n\n");
				RichEdit.AddText(sText);
				if(bUsePreStress)	sText.Format("      ② 최소두께 T  =  (80  ×  %.3f  +  230) × 0.9  =  %.1f mm이상\n", dL, dT);
				else				sText.Format("      ② 최소두께 T  =  80  ×  %.3f  +  230  =  %.1f mm이상\n", dL, dT);
				RichEdit.AddText(sText);
			}
			else
			{
				sign = _T("≤");
				sText.Format("      L  =  %.3f  %s  0.25    지간길이가 0.25보다 작거나 같으므로\n\n",	dL, sign );
				RichEdit.AddText(sText);
				sText.Format("      ∴ T  =  280  ×  L  +  180 적용\n");
				RichEdit.AddText(sText);
				if(bUsePreStress)	sText.Format("      ② 최소두께 T  =  (280 ×  %.3f + 180) × 0.9  =  %0.1f mm이상\n", dL, dT );
				else				sText.Format("      ② 최소두께 T  =  280 ×  %.3f + 180  =  %.1f mm이상\n", dL, dT );
				RichEdit.AddText(sText);
				
			}
		}
		else if (nGirdersu > 2)//중앙부이고 연속판이면..
		{
			sText.Format("       L = %.3f m\n", dL);
			RichEdit.AddText(sText);
			sText.Format("      ∴ T  =  30  ×  L  +  130 적용\n");
			RichEdit.AddText(sText);
			if(bUsePreStress)	sText.Format("      ② 최소두께 T  =  (30 ×  %.3f + 130) × 0.9  =  %0.1f mm이상\n", dL, dT );
			else				sText.Format("      ② 최소두께 T  =  30 ×  %.3f + 130  =  %.1f mm이상\n", dL, dT );
			RichEdit.AddText(sText);			
		}
		else
		{
			sText.Format("       L = %.3f m\n", dL);
			RichEdit.AddText(sText);
			sText.Format("      ∴ T  =  40  ×  L  +  130 적용\n");
			RichEdit.AddText(sText);
			if(bUsePreStress)	sText.Format("      ② 최소두께 T  =  (40 ×  %.3f + 130) × 0.9  =  %0.1f mm이상\n", dL, dT );
			else				sText.Format("      ② 최소두께 T  =  40 ×  %.3f + 130  =  %.1f mm이상\n", dL, dT );
			RichEdit.AddText(sText);	
		}

		sText.Format("         T = %.1f mm  %s   %.1f mm 사용 .... ", dT, sign2, dTB);
		RichEdit.AddText(sText);
		RichEdit.AddMsg(signDet, RE_RED, FALSE, TRUE);
		RichEdit.AddText("");

		if(bUsePreStress)
		{
			sText = "      바닥판의 지간방향으로 Prestress도입시 계산량의 90% 적용.     (☞ 도.설.기 4.7.4.2) \n";
			RichEdit.AddText(sText);
			sText = "      최소두께 계산값과 200 mm 중에서 큰값을 사용한다. (☞ 도.설.기 4.7.4.1) \n";
			RichEdit.AddText(sText);
		}
		else
		{			
			sText = "      최소두께 계산값과 220 mm 중에서 큰값을 사용한다. (☞ 도.설.기 3.6.1.5) \n";
			RichEdit.AddText(sText);
		}
	}
	else
	{
		sText.Format("       활하중이 없으므로 차도부 바닥판 최소두께 %.0fmm 이상을 사용\n",dLastCheck );
		RichEdit.AddText(sText);
	}
}

// 강선이 있을 경우 바닥판 최소 두께 설정
void CPlateStressOut::Output_SlabThickness_Tendon(CShRichEdit &RichEdit)
{	
	CString sText	= _T("");

	RichEdit.AddText("\n");
	sText.Format("   (1)	바닥판의 최소 두께                                                 ( ☞ 도.설.해 4.7.4.2 )\n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	sText.Format("     ①  본 교량은 장지간 바닥판에 프리스트레스를 도입한 교량으로서, 단지간 바닥판보다 두께가 두껍기\n");
	RichEdit.AddText(sText);

	sText.Format("         때문에 반복되는 트럭하중에 의한 펀칭 파괴가 발생하기 어렵다. 따라서 바닥판의 최소두께 검토시\n");
	RichEdit.AddText(sText);
	sText.Format("         설계하중에 대한 안전성 및 충격을 포함한 활하중에 대한 사용성을 만족하는 수준으로 바닥판 최소\n");
	RichEdit.AddText(sText);
	sText.Format("         두께를 결정한다.\n");
	RichEdit.AddText(sText);
	sText.Format("         ( 트럭 윤하중을 집중하중으로 모델링하여 해석하는 경우 하중재하 위치에서 휨 모멘트가 과대평가\n");
	RichEdit.AddText(sText);
	sText.Format("         되므로 정밀 해석시 트럭 윤하중의 접지면적을 고려하여 해석상 오류를 최소화 하여야 한다. )\n\n");
	RichEdit.AddText(sText);
	sText.Format("     ②  트럭 윤하중의 접지면적을 고려하는 방법은 '도.설.해 3.6.1.4 바닥판의 설계 휨모멘트'에서 윤하중 분포를\n");
	RichEdit.AddText(sText);
	sText.Format("         고려해서 바닥판에 작용하는 휨모멘트의 크기를 간략식을 이용하여 산정할 수 있으나, 트럭 윤하중의 접지면적을\n");
	RichEdit.AddText(sText);
	sText.Format("         고려한 정밀한 해석 방법을 사용하는 경우에는 간략식을 사용하지 않아도 된다. 정밀해석시 요구되는 트럭\n");
	RichEdit.AddText(sText);
	sText.Format("         윤하중의 접지면적은 '도.설.해 2.1.3 활하중'을 참조한다.\n");
	RichEdit.AddText(sText);
}

// 중앙부 경험적 설계
void CPlateStressOut::Output_RebarQtyExperienceCheck(CShRichEdit &RichEdit)
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CCalcData			*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcGeneral		CalcGeneral(m_pStd->GetDataManage());
	CPlateGirderApp		*pGirFir	= pDB->GetGirder(0);
	CPlateBxFinder		FinderStt(pGirFir);
	CPlateBasicIndex	*pBxFir		= FinderStt.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateGirderApp		*pGirEnd	= pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBxFinder		FinderEnd(pGirEnd);
	CPlateBasicIndex	*pBxEnd		= FinderEnd.GetBxFirst(BX_CROSSBEAM_VBRACING);

	CString szText	= _T("");
	double	dSlabThick				= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;//pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Req_T;	// 바닥판 최소 두께
	double	dLengthCen				= CalcGeneral.CalcCentPart_L();						// 중앙부 유효길이
	double	dLengthLeft				= pDB->m_dWidthSlabLeft - pGirFir->GetThickJewon(G_W, pBxFir)/2;	// 좌측 캔틸레버부 끝단에서 첫번째거더 좌측복부판 중앙까지 거리
	double	dLengthRight			= pDB->m_dWidthSlabRight - pGirEnd->GetThickJewon(G_W, pBxEnd)/2;	// 우측 캔틸레버부 끝단에서 마지막거더 우측복부판 중앙까지 거리
	double	dCoverUp				= pCalcData->m_dCoverUp;							// 철근덮게(상면)
	double	dCoverDn				= pCalcData->m_dCoverDn;							// 철근덮게(하면)
	double	dRate					= dLengthCen/dSlabThick;
	double	dMainDiaUpper_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy];	// 주철근 상면 Dia Idx 1-Cylce
	double	dMainDiaUpper_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy];	// 주철근 상면 Dia Idx 2-Cycle
	double	dMainCTCUpper			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1;					// 주철근 상면 간격
	double	dMainDiaLower_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1];			// 주철근 하면 Dia Idx 1-Cylce
	double	dMainDiaLower_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy];		// 주철근 하면 Dia Idx 2-Cycle
	double	dMainCTCLower			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1;						// 주철근 하면 간격
	double	dBaeRuckDiaUpper		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2];		// 배력철근 상면 Dia Idx
	double	dBaeRuckCTCUpper		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2;					// 배력철근 상면 간격
	double	dBaeRuckDiaLower		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3];			// 배력철근 하면 Dia Idx
	double	dBaeRuckCTCLower		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB;						// 배력철근 하면 간격
	double	dEndReinforceDiaMain_1Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2];			// 단부보강 주철근 Dia Idx 1-Cycle
	double	dEndReinforceDiaMain_2Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy];		// 단부보강 주철근 Dia Idx 2-Cycle
	double	dEndReinforceCTCMain	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// 단부보강 주철근 간격
	double	dEndReinforceDiaBeryuk	= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4];			// 단부보강 배력철근 Dia Idx
	double	dEndReinforceCTCBeryuk	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1;						// 단부보강 배력철근 간격
	double	dSlabArea				= 1000*dSlabThick;
	double	dArea					= 0;
	double	dArea_1Cy				= 0;
	double	dArea_2Cy				= 0;
	double	dAsreq					= 0;
	CString	sLeftGuard				= _T("");
	CString	sRightGuard				= _T("");
	CString	sRebar					= pCalcData->DESIGN_MATERIAL.m_sSigmaY;

	CCentSeparation	*pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::LEFT);
	if(pCentSeparation)	sLeftGuard	= _T("방호책 있음");
	else				sLeftGuard	= _T("방호책 없음");
	pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
	if(pCentSeparation)	sRightGuard	= _T("방호책 있음");
	else				sRightGuard	= _T("방호책 없음");

	szText.Format("    < 중앙부 슬래브 철근량 산정 >\n\n");
	RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	szText.Format("  (1) 설계조건\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("     바닥판 두께 : %.0f mm     유효지간 : %.0f mm     철근덮개(상면) : %.0f mm     철근덮개(하면) : %.0f mm\n\n",
					dSlabThick, dLengthCen, dCoverUp, dCoverDn);
	RichEdit.AddText(szText);
	szText.Format("     ① 지지부재 : 강재 주거더\n");
	RichEdit.AddText(szText);
	szText.Format("     ② 콘크리트가 현장타설되고 습윤양생되어야 함.\n");
	RichEdit.AddText(szText);
	szText.Format("     ③ 주거더 플랜지부의 헌치와 같이 국부적으로 두껍게 한 곳을 제외한 상태에서 전체적으로 바닥판의 두께가 일정함.\n");
	RichEdit.AddText(szText);
	szText.Format("     ④ 바닥판 두께에 대한 유효지간의 비가 6 이상 15 이하\n");
	RichEdit.AddText(szText);
	if(dRate >= 6 && dRate <= 15)
	{
		szText.Format("          6  <  %.3f  /  %.3f  =  %.3f  <  15", toM(dLengthCen), toM(dSlabThick), dRate);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else if(dRate < 6)
	{
		szText.Format("          6  >  %.3f  /  %.3f  =  %.3f", toM(dLengthCen), toM(dSlabThick), dRate);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	else
	{
		szText.Format("          %.3f  /  %.3f  =  %.3f  >  15", toM(dLengthCen), toM(dSlabThick), dRate);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	szText.Format("     ⑤ 바닥판의 상부와 하부에 배근된 철근의 외측면 사이의 두께가 150 mm 이상\n");
	RichEdit.AddText(szText);
	double dTemp = dMainDiaUpper_1Cy/2+(dSlabThick-dCoverUp-dCoverDn) + dMainDiaLower_1Cy/2;
	if(dTemp > 150)
	{
		szText.Format("          %.3f  +  %.3f  +  %.3f  =  %.3f  >  %.3f",	dMainDiaUpper_1Cy/2, dSlabThick-dCoverUp-dCoverDn, dMainDiaLower_1Cy/2, dTemp, 150.0);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("          %.3f  +  %.3f  +  %.3f  =  %.3f  <  %.3f", dMainDiaUpper_1Cy/2, dSlabThick-dCoverUp-dCoverDn, dMainDiaLower_1Cy/2, dTemp, 150.0);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	
	szText.Format("     ⑥ 유효지간이 표준차선폭 3.6m 이하\n");
	RichEdit.AddText(szText);
	if(dLengthCen < 3600.0)
	{
		szText.Format("          %.3f m  <  3.6 m", toM(dLengthCen));
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("          %.3f m  >  3.6 m", toM(dLengthCen));
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	szText.Format("     ⑦ 바닥판의 흠집, 마모면, 그리고 보호덮개층을 제외한 바닥판의 최소두께가 240 mm\n");
	RichEdit.AddText(szText);
	szText.Format("     ⑧ 캔틸레버부의 길이가 내측바닥판 두께의 5배 이상이거나, 캔틸레버부의 길이가 내측\n");
	RichEdit.AddText(szText);
	szText.Format("         바닥판 두께의 3배 이상이고 구조적으로 연속적인 콘크리트 방호책과 합성이 된 경우\n");
	RichEdit.AddText(szText);
	if(sLeftGuard == _T("방호책 있음"))
	{
		if(toM(dLengthLeft) > toM(dSlabThick)*3)
		{
			szText.Format("         ▷ 좌측 캔틸레버 길이 (%s) :  %.3f m  >  %.3f  ×  3  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         ▷ 좌측 캔틸레버 길이 (%s) :  %.3f m  <  %.3f  ×  3  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}
	else
	{
		if(toM(dLengthLeft) > toM(dSlabThick)*5)
		{
			szText.Format("         ▷ 좌측 캔틸레버 길이 (%s) :  %.3f m  >  %.3f  ×  5  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         ▷ 좌측 캔틸레버 길이 (%s) :  %.3f m  <  %.3f  ×  5  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}

	if(sRightGuard == _T("방호책 있음"))
	{
		if(toM(dLengthRight) > toM(dSlabThick)*3)
		{
			szText.Format("         ▷ 우측 캔틸레버 길이 (%s) :  %.3f m  >  %.3f  ×  3  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         ▷ 우측 캔틸레버 길이 (%s) :  %.3f m  <  %.3f  ×  3  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}
	else
	{
		if(toM(dLengthRight) > toM(dSlabThick)*5)
		{
			szText.Format("         ▷ 우측 캔틸레버 길이 (%s) :  %.3f m  >  %.3f  ×  5  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         ▷ 우측 캔틸레버 길이 (%s) :  %.3f m  <  %.3f  ×  5  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}

	szText.Format("     ⑨ 콘크리트의 28일 압축강도는 %.0f MPa 이상\n", pCalcData->DESIGN_MATERIAL.m_dSigmaCK);
	RichEdit.AddText(szText);
	szText.Format("     ⑩ 철근 콘크리트 바닥판은 바닥판을 지지하는 구조부재들과 완전합성거동\n\n");
	RichEdit.AddText(szText);
	szText.Format("  (2) 철근배근량\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("     ① 지간방향  :\n");
	RichEdit.AddText(szText);
	szText.Format("        하부철근량 - 콘크리트 바닥판 단면의 0.4%% 이상\n");
	RichEdit.AddText(szText);
	szText.Format("        상부철근량 - 콘크리트 바닥판 단면의 0.3%% 이상\n");
	RichEdit.AddText(szText);
	szText.Format("     ② 지간방향에 직각방향  :\n");
	RichEdit.AddText(szText);
	szText.Format("        하부철근량 - 콘크리트 바닥판 단면의 0.3%% 이상\n");
	RichEdit.AddText(szText);
	szText.Format("        상부철근량 - 콘크리트 바닥판 단면의 0.3%% 이상\n\n");
	RichEdit.AddText(szText);

	szText.Format("  (3) 사용철근량\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("      ① 일반부 주철근\n");
	RichEdit.AddText(szText);
	dArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1_1Cy;
	dArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1_2Cy;
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1;
	szText.Format("         상면철근     필요철근량  Asreq  :  %.0f  ×  0.3%%  =  %.3f  mm²\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f   mm²\n", sRebar, dMainDiaUpper_1Cy, dMainCTCUpper, dArea_1Cy);
	RichEdit.AddText(szText);
	szText.Format("                                                         %s%.0f  @ %.0f   =  %.3f   mm²\n", sRebar, dMainDiaUpper_2Cy, dMainCTCUpper, dArea_2Cy);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                                                      합계 %.3f mm²  >  Asreq ( %.3f mm² )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                                                      합계 %.3f mm²  >  Asreq ( %.3f mm² )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	dArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs_1Cy;
	dArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs_2Cy;
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asuse;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asreq;
	szText.Format("         하면철근     필요철근량  Asreq  :  %.0f  ×  0.4%%  =  %.3f  mm²\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f   mm²\n", sRebar, dMainDiaLower_1Cy, dMainCTCLower, dArea_1Cy);
	RichEdit.AddText(szText);
	szText.Format("                                                         %s%.0f  @ %.0f   =  %.3f   mm²\n", sRebar, dMainDiaLower_2Cy, dMainCTCLower, dArea_2Cy);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                                                      합계 %.3f mm²  >  Asreq ( %.3f mm² )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                                                      합계 %.3f mm²  >  Asreq ( %.3f mm² )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}

	szText.Format("        ② 일반부 배력철근\n");
	RichEdit.AddText(szText);
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse2;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1;
	szText.Format("         상면철근     필요철근량  Asreq  :  %.0f  ×  0.3%%  =  %.3f  mm²\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm²", sRebar, dBaeRuckDiaUpper, dBaeRuckCTCUpper, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm²", sRebar, dBaeRuckDiaUpper, dBaeRuckCTCUpper, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsuse;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsreq;
	szText.Format("         하면철근     필요철근량  Asreq  :  %.0f  ×  0.3%%  =  %.3f  mm²\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm²", sRebar, dBaeRuckDiaLower, dBaeRuckCTCLower, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm²", sRebar, dBaeRuckDiaLower, dBaeRuckCTCLower, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}

	szText.Format("        ③ 단부 주철근\n");
	RichEdit.AddText(szText);
	dArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_1Cy;
	dArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_2Cy;
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asreq*2;
	szText.Format("                          필요철근량  Asreq  :  %.0f  ×  2배  =  %.3f  mm²\n", dAsreq/2, dAsreq);
	RichEdit.AddText(szText);
	szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f   mm²\n", sRebar, dEndReinforceDiaMain_1Cy, dEndReinforceCTCMain, dArea_1Cy);
	RichEdit.AddText(szText);
	szText.Format("                                                         %s%.0f  @ %.0f   =  %.3f   mm²\n", sRebar, dEndReinforceDiaMain_2Cy, dEndReinforceCTCMain, dArea_2Cy);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                                                      합계 %.3f mm²  >  Asreq ( %.3f mm² )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                                                      합계 %.3f mm²  >  Asreq ( %.3f mm² )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}

	szText.Format("        ④ 단부 배력철근\n");
	RichEdit.AddText(szText);
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_BAsuse2;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsreq*2;
	szText.Format("                              필요철근량  Asreq  :  %.0f  ×  2배  =  %.3f  mm²\n", dAsreq/2, dAsreq);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm²", sRebar, dEndReinforceDiaBeryuk, dEndReinforceCTCBeryuk, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                          사용철근량  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm²", sRebar, dEndReinforceDiaBeryuk, dEndReinforceCTCBeryuk, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
}

void CPlateStressOut::Output_UltimateLoad(long nSide, CShRichEdit &RichEdit)
{
	CCalcData       *pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor		CalcFloor(m_pStd->GetDataManage());
	CMap <CString, LPCTSTR, double, double> MapStress;
	CalcFloor.SetCantileverUltimateLoad(nSide, MapStress);

	CString	strText			 = _T("");
	CString strGenTDia_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_strGenReinTDia_1Cy;
	CString strGenTDia_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_strGenReinTDia_2Cy;
	double	dGenTAsuse_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTAsuse_1Cy;
	double	dGenTAsuse_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTAsuse_1Cy;
	double	dGenTAsuse		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTAsuse;
	double	dGenTCTC		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTCTC;
	CString strGenCDia_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_strGenReinCDia_1Cy;
	CString strGenCDia_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_strGenReinCDia_2Cy;
	double	dGenCAsuse_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCAsuse_1Cy;
	double	dGenCAsuse_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCAsuse_1Cy;
	double	dGenCAsuse		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCAsuse;
	double	dGenCCTC		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCCTC;
	double	dFck			= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dFy				= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dB				= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_b;
	double	dH				= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Min_T;
	CString	strBDia			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CaseH3;
	double	dBCTC			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB;
	double	dAsuse			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsuse;
	double	dAsreq			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsreq;
	double	dL				= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Dead_L;
	CString	sRebar			= pCalcData->DESIGN_MATERIAL.m_sSigmaY;

	strText.Format("    (1) 일반부\n\n");		RichEdit.AddMsg(strText, RE_BLACK, FALSE, TRUE);
	strText.Format("      - 주철근\n");			RichEdit.AddText(strText);
	strText.Format("      1) 극한하중 Mu  =  %.3f kN·m\n", tokNM(MapStress["UltimateLoad"]));			RichEdit.AddText(strText);
	strText.Format("      2) 설계 휨강도       ( ☞ 도.설.기 4.6.3.5 )\n");								RichEdit.AddText(strText);
	strText.Format("         ① PS강재의 응력 : 부착된 부재  fps  =  %.3f MPa\n", MapStress["Fps"]);	RichEdit.AddText(strText);
	strText.Format("            인장 :   %s%s   @   %.0f   ⇒   %.3f mm²  (1 Cycle)\n", sRebar, strGenTDia_1Cy, dGenTCTC, dGenTAsuse_1Cy);		RichEdit.AddText(strText);
	strText.Format("                       %s%s   @   %.0f   ⇒   %.3f mm²  (2 Cycle)\n", sRebar, strGenTDia_2Cy, dGenTCTC, dGenTAsuse_2Cy);	RichEdit.AddText(strText);
	strText.Format("                                합계  : %.3f mm²\n", dGenTAsuse);	RichEdit.AddText(strText);
	strText.Format("            압축 :   %s%s   @   %.0f   ⇒   %.3f mm²  (1 Cycle)\n", sRebar, strGenCDia_1Cy, dGenCCTC, dGenCAsuse_1Cy);		RichEdit.AddText(strText);
	strText.Format("                       %s%s   @   %.0f   ⇒   %.3f mm²  (2 Cycle)\n", sRebar, strGenCDia_2Cy, dGenCCTC, dGenCAsuse_2Cy);	RichEdit.AddText(strText);
	strText.Format("                                합계  : %.3f mm²\n", dGenCAsuse);														RichEdit.AddText(strText);
	strText.Format("            인장철근비 ρ : %.6f      압축철근비 ρ' : %.6f\n", MapStress["Rate_Tension"], MapStress["Rate_Compre"]);	RichEdit.AddText(strText);
	strText.Format("         ② 플랜지를 갖는 단면 확인\n");			RichEdit.AddText(strText);
	strText.Format("            1.4 × dp × ρp × fps / fck  =  1.4 × %.0f × %.6f × %.3f / %.0f\n", MapStress["Depth_PS"], MapStress["Rho_PS"], MapStress["Fps"], dFck);
	RichEdit.AddText(strText);
	if(MapStress["FlangeCheck"]<MapStress["Slab_Thick"])
		strText.Format("            = %.3f mm  <  %.3f mm   이므로 구형단면으로 검토\n", MapStress["FlangeCheck"], MapStress["Slab_Thick"]);
	else
		strText.Format("            = %.3f mm  >  %.3f mm   이므로 플랜지를 갖는 단면으로 검토\n", MapStress["FlangeCheck"], MapStress["Slab_Thick"]);
	RichEdit.AddText(strText);
	strText.Format("         ③ 휨강도\n");													RichEdit.AddText(strText);
	strText.Format("            ΦMn  =  Φ × { Ap×Fps×dp×(1-0.59×Pp×fps/fck) }\n");	RichEdit.AddText(strText);
	if(MapStress["Bend_Mn"] > MapStress["UltimateLoad"]){
		strText.Format("                   =  %.3f kN.m > Mu = %.3f kN.m", tokNM(MapStress["Bend_Mn"]), tokNM(MapStress["UltimateLoad"]));
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, TRUE, 1);
	}
	else{
		strText.Format("                   =  %.3f kN.m < Mu = %.3f kN.m", tokNM(MapStress["Bend_Mn"]), tokNM(MapStress["UltimateLoad"]));
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, FALSE, 1);
	}	
	strText.Format("         ④ 최대 강재비\n");	RichEdit.AddText(strText);
	if(MapStress["MaxSteelRate"]<MapStress["MinQp"]){
		strText.Format("            qp  =  Pp × ( fps / fck ) = %.3f < 0.36·β1 = %.3f", MapStress["MaxSteelRate"], MapStress["MinQp"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, TRUE, 1);
	}
	else{
		strText.Format("            qp  =  Pp × ( fps / fck ) = %.3f > 0.36·β1 = %.3f", MapStress["MaxSteelRate"], MapStress["MinQp"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, FALSE, 1);
		}	
	strText.Format("         ⑤ 최소 강재량\n");										RichEdit.AddText(strText);
	strText.Format("            ·균열 모멘트 Mcr = %.3f kN.m\n", MapStress["Mcr"]);	RichEdit.AddText(strText);
	if(MapStress["Bend_Mn"]/MapStress["Mcr"]>1.2){
		strText.Format("               ΦMn/Mcr = %.3f / %.3f = %.3f > 1.2", tokNM(MapStress["Bend_Mn"]), tokNM(MapStress["Mcr"]), MapStress["Bend_Mn"]/MapStress["Mcr"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, TRUE, 2);
	}
	else{
		strText.Format("               ΦMn/Mcr = %.3f / %.3f = %.3f < 1.2", tokNM(MapStress["Bend_Mn"]), tokNM(MapStress["Mcr"]), MapStress["Bend_Mn"]/MapStress["Mcr"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, FALSE, 2);
	}	
	strText.Format("      - 배력철근\n");							RichEdit.AddText(strText);
	strText.Format("         ① [도로교설계기준] 113Page\n");		RichEdit.AddText(strText);
	strText.Format("           ⇒ 주철근이 차량진행방향에 직각일 경우 120/√L 과 67%%중 작은값 이상 사용\n");	RichEdit.AddText(strText);
	
	//MapStress["Dead-L"] 사용을 하지 않고  XLDesignFloor의 공식을 이용해 L 값을 구함. LSH(2009.03.26)
	double dRate		= RoundUp(120/sqrt(dL/1000),3);
	double dMapStress	= RoundUp(min(dRate,67.0)/100,3);

	strText.Format("              주철근에 대한 백분율 : 120 / √L = 120 / √%.3f = %.3f %% < 67%%\n", toM(dL), dRate);
	RichEdit.AddText(strText);
	
	//LSH -추가
	strText.Format("				As × %.2f = %.0f × %.2f = %.3f mm²\n", dMapStress, dGenTAsuse, dMapStress,dGenTAsuse*dMapStress);
	RichEdit.AddText(strText);
	strText.Format("         ② [콘크리트설계기준] 5.7.2.1\n");		RichEdit.AddText(strText);
	strText.Format("           ⇒ As = 0.002·b·h = 0.002 × %.3f × %.3f = %.3f mm²\n", dB, dH, 0.002*dB*dH);	RichEdit.AddText(strText);
	strText.Format("         ③ 사용 철근량\n");					RichEdit.AddText(strText);
	strText.Format("            필요철근  : %.3f mm²\n", max(dAsreq,dGenTAsuse*dMapStress));	RichEdit.AddText(strText);
	if(dAsuse>dAsreq){
		strText.Format("            사용철근  :  %s%s   @   %.0f   ⇒   %.3f mm² > Asreq", sRebar, strBDia, dBCTC, dAsuse);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, TRUE, 2);
	}
	else{
		strText.Format("            사용철근  :  %s%s   @   %.0f   ⇒   %.3f mm² < Asreq", sRebar, strBDia, dBCTC, dAsuse);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, FALSE, 2);
	}	

	if(nSide==FLOOR_MID)
	{
		CMap<CString, LPCTSTR, double, double> MapCen;
		CalcFloor.DesignExpansionJointUltimateLoad(MapCen);
		double	dPib			= pCalcData->m_Pib;
		double	dPis			= pCalcData->m_Pis;
		double	dDMainCTC		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// 단부 주철근
		CString	strDanDia_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2;
		CString	strDanDia_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2_2Cy;
		double	dDanArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_1Cy;
		double	dDanArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_2Cy;
		double	dDanArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2;
		strText.Format("    (2) 지점캔틸레버부(신축이음부)\n\n");		RichEdit.AddMsg(strText, RE_BLACK, FALSE, TRUE);
		strText.Format("      - 주철근\n");		RichEdit.AddText(strText);
		strText.Format("        · 신축이음 :  %.0f × %.0f    · 슬래브두께 : %.3f mm    · 모멘트 팔길이 : %.3f mm\n",
			MapCen["WidthExp"], MapCen["HeightExp"], MapCen["ThickSlab"], MapCen["LengthArm"]);
		RichEdit.AddText(strText);
		strText.Format("      1) 극한하중 산정\n");							RichEdit.AddText(strText);
		strText.Format("         ① 고정하중\n");							RichEdit.AddText(strText);
		strText.Format("            P   =  %.3f kN\n", MapCen["DeadLoad"]);	RichEdit.AddText(strText);
		strText.Format("            Md =  %.3f × %.3f/2 = %.3f kN.m\n",
			MapCen["DeadLoad"], toM(MapCen["LengthArm"]), tokN(MapCen["Md"]));	RichEdit.AddText(strText);
		strText.Format("         ② 활하중\n");								RichEdit.AddText(strText);
		strText.Format("           · 충격계수 i  =  %.3f\n", MapCen["CollCoeff"]);	RichEdit.AddText(strText);
		strText.Format("           · 차량하중분포폭\n");							RichEdit.AddText(strText);
		strText.Format("              E  = 0.35X + 0.98  = 0.35 × ( %.3f - 0.1 ) + 0.98 = %.3f m\n", toM(MapCen["LengthArm"]), toM(MapCen["E"]));
		RichEdit.AddText(strText);
		strText.Format("           · 폭 1m 당 휨모멘트\n");				RichEdit.AddText(strText);
		strText.Format("              Ml = %.1f × ( %.3f - 0.1 ) × (1+%.3f) / %.3f = %.3f kN.m\n",
			tokN(MapCen["Pr"]), toM(MapCen["LengthArm"]), MapCen["CollCoeff"], toM(MapCen["E"]), tokN(MapCen["Ml"]));RichEdit.AddText(strText);
		strText.Format("         ③ 하중조합\n");							RichEdit.AddText(strText);
		strText.Format("              Mu = 1.3Md + 2.15Ml = 1.3 × %.3f + 2.15 × %.3f = %.3f kN.m\n\n",
			tokN(MapCen["Md"]), tokN(MapCen["Ml"]), tokN(MapCen["Mu"]));	RichEdit.AddText(strText);
		strText.Format("      2) 단면설계\n");								RichEdit.AddText(strText);
		strText.Format("         ① 필요철근량검토\n");						RichEdit.AddText(strText);
		strText.Format("             Mu = %.3f kN.m    fck = %.1f MPa    fy = %.1f MPa    B = %.1f mm\n", tokN(MapCen["Mu"]), dFck, dFy, MapCen["B"]);
		RichEdit.AddText(strText);
		strText.Format("             d  = %.1f mm         d' = %.1f mm     φf = %.2f        φv = %.1f\n", MapCen["D"], MapCen["Cover"], dPib, dPis);
		RichEdit.AddText(strText);
		strText.Format("             Mu /φ = As × fy × (d - a/2)     ...... (1)\n");	RichEdit.AddText(strText);
		strText.Format("             a = As × fy / (0.85 × fck × b)  ...... (2)\n");	RichEdit.AddText(strText);
		strText.Format("             필요철근량  Asreq  :  %.3f mm²\n", MapCen["Asreq"]);		RichEdit.AddText(strText);
		strText.Format("             사용철근량  Asuse  :  %s%s  @ %.0f   =  %.3f   mm² (1Cycle)\n", sRebar, strDanDia_1Cy, dDMainCTC, dDanArea_1Cy);RichEdit.AddText(strText);
		strText.Format("                                            %s%s  @ %.0f   =  %.3f   mm²  (2Cycle)\n", sRebar, strDanDia_2Cy, dDMainCTC, dDanArea_2Cy);RichEdit.AddText(strText);
		if(dDanArea>MapCen["Asreq"]){
			strText.Format("                                    합계  :  %.3f mm² >  Asreq", dDanArea);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 1);
		}
		else{
			strText.Format("                                    합계  :  %.3f mm² <  Asreq", dDanArea);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 1);
		}		
		strText.Format("         ② 철근비검토\n");														RichEdit.AddText(strText);
		strText.Format("            Asmin  =  0.8 √fck × b × d/fy = %.3f mm²\n", MapCen["Asmin1"]);	RichEdit.AddText(strText);
		strText.Format("            Asmin  =  14 × b × d / fy = %.3f mm²\n", MapCen["Asmin2"]);		RichEdit.AddText(strText);
		strText.Format("            Asmin  =  Asreq × 4/3 = %.3f mm²\n", MapCen["Asmin3"]);			RichEdit.AddText(strText);
		strText.Format("                ∴ Asmin = %.3f mm²\n", MapCen["Asmin"]);						RichEdit.AddText(strText);
		strText.Format("             Pmax  =  0.75 × Pb = 0.75 × 0.85 × k1 × fck/fy × 600 / (600 + fy)\n");RichEdit.AddText(strText);
		strText.Format("                       =  %.3f\n", MapCen["Pmax"]);								RichEdit.AddText(strText);
		strText.Format("            Asmax  = Pmax × b × d = %.3f mm²\n", MapCen["Asmax"]);			RichEdit.AddText(strText);
		if(dDanArea>MapCen["Asmin"] && dDanArea<MapCen["Asmax"]){
			strText.Format("                 ∴ Asmin  ≤  Asuse  <  Asmax");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 2);			
		}
		else if(dDanArea>MapCen["Asmax"]){
			strText.Format("                 ∴ Asuse  >  Asmax");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 2);			
		}
		else{
			strText.Format("                 ∴ Asuse  <  Asmin");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 2);			
		}		
		strText.Format("      3) 휨에대한 검토\n");														RichEdit.AddText(strText);
		strText.Format("           a  = As × fy / ( 0.85 × fck × b ) = %.3f mm\n", MapCen["A"]);			RichEdit.AddText(strText);
		strText.Format("       φMn = φ × 0.85 × As × fy × (d - a/2) = %.3f kN.m\n", tokNM(MapCen["Mn"]));	RichEdit.AddText(strText);
		if(MapCen["Mn"]>MapCen["Mu"]){
			strText.Format("             ∴ φMn  ≥  Mu");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 1);			
		}
		else{
			strText.Format("             ∴ φMn  <  Mu");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 1);			
		}		
	}
	else
	{
		double	dMl			= MapStress["D_LiveLoad"];
		double	dPreLoad	= MapStress["D_PreLoad"];
		double	dPostLoad	= MapStress["D_PostLoad"];
		double	dDanMu		= MapStress["D_Mu"];
		double	dDnaPe		= MapStress["D_Pe"];
		double	dDanUpper	= MapStress["D_Upper"];
		double	dDanLower	= MapStress["D_Lower"];
		double	dSlabThick	= MapStress["Slab_Thick"];
		double	dDanX		= MapStress["D_X"];
		double	dDanT		= MapStress["D_T"];
		double	dDanAsreq	= MapStress["D_As"];
		double	dMe			= MapStress["D_Me"];
		double	dFsa		= pCalcData->DESIGN_MATERIAL.m_dSigmaSA;
		double	dDanAsuse	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2;
		CString	strDia_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CaseH2;
		CString	strDia_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CaseH2_2Cy;
		double	dMainCTC	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Ctc2;
		double	dMainAs_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2_1Cy;
		double	dMainAs_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2_2Cy;
		double	dBaeAsuse	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsuse2;
		CString	strBaeDia	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CaseH4;
		double	dBaeCTC		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB1;
		double	dDanBAsreq	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAsBr;
		strText.Format("    (2) 단부\n\n");		RichEdit.AddMsg(strText, RE_BLACK, FALSE, TRUE);
		strText.Format("      - 주철근\n");		RichEdit.AddText(strText);
		strText.Format("        ☞ 들보 단부의 차도부분 바닥판은 연속성의 단절로 인한 휨모멘트의 증가 및 \n");					RichEdit.AddText(strText);
		strText.Format("           신축이음장치로 인한 차량하중의 충격이 작용하므로 이에 대한 보강을 하여야 한다.\n");			RichEdit.AddText(strText);
		strText.Format("             Mu  =  2Ml + Md  = 2 × %.3f + %.3f + %.3f = %.3f kN.m\n", tokNM(dMl), tokNM(dPreLoad), tokNM(dPostLoad), tokNM(dDanMu));
		RichEdit.AddText(strText);
		strText.Format("        · 설계하중 작용시 유효프리스트레스 긴장력 Pe = %.3f kN\n", tokN(dDnaPe));						RichEdit.AddText(strText);
		strText.Format("        · 설계하중 작용시 유효프리스트레스 모멘트 Me = %.3f kN.m\n", tokNM(dMe));						RichEdit.AddText(strText);
		strText.Format("           상연응력  :  %.3f MPa      하연응력  : %.3f MPa\n", dDanUpper, dDanLower);					RichEdit.AddText(strText);
		strText.Format("           사용철근  :  %s%s   @   %.0f   ⇒   %.3f mm² (1 Cycle)\n", sRebar, strDia_1Cy, dMainCTC, dMainAs_1Cy);	RichEdit.AddText(strText);
		strText.Format("                          %s%s   @   %.0f   ⇒   %.3f mm² (2 Cycle)\n", sRebar, strDia_2Cy, dMainCTC, dMainAs_2Cy);	RichEdit.AddText(strText);
		strText.Format("                                        합계 : %.3f mm²\n", dDanAsuse);									RichEdit.AddText(strText);
		strText.Format("            x = %.3f × %.3f / ( %.3f + %.3f ) = %.3f mm\n", fabs(dDanUpper), dSlabThick, fabs(dDanUpper), fabs(dDanLower), dDanX);
		RichEdit.AddText(strText);
		strText.Format("            T = %.3f × %.3f × 1/2 = %.3f N\n", fabs(dDanUpper), dDanX, dDanT);						RichEdit.AddText(strText);
		if(dDanAsuse>dDanAsreq){
			strText.Format("            Asreq = %.3f / %.3f = %.3f mm²  <  Asuse", dDanT, dFsa, dDanAsreq);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 2);			
		}
		else{
			strText.Format("            Asreq = %.3f / %.3f = %.3f mm²  >  Asuse", dDanT, dFsa, dDanAsreq);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 2);
		}		
		strText.Format("      - 배력철근\n");		RichEdit.AddText(strText);
		strText.Format("           필요철근  :  Asreq  =  %.3f × 2  =  %.3f mm²\n", dAsreq, dDanBAsreq);		RichEdit.AddText(strText);
		if(dBaeAsuse>dDanBAsreq){
			strText.Format("           사용철근  :  %s%s   @   %.0f   ⇒   %.3f mm² >  Asreq", sRebar, strBaeDia, dBaeCTC, dBaeAsuse);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 1);
		}
		else{
			strText.Format("           사용철근  :  %s%s   @   %.0f   ⇒   %.3f mm² <  Asreq", sRebar, strBaeDia, dBaeCTC, dBaeAsuse);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 1);
		}		
	}
}

void CPlateStressOut::SeekFlange_Web(double& dFlange, double& dWeb)
{
	dFlange = dWeb = 0;
	
	__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;
	CPlateBridgeApp		*pDB = m_pStd->GetBridge();;
	CPlateBxFinder		Finder(pDB->GetGirder(0));
	CPlateBasicIndex	*pIndex = Finder.GetBxFirst(bxFlag);
	
	while(Finder.GetBxFixNext())
	{
		double dFlangeLen = pIndex->GetFactChain(G_F_U)->m_dWidth;
		
		if (dFlange<dFlangeLen)
		{
			dFlange = dFlangeLen;
			double dWebThick = pIndex->GetFactChain(G_W)->m_dFactChainThick;
			dWeb = (dWeb<dWebThick)? dWebThick : dWeb;
		}
		pIndex = Finder.GetBxNext();
	}
}

// 극한하중에 대한 단면검토
void CPlateStressOut::Output_UltimateLoadCheck(long nSide, CShRichEdit &RichEdit)
{
	CPlateBridgeApp		*pDB			= m_pStd->GetBridge();
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pStd->m_pCalcStd;
	
	CString szText(_T(""));
	CString szSign(_T(""));
	CString szOk(_T(""));
	CString szStress(_T(""));
	
	CCalcFloor			CalcFloor(pDataManage);
	CMap <CString, LPCTSTR, double, double> Map;
	CalcFloor.CalcCantileverUltimateLoadCheck(pCalcStd, Map, nSide);

	// 위치
	szText = (nSide==0)? _T("    좌측 캔틸레버부") : (nSide==1)? _T("    우측 캔틸레버부") : _T("    슬래브 중앙부");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText = (nSide==0 && pDB->m_nTendonFix<1)? _T(" (고정단)") : (nSide==0 && pDB->m_nTendonFix==1)?  _T(" (긴장단)") : _T(""); 
	szText += (nSide==1 && pDB->m_nTendonFix<1)? _T(" (긴장단)") : (nSide==1 && pDB->m_nTendonFix==1)?  _T(" (고정단)") : _T("");
	szText += _T("\n\n");
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ① 부착된 부재의 PS 강재의 응력 (fps)
	double dBar_Dia1 = Map["Bar_Dia1"];
	double dBar_Spa1 = Map["Bar_Spa1"];
	double dBar_Are1 = Map["Bar_Are1"];
	double dBar_Dia2 = Map["Bar_Dia2"];
	double dBar_Spa2 = Map["Bar_Spa2"];
	double dBar_Are2 = Map["Bar_Are2"];
	double dBar_Area = Map["Bar_Area"];
	double dFps		 = Map["Fps"];

	szText = _T("    ① 부착된 부재의 PS 강재의 응력 (fps)           ( ☞ 도.설 4.4.3.1, 도.설 4.6.3.5, 도.설 4.6.3.6 ) \n");
	RichEdit.AddText(szText);
	szText.Format("        As    =    H %.0f  @  %.0f    =    %.3f    mm²    (1Cycle) \n", dBar_Dia1, dBar_Spa1, dBar_Are1);
	RichEdit.AddText(szText);
	szText.Format("        As    =    H %.0f  @  %.0f    =    %.3f    mm²    (2Cycle) \n", dBar_Dia2, dBar_Spa2, dBar_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                    ∑    =    %.3f    mm² \n", dBar_Area);
	RichEdit.AddText(szText);
	szText.Format("        fps    =    %.3f    MPa \n\n", dFps);
	RichEdit.AddText(szText);

	// ② 극한강도 검토
	double dPiMn1	= Map["Mn1"];
	double dPiMn2	= Map["Mn2"];
	double dQp		= Map["Qp"];
	double dBeta	= Map["Beta"];
	double dMcr		= Map["Mcr"];
	double dMu		= Map["Mu"];

 	if (dPiMn1<dMu)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
 	if (dPiMn1>dMu)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }

	szText = _T("    ② 극한강도 검토 \n");
	RichEdit.AddText(szText);
	szText = _T("    - 사각형 단면 또는 중립축이 압축플랜지 내에 있는 플랜지를 갖는 단면에 대한 설계휨강도 \n");
	RichEdit.AddText(szText);
	szText.Format("    ∴ ΦMn    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dPiMn1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    Mu    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dMu);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	if (dPiMn2<dMu)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dPiMn2>dMu)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }

	szText = _T("    - 인장측에 철근을 배치하지 않았거나 배치했더라도 그 영향을 무시할 수 있는 단면에 대한 설계휨강도 \n");
	RichEdit.AddText(szText);
	szText.Format("    ∴ ΦMn    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dPiMn2);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    Mu    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dMu);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ③ 최대 강재비
	if (dQp<0.36*dBeta)	{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (dQp>0.36*dBeta)	{ szSign=_T("＞"); szOk=_T("∴ N.G"); }

	szText = _T("    ③ 최대 강재비 \n");
	RichEdit.AddText(szText);
	szText.Format("    ∴ qp    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    ", dQp);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    0.36β₁    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    ", 0.36*dBeta);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);	
	szText.Format("%s    \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ④ 최소 강재량
	double dMn = min(dPiMn1, dPiMn2);
	if (dMn<1.2*dMcr)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dMn>1.2*dMcr)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }

	szText = _T("    ④ 최소 강재량 \n");
	RichEdit.AddText(szText);
	szText = _T("    - PS강재 및 철근의 총량은 위험단면에서 균열모멘트의 1.2배 이상의 계수모멘트를 받을 수 있어야 한다. \n");
	RichEdit.AddText(szText);
	szText.Format("    ∴ ΦMn    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dMn);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    1.2Mcr    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", 1.2*dMcr);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ⑤ 바닥판 배력근의 철근량 산정
	double dLength	 = Map["Length"];
	double dSlab_Th  = Map["Slab_Th"];
	double dSup_Rat1 = Map["Sup_Rat1"];
	double dSup_Rat2 = Map["Sup_Rat2"];
	double dSup_Req1 = Map["Sup_Req1"];
	double dSup_Req2 = Map["Sup_Req2"];
	double dSup_Req3 = Map["Sup_Req3"];
	double dSup_Req4 = Map["Sup_Req4"];
	double dSup_Min  = Map["Sup_Min"];	
	double dSup_Dia1 = Map["Sup_Dia1"]; 
	double dSup_Spa1 = Map["Sup_Spa1"];
	double dSup_Are1 = Map["Sup_Are1"];
	double dSup_Area1= Map["Sup_Area1"];
	double dSup_Dia2 = Map["Sup_Dia2"];
	double dSup_Spa2 = Map["Sup_Spa2"];
	double dSup_Are2 = Map["Sup_Are2"];
	double dSup_Area2= Map["Sup_Area2"];

	szText = _T("    ⑤ 바닥판 배력근의 철근량 산정 \n");
	RichEdit.AddText(szText);
	szText = _T("    - 주철근이 차량 진행방향에 직각일 경우 120/√L 과 67% 중 작은 값 이상 사용 \n");
	RichEdit.AddText(szText);

	if (dSup_Rat1>0.67)		{ szSign=_T("＞"); szOk.Format("그러므로 %.1f%% 를 적용", dSup_Rat2*100); }
	if (dSup_Rat1<=0.67)	{ szSign=_T("＜"); szOk.Format("그러므로 %.1f%% 를 적용", dSup_Rat2*100); }
	szText.Format("       120 / √L   =   120 / √(%.3f)   =   %.3f %%   %s 67.0 %%,   %s \n", dLength, dSup_Rat1*100, szSign, szOk);
	RichEdit.AddText(szText);

	szText.Format("        횡방향철근    =    H %.0f  @  %.0f    =    %.3f    mm²    (1Cycle) \n", dBar_Dia1, dBar_Spa1, dBar_Are1);
	RichEdit.AddText(szText);
	szText.Format("        횡방향철근    =    H %.0f  @  %.0f    =    %.3f    mm²    (2Cycle) \n", dBar_Dia2, dBar_Spa2, dBar_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                    ∑    =    %.3f    mm² \n", dBar_Area);
	RichEdit.AddText(szText);
	szText.Format("        Req As    =   %.2f  ×   %.3f   =   %.3f   mm² \n", dSup_Rat2, dBar_Area, dSup_Req1);
	RichEdit.AddText(szText);
	szText.Format("        최소철근량   =   0.002 × b × h   =    0.002    ×   1,000.000   ×   %.3f   =    %.3f   mm² \n", 
											dSlab_Th,  dSup_Min);
	RichEdit.AddText(szText);

	// 사용 철근량 검토
	if (dBar_Area<dSup_Req1)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dBar_Area>=dSup_Req1)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }
	szText = _T("    - 사용 철근량 검토 \n");
	RichEdit.AddText(szText);
	szText.Format("        횡방향 철근   =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²   ", dBar_Area);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm²   ", dSup_Req1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	
	if (dSup_Min<dSup_Req1)		{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dSup_Min>=dSup_Req1)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }
	szText.Format("        최소철근량    =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²   ", dSup_Min);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm²   ", dSup_Req1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	

	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm²    (1Cycle) \n", dSup_Dia1, dSup_Spa1, dSup_Are1);
	RichEdit.AddText(szText);
	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm²    (2Cycle) \n", dSup_Dia1, dSup_Spa1, dSup_Are1);
	RichEdit.AddText(szText);
	szText.Format("                                    ∑    =    %.3f    mm² \n", dSup_Area1);
	RichEdit.AddText(szText);

	if (dSup_Area1<dSup_Req2)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dSup_Area1>=dSup_Req2)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }
 	szText.Format("        ∴ Use As    =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²   ", dSup_Area1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm²   ", dSup_Req2);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 단부 사용 철근량 검토
	szText = _T("    - 단부 사용 철근량 검토 \n");
	RichEdit.AddText(szText);
	szText.Format("        Req As    =    %.2f    ×   2   =    %.3f    mm² \n", dSup_Req3, dSup_Req4);
	RichEdit.AddText(szText);
	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm²    (1Cycle) \n", dSup_Dia2, dSup_Spa2, dSup_Are2);
	RichEdit.AddText(szText);
	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm²    (2Cycle) \n", dSup_Dia2, dSup_Spa2, dSup_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                    ∑    =    %.3f    mm² \n", dSup_Area2);
	RichEdit.AddText(szText);

	if (dSup_Area2<dSup_Req4)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dSup_Area2>=dSup_Req4)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }
	szText.Format("        ∴ Use As    =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²   ", dSup_Area2);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm²   ", dSup_Req4);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
}


///////////////////////////////////////////////////////////////////////////////////
//∫∴∵∮∑∏ㄷ ΑΒΓΔΕΖΗαβγδεζηΣθιΙκλμνξοπρf υφχψω
//	´≤＞＜≠±≤≥∞⌒×㎝⁴ ㎤ ㎠ ㎝ kgf/㎠ t·m  ¹²³⁴₁₂₃₄
//  ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩ ⑪ ⑫ ⑬ ⑭ ⑮ √½⅓㉮㉯㉰㉱㉲㉳㉴☞Φ
//  ━━━━━━━━━━━━━  ⇒ ⊙
//  ┏ ┗ ┛ ┓┃━ ┣ ┫ⅰ ⅱ ⅲ ⅳ ⅴ ⅵ ⅶ ⅷ ⅸ ⅹ Ⅰ Ⅱ Ⅲ Ⅳ Ⅳ Ⅴ Ⅵ
///////////////////////////////////////////////////////////////////////////////////// 
void CPlateStressOut::Output_FixingPlate(CShRichEdit &RichEdit)
{
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	
	CString szText(_T(""));
	CString szSign(_T(""));
	CString szOk(_T(""));
	CString szStress(_T(""));
	
	CCalcFloor			CalcFloor(pDataManage);
	CMap <CString, LPCTSTR, double, double> Map;
	CalcFloor.CalcCantileverFixedCheck(Map);

	// 위치
	szText = _T("    정착부 보강 \n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);

	// 정착부 입력 제원
	double dFix_Qty		=Map["Fix_Qty"];
	double dFix_Width	=Map["Fix_Width"];
	double dFix_Height	=Map["Fix_Height"];
	double dFix_Space	=Map["Fix_Space"];
	double dEff_Space	=Map["Eff_Space"];
	double dSlab_Thk	=Map["Slab_Thk"];
	double dTdn_Str		=Map["Tdn_Str"];
	double dTdn_Area	=Map["Tdn_Area"];
	double dTdn_Pu		=Map["Tdn_Pu"];

	szText = _T("    ㆍ정착부 입력 제원 \n");
	RichEdit.AddText(szText);
	szText.Format("       정착판당 정착구 =	%.3f ea			슬래브 두께(t) =	%.3f mm \n", dFix_Qty, dSlab_Thk);
	RichEdit.AddText(szText);
	szText.Format("       정착판 폭원(a) =	%.3f mm		정착구 간격 =	%.3f mm \n", dFix_Width, dFix_Space);
	RichEdit.AddText(szText);
	szText.Format("       정착판 높이(b) =	%.3f mm		유효 지압폭 =	%.3f mm \n", dFix_Height, dEff_Space);
	RichEdit.AddText(szText);
	szText.Format("       최대 긴장응력 =	%.3f MPa		강연선 단면적 =	%.3f mm² \n", dTdn_Str, dTdn_Area);
	RichEdit.AddText(szText);
	szText.Format("       강연선 긴장력(Pu) =	%.3f kN \n\n", dTdn_Pu);
	RichEdit.AddText(szText);

	// ① 횡방향 파열력에 대한 보강 철근량 산정
	double dAH				= Map["dAH"];		
	double dSH				= Map["dSH"];		
	double dT2H				= Map["T2H"];		
	double dFsa				= Map["Fsa"];		
	double dReq_AsT2H		= Map["Req_AsT2H"];
	double dDia_T2H			= Map["Dia_T2H"];	
	double dQty_T2H			= Map["Qty_T2H"];	
	double dUse_AsT2H		= Map["Use_AsT2H"];

	if (dUse_AsT2H < dReq_AsT2H)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dUse_AsT2H > dReq_AsT2H)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }

	szText = _T("    ① 횡방향 파열력에 대한 보강 철근량 산정		( ☞ 도.설 4.6.3.9 (3)⑦(다) ) \n");
	RichEdit.AddText(szText);
	szText = _T("       파열력(T2)	=	0.20 x Pu x ( 1 - a / s ) \n");
	RichEdit.AddText(szText);
	szText.Format("		=	0.20 × %.3f ×( 1 - %.1f / %.1f )	=	%.3f	kN \n", dTdn_Pu, dAH, dSH, dT2H);
	RichEdit.AddText(szText);
	szText = _T("       철근량 검토 \n");
	RichEdit.AddText(szText);
	szText.Format("       Req As	=	T₂/ fsa	=	%.3f / %.1f	=	%.3f mm² \n", dT2H*1000, dFsa, dReq_AsT2H);
	RichEdit.AddText(szText);
	szText.Format("       Use As	=	H %.0f × %.0f    =    ", dDia_T2H, dQty_T2H);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²	", dUse_AsT2H);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	Req As	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²	", dReq_AsT2H);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ② 수직방향 파열력에 대한 보강철근량 산정
	double dAV			= Map["dAV"];		
	double dSV			= Map["dSV"];		
	double dT2V			= Map["T2V"];		
	double dReq_AsT2V	= Map["Req_AsT2V"];
	double dDia_T2V		= Map["Dia_T2V"];	
	double dQty_T2V		= Map["Qty_T2V"];	
	double dUse_AsT2V	= Map["Use_AsT2V"];

	if (dUse_AsT2V < dReq_AsT2V)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dUse_AsT2V > dReq_AsT2V)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }
	
	szText = _T("    ② 수직방향 파열력에 대한 보강철근량 산정		( ☞ 도.설 4.6.3.9 (3)⑦(다) ) \n");
	RichEdit.AddText(szText);
	szText = _T("       파열력(T2)	=	0.20 x Pu x ( 1 - a / s ) \n");
	RichEdit.AddText(szText);
	szText.Format("		=	0.20 × %.3f ×( 1 - %.1f / %.1f )	=	%.3f	kN \n", dTdn_Pu, dAV, dSV, dT2V);
	RichEdit.AddText(szText);
	szText = _T("       철근량 검토 \n");
	RichEdit.AddText(szText);
	szText.Format("       Req As	=	T₂/ fsa	=	%.3f / %.1f	=	%.3f mm² \n", dT2V*1000, dFsa, dReq_AsT2V);
	RichEdit.AddText(szText);
	szText.Format("       Use As	=	H %.0f × %.0f    =    ", dDia_T2V, dQty_T2V);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²	", dUse_AsT2V);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	Req As	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²	", dReq_AsT2V);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ③ 단부 인장력에 대한 보강철근량 산정
	double dT1			= Map["T1"];
	double dReq_AsT1	= Map["Req_AsT1"];
	double dDia_T1		= Map["Dia_T1"];
	double dQty_T1		= Map["Qty_T1"];
	double dUse_AsT1	= Map["Use_AsT1"];

	szText = _T("    ③ 단부 인장력에 대한 보강철근량 산정		( ☞ 도.설 4.6.3.9 (3)⑦(다) ) \n");
	RichEdit.AddText(szText);
	szText = _T("       파열력(T1)	=	0.20 x Pu x ( 1 - a / s ) \n");
	RichEdit.AddText(szText);
	szText.Format("		=	0.20 × %.3f ×( 1 - %.1f / %.1f )	=	%.3f	kN \n", dTdn_Pu, dAH, dSH, dT1);
	RichEdit.AddText(szText);
	szText = _T("       철근량 검토 \n");
	RichEdit.AddText(szText);
	szText.Format("       Req As	=	T₁/ fsa	=	%.3f / %.1f	=	%.3f mm² \n", dT1*1000, dFsa, dReq_AsT1);
	RichEdit.AddText(szText);
	szText.Format("       Use As	=	H %.0f × %.0f    =    ", dDia_T1, dQty_T1);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²	", dUse_AsT1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	Req As	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm²	", dReq_AsT1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	
	// ④ 정착구 지압응력 검토	
	double dFb	= Map["Fb"];
	double dFct	= Map["Fct"];
	double dAo	= Map["Ao"];
	double dAg	= Map["Ag"];
	double dFba	= Map["Fba"];

	szText = _T("    ④ 정착구 지압응력 검토 \n");
	RichEdit.AddText(szText);
	szText = _T("    ㆍ허용지압응력 산정 (fba) \n");
	RichEdit.AddText(szText);
	szText.Format("       fb	= %.3f × %.3f × / ( %.3f × %.3f )	=	%.3f MPa \n", dTdn_Pu, dTdn_Area, dFix_Height, dFix_Width, dFb);
	RichEdit.AddText(szText);
	szText.Format("       fba	= 0.7 × φ × fct' × √(A/Ag) \n");
	RichEdit.AddText(szText);
	szText.Format("       	= 0.70 × 0.80 × %.1f × √(%.1f / %.1f)	=	%.3f MPa \n", dFct, dAo, dAg, dFba);
	RichEdit.AddText(szText);

	if (dFb < dFba)	{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (dFb > dFba)	{ szSign=_T("＞"); szOk=_T("∴ N.G"); }
	szText.Format("    ∴ fb	=	");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa	", dFb);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	fba	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f  MPa	", dFba);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	if (dFb < 2.25*0.80*dFct)	{ szSign=_T("＜"); szOk=_T("∴ O.K"); }
	if (dFb > 2.25*0.80*dFct)	{ szSign=_T("＞"); szOk=_T("∴ N.G"); }
	szText.Format("    ∴ fb	=	");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa	", dFb);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	2.25φfct'	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f  MPa	", 2.25*0.8*dFct);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

}

// 슬래브철근량검토
// nCheckType  = 0 : 강도설계법			1 : 경험적 설계법
void CPlateStressOut::Output_RebarQtyCheck(long nPos, CShRichEdit &RichEdit, long nCheckType)
{
	CCalcData		*pData = m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp	*pDB   = m_pStd->GetBridge();
	CCalcGeneral	CalcGeneral(m_pStd->GetDataManage());
	if(nCheckType==1)
	{
		Output_RebarQtyExperienceCheck(RichEdit);
		return;
	}

	CStringArray arr;	
	CString newElement(_T(""));

	double Mu   = tokNM(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Mu);
	double d11  = pData->m_dCoverUp;

	if(nPos == 2) d11 = pData->m_dCoverDn;

	double L		= toM(pData->CALC_CANTILEVER_DATA[nPos].m_Dead_L);//toM(CalcGeneral.CalcCentPart_L());
	double a		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_a;
	double Asreq	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Asreq;
	double b		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_b;
	double h		= 0;//Data->CALC_CANTILEVER_DATA[nPos].m_Min_T - pDB->m_BindConc.m_dDeep; // pData->CALC_CANTILEVER_DATA[nPos].m_Min_T;
	if(nPos==0)			h = pDB->m_dThickSlabBase;
	else if(nPos==1)	h = pDB->m_dThickSlabBase;
	else if(nPos==2)	h = pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;
	double d		= h - d11;
	double Ck		= pData->DESIGN_MATERIAL.m_dSigmaCK;
	double y		= pData->DESIGN_MATERIAL.m_dSigmaY;
	CString sRebar	= pData->DESIGN_MATERIAL.m_sSigmaY;
	
	double dPI  = pData->m_Pib;						// 휨에 대한 강도감소계수
	double Pmax = pData->m_dRebarPmax;
	double Pused= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Pused;

	double UseAs_1Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs_1Cy;
	double UseAs_2Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs_2Cy;
	double UseAs		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs;
	double UseAs2		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs2;
	double UseAs2_1Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs2_1Cy;
	double UseAs2_2Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs2_2Cy;
	double BAsreq		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_BAsreq;
	double BAsuse		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_BAsuse;
	double BAsuse2		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_BAsuse2;
	
	double Md     = CalcGeneral.CalcReinRod_Md(dPI, UseAs, y, d, a);
	double Es     = pData->DESIGN_MATERIAL.m_dEs;
	double Maxc   = 0.75*0.003/(0.003+y/Es)*d;
	double areq   = CalcGeneral.CalcReinRod_a(UseAs,y,Ck,b);	
	double c      = areq/0.85;
	double Maxa   = 0.85*Maxc;
 
	CString	CaseH1_1Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CaseH1;
	CString	CaseH1_2Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CaseH1_2Cy;
	CString	CaseH2_1Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CaseH2;
	CString	CaseH2_2Cy	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CaseH2_2Cy;
	CString	CaseH3		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CaseH3;
	CString	CaseH4		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CaseH4;

	double Ctc1		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Ctc1;
	double Ctc2		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Ctc1;
	double CtcB		= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CtcB;
	double CtcB1	= pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CtcB1;

    CString OkOrNo1(_T("")),OkOrNo2(_T("")),strSign1(_T("")),strSign2(_T("")),strSign3(_T("")),strSign4(_T(""));

	if(Pmax > Pused)
	{
		OkOrNo1 = _T("... O.K");
		strSign1 = _T("<");
	}
	else
	{
		OkOrNo1 = _T("... N.G");
		strSign1 = _T(">");
	}

	if(Maxc > c)		strSign2 = _T(">");
	else				strSign2  = _T("<");

	if(Maxc > c)		strSign3 = _T(">");
	else				strSign3  = _T("<");	

	if(Md > Mu)	{		OkOrNo2 = _T("... O.K");		strSign4 = _T(">");}
	else		{		OkOrNo2 = _T("... N.G");		strSign4 = _T("<");}
	
	if(nPos==FLOOR_LEFT)		newElement.Format("    < 좌측 캔틸레버 철근량 산정 >\n\n");
	if(nPos==FLOOR_RIGHT)		newElement.Format("    < 우측 캔틸레버 철근량 산정 >\n\n");
	if(nPos==FLOOR_MID)			newElement.Format("    < 중앙부 슬래브 철근량 산정 >\n\n");
	RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);

	newElement.Format("  (1) 입력자료\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
	newElement.Format("      Mu   =    %.3f kN.m\n", Mu);
	RichEdit.AddText(newElement);
	newElement.Format("       b    =    %.3f mm        h    =    %.3f mm", b, h);
	RichEdit.AddText(newElement);
	newElement.Format("      d'    =    %.3f mm           d    =    %.3f mm\n", d11,d);
	RichEdit.AddText(newElement); 
	newElement.Format("      fck   =    %.3f MPa          fy   =    %.3f MPa", Ck,y);
	RichEdit.AddText(newElement);
	newElement.Format("      φ(Bending)  =  %.2f\n\n", dPI);
	RichEdit.AddText(newElement);
	newElement.Format("  (2) 철근검토\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
	newElement.Format("      ① 일반부 주철근\n");
	RichEdit.AddText(newElement);
	newElement.Format("         Mu = φ × As × fy × (d-a/2)");
	RichEdit.AddText(newElement);
	if(Ck<=28)	newElement.Format("         a  = As × fy / (0.85 × fck × b)\n");
	else		newElement.Format("         a  = As × fy / (%.3lf × fck × b)\n", 0.85-(Ck-28)*0.007);
	RichEdit.AddText(newElement);
	newElement.Format("          - 필요철근    Asreq = %.3f mm² \n",Asreq);
	RichEdit.AddText(newElement);
	newElement.Format("          - 사용철근    %s%s   @   %.0f    ⇒    %.3f mm²  (1 Cycle)\n", sRebar, CaseH1_1Cy, Ctc1, UseAs_1Cy);
	RichEdit.AddText(newElement);
	newElement.Format("                             %s%s   @   %.0f    ⇒    %.3f mm²  (2 Cycle)\n", sRebar, CaseH1_2Cy, Ctc1, UseAs_2Cy);
	RichEdit.AddText(newElement);
	newElement.Format("                                                 합계   %.3f mm²", UseAs);
	RichEdit.AddText(newElement);
	if(UseAs>Asreq)
	{
		newElement.Format("    ... O.K\n");
		RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		newElement.Format("    ... N.G\n");
		RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
	}

	newElement.Format("          - 철근비검토 \n");
	RichEdit.AddText(newElement);
	newElement.Format("             P  =  As / b × d = %.4f   %s   Pmax = 0.75 × Pb = %.4f", Pused, strSign1, Pmax);
	RichEdit.AddText(newElement);
	if(Pmax > Pused)
	{
		newElement.Format("    ... O.K\n");
		RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		newElement.Format("    ... N.G\n");
		RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
	}
	
	newElement.Format("          - 설계모멘트검토\n");
	RichEdit.AddText(newElement);
	newElement.Format("             Max.c = 0.75 × %.f / %.f + fy×d = %.3f mm   %s   c = a/0.85 = %.3f mm\n", 0.003*Es,0.003*Es,Maxc,strSign2, c);
	RichEdit.AddText(newElement);
	newElement.Format("             Max.a = 0.85 × c = %.3f mm   %s   a = As × fy / ( 0.85 × fck × b ) = %.3f mm\n", Maxa, strSign3, a);
	RichEdit.AddText(newElement);
	newElement.Format("             jd       = d - a/2 = %.3f mm\n", d-a/2);
	RichEdit.AddText(newElement);
	newElement.Format("             C       = 0.85 × fck × a × b =  %.3f kN   ,   T = fy × As =  %.3f kN\n", tokN(0.85*Ck*a*b), tokN(y*UseAs));
	RichEdit.AddText(newElement);
	newElement.Format("             Mn     = φ×fy×As×jd  = %.3f kN.m   %s   Mu =  %.3f kN.m", Md,strSign4,Mu);
	RichEdit.AddText(newElement);
	if(Md > Mu)
	{
		newElement.Format("    ... O.K\n\n");
		RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		newElement.Format("    ... N.G\n\n");
		RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
	}

	newElement.Format("      ② 일반부 배력철근       ☞도.시 P191 \n");
	RichEdit.AddText(newElement);
	newElement.Format("             배력철근의 주철근에 대한 백분율");
	RichEdit.AddText(newElement);
	
	double BPer = 120/sqrt(L);
	if(BPer > 67)
	{
		newElement.Format("      120/√L = 120 / √%.3f  = %.3f  %%   >   67 %%\n",L,BPer);
		RichEdit.AddText(newElement);
		newElement.Format("             Asreq  =  As × 67%% = %.3f × 0.67 =  %.3f  mm²\n",Asreq,Asreq*0.67);
		RichEdit.AddText(newElement);
		newElement.Format("             수축·온도철근량  :  0.002·b·h  = %.3f mm²\n", 0.002*b*h);
		RichEdit.AddText(newElement);
		newElement.Format("             사용철근     %s%s   @   %.0f     =  %.3f  mm²", sRebar, CaseH3,CtcB,BAsuse);	// USE는 임시의 값임
		RichEdit.AddText(newElement);

		if(BAsuse>0.002*b*h && BAsuse>Asreq*0.67)
		{
			newElement.Format("    ... O.K\n\n");
			RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			newElement.Format("    ... N.G\n\n");
			RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
		}
	}
	else
	{
		newElement.Format("      120/√L = 120 / √%.3f  = %.0f  %%   <   67 %%\n",L,BPer);
		RichEdit.AddText(newElement);
		newElement.Format("             Asreq  =  As × %.0f %% = %.3f × %.2f =  %.3f  mm²\n",BPer, Asreq,(BPer/100),Asreq*(BPer/100));
		RichEdit.AddText(newElement);
		newElement.Format("             수축·온도철근량  :  0.002·b·h  = %.3f mm²\n", 0.002*b*h);
		RichEdit.AddText(newElement);
		newElement.Format("               사용철근     %s%s   @   %.0f     =  %.3f  mm", sRebar, CaseH3,CtcB,BAsuse);
		RichEdit.AddText(newElement);
		if(BAsuse>0.002*b*h && BAsuse>Asreq*(BPer/100))
		{
			newElement.Format("    ... O.K\n\n");
			RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			newElement.Format("    ... N.G\n\n");
			RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
		}
	}

	if(nPos==FLOOR_MID)		//중앙부
	{
		newElement.Format("      ③ 단부 주철근 설계				☞도.시 P195\n");
		RichEdit.AddText(newElement);
		newElement.Format("             들보단부이외의 중간지간의 바닥판에 필요한 주철근량의 2배를 주철근으로 배치한다. \n");
		RichEdit.AddText(newElement);
		newElement.Format("           ∴  Asreq  =  %.3f mm² × 2 = %.3f mm²\n",Asreq, 2*Asreq);
		RichEdit.AddText(newElement);
		newElement.Format("               사용철근      %s%s   @   %.0f     =  %.3f mm²\n", sRebar, CaseH2_1Cy, Ctc2, UseAs2_1Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                 %s%s   @   %.0f     =  %.3f mm²\n", sRebar, CaseH2_2Cy, Ctc2, UseAs2_2Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                                    합계  %.3f mm²", UseAs2);
		RichEdit.AddText(newElement);
		if(UseAs2>2*Asreq)
		{
			newElement.Format("    ... O.K\n\n");
			RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			newElement.Format("    ... N.G\n\n");
			RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
		}
		
		newElement.Format("      ④ 단부 배력철근\n");
		RichEdit.AddText(newElement);
		newElement.Format("             필요철근      Asreq  =  %.3f mm² × 2 = %.3f mm²\n",BAsreq, 2*BAsreq);
		RichEdit.AddText(newElement);
		newElement.Format("             사용철근      %s%s   @   %.0f     =  %.3f mm²", sRebar, CaseH4,CtcB1,BAsuse2);	// USE는 임시의 값임
		RichEdit.AddText(newElement);
		if(BAsuse2>2*BAsreq)
		{
			newElement.Format("    ... O.K\n\n");
			RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			newElement.Format("    ... N.G\n\n");
			RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
		}

		newElement.Format("      ⑤ 단부 배치범위\n");
		RichEdit.AddText(newElement);
		newElement.Format("            캔틸레버판의 고정하중에 대한 지간의 ½ (L=%.3fm) 이상", 
			toM(pData->CALC_CANTILEVER_DATA[nPos].m_Dead_L)/2);
		RichEdit.AddText(newElement);
	}
	else			//캔틸레버부
	{
		newElement.Format("      ③ 단부 주철근 설계 \n");
		RichEdit.AddText(newElement);
		newElement.Format("              필요철근      Asreq  =  %.3f mm² × 2 = %.3f mm²\n",Asreq, 2*Asreq);
		RichEdit.AddText(newElement);
		newElement.Format("              사용철근      %s%s   @   %.0f     =  %.3f mm²\n", sRebar, CaseH2_1Cy, Ctc2, UseAs2_1Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                %s%s   @   %.0f     =  %.3f mm²\n", sRebar, CaseH2_2Cy, Ctc2, UseAs2_2Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                             합계  %.3f mm²", UseAs2);
		RichEdit.AddText(newElement);
		if(UseAs2>2*Asreq)
		{
			newElement.Format("    ... O.K\n\n");
			RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			newElement.Format("    ... N.G\n\n");
			RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
		}

		newElement.Format("      ④ 단부 배력철근\n");
		RichEdit.AddText(newElement);
		newElement.Format("             필요철근      Asreq  =  %.3f mm² × 2 = %.3f mm²\n",BAsreq, 2*BAsreq);
		RichEdit.AddText(newElement);
		newElement.Format("             사용철근      %s%s   @   %.0f     =  %.3f mm²", sRebar, CaseH4,CtcB1,BAsuse2);	// USE는 임시의 값임
		RichEdit.AddText(newElement);
		if(BAsuse2>2*BAsreq)
		{
			newElement.Format("    ... O.K\n\n");
			RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			newElement.Format("    ... N.G\n\n");
			RichEdit.AddMsg(newElement, RE_RED, FALSE, TRUE);
		}

		newElement.Format("      ⑤ 단부 배치범위\n");
		RichEdit.AddText(newElement);
		newElement.Format("             캔틸레버판의 고정하중에 대한 지간 (L=%.3fm) 이상", 
			toM(pData->CALC_CANTILEVER_DATA[nPos].m_Dead_L));
		RichEdit.AddText(newElement);
	}
}

void CPlateStressOut::Output_InnerFloor(CShRichEdit &RichEdit)
{
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	CCalcData			*pData			= m_pStd->GetDataManage()->GetCalcData();

	CCalcFloor			CalcFloor(pDataManage);
	CMap <CString, LPCTSTR, double, double> Map;
	CalcFloor.CalcCantileverInnerPositionCheck(Map);

	double dSlab_Thk =	Map["Slab_Thk"];
	double dReq_As	 =	Map["Req_As"];
	double dBar_Dia1 =	Map["Bar_Dia1"];
	double dBar_Spa1 =	Map["Bar_Spa1"];
	double dBar_Are1 =	Map["Bar_Are1"];
	double dBar_Dia2 =	Map["Bar_Dia2"];
	double dBar_Spa2 =	Map["Bar_Spa2"];
	double dBar_Are2 =	Map["Bar_Are2"];
	double dBar_Area =	Map["Bar_Area"];
	double dBar_Use1 =	Map["Bar_Use1"];
	double dBar_Use2 =	Map["Bar_Use2"];
	double dBar_n	 =	Map["Bar_n"];
	double dUr		 =	Map["Ur"];

	
	CString szText(_T(""));
	CString szSign(_T(""));
	CString szOk(_T(""));
	CString szStress(_T(""));
		
	// 위치
	szText = _T("    중간 지점부 \n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText= _T("    ㆍ인장력을 받는 바닥판에서 콘크리트의 단면을 무시하고 설계를 하는 경우에는 바닥판 콘크리트 단면적의 \n");
	RichEdit.AddText(szText);
	szText .Format("       %.1f%%이상의 교축방향 철근을 배근하여야 한다. 이 경우 교축방향 철근의 총주변장의 바닥판 콘크리트의 \n", CalcFloor.GetFloorConcAreaCoefficient());
	RichEdit.AddText(szText);
	szText.Format("       단면적에 대한 비, 즉 주장률은 %.4f mm/mm² 이상으로 하면 좋다. \n\n", CalcFloor.GetFloorJujangStd());
	RichEdit.AddText(szText);

	// ① 철근량 검토
	if (dBar_Area<dReq_As)	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dBar_Area>dReq_As)	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }

	szText = _T("    ① 철근량 검토 \n");
	RichEdit.AddText(szText);
	szText.Format("       필요 철근량    Req As    =    1000.0    ×    %.3f    ×    %.3f    =    %.1f  mm² \n", 
					dSlab_Thk, CalcFloor.GetFloorConcAreaCoefficient()/100, dReq_As);
	RichEdit.AddText(szText);
	szText = _T("       사용 철근량 \n");
	RichEdit.AddText(szText);
	szText.Format("       상 면    :    H %.0f    @    %.0f    =    %.1f    mm² \n", dBar_Dia1, dBar_Spa1, dBar_Are1);
	RichEdit.AddText(szText);	
	szText.Format("       하 면    :    H %.0f    @    %.0f    =    %.1f    mm² \n", dBar_Dia2, dBar_Spa2, dBar_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                          ∑    =    %.1f    mm²\n", dBar_Area);
	RichEdit.AddText(szText);
	szText = _T("       ∴    Use As    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.1f    mm²    ", dBar_Area);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    Req As    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f    mm²    ", dReq_As);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// ② 주장률 검토
	if (dUr<CalcFloor.GetFloorJujangStd())	{ szSign=_T("＜"); szOk=_T("∴ N.G"); }
	if (dUr>CalcFloor.GetFloorJujangStd())	{ szSign=_T("＞"); szOk=_T("∴ O.K"); }

	szText = _T("    ② 주장률 검토 \n");
	RichEdit.AddText(szText);
	szText= _T("    ㆍ교축방향 철근의 총주변장과 바닥판 콘크리트의 단면적에 대한 비 \n");
	RichEdit.AddText(szText);
	szText.Format("       철근 사용량    :    %.2f    +    %.2f    =    %.2f    EA/m \n\n",
					dBar_Use1, dBar_Use2, dBar_n);
	RichEdit.AddText(szText);
	szText = _T("		            π × D × n + π × D × n\n");
	RichEdit.AddText(szText);
	szText.Format("       주 장 률    :    μ = ────────────     >    %.4f mm/mm² \n", CalcFloor.GetFloorJujangStd());
	RichEdit.AddText(szText);
	szText = _T("       		               1000 × tm \n\n");
	RichEdit.AddText(szText);
	szText = _T("                            D       :    철근 직경 \n");
	RichEdit.AddText(szText);
	szText = _T("                            n       :    철근 개수 \n");
	RichEdit.AddText(szText);
	szText = _T("                            1000    :    단위 폭 (mm) \n");
	RichEdit.AddText(szText);
	szText = _T("                            tm      :    중간 슬래브 두께 (mm) \n\n");
	RichEdit.AddText(szText);

	szText.Format("		            π × %.2f × %.2f + π × %.2f × %.2f\n", _dRebarDiadim[pData->m_ComboInnerJijumSlab1], dBar_Use1, _dRebarDiadim[pData->m_ComboInnerJijumSlab2], dBar_Use2);
	RichEdit.AddText(szText);	
	szText.Format("                           μ = ───────────────     =    %.4f  mm/mm² \n", dUr);
	RichEdit.AddText(szText);	
	szText.Format("		                  1000 × %.2f \n", dSlab_Thk);

	RichEdit.AddText(szText);
	szText = _T("       ∴    μ    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.4f    mm/mm²    ", dUr);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    μa    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.4f    mm/mm²    ", CalcFloor.GetFloorJujangStd());
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

}

void CPlateStressOut::Output_DroopReview(long nSide, CShRichEdit &RichEdit)		
{
	CCalcData		*pData	= m_pStd->GetDataManage()->GetCalcData();
	CCalcFloor		CalcFloor(m_pStd->GetDataManage());

	CString sText	= _T("");
	double	dDeadL	= pData->CALC_CANTILEVER_DATA[nSide].m_Dead_L;
	double	dSigmaY	= pData->DESIGN_MATERIAL.m_dSigmaY;
	double	dCoeff	= nSide == FLOOR_MID ? 28 : 10;
	double	dTmin	= pData->CALC_CANTILEVER_DATA[nSide].m_Min_T;
	double	dT		= CalcFloor.GetCantileverDroop(nSide);

	sText.Format("    (1) 처짐 검토         ☞콘.설 P79\n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	sText.Format("       처짐계산을 하지 않는 부재의 최소두께  T  = %.1f mm\n", dTmin);
	RichEdit.AddText(sText);

	if(dSigmaY==400.0)
	{
		sText.Format("       fy  =  %.1f MPa  이므로  tmin  =  L / %.0f  적용\n", dSigmaY, dCoeff);
		RichEdit.AddText(sText);
		sText.Format("       tmin  =  L / %.1f  =  %.3f / %.1f  =  %.3f mm\n", dCoeff, dDeadL, dCoeff, dT);
		RichEdit.AddText(sText);
	}
	else
	{
		sText.Format("       fy  =  %.1f MPa  이므로  tmin  =  L / %.0f × (0.43 + fy / 700) 적용\n", dSigmaY, dCoeff);
		RichEdit.AddText(sText);
		sText.Format("       tmin  =  L / %.1f × ( 0.43 + %.1f / 700 )  = %.3f mm\n", dCoeff, dSigmaY, dT);
		RichEdit.AddText(sText);
	}

	if(dT>dTmin)
	{
		sText.Format("       ∴ tmin   >   %.3f mm  이므로 처짐검토 필요.", dTmin);
		RichEdit.AddText(sText);
	}
	else
	{
		sText.Format("       ∴ tmin   <   %.3f mm  이므로 별도의 처짐검토 필요없음.", dTmin);
		RichEdit.AddText(sText);
	}
}

void CPlateStressOut::Output_CrackReview(long nSide, CShRichEdit &RichEdit)		
{
	CDataManage			*pDataManage	= m_pStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CCalcFloor			CalcFloor(pDataManage);

	CMap <CString, LPCTSTR, double, double> Map;
	CalcFloor.SetCantileverCrack(Map, nSide);

	double		ddWaCoeff		= pOpt->GetdCoeffDesignAllowCrackWidthEnv();
	CString		strAllowCrackWidth = pOpt->GetStrDesignAllowCrackWidthEnv();
	
	CString	sText	= _T("");
	double	dEs			= pCalcData->DESIGN_MATERIAL.m_dEs;			// 철근 탄성계수
	double	dEc			= pCalcData->DESIGN_MATERIAL.m_dEc;			// 콘크리트 탄성계수
	double	dStop_M		= Map["CRACK_MSTOP"];
	double	dMove_Ml	= Map["CRACK_MMOVE"];
	double	dCent_Mcf	= Map["CRACK_MCENT"];
	double	dWind_Mw	= Map["CRACK_MW"];
	double	dMmax		= Map["CRACK_MMAX"];
	double	dAs			= Map["CRACK_As"] + Map["CRACK_Ap"];		// 소수주형일 경우 긴장재의 면적 포함 
	double	dD			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_d;
	double	dH			= Map["CRACK_H"];
	double	dEA			= Map["CRACK_EA"];
	double	dB			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_b;
	double	dC			= Map["CRACK_C"];		//철근콘크리트 최소덮개
	long	n			= long(dEs / dEc);
	double	dpDomyunAAA	= Map["CRACK_P"];
	double	dK			= Map["CRACK_K"];
	double	dJ			= Map["CRACK_J"];
	double	dSigmaSmax	= Map["CRACK_Fsmax"];
	double	dA			= Map["CRACK_A"];
	double	dDC			= dH/2.0;
	double	dZ			= Map["CRACK_Z"];
	double	dR			= Map["CRACK_R"];
	double	dW			= Map["CRACK_W"];
	double	dWa			= Map["CRACK_Wa"];

	CString sign1(_T("")),sign2(_T("")),OK_NG1(_T("")),OK_NG2(_T(""));
	if(dZ < 23170.0)
	{
		sign1 = "<";
		OK_NG1 = "... O.K !";
	}
	else
	{
		sign1 = ">";
		OK_NG1 = "... N.G !";
	}
	if(dW < dWa)
	{
		sign2 = "<";
		OK_NG2 = "... O.K !";
	}
	else
	{
		sign2 = ">";
		OK_NG2 = "... N.G !";
	}

	sText.Format("    (2) 균열 검토\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	sText.Format("       ① 휨 철근의 응력 (fs)\n");
	RichEdit.AddText(sText);
	sText.Format("          Mmax  =  %.3f  kN.m \n", tokNM(dMmax));
	RichEdit.AddText(sText);
	sText.Format("          사용하중하에서 최대하중을 유발하는 하중조합을 사용한다.\n");
	RichEdit.AddText(sText);
	sText.Format("          M  =  Md + Ml+i + Mcf + 0.3 × Mw  =  %.3f + %.3f + %.3f + %.3f  =  %.3f kN.m\n", tokNM(dStop_M), tokNM(dMove_Ml), tokNM(dCent_Mcf), tokNM(dWind_Mw*0.3), tokNM(dMmax));
	RichEdit.AddText(sText);
	sText.Format("          n  =  Es / Ec  =  %ld\n ", n);
	RichEdit.AddText(sText);
	sText.Format("         ρ =  As / bd   =  %.3f / (%.3f × %.3f)  =  %.5f\n", dAs, dB, dD, dpDomyunAAA);
	RichEdit.AddText(sText);
	sText.Format("          k  =  -nρ + √{(nρ)² + 2nρ}  =  %ld × %.5f + √{(%.4f)² + 2 × %.4f}  =  %.4f\n", -n, dpDomyunAAA, n*dpDomyunAAA, n*dpDomyunAAA, dK);
	RichEdit.AddText(sText);
	sText.Format("          j  =  1 - (k/3) = 1 - (%.4f/3)  =  %.3f\n", dK, dJ);
	RichEdit.AddText(sText);
	sText.Format("          fs  =  Mmax / (ρㆍbㆍjㆍd²)  =  (%.3f × 10^5) / (%.5f × %.1f × %.3f × %.1f²)\n", dMmax, dpDomyunAAA, dB, dJ, dD);
	RichEdit.AddText(sText);
	sText.Format("              =  %.3f  MPa\n\n", dSigmaSmax);
	RichEdit.AddText(sText);

	sText.Format("       ② 유효 인장 단면적\n");
	RichEdit.AddText(sText);
	sText.Format("          A  =  b × h / EA  =  %.3f × %.3f / %.0f  =  %.1f mm²\n", dB, dH, dEA, dA);
	RichEdit.AddText(sText);
	sText.Format("           ㆍA  : 주인장철근 주위의 콘크리트 단면적을 철근의 수량으로 나눈 유효인장 단면적\n");
	RichEdit.AddText(sText);
	sText.Format("           ㆍdc : 인장연단에서 가장 가까이 위치한 철근 중심까지의 피복두께\n");
	RichEdit.AddText(sText);
	sText.Format("           ㆍz  : 휨철근의 분배를 제한하는 수량\n");
	RichEdit.AddText(sText);
	sText.Format("          z  =  fs × (dc × A)^⅓ = %.3f ×( %.3f × %.1f )^⅓\n ", dSigmaSmax, dDC, dA);
	RichEdit.AddText(sText);
	sText.Format("             =  %.3f N/mm", dZ);
	RichEdit.AddText(sText);
	if(dZ < 23170.0)
	{
		sText.Format("    <  %.1f N/mm (심한 외노출)", 23170.0);
		RichEdit.AddText(sText);
		sText.Format("    ... O.K\n\n");
		RichEdit.AddMsg(sText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		sText.Format("    >  %.1f N/mm (심한 외노출)", 23170.0);
		RichEdit.AddText(sText);
		sText.Format("    ... N.G\n\n");
		RichEdit.AddMsg(sText, RE_RED, FALSE, TRUE);
	}

	//균열폭검토
	sText.Format("       ③ 균열폭의 검토\n");
	RichEdit.AddText(sText);
	sText.Format("          ㉮ wa =  %.4f × C  (%s)\n", ddWaCoeff, strAllowCrackWidth);
	RichEdit.AddText(sText);
	sText.Format("              여기서, C  :  철근 콘크리트  최소 덮개\n");
	RichEdit.AddText(sText);
	sText.Format("          ㉯ wa  =  %.4f × %.1f  =  %.3f  mm\n", ddWaCoeff, dC, dWa);
	RichEdit.AddText(sText);
	sText.Format("          ㉰  w  =  1.08 × R × fs × ³√(dc ×A) × 0.00001\n");
	RichEdit.AddText(sText);
	sText.Format("               w  =  1.08 × %.3f × %.3f × ³√(%.1f ×%.1f) × 0.00001\n", dR, dSigmaSmax, dDC, dA);
	RichEdit.AddText(sText);
	sText.Format("                   =  %.3f mm", dW);
	RichEdit.AddText(sText);
	if(dW < dWa)
	{
		sText.Format("  <  %.3f mm", dWa);
		RichEdit.AddText(sText);
		sText.Format("    ... O.K");
		RichEdit.AddMsg(sText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		sText.Format("  >  %.3f mm", dWa);
		RichEdit.AddText(sText);
		sText.Format("    ... N.G");
		RichEdit.AddMsg(sText, RE_RED, FALSE, TRUE);
	}
}

void CPlateStressOut::Output_GiJumReview(CStringArray &Arr)		
{
}

void CPlateStressOut::Output_NodeDisplaceCheck(CStringArray &Arr)
{
	CPlateBridgeApp	*pBridge	= (CPlateBridgeApp*)m_pStd->GetBridge();
	CFemModelingCalc		ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	long	nGirsu			= pBridge->GetGirdersu();
	double	dMax			= ModelCalc.GetCalcMaxDisplace(0, nGirsu-1);
	double	dLengthJigan	= toM(pBridge->m_dLengthJigan[0]);
	double	dAllow			= dLengthJigan / (20000.0/dLengthJigan);
	CString newElement(_T(""));
	Arr.Add(newElement+"\r\n");
	newElement.Format("    << 처짐 검토 >>");
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ⇒ 활화중에 의한 최대 처짐 : %.3f cm", dMax);
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ⇒ 최대 허용 처짐");
	Arr.Add(newElement+"\r\n");

	newElement.Format("       교량구조물의 휨부재는 사용활하중(충격하중 포함)으로 구조물의 강도나 실제하중에  지장을");
	Arr.Add(newElement+"\r\n");

	newElement.Format("       주는 처짐 또는 그외 변형이 일어나지 않도록 충분한 강성을 같게 설계하여야 한다.");
	Arr.Add(newElement+"\r\n");

	newElement.Format("       강교의 허용최대 처짐량은 아래와 같다.         ☞도.설 P53");
	Arr.Add(newElement+"\r\n");

	newElement.Format("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━┓");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                      교 량 형 식                         ┃            최 대 처 짐           ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┣━━━━━━━━┳━━━━━━━━━━━━┳━━━━━━━╋━━━━━━━━━┳━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┃         종 류          ┃     지 간    ┃ 단순지지거더 및  ┃ 게르버거더의 ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┃                        ┃              ┃    연속 거더     ┃ 캔틸레버부   ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┣━━━━━━━━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃  플레이트 거더 ┃                        ┃  L   ≤ 10 m ┃    L / 2,000     ┃   L / 1,200  ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┃   철근콘크리트 슬래브  ┣━━━━━━━╋━━━━━━━━━╋━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃      형식      ┃   가 있는 플레이트거더 ┃10 < L ≤ 40 m┃   L / (20,000/L) ┃ L / (1,200/L)┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┃                        ┣━━━━━━━╋━━━━━━━━━╋━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┃                        ┃    L > 40 m  ┃      L / 500     ┃    L / 300   ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┣━━━━━━━━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                ┃    기타의 슬래브가 있는 플레이트거더   ┃      L / 500     ┃    L / 300   ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┣━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━┻━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                      현 수 교 형 식                      ┃              L / 350             ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                      사 장 교 형 식                      ┃              L / 400             ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━┳━━━━━━━┫");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┃                       기 타 형 식                        ┃      L / 600     ┃    L / 400   ┃");
	Arr.Add(newElement+"\r\n");
	newElement.Format("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━┻━━━━━━━┛");
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ∴ 허용 처짐량 Δa = L / (20,000/L) = %.3f / ( 20,000 / %.3f ) = %.3f m = %.3f cm", dLengthJigan, dLengthJigan, dAllow, dAllow*100);
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ⇒ 처짐 검토");
	Arr.Add(newElement+"\r\n");

	CString strSign;
	if(dMax < dAllow*100)	strSign = "<";
	else					strSign = ">";
	newElement.Format("      Δmax  = %.3f cm   %s   Δa = %.3f cm", dMax, strSign, dAllow*100);
	Arr.Add(newElement+"\r\n");

	if(dMax < dAllow*100)	strSign = "∴ O.K";
	else					strSign = "∴ N.G";
	newElement.Format("      %s", strSign);
	Arr.Add(newElement+"\r\n");

}



void CPlateStressOut::SpliceCheckOutput(CStringArray &Arr, CMap<CString, LPCTSTR, double, double> &MapSplice)
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CPlateGirderApp *pGir       = NULL;

	MapSplice.RemoveAll();

	// 거더별 현장이음 개수를 구함.
	long nSplicesu = 0;
	for (long nG = 0; nG < pDB->GetGirdersu (); nG++)  
	{
		pGir = pDB->GetGirder(nG);
		nSplicesu = max(pGir->GetQtySplice(), nSplicesu);
	}

	for (long nSp = 0; nSp < nSplicesu; nSp++)   
	{
		double dMaxMoment = 0.0;
		long   nMaxG = 0;
		for (long nG = 0; nG < pDB->GetGirdersu (); nG++) 
		{
			pGir = pDB->GetGirder(nG);
			if (nSp >= pGir->GetQtySplice())
				continue;
			CPlateBasicIndex *pBx = pGir->GetBxBySpliceNoSPType(nSp);
			CPlateSplice     *pSp = pBx->GetSplice();

			double dSta    = pBx->GetStation();
			double dMoment = StressCalc.GetElementForce(ELE_MOMENT3, 0, nG, dSta, 4);
			if (fabs(dMoment) > fabs (dMaxMoment)) 
			{
				dMaxMoment = dMoment;
				nMaxG = nG;
			}
		}
		SpliceCheckOutput (Arr, nMaxG, nSp, MapSplice);
		Arr.Add ("");
	}
}


void CPlateStressOut::SpliceCheckOutput(CStringArray &Arr, long nG, long nSp, CMap<CString, LPCTSTR, double, double> &MapSplice)
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CSteel			*pSteelCtl	= m_pStd->GetDataManage()->GetSteel();
	CSteelBolt      *pBolt      = m_pStd->GetDataManage()->GetSteelBolt();

	CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxBySpliceNoSPType(nSp);
	CPlateSplice     *pSp = pBx->GetSplice();

	BOOL	bHybrid	= pBx->IsHybrid();
	double	dSta	= pBx->GetStation();

	ModelCalc.GetSectionCoefficient(pBx, TRUE);
	StressCalc.GetSectionStress(nG, dSta);
		
	double	Su = 0, Sl = 0, Safu = 0, Safl = 0, Saw = 0, Dh = 0, Ra = 0;	
	CString	STfu, STfl, STw, STru, STrl, BD, BT, BS, sJuType;

	double	dHyR		= StressCalc.GetMapValueSecStress("HyR_Pos");	// 하이브리드계수 R
	BOOL	bPositive	= StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);
	BOOL	bHapSung	= m_pStd->GetBridge()->IsHapsung();
	MapSplice["ISPLUS_MOMENT"] = bPositive;	
	
	double Ms = StressCalc.GetMapValueSecStress("MomentPreDead");
	double Mv = StressCalc.GetMapValueSecStress("MomentPostDead")
				+StressCalc.GetMapValueSecStress("MomentLive")
				+StressCalc.GetMapValueSecStress("MomentWind");
				//+StressCalc.GetMapValueSecStress("MomentDisp");
	double Ss = fabs(StressCalc.GetMapValueSecStress("ShearPreDead"));
	double Sv = fabs(StressCalc.GetMapValueSecStress("ShearPostDead")
				+StressCalc.GetMapValueSecStress("ShearLive")
				+StressCalc.GetMapValueSecStress("ShearWind"));
				//+StressCalc.GetMapValueSecStress("ShearDisp"));
	double Mt = fabs(StressCalc.GetMapValueSecStress("TorsionSum"));
	MapSplice["Ms"] = Ms;
	MapSplice["Mv"] = Mv;

	if(!bHapSung) 
	{  // 비합성형
		Ms += Mv;
		Mv = 0.0;
		Ss += Sv;
		Sv = 0.0;
	}
	double Max_up=0, Max_lo=0, Max_upA=0, Max_loA=0, Tau_up=0, Tau_lo=0, Tau_upA=0, Tau_loA=0;
	StressCalc.GetFlangeStress(nG, dSta, Max_up, Max_lo, Max_upA, Max_loA, Tau_up, Tau_lo, Tau_upA, Tau_loA);

	Su = Max_up;
	Sl = Max_lo;

	STfu = pDB->GetMaterial(G_F_U, pBx);
	STfl = pDB->GetMaterial(G_F_L, pBx);
	STw  = pDB->GetMaterial(G_W, pBx);

	double	dUpThick = pSp->m_uT;
	double	dLoThick = pSp->m_dT;

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(STfu)];
	CHGAllowStress	AllowStress(pSteel);
	Safu = AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dUpThick);
	
	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(STfl)];
	AllowStress.SetBaseSteel(pSteel);
	Safl = AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dLoThick);

	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(STw)];
	AllowStress.SetBaseSteel(pSteel);
	Saw  = AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dUpThick);

	BD = pSp->m_ZzsBoltSize;		//  = M22  (Bolt 지름)
	BT = pDB->GetMaterial(SP_U_BOLT, pBx);	
	CSteelBoltData *pBoltData = pBolt->GetBolt(BT, BD);
	Dh = pBoltData->GetBoltSize() + pBoltData->m_HoleMargin;
	Ra  = pBoltData->m_ForceAllow;
	
	Arr.Add("");
	CString cs;
	if(Su > 0.0)	cs.Format(" A. 검토단면 : 정 모멘트 구간 (G%d, SP%d)", nG + 1, nSp + 1);
	else			cs.Format(" A. 검토단면 : 부 모멘트 구간 (G%d, SP%d)", nG + 1, nSp + 1);
	Arr.Add (cs);
	Arr.Add("");
	cs.Format(" B. 작용력 및 단면력" );
	Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())
	{  // 비합성형
		cs.Format("    - 작용 총 휨모멘트        : Ms  = %8.2f kN.m", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("    - 작용 총 전단력          : Ss  = %8.2f kN", tokN(Ss));
		Arr.Add (cs);
	}
	else
	{  // 합성형
		cs.Format("    - 합성전 휨모멘트         : Ms  = %8.2f kN.m", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("    - 합성후 휨모멘트         : Mv  = %8.2f kN.m", tokNM(Mv));
		Arr.Add (cs);
		cs.Format("    - 합성전 전단력           : Ss  = %8.2f kN", tokN(Ss));
		Arr.Add (cs);
		cs.Format("    - 합성후 전단력           : Sv  = %8.2f kN", tokN(Sv));
		Arr.Add (cs);
	}
	cs.Format("    - 비틀림 모멘트           : Mt  = %8.2f kN.m", tokNM(Mt));
	Arr.Add (cs);
	cs.Format("    - 상부 FLANGE의 최대 응력 : fu  = %8.2f MPa", Su);
	Arr.Add (cs);
	cs.Format("    - 하부 FLANGE의 최대 응력 : fl  = %8.2f MPa", Sl);
	Arr.Add (cs);
	if(bHybrid)
	{
		cs.Format("    - 합성후 하이브리드 계수  : R   = %7.3f", dHyR);		
		Arr.Add (cs);
	}

	MapSplice["HyR"]= dHyR;
	MapSplice["Su"] = Su;
	MapSplice["Sl"] = Sl;
	Arr.Add("");
	cs.Format(" C. 이음의 특성");
	Arr.Add (cs);
	if (STfu == STfl && STfl == STw && STw == STru && STru == STrl)
	{
		cs.Format("    1) 강 종 : %s   f = %g MPa", STfu, Safu);
		Arr.Add (cs);
	}
	else
	{
		cs.Format("    1) 강 종");
		Arr.Add (cs);
		cs.Format("       상  판 :%10s   f =%6g MPa", STfu, Safu);
		Arr.Add (cs);
		cs.Format("       하  판 :%10s   f =%6g MPa", STfl, Safl);
		Arr.Add (cs);
		cs.Format("       복부판 :%10s   f =%6g MPa", STw,  Saw);
		Arr.Add (cs);
	}
	MapSplice["Safu"] = Safu*dHyR;
	MapSplice["Safl"] = Safl*dHyR;
	MapSplice["Saw"] = Saw*dHyR;
	cs.Format("    2) BOLT  : H.T.B %s (%s) ρa  =  %g x 2면 마찰 = %g MPa", BD, BT, Ra, Ra * 2);
	MapSplice["BOLT_ALLOW_STRESS"] = Ra * 2 * dHyR;
	Arr.Add (cs);
	Ra *= 2.0;
/*	double dFs = 0;
	if((Ms+Mv)>0)	dFs = abs(Su);
	else			dFs = abs(Sl);
*/
	Arr.Add("");
	cs.Format(" D. %s   (상부 플랜지의 이음)", bPositive ? "압축부 FLANGE의 계산":"인장부 FLANGE의 계산");
	Arr.Add (cs);
	SpliceFlangeCheckUp(Arr, nG, nSp, Ss, Sv, Su, Safu, Ra, Dh, MapSplice);
//	if((Ms+Mv)>0)	dFs = abs(Sl);
//	else			dFs = abs(Su);
	Arr.Add("");
	cs.Format(" E. %s   (하부 플랜지의 이음)", bPositive ? "인장부 FLANGE의 계산":"압축부 FLANGE의 계산");
	Arr.Add (cs);
	SpliceFlangeCheckLo(Arr, nG, nSp, Ss, Sv, Sl, Safl, Ra, Dh, MapSplice);
	Arr.Add("");
	cs.Format(" F. WEB 연결부의 계산");
	Arr.Add (cs);
	SpliceWebCheck(Arr, nG, nSp, Ss, Sv, Ms, Mv, Su, Sl, Saw, Ra, MapSplice);

}
//	Ss : 합성전 전단력
//	Sv : 합성후 전단력
//	Sf : 플랜지 응력
//	Sa : 강재 허용 응력
//	Ra : 볼트 허용 강도 (2면 마찰 기준)
//	Dh : 볼트 구멍 지름
//  여기서의 내부 변수들의 단위는 편의를 위해 기본적으로 cm, kgf으로 하였음.
void CPlateStressOut::SpliceFlangeCheckUp(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Sf, double Sa, double Ra, double Dh, CMap<CString, LPCTSTR, double, double> &MapSplice)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir= pDB->GetGirder(nG);
	CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxBySpliceNoSPType(nSp);
	CPlateSplice     *pSp = pBx->GetSplice();
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);

	BOOL	bHybrid	= pBx->IsHybrid();
	double	dSta	= pBx->GetStation();

	ModelCalc.GetSectionCoefficient(pBx, TRUE);
	StressCalc.GetSectionStress(nG, dSta);	
	
	BOOL bTension = (Sf < 0.0);
	
	double dHyR	= StressCalc.GetMapValueSecStress("HyR_Pos");		// 하이브리드계수 R
	double Tf = pGir->GetThickFactByStation(dSta, G_F_U);			// 상부플랜지 두께
	double Wf = pGir->GetWidthOnStation(dSta, TRUE);				// 상부플랜지 너비
	double Ts = pSp->m_uT;											// 상부이음판 두께
	double LoW= pSp->m_uW;											// 상부 작은 이음판의 폭
	double Ls = (pSp->m_uA + pSp->m_uB * pSp->m_uC) * 2 + pSp->m_uC1;	// 상부이음판 길이
	int Nl = (int)(pSp->m_uB+1);										// 상부이음판-사용 볼트 라인수
	int Nb = (int)(pSp->m_uE+1)*2;										// 상부이음판-사용 볼트 라인당 개수
	double Ys = Round(fabs(ModelCalc.GetMapValueSecCo("Ysu")),1);			// 합성전-중립축에 대한 플랜지 거리
	double Yv = Round(fabs(ModelCalc.GetMapValueSecCo("Yvsu")),1);			// 합성후-중립축에 대한 플랜지 거리
	double Pb = Wf / Nb;   // 볼트 평균 핏치
	double As = Wf*Ts + LoW*Ts*2;
	MapSplice["Tf_UP"] = Tf;

	CString cs;
	cs.Format("     fs  = %g MPa", Sf);											Arr.Add (cs);
	cs.Format("     fs' = 0.75 x %g  =  %g MPa", Sa, Sa * 0.75);					Arr.Add (cs);
	Sf = max (fabs (Sf), Sa * 0.75);
	
	cs.Format("     ∴이음판의 설계응력(fu)은 %g MPa을 적용한다.", Sf);		Arr.Add (cs);
	cs.Format("    1) 필요볼트수의 결정");											Arr.Add (cs);
	cs.Format("              As x fs       %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);	Arr.Add (cs);
	if (Nl*Nb > Tf*Wf*Sf/(dHyR*Ra))
		cs.Format("       n =  --------- =  --------------------------  = %g EA  <  n = %d EA   O.K", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);
	else
		cs.Format("       n =  --------- =  --------------------------  = %g EA  >  n = %d EA   N.G", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);

	MapSplice["REQ_BOLTSU_UP"]   = Tf*Wf*Sf/(dHyR*Ra);
	MapSplice["USE_BOLTSU_UP"]   = Nl*Nb;

	Arr.Add (cs);
	if(bHybrid)	cs.Format("             R x ρa       %.3f  x  %.1f", dHyR, Ra);
	else		cs.Format("               ρa                 %.1f", Ra);									Arr.Add (cs);
	cs.Format("    2) 이음판의 결정");																		Arr.Add (cs);
	cs.Format("                           %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);								Arr.Add (cs);
	cs.Format("       필요단면적 : As =  --------------------------  =  %.2f mm²", Tf*Wf*Sf/(dHyR*Sa));	Arr.Add (cs);
	if(bHybrid)	cs.Format("                              %.3f  x  %.1f", dHyR, Sa);
	else		cs.Format("                                    %.1f", Sa);									Arr.Add (cs);
	cs.Format("       사용이음판 : 1-PL  %5g  x %4g  x  %g  =  %7.2f mm²", Wf, Ts, Ls, Wf*Ts);				Arr.Add (cs);
	cs.Format("		            2-PL  %5g  x %4g  x  %g  =  %7.2f mm²", LoW, Ts, Ls, LoW*Ts*2);			Arr.Add (cs);
	cs.Format("					    합계  =  %7.2f mm²", As);												Arr.Add (cs);
	cs.Format("                                               -------------------");						Arr.Add (cs);
	if (bTension)   // 인장부이면
	{  
		cs.Format("                                                 =  %7.2f mm²", As);	Arr.Add (cs);
		cs.Format("       이음판의 순단면적 : 인장부의 순단면적은 BOLT지름을 공제");		Arr.Add (cs);
		cs.Format("       An  =  %.2f  -  %.2f  x  %.2f  x  %d  x  2 EA", As, Dh, Ts, Nb);	Arr.Add (cs);
		As -= Dh * Ts * Nb * 2;
	}
	if (As > Tf*Wf*Sf/(dHyR*Sa))
		cs.Format("                                                 =  %7.2f mm²  >  Asreq'd = %7.2f mm²   O.K", As, Tf*Wf*Sf/(dHyR*Sa));
	else
		cs.Format("                                                 =  %7.2f mm²  <  Asreq'd = %7.2f mm²   N.G", As, Tf*Wf*Sf/(dHyR*Sa));

	MapSplice["REQ_AS_UP"]       = As;
	MapSplice["USE_AS_UP"]       = Tf*Wf*Sf/(dHyR*Sa);

	Arr.Add (cs);
	cs.Format("                             %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);		Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf/As)	cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa <  R x f sa , O.K", Tf*Wf*Sf/As);
		else						cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa >  R x f sa , N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf/As)	cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa <  f sa , O.K", Tf*Wf*Sf/As);
		else					cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa >  f sa , N.G", Tf*Wf*Sf/As);
	}
	
	Arr.Add (cs);
	cs.Format("                                       %.1f", As);				Arr.Add (cs);

	MapSplice["SPLICE_STRESS_UP"] = Tf*Wf*Sf/As;
	///////////////////////////////////////////////////////////////////////////////
	double Rp = Tf * Wf * Sf / (Nl * Nb);
	double Qs = Wf * Tf * (Ys - Tf / 2);
	double Qv = Wf * Tf * (Yv - Tf / 2);

	double Is = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBx);
	double Iv = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
//	double Is = ModelCalc.GetMapValueSecCo("I33");
//	double Iv = 0;
//	if(!bTension && !pDB->IsTUGir())
//		Iv = ModelCalc.GetMapValueSecCo("Iv33");
//	else if(!bTension)
//		Iv = ModelCalc.GetMapValueSecCo("Ivd33");
//	else
//		Iv = StressCalc.GetMapValueSecStress("Irv");
	double Rh = (Ss * Qs / Is + Sv * Qv / Iv) * Pb / Nl;
	double R  = hypot (Rp, Rh);
	MapSplice["BOLT_ACTING_STRESS_UP"] = R;
	///////////////////////////////////////////////////////////////////////////////
	Arr.Add ("");
	cs.Format("    3) BOLT의 응력 검토" );													Arr.Add (cs);
	cs.Format("       a) 직응력에 대한 응력" );												Arr.Add (cs);
	cs.Format("                   P      %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);				Arr.Add (cs);
	cs.Format("         ρP  =  ---- =  --------------------------  =   %.2f  N/EA", Rp);	Arr.Add (cs);
	cs.Format("                   n               %d EA", Nl * Nb);							Arr.Add (cs);
	cs.Format("       b) 휨전단력에 대한 응력" );											Arr.Add (cs);

	if (pDB->GetNumHaiseokCombo())// 비합성형
	{  
		cs.Format("                S Qs       p");							Arr.Add (cs);
		cs.Format("         ρh = ------- x ----");							Arr.Add (cs);
		cs.Format("                 Is        n");							Arr.Add (cs);
		cs.Format("         S  : 작용 총 전단력  ( %.3f kN )", tokN(Ss));	Arr.Add (cs);
		cs.Format("         Qs : 강재 단면의 중립축에 대한 접합선 외측의 단면 1차 모멘트");						Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm²", Wf, Tf, Ys, Tf/2, Qs);	Arr.Add (cs);
		cs.Format("         Is : 강재 단면의 단면 2차 모멘트  (  %.1f mm⁴)", Is);								Arr.Add (cs);
		cs.Format("         p : 접합선 방향의 BOLT의 평균 핏치 (mm) = 플랜지 전폭 / 횡방향볼트수 = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n  : 접합선 직각의 BOLT의 개수");									Arr.Add (cs);
		cs.Format("                %9.1f x %9.1f       %4.1f mm", Ss, Qs, Pb);					Arr.Add (cs);
		cs.Format("         ρh =  ---------------------  x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                     %11.1f             %d EA", Is, Nl);						Arr.Add (cs);
	}
	else  
	{
		cs.Format("               ┌ Ss Qs     Sv Qv  ┐     p" );								Arr.Add (cs);
		cs.Format("         ρh = │------- + ------- │ x ----" );								Arr.Add (cs);
		cs.Format("               └  Is        Iv    ┘     n" );								Arr.Add (cs);
		cs.Format("         Ss : 합성전 전단력  ( %.3f kN ),   Sv : 합성전 전단력  ( %.3f kN )", tokN(Ss), tokN(Sv));	Arr.Add (cs);
		cs.Format("         Qs, Qv : 합성전 및 합성후의 중립축에 대한 접합선 외측의 단면 1차 모멘트");					Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm²", Wf, Tf, Ys, Tf/2, Qs);		Arr.Add (cs);
		cs.Format("           Qv  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm²", Wf, Tf, Yv, Tf/2, Qv);		Arr.Add (cs);
		MapSplice["Ys_UP"] = Ys;
		MapSplice["Yv_UP"] = Yv;
		cs.Format("         Is, Iv : 합성전 및 합성후의 중립축에 대한 총단면의 단면 2차 모멘트");						Arr.Add (cs);
		cs.Format("           ( Is =  %.1f mm⁴,   Iv =  %.1f mm⁴)", Is, Iv);											Arr.Add (cs);
		cs.Format("         p : 접합선 방향의 BOLT의 평균 핏치 (mm) = 플랜지 전폭 / 횡방향볼트수 = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n : 접합선 직각의 BOLT의 개수");																	Arr.Add (cs);
		cs.Format("                ┌ %10.1f x %10.1f   %10.1f x %10.1f ┐     %4.1f mm", Ss, Qs, Sv, Qv, Pb);					Arr.Add (cs);
		cs.Format("         ρh =  │------------------------- + ---------------------- │ x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                └      %12.1f           %12.1f      ┘      %d EA", Is, Iv, Nl);							Arr.Add (cs);
	}
	cs.Format("       c) 합성응력의 검토");													Arr.Add (cs);
	cs.Format("               __________________     _________________________");			Arr.Add (cs);	
	if(bHybrid)
	{
		if (R < dHyR*Ra)	cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  <  R x ρa   O.K", Rp, Rh, R);
		else				cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  >  R x ρa   N.G", Rp, Rh, R);
	}
	else
	{
		if (R < Ra)	cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  <  ρa   O.K", Rp, Rh, R);
		else		cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  >  ρa   N.G", Rp, Rh, R);
	}
	
	Arr.Add (cs);

	if (!bTension)
		return;

	As = Tf * (Wf - Dh * Nb);
	cs.Format("    4) 모재의 응력 검토" );																							Arr.Add (cs);
	cs.Format("       플랜지의 총단면적                     :  As  =  %.1f  x  %.1f  =  %.1f mm²", Tf, Wf, Tf*Wf);					Arr.Add (cs);
	cs.Format("       볼트 구멍을 공제한 플랜지의 순단면적  :  Asn =  %.1f x (%.1f - %.1f x %d)  =  %.1f mm²", Tf, Wf, Dh, Nb, As);	Arr.Add (cs);
	cs.Format("                            %6.1f  x  %6.1f", Tf*Wf, Sf);															Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf / As)	cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² <  R x f a ,  O.K", Tf*Wf*Sf/As);
		else							cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² >  R x f a ,  N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf / As)	cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² <  f a ,  O.K", Tf*Wf*Sf/As);
		else					cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² >  f a ,  N.G", Tf*Wf*Sf/As);
	}
	
	Arr.Add (cs);
	MapSplice["인장모재작용응력_UP"] = Tf*Wf*Sf/As;
	cs.Format("                                 %6.1f", As);		Arr.Add (cs);
}

//	Ss : 합성전 전단력
//	Sv : 합성후 전단력
//	Sf : 플랜지 응력
//	Sa : 강재 허용 응력
//	Ra : 볼트 허용 강도 (2면 마찰 기준)
//	Dh : 볼트 구멍 지름
//  여기서의 내부 변수들의 단위는 편의를 위해 기본적으로 cm, kgf으로 하였음.
void CPlateStressOut::SpliceFlangeCheckLo(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Sf, double Sa, double Ra, double Dh, CMap<CString, LPCTSTR, double, double> &MapSplice)
{
	CPlateBridgeApp  *pDB = m_pStd->GetBridge();
	CPlateGirderApp  *pGir= pDB->GetGirder(nG);
	CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxBySpliceNoSPType(nSp);
	CPlateSplice     *pSp = pBx->GetSplice();
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	double dSta = pBx->GetStation();
	BOOL bTension = (Sf < 0.0);

	ModelCalc.GetSectionCoefficient(pBx, TRUE);
	StressCalc.GetSectionStress(nG, dSta);

	BOOL	bHybrid	= pBx->IsHybrid();
	double	dHyR	= StressCalc.GetMapValueSecStress("HyR_Pos");	// 하이브리드계수 R

	double Tf = pGir->GetThickFactByStation(dSta, G_F_L);			// 하부플랜지 두께
	double Wf = pGir->GetWidthOnStation(dSta, FALSE);				// 하부플랜지 너비
	double Ts = pSp->m_dT;											// 하부이음판 두께
	double LoW= pSp->m_dW;											// 하부 작은 이음판의 폭
	double Ls = (pSp->m_dA + pSp->m_dB * pSp->m_dC) * 2 + pSp->m_dC1;		// 하부이음판 길이
	int Nl = (int)(pSp->m_dB+1);										// 사용 볼트 라인수
	int Nb = (int)(pSp->m_dE+1)*2;										// 사용 볼트 라인당 개수
	double Ys = Round(fabs(ModelCalc.GetMapValueSecCo("Ysl")),1);				//  (중립축에 대한 플랜지 거리)
	double Yv = Round(fabs(ModelCalc.GetMapValueSecCo("Yvsl")),1);			//  (중립축에 대한 플랜지 거리)
	double Pb = Wf / Nb;   // 볼트 평균 핏치
	double As = Wf*Ts + LoW*Ts*2;
	
	MapSplice["ALLOW_STRESS_DN"] = Ra;
	MapSplice["Tf_DN"] = Tf;

	CString cs;
	cs.Format("     fs  = %g MPa", Sf);
	Arr.Add (cs);
	cs.Format("     fs' = 0.75 x %g  =  %g MPa", Sa, Sa * 0.75);
	Arr.Add (cs);
	Sf = max (fabs (Sf), Sa * 0.75);
	
	cs.Format("     ∴이음판의 설계응력(fl)은 %g MPa을 적용한다.", Sf);		Arr.Add (cs);
	cs.Format("    1) 필요볼트수의 결정");											Arr.Add (cs);
	cs.Format("              As x fs       %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);	Arr.Add (cs);
	if (Nl*Nb > Tf*Wf*Sf/(dHyR*Ra))
		cs.Format("       n =  --------- =  --------------------------  = %g EA  <  n = %d EA   O.K", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);
	else
		cs.Format("       n =  --------- =  --------------------------  = %g EA  >  n = %d EA   N.G", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);

	MapSplice["REQ_BOLTSU_DN"]   = Tf*Wf*Sf/(dHyR*Ra);
	MapSplice["USE_BOLTSU_DN"]   = Nl*Nb;

	Arr.Add (cs);
	if(bHybrid)	cs.Format("             R x ρa       %.3f  x  %.1f", dHyR, Ra);
	else		cs.Format("               ρa                 %.1f", Ra);									Arr.Add (cs);
	cs.Format("    2) 이음판의 결정");																		Arr.Add (cs);
	cs.Format("                           %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);								Arr.Add (cs);
	cs.Format("       필요단면적 : As =  --------------------------  =  %.2f mm²", Tf*Wf*Sf/(dHyR*Sa));	Arr.Add (cs);
	if(bHybrid)	cs.Format("                              %.3f  x  %.1f", dHyR, Sa);
	else		cs.Format("                                    %.1f", Sa);									Arr.Add (cs);
	cs.Format("       사용이음판 : 1-PL  %5g  x %4g  x  %g  =  %7.2f mm²", Wf, Ts, Ls, Wf*Ts);				Arr.Add (cs);
	cs.Format("		            2-PL  %5g  x %4g  x  %g  =  %7.2f mm²", LoW, Ts, Ls, LoW*Ts*2);			Arr.Add (cs);
	cs.Format("					    합계  =  %7.2f mm²", As);												Arr.Add (cs);
	cs.Format("                                               -------------------");						Arr.Add (cs);
	if (bTension)   {  // 인장부이면
		cs.Format("                                                 =  %7.2f mm²", As);					Arr.Add (cs);
		cs.Format("       이음판의 순단면적 : 인장부의 순단면적은 BOLT지름을 공제");						Arr.Add (cs);
		cs.Format("       An  =  %.2f  -  %.2f  x  %.2f  x  %d  x  2 EA", As, Dh, Ts, Nb);					Arr.Add (cs);
		As -= Dh * Ts * Nb * 2;
	}
	if (As > Tf*Wf*Sf/(dHyR*Sa))
		cs.Format("                                                 =  %7.2f ㎠  >  Asreq'd = %7.2f mm²   O.K", As, Tf*Wf*Sf/(dHyR*Sa));
	else
		cs.Format("                                                 =  %7.2f ㎠  <  Asreq'd = %7.2f mm²   N.G", As, Tf*Wf*Sf/(dHyR*Sa));

	MapSplice["REQ_AS_DN"]       = As;
	MapSplice["USE_AS_DN"]       = Tf*Wf*Sf/(dHyR*Sa);

	Arr.Add (cs);
	cs.Format("                             %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);		Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf/As)	cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa <  R x f sa , O.K", Tf*Wf*Sf/As);
		else						cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa >  R x f sa , N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf/As)	cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa <  f sa , O.K", Tf*Wf*Sf/As);
		else					cs.Format("       이음판의 응력 f s =  --------------------------  =   %.2f  MPa >  f sa , N.G", Tf*Wf*Sf/As);
	}
	
	Arr.Add (cs);
	cs.Format("                                       %.1f", As);				Arr.Add (cs);

	MapSplice["SPLICE_STRESS_DN"] = Tf*Wf*Sf/As;
	///////////////////////////////////////////////////////////////////////////////
	double Rp = Tf * Wf * Sf / (Nl * Nb);
	double Qs = Wf * Tf * (Ys - Tf / 2);
	double Qv = Wf * Tf * (Yv - Tf / 2);
	double Is = ModelCalc.GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pBx);
	double Iv = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
	
	/*
	if (bTension && !pDB->IsTUGir())   // 정모멘트
		Iv = ModelCalc.GetMapValueSecCo("Iv33");
	else if(bTension)
		Iv = ModelCalc.GetMapValueSecCo("Ivd33");
	else
		Iv = StressCalc.GetMapValueSecStress("Irv");
	*/
	double Rh = (Ss * Qs / Is + Sv * Qv / Iv) * Pb / Nl;
	double R  = hypot (Rp, Rh);
	MapSplice["BOLT_ACTING_STRESS_DN"] = R;
	///////////////////////////////////////////////////////////////////////////////
	Arr.Add ("");
	cs.Format("    3) BOLT의 응력 검토" );													Arr.Add (cs);
	cs.Format("       a) 직응력에 대한 응력" );												Arr.Add (cs);
	cs.Format("                   P      %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);				Arr.Add (cs);
	cs.Format("         ρP  =  ---- =  --------------------------  =   %.2f  N/EA", Rp);	Arr.Add (cs);
	cs.Format("                   n               %d EA", Nl * Nb);							Arr.Add (cs);
	cs.Format("       b) 휨전단력에 대한 응력" );											Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())  // 비합성형
	{  
		cs.Format("                S Qs       p");							Arr.Add (cs);
		cs.Format("         ρh = ------- x ----");							Arr.Add (cs);
		cs.Format("                 Is        n");							Arr.Add (cs);
		cs.Format("         S  : 작용 총 전단력  ( %.3f kN )", tokN(Ss));	Arr.Add (cs);
		cs.Format("         Qs : 강재 단면의 중립축에 대한 접합선 외측의 단면 1차 모멘트");						Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm²", Wf, Tf, Ys, Tf/2, Qs);	Arr.Add (cs);
		cs.Format("         Is : 강재 단면의 단면 2차 모멘트  (  %.1f mm⁴)", Is);								Arr.Add (cs);
		cs.Format("         p : 접합선 방향의 BOLT의 평균 핏치 (mm) = 플랜지 전폭 / 횡방향볼트수 = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n  : 접합선 직각의 BOLT의 개수");									Arr.Add (cs);
		cs.Format("                %9.1f x%9.1f       %4.1f mm", Ss, Qs, Pb);					Arr.Add (cs);
		cs.Format("         ρh =  ---------------------  x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                     %11.1f             %d EA", Is, Nl);						Arr.Add (cs);
	}
	else  
	{
		cs.Format("               ┌ Ss Qs     Sv Qv  ┐     p" );								Arr.Add (cs);
		cs.Format("         ρh = │------- + ------- │ x ----" );								Arr.Add (cs);
		cs.Format("               └  Is        Iv    ┘     n" );								Arr.Add (cs);
		cs.Format("         Ss : 합성전 전단력  ( %.3f kN ),   Sv : 합성전 전단력  ( %.3f kN )", Ss, Sv);				Arr.Add (cs);
		cs.Format("         Qs, Qv : 합성전 및 합성전의 중립축에 대한 접합선 외측의 단면 1차 모멘트");					Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm²", Wf, Tf, Ys, Tf/2, Qs);		Arr.Add (cs);
		cs.Format("           Qv  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm²", Wf, Tf, Yv, Tf/2, Qv);		Arr.Add (cs);
		MapSplice["Ys_DN"] = Ys;
		MapSplice["Yv_DN"] = Yv;
		cs.Format("         Is, Iv : 합성전 및 합성전의 중립축에 대한 총단면의 단면 2차 모멘트");						Arr.Add (cs);
		cs.Format("           ( Is =  %.1f mm⁴,   Iv =  %.1f mm⁴)", Is, Iv);											Arr.Add (cs);
		cs.Format("         p : 접합선 방향의 BOLT의 평균 핏치 (mm) = 플랜지 전폭 / 횡방향볼트수 = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n : 접합선 직각의 BOLT의 개수");																	Arr.Add (cs);
		cs.Format("                ┌ %10.1f x %10.1f   %10.1f x %10.1f ┐     %4.1f mm", Ss, Qs, Sv, Qv, Pb);					Arr.Add (cs);
		cs.Format("         ρh =  │------------------------- + ---------------------- │ x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                └      %12.1f           %12.1f      ┘      %d EA", Is, Iv, Nl);							Arr.Add (cs);
	}
	cs.Format("       c) 합성응력의 검토");													Arr.Add (cs);
	cs.Format("               __________________     _________________________");			Arr.Add (cs);
	if(bHybrid)
	{
		if(R < dHyR*Ra)	cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  <  R x ρa   O.K", Rp, Rh, R);
		else			cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  >  R x ρa   N.G", Rp, Rh, R);
	}
	else
	{
		if(R < Ra)	cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  <  ρa   O.K", Rp, Rh, R);
		else		cs.Format("         ρ = √( ρρ²+ ρh ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  >  ρa   N.G", Rp, Rh, R);
	}
	
	Arr.Add (cs);

	if (!bTension)
		return;

	As = Tf * (Wf - Dh * Nb);
	cs.Format("    4) 모재의 응력 검토" );																							Arr.Add (cs);
	cs.Format("       플랜지의 총단면적                     :  As  =  %.1f  x  %.1f  =  %.1f mm²", Tf, Wf, Tf*Wf);					Arr.Add (cs);
	cs.Format("       볼트 구멍을 공제한 플랜지의 순단면적  :  Asn =  %.1f x (%.1f - %.1f x %d)  =  %.1f mm²", Tf, Wf, Dh, Nb, As);	Arr.Add (cs);
	cs.Format("                            %6.1f  x  %6.1f", Tf*Wf, Sf);															Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf / As)	cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² <  R x f a ,  O.K", Tf*Wf*Sf/As);
		else							cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² >  R x f a ,  N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf / As)	cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² <  f a ,  O.K", Tf*Wf*Sf/As);
		else					cs.Format("       모재의 응력  f  =  ---------------------  =   %.2f  N/cm² >  f a ,  N.G", Tf*Wf*Sf/As);
	}
	
	Arr.Add (cs);
	MapSplice["인장모재작용응력_DN"] = Tf*Wf*Sf/As;
	cs.Format("                                 %6.1f", As);		Arr.Add (cs);
}
//	Ss : 합성전 전단력
//	Sv : 합성후 전단력
//	Ms : 합성전 고정하중 모멘트
//	Mv : 합성후 고정하중 모멘트
//	Su : 상판 응력
//	Sl : 하판 응력
//	Sa : 강재 허용 응력
//	Ra : 볼트 허용 강도 (2면 마찰 기준)
void CPlateStressOut::SpliceWebCheck(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Ms, double Mv,
								 double Su, double Sl, double Sa, double Ra, CMap<CString, LPCTSTR, double, double> &MapSplice)

{
//  여기서의 내부 변수들의 단위는 편의를 위해 기본적으로 cm, kgf으로 하였음.
	CPlateBridgeApp *pDB = m_pStd->GetBridge();
	CPlateGirderApp *pGir= pDB->GetGirder(nG);
	CPlateBasicIndex *pBx = pDB->GetGirder(nG)->GetBxBySpliceNoSPType(nSp);
	CPlateSplice     *pSp = pBx->GetSplice();
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	double dSta = pBx->GetStation();
	double Sw, Tw, Hw, Lb, Lp, Lps=0, Ls, Ho=0, Rb, Rbp=0, RbMax = 0.0;
	int Nl;

	ModelCalc.GetSectionCoefficient(pBx,TRUE);
	StressCalc.GetSectionStress(nG, dSta);

	Su  = max (Sa * 0.75, fabs (Su));
	Sl  = max (Sa * 0.75, fabs (Sl));
	Sw  = max (Su, Sl);
	Rbp	= Sw;
	Tw  = pGir->GetThickFactByStation(dSta, G_W);		// 복부판 두께
	Hw  = pBx->GetHeightGirderByBx();					// 복부판 높이
	Ho	= Hw/2;
	Lb  = (Hw - ((pSp->m_mD + pSp->m_mF * pSp->m_mE)* 2 + pSp->m_sF*pSp->m_sE + pSp->m_sF1*pSp->m_sE1*2)) / 2;  // 이음판 1,2 사이의 볼트거리

	CString cs;
	cs.Format("    1) BOLT의 응력 계산");
	Arr.Add (cs);
	Arr.Add("");
	cs.Format("       a) 휨에 의한 BOLT의 응력");
	Arr.Add (cs);
	
	BOOL bMoment = FALSE;	//모멘트 여부
	long ni;				//인덱스 조정자
	double d1;
	double d2;
	
	d1 = (pSp->m_mD + pSp->m_mE * pSp->m_mF) * 2;
	d2 = pSp->m_sE * pSp->m_sF + (pSp->m_sE1 * pSp->m_sF1)*2;
					
	for (int i=0; i<5 ; i++)  
	{
		if (i <= pSp->m_mE && pSp->m_mE >0)	 //모멘트 이음판
		{  
			bMoment = TRUE;
			Nl = (int) pSp->m_mB + 1;	// 모멘트 이음판 라인당 볼트수
			
			if (i == 0)					// 모멘트 이음판 첫줄							
				Lp = pSp->m_mD + pSp->m_mF / 2;	
			else if (i == pSp->m_mE)	// 모멘트 이음판 마지막줄						
				Lp = pSp->m_mF / 2 + (Hw - (d1 +d2))/4;	
			else						// 모멘트 이음판 중간줄				
				Lp = pSp->m_mF;					
		}
		else	// 전단 이음판
		{  
			ni = bMoment ? i-pSp->m_mE-1 : i-pSp->m_mE;
			if(pSp->m_sE1 > 0) // sE1 > 0
			{
				if(ni ==0)
					Lp = bMoment ? pSp->m_sF1 /2 + (Hw-(d1+d2))/4 :	 pSp->m_sF1 / 2 + Lb;
				else if(pSp->m_sE1 > ni)
					Lp = pSp->m_sF1;
				else if(pSp->m_sE1 == ni)
					Lp = pSp->m_sF1/2 +pSp->m_sF/2;
				else
					Lp = pSp->m_sF;
			}
			else // sE1 ==0
			{
				if (ni == 0)	//전단 이음판 첫번째 열
					Lp = bMoment ? (pSp->m_sF /2) + (Hw-(d1 + d2)) /4 :	 pSp->m_sF / 2 + Lb;
				else if(pSp->m_sE > ni)	// 전단 이음판 중간 열				
					Lp = pSp->m_sF;				
			}	
			Nl = (int) pSp->m_sB + 1;	// 전단 이음판 라인당 볼트수
		}

		Lps	+= Lp;
		Ls	= Sw*(Ho-Lps)/Ho;
		Rb	= (Rbp+Ls)/2 * Lp * Tw / Nl;		
		cs.Format("                %6.1f  +  %6.1f",  Rbp, Ls);
		Arr.Add (cs);
		if (Rb < Ra)
			cs.Format("         %d열 : ------------------- x  %4.1f  x  %.1f mm  ÷ %d  =  %6.1f N/EA  <  ρa   O.K", i+1, Lp, Tw, Nl, Rb);
		else
			cs.Format("         %d열 : ------------------- x  %4.1f  x  %.1f mm  ÷ %d  =  %6.1f N/EA  >  ρa   N.G", i+1, Lp, Tw, Nl, Rb);
		Arr.Add (cs);
		cs.Format("                         2");
		Arr.Add (cs);

		Rbp = Ls;
		RbMax = max(RbMax, Rb);

		cs.Format("BOLT_ALLOW_%d", i+1);
		MapSplice[cs] = Rb;	
	}
	
	MapSplice["RbMax"] = RbMax;

	double Mt, Tfu, Rs, R, K;
	int Nt;
	Mt  = fabs (StressCalc.GetElementForce (ELE_AXTORSN, 0, nG, dSta, 4));   // Torsion
	Tfu = pGir->GetThickJewon(G_F_U, pBx);
	Nt  = (pSp->m_mB+1)*2*(pSp->m_mE+1) + (pSp->m_sB+1)*(pSp->m_sE+pSp->m_sE1*2+1);  // 볼트 총 개수
	K   = ModelCalc.GetMapValueSecCo("K");
	Rs  = (fabs(Ss + Sv) + fabs(Mt*Tw)/K)/Nt;
	R   = hypot (RbMax, Rs);
	double dFlangeUpW = pGir->GetWidthOnStation(dSta, TRUE);
	double dFlangeDnW = pGir->GetWidthOnStation(dSta, FALSE);
	Arr.Add("");
	cs.Format("       b) 전단력에 대한 BOLT의 응력" );								Arr.Add (cs);
	cs.Format("         ⊙ 복부판이 부담하는 전단력 : Ss+Sv");						Arr.Add (cs);
	cs.Format("         ⊙ 비틀림 모멘트에 의한 복부의 전단력 : St=Mt·t/ K");		Arr.Add (cs);
	cs.Format("         ⊙ K = ∑(1/3)·b·t³   ( 도·설 해설 P 273 참조 )");		Arr.Add (cs);
	cs.Format("              = ( %.1lf × %.1lf³ + %.1lf × %.1lf³+ %.1lf × %.1lf³)/3 = %.1lf mm⁴"
		, dFlangeUpW, Tfu, Hw, Tw, dFlangeUpW, Tfu, K);					
	Arr.Add (cs);

	if (Rs < Ra)
		cs.Format("         ρs = (%.1lf +  (%.2lf × %.1lf)/ %.0lf ) / %d EA  =  %.2f N/EA  <  ρa   O.K", fabs(Ss+Sv), Mt, Tw, K, Nt, Rs);
	else
		cs.Format("         ρs = (%.1lf +  (%.2lf × %.1lf)/ %.0lf ) / %d EA  =  %.2f N/EA  >  ρa   N.G", fabs(Ss+Sv), Mt, Tw, K, Nt, Rs);
	Arr.Add (cs);
	MapSplice["SHEAR_BOLT_STRESS"] = Rs;
	Arr.Add("");
	cs.Format("       c) 합성응력의 검토" );
	Arr.Add (cs);
	cs.Format("                __________________     _________________________");
	Arr.Add (cs);
	if (R < Ra)
		cs.Format("         ρ  = √( ρP²+ ρs ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  <  ρa   O.K", RbMax, Rs, R);
	else
		cs.Format("         ρ  = √( ρP²+ ρs ²)  = √( %.2f² +  %.2f²) =  %.2f N/EA  >  ρa   N.G", RbMax, Rs, R);
	Arr.Add (cs);
	MapSplice["WEB_COMPOSE_STRESS"] = R;
	double Iv, Isw2, Ivw2, dTable[3][8], *pSum;
	double Ts1, Ts2, Hs1, Hs2, Ys1, I, Isw, Ivw, Msw, Mvw, Ssu, Ssl;

	double Ysu  = MapSplice["Ms"]+MapSplice["Mv"]>0 ? MapSplice["Ys_UP"] : MapSplice["Ys_DN"];
	double Yvsu = MapSplice["Ms"]+MapSplice["Mv"]>0 ? MapSplice["Yv_UP"] : MapSplice["Yv_DN"];
	double Ysl  = Hw+MapSplice["Tf_UP"]+MapSplice["Tf_DN"]-Ysu;
	double Yvsl = Hw+MapSplice["Tf_UP"]+MapSplice["Tf_DN"]-Yvsu;
	double Is   = ModelCalc.GetMapValueSecCo("I33");
	
	// 정.부 모멘트 구간의 결정은 합성전과 합성후의 각각의 모멘트의 합으로 결정한다.
	Iv = ModelCalc.GetSectionCoeffI33(AFTER_COMPOSITE_ANALYSIS);
	/*if ( (Ms+Mv) > 0.0)
		Iv = ModelCalc.GetMapValueSecCo("Iv33");
	else	
		Iv = StressCalc.GetMapValueSecStress("Irv");*/

	Isw2 = Tw * pow(Hw,3) / 12 + Tw * Hw * pow(Hw/2 - Ysl, 2);
	Ivw2 = Tw * pow(Hw,3) / 12 + Tw * Hw * pow(Yvsl - Hw/2, 2);
	Ms   = fabs (Ms);
	Mv   = fabs (Mv);
	Msw  = Ms * Isw2 / Is;
	Mvw  = Mv * Ivw2 / Iv;
	Ts1  = pSp->m_mT;
	Ts2  = pSp->m_sT;
	Hs1  = pSp->m_mW;
	Hs2  = pSp->m_sW;
	Ys1 = Hw / 2 - (pSp->m_mG + Hs1 / 2);

	Arr.Add("");
	cs.Format("    2) 이음판의 응력계산" );
	Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())
	{  // 비합성형
		cs.Format("                Mw");
		Arr.Add (cs);
		cs.Format("        fu = --------- x Ysu");
		Arr.Add (cs);
		cs.Format("                Isw");
		Arr.Add (cs);
		cs.Format("                Mw");
		Arr.Add (cs);
		cs.Format("        fl = --------- x Ysl");
		Arr.Add (cs);
		cs.Format("                Isw");
		Arr.Add (cs);
		cs.Format("        fu  : 이음판의 상연응력");
		Arr.Add (cs);
		cs.Format("        fl  : 이음판의 하연응력");
		Arr.Add (cs);
		cs.Format("        Mw  : 복부에 작용하는 모멘트,  M =  %.2f kN.m ", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("        Isw : 강재보단면의 중립축에 대한 이음판의 총단면 2차 모멘트");
		Arr.Add (cs);
		cs.Format("        Ysu, Ysl : 강재보 단면의 중립축에서 이음판 상,하연까지의 거리");
		Arr.Add (cs);
		cs.Format("        Ysu  =  %.2f mm,      Ysl  =  %.2f mm", Ysu, Ysl);
		Arr.Add (cs);
		cs.Format("        Is  : 강재보 단면의 단면 2차 모멘트,   Is = %.2f mm⁴" , Is);
		Arr.Add (cs);
		cs.Format("                %4.1f x %5.1f³", Tw, Hw);
		Arr.Add (cs);
		cs.Format("        Isw' = ---------------  + (%5.1f²+ %5.1f²) + %4.1f x %5.1f x ( %6.2f - %6.2f )² =  %.2f mm⁴", Hw, Tw, Tw, Hw, Hw/2, Ysl, Isw2);
		Arr.Add (cs);
		cs.Format("                     12");
		Arr.Add (cs);
		cs.Format("                  Isw'                 %.1f", Isw2);
		Arr.Add (cs);
		cs.Format("        Msw = Ms ------  = %8.2f x ------------  =  %.2f kN.m", tokNM(Ms), tokN(Msw));
		Arr.Add (cs);
		cs.Format("                   Is                  %.1f", Is);
		Arr.Add (cs);
	}
	else   {
		cs.Format("                Msw                 Mvw");
		Arr.Add (cs);
		cs.Format("        fu = --------- x Ysu  +  --------- x Yvu");
		Arr.Add (cs);
		cs.Format("                Isw                 Ivw");
		Arr.Add (cs);
		cs.Format("                Msw                 Mvw");
		Arr.Add (cs);
		cs.Format("        fl = --------- x Ysl  +  --------- x Yvl");
		Arr.Add (cs);
		cs.Format("                Isw                 Ivw");
		Arr.Add (cs);
		cs.Format("        fu  : 이음판의 상연응력");
		Arr.Add (cs);
		cs.Format("        fl  : 이음판의 하연응력");
		Arr.Add (cs);
		cs.Format("        Msw : 복부에 작용하는 합성전 고정하중에 의한 모멘트,  Ms =  %.2f kN.m ", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("        Mvw : 복부에 작용하는 합성후 고정하중에 의한 모멘트,  Mv =  %.2f kN.m ", tokNM(Mv));
		Arr.Add (cs);
		cs.Format("        Isw : 강재보단면의 중립축에 대한 이음판의 총단면 2차 모멘트");
		Arr.Add (cs);
		cs.Format("        Ivw : 합성단면의 중립축에 대한 이음판의 총단면 2차 모멘트");
		Arr.Add (cs);
		cs.Format("        Ysu, Ysl : 강재보 단면의 중립축에서 이음판 상,하연까지의 거리");
		Arr.Add (cs);
		cs.Format("        Ysu  =  %.2f mm,      Ysl  =  %.2f mm", Ysu, Ysl);
		Arr.Add (cs);
		cs.Format("        Yvu, Yvl : 합성단면의 중립축에서 이음판 상,하연까지의 거리");
		Arr.Add (cs);
		cs.Format("        Yvu  =  %.2f mm,      Yvl  =  %.2f mm", Yvsu, Yvsl);
		Arr.Add (cs);
		cs.Format("        ls  : 강재보 단면의 단면 2차 모멘트,   ls = %.2f mm⁴" , Is);
		Arr.Add (cs);
		cs.Format("        lv  : 합성 단면의 단면 2차 모멘트,     lv = %.2f mm⁴" , Iv);
		Arr.Add (cs);
		cs.Format("                %4.1f x %5.1f³", Tw, Hw);
		Arr.Add (cs);
		cs.Format("        Isw' = ---------------  + (%5.1f² + %5.1f²) +  %4.1f x %5.1f x ( %6.2f - %6.2f )² =  %.2f mm⁴", Hw, Tw, Tw, Hw, Hw/2, Ysl, Isw2);
		Arr.Add (cs);
		cs.Format("                     12");
		Arr.Add (cs);
		cs.Format("                %4.1f x %5.1f³", Tw, Hw);
		Arr.Add (cs);
		cs.Format("        Ivw' = ---------------  + (%5.1f² + %5.1f²) + %4.1f x %5.1f x ( %6.2f - %6.2f )² =  %.2f mm⁴", Hw, Tw, Tw, Hw, Yvsl, Hw/2, Ivw2);
		Arr.Add (cs);
		cs.Format("                     12");
		Arr.Add (cs);
		cs.Format("                  Isw'                 %.1f", Isw2);
		Arr.Add (cs);
		cs.Format("        Msw = Ms ------  = %8.2f x ------------  =  %.2f kN.m", tokNM(Ms), tokNM(Msw));
		Arr.Add (cs);
		cs.Format("                   Is                  %.1f", Is);
		Arr.Add (cs);
		cs.Format("                  Ivw'                 %.1f", Ivw2);
		Arr.Add (cs);
		cs.Format("        Mvw = Mv ------  = %8.2f x ------------  =  %.2f kN.m", tokNM(Mv), tokNM(Mvw));
		Arr.Add (cs);
		cs.Format("                   Iv                  %.1f", Iv);
		Arr.Add (cs);
	}
	dTable [0][0] = 4;
	dTable [0][1] = Hs1;
	dTable [0][2] = Ts1;
	dTable [0][4] = Ys1;
	dTable [1][0] = 2;
	dTable [1][1] = Hs2;
	dTable [1][2] = Ts2;
	dTable [1][4] = 0;
	dTable [0][7] = dTable [1][7] = -1.0;  // 수직부재
	MakeIoTable (Arr, dTable, 2, 7);
	pSum = dTable [2];
	I    = pSum [7] + pSum [6];
	Isw  = I + pSum [3] * pow (Hw/2 - Ysl, 2);
	Ivw  = I + pSum [3] * pow (Yvsl - Hw/2, 2);
	Ssu  = Msw * Ysu / Isw + Mvw * Yvsu / Ivw;
	Ssl  = Msw * Ysl / Isw + Mvw * Yvsl / Ivw;
	cs.Format("        I   =  lo + AY² =  %.1f  +  %.1f  =  %.2f", pSum [7], pSum [6], I);
	Arr.Add (cs);
	cs.Format("        Isw =  %.1f  +  %.2f x ( %6.2f - %6.2f )² =  %.2f mm⁴", I, pSum [3], Hw/2, Ysl, Isw);
	Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())
	{  // 비합성형
		cs.Format("        이음판의 응력 " );
		Arr.Add (cs);
		cs.Format("               %8.2f x 100000", tokNM(Msw));
		Arr.Add (cs);
		if (fabs (Ssu) < Sa)
			cs.Format("        f u = ------------------- x %6.2f  = %8.2f MPa <  f sa    O.K", Ysu, Ssu);
		else
			cs.Format("        f u = ------------------- x %6.2f  = %8.2f MPa >  f sa    N.G", Ysu, Ssu);
		Arr.Add (cs);
		cs.Format("                   %9.1f", Isw);
		Arr.Add (cs);
		cs.Format("               %8.2f x 100000", tokN(Msw));
		Arr.Add (cs);
		if (fabs (Ssl) < Sa)
			cs.Format("        f l = ------------------- x %6.2f  = %8.2f MPa <  f sa    O.K", Ysl, Ssl);
		else
			cs.Format("        f l = ------------------- x %6.2f  = %8.2f MPa >  f sa    N.G", Ysl, Ssl);
		Arr.Add (cs);
		cs.Format("                   %9.1f", Isw);
		Arr.Add (cs);
	}
	else
	{
		cs.Format("        Ivw =  %.1f  +  %.2f x ( %6.2f - %6.2f )² =  %.2f mm⁴", I, pSum [3], Yvsl, Hw/2, Ivw);
		Arr.Add (cs);
		cs.Format("        이음판의 응력 " );
		Arr.Add (cs);
		cs.Format("               %8.2f x 100000               %8.2f x 100000", tokNM(Msw), tokNM(Mvw));
		Arr.Add (cs);
		cs.Format("        f u = ------------------- x %6.2f  +  ------------------  x %6.2f", Ysu, Yvsu);
		Arr.Add (cs);
		cs.Format("                   %9.1f                       %9.1f ", Isw, Ivw);
		Arr.Add (cs);
		if (fabs (Ssu) < Sa)
			cs.Format("            =  %.2f  +  %.2f  = %8.2f MPa <  f sa    O.K", Msw*Ysu/Isw, Mvw*Yvsu/Ivw, Ssu);
		else
			cs.Format("            =  %.2f  +  %.2f  = %8.2f MPa >  f sa    N.G", Msw*Ysu/Isw, Mvw*Yvsu/Ivw, Ssu);
		Arr.Add (cs);
		cs.Format("               %8.2f x 100000               %8.2f x 100000", tokNM(Msw), tokNM(Mvw));
		Arr.Add (cs);
		cs.Format("        f l = ------------------- x %6.2f  +  ------------------  x %6.2f", Ysl, Yvsl);
		Arr.Add (cs);
		cs.Format("                   %9.1f                       %9.1f ", Isw, Ivw);
		Arr.Add (cs);
		if (fabs (Ssl) < Sa)
			cs.Format("            =  %.2f  +  %.2f  = %8.2f MPa <  f sa    O.K", Msw*Ysl/Isw, Mvw*Yvsl/Ivw, Ssl);
		else
			cs.Format("            =  %.2f  +  %.2f  = %8.2f MPa >  f sa    N.G", Msw*Ysl/Isw, Mvw*Yvsl/Ivw, Ssl);
		Arr.Add (cs);
	}
	MapSplice["WEB_STRESS"] = max(Ssu, Ssl);
	MapSplice["WEB_ALLOW_STRESS"] = Sa;
	MapSplice["UP_WEB_STRESS"] = Ssu;
	MapSplice["DN_WEB_STRESS"] = Ssl;
}


////////////////////////////////////////////////////////////////////
//  목적 :  I값 구하는 도표그리기.
//  입력 :  Arr           :  도표 String을 저장할 CStringArray.
//          pTable [][0]  :  Plate 개수
//          pTable [][1]  :  Plate 폭
//          pTable [][2]  :  Plate 두께
//          pTable [][4]  :  Y값
//          pTable [][7]  :  Plate 방향 ( + : 수평,  - : 수직)
//          nCount        :  입력한 Line수
//          nLMargin      :  왼쪽 여백 값
//  출력 :  pTable [][3]  :  A값 (면적)
//          pTable [][5]  :  AY값
//          pTable [][6]  :  AY²값
//          pTable [][7]  :  Io값
//          nCount번째 Line(입력한 Line 다음)에는 합계값 들어있음.
//
void CPlateStressOut::MakeIoTable(CStringArray &Arr, double pTable[][8], int nCount, int nLMargin)
{
	// 합계 Line 초기화.
	int i = 0;
	for (i = 3; i < 8; i++)
		pTable [nCount][i] = 0.0;

	// Table값 작성.
	for (i = 0; i < nCount; i++)   {
		pTable [i][3] = pTable [i][0] * pTable [i][1] * pTable [i][2];   // 'A'
		pTable [i][5] = pTable [i][3] * pTable [i][4];                   // 'AY'
		pTable [i][6] = pTable [i][4] * pTable [i][5];                   // 'AY²'
		if (pTable [i][7] > 0)   // 수평 부재면
			pTable [i][7] = pTable [i][0] * pTable [i][1] * pow (pTable [i][2], 3) / 12;   // 'Io'
		else   // 수직 부재면
			pTable [i][7] = pTable [i][0] * pTable [i][2] * pow (pTable [i][1], 3) / 12;   // 'Io'
		for (int j = 3; j < 8; j++)   // 합계 계산.
			pTable [nCount][j] += pTable [i][j];
	}
	// 도표 출력.
	CString cs, sLM;
	sLM.Format("%*c", nLMargin, ' ');
	cs.Format("%s┏━━━━━━━━━━┳━━━━━┳━━━━┳━━━━━┳━━━━━━┳━━━━━━━┓", sLM);
	Arr.Add (cs);
//	cs.Format("%s┃                    ┃    A   ┃    Y   ┃    AY    ┃     AY²   ┃     Io     ┃", sLM);
	cs.Format("%s┃                    ┃ A (mm²) ┃  Y (mm)┃ AY (mm³)┃  AY²(mm⁴)┃   Io (m⁴)   ┃", sLM);
	Arr.Add (cs);
	cs.Format("%s┣━━━━━━━━━━╋━━━━━╋━━━━╋━━━━━╋━━━━━━╋━━━━━━━┫", sLM);
	Arr.Add (cs);
	for (i = 0; i < nCount; i++)
	{
		if (pTable [i][4] != 0.0)
			cs.Format("%s┃%2g - PL%6g x%4g ┃%9.2f ┃%7.2f ┃%9.1f ┃%11.1f ┃%13.1f ┃", sLM,
						pTable[i][0], pTable[i][1], pTable[i][2], pTable[i][3],
						pTable[i][4], pTable[i][5], pTable[i][6], pTable[i][7]);
		else   // Y값 0이면 ' - ' 출력.
			cs.Format("%s┃%2g - PL%6g x%4g ┃%9.2f ┃    -   ┃     -    ┃      -     ┃%13.1f ┃", sLM,
						pTable [i][0], pTable [i][1], pTable [i][2], pTable [i][3], pTable [i][7]);
		Arr.Add (cs);
	}
//		         ┃ 4 - PL   151 x  12 ┃  42.40 ┃ 102.25 ┃ 123456.0 ┃    12578.1 ┃   123885.0 ┃
//		         ┃10 - PL1510.5 x12.5 ┃1000.00 ┃ 150.00 ┃1500000.0 ┃123456789.1 ┃   123885.5 ┃
	cs.Format("%s┣━━━━━━━━━━╋━━━━━╋━━━━╋━━━━━╋━━━━━━╋━━━━━━━┫", sLM);
	Arr.Add (cs);
	cs.Format("%s┃     합      계     ┃%9.2f ┃        ┃%9.1f ┃%11.1f ┃%13.1f ┃", sLM,
						pTable [nCount][3], pTable [nCount][5], pTable [nCount][6], pTable [nCount][7]);
	Arr.Add (cs);
	cs.Format("%s┗━━━━━━━━━━┻━━━━━┻━━━━┻━━━━━┻━━━━━━┻━━━━━━━┛", sLM);
	Arr.Add (cs);
}


void CPlateStressOut::StressCheckCoefOutput(CStringArray &Arr, long nG, double dSta, BOOL bDistance)
{
	CCalcData		*pData		= m_pStd->GetDataManage()->GetCalcData();
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CSteel			*pSteelCtl	= m_pStd->GetDataManage()->GetSteel();
	CCalcGeneral	CalcGen(m_pStd->GetDataManage());

	BOOL bWind    = pData->m_bWind;
	BOOL bHAPSUNG = TRUE;
	if(pDB->GetNumHaiseokCombo()) bHAPSUNG = FALSE;

	CPlateGirderApp  *pGir = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx  = pGir->GetBxByStation(dSta);
	double dRatio =0, dRatioElm=0;
	long nNode = ModelCalc.GetJointNoByStation(nG, dSta, dRatio);
	long nElm  = ModelCalc.GetElemNoAndRatio(nG, dSta, dRatioElm);
	double dDistance	= 0;

	if(bDistance)
	{
		if(dRatio < 0)
		{
			nNode--;
			dRatio += 1;
		}
		dDistance = ModelCalc.GetElemLength(nElm, FALSE) * dRatio;
	}

	CString cs,str;
	if(bDistance)	cs.Format("< 단면 응력 검토 >  거더%d, 절점%d 으로부터 %.3fm", nG+1, nNode+1, dDistance);
	else			cs.Format("< 단면 응력 검토 >  거더%d, 절점%d", nG+1, nNode+1);

	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(!bHAPSUNG)	return;
	// 합성 교량 일때
	BOOL bPLUS_M	= StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);
	StressCalc.GetSectionStress(nG, dSta, bHAPSUNG);
	ModelCalc.GetSectionCoefficient(nG, dSta, bHAPSUNG);
	
	BOOL	bHybrid	= pBx->IsHybrid();
	double	dHyR	= StressCalc.GetMapValueSecStress("HyR_Pos");
	double	up_f_w	= ModelCalc.m_MapSecCoe["FLANGE_W_UP"];//pGir->GetWidthOnStation(dSta, TRUE);
	double	up_f_t	= ModelCalc.m_MapSecCoe["FLANGE_T_UP"];//pGir->GetThickFactByStation(dSta, G_F_U);
	double	lo_f_w	= ModelCalc.m_MapSecCoe["FLANGE_W_DN"];//pGir->GetWidthOnStation(dSta, FALSE);
	double	lo_f_t	= ModelCalc.m_MapSecCoe["FLANGE_T_DN"];//pGir->GetThickFactByStation(dSta, G_F_L);
	double	w_h		= ModelCalc.m_MapSecCoe["WEB_H"];//Round(pGir->GetHeightGirderByStaAng(dSta), 0);		
	double	w_t		= ModelCalc.m_MapSecCoe["WEB_T"];//pGir->GetThickFactByStation(dSta, G_W);
	double	hunch	= ModelCalc.m_MapSecCoe["HUNCH"];//pDB->m_dThickSlabHunch;
	double	slab_t	= ModelCalc.m_MapSecCoe["SLAB_THICK"];//pDB->m_dThickSlabBase-hunch;

	// 사용강종 구하기
	CString sType, sNum, sJuType, sLeft, sRight;
	CalcGen.GetMaterial(pBx, G_F_U, sLeft, sRight);
	sJuType = pDB->GetMaterial(G_F_U, pBx);
	//////////////////////////////////////
	double H	= w_h;
	double B	= ModelCalc.GetEffectWidthSlab(nG, dSta);
	double Bu	= up_f_w;
	double Bl	= lo_f_w;
	double Tc	= slab_t;
	double Th	= hunch;
	double Tu	= up_f_t;
	double Tl	= lo_f_t;
	double Tw	= w_t;
	double k	= StressCalc.GetMapValueSecStress("K");
	double R	= StressCalc.GetMapValueSecStress("HyR_Pos");

	cs.Format("1) 단면 가정 및 설계조건 ");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	CalcGen.GetMaterial(pBx, G_F_U, sLeft, sRight);
	cs.Format("					                     주부재 사용강종  : 상부 플랜지 %s%s",			sLeft, sRight);	Arr.Add(cs);
	CalcGen.GetMaterial(pBx, G_F_L, sLeft, sRight);
	cs.Format("					                                        하부 플랜지 %s%s",			sLeft, sRight);	Arr.Add(cs);
	CalcGen.GetMaterial(pBx, G_W, sLeft, sRight);
	cs.Format("					                                        복부판 %s%s",				sLeft, sRight);	Arr.Add(cs);
	cs.Format("					                     H    = %6.1f mm (거더복부의 높이)",	H);			Arr.Add(cs);
	cs.Format("					                     B    = %6.1f mm (슬래브 유효폭)",		B);			Arr.Add(cs);
	cs.Format("					                     Bu   = %6.1f mm (플랜지 상부의 너비)",	Bu);		Arr.Add(cs);
	cs.Format("					                     Bl   = %6.1f mm (플랜지 하부의 너비)",	Bl);		Arr.Add(cs);
	cs.Format("					                     Tc   = %6.1f mm (슬래브 두께)",		Tc);		Arr.Add(cs);
	cs.Format("					                     Th   = %6.1f mm (헌치 높이)",			Th);		Arr.Add(cs);
	cs.Format("					                     Tu   = %6.1f mm (상판 두께)",			Tu);		Arr.Add(cs);
	cs.Format("					                     Tl   = %6.1f mm (하판 두께)",			Tl);		Arr.Add(cs);
	cs.Format("					                     Tw   = %6.1f mm (WEB 두께)",			Tw);		Arr.Add(cs);
	cs.Format("					                     k    = %6.3f    (모멘트 영향계수)",	k);			Arr.Add(cs);
	if(pBx->IsHybrid())
	{
		cs.Format("					                     R    = %6.3f    (하이브리드 계수)",	R);		Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	///////////////////////////////////////////
	double dEst			= pData->DESIGN_MATERIAL.m_dEst;
	double Ec			= pData->DESIGN_MATERIAL.m_dEc;
	double Alpha		= pData->DESIGN_MATERIAL.m_dAlpha;
	double Epsilon		= StressCalc.GetMapValueSecStress("Epsilon");
	//double Alpha		= StressCalc.GetMapValueSecStress("Alpha");
	double CreepCoef2	= StressCalc.GetMapValueSecStress("CreepCoef2");
	double n			= StressCalc.GetMapValueSecStress("N");
	double t			= StressCalc.GetMapValueSecStress("Temperature");

	double	Ms  = tokNM(StressCalc.GetMapValueSecStress("MomentPreDead"));
	double	Msc = tokNM(StressCalc.GetMapValueSecStress("MomentPostDead"));
	double	Mw	= tokNM(StressCalc.GetMapValueSecStress("MomentWind"));
	double	Mv  = tokNM(StressCalc.GetMapValueSecStress("MomentLive"));
	double	Ss	= tokN(StressCalc.GetMapValueSecStress("ShearPreDead"));
	double	Ssc	= tokN(StressCalc.GetMapValueSecStress("ShearPostDead"));
	double	Sw	= tokN(StressCalc.GetMapValueSecStress("ShearWind"));
	double	Sv	= tokN(StressCalc.GetMapValueSecStress("ShearLive"));

	double	Mst	= tokNM(StressCalc.GetMapValueSecStress("TorsionPreDead"));
	double	Msct= tokNM(StressCalc.GetMapValueSecStress("TorsionPostDead"));
	double	Mwt	= tokNM(StressCalc.GetMapValueSecStress("TorsionWind" ));
	double	Mvt	= tokNM(StressCalc.GetMapValueSecStress("TorsionLive"));

	double	tcu	= StressCalc.GetMapValueSecStress("tcu");
	double	tcl	= StressCalc.GetMapValueSecStress("tcl");
	double	Aru	= StressCalc.GetMapValueSecStress("Aru");
	double	Arl	= StressCalc.GetMapValueSecStress("Arl");
	double	Ar	= Aru + Arl;
	
	///////////////////////////////////////////

	cs.Format(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━┳━━━━━━━━━┓");		Arr.Add(cs);
	cs.Format(" ┃                   구            분                         ┃ 단 위  ┃  입력 DATA       ┃");		Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Es  : 강의 탄성계수                                        ┃  MPa   ┃%15.1f   ┃",dEst);			Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Ec  : 콘크리트의 탄성계수                                  ┃  MPa   ┃%15.1f   ┃",Ec);				Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ εs : 바닥판 콘크리트의 건조수축에 의한 최종수축률         ┃        ┃%15.5f   ┃",Epsilon);		Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ α  : 강재 및 콘크리트의 선팽창계수                        ┃        ┃%15.6f   ┃",Alpha);			Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Φ2 : 건조수축에의한 크리이프계수                          ┃        ┃%15.f   ┃",CreepCoef2);		Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ n   : 강과 콘크리트의 탄성계수비                           ┃        ┃%15.f   ┃",n);				Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ t   : 강과 콘크리트의 온도차                               ┃  ℃    ┃%15.f   ┃", t);				Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Ms  : 합성전 고정하중에 의한 모멘트                        ┃ kN·m  ┃%15.3f   ┃",Ms);				Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Msc : 합성후 고정하중에 의한 모멘트                        ┃ kN·m  ┃%15.3f   ┃",Msc);			Arr.Add(cs);
	if(bWind)
	{
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Mw  : 풍하중에의한 모멘트                                  ┃ kN·m  ┃%15.3f   ┃",Mw);				Arr.Add(cs);
	}
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Mv  : (활하중 + 지점침하)에 의한 모멘트                    ┃ kN·m  ┃%15.3f   ┃",Mv);				Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Ss  : 합성전 고정하중에 의한 전단력                        ┃   kN   ┃%15.3f   ┃",Ss);				Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Ssc : 합성후 고정하중에 의한 전단력                        ┃   kN   ┃%15.3f   ┃",Ssc);			Arr.Add(cs);
	if(bWind)
	{
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Sw  : 풍하중에의한 전단력                                  ┃   kN   ┃%15.3f   ┃",Sw);				Arr.Add(cs);
	}
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Sv  : (활하중 + 지점침하)에 의한 전단력                    ┃   kN   ┃%15.3f   ┃",Sv);				Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Mst : 합성전 고정하중에 의한 비틀림 모멘트                 ┃ kN·m  ┃%15.3f   ┃",Mst);			Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Msct: 합성후 고정하중에 의한 비틀림 모멘트                 ┃ kN·m  ┃%15.3f   ┃",Msct);			Arr.Add(cs);
	if(bWind)
	{
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Mwt : 풍하중에의한 비틀림 모멘트                           ┃  kN    ┃%15.3f   ┃",Mwt);			Arr.Add(cs);
	}
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ Mvt : (활하중 + 지점침하)에 의한 비틀림 모멘트             ┃ kN·m  ┃%15.3f   ┃",Mvt);			Arr.Add(cs);
	if(!bPLUS_M)
	{
	cs.Format(" ┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━╋━━━━━━━━━┫");		Arr.Add(cs);
	cs.Format(" ┃ tc  : 교축방향 철근의 피복두께(상부,하부)                  ┃  mm    ┃   %5.1f, %5.1f   ┃",tcu,tcl);Arr.Add(cs);
	}
	cs.Format(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━┻━━━━━━━━━┛");		Arr.Add(cs);

	cs.Format("   < 참고 > 합성전 고정하중 : 강재자중 + 바닥판 콘크리트자중"); Arr.Add(cs);
	cs.Format("            합성후 고정하중 : 난간 및 방호벽 자중 + PAVEMENT자중");Arr.Add(cs);	
	cs.Format(""); Arr.Add(cs);
	
	/////////////////////////////////////////
	cs.Format("  단면 제원");	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(" ┏━━━━━━━━┳━━━━┳━━━━┳━━━━━┳━━━━━┳━━━━━━━┳━━━━━━━━━┳━━━━━━━━┓");	Arr.Add(cs);
	cs.Format(" ┃ 구         분  ┃ B(mm)  ┃ H(mm)  ┃  A(mm²) ┃  Y(mm)   ┃  A·Y(mm³)  ┃   A·Y²( mm⁴)  ┃    Io( mm⁴)   ┃");	Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━╋━━━━╋━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	cs.Format(" ┃1  -  UPPER FL. ┃  %5.0f ┃ %6.0f ┃ %8.1f ┃ %8.1f ┃ %12.1f ┃ %16.1f ┃ %14.1f ┃"
		,up_f_w, up_f_t, ModelCalc.GetMapValueSecCo("As_UP"), ModelCalc.GetMapValueSecCo("Y_UP")
		,ModelCalc.GetMapValueSecCo("Asy_UP"), ModelCalc.GetMapValueSecCo("Asy2_UP")
		,ModelCalc.GetMapValueSecCo("Iyy_UP"));	Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━╋━━━━╋━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	cs.Format(" ┃1  -     WEB.   ┃  %5.0f ┃ %6.0f ┃ %8.1f ┃ %8.1f ┃ %12.1f ┃ %16.1f ┃ %14.1f ┃"
		,w_t, w_h, ModelCalc.GetMapValueSecCo("As_WEB"), ModelCalc.GetMapValueSecCo("Y_WEB")
		,ModelCalc.GetMapValueSecCo("Asy_WEB"), ModelCalc.GetMapValueSecCo("Asy2_WEB"), ModelCalc.GetMapValueSecCo("Iyy_WEB"));		Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━╋━━━━╋━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	cs.Format(" ┃1  -  LOWER FL. ┃  %5.0f ┃ %6.0f ┃%9.1f ┃ %8.1f ┃ %12.1f ┃ %16.1f ┃ %14.1f ┃"
		,lo_f_w, lo_f_t, ModelCalc.GetMapValueSecCo("As_DN")
		,ModelCalc.GetMapValueSecCo("Y_DN"), ModelCalc.GetMapValueSecCo("Asy_DN")
		,ModelCalc.GetMapValueSecCo("Asy2_DN"), ModelCalc.GetMapValueSecCo("Iyy_DN"));	Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━╋━━━━┻━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	cs.Format(" ┃  강단면 합계   ┃                  ┃%9.1f ┃          ┃ %12.1f ┃ %16.1f ┃ %14.1f ┃"
		,ModelCalc.GetMapValueSecCo("As_SUM"), ModelCalc.GetMapValueSecCo("Asy_SUM")
		,ModelCalc.GetMapValueSecCo("Asy2_SUM"), ModelCalc.GetMapValueSecCo("Iyy_SUM")); Arr.Add(cs);
	
	if(!bPLUS_M)
	{
	cs.Format(" ┣━━━━━━━━╋━━━━┳━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	cs.Format(" ┃  UPPER RE-BAR  ┃                  ┃%9.1f ┃ %8.1f ┃ %12.1f ┃ %16.1f ┃              ┃"
		,Aru, (H/2+Th+Tc)-tcu, Aru*((H/2+Th+Tc)-tcu),Aru*pow(((H/2+Th+Tc)-tcu),2) );
	Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━╋━━━━╋━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	cs.Format(" ┃  LOWER RE-BAR  ┃                  ┃%9.1f ┃ %8.1f ┃ %12.1f ┃ %16.1f ┃              ┃"
		,Arl, (H/2+Th)+tcl, Arl*((H/2+Th)+tcl),Arl*pow(((H/2+Th)+tcl),2) );
	Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━╋━━━━╋━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	cs.Format(" ┃   RE-BAR 합계  ┃                  ┃%9.1f ┃        ┃ %12.1f ┃ %16.1f ┃              ┃"
		,Ar,Aru*((H/2+Th+Tc)-tcu) + Arl*((H/2+Th)+tcl),Aru*pow(((H/2+Th+Tc)-tcu),2) + Arl*pow(((H/2+Th)+tcl),2) );							Arr.Add(cs);
	cs.Format(" ┣━━━━━━━━╋━━━━╋━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	}
	else
	cs.Format(" ┣━━━━━━━━╋━━━━┳━━━━╋━━━━━╋━━━━━╋━━━━━━━╋━━━━━━━━━╋━━━━━━━━┫");	Arr.Add(cs);
	
	
	cs.Format(" ┃1  -  RC SLAB   ┃  %5.0f ┃ %6.0f ┃%9.1f ┃ %8.1f ┃ %12.1f ┃ %16.1f ┃ %14.1f ┃"
		,B,Tc,ModelCalc.GetMapValueSecCo("As_SLAB"), ModelCalc.GetMapValueSecCo("Y_SLAB")
		,ModelCalc.GetMapValueSecCo("Asy_SLAB"), ModelCalc.GetMapValueSecCo("Asy2_SLAB")
		,ModelCalc.GetMapValueSecCo("Iyy_SLAB"));	Arr.Add(cs);
	cs.Format(" ┗━━━━━━━━┻━━━━┻━━━━┻━━━━━┻━━━━━┻━━━━━━━┻━━━━━━━━━┻━━━━━━━━┛");	Arr.Add(cs);
	cs.Format("   * As = %7.2f mm², Ac = %7.2f mm²" ,ModelCalc.GetMapValueSecCo("As_SUM"), ModelCalc.GetMapValueSecCo("As_SLAB"));
	if(!bPLUS_M)
	{	
		str.Format(" , Ar = %7.2f mm²" , Ar);
		Arr.Add(cs+str);
	}
	else
	{
		Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("2) 응력 검토");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);    

	cs.Format("  (1) 합성전");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format(" Is´= (A ·Y²) + Io           = %.3f mm⁴", ModelCalc.GetMapValueSecCo("IsApo"));			Arr.Add(cs);
	cs.Format(" δs = ∑(A ·Y) / As           = %.3f mm", ModelCalc.GetMapValueSecCo("de"));				Arr.Add(cs);
	cs.Format(" Is = Is´- As ×(δs)²        = %.3f mm⁴", ModelCalc.GetMapValueSecCo("I33"));			Arr.Add(cs);
	cs.Format(" Ysu = H / 2 + Tu - δs         = %.3f mm", ModelCalc.GetMapValueSecCo("Ysu"));				Arr.Add(cs);
	cs.Format(" Ysl = Ysu - ( H + Tu + Tl )    = %.3f mm", ModelCalc.GetMapValueSecCo( "Ysl"));				Arr.Add(cs);
	cs.Format(" fsu = Ms / Is × Ysu           = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_su"));	Arr.Add(cs);
	cs.Format(" fsl = Ms / Is × Ysl           = %.3f mm⁴", StressCalc.GetMapValueSecStress( "Sigma_sl"));	Arr.Add(cs);
	cs.Format(" τs = Ss / A(web) + Mst ·Tw/K = %.3f mm⁴", StressCalc.GetMapValueSecStress( "Tau_s"));	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (2) 합성후");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 정모멘트
	{	
		cs.Format(" Av = As + Ac/n                 = %.3f mm²",ModelCalc.GetMapValueSecCo("Av_SUM"));	Arr.Add(cs);
		cs.Format(" Ic / n                         = %.3f mm⁴",ModelCalc.GetMapValueSecCo("Igcs"));	Arr.Add(cs);
		cs.Format(" dv = Ysu - Tu + Th + Tc/2      = %.3f mm",ModelCalc.GetMapValueSecCo("dv"));		Arr.Add(cs);
		cs.Format(" dvc = dv × As / ( Ac/n + As ) = %.3f mm",ModelCalc.GetMapValueSecCo("dvc"));		Arr.Add(cs);
		cs.Format(" dvs = dv - dvc                 = %.3f mm",ModelCalc.GetMapValueSecCo( "dvs"));		Arr.Add(cs);
		cs.Format(" Yvcu = dvc + Tc/2              = %.3f mm",ModelCalc.GetMapValueSecCo("Yvcu"));		Arr.Add(cs);
		cs.Format(" Yvcl = Yvcu - Tc               = %.3f mm",ModelCalc.GetMapValueSecCo("Yvcl"));		Arr.Add(cs);
		cs.Format(" Yvsu = Yvcu - H2 + Tu          = %.3f mm",ModelCalc.GetMapValueSecCo("Yvsu"));		Arr.Add(cs);
		cs.Format(" Yvsl = Yvsu - ( H + Tu + Tl )  = %.3f mm",ModelCalc.GetMapValueSecCo("Yvsl"));		Arr.Add(cs);
		cs.Format(" Iv = Is + Ic/n + As × dvs² + Ac/n × dvc² = %.3f mm⁴", ModelCalc.GetMapValueSecCo("Iv33"));	Arr.Add(cs);
		cs.Format(" fvcu = Msc × Yvcu / ( n ×Iv ) = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcu"));		Arr.Add(cs);
		cs.Format(" fvcl = Msc × Yvcl / ( n ×Iv ) = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcl"));		Arr.Add(cs);
		cs.Format(" fvsu = Msc × Yvsu / Iv         = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsu"));		Arr.Add(cs);
		cs.Format(" fvsl = Msc × Yvsl / Iv         = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsl")); 		Arr.Add(cs);
		cs.Format(" K    = K + B × (Tc / n)³/ 3   = %.3f mm⁴",ModelCalc.GetMapValueSecCo("Kv"));				Arr.Add(cs);
	}
	else    // 부모멘트
	{
		cs.Format(" Av = As + Ar = %.3f mm²"						,StressCalc.GetMapValueSecStress( "Av"));		Arr.Add(cs);
		cs.Format(" d = Ysu - Tu + Th + tcl + (Tc - tcu - tcl) ×Aru / Ar = %.3f mm",toCm(StressCalc.GetMapValueSecStress( "d")) );		Arr.Add(cs);
		cs.Format(" dr = d ×As / Av = %.3f mm"					,StressCalc.GetMapValueSecStress( "dr"));		Arr.Add(cs);
		cs.Format(" ds = d - dr = %.3f mm"						,StressCalc.GetMapValueSecStress( "ds"));		Arr.Add(cs);
		cs.Format(" Yvru = dr + (Tc - tcu - tcl) × Arl / Ar = %.3f mm",StressCalc.GetMapValueSecStress( "Yvru"));	Arr.Add(cs);
		cs.Format(" Yvrl = dr - (Tc - tcu - tcl) × Aru / Ar = %.3f mm",StressCalc.GetMapValueSecStress( "Yvrl"));	Arr.Add(cs);
		cs.Format(" Yvsu = Yvrl - tcl - Th + Tu      = %.3f mm" 	  ,StressCalc.GetMapValueSecStress( "Yvsu"));	Arr.Add(cs);
		cs.Format(" Yvsl = Yvsu - ( H + Tu + Tl )    = %.3f mm"  	  ,StressCalc.GetMapValueSecStress( "Yvsl"));	Arr.Add(cs);
		cs.Format(" Irv = Is + As × ds² + Ar × dr² = %.3f mm⁴"	  ,StressCalc.GetMapValueSecStress( "Irv"));	Arr.Add(cs);
		cs.Format(" fvru = Msc × Yvru / Irv = %.3f MPa"  		  ,StressCalc.GetMapValueSecStress( "Sigma_vcu"));	Arr.Add(cs);
		cs.Format(" fvrl = Msc × Yvrl / Irv = %.3f MPa"		  	  ,StressCalc.GetMapValueSecStress( "Sigma_vcl"));	Arr.Add(cs);
		cs.Format(" fvsu = Msc × Yvsu / Irv = %.3f MPa"			  ,StressCalc.GetMapValueSecStress( "Sigma_vsu"));	Arr.Add(cs);
		cs.Format(" fvsl = Msc × Yvsl / Irv = %.3f MPa"			  ,StressCalc.GetMapValueSecStress( "Sigma_vsl"));	Arr.Add(cs);
		cs.Format(" K    = K + B × (Tc / n)³/ 3   = %.3f mm⁴",ModelCalc.GetMapValueSecCo("Kv"));				Arr.Add(cs);
	}
	cs.Format(" τsv  = Ssc / A(web) + Msct ·Tw/K = %.3f MPa", StressCalc.GetMapValueSecStress( "Tau_sv"));	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	long nCountNO = 3;
	cs.Format("  (%d) 활하중", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 정모멘트
	{	
		cs.Format(" fvcu = Mv·Yvcu / (n·Iv)     = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcu_l"));		Arr.Add(cs);
		cs.Format(" fvcl = Mv·Yvcl / (n·Iv)     = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcl_l"));		Arr.Add(cs);
		cs.Format(" fvsu = Mv·Yvsu / Iv          = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsu_l"));		Arr.Add(cs);
		cs.Format(" fvsl = Mv·Yvsl / Iv          = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsl_l"));		Arr.Add(cs);
		cs.Format(" υ = Sv / A(Web) + Mvt ·Tw/K = %.3f MPa",StressCalc.GetMapValueSecStress("Tau_sv_l"));			Arr.Add(cs);
	}
	else   // 부모멘트
	{
		cs.Format(" fvru = Mv·Yvru / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcu_l"));		Arr.Add(cs);
		cs.Format(" fvrl = Mv·Yvrl / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcl_l"));		Arr.Add(cs);
		cs.Format(" fvsu = Mv·Yvsu / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsu_l"));		Arr.Add(cs);
		cs.Format(" fvsl = Mv·Yvsl / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsl_l"));		Arr.Add(cs);
		cs.Format(" υ = Sv / A(Web) + Mvt ·Tw/K = %.3f MPa",StressCalc.GetMapValueSecStress( "Tau_sv_l"));			Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bWind)
	{
		cs.Format("  (%d) 풍하중 - 활하중 비재하시의 풍하중에 의한 응력값으로 활하중 재하시에는 계산된 값의 1/2을 적용", nCountNO++);
		Arr.Add(cs);
		cs.Format(""); Arr.Add(cs);

		if(bPLUS_M)    // 정모멘트
		{	
			cs.Format(" fvcu = Mw·Yvcu / (n·Iv)     = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcu_w"));		Arr.Add(cs);
			cs.Format(" fvcl = Mw·Yvcl / (n·Iv)     = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcl_w"));		Arr.Add(cs);
			cs.Format(" fvsu = Mw·Yvsu / Iv          = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsu_w"));		Arr.Add(cs);
			cs.Format(" fvsl = Mw·Yvsl / Iv          = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsl_w"));		Arr.Add(cs);
			cs.Format(" υ = Sw / A(Web) + Mwt ·Tw/K = %.3f MPa", StressCalc.GetMapValueSecStress( "Tau_sv_w"));			Arr.Add(cs);
		}
		else   // 부모멘트
		{
			cs.Format(" fvru = Mw·Yvru / (n·Irv)    = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcu_w"));		Arr.Add(cs);
			cs.Format(" fvrl = Mw·Yvrl / (n·Irv)    = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcl_w"));		Arr.Add(cs);
			cs.Format(" fvsu = Mw·Yvru / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsu_w"));		Arr.Add(cs);
			cs.Format(" fvsl = Mw·Yvrl / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsl_w"));		Arr.Add(cs);
			cs.Format(" υ = Sw / A(Web) + Mwt ·Tw/K = %.3f MPa", StressCalc.GetMapValueSecStress( "Tau_sv_w"));		Arr.Add(cs);
		}
		cs.Format(""); Arr.Add(cs);
		cs.Format(""); Arr.Add(cs);
	}
	cs.Format("  (%d) Creep", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 정모멘트
	{	
		cs.Format(" Φ1 = %.f"								 , StressCalc.GetMapValueSecStress( "CreepCoef1"));			Arr.Add(cs);
		cs.Format(" n1  = n ×( 1 + Φ1/2 )            = %.0f ", StressCalc.GetMapValueSecStress( "n1"));				Arr.Add(cs);
		cs.Format(" Av1 = As + Ac / n1                 = %.3f mm²", StressCalc.GetMapValueSecStress( "Av1"));			Arr.Add(cs);
		cs.Format(" dc1 = dv ×As / ( Ac/n1 + As )     = %.3f mm", StressCalc.GetMapValueSecStress( "Dc1"));			Arr.Add(cs);
		cs.Format(" ds1 = dv - dc1                     = %.3f mm", StressCalc.GetMapValueSecStress( "Ds1"));			Arr.Add(cs);
		cs.Format(" Yv1cu = dc1 + Tc/2                 = %.3f mm", StressCalc.GetMapValueSecStress( "Yv1cu"));			Arr.Add(cs);
		cs.Format(" Yv1cl = Yv1cu - Tc                 = %.3f mm", StressCalc.GetMapValueSecStress( "Yv1cl"));			Arr.Add(cs);
		cs.Format(" Yv1su = Yv1cu - H2 + Tu            = %.3f mm", StressCalc.GetMapValueSecStress( "Yv1su"));			Arr.Add(cs);
		cs.Format(" Yv1sl = Yv1su - ( H + Tu + Tl )    = %.3f mm",StressCalc.GetMapValueSecStress( "Yv1sl"));			Arr.Add(cs);
		cs.Format(" Iv1 = Is + Ic/n1 + As × ds1² + Ac/n1 × dc1² = %.3f mm⁴", StressCalc.GetMapValueSecStress( "Iv1"));			Arr.Add(cs);
		cs.Format(" Nc = Msc × dvc × Ac / ( n ×Iv ) = %.3f N",StressCalc.GetMapValueSecStress( "Nc"));				Arr.Add(cs);
		cs.Format(" PΦ = Nc × 2 ×Φ1 / ( 2 + Φ1 )  = %.3f N",StressCalc.GetMapValueSecStress( "PCreep"));			Arr.Add(cs);
		cs.Format(" MΦ = PΦ × dc1                   = %.3f N.mm",StressCalc.GetMapValueSecStress( "MCreep"));		Arr.Add(cs);
		cs.Format(" fcu = Msc × Yvcu / ( n ×Iv )     = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cu"));		Arr.Add(cs);
		cs.Format(" fcl = Msc × Yvcl / ( n ×Iv )     = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cl"));		Arr.Add(cs);
		cs.Format(" fvcu1 = 1 / n1 ×(PΦ/ Av1 + k × MΦ× Yv1cu / Iv1 )  - Ec1·fcu·φ1/ Ec = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cu1"));			Arr.Add(cs);
		cs.Format(" fvcl1 = 1 / n1 ×(PΦ/ Av1 + k × MΦ× Yv1cl / Iv1 )  - Ec1·fcl·φ1/ Ec = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cl1"));			Arr.Add(cs);
		cs.Format(" fvsu1 = PΦ/ Av1 + k × MΦ× Yv1su / Iv1 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsu1"));			Arr.Add(cs);
		cs.Format(" fvsl1 = PΦ/ Av1 + k × MΦ× Yv1sl / Iv1 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsl1"));			Arr.Add(cs);
	}
	else   // 부모멘트
	{
		cs.Format(" Avc = As + Ac/n = %.3f mm²"							,ModelCalc.GetMapValueSecCo("Av_SUM"));			Arr.Add(cs);
		cs.Format(" dv = Ysu - Tu + Th + Tc/2 = %.3f mm"					,ModelCalc.GetMapValueSecCo("dv"));				Arr.Add(cs);
		cs.Format(" dvc = dv × As / ( Ac/n + As ) = %.3f mm"				,ModelCalc.GetMapValueSecCo("dvc"));			Arr.Add(cs);
		cs.Format(" dvs = dv - dvc = %.3f mm"								,ModelCalc.GetMapValueSecCo("dvs"));			Arr.Add(cs);
		cs.Format(" n1 = n ×( 1 + Φ1/2 ) = %.3f mm"						,StressCalc.GetMapValueSecStress( "n1"));		Arr.Add(cs);
		cs.Format(" Av1 = As + Ac / n1 = %.3f mm²"							,StressCalc.GetMapValueSecStress( "Av1"));		Arr.Add(cs);
		cs.Format(" dc1 = dv × As / ( Ac/n1 + As ) = %.3f mm"				,StressCalc.GetMapValueSecStress( "Dc1"));		Arr.Add(cs);
		cs.Format(" ds1 = dv - dc1 = %.3f mm"								,StressCalc.GetMapValueSecStress( "Ds1"));		Arr.Add(cs);
		cs.Format(" Icv = Is + Ic/n + As × dvs² + Ac/n ×dvc² = %.3f mm⁴",StressCalc.GetMapValueSecStress( "Icv"));		Arr.Add(cs);
		cs.Format(" Nc = Msc × dvc × Ac / ( n × Icv ) = %.3f N"			,StressCalc.GetMapValueSecStress( "Nc"));		Arr.Add(cs);
		cs.Format(" PΦ = Nc × 2 ×Φ1 / ( 2 + Φ1 ) = %.3f N"			,StressCalc.GetMapValueSecStress( "PCreep"));		Arr.Add(cs);
		cs.Format(" MΦ = PΦ × dc1 = %.3f N.mm"							,StressCalc.GetMapValueSecStress( "MCreep"));	Arr.Add(cs);
		cs.Format(" fvru1 = PΦ/ Av + k × MΦ × Yvru / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_cu1"));	Arr.Add(cs);
		cs.Format(" fvrl1 = PΦ/ Av + k × MΦ × Yvrl / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_cl1"));	Arr.Add(cs);
		cs.Format(" fvsu1 = PΦ/ Av + k × MΦ × Yvsu / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_vsu1"));	Arr.Add(cs);
		cs.Format(" fvsl1 = PΦ/ Av + k × MΦ × Yvsl / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_vsl1"));	Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (%d) 건조수축", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 정모멘트
	{	
		cs.Format(" n2  = n ×( 1 + Φ2 / 2 )        = %.0f ",StressCalc.GetMapValueSecStress( "n2"));				Arr.Add(cs);
		cs.Format("	Av2 = As + Ac / n2               = %.3f mm²",StressCalc.GetMapValueSecStress( "Av2"));		Arr.Add(cs);
		cs.Format(" dc2 = dv × As / ( Ac/n2 + As )  = %.3f mm"	,StressCalc.GetMapValueSecStress( "Dc2"));		Arr.Add(cs);
		cs.Format(" ds2 = dv - dc2                   = %.3f mm"	,StressCalc.GetMapValueSecStress( "Ds2"));		Arr.Add(cs);
		cs.Format(" Yv2cu = dc2 + Tc/2               = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2cu"));		Arr.Add(cs);
		cs.Format(" Yv2cl = Yv2cu - Tc               = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2cl"));		Arr.Add(cs);
		cs.Format(" Yv2su = Yv2cu - H2 - Tu          = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2su"));		Arr.Add(cs);
		cs.Format(" Yv2sl = Yv2su - ( H + Tu + Tl )  = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2sl"));		Arr.Add(cs);
		cs.Format(" Iv2 = Is + Ic / n2 + As × ds2² + Ac / n2 × dc2² = %.3f mm⁴",StressCalc.GetMapValueSecStress( "Iv2"));		Arr.Add(cs);
		cs.Format(" P2 = Es ×εs × Ac / n2        = %.3f N",StressCalc.GetMapValueSecStress( "PShrink"));							Arr.Add(cs);
		cs.Format(" Mv2 = P2 × dc2                 = %.3f N.mm",StressCalc.GetMapValueSecStress( "MShrink"));	Arr.Add(cs);
		cs.Format(" Ec2 = Es / n2                   = %.3f MPa",StressCalc.GetMapValueSecStress( "Ec2"));	Arr.Add(cs);
		cs.Format(" fvcu2 = 1  / n2 ×( P2 / Av2 + k × Mv2 × Yv2cu / Iv2) - Ec2×εs = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cu2"));	Arr.Add(cs);
		cs.Format(" fvcl2 = 1  / n2 ×( P2 / Av2 + k × Mv2 × Yv2cl / Iv2) - Ec2×εs = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cl2"));	Arr.Add(cs);
		cs.Format(" fvsu2 = P2 / Av2 + k × Mv2 × Yv2su / Iv2 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsu2"));	Arr.Add(cs);
		cs.Format(" fvsl2 = P2 / Av2 + k × Mv2 × Yv2sl / Iv2 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsl2"));	Arr.Add(cs);
	}
	else   // 부모멘트
	{
		cs.Format(" n2  = n ×( 1 + Φ2 / 2 )                = %.3f ",StressCalc.GetMapValueSecStress("n2"));				Arr.Add(cs);
		cs.Format("	Av2 = As + Ac / n2                       = %.3f mm²",StressCalc.GetMapValueSecStress("Av2"));			Arr.Add(cs);
		cs.Format(" dc2 = dv ×As / ( Ac/n2 + As )           = %.3f mm",StressCalc.GetMapValueSecStress("Dc2"));			Arr.Add(cs);
		cs.Format(" ds2 = dv - dc2                           = %.3f mm",StressCalc.GetMapValueSecStress("Ds2"));			Arr.Add(cs);
		cs.Format(" P2 = Es ×εs ×Ac / n2                  = %.3f N",StressCalc.GetMapValueSecStress("PShrink"));			Arr.Add(cs);
		cs.Format(" Mv2 = P2 × dc2                          = %.3f N.mm",StressCalc.GetMapValueSecStress("MShrink"));		Arr.Add(cs);
		cs.Format(" fvru2 = P2 / Av + k ×Mv2 × Yvru / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_cu2"));		Arr.Add(cs);
		cs.Format(" fvrl2 = P2 / Av + k ×Mv2 × Yvrl / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_cl2"));		Arr.Add(cs);
		cs.Format(" fvsu2 = P2 / Av + k ×Mv2 × Yvsu / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsu2"));	Arr.Add(cs);
		cs.Format(" fvsl2 = P2 / Av + k ×Mv2 × Yvsl / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsl2"));	Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	
	cs.Format("  (%d) 온 도 차", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	double P1=StressCalc.GetMapValueSecStress( "P1"); 

	if(bPLUS_M)    // 정모멘트
	{	
		cs.Format(" εt = α×ΔT = %.6f ",StressCalc.GetMapValueSecStress( "Et") );				Arr.Add(cs);
		cs.Format(" P1 = Es ×εt × Ac / n ) = %.3f N", P1);										Arr.Add(cs);
		cs.Format(" Mt = P1 × dvc = %.3f N.mm"		,StressCalc.GetMapValueSecStress( "Mt"));		Arr.Add(cs);
		cs.Format(" Ect = Ec ×εt = %.3f MPa"		,Ec * StressCalc.GetMapValueSecStress( "Ect"));	Arr.Add(cs);			
		cs.Format(" ±f cut = 1 / n ×(P1 / Av + k ×Mt × Yvcu / Iv )  - Ect = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cut"));		Arr.Add(cs);
		cs.Format(" ±f clt = 1 / n ×(P1 / Av + k ×Mt × Yvcl / Iv )  - Ect = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_clt"));		Arr.Add(cs);
		cs.Format(" ±f sut = P1 / Av + k ×Mt × Yvsu / Iv = %.3f MPa"				   ,StressCalc.GetMapValueSecStress( "Sigma_sut"));			Arr.Add(cs);
		cs.Format(" ±f slt = P1 / Av + k ×Mt × Yvsl / Iv = %.3f MPa"				   ,StressCalc.GetMapValueSecStress( "Sigma_slt"));			Arr.Add(cs);
	}
	else        //  부모멘트
	{
		cs.Format(" εt = α×ΔT = %.6f ",StressCalc.GetMapValueSecStress( "Et") );		Arr.Add(cs);
		cs.Format(" P1 = Es ×εt ×Ac / n = %.3f N", P1);									Arr.Add(cs);
		cs.Format(" Mt = P1 × dvc = %.3f N.mm"	,StressCalc.GetMapValueSecStress( "Mt"));	Arr.Add(cs);
		cs.Format(" Ert = Es ×εt = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Ert"));	Arr.Add(cs);			
		cs.Format(" ±f rut = P1 / Av + k × Mt × Yvru / Irv ) - Ert = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cut"));	Arr.Add(cs);
		cs.Format(" ±f rlt = P1 / Av + k × Mt × Yvrl / Irv ) - Ert = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_clt"));	Arr.Add(cs);
		cs.Format(" ±f sut = P1 / Av + k × Mt × Yvsu / Irv = %.3f MPa"		  ,StressCalc.GetMapValueSecStress( "Sigma_sut"));	Arr.Add(cs);
		cs.Format(" ±f slt = P1 / Av + k × Mt × Yvsl / Irv = %.3f MPa" 		  ,StressCalc.GetMapValueSecStress( "Sigma_slt"));	Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

// 전단 및 비틀림 순부 비틀림 고려
	pSteelCtl = m_pStd->m_pDataManage->GetSteel();

	CString strMatLeft, strMatRight;
	CalcGen.GetMaterial(pBx, G_W, strMatLeft, strMatRight);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strMatLeft + strMatRight)];
	CHGAllowStress	AllowShearTension(pSteel);
	double	dTw			= pGir->GetThickFactByStation(dSta, G_W);
	
	double	dTauAllow	= AllowShearTension.GetAllowStress(ALLOW_SHEARSTRESS, dTw);
	double	dNu_Web		= StressCalc.GetMapValueSecStress("Nu_Web");
	double	dNu_FL		= StressCalc.GetMapValueSecStress("Nu_FL");
	CalcGen.GetMaterial(pBx, G_W, sLeft, sRight);
	cs.Format("  (%d) 전단 및 비틀림    복부 사용강재 : %s%s", nCountNO++, sLeft, sRight);		Arr.Add(cs);
	cs.Format("");										Arr.Add(cs);
	if(bWind)
	{
		cs.Format("- 풍하중 비재하시의 전단응력"); Arr.Add(cs);
	}
	cs.Format(" Mt  = Mst + Msct + Mvt                                                 = %.3f N.mm", StressCalc.GetMapValueSecStress("Mt"));		Arr.Add(cs);
	cs.Format(" Wn  = H × B / 4                                                       = %.3f mm²", StressCalc.GetMapValueSecStress("Wn"));		Arr.Add(cs);
	cs.Format(" lw  = (H²× (Tu·Bu³·Tb·Bl³))/(12 × (Tu·Bu³+Tb·Bl³))         = %.3f mm^6", StressCalc.GetMapValueSecStress("lw"));		Arr.Add(cs);
	cs.Format("  a  = L·√((G·K)/(Es·Iw))                                           = %.3f", ModelCalc.GetMapValueSecCo("WarpingAlpha"));		Arr.Add(cs);
	cs.Format(" Mwa = (Mt / L) / (a / L)²× (2·sinh(a/2) / sinh(a)-1)                = %.3f N.mm²", StressCalc.GetMapValueSecStress("Mwa"));		Arr.Add(cs);
	cs.Format(" Twa = -(Mt / L) / (a/L) × (1-cosh(a)) / sinh(a)))                     = %.3f N.mm",  StressCalc.GetMapValueSecStress("Twa"));		Arr.Add(cs);
	cs.Format(" Swa = H × Tu × Bl²/ 16)                                             = %.3f mm⁴", StressCalc.GetMapValueSecStress("Swa"));		Arr.Add(cs);
	cs.Format("  υ = 전단력에 의한 복부 전단응력       : (Ss + Ssc + Sv) / As(Web)    = %.3f MPa", StressCalc.GetMapValueSecStress("Nu"));			Arr.Add(cs);
	cs.Format(" υt = 순수비틀림에 의한 복부 전단응력   : Mt·Tw / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_Web"));	Arr.Add(cs);
	cs.Format(" υt = 순수비틀림에 의한 플랜지 전단응력 : Mt·Tu / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_FL"));		Arr.Add(cs);
	cs.Format(" υw = 뒴비틀림에 의한 플랜지 전단응력   : Twa·Sw / (Iw·Tu)           = %.3f MPa", StressCalc.GetMapValueSecStress("Nuw"));		Arr.Add(cs);
	if(dNu_Web < dTauAllow)
		cs.Format("     υweb    = υ + υt   = %4.1f   MPa  <  υa = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_Web"), dTauAllow);
	else
		cs.Format("     υweb    = υ + υt   = %4.1f   MPa  >  υa = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_Web"), dTauAllow);
	Arr.Add(cs);
	if(dNu_FL < dTauAllow)
		cs.Format("     υflange = υ + υt   = %4.1f   MPa  <  υa = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_FL"), dTauAllow);
	else
		cs.Format("     υflange = υ + υt   = %4.1f   MPa  >  υa = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_FL"), dTauAllow);
	Arr.Add(cs);
	cs.Format("  fw = 뒴비틀림에 의한 수직응력          : Mwa·Wn / Iw                 = %.3f MPa", StressCalc.GetMapValueSecStress("Fw"));		Arr.Add(cs);

	if(bWind)
	{
		cs.Format(""); Arr.Add(cs);
		double	dNu_w			= StressCalc.GetMapValueSecStress("Nu_w");
		double	dNut_Web_w		= StressCalc.GetMapValueSecStress("Nut_Web_w");
		double	dNuw_w			= StressCalc.GetMapValueSecStress("Nuw_w");
		double	dNu_wl			= StressCalc.GetMapValueSecStress("Nu_wl");
		double	dNut_Web_wl		= StressCalc.GetMapValueSecStress("Nut_Web_wl");
		double	dNuw_wl			= StressCalc.GetMapValueSecStress("Nuw_wl");
		double	dTauAllow_w		= dTauAllow * 1.2;
		double	dTauAllow_wl	= dTauAllow * 1.25;

		cs.Format("- 풍하중 재하시의 전단응력");Arr.Add(cs);

		cs.Format("  · 활하중 비재하시");		Arr.Add(cs);
		cs.Format(" Mt  = Mst + Msct + Mwt                                                 = %.3f N.mm", StressCalc.GetMapValueSecStress("Mt_w"));		Arr.Add(cs);
		cs.Format(" Wn  = H × B / 4                                                       = %.3f mm²", StressCalc.GetMapValueSecStress("Wn_w"));		Arr.Add(cs);
		cs.Format(" lw  = (H²× (Tu·Bu³·Tb·Bl³))/(12 × (Tu·Bu³+Tb·Bl³))         = %.3f mm^6", StressCalc.GetMapValueSecStress("lw_w"));		Arr.Add(cs);
		cs.Format("  a  = L·√((G·K)/(Es·Iw))                                           = %.3f", ModelCalc.GetMapValueSecCo("WarpingAlpha"));		Arr.Add(cs);
		cs.Format(" Mwa	= (Mt / L) / (a / L)²× (2·sinh(a/2) / sinh(a)-1)             = %.3f N.mm²", StressCalc.GetMapValueSecStress("Mwa_w"));	Arr.Add(cs);
		cs.Format(" Twa = -(Mt / L) / ( a / L) × (1-cosh(a)) / sinh(a)))                  = %.3f N.mm",  StressCalc.GetMapValueSecStress("Twa_w"));	Arr.Add(cs);
		cs.Format(" Swa = H × Tu × Bl²/ 16)                                             = %.3f mm⁴", StressCalc.GetMapValueSecStress("Swa_w"));		Arr.Add(cs);
		//////////////////////////////////////////////////////////////////////////
		cs.Format("  υ = 전단력에 의한 복부 전단응력       : (Ss + Ssc + Sv) / As(Web)    = %.3f MPa", StressCalc.GetMapValueSecStress("Nu_w"));		Arr.Add(cs);
		cs.Format(" υt = 순수비틀림에 의한 복부 전단응력   : Mt·Tw / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_Web_w"));	Arr.Add(cs);
		cs.Format(" υt = 순수비틀림에 의한 플랜지 전단응력 : Mt·Tu / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_FL_w"));	Arr.Add(cs);
		cs.Format(" υw = 뒴비틀림에 의한 플랜지 전단응력   : Twa·Sw / (Iw·Tu)           = %.3f MPa", StressCalc.GetMapValueSecStress("Nuw_w"));	Arr.Add(cs);
//		if(dNu_Web_w < dTauAllow_w)
//			cs.Format("     υweb    = υ + υt   = %4.1f   MPa  <  υa = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_Web_w"), dTauAllow_w);
//		else
//			cs.Format("     υweb    = υ + υt   = %4.1f   MPa  >  υa = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_Web_w"), dTauAllow_w);
//		Arr.Add(cs);
//		if(dNu_FL_w < dTauAllow_w)
//			cs.Format("     υflange = υ + υt   = %4.1f   MPa  <  υa = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_FL_w"), dTauAllow_w);
//		else
//			cs.Format("     υflange = υ + υt   = %4.1f   MPa  >  υa = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_FL_w"), dTauAllow_w);
//		Arr.Add(cs);
		cs.Format("  fw = 뒴비틀림에 의한 수직응력          : Mwa·Wn / Iw                 = %.3f MPa", StressCalc.GetMapValueSecStress("Fw_w"));		Arr.Add(cs);
		if(dNu_w<dTauAllow_w && dNut_Web_w<dTauAllow_w && dNuw_w<dTauAllow_w)
			cs.Format("                            υ,υt,υw  < υa = %.3f MPa  ------ O.K", dTauAllow_w);
		else
			cs.Format("                            υ,υt,υw  > υa = %.3f MPa  ------ N.G", dTauAllow_w);
		Arr.Add(cs);
		//////////////////////////////////////////////////////////////////////////

		cs.Format("  · 활하중 재하시");	Arr.Add(cs);
		cs.Format(" Mt  = Mst + Msct + Mwt/2 + Mvt                                         = %.3f N.mm", StressCalc.GetMapValueSecStress("Mt_wl"));	Arr.Add(cs);
		cs.Format(" Wn  = H × B / 4                                                       = %.3f mm²", StressCalc.GetMapValueSecStress("Wn_wl"));	Arr.Add(cs);
		cs.Format(" lw  = (H²× (Tu·Bu³·Tb·Bl³))/(12 × (Tu·Bu³+Tb·Bl³))         = %.3f mm^6", StressCalc.GetMapValueSecStress("lw_wl"));	Arr.Add(cs);
		cs.Format("  a  = L·√((G·K)/(Es·Iw))                                           = %.3f", ModelCalc.GetMapValueSecCo("WarpingAlpha"));		Arr.Add(cs);
		cs.Format(" Mwa	= (Mt / L) / (a / L)²× (2·sinh(a/2) / sinh(a)-1)             = %.3f N.mm²", StressCalc.GetMapValueSecStress("Mwa_wl"));Arr.Add(cs);
		cs.Format(" Twa = -(Mt / L) / (a/L) × (1-cosh(a)) / sinh(a)))                     = %.3f N.mm",  StressCalc.GetMapValueSecStress("Twa_wl"));	Arr.Add(cs);
		cs.Format(" Swa = H × Tu × Bl²/ 16)                                             = %.3f mm⁴", StressCalc.GetMapValueSecStress("Swa_wl"));	Arr.Add(cs);
		//////////////////////////////////////////////////////////////////////////
		cs.Format("  υ = 전단력에 의한 복부 전단응력       : (Ss + Ssc + Sv) / As(Web)    = %.3f MPa", StressCalc.GetMapValueSecStress("Nu_wl"));		Arr.Add(cs);
		cs.Format(" υt = 순수비틀림에 의한 복부 전단응력   : Mt·Tw / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_Web_wl"));	Arr.Add(cs);
		cs.Format(" υt = 순수비틀림에 의한 플랜지 전단응력 : Mt·Tu / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_FL_wl"));	Arr.Add(cs);
		cs.Format(" υw = 뒴비틀림에 의한 플랜지 전단응력   : Twa·Sw / (Iw·Tu)           = %.3f MPa", StressCalc.GetMapValueSecStress("Nuw_wl"));	Arr.Add(cs);
//		if(dNu_Web_wl < dTauAllow_wl)
//			cs.Format("     υweb    = υ + υt   = %4.1f   MPa  <  υa = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_Web_wl"), dTauAllow_wl);
//		else
//			cs.Format("     υweb    = υ + υt   = %4.1f   MPa  >  υa = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_Web_wl"), dTauAllow_wl);
//		Arr.Add(cs);
//		if(dNu_FL_wl < dTauAllow_wl)
//			cs.Format("     υflange = υ + υt   = %4.1f   MPa  <  υa = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_FL_wl"), dTauAllow_wl);
//		else
//			cs.Format("     υflange = υ + υt   = %4.1f   MPa  >  υa = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_FL_wl"), dTauAllow_wl);
//		Arr.Add(cs);
		cs.Format("  fw = 뒴비틀림에 의한 수직응력          : Mwa·Wn / Iw                 = %.3f MPa", StressCalc.GetMapValueSecStress("Fw_wl"));		Arr.Add(cs);		
		if(dNu_wl<dTauAllow_wl && dNut_Web_wl<dTauAllow_wl && dNuw_wl<dTauAllow_wl)
			cs.Format("                            υ,υt,υw  < υa = %.3f MPa  ------ O.K", dTauAllow_wl);
		else
			cs.Format("                            υ,υt,υw  > υa = %.3f MPa  ------ N.G", dTauAllow_wl);
		Arr.Add(cs);
	}

	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	
	cs.Format("  (%d) 조합응력의 검토", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	//////////////////////////////////////////////////
	double	dSigmaCAAfter	= 0;
	double	dSigmaCABefore	= 0;
	double	dSigmaTA		= 0;
	double	dTmin			= 0;
	BOOL	b2K				= TRUE;

	if(bPLUS_M)	dTmin = up_f_t;
	else		dTmin = lo_f_t;
	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	long	nMaterial	= bPLUS_M ? G_F_U : G_F_L;
	double	dWidth		= (pGir->GetWidthOnStation(dSta, bPLUS_M) - pGir->GetThickFactByStation(dSta, G_W))/2;

	CalcGen.GetMaterial(pBx, nMaterial, strLeft, strRight);

	pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];

	CHGAllowStress	AllowTension(pSteel);
	CHGAllowStress	AllowBuckling(pSteel);
	AllowBuckling.SetAllowBuckling(dTmin, dWidth);
	
	dSigmaTA		= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dTmin);
	dSigmaCAAfter	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dTmin);
	dSigmaCABefore	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dTmin);

	CString	sLeftUp, sRightUp, sLeftLo, sRightLo;
	CalcGen.GetMaterial(pBx, G_F_U, sLeftUp, sRightUp);
	CalcGen.GetMaterial(pBx, G_F_L, sLeftLo, sRightLo);
	double	dTu			= pGir->GetThickFactByStation(dSta, G_F_U);
	double	dTl			= pGir->GetThickFactByStation(dSta, G_F_L);
	long	nThickIdx	= 0;

	if(bPLUS_M)	//정모멘트
	{
		CalcGen.GetMaterial(pBx, G_F_L, sLeftLo, sRightLo);
		pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeftLo + sRightLo)];
		CHGAllowStress	AllowTension(pSteel);
		CHGAllowStress	AllowBuckling(pSteel);

		dSigmaTA	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dTl);
		
		CalcGen.GetMaterial(pBx, G_F_U, sLeftUp, sRightUp);
		pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeftUp + sRightUp)];
		
		AllowTension.SetBaseSteel(pSteel);
		AllowBuckling.SetBaseSteel(pSteel);
		AllowBuckling.SetAllowBuckling(dTu, dWidth);

		dSigmaCAAfter	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dTu);
		dSigmaCABefore	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dTu);
		nThickIdx		= AllowBuckling.GetIdxByThick(dTu);
	}
	else	//부모멘트
	{
		CalcGen.GetMaterial(pBx, G_F_U, sLeftUp, sRightUp);
		pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeftUp + sRightUp)];
		
		CHGAllowStress	AllowTension(pSteel);
		CHGAllowStress	AllowBuckling(pSteel);
		
		dSigmaTA	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dTu);
		CalcGen.GetMaterial(pBx, G_F_L, sLeftLo, sRightLo);

		AllowTension.SetBaseSteel(pSteel);
		AllowBuckling.SetBaseSteel(pSteel);
		AllowBuckling.SetAllowBuckling(dTl, dWidth);		
		dSigmaCAAfter	= AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dTl);
		dSigmaCABefore	= min(AllowBuckling.GetAllowStress(ALLOW_TENSIONSTRESS, dTl), AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY, dTl));
		nThickIdx		= AllowBuckling.GetIdxByThick(dTl);
	}

	cs.Format(""); Arr.Add(cs);
	if(bPLUS_M)	cs.Format(" # 인장 (하부)플랜지의 허용응력 (%s%s 사용)",sLeftLo, sRightLo);
	else		cs.Format(" # 인장 (상부)플랜지의 허용응력 (%s%s 사용)",sLeftUp, sRightUp);
	Arr.Add(cs);
	cs.Format("   fta = %.3f MPa", dSigmaTA);	Arr.Add(cs);
	cs.Format("");								Arr.Add(cs);
	if(bPLUS_M)	cs.Format(" # 압축 (상부)플랜지의 허용응력 (%s%s 사용)",sLeftUp, sRightUp);
	else		cs.Format(" # 압축 (하부)플랜지의 허용응력 (%s%s 사용)",sLeftLo, sRightLo);
	Arr.Add(cs);
	AllowBuckling.SetTypeAllow(ALLOW_BUCKLING_FREEJETTY);
	HGALLOWSTRESS vtAllowStress = AllowBuckling.m_AllowStress[nThickIdx];
	
	cs.Format("     b / %.1f ≤ tu                   이면, fca = 140 MPa", vtAllowStress.m_dCoeff2, dSigmaCAAfter);
	Arr.Add(cs);
	cs.Format("     b / %.1f ≤ tu < b / %.1f        이면, fca = %.0f(t/b)²MPa",
		vtAllowStress.m_dCoeff2, vtAllowStress.m_dCoeff4, vtAllowStress.m_dCoeff3);
	Arr.Add(cs);

	//////////////////////////////////////////
	if(bPLUS_M)		cs.Format("    사용 Tu = %.3f mm", Tu);
	else			cs.Format("    사용 Tl = %.3f mm", Tl);
	Arr.Add(cs);

	if(bPLUS_M)
	{
		cs.Format("    콘크리트 슬래브에 의해 판의 국부좌굴이 충분히 방지된 경우이므로");
		Arr.Add(cs);
	}
	cs.Format("    따라서,  fca = %.3f MPa", dSigmaCABefore);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	double SigmaCCA=0;
	double SigmaCTA=0;

	CString RebarType="SD40";            // 철근의 종류

	if(bPLUS_M)    // 정모멘트
	{
		cs.Format(" # 철근 콘크리트의 허용응력");
		Arr.Add(cs);
		if(b2K)
		{
			SigmaCCA = 0.4*pData->DESIGN_MATERIAL.m_dSigmaCK;
			cs.Format("   fca = 0.4  × f ck = %.3f MPa", SigmaCCA);
			Arr.Add(cs);
		}
		else
		{
			SigmaCCA = 0.3*pData->DESIGN_MATERIAL.m_dSigmaCK;
			cs.Format("   fca = 0.3  × f ck = %.3f MPa", SigmaCCA);
			Arr.Add(cs);
		}

		SigmaCTA = min(0.07*pData->DESIGN_MATERIAL.m_dSigmaCK, 2.5);

		cs.Format("   fta = 0.07 × f ck = %.3f MPa", SigmaCTA);
		Arr.Add(cs);
	}
	else           // 부모멘트
	{
		cs.Format(" # RE-BAR의 허용응력");
		Arr.Add(cs);

		if(RebarType=="SD30")      SigmaCCA=150;
		else if(RebarType=="SD35") SigmaCCA=175;
		else                       SigmaCCA=180;  
		
		SigmaCTA=160;
		cs.Format("   fta = %.3f MPa", SigmaCTA);			Arr.Add(cs);
		cs.Format("   fca = %.3f MPa", SigmaCCA);			Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	double Sigma_up,Sigma_lo,Sigma_upA,Sigma_loA; // 상, 하, 허용 상, 허용 하
	double Max_up,Max_lo,Max_upA,Max_loA;

	//////////////////////////////////////////////////////////////////////////	
	SigmaCCA *= dHyR;
	SigmaCTA *= dHyR;
	dSigmaCAAfter	*= dHyR;
	dSigmaCABefore	*= dHyR;
	dSigmaTA *= dHyR;
	//////////////////////////////////////////////////////////////////////////	

	if(bWind)
	{
		if(bPLUS_M)    // 정모멘트
		{
			cs.Format(" ┏━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");	Arr.Add(cs);
			cs.Format(" ┃                  ┃             CONCRETE                 ┃                        STEEL                         ┃");	Arr.Add(cs);
			cs.Format(" ┃                  ┣━━━━━━━━━━━━━┳━━━━━╋━━━━━━━━━━━━━┳━━━━━━━━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃    하중  조합    ┃     작용 응력 ( MPa )    ┃ 허용응력 ┃     작용 응력 ( MPa )    ┃     허용 응력 ( MPa )    ┃");	Arr.Add(cs);	
			cs.Format(" ┃                  ┣━━━━━━┳━━━━━━┫          ┣━━━━━━┳━━━━━━╋━━━━━━┳━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃                  ┃  상    연  ┃  하    연  ┃ ( MPa )  ┃  상    연  ┃  하    연  ┃  상    연  ┃ 하     연  ┃");	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaCABefore;
			Sigma_loA=dSigmaTA;

			Max_up=Sigma_up;
			Max_lo=Sigma_lo;
			Max_upA=Sigma_upA;
			Max_loA=Sigma_loA;

			cs.Format(" ┃1                 ┃            ┃            ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_vsu")+StressCalc.GetMapValueSecStress("Sigma_vsu_l")+StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_vsl")+StressCalc.GetMapValueSecStress("Sigma_vsl_l")+StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaCAAfter;
			Sigma_loA=dSigmaTA;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			double temp1			= StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l");
			double temp2			= StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l");
			double dAlpha			= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			double Sigma_up_post	= StressCalc.GetMapValueSecStress("Sigma_vsu");
			double Sigma_up_live	= StressCalc.GetMapValueSecStress("Sigma_vsu_l");
			double Sigma_lo_post	= StressCalc.GetMapValueSecStress("Sigma_vsl");
			double Sigma_lo_live	= StressCalc.GetMapValueSecStress("Sigma_vsl_l");

			if(dAlpha < 10 && Sigma_up_post+Sigma_up_live > 0)	temp1 += StressCalc.GetMapValueSecStress("Fw");
			if(dAlpha < 10 && Sigma_up_post+Sigma_up_live < 0)	temp1 += -StressCalc.GetMapValueSecStress("Fw");
			if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live > 0)	temp2 += StressCalc.GetMapValueSecStress("Fw");
			if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live < 0)	temp2 += -StressCalc.GetMapValueSecStress("Fw");

			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3             ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3             ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su")+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l")+StressCalc.GetMapValueSecStress("Sigma_vsu1");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl")+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l")+StressCalc.GetMapValueSecStress("Sigma_vsl1");
			Sigma_upA=dSigmaCAAfter*1.15;
			Sigma_loA=dSigmaTA;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}
			
			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") + StressCalc.GetMapValueSecStress("Sigma_cu1");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") + StressCalc.GetMapValueSecStress("Sigma_cl1");
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+5           ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cu1")
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+5           ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2") ;
			Sigma_upA=dSigmaCAAfter*1.15;
			Sigma_loA=dSigmaTA;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2"); 
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+5+6         ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+5+6         ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						+StressCalc.GetMapValueSecStress("Sigma_sut") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						+StressCalc.GetMapValueSecStress("Sigma_slt") ;
			Sigma_upA=dSigmaCAAfter*1.3;
			Sigma_loA=dSigmaTA*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					+StressCalc.GetMapValueSecStress("Sigma_cut");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					+StressCalc.GetMapValueSecStress("Sigma_clt"); 

			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+5+6+7       ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCCA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+5+6+7       ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						-StressCalc.GetMapValueSecStress("Sigma_sut") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						-StressCalc.GetMapValueSecStress("Sigma_slt") ;
			Sigma_upA=dSigmaCAAfter*1.3;
			Sigma_loA=dSigmaTA*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					-StressCalc.GetMapValueSecStress("Sigma_cut");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					-StressCalc.GetMapValueSecStress("Sigma_clt");
			
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+5+6-7       ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							-StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							-StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCCA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+5+6-7       ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							-StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							-StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);


			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_vsu")
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") 
						+StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_vsl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") 
						+StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaCAAfter*1.2;
			Sigma_loA=dSigmaTA*1.2;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_w");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_w");
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+4             ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_w") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_w") 
						   ,SigmaCCA*1.2 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+4             ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_w") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_w") 
						   ,SigmaCTA*1.2 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l")
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaCAAfter*1.25;
			Sigma_loA=dSigmaTA*1.25;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}
			
			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0;
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0;
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+1/2(4)      ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCCA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+1/2(4)      ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0  
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaCAAfter*1.25;
			Sigma_loA=dSigmaTA*1.25;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0;
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0; 
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+1/2(4)+5    ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCCA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+1/2(4)+5    ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaCAAfter*1.25;
			Sigma_loA=dSigmaTA*1.25;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0;
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					+StressCalc.GetMapValueSecStress("SSigma_vcl_w") / 2.0; 

			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+1/2(4)+5+6  ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCCA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+1/2(4)+5+6  ┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0  
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2")
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0
						+StressCalc.GetMapValueSecStress("Sigma_sut") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0
						+StressCalc.GetMapValueSecStress("Sigma_slt") ;
			Sigma_upA=dSigmaCAAfter*1.35;
			Sigma_loA=dSigmaTA*1.35;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2")
					+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
					+StressCalc.GetMapValueSecStress("Sigma_cut");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0
					+StressCalc.GetMapValueSecStress("Sigma_clt");
			
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+1/2(4)+5+6+7┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_cut")
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_clt") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0
						   ,SigmaCCA*1.35 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+1/2(4)+5+6+7┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_cut") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_clt") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0
						   ,SigmaCTA*1.35 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_l")
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						-StressCalc.GetMapValueSecStress("Sigma_sut") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0
						+StressCalc.GetMapValueSecStress("Sigma_vsl_l")    
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						-StressCalc.GetMapValueSecStress("Sigma_slt") ;
			Sigma_upA=dSigmaCAAfter*1.35;
			Sigma_loA=dSigmaTA*1.35;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2")
					+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
					-StressCalc.GetMapValueSecStress("Sigma_cut");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
					-StressCalc.GetMapValueSecStress("Sigma_clt");
			
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+1/2(4)+5+6-7┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2")
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
							-StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2")
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0
							-StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCCA*1.35 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+1/2(4)+5+6-7┃%9.3f   ┃%9.3f   ┃%9.3f ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2")
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
							-StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2")
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0
							-StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCTA*1.35 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┗━━━━━━━━━┻━━━━━━┻━━━━━━┻━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┛");	Arr.Add(cs);


		}
		else     // 부모멘트
		{
			cs.Format(" ┏━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");	Arr.Add(cs);
			cs.Format(" ┃                  ┃              RE-BAR                  ┃                        STEEL                         ┃");	Arr.Add(cs);
			cs.Format(" ┃                  ┣━━━━━━━━━━━━━┳━━━━━╋━━━━━━━━━━━━━┳━━━━━━━━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃    하중  조합    ┃     작용 응력 ( MPa )    ┃ 허용응력 ┃     작용 응력 ( MPa )    ┃     허용 응력 ( MPa )    ┃");	Arr.Add(cs);	
			cs.Format(" ┃                  ┣━━━━━━┳━━━━━━┫          ┣━━━━━━┳━━━━━━╋━━━━━━┳━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃                  ┃  상    연  ┃  하    연  ┃   (MPa)  ┃  상    연  ┃  하    연  ┃  상    연  ┃ 하     연  ┃");	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaTA;
			Sigma_loA=dSigmaCABefore;

			Max_up=Sigma_up;
			Max_lo=Sigma_lo;
			Max_upA=Sigma_upA;
			Max_loA=Sigma_loA;

			cs.Format(" ┃1                 ┃            ┃            ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaTA;
			Sigma_loA=dSigmaCAAfter;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3             ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l")
						+StressCalc.GetMapValueSecStress("Sigma_vsl1");
			Sigma_upA=dSigmaTA;
			Sigma_loA=dSigmaCAAfter;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+5           ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f    | %9.3f  ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2") ;
			Sigma_upA=dSigmaTA;
			Sigma_loA=dSigmaCAAfter;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}
			cs.Format(" ┃1+2+3+5+6         ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						+StressCalc.GetMapValueSecStress("Sigma_sut") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						+StressCalc.GetMapValueSecStress("Sigma_slt") ;
			Sigma_upA=dSigmaTA*1.15;
			Sigma_loA=dSigmaCAAfter*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+5+6+7       ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						+StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						+StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						-StressCalc.GetMapValueSecStress("Sigma_sut") ;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						-StressCalc.GetMapValueSecStress("Sigma_slt") ;
			Sigma_upA=dSigmaTA*1.15;
			Sigma_loA=dSigmaCAAfter*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+5+6-7       ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						-StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						-StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_w") 
						+StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_w") 
						+StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaTA*1.25;
			Sigma_loA=dSigmaCAAfter*1.25;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+4             ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_w") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_w") 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaTA*1.25;
			Sigma_loA=dSigmaCAAfter*1.25;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+1/2(4)      ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaTA*1.25;
			Sigma_loA=dSigmaCAAfter*1.25;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+1/2(4)+5    ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaTA*1.25;
			Sigma_loA=dSigmaCAAfter*1.25;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+1/2(4)+5+6  ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						+StressCalc.GetMapValueSecStress("Sigma_sut")
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						+StressCalc.GetMapValueSecStress("Sigma_slt")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaTA*1.35;
			Sigma_loA=dSigmaCAAfter*1.35;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+1/2(4)+5+6+7┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						+StressCalc.GetMapValueSecStress("Sigma_cut")
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						+StressCalc.GetMapValueSecStress("Sigma_clt") 
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0
					   ,SigmaCTA*1.35 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu") + StressCalc.GetMapValueSecStress("Sigma_vsu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
						-StressCalc.GetMapValueSecStress("Sigma_sut")
						+StressCalc.GetMapValueSecStress("Sigma_vsu_w") / 2.0;
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl") + StressCalc.GetMapValueSecStress("Sigma_vsl_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vsl1")
						+StressCalc.GetMapValueSecStress("Sigma_vsl2")
						-StressCalc.GetMapValueSecStress("Sigma_slt")
						+StressCalc.GetMapValueSecStress("Sigma_vsl_w") / 2.0;
			Sigma_upA=dSigmaTA*1.35;
			Sigma_loA=dSigmaCAAfter*1.35;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+1/2(4)+5+6-7┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						-StressCalc.GetMapValueSecStress("Sigma_cut") 
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						-StressCalc.GetMapValueSecStress("Sigma_clt")
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0
					   ,SigmaCTA*1.35 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┗━━━━━━━━━┻━━━━━━┻━━━━━━┻━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┛");	Arr.Add(cs);

		}

	}
	else
	{
		if(bPLUS_M)    // 정모멘트
		{
			cs.Format(" ┏━━━━━━┳━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");	Arr.Add(cs);
			cs.Format(" ┃            ┃             CONCRETE                 ┃                        STEEL                         ┃");	Arr.Add(cs);
			cs.Format(" ┃            ┣━━━━━━━━━━━━━┳━━━━━╋━━━━━━━━━━━━━┳━━━━━━━━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃ 하중  조합 ┃     작용 응력 ( MPa )    ┃ 허용응력 ┃     작용 응력 ( MPa )    ┃     허용 응력 ( MPa )    ┃");	Arr.Add(cs);	
			cs.Format(" ┃            ┣━━━━━━┳━━━━━━┫          ┣━━━━━━┳━━━━━━╋━━━━━━┳━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃            ┃  상    연  ┃  하    연  ┃   (MPa)  ┃  상    연  ┃  하    연  ┃  상    연  ┃ 하     연  ┃");	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	= StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo	= StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA	= dSigmaCABefore;
			Sigma_loA	= dSigmaTA;
			Max_up		= Sigma_up;
			Max_lo		= Sigma_lo;
			Max_upA		= Sigma_upA;
			Max_loA		= Sigma_loA;

			cs.Format(" ┃1           ┃            ┃            ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	= StressCalc.GetMapValueSecStress("Sigma_vsu")+StressCalc.GetMapValueSecStress("Sigma_vsu_l")+StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo	= StressCalc.GetMapValueSecStress("Sigma_vsl")+StressCalc.GetMapValueSecStress("Sigma_vsl_l")+StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA	= dSigmaCAAfter;
			Sigma_loA	= dSigmaTA;

			double temp1			= StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l");
			double temp2			= StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l");
			double dAlpha			= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			double Sigma_up_post	= StressCalc.GetMapValueSecStress("Sigma_vsu");
			double Sigma_up_live	= StressCalc.GetMapValueSecStress("Sigma_vsu_l");
			double Sigma_lo_post	= StressCalc.GetMapValueSecStress("Sigma_vsl");
			double Sigma_lo_live	= StressCalc.GetMapValueSecStress("Sigma_vsl_l");

			if(dAlpha < 10 && Sigma_up_post+Sigma_up_live > 0)	Sigma_up += StressCalc.GetMapValueSecStress("Fw");
			if(dAlpha < 10 && Sigma_up_post+Sigma_up_live < 0)	Sigma_up += -StressCalc.GetMapValueSecStress("Fw");
			if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live > 0)	Sigma_lo += StressCalc.GetMapValueSecStress("Fw");
			if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live < 0)	Sigma_lo += -StressCalc.GetMapValueSecStress("Fw");

			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3       ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,temp1,temp2,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3       ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,temp1,temp2,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= StressCalc.GetMapValueSecStress("Sigma_vsu1");
			Sigma_lo	+= StressCalc.GetMapValueSecStress("Sigma_vsl1");
			Sigma_upA	= dSigmaCAAfter*1.15;
			Sigma_loA	= dSigmaTA;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}
			
			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cu1");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1");
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+4     ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+4     ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= StressCalc.GetMapValueSecStress("Sigma_vsu2");
			Sigma_lo	+= StressCalc.GetMapValueSecStress("Sigma_vsl2");
			Sigma_upA=dSigmaCAAfter*1.15;
			Sigma_loA=dSigmaTA;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2"); 
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+4+5   ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+4+5   ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= StressCalc.GetMapValueSecStress("Sigma_sut");
			Sigma_lo	+= StressCalc.GetMapValueSecStress("Sigma_slt");
			Sigma_upA	= dSigmaCAAfter*1.3;
			Sigma_loA	= dSigmaTA*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					+StressCalc.GetMapValueSecStress("Sigma_cut");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					+StressCalc.GetMapValueSecStress("Sigma_clt"); 

			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+4+5+6 ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCCA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+4+5+6 ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							+StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	-= StressCalc.GetMapValueSecStress("Sigma_sut")*2;
			Sigma_lo	-= StressCalc.GetMapValueSecStress("Sigma_slt")*2;
			Sigma_upA	= dSigmaCAAfter*1.3;
			Sigma_loA	= dSigmaTA*1.15;
			Sigma_upA	= dSigmaCAAfter*1.3;
			Sigma_loA	= dSigmaTA*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			temp1 = StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
					+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					-StressCalc.GetMapValueSecStress("Sigma_cut");
			temp2 = StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					-StressCalc.GetMapValueSecStress("Sigma_clt");
			
			if(temp1 > 0 && temp2 > 0)
			{
				cs.Format(" ┃1+2+3+4+5-6 ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							-StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							-StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCCA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" ┃1+2+3+4+5-6 ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_l")
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							-StressCalc.GetMapValueSecStress("Sigma_cut") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
							-StressCalc.GetMapValueSecStress("Sigma_clt") 
						   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" ┗━━━━━━┻━━━━━━┻━━━━━━┻━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┛");	Arr.Add(cs);
		}
		else     // 부모멘트
		{
			cs.Format(" ┏━━━━━━┳━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");	Arr.Add(cs);
			cs.Format(" ┃            ┃              RE-BAR                  ┃                        STEEL                         ┃");	Arr.Add(cs);
			cs.Format(" ┃            ┣━━━━━━━━━━━━━┳━━━━━╋━━━━━━━━━━━━━┳━━━━━━━━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃ 하중  조합 ┃     작용 응력 ( MPa )    ┃ 허용응력 ┃     작용 응력 ( MPa )    ┃     허용 응력 ( MPa )    ┃");	Arr.Add(cs);	
			cs.Format(" ┃            ┣━━━━━━┳━━━━━━┫          ┣━━━━━━┳━━━━━━╋━━━━━━┳━━━━━━┫");	Arr.Add(cs);
			cs.Format(" ┃            ┃  상    연  ┃  하    연  ┃   (MPa)  ┃  상    연  ┃  하    연  ┃  상    연  ┃ 하     연  ┃");	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaTA;
			Sigma_loA=dSigmaCABefore;

			Max_up=Sigma_up;
			Max_lo=Sigma_lo;
			Max_upA=Sigma_upA;
			Max_loA=Sigma_loA;

			cs.Format(" ┃1           ┃            ┃            ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= StressCalc.GetMapValueSecStress("Sigma_vsu")+StressCalc.GetMapValueSecStress("Sigma_vsu_l");
			Sigma_lo	+= StressCalc.GetMapValueSecStress("Sigma_vsl")+StressCalc.GetMapValueSecStress("Sigma_vsl_l");
			Sigma_upA	= dSigmaTA;
			Sigma_loA	= dSigmaCAAfter;

			double dAlpha			= ModelCalc.GetMapValueSecCo("WarpingAlpha");
			double Sigma_up_post	= StressCalc.GetMapValueSecStress("Sigma_vsu");
			double Sigma_up_live	= StressCalc.GetMapValueSecStress("Sigma_vsu_l");
			double Sigma_lo_post	= StressCalc.GetMapValueSecStress("Sigma_vsl");
			double Sigma_lo_live	= StressCalc.GetMapValueSecStress("Sigma_vsl_l");
			double dFw				= StressCalc.GetMapValueSecStress("Fw");

			if(dAlpha < 10 && Sigma_up_post+Sigma_up_live > 0)	Sigma_up += dFw;
			if(dAlpha < 10 && Sigma_up_post+Sigma_up_live < 0)	Sigma_up += -dFw;
			if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live > 0)	Sigma_lo += dFw;
			if(dAlpha < 10 && Sigma_lo_post+Sigma_lo_live < 0)	Sigma_lo += -dFw;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}
			cs.Format(" ┃1+2+3       ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= StressCalc.GetMapValueSecStress("Sigma_vsu1");
			Sigma_lo	+= StressCalc.GetMapValueSecStress("Sigma_vsl1");
			Sigma_upA	= dSigmaTA;
			Sigma_loA	= dSigmaCAAfter;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+4     ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= StressCalc.GetMapValueSecStress("Sigma_vsu2") ;
			Sigma_lo	+= StressCalc.GetMapValueSecStress("Sigma_vsl2") ;
			Sigma_upA	= dSigmaTA;
			Sigma_loA	= dSigmaCAAfter;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}
			cs.Format(" ┃1+2+3+4+5   ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= StressCalc.GetMapValueSecStress("Sigma_sut") ;
			Sigma_lo	+= StressCalc.GetMapValueSecStress("Sigma_slt") ;
			Sigma_upA	= dSigmaTA*1.15;
			Sigma_loA	= dSigmaCAAfter*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+4+5+6 ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						+StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						+StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┣━━━━━━╋━━━━━━╋━━━━━━╋━━━━━╋━━━━━━╋━━━━━━╋━━━━━━╋━━━━━━┫");	Arr.Add(cs);

			Sigma_up	+= -StressCalc.GetMapValueSecStress("Sigma_sut")*2;
			Sigma_lo	+= -StressCalc.GetMapValueSecStress("Sigma_slt")*2;
			Sigma_upA	= dSigmaTA*1.15;
			Sigma_loA	= dSigmaCAAfter*1.15;
			if(pow((Sigma_up/Sigma_upA),2) > pow((Max_up/Max_upA),2))
			{
				Max_up=Sigma_up;
				Max_upA=Sigma_upA;
			}
			if(pow((Sigma_lo/Sigma_loA),2) > pow((Max_lo/Max_loA),2))
			{
				Max_lo=Sigma_lo;
				Max_loA=Sigma_loA;
			}

			cs.Format(" ┃1+2+3+4+5-6 ┃%9.3f   ┃%9.3f   ┃%9.3f  ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃%9.3f   ┃"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						-StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						-StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" ┗━━━━━━┻━━━━━━┻━━━━━━┻━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┻━━━━━━┛");	Arr.Add(cs);
		}
	}

	cs.Format("    주) 압축응력을 정(+), 인장응력을 부(-)로 하였음.");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (%d) 합성응력의 검토", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format("     ( 상  연 )");	Arr.Add(cs);
	
	double Tau		= StressCalc.GetMapValueSecStress("Tau_shear");
	double TauA		= dTauAllow;
	double Result	= pow((Max_up/Max_upA),2) + pow((Tau/TauA),2);

	if(bHybrid)	cs.Format("	    (f  / R×fa)² + (υ / υa)²");
	else		cs.Format("	    (f  / fa)² + (υ / υa)²");
	Arr.Add(cs);
	if(fabs(Result) <= 1.2) 
	{		
		cs.Format("         =  (%.3f/%.3f)² + (%.3f/%.3f)² = %.4f < 1.2 ---- O.K"
						,Max_up,Max_upA*dHyR,Tau,TauA,Result);
	}
	else
	{
		cs.Format("         =  (%.3f/%.3f)² + (%.3f/%.3f)² = %.4f ≥1.2 ---- N.G"
						,Max_up,Max_upA,Tau,TauA,Result);
	}
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("     ( 하  연 )");	Arr.Add(cs);
	Result  = pow((Max_lo/Max_loA),2) + pow((Tau/TauA),2);
	
	if(bHybrid)	cs.Format("	    (f  / R×fa)² + (υ / υa)²");
	else		cs.Format("	    (f  / fa)² + (υ / υa)²");
	Arr.Add(cs);
	if(fabs(Result) <= 1.2) 
	{
		cs.Format("         =  (%.3f/%.3f)² + (%.3f/%.3f)² = %.4f < 1.2 ---- O.K"
						,Max_lo,Max_loA,Tau,TauA,Result);
	}
	else
	{
		cs.Format("         =  (%.3f/%.3f)² + (%.3f/%.3f)² = %.4f ≥1.2 ---- N.G"
						,Max_lo,Max_loA,Tau,TauA,Result);	}
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (%d) 항복에 대한 안전도 검사", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bWind)
	{
		cs.Format("   - 풍하중 비재하시");
		Arr.Add(cs);  
		cs.Format("     Σf = 1.3 × (합성전 고정하중 응력 + 합성후 고정하중) + 2.15 × 활하중에 의한 응력(충격포함)");
		Arr.Add(cs);  
		cs.Format("           + (크리프응력 변화량) + (건조수축응력변화량) + (온도차응력변화량) 의 조합 중에서");
		Arr.Add(cs);  
		cs.Format("           가장 불리한 하중 조합에 대하여 검토");
		Arr.Add(cs);  
		cs.Format("   - 풍하중 재하시");
		Arr.Add(cs);  
		cs.Format("    * 활하중 비재하시");
		Arr.Add(cs);  
		cs.Format("     Σf = 1.3 × (합성전 고정하중 응력 + 합성후 고정하중) + 1.3 × 풍하중에 의한 응력");
		Arr.Add(cs);  
		cs.Format("           + (크리프응력 변화량) + (건조수축응력변화량) + (온도차응력변화량) 의 조합 중에서");
		Arr.Add(cs);  
		cs.Format("           가장 불리한 하중 조합에 대하여 검토");
		Arr.Add(cs);  
		cs.Format("    * 활하중 재하시");
		Arr.Add(cs);  
		cs.Format("     Σf = 1.3 × (합성전 고정하중 응력 + 합성후 고정하중) + 2.15 × 활하중에 의한 응력");
		Arr.Add(cs);  
		cs.Format("           + 1.3 ×1/2 활하중 비재하시의 풍하중 응력 + 크리프 응력 + 건조수축 응력 + 온도차 응력");
		Arr.Add(cs);  
		cs.Format("           의 조합 중에서 가장 불리한 하중 조합에 대하여 검토");
		Arr.Add(cs);  
	}
	else
	{
		cs.Format("     위에 열거한 하중조합중에서 가장 불리한 조합에 대해 검사");
		Arr.Add(cs);
		cs.Format("     Σf = 1.3 ×(합성전 고정하중 응력 + 합성후 고정하중) + 2.15 ×(활하중에 의한 응력(충격포함))");
		Arr.Add(cs);
		cs.Format("           + (크리프 응력 변화량) + (건조수축 응력 변화량) + (온도차 응력 변화량)의 조합 중에서");
		Arr.Add(cs);
		cs.Format("           가장 불리한 하중 조합에 대하여 검토");
		Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);

	//////////////////////////////////////////////////////////////////////

	if(bPLUS_M)	cs.Format("   ① 바닥판 콘크리트");		// 정모멘트
	else		cs.Format("   ① 상부바닥판 REBAR");	// 부모멘트
	Arr.Add(cs);

	double Sigma_ckA;
	if(bPLUS_M)	Sigma_ckA = pData->DESIGN_MATERIAL.m_dSigmaCK*3/5;	// 정모멘트
	else		Sigma_ckA = 400;									// 부모멘트
		
	Sigma_ckA *= dHyR;
	
	double Sigma_YieldUp, Sigma_YieldLo;

	CalcGen.GetMaterial(pBx, G_F_U, sLeftUp, sRightUp);

	pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeftUp + sRightUp)];
	CHGAllowStress	AllowYeildStressUp(pSteel);

	Sigma_YieldUp = AllowYeildStressUp.GetAllowStress(ALLOW_STDYIELDPOINT_UP, dTu);

	CalcGen.GetMaterial(pBx, G_F_L, sLeftUp, sRightUp);
	
	pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sLeftUp + sRightUp)];
	CHGAllowStress	AllowYeildStressLo(pSteel);
	
	Sigma_YieldLo = AllowYeildStressLo.GetAllowStress(ALLOW_STDYIELDPOINT_UP, dTl);
	

	double Sigma_Dead,Sigma_Live,Sigma_Sigma,Sigma_Sigma2;
	double Irv = StressCalc.GetMapValueSecStress("Irv");
	if(bPLUS_M)	Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead")/ModelCalc.GetMapValueSecCo("Iv33")*ModelCalc.GetMapValueSecCo("Yvcu")/StressCalc.GetMapValueSecStress("N");
	else		Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead")/Irv*StressCalc.GetMapValueSecStress("Yvru");
	Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vcu_l");

	if(bWind)
	{
		cs.Format("   - 풍하중 비재하시");
		Arr.Add(cs);
	}
	else
	{
		cs.Format("     합성후 고정하중 응력 = %.3f MPa", Sigma_Dead);
		Arr.Add(cs);

		cs.Format("     합성후 활하중 응력 = %.3f MPa", Sigma_Live);
		Arr.Add(cs);
	}

	if(bPLUS_M)    // 정모멘트
	{
		cs.Format("     Σf = 1.3 × %.3f + 2.15 × %.3f + (%.3f) + (%.3f) + (%.3f)"
				   ,Sigma_Dead,Sigma_Live
				   ,StressCalc.GetMapValueSecStress("Sigma_cu1") 
				   ,StressCalc.GetMapValueSecStress("Sigma_cu2") 
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
		Arr.Add(cs);
		Sigma_Sigma=Sigma_Dead*1.3+Sigma_Live*2.15
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
	}
	else           // 부모멘트
	{
		cs.Format("     Σf = 1.3 × %.3f + 2.15 × %.3f + (%.3f) + (%.3f) - (%.3f)"
				   ,Sigma_Dead,Sigma_Live
				   ,StressCalc.GetMapValueSecStress("Sigma_cu1") 
				   ,StressCalc.GetMapValueSecStress("Sigma_cu2") 
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
		Arr.Add(cs);
		Sigma_Sigma=Sigma_Dead*1.3+Sigma_Live*2.15
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
							-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
	}

	cs.Format("         = %.3f MPa ",Sigma_Sigma);
	CString szText = _T("");
	if(bPLUS_M)    // 정모멘트
	{
		if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  (3/5)");
		else								szText.Format("  ≥ (3/5)");
		if(bHybrid)							szText += "R × ";
		szText += "f ck";
	}
	else           // 부모멘트
	{
		if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  f y");
		else								szText.Format("  ≥ f y");
		if(bHybrid)							szText += " × R";
	}
	if(Sigma_ckA > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (크리프,건조수축 포함시)",szText,Sigma_ckA);
	else								str.Format("%s = %.2f MPa  -------- N.G (크리프,건조수축 포함시)",szText,Sigma_ckA);
	Arr.Add(cs+str);

	if(bPLUS_M)    // 정모멘트
	{
		cs.Format("     Σf = 1.3 × %.3f + 2.15 × %.3f + (%.3f)"
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
		Arr.Add(cs);
		Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
							+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
	}
	else           // 부모멘트
	{
		cs.Format("     Σf = 1.3 × %.3f + 2.15 × %.3f - (%.3f)"
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
		Arr.Add(cs);
		Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
							-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
	}

	cs.Format("         = %.3f MPa ",Sigma_Sigma2);
	if(bPLUS_M)    // 정모멘트
	{
		if(Sigma_ckA > fabs(Sigma_Sigma2))	szText.Format("  <  (3/5)");
		else								szText.Format("  ≥ (3/5)");
		if(bHybrid)							szText += "R × ";
		szText += "f ck";
	}
	else           // 부모멘트
	{
		if(Sigma_ckA > fabs(Sigma_Sigma2))	szText.Format("  <  f y");
		else								szText.Format("  ≥ f y");
		if(bHybrid)							szText += " × R";	
	}
	if(Sigma_ckA > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (크리프,건조수축 제외시)",szText,Sigma_ckA);
	else								str.Format("%s = %.2f MPa  -------- N.G (크리프,건조수축 제외시)",szText,Sigma_ckA);
	Arr.Add(cs+str);
	cs.Format(""); Arr.Add(cs);

	double Sigma_Wind = 0;
	if(bWind)
	{
		cs.Format("   - 풍하중 재하시");
		Arr.Add(cs);  

		Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vcu_w");

		/////////////////////////////////////////////////////////
		// 활하중 비재하시

		if(bPLUS_M)    // 정모멘트
		{
			cs.Format("     Σf = 1.3 × %.3f + 1.3 × %.3f + (%.3f) + (%.3f) + (%.3f)"
					   ,Sigma_Dead, Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
			Arr.Add(cs);
			Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Wind * 1.3
								+StressCalc.GetMapValueSecStress("Sigma_cu1") 
								+StressCalc.GetMapValueSecStress("Sigma_cu2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
		}
		else           // 부모멘트
		{
			cs.Format("     Σf = 1.3 × %.3f + 1.3 × %.3f + (%.3f) + (%.3f) - (%.3f)"
					   ,Sigma_Dead,Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
			Arr.Add(cs);
			Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Wind * 1.3
								+StressCalc.GetMapValueSecStress("Sigma_cu1") 
								+StressCalc.GetMapValueSecStress("Sigma_cu2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
		}

		cs.Format("         = %.3f MPa ",Sigma_Sigma);
		if(bPLUS_M)    // 정모멘트
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  (3/5)");
			else								szText.Format("  ≥ (3/5)");
			if(bHybrid)							szText += "R × ";
			szText += "f ck";
		}
		else           // 부모멘트
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  f y");
			else								szText.Format("  ≥ f y");
			if(bHybrid)							szText += " × R";
		}
		if(Sigma_ckA > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (활하중 비재하시)",szText,Sigma_ckA);
		else								str.Format("%s = %.2f MPa  -------- N.G (활하중 비재하시)",szText,Sigma_ckA);
		Arr.Add(cs+str);

		/////////////////////////////////////////////////////////
		// 활하중 재하시

		if(bPLUS_M)    // 정모멘트
		{
			cs.Format("     Σf = 1.3 × %.3f + 2.15 × %.3f + 1.3 × %.3f/2 + (%.3f) + (%.3f) + (%.3f)"
					   ,Sigma_Dead, Sigma_Live, Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
			Arr.Add(cs);
			Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Live * 2.15 + Sigma_Wind / 2.0 * 1.3
								+StressCalc.GetMapValueSecStress("Sigma_cu1") 
								+StressCalc.GetMapValueSecStress("Sigma_cu2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
		}
		else           // 부모멘트
		{
			cs.Format("     Σf = 1.3 × %.3f + 2.15 × %.3f + 1.3 × %.3f/2 + (%.3f) + (%.3f) - (%.3f)"
					   ,Sigma_Dead, Sigma_Live, Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
			Arr.Add(cs);
			Sigma_Sigma = Sigma_Dead * 1.3 + Sigma_Live * 2.15 + Sigma_Wind / 2.0 * 1.3
								+StressCalc.GetMapValueSecStress("Sigma_cu1") 
								+StressCalc.GetMapValueSecStress("Sigma_cu2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
		}

		cs.Format("         = %.3f MPa ",Sigma_Sigma);
		if(bPLUS_M)    // 정모멘트
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  (3/5)");
			else								szText.Format("  ≥ (3/5)");
			if(bHybrid)							szText += "R × ";
			szText += "f ck";
		}
		else           // 부모멘트
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  f y");
			else								szText.Format("  ≥ f y");
			if(bHybrid)							szText += " × R";
		}
		if(Sigma_ckA > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (활하중 재하시)",szText,Sigma_ckA);
		else								str.Format("%s = %.2f MPa  -------- N.G (활하중 재하시)",szText,Sigma_ckA);
		Arr.Add(cs+str);
		cs.Format(""); Arr.Add(cs);
	}

	//////////////////////////////////////////////////////////////////////

	cs.Format("   ② 강재 주형 상판");
	Arr.Add(cs);

	if(bPLUS_M)    // 정모멘트
	{
		Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead")/
			ModelCalc.GetMapValueSecCo("Iv33")*ModelCalc.GetMapValueSecCo("Yvsu");
	}
	else           // 부모멘트
	{
		Sigma_Dead = (StressCalc.GetMapValueSecStress("MomentPostDead") / Irv
								*StressCalc.GetMapValueSecStress("Yvsu"));
	}
	Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l"); 

	if(bWind)
	{
		cs.Format("   - 풍하중 비재하시");
		Arr.Add(cs);  
	}
	else
	{
		cs.Format("     합성전 고정하중 응력 = %.3f MPa"
					,StressCalc.GetMapValueSecStress("Sigma_su") );
		Arr.Add(cs);
		cs.Format("     합성후 고정하중 응력 = %.3f MPa",Sigma_Dead);
		Arr.Add(cs);
		cs.Format("     합성후 활하중 응력 = %.3f MPa", Sigma_Live);
		Arr.Add(cs);
	}

	if(bPLUS_M)    // 정모멘트
	{
		cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + (%.3f) + (%.3f) + (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_su")
				   ,Sigma_Dead,Sigma_Live
				   ,StressCalc.GetMapValueSecStress("Sigma_vsu1") 
				   ,StressCalc.GetMapValueSecStress("Sigma_vsu2") 
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
		Arr.Add(cs);
		Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
							+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
							+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
	}
	else           // 부모멘트
	{
		cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + (%.3f) + (%.3f) - (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_su")
				   ,Sigma_Dead,Sigma_Live
				   ,StressCalc.GetMapValueSecStress("Sigma_vsu1") 
				   ,StressCalc.GetMapValueSecStress("Sigma_vsu2") 
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
		Arr.Add(cs);
		Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
							+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
							-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
	}
	cs.Format("         = %.3f MPa ",Sigma_Sigma);
	if(Sigma_YieldUp > fabs(Sigma_Sigma))	szText.Format("  <  f y");
	else								szText.Format("  ≥ f y");
	if(bHybrid)							szText += " × R";
	if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (크리프,건조수축 포함시)",szText,Sigma_YieldUp);
	else								str.Format("%s = %.2f MPa  -------- N.G (크리프,건조수축 포함시)",szText,Sigma_YieldUp);
	Arr.Add(cs+str);

	if(bPLUS_M)    // 정모멘트
	{
		cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_su")
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
		Arr.Add(cs);
		Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
	}
	else           // 부모멘트
	{
		cs.Format("     Σf = 1.3 ×(%.3f + %.3f) + 2.15 ×%.3f - (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_su")
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
		Arr.Add(cs);
		Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
	}
	cs.Format("         = %.3f MPa ",Sigma_Sigma2);

	if(Sigma_YieldLo > fabs(Sigma_Sigma2))	szText.Format("  <  f y");
	else									szText.Format("  ≥ f y");
	if(bHybrid)								szText += " × R";
	if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (크리프,건조수축 제외시)",szText,Sigma_YieldLo);
	else									str.Format("%s = %.2f MPa  -------- N.G (크리프,건조수축 제외시)",szText,Sigma_YieldLo);
	Arr.Add(cs+str);
	cs.Format(""); Arr.Add(cs);

	if(bWind)
	{
		cs.Format("   - 풍하중 재하시");
		Arr.Add(cs);  

		Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");

		/////////////////////////////////////////////////////////
		// 활하중 비재하시
		if(bPLUS_M)    // 정모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 1.3 × %.3f + (%.3f) + (%.3f) + (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_su")
					   ,Sigma_Dead,Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
			Arr.Add(cs);
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
								+Sigma_Wind*1.3
								+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		else           // 부모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 1.3 × %.3f + (%.3f) + (%.3f) - (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_su")
					   ,Sigma_Dead,Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
			Arr.Add(cs);
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
								+Sigma_Wind*1.3
								+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		cs.Format("         = %.3f MPa ",Sigma_Sigma);

		if(Sigma_YieldUp > fabs(Sigma_Sigma))	szText.Format("  <  f y");
		else								szText.Format("  ≥ f y");
		if(bHybrid)							szText += " × R";
		if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (활하중 비재하시)",szText,Sigma_YieldUp);
		else								str.Format("%s = %.2f MPa  -------- N.G (활하중 비재하시)",szText,Sigma_YieldUp);
		Arr.Add(cs+str);

		if(bPLUS_M)    // 정모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + 1.3 × %.3f/2 + (%.3f) + (%.3f) + (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_su")
					   ,Sigma_Dead,Sigma_Live,Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
			Arr.Add(cs);
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
								+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
								+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		else           // 부모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + 1.3 × %.3f/2 + (%.3f) + (%.3f) - (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_su")
					   ,Sigma_Dead,Sigma_Live,Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsu2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
			Arr.Add(cs);
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
								+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
								+StressCalc.GetMapValueSecStress("Sigma_vsu1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsu2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
		}
		cs.Format("         = %.3f MPa ",Sigma_Sigma2);
		if(Sigma_YieldLo > fabs(Sigma_Sigma2))	szText.Format("  <  f y");
		else									szText.Format("  ≥ f y");
		if(bHybrid)								szText += " × R";
		if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (활하중 재하시)",szText,Sigma_YieldLo);
		else									str.Format("%s = %.2f MPa  -------- N.G (활하중 재하시)",szText,Sigma_YieldLo);
		Arr.Add(cs+str);
	}
	cs.Format(""); Arr.Add(cs);

	///////////////////////////////////////////////////////////////////////

	cs.Format("   ③ 강재 주형 하판");
	Arr.Add(cs);

	if(bPLUS_M)    // 정모멘트
	{
		Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead")
								/ModelCalc.GetMapValueSecCo("Iv33")
								*ModelCalc.GetMapValueSecCo("Yvsl");
	}
	else           // 부모멘트
	{
		Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead") / Irv
								*StressCalc.GetMapValueSecStress("Yvsl");
	}

	Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l"); 

	if(bWind)
	{
		cs.Format("   - 풍하중 비재하시");
		Arr.Add(cs);  
	}
	else
	{
		cs.Format("     합성전 고정하중 응력 = %.3f MPa"
					,StressCalc.GetMapValueSecStress("Sigma_sl") );
		Arr.Add(cs);
		cs.Format("     합성후 고정하중 응력 = %.3f MPa",Sigma_Dead);
		Arr.Add(cs);
		cs.Format("     합성후 활하중 응력 = %.3f MPa", Sigma_Live);
		Arr.Add(cs);
	}

	if(bPLUS_M)    // 정모멘트
	{
		cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + (%.3f) + (%.3f) - (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_sl")
				   ,Sigma_Dead,Sigma_Live
				   ,StressCalc.GetMapValueSecStress("Sigma_vsl1") 
				   ,StressCalc.GetMapValueSecStress("Sigma_vsl2") 
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
		Arr.Add(cs);
		Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
							+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
							-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
	}
	else           // 부모멘트
	{
		cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + (%.3f) + (%.3f) + (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_sl")
				   ,Sigma_Dead,Sigma_Live
				   ,StressCalc.GetMapValueSecStress("Sigma_vsl1") 
				   ,StressCalc.GetMapValueSecStress("Sigma_vsl2") 
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
		Arr.Add(cs);
		Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
							+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
							+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
	}
	cs.Format("         = %.3f MPa ",Sigma_Sigma);
	if(Sigma_YieldUp > fabs(Sigma_Sigma))	szText.Format("  <  f y");
	else								szText.Format("  ≥ f y");
	if(bHybrid)							szText += " × R";
	if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (크리프,건조수축 포함시)",szText,Sigma_YieldUp);
	else								str.Format("%s = %.2f MPa  -------- N.G (크리프,건조수축 포함시)",szText,Sigma_YieldUp);
	Arr.Add(cs+str);

	if(bPLUS_M)    // 정모멘트
	{
		cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f - (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_sl")
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
		Arr.Add(cs);
		Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
	}
	else           // 부모멘트
	{
		cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_sl")
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
		Arr.Add(cs);
		Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
	}
	cs.Format("         = %.3f MPa ",Sigma_Sigma2);
	if(Sigma_YieldLo > fabs(Sigma_Sigma2))	szText.Format("  <  f y");
	else									szText.Format("  ≥ f y");
	if(bHybrid)								szText += " × R";
	if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (크리프,건조수축 제외시)",szText,Sigma_YieldLo);
	else									str.Format("%s = %.2f MPa  -------- N.G (크리프,건조수축 제외시)",szText,Sigma_YieldLo);
	Arr.Add(cs+str);
	cs.Format("");Arr.Add(cs);

	if(bWind)
	{
		cs.Format("   - 풍하중 재하시");
		Arr.Add(cs);  

		Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsl_w");

		/////////////////////////////////////////////////////////
		// 활하중 비재하시
		if(bPLUS_M)    // 정모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 1.3 × %.3f + (%.3f) + (%.3f) - (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_sl")
					   ,Sigma_Dead,Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
			Arr.Add(cs);
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
								+Sigma_Wind*1.3
								+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}
		else           // 부모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 1.3 × %.3f + (%.3f) + (%.3f) + (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_sl")
					   ,Sigma_Dead,Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
			Arr.Add(cs);
			Sigma_Sigma=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
								+Sigma_Wind*1.3
								+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}
		cs.Format("         = %.3f MPa ",Sigma_Sigma);
		if(Sigma_YieldUp > fabs(Sigma_Sigma))	szText.Format("  <  f y");
		else								szText.Format("  ≥ f y");
		if(bHybrid)							szText += " × R";
		if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (활하중 비재하시)",szText,Sigma_YieldUp);
		else								str.Format("%s = %.2f MPa  -------- N.G (활하중 비재하시)",szText,Sigma_YieldUp);
		Arr.Add(cs+str);

		if(bPLUS_M)    // 정모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + 1.3 × %.3f/2 + (%.3f) + (%.3f) - (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_sl")
					   ,Sigma_Dead,Sigma_Live, Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
			Arr.Add(cs);
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
								+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
								+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
								-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}
		else           // 부모멘트
		{
			cs.Format("     Σf = 1.3 × (%.3f + %.3f) + 2.15 × %.3f + 1.3 × %.3f/2 + (%.3f) + (%.3f) + (%.3f)"
					   ,StressCalc.GetMapValueSecStress("Sigma_sl")
					   ,Sigma_Dead,Sigma_Live, Sigma_Wind
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vsl2") 
					   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
			Arr.Add(cs);
			Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
								+Sigma_Live*2.15+Sigma_Wind*1.3/2.0
								+StressCalc.GetMapValueSecStress("Sigma_vsl1") 
								+StressCalc.GetMapValueSecStress("Sigma_vsl2") 
								+fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
		}
		cs.Format("         = %.3f MPa ",Sigma_Sigma2);
		if(Sigma_YieldLo > fabs(Sigma_Sigma2))	szText.Format("  <  f y");
		else									szText.Format("  ≥ f y");
		if(bHybrid)								szText += " × R";
		if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (활하중 재하시)",szText,Sigma_YieldLo);
		else									str.Format("%s = %.2f MPa  -------- N.G (활하중 재하시)",szText,Sigma_YieldLo);
		Arr.Add(cs+str);
	}
	cs.Format("");Arr.Add(cs);

// error check
//	if(pNem->GetSectionStress(nG,nIdx,"K")==-10.0)
//	AfxMessageBox("영향계수 k값이 계산되지 않았습니다!");

}
void CPlateStressOut::StressCheckVBracingJijumOutput(CStringArray &Arr, CPlateBasicIndex *pBx)
{
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	

	long nG = pBx->GetNumberGirder();
	CVBracing       *pVr  = pBx->GetSection()->GetVBracing();

	CString cs;
	cs.Format("< 수직브레이싱의 설계 - 거더:%d - %d>", nG+1, pVr->GetSelfCount()+1); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("현재 지원되지 않고 있습니다.");	Arr.Add(cs);


}

void CPlateStressOut::StressCheckVBracingGenOutput(CStringArray &Arr, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CCentSeparation	*pSepLeft	= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	CCentSeparation	*pSepRight	= pDB->GetCurGuardWall(CCentSeparation::RIGHT);
	long nG = pBx->GetNumberGirder();
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	CVBracing       *pVr  = pBx->GetSection()->GetVBracing();

	/// 필요한 데이터 계산

	double	dHeightLeft		= pSepLeft ? pSepLeft->GetHeight() : 0;
	double	dHeightRight	= pSepRight ? pSepRight->GetHeight() : 0;
	double dHBangHo = max(dHeightLeft, dHeightRight);	//방호벽높이(좌.우측중 큰것)
	double dSta     = (pGir->GetBxOnJijum(1)->GetStation() - pGir->GetBxOnJijum(0)->GetStation())/2;//측경간의 중앙스테이션
	double dH       = pGir->GetHeightGirderByStaAng(dSta) - pGir->GetThickFactByStation(dSta, G_F_U);		//거더의 높이(상부플랜지두께제외)
	double dWFlange = pGir->GetWidthOnStation(dSta, TRUE);											//거더 상부 플랜지의 폭			
	long   nTotalVBr = pGir->GetQtyVBracing();
	long   nCurVBracing = pGir->GetVBracingPosByBx(pBx);

	CPlateBasicIndex *pBxCompPre  = NULL;
	CPlateBasicIndex *pBxCompPost = NULL;
	if(pBx==pGir->GetBxOnJijum(0))	//처음 수직브레이싱
	{
		pBxCompPre  = NULL;
		pBxCompPost = pGir->GetBxByVBracingPos(1);
	}
	else if(pBx==pGir->GetBxOnJijum(pDB->m_nQtyJigan))	//마지막 수직브레이싱
	{
		pBxCompPre  = pGir->GetBxByVBracingPos(nTotalVBr-2);
		pBxCompPost = NULL;
	}
	else
	{	
		pBxCompPre  = pGir->GetBxByVBracingPos(nCurVBracing-1);
		pBxCompPost = pGir->GetBxByVBracingPos(nCurVBracing+1);
	}
	double dGapVBracingPre  = pBxCompPre ? fabs(pBxCompPre->GetStation()-pBx->GetStation()) : 0;
	double dGapVBracingPost = pBxCompPost? fabs(pBxCompPost->GetStation()-pBx->GetStation()) : 0;
	double dGapVBracing     = toM(min(dGapVBracingPre, dGapVBracingPost));

	// 수직브레이싱 빔의 제원(일반부)
//	long nSteelTypeUp = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_UVBEAM);
//	long nSteelTypeLo = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_LVBEAM);
//	long nSteelTypeC  = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_CVBEAM);
//	long nJewonTypeUp = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_UVBEAM);
//	long nJewonTypeLo = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_LVBEAM);
//	long nJewonTypeC  = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_CVBEAM);
	//상현
	CSteelMaterialData	*pMatData	= pVr->GetSteelMaterialData(STEELMAT_UPPER);
	double dHU  = pMatData->m_dH;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dH;
	double dBU  = pMatData->m_dB;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dB;
	double dT1U = pMatData->m_dT1;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dT1;
	double dLengthU = pVr->GetLengthBeam(0);
	//하현
	pMatData	= pVr->GetSteelMaterialData(STEELMAT_LOWER);
	//사재
	pMatData	= pVr->GetSteelMaterialData(STEELMAT_DIAGONAL);
	double dHC  = pMatData->m_dH;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dH;
	double dBC  = pMatData->m_dB;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dB;
	double dT1C = pMatData->m_dT1;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dT1;
	double dLengthC = pVr->GetLengthBeam(2);
	//
	double dHeightFrame = toM(pVr->GetLengthFrameLine(4));	// 프레임의 수직높이
	double dWidthHalf   = toM(pVr->GetLengthFrameLine(1)/2);// 프레임의 수평높이/2
	double D = toM(dHBangHo+pDB->m_dThickSlabBase+dH);
	double B = toM(pDB->m_dBridgeWidth);
	double Pw = (400-20*(B/D))*D;
	double He = Pw*dGapVBracing;
	double d = sqrt(pow(dHeightFrame,2)+pow(dWidthHalf,2));
	double Nd_af = -0.5*(-He*0.5)*d/dWidthHalf;
	double Nd_bf = -Nd_af;
	double Nu = -(He/2)-Nd_af*dWidthHalf/d;
	if(B/D >= 8)
		Pw = 240*D;
	CString cs;
	cs.Format("< 수직브레이싱의 설계 - 거더:%d - %d>", nG+1, pVr->GetSelfCount()+1);	Arr.Add(cs);
	Arr.Add("");
	
	Arr.Add  ("(1) 하중산정");
	Arr.Add  ("   ① 교량 총 높이");
	Arr.Add  ("     주형의 하부 플랜지 바닥에서 부터 방호벽의 끝까지");
	cs.Format("     D = %.3lf + %.3lf + %.3lf = %.3lf m", toM(dHBangHo), toM(pDB->m_dThickSlabBase), toM(dH), D);	Arr.Add(cs);
	Arr.Add  ("   ② 교량 총 폭");
	cs.Format("     B = %.3lf m", B);	Arr.Add(cs);
	Arr.Add  ("   ③ 풍하중 강도");
	cs.Format("     B/D = %.3lf / %.3lf = %.3lf", B, D, B/D);						Arr.Add(cs);
	cs.Format("       ∴ 1 %s B/D %s 8", 1<(B/D) ? "<":">", (B/D)<8 ? "<":">");		Arr.Add(cs);
	if(B/D<8)
	{
		Arr.Add("     Pw = [400 - 20 (B/D)]D ≥ 600 kgf/m");	
		cs.Format("          (400 - 20 × %.3lf) × %.3lf = %.3lf kgf/m %s 600 kgf/m", (B/D), D, Pw, Pw>600 ? ">":"<");		Arr.Add(cs);
	}
	else
	{
		Arr.Add("     Pw = 240 × D ≥ 600 kgf/m");
		cs.Format("          240 × %.3lf = %.3lf kgf/m %s 600 kgf/m", D, Pw, Pw>600 ? ">":"<");		Arr.Add(cs);
	}
	
	cs.Format("     수직브레이싱의 간격 : %.1f m  (최소 간격 규정 6m)", dGapVBracing);			Arr.Add(cs);
	cs.Format("                                   플랜지 폭 : %.0lf mm", dWFlange);				Arr.Add(cs);
	cs.Format("     1개의 수직브레이싱에 작용하는 하중 :");										Arr.Add(cs);
	cs.Format("       He = %.1lf × %.1lf m = %.1lf kgf", Pw, dGapVBracing, He);	Arr.Add(cs);

	Arr.Add  ("(2) 부재력 계산");
	Arr.Add  ("   ① 부재 제원");
	cs.Format("     상현재(ab), 하현재(eg) : L - %.0lf × %.0lf × %.0lf  (%s)", dHU, dBU, dT1U, pDB->GetMaterial(VB_UB));	Arr.Add(cs);
	cs.Format("     사재(af, bf)           : L - %.0lf × %.0lf × %.0lf  (%s)", dHC, dBC, dT1C, pDB->GetMaterial(VB_CR));	Arr.Add(cs);
	Arr.Add  ("   ② 부재력 계산(간략식으로 계산)");
	Arr.Add  ("     ⓐ 하현재(ef, fg)");
	cs.Format("        Nℓ = -1/2 * He = %.0lf kgf  %s", -He*0.5, He<0 ? "(압축)":"(인장)");
	Arr.Add(cs);
	Arr.Add  ("     ⓑ 사재(af, bf)");
	cs.Format("        Nd(af) = - 1/2 × Nℓ × d/ℓ = -1/2 × %.3lf × %.3lf / %.3lf = %.3lf kgf %s", -He*0.5, d, dWidthHalf, Nd_af, Nd_af<0 ? "(압축)":"(인장)");
	Arr.Add(cs);
	cs.Format("        Nd(bf) = - Nd(af) = %.3lf kgf %s", Nd_bf, Nd_bf<0 ? "(압축)":"(인장)");
	Arr.Add(cs);
	Arr.Add  ("     ⓒ 상현재(ab)");
	cs.Format("        Nu = - (He / 2) - Nd(af) ×ℓ/ d = - (%.3lf / 2) - %.3lf × %.3lf/%.3lf = %.3lf kgf %s", He, Nd_af, dWidthHalf, d, Nu, Nu<0 ? "(압축)":"(인장)");
	Arr.Add(cs);
	Arr.Add  ("(3) 단면결정 및 검토");
	for(long n=0; n<2; n++)	
	{
//		long	nSteelType	= n==0 ? nSteelTypeUp : nSteelTypeC;
//		long	nJewonType	= n==0 ? nJewonTypeUp : nJewonTypeC;
		CSteelMaterialData	*pMatData	= NULL;
		if(n==0)	pMatData	= pVr->GetSteelMaterialData(STEELMAT_UPPER);
		else		pMatData	= pVr->GetSteelMaterialData(STEELMAT_DIAGONAL);
		double	dArea		= pMatData->m_dSectionArea;//pMat->GetMatData(nSteelType,nJewonType)->m_dSectionArea;
//		double	dGammaMin	= MIN(  pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyrationMaxMin.x,
//									pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyrationMaxMin.y);
		double	dGammaMin	= MIN(pMatData->m_dRadiusOfGyrationMaxMin.x, pMatData->m_dRadiusOfGyrationMaxMin.y);
		double	dGammaX		= pMatData->m_dRadiusOfGyration.x;//pMat->GetMatData(nSteelType,nJewonType)->m_dRadiusOfGyration.x;
		double	dLength		= n==0?toCm(dLengthU):toCm(dLengthC);
		double	dSejang		= dLength/dGammaMin;
		double	dL_Div_Y	= dLength/dGammaX;
		double	dSigmaCa	= 1400 - 8.4*(dLength/dGammaX-20);
		double	dSigmaCaF	= dSigmaCa*(0.5+(dLength/dGammaX)/1000);
		double	dSigma		= abs(Nu)/dArea;
		cs.Format("   %s : L - %.0lf × %.0lf × %.0lf × %.0lf (%s)", n==0 ? "① 상 현 재":"② 사  재 :",
					n==0 ? dHU:dHC, n==0 ? dBU:dBC, n==0 ? dT1U:dT1C, dLength, pDB->GetMaterial(n==0 ? VB_UB:VB_CR));
		Arr.Add(cs);
		cs.Format("                 단면적                : A             %.3lf cm²", dArea);
		Arr.Add(cs);
		cs.Format("                 단면2차 반경의 최소값 : γmin         %.3lf cm", dGammaMin);
		Arr.Add(cs);
		cs.Format("                 단면2차 반경          : γx(= γy)    %.3lf cm", dGammaX);
		Arr.Add(cs);
		Arr.Add  ("     ⓐ 세장비 검토");
		cs.Format("        ℓ/γmin = %.1lf / %.1lf = %.3lf  %s  120   %s", dLength, dGammaMin, dSejang, dSejang<120 ? "<":">", dSejang<120 ? "O.K":"N.G");
		Arr.Add(cs);
		Arr.Add  ("     ⓑ 허용압축응력");
		cs.Format("        ℓ/γy   = %.1lf / %.1lf = %.3lf  ( %s  ℓ/γy  %s )", dLength, dGammaX, dL_Div_Y, dL_Div_Y>20 ? "20 <":"", dL_Div_Y<93 ? "< 93":"");
		Arr.Add(cs);
		cs.Format("          σca   = 1400 - 8.4·(ℓ/γ - 20) = 1400 - 8.4·(%.1lf/%.1lf - 20) = %.3lf kgf/cm²", dLength, dGammaX, dSigmaCa);
		Arr.Add(cs);
		Arr.Add  ("          ∴ GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여야 한다.");
		Arr.Add  ("");
		Arr.Add  ("           저감률은 0.5 + (ℓ/γx) / 1000");
		cs.Format("        ∴ σca' = σca × ( 0.5 + ( ℓ/γX ) / 1000 ) = %.1lf × ( 0.5 + ( %.1lf/%.1lf) / 1000 ) = %.3lf kgf/cm²",
							dSigmaCa, dLength, dGammaX, dSigmaCaF);
		Arr.Add(cs);
		Arr.Add  ("     ⓒ 응력검토");
		double dAbsNu = abs(Nu);
		double dA_Area = 0;
		if(n==1)
		{
			dA_Area = 3.0/4.0*dArea;
			Arr.Add  ("           인장 L형강의 유효단면적     (도로교 설계기준 p89)");
			cs.Format("           As = A × 3 / 4 = %.2lf × 3 / 4 = %.2lf cm²", dArea, dA_Area);
			Arr.Add(cs);
		}
		else	dA_Area = dArea;
		cs.Format("             σ  = %.1f / %.1f = %.3lf kgf/cm² %s σca'    %s", dAbsNu, dA_Area, dSigma, dSigma<dSigmaCaF ? "<":">", dSigma<dSigmaCaF ? "O.K":"N.G");
		Arr.Add(cs);
		Arr.Add("");
	}

}

// 단부 가로보(H형강타입, 제작타입)
void CPlateStressOut::StressCheckEndCrossBeamGeneralType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map)
{
	CPlateBridgeApp		*pDB		= m_pStd->GetBridge();
	CDesignCrossBeam	CrossBeam(m_pStd->m_pCalcStd, pBx);
	CDesignEndCrossBeam	*pEndCr		= (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CCalcData			*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CGlobarOption		*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	CFemModelingCalc ModelCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	

	CDesignHorBracing	HorBracing(m_pStd->m_pCalcStd, 0, 1);	

	CString	strText		= _T("");
	long	nG			= pBx->GetNumberGirder();
	double	dStation	= pBx->GetStation();
	double	dThickSlab	= pGir->GetThickSlab(pBx, 0);
	double	dThickPave	= pDB->m_dThickPave;
	double	dUnitPave	= pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt;
	double	dUnitConc	= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;
	double	dPr			= pCalcData->DESIGN_CONDITION.m_dPr;
	long	nGradeEarth			= pOpt->GetIdxDegreeEarthQuake();
	long	nRegionEarth		= pOpt->GetIdxRegionEarthQuake();
	double	dRiskCoefficient	= pOpt->GetRiskCoefficient();
	double	dZoneCoefficient	= pOpt->GetZoneCoefficient();
		
	Arr.Add("");
	strText.Format("    < 단부 가로보 - 거더 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("      사용강재 : %s", pEndCr->m_pJewonFlangeUp->m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("      플랜지폭 : bfu = %.0f mm(상부)    bfl = %.0f mm(하부)", pEndCr->m_pJewonFlangeUp->m_dWidth, pEndCr->m_pJewonFlangeLo->m_dWidth);
	Arr.Add(strText+"");
	strText.Format("      복부의폭 : bw = %.0f mm", pEndCr->m_pJewonWeb->m_dHeight);
	Arr.Add(strText+"");
	strText.Format("      수평보강재 사용단수 %d 단", pEndCr->m_nQtyHorStiff);
	Arr.Add(strText+"");
	double	dBfuP	= (pEndCr->m_pJewonFlangeUp->m_dWidth-pEndCr->m_pJewonWeb->m_dWidth)/2.0;
	double	dBflp	= (pEndCr->m_pJewonFlangeLo->m_dWidth-pEndCr->m_pJewonWeb->m_dWidth)/2.0;
	if(pEndCr->m_pJewonWeb->m_dWidth > pEndCr->m_strtMinThick.m_dThickWebReq)
		strText.Format("      tw = %.0f mm > twmin = %.1f mm .... O.K", pEndCr->m_pJewonWeb->m_dWidth, pEndCr->m_strtMinThick.m_dThickWebReq);
	else
		strText.Format("      tw = %.0f mm < twmin = %.1f mm .... N.G", pEndCr->m_pJewonWeb->m_dWidth, pEndCr->m_strtMinThick.m_dThickWebReq);
	Arr.Add(strText+"");
	strText.Format("      tfu(min) = bfu'/16 = %.3f / 16 = %.3f mm", dBfuP, pEndCr->m_strtMinThick.m_dMinThickFlangeUp);
	Arr.Add(strText+"");
	strText.Format("      tfl(min) = bfl'/16 = %.3f / 16 = %.3f mm", dBflp, pEndCr->m_strtMinThick.m_dMinThickFlangeLo);
	Arr.Add(strText+""+"");
	if(pEndCr->m_pJewonFlangeUp->m_dHeight>pEndCr->m_strtMinThick.m_dMinThickFlangeUp)
		strText.Format("      상판두께  tfu = %.0f mm > tfu(min)    .... O.K", pEndCr->m_pJewonFlangeUp->m_dHeight);
	else
		strText.Format("      상판두께  tfu = %.0f mm < tfu(min)    .... N.G", pEndCr->m_pJewonFlangeUp->m_dHeight);
	Arr.Add(strText+""+"");
	if(pEndCr->m_pJewonFlangeLo->m_dHeight>pEndCr->m_strtMinThick.m_dMinThickFlangeLo)
		strText.Format("      하판두께  tfl = %.0f mm > tfl(min)    .... O.K", pEndCr->m_pJewonFlangeLo->m_dHeight);
	else
		strText.Format("      하판두께  tfl = %.0f mm < tfl(min)    .... N.G", pEndCr->m_pJewonFlangeLo->m_dHeight);
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    가) 하 중 산 정");
	Arr.Add(strText+"");
	strText.Format("      (1) 연직하중");
	Arr.Add(strText+"");
	strText.Format("        1) 고정하중");
	Arr.Add(strText+"");
	strText.Format("          포장두께 Tp = %.3f m    상판두께 Tc = %.3f m", toM(dThickPave), toM(dThickSlab));
	Arr.Add(strText+"");
	strText.Format("          주형간격 a = %.3f m      S = %.3f m (수직브레이싱에서 상판단부까지의 거리)",
		toM(pEndCr->m_strtVerLoad.m_dGirderSpace), toM(pEndCr->m_strtVerLoad.m_dDistVerBracingToEndSlab));
	Arr.Add(strText+"");
	strText.Format("          포장 : %.3f × %.3f = %.3f kN/m²", toM(dThickPave), tokNPM3(dUnitPave), tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave));
	Arr.Add(strText+"");
	strText.Format("          상판 : %.3f × %.3f = %.3f kN/m²", toM(dThickSlab), tokNPM3(dUnitConc), tokNPM2(pEndCr->m_strtVerLoad.m_dLoadSlab));
	Arr.Add(strText+"");
	strText.Format("                           TOTAL = %.3f kN/m²", tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave+pEndCr->m_strtVerLoad.m_dLoadSlab));
	Arr.Add(strText+"");
	strText.Format("          q1 = %.3f × %.3f = %.3f kN/m²", toM(pEndCr->m_strtVerLoad.m_dDistVerBracingToEndSlab),
		tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave+pEndCr->m_strtVerLoad.m_dLoadSlab), tokNPM(pEndCr->m_strtVerLoad.m_dQ1));
	Arr.Add(strText+"");
	strText.Format("          q2 = %.3f × %.3f = %.3f kN/m²", toM(pEndCr->m_strtVerLoad.m_dGirderSpace/2),
		tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave+pEndCr->m_strtVerLoad.m_dLoadSlab), tokNPM(pEndCr->m_strtVerLoad.m_dQ2));
	Arr.Add(strText+""+"");
	strText.Format("        2) 활하중");
	Arr.Add(strText+"");
	strText.Format("           윤하중 : %.3f kN", tokN(pCalcData->DESIGN_CONDITION.m_dPr));
	Arr.Add(strText+"");
	strText.Format("           충격계수 : i = 15 / (40 + L) = 15 / (40 + %.3f)", toM(pEndCr->m_strtVerLoad.m_dGirderSpace));
	Arr.Add(strText+"");
	if(pEndCr->m_strtVerLoad.m_dCollisionCoeff>0.3)
		strText.Format("                        = %.3f > 0.3   ⇒  i = %.3f", pEndCr->m_strtVerLoad.m_dCollisionCoeff, pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
	else
		strText.Format("                        = %.3f < 0.3   ⇒  i = %.3f", pEndCr->m_strtVerLoad.m_dCollisionCoeff, pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
	Arr.Add(strText+"");
	strText.Format("      (2) 수평하중");
	Arr.Add(strText+"");
	strText.Format("          풍하중에 대하여는 2조의 단부 가로보가 받는 것으로 하며,");
	Arr.Add(strText+"");
	strText.Format("          지진하중에 대하여는 전체의 단부 가로보가 균등하게 부담하는 것으로 한다.");
	Arr.Add(strText+"");
	strText.Format("        1) 풍하중");
	Arr.Add(strText+"");
	strText.Format("           - 풍하중 강도");
	Arr.Add(strText+"");
	strText.Format("              ┌──────────┬──────────────┐");		Arr.Add(strText+"");
	strText.Format("              │     단 면 형 상    │        풍 하 중 (kN/m)     │");		Arr.Add(strText+"");
	strText.Format("              ├──────────┼──────────────┤");		Arr.Add(strText+"");
	strText.Format("              │   1 ≤ B/D < 8     │ [ 4.0 - 0.2(B/D) ] D ≥ 6  │");		Arr.Add(strText+"");
	strText.Format("              ├──────────┼──────────────┤");		Arr.Add(strText+"");
	strText.Format("              │      8 ≤ B/D      │         2.4 D ≥ 6         │");		Arr.Add(strText+"");
	strText.Format("              └──────────┴──────────────┘");		Arr.Add(strText+"");
	strText.Format("           - 풍하중 강도 계산");
	Arr.Add(strText+"");
	strText.Format("           교량 폭원 (B) = %.3f m", toM(pEndCr->m_strtHorLoad.m_dWidthBridge));
	Arr.Add(strText+"");
	strText.Format("           교량 높이 (D) = %.3f m", toM(pEndCr->m_strtHorLoad.m_dHeightBridge));
	Arr.Add(strText+"");
	strText.Format("           B / D = %.3f / %.3f = %.3f m",
		toM(pEndCr->m_strtHorLoad.m_dWidthBridge), toM(pEndCr->m_strtHorLoad.m_dHeightBridge), pEndCr->m_strtHorLoad.m_dRateWidthHeight);
	Arr.Add(strText+"");
	if(pEndCr->m_strtHorLoad.m_dRateWidthHeight>=1 && pEndCr->m_strtHorLoad.m_dRateWidthHeight<8)
		strText.Format("            1 ≤ B / D < 8  이므로  Ww = [ 4.0 - 0.2 ( B / D ) ] D = %.3f kN/m", tokN(pEndCr->m_strtHorLoad.m_dWw));
	else if(pEndCr->m_strtHorLoad.m_dRateWidthHeight>=8)
		strText.Format("              8 ≤ B / D   이므로  Ww = 2.4 ·D = %.3f kN/m", tokN(pEndCr->m_strtHorLoad.m_dWw));
	else
		strText.Format("              재검토 요망");
	Arr.Add(strText+"");
	if(tokN(pEndCr->m_strtHorLoad.m_dWw)>=6.0)
		strText.Format("              Ww ≥ 6.0 kN/m    이므로 ∴ Ww = %.3f", tokN(pEndCr->m_strtHorLoad.m_dWwUse));
	else
		strText.Format("              Ww < 6.0 kN/m    이므로 ∴ Ww = %.3f", tokN(pEndCr->m_strtHorLoad.m_dWwUse));
	Arr.Add(strText+"");
	strText.Format("           - 풍하중에 의한 브레이싱 1조의 작용력");
	Arr.Add(strText+"");
	strText.Format("             해당 지간장 L = %.3f m", toM(pEndCr->m_strtHorLoad.m_dLengthSpan));
	Arr.Add(strText+"");
	strText.Format("             거더수      n = %d", pDB->GetGirdersu());
	Arr.Add(strText+"");
	if(pDB->GetGirdersu()>=3)
		strText.Format("             Hw = 1/2 · Ww · L = %.3f kN", tokNM(pEndCr->m_strtHorLoad.m_dHw));
	else
		strText.Format("             Hw = Ww · L = %.3f kN", tokNM(pEndCr->m_strtHorLoad.m_dHw));
	Arr.Add(strText+"");
	strText.Format("        2) 지진하중");
	Arr.Add(strText+"");
	strText.Format("           - 해당 지점에 작용하는 고정하중");
	Arr.Add(strText+"");
	strText.Format("             ΣRd = 해당 지점반력의 합계 = %.3f kN", tokN(pEndCr->m_strtEarthQuakeLoad.m_dReaction));
	Arr.Add(strText+"");
	strText.Format("           - 지진 가속도 계수(A)");
	Arr.Add(strText+"");
	if(nGradeEarth==0)	strText.Format("              내진등급 : Ⅰ등급  ⇒  위험도 계수 : %.3f", dRiskCoefficient);
	else				strText.Format("              내진등급 : Ⅱ등급  ⇒  위험도 계수 : %.3f", dRiskCoefficient);
	Arr.Add(strText+"");
	if(nRegionEarth==0)	strText.Format("              내진구역 : Ⅰ구역  ⇒  지진구역계수 : %.3f", dZoneCoefficient);
	else				strText.Format("              내진구역 : Ⅱ구역  ⇒  지진구역계수 : %.3f", dZoneCoefficient);
	Arr.Add(strText+"");
	strText.Format("              A = %.3f × %.3f = %.3f", dRiskCoefficient, dZoneCoefficient, pEndCr->m_strtEarthQuakeLoad.m_dAcceleration);
	Arr.Add(strText+"");
	strText.Format("           - 단부 브레이싱에 작용하는 총지진하중");
	Arr.Add(strText+"");
	strText.Format("             ΣHe = A ΣRd = %.3f × %.3f = %.3f kN", pEndCr->m_strtEarthQuakeLoad.m_dAcceleration,
		tokN(pEndCr->m_strtEarthQuakeLoad.m_dReaction), tokN(pEndCr->m_strtEarthQuakeLoad.m_dTotEarthQuakeLoad));
	Arr.Add(strText+"");
	strText.Format("           - 지진하중에 의한 브레이싱 1조의 작용력");
	Arr.Add(strText+"");
	strText.Format("             He = 1/ (n-1)·ΣHe = 1/(%d-1) × %.3f = %.3f kN", pDB->GetGirdersu(),
		tokN(pEndCr->m_strtEarthQuakeLoad.m_dTotEarthQuakeLoad), tokN(pEndCr->m_strtEarthQuakeLoad.m_dEarthQuakeLoad));
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    나) 부재력 계산");								Arr.Add(strText+"");
	strText.Format("        1) 휨모멘트");								Arr.Add(strText+"");
	strText.Format("           고정하중에 의한 휨모멘트");				Arr.Add(strText+"");
	strText.Format("           Md = 1/8·q1·L² + 1/12·q2·L² ");	Arr.Add(strText+"");
	strText.Format("              = 1/8 × %.3f × %.3f² + 1/12 × %.3f × %.3f²", tokNPM(pEndCr->m_strtVerLoad.m_dQ1), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
		tokNPM(pEndCr->m_strtVerLoad.m_dQ1), toM(pEndCr->m_strtVerLoad.m_dGirderSpace), tokNM(pEndCr->m_strtElementLoad.m_dMd));
	Arr.Add(strText+"");
	strText.Format("              = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMd));
	Arr.Add(strText+"");
	strText.Format("           활하중에 의한 휨모멘트");
	Arr.Add(strText+"");
	if(pEndCr->m_strtVerLoad.m_dGirderSpace>2400.0)
	{
		strText.Format("           Ml = (1/4·Pr·L + 1/2·Pr·b )·( 1 + i )");
		Arr.Add(strText+"");
		strText.Format("              = (1/4 × %.1f × %.3f + 1/2 × %.1f × %.3f × (1 + %.3f)", tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
			tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace/2-1200), pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
		Arr.Add(strText+"");
		strText.Format("              = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMl));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("           Ml = 1/4·Pr·L·( 1 + i )");
		Arr.Add(strText+"");
		strText.Format("              = 1/4 × %.1f × %.3f × (1 + %.3f)", tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace), pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
		Arr.Add(strText+"");
		strText.Format("              = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMl));
		Arr.Add(strText+"");
	}
	strText.Format("          ΣM = Md + Ml = %.3f + %.3f = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMd), tokNM(pEndCr->m_strtElementLoad.m_dMl),
		tokNM(pEndCr->m_strtElementLoad.m_dMd+pEndCr->m_strtElementLoad.m_dMl));
	Arr.Add(strText+"");
	strText.Format("        2) 전단력");
	Arr.Add(strText+"");
	strText.Format("           Sd = 1/2·q1·L + 1/4·q2·L ");
	Arr.Add(strText+"");
	strText.Format("              = 1/2 × %.3f × %.3f + 1/4 × %.3f × %.3f", tokNPM(pEndCr->m_strtVerLoad.m_dQ1), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
		tokNPM(pEndCr->m_strtVerLoad.m_dQ2), toM(pEndCr->m_strtVerLoad.m_dGirderSpace));
	Arr.Add(strText+"");
	strText.Format("              = %.3f kN", tokN(pEndCr->m_strtElementLoad.m_dSd));
	Arr.Add(strText+"");
	strText.Format("           Sl = Pr·{ 1 + ( L-1.20 ) / L }·( 1 + i )");
	Arr.Add(strText+"");
	strText.Format("              = %.1f × { 1 + (%.3f - 1.2) / × %.3f } × (1 + i)", tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
		toM(pEndCr->m_strtVerLoad.m_dGirderSpace),	pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
	Arr.Add(strText+"");
	strText.Format("              = %.3f kN", tokN(pEndCr->m_strtElementLoad.m_dSl));
	Arr.Add(strText+"");
	strText.Format("          ΣS = Sd + Sl = %.3f + %.3f = %.3f kN.m", tokN(pEndCr->m_strtElementLoad.m_dSd), tokN(pEndCr->m_strtElementLoad.m_dSl),
		tokN(pEndCr->m_strtElementLoad.m_dSd+pEndCr->m_strtElementLoad.m_dSl));
	Arr.Add(strText+"");
	strText.Format("        3) 수평력");
	Arr.Add(strText+"");
	strText.Format("           풍하중    Hw = %.3f kN", tokNM(pEndCr->m_strtElementLoad.m_dHw));
	Arr.Add(strText+"");
	strText.Format("           지진하중  He = %.3f kN", tokN(pEndCr->m_strtElementLoad.m_dHe));
	Arr.Add(strText+"");
	strText.Format("        4) FRAME 해석결과집계");
	Arr.Add(strText+"");
	strText.Format("           하중분배작용에 대한 영향을 반영하기 위하여 FRAME 해석결과를 고려한다.");	
	Arr.Add(strText+"");
	strText.Format("       ┌────────┬───────────┬───────────┬───────────┐");	Arr.Add(strText+"");
	strText.Format("       │                │        M (kN.m)      │       S (kN)         │        N (kN)        │");	Arr.Add(strText+"");
	strText.Format("       │    구    분    ├─────┬─────┼─────┬─────┼─────┬─────┤");	Arr.Add(strText+"");
	strText.Format("       │                │    Max   │    Min   │    Max   │    Min   │    Max   │    Min   │");	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┴─────┼─────┴─────┼─────┴─────┤");	Arr.Add(strText+"");
	strText.Format("       │ 합성전 고정하중│  %12.3f        │  %12.3f        │  %12.3f        │",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dPreDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dPreDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       ├────────┼───────────┼───────────┼───────────┤");	Arr.Add(strText+"");
	strText.Format("       │ 합성후 고정하중│  %12.3f        │  %12.3f        │  %12.3f        │",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dPostDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dPostDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┬─────┼─────┬─────┼─────┬─────┤");	Arr.Add(strText+"");
	strText.Format("       │  활   하   중  │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMaxLiveLoad), tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMinLiveLoad),
		tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad),
		tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMaxLiveLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMinLiveLoad));	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │  지 점 침 하   │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMaxSettlement), tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMinSettlement),
		tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMinSettlement),
		tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMaxSettlement), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMinSettlement));	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┼─────┼─────┼─────┼─────┼─────┤");					Arr.Add(strText+"");
	strText.Format("       │     합    계   │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), tokNM(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment),
		tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinShear),
		tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxAxial), tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinAxial));	Arr.Add(strText+"");
	strText.Format("       └────────┴─────┴─────┴─────┴─────┴─────┴─────┘");				Arr.Add(strText+"");


	strText.Format("    다) 단면 설계");								Arr.Add(strText+"");
	strText.Format("        ┌──────────┬─────┬─────┬─────┬──────┬──────┐");	Arr.Add(strText+"");
	strText.Format("        │     구      분     │  A(mm²) │  Y (mm)  │ AY (mm³)│ AY²(mm⁴) │  Io(mm⁴)  │");	Arr.Add(strText+"");
	strText.Format("        ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("        │1 - UFLG-%4.0f × %2.0f │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pEndCr->m_pJewonFlangeUp->m_dWidth, pEndCr->m_pJewonFlangeUp->m_dHeight, pEndCr->m_pJewonFlangeUp->m_dA,
		pEndCr->m_pJewonFlangeUp->m_dY,	pEndCr->m_pJewonFlangeUp->m_dAY, pEndCr->m_pJewonFlangeUp->m_dAY2, pEndCr->m_pJewonFlangeUp->m_dIo);Arr.Add(strText+"");
	strText.Format("        ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("        │1 - WEB -%4.0f × %2.0f │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pEndCr->m_pJewonWeb->m_dHeight, pEndCr->m_pJewonWeb->m_dWidth, pEndCr->m_pJewonWeb->m_dA,
		pEndCr->m_pJewonWeb->m_dY, pEndCr->m_pJewonWeb->m_dAY, pEndCr->m_pJewonWeb->m_dAY2, pEndCr->m_pJewonWeb->m_dIo);	Arr.Add(strText+"");
	strText.Format("        ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("        │1 - LFLG-%4.0f × %2.0f │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pEndCr->m_pJewonFlangeLo->m_dWidth, pEndCr->m_pJewonFlangeLo->m_dHeight, pEndCr->m_pJewonFlangeLo->m_dA,
		pEndCr->m_pJewonFlangeLo->m_dY,	pEndCr->m_pJewonFlangeLo->m_dAY, pEndCr->m_pJewonFlangeLo->m_dAY2, pEndCr->m_pJewonFlangeLo->m_dIo);Arr.Add(strText+"");
	strText.Format("        ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("        │      합      계    │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pEndCr->GetTotSectionArea(), 0.0, pEndCr->GetTotSectionAY(), pEndCr->GetTotSectionAY2(), pEndCr->GetTotSectionIo());Arr.Add(strText+"");
	strText.Format("        └──────────┴─────┴─────┴─────┴──────┴──────┘");	Arr.Add(strText+"");
	strText.Format("       δ = ΣA·Y / ΣA = %.1f mm", pEndCr->m_strtSection.m_dDelta);					Arr.Add(strText+"");
	strText.Format("        I = ΣIo + ΣA·Y² - ΣA·δ² = %.1f mm⁴", pEndCr->m_strtSection.m_dI);		Arr.Add(strText+"");
	strText.Format("      Ysu =  bw / 2 + tfu - δ = %.1f mm", pEndCr->m_strtSection.m_dYsu);				Arr.Add(strText+"");
	strText.Format("      Ysl =  bw / 2 + tfl + δ = %.1f mm", pEndCr->m_strtSection.m_dYsl);				Arr.Add(strText+"");
	strText.Format("      압축부의 국부좌굴에 대한 허용응력은");											Arr.Add(strText+"");
	strText.Format("       ∴ fca = %.3f MPa,    σta = %.3f MPa,    υa = %.3f MPa",
		pEndCr->m_strtSection.m_dFca, pEndCr->m_strtSection.m_dSigmata, pEndCr->m_strtSection.m_dSheara);	Arr.Add(strText+"");Arr.Add("");
	strText.Format("- 부재력의 조합및 응력검토");															Arr.Add(strText+"");
	strText.Format("   다음과 같은 방식으로 각 하중조합에 대한 응력도를 조사한다.");						Arr.Add(strText+"");
	strText.Format("   fu = M·Ysu / I ± N / Ag,   fl = M·Ysl / I ± N / Ag,   υ = S / Aw");				Arr.Add(strText+"");
	CString	strChkFlange	= _T("");
	CString	strChkShear		= _T("");
	strText.Format("        ┌────────┬─────┬─────┬─────┬─────┬─────┬──┬─────┬──┐");	Arr.Add(strText+"");
	strText.Format("        │    구    분    │M (kN·m )│   S(kN)  │   N(kN)  │  fu(MPa) │  fl(MPa) │판정│υ( MPa ) │판정│");	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │1. 고정하중     │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │2. 활하중       │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │3. 풍하중       │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │4. 지진하중     │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │상시(1+2)       │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │풍시(1+2+3)/1.25│ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │지진시(1+4)/1.5 │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        ├────────┼─────┼─────┼─────┼─────┼─────┼──┼─────┼──┤");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        │FRAME해석결과   │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │%s │ %8.3f │%s │",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        └────────┴─────┴─────┴─────┴─────┴─────┴──┴─────┴──┘");	Arr.Add(strText+"");Arr.Add("");

	CCrossBeamVerStiff	*pVerStiff	= pEndCr->m_pVerStiff;
	CCrossBeamHorStiff	*pHorStiff	= pEndCr->m_pHorStiff;
	strText.Format("         수직보강재를 생략할 수 있는 플랜지 순간격의 최대치 [도.설 3.8.5.1]");		Arr.Add(strText+"");
	if(pVerStiff->m_dSpaceCoeff>1.2)
		strText.Format("          √(υa/υ) = √( %.3f / %.3f ) = %.3f > 1.2  ⇒ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear,	pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	else
		strText.Format("          √(υa/υ) = √( %.3f / %.3f ) = %.3f < 1.2  ⇒ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear, pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	Arr.Add(strText+"");
	if(pVerStiff->m_dMaxSpace<pVerStiff->m_dHeightWeb)
		strText.Format("          %.0f × tw = %.1f mm  <  bw = %.0f mm  수직보강재 필요", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	else
		strText.Format("          %.0f × tw = %.1f mm  >  bw = %.0f mm  수직보강재 필요없음", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	Arr.Add(strText+"");

	if(pVerStiff->m_bIsNeedVerStiff)	VerStiff(Arr, pVerStiff, pEndCr->m_nQtyHorStiff);
	if(pEndCr->m_nQtyHorStiff>0)		HorStiff(Arr, pHorStiff);
}

void CPlateStressOut::VerStiff(CStringArray &Arr, CCrossBeamVerStiff *pVerStiff, long nQtyHorStiff)
{
	CString	strText;

	strText.Format("     * 수직보강재  ( %s 사용 )", pVerStiff->m_strMaterial);									Arr.Add(strText+"");
	strText.Format("       수직보강재 간격 : a = %.1f mm [도.설 3.8.5.1]", pVerStiff->m_dVerStiffSpace);		Arr.Add(strText+"");
	if(pVerStiff->m_dRateSpaceHeight<1.5)
		strText.Format("        a / b = %.1f / %.1f = %.3f  <  1.5    .... O.K", pVerStiff->m_dVerStiffSpace, pVerStiff->m_dHeightWeb, pVerStiff->m_dRateSpaceHeight);
	else
		strText.Format("        a / b = %.1f / %.1f = %.3f  >  1.5    .... N.G", pVerStiff->m_dVerStiffSpace, pVerStiff->m_dHeightWeb, pVerStiff->m_dRateSpaceHeight);
	Arr.Add(strText+"");
	strText.Format("       수직보강재 폭 [도.설 3.8.5.2]");	Arr.Add(strText+"");
	if(pVerStiff->m_dBvs > pVerStiff->m_dBvsMin)
		strText.Format("        bvs = %.1f mm  >  bvs(min) = b / 30 + 50 = %.1f mm    .... O.K", pVerStiff->m_dBvs, pVerStiff->m_dBvsMin);
	else
		strText.Format("        bvs = %.1f mm  <  bvs(min) = b / 30 + 50 = %.1f mm    .... N.G", pVerStiff->m_dBvs, pVerStiff->m_dBvsMin);
	Arr.Add(strText+"");
	strText.Format("       수직보강재 두께");	Arr.Add(strText+"");
	if(pVerStiff->m_dTvs > pVerStiff->m_dTvsMin)
		strText.Format("        tvs = %.1f mm  >  tvs(min) = b / 30 + 50 = %.1f mm    .... O.K", pVerStiff->m_dTvs, pVerStiff->m_dTvsMin);
	else
		strText.Format("        tvs = %.1f mm  <  tvs(min) = b / 30 + 50 = %.1f mm    .... N.G", pVerStiff->m_dTvs, pVerStiff->m_dTvsMin);
	Arr.Add(strText+"");
	strText.Format("       수직보강재의 간격검토");Arr.Add(strText+"");
	if(nQtyHorStiff==0)
	{
		if(pVerStiff->m_dRateSpaceHeight>1.0)
		{
			strText.Format("        수평보강재를 사용하지 않고  a/b ＞ 1");
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )⁴ + [ ( f / 365 ) ² + { υ / ( 61 + 81(b/a)²) }² ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
		else
		{
			strText.Format("        수평보강재를 사용하지 않고  a/b ≤ 1");
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )⁴ + [ ( f / 365 ) ² + { υ / ( 81 + 61(b/a)²) }² ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
	}
	else if(nQtyHorStiff==1)
	{
		if(pVerStiff->m_dRateSpaceHeight>0.8)
		{
			strText.Format("        수평보강재 %d단을 사용하고  a/b ＞ 0.8", 1);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )⁴ + [ ( f / 950 ) ² + { υ / ( 127 + 61(b/a)²) }² ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
		else
		{
			strText.Format("        수평보강재 %d단을 사용하고  a/b ≤ 0.8", 1);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )⁴ + [ ( f / 950 ) ² + { υ / ( 95 + 81(b/a)²) }² ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
	}
	else if(nQtyHorStiff==2)
	{
		if(pVerStiff->m_dRateSpaceHeight>0.64)
		{
			strText.Format("        수평보강재 %d단을 사용하고  a/b ＞ 0.64", 2);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )⁴ + [ ( f / 3150 ) ² + { υ / ( 197 + 61(b/a)²) }² ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
		else
		{
			strText.Format("        수평보강재 %d단을 사용하고  a/b ≤ 0.64", 2);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )⁴ + [ ( f / 3150 ) ² + { υ / ( 148 + 81(b/a)²) }² ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.4f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.4f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
	}
	strText.Format("      수직보강재의 강도검토    [도.설 3.8.5.2]");
	Arr.Add(strText+"");
	strText.Format("       γ = 8.0 × (b/a)² = %.3f", pVerStiff->m_dGamma);
	Arr.Add(strText+"");
	strText.Format("        I(req) = b·t³·γ / 10.92 = %.1f mm⁴", pVerStiff->m_dIReq);
	Arr.Add(strText+"");
	if(pVerStiff->m_dIUse>pVerStiff->m_dIReq)
		strText.Format("        I(use) = %.1f × %.1f³/ 3 × %d본 = %.1f mm⁴  >  I(req)    .... O.K", pVerStiff->m_dTvs, pVerStiff->m_dBvs, pVerStiff->m_nVerSide, pVerStiff->m_dIUse);
	else
		strText.Format("        I(use) = %.1f × %.1f³/ 3 × %d본 = %.1f mm⁴  <  I(req)    .... N.G", pVerStiff->m_dTvs, pVerStiff->m_dBvs, pVerStiff->m_nVerSide, pVerStiff->m_dIUse);
	Arr.Add(strText+"");
}

void CPlateStressOut::HorStiff(CStringArray &Arr, CCrossBeamHorStiff *pHorStiff)
{
	CString	strText;

	strText.Format("     * 수평보강재  ( %s 사용 )", pHorStiff->m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        수직보강재의 간격      :   a = %.1f mm", pHorStiff->m_dVerStiffSpace);
	Arr.Add(strText+"");
	strText.Format("        사용 수평보강재의 폭   : bhs = %.1f mm", pHorStiff->m_dBhs);
	Arr.Add(strText+"");
	if(pHorStiff->m_dThs>pHorStiff->m_dThsMin)
		strText.Format("        사용 수평보강재의 두께 : ths = %.1f mm  >  tmin = b / 16 = %.3f mm    .... O.K", pHorStiff->m_dThs, pHorStiff->m_dThsMin);
	else
		strText.Format("        사용 수평보강재의 두께 : ths = %.1f mm  <  tmin = b / 16 = %.3f mm    .... N.G", pHorStiff->m_dThs, pHorStiff->m_dThsMin);
	Arr.Add(strText+"");
	strText.Format("      γ = 30.0 × (a/b) = 30.0 × ( %.3f / %.3f ) = %.3f", pHorStiff->m_dVerStiffSpace, pHorStiff->m_dHeightWeb, pHorStiff->m_dGamma);
	Arr.Add(strText+"");
	strText.Format("       I(req) = b·t³/ 10.92 × γh = %.1f mm⁴", pHorStiff->m_dIReq);
	Arr.Add(strText+"");
	strText.Format("       I(use) = ⅓ × %.0f × %.0f³ = %.1fmm⁴", pHorStiff->m_dThs, pHorStiff->m_dBhs, pHorStiff->m_dIUse);
	Arr.Add(strText+"");
}

// 단부 가로보(역V브레이싱형 타입)
void CPlateStressOut::StressCheckEndCrossBeamVBracingType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map)
{
	CPlateBridgeApp			*pDB		= m_pStd->GetBridge();
	CCalcData				*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	CDesignCrossBeam		CrossBeam(m_pStd->m_pCalcStd, pBx);
	CDesignInverseVBracing	*pInVBracing	= (CDesignInverseVBracing*)CrossBeam.GetDesignCrossBeam();
	CPlateGirderApp			*pGir		= pBx->GetGirder();
	CGlobarOption			*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();

	CString	strText			= _T(""), str = _T("");
	long	nG				= pBx->GetNumberGirder();
	double	dStation		= pBx->GetStation();
	double	dPr				= pCalcData->DESIGN_CONDITION.m_dPr;
	double	dUnitPave		= pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt;
	double	dUnitConc		= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;
	double	dThickSlab		= pGir->GetThickSlab(pBx, 0);
	double	dThickPave		= pDB->m_dThickPave;
	double	dSumLoad		= pInVBracing->m_strtVerLoad.m_dLoadPave+pInVBracing->m_strtVerLoad.m_dLoadSlab;
	long	nGradeEarth		= pOpt->GetIdxDegreeEarthQuake();
	long	nRegionEarth	= pOpt->GetIdxRegionEarthQuake();
	double	dRiskCoefficient	= pOpt->GetRiskCoefficient();
	double	dZoneCoefficient	= pOpt->GetZoneCoefficient();

	Arr.Add("");
	strText.Format("    < 단부 수직브레이싱(역 V형) - 거더 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");
	strText.Format("    ·h = %.1f mm    ·a = %.1f mm    ·θ = %.3f°", pInVBracing->m_dHeight, pInVBracing->m_dGirderSpace, ToDegree(pInVBracing->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("     ·사 용 강 재");
	Arr.Add(strText+"");
	strText.Format("       ┌──────┬──────────┬─────┬─────┬─────┬─────┬─────┐");	Arr.Add(strText+"");
	strText.Format("       │   부   재  │    강 재 규 격     │ A (mm²) │ γx (mm) │ γy (mm) │  γmin   │ ℓ (mm)  │");	Arr.Add(strText+"");
	strText.Format("       ├──────┼──────────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │   상현재   │ L - %3.0f X %3.0f X %2.0f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │", pInVBracing->m_pChordUp->m_dHeight, pInVBracing->m_pChordUp->m_dWidth,
		pInVBracing->m_pChordUp->m_dThick1, pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_pChordUp->m_dGammaX, pInVBracing->m_pChordUp->m_dGammaY, pInVBracing->m_pChordUp->m_dGammaMin, pInVBracing->m_pChordUp->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       ├──────┼──────────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │   사  재   │ L - %3.0f X %3.0f X %2.0f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │", pInVBracing->m_pDiagonal->m_dHeight, pInVBracing->m_pDiagonal->m_dWidth,
		pInVBracing->m_pChordUp->m_dThick1, pInVBracing->m_pDiagonal->m_dArea, pInVBracing->m_pDiagonal->m_dGammaX, pInVBracing->m_pDiagonal->m_dGammaY, pInVBracing->m_pDiagonal->m_dGammaMin, pInVBracing->m_pDiagonal->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       ├──────┼──────────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │   하현재   │ L - %3.0f X %3.0f X %2.0f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │", pInVBracing->m_pChordLo->m_dHeight, pInVBracing->m_pChordLo->m_dWidth,
		pInVBracing->m_pChordUp->m_dThick1, pInVBracing->m_pChordLo->m_dArea, pInVBracing->m_pChordLo->m_dGammaX, pInVBracing->m_pChordLo->m_dGammaY, pInVBracing->m_pChordLo->m_dGammaMin, pInVBracing->m_pChordLo->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       └──────┴──────────┴─────┴─────┴─────┴─────┴─────┘");	Arr.Add(strText+"");
	strText.Format("    가) 하 중 산 정");	Arr.Add(strText+"");
	strText.Format("      (1) 연직하중");	Arr.Add(strText+"");
	strText.Format("       1) 고정하중");	Arr.Add(strText+"");
	strText.Format("        · 포장두께 Tp = %.3f m    · 상판두께 Tc = %.3f m ", toM(dThickPave), toM(dThickSlab));
	Arr.Add(strText+"");
	strText.Format("        · 주형간격 a = %.3f m     · S = %.3f m", toM(pInVBracing->m_dGirderSpace), toM(pInVBracing->m_strtVerLoad.m_dDistVerBracingToEndSlab));
	Arr.Add(strText+"");
	strText.Format("            S : 수직브레이싱에서 상판단부까지의 거리");
	Arr.Add(strText+"");
	strText.Format("            포장 : %.3f × %.3f = %.3f kN/m²", toM(dThickPave), tokNPM3(dUnitPave), tokNPM2(pInVBracing->m_strtVerLoad.m_dLoadPave));
	Arr.Add(strText+"");
	strText.Format("            상판 : %.3f × %.3f = %.3f kN/m²", toM(dThickSlab), tokNPM3(dUnitConc), tokNPM2(pInVBracing->m_strtVerLoad.m_dLoadSlab));
	Arr.Add(strText+"");
	strText.Format("                           합계 : %.3f kN/m²", tokNPM2(dSumLoad));
	Arr.Add(strText+"");
	strText.Format("            q1 = %.3f × %.3f = %.3f kN/m", toM(pInVBracing->m_strtVerLoad.m_dDistVerBracingToEndSlab), tokNPM2(dSumLoad), tokNPM(pInVBracing->m_strtVerLoad.m_dQ1));
	Arr.Add(strText+"");
	strText.Format("            q2 = %.3f × %.3f = %.3f kN/m", toM(pInVBracing->m_dGirderSpace/2), tokNPM2(dSumLoad), tokNPM(pInVBracing->m_strtVerLoad.m_dQ2));
	Arr.Add(strText+"");
	strText.Format("            Pd = q1·a/2 + 1/3·q2·a = %.3f kN", tokN(pInVBracing->m_strtVerLoad.m_dPd));
	Arr.Add(strText+"");
	strText.Format("       2) 활하중     Pr = %.0f kN", tokN(dPr));
	Arr.Add(strText+"");
	strText.Format("         Pr·Σy = %.0f × %.3f = %.3f kN", tokN(dPr), pInVBracing->m_strtVerLoad.m_dLength, tokN(dPr*pInVBracing->m_strtVerLoad.m_dLength));
	Arr.Add(strText+"");
	strText.Format("          N1 = 0");
	Arr.Add(strText+"");
	strText.Format("          N2 = Pr·Σy / (2·sinΘ)      sinΘ = %.3f", sin(pInVBracing->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("          N3 = Pr·Σy / (2·tanΘ)      tanΘ = %.3f", tan(pInVBracing->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("      (2) 수평하중");
	Arr.Add(strText+"");
	strText.Format("          풍하중에 대하여는 2조의 단부수직브레이싱이 받는 것으로 하며,");
	Arr.Add(strText+"");
	strText.Format("          지진하중에 대하여는 전체의 단부수직브레이싱이 균등하게 부담하는 것으로 한다.");
	Arr.Add(strText+"");
	strText.Format("       1) 풍하중");
	Arr.Add(strText+"");
	strText.Format("         - 풍하중 강도");
	Arr.Add(strText+"");
	strText.Format("            ┌──────────┬──────────────┐");		Arr.Add(strText+"");
	strText.Format("            │     단 면 형 상    │        풍 하 중 (kN/m)     │");		Arr.Add(strText+"");
	strText.Format("            ├──────────┼──────────────┤");		Arr.Add(strText+"");
	strText.Format("            │   1 ≤ B/D < 8     │ [ 4.0 - 0.2(B/D) ] D ≥ 6  │");		Arr.Add(strText+"");
	strText.Format("            ├──────────┼──────────────┤");		Arr.Add(strText+"");
	strText.Format("            │      8 ≤ B/D      │         2.4 D ≥ 6         │");		Arr.Add(strText+"");
	strText.Format("            └──────────┴──────────────┘");		Arr.Add(strText+"");
	strText.Format("         - 풍하중 강도 계산");
	Arr.Add(strText+"");
	strText.Format("           교량 폭원 (B) = %.3f m", toM(pInVBracing->m_strtHorLoad.m_dWidthBridge));
	Arr.Add(strText+"");

	CCentSeparation	*pSep				= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	double			dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double			dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double			dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double			dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;

	double dBridgeHeight = toM(pBx->GetHeightGirderByBx() + pGir->GetThickSlab(pBx, 0) + dHeightGuardFence + dHeightSoundProof + dHeightHandRail);
	strText.Format("           교량 높이 (D) = %.3f m", dBridgeHeight);
	Arr.Add(strText+"");
	strText.Format("           B / D = %.3f / %.3f = %.3f m",
		toM(pInVBracing->m_strtHorLoad.m_dWidthBridge), dBridgeHeight, pInVBracing->m_strtHorLoad.m_dRateWidthHeight);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtHorLoad.m_dRateWidthHeight>=1 && pInVBracing->m_strtHorLoad.m_dRateWidthHeight<8)
		strText.Format("            1 ≤ B / D < 8  이므로  Ww = [ 4.0 - 0.2 ( B / D ) ] D = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWw));
	else if(pInVBracing->m_strtHorLoad.m_dRateWidthHeight>=8)
		strText.Format("              8 ≤ B / D   이므로  Ww = 2.4 ·D = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWw));
	else
		strText.Format("              재검토 요망");
	Arr.Add(strText+"");
	if(tokN(pInVBracing->m_strtHorLoad.m_dWw)>=6.0)
		strText.Format("              Ww ≥ 6.0 kN/m    이므로 ∴ Ww = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWwUse));
	else
		strText.Format("              Ww < 6.0 kN/m    이므로 ∴ Ww = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWwUse));
	Arr.Add(strText+"");
	strText.Format("           - 풍하중에 의한 브레이싱 1조의 작용력");
	Arr.Add(strText+"");
	strText.Format("             해당 지간장 L = %.3f m", toM(pInVBracing->m_strtHorLoad.m_dLengthSpan));
	Arr.Add(strText+"");
	strText.Format("             거더수      n = %d", pDB->GetGirdersu());
	Arr.Add(strText+"");
	strText.Format("             풍하중 계산시 허용응력 할증계수 : 1.2");
	Arr.Add(strText+"");
	if(pDB->GetGirdersu()>=3)
		strText.Format("             Hw = 1/2 · Ww · L / 1.2 = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dHw));
	else
		strText.Format("             Hw = Ww · L  / 1.2 = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dHw));
	Arr.Add(strText+"");
	strText.Format("       2) 지진하중");
	Arr.Add(strText+"");
	strText.Format("         - 해당 지점에 작용하는 고정하중");
	Arr.Add(strText+"");
	strText.Format("         ΣRd = 해당 지점반력의 합계 = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dReaction));
	Arr.Add(strText+"");
	strText.Format("         - 지진 가속도 계수(A)");
	Arr.Add(strText+"");
	if(nGradeEarth==0)	strText.Format("              내진등급 : Ⅰ등급  ⇒  위험도 계수 : %.3f", dRiskCoefficient);
	else				strText.Format("              내진등급 : Ⅱ등급  ⇒  위험도 계수 : %.3f", dRiskCoefficient);
	Arr.Add(strText+"");
	if(nRegionEarth==0)	strText.Format("              내진구역 : Ⅰ구역  ⇒  지진구역계수 : %.3f", dZoneCoefficient);
	else				strText.Format("              내진구역 : Ⅱ구역  ⇒  지진구역계수 : %.3f", dZoneCoefficient);
	Arr.Add(strText+"");
	strText.Format("          A = %.3f × %.3f = %.3f", dRiskCoefficient, dZoneCoefficient, pInVBracing->m_strtHorLoad.m_dAcceleration);
	Arr.Add(strText+"");
	strText.Format("          - 단부 브레이싱에 작용하는 총지진하중");
	Arr.Add(strText+"");
	strText.Format("          ΣHe = A ΣRd = %.3f × %.3f = %.3f kN",
		pInVBracing->m_strtHorLoad.m_dAcceleration, tokN(pInVBracing->m_strtHorLoad.m_dReaction), tokN(pInVBracing->m_strtHorLoad.m_dTotEarthQuakeLoad));
	Arr.Add(strText+"");
	strText.Format("         - 지진하중에 의한 브레이싱 1조의 작용력");
	Arr.Add(strText+"");
	strText.Format("           지진시의 허용응력 할증계수 : 1.5");
	Arr.Add(strText+"");
	strText.Format("           He = 1/ (n-1)·ΣHe = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dEarthQuakeLoad));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtHorLoad.m_dHw>pInVBracing->m_strtHorLoad.m_dEarthQuakeLoad)
		strText.Format("           Hw  >  He  이므로   H = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dH));
	else
		strText.Format("           Hw  <  He  이므로   H = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dH));
	Arr.Add(strText+"");	Arr.Add("");
	strText.Format("    나) 부재력 계산");	Arr.Add(strText+"");
	strText.Format("     1) 상현재(AB)");	Arr.Add(strText+"");
	double	dCollCoeff	= 15.0/(40.0+toM(pInVBracing->m_pChordUp->m_dLength/2.0));
	if(dCollCoeff>0.3)
		strText.Format("        충격계수 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f > 0.3  ⇒ i = %.3f",
		toM(pInVBracing->m_pChordUp->m_dLength/2.0), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefUp);
	else
		strText.Format("        충격계수 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f < 0.3  ⇒ i = %.3f",
		toM(pInVBracing->m_pChordUp->m_dLength/2.0), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefUp);
	Arr.Add(strText+"");
	strText.Format("        수평하중 재하시");
	Arr.Add(strText+"");
	strText.Format("         N1  =  1/2 H = 1/2 × %.3f = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dH), tokN(pInVBracing->m_strtElementLoad.m_dN1));
	Arr.Add(strText+"");
	strText.Format("         연직 활하중 재하시 (①의 경우)");
	Arr.Add(strText+"");
	strText.Format("          Ml+i = Pr·(a/2)/4 × (1+i) = %.3f kN·m", tokNM(pInVBracing->m_strtElementLoad.m_dMl));
	Arr.Add(strText+"");
	strText.Format("     2) 사재(BD,BE)");
	Arr.Add(strText+"");
	dCollCoeff	= 15.0/(40.0+toM(pInVBracing->m_pDiagonal->m_dLength));
	if(dCollCoeff>0.3)
		strText.Format("        충격계수 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f > 0.3  ⇒ i = %.3f",
		toM(pInVBracing->m_pDiagonal->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefDiagonal);
	else
		strText.Format("        충격계수 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f < 0.3  ⇒ i = %.3f",
		toM(pInVBracing->m_pDiagonal->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefDiagonal);
	Arr.Add(strText+"");
	strText.Format("        N2 = 1/4 H·secΘ     ( 수평하중만 작용시, BD부재 인장력 최대)");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN2Hor));
	Arr.Add(strText+"");
	strText.Format("        N2 = (Pd + PrΣy(1+i))/(2·sinΘ) + 1/4 H·secΘ    ( 연직+수평하중 작용시, BE부재 압축력 최대 )");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN2VerHor));
	Arr.Add(strText+"");
	strText.Format("     3) 하현재(DE)");
	Arr.Add(strText+"");
	dCollCoeff	= 15.0/(40.0+toM(pInVBracing->m_pChordLo->m_dLength));
	if(dCollCoeff>0.3)
		strText.Format("        충격계수 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f > 0.3  ⇒ i = %.3f",
		toM(pInVBracing->m_pChordLo->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefLo);
	else
		strText.Format("        충격계수 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f < 0.3  ⇒ i = %.3f",
		toM(pInVBracing->m_pChordLo->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefLo);
	Arr.Add(strText+"");
	strText.Format("        N3 = (Pd + PrΣy·(1+i))/(2·tanΘ)     (연직하중만 작용시, 인장력 최대)");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN3Ver));
	Arr.Add(strText+"");
	strText.Format("        N3 = 3/4 H    (수평하중만 작용시, 압축력 최대)");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN3Hor));
	Arr.Add(strText+"");Arr.Add("");
	
	strText.Format("    다) 부재의 설계");	Arr.Add(strText+"");
	strText.Format("     1) 상 현 재    ( %s )", pInVBracing->m_strtChordUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        단면적                :     Ag = %.1f mm²", pInVBracing->m_pChordUp->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경의 최소값 :  γmin = %.1f mm", pInVBracing->m_pChordUp->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경          :    γx = %.1f mm", pInVBracing->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	if(pInVBracing->m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
		strText.Format("        단면 계수             :     Zx = %.1f mm³", pInVBracing->m_pChordUp->m_dZy);
	else
		strText.Format("        단면 계수             :     Zy = %.1f mm³", pInVBracing->m_pChordUp->m_dZy);
	Arr.Add(strText+"");
	strText.Format("          ① 세장비 검토");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordUp.m_dSlendernessRatio<150.0)
		strText.Format("            ℓ/γmin = %.1f / %.1f = %.3f  < 150      .... O.K  ( ☞ 도.설.기 3.4.1.4 )",
			pInVBracing->m_pChordUp->m_dLength, pInVBracing->m_pChordUp->m_dGammaMin, pInVBracing->m_strtChordUp.m_dSlendernessRatio);
	else
		strText.Format("            ℓ/γmin = %.1f / %.1f = %.3f  > 150      .... N.G  ( ☞ 도.설.기 3.4.1.4 )",
			pInVBracing->m_pChordUp->m_dLength, pInVBracing->m_pChordUp->m_dGammaMin, pInVBracing->m_strtChordUp.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          ② 허용응력     ( ☞ 도.설.기 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("           - 허용 축방향 압축응력");
	Arr.Add(strText+"");
	strText.Format("            ℓ/γx = %.1f / %.1f = %.3f", pInVBracing->m_pChordUp->m_dLength, pInVBracing->m_pChordUp->m_dGammaX,
		pInVBracing->m_pChordUp->m_dLength/pInVBracing->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("             fca = %.3f MPa", pInVBracing->m_strtChordUp.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여 검토한다.");
	Arr.Add(strText+"");
	strText.Format("             fca＇=  fca × ( 0.5 + ( ℓ/γx ) / 1000 )");
	Arr.Add(strText+"");
	strText.Format("                  = %.3f MPa", pInVBracing->m_strtChordUp.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("           - 허용 휨응력");
	Arr.Add(strText+"");
	strText.Format("             fa = %.1f MPa", pInVBracing->m_strtChordUp.m_dFa);
	Arr.Add(strText+"");
	strText.Format("          ③ 응력검토");
	Arr.Add(strText+"");
	strText.Format("              축방향압축응력");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordUp.m_dFc<pInVBracing->m_strtChordUp.m_dFcaApo)
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  <  fca'    .... O.K",
			pInVBracing->m_strtElementLoad.m_dN1, pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_strtChordUp.m_dFc);
	else
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  >  fca'    .... N.G",
			pInVBracing->m_strtElementLoad.m_dN1, pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_strtChordUp.m_dFc);
	Arr.Add(strText+"");
	strText.Format("              휨응력");
	Arr.Add(strText+"");
	
	if(pInVBracing->m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)	str.Format("              f = Ml+i / Zx = ");
	else																str.Format("              f = Ml+i / Zy = ");
	if(pInVBracing->m_strtChordUp.m_dF<pInVBracing->m_strtChordUp.m_dFa)
		strText.Format("%s%.3f / %.1f = %.3f  < fa    .... O.K", str,
		pInVBracing->m_strtElementLoad.m_dMl, pInVBracing->m_pChordUp->m_dZy, pInVBracing->m_strtChordUp.m_dF);
	else
		strText.Format("%s%.3f / %.1f = %.3f  > fa    .... N.G", str,
		pInVBracing->m_strtElementLoad.m_dMl, pInVBracing->m_pChordUp->m_dZy, pInVBracing->m_strtChordUp.m_dF);
	Arr.Add(strText+"");
	strText.Format("          ④ Gusset Plate와의 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접부의 허용응력 : υa = %.3f MPa", pInVBracing->m_strtChordUp.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접각장          : ΣL = %.1f × 2EA = %.3f mm", pInVBracing->m_strtChordUp.m_dLengthWeld/2, pInVBracing->m_strtChordUp.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접목두께        :  a  = %.1f / √2 = %.3f mm", pInVBracing->m_strtChordUp.m_dThroatDepth, pInVBracing->m_strtChordUp.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordUp.m_dFc<0.75*pInVBracing->m_strtChordUp.m_dFcaApo)
	{
		strText.Format("             작용응력이 모재 전강도에 75%%이하 이므로 모재강도의 75%%를 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N1' = 0.75 × %.1f × %.3f = %.3f kN",
			pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_strtChordUp.m_dFcaApo, tokN(pInVBracing->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             작용응력이 모재 전강도에 75%%이상 이므로 작용하중을 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N1' = N1 = %.3f kN", tokN(pInVBracing->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	if(pInVBracing->m_strtChordUp.m_dShear<pInVBracing->m_strtChordUp.m_dAllowShear)
		strText.Format("             υ = N1'/ a·ΣL = %.3f MPa  <  υa    .... O.K", pInVBracing->m_strtChordUp.m_dShear);
	else
		strText.Format("             υ = N1'/ a·ΣL = %.3f MPa  >  υa    .... N.G", pInVBracing->m_strtChordUp.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     2) 사 재    ( %s )", pInVBracing->m_strtDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        단면적                :    Ag = %.1f mm²", pInVBracing->m_pDiagonal->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경의 최소값 : γmin = %.1f mm", pInVBracing->m_pDiagonal->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경          :  γx  = %.1f mm", pInVBracing->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          ① 세장비 검토");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  <  150    .... O.K  ( ☞ 도.설.기 3.4.1.4 )",
			pInVBracing->m_pDiagonal->m_dLength, pInVBracing->m_pDiagonal->m_dGammaMin, pInVBracing->m_strtDiagonal.m_dSlendernessRatio);
	else
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  >  150    .... N.G  ( ☞ 도.설.기 3.4.1.4 )",
			pInVBracing->m_pDiagonal->m_dLength, pInVBracing->m_pDiagonal->m_dGammaMin, pInVBracing->m_strtDiagonal.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          ② 허용응력    ( ☞ 도.설.기 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - 허용압축응력");
	Arr.Add(strText+"");
	strText.Format("            ℓ/γx = %.1f / %.1f = %.3f",
		pInVBracing->m_pDiagonal->m_dLength, pInVBracing->m_pDiagonal->m_dGammaX, pInVBracing->m_pDiagonal->m_dLength/pInVBracing->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pInVBracing->m_strtDiagonal.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여 검토한다.");
	Arr.Add(strText+"");
	strText.Format("            fca＇=  fca × ( 0.5 + ( ℓ/γx ) / 1000 ) = %.1f MPa", pInVBracing->m_strtDiagonal.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("            - 허용인장응력");
	Arr.Add(strText+"");
	strText.Format("            fta = %.1f MPa", pInVBracing->m_strtDiagonal.m_dFta);
	Arr.Add(strText+"");
	strText.Format("          ③ 응력검토");
	Arr.Add(strText+"");
	if(pInVBracing->m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		strText.Format("            인장 L형강의 유효단면적     (도설 3.4.5)");
	else
		strText.Format("            인장 T형강의 유효단면적     (도설 3.4.5)");
	Arr.Add(strText+"");
	strText.Format("            An = %.1f mm²", pInVBracing->m_strtDiagonal.m_dAn);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtDiagonal.m_dFt<pInVBracing->m_strtDiagonal.m_dFta)
		strText.Format("            ft = N2 / An = %.3f MPa  <  fta    .... O.K", pInVBracing->m_strtDiagonal.m_dFt);
	else
		strText.Format("            ft = N2 / An = %.3f MPa  >  fta    .... N.G", pInVBracing->m_strtDiagonal.m_dFt);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtDiagonal.m_dFc<pInVBracing->m_strtDiagonal.m_dFcaApo)
		strText.Format("            fc = N2 / Ag = %.3f MPa  <  fca'    .... O.K", pInVBracing->m_strtDiagonal.m_dFc);
	else
		strText.Format("            fc = N2 / Ag = %.3f MPa  >  fca'    .... N.G", pInVBracing->m_strtDiagonal.m_dFc);
	Arr.Add(strText+"");
	strText.Format("          ④ Gusset Plate와의 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접부의 허용응력 : υa = %.1f MPa ( ☞ 도.설.기 3.3.2.3 )", pInVBracing->m_strtDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접각장          : ΣL = %.1f × 2EA = %.1f mm", pInVBracing->m_strtDiagonal.m_dLengthWeld/2.0, pInVBracing->m_strtDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접목두께        :  a  = %.1f / √2 = %.1f mm", pInVBracing->m_strtDiagonal.m_dThroatDepth, pInVBracing->m_strtDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtDiagonal.m_dFc<0.75*pInVBracing->m_strtDiagonal.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 이므로", pInVBracing->m_strtDiagonal.m_dFc, 0.75*pInVBracing->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이하 이므로 모재강도의 75%%를 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N2' = 0.75 × %.1f × %.1f = %.3f kN",
			pInVBracing->m_pDiagonal->m_dArea, pInVBracing->m_strtDiagonal.m_dFcaApo, tokN(pInVBracing->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 이므로", pInVBracing->m_strtDiagonal.m_dFc, 0.75*pInVBracing->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이상 이므로 작용하중을 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N2' = N2 = %.3f kN", tokN(pInVBracing->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	if(pInVBracing->m_strtDiagonal.m_dShear<pInVBracing->m_strtDiagonal.m_dAllowShear)
		strText.Format("             υ = N2'/ a·ΣL = %.1f MPa  <  υa    .... O.K", pInVBracing->m_strtDiagonal.m_dShear);
	else
		strText.Format("             υ = N2'/ a·ΣL = %.1f MPa  >  υa    .... N.G", pInVBracing->m_strtDiagonal.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     3) 하 현 재    ( %s )", pInVBracing->m_strtChordLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        단면적                :    Ag = %.1f mm²", pInVBracing->m_pChordLo->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경의 최소값 : γmin = %.1f mm", pInVBracing->m_pChordLo->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경          :  γx  = %.1f mm", pInVBracing->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          ① 세장비 검토");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  <  150    .... O.K  ( ☞ 도.설.기 3.4.1.4 )",
			pInVBracing->m_pChordLo->m_dLength, pInVBracing->m_pChordLo->m_dGammaMin, pInVBracing->m_strtChordLo.m_dSlendernessRatio);
	else
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  >  150    .... N.G  ( ☞ 도.설.기 3.4.1.4 )",
			pInVBracing->m_pChordLo->m_dLength, pInVBracing->m_pChordLo->m_dGammaMin, pInVBracing->m_strtChordLo.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          ② 허용응력    ( ☞ 도.설.기 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - 허용압축응력");
	Arr.Add(strText+"");
	strText.Format("            ℓ/γx = %.1f / %.1f = %.3f",
		pInVBracing->m_pChordLo->m_dLength, pInVBracing->m_pChordLo->m_dGammaX, pInVBracing->m_pChordLo->m_dLength/pInVBracing->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pInVBracing->m_strtChordLo.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여 검토한다.");
	Arr.Add(strText+"");
	strText.Format("            fca＇=  fca × ( 0.5 + ( ℓ/γx ) / 1000 ) = %.1f MPa", pInVBracing->m_strtChordLo.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("            - 허용인장응력");
	Arr.Add(strText+"");
	strText.Format("            fta = %.1f MPa", pInVBracing->m_strtChordLo.m_dFta);
	Arr.Add(strText+"");
	strText.Format("          ③ 응력검토");
	Arr.Add(strText+"");
	if(pInVBracing->m_pChordLo->m_nType==CSteelMaterialData::CTSTEEL)
		strText.Format("            인장 L형강의 유효단면적     (도설 3.4.5)");
	else
		strText.Format("            인장 T형강의 유효단면적     (도설 3.4.5)");
	Arr.Add(strText+"");
	strText.Format("                An = %.1f mm²", pInVBracing->m_strtChordLo.m_dAn);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dFt<pInVBracing->m_strtChordLo.m_dFta)
		strText.Format("                ft = N3 / An = %.1f / %.1f = %.3f MPa  <  fta    .... O.K",
		pInVBracing->m_strtElementLoad.m_dN3Ver, pInVBracing->m_strtChordLo.m_dAn, pInVBracing->m_strtChordLo.m_dFt);
	else
		strText.Format("                ft = N3 / An = %.1f / %.1f = %.3f MPa  >  fta    .... N.G",
		pInVBracing->m_strtElementLoad.m_dN3Ver, pInVBracing->m_strtChordLo.m_dAn, pInVBracing->m_strtChordLo.m_dFt);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dFc<pInVBracing->m_strtChordLo.m_dFcaApo)
		strText.Format("                fc = N3 / An = %.1f / %.1f = %.3f MPa  <  fca'    .... O.K",
		pInVBracing->m_strtElementLoad.m_dN3Hor, pInVBracing->m_pChordLo->m_dArea, pInVBracing->m_strtChordLo.m_dFc);
	else
		strText.Format("                fc = N3 / An = %.1f / %.1f = %.3f MPa  >  fca'    .... N.G",
		pInVBracing->m_strtElementLoad.m_dN3Hor, pInVBracing->m_pChordLo->m_dArea, pInVBracing->m_strtChordLo.m_dFc);
	Arr.Add(strText+"");
	strText.Format("          ④ Gusset Plate와의 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접부의 허용응력 : υa = %.1f MPa    ( ☞ 도.설.기 3.3.2.3 )", pInVBracing->m_strtChordLo.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접각장          : ΣL = %.1f × 2EA = %.1f mm", pInVBracing->m_strtChordLo.m_dLengthWeld/2, pInVBracing->m_strtChordLo.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접목두께        :   a = %.1f / √2 = %.1f mm", pInVBracing->m_strtChordLo.m_dThroatDepth, pInVBracing->m_strtChordLo.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dFc<0.75*pInVBracing->m_strtChordLo.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 이므로", pInVBracing->m_strtChordLo.m_dFc, 0.75*pInVBracing->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이하 이므로 모재강도의 75%%를 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N3' = 0.75 × %.1f × %.1f = %.3f kN",
			pInVBracing->m_pChordLo->m_dArea, pInVBracing->m_strtChordLo.m_dFcaApo, tokN(pInVBracing->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 이므로", pInVBracing->m_strtChordLo.m_dFc, 0.75*pInVBracing->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이상 이므로 작용하중을 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N3' = N3 = %.3f kN", tokN(pInVBracing->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	if(pInVBracing->m_strtChordLo.m_dShear<pInVBracing->m_strtChordLo.m_dAllowShear)
		strText.Format("             υ = N3'/ a·ΣL = %.1f MPa  <  υa    .... O.K", pInVBracing->m_strtChordLo.m_dShear);
	else
		strText.Format("             υ = N3'/ a·ΣL = %.1f MPa  >  υa    .... N.G", pInVBracing->m_strtChordLo.m_dShear);
	Arr.Add(strText+"");
	strText.Format("    다) Gusset Plate의 검토");
	Arr.Add(strText+"");
	strText.Format("        작용하중과 모재강도의 75%%를 고려하여 설계한다.");
	Arr.Add(strText+"");
	strText.Format("      1) 상현재/사재 연결 (A,B)");
	Arr.Add(strText+"");
	strText.Format("       · 사용강재           : %.0f × %.0f × %.0f ( %s )",
		pInVBracing->m_strtGussetUp.m_dHeight, pInVBracing->m_strtGussetUp.m_dThick, pInVBracing->m_strtGussetUp.m_dWidth, pInVBracing->m_strtGussetUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       · 강재의 허용응력    : fa = %.1f MPa", pInVBracing->m_strtGussetUp.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       · 작용하중");
	Arr.Add(strText+"");
	strText.Format("          P = N1 = %.3f kN", tokN(pInVBracing->m_strtGussetUp.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       · 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1면마찰)   ρa = %.1f kN", pInVBracing->m_strtGussetUp.m_strBoltMaterial, pInVBracing->m_strtGussetUp.m_dDiaBolt, tokN(pInVBracing->m_strtGussetUp.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          필요 BOLT수");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetUp.m_dQtyBoltUse>pInVBracing->m_strtGussetUp.m_dQtyBoltReq)
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... O.K",
		tokN(pInVBracing->m_strtGussetUp.m_dLoad), tokN(pInVBracing->m_strtGussetUp.m_dBoltAllowStress), pInVBracing->m_strtGussetUp.m_dQtyBoltReq, pInVBracing->m_strtGussetUp.m_dQtyBoltUse);
	else
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... N.G",
		tokN(pInVBracing->m_strtGussetUp.m_dLoad), tokN(pInVBracing->m_strtGussetUp.m_dBoltAllowStress), pInVBracing->m_strtGussetUp.m_dQtyBoltReq, pInVBracing->m_strtGussetUp.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("     · Gusset Plate의 응력검토");
	Arr.Add(strText+"");
	strText.Format("          An = ( %.0f - %.0f × %.0f ) × %.0f = %.1f mm²",
		pInVBracing->m_strtGussetUp.m_dHeight, pInVBracing->m_strtGussetUp.m_dQtyBoltUse, pInVBracing->m_strtGussetUp.m_dDiaBolt+3.0, pInVBracing->m_strtGussetUp.m_dThick, pInVBracing->m_strtGussetUp.m_dAn);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetUp.m_dStress<pInVBracing->m_strtGussetUp.m_dAllowStress)
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetUp.m_dLoad, pInVBracing->m_strtGussetUp.m_dAn, pInVBracing->m_strtGussetUp.m_dStress);
	else
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetUp.m_dLoad, pInVBracing->m_strtGussetUp.m_dAn, pInVBracing->m_strtGussetUp.m_dStress);
	Arr.Add(strText+"");
	strText.Format("      2) 하현재 연결 (C,E)");
	Arr.Add(strText+"");
	strText.Format("       · 사용강재            : %.0f × %.0f × %.0f ( %s )",
		pInVBracing->m_strtGussetDiagonal.m_dHeight, pInVBracing->m_strtGussetDiagonal.m_dThick, pInVBracing->m_strtGussetDiagonal.m_dWidth, pInVBracing->m_strtGussetDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       · 강재의 허용응력     : fa = %.1f MPa", pInVBracing->m_strtGussetDiagonal.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       · 강재의 허용전단응력 : υa = %.1f MPa", pInVBracing->m_strtGussetDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("       · 작용하중");
	Arr.Add(strText+"");
	strText.Format("          Ph = N1' = %.3f kN", tokN(pInVBracing->m_strtGussetDiagonal.m_dLoadHor));
	Arr.Add(strText+"");
	strText.Format("          Pv = ΣP = Pd + Pl·(1+i) = %.3f kN", tokN(pInVBracing->m_strtGussetDiagonal.m_dLoadVer));
	Arr.Add(strText+"");
	strText.Format("          P = √( Ph2 + Pv2 ) = %.3f kN", tokN(pInVBracing->m_strtGussetDiagonal.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       · 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("          Fillet 용접부의 허용응력 : υa = %.3f MPa", pInVBracing->m_strtGussetDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("          Fillet 용접각장          : ΣL = %.1f × 2EA = %.1f mm", pInVBracing->m_strtGussetDiagonal.m_dLengthWeld/2.0, pInVBracing->m_strtGussetDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("          Fillet 용접목두께        :  a  = %.1f / √2 = %.3f mm", pInVBracing->m_strtGussetDiagonal.m_dThroatDepth, pInVBracing->m_strtGussetDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetDiagonal.m_dShearWeld<pInVBracing->m_strtGussetDiagonal.m_dAllowShearWeld)
		strText.Format("          υ = P/ a·ΣL = %.3f MPa  <  υa    .... O.K", pInVBracing->m_strtGussetDiagonal.m_dShearWeld);
	else
		strText.Format("          υ = P/ a·ΣL = %.3f MPa  >  υa    .... N.G", pInVBracing->m_strtGussetDiagonal.m_dShearWeld);
	Arr.Add(strText+"");
	strText.Format("       · Gusset Plate의 응력검토");
	Arr.Add(strText+"");
	strText.Format("          Agv = %.1f mm²    Agh = %.1f mm²", pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dAgh);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetDiagonal.m_dStress<pInVBracing->m_strtGussetDiagonal.m_dAllowStress)
		strText.Format("         f = Pv / Agv = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetDiagonal.m_dLoadVer, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dStress);
	else
		strText.Format("         f = Pv / Agv = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetDiagonal.m_dLoadVer, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dStress);
	Arr.Add(strText+"");

	if(pInVBracing->m_strtGussetDiagonal.m_dShearGusset<pInVBracing->m_strtGussetDiagonal.m_dAllowShear)
		strText.Format("         υ = Ph / Agh = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetDiagonal.m_dLoadHor, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dShearGusset);
	else
		strText.Format("         υ = Ph / Agh = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetDiagonal.m_dLoadHor, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dShearGusset);
	Arr.Add(strText+"");

	strText.Format("      3) 사재/하현재 연결 (D)");
	Arr.Add(strText+"");
	strText.Format("       · 사용강재           : %.0f × %.0f × %.0f ( %s )",
		pInVBracing->m_strtGussetLo.m_dHeight, pInVBracing->m_strtGussetLo.m_dThick, pInVBracing->m_strtGussetLo.m_dWidth, pInVBracing->m_strtGussetLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       · 강재의 허용응력    : fa = %.1f MPa", pInVBracing->m_strtGussetLo.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       · 작용하중");
	Arr.Add(strText+"");
	strText.Format("          Rh = N3' + N2'cosΘ = %.3f kN", tokN(pInVBracing->m_strtGussetLo.m_dRh));
	Arr.Add(strText+"");
	strText.Format("          Rv = N2'sinΘ kN", tokN(pInVBracing->m_strtGussetLo.m_dRv));
	Arr.Add(strText+"");
	strText.Format("          P = √( Rh2 + Rv2 ) = %.3f kN", tokN(pInVBracing->m_strtGussetLo.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       · 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1면마찰)   ρa = %.1f kN", pInVBracing->m_strtGussetLo.m_strBoltMaterial, pInVBracing->m_strtGussetLo.m_dDiaBolt, tokN(pInVBracing->m_strtGussetLo.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          필요 BOLT수");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetLo.m_dQtyBoltUse>pInVBracing->m_strtGussetLo.m_dQtyBoltReq)
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... O.K",
		tokN(pInVBracing->m_strtGussetLo.m_dLoad), tokN(pInVBracing->m_strtGussetLo.m_dBoltAllowStress), pInVBracing->m_strtGussetLo.m_dQtyBoltReq, pInVBracing->m_strtGussetLo.m_dQtyBoltUse);
	else
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... N.G",
		tokN(pInVBracing->m_strtGussetLo.m_dLoad), tokN(pInVBracing->m_strtGussetLo.m_dBoltAllowStress), pInVBracing->m_strtGussetLo.m_dQtyBoltReq, pInVBracing->m_strtGussetLo.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("       · Gusset Plate의 응력검토");
	Arr.Add(strText+"");
	strText.Format("           An = %.0f × %.0f = %.1f mm²", pInVBracing->m_strtGussetLo.m_dWidth, pInVBracing->m_strtGussetLo.m_dThick, pInVBracing->m_strtGussetLo.m_dAn);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetLo.m_dStress<pInVBracing->m_strtGussetLo.m_dAllowStress)
		strText.Format("           f = P / An = %.3f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetLo.m_dStress);
	else
		strText.Format("           f = P / An = %.3f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetLo.m_dStress);
	Arr.Add(strText+"");Arr.Add("");
}

// 중간 가로보(H형강타입, 제작타입)
void CPlateStressOut::StressCheckCenterCrossBeamGeneralType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map)
{
	CDesignCrossBeam		CrossBeam(m_pStd->m_pCalcStd, pBx);
	CDesignCenterCrossBeam	*pCenCr		= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();

	CString	strText		= _T("");
	long	nG			= pBx->GetNumberGirder();
	double	dStation	= pBx->GetStation();

	Arr.Add("");
	strText.Format("    < 중간 가로보 - 거더 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");
	strText.Format("                                 사용강재 : %s", pCenCr->m_pJewonFlangeUp->m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("                                 플랜지폭 : bfu = %.0f mm(상부)    bfl = %.0f mm(하부)", pCenCr->m_pJewonFlangeUp->m_dWidth, pCenCr->m_pJewonFlangeLo->m_dWidth);
	Arr.Add(strText+"");
	strText.Format("                                 복부의폭 : bw = %.0f mm", pCenCr->m_pJewonWeb->m_dHeight);
	Arr.Add(strText+"");
	strText.Format("                                 수평보강재 사용단수 %d 단", pCenCr->m_nQtyHorStiff);
	Arr.Add(strText+"");
	if(pCenCr->m_pJewonWeb->m_dWidth>=pCenCr->m_strtMinThick.m_dThickWebReq)
		strText.Format("                                 tw = %.0f mm  ≥  twmin = %.1f mm    .... O.K",
			pCenCr->m_pJewonWeb->m_dWidth, pCenCr->m_strtMinThick.m_dThickWebReq);
	else
		strText.Format("                                 tw = %.0f mm  <  twmin = %.1f mm    .... N.G",
			pCenCr->m_pJewonWeb->m_dWidth, pCenCr->m_strtMinThick.m_dThickWebReq);
	Arr.Add(strText+"");
	strText.Format("                                 tfu(min) = bfu' / 16 = %.1f / 16 = %.2f mm",
		(pCenCr->m_pJewonFlangeUp->m_dWidth-pCenCr->m_pJewonWeb->m_dWidth)/2.0, pCenCr->m_strtMinThick.m_dMinThickFlangeUp);
	Arr.Add(strText+"");
	strText.Format("                                 tfl(min) = bfl' / 16 = %.1f / 16 = %.2f mm",
		(pCenCr->m_pJewonFlangeLo->m_dWidth-pCenCr->m_pJewonWeb->m_dWidth)/2.0, pCenCr->m_strtMinThick.m_dMinThickFlangeLo);
	Arr.Add(strText+"");
	if(pCenCr->m_pJewonFlangeUp->m_dHeight>pCenCr->m_strtMinThick.m_dMinThickFlangeUp)
		strText.Format("                                 상판두께 tfu = %.0f mm  >  tfu(mim)    .... O.K", pCenCr->m_pJewonFlangeUp->m_dHeight);
	else
		strText.Format("                                 상판두께 tfu = %.0f mm  <  tfu(mim)    .... N.G", pCenCr->m_pJewonFlangeUp->m_dHeight);
	Arr.Add(strText+"");
	if(pCenCr->m_pJewonFlangeLo->m_dHeight>pCenCr->m_strtMinThick.m_dMinThickFlangeLo)
		strText.Format("                                 하판두께 tfl = %.0f mm  >  tfl(mim)    .... O.K", pCenCr->m_pJewonFlangeLo->m_dHeight);
	else
		strText.Format("                                 하판두께 tfl = %.0f mm  <  tfl(mim)    .... N.G", pCenCr->m_pJewonFlangeLo->m_dHeight);
	Arr.Add(strText+"");
	strText.Format("                                 주형간격 a = %.3f m", toM(pCenCr->m_dGirderSpace));
	Arr.Add(strText+"");
	strText.Format("    가. FRAME 해석결과집계");
	Arr.Add(strText+"");
	strText.Format("        하중분배작용에 대한 영향을 반영하기 위하여 FRAME 해석결과를 고려한다.");	
	Arr.Add(strText+"");
	strText.Format("       ┌────────┬───────────┬───────────┬───────────┐");	Arr.Add(strText+"");
	strText.Format("       │                │        M (kN.m)      │       S (kN)         │        N (kN)        │");	Arr.Add(strText+"");
	strText.Format("       │    구    분    ├─────┬─────┼─────┬─────┼─────┬─────┤");	Arr.Add(strText+"");
	strText.Format("       │                │    Max   │    Min   │    Max   │    Min   │    Max   │    Min   │");	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┴─────┼─────┴─────┼─────┴─────┤");	Arr.Add(strText+"");
	strText.Format("       │ 합성전 고정하중│  %12.3f        │  %12.3f        │  %12.3f        │",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dPreDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dPreDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       ├────────┼───────────┼───────────┼───────────┤");	Arr.Add(strText+"");
	strText.Format("       │ 합성후 고정하중│  %12.3f        │  %12.3f        │  %12.3f        │",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dPostDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dPostDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┬─────┼─────┬─────┼─────┬─────┤");	Arr.Add(strText+"");
	strText.Format("       │  활   하   중  │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMaxLiveLoad), tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMinLiveLoad),
		tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad),
		tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMaxLiveLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMinLiveLoad));	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │  지 점 침 하   │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMaxSettlement), tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMinSettlement),
		tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMinSettlement),
		tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMaxSettlement), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMinSettlement));	Arr.Add(strText+"");
	strText.Format("       ├────────┼─────┼─────┼─────┼─────┼─────┼─────┤");					Arr.Add(strText+"");
	strText.Format("       │     합    계   │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │ %8.3f │",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), tokNM(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment),
		tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinShear),
		tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxAxial), tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinAxial));	Arr.Add(strText+"");
	strText.Format("       └────────┴─────┴─────┴─────┴─────┴─────┴─────┘");				Arr.Add(strText+"");

	strText.Format("    나. 단면 설계");
	Arr.Add(strText+"");
	strText.Format("          ┌──────────┬─────┬─────┬─────┬──────┬──────┐");	Arr.Add(strText+"");
	strText.Format("          │     구      분     │  A(mm²) │  Y (mm)  │ AY (mm³)│ AY²(mm⁴) │  Io(mm⁴)  │");	Arr.Add(strText+"");
	strText.Format("          ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("          │1 - UFLG-%4.0f × %2.0f │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pCenCr->m_pJewonFlangeUp->m_dWidth, pCenCr->m_pJewonFlangeUp->m_dHeight, pCenCr->m_pJewonFlangeUp->m_dA,
		pCenCr->m_pJewonFlangeUp->m_dY,	pCenCr->m_pJewonFlangeUp->m_dAY, pCenCr->m_pJewonFlangeUp->m_dAY2, pCenCr->m_pJewonFlangeUp->m_dIo);Arr.Add(strText+"");
	strText.Format("          ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("          │1 - WEB -%4.0f × %2.0f │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pCenCr->m_pJewonWeb->m_dHeight, pCenCr->m_pJewonWeb->m_dWidth, pCenCr->m_pJewonWeb->m_dA,
		pCenCr->m_pJewonWeb->m_dY, pCenCr->m_pJewonWeb->m_dAY, pCenCr->m_pJewonWeb->m_dAY2, pCenCr->m_pJewonWeb->m_dIo);	Arr.Add(strText+"");
	strText.Format("          ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("          │1 - LFLG-%4.0f × %2.0f │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pCenCr->m_pJewonFlangeLo->m_dWidth, pCenCr->m_pJewonFlangeLo->m_dHeight, pCenCr->m_pJewonFlangeLo->m_dA,
		pCenCr->m_pJewonFlangeLo->m_dY,	pCenCr->m_pJewonFlangeLo->m_dAY, pCenCr->m_pJewonFlangeLo->m_dAY2, pCenCr->m_pJewonFlangeLo->m_dIo);Arr.Add(strText+"");
	strText.Format("          ├──────────┼─────┼─────┼─────┼──────┼──────┤");	Arr.Add(strText+"");
	strText.Format("          │      합      계    │ %8.0f │ %8.0f │ %8.0f │ %10.0f │ %10.0f │",
		pCenCr->GetTotSectionArea(), 0.0, pCenCr->GetTotSectionAY(), pCenCr->GetTotSectionAY2(), pCenCr->GetTotSectionIo());Arr.Add(strText+"");
	strText.Format("          └──────────┴─────┴─────┴─────┴──────┴──────┘");	Arr.Add(strText+"");
	strText.Format("      δ = ΣA·Y / ΣA = %.3f mm", pCenCr->m_strtSection.m_dDelta);									Arr.Add(strText+"");
	strText.Format("       I = ΣIo + ΣA·Y² - ΣA·δ² = %.3f mm⁴", pCenCr->m_strtSection.m_dI);						Arr.Add(strText+"");
	strText.Format("     Ysu =  bw / 2 + tfu - δ = %.3f mm", pCenCr->m_strtSection.m_dYsu);								Arr.Add(strText+"");
	strText.Format("     Ysl =  bw / 2 + tfl + δ = %.3f mm", pCenCr->m_strtSection.m_dYsl);								Arr.Add(strText+"");
	strText.Format("     압축부의 국부좌굴에 대한 허용응력은  [도.설 표3.3.4(b), 표 3.4.4]");								Arr.Add(strText+"");
	strText.Format("      ∴ Fca = %.3f MPa,    σta = %.3f MPa,    υa = %.3f MPa",
		pCenCr->m_strtSection.m_dFca, pCenCr->m_strtSection.m_dSigmata, pCenCr->m_strtSection.m_dSheara);					Arr.Add(strText+"");
	if(pCenCr->m_strtSection.m_dFu<pCenCr->m_strtSection.m_dFca)
		strText.Format("      fu = M·Ysu / I - Nmin / Ag = %.3f MPa  <  fca = %.3f MPa    .... O.K", pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca);
	else
		strText.Format("      fu = M·Ysu / I - Nmin / Ag = %.3f MPa  >  fca = %.3f MPa    .... N.G", pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca);
	Arr.Add(strText+"");
	if(pCenCr->m_strtSection.m_dFl<pCenCr->m_strtSection.m_dSigmata)
		strText.Format("      fl = M·Ysl / I + Nmin / Ag = %.3f MPa  <  fta = %.3f MPa    .... O.K", pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata);
	else
		strText.Format("      fl = M·Ysl / I + Nmin / Ag = %.3f MPa  >  fta = %.3f MPa    .... N.G", pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata);
	Arr.Add(strText+"");
	if(pCenCr->m_strtSection.m_dShear<pCenCr->m_strtSection.m_dSheara)
		strText.Format("      υ = S / Aw = %.3f MPa  <  υa = %.3f MPa    .... O.K", pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara);
	else
		strText.Format("      υ = S / Aw = %.3f MPa  >  υa = %.3f MPa    .... N.G", pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara);
	Arr.Add(strText+"");

	strText.Format("      합성응력의 검토");
	Arr.Add(strText+"");
	if(fabs(pCenCr->m_strtSection.m_dFu/pCenCr->m_strtSection.m_dFca)>fabs(pCenCr->m_strtSection.m_dFl/pCenCr->m_strtSection.m_dSigmata))
	{
		if(pCenCr->m_strtSection.m_dCompStress<1.2)
		{
			strText.Format("      ( %.3f / %.3f )² + ( %.3f / %.3f )² = %.3f < 1.2    .... O.K",
			pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
		else
		{
			strText.Format("      ( %.3f / %.3f )² + ( %.3f / %.3f )² = %.3f < 1.2    .... N.G",
			pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
	}
	else
	{
		if(pCenCr->m_strtSection.m_dCompStress<1.2)
		{
			strText.Format("      ( %.3f / %.3f )² + ( %.3f / %.3f )² = %.3f < 1.2    .... O.K",
			pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
		else
		{
			strText.Format("      ( %.3f / %.3f )² + ( %.3f / %.3f )² = %.3f < 1.2    .... N.G",
			pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
	}
	Arr.Add(strText+"");
	CCrossBeamVerStiff	*pVerStiff	= pCenCr->m_pVerStiff;
	CCrossBeamHorStiff	*pHorStiff	= pCenCr->m_pHorStiff;
	strText.Format("         수직보강재를 생략할 수 있는 플랜지 순간격의 최대치 [도.설 3.8.5.1]");		Arr.Add(strText+"");
	if(pVerStiff->m_dSpaceCoeff>1.2)
		strText.Format("          √(υa/υ) = √( %.3f / %.3f ) = %.3f > 1.2  ⇒ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear,	pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	else
		strText.Format("          √(υa/υ) = √( %.3f / %.3f ) = %.3f < 1.2  ⇒ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear, pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	Arr.Add(strText+"");
	if(pVerStiff->m_dMaxSpace<pVerStiff->m_dHeightWeb)
		strText.Format("          %.0f × tw = %.1f mm  <  bw = %.0f mm  수직보강재 필요", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	else
		strText.Format("          %.0f × tw = %.1f mm  >  bw = %.0f mm  수직보강재 필요없음", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	Arr.Add(strText+"");

	if(pVerStiff->m_bIsNeedVerStiff)	VerStiff(Arr, pVerStiff, pCenCr->m_nQtyHorStiff);
	if(pCenCr->m_nQtyHorStiff>0)		HorStiff(Arr, pHorStiff);
}

// 중간 가로보(V브레이싱형 타입)
void CPlateStressOut::StressCheckCenterCrossBeamVBracingType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map)
{
	CPlateBridgeApp			*pDB		= m_pStd->GetBridge();
	CDesignCrossBeam		CrossBeam(m_pStd->m_pCalcStd, pBx);
	CDesignVBracing			*pVBr		= (CDesignVBracing*)CrossBeam.GetDesignCrossBeam();
	CGlobarOption			*pOpt		= m_pStd->GetDataManage()->GetGlobalOption();

	CString	strText		= _T("");
	long	nG			= pBx->GetNumberGirder();
	double	dStation	= pBx->GetStation();
	long	nGradeEarth			= pOpt->GetIdxDegreeEarthQuake();
	long	nRegionEarth		= pOpt->GetIdxRegionEarthQuake();
	double	dRiskCoefficient	= pOpt->GetRiskCoefficient();
	double	dZoneCoefficient	= pOpt->GetZoneCoefficient();
	
	Arr.Add("");
	strText.Format("    < 중간 수직브레이싱(V형) - 거더 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");
	strText.Format("    ·h = %.1f mm    ·a = %.1f mm    ·θ = %.3f°", pVBr->m_dHeight, pVBr->m_dGirderSpace, ToDegree(pVBr->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("     ·사 용 강 재");
	Arr.Add(strText+"");
	strText.Format("       ┌──────┬──────────┬─────┬─────┬─────┬─────┬─────┐");	Arr.Add(strText+"");
	strText.Format("       │   부   재  │    강 재 규 격     │ A (mm²) │ γx (mm) │ γy (mm) │  γmin   │ ℓ (mm)  │");	Arr.Add(strText+"");
	strText.Format("       ├──────┼──────────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │   상현재   │ L - %3.0f X %3.0f X %2.0f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │", pVBr->m_pChordUp->m_dHeight, pVBr->m_pChordUp->m_dWidth,
		pVBr->m_pChordUp->m_dThick1, pVBr->m_pChordUp->m_dArea, pVBr->m_pChordUp->m_dGammaX, pVBr->m_pChordUp->m_dGammaY, pVBr->m_pChordUp->m_dGammaMin, pVBr->m_pChordUp->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       ├──────┼──────────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │   사  재   │ L - %3.0f X %3.0f X %2.0f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │", pVBr->m_pDiagonal->m_dHeight, pVBr->m_pDiagonal->m_dWidth,
		pVBr->m_pChordUp->m_dThick1, pVBr->m_pDiagonal->m_dArea, pVBr->m_pDiagonal->m_dGammaX, pVBr->m_pDiagonal->m_dGammaY, pVBr->m_pDiagonal->m_dGammaMin, pVBr->m_pDiagonal->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       ├──────┼──────────┼─────┼─────┼─────┼─────┼─────┤");	Arr.Add(strText+"");
	strText.Format("       │   하현재   │ L - %3.0f X %3.0f X %2.0f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │ %8.1f │", pVBr->m_pChordLo->m_dHeight, pVBr->m_pChordLo->m_dWidth,
		pVBr->m_pChordUp->m_dThick1, pVBr->m_pChordLo->m_dArea, pVBr->m_pChordLo->m_dGammaX, pVBr->m_pChordLo->m_dGammaY, pVBr->m_pChordLo->m_dGammaMin, pVBr->m_pChordLo->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       └──────┴──────────┴─────┴─────┴─────┴─────┴─────┘");	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    가) 하 중 산 정");														Arr.Add(strText+"");
	strText.Format("        풍하중에 대하여는 2조의 중간 수직브레이싱이 받는 것으로 하며,");	Arr.Add(strText+"");
	strText.Format("        지진하중에 대하여는 주형간격수에 해당하는 수직브레이싱이 균등하게 부담하는 것으로 한다.");Arr.Add(strText+"");
	strText.Format("      1) 풍 하 중");		Arr.Add(strText+"");
	strText.Format("       - 풍하중 강도");		Arr.Add(strText+"");
	strText.Format("            ┌──────────┬──────────────┐");		Arr.Add(strText+"");
	strText.Format("            │     단 면 형 상    │        풍 하 중 (kN/m)     │");		Arr.Add(strText+"");
	strText.Format("            ├──────────┼──────────────┤");		Arr.Add(strText+"");
	strText.Format("            │   1 ≤ B/D < 8     │ [ 4.0 - 0.2(B/D) ] D ≥ 6  │");		Arr.Add(strText+"");
	strText.Format("            ├──────────┼──────────────┤");		Arr.Add(strText+"");
	strText.Format("            │      8 ≤ B/D      │         2.4 D ≥ 6         │");		Arr.Add(strText+"");
	strText.Format("            └──────────┴──────────────┘");		Arr.Add(strText+"");
	strText.Format("       - 풍하중 강도 계산");												Arr.Add(strText+"");
	strText.Format("         교량 폭원 (B) = %.3f m", toM(pVBr->m_strtWind.m_dWidthBridge));	Arr.Add(strText+"");
	strText.Format("         교량 높이 (D) = %.3f m", toM(pVBr->m_strtWind.m_dHeightBridge));
	Arr.Add(strText+"");
	strText.Format("           B / D = %.3f / %.3f = %.3f m",
		toM(pVBr->m_strtWind.m_dWidthBridge), toM(pVBr->m_strtWind.m_dHeightBridge), pVBr->m_strtWind.m_dRateWidthHeight);
	Arr.Add(strText+"");
	if(pVBr->m_strtWind.m_dRateWidthHeight>=1 && pVBr->m_strtWind.m_dRateWidthHeight<8)
		strText.Format("            1 ≤ B / D < 8  이므로  Ww = [ 4.0 - 0.2 ( B / D ) ] D = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWw));
	else if(pVBr->m_strtWind.m_dRateWidthHeight>=8)
		strText.Format("              8 ≤ B / D   이므로  Ww = 2.4 ·D = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWw));
	else
		strText.Format("              재검토 요망");
	Arr.Add(strText+"");
	if(tokN(pVBr->m_strtWind.m_dWw)>=6.0)
		strText.Format("              Ww ≥ 6.0 kN/m    이므로 ∴ Ww = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWwUse));
	else
		strText.Format("              Ww < 6.0 kN/m    이므로 ∴ Ww = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWwUse));
	Arr.Add(strText+"");
	strText.Format("       - 풍하중에 의한 브레이싱 1조의 작용력");
	Arr.Add(strText+"");
	strText.Format("         브레이싱 간격 λ = %.3f m", toM(pVBr->m_strtWind.m_dBracingSpace));
	Arr.Add(strText+"");
	strText.Format("         거더수         n = %d", pDB->GetGirdersu());
	Arr.Add(strText+"");
	strText.Format("         풍하중 계산시 허용응력 할증계수 : %.1f", pVBr->m_strtWind.m_dAllowCoeff);
	Arr.Add(strText+"");
	if(pDB->GetGirdersu()>=3)
		strText.Format("         Hw = 1/2·Ww·λ / 1.2 = 1/2 × %.3f × %.3f / %.1f = %.3f kN",
		tokNPM(pVBr->m_strtWind.m_dWwUse), toM(pVBr->m_strtWind.m_dBracingSpace), pVBr->m_strtWind.m_dAllowCoeff, tokN(pVBr->m_strtWind.m_dHw));
	else
		strText.Format("         Hw = Ww·λ / 1.2 = %.3f × %.3f / %.1f = %.3f kN",
		tokNPM(pVBr->m_strtWind.m_dWwUse), toM(pVBr->m_strtWind.m_dBracingSpace), pVBr->m_strtWind.m_dAllowCoeff, tokN(pVBr->m_strtWind.m_dHw));
	Arr.Add(strText+"");
	strText.Format("      2) 지진 하중");			Arr.Add(strText+"");
	strText.Format("       - 단위길이당 고정하중");	Arr.Add(strText+"");
	strText.Format("         Wd = 반력의 총합계 / 교량연장 = %.3f kN / %.3f m = %.3f kN/m",
		tokN(pVBr->m_strtEarthQuake.m_dReaction), toM(pVBr->m_strtEarthQuake.m_dLengthBridge), tokNPM(pVBr->m_strtEarthQuake.m_dWd));
	Arr.Add(strText+"");
	strText.Format("       - 지진 가속도 계수(A)");
	Arr.Add(strText+"");
	if(nGradeEarth==0)	strText.Format("         내진등급 : Ⅰ등급  ⇒  위험도 계수 : %.3f", dRiskCoefficient);
	else				strText.Format("         내진등급 : Ⅱ등급  ⇒  위험도 계수 : %.3f", dRiskCoefficient);
	Arr.Add(strText+"");
	if(nRegionEarth==0)	strText.Format("         내진구역 : Ⅰ구역  ⇒  지진구역계수 : %.3f", dZoneCoefficient);
	else				strText.Format("         내진구역 : Ⅱ구역  ⇒  지진구역계수 : %.3f", dZoneCoefficient);
	Arr.Add(strText+"");
	strText.Format("         A = %.3f × %.3f = %.3f", dRiskCoefficient, dZoneCoefficient, pVBr->m_strtEarthQuake.m_dAcceleration);
	Arr.Add(strText+"");
	strText.Format("       - 단위길이당 지진하중");
	Arr.Add(strText+"");
	strText.Format("         We = A × Wd = %.3f kN/m", tokNPM(pVBr->m_strtEarthQuake.m_dWe));
	Arr.Add(strText+"");
	strText.Format("       - 지진하중에 의한 브레이싱 1조의 작용력");
	Arr.Add(strText+"");
	strText.Format("         지진시의 허용응력 할증계수 : %.1f", pVBr->m_strtEarthQuake.m_dAllowCoeff);
	Arr.Add(strText+"");
	strText.Format("         He = 1/ (n-1)·We·λ = %.3f kN", tokN(pVBr->m_strtEarthQuake.m_dHe));
	Arr.Add(strText+"");
	if(pVBr->m_strtWind.m_dHw>pVBr->m_strtEarthQuake.m_dHe)
		strText.Format("         Hw  >  He 이므로   H = %.3f kN 적용", tokN(pVBr->m_strtEarthQuake.m_dH));
	else
		strText.Format("         Hw  <  He 이므로   H = %.3f kN 적용", tokN(pVBr->m_strtEarthQuake.m_dH));
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    나) 부재력 계산");
	Arr.Add(strText+"");
	strText.Format("      N1 = 3/4H    N2 = 1/4 H·secΘ    N3 = 1/2 H    secΘ = %.3f", 1.0/cos(pVBr->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("      1) 상현재(AB)");
	Arr.Add(strText+"");
	strText.Format("         N1 = -3/4H = %.3f kN (압축)", tokN(pVBr->m_strtElementLoad.m_dN1));
	Arr.Add(strText+"");
	strText.Format("      2) 사재(AD,BD)");
	Arr.Add(strText+"");
	strText.Format("         N2 = ± 1/4 H·secΘ = ± %.3f kN (AD:인장, BD:압축)", tokN(pVBr->m_strtElementLoad.m_dN2Tension));
	Arr.Add(strText+"");
	strText.Format("      3) 하현재(CD)");
	Arr.Add(strText+"");
	strText.Format("         N3 = - 1/2 H = %.3f kN (압축)", tokN(pVBr->m_strtElementLoad.m_dN3));
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    다) 부재력 계산");
	Arr.Add(strText+"");
	strText.Format("     1) 상 현 재    ( %s )", pVBr->m_strtChordUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        단면적                :     Ag = %.1f mm²", pVBr->m_pChordUp->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경의 최소값 :  γmin = %.1f mm", pVBr->m_pChordUp->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경          :    γx = %.1f mm", pVBr->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          ① 세장비 검토");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordUp.m_dSlendernessRatio<150.0)
		strText.Format("            ℓ/γmin = %.1f / %.1f = %.3f  < 150      .... O.K  ( ☞ 도.설.기 3.4.1.4 )",
			pVBr->m_pChordUp->m_dLength, pVBr->m_pChordUp->m_dGammaMin, pVBr->m_strtChordUp.m_dSlendernessRatio);
	else
		strText.Format("            ℓ/γmin = %.1f / %.1f = %.3f  > 150      .... N.G  ( ☞ 도.설.기 3.4.1.4 )",
			pVBr->m_pChordUp->m_dLength, pVBr->m_pChordUp->m_dGammaMin, pVBr->m_strtChordUp.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          ② 허용응력     ( ☞ 도.설.기 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("           - 허용 축방향 압축응력");
	Arr.Add(strText+"");
	strText.Format("            ℓ/γx = %.1f / %.1f = %.3f", pVBr->m_pChordUp->m_dLength, pVBr->m_pChordUp->m_dGammaX,
		pVBr->m_pChordUp->m_dLength/pVBr->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("             fca = %.3f MPa", pVBr->m_strtChordUp.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여 검토한다.");
	Arr.Add(strText+"");
	strText.Format("             fca＇=  fca × ( 0.5 + ( ℓ/γx ) / 1000 )");
	Arr.Add(strText+"");
	strText.Format("                  = %.3f MPa", pVBr->m_strtChordUp.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("          ③ 응력검토");
	Arr.Add(strText+"");
	strText.Format("              축방향압축응력");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordUp.m_dFc<pVBr->m_strtChordUp.m_dFcaApo)
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  <  fca'    .... O.K",
			pVBr->m_strtElementLoad.m_dN1, pVBr->m_pChordUp->m_dArea, pVBr->m_strtChordUp.m_dFc);
	else
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  >  fca'    .... N.G",
			pVBr->m_strtElementLoad.m_dN1, pVBr->m_pChordUp->m_dArea, pVBr->m_strtChordUp.m_dFc);
	Arr.Add(strText+"");
	strText.Format("          ④ Gusset Plate와의 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접부의 허용응력 : υa = %.3f MPa", pVBr->m_strtChordUp.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접각장          : ΣL = %.1f × 2EA = %.3f mm", pVBr->m_strtChordUp.m_dLengthWeld/2, pVBr->m_strtChordUp.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접목두께        :  a  = %.1f / √2 = %.3f mm", pVBr->m_strtChordUp.m_dThroatDepth, pVBr->m_strtChordUp.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtChordUp.m_dFc<0.75*pVBr->m_strtChordUp.m_dFcaApo)
	{
		strText.Format("             작용응력이 모재 전강도에 75%%이하 이므로 모재강도의 75%%를 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N1' = 0.75 × %.1f × %.3f = %.3f kN",
			pVBr->m_pChordUp->m_dArea, pVBr->m_strtChordUp.m_dFcaApo, tokN(pVBr->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             작용응력이 모재 전강도에 75%%이상 이므로 작용하중을 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N1' = N1 = %.3f kN", tokN(pVBr->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	if(pVBr->m_strtChordUp.m_dShear<pVBr->m_strtChordUp.m_dAllowShear)
		strText.Format("             υ = N1'/ a·ΣL = %.3f MPa  <  υa    .... O.K", pVBr->m_strtChordUp.m_dShear);
	else
		strText.Format("             υ = N1'/ a·ΣL = %.3f MPa  >  υa    .... N.G", pVBr->m_strtChordUp.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     2) 사 재    ( %s )", pVBr->m_strtDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        단면적                :    Ag = %.1f mm²", pVBr->m_pDiagonal->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경의 최소값 : γmin = %.1f mm", pVBr->m_pDiagonal->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경          :  γx  = %.1f mm", pVBr->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          ① 세장비 검토");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  <  150    .... O.K  ( ☞ 도.설.기 3.4.1.4 )",
			pVBr->m_pDiagonal->m_dLength, pVBr->m_pDiagonal->m_dGammaMin, pVBr->m_strtDiagonal.m_dSlendernessRatio);
	else
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  >  150    .... N.G  ( ☞ 도.설.기 3.4.1.4 )",
			pVBr->m_pDiagonal->m_dLength, pVBr->m_pDiagonal->m_dGammaMin, pVBr->m_strtDiagonal.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          ② 허용응력    ( ☞ 도.설.기 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - 허용압축응력");
	Arr.Add(strText+"");
	strText.Format("            ℓ/γx = %.1f / %.1f = %.3f",
		pVBr->m_pDiagonal->m_dLength, pVBr->m_pDiagonal->m_dGammaX, pVBr->m_pDiagonal->m_dLength/pVBr->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pVBr->m_strtDiagonal.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여 검토한다.");
	Arr.Add(strText+"");
	strText.Format("            fca＇=  fca × ( 0.5 + ( ℓ/γx ) / 1000 ) = %.1f MPa", pVBr->m_strtDiagonal.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("            - 허용인장응력");
	Arr.Add(strText+"");
	strText.Format("            fta = %.1f MPa", pVBr->m_strtDiagonal.m_dFta);
	Arr.Add(strText+"");
	strText.Format("          ③ 응력검토");
	Arr.Add(strText+"");
	if(pVBr->m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		strText.Format("            인장 L형강의 유효단면적     (도설 3.4.5)");
	else
		strText.Format("            인장 T형강의 유효단면적     (도설 3.4.5)");
	Arr.Add(strText+"");
	strText.Format("            An = %.1f mm²", pVBr->m_strtDiagonal.m_dAn);
	Arr.Add(strText+"");
	if(pVBr->m_strtDiagonal.m_dFt<pVBr->m_strtDiagonal.m_dFta)
		strText.Format("            ft = N2 / An = %.3f MPa  <  fta    .... O.K", pVBr->m_strtDiagonal.m_dFt);
	else
		strText.Format("            ft = N2 / An = %.3f MPa  >  fta    .... N.G", pVBr->m_strtDiagonal.m_dFt);
	Arr.Add(strText+"");
	if(pVBr->m_strtDiagonal.m_dFc<pVBr->m_strtDiagonal.m_dFcaApo)
		strText.Format("            fc = N2 / Ag = %.3f MPa  <  fca'    .... O.K", pVBr->m_strtDiagonal.m_dFc);
	else
		strText.Format("            fc = N2 / Ag = %.3f MPa  >  fca'    .... N.G", pVBr->m_strtDiagonal.m_dFc);
	Arr.Add(strText+"");
	strText.Format("          ④ Gusset Plate와의 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접부의 허용응력 : υa = %.1f MPa ( ☞ 도.설.기 3.3.2.3 )", pVBr->m_strtDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접각장          : ΣL = %.1f × 2EA = %.1f mm", pVBr->m_strtDiagonal.m_dLengthWeld/2.0, pVBr->m_strtDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접목두께        :  a  = %.1f / √2 = %.1f mm", pVBr->m_strtDiagonal.m_dThroatDepth, pVBr->m_strtDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtDiagonal.m_dFc<0.75*pVBr->m_strtDiagonal.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 이므로", pVBr->m_strtDiagonal.m_dFc, 0.75*pVBr->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이하 이므로 모재강도의 75%%를 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N2' = 0.75 × %.1f × %.1f = %.3f kN",
			pVBr->m_pDiagonal->m_dArea, pVBr->m_strtDiagonal.m_dFcaApo, tokN(pVBr->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 이므로", pVBr->m_strtDiagonal.m_dFc, 0.75*pVBr->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이상 이므로 작용하중을 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N2' = N2 = %.3f kN", tokN(pVBr->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	if(pVBr->m_strtDiagonal.m_dShear<pVBr->m_strtDiagonal.m_dAllowShear)
		strText.Format("             υ = N2'/ a·ΣL = %.1f MPa  <  υa    .... O.K", pVBr->m_strtDiagonal.m_dShear);
	else
		strText.Format("             υ = N2'/ a·ΣL = %.1f MPa  >  υa    .... N.G", pVBr->m_strtDiagonal.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     3) 하 현 재    ( %s )", pVBr->m_strtChordLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        단면적                :    Ag = %.1f mm²", pVBr->m_pChordLo->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경의 최소값 : γmin = %.1f mm", pVBr->m_pChordLo->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        단면2차 반경          :  γx  = %.1f mm", pVBr->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          ① 세장비 검토");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  <  150    .... O.K  ( ☞ 도.설.기 3.4.1.4 )",
			pVBr->m_pChordLo->m_dLength, pVBr->m_pChordLo->m_dGammaMin, pVBr->m_strtChordLo.m_dSlendernessRatio);
	else
		strText.Format("             ℓ/γmin = %.1f / %.1f = %.3f  >  150    .... N.G  ( ☞ 도.설.기 3.4.1.4 )",
			pVBr->m_pChordLo->m_dLength, pVBr->m_pChordLo->m_dGammaMin, pVBr->m_strtChordLo.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          ② 허용응력    ( ☞ 도.설.기 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - 허용압축응력");
	Arr.Add(strText+"");
	strText.Format("            ℓ/γx = %.1f / %.1f = %.3f",
		pVBr->m_pChordLo->m_dLength, pVBr->m_pChordLo->m_dGammaX, pVBr->m_pChordLo->m_dLength/pVBr->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pVBr->m_strtChordLo.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE에 연결되어 있는 경우에는 허용압축응력에 저감률을 곱하여 검토한다.");
	Arr.Add(strText+"");
	strText.Format("            fca＇=  fca × ( 0.5 + ( ℓ/γx ) / 1000 ) = %.1f MPa", pVBr->m_strtChordLo.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("          ③ 응력검토");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dFc<pVBr->m_strtChordLo.m_dFcaApo)
		strText.Format("                fc = N3 / An = %.1f / %.1f = %.3f MPa  <  fca'    .... O.K",
		pVBr->m_strtElementLoad.m_dN3, pVBr->m_pChordLo->m_dArea, pVBr->m_strtChordLo.m_dFc);
	else
		strText.Format("                fc = N3 / An = %.1f / %.1f = %.3f MPa  >  fca'    .... N.G",
		pVBr->m_strtElementLoad.m_dN3, pVBr->m_pChordLo->m_dArea, pVBr->m_strtChordLo.m_dFc);
	Arr.Add(strText+"");
	strText.Format("          ④ Gusset Plate와의 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접부의 허용응력 : υa = %.1f MPa    ( ☞ 도.설.기 3.3.2.3 )", pVBr->m_strtChordLo.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접각장          : ΣL = %.1f × 2EA = %.1f mm", pVBr->m_strtChordLo.m_dLengthWeld/2, pVBr->m_strtChordLo.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 용접목두께        :   a = %.1f / √2 = %.1f mm", pVBr->m_strtChordLo.m_dThroatDepth, pVBr->m_strtChordLo.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dFc<0.75*pVBr->m_strtChordLo.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 이므로", pVBr->m_strtChordLo.m_dFc, 0.75*pVBr->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이하 이므로 모재강도의 75%%를 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N3' = 0.75 × %.1f × %.1f = %.3f kN",
			pVBr->m_pChordLo->m_dArea, pVBr->m_strtChordLo.m_dFcaApo, tokN(pVBr->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 이므로", pVBr->m_strtChordLo.m_dFc, 0.75*pVBr->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             작용응력이 모재 전강도에 75%%이상 이므로 작용하중을 적용한다.");
		Arr.Add(strText+"");
		strText.Format("             N3' = N3 = %.3f kN", tokN(pVBr->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	if(pVBr->m_strtChordLo.m_dShear<pVBr->m_strtChordLo.m_dAllowShear)
		strText.Format("             υ = N3'/ a·ΣL = %.1f MPa  <  υa    .... O.K", pVBr->m_strtChordLo.m_dShear);
	else
		strText.Format("             υ = N3'/ a·ΣL = %.1f MPa  >  υa    .... N.G", pVBr->m_strtChordLo.m_dShear);
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    다) Gusset Plate의 검토");
	Arr.Add(strText+"");
	strText.Format("        작용하중과 모재강도의 75%%를 고려하여 설계한다.");
	Arr.Add(strText+"");
	strText.Format("      1) 상현재/사재 연결 (A,B)");
	Arr.Add(strText+"");
	strText.Format("       · 사용강재           : %.0f × %.0f × %.0f ( %s )",
		pVBr->m_strtGussetUp.m_dHeight, pVBr->m_strtGussetUp.m_dThick, pVBr->m_strtGussetUp.m_dWidth, pVBr->m_strtGussetUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       · 강재의 허용응력    : fa = %.1f MPa", pVBr->m_strtGussetUp.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       · 작용하중");
	Arr.Add(strText+"");
	strText.Format("          Rh = N1' + N2'cosΘ = %.3f + %.3f × cosΘ = %.3f kN",
		tokN(pVBr->m_strtChordUp.m_dN1Apo), tokN(pVBr->m_strtDiagonal.m_dN2Apo), tokN(pVBr->m_strtGussetUp.m_dRh));
	Arr.Add(strText+"");
	strText.Format("          Rv = N2'sinΘ = %.3f × sinΘ = %.3f kN", tokN(pVBr->m_strtDiagonal.m_dN2Apo), tokN(pVBr->m_strtGussetUp.m_dRv));
	Arr.Add(strText+"");
	strText.Format("          P = √( Rh2 + Rv2 ) = %.3f kN", tokN(pVBr->m_strtGussetUp.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       · 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1면마찰)   ρa = %.1f kN", pVBr->m_strtGussetUp.m_strBoltMaterial, pVBr->m_strtGussetUp.m_dDiaBolt, tokN(pVBr->m_strtGussetUp.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          필요 BOLT수");
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetUp.m_dQtyBoltUse>pVBr->m_strtGussetUp.m_dQtyBoltReq)
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... O.K",
		tokN(pVBr->m_strtGussetUp.m_dLoad), tokN(pVBr->m_strtGussetUp.m_dBoltAllowStress), pVBr->m_strtGussetUp.m_dQtyBoltReq, pVBr->m_strtGussetUp.m_dQtyBoltUse);
	else
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... N.G",
		tokN(pVBr->m_strtGussetUp.m_dLoad), tokN(pVBr->m_strtGussetUp.m_dBoltAllowStress), pVBr->m_strtGussetUp.m_dQtyBoltReq, pVBr->m_strtGussetUp.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("     · Gusset Plate의 응력검토");
	Arr.Add(strText+"");
	strText.Format("          An = ( %.0f - %.0f × %.0f ) × %.0f = %.1f mm²",
		pVBr->m_strtGussetUp.m_dHeight, pVBr->m_strtGussetUp.m_dQtyBoltUse, pVBr->m_strtGussetUp.m_dDiaBolt+3.0, pVBr->m_strtGussetUp.m_dThick, pVBr->m_strtGussetUp.m_dAn);
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetUp.m_dStress<pVBr->m_strtGussetUp.m_dAllowStress)
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pVBr->m_strtGussetUp.m_dLoad, pVBr->m_strtGussetUp.m_dAn, pVBr->m_strtGussetUp.m_dStress);
	else
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pVBr->m_strtGussetUp.m_dLoad, pVBr->m_strtGussetUp.m_dAn, pVBr->m_strtGussetUp.m_dStress);
	Arr.Add(strText+"");
	strText.Format("      2) 하현재 연결 (C,E)");
	Arr.Add(strText+"");
	strText.Format("       · 사용강재           : %.0f × %.0f × %.0f ( %s )",
		pVBr->m_strtGussetLo.m_dHeight, pVBr->m_strtGussetLo.m_dThick, pVBr->m_strtGussetLo.m_dWidth, pVBr->m_strtGussetLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       · 강재의 허용응력    : fa = %.1f MPa", pVBr->m_strtGussetLo.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       · 작용하중");
	Arr.Add(strText+"");
	strText.Format("          P = N3' = %.3f kN", tokN(pVBr->m_strtGussetLo.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       · 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1면마찰)   ρa = %.1f kN", pVBr->m_strtGussetLo.m_strBoltMaterial, pVBr->m_strtGussetLo.m_dDiaBolt, tokN(pVBr->m_strtGussetLo.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          필요 BOLT수");
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetLo.m_dQtyBoltUse>pVBr->m_strtGussetLo.m_dQtyBoltReq)
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... O.K",
		tokN(pVBr->m_strtGussetLo.m_dLoad), tokN(pVBr->m_strtGussetLo.m_dBoltAllowStress), pVBr->m_strtGussetLo.m_dQtyBoltReq, pVBr->m_strtGussetLo.m_dQtyBoltUse);
	else
		strText.Format("          n = P / ρa = %.3f / %.1f = %.1f EA → %.0f 개사용    .... N.G",
		tokN(pVBr->m_strtGussetLo.m_dLoad), tokN(pVBr->m_strtGussetLo.m_dBoltAllowStress), pVBr->m_strtGussetLo.m_dQtyBoltReq, pVBr->m_strtGussetLo.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("       · Gusset Plate의 응력검토");
	Arr.Add(strText+"");
	strText.Format("          An = ( %.0f - %.0f × %.0f ) × %.0f = %.1f mm²",
		pVBr->m_strtGussetLo.m_dHeight, pVBr->m_strtGussetLo.m_dQtyBoltUse, pVBr->m_strtGussetLo.m_dDiaBolt+3.0, pVBr->m_strtGussetLo.m_dThick, pVBr->m_strtGussetLo.m_dAn);
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetLo.m_dStress<pVBr->m_strtGussetLo.m_dAllowStress)
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pVBr->m_strtGussetLo.m_dLoad, pVBr->m_strtGussetLo.m_dAn, pVBr->m_strtGussetLo.m_dStress);
	else
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pVBr->m_strtGussetLo.m_dLoad, pVBr->m_strtGussetLo.m_dAn, pVBr->m_strtGussetLo.m_dStress);
	Arr.Add(strText+"");
	strText.Format("      3) 사재/하현재 연결 (D)");
	Arr.Add(strText+"");
	strText.Format("       · 사용강재           : %.0f × %.0f × %.0f ( %s )",
		pVBr->m_strtGussetDiagonal.m_dHeight, pVBr->m_strtGussetDiagonal.m_dThick, pVBr->m_strtGussetDiagonal.m_dWidth, pVBr->m_strtGussetDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       · 강재의 허용응력    : fa = %.1f MPa", pVBr->m_strtGussetDiagonal.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       · 작용하중");
	Arr.Add(strText+"");
	strText.Format("          P = N3' = %.3f kN", tokN(pVBr->m_strtGussetDiagonal.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       · 연결부 검토");
	Arr.Add(strText+"");
	strText.Format("          Fillet 용접부의 허용응력 : υa = %.3f MPa", pVBr->m_strtGussetDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("          Fillet 용접각장          : ΣL = %.1f × 2EA = %.1f mm", pVBr->m_strtGussetDiagonal.m_dLengthWeld/2.0, pVBr->m_strtGussetDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("          Fillet 용접목두께        :  a  = %.1f / √2 = %.3f mm", pVBr->m_strtGussetDiagonal.m_dThroatDepth, pVBr->m_strtGussetDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetDiagonal.m_dShearWeld<pVBr->m_strtGussetDiagonal.m_dAllowShearWeld)
		strText.Format("          υ = P/ a·ΣL = %.3f MPa  <  υa    .... O.K", pVBr->m_strtGussetDiagonal.m_dShearWeld);
	else
		strText.Format("          υ = P/ a·ΣL = %.3f MPa  >  υa    .... N.G", pVBr->m_strtGussetDiagonal.m_dShearWeld);
	Arr.Add(strText+"");
	strText.Format("       · Gusset Plate의 응력검토");
	Arr.Add(strText+"");
	strText.Format("           Ag = %.0f × %.0f = %.1f mm²", pVBr->m_strtGussetDiagonal.m_dWidth, pVBr->m_strtGussetDiagonal.m_dThick, pVBr->m_strtGussetDiagonal.m_dAg);
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetDiagonal.m_dShear<pVBr->m_strtGussetDiagonal.m_dAllowShear)
		strText.Format("           υ = P / Ag = %.3f MPa  <  υa    .... O.K", pVBr->m_strtGussetDiagonal.m_dShear);
	else
		strText.Format("           υ = P / Ag = %.3f MPa  >  υa    .... N.G", pVBr->m_strtGussetDiagonal.m_dShear);
	Arr.Add(strText+"");Arr.Add("");
}
///////////////////////////////////////////////////////////////////////////////////
//∫∴∵∮∑∏ㄷ ΑΒΓΔΕΖΗαβγδεζηΣθιΙκλμνξοπρf υφχψω
//	´≤＞＜≠±≤≥∞⌒×㎝⁴ ㎤ ㎠ ㎝ kgf/㎠ t·m  ¹²³⁴₁₂₃₄
//  ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩ ⑪ ⑫ ⑬ ⑭ ⑮ √½⅓㉮㉯㉰㉱㉲㉳㉴☞Φ
//  ━━━━━━━━━━━━━  ⇒ ⊙
//  ┏ ┗ ┛ ┓┃━ ┣ ┫ⅰ ⅱ ⅲ ⅳ ⅴ ⅵ ⅶ ⅷ ⅸ ⅹ Ⅰ Ⅱ Ⅲ Ⅳ Ⅳ Ⅴ Ⅵ
///////////////////////////////////////////////////////////////////////////////////// 
// 단면(가로보, 수직브레이싱) 검토
void CPlateStressOut::SectionCheckOutput(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map)
{
	CSection	*pSection	= pBx->GetSection();

	if(pSection->IsenType(SECTION_A))
	{
		if(pSection->IsenDetType(SECDET_CROSSBEAM))
			StressCheckEndCrossBeamGeneralType(Arr, pBx, Map);	// 단부 가로보(제작타입, H형강타입)
		else if(pSection->IsenDetType(SECDET_VBRACING))
			StressCheckEndCrossBeamVBracingType(Arr, pBx, Map);	// 단부 역V브레이싱형 타입
	}
	else
	{
		if(pSection->IsenDetType(SECDET_CROSSBEAM))
			StressCheckCenterCrossBeamGeneralType(Arr, pBx, Map);	// 중간 가로보(제작타입, H형강타입)
		else if(pSection->IsenDetType(SECDET_VBRACING))
			StressCheckCenterCrossBeamVBracingType(Arr, pBx, Map);	// 중간 V브레이싱형 타입
	}
/*
	if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))	// 수직브레이싱
	{
		Map["MType"] = 1;

		if(pBx->IsJijum())
			StressCheckVBracingJijumOutput(Arr, pBx);
		else
			StressCheckVBracingGenOutput(Arr, pBx);
	}
	else
	{
		Map["MType"] = 2;
		StressCheckCrossBeam(Arr, pBx, Map);	// 단부 가로보(제작타입, H형강타입)
	}
*/
}

void CPlateStressOut::CheckVStiff(CStringArray &Arr, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB		= m_pStd->GetBridge();
	CDataManage		*pDataManage	= m_pStd->GetDataManage();
	CPlateGirderApp	*pGir			= pBx->GetGirder();
	CAPlateCalcStd	*pCalcStd		= m_pStd->m_pCalcStd;
	CSection		*pSec			= pBx->GetSection();

	CFemStressCalc	StressCalc(pDataManage, pCalcStd);
	ASSERT(pSec);
	CVStiff *pVStiff = pSec->GetVStiff();

	if(!pVStiff) return;
	CString szText(_T("")), szSign, szCheck;
	CString	sText, sLeft, sNum, sRight;
	
	double	dSta		= pBx->GetStation();
	long	nG			= pBx->GetNumberGirder();
	BOOL	bUpper		= StressCalc.GetElementForce(ELE_MOMENT3, LC_SUM_MAX, nG, dSta, pBx->IsJijum() ? 1 : 2) > 0.0;
	long	nUpperNr	= MAX(pBx->GetWebHStiff()->GetTotalDansu(TRUE, TRUE),pBx->GetWebHStiff()->GetTotalDansu(TRUE, FALSE));
	long	nLowerNr	= MAX(pBx->GetWebHStiff()->GetTotalDansu(FALSE, TRUE),pBx->GetWebHStiff()->GetTotalDansu(FALSE, FALSE));
	long	nNr			= MAX(nUpperNr,nLowerNr);
	double	dTemp1		= 0;
	double	dMax_up		= 0;
	double	dMax_lo		= 0;
	double	dMax_upA	= 0;
	double	dMax_loA	= 0;
	double	dTau_up		= 0;
	double	dTau_lo		= 0;
	double	dTau_upA	= 0;
	double	dTau_loA	= 0;
	double	dVStiffW	= pVStiff->m_dWidth[0] != 0 ? (pVStiff->m_dWidth[0]) : (pVStiff->m_dWidth[1]);
	double	dVStiffT	= pVStiff->m_dThick[0] != 0 ? (pVStiff->m_dThick[0]) : (pVStiff->m_dThick[1]);
	double	dGap		= (pDB->m_dLengthBaseVStiff);
	double	dHGir		= pGir->GetHeightGirderByStaAng(dSta);
	double	dVStiffH	= pVStiff->GetVStiffHeight(TRUE, dHGir) != 0 ? pVStiff->GetVStiffHeight(TRUE, dHGir) : pVStiff->GetVStiffHeight(FALSE, dHGir);
	double	dTw			= (pGir->GetThickJewon(G_W, pBx));
	double	a_b			= dGap/dVStiffH;
	
	sLeft	=	pDB->GetMaterialNumber(CP_V, -1);
	sNum	=	pDB->GetMaterialNumber(CP_V,  0);
	sRight	=	pDB->GetMaterialNumber(CP_V,  1);

	StressCalc.GetFlangeStress(nG, dSta, dMax_up, dMax_lo, dMax_upA, dMax_loA, dTau_up, dTau_lo, dTau_upA, dTau_loA);

	double	dMax = bUpper ? dMax_up : dMax_lo;
	double	dTau = bUpper ? dTau_up : dTau_lo;
	szText.Format("  < 수직 보강재 검토  %s %s %s >", sLeft, sNum, sRight);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("   ·수평보강재의 사용 단수    :   n = %d", nNr);
	Arr.Add(szText+"");
	szText.Format("   ·복부판의 연섬유압축응력   :   f = %.3f MPa", dMax);
	Arr.Add(szText+"");
	szText.Format("   ·복부판의 전단응력         :  υ = %.3f MPa", dTau);
	Arr.Add(szText+"");
	szText.Format("   ·사용수직보강재 폭         : bvs = %.1f mm", dVStiffW);
	Arr.Add(szText+"");
	szText.Format("   ·사용 수직보강재의 두께    : tvs = %.1f mm", dVStiffT);
	Arr.Add(szText+"");
	szText.Format("   ·수직보강재의 간격         :   a = %.1f mm", dGap);
	Arr.Add(szText+"");
	szText.Format("   ·상하 양플랜지의 순간격    :   b = %.1f mm", dVStiffH);
	Arr.Add(szText+"");
	szText.Format("   ·복부판의 두께             :   t = %.1f mm", dTw);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("  ① 수직보강재의 사용여부 검토");
	Arr.Add(szText+"");

	CString strWeb = pDB->GetMaterial(G_W, pBx);
	CSteel *pSteelCtl = m_pStd->m_pDataManage->GetSteel();
	long nIndexSteel = pSteelCtl->GetIndex(strWeb);
	double dTerm = pSteelCtl->GetSteelAllowValue(ALLOW_VSTIFFNOT, nIndexSteel,	THICK40BELOW);

// 	double dTerm = 0;
// 	if(sNum == _T("400"))		dTerm = 70;
// 	else if(sNum == _T("490"))	dTerm = 60;
// 	else						dTerm = 57;

	szText.Format("   ·수직보강재를 생략할 수 있는 플랜지 순간격 : %.0ft", dTerm);
	Arr.Add(szText+"");
	szSign	= dTerm*dTw < dVStiffH ? _T("<")	: _T(">");
	szCheck	= dTerm*dTw < dVStiffH ? _T("∴ 수직보강재필요!")	: _T("∴ 수직보강재필요없음!");
	szText.Format("      %.0f × t = %.1f mm  %s  b = %.1f mm", dTerm, dTerm*dTw, szSign, dVStiffH);
	Arr.Add(szText+"");
	szText.Format("      %s", szCheck);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("  ② 수직보강재의 간격검토            ( ☞ 도.설.기 3.8.5.1 )");
	Arr.Add(szText+"");
	szSign	= a_b < 3.0 ? _T("<") : _T(">");
	szCheck	= a_b < 3.0 ? _T("O.K") : _T("N.G");
	szText.Format("    a/b = %.1f / %.1f = %.1f   %s   3.0     ===> %s", dGap, dVStiffH, a_b, szSign, szCheck);
	Arr.Add(szText+"");
	if(nNr > 0)
	{
		if(a_b>1)
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/3650,2)+pow(dTau/(810+610*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ·수평보강재를 %d단으로하고 a/b > 1 이므로", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)⁴[(f/3650)²+ { υ / 810+610(b/a)²}²] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
		}
		else
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/3650,2)+pow(dTau/(610+810*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ·수평보강재를 %d단으로하고 a/b ≤ 1 이므로", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)⁴[(f/3650)²+ { υ / 610+810(b/a)²}²] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
		}
	}
	else if(nNr > 1)
	{
		if(a_b>0.8)
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/9500,2)+pow(dTau/(1270+610*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ·수평보강재를 %d단으로하고 ＞ 0.80 이므로", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)⁴[(f/9500)²+ { υ / 1270+610(b/a)²}²] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
		}
		else
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/9500,2)+pow(dTau/(950+810*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ·수평보강재를 %d단으로하고 a/b ≤ 0.80 이므로", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)⁴[(f/9500)²+ { υ / 950+810(b/a)²}²] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
		}
	}
	else
	{
		if(a_b>0.64)
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/31500,2)+pow(dTau/(1970+610*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ·수평보강재를 %d단으로하고 ＞ 0.64 이므로", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)⁴[(f/31500)²+ { υ / 1970+610(b/a)²}²] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
			
		}
		else
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/31500,2)+pow(dTau/(1480+810*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ·수평보강재를 %d단으로하고 a/b ≤ 0.64 이므로", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)⁴[(f/31500)²+ { υ / 1480+810(b/a)²}²] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
			
		}		
	}
	Arr.Add("");
	szText.Format("  ③ 수직보강재의 강도 검토           ( ☞ 도.설.기 3.8.5.2 )");
	Arr.Add(szText+"");
	dTemp1 = 8.0*pow(dVStiffH/dGap, 2);
	double dIreq = dVStiffH*pow(dTw,3)/10.92*dTemp1;
	szText.Format("        γ = 8.0 × ( b / a )²= 8.0 × ( %.3f / %.3f )² = %.3f", dVStiffH, dGap, dTemp1);
	Arr.Add(szText+"");
	szText.Format("                   bt³                  %3.0f × %1.1f³", dVStiffH, dTw);
	Arr.Add(szText+"");
	szText.Format("    I(req) =  ━━━━━━ × γ = ━━━━━━━━━━━ × %.3f = %.3f mm⁴", dTemp1, dIreq);
	Arr.Add(szText+"");
	szText.Format("                  10.92                     10.92");
	Arr.Add(szText+"");
	double dIused = 1.0/3.0 * dVStiffT * pow(dVStiffW,3);
	szSign	= dIused > dIreq ? _T(">")		: _T("<");
	szCheck	= dIused > dIreq ? _T("O.K")	: _T("N.G");
	szText.Format("   I(used) = ⅓ × %.3f × %.3f³= %.3f mm⁴ %s  I(req)   ===> %s", dVStiffT, dVStiffW, dIused, szSign, szCheck);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("  ④ 수직보강재 돌출각의 최소폭");
	Arr.Add(szText+"");
	double dBmin = dVStiffH/30+5;
	szSign	= dBmin < dVStiffW ? _T("<")	: _T(">");
	szCheck	= dBmin < dVStiffW ? _T("O.K")	: _T("N.G");
	szText.Format("      bmin = %.3f / 30 + 5 = %.3f mm   %s   bvs = %.3f mm   ===> %s", dVStiffH, dBmin, szSign, dVStiffW, szCheck);
	Arr.Add(szText+"");
	Arr.Add("");

	szText.Format("  ⑤ 돌출각의 최소폭");
	Arr.Add(szText+"");
	double dTmin = dVStiffW/13;
	szSign	= dTmin < dVStiffT ? _T("<")	: _T(">");
	szCheck	= dTmin < dVStiffT ? _T("O.K")	: _T("N.G");
	szText.Format("      tmin = %.3f / 13 = %.3f mm   %s   tvs = %.3f mm   ===> %s", dVStiffW, dTmin, szSign, dVStiffT, szCheck);
	Arr.Add(szText+"");
}

void CPlateStressOut::Output_WeldReview(long nType, CShRichEdit &RichEdit, BOOL bJijum)
{
	CPlateBridgeApp *pDB      = m_pStd->GetBridge();
	CPlateDesigCalc	DesignCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);
	
	CString sText = _T("");
	CString sCheck1, sCheck2, sCheck3, sCheck4;	
	
	if (!bJijum)
	{
				///< 필렛 용접 치수 검토.	
		CMap <CString, LPCTSTR, double, double> Map;
		DesignCalc.GetMapWeldReview(nType, Map);

		double Smax  = Map["Smax"];///< 최대전단력(kN->N)
		double Tfu   = Map["Tfu"];
		double Tfl   = Map["Tfl"];
		double Tw    = Map["Tw"];
		double Yvsu  = Map["Yvsu"];
		double Yvsl  = Map["Yvsl"];
		double Iv    = Map["Iv"];
		double Su    = Map["Su"];
		double Sl    = Map["Sl"];
		double Tu	 = Map["Tu"];
		double Tl	 = Map["Tl"];		   
		double Sumin = Map["Sumin"];		   
		double Sumax = Map["Sumax"];
		double Slmin = Map["Slmin"];
		double Slmax = Map["Slmax"];
		double Tau	 = Map["Tau"];
		double Tal	 = Map["Tal"];
			
		long nColor = 0;

		sText.Format("[플랜지와 복부판의 용접부 검토]\n\n");
		RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

		sText.Format("  * 단면번호 : 단면 - %d              전단력 S(kN) : %.3f\n", nType+1, tokN(Smax));
		RichEdit.AddText(sText);
		sText.Format("  * 단면제원\n");
		RichEdit.AddText(sText);
		sText.Format("    - 상판 두께(mm)                : %.0f\n", Tfu);
		RichEdit.AddText(sText);
		sText.Format("    - 하판 두께(mm)                : %.0f\n", Tfl);
		RichEdit.AddText(sText);
		sText.Format("    - 복부판 두께(mm)             : %.0f\n", Tw);
		RichEdit.AddText(sText);

		sText.Format("    - 상부 Yvsu(mm)               : %.1f\n", Yvsu);
		RichEdit.AddText(sText);
		sText.Format("    - 하부 Yvsl(mm)                : %.1f\n", Yvsl);
		RichEdit.AddText(sText);
		sText.Format("    - 단면2차 모멘트 Iv (mm⁴) : %.3f\n", Iv);
		RichEdit.AddText(sText);

		sText.Format("    - 상부 용접 목두께 (mm)     : %.0f\n", Su);
		RichEdit.AddText(sText);
		sText.Format("    - 하부 용접 목두께 (mm)     : %.0f\n", Sl);
		RichEdit.AddText(sText);
		sText.Format("    - 상부 전단응력 υ (MPa)     : %.1f\n", Tu);
		RichEdit.AddText(sText);
		sText.Format("    - 하부 전단응력 υ (MPa)     : %.1f\n", Tl);
		RichEdit.AddText(sText);
		
		RichEdit.AddText("\n");

		sText.Format("  ① 필렛용접 치수검토\n");
		RichEdit.AddText(sText);

		nColor = RE_RED;
		if (Su < Sumin)
		{
			sCheck1.Format("    >> 상부 :  Suse (%g mm)  <  Smin (%.1f mm)  ", Su, Sumin);
			sText.Format("		...N.G\n");
		}
		else if (Su > Sumax)
		{
			sCheck1.Format("    >> 상부 :  Suse (%g mm)  >  Smax (%.1f mm)  ", Su, Sumax);
			sText.Format("		...N.G\n");
		}
		else
		{
			sCheck1.Format("    >> 상부 :  Smin (%.1f mm)  ≤  Suse (%g mm)  ≤  Smax (%.1f mm)  ", Sumin, Su, Sumax);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck1);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);

		nColor = RE_RED;
		if (Sl < Slmin)
		{
			sCheck2.Format("    >> 하부 :  Suse (%g mm)  <  Smin (%.1f mm)  ", Sl, Slmin);
			sText.Format("		...N.G\n");
		}
		else if (Sl > Slmax)
		{
			sCheck2.Format("    >> 하부 :  Suse (%g mm)  >  Smax (%.1f mm)  ", Sl, Slmax);
			sText.Format("		...N.G\n");
		}
		else
		{
			sCheck2.Format("    >> 하부 :  Smin (%.1f mm)  ≤  Suse (%g mm)  ≤  Smax (%.1f mm)  ", Slmin, Sl, Slmax);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck2);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);

		RichEdit.AddText("\n");
		sText.Format("  ② 용접부의 응력검토\n");
		RichEdit.AddText(sText);
		
		if (Tu > Tau)
		{
			sCheck3.Format("    >> 상부 :  전단응력 υ (%.1f MPa )  >  허용 전단응력 υa (%g MPa)  ", Tu, Tau);
			sText.Format("		...N.G\n");
			nColor = RE_RED;
		}
		else
		{
			sCheck3.Format("    >> 상부 :  전단응력 υ (%.1f MPa )  <  허용 전단응력 υa (%g MPa)  ", Tu, Tau);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck3);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);

		if (Tl > Tal)
		{
			sCheck4.Format("    >> 하부 :  전단응력 υ (%.1f MPa )  >  허용 전단응력 υa (%g MPa)  ", Tl, Tal);
			sText.Format("		...N.G\n");
			nColor = RE_RED;
		}
		else
		{
			sCheck4.Format("    >> 하부 :  전단응력 υ (%.1f MPa )  <  허용 전단응력 υa (%g MPa)  ", Tl, Tal);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck4);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);
	}
	else
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////
		// 지점보강재 용접 검토
		/////////////////////////////////////////////////////////////////////////////////////////////////		
		sText.Format("[지점보강재 용접부 검토]\n\n");
		RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);	
		
		long Nstiff=0;
		double dRectionMax, Ws, Ts, Ls, Tau, Ta;
		CString sPlate;

		int nIdx = 1;
		long nCheck = 2;
		if(pDB->m_nQtyJigan==1)		nCheck = 1;
		for (long i = 0; i < nCheck; i++)  
		{
			CMap <CString, LPCTSTR, double, double> JijumMap;	
			if(!DesignCalc.GetMapWeldJijumStiff(JijumMap, i==0 ? FALSE : TRUE))
				continue;
										
			dRectionMax = JijumMap["dRectionMax"];
			Ws			= JijumMap["Ws"];		// 지점 보강재 폭
			Ts			= JijumMap["Ts"];		// 지점 보강재 두께
			Ls			= JijumMap["Ls"];		// 지점 보강재 길이 (복부판 높이)
			Nstiff		= (long)JijumMap["Nstiff"];	// 지점 보강재 개수
			Tau			= JijumMap["Tau"];		// 전단응력		
			Ta			= JijumMap["Ta"];		// 허용전단 응력

			sPlate.Format ("%d - PL %g x %g x %g", Nstiff, Ws, Ts, Ls);
			
			if (i == 0)// 교대부		
				sText.Format("  %d) 교대부\n", nIdx++);
			else
			{
				RichEdit.AddText("\n");
				sText.Format("  %d) 교각부\n", nIdx);
			}

			RichEdit.AddText(sText);

			sText.Format("    - 반력 R(kN) : %.3f\n", dRectionMax);
			RichEdit.AddText(sText);
			sText.Format("    - 사용보강재 : %s\n", sPlate);
			RichEdit.AddText(sText);
			sText.Format("    - 전단응력 υ (MPa) : %.3f\n", Tau);
			RichEdit.AddText(sText);

			RichEdit.AddText("\n");

			if (Tau <= Ta)
			{
				sCheck1 = "    >> 전단응력 (υ)  <  허용 전단응력 (υa)";
				RichEdit.AddText(sCheck1);
				sCheck2 = "			...O.K\n";
				RichEdit.AddMsg(sCheck2, RE_BLUE, FALSE, TRUE);
			}
			else
			{
				sCheck1 = "    >> 전단응력 (υ)  >  허용 전단응력 (υa)";
				RichEdit.AddText(sCheck1);
				sCheck2 = "			...N.G\n";
				RichEdit.AddMsg(sCheck2, RE_RED, FALSE, TRUE);
			}		
		}
	}		
}

void CPlateStressOut::FillOnOKorNG(CShRichEdit &RichEdit, BOOL bIsOK, long nCnt)
{
	CString sTxt = "";
	long nColor = 0;
	if(bIsOK)
	{
		sTxt = "            ...O.K\n";
		nColor = RE_BLUE;
	}
	else
	{
		sTxt = "            ...N.G\n";
		nColor = RE_RED;
	}
	
	for (int i=1; i<nCnt; i++)
		sTxt += "\n";

	RichEdit.AddMsg(sTxt, nColor, FALSE,TRUE);
}
///////////////////////////////////////////////////////////////////////////////////
//∫∴∵∮∑∏ㄷ ΑΒΓΔΕΖΗαβγδεζηΣθιΙκλμνξοπρf υφχψω
//	´≤＞＜≠±≤≥∞⌒×㎝⁴ ㎤ ㎠ ㎝ kgf/㎠ t·m  ¹²³⁴₁₂₃₄
//  ① ② ③ ④ ⑤ ⑥ ⑦ ⑧ ⑨ ⑩ ⑪ ⑫ ⑬ ⑭ ⑮ √½⅓㉮㉯㉰㉱㉲㉳㉴☞
//  ━━━━━━━━━━━━━  ⇒ ⊙
//  ┏ ┗ ┛ ┓┃━ ┣ ┫ⅰ ⅱ ⅲ ⅳ ⅴ ⅵ ⅶ ⅷ ⅸ ⅹ Ⅰ Ⅱ Ⅲ Ⅳ Ⅳ Ⅴ Ⅵ
///////////////////////////////////////////////////////////////////////////////////// 