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
	Arr.Add ("  << 睡 營 溘 >>\r\n");
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
			Arr.Add ("旨收收有收收有收收收收收收收收收收收收收收收收收收收收有收收收收收有收收收收收收收收收收有收收收收收收收收收收有收收收收收收收收收收收收收有收收旬");
			Arr.Add ("早睡營早瞰薄早             м撩瞪 堅薑ж醞            早  м撩��  早      �� ж 醞      早     雖薄 藹ж      早         м    啗         早綠堅早");
			Arr.Add ("早廓�ㄕ食醽ㄕ� 鬼營濠醞 掘樓夔濠醞  180◥攪棚  夥款っ 早 堅薑ж醞 早   Max        Min   早   Max        Min   早      Max        Min      早    早");
			Arr.Add ("曳收收朱收收朱收收收收收收收收收收收收收收收收收收收收朱收收收收收朱收收收收收收收收收收朱收收收收收收收收收收朱收收收收收收收收收收收收收朱收收朽");
		}
		else
		{
			Arr.Add ("旨收收有收收有收收收收收收收收收收有收收收收收有收收收收收收收收收收有收收收收收收收收收收有收收收收收收收收收收收收收有收收旬");
			Arr.Add ("早睡營早瞰薄早   м撩瞪 堅薑ж醞  早  м撩��  早      �� ж 醞      早     雖薄 藹ж      早         м    啗         早綠堅早");
			Arr.Add ("早廓�ㄕ食醽ㄕ�  Steel    Concrete 早 堅薑ж醞 早   Max        Min   早   Max        Min   早      Max        Min      早    早");
			Arr.Add ("曳收收朱收收朱收收收收收收收收收收朱收收收收收朱收收收收收收收收收收朱收收收收收收收收收收朱收收收收收收收收收收收收收朱收收朽");
		}
		
		// 唸婁 Data Line 轎溘
		CString szBujeNum, szJointNum, szDeadSteel, szDeadBind, szDeadTurn, szDeadSlab, szPostDead;
		CString szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szName;
		double dDeadSteel, dDeadBind, dDeadTurn, dDeadSlab, dPostDead;
		double dLiveMax, dLiveMin, dUnSettleMax, dUnSettleMin, dTotalMax, dTotalMin;
		
		CFEMManage	*pFemPre = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);	///< м撩瞪 堅薑ж醞
		CFEMManage	*pFemPos = pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);	///< м撩�� 堅薑ж醞
		CFEMManage	*pFemLiv = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);		///< �勒狫�
		CFEMManage	*pFemSet = pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS);		///< 雖薄藹ж
		CFEMManage	*pFemCon = NULL;
		CResultArray	*pPreResult = pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);	///< м撩瞪 堅薑ж醞
		CResultArray	*pPosResult = pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);	///< м撩�� 堅薑ж醞
		CResultArray	*pLivResult = pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);		///< �勒狫�
		CResultArray	*pSetResult = pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);		///< 雖薄藹ж
		CResultArray	*pConResult = NULL;

		if(pDB->IsTUGir())
		{
			pFemCon = pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);		///< м撩瞪-180紫 攪棚 + 蝸楚粽(+陛煎爾)
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
						sprintf(sBuf, "早%s早%s早%s %s %s %s 早%s 早%s %s 早%s %s 早%s  %s  早 %s 早", 
									szBujeNum, szJointNum, szDeadSteel, szDeadBind, szDeadTurn, szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szName);
					}
					else
					{
						sprintf(sBuf, "早%s早%s早%s %s 早%s 早%s %s 早%s %s 早%s  %s  早 %s 早", 
									szBujeNum, szJointNum, szDeadSteel,	szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szName);
					}

					Arr.Add(sBuf);
				}
			}
		}
		if(pDB->IsTUGir())
			Arr.Add ("曲收收朴收收朴收收收收收收收收收收收收收收收收收收收收朴收收收收收朴收收收收收收收收收收朴收收收收收收收收收收朴收收收收收收收收收收收收收朴收收旭\r\n\r\n");
		else
			Arr.Add ("曲收收朴收收朴收收收收收收收收收收朴收收收收收朴收收收收收收收收收收朴收收收收收收收收收收朴收收收收收收收收收收收收收朴收收旭\r\n\r\n");
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
		if(nLoadGir==0)	sText.Format("%c. 謝難 譆渠檣 唳辦\r\n", nTitle++);
		else			sText.Format("%c. 辦難 譆渠檣 唳辦\r\n", nTitle++);
//		sText.Format("%c. G%d 譆渠檣 唳辦\r\n", nTitle++, nLoadGir + 1);
		Arr.Add (sText);
		
		long nLoadTypesu = 4;
		if(nLoadGir == nGirdersu - 1)	nLoadTypesu = 6;

		for(long nLoadType = 1; nLoadType <= nLoadTypesu; nLoadType++)
		{						
			pModelCalc.GetLiveLoadReaction(nLoadType, nLoadGir, 0, 0, bExist);   // п渡 LC 氈朝雖 褻餌.
			if (!bExist)   
			{
				pModelCalc.GetLiveLoadReaction(nLoadType, nLoadGir, 1, 0, bExist);   // п渡 LC 氈朝雖 褻餌.				
				if (!bExist) continue;
			}
			
			switch (nLoadType)
			{
				case 1 :	sText =" 1) DB ж醞\r\n";break;
				case 2 :	sText =" 2) DL ж醞\r\n";break;
				case 3 :	sText =" 3) TANK ж醞\r\n";break;
				case 4 :	sText =" 4) TANK TRAILER ж醞\r\n";break;
				case 5 :	sText.Format("%c. 爾紫 ж醞\r\n", nTitle++);break;
				case 6 :	sText.Format("%c. Ёж醞\r\n", nTitle++);	break;
			}
			Arr.Add(sText);

			long nMaxLanesu = 20;
			for(long nLane = 0; nLane <= nMaxLanesu; nLane++)
			{
				pModelCalc.GetLiveLoadReaction(nLoadType, nLoadGir, nLane, 0, bExist);   // п渡 離摹 氈朝雖 褻餌.
				if(!bExist) continue;

				if(nLoadType <= 2)
				{
					sText.Format(" %c. %d離摹 營ж\r\n", nLane+'a', nLane+1);
					Arr.Add(sText);
				}
				for(long nK = 0; nK <=  pDB->m_nQtyJigan; nK++)
				{															
					szTemp.Format("   < %s >\r\n", pDB->m_strJijumName[nK]);
					Arr.Add (szTemp);

					if(pDB->IsTUGir())
					{
						Arr.Add ("旨收收有收收有收收收收收收收收收收收收收收收收收收收收有收收收收收有收收收收收收收收收收有收收收收收收收收收有收收收收收收收收收收旬");
						Arr.Add ("早雖薄早瞰薄早             м撩瞪 堅薑ж醞            早  м撩��  早      �� ж 醞      早     雖薄 藹ж    早      м    啗      早");
						Arr.Add ("早嬪纂早廓�ㄕ� 鬼營濠醞 掘樓夔濠醞 180◥攪棚   夥款っ 早 堅薑ж醞 早   Max        Min   早   Max       Min  早   Max        Min   早");
						Arr.Add ("曳收收朱收收朱收收收收收收收收收收收收收收收收收收收收朱收收收收收朱收收收收收收收收收收朱收收收收收收收收收朱收收收收收收收收收收朽");
					}
					else
					{
						Arr.Add ("旨收收有收收有收收收收收收收收收收有收收收收收有收收收收收收收收收收有收收收收收收收收收有收收收收收收收收收收旬");
						Arr.Add ("早雖薄早瞰薄早   м撩瞪 堅薑ж醞  早  м撩��  早      �� ж 醞      早     雖薄 藹ж    早      м    啗      早");
						Arr.Add ("早嬪纂早廓�ㄕ�  Steel    Concrete 早 堅薑ж醞 早   Max        Min   早   Max       Min  早   Max        Min   早");
						Arr.Add ("曳收收朱收收朱收收收收收收收收收收朱收收收收收朱收收收收收收收收收收朱收收收收收收收收收朱收收收收收收收收收收朽");
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
								sprintf(sBuf, "早 %s 早%4d早%s  %s   %s  %s 早%s  早%s  %s  早%s %s 早 %s  %s 早", 
									szName, pJ->m_nIdx+1, szDeadSteel, szDeadConc, szDeadTurn, szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin);
							}
							else
							{
								sprintf(sBuf, "早 %s 早%4d早%s  %s  早%s  早%s  %s  早%s %s 早 %s  %s 早", 
									szName, pJ->m_nIdx+1, szDeadSteel, szDeadSlab, szPostDead,
									szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin);
							}
							

							if(ABS(atof(szTotalMax)) > 0.1)
								Arr.Add(sBuf);
						}
					}

					if(pDB->IsTUGir())
						Arr.Add ("曲收收朴收收朴收收收收收收收收收收收收收收收收收收收收朴收收收收收朴收收收收收收收收收收朴收收收收收收收收收朴收收收收收收收收收收旭\r\n\r\n");
					else
						Arr.Add ("曲收收朴收收朴收收收收收收收收收收朴收收收收收朴收收收收收收收收收收朴收收收收收收收收收朴收收收收收收收收收收旭\r\n\r\n");
				}			

				if(nLoadType >= 3)  // 戀觼諦 お溯橾楝朝 離摹檜 橈擠.
					break;
			}
		}
	}

	sText.Format("%c. 譆渠 奩溘 餵啗\r\n", nTitle++);
	Arr.Add (sText);
	// 唸婁 Data Line 轎溘
	for(long nK = 0; nK <= pDB->m_nQtyJigan; nK++)
	{		
		szTemp.Format("   < %s >\r\n", pDB->m_strJijumName[nK]);
		Arr.Add (szTemp);
		if(pDB->IsTUGir())
		{
			Arr.Add ("旨收收有收收有收收收收收收收收收收收收收收收收收收收收有收收收收收有收收收收收收收收收收有收收收收收收收收收有收收收收收收收收收收有收收收旬");
			Arr.Add ("早雖薄早瞰薄早             м撩瞪 堅薑ж醞            早  м撩��  早      �� ж 醞      早     雖薄 藹ж    早      м    啗      早 餌辨 早");
			Arr.Add ("早嬪纂早廓�ㄕ� 鬼營濠醞 掘樓夔濠醞 180◥攪棚   夥款っ 早 堅薑ж醞 早   Max        Min   早   Max       Min  早   Max        Min   早 Shoe 早");
			Arr.Add ("曳收收朱收收朱收收收收收收收收收收收收收收收收收收收收朱收收收收收朱收收收收收收收收收收朱收收收收收收收收收朱收收收收收收收收收收朱收收收朽");
		}
		else
		{
			Arr.Add ("旨收收有收收有收收收收收收收收收收有收收收收收有收收收收收收收收收收有收收收收收收收收收有收收收收收收收收收收有收收收旬");
			Arr.Add ("早雖薄早瞰薄早   м撩瞪 堅薑ж醞  早  м撩��  早      �� ж 醞      早     雖薄 藹ж    早      м    啗      早 餌辨 早");
			Arr.Add ("早嬪纂早廓�ㄕ�  Steel    Concrete 早 堅薑ж醞 早   Max        Min   早   Max       Min  早   Max        Min   早 Shoe 早");
			Arr.Add ("曳收收朱收收朱收收收收收收收收收收朱收收收收收朱收收收收收收收收收收朱收收收收收收收收收朱收收收收收收收收收收朱收收收朽");
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
					sprintf(sBuf, "早 %s 早%4d早%s  %s   %s  %s 早%s  早%s  %s  早%s %s 早 %s  %s 早%s 早", 
						szName, pJ->m_nIdx+1, szDeadSteel, szDeadConc, szDeadTurn, szDeadSlab, szPostDead,
						szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szShoeForce);
				}
				else
				{
					sprintf(sBuf, "早 %s 早%4d早%s  %s  早%s  早%s  %s  早%s %s 早 %s  %s 早%s 早", 
						szName, pJ->m_nIdx+1, szDeadSteel, szDeadSlab, szPostDead,
						szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szShoeForce);
				}

				if(ABS(atof(szTotalMax)) > 0.1)
					Arr.Add(sBuf);
			}
		}
		if(pDB->IsTUGir())
			Arr.Add ("曲收收朴收收朴收收收收收收收收收收收收收收收收收收收收朴收收收收收朴收收收收收收收收收收朴收收收收收收收收收朴收收收收收收收收收收朴收收收旭\r\n\r\n");
		else
			Arr.Add ("曲收收朴收收朴收收收收收收收收收收朴收收收收收朴收收收收收收收收收收朴收收收收收收收收收朴收收收收收收收收收收朴收收收旭\r\n\r\n");
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

	Arr.Add ("  << 籀颶榆餵啗 >>\r\n");
	// 唸婁 Data Line 轎溘
	double dLengthJigan = 0;
	double dDroop       = 0;
	long nForce = JOINT_DISPLACEMENT;
	long nStt = 0; 
	long nEnd = 0;
	CString szDeadSteel, szDeadSlab, szPostDead, szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szAllowDroop, szName, szTemp, szCheck;

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);

		szTemp.Format("   < 剪渦%d >\r\n", nG+1);
		Arr.Add (szTemp);
		Arr.Add ("旨收收有收收有收收收收收收收收收收有收收收收收有收收收收收收收收收收有收收收收收收收收收有收收收收收收收收收收有收收收收有收收旬");
		Arr.Add ("早雖薄早瞰薄早   м撩瞪 堅薑ж醞  早  м撩��  早      �� ж 醞      早     雖薄 藹ж    早      м    啗      早 ъ  辨 早綠堅早");
		Arr.Add ("早嬪纂早廓�ㄕ�  Steel    Concrete 早 堅薑ж醞 早   Max        Min   早   Max       Min  早   Max        Min   早 籀  颶 早    早");
		Arr.Add ("曳收收朱收收朱收收收收收收收收收收朱收收收收收朱收收收收收收收收收收朱收收收收收收收收收朱收收收收收收收收收收朱收收收收朱收收朽");
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
			dLengthJigan = toM (pDB->m_dLengthJigan[nJigan]);   // 雖除 剪葬.
			if(dLengthJigan > 40)   	dDroop = dLengthJigan / 500;
			else if(dLengthJigan > 10)  dDroop = dLengthJigan * dLengthJigan / 20000;
			else						dDroop = dLengthJigan / 2000;

			szAllowDroop.Format ("%8.3lf", dDroop);
			if(dDroop>fabs(pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMin[nForce].z))
				szCheck = "O.K.";
			else
				szCheck = "N.G.";
			
			sprintf(sBuf, "早 %s 早%4d早%s  %s  早%s  早%s  %s  早%s %s 早 %s  %s 早%s早%s早 ", 
					szName, pJ->m_nIdx+1, 
					szDeadSteel, szDeadSlab, szPostDead, szLiveMax, szLiveMin, szUnSettleMax, szUnSettleMin, szTotalMax, szTotalMin, szAllowDroop, szCheck);

			Arr.Add(sBuf);
		}
		Arr.Add ("曲收收朴收收朴收收收收收收收收收收朴收收收收收朴收收收收收收收收收收朴收收收收收收收收收朴收收收收收收收收收收朴收收收收朴收收旭\r\n\r\n");
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

	Arr.Add ("  << 歐 擠 (CAMBER) >>\r\n");

	for (int i = 0; i < pFEMManage->GetJointSize(); i++)   
	{
		pJ = pFEMManage->GetJoint(i);
		//if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE))	
		//	continue;

		if (pFEMManage->GetJoint(i)->m_LongAttrib[JOINT_GIR] != nCurGir)   // Girder陛 夥船賊
		{  
			// Header 轎溘
			nCurGir = pFEMManage->GetJoint(i)->m_LongAttrib[JOINT_GIR];
			pGir  = pDB->GetGirder(nCurGir);
			nCamberIdx = 0;
			szText.Format(" < Girder %d >", nCurGir+1);
			Arr.Add (szText);

			if(pDB->IsTUGir())
			{
				Arr.Add ("旨收收收有收收收收收收收收收收收收收收收收收收收收有收收收收收有收收收收收有收收收收收有收收收收收有收收收收旬");
				Arr.Add ("早 瞰薄 早             м撩瞪 堅薑ж醞            早  м撩��  早  謙堊摹  早  ら唳餌  早  歐擠榆  早  綠堅  早");
				Arr.Add ("早 廓�� 早 鬼營濠醞 掘樓夔濠醞 180◥攪棚   夥款っ 早 堅薑ж醞 早  爾薑榆  早  爾薑榆  早          早        早");
				Arr.Add ("曳收收收朱收收收收收收收收收收收收收收收收收收收收朱收收收收收朱收收收收收朱收收收收收朱收收收收收朱收收收收朽");
			}
			else
			{
				Arr.Add ("旨收收收有收收收收收收收收收收有收收收收收有收收收收收有收收收收收有收收收收收有收收收收旬");
				Arr.Add ("早 瞰薄 早   м撩瞪 堅薑ж醞  早  м撩��  早  謙堊摹  早  ら唳餌  早  歐擠榆  早  綠堅  早");
				Arr.Add ("早 廓�� 早  Steel    Concrete 早 堅薑ж醞 早  爾薑榆  早  爾薑榆  早          早        早");
				Arr.Add ("曳收收收朱收收收收收收收收收收朱收收收收收朱收收收收收朱收收收收收朱收收收收收朱收收收收朽");
			}
		}

		double dSta = pJ->m_vPointReal.x;
		nCamberIdx = pGir->GetCamberNodeByStation(dSta);
		CCamber	*pCamber	= pGir->GetCamberByNode(nCamberIdx);

		double dJong		= pCamber->GetCamberByType(pDB->GetBridgeType(), 0);	// 謙堊摹 爾薑榆
		double dSlop		= pCamber->GetCamberByType(pDB->GetBridgeType(), 1);	// ら唳餌 爾薑榆
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
			szTemp.Format("早%s 早 %s  %s  %s  %s 早 %s 早 %s 早 %s 早 %s 早   %s   早",
				szNode, szPreSteel, szPreConf, szPreTurn, szPreSlab, szPos, szJong, szSlop, szCamber, szJijum);
		}
		else
		{
			szTemp.Format("早%s 早 %s  %s 早 %s 早 %s 早 %s 早 %s 早   %s   早",
				szNode, szPreSteel, szPreConf, szPos, szJong, szSlop, szCamber, szJijum);
		}
		Arr.Add(szTemp);

		nCamberIdx++;
		
		long nSttJoint=0;
		long nEndJoint=0;
		ModelCalc.GetJointRangeByGirder(nCurGir, nSttJoint, nEndJoint);

		if (nEndJoint == i)	// Footer 轎溘
		{
			if(pDB->IsTUGir())
				Arr.Add ("曲收收收朴收收收收收收收收收收收收收收收收收收收收朴收收收收收朴收收收收收朴收收收收收朴收收收收收朴收收收收旭\r\n");
			else
				Arr.Add ("曲收收收朴收收收收收收收收收收朴收收收收收朴收收收收收朴收收收收收朴收收收收收朴收收收收旭\r\n");
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


	// 欽賊薯錳
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

	szText.Format("    < 欽賊薯錳 >\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);

	for (long n=0; n<3; n++)
	{
		szSub = (n==0)? _T("    - 謝難 警せ溯幗睡 : ") : (n==1)? _T("    - 醞       懈      睡 : ") : _T("    - 辦難 警せ溯幗睡 : ");
		szText.Format("%s 王堪檜H = %.0f mm, 王A = %.0f mm昌, 王yb = %.0f mm, 王I = %.0f mm昂, Zb = %.0f mm昆, ep = %.0f mm \n", 
			szSub, dHeight[n], dArea[n], dYb[n], dIx[n], dZb[n], dEp[n]);
		RichEdit.AddText(szText);
	}
	RichEdit.AddText("\n");

	// PS鬼營薯錳
	double dTendon_Dia = Map["Tendon_Dia"];
	double dTendon_Area= Map["Tendon_Area"];
	double dTendon_Qty = Map["Tendon_Qty"];

	szText.Format("    < PS鬼營 薯錳 >\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);

	szText = _T("    王餌辨 曹濰營\n");
	RichEdit.AddText(szText);
	szText.Format("      仰 %.2f mm   x   %.0f   =   %.2f   x   %.0f   =   %.2f mm昌\n\n", 
			dTendon_Dia, dTendon_Qty, dTendon_Area, dTendon_Qty, dTendon_Area*dTendon_Qty );
	RichEdit.AddText(szText);

	// 睡嬪滌 擬溘匐饜
	// Щ葬蝶お溯諒 霜�� 曹濰營 擬溘 塽 ъ辨擬溘 匐饜
	double dStress_Fj = Map["Stress_Fj"];
	double dLoss_Fric = Map["Loss_Fric"];
	double dLoss_Act  = Map["Loss_Act"];
	double dLoss_Elec = Map["Loss_E"];
	double dLoss_Total= Map["Loss_Total"];
	double dStress_Total = Map["Stress_Total"];

	szText = (nSide==0)? _T("    謝難 警せ溯幗睡") : (nSide==1)? _T("    辦難 警せ溯幗睡") : _T("    蝸楚粽 醞懈睡");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText = (nSide==0 && pDB->m_nTendonFix<1)? _T(" (堅薑欽)") : (nSide==0 && pDB->m_nTendonFix==1)?  _T(" (曹濰欽)") : _T(""); 
	szText += (nSide==1 && pDB->m_nTendonFix<1)? _T(" (曹濰欽)") : (nSide==1 && pDB->m_nTendonFix==1)?  _T(" (堅薑欽)") : _T("");
	szText += _T("\n\n");
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	szText = _T("    函 Щ葬蝶お溯諒 霜�� 曹濰營 擬溘 塽 ъ辨擬溘 匐饜   ( Ｃ 紫.撲.п 4.6.3.3 )		(欽嬪 : MPa)\n");
	RichEdit.AddText(szText);
	szText.Format("     王曹濰擬溘 (fj) : %.3f 王葆雞槳褒(汁fpF) : %.3f 王�做蕉桵�(汁fan) : %.3f 王驕撩槳褒(汁fpEL) : %.3f 王識槳褒榆(汁fpt) : %.3f 王曹濰營擬溘(fpt) : %.3f \n\n",
					dStress_Fj, dLoss_Fric, dLoss_Act, dLoss_Elec, dLoss_Total, dStress_Total);
	RichEdit.AddText(szText);

	// ъ辨擬溘匐饜-1
	double dStrength_Ultimate	= Map["Str_Ulimate"];
	double dStrength_Yield		= Map["Str_Yield"];
	double dFactored_Ultimate	= Map["Fact_Ulimate"];
	double dFactored_Yield		= Map["Fact_Yield"];

	szText = "     - ъ辨擬溘匐饜\n";
	RichEdit.AddText(szText);
	
	if (dStress_Total<=dFactored_Ultimate)	{ szSign=_T("‾"); szOk=_T("ˍ O.K"); }
	if (dStress_Total<dFactored_Ultimate)	{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (dStress_Total>=dFactored_Ultimate)	{ szSign=_T("￣"); szOk=_T("ˍ N.G"); }
	if (dStress_Total>dFactored_Ultimate)	{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }

	szText = "     王fpt    =    ";
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dStress_Total);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    0.70 fpu    =    0.70 * %.2f    =    ", szSign, dStrength_Ultimate);
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dFactored_Ultimate);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	if (dStress_Total<=dFactored_Yield)	{ szSign=_T("‾"); szOk=_T("ˍ O.K"); }
	if (dStress_Total<dFactored_Yield)	{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (dStress_Total>=dFactored_Yield)	{ szSign=_T("￣"); szOk=_T("ˍ N.G"); }
	if (dStress_Total>dFactored_Yield)	{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }

	szText = "     王fpt    =    ";
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dStress_Total);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    0.83 fy      =    0.83 * %.2f    =    ", szSign, dStrength_Yield);
	RichEdit.AddText(szText);
	szText.Format("%.2f MPa    ", dFactored_Yield);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s\n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 衛除縑 評艇 識 槳褒擊 堅溥и 曹濰營 擬溘 骯薑 塽 ъ辨擬溘 匐饜
	double dTime_FJ = Map["Time_FJ"];	// 曹濰營擬溘
	double dTime_Sh = Map["Time_Sh"];	// 勒褻熱蹴
	double dTime_Cr = Map["Time_Cr"];	// 觼葬Щ
	double dTime_Re = Map["Time_Re"];	// 萵滓撮檜暮
	double dTime_To = Map["Time_To"];	// 識槳褒榆
	double dTime_EFJ = Map["Time_EFJ"];	// 嶸�膨靺敻蝩懋�


	szText = _T("    刻 衛除縑 評艇 識 槳褒擊 堅溥и 曹濰營 擬溘 骯薑 塽 ъ辨擬溘 匐饜   ( Ｃ 紫.撲 4.6.3.3 )	\n");
	RichEdit.AddText(szText);
	szText.Format("     王曹濰營擬溘 (fpt) : %.3f 王勒褻熱蹴(汁fpSH) : %.3f 王觼葬Щ槳褒(汁fpCR) : %.3f 王萵楷撮檜暮(汁fpR) : %.3f 王識槳褒榆(汁fpe) : %.3f 王嶸�膨靺敻蝩懋�(fpe) : %.3f \n\n",
		dTime_FJ, dTime_Sh, dTime_Cr, dTime_Re, dTime_To, dTime_EFJ);
	RichEdit.AddText(szText);
	
	// ъ辨擬溘匐饜-2
	double dStrength_Yield2		= Map["Str_Yield2"];
	double dFactored_Yield2		= Map["Fact_Yield2"];

	szText = "     - ъ辨擬溘匐饜\n";
	RichEdit.AddText(szText);

	if (dTime_EFJ<=dFactored_Yield2)	{ szSign=_T("‾"); szOk=_T("ˍ O.K"); }
	if (dTime_EFJ<dFactored_Yield2)		{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (dTime_EFJ>=dFactored_Yield2)	{ szSign=_T("￣"); szOk=_T("ˍ N.G"); }
	if (dTime_EFJ>dFactored_Yield2)		{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }

	szText = "     王fpe    =    ";
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

	// 0028262  檜蓬縑 曖п 瞰渠高擊 鏃ж雖 彊擠 - 匐饜 晦�� 綠掖衛縑虜 瞰渠高 餌辨.
	double dForced_Top_Before =	Map["Force_Top_Before"];	// fabs(Map["Force_Top_Before"]);
	double dForced_Top_After  =	Map["Force_Top_After"];		// fabs(Map["Force_Top_After"]);
	double dForced_Bot_Before =	Map["Force_Bot_Before"];	// fabs(Map["Force_Bot_Before"]);
	double dForced_Bot_After  =	Map["Force_Bot_After"];		// fabs(Map["Force_Bot_After"]);

	double dAllow_Top_Before  =	Map["Allow_Top_Before"];	// fabs(Map["Allow_Top_Before"]);
	double dAllow_Top_After   =	Map["Allow_Top_After"];		// fabs(Map["Allow_Top_After"]);
	double dAllow_Bot_Before  =	Map["Allow_Bot_Before"];	// fabs(Map["Allow_Bot_Before"]);
	double dAllow_Bot_After   =	Map["Allow_Bot_After"];		// fabs(Map["Allow_Bot_After"]);

	// 嬪纂
	szText = (nSide==0)? _T("    謝難 警せ溯幗睡") : (nSide==1)? _T("    辦難 警せ溯幗睡") : _T("    蝸楚粽 醞懈睡");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText = (nSide==0 && pDB->m_nTendonFix<1)? _T(" (堅薑欽)") : (nSide==0 && pDB->m_nTendonFix==1)?  _T(" (曹濰欽)") : _T(""); 
	szText += (nSide==1 && pDB->m_nTendonFix<1)? _T(" (曹濰欽)") : (nSide==1 && pDB->m_nTendonFix==1)?  _T(" (堅薑欽)") : _T("");
	szText += _T("\n\n");
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 函 Щ葬蝶お溯諒 紫殮 霜�釋� 擬溘 匐饜								
	szText = _T("    函 Щ葬蝶お溯諒 紫殮 霜�釋� 擬溘 匐饜\n\n");
	RichEdit.AddText(szText);

	// 鼻翱
	if (fabs(dForced_Top_Before) <= fabs(dAllow_Top_Before))	{ szSign=_T("‾"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Top_Before) < fabs(dAllow_Top_Before))		{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Top_Before) >= fabs(dAllow_Top_Before))	{ szSign=_T("￣"); szOk=_T("ˍ N.G"); }
	if (fabs(dForced_Top_Before) > fabs(dAllow_Top_Before))		{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }
	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow;
	szStress =  (dForced_Top_Before < 0) ? szStr2 : szStr1;

	szText = _T("     - 鼻翱 ъ辨 擬溘 匐饜\n");
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
	
	// ж翱
	if (fabs(dForced_Bot_Before) <= fabs(dAllow_Bot_Before))	{ szSign=_T("‾"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Bot_Before) < fabs(dAllow_Bot_Before))		{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Bot_Before) >= fabs(dAllow_Bot_Before))	{ szSign=_T("￣"); szOk=_T("ˍ N.G"); }
	if (fabs(dForced_Bot_Before) > fabs(dAllow_Bot_Before))		{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }
	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_PreStressStretchPow;
	szStress =  (dForced_Bot_Before < 0) ? szStr2 : szStr1;

	szText = _T("     - ж翱 ъ辨 擬溘 匐饜\n");
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

	// 刻 撲啗ж醞 濛辨衛曖 擬溘 匐饜						
	szText = _T("    刻 撲啗ж醞 濛辨衛曖 擬溘 匐饜\n\n");
	RichEdit.AddText(szText);

	// 鼻翱
	if (fabs(dForced_Top_After) <= fabs(dAllow_Top_After))	{ szSign=_T("‾"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Top_After) < fabs(dAllow_Top_After))		{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Top_After) >= fabs(dAllow_Top_After))	{ szSign=_T("￣"); szOk=_T("ˍ N.G"); }
	if (fabs(dForced_Top_After) > fabs(dAllow_Top_After))		{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }

	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow;
	szStress =  (dForced_Top_After < 0) ?  szStr2 : szStr1;
	
	szText = _T("     - 鼻翱 ъ辨 擬溘 匐饜\n");
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

	// ж翱
	if (fabs(dForced_Bot_After) <= fabs(dAllow_Bot_After))		{ szSign=_T("‾"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Bot_After) < fabs(dAllow_Bot_After))		{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (fabs(dForced_Bot_After) >= fabs(dAllow_Bot_After))		{ szSign=_T("￣"); szOk=_T("ˍ N.G"); }
	if (fabs(dForced_Bot_After) > fabs(dAllow_Bot_After))		{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }

	szStr1	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightPressPow;
	szStr2	 =  pData->DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow;
	szStress =  (dForced_Bot_After < 0) ? szStr2 :szStr1;
	
	szText = _T("     - ж翱 ъ辨 擬溘 匐饜\n");
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

	newElement.Format("3. 夥款っ 撲啗");
	Arr.Add(newElement+"\r\n");
	newElement.Format(" 3.1 騰せ溯幗睡");
	Arr.Add(newElement+"\r\n");
	newElement.Format("  1) 謝難騰せ溯幗睡");
	Arr.Add(newElement+"\r\n");

	Output_DeadLoad(Arr, 0);				//餌ж醞
//	Output_MoveLoad(Arr, TRUE);				//�勒狫�
//	Output_EtcLoad(Arr, TRUE);				//晦顫ж醞	
//	Output_TotalCombinationLoad(Arr, 0);	//ж醞褻м
//	Output_SlabThickness(Arr, 0);		//蝸楚粽 譆模舒眷匐饜
}

void CPlateStressOut::SlabSectionOutputRight(CStringArray &Arr)
{

	CString newElement(_T(""));	

	newElement.Format("  2) 辦難騰せ溯幗睡");
	Arr.Add(newElement+"\r\n");

	Output_DeadLoad(Arr, 1);				//餌ж醞
//	Output_MoveLoad(Arr, FALSE);			//�勒狫�
//	Output_EtcLoad(Arr, FALSE);				//晦顫ж醞	
//	Output_TotalCombinationLoad(Arr, 1);	//ж醞褻м
//	Output_SlabThickness(Arr, 1);		//蝸楚粽 譆模舒眷匐饜
}

void CPlateStressOut::SlabSectionOutputCenter(CStringArray &Arr)
{
//	CCalcData       *pData = m_pStd->GetDataManage()->GetCalcData();
//	CPlateBridgeApp *pDB   = m_pStd->GetBridge();

	CString newElement(_T(""));	

	newElement.Format("  3) 蝸楚粽 醞懈睡");
	Arr.Add(newElement+"\r\n");

	Output_DeadLoad(Arr, 2);			    //餌ж醞
//	Output_MoveLoadCenter(Arr);		    	//�勒狫�
//	if(pDB->IsStraightLineTotalBridge())
//		OutPut_CentriodLoad(Arr);				//錳褕ж醞
//	Output_TotalCombinationLoad(Arr, 2);	//ж醞褻м
//	Output_SlabThickness(Arr, 2);		//蝸楚粽 譆模舒眷匐饜
}

void CPlateStressOut::SlabSectionOutputReview(CStringArray &Arr)
{
	CString newElement(_T(""));		

//	Output_DroopReview(Arr);			//籀颶匐饜

	newElement.Format("    <<敕翮 塽 敕翮ァ 匐饜>>         (夔觼葬お 衛寞憮   P139)");
	Arr.Add(newElement+"\r\n");	 
//	Output_CrackReview(Arr, 0);			//謝難騰せ溯幗睡 敕翮匐饜
//	Output_CrackReview(Arr, 1);			//辦難騰せ溯幗睡 敕翮匐饜
//	Output_CrackReview(Arr, 2);			//醞懈睡 敕翮匐饜
//	Output_CrackReview(Arr, 3);			//諼難陛煎爾 醞懈睡 - 謝難
//	Output_CrackReview(Arr, 4);			//諼難陛煎爾 醞懈睡 - 辦難
//	Output_GiJumReview(Arr);			//雖薄睡夥款っ 匐饜
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
	newElement.Format("    <<餌ж醞>>");
	Arr.Add(newElement+"\r\n");
	if(nPos==2)
	{
		newElement.Format("  旨收收有收收收收收收收收收收收收收收收收收收收收收收收有收收收收收收有收收收收收收收旬");
		Arr.Add(newElement);
		newElement.Format("  早 NO 早         骯                   衝              早 濠醞(tonf) 早賅詮お(tonf.m)早");
		Arr.Add(newElement);
		newElement.Format("  曳收收朱收收收收收收收收收收收收收收收收收收收收收收收朱收收收收收收朱收收收收收收收朽");
		Arr.Add(newElement);
	}
	else
	{
		newElement.Format("  旨收收有收收收收收收收收收收收收收收收收收收收收收收收收有收收收收收收有收收收收收收有收收收收收收收旬");
		Arr.Add(newElement);
		newElement.Format("  早 NO 早         骯                   衝                早 濠醞(tonf) 早   剪葬(m)  早賅詮お(tonf.m)早");
		Arr.Add(newElement);
		newElement.Format("  曳收收朱收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收收朽");
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
			newElement.Format("  早  %2d早%-44s  早 %-11s早%-13s 早", i+1, str1, str2, str4 );
		else
			newElement.Format("  早  %2d早 %-44s   早   %s   早   %s   早    %s  早", i+1, str1, str2, str3, str4 );

		Arr.Add(newElement);
	}

	if(nPos==2)
	{
		newElement.Format("  曲收收朴收收收收收收收收收收收收收收收收收收收收收收收朴收收收收收收朴收收收收收收收旭");
		Arr.Add(newElement+"\r\n\r\n\r\n");
	}
	else
	{
		CString str1 = ArrSrc.GetAt(index++);
		CString str2 = ArrSrc.GetAt(index++);

		newElement.Format("  曳收收朱收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收收朽");
		Arr.Add(newElement);
		newElement.Format("  早 啗 早                                                早   %5s   早            早   %8s  早", str1, str2 );
		Arr.Add(newElement);
		newElement.Format("  曲收收朴收收收收收收收收收收收收收收收收收收收收收收收收朴收收收收收收朴收收收收收收朴收收收收收收收旭");
		Arr.Add(newElement+"\r\n\r\n\r\n");
	}
}

// �勒狫�
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

	if(bWalkLoad)	sText.Format("    (2) �� ж 醞  ( 捱醞ж醞 : 5 kN/m昌)	(Ｃ紫.撲.晦 2.1.3)\n\n");
	else			sText.Format("    (2) �� ж 醞  ( DB-24 : Pr = %.1f kN )	(Ｃ紫.撲.晦 2.1.3)\n\n", tokN(dPr));
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	if(pCalcData->DESIGN_FLOOR_DATA[nIndex].m_bWalkLoad)
	{
		dL	= toM(CalcGeneral.GetWidthBodoDeadLoad(bLeft, pBx));
		if(dL > 0)
		{
			sText.Format("     Ml = 5 kN/妊 ▼ L昌/ 2 = %.3f ▼ %.3f昌/ 2\n", 5.0, dL);
			RichEdit.AddText(sText);
			sText.Format("        = %.3f kN.m/m (爾紫 5 kN/妊 蛔碟んж醞 營ж)\n", dMl);
			RichEdit.AddText(sText);
		}
		else
		{
			sText.Format("爾紫ж醞 啗骯 в蹂橈擠\n");
			RichEdit.AddText(sText);
		}

		return;
	}

	dL = toM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_L);
	if(CalcFloor.IsLaneLoad(nIndex) && dL > 0)
	{
		// 橾奩�勒狫�
		double di			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_i;
		double dE			= toM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_E);
		double dMl			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml);
		double dMl_			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml_);

		// 戀觼捨紫ж醞
		double dTankWeight	= pCalcData->m_TankWeight;
		double dL2			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Tank_L;
		double dP2			= dTankWeight / 2 / pCalcData->m_TankEdit1;
		double dMl2			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml_Tank);
		
		// 戀觼お溯橾楝ж醞
		double dTrailer		= max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
		double dL3			= pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Trailer_L;
		double dE3			= CalcGeneral.CalcMovingLoad_E(dL3);
		double dMl3			= tokNM(pCalcData->CALC_CANTILEVER_DATA[nIndex].m_Move_Ml_Trailer);

		CalcGeneral.CalcMovingLoad_I(dL, di, dii);
		if( dii > 0.3 )	sign = _T(">");
		else			sign = _T("<");

		sText.Format("      函 醱 問 啗 熱\n");
		RichEdit.AddText(sText);
		sText.Format("       i = 15 / (40 + L) = 15 / (40 + %.3f) = %.3f  %s  0.30      ˍ i = %.3f\n\n", dL, dii, sign, di);
		RichEdit.AddText(sText);
		sText.Format("      刻 彌ж醞 碟んァ\n");
		RichEdit.AddText(sText);
		sText.Format("        E  = 0.8 ▼L + 1.14 = 0.8 ▼ %.3f + 1.14 = %.3f m   罹晦憮,  L = %.3f m\n\n", dL, 0.8*dL+1.14, dL);
		RichEdit.AddText(sText);
		sText.Format("      券 �� 賅 詮 お\n");
		RichEdit.AddText(sText);
		sText.Format("       - 橾奩�勒狫� 瞳辨衛\n");
		RichEdit.AddText(sText);
		// lina
		sText.Format("       Ml  =  (Pr/E)  ▼  L  ▼ (1+i)  = (%.3f/%.3f)  ▼ %.3f  ▼ (1+%.3f)  = %.3f  kN.m/m\n\n", tokN(dPr), dE, dL, di, (tokN(dPr)/dE)*dL*(1+di));
		RichEdit.AddText(sText);
		if(dL-1.8>0)
		{
			CalcGeneral.CalcMovingLoad_I(dL-1.8, di, dii);
			if( dii > 0.3 )	sign = _T(">");
			else			sign = _T("<");

			sText.Format("      刷 醱 問 啗 熱\n");
			RichEdit.AddText(sText);
			sText.Format("       i = 15 / (40 + L) = 15 / (40 + %.3f) = %.3f  %s  0.30      ˍ i = %.3f\n\n", dL-1.8, dii, sign, di);
			RichEdit.AddText(sText);
			sText.Format("      刺 彌ж醞 碟んァ\n");
			RichEdit.AddText(sText);
			sText.Format("        E2  = 0.8 ▼L + 1.14 = 0.8 ▼ %.3f + 1.14 = %.3f m   罹晦憮,  L = %.3f m\n\n", dL-1.8, dE, dL-1.8);
			RichEdit.AddText(sText);
			sText.Format("      到 �� 賅 詮 お\n");
			RichEdit.AddText(sText);
			sText.Format("       - 橾奩�勒狫� 瞳辨衛\n");
			RichEdit.AddText(sText);
			sText.Format("       Ml2  =  (Pr/E)  ▼  L  ▼ (1+i)  = (%.3f/%.3f)  ▼ %.3f  ▼ (1+%.3f)  = %.3f  kN.m/m\n\n", tokN(dPr), dE, dL-1.8, di, dMl_);
			RichEdit.AddText(sText);
			sText.Format("       Ml Total = Ml + Ml2 = %.3f + %.3f = %.3f kN.m/m\n\n", dMl-dMl_,dMl_,dMl);
			RichEdit.AddText(sText);
		}

		if(pCalcData->m_bTank)			// 戀觼ж醞
		{
			sText.Format("       - 戀觼捨紫ж醞 濛辨衛\n");
			RichEdit.AddText(sText);
			sText.Format("       Pr  =  W / 2 / L ( 戀觼蕾雖望檜 ) =  %.1f / 2 / %.3f   =   %.3f kN/m\n", dTankWeight, pCalcData->m_TankEdit1, dP2);
			RichEdit.AddText(sText);
			sText.Format("       Ml  =  Pr  ▼  L ( 賅詮お望檜 )  ▼ (1 + i)  = %.3f  ▼ %.3f  ▼ (1+%.3f)  = %.3f  kN.m/m\n", dP2, dL2, diT, dMl2);
			RichEdit.AddText(sText);
		}

		if(pCalcData->m_bTrailer)
		{
			if(dL3 > 0)
			{
				sText.Format("       - 戀觼お溯橾楝ж醞 濛辨衛\n");
				RichEdit.AddText(sText);
				sText.Format("       E  =  0.8 ▼ L + 1.14  =  0.8 ▼ %.3f + 1.14   =   %.3f m\n", dL3, dE3);
				RichEdit.AddText(sText);
				sText.Format("       Ml =  (Pr/E)  ▼  L ( 賅詮お望檜 )  ▼ (1+i)  = (%.3f/%.3f)  ▼ %.3f  ▼ (1+%.3f)  = %.3f  kN.m/m\n", dTrailer, dE3, dL3, diT, dMl3);
				RichEdit.AddText(sText);
			}
			if(dL3<=0)
			{
				sText.Format("     戀觼お溯橾楝ж醞 啗骯 в蹂橈擠\n");
				RichEdit.AddText(sText);
			}
		}
	}
	else
	{
		sText.Format("     �勒狫萺� 營ж腎雖 彊擠\n");
		RichEdit.AddText(sText);
	}
}

// �勒狫� 啗骯 - 醞懈睡
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

	sText.Format("    (2) �勒狫�     ( DB-24 : Pr = %.1f kN)     (Ｃ紫.撲.晦 2.1.3)\n\n", dPr);
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	if(CalcFloor.IsLaneLoad(FLOOR_MID))
	{
		sText.Format("      啗骯雖除 L = %.3f m      \r\n", dL);						RichEdit.AddText(sText);
		sText.Format("      欽牖雖雖曖 唳辦 夥款っ曖 雖除擎 雖雖剪渦曖 醞褕除問戲煎 撲薑и棻.\n");;	RichEdit.AddText(sText);
		sText.Format("      檜陽 雖除擎 夥款っ曖 牖雖除縑 夥款っ曖 舒眷蒂 渦и 望檜蒂 蟾婁й в蹂朝 橈棻.\n\n");
		RichEdit.AddText(sText);

		if( dII > dComp_i ) sign = _T(" >");
		else 	sign = _T("<");

		sText.Format("      函 醱 問 啗 熱  i = 15 / (40 + L) = 15 / (40 + %.3f) = %.3f   %s   0.3    ˍ  i = %.3f\n\n", dL, dII, sign, dI);
		RichEdit.AddText(sText);
		sText.Format("      刻 �� 賅 詮 お\n");
		RichEdit.AddText(sText);
		sText.Format("       - 橾奩�勒狫� 瞳辨衛\n");
		RichEdit.AddText(sText);
		sText.Format("         Ml  =  ( L + 0.6 ) / 9.6 ▼ Pr ▼ ( 1 + i )  (夥款っ檜 3偃 檜鼻曖 雖薄擊 陛霞 翱樓っ橾陽朝 0.8寡蒂 鏃л)\n");
		RichEdit.AddText(sText);
		sText.Format("              =  ( %.3f + 0.6 ) / 9.6 ▼ %.3f ▼ ( 1 + i ) = %.3f  kN/m\n", dL, dPr, dMl);
		RichEdit.AddText(sText);

		if(pCalcData->m_bTank)
		{
			sText.Format("       - 戀觼捨紫ж醞 瞳辨衛\n");
			RichEdit.AddText(sText);
			RichEdit.AddText("");
			sText.Format("         Pr  =  W / 2 / L(戀觼蕾雖望檜) =  %.1f / 2 / %.3f   =   %.3f kN/m\n", pCalcData->m_TankWeight, pCalcData->m_TankEdit1, dTankWeight);
			RichEdit.AddText(sText);
			sText.Format("         Ml  =  ( %.3f + 0.6 ) / 9.6 ▼ %.3f ▼ (1 + %.3f) = %.3f kN/m\n", dL, dTankWeight, diT, dMl_Tank);
			RichEdit.AddText(sText);
		}

		if(pCalcData->m_bTrailer)
		{
			sText.Format("       - 戀觼お溯橾楝ж醞 瞳辨衛\n");
			RichEdit.AddText(sText);
			RichEdit.AddText("");
			sText.Format("         Pr  =  90 kN/m\n");
			RichEdit.AddText(sText);
			sText.Format("         Ml  =  ( %.3f + 0.6 ) / 9.6 ▼ %.3f ▼ (1 + %.3f) = %.3f kN/m (戀觼お溯橾楝 ж醞 濛辨衛)\n", dL, dTrailerWeight, diT, dMl_Trailer);
			RichEdit.AddText(sText);
		}
	}
	else
	{
		sText.Format("     �勒狫萺� 營ж腎雖 彊擠\n");
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

	sText.Format("    (3) Ё ж 醞 \n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	long nHDan = 0;

	if(nSide == FLOOR_LEFT)
		nHDan = 0;
	else if(nSide == FLOOR_RIGHT)
		nHDan = m_pStd->GetDataManage()->GetBridge()->GetQtyHDanNode() - 1;

	if(nSide==FLOOR_MID || !(pDB->GetValueTypeHDan(nHDan, 1) == 1))
	{
		sText.Format("      ≦ Ёж醞縑 渠и 頂辨檜 橈蝗棲棻.\n");
		RichEdit.AddText(sText);
		return;
	}

	if(dHH > 0)
	{
		sText.Format("      Pw  =  %.1f  ▼ H  =  %.1f  ▼  %.3f\n", dWindLoadForce, dWindLoadForce, dHH);	RichEdit.AddText(sText);
		sText.Format("            =  %.3f  kN/m\n", dPw);					RichEdit.AddText(sText);
		if(bWalkLoad)
		{
			sText.Format("      Mw  =  Pw ▼ ( H / 2 + t )  =  %.3f  ▼ ( %.3f / 2 + %.3f )\n", dPw, dHH, dWalkLoadH);	RichEdit.AddText(sText);
			sText.Format("             =  %.3f    kN﹞m/m\n", dMw);														RichEdit.AddText(sText);
		}
		else
		{
			sText.Format("      Mw  =  Pw ▼ ( H / 2 + t ) = %.3f  ▼ ( %.3f / 2 + %.3f )\n", dPw, dHH, dPaveT);	RichEdit.AddText(sText);
			sText.Format("            =  %.3f    kN﹞m/m\n", dMw);													RichEdit.AddText(sText);
		}
	}
}

// 夥款っ 醞懈睡 錳褕ж醞 匐饜
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
						//	2.4 ▼  5.75  /  ( 5.75 + 0.6 ) 								
	double	dTankV			= 40.0;
	double	dCf				= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
	double	dCf_Tank		= CalcGeneral.CalcCentriodLoad_CF(dTankV, dR);
	double	dCf_Trailer		= CalcGeneral.CalcCentriodLoad_CF(dTankV, dR);

	newElement.Format("    (4) 錳 褕 ж 醞\n\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);

	if(dR == 0.0)
	{
		newElement.Format("      ≦ 錳褕ж醞縑 渠и 頂辨檜 橈蝗棲棻.\n");
		RichEdit.AddText(newElement);
		return;
	}

	newElement.Format("      ≦ 撲啗樓紫  V  =  %.3f       R  = %.3f m\n", dV, dR);
	RichEdit.AddText(newElement);

	newElement.Format("         CF  =  0.79  ▼  V昌 /   R  (%%) =  0.79  ▼  %.3f昌 /   %.3f  =  %.3f %%\n", dV, dR, dCf);
	RichEdit.AddText(newElement);
	newElement.Format("        Mcf  =  Pcf▼1.80  =  (9.6/E)▼CF/100▼1.8  =  (9.6/%.3f) ▼ %.3f / 100▼ 1.8  =  %.3f kN.m/m\n", dE, dCf, dMcf);
	RichEdit.AddText(newElement);

	if(pCalcData->m_bTank)
	{
		dPr = pCalcData->m_TankWeight / 2 / pCalcData->m_TankEdit1;
		newElement.Format("      - 戀觼捨紫ж醞 濛辨衛");
		newElement.Format("        Mcf  =  Pr ▼ CF/100 ▼1.80  =  %.3f ▼ %.3f/100 ▼ 1.80  =  %.3f kN﹞m (戀觼捨紫ж醞濛辨衛)\n", dPr, dCf_Tank, dMcf_Tank);
		RichEdit.AddText(newElement);
	}

	if(pCalcData->m_bTrailer)
	{
		dPr = max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));

		newElement.Format("        Mcf  =  (Pr/E) ▼ CF/100 ▼1.80  =  (%.3f/%.3f) ▼ %.3f/100 ▼ 1.80 = %.3f kN﹞m (戀觼お溯橾楝ж醞濛辨衛)\n", dPr, dE, dCf_Trailer, dMcf_Trailer);
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
		newElement.Format("    (5) 離榆 醱給 ж醞        (Ｃ 紫.撲.п 2.4.3.5)\n\n");
		RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
		newElement.Format("       ≦ 離榆 醱給 ж醞 渠и 頂辨檜 橈蝗棲棻.");
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

		newElement.Format("    (5) 捱醞縑 曖и 蹺溘 傳朝 離榆曖 2離 醱給ж醞	       \n\n");
		RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
		newElement.Format("      ﹞ 熱ゎ醱給溘 H諦 熱霜溘 V陛 鼻欽睡諦 陪除 薑鼻睡縑 濛辨ж朝 匙戲煎 и棻.\n");
		RichEdit.AddText(newElement);
		newElement.Format("            熱ゎ 醱給溘   H  =  %.3f  kN/m\n", dHoriF);
		RichEdit.AddText(newElement);
		newElement.Format("            熱霜溘   V  =  %.3f  kN/m\n", dVeriF);
		RichEdit.AddText(newElement);
		newElement.Format("            濛辨 賅詮お  Mco = %.3f ▼ %.3f + %.3f ▼ %.3f =  %.3f kN﹞m/m\n", dHoriF, dH, dVeriF, dL, dMCo);
		RichEdit.AddText(newElement);	
	}
	else
	{
		newElement.Format("    (5) 離榆 醱給 ж醞        (Ｃ 紫.撲.п 2.4.3.5)\n\n");
		RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
		dVelocity	= pCalcData->m_dSpeed;
		dHoriForce	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Co;
		dMoment1	= dHoriForce*dH2;
		dMoment2	= 10.0 * dH;
		dMoment		= max(dMoment1,dMoment2);
			
		newElement.Format("    函 熱ゎ醱給溘  H陛 喻賊鼻 1m 堪檜縑 掖蹴擊 評塭 1m 葆棻 濛辨ж朝 匙戲煎 и棻.\n" );
		RichEdit.AddText(newElement);
		newElement.Format("      H = (V/60)昌 ▼ 750 + 250 = (%.3f/60)昌 ▼ 750 + 250 = %.3f N/m = %.3f kN/m\n", dVelocity, frkN(dHoriForce), dHoriForce);
		RichEdit.AddText(newElement);
		newElement.Format("      濛辨  堪檜   h = %.3f m\n ", dH2);
		RichEdit.AddText(newElement);
		newElement.Format("      濛辨賅詮お Mco = H ▼ h = %.3f ▼ %.3f = %.3f kN﹞m\n", dHoriForce, dH2, dMoment1);
		RichEdit.AddText(newElement+"\r\n");
		newElement.Format("    刻 寞�ㄩ� 鼻欽縑 �器磈熏虞� 10kN/m 曖 熱ゎж醞擊 營жи棻.\n");
		RichEdit.AddText(newElement);

		newElement.Format("      濛辨  堪檜   h = %.3f m \n", dH);
		RichEdit.AddText(newElement);
		newElement.Format("      濛辨賅詮お Mco = H ▼ h = 1.000 ▼ %.3f = %.3f kN﹞m\n", dH, dMoment2);
		RichEdit.AddText(newElement);

		if(dR>200 || dR==0)
		{
			if(dR==0)
			{
				newElement.Format("      堊睦奩雖葷  R = ＿ m ˇ 200.0 m 檜嘎煎, 函婁 刻曖 高 醞 贖 高擊 瞳辨и棻.\n", dR);
				RichEdit.AddText(newElement);
			}
			else
			{
				newElement.Format("     堊睦奩雖葷  R = %.1f m ˇ 200.0 m 檜嘎煎, 函婁 刻曖 高 醞 贖 高擊 瞳辨и棻.\n", dR);
				RichEdit.AddText(newElement);
			}
				
			newElement.Format("     Mco = %.3f kN﹞m",dMoment);
			RichEdit.AddText(newElement);
		}
		else
		{
			newElement.Format("     堊睦奩雖葷  R = %.1f m ‾ 200.0 m 檜嘎煎, 函婁 刻曖 高 醞 贖 高曖 2寡蒂 瞳辨и棻.\n", dR);
			RichEdit.AddText(newElement);
			newElement.Format("     Mco = %.3f kN﹞m\n",2.0*dMoment);
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

	sText.Format("    (4) 錳 褕 ж 醞 \n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	if(!bWalkLoad && dL>0 && dR>0)
	{
		sText.Format("      ≦  撲啗樓紫  V  =  %.3f km/hr       R  = %.3f\n", dV, dR);
		RichEdit.AddText(sText);
		dCf	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
		if(dR == 0) dCf = 0;

		if(nSide == FLOOR_LEFT || nSide == FLOOR_RIGHT)
		{
			sText.Format("      CF  =  0.79  ▼  V昌/  R  (%%) =  0.79  ▼  %.3f昌  /  %.3f  =  %.3f %%\n", dV, dR, dCf);
			RichEdit.AddText(sText);
			sText.Format("      Pcf  =  (Pr/E) ▼ CF/100  =  (%.3f/%.3f) ▼ %.3f/100  =  %.3f kN﹞m\n", dPr, dE, dCf, dPcf);
			RichEdit.AddText(sText);

			sText.Format("      - 橾奩�勒狫� 濛辨衛\n");
			RichEdit.AddText(sText);
			if(bTank || bTrailer)
				sText.Format("      Mcf  =  Pcf ▼ 1.80  =  %.3f ▼ 1.80  =  %.3f kN﹞m\n", dPcf, dMcf);
			else
				sText.Format("      Mcf  =  Pcf ▼ 1.80  =  %.3f ▼ 1.80  =  %.3f kN﹞m\n", dPcf, dMcf);
			RichEdit.AddText(sText);

			if(bTank)		// 戀觼捨紫ж醞 濛辨衛
			{
				dV			= 40;//戀觼釭 お溯橾楝朝 樓紫 40 堅薑..
				if(dR ==0)
				{
					dCf_Tank	= 0;					
				}
				else
				{
					dCf_Tank	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
				}
				
				sText.Format("      - 戀觼捨紫ж醞 濛辨衛\n");
				RichEdit.AddText(sText);
				sText.Format("      CF  =  0.79  ▼  V昌/  R  (%%) =  0.79  ▼  %.3f昌   /   %.3f  =  %.3f %%\n", dV, dR, dCf_Tank);
				RichEdit.AddText(sText);
				sText.Format("      Pcf  =  Pr ▼ CF/100  =  %.3f ▼ %.3f/100  =  %.3f kN﹞m\n", dPr_Tank, dCf_Tank, dPcf_Tank);
				RichEdit.AddText(sText);
				sText.Format("      Mcf  =  Pcf ▼ 1.80  =  %.3f ▼ 1.80  =  %.3f kN﹞m\n", dPcf_Tank, dMcf_Tank);
				RichEdit.AddText(sText);
			}
			if(bTrailer)	// 戀觼お溯橾楝 捨紫ж醞濛辨衛
			{
				dV			= 40;//戀觼釭 お溯橾楝朝 樓紫 40 堅薑..
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
				sText.Format("      - 戀觼お溯橾楝ж醞 濛辨衛\n");
				RichEdit.AddText(sText);
				sText.Format("      CF  =  0.79  ▼  V昌/  R  (%%) =  0.79  ▼  %.3f昌   /   %.3f  =  %.3f %%\n", dV, dR, dCf_Trailer);
				RichEdit.AddText(sText);
				sText.Format("      Pcf  =  (Pr/E) ▼ CF/100  =  (%.3f/%.3f) ▼ %.3f/100  =  %.3f kN﹞m\n", dPr_Trailer, dE3, dCf_Trailer, dPcf_Trailer);
				RichEdit.AddText(sText);
				sText.Format("      Mcf  =  Pcf ▼ 1.80  =  %.3f ▼ 1.80  =  %.3f kN﹞m\n", dPcf_Trailer, dMcf_Trailer);
				RichEdit.AddText(sText);
			}
		}
		else
		{
			sText.Format("      CF  =  0.79  ▼  V昌/  R  (%%) =  0.79  ▼  %.3f昌  /  %.3f  =  %.3f %%\n", dV, dR, dCf);
			RichEdit.AddText(sText);
			sText.Format("      Mcf  =  Pcf ▼ 1.8  =  (%.3f / E)	▼ CF/100 ▼ 1.8\n", dPr);
			RichEdit.AddText(sText);
			sText.Format("           =  ( %.3f / %.3f ) ▼ %.3f/100 ▼ 1.8  =  %.3f kN﹞m\n", dPr, dE, dCf, dMcf);
			RichEdit.AddText(sText);
		}
	}
	else
	{
		sText.Format("      ≦ 錳褕ж醞縑 渠и 頂辨檜 橈蝗棲棻.");
		RichEdit.AddText(sText);
	}
}

void CPlateStressOut::Output_TotalCombinationLoad(long nPos, CShRichEdit &RichEdit) 
{
	CCalcData			*pCalcData	= m_pStd->GetDataManage()->GetCalcData();

	CString newElement(_T(""));	

	double	dD   = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Stop_M);			// 餌ж醞
	double	dCo  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Coll_Mo);			// 醱給ж醞
	double	dCF  = tokNM((pCalcData->CALC_CANTILEVER_DATA[nPos].m_Cent_Mcf));				// 錳褕ж醞
	double	dCF2 = (pCalcData->CALC_CANTILEVER_DATA[nPos].m_Cent_Mcf_Tank);			// 錳褕ж醞(戀觼捨紫ж醞橾陽)
	double	dCF3 = (pCalcData->CALC_CANTILEVER_DATA[nPos].m_Cent_Mcf_Trailer);		// 錳褕ж醞(戀觼お溯橾楝ж醞橾陽)
	double	dLi  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_Ml);			// �勒狫�
	double	dLi2 = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_Ml_Tank);		// �勒狫�(戀觼捨紫ж醞橾陽)
	double	dLi3 = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_Ml_Trailer);	// �勒狫�(戀觼お溯橾楝ж醞橾陽)
	double	dW   = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Wind_Mw);			// Ёж醞
	double	dMu  = tokNM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Rein_Mu);

	newElement.Format("    (6) ж醞 褻м    ( Ｃ 紫.撲 2.2.3.2 )\n\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);

	newElement.Format("    - 譆渠 啗熱 賅詮お 啗骯\n");	
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, FALSE);

	if(nPos == 0 || nPos == 1) // ж醞褻м 謝難. 辦難
	{
		if(pCalcData->DESIGN_FLOOR_DATA[nPos].m_bWalkLoad) // 爾紫ж醞檜 氈朝 唳辦
		{
			// #APLATE-660, 啗骯衝曖 螃盟蒂 橈擁晦 嬪п 啗骯衝擊 熱薑и棻.
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

			newElement.Format("     函 Mu = 1.3D + 2.15(L+i)          = 1.3 ▼ %.3f + 2.15▼ %.3f                 = %.3f kN.m/m\n", dD, dLi, dMu_1);
			RichEdit.AddText(newElement);
			newElement.Format("     刻 Mu = 1.3D +  1.3(L+i) + 1.3CO  = 1.3 ▼ %.3f + 1.3 ▼ %.3f + 1.3  ▼ %.3f  = %.3f kN.m/m\n", dD, dLi, dCo, dMu_2);
			RichEdit.AddText(newElement);
			newElement.Format("     券 Mu = 1.3D +  1.3(L+i) + 0.65W  = 1.3 ▼ %.3f + 1.3 ▼ %.3f + 0.65 ▼ %.3f  = %.3f kN.m/m\n", dD, dLi, dW, dMu_3);
			RichEdit.AddText(newElement);
			newElement.Format("     刷 Mu = 1.2D + 1.2W + 1.2CO       = 1.2 ▼ %.3f + 1.2 ▼ %.3f + 1.2  ▼ %.3f  = %.3f kN.m/m\n", dD, dW, dCo, dMu_4);
			RichEdit.AddText(newElement);
			newElement.Format("     刺 Mu = 1.3D + 1.3W               = 1.3 ▼ %.3f + 1.3 ▼ %.3f                 = %.3f kN.m/m\n", dD, dW, dMu_5);
			RichEdit.AddText(newElement);
		}
		else
		{
			// #APLATE-660, 啗骯衝曖 螃盟蒂 橈擁晦 嬪п 摹⑽曖 鼻鷓縑 渠и 碟晦蒂 餉薯и棻.
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

			newElement.Format("     函 Mu = 1.3D + 2.15(L+i) + 1.3CF           = 1.3 ▼ %.3f + 2.15 ▼ %.3f + 1.3 ▼ %.3f                 = %.3f kN.m/m\n", dD, dLi, dCF, dMu_1);
			RichEdit.AddText(newElement);
			newElement.Format("     刻 Mu = 1.3D +  1.3(L+i) + 1.3CF + 1.3CO   = 1.3 ▼ %.3f + 1.3  ▼ %.3f + 1.3 ▼ %.3f + 1.3  ▼ %.3f  = %.3f kN.m/m\n", dD, dLi, dCF, dCo, dMu_2);
			RichEdit.AddText(newElement);
			newElement.Format("     券 Mu = 1.3D +  1.3(L+i) + 1.3CF + 0.65W   = 1.3 ▼ %.3f + 1.3  ▼ %.3f + 1.3 ▼ %.3f + 0.65 ▼ %.3f  = %.3f kN.m/m\n", dD, dLi, dCF, dW, dMu_3);
			RichEdit.AddText(newElement);
			newElement.Format("     刷 Mu = 1.2D + 1.2W + 1.2CO                = 1.2 ▼ %.3f + 1.2  ▼ %.3f + 1.2 ▼ %.3f                 = %.3f kN.m/m\n", dD, dW, dCo, dMu_4);
			RichEdit.AddText(newElement);
			newElement.Format("     刺 Mu = 1.3D + 1.3W                        = 1.3 ▼ %.3f + 1.3  ▼ %.3f                               = %.3f kN.m/m\n", dD, dW, dMu_5);
			RichEdit.AddText(newElement);

			if(pCalcData->m_bTank)
			{
				double dMu_6 = 1.3 * dD + 1.3 * dLi2 + 1.3 * dCF2;
				dMu = dMu > dMu_6 ? dMu : dMu_6;

				newElement.Format("     到 Mu = 1.3D + 1.3(L+i) + 1.3CF (戀觼捨紫ж醞濛辨衛)      = 1.3 ▼ %.3f + 1.3 ▼ %.3f + 1.3 ▼ %.3f   = %.3f kN.m/m\n", dD, dLi2, dCF2, dMu_6);
				RichEdit.AddText(newElement);
			}

			if(pCalcData->m_bTrailer)
			{
				double dMu_7 = 1.3 * dD + 1.3 * dLi3 + 1.3 * dCF3;
				dMu = dMu > dMu_7 ? dMu : dMu_7;

				if(!pCalcData->m_bTank)
				{
					newElement.Format("     到 Mu = 1.3D + 1.3(L+i) + 1.3CF (戀觼お溯橾楝ж醞濛辨衛)  = 1.3 ▼ %.3f + 1.3 ▼ %.3f + 1.3 ▼ %.3f   = %.3f kN.m/m\n", dD, dLi3, dCF3, dMu_7);
					RichEdit.AddText(newElement);
				}
				else
				{
					newElement.Format("     刮 Mu = 1.3D + 1.3(L+i) + 1.3CF (戀觼お溯橾楝ж醞濛辨衛)  = 1.3 ▼ %.3f + 1.3 ▼ %.3f + 1.3 ▼ %.3f   = %.3f kN.m/m\n", dD, dLi3, dCF3, dMu_7);
					RichEdit.AddText(newElement);
				}
			}
		}
		newElement.Format("   王 譆渠 啗熱 賅詮お朝 ж醞褻м 醞縑憮 陛濰 贖 Mu = %.3f kN.m/m 煎 欽賊 匐饜и棻. \n\n", dMu);
		RichEdit.AddText(newElement);
	}
	else if(nPos == 1)
	{
		newElement.Format("    < 警せ溯幗っ 欽睡撲啗 >\n\n");
		RichEdit.AddText(newElement);
		newElement.Format("   函 輿繹斬榆 : 菟爾 欽睡 檜諼曖 騰せ溯幗睡 夥款っ縑憮 в蹂и 輿繹斬榆曖 2寡蒂 輿繹斬戲煎 寡斬и棻.\n");
		RichEdit.AddText(newElement);
		newElement.Format("   刻 寡溘繹斬榆 : 菟爾 欽睡 檜諼曖 騰せ溯幗睡 夥款っ曖 鼻蹴縑 寡溘繹斬榆曖 2寡蒂 寡溘繹斬戲煎 寡斬и棻.\n");
		RichEdit.AddText(newElement);
		newElement.Format("   券 紫煎掖 衛寞憮 鬼掖ら 夥款っ縑 寡纂彰嬪陛 貲衛腎橫 氈雖 彊戲嘎煎 夔觼葬おら\n");
		RichEdit.AddText(newElement);
		newElement.Format("      Ｃ紫.衛 P453 擊 瞳辨ж罹 騰せ溯幗っ曖 餌ж醞縑 渠и 雖除擊 寡纂彰嬪煎 瞳辨и棻.\n");
		RichEdit.AddText(newElement);	
	}
	else if(nPos == 2)		// ж醞褻м 醞懈睡
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

		// #APLATE-660, 啗骯衝曖 螃盟蒂 橈擁晦 嬪п 摹⑽曖 鼻鷓縑 渠и 碟晦蒂 餉薯и棻.
		newElement.Format("     函 Mu = 1.3D + 2.15(L+i) + 1.3CF  = 1.3 ▼ %.3f + 2.15 ▼ %.3f + 1.3 ▼ %.3f   = %.3f  kN.m/m\n",	dD, dML1, dMcf1, dMu_1);
		RichEdit.AddText(newElement);

		if(pCalcData->m_bTank)
		{
			dMu_1 = dMu_1 > dMu_2 ? dMu_1 : dMu_2;

			newElement.Format("     刻 Mu = 1.3D + 1.3(L+i) + 1.3CF  = 1.3 ▼ %.3f +  1.3 ▼ %.3f + 1.3 ▼ %.3f   = %.3f  kN.m/m( 戀觼捨紫ж醞濛辨衛 )\n", dD, dML2, dMcf2, dMu_2);
			RichEdit.AddText(newElement);
		}

		if(pCalcData->m_bTrailer)
		{
			dMu_1 = dMu_1 > dMu_3 ? dMu_1 : dMu_3;

			if(!pCalcData->m_bTank)
				newElement.Format("     刻 Mu = 1.3D + 1.3(L+i) + 1.3CF  = 1.3 ▼ %.3f +  1.3 ▼ %.3f + 1.3 ▼ %.3f   = %.3f   kN.m/m( 戀觼お溯橾楝ж醞濛辨衛 )\n", dD, dML3, dMcf3, dMu_3);
			else
				newElement.Format("     券 Mu = 1.3D + 1.3(L+i) + 1.3CF  = 1.3 ▼ %.3f +  1.3 ▼ %.3f + 1.3 ▼ %.3f   = %.3f   kN.m/m( 戀觼お溯橾楝ж醞濛辨衛 )\n", dD, dML3, dMcf3, dMu_3);
			RichEdit.AddText(newElement);
		}

		newElement.Format("   王 譆渠 啗熱 賅詮お朝 ж醞褻м 醞縑憮 陛濰 贖 Mu = %.3f kN.m/m 煎 欽賊 匐饜и棻. \n\n", dMu_1);
		RichEdit.AddText(newElement);
	}

	// 2014. 12. 26 #APLATE-660, 譆渠 餌辨 賅詮お ж醞 褻м擊 蹺陛и棻.
	newElement.Format("    - 譆渠 餌辨 賅詮お 啗骯\n");	
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, FALSE);

	double	dMu_Use	= Round(dD + dLi + 0.3 * dW, 3);

	newElement.Format("     函 Ms = D + Li + 0.3W = %.3f + %.3f + 0.3 ▼ %.3f = %.3f kN.m/m\n", dD, dLi, dW, dMu_Use);
	RichEdit.AddText(newElement);

	newElement.Format("   王 譆渠 餌辨 賅詮お朝 ж醞褻м 醞縑憮 陛濰 贖 Ms = %.3f kN.m/m 煎 敕翮 匐饜и棻. \n\n", dMu_Use);
	RichEdit.AddText(newElement);
}

//蝸楚粽譆模舒眷匐饜
void CPlateStressOut::Output_SlabThickness(long nPos, CShRichEdit &RichEdit) 
{
	CPlateBridgeApp		*pDB	= m_pStd->GetBridge();
	CCalcData			*pCalcData	= m_pStd->GetDataManage()->GetCalcData();
	BOOL bUsePreStress  = pDB->m_nSlabTensionSize==0 ? FALSE : TRUE;
	BOOL bWalkLoad		= pCalcData->DESIGN_FLOOR_DATA[nPos].m_bWalkLoad;	

	double  dLastCheck	= 220;//離紫睡 夥款っ曖 譆模舒眷朝 220
	if(bUsePreStress)	dLastCheck = 200;//Щ葬蝶お溯蝶 瞳辨檜賊...(模熱輿剪渦掖)

	CString sText	= _T("");
	CString sign(_T("")), sign2(_T("")), signDet(_T(""));
	double	dL	= toM(pCalcData->CALC_CANTILEVER_DATA[nPos].m_Move_L);	// �勒狫萷� 曖и 雖除
	double	dT	= pCalcData->CALC_CANTILEVER_DATA[nPos].m_Req_T;		// 譆夥款っ 譆模舒眷
	double	dmT	= 0.25;
	long	nGirdersu	= pDB->GetGirdersu();

	RichEdit.AddText("\n");
	sText.Format("    (1) 夥款っ曖 譆模 舒眷             (Ｃ 紫.撲.晦 3.6.1.5)\n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

	if(bWalkLoad)
	{
		sText = "       爾紫睡 夥款っ曖 譆模 舒眷朝 140 mm蒂 ル遽戲煎 и棻.\n";
		RichEdit.AddText(sText);
	}
	else if(dL > 0)
	{
	
		double dTB	= pDB->m_dThickSlabBase;

		//器 檜勘 貍詳棻棲..荼...070307....KB...
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

		sText.Format("      函 �勒狫萷� 曖и 雖除 望檜 ( L )\n");
		RichEdit.AddText(sText);

		if(nPos == FLOOR_LEFT || nPos== FLOOR_RIGHT)	// 謝.辦難 譆模舒眷 匐饜
		{
			if(dL > dmT)	
			{
				sign = _T(">");
				sText.Format("       L  =  %.3f  %s  0.25    雖除望檜陛 0.25爾棻 觼嘎煎\n", dL, sign);
				RichEdit.AddText(sText);
				sText.Format("      ˍ T  =  80 ▼  L  +  230\n\n");
				RichEdit.AddText(sText);
				if(bUsePreStress)	sText.Format("      刻 譆模舒眷 T  =  (80  ▼  %.3f  +  230) ▼ 0.9  =  %.1f mm檜鼻\n", dL, dT);
				else				sText.Format("      刻 譆模舒眷 T  =  80  ▼  %.3f  +  230  =  %.1f mm檜鼻\n", dL, dT);
				RichEdit.AddText(sText);
			}
			else
			{
				sign = _T("‾");
				sText.Format("      L  =  %.3f  %s  0.25    雖除望檜陛 0.25爾棻 濛剪釭 偽戲嘎煎\n\n",	dL, sign );
				RichEdit.AddText(sText);
				sText.Format("      ˍ T  =  280  ▼  L  +  180 瞳辨\n");
				RichEdit.AddText(sText);
				if(bUsePreStress)	sText.Format("      刻 譆模舒眷 T  =  (280 ▼  %.3f + 180) ▼ 0.9  =  %0.1f mm檜鼻\n", dL, dT );
				else				sText.Format("      刻 譆模舒眷 T  =  280 ▼  %.3f + 180  =  %.1f mm檜鼻\n", dL, dT );
				RichEdit.AddText(sText);
				
			}
		}
		else if (nGirdersu > 2)//醞懈睡檜堅 翱樓っ檜賊..
		{
			sText.Format("       L = %.3f m\n", dL);
			RichEdit.AddText(sText);
			sText.Format("      ˍ T  =  30  ▼  L  +  130 瞳辨\n");
			RichEdit.AddText(sText);
			if(bUsePreStress)	sText.Format("      刻 譆模舒眷 T  =  (30 ▼  %.3f + 130) ▼ 0.9  =  %0.1f mm檜鼻\n", dL, dT );
			else				sText.Format("      刻 譆模舒眷 T  =  30 ▼  %.3f + 130  =  %.1f mm檜鼻\n", dL, dT );
			RichEdit.AddText(sText);			
		}
		else
		{
			sText.Format("       L = %.3f m\n", dL);
			RichEdit.AddText(sText);
			sText.Format("      ˍ T  =  40  ▼  L  +  130 瞳辨\n");
			RichEdit.AddText(sText);
			if(bUsePreStress)	sText.Format("      刻 譆模舒眷 T  =  (40 ▼  %.3f + 130) ▼ 0.9  =  %0.1f mm檜鼻\n", dL, dT );
			else				sText.Format("      刻 譆模舒眷 T  =  40 ▼  %.3f + 130  =  %.1f mm檜鼻\n", dL, dT );
			RichEdit.AddText(sText);	
		}

		sText.Format("         T = %.1f mm  %s   %.1f mm 餌辨 .... ", dT, sign2, dTB);
		RichEdit.AddText(sText);
		RichEdit.AddMsg(signDet, RE_RED, FALSE, TRUE);
		RichEdit.AddText("");

		if(bUsePreStress)
		{
			sText = "      夥款っ曖 雖除寞щ戲煎 Prestress紫殮衛 啗骯榆曖 90% 瞳辨.     (Ｃ 紫.撲.晦 4.7.4.2) \n";
			RichEdit.AddText(sText);
			sText = "      譆模舒眷 啗骯高婁 200 mm 醞縑憮 贖高擊 餌辨и棻. (Ｃ 紫.撲.晦 4.7.4.1) \n";
			RichEdit.AddText(sText);
		}
		else
		{			
			sText = "      譆模舒眷 啗骯高婁 220 mm 醞縑憮 贖高擊 餌辨и棻. (Ｃ 紫.撲.晦 3.6.1.5) \n";
			RichEdit.AddText(sText);
		}
	}
	else
	{
		sText.Format("       �勒狫萺� 橈戲嘎煎 離紫睡 夥款っ 譆模舒眷 %.0fmm 檜鼻擊 餌辨\n",dLastCheck );
		RichEdit.AddText(sText);
	}
}

// 鬼摹檜 氈擊 唳辦 夥款っ 譆模 舒眷 撲薑
void CPlateStressOut::Output_SlabThickness_Tendon(CShRichEdit &RichEdit)
{	
	CString sText	= _T("");

	RichEdit.AddText("\n");
	sText.Format("   (1)	夥款っ曖 譆模 舒眷                                                 ( Ｃ 紫.撲.п 4.7.4.2 )\n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	sText.Format("     函  獄 掖榆擎 濰雖除 夥款っ縑 Щ葬蝶お溯蝶蒂 紫殮и 掖榆戲煎憮, 欽雖除 夥款っ爾棻 舒眷陛 舒皎晦\n");
	RichEdit.AddText(sText);

	sText.Format("         陽僥縑 奩犒腎朝 お楣ж醞縑 曖и べ蘆 だ惚陛 嫦儅ж晦 橫滑棻. 評塭憮 夥款っ曖 譆模舒眷 匐饜衛\n");
	RichEdit.AddText(sText);
	sText.Format("         撲啗ж醞縑 渠и 寰瞪撩 塽 醱問擊 んли �勒狫萷� 渠и 餌辨撩擊 虜褶ж朝 熱遽戲煎 夥款っ 譆模\n");
	RichEdit.AddText(sText);
	sText.Format("         舒眷蒂 唸薑и棻.\n");
	RichEdit.AddText(sText);
	sText.Format("         ( お楣 彌ж醞擊 餵醞ж醞戲煎 賅筐葭ж罹 п戮ж朝 唳辦 ж醞營ж 嬪纂縑憮 �� 賅詮お陛 婁渠ゎ陛\n");
	RichEdit.AddText(sText);
	sText.Format("         腎嘎煎 薑塵 п戮衛 お楣 彌ж醞曖 蕾雖賊瞳擊 堅溥ж罹 п戮鼻 螃盟蒂 譆模�� ж罹撿 и棻. )\n\n");
	RichEdit.AddText(sText);
	sText.Format("     刻  お楣 彌ж醞曖 蕾雖賊瞳擊 堅溥ж朝 寞徹擎 '紫.撲.п 3.6.1.4 夥款っ曖 撲啗 �皒薷鉐�'縑憮 彌ж醞 碟ん蒂\n");
	RichEdit.AddText(sText);
	sText.Format("         堅溥п憮 夥款っ縑 濛辨ж朝 �皒薷鉐挫� 觼晦蒂 除楞衝擊 檜辨ж罹 骯薑й 熱 氈戲釭, お楣 彌ж醞曖 蕾雖賊瞳擊\n");
	RichEdit.AddText(sText);
	sText.Format("         堅溥и 薑塵и п戮 寞徹擊 餌辨ж朝 唳辦縑朝 除楞衝擊 餌辨ж雖 彊嬴紫 脹棻. 薑塵п戮衛 蹂掘腎朝 お楣\n");
	RichEdit.AddText(sText);
	sText.Format("         彌ж醞曖 蕾雖賊瞳擎 '紫.撲.п 2.1.3 �勒狫�'擊 霤褻и棻.\n");
	RichEdit.AddText(sText);
}

// 醞懈睡 唳я瞳 撲啗
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
	double	dSlabThick				= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;//pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Req_T;	// 夥款っ 譆模 舒眷
	double	dLengthCen				= CalcGeneral.CalcCentPart_L();						// 醞懈睡 嶸�膨磍�
	double	dLengthLeft				= pDB->m_dWidthSlabLeft - pGirFir->GetThickJewon(G_W, pBxFir)/2;	// 謝難 警せ溯幗睡 部欽縑憮 羅廓簞剪渦 謝難犒睡っ 醞懈梱雖 剪葬
	double	dLengthRight			= pDB->m_dWidthSlabRight - pGirEnd->GetThickJewon(G_W, pBxEnd)/2;	// 辦難 警せ溯幗睡 部欽縑憮 葆雖虞剪渦 辦難犒睡っ 醞懈梱雖 剪葬
	double	dCoverUp				= pCalcData->m_dCoverUp;							// 繹斬竣啪(鼻賊)
	double	dCoverDn				= pCalcData->m_dCoverDn;							// 繹斬竣啪(ж賊)
	double	dRate					= dLengthCen/dSlabThick;
	double	dMainDiaUpper_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy];	// 輿繹斬 鼻賊 Dia Idx 1-Cylce
	double	dMainDiaUpper_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy];	// 輿繹斬 鼻賊 Dia Idx 2-Cycle
	double	dMainCTCUpper			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1;					// 輿繹斬 鼻賊 除問
	double	dMainDiaLower_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1];			// 輿繹斬 ж賊 Dia Idx 1-Cylce
	double	dMainDiaLower_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy];		// 輿繹斬 ж賊 Dia Idx 2-Cycle
	double	dMainCTCLower			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1;						// 輿繹斬 ж賊 除問
	double	dBaeRuckDiaUpper		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2];		// 寡溘繹斬 鼻賊 Dia Idx
	double	dBaeRuckCTCUpper		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2;					// 寡溘繹斬 鼻賊 除問
	double	dBaeRuckDiaLower		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3];			// 寡溘繹斬 ж賊 Dia Idx
	double	dBaeRuckCTCLower		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB;						// 寡溘繹斬 ж賊 除問
	double	dEndReinforceDiaMain_1Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2];			// 欽睡爾鬼 輿繹斬 Dia Idx 1-Cycle
	double	dEndReinforceDiaMain_2Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy];		// 欽睡爾鬼 輿繹斬 Dia Idx 2-Cycle
	double	dEndReinforceCTCMain	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// 欽睡爾鬼 輿繹斬 除問
	double	dEndReinforceDiaBeryuk	= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4];			// 欽睡爾鬼 寡溘繹斬 Dia Idx
	double	dEndReinforceCTCBeryuk	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1;						// 欽睡爾鬼 寡溘繹斬 除問
	double	dSlabArea				= 1000*dSlabThick;
	double	dArea					= 0;
	double	dArea_1Cy				= 0;
	double	dArea_2Cy				= 0;
	double	dAsreq					= 0;
	CString	sLeftGuard				= _T("");
	CString	sRightGuard				= _T("");
	CString	sRebar					= pCalcData->DESIGN_MATERIAL.m_sSigmaY;

	CCentSeparation	*pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::LEFT);
	if(pCentSeparation)	sLeftGuard	= _T("寞��疇 氈擠");
	else				sLeftGuard	= _T("寞��疇 橈擠");
	pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
	if(pCentSeparation)	sRightGuard	= _T("寞��疇 氈擠");
	else				sRightGuard	= _T("寞��疇 橈擠");

	szText.Format("    < 醞懈睡 蝸楚粽 繹斬榆 骯薑 >\n\n");
	RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	szText.Format("  (1) 撲啗褻勒\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("     夥款っ 舒眷 : %.0f mm     嶸�螟鰤� : %.0f mm     繹斬竣偃(鼻賊) : %.0f mm     繹斬竣偃(ж賊) : %.0f mm\n\n",
					dSlabThick, dLengthCen, dCoverUp, dCoverDn);
	RichEdit.AddText(szText);
	szText.Format("     函 雖雖睡營 : 鬼營 輿剪渦\n");
	RichEdit.AddText(szText);
	szText.Format("     刻 夔觼葬お陛 ⑷濰顫撲腎堅 蝗彌曄儅腎橫撿 л.\n");
	RichEdit.AddText(szText);
	szText.Format("     券 輿剪渦 Ы楠雖睡曖 ь纂諦 偽檜 措睡瞳戲煎 舒皎啪 и 夠擊 薯諼и 鼻鷓縑憮 瞪羹瞳戲煎 夥款っ曖 舒眷陛 橾薑л.\n");
	RichEdit.AddText(szText);
	szText.Format("     刷 夥款っ 舒眷縑 渠и 嶸�螟鰤�曖 綠陛 6 檜鼻 15 檜ж\n");
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
	szText.Format("     刺 夥款っ曖 鼻睡諦 ж睡縑 寡斬脹 繹斬曖 諼難賊 餌檜曖 舒眷陛 150 mm 檜鼻\n");
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
	
	szText.Format("     到 嶸�螟鰤�檜 ル遽離摹ァ 3.6m 檜ж\n");
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
	szText.Format("     刮 夥款っ曖 �嚌�, 葆賅賊, 斜葬堅 爾�ㄤ什傢�擊 薯諼и 夥款っ曖 譆模舒眷陛 240 mm\n");
	RichEdit.AddText(szText);
	szText.Format("     制 警せ溯幗睡曖 望檜陛 頂難夥款っ 舒眷曖 5寡 檜鼻檜剪釭, 警せ溯幗睡曖 望檜陛 頂難\n");
	RichEdit.AddText(szText);
	szText.Format("         夥款っ 舒眷曖 3寡 檜鼻檜堅 掘褻瞳戲煎 翱樓瞳檣 夔觼葬お 寞��疇婁 м撩檜 脹 唳辦\n");
	RichEdit.AddText(szText);
	if(sLeftGuard == _T("寞��疇 氈擠"))
	{
		if(toM(dLengthLeft) > toM(dSlabThick)*3)
		{
			szText.Format("         Ⅰ 謝難 警せ溯幗 望檜 (%s) :  %.3f m  >  %.3f  ▼  3  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         Ⅰ 謝難 警せ溯幗 望檜 (%s) :  %.3f m  <  %.3f  ▼  3  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}
	else
	{
		if(toM(dLengthLeft) > toM(dSlabThick)*5)
		{
			szText.Format("         Ⅰ 謝難 警せ溯幗 望檜 (%s) :  %.3f m  >  %.3f  ▼  5  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         Ⅰ 謝難 警せ溯幗 望檜 (%s) :  %.3f m  <  %.3f  ▼  5  =  %.3f m", sLeftGuard, toM(dLengthLeft), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}

	if(sRightGuard == _T("寞��疇 氈擠"))
	{
		if(toM(dLengthRight) > toM(dSlabThick)*3)
		{
			szText.Format("         Ⅰ 辦難 警せ溯幗 望檜 (%s) :  %.3f m  >  %.3f  ▼  3  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         Ⅰ 辦難 警せ溯幗 望檜 (%s) :  %.3f m  <  %.3f  ▼  3  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*3);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}
	else
	{
		if(toM(dLengthRight) > toM(dSlabThick)*5)
		{
			szText.Format("         Ⅰ 辦難 警せ溯幗 望檜 (%s) :  %.3f m  >  %.3f  ▼  5  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... O.K\n");
			RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
		}
		else
		{
			szText.Format("         Ⅰ 辦難 警せ溯幗 望檜 (%s) :  %.3f m  <  %.3f  ▼  5  =  %.3f m", sRightGuard, toM(dLengthRight), toM(dSlabThick), toM(dSlabThick)*5);
			RichEdit.AddText(szText);
			szText.Format("    ... N.G\n");
			RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
		}
	}

	szText.Format("     剁 夔觼葬お曖 28橾 憶蹴鬼紫朝 %.0f MPa 檜鼻\n", pCalcData->DESIGN_MATERIAL.m_dSigmaCK);
	RichEdit.AddText(szText);
	szText.Format("     劾 繹斬 夔觼葬お 夥款っ擎 夥款っ擊 雖雖ж朝 掘褻睡營菟婁 諫瞪м撩剪翕\n\n");
	RichEdit.AddText(szText);
	szText.Format("  (2) 繹斬寡斬榆\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("     函 雖除寞щ  :\n");
	RichEdit.AddText(szText);
	szText.Format("        ж睡繹斬榆 - 夔觼葬お 夥款っ 欽賊曖 0.4%% 檜鼻\n");
	RichEdit.AddText(szText);
	szText.Format("        鼻睡繹斬榆 - 夔觼葬お 夥款っ 欽賊曖 0.3%% 檜鼻\n");
	RichEdit.AddText(szText);
	szText.Format("     刻 雖除寞щ縑 霜陝寞щ  :\n");
	RichEdit.AddText(szText);
	szText.Format("        ж睡繹斬榆 - 夔觼葬お 夥款っ 欽賊曖 0.3%% 檜鼻\n");
	RichEdit.AddText(szText);
	szText.Format("        鼻睡繹斬榆 - 夔觼葬お 夥款っ 欽賊曖 0.3%% 檜鼻\n\n");
	RichEdit.AddText(szText);

	szText.Format("  (3) 餌辨繹斬榆\n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("      函 橾奩睡 輿繹斬\n");
	RichEdit.AddText(szText);
	dArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1_1Cy;
	dArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1_2Cy;
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1;
	szText.Format("         鼻賊繹斬     в蹂繹斬榆  Asreq  :  %.0f  ▼  0.3%%  =  %.3f  mm昌\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f   mm昌\n", sRebar, dMainDiaUpper_1Cy, dMainCTCUpper, dArea_1Cy);
	RichEdit.AddText(szText);
	szText.Format("                                                         %s%.0f  @ %.0f   =  %.3f   mm昌\n", sRebar, dMainDiaUpper_2Cy, dMainCTCUpper, dArea_2Cy);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                                                      м啗 %.3f mm昌  >  Asreq ( %.3f mm昌 )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                                                      м啗 %.3f mm昌  >  Asreq ( %.3f mm昌 )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	dArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs_1Cy;
	dArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs_2Cy;
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asuse;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asreq;
	szText.Format("         ж賊繹斬     в蹂繹斬榆  Asreq  :  %.0f  ▼  0.4%%  =  %.3f  mm昌\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f   mm昌\n", sRebar, dMainDiaLower_1Cy, dMainCTCLower, dArea_1Cy);
	RichEdit.AddText(szText);
	szText.Format("                                                         %s%.0f  @ %.0f   =  %.3f   mm昌\n", sRebar, dMainDiaLower_2Cy, dMainCTCLower, dArea_2Cy);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                                                      м啗 %.3f mm昌  >  Asreq ( %.3f mm昌 )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                                                      м啗 %.3f mm昌  >  Asreq ( %.3f mm昌 )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}

	szText.Format("        刻 橾奩睡 寡溘繹斬\n");
	RichEdit.AddText(szText);
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse2;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1;
	szText.Format("         鼻賊繹斬     в蹂繹斬榆  Asreq  :  %.0f  ▼  0.3%%  =  %.3f  mm昌\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm昌", sRebar, dBaeRuckDiaUpper, dBaeRuckCTCUpper, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm昌", sRebar, dBaeRuckDiaUpper, dBaeRuckCTCUpper, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsuse;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsreq;
	szText.Format("         ж賊繹斬     в蹂繹斬榆  Asreq  :  %.0f  ▼  0.3%%  =  %.3f  mm昌\n", dSlabArea, dAsreq);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm昌", sRebar, dBaeRuckDiaLower, dBaeRuckCTCLower, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm昌", sRebar, dBaeRuckDiaLower, dBaeRuckCTCLower, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}

	szText.Format("        券 欽睡 輿繹斬\n");
	RichEdit.AddText(szText);
	dArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_1Cy;
	dArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_2Cy;
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asreq*2;
	szText.Format("                          в蹂繹斬榆  Asreq  :  %.0f  ▼  2寡  =  %.3f  mm昌\n", dAsreq/2, dAsreq);
	RichEdit.AddText(szText);
	szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f   mm昌\n", sRebar, dEndReinforceDiaMain_1Cy, dEndReinforceCTCMain, dArea_1Cy);
	RichEdit.AddText(szText);
	szText.Format("                                                         %s%.0f  @ %.0f   =  %.3f   mm昌\n", sRebar, dEndReinforceDiaMain_2Cy, dEndReinforceCTCMain, dArea_2Cy);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                                                      м啗 %.3f mm昌  >  Asreq ( %.3f mm昌 )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                                                      м啗 %.3f mm昌  >  Asreq ( %.3f mm昌 )", dArea, dAsreq);
		RichEdit.AddText(szText);
		szText.Format("    ... N.G\n");
		RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	}

	szText.Format("        刷 欽睡 寡溘繹斬\n");
	RichEdit.AddText(szText);
	dArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_BAsuse2;
	dAsreq		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsreq*2;
	szText.Format("                              в蹂繹斬榆  Asreq  :  %.0f  ▼  2寡  =  %.3f  mm昌\n", dAsreq/2, dAsreq);
	RichEdit.AddText(szText);
	if(dArea > dAsreq)
	{
		szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm昌", sRebar, dEndReinforceDiaBeryuk, dEndReinforceCTCBeryuk, dArea);
		RichEdit.AddText(szText);
		szText.Format("    ... O.K\n");
		RichEdit.AddMsg(szText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		szText.Format("                          餌辨繹斬榆  Asuse  :  %s%.0f  @ %.0f   =  %.3f  mm昌", sRebar, dEndReinforceDiaBeryuk, dEndReinforceCTCBeryuk, dArea);
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

	strText.Format("    (1) 橾奩睡\n\n");		RichEdit.AddMsg(strText, RE_BLACK, FALSE, TRUE);
	strText.Format("      - 輿繹斬\n");			RichEdit.AddText(strText);
	strText.Format("      1) 斛иж醞 Mu  =  %.3f kN﹞m\n", tokNM(MapStress["UltimateLoad"]));			RichEdit.AddText(strText);
	strText.Format("      2) 撲啗 �痚音�       ( Ｃ 紫.撲.晦 4.6.3.5 )\n");								RichEdit.AddText(strText);
	strText.Format("         函 PS鬼營曖 擬溘 : 睡雜脹 睡營  fps  =  %.3f MPa\n", MapStress["Fps"]);	RichEdit.AddText(strText);
	strText.Format("            檣濰 :   %s%s   @   %.0f   ╮   %.3f mm昌  (1 Cycle)\n", sRebar, strGenTDia_1Cy, dGenTCTC, dGenTAsuse_1Cy);		RichEdit.AddText(strText);
	strText.Format("                       %s%s   @   %.0f   ╮   %.3f mm昌  (2 Cycle)\n", sRebar, strGenTDia_2Cy, dGenTCTC, dGenTAsuse_2Cy);	RichEdit.AddText(strText);
	strText.Format("                                м啗  : %.3f mm昌\n", dGenTAsuse);	RichEdit.AddText(strText);
	strText.Format("            憶蹴 :   %s%s   @   %.0f   ╮   %.3f mm昌  (1 Cycle)\n", sRebar, strGenCDia_1Cy, dGenCCTC, dGenCAsuse_1Cy);		RichEdit.AddText(strText);
	strText.Format("                       %s%s   @   %.0f   ╮   %.3f mm昌  (2 Cycle)\n", sRebar, strGenCDia_2Cy, dGenCCTC, dGenCAsuse_2Cy);	RichEdit.AddText(strText);
	strText.Format("                                м啗  : %.3f mm昌\n", dGenCAsuse);														RichEdit.AddText(strText);
	strText.Format("            檣濰繹斬綠 伏 : %.6f      憶蹴繹斬綠 伏' : %.6f\n", MapStress["Rate_Tension"], MapStress["Rate_Compre"]);	RichEdit.AddText(strText);
	strText.Format("         刻 Ы楠雖蒂 乾朝 欽賊 �挫垥n");			RichEdit.AddText(strText);
	strText.Format("            1.4 ▼ dp ▼ 伏p ▼ fps / fck  =  1.4 ▼ %.0f ▼ %.6f ▼ %.3f / %.0f\n", MapStress["Depth_PS"], MapStress["Rho_PS"], MapStress["Fps"], dFck);
	RichEdit.AddText(strText);
	if(MapStress["FlangeCheck"]<MapStress["Slab_Thick"])
		strText.Format("            = %.3f mm  <  %.3f mm   檜嘎煎 掘⑽欽賊戲煎 匐饜\n", MapStress["FlangeCheck"], MapStress["Slab_Thick"]);
	else
		strText.Format("            = %.3f mm  >  %.3f mm   檜嘎煎 Ы楠雖蒂 乾朝 欽賊戲煎 匐饜\n", MapStress["FlangeCheck"], MapStress["Slab_Thick"]);
	RichEdit.AddText(strText);
	strText.Format("         券 �痚音琵n");													RichEdit.AddText(strText);
	strText.Format("            白Mn  =  白 ▼ { Ap▼Fps▼dp▼(1-0.59▼Pp▼fps/fck) }\n");	RichEdit.AddText(strText);
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
	strText.Format("         刷 譆渠 鬼營綠\n");	RichEdit.AddText(strText);
	if(MapStress["MaxSteelRate"]<MapStress["MinQp"]){
		strText.Format("            qp  =  Pp ▼ ( fps / fck ) = %.3f < 0.36﹞乒1 = %.3f", MapStress["MaxSteelRate"], MapStress["MinQp"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, TRUE, 1);
	}
	else{
		strText.Format("            qp  =  Pp ▼ ( fps / fck ) = %.3f > 0.36﹞乒1 = %.3f", MapStress["MaxSteelRate"], MapStress["MinQp"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, FALSE, 1);
		}	
	strText.Format("         刺 譆模 鬼營榆\n");										RichEdit.AddText(strText);
	strText.Format("            ﹞敕翮 賅詮お Mcr = %.3f kN.m\n", MapStress["Mcr"]);	RichEdit.AddText(strText);
	if(MapStress["Bend_Mn"]/MapStress["Mcr"]>1.2){
		strText.Format("               白Mn/Mcr = %.3f / %.3f = %.3f > 1.2", tokNM(MapStress["Bend_Mn"]), tokNM(MapStress["Mcr"]), MapStress["Bend_Mn"]/MapStress["Mcr"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, TRUE, 2);
	}
	else{
		strText.Format("               白Mn/Mcr = %.3f / %.3f = %.3f < 1.2", tokNM(MapStress["Bend_Mn"]), tokNM(MapStress["Mcr"]), MapStress["Bend_Mn"]/MapStress["Mcr"]);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, FALSE, 2);
	}	
	strText.Format("      - 寡溘繹斬\n");							RichEdit.AddText(strText);
	strText.Format("         函 [紫煎掖撲啗晦遽] 113Page\n");		RichEdit.AddText(strText);
	strText.Format("           ╮ 輿繹斬檜 離榆霞ч寞щ縑 霜陝橾 唳辦 120/∵L 婁 67%%醞 濛擎高 檜鼻 餌辨\n");	RichEdit.AddText(strText);
	
	//MapStress["Dead-L"] 餌辨擊 ж雖 彊堅  XLDesignFloor曖 奢衝擊 檜辨п L 高擊 掘л. LSH(2009.03.26)
	double dRate		= RoundUp(120/sqrt(dL/1000),3);
	double dMapStress	= RoundUp(min(dRate,67.0)/100,3);

	strText.Format("              輿繹斬縑 渠и 寥碟徽 : 120 / ∵L = 120 / ∵%.3f = %.3f %% < 67%%\n", toM(dL), dRate);
	RichEdit.AddText(strText);
	
	//LSH -蹺陛
	strText.Format("				As ▼ %.2f = %.0f ▼ %.2f = %.3f mm昌\n", dMapStress, dGenTAsuse, dMapStress,dGenTAsuse*dMapStress);
	RichEdit.AddText(strText);
	strText.Format("         刻 [夔觼葬お撲啗晦遽] 5.7.2.1\n");		RichEdit.AddText(strText);
	strText.Format("           ╮ As = 0.002﹞b﹞h = 0.002 ▼ %.3f ▼ %.3f = %.3f mm昌\n", dB, dH, 0.002*dB*dH);	RichEdit.AddText(strText);
	strText.Format("         券 餌辨 繹斬榆\n");					RichEdit.AddText(strText);
	strText.Format("            в蹂繹斬  : %.3f mm昌\n", max(dAsreq,dGenTAsuse*dMapStress));	RichEdit.AddText(strText);
	if(dAsuse>dAsreq){
		strText.Format("            餌辨繹斬  :  %s%s   @   %.0f   ╮   %.3f mm昌 > Asreq", sRebar, strBDia, dBCTC, dAsuse);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, TRUE, 2);
	}
	else{
		strText.Format("            餌辨繹斬  :  %s%s   @   %.0f   ╮   %.3f mm昌 < Asreq", sRebar, strBDia, dBCTC, dAsuse);
		RichEdit.AddText(strText);
		FillOnOKorNG(RichEdit, FALSE, 2);
	}	

	if(nSide==FLOOR_MID)
	{
		CMap<CString, LPCTSTR, double, double> MapCen;
		CalcFloor.DesignExpansionJointUltimateLoad(MapCen);
		double	dPib			= pCalcData->m_Pib;
		double	dPis			= pCalcData->m_Pis;
		double	dDMainCTC		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// 欽睡 輿繹斬
		CString	strDanDia_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2;
		CString	strDanDia_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CaseH2_2Cy;
		double	dDanArea_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_1Cy;
		double	dDanArea_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_2Cy;
		double	dDanArea		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2;
		strText.Format("    (2) 雖薄警せ溯幗睡(褐蹴檜擠睡)\n\n");		RichEdit.AddMsg(strText, RE_BLACK, FALSE, TRUE);
		strText.Format("      - 輿繹斬\n");		RichEdit.AddText(strText);
		strText.Format("        ﹞ 褐蹴檜擠 :  %.0f ▼ %.0f    ﹞ 蝸楚粽舒眷 : %.3f mm    ﹞ 賅詮お つ望檜 : %.3f mm\n",
			MapCen["WidthExp"], MapCen["HeightExp"], MapCen["ThickSlab"], MapCen["LengthArm"]);
		RichEdit.AddText(strText);
		strText.Format("      1) 斛иж醞 骯薑\n");							RichEdit.AddText(strText);
		strText.Format("         函 堅薑ж醞\n");							RichEdit.AddText(strText);
		strText.Format("            P   =  %.3f kN\n", MapCen["DeadLoad"]);	RichEdit.AddText(strText);
		strText.Format("            Md =  %.3f ▼ %.3f/2 = %.3f kN.m\n",
			MapCen["DeadLoad"], toM(MapCen["LengthArm"]), tokN(MapCen["Md"]));	RichEdit.AddText(strText);
		strText.Format("         刻 �勒狫綅n");								RichEdit.AddText(strText);
		strText.Format("           ﹞ 醱問啗熱 i  =  %.3f\n", MapCen["CollCoeff"]);	RichEdit.AddText(strText);
		strText.Format("           ﹞ 離榆ж醞碟んァ\n");							RichEdit.AddText(strText);
		strText.Format("              E  = 0.35X + 0.98  = 0.35 ▼ ( %.3f - 0.1 ) + 0.98 = %.3f m\n", toM(MapCen["LengthArm"]), toM(MapCen["E"]));
		RichEdit.AddText(strText);
		strText.Format("           ﹞ ァ 1m 渡 �皒薷鉐娉n");				RichEdit.AddText(strText);
		strText.Format("              Ml = %.1f ▼ ( %.3f - 0.1 ) ▼ (1+%.3f) / %.3f = %.3f kN.m\n",
			tokN(MapCen["Pr"]), toM(MapCen["LengthArm"]), MapCen["CollCoeff"], toM(MapCen["E"]), tokN(MapCen["Ml"]));RichEdit.AddText(strText);
		strText.Format("         券 ж醞褻м\n");							RichEdit.AddText(strText);
		strText.Format("              Mu = 1.3Md + 2.15Ml = 1.3 ▼ %.3f + 2.15 ▼ %.3f = %.3f kN.m\n\n",
			tokN(MapCen["Md"]), tokN(MapCen["Ml"]), tokN(MapCen["Mu"]));	RichEdit.AddText(strText);
		strText.Format("      2) 欽賊撲啗\n");								RichEdit.AddText(strText);
		strText.Format("         函 в蹂繹斬榆匐饜\n");						RichEdit.AddText(strText);
		strText.Format("             Mu = %.3f kN.m    fck = %.1f MPa    fy = %.1f MPa    B = %.1f mm\n", tokN(MapCen["Mu"]), dFck, dFy, MapCen["B"]);
		RichEdit.AddText(strText);
		strText.Format("             d  = %.1f mm         d' = %.1f mm     仰f = %.2f        仰v = %.1f\n", MapCen["D"], MapCen["Cover"], dPib, dPis);
		RichEdit.AddText(strText);
		strText.Format("             Mu /仰 = As ▼ fy ▼ (d - a/2)     ...... (1)\n");	RichEdit.AddText(strText);
		strText.Format("             a = As ▼ fy / (0.85 ▼ fck ▼ b)  ...... (2)\n");	RichEdit.AddText(strText);
		strText.Format("             в蹂繹斬榆  Asreq  :  %.3f mm昌\n", MapCen["Asreq"]);		RichEdit.AddText(strText);
		strText.Format("             餌辨繹斬榆  Asuse  :  %s%s  @ %.0f   =  %.3f   mm昌 (1Cycle)\n", sRebar, strDanDia_1Cy, dDMainCTC, dDanArea_1Cy);RichEdit.AddText(strText);
		strText.Format("                                            %s%s  @ %.0f   =  %.3f   mm昌  (2Cycle)\n", sRebar, strDanDia_2Cy, dDMainCTC, dDanArea_2Cy);RichEdit.AddText(strText);
		if(dDanArea>MapCen["Asreq"]){
			strText.Format("                                    м啗  :  %.3f mm昌 >  Asreq", dDanArea);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 1);
		}
		else{
			strText.Format("                                    м啗  :  %.3f mm昌 <  Asreq", dDanArea);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 1);
		}		
		strText.Format("         刻 繹斬綠匐饜\n");														RichEdit.AddText(strText);
		strText.Format("            Asmin  =  0.8 ∵fck ▼ b ▼ d/fy = %.3f mm昌\n", MapCen["Asmin1"]);	RichEdit.AddText(strText);
		strText.Format("            Asmin  =  14 ▼ b ▼ d / fy = %.3f mm昌\n", MapCen["Asmin2"]);		RichEdit.AddText(strText);
		strText.Format("            Asmin  =  Asreq ▼ 4/3 = %.3f mm昌\n", MapCen["Asmin3"]);			RichEdit.AddText(strText);
		strText.Format("                ˍ Asmin = %.3f mm昌\n", MapCen["Asmin"]);						RichEdit.AddText(strText);
		strText.Format("             Pmax  =  0.75 ▼ Pb = 0.75 ▼ 0.85 ▼ k1 ▼ fck/fy ▼ 600 / (600 + fy)\n");RichEdit.AddText(strText);
		strText.Format("                       =  %.3f\n", MapCen["Pmax"]);								RichEdit.AddText(strText);
		strText.Format("            Asmax  = Pmax ▼ b ▼ d = %.3f mm昌\n", MapCen["Asmax"]);			RichEdit.AddText(strText);
		if(dDanArea>MapCen["Asmin"] && dDanArea<MapCen["Asmax"]){
			strText.Format("                 ˍ Asmin  ‾  Asuse  <  Asmax");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 2);			
		}
		else if(dDanArea>MapCen["Asmax"]){
			strText.Format("                 ˍ Asuse  >  Asmax");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 2);			
		}
		else{
			strText.Format("                 ˍ Asuse  <  Asmin");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 2);			
		}		
		strText.Format("      3) �睊●踿� 匐饜\n");														RichEdit.AddText(strText);
		strText.Format("           a  = As ▼ fy / ( 0.85 ▼ fck ▼ b ) = %.3f mm\n", MapCen["A"]);			RichEdit.AddText(strText);
		strText.Format("       仰Mn = 仰 ▼ 0.85 ▼ As ▼ fy ▼ (d - a/2) = %.3f kN.m\n", tokNM(MapCen["Mn"]));	RichEdit.AddText(strText);
		if(MapCen["Mn"]>MapCen["Mu"]){
			strText.Format("             ˍ 仰Mn  ￣  Mu");
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 1);			
		}
		else{
			strText.Format("             ˍ 仰Mn  <  Mu");
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
		strText.Format("    (2) 欽睡\n\n");		RichEdit.AddMsg(strText, RE_BLACK, FALSE, TRUE);
		strText.Format("      - 輿繹斬\n");		RichEdit.AddText(strText);
		strText.Format("        Ｃ 菟爾 欽睡曖 離紫睡碟 夥款っ擎 翱樓撩曖 欽瞰煎 檣и �皒薷鉐挫� 隸陛 塽 \n");					RichEdit.AddText(strText);
		strText.Format("           褐蹴檜擠濰纂煎 檣и 離榆ж醞曖 醱問檜 濛辨ж嘎煎 檜縑 渠и 爾鬼擊 ж罹撿 и棻.\n");			RichEdit.AddText(strText);
		strText.Format("             Mu  =  2Ml + Md  = 2 ▼ %.3f + %.3f + %.3f = %.3f kN.m\n", tokNM(dMl), tokNM(dPreLoad), tokNM(dPostLoad), tokNM(dDanMu));
		RichEdit.AddText(strText);
		strText.Format("        ﹞ 撲啗ж醞 濛辨衛 嶸�褲襄捐瘋捕厭� 曹濰溘 Pe = %.3f kN\n", tokN(dDnaPe));						RichEdit.AddText(strText);
		strText.Format("        ﹞ 撲啗ж醞 濛辨衛 嶸�褲襄捐瘋捕厭� 賅詮お Me = %.3f kN.m\n", tokNM(dMe));						RichEdit.AddText(strText);
		strText.Format("           鼻翱擬溘  :  %.3f MPa      ж翱擬溘  : %.3f MPa\n", dDanUpper, dDanLower);					RichEdit.AddText(strText);
		strText.Format("           餌辨繹斬  :  %s%s   @   %.0f   ╮   %.3f mm昌 (1 Cycle)\n", sRebar, strDia_1Cy, dMainCTC, dMainAs_1Cy);	RichEdit.AddText(strText);
		strText.Format("                          %s%s   @   %.0f   ╮   %.3f mm昌 (2 Cycle)\n", sRebar, strDia_2Cy, dMainCTC, dMainAs_2Cy);	RichEdit.AddText(strText);
		strText.Format("                                        м啗 : %.3f mm昌\n", dDanAsuse);									RichEdit.AddText(strText);
		strText.Format("            x = %.3f ▼ %.3f / ( %.3f + %.3f ) = %.3f mm\n", fabs(dDanUpper), dSlabThick, fabs(dDanUpper), fabs(dDanLower), dDanX);
		RichEdit.AddText(strText);
		strText.Format("            T = %.3f ▼ %.3f ▼ 1/2 = %.3f N\n", fabs(dDanUpper), dDanX, dDanT);						RichEdit.AddText(strText);
		if(dDanAsuse>dDanAsreq){
			strText.Format("            Asreq = %.3f / %.3f = %.3f mm昌  <  Asuse", dDanT, dFsa, dDanAsreq);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 2);			
		}
		else{
			strText.Format("            Asreq = %.3f / %.3f = %.3f mm昌  >  Asuse", dDanT, dFsa, dDanAsreq);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, FALSE, 2);
		}		
		strText.Format("      - 寡溘繹斬\n");		RichEdit.AddText(strText);
		strText.Format("           в蹂繹斬  :  Asreq  =  %.3f ▼ 2  =  %.3f mm昌\n", dAsreq, dDanBAsreq);		RichEdit.AddText(strText);
		if(dBaeAsuse>dDanBAsreq){
			strText.Format("           餌辨繹斬  :  %s%s   @   %.0f   ╮   %.3f mm昌 >  Asreq", sRebar, strBaeDia, dBaeCTC, dBaeAsuse);
			RichEdit.AddText(strText);
			FillOnOKorNG(RichEdit, TRUE, 1);
		}
		else{
			strText.Format("           餌辨繹斬  :  %s%s   @   %.0f   ╮   %.3f mm昌 <  Asreq", sRebar, strBaeDia, dBaeCTC, dBaeAsuse);
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

// 斛иж醞縑 渠и 欽賊匐饜
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

	// 嬪纂
	szText = (nSide==0)? _T("    謝難 警せ溯幗睡") : (nSide==1)? _T("    辦難 警せ溯幗睡") : _T("    蝸楚粽 醞懈睡");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText = (nSide==0 && pDB->m_nTendonFix<1)? _T(" (堅薑欽)") : (nSide==0 && pDB->m_nTendonFix==1)?  _T(" (曹濰欽)") : _T(""); 
	szText += (nSide==1 && pDB->m_nTendonFix<1)? _T(" (曹濰欽)") : (nSide==1 && pDB->m_nTendonFix==1)?  _T(" (堅薑欽)") : _T("");
	szText += _T("\n\n");
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 函 睡雜脹 睡營曖 PS 鬼營曖 擬溘 (fps)
	double dBar_Dia1 = Map["Bar_Dia1"];
	double dBar_Spa1 = Map["Bar_Spa1"];
	double dBar_Are1 = Map["Bar_Are1"];
	double dBar_Dia2 = Map["Bar_Dia2"];
	double dBar_Spa2 = Map["Bar_Spa2"];
	double dBar_Are2 = Map["Bar_Are2"];
	double dBar_Area = Map["Bar_Area"];
	double dFps		 = Map["Fps"];

	szText = _T("    函 睡雜脹 睡營曖 PS 鬼營曖 擬溘 (fps)           ( Ｃ 紫.撲 4.4.3.1, 紫.撲 4.6.3.5, 紫.撲 4.6.3.6 ) \n");
	RichEdit.AddText(szText);
	szText.Format("        As    =    H %.0f  @  %.0f    =    %.3f    mm昌    (1Cycle) \n", dBar_Dia1, dBar_Spa1, dBar_Are1);
	RichEdit.AddText(szText);
	szText.Format("        As    =    H %.0f  @  %.0f    =    %.3f    mm昌    (2Cycle) \n", dBar_Dia2, dBar_Spa2, dBar_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                    ３    =    %.3f    mm昌 \n", dBar_Area);
	RichEdit.AddText(szText);
	szText.Format("        fps    =    %.3f    MPa \n\n", dFps);
	RichEdit.AddText(szText);

	// 刻 斛и鬼紫 匐饜
	double dPiMn1	= Map["Mn1"];
	double dPiMn2	= Map["Mn2"];
	double dQp		= Map["Qp"];
	double dBeta	= Map["Beta"];
	double dMcr		= Map["Mcr"];
	double dMu		= Map["Mu"];

 	if (dPiMn1<dMu)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
 	if (dPiMn1>dMu)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }

	szText = _T("    刻 斛и鬼紫 匐饜 \n");
	RichEdit.AddText(szText);
	szText = _T("    - 餌陝⑽ 欽賊 傳朝 醞董蹴檜 憶蹴Ы楠雖 頂縑 氈朝 Ы楠雖蒂 乾朝 欽賊縑 渠и 撲啗�痚音� \n");
	RichEdit.AddText(szText);
	szText.Format("    ˍ 白Mn    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dPiMn1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    Mu    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dMu);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	if (dPiMn2<dMu)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dPiMn2>dMu)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }

	szText = _T("    - 檣濰難縑 繹斬擊 寡纂ж雖 彊懊剪釭 寡纂ц渦塭紫 斜 艙щ擊 鼠衛й 熱 氈朝 欽賊縑 渠и 撲啗�痚音� \n");
	RichEdit.AddText(szText);
	szText.Format("    ˍ 白Mn    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dPiMn2);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    Mu    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dMu);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 券 譆渠 鬼營綠
	if (dQp<0.36*dBeta)	{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (dQp>0.36*dBeta)	{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }

	szText = _T("    券 譆渠 鬼營綠 \n");
	RichEdit.AddText(szText);
	szText.Format("    ˍ qp    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    ", dQp);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    0.36乒昀    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    ", 0.36*dBeta);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);	
	szText.Format("%s    \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 刷 譆模 鬼營榆
	double dMn = min(dPiMn1, dPiMn2);
	if (dMn<1.2*dMcr)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dMn>1.2*dMcr)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }

	szText = _T("    刷 譆模 鬼營榆 \n");
	RichEdit.AddText(szText);
	szText = _T("    - PS鬼營 塽 繹斬曖 識榆擎 嬪я欽賊縑憮 敕翮賅詮お曖 1.2寡 檜鼻曖 啗熱賅詮お蒂 嫡擊 熱 氈橫撿 и棻. \n");
	RichEdit.AddText(szText);
	szText.Format("    ˍ 白Mn    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", dMn);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    1.2Mcr    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f    kN.m    ", 1.2*dMcr);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 刺 夥款っ 寡溘斬曖 繹斬榆 骯薑
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

	szText = _T("    刺 夥款っ 寡溘斬曖 繹斬榆 骯薑 \n");
	RichEdit.AddText(szText);
	szText = _T("    - 輿繹斬檜 離榆 霞ч寞щ縑 霜陝橾 唳辦 120/∵L 婁 67% 醞 濛擎 高 檜鼻 餌辨 \n");
	RichEdit.AddText(szText);

	if (dSup_Rat1>0.67)		{ szSign=_T("ˇ"); szOk.Format("斜楝嘎煎 %.1f%% 蒂 瞳辨", dSup_Rat2*100); }
	if (dSup_Rat1<=0.67)	{ szSign=_T("ˉ"); szOk.Format("斜楝嘎煎 %.1f%% 蒂 瞳辨", dSup_Rat2*100); }
	szText.Format("       120 / ∵L   =   120 / ∵(%.3f)   =   %.3f %%   %s 67.0 %%,   %s \n", dLength, dSup_Rat1*100, szSign, szOk);
	RichEdit.AddText(szText);

	szText.Format("        �器磈熁雇�    =    H %.0f  @  %.0f    =    %.3f    mm昌    (1Cycle) \n", dBar_Dia1, dBar_Spa1, dBar_Are1);
	RichEdit.AddText(szText);
	szText.Format("        �器磈熁雇�    =    H %.0f  @  %.0f    =    %.3f    mm昌    (2Cycle) \n", dBar_Dia2, dBar_Spa2, dBar_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                    ３    =    %.3f    mm昌 \n", dBar_Area);
	RichEdit.AddText(szText);
	szText.Format("        Req As    =   %.2f  ▼   %.3f   =   %.3f   mm昌 \n", dSup_Rat2, dBar_Area, dSup_Req1);
	RichEdit.AddText(szText);
	szText.Format("        譆模繹斬榆   =   0.002 ▼ b ▼ h   =    0.002    ▼   1,000.000   ▼   %.3f   =    %.3f   mm昌 \n", 
											dSlab_Th,  dSup_Min);
	RichEdit.AddText(szText);

	// 餌辨 繹斬榆 匐饜
	if (dBar_Area<dSup_Req1)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dBar_Area>=dSup_Req1)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }
	szText = _T("    - 餌辨 繹斬榆 匐饜 \n");
	RichEdit.AddText(szText);
	szText.Format("        �器磈� 繹斬   =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌   ", dBar_Area);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm昌   ", dSup_Req1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	
	if (dSup_Min<dSup_Req1)		{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dSup_Min>=dSup_Req1)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }
	szText.Format("        譆模繹斬榆    =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌   ", dSup_Min);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm昌   ", dSup_Req1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	

	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm昌    (1Cycle) \n", dSup_Dia1, dSup_Spa1, dSup_Are1);
	RichEdit.AddText(szText);
	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm昌    (2Cycle) \n", dSup_Dia1, dSup_Spa1, dSup_Are1);
	RichEdit.AddText(szText);
	szText.Format("                                    ３    =    %.3f    mm昌 \n", dSup_Area1);
	RichEdit.AddText(szText);

	if (dSup_Area1<dSup_Req2)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dSup_Area1>=dSup_Req2)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }
 	szText.Format("        ˍ Use As    =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌   ", dSup_Area1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm昌   ", dSup_Req2);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 欽睡 餌辨 繹斬榆 匐饜
	szText = _T("    - 欽睡 餌辨 繹斬榆 匐饜 \n");
	RichEdit.AddText(szText);
	szText.Format("        Req As    =    %.2f    ▼   2   =    %.3f    mm昌 \n", dSup_Req3, dSup_Req4);
	RichEdit.AddText(szText);
	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm昌    (1Cycle) \n", dSup_Dia2, dSup_Spa2, dSup_Are2);
	RichEdit.AddText(szText);
	szText.Format("        Use As    =    H %.0f  @  %.0f    =    %.3f    mm昌    (2Cycle) \n", dSup_Dia2, dSup_Spa2, dSup_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                    ３    =    %.3f    mm昌 \n", dSup_Area2);
	RichEdit.AddText(szText);

	if (dSup_Area2<dSup_Req4)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dSup_Area2>=dSup_Req4)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }
	szText.Format("        ˍ Use As    =   ");
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌   ", dSup_Area2);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s   Req As   =   ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f mm昌   ", dSup_Req4);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
}


///////////////////////////////////////////////////////////////////////////////////
//♁ˍ♂２３４之 民氐永汁汀氾犯丟乒乓乩亙交亦甲亥仿玉伉伙伊伕伍伐休伏f 任仰仳份企
//	╞‾ˇˉ℅▽‾￣＿÷▼妖昂 夾 妓 妖 kgf/妓 t﹞m  易昌昆昂昀昏昕昊
//  函 刻 券 刷 刺 到 刮 制 剁 劾 劻 卒 協 卓 卑 ∵卦卷阮阱阪阬並乖乳Ｃ白
//  收收收收收收收收收收收收收  ╮ Ⅸ
//  旨 曲 旭 旬早收 曳 朽央 失 奴 奶 孕 它 尼 巨 巧 左 弘 弗 必 戊 戊 打 扔
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

	// 嬪纂
	szText = _T("    薑雜睡 爾鬼 \n\n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);

	// 薑雜睡 殮溘 薯錳
	double dFix_Qty		=Map["Fix_Qty"];
	double dFix_Width	=Map["Fix_Width"];
	double dFix_Height	=Map["Fix_Height"];
	double dFix_Space	=Map["Fix_Space"];
	double dEff_Space	=Map["Eff_Space"];
	double dSlab_Thk	=Map["Slab_Thk"];
	double dTdn_Str		=Map["Tdn_Str"];
	double dTdn_Area	=Map["Tdn_Area"];
	double dTdn_Pu		=Map["Tdn_Pu"];

	szText = _T("    王薑雜睡 殮溘 薯錳 \n");
	RichEdit.AddText(szText);
	szText.Format("       薑雜っ渡 薑雜掘 =	%.3f ea			蝸楚粽 舒眷(t) =	%.3f mm \n", dFix_Qty, dSlab_Thk);
	RichEdit.AddText(szText);
	szText.Format("       薑雜っ ァ錳(a) =	%.3f mm		薑雜掘 除問 =	%.3f mm \n", dFix_Width, dFix_Space);
	RichEdit.AddText(szText);
	szText.Format("       薑雜っ 堪檜(b) =	%.3f mm		嶸�� 雖憶ァ =	%.3f mm \n", dFix_Height, dEff_Space);
	RichEdit.AddText(szText);
	szText.Format("       譆渠 曹濰擬溘 =	%.3f MPa		鬼翱摹 欽賊瞳 =	%.3f mm昌 \n", dTdn_Str, dTdn_Area);
	RichEdit.AddText(szText);
	szText.Format("       鬼翱摹 曹濰溘(Pu) =	%.3f kN \n\n", dTdn_Pu);
	RichEdit.AddText(szText);

	// 函 �器磈� だ翮溘縑 渠и 爾鬼 繹斬榆 骯薑
	double dAH				= Map["dAH"];		
	double dSH				= Map["dSH"];		
	double dT2H				= Map["T2H"];		
	double dFsa				= Map["Fsa"];		
	double dReq_AsT2H		= Map["Req_AsT2H"];
	double dDia_T2H			= Map["Dia_T2H"];	
	double dQty_T2H			= Map["Qty_T2H"];	
	double dUse_AsT2H		= Map["Use_AsT2H"];

	if (dUse_AsT2H < dReq_AsT2H)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dUse_AsT2H > dReq_AsT2H)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }

	szText = _T("    函 �器磈� だ翮溘縑 渠и 爾鬼 繹斬榆 骯薑		( Ｃ 紫.撲 4.6.3.9 (3)刮(棻) ) \n");
	RichEdit.AddText(szText);
	szText = _T("       だ翮溘(T2)	=	0.20 x Pu x ( 1 - a / s ) \n");
	RichEdit.AddText(szText);
	szText.Format("		=	0.20 ▼ %.3f ▼( 1 - %.1f / %.1f )	=	%.3f	kN \n", dTdn_Pu, dAH, dSH, dT2H);
	RichEdit.AddText(szText);
	szText = _T("       繹斬榆 匐饜 \n");
	RichEdit.AddText(szText);
	szText.Format("       Req As	=	T昏/ fsa	=	%.3f / %.1f	=	%.3f mm昌 \n", dT2H*1000, dFsa, dReq_AsT2H);
	RichEdit.AddText(szText);
	szText.Format("       Use As	=	H %.0f ▼ %.0f    =    ", dDia_T2H, dQty_T2H);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌	", dUse_AsT2H);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	Req As	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌	", dReq_AsT2H);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 刻 熱霜寞щ だ翮溘縑 渠и 爾鬼繹斬榆 骯薑
	double dAV			= Map["dAV"];		
	double dSV			= Map["dSV"];		
	double dT2V			= Map["T2V"];		
	double dReq_AsT2V	= Map["Req_AsT2V"];
	double dDia_T2V		= Map["Dia_T2V"];	
	double dQty_T2V		= Map["Qty_T2V"];	
	double dUse_AsT2V	= Map["Use_AsT2V"];

	if (dUse_AsT2V < dReq_AsT2V)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dUse_AsT2V > dReq_AsT2V)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }
	
	szText = _T("    刻 熱霜寞щ だ翮溘縑 渠и 爾鬼繹斬榆 骯薑		( Ｃ 紫.撲 4.6.3.9 (3)刮(棻) ) \n");
	RichEdit.AddText(szText);
	szText = _T("       だ翮溘(T2)	=	0.20 x Pu x ( 1 - a / s ) \n");
	RichEdit.AddText(szText);
	szText.Format("		=	0.20 ▼ %.3f ▼( 1 - %.1f / %.1f )	=	%.3f	kN \n", dTdn_Pu, dAV, dSV, dT2V);
	RichEdit.AddText(szText);
	szText = _T("       繹斬榆 匐饜 \n");
	RichEdit.AddText(szText);
	szText.Format("       Req As	=	T昏/ fsa	=	%.3f / %.1f	=	%.3f mm昌 \n", dT2V*1000, dFsa, dReq_AsT2V);
	RichEdit.AddText(szText);
	szText.Format("       Use As	=	H %.0f ▼ %.0f    =    ", dDia_T2V, dQty_T2V);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌	", dUse_AsT2V);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	Req As	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌	", dReq_AsT2V);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 券 欽睡 檣濰溘縑 渠и 爾鬼繹斬榆 骯薑
	double dT1			= Map["T1"];
	double dReq_AsT1	= Map["Req_AsT1"];
	double dDia_T1		= Map["Dia_T1"];
	double dQty_T1		= Map["Qty_T1"];
	double dUse_AsT1	= Map["Use_AsT1"];

	szText = _T("    券 欽睡 檣濰溘縑 渠и 爾鬼繹斬榆 骯薑		( Ｃ 紫.撲 4.6.3.9 (3)刮(棻) ) \n");
	RichEdit.AddText(szText);
	szText = _T("       だ翮溘(T1)	=	0.20 x Pu x ( 1 - a / s ) \n");
	RichEdit.AddText(szText);
	szText.Format("		=	0.20 ▼ %.3f ▼( 1 - %.1f / %.1f )	=	%.3f	kN \n", dTdn_Pu, dAH, dSH, dT1);
	RichEdit.AddText(szText);
	szText = _T("       繹斬榆 匐饜 \n");
	RichEdit.AddText(szText);
	szText.Format("       Req As	=	T昀/ fsa	=	%.3f / %.1f	=	%.3f mm昌 \n", dT1*1000, dFsa, dReq_AsT1);
	RichEdit.AddText(szText);
	szText.Format("       Use As	=	H %.0f ▼ %.0f    =    ", dDia_T1, dQty_T1);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌	", dUse_AsT1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	Req As	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f mm昌	", dReq_AsT1);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);
	
	// 刷 薑雜掘 雖憶擬溘 匐饜	
	double dFb	= Map["Fb"];
	double dFct	= Map["Fct"];
	double dAo	= Map["Ao"];
	double dAg	= Map["Ag"];
	double dFba	= Map["Fba"];

	szText = _T("    刷 薑雜掘 雖憶擬溘 匐饜 \n");
	RichEdit.AddText(szText);
	szText = _T("    王ъ辨雖憶擬溘 骯薑 (fba) \n");
	RichEdit.AddText(szText);
	szText.Format("       fb	= %.3f ▼ %.3f ▼ / ( %.3f ▼ %.3f )	=	%.3f MPa \n", dTdn_Pu, dTdn_Area, dFix_Height, dFix_Width, dFb);
	RichEdit.AddText(szText);
	szText.Format("       fba	= 0.7 ▼ 仰 ▼ fct' ▼ ∵(A/Ag) \n");
	RichEdit.AddText(szText);
	szText.Format("       	= 0.70 ▼ 0.80 ▼ %.1f ▼ ∵(%.1f / %.1f)	=	%.3f MPa \n", dFct, dAo, dAg, dFba);
	RichEdit.AddText(szText);

	if (dFb < dFba)	{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (dFb > dFba)	{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }
	szText.Format("    ˍ fb	=	");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa	", dFb);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	fba	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f  MPa	", dFba);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	if (dFb < 2.25*0.80*dFct)	{ szSign=_T("ˉ"); szOk=_T("ˍ O.K"); }
	if (dFb > 2.25*0.80*dFct)	{ szSign=_T("ˇ"); szOk=_T("ˍ N.G"); }
	szText.Format("    ˍ fb	=	");
	RichEdit.AddText(szText);
	szText.Format("%.3f MPa	", dFb);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s	2.25仰fct'	=	", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.3f  MPa	", 2.25*0.8*dFct);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

}

// 蝸楚粽繹斬榆匐饜
// nCheckType  = 0 : 鬼紫撲啗徹			1 : 唳я瞳 撲啗徹
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
	
	double dPI  = pData->m_Pib;						// �睊� 渠и 鬼紫馬模啗熱
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
	
	if(nPos==FLOOR_LEFT)		newElement.Format("    < 謝難 警せ溯幗 繹斬榆 骯薑 >\n\n");
	if(nPos==FLOOR_RIGHT)		newElement.Format("    < 辦難 警せ溯幗 繹斬榆 骯薑 >\n\n");
	if(nPos==FLOOR_MID)			newElement.Format("    < 醞懈睡 蝸楚粽 繹斬榆 骯薑 >\n\n");
	RichEdit.AddMsg(newElement, RE_BLUE, FALSE, TRUE);

	newElement.Format("  (1) 殮溘濠猿\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
	newElement.Format("      Mu   =    %.3f kN.m\n", Mu);
	RichEdit.AddText(newElement);
	newElement.Format("       b    =    %.3f mm        h    =    %.3f mm", b, h);
	RichEdit.AddText(newElement);
	newElement.Format("      d'    =    %.3f mm           d    =    %.3f mm\n", d11,d);
	RichEdit.AddText(newElement); 
	newElement.Format("      fck   =    %.3f MPa          fy   =    %.3f MPa", Ck,y);
	RichEdit.AddText(newElement);
	newElement.Format("      仰(Bending)  =  %.2f\n\n", dPI);
	RichEdit.AddText(newElement);
	newElement.Format("  (2) 繹斬匐饜\n");
	RichEdit.AddMsg(newElement, RE_BLACK, FALSE, TRUE);
	newElement.Format("      函 橾奩睡 輿繹斬\n");
	RichEdit.AddText(newElement);
	newElement.Format("         Mu = 仰 ▼ As ▼ fy ▼ (d-a/2)");
	RichEdit.AddText(newElement);
	if(Ck<=28)	newElement.Format("         a  = As ▼ fy / (0.85 ▼ fck ▼ b)\n");
	else		newElement.Format("         a  = As ▼ fy / (%.3lf ▼ fck ▼ b)\n", 0.85-(Ck-28)*0.007);
	RichEdit.AddText(newElement);
	newElement.Format("          - в蹂繹斬    Asreq = %.3f mm昌 \n",Asreq);
	RichEdit.AddText(newElement);
	newElement.Format("          - 餌辨繹斬    %s%s   @   %.0f    ╮    %.3f mm昌  (1 Cycle)\n", sRebar, CaseH1_1Cy, Ctc1, UseAs_1Cy);
	RichEdit.AddText(newElement);
	newElement.Format("                             %s%s   @   %.0f    ╮    %.3f mm昌  (2 Cycle)\n", sRebar, CaseH1_2Cy, Ctc1, UseAs_2Cy);
	RichEdit.AddText(newElement);
	newElement.Format("                                                 м啗   %.3f mm昌", UseAs);
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

	newElement.Format("          - 繹斬綠匐饜 \n");
	RichEdit.AddText(newElement);
	newElement.Format("             P  =  As / b ▼ d = %.4f   %s   Pmax = 0.75 ▼ Pb = %.4f", Pused, strSign1, Pmax);
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
	
	newElement.Format("          - 撲啗賅詮お匐饜\n");
	RichEdit.AddText(newElement);
	newElement.Format("             Max.c = 0.75 ▼ %.f / %.f + fy▼d = %.3f mm   %s   c = a/0.85 = %.3f mm\n", 0.003*Es,0.003*Es,Maxc,strSign2, c);
	RichEdit.AddText(newElement);
	newElement.Format("             Max.a = 0.85 ▼ c = %.3f mm   %s   a = As ▼ fy / ( 0.85 ▼ fck ▼ b ) = %.3f mm\n", Maxa, strSign3, a);
	RichEdit.AddText(newElement);
	newElement.Format("             jd       = d - a/2 = %.3f mm\n", d-a/2);
	RichEdit.AddText(newElement);
	newElement.Format("             C       = 0.85 ▼ fck ▼ a ▼ b =  %.3f kN   ,   T = fy ▼ As =  %.3f kN\n", tokN(0.85*Ck*a*b), tokN(y*UseAs));
	RichEdit.AddText(newElement);
	newElement.Format("             Mn     = 仰▼fy▼As▼jd  = %.3f kN.m   %s   Mu =  %.3f kN.m", Md,strSign4,Mu);
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

	newElement.Format("      刻 橾奩睡 寡溘繹斬       Ｃ紫.衛 P191 \n");
	RichEdit.AddText(newElement);
	newElement.Format("             寡溘繹斬曖 輿繹斬縑 渠и 寥碟徽");
	RichEdit.AddText(newElement);
	
	double BPer = 120/sqrt(L);
	if(BPer > 67)
	{
		newElement.Format("      120/∵L = 120 / ∵%.3f  = %.3f  %%   >   67 %%\n",L,BPer);
		RichEdit.AddText(newElement);
		newElement.Format("             Asreq  =  As ▼ 67%% = %.3f ▼ 0.67 =  %.3f  mm昌\n",Asreq,Asreq*0.67);
		RichEdit.AddText(newElement);
		newElement.Format("             熱蹴﹞螞紫繹斬榆  :  0.002﹞b﹞h  = %.3f mm昌\n", 0.002*b*h);
		RichEdit.AddText(newElement);
		newElement.Format("             餌辨繹斬     %s%s   @   %.0f     =  %.3f  mm昌", sRebar, CaseH3,CtcB,BAsuse);	// USE朝 歜衛曖 高歜
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
		newElement.Format("      120/∵L = 120 / ∵%.3f  = %.0f  %%   <   67 %%\n",L,BPer);
		RichEdit.AddText(newElement);
		newElement.Format("             Asreq  =  As ▼ %.0f %% = %.3f ▼ %.2f =  %.3f  mm昌\n",BPer, Asreq,(BPer/100),Asreq*(BPer/100));
		RichEdit.AddText(newElement);
		newElement.Format("             熱蹴﹞螞紫繹斬榆  :  0.002﹞b﹞h  = %.3f mm昌\n", 0.002*b*h);
		RichEdit.AddText(newElement);
		newElement.Format("               餌辨繹斬     %s%s   @   %.0f     =  %.3f  mm", sRebar, CaseH3,CtcB,BAsuse);
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

	if(nPos==FLOOR_MID)		//醞懈睡
	{
		newElement.Format("      券 欽睡 輿繹斬 撲啗				Ｃ紫.衛 P195\n");
		RichEdit.AddText(newElement);
		newElement.Format("             菟爾欽睡檜諼曖 醞除雖除曖 夥款っ縑 в蹂и 輿繹斬榆曖 2寡蒂 輿繹斬戲煎 寡纂и棻. \n");
		RichEdit.AddText(newElement);
		newElement.Format("           ˍ  Asreq  =  %.3f mm昌 ▼ 2 = %.3f mm昌\n",Asreq, 2*Asreq);
		RichEdit.AddText(newElement);
		newElement.Format("               餌辨繹斬      %s%s   @   %.0f     =  %.3f mm昌\n", sRebar, CaseH2_1Cy, Ctc2, UseAs2_1Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                 %s%s   @   %.0f     =  %.3f mm昌\n", sRebar, CaseH2_2Cy, Ctc2, UseAs2_2Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                                    м啗  %.3f mm昌", UseAs2);
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
		
		newElement.Format("      刷 欽睡 寡溘繹斬\n");
		RichEdit.AddText(newElement);
		newElement.Format("             в蹂繹斬      Asreq  =  %.3f mm昌 ▼ 2 = %.3f mm昌\n",BAsreq, 2*BAsreq);
		RichEdit.AddText(newElement);
		newElement.Format("             餌辨繹斬      %s%s   @   %.0f     =  %.3f mm昌", sRebar, CaseH4,CtcB1,BAsuse2);	// USE朝 歜衛曖 高歜
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

		newElement.Format("      刺 欽睡 寡纂彰嬪\n");
		RichEdit.AddText(newElement);
		newElement.Format("            警せ溯幗っ曖 堅薑ж醞縑 渠и 雖除曖 卦 (L=%.3fm) 檜鼻", 
			toM(pData->CALC_CANTILEVER_DATA[nPos].m_Dead_L)/2);
		RichEdit.AddText(newElement);
	}
	else			//警せ溯幗睡
	{
		newElement.Format("      券 欽睡 輿繹斬 撲啗 \n");
		RichEdit.AddText(newElement);
		newElement.Format("              в蹂繹斬      Asreq  =  %.3f mm昌 ▼ 2 = %.3f mm昌\n",Asreq, 2*Asreq);
		RichEdit.AddText(newElement);
		newElement.Format("              餌辨繹斬      %s%s   @   %.0f     =  %.3f mm昌\n", sRebar, CaseH2_1Cy, Ctc2, UseAs2_1Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                %s%s   @   %.0f     =  %.3f mm昌\n", sRebar, CaseH2_2Cy, Ctc2, UseAs2_2Cy);
		RichEdit.AddText(newElement);
		newElement.Format("                                             м啗  %.3f mm昌", UseAs2);
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

		newElement.Format("      刷 欽睡 寡溘繹斬\n");
		RichEdit.AddText(newElement);
		newElement.Format("             в蹂繹斬      Asreq  =  %.3f mm昌 ▼ 2 = %.3f mm昌\n",BAsreq, 2*BAsreq);
		RichEdit.AddText(newElement);
		newElement.Format("             餌辨繹斬      %s%s   @   %.0f     =  %.3f mm昌", sRebar, CaseH4,CtcB1,BAsuse2);	// USE朝 歜衛曖 高歜
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

		newElement.Format("      刺 欽睡 寡纂彰嬪\n");
		RichEdit.AddText(newElement);
		newElement.Format("             警せ溯幗っ曖 堅薑ж醞縑 渠и 雖除 (L=%.3fm) 檜鼻", 
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
		
	// 嬪纂
	szText = _T("    醞除 雖薄睡 \n");
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText= _T("    王檣濰溘擊 嫡朝 夥款っ縑憮 夔觼葬お曖 欽賊擊 鼠衛ж堅 撲啗蒂 ж朝 唳辦縑朝 夥款っ 夔觼葬お 欽賊瞳曖 \n");
	RichEdit.AddText(szText);
	szText .Format("       %.1f%%檜鼻曖 掖蹴寞щ 繹斬擊 寡斬ж罹撿 и棻. 檜 唳辦 掖蹴寞щ 繹斬曖 識輿滲濰曖 夥款っ 夔觼葬お曖 \n", CalcFloor.GetFloorConcAreaCoefficient());
	RichEdit.AddText(szText);
	szText.Format("       欽賊瞳縑 渠и 綠, 闊 輿濰睦擎 %.4f mm/mm昌 檜鼻戲煎 ж賊 謠棻. \n\n", CalcFloor.GetFloorJujangStd());
	RichEdit.AddText(szText);

	// 函 繹斬榆 匐饜
	if (dBar_Area<dReq_As)	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dBar_Area>dReq_As)	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }

	szText = _T("    函 繹斬榆 匐饜 \n");
	RichEdit.AddText(szText);
	szText.Format("       в蹂 繹斬榆    Req As    =    1000.0    ▼    %.3f    ▼    %.3f    =    %.1f  mm昌 \n", 
					dSlab_Thk, CalcFloor.GetFloorConcAreaCoefficient()/100, dReq_As);
	RichEdit.AddText(szText);
	szText = _T("       餌辨 繹斬榆 \n");
	RichEdit.AddText(szText);
	szText.Format("       鼻 賊    :    H %.0f    @    %.0f    =    %.1f    mm昌 \n", dBar_Dia1, dBar_Spa1, dBar_Are1);
	RichEdit.AddText(szText);	
	szText.Format("       ж 賊    :    H %.0f    @    %.0f    =    %.1f    mm昌 \n", dBar_Dia2, dBar_Spa2, dBar_Are2);
	RichEdit.AddText(szText);
	szText.Format("                                          ３    =    %.1f    mm昌\n", dBar_Area);
	RichEdit.AddText(szText);
	szText = _T("       ˍ    Use As    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.1f    mm昌    ", dBar_Area);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    Req As    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.1f    mm昌    ", dReq_As);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s \n\n", szOk);
	RichEdit.AddMsg(szText, RE_RED, FALSE, TRUE);

	// 刻 輿濰睦 匐饜
	if (dUr<CalcFloor.GetFloorJujangStd())	{ szSign=_T("ˉ"); szOk=_T("ˍ N.G"); }
	if (dUr>CalcFloor.GetFloorJujangStd())	{ szSign=_T("ˇ"); szOk=_T("ˍ O.K"); }

	szText = _T("    刻 輿濰睦 匐饜 \n");
	RichEdit.AddText(szText);
	szText= _T("    王掖蹴寞щ 繹斬曖 識輿滲濰婁 夥款っ 夔觼葬お曖 欽賊瞳縑 渠и 綠 \n");
	RichEdit.AddText(szText);
	szText.Format("       繹斬 餌辨榆    :    %.2f    +    %.2f    =    %.2f    EA/m \n\n",
					dBar_Use1, dBar_Use2, dBar_n);
	RichEdit.AddText(szText);
	szText = _T("		            休 ▼ D ▼ n + 休 ▼ D ▼ n\n");
	RichEdit.AddText(szText);
	szText.Format("       輿 濰 睦    :    伊 = 式式式式式式式式式式式式     >    %.4f mm/mm昌 \n", CalcFloor.GetFloorJujangStd());
	RichEdit.AddText(szText);
	szText = _T("       		               1000 ▼ tm \n\n");
	RichEdit.AddText(szText);
	szText = _T("                            D       :    繹斬 霜唳 \n");
	RichEdit.AddText(szText);
	szText = _T("                            n       :    繹斬 偃熱 \n");
	RichEdit.AddText(szText);
	szText = _T("                            1000    :    欽嬪 ァ (mm) \n");
	RichEdit.AddText(szText);
	szText = _T("                            tm      :    醞除 蝸楚粽 舒眷 (mm) \n\n");
	RichEdit.AddText(szText);

	szText.Format("		            休 ▼ %.2f ▼ %.2f + 休 ▼ %.2f ▼ %.2f\n", _dRebarDiadim[pData->m_ComboInnerJijumSlab1], dBar_Use1, _dRebarDiadim[pData->m_ComboInnerJijumSlab2], dBar_Use2);
	RichEdit.AddText(szText);	
	szText.Format("                           伊 = 式式式式式式式式式式式式式式式     =    %.4f  mm/mm昌 \n", dUr);
	RichEdit.AddText(szText);	
	szText.Format("		                  1000 ▼ %.2f \n", dSlab_Thk);

	RichEdit.AddText(szText);
	szText = _T("       ˍ    伊    =    ");
	RichEdit.AddText(szText);
	szText.Format("%.4f    mm/mm昌    ", dUr);
	RichEdit.AddMsg(szText, RE_BLACK, FALSE, TRUE);
	szText.Format("%s    伊a    =    ", szSign);
	RichEdit.AddText(szText);
	szText.Format("%.4f    mm/mm昌    ", CalcFloor.GetFloorJujangStd());
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

	sText.Format("    (1) 籀颶 匐饜         Ｃ夔.撲 P79\n\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	sText.Format("       籀颶啗骯擊 ж雖 彊朝 睡營曖 譆模舒眷  T  = %.1f mm\n", dTmin);
	RichEdit.AddText(sText);

	if(dSigmaY==400.0)
	{
		sText.Format("       fy  =  %.1f MPa  檜嘎煎  tmin  =  L / %.0f  瞳辨\n", dSigmaY, dCoeff);
		RichEdit.AddText(sText);
		sText.Format("       tmin  =  L / %.1f  =  %.3f / %.1f  =  %.3f mm\n", dCoeff, dDeadL, dCoeff, dT);
		RichEdit.AddText(sText);
	}
	else
	{
		sText.Format("       fy  =  %.1f MPa  檜嘎煎  tmin  =  L / %.0f ▼ (0.43 + fy / 700) 瞳辨\n", dSigmaY, dCoeff);
		RichEdit.AddText(sText);
		sText.Format("       tmin  =  L / %.1f ▼ ( 0.43 + %.1f / 700 )  = %.3f mm\n", dCoeff, dSigmaY, dT);
		RichEdit.AddText(sText);
	}

	if(dT>dTmin)
	{
		sText.Format("       ˍ tmin   >   %.3f mm  檜嘎煎 籀颶匐饜 в蹂.", dTmin);
		RichEdit.AddText(sText);
	}
	else
	{
		sText.Format("       ˍ tmin   <   %.3f mm  檜嘎煎 滌紫曖 籀颶匐饜 в蹂橈擠.", dTmin);
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
	double	dEs			= pCalcData->DESIGN_MATERIAL.m_dEs;			// 繹斬 驕撩啗熱
	double	dEc			= pCalcData->DESIGN_MATERIAL.m_dEc;			// 夔觼葬お 驕撩啗熱
	double	dStop_M		= Map["CRACK_MSTOP"];
	double	dMove_Ml	= Map["CRACK_MMOVE"];
	double	dCent_Mcf	= Map["CRACK_MCENT"];
	double	dWind_Mw	= Map["CRACK_MW"];
	double	dMmax		= Map["CRACK_MMAX"];
	double	dAs			= Map["CRACK_As"] + Map["CRACK_Ap"];		// 模熱輿⑽橾 唳辦 曹濰營曖 賊瞳 んл 
	double	dD			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_d;
	double	dH			= Map["CRACK_H"];
	double	dEA			= Map["CRACK_EA"];
	double	dB			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_b;
	double	dC			= Map["CRACK_C"];		//繹斬夔觼葬お 譆模竣偃
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

	sText.Format("    (2) 敕翮 匐饜\n");
	RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);
	sText.Format("       函 �� 繹斬曖 擬溘 (fs)\n");
	RichEdit.AddText(sText);
	sText.Format("          Mmax  =  %.3f  kN.m \n", tokNM(dMmax));
	RichEdit.AddText(sText);
	sText.Format("          餌辨ж醞ж縑憮 譆渠ж醞擊 嶸嫦ж朝 ж醞褻м擊 餌辨и棻.\n");
	RichEdit.AddText(sText);
	sText.Format("          M  =  Md + Ml+i + Mcf + 0.3 ▼ Mw  =  %.3f + %.3f + %.3f + %.3f  =  %.3f kN.m\n", tokNM(dStop_M), tokNM(dMove_Ml), tokNM(dCent_Mcf), tokNM(dWind_Mw*0.3), tokNM(dMmax));
	RichEdit.AddText(sText);
	sText.Format("          n  =  Es / Ec  =  %ld\n ", n);
	RichEdit.AddText(sText);
	sText.Format("         伏 =  As / bd   =  %.3f / (%.3f ▼ %.3f)  =  %.5f\n", dAs, dB, dD, dpDomyunAAA);
	RichEdit.AddText(sText);
	sText.Format("          k  =  -n伏 + ∵{(n伏)昌 + 2n伏}  =  %ld ▼ %.5f + ∵{(%.4f)昌 + 2 ▼ %.4f}  =  %.4f\n", -n, dpDomyunAAA, n*dpDomyunAAA, n*dpDomyunAAA, dK);
	RichEdit.AddText(sText);
	sText.Format("          j  =  1 - (k/3) = 1 - (%.4f/3)  =  %.3f\n", dK, dJ);
	RichEdit.AddText(sText);
	sText.Format("          fs  =  Mmax / (伏王b王j王d昌)  =  (%.3f ▼ 10^5) / (%.5f ▼ %.1f ▼ %.3f ▼ %.1f昌)\n", dMmax, dpDomyunAAA, dB, dJ, dD);
	RichEdit.AddText(sText);
	sText.Format("              =  %.3f  MPa\n\n", dSigmaSmax);
	RichEdit.AddText(sText);

	sText.Format("       刻 嶸�� 檣濰 欽賊瞳\n");
	RichEdit.AddText(sText);
	sText.Format("          A  =  b ▼ h / EA  =  %.3f ▼ %.3f / %.0f  =  %.1f mm昌\n", dB, dH, dEA, dA);
	RichEdit.AddText(sText);
	sText.Format("           王A  : 輿檣濰繹斬 輿嬪曖 夔觼葬お 欽賊瞳擊 繹斬曖 熱榆戲煎 釭換 嶸�螃怹� 欽賊瞳\n");
	RichEdit.AddText(sText);
	sText.Format("           王dc : 檣濰翱欽縑憮 陛濰 陛梱檜 嬪纂и 繹斬 醞褕梱雖曖 Я犒舒眷\n");
	RichEdit.AddText(sText);
	sText.Format("           王z  : �矞雇梴� 碟寡蒂 薯иж朝 熱榆\n");
	RichEdit.AddText(sText);
	sText.Format("          z  =  fs ▼ (dc ▼ A)^卷 = %.3f ▼( %.3f ▼ %.1f )^卷\n ", dSigmaSmax, dDC, dA);
	RichEdit.AddText(sText);
	sText.Format("             =  %.3f N/mm", dZ);
	RichEdit.AddText(sText);
	if(dZ < 23170.0)
	{
		sText.Format("    <  %.1f N/mm (褕и 諼喻轎)", 23170.0);
		RichEdit.AddText(sText);
		sText.Format("    ... O.K\n\n");
		RichEdit.AddMsg(sText, RE_BLUE, FALSE, TRUE);
	}
	else
	{
		sText.Format("    >  %.1f N/mm (褕и 諼喻轎)", 23170.0);
		RichEdit.AddText(sText);
		sText.Format("    ... N.G\n\n");
		RichEdit.AddMsg(sText, RE_RED, FALSE, TRUE);
	}

	//敕翮ァ匐饜
	sText.Format("       券 敕翮ァ曖 匐饜\n");
	RichEdit.AddText(sText);
	sText.Format("          阮 wa =  %.4f ▼ C  (%s)\n", ddWaCoeff, strAllowCrackWidth);
	RichEdit.AddText(sText);
	sText.Format("              罹晦憮, C  :  繹斬 夔觼葬お  譆模 竣偃\n");
	RichEdit.AddText(sText);
	sText.Format("          阱 wa  =  %.4f ▼ %.1f  =  %.3f  mm\n", ddWaCoeff, dC, dWa);
	RichEdit.AddText(sText);
	sText.Format("          阪  w  =  1.08 ▼ R ▼ fs ▼ 昆∵(dc ▼A) ▼ 0.00001\n");
	RichEdit.AddText(sText);
	sText.Format("               w  =  1.08 ▼ %.3f ▼ %.3f ▼ 昆∵(%.1f ▼%.1f) ▼ 0.00001\n", dR, dSigmaSmax, dDC, dA);
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
	newElement.Format("    << 籀颶 匐饜 >>");
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ╮ �務俯萷� 曖и 譆渠 籀颶 : %.3f cm", dMax);
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ╮ 譆渠 ъ辨 籀颶");
	Arr.Add(newElement+"\r\n");

	newElement.Format("       掖榆掘褻僭曖 �睆怹蝝� 餌辨�勒狫�(醱問ж醞 んл)戲煎 掘褻僭曖 鬼紫釭 褒薯ж醞縑  雖濰擊");
	Arr.Add(newElement+"\r\n");

	newElement.Format("       輿朝 籀颶 傳朝 斜諼 滲⑽檜 橾橫釭雖 彊紫煙 醱碟и 鬼撩擊 偽啪 撲啗ж罹撿 и棻.");
	Arr.Add(newElement+"\r\n");

	newElement.Format("       鬼掖曖 ъ辨譆渠 籀颶榆擎 嬴楚諦 偽棻.         Ｃ紫.撲 P53");
	Arr.Add(newElement+"\r\n");

	newElement.Format("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收有收收收收收收收收收收收收收收收收收旬");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                      掖 榆 ⑽ 衝                         早            譆 渠 籀 颶           早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("曳收收收收收收收收有收收收收收收收收收收收收有收收收收收收收朱收收收收收收收收收有收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                早         謙 盟          早     雖 除    早 欽牖雖雖剪渦 塽  早 啪腦幗剪渦曖 早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                早                        早              早    翱樓 剪渦     早 警せ溯幗睡   早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                曳收收收收收收收收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早  Ы溯檜お 剪渦 早                        早  L   ‾ 10 m 早    L / 2,000     早   L / 1,200  早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                早   繹斬夔觼葬お 蝸楚粽  曳收收收收收收收朱收收收收收收收收收朱收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早      ⑽衝      早   陛 氈朝 Ы溯檜お剪渦 早10 < L ‾ 40 m早   L / (20,000/L) 早 L / (1,200/L)早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                早                        曳收收收收收收收朱收收收收收收收收收朱收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                早                        早    L > 40 m  早      L / 500     早    L / 300   早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                曳收收收收收收收收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                早    晦顫曖 蝸楚粽陛 氈朝 Ы溯檜お剪渦   早      L / 500     早    L / 300   早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("曳收收收收收收收收朴收收收收收收收收收收收收收收收收收收收收朱收收收收收收收收收朴收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                      ⑷ 熱 掖 ⑽ 衝                      早              L / 350             早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收收收收收收收收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                      餌 濰 掖 ⑽ 衝                      早              L / 400             早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收收收收收收有收收收收收收收朽");
	Arr.Add(newElement+"\r\n");
	newElement.Format("早                       晦 顫 ⑽ 衝                        早      L / 600     早    L / 400   早");
	Arr.Add(newElement+"\r\n");
	newElement.Format("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朴收收收收收收收收收朴收收收收收收收旭");
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ˍ ъ辨 籀颶榆 汁a = L / (20,000/L) = %.3f / ( 20,000 / %.3f ) = %.3f m = %.3f cm", dLengthJigan, dLengthJigan, dAllow, dAllow*100);
	Arr.Add(newElement+"\r\n");

	newElement.Format("    ╮ 籀颶 匐饜");
	Arr.Add(newElement+"\r\n");

	CString strSign;
	if(dMax < dAllow*100)	strSign = "<";
	else					strSign = ">";
	newElement.Format("      汁max  = %.3f cm   %s   汁a = %.3f cm", dMax, strSign, dAllow*100);
	Arr.Add(newElement+"\r\n");

	if(dMax < dAllow*100)	strSign = "ˍ O.K";
	else					strSign = "ˍ N.G";
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

	// 剪渦滌 ⑷濰檜擠 偃熱蒂 掘л.
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

	double	dHyR		= StressCalc.GetMapValueSecStress("HyR_Pos");	// ж檜粽葬萄啗熱 R
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
	{  // 綠м撩⑽
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

	BD = pSp->m_ZzsBoltSize;		//  = M22  (Bolt 雖葷)
	BT = pDB->GetMaterial(SP_U_BOLT, pBx);	
	CSteelBoltData *pBoltData = pBolt->GetBolt(BT, BD);
	Dh = pBoltData->GetBoltSize() + pBoltData->m_HoleMargin;
	Ra  = pBoltData->m_ForceAllow;
	
	Arr.Add("");
	CString cs;
	if(Su > 0.0)	cs.Format(" A. 匐饜欽賊 : 薑 賅詮お 掘除 (G%d, SP%d)", nG + 1, nSp + 1);
	else			cs.Format(" A. 匐饜欽賊 : 睡 賅詮お 掘除 (G%d, SP%d)", nG + 1, nSp + 1);
	Arr.Add (cs);
	Arr.Add("");
	cs.Format(" B. 濛辨溘 塽 欽賊溘" );
	Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())
	{  // 綠м撩⑽
		cs.Format("    - 濛辨 識 �皒薷鉐�        : Ms  = %8.2f kN.m", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("    - 濛辨 識 瞪欽溘          : Ss  = %8.2f kN", tokN(Ss));
		Arr.Add (cs);
	}
	else
	{  // м撩⑽
		cs.Format("    - м撩瞪 �皒薷鉐�         : Ms  = %8.2f kN.m", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("    - м撩�� �皒薷鉐�         : Mv  = %8.2f kN.m", tokNM(Mv));
		Arr.Add (cs);
		cs.Format("    - м撩瞪 瞪欽溘           : Ss  = %8.2f kN", tokN(Ss));
		Arr.Add (cs);
		cs.Format("    - м撩�� 瞪欽溘           : Sv  = %8.2f kN", tokN(Sv));
		Arr.Add (cs);
	}
	cs.Format("    - 綠ぎ葡 賅詮お           : Mt  = %8.2f kN.m", tokNM(Mt));
	Arr.Add (cs);
	cs.Format("    - 鼻睡 FLANGE曖 譆渠 擬溘 : fu  = %8.2f MPa", Su);
	Arr.Add (cs);
	cs.Format("    - ж睡 FLANGE曖 譆渠 擬溘 : fl  = %8.2f MPa", Sl);
	Arr.Add (cs);
	if(bHybrid)
	{
		cs.Format("    - м撩�� ж檜粽葬萄 啗熱  : R   = %7.3f", dHyR);		
		Arr.Add (cs);
	}

	MapSplice["HyR"]= dHyR;
	MapSplice["Su"] = Su;
	MapSplice["Sl"] = Sl;
	Arr.Add("");
	cs.Format(" C. 檜擠曖 か撩");
	Arr.Add (cs);
	if (STfu == STfl && STfl == STw && STw == STru && STru == STrl)
	{
		cs.Format("    1) 鬼 謙 : %s   f = %g MPa", STfu, Safu);
		Arr.Add (cs);
	}
	else
	{
		cs.Format("    1) 鬼 謙");
		Arr.Add (cs);
		cs.Format("       鼻  っ :%10s   f =%6g MPa", STfu, Safu);
		Arr.Add (cs);
		cs.Format("       ж  っ :%10s   f =%6g MPa", STfl, Safl);
		Arr.Add (cs);
		cs.Format("       犒睡っ :%10s   f =%6g MPa", STw,  Saw);
		Arr.Add (cs);
	}
	MapSplice["Safu"] = Safu*dHyR;
	MapSplice["Safl"] = Safl*dHyR;
	MapSplice["Saw"] = Saw*dHyR;
	cs.Format("    2) BOLT  : H.T.B %s (%s) 伏a  =  %g x 2賊 葆雞 = %g MPa", BD, BT, Ra, Ra * 2);
	MapSplice["BOLT_ALLOW_STRESS"] = Ra * 2 * dHyR;
	Arr.Add (cs);
	Ra *= 2.0;
/*	double dFs = 0;
	if((Ms+Mv)>0)	dFs = abs(Su);
	else			dFs = abs(Sl);
*/
	Arr.Add("");
	cs.Format(" D. %s   (鼻睡 Ы楠雖曖 檜擠)", bPositive ? "憶蹴睡 FLANGE曖 啗骯":"檣濰睡 FLANGE曖 啗骯");
	Arr.Add (cs);
	SpliceFlangeCheckUp(Arr, nG, nSp, Ss, Sv, Su, Safu, Ra, Dh, MapSplice);
//	if((Ms+Mv)>0)	dFs = abs(Sl);
//	else			dFs = abs(Su);
	Arr.Add("");
	cs.Format(" E. %s   (ж睡 Ы楠雖曖 檜擠)", bPositive ? "檣濰睡 FLANGE曖 啗骯":"憶蹴睡 FLANGE曖 啗骯");
	Arr.Add (cs);
	SpliceFlangeCheckLo(Arr, nG, nSp, Ss, Sv, Sl, Safl, Ra, Dh, MapSplice);
	Arr.Add("");
	cs.Format(" F. WEB 翱唸睡曖 啗骯");
	Arr.Add (cs);
	SpliceWebCheck(Arr, nG, nSp, Ss, Sv, Ms, Mv, Su, Sl, Saw, Ra, MapSplice);

}
//	Ss : м撩瞪 瞪欽溘
//	Sv : м撩�� 瞪欽溘
//	Sf : Ы楠雖 擬溘
//	Sa : 鬼營 ъ辨 擬溘
//	Ra : 獐お ъ辨 鬼紫 (2賊 葆雞 晦遽)
//	Dh : 獐お 掘誠 雖葷
//  罹晦憮曖 頂睡 滲熱菟曖 欽嬪朝 ら曖蒂 嬪п 晦獄瞳戲煎 cm, kgf戲煎 ж艘擠.
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
	
	double dHyR	= StressCalc.GetMapValueSecStress("HyR_Pos");		// ж檜粽葬萄啗熱 R
	double Tf = pGir->GetThickFactByStation(dSta, G_F_U);			// 鼻睡Ы楠雖 舒眷
	double Wf = pGir->GetWidthOnStation(dSta, TRUE);				// 鼻睡Ы楠雖 傘綠
	double Ts = pSp->m_uT;											// 鼻睡檜擠っ 舒眷
	double LoW= pSp->m_uW;											// 鼻睡 濛擎 檜擠っ曖 ァ
	double Ls = (pSp->m_uA + pSp->m_uB * pSp->m_uC) * 2 + pSp->m_uC1;	// 鼻睡檜擠っ 望檜
	int Nl = (int)(pSp->m_uB+1);										// 鼻睡檜擠っ-餌辨 獐お 塭檣熱
	int Nb = (int)(pSp->m_uE+1)*2;										// 鼻睡檜擠っ-餌辨 獐お 塭檣渡 偃熱
	double Ys = Round(fabs(ModelCalc.GetMapValueSecCo("Ysu")),1);			// м撩瞪-醞董蹴縑 渠и Ы楠雖 剪葬
	double Yv = Round(fabs(ModelCalc.GetMapValueSecCo("Yvsu")),1);			// м撩��-醞董蹴縑 渠и Ы楠雖 剪葬
	double Pb = Wf / Nb;   // 獐お ゎ敕 е纂
	double As = Wf*Ts + LoW*Ts*2;
	MapSplice["Tf_UP"] = Tf;

	CString cs;
	cs.Format("     fs  = %g MPa", Sf);											Arr.Add (cs);
	cs.Format("     fs' = 0.75 x %g  =  %g MPa", Sa, Sa * 0.75);					Arr.Add (cs);
	Sf = max (fabs (Sf), Sa * 0.75);
	
	cs.Format("     ˍ檜擠っ曖 撲啗擬溘(fu)擎 %g MPa擊 瞳辨и棻.", Sf);		Arr.Add (cs);
	cs.Format("    1) в蹂獐お熱曖 唸薑");											Arr.Add (cs);
	cs.Format("              As x fs       %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);	Arr.Add (cs);
	if (Nl*Nb > Tf*Wf*Sf/(dHyR*Ra))
		cs.Format("       n =  --------- =  --------------------------  = %g EA  <  n = %d EA   O.K", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);
	else
		cs.Format("       n =  --------- =  --------------------------  = %g EA  >  n = %d EA   N.G", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);

	MapSplice["REQ_BOLTSU_UP"]   = Tf*Wf*Sf/(dHyR*Ra);
	MapSplice["USE_BOLTSU_UP"]   = Nl*Nb;

	Arr.Add (cs);
	if(bHybrid)	cs.Format("             R x 伏a       %.3f  x  %.1f", dHyR, Ra);
	else		cs.Format("               伏a                 %.1f", Ra);									Arr.Add (cs);
	cs.Format("    2) 檜擠っ曖 唸薑");																		Arr.Add (cs);
	cs.Format("                           %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);								Arr.Add (cs);
	cs.Format("       в蹂欽賊瞳 : As =  --------------------------  =  %.2f mm昌", Tf*Wf*Sf/(dHyR*Sa));	Arr.Add (cs);
	if(bHybrid)	cs.Format("                              %.3f  x  %.1f", dHyR, Sa);
	else		cs.Format("                                    %.1f", Sa);									Arr.Add (cs);
	cs.Format("       餌辨檜擠っ : 1-PL  %5g  x %4g  x  %g  =  %7.2f mm昌", Wf, Ts, Ls, Wf*Ts);				Arr.Add (cs);
	cs.Format("		            2-PL  %5g  x %4g  x  %g  =  %7.2f mm昌", LoW, Ts, Ls, LoW*Ts*2);			Arr.Add (cs);
	cs.Format("					    м啗  =  %7.2f mm昌", As);												Arr.Add (cs);
	cs.Format("                                               -------------------");						Arr.Add (cs);
	if (bTension)   // 檣濰睡檜賊
	{  
		cs.Format("                                                 =  %7.2f mm昌", As);	Arr.Add (cs);
		cs.Format("       檜擠っ曖 牖欽賊瞳 : 檣濰睡曖 牖欽賊瞳擎 BOLT雖葷擊 奢薯");		Arr.Add (cs);
		cs.Format("       An  =  %.2f  -  %.2f  x  %.2f  x  %d  x  2 EA", As, Dh, Ts, Nb);	Arr.Add (cs);
		As -= Dh * Ts * Nb * 2;
	}
	if (As > Tf*Wf*Sf/(dHyR*Sa))
		cs.Format("                                                 =  %7.2f mm昌  >  Asreq'd = %7.2f mm昌   O.K", As, Tf*Wf*Sf/(dHyR*Sa));
	else
		cs.Format("                                                 =  %7.2f mm昌  <  Asreq'd = %7.2f mm昌   N.G", As, Tf*Wf*Sf/(dHyR*Sa));

	MapSplice["REQ_AS_UP"]       = As;
	MapSplice["USE_AS_UP"]       = Tf*Wf*Sf/(dHyR*Sa);

	Arr.Add (cs);
	cs.Format("                             %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);		Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf/As)	cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa <  R x f sa , O.K", Tf*Wf*Sf/As);
		else						cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa >  R x f sa , N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf/As)	cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa <  f sa , O.K", Tf*Wf*Sf/As);
		else					cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa >  f sa , N.G", Tf*Wf*Sf/As);
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
	cs.Format("    3) BOLT曖 擬溘 匐饜" );													Arr.Add (cs);
	cs.Format("       a) 霜擬溘縑 渠и 擬溘" );												Arr.Add (cs);
	cs.Format("                   P      %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);				Arr.Add (cs);
	cs.Format("         伏P  =  ---- =  --------------------------  =   %.2f  N/EA", Rp);	Arr.Add (cs);
	cs.Format("                   n               %d EA", Nl * Nb);							Arr.Add (cs);
	cs.Format("       b) �睎�欽溘縑 渠и 擬溘" );											Arr.Add (cs);

	if (pDB->GetNumHaiseokCombo())// 綠м撩⑽
	{  
		cs.Format("                S Qs       p");							Arr.Add (cs);
		cs.Format("         伏h = ------- x ----");							Arr.Add (cs);
		cs.Format("                 Is        n");							Arr.Add (cs);
		cs.Format("         S  : 濛辨 識 瞪欽溘  ( %.3f kN )", tokN(Ss));	Arr.Add (cs);
		cs.Format("         Qs : 鬼營 欽賊曖 醞董蹴縑 渠и 蕾м摹 諼難曖 欽賊 1離 賅詮お");						Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm昌", Wf, Tf, Ys, Tf/2, Qs);	Arr.Add (cs);
		cs.Format("         Is : 鬼營 欽賊曖 欽賊 2離 賅詮お  (  %.1f mm昂)", Is);								Arr.Add (cs);
		cs.Format("         p : 蕾м摹 寞щ曖 BOLT曖 ゎ敕 е纂 (mm) = Ы楠雖 瞪ァ / �器磈熇數挺� = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n  : 蕾м摹 霜陝曖 BOLT曖 偃熱");									Arr.Add (cs);
		cs.Format("                %9.1f x %9.1f       %4.1f mm", Ss, Qs, Pb);					Arr.Add (cs);
		cs.Format("         伏h =  ---------------------  x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                     %11.1f             %d EA", Is, Nl);						Arr.Add (cs);
	}
	else  
	{
		cs.Format("               忙 Ss Qs     Sv Qv  忖     p" );								Arr.Add (cs);
		cs.Format("         伏h = 弛------- + ------- 弛 x ----" );								Arr.Add (cs);
		cs.Format("               戌  Is        Iv    戎     n" );								Arr.Add (cs);
		cs.Format("         Ss : м撩瞪 瞪欽溘  ( %.3f kN ),   Sv : м撩瞪 瞪欽溘  ( %.3f kN )", tokN(Ss), tokN(Sv));	Arr.Add (cs);
		cs.Format("         Qs, Qv : м撩瞪 塽 м撩�釋� 醞董蹴縑 渠и 蕾м摹 諼難曖 欽賊 1離 賅詮お");					Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm昌", Wf, Tf, Ys, Tf/2, Qs);		Arr.Add (cs);
		cs.Format("           Qv  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm昌", Wf, Tf, Yv, Tf/2, Qv);		Arr.Add (cs);
		MapSplice["Ys_UP"] = Ys;
		MapSplice["Yv_UP"] = Yv;
		cs.Format("         Is, Iv : м撩瞪 塽 м撩�釋� 醞董蹴縑 渠и 識欽賊曖 欽賊 2離 賅詮お");						Arr.Add (cs);
		cs.Format("           ( Is =  %.1f mm昂,   Iv =  %.1f mm昂)", Is, Iv);											Arr.Add (cs);
		cs.Format("         p : 蕾м摹 寞щ曖 BOLT曖 ゎ敕 е纂 (mm) = Ы楠雖 瞪ァ / �器磈熇數挺� = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n : 蕾м摹 霜陝曖 BOLT曖 偃熱");																	Arr.Add (cs);
		cs.Format("                忙 %10.1f x %10.1f   %10.1f x %10.1f 忖     %4.1f mm", Ss, Qs, Sv, Qv, Pb);					Arr.Add (cs);
		cs.Format("         伏h =  弛------------------------- + ---------------------- 弛 x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                戌      %12.1f           %12.1f      戎      %d EA", Is, Iv, Nl);							Arr.Add (cs);
	}
	cs.Format("       c) м撩擬溘曖 匐饜");													Arr.Add (cs);
	cs.Format("               __________________     _________________________");			Arr.Add (cs);	
	if(bHybrid)
	{
		if (R < dHyR*Ra)	cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  <  R x 伏a   O.K", Rp, Rh, R);
		else				cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  >  R x 伏a   N.G", Rp, Rh, R);
	}
	else
	{
		if (R < Ra)	cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  <  伏a   O.K", Rp, Rh, R);
		else		cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  >  伏a   N.G", Rp, Rh, R);
	}
	
	Arr.Add (cs);

	if (!bTension)
		return;

	As = Tf * (Wf - Dh * Nb);
	cs.Format("    4) 賅營曖 擬溘 匐饜" );																							Arr.Add (cs);
	cs.Format("       Ы楠雖曖 識欽賊瞳                     :  As  =  %.1f  x  %.1f  =  %.1f mm昌", Tf, Wf, Tf*Wf);					Arr.Add (cs);
	cs.Format("       獐お 掘誠擊 奢薯и Ы楠雖曖 牖欽賊瞳  :  Asn =  %.1f x (%.1f - %.1f x %d)  =  %.1f mm昌", Tf, Wf, Dh, Nb, As);	Arr.Add (cs);
	cs.Format("                            %6.1f  x  %6.1f", Tf*Wf, Sf);															Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf / As)	cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 <  R x f a ,  O.K", Tf*Wf*Sf/As);
		else							cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 >  R x f a ,  N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf / As)	cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 <  f a ,  O.K", Tf*Wf*Sf/As);
		else					cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 >  f a ,  N.G", Tf*Wf*Sf/As);
	}
	
	Arr.Add (cs);
	MapSplice["檣濰賅營濛辨擬溘_UP"] = Tf*Wf*Sf/As;
	cs.Format("                                 %6.1f", As);		Arr.Add (cs);
}

//	Ss : м撩瞪 瞪欽溘
//	Sv : м撩�� 瞪欽溘
//	Sf : Ы楠雖 擬溘
//	Sa : 鬼營 ъ辨 擬溘
//	Ra : 獐お ъ辨 鬼紫 (2賊 葆雞 晦遽)
//	Dh : 獐お 掘誠 雖葷
//  罹晦憮曖 頂睡 滲熱菟曖 欽嬪朝 ら曖蒂 嬪п 晦獄瞳戲煎 cm, kgf戲煎 ж艘擠.
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
	double	dHyR	= StressCalc.GetMapValueSecStress("HyR_Pos");	// ж檜粽葬萄啗熱 R

	double Tf = pGir->GetThickFactByStation(dSta, G_F_L);			// ж睡Ы楠雖 舒眷
	double Wf = pGir->GetWidthOnStation(dSta, FALSE);				// ж睡Ы楠雖 傘綠
	double Ts = pSp->m_dT;											// ж睡檜擠っ 舒眷
	double LoW= pSp->m_dW;											// ж睡 濛擎 檜擠っ曖 ァ
	double Ls = (pSp->m_dA + pSp->m_dB * pSp->m_dC) * 2 + pSp->m_dC1;		// ж睡檜擠っ 望檜
	int Nl = (int)(pSp->m_dB+1);										// 餌辨 獐お 塭檣熱
	int Nb = (int)(pSp->m_dE+1)*2;										// 餌辨 獐お 塭檣渡 偃熱
	double Ys = Round(fabs(ModelCalc.GetMapValueSecCo("Ysl")),1);				//  (醞董蹴縑 渠и Ы楠雖 剪葬)
	double Yv = Round(fabs(ModelCalc.GetMapValueSecCo("Yvsl")),1);			//  (醞董蹴縑 渠и Ы楠雖 剪葬)
	double Pb = Wf / Nb;   // 獐お ゎ敕 е纂
	double As = Wf*Ts + LoW*Ts*2;
	
	MapSplice["ALLOW_STRESS_DN"] = Ra;
	MapSplice["Tf_DN"] = Tf;

	CString cs;
	cs.Format("     fs  = %g MPa", Sf);
	Arr.Add (cs);
	cs.Format("     fs' = 0.75 x %g  =  %g MPa", Sa, Sa * 0.75);
	Arr.Add (cs);
	Sf = max (fabs (Sf), Sa * 0.75);
	
	cs.Format("     ˍ檜擠っ曖 撲啗擬溘(fl)擎 %g MPa擊 瞳辨и棻.", Sf);		Arr.Add (cs);
	cs.Format("    1) в蹂獐お熱曖 唸薑");											Arr.Add (cs);
	cs.Format("              As x fs       %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);	Arr.Add (cs);
	if (Nl*Nb > Tf*Wf*Sf/(dHyR*Ra))
		cs.Format("       n =  --------- =  --------------------------  = %g EA  <  n = %d EA   O.K", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);
	else
		cs.Format("       n =  --------- =  --------------------------  = %g EA  >  n = %d EA   N.G", Tf*Wf*Sf/(dHyR*Ra), Nl*Nb);

	MapSplice["REQ_BOLTSU_DN"]   = Tf*Wf*Sf/(dHyR*Ra);
	MapSplice["USE_BOLTSU_DN"]   = Nl*Nb;

	Arr.Add (cs);
	if(bHybrid)	cs.Format("             R x 伏a       %.3f  x  %.1f", dHyR, Ra);
	else		cs.Format("               伏a                 %.1f", Ra);									Arr.Add (cs);
	cs.Format("    2) 檜擠っ曖 唸薑");																		Arr.Add (cs);
	cs.Format("                           %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);								Arr.Add (cs);
	cs.Format("       в蹂欽賊瞳 : As =  --------------------------  =  %.2f mm昌", Tf*Wf*Sf/(dHyR*Sa));	Arr.Add (cs);
	if(bHybrid)	cs.Format("                              %.3f  x  %.1f", dHyR, Sa);
	else		cs.Format("                                    %.1f", Sa);									Arr.Add (cs);
	cs.Format("       餌辨檜擠っ : 1-PL  %5g  x %4g  x  %g  =  %7.2f mm昌", Wf, Ts, Ls, Wf*Ts);				Arr.Add (cs);
	cs.Format("		            2-PL  %5g  x %4g  x  %g  =  %7.2f mm昌", LoW, Ts, Ls, LoW*Ts*2);			Arr.Add (cs);
	cs.Format("					    м啗  =  %7.2f mm昌", As);												Arr.Add (cs);
	cs.Format("                                               -------------------");						Arr.Add (cs);
	if (bTension)   {  // 檣濰睡檜賊
		cs.Format("                                                 =  %7.2f mm昌", As);					Arr.Add (cs);
		cs.Format("       檜擠っ曖 牖欽賊瞳 : 檣濰睡曖 牖欽賊瞳擎 BOLT雖葷擊 奢薯");						Arr.Add (cs);
		cs.Format("       An  =  %.2f  -  %.2f  x  %.2f  x  %d  x  2 EA", As, Dh, Ts, Nb);					Arr.Add (cs);
		As -= Dh * Ts * Nb * 2;
	}
	if (As > Tf*Wf*Sf/(dHyR*Sa))
		cs.Format("                                                 =  %7.2f 妓  >  Asreq'd = %7.2f mm昌   O.K", As, Tf*Wf*Sf/(dHyR*Sa));
	else
		cs.Format("                                                 =  %7.2f 妓  <  Asreq'd = %7.2f mm昌   N.G", As, Tf*Wf*Sf/(dHyR*Sa));

	MapSplice["REQ_AS_DN"]       = As;
	MapSplice["USE_AS_DN"]       = Tf*Wf*Sf/(dHyR*Sa);

	Arr.Add (cs);
	cs.Format("                             %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);		Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf/As)	cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa <  R x f sa , O.K", Tf*Wf*Sf/As);
		else						cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa >  R x f sa , N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf/As)	cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa <  f sa , O.K", Tf*Wf*Sf/As);
		else					cs.Format("       檜擠っ曖 擬溘 f s =  --------------------------  =   %.2f  MPa >  f sa , N.G", Tf*Wf*Sf/As);
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
	if (bTension && !pDB->IsTUGir())   // 薑賅詮お
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
	cs.Format("    3) BOLT曖 擬溘 匐饜" );													Arr.Add (cs);
	cs.Format("       a) 霜擬溘縑 渠и 擬溘" );												Arr.Add (cs);
	cs.Format("                   P      %.1f  x  %.1f  x  %.1f", Tf, Wf, Sf);				Arr.Add (cs);
	cs.Format("         伏P  =  ---- =  --------------------------  =   %.2f  N/EA", Rp);	Arr.Add (cs);
	cs.Format("                   n               %d EA", Nl * Nb);							Arr.Add (cs);
	cs.Format("       b) �睎�欽溘縑 渠и 擬溘" );											Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())  // 綠м撩⑽
	{  
		cs.Format("                S Qs       p");							Arr.Add (cs);
		cs.Format("         伏h = ------- x ----");							Arr.Add (cs);
		cs.Format("                 Is        n");							Arr.Add (cs);
		cs.Format("         S  : 濛辨 識 瞪欽溘  ( %.3f kN )", tokN(Ss));	Arr.Add (cs);
		cs.Format("         Qs : 鬼營 欽賊曖 醞董蹴縑 渠и 蕾м摹 諼難曖 欽賊 1離 賅詮お");						Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm昌", Wf, Tf, Ys, Tf/2, Qs);	Arr.Add (cs);
		cs.Format("         Is : 鬼營 欽賊曖 欽賊 2離 賅詮お  (  %.1f mm昂)", Is);								Arr.Add (cs);
		cs.Format("         p : 蕾м摹 寞щ曖 BOLT曖 ゎ敕 е纂 (mm) = Ы楠雖 瞪ァ / �器磈熇數挺� = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n  : 蕾м摹 霜陝曖 BOLT曖 偃熱");									Arr.Add (cs);
		cs.Format("                %9.1f x%9.1f       %4.1f mm", Ss, Qs, Pb);					Arr.Add (cs);
		cs.Format("         伏h =  ---------------------  x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                     %11.1f             %d EA", Is, Nl);						Arr.Add (cs);
	}
	else  
	{
		cs.Format("               忙 Ss Qs     Sv Qv  忖     p" );								Arr.Add (cs);
		cs.Format("         伏h = 弛------- + ------- 弛 x ----" );								Arr.Add (cs);
		cs.Format("               戌  Is        Iv    戎     n" );								Arr.Add (cs);
		cs.Format("         Ss : м撩瞪 瞪欽溘  ( %.3f kN ),   Sv : м撩瞪 瞪欽溘  ( %.3f kN )", Ss, Sv);				Arr.Add (cs);
		cs.Format("         Qs, Qv : м撩瞪 塽 м撩瞪曖 醞董蹴縑 渠и 蕾м摹 諼難曖 欽賊 1離 賅詮お");					Arr.Add (cs);
		cs.Format("           Qs  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm昌", Wf, Tf, Ys, Tf/2, Qs);		Arr.Add (cs);
		cs.Format("           Qv  =  %.1f  x  %.1f  x  ( %.2f  -  %.1f )  =  %.1f  mm昌", Wf, Tf, Yv, Tf/2, Qv);		Arr.Add (cs);
		MapSplice["Ys_DN"] = Ys;
		MapSplice["Yv_DN"] = Yv;
		cs.Format("         Is, Iv : м撩瞪 塽 м撩瞪曖 醞董蹴縑 渠и 識欽賊曖 欽賊 2離 賅詮お");						Arr.Add (cs);
		cs.Format("           ( Is =  %.1f mm昂,   Iv =  %.1f mm昂)", Is, Iv);											Arr.Add (cs);
		cs.Format("         p : 蕾м摹 寞щ曖 BOLT曖 ゎ敕 е纂 (mm) = Ы楠雖 瞪ァ / �器磈熇數挺� = %.1lf / %d = %.1lf mm", Wf, Nb, double(Wf/Nb));		Arr.Add (cs);
		cs.Format("         n : 蕾м摹 霜陝曖 BOLT曖 偃熱");																	Arr.Add (cs);
		cs.Format("                忙 %10.1f x %10.1f   %10.1f x %10.1f 忖     %4.1f mm", Ss, Qs, Sv, Qv, Pb);					Arr.Add (cs);
		cs.Format("         伏h =  弛------------------------- + ---------------------- 弛 x  ---------  =   %.2f  N/EA", Rh);	Arr.Add (cs);
		cs.Format("                戌      %12.1f           %12.1f      戎      %d EA", Is, Iv, Nl);							Arr.Add (cs);
	}
	cs.Format("       c) м撩擬溘曖 匐饜");													Arr.Add (cs);
	cs.Format("               __________________     _________________________");			Arr.Add (cs);
	if(bHybrid)
	{
		if(R < dHyR*Ra)	cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  <  R x 伏a   O.K", Rp, Rh, R);
		else			cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  >  R x 伏a   N.G", Rp, Rh, R);
	}
	else
	{
		if(R < Ra)	cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  <  伏a   O.K", Rp, Rh, R);
		else		cs.Format("         伏 = ∵( 伏伏昌+ 伏h 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  >  伏a   N.G", Rp, Rh, R);
	}
	
	Arr.Add (cs);

	if (!bTension)
		return;

	As = Tf * (Wf - Dh * Nb);
	cs.Format("    4) 賅營曖 擬溘 匐饜" );																							Arr.Add (cs);
	cs.Format("       Ы楠雖曖 識欽賊瞳                     :  As  =  %.1f  x  %.1f  =  %.1f mm昌", Tf, Wf, Tf*Wf);					Arr.Add (cs);
	cs.Format("       獐お 掘誠擊 奢薯и Ы楠雖曖 牖欽賊瞳  :  Asn =  %.1f x (%.1f - %.1f x %d)  =  %.1f mm昌", Tf, Wf, Dh, Nb, As);	Arr.Add (cs);
	cs.Format("                            %6.1f  x  %6.1f", Tf*Wf, Sf);															Arr.Add (cs);
	if(bHybrid)
	{
		if (dHyR*Sa > Tf*Wf*Sf / As)	cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 <  R x f a ,  O.K", Tf*Wf*Sf/As);
		else							cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 >  R x f a ,  N.G", Tf*Wf*Sf/As);
	}
	else
	{
		if (Sa > Tf*Wf*Sf / As)	cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 <  f a ,  O.K", Tf*Wf*Sf/As);
		else					cs.Format("       賅營曖 擬溘  f  =  ---------------------  =   %.2f  N/cm昌 >  f a ,  N.G", Tf*Wf*Sf/As);
	}
	
	Arr.Add (cs);
	MapSplice["檣濰賅營濛辨擬溘_DN"] = Tf*Wf*Sf/As;
	cs.Format("                                 %6.1f", As);		Arr.Add (cs);
}
//	Ss : м撩瞪 瞪欽溘
//	Sv : м撩�� 瞪欽溘
//	Ms : м撩瞪 堅薑ж醞 賅詮お
//	Mv : м撩�� 堅薑ж醞 賅詮お
//	Su : 鼻っ 擬溘
//	Sl : жっ 擬溘
//	Sa : 鬼營 ъ辨 擬溘
//	Ra : 獐お ъ辨 鬼紫 (2賊 葆雞 晦遽)
void CPlateStressOut::SpliceWebCheck(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Ms, double Mv,
								 double Su, double Sl, double Sa, double Ra, CMap<CString, LPCTSTR, double, double> &MapSplice)

{
//  罹晦憮曖 頂睡 滲熱菟曖 欽嬪朝 ら曖蒂 嬪п 晦獄瞳戲煎 cm, kgf戲煎 ж艘擠.
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
	Tw  = pGir->GetThickFactByStation(dSta, G_W);		// 犒睡っ 舒眷
	Hw  = pBx->GetHeightGirderByBx();					// 犒睡っ 堪檜
	Ho	= Hw/2;
	Lb  = (Hw - ((pSp->m_mD + pSp->m_mF * pSp->m_mE)* 2 + pSp->m_sF*pSp->m_sE + pSp->m_sF1*pSp->m_sE1*2)) / 2;  // 檜擠っ 1,2 餌檜曖 獐お剪葬

	CString cs;
	cs.Format("    1) BOLT曖 擬溘 啗骯");
	Arr.Add (cs);
	Arr.Add("");
	cs.Format("       a) �睊� 曖и BOLT曖 擬溘");
	Arr.Add (cs);
	
	BOOL bMoment = FALSE;	//賅詮お 罹睡
	long ni;				//檣策蝶 褻薑濠
	double d1;
	double d2;
	
	d1 = (pSp->m_mD + pSp->m_mE * pSp->m_mF) * 2;
	d2 = pSp->m_sE * pSp->m_sF + (pSp->m_sE1 * pSp->m_sF1)*2;
					
	for (int i=0; i<5 ; i++)  
	{
		if (i <= pSp->m_mE && pSp->m_mE >0)	 //賅詮お 檜擠っ
		{  
			bMoment = TRUE;
			Nl = (int) pSp->m_mB + 1;	// 賅詮お 檜擠っ 塭檣渡 獐お熱
			
			if (i == 0)					// 賅詮お 檜擠っ 羅還							
				Lp = pSp->m_mD + pSp->m_mF / 2;	
			else if (i == pSp->m_mE)	// 賅詮お 檜擠っ 葆雖虞還						
				Lp = pSp->m_mF / 2 + (Hw - (d1 +d2))/4;	
			else						// 賅詮お 檜擠っ 醞除還				
				Lp = pSp->m_mF;					
		}
		else	// 瞪欽 檜擠っ
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
				if (ni == 0)	//瞪欽 檜擠っ 羅廓簞 翮
					Lp = bMoment ? (pSp->m_sF /2) + (Hw-(d1 + d2)) /4 :	 pSp->m_sF / 2 + Lb;
				else if(pSp->m_sE > ni)	// 瞪欽 檜擠っ 醞除 翮				
					Lp = pSp->m_sF;				
			}	
			Nl = (int) pSp->m_sB + 1;	// 瞪欽 檜擠っ 塭檣渡 獐お熱
		}

		Lps	+= Lp;
		Ls	= Sw*(Ho-Lps)/Ho;
		Rb	= (Rbp+Ls)/2 * Lp * Tw / Nl;		
		cs.Format("                %6.1f  +  %6.1f",  Rbp, Ls);
		Arr.Add (cs);
		if (Rb < Ra)
			cs.Format("         %d翮 : ------------------- x  %4.1f  x  %.1f mm  ㊣ %d  =  %6.1f N/EA  <  伏a   O.K", i+1, Lp, Tw, Nl, Rb);
		else
			cs.Format("         %d翮 : ------------------- x  %4.1f  x  %.1f mm  ㊣ %d  =  %6.1f N/EA  >  伏a   N.G", i+1, Lp, Tw, Nl, Rb);
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
	Nt  = (pSp->m_mB+1)*2*(pSp->m_mE+1) + (pSp->m_sB+1)*(pSp->m_sE+pSp->m_sE1*2+1);  // 獐お 識 偃熱
	K   = ModelCalc.GetMapValueSecCo("K");
	Rs  = (fabs(Ss + Sv) + fabs(Mt*Tw)/K)/Nt;
	R   = hypot (RbMax, Rs);
	double dFlangeUpW = pGir->GetWidthOnStation(dSta, TRUE);
	double dFlangeDnW = pGir->GetWidthOnStation(dSta, FALSE);
	Arr.Add("");
	cs.Format("       b) 瞪欽溘縑 渠и BOLT曖 擬溘" );								Arr.Add (cs);
	cs.Format("         Ⅸ 犒睡っ檜 睡氬ж朝 瞪欽溘 : Ss+Sv");						Arr.Add (cs);
	cs.Format("         Ⅸ 綠ぎ葡 賅詮お縑 曖и 犒睡曖 瞪欽溘 : St=Mt﹞t/ K");		Arr.Add (cs);
	cs.Format("         Ⅸ K = ３(1/3)﹞b﹞t昆   ( 紫﹞撲 п撲 P 273 霤褻 )");		Arr.Add (cs);
	cs.Format("              = ( %.1lf ▼ %.1lf昆 + %.1lf ▼ %.1lf昆+ %.1lf ▼ %.1lf昆)/3 = %.1lf mm昂"
		, dFlangeUpW, Tfu, Hw, Tw, dFlangeUpW, Tfu, K);					
	Arr.Add (cs);

	if (Rs < Ra)
		cs.Format("         伏s = (%.1lf +  (%.2lf ▼ %.1lf)/ %.0lf ) / %d EA  =  %.2f N/EA  <  伏a   O.K", fabs(Ss+Sv), Mt, Tw, K, Nt, Rs);
	else
		cs.Format("         伏s = (%.1lf +  (%.2lf ▼ %.1lf)/ %.0lf ) / %d EA  =  %.2f N/EA  >  伏a   N.G", fabs(Ss+Sv), Mt, Tw, K, Nt, Rs);
	Arr.Add (cs);
	MapSplice["SHEAR_BOLT_STRESS"] = Rs;
	Arr.Add("");
	cs.Format("       c) м撩擬溘曖 匐饜" );
	Arr.Add (cs);
	cs.Format("                __________________     _________________________");
	Arr.Add (cs);
	if (R < Ra)
		cs.Format("         伏  = ∵( 伏P昌+ 伏s 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  <  伏a   O.K", RbMax, Rs, R);
	else
		cs.Format("         伏  = ∵( 伏P昌+ 伏s 昌)  = ∵( %.2f昌 +  %.2f昌) =  %.2f N/EA  >  伏a   N.G", RbMax, Rs, R);
	Arr.Add (cs);
	MapSplice["WEB_COMPOSE_STRESS"] = R;
	double Iv, Isw2, Ivw2, dTable[3][8], *pSum;
	double Ts1, Ts2, Hs1, Hs2, Ys1, I, Isw, Ivw, Msw, Mvw, Ssu, Ssl;

	double Ysu  = MapSplice["Ms"]+MapSplice["Mv"]>0 ? MapSplice["Ys_UP"] : MapSplice["Ys_DN"];
	double Yvsu = MapSplice["Ms"]+MapSplice["Mv"]>0 ? MapSplice["Yv_UP"] : MapSplice["Yv_DN"];
	double Ysl  = Hw+MapSplice["Tf_UP"]+MapSplice["Tf_DN"]-Ysu;
	double Yvsl = Hw+MapSplice["Tf_UP"]+MapSplice["Tf_DN"]-Yvsu;
	double Is   = ModelCalc.GetMapValueSecCo("I33");
	
	// 薑.睡 賅詮お 掘除曖 唸薑擎 м撩瞪婁 м撩�釋� 陝陝曖 賅詮お曖 м戲煎 唸薑и棻.
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
	cs.Format("    2) 檜擠っ曖 擬溘啗骯" );
	Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())
	{  // 綠м撩⑽
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
		cs.Format("        fu  : 檜擠っ曖 鼻翱擬溘");
		Arr.Add (cs);
		cs.Format("        fl  : 檜擠っ曖 ж翱擬溘");
		Arr.Add (cs);
		cs.Format("        Mw  : 犒睡縑 濛辨ж朝 賅詮お,  M =  %.2f kN.m ", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("        Isw : 鬼營爾欽賊曖 醞董蹴縑 渠и 檜擠っ曖 識欽賊 2離 賅詮お");
		Arr.Add (cs);
		cs.Format("        Ysu, Ysl : 鬼營爾 欽賊曖 醞董蹴縑憮 檜擠っ 鼻,ж翱梱雖曖 剪葬");
		Arr.Add (cs);
		cs.Format("        Ysu  =  %.2f mm,      Ysl  =  %.2f mm", Ysu, Ysl);
		Arr.Add (cs);
		cs.Format("        Is  : 鬼營爾 欽賊曖 欽賊 2離 賅詮お,   Is = %.2f mm昂" , Is);
		Arr.Add (cs);
		cs.Format("                %4.1f x %5.1f昆", Tw, Hw);
		Arr.Add (cs);
		cs.Format("        Isw' = ---------------  + (%5.1f昌+ %5.1f昌) + %4.1f x %5.1f x ( %6.2f - %6.2f )昌 =  %.2f mm昂", Hw, Tw, Tw, Hw, Hw/2, Ysl, Isw2);
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
		cs.Format("        fu  : 檜擠っ曖 鼻翱擬溘");
		Arr.Add (cs);
		cs.Format("        fl  : 檜擠っ曖 ж翱擬溘");
		Arr.Add (cs);
		cs.Format("        Msw : 犒睡縑 濛辨ж朝 м撩瞪 堅薑ж醞縑 曖и 賅詮お,  Ms =  %.2f kN.m ", tokNM(Ms));
		Arr.Add (cs);
		cs.Format("        Mvw : 犒睡縑 濛辨ж朝 м撩�� 堅薑ж醞縑 曖и 賅詮お,  Mv =  %.2f kN.m ", tokNM(Mv));
		Arr.Add (cs);
		cs.Format("        Isw : 鬼營爾欽賊曖 醞董蹴縑 渠и 檜擠っ曖 識欽賊 2離 賅詮お");
		Arr.Add (cs);
		cs.Format("        Ivw : м撩欽賊曖 醞董蹴縑 渠и 檜擠っ曖 識欽賊 2離 賅詮お");
		Arr.Add (cs);
		cs.Format("        Ysu, Ysl : 鬼營爾 欽賊曖 醞董蹴縑憮 檜擠っ 鼻,ж翱梱雖曖 剪葬");
		Arr.Add (cs);
		cs.Format("        Ysu  =  %.2f mm,      Ysl  =  %.2f mm", Ysu, Ysl);
		Arr.Add (cs);
		cs.Format("        Yvu, Yvl : м撩欽賊曖 醞董蹴縑憮 檜擠っ 鼻,ж翱梱雖曖 剪葬");
		Arr.Add (cs);
		cs.Format("        Yvu  =  %.2f mm,      Yvl  =  %.2f mm", Yvsu, Yvsl);
		Arr.Add (cs);
		cs.Format("        ls  : 鬼營爾 欽賊曖 欽賊 2離 賅詮お,   ls = %.2f mm昂" , Is);
		Arr.Add (cs);
		cs.Format("        lv  : м撩 欽賊曖 欽賊 2離 賅詮お,     lv = %.2f mm昂" , Iv);
		Arr.Add (cs);
		cs.Format("                %4.1f x %5.1f昆", Tw, Hw);
		Arr.Add (cs);
		cs.Format("        Isw' = ---------------  + (%5.1f昌 + %5.1f昌) +  %4.1f x %5.1f x ( %6.2f - %6.2f )昌 =  %.2f mm昂", Hw, Tw, Tw, Hw, Hw/2, Ysl, Isw2);
		Arr.Add (cs);
		cs.Format("                     12");
		Arr.Add (cs);
		cs.Format("                %4.1f x %5.1f昆", Tw, Hw);
		Arr.Add (cs);
		cs.Format("        Ivw' = ---------------  + (%5.1f昌 + %5.1f昌) + %4.1f x %5.1f x ( %6.2f - %6.2f )昌 =  %.2f mm昂", Hw, Tw, Tw, Hw, Yvsl, Hw/2, Ivw2);
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
	dTable [0][7] = dTable [1][7] = -1.0;  // 熱霜睡營
	MakeIoTable (Arr, dTable, 2, 7);
	pSum = dTable [2];
	I    = pSum [7] + pSum [6];
	Isw  = I + pSum [3] * pow (Hw/2 - Ysl, 2);
	Ivw  = I + pSum [3] * pow (Yvsl - Hw/2, 2);
	Ssu  = Msw * Ysu / Isw + Mvw * Yvsu / Ivw;
	Ssl  = Msw * Ysl / Isw + Mvw * Yvsl / Ivw;
	cs.Format("        I   =  lo + AY昌 =  %.1f  +  %.1f  =  %.2f", pSum [7], pSum [6], I);
	Arr.Add (cs);
	cs.Format("        Isw =  %.1f  +  %.2f x ( %6.2f - %6.2f )昌 =  %.2f mm昂", I, pSum [3], Hw/2, Ysl, Isw);
	Arr.Add (cs);
	if (pDB->GetNumHaiseokCombo())
	{  // 綠м撩⑽
		cs.Format("        檜擠っ曖 擬溘 " );
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
		cs.Format("        Ivw =  %.1f  +  %.2f x ( %6.2f - %6.2f )昌 =  %.2f mm昂", I, pSum [3], Yvsl, Hw/2, Ivw);
		Arr.Add (cs);
		cs.Format("        檜擠っ曖 擬溘 " );
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
//  跡瞳 :  I高 掘ж朝 紫ル斜葬晦.
//  殮溘 :  Arr           :  紫ル String擊 盪濰й CStringArray.
//          pTable [][0]  :  Plate 偃熱
//          pTable [][1]  :  Plate ァ
//          pTable [][2]  :  Plate 舒眷
//          pTable [][4]  :  Y高
//          pTable [][7]  :  Plate 寞щ ( + : 熱ゎ,  - : 熱霜)
//          nCount        :  殮溘и Line熱
//          nLMargin      :  豭薹 罹寥 高
//  轎溘 :  pTable [][3]  :  A高 (賊瞳)
//          pTable [][5]  :  AY高
//          pTable [][6]  :  AY昌高
//          pTable [][7]  :  Io高
//          nCount廓簞 Line(殮溘и Line 棻擠)縑朝 м啗高 菟橫氈擠.
//
void CPlateStressOut::MakeIoTable(CStringArray &Arr, double pTable[][8], int nCount, int nLMargin)
{
	// м啗 Line 蟾晦��.
	int i = 0;
	for (i = 3; i < 8; i++)
		pTable [nCount][i] = 0.0;

	// Table高 濛撩.
	for (i = 0; i < nCount; i++)   {
		pTable [i][3] = pTable [i][0] * pTable [i][1] * pTable [i][2];   // 'A'
		pTable [i][5] = pTable [i][3] * pTable [i][4];                   // 'AY'
		pTable [i][6] = pTable [i][4] * pTable [i][5];                   // 'AY昌'
		if (pTable [i][7] > 0)   // 熱ゎ 睡營賊
			pTable [i][7] = pTable [i][0] * pTable [i][1] * pow (pTable [i][2], 3) / 12;   // 'Io'
		else   // 熱霜 睡營賊
			pTable [i][7] = pTable [i][0] * pTable [i][2] * pow (pTable [i][1], 3) / 12;   // 'Io'
		for (int j = 3; j < 8; j++)   // м啗 啗骯.
			pTable [nCount][j] += pTable [i][j];
	}
	// 紫ル 轎溘.
	CString cs, sLM;
	sLM.Format("%*c", nLMargin, ' ');
	cs.Format("%s旨收收收收收收收收收收有收收收收收有收收收收有收收收收收有收收收收收收有收收收收收收收旬", sLM);
	Arr.Add (cs);
//	cs.Format("%s早                    早    A   早    Y   早    AY    早     AY昌   早     Io     早", sLM);
	cs.Format("%s早                    早 A (mm昌) 早  Y (mm)早 AY (mm昆)早  AY昌(mm昂)早   Io (m昂)   早", sLM);
	Arr.Add (cs);
	cs.Format("%s曳收收收收收收收收收收朱收收收收收朱收收收收朱收收收收收朱收收收收收收朱收收收收收收收朽", sLM);
	Arr.Add (cs);
	for (i = 0; i < nCount; i++)
	{
		if (pTable [i][4] != 0.0)
			cs.Format("%s早%2g - PL%6g x%4g 早%9.2f 早%7.2f 早%9.1f 早%11.1f 早%13.1f 早", sLM,
						pTable[i][0], pTable[i][1], pTable[i][2], pTable[i][3],
						pTable[i][4], pTable[i][5], pTable[i][6], pTable[i][7]);
		else   // Y高 0檜賊 ' - ' 轎溘.
			cs.Format("%s早%2g - PL%6g x%4g 早%9.2f 早    -   早     -    早      -     早%13.1f 早", sLM,
						pTable [i][0], pTable [i][1], pTable [i][2], pTable [i][3], pTable [i][7]);
		Arr.Add (cs);
	}
//		         早 4 - PL   151 x  12 早  42.40 早 102.25 早 123456.0 早    12578.1 早   123885.0 早
//		         早10 - PL1510.5 x12.5 早1000.00 早 150.00 早1500000.0 早123456789.1 早   123885.5 早
	cs.Format("%s曳收收收收收收收收收收朱收收收收收朱收收收收朱收收收收收朱收收收收收收朱收收收收收收收朽", sLM);
	Arr.Add (cs);
	cs.Format("%s早     м      啗     早%9.2f 早        早%9.1f 早%11.1f 早%13.1f 早", sLM,
						pTable [nCount][3], pTable [nCount][5], pTable [nCount][6], pTable [nCount][7]);
	Arr.Add (cs);
	cs.Format("%s曲收收收收收收收收收收朴收收收收收朴收收收收朴收收收收收朴收收收收收收朴收收收收收收收旭", sLM);
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
	if(bDistance)	cs.Format("< 欽賊 擬溘 匐饜 >  剪渦%d, 瞰薄%d 戲煎睡攪 %.3fm", nG+1, nNode+1, dDistance);
	else			cs.Format("< 欽賊 擬溘 匐饜 >  剪渦%d, 瞰薄%d", nG+1, nNode+1);

	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(!bHAPSUNG)	return;
	// м撩 掖榆 橾陽
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

	// 餌辨鬼謙 掘ж晦
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

	cs.Format("1) 欽賊 陛薑 塽 撲啗褻勒 ");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	CalcGen.GetMaterial(pBx, G_F_U, sLeft, sRight);
	cs.Format("					                     輿睡營 餌辨鬼謙  : 鼻睡 Ы楠雖 %s%s",			sLeft, sRight);	Arr.Add(cs);
	CalcGen.GetMaterial(pBx, G_F_L, sLeft, sRight);
	cs.Format("					                                        ж睡 Ы楠雖 %s%s",			sLeft, sRight);	Arr.Add(cs);
	CalcGen.GetMaterial(pBx, G_W, sLeft, sRight);
	cs.Format("					                                        犒睡っ %s%s",				sLeft, sRight);	Arr.Add(cs);
	cs.Format("					                     H    = %6.1f mm (剪渦犒睡曖 堪檜)",	H);			Arr.Add(cs);
	cs.Format("					                     B    = %6.1f mm (蝸楚粽 嶸�褪�)",		B);			Arr.Add(cs);
	cs.Format("					                     Bu   = %6.1f mm (Ы楠雖 鼻睡曖 傘綠)",	Bu);		Arr.Add(cs);
	cs.Format("					                     Bl   = %6.1f mm (Ы楠雖 ж睡曖 傘綠)",	Bl);		Arr.Add(cs);
	cs.Format("					                     Tc   = %6.1f mm (蝸楚粽 舒眷)",		Tc);		Arr.Add(cs);
	cs.Format("					                     Th   = %6.1f mm (ь纂 堪檜)",			Th);		Arr.Add(cs);
	cs.Format("					                     Tu   = %6.1f mm (鼻っ 舒眷)",			Tu);		Arr.Add(cs);
	cs.Format("					                     Tl   = %6.1f mm (жっ 舒眷)",			Tl);		Arr.Add(cs);
	cs.Format("					                     Tw   = %6.1f mm (WEB 舒眷)",			Tw);		Arr.Add(cs);
	cs.Format("					                     k    = %6.3f    (賅詮お 艙щ啗熱)",	k);			Arr.Add(cs);
	if(pBx->IsHybrid())
	{
		cs.Format("					                     R    = %6.3f    (ж檜粽葬萄 啗熱)",	R);		Arr.Add(cs);
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

	cs.Format(" 旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收有收收收收有收收收收收收收收收旬");		Arr.Add(cs);
	cs.Format(" 早                   掘            碟                         早 欽 嬪  早  殮溘 DATA       早");		Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Es  : 鬼曖 驕撩啗熱                                        早  MPa   早%15.1f   早",dEst);			Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Ec  : 夔觼葬お曖 驕撩啗熱                                  早  MPa   早%15.1f   早",Ec);				Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 亙s : 夥款っ 夔觼葬お曖 勒褻熱蹴縑 曖и 譆謙熱蹴睦         早        早%15.5f   早",Epsilon);		Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 丟  : 鬼營 塽 夔觼葬お曖 摹ぴ璽啗熱                        早        早%15.6f   早",Alpha);			Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 白2 : 勒褻熱蹴縑曖и 觼葬檜Щ啗熱                          早        早%15.f   早",CreepCoef2);		Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 n   : 鬼婁 夔觼葬お曖 驕撩啗熱綠                           早        早%15.f   早",n);				Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 t   : 鬼婁 夔觼葬お曖 螞紫離                               早  ﹎    早%15.f   早", t);				Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Ms  : м撩瞪 堅薑ж醞縑 曖и 賅詮お                        早 kN﹞m  早%15.3f   早",Ms);				Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Msc : м撩�� 堅薑ж醞縑 曖и 賅詮お                        早 kN﹞m  早%15.3f   早",Msc);			Arr.Add(cs);
	if(bWind)
	{
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Mw  : Ёж醞縑曖и 賅詮お                                  早 kN﹞m  早%15.3f   早",Mw);				Arr.Add(cs);
	}
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Mv  : (�勒狫� + 雖薄藹ж)縑 曖и 賅詮お                    早 kN﹞m  早%15.3f   早",Mv);				Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Ss  : м撩瞪 堅薑ж醞縑 曖и 瞪欽溘                        早   kN   早%15.3f   早",Ss);				Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Ssc : м撩�� 堅薑ж醞縑 曖и 瞪欽溘                        早   kN   早%15.3f   早",Ssc);			Arr.Add(cs);
	if(bWind)
	{
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Sw  : Ёж醞縑曖и 瞪欽溘                                  早   kN   早%15.3f   早",Sw);				Arr.Add(cs);
	}
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Sv  : (�勒狫� + 雖薄藹ж)縑 曖и 瞪欽溘                    早   kN   早%15.3f   早",Sv);				Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Mst : м撩瞪 堅薑ж醞縑 曖и 綠ぎ葡 賅詮お                 早 kN﹞m  早%15.3f   早",Mst);			Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Msct: м撩�� 堅薑ж醞縑 曖и 綠ぎ葡 賅詮お                 早 kN﹞m  早%15.3f   早",Msct);			Arr.Add(cs);
	if(bWind)
	{
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Mwt : Ёж醞縑曖и 綠ぎ葡 賅詮お                           早  kN    早%15.3f   早",Mwt);			Arr.Add(cs);
	}
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 Mvt : (�勒狫� + 雖薄藹ж)縑 曖и 綠ぎ葡 賅詮お             早 kN﹞m  早%15.3f   早",Mvt);			Arr.Add(cs);
	if(!bPLUS_M)
	{
	cs.Format(" 曳收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朱收收收收朱收收收收收收收收收朽");		Arr.Add(cs);
	cs.Format(" 早 tc  : 掖蹴寞щ 繹斬曖 Я犒舒眷(鼻睡,ж睡)                  早  mm    早   %5.1f, %5.1f   早",tcu,tcl);Arr.Add(cs);
	}
	cs.Format(" 曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朴收收收收朴收收收收收收收收收旭");		Arr.Add(cs);

	cs.Format("   < 霤堅 > м撩瞪 堅薑ж醞 : 鬼營濠醞 + 夥款っ 夔觼葬お濠醞"); Arr.Add(cs);
	cs.Format("            м撩�� 堅薑ж醞 : 陪除 塽 寞�ㄩ� 濠醞 + PAVEMENT濠醞");Arr.Add(cs);	
	cs.Format(""); Arr.Add(cs);
	
	/////////////////////////////////////////
	cs.Format("  欽賊 薯錳");	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(" 旨收收收收收收收收有收收收收有收收收收有收收收收收有收收收收收有收收收收收收收有收收收收收收收收收有收收收收收收收收旬");	Arr.Add(cs);
	cs.Format(" 早 掘         碟  早 B(mm)  早 H(mm)  早  A(mm昌) 早  Y(mm)   早  A﹞Y(mm昆)  早   A﹞Y昌( mm昂)  早    Io( mm昂)   早");	Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收朱收收收收朱收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	cs.Format(" 早1  -  UPPER FL. 早  %5.0f 早 %6.0f 早 %8.1f 早 %8.1f 早 %12.1f 早 %16.1f 早 %14.1f 早"
		,up_f_w, up_f_t, ModelCalc.GetMapValueSecCo("As_UP"), ModelCalc.GetMapValueSecCo("Y_UP")
		,ModelCalc.GetMapValueSecCo("Asy_UP"), ModelCalc.GetMapValueSecCo("Asy2_UP")
		,ModelCalc.GetMapValueSecCo("Iyy_UP"));	Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收朱收收收收朱收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	cs.Format(" 早1  -     WEB.   早  %5.0f 早 %6.0f 早 %8.1f 早 %8.1f 早 %12.1f 早 %16.1f 早 %14.1f 早"
		,w_t, w_h, ModelCalc.GetMapValueSecCo("As_WEB"), ModelCalc.GetMapValueSecCo("Y_WEB")
		,ModelCalc.GetMapValueSecCo("Asy_WEB"), ModelCalc.GetMapValueSecCo("Asy2_WEB"), ModelCalc.GetMapValueSecCo("Iyy_WEB"));		Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收朱收收收收朱收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	cs.Format(" 早1  -  LOWER FL. 早  %5.0f 早 %6.0f 早%9.1f 早 %8.1f 早 %12.1f 早 %16.1f 早 %14.1f 早"
		,lo_f_w, lo_f_t, ModelCalc.GetMapValueSecCo("As_DN")
		,ModelCalc.GetMapValueSecCo("Y_DN"), ModelCalc.GetMapValueSecCo("Asy_DN")
		,ModelCalc.GetMapValueSecCo("Asy2_DN"), ModelCalc.GetMapValueSecCo("Iyy_DN"));	Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收朱收收收收朴收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	cs.Format(" 早  鬼欽賊 м啗   早                  早%9.1f 早          早 %12.1f 早 %16.1f 早 %14.1f 早"
		,ModelCalc.GetMapValueSecCo("As_SUM"), ModelCalc.GetMapValueSecCo("Asy_SUM")
		,ModelCalc.GetMapValueSecCo("Asy2_SUM"), ModelCalc.GetMapValueSecCo("Iyy_SUM")); Arr.Add(cs);
	
	if(!bPLUS_M)
	{
	cs.Format(" 曳收收收收收收收收朱收收收收有收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	cs.Format(" 早  UPPER RE-BAR  早                  早%9.1f 早 %8.1f 早 %12.1f 早 %16.1f 早              早"
		,Aru, (H/2+Th+Tc)-tcu, Aru*((H/2+Th+Tc)-tcu),Aru*pow(((H/2+Th+Tc)-tcu),2) );
	Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收朱收收收收朱收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	cs.Format(" 早  LOWER RE-BAR  早                  早%9.1f 早 %8.1f 早 %12.1f 早 %16.1f 早              早"
		,Arl, (H/2+Th)+tcl, Arl*((H/2+Th)+tcl),Arl*pow(((H/2+Th)+tcl),2) );
	Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收朱收收收收朱收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	cs.Format(" 早   RE-BAR м啗  早                  早%9.1f 早        早 %12.1f 早 %16.1f 早              早"
		,Ar,Aru*((H/2+Th+Tc)-tcu) + Arl*((H/2+Th)+tcl),Aru*pow(((H/2+Th+Tc)-tcu),2) + Arl*pow(((H/2+Th)+tcl),2) );							Arr.Add(cs);
	cs.Format(" 曳收收收收收收收收朱收收收收朱收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	}
	else
	cs.Format(" 曳收收收收收收收收朱收收收收有收收收收朱收收收收收朱收收收收收朱收收收收收收收朱收收收收收收收收收朱收收收收收收收收朽");	Arr.Add(cs);
	
	
	cs.Format(" 早1  -  RC SLAB   早  %5.0f 早 %6.0f 早%9.1f 早 %8.1f 早 %12.1f 早 %16.1f 早 %14.1f 早"
		,B,Tc,ModelCalc.GetMapValueSecCo("As_SLAB"), ModelCalc.GetMapValueSecCo("Y_SLAB")
		,ModelCalc.GetMapValueSecCo("Asy_SLAB"), ModelCalc.GetMapValueSecCo("Asy2_SLAB")
		,ModelCalc.GetMapValueSecCo("Iyy_SLAB"));	Arr.Add(cs);
	cs.Format(" 曲收收收收收收收收朴收收收收朴收收收收朴收收收收收朴收收收收收朴收收收收收收收朴收收收收收收收收收朴收收收收收收收收旭");	Arr.Add(cs);
	cs.Format("   * As = %7.2f mm昌, Ac = %7.2f mm昌" ,ModelCalc.GetMapValueSecCo("As_SUM"), ModelCalc.GetMapValueSecCo("As_SLAB"));
	if(!bPLUS_M)
	{	
		str.Format(" , Ar = %7.2f mm昌" , Ar);
		Arr.Add(cs+str);
	}
	else
	{
		Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("2) 擬溘 匐饜");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);    

	cs.Format("  (1) м撩瞪");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format(" Is╞= (A ﹞Y昌) + Io           = %.3f mm昂", ModelCalc.GetMapValueSecCo("IsApo"));			Arr.Add(cs);
	cs.Format(" 乩s = ３(A ﹞Y) / As           = %.3f mm", ModelCalc.GetMapValueSecCo("de"));				Arr.Add(cs);
	cs.Format(" Is = Is╞- As ▼(乩s)昌        = %.3f mm昂", ModelCalc.GetMapValueSecCo("I33"));			Arr.Add(cs);
	cs.Format(" Ysu = H / 2 + Tu - 乩s         = %.3f mm", ModelCalc.GetMapValueSecCo("Ysu"));				Arr.Add(cs);
	cs.Format(" Ysl = Ysu - ( H + Tu + Tl )    = %.3f mm", ModelCalc.GetMapValueSecCo( "Ysl"));				Arr.Add(cs);
	cs.Format(" fsu = Ms / Is ▼ Ysu           = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_su"));	Arr.Add(cs);
	cs.Format(" fsl = Ms / Is ▼ Ysl           = %.3f mm昂", StressCalc.GetMapValueSecStress( "Sigma_sl"));	Arr.Add(cs);
	cs.Format(" 件s = Ss / A(web) + Mst ﹞Tw/K = %.3f mm昂", StressCalc.GetMapValueSecStress( "Tau_s"));	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (2) м撩��");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 薑賅詮お
	{	
		cs.Format(" Av = As + Ac/n                 = %.3f mm昌",ModelCalc.GetMapValueSecCo("Av_SUM"));	Arr.Add(cs);
		cs.Format(" Ic / n                         = %.3f mm昂",ModelCalc.GetMapValueSecCo("Igcs"));	Arr.Add(cs);
		cs.Format(" dv = Ysu - Tu + Th + Tc/2      = %.3f mm",ModelCalc.GetMapValueSecCo("dv"));		Arr.Add(cs);
		cs.Format(" dvc = dv ▼ As / ( Ac/n + As ) = %.3f mm",ModelCalc.GetMapValueSecCo("dvc"));		Arr.Add(cs);
		cs.Format(" dvs = dv - dvc                 = %.3f mm",ModelCalc.GetMapValueSecCo( "dvs"));		Arr.Add(cs);
		cs.Format(" Yvcu = dvc + Tc/2              = %.3f mm",ModelCalc.GetMapValueSecCo("Yvcu"));		Arr.Add(cs);
		cs.Format(" Yvcl = Yvcu - Tc               = %.3f mm",ModelCalc.GetMapValueSecCo("Yvcl"));		Arr.Add(cs);
		cs.Format(" Yvsu = Yvcu - H2 + Tu          = %.3f mm",ModelCalc.GetMapValueSecCo("Yvsu"));		Arr.Add(cs);
		cs.Format(" Yvsl = Yvsu - ( H + Tu + Tl )  = %.3f mm",ModelCalc.GetMapValueSecCo("Yvsl"));		Arr.Add(cs);
		cs.Format(" Iv = Is + Ic/n + As ▼ dvs昌 + Ac/n ▼ dvc昌 = %.3f mm昂", ModelCalc.GetMapValueSecCo("Iv33"));	Arr.Add(cs);
		cs.Format(" fvcu = Msc ▼ Yvcu / ( n ▼Iv ) = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcu"));		Arr.Add(cs);
		cs.Format(" fvcl = Msc ▼ Yvcl / ( n ▼Iv ) = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcl"));		Arr.Add(cs);
		cs.Format(" fvsu = Msc ▼ Yvsu / Iv         = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsu"));		Arr.Add(cs);
		cs.Format(" fvsl = Msc ▼ Yvsl / Iv         = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsl")); 		Arr.Add(cs);
		cs.Format(" K    = K + B ▼ (Tc / n)昆/ 3   = %.3f mm昂",ModelCalc.GetMapValueSecCo("Kv"));				Arr.Add(cs);
	}
	else    // 睡賅詮お
	{
		cs.Format(" Av = As + Ar = %.3f mm昌"						,StressCalc.GetMapValueSecStress( "Av"));		Arr.Add(cs);
		cs.Format(" d = Ysu - Tu + Th + tcl + (Tc - tcu - tcl) ▼Aru / Ar = %.3f mm",toCm(StressCalc.GetMapValueSecStress( "d")) );		Arr.Add(cs);
		cs.Format(" dr = d ▼As / Av = %.3f mm"					,StressCalc.GetMapValueSecStress( "dr"));		Arr.Add(cs);
		cs.Format(" ds = d - dr = %.3f mm"						,StressCalc.GetMapValueSecStress( "ds"));		Arr.Add(cs);
		cs.Format(" Yvru = dr + (Tc - tcu - tcl) ▼ Arl / Ar = %.3f mm",StressCalc.GetMapValueSecStress( "Yvru"));	Arr.Add(cs);
		cs.Format(" Yvrl = dr - (Tc - tcu - tcl) ▼ Aru / Ar = %.3f mm",StressCalc.GetMapValueSecStress( "Yvrl"));	Arr.Add(cs);
		cs.Format(" Yvsu = Yvrl - tcl - Th + Tu      = %.3f mm" 	  ,StressCalc.GetMapValueSecStress( "Yvsu"));	Arr.Add(cs);
		cs.Format(" Yvsl = Yvsu - ( H + Tu + Tl )    = %.3f mm"  	  ,StressCalc.GetMapValueSecStress( "Yvsl"));	Arr.Add(cs);
		cs.Format(" Irv = Is + As ▼ ds昌 + Ar ▼ dr昌 = %.3f mm昂"	  ,StressCalc.GetMapValueSecStress( "Irv"));	Arr.Add(cs);
		cs.Format(" fvru = Msc ▼ Yvru / Irv = %.3f MPa"  		  ,StressCalc.GetMapValueSecStress( "Sigma_vcu"));	Arr.Add(cs);
		cs.Format(" fvrl = Msc ▼ Yvrl / Irv = %.3f MPa"		  	  ,StressCalc.GetMapValueSecStress( "Sigma_vcl"));	Arr.Add(cs);
		cs.Format(" fvsu = Msc ▼ Yvsu / Irv = %.3f MPa"			  ,StressCalc.GetMapValueSecStress( "Sigma_vsu"));	Arr.Add(cs);
		cs.Format(" fvsl = Msc ▼ Yvsl / Irv = %.3f MPa"			  ,StressCalc.GetMapValueSecStress( "Sigma_vsl"));	Arr.Add(cs);
		cs.Format(" K    = K + B ▼ (Tc / n)昆/ 3   = %.3f mm昂",ModelCalc.GetMapValueSecCo("Kv"));				Arr.Add(cs);
	}
	cs.Format(" 件sv  = Ssc / A(web) + Msct ﹞Tw/K = %.3f MPa", StressCalc.GetMapValueSecStress( "Tau_sv"));	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	long nCountNO = 3;
	cs.Format("  (%d) �勒狫�", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 薑賅詮お
	{	
		cs.Format(" fvcu = Mv﹞Yvcu / (n﹞Iv)     = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcu_l"));		Arr.Add(cs);
		cs.Format(" fvcl = Mv﹞Yvcl / (n﹞Iv)     = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vcl_l"));		Arr.Add(cs);
		cs.Format(" fvsu = Mv﹞Yvsu / Iv          = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsu_l"));		Arr.Add(cs);
		cs.Format(" fvsl = Mv﹞Yvsl / Iv          = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsl_l"));		Arr.Add(cs);
		cs.Format(" 任 = Sv / A(Web) + Mvt ﹞Tw/K = %.3f MPa",StressCalc.GetMapValueSecStress("Tau_sv_l"));			Arr.Add(cs);
	}
	else   // 睡賅詮お
	{
		cs.Format(" fvru = Mv﹞Yvru / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcu_l"));		Arr.Add(cs);
		cs.Format(" fvrl = Mv﹞Yvrl / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcl_l"));		Arr.Add(cs);
		cs.Format(" fvsu = Mv﹞Yvsu / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsu_l"));		Arr.Add(cs);
		cs.Format(" fvsl = Mv﹞Yvsl / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsl_l"));		Arr.Add(cs);
		cs.Format(" 任 = Sv / A(Web) + Mvt ﹞Tw/K = %.3f MPa",StressCalc.GetMapValueSecStress( "Tau_sv_l"));			Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bWind)
	{
		cs.Format("  (%d) Ёж醞 - �勒狫� 綠營ж衛曖 Ёж醞縑 曖и 擬溘高戲煎 �勒狫� 營ж衛縑朝 啗骯脹 高曖 1/2擊 瞳辨", nCountNO++);
		Arr.Add(cs);
		cs.Format(""); Arr.Add(cs);

		if(bPLUS_M)    // 薑賅詮お
		{	
			cs.Format(" fvcu = Mw﹞Yvcu / (n﹞Iv)     = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcu_w"));		Arr.Add(cs);
			cs.Format(" fvcl = Mw﹞Yvcl / (n﹞Iv)     = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcl_w"));		Arr.Add(cs);
			cs.Format(" fvsu = Mw﹞Yvsu / Iv          = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsu_w"));		Arr.Add(cs);
			cs.Format(" fvsl = Mw﹞Yvsl / Iv          = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsl_w"));		Arr.Add(cs);
			cs.Format(" 任 = Sw / A(Web) + Mwt ﹞Tw/K = %.3f MPa", StressCalc.GetMapValueSecStress( "Tau_sv_w"));			Arr.Add(cs);
		}
		else   // 睡賅詮お
		{
			cs.Format(" fvru = Mw﹞Yvru / (n﹞Irv)    = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcu_w"));		Arr.Add(cs);
			cs.Format(" fvrl = Mw﹞Yvrl / (n﹞Irv)    = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vcl_w"));		Arr.Add(cs);
			cs.Format(" fvsu = Mw﹞Yvru / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsu_w"));		Arr.Add(cs);
			cs.Format(" fvsl = Mw﹞Yvrl / Irv         = %.3f MPa", StressCalc.GetMapValueSecStress( "Sigma_vsl_w"));		Arr.Add(cs);
			cs.Format(" 任 = Sw / A(Web) + Mwt ﹞Tw/K = %.3f MPa", StressCalc.GetMapValueSecStress( "Tau_sv_w"));		Arr.Add(cs);
		}
		cs.Format(""); Arr.Add(cs);
		cs.Format(""); Arr.Add(cs);
	}
	cs.Format("  (%d) Creep", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 薑賅詮お
	{	
		cs.Format(" 白1 = %.f"								 , StressCalc.GetMapValueSecStress( "CreepCoef1"));			Arr.Add(cs);
		cs.Format(" n1  = n ▼( 1 + 白1/2 )            = %.0f ", StressCalc.GetMapValueSecStress( "n1"));				Arr.Add(cs);
		cs.Format(" Av1 = As + Ac / n1                 = %.3f mm昌", StressCalc.GetMapValueSecStress( "Av1"));			Arr.Add(cs);
		cs.Format(" dc1 = dv ▼As / ( Ac/n1 + As )     = %.3f mm", StressCalc.GetMapValueSecStress( "Dc1"));			Arr.Add(cs);
		cs.Format(" ds1 = dv - dc1                     = %.3f mm", StressCalc.GetMapValueSecStress( "Ds1"));			Arr.Add(cs);
		cs.Format(" Yv1cu = dc1 + Tc/2                 = %.3f mm", StressCalc.GetMapValueSecStress( "Yv1cu"));			Arr.Add(cs);
		cs.Format(" Yv1cl = Yv1cu - Tc                 = %.3f mm", StressCalc.GetMapValueSecStress( "Yv1cl"));			Arr.Add(cs);
		cs.Format(" Yv1su = Yv1cu - H2 + Tu            = %.3f mm", StressCalc.GetMapValueSecStress( "Yv1su"));			Arr.Add(cs);
		cs.Format(" Yv1sl = Yv1su - ( H + Tu + Tl )    = %.3f mm",StressCalc.GetMapValueSecStress( "Yv1sl"));			Arr.Add(cs);
		cs.Format(" Iv1 = Is + Ic/n1 + As ▼ ds1昌 + Ac/n1 ▼ dc1昌 = %.3f mm昂", StressCalc.GetMapValueSecStress( "Iv1"));			Arr.Add(cs);
		cs.Format(" Nc = Msc ▼ dvc ▼ Ac / ( n ▼Iv ) = %.3f N",StressCalc.GetMapValueSecStress( "Nc"));				Arr.Add(cs);
		cs.Format(" P白 = Nc ▼ 2 ▼白1 / ( 2 + 白1 )  = %.3f N",StressCalc.GetMapValueSecStress( "PCreep"));			Arr.Add(cs);
		cs.Format(" M白 = P白 ▼ dc1                   = %.3f N.mm",StressCalc.GetMapValueSecStress( "MCreep"));		Arr.Add(cs);
		cs.Format(" fcu = Msc ▼ Yvcu / ( n ▼Iv )     = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cu"));		Arr.Add(cs);
		cs.Format(" fcl = Msc ▼ Yvcl / ( n ▼Iv )     = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cl"));		Arr.Add(cs);
		cs.Format(" fvcu1 = 1 / n1 ▼(P白/ Av1 + k ▼ M白▼ Yv1cu / Iv1 )  - Ec1﹞fcu﹞仰1/ Ec = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cu1"));			Arr.Add(cs);
		cs.Format(" fvcl1 = 1 / n1 ▼(P白/ Av1 + k ▼ M白▼ Yv1cl / Iv1 )  - Ec1﹞fcl﹞仰1/ Ec = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cl1"));			Arr.Add(cs);
		cs.Format(" fvsu1 = P白/ Av1 + k ▼ M白▼ Yv1su / Iv1 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsu1"));			Arr.Add(cs);
		cs.Format(" fvsl1 = P白/ Av1 + k ▼ M白▼ Yv1sl / Iv1 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsl1"));			Arr.Add(cs);
	}
	else   // 睡賅詮お
	{
		cs.Format(" Avc = As + Ac/n = %.3f mm昌"							,ModelCalc.GetMapValueSecCo("Av_SUM"));			Arr.Add(cs);
		cs.Format(" dv = Ysu - Tu + Th + Tc/2 = %.3f mm"					,ModelCalc.GetMapValueSecCo("dv"));				Arr.Add(cs);
		cs.Format(" dvc = dv ▼ As / ( Ac/n + As ) = %.3f mm"				,ModelCalc.GetMapValueSecCo("dvc"));			Arr.Add(cs);
		cs.Format(" dvs = dv - dvc = %.3f mm"								,ModelCalc.GetMapValueSecCo("dvs"));			Arr.Add(cs);
		cs.Format(" n1 = n ▼( 1 + 白1/2 ) = %.3f mm"						,StressCalc.GetMapValueSecStress( "n1"));		Arr.Add(cs);
		cs.Format(" Av1 = As + Ac / n1 = %.3f mm昌"							,StressCalc.GetMapValueSecStress( "Av1"));		Arr.Add(cs);
		cs.Format(" dc1 = dv ▼ As / ( Ac/n1 + As ) = %.3f mm"				,StressCalc.GetMapValueSecStress( "Dc1"));		Arr.Add(cs);
		cs.Format(" ds1 = dv - dc1 = %.3f mm"								,StressCalc.GetMapValueSecStress( "Ds1"));		Arr.Add(cs);
		cs.Format(" Icv = Is + Ic/n + As ▼ dvs昌 + Ac/n ▼dvc昌 = %.3f mm昂",StressCalc.GetMapValueSecStress( "Icv"));		Arr.Add(cs);
		cs.Format(" Nc = Msc ▼ dvc ▼ Ac / ( n ▼ Icv ) = %.3f N"			,StressCalc.GetMapValueSecStress( "Nc"));		Arr.Add(cs);
		cs.Format(" P白 = Nc ▼ 2 ▼白1 / ( 2 + 白1 ) = %.3f N"			,StressCalc.GetMapValueSecStress( "PCreep"));		Arr.Add(cs);
		cs.Format(" M白 = P白 ▼ dc1 = %.3f N.mm"							,StressCalc.GetMapValueSecStress( "MCreep"));	Arr.Add(cs);
		cs.Format(" fvru1 = P白/ Av + k ▼ M白 ▼ Yvru / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_cu1"));	Arr.Add(cs);
		cs.Format(" fvrl1 = P白/ Av + k ▼ M白 ▼ Yvrl / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_cl1"));	Arr.Add(cs);
		cs.Format(" fvsu1 = P白/ Av + k ▼ M白 ▼ Yvsu / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_vsu1"));	Arr.Add(cs);
		cs.Format(" fvsl1 = P白/ Av + k ▼ M白 ▼ Yvsl / Irv = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Sigma_vsl1"));	Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (%d) 勒褻熱蹴", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bPLUS_M)    // 薑賅詮お
	{	
		cs.Format(" n2  = n ▼( 1 + 白2 / 2 )        = %.0f ",StressCalc.GetMapValueSecStress( "n2"));				Arr.Add(cs);
		cs.Format("	Av2 = As + Ac / n2               = %.3f mm昌",StressCalc.GetMapValueSecStress( "Av2"));		Arr.Add(cs);
		cs.Format(" dc2 = dv ▼ As / ( Ac/n2 + As )  = %.3f mm"	,StressCalc.GetMapValueSecStress( "Dc2"));		Arr.Add(cs);
		cs.Format(" ds2 = dv - dc2                   = %.3f mm"	,StressCalc.GetMapValueSecStress( "Ds2"));		Arr.Add(cs);
		cs.Format(" Yv2cu = dc2 + Tc/2               = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2cu"));		Arr.Add(cs);
		cs.Format(" Yv2cl = Yv2cu - Tc               = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2cl"));		Arr.Add(cs);
		cs.Format(" Yv2su = Yv2cu - H2 - Tu          = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2su"));		Arr.Add(cs);
		cs.Format(" Yv2sl = Yv2su - ( H + Tu + Tl )  = %.3f mm",StressCalc.GetMapValueSecStress( "Yv2sl"));		Arr.Add(cs);
		cs.Format(" Iv2 = Is + Ic / n2 + As ▼ ds2昌 + Ac / n2 ▼ dc2昌 = %.3f mm昂",StressCalc.GetMapValueSecStress( "Iv2"));		Arr.Add(cs);
		cs.Format(" P2 = Es ▼亙s ▼ Ac / n2        = %.3f N",StressCalc.GetMapValueSecStress( "PShrink"));							Arr.Add(cs);
		cs.Format(" Mv2 = P2 ▼ dc2                 = %.3f N.mm",StressCalc.GetMapValueSecStress( "MShrink"));	Arr.Add(cs);
		cs.Format(" Ec2 = Es / n2                   = %.3f MPa",StressCalc.GetMapValueSecStress( "Ec2"));	Arr.Add(cs);
		cs.Format(" fvcu2 = 1  / n2 ▼( P2 / Av2 + k ▼ Mv2 ▼ Yv2cu / Iv2) - Ec2▼亙s = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cu2"));	Arr.Add(cs);
		cs.Format(" fvcl2 = 1  / n2 ▼( P2 / Av2 + k ▼ Mv2 ▼ Yv2cl / Iv2) - Ec2▼亙s = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cl2"));	Arr.Add(cs);
		cs.Format(" fvsu2 = P2 / Av2 + k ▼ Mv2 ▼ Yv2su / Iv2 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsu2"));	Arr.Add(cs);
		cs.Format(" fvsl2 = P2 / Av2 + k ▼ Mv2 ▼ Yv2sl / Iv2 = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_vsl2"));	Arr.Add(cs);
	}
	else   // 睡賅詮お
	{
		cs.Format(" n2  = n ▼( 1 + 白2 / 2 )                = %.3f ",StressCalc.GetMapValueSecStress("n2"));				Arr.Add(cs);
		cs.Format("	Av2 = As + Ac / n2                       = %.3f mm昌",StressCalc.GetMapValueSecStress("Av2"));			Arr.Add(cs);
		cs.Format(" dc2 = dv ▼As / ( Ac/n2 + As )           = %.3f mm",StressCalc.GetMapValueSecStress("Dc2"));			Arr.Add(cs);
		cs.Format(" ds2 = dv - dc2                           = %.3f mm",StressCalc.GetMapValueSecStress("Ds2"));			Arr.Add(cs);
		cs.Format(" P2 = Es ▼亙s ▼Ac / n2                  = %.3f N",StressCalc.GetMapValueSecStress("PShrink"));			Arr.Add(cs);
		cs.Format(" Mv2 = P2 ▼ dc2                          = %.3f N.mm",StressCalc.GetMapValueSecStress("MShrink"));		Arr.Add(cs);
		cs.Format(" fvru2 = P2 / Av + k ▼Mv2 ▼ Yvru / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_cu2"));		Arr.Add(cs);
		cs.Format(" fvrl2 = P2 / Av + k ▼Mv2 ▼ Yvrl / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_cl2"));		Arr.Add(cs);
		cs.Format(" fvsu2 = P2 / Av + k ▼Mv2 ▼ Yvsu / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsu2"));	Arr.Add(cs);
		cs.Format(" fvsl2 = P2 / Av + k ▼Mv2 ▼ Yvsl / Irv = %.3f MPa",StressCalc.GetMapValueSecStress("Sigma_vsl2"));	Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	
	cs.Format("  (%d) 螞 紫 離", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	double P1=StressCalc.GetMapValueSecStress( "P1"); 

	if(bPLUS_M)    // 薑賅詮お
	{	
		cs.Format(" 亙t = 丟▼汁T = %.6f ",StressCalc.GetMapValueSecStress( "Et") );				Arr.Add(cs);
		cs.Format(" P1 = Es ▼亙t ▼ Ac / n ) = %.3f N", P1);										Arr.Add(cs);
		cs.Format(" Mt = P1 ▼ dvc = %.3f N.mm"		,StressCalc.GetMapValueSecStress( "Mt"));		Arr.Add(cs);
		cs.Format(" Ect = Ec ▼亙t = %.3f MPa"		,Ec * StressCalc.GetMapValueSecStress( "Ect"));	Arr.Add(cs);			
		cs.Format(" ▽f cut = 1 / n ▼(P1 / Av + k ▼Mt ▼ Yvcu / Iv )  - Ect = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cut"));		Arr.Add(cs);
		cs.Format(" ▽f clt = 1 / n ▼(P1 / Av + k ▼Mt ▼ Yvcl / Iv )  - Ect = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_clt"));		Arr.Add(cs);
		cs.Format(" ▽f sut = P1 / Av + k ▼Mt ▼ Yvsu / Iv = %.3f MPa"				   ,StressCalc.GetMapValueSecStress( "Sigma_sut"));			Arr.Add(cs);
		cs.Format(" ▽f slt = P1 / Av + k ▼Mt ▼ Yvsl / Iv = %.3f MPa"				   ,StressCalc.GetMapValueSecStress( "Sigma_slt"));			Arr.Add(cs);
	}
	else        //  睡賅詮お
	{
		cs.Format(" 亙t = 丟▼汁T = %.6f ",StressCalc.GetMapValueSecStress( "Et") );		Arr.Add(cs);
		cs.Format(" P1 = Es ▼亙t ▼Ac / n = %.3f N", P1);									Arr.Add(cs);
		cs.Format(" Mt = P1 ▼ dvc = %.3f N.mm"	,StressCalc.GetMapValueSecStress( "Mt"));	Arr.Add(cs);
		cs.Format(" Ert = Es ▼亙t = %.3f MPa"	,StressCalc.GetMapValueSecStress( "Ert"));	Arr.Add(cs);			
		cs.Format(" ▽f rut = P1 / Av + k ▼ Mt ▼ Yvru / Irv ) - Ert = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_cut"));	Arr.Add(cs);
		cs.Format(" ▽f rlt = P1 / Av + k ▼ Mt ▼ Yvrl / Irv ) - Ert = %.3f MPa",StressCalc.GetMapValueSecStress( "Sigma_clt"));	Arr.Add(cs);
		cs.Format(" ▽f sut = P1 / Av + k ▼ Mt ▼ Yvsu / Irv = %.3f MPa"		  ,StressCalc.GetMapValueSecStress( "Sigma_sut"));	Arr.Add(cs);
		cs.Format(" ▽f slt = P1 / Av + k ▼ Mt ▼ Yvsl / Irv = %.3f MPa" 		  ,StressCalc.GetMapValueSecStress( "Sigma_slt"));	Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

// 瞪欽 塽 綠ぎ葡 牖睡 綠ぎ葡 堅溥
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
	cs.Format("  (%d) 瞪欽 塽 綠ぎ葡    犒睡 餌辨鬼營 : %s%s", nCountNO++, sLeft, sRight);		Arr.Add(cs);
	cs.Format("");										Arr.Add(cs);
	if(bWind)
	{
		cs.Format("- Ёж醞 綠營ж衛曖 瞪欽擬溘"); Arr.Add(cs);
	}
	cs.Format(" Mt  = Mst + Msct + Mvt                                                 = %.3f N.mm", StressCalc.GetMapValueSecStress("Mt"));		Arr.Add(cs);
	cs.Format(" Wn  = H ▼ B / 4                                                       = %.3f mm昌", StressCalc.GetMapValueSecStress("Wn"));		Arr.Add(cs);
	cs.Format(" lw  = (H昌▼ (Tu﹞Bu昆﹞Tb﹞Bl昆))/(12 ▼ (Tu﹞Bu昆+Tb﹞Bl昆))         = %.3f mm^6", StressCalc.GetMapValueSecStress("lw"));		Arr.Add(cs);
	cs.Format("  a  = L﹞∵((G﹞K)/(Es﹞Iw))                                           = %.3f", ModelCalc.GetMapValueSecCo("WarpingAlpha"));		Arr.Add(cs);
	cs.Format(" Mwa = (Mt / L) / (a / L)昌▼ (2﹞sinh(a/2) / sinh(a)-1)                = %.3f N.mm昌", StressCalc.GetMapValueSecStress("Mwa"));		Arr.Add(cs);
	cs.Format(" Twa = -(Mt / L) / (a/L) ▼ (1-cosh(a)) / sinh(a)))                     = %.3f N.mm",  StressCalc.GetMapValueSecStress("Twa"));		Arr.Add(cs);
	cs.Format(" Swa = H ▼ Tu ▼ Bl昌/ 16)                                             = %.3f mm昂", StressCalc.GetMapValueSecStress("Swa"));		Arr.Add(cs);
	cs.Format("  任 = 瞪欽溘縑 曖и 犒睡 瞪欽擬溘       : (Ss + Ssc + Sv) / As(Web)    = %.3f MPa", StressCalc.GetMapValueSecStress("Nu"));			Arr.Add(cs);
	cs.Format(" 任t = 牖熱綠ぎ葡縑 曖и 犒睡 瞪欽擬溘   : Mt﹞Tw / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_Web"));	Arr.Add(cs);
	cs.Format(" 任t = 牖熱綠ぎ葡縑 曖и Ы楠雖 瞪欽擬溘 : Mt﹞Tu / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_FL"));		Arr.Add(cs);
	cs.Format(" 任w = �首鮿硫窒� 曖и Ы楠雖 瞪欽擬溘   : Twa﹞Sw / (Iw﹞Tu)           = %.3f MPa", StressCalc.GetMapValueSecStress("Nuw"));		Arr.Add(cs);
	if(dNu_Web < dTauAllow)
		cs.Format("     任web    = 任 + 任t   = %4.1f   MPa  <  任a = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_Web"), dTauAllow);
	else
		cs.Format("     任web    = 任 + 任t   = %4.1f   MPa  >  任a = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_Web"), dTauAllow);
	Arr.Add(cs);
	if(dNu_FL < dTauAllow)
		cs.Format("     任flange = 任 + 任t   = %4.1f   MPa  <  任a = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_FL"), dTauAllow);
	else
		cs.Format("     任flange = 任 + 任t   = %4.1f   MPa  >  任a = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_FL"), dTauAllow);
	Arr.Add(cs);
	cs.Format("  fw = �首鮿硫窒� 曖и 熱霜擬溘          : Mwa﹞Wn / Iw                 = %.3f MPa", StressCalc.GetMapValueSecStress("Fw"));		Arr.Add(cs);

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

		cs.Format("- Ёж醞 營ж衛曖 瞪欽擬溘");Arr.Add(cs);

		cs.Format("  ﹞ �勒狫� 綠營ж衛");		Arr.Add(cs);
		cs.Format(" Mt  = Mst + Msct + Mwt                                                 = %.3f N.mm", StressCalc.GetMapValueSecStress("Mt_w"));		Arr.Add(cs);
		cs.Format(" Wn  = H ▼ B / 4                                                       = %.3f mm昌", StressCalc.GetMapValueSecStress("Wn_w"));		Arr.Add(cs);
		cs.Format(" lw  = (H昌▼ (Tu﹞Bu昆﹞Tb﹞Bl昆))/(12 ▼ (Tu﹞Bu昆+Tb﹞Bl昆))         = %.3f mm^6", StressCalc.GetMapValueSecStress("lw_w"));		Arr.Add(cs);
		cs.Format("  a  = L﹞∵((G﹞K)/(Es﹞Iw))                                           = %.3f", ModelCalc.GetMapValueSecCo("WarpingAlpha"));		Arr.Add(cs);
		cs.Format(" Mwa	= (Mt / L) / (a / L)昌▼ (2﹞sinh(a/2) / sinh(a)-1)             = %.3f N.mm昌", StressCalc.GetMapValueSecStress("Mwa_w"));	Arr.Add(cs);
		cs.Format(" Twa = -(Mt / L) / ( a / L) ▼ (1-cosh(a)) / sinh(a)))                  = %.3f N.mm",  StressCalc.GetMapValueSecStress("Twa_w"));	Arr.Add(cs);
		cs.Format(" Swa = H ▼ Tu ▼ Bl昌/ 16)                                             = %.3f mm昂", StressCalc.GetMapValueSecStress("Swa_w"));		Arr.Add(cs);
		//////////////////////////////////////////////////////////////////////////
		cs.Format("  任 = 瞪欽溘縑 曖и 犒睡 瞪欽擬溘       : (Ss + Ssc + Sv) / As(Web)    = %.3f MPa", StressCalc.GetMapValueSecStress("Nu_w"));		Arr.Add(cs);
		cs.Format(" 任t = 牖熱綠ぎ葡縑 曖и 犒睡 瞪欽擬溘   : Mt﹞Tw / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_Web_w"));	Arr.Add(cs);
		cs.Format(" 任t = 牖熱綠ぎ葡縑 曖и Ы楠雖 瞪欽擬溘 : Mt﹞Tu / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_FL_w"));	Arr.Add(cs);
		cs.Format(" 任w = �首鮿硫窒� 曖и Ы楠雖 瞪欽擬溘   : Twa﹞Sw / (Iw﹞Tu)           = %.3f MPa", StressCalc.GetMapValueSecStress("Nuw_w"));	Arr.Add(cs);
//		if(dNu_Web_w < dTauAllow_w)
//			cs.Format("     任web    = 任 + 任t   = %4.1f   MPa  <  任a = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_Web_w"), dTauAllow_w);
//		else
//			cs.Format("     任web    = 任 + 任t   = %4.1f   MPa  >  任a = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_Web_w"), dTauAllow_w);
//		Arr.Add(cs);
//		if(dNu_FL_w < dTauAllow_w)
//			cs.Format("     任flange = 任 + 任t   = %4.1f   MPa  <  任a = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_FL_w"), dTauAllow_w);
//		else
//			cs.Format("     任flange = 任 + 任t   = %4.1f   MPa  >  任a = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_FL_w"), dTauAllow_w);
//		Arr.Add(cs);
		cs.Format("  fw = �首鮿硫窒� 曖и 熱霜擬溘          : Mwa﹞Wn / Iw                 = %.3f MPa", StressCalc.GetMapValueSecStress("Fw_w"));		Arr.Add(cs);
		if(dNu_w<dTauAllow_w && dNut_Web_w<dTauAllow_w && dNuw_w<dTauAllow_w)
			cs.Format("                            任,任t,任w  < 任a = %.3f MPa  ------ O.K", dTauAllow_w);
		else
			cs.Format("                            任,任t,任w  > 任a = %.3f MPa  ------ N.G", dTauAllow_w);
		Arr.Add(cs);
		//////////////////////////////////////////////////////////////////////////

		cs.Format("  ﹞ �勒狫� 營ж衛");	Arr.Add(cs);
		cs.Format(" Mt  = Mst + Msct + Mwt/2 + Mvt                                         = %.3f N.mm", StressCalc.GetMapValueSecStress("Mt_wl"));	Arr.Add(cs);
		cs.Format(" Wn  = H ▼ B / 4                                                       = %.3f mm昌", StressCalc.GetMapValueSecStress("Wn_wl"));	Arr.Add(cs);
		cs.Format(" lw  = (H昌▼ (Tu﹞Bu昆﹞Tb﹞Bl昆))/(12 ▼ (Tu﹞Bu昆+Tb﹞Bl昆))         = %.3f mm^6", StressCalc.GetMapValueSecStress("lw_wl"));	Arr.Add(cs);
		cs.Format("  a  = L﹞∵((G﹞K)/(Es﹞Iw))                                           = %.3f", ModelCalc.GetMapValueSecCo("WarpingAlpha"));		Arr.Add(cs);
		cs.Format(" Mwa	= (Mt / L) / (a / L)昌▼ (2﹞sinh(a/2) / sinh(a)-1)             = %.3f N.mm昌", StressCalc.GetMapValueSecStress("Mwa_wl"));Arr.Add(cs);
		cs.Format(" Twa = -(Mt / L) / (a/L) ▼ (1-cosh(a)) / sinh(a)))                     = %.3f N.mm",  StressCalc.GetMapValueSecStress("Twa_wl"));	Arr.Add(cs);
		cs.Format(" Swa = H ▼ Tu ▼ Bl昌/ 16)                                             = %.3f mm昂", StressCalc.GetMapValueSecStress("Swa_wl"));	Arr.Add(cs);
		//////////////////////////////////////////////////////////////////////////
		cs.Format("  任 = 瞪欽溘縑 曖и 犒睡 瞪欽擬溘       : (Ss + Ssc + Sv) / As(Web)    = %.3f MPa", StressCalc.GetMapValueSecStress("Nu_wl"));		Arr.Add(cs);
		cs.Format(" 任t = 牖熱綠ぎ葡縑 曖и 犒睡 瞪欽擬溘   : Mt﹞Tw / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_Web_wl"));	Arr.Add(cs);
		cs.Format(" 任t = 牖熱綠ぎ葡縑 曖и Ы楠雖 瞪欽擬溘 : Mt﹞Tu / K                   = %.3f MPa", StressCalc.GetMapValueSecStress("Nut_FL_wl"));	Arr.Add(cs);
		cs.Format(" 任w = �首鮿硫窒� 曖и Ы楠雖 瞪欽擬溘   : Twa﹞Sw / (Iw﹞Tu)           = %.3f MPa", StressCalc.GetMapValueSecStress("Nuw_wl"));	Arr.Add(cs);
//		if(dNu_Web_wl < dTauAllow_wl)
//			cs.Format("     任web    = 任 + 任t   = %4.1f   MPa  <  任a = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_Web_wl"), dTauAllow_wl);
//		else
//			cs.Format("     任web    = 任 + 任t   = %4.1f   MPa  >  任a = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_Web_wl"), dTauAllow_wl);
//		Arr.Add(cs);
//		if(dNu_FL_wl < dTauAllow_wl)
//			cs.Format("     任flange = 任 + 任t   = %4.1f   MPa  <  任a = %.3f Mpa  ------ O.K", StressCalc.GetMapValueSecStress("Nu_FL_wl"), dTauAllow_wl);
//		else
//			cs.Format("     任flange = 任 + 任t   = %4.1f   MPa  >  任a = %.3f Mpa  ------ N.G", StressCalc.GetMapValueSecStress("Nu_FL_wl"), dTauAllow_wl);
//		Arr.Add(cs);
		cs.Format("  fw = �首鮿硫窒� 曖и 熱霜擬溘          : Mwa﹞Wn / Iw                 = %.3f MPa", StressCalc.GetMapValueSecStress("Fw_wl"));		Arr.Add(cs);		
		if(dNu_wl<dTauAllow_wl && dNut_Web_wl<dTauAllow_wl && dNuw_wl<dTauAllow_wl)
			cs.Format("                            任,任t,任w  < 任a = %.3f MPa  ------ O.K", dTauAllow_wl);
		else
			cs.Format("                            任,任t,任w  > 任a = %.3f MPa  ------ N.G", dTauAllow_wl);
		Arr.Add(cs);
	}

	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	
	cs.Format("  (%d) 褻м擬溘曖 匐饜", nCountNO++);
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

	if(bPLUS_M)	//薑賅詮お
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
	else	//睡賅詮お
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
	if(bPLUS_M)	cs.Format(" # 檣濰 (ж睡)Ы楠雖曖 ъ辨擬溘 (%s%s 餌辨)",sLeftLo, sRightLo);
	else		cs.Format(" # 檣濰 (鼻睡)Ы楠雖曖 ъ辨擬溘 (%s%s 餌辨)",sLeftUp, sRightUp);
	Arr.Add(cs);
	cs.Format("   fta = %.3f MPa", dSigmaTA);	Arr.Add(cs);
	cs.Format("");								Arr.Add(cs);
	if(bPLUS_M)	cs.Format(" # 憶蹴 (鼻睡)Ы楠雖曖 ъ辨擬溘 (%s%s 餌辨)",sLeftUp, sRightUp);
	else		cs.Format(" # 憶蹴 (ж睡)Ы楠雖曖 ъ辨擬溘 (%s%s 餌辨)",sLeftLo, sRightLo);
	Arr.Add(cs);
	AllowBuckling.SetTypeAllow(ALLOW_BUCKLING_FREEJETTY);
	HGALLOWSTRESS vtAllowStress = AllowBuckling.m_AllowStress[nThickIdx];
	
	cs.Format("     b / %.1f ‾ tu                   檜賊, fca = 140 MPa", vtAllowStress.m_dCoeff2, dSigmaCAAfter);
	Arr.Add(cs);
	cs.Format("     b / %.1f ‾ tu < b / %.1f        檜賊, fca = %.0f(t/b)昌MPa",
		vtAllowStress.m_dCoeff2, vtAllowStress.m_dCoeff4, vtAllowStress.m_dCoeff3);
	Arr.Add(cs);

	//////////////////////////////////////////
	if(bPLUS_M)		cs.Format("    餌辨 Tu = %.3f mm", Tu);
	else			cs.Format("    餌辨 Tl = %.3f mm", Tl);
	Arr.Add(cs);

	if(bPLUS_M)
	{
		cs.Format("    夔觼葬お 蝸楚粽縑 曖п っ曖 措睡謝掉檜 醱碟�� 寞雖脹 唳辦檜嘎煎");
		Arr.Add(cs);
	}
	cs.Format("    評塭憮,  fca = %.3f MPa", dSigmaCABefore);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	double SigmaCCA=0;
	double SigmaCTA=0;

	CString RebarType="SD40";            // 繹斬曖 謙盟

	if(bPLUS_M)    // 薑賅詮お
	{
		cs.Format(" # 繹斬 夔觼葬お曖 ъ辨擬溘");
		Arr.Add(cs);
		if(b2K)
		{
			SigmaCCA = 0.4*pData->DESIGN_MATERIAL.m_dSigmaCK;
			cs.Format("   fca = 0.4  ▼ f ck = %.3f MPa", SigmaCCA);
			Arr.Add(cs);
		}
		else
		{
			SigmaCCA = 0.3*pData->DESIGN_MATERIAL.m_dSigmaCK;
			cs.Format("   fca = 0.3  ▼ f ck = %.3f MPa", SigmaCCA);
			Arr.Add(cs);
		}

		SigmaCTA = min(0.07*pData->DESIGN_MATERIAL.m_dSigmaCK, 2.5);

		cs.Format("   fta = 0.07 ▼ f ck = %.3f MPa", SigmaCTA);
		Arr.Add(cs);
	}
	else           // 睡賅詮お
	{
		cs.Format(" # RE-BAR曖 ъ辨擬溘");
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

	double Sigma_up,Sigma_lo,Sigma_upA,Sigma_loA; // 鼻, ж, ъ辨 鼻, ъ辨 ж
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
		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format(" 旨收收收收收收收收收有收收收收收收收收收收收收收收收收收收收有收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");	Arr.Add(cs);
			cs.Format(" 早                  早             CONCRETE                 早                        STEEL                         早");	Arr.Add(cs);
			cs.Format(" 早                  曳收收收收收收收收收收收收收有收收收收收朱收收收收收收收收收收收收收有收收收收收收收收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早    ж醞  褻м    早     濛辨 擬溘 ( MPa )    早 ъ辨擬溘 早     濛辨 擬溘 ( MPa )    早     ъ辨 擬溘 ( MPa )    早");	Arr.Add(cs);	
			cs.Format(" 早                  曳收收收收收收有收收收收收收朽          曳收收收收收收有收收收收收收朱收收收收收收有收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早                  早  鼻    翱  早  ж    翱  早 ( MPa )  早  鼻    翱  早  ж    翱  早  鼻    翱  早 ж     翱  早");	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaCABefore;
			Sigma_loA=dSigmaTA;

			Max_up=Sigma_up;
			Max_lo=Sigma_lo;
			Max_upA=Sigma_upA;
			Max_loA=Sigma_loA;

			cs.Format(" 早1                 早            早            早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3             早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" 早1+2+3             早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+5           早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cu1")
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" 早1+2+3+5           早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+5+6         早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+5+6         早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+5+6+7       早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+5+6+7       早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+5+6-7       早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+5+6-7       早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);


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
				cs.Format(" 早1+2+4             早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_w") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_w") 
						   ,SigmaCCA*1.2 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" 早1+2+4             早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_w") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_w") 
						   ,SigmaCTA*1.2 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+1/2(4)      早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCCA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" 早1+2+3+1/2(4)      早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0  
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+1/2(4)+5    早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+1/2(4)+5    早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
						   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+1/2(4)+5+6  早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+1/2(4)+5+6  早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+1/2(4)+5+6+7早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+1/2(4)+5+6+7早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+1/2(4)+5+6-7早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+1/2(4)+5+6-7早%9.3f   早%9.3f   早%9.3f 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曲收收收收收收收收收朴收收收收收收朴收收收收收收朴收收收收收朴收收收收收收朴收收收收收收朴收收收收收收朴收收收收收收旭");	Arr.Add(cs);


		}
		else     // 睡賅詮お
		{
			cs.Format(" 旨收收收收收收收收收有收收收收收收收收收收收收收收收收收收收有收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");	Arr.Add(cs);
			cs.Format(" 早                  早              RE-BAR                  早                        STEEL                         早");	Arr.Add(cs);
			cs.Format(" 早                  曳收收收收收收收收收收收收收有收收收收收朱收收收收收收收收收收收收收有收收收收收收收收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早    ж醞  褻м    早     濛辨 擬溘 ( MPa )    早 ъ辨擬溘 早     濛辨 擬溘 ( MPa )    早     ъ辨 擬溘 ( MPa )    早");	Arr.Add(cs);	
			cs.Format(" 早                  曳收收收收收收有收收收收收收朽          曳收收收收收收有收收收收收收朱收收收收收收有收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早                  早  鼻    翱  早  ж    翱  早   (MPa)  早  鼻    翱  早  ж    翱  早  鼻    翱  早 ж     翱  早");	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaTA;
			Sigma_loA=dSigmaCABefore;

			Max_up=Sigma_up;
			Max_lo=Sigma_lo;
			Max_upA=Sigma_upA;
			Max_loA=Sigma_loA;

			cs.Format(" 早1                 早            早            早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3             早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+5           早%9.3f   早%9.3f   早%9.3f  早%9.3f    | %9.3f  早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
			cs.Format(" 早1+2+3+5+6         早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+5+6+7       早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						+StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						+StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+5+6-7       早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						-StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						-StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+4             早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_w") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_w") 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+1/2(4)      早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+1/2(4)+5    早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+1/2(4)+5+6  早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						+StressCalc.GetMapValueSecStress("Sigma_vcu_w") / 2.0 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						+StressCalc.GetMapValueSecStress("Sigma_vcl_w") / 2.0 
					   ,SigmaCTA*1.25 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+1/2(4)+5+6+7早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曳收收收收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+1/2(4)+5+6-7早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曲收收收收收收收收收朴收收收收收收朴收收收收收收朴收收收收收朴收收收收收收朴收收收收收收朴收收收收收收朴收收收收收收旭");	Arr.Add(cs);

		}

	}
	else
	{
		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format(" 旨收收收收收收有收收收收收收收收收收收收收收收收收收收有收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");	Arr.Add(cs);
			cs.Format(" 早            早             CONCRETE                 早                        STEEL                         早");	Arr.Add(cs);
			cs.Format(" 早            曳收收收收收收收收收收收收收有收收收收收朱收收收收收收收收收收收收收有收收收收收收收收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早 ж醞  褻м 早     濛辨 擬溘 ( MPa )    早 ъ辨擬溘 早     濛辨 擬溘 ( MPa )    早     ъ辨 擬溘 ( MPa )    早");	Arr.Add(cs);	
			cs.Format(" 早            曳收收收收收收有收收收收收收朽          曳收收收收收收有收收收收收收朱收收收收收收有收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早            早  鼻    翱  早  ж    翱  早   (MPa)  早  鼻    翱  早  ж    翱  早  鼻    翱  早 ж     翱  早");	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

			Sigma_up	= StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo	= StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA	= dSigmaCABefore;
			Sigma_loA	= dSigmaTA;
			Max_up		= Sigma_up;
			Max_lo		= Sigma_lo;
			Max_upA		= Sigma_upA;
			Max_loA		= Sigma_loA;

			cs.Format(" 早1           早            早            早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3       早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,temp1,temp2,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" 早1+2+3       早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,temp1,temp2,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+4     早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCCA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			else
			{
				cs.Format(" 早1+2+3+4     早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+4+5   早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+4+5   早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
						   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")   
							+StressCalc.GetMapValueSecStress("Sigma_cu1") 
							+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")    
							+StressCalc.GetMapValueSecStress("Sigma_cl1") 
							+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			}
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+4+5+6 早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+4+5+6 早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
				cs.Format(" 早1+2+3+4+5-6 早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
				cs.Format(" 早1+2+3+4+5-6 早%9.3f   早%9.3f   早%9.3f   早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
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
			cs.Format(" 曲收收收收收收朴收收收收收收朴收收收收收收朴收收收收收朴收收收收收收朴收收收收收收朴收收收收收收朴收收收收收收旭");	Arr.Add(cs);
		}
		else     // 睡賅詮お
		{
			cs.Format(" 旨收收收收收收有收收收收收收收收收收收收收收收收收收收有收收收收收收收收收收收收收收收收收收收收收收收收收收收旬");	Arr.Add(cs);
			cs.Format(" 早            早              RE-BAR                  早                        STEEL                         早");	Arr.Add(cs);
			cs.Format(" 早            曳收收收收收收收收收收收收收有收收收收收朱收收收收收收收收收收收收收有收收收收收收收收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早 ж醞  褻м 早     濛辨 擬溘 ( MPa )    早 ъ辨擬溘 早     濛辨 擬溘 ( MPa )    早     ъ辨 擬溘 ( MPa )    早");	Arr.Add(cs);	
			cs.Format(" 早            曳收收收收收收有收收收收收收朽          曳收收收收收收有收收收收收收朱收收收收收收有收收收收收收朽");	Arr.Add(cs);
			cs.Format(" 早            早  鼻    翱  早  ж    翱  早   (MPa)  早  鼻    翱  早  ж    翱  早  鼻    翱  早 ж     翱  早");	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

			Sigma_up=StressCalc.GetMapValueSecStress("Sigma_su");
			Sigma_lo=StressCalc.GetMapValueSecStress("Sigma_sl");
			Sigma_upA=dSigmaTA;
			Sigma_loA=dSigmaCABefore;

			Max_up=Sigma_up;
			Max_lo=Sigma_lo;
			Max_upA=Sigma_upA;
			Max_loA=Sigma_loA;

			cs.Format(" 早1           早            早            早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
			cs.Format(" 早1+2+3       早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+4     早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l") 
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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
			cs.Format(" 早1+2+3+4+5   早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
					   ,SigmaCTA ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+4+5+6 早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						+StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						+StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曳收收收收收收朱收收收收收收朱收收收收收收朱收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朱收收收收收收朽");	Arr.Add(cs);

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

			cs.Format(" 早1+2+3+4+5-6 早%9.3f   早%9.3f   早%9.3f  早%9.3f   早%9.3f   早%9.3f   早%9.3f   早"
					   ,StressCalc.GetMapValueSecStress("Sigma_vcu") + StressCalc.GetMapValueSecStress("Sigma_vcu_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cu1") 
						+StressCalc.GetMapValueSecStress("Sigma_cu2") 
						-StressCalc.GetMapValueSecStress("Sigma_cut") 
					   ,StressCalc.GetMapValueSecStress("Sigma_vcl") + StressCalc.GetMapValueSecStress("Sigma_vcl_l")  
						+StressCalc.GetMapValueSecStress("Sigma_cl1") 
						+StressCalc.GetMapValueSecStress("Sigma_cl2") 
						-StressCalc.GetMapValueSecStress("Sigma_clt") 
					   ,SigmaCTA*1.15 ,Sigma_up ,Sigma_lo ,Sigma_upA ,Sigma_loA );	Arr.Add(cs);
			cs.Format(" 曲收收收收收收朴收收收收收收朴收收收收收收朴收收收收收朴收收收收收收朴收收收收收收朴收收收收收收朴收收收收收收旭");	Arr.Add(cs);
		}
	}

	cs.Format("    輿) 憶蹴擬溘擊 薑(+), 檣濰擬溘擊 睡(-)煎 ж艘擠.");
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (%d) м撩擬溘曖 匐饜", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format("     ( 鼻  翱 )");	Arr.Add(cs);
	
	double Tau		= StressCalc.GetMapValueSecStress("Tau_shear");
	double TauA		= dTauAllow;
	double Result	= pow((Max_up/Max_upA),2) + pow((Tau/TauA),2);

	if(bHybrid)	cs.Format("	    (f  / R▼fa)昌 + (任 / 任a)昌");
	else		cs.Format("	    (f  / fa)昌 + (任 / 任a)昌");
	Arr.Add(cs);
	if(fabs(Result) <= 1.2) 
	{		
		cs.Format("         =  (%.3f/%.3f)昌 + (%.3f/%.3f)昌 = %.4f < 1.2 ---- O.K"
						,Max_up,Max_upA*dHyR,Tau,TauA,Result);
	}
	else
	{
		cs.Format("         =  (%.3f/%.3f)昌 + (%.3f/%.3f)昌 = %.4f ￣1.2 ---- N.G"
						,Max_up,Max_upA,Tau,TauA,Result);
	}
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("     ( ж  翱 )");	Arr.Add(cs);
	Result  = pow((Max_lo/Max_loA),2) + pow((Tau/TauA),2);
	
	if(bHybrid)	cs.Format("	    (f  / R▼fa)昌 + (任 / 任a)昌");
	else		cs.Format("	    (f  / fa)昌 + (任 / 任a)昌");
	Arr.Add(cs);
	if(fabs(Result) <= 1.2) 
	{
		cs.Format("         =  (%.3f/%.3f)昌 + (%.3f/%.3f)昌 = %.4f < 1.2 ---- O.K"
						,Max_lo,Max_loA,Tau,TauA,Result);
	}
	else
	{
		cs.Format("         =  (%.3f/%.3f)昌 + (%.3f/%.3f)昌 = %.4f ￣1.2 ---- N.G"
						,Max_lo,Max_loA,Tau,TauA,Result);	}
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("  (%d) о犒縑 渠и 寰瞪紫 匐餌", nCountNO++);
	Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	if(bWind)
	{
		cs.Format("   - Ёж醞 綠營ж衛");
		Arr.Add(cs);  
		cs.Format("     甲f = 1.3 ▼ (м撩瞪 堅薑ж醞 擬溘 + м撩�� 堅薑ж醞) + 2.15 ▼ �勒狫萷� 曖и 擬溘(醱問んл)");
		Arr.Add(cs);  
		cs.Format("           + (觼葬Щ擬溘 滲�風�) + (勒褻熱蹴擬溘滲�風�) + (螞紫離擬溘滲�風�) 曖 褻м 醞縑憮");
		Arr.Add(cs);  
		cs.Format("           陛濰 碳葬и ж醞 褻м縑 渠ж罹 匐饜");
		Arr.Add(cs);  
		cs.Format("   - Ёж醞 營ж衛");
		Arr.Add(cs);  
		cs.Format("    * �勒狫� 綠營ж衛");
		Arr.Add(cs);  
		cs.Format("     甲f = 1.3 ▼ (м撩瞪 堅薑ж醞 擬溘 + м撩�� 堅薑ж醞) + 1.3 ▼ Ёж醞縑 曖и 擬溘");
		Arr.Add(cs);  
		cs.Format("           + (觼葬Щ擬溘 滲�風�) + (勒褻熱蹴擬溘滲�風�) + (螞紫離擬溘滲�風�) 曖 褻м 醞縑憮");
		Arr.Add(cs);  
		cs.Format("           陛濰 碳葬и ж醞 褻м縑 渠ж罹 匐饜");
		Arr.Add(cs);  
		cs.Format("    * �勒狫� 營ж衛");
		Arr.Add(cs);  
		cs.Format("     甲f = 1.3 ▼ (м撩瞪 堅薑ж醞 擬溘 + м撩�� 堅薑ж醞) + 2.15 ▼ �勒狫萷� 曖и 擬溘");
		Arr.Add(cs);  
		cs.Format("           + 1.3 ▼1/2 �勒狫� 綠營ж衛曖 Ёж醞 擬溘 + 觼葬Щ 擬溘 + 勒褻熱蹴 擬溘 + 螞紫離 擬溘");
		Arr.Add(cs);  
		cs.Format("           曖 褻м 醞縑憮 陛濰 碳葬и ж醞 褻м縑 渠ж罹 匐饜");
		Arr.Add(cs);  
	}
	else
	{
		cs.Format("     嬪縑 翮剪и ж醞褻м醞縑憮 陛濰 碳葬и 褻м縑 渠п 匐餌");
		Arr.Add(cs);
		cs.Format("     甲f = 1.3 ▼(м撩瞪 堅薑ж醞 擬溘 + м撩�� 堅薑ж醞) + 2.15 ▼(�勒狫萷� 曖и 擬溘(醱問んл))");
		Arr.Add(cs);
		cs.Format("           + (觼葬Щ 擬溘 滲�風�) + (勒褻熱蹴 擬溘 滲�風�) + (螞紫離 擬溘 滲�風�)曖 褻м 醞縑憮");
		Arr.Add(cs);
		cs.Format("           陛濰 碳葬и ж醞 褻м縑 渠ж罹 匐饜");
		Arr.Add(cs);
	}
	cs.Format(""); Arr.Add(cs);

	//////////////////////////////////////////////////////////////////////

	if(bPLUS_M)	cs.Format("   函 夥款っ 夔觼葬お");		// 薑賅詮お
	else		cs.Format("   函 鼻睡夥款っ REBAR");	// 睡賅詮お
	Arr.Add(cs);

	double Sigma_ckA;
	if(bPLUS_M)	Sigma_ckA = pData->DESIGN_MATERIAL.m_dSigmaCK*3/5;	// 薑賅詮お
	else		Sigma_ckA = 400;									// 睡賅詮お
		
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
		cs.Format("   - Ёж醞 綠營ж衛");
		Arr.Add(cs);
	}
	else
	{
		cs.Format("     м撩�� 堅薑ж醞 擬溘 = %.3f MPa", Sigma_Dead);
		Arr.Add(cs);

		cs.Format("     м撩�� �勒狫� 擬溘 = %.3f MPa", Sigma_Live);
		Arr.Add(cs);
	}

	if(bPLUS_M)    // 薑賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ %.3f + 2.15 ▼ %.3f + (%.3f) + (%.3f) + (%.3f)"
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
	else           // 睡賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ %.3f + 2.15 ▼ %.3f + (%.3f) + (%.3f) - (%.3f)"
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
	if(bPLUS_M)    // 薑賅詮お
	{
		if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  (3/5)");
		else								szText.Format("  ￣ (3/5)");
		if(bHybrid)							szText += "R ▼ ";
		szText += "f ck";
	}
	else           // 睡賅詮お
	{
		if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  f y");
		else								szText.Format("  ￣ f y");
		if(bHybrid)							szText += " ▼ R";
	}
	if(Sigma_ckA > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (觼葬Щ,勒褻熱蹴 んл衛)",szText,Sigma_ckA);
	else								str.Format("%s = %.2f MPa  -------- N.G (觼葬Щ,勒褻熱蹴 んл衛)",szText,Sigma_ckA);
	Arr.Add(cs+str);

	if(bPLUS_M)    // 薑賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ %.3f + 2.15 ▼ %.3f + (%.3f)"
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
		Arr.Add(cs);
		Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
							+fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
	}
	else           // 睡賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ %.3f + 2.15 ▼ %.3f - (%.3f)"
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_cut")) );
		Arr.Add(cs);
		Sigma_Sigma2=Sigma_Dead*1.3+Sigma_Live*2.15
							-fabs(StressCalc.GetMapValueSecStress("Sigma_cut") );
	}

	cs.Format("         = %.3f MPa ",Sigma_Sigma2);
	if(bPLUS_M)    // 薑賅詮お
	{
		if(Sigma_ckA > fabs(Sigma_Sigma2))	szText.Format("  <  (3/5)");
		else								szText.Format("  ￣ (3/5)");
		if(bHybrid)							szText += "R ▼ ";
		szText += "f ck";
	}
	else           // 睡賅詮お
	{
		if(Sigma_ckA > fabs(Sigma_Sigma2))	szText.Format("  <  f y");
		else								szText.Format("  ￣ f y");
		if(bHybrid)							szText += " ▼ R";	
	}
	if(Sigma_ckA > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (觼葬Щ,勒褻熱蹴 薯諼衛)",szText,Sigma_ckA);
	else								str.Format("%s = %.2f MPa  -------- N.G (觼葬Щ,勒褻熱蹴 薯諼衛)",szText,Sigma_ckA);
	Arr.Add(cs+str);
	cs.Format(""); Arr.Add(cs);

	double Sigma_Wind = 0;
	if(bWind)
	{
		cs.Format("   - Ёж醞 營ж衛");
		Arr.Add(cs);  

		Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vcu_w");

		/////////////////////////////////////////////////////////
		// �勒狫� 綠營ж衛

		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ %.3f + 1.3 ▼ %.3f + (%.3f) + (%.3f) + (%.3f)"
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
		else           // 睡賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ %.3f + 1.3 ▼ %.3f + (%.3f) + (%.3f) - (%.3f)"
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
		if(bPLUS_M)    // 薑賅詮お
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  (3/5)");
			else								szText.Format("  ￣ (3/5)");
			if(bHybrid)							szText += "R ▼ ";
			szText += "f ck";
		}
		else           // 睡賅詮お
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  f y");
			else								szText.Format("  ￣ f y");
			if(bHybrid)							szText += " ▼ R";
		}
		if(Sigma_ckA > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (�勒狫� 綠營ж衛)",szText,Sigma_ckA);
		else								str.Format("%s = %.2f MPa  -------- N.G (�勒狫� 綠營ж衛)",szText,Sigma_ckA);
		Arr.Add(cs+str);

		/////////////////////////////////////////////////////////
		// �勒狫� 營ж衛

		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ %.3f + 2.15 ▼ %.3f + 1.3 ▼ %.3f/2 + (%.3f) + (%.3f) + (%.3f)"
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
		else           // 睡賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ %.3f + 2.15 ▼ %.3f + 1.3 ▼ %.3f/2 + (%.3f) + (%.3f) - (%.3f)"
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
		if(bPLUS_M)    // 薑賅詮お
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  (3/5)");
			else								szText.Format("  ￣ (3/5)");
			if(bHybrid)							szText += "R ▼ ";
			szText += "f ck";
		}
		else           // 睡賅詮お
		{
			if(Sigma_ckA > fabs(Sigma_Sigma))	szText.Format("  <  f y");
			else								szText.Format("  ￣ f y");
			if(bHybrid)							szText += " ▼ R";
		}
		if(Sigma_ckA > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (�勒狫� 營ж衛)",szText,Sigma_ckA);
		else								str.Format("%s = %.2f MPa  -------- N.G (�勒狫� 營ж衛)",szText,Sigma_ckA);
		Arr.Add(cs+str);
		cs.Format(""); Arr.Add(cs);
	}

	//////////////////////////////////////////////////////////////////////

	cs.Format("   刻 鬼營 輿⑽ 鼻っ");
	Arr.Add(cs);

	if(bPLUS_M)    // 薑賅詮お
	{
		Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead")/
			ModelCalc.GetMapValueSecCo("Iv33")*ModelCalc.GetMapValueSecCo("Yvsu");
	}
	else           // 睡賅詮お
	{
		Sigma_Dead = (StressCalc.GetMapValueSecStress("MomentPostDead") / Irv
								*StressCalc.GetMapValueSecStress("Yvsu"));
	}
	Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsu_l"); 

	if(bWind)
	{
		cs.Format("   - Ёж醞 綠營ж衛");
		Arr.Add(cs);  
	}
	else
	{
		cs.Format("     м撩瞪 堅薑ж醞 擬溘 = %.3f MPa"
					,StressCalc.GetMapValueSecStress("Sigma_su") );
		Arr.Add(cs);
		cs.Format("     м撩�� 堅薑ж醞 擬溘 = %.3f MPa",Sigma_Dead);
		Arr.Add(cs);
		cs.Format("     м撩�� �勒狫� 擬溘 = %.3f MPa", Sigma_Live);
		Arr.Add(cs);
	}

	if(bPLUS_M)    // 薑賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + (%.3f) + (%.3f) + (%.3f)"
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
	else           // 睡賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + (%.3f) + (%.3f) - (%.3f)"
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
	else								szText.Format("  ￣ f y");
	if(bHybrid)							szText += " ▼ R";
	if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (觼葬Щ,勒褻熱蹴 んл衛)",szText,Sigma_YieldUp);
	else								str.Format("%s = %.2f MPa  -------- N.G (觼葬Щ,勒褻熱蹴 んл衛)",szText,Sigma_YieldUp);
	Arr.Add(cs+str);

	if(bPLUS_M)    // 薑賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_su")
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_sut")) );
		Arr.Add(cs);
		Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_su")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							+fabs(StressCalc.GetMapValueSecStress("Sigma_sut"));
	}
	else           // 睡賅詮お
	{
		cs.Format("     甲f = 1.3 ▼(%.3f + %.3f) + 2.15 ▼%.3f - (%.3f)"
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
	else									szText.Format("  ￣ f y");
	if(bHybrid)								szText += " ▼ R";
	if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (觼葬Щ,勒褻熱蹴 薯諼衛)",szText,Sigma_YieldLo);
	else									str.Format("%s = %.2f MPa  -------- N.G (觼葬Щ,勒褻熱蹴 薯諼衛)",szText,Sigma_YieldLo);
	Arr.Add(cs+str);
	cs.Format(""); Arr.Add(cs);

	if(bWind)
	{
		cs.Format("   - Ёж醞 營ж衛");
		Arr.Add(cs);  

		Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsu_w");

		/////////////////////////////////////////////////////////
		// �勒狫� 綠營ж衛
		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 1.3 ▼ %.3f + (%.3f) + (%.3f) + (%.3f)"
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
		else           // 睡賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 1.3 ▼ %.3f + (%.3f) + (%.3f) - (%.3f)"
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
		else								szText.Format("  ￣ f y");
		if(bHybrid)							szText += " ▼ R";
		if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (�勒狫� 綠營ж衛)",szText,Sigma_YieldUp);
		else								str.Format("%s = %.2f MPa  -------- N.G (�勒狫� 綠營ж衛)",szText,Sigma_YieldUp);
		Arr.Add(cs+str);

		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + 1.3 ▼ %.3f/2 + (%.3f) + (%.3f) + (%.3f)"
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
		else           // 睡賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + 1.3 ▼ %.3f/2 + (%.3f) + (%.3f) - (%.3f)"
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
		else									szText.Format("  ￣ f y");
		if(bHybrid)								szText += " ▼ R";
		if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (�勒狫� 營ж衛)",szText,Sigma_YieldLo);
		else									str.Format("%s = %.2f MPa  -------- N.G (�勒狫� 營ж衛)",szText,Sigma_YieldLo);
		Arr.Add(cs+str);
	}
	cs.Format(""); Arr.Add(cs);

	///////////////////////////////////////////////////////////////////////

	cs.Format("   券 鬼營 輿⑽ жっ");
	Arr.Add(cs);

	if(bPLUS_M)    // 薑賅詮お
	{
		Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead")
								/ModelCalc.GetMapValueSecCo("Iv33")
								*ModelCalc.GetMapValueSecCo("Yvsl");
	}
	else           // 睡賅詮お
	{
		Sigma_Dead = StressCalc.GetMapValueSecStress("MomentPostDead") / Irv
								*StressCalc.GetMapValueSecStress("Yvsl");
	}

	Sigma_Live = StressCalc.GetMapValueSecStress("Sigma_vsl_l"); 

	if(bWind)
	{
		cs.Format("   - Ёж醞 綠營ж衛");
		Arr.Add(cs);  
	}
	else
	{
		cs.Format("     м撩瞪 堅薑ж醞 擬溘 = %.3f MPa"
					,StressCalc.GetMapValueSecStress("Sigma_sl") );
		Arr.Add(cs);
		cs.Format("     м撩�� 堅薑ж醞 擬溘 = %.3f MPa",Sigma_Dead);
		Arr.Add(cs);
		cs.Format("     м撩�� �勒狫� 擬溘 = %.3f MPa", Sigma_Live);
		Arr.Add(cs);
	}

	if(bPLUS_M)    // 薑賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + (%.3f) + (%.3f) - (%.3f)"
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
	else           // 睡賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + (%.3f) + (%.3f) + (%.3f)"
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
	else								szText.Format("  ￣ f y");
	if(bHybrid)							szText += " ▼ R";
	if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (觼葬Щ,勒褻熱蹴 んл衛)",szText,Sigma_YieldUp);
	else								str.Format("%s = %.2f MPa  -------- N.G (觼葬Щ,勒褻熱蹴 んл衛)",szText,Sigma_YieldUp);
	Arr.Add(cs+str);

	if(bPLUS_M)    // 薑賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f - (%.3f)"
				   ,StressCalc.GetMapValueSecStress("Sigma_sl")
				   ,Sigma_Dead,Sigma_Live
				   ,fabs(StressCalc.GetMapValueSecStress("Sigma_slt")) );
		Arr.Add(cs);
		Sigma_Sigma2=(StressCalc.GetMapValueSecStress("Sigma_sl")+Sigma_Dead)*1.3
							+Sigma_Live*2.15
							-fabs(StressCalc.GetMapValueSecStress("Sigma_slt") );
	}
	else           // 睡賅詮お
	{
		cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + (%.3f)"
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
	else									szText.Format("  ￣ f y");
	if(bHybrid)								szText += " ▼ R";
	if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (觼葬Щ,勒褻熱蹴 薯諼衛)",szText,Sigma_YieldLo);
	else									str.Format("%s = %.2f MPa  -------- N.G (觼葬Щ,勒褻熱蹴 薯諼衛)",szText,Sigma_YieldLo);
	Arr.Add(cs+str);
	cs.Format("");Arr.Add(cs);

	if(bWind)
	{
		cs.Format("   - Ёж醞 營ж衛");
		Arr.Add(cs);  

		Sigma_Wind = StressCalc.GetMapValueSecStress("Sigma_vsl_w");

		/////////////////////////////////////////////////////////
		// �勒狫� 綠營ж衛
		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 1.3 ▼ %.3f + (%.3f) + (%.3f) - (%.3f)"
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
		else           // 睡賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 1.3 ▼ %.3f + (%.3f) + (%.3f) + (%.3f)"
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
		else								szText.Format("  ￣ f y");
		if(bHybrid)							szText += " ▼ R";
		if(Sigma_YieldUp > fabs(Sigma_Sigma))	str.Format("%s = %.2f MPa  -------- O.K (�勒狫� 綠營ж衛)",szText,Sigma_YieldUp);
		else								str.Format("%s = %.2f MPa  -------- N.G (�勒狫� 綠營ж衛)",szText,Sigma_YieldUp);
		Arr.Add(cs+str);

		if(bPLUS_M)    // 薑賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + 1.3 ▼ %.3f/2 + (%.3f) + (%.3f) - (%.3f)"
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
		else           // 睡賅詮お
		{
			cs.Format("     甲f = 1.3 ▼ (%.3f + %.3f) + 2.15 ▼ %.3f + 1.3 ▼ %.3f/2 + (%.3f) + (%.3f) + (%.3f)"
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
		else									szText.Format("  ￣ f y");
		if(bHybrid)								szText += " ▼ R";
		if(Sigma_YieldLo > fabs(Sigma_Sigma2))	str.Format("%s = %.2f MPa  -------- O.K (�勒狫� 營ж衛)",szText,Sigma_YieldLo);
		else									str.Format("%s = %.2f MPa  -------- N.G (�勒狫� 營ж衛)",szText,Sigma_YieldLo);
		Arr.Add(cs+str);
	}
	cs.Format("");Arr.Add(cs);

// error check
//	if(pNem->GetSectionStress(nG,nIdx,"K")==-10.0)
//	AfxMessageBox("艙щ啗熱 k高檜 啗骯腎雖 彊懊蝗棲棻!");

}
void CPlateStressOut::StressCheckVBracingJijumOutput(CStringArray &Arr, CPlateBasicIndex *pBx)
{
	CFemModelingCalc ModelCalc (m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	
	CFemStressCalc   StressCalc(m_pStd->GetDataManage(), m_pStd->m_pCalcStd);	

	long nG = pBx->GetNumberGirder();
	CVBracing       *pVr  = pBx->GetSection()->GetVBracing();

	CString cs;
	cs.Format("< 熱霜粽溯檜諒曖 撲啗 - 剪渦:%d - %d>", nG+1, pVr->GetSelfCount()+1); Arr.Add(cs);
	cs.Format(""); Arr.Add(cs);

	cs.Format("⑷營 雖錳腎雖 彊堅 氈蝗棲棻.");	Arr.Add(cs);


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

	/// в蹂и 等檜攪 啗骯

	double	dHeightLeft		= pSepLeft ? pSepLeft->GetHeight() : 0;
	double	dHeightRight	= pSepRight ? pSepRight->GetHeight() : 0;
	double dHBangHo = max(dHeightLeft, dHeightRight);	//寞�ㄩ拿蘉�(謝.辦難醞 贖匙)
	double dSta     = (pGir->GetBxOnJijum(1)->GetStation() - pGir->GetBxOnJijum(0)->GetStation())/2;//難唳除曖 醞懈蝶纔檜暮
	double dH       = pGir->GetHeightGirderByStaAng(dSta) - pGir->GetThickFactByStation(dSta, G_F_U);		//剪渦曖 堪檜(鼻睡Ы楠雖舒眷薯諼)
	double dWFlange = pGir->GetWidthOnStation(dSta, TRUE);											//剪渦 鼻睡 Ы楠雖曖 ァ			
	long   nTotalVBr = pGir->GetQtyVBracing();
	long   nCurVBracing = pGir->GetVBracingPosByBx(pBx);

	CPlateBasicIndex *pBxCompPre  = NULL;
	CPlateBasicIndex *pBxCompPost = NULL;
	if(pBx==pGir->GetBxOnJijum(0))	//籀擠 熱霜粽溯檜諒
	{
		pBxCompPre  = NULL;
		pBxCompPost = pGir->GetBxByVBracingPos(1);
	}
	else if(pBx==pGir->GetBxOnJijum(pDB->m_nQtyJigan))	//葆雖虞 熱霜粽溯檜諒
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

	// 熱霜粽溯檜諒 綺曖 薯錳(橾奩睡)
//	long nSteelTypeUp = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_UVBEAM);
//	long nSteelTypeLo = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_LVBEAM);
//	long nSteelTypeC  = pMat->GetSteelType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_CVBEAM);
//	long nJewonTypeUp = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_UVBEAM);
//	long nJewonTypeLo = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_LVBEAM);
//	long nJewonTypeC  = pMat->GetJewonType(CSteelMaterial::TYPE_GEN,CSteelMaterial::BUJE_CVBEAM);
	//鼻⑷
	CSteelMaterialData	*pMatData	= pVr->GetSteelMaterialData(STEELMAT_UPPER);
	double dHU  = pMatData->m_dH;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dH;
	double dBU  = pMatData->m_dB;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dB;
	double dT1U = pMatData->m_dT1;//pMat->GetMatData(nSteelTypeUp,nJewonTypeUp)->m_dT1;
	double dLengthU = pVr->GetLengthBeam(0);
	//ж⑷
	pMatData	= pVr->GetSteelMaterialData(STEELMAT_LOWER);
	//餌營
	pMatData	= pVr->GetSteelMaterialData(STEELMAT_DIAGONAL);
	double dHC  = pMatData->m_dH;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dH;
	double dBC  = pMatData->m_dB;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dB;
	double dT1C = pMatData->m_dT1;//pMat->GetMatData(nSteelTypeC,nJewonTypeC)->m_dT1;
	double dLengthC = pVr->GetLengthBeam(2);
	//
	double dHeightFrame = toM(pVr->GetLengthFrameLine(4));	// Щ溯歜曖 熱霜堪檜
	double dWidthHalf   = toM(pVr->GetLengthFrameLine(1)/2);// Щ溯歜曖 熱ゎ堪檜/2
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
	cs.Format("< 熱霜粽溯檜諒曖 撲啗 - 剪渦:%d - %d>", nG+1, pVr->GetSelfCount()+1);	Arr.Add(cs);
	Arr.Add("");
	
	Arr.Add  ("(1) ж醞骯薑");
	Arr.Add  ("   函 掖榆 識 堪檜");
	Arr.Add  ("     輿⑽曖 ж睡 Ы楠雖 夥款縑憮 睡攪 寞�ㄩ挫� 部梱雖");
	cs.Format("     D = %.3lf + %.3lf + %.3lf = %.3lf m", toM(dHBangHo), toM(pDB->m_dThickSlabBase), toM(dH), D);	Arr.Add(cs);
	Arr.Add  ("   刻 掖榆 識 ァ");
	cs.Format("     B = %.3lf m", B);	Arr.Add(cs);
	Arr.Add  ("   券 Ёж醞 鬼紫");
	cs.Format("     B/D = %.3lf / %.3lf = %.3lf", B, D, B/D);						Arr.Add(cs);
	cs.Format("       ˍ 1 %s B/D %s 8", 1<(B/D) ? "<":">", (B/D)<8 ? "<":">");		Arr.Add(cs);
	if(B/D<8)
	{
		Arr.Add("     Pw = [400 - 20 (B/D)]D ￣ 600 kgf/m");	
		cs.Format("          (400 - 20 ▼ %.3lf) ▼ %.3lf = %.3lf kgf/m %s 600 kgf/m", (B/D), D, Pw, Pw>600 ? ">":"<");		Arr.Add(cs);
	}
	else
	{
		Arr.Add("     Pw = 240 ▼ D ￣ 600 kgf/m");
		cs.Format("          240 ▼ %.3lf = %.3lf kgf/m %s 600 kgf/m", D, Pw, Pw>600 ? ">":"<");		Arr.Add(cs);
	}
	
	cs.Format("     熱霜粽溯檜諒曖 除問 : %.1f m  (譆模 除問 敘薑 6m)", dGapVBracing);			Arr.Add(cs);
	cs.Format("                                   Ы楠雖 ァ : %.0lf mm", dWFlange);				Arr.Add(cs);
	cs.Format("     1偃曖 熱霜粽溯檜諒縑 濛辨ж朝 ж醞 :");										Arr.Add(cs);
	cs.Format("       He = %.1lf ▼ %.1lf m = %.1lf kgf", Pw, dGapVBracing, He);	Arr.Add(cs);

	Arr.Add  ("(2) 睡營溘 啗骯");
	Arr.Add  ("   函 睡營 薯錳");
	cs.Format("     鼻⑷營(ab), ж⑷營(eg) : L - %.0lf ▼ %.0lf ▼ %.0lf  (%s)", dHU, dBU, dT1U, pDB->GetMaterial(VB_UB));	Arr.Add(cs);
	cs.Format("     餌營(af, bf)           : L - %.0lf ▼ %.0lf ▼ %.0lf  (%s)", dHC, dBC, dT1C, pDB->GetMaterial(VB_CR));	Arr.Add(cs);
	Arr.Add  ("   刻 睡營溘 啗骯(除楞衝戲煎 啗骯)");
	Arr.Add  ("     侍 ж⑷營(ef, fg)");
	cs.Format("        N坐 = -1/2 * He = %.0lf kgf  %s", -He*0.5, He<0 ? "(憶蹴)":"(檣濰)");
	Arr.Add(cs);
	Arr.Add  ("     佳 餌營(af, bf)");
	cs.Format("        Nd(af) = - 1/2 ▼ N坐 ▼ d/坐 = -1/2 ▼ %.3lf ▼ %.3lf / %.3lf = %.3lf kgf %s", -He*0.5, d, dWidthHalf, Nd_af, Nd_af<0 ? "(憶蹴)":"(檣濰)");
	Arr.Add(cs);
	cs.Format("        Nd(bf) = - Nd(af) = %.3lf kgf %s", Nd_bf, Nd_bf<0 ? "(憶蹴)":"(檣濰)");
	Arr.Add(cs);
	Arr.Add  ("     使 鼻⑷營(ab)");
	cs.Format("        Nu = - (He / 2) - Nd(af) ▼坐/ d = - (%.3lf / 2) - %.3lf ▼ %.3lf/%.3lf = %.3lf kgf %s", He, Nd_af, dWidthHalf, d, Nu, Nu<0 ? "(憶蹴)":"(檣濰)");
	Arr.Add(cs);
	Arr.Add  ("(3) 欽賊唸薑 塽 匐饜");
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
		cs.Format("   %s : L - %.0lf ▼ %.0lf ▼ %.0lf ▼ %.0lf (%s)", n==0 ? "函 鼻 ⑷ 營":"刻 餌  營 :",
					n==0 ? dHU:dHC, n==0 ? dBU:dBC, n==0 ? dT1U:dT1C, dLength, pDB->GetMaterial(n==0 ? VB_UB:VB_CR));
		Arr.Add(cs);
		cs.Format("                 欽賊瞳                : A             %.3lf cm昌", dArea);
		Arr.Add(cs);
		cs.Format("                 欽賊2離 奩唳曖 譆模高 : 乓min         %.3lf cm", dGammaMin);
		Arr.Add(cs);
		cs.Format("                 欽賊2離 奩唳          : 乓x(= 乓y)    %.3lf cm", dGammaX);
		Arr.Add(cs);
		Arr.Add  ("     侍 撮濰綠 匐饜");
		cs.Format("        坐/乓min = %.1lf / %.1lf = %.3lf  %s  120   %s", dLength, dGammaMin, dSejang, dSejang<120 ? "<":">", dSejang<120 ? "O.K":"N.G");
		Arr.Add(cs);
		Arr.Add  ("     佳 ъ辨憶蹴擬溘");
		cs.Format("        坐/乓y   = %.1lf / %.1lf = %.3lf  ( %s  坐/乓y  %s )", dLength, dGammaX, dL_Div_Y, dL_Div_Y>20 ? "20 <":"", dL_Div_Y<93 ? "< 93":"");
		Arr.Add(cs);
		cs.Format("          仲ca   = 1400 - 8.4﹞(坐/乓 - 20) = 1400 - 8.4﹞(%.1lf/%.1lf - 20) = %.3lf kgf/cm昌", dLength, dGammaX, dSigmaCa);
		Arr.Add(cs);
		Arr.Add  ("          ˍ GUSSET PLATE縑 翱唸腎橫 氈朝 唳辦縑朝 ъ辨憶蹴擬溘縑 盪馬睦擊 培ж罹撿 и棻.");
		Arr.Add  ("");
		Arr.Add  ("           盪馬睦擎 0.5 + (坐/乓x) / 1000");
		cs.Format("        ˍ 仲ca' = 仲ca ▼ ( 0.5 + ( 坐/乓X ) / 1000 ) = %.1lf ▼ ( 0.5 + ( %.1lf/%.1lf) / 1000 ) = %.3lf kgf/cm昌",
							dSigmaCa, dLength, dGammaX, dSigmaCaF);
		Arr.Add(cs);
		Arr.Add  ("     使 擬溘匐饜");
		double dAbsNu = abs(Nu);
		double dA_Area = 0;
		if(n==1)
		{
			dA_Area = 3.0/4.0*dArea;
			Arr.Add  ("           檣濰 L⑽鬼曖 嶸�艘僄橉�     (紫煎掖 撲啗晦遽 p89)");
			cs.Format("           As = A ▼ 3 / 4 = %.2lf ▼ 3 / 4 = %.2lf cm昌", dArea, dA_Area);
			Arr.Add(cs);
		}
		else	dA_Area = dArea;
		cs.Format("             仲  = %.1f / %.1f = %.3lf kgf/cm昌 %s 仲ca'    %s", dAbsNu, dA_Area, dSigma, dSigma<dSigmaCaF ? "<":">", dSigma<dSigmaCaF ? "O.K":"N.G");
		Arr.Add(cs);
		Arr.Add("");
	}

}

// 欽睡 陛煎爾(H⑽鬼顫殮, 薯濛顫殮)
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
	strText.Format("    < 欽睡 陛煎爾 - 剪渦 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("      餌辨鬼營 : %s", pEndCr->m_pJewonFlangeUp->m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("      Ы楠雖ァ : bfu = %.0f mm(鼻睡)    bfl = %.0f mm(ж睡)", pEndCr->m_pJewonFlangeUp->m_dWidth, pEndCr->m_pJewonFlangeLo->m_dWidth);
	Arr.Add(strText+"");
	strText.Format("      犒睡曖ァ : bw = %.0f mm", pEndCr->m_pJewonWeb->m_dHeight);
	Arr.Add(strText+"");
	strText.Format("      熱ゎ爾鬼營 餌辨欽熱 %d 欽", pEndCr->m_nQtyHorStiff);
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
		strText.Format("      鼻っ舒眷  tfu = %.0f mm > tfu(min)    .... O.K", pEndCr->m_pJewonFlangeUp->m_dHeight);
	else
		strText.Format("      鼻っ舒眷  tfu = %.0f mm < tfu(min)    .... N.G", pEndCr->m_pJewonFlangeUp->m_dHeight);
	Arr.Add(strText+""+"");
	if(pEndCr->m_pJewonFlangeLo->m_dHeight>pEndCr->m_strtMinThick.m_dMinThickFlangeLo)
		strText.Format("      жっ舒眷  tfl = %.0f mm > tfl(min)    .... O.K", pEndCr->m_pJewonFlangeLo->m_dHeight);
	else
		strText.Format("      жっ舒眷  tfl = %.0f mm < tfl(min)    .... N.G", pEndCr->m_pJewonFlangeLo->m_dHeight);
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    陛) ж 醞 骯 薑");
	Arr.Add(strText+"");
	strText.Format("      (1) 翱霜ж醞");
	Arr.Add(strText+"");
	strText.Format("        1) 堅薑ж醞");
	Arr.Add(strText+"");
	strText.Format("          ん濰舒眷 Tp = %.3f m    鼻っ舒眷 Tc = %.3f m", toM(dThickPave), toM(dThickSlab));
	Arr.Add(strText+"");
	strText.Format("          輿⑽除問 a = %.3f m      S = %.3f m (熱霜粽溯檜諒縑憮 鼻っ欽睡梱雖曖 剪葬)",
		toM(pEndCr->m_strtVerLoad.m_dGirderSpace), toM(pEndCr->m_strtVerLoad.m_dDistVerBracingToEndSlab));
	Arr.Add(strText+"");
	strText.Format("          ん濰 : %.3f ▼ %.3f = %.3f kN/m昌", toM(dThickPave), tokNPM3(dUnitPave), tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave));
	Arr.Add(strText+"");
	strText.Format("          鼻っ : %.3f ▼ %.3f = %.3f kN/m昌", toM(dThickSlab), tokNPM3(dUnitConc), tokNPM2(pEndCr->m_strtVerLoad.m_dLoadSlab));
	Arr.Add(strText+"");
	strText.Format("                           TOTAL = %.3f kN/m昌", tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave+pEndCr->m_strtVerLoad.m_dLoadSlab));
	Arr.Add(strText+"");
	strText.Format("          q1 = %.3f ▼ %.3f = %.3f kN/m昌", toM(pEndCr->m_strtVerLoad.m_dDistVerBracingToEndSlab),
		tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave+pEndCr->m_strtVerLoad.m_dLoadSlab), tokNPM(pEndCr->m_strtVerLoad.m_dQ1));
	Arr.Add(strText+"");
	strText.Format("          q2 = %.3f ▼ %.3f = %.3f kN/m昌", toM(pEndCr->m_strtVerLoad.m_dGirderSpace/2),
		tokNPM2(pEndCr->m_strtVerLoad.m_dLoadPave+pEndCr->m_strtVerLoad.m_dLoadSlab), tokNPM(pEndCr->m_strtVerLoad.m_dQ2));
	Arr.Add(strText+""+"");
	strText.Format("        2) �勒狫�");
	Arr.Add(strText+"");
	strText.Format("           彌ж醞 : %.3f kN", tokN(pCalcData->DESIGN_CONDITION.m_dPr));
	Arr.Add(strText+"");
	strText.Format("           醱問啗熱 : i = 15 / (40 + L) = 15 / (40 + %.3f)", toM(pEndCr->m_strtVerLoad.m_dGirderSpace));
	Arr.Add(strText+"");
	if(pEndCr->m_strtVerLoad.m_dCollisionCoeff>0.3)
		strText.Format("                        = %.3f > 0.3   ╮  i = %.3f", pEndCr->m_strtVerLoad.m_dCollisionCoeff, pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
	else
		strText.Format("                        = %.3f < 0.3   ╮  i = %.3f", pEndCr->m_strtVerLoad.m_dCollisionCoeff, pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
	Arr.Add(strText+"");
	strText.Format("      (2) 熱ゎж醞");
	Arr.Add(strText+"");
	strText.Format("          Ёж醞縑 渠ж罹朝 2褻曖 欽睡 陛煎爾陛 嫡朝 匙戲煎 ж貊,");
	Arr.Add(strText+"");
	strText.Format("          雖霞ж醞縑 渠ж罹朝 瞪羹曖 欽睡 陛煎爾陛 敕蛔ж啪 睡氬ж朝 匙戲煎 и棻.");
	Arr.Add(strText+"");
	strText.Format("        1) Ёж醞");
	Arr.Add(strText+"");
	strText.Format("           - Ёж醞 鬼紫");
	Arr.Add(strText+"");
	strText.Format("              忙式式式式式式式式式式成式式式式式式式式式式式式式式忖");		Arr.Add(strText+"");
	strText.Format("              弛     欽 賊 ⑽ 鼻    弛        Ё ж 醞 (kN/m)     弛");		Arr.Add(strText+"");
	strText.Format("              戍式式式式式式式式式式托式式式式式式式式式式式式式式扣");		Arr.Add(strText+"");
	strText.Format("              弛   1 ‾ B/D < 8     弛 [ 4.0 - 0.2(B/D) ] D ￣ 6  弛");		Arr.Add(strText+"");
	strText.Format("              戍式式式式式式式式式式托式式式式式式式式式式式式式式扣");		Arr.Add(strText+"");
	strText.Format("              弛      8 ‾ B/D      弛         2.4 D ￣ 6         弛");		Arr.Add(strText+"");
	strText.Format("              戌式式式式式式式式式式扛式式式式式式式式式式式式式式戎");		Arr.Add(strText+"");
	strText.Format("           - Ёж醞 鬼紫 啗骯");
	Arr.Add(strText+"");
	strText.Format("           掖榆 ァ錳 (B) = %.3f m", toM(pEndCr->m_strtHorLoad.m_dWidthBridge));
	Arr.Add(strText+"");
	strText.Format("           掖榆 堪檜 (D) = %.3f m", toM(pEndCr->m_strtHorLoad.m_dHeightBridge));
	Arr.Add(strText+"");
	strText.Format("           B / D = %.3f / %.3f = %.3f m",
		toM(pEndCr->m_strtHorLoad.m_dWidthBridge), toM(pEndCr->m_strtHorLoad.m_dHeightBridge), pEndCr->m_strtHorLoad.m_dRateWidthHeight);
	Arr.Add(strText+"");
	if(pEndCr->m_strtHorLoad.m_dRateWidthHeight>=1 && pEndCr->m_strtHorLoad.m_dRateWidthHeight<8)
		strText.Format("            1 ‾ B / D < 8  檜嘎煎  Ww = [ 4.0 - 0.2 ( B / D ) ] D = %.3f kN/m", tokN(pEndCr->m_strtHorLoad.m_dWw));
	else if(pEndCr->m_strtHorLoad.m_dRateWidthHeight>=8)
		strText.Format("              8 ‾ B / D   檜嘎煎  Ww = 2.4 ﹞D = %.3f kN/m", tokN(pEndCr->m_strtHorLoad.m_dWw));
	else
		strText.Format("              營匐饜 蹂蜂");
	Arr.Add(strText+"");
	if(tokN(pEndCr->m_strtHorLoad.m_dWw)>=6.0)
		strText.Format("              Ww ￣ 6.0 kN/m    檜嘎煎 ˍ Ww = %.3f", tokN(pEndCr->m_strtHorLoad.m_dWwUse));
	else
		strText.Format("              Ww < 6.0 kN/m    檜嘎煎 ˍ Ww = %.3f", tokN(pEndCr->m_strtHorLoad.m_dWwUse));
	Arr.Add(strText+"");
	strText.Format("           - Ёж醞縑 曖и 粽溯檜諒 1褻曖 濛辨溘");
	Arr.Add(strText+"");
	strText.Format("             п渡 雖除濰 L = %.3f m", toM(pEndCr->m_strtHorLoad.m_dLengthSpan));
	Arr.Add(strText+"");
	strText.Format("             剪渦熱      n = %d", pDB->GetGirdersu());
	Arr.Add(strText+"");
	if(pDB->GetGirdersu()>=3)
		strText.Format("             Hw = 1/2 ﹞ Ww ﹞ L = %.3f kN", tokNM(pEndCr->m_strtHorLoad.m_dHw));
	else
		strText.Format("             Hw = Ww ﹞ L = %.3f kN", tokNM(pEndCr->m_strtHorLoad.m_dHw));
	Arr.Add(strText+"");
	strText.Format("        2) 雖霞ж醞");
	Arr.Add(strText+"");
	strText.Format("           - п渡 雖薄縑 濛辨ж朝 堅薑ж醞");
	Arr.Add(strText+"");
	strText.Format("             甲Rd = п渡 雖薄奩溘曖 м啗 = %.3f kN", tokN(pEndCr->m_strtEarthQuakeLoad.m_dReaction));
	Arr.Add(strText+"");
	strText.Format("           - 雖霞 陛樓紫 啗熱(A)");
	Arr.Add(strText+"");
	if(nGradeEarth==0)	strText.Format("              頂霞蛔晝 : 弘蛔晝  ╮  嬪я紫 啗熱 : %.3f", dRiskCoefficient);
	else				strText.Format("              頂霞蛔晝 : 弗蛔晝  ╮  嬪я紫 啗熱 : %.3f", dRiskCoefficient);
	Arr.Add(strText+"");
	if(nRegionEarth==0)	strText.Format("              頂霞掘羲 : 弘掘羲  ╮  雖霞掘羲啗熱 : %.3f", dZoneCoefficient);
	else				strText.Format("              頂霞掘羲 : 弗掘羲  ╮  雖霞掘羲啗熱 : %.3f", dZoneCoefficient);
	Arr.Add(strText+"");
	strText.Format("              A = %.3f ▼ %.3f = %.3f", dRiskCoefficient, dZoneCoefficient, pEndCr->m_strtEarthQuakeLoad.m_dAcceleration);
	Arr.Add(strText+"");
	strText.Format("           - 欽睡 粽溯檜諒縑 濛辨ж朝 識雖霞ж醞");
	Arr.Add(strText+"");
	strText.Format("             甲He = A 甲Rd = %.3f ▼ %.3f = %.3f kN", pEndCr->m_strtEarthQuakeLoad.m_dAcceleration,
		tokN(pEndCr->m_strtEarthQuakeLoad.m_dReaction), tokN(pEndCr->m_strtEarthQuakeLoad.m_dTotEarthQuakeLoad));
	Arr.Add(strText+"");
	strText.Format("           - 雖霞ж醞縑 曖и 粽溯檜諒 1褻曖 濛辨溘");
	Arr.Add(strText+"");
	strText.Format("             He = 1/ (n-1)﹞甲He = 1/(%d-1) ▼ %.3f = %.3f kN", pDB->GetGirdersu(),
		tokN(pEndCr->m_strtEarthQuakeLoad.m_dTotEarthQuakeLoad), tokN(pEndCr->m_strtEarthQuakeLoad.m_dEarthQuakeLoad));
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    釭) 睡營溘 啗骯");								Arr.Add(strText+"");
	strText.Format("        1) �皒薷鉐�");								Arr.Add(strText+"");
	strText.Format("           堅薑ж醞縑 曖и �皒薷鉐�");				Arr.Add(strText+"");
	strText.Format("           Md = 1/8﹞q1﹞L昌 + 1/12﹞q2﹞L昌 ");	Arr.Add(strText+"");
	strText.Format("              = 1/8 ▼ %.3f ▼ %.3f昌 + 1/12 ▼ %.3f ▼ %.3f昌", tokNPM(pEndCr->m_strtVerLoad.m_dQ1), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
		tokNPM(pEndCr->m_strtVerLoad.m_dQ1), toM(pEndCr->m_strtVerLoad.m_dGirderSpace), tokNM(pEndCr->m_strtElementLoad.m_dMd));
	Arr.Add(strText+"");
	strText.Format("              = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMd));
	Arr.Add(strText+"");
	strText.Format("           �勒狫萷� 曖и �皒薷鉐�");
	Arr.Add(strText+"");
	if(pEndCr->m_strtVerLoad.m_dGirderSpace>2400.0)
	{
		strText.Format("           Ml = (1/4﹞Pr﹞L + 1/2﹞Pr﹞b )﹞( 1 + i )");
		Arr.Add(strText+"");
		strText.Format("              = (1/4 ▼ %.1f ▼ %.3f + 1/2 ▼ %.1f ▼ %.3f ▼ (1 + %.3f)", tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
			tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace/2-1200), pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
		Arr.Add(strText+"");
		strText.Format("              = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMl));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("           Ml = 1/4﹞Pr﹞L﹞( 1 + i )");
		Arr.Add(strText+"");
		strText.Format("              = 1/4 ▼ %.1f ▼ %.3f ▼ (1 + %.3f)", tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace), pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
		Arr.Add(strText+"");
		strText.Format("              = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMl));
		Arr.Add(strText+"");
	}
	strText.Format("          甲M = Md + Ml = %.3f + %.3f = %.3f kN.m", tokNM(pEndCr->m_strtElementLoad.m_dMd), tokNM(pEndCr->m_strtElementLoad.m_dMl),
		tokNM(pEndCr->m_strtElementLoad.m_dMd+pEndCr->m_strtElementLoad.m_dMl));
	Arr.Add(strText+"");
	strText.Format("        2) 瞪欽溘");
	Arr.Add(strText+"");
	strText.Format("           Sd = 1/2﹞q1﹞L + 1/4﹞q2﹞L ");
	Arr.Add(strText+"");
	strText.Format("              = 1/2 ▼ %.3f ▼ %.3f + 1/4 ▼ %.3f ▼ %.3f", tokNPM(pEndCr->m_strtVerLoad.m_dQ1), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
		tokNPM(pEndCr->m_strtVerLoad.m_dQ2), toM(pEndCr->m_strtVerLoad.m_dGirderSpace));
	Arr.Add(strText+"");
	strText.Format("              = %.3f kN", tokN(pEndCr->m_strtElementLoad.m_dSd));
	Arr.Add(strText+"");
	strText.Format("           Sl = Pr﹞{ 1 + ( L-1.20 ) / L }﹞( 1 + i )");
	Arr.Add(strText+"");
	strText.Format("              = %.1f ▼ { 1 + (%.3f - 1.2) / ▼ %.3f } ▼ (1 + i)", tokN(dPr), toM(pEndCr->m_strtVerLoad.m_dGirderSpace),
		toM(pEndCr->m_strtVerLoad.m_dGirderSpace),	pEndCr->m_strtVerLoad.m_dCollisionCoeffUse);
	Arr.Add(strText+"");
	strText.Format("              = %.3f kN", tokN(pEndCr->m_strtElementLoad.m_dSl));
	Arr.Add(strText+"");
	strText.Format("          甲S = Sd + Sl = %.3f + %.3f = %.3f kN.m", tokN(pEndCr->m_strtElementLoad.m_dSd), tokN(pEndCr->m_strtElementLoad.m_dSl),
		tokN(pEndCr->m_strtElementLoad.m_dSd+pEndCr->m_strtElementLoad.m_dSl));
	Arr.Add(strText+"");
	strText.Format("        3) 熱ゎ溘");
	Arr.Add(strText+"");
	strText.Format("           Ёж醞    Hw = %.3f kN", tokNM(pEndCr->m_strtElementLoad.m_dHw));
	Arr.Add(strText+"");
	strText.Format("           雖霞ж醞  He = %.3f kN", tokN(pEndCr->m_strtElementLoad.m_dHe));
	Arr.Add(strText+"");
	strText.Format("        4) FRAME п戮唸婁餵啗");
	Arr.Add(strText+"");
	strText.Format("           ж醞碟寡濛辨縑 渠и 艙щ擊 奩艙ж晦 嬪ж罹 FRAME п戮唸婁蒂 堅溥и棻.");	
	Arr.Add(strText+"");
	strText.Format("       忙式式式式式式式式成式式式式式式式式式式式成式式式式式式式式式式式成式式式式式式式式式式式忖");	Arr.Add(strText+"");
	strText.Format("       弛                弛        M (kN.m)      弛       S (kN)         弛        N (kN)        弛");	Arr.Add(strText+"");
	strText.Format("       弛    掘    碟    戍式式式式式成式式式式式托式式式式式成式式式式式托式式式式式成式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛                弛    Max   弛    Min   弛    Max   弛    Min   弛    Max   弛    Min   弛");	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式扛式式式式式托式式式式式扛式式式式式托式式式式式扛式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛 м撩瞪 堅薑ж醞弛  %12.3f        弛  %12.3f        弛  %12.3f        弛",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dPreDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dPreDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式式式式式式式托式式式式式式式式式式式托式式式式式式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛 м撩�� 堅薑ж醞弛  %12.3f        弛  %12.3f        弛  %12.3f        弛",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dPostDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dPostDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式成式式式式式托式式式式式成式式式式式托式式式式式成式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛  ��   ж   醞  弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMaxLiveLoad), tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMinLiveLoad),
		tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad),
		tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMaxLiveLoad), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMinLiveLoad));	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛  雖 薄 藹 ж   弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMaxSettlement), tokNM(pEndCr->m_pFrameElementLoad->m_strtMoment.m_dMinSettlement),
		tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement), tokN(pEndCr->m_pFrameElementLoad->m_strtShear.m_dMinSettlement),
		tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMaxSettlement), tokN(pEndCr->m_pFrameElementLoad->m_strtAxial.m_dMinSettlement));	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");					Arr.Add(strText+"");
	strText.Format("       弛     м    啗   弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛",
		tokNM(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), tokNM(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment),
		tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinShear),
		tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxAxial), tokN(pEndCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinAxial));	Arr.Add(strText+"");
	strText.Format("       戌式式式式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式戎");				Arr.Add(strText+"");


	strText.Format("    棻) 欽賊 撲啗");								Arr.Add(strText+"");
	strText.Format("        忙式式式式式式式式式式成式式式式式成式式式式式成式式式式式成式式式式式式成式式式式式式忖");	Arr.Add(strText+"");
	strText.Format("        弛     掘      碟     弛  A(mm昌) 弛  Y (mm)  弛 AY (mm昆)弛 AY昌(mm昂) 弛  Io(mm昂)  弛");	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("        弛1 - UFLG-%4.0f ▼ %2.0f 弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pEndCr->m_pJewonFlangeUp->m_dWidth, pEndCr->m_pJewonFlangeUp->m_dHeight, pEndCr->m_pJewonFlangeUp->m_dA,
		pEndCr->m_pJewonFlangeUp->m_dY,	pEndCr->m_pJewonFlangeUp->m_dAY, pEndCr->m_pJewonFlangeUp->m_dAY2, pEndCr->m_pJewonFlangeUp->m_dIo);Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("        弛1 - WEB -%4.0f ▼ %2.0f 弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pEndCr->m_pJewonWeb->m_dHeight, pEndCr->m_pJewonWeb->m_dWidth, pEndCr->m_pJewonWeb->m_dA,
		pEndCr->m_pJewonWeb->m_dY, pEndCr->m_pJewonWeb->m_dAY, pEndCr->m_pJewonWeb->m_dAY2, pEndCr->m_pJewonWeb->m_dIo);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("        弛1 - LFLG-%4.0f ▼ %2.0f 弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pEndCr->m_pJewonFlangeLo->m_dWidth, pEndCr->m_pJewonFlangeLo->m_dHeight, pEndCr->m_pJewonFlangeLo->m_dA,
		pEndCr->m_pJewonFlangeLo->m_dY,	pEndCr->m_pJewonFlangeLo->m_dAY, pEndCr->m_pJewonFlangeLo->m_dAY2, pEndCr->m_pJewonFlangeLo->m_dIo);Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("        弛      м      啗    弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pEndCr->GetTotSectionArea(), 0.0, pEndCr->GetTotSectionAY(), pEndCr->GetTotSectionAY2(), pEndCr->GetTotSectionIo());Arr.Add(strText+"");
	strText.Format("        戌式式式式式式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式式扛式式式式式式戎");	Arr.Add(strText+"");
	strText.Format("       乩 = 甲A﹞Y / 甲A = %.1f mm", pEndCr->m_strtSection.m_dDelta);					Arr.Add(strText+"");
	strText.Format("        I = 甲Io + 甲A﹞Y昌 - 甲A﹞乩昌 = %.1f mm昂", pEndCr->m_strtSection.m_dI);		Arr.Add(strText+"");
	strText.Format("      Ysu =  bw / 2 + tfu - 乩 = %.1f mm", pEndCr->m_strtSection.m_dYsu);				Arr.Add(strText+"");
	strText.Format("      Ysl =  bw / 2 + tfl + 乩 = %.1f mm", pEndCr->m_strtSection.m_dYsl);				Arr.Add(strText+"");
	strText.Format("      憶蹴睡曖 措睡謝掉縑 渠и ъ辨擬溘擎");											Arr.Add(strText+"");
	strText.Format("       ˍ fca = %.3f MPa,    仲ta = %.3f MPa,    任a = %.3f MPa",
		pEndCr->m_strtSection.m_dFca, pEndCr->m_strtSection.m_dSigmata, pEndCr->m_strtSection.m_dSheara);	Arr.Add(strText+"");Arr.Add("");
	strText.Format("- 睡營溘曖 褻м塽 擬溘匐饜");															Arr.Add(strText+"");
	strText.Format("   棻擠婁 偽擎 寞衝戲煎 陝 ж醞褻м縑 渠и 擬溘紫蒂 褻餌и棻.");						Arr.Add(strText+"");
	strText.Format("   fu = M﹞Ysu / I ▽ N / Ag,   fl = M﹞Ysl / I ▽ N / Ag,   任 = S / Aw");				Arr.Add(strText+"");
	CString	strChkFlange	= _T("");
	CString	strChkShear		= _T("");
	strText.Format("        忙式式式式式式式式成式式式式式成式式式式式成式式式式式成式式式式式成式式式式式成式式成式式式式式成式式忖");	Arr.Add(strText+"");
	strText.Format("        弛    掘    碟    弛M (kN﹞m )弛   S(kN)  弛   N(kN)  弛  fu(MPa) 弛  fl(MPa) 弛っ薑弛任( MPa ) 弛っ薑弛");	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛1. 堅薑ж醞     弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dDeadLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛2. �勒狫�       弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dLiveLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛3. Ёж醞       弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dWindLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛4. 雖霞ж醞     弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dEarthQuakeLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛鼻衛(1+2)       弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompGen[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛Ё衛(1+2+3)/1.25弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompWind[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛雖霞衛(1+4)/1.5 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式托式式式式式托式式扣");	Arr.Add(strText+"");
	if(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGEUP]<=pEndCr->m_strtSection.m_dFca && pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGELO]<=pEndCr->m_strtSection.m_dSigmata)
			strChkFlange.Format("O.K");
	else	strChkFlange.Format("N.G");
	if(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_ALLOWSHEAR]<=pEndCr->m_strtSection.m_dSheara)
			strChkShear.Format("O.K");
	else	strChkShear.Format("N.G");
	strText.Format("        弛FRAMEп戮唸婁   弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛%s 弛 %8.3f 弛%s 弛",
		tokNM(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_MOMENT]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_SHEAR]), tokN(pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_HORIZONTAL]),
		pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGEUP], pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_FLANGELO],
		strChkFlange, pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[COMP_ALLOWSHEAR], strChkShear);	Arr.Add(strText+"");
	strText.Format("        戌式式式式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式扛式式式式式扛式式戎");	Arr.Add(strText+"");Arr.Add("");

	CCrossBeamVerStiff	*pVerStiff	= pEndCr->m_pVerStiff;
	CCrossBeamHorStiff	*pHorStiff	= pEndCr->m_pHorStiff;
	strText.Format("         熱霜爾鬼營蒂 儅楞й 熱 氈朝 Ы楠雖 牖除問曖 譆渠纂 [紫.撲 3.8.5.1]");		Arr.Add(strText+"");
	if(pVerStiff->m_dSpaceCoeff>1.2)
		strText.Format("          ∵(任a/任) = ∵( %.3f / %.3f ) = %.3f > 1.2  ╮ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear,	pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	else
		strText.Format("          ∵(任a/任) = ∵( %.3f / %.3f ) = %.3f < 1.2  ╮ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear, pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	Arr.Add(strText+"");
	if(pVerStiff->m_dMaxSpace<pVerStiff->m_dHeightWeb)
		strText.Format("          %.0f ▼ tw = %.1f mm  <  bw = %.0f mm  熱霜爾鬼營 в蹂", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	else
		strText.Format("          %.0f ▼ tw = %.1f mm  >  bw = %.0f mm  熱霜爾鬼營 в蹂橈擠", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	Arr.Add(strText+"");

	if(pVerStiff->m_bIsNeedVerStiff)	VerStiff(Arr, pVerStiff, pEndCr->m_nQtyHorStiff);
	if(pEndCr->m_nQtyHorStiff>0)		HorStiff(Arr, pHorStiff);
}

void CPlateStressOut::VerStiff(CStringArray &Arr, CCrossBeamVerStiff *pVerStiff, long nQtyHorStiff)
{
	CString	strText;

	strText.Format("     * 熱霜爾鬼營  ( %s 餌辨 )", pVerStiff->m_strMaterial);									Arr.Add(strText+"");
	strText.Format("       熱霜爾鬼營 除問 : a = %.1f mm [紫.撲 3.8.5.1]", pVerStiff->m_dVerStiffSpace);		Arr.Add(strText+"");
	if(pVerStiff->m_dRateSpaceHeight<1.5)
		strText.Format("        a / b = %.1f / %.1f = %.3f  <  1.5    .... O.K", pVerStiff->m_dVerStiffSpace, pVerStiff->m_dHeightWeb, pVerStiff->m_dRateSpaceHeight);
	else
		strText.Format("        a / b = %.1f / %.1f = %.3f  >  1.5    .... N.G", pVerStiff->m_dVerStiffSpace, pVerStiff->m_dHeightWeb, pVerStiff->m_dRateSpaceHeight);
	Arr.Add(strText+"");
	strText.Format("       熱霜爾鬼營 ァ [紫.撲 3.8.5.2]");	Arr.Add(strText+"");
	if(pVerStiff->m_dBvs > pVerStiff->m_dBvsMin)
		strText.Format("        bvs = %.1f mm  >  bvs(min) = b / 30 + 50 = %.1f mm    .... O.K", pVerStiff->m_dBvs, pVerStiff->m_dBvsMin);
	else
		strText.Format("        bvs = %.1f mm  <  bvs(min) = b / 30 + 50 = %.1f mm    .... N.G", pVerStiff->m_dBvs, pVerStiff->m_dBvsMin);
	Arr.Add(strText+"");
	strText.Format("       熱霜爾鬼營 舒眷");	Arr.Add(strText+"");
	if(pVerStiff->m_dTvs > pVerStiff->m_dTvsMin)
		strText.Format("        tvs = %.1f mm  >  tvs(min) = b / 30 + 50 = %.1f mm    .... O.K", pVerStiff->m_dTvs, pVerStiff->m_dTvsMin);
	else
		strText.Format("        tvs = %.1f mm  <  tvs(min) = b / 30 + 50 = %.1f mm    .... N.G", pVerStiff->m_dTvs, pVerStiff->m_dTvsMin);
	Arr.Add(strText+"");
	strText.Format("       熱霜爾鬼營曖 除問匐饜");Arr.Add(strText+"");
	if(nQtyHorStiff==0)
	{
		if(pVerStiff->m_dRateSpaceHeight>1.0)
		{
			strText.Format("        熱ゎ爾鬼營蒂 餌辨ж雖 彊堅  a/b ˇ 1");
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )昂 + [ ( f / 365 ) 昌 + { 任 / ( 61 + 81(b/a)昌) }昌 ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
		else
		{
			strText.Format("        熱ゎ爾鬼營蒂 餌辨ж雖 彊堅  a/b ‾ 1");
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )昂 + [ ( f / 365 ) 昌 + { 任 / ( 81 + 61(b/a)昌) }昌 ]");
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
			strText.Format("        熱ゎ爾鬼營 %d欽擊 餌辨ж堅  a/b ˇ 0.8", 1);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )昂 + [ ( f / 950 ) 昌 + { 任 / ( 127 + 61(b/a)昌) }昌 ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
		else
		{
			strText.Format("        熱ゎ爾鬼營 %d欽擊 餌辨ж堅  a/b ‾ 0.8", 1);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )昂 + [ ( f / 950 ) 昌 + { 任 / ( 95 + 81(b/a)昌) }昌 ]");
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
			strText.Format("        熱ゎ爾鬼營 %d欽擊 餌辨ж堅  a/b ˇ 0.64", 2);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )昂 + [ ( f / 3150 ) 昌 + { 任 / ( 197 + 61(b/a)昌) }昌 ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.3f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.3f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
		else
		{
			strText.Format("        熱ゎ爾鬼營 %d欽擊 餌辨ж堅  a/b ‾ 0.64", 2);
			Arr.Add(strText+"");
			strText.Format("        ( b / 100t )昂 + [ ( f / 3150 ) 昌 + { 任 / ( 148 + 81(b/a)昌) }昌 ]");
			Arr.Add(strText+"");
			if(pVerStiff->m_dAllowSpaceCoeff<1.0)
				strText.Format("         = %.4f  <  1.0  .... O.K", pVerStiff->m_dAllowSpaceCoeff);
			else
				strText.Format("         = %.4f  >  1.0  .... N.G", pVerStiff->m_dAllowSpaceCoeff);
			Arr.Add(strText+"");
		}
	}
	strText.Format("      熱霜爾鬼營曖 鬼紫匐饜    [紫.撲 3.8.5.2]");
	Arr.Add(strText+"");
	strText.Format("       乓 = 8.0 ▼ (b/a)昌 = %.3f", pVerStiff->m_dGamma);
	Arr.Add(strText+"");
	strText.Format("        I(req) = b﹞t昆﹞乓 / 10.92 = %.1f mm昂", pVerStiff->m_dIReq);
	Arr.Add(strText+"");
	if(pVerStiff->m_dIUse>pVerStiff->m_dIReq)
		strText.Format("        I(use) = %.1f ▼ %.1f昆/ 3 ▼ %d獄 = %.1f mm昂  >  I(req)    .... O.K", pVerStiff->m_dTvs, pVerStiff->m_dBvs, pVerStiff->m_nVerSide, pVerStiff->m_dIUse);
	else
		strText.Format("        I(use) = %.1f ▼ %.1f昆/ 3 ▼ %d獄 = %.1f mm昂  <  I(req)    .... N.G", pVerStiff->m_dTvs, pVerStiff->m_dBvs, pVerStiff->m_nVerSide, pVerStiff->m_dIUse);
	Arr.Add(strText+"");
}

void CPlateStressOut::HorStiff(CStringArray &Arr, CCrossBeamHorStiff *pHorStiff)
{
	CString	strText;

	strText.Format("     * 熱ゎ爾鬼營  ( %s 餌辨 )", pHorStiff->m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        熱霜爾鬼營曖 除問      :   a = %.1f mm", pHorStiff->m_dVerStiffSpace);
	Arr.Add(strText+"");
	strText.Format("        餌辨 熱ゎ爾鬼營曖 ァ   : bhs = %.1f mm", pHorStiff->m_dBhs);
	Arr.Add(strText+"");
	if(pHorStiff->m_dThs>pHorStiff->m_dThsMin)
		strText.Format("        餌辨 熱ゎ爾鬼營曖 舒眷 : ths = %.1f mm  >  tmin = b / 16 = %.3f mm    .... O.K", pHorStiff->m_dThs, pHorStiff->m_dThsMin);
	else
		strText.Format("        餌辨 熱ゎ爾鬼營曖 舒眷 : ths = %.1f mm  <  tmin = b / 16 = %.3f mm    .... N.G", pHorStiff->m_dThs, pHorStiff->m_dThsMin);
	Arr.Add(strText+"");
	strText.Format("      乓 = 30.0 ▼ (a/b) = 30.0 ▼ ( %.3f / %.3f ) = %.3f", pHorStiff->m_dVerStiffSpace, pHorStiff->m_dHeightWeb, pHorStiff->m_dGamma);
	Arr.Add(strText+"");
	strText.Format("       I(req) = b﹞t昆/ 10.92 ▼ 乓h = %.1f mm昂", pHorStiff->m_dIReq);
	Arr.Add(strText+"");
	strText.Format("       I(use) = 卷 ▼ %.0f ▼ %.0f昆 = %.1fmm昂", pHorStiff->m_dThs, pHorStiff->m_dBhs, pHorStiff->m_dIUse);
	Arr.Add(strText+"");
}

// 欽睡 陛煎爾(羲V粽溯檜諒⑽ 顫殮)
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
	strText.Format("    < 欽睡 熱霜粽溯檜諒(羲 V⑽) - 剪渦 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");
	strText.Format("    ﹞h = %.1f mm    ﹞a = %.1f mm    ﹞亥 = %.3f﹉", pInVBracing->m_dHeight, pInVBracing->m_dGirderSpace, ToDegree(pInVBracing->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("     ﹞餌 辨 鬼 營");
	Arr.Add(strText+"");
	strText.Format("       忙式式式式式式成式式式式式式式式式式成式式式式式成式式式式式成式式式式式成式式式式式成式式式式式忖");	Arr.Add(strText+"");
	strText.Format("       弛   睡   營  弛    鬼 營 敘 問     弛 A (mm昌) 弛 乓x (mm) 弛 乓y (mm) 弛  乓min   弛 坐 (mm)  弛");	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式托式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛   鼻⑷營   弛 L - %3.0f X %3.0f X %2.0f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛", pInVBracing->m_pChordUp->m_dHeight, pInVBracing->m_pChordUp->m_dWidth,
		pInVBracing->m_pChordUp->m_dThick1, pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_pChordUp->m_dGammaX, pInVBracing->m_pChordUp->m_dGammaY, pInVBracing->m_pChordUp->m_dGammaMin, pInVBracing->m_pChordUp->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式托式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛   餌  營   弛 L - %3.0f X %3.0f X %2.0f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛", pInVBracing->m_pDiagonal->m_dHeight, pInVBracing->m_pDiagonal->m_dWidth,
		pInVBracing->m_pChordUp->m_dThick1, pInVBracing->m_pDiagonal->m_dArea, pInVBracing->m_pDiagonal->m_dGammaX, pInVBracing->m_pDiagonal->m_dGammaY, pInVBracing->m_pDiagonal->m_dGammaMin, pInVBracing->m_pDiagonal->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式托式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛   ж⑷營   弛 L - %3.0f X %3.0f X %2.0f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛", pInVBracing->m_pChordLo->m_dHeight, pInVBracing->m_pChordLo->m_dWidth,
		pInVBracing->m_pChordUp->m_dThick1, pInVBracing->m_pChordLo->m_dArea, pInVBracing->m_pChordLo->m_dGammaX, pInVBracing->m_pChordLo->m_dGammaY, pInVBracing->m_pChordLo->m_dGammaMin, pInVBracing->m_pChordLo->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       戌式式式式式式扛式式式式式式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式戎");	Arr.Add(strText+"");
	strText.Format("    陛) ж 醞 骯 薑");	Arr.Add(strText+"");
	strText.Format("      (1) 翱霜ж醞");	Arr.Add(strText+"");
	strText.Format("       1) 堅薑ж醞");	Arr.Add(strText+"");
	strText.Format("        ﹞ ん濰舒眷 Tp = %.3f m    ﹞ 鼻っ舒眷 Tc = %.3f m ", toM(dThickPave), toM(dThickSlab));
	Arr.Add(strText+"");
	strText.Format("        ﹞ 輿⑽除問 a = %.3f m     ﹞ S = %.3f m", toM(pInVBracing->m_dGirderSpace), toM(pInVBracing->m_strtVerLoad.m_dDistVerBracingToEndSlab));
	Arr.Add(strText+"");
	strText.Format("            S : 熱霜粽溯檜諒縑憮 鼻っ欽睡梱雖曖 剪葬");
	Arr.Add(strText+"");
	strText.Format("            ん濰 : %.3f ▼ %.3f = %.3f kN/m昌", toM(dThickPave), tokNPM3(dUnitPave), tokNPM2(pInVBracing->m_strtVerLoad.m_dLoadPave));
	Arr.Add(strText+"");
	strText.Format("            鼻っ : %.3f ▼ %.3f = %.3f kN/m昌", toM(dThickSlab), tokNPM3(dUnitConc), tokNPM2(pInVBracing->m_strtVerLoad.m_dLoadSlab));
	Arr.Add(strText+"");
	strText.Format("                           м啗 : %.3f kN/m昌", tokNPM2(dSumLoad));
	Arr.Add(strText+"");
	strText.Format("            q1 = %.3f ▼ %.3f = %.3f kN/m", toM(pInVBracing->m_strtVerLoad.m_dDistVerBracingToEndSlab), tokNPM2(dSumLoad), tokNPM(pInVBracing->m_strtVerLoad.m_dQ1));
	Arr.Add(strText+"");
	strText.Format("            q2 = %.3f ▼ %.3f = %.3f kN/m", toM(pInVBracing->m_dGirderSpace/2), tokNPM2(dSumLoad), tokNPM(pInVBracing->m_strtVerLoad.m_dQ2));
	Arr.Add(strText+"");
	strText.Format("            Pd = q1﹞a/2 + 1/3﹞q2﹞a = %.3f kN", tokN(pInVBracing->m_strtVerLoad.m_dPd));
	Arr.Add(strText+"");
	strText.Format("       2) �勒狫�     Pr = %.0f kN", tokN(dPr));
	Arr.Add(strText+"");
	strText.Format("         Pr﹞甲y = %.0f ▼ %.3f = %.3f kN", tokN(dPr), pInVBracing->m_strtVerLoad.m_dLength, tokN(dPr*pInVBracing->m_strtVerLoad.m_dLength));
	Arr.Add(strText+"");
	strText.Format("          N1 = 0");
	Arr.Add(strText+"");
	strText.Format("          N2 = Pr﹞甲y / (2﹞sin玄)      sin玄 = %.3f", sin(pInVBracing->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("          N3 = Pr﹞甲y / (2﹞tan玄)      tan玄 = %.3f", tan(pInVBracing->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("      (2) 熱ゎж醞");
	Arr.Add(strText+"");
	strText.Format("          Ёж醞縑 渠ж罹朝 2褻曖 欽睡熱霜粽溯檜諒檜 嫡朝 匙戲煎 ж貊,");
	Arr.Add(strText+"");
	strText.Format("          雖霞ж醞縑 渠ж罹朝 瞪羹曖 欽睡熱霜粽溯檜諒檜 敕蛔ж啪 睡氬ж朝 匙戲煎 и棻.");
	Arr.Add(strText+"");
	strText.Format("       1) Ёж醞");
	Arr.Add(strText+"");
	strText.Format("         - Ёж醞 鬼紫");
	Arr.Add(strText+"");
	strText.Format("            忙式式式式式式式式式式成式式式式式式式式式式式式式式忖");		Arr.Add(strText+"");
	strText.Format("            弛     欽 賊 ⑽ 鼻    弛        Ё ж 醞 (kN/m)     弛");		Arr.Add(strText+"");
	strText.Format("            戍式式式式式式式式式式托式式式式式式式式式式式式式式扣");		Arr.Add(strText+"");
	strText.Format("            弛   1 ‾ B/D < 8     弛 [ 4.0 - 0.2(B/D) ] D ￣ 6  弛");		Arr.Add(strText+"");
	strText.Format("            戍式式式式式式式式式式托式式式式式式式式式式式式式式扣");		Arr.Add(strText+"");
	strText.Format("            弛      8 ‾ B/D      弛         2.4 D ￣ 6         弛");		Arr.Add(strText+"");
	strText.Format("            戌式式式式式式式式式式扛式式式式式式式式式式式式式式戎");		Arr.Add(strText+"");
	strText.Format("         - Ёж醞 鬼紫 啗骯");
	Arr.Add(strText+"");
	strText.Format("           掖榆 ァ錳 (B) = %.3f m", toM(pInVBracing->m_strtHorLoad.m_dWidthBridge));
	Arr.Add(strText+"");

	CCentSeparation	*pSep				= pDB->GetCurGuardWall(CCentSeparation::LEFT);
	double			dHeightGuard		= pSep ? pSep->GetHeight() : 0;
	double			dHeightGuardFence	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? 0.0 : dHeightGuard;
	double			dHeightSoundProof	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm ? pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmHeight : 0.0;
	double			dHeightHandRail		= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan ? 400.0 : 0.0;

	double dBridgeHeight = toM(pBx->GetHeightGirderByBx() + pGir->GetThickSlab(pBx, 0) + dHeightGuardFence + dHeightSoundProof + dHeightHandRail);
	strText.Format("           掖榆 堪檜 (D) = %.3f m", dBridgeHeight);
	Arr.Add(strText+"");
	strText.Format("           B / D = %.3f / %.3f = %.3f m",
		toM(pInVBracing->m_strtHorLoad.m_dWidthBridge), dBridgeHeight, pInVBracing->m_strtHorLoad.m_dRateWidthHeight);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtHorLoad.m_dRateWidthHeight>=1 && pInVBracing->m_strtHorLoad.m_dRateWidthHeight<8)
		strText.Format("            1 ‾ B / D < 8  檜嘎煎  Ww = [ 4.0 - 0.2 ( B / D ) ] D = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWw));
	else if(pInVBracing->m_strtHorLoad.m_dRateWidthHeight>=8)
		strText.Format("              8 ‾ B / D   檜嘎煎  Ww = 2.4 ﹞D = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWw));
	else
		strText.Format("              營匐饜 蹂蜂");
	Arr.Add(strText+"");
	if(tokN(pInVBracing->m_strtHorLoad.m_dWw)>=6.0)
		strText.Format("              Ww ￣ 6.0 kN/m    檜嘎煎 ˍ Ww = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWwUse));
	else
		strText.Format("              Ww < 6.0 kN/m    檜嘎煎 ˍ Ww = %.3f kN/m", tokNPM(pInVBracing->m_strtHorLoad.m_dWwUse));
	Arr.Add(strText+"");
	strText.Format("           - Ёж醞縑 曖и 粽溯檜諒 1褻曖 濛辨溘");
	Arr.Add(strText+"");
	strText.Format("             п渡 雖除濰 L = %.3f m", toM(pInVBracing->m_strtHorLoad.m_dLengthSpan));
	Arr.Add(strText+"");
	strText.Format("             剪渦熱      n = %d", pDB->GetGirdersu());
	Arr.Add(strText+"");
	strText.Format("             Ёж醞 啗骯衛 ъ辨擬溘 й隸啗熱 : 1.2");
	Arr.Add(strText+"");
	if(pDB->GetGirdersu()>=3)
		strText.Format("             Hw = 1/2 ﹞ Ww ﹞ L / 1.2 = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dHw));
	else
		strText.Format("             Hw = Ww ﹞ L  / 1.2 = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dHw));
	Arr.Add(strText+"");
	strText.Format("       2) 雖霞ж醞");
	Arr.Add(strText+"");
	strText.Format("         - п渡 雖薄縑 濛辨ж朝 堅薑ж醞");
	Arr.Add(strText+"");
	strText.Format("         甲Rd = п渡 雖薄奩溘曖 м啗 = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dReaction));
	Arr.Add(strText+"");
	strText.Format("         - 雖霞 陛樓紫 啗熱(A)");
	Arr.Add(strText+"");
	if(nGradeEarth==0)	strText.Format("              頂霞蛔晝 : 弘蛔晝  ╮  嬪я紫 啗熱 : %.3f", dRiskCoefficient);
	else				strText.Format("              頂霞蛔晝 : 弗蛔晝  ╮  嬪я紫 啗熱 : %.3f", dRiskCoefficient);
	Arr.Add(strText+"");
	if(nRegionEarth==0)	strText.Format("              頂霞掘羲 : 弘掘羲  ╮  雖霞掘羲啗熱 : %.3f", dZoneCoefficient);
	else				strText.Format("              頂霞掘羲 : 弗掘羲  ╮  雖霞掘羲啗熱 : %.3f", dZoneCoefficient);
	Arr.Add(strText+"");
	strText.Format("          A = %.3f ▼ %.3f = %.3f", dRiskCoefficient, dZoneCoefficient, pInVBracing->m_strtHorLoad.m_dAcceleration);
	Arr.Add(strText+"");
	strText.Format("          - 欽睡 粽溯檜諒縑 濛辨ж朝 識雖霞ж醞");
	Arr.Add(strText+"");
	strText.Format("          甲He = A 甲Rd = %.3f ▼ %.3f = %.3f kN",
		pInVBracing->m_strtHorLoad.m_dAcceleration, tokN(pInVBracing->m_strtHorLoad.m_dReaction), tokN(pInVBracing->m_strtHorLoad.m_dTotEarthQuakeLoad));
	Arr.Add(strText+"");
	strText.Format("         - 雖霞ж醞縑 曖и 粽溯檜諒 1褻曖 濛辨溘");
	Arr.Add(strText+"");
	strText.Format("           雖霞衛曖 ъ辨擬溘 й隸啗熱 : 1.5");
	Arr.Add(strText+"");
	strText.Format("           He = 1/ (n-1)﹞甲He = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dEarthQuakeLoad));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtHorLoad.m_dHw>pInVBracing->m_strtHorLoad.m_dEarthQuakeLoad)
		strText.Format("           Hw  >  He  檜嘎煎   H = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dH));
	else
		strText.Format("           Hw  <  He  檜嘎煎   H = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dH));
	Arr.Add(strText+"");	Arr.Add("");
	strText.Format("    釭) 睡營溘 啗骯");	Arr.Add(strText+"");
	strText.Format("     1) 鼻⑷營(AB)");	Arr.Add(strText+"");
	double	dCollCoeff	= 15.0/(40.0+toM(pInVBracing->m_pChordUp->m_dLength/2.0));
	if(dCollCoeff>0.3)
		strText.Format("        醱問啗熱 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f > 0.3  ╮ i = %.3f",
		toM(pInVBracing->m_pChordUp->m_dLength/2.0), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefUp);
	else
		strText.Format("        醱問啗熱 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f < 0.3  ╮ i = %.3f",
		toM(pInVBracing->m_pChordUp->m_dLength/2.0), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefUp);
	Arr.Add(strText+"");
	strText.Format("        熱ゎж醞 營ж衛");
	Arr.Add(strText+"");
	strText.Format("         N1  =  1/2 H = 1/2 ▼ %.3f = %.3f kN", tokN(pInVBracing->m_strtHorLoad.m_dH), tokN(pInVBracing->m_strtElementLoad.m_dN1));
	Arr.Add(strText+"");
	strText.Format("         翱霜 �勒狫� 營ж衛 (函曖 唳辦)");
	Arr.Add(strText+"");
	strText.Format("          Ml+i = Pr﹞(a/2)/4 ▼ (1+i) = %.3f kN﹞m", tokNM(pInVBracing->m_strtElementLoad.m_dMl));
	Arr.Add(strText+"");
	strText.Format("     2) 餌營(BD,BE)");
	Arr.Add(strText+"");
	dCollCoeff	= 15.0/(40.0+toM(pInVBracing->m_pDiagonal->m_dLength));
	if(dCollCoeff>0.3)
		strText.Format("        醱問啗熱 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f > 0.3  ╮ i = %.3f",
		toM(pInVBracing->m_pDiagonal->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefDiagonal);
	else
		strText.Format("        醱問啗熱 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f < 0.3  ╮ i = %.3f",
		toM(pInVBracing->m_pDiagonal->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefDiagonal);
	Arr.Add(strText+"");
	strText.Format("        N2 = 1/4 H﹞sec玄     ( 熱ゎж醞虜 濛辨衛, BD睡營 檣濰溘 譆渠)");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN2Hor));
	Arr.Add(strText+"");
	strText.Format("        N2 = (Pd + Pr甲y(1+i))/(2﹞sin玄) + 1/4 H﹞sec玄    ( 翱霜+熱ゎж醞 濛辨衛, BE睡營 憶蹴溘 譆渠 )");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN2VerHor));
	Arr.Add(strText+"");
	strText.Format("     3) ж⑷營(DE)");
	Arr.Add(strText+"");
	dCollCoeff	= 15.0/(40.0+toM(pInVBracing->m_pChordLo->m_dLength));
	if(dCollCoeff>0.3)
		strText.Format("        醱問啗熱 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f > 0.3  ╮ i = %.3f",
		toM(pInVBracing->m_pChordLo->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefLo);
	else
		strText.Format("        醱問啗熱 i = 15 / (40 + L) = 15 / ( 40 + %.3f ) = %.3f < 0.3  ╮ i = %.3f",
		toM(pInVBracing->m_pChordLo->m_dLength), dCollCoeff, pInVBracing->m_strtElementLoad.m_dCollCoefLo);
	Arr.Add(strText+"");
	strText.Format("        N3 = (Pd + Pr甲y﹞(1+i))/(2﹞tan玄)     (翱霜ж醞虜 濛辨衛, 檣濰溘 譆渠)");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN3Ver));
	Arr.Add(strText+"");
	strText.Format("        N3 = 3/4 H    (熱ゎж醞虜 濛辨衛, 憶蹴溘 譆渠)");
	Arr.Add(strText+"");
	strText.Format("           = %.3f kN", tokN(pInVBracing->m_strtElementLoad.m_dN3Hor));
	Arr.Add(strText+"");Arr.Add("");
	
	strText.Format("    棻) 睡營曖 撲啗");	Arr.Add(strText+"");
	strText.Format("     1) 鼻 ⑷ 營    ( %s )", pInVBracing->m_strtChordUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        欽賊瞳                :     Ag = %.1f mm昌", pInVBracing->m_pChordUp->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳曖 譆模高 :  乓min = %.1f mm", pInVBracing->m_pChordUp->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳          :    乓x = %.1f mm", pInVBracing->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	if(pInVBracing->m_pChordUp->m_nType==CSteelMaterialData::CSTEEL)
		strText.Format("        欽賊 啗熱             :     Zx = %.1f mm昆", pInVBracing->m_pChordUp->m_dZy);
	else
		strText.Format("        欽賊 啗熱             :     Zy = %.1f mm昆", pInVBracing->m_pChordUp->m_dZy);
	Arr.Add(strText+"");
	strText.Format("          函 撮濰綠 匐饜");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordUp.m_dSlendernessRatio<150.0)
		strText.Format("            坐/乓min = %.1f / %.1f = %.3f  < 150      .... O.K  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pInVBracing->m_pChordUp->m_dLength, pInVBracing->m_pChordUp->m_dGammaMin, pInVBracing->m_strtChordUp.m_dSlendernessRatio);
	else
		strText.Format("            坐/乓min = %.1f / %.1f = %.3f  > 150      .... N.G  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pInVBracing->m_pChordUp->m_dLength, pInVBracing->m_pChordUp->m_dGammaMin, pInVBracing->m_strtChordUp.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          刻 ъ辨擬溘     ( Ｃ 紫.撲.晦 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("           - ъ辨 蹴寞щ 憶蹴擬溘");
	Arr.Add(strText+"");
	strText.Format("            坐/乓x = %.1f / %.1f = %.3f", pInVBracing->m_pChordUp->m_dLength, pInVBracing->m_pChordUp->m_dGammaX,
		pInVBracing->m_pChordUp->m_dLength/pInVBracing->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("             fca = %.3f MPa", pInVBracing->m_strtChordUp.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE縑 翱唸腎橫 氈朝 唳辦縑朝 ъ辨憶蹴擬溘縑 盪馬睦擊 培ж罹 匐饜и棻.");
	Arr.Add(strText+"");
	strText.Format("             fcaㄖ=  fca ▼ ( 0.5 + ( 坐/乓x ) / 1000 )");
	Arr.Add(strText+"");
	strText.Format("                  = %.3f MPa", pInVBracing->m_strtChordUp.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("           - ъ辨 �睎懋�");
	Arr.Add(strText+"");
	strText.Format("             fa = %.1f MPa", pInVBracing->m_strtChordUp.m_dFa);
	Arr.Add(strText+"");
	strText.Format("          券 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("              蹴寞щ憶蹴擬溘");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordUp.m_dFc<pInVBracing->m_strtChordUp.m_dFcaApo)
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  <  fca'    .... O.K",
			pInVBracing->m_strtElementLoad.m_dN1, pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_strtChordUp.m_dFc);
	else
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  >  fca'    .... N.G",
			pInVBracing->m_strtElementLoad.m_dN1, pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_strtChordUp.m_dFc);
	Arr.Add(strText+"");
	strText.Format("              �睎懋�");
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
	strText.Format("          刷 Gusset Plate諦曖 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.3f MPa", pInVBracing->m_strtChordUp.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.3f mm", pInVBracing->m_strtChordUp.m_dLengthWeld/2, pInVBracing->m_strtChordUp.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾跡舒眷        :  a  = %.1f / ∵2 = %.3f mm", pInVBracing->m_strtChordUp.m_dThroatDepth, pInVBracing->m_strtChordUp.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordUp.m_dFc<0.75*pInVBracing->m_strtChordUp.m_dFcaApo)
	{
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜ж 檜嘎煎 賅營鬼紫曖 75%%蒂 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N1' = 0.75 ▼ %.1f ▼ %.3f = %.3f kN",
			pInVBracing->m_pChordUp->m_dArea, pInVBracing->m_strtChordUp.m_dFcaApo, tokN(pInVBracing->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜鼻 檜嘎煎 濛辨ж醞擊 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N1' = N1 = %.3f kN", tokN(pInVBracing->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	if(pInVBracing->m_strtChordUp.m_dShear<pInVBracing->m_strtChordUp.m_dAllowShear)
		strText.Format("             任 = N1'/ a﹞甲L = %.3f MPa  <  任a    .... O.K", pInVBracing->m_strtChordUp.m_dShear);
	else
		strText.Format("             任 = N1'/ a﹞甲L = %.3f MPa  >  任a    .... N.G", pInVBracing->m_strtChordUp.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     2) 餌 營    ( %s )", pInVBracing->m_strtDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        欽賊瞳                :    Ag = %.1f mm昌", pInVBracing->m_pDiagonal->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳曖 譆模高 : 乓min = %.1f mm", pInVBracing->m_pDiagonal->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳          :  乓x  = %.1f mm", pInVBracing->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          函 撮濰綠 匐饜");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  <  150    .... O.K  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pInVBracing->m_pDiagonal->m_dLength, pInVBracing->m_pDiagonal->m_dGammaMin, pInVBracing->m_strtDiagonal.m_dSlendernessRatio);
	else
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  >  150    .... N.G  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pInVBracing->m_pDiagonal->m_dLength, pInVBracing->m_pDiagonal->m_dGammaMin, pInVBracing->m_strtDiagonal.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          刻 ъ辨擬溘    ( Ｃ 紫.撲.晦 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - ъ辨憶蹴擬溘");
	Arr.Add(strText+"");
	strText.Format("            坐/乓x = %.1f / %.1f = %.3f",
		pInVBracing->m_pDiagonal->m_dLength, pInVBracing->m_pDiagonal->m_dGammaX, pInVBracing->m_pDiagonal->m_dLength/pInVBracing->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pInVBracing->m_strtDiagonal.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE縑 翱唸腎橫 氈朝 唳辦縑朝 ъ辨憶蹴擬溘縑 盪馬睦擊 培ж罹 匐饜и棻.");
	Arr.Add(strText+"");
	strText.Format("            fcaㄖ=  fca ▼ ( 0.5 + ( 坐/乓x ) / 1000 ) = %.1f MPa", pInVBracing->m_strtDiagonal.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("            - ъ辨檣濰擬溘");
	Arr.Add(strText+"");
	strText.Format("            fta = %.1f MPa", pInVBracing->m_strtDiagonal.m_dFta);
	Arr.Add(strText+"");
	strText.Format("          券 擬溘匐饜");
	Arr.Add(strText+"");
	if(pInVBracing->m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		strText.Format("            檣濰 L⑽鬼曖 嶸�艘僄橉�     (紫撲 3.4.5)");
	else
		strText.Format("            檣濰 T⑽鬼曖 嶸�艘僄橉�     (紫撲 3.4.5)");
	Arr.Add(strText+"");
	strText.Format("            An = %.1f mm昌", pInVBracing->m_strtDiagonal.m_dAn);
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
	strText.Format("          刷 Gusset Plate諦曖 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.1f MPa ( Ｃ 紫.撲.晦 3.3.2.3 )", pInVBracing->m_strtDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.1f mm", pInVBracing->m_strtDiagonal.m_dLengthWeld/2.0, pInVBracing->m_strtDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾跡舒眷        :  a  = %.1f / ∵2 = %.1f mm", pInVBracing->m_strtDiagonal.m_dThroatDepth, pInVBracing->m_strtDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtDiagonal.m_dFc<0.75*pInVBracing->m_strtDiagonal.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 檜嘎煎", pInVBracing->m_strtDiagonal.m_dFc, 0.75*pInVBracing->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜ж 檜嘎煎 賅營鬼紫曖 75%%蒂 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N2' = 0.75 ▼ %.1f ▼ %.1f = %.3f kN",
			pInVBracing->m_pDiagonal->m_dArea, pInVBracing->m_strtDiagonal.m_dFcaApo, tokN(pInVBracing->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 檜嘎煎", pInVBracing->m_strtDiagonal.m_dFc, 0.75*pInVBracing->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜鼻 檜嘎煎 濛辨ж醞擊 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N2' = N2 = %.3f kN", tokN(pInVBracing->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	if(pInVBracing->m_strtDiagonal.m_dShear<pInVBracing->m_strtDiagonal.m_dAllowShear)
		strText.Format("             任 = N2'/ a﹞甲L = %.1f MPa  <  任a    .... O.K", pInVBracing->m_strtDiagonal.m_dShear);
	else
		strText.Format("             任 = N2'/ a﹞甲L = %.1f MPa  >  任a    .... N.G", pInVBracing->m_strtDiagonal.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     3) ж ⑷ 營    ( %s )", pInVBracing->m_strtChordLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        欽賊瞳                :    Ag = %.1f mm昌", pInVBracing->m_pChordLo->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳曖 譆模高 : 乓min = %.1f mm", pInVBracing->m_pChordLo->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳          :  乓x  = %.1f mm", pInVBracing->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          函 撮濰綠 匐饜");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  <  150    .... O.K  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pInVBracing->m_pChordLo->m_dLength, pInVBracing->m_pChordLo->m_dGammaMin, pInVBracing->m_strtChordLo.m_dSlendernessRatio);
	else
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  >  150    .... N.G  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pInVBracing->m_pChordLo->m_dLength, pInVBracing->m_pChordLo->m_dGammaMin, pInVBracing->m_strtChordLo.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          刻 ъ辨擬溘    ( Ｃ 紫.撲.晦 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - ъ辨憶蹴擬溘");
	Arr.Add(strText+"");
	strText.Format("            坐/乓x = %.1f / %.1f = %.3f",
		pInVBracing->m_pChordLo->m_dLength, pInVBracing->m_pChordLo->m_dGammaX, pInVBracing->m_pChordLo->m_dLength/pInVBracing->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pInVBracing->m_strtChordLo.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE縑 翱唸腎橫 氈朝 唳辦縑朝 ъ辨憶蹴擬溘縑 盪馬睦擊 培ж罹 匐饜и棻.");
	Arr.Add(strText+"");
	strText.Format("            fcaㄖ=  fca ▼ ( 0.5 + ( 坐/乓x ) / 1000 ) = %.1f MPa", pInVBracing->m_strtChordLo.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("            - ъ辨檣濰擬溘");
	Arr.Add(strText+"");
	strText.Format("            fta = %.1f MPa", pInVBracing->m_strtChordLo.m_dFta);
	Arr.Add(strText+"");
	strText.Format("          券 擬溘匐饜");
	Arr.Add(strText+"");
	if(pInVBracing->m_pChordLo->m_nType==CSteelMaterialData::CTSTEEL)
		strText.Format("            檣濰 L⑽鬼曖 嶸�艘僄橉�     (紫撲 3.4.5)");
	else
		strText.Format("            檣濰 T⑽鬼曖 嶸�艘僄橉�     (紫撲 3.4.5)");
	Arr.Add(strText+"");
	strText.Format("                An = %.1f mm昌", pInVBracing->m_strtChordLo.m_dAn);
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
	strText.Format("          刷 Gusset Plate諦曖 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.1f MPa    ( Ｃ 紫.撲.晦 3.3.2.3 )", pInVBracing->m_strtChordLo.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.1f mm", pInVBracing->m_strtChordLo.m_dLengthWeld/2, pInVBracing->m_strtChordLo.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾跡舒眷        :   a = %.1f / ∵2 = %.1f mm", pInVBracing->m_strtChordLo.m_dThroatDepth, pInVBracing->m_strtChordLo.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtChordLo.m_dFc<0.75*pInVBracing->m_strtChordLo.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 檜嘎煎", pInVBracing->m_strtChordLo.m_dFc, 0.75*pInVBracing->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜ж 檜嘎煎 賅營鬼紫曖 75%%蒂 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N3' = 0.75 ▼ %.1f ▼ %.1f = %.3f kN",
			pInVBracing->m_pChordLo->m_dArea, pInVBracing->m_strtChordLo.m_dFcaApo, tokN(pInVBracing->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 檜嘎煎", pInVBracing->m_strtChordLo.m_dFc, 0.75*pInVBracing->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜鼻 檜嘎煎 濛辨ж醞擊 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N3' = N3 = %.3f kN", tokN(pInVBracing->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	if(pInVBracing->m_strtChordLo.m_dShear<pInVBracing->m_strtChordLo.m_dAllowShear)
		strText.Format("             任 = N3'/ a﹞甲L = %.1f MPa  <  任a    .... O.K", pInVBracing->m_strtChordLo.m_dShear);
	else
		strText.Format("             任 = N3'/ a﹞甲L = %.1f MPa  >  任a    .... N.G", pInVBracing->m_strtChordLo.m_dShear);
	Arr.Add(strText+"");
	strText.Format("    棻) Gusset Plate曖 匐饜");
	Arr.Add(strText+"");
	strText.Format("        濛辨ж醞婁 賅營鬼紫曖 75%%蒂 堅溥ж罹 撲啗и棻.");
	Arr.Add(strText+"");
	strText.Format("      1) 鼻⑷營/餌營 翱唸 (A,B)");
	Arr.Add(strText+"");
	strText.Format("       ﹞ 餌辨鬼營           : %.0f ▼ %.0f ▼ %.0f ( %s )",
		pInVBracing->m_strtGussetUp.m_dHeight, pInVBracing->m_strtGussetUp.m_dThick, pInVBracing->m_strtGussetUp.m_dWidth, pInVBracing->m_strtGussetUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 鬼營曖 ъ辨擬溘    : fa = %.1f MPa", pInVBracing->m_strtGussetUp.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 濛辨ж醞");
	Arr.Add(strText+"");
	strText.Format("          P = N1 = %.3f kN", tokN(pInVBracing->m_strtGussetUp.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       ﹞ 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1賊葆雞)   伏a = %.1f kN", pInVBracing->m_strtGussetUp.m_strBoltMaterial, pInVBracing->m_strtGussetUp.m_dDiaBolt, tokN(pInVBracing->m_strtGussetUp.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          в蹂 BOLT熱");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetUp.m_dQtyBoltUse>pInVBracing->m_strtGussetUp.m_dQtyBoltReq)
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... O.K",
		tokN(pInVBracing->m_strtGussetUp.m_dLoad), tokN(pInVBracing->m_strtGussetUp.m_dBoltAllowStress), pInVBracing->m_strtGussetUp.m_dQtyBoltReq, pInVBracing->m_strtGussetUp.m_dQtyBoltUse);
	else
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... N.G",
		tokN(pInVBracing->m_strtGussetUp.m_dLoad), tokN(pInVBracing->m_strtGussetUp.m_dBoltAllowStress), pInVBracing->m_strtGussetUp.m_dQtyBoltReq, pInVBracing->m_strtGussetUp.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("     ﹞ Gusset Plate曖 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("          An = ( %.0f - %.0f ▼ %.0f ) ▼ %.0f = %.1f mm昌",
		pInVBracing->m_strtGussetUp.m_dHeight, pInVBracing->m_strtGussetUp.m_dQtyBoltUse, pInVBracing->m_strtGussetUp.m_dDiaBolt+3.0, pInVBracing->m_strtGussetUp.m_dThick, pInVBracing->m_strtGussetUp.m_dAn);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetUp.m_dStress<pInVBracing->m_strtGussetUp.m_dAllowStress)
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetUp.m_dLoad, pInVBracing->m_strtGussetUp.m_dAn, pInVBracing->m_strtGussetUp.m_dStress);
	else
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetUp.m_dLoad, pInVBracing->m_strtGussetUp.m_dAn, pInVBracing->m_strtGussetUp.m_dStress);
	Arr.Add(strText+"");
	strText.Format("      2) ж⑷營 翱唸 (C,E)");
	Arr.Add(strText+"");
	strText.Format("       ﹞ 餌辨鬼營            : %.0f ▼ %.0f ▼ %.0f ( %s )",
		pInVBracing->m_strtGussetDiagonal.m_dHeight, pInVBracing->m_strtGussetDiagonal.m_dThick, pInVBracing->m_strtGussetDiagonal.m_dWidth, pInVBracing->m_strtGussetDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 鬼營曖 ъ辨擬溘     : fa = %.1f MPa", pInVBracing->m_strtGussetDiagonal.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 鬼營曖 ъ辨瞪欽擬溘 : 任a = %.1f MPa", pInVBracing->m_strtGussetDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 濛辨ж醞");
	Arr.Add(strText+"");
	strText.Format("          Ph = N1' = %.3f kN", tokN(pInVBracing->m_strtGussetDiagonal.m_dLoadHor));
	Arr.Add(strText+"");
	strText.Format("          Pv = 甲P = Pd + Pl﹞(1+i) = %.3f kN", tokN(pInVBracing->m_strtGussetDiagonal.m_dLoadVer));
	Arr.Add(strText+"");
	strText.Format("          P = ∵( Ph2 + Pv2 ) = %.3f kN", tokN(pInVBracing->m_strtGussetDiagonal.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       ﹞ 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("          Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.3f MPa", pInVBracing->m_strtGussetDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("          Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.1f mm", pInVBracing->m_strtGussetDiagonal.m_dLengthWeld/2.0, pInVBracing->m_strtGussetDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("          Fillet 辨蕾跡舒眷        :  a  = %.1f / ∵2 = %.3f mm", pInVBracing->m_strtGussetDiagonal.m_dThroatDepth, pInVBracing->m_strtGussetDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetDiagonal.m_dShearWeld<pInVBracing->m_strtGussetDiagonal.m_dAllowShearWeld)
		strText.Format("          任 = P/ a﹞甲L = %.3f MPa  <  任a    .... O.K", pInVBracing->m_strtGussetDiagonal.m_dShearWeld);
	else
		strText.Format("          任 = P/ a﹞甲L = %.3f MPa  >  任a    .... N.G", pInVBracing->m_strtGussetDiagonal.m_dShearWeld);
	Arr.Add(strText+"");
	strText.Format("       ﹞ Gusset Plate曖 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("          Agv = %.1f mm昌    Agh = %.1f mm昌", pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dAgh);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetDiagonal.m_dStress<pInVBracing->m_strtGussetDiagonal.m_dAllowStress)
		strText.Format("         f = Pv / Agv = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetDiagonal.m_dLoadVer, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dStress);
	else
		strText.Format("         f = Pv / Agv = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetDiagonal.m_dLoadVer, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dStress);
	Arr.Add(strText+"");

	if(pInVBracing->m_strtGussetDiagonal.m_dShearGusset<pInVBracing->m_strtGussetDiagonal.m_dAllowShear)
		strText.Format("         任 = Ph / Agh = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetDiagonal.m_dLoadHor, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dShearGusset);
	else
		strText.Format("         任 = Ph / Agh = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetDiagonal.m_dLoadHor, pInVBracing->m_strtGussetDiagonal.m_dAgv, pInVBracing->m_strtGussetDiagonal.m_dShearGusset);
	Arr.Add(strText+"");

	strText.Format("      3) 餌營/ж⑷營 翱唸 (D)");
	Arr.Add(strText+"");
	strText.Format("       ﹞ 餌辨鬼營           : %.0f ▼ %.0f ▼ %.0f ( %s )",
		pInVBracing->m_strtGussetLo.m_dHeight, pInVBracing->m_strtGussetLo.m_dThick, pInVBracing->m_strtGussetLo.m_dWidth, pInVBracing->m_strtGussetLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 鬼營曖 ъ辨擬溘    : fa = %.1f MPa", pInVBracing->m_strtGussetLo.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 濛辨ж醞");
	Arr.Add(strText+"");
	strText.Format("          Rh = N3' + N2'cos玄 = %.3f kN", tokN(pInVBracing->m_strtGussetLo.m_dRh));
	Arr.Add(strText+"");
	strText.Format("          Rv = N2'sin玄 kN", tokN(pInVBracing->m_strtGussetLo.m_dRv));
	Arr.Add(strText+"");
	strText.Format("          P = ∵( Rh2 + Rv2 ) = %.3f kN", tokN(pInVBracing->m_strtGussetLo.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       ﹞ 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1賊葆雞)   伏a = %.1f kN", pInVBracing->m_strtGussetLo.m_strBoltMaterial, pInVBracing->m_strtGussetLo.m_dDiaBolt, tokN(pInVBracing->m_strtGussetLo.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          в蹂 BOLT熱");
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetLo.m_dQtyBoltUse>pInVBracing->m_strtGussetLo.m_dQtyBoltReq)
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... O.K",
		tokN(pInVBracing->m_strtGussetLo.m_dLoad), tokN(pInVBracing->m_strtGussetLo.m_dBoltAllowStress), pInVBracing->m_strtGussetLo.m_dQtyBoltReq, pInVBracing->m_strtGussetLo.m_dQtyBoltUse);
	else
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... N.G",
		tokN(pInVBracing->m_strtGussetLo.m_dLoad), tokN(pInVBracing->m_strtGussetLo.m_dBoltAllowStress), pInVBracing->m_strtGussetLo.m_dQtyBoltReq, pInVBracing->m_strtGussetLo.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("       ﹞ Gusset Plate曖 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("           An = %.0f ▼ %.0f = %.1f mm昌", pInVBracing->m_strtGussetLo.m_dWidth, pInVBracing->m_strtGussetLo.m_dThick, pInVBracing->m_strtGussetLo.m_dAn);
	Arr.Add(strText+"");
	if(pInVBracing->m_strtGussetLo.m_dStress<pInVBracing->m_strtGussetLo.m_dAllowStress)
		strText.Format("           f = P / An = %.3f MPa  <  fa    .... O.K", pInVBracing->m_strtGussetLo.m_dStress);
	else
		strText.Format("           f = P / An = %.3f MPa  >  fa    .... N.G", pInVBracing->m_strtGussetLo.m_dStress);
	Arr.Add(strText+"");Arr.Add("");
}

// 醞除 陛煎爾(H⑽鬼顫殮, 薯濛顫殮)
void CPlateStressOut::StressCheckCenterCrossBeamGeneralType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map)
{
	CDesignCrossBeam		CrossBeam(m_pStd->m_pCalcStd, pBx);
	CDesignCenterCrossBeam	*pCenCr		= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();

	CString	strText		= _T("");
	long	nG			= pBx->GetNumberGirder();
	double	dStation	= pBx->GetStation();

	Arr.Add("");
	strText.Format("    < 醞除 陛煎爾 - 剪渦 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");
	strText.Format("                                 餌辨鬼營 : %s", pCenCr->m_pJewonFlangeUp->m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("                                 Ы楠雖ァ : bfu = %.0f mm(鼻睡)    bfl = %.0f mm(ж睡)", pCenCr->m_pJewonFlangeUp->m_dWidth, pCenCr->m_pJewonFlangeLo->m_dWidth);
	Arr.Add(strText+"");
	strText.Format("                                 犒睡曖ァ : bw = %.0f mm", pCenCr->m_pJewonWeb->m_dHeight);
	Arr.Add(strText+"");
	strText.Format("                                 熱ゎ爾鬼營 餌辨欽熱 %d 欽", pCenCr->m_nQtyHorStiff);
	Arr.Add(strText+"");
	if(pCenCr->m_pJewonWeb->m_dWidth>=pCenCr->m_strtMinThick.m_dThickWebReq)
		strText.Format("                                 tw = %.0f mm  ￣  twmin = %.1f mm    .... O.K",
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
		strText.Format("                                 鼻っ舒眷 tfu = %.0f mm  >  tfu(mim)    .... O.K", pCenCr->m_pJewonFlangeUp->m_dHeight);
	else
		strText.Format("                                 鼻っ舒眷 tfu = %.0f mm  <  tfu(mim)    .... N.G", pCenCr->m_pJewonFlangeUp->m_dHeight);
	Arr.Add(strText+"");
	if(pCenCr->m_pJewonFlangeLo->m_dHeight>pCenCr->m_strtMinThick.m_dMinThickFlangeLo)
		strText.Format("                                 жっ舒眷 tfl = %.0f mm  >  tfl(mim)    .... O.K", pCenCr->m_pJewonFlangeLo->m_dHeight);
	else
		strText.Format("                                 жっ舒眷 tfl = %.0f mm  <  tfl(mim)    .... N.G", pCenCr->m_pJewonFlangeLo->m_dHeight);
	Arr.Add(strText+"");
	strText.Format("                                 輿⑽除問 a = %.3f m", toM(pCenCr->m_dGirderSpace));
	Arr.Add(strText+"");
	strText.Format("    陛. FRAME п戮唸婁餵啗");
	Arr.Add(strText+"");
	strText.Format("        ж醞碟寡濛辨縑 渠и 艙щ擊 奩艙ж晦 嬪ж罹 FRAME п戮唸婁蒂 堅溥и棻.");	
	Arr.Add(strText+"");
	strText.Format("       忙式式式式式式式式成式式式式式式式式式式式成式式式式式式式式式式式成式式式式式式式式式式式忖");	Arr.Add(strText+"");
	strText.Format("       弛                弛        M (kN.m)      弛       S (kN)         弛        N (kN)        弛");	Arr.Add(strText+"");
	strText.Format("       弛    掘    碟    戍式式式式式成式式式式式托式式式式式成式式式式式托式式式式式成式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛                弛    Max   弛    Min   弛    Max   弛    Min   弛    Max   弛    Min   弛");	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式扛式式式式式托式式式式式扛式式式式式托式式式式式扛式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛 м撩瞪 堅薑ж醞弛  %12.3f        弛  %12.3f        弛  %12.3f        弛",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dPreDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dPreDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dPreDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式式式式式式式托式式式式式式式式式式式托式式式式式式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛 м撩�� 堅薑ж醞弛  %12.3f        弛  %12.3f        弛  %12.3f        弛",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dPostDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dPostDeadLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dPostDeadLoad));
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式成式式式式式托式式式式式成式式式式式托式式式式式成式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛  ��   ж   醞  弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMaxLiveLoad), tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMinLiveLoad),
		tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMaxLiveLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMinLiveLoad),
		tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMaxLiveLoad), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMinLiveLoad));	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛  雖 薄 藹 ж   弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMaxSettlement), tokNM(pCenCr->m_pFrameElementLoad->m_strtMoment.m_dMinSettlement),
		tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMaxSettlement), tokN(pCenCr->m_pFrameElementLoad->m_strtShear.m_dMinSettlement),
		tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMaxSettlement), tokN(pCenCr->m_pFrameElementLoad->m_strtAxial.m_dMinSettlement));	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");					Arr.Add(strText+"");
	strText.Format("       弛     м    啗   弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛 %8.3f 弛",
		tokNM(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), tokNM(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment),
		tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinShear),
		tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxAxial), tokN(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinAxial));	Arr.Add(strText+"");
	strText.Format("       戌式式式式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式戎");				Arr.Add(strText+"");

	strText.Format("    釭. 欽賊 撲啗");
	Arr.Add(strText+"");
	strText.Format("          忙式式式式式式式式式式成式式式式式成式式式式式成式式式式式成式式式式式式成式式式式式式忖");	Arr.Add(strText+"");
	strText.Format("          弛     掘      碟     弛  A(mm昌) 弛  Y (mm)  弛 AY (mm昆)弛 AY昌(mm昂) 弛  Io(mm昂)  弛");	Arr.Add(strText+"");
	strText.Format("          戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("          弛1 - UFLG-%4.0f ▼ %2.0f 弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pCenCr->m_pJewonFlangeUp->m_dWidth, pCenCr->m_pJewonFlangeUp->m_dHeight, pCenCr->m_pJewonFlangeUp->m_dA,
		pCenCr->m_pJewonFlangeUp->m_dY,	pCenCr->m_pJewonFlangeUp->m_dAY, pCenCr->m_pJewonFlangeUp->m_dAY2, pCenCr->m_pJewonFlangeUp->m_dIo);Arr.Add(strText+"");
	strText.Format("          戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("          弛1 - WEB -%4.0f ▼ %2.0f 弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pCenCr->m_pJewonWeb->m_dHeight, pCenCr->m_pJewonWeb->m_dWidth, pCenCr->m_pJewonWeb->m_dA,
		pCenCr->m_pJewonWeb->m_dY, pCenCr->m_pJewonWeb->m_dAY, pCenCr->m_pJewonWeb->m_dAY2, pCenCr->m_pJewonWeb->m_dIo);	Arr.Add(strText+"");
	strText.Format("          戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("          弛1 - LFLG-%4.0f ▼ %2.0f 弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pCenCr->m_pJewonFlangeLo->m_dWidth, pCenCr->m_pJewonFlangeLo->m_dHeight, pCenCr->m_pJewonFlangeLo->m_dA,
		pCenCr->m_pJewonFlangeLo->m_dY,	pCenCr->m_pJewonFlangeLo->m_dAY, pCenCr->m_pJewonFlangeLo->m_dAY2, pCenCr->m_pJewonFlangeLo->m_dIo);Arr.Add(strText+"");
	strText.Format("          戍式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式式托式式式式式式扣");	Arr.Add(strText+"");
	strText.Format("          弛      м      啗    弛 %8.0f 弛 %8.0f 弛 %8.0f 弛 %10.0f 弛 %10.0f 弛",
		pCenCr->GetTotSectionArea(), 0.0, pCenCr->GetTotSectionAY(), pCenCr->GetTotSectionAY2(), pCenCr->GetTotSectionIo());Arr.Add(strText+"");
	strText.Format("          戌式式式式式式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式式扛式式式式式式戎");	Arr.Add(strText+"");
	strText.Format("      乩 = 甲A﹞Y / 甲A = %.3f mm", pCenCr->m_strtSection.m_dDelta);									Arr.Add(strText+"");
	strText.Format("       I = 甲Io + 甲A﹞Y昌 - 甲A﹞乩昌 = %.3f mm昂", pCenCr->m_strtSection.m_dI);						Arr.Add(strText+"");
	strText.Format("     Ysu =  bw / 2 + tfu - 乩 = %.3f mm", pCenCr->m_strtSection.m_dYsu);								Arr.Add(strText+"");
	strText.Format("     Ysl =  bw / 2 + tfl + 乩 = %.3f mm", pCenCr->m_strtSection.m_dYsl);								Arr.Add(strText+"");
	strText.Format("     憶蹴睡曖 措睡謝掉縑 渠и ъ辨擬溘擎  [紫.撲 ル3.3.4(b), ル 3.4.4]");								Arr.Add(strText+"");
	strText.Format("      ˍ Fca = %.3f MPa,    仲ta = %.3f MPa,    任a = %.3f MPa",
		pCenCr->m_strtSection.m_dFca, pCenCr->m_strtSection.m_dSigmata, pCenCr->m_strtSection.m_dSheara);					Arr.Add(strText+"");
	if(pCenCr->m_strtSection.m_dFu<pCenCr->m_strtSection.m_dFca)
		strText.Format("      fu = M﹞Ysu / I - Nmin / Ag = %.3f MPa  <  fca = %.3f MPa    .... O.K", pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca);
	else
		strText.Format("      fu = M﹞Ysu / I - Nmin / Ag = %.3f MPa  >  fca = %.3f MPa    .... N.G", pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca);
	Arr.Add(strText+"");
	if(pCenCr->m_strtSection.m_dFl<pCenCr->m_strtSection.m_dSigmata)
		strText.Format("      fl = M﹞Ysl / I + Nmin / Ag = %.3f MPa  <  fta = %.3f MPa    .... O.K", pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata);
	else
		strText.Format("      fl = M﹞Ysl / I + Nmin / Ag = %.3f MPa  >  fta = %.3f MPa    .... N.G", pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata);
	Arr.Add(strText+"");
	if(pCenCr->m_strtSection.m_dShear<pCenCr->m_strtSection.m_dSheara)
		strText.Format("      任 = S / Aw = %.3f MPa  <  任a = %.3f MPa    .... O.K", pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara);
	else
		strText.Format("      任 = S / Aw = %.3f MPa  >  任a = %.3f MPa    .... N.G", pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara);
	Arr.Add(strText+"");

	strText.Format("      м撩擬溘曖 匐饜");
	Arr.Add(strText+"");
	if(fabs(pCenCr->m_strtSection.m_dFu/pCenCr->m_strtSection.m_dFca)>fabs(pCenCr->m_strtSection.m_dFl/pCenCr->m_strtSection.m_dSigmata))
	{
		if(pCenCr->m_strtSection.m_dCompStress<1.2)
		{
			strText.Format("      ( %.3f / %.3f )昌 + ( %.3f / %.3f )昌 = %.3f < 1.2    .... O.K",
			pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
		else
		{
			strText.Format("      ( %.3f / %.3f )昌 + ( %.3f / %.3f )昌 = %.3f < 1.2    .... N.G",
			pCenCr->m_strtSection.m_dFu, pCenCr->m_strtSection.m_dFca, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
	}
	else
	{
		if(pCenCr->m_strtSection.m_dCompStress<1.2)
		{
			strText.Format("      ( %.3f / %.3f )昌 + ( %.3f / %.3f )昌 = %.3f < 1.2    .... O.K",
			pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
		else
		{
			strText.Format("      ( %.3f / %.3f )昌 + ( %.3f / %.3f )昌 = %.3f < 1.2    .... N.G",
			pCenCr->m_strtSection.m_dFl, pCenCr->m_strtSection.m_dSigmata, pCenCr->m_strtSection.m_dShear, pCenCr->m_strtSection.m_dSheara, pCenCr->m_strtSection.m_dCompStress);
		}
	}
	Arr.Add(strText+"");
	CCrossBeamVerStiff	*pVerStiff	= pCenCr->m_pVerStiff;
	CCrossBeamHorStiff	*pHorStiff	= pCenCr->m_pHorStiff;
	strText.Format("         熱霜爾鬼營蒂 儅楞й 熱 氈朝 Ы楠雖 牖除問曖 譆渠纂 [紫.撲 3.8.5.1]");		Arr.Add(strText+"");
	if(pVerStiff->m_dSpaceCoeff>1.2)
		strText.Format("          ∵(任a/任) = ∵( %.3f / %.3f ) = %.3f > 1.2  ╮ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear,	pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	else
		strText.Format("          ∵(任a/任) = ∵( %.3f / %.3f ) = %.3f < 1.2  ╮ %.1f", 
			pVerStiff->m_dAllowShear, pVerStiff->m_dShear, pVerStiff->m_dSpaceCoeff, pVerStiff->m_dSpaceCoeffUse);
	Arr.Add(strText+"");
	if(pVerStiff->m_dMaxSpace<pVerStiff->m_dHeightWeb)
		strText.Format("          %.0f ▼ tw = %.1f mm  <  bw = %.0f mm  熱霜爾鬼營 в蹂", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	else
		strText.Format("          %.0f ▼ tw = %.1f mm  >  bw = %.0f mm  熱霜爾鬼營 в蹂橈擠", 
			pVerStiff->m_dMaxSpaceCoeff, pVerStiff->m_dMaxSpace, pVerStiff->m_dHeightWeb);
	Arr.Add(strText+"");

	if(pVerStiff->m_bIsNeedVerStiff)	VerStiff(Arr, pVerStiff, pCenCr->m_nQtyHorStiff);
	if(pCenCr->m_nQtyHorStiff>0)		HorStiff(Arr, pHorStiff);
}

// 醞除 陛煎爾(V粽溯檜諒⑽ 顫殮)
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
	strText.Format("    < 醞除 熱霜粽溯檜諒(V⑽) - 剪渦 : %d    STA : %.0f >", nG+1, dStation);
	Arr.Add(strText+"");
	strText.Format("    ﹞h = %.1f mm    ﹞a = %.1f mm    ﹞亥 = %.3f﹉", pVBr->m_dHeight, pVBr->m_dGirderSpace, ToDegree(pVBr->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("     ﹞餌 辨 鬼 營");
	Arr.Add(strText+"");
	strText.Format("       忙式式式式式式成式式式式式式式式式式成式式式式式成式式式式式成式式式式式成式式式式式成式式式式式忖");	Arr.Add(strText+"");
	strText.Format("       弛   睡   營  弛    鬼 營 敘 問     弛 A (mm昌) 弛 乓x (mm) 弛 乓y (mm) 弛  乓min   弛 坐 (mm)  弛");	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式托式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛   鼻⑷營   弛 L - %3.0f X %3.0f X %2.0f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛", pVBr->m_pChordUp->m_dHeight, pVBr->m_pChordUp->m_dWidth,
		pVBr->m_pChordUp->m_dThick1, pVBr->m_pChordUp->m_dArea, pVBr->m_pChordUp->m_dGammaX, pVBr->m_pChordUp->m_dGammaY, pVBr->m_pChordUp->m_dGammaMin, pVBr->m_pChordUp->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式托式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛   餌  營   弛 L - %3.0f X %3.0f X %2.0f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛", pVBr->m_pDiagonal->m_dHeight, pVBr->m_pDiagonal->m_dWidth,
		pVBr->m_pChordUp->m_dThick1, pVBr->m_pDiagonal->m_dArea, pVBr->m_pDiagonal->m_dGammaX, pVBr->m_pDiagonal->m_dGammaY, pVBr->m_pDiagonal->m_dGammaMin, pVBr->m_pDiagonal->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       戍式式式式式式托式式式式式式式式式式托式式式式式托式式式式式托式式式式式托式式式式式托式式式式式扣");	Arr.Add(strText+"");
	strText.Format("       弛   ж⑷營   弛 L - %3.0f X %3.0f X %2.0f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛 %8.1f 弛", pVBr->m_pChordLo->m_dHeight, pVBr->m_pChordLo->m_dWidth,
		pVBr->m_pChordUp->m_dThick1, pVBr->m_pChordLo->m_dArea, pVBr->m_pChordLo->m_dGammaX, pVBr->m_pChordLo->m_dGammaY, pVBr->m_pChordLo->m_dGammaMin, pVBr->m_pChordLo->m_dLength);
	Arr.Add(strText+"");
	strText.Format("       戌式式式式式式扛式式式式式式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式扛式式式式式戎");	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    陛) ж 醞 骯 薑");														Arr.Add(strText+"");
	strText.Format("        Ёж醞縑 渠ж罹朝 2褻曖 醞除 熱霜粽溯檜諒檜 嫡朝 匙戲煎 ж貊,");	Arr.Add(strText+"");
	strText.Format("        雖霞ж醞縑 渠ж罹朝 輿⑽除問熱縑 п渡ж朝 熱霜粽溯檜諒檜 敕蛔ж啪 睡氬ж朝 匙戲煎 и棻.");Arr.Add(strText+"");
	strText.Format("      1) Ё ж 醞");		Arr.Add(strText+"");
	strText.Format("       - Ёж醞 鬼紫");		Arr.Add(strText+"");
	strText.Format("            忙式式式式式式式式式式成式式式式式式式式式式式式式式忖");		Arr.Add(strText+"");
	strText.Format("            弛     欽 賊 ⑽ 鼻    弛        Ё ж 醞 (kN/m)     弛");		Arr.Add(strText+"");
	strText.Format("            戍式式式式式式式式式式托式式式式式式式式式式式式式式扣");		Arr.Add(strText+"");
	strText.Format("            弛   1 ‾ B/D < 8     弛 [ 4.0 - 0.2(B/D) ] D ￣ 6  弛");		Arr.Add(strText+"");
	strText.Format("            戍式式式式式式式式式式托式式式式式式式式式式式式式式扣");		Arr.Add(strText+"");
	strText.Format("            弛      8 ‾ B/D      弛         2.4 D ￣ 6         弛");		Arr.Add(strText+"");
	strText.Format("            戌式式式式式式式式式式扛式式式式式式式式式式式式式式戎");		Arr.Add(strText+"");
	strText.Format("       - Ёж醞 鬼紫 啗骯");												Arr.Add(strText+"");
	strText.Format("         掖榆 ァ錳 (B) = %.3f m", toM(pVBr->m_strtWind.m_dWidthBridge));	Arr.Add(strText+"");
	strText.Format("         掖榆 堪檜 (D) = %.3f m", toM(pVBr->m_strtWind.m_dHeightBridge));
	Arr.Add(strText+"");
	strText.Format("           B / D = %.3f / %.3f = %.3f m",
		toM(pVBr->m_strtWind.m_dWidthBridge), toM(pVBr->m_strtWind.m_dHeightBridge), pVBr->m_strtWind.m_dRateWidthHeight);
	Arr.Add(strText+"");
	if(pVBr->m_strtWind.m_dRateWidthHeight>=1 && pVBr->m_strtWind.m_dRateWidthHeight<8)
		strText.Format("            1 ‾ B / D < 8  檜嘎煎  Ww = [ 4.0 - 0.2 ( B / D ) ] D = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWw));
	else if(pVBr->m_strtWind.m_dRateWidthHeight>=8)
		strText.Format("              8 ‾ B / D   檜嘎煎  Ww = 2.4 ﹞D = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWw));
	else
		strText.Format("              營匐饜 蹂蜂");
	Arr.Add(strText+"");
	if(tokN(pVBr->m_strtWind.m_dWw)>=6.0)
		strText.Format("              Ww ￣ 6.0 kN/m    檜嘎煎 ˍ Ww = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWwUse));
	else
		strText.Format("              Ww < 6.0 kN/m    檜嘎煎 ˍ Ww = %.3f kN/m", tokNPM(pVBr->m_strtWind.m_dWwUse));
	Arr.Add(strText+"");
	strText.Format("       - Ёж醞縑 曖и 粽溯檜諒 1褻曖 濛辨溘");
	Arr.Add(strText+"");
	strText.Format("         粽溯檜諒 除問 伙 = %.3f m", toM(pVBr->m_strtWind.m_dBracingSpace));
	Arr.Add(strText+"");
	strText.Format("         剪渦熱         n = %d", pDB->GetGirdersu());
	Arr.Add(strText+"");
	strText.Format("         Ёж醞 啗骯衛 ъ辨擬溘 й隸啗熱 : %.1f", pVBr->m_strtWind.m_dAllowCoeff);
	Arr.Add(strText+"");
	if(pDB->GetGirdersu()>=3)
		strText.Format("         Hw = 1/2﹞Ww﹞伙 / 1.2 = 1/2 ▼ %.3f ▼ %.3f / %.1f = %.3f kN",
		tokNPM(pVBr->m_strtWind.m_dWwUse), toM(pVBr->m_strtWind.m_dBracingSpace), pVBr->m_strtWind.m_dAllowCoeff, tokN(pVBr->m_strtWind.m_dHw));
	else
		strText.Format("         Hw = Ww﹞伙 / 1.2 = %.3f ▼ %.3f / %.1f = %.3f kN",
		tokNPM(pVBr->m_strtWind.m_dWwUse), toM(pVBr->m_strtWind.m_dBracingSpace), pVBr->m_strtWind.m_dAllowCoeff, tokN(pVBr->m_strtWind.m_dHw));
	Arr.Add(strText+"");
	strText.Format("      2) 雖霞 ж醞");			Arr.Add(strText+"");
	strText.Format("       - 欽嬪望檜渡 堅薑ж醞");	Arr.Add(strText+"");
	strText.Format("         Wd = 奩溘曖 識м啗 / 掖榆翱濰 = %.3f kN / %.3f m = %.3f kN/m",
		tokN(pVBr->m_strtEarthQuake.m_dReaction), toM(pVBr->m_strtEarthQuake.m_dLengthBridge), tokNPM(pVBr->m_strtEarthQuake.m_dWd));
	Arr.Add(strText+"");
	strText.Format("       - 雖霞 陛樓紫 啗熱(A)");
	Arr.Add(strText+"");
	if(nGradeEarth==0)	strText.Format("         頂霞蛔晝 : 弘蛔晝  ╮  嬪я紫 啗熱 : %.3f", dRiskCoefficient);
	else				strText.Format("         頂霞蛔晝 : 弗蛔晝  ╮  嬪я紫 啗熱 : %.3f", dRiskCoefficient);
	Arr.Add(strText+"");
	if(nRegionEarth==0)	strText.Format("         頂霞掘羲 : 弘掘羲  ╮  雖霞掘羲啗熱 : %.3f", dZoneCoefficient);
	else				strText.Format("         頂霞掘羲 : 弗掘羲  ╮  雖霞掘羲啗熱 : %.3f", dZoneCoefficient);
	Arr.Add(strText+"");
	strText.Format("         A = %.3f ▼ %.3f = %.3f", dRiskCoefficient, dZoneCoefficient, pVBr->m_strtEarthQuake.m_dAcceleration);
	Arr.Add(strText+"");
	strText.Format("       - 欽嬪望檜渡 雖霞ж醞");
	Arr.Add(strText+"");
	strText.Format("         We = A ▼ Wd = %.3f kN/m", tokNPM(pVBr->m_strtEarthQuake.m_dWe));
	Arr.Add(strText+"");
	strText.Format("       - 雖霞ж醞縑 曖и 粽溯檜諒 1褻曖 濛辨溘");
	Arr.Add(strText+"");
	strText.Format("         雖霞衛曖 ъ辨擬溘 й隸啗熱 : %.1f", pVBr->m_strtEarthQuake.m_dAllowCoeff);
	Arr.Add(strText+"");
	strText.Format("         He = 1/ (n-1)﹞We﹞伙 = %.3f kN", tokN(pVBr->m_strtEarthQuake.m_dHe));
	Arr.Add(strText+"");
	if(pVBr->m_strtWind.m_dHw>pVBr->m_strtEarthQuake.m_dHe)
		strText.Format("         Hw  >  He 檜嘎煎   H = %.3f kN 瞳辨", tokN(pVBr->m_strtEarthQuake.m_dH));
	else
		strText.Format("         Hw  <  He 檜嘎煎   H = %.3f kN 瞳辨", tokN(pVBr->m_strtEarthQuake.m_dH));
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    釭) 睡營溘 啗骯");
	Arr.Add(strText+"");
	strText.Format("      N1 = 3/4H    N2 = 1/4 H﹞sec玄    N3 = 1/2 H    sec玄 = %.3f", 1.0/cos(pVBr->m_dTheta));
	Arr.Add(strText+"");
	strText.Format("      1) 鼻⑷營(AB)");
	Arr.Add(strText+"");
	strText.Format("         N1 = -3/4H = %.3f kN (憶蹴)", tokN(pVBr->m_strtElementLoad.m_dN1));
	Arr.Add(strText+"");
	strText.Format("      2) 餌營(AD,BD)");
	Arr.Add(strText+"");
	strText.Format("         N2 = ▽ 1/4 H﹞sec玄 = ▽ %.3f kN (AD:檣濰, BD:憶蹴)", tokN(pVBr->m_strtElementLoad.m_dN2Tension));
	Arr.Add(strText+"");
	strText.Format("      3) ж⑷營(CD)");
	Arr.Add(strText+"");
	strText.Format("         N3 = - 1/2 H = %.3f kN (憶蹴)", tokN(pVBr->m_strtElementLoad.m_dN3));
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    棻) 睡營溘 啗骯");
	Arr.Add(strText+"");
	strText.Format("     1) 鼻 ⑷ 營    ( %s )", pVBr->m_strtChordUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        欽賊瞳                :     Ag = %.1f mm昌", pVBr->m_pChordUp->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳曖 譆模高 :  乓min = %.1f mm", pVBr->m_pChordUp->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳          :    乓x = %.1f mm", pVBr->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          函 撮濰綠 匐饜");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordUp.m_dSlendernessRatio<150.0)
		strText.Format("            坐/乓min = %.1f / %.1f = %.3f  < 150      .... O.K  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pVBr->m_pChordUp->m_dLength, pVBr->m_pChordUp->m_dGammaMin, pVBr->m_strtChordUp.m_dSlendernessRatio);
	else
		strText.Format("            坐/乓min = %.1f / %.1f = %.3f  > 150      .... N.G  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pVBr->m_pChordUp->m_dLength, pVBr->m_pChordUp->m_dGammaMin, pVBr->m_strtChordUp.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          刻 ъ辨擬溘     ( Ｃ 紫.撲.晦 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("           - ъ辨 蹴寞щ 憶蹴擬溘");
	Arr.Add(strText+"");
	strText.Format("            坐/乓x = %.1f / %.1f = %.3f", pVBr->m_pChordUp->m_dLength, pVBr->m_pChordUp->m_dGammaX,
		pVBr->m_pChordUp->m_dLength/pVBr->m_pChordUp->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("             fca = %.3f MPa", pVBr->m_strtChordUp.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE縑 翱唸腎橫 氈朝 唳辦縑朝 ъ辨憶蹴擬溘縑 盪馬睦擊 培ж罹 匐饜и棻.");
	Arr.Add(strText+"");
	strText.Format("             fcaㄖ=  fca ▼ ( 0.5 + ( 坐/乓x ) / 1000 )");
	Arr.Add(strText+"");
	strText.Format("                  = %.3f MPa", pVBr->m_strtChordUp.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("          券 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("              蹴寞щ憶蹴擬溘");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordUp.m_dFc<pVBr->m_strtChordUp.m_dFcaApo)
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  <  fca'    .... O.K",
			pVBr->m_strtElementLoad.m_dN1, pVBr->m_pChordUp->m_dArea, pVBr->m_strtChordUp.m_dFc);
	else
		strText.Format("              fc = N1 / Ag = %.3f / %.1f = %.3f MPa  >  fca'    .... N.G",
			pVBr->m_strtElementLoad.m_dN1, pVBr->m_pChordUp->m_dArea, pVBr->m_strtChordUp.m_dFc);
	Arr.Add(strText+"");
	strText.Format("          刷 Gusset Plate諦曖 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.3f MPa", pVBr->m_strtChordUp.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.3f mm", pVBr->m_strtChordUp.m_dLengthWeld/2, pVBr->m_strtChordUp.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾跡舒眷        :  a  = %.1f / ∵2 = %.3f mm", pVBr->m_strtChordUp.m_dThroatDepth, pVBr->m_strtChordUp.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtChordUp.m_dFc<0.75*pVBr->m_strtChordUp.m_dFcaApo)
	{
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜ж 檜嘎煎 賅營鬼紫曖 75%%蒂 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N1' = 0.75 ▼ %.1f ▼ %.3f = %.3f kN",
			pVBr->m_pChordUp->m_dArea, pVBr->m_strtChordUp.m_dFcaApo, tokN(pVBr->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜鼻 檜嘎煎 濛辨ж醞擊 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N1' = N1 = %.3f kN", tokN(pVBr->m_strtChordUp.m_dN1Apo));
		Arr.Add(strText+"");
	}
	if(pVBr->m_strtChordUp.m_dShear<pVBr->m_strtChordUp.m_dAllowShear)
		strText.Format("             任 = N1'/ a﹞甲L = %.3f MPa  <  任a    .... O.K", pVBr->m_strtChordUp.m_dShear);
	else
		strText.Format("             任 = N1'/ a﹞甲L = %.3f MPa  >  任a    .... N.G", pVBr->m_strtChordUp.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     2) 餌 營    ( %s )", pVBr->m_strtDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        欽賊瞳                :    Ag = %.1f mm昌", pVBr->m_pDiagonal->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳曖 譆模高 : 乓min = %.1f mm", pVBr->m_pDiagonal->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳          :  乓x  = %.1f mm", pVBr->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          函 撮濰綠 匐饜");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  <  150    .... O.K  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pVBr->m_pDiagonal->m_dLength, pVBr->m_pDiagonal->m_dGammaMin, pVBr->m_strtDiagonal.m_dSlendernessRatio);
	else
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  >  150    .... N.G  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pVBr->m_pDiagonal->m_dLength, pVBr->m_pDiagonal->m_dGammaMin, pVBr->m_strtDiagonal.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          刻 ъ辨擬溘    ( Ｃ 紫.撲.晦 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - ъ辨憶蹴擬溘");
	Arr.Add(strText+"");
	strText.Format("            坐/乓x = %.1f / %.1f = %.3f",
		pVBr->m_pDiagonal->m_dLength, pVBr->m_pDiagonal->m_dGammaX, pVBr->m_pDiagonal->m_dLength/pVBr->m_pDiagonal->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pVBr->m_strtDiagonal.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE縑 翱唸腎橫 氈朝 唳辦縑朝 ъ辨憶蹴擬溘縑 盪馬睦擊 培ж罹 匐饜и棻.");
	Arr.Add(strText+"");
	strText.Format("            fcaㄖ=  fca ▼ ( 0.5 + ( 坐/乓x ) / 1000 ) = %.1f MPa", pVBr->m_strtDiagonal.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("            - ъ辨檣濰擬溘");
	Arr.Add(strText+"");
	strText.Format("            fta = %.1f MPa", pVBr->m_strtDiagonal.m_dFta);
	Arr.Add(strText+"");
	strText.Format("          券 擬溘匐饜");
	Arr.Add(strText+"");
	if(pVBr->m_pDiagonal->m_nType==CSteelMaterialData::CTSTEEL)
		strText.Format("            檣濰 L⑽鬼曖 嶸�艘僄橉�     (紫撲 3.4.5)");
	else
		strText.Format("            檣濰 T⑽鬼曖 嶸�艘僄橉�     (紫撲 3.4.5)");
	Arr.Add(strText+"");
	strText.Format("            An = %.1f mm昌", pVBr->m_strtDiagonal.m_dAn);
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
	strText.Format("          刷 Gusset Plate諦曖 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.1f MPa ( Ｃ 紫.撲.晦 3.3.2.3 )", pVBr->m_strtDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.1f mm", pVBr->m_strtDiagonal.m_dLengthWeld/2.0, pVBr->m_strtDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾跡舒眷        :  a  = %.1f / ∵2 = %.1f mm", pVBr->m_strtDiagonal.m_dThroatDepth, pVBr->m_strtDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtDiagonal.m_dFc<0.75*pVBr->m_strtDiagonal.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 檜嘎煎", pVBr->m_strtDiagonal.m_dFc, 0.75*pVBr->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜ж 檜嘎煎 賅營鬼紫曖 75%%蒂 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N2' = 0.75 ▼ %.1f ▼ %.1f = %.3f kN",
			pVBr->m_pDiagonal->m_dArea, pVBr->m_strtDiagonal.m_dFcaApo, tokN(pVBr->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 檜嘎煎", pVBr->m_strtDiagonal.m_dFc, 0.75*pVBr->m_strtDiagonal.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜鼻 檜嘎煎 濛辨ж醞擊 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N2' = N2 = %.3f kN", tokN(pVBr->m_strtDiagonal.m_dN2Apo));
		Arr.Add(strText+"");
	}
	if(pVBr->m_strtDiagonal.m_dShear<pVBr->m_strtDiagonal.m_dAllowShear)
		strText.Format("             任 = N2'/ a﹞甲L = %.1f MPa  <  任a    .... O.K", pVBr->m_strtDiagonal.m_dShear);
	else
		strText.Format("             任 = N2'/ a﹞甲L = %.1f MPa  >  任a    .... N.G", pVBr->m_strtDiagonal.m_dShear);
	Arr.Add(strText+"");
	strText.Format("     3) ж ⑷ 營    ( %s )", pVBr->m_strtChordLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("        欽賊瞳                :    Ag = %.1f mm昌", pVBr->m_pChordLo->m_dArea);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳曖 譆模高 : 乓min = %.1f mm", pVBr->m_pChordLo->m_dGammaMin);
	Arr.Add(strText+"");
	strText.Format("        欽賊2離 奩唳          :  乓x  = %.1f mm", pVBr->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("          函 撮濰綠 匐饜");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dSlendernessRatio<150.0)
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  <  150    .... O.K  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pVBr->m_pChordLo->m_dLength, pVBr->m_pChordLo->m_dGammaMin, pVBr->m_strtChordLo.m_dSlendernessRatio);
	else
		strText.Format("             坐/乓min = %.1f / %.1f = %.3f  >  150    .... N.G  ( Ｃ 紫.撲.晦 3.4.1.4 )",
			pVBr->m_pChordLo->m_dLength, pVBr->m_pChordLo->m_dGammaMin, pVBr->m_strtChordLo.m_dSlendernessRatio);
	Arr.Add(strText+"");
	strText.Format("          刻 ъ辨擬溘    ( Ｃ 紫.撲.晦 3.3.2.1 )");
	Arr.Add(strText+"");
	strText.Format("             - ъ辨憶蹴擬溘");
	Arr.Add(strText+"");
	strText.Format("            坐/乓x = %.1f / %.1f = %.3f",
		pVBr->m_pChordLo->m_dLength, pVBr->m_pChordLo->m_dGammaX, pVBr->m_pChordLo->m_dLength/pVBr->m_pChordLo->m_dGammaX);
	Arr.Add(strText+"");
	strText.Format("            fca = %.1f MPa", pVBr->m_strtChordLo.m_dFca);
	Arr.Add(strText+"");
	strText.Format("            GUSSET PLATE縑 翱唸腎橫 氈朝 唳辦縑朝 ъ辨憶蹴擬溘縑 盪馬睦擊 培ж罹 匐饜и棻.");
	Arr.Add(strText+"");
	strText.Format("            fcaㄖ=  fca ▼ ( 0.5 + ( 坐/乓x ) / 1000 ) = %.1f MPa", pVBr->m_strtChordLo.m_dFcaApo);
	Arr.Add(strText+"");
	strText.Format("          券 擬溘匐饜");
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dFc<pVBr->m_strtChordLo.m_dFcaApo)
		strText.Format("                fc = N3 / An = %.1f / %.1f = %.3f MPa  <  fca'    .... O.K",
		pVBr->m_strtElementLoad.m_dN3, pVBr->m_pChordLo->m_dArea, pVBr->m_strtChordLo.m_dFc);
	else
		strText.Format("                fc = N3 / An = %.1f / %.1f = %.3f MPa  >  fca'    .... N.G",
		pVBr->m_strtElementLoad.m_dN3, pVBr->m_pChordLo->m_dArea, pVBr->m_strtChordLo.m_dFc);
	Arr.Add(strText+"");
	strText.Format("          刷 Gusset Plate諦曖 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.1f MPa    ( Ｃ 紫.撲.晦 3.3.2.3 )", pVBr->m_strtChordLo.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.1f mm", pVBr->m_strtChordLo.m_dLengthWeld/2, pVBr->m_strtChordLo.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("             Fillet 辨蕾跡舒眷        :   a = %.1f / ∵2 = %.1f mm", pVBr->m_strtChordLo.m_dThroatDepth, pVBr->m_strtChordLo.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtChordLo.m_dFc<0.75*pVBr->m_strtChordLo.m_dFcaApo)
	{
		strText.Format("             fc = %.1f MPa  >  0.75fca' = %.1f MPa 檜嘎煎", pVBr->m_strtChordLo.m_dFc, 0.75*pVBr->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜ж 檜嘎煎 賅營鬼紫曖 75%%蒂 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N3' = 0.75 ▼ %.1f ▼ %.1f = %.3f kN",
			pVBr->m_pChordLo->m_dArea, pVBr->m_strtChordLo.m_dFcaApo, tokN(pVBr->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	else
	{
		strText.Format("             fc = %.1f MPa  <  0.75fca' = %.1f MPa 檜嘎煎", pVBr->m_strtChordLo.m_dFc, 0.75*pVBr->m_strtChordLo.m_dFcaApo);
		Arr.Add(strText+"");
		strText.Format("             濛辨擬溘檜 賅營 瞪鬼紫縑 75%%檜鼻 檜嘎煎 濛辨ж醞擊 瞳辨и棻.");
		Arr.Add(strText+"");
		strText.Format("             N3' = N3 = %.3f kN", tokN(pVBr->m_strtChordLo.m_dN3Apo));
		Arr.Add(strText+"");
	}
	if(pVBr->m_strtChordLo.m_dShear<pVBr->m_strtChordLo.m_dAllowShear)
		strText.Format("             任 = N3'/ a﹞甲L = %.1f MPa  <  任a    .... O.K", pVBr->m_strtChordLo.m_dShear);
	else
		strText.Format("             任 = N3'/ a﹞甲L = %.1f MPa  >  任a    .... N.G", pVBr->m_strtChordLo.m_dShear);
	Arr.Add(strText+"");Arr.Add("");
	strText.Format("    棻) Gusset Plate曖 匐饜");
	Arr.Add(strText+"");
	strText.Format("        濛辨ж醞婁 賅營鬼紫曖 75%%蒂 堅溥ж罹 撲啗и棻.");
	Arr.Add(strText+"");
	strText.Format("      1) 鼻⑷營/餌營 翱唸 (A,B)");
	Arr.Add(strText+"");
	strText.Format("       ﹞ 餌辨鬼營           : %.0f ▼ %.0f ▼ %.0f ( %s )",
		pVBr->m_strtGussetUp.m_dHeight, pVBr->m_strtGussetUp.m_dThick, pVBr->m_strtGussetUp.m_dWidth, pVBr->m_strtGussetUp.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 鬼營曖 ъ辨擬溘    : fa = %.1f MPa", pVBr->m_strtGussetUp.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 濛辨ж醞");
	Arr.Add(strText+"");
	strText.Format("          Rh = N1' + N2'cos玄 = %.3f + %.3f ▼ cos玄 = %.3f kN",
		tokN(pVBr->m_strtChordUp.m_dN1Apo), tokN(pVBr->m_strtDiagonal.m_dN2Apo), tokN(pVBr->m_strtGussetUp.m_dRh));
	Arr.Add(strText+"");
	strText.Format("          Rv = N2'sin玄 = %.3f ▼ sin玄 = %.3f kN", tokN(pVBr->m_strtDiagonal.m_dN2Apo), tokN(pVBr->m_strtGussetUp.m_dRv));
	Arr.Add(strText+"");
	strText.Format("          P = ∵( Rh2 + Rv2 ) = %.3f kN", tokN(pVBr->m_strtGussetUp.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       ﹞ 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1賊葆雞)   伏a = %.1f kN", pVBr->m_strtGussetUp.m_strBoltMaterial, pVBr->m_strtGussetUp.m_dDiaBolt, tokN(pVBr->m_strtGussetUp.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          в蹂 BOLT熱");
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetUp.m_dQtyBoltUse>pVBr->m_strtGussetUp.m_dQtyBoltReq)
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... O.K",
		tokN(pVBr->m_strtGussetUp.m_dLoad), tokN(pVBr->m_strtGussetUp.m_dBoltAllowStress), pVBr->m_strtGussetUp.m_dQtyBoltReq, pVBr->m_strtGussetUp.m_dQtyBoltUse);
	else
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... N.G",
		tokN(pVBr->m_strtGussetUp.m_dLoad), tokN(pVBr->m_strtGussetUp.m_dBoltAllowStress), pVBr->m_strtGussetUp.m_dQtyBoltReq, pVBr->m_strtGussetUp.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("     ﹞ Gusset Plate曖 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("          An = ( %.0f - %.0f ▼ %.0f ) ▼ %.0f = %.1f mm昌",
		pVBr->m_strtGussetUp.m_dHeight, pVBr->m_strtGussetUp.m_dQtyBoltUse, pVBr->m_strtGussetUp.m_dDiaBolt+3.0, pVBr->m_strtGussetUp.m_dThick, pVBr->m_strtGussetUp.m_dAn);
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetUp.m_dStress<pVBr->m_strtGussetUp.m_dAllowStress)
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pVBr->m_strtGussetUp.m_dLoad, pVBr->m_strtGussetUp.m_dAn, pVBr->m_strtGussetUp.m_dStress);
	else
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pVBr->m_strtGussetUp.m_dLoad, pVBr->m_strtGussetUp.m_dAn, pVBr->m_strtGussetUp.m_dStress);
	Arr.Add(strText+"");
	strText.Format("      2) ж⑷營 翱唸 (C,E)");
	Arr.Add(strText+"");
	strText.Format("       ﹞ 餌辨鬼營           : %.0f ▼ %.0f ▼ %.0f ( %s )",
		pVBr->m_strtGussetLo.m_dHeight, pVBr->m_strtGussetLo.m_dThick, pVBr->m_strtGussetLo.m_dWidth, pVBr->m_strtGussetLo.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 鬼營曖 ъ辨擬溘    : fa = %.1f MPa", pVBr->m_strtGussetLo.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 濛辨ж醞");
	Arr.Add(strText+"");
	strText.Format("          P = N3' = %.3f kN", tokN(pVBr->m_strtGussetLo.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       ﹞ 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("          BOLT  %s  M%.0f  (1賊葆雞)   伏a = %.1f kN", pVBr->m_strtGussetLo.m_strBoltMaterial, pVBr->m_strtGussetLo.m_dDiaBolt, tokN(pVBr->m_strtGussetLo.m_dBoltAllowStress));
	Arr.Add(strText+"");
	strText.Format("          в蹂 BOLT熱");
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetLo.m_dQtyBoltUse>pVBr->m_strtGussetLo.m_dQtyBoltReq)
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... O.K",
		tokN(pVBr->m_strtGussetLo.m_dLoad), tokN(pVBr->m_strtGussetLo.m_dBoltAllowStress), pVBr->m_strtGussetLo.m_dQtyBoltReq, pVBr->m_strtGussetLo.m_dQtyBoltUse);
	else
		strText.Format("          n = P / 伏a = %.3f / %.1f = %.1f EA ⊥ %.0f 偃餌辨    .... N.G",
		tokN(pVBr->m_strtGussetLo.m_dLoad), tokN(pVBr->m_strtGussetLo.m_dBoltAllowStress), pVBr->m_strtGussetLo.m_dQtyBoltReq, pVBr->m_strtGussetLo.m_dQtyBoltUse);
	Arr.Add(strText+"");
	strText.Format("       ﹞ Gusset Plate曖 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("          An = ( %.0f - %.0f ▼ %.0f ) ▼ %.0f = %.1f mm昌",
		pVBr->m_strtGussetLo.m_dHeight, pVBr->m_strtGussetLo.m_dQtyBoltUse, pVBr->m_strtGussetLo.m_dDiaBolt+3.0, pVBr->m_strtGussetLo.m_dThick, pVBr->m_strtGussetLo.m_dAn);
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetLo.m_dStress<pVBr->m_strtGussetLo.m_dAllowStress)
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  <  fa    .... O.K", pVBr->m_strtGussetLo.m_dLoad, pVBr->m_strtGussetLo.m_dAn, pVBr->m_strtGussetLo.m_dStress);
	else
		strText.Format("         f = P / An = %.3f / %.1f = %.1f MPa  >  fa    .... N.G", pVBr->m_strtGussetLo.m_dLoad, pVBr->m_strtGussetLo.m_dAn, pVBr->m_strtGussetLo.m_dStress);
	Arr.Add(strText+"");
	strText.Format("      3) 餌營/ж⑷營 翱唸 (D)");
	Arr.Add(strText+"");
	strText.Format("       ﹞ 餌辨鬼營           : %.0f ▼ %.0f ▼ %.0f ( %s )",
		pVBr->m_strtGussetDiagonal.m_dHeight, pVBr->m_strtGussetDiagonal.m_dThick, pVBr->m_strtGussetDiagonal.m_dWidth, pVBr->m_strtGussetDiagonal.m_strMaterial);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 鬼營曖 ъ辨擬溘    : fa = %.1f MPa", pVBr->m_strtGussetDiagonal.m_dAllowStress);
	Arr.Add(strText+"");
	strText.Format("       ﹞ 濛辨ж醞");
	Arr.Add(strText+"");
	strText.Format("          P = N3' = %.3f kN", tokN(pVBr->m_strtGussetDiagonal.m_dLoad));
	Arr.Add(strText+"");
	strText.Format("       ﹞ 翱唸睡 匐饜");
	Arr.Add(strText+"");
	strText.Format("          Fillet 辨蕾睡曖 ъ辨擬溘 : 任a = %.3f MPa", pVBr->m_strtGussetDiagonal.m_dAllowShear);
	Arr.Add(strText+"");
	strText.Format("          Fillet 辨蕾陝濰          : 甲L = %.1f ▼ 2EA = %.1f mm", pVBr->m_strtGussetDiagonal.m_dLengthWeld/2.0, pVBr->m_strtGussetDiagonal.m_dLengthWeld);
	Arr.Add(strText+"");
	strText.Format("          Fillet 辨蕾跡舒眷        :  a  = %.1f / ∵2 = %.3f mm", pVBr->m_strtGussetDiagonal.m_dThroatDepth, pVBr->m_strtGussetDiagonal.m_dThroatDepth/sqrt(2.0));
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetDiagonal.m_dShearWeld<pVBr->m_strtGussetDiagonal.m_dAllowShearWeld)
		strText.Format("          任 = P/ a﹞甲L = %.3f MPa  <  任a    .... O.K", pVBr->m_strtGussetDiagonal.m_dShearWeld);
	else
		strText.Format("          任 = P/ a﹞甲L = %.3f MPa  >  任a    .... N.G", pVBr->m_strtGussetDiagonal.m_dShearWeld);
	Arr.Add(strText+"");
	strText.Format("       ﹞ Gusset Plate曖 擬溘匐饜");
	Arr.Add(strText+"");
	strText.Format("           Ag = %.0f ▼ %.0f = %.1f mm昌", pVBr->m_strtGussetDiagonal.m_dWidth, pVBr->m_strtGussetDiagonal.m_dThick, pVBr->m_strtGussetDiagonal.m_dAg);
	Arr.Add(strText+"");
	if(pVBr->m_strtGussetDiagonal.m_dShear<pVBr->m_strtGussetDiagonal.m_dAllowShear)
		strText.Format("           任 = P / Ag = %.3f MPa  <  任a    .... O.K", pVBr->m_strtGussetDiagonal.m_dShear);
	else
		strText.Format("           任 = P / Ag = %.3f MPa  >  任a    .... N.G", pVBr->m_strtGussetDiagonal.m_dShear);
	Arr.Add(strText+"");Arr.Add("");
}
///////////////////////////////////////////////////////////////////////////////////
//♁ˍ♂２３４之 民氐永汁汀氾犯丟乒乓乩亙交亦甲亥仿玉伉伙伊伕伍伐休伏f 任仰仳份企
//	╞‾ˇˉ℅▽‾￣＿÷▼妖昂 夾 妓 妖 kgf/妓 t﹞m  易昌昆昂昀昏昕昊
//  函 刻 券 刷 刺 到 刮 制 剁 劾 劻 卒 協 卓 卑 ∵卦卷阮阱阪阬並乖乳Ｃ白
//  收收收收收收收收收收收收收  ╮ Ⅸ
//  旨 曲 旭 旬早收 曳 朽央 失 奴 奶 孕 它 尼 巨 巧 左 弘 弗 必 戊 戊 打 扔
///////////////////////////////////////////////////////////////////////////////////// 
// 欽賊(陛煎爾, 熱霜粽溯檜諒) 匐饜
void CPlateStressOut::SectionCheckOutput(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map)
{
	CSection	*pSection	= pBx->GetSection();

	if(pSection->IsenType(SECTION_A))
	{
		if(pSection->IsenDetType(SECDET_CROSSBEAM))
			StressCheckEndCrossBeamGeneralType(Arr, pBx, Map);	// 欽睡 陛煎爾(薯濛顫殮, H⑽鬼顫殮)
		else if(pSection->IsenDetType(SECDET_VBRACING))
			StressCheckEndCrossBeamVBracingType(Arr, pBx, Map);	// 欽睡 羲V粽溯檜諒⑽ 顫殮
	}
	else
	{
		if(pSection->IsenDetType(SECDET_CROSSBEAM))
			StressCheckCenterCrossBeamGeneralType(Arr, pBx, Map);	// 醞除 陛煎爾(薯濛顫殮, H⑽鬼顫殮)
		else if(pSection->IsenDetType(SECDET_VBRACING))
			StressCheckCenterCrossBeamVBracingType(Arr, pBx, Map);	// 醞除 V粽溯檜諒⑽ 顫殮
	}
/*
	if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))	// 熱霜粽溯檜諒
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
		StressCheckCrossBeam(Arr, pBx, Map);	// 欽睡 陛煎爾(薯濛顫殮, H⑽鬼顫殮)
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
	szText.Format("  < 熱霜 爾鬼營 匐饜  %s %s %s >", sLeft, sNum, sRight);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("   ﹞熱ゎ爾鬼營曖 餌辨 欽熱    :   n = %d", nNr);
	Arr.Add(szText+"");
	szText.Format("   ﹞犒睡っ曖 翱撰嶸憶蹴擬溘   :   f = %.3f MPa", dMax);
	Arr.Add(szText+"");
	szText.Format("   ﹞犒睡っ曖 瞪欽擬溘         :  任 = %.3f MPa", dTau);
	Arr.Add(szText+"");
	szText.Format("   ﹞餌辨熱霜爾鬼營 ァ         : bvs = %.1f mm", dVStiffW);
	Arr.Add(szText+"");
	szText.Format("   ﹞餌辨 熱霜爾鬼營曖 舒眷    : tvs = %.1f mm", dVStiffT);
	Arr.Add(szText+"");
	szText.Format("   ﹞熱霜爾鬼營曖 除問         :   a = %.1f mm", dGap);
	Arr.Add(szText+"");
	szText.Format("   ﹞鼻ж 曄Ы楠雖曖 牖除問    :   b = %.1f mm", dVStiffH);
	Arr.Add(szText+"");
	szText.Format("   ﹞犒睡っ曖 舒眷             :   t = %.1f mm", dTw);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("  函 熱霜爾鬼營曖 餌辨罹睡 匐饜");
	Arr.Add(szText+"");

	CString strWeb = pDB->GetMaterial(G_W, pBx);
	CSteel *pSteelCtl = m_pStd->m_pDataManage->GetSteel();
	long nIndexSteel = pSteelCtl->GetIndex(strWeb);
	double dTerm = pSteelCtl->GetSteelAllowValue(ALLOW_VSTIFFNOT, nIndexSteel,	THICK40BELOW);

// 	double dTerm = 0;
// 	if(sNum == _T("400"))		dTerm = 70;
// 	else if(sNum == _T("490"))	dTerm = 60;
// 	else						dTerm = 57;

	szText.Format("   ﹞熱霜爾鬼營蒂 儅楞й 熱 氈朝 Ы楠雖 牖除問 : %.0ft", dTerm);
	Arr.Add(szText+"");
	szSign	= dTerm*dTw < dVStiffH ? _T("<")	: _T(">");
	szCheck	= dTerm*dTw < dVStiffH ? _T("ˍ 熱霜爾鬼營в蹂!")	: _T("ˍ 熱霜爾鬼營в蹂橈擠!");
	szText.Format("      %.0f ▼ t = %.1f mm  %s  b = %.1f mm", dTerm, dTerm*dTw, szSign, dVStiffH);
	Arr.Add(szText+"");
	szText.Format("      %s", szCheck);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("  刻 熱霜爾鬼營曖 除問匐饜            ( Ｃ 紫.撲.晦 3.8.5.1 )");
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
			szText.Format("   ﹞熱ゎ爾鬼營蒂 %d欽戲煎ж堅 a/b > 1 檜嘎煎", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)昂[(f/3650)昌+ { 任 / 810+610(b/a)昌}昌] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
		}
		else
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/3650,2)+pow(dTau/(610+810*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ﹞熱ゎ爾鬼營蒂 %d欽戲煎ж堅 a/b ‾ 1 檜嘎煎", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)昂[(f/3650)昌+ { 任 / 610+810(b/a)昌}昌] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
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
			szText.Format("   ﹞熱ゎ爾鬼營蒂 %d欽戲煎ж堅 ˇ 0.80 檜嘎煎", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)昂[(f/9500)昌+ { 任 / 1270+610(b/a)昌}昌] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
		}
		else
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/9500,2)+pow(dTau/(950+810*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ﹞熱ゎ爾鬼營蒂 %d欽戲煎ж堅 a/b ‾ 0.80 檜嘎煎", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)昂[(f/9500)昌+ { 任 / 950+810(b/a)昌}昌] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
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
			szText.Format("   ﹞熱ゎ爾鬼營蒂 %d欽戲煎ж堅 ˇ 0.64 檜嘎煎", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)昂[(f/31500)昌+ { 任 / 1970+610(b/a)昌}昌] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
			
		}
		else
		{
			dTemp1 = pow(dVStiffH/(100*dTw), 4) * (pow(dMax/31500,2)+pow(dTau/(1480+810*pow(dVStiffH/dGap,2)),2));
			szSign	= dTemp1 < 1 ? _T("<") : _T(">");
			szCheck	= dTemp1 < 1 ? _T("O.K") : _T("N.G");
			szText.Format("   ﹞熱ゎ爾鬼營蒂 %d欽戲煎ж堅 a/b ‾ 0.64 檜嘎煎", nNr);
			Arr.Add(szText+"");
			szText.Format("     (b/100t)昂[(f/31500)昌+ { 任 / 1480+810(b/a)昌}昌] = %.3f %s 1  ===> %s", dTemp1, szSign, szCheck);
			Arr.Add(szText+"");
			
		}		
	}
	Arr.Add("");
	szText.Format("  券 熱霜爾鬼營曖 鬼紫 匐饜           ( Ｃ 紫.撲.晦 3.8.5.2 )");
	Arr.Add(szText+"");
	dTemp1 = 8.0*pow(dVStiffH/dGap, 2);
	double dIreq = dVStiffH*pow(dTw,3)/10.92*dTemp1;
	szText.Format("        乓 = 8.0 ▼ ( b / a )昌= 8.0 ▼ ( %.3f / %.3f )昌 = %.3f", dVStiffH, dGap, dTemp1);
	Arr.Add(szText+"");
	szText.Format("                   bt昆                  %3.0f ▼ %1.1f昆", dVStiffH, dTw);
	Arr.Add(szText+"");
	szText.Format("    I(req) =  收收收收收收 ▼ 乓 = 收收收收收收收收收收收 ▼ %.3f = %.3f mm昂", dTemp1, dIreq);
	Arr.Add(szText+"");
	szText.Format("                  10.92                     10.92");
	Arr.Add(szText+"");
	double dIused = 1.0/3.0 * dVStiffT * pow(dVStiffW,3);
	szSign	= dIused > dIreq ? _T(">")		: _T("<");
	szCheck	= dIused > dIreq ? _T("O.K")	: _T("N.G");
	szText.Format("   I(used) = 卷 ▼ %.3f ▼ %.3f昆= %.3f mm昂 %s  I(req)   ===> %s", dVStiffT, dVStiffW, dIused, szSign, szCheck);
	Arr.Add(szText+"");
	Arr.Add("");
	szText.Format("  刷 熱霜爾鬼營 給轎陝曖 譆模ァ");
	Arr.Add(szText+"");
	double dBmin = dVStiffH/30+5;
	szSign	= dBmin < dVStiffW ? _T("<")	: _T(">");
	szCheck	= dBmin < dVStiffW ? _T("O.K")	: _T("N.G");
	szText.Format("      bmin = %.3f / 30 + 5 = %.3f mm   %s   bvs = %.3f mm   ===> %s", dVStiffH, dBmin, szSign, dVStiffW, szCheck);
	Arr.Add(szText+"");
	Arr.Add("");

	szText.Format("  刺 給轎陝曖 譆模ァ");
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
				///< в滇 辨蕾 纂熱 匐饜.	
		CMap <CString, LPCTSTR, double, double> Map;
		DesignCalc.GetMapWeldReview(nType, Map);

		double Smax  = Map["Smax"];///< 譆渠瞪欽溘(kN->N)
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

		sText.Format("[Ы楠雖諦 犒睡っ曖 辨蕾睡 匐饜]\n\n");
		RichEdit.AddMsg(sText, RE_BLACK, FALSE, TRUE);

		sText.Format("  * 欽賊廓�� : 欽賊 - %d              瞪欽溘 S(kN) : %.3f\n", nType+1, tokN(Smax));
		RichEdit.AddText(sText);
		sText.Format("  * 欽賊薯錳\n");
		RichEdit.AddText(sText);
		sText.Format("    - 鼻っ 舒眷(mm)                : %.0f\n", Tfu);
		RichEdit.AddText(sText);
		sText.Format("    - жっ 舒眷(mm)                : %.0f\n", Tfl);
		RichEdit.AddText(sText);
		sText.Format("    - 犒睡っ 舒眷(mm)             : %.0f\n", Tw);
		RichEdit.AddText(sText);

		sText.Format("    - 鼻睡 Yvsu(mm)               : %.1f\n", Yvsu);
		RichEdit.AddText(sText);
		sText.Format("    - ж睡 Yvsl(mm)                : %.1f\n", Yvsl);
		RichEdit.AddText(sText);
		sText.Format("    - 欽賊2離 賅詮お Iv (mm昂) : %.3f\n", Iv);
		RichEdit.AddText(sText);

		sText.Format("    - 鼻睡 辨蕾 跡舒眷 (mm)     : %.0f\n", Su);
		RichEdit.AddText(sText);
		sText.Format("    - ж睡 辨蕾 跡舒眷 (mm)     : %.0f\n", Sl);
		RichEdit.AddText(sText);
		sText.Format("    - 鼻睡 瞪欽擬溘 任 (MPa)     : %.1f\n", Tu);
		RichEdit.AddText(sText);
		sText.Format("    - ж睡 瞪欽擬溘 任 (MPa)     : %.1f\n", Tl);
		RichEdit.AddText(sText);
		
		RichEdit.AddText("\n");

		sText.Format("  函 в滇辨蕾 纂熱匐饜\n");
		RichEdit.AddText(sText);

		nColor = RE_RED;
		if (Su < Sumin)
		{
			sCheck1.Format("    >> 鼻睡 :  Suse (%g mm)  <  Smin (%.1f mm)  ", Su, Sumin);
			sText.Format("		...N.G\n");
		}
		else if (Su > Sumax)
		{
			sCheck1.Format("    >> 鼻睡 :  Suse (%g mm)  >  Smax (%.1f mm)  ", Su, Sumax);
			sText.Format("		...N.G\n");
		}
		else
		{
			sCheck1.Format("    >> 鼻睡 :  Smin (%.1f mm)  ‾  Suse (%g mm)  ‾  Smax (%.1f mm)  ", Sumin, Su, Sumax);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck1);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);

		nColor = RE_RED;
		if (Sl < Slmin)
		{
			sCheck2.Format("    >> ж睡 :  Suse (%g mm)  <  Smin (%.1f mm)  ", Sl, Slmin);
			sText.Format("		...N.G\n");
		}
		else if (Sl > Slmax)
		{
			sCheck2.Format("    >> ж睡 :  Suse (%g mm)  >  Smax (%.1f mm)  ", Sl, Slmax);
			sText.Format("		...N.G\n");
		}
		else
		{
			sCheck2.Format("    >> ж睡 :  Smin (%.1f mm)  ‾  Suse (%g mm)  ‾  Smax (%.1f mm)  ", Slmin, Sl, Slmax);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck2);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);

		RichEdit.AddText("\n");
		sText.Format("  刻 辨蕾睡曖 擬溘匐饜\n");
		RichEdit.AddText(sText);
		
		if (Tu > Tau)
		{
			sCheck3.Format("    >> 鼻睡 :  瞪欽擬溘 任 (%.1f MPa )  >  ъ辨 瞪欽擬溘 任a (%g MPa)  ", Tu, Tau);
			sText.Format("		...N.G\n");
			nColor = RE_RED;
		}
		else
		{
			sCheck3.Format("    >> 鼻睡 :  瞪欽擬溘 任 (%.1f MPa )  <  ъ辨 瞪欽擬溘 任a (%g MPa)  ", Tu, Tau);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck3);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);

		if (Tl > Tal)
		{
			sCheck4.Format("    >> ж睡 :  瞪欽擬溘 任 (%.1f MPa )  >  ъ辨 瞪欽擬溘 任a (%g MPa)  ", Tl, Tal);
			sText.Format("		...N.G\n");
			nColor = RE_RED;
		}
		else
		{
			sCheck4.Format("    >> ж睡 :  瞪欽擬溘 任 (%.1f MPa )  <  ъ辨 瞪欽擬溘 任a (%g MPa)  ", Tl, Tal);
			sText.Format("		...O.K\n");
			nColor = RE_BLUE;
		}
		RichEdit.AddText(sCheck4);
		RichEdit.AddMsg(sText, nColor, FALSE, TRUE);
	}
	else
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////
		// 雖薄爾鬼營 辨蕾 匐饜
		/////////////////////////////////////////////////////////////////////////////////////////////////		
		sText.Format("[雖薄爾鬼營 辨蕾睡 匐饜]\n\n");
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
			Ws			= JijumMap["Ws"];		// 雖薄 爾鬼營 ァ
			Ts			= JijumMap["Ts"];		// 雖薄 爾鬼營 舒眷
			Ls			= JijumMap["Ls"];		// 雖薄 爾鬼營 望檜 (犒睡っ 堪檜)
			Nstiff		= (long)JijumMap["Nstiff"];	// 雖薄 爾鬼營 偃熱
			Tau			= JijumMap["Tau"];		// 瞪欽擬溘		
			Ta			= JijumMap["Ta"];		// ъ辨瞪欽 擬溘

			sPlate.Format ("%d - PL %g x %g x %g", Nstiff, Ws, Ts, Ls);
			
			if (i == 0)// 掖渠睡		
				sText.Format("  %d) 掖渠睡\n", nIdx++);
			else
			{
				RichEdit.AddText("\n");
				sText.Format("  %d) 掖陝睡\n", nIdx);
			}

			RichEdit.AddText(sText);

			sText.Format("    - 奩溘 R(kN) : %.3f\n", dRectionMax);
			RichEdit.AddText(sText);
			sText.Format("    - 餌辨爾鬼營 : %s\n", sPlate);
			RichEdit.AddText(sText);
			sText.Format("    - 瞪欽擬溘 任 (MPa) : %.3f\n", Tau);
			RichEdit.AddText(sText);

			RichEdit.AddText("\n");

			if (Tau <= Ta)
			{
				sCheck1 = "    >> 瞪欽擬溘 (任)  <  ъ辨 瞪欽擬溘 (任a)";
				RichEdit.AddText(sCheck1);
				sCheck2 = "			...O.K\n";
				RichEdit.AddMsg(sCheck2, RE_BLUE, FALSE, TRUE);
			}
			else
			{
				sCheck1 = "    >> 瞪欽擬溘 (任)  >  ъ辨 瞪欽擬溘 (任a)";
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
//♁ˍ♂２３４之 民氐永汁汀氾犯丟乒乓乩亙交亦甲亥仿玉伉伙伊伕伍伐休伏f 任仰仳份企
//	╞‾ˇˉ℅▽‾￣＿÷▼妖昂 夾 妓 妖 kgf/妓 t﹞m  易昌昆昂昀昏昕昊
//  函 刻 券 刷 刺 到 刮 制 剁 劾 劻 卒 協 卓 卑 ∵卦卷阮阱阪阬並乖乳Ｃ
//  收收收收收收收收收收收收收  ╮ Ⅸ
//  旨 曲 旭 旬早收 曳 朽央 失 奴 奶 孕 它 尼 巨 巧 左 弘 弗 必 戊 戊 打 扔
///////////////////////////////////////////////////////////////////////////////////// 