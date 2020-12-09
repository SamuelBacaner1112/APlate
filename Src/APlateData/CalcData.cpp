// CalcData.cpp: implementation of the CCalcData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalcData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHLLGuardFence::CHLLGuardFence()
{
	m_nGuardIndex	= 0;
	m_dSttHDanPos	= 0;
	m_dEndHDanPos	= 0;
}

CHLLGuardFence::CHLLGuardFence(const CHLLGuardFence& rhs)
{
	m_nGuardIndex	= rhs.m_nGuardIndex;
	m_dSttHDanPos	= rhs.m_dSttHDanPos;
	m_dEndHDanPos	= rhs.m_dEndHDanPos;
}

CHLLGuardFence::~CHLLGuardFence()
{

}

CHLLGuardFence& CHLLGuardFence::operator=(const CHLLGuardFence& rhs)
{
	if(&rhs == this)
		return *this;
	m_nGuardIndex	= rhs.m_nGuardIndex;
	m_dSttHDanPos	= rhs.m_dSttHDanPos;
	m_dEndHDanPos	= rhs.m_dEndHDanPos;
	return *this;
}
//////////////////////////////////////////////////////////////////////////
CHLLLaneElement::CHLLLaneElement()
{
	m_dLoad			= 0;
	m_dSttHDanPos	= 0;
	m_dEndHDanPos	= 0;
	m_bMove			= FALSE;
}

CHLLLaneElement::CHLLLaneElement(const CHLLLaneElement& rhs)
{
	m_dLoad			= rhs.m_dLoad;
	m_dSttHDanPos	= rhs.m_dSttHDanPos;
	m_dEndHDanPos	= rhs.m_dEndHDanPos;
	m_bMove			= rhs.m_bMove;
}

CHLLLaneElement::~CHLLLaneElement()
{

}

CHLLLaneElement& CHLLLaneElement::operator=(const CHLLLaneElement& rhs)
{
	if(&rhs == this)
		return *this;
	m_dLoad			= rhs.m_dLoad;
	m_dSttHDanPos	= rhs.m_dSttHDanPos;
	m_dEndHDanPos	= rhs.m_dEndHDanPos;
	m_bMove			= rhs.m_bMove;
	return *this;
}
//////////////////////////////////////////////////////////////////////////
CHLLLaneUnit::CHLLLaneUnit()
{
	m_nLaneType		= 0;
	m_szLCName		= _T("");
	m_szDescription	= _T("");
	m_vLaneElem.clear();
}

CHLLLaneUnit::CHLLLaneUnit(const CHLLLaneUnit& rhs)
{
	m_nLaneType		= rhs.m_nLaneType;
	m_szLCName		= rhs.m_szLCName;
	m_szDescription	= rhs.m_szDescription;
	m_vLaneElem		= rhs.m_vLaneElem;
}

CHLLLaneUnit::~CHLLLaneUnit()
{
}

CHLLLaneUnit& CHLLLaneUnit::operator=(const CHLLLaneUnit& rhs)
{
	if(&rhs == this)
		return *this;
	m_nLaneType		= rhs.m_nLaneType;
	m_szLCName		= rhs.m_szLCName;
	m_szDescription	= rhs.m_szDescription;
	m_vLaneElem		= rhs.m_vLaneElem;
	return *this;
}

CHLLLaneElement& CHLLLaneUnit::GetElementSide(BOOL bLeft)
{
	long	nIdx		= 0;
	long	nIdxSide	= 0;
	double	dDist		= bLeft ? 999999999 : 0;

	vector<CHLLLaneElement>::iterator w;
	w = m_vLaneElem.begin();

	while(w != m_vLaneElem.end())
	{
		CHLLLaneElement Elem = *w;
		if(bLeft && dDist>Elem.m_dSttHDanPos)
		{
			dDist		= Elem.m_dSttHDanPos;
			nIdxSide	= nIdx;
		}
		if(!bLeft && dDist<Elem.m_dEndHDanPos)
		{
			dDist		= Elem.m_dEndHDanPos;
			nIdxSide	= nIdx;
		}
		w++;
		nIdx++;
	}

	return (CHLLLaneElement&)m_vLaneElem.at(nIdxSide);
}

//////////////////////////////////////////////////////////////////////////
CHLLLane::CHLLLane()
{

}

CHLLLane::CHLLLane(const CHLLLane& rhs)
{

}

CHLLLane::~CHLLLane()
{
}

CHLLLane& CHLLLane::operator=(const CHLLLane& rhs)
{
	if(&rhs == this)
		return *this;
	return *this;
}
//////////////////////////////////////////////////////////////////////////

CCalcData::CCalcData(CDataManage* pManage)
{
	m_pDataManage = pManage;
	CSteelConc *pConc = m_pDataManage->GetSteelConc();
	
	m_nFlag							 = 0;
	DESIGN_BASE.m_szBridgeType       = ""; //��������
	DESIGN_BASE.m_szBridgeLength     = ""; //��������
	DESIGN_BASE.m_szBridgeLineType   = ""; //��������
	DESIGN_BASE.m_dBridgeSkew        = 90; //�簢

	DESIGN_CONDITION.m_szBridgeGrade = ""; //�������
	DESIGN_CONDITION.m_dPf           = 0;  //DB����-Pf
	DESIGN_CONDITION.m_dPr           = 0;  //DB����-Pr 
	DESIGN_CONDITION.m_dW            = 0;  //DL����-���������
	DESIGN_CONDITION.m_dPm           = 0;  //DL����-��������(���Ʈ ����)
	DESIGN_CONDITION.m_dPs           = 0;  //DL����-��������(���ܷ� ����)
	//kg.cm����
	long nIdx = pConc->GetIndex("S27");	

	DESIGN_MATERIAL.m_dSigmaCK		= 27;  //��ũ��Ʈ ���భ��
	DESIGN_MATERIAL.m_dSigmaCi		= 28;  //��ũ��Ʈ ���భ��
	DESIGN_MATERIAL.m_dBindSigmaCK	= 50;	//������ũ��Ʈ ���భ��
	DESIGN_MATERIAL.m_dSigmaY		= 400;  //ö��     �������� 
	DESIGN_MATERIAL.m_sSigmaY		= "H";  //400 MPa �̻� ���� ö��...ǥ��..
	DESIGN_MATERIAL.m_dSigmaSA		= 180;
	DESIGN_MATERIAL.m_dSigmaDeckSA	= 160;
	DESIGN_MATERIAL.m_szSwsJu		= "SM490B"; //��������(�ֺ���)
	DESIGN_MATERIAL.m_szSwsBu		= "SM490B"; //��������(�κ���)
	DESIGN_MATERIAL.m_dEc			= GetValueEcConc(DESIGN_MATERIAL.m_dSigmaCK,2.35);
	DESIGN_MATERIAL.m_dEci			= GetValueEcConc(DESIGN_MATERIAL.m_dSigmaCi,2.35);
	DESIGN_MATERIAL.m_dBindEc		= 34000;	//������ũ��Ʈ
	DESIGN_MATERIAL.m_dEs			= 200000;  //ö�� ź�����
	DESIGN_MATERIAL.m_dEst			= 210000;
	DESIGN_MATERIAL.m_dDummyEst	 	= 210000;
	DESIGN_MATERIAL.m_nN			= 8;  //ź�������
	DESIGN_MATERIAL.m_nBindNc		= 6;  //������ũ��Ʈ ź�������
	DESIGN_MATERIAL.m_dEpsilonS		= pConc->GetEpsilon(nIdx);  //���������� 
	DESIGN_MATERIAL.m_dAlpha		= pConc->GetAlpha(nIdx);  //����â��� 
	DESIGN_MATERIAL.m_dCreep		= pConc->GetCreep1(nIdx);  //ũ����
	DESIGN_MATERIAL.m_dCreep2		= pConc->GetCreep2(nIdx);
	DESIGN_MATERIAL.m_dTemparature	= pConc->GetTemperature(nIdx);  //���� ��ũ��Ʈ�� �µ��� 
	DESIGN_MATERIAL.m_dShearEs		= 81000.0;	///< ���� ź�����
	DESIGN_MATERIAL.m_RatePoasong	= 0.167;
	//�������
	//������Ʈ���� ���� ���� ��������
	SetDefaultAllowDesignMaterials();

	DESIGN_UNIT_WEIGHT.m_dGangjae	= 0;
	DESIGN_UNIT_WEIGHT.m_dAluminum	= 0;				
	DESIGN_UNIT_WEIGHT.m_dChulgun	= 0;		
	DESIGN_UNIT_WEIGHT.m_dMugun		= 0;
	DESIGN_UNIT_WEIGHT.m_dCement	= 0;
	DESIGN_UNIT_WEIGHT.m_dMokjae	= 0;
	DESIGN_UNIT_WEIGHT.m_dYukchung	= 0;
	DESIGN_UNIT_WEIGHT.m_dAsphalt	= 0;
	DESIGN_UNIT_WEIGHT.m_dGravel	= 0;

	long n = 0;
	//DESIGN_FLOOR_DATA	
	for(n=0; n<FLOOR_SIZE; n++)
	{
		DESIGN_FLOOR_DATA[n].m_bBangEm			= FALSE;
		DESIGN_FLOOR_DATA[n].m_dBangEmWeight	= 0.0;
		DESIGN_FLOOR_DATA[n].m_dBangEmHeight	= 0.0;
		DESIGN_FLOOR_DATA[n].m_dBangEmDis		= 0.0;	

		DESIGN_FLOOR_DATA[n].m_bNanGan			= FALSE;	
		DESIGN_FLOOR_DATA[n].m_dNanGanWeight	= 0.0;
		DESIGN_FLOOR_DATA[n].m_dNanGanDis		= 0.0;
		DESIGN_FLOOR_DATA[n].m_dNanGanHeight	= 0.0;
		
		DESIGN_FLOOR_DATA[n].m_bWalkLoad		= FALSE;
		DESIGN_FLOOR_DATA[n].m_bPeopleLoad		= FALSE;
		DESIGN_FLOOR_DATA[n].m_dWalkLoadWidth	= 0.0;
		DESIGN_FLOOR_DATA[n].m_dWalkLoad1		= 0.0;
		DESIGN_FLOOR_DATA[n].m_dWalkLoad2		= 0.0;
		DESIGN_FLOOR_DATA[n].m_dWalkLoadMoment  = 0.0;
		DESIGN_FLOOR_DATA[n].m_dWalkLoadH		= 0.0;

		DESIGN_FLOOR_DATA[n].m_bBangHo			= FALSE;
		DESIGN_FLOOR_DATA[n].m_dBangHoWidth		= 0.0;	
		DESIGN_FLOOR_DATA[n].m_dBangHoWeight	= 0.0;	
		for(long a=0; a<HLLMAXSU; a++)
		{
			DESIGN_FLOOR_DATA[n].m_dBangHoLEdge[a]		= 0.0;	
			DESIGN_FLOOR_DATA[n].m_dBangHoREdge[a]		= 0.0;	
			DESIGN_FLOOR_DATA[n].m_dWalkLoadLEdge[a]	= 0.0;	
			DESIGN_FLOOR_DATA[n].m_dWalkLoadREdge[a]	= 0.0;	
		}
	}
	_CALC_CANTILEVER_DATA Cantilever_Data;
	for(n=0; n<FLOOR_SIZE; n++)
	{
		Cantilever_Data = CALC_CANTILEVER_DATA[n];
		Cantilever_Data.m_Min_T = 0.0;		// �ٴ��� �ּҵβ�
		CALC_CANTILEVER_DATA[n].m_Req_T = 0.0;
		
		Cantilever_Data.m_Stop_M = 0.0;		// ������
		Cantilever_Data.m_Stop_Ton = 0.0;
		Cantilever_Data.m_StopE_M = 0.0;	// ������( Slab ���� )
		Cantilever_Data.m_StopE_Ton = 0.0;
		Cantilever_Data.m_StopB_M = 0.0;

		Cantilever_Data.m_Dead_L = 0.0;
		Cantilever_Data.m_Move_L = 0.0;		// Ȱ����
		Cantilever_Data.m_Tank_L = 0.0;		// 
		Cantilever_Data.m_Trailer_L = 0.0;		// 
		Cantilever_Data.m_Move_E = 0.0;
		Cantilever_Data.m_Dead_E = 0.0;
		Cantilever_Data.m_Move_E3 = 0.0;
		Cantilever_Data.m_Move_i = 0.0;
		Cantilever_Data.m_Move_Pr = 0.0;
		Cantilever_Data.m_Move_Ml = 0.0;
		Cantilever_Data.m_Move_Ml_ = 0.0;
		Cantilever_Data.m_MoveB_Ml = 0.0;
		Cantilever_Data.m_MoveB_Ml_Tank = 0.0;
		Cantilever_Data.m_MoveB_Ml_Trailer = 0.0;
		Cantilever_Data.m_Move_Ml_Tank = 0.0;
		Cantilever_Data.m_Move_Ml_Trailer = 0.0;
		Cantilever_Data.m_Move_GiJumsu = 0;
		Cantilever_Data.m_Move_M = 0.0;

		Cantilever_Data.m_Coll_V = 120.0;		// �浹����
		Cantilever_Data.m_Coll_H = 0.0;
		Cantilever_Data.m_Coll_Co = 0.0;
		Cantilever_Data.m_Coll_Mo = 0.0;
		Cantilever_Data.m_Nangan_VerF = 1.0;
		Cantilever_Data.m_Nangan_HoriF = 3.75;
		
		Cantilever_Data.m_Nangan_dH = 0.0;
		Cantilever_Data.m_Nangan_dL = 0.0;

		Cantilever_Data.m_Wind_Pw = 0.0;
		Cantilever_Data.m_Wind_Mw = 0.0;
		Cantilever_Data.m_Wind_H =0.0;

		Cantilever_Data.m_Cent_R = 0.0;		// ��������
		Cantilever_Data.m_Cent_Pcf = 0.0;
		Cantilever_Data.m_Cent_Pcf_Tank = 0.0;
		Cantilever_Data.m_Cent_Pcf_Trailer = 0.0;
		Cantilever_Data.m_Cent_Mcf = 0.0;
		Cantilever_Data.m_Cent_Mcf_Tank = 0.0;
		Cantilever_Data.m_Cent_Mcf_Trailer = 0.0;
		Cantilever_Data.m_CentB_Mcf = 0.0;
		Cantilever_Data.m_CentB_Mcf_Tank = 0.0;
		Cantilever_Data.m_CentB_Mcf_Trailer = 0.0;

		Cantilever_Data.m_Rein_Mu = 0.0;	// ö�ٷ� ���Ʈ
		Cantilever_Data.m_Rein_Md = 0.0;	// Md = toTonM(Pib*UseAs*y*(d-a/2)*10); //ton.m
		Cantilever_Data.m_Rein_a = 0.0;
		Cantilever_Data.m_Rein_b = 1000.0;
		Cantilever_Data.m_Rein_d = 0.0;//260.0;
		Cantilever_Data.m_Rein_CaseH1		= _T("");
		Cantilever_Data.m_Rein_CaseH1_2Cy	= _T("");
		Cantilever_Data.m_Rein_CaseH2		= _T("");
		Cantilever_Data.m_Rein_CaseH2_2Cy	= _T("");
		Cantilever_Data.m_Rein_CaseH3		= _T("");
		Cantilever_Data.m_Rein_CaseH4		= _T("");
		Cantilever_Data.m_Rein_Combo1		= 0;
		Cantilever_Data.m_Rein_Combo1_2Cy	= 0;
		Cantilever_Data.m_Rein_Combo2		= 0;
		Cantilever_Data.m_Rein_Combo2_2Cy	= 0;
		Cantilever_Data.m_Rein_Combo3		= 0;
		Cantilever_Data.m_Rein_Combo4		= 0;
		Cantilever_Data.m_Rein_Ctc1			= 0.0;
		Cantilever_Data.m_Rein_Ctc2			= 0.0;
		Cantilever_Data.m_Rein_CtcB			= 0.0;
		Cantilever_Data.m_Rein_CtcB1		= 0.0;
		Cantilever_Data.m_Rein_UseAs		= 0.0;
		Cantilever_Data.m_Rein_UseAs_1Cy	= 0.0;
		Cantilever_Data.m_Rein_UseAs_2Cy	= 0.0;
		Cantilever_Data.m_Rein_UseAs2		= 0.0;
		Cantilever_Data.m_Rein_UseAs2_1Cy	= 0.0;
		Cantilever_Data.m_Rein_UseAs2_2Cy	= 0.0;
		Cantilever_Data.m_Rein_UseAsBr		= 0.0;
		Cantilever_Data.m_Rein_BAsuse		= 0.0;
		Cantilever_Data.m_Rein_BAsuse2		= 0.0;
		Cantilever_Data.m_Rein_Asreq		= 0.0;
		Cantilever_Data.m_Rein_BAsreq		= 0.0;
		Cantilever_Data.m_Rein_Pused		= 0.0;
		Cantilever_Data.m_Rein_dia			= 0.0;
		Cantilever_Data.m_bCheckGen1		= FALSE;		// �Ϲݺ� ��ö��   ����
		Cantilever_Data.m_bCheckGen2		= FALSE;		// �Ϲݺ� ���ö�� ����
		Cantilever_Data.m_bCheckDan1		= FALSE;		// �ܺ�   ��ö��   ����
		Cantilever_Data.m_bCheckDan2		= FALSE;		// �ܺ�   ���ö�� ����

		Cantilever_Data.m_Rein_Up_CaseH1_1Cy	= _T("");
		Cantilever_Data.m_Rein_Up_CaseH1_2Cy	= _T("");
		Cantilever_Data.m_Rein_Up_Combo1_1Cy	= 0;
		Cantilever_Data.m_Rein_Up_Combo1_2Cy	= 0;
		Cantilever_Data.m_Rein_Up_Ctc1			= 0;
		Cantilever_Data.m_Rein_Up_Asuse1_1Cy	= 0;
		Cantilever_Data.m_Rein_Up_Asuse1_2Cy	= 0;
		Cantilever_Data.m_Rein_Up_Asuse1		= 0;
		Cantilever_Data.m_Rein_Up_Asreq1		= 0;
		Cantilever_Data.m_Rein_Up_CaseH2		= _T("");
		Cantilever_Data.m_Rein_Up_Combo2		= 0;
		Cantilever_Data.m_Rein_Up_Ctc2			= 0;
		Cantilever_Data.m_Rein_Up_Asuse2		= 0;
		Cantilever_Data.m_Rein_Up_Asreq2		= 0;
		Cantilever_Data.m_bCheck_Up				= FALSE;
		Cantilever_Data.m_bCheck_UpB			= FALSE;
		Cantilever_Data.m_Rein_Lo_Asreq			= 0.0;
		Cantilever_Data.m_Rein_Lo_BAsreq		= 0.0;
		Cantilever_Data.m_Rein_Lo_Asuse			= 0.0;
		Cantilever_Data.m_Rein_Lo_BAsuse		= 0.0;

		Cantilever_Data.m_Crack_w = 0;
		Cantilever_Data.m_Crack_wa= 0;
		Cantilever_Data.m_Crack_wBr = 0;
		Cantilever_Data.m_Crack_waBr = 0;

		Cantilever_Data.m_strGenReinTDia_1Cy	= _T("");
		Cantilever_Data.m_strGenReinTDia_2Cy	= _T("");
		Cantilever_Data.m_nGenReinTDiaIdx_1Cy	= 0;
		Cantilever_Data.m_nGenReinTDiaIdx_2Cy	= 0;
		Cantilever_Data.m_dGenReinTCTC			= 0;
		Cantilever_Data.m_dGenReinTAsreq		= 0;
		Cantilever_Data.m_dGenReinTAsuse		= 0;
		Cantilever_Data.m_dGenReinTAsuse_1Cy	= 0;
		Cantilever_Data.m_dGenReinTAsuse_2Cy	= 0;

		Cantilever_Data.m_strGenReinCDia_1Cy	= _T("");
		Cantilever_Data.m_strGenReinCDia_2Cy	= _T("");
		Cantilever_Data.m_nGenReinCDiaIdx_1Cy	= 0;
		Cantilever_Data.m_nGenReinCDiaIdx_2Cy	= 0;
		Cantilever_Data.m_dGenReinCCTC			= 0;
		Cantilever_Data.m_dGenReinCAsreq		= 0;
		Cantilever_Data.m_dGenReinCAsuse		= 0;
		Cantilever_Data.m_dGenReinCAsuse_1Cy	= 0;
		Cantilever_Data.m_dGenReinCAsuse_2Cy	= 0;

		CALC_CANTILEVER_DATA[n] = Cantilever_Data;
	}
	m_Pib			= 0.85;
	m_Pis			= 0.80;
	m_dCoverUp	    = 60.0;
	m_dCoverDn		= 40.0;
	m_dCoverSd		= 50.0;
	m_dRebarPmax    = 0.0;
	m_Rein_Arrange  = 0.0;
	m_Asreq         = 0.0;
	m_Ag            = 0.0;
	m_AsEa1         = 0.0;
	m_AsEa2         = 0.0;
	m_Asuse1        = 0.0;
	m_Asuse2        = 0.0;
	m_Asuset        = 0.0;
	m_Ju_ratio      = 0.0;
	m_dia1          = 0.0;
	m_dia2          = 0.0;
	m_Ctc1          = 0.0;
	m_Ctc2          = 0.0;
	m_CaseH1 = _T("");
	m_CaseH2 = _T("");
	m_ComboInnerJijumSlab1 = 0;
	m_ComboInnerJijumSlab2 = 0;
	m_bCheckInnerJijum		= FALSE;
	m_nEndPartRebarConditionIdx = 1;

	m_bTank			= FALSE;			
	m_bTrailer		= FALSE;		
	m_bMugun        = FALSE;	
	m_bJungbunMugun	= FALSE;
	m_bExtraLoad    = FALSE;	
	m_bWind         = FALSE; 
	m_dSpeed        = 120;
	m_dWindLoadForce = 3;
	m_dRadiusOfCurvature = 0;

	m_bIsFirst		= TRUE;				
	m_bHLLError		= FALSE;				
	m_bLWalkHLL		= FALSE;				
	m_bRWalkHLL		= FALSE;				
	m_bNoPojang		= FALSE;

	m_HLType		= 0;					
	m_HLsu			= 0;					
	m_nHLiveLoadType = 1;
	m_bNoDistOnNoGuard = FALSE;
	
	m_bMakeModeling		  = FALSE;
	m_bPassedCalcFloorDlg = FALSE;
	m_bCheckSettlement    = TRUE;
	m_bRunCross			  = FALSE;
	m_bRunCrossWind		  = FALSE;
	m_bRunInfluence		  = FALSE;

	m_CurHL				= 0;						
	m_CurLiveHL			= 0;

	m_dDesignLaneWidth	= 0;	
	m_dUserLaneWidth	= 3.0;				
 	m_DesignChasunSu	= 0;
	m_DesignChasunSuR	= 0;

	m_AddRowTRL			= 0;
	m_MaxImpact			= 0.3;
	m_DBWidth			= 1.8;
	m_DBFromEdge		= 0.6;
	m_DBDis				= 1.2;
	m_DLDis				= 3;

	m_TankDis			= 0.6;		// ���� : m
	m_TankWidth			= 2.60;		// ���� : m
	m_TrailDis			= 0.60;		// ���� : m
	m_TrailWidth		= 2.225;	// ���� : m
	
	m_TankWeight		= 600;		// ���� : kN
	m_TankEdit1			= 4.500;	// ���� : m
	m_TankEdit2			= 0.700;	// ���� : m
	m_TankEdit3			= 1.900;	// ���� : m
	m_TankImpact		= 15.0;

	m_TrailEdit1		= 3.600;	// ���� : m
	m_TrailEdit2		= 1.500;	// ���� : m
	m_TrailEdit3		= 4.500;	// ���� : m
	m_TrailEdit4		= 1.200;	// ���� : m
	m_TrailEdit5		= 0.975;	// ���� : m
	m_TrailEdit6		= 1.250;	// ���� : m
	m_TrailImpact		= 15.0;

	m_TrailP1			= 45.0;		// ���� : kN
	m_TrailP2			= 90.0;		// ���� : kN
	m_TrailP3			= 65.0;		// ���� : kN

	m_AddLoad			= 0;
	m_TotalWeight		= 0;		// ���� : kN
	m_ModelWeight		= 0;									
	m_BonusRate			= 0;									
	m_JijumDownSize		= 0;
	m_nModelingSettleMethod	= 0;
	m_bApplyAddRow		= FALSE;								
	m_bPic				= FALSE;								
	m_bReadFrameModelingOutput = FALSE;
	m_bReadLiveDispOutput      = FALSE;

	m_szFolderPath = "";

	for(n=0; n<HLLMAXSU; n++)
	{
		m_dLeftWindFroceFromL[n]	= 0;
		m_dLeftWindFroceFromR[n]	= 0;
		m_dRightWindFroceFromL[n]	= 0;
		m_dRightWindFroceFromR[n]	= 0;
		m_dWindForceToGirderL[n]	= 0;
		m_dWindForceToGirderR[n]	= 0;
		m_dLeftWindMomentFromL[n]	= 0;
		m_dLeftWindMomentFromR[n]	= 0;
		m_dRightWindMomentFromL[n]	= 0;
		m_dRightWindMomentFromR[n]	= 0;
	}
	m_nFatigueRoadType = 0;
	m_nFatigueStructureType = 0;

	// �������� �ʱ�ȭ
	m_dLenSinchukGir[0] = 150;		// ���� �麸�� �Ÿ�
	m_dLenSinchukGir[1] = 150;		// ���� �麸�� �Ÿ�
	m_dHeightGir[0]  = 2.3;		// ������ ���� h :
	m_dHeightGir[1]  = 2.3;		// ������ ���� h :
	m_nTypePos       = 0;
	m_nTypeResist[0] = 0;		//������������
	m_nTypeResist[1] = 0;		//������������
	m_nTypeArea[0] = 0;		//����/�ѷ����� 
	m_nTypeArea[1] = 0;		//����/�ѷ����� 

			
	m_RotAngle = 1.0/150.0;		//ȸ���� ��:
	m_D1[0] = 10;		//��ġ������ d1 :
	m_D1[1] = 10;		//��ġ������ d1 :
	m_Bfa = 0;		//���� ��� b :
	m_Tfa = 50;		//�µ���ȭ�� ��T :
	m_Afa = 1.2e-05;		//����â��� a :
	m_D2[0] = 20;		//�ΰ� ������ d2 			
	m_D2[1] = 20;		//�ΰ� ������ d2 			

	m_MinUgan[0] = 120;
	m_MinUgan[1] = 120;
	m_MaxUgan[0] = 280;
	m_MaxUgan[1] = 280;

	m_szUseJoint[0] = "NO.160";
	m_szUseJoint[1] = "NO.160";

	m_AllowUgan[0] = 160;
	m_AllowUgan[1] = 160;

	m_dEQd = 0;		//�����û�κ���...

	DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx		= 12;
	DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName	= _T("12.7mm 7����");
	DESIGN_STEELPRESTRESS_DATA.m_dEs					= 200000.0;				///< ź�����Es(MPa)
	DESIGN_STEELPRESTRESS_DATA.m_dArea					= 98.71;
	DESIGN_STEELPRESTRESS_DATA.m_dDiameter				= 12.7;
	DESIGN_STEELPRESTRESS_DATA.m_dTensionLoad			= 1894.44;				///< ���Ѱ���(MPa)
	DESIGN_STEELPRESTRESS_DATA.m_dLoadOfElongation		= 1610.78;				///< �׺�����(MPa)
	DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength		= 1894.44;
	DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength			= 1610.78;
	DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction		= 0.005;				///< �Ļ������
	DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction		= 0.25;					///< ����������
	DESIGN_STEELPRESTRESS_DATA.m_nDuctType				= 1;					///< ��Ʈ���� - �ƿ����� �ݼӽ���
	DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty			= 1;					///< PS���� ����
	DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC			= 125.0;				///< ����
	//DESIGN_STEELPRESTRESS_DATA.m_dSttAnchorageSlip		= 6.0;					///< ������ ������ Ȱ����
	//DESIGN_STEELPRESTRESS_DATA.m_dEndAnchorageSlip		= 6.0;					///< ������ ������ Ȱ����
	DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip			=	6.0;				//������ Ȱ����
	DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter			= 22.4;					///< ��Ʈ ����
	DESIGN_STEELPRESTRESS_DATA.m_dHumidity				= 70.0;					///< ��ջ�����
	DESIGN_STEELPRESTRESS_DATA.m_nFixingPlateType		= 0;					///< �������� ����
	DESIGN_STEELPRESTRESS_DATA.m_nVerRein				= 1;					///< ����ö�� combo
	DESIGN_STEELPRESTRESS_DATA.m_nHorRein1				= 1;					///< ���� ������ ö�� combo
	DESIGN_STEELPRESTRESS_DATA.m_nHorRein2				= 1;					///< ���� ������ ���� ö�� combo
	DESIGN_STEELPRESTRESS_DATA.m_dVerReinAs				= 7.944;					///< ����ö�ٷ�
	DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs1			= 3.972;				///< ���� ������ ö�ٷ�
	DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs2			= 7.944;				///< ���� ������ ���� ö�ٷ�
	DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty			= 4;					///< ����ö�ټ�
	DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1			= 2;					///< ���� ������ ö�ټ�
	DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2			= 4;					///< ���� ������ ���� ö�ټ�
	DESIGN_STEELPRESTRESS_DATA.m_dFixingPlateLength		= 75;					///< �������� ����
	
	//APlate2010 �߰�
	DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP				= 0.28;					///< PS ������ ������ ���� ���
	DESIGN_STEELPRESTRESS_DATA.m_dSheathW				= 190;					///< ������ ��
	DESIGN_STEELPRESTRESS_DATA.m_dSheathH				= 70;					///< ������ ����
	DESIGN_STEELPRESTRESS_DATA.m_sJackMax				= "0.90��fpy";			///< Jack�� ���� �ִ� ���� ����

	//�ű�...090723
	DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2			= 4;					///< ���� ������ ���� ö�ټ�
	DESIGN_STEELPRESTRESS_DATA.m_nVerRein2              = 1;					///< ���� ������ ���� ö�� combo

	
	//2006.08.31 �߰�
	DESIGN_STEELPRESTRESS_DATA.m_nAnchorageEA			= 4;					//�����Ǵ� ������
	DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth		= 0;					//������ ����		
	DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight		= 0;					//������ ����	double  
	DESIGN_STEELPRESTRESS_DATA.m_dSpallingForce			= 0;					//T1 ������ ��� �ҷ·�	double  
	DESIGN_STEELPRESTRESS_DATA.m_dBurstingForce			= 0;					//T2 ������ �պκ��� �Ŀ���	double  
	DESIGN_STEELPRESTRESS_DATA.m_dReqAs1				= 0;					//T1�� 	double  
	DESIGN_STEELPRESTRESS_DATA.m_dUseAs1				= 0;					//T1�� 	double  
	DESIGN_STEELPRESTRESS_DATA.m_dReqAs2				= 0;					//T2�� 	double  
	DESIGN_STEELPRESTRESS_DATA.m_dUseAs2				= 0;					//T2��	double  
	DESIGN_STEELPRESTRESS_DATA.m_dReinCTC				= 0;					//ö�ٰ���	double  
	DESIGN_STEELPRESTRESS_DATA.m_sCaseH1				= "";	 
	DESIGN_STEELPRESTRESS_DATA.m_sCaseH2				= "";
	DESIGN_STEELPRESTRESS_DATA.m_dPj					= 0;					//�������� ��������� ���	CString 

	m_bBucklingStress	= TRUE;
	m_bShoeGirderLower	= FALSE;

	m_dMatMinSpace = 1000;
	m_dMatMaxSpace = 100000;

	m_nLBDeckDiaIdx = 0;				// LB-Deck ���� index
	m_dLBDeckCTC = 0.0;					// LB-Deck CTC
	m_dLBDeckCover = 0.0;				// LB-Deck �Ǻ�
}

CCalcData::~CCalcData()
{

}

void CCalcData::Serialize(CArchive& ar)
{	
	//long nFlag=13;  m_dWalkLoadMoment �߰�
	//long nFlag=14;  m_Tank_L, m_Trailer_L �߰�...
//	long nFlag = 24;	//Aplate 2009.07.23 �۾�
//	long nFlag = 25;	//Aplate 2009.08.28 �۾�
//	long nFlag = 26;	//Aplate 2009.11.2 �۾�
//	long nFlag = 27; // ����ħ�� ��������
//	long nFlag = 28; // LB-Deck ���� ���� �߰�	2012.02.28
//	long nFlag = 29; // m_nEndPartRebarConditionIdx  2012.06.01
//	long nFlag = 30;	// Ⱦ���� ���ߺй� ��ġ ���� ������ ǰ (���� 20��) 2013.10.29 #41383
	long nFlag = 31; //m_bRunCrossWind
		 

	long	nVal	=	0;
	double	dTemp1	=	0.0;
	long	n		=	0;
	if(ar.IsStoring())
	{
		ar << nFlag;

		ar << DESIGN_BASE.m_szBridgeType;
		ar << DESIGN_BASE.m_szBridgeLength;
		ar << DESIGN_BASE.m_szBridgeLineType;
		ar << DESIGN_BASE.m_dBridgeWidth;
		ar << DESIGN_BASE.m_dBridgeSkew;

		ar << DESIGN_CONDITION.m_szBridgeGrade;
		ar << DESIGN_CONDITION.m_dPf;
		ar << DESIGN_CONDITION.m_dPr;
		ar << DESIGN_CONDITION.m_dW;
		ar << DESIGN_CONDITION.m_dPm;
		ar << DESIGN_CONDITION.m_dPs;
				
		ar << DESIGN_MATERIAL.m_dSigmaCK;
		ar << DESIGN_MATERIAL.m_dSigmaY;
		ar << DESIGN_MATERIAL.m_dSigmaSA;
		ar << DESIGN_MATERIAL.m_szSwsJu;
		ar << DESIGN_MATERIAL.m_szSwsBu;
		ar << DESIGN_MATERIAL.m_dEc;
		ar << DESIGN_MATERIAL.m_dEs;
		ar << DESIGN_MATERIAL.m_dEst;
		ar << DESIGN_MATERIAL.m_nN;
		ar << DESIGN_MATERIAL.m_dEpsilonS;
		ar << DESIGN_MATERIAL.m_dAlpha;
		ar << DESIGN_MATERIAL.m_dCreep;
		ar << DESIGN_MATERIAL.m_dCreep2;
		ar << DESIGN_MATERIAL.m_dTemparature;
		ar << DESIGN_MATERIAL.m_dShearEs;		
		ar << DESIGN_MATERIAL.m_dSigmaDeckSA;
		ar << DESIGN_MATERIAL.m_dEci;
		ar << DESIGN_MATERIAL.m_RatePoasong;
		ar << DESIGN_MATERIAL.m_strAllow_PreStressPressPow;
		ar << DESIGN_MATERIAL.m_strAllow_PreStressStretchPow;
		ar << DESIGN_MATERIAL.m_strAllow_CreWeightPressPow;
		ar << DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow;
		ar << DESIGN_MATERIAL.m_strAllow_DeckConcStretchPow;
		ar << DESIGN_MATERIAL.m_strCrackPow;
				
		ar << DESIGN_UNIT_WEIGHT.m_dGangjae;
		ar << DESIGN_UNIT_WEIGHT.m_dAluminum;				
		ar << DESIGN_UNIT_WEIGHT.m_dChulgun;		
		ar << DESIGN_UNIT_WEIGHT.m_dMugun;
		ar << DESIGN_UNIT_WEIGHT.m_dCement;
		ar << DESIGN_UNIT_WEIGHT.m_dMokjae;
		ar << DESIGN_UNIT_WEIGHT.m_dYukchung;
		ar << DESIGN_UNIT_WEIGHT.m_dAsphalt;
		ar << DESIGN_UNIT_WEIGHT.m_dGravel;

		for(n=0; n<FLOOR_SIZE; n++)
		{
			ar << DESIGN_FLOOR_DATA[n].m_bBangEm;
			ar << DESIGN_FLOOR_DATA[n].m_dBangEmWeight;
			ar << DESIGN_FLOOR_DATA[n].m_dBangEmHeight;
			ar << DESIGN_FLOOR_DATA[n].m_dBangEmDis;
			
			ar << DESIGN_FLOOR_DATA[n].m_bNanGan;	
			ar << DESIGN_FLOOR_DATA[n].m_dNanGanWeight;
			ar << DESIGN_FLOOR_DATA[n].m_dNanGanDis;
			ar << DESIGN_FLOOR_DATA[n].m_dNanGanHeight;
			
			ar << DESIGN_FLOOR_DATA[n].m_bWalkLoad;	
			ar << DESIGN_FLOOR_DATA[n].m_bPeopleLoad;
			ar << DESIGN_FLOOR_DATA[n].m_dWalkLoadWidth;
			ar << DESIGN_FLOOR_DATA[n].m_dWalkLoad1;
			ar << DESIGN_FLOOR_DATA[n].m_dWalkLoad2;
			ar << DESIGN_FLOOR_DATA[n].m_dWalkLoadMoment;
			ar << DESIGN_FLOOR_DATA[n].m_dWalkLoadH;
			
			ar << DESIGN_FLOOR_DATA[n].m_bBangHo;
			ar << DESIGN_FLOOR_DATA[n].m_dBangHoWidth;	
			ar << DESIGN_FLOOR_DATA[n].m_dBangHoWeight;	
		
			for(long a=0; a<HLLMAXSU; a++)
			{
				ar << DESIGN_FLOOR_DATA[n].m_dBangHoLEdge[a];	
				ar << DESIGN_FLOOR_DATA[n].m_dBangHoREdge[a];	
				ar << DESIGN_FLOOR_DATA[n].m_dWalkLoadLEdge[a];	
				ar << DESIGN_FLOOR_DATA[n].m_dWalkLoadREdge[a];	
			}
		}

		_CALC_CANTILEVER_DATA Cantilever_Data;
		for(n=0; n<FLOOR_SIZE; n++)
		{
			Cantilever_Data = CALC_CANTILEVER_DATA[n];
			ar << Cantilever_Data.m_Min_T;		
			ar << Cantilever_Data.m_Req_T;		

			ar << Cantilever_Data.m_Stop_M;	
			ar << Cantilever_Data.m_Stop_Ton;	
			ar << Cantilever_Data.m_StopE_M;	
			ar << Cantilever_Data.m_StopE_Ton;
			ar << Cantilever_Data.m_StopB_M;
			
			ar << Cantilever_Data.m_Move_L;
			ar << Cantilever_Data.m_Tank_L;		//070409..�߰�...
			ar << Cantilever_Data.m_Trailer_L;	//070409..�߰�...
			ar << Cantilever_Data.m_Dead_L;			
			ar << Cantilever_Data.m_Move_E;   
			ar << Cantilever_Data.m_Dead_E;
			ar << Cantilever_Data.m_Move_E3;
			ar << Cantilever_Data.m_Move_i;   
			ar << Cantilever_Data.m_Move_Pr;  
			ar << Cantilever_Data.m_Move_Ml;
			ar << Cantilever_Data.m_Move_Ml_;
			ar << Cantilever_Data.m_MoveB_Ml;
			ar << Cantilever_Data.m_MoveB_Ml_Tank;
			ar << Cantilever_Data.m_MoveB_Ml_Trailer;
			ar << Cantilever_Data.m_Move_Ml_Tank;
			ar << Cantilever_Data.m_Move_Ml_Trailer;
			ar << Cantilever_Data.m_Move_GiJumsu;
			ar << Cantilever_Data.m_Move_M;  

			ar << Cantilever_Data.m_Coll_V;	
			ar << Cantilever_Data.m_Coll_H;   
			ar << Cantilever_Data.m_Coll_Co;  
			ar << Cantilever_Data.m_Coll_Mo;  
			ar << Cantilever_Data.m_Nangan_VerF; 
			ar << Cantilever_Data.m_Nangan_HoriF;			

			ar << Cantilever_Data.m_Wind_Pw;  
			ar << Cantilever_Data.m_Wind_Mw;  
			ar << Cantilever_Data.m_Wind_H;   

			ar << Cantilever_Data.m_Cent_R;	
			ar << Cantilever_Data.m_Cent_Pcf; 
			ar << Cantilever_Data.m_Cent_Pcf_Tank;
			ar << Cantilever_Data.m_Cent_Pcf_Trailer;
			ar << Cantilever_Data.m_Cent_Mcf;
			ar << Cantilever_Data.m_Cent_Mcf_Tank;
			ar << Cantilever_Data.m_Cent_Mcf_Trailer;
			ar << Cantilever_Data.m_CentB_Mcf;
			ar << Cantilever_Data.m_CentB_Mcf_Tank;
			ar << Cantilever_Data.m_CentB_Mcf_Trailer;
			
			ar << Cantilever_Data.m_Rein_Mu;	 
			ar << Cantilever_Data.m_Rein_Md;	 
			ar << Cantilever_Data.m_Rein_a;    
			ar << Cantilever_Data.m_Rein_b;    
			ar << Cantilever_Data.m_Rein_d;    
			ar << Cantilever_Data.m_Rein_CaseH1;
			ar << Cantilever_Data.m_Rein_CaseH2;
			ar << Cantilever_Data.m_Rein_CaseH3;
			ar << Cantilever_Data.m_Rein_CaseH4;
			ar << Cantilever_Data.m_Rein_Ctc1;   
			ar << Cantilever_Data.m_Rein_Ctc2;   
			ar << Cantilever_Data.m_Rein_CtcB;   
			ar << Cantilever_Data.m_Rein_CtcB1;  
			ar << Cantilever_Data.m_Rein_UseAs;
			ar << Cantilever_Data.m_Rein_UseAs2;
			ar << Cantilever_Data.m_Rein_UseAsBr;
			ar << Cantilever_Data.m_Rein_BAsuse; 
			ar << Cantilever_Data.m_Rein_BAsuse2;
			ar << Cantilever_Data.m_Rein_Asreq;  
			ar << Cantilever_Data.m_Rein_BAsreq; 
			ar << Cantilever_Data.m_Rein_Pused;  
			ar << Cantilever_Data.m_Rein_dia;    
			ar << Cantilever_Data.m_Rein_Combo1;	  
			ar << Cantilever_Data.m_Rein_Combo2;    
			ar << Cantilever_Data.m_Rein_Combo3;    
			ar << Cantilever_Data.m_Rein_Combo4;    
			ar << Cantilever_Data.m_Crack_w;     
			ar << Cantilever_Data.m_Crack_wa;
			ar << Cantilever_Data.m_Crack_wBr;
			ar << Cantilever_Data.m_Crack_waBr;
		}

		ar << m_Pib;		 
		ar << m_Pis;		 
		ar << m_dCoverUp;  
		ar << m_dCoverDn;
		ar << m_dCoverSd;
		ar << m_dRebarPmax;   
		ar << m_Rein_Arrange;
		ar << m_Asreq;       
		ar << m_Ag;          
		ar << m_AsEa1;       
		ar << m_AsEa2;       
		ar << m_Asuse1;      
		ar << m_Asuse2;	  
		ar << m_Asuset;      
		ar << m_Ju_ratio;    
		ar << m_dia1;        
		ar << m_dia2;        
		ar << m_Ctc1;        
		ar << m_Ctc2;        
		ar << m_CaseH1;     
		ar << m_CaseH2;     
		ar << m_ComboInnerJijumSlab1;
		ar << m_ComboInnerJijumSlab2;

		ar << m_dSpeed;
		ar << m_bTank;
		ar << m_bTrailer;
		ar << m_bMugun;
		ar << m_bExtraLoad;
		ar << m_bWind;

		m_ELoadTitleArr.Serialize(ar);
		m_ELoadWeightArr.Serialize(ar);
		m_ELoadDisArr.Serialize(ar);
		m_szRefArray.Serialize(ar);		

		ar << m_bIsFirst;				
		ar << m_bHLLError;				
		ar << m_bLWalkHLL;				
		ar << m_bRWalkHLL;	
		
		BOOL dTemp=0;	

		ar << dTemp;	// ���߿� ����				
		ar << m_bNoPojang;				

		ar << m_HLType;					
		ar << m_HLsu;					
		ar << m_nHLiveLoadType;			
		ar << m_bNoDistOnNoGuard;
		ar << m_bMakeModeling;
		ar << m_bPassedCalcFloorDlg;
		ar << m_bCheckSettlement;		
		ar << m_bReadFrameModelingOutput;
		ar << m_bRunCrossWind;

		ar << m_CurHL;						
		ar << m_CurLiveHL;		

		ar << m_dDesignLaneWidth;	
		ar << m_dUserLaneWidth;				
		ar << nVal;//m_ChasunSu;		
		ar << m_DesignChasunSu;
		ar << m_DesignChasunSuR;

		ar << m_AddRowTRL;		
		ar << m_MaxImpact;				
		ar << m_DBWidth;				
		ar << m_DBFromEdge;			
		ar << m_DBDis;					
		ar << m_DLDis;

		ar << m_bTank;
		ar << m_bTrailer;
		ar << m_TrailWidth;
		ar << m_TrailDis;
		ar << m_TankWidth;
		ar << m_TankDis;

		ar << m_TankWeight;
		ar << m_TankEdit1;
		ar << m_TankEdit2;
		ar << m_TankEdit3;
		ar << m_TankImpact;

		ar << m_TrailEdit1;
		ar << m_TrailEdit2;
		ar << m_TrailEdit3;
		ar << m_TrailEdit4;
		ar << m_TrailEdit5;
		ar << m_TrailEdit6;
		ar << m_TrailImpact;

		ar << m_TrailP1;
		ar << m_TrailP2;
		ar << m_TrailP3;

		for(n=0; n<HLLMAXSU; n++)
		{
			m_DArrDistanceDB[n].Serialize(ar);
			m_DArrDistanceDL[n].Serialize(ar);

			m_DArrDBChasun[n].Serialize(ar);
			m_DArrDLChasun[n].Serialize(ar);
			
			m_DArrMaxDBChasun[n].Serialize(ar);
			m_DArrMaxDLChasun[n].Serialize(ar);
			
			m_DArrDistanceTank[n].Serialize(ar);
			m_DArrDistanceTrail[n].Serialize(ar);

			ar << m_dLeftWindFroceFromL[n];			
			ar << m_dLeftWindFroceFromR[n];			
			ar << m_dRightWindFroceFromL[n];			
			ar << m_dRightWindFroceFromR[n];			
			ar << m_dWindForceToGirderL[n];			
			ar << m_dWindForceToGirderR[n];			
			ar << m_dLeftWindMomentFromL[n];			
			ar << m_dLeftWindMomentFromR[n];			
			ar << m_dRightWindMomentFromL[n];			
			ar << m_dRightWindMomentFromR[n];
		}
		m_dStationHLLPos.Serialize(ar);
		ar << m_AddLoad;									
		ar << m_TotalWeight;								
		ar << m_ModelWeight;								
		ar << m_BonusRate;									
		ar << m_JijumDownSize;								
		ar << m_bApplyAddRow;								
		ar << m_bPic;
		ar << m_bReadLiveDispOutput;
		ar << m_szFolderPath;

		long	nArrsu	=	m_ArrExtraSecTitle.GetSize();

		ar << nArrsu;

		for(n=0; n<nArrsu; n++)
		{
			ar << m_ArrExtraSecTitle[n];
			ar << m_ArrExtraSecGir[n];
			ar << m_ArrExtraSecSta[n];
		}

		ar << m_dMatMinSpace;
		ar << m_dMatMaxSpace;

		ar << m_nTypePos;									// ����������ġ(A1, A2)
		ar << m_nTypeResist[0];							    // ������������
		ar << m_nTypeResist[1];							    // ������������
		ar << m_dLenSinchukGir[0];							// ���� �麸�� �Ÿ�
		ar << m_dLenSinchukGir[1];							// ���� �麸�� �Ÿ�
		ar << m_dHeightGir[0];								// ������ ���� h :
		ar << m_dHeightGir[1];								// ������ ���� h :
		ar << m_RotAngle;									// ȸ���� ��:
		ar << m_Bfa;										// ���� ��� b :
		ar << m_Tfa;										// �µ���ȭ�� ��T :
		ar << m_Afa;										// ����â��� a :	
		ar << m_D1[0];										// ��ġ������ d1 :
		ar << m_D1[1];										// ��ġ������ d1 :
		ar << m_D2[0];										// �ΰ� ������ d2 :
		ar << m_D2[1];										// �ΰ� ������ d2 :
		ar << m_MaxUgan[0];									// �ִ�����
		ar << m_MaxUgan[1];									// �ִ�����
		ar << m_MinUgan[0];									// �ּ�����
		ar << m_MinUgan[1];									// �ּ�����
		ar << m_szUseJoint[0];								// ����ƮŸ��  
		ar << m_szUseJoint[1];								// ����ƮŸ��  

		ar << DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
		ar << DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dEs;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dArea;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dDiameter;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dTensionLoad;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dLoadOfElongation;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction;				///< �Ļ������
		ar << DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction;			///< ����������
		ar << DESIGN_STEELPRESTRESS_DATA.m_nDuctType;					///< ��Ʈ ����
		ar << DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;				///< PS���� ����
		ar << DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;				///< ����
		ar << DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip;				///< ������ Ȱ����
		ar << DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter;				///< ��Ʈ ����
		ar << DESIGN_STEELPRESTRESS_DATA.m_dHumidity;					///< ��ջ�� ����
		ar << DESIGN_STEELPRESTRESS_DATA.m_nFixingPlateType;
		ar << DESIGN_STEELPRESTRESS_DATA.m_nVerRein;
		ar << DESIGN_STEELPRESTRESS_DATA.m_nHorRein1;
		ar << DESIGN_STEELPRESTRESS_DATA.m_nHorRein2;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dVerReinAs;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs1;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs2;
		ar << DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty;	
		ar << DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1;
		ar << DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dFixingPlateLength;			///< �������� ����

		ar << m_dRadiusOfCurvature;	///< ���������

		ar << DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;
		ar << m_bBucklingStress;

		ar << DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP;	//PS ������ ������ ���� ���(��p)
		ar << DESIGN_STEELPRESTRESS_DATA.m_dSheathW;	//������ ��
		ar << DESIGN_STEELPRESTRESS_DATA.m_dSheathH;	//������ ����
		ar << DESIGN_STEELPRESTRESS_DATA.m_sJackMax;	//Jack�� ���� �ִ� ��������

		// 2009.07.23
		ar << DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2;
		ar << DESIGN_STEELPRESTRESS_DATA.m_nVerRein2;

		for(n=0; n<FLOOR_SIZE; n++)
		{
			ar << CALC_CANTILEVER_DATA[n].m_Rein_CaseH1_2Cy;	// ö������(�Ϲݺ� ��ö�� 2-Cycle)
			ar << CALC_CANTILEVER_DATA[n].m_Rein_CaseH2_2Cy;	// ö������(�ܺ�   ��ö�� 2-Cycle)
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Combo1_2Cy;	// �Ϲݺ� ��ö��Index 2-Cycle
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Combo2_2Cy;	// �ܺ�   ��ö��Index 2-Cycle
			ar << CALC_CANTILEVER_DATA[n].m_Rein_UseAs_1Cy;		// ���ö�ٷ�(�Ϲݺ� ��ö�� 1-Cycle)
			ar << CALC_CANTILEVER_DATA[n].m_Rein_UseAs_2Cy;		// ���ö�ٷ�(�Ϲݺ� ��ö�� 2-Cycle)
			ar << CALC_CANTILEVER_DATA[n].m_Rein_UseAs2_1Cy;	// ���ö�ٷ�(�ܺ�   ��ö�� 1-Cycle)
			ar << CALC_CANTILEVER_DATA[n].m_Rein_UseAs2_2Cy;	// ���ö�ٷ�(�ܺ�   ��ö�� 2-Cycle)
			ar << CALC_CANTILEVER_DATA[n].m_bCheckGen1;			// �Ϲݺ� ��ö��   ����
			ar << CALC_CANTILEVER_DATA[n].m_bCheckGen2;			// �Ϲݺ� ���ö�� ����
			ar << CALC_CANTILEVER_DATA[n].m_bCheckDan1;			// �ܺ�   ��ö��   ����
			ar << CALC_CANTILEVER_DATA[n].m_bCheckDan2;			// �ܺ�   ���ö�� ����
		}

		ar << m_bCheckInnerJijum;
		ar << m_nEndPartRebarConditionIdx;

		for(n=0; n<FLOOR_SIZE; n++)
		{
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_CaseH1_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_CaseH1_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Combo1_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Combo1_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Ctc1;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse1_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse1_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse1;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Asreq1;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_CaseH2;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Combo2;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Ctc2;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse2;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Up_Asreq2;
			ar << CALC_CANTILEVER_DATA[n].m_bCheck_Up;
			ar << CALC_CANTILEVER_DATA[n].m_bCheck_UpB;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Lo_Asreq;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Lo_BAsreq;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Lo_Asuse;
			ar << CALC_CANTILEVER_DATA[n].m_Rein_Lo_BAsuse;

			ar << CALC_CANTILEVER_DATA[n].m_strGenReinTDia_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_strGenReinTDia_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_nGenReinTDiaIdx_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_nGenReinTDiaIdx_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinTCTC;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinTAsreq;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinTAsuse;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinTAsuse_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinTAsuse_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_strGenReinCDia_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_strGenReinCDia_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_nGenReinCDiaIdx_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_nGenReinCDiaIdx_2Cy;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinCCTC;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinCAsreq;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinCAsuse;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinCAsuse_1Cy;
			ar << CALC_CANTILEVER_DATA[n].m_dGenReinCAsuse_2Cy;
		}

		ar << DESIGN_STEELPRESTRESS_DATA.m_nAnchorageEA;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dSpallingForce;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dBurstingForce;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dReqAs1;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dUseAs1;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dReqAs2;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dUseAs2;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dReinCTC;
		ar << DESIGN_STEELPRESTRESS_DATA.m_sCaseH1;
		ar << DESIGN_STEELPRESTRESS_DATA.m_sCaseH2;
		ar << DESIGN_STEELPRESTRESS_DATA.m_dPj;

		for(n=0; n<FLOOR_SIZE; n++)
		{
			ar << CALC_CANTILEVER_DATA[n].m_Nangan_dH; //���� ����
			ar << CALC_CANTILEVER_DATA[n].m_Nangan_dL;  //���� ����
		}

		ar << DESIGN_MATERIAL.m_sSigmaY;	// nFlag=10�� �߰�...
		ar << DESIGN_MATERIAL.m_dSigmaCi;	// nFlag=11�� �߰�...
		ar << m_bJungbunMugun;				// nFlag=12�� �߰�...	
		ar << DESIGN_MATERIAL.m_dBindSigmaCK;	// nFlag=20�� �߰�...TU
		ar << DESIGN_MATERIAL.m_nBindNc;		// nFlag=20�� �߰�...TU
		ar << DESIGN_MATERIAL.m_dBindEc;		// nFlag=20�� �߰�...TU

		ar << m_bShoeGirderLower;
		ar << DESIGN_MATERIAL.m_dDummyEst;
		ar << m_nModelingSettleMethod;
	
		ar << m_nLBDeckDiaIdx;				// LB-Deck ���� index
		ar << m_dLBDeckCTC;					// LB-Deck CTC
		ar << m_dLBDeckCover;				// LB-Deck �Ǻ�
	
	}
	else
	{		
		ar >> nFlag;
		m_nFlag = nFlag;
		ar >> DESIGN_BASE.m_szBridgeType;
		ar >> DESIGN_BASE.m_szBridgeLength;
		ar >> DESIGN_BASE.m_szBridgeLineType;
		ar >> DESIGN_BASE.m_dBridgeWidth;
		ar >> DESIGN_BASE.m_dBridgeSkew;

		ar >> DESIGN_CONDITION.m_szBridgeGrade;
		ar >> DESIGN_CONDITION.m_dPf;
		ar >> DESIGN_CONDITION.m_dPr;
		ar >> DESIGN_CONDITION.m_dW;
		ar >> DESIGN_CONDITION.m_dPm;
		ar >> DESIGN_CONDITION.m_dPs;
				
		ar >> DESIGN_MATERIAL.m_dSigmaCK;
		ar >> DESIGN_MATERIAL.m_dSigmaY;
		ar >> DESIGN_MATERIAL.m_dSigmaSA;
		ar >> DESIGN_MATERIAL.m_szSwsJu;
		ar >> DESIGN_MATERIAL.m_szSwsBu;
		ar >> DESIGN_MATERIAL.m_dEc;
		ar >> DESIGN_MATERIAL.m_dEs;
		ar >> DESIGN_MATERIAL.m_dEst;
		ar >> DESIGN_MATERIAL.m_nN;
		ar >> DESIGN_MATERIAL.m_dEpsilonS;
		ar >> DESIGN_MATERIAL.m_dAlpha;
		ar >> DESIGN_MATERIAL.m_dCreep;
		ar >> DESIGN_MATERIAL.m_dCreep2;
		ar >> DESIGN_MATERIAL.m_dTemparature;
		ar >> DESIGN_MATERIAL.m_dShearEs;
		
		if(nFlag > 22)
		{
			ar >> DESIGN_MATERIAL.m_dSigmaDeckSA;
			ar >> DESIGN_MATERIAL.m_dEci;
			ar >> DESIGN_MATERIAL.m_RatePoasong;
			ar >> DESIGN_MATERIAL.m_strAllow_PreStressPressPow;
			ar >> DESIGN_MATERIAL.m_strAllow_PreStressStretchPow;
			ar >> DESIGN_MATERIAL.m_strAllow_CreWeightPressPow;
			ar >> DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow;
			ar >> DESIGN_MATERIAL.m_strAllow_DeckConcStretchPow;
			ar >> DESIGN_MATERIAL.m_strCrackPow;
		}

		ar >> DESIGN_UNIT_WEIGHT.m_dGangjae;
		ar >> DESIGN_UNIT_WEIGHT.m_dAluminum;
		ar >> DESIGN_UNIT_WEIGHT.m_dChulgun;
		ar >> DESIGN_UNIT_WEIGHT.m_dMugun;
		ar >> DESIGN_UNIT_WEIGHT.m_dCement;
		ar >> DESIGN_UNIT_WEIGHT.m_dMokjae;
		ar >> DESIGN_UNIT_WEIGHT.m_dYukchung;
		ar >> DESIGN_UNIT_WEIGHT.m_dAsphalt;
		ar >> DESIGN_UNIT_WEIGHT.m_dGravel;

		// for(long n=0; n<FLOOR_SIZE; n++)
		for(n=0; n<FLOOR_SIZE; n++)
		{
			ar >> DESIGN_FLOOR_DATA[n].m_bBangEm;
			ar >> DESIGN_FLOOR_DATA[n].m_dBangEmWeight;
			ar >> DESIGN_FLOOR_DATA[n].m_dBangEmHeight;
			ar >> DESIGN_FLOOR_DATA[n].m_dBangEmDis;
			
			ar >> DESIGN_FLOOR_DATA[n].m_bNanGan;	
			ar >> DESIGN_FLOOR_DATA[n].m_dNanGanWeight;
			ar >> DESIGN_FLOOR_DATA[n].m_dNanGanDis;
			ar >> DESIGN_FLOOR_DATA[n].m_dNanGanHeight;
			
			ar >> DESIGN_FLOOR_DATA[n].m_bWalkLoad;	

			if(nFlag>2) ar >> DESIGN_FLOOR_DATA[n].m_bPeopleLoad;

			ar >> DESIGN_FLOOR_DATA[n].m_dWalkLoadWidth;
			ar >> DESIGN_FLOOR_DATA[n].m_dWalkLoad1;
			ar >> DESIGN_FLOOR_DATA[n].m_dWalkLoad2;

			if(nFlag>13) ar >> DESIGN_FLOOR_DATA[n].m_dWalkLoadMoment;

			ar >> DESIGN_FLOOR_DATA[n].m_dWalkLoadH;
			
			ar >> DESIGN_FLOOR_DATA[n].m_bBangHo;
			ar >> DESIGN_FLOOR_DATA[n].m_dBangHoWidth;	
			ar >> DESIGN_FLOOR_DATA[n].m_dBangHoWeight;	
		
			for(long a=0; a<HLLMAXSU; a++)
			{
				ar >> DESIGN_FLOOR_DATA[n].m_dBangHoLEdge[a];	
				ar >> DESIGN_FLOOR_DATA[n].m_dBangHoREdge[a];	
				ar >> DESIGN_FLOOR_DATA[n].m_dWalkLoadLEdge[a];	
				ar >> DESIGN_FLOOR_DATA[n].m_dWalkLoadREdge[a];	
			}
		}

		for(n=0; n<FLOOR_SIZE; n++)
		{
			ar >> CALC_CANTILEVER_DATA[n].m_Min_T;		
			ar >> CALC_CANTILEVER_DATA[n].m_Req_T;		

			ar >> CALC_CANTILEVER_DATA[n].m_Stop_M;	
			ar >> CALC_CANTILEVER_DATA[n].m_Stop_Ton;	
			ar >> CALC_CANTILEVER_DATA[n].m_StopE_M;	
			ar >> CALC_CANTILEVER_DATA[n].m_StopE_Ton;
			ar >> CALC_CANTILEVER_DATA[n].m_StopB_M;
			
			ar >> CALC_CANTILEVER_DATA[n].m_Move_L;	
		
			if (nFlag>14)
			{
				ar >> CALC_CANTILEVER_DATA[n].m_Tank_L;	
				ar >> CALC_CANTILEVER_DATA[n].m_Trailer_L;	
			}

			ar >> CALC_CANTILEVER_DATA[n].m_Dead_L;
			ar >> CALC_CANTILEVER_DATA[n].m_Move_E;   
			ar >> CALC_CANTILEVER_DATA[n].m_Dead_E;
			ar >> CALC_CANTILEVER_DATA[n].m_Move_E3;  
			ar >> CALC_CANTILEVER_DATA[n].m_Move_i;   
			ar >> CALC_CANTILEVER_DATA[n].m_Move_Pr;  
			ar >> CALC_CANTILEVER_DATA[n].m_Move_Ml;
			
			if(nFlag>16)
				ar >> CALC_CANTILEVER_DATA[n].m_Move_Ml_;

			ar >> CALC_CANTILEVER_DATA[n].m_MoveB_Ml;
			ar >> CALC_CANTILEVER_DATA[n].m_MoveB_Ml_Tank;
			ar >> CALC_CANTILEVER_DATA[n].m_MoveB_Ml_Trailer;
			ar >> CALC_CANTILEVER_DATA[n].m_Move_Ml_Tank;
			ar >> CALC_CANTILEVER_DATA[n].m_Move_Ml_Trailer;
			ar >> CALC_CANTILEVER_DATA[n].m_Move_GiJumsu;
			ar >> CALC_CANTILEVER_DATA[n].m_Move_M;  

			ar >> CALC_CANTILEVER_DATA[n].m_Coll_V;	
			ar >> CALC_CANTILEVER_DATA[n].m_Coll_H;   
			ar >> CALC_CANTILEVER_DATA[n].m_Coll_Co;  
			ar >> CALC_CANTILEVER_DATA[n].m_Coll_Mo;  
			ar >> CALC_CANTILEVER_DATA[n].m_Nangan_VerF; 
			ar >> CALC_CANTILEVER_DATA[n].m_Nangan_HoriF;

			ar >> CALC_CANTILEVER_DATA[n].m_Wind_Pw;  
			ar >> CALC_CANTILEVER_DATA[n].m_Wind_Mw;  
			ar >> CALC_CANTILEVER_DATA[n].m_Wind_H;   

			ar >> CALC_CANTILEVER_DATA[n].m_Cent_R;	
			ar >> CALC_CANTILEVER_DATA[n].m_Cent_Pcf; 
			ar >> CALC_CANTILEVER_DATA[n].m_Cent_Pcf_Tank;
			ar >> CALC_CANTILEVER_DATA[n].m_Cent_Pcf_Trailer;
			ar >> CALC_CANTILEVER_DATA[n].m_Cent_Mcf; 
			ar >> CALC_CANTILEVER_DATA[n].m_Cent_Mcf_Tank;
			ar >> CALC_CANTILEVER_DATA[n].m_Cent_Mcf_Trailer;
			ar >> CALC_CANTILEVER_DATA[n].m_CentB_Mcf;
			ar >> CALC_CANTILEVER_DATA[n].m_CentB_Mcf_Tank;
			ar >> CALC_CANTILEVER_DATA[n].m_CentB_Mcf_Trailer;
			
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Mu;	 
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Md;	 
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_a;    
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_b;    
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_d;    
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_CaseH1;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_CaseH2;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_CaseH3;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_CaseH4;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Ctc1;   
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Ctc2;   
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_CtcB;   
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_CtcB1;  
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_UseAs;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_UseAs2;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_UseAsBr;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_BAsuse; 
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_BAsuse2;
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Asreq;  
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_BAsreq; 
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Pused;  
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_dia;    
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Combo1;	  
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Combo2;    
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Combo3;    
			ar >> CALC_CANTILEVER_DATA[n].m_Rein_Combo4;    
			ar >> CALC_CANTILEVER_DATA[n].m_Crack_w;     
			ar >> CALC_CANTILEVER_DATA[n].m_Crack_wa;
			ar >> CALC_CANTILEVER_DATA[n].m_Crack_wBr;
			ar >> CALC_CANTILEVER_DATA[n].m_Crack_waBr;
		}

		ar >> m_Pib;		 
		ar >> m_Pis;		 
		ar >> m_dCoverUp;  
		ar >> m_dCoverDn;  
		ar >> m_dCoverSd;  
		ar >> m_dRebarPmax;   
		ar >> m_Rein_Arrange;
		ar >> m_Asreq;       
		ar >> m_Ag;          
		ar >> m_AsEa1;       
		ar >> m_AsEa2;       
		ar >> m_Asuse1;      
		ar >> m_Asuse2;	  
		ar >> m_Asuset;      
		ar >> m_Ju_ratio;    
		ar >> m_dia1;        
		ar >> m_dia2;        
		ar >> m_Ctc1;        
		ar >> m_Ctc2;        
		ar >> m_CaseH1;     
		ar >> m_CaseH2;     
		ar >> m_ComboInnerJijumSlab1;
		ar >> m_ComboInnerJijumSlab2;

		ar >> m_dSpeed;
		ar >> m_bTank;
		ar >> m_bTrailer;
		ar >> m_bMugun;
		ar >> m_bExtraLoad;
		ar >> m_bWind;

		m_ELoadTitleArr.Serialize(ar);
		m_ELoadWeightArr.Serialize(ar);
		m_ELoadDisArr.Serialize(ar);
		m_szRefArray.Serialize(ar);		

		ar >> m_bIsFirst;				
		ar >> m_bHLLError;				
		ar >> m_bLWalkHLL;				
		ar >> m_bRWalkHLL;	
		
		BOOL dTemp;	

		ar >> dTemp;	// ���߿� ����				
		ar >> m_bNoPojang;				

		ar >> m_HLType;					
		ar >> m_HLsu;					
		ar >> m_nHLiveLoadType;	

		if(nFlag>3)
			ar >> m_bNoDistOnNoGuard;

		ar >> m_bMakeModeling;
		ar >> m_bPassedCalcFloorDlg;
		ar >> m_bCheckSettlement;		
		if(nFlag>24) ar >> m_bReadFrameModelingOutput;
		if(nFlag>30) ar >> m_bRunCrossWind;

		ar >> m_CurHL;						
		ar >> m_CurLiveHL;		

		ar >> m_dDesignLaneWidth;	
		ar >> m_dUserLaneWidth;				
		ar >> nVal;//m_ChasunSu;		
		ar >> m_DesignChasunSu;
		ar >> m_DesignChasunSuR;

		ar >> m_AddRowTRL;		
		ar >> m_MaxImpact;				
		ar >> m_DBWidth;				
		ar >> m_DBFromEdge;			
		ar >> m_DBDis;					
		ar >> m_DLDis;	

		ar >> m_bTank;
		ar >> m_bTrailer;
		ar >> m_TrailWidth;
		ar >> m_TrailDis;
		ar >> m_TankWidth;
		ar >> m_TankDis;

		ar >> m_TankWeight;
		ar >> m_TankEdit1;
		ar >> m_TankEdit2;
		ar >> m_TankEdit3;
		ar >> m_TankImpact;

		ar >> m_TrailEdit1;
		ar >> m_TrailEdit2;
		ar >> m_TrailEdit3;
		ar >> m_TrailEdit4;
		ar >> m_TrailEdit5;
		ar >> m_TrailEdit6;
		ar >> m_TrailImpact;

		ar >> m_TrailP1;
		ar >> m_TrailP2;
		ar >> m_TrailP3;

		if(nFlag<30)
			m_dStationHLLPos.SetSize(20);
		for(n=0; n<HLLMAXSU; n++)
		{
			m_DArrDistanceDB[n].Serialize(ar);
			m_DArrDistanceDL[n].Serialize(ar);

			m_DArrDBChasun[n].Serialize(ar);
			m_DArrDLChasun[n].Serialize(ar);
			
			m_DArrMaxDBChasun[n].Serialize(ar);
			m_DArrMaxDLChasun[n].Serialize(ar);
			
			m_DArrDistanceTank[n].Serialize(ar);
			m_DArrDistanceTrail[n].Serialize(ar);

			ar >> m_dLeftWindFroceFromL[n];			
			ar >> m_dLeftWindFroceFromR[n];			
			ar >> m_dRightWindFroceFromL[n];			
			ar >> m_dRightWindFroceFromR[n];			
			ar >> m_dWindForceToGirderL[n];			
			ar >> m_dWindForceToGirderR[n];			
			ar >> m_dLeftWindMomentFromL[n];			
			ar >> m_dLeftWindMomentFromR[n];			
			ar >> m_dRightWindMomentFromL[n];			
			ar >> m_dRightWindMomentFromR[n];
			if(nFlag<30)
				ar >> m_dStationHLLPos[n];
		}
		if(nFlag<30)
			m_dStationHLLPos.SetSize(m_HLsu);
		else
			m_dStationHLLPos.Serialize(ar);

		ar >> m_AddLoad;									
		ar >> m_TotalWeight;								
		ar >> m_ModelWeight;								
		ar >> m_BonusRate;									
		ar >> m_JijumDownSize;								
		ar >> m_bApplyAddRow;								
		ar >> m_bPic;	
		ar >> m_bReadLiveDispOutput;
		ar >> m_szFolderPath;

		long	nArrsu = 0, nG = 0;
		
		CString cs		= _T("");
		double	dSta	= 0.0;

		ar >> nArrsu;

		for(n=0; n<nArrsu; n++)
		{
			ar >> cs;	m_ArrExtraSecTitle.Add(cs);
			ar >> nG;	m_ArrExtraSecGir.Add(nG);
			ar >> dSta;	m_ArrExtraSecSta.Add(dSta);
		}		

		ar >> m_dMatMinSpace;
		if(nFlag>25)
			ar >> m_dMatMaxSpace;

		ar >> m_nTypePos;									// ����������ġ(A1, A2)
		ar >> m_nTypeResist[0];							    // ������������
		ar >> m_nTypeResist[1];							    // ������������
		ar >> m_dLenSinchukGir[0];							// ���� �麸�� �Ÿ�
		ar >> m_dLenSinchukGir[1];							// ���� �麸�� �Ÿ�
		ar >> m_dHeightGir[0];								// ������ ���� h :
		ar >> m_dHeightGir[1];								// ������ ���� h :
		ar >> m_RotAngle;									// ȸ���� ��:
		ar >> m_Bfa;										// ���� ��� b :
		ar >> m_Tfa;										// �µ���ȭ�� ��T :
		ar >> m_Afa;										// ����â��� a :	
		ar >> m_D1[0];										// ��ġ������ d1 :
		ar >> m_D1[1];										// ��ġ������ d1 :
		ar >> m_D2[0];										// �ΰ� ������ d2 :
		ar >> m_D2[1];										// �ΰ� ������ d2 :
		ar >> m_MaxUgan[0];									// �ִ�����
		ar >> m_MaxUgan[1];									// �ִ�����
		ar >> m_MinUgan[0];									// �ּ�����
		ar >> m_MinUgan[1];									// �ּ�����
		ar >> m_szUseJoint[0];								// ����ƮŸ��  
		ar >> m_szUseJoint[1];								// ����ƮŸ��

		ar >> DESIGN_STEELPRESTRESS_DATA.m_nSteelPrestressIdx;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_szSteelPrestressName;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dEs;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dArea;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dDiameter;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dTensionLoad;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dLoadOfElongation;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction;	///< �Ļ������
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction;	///< ����������
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nDuctType;			///< ��Ʈ ����
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;		///< PS���� ����
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;		///< ����
			
		if(nFlag > 22)
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip;	///< ������ Ȱ����
		else
		{
			ar >> dTemp1;	//2009���� - ������ Ȱ����(������)
			ar >> dTemp1;	//2009���� - ������ Ȱ����(������)

		}

		ar >> DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dHumidity;			///< ��ջ�����
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nFixingPlateType;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nVerRein;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nHorRein1;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nHorRein2;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dVerReinAs;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs1;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs2;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty;	
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dFixingPlateLength;
		ar >> m_dRadiusOfCurvature;	///< ���������

		ar >> DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength;
		ar >> DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;
		
		if(nFlag>1)
			ar >> m_bBucklingStress;

		if(nFlag > 22)
		{
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP;	//PS ������ ������ ���� ���(��p)
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dSheathW;	//������ ��
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dSheathH;	//������ ����
			ar >> DESIGN_STEELPRESTRESS_DATA.m_sJackMax;	//Jack�� ���� �ִ� ��������

			// 0021410  �� ���� ���� - ������ �� ��Ʈ������ ���� ����� ���� ���� 
			if(DESIGN_STEELPRESTRESS_DATA.m_sJackMax == _T("0.70 fpy"))
				DESIGN_STEELPRESTRESS_DATA.m_sJackMax = _T("0.70��fpu");
			else if(DESIGN_STEELPRESTRESS_DATA.m_sJackMax == _T("0.83 fpy"))
				DESIGN_STEELPRESTRESS_DATA.m_sJackMax = _T("0.83��fpy");
			else if( DESIGN_STEELPRESTRESS_DATA.m_sJackMax == _T("0.90 fpy"))
				DESIGN_STEELPRESTRESS_DATA.m_sJackMax = _T("0.90��fpy");
		}
		if (nFlag>23)
		{		
			ar >> DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_nVerRein2;
		}
		else
		{
			DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2	= 4;
			DESIGN_STEELPRESTRESS_DATA.m_nVerRein2		= 0;
		}

		if(nFlag>4)
		{
			for(n=0; n<FLOOR_SIZE; n++)
			{
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_CaseH1_2Cy;	// ö������(�Ϲݺ� ��ö�� 2-Cycle)
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_CaseH2_2Cy;	// ö������(�ܺ�   ��ö�� 2-Cycle)
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Combo1_2Cy;	// �Ϲݺ� ��ö��Index 2-Cycle
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Combo2_2Cy;	// �ܺ�   ��ö��Index 2-Cycle
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_UseAs_1Cy;		// ���ö�ٷ�(�Ϲݺ� ��ö�� 1-Cycle)
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_UseAs_2Cy;		// ���ö�ٷ�(�Ϲݺ� ��ö�� 2-Cycle)
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_UseAs2_1Cy;	// ���ö�ٷ�(�ܺ�   ��ö�� 1-Cycle)
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_UseAs2_2Cy;	// ���ö�ٷ�(�ܺ�   ��ö�� 2-Cycle)
				ar >> CALC_CANTILEVER_DATA[n].m_bCheckGen1;			// �Ϲݺ� ��ö��   ����
				ar >> CALC_CANTILEVER_DATA[n].m_bCheckGen2;			// �Ϲݺ� ���ö�� ����
				ar >> CALC_CANTILEVER_DATA[n].m_bCheckDan1;			// �ܺ�   ��ö��   ����
				ar >> CALC_CANTILEVER_DATA[n].m_bCheckDan2;			// �ܺ�   ���ö�� ����
			}
			ar >> m_bCheckInnerJijum;
			if(nFlag > 28)
				ar >> m_nEndPartRebarConditionIdx;
		}
		
		if(nFlag>5)
		{
			for(n=0; n<FLOOR_SIZE; n++)
			{
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_CaseH1_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_CaseH1_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Combo1_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Combo1_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Ctc1;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse1_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse1_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse1;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Asreq1;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_CaseH2;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Combo2;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Ctc2;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Asuse2;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Up_Asreq2;
				ar >> CALC_CANTILEVER_DATA[n].m_bCheck_Up;
				ar >> CALC_CANTILEVER_DATA[n].m_bCheck_UpB;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Lo_Asreq;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Lo_BAsreq;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Lo_Asuse;
				ar >> CALC_CANTILEVER_DATA[n].m_Rein_Lo_BAsuse;

				ar >> CALC_CANTILEVER_DATA[n].m_strGenReinTDia_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_strGenReinTDia_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_nGenReinTDiaIdx_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_nGenReinTDiaIdx_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinTCTC;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinTAsreq;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinTAsuse;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinTAsuse_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinTAsuse_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_strGenReinCDia_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_strGenReinCDia_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_nGenReinCDiaIdx_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_nGenReinCDiaIdx_2Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinCCTC;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinCAsreq;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinCAsuse;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinCAsuse_1Cy;
				ar >> CALC_CANTILEVER_DATA[n].m_dGenReinCAsuse_2Cy;
			}
		}

		if(nFlag > 6)
		{
			ar >> DESIGN_STEELPRESTRESS_DATA.m_nAnchorageEA;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dSpallingForce;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dBurstingForce;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dReqAs1;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dUseAs1;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dReqAs2;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dUseAs2;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dReinCTC;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_sCaseH1;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_sCaseH2;
			ar >> DESIGN_STEELPRESTRESS_DATA.m_dPj;			
		}
		
		if(nFlag > 8)
		{
			for(n=0; n<FLOOR_SIZE; n++)
			{
				ar >> CALC_CANTILEVER_DATA[n].m_Nangan_dH; 
				ar >> CALC_CANTILEVER_DATA[n].m_Nangan_dL;
			}
		}

		if(nFlag>9)
			ar >> DESIGN_MATERIAL.m_sSigmaY;
		else
		{
			if(DESIGN_MATERIAL.m_dSigmaY>=400)
				DESIGN_MATERIAL.m_sSigmaY = "H";
			else
				DESIGN_MATERIAL.m_sSigmaY = "D";
		}
		
		if(nFlag>10)
			ar >> DESIGN_MATERIAL.m_dSigmaCi;
		else
			DESIGN_MATERIAL.m_dSigmaCi = 28;

		if(nFlag>11)		
			ar >> m_bJungbunMugun;	
		
		if(nFlag>19)
		{
			ar >> DESIGN_MATERIAL.m_dBindSigmaCK;	// nFlag=20�� �߰�...TU
			ar >> DESIGN_MATERIAL.m_nBindNc;		// nFlag=20�� �߰�...TU
			ar >> DESIGN_MATERIAL.m_dBindEc;		// nFlag=20�� �߰�...TU
		}

		if(nFlag>20)
			ar >> m_bShoeGirderLower;

		if(nFlag>21)
			ar >> DESIGN_MATERIAL.m_dDummyEst;
		else
			DESIGN_MATERIAL.m_dDummyEst = DESIGN_MATERIAL.m_dEst;
		if(nFlag>26)
			ar >> m_nModelingSettleMethod;

		if(nFlag > 27)
		{
			ar >> m_nLBDeckDiaIdx;				// LB-Deck ���� index
			ar >> m_dLBDeckCTC;					// LB-Deck CTC
			ar >> m_dLBDeckCover;				// LB-Deck �Ǻ�
		}
	} 

	if(nFlag<26 && !ar.IsStoring())
	{	
		CDoubleArray m_ArrMaxM [GIRDERSUMAX];					// + ���׸��Ʈ
		CDoubleArray m_ArrMaxMSta[GIRDERSUMAX];				// + ���׸��Ʈ�� station(���۰� ��)
		CDoubleArray m_ArrMinM [GIRDERSUMAX];					// - ���׸��Ʈ
		CDoubleArray m_ArrMinMSta[GIRDERSUMAX];				// - ���׸��Ʈ�� station(���۰� ��)
		CDWordArray  m_ArrMaxByun[GIRDERSUMAX];				// ���ܸ��� ����
		CDWordArray  m_ArrMinByun[GIRDERSUMAX];				// ���ܸ��� ����
		CDWordArray  m_ArrMaxMMatId[GIRDERSUMAX];				// �ܸ� ��ȣ
		CDWordArray  m_ArrMinMMatId[GIRDERSUMAX];				// �ܸ� ��ȣ
		if(nFlag<16 && !ar.IsStoring())
		{	
			for(long n=0; n<10/*GIRDERSUMAX*/; n++)			// �ִ� �Ŵ��� 10���� 20���� Ȯ�� (2007.10.23 Sdy)
			{			
				m_ArrMaxM[n].Serialize(ar);		m_ArrMaxM[n].RemoveAll();
				m_ArrMaxMSta[n].Serialize(ar);	m_ArrMaxMSta[n].RemoveAll();
				m_ArrMinM [n].Serialize(ar);	m_ArrMinM[n].RemoveAll();
				m_ArrMinMSta[n].Serialize(ar);	m_ArrMinMSta[n].RemoveAll();	
				m_ArrMaxByun[n].Serialize(ar);  m_ArrMaxByun[n].RemoveAll(); 
				m_ArrMinByun[n].Serialize(ar);  m_ArrMinByun[n].RemoveAll();
				m_ArrMaxMMatId[n].Serialize(ar);m_ArrMaxMMatId[n].RemoveAll();
				m_ArrMinMMatId[n].Serialize(ar);m_ArrMinMMatId[n].RemoveAll();
			}
		}

		m_ArrMomentZeroSta.Serialize(ar);	// �������� ��ġ
	
		CDoubleArray m_ArrSecSta[10];				// ����ܸ��� �����̼�	
		CDWordArray  m_ArrSecIndex[10];				// ����ܸ��� �ܸ��ȣ	
		CDWordArray  m_ArrSecByunType[10];			// ����ܸ��� ���ܸ����
		for(long n=0; n<10/*GIRDERSUMAX*/; n++)				// �ִ� �Ŵ��� 10���� 20���� Ȯ�� (2007.10.23 Sdy)
		{
			m_ArrSecSta[n].Serialize(ar);
			m_ArrSecIndex[n].Serialize(ar);
			m_ArrSecByunType[n].Serialize(ar);   // ���ܸ� ����
		}
	}

	m_ArrSec.Serialize(ar);
	m_ArrSecSpDesign.Serialize(ar);
	m_ArrSecHBeam.Serialize(ar);							// H��������
	m_ArrSecHBeamLSp.Serialize(ar);							// H��������(������ ������ �����ΰ��)
	m_ArrSecHBeamRSp.Serialize(ar);							// H��������(������ ������ �����ΰ��)
	m_ArrSecHBeamLRSp.Serialize(ar);						// H��������(��,������ ������ �����ΰ��)
	m_ArrSecCross.Serialize(ar);							// ���Ʈ�������� ���� ���κ���ġ
	m_ArrSecCrossMoment.Serialize(ar);						// ���Ʈ�������� �ִ� ���κ���ġ		
	m_ArrSecCrossWithStg.Serialize(ar);						// ���Ʈ�������� ���� ���κ����� ���κ���ġ
	m_ArrSecCrossWithStgMoment.Serialize(ar);				// ���Ʈ�������� �ִ� ���κ����� ���κ���ġ
	m_ArrSecStringer.Serialize(ar);							// ���κ���ġ
	m_ArrSecVBracing.Serialize(ar);
	m_ArrSecHBracing.Serialize(ar);
	m_sz3DFrameOutFilePath.Serialize(ar);

	if(nFlag > 7)
		m_ArrSecCrossConnectionPlate.Serialize(ar);

	if(nFlag>12)//������������ ���򺸰��� �ڵ���ġ�� ���� �迭 �ΰ��� ����...
	{
		m_ArrMaxZeroSta.Serialize(ar);
		m_ArrMinZeroSta.Serialize(ar);
	}

	if(nFlag<26 && !ar.IsStoring())
	{	
		CDoubleArray m_ArrMaxM [GIRDERSUMAX];					// + ���׸��Ʈ
		CDoubleArray m_ArrMaxMSta[GIRDERSUMAX];				// + ���׸��Ʈ�� station(���۰� ��)
		CDoubleArray m_ArrMinM [GIRDERSUMAX];					// - ���׸��Ʈ
		CDoubleArray m_ArrMinMSta[GIRDERSUMAX];				// - ���׸��Ʈ�� station(���۰� ��)
		CDWordArray  m_ArrMaxByun[GIRDERSUMAX];				// ���ܸ��� ����
		CDWordArray  m_ArrMinByun[GIRDERSUMAX];				// ���ܸ��� ����
		CDWordArray  m_ArrMaxMMatId[GIRDERSUMAX];				// �ܸ� ��ȣ
		CDWordArray  m_ArrMinMMatId[GIRDERSUMAX];				// �ܸ� ��ȣ
		CDoubleArray m_ArrSecSta[GIRDERSUMAX];				// ����ܸ��� �����̼�	
		CDWordArray  m_ArrSecIndex[GIRDERSUMAX];				// ����ܸ��� �ܸ��ȣ	
		CDWordArray  m_ArrSecByunType[GIRDERSUMAX];			// ����ܸ��� ���ܸ����
			
		for(long n=10; n<GIRDERSUMAX; n++)
		{			
			m_ArrMaxM[n].Serialize(ar);
			m_ArrMaxMSta[n].Serialize(ar);
			m_ArrMinM [n].Serialize(ar);
			m_ArrMinMSta[n].Serialize(ar);
			m_ArrMaxByun[n].Serialize(ar);
			m_ArrMinByun[n].Serialize(ar);
			m_ArrMaxMMatId[n].Serialize(ar);
			m_ArrMinMMatId[n].Serialize(ar);
			m_ArrSecSta[n].Serialize(ar);
			m_ArrSecIndex[n].Serialize(ar);
			m_ArrSecByunType[n].Serialize(ar);			
		}
	}
}


CString CCalcData::GetFixingPlateType(long nType)
{
	if(nType==0)	return _T("VSL type S6-4");

	return _T("");
}

long CCalcData::GetFixingPlateType(CString strFixingPlateType)
{
	if(strFixingPlateType == _T("VSL type S6-4"))	return	 0;

	return -1;
}
//Jack�� ���� �ִ� �������� ��갪
double CCalcData::GetJackMaxStrToDouble(CString strFormula)
{
	double dPSfty = GetPSfty(strFormula);
	double dFpu		= DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength;
	double dFpy		= DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;
	double dValue	= 0.0;
	
	if(strFormula.Find("fp")!=-1)
	{
		if(dPSfty < 0.83)	dValue	= dPSfty * dFpu;
		else				dValue	= dPSfty * dFpy;
	}
	else
		dValue = dPSfty;	

	return dValue;
}

double CCalcData::GetPSfty(CString strFormula)
{
	BOOL bNumber = TRUE;
	CString strTemp = _T("");
	double dPSfty = 0.0;
	for(long nIdx =0; nIdx < strFormula.GetLength(); nIdx++)
	{
		char cTemp = strFormula.GetAt(nIdx);

		if(strFormula.GetAt(nIdx) < 0)
		{
			break;
		}
		if(!isdigit(strFormula.GetAt(nIdx)) && strFormula.GetAt(nIdx) != '.')
		{
			bNumber = FALSE;
			break;
		}
		strTemp += cTemp;
	}

	if(strFormula.Find("fpy") == -1)
	{
		//����� ����
		if(bNumber)	return atof(strFormula);
		else		return 0.0;

	}

	strFormula = strTemp;
	dPSfty = atof(strFormula);

	return dPSfty;
}

double CCalcData::GetConstPSfpu()
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
	long nStandYear = pOpt->GetSteelStandardYear();
	double dConstPSfpu = 0.0;
	if(nStandYear == APPLYYEAR2010)
		dConstPSfpu = 0.74;
	else
		dConstPSfpu = 0.70;
	return dConstPSfpu;
}

double CCalcData::GetConstPSfy()
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
	long nStandYear = pOpt->GetSteelStandardYear();
	double dConstPSfy = 0.0;
	if(nStandYear == APPLYYEAR2010)
		dConstPSfy = 0.82;
	else
		dConstPSfy = 0.83;
	return dConstPSfy;
}

CString CCalcData::GetStrConstPSfpu()
{
	CString strConstPSfy = _T("");
	double dConstPSfpu = GetConstPSfpu();
	strConstPSfy.Format(_T("%0.2f��fpu"), dConstPSfpu);
	return strConstPSfy;
}

CString CCalcData::GetStrConstPSfy()
{
	CString strConstPSfy = _T("");
	double dConstPSfy = GetConstPSfy();
	strConstPSfy.Format(_T("%0.2f��fy"), dConstPSfy);
	return strConstPSfy;
}

void CCalcData::GetDefaultConstPSf(CStringArray &strArrConstPSf)
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
	long nStandYear = pOpt->GetSteelStandardYear();

	strArrConstPSf.RemoveAll();

	if(nStandYear == APPLYYEAR2010)
		strArrConstPSf.Add(_T("0.74��fpu"));
	else
		strArrConstPSf.Add(_T("0.70��fpu"));

	strArrConstPSf.Add(_T("0.83��fpy"));
	strArrConstPSf.Add(_T("0.90��fpy"));
	strArrConstPSf.Add(_T("����� ����"));
}

void CCalcData::SetDefaultAllowDesignMaterials()
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
	long nStandYear = pOpt->GetSteelStandardYear();

	

	if(nStandYear == APPLYYEAR2010)
	{
		// #0028192 - ������� 2010 ���ý� �⺻�� ����

		DESIGN_MATERIAL.m_strAllow_PreStressStretchPow	= _T("0.25 ��(fci)");			//������Ʈ���� ���� ���� ��������
		DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow  = _T("0.63 ��(fck)");	
	}
	else
	{
		DESIGN_MATERIAL.m_strAllow_PreStressStretchPow	= _T("1.4");	//������Ʈ���� ���� ���� ��������
		DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow  = _T("0.50 ��(fck)");	//���� ���� �ۿ�� ��������
	}

	DESIGN_MATERIAL.m_strAllow_DeckConcStretchPow   = _T("0");				//�ٴ��� ��� �� ��������
	DESIGN_MATERIAL.m_strCrackPow					= _T("0.63 ��(fck)");	//�տ� ����

	double dConstPreStressComp = GetConstPreStressCompress();
	double dConstDesignLoadComp = GetConstDesignLoadCompress();
	CString strText = _T("");

	strText.Format(_T("%0.2f (fci)"), dConstPreStressComp);
	DESIGN_MATERIAL.m_strAllow_PreStressPressPow = strText;
	strText.Format(_T("%0.2f (fck)"), dConstDesignLoadComp);	//���� ���� �ۿ�� ��������
	DESIGN_MATERIAL.m_strAllow_CreWeightPressPow = strText;
}

double CCalcData::GetConstPreStressCompress()
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
	long nStandYear = pOpt->GetSteelStandardYear();
	double dValue = 0;
	if(nStandYear == APPLYYEAR2010)
		dValue = 0.6;
	else
		dValue = 0.55;
	return dValue;
}

double CCalcData::GetConstDesignLoadCompress()
{
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();
	long nStandYear = pOpt->GetSteelStandardYear();
	double dValue = 0;
	if(nStandYear == APPLYYEAR2010)
		dValue = 0.45;
	else
		dValue = 0.40;
	return dValue;
}

double CCalcData::GetConstPreStressStretch()
{
	double dValue = 0;

	CString strValue = DESIGN_MATERIAL.m_strAllow_PreStressStretchPow;

	if(strValue.Find("0.50") > -1)
		dValue = 0.5;
	else if(strValue.Find("0.25") > -1)
		dValue = 0.25;
	else if(strValue.Find("1.4") > -1)
		dValue = 1.4;

	return dValue;
}

double CCalcData::GetConstDesignLoadStretch()
{
	double dValue = 0;

	CString strValue = DESIGN_MATERIAL.m_strAllow_CreWeightStretchPow;

	if(strValue.Find("0.50") > -1)
		dValue = 0.5;
	else if(strValue.Find("0.25") > -1)
		dValue = 0.25;
	else if(strValue.Find("0.63") > -1)
		dValue = 0.63;

	return dValue;
}