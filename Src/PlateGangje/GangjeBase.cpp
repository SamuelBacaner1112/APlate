// GangjeBase.cpp: implementation of the CGangjeBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "GangjeBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CString CGangjeBase::m_sEleNameMain[MAX_ELEMAIN] = 
	{			
		"<->",
		"�Ϲ�",
		"��� ��������",
		"�Ϻ� ��������",
		"������ ��������",				
		"������ ���κ�",
		"�й� ���κ�",
		"���� �극�̽�",
		"���� �극�̽�",
		"��Ÿ����",
		""	// ������ �ĺ���
//		"���κ�",
	};
CString CGangjeBase::m_sEleNameSub[MAX_ELESUB] =
	{
		"<->",
		"����",
		"����",
		"������",
		"���򺸰���",
		"���ܿ�����",
		"���ܿ����� ����",
		"���",
		"��� ������",
		"�ϸ� ������",
		"�극�̽� ������",
		"���Ʈ ������",
		"���� ������",		
		"���򺸰���",
		"����� ������",
		"����� ����",
		"����� ����",
		"����� ������",
		"����� ������",
		"���ٸ��� ��",
		"��Ÿ �÷���Ʈ",
		"����������",
		"�Ŵ�����������",
		"������ ��Ŀ",
		"��Ʈ-��Ʈ",
		"EXPANDED METAL",
		"STUD BOLT",
		"NET WASHER",
		"���÷���Ʈ",
		"��� ���÷���Ʈ",
		"���������",
		"�����κ�����",
		"REBAR",
		"FACE PLATE",
		"� �� ����",
		"�ε �� ����",
		"�� ����",
		"CT ����",
		"I ����",
		"H ����",
		"ä����",
		"�ż�Ʈ �÷���Ʈ",
		"������",
		"������",
		"����",
		""	// ������ �ĺ���
//		"������ ������",
//		"�� �÷���Ʈ",
//		"�� �÷���Ʈ",
//		"�� �÷���Ʈ",
//		"���� �÷���Ʈ",
//		"�÷���Ʈ",
//		"���� ��Ʈ-��Ʈ",
//		"WELD STUD",
//		"���� �÷���Ʈ",
//		"�ڵ�Ȧ ��Ʈ",
//		"�ڵ�",
//		"������",
//		"ROUND BAR",
//		"STEEL PIPE",
//		"L ����",
//		"��� ������",
//		"������",
//		"�˹̴��÷���Ʈ",
//		"SPACER",
//		"WASHER",
//		"�����η�����Ʈ",
//		"SLAB ANCHOR",
//		"ANGLE"
	};

CString CGangjeBase::m_sKindName[MAX_KINDNAME] =
	{
		"<->",
		"GIRDER",
		"SEG",
		"����",
		"����",
		"������",				
		"����������",		
		"��������å",
		"��������",
		"���κ�",
		"������ ��Ŀ",
		"���ܿ�����",
		"���ܿ����� ����",
		"���÷���Ʈ",
		"��Ÿ����",
		""	// ������ �ĺ���
//		"���κ�",
//		"�������κ�",		
//		"WELD STUD",
	};
 
//�߰����..
CString CGangjeBase::m_sProcessBig[MAX_PROCESS_BIG] =
	{
		"<->",			// 0
		"MAIN P/N",
		"GIRDER",
		"SPLICE",
		"CROSS BEAM",
		"C/B BRACKET",
		"BOLT",
		"REBAR",		// 15
		"����",
		"������",
		""
//		"DIAPHRAGM",
//		"SUB DIAPH",
//		"STRINGER",
//		"S/T BRACKET",
//		"WING BEAM",
//		"W/B BRACKET",
//		"END BEAM",
//		"E/B BRACKET",
	};

CString CGangjeBase::m_sProcessMid[MAX_PROCESS_MID] =
	{
		"<->",			// 0
		"������",
		"�߰���",
		"�񿬰���",
		"H-STIFF",
		"JACK-UP����",
		"LUG",
		"MAIN WEB RING",
		"���ٸ�",
		"��������",
		"SOLE",
		"��Ÿ���� ��ü",
		"V-STIFF",
		"V-STIFF FLG",		// 20
		"WEB",
		"FLG",
		"GIRDER",
		"CROSS BEAM",
		"FLG",
		"T-FLG",
		"B-FLG",
		"V-����",
		"V-����FLG",
		"FLG",
		"H.T.BOLT",
		"STUD BOLT",
		"SLAB ANCHOR",
		"T.S.BOLT",
		"C. BOLT",
		"ö��",		// 40
		"RUBBER",
		"ANGLE",
		"PIPE",
		"ROD BAR",
		"WIRE MESH",		
		"VBRACING",
		"HBRACING",
""
		//		"������",
//		"DRAIN",
//		"MANHOLE����",
//		"ȯǳ��",
//		"DOOR",
//		"AIR VENT",
//		"DIAPHRAGM",
//		"DIA OPN'G ����",
//		"STRINGER",
//		"WING BEAM",
//		"END BEAM",
	};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGangjeBase::CGangjeBase(CPlateBridgeApp* pDB)
{
	m_pDB = pDB;
}

CGangjeBase::CGangjeBase()
{
}

CGangjeBase::~CGangjeBase()
{
}

//�÷��� �κ������� ���� ���ϱ�
double CGangjeBase::GetAreaFlangePart(long nMarkBuje, CPlateBasicIndex *pBx, BOOL bUpper)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	if(bUpper)
	{
		double WS  = pGir->GetWidthOnStation(pBx->GetStation(),bUpper) / 2;
		double LA = pGir->GetLengthJewon(nMarkBuje,pBx);
		return (WS * 2 * LA);
	}
	else
	{
		double WA  = pGir->GetWidthOnStation(pBx->GetStation(),bUpper);
		double WS  = pGir->GetWidthOnStation(pBx->GetStation(),bUpper) / 2;
		double TL = pBx->GetFactChain(nMarkBuje)->m_dFactChainThick;
		double LA = pGir->GetLengthJewon(nMarkBuje,pBx);
		return (WA+WS*2+TL*2)*LA;
	}

	return 0;
}

double CGangjeBase::GetThickFilletWeld(double T1, double T2)
{
	double MaxT  = max(T1,T2);
	double WeldT = RoundUp(sqrt(2*MaxT),0);
	if(WeldT<6)	WeldT = 6;

	return WeldT;
}

double CGangjeBase::GetPerimeterBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam)
{
	if(pBx==NULL)	return -1;
	CVector vJewon = pBx->GetGirder()->GetJewonTotal(nMarkBuje,pBx,nRow,nCol,nDimType,bFront,bLeftBeam);

	switch(nMarkBuje)
	{
		// �Ϲݺ���
		case G_F_U :		break;
		case G_F_L :		break;
		case G_W   :		break;
		case G_H_L :		return GetPerimeterHStiff(pBx,nRow, TRUE,nDimType);
		case G_H_R :		return GetPerimeterHStiff(pBx,nRow,FALSE,nDimType);
		
		// ��������
		case SP_L_L:		break;
		case SP_U_U:		break;
		case SP_U_L:		break;
		case SP_L_U:		break;
		case SP_W_MO:		break;
		case SP_W_SH:		break;
		
		// ���κ� 
		case CG_W:
		case CP_W:			break;
		case CG_F_U:
		case CP_F_U:		break;
		case CG_F_L:
		case CP_F_L:		break;
		case CG_BR_W:
		case CP_BR_W:		break;
		case CG_BR_F_U:
		case CP_BR_F_U:		break;
		case CG_BR_F_L:
		case CP_BR_F_L:		break;
		case CG_BR_GA:
		case CP_BR_GA:		break;
		case CG_BR_GA_FA:
		case CP_BR_GA_FA:	break;
		case CG_V:
		case CP_V:			return GetPerimeterCrossBeamVStiff(pBx,nRow,nCol,nDimType,bFront);
		case CG_H:
		case CP_H:			return GetPerimeterCrossBeamHStiff(pBx,nRow,nCol,nDimType,bFront);
		case CP_SP_U_U:
		case CG_SP_U_U:		break;
		case CP_SP_U_L:
		case CG_SP_U_L:		break;
		case CP_SP_L_U:
		case CG_SP_L_U:		break;
		case CG_SP_L_L:
		case CP_SP_L_L:		break;
		case CG_SP_W_MO:
		case CP_SP_W_MO:	break;
		case CG_SP_W_SH:
		case CP_SP_W_SH:	break;

		// ��Ÿ����
		case GI_SB:				break;
		case GI_SQ:				break;
		case GI_SO:				break;
		case GI_FA:				break;
		case GI_LP:				break;
		case GI_JA_L:			break;	
		case GI_JA_R:			break;
		case GI_JA_SA:			break;
		case GI_ZST:			break;
		case GI_HG:				return GetPerimeterDongbariGori(pBx,nDimType);
		case GI_SA:				break;

	}

	return (vJewon.x+vJewon.z)*2;
}


double CGangjeBase::GetPerimeterCrossBeamHStiff(CPlateBasicIndex *pBx, long nRow, long nCol,
												DIMTYPE nDimType, BOOL bFront)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();

	double dPerimeter = 0;
	CVector	vJewon = pGir->GetJewonTotal(CP_H,pBx,nRow,nCol,DIM_RECT,bFront);
	double  vD	   = pGir->m_pSangse->m_HStiff_VStiff_D;
	double Radian  = GetAngleCuttingHStiff(pBx,vJewon);
	double dLeveL  = Radian<ConstPi/2 ? vJewon.z - (vJewon.x-vD)/tan(Radian) : vJewon.z;
	double dBiasL  = sqrt( pow(vJewon.x-vD,2) + pow(vJewon.z-dLeveL,2) );

	dPerimeter = vJewon.x + (dBiasL + vD)*2 + dLeveL;

	return dPerimeter;
}

double CGangjeBase::GetPerimeterHStiff(CPlateBasicIndex *pBx, long nRow, BOOL bLeft, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();

	double dPerimeter = 0;
	long nSide	   = bLeft ? -1 : 1;
	long nMarkBuje = bLeft ? G_H_L : G_H_R;
	CVector	vJewon = pGir->GetJewonTotal(nMarkBuje, pBx, nRow);

	double  vD	   = pGir->m_pSangse->m_HStiff_VStiff_D;
	double Radian  = GetAngleCuttingHStiff(pBx,vJewon);
	double dLeveL  = Radian<ConstPi/2 ? vJewon.z - (vJewon.x-vD)/tan(Radian) : vJewon.z;
	double dBiasL  = sqrt( pow(vJewon.x-vD,2) + pow(vJewon.z-dLeveL,2) );

	dPerimeter = vJewon.x + (dBiasL + vD)*2 + dLeveL;

	return dPerimeter;
}

double CGangjeBase::GetPerimeterDongbariGori(CPlateBasicIndex *pBx, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();

	double dC = pGir->m_pSangse->m_Dongbari_C;
	CVector	vJewon = pGir->GetJewonTotal(GI_HG,pBx);
	double dPerimeter = (vJewon.x+vJewon.z)*2;

	//�Ϲ�Ÿ��
	if (pGir->m_pSangse->m_Dongbari_Type == 0)
		dPerimeter += (sqrt(2.0)*dC*2 - dC*4);

	//�÷��� ���� Ÿ�� 
	if (pGir->m_pSangse->m_Dongbari_Type == 1)
		dPerimeter += (sqrt(2.0)*dC - dC*2);

	return dPerimeter;
}


double CGangjeBase::GetPerimeterCrossBeamVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, 
												DIMTYPE nDimType, BOOL bFront)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();

	long nMarkBuje = CP_V;
	CVector	vJewon = pGir->GetJewonTotal(nMarkBuje,pBx,nRow,nCol,nDimType,bFront);
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();

	double Station = pBx->GetStation();
	double JSlop   = nDimType==DIM_BASIC ? pGir->GetGiulFlangeUpperLower(Station)
	                                     : pGir->GetGiulFlangeUpperLowerCamber(Station);
	double vD = 10; // FIXED VALUE(??)
	double sL = vJewon.x / cos(atan(JSlop));		// ���ǰ� ���յǴ� ���� ����(������)
	double L1 = vJewon.z;	// ���̰� �� ��
	double L2 = vJewon.z - vJewon.x * tan(atan(JSlop));

	double dPerimeter = sL + pC->m_vW_CR + L1 + L2;

	if(pC->m_vL_CR<pC->m_H_CR)
		dPerimeter += ( sqrt(2.0)*(vJewon.x-vD) - (vJewon.x-vD)*2 );

	return dPerimeter;
}
//�� ������ �ܸ���(�ޱ��� ������)
double CGangjeBase::GetAreaBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam)
{
	if(pBx==NULL)	return -1;
	CPlateBridgeApp	*pDB = m_pDB;
	CPlateGirderApp	*pGir = pBx->GetGirder();
	if(!pDB->IsPlate(nMarkBuje)) return 0;

	CVector vJewon = pGir->GetJewonTotal(nMarkBuje,pBx,nRow,nCol,nDimType,bFront,bLeftBeam);
	if(nDimType==DIM_BASIC)
	{
		double dResult = ( Round(vJewon.x,0) * Round(vJewon.z,0) );
		if(nMarkBuje==G_W && pDB->IsTUGir())
		{
			double dUFlangT = pBx->GetFactChain(G_F_U)->m_dFactChainThick;
			double dS2		= pDB->m_BindConc.m_dDis;
			dResult = (Round(vJewon.x,0) - Round(dS2,0) + Round(dUFlangT,0))*Round(vJewon.z,0);
		}
		if(nMarkBuje==G_W)
			dResult *= 2; //������ // dResult ���� ���ʸ鰪(ARoad��)�̹Ƿ� APlate������ *2 �� �Ѵ�.
		return dResult;
	}

	switch(nMarkBuje)//���縶ũ�� define������
	{
		// �Ϲݺ���
		case G_F_U :		return GetAreaFlangeUpper(pBx,nDimType);
		case G_F_L :		return GetAreaFlangeLower(pBx,nDimType);
		case G_W :			return GetAreaFlangeWeb(pBx,nDimType);
		case G_H_L :		return GetAreaHStiff(pBx,nRow,TRUE ,nDimType);
		case G_H_R :		return GetAreaHStiff(pBx,nRow,FALSE,nDimType);
		case V_ST  :        return GetAreaVStiff(pBx, nRow, nCol, nDimType);
		case V_J_ST:        return GetAreaVJijumStiff(pBx, nRow, nCol, nDimType);
		case VB_GU:         return GetAreaVBrGusset(pBx, nRow, nCol, nDimType); 
		case HB_GU:			return GetAreaHBrGusset(pBx, nDimType); 
		case HB_B:			break;//����극�̽�..			
		// ��������
		case SP_U_U:		
		case SP_U_L:		
		case SP_U_FILL:
		case SP_L_L:		
		case SP_L_U:
		case SP_L_FILL:		break;
		case SP_W_MO:		return pBx->GetSplice()->m_mW*vJewon.z;
		case SP_W_SH:
		case SP_W_FILL:
			if(pBx->GetSplice()->m_nWebType == SP_WEB_FL)
				return pBx->GetSplice()->m_sW*vJewon.z;
			else
				return (pBx->GetSplice()->m_sW*vJewon.z) + (pBx->GetSplice()->m_mW*vJewon.z*2);
		// ���κ� 
		case CG_STEEL:
		case CP_STEEL:
			return pBx->GetSection()->GetCrossBeam()->m_H_CR*vJewon.z
					+ vJewon.z*pBx->GetSection()->GetCrossBeam()->m_uM_CR*2
					+ vJewon.z*pBx->GetSection()->GetCrossBeam()->m_dM_CR*2;
		case CG_W:
		case CP_W:			return pBx->GetSection()->GetCrossBeam()->m_H_CR*vJewon.z;
		case CG_F_U:
		case CP_F_U:		return vJewon.z*pBx->GetSection()->GetCrossBeam()->m_uM_CR*2;
		case CG_F_L:
		case CP_F_L:		return vJewon.z*pBx->GetSection()->GetCrossBeam()->m_dM_CR*2;
		case CG_BR_W:
		case CP_BR_W:		return GetAreaCrossBeamBracketWeb(pBx,nRow,nCol,nDimType);
		case CG_BR_F_U:
		case CP_BR_F_U:		return GetAreaCrossBeamBracketFlange(pBx, 0, nCol,nDimType);
		case CG_BR_F_L:
		case CP_BR_F_L:		return GetAreaCrossBeamBracketFlange(pBx, 1, nCol,nDimType);
		case CG_BR_GA:
		case CP_BR_GA:		return GetAreaCrossBeamBracketGasset(pBx,nRow,nCol,nDimType);
		case CG_BR_GA_FA:
		case CP_BR_GA_FA:	return GetAreaCrossBeamBracketGassetPlate(pBx,nRow,nCol,nDimType);
		case CG_V:
		case CP_V:			return GetAreaCrossBeamVStiff(pBx,nRow,nCol,nDimType,bFront);
		case CG_H:
		case CP_H:			return GetAreaCrossBeamHStiff(pBx,nRow,nCol,nDimType,bFront);
		case CP_SP_U_U:
		case CG_SP_U_U:		break;
		case CP_SP_U_L:
		case CG_SP_U_L:		break;
		case CP_SP_L_U:
		case CG_SP_L_U:		break;
		case CG_SP_L_L:
		case CP_SP_L_L:		break;
		case CG_SP_W_MO:
		case CP_SP_W_MO:	return pBx->GetSection()->GetCrossBeam()->m_mW*vJewon.x;
		case CG_SP_W_FH://ä����
		case CP_SP_W_FH:
		case CG_SP_W_SH:
		case CP_SP_W_SH:	return pBx->GetSection()->GetCrossBeam()->m_sW*vJewon.x;

		// ��Ÿ����
		case GI_SB:				break;
		case GI_SQ:				break;
		case GI_SA:				break;
		case GI_SO:				break;
		case GI_FA:				break;
		case GI_LP:				break;
		case GI_JA_L:			break;	
		case GI_JA_R:			break;
		case GI_JA_SA:			break;
		case GI_ZST:			break;
		case GI_HG:				return GetAreaDongbariGori(pBx,nDimType);
	}
	
	return ( Round(vJewon.x,0) * Round(vJewon.z,0) );
}

double CGangjeBase::GetAreaFlangeUpper(CPlateBasicIndex *pBx, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();

 	CVectorArray vFlange;
	CVector Center;
	pGir->GetVertexFlange(vFlange, pBx, TRUE);
	double Area = vFlange.GetAreaInfo(Center, FALSE);

	return Area;
}

double CGangjeBase::GetAreaFlangeLower(CPlateBasicIndex *pBx, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();

	CVectorArray vFlange;
	CVector Center;
	pGir->GetVertexFlange(vFlange, pBx, FALSE);
	double Area = vFlange.GetAreaInfo(Center, FALSE);

	return Area;
}

double CGangjeBase::GetAreaFlangeWeb(CPlateBasicIndex *pBx, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();

	CVectorArray vWeb;
	CVector Center;
	pGir->GetVertexWeb(vWeb, pBx);
	double dArea = vWeb.GetAreaInfo(Center, FALSE);
	
	return dArea;

}
 
// ���򺸰��� (�Ϲ�)
double CGangjeBase::GetAreaHStiff(CPlateBasicIndex *pBx, long nRow, BOOL bLeftWeb, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();

	double dArea=0;

	long nSide	   = bLeftWeb ? -1 : 1;
	long nMarkBuje = bLeftWeb ? G_H_L : G_H_R;

	CVector	vJewon = pGir->GetJewonTotal(nMarkBuje, pBx, nRow);
	double  vD	   = pGir->m_pSangse->m_HStiff_VStiff_D;
	double Radian  = GetAngleCuttingHStiff(pBx,vJewon);
	double dLeveL  = Radian<ConstPi/2 ? vJewon.z - (vJewon.x-vD)/tan(Radian) : vJewon.z;

	dArea = vJewon.x*vD + (vJewon.x-vD)*(vJewon.z+dLeveL)/2;

	return dArea;
}

// ���������� (�ܸ�)
double CGangjeBase::GetAreaVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CVector	 vJewon = pGir->GetJewonTotal(V_ST,pBx, nRow, nCol);
	double   dArea  = vJewon.z*vJewon.x;
	CVStiff* pVStiff = pBx->GetSection()->GetVStiff();
	
	if(nRow==0 && pVStiff->m_dH[nCol][DOWN]!=0)  // �̰ݵ� �ﰢ�� ����
		dArea -= (pVStiff->m_dWidth[nCol]*pVStiff->m_dWidth[nCol])/2;

	return dArea;
}
// ������ ���������� (�ܸ�)
double CGangjeBase::GetAreaVJijumStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CVector	 vJewon = pGir->GetJewonTotal(V_J_ST,pBx, nRow, nCol);
	double   dArea  = vJewon.z*vJewon.x;
	CVStiff* pVStiff = pBx->GetJijumStiff();
	
	if(nRow==0 && pVStiff->m_dH[nCol][DOWN]!=0)  // �̰ݵ� �ﰢ�� ����
		dArea -= (pVStiff->m_dWidth[nCol]*pVStiff->m_dWidth[nCol])/2;

	return dArea;
}
double CGangjeBase::GetAreaVBrGusset(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CVBracing *pVr        = pBx->GetSection()->GetVBracing();

	CPlatePieceObject* pObj = new CPlatePieceObject(pGir);
	pObj->m_nMarkBuje = VB_GU;
	pVr->CreatePieceObj(pObj, FALSE, FALSE, nCol);
	
	return GetAreaObject(pObj);
}
double CGangjeBase::GetAreaHBrGusset(CPlateBasicIndex *pBx, DIMTYPE nDimType)
{
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();

	CPlatePieceObject* pObj1 = new CPlatePieceObject(pGir);
	CPlatePieceObject* pObj2 = new CPlatePieceObject(pGir);

	pObj1->m_nMarkBuje = HB_GU;
	pObj2->m_nMarkBuje = HB_GU;

	pBrGu->CreatePieceObj(pObj1, pObj2, FALSE, FALSE);
	double dArea1 = GetAreaObject(pObj1);
	double dArea2 = GetAreaObject(pObj2);

	return dArea1+dArea2;
}
// ���κ� ���򺸰���
double CGangjeBase::GetAreaCrossBeamHStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	double dArea=0;

	long nMarkBuje = CP_H;

	CVector	vJewon = pGir->GetJewonTotal(nMarkBuje,pBx,nRow,nCol,nDimType,bFront);
	double  vD	   = pGir->m_pSangse->m_HStiff_VStiff_D;
	double Radian  = GetAngleCuttingHStiff(pBx,vJewon);
	double dLeveL  = Radian<ConstPi/2 ? vJewon.z - (vJewon.x-vD)/tan(Radian) : vJewon.z;

	dArea = vJewon.x*vD + (vJewon.x-vD)*(vJewon.z+dLeveL)/2;

	return dArea;
}

// ���κ� ����������
double CGangjeBase::GetAreaCrossBeamVStiff(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	long nMarkBuje = CP_V;
	CVector	vJewon = pGir->GetJewonTotal(nMarkBuje,pBx,nRow,nCol,nDimType,bFront);
	CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();

	double vDist   = pGir->GetDisGirderToGirder(pBx)/2;
	double Station = pGir->GetStationBySc(pBx->GetStation(),vDist,pBx->GetAngleSectionRight());
	double JSlop   = nDimType==DIM_BASIC ? pGir->GetGiulFlangeUpperLower(Station)
	                                     : pGir->GetGiulFlangeUpperLowerCamber(Station,vDist);
	double vD = 10; // FIXED VALUE(??)
	double dTriAngle = vJewon.x * vJewon.x*JSlop /2;
	double dArea     = vJewon.x * vJewon.z - dTriAngle;

	if(pC->m_vL_CR<pC->m_H_CR)
		dArea -= pow(vJewon.x-vD,2)/2;

	return dArea;
}

// ���ٸ���
double CGangjeBase::GetAreaDongbariGori(CPlateBasicIndex *pBx, DIMTYPE nDimType)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	double dA = pGir->m_pSangse->m_Dongbari_A;
	double dB = pGir->m_pSangse->m_Dongbari_B;
	double dC = pGir->m_pSangse->m_Dongbari_C;
	double dD = pGir->m_pSangse->m_Dongbari_D;
	double dE = pGir->m_pSangse->m_Dongbari_E;

	CVector	vJewon = pGir->GetJewonTotal(GI_HG,pBx);
	double dArea   = vJewon.x*vJewon.z;

	//�Ϲ� Ÿ��
	if (pGir->m_pSangse->m_Dongbari_Type == 0)
		dArea -= ( pow(dC,2)/2*2 + dB*dD + ConstPi*pow(dB/2,2) );
	//�÷��� ���� Ÿ�� 
	if (pGir->m_pSangse->m_Dongbari_Type == 1)
		dArea -= ( pow(dC,2)/2 + ConstPi*pow(dA/2,2)/4 );

	return dArea;
}

double CGangjeBase::GetAngleCuttingHStiff(CPlateBasicIndex *pBx, CVector vJewon)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	// 90���� ����
	if(vJewon.z<=70)	return ConstPi/2;

	// ���ο��� LEVEL �Ÿ�
	double D = pGir->m_pSangse->m_HStiff_VStiff_D;
	double L = vJewon.z;
	double W = vJewon.x;
	double MinLevel = 50; //
	double Rad = ConstPi/4;

	// 60���� ����
	if( (L-(W-D)*2/tan(Rad)) < 50) Rad = ConstPi/3;
	// 60��~90�� ���̷� ����
	if( (L-(W-D)*2/tan(Rad)) < 50) Rad = atan((W-D)/((L-50)/2));

	return min(Rad,ConstPi/2);
}

//���κ� ����� ������
double CGangjeBase::GetAreaCrossBeamBracketGasset(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();

	long nID = 0;
	if((nCol==0 && pC->m_nLGirSpType) || (nCol==1 && pC->m_nRGirSpType))
		nID = 0;		//��������
	if((nCol==0 && !pC->m_nLGirSpType) || (nCol==1 && !pC->m_nRGirSpType))
		nID = 1;		//��Ʈ����

	CPlatePieceObject* pObj = new CPlatePieceObject(pGir);
	pObj->m_nMarkBuje = pBx->IsJijum() ? CP_BR_GA : CG_BR_GA;
	pObj->m_nCol      = nCol;
	if(nID==0)
		pC->CreatePieceObjPlateGassetPlate(pObj);
	else
		pC->CreatePieceObjPlateGassetPlateByBolt(pObj);
	
	return GetAreaObject(pObj);

}

//���κ� ����� ������
double CGangjeBase::GetAreaCrossBeamBracketGassetPlate(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();

	CPlatePieceObject* pObj = new CPlatePieceObject(pGir);
	pObj->m_nMarkBuje = pBx->IsJijum() ? CP_BR_GA_FA : CG_BR_GA_FA;
	pObj->m_nCol      = nCol;

	pC->CreatePieceObjPlate(pObj);
	
	return GetAreaObject(pObj);
	
}

//���κ� ����� ������
double CGangjeBase::GetAreaCrossBeamBracketWeb(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();

	CPlatePieceObject* pObj = new CPlatePieceObject(pGir);
	pObj->m_nMarkBuje = pBx->IsJijum() ? CP_BR_W : CG_BR_W;
	pObj->m_nCol      = nCol;

	pC->CreatePieceObjPlate(pObj);
	
	return GetAreaObject(pObj);
}
//���κ� ����� ��,����
double CGangjeBase::GetAreaCrossBeamBracketFlange(CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();

	CPlatePieceObject* pObj = new CPlatePieceObject(pGir);
	pObj->m_nCol      = nCol;
	if(nRow==0)
		pObj->m_nMarkBuje = pBx->IsJijum() ? CP_BR_F_U : CG_BR_F_U;
	else
		pObj->m_nMarkBuje = pBx->IsJijum() ? CP_BR_F_L : CG_BR_F_L;

	pC->CreatePieceObjPlateGassetFlange(pObj, TRUE, FALSE, FALSE, FALSE);
	
	return GetAreaObject(pObj);

}

// ���� ������ �ش��ϴ� �߷��� ����
double CGangjeBase::GetWeightBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam)
{
	CPlateGirderApp *pGir   = pBx->GetGirder();
	CPlateBridgeApp *pDB    = m_pDB;
	CGlobarOption   *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	//
	CVector vJewon   = pGir->GetJewonTotal(nMarkBuje, pBx, nRow, nCol, nDimType, bFront, bLeftBeam);
	double dUWeigth  = pDB->GetWeightUnit(nMarkBuje,pBx,vJewon);
	double dAreaBuje = GetAreaBuje(nMarkBuje, pBx, nRow, nCol, nDimType, bFront);
	long   nRound    = pGlopt->GetPointValueOfGangJae(); // ���� �ݿø� �ڸ���
	BOOL   bRound	 = FALSE;

	// ��Ʈ  : �Ѱ����߷��� ����
	// PLATE : �Ϲݺ���(���� x �����߷�)
	// PIPE  : ���̴��߷� x ����
	double dTWeight = 0;
	if(pDB->IsBolt(nMarkBuje))
		dTWeight = dUWeigth;
	else if(pDB->IsPlate(nMarkBuje))
		dTWeight = dAreaBuje*vJewon.y*dUWeigth;
	else
		dTWeight = vJewon.z*dUWeigth;

	// �ݿø� ó��
	if(bRound)
		dTWeight = Round(dTWeight,nRound);

	return dTWeight;
}

// ����(W*L) �ش��ϴ� �߷��� ����
double CGangjeBase::GetWeightJewon(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, DIMTYPE nDimType, BOOL bFront, BOOL bLeftBeam, CVector *pJewon)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = m_pDB;
	CGlobarOption   *pGlopt = m_pDB->m_pDataManage->GetGlobalOption();
	//
	long nRoundUnit  = pGlopt->GetPointValueOfUnitGangJae();
	long nRoundTot   = pGlopt->GetPointValueOfGangJae();

	CVector vJewon    = (pJewon) ? *pJewon : pGir->GetJewonTotal(nMarkBuje, pBx, nRow, nCol, nDimType, bFront,bLeftBeam);
	double  W		  = toM(Round(vJewon.x,0));
	double  T		  = toM(Round(vJewon.y,0));
	double  L		  = toM(Round(vJewon.z,0));
	double  dUWeigth  = Round(pDB->GetWeightUnitKgM(nMarkBuje,pBx,vJewon),nRoundUnit);
	BOOL    bRound	  = TRUE;

	// ��Ʈ  : �Ѱ����߷��� ����
	// PLATE : �Ϲݺ���(���� x �����߷�)
	// PIPE  : ���̴��߷� x ����
	double dTWeight = 0;
	if(pDB->IsBolt(nMarkBuje) || nMarkBuje==GI_SB)
		dTWeight = dUWeigth;
	else if(pDB->IsPlate(nMarkBuje))
		dTWeight = W*L*T*dUWeigth;
	else
		dTWeight = L*dUWeigth;

	// �ݿø� ó��
	if(bRound)
		dTWeight = Round(dTWeight,nRoundTot);

	return dTWeight;
}

double CGangjeBase::GetAreaDeductBoltHole(long nMarkBuje, CPlateBasicIndex *pBx, CString sBoltSize)
{	
	double BoltR = m_pDB->GetBoltRad(sBoltSize);
	double dArea = pow(BoltR,2)*ConstPi;

	return dArea;
}

CVector CGangjeBase::GetJewonDeductBoltHole(long nMarkBuje, CPlateBasicIndex *pBx, CString sBoltSize)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	double BoltR = m_pDB->GetBoltRad(sBoltSize);

	CVector vJewon(0,0,0);
	vJewon.y = pGir->GetThickJewon(nMarkBuje,pBx);
	vJewon.x = BoltR*2;
	vJewon.z = BoltR*2;

	return vJewon;
}

long CGangjeBase::GetMaterialKindNumber(long nMarkBuje)
{
	CPlateBridgeApp *pDB  = m_pDB;

	long nKindNumber = -1;
	CString str = pDB->GetMaterialSteelSymbol(nMarkBuje);

	if(str.Compare("PL")==0)		
		return MATERIAL_KIND_PLATE;
	else if(str.Compare("BT")==0)	
		return MATERIAL_KIND_BOLT;
	else if(str.Compare("BT 2N")==0)
		return MATERIAL_KIND_BOLT2NUT;
	else if(str.Compare("RB")==0)	
		return MATERIAL_KIND_ROUNDBAR;
	else if(str.Compare("WS")==0)	
		return MATERIAL_KIND_STUDWELD;
	else if(str.Compare("BS")==0)	
		return MATERIAL_KIND_STUDBOLT;
	else if(str.Compare("WM")==0)	
		return MATERIAL_KIND_WIREMESH;
	else if(str.Compare("PI")==0)	
		return MATERIAL_KIND_PIPE;
	else if(str.Compare("RE-BAR")==0)
		return MATERIAL_KIND_REBAR;
	else if(str.Compare("EL")==0)	
		return MATERIAL_KIND_ELBOW;
	else if(str.Compare("LA")==0)	
		return MATERIAL_KIND_ELBOWEQUAL;
	else if(str.Compare("SL")==0)	
		return MATERIAL_KIND_SLEEVEFORPIPE;
	else if(str.Compare("BEAM")==0)	
		return MATERIAL_KIND_BEAM;
	else
		return -1;
}

double CGangjeBase::GetAreaDeductWebTaper(CPlateBasicIndex* pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	BOOL bStt = (pBx->IsState(BX_STT_GIRDER)) ? TRUE : FALSE;

	CVector vJeown = GetJewonDeductWebTaper(pBx, bLeft);

	double dTaperWidth = bStt ? pDB->m_dLengthTaperToTaperStt : pDB->m_dLengthTaperToTaperEnd;
	double dArea = vJeown.x * vJeown.z - vJeown.x*dTaperWidth/2;
	
	return dArea;
}

CVector CGangjeBase::GetJewonDeductWebTaper(CPlateBasicIndex* pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	BOOL bStt = (pBx->IsState(BX_STT_GIRDER)) ? TRUE : FALSE;

	double dLenGirderShoe = bStt ? pDB->m_dLengthGirderShoeStt   : pDB->m_dLengthGirderShoeEnd;
	double dLenShoeTaper  = bStt ? pDB->m_dLengthShoeToTaperStt  : pDB->m_dLengthShoeToTaperEnd;
	double dLenTaper	  = bStt ? pDB->m_dLengthTaperToTaperStt : pDB->m_dLengthTaperToTaperEnd;

	CVector vJewon;
	vJewon.z = dLenGirderShoe + dLenShoeTaper + dLenTaper;
	vJewon.y = pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	vJewon.x = bStt ? pDB->m_dHeightTaperStt : pDB->m_dHeightTaperEnd;

	return vJewon;
}

double  CGangjeBase::GetAreaDeductCrossBeamHStiff(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol, BOOL bFront)
{
	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC	  = pBx->GetSection()->GetCrossBeam();

	if(pC==NULL) return 0;

	double D = pGir->m_pSangse->m_HStiff_VStiff_D;
	double L = pGir->GetLengthJewon(nMarkBuje,pBx,nRow,nCol);
	double W = pC->m_hW_CR;
	double MinLevel = 50; //
	double Rad = ConstPi/4;
	
	if( (L-(W-D)*2/tan(Rad)) < 50) Rad = ConstPi/3;
	if( (L-(W-D)*2/tan(Rad)) < 50) Rad = atan((W-D)/((L-50)/2));

	double slopL = (W-D)/tan(Rad);

	double dArea = (W-D) * (L-slopL)/2 / 2;

	return dArea;
}

// CPlateCrossBeam�� PieceObject���� ����
CVector CGangjeBase::GetJewonDeductCrossBeamHStiff(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol, BOOL bFront)
{
	CPlateGirderApp*pGir = pBx->GetGirder();
	CPlateCrossBeam *pC  = pBx->GetSection()->GetCrossBeam();

	CDPoint vAng = pBx->GetAngleSectionRight();
	double D = pGir->m_pSangse->m_HStiff_VStiff_D;
	double L = pGir->GetLengthJewon(nMarkBuje,pBx,nRow,nCol,DIM_RECT,bFront);
	double W = pC->m_hW_CR;
	double MinLevel = 50; //
	double Rad = ConstPi/4;
	if( (L-(W-D)*2/tan(Rad)) < 50) Rad = ConstPi/3;
	if( (L-(W-D)*2/tan(Rad)) < 50) Rad = atan((W-D)/((L-50)/2));
	double slopL = (W-D)/tan(Rad);

	CVector vJewon;
	vJewon.x = (L-slopL)/2;
	vJewon.y = pC->m_hT_CR;
	vJewon.z = W - D;

	return vJewon;
}

double  CGangjeBase::GetAreaDeductCrossBeamBracketGasset(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol)
{
	CVector vJewon = GetJewonDeductCrossBeamBracketGasset(nMarkBuje,pBx,nRow,nCol);

	double dArea = vJewon.x*vJewon.z/2;

	return dArea;
}

CVector CGangjeBase::GetJewonDeductCrossBeamBracketGasset(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateGirderApp *pGirR= pGir->m_pRight;
	CPlateBridgeApp	*pBridge  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);

	long nID=0;
	CPlateCrossBeam* pC1 = pBx->GetSection()->GetCrossBeam();
	if((nCol==0 && pC1->m_nLGirSpType) || (nCol==1 && pC1->m_nRGirSpType))
		nID = 0;		//��������
	if((nCol==0 && !pC1->m_nLGirSpType) || (nCol==1 && !pC1->m_nRGirSpType))
		nID = 1;		//��Ʈ����

	CDPoint A[10];
	pC->GetCrossXyDom(A);
	CDPoint vAng = pBx->GetAngle();
	double Sharp = pGir->GetLengthSharpOnBx(pBx, TRUE);
	double Tl    = pC->m_T1_CR;
	double Dslop = 0;
	double slop  = pGir->GetGiulPlateCrossGusset(pBx);
	double S1	 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S1);	// ���Է¸޴� ����
	double S2	 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S2);
	double S3	 = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_S3);
	double BoxH  = pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double WebT	 = pGir->GetThickFactByStation(pBx->GetStation(), 2);

	///////////
//	CDPoint Po[5];
	CDPoint xy1, xy2;
	if(nCol==0)
	{
/*		Po[0] = CDPoint(A[8].x+WebT, A[2].y-(A[2].x-A[8].x+WebT)*slop-pC->m_H_CR-Tl);
		Po[1] = CDPoint(A[2].x-S2,  A[2].y-S2*slop-pC->m_H_CR-Tl);
		Po[2] = CDPoint(A[2].x-S2,  A[2].y-S2*slop-pC->m_H_CR-Tl-S3);
		Po[3] = CDPoint(A[8].x+Sharp-S1, A[8].y+(Sharp-S1)*Dslop-BoxH);
		Po[4] = CDPoint(A[8].x+WebT, A[8].y-BoxH);
*/
		if(pC->m_nLGirSpType)
		{
			xy1 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, TRUE, TRUE);
			xy2 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, FALSE, TRUE);
		}
		else
		{
			xy1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, TRUE, TRUE);
			xy2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, FALSE, TRUE);
		}
	}
	else 
	{
/*		slop  = pGir->GetGiulPlateCrossGusset(pBxR);
		Dslop = 0;
		Sharp = pGirR->GetLengthSharpOnBx(pBxR, TRUE);
		BoxH  = pGirR->GetHeightGirderByStaAng(pBxR->GetStation());
		//
		Po[0] = CDPoint(A[9].x-WebT, A[5].y+(A[9].x-A[5].x+WebT)*slop-pC->m_H_CR-Tl);
		Po[1] = CDPoint(A[5].x+S2,  A[5].y+S2*slop-pC->m_H_CR-Tl);
		Po[2] = CDPoint(A[5].x+S2,  A[5].y+S2*slop-pC->m_H_CR-Tl-S3);
		Po[3] = CDPoint(A[9].x-(Sharp-S1), A[9].y-(Sharp-S1)*Dslop-BoxH);
		Po[4] = CDPoint(A[9].x-WebT, A[9].y-BoxH-WebT*slop);
*/	
		if(pC->m_nRGirSpType)
		{
			xy1 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, TRUE, FALSE);
			xy2 = pC->GetPointGassetSupPlateEndBx(pBx, vAng, FALSE, FALSE);
		}
		else
		{
			xy1 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, TRUE, FALSE);
			xy2 = pC->GetPointGassetSupPlateByBoltEndBx(pBx, vAng, FALSE, FALSE);
		}
	}

	CVector vJewon;
//	vJewon.x = fabs(Po[2].x - Po[3].x);
//	vJewon.z = fabs(Po[2].y - Po[3].y);
	vJewon.x = fabs(xy1.x - xy2.x);
	vJewon.z = fabs(xy1.y - xy2.y);
	vJewon.y = pBridge->GetGussetPlateJewon(pBx, nID, CPlateBridgeApp::GUSSET_T);

	return vJewon;
}

double  CGangjeBase::GetAreaDeductCrossBeamBracketWeb(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol)
{
	CVector vJewon = GetJewonDeductCrossBeamBracketWeb(nMarkBuje,pBx,nRow,nCol);

	double dArea = vJewon.x*vJewon.z/2;

	return dArea;
}

double  CGangjeBase::GetAreaDeductCrossBeamBracketFlange(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol)
{
	CVector vJewon = GetJewonDeductCrossBeamBracketFlange(nMarkBuje,pBx,nRow,nCol);

	double dArea = vJewon.x*vJewon.z/2;

	return dArea;
}

// �������� : �ϴ� �ﰢ��(??)���� ó��
CVector CGangjeBase::GetJewonDeductCrossBeamBracketWeb(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateGirderApp *pGirR= pGir->m_pRight;
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);

	double Giul = pGir->GetGiulPlateCrossGusset(pBx);
	double Len  = pC->m_P_CR;
	double Wid  = fabs(Len*Giul);

	CVector vJewon;

	vJewon.x = Wid;
	vJewon.y = pC->m_T3_CR;
	vJewon.z = Len;

	return vJewon;
}

// �������� : �ϴ� �ﰢ��(??)���� ó��
CVector CGangjeBase::GetJewonDeductCrossBeamBracketFlange(long nMarkBuje, CPlateBasicIndex* pBx, long nRow, long nCol)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateGirderApp *pGirR= pGir->m_pRight;
	CPlateBridgeApp	*pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateCrossBeam *pC   = pBx->GetSection()->GetCrossBeam();
	CPlateBasicIndex* pBxR = pBx->GetBxMatchByCrossBeam(pGirR);

	BOOL  bUpper = (nMarkBuje==CP_BR_F_U || nMarkBuje==CG_BR_F_U) ? TRUE : FALSE;

	double webT  = nCol==0 ? pGir->GetThickFactByStation(pBx->GetStation(), 2)
		                   : pGirR->GetThickFactByStation(pBx->GetStation(), 2);
    double Sharp = nCol==0 ? pGir->GetLengthSharpOnBx(pBx, TRUE)
	                       : pGirR->GetLengthSharpOnBx(pBxR, TRUE);
	double C	 = bUpper  ? pDB->m_dRoundChipCrossUpper : pDB->m_dRoundChipCrossLower;
	double Wid   = bUpper  ? pC->m_uN_CR : pC->m_dN_CR;
	double Len   = bUpper  ? pC->m_P_CR-Sharp-C : pC->m_P_CR-webT-C;

	CVector vJewon;

	vJewon.x = Wid;
	vJewon.y = bUpper ? pC->m_T2_CR : pC->m_T1_CR;
	vJewon.z = Len;

	return vJewon;
}

double CGangjeBase::GetAreaObject(CPlatePieceObject* pObj)
{
	CVectorArray vArr;
	pObj->GetPiecePointArray().GetVectorArray(vArr);
	CDPointArray xyArr;
	for(long a=0; a<vArr.GetSize(); a++)
	{
		CDPoint xy = CDPoint(vArr.GetAt(a).x, vArr.GetAt(a).y) ;
		xyArr.Add(xy);
	}
	return xyArr.GetArea();

}
