// PlateCrossBeam.cpp: implementation of the CPlateCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateCrossBeam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlateCrossBeam::CPlateCrossBeam() : m_pSection(NULL)
{
	m_nLevelType			= 0;
	m_pVStiff				= NULL;
	m_pSteelMaterialData	= NULL;
}

CPlateCrossBeam::CPlateCrossBeam(CSection *pSection) : m_pSection(pSection)
{
	InitData();
	m_pVStiff				= new CVStiff(pSection);
	m_pSteelMaterialData	= new CSteelMaterialData();
	m_pSteelMaterialData->m_nType		= 0;
	m_pSteelMaterialData->m_nJewonIdx	= 0;
}

CPlateCrossBeam::~CPlateCrossBeam()
{
	if(m_pVStiff)
		delete m_pVStiff;
	if(m_pSteelMaterialData)
		delete	m_pSteelMaterialData;
}

CPlateBasicIndex *CPlateCrossBeam::GetBx()
{ 
	return (CPlateBasicIndex *)m_pSection->GetBx(); 
}

void CPlateCrossBeam::InitData()
{
	m_nLevelType = 0;
	m_nLGirSpType = TRUE;
	m_nRGirSpType = TRUE;
	m_nType = 0;
	//
	m_dbL1 = 50;
	m_dbL2 = 50;
	m_dbL3 = 50;
	m_dbL4 = 50;
	m_dbR1 = 50;
	m_dbR2 = 50;
	m_dbR3 = 50;
	m_dbR4 = 50;
	m_dLH1 = 200;
	m_dLH2 = 500;
	m_dRH1 = 200;
	m_dRH2 = 500;
	m_dwB = 20;
	m_dwR = 20;
	m_dwH = 20;
	m_wE = 5;
	m_wF = 100;
	m_dltH = 400;
	m_ltE = 5;
	m_ltF = 100;
	m_drtH = 400;
	m_rtE = 5;
	m_rtF = 100;
	//
	m_P_CR		=  500;
	m_H_CR		= 1200;
	m_T1_CR		=  12;
	m_T2_CR		=  12;
	m_T3_CR		=  12;
	m_uM_CR		= 100;
	m_uN_CR		= 100;
	m_dM_CR		= 100;
	m_dN_CR		= 100;
	//
	double YonDan = 50;
	double CTerm  = 50;
	//
	m_uA	= YonDan;
	if( (m_uA+2*m_uC+m_uC1/2+20) <= m_P_CR ) m_uB = 2;
	else m_uB = 1;
	m_uC	= CTerm;
	m_uC1	= CTerm;
	m_uD	= (m_uM_CR-m_T3_CR/2-20)/2;
	m_uE	= 0;
	m_uF	= 0;
	m_uW	= m_uD*2;
	m_uT	= 10;
	//
	m_dA	= m_uA;
	m_dB	= m_uB;
	m_dC	= m_uC;
	m_dC1	= m_uC1;
	m_dD	= m_uD;
	m_dE	= 0;
	m_dF	= 0;
	m_dW	= m_uW;
	m_dT	= m_uT;
	//
	m_sA	= m_uA;
	m_sB	= m_uB-1;
	m_sC 	= m_uC;
	m_sC1	= m_uC1;
	// 
	double tmp_mG = 40;
	m_sF	= 100;
	m_sE	= (long)((m_H_CR-tmp_mG*2)/m_sF-1);
	m_sW	= m_sE*m_sF + m_sA*2;
	m_sT	= m_T3_CR;
	m_dL = 80; 
	m_uL = 80;
	m_sL = 80;

	double GTerm  = 2000;//pGir->GetDisGirderToGirder(pBx); 
	double  l  = GTerm - m_P_CR*2,
			l1 = l - ((m_sA+m_sB*m_sC)*2);
	m_nVStiffsu_CR = max((long)(Round(l1/1000,0)-1),0);
	//
	long n = 0;
	for(n = 0;n < m_nVStiffsu_CR;n++)
	{
		m_VStiffDis_CR[n] = l1/(m_nVStiffsu_CR+1);
		if( n == 0 ) m_VStiffDis_CR[n] += (l-l1)/2;
	}

	// 슬래브 앙카는 수직보강재의 배치를 따른다
	m_nAnchorsu_CR = m_nVStiffsu_CR;
	for(n = 0; n < m_nAnchorsu_CR;n++)
		m_AnchorDis_CR[n] = m_VStiffDis_CR[n];

	if( m_nVStiffsu_CR )
	{
		m_vW_CR = Round(m_dM_CR-(m_T3_CR/2+10),-1);
		m_vT_CR = m_T3_CR;
		m_vL_CR = m_H_CR;
	}

	m_enTypeNum		= 0	;
	m_H_CR			= 0	;			//가로보 높이
	m_T1_CR			= 0	;			//가로보 하판 두께
	m_T2_CR			= 0	;			//가로보 상판 두께
	m_T3_CR			= 0	;			//가로보 복부판 두께
	m_P_CR			= 0	;			//브라켓 돌출 길이
	m_uN_CR			= 0	;			//상판 브라켓
	m_uM_CR			= 0	;			//상판 너비/2
	m_dN_CR			= 0	;			//하판 브라켓
	m_dM_CR			= 0	;			//하판 너비/2
	m_X_CR			= 0	;
	m_vW_CR			= 0	;			//수직보강재
	m_vT_CR			= 0	;
	m_vL_CR			= 0	;
	m_hW_CR			= 0	;			//수평보강재
	m_hT_CR			= 0	;
	m_nVStiffsu_CR	= 0	;
	m_nQtyStringer_CR = 0;
	m_nAnchorsu_CR	= 0	;
	m_nHStiffUppersu_CR	= 0;
	m_nHStiffLowersu_CR	 = 0;
	m_cVSide_CR		= 0	;  //수직보강재 편측 적용
	m_cGasset_CR	= TRUE	;  //브라켓 보강재 유무
	for(long m = 0;m < 9;m++){
		m_VStiffDis_CR[m] = 0;
		m_AnchorDis_CR[m] = 0;
	}
	m_vT_G  = 12;
	m_vW_G  = 150;
	m_vW1_G = 0;

	m_bBoltInterver = FALSE;
	m_dBoltInterver = 80.0;
	m_bScallopUpperFlange = TRUE;
	m_bScallopLowerFlange = FALSE;
	m_nFillerUseType = FALSE;
	m_dThickFiller = 0;//초기값...

	m_bMomentPan = 0;
	m_bConnectionPlate = FALSE;
	m_nRadioConnectionPlateInOut = 0;
	m_bHunchTopConnect = 0;


	m_uM1 = 0;
	m_uD1 = 0;
	m_uE1 = 0;
	m_uF1 = 0;
	m_uB1 = 0;
	m_dM1 = 0;
	m_dD1 = 0;
	m_dE1 = 0;
	m_dF1 = 0;
	m_dB1 = 0;
	m_bSectionCheck	= FALSE;
}


void CPlateCrossBeam::Serialize(CArchive& ar)
{
	long nFlag = 4;// TU-Girder
	long vlong = 0;
	CPlateSpliceBase::SerializeSpliceBase(ar);

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_enTypeNum			;
		ar << m_H_CR				;			//가로보 높이
		ar << m_T1_CR				;			//가로보 하판 두께
		ar << m_T2_CR				;			//가로보 상판 두께
		ar << m_T3_CR				;			//가로보 복부판 두께
		ar << m_P_CR				;			//브라켓 돌출 길이
		ar << m_uN_CR				;			//상판 브라켓
		ar << m_uM_CR				;			//상판 너비/2
		ar << m_dN_CR				;			//하판 브라켓
		ar << m_dM_CR				;			//하판 너비/2
		ar << m_X_CR				;
		ar << m_vW_CR				;			//수직보강재
		ar << m_vT_CR				;
		ar << m_vL_CR				;
		ar << m_hW_CR				;			//수평보강재
		ar << m_hT_CR				;
		ar << m_nVStiffsu_CR		;
		ar << m_nAnchorsu_CR		;
		ar << m_nHStiffUppersu_CR	;
		ar << m_nHStiffLowersu_CR	;
		ar << m_cVSide_CR			;  //수직보강재 편측 적용
		ar << m_cGasset_CR			;  //브라켓 보강재 유무
		long n = 0;
		for(n = 0;n < 9;n++)
		{
			ar << m_VStiffDis_CR[n];
			ar << m_AnchorDis_CR[n];
		}

		ar << m_nQtyStringer_CR;
		
		ar <<	m_nLevelType;
		ar <<	m_nLGirSpType;
		ar <<	m_nRGirSpType;
		ar <<	m_nType;
		ar <<	vlong;
		ar <<	m_dbL1;
		ar <<	m_dbL2;
		ar <<	m_dbL3;
		ar <<	m_dbL4;
		ar <<	m_dbR1;
		ar <<	m_dbR2;
		ar <<	m_dbR3;
		ar <<	m_dbR4;
		ar <<	m_dLH1;
		ar <<	m_dLH2;
		ar <<	m_dRH1;
		ar <<	m_dRH2;

		m_pVStiff->Serialize(ar);
		ar << m_dwB;
		ar << m_dwR;
		ar << m_dwH;
		ar << m_wE;
		ar << m_wF;
		ar << m_dltH;
		ar << m_ltE;
		ar << m_ltF;
		ar << m_drtH;
		ar << m_rtE;
		ar << m_rtF;
		ar << m_vT_G;
		ar << m_vW_G;
		ar << m_vW1_G;
		ar << m_dBoltInterver;
		ar << m_bBoltInterver;
		ar << m_bScallopUpperFlange;
		ar << m_bScallopLowerFlange;
		ar << m_nFillerUseType;
		for( n = 0; n < 8; n++)
			ar << vlong;

		ar << m_dThickFiller;		
		ar << m_bMomentPan;	
		ar << m_bHunchTopConnect;
		ar << m_bConnectionPlate;		
		ar << m_nRadioConnectionPlateInOut;		
		ar << m_uM1;		
		ar << m_uD1;		
		ar << m_uE1;		
		ar << m_uF1;		
		ar << m_uB1;		
		ar << m_dM1;		
		ar << m_dD1;		
		ar << m_dE1;		
		ar << m_dF1;		
		ar << m_dB1;
		m_pSteelMaterialData->Serialize(ar);
		ar << m_bSectionCheck;
	}
	else
	{
		ar >> nFlag;
		ar >> m_enTypeNum			;
		ar >> m_H_CR				;			//가로보 높이
		ar >> m_T1_CR				;			//가로보 하판 두께
		ar >> m_T2_CR				;			//가로보 상판 두께
		ar >> m_T3_CR				;			//가로보 복부판 두께
		ar >> m_P_CR				;			//브라켓 돌출 길이
		ar >> m_uN_CR				;			//상판 브라켓
		ar >> m_uM_CR				;			//상판 너비/2
		ar >> m_dN_CR				;			//하판 브라켓
		ar >> m_dM_CR				;			//하판 너비/2
		ar >> m_X_CR				;
		ar >> m_vW_CR				;			//수직보강재
		ar >> m_vT_CR				;
		ar >> m_vL_CR				;
		ar >> m_hW_CR				;			//수평보강재
		ar >> m_hT_CR				;
		ar >> m_nVStiffsu_CR		;
		ar >> m_nAnchorsu_CR		;
		ar >> m_nHStiffUppersu_CR	;
		ar >> m_nHStiffLowersu_CR	;
		ar >> m_cVSide_CR			;  // 수직보강재 편측 적용
		ar >> m_cGasset_CR			;  //브라켓 보강재 유무
		long n = 0;
		for(n = 0;n < 9;n++)
		{
			ar >> m_VStiffDis_CR[n];
			ar >> m_AnchorDis_CR[n];
		}
		ar >> m_nQtyStringer_CR;

		ar >>	m_nLevelType;
		ar >>	m_nLGirSpType;
		ar >>	m_nRGirSpType;
		ar >>	m_nType;
		ar >>	vlong;
		ar >>	m_dbL1;
		ar >>	m_dbL2;
		ar >>	m_dbL3;
		ar >>	m_dbL4;
		ar >>	m_dbR1;
		ar >>	m_dbR2;
		ar >>	m_dbR3;
		ar >>	m_dbR4;
		ar >>	m_dLH1;
		ar >>	m_dLH2;
		ar >>	m_dRH1;
		ar >>	m_dRH2;

		m_pVStiff->Serialize(ar);
		ar >> m_dwB;
		ar >> m_dwR;
		ar >> m_dwH;
		ar >> m_wE;
		ar >> m_wF;
		ar >> m_dltH;
		ar >> m_ltE;
		ar >> m_ltF;
		ar >> m_drtH;
		ar >> m_rtE;
		ar >> m_rtF;
		ar >> m_vT_G;
		ar >> m_vW_G;
		ar >> m_vW1_G;
		
		//nFlag 값이 4보다 작은 버전에서는 m_vW_G와 m_vW1_G가 동일하게 적용.
		if(nFlag<4)
			m_vW1_G = m_vW_G;
		
		ar >> m_dBoltInterver;
		ar >> m_bBoltInterver;
		ar >> m_bScallopUpperFlange;
		ar >> m_bScallopLowerFlange;

		ar >> m_nFillerUseType;
		for( n = 0; n < 8; n++)
			ar >> vlong;

		ar >> m_dThickFiller;
		ar >> m_bMomentPan;	
		if(nFlag>1) ar >> m_bHunchTopConnect;
		ar >> m_bConnectionPlate;		
		ar >> m_nRadioConnectionPlateInOut;		
		ar >> m_uM1;		
		ar >> m_uD1;		
		ar >> m_uE1;		
		ar >> m_uF1;		
		ar >> m_uB1;		
		ar >> m_dM1;		
		ar >> m_dD1;		
		ar >> m_dE1;		
		ar >> m_dF1;		
		ar >> m_dB1;
		if(nFlag>0)
			m_pSteelMaterialData->Serialize(ar);
		if(nFlag>2)
			ar >> m_bSectionCheck;
	}
}

BOOL CPlateCrossBeam::IsSamePlateJewon(CPlateCrossBeam *pCrComp)
{
	if(m_nLevelType				!= pCrComp->m_nLevelType					)return FALSE;	
	if(m_nLGirSpType			!= pCrComp->m_nLGirSpType					)return FALSE;	
	if(m_nRGirSpType			!= pCrComp->m_nRGirSpType					)return FALSE;	
	if(m_nType					!= pCrComp->m_nType							)return FALSE;	
	if(m_dbL1					!= pCrComp->m_dbL1							)return FALSE;	
	if(m_dbL2					!= pCrComp->m_dbL2							)return FALSE;	
	if(m_dbL3					!= pCrComp->m_dbL3							)return FALSE;	
	if(m_dbL4					!= pCrComp->m_dbL4							)return FALSE;	
	if(m_dbR1					!= pCrComp->m_dbR1							)return FALSE;	
	if(m_dbR2					!= pCrComp->m_dbR2							)return FALSE;	
	if(m_dbR3					!= pCrComp->m_dbR3							)return FALSE;	
	if(m_dbR4					!= pCrComp->m_dbR4							)return FALSE;	
	if(m_dLH1					!= pCrComp->m_dLH1							)return FALSE;	
	if(m_dLH2					!= pCrComp->m_dLH2							)return FALSE;	
	if(m_dRH1					!= pCrComp->m_dRH1							)return FALSE;	
	if(m_dRH2					!= pCrComp->m_dRH2							)return FALSE;    
	if(m_dwB					!= pCrComp->m_dwB							)return FALSE;
	if(m_dwR					!= pCrComp->m_dwR							)return FALSE;
	if(m_dwH				  	!= pCrComp->m_dwH				  			)return FALSE;
	if(m_wE						!= pCrComp->m_wE							)return FALSE;
	if(m_wF						!= pCrComp->m_wF							)return FALSE;
	if(m_dltH				  	!= pCrComp->m_dltH				  			)return FALSE;
	if(m_ltE					!= pCrComp->m_ltE							)return FALSE;
	if(m_ltF					!= pCrComp->m_ltF							)return FALSE;
	if(m_drtH				  	!= pCrComp->m_drtH				  			)return FALSE;
	if(m_rtE					!= pCrComp->m_rtE							)return FALSE;
	if(m_rtF					!= pCrComp->m_rtF							)return FALSE;

	if(m_enTypeNum		!= pCrComp->m_enTypeNum) return FALSE	;
	if(m_H_CR			!= pCrComp->m_H_CR)  return FALSE	;			//가로보 높이
	if(m_T1_CR			!= pCrComp->m_T1_CR) return FALSE	;			//가로보 하판 두께
	if(m_T2_CR			!= pCrComp->m_T2_CR) return FALSE	;			//가로보 상판 두께
	if(m_T3_CR			!= pCrComp->m_T3_CR) return FALSE	;			//가로보 복부판 두께
	if(m_P_CR			!= pCrComp->m_P_CR)  return FALSE	;			//브라켓 돌출 길이
	if(m_uN_CR			!= pCrComp->m_uN_CR) return FALSE	;			//상판 브라켓
	if(m_uM_CR			!= pCrComp->m_uM_CR) return FALSE	;			//상판 너비/2
	if(m_dN_CR			!= pCrComp->m_dN_CR) return FALSE	;			//하판 브라켓
	if(m_dM_CR			!= pCrComp->m_dM_CR) return FALSE	;			//하판 너비/2
	if(m_X_CR			!= pCrComp->m_X_CR)  return FALSE	;
	if(m_vW_CR			!= pCrComp->m_vW_CR) return FALSE	;			//수직보강재
	if(m_vT_CR			!= pCrComp->m_vT_CR) return FALSE	;
	if(m_vL_CR			!= pCrComp->m_vL_CR) return FALSE	;
	if(m_hW_CR			!= pCrComp->m_hW_CR) return FALSE	;			//수평보강재
	if(m_hT_CR			!= pCrComp->m_hT_CR) return FALSE	;
	if(m_nVStiffsu_CR	!= pCrComp->m_nVStiffsu_CR) return FALSE;
	if(m_nQtyStringer_CR	 != pCrComp->m_nQtyStringer_CR) return FALSE;
	if(m_nAnchorsu_CR		 != pCrComp->m_nAnchorsu_CR) return FALSE	;
	if(m_nHStiffUppersu_CR	 != pCrComp->m_nHStiffUppersu_CR) return FALSE;
	if(m_nHStiffLowersu_CR	 != pCrComp->m_nHStiffLowersu_CR) return FALSE;
	if(m_cVSide_CR			 != pCrComp->m_cVSide_CR) return FALSE;  //수직보강재 편측 적용
	if(m_cGasset_CR			 != pCrComp->m_cGasset_CR) return FALSE;  //브라켓 보강재 유무
	if(m_bScallopUpperFlange != pCrComp->m_bScallopUpperFlange) return FALSE; 
	if(m_bScallopLowerFlange != pCrComp->m_bScallopLowerFlange) return FALSE; 

	if(m_cGasset_CR && pCrComp->m_cGasset_CR)
	{
		double HgtThis  = GetBx()->GetHeightGirderByBx();
		double HgtObj   = pCrComp->GetBx()->GetHeightGirderByBx();
//		if(HgtThis != HgtObj) return FALSE;
		if(!EQUAL(HgtThis,HgtObj,1)) return FALSE;	
	}
	long n = 0;
	for(n = 0 ;n < m_nVStiffsu_CR ;n++) 
	{
		if(m_VStiffDis_CR[n] != pCrComp->m_VStiffDis_CR[n]) return FALSE;
	}
	for(n = 0 ;n < m_nAnchorsu_CR ;n++)
	{
		if(m_AnchorDis_CR[n] != pCrComp->m_AnchorDis_CR[n]) return FALSE;
	}

	if(m_uM1 != pCrComp->m_uM1) return FALSE;
	if(m_uD1 != pCrComp->m_uD1) return FALSE;
	if(m_uE1 != pCrComp->m_uE1) return FALSE;
	if(m_uF1 != pCrComp->m_uF1) return FALSE;
	if(m_uB1 != pCrComp->m_uB1) return FALSE;
	if(m_dM1 != pCrComp->m_dM1) return FALSE;
	if(m_dD1 != pCrComp->m_dD1) return FALSE;
	if(m_dE1 != pCrComp->m_dE1) return FALSE;
	if(m_dF1 != pCrComp->m_dF1) return FALSE;
	if(m_dB1 != pCrComp->m_dB1) return FALSE;

	return TRUE;
}


void CPlateCrossBeam::CreatePieceObjPlate(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, BOOL bMakeChip)
{
	switch(pObj->m_nMarkBuje) {
		case CP_W:
		case CG_W:
			CreatePieceObjPlateMainPlate(pObj);
			if(bMakeBoltHole || bMakeBoltLine)
				CreatePieceObjPlateMainPlateBolt(pObj,bMakeBoltHole,bMakeBoltLine);
			break;
		case CP_F_U:
		case CG_F_U:
		case CP_F_L:
		case CG_F_L:
			CreatePieceObjPlateFlange(pObj);				
			CreatePieceObjPlateFlangeBolt(pObj,bMakeBoltHole,bMakeBoltLine);	
			break;
		case CP_BR_W:
		case CG_BR_W:
			CreatePieceObjPlateGassetMain(pObj);
			CreatePieceObjPlateGassetMainBolt(pObj,bMakeBoltHole,bMakeBoltLine);
			break;
		case CP_BR_F_U:
		case CG_BR_F_U:
		case CP_BR_F_L:
		case CG_BR_F_L:
			CreatePieceObjPlateGassetFlange(pObj,TRUE,bMakeBoltHole,bMakeBoltLine,bMakeChip);
		case CP_BR_GA:
		case CG_BR_GA:
			CreatePieceObjPlateGassetPlate(pObj);
			break;
		case CP_BR_GA_FA:
		case CG_BR_GA_FA:
			CreatePieceObjPlateGassetBoard(pObj);
			break;
		case CP_SP_W_SH:
		case CG_SP_W_SH:
			CreatePieceObjPlateSpliceWebShear(pObj);
			CreatePieceObjPlateSpliceWebShearBolt(pObj, bMakeBoltHole, bMakeBoltLine);
			break;
		case CP_SP_W_FH:
			CreatePieceObjPlateSpliceWebFiller(pObj);
			CreatePieceObjPlateSpliceWebFillerBolt(pObj, bMakeBoltHole, bMakeBoltLine);
			break;
		case CP_SP_W_MO:
		case CG_SP_W_MO:
			CreatePieceObjPlateSpliceWebMoment(pObj);
			CreatePieceObjPlateSpliceWebMomentBolt(pObj,bMakeBoltHole, bMakeBoltLine, 0);
			break;
		case CP_SP_U_U:
		case CG_SP_U_U:
			CreatePieceObjPlateSpliceFlange(pObj);
			CreatePieceObjPlateSpliceFlangeUpperUpperBolt(pObj,bMakeBoltHole,bMakeBoltLine);
			break;
		case CP_SP_U_L:
		case CG_SP_U_L:
			CreatePieceObjPlateSpliceFlange(pObj);
			CreatePieceObjPlateSpliceFlangeUpperLowerBolt(pObj,bMakeBoltHole,bMakeBoltLine);
			break;
		case CP_SP_L_U:
		case CG_SP_L_U:
			CreatePieceObjPlateSpliceFlange(pObj);
			CreatePieceObjPlateSpliceFlangeLowerUpperBolt(pObj,bMakeBoltHole,bMakeBoltLine);
			break;
		case CP_SP_L_L:
		case CG_SP_L_L:
			CreatePieceObjPlateSpliceFlange(pObj);
			CreatePieceObjPlateSpliceFlangeLowerLowerBolt(pObj,bMakeBoltHole,bMakeBoltLine);
			break;
	/*	case CP_H:
		case CG_H:
			CreatePieceObjPlateHStiffner(pObj);
			break;*/
		case CP_V:
		case CG_V:
			CreatePieceObjPlateVStiffner(pObj);
			break;
	}
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceWebShear(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx   = GetBx();
	CPlateGirderApp	 *pGir  = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption    *pGlopt= pDB->m_pDataManage->GetGlobalOption();

	CDPoint A[10], xy;
	GetCrossXyDom(A);

	double Zorib = 0; // 조립여유치
	double C1 = m_sC1 + Zorib;
	double offsetY = (m_H_CR - m_sW)/2;
	// 이음판 기울기 설정
	long nBoltArrange = pGlopt->GetDesignBoltHoleArrangeCrossBeam();
	double slop = (nBoltArrange==2) ? 0.0 : (A[3].y - A[1].y)/(A[3].x - A[1].x);
	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();
	CDPoint vDir = slop ? CDPoint(1/slop, slop).Unit() : CDPoint(1,0);

	if(pObj->m_nCol==0) //m_nCol =bLeft ? 0 : 1;
	{
		xy.x = A[2].x - m_sA - m_sB*m_sC - C1/2;
		xy.y = A[2].y - offsetY - slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);
		xy.x = A[2].x + m_sA + m_sB*m_sC + C1/2;
		xy.y = A[2].y - offsetY + slop * (m_sA + m_sB*m_sC + C1/2);
		xy = xy + GapCutting/vDir.x * vDir;
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.x = A[2].x + m_sA + m_sB*m_sC + C1/2;
		xy.y = A[2].y - offsetY - m_sW + slop * (m_sA + m_sB*m_sC + C1/2);
		xy = xy + GapCutting/vDir.x * vDir;
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.x = A[2].x - m_sA - m_sB*m_sC - C1/2;
		xy.y = A[2].y - offsetY - m_sW - slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy);
	}
	else
	{
		slop = (nBoltArrange==2) ? 0.0 : (A[6].y - A[4].y)/(A[6].x - A[4].x);
		vDir = slop ? CDPoint(1/slop, slop).Unit() : CDPoint(1,0);
		xy.x = A[5].x - m_sA - m_sB*m_sC - C1/2;
		xy.y = A[5].y - offsetY - slop * (m_sA + m_sB*m_sC + C1/2);
		xy = xy - GapCutting/vDir.x * vDir;
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);
		xy.x = A[5].x + m_sA + m_sB*m_sC + C1/2;
		xy.y = A[5].y - offsetY + slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.y = A[5].y - offsetY - m_sW + slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.x = A[5].x - m_sA - m_sB*m_sC - C1/2;
		xy.y = A[5].y - offsetY - m_sW - slop * (m_sA + m_sB*m_sC + C1/2);
		xy = xy - GapCutting/vDir.x * vDir;
		pObj->AddElement(xy);
	}
		
	pObj->RegenVertex();
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceWebShearBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CMatrixPoint *xyMat = new CMatrixPoint;
	GetXyMatrixWebSH(xyMat, pObj->m_nRow, pObj->m_nCol);

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1, xy2;
	// 볼트홀
	long nSttCol = nDir == 1  ? xyMat->GetCols()/2 : 0;
	long nEndCol = nDir == -1 ? xyMat->GetCols()/2 : xyMat->GetCols();
	long r	= 0;
	for(r=0; r<xyMat->GetRows(); r++)
	{
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}

	// 볼트라인 : 가로방향
	for(r=0; r<xyMat->GetRows(); r++)
	{
		xy1 = xyMat->GetMatrixPoint(r,nSttCol);
		xy2 = xyMat->GetMatrixPoint(r,nEndCol-1);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	// 볼트라인 : 세로방향
	for(long c=nSttCol; c<nEndCol; c++)
	{
		if(xyMat->GetRows()==0) break;
		
		xy1 = xyMat->GetMatrixPoint(0,c);
		xy2 = xyMat->GetMatrixPoint(xyMat->GetRows()-1,c);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	
	delete xyMat;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);

	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;
	if (bMakeBoltLine)
		pObj->AddChildObj(pObjLine);
	else
		delete pObjLine;
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceWebFiller(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx   = GetBx();
	CPlateGirderApp	 *pGir  = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption    *pGlopt= pDB->m_pDataManage->GetGlobalOption();

	CDPoint A[10], xy;
	GetCrossXyDom(A);

	double Zorib = 0; // 조립여유치
	double C1 = m_sC1 + Zorib;
	double offsetY = (m_H_CR - m_sW)/2;
	// 이음판 기울기 설정
	long nBoltArrange = pGlopt->GetDesignBoltHoleArrangeCrossBeam();
	double slop = (nBoltArrange==2) ? 0.0 : (A[3].y - A[1].y)/(A[3].x - A[1].x);
	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();
	CDPoint vDir = slop ? CDPoint(1/slop, slop).Unit() : CDPoint(1,0);

	if(pObj->m_nCol==0) //m_nCol =bLeft ? 0 : 1;
	{
		xy.x = A[2].x;// + m_sA + m_sB*m_sC + C1/2;
		xy.y = A[2].y - offsetY;// + slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);
		xy.x = A[2].x + m_sA + m_sB*m_sC + C1/2 + (m_sA *2);
		xy.y = A[2].y - offsetY + slop * (m_sA + m_sB*m_sC + C1/2 + (m_sA *2));
		xy = xy + GapCutting/vDir.x * vDir;
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.x = A[2].x + m_sA + m_sB*m_sC + C1/2 + (m_sA *2);
		xy.y = A[2].y - offsetY - m_sW + slop * (m_sA + m_sB*m_sC + C1/2 + (m_sA *2));
		xy = xy + GapCutting/vDir.x * vDir;
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.x = A[2].x;// + m_sA + m_sB*m_sC + C1/2;
		xy.y = A[2].y - offsetY - m_sW;// + slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy);
	}
	else
	{
		slop = (nBoltArrange==2) ? 0.0 : (A[6].y - A[4].y)/(A[6].x - A[4].x);
		vDir = slop ? CDPoint(1/slop, slop).Unit() : CDPoint(1,0);
		xy.x = A[5].x - m_sA - m_sB*m_sC - C1/2 - (m_sA *2);
		xy.y = A[5].y - offsetY - slop * (m_sA + m_sB*m_sC + C1/2 + (m_sA *2));
		xy = xy - GapCutting/vDir.x * vDir;
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);
		xy.x = A[5].x;// - m_sA - m_sB*m_sC - C1/2;
		xy.y = A[5].y - offsetY;// - slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.y = A[5].y - offsetY - m_sW;// - slop * (m_sA + m_sB*m_sC + C1/2);
		pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy.x = A[5].x - m_sA - m_sB*m_sC - C1/2 - (m_sA *2);
		xy.y = A[5].y - offsetY - m_sW - slop * (m_sA + m_sB*m_sC + C1/2 + (m_sA *2));
		xy = xy - GapCutting/vDir.x * vDir;
		pObj->AddElement(xy);
	}
		
	pObj->RegenVertex();
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceWebFillerBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CMatrixPoint *xyMat = new CMatrixPoint;
	GetXyMatrixWebFH(xyMat, pObj->m_nRow, pObj->m_nCol);

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1, xy2;
	// 볼트홀
	long nSttCol = nDir == 1  ? xyMat->GetCols()/2 : 0;
	long nEndCol = nDir == -1 ? xyMat->GetCols()/2 : xyMat->GetCols();

	for(long r=0; r<xyMat->GetRows(); r++)
	{
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}

	// 볼트라인 : 가로방향
/*
	for(r=0; r<xyMat->GetRows(); r++)
	{
		xy1 = xyMat->GetMatrixPoint(r,nSttCol);
		xy2 = xyMat->GetMatrixPoint(r,nEndCol-1);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
*/	
	// 볼트라인 : 세로방향
//	for(long c=nSttCol; c<nEndCol; c++)
//	{
		xy1 = xyMat->GetMatrixPoint(0,0);
		xy2 = xyMat->GetMatrixPoint(xyMat->GetRows()-1,0);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
//	}
	
	delete xyMat;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);

	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;
	if (bMakeBoltLine)
		pObj->AddChildObj(pObjLine);
	else
		delete pObjLine;
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceWebMoment(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp  *pGir= pBx->GetGirder();
	CPlateBridgeApp  *pDB = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	if(pBx==NULL) return;
	if(m_mW==0 || m_mE==0 || m_mF==0)
		return;

	CDPoint A[10], xy;
	GetCrossXyDom(A);
	// 이음판 기울기 설정
	long nBoltArrange = pGlopt->GetDesignBoltHoleArrangeCrossBeam();
	double hgt  = m_mW;	 // 이음판 전체높이 (CENTER기준)
	double wid  = m_mA+m_mB*m_mC+m_mC1/2;            // 1/2 폭
	double offsetY = pObj->m_nRow==0 ? m_mG : (m_H_CR - m_mG - hgt);
	double GapCutting = 0;

	double slop = (nBoltArrange==2) ? 0.0 : (A[3].y - A[1].y)/(A[3].x - A[1].x);
	CDPoint vDir = slop ? CDPoint(1/slop, slop).Unit() : CDPoint(1,0);
//	xy = xy - GapCutting/vDir.x * vDir;
	xy.x = (pObj->m_nCol==0 ? A[2].x : A[5].x) - wid;
	xy.y = (pObj->m_nCol==0 ? A[2].y : A[5].y) - offsetY - wid*slop;
	if (pObj->m_nCol!=0)
		xy = xy - GapCutting/vDir.x * vDir;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);
	xy.x = (pObj->m_nCol==0 ? A[2].x : A[5].x) + wid;
	xy.y = (pObj->m_nCol==0 ? A[2].y : A[5].y) - offsetY + wid*slop;
	if (pObj->m_nCol==0)
		xy = xy + GapCutting/vDir.x * vDir;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
	xy.x = (pObj->m_nCol==0 ? A[2].x : A[5].x) + wid;
	xy.y = (pObj->m_nCol==0 ? A[2].y : A[5].y) - offsetY - hgt + wid*slop;
	if (pObj->m_nCol==0)
		xy = xy + GapCutting/vDir.x * vDir;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
	xy.x = (pObj->m_nCol==0 ? A[2].x : A[5].x) - wid;
	xy.y = (pObj->m_nCol==0 ? A[2].y : A[5].y) - offsetY - hgt - wid*slop;
	if (pObj->m_nCol!=0)
		xy = xy - GapCutting/vDir.x * vDir;
	pObj->AddElement(xy);
	
	pObj->RegenVertex();
	return;
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceWebMomentBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CMatrixPoint *xyMat = new CMatrixPoint;
	GetXyMatrixWebMO(xyMat, pObj->m_nRow,pObj->m_nCol);

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1, xy2;
	// 볼트홀
	long nSttCol = nDir == 1  ? xyMat->GetCols()/2 : 0;
	long nEndCol = nDir == -1 ? xyMat->GetCols()/2 : xyMat->GetCols();
	long r		 = 0;
	for(r=0; r<xyMat->GetRows(); r++)
	{
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}

	// 볼트라인 : 가로방향
	for(r=0; r<xyMat->GetRows(); r++)
	{
		xy1 = xyMat->GetMatrixPoint(r,nSttCol);
		xy2 = xyMat->GetMatrixPoint(r,nEndCol-1);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	// 볼트라인 : 세로방향
	for(long c=nSttCol; c<nEndCol; c++)
	{
		xy1 = xyMat->GetMatrixPoint(0,c);
		xy2 = xyMat->GetMatrixPoint(xyMat->GetRows()-1,c);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	
	delete xyMat;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);

	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;
	if (bMakeBoltLine)
		pObj->AddChildObj(pObjLine);
	else
		delete pObjLine;
}

void CPlateCrossBeam::GetXyMatrixFlangeUpperConnectionPlate(CMatrixPoint *xyMat, long nRow,long nCol, BOOL bLine, BOOL bCrossConnect)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption	*pGlopt = pDB->m_pDataManage->GetGlobalOption();

	BOOL bZigZag= FALSE;
	if(!bLine && pGlopt->GetDesignBoltHoleJigJaegOfHorPlate()==0 && m_uE>0 && m_uF>0)
		bZigZag= TRUE;
	double	GapCutting = 0;//pGlopt->GetBracketGapOfCutting();
	long	nRowSu = m_uE+1;
	long	nColSu = m_uB+1;
	xyMat->SetSize(nRowSu*2, nColSu*2);
	CDPoint cp = CDPoint(0,0);
	double dY = m_uM_CR-m_uD;
	CDPoint xy = CDPoint(0,0);
	long	nDir	= nCol==1 ? -1 : 1;

	if(bCrossConnect)
	{
		// 가로보와의 연결되는 볼트
		for(long r=0; r<nRowSu; r++)
		{
			for(long c=0; c<nColSu; c++)
			{
				xy.x = m_uC1/2*nDir + m_uC*c*nDir;
				xy.y = dY-m_uF*r;
				if (nCol==1)
					xy.x -= GapCutting;
				if(!bZigZag || (r%2)==(c%2))
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y+xy.y));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y-xy.y));
				}
				else
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(NULL,NULL));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(NULL,NULL));
				}
			}
		}
	}
	else
	{
		// 가로보, 거세트와 연결되는 볼트
		nRowSu = (long)m_uE1+1;
		nColSu = (long)m_uB1+1;
		xyMat->SetSize(nRowSu*2, nColSu*2);
		dY = m_uM1-m_uD1;

		for(long r=0; r<nRowSu; r++)
		{
			for(long c=0; c<nColSu; c++)
			{
				xy.x = -m_uC1/2*nDir - m_uC*c*nDir;
				xy.y = dY-m_uF1*r;
				if (nCol==1)
					xy.x -= GapCutting;
				if(!bZigZag || (r%2)==(c%2))
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y+xy.y));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y-xy.y));
				}
				else
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(NULL,NULL));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(NULL,NULL));
				}
			}
		}
	}
}

void CPlateCrossBeam::GetXyMatrixFlangeUpper(CMatrixPoint *xyMat, long nRow,long nCol, BOOL bLine)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	BOOL bZigZag= FALSE;
	if(!bLine && pGlopt->GetDesignBoltHoleJigJaegOfHorPlate()==0 && m_uE>0 && m_uF>0)
		bZigZag= TRUE;
	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();

	long nRowSu = m_uE+1;
	long nColSu = m_uB+1;
	xyMat->SetSize(nRowSu*2, nColSu*2);
	CDPoint cp = CDPoint(0,0);
	double dY = m_uM_CR-m_uD;
	CDPoint xyL = CDPoint(0,0);
	CDPoint xyR = CDPoint(0,0);
	// 위쪽으로
	for(long r=0; r<nRowSu; r++)
	{
		for(long c=0; c<nColSu; c++)
		{
			xyL.x = -m_uC1/2 - m_uC*c; // 좌측
			xyL.y = dY-m_uF*r;
			if (nCol==1)
				xyL.x -= GapCutting;
			if(!bZigZag || (r%2)==(c%2))
			{
				xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(cp.x+xyL.x, cp.y+xyL.y));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(cp.x+xyL.x, cp.y-xyL.y));
			}
			else
			{
				xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(NULL,NULL));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(NULL,NULL));
			}
			
			xyR.x = m_uC1/2 + m_uC*c; // 우측
			if (nCol==0)
				xyR.x += GapCutting;
			xyR.y = dY-m_uF*r;
			if(!bZigZag || (r%2)==(c%2))
			{
				xyMat->SetMatrixPoint(r,nColSu+c,CDPoint(cp.x+xyR.x, cp.y+xyR.y));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu+c,CDPoint(cp.x+xyR.x, cp.y-xyR.y));
			}
			else
			{
				xyMat->SetMatrixPoint(r,nColSu+c,CDPoint(NULL,NULL));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu+c,CDPoint(NULL,NULL));
			}
		}
	}
}

void CPlateCrossBeam::GetXyMatrixFlangeLower(CMatrixPoint *xyMat, long nRow,long nCol,BOOL bLine)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	BOOL bZigZag= FALSE;
	if(!bLine && pGlopt->GetDesignBoltHoleJigJaegOfHorPlate()==0 && m_dE>0 && m_dF>0)
		bZigZag= TRUE;
	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();

	long nRowSu = m_dE+1;
	long nColSu = m_dB+1;
	xyMat->SetSize(nRowSu*2, nColSu*2);
	CDPoint cp = CDPoint(0,0);
	double dY = m_dM_CR-m_dD;
	CDPoint xyL = CDPoint(0,0);
	CDPoint xyR = CDPoint(0,0);
	// 위쪽으로
	for(long r=0; r<nRowSu; r++)
	{
		for(long c=0; c<nColSu; c++)
		{
			xyL.x = -m_dC1/2 - m_dC*c; // 좌측
			xyL.y = dY-m_dF*r;
			if (nCol==1)
				xyL.x -= GapCutting;
			if(!bZigZag || (r%2)==(c%2))
			{
				xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(cp.x+xyL.x, cp.y+xyL.y));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(cp.x+xyL.x, cp.y-xyL.y));
			}
			else
			{
				xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(NULL,NULL));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(NULL,NULL));
			}
			
			xyR.x = m_dC1/2 + m_dC*c; // 우측
			xyR.y = dY-m_dF*r;
			if (nCol==0)
				xyR.x += GapCutting;
			if(!bZigZag || (r%2)==(c%2))
			{
				xyMat->SetMatrixPoint(r,nColSu+c,CDPoint(cp.x+xyR.x, cp.y+xyR.y));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu+c,CDPoint(cp.x+xyR.x, cp.y-xyR.y));
			}
			else
			{
				xyMat->SetMatrixPoint(r,nColSu+c,CDPoint(NULL,NULL));
				xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu+c,CDPoint(NULL,NULL));
			}
		}
	}
}

void CPlateCrossBeam::GetXyMatrixWebSH(CMatrixPoint *xyMat, long nRow,long nCol)
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp  *pGir= pBx->GetGirder();
	CPlateBridgeApp  *pDB = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();

	CDPoint A[10];
	GetCrossXyDom(A);

	CDPoint xy1 = nCol ==0 ? A[2] : A[5];
	double sGiul = nCol ==0 ? (A[3].y - A[2].y)/(A[3].x - A[2].x)
							: (A[5].y - A[4].y)/(A[5].x - A[4].x);
	// 0-균등배치,1-일반배치,2-수평배치
	long nBoltArray = pGlopt->GetDesignBoltHoleArrangeCrossBeam();
	if(nBoltArray==2) sGiul = 0.0;

	double dX = xy1.x;
	double dY = xy1.y-(m_H_CR-m_sE*m_sF)/2;

	CDPoint xyL(dX,dY);
	CDPoint xyR(dX,dY);
	long nRowSu = m_sE+1;
	long nColSu = m_sB+1;
	xyMat->SetSize(nRowSu, nColSu*2);
	for(long r=0; r<nRowSu; r++)
	{
		for(long c=0; c<nColSu; c++)
		{
			xyL.x = -m_sC1/2 - m_sC*c; // 좌측
			if (nCol==1)
				xyL.x -= GapCutting;
			xyL.y = dY - m_sF*r + xyL.x*sGiul;
			xyR.x = m_sC1/2 + m_sC*c; // 우측
			if (nCol==0)
				xyR.x += GapCutting;
			xyR.y = dY - m_sF*r + xyR.x*sGiul;
			xyL.x += dX;
			xyR.x += dX;
			xyMat->SetMatrixPoint(r,nColSu-1-c,xyL);
			xyMat->SetMatrixPoint(r,nColSu+c  ,xyR);
		}
	}
}

//Filler
void CPlateCrossBeam::GetXyMatrixWebFH(CMatrixPoint *xyMat, long nRow,long nCol)
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp  *pGir= pBx->GetGirder();
	CPlateBridgeApp  *pDB = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	//double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();

	CDPoint A[10], xy;
	GetCrossXyDom(A);

	double Zorib = 0; // 조립여유치
	double C1 = m_sC1 + Zorib;

	CDPoint xy1 = nCol ==0 ? A[2] : A[5];
	double sGiul = nCol ==0 ? (A[3].y - A[2].y)/(A[3].x - A[2].x)
							: (A[5].y - A[4].y)/(A[5].x - A[4].x);
	// 0-균등배치,1-일반배치,2-수평배치
	long nBoltArray = pGlopt->GetDesignBoltHoleArrangeCrossBeam();
	if(nBoltArray==2) sGiul = 0.0;

	double dX, dY;
	double dGap;
	if(nCol == 0)
	{
		dX = A[2].x;// + m_sA + m_sB*m_sC + C1/2;// + (m_sA);
		dY = xy1.y-(m_H_CR-m_sE*m_sF)/2;
		dGap = m_sA + m_sB*m_sC + C1/2 + (m_sA); 
	}
	else
	{
		dX = A[5].x;// - m_sA - m_sB*m_sC - C1/2;// - (m_sA);
		dY = xy1.y-(m_H_CR-m_sE*m_sF)/2;
		dGap = - (m_sA * 2) - m_sB*m_sC - C1/2;
	}
	

	//CDPoint xyL(dX,dY);
	CDPoint xyR(dX,dY);
	long nRowSu = m_sE+1;
	long nColSu = 1;//m_sB+1;
	xyMat->SetSize(nRowSu, nColSu);
	for(long r=0; r<nRowSu; r++)
	{
		for(long c=0; c<nColSu; c++)
		{
			
			xyR.x =  dGap;//m_sC1/2 + m_sC*c; // 우측
			xyR.y = dY - m_sF*r + xyR.x*sGiul;
			xyR.x += dX;
			xyMat->SetMatrixPoint(r,c,xyR);
		}
	}
}

void CPlateCrossBeam::CreatePieceObjPlateGassetFlange(CPlatePieceObject *pObj, BOOL bRotate, BOOL bMakeBoltHole, BOOL bMakeBoltLine, BOOL bMakeChip, long nMode)
{
	CPlateBasicIndex *pBx    = GetBx();
	CPlateGirderApp  *pGir   = pBx->GetGirder();
	CPlateBridgeApp  *pDB= (CPlateBridgeApp*)pGir->GetBridgeApp();

	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	CDPoint vCross	= GetVectorPlan().Rotate(CDPoint(-1,0));
	CDPoint vAng	= vCross.RotateInv(vGir);
	BOOL bLeft		= (pObj->m_nCol==0)	 ? TRUE : FALSE;

	if(!bLeft)
	{
		pGir	= pGir->m_pRight;
		pBx		= pBx->GetBxMatchByCrossBeam(pGir);
		if(!pBx || !pBx->GetSection()) return;
	}

	double WebT	  = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dHGir  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double Dis1 = 0, Dis2 = 0;
	BOOL bUpper = TRUE;
	if(pObj->m_nMarkBuje == CP_BR_F_U)		bUpper = TRUE;
	if(pObj->m_nMarkBuje == CP_BR_F_L)		bUpper = FALSE;

	if(bUpper)
	{
		if(m_dLH1>0 || m_dRH1>0 )	Dis1 = 0;
		else						Dis1 = pGir->GetWidthOnStation(pBx->GetStation(), bUpper)/2-WebT/2;
	}
	else
	{
		// 가로보가 거더복부판과 크기가 같을 경우
		if(m_H_CR == dHGir)	Dis1 = pGir->GetWidthOnStation(pBx->GetStation(), bUpper)/2-WebT/2;
		else				Dis1 = 0;
	}
	Dis2 = m_P_CR/fabs(vAng.y);

	double W1	= bUpper ? m_uM1+m_uN_CR  : m_dM1+m_dN_CR;
	double W2	= bUpper ? m_uM1 : m_dM1;
	double R	= bUpper ? (m_uN_CR == 0 ? 0 : pDB->m_dRoundCrossUpper) 
		                 : (m_dN_CR == 0 ? 0 : pDB->m_dRoundCrossLower);
	double C	= bUpper ? (m_uN_CR == 0 ? 0 : pDB->m_dRoundChipCrossUpper)
		                 : (m_dN_CR == 0 ? 0 : pDB->m_dRoundChipCrossLower);

	double ChipHeight = 30;
	double ChipWidth  = 40;
	double suchuk  = 0;
	double suchuk2 = 0;

	CPlatePieceObject *pChipObj = new CPlatePieceObject(pGir);
	// 수축치 적용
	Dis1 += (suchuk+suchuk2);
	CDPoint BP(0,0), Chip[8], gP[10],cp;
	CDPoint imPt;

	if(bLeft)	BP = -vCross*WebT/2/fabs(vCross.RotateInv(vGir).y);
	else		BP = vCross*WebT/2/fabs(vCross.RotateInv(vGir).y);

	gP[0] = GetGassetPlateVertex(2,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, TRUE ,vAng, vGir);
	gP[1] = GetGassetPlateVertex(4,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, TRUE ,vAng, vGir);
	gP[2] = GetGassetPlateVertex(5,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, TRUE ,vAng, vGir);
	gP[3] = GetGassetPlateVertex(1,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, TRUE ,vAng, vGir);
	gP[4] = GetGassetPlateVertex(1,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, FALSE,vAng, vGir);
	gP[5] = GetGassetPlateVertex(5,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, FALSE,vAng, vGir);
	gP[6] = GetGassetPlateVertex(4,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, FALSE,vAng, vGir);
	gP[7] = GetGassetPlateVertex(2,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, FALSE,vAng, vGir);
	gP[8] = GetGassetPlateVertex(0,BP.x,BP.y,Dis1,Dis2,W1,W2,R,C,!bLeft, FALSE,vAng, vGir);
	gP[9] = (gP[0]+gP[7])/2;

	if(R==0) {	gP[2] = gP[1];	gP[5] = gP[6];	}
	if(C==0) {	gP[1] = gP[0];	gP[6] = gP[7];	}

	//if(gP[3].y < gP[2].y && !bLeft)	gP[2] = gP[3];
	//if(gP[5].y < gP[4].y && !bLeft)	gP[5] = gP[4];

	if (nMode == 0)
	{
		pObj->AddElement(gP[0],P_TYPE_LINE,-1,pBx);
		pObj->AddElement(gP[1]);
		if(R>0)
			pObj->AddElement(gP[2],bLeft?-R:R);
		else
			pObj->AddElement(gP[2]);
		
		pObj->AddElement(gP[3]);
		pObj->AddElement(gP[4]);
		pObj->AddElement(gP[5]);
		if(R>0)
			pObj->AddElement(gP[6],bLeft?-R:R);
		else
			pObj->AddElement(gP[6]);
		
		pObj->AddElement(gP[7]);
		pObj->AddElement(gP[8],P_TYPE_DIMPOINT);
		pObj->AddElement(gP[9],P_TYPE_DIMPOINT);
	}
	else if (nMode == 1)//가로보 개수로 분할시 우측만 그리기...070201....KB...
	{
		pObj->AddElement(gP[0],P_TYPE_LINE,-1,pBx);
		pObj->AddElement(gP[1]);
		if(R>0)
			pObj->AddElement(gP[2],bLeft?-R:R);
		else
			pObj->AddElement(gP[2]);
		
		pObj->AddElement(gP[3]);

		imPt.x = gP[4].x + (gP[3].x - gP[4].x)/2;
		imPt.y = gP[4].y;
		pObj->AddElement(imPt);

		imPt.y = gP[0].y;
		pObj->AddElement(imPt);		
	}
	else// nMode == 2  //가로보 개수로 분할시 좌측만 그리기...
	{		
		imPt.x = gP[4].x + (gP[3].x - gP[4].x)/2;
		imPt.y = gP[0].y;
		pObj->AddElement(imPt);
				
		imPt.y = gP[4].y;
		pObj->AddElement(imPt);
		
		pObj->AddElement(gP[4]);
		pObj->AddElement(gP[5]);
		if(R>0)
			pObj->AddElement(gP[6],bLeft?-R:R);
		else
			pObj->AddElement(gP[6]);
		
		pObj->AddElement(gP[7]);
		pObj->AddElement(gP[8],P_TYPE_DIMPOINT);
		pObj->AddElement(gP[9],P_TYPE_DIMPOINT);

	}

	if(bUpper && bMakeChip)
	{
		if (!bLeft) ChipHeight *=-1;
		double GiulGi = (gP[3].y - gP[2].y) / (gP[3].x - gP[2].x);
		double Height   = gP[3].y - gP[0].y + ChipHeight;
		double Dist = Height/GiulGi;
		
		Chip[0] = gP[0];
		Chip[1] = CDPoint(gP[0].x+ChipWidth, gP[0].y);
		Chip[2] = CDPoint(gP[0].x+ChipWidth, gP[0].y-ChipHeight);
		Chip[3] = CDPoint(gP[0].x, gP[0].y-ChipHeight);

		GiulGi = (gP[5].y - gP[4].y) / (gP[5].x - gP[4].x);
		Height   = gP[4].y - gP[7].y + ChipHeight;
		Dist = Height/GiulGi;

		Chip[4] = gP[7];
		Chip[5] = CDPoint(gP[7].x-ChipWidth, gP[7].y);
		Chip[6] = CDPoint(gP[7].x-ChipWidth, gP[7].y-ChipHeight);
		Chip[7] = CDPoint(gP[7].x, gP[7].y-ChipHeight);

		pChipObj->AddElement(Chip[0],P_TYPE_LINE,-1,pBx);
		pChipObj->AddElement(Chip[1]);
		pChipObj->AddElement(Chip[2]);
		pChipObj->AddElement(Chip[3]);
		pChipObj->AddElement(Chip[0]);		//추가
		pChipObj->SetEnd();
		pChipObj->AddElement(Chip[4],P_TYPE_LINE,-1,pBx);
		pChipObj->AddElement(Chip[5]);
		pChipObj->AddElement(Chip[6]);
		pChipObj->AddElement(Chip[7]);
		pChipObj->AddElement(Chip[4]);		//추가 
		pChipObj->SetEnd();
	}

	// BOLT & HOLE
	CreatePieceObjPlateGassetFlangeBolt(pObj, gP, bRotate,bMakeBoltHole,bMakeBoltLine);

	// 90도 회전 ///////////////////////////////////////////
	if(bRotate)
	{
		long n = 0;
		for(n=0;n<pObj->GetObjSize();n++)
			pObj->SetPoint(n,GetXyRotateVector(CDPoint(0,0),pObj->GetPoint(n),CDPoint(0,1)));
		for(n=0;n<pChipObj->GetObjSize();n++)
			pChipObj->SetPoint(n,GetXyRotateVector(CDPoint(0,0),pChipObj->GetPoint(n),CDPoint(0,1)));
	}

	pObj->SetFaceRange(0,1);
	pObj->SetFaceRange(1,2);
	pObj->SetFaceRange(2,3);
	pObj->SetFaceRange(3,4);
	pObj->SetFaceRange(4,5);
	pObj->SetFaceRange(5,6);
	pObj->SetFaceRange(6,7);
	pObj->SetFaceRange(7,0);

	if(bUpper)
	{
		pChipObj->SetFaceRange(0,1);
		pChipObj->SetFaceRange(5,4);
		pObj->AddChildObj(pChipObj);
	}
	else
		delete pChipObj;
	pObj->RegenVertex();
}

//<summary>
//가로보 블라켓 플랜지 볼트홀 생성 
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
//<param name="gP[]"></param>
//<param name="bRotate"></param>
//<param name="bMakeBoltHole"></param>
//<param name="bMakeBoltLine"></param>
void CPlateCrossBeam::CreatePieceObjPlateGassetFlangeBolt(CPlatePieceObject *pObj, CDPoint gP[], BOOL bRotate, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	if(!bMakeBoltHole && !bMakeBoltLine)	return;
	
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption    *pGlopt= pDB->m_pDataManage->GetGlobalOption();
	
	BOOL bUpper;
	if(pObj->m_nMarkBuje == CP_BR_F_U)
		bUpper = TRUE;
	if(pObj->m_nMarkBuje == CP_BR_F_L)
		bUpper = FALSE;
	
	long   E  = bUpper ? m_uE : m_dE;
	double F  = bUpper ? m_uF : m_dF;
	long   B  = bUpper ? m_uB    : m_dB;
	double C  = bUpper ? m_uC    : m_dC;
	double C1 = bUpper ? m_uC1   : m_dC1;
	double D  = bUpper ? m_uD    : m_dD;
	double M  = bUpper ? m_uM_CR : m_dM_CR;
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);
	BOOL bZigZag= FALSE;
	if(pGlopt->GetDesignBoltHoleJigJaegOfHorPlate()==0 && E>0 && F>0)
		bZigZag= TRUE;

	if(pObj->m_nCol)
	{
		C  *= -1;
		C1 *= -1;
	}

	CDPoint vDir1 = (gP[3]-gP[4]).Unit(); // 브라켓 접촉면 방향
	CDPoint vDir2 = vDir1.Rotate90();
	double dDir = pObj->m_nCol == 1 ? -1 : 1;

	double L1;
	CDPoint xy;
	CPlatePieceObject *pBolt = new CPlatePieceObject(pBx->GetGirder());
	CPlatePieceObject *pLine = new CPlatePieceObject(pBx->GetGirder());
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);

	if(m_bConnectionPlate)
	{
		long i = 0;
		for(i=0; i<E+1; i++) 
		{
			for(long n=0; n<B+1; n++) 
			{
				L1 = M-D-F*i;
				xy = gP[8] + vDir1*L1 + vDir2*(-C1/2-C*n);
				if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
					pBolt->AddCircle(xy,BoltR,pBx);
				if(i==0) pLine->AddElement(xy);
				L1 = -M+D+F*i;
				xy = gP[8] + vDir1*L1 + vDir2*(-C1/2-C*n);
				if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
					pBolt->AddCircle(xy,BoltR,pBx);
				if(i==0) pLine->AddElement(xy);
				if(i==0) pLine->SetEnd();
				if(n==0)
				{
					L1 = M-D-E*F+F*i;
					xy = gP[8] + vDir1*L1 + vDir2*(-C1/2);
					pLine->AddElement(xy);
					xy = gP[8] + vDir1*L1 + vDir2*(-C1/2-B*C);
					pLine->AddElement(xy);
					pLine->SetEnd();
					//
					L1 = -M+D+E*F-F*i;
					xy = gP[8] + vDir1*L1 + vDir2*(-C1/2);
					pLine->AddElement(xy);
					xy = gP[8] + vDir1*L1 + vDir2*(-C1/2-B*C);
					pLine->AddElement(xy);
					pLine->SetEnd();
				}
			}
		}

		E  = bUpper ? (long)m_uE1 : (long)m_dE1;
		F  = bUpper ? (long)m_uF1 : (long)m_dF1;
		B  = bUpper ? (long)m_uB1 : (long)m_dB1;
		C  = bUpper ? (long)m_uC  : (long)m_dC;
		C1 = bUpper ? (long)m_uC1 : (long)m_dC1;
		D  = bUpper ? (long)m_uD1 : (long)m_dD1;
		M  = bUpper ? (long)m_uM1 : (long)m_dM1;

		for(i=0; i<E+1; i++) 
		{
			for(long n=0; n<B+1; n++) 
			{
				L1 = M-D-F*i;
				xy = gP[8] + vDir1*L1 + vDir2*(C1/2+C*n)*dDir;
				if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
					pBolt->AddCircle(xy,BoltR,pBx);
				if(i==0) pLine->AddElement(xy);
				L1 = -M+D+F*i;
				xy = gP[8] + vDir1*L1 + vDir2*(C1/2+C*n)*dDir;
				if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
					pBolt->AddCircle(xy,BoltR,pBx);
				if(i==0) pLine->AddElement(xy);
				if(i==0) pLine->SetEnd();
				if(n==0)
				{
					L1 = M-D-E*F+F*i;
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2)*dDir;
					pLine->AddElement(xy);
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2+B*C)*dDir;
					pLine->AddElement(xy);
					pLine->SetEnd();
					//
					L1 = -M+D+E*F-F*i;
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2)*dDir;
					pLine->AddElement(xy);
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2+B*C)*dDir;
					pLine->AddElement(xy);
					pLine->SetEnd();
				}
			}
		}
	}
	else
	{
		for(long i=0; i<E+1; i++) 
		{
			for(long n=0; n<B+1; n++) 
			{
				L1 = M-D-F*i;
				xy = gP[8] + vDir1*L1 + vDir2*(C1/2+C*n);
				if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
					pBolt->AddCircle(xy,BoltR,pBx);
				if(i==0) pLine->AddElement(xy);
				L1 = -M+D+F*i;
				xy = gP[8] + vDir1*L1 + vDir2*(C1/2+C*n);
				if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
					pBolt->AddCircle(xy,BoltR,pBx);
				if(i==0) pLine->AddElement(xy);
				if(i==0) pLine->SetEnd();
				if(n==0)
				{
					L1 = M-D-E*F+F*i;
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2);
					pLine->AddElement(xy);
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2+B*C);
					pLine->AddElement(xy);
					pLine->SetEnd();
					//
					L1 = -M+D+E*F-F*i;
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2);
					pLine->AddElement(xy);
					xy = gP[8] + vDir1*L1 + vDir2*(C1/2+B*C);
					pLine->AddElement(xy);
					pLine->SetEnd();
				}
			}
		}
	}
	// 90도 회전 ///////////////////////////////////////////
	if(bRotate)
	{
		long i = 0;
		for(i=0;i<pBolt->GetObjSize();i++)
			pBolt->SetPoint(i,GetXyRotateVector(CDPoint(0,0),pBolt->GetPoint(i),CDPoint(0,1)));
		for(i=0;i<pLine->GetObjSize();i++)
			pLine->SetPoint(i,GetXyRotateVector(CDPoint(0,0),pLine->GetPoint(i),CDPoint(0,1)));
	}

	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}

CPlateCrossBeam& CPlateCrossBeam::operator<<=(const CPlateCrossBeam& Obj)
{
	if( this == &Obj ) return *this;

	CPlateGirderApp *pGir	 = m_pGir;
	POS psBx = GetPOS();

	CPlateSpliceBase::operator=(Obj);
//	*m_pVStiff = *Obj.m_pVStiff;

	SetPOS( psBx );
	m_pGir = pGir;

	m_enTypeNum		= Obj.m_enTypeNum;          
	m_nType         = Obj.m_nType;
	m_H_CR			= Obj.m_H_CR;			
	m_T1_CR			= Obj.m_T1_CR;			
	m_T2_CR			= Obj.m_T2_CR;			
	m_T3_CR			= Obj.m_T3_CR;			
	m_P_CR			= Obj.m_P_CR;			
	m_uN_CR			= Obj.m_uN_CR;			
	m_uM_CR			= Obj.m_uM_CR;			
	m_dN_CR			= Obj.m_dN_CR;			
	m_dM_CR			= Obj.m_dM_CR;			
	m_X_CR			= Obj.m_X_CR;			
	m_vW_CR			= Obj.m_vW_CR;			
	m_vT_CR			= Obj.m_vT_CR;			
	m_vL_CR			= Obj.m_vL_CR;			
	m_hW_CR			= Obj.m_hW_CR;			
	m_hT_CR			= Obj.m_hT_CR;			
	m_nVStiffsu_CR	= Obj.m_nVStiffsu_CR;
	m_nQtyStringer_CR  = Obj.m_nQtyStringer_CR;
	m_nAnchorsu_CR	   = Obj.m_nAnchorsu_CR;
	m_nHStiffUppersu_CR= Obj.m_nHStiffUppersu_CR;
	m_nHStiffLowersu_CR= Obj.m_nHStiffLowersu_CR;
	m_cVSide_CR		   = Obj.m_cVSide_CR;
	m_cGasset_CR	   = Obj.m_cGasset_CR;
	m_bScallopUpperFlange= Obj.m_bScallopUpperFlange;
	m_bScallopLowerFlange= Obj.m_bScallopLowerFlange;

//	for(long a=0; a<9; a++)
//	{
//		m_VStiffDis_CR[a]  = Obj.m_VStiffDis_CR[a];
//		m_AnchorDis_CR[a]  = Obj.m_AnchorDis_CR[a];
//	}
	
	m_nLevelType = Obj.m_nLevelType;
	m_dLH1  = Obj.m_dLH1;
	m_dLH2	= Obj.m_dLH2;
	m_dRH1	= Obj.m_dRH1;
	m_dRH2	= Obj.m_dRH2;
	m_dbL1	= Obj.m_dbL1;
	m_dbL2	= Obj.m_dbL2;
	m_dbL3	= Obj.m_dbL3;
	m_dbL4	= Obj.m_dbL4;
	m_dbR1	= Obj.m_dbR1;
	m_dbR2	= Obj.m_dbR2;
	m_dbR3	= Obj.m_dbR3;
	m_dbR4	= Obj.m_dbR4;
	m_dwB	= Obj.m_dwB;
	m_dwR	= Obj.m_dwR;
	m_dwH	= Obj.m_dwH;
	m_wE	= Obj.m_wE;
	m_wF	= Obj.m_wF;
	m_dltH	= Obj.m_dltH;
	m_ltE	= Obj.m_ltE;
	m_ltF	= Obj.m_ltF;
	m_drtH	= Obj.m_drtH;
	m_rtE	= Obj.m_rtE;
	m_rtF	= Obj.m_rtF;
	m_vW_G  = Obj.m_vW_G;
	m_vW1_G = Obj.m_vW1_G;
	m_vT_G  = Obj.m_vT_G;
	m_uM1  = Obj.m_uM1;
	m_uD1  = Obj.m_uD1;
	m_uE1  = Obj.m_uE1;
	m_uF1  = Obj.m_uF1;
	m_uB1  = Obj.m_uB1;
	m_dM1  = Obj.m_dM1;
	m_dD1  = Obj.m_dD1;
	m_dE1  = Obj.m_dE1;
	m_dF1  = Obj.m_dF1;
	m_dB1  = Obj.m_dB1;

	*m_pSteelMaterialData <<= *Obj.m_pSteelMaterialData;

	return *this;
}

//형강타입의 가로보는 레벨타입니 없는것으로 간주함.
void CPlateCrossBeam::GetCrossXyDom(CDPoint* A)
{
	ZeroMemory(A, sizeof(A));

	CPlateBasicIndex *pBx     = GetBx();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirN   = pGir->m_pRight;
	CPlateBridgeApp  *pBridge = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption    *pGlopt  = pBridge->m_pDataManage->GetGlobalOption();

	if(pGirN == NULL)	return;

	CPlateBasicIndex *pBxN   = pBx->GetBxMatchByCrossBeam(pGirN);
	CPlateCrossBeam  *pCMatch= pBxN->GetCrossBeamLeft();

	CDPoint vX(1,0), vY(0,1);
	CDPoint B[10];
	pGir->GetHunchXyDom(pBx, B);
	long nLevel = pGlopt->GetStrDesignCrossBeamBracketType()=="일반타입" ? 0 : 1;

	double dFlUpSp     = (m_uA+m_uB*m_uC)*2+m_uC1;
	if(GetType()==CR_TYPE_HSTEEL)
		dFlUpSp = 0;

	double dWidthGir   = pGir->GetWidthOnStation(pBx->GetStation(), TRUE);
	double dWidthGirN  = pGirN->GetWidthOnStation(pBxN->GetStation(), TRUE);
	double dThickWeb   = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dThickWebN  = pGirN->GetThickFactByStation(pBxN->GetStation(), G_W);
	double dSharp      = dWidthGir/2-dThickWeb/2;
	double dSharpN     = dWidthGirN/2-dThickWebN/2;
	double dWStiffLR   = m_pVStiff->m_dWidth[1];								//좌측거더오른쪽
	double dWStiffRL   = 0;
	if(pCMatch!=NULL)
		dWStiffRL = pCMatch->m_pVStiff ? pCMatch->m_pVStiff->m_dWidth[0] : 0;	//우측거더왼쪽

	CDPoint xyUpCen = (B[1]+B[2])/2;
	
	if(m_dLH1>0 && m_dRH1>0)  //가로보상판이 거더상판 아래에 있는 경우
	{
		A[0] = (B[1]+B[2])/2-vY*(m_dLH1+m_T2_CR)+vX*dThickWeb/2;
		A[7] = B[7]+vX*(dWidthGirN/2-dThickWebN/2)-vY*(m_dRH1+m_T2_CR);
		CDPoint vCross = (A[7]-A[0]).Unit();
		if(nLevel==1) vCross = CDPoint(1,0);

		if(!m_nLGirSpType)		A[0] += vCross*(dWStiffLR+m_dbL4)/vCross.x;
		if(!m_nRGirSpType)		A[7] -= vCross*(dWStiffRL+m_dbR4)/vCross.x;
		
		A[8].x = (((B[1]+B[2])/2)-vX*dThickWeb/2).x;
		A[8].y =((B[1]+B[2])/2-vY*(m_dLH1+m_T2_CR)-vX*dThickWeb/2).y;
		A[9].x = (B[7]+vX*(dThickWebN/2+dWidthGirN/2)).x;
		A[9].y = (B[7]+vX*(dWidthGirN/2-dThickWebN/2)-vY*(m_dRH1+m_T2_CR)).y;
		if(nLevel==1)
		{
			A[2] = A[8] + vX*(m_P_CR+dThickWeb);
			A[1] = A[2] - vX*(dFlUpSp/2+m_X_CR);
			A[3] = A[2] + vX*(dFlUpSp/2+m_X_CR);
			A[5] = A[9] - vX*(m_P_CR+dThickWebN);
			A[4] = A[5] - vX*(dFlUpSp/2+m_X_CR);
			A[6] = A[5] + vX*(dFlUpSp/2+m_X_CR);
		}
		else
		{
			A[2] = A[8] + vX*dThickWeb + vCross*m_P_CR/vCross.x;
			A[1] = A[2] - vCross*(dFlUpSp/2/vCross.x+m_X_CR);
			A[3] = A[2] + vCross*(dFlUpSp/2/vCross.x+m_X_CR);
			A[5] = A[9] - vX*dThickWebN - vCross*m_P_CR/vCross.x;
			A[4] = A[5] - vCross*(dFlUpSp/2/vCross.x+m_X_CR);
			A[6] = A[5] + vCross*(dFlUpSp/2/vCross.x+m_X_CR);
		}
	}
	else
	{
		A[0] = B[2];
		A[7] = B[7];
		CDPoint vCross = (A[7]-A[0]).Unit();
		if(nLevel==1) vCross = vX;

		if(!m_nLGirSpType)		A[0] += vCross*(dWStiffLR+m_dbL4-dWidthGir/2+dThickWeb/2)/vCross.x;
		if(!m_nRGirSpType)		A[7] -= vCross*(dWStiffRL+m_dbR4-dWidthGirN/2+dThickWebN/2)/vCross.x;

		A[8].x = (((B[1]+B[2])/2)-vX*dThickWeb/2).x;
		A[8].y = B[1].y;
		A[9].x = (B[7]+vX*(dThickWebN/2+dWidthGirN/2)).x;
		A[9].y = B[7].y;	

		A[2] = B[2] + vCross*(m_P_CR-dSharp)/vCross.x;
		A[1] = A[2] - vCross*(dFlUpSp/2/vCross.x+m_X_CR);
		A[3] = A[2] + vCross*(dFlUpSp/2/vCross.x+m_X_CR);
		A[5] = B[7] - vCross*(m_P_CR-dSharpN)/vCross.x;
		A[4] = A[5] - vCross*(dFlUpSp/2/vCross.x+m_X_CR);
		A[6] = A[5] + vCross*(dFlUpSp/2/vCross.x+m_X_CR);
	}
}


CDPoint CPlateCrossBeam::GetVectorCross()
{
	CDPoint A[10];
	GetCrossXyDom(A);

	// m_P_CR 값이 너무 큰 경우 좌표가 역전될 수 있으므로 바로잡는다
	if(A[4].x < A[3].x)
		swap(A[4], A[3]);

	return (A[4]-A[3]).Unit();

}

CDPoint CPlateCrossBeam::GetVectorGasset()
{
	CDPoint A[10];
	GetCrossXyDom(A);

	// m_P_CR 값이 너무 큰 경우 좌표가 역전될 수 있으므로 바로잡는다
	if(A[3].x < A[2].x)
		swap(A[3], A[2]);

	return (A[3]-A[2]).Unit();

}

void CPlateCrossBeam::GetXyBaseLR(CDPoint &xyCrL, CDPoint &xyCrR)
{
	xyCrL = xyCrR = CDPoint(0,0);
	CPlateBasicIndex *pBx = GetBx();	
	if(!pBx) return;

	CPlateGirderApp	 *pGir	= pBx->GetGirder();		
	CPlateGirderApp	 *pGirR	= pGir->m_pRight;

	if(!pGirR)	// 마지막 거더에서 가로보를 찾을경우 이전 거더에서 체크한다
	{
		pGirR	= pGir;
		pGir	= pGir->m_pLeft;
		pBx		= pBx->GetBxMatchByCrossBeam(pGir);
		if(!pBx) return;
	}
	
	CPlateBasicIndex *pBxR	= pBx->GetBxMatchByCrossBeam(pGirR);
	
	if(!pBxR) return;

	double	dGirT	= pGir->GetThickFactByStation(pBx->GetStation(), G_W)/2;
	double	dGirTR	= pGirR->GetThickFactByStation(pBxR->GetStation(), G_W)/2;
	CDPoint xyL		= pGir->GetXyGirderDis(pBx->GetStation());
	CDPoint xyR		= pGirR->GetXyGirderDis(pBxR->GetStation());
	CDPoint vCross	= (xyR-xyL).Unit();
	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());

	xyCrL	= pGir->GetXyGirderDis(pBx->GetStation(), dGirT, vCross.RotateInv(vGir));
	xyCrR	= pGirR->GetXyGirderDis(pBxR->GetStation(), -dGirTR, vCross.RotateInv(vGir));
}

CDPoint CPlateCrossBeam::GetVectorPlan()
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp	*pGir = pBx->GetGirder();
	if(!pBx) return CDPoint(0,0);

	CDPoint xyCrL(0,0), xyCrR(0,0);

	GetXyBaseLR(xyCrL, xyCrR);
	return (xyCrR-xyCrL).Unit();
}

// 거더대비 직각인지, 아닌지
BOOL CPlateCrossBeam::IsSkew()
{
	CPlateBasicIndex *pBx = GetBx();	
	if(!pBx) return FALSE;

	CPlateGirderApp	 *pGir	= pBx->GetGirder();	

	CDPoint vGir	= pGir->GetLine()->GetAngleAzimuth(pBx->GetStation());
	double dCross	= fabs(GetVectorPlan().RotateInv(vGir).y);
	BOOL   bSkew	= Round(dCross,3)==1 ? FALSE : TRUE;

	return bSkew;
}

//<summary>
//가로보 복부판 2D 생성함수
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
//<param name="bShopApply"></param>
void CPlateCrossBeam::CreatePieceObjPlateMainPlate(CPlatePieceObject *pObj)
{
	CPlateBasicIndex	*pBx		= GetBx();
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CPlateBasicIndex	*pBxSec		= pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateBridgeApp		*pDB		= (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp		*pGirCen	= pBx->GetGirder();
	CGlobarOption		*pGlopt		= pDB->m_pDataManage->GetGlobalOption();

	CDPoint A[10], B[10], C[10];
	GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);
	pGir->m_pRight->GetHunchXyDom(pBx, C);

	double  dHeight		= m_H_CR;
	double	dChamfer	= m_T3_CR <=16 ? pGirCen->m_pSangse->m_Chamfer_C1 : pGirCen->m_pSangse->m_Chamfer_C2;
	double	dWebT		= pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double	dWebTN		= pGir->m_pRight->GetThickFactByStation(pBx->GetStation(), 2);
	double	dWidthGirN	=  pGir->m_pRight->GetWidthOnStation(pBxSec->GetStation(), TRUE);
	double	dWVStiffL	= pBx->GetSection()->GetVStiff()->m_dWidth[1];
	double	dWVStiffR	= pBxSec->GetSection()->GetVStiff()->m_dWidth[0];

	CDPoint vX(1,0), vY(0,1);
	long nLevel = pGlopt->GetStrDesignCrossBeamBracketType()=="일반타입" ? 0 : 1;
	CDPoint vCross = nLevel==0 ? (A[7]-A[0]).Unit() : vX;

	if(GetType()==CR_TYPE_HSTEEL)
	{
		CDPoint xy3 = A[0] + vCross * m_dwB;
		CDPoint xy2 = xy3 + vCross.RotateInv90()*m_dwH;
		CDPoint xy1;

		GetXyMatchLineAndLine(xy2, vCross, (B[1]+B[2])/2+vX*(dWebT/2+GetLengthGirWebToCrWeb(TRUE)), vY, xy1);
		pObj->AddElement(xy1, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2, P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx,m_dwR);
		pObj->AddElement(xy3, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);

		xy3 = A[7] - vCross*m_dwB;
		xy2 = xy3  + vCross.RotateInv90()*m_dwH;
		GetXyMatchLineAndLine(xy2, vCross, B[7]+vX*(dWidthGirN/2-dWebTN/2-GetLengthGirWebToCrWeb(FALSE)), vY, xy1);
		pObj->AddElement(xy3,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2,  P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx,m_dwR);
		pObj->AddElement(xy1,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);

		// 하부라인
		xy3 = CDPoint((C[1].x+C[2].x)/2-(m_dwB+dWVStiffR+dWebTN/2+m_dbR3), A[7].y-dHeight);
		xy2 = xy3 - vCross.RotateInv90()*m_dwH;
		GetXyMatchLineAndLine(xy2, vCross, B[7]+vX*(dWidthGirN/2-dWebTN/2-GetLengthGirWebToCrWeb(FALSE)), vY, xy1);
		pObj->AddElement(xy1, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2, P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx,m_dwR);		
		pObj->AddElement(xy3, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,m_dwR);

		xy3 = CDPoint((B[1].x+B[2].x)/2+m_dwB+dWVStiffL+dWebT/2+m_dbL3, A[0].y-dHeight);
		xy2 = xy3 - vCross.RotateInv90()*m_dwH;
		GetXyMatchLineAndLine(xy2, vCross, (B[1]+B[2])/2+vX*(dWebT/2+GetLengthGirWebToCrWeb(TRUE)), vY, xy1);
		pObj->AddElement(xy3,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2,  P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx,m_dwR);
		pObj->AddElement(xy1,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);	
	}
	else
	{
		// 상부라인
		pObj->AddElement(A[2], P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(A[3], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(A[4], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(A[5], P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);

		// 하부라인
		pObj->AddElement(CDPoint(A[5].x,A[5].y-dHeight),P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(CDPoint(A[4].x,A[4].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(CDPoint(A[3].x,A[3].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(CDPoint(A[2].x,A[2].y-dHeight),P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
	}
/*
	if(m_nLGirSpType && m_nRGirSpType)
	{
		// 상부라인
		pObj->AddElement(A[2], P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(A[3], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(A[4], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(A[5], P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);

		// 하부라인
		CDPoint xy = CDPoint(A[5].x,A[5].y-dHeight);
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(CDPoint(A[4].x,A[4].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(CDPoint(A[3].x,A[3].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy = CDPoint(A[2].x,A[2].y-dHeight);
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
	}
	else if(!m_nLGirSpType && m_nRGirSpType)
	{
		CDPoint xy3 = A[0] + vCross * m_dwB;
		CDPoint xy2 = xy3+vCross.RotateInv90()*m_dwH;
		CDPoint xy1;
		GetXyMatchLineAndLine(xy2, vCross, (B[1]+B[2])/2+vX*(dWebT/2+GetLengthGirWebToCrWeb(TRUE)), vY, xy1);
		// 상부라인
		pObj->AddElement(xy1);
		pObj->AddElement(xy2, P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx, m_dwR);
		pObj->AddElement(xy3);
		pObj->AddElement(A[3], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(A[4], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(A[5], P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		// 하부라인
		xy2 = CDPoint(A[0].x,A[0].y-dHeight);
		GetXyMatchLineAndLine(xy2, vCross, (B[1]+B[2])/2+vX*(dWebT/2+GetLengthGirWebToCrWeb(TRUE)), vY, xy1);

		CDPoint xy = CDPoint(A[5].x,A[5].y-dHeight);
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(CDPoint(A[4].x,A[4].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(CDPoint(A[3].x,A[3].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(xy2,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,m_dwR);
		pObj->AddElement(xy1,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
	}
	else if(!m_nRGirSpType && m_nLGirSpType)
	{
		CDPoint xy3 = A[7] - vCross * m_dwB;
		CDPoint xy2 = xy3  + vCross.RotateInv90()*m_dwH;
		CDPoint xy1;
		GetXyMatchLineAndLine(xy2, vCross, B[7]+vX*(dWidthGirN/2-(dWebTN/2+GetLengthGirWebToCrWeb(FALSE))), vY, xy1);
		// 상부라인
		pObj->AddElement(A[2], P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(A[3], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(A[4], P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(xy3,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2,  P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx,m_dwR);
		pObj->AddElement(xy1,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		// 하부라인
		xy2 = CDPoint(A[7].x,A[7].y-dHeight);
		GetXyMatchLineAndLine(xy2, vCross, B[7]+vX*(dWidthGirN/2-(dWebTN/2+GetLengthGirWebToCrWeb(FALSE))), vY, xy1);

		pObj->AddElement(xy1,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);		
		pObj->AddElement(CDPoint(A[4].x,A[4].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		pObj->AddElement(CDPoint(A[3].x,A[3].y-dHeight),P_TYPE_LINE|P_TYPE_DIMPOINT);
		CDPoint xy = CDPoint(A[2].x,A[2].y-dHeight);
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
	}
	else//양쪽이 모두 볼트이음인 경우(H형강타입)은 레벨타입을 지원하지 않음.
	{
		CDPoint xy3 = A[0] + vCross * m_dwB;
		CDPoint xy2 = xy3 + vCross.RotateInv90()*m_dwH;
		CDPoint xy1;

		GetXyMatchLineAndLine(xy2, vCross, (B[1]+B[2])/2+vX*(dWebT/2+GetLengthGirWebToCrWeb(TRUE)), vY, xy1);
		pObj->AddElement(xy1, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2, P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx,m_dwR);
		pObj->AddElement(xy3, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);

		xy3 = A[7] - vCross*m_dwB;
		xy2 = xy3  + vCross.RotateInv90()*m_dwH;
		GetXyMatchLineAndLine(xy2, vCross, B[7]+vX*(dWidthGirN/2-dWebTN/2-GetLengthGirWebToCrWeb(FALSE)), vY, xy1);
		pObj->AddElement(xy3,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2,  P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy1,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		// 하부라인
		xy2 = CDPoint(A[7].x,A[7].y-dHeight);
		GetXyMatchLineAndLine(xy2, vCross, B[7]+vX*(dWidthGirN/2-dWebTN/2-GetLengthGirWebToCrWeb(FALSE)), vY, xy1);
		pObj->AddElement(xy1,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy2,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,m_dwR);		
		xy2 = CDPoint(A[0].x,A[0].y-dHeight);
		GetXyMatchLineAndLine(xy2, vCross, (B[1]+B[2])/2+vX*(dWebT/2+GetLengthGirWebToCrWeb(TRUE)), vY, xy1);
		pObj->AddElement(xy2,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);
		pObj->AddElement(xy1,  P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx,dChamfer);	

	}
	// FACE
		*/
	pObj->RegenVertex();
}

void CPlateCrossBeam::CreatePieceObjPlateMainPlateBolt(CPlatePieceObject* pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	CPlateBasicIndex *pBx  = GetBx();
	
	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight ? pGir->m_pRight : pGir);
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen = pBx->GetGirder();

	CDPoint A[10], B[10];
	GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	double dWebT  = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dWebTN = pGir->m_pRight->GetThickFactByStation(pBxR->GetStation(), G_W);
	double dWidthGirN = pGir->m_pRight->GetWidthOnStation(pBxR->GetStation(), TRUE);
	CDPoint vCross = GetVectorCross();
	CDPoint vX(1,0), vY(0,1);

	//볼트홀
	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);
	double dBoltR = pDB->GetBoltRad(m_ZzsBoltSize);
	
	CDPoint xyStt, xyEnd, xyMid;
	long	n = 0;
	if(!m_nLGirSpType)
	{
		CDPoint vCross   = (A[7]-A[0]).Unit();
		CDPoint xy3 = A[0]+vCross*m_dwB;
		CDPoint xy2 = xy3+vCross.RotateInv90()*m_dwH;
		GetXyMatchLineAndLine(xy2, vCross, (B[1]+B[2])/2+vX*(dWebT/2+GetLengthGirWebToCrWeb(TRUE)), vY, xyStt);
		xyEnd = xyStt - vY*(m_H_CR-2*m_dwH);
		xyMid = (xyStt+xyEnd)/2 + vCross*m_dbL1;

		if(m_wE%2==0)
		{
			long nHalf = m_wE/2;

			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*m_wF*n, dBoltR);
				pLine->AddElement(xyMid+vY*m_wF*n);
			}
			pLine->SetEnd();
			pLine->AddElement(xyMid);
			for(n=1; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*m_wF*n, dBoltR);
				pLine->AddElement(xyMid-vY*m_wF*n);
				if(n==nHalf)
					xyBoltPos[0] = xyMid-vY*m_wF*n;
			}
			pLine->SetEnd();
		}
		else
		{
			long nHalf = (m_wE-1)/2;
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*(m_wF/2+m_wF*n), dBoltR);
				pLine->AddElement(xyMid+vY*(m_wF/2+m_wF*n));
			}
			pLine->SetEnd();
			
			pLine->AddElement(xyMid+vY*m_wF/2);
			pLine->AddElement(xyMid-vY*m_wF/2);
			pLine->SetEnd();
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*(m_wF/2+m_wF*n), dBoltR);
				pLine->AddElement(xyMid-vY*(m_wF/2+m_wF*n));
				if(n==nHalf)
					xyBoltPos[0] = xyMid-vY*(m_wF/2+m_wF*n);
			}
			pLine->SetEnd();

		}
	}
		
	if(!m_nRGirSpType)
	{
		
		

		CDPoint vCross = (A[7]-A[0]).Unit();
		CDPoint xy3 = A[7] - vCross*m_dwB;
		CDPoint xy2 = xy3  + vCross.RotateInv90()*m_dwH;
		CDPoint xyStt;
		GetXyMatchLineAndLine(xy2, vCross, B[7]+vX*(dWidthGirN/2-(dWebTN/2+GetLengthGirWebToCrWeb(FALSE))), vY, xyStt);
		xyEnd = xyStt - vY*(m_H_CR-2*m_dwH);
		xyMid = (xyStt+xyEnd)/2 - vCross*m_dbR1;

		if(m_wE%2==0)
		{
			long nHalf = m_wE/2;
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*m_wF*n, dBoltR);
				pLine->AddElement(xyMid+vY*m_wF*n);
			}
			pLine->SetEnd();
			pLine->AddElement(xyMid);
			for(n=1; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*m_wF*n, dBoltR);
				pLine->AddElement(xyMid-vY*m_wF*n);
				if(n==nHalf)
					xyBoltPos[2] = xyMid-vY*m_wF*n;
			}
			pLine->SetEnd();

		}
		else
		{
			long nHalf = (m_wE-1)/2;
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*(m_wF/2+m_wF*n), dBoltR);
				pLine->AddElement(xyMid+vY*(m_wF/2+m_wF*n));
			}
			pLine->AddElement(xyMid+vY*m_wF/2);
			pLine->AddElement(xyMid-vY*m_wF/2);
			pLine->SetEnd();
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*(m_wF/2+m_wF*n), dBoltR);
				pLine->AddElement(xyMid-vY*(m_wF/2+m_wF*n));
				if(n==nHalf)
					xyBoltPos[2] = xyMid-vY*(m_wF/2+m_wF*n);
			}

			pLine->SetEnd();
		}
	}

	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}
void CPlateCrossBeam::CreatePieceObjPlateFlange(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx   = GetBx();
	CPlateGirderApp  *pGir  = pBx->GetGirder();
	CPlateGirderApp  *pGirN = pGir->m_pRight;
	CPlateBasicIndex *pBxN  = pBx->GetBxMatchByCrossBeam(pGirN);
	CPlateCrossBeam  *pCR   = pBxN->GetCrossBeam();

	double dWebT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWebTN = pGir->GetThickFactByStation(pBxN->GetStation(), 2);
	//double Zorib = 0;//bPieceApply ? fabs(pData->m_dCrossWebAssmByWeld)  : 0;
	double Weld  = 0;//bPieceApply ? fabs(pData->m_dCrossWebWeldByWidth) : 0;
	double WeldT = 0;//pData->m_dCrossWeld;
	double plus  = (m_vT_CR<=WeldT) ? Weld*m_nVStiffsu_CR : 0;

	CDPoint A[10];
	GetCrossXyDom(A);

	double  cW   = m_uM_CR; // 가로보 폭의 1/2 (상판)
	CDPoint vDir = (A[3]-A[2]).Unit();
	double  gLen = (A[3].x-A[2].x)/vDir.x;  // gSlop를 따르는 길이
	        vDir = (A[4]-A[3]).Unit();
	double  bLen = (A[4].x-A[3].x+plus)/vDir.x;		// bSlop를 따르는 길이

	if(fabs(fabs(gLen)*2 - fabs(bLen)) < 0.000000001) return;
	
	CDPoint xy;
	//좌측상단에서(위에서볼때 시계방향배열)
	if(!m_nLGirSpType)		xy.x = A[8].x+dWebT+GetLengthGirWebToCrWeb(TRUE);
	else					xy.x = A[2].x; 
	xy.y = cW;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);

	xy.x = A[2].x+gLen;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
	
	xy.x = A[2].x+gLen+bLen;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
	
	if(!m_nRGirSpType)		xy.x = A[9].x-dWebTN-GetLengthGirWebToCrWeb(FALSE);
	else					xy.x = A[5].x;//A[2].x+gLen*2+bLen;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
	//
	xy.y = m_T3_CR/2;
	pObj->AddElement(xy,P_TYPE_DIMPOINT);
	//
	if(!m_nRGirSpType)		xy.x = A[9].x-dWebTN-GetLengthGirWebToCrWeb(FALSE);
	else					xy.x = A[5].x;//A[2].x+gLen*2+bLen;
	xy.y = -cW;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);

	xy.x = A[2].x+gLen+bLen;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);

	xy.x = A[2].x+gLen;
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);

	if(!m_nLGirSpType)		xy.x = A[8].x+dWebT+GetLengthGirWebToCrWeb(TRUE);
	else					xy.x = A[2].x; 
	pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
	//
	xy.y = m_T3_CR/2;
	pObj->AddElement(xy,P_TYPE_DIMPOINT);
	xy.y = cW;
	pObj->AddElement(xy,P_TYPE_DIMPOINT,-1,pBx);

	pObj->RegenVertex();
}
//<summary>
//가로보 상판 2D 볼트홀 및 볼트선
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
//<param name="bMakeBoltHole"></param>
//<param name="bMakeBoltLine"></param>
void CPlateCrossBeam::CreatePieceObjPlateFlangeBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	if(pObj->GetObjSize() == 0) return;
	if(!bMakeBoltHole && !bMakeBoltLine) return;

	CPlateBasicIndex *pBx   = GetBx();
	CPlateGirderApp  *pGir  = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption    *pGlopt= pDB->m_pDataManage->GetGlobalOption();

	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	long nMarkBuje = pBx->IsJijum() ? CP_F_U : CG_F_U;
	BOOL bUpper = (pObj->m_nMarkBuje==nMarkBuje) ? TRUE : FALSE;

	long   E  = bUpper ? m_uE : m_dE;
	double F  = bUpper ? m_uF : m_dF;
	long   B  = bUpper ? m_uB    : m_dB;
	double C  = bUpper ? m_uC    : m_dC;
	double C1 = bUpper ? m_uC1   : m_dC1;
	double D  = bUpper ? m_uD    : m_dD;
	double M  = bUpper ? m_uM_CR : m_dM_CR;
	BOOL bZigZag= FALSE;
	if(pGlopt->GetDesignBoltHoleJigJaegOfHorPlate()==0 && E>0 && F>0)
		bZigZag= TRUE;

	CDPoint	xy,sp,xyBolt;
	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);

	// 좌측 볼트
	sp = pObj->GetSttFacePoint(0);
	sp.y -= M;
	long i = 0;
	for(i=0; i<E+1; i++) 
	{
		xy.y = sp.y + M - D - i*F;
		pLine->AddElement(CDPoint(sp.x+C1/2,xy.y));
		pLine->AddElement(CDPoint(sp.x+C1/2+B*C,xy.y));
		pLine->SetEnd();
		pLine->AddElement(CDPoint(sp.x+C1/2,-xy.y));
		pLine->AddElement(CDPoint(sp.x+C1/2+B*C,-xy.y));
		pLine->SetEnd();
		for(long n=0; n<B+1; n++) 
		{
			xyBolt.y = sp.y + M - D - i*F;
			xyBolt.x = sp.x + C1/2 + n*C;
			if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
				pBolt->AddCircle(xyBolt,BoltR,pBx);

			xyBolt.y *= -1;
			if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
				pBolt->AddCircle(xyBolt,BoltR,pBx);

			if(i==0)
			{
				pLine->AddElement(CDPoint(xyBolt.x,sp.y+M-D));
				pLine->AddElement(CDPoint(xyBolt.x,sp.y-M+D));
				pLine->SetEnd();
			}
		}
	}

	// 우측 볼트
	sp = pObj->GetEndFacePoint(0);
	sp.y -= M;
	for(i=0; i<E+1; i++) 
	{
		xy.y = sp.y + M - D - i*F;
		pLine->AddElement(CDPoint(sp.x-C1/2    ,xy.y));
		pLine->AddElement(CDPoint(sp.x-C1/2-B*C,xy.y));
		pLine->SetEnd();
		pLine->AddElement(CDPoint(sp.x-C1/2    ,sp.y-M+D+i*F));
		pLine->AddElement(CDPoint(sp.x-C1/2-B*C,sp.y-M+D+i*F));
		pLine->SetEnd();
		for(long n=0; n<B+1; n++) 
		{
//			xy.y = sp.y + M - D - i*F;
//			xy.x = sp.x - C1/2 - n*C;
			xyBolt.y = sp.y + M - D - i*F;
			xyBolt.x = sp.x - C1/2 - n*C;
			if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
				pBolt->AddCircle(xyBolt,BoltR,pBx);
			xyBolt.y *= -1;
//			xyBolt.y = sp.y - M + D + i*F;
			if(!bZigZag || (i%2==0 && n%2==0) || (i%2==1 && n%2==1))
				pBolt->AddCircle(xyBolt,BoltR,pBx);
			if(i==0)
			{
				pLine->AddElement(CDPoint(xyBolt.x,sp.y+M-D));
				pLine->AddElement(CDPoint(xyBolt.x,sp.y-M+D));
				pLine->SetEnd();
			}
		}
	}
	///////////////////////////////////////////////////////////////
	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}


//<summary>
//가로보 플랜지 현장이음판 볼트
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
//<param name="bMakeBoltHole"></param>
//<param name="bMakeBoltLine"></param>
void CPlateCrossBeam::CreatePieceObjPlateSpliceFlangeUpperUpperBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CMatrixPoint *xyMat = new CMatrixPoint;
	CMatrixPoint *xyMat2 = new CMatrixPoint;
	CMatrixPoint *xyMatLine = new CMatrixPoint;
	CMatrixPoint *xyMatLine2 = new CMatrixPoint;
	if(m_bConnectionPlate)
	{
		GetXyMatrixFlangeUpperConnectionPlate(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE, TRUE);
		GetXyMatrixFlangeUpperConnectionPlate(xyMat2, pObj->m_nRow, pObj->m_nCol, FALSE, FALSE);
		GetXyMatrixFlangeUpperConnectionPlate(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE, FALSE);
		GetXyMatrixFlangeUpperConnectionPlate(xyMatLine2, pObj->m_nRow, pObj->m_nCol, TRUE, FALSE);
	}
	else
	{
		GetXyMatrixFlangeUpper(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE);
		GetXyMatrixFlangeUpper(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE);
	}

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1, xy2;
	// 볼트홀
	long nSttCol = nDir == 1  ? xyMat->GetCols()/2 : 0;
	long nEndCol = nDir == -1 ? xyMat->GetCols()/2 : xyMat->GetCols();
	long nSttRow = 0;
	long nEndRow = xyMat->GetRows();
	long r = 0;
	for(r=nSttRow; r<nEndRow; r++)
	{
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			if (xy1==CDPoint(NULL,NULL)) continue;
			pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}
	if(m_bConnectionPlate)
	{
		nSttCol = nDir == 1  ? xyMat2->GetCols()/2 : 0;
		nEndCol = nDir == -1 ? xyMat2->GetCols()/2 : xyMat2->GetCols();
		nSttRow = 0;
		nEndRow = xyMat2->GetRows();

		for(r=nSttRow; r<nEndRow; r++)
		{
			for(long c=nSttCol; c<nEndCol; c++)
			{
				xy1 = xyMat2->GetMatrixPoint(r,c);
				if (xy1==CDPoint(NULL,NULL)) continue;
				pObjBolt->AddCircle(xy1,BoltR,GetBx());
			}
		}
	}

	long nSttColLine = nDir == 1  ? xyMatLine->GetCols()/2 : 0;
	long nEndColLine = nDir == -1 ? xyMatLine->GetCols()/2 : xyMatLine->GetCols();
	long nSttRowLine = 0;
	long nEndRowLine = xyMatLine->GetRows();

	if(m_bConnectionPlate)
		nSttColLine = xyMatLine->GetCols()/2;
	// 볼트라인 : 가로방향
	for(r=nSttRowLine; r<nEndRowLine; r++)
	{
		xy1 = xyMatLine->GetMatrixPoint(r,nSttColLine);
		xy2 = xyMatLine->GetMatrixPoint(r,nEndColLine-1);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}

	// 볼트라인 : 세로방향
	for(long c=nSttColLine; c<nEndColLine; c++)
	{
		xy1 = xyMatLine->GetMatrixPoint(nSttRowLine,c);
		xy2 = xyMatLine->GetMatrixPoint(nEndRowLine-1,c);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	if(m_bConnectionPlate)
	{
		nSttColLine = nDir == 1  ? xyMatLine2->GetCols()/2 : 0;
		nEndColLine = nDir == -1 ? xyMatLine2->GetCols()/2 : xyMatLine2->GetCols();
		nSttRowLine = 0;
		nEndRowLine = xyMatLine2->GetRows();

		if(m_bConnectionPlate)
			nSttColLine = xyMatLine2->GetCols()/2;
		// 볼트라인 : 가로방향
		for(r=nSttRowLine; r<nEndRowLine; r++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(r,nSttColLine);
			xy2 = xyMatLine2->GetMatrixPoint(r,nEndColLine-1);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
		// 볼트라인 : 세로방향
		for(long c=nSttColLine; c<nEndColLine; c++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(nSttRowLine,c);
			xy2 = xyMatLine2->GetMatrixPoint(nEndRowLine-1,c);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
	}
	
	delete xyMat;
	delete xyMat2;
	delete xyMatLine;
	delete xyMatLine2;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);

	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;
	if (bMakeBoltLine)
		pObj->AddChildObj(pObjLine);
	else
		delete pObjLine;

}

void CPlateCrossBeam::CreatePieceObjPlateSpliceFlangeUpperLowerBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CMatrixPoint *xyMat = new CMatrixPoint;
	CMatrixPoint *xyMat2 = new CMatrixPoint;
	CMatrixPoint *xyMatLine = new CMatrixPoint;
	CMatrixPoint *xyMatLine2 = new CMatrixPoint;
	if(m_bConnectionPlate)
	{
		GetXyMatrixFlangeUpperConnectionPlate(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE, TRUE);
		GetXyMatrixFlangeUpperConnectionPlate(xyMat2, pObj->m_nRow, pObj->m_nCol, FALSE, FALSE);
		GetXyMatrixFlangeUpperConnectionPlate(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE, TRUE);
		GetXyMatrixFlangeUpperConnectionPlate(xyMatLine2, pObj->m_nRow, pObj->m_nCol, TRUE, FALSE);
	}
	else
	{
		GetXyMatrixFlangeUpper(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE);
		GetXyMatrixFlangeUpper(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE);
	}

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1, xy2;
	// 볼트홀
	long nSttRow = pObj->m_bFront == 1 ? xyMat->GetRows()/2 : 0;
	long nEndRow = pObj->m_bFront == 0 ? xyMat->GetRows()/2 : xyMat->GetRows();
	long r = 0;
	long c = 0;
	for(r=nSttRow; r<nEndRow; r++)
	{
		for(long c=0; c<xyMat->GetCols(); c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			if (xy1==CDPoint(NULL,NULL)) continue;
			pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}

	if(m_bConnectionPlate)
	{
		long nSttRow = pObj->m_bFront == 1 ? xyMat2->GetRows()/2 : 0;
		long nEndRow = pObj->m_bFront == 0 ? xyMat2->GetRows()/2 : xyMat2->GetRows();
		for(r=nSttRow; r<nEndRow; r++)
		{
			for(c=0; c<xyMat2->GetCols(); c++)
			{
				xy1 = xyMat2->GetMatrixPoint(r,c);
				if (xy1==CDPoint(NULL,NULL)) continue;
				pObjBolt->AddCircle(xy1,BoltR,GetBx());
			}
		}
	}
	if(m_bConnectionPlate)
	{
		long nSttColLine = nDir == 1  ? xyMatLine->GetCols()/2 : 0;
		long nEndColLine = nDir == -1 ? xyMatLine->GetCols()/2 : xyMatLine->GetCols();
		long nSttRowLine = 0;
		long nEndRowLine = xyMatLine->GetRows();

		if(m_bConnectionPlate)
		{
			nSttColLine = xyMatLine->GetCols()/2;
		}
		// 볼트라인 : 가로방향
		for(r=nSttRowLine; r<nEndRowLine; r++)
		{
			xy1 = xyMatLine->GetMatrixPoint(r,nSttColLine);
			xy2 = xyMatLine->GetMatrixPoint(r,nEndColLine-1);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
		// 볼트라인 : 세로방향
		for(c=nSttColLine; c<nEndColLine; c++)
		{
			xy1 = xyMatLine->GetMatrixPoint(nSttRowLine,c);
			xy2 = xyMatLine->GetMatrixPoint(nEndRowLine-1,c);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}

		nSttColLine = nDir == 1  ? xyMatLine2->GetCols()/2 : 0;
		nEndColLine = nDir == -1 ? xyMatLine2->GetCols()/2 : xyMatLine2->GetCols();
		nSttRowLine = 0;
		nEndRowLine = xyMatLine2->GetRows();

		if(m_bConnectionPlate)
		{
			nSttColLine = xyMatLine2->GetCols()/2;
		}
		// 볼트라인 : 가로방향
		for(long r=nSttRowLine; r<nEndRowLine; r++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(r,nSttColLine);
			xy2 = xyMatLine2->GetMatrixPoint(r,nEndColLine-1);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
		// 볼트라인 : 세로방향
		for(c=nSttColLine; c<nEndColLine; c++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(nSttRowLine,c);
			xy2 = xyMatLine2->GetMatrixPoint(nEndRowLine-1,c);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
	}
	else
	{
		long nSttRowLine = pObj->m_bFront == 1 ? xyMatLine->GetRows()/2 : 0;
		long nEndRowLine = pObj->m_bFront == 0 ? xyMatLine->GetRows()/2 : xyMatLine->GetRows();
		// 볼트라인 : 가로방향
		for(long r=nSttRowLine; r<nEndRowLine; r++)
		{
			xy1 = xyMatLine->GetMatrixPoint(r,0);
			xy2 = xyMatLine->GetMatrixPoint(r,xyMatLine->GetCols()-1);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
		// 볼트라인 : 세로방향
		for(c=0; c<xyMatLine->GetCols(); c++)
		{
			xy1 = xyMatLine->GetMatrixPoint(nSttRowLine,c);
			xy2 = xyMatLine->GetMatrixPoint(nEndRowLine-1,c);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
	}
	delete xyMat;
	delete xyMat2;
	delete xyMatLine;
	delete xyMatLine2;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);
	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;
	if (bMakeBoltLine)
		pObj->AddChildObj(pObjLine);
	else
		delete pObjLine;
}

void CPlateCrossBeam::GetXyMatrixFlangeLowerConnectionPlate(CMatrixPoint *xyMat, long nRow,long nCol, BOOL bLine, BOOL bCrossConnect)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption	*pGlopt = pDB->m_pDataManage->GetGlobalOption();

	BOOL bZigZag= FALSE;
	if(!bLine && pGlopt->GetDesignBoltHoleJigJaegOfHorPlate()==0 && m_dE>0 && m_dF>0)
		bZigZag= TRUE;
	double	GapCutting = 0;//pGlopt->GetBracketGapOfCutting();
	long	nRowSu = m_dE+1;
	long	nColSu = m_dB+1;
	xyMat->SetSize(nRowSu*2, nColSu*2);
	CDPoint cp = CDPoint(0,0);
	double dY = m_dM_CR-m_dD;
	CDPoint xy = CDPoint(0,0);
	long	nDir	= nCol==1 ? 1 : -1;
	// 위쪽으로
	if(bCrossConnect)
	{
		// 가로보와의 연결되는 볼트
		for(long r=0; r<nRowSu; r++)
		{
			for(long c=0; c<nColSu; c++)
			{
				xy.x = -m_dC1/2*nDir - m_dC*c*nDir; // 좌측
				xy.y = dY-m_dF*r;
				if (nCol==1)
					xy.x -= GapCutting;
				if(!bZigZag || (r%2)==(c%2))
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y+xy.y));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y-xy.y));
				}
				else
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(NULL,NULL));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(NULL,NULL));
				}
			}
		}
	}
	else
	{
		// 가로보, 거세트와 연결되는 볼트
		nRowSu = (long)m_dE1+1;
		nColSu = (long)m_dB1+1;
		xyMat->SetSize(nRowSu*2, nColSu*2);
		dY = m_dM1-m_dD1;

		for(long r=0; r<nRowSu; r++)
		{
			for(long c=0; c<nColSu; c++)
			{
				xy.x = m_dC1/2*nDir + m_dC*c*nDir; // 좌측
				xy.y = dY-m_dF1*r;
				if (nCol==1)
					xy.x -= GapCutting;
				if(!bZigZag || (r%2)==(c%2))
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y+xy.y));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(cp.x+xy.x, cp.y-xy.y));
				}
				else
				{
					xyMat->SetMatrixPoint(r,nColSu-1-c,CDPoint(NULL,NULL));
					xyMat->SetMatrixPoint(nRowSu*2-1-r  ,nColSu-1-c,CDPoint(NULL,NULL));
				}


			}
		}
	}	
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceFlangeLowerUpperBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CMatrixPoint *xyMat = new CMatrixPoint;
	CMatrixPoint *xyMat2 = new CMatrixPoint;
	CMatrixPoint *xyMatLine = new CMatrixPoint;
	CMatrixPoint *xyMatLine2 = new CMatrixPoint;
	if(m_bConnectionPlate)
	{
		GetXyMatrixFlangeLowerConnectionPlate(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE, TRUE);
		GetXyMatrixFlangeLowerConnectionPlate(xyMat2, pObj->m_nRow, pObj->m_nCol, FALSE, FALSE);
		GetXyMatrixFlangeLowerConnectionPlate(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE, TRUE);
		GetXyMatrixFlangeLowerConnectionPlate(xyMatLine2, pObj->m_nRow, pObj->m_nCol, TRUE, FALSE);
	}
	else
	{
		GetXyMatrixFlangeLower(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE);
		GetXyMatrixFlangeLower(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE);
	}

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1, xy2;
	// 볼트홀
	long nSttRow = pObj->m_bFront == 1 ? xyMat->GetRows()/2 : 0;
	long nEndRow = pObj->m_bFront == 0 ? xyMat->GetRows()/2 : xyMat->GetRows();
	long nSttCol = 0;
	long nEndCol = xyMat->GetCols();
	long r = 0;
	for(r=nSttRow; r<nEndRow; r++)
	{
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			if (xy1==CDPoint(NULL,NULL)) continue;
			pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}

	if(m_bConnectionPlate)
	{
		nSttCol = nDir == 1  ? xyMat2->GetCols()/2 : 0;
		nEndCol = nDir == -1 ? xyMat2->GetCols()/2 : xyMat2->GetCols();
		nSttRow = 0;
		nEndRow = xyMat2->GetRows();
		for(r=nSttRow; r<nEndRow; r++)
		{
			for(long c=nSttCol; c<nEndCol; c++)
			{
				xy1 = xyMat2->GetMatrixPoint(r,c);
				if (xy1==CDPoint(NULL,NULL)) continue;
				pObjBolt->AddCircle(xy1,BoltR,GetBx());
			}
		}
	}

	nSttCol = 0;
	nEndCol = xyMatLine->GetCols();
	nSttRow = 0;
	nEndRow = xyMatLine->GetRows();
	if(m_bConnectionPlate)
	{
		nSttCol = xyMatLine->GetCols()/2;
	}
	// 볼트라인 : 가로방향
	for(r=nSttRow; r<nEndRow; r++)
	{
		xy1 = xyMatLine->GetMatrixPoint(r,nSttCol);
		xy2 = xyMatLine->GetMatrixPoint(r,nEndCol-1);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	// 볼트라인 : 세로방향
	for(long c=nSttCol; c<nEndCol; c++)
	{
		xy1 = xyMatLine->GetMatrixPoint(nSttRow,c);
		xy2 = xyMatLine->GetMatrixPoint(nEndRow-1,c);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}

	if(m_bConnectionPlate)
	{
		nSttCol = xyMatLine2->GetCols()/2;
		nEndCol = nDir == -1 ? xyMatLine2->GetCols()/2 : xyMatLine2->GetCols();
		nSttRow = 0;
		nEndRow = xyMatLine2->GetRows();
		for(r=nSttRow; r<nEndRow; r++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(r,nSttCol);
			xy2 = xyMatLine2->GetMatrixPoint(r,xyMatLine2->GetCols()-1);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
		// 볼트라인 : 세로방향
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(nSttRow,c);
			xy2 = xyMatLine2->GetMatrixPoint(nEndRow-1,c);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
	}
	delete xyMat;
	delete xyMat2;
	delete xyMatLine;
	delete xyMatLine2;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);
	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;
	if (bMakeBoltLine)
		pObj->AddChildObj(pObjLine);
	else
		delete pObjLine;
}

void CPlateCrossBeam::CreatePieceObjPlateSpliceFlangeLowerLowerBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine, long nDir)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	double BoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CMatrixPoint *xyMat = new CMatrixPoint;
	CMatrixPoint *xyMat2 = new CMatrixPoint;
	CMatrixPoint *xyMatLine = new CMatrixPoint;
	CMatrixPoint *xyMatLine2 = new CMatrixPoint;
	if(m_bConnectionPlate)
	{
		GetXyMatrixFlangeLowerConnectionPlate(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE, TRUE);
		GetXyMatrixFlangeLowerConnectionPlate(xyMat2, pObj->m_nRow, pObj->m_nCol, FALSE, FALSE);
		GetXyMatrixFlangeLowerConnectionPlate(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE, TRUE);
		GetXyMatrixFlangeLowerConnectionPlate(xyMatLine2, pObj->m_nRow, pObj->m_nCol, TRUE, FALSE);
	}
	else
	{
		GetXyMatrixFlangeLower(xyMat, pObj->m_nRow, pObj->m_nCol, FALSE);
		GetXyMatrixFlangeLower(xyMatLine, pObj->m_nRow, pObj->m_nCol, TRUE);
	}

	CPlatePieceObject *pObjBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pObjLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pObjBolt->SetLayer(HCAD_STLC);
	pObjLine->SetLayer(HCAD_STLC);

	CDPoint xy1, xy2;
	// 볼트홀
	long nSttCol = nDir == 1  ? xyMat->GetCols()/2 : 0;
	long nEndCol = nDir == -1 ? xyMat->GetCols()/2 : xyMat->GetCols();
	long nSttRow = 0;
	long nEndRow = xyMat->GetRows();
	if (!m_cGasset_CR || m_H_CR == pBx->GetHeightGirderByBx())
	{
		nSttRow = 0;
		nEndRow = xyMat->GetRows();
	}
	long r = 0;
	for(r=nSttRow; r<nEndRow; r++)
	{
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMat->GetMatrixPoint(r,c);
			if (xy1==CDPoint(NULL,NULL)) continue;
			pObjBolt->AddCircle(xy1,BoltR,GetBx());
		}
	}

	if(m_bConnectionPlate)
	{
		nSttCol = nDir == 1  ? xyMat2->GetCols()/2 : 0;
		nEndCol = nDir == -1 ? xyMat2->GetCols()/2 : xyMat2->GetCols();
		nSttRow = 0;
		nEndRow = xyMat2->GetRows();

		if (!m_cGasset_CR || m_H_CR == pBx->GetHeightGirderByBx())
		{
			nSttRow = 0;
			nEndRow = xyMat2->GetRows();
		}

		for(long r=nSttRow; r<nEndRow; r++)
		{
			for(long c=nSttCol; c<nEndCol; c++)
			{
				xy1 = xyMat2->GetMatrixPoint(r,c);
				if (xy1==CDPoint(NULL,NULL)) continue;
				pObjBolt->AddCircle(xy1,BoltR,GetBx());
			}
		}
	}

	nSttCol = nDir == 1  ? xyMatLine->GetCols()/2 : 0;
	nEndCol = nDir == -1 ? xyMatLine->GetCols()/2 : xyMatLine->GetCols();
	nSttRow = 0;
	nEndRow = xyMatLine->GetRows();
	if(m_bConnectionPlate)
	{
		nSttCol = xyMatLine->GetCols()/2;
	}
	// 볼트라인 : 가로방향
	for(r=nSttRow; r<nEndRow; r++)
	{
		xy1 = xyMatLine->GetMatrixPoint(r,nSttCol);
		xy2 = xyMatLine->GetMatrixPoint(r,nEndCol-1);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}
	// 볼트라인 : 세로방향
	for(long c=nSttCol; c<nEndCol; c++)
	{
		xy1 = xyMatLine->GetMatrixPoint(nSttRow,c);
		xy2 = xyMatLine->GetMatrixPoint(nEndRow-1,c);
		pObjLine->AddElement(xy1);
		pObjLine->AddElement(xy2);
		pObjLine->SetEnd();
	}

	if(m_bConnectionPlate)
	{
		nSttCol = xyMatLine2->GetCols()/2;
		nEndCol = nDir == -1 ? xyMatLine2->GetCols()/2 : xyMatLine2->GetCols();
		nSttRow = 0;
		nEndRow = xyMatLine2->GetRows();
		for(r=nSttRow; r<nEndRow; r++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(r,nSttCol);
			xy2 = xyMatLine2->GetMatrixPoint(r,nEndCol-1);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
		// 볼트라인 : 세로방향
		for(long c=nSttCol; c<nEndCol; c++)
		{
			xy1 = xyMatLine2->GetMatrixPoint(nSttRow,c);
			xy2 = xyMatLine2->GetMatrixPoint(nEndRow-1,c);
			pObjLine->AddElement(xy1);
			pObjLine->AddElement(xy2);
			pObjLine->SetEnd();
		}
	}

	delete xyMat;
	delete xyMat2;
	delete xyMatLine;
	delete xyMatLine2;

	pObjBolt->SetBoltHole(TRUE);
	pObjBolt->SetSolid(FALSE);

	if (bMakeBoltHole)
		pObj->AddChildObj(pObjBolt);
	else
		delete pObjBolt;
	if (bMakeBoltLine)
		pObj->AddChildObj(pObjLine);
	else
		delete pObjLine;
}

//<summary>
//가로보 블라켓 2D 복부판
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
//<param name="bShopApply"></param>
void CPlateCrossBeam::CreatePieceObjPlateGassetMain(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx   = GetBx();
	CPlateGirderApp  *pGir  = pBx->GetGirder();
	CPlateBasicIndex *pBxR  = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CPlateBridgeApp  *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen= pBx->GetGirder();
	CGlobarOption    *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	if(pGir->m_pRight==NULL)	return;
	
	CDPoint A[10],xy;

	GetCrossXyDom(A);
	double Zorib = 0;
	double GiulGasset = pGir->GetGiulPlateCrossGusset(pBx);	// 좌.우를 같이 처리(App 100%)
	double cH  = m_H_CR;
	double gHL = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double gHR = pGir->m_pRight->GetHeightGirderByStaAng(pBxR->GetStation());				
	double gH  = pObj->m_nCol ? gHR : gHL;
	double dWGirLo  = pGir->GetWidthFlangeLower(pBx);
	double dWGirLoN = pGir->m_pRight->GetWidthFlangeLower(pBxR);
	double dTWeb    = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double dTWebR   = pGir->m_pRight->GetThickFactByStation(pBxR->GetStation(), G_W);
	
	BOOL bCrossH = (cH == gH) ? TRUE : FALSE;
	// 확장스캘럽으로 처리할지를 판단
	double dLenFilletMin = pGlopt->GetDesignMinLenOfFilletWeld();
	double dLenFilletLen = 0;
	BOOL   bExtScallop	 = TRUE;
	long nScallopUpper   = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,G_F_U,pBx); // 스,채,모,없
	long nScallopLower   = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,CP_BR_F_L,pBx);
	if (bCrossH)
		nScallopLower = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,G_F_L,pBx);
	if (pObj->m_nCol == 1) {
		nScallopUpper = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,G_F_U,pBx); // 스,채,모,없
		nScallopLower = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,CP_BR_F_L,pBx);
		if (bCrossH)
			nScallopLower = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,G_F_L,pBx);
	}
	P_TYPE ScallopTypeUpper = pObj->GetPieceData()->GetPTypeByScallop(nScallopUpper);	//스캘럽 타입 
	P_TYPE ScallopTypeLower = pObj->GetPieceData()->GetPTypeByScallop(nScallopLower);	//스캘럽 타입 

	double sR = m_T3_CR <=16 ? pGirCen->m_pSangse->m_Scallop_R1 : pGirCen->m_pSangse->m_Scallop_R2;
	double cL = m_T3_CR <=16 ? pGirCen->m_pSangse->m_Chamfer_C1 : pGirCen->m_pSangse->m_Chamfer_C2;

	double  scallopRChampUpper = 0; ////
	double  scallopRChampLower = 0; ////
	
	if (ScallopTypeUpper == P_TYPE_SCALLOP)
		scallopRChampUpper = sR;
	else if (ScallopTypeUpper == P_TYPE_CHAMFER)
		scallopRChampUpper = cL;

	if (ScallopTypeLower == P_TYPE_SCALLOP)
		scallopRChampLower = sR;
	else if (ScallopTypeLower == P_TYPE_CHAMFER)
		scallopRChampLower = cL;

	// 제형단면관련 수정사항
	CDPoint xyTopLR(0,0); // 웨브와 접촉하는 상단점
	CDPoint xyBotLR(0,0); // 가로보이음면 하단점
	CDPoint vX(1,0), vY(0,1);
	bExtScallop = TRUE;
	if(pObj->m_nCol==0)
	{
		double webT		= pGir->GetThickFactByStation(pBx->GetStation(), 2);
		dLenFilletLen   = pGir->GetLengthSharpOnBx(pBx, TRUE) - (sR + scallopRChampUpper);
		bExtScallop	    = (dLenFilletLen<dLenFilletMin) ? TRUE : FALSE;
		CDPoint vDirBrk = -GetVectorGasset();

		xyTopLR = A[8]+vX*webT;
		xyBotLR = CDPoint(A[2].x-Zorib, A[2].y-Zorib*GiulGasset-cH);
		CDPoint xyCro;
		GetXyMatchSegAndSeg(xyTopLR,xyTopLR+1*vY,
				          xyBotLR,xyBotLR+1*vDirBrk,xyCro);	
		if(bExtScallop && m_dLH1<=0)
		{
			pObj->AddElement(xyTopLR,P_TYPE_DIMPOINT);
			xy = A[8] + vX*webT -vY*sR;
			pObj->AddElement(xy,P_TYPE_LINE,-1,pBx,sR);
			xy = A[0]-vY*sR;
			pObj->AddElement(xy);
			xy.x = A[0].x + sR;
			xy.y = A[0].y + sR*GiulGasset;
			pObj->AddElement(xy,-sR,-1);
		}
		else
		{
			pObj->AddElement(xyTopLR,P_TYPE_DIMPOINT|ScallopTypeUpper,-1,pBx,scallopRChampUpper);
			if(m_dLH1<=0)
				pObj->AddElement(A[0], /*P_TYPE_SCALLOP|*/P_TYPE_LINE,-1,pBx,sR);
		}

		xy.x = A[2].x - Zorib;
		xy.y = A[2].y - Zorib*GiulGasset;
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,cL);
		xy.y -= cH;
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,cL);
		xyBotLR = xy;
		//일단 무시함.
		if (bCrossH) 
		{
		/*	xy.x = A[0].x + sR;
			xy.y = A[0].y - cH+sR*GiulGasset;
			pObj->AddElement(xy);*/
			if (bExtScallop)
			{
				xy.x = A[8].x+webT;
				xy.y = A[8].y - cH + sR;
				pObj->AddElement(xy,-sR,-1);
				xy.x = A[8].x + webT + sR;
				xy.y = A[8].y - cH + sR;
				pObj->AddElement(xy);
			}
			else
			{
				xy.x = A[8].x+dTWeb/2+dWGirLo/2;
				xy.y = A[8].y-m_H_CR;
				pObj->AddElement(xy, P_TYPE_SCALLOP,-1, pBx,sR);
				pObj->AddElement(xyCro,ScallopTypeLower|P_TYPE_DIMPOINT,-1,pBx,scallopRChampLower);
			}
			pObj->SetFaceRange(0,4);
			pObj->SetFaceRange(4,5);
			pObj->SetFaceRange(5,6);
			pObj->SetFaceRange(6,7);
			pObj->SetFaceRange(7,8);
			pObj->SetFaceRange(8,0);

		}
		else 
		{
			pObj->AddElement(xyCro,ScallopTypeLower|P_TYPE_DIMPOINT,-1,pBx,scallopRChampLower);

			pObj->SetFaceRange(0,4);
			pObj->SetFaceRange(4,5);
			pObj->SetFaceRange(5,6);
			pObj->SetFaceRange(6,0);
		}
	}	
	if(pObj->m_nCol==1)
	{
		CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
		double webT		= pGir->m_pRight->GetThickFactByStation(pBxSec->GetStation(), 2);;
		dLenFilletLen   = pGir->GetLengthSharpOnBx(pBxR, TRUE) - (sR + scallopRChampUpper);
		bExtScallop	    = (dLenFilletLen<dLenFilletMin) ? TRUE : FALSE;

		xy.x = A[5].x + Zorib;
		xy.y = A[5].y + Zorib*GiulGasset;
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,cL);

		CDPoint vDirBrk = CDPoint(1,GiulGasset).Unit();
		xyTopLR = A[9]-vX*webT;
		xyBotLR = A[5] + vX*Zorib - vY*cH;
		CDPoint xyCro;
		GetXyMatchSegAndSeg(xyTopLR,xyTopLR+1*vY,
				          xyBotLR,xyBotLR+1*vDirBrk,xyCro);
		if(bExtScallop && m_dRH1<=0)
		{
			xy.x = A[7].x - sR;
			xy.y = A[7].y - sR*GiulGasset;
			pObj->AddElement(xy);
			xy.x = A[7].x;
			xy.y = A[7].y - sR;
			pObj->AddElement(xy,-sR,-1);
			xy.x = A[9].x - webT;
			xy.y = A[9].y - sR;
			pObj->AddElement(xy,P_TYPE_LINE,-1,pBx,sR);
			pObj->AddElement(xyTopLR,P_TYPE_DIMPOINT,-1,pBx,sR);
		}
		else
		{
			if(m_dRH1<=0)
				pObj->AddElement(A[7],/*P_TYPE_SCALLOP|*/P_TYPE_LINE,-1,pBx,sR);
			pObj->AddElement(xyTopLR, P_TYPE_DIMPOINT|ScallopTypeUpper|P_TYPE_LINE,-1,pBx,scallopRChampUpper);
		}
		//일단 무시
		if (bCrossH) 
		{
			if (bExtScallop)
			{
				xy.x = A[9].x - webT - sR;
				xy.y = A[9].y - cH + sR;
				pObj->AddElement(xy);
				xy.x = A[7].x;
				xy.y = A[7].y - cH + sR;
				pObj->AddElement(xy);
				xy.x = A[7].x;
				xy.y = A[7].y - cH;
				pObj->AddElement(xy,P_TYPE_SCALLOP,-1, pBx,sR);
			}
			else
			{
				pObj->AddElement(xyCro,ScallopTypeLower|P_TYPE_DIMPOINT,-1,pBx,scallopRChampLower);
				xy.x = A[9].x-dWGirLoN/2-dTWebR/2;
				xy.y = A[9].y - cH;
				pObj->AddElement(xy,P_TYPE_SCALLOP,-1, pBx,sR);

			}
			pObj->AddElement(xyBotLR,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,cL);

			pObj->SetFaceRange(0,4);
			pObj->SetFaceRange(4,5);
			pObj->SetFaceRange(5,6);
			pObj->SetFaceRange(6,7);
			pObj->SetFaceRange(7,8);
			pObj->SetFaceRange(8,0);
		}
		else 
		{
			pObj->AddElement(xyCro,ScallopTypeLower|P_TYPE_DIMPOINT,-1,pBx,scallopRChampLower);
			pObj->AddElement(xyBotLR,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,cL);

			pObj->SetFaceRange(0,4);
			pObj->SetFaceRange(4,5);
			pObj->SetFaceRange(5,6);
			pObj->SetFaceRange(6,0);
		}
	}
		
	pObj->RegenVertex();
}
//<summary>
//가로보 블라켓 복부판 2D 볼트홀 및 볼트선
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
void CPlateCrossBeam::CreatePieceObjPlateGassetMainBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{


}

//<summary>
//가로보 블라켓 2D 보강재
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
//<param name="bShopApply"></param>
void CPlateCrossBeam::CreatePieceObjPlateGassetPlate(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx	  = GetBx();
	CPlateGirderApp  *pGir    = pBx->GetGirder();
	CPlateGirderApp  *pGirN   = pGir->m_pRight;
	CPlateBridgeApp	 *pDB     = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen = pDB->GetGirder(-1);
	CPlateBasicIndex *pBxSec  = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CGlobarOption    *pGlopt  = pDB->m_pDataManage->GetGlobalOption();

	CDPoint A[10], B[10], xy, vAng;
	GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	long nCol = pObj->m_nCol;
	double GiulGasset = pGir->GetGiulPlateCrossGusset(pBx);// 좌.우를 같이 처리(App 100%)
	double cH = m_H_CR;
	double lT = m_T1_CR;
	int nGen = (int)pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_GEN);
	double dS1 = pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S1);
	double dS2 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S2) : 0;
	double dS3 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S3) : 0;
	double station = pBx->GetStation();
	double gW = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);

	long nScallopUpper = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,CP_BR_F_L,pBx); // 스,채,모,없
	long nScallopLower = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,G_F_L,pBx);
	if (nCol==1)
	{
		nScallopUpper = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,CP_BR_F_L,pBxSec); // 스,채,모,없
		nScallopLower = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,G_W,G_F_L,pBxSec);
	}
	
	P_TYPE ScallopTypeUpper = pObj->GetPieceData()->GetPTypeByScallop(nScallopUpper);	//스캘럽 타입 
	P_TYPE ScallopTypeLower = pObj->GetPieceData()->GetPTypeByScallop(nScallopLower);	//스캘럽 타입 

	double  scallopR = m_T3_CR <=16 ? pGirCen->m_pSangse->m_Scallop_R1 : pGirCen->m_pSangse->m_Scallop_R2;
	double  scallopC = m_T3_CR <=16 ? pGirCen->m_pSangse->m_Chamfer_C1 : pGirCen->m_pSangse->m_Chamfer_C2;
	double len = 0;
	if (ScallopTypeLower == P_TYPE_SCALLOP)
	{
		double MinWeldThick = pGlopt->GetDesignMinLenOfFilletWeld();	//용접 최소 길이
		if (nCol ==0)
			len = pGir->GetLengthSharpOnBx(pBx, TRUE) - scallopR - scallopC;
		else
			len = pGirN->GetLengthSharpOnBx(pBx, FALSE) - scallopR - scallopC;
		if (MinWeldThick > len)
			ScallopTypeLower = P_TYPE_CHAMFER;
	}
	
	double  scallopRChampUpper = 0; ////
	double  scallopRChampLower = 0; ////
	if (ScallopTypeUpper == P_TYPE_SCALLOP)
		scallopRChampUpper = scallopR;
	else if (ScallopTypeUpper == P_TYPE_CHAMFER)
		scallopRChampUpper = scallopC;

	if (ScallopTypeLower == P_TYPE_SCALLOP)
		scallopRChampLower = scallopR;
	else if (ScallopTypeLower == P_TYPE_CHAMFER)
		scallopRChampLower = scallopC;
	
	double  scallopRChampLower1 = scallopC; 
	double  scallopRChampLower2 = scallopC; 
/*	if (ScallopTypeLower == P_TYPE_SCALLOP)
		scallopRChampLower1 = scallopR;
	else if (ScallopTypeLower == P_TYPE_CHAMFER && (dS1>10 || dS1==0))
		scallopRChampLower1 = scallopC;
	if (ScallopTypeLower == P_TYPE_SCALLOP)
		scallopRChampLower2 = scallopR;
	else if (ScallopTypeLower == P_TYPE_CHAMFER && (dS2>10 || dS2==0))
		scallopRChampLower2 = scallopC;*/

	CDPoint xyTopLR(0,0); // 웨브와 접촉하는 상단점
	CDPoint xyBotLR(0,0); // 가로보이음면 하단점
	CDPoint vX(1,0), vY(0,1);
	if(nCol==0) 
	{
		vAng = CDPoint(0,1);
		double webT		= pGir->GetThickFactByStation(pBx->GetStation(), 2);
		double gHeight	= pGir->GetHeightGirderByStaAng(station);
		double gW = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
		//
		CDPoint vDirBrk = -GetVectorGasset();
		xyTopLR = A[8] + vX*webT;
		xyBotLR = A[2] - vY*(cH+lT);
		CDPoint xyCro;
		GetXyMatchSegAndSeg(xyTopLR,xyTopLR+1*vY,
				          xyBotLR,xyBotLR+1*vDirBrk,xyCro);
		pObj->AddElement(xyCro, ScallopTypeUpper|P_TYPE_DIMPOINT,-1,pBx,scallopRChampUpper);
		xy.x = A[2].x - dS2;
		xy.y = A[2].y - cH - lT - dS2*GiulGasset;
		pObj->AddElement(xy, P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,NULL,scallopRChampLower2);
		xy.y = A[2].y - cH - lT - dS2*GiulGasset - dS3;
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);
		xy = (B[1]+B[2])/2+vX*(gW/2-dS1)-vY*gHeight;
		pObj->AddElement(xy ,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,NULL,scallopRChampLower1);
		xy = (B[1]+B[2])/2+vX*(webT/2)-vY*gHeight;
		pObj->AddElement(xy ,ScallopTypeLower|P_TYPE_DIMPOINT,-1,NULL,scallopRChampLower);
		// Model의 기준점을 위한 FACE (4번 uXy를 기준으로 이동)
		pObj->SetFaceRange(3,4);
	}	
	if(nCol==1)
	{
		vAng = CDPoint(0,1);
		station = pBxSec->GetStation();
		pGir = pGir->m_pRight;
		gW = pGir->GetWidthOnStation(pBxSec->GetStation(), FALSE);
		double webT	    = pGir->GetThickFactByStation(pBxSec->GetStation(), 2);
		double gHeight	= pGir->GetHeightGirderByStaAng(station);
		double gWU       = pGir->GetWidthOnStation(pBxSec->GetStation(), TRUE);
		double gWL       = pGir->GetWidthOnStation(pBxSec->GetStation(), FALSE);
		//
		CDPoint vDirBrk = GetVectorGasset();
		xyTopLR = CDPoint(A[9].x-webT, A[9].y);
		xyBotLR = CDPoint(A[5].x,A[5].y-cH-lT);
		CDPoint xyCro;
		GetXyMatchSegAndSeg(xyTopLR,xyTopLR+1*vY,
				          xyBotLR,xyBotLR+1*vDirBrk,xyCro);
		xy.x = A[5].x + dS2;
		xy.y = A[5].y - cH - lT + dS2*GiulGasset;
		pObj->AddElement(xy, P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBxSec,scallopRChampLower2);
		pObj->AddElement(xyCro, ScallopTypeUpper|P_TYPE_DIMPOINT,-1,NULL,scallopRChampUpper);
		xy.x = A[9].x - webT;
		xy.y = B[7].y - gHeight;
		pObj->AddElement(xy, ScallopTypeLower|P_TYPE_DIMPOINT,-1,NULL,scallopRChampLower);
		CDPoint xy = B[7] + vX*gWU/2;
		xy.x = xy.x - gWL/2 + dS1; 
		xy.y = xy.y - gHeight;
		pObj->AddElement(xy, P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,NULL,scallopRChampLower1);
		xy.x = A[5].x + dS2;
		xy.y = A[5].y - cH - lT + dS2*GiulGasset - dS3;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);
		// Model의 기준점을 위한 FACE (2번 uXy를 기준으로 이동)
		pObj->SetFaceRange(2,3);
	}

	pObj->RegenVertex();
}
//<summary>
//가로보 블라켓 2D 보강재
//</summary>
//<param name="*pObj"></param>
//<param name="bPieceApply"></param>
//<param name="bShopApply"></param>
void CPlateCrossBeam::CreatePieceObjPlateGassetPlateByBolt(CPlatePieceObject *pObj, BOOL bMakeBoltHole, BOOL bMakeBoltLine)
{
	CPlateBasicIndex *pBx    = GetBx();
	CPlateGirderApp  *pGir   = pBx->GetGirder();
	CPlateGirderApp  *pGirN  = pGir->m_pRight;
	CPlateBridgeApp	 *pDB    = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirCen= pDB->GetGirder(-1);
	CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGirN);
	CPlateCrossBeam  *pCR    = pBxSec->GetSection()->GetCrossBeam();	

	double dWVStiffL = m_pVStiff->m_dWidth[1];
	double dWVStiffR = pBxSec->GetSection()->GetVStiff()->m_dWidth[0];

	CDPoint A[10], B[10], xy, vAng;
	GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	long nCol = pObj->m_nCol;
	double GiulGasset = pGir->GetGiulPlateCrossGusset(pBx);// 좌.우를 같이 처리(App 100%)
	double cH = m_H_CR;
	double lT = m_T1_CR;
	double station = pBx->GetStation();

	CDPoint xyTopLR(0,0); // 웨브와 접촉하는 상단점
	CDPoint xyBotLR(0,0); // 가로보이음면 하단점
	CDPoint vX(1,0), vY(0,1);
	BOOL	bHSteel	= GetType()==CR_TYPE_HSTEEL ? TRUE : FALSE;
	double	dB = bHSteel ? 0 : pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
	double	dH = bHSteel ? 0 : pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	double	dR = bHSteel ? 0 : pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_R);
	double	nGen = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_GEN);
	double	dT1 = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S1);
	double	dS2 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S2) : 0;
	double	dS3 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S3) : 0;

	if(nCol==0) 
	{
		double webT		= pGir->GetThickFactByStation(pBx->GetStation(), 2);
		double	dLenWebToCr	= GetLengthGirWebToCrWeb(TRUE);
		//
		CDPoint vDirBrk = GetVectorGasset();
		xyTopLR = (B[1]+B[2])/2 + vX*webT/2;
		xyBotLR = A[2] - vY*(cH+lT);
		CDPoint xyCro;
		GetXyMatchLineAndLine(xyTopLR, vY, xyBotLR, vDirBrk, xyCro);
		
		xy = xyCro + vDirBrk*((dWVStiffL+m_dbL3)/vDirBrk.x+dB);
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);//0
		
		xy.x = A[2].x - dS2;
		xy.y = A[2].y - cH - lT - dS2*GiulGasset;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);//1
		
		xy.y -= dS3;
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);//2
		
		xy = xyCro + vDirBrk*dLenWebToCr/vDirBrk.x - vY*(m_dltH+dH)
			       + vX*(dT1+dWVStiffL-dLenWebToCr);
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);//3
		
		xy = xyCro + vDirBrk*dLenWebToCr/vDirBrk.x - vY*(m_dltH+dH);
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);//4

		xy = xyCro - vY*dH + vX*dLenWebToCr;
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);//5

		xy = xyCro + vDirBrk*(dWVStiffL+m_dbL3+dB)/vDirBrk.x - vY*dH;
		if(bHSteel)	pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT,-1, pBx, dR);//6
		else		pObj->AddElement(xy, P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1, pBx, dR);//6
	}	
	else
	{
		station = pBxSec->GetStation();
		double webT	    = pGirN->GetThickFactByStation(pBxSec->GetStation(), 2);
		CDPoint vDirBrk = GetVectorGasset();
		xyTopLR = CDPoint(A[9].x-webT, A[9].y);
		xyBotLR = CDPoint(A[5].x,A[5].y-cH-lT);
		CDPoint xyCro;
		GetXyMatchLineAndLine(xyTopLR, vY, xyBotLR, vDirBrk, xyCro);
		//6
		xy = xyCro - vDirBrk*(dWVStiffR+m_dbR3+dB)/vDirBrk.x - vY*dH;
		pObj->AddElement(xy, P_TYPE_SCALLOP|P_TYPE_DIMPOINT,-1, pBx, dR);
		//5
		xy = xyCro - vY*dH - vX*GetLengthGirWebToCrWeb(FALSE);
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);
		//4
		xy = xyCro - vDirBrk*GetLengthGirWebToCrWeb(FALSE)/vDirBrk.x - vY*(m_drtH+dH);
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);
		//3
		xy = xyCro - vDirBrk*GetLengthGirWebToCrWeb(FALSE)/vDirBrk.x - vY*(m_drtH+dH)
			       - vX*(dT1+dWVStiffR-GetLengthGirWebToCrWeb(FALSE));
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);
		//2
		xy.x = A[5].x + dS2;
		xy.y = A[5].y - cH - lT - dS2*GiulGasset - dS3;
		pObj->AddElement(xy ,P_TYPE_LINE|P_TYPE_DIMPOINT);
		//1
		xy.x = A[5].x + dS2;
		xy.y = A[5].y - cH - lT - dS2*GiulGasset;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);
		//0
		xy = xyCro - vDirBrk*(dWVStiffR+m_dbR3+dB)/vDirBrk.x;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);
	}

	pObj->RegenVertex();

///////////////////////////////////////////////////////////////////////////////////
//볼트
///////////////////////////////////////////////////////////////////////////////////
	CPlatePieceObject *pBolt = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	CPlatePieceObject *pLine = new CPlatePieceObject(pGir,pObj->m_nMarkBuje);
	pBolt->SetLayer(HCAD_STLC);
	pLine->SetLayer(HCAD_STLC);
	double dBoltR = pDB->GetBoltRad(m_ZzsBoltSize);

	CDPoint xyStt, xyEnd, xyMid;
	long n = 0;
	if(!m_nLGirSpType && nCol==0)
	{
		double webT		= pGir->GetThickFactByStation(pBx->GetStation(), 2);
		CDPoint vDirBrk = GetVectorGasset();
		xyTopLR = (B[1]+B[2])/2 + vX*webT/2;
		xyBotLR = A[2] - vY*(cH+lT);
		CDPoint xyCro;
		double H = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
		GetXyMatchLineAndLine(xyTopLR, vY, xyBotLR, vDirBrk, xyCro);
		xyStt = xyCro + vDirBrk*GetLengthGirWebToCrWeb(TRUE)/vDirBrk.x - vY*dH;
		xyEnd = xyCro + vDirBrk*GetLengthGirWebToCrWeb(TRUE)/vDirBrk.x - vY*(m_dltH+H);
		xyMid = (xyStt+xyEnd)/2 + vX*m_dbL1;

		if(long(m_ltE)%2==0)
		{
			long nHalf = m_ltE/2;
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*m_ltF*n, dBoltR);
				pLine->AddElement(xyMid+vY*m_ltF*n);
			}
			pLine->SetEnd();
			pLine->AddElement(xyMid);
			for(n=1; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*m_ltF*n, dBoltR);
				pLine->AddElement(xyMid-vY*m_ltF*n);
				if(n==nHalf)
					xyBoltPos[1] = xyMid-vY*m_ltF*n;
			}
			pLine->SetEnd();
		}
		else
		{
			long nHalf = (m_ltE-1)/2;
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*(m_ltF/2+m_ltF*n), dBoltR);
				pLine->AddElement(xyMid+vY*(m_ltF/2+m_ltF*n));
			}
			pLine->SetEnd();
			
			pLine->AddElement(xyMid+vY*m_ltF/2);
			pLine->AddElement(xyMid-vY*m_ltF/2);
			pLine->SetEnd();
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*(m_ltF/2+m_ltF*n), dBoltR);
				pLine->AddElement(xyMid-vY*(m_ltF/2+m_ltF*n));
				if(n==nHalf)
					xyBoltPos[1] = xyMid-vY*(m_ltF/2+m_ltF*n);
			}
			pLine->SetEnd();

		}
	}
		
	if(!m_nRGirSpType && nCol==1)
	{
		station = pBxSec->GetStation();
		double webT	    = pGirN->GetThickFactByStation(pBxSec->GetStation(), 2);
		CDPoint vDirBrk = GetVectorGasset();
		xyTopLR = CDPoint(A[9].x-webT, A[9].y);
		xyBotLR = CDPoint(A[5].x,A[5].y-cH-lT);
		CDPoint xyCro;
		double H = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
		GetXyMatchLineAndLine(xyTopLR, vY, xyBotLR, vDirBrk, xyCro);
		xyStt = xyCro - vDirBrk*GetLengthGirWebToCrWeb(FALSE)/vDirBrk.x - vY*dH;
		xyEnd = xyCro - vDirBrk*GetLengthGirWebToCrWeb(FALSE)/vDirBrk.x - vY*(m_drtH+H);
		xyMid = (xyStt+xyEnd)/2 - vX*m_dbR1;

		if(long(m_rtE)%2==0)
		{
			long nHalf = m_rtE/2;
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*m_rtF*n, dBoltR);
				pLine->AddElement(xyMid+vY*m_rtF*n);
			}
			pLine->SetEnd();
			pLine->AddElement(xyMid);
			for(n=1; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*m_rtF*n, dBoltR);
				pLine->AddElement(xyMid-vY*m_rtF*n);
				if(n==nHalf)
					xyBoltPos[3] = xyMid-vY*m_rtF*n;
			}
			pLine->SetEnd();

		}
		else
		{
			long nHalf = (m_rtE-1)/2;
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid+vY*(m_rtF/2+m_rtF*n), dBoltR);
				pLine->AddElement(xyMid+vY*(m_rtF/2+m_rtF*n));
			}
			pLine->AddElement(xyMid+vY*m_rtF/2);
			pLine->AddElement(xyMid-vY*m_rtF/2);
			pLine->SetEnd();
			for(n=0; n<=nHalf; n++)
			{
				pBolt->AddCircle(xyMid-vY*(m_rtF/2+m_rtF*n), dBoltR);
				pLine->AddElement(xyMid-vY*(m_rtF/2+m_rtF*n));
				if(n==nHalf)
					xyBoltPos[3] = xyMid-vY*(m_rtF/2+m_rtF*n);
			}

			pLine->SetEnd();
		}
	}

	pBolt->SetBoltHole(TRUE);
	pBolt->SetSolid(FALSE);
	if(bMakeBoltHole)
		pObj->AddChildObj(pBolt);
	else
		delete pBolt;
	if(bMakeBoltLine)
		pObj->AddChildObj(pLine);
	else
		delete pLine;
}


CDPoint CPlateCrossBeam::GetPointGassetSupPlateEndBx(CPlateBasicIndex *pBx, CDPoint vAng, BOOL bUpper, BOOL bLeft)
{
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CPlateGirderApp *pGirN   = pGir->m_pRight ? pGir->m_pRight : pGir;
	CPlateBridgeApp	*pDB     = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp *pGirCen = pDB->GetGirder(-1);
	if(m_H_CR==pGir->GetHeightGirderByStaAng(pBx->GetStation())) return CDPoint(0,0);

	CDPoint A[20], B[20], vX(1,0), vY(0,1);
	GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	int nGen = (int)pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_GEN);
	double Tl    = m_T1_CR;
	double slop  = (A[3]-A[2]).Unit().y;
	double dS1 = pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S1);
	double dS2 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S2) : 0;
	double dS3 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 0, CPlateBridgeApp::GUSSET_S3) : 0;
	double BoxH  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double dWidthGirN = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);

	CDPoint Gp1 = CDPoint(A[2].x-dS2,
						  A[2].y-dS2*slop-m_H_CR-Tl-dS3);
	CDPoint xy = (B[1]+B[2])/2;
	CDPoint Gp2 = CDPoint(xy.x+dWidthGirN/2-dS1/vAng.y, xy.y-BoxH);
	if(!bLeft) 
	{
		CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir->m_pRight ? pGir->m_pRight : pGir);
		double dWidthGirNUp = pGir->GetWidthOnStation(pBxSec->GetStation(), TRUE);
		double dWidthGirNLo = pGir->GetWidthOnStation(pBxSec->GetStation(), FALSE);
		BoxH  = pGirN->GetHeightGirderByStaAng(pBxSec->GetStation());
		//
		Gp1 = CDPoint(A[5].x+dS2,
					  A[5].y+dS2*slop-m_H_CR-Tl-dS3);
		xy = B[7]+vX*dWidthGirNUp/2;
		Gp2 = CDPoint(xy.x-dWidthGirNLo/2+dS1/vAng.y, xy.y-BoxH);
	}
	//
	if(bUpper)	return Gp1;
	else		return Gp2;

}

CDPoint CPlateCrossBeam::GetPointGassetSupPlateByBoltEndBx(CPlateBasicIndex *pBx, CDPoint vAng, BOOL bUpper, BOOL bLeft)
{
	if(pBx==NULL) return CDPoint(0,0);
	CPlateGirderApp* pGir = pBx->GetGirder();
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC == NULL) return CDPoint(0,0);
	if(pC->m_H_CR >= pGir->GetHeightGirderByStaAng(pBx->GetStation())) return CDPoint(0,0);

	CPlatePieceObject pObj(pGir);
	pObj.m_nCol	= bLeft ? 0 : 1;
	pC->CreatePieceObjPlateGassetPlateByBolt(&pObj);

	CDPoint Gp1 = bLeft ? pObj.GetPoint(2) : pObj.GetPoint(6);
	CDPoint Gp2 = bLeft ? pObj.GetPoint(3) : pObj.GetPoint(5);

	return bUpper ? Gp1 : Gp2;

/*	원본
	CPlateGirderApp *pGir    = pBx->GetGirder();
	CPlateGirderApp *pGirN   = pGir->m_pRight;
	CPlateBridgeApp	*pDB     = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp *pGirCen = pDB->GetGirder(-1);

	if(m_H_CR==pGir->GetHeightGirderByStaAng(pBx->GetStation())) return CDPoint(0,0);
	
	CPlateBasicIndex* pBxR = pGir->GetBxMatchByCrossBeam(pBx, pGirN);
	if(!pBxR)	return CDPoint(0,0);
	CPlateCrossBeam* pCR = pBxR->GetSection()->GetCrossBeam();
	if(!pCR)	return CDPoint(0,0);
	CDPoint A[20], B[20], vX(1,0), vY(0,1);
	GetCrossXyDom(A);
	pGir->GetHunchXyDom(pBx, B);

	double Sharp = pGir->GetLengthSharpOnBx(pBx, TRUE)/vAng.y;
	if(m_dLH1<=0)
		Sharp = 0;
	double Tl    = m_T1_CR;
	double slop  = (A[3]-A[2]).Unit().y;

	double BoxH  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double wDiff   = 0;
	double dThickWeb = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double dWidthGirN = pGir->GetWidthOnStation(pBx->GetStation(), FALSE);
	double dWVStiffL = m_pVStiff->m_dWidth[1];
	double dWVStiffR = pBxR->GetSection()->GetVStiff()->m_dWidth[0];
	double dB = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_B);
	double dH = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H);
	double dR = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_R);
	int nGen = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_GEN);
	double dT1 = pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S1);
	double dS2 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S2) : 0;
	double dS3 = nGen==0 ? pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_S3) : 0;

	CDPoint Gp1 = CDPoint(A[2].x-dS2, A[2].y-dS2*slop-m_H_CR-Tl-dS3);
	CDPoint xy = (B[1]+B[2])/2;
	double GiulGasset = pGir->GetGiulPlateCrossGusset(pBx);// 좌.우를 같이 처리(App 100%)
	CDPoint vDirBrk = GetVectorGasset();
	CDPoint xyTopLR = (B[1]+B[2])/2 + vX*dThickWeb/2;
	CDPoint xyBotLR = A[2] - vY*(m_H_CR+m_T1_CR);
	CDPoint xyCro;
	GetXyMatchLineAndLine(xyTopLR, vY, xyBotLR, vDirBrk, xyCro);

	CDPoint Gp2 = xyCro + vDirBrk*GetLengthGirWebToCrWeb(TRUE)/vDirBrk.x - vY*(m_dltH+pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H))
	       + vX*(dT1+dWVStiffL-GetLengthGirWebToCrWeb(TRUE));

	if(!bLeft) 
	{
		CPlateBasicIndex *pBxSec = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
		double dWidthGirNUp = pGir->GetWidthOnStation(pBxSec->GetStation(), TRUE);
		double dWidthGirNLo = pGir->GetWidthOnStation(pBxSec->GetStation(), FALSE);
		Sharp = pGirN->GetLengthSharpOnBx(pBxSec, TRUE)/vAng.y;
		BoxH  = pGirN->GetHeightGirderByStaAng(pBxSec->GetStation());
		dThickWeb  = pGirN->GetThickFactByStation(pBxSec->GetStation(), 2);
		//
		Gp1 = CDPoint(A[5].x+dS2,
					  A[5].y+dS2*slop-m_H_CR-m_T1_CR-dS3);
		xyTopLR = CDPoint(A[9].x-dThickWeb, A[9].y);
		xyBotLR = CDPoint(A[5].x,A[5].y-m_H_CR-m_T1_CR);
		GetXyMatchLineAndLine(xyTopLR, vY, xyBotLR, vDirBrk, xyCro);
		Gp2 = xyCro - vDirBrk*GetLengthGirWebToCrWeb(FALSE)/vDirBrk.x - vY*(m_drtH+pDB->GetGussetPlateJewon(pBx, 1, CPlateBridgeApp::GUSSET_H))
			       - vX*(dT1+dWVStiffR-GetLengthGirWebToCrWeb(FALSE));
	}
	//
	if(bUpper)	return Gp1;
	else		return Gp2;
	*/

}



void CPlateCrossBeam::CreatePieceObjPlateSpliceFlange(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp	 *pGir = pBx->GetGirder();

	long nMarkBuje = pObj->m_nMarkBuje;
	double W = 0;
	double L = 0;
	if(m_bConnectionPlate)	L = m_uA*2 + m_uB*m_uC + m_uC1 + m_uB1*m_uC;
	else					L = (m_uA + m_uB * m_uC)*2 + m_uC1;
	if(nMarkBuje==CP_SP_L_L||nMarkBuje==CP_SP_L_U||nMarkBuje==CG_SP_L_L||nMarkBuje==CG_SP_L_U)
	{
		if(m_bConnectionPlate)	L = m_dA*2 + m_dB*m_dC + m_dC1 + m_dB1*m_dC;
		else					L = (m_dA + m_dB * m_dC)*2 + m_dC1;
	}

	double GapCutting = 0;

	CDPoint xy;
	BOOL bUpper = nMarkBuje==CP_SP_U_U || nMarkBuje==CG_SP_U_U || nMarkBuje==CP_SP_U_L || nMarkBuje==CG_SP_U_L ? TRUE : FALSE; 
	// 상부상면, 하부하면 (박스높이와 가로보높이가 같은 경우)
	long nMarkBujeCh1 = pBx->IsJijum() ? CP_SP_U_U : CG_SP_U_U;
	long nMarkBujeCh2 = pBx->IsJijum() ? CP_SP_L_L : CG_SP_L_L;
	long nMarkBujeCh3 = pBx->IsJijum() ? CP_SP_L_U : CG_SP_L_U;
	BOOL bRoundFillet1 = EQUAL(m_dLH1,0, 1) || EQUAL(m_dRH1,0, 1) ? FALSE : TRUE;
	BOOL bRoundFillet2 = m_vW_G > m_P_CR-(m_uA+m_uB1*m_uC+m_uC1/2) ? TRUE : FALSE;
	BOOL bRoundFillet = FALSE;
	if(bUpper)
		bRoundFillet = bRoundFillet1 && bRoundFillet2 ? TRUE : FALSE;  
	else
		bRoundFillet = bRoundFillet2 ? TRUE : FALSE;
	
//	double dA = (nMarkBuje==nMarkBujeCh1) ? m_uA : m_dA;
//	double dB = (nMarkBuje==nMarkBujeCh1) ? m_uB : m_dB;
//	double dB1 = (nMarkBuje==nMarkBujeCh1) ? m_uB1 : m_dB1;
//	double dC = (nMarkBuje==nMarkBujeCh1) ? m_uC : m_dC;
//	double dC1 = (nMarkBuje==nMarkBujeCh1) ? m_uC1 : m_dC1;
//	double dM1 = (nMarkBuje==nMarkBujeCh1) ? m_uM1 : m_dM1;
//	double dW = (nMarkBuje==nMarkBujeCh1) ? m_uW : m_dW;
//	double dM_CR = (nMarkBuje==nMarkBujeCh1) ? m_uM_CR : m_dM_CR;

	double dA		= bUpper ? m_uA : m_dA;
	double dB		= bUpper ? m_uB : m_dB;
	double dB1		= bUpper ? m_uB1 : m_dB1;
	double dC		= bUpper ? m_uC : m_dC;
	double dC1		= bUpper ? m_uC1 : m_dC1;
	double dM1		= bUpper ? m_uM1 : m_dM1;
	double dW		= bUpper ? m_uW : m_dW;
	double dM_CR	= bUpper ? m_uM_CR : m_dM_CR;

	if(m_bConnectionPlate)
	{
		if( nMarkBuje==nMarkBujeCh1 || (nMarkBuje==nMarkBujeCh2 && m_H_CR==pBx->GetHeightGirderByBx()) 
			|| (nMarkBuje==nMarkBujeCh2 && m_cGasset_CR==0))
		{
			if(nMarkBuje==nMarkBujeCh1)			W = m_uM1*2;
			else if(nMarkBuje==nMarkBujeCh2)	W = m_dM1*2;
			if(pObj->m_nCol==0)	xy.x = -(dA+dB1*dC+dC1/2);
			else				xy.x = dA+dB1*dC+dC1/2;
			xy.y = +W/2;
			pObj->AddElement(xy,P_TYPE_LINE,-1,pBx);

			xy.y = +W/2-dW;			
			pObj->AddElement(xy);

			if(pObj->m_nCol==0)	xy.x = -m_P_CR+m_vW_G;
			else				xy.x = m_P_CR-m_vW_G;
			if(bRoundFillet)	pObj->AddElement(xy);

			xy.y = -(dM1-dW);
			
			if(bRoundFillet)	pObj->AddElement(xy, pObj->m_nCol == 0 ? fabs(dM1-dW) : -fabs(dM1-dW));

			if(pObj->m_nCol==0)	xy.x = -(dA+dB1*dC+dC1/2);
			else				xy.x = dA+dB1*dC+dC1/2;
			if(bRoundFillet)	pObj->AddElement(xy);

			xy.y = -W/2;
			pObj->AddElement(xy);

			xy.x = 0;
			pObj->AddElement(xy);

			if(pObj->m_nCol==0)	xy.x = dB*dC+dA+dC1/2.0;
			else				xy.x = -(dB*dC+dA+dC1/2.0);
			xy.y = -dM_CR;
			
			pObj->AddElement(xy);

			if(pObj->m_nCol==0)	xy.x = dB*dC+dA+dC1/2.0;
			else				xy.x = -(dB*dC+dA+dC1/2.0);
			xy.y = dM_CR;
			
			pObj->AddElement(xy);

			xy.x = 0;
			xy.y = +W/2;
			pObj->AddElement(xy);
		}
		else 
		{
			if(nMarkBuje!=nMarkBujeCh2 && m_nRadioConnectionPlateInOut)	return;

			double sW = pObj->m_bFront ? -1 : 1;
			W = m_uM1;
			if(nMarkBuje==nMarkBujeCh2 || nMarkBuje==nMarkBujeCh3)
				W = m_dM1;

			if(pObj->m_nCol==0)	xy.x = -(dA+dB1*dC+dC1/2);
			else				xy.x = dA+dB1*dC+dC1/2;
			xy.y = W*sW;
			pObj->AddElement(xy,P_TYPE_LINE,-1,pBx);

			if(pObj->m_nCol==0)	xy.y = (W-dW)*sW;
			else				xy.y = (W-dW)*sW;
			pObj->AddElement(xy);

			if(pObj->m_nCol==0)	xy.x = dB*dC+dA+dC1/2.0;
			else				xy.x = -(dB*dC+dA+dC1/2.0);
			pObj->AddElement(xy);

			if(pObj->m_nCol==0)	xy.y = dM_CR*sW;
			else				xy.y = dM_CR*sW;
			pObj->AddElement(xy);

			xy.x = 0;
			xy.y = W*sW;
			pObj->AddElement(xy);
		}
	}
	else
	{
		if( nMarkBuje==nMarkBujeCh1 || (nMarkBuje==nMarkBujeCh2 && m_H_CR==pBx->GetHeightGirderByBx()) 
			|| (nMarkBuje==nMarkBujeCh2 && m_cGasset_CR==0))
		{
			if (nMarkBuje == nMarkBujeCh1)
				W = m_uM_CR*2;
			else if (nMarkBuje == nMarkBujeCh2)
				W = m_dM_CR*2;
			xy.x = -L/2;
			xy.y = +W/2;
			if (pObj->m_nCol != 0)
				xy.x = -L/2 - GapCutting;
			pObj->AddElement(xy,P_TYPE_LINE,-1,pBx);
			xy.x =  L/2;
			if (pObj->m_nCol == 0)
				xy.x =  L/2 + GapCutting;
			pObj->AddElement(xy);
			xy.x =  L/2;
			xy.y = -W/2;
			if (pObj->m_nCol == 0)
				xy.x =  L/2 + GapCutting;
			pObj->AddElement(xy);
			xy.x = -L/2;
			if (pObj->m_nCol != 0)
				xy.x = -L/2 - GapCutting;
			pObj->AddElement(xy);
		}
		else 
		{
			double sW = pObj->m_bFront ? -1 : 1;
			double cW = m_uM_CR;
			W = m_uW;
			if(nMarkBuje==nMarkBujeCh2 || nMarkBuje==nMarkBujeCh3)
			{
				cW = m_dM_CR;
				W  = m_dW;
			}
			xy.x = -L/2;
			xy.y = (-cW + W)*sW;
			if (pObj->m_nCol != 0)
				xy.x = -L/2 - GapCutting;
			pObj->AddElement(xy,P_TYPE_LINE,-1,pBx);
			xy.x =  L/2;
			if (pObj->m_nCol == 0)
				xy.x =  L/2 + GapCutting;
			pObj->AddElement(xy);
			xy.x =  L/2;
			xy.y = -cW*sW;
			if (pObj->m_nCol == 0)
				xy.x =  L/2 + GapCutting;
			pObj->AddElement(xy);
			xy.x = -L/2;
			if (pObj->m_nCol != 0)
				xy.x = -L/2 - GapCutting;
			pObj->AddElement(xy);
		}
	}
}


void CPlateCrossBeam::CreatePieceObjPlateVStiffner(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx    = GetBx();
	CPlateGirderApp  *pGir   = pBx->GetGirder();
	CPlateBridgeApp  *pDB    = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption    *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	double dWebT = pGir->GetThickFactByStation(pBx->GetStation(), 2);
	double Dist = pGir->GetWidthOnStation(pBx->GetStation(), TRUE)/2 + m_P_CR - dWebT;
	for(long i=0; i<pObj->m_nCol; i++)
		Dist += m_VStiffDis_CR[i];
	double GiulUpper  = pGir->GetGiulFlangeUpperLowerCamber(pBx->GetStation(),Dist,TRUE);
	double sR = m_vT_CR <=16 ? pGir->m_pSangse->m_Scallop_R1 : pGir->m_pSangse->m_Scallop_R2;
	double cL = m_vT_CR <=16 ? pGir->m_pSangse->m_Chamfer_C1 : pGir->m_pSangse->m_Chamfer_C2;
	double D  = 10; // 가로보복부와의 LEVEL 거리(??)
	double T  = m_T3_CR;
	double L  = m_vL_CR;
	double W  = m_vW_CR;
	BOOL bFront = (pObj->m_nRow==0) ? TRUE : FALSE;

	long nMarkBuje1 = pDB->GetMarkBujeByMarkBujeAndBx(CP_W, pBx);
	long nMarkBuje2 = pDB->GetMarkBujeByMarkBujeAndBx(CP_F_U, pBx);
	long nScallopUpper = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,nMarkBuje1,nMarkBuje2,pBx); // 스,채,모,없
	nMarkBuje2 = pDB->GetMarkBujeByMarkBujeAndBx(CP_F_L, pBx);
	long nScallopLower = pDB->GetIndexBaseScallop(pObj->m_nMarkBuje,nMarkBuje1,nMarkBuje2,pBx); // 스,채,모,없
 
	P_TYPE ScallopTypeUpper = pObj->GetPieceData()->GetPTypeByScallop(nScallopUpper);	//스캘럽 타입 
	P_TYPE ScallopTypeLower = pObj->GetPieceData()->GetPTypeByScallop(nScallopLower);	//스캘럽 타입 

	if (ScallopTypeUpper == P_TYPE_SCALLOP || ScallopTypeLower == P_TYPE_SCALLOP)
	{
		double MinWeldThick = pGlopt->GetDesignMinLenOfFilletWeld();	//용접 최소 길이
		double len1 = (m_vW_CR - sR -cL)/cos(atan(GiulUpper));
		double len2 = (m_vW_CR - sR -cL);

		if (MinWeldThick > len1)
			ScallopTypeUpper = P_TYPE_CHAMFER;
		if (MinWeldThick > len2)
			ScallopTypeLower = P_TYPE_CHAMFER;
	}

	double  scallopRChampUpper = 0; ////
	double  scallopRChampLower = 0; ////
	
	if (ScallopTypeUpper == P_TYPE_SCALLOP)
		scallopRChampUpper = sR;
	else
		scallopRChampUpper = cL;

	if (ScallopTypeLower == P_TYPE_SCALLOP)
		scallopRChampLower = sR;
	else
		scallopRChampLower = cL;

	CDPoint A[10];
	GetCrossXyDom(A);

	double dCutEdgeUp = cL;
	double dCutEdgeDn = cL;
	if(m_uM_CR-15 < W)	dCutEdgeUp = W-(m_uM_CR-15);
	if(m_dM_CR-15 < W)	dCutEdgeDn = W-(m_dM_CR-15);
	
	// 현재 수직보강재가 세로보와 만나는 지를 판단
/*	long nMeet = -1;
	double xDist = A[2].x;
	for(i = 0 ; i < m_nVStiffsu_CR ; i++)
	{
		xDist += m_VStiffDis_CR[i]/vAng.y;
		if (i == pObj->m_nCol)
		{
			if (!bFront)
				nMeet  = pGir->CheckMeetCrossBeamVStiffStringer(pBx,xDist,bShopApply);
			else
				nMeet  = pGir->CheckMeetCrossBeamVStiffStringerPrv(pBx,xDist,bShopApply);

			if(nMeet != -1) break;
		}
	}	

	double Giul = GiulUpper;
	CBaseDesignCtl *pBet = &pGir->GetBridgeApp()->m_pBaseChief->m_BaseEtcCtl;		
	BOOL bDanBu = pBet->GetDesignStringerJijumApply();

	CStringer *pStr = NULL;
	if(nMeet != -1)
	{
		long nCross = bFront ? pBx->GetNumberCrossBeam()-1 : pBx->GetNumberCrossBeam();
		if(pBx->IsAbutStt() && bFront)
			nCross = 0;
		if (pBx->IsAbutEnd() && !bFront && bDanBu)
			nCross = pBx->GetNumberCrossBeam()-1;
		pStr = pGir->GetStringerByCrossNo(nCross, nMeet);
		if(pStr==NULL) return;
		pStr->SetStringerPointAll();
		Giul = pStr->GetGiulSeroboBody();
	}
*/
	double Giul = GiulUpper;
	CDPoint xy;
	double sY = bFront ? T/2 * GiulUpper : -T/2 * GiulUpper;
//	if(nMeet != -1 && i == pObj->m_nCol)
//		sY = m_vL_CR - L;

	if(bFront)
	{
		xy.x = -(T/2+W);
		xy.y = -sY - W*Giul;
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dCutEdgeUp);
		xy.x = -T/2;
		xy.y = -sY;
		pObj->AddElement(xy,ScallopTypeUpper|P_TYPE_DIMPOINT,-1,pBx,scallopRChampUpper);

		if (m_vL_CR < m_H_CR)
		{
			xy.y = -(L+sY);
			pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
			xy.x = -(T/2+D);
			pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
			xy.x  = -(T/2+W);
			xy.y += (W-D);
			pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		}
		else
		{
			xy.y = -(L+sY);
			pObj->AddElement(xy,ScallopTypeLower|P_TYPE_DIMPOINT, -1, pBx, scallopRChampLower);
			xy.x = -(T/2+W);
			pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dCutEdgeDn);
		}
		// FACE SETTING
		pObj->SetFaceRange(0,1);
		pObj->SetFaceRange(1,2);
		pObj->SetFaceRange(2,3);
		pObj->SetFaceRange(3,0);
	}
	if(!bFront)
	{
		xy.x = T/2;
		xy.y = -sY;
		pObj->AddElement(xy,ScallopTypeUpper|P_TYPE_DIMPOINT,-1,pBx,scallopRChampUpper);
		xy.x = T/2+W;
		xy.y = (xy.x-T/2)*Giul - sY;
		pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dCutEdgeUp);

		if (m_vL_CR < m_H_CR)
		{
			xy.x = T/2+W;
			xy.y = -L + (W-D);
			pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
			xy.x = T/2+D;
			xy.y = -L;
			pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
			xy.x = T/2;
			pObj->AddElement(xy,P_TYPE_LINE|P_TYPE_DIMPOINT);
		}
		else
		{
			xy.x = T/2+W;
			xy.y = -(L+sY);
			pObj->AddElement(xy,P_TYPE_CHAMFER|P_TYPE_DIMPOINT,-1,pBx,dCutEdgeDn);
			xy.x = T/2;
			pObj->AddElement(xy,ScallopTypeLower|P_TYPE_DIMPOINT,-1,pBx,scallopRChampLower);
		}

		// FACE SETTING
		pObj->SetFaceRange(0,1);
		pObj->SetFaceRange(1,3);
		pObj->SetFaceRange(3,4);
		pObj->SetFaceRange(4,0);
	}
	//
	pObj->RegenVertex();
}
void CPlateCrossBeam::CreatePieceObjPlateGassetBoard(CPlatePieceObject *pObj)
{
	CPlateBasicIndex *pBx	= GetBx();
	CPlateGirderApp  *pGir  = pBx->GetGirder();
	CPlateBridgeApp	 *pDB   = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp  *pGirC = pDB->GetGirder(-1);
	long nCol = pObj->m_nCol;
	CPlateCrossBeam* pC = pBx->GetSection()->GetCrossBeam();
	long nID=0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//용접이음
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//볼트이음
	
	int nGen = (int)pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_GEN);
	double W = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_W);		//브라켓 보강판 폭
	double dA = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DA);	//브라켓 보강판 빗면높이(하)
	double dB = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DB);
	double dC = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_DC);	//브라켓 보강판 중심부터 빗면폭(하)
	double uA = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UA);	//브라켓 보강판 빗면높이(상)
	double uB = nGen==0 ? pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UB) : 0;
	double uC = pDB->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_UC);	//브라켓 보강판 중심부터 빗면폭(상)
	double dFW = pC->m_dM_CR;

	DIMTYPE nDimType = DIM_BASIC;
	double Len = pGir->GetLengthJewon(pObj->m_nMarkBuje,pBx,pObj->m_nRow,pObj->m_nCol,nDimType)-dB-uB;

	CDPoint xy;
	// (0,0)은 상단의 중앙
	if(nGen==0)
	{
		xy.x = -W/2;	
		xy.y = -uA;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);	//(0)
		xy.x = -uC;
		xy.y =   0;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(1)
		xy.x =  uC;
		xy.y =   0;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(2)
		xy.x =  W/2;	
		xy.y = -uA;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(3)
		xy.x =  W/2;	
		xy.y = -Len + dA;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(4)
		xy.x =  dC;	
		xy.y = -Len;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(5)
		xy.x = -dC;	
		xy.y = -Len;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(6)
		xy.x = -W/2;	
		xy.y = -Len + dA;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(7)

		// FACE SETTING
		pObj->SetFaceRange(0,3);
		pObj->SetFaceRange(2,5);
		pObj->SetFaceRange(4,7);
	}
	else
	{
		xy.x = -dFW;
		xy.y = 0;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT,-1,pBx);	//(0)
		xy.x = dFW;
		xy.y = 0;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(1)
		xy.x = dC;
		xy.y = -Len;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(2)
		xy.x = -dC;
		xy.y = -Len;
		pObj->AddElement(xy, P_TYPE_LINE|P_TYPE_DIMPOINT);	//(3)
		
		pObj->SetFaceRange(1,0);
	}

	pObj->RegenVertex();

}

//<summary>
//가로보 블라켓 플랜지의 좌표를 가져옴.
//</summary>
//<param name="nPoint"></param>
//<param name="BX"></param>
//<param name="BY"></param>
//<param name="W1"></param>
//<param name="W2"></param>
//<param name="dis1"></param>
//<param name="dis2"></param>
//<param name="R"></param>
//<param name="C"></param>
//<param name="bLeft"></param>
//<param name="bFront"></param>
//<param name="vAng"></param>
//<param name="vRotate"></param>
//<return></return>
CDPoint CPlateCrossBeam::GetGassetPlateVertex(long nPoint, double BX, double BY, double dis1, double dis2, double W1, double W2, 
					double R, double C, BOOL bLeft, BOOL bFront, CDPoint vAng, CDPoint vRotate)
{
	CDPoint sp[8];
	CDPoint vX (1, 0), vY (0, 1), vD, vN;

	CDPoint vGasset = vAng;
	CDPoint vWidth  = vGasset.RotateInv90();

	double factor = 1 / vGasset.y;	

	if (!bFront)
	{
		vX = -vX;
		vWidth = -vWidth;
	}
	if (!bLeft)
	{
		vGasset = -vGasset;
		vY = -vY;
	}

	CDPoint xyCent	= vGasset * dis1*factor;

	sp[0] = xyCent + vGasset * (dis2-dis1*factor);
	sp[1] = sp[0] + vWidth * W2;
	sp[2] = xyCent + vX * W1;
	sp[4] = sp[2] + vY * C;
	if (R > 0)
	{
		sp[3] = sp[2] - vX * (R / 2.5);
		vD  = (sp[1] - sp[3]).Unit();
		vN  = vD.RotateInv90() * R;
		if (bFront ^ bLeft)  vN = -vN;
		GetXyMatchSegAndCircle (sp[3] + vN, sp[1] + vN, sp[4], R, sp[6], sp[7]);
		sp[5] = sp[3] + vD * ((sp[6] - sp[3]) ^ vD);
	}
	return GetXyRotateVector(CDPoint(0, 0), sp[nPoint], vRotate) + CDPoint(BX, BY);

/*	원본
	sp[0] = vGasset * (W2 * factor);
	sp[1] = sp[0] + vWidth * dis2;
	sp[2] = vGasset * (W1 * factor) + vX * (dis1 * factor);
	sp[4] = sp[2] + vY * C;
	if (R > 0)
	{
		sp[3] = sp[2] - vX * (R / 2.5);
		vD  = (sp[1] - sp[3]).Unit();
		vN  = vD.RotateInv90() * R;
		if (bFront ^ bLeft)  vN = -vN;
		GetXyMatchSegAndCircle (sp[3] + vN, sp[1] + vN, sp[4], R, sp[6], sp[7]);
		sp[5] = sp[3] + vD * ((sp[6] - sp[3]) ^ vD);
	}
	return GetXyRotateVector(CDPoint(0, 0), sp[nPoint], vRotate) + CDPoint(BX, BY);
*/
}


long CPlateCrossBeam::GetsuStringer()
{
	return 0;

}

void CPlateCrossBeam::GetXyMatrixWebMO(CMatrixPoint *xyMat, long nRow,long nCol)
{
	CPlateBasicIndex *pBx = GetBx();
	CPlateGirderApp  *pGir= pBx->GetGirder();
	CPlateBridgeApp  *pDB = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption *pGlopt = pDB->m_pDataManage->GetGlobalOption();

	double GapCutting = 0;//pGlopt->GetBracketGapOfCutting();

	CDPoint A[10];
	GetCrossXyDom(A);

	CDPoint xy1 = nCol ==0 ? A[2] : A[5];
	double sGiul = nCol ==0 ? (A[3].y - A[2].y)/(A[3].x - A[2].x)
							: (A[5].y - A[4].y)/(A[5].x - A[4].x);
	// 0-균등배치,1-일반배치,2-수평배치
	long nBoltArray = pGlopt->GetDesignBoltHoleArrangeCrossBeam();
	if(nBoltArray==2) sGiul = 0.0;

	double dX = xy1.x;
	double dY = nRow ==0 ? xy1.y-m_mD
						 : xy1.y-m_H_CR+m_mD+m_mE*m_mF;
	CDPoint xyL(dX,dY);
	CDPoint xyR(dX,dY);
	long nRowSu = m_mE+1;
	long nColSu = m_mB+1;
	xyMat->SetSize(nRowSu, nColSu*2);
	for(long r=0; r<nRowSu; r++)
	{
		for(long c=0; c<nColSu; c++)
		{
			xyL.x = -m_mC1/2 - m_mC*c; // 좌측
			if (nCol==1)
				xyL.x -=GapCutting;
			xyL.y = dY - m_mF*r + xyL.x*sGiul;
			xyR.x = m_mC1/2 + m_mC*c; // 우측
			if (nCol==0)
				xyR.x +=GapCutting;
			xyR.y = dY - m_mF*r + xyR.x*sGiul;
			xyL.x += dX;
			xyR.x += dX;
			xyMat->SetMatrixPoint(r,nColSu-1-c,xyL);
			xyMat->SetMatrixPoint(r,nColSu+c  ,xyR);
		}
	}
}

long CPlateCrossBeam::GetHStiffUpperDistRate(double Rate[]) const 
{
	switch( m_nHStiffUppersu_CR )
	{
		case 1:	Rate[0] = 0.20 * m_H_CR; break;
		case 2: Rate[0] = 0.14 * m_H_CR;
				Rate[1] = 0.36 * m_H_CR; break;
		default:Rate[0] = 0.0; 			 break;
	}
	return m_nHStiffUppersu_CR;
}

long CPlateCrossBeam::GetHStiffLowerDistRate(double Rate[]) const 
{
	switch( m_nHStiffLowersu_CR )
	{
		case 1:	Rate[0] = (1.0 - 0.20) * m_H_CR; break;
		case 2: Rate[0] = (1.0 - 0.36) * m_H_CR;
				Rate[1] = (1.0 - 0.14) * m_H_CR; break;
		default:Rate[0] = 0.0;					 break;
	}
	return m_nHStiffLowersu_CR;
}

double CPlateCrossBeam::GetLengthGirWebToCrWeb(BOOL bLeft)
{
	CPlateBasicIndex *pBx  = GetBx();
	CPlateGirderApp  *pGir = pBx->GetGirder();
	
	if(!pGir->m_pRight) return 0;

	CPlateCrossBeam  *pCR  = pBx->GetBxMatchByCrossBeam(pGir->m_pRight)->GetCrossBeam();
	if(bLeft)
		return m_pVStiff->m_dWidth[1]-m_dbL1-m_dbL2;
	else
	{
		if(pCR)
			return pCR->m_pVStiff->m_dWidth[0]-m_dbR1-m_dbR2;
		else
			return 0;
	}
}

double CPlateCrossBeam::GetFillerThick()
{
	double dThickFiller = 0;

	if(m_nFillerUseType==1)			//한쪽	
		dThickFiller = m_vT_G - m_T3_CR;
	else if(m_nFillerUseType==2)	//양쪽
		dThickFiller = (m_vT_G - m_T3_CR)/2;

	return dThickFiller;
}

double CPlateCrossBeam::GetFillerBoltLength(CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(!pBx->GetSection())	return 0;
	if(!bLeft && !pBx->GetGirder()->m_pRight)	return 0;
	
	if(!bLeft)	pBx = pBx->GetBxMatchByCrossBeam(pBx->GetGirder()->m_pRight);
	if(!pBx)	return 0;

	CVStiff	*pVStiff = pBx->GetSection()->GetVStiff();

	double dThickFiller = 0, Wid = 0;
	double dBoltLength = 0;

	if(m_nFillerUseType==1)//한쪽
	{
		Wid   = m_T3_CR; // 가로보 주플레이트 T		
		dThickFiller = pVStiff->m_dThick[bLeft?1:0] - Wid;

		if (dThickFiller >= 9)
			dBoltLength = m_T3_CR + Round(dThickFiller,0);
	}
	else if(m_nFillerUseType==2)//양쪽
	{
		Wid   = m_T3_CR/2; // 가로보 주플레이트 T		
		dThickFiller = pVStiff->m_dThick[bLeft?1:0]/2 - Wid;
		
		if (dThickFiller >= 9)
			dBoltLength = m_T3_CR + (Round(dThickFiller,0) *2);
	}

	return dBoltLength;
}

void CPlateCrossBeam::SetHSteelData()
{
	if(GetType()!=CR_TYPE_HSTEEL)	return;

	CSteelMaterialData	*pSteel	= GetSteelMaterialData();

	m_H_CR		= pSteel->m_dH - pSteel->m_dT2*2;
	m_T1_CR		= pSteel->m_dT2;	// 하부플랜지 두께
	m_T2_CR		= pSteel->m_dT2;	// 상부플랜지 두께
	m_T3_CR		= pSteel->m_dT1;	// 복부 두께
	m_uM_CR		= pSteel->m_dB/2;	// 상부플랜지 폭
	m_dM_CR		= pSteel->m_dB/2;	// 하부플랜지 폭
}

BOOL CPlateCrossBeam::IsLeftHBGu()
{
	CPlateBasicIndex *pBx	= GetBx();
	CPlateGirderApp  *pGir  = pBx->GetGirder();
	CHBracingGusset	 *pBrGuL  = pBx->GetHBracingGusset();

	double dHL	  = pBrGuL ? (pBrGuL->m_dH[1]) : 0;
	return dHL>0 ? TRUE : FALSE;
}

BOOL CPlateCrossBeam::IsRightHBGu()
{
	CPlateBasicIndex *pBx	= GetBx();
	CPlateGirderApp  *pGir  = pBx->GetGirder();
	CPlateBasicIndex *pBxR	= pBx->GetBxMatchByCrossBeam(pGir->m_pRight ? pGir->m_pRight : pGir);
	CHBracingGusset	 *pBrGuR  = pBxR->GetHBracingGusset();

	double dHR	  = pBrGuR ? (pBrGuR->m_dH[0]) : 0;
	return dHR>0 ? TRUE : FALSE;
}
