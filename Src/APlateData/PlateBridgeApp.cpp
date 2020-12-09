// PlateBridgeApp.cpp: implementation of the CPlateBridgeApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"
#include "PlateBridgeApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateBridgeApp::CPlateBridgeApp(CLineInfo* pLine) : CPlateBridge(pLine)
{
	delete m_pCRGirder;
	m_pCRGirder = new CPlateGirderApp(this,pLine,NULL);
}

CPlateBridgeApp::~CPlateBridgeApp()
{
	AhTPADelete(&m_SlabArr, (CSlabApp *)0);
}

void CPlateBridgeApp::Serialize(CArchive &ar)
{
	CPlateBridge::Serialize(ar);
	
	if(!m_SlabArr.GetSize()) MakeSlab();	
	GetSlab(SLAB_LEFT)->Serialize(ar);
	GetSlab(SLAB_RIGHT)->Serialize(ar);		
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���������(Ⱦ���Է¿��� ������)
///////////////////////////////////////////////////////////////////////////////////////////////
void CPlateBridgeApp::MakeSlab()
{	
	if(m_SlabArr.GetSize()>0)
		AhTPADelete(&m_SlabArr, (CSlabApp *)0);	

	for(long nS = 0; nS < 2; nS++)
	{
		CSlabApp *pGir = new CSlabApp(this, m_pLine);

		m_SlabArr.Add(pGir);		
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////
///�����
///////////////////////////////////////////////////////////////////////////////////////////////
long CPlateBridgeApp::GetAbutSu() const
{
	long nResult = 0;

	if(m_nTypeAbutOrPier[0] == 0) ++nResult;
	if(m_nTypeAbutOrPier[1] == 0) ++nResult;

	return nResult;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///�Ǿ��
///////////////////////////////////////////////////////////////////////////////////////////////
long CPlateBridgeApp::GetPierSu() const
{
	return m_nQtyJigan - GetAbutSu() + 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///������ �����ΰ�?
///////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPlateBridgeApp::IsAbutOnJiJum(long nJijum) const
{
	if(nJijum == 0 && m_nTypeAbutOrPier[STT_POS] == 0)				return TRUE;
	if(nJijum == m_nQtyJigan && m_nTypeAbutOrPier[END_POS] == 0)	return TRUE;

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// ���� ȸ����
///////////////////////////////////////////////////////////////////////////////////////////////
CDPoint CPlateBridgeApp::GetAngleBridgeCenter() const
{
	CDPoint ptStt = m_pLine->GetXyLineDis(GetStationBridgeStt(), 0);
	CDPoint ptEnd = m_pLine->GetXyLineDis(GetStationBridgeEnd(), 0);

	return (ptEnd - ptStt).Unit(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�������� ��ǥ
///////////////////////////////////////////////////////////////////////////////////////////////
void CPlateBridgeApp::GetXyPaveUpperDom(CPlateBasicIndex *pBx, CDPoint A[HDANLINESU]) const
{
//	GetXyPaveAct(pBx->GetStation(), A);
	for(long i=0;i<HDANLINESU;i++)
	{
		A[i].x=0;
		A[i].y=0;
	}

	double sta			= pBx->GetStation();
	double disSlabLeft	= GetSlabLeft()->m_dGirderCenter;
	double slope		= m_pCRGirder->GetLine()->GetPyungubae(sta, disSlabLeft);//���� ������ ������ ����
	double eleSlabLeft	= 0;
	if(disSlabLeft < 0)			//������ ������ �����߽� ������ ������
		eleSlabLeft = -(disSlabLeft + GetDisSlabLeftLevelDom(pBx))*slope;
	else
	{
		if(!m_cLevelRoadCenterToSlabEdge)
			eleSlabLeft = disSlabLeft * slope;
	}

	double x = disSlabLeft;
	double y = eleSlabLeft;

	A[0].x = x;
	A[0].y = y;

	long nQtyHDanNode = GetQtyHDanNode();
	double dLengthHDanAct = 0;		
	for(long n = 0; n<nQtyHDanNode; n++)
	{
		dLengthHDanAct = GetLengthHDanAct(sta, n);		
		//x += GetLengthHDanDom(pBx, n);
		if(GetTypeLevelHDan(n) == 0)		//Level�� �ƴҶ�
		{
			slope	= m_pCRGirder->GetLine()->GetPyungubae(sta, A[n].x);
			if(A[n].x < -0.0000001)	slope *= -1;		//�����϶��� ����� ���� ���(��->��)�� �ݴ�
			
			x += dLengthHDanAct;
			y += dLengthHDanAct*slope;
			if(x > 0 && A[n].x < 0)
			{
				double slopeRight = m_pCRGirder->GetLine()->GetPyungubae(sta, x);
				y -= x * slope;	//�߰��Ǿ��� ���� ���̸� ������.
				y += x * slopeRight;
			}
			A[n+1].x += x;
			A[n+1].y = y;
		}
		else					//Level �϶�
		{
			x += dLengthHDanAct;

			A[n+1].x += x;
			A[n+1].y = y;
		}		
	}	
}

/////////////////////////////////////////////////////////////////////
///������ ��(����Ÿ�)
///Ȯ�� : ���κ��� ���� ���� ���� ����
///�Ϲ� : ���κ��� ���� ���� ���� ����
/////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetWidthSlabDom(CPlateBasicIndex *pBxGir) const
{
	if(IsExtLine())	return GetWidthDiap90CrossAng(pBxGir);			//Ȯ���� ���
	else			return GetWidthDiap90Cross90(pBxGir);			//Ȯ���� �ƴҰ��
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���� ������ġ(�Ŵ���ȣ, ������ȣ)
///////////////////////////////////////////////////////////////////////////////////////////////
CDPoint CPlateBridgeApp::GetXyPlateShoe(long nG, long nJijum)
{
	CPlateGirderApp *pGir = GetGirder(nG);

	double	sta		= pGir->GetStationOnJijum(nJijum);
	CDPoint vAng    = pGir->GetAngleOnJijum(nJijum);
	
	CDPoint xy = pGir->GetXyGirderDis(sta, 0, vAng);

	return xy;
}

BOOL CPlateBridgeApp::IsSafeCross(long nRow, long nCol, BOOL bCheckPier, BOOL bCheckCrossRight, BOOL bCheckCrossLeft) const
{
	BOOL bSafeCrossByPier = bCheckPier ? IsSafeCrossByPier(nRow, nCol) : TRUE;
	//BOOL bSafeCrossByCrossBeamRight = bCheckPier ? IsSafeCrossByCrossBeam(nRow, nCol, TRUE) : TRUE;
	//BOOL bSafeCrossByCrossBeamLeft = bCheckPier ? IsSafeCrossByCrossBeam(nRow, nCol, FALSE) : TRUE;

	if(bSafeCrossByPier)
	{
		//if(bSafeCrossByCrossBeamRight || bSafeCrossByCrossBeamLeft)	return TRUE;
		return TRUE;
		if(GetGirdersu() == 1)	return TRUE;
	}

	return FALSE;
}

BOOL CPlateBridgeApp::IsSafeCrossByPier(long nRow, long nCol) const
{
	if(m_CrossAngleGrid.GetTextMatrixDouble(nRow, nCol) <= 0)	return FALSE;
	long nG = (nRow-3)/2;
	CPlateGirderApp *pGir	= GetGirder(nG);

	double safeZoneDis = 200;
	double staCross = GetStationInDBGrid(0, nRow, nCol);
	double angRad = ToRadian(m_CrossAngleGrid.GetTextMatrixDouble(nRow+1, nCol));
	CDPoint vAng(cos(angRad), sin(angRad));
	double staCrossLeft = pGir->GetStationBySc(staCross, 0, vAng);
	double staCrossRight = pGir->GetStationBySc(staCross, 0, vAng);

	double staPier = pGir->GetSSS(OFF_STT_SHOE);
	CDPoint vAngPier = pGir->GetAAA(OFF_STT_SHOE);
	double staPierLeft = pGir->GetStationBySc(staPier, 0, vAngPier);
	double staPierRight = pGir->GetStationBySc(staPier, 0, vAngPier);

	if(staCrossLeft < staPierLeft+safeZoneDis || staCrossRight < staPierRight+safeZoneDis)//���� ���� ����
		return FALSE;


	for(long nPier=0; nPier < m_nQtyJigan-1; nPier++)//������ ����
	{
		staPier = pGir->GetStationOnJijum(nPier+1);
		vAngPier = pGir->GetAngleOnJijum(nPier+1);
		staPierLeft = pGir->GetStationBySc(staPier, 0, vAngPier);
		staPierRight = pGir->GetStationBySc(staPier, 0, vAngPier);
//		if((staCrossLeft < staPierLeft+safeZoneDis && staCrossRight > staPierRight+safeZoneDis) ||
//			staCrossLeft < staPierLeft-safeZoneDis && staCrossRight < staPierRight-safeZoneDis)
		double staLeftDiff = staCrossLeft - staPierLeft;
		double staRightDiff = staCrossRight - staPierRight;
		if(staLeftDiff * staRightDiff < 0.0 || fabs(staLeftDiff) < safeZoneDis || fabs(staRightDiff) < safeZoneDis)
			return FALSE;
	}

//	staPier = pGir->GetStationEnd();
//	vAngPier = pGir->GetAngleEnd();
	staPier = pGir->GetSSS(OFF_END_SHOE);
	vAngPier = pGir->GetAAA(OFF_END_SHOE);
	staPierLeft = pGir->GetStationBySc(staPier, 0, vAngPier);
	staPierRight = pGir->GetStationBySc(staPier, 0, vAngPier);

	if(staCrossLeft > staPierLeft-safeZoneDis || staCrossRight > staPierRight-safeZoneDis)//���� ���� ����
		return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�����ο� �Ϲݺ��� ��츦 �����Ͽ� ��ũ�� ����
///////////////////////////////////////////////////////////////////////////////////////////////
long CPlateBridgeApp::GetMarkBujeByMarkBujeAndBx(long nMarkBuje, CPlateBasicIndex *pBx) const
{	
	long nMarkBujeReal = nMarkBuje;
	switch(nMarkBuje)
	{
	case CP_W:			
	case CG_W:			
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_W;
			else				nMarkBujeReal = CG_W;
			break;
		}
	
	case CP_F_U:		
	case CG_F_U:		
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_F_U;
			else				nMarkBujeReal = CG_F_U;
			break;
		}
	case CP_STEEL:
	case CG_STEEL:
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_STEEL;
			else				nMarkBujeReal = CG_STEEL;
			break;
		}
	case CP_F_L:		
	case CG_F_L:		
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_F_L;
			else				nMarkBujeReal = CG_F_L;
			break;
		}

	case CP_V:			
	case CG_V:			
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_V;
			else				nMarkBujeReal = CG_V;
			break;
		}

	case CP_H:			
	case CG_H:			
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_H;
			else				nMarkBujeReal = CG_H;
			break;
		}

	case CP_SP_U_U:		
	case CG_SP_U_U:		
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_SP_U_U;
			else				nMarkBujeReal = CG_SP_U_U;
			break;
		}

	case CP_SP_U_L:		
	case CG_SP_U_L:		
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_SP_U_L;
			else				nMarkBujeReal = CG_SP_U_L;
			break;
		}

	case CP_SP_L_U:		
	case CG_SP_L_U:		
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_SP_L_U;
			else				nMarkBujeReal = CG_SP_L_U;
			break;
		}

	case CP_SP_L_L:		
	case CG_SP_L_L:		
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_SP_L_L;
			else				nMarkBujeReal = CG_SP_L_L;
			break;
		}

	case CP_SP_W_MO:	
	case CG_SP_W_MO:	
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_SP_W_MO;
			else				nMarkBujeReal = CG_SP_W_MO;
			break;
		}
	case CP_SP_W_SH:	
	case CG_SP_W_SH:	
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_SP_W_SH;
			else				nMarkBujeReal = CG_SP_W_SH;
			break;
		}
	case CP_SP_W_FH:	///ä����.
	case CG_SP_W_FH:	
		{
			if(pBx->IsJijum())	nMarkBujeReal = CP_SP_W_FH;
			else				nMarkBujeReal = CG_SP_W_FH;
			break;
		}
	}
	return nMarkBujeReal;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���������� ���縶ũ�� �̿��� ������
///////////////////////////////////////////////////////////////////////////////////////////////
CString CPlateBridgeApp::GetMaterial(long nMarkBuje) const
{
	return 	m_pDataManage->GetSteelSection()->GetMaterialByMarkBuje(nMarkBuje);
}

long CPlateBridgeApp::GetBridgeType() const
{
	return m_pDataManage->GetGlobalOption()->GetDesignBridgeType();
}

BOOL CPlateBridgeApp::IsPlateGir()
{
	return GetBridgeType()==PLATE_GIR?TRUE:FALSE;
}

BOOL CPlateBridgeApp::IsSosuGir()
{
	return GetBridgeType()==SOSU_GIR?TRUE:FALSE;
}

BOOL CPlateBridgeApp::IsTUGir()
{
	return GetBridgeType()==TU_GIR?TRUE:FALSE;
}

BOOL CPlateBridgeApp::IsTUContinuousSpan()
{
	BOOL bContinuousSpan = m_nQtyJigan>1 ? TRUE : FALSE;
	return (IsTUGir()&&bContinuousSpan) ? TRUE : FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///�Է� : nRow	���򺸰���(������, ���̾�����, ���κ�, ���κ� ��)	-> ������ ���� 1��, 2��, 3��, 4��
///				������ -> �����δ� 1��°, 2��°...
///		 nCol	���򺸰���(������) -> �տ��� ����(����) ù��°, �ι�°....
///				���򺸰���(���̾�����, ���κ�, ���κ� ��) -> �������� ���� ù��°, �ι�°....
///��� : nMarkBuje ���翡 ���� Mark�� ������
///////////////////////////////////////////////////////////////////////////////////////////////
CString CPlateBridgeApp::GetMarkBuje(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol, BOOL bFront) const
{
	nMarkBuje = GetMarkBujeByMarkBujeAndBx(nMarkBuje, pBx);
	return "";
} 

///////////////////////////////////////////////////////////////////////////////////////////////
///��� : �Ϲ� ���� ���� �߷��� ���� (W*T*L*Q*U = WeightTotal)
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetWeightUnitPlate(long nMarkBuje) const
{
	CString sMat = GetMaterial(nMarkBuje);
	long nIdxMaterial = m_pDataManage->GetSteel()->GetIndex(sMat);
	if(nIdxMaterial==-1)	return 0;
	
	return	m_pDataManage->GetSteel()->GetUnitWeight();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///��� : ��Ʈ�� ���� �߷��� ���� (Q*U = WeightTotal)
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetWeightUnitBolt(long nMarkBuje, CVector vJ)const
{
	CString sMat = GetMaterial(nMarkBuje);
	CSteelBoltData *pBoltData = m_pDataManage->GetSteelBolt()->GetBolt(sMat, Round(vJ.y,0));

	if(!pBoltData) return -1;

	double dHeadWeight	= pBoltData->m_BaseWeight;
	double dUnitWeight	= pBoltData->m_LengthPerWeight;
	double dNutWeight	= pBoltData->m_NutWeight;
	double dWasher		= pBoltData->m_WasherWeight;
	long nWasherQty		= (pBoltData->m_strGrade.Find("S") != -1 ? 1 : 2);

	return dHeadWeight + dUnitWeight*vJ.z + dNutWeight + dWasher*nWasherQty;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////////
CShoe *CPlateBridgeApp::GetShoeByRowCol(long nRow, long nCol) const
{
	CPlateGirderApp *pGir = GetGirder(nRow);
	return pGir->GetShoe(nCol);	
}

///////////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetStationShoeOnCRLine(long nRow, long nCol) const
{
	CPlateGirderApp *pGir = GetGirder(nRow);
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nCol);
	CDPoint xy = pGir->GetXyGirderDis(pBx->GetStation(),0);
	double sta = 0;
	m_pCRGirder->GetLine()->GetStationMatchVerticalByXy(xy, sta);
	return sta;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�� ��ǥ���� ������.
///////////////////////////////////////////////////////////////////////////////////////////////
CDPoint CPlateBridgeApp::GetXyShoe(long nRow, long nCol)const
{	
	CPlateGirderApp *pGir = GetGirder(nRow);

	double	sta		= pGir->GetStationOnJijum(nCol);
	CDPoint vAng    = pGir->GetAngleOnJijum(nCol);	
	
	CDPoint xy = pGir->GetXyGirderDis(sta, 0, vAng);
	return xy;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// ��(nRow,nCol)�� �����ִ� �Ŵ� ��ȣ�� ����
///////////////////////////////////////////////////////////////////////////////////////////////
long CPlateBridgeApp::GetNumberGirderByShoe(long nRow, long nCol)const
{
	return nRow;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�Ŵ��߽ɿ��� �������� �Ÿ��� ����
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetDisShoeOnGirderCenter(long nRow, long nCol)const
{	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�� ������ ���� ����=�Ŵ���
///////////////////////////////////////////////////////////////////////////////////////////////
long CPlateBridgeApp::GetQtyShoeOnJijum(long nJijum)const
{
	return GetGirdersu();	
}


double CPlateBridgeApp::GetDisSlopActOnShoe(long nRow, long nCol) const
{
	CPlateGirderApp		*pGir	= GetGirder(GetNumberGirderByShoe(nRow,nCol));
	CPlateBasicIndex	*pBx	= pGir->GetBxOnJijum(nCol);

//	double dSta  = GetStationShoeOnCRLine(nRow,nCol);
	double  dSta   = pBx->GetStation();
	double  dDis   = GetDisShoeOnGirderCenter(nRow, nCol);
	CDPoint vAngle = pGir->GetAngleOnJijum(nCol);

	double dDisToCenterLine = fabs(pGir->GetDisToCenterLineAct(dSta,dDis,vAngle));
	double dDisLevel	    = fabs(GetDisLevelActOnShoe(nRow,nCol));

	return fabs(dDisToCenterLine - dDisLevel);

}
///////////////////////////////////////////////////////////////////////////////////////////////
///����ĵƿ������ ����(1���Ŵ��߽ɿ��� �������������ܱ����ǰŸ�)
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetWidthSlabLeft()  const
{ 
	return fabs(GetSlabLeft()->m_dGirderCenter - GetGirder(0)->m_dGirderCenter); 
}

///////////////////////////////////////////////////////////////////////////////////////////////
///����ĵƿ������ ����(�������Ŵ��߽ɿ��� ������������ܱ����ǰŸ�)
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetWidthSlabRight() const 
{ 
	return fabs(GetSlabRight()->m_dGirderCenter - GetGirder(GetGirdersu()-1)->m_dGirderCenter); 
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// nMarkBuje ���簡 ��Ʈ������ ����(BOLT / NUT / WASHER)
///////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPlateBridgeApp::IsMaterialBolt(long nMarkBuje) const
{
	switch(nMarkBuje)
	{
		case	SP_U_BOLT		:	//��� ��������	��Ʈ&��Ʈ
		case	SP_L_BOLT		:	//�Ϻ� ��������	��Ʈ&��Ʈ
		case	SP_W_BOLT		:	//������ ��������	��Ʈ&��Ʈ
		case	CP_BOLT			:	//���κ� ������	��Ʈ&��Ʈ
		case	CG_BOLT			:	//���κ� �Ϲݺ�	������ ��Ʈ&��Ʈ
		case	VB_BOLT	:
		case	HB_BOLT	: return TRUE;
	}
	return FALSE;
}

double CPlateBridgeApp::GetGussetPlateJewon(CPlateBasicIndex* pBx, int nWeld, long nJewon)
{
	switch(nJewon)
	{
	case GUSSET_GEN:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_nGussetStiff_Gen[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_nGussetStiff_Gen[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_nGussetStiff_Gen[2][nWeld];
			break;
		}
	case GUSSET_B:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Bolt_B[0];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Bolt_B[1];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Bolt_B[2];
			break;
		}
	case GUSSET_H:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Bolt_H[0];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Bolt_H[1];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Bolt_H[2];
			break;
		}
	case GUSSET_R:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Bolt_R[0];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Bolt_R[1];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Bolt_R[2];
			break;
		}
	case GUSSET_ST:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Sub_sT[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Sub_sT[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Sub_sT[2][nWeld];
			break;
		}
	case GUSSET_DD:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Sub_dD[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Sub_dD[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Sub_dD[2][nWeld];
			break;
		}
	case GUSSET_DE:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Sub_dE[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Sub_dE[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Sub_dE[2][nWeld];
			break;
		}
	case GUSSET_UD:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Sub_uD[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Sub_uD[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Sub_uD[2][nWeld];
			break;
		}
	case GUSSET_UE:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_Sub_uE[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_Sub_uE[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_Sub_uE[2][nWeld];
			break;
		}
	case GUSSET_T:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_T[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_T[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_T[2][nWeld];
			break;
		}
	case GUSSET_S1:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_S1[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_S1[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_S1[2][nWeld];
			break;
		}
	case GUSSET_S2:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_S2[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_S2[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_S2[2][nWeld];
			break;
		}
	case GUSSET_S3:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_S3[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_S3[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_S3[2][nWeld];
			break;
		}
	case GUSSET_W:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_W[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_W[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_W[2][nWeld];
			break;
		}
	case GUSSET_DA:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_dA[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_dA[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_dA[2][nWeld];
			break;
		}
	case GUSSET_DB:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_nGussetStiff_Gen[0][nWeld]==0 ? m_dGS_Gen_dB[0][nWeld] : m_dGS_Trap_dB[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_nGussetStiff_Gen[1][nWeld]==0 ? m_dGS_Gen_dB[1][nWeld] : m_dGS_Trap_dB[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_nGussetStiff_Gen[2][nWeld]==0 ? m_dGS_Gen_dB[2][nWeld] : m_dGS_Trap_dB[2][nWeld];
			break;
		}
	case GUSSET_DC:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_nGussetStiff_Gen[0][nWeld]==0 ? m_dGS_Gen_dC[0][nWeld] : m_dGS_Trap_dC[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_nGussetStiff_Gen[1][nWeld]==0 ? m_dGS_Gen_dC[1][nWeld] : m_dGS_Trap_dC[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_nGussetStiff_Gen[2][nWeld]==0 ? m_dGS_Gen_dC[2][nWeld] : m_dGS_Trap_dC[2][nWeld];
			break;
		}
	case GUSSET_UA:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_uA[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_uA[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_uA[2][nWeld];
			break;
		}
	case GUSSET_UB:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_uB[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_uB[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_uB[2][nWeld];
			break;
		}
	case GUSSET_UC:
		{
			if(pBx->GetSection()->IsenType(SECTION_A))
				return m_dGS_uC[0][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_P))
				return m_dGS_uC[1][nWeld];
			else if(pBx->GetSection()->IsenType(SECTION_C))
				return m_dGS_uC[2][nWeld];
			break;
		}
	}
	return 0;
}

//�Է� : pBxGir	�ش� �Ŵ��� Bx ��ġ
//��� : ������ ����� �������� ������.(�����߽��� ������ ���̿� ������ +1 �� �������� ����)
//		 ���� ������ ����� �׸��ų�, Elevation, ���̰��� ��쿡 ���
long CPlateBridgeApp::GetQtyHDanNodeByCenterLinePositionAct(CPlateBasicIndex *pBxGir) const
{
	if(pBxGir == NULL)	return m_nQtyHDanNode;
	CPlateBasicIndex *pBxCen = pBxGir->GetBxMatchByCrossBeam(m_pCRGirder);
	if(pBxCen == NULL)	pBxCen = pBxGir->GetBxMatchByVert(m_pCRGirder);
	double dSta = pBxCen->GetStation();

	double width		= GetWidthAct(dSta);
	double disSlabLeft	= GetDisSlabLeftAct(dSta);
	if(disSlabLeft < 0 && (disSlabLeft + width) > 0)	//�����߽��� ������ ���̿� ���� ���
		return m_nQtyHDanNode+1; 

	return m_nQtyHDanNode;
}
//��� : ������ ����� �������� ������.(�����߽��� ������ ���̿� ������ +1 �� �������� ����)
long CPlateBridgeApp::GetQtyHDanNode() const
{
	return m_nQtyHDanNode;
}
void CPlateBridgeApp::SetQtyHDanNode(long nQtyHDanNode)
{
	m_nQtyHDanNode = nQtyHDanNode; 
}

//�Է� : sta	�ش� �Ŵ��� �����̼�
//		 nHDanNode	ã������ ������ ���� ����
//��� : �ش� �����̼ǿ��� nHDanNode�� ����
//		 Act �϶��� �ʺ���.
double CPlateBridgeApp::GetLengthHDanAct(double sta, long nHDanNode)  const
{
	if(GetTypeVariableLengthHDan(nHDanNode) != 1 || !IsExtLine())		//���� ���� �ƴ� ��� 
		return m_dLengthHDan[nHDanNode];

	//���� �����϶�
	double	widthAct = GetWidthAct(sta);
	double	widthInputTotal	= 0;	//���� �� ��ġ���� �Է� �ʺ� ��� ���� ��
	double	widthInputWithoutVariable = 0;
	long	nVarLineSu	= 0;		//���� ���� ����
	for(long n = 0; n<GetQtyHDanNode(); n++)
	{
		widthInputTotal += m_dLengthHDan[n];
		if(GetTypeVariableLengthHDan(n) == 1)		//���� �����϶�(Ȯ������ ���, �⺻ ������ ���� �̻����.)
			nVarLineSu++;
		else
			widthInputWithoutVariable += m_dLengthHDan[n];
	}
//	if(leftDisDom > 0 || rightDisDom < 0)	//�����߽��� ������ ������ ���� ���
		return (widthAct - widthInputWithoutVariable)/nVarLineSu;

/*
	//���������̰�, Ȯ���̸�, �����߽��� ������ �߰��ο� �ִ� ���
	CPlateBasicIndex *pBx = GetGirder(0)->GetBxOnJijum(0);	//�Է� ������ 0�� �Ŵ� ù��° ���� ���̾�����
	double  widthDom = GetWidthSlabDom(pBx);
	double  leftDisDom = GetDisSlabLeftDom(pBx); //������ ������ [-], ������ ������ [+]
	double  rightDisDom = widthDom + leftDisDom;

	double  leftDisInput = 0;
	long    nHDanCenter = 0;
	double  CenterDisInput = 0;
	double	widthAct = GetWidthAct(sta);
	double  leftDisAct = GetDisSlabLeftAct(sta); //������ ������ [-], ������ ������ [+] ����� �׻� [-]
	double  rightDisAct = widthAct + leftDisAct;
	long	nVarLineSu	= 0;		//���� ���� ����
	double  widthInputHDan = 0;
	for(long nHDan = 0; nHDan<GetQtyHDanNode(); nHDan++)
	{
		if(nHDan == nHDanNode)	break;
		widthInputHDan += m_dLengthHDan[nHDan];
	}
	for(nHDan = 0; nHDan<GetQtyHDanNode(); nHDan++)
	{
		CenterDisInput += m_dLengthHDan[nHDan];
		if(CenterDisInput + 50 > -leftDisDom)
		{
			nHDanCenter = nHDan+1;
			break;
		}
	}
	if(widthInputHDan < -leftDisDom)	//���������� ���ؼ��� ������ ���� ���
	{
		for(nHDan = 0; nHDan < nHDanCenter; nHDan++)
		{
			widthInputWithVariable += m_dLengthHDan[nHDan];
			if(GetTypeVariableLengthHDan(nHDan) == 1)		//���� �����϶�(Ȯ������ ���, �⺻ ������ ���� �̻����.)
				nVarLineSu++;
			else
			{
				if(widthInputWithVariable > -leftDisAct)	//�����߽ɿ� �������� ���
					widthInputWithoutVariable += (widthInputWithVariable + leftDisAct);
				else
					widthInputWithoutVariable += m_dLengthHDan[nHDan];
			}
		}
		return (-leftDisAct - widthInputWithoutVariable)/nVarLineSu;
	}

	for(nHDan = nHDanCenter; nHDan<GetQtyHDanNode(); nHDan++)
	{
		if(GetTypeVariableLengthHDan(nHDan) == 1)		//���� �����϶�(Ȯ������ ���, �⺻ ������ ���� �̻����.)
			nVarLineSu++;
		else
			widthInputWithoutVariable += m_dLengthHDan[nHDan];
	}
	return (rightDisAct - widthInputWithoutVariable)/nVarLineSu;
*/
}
long CPlateBridgeApp::GetTypeLevelHDan(long nHDanNode)  const
{
	return GetValueTypeHDan(nHDanNode, 0);
}
long CPlateBridgeApp::GetWallGuardTypeHDan(long nHDanNode)  const
{
	return GetValueTypeHDan(nHDanNode, 1);
}
long CPlateBridgeApp::GetTypeVariableLengthHDan(long nHDanNode)  const
{
	return GetValueTypeHDan(nHDanNode, 2);
}

BOOL CPlateBridgeApp::IsGuard(long nHDanNode, long nTypeHDan)
{
	BOOL bGuard		= GetValueTypeHDan(nHDanNode, nTypeHDan) == 1 ? TRUE : FALSE;
	BOOL bMiddle	= GetValueTypeHDan(nHDanNode, nTypeHDan) == 2 ? TRUE : FALSE;

	return (bGuard || bMiddle) ? TRUE : FALSE;
}
//�Է� : sta	�ش� �Ŵ��� �����̼�
//		 nHDanNode	ã������ ������ ���� ����
//		 nValueHDan	0 = ���� ����, 1 = ��ȣ�� ����, 2 = �������� ���θ� ����
//��� : �ش� �����̼ǿ��� nHDanNode�� ���� ����, ��ȣ�� ����, �������� ���θ� ����
long CPlateBridgeApp::GetValueTypeHDan(long nHDanNode, long nTypeHDan)  const
{
	return m_nValueTypeHDan[nHDanNode][nTypeHDan];//����. ����
}

//�Է� : sta	�ش� �Ŵ��� �����̼�
//		 nHDanNode	ã������ ������ ���� ����
//		 nValueHDan	0 = ���� ����, 1 = ��ȣ�� ����, 2 = �������� ���θ� ����
//��� : �ش� �����̼ǿ��� nHDanNode�� ���� ����, ��ȣ�� ����, �������� ���θ� ����
void CPlateBridgeApp::SetValueTypeHDan(long nHDanNode, long nTypeHDan, long nValueHDan) 
{
	m_nValueTypeHDan[nHDanNode][nTypeHDan] = nValueHDan;	//����. ����
}

double	CPlateBridgeApp::GetSlopeApplyBase(BOOL bJong) const
{
	CGlobarOption *pGlopt = m_pDataManage->GetGlobalOption();
	return bJong ? pGlopt->GetDesignBoltJongVSlopApplyUp()/100 : pGlopt->GetDesignBoltJongVSlopApplyDn()/100;
}

//�Է� : nRow �׸����� ��(nG=(nRow-3)/2)
//       nCol ���̾����� ���� ũ���� Į����ȣ (ù��° �׸��� Į�� = "0")
//��� : nCol �׸����� ���̾������� ��ȿ�� ���̾��������� Ȯ��(TRUE == ��ȿ��)
//            ��ȿ��-�����ο� ���̾�����(200mm)�� ���������ʰ�, ���κ� ������ ����.
//                   ���̾������� ��ȿ�ϰ� ���� �Ǵ� ���� ���κ��� ��ȿ��.
// bCheckPier => �Ǿ�κ� üũ
// bCheckDiapRight => ������ ���̾����� üũ
// bCheckDiapLeft => ���� ���̾����� üũ
BOOL CPlateBridgeApp::IsSafeDiap(long nRow, long nCol, BOOL bCheckPier, BOOL bCheckDiapRight, BOOL bCheckDiapLeft) const
{
	BOOL bSafeDiapByPier = bCheckPier ? IsSafeDiapByPier(nRow, nCol) : TRUE;
	BOOL bSafeDiapByCrossBeamRight = bCheckPier ? IsSafeDiapByCrossBeam(nRow, nCol, TRUE) : TRUE;
	BOOL bSafeDiapByCrossBeamLeft = bCheckPier ? IsSafeDiapByCrossBeam(nRow, nCol, FALSE) : TRUE;

	if(bSafeDiapByPier)
	{
		if(bSafeDiapByCrossBeamRight || bSafeDiapByCrossBeamLeft)	return TRUE;
		if(GetGirdersu() == 1)	return TRUE;
	}

	return FALSE;
}

//�Է� : nRow �׸����� ��(nG=(nRow-3)/2)
//       nCol ���̾����� ���� ũ���� Į����ȣ (ù��° �׸��� Į�� = "0")
//��� : nCol �׸����� ���̾������� ��ȿ�� ���̾��������� Ȯ��(TRUE == ��ȿ��)
//            ��ȿ��-�����ο� ���̾�����(200mm)�� ���� ����
BOOL CPlateBridgeApp::IsSafeDiapByPier(long nRow, long nCol) const
{
	if(m_CrossAngleGrid.GetTextMatrixDouble(nRow, nCol) <= 200)	return FALSE;
	long nG = (nRow-3)/2;
	CPlateGirderApp *pGir	= GetGirder(nG);

	double safeZoneDis = 10;
 	double staDiap = GetStationInDBGrid(0, nRow, nCol);
	double angRad = ToRadian(m_CrossAngleGrid.GetTextMatrixDouble(nRow+1, nCol));
	CDPoint vAng(cos(angRad), sin(angRad));
	double staDiapLeft = pGir->GetStationBySc(staDiap, 0, vAng);
	double staDiapRight = pGir->GetStationBySc(staDiap, 0, vAng);

//	double staPier = pGir->GetStationStt();
//	CDPoint vAngPier = pGir->GetAngleStt();
	double staPier = pGir->GetSSS(OFF_STT_SHOE);
	CDPoint vAngPier = pGir->GetAAA(OFF_STT_SHOE);
	double staPierLeft = pGir->GetStationBySc(staPier, 0, vAngPier);
	double staPierRight = pGir->GetStationBySc(staPier, 0, vAngPier);

	if(staDiapLeft < staPierLeft+safeZoneDis || staDiapRight < staPierRight+safeZoneDis)//���� ���� ����
		return FALSE;


	for(long nPier=0; nPier < m_nQtyJigan-1; nPier++)//������ ����
	{
		staPier = pGir->GetStationOnJijum(nPier+1);
		vAngPier = pGir->GetAngleOnJijum(nPier+1);
		staPierLeft = pGir->GetStationBySc(staPier, 0, vAngPier);
		staPierRight = pGir->GetStationBySc(staPier, 0, vAngPier);
//		if((staDiapLeft < staPierLeft+safeZoneDis && staDiapRight > staPierRight+safeZoneDis) ||
//			staDiapLeft < staPierLeft-safeZoneDis && staDiapRight < staPierRight-safeZoneDis)
		double staLeftDiff = staDiapLeft - staPierLeft;
		double staRightDiff = staDiapRight - staPierRight;
		if(staLeftDiff * staRightDiff < 0.0 || fabs(staLeftDiff) < safeZoneDis || fabs(staRightDiff) < safeZoneDis)
			return FALSE;
	}

//	staPier = pGir->GetStationEnd();
//	vAngPier = pGir->GetAngleEnd();
	staPier = pGir->GetSSS(OFF_END_SHOE);
	vAngPier = pGir->GetAAA(OFF_END_SHOE);
	staPierLeft = pGir->GetStationBySc(staPier, 0, vAngPier);
	staPierRight = pGir->GetStationBySc(staPier, 0, vAngPier);

	if(staDiapLeft > staPierLeft-safeZoneDis || staDiapRight > staPierRight-safeZoneDis)//���� ���� ����
		return FALSE;

	return TRUE;
}

//�Է� : nRow �׸����� ��(nG=(nRow-3)/2)
//       nCol ���̾����� ���� ũ���� Į����ȣ (ù��° �׸��� Į�� = "0")
//��� : nCol �׸����� ���κ��� ��ȿ�� ���κ����� Ȯ��(TRUE == ��ȿ��)
//            ��ȿ��-�����ο� ���κ�(500mm)�� ���� ����
BOOL CPlateBridgeApp::IsSafeDiapByCrossBeam(long nRow, long nCol, BOOL bRight) const
{

	long nG = (nRow-3)/2;
	if(nCol < 0 || (bRight && nG >= GetGirdersu()-1) || (!bRight && nG <= 0))	return FALSE;
	if(!bRight) nRow -= 2;	//���� ���κ� ���˽ÿ� ���� �Ŵ��� �������� �ű�.
	nG = (nRow-3)/2;		//nG �� �׻� 1�̻� ��.

	CPlateGirderApp *pGir	= GetGirder(nG);
	CPlateGirderApp *pGirRight	= GetGirder(nG+1);

	if(nCol >= m_CrossAngleGrid.GetCols()) return FALSE;
	
	double disTotal = GetStationInDBGrid(0, nRow, nCol);
	double disTotalRight = GetStationInDBGrid(0, nRow+2, nCol);

	long nJiganNo = pGir->GetNumberJiganByStation(disTotal);
	long nJiganNoRight = pGirRight->GetNumberJiganByStation(disTotalRight);

	BOOL bLeftDiap  = IsSafeDiapByPier(nRow, nCol);
	BOOL bRightDiap = IsSafeDiapByPier(nRow + 2, nCol);

	return ((nJiganNo == nJiganNoRight) && (bLeftDiap && bRightDiap));
}

double CPlateBridgeApp::GetStationInDBGrid(long nGridType, long nRow, long nCol) const
{
	int nLengthType;
	double dDisCenter;
	if (nGridType == 0)   {
		nLengthType = m_nLengthTypeOnCrBeam;
		dDisCenter  = m_dDisCenterOnCrBeam;
	}
	else   {
		nLengthType = m_nLengthTypeOnVst;
		dDisCenter  = m_dDisCenterOnVst;
	}
	double lenSum = 0, sta;
	for(long c = 1; c <= nCol; c++)
		lenSum += nGridType ? m_VStiffGrid.GetTextMatrixDouble(nRow, c) : m_CrossAngleGrid.GetTextMatrixDouble(nRow, c);
  	sta = GetStationByLengthInGrid(TRUE, nRow/2-1, lenSum, nLengthType, dDisCenter);
	return sta;
}
 
//<summary>
//
//</summary>
//<param name="bSttShoe"></param>
//<param name="nG"></param>
//<param name="dTotalLen"></param>
//<param name="nLengthType"></param>
//<param name="dDisCenter"></param>
//<return></return>
double CPlateBridgeApp::GetStationByLengthInGrid(BOOL bSttShoe, long nG, double dTotalLen, int nLengthType, double dDisCenter) const
{
	CPlateGirderApp *pGir = GetGirder(nG);
	double staStt = pGir->GetSSS(bSttShoe ? OFF_STT_SHOE : OFF_STT_GIRDER);
	double dDisCenterLine = dDisCenter;

	if (nLengthType == 0)	//�Ŵ��߾� ��������
		dDisCenterLine = pGir->m_dGirderCenter;
	
	double sta = pGir->GetLine()->GetStationByDisLength(staStt, dTotalLen, dDisCenterLine);
	return sta;
//	return Round(sta, 0);
}

//<summary>
//���� ����ġ �Ǵ� �Ŵ� �������� ���� �����̼� ������ ���̸� ����.
//</summary>
//<param name="bSttShoe">������ ����ġ���� �Ŵ� ���� ��ġ������ ����</param>
//<param name="nG">�Ŵ� ��ȣ</param>
//<param name="sta">�����̼�</param>
//<param name="nLengthType"> 0 = �� �Ŵ��߾� ��������, 1 = �����߽� �̰� �Ÿ�����</param>
//<param name="dDisCenter">�����߽� �̰� �Ÿ������� ���� �Ÿ�, �� �Ŵ��߾� ��������(nLengthType == 0)������ ������� ����</param>
//<return>���� ����ġ �Ǵ� �Ŵ� �������� ���� �����̼� ������ ���̸� ����.
//		  Ȯ���϶��� �ƴ� �Ŵ� �ڽ��� ������ ���� �Ÿ���.</return>
double CPlateBridgeApp::GetLengthByStationInGrid(BOOL bSttShoe, long nG, double sta, int nLengthType, double dDisCenter) const
{
	CPlateGirderApp *pGir = GetGirder(nG);
	double staStt = pGir->GetSSS(bSttShoe ? OFF_STT_SHOE : OFF_STT_GIRDER);
	double dDisCenterLine = dDisCenter;

	if (nLengthType == 0)	//�Ŵ��߾� ��������
		dDisCenterLine = pGir->m_dGirderCenter;;

	double length = pGir->GetLine()->GetLengthByDis(staStt, sta, dDisCenterLine);//���ؼ��� ���� ����

	return length;
}



//���� : �����߽�(Ȯ���� ��� ����) �����̼ǿ��� �������� �÷������� ������ ���ܰ� ����
//�Է� : sta	�����߽�(Ȯ���� ��� ����) �����̼�
//��� : ������ ���ܿ��� ���ܱ����� �Ÿ�
double CPlateBridgeApp::GetWidthAct(double sta) const
{
	CPlateGirderApp *pGirL = GetGirder(0);				//���� �Ŵ�(0��) ������
	CPlateGirderApp *pGirR = GetGirder(GetGirdersu()-1);	//���� �Ŵ� ������
	CDPoint vAngSta(0,1);	//���� �����߽� �����̼ǰ� �̷�� ���� 90���� ��.
	CDPoint vAng = m_pCRGirder->GetLine()->GetAngleAzimuth(sta).Rotate(CDPoint(0,1));//������ + 90��,��ü ��ǥ���� ����
	CDPoint xy = m_pCRGirder->GetLine()->GetXyLine(sta);
	double	staL = sta;
	double	staR = sta;	//���� �����̼�
	CDPoint vAngL = CDPoint(0, 1);
	CDPoint vAngR = CDPoint(0, 1);
	CDPoint xyL;
	CDPoint xyR;
 
	if(pGirL->GetLine() == m_pLine)
		xyL = pGirL->GetXyGirderDis(staL, -m_dWidthSlabLeft, vAngL);
	else
	{
		if(!pGirL->GetLine()->GetStationMatchByXyAndAngle(xy, vAng, staL))	return 0;
		vAngL = pGirL->GetLine()->GetAngleAzimuth(staL).BetweenAngle(vAng);
		double staSlabLeft = pGirL->GetLine()->GetStationByScInLine(staL, (pGirL->m_dGirderCenter-m_dWidthSlabLeft), vAngL);
		xyL  = pGirL->GetLine()->GetXyLineDis(staSlabLeft, (pGirL->m_dGirderCenter-m_dWidthSlabLeft));
	}

	if(pGirR->GetLine() == m_pLine)
		xyR = pGirR->GetXyGirderDis(staR, m_dWidthSlabRight, vAngR);
	else
	{
		if(!pGirR->GetLine()->GetStationMatchByXyAndAngle(xy, vAng, staR))	return 0;
		vAngR = pGirR->GetLine()->GetAngleAzimuth(staR).BetweenAngle(vAng);
		xyR  = pGirR->GetXyGirderDis(staR, m_dWidthSlabRight, vAngR);
	}

	return ~(xyL-xyR);
}

//���� : Pavement ����� ���� �Է�, ���� ������ ���� x, y ��ǥ�� ���Ѵ�.
//�Է� : sta	�⺻ ������ ���� �����̼�
//		 A[]	��ȯ ���� ���� ��ǥ��
//��� : �⺻ ������ ������ Pavement ����� �� ��ǥ��
//		 ���� ��ǥ�� �����߽ɼ��� Pavement ��ǥ�� (0,��ȹ��)���� ��. (���� [-], ���� [+])
//					*-> �����߽� ��ȹ�� ���ԵǾ� ����.
void CPlateBridgeApp::GetXyPaveAct(double sta, CDPoint A[HDANLINESU]) const
{
	long nHDan = 0;
//	GetXyPave(TRUE, A, sta);
	for(nHDan=0; nHDan<HDANLINESU; nHDan++)
	{
		A[nHDan].x = 0;
		A[nHDan].y = 0;
	}
	double disSlabLeft	= 0;
	long nQtyHDanReal = 0;

	double disLineToLine = 0;

	disSlabLeft	= GetDisSlabLeftAct(sta);
	nQtyHDanReal = GetQtyHDanAct(sta);
	
	A[0].x = disSlabLeft;
	A[0].y = 0;

	long nHDanAct = 0;	//���� ��� ������. nHdan �� ���ų� 1�� �� ���� �� ����.(�����߽��� �����곻�� ���� ���)
	double slope = 0;
	double heightFrSlabLeftToCen = 0;	//������ �������ܰ� Ⱦ�� ���ؼ��� �߽ɰ��� ������
	double dLengthHDanAct = 0;
	long nQtyHDan = GetQtyHDanNode();
	for(nHDan = 0; nHDan<nQtyHDan; nHDan++)
	{
		if(GetTypeLevelHDan(nHDan) == 0)	slope	= m_pCRGirder->GetLine()->GetPyungubae(sta, A[nHDanAct].x);
		else							slope	= 0;
		if(A[nHDanAct].x - disLineToLine <= -0.0000001)	slope = -slope; 		

		dLengthHDanAct = GetLengthHDanAct(sta, nHDan);		

		if(A[nHDanAct].x <= disLineToLine && (A[nHDanAct].x + dLengthHDanAct) > disLineToLine)	//�����߽��� ���������� Ⱦ���� �����߽ɿ��� �и��Ͽ� �߰�
		{
			double disFrZero = dLengthHDanAct + A[nHDanAct].x;
			A[nHDanAct+1].x = disLineToLine;
			A[nHDanAct+1].y = A[nHDanAct].y + fabs(A[nHDanAct].x-disLineToLine)*slope;
			if(GetTypeLevelHDan(nHDan) == 0)
				slope	= m_pCRGirder->GetLine()->GetPyungubae(sta, 0);	//"0"�� ���(���ؼ��� �߽��� ���)
			heightFrSlabLeftToCen = A[nHDanAct+1].y;
			A[nHDanAct+2].x = disFrZero;
			A[nHDanAct+2].y = A[nHDanAct+1].y + fabs(disFrZero-disLineToLine) * slope;

			nHDanAct++;
		}
		else
		{
			A[nHDanAct+1].x = A[nHDanAct].x + dLengthHDanAct;
			A[nHDanAct+1].y = A[nHDanAct].y + dLengthHDanAct*slope;

			if(A[nHDanAct].x + dLengthHDanAct == disLineToLine)
			{			
				double disFrZero = dLengthHDanAct + A[nHDanAct].x;
				heightFrSlabLeftToCen = A[nHDanAct+1].y;
				A[nHDanAct+2].x = disFrZero;
				A[nHDanAct+2].y = A[nHDanAct+1].y + fabs(disFrZero-disLineToLine) * slope;
			}
		}
		nHDanAct++;		
	}
		
	//heightFrSlabLeftToCen �� ���ؼ����� ������ �¿��� ���̿� ���� ���(���ؼ����� �����긦 ����� ���� ���)���� ���� ������ ����.
	if(A[0].x >= 0)	//���� ������ ������ �����߽��� ������ ���� ���
	{
		heightFrSlabLeftToCen = A[0].y;	//�׻� "0"��
		if(!m_cLevelRoadCenterToSlabEdge)	//ö������ ��쿡�� �׻� ������ ������.
		{
			double slopeCen		= m_pCRGirder->GetLine()->GetPyungubae(sta, A[0].x);//���� ������ ������ ����
			heightFrSlabLeftToCen += (-A[0].x*slopeCen);	//���� �����곡�ܿ��� ���ؼ����߽ɱ��� �Ÿ� ���簡 �����
		}
	}
	else if(A[nQtyHDanReal].x <= 0)	//���� ������ ������ �����߽��� ������ ���� ���
	{
		heightFrSlabLeftToCen = A[nQtyHDanReal].y;
		if(!m_cLevelRoadCenterToSlabEdge)	//ö������ ��쿡�� �׻� ������ ������.
		{
			double slopeCen		= m_pCRGirder->GetLine()->GetPyungubae(sta, A[nQtyHDanReal].x);//���� ������ ������ ����
			heightFrSlabLeftToCen += (A[nQtyHDanReal].x*slopeCen);	//���� �����곡�ܿ��� ���ؼ����߽ɱ��� �Ÿ� ���簡 �����
		}
	}
	
	//�� ���α����� ������ �������� �������̼��� "0"�� �����Ǿ� ����. �̰��� Ⱦ���� ���ؼ��� �߽��� "0"�� �ǵ��� ��ȯ��.
	for(nHDan=0; nHDan < nQtyHDanReal+1; nHDan++)	//�����߽� ��ġ�� ���̸� "0"�� ������.
		A[nHDan].y -= heightFrSlabLeftToCen;

	// (APLATE-686) ���ܰ� ������ ���� �־���.
//	GetXyPaveActWithoutJongPlanEL(sta, A);
//
//	double planEL = m_pLine->GetElevationPlanEl(sta);
//	for(long n = 0; n <HDANLINESU; n++)
//	{
//		A[n].y += planEL;
//	}
}	

//<summary>
//�⺻ ������ ������ Pavement ����� �� ��ǥ��
//���� ��ǥ�� �����߽ɼ��� Pavement ��ǥ�� (0,0)���� ��.(���� [-], ���� [+])
//		-> �����߽� ��ȹ�� ���ԵǾ� ���� ����.
//</summary>
//<param name="sta">���ؼ��� �����̼�</param>
//<param name="A[HDANLINESU]">���� ��� ��ǥ��(�������̼�)</param>
void CPlateBridgeApp::GetXyPaveActWithoutJongPlanEL(double sta, CDPoint A[HDANLINESU]) const
{
	long  n = 0;
	for(n=0; n<HDANLINESU; n++)
	{
		A[n].x = 0;
		A[n].y = 0;
	}
	double disSlabLeft	= GetDisSlabLeftAct(sta);
	double slope		= m_pCRGirder->GetLine()->GetPyungubae(sta, disSlabLeft);	//���� �����곡���� ����
	double eleSlabLeft	= 0;
	if(disSlabLeft < 0)									//������ ������ �����߽� ������ ������
		eleSlabLeft = (fabs(disSlabLeft) - GetDisSlabLeftLevelAct(sta))*slope;
	else if(disSlabLeft > 0 && !m_cLevelRoadCenterToSlabEdge)//������ �ְ�, [�����߽� ��������]�� �ƴҰ��
		eleSlabLeft = disSlabLeft * slope;

	double x = disSlabLeft;
	double y = eleSlabLeft;
	A[0].x = x;
	A[0].y = y;
	
	long nQtyHDanNode = GetQtyHDanNode();
	double dLengthHDanAct = 0;	
	for(n = 0; n<nQtyHDanNode; n++)
	{
		dLengthHDanAct = GetLengthHDanAct(sta, n);		
		if(GetTypeLevelHDan(n) == 0)		//Level�� �ƴҶ�
		{												//Ȯ�� ������ ���Ͽ� �۾�
			slope	= m_pCRGirder->GetLine()->GetPyungubae(sta, A[n].x);
			if(A[n].x < -0.0000001)	slope *= -1;		//������ ���� ���

			x += dLengthHDanAct;
			y += dLengthHDanAct*slope;
			if(x > 0 && A[n].x < 0)
			{
				double slopeRight = m_pCRGirder->GetLine()->GetPyungubae(sta, x);
				y -= x * slope;	//�߰��Ǿ��� ���� ���̸� ������.
				y += x * slopeRight;
			}
			A[n+1].x += x;
			A[n+1].y = y;
		}
		else					//Level �϶�
		{
			x += dLengthHDanAct;

			A[n+1].x += x;
			A[n+1].y = y;
		}
	}
/*
    2003.04.05 Ȳ��ȯ GetLengthHDanAct()����Ͽ� ������.
	double	width	= 0;
	long	nQtyVariableLine	= 0;		//���� ���� ����
	double	lenVariableLine	= 0;		//���� ������ ��ȭ ����
	CPlateBasicIndex *pBx = m_pCRGirder->GetBxByStation(sta);
	for(n = 0; n<GetQtyHDanNode(); n++)
	{
		width += m_dLengthHDan[n];
		if(GetTypeVariableLengthHDan(n) == 1)		//���� �����϶�(Ȯ������ ���, �⺻ ������ ���� �̻����.)
			nQtyVariableLine++;
	}
	if(IsExtLine())
	{
		double widthAct		= GetWidthAct(sta);
		if(nQtyVariableLine == 1)		lenVariableLine = widthAct - width;
		else							lenVariableLine = (widthAct - width)/2;	//���� ������ 2���϶�
	}

	for(n = 0; n<GetQtyHDanNode(); n++)
	{
		if(GetTypeLevelHDan(n) == 0)		//Level�� �ƴҶ�
		{												//Ȯ�� ������ ���Ͽ� �۾�
			slope	= m_pCRGirder->GetLine()->GetPyungubae(sta, A[n].x);
			if(A[n].x < -0.0000001)	slope *= -1;		//������ ���� ���

			if(GetTypeVariableLengthHDan(n) == 1)		//���� �����϶�(Ȯ������ ���, �⺻ ������ ���� �̻����.)
			{
				x += m_dLengthHDan[n] + lenVariableLine;
				y += (m_dLengthHDan[n] + lenVariableLine)*slope;
			}
			else
			{
				x += m_dLengthHDan[n];
				y += (m_dLengthHDan[n])*slope;
				if(x > 0 && A[n].x < 0)
				{
					double slopeRight = m_pCRGirder->GetLine()->GetPyungubae(sta, x);
					y -= x * slope;	//�߰��Ǿ��� ���� ���̸� ������.
					y += x * slopeRight;
				}
			}
			A[n+1].x += x;
			A[n+1].y = y;
		}
		else					//Level �϶�
		{
			if(GetTypeVariableLengthHDan(n) == 1)	//���� �����϶�(Ȯ������ ���, �⺻ ������ ���� �̻����.)
				x += m_dLengthHDan[n] + lenVariableLine;
			else
				x += m_dLengthHDan[n];

			A[n+1].x += x;
			A[n+1].y = y;
		}
	}

*/
}

//��� : ���� �߽ɺ��� ���� ������ ���ܱ����� �̰ݰŸ�
double CPlateBridgeApp::GetDisSlabLeftAct(double sta) const
{
	CPlateGirderApp *pGirL = GetGirder(0);				//���� �Ŵ�(0��) ������
	CLineInfo*  pLineSec = pGirL->GetLine();
	if(pLineSec == m_pLine)	return pGirL->m_dGirderCenter - m_dWidthSlabLeft;

	CDPoint vAngSta(0,1);	//���� �����߽� �����̼ǰ� �̷�� ���� 90���� ��.
	CDPoint vAng = m_pCRGirder->GetLine()->GetAngleAzimuth(sta).Rotate(vAngSta);//��ü ��ǥ���� ����
	CDPoint xy = m_pCRGirder->GetLine()->GetXyLine(sta);
	double	staL = 0;	//���� �����̼�
 
	if(!pLineSec->GetStationMatchByXyAndAngle(xy, vAng, staL))	return 0;
	CDPoint vAngL = pLineSec->GetAngleAzimuth(staL).BetweenAngle(vAng);
	double staSlabLeft = pLineSec->GetStationByScInLine(staL, (pGirL->m_dGirderCenter-m_dWidthSlabLeft), vAngL);
	CDPoint xyL  = pLineSec->GetXyLineDis(staSlabLeft, (pGirL->m_dGirderCenter-m_dWidthSlabLeft));

	return vAng ^ (xy - xyL);
}



//��� : ���� �߽ɺ��� ���� ������ ���ܱ����� �̰ݰŸ�
double CPlateBridgeApp::GetDisSlabRighAct(double sta) const
{
	CPlateGirderApp *pGirR = GetGirder(GetGirdersu()-1);				// ���� �Ŵ� ������
	CLineInfo*  pLineSec = pGirR->GetLine();
	if(pLineSec == m_pLine)	return pGirR->m_dGirderCenter+m_dWidthSlabRight;

	CDPoint vAngSta(0,1);	//���� �����߽� �����̼ǰ� �̷�� ���� 90���� ��.
	CDPoint vAng = m_pCRGirder->GetLine()->GetAngleAzimuth(sta).Rotate(vAngSta);//��ü ��ǥ���� ����
	CDPoint xy = m_pCRGirder->GetLine()->GetXyLine(sta);
	double	staR = 0;	//���� �����̼�
 
	if(!pLineSec->GetStationMatchByXyAndAngle(xy, vAng, staR))	return 0;
	CDPoint vAngR = pLineSec->GetAngleAzimuth(staR).BetweenAngle(vAng);
	double staSlabRigh = pLineSec->GetStationByScInLine(staR, (pGirR->m_dGirderCenter-m_dWidthSlabRight), vAngR);
	CDPoint xyR  = pLineSec->GetXyLineDis(staSlabRigh, (pGirR->m_dGirderCenter-m_dWidthSlabRight));

	return vAng ^ (xy - xyR);
}


//��� : �����߽� ���� ������ �ѱ���
double CPlateBridgeApp::GetDisSlabLeftLevelAct(double sta) const
{
	double	dDisLevel	= 0;		//�����߽� ������ ���� �� �Ÿ�
	double width = GetWidthAct(sta);
	double	disSlabLeft =  GetDisSlabLeftAct(sta);	//�����߽ɿ��� ������ ���������� ����
	if(disSlabLeft >= 0) return dDisLevel;		//������ ������ �����߽� ������ ������

	double lenTotal = 0;
	double dLengthAct = 0;
	for(long n=0; n<GetQtyHDanNode(); n++)
	{
		dLengthAct = GetLengthHDanAct(sta, n);
		lenTotal += dLengthAct;
		if(GetTypeLevelHDan(n) == 1)		//Level �̸�
			if((lenTotal <= -disSlabLeft))
				dDisLevel += m_dLengthHDan[n];
			else if((lenTotal > -disSlabLeft) && (lenTotal - GetLengthHDanAct(sta, n) < -disSlabLeft))//���� ������ �����߽ɿ� ������ ��
				dDisLevel += (-disSlabLeft - (lenTotal - GetLengthHDanAct(sta, n)));
	}
	if(-disSlabLeft > width && m_cLevelRoadCenterToSlabEdge == TRUE)	dDisLevel += (-disSlabLeft - width);
	return dDisLevel;
}
//<summary>
//�����߽ɿ��� ������ �߾ӱ����� �Ÿ��� ����(������ �߾��� �����߽��� ������ ������ [-] ����)
//</summary>
//<param name="sta">�����߽� �Ŵ��� �����̼�</param>
//<return></return>
double CPlateBridgeApp::GetDisCenLineToCenSlabAct(double sta) const
{
	return GetWidthAct(sta)/2 + GetDisSlabLeftAct(sta);
}

//�Է� : staCR ���� �����̼�
//	   : D	 ���������� �̰ݰŸ�(���� [-], ���� [+])
//		 bEL Elevation ����. �����߽� ��ǥ�� (0,0)�϶��� ���� ��� ����
//��� : ���� �����̼ǿ��� �̰ݵ� �Ÿ��� ��ġ�� ������ ����� ���� ���� ����(mm)
//double CPlateBridgeApp::GetElevationPaveActByDis(double staCR, double D, BOOL bEL/*TRUE*/) const
double CPlateBridgeApp::GetElevationCantilever(double staCR, BOOL bLeft) const
{
	CDPoint A[HDANLINESU];	

	GetXyPaveAct(staCR, A);
	double staGir = 0;
	if(bLeft == TRUE)
	{
		CPlateGirderApp *pGir = GetGirder(0);		
		CDPoint xy = m_pLine->GetXyLineDis(staCR, A[0].x);
		pGir->GetLine()->GetStationMatchVerticalByXy(xy, staGir);
		double D = -m_dWidthSlabLeft;
		CDPoint vAng = CDPoint(0, 1);  //90��
		double dHeightFlangeUpperByTaper = pGir->GetHeightFlangeUpperByTaper(staGir, D, vAng);
		return A[0].y - (m_dThickPave + m_dThickSlabLeft + m_dThickSlabHunch + dHeightFlangeUpperByTaper);
	}
	else
	{
		CPlateGirderApp *pGir = GetGirder(GetGirdersu()-1);		
		CDPoint xy = m_pLine->GetXyLineDis(pGir->GetStationCenLineForEl(staCR), A[GetQtyHDanNode()].x);
		pGir->GetLine()->GetStationMatchVerticalByXy(xy, staGir);
		double D = m_dWidthSlabRight;
		CDPoint vAng = CDPoint(0, 1);  //90��
		double dHeightFlangeUpperByTaper = pGir->GetHeightFlangeUpperByTaper(staGir, D, vAng);
		return A[GetQtyHDanNode()].y - (m_dThickPave + m_dThickSlabRight + m_dThickSlabHunch + dHeightFlangeUpperByTaper);
	}
	return 0;
}
//<summary>
//���� �����̼ǿ��� �������� �̰ݵ� �Ÿ��� ��ġ�� ������ ����� ���� ���� ����(mm)
//</summary>
//<param name="staCR">���ؼ��� �����̼�</param>
//<param name="D">���ؼ��������� �̰ݰŸ�(���� [-], ���� [+])</param>
//<return></return>
double CPlateBridgeApp::GetElevationPaveActByDis(double staCR, double D) const
{
	if(D == 0)		return 0;//GetGirder(-1)->GetLine()->GetElevationPlanEl(staCR);

	double disSlabLeft = GetDisSlabLeftAct(staCR);
	double width = GetWidthAct(staCR);
	double SlabRightDis = width + disSlabLeft;
	if(disSlabLeft > D)		D = disSlabLeft;
	if(SlabRightDis < D)	D = SlabRightDis;

	CDPoint A[HDANLINESU];
	GetXyPaveAct(staCR, A);

	long nQtyHDanNode = GetQtyHDanNode();
	if(D <= disSlabLeft)	return A[0].y;		//������ ���� ������ ����� ���
	if(D >= SlabRightDis)	return A[nQtyHDanNode].y;//������ ���� ������ ����� ���

	double ele = 0;	//�����߽� �������̼�(mm)
	for(long n=1; n<nQtyHDanNode+2; n++)//����� ���� D -> disSlabLeft < D <  SlabRightDis
	{
		if(A[n].x >= D)
		{
			double disFrNode = D - A[n-1].x;
			double DisEle = (A[n].y - A[n-1].y) * disFrNode / (A[n].x - A[n-1].x);
			ele = A[n-1].y + DisEle;
			break;
		}
	}
		
	return ele;		
}

//<summary>
//���� �����̼ǿ��� �������� �̰ݵ� �Ÿ��� ��ġ�� ������ ����� ���� ���� ����(mm)
//</summary>
//<param name="staCR">���ؼ��� �����̼�</param>
//<param name="D">���ؼ��������� �̰ݰŸ�(���� [-], ���� [+])</param>
//<return></return>
double CPlateBridgeApp::GetElevationPaveDomByDis(CPlateBasicIndex *pBx, double D) const
{
	double staCR = pBx->GetStation();
	if(D == 0)		return 0;//GetGirder(-1)->GetLine()->GetElevationPlanEl(staCR);

	double disSlabLeft = GetDisSlabLeftAct(staCR);
	double width = GetWidthAct(staCR);
	double SlabRightDis = width + disSlabLeft;
	if(disSlabLeft > D)		D = disSlabLeft;
	if(SlabRightDis < D)	D = SlabRightDis;

	CDPoint A[HDANLINESU];
	GetXyPaveUpperDom(pBx, A);

	long nQtyHDanNode = GetQtyHDanNode();
	if(D <= disSlabLeft)	return A[0].y;		//������ ���� ������ ����� ���
	if(D >= SlabRightDis)	return A[nQtyHDanNode].y;//������ ���� ������ ����� ���

	double ele = 0;	//�����߽� �������̼�(mm)
	for(long n=1; n<nQtyHDanNode+1; n++)//����� ���� D -> disSlabLeft < D <  SlabRightDis
	{
		if(A[n].x >= D)
		{
			double Dis = D - A[n-1].x;
			double DisEle = 0;
			if(A[n].x - A[n-1].x == 0)		DisEle = 0;
			else DisEle = (A[n].y - A[n-1].y) * Dis / (A[n].x - A[n-1].x);
			ele += A[n-1].y + DisEle;
			break;
		}
	}
		
	return ele;		
}

//�Է� : xy	 ���� ��� ã������ ��ǥ
//��� : xy ����Ʈ�� ��ġ�� ���� ����� ���� ���� ����(mm)
double CPlateBridgeApp::GetElevationPaveActByXY(CDPoint xy) const
{
	double sta = 0;
	m_pLine->GetStationMatchVerticalByXy(xy, sta);
	double disCen = m_pLine->GetDisMatchVerticalByXy(xy);

	return GetElevationPaveActByDis(sta, disCen);
}
//�Է� : sta	�����߽� �Ŵ��� dSta
//��� : ���� �����߽� dSta�� ���� ��� ������ ���� ���� ��ȹ��
double CPlateBridgeApp::GetPaveMaxEleAct(double sta) const
{
	CDPoint A[HDANLINESU];
	GetXyPaveAct(sta, A);

	double MaxEle = 0;
	for(long n=0; n<GetQtyHDanNode(); n++)
		MaxEle = max(MaxEle, A[n].y);
		
	return MaxEle;		
}

//�Է� : sta	�����߽� �Ŵ��� dSta
//��� : ���� �����߽� dSta�� ���� ��� ������ ���� ���� ��ȹ��
double CPlateBridgeApp::GetPaveMinEleAct(double sta) const
{
	CDPoint A[HDANLINESU];
	GetXyPaveAct(sta, A);

	double MinEle = 0;
	for(long n=0; n<GetQtyHDanNode(); n++)
		MinEle = min(MinEle, A[n].y);
		
	return MinEle;		
}

//�Է� : 
//��� : nMarkBuje ���翡 ���� Mark�� ������
CString CPlateBridgeApp::GetMarkWeld(long nMarkWeld, CPlateBasicIndex *pBx) const
{
	return "ERROR";
	//return m_pMarkStandard->GetMarkWeld(nMarkWeld, pBx);
}

//�Է� : 
//��� : nMarkBuje ���翡 ���� Mark�� ������
CString CPlateBridgeApp::GetMarkWeldDetail(long nMarkWeld, CPlateBasicIndex *pBx) const
{
	return "ERROR";
	//return m_pMarkStandard->GetMarkWeldDetail(nMarkWeld, pBx);
}

// ARoadDoc.cpp�� Serialize���� ȣ��
void CPlateBridgeApp::SetLineInfo()
{
	long	nLineNum = 0;
	for(long	nG = -1;nG < GetGirdersu();nG++)
	{
		CPlateGirderApp*	pGir = GetGirder(nG);
		nLineNum	= pGir->m_nLineNum;
		if( nG == -1 ) nLineNum = 0;

		CLineInfo*	pLine	 = ((CLineInfoApp*)m_pLine)->GetLine(nLineNum);
		if( pLine == NULL ) return;

		pGir->SetLine(pLine);
	}
}
//�Է� : T1, T2  �����Ǵ� ����� �β�
//��� : T1 �Ǵ� T2�� 16mm �ʰ��̸�, ū ��Ķ�� ������(40)�� ����
double CPlateBridgeApp::GetScallopRadius(double T1, double T2) const
{
	if(T1 > 16 || T2>16)	return m_pCRGirder->m_pSangse->m_Scallop_R2;
	else					return m_pCRGirder->m_pSangse->m_Scallop_R1;
}

//�Է� : pBx	���� �Ŵ� ���� Bx(�����߽� �Ŵ��� ���� Bx�� �ƴ�)
//��� : ���� -> Ȯ���� ��� ���
//       �����߽�(Ȯ���� ��� ����) �Ŵ� nIdx�� �������� �� �Ŵ��� Match nIdx���� �Ÿ� ����.
//         ���̾�����(������ ���� ����) ���� ����(��Ÿ�0, ���κ� ���� ����(��Ÿ�)�� ���� ������ ������ ����.
//		 BasicIndex�� ���� ��� dSta 500������ ������ ���� ����
double CPlateBridgeApp::GetWidthDiapAngCrossAng(CPlateBasicIndex *pBx) const
{
	CPlateGirderApp *pGir;
	CPlateBasicIndex *pBxMatch;
	double width = 0;
	for(long n = 0; n < GetGirdersu(); n++)
	{
		pGir = GetGirder(n);
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);
		if(n < GetGirdersu()-1 && pBxMatch == NULL)	return 0;//������ �Ŵ��� �ƴϰ� ��ġ �ε����� ������,
		if(n == 0)				 width += m_dWidthSlabLeft / pBxMatch->GetAngle().y;
		if(n == GetGirdersu()-1) width += m_dWidthSlabRight / pBxMatch->GetAngle().y;
		double dDisWeb = pGir->GetDisCRSttToCREndReal(pBxMatch);
		width += dDisWeb;
	}

	return width;
}

//<summary>
//������ ����(������ ��ܸ�-������ �ϴܸ�)
//</summary>
//<param name="*pBxGir"></param>
//<param name="xy">������ ���� ���ϴ� ��ġ</param>
//<return>������ ����</return>
double CPlateBridgeApp::GetThickFrPaveByXy(CPlateBasicIndex *pBxGir, CDPoint xy) const
{
	CDPoint vAng = CDPoint(0, 1);
	CDPoint xyResult;
	CDPoint A[HDANLINESU];

	GetXyPaveDom(pBxGir,A);

	for(long n=0; n<HDANLINESU; n++)
		if(GetXyMatchSegAndLine(A[n],A[n+1],xy,vAng,xyResult))		break;

	return fabs(xyResult.y - xy.y);
}


//��� : nMarkBuje ������ ���� ���� ��ȣ�� ����
CString CPlateBridgeApp::GetMaterialSteelSymbol(long nMarkBuje) const
{
	if(IsMaterialBolt(nMarkBuje))		return "BT";		//��Ʈ������ ����(BOLT / NUT / WASHER)	
	if(IsMaterialStudWeld(nMarkBuje))	return "WS";		//WELD STUD������ ����			
	if(IsMaterialReBar(nMarkBuje))		return "RE-BAR";	//ReBar ������ ����(ö��)
	if(IsMaterialElbow(nMarkBuje))		return "EL";		//Elbow ������ ����	
	if(IsMaterialSleeveForPipe(nMarkBuje))	return "SL";	//Sleeve For Pipe ������ ����	
	if(IsMaterialBeam(nMarkBuje))		return "BEAM";		//Beam ������ ����	

	return "PL";  //Plate
}

//��� : nMarkBuje �� WELD STUD������ ����
BOOL CPlateBridgeApp::IsMaterialStudWeld(long nMarkBuje) const
{
	switch(nMarkBuje)
	{
	case	GI_SB			:	return TRUE;	//��Ÿ����	���ܿ�����
	}
	return FALSE;
}

//��� : nMarkBuje �� ReBar ������ ����(ö��)
BOOL CPlateBridgeApp::IsMaterialReBar(long nMarkBuje) const
{
	switch(nMarkBuje)
	{
	case	GI_SA			:	return TRUE;	//��Ÿ����	SLAB ANCHOR
	}
	return FALSE;
}

//��� : nMarkBuje �� Elbow ������ ����
BOOL CPlateBridgeApp::IsMaterialElbow(long nMarkBuje) const
{
	return FALSE;
}

//��� : nMarkBuje �� Beam ������ ����
BOOL CPlateBridgeApp::IsMaterialBeam(long nMarkBuje) const
{
	switch(nMarkBuje)
	{
	case	VB_UB			:	return TRUE;	//��Ÿ����	SLAB ANCHOR
	case	VB_LB			:	return TRUE;	//��Ÿ����	SLAB ANCHOR
	case	VB_CR			:	return TRUE;	//��Ÿ����	SLAB ANCHOR
	case	HB_B			:	return TRUE;	//��Ÿ����	SLAB ANCHOR
	case	CP_STEEL		:	return TRUE;
	case	CG_STEEL		:	return TRUE;
	}

	return FALSE;
}

//��� : nMarkBuje �� Sleeve For Pipe ������ ����
BOOL CPlateBridgeApp::IsMaterialSleeveForPipe(long nMarkBuje) const
{
	return FALSE;
}


//��� : nMarkBuje ���翡 ���� ������ ����
CString CPlateBridgeApp::GetMaterial(long nMarkBuje, CPlateBasicIndex *pBx, long nRow, long nCol) const
{
	long	nMarkBujeReal = GetMarkBujeByMarkBujeAndBx(nMarkBuje, pBx);
	double	dThick        = pBx->GetGirder()->GetThickJewon(nMarkBujeReal, pBx, nRow, nCol);
	CString	sMat	= _T("");
	if(pBx && IsMaterialGirder(nMarkBuje))
		sMat = pBx->GetFactChain(nMarkBuje)->m_strSymbol;
	else
		sMat = GetMaterialByThick(nMarkBujeReal, dThick);

	return 	sMat;
}

BOOL CPlateBridgeApp::IsMaterialGirder(long nMarkBuje) const
{
	switch(nMarkBuje)
	{
	case G_F_U:
	case G_F_L:
	case G_W:
		return TRUE;
	}
	return FALSE;
}

//��� : nMarkBuje ���翡 ���� ������ ����
//	if(nTypeMatChange == "�β��� ���� �ڵ� ��ȯ")	nTypeMatChange = 0;
//	if(nTypeMatChange == "�β��� 520 ����")			nTypeMatChange = 1;  => 520B �� ����
//	if(nTypeMatChange == "�β��� ���� ��ȯ ����")	nTypeMatChange = 2;
CString CPlateBridgeApp::GetMaterialByThick(long nMarkBuje, double dThick) const
{
	CSteel        *pSteel    = m_pDataManage->GetSteel(); 
	CSteelSection *pSteelSec = m_pDataManage->GetSteelSection(); 
	CString       sMat       = pSteelSec->GetMaterialByMarkBuje(nMarkBuje);
	long nTypeMatChange = m_pDataManage->GetGlobalOption()->GetDesignSteelChangeByThick();

	if(nTypeMatChange == 1 && sMat == "SM490B")	//SM490B �� SM520B�θ� �ٲپ���
	{
		//long	nIdxMat        = pSteel->GetIndex(sMat);
		//LSH_ZZ�ӽ�(DB)
		double	dThickAllowMax = 0;//pSteel->GetThickAllowMax(nIdxMat);
		if(dThick >= dThickAllowMax)	sMat = "SM520B";
	}
	else if(nTypeMatChange == 0 && sMat.Left(3) == "SM4")
	{
		if(sMat.Left(5) == "SM490")
		{
			//long	nIdxMatA        = pSteel->GetIndex("SM490A");
			//LSH_ZZ�ӽ�(DB)
			double	dThickAllowMaxA = 0;//pSteel->GetThickAllowMax(nIdxMatA);
			//long	nIdxMatB        = pSteel->GetIndex("SM490B");
			double	dThickAllowMaxB = 0;//pSteel->GetThickAllowMax(nIdxMatB);
			if     (dThick >= dThickAllowMaxB)	sMat = "SM490C";
			else if(dThick >= dThickAllowMaxA)	sMat = "SM490B";
			else								sMat = "SM490A";
		}
		if(sMat.Left(5) == "SM400")
		{
			//long	nIdxMatA        = pSteel->GetIndex("SM400A");
			//LSH_ZZ�ӽ�(DB)
			double	dThickAllowMaxA = 0;//pSteel->GetThickAllowMax(nIdxMatA);
			//long	nIdxMatB        = pSteel->GetIndex("SM400B");
			double	dThickAllowMaxB = 0;//pSteel->GetThickAllowMax(nIdxMatB);
			if     (dThick >= dThickAllowMaxB)	sMat = "SM400C";
			else if(dThick >= dThickAllowMaxA)	sMat = "SM400B";
			else								sMat = "SM400A";
		}
	}
	return 	sMat;
}

//��� : nMarkBuje ���翡 ���� �������� ���ڸ� ���� : SM490B ���� "490"�� ����
//			nPos = -1	: SM490B ���� "SM"�� ����
//			nPos =  1	: SM490B ���� "B" �� ����
CString CPlateBridgeApp::GetMaterialNumber(long nMarkBuje, long nPos/*0*/) const
{
	CString str = m_pDataManage->GetSteelSection()->GetMaterialByMarkBuje(nMarkBuje);
	CString strNumber = "", strLeft = "", strRight = "";
	CString ch = "";

	long strlen = str.GetLength();
	for(long i = 0; i < strlen; i++)
	{
		ch = str.GetAt(i);
		if(ch == "0" || ch == "1" || ch == "2" || ch == "3" || ch == "4" ||
		   ch == "5" || ch == "6" || ch == "7" || ch == "8" || ch == "9")
		{
			strNumber += ch;				
			continue;
		}
		if(strNumber.GetLength() > 0)
		{
			if     (nPos == -1)		return strLeft;
			else if(nPos ==  1)		strRight += ch;
			else					return strNumber;
		}
		else 
			strLeft += ch;
	}

	if(nPos == -1)  return strLeft;
	else if(nPos ==  1)	return strRight;
	return strNumber;	// error
}

//��� : ���߷� �߷��� ����
double CPlateBridgeApp::GetWeightTotal(long nMarkBuje, CPlateBasicIndex *pBx, CVector vJ, long nQty)const
{
	double dWeightUnit = GetWeightUnit(nMarkBuje, pBx, vJ);
	double dWeightTotal = 0;

	if(IsMaterialBolt(nMarkBuje))				dWeightTotal = dWeightUnit*nQty;		//��Ʈ������ ����(BOLT / NUT / WASHER)	
	else if(IsMaterialStudWeld(nMarkBuje))		dWeightTotal = dWeightUnit*nQty;		//WELD STUD������ ����			
	else if(IsMaterialReBar(nMarkBuje))			dWeightTotal = dWeightUnit*nQty;		//ReBar ������ ����(ö��)
	else if(IsMaterialElbow(nMarkBuje))			dWeightTotal = dWeightUnit*nQty;		//Elbow ������ ����	
	else if(IsMaterialSleeveForPipe(nMarkBuje))	dWeightTotal = dWeightUnit*nQty;		//Sleeve For Pipe ������ ����	
	else dWeightTotal = vJ.x*vJ.y*vJ.z*dWeightUnit*nQty;									//Plate

	return RoundUp(dWeightTotal, 1);	//���ʼ������� ������ �ð�
}

//�Է� : nMarkBuje ���� �ѹ� (��ü �����߿���)
//��� : nMarkBuje ���翡 ���� ���� �߷��� ����
//		 �������� : 1. ��Ʈ&��Ʈ&�ͼ�(kg/��Ʈ)			Q*U
//					2. ���ܿ�����(kg/��)					Q*U
//					3. ���ܿ����纼Ʈ&��Ʈ&�ͼ�(kg/��Ʈ)	Q*U
//					4. Expanded Metal(kg/mm2)			W*W*Q*U
//					5. Plate(kg/mm3)					T*W*L*Q*U
//                  *���� ���� ��ȣ�� ���� �з��� �� ��.
double CPlateBridgeApp::GetWeightUnit(long nMarkBuje, CPlateBasicIndex *pBx, CVector vJ)const
{
	long nMarkBujeReal = GetMarkBujeByMarkBujeAndBx(nMarkBuje, pBx);
	if(IsMaterialBolt(nMarkBujeReal))		return GetWeightUnitBolt(nMarkBujeReal, vJ);		//��Ʈ������ ����(BOLT / NUT / WASHER)	
	if(IsMaterialStudWeld(nMarkBujeReal))	return GetWeightUnitStudWeld(nMarkBujeReal, vJ);	//WELD STUD������ ����			
	if(IsMaterialReBar(nMarkBujeReal))		return GetWeightUnitReBar(nMarkBujeReal, vJ);		//ReBar ������ ����(ö��)
	if(IsMaterialElbow(nMarkBujeReal))		return GetWeightUnitElbow(nMarkBujeReal, vJ);		//Elbow ������ ����	
	if(IsMaterialSleeveForPipe(nMarkBujeReal))	return GetWeightUnitSleeveForPipe(nMarkBujeReal, vJ);	//Sleeve For Pipe ������ ����	

	return GetWeightUnitPlate(nMarkBujeReal);  //Plate
}

double CPlateBridgeApp::GetWeightUnitKgM(long nMarkBuje, CPlateBasicIndex *pBx, CVector vJ) const
{
	double dUnitWeight = GetWeightUnit(nMarkBuje, pBx, vJ);

	long nMarkBujeReal = GetMarkBujeByMarkBujeAndBx(nMarkBuje, pBx);
	if(IsMaterialBolt(nMarkBujeReal))			return dUnitWeight;	
	if(IsMaterialStudWeld(nMarkBujeReal))		return dUnitWeight;			
	if(IsMaterialReBar(nMarkBujeReal))			return toKgPM(dUnitWeight);
	if(IsMaterialElbow(nMarkBujeReal))			return toKgPM(dUnitWeight);	
	if(IsMaterialSleeveForPipe(nMarkBujeReal))	return toKgPM(dUnitWeight);	

	return toKgPM3(dUnitWeight);
}

//��� : Stud�� ���� �߷��� ���� (Q*U = WeightTotal)kg/��
// http://www.koreabolt.co.kr/kp_b_e.html#
double CPlateBridgeApp::GetWeightUnitStudWeld(long nMarkBuje, CVector vJ)const
{
	double Dia = vJ.y;//�β�
	double Len = vJ.z;//����
	CSteelStud* pStud = m_pDataManage->GetSteelStud();
	CString str;
	str.Format("%d",(int)Len);
	
//	CSteelStudData* pStudData =  pStud->GetStudData(str);
	double	dUnitWeight	= pStud->GetStudUnitWeight(str,Dia);
	return 	dUnitWeight;//pStudData->GetStudUnitWeight(Dia);	
}

//��� : StudBolt�� ���� �߷��� ���� (Q*U = WeightTotal)
double CPlateBridgeApp::GetWeightUnitStudBolt(long nMarkBuje, CVector vJ)const
{
	return	0.8;
}
//��� : Expanded Metal�� ���� �߷��� ���� (W*W*Q*U = WeightTotal)
double CPlateBridgeApp::GetWeightUnitExpandedMetal(long nMarkBuje, CVector vJ)const
{
	return	1.1/(270*270*16);
}
//��� : ReBar�� ���� �߷��� ���� (W*W*Q*U = WeightTotal)
double CPlateBridgeApp::GetWeightUnitReBar(long nMarkBuje, CVector vJ)const
{
	switch((long)vJ.y)
	{
		case 10:	return 0.560/1000; // (tf/m)
		case 13:	return 0.995/1000;
		case 16:	return 1.560/1000;
		case 19:	return 2.250/1000;
		case 22:	return 3.040/1000;
		case 25:	return 3.980/1000;
		case 29:	return 5.040/1000;
		case 32:	return 6.230/1000;
		case 35:	return 7.510/1000;
	}
	return	0;
}
//��� : Elbow�� ���� �߷��� ���� (W*W*Q*U = WeightTotal)
double CPlateBridgeApp::GetWeightUnitElbow(long nMarkBuje, CVector vJ)const
{
	return	0;
}

//��� : SleeveForPipe�� ���� �߷��� ���� (W*W*Q*U = WeightTotal)
double CPlateBridgeApp::GetWeightUnitSleeveForPipe(long nMarkBuje, CVector vJ)const
{
	return	0;
}
//��� : HBeam�� ���� �߷��� ���� (W*W*Q*U = WeightTotal)
double CPlateBridgeApp::GetWeightUnitHBeam(long nMarkBuje, CVector vJ)const
{
	return	0;//����
}
 

long CPlateBridgeApp::GetIndexBaseScallop(long nMarkBuje1st, long nMarkBuje2nd, long nMarkBuje3rd, CPlateBasicIndex *pBx)
{
	CSteelScallop* pSteelScallop = m_pDataManage->GetSteelScallop();

	long nAdd	 = 2; // 070207 ���ڷ� 2�� �׸� �߰�...KB....
	long nCRO	 =  2 + nAdd;
	long nSTR	 =  20 + nAdd;
	long nBRA	 =  27 + nAdd;
	long nBRBEAM =  41 + nAdd;
	long nGITA	 =  45 + nAdd;

	long n1 = GetMarkBujeByMarkBujeAndBx(nMarkBuje1st,pBx);
	long n2 = GetMarkBujeByMarkBujeAndBx(nMarkBuje2nd,pBx);
	long n3 = GetMarkBujeByMarkBujeAndBx(nMarkBuje3rd,pBx);

	long nIdxScallop = -1;
	//������� ���̷� ����...
	if (n3 == G_W)
	{
		if (n1 == G_F_U && n2 == G_F_U) nIdxScallop = 0;
		if (n1 == G_F_L && n2 == G_F_L) nIdxScallop = 1;
	}
	// �Ϲݺ� ����������
	if (n2 == G_W && n3==V_ST)
	{
		if (n1 == G_F_U)	nIdxScallop = 0+nAdd;
		if (n1 == G_F_L)	nIdxScallop = 1+nAdd;
	}
	// ���κ�
	if(n1==CP_BR_W)
	{
		if(n3==G_F_U)		nIdxScallop =  nCRO+0;
		if(n3==G_F_L)		nIdxScallop =  nCRO+1;
		if(n3==CP_BR_F_L)	nIdxScallop =  nCRO+2;
	}
	if(n1==CP_BR_GA)
	{
		if(n3==CP_BR_F_L)	nIdxScallop =  nCRO+3;
		if(n3==G_F_L)		nIdxScallop =  nCRO+4;
	}
	if(n1==CP_V)
	{
		if(n3==CP_F_U)		nIdxScallop =  nCRO+5;
		if(n3==CP_F_L)		nIdxScallop =  nCRO+6;
	}
	if(n1==CP_BR_F_U && n2==V_ST && n3==G_W)		nIdxScallop =  nCRO+7;
	if(n1==CP_BR_F_L && n2==V_ST && n3==G_W)		nIdxScallop =  nCRO+8;
	//
	if(n1==CG_BR_W)
	{
		if(n3==G_F_U)		nIdxScallop =  nCRO+9;
		if(n3==G_F_L)		nIdxScallop =  nCRO+10;
		if(n3==CG_BR_F_L)	nIdxScallop =  nCRO+11;
	}
	if(n1==CG_BR_GA)
	{
		if(n3==CG_BR_F_L)	nIdxScallop =  nCRO+12;
		if(n3==G_F_L)		nIdxScallop =  nCRO+13;
	}
	if(n1==CG_V)
	{
		if(n3==CG_F_U)		nIdxScallop =  nCRO+14;
		if(n3==CG_F_L)		nIdxScallop =  nCRO+15;
	}
	if(n1==CG_BR_F_U && n2==V_ST && n3==G_W)		nIdxScallop =  nCRO+16;
	if(n1==CG_BR_F_L && n2==V_ST && n3==G_W)		nIdxScallop =  nCRO+17;

	// ���κ�
	if (n1 == ST_BR_W)
	{
		if (n3 == CP_F_U || n3 == CG_F_U)	nIdxScallop = nSTR + 0;
		if (n3 == CP_F_L || n3 == CG_F_L)	nIdxScallop = nSTR + 1;
		if (n3 == ST_BR_F_L)	nIdxScallop = nSTR + 2;
	}
	if (n1 == ST_BR_GA)
	{
		if (n3 == ST_BR_F_L)	nIdxScallop = nSTR + 3;
		if (n3 == CP_F_L || n3 == CG_F_L)	nIdxScallop = nSTR + 4;
	}
	if (n1 == ST_V)
	{
		if (n3 == ST_F_U)	nIdxScallop = nSTR + 5;
		if (n3 == ST_F_L)	nIdxScallop = nSTR + 6;
	}

	//���� ���κ� ������
	if(n1==WI_P_BR_W)
	{
		if(n3==G_F_U)		nIdxScallop =  nBRA+0;
		if(n3==G_F_L)		nIdxScallop =  nBRA+1;
		if(n3==WI_P_BR_F_L)	nIdxScallop =  nBRA+2;
	}
	if(n1==WI_P_BR_GA)
	{
		if(n3==WI_P_BR_F_L)	nIdxScallop =  nBRA+3;
		if(n3==G_F_L)		nIdxScallop =  nBRA+4;
	}
	if(n1==WI_P_V)
	{
		if(n3==WI_P_F_U)	nIdxScallop =  nBRA+5;
		if(n3==WI_P_F_L)	nIdxScallop =  nBRA+6;
	}
	//���� ���κ� �Ϲݺ�
	if(n1==WI_G_BR_W)
	{
		if(n3==G_F_U)		nIdxScallop =  nBRA+0;
		if(n3==G_F_L)		nIdxScallop =  nBRA+1;
		if(n3==WI_G_BR_F_L)	nIdxScallop =  nBRA+2;
	}
	if(n1==WI_G_BR_GA)
	{
		if(n3==WI_G_BR_F_L)	nIdxScallop =  nBRA+3;
		if(n3==G_F_L)		nIdxScallop =  nBRA+4;
	}
	if(n1==WI_G_V)
	{
		if(n3==WI_G_F_U)	nIdxScallop =  nBRA+5;
		if(n3==WI_G_F_L)	nIdxScallop =  nBRA+6;
	}
	//���� �� 
	if(n1==EN_V)
	{
		if(n3==EN_F_U)	nIdxScallop =  nBRBEAM+0;
		if(n3==EN_F_L)	nIdxScallop =  nBRBEAM+1;
	}

	//��Ÿ
	if(n1==GI_SB)			nIdxScallop =  nGITA+0;
	if(n1==GI_SA)			nIdxScallop =  nGITA+1;
	if(n1==GI_SO)			nIdxScallop =  nGITA+2;
	if(n1==GI_LP)			nIdxScallop =  nGITA+3;
	if(n1==GI_JA_L || n1==GI_JA_R)
	{
		if(n3==G_F_U)		nIdxScallop =  nGITA+4;
		if(n3==G_F_L)		nIdxScallop =  nGITA+5;
	}
	if(n1==GI_JA_SA)		nIdxScallop =  nGITA+6;
	if(n1==GI_ZST)			nIdxScallop =  nGITA+7;
	if(n1==GI_HG)			nIdxScallop =  nGITA+8;


	if(nIdxScallop == -1) return -1;

	return pSteelScallop->GetBaseScallopByIndex(nIdxScallop)->m_nCutType;

}

long CPlateBridgeApp::GetIndexBaseWeld(long nMarkBujeFir, long nMarkBujeSec, CPlateBasicIndex *pBx)
{
	long nFir = nMarkBujeFir;
	long nSec = nMarkBujeSec;
	if(pBx)
	{
		nFir = GetMarkBujeByMarkBujeAndBx(nMarkBujeFir,pBx);
		nSec = GetMarkBujeByMarkBujeAndBx(nMarkBujeSec,pBx);
	}
	long nCRO = 16;
	long nSTR = 56;
	long nBRA = 73;
	long nBRBEAM = 109;
	long nBracing= 116;
	long nGita   = 122;
	switch(nFir) 
	{
		// �Ϲ� ////////////////////////
		case G_F_U:					return 0;
		case G_F_L:					return 1;
		case WELD_FIELDU:			return 2;
		case WELD_FIELDL:			return 3;
		case WELD_FIELDWEB:			return 5;
		case G_W:		
			if(nSec==nFir)			return 4;
			if(nSec==G_F_U)			return 6;
			if(nSec==G_F_L)			return 7;
			ASSERT(TRUE);
		case G_H_L:
		case G_H_R:			
			return 15;
		case V_J_ST:
			if(nSec==G_F_U)			return 9;
			if(nSec==G_F_L)			return 10;
			if(nSec==G_W)			return 11;
			ASSERT(TRUE);
		case V_ST:
			if(nSec==G_F_U)			return 12;
			if(nSec==G_F_L)			return 13;
			if(nSec==G_W)			return 14;
			ASSERT(TRUE);
		// ���κ� //////////////////////////
		case CP_F_U:
			if(nSec==CP_W)		return  nCRO;
			ASSERT(TRUE);
		case CP_F_L:
			if(nSec==CP_W)		return  nCRO+1;
			ASSERT(TRUE);
		case CP_BR_W:
			if(nSec==G_F_U)		return  nCRO+2;
			if(nSec==G_F_L)		return  nCRO+3;
			if(nSec==G_W)		return  nCRO+4;
			ASSERT(TRUE);
		case CP_BR_F_U:
			if(nSec==G_F_U)		return  nCRO+5;
			if(nSec==CP_BR_W)	return  nCRO+6;
			if(nSec==V_ST)		return  nCRO+7;
			ASSERT(TRUE);
		case CP_BR_F_L:
			if(nSec==G_F_L)		return  nCRO+8;
			if(nSec==G_W)		return  nCRO+9;
			if(nSec==CP_BR_W)	return  nCRO+10;
			if(nSec==V_ST)		return  nCRO+11;
			ASSERT(TRUE);
		case CP_BR_GA:
			if(nSec==G_W)		return  nCRO+12;
			if(nSec==G_F_L)		return  nCRO+13;
			if(nSec==CP_BR_F_L)	return  nCRO+14;
			ASSERT(TRUE);
		case CP_BR_GA_FA:
			if(nSec==CP_BR_GA)	return  nCRO+15;
			ASSERT(TRUE);
		case CP_V:
			if(nSec==CP_W)		return  nCRO+16;
			if(nSec==CP_F_U)	return  nCRO+17;
			if(nSec==CP_F_L)	return  nCRO+18;
			ASSERT(TRUE);
		case CP_H:
			if(nSec==CP_W)		return  nCRO+19;
			ASSERT(TRUE);
		//
		case CG_F_U:
			if(nSec==CG_W)		return  nCRO+20;
			ASSERT(TRUE);
		case CG_F_L:
			if(nSec==CG_W)		return  nCRO+21;
			ASSERT(TRUE);
		case CG_BR_W:
			if(nSec==G_F_U)		return  nCRO+22;
			if(nSec==G_F_L)		return  nCRO+23;
			if(nSec==G_W)		return  nCRO+24;
			ASSERT(TRUE);
		case CG_BR_F_U:
			if(nSec==G_F_U)		return  nCRO+25;
			if(nSec==CG_BR_W)	return  nCRO+26;
			if(nSec==V_ST)		return  nCRO+27;
			ASSERT(TRUE);
		case CG_BR_F_L:
			if(nSec==G_F_L)		return  nCRO+28;
			if(nSec==G_W)		return  nCRO+29;
			if(nSec==CG_BR_W)	return  nCRO+30;
			if(nSec==V_ST)		return  nCRO+31;
			ASSERT(TRUE);
		case CG_BR_GA:
			if(nSec==G_W)		return  nCRO+32;
			if(nSec==G_F_L)		return  nCRO+33;
			if(nSec==CG_BR_F_L)	return  nCRO+34;
			ASSERT(TRUE);
		case CG_BR_GA_FA:
			if(nSec==CG_BR_GA)	return  nCRO+35;
			ASSERT(TRUE);
		case CG_V:
			if(nSec==CG_W)		return  nCRO+36;
			if(nSec==CG_F_U)	return  nCRO+37;
			if(nSec==CG_F_L)	return  nCRO+38;
			ASSERT(TRUE);
		case CG_H:
			if(nSec==CG_W)		return  nCRO+39;
			ASSERT(TRUE);
		// ���κ� ////////////////////////
		case ST_F_U:
			if(nSec==ST_W)		return  nSTR+0;
			ASSERT(TRUE);
		case ST_F_L:
			if(nSec==ST_W)		return  nSTR+1;
			ASSERT(TRUE);
		case ST_BR_W:
			if(nSec==CP_W   || nSec==CG_W)		return  nSTR+2;
			if(nSec==CP_F_U || nSec==CG_F_U)	return  nSTR+3;
			if(nSec==CP_F_L || nSec==CG_F_L)	return  nSTR+4;
			ASSERT(TRUE);
		case ST_BR_F_U:
			if(nSec==CP_F_U || nSec==CG_F_U)	return  nSTR+5;
			if(nSec==ST_BR_W)	return  nSTR+6;
			ASSERT(TRUE);
		case ST_BR_F_L:
			if(nSec==CP_BR_F_L || nSec==CG_BR_F_L)	return  nSTR+7;
			if(nSec==ST_BR_W)	return  nSTR+8;
			ASSERT(TRUE);
		case ST_BR_GA:
			if(nSec==CP_BR_W || nSec==CG_BR_W)	return  nSTR+9;
			if(nSec==CP_F_L  || nSec==CG_F_L)	return  nSTR+10;
			if(nSec==ST_BR_F_L)					return  nSTR+11;
			if(nSec==ST_BR_GA_FA)				return  nSTR+12;
			ASSERT(TRUE);
		case ST_BR_GA_FA:
			if(nSec==ST_BR_GA)	return  nSTR+12;
			ASSERT(TRUE);
		case ST_V:
			if(nSec==ST_W)		return  nSTR+13;
			if(nSec==ST_F_U)	return  nSTR+14;
			if(nSec==ST_F_L)	return  nSTR+15;
			ASSERT(TRUE);
		case ST_H:
			if(nSec==ST_W)		return  nSTR+16;
			ASSERT(TRUE);
		// �������κ� /////////////////////
		case WI_P_F_U:
			if(nSec==WI_P_W)				return  nBRA;
			ASSERT(TRUE);
		case WI_P_F_L:
			if(nSec==WI_P_W)				return  nBRA+1;
			ASSERT(TRUE);
		case WI_P_BR_W:
			if(nSec==G_F_U)					return  nBRA+2;
			if(nSec==G_F_L)					return  nBRA+3;
			if(nSec==G_W)					return  nBRA+4;
			ASSERT(TRUE);
		case WI_P_BR_F_U:
			if(nSec==G_F_U)					return  nBRA+5;
			if(nSec==WI_P_BR_W)				return  nBRA+6;
			ASSERT(TRUE);
		case WI_P_BR_F_L:
			if(nSec==G_F_L)					return  nBRA+7;
			if(nSec==G_W)					return  nBRA+8;
			if(nSec==WI_P_BR_W)				return  nBRA+9;
			ASSERT(TRUE);
		case WI_P_BR_GA:
			if(nSec==G_W)					return  nBRA+10;
			if(nSec==G_F_L)					return  nBRA+11;
			if(nSec==WI_P_BR_F_L)			return  nBRA+12;
			ASSERT(TRUE);
		case WI_P_BR_GA_FA:
			if(nSec==WI_P_BR_GA)			return  nBRA+13;
			ASSERT(TRUE);
		case WI_P_V:
			if(nSec==WI_P_W)				return  nBRA+14;
			if(nSec==WI_P_F_U)				return  nBRA+15;
			if(nSec==WI_P_F_L)				return  nBRA+16;
			ASSERT(TRUE);
		case WI_P_H:
			if(nSec==WI_P_W)				return  nBRA+17;
			ASSERT(TRUE);
		//
		case WI_G_F_U:
			if(nSec==WI_G_W)				return  nBRA+18;
			ASSERT(TRUE);
		case WI_G_F_L:
			if(nSec==WI_G_W)				return  nBRA+19;
			ASSERT(TRUE);
		case WI_G_BR_W:
			if(nSec==G_F_U)					return  nBRA+20;
			if(nSec==G_F_L)					return  nBRA+21;
			if(nSec==G_W)					return  nBRA+22;
			ASSERT(TRUE);
		case WI_G_BR_F_U:
			if(nSec==G_F_U)					return  nBRA+23;
			if(nSec==WI_G_BR_W)				return  nBRA+24;
			ASSERT(TRUE);
		case WI_G_BR_F_L:
			if(nSec==G_F_L)					return  nBRA+25;
			if(nSec==G_W)					return  nBRA+26;
			if(nSec==WI_G_BR_W)				return  nBRA+27;
			ASSERT(TRUE);
		case WI_G_BR_GA:
			if(nSec==G_W)					return  nBRA+28;
			if(nSec==G_F_L)					return  nBRA+29;
			if(nSec==WI_G_BR_F_L)			return  nBRA+30;
			ASSERT(TRUE);
		case WI_G_BR_GA_FA:
			if(nSec==WI_G_BR_GA)			return  nBRA+31;
			ASSERT(TRUE);
		case WI_G_V:
			if(nSec==WI_G_W)				return  nBRA+32;
			if(nSec==WI_G_F_U)				return  nBRA+33;
			if(nSec==WI_G_F_L)				return  nBRA+34;
			ASSERT(TRUE);
		case WI_G_H:
			if(nSec==WI_G_W)				return  nBRA+35;
			ASSERT(TRUE);
		// ���� ��  ////////////////////////
		case EN_F_U:
			if (nSec==EN_W)			return  nBRBEAM+0;
		case EN_F_L:
			if (nSec==EN_W)			return  nBRBEAM+1;
		case EN_V:
			if (nSec==EN_F_U)		return  nBRBEAM+2;
			if (nSec==EN_F_L)		return  nBRBEAM+3;
			if (nSec==EN_W)			return  nBRBEAM+4;
		case EN_H:
			if (nSec==EN_W)			return  nBRBEAM+5;
		case EN_BR_F_U:
			if (nSec==EN_BR_W)		return  nBRBEAM+6;
		case EN_BR_F_L:
			if (nSec==EN_BR_W)		return  nBRBEAM+7;
		case EN_BR_W:
			{
				if (nSec==EN_BR_F_U)	return  nBRBEAM+6;
				if (nSec==EN_BR_F_L)	return  nBRBEAM+7;
			}
		// �����극�̽�
		case VB_GU:
			{
				if(nSec==V_ST)			return nBracing+0;
				if(nSec==VB_UB)			return nBracing+1;
				if(nSec==VB_LB)			return nBracing+1;
				if(nSec==VB_CR)			return nBracing+1;
			}
		// ����극�̽�
		case HB_GU:
			{
				if(nSec==G_W)			return nBracing+2;
				if(nSec==CP_BR_W)		return nBracing+3;
				if(nSec==CG_BR_W)		return nBracing+3;
				if(nSec==HB_B)			return nBracing+4;
			}
		// ��Ÿ���� ////////////////////////		
		case GI_SB:				return  nGita+0;
		case GI_SQ:				return  nGita+1;
		case GI_SA:				return  nGita+2;
		case GI_SO:				return  nGita+3;
		case GI_LP:				return  nGita+5;
		case GI_JA_L:
			{
				if(nSec==G_F_U)		return  nGita+6;
				if(nSec==G_F_L)		return  nGita+7;
				if(nSec==G_W)		return  nGita+8;
				ASSERT(TRUE);
			}
		case GI_JA_R:
			{
				if(nSec==G_F_U)		return  nGita+6;
				if(nSec==G_F_L)		return  nGita+7;
				if(nSec==G_W) 		return  nGita+8;
				ASSERT(TRUE);
			}
		case GI_JA_SA:				return nGita+9;
		case GI_ZST:				return nGita+10;
		case GI_HG:					return nGita+11;
	}

	return -1;
}

BOOL CPlateBridgeApp::IsBolt(long nMarkBuje)
{
	if(IsMaterialBolt(nMarkBuje))			return TRUE;	

	return FALSE;
}

BOOL CPlateBridgeApp::IsPlate(long nMarkBuje)
{
	if(IsMaterialBolt(nMarkBuje))			return FALSE;	
	if(IsMaterialStudWeld(nMarkBuje))		return FALSE;			
	if(IsMaterialReBar(nMarkBuje))			return FALSE;
	if(IsMaterialElbow(nMarkBuje))			return FALSE;	
	//if(IsMaterialBeam(nMarkBuje))			return FALSE;	
	if(IsMaterialSleeveForPipe(nMarkBuje))	return FALSE;	

	return TRUE;
}

long CPlateBridgeApp::GetNumberStartGirder() const
{
	CGlobarOption *pGlOpt = m_pDataManage->GetGlobalOption();
	
	return pGlOpt->GetDesignStartNumberOfGirder();
}
long CPlateBridgeApp::GetNumberStartSplice() const
{
	CGlobarOption *pGlOpt = m_pDataManage->GetGlobalOption();
	
	return pGlOpt->GetDesignStartNumberOfSplice();
}
 
//<summary>
//double Sta, long nDecimalPoint = 3, BOOL bMarkSTA=TRUE
//</summary>
//<param name="Sta"></param>
//<param name="nDecimalPoint">�⺻�� 3</param>
//<param name="bMarkSTA">�⺻�� TRUE</param>
//<return></return>
CString CPlateBridgeApp::GetStationForStringOutDB(double Sta, long nDecimalPoint, BOOL bMarkSTA) const
{
	return GetStationForStringOut(Sta, nDecimalPoint, bMarkSTA);
}

//<summary>
//�� ��� �������� ������.(0~180��)
//�� ���� ���� : 0=������ ����, 1=������ ����, 2=������ ����, 3=������
//</summary>
//<param name="nRow">�Ŵ���ȣ�� �������� ����</param>
//<param name="nCol">������ȣ�� ������. A1=0, P1=1...</param>
//<return></return>
CDPoint CPlateBridgeApp::GetAngleShoeOnGirCenter(long nRow, long nCol, BOOL bJijumStiff/*= FALSE*/)const
{
	CPlateGirderApp *pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));
	double sta = GetStationShoeOnGirCenter(nRow, nCol);
	CDPoint vAng = CDPoint(0,1);
	CDPoint vAngBang = pGir->GetLine()->GetAngleAzimuth(sta);
	long nColFixShoe = GetNumberFixShoeCol(nRow, nCol);
	long nDirection = bJijumStiff ? m_cDirVStiffJijum : m_cDirShoe;

	if(nColFixShoe < 0)			//�������� ���� ���
	{				
		return CDPoint(0,1);	//90�� ����
	}
	else if(nDirection == 0)			//������ ���� ���
	{	
		return CDPoint(0,1);	//90�� ����
	
/*		long nRowFixShoe = GetNumberFixShoeRow(nRow, nCol);

		if(nCol != nColFixShoe)		//���� �����ΰ� �ƴ� ���
			return CDPoint(0,1);	//90�� ����
		else if(nRow==nRowFixShoe)
			return CDPoint(0,1);	//90�� ����

		CDPoint xyShoe = GetXyShoe(nRow, nCol);
		CDPoint xyShoeFix = GetXyShoe(nRowFixShoe, nColFixShoe);
		vAng = (xyShoeFix - xyShoe).Unit();	//���⼺(xyShoe-xyShoeFix)�� ������ ������ ��.
		if(nRow < nRowFixShoe) vAng = -vAng;
		vAng = vAngBang.BetweenAngle(vAng);
*/
	}
	else if(nDirection == 1)		//�� ���������� ����
	{				
		double dStaJijum = pGir->GetStationOnJijum(nCol);
		CDPoint vAngJijum = pGir->GetAngleOnJijum(nCol);
		double dStaShoe = GetStationShoeOnGirCenter(nRow, nCol);
		vAng = pGir->GetLine()->GetAngleByOffset(dStaJijum, dStaShoe, vAngJijum);
	}
	else if(nDirection == 2)				//������ ����
	{
		long nRowFixShoe = GetNumberFixShoeRow(nRow, nCol);
		if(nCol == nColFixShoe)		//���� �������� ���
		{
			double dStaJijum = pGir->GetStationOnJijum(nCol);
			CDPoint vAngJijum = pGir->GetAngleOnJijum(nCol);
			double dStaShoe = GetStationShoeOnGirCenter(nRow, nCol);
			vAng = pGir->GetLine()->GetAngleByOffset(dStaJijum, dStaShoe, vAngJijum);
		}else{
			CDPoint xyShoe = GetXyShoe(nRow, nCol);
			CDPoint xyShoeFix = GetXyShoe(nRowFixShoe, nColFixShoe);
			vAng = (xyShoeFix - xyShoe).Unit();	//���⼺(xyShoe-xyShoeFix)�� ������ ������ ��.
			if(nCol > nColFixShoe) vAng = -vAng;
			vAng = vAngBang.BetweenAngle(vAng);
			vAng = vAng.Rotate90();
		}
	}
	else if(nDirection == 3)				//�� ����
	{
		long nRowFixShoe = GetNumberFixShoeByHyunRow(nRow, nCol);
		if(nCol == nColFixShoe)		//���� �������� ���
		{
			double dStaJijum = pGir->GetStationOnJijum(nCol);
			CDPoint vAngJijum = pGir->GetAngleOnJijum(nCol);
			double dStaShoe = GetStationShoeOnGirCenter(nRow, nCol);
			vAng = pGir->GetLine()->GetAngleByOffset(dStaJijum, dStaShoe, vAngJijum);
		}else{
			CDPoint xyShoe = GetXyShoe(nRow, nCol);
			CDPoint xyShoeFix = GetXyShoe(nRowFixShoe, nColFixShoe);
			vAng = (xyShoeFix - xyShoe).Unit();	//���⼺(xyShoe-xyShoeFix)�� ������ ������ ��.
			if(nCol > nColFixShoe) vAng = -vAng;
			vAng = vAngBang.BetweenAngle(vAng);
			vAng = vAng.Rotate90();
		}
	}

	return vAng;
}

//<summary>
//���� ��(nRow, nCol)�� ���� ������(�������� 2���̻��� ��� ���� ����� ������) ����
//-1 �̸� �������� ����.
//</summary>
//<param name="nRow">�Ŵ���ȣ�� �������� ����</param>
//<param name="nCol">������ȣ�� ������. A1=0, P1=1...</param>
//<return></return>
long CPlateBridgeApp::GetNumberFixShoeRow(long nRow, long nCol)const
{
	long nRowResult = -1;
	double dDisResult = 0;
	double dDisMin = 0;
	CDPoint xyShoe = GetXyShoe(nRow, nCol);
	CDPoint xyShoeFix;
	BOOL	bFirstShoeFix = TRUE;
	for (long nPier = 0; nPier <= m_nQtyJigan; nPier++)
	{
		long nRows = GetQtyShoeOnJijum(nPier);
		for (long nRowCur = 0; nRowCur < nRows; nRowCur++)
		{
			if(GetShoeByRowCol(nRowCur, nPier)->GetDirectionShoe() == 0)
			{
				xyShoeFix = GetXyShoe(nRowCur, nPier);
				dDisResult = ~(xyShoe-xyShoeFix);
				if(bFirstShoeFix == TRUE)	dDisMin = dDisResult;
				if(dDisResult <= dDisMin)
				{
					bFirstShoeFix = FALSE;
					nRowResult = nRowCur;
					dDisMin = dDisResult;
				}
			}
		}
	}

	return nRowResult;
}

//<summary>
//���� ��(nRow, nCol)�� ���� ������(�������� 2���̻��� ��� ���� ����� ������) ����
//-1 �̸� �������� ����.
//</summary>
//<param name="nRow">�Ŵ���ȣ�� �������� ����</param>
//<param name="nCol">������ȣ�� ������. A1=0, P1=1...</param>
//<return></return>
long CPlateBridgeApp::GetNumberFixShoeCol(long nRow, long nCol)const
{
	long nRowResult = -1;
	double dDisResult = 0;
	double dDisMin = 0;
	CDPoint xyShoe = GetXyShoe(nRow, nCol);
	CDPoint xyShoeFix;
	BOOL	bFirstShoeFix = TRUE;
	for (long nPier = 0; nPier <= m_nQtyJigan; nPier++)
	{
		long nRows = GetQtyShoeOnJijum(nPier);
		for (long nRowCur = 0; nRowCur < nRows; nRowCur++)
		{
			if(GetShoeByRowCol(nRowCur, nPier)->GetDirectionShoe() == 0)
			{
				xyShoeFix = GetXyShoe(nRowCur, nPier);
				dDisResult = ~(xyShoe-xyShoeFix);
				if(bFirstShoeFix == TRUE)	dDisMin = dDisResult;
				if(dDisResult <= dDisMin)
				{
					bFirstShoeFix = FALSE;
					nRowResult = nPier;
					dDisMin = dDisResult;
				}
			}
		}
	}

	return nRowResult;
}

//<summary>
//�� ������ ��� ���� ��(nRow, nCol)�� ���� ������(�������� 2���̻��� ��� ���� ����� ������) ����
//���� ���� ���Ͽ� ������ ������ �ִ� ��� ������ ���� ����� ��(�������� �ƴ� ���� ����)
//-1 �̸� �������� ����.
//</summary>
//<param name="nRow">�Ŵ���ȣ�� �������� ����</param>
//<param name="nCol">������ȣ�� ������. A1=0, P1=1...</param>
//<return></return>
long CPlateBridgeApp::GetNumberFixShoeByHyunRow(long nRow, long nCol)const
{
	long nG = GetNumberGirderByShoe(nRow, nCol);
	long nColFix = GetNumberFixShoeCol(nRow, nCol);
	if(nColFix < 0)	return -1;
	long nRows = GetQtyShoeOnJijum(nColFix);
	long nRowResult = -1;
	double dDisResult = 0;
	double dDisMin = 0;
	CDPoint xyShoe = GetXyShoe(nRow, nCol);
	CDPoint xyShoeFix;
	BOOL	bFirstShoeFix = TRUE;
	for (long nRowCur = 0; nRowCur < nRows; nRowCur++)
	{
		long nGCur = GetNumberGirderByShoe(nRowCur, nColFix);
		if(nG != nGCur)	continue;				//���� �Ŵ��� �ƴϸ� ������ ��ġ�� �ƴ�
		xyShoeFix = GetXyShoe(nRowCur, nColFix);
		dDisResult = ~(xyShoe-xyShoeFix);
		if(bFirstShoeFix == TRUE)	dDisMin = dDisResult;
		if(dDisResult <= dDisMin)
		{
			bFirstShoeFix = FALSE;
			nRowResult = nRowCur;
			dDisMin = dDisResult;
		}
	}

	return nRowResult;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���� ����(�Ŵ������� �ƴ�)�� �����̼��� ����. Ȯ���� ��� �Ŵ������� ���� ������ �ٸ� �� ����.
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetStationCenLineForElByShoe(long nRow, long nCol)const
{
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));

	double	sta		= pGir->GetStationOnJijum(nCol);
	CDPoint vAng    = pGir->GetAngleOnJijum(nCol);
	double  disShoe = GetDisShoeOnGirderCenter(nRow,nCol); // �Ŵ� �߽ɿ��� �̰� �Ÿ�

	double staCR = pGir->GetStationCenLineForEl(sta, disShoe, vAng);
	return staCR;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�Ŵ�����(���� ������ �ƴ�)�� �����̼��� ����. Ȯ���� ��� �Ŵ������� ���� ������ �ٸ� �� ����.
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetStationShoeOnGirCenter(long nRow, long nCol)const
{
 	CDPoint xy = GetXyShoe(nRow, nCol);
	long nG = GetNumberGirderByShoe(nRow, nCol);
	double sta = 0;
	GetGirder(nG)->GetLine()->GetStationMatchVerticalByXy(xy, sta);
  	return sta;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�Է� : 
///		 nMarkBuje  0= ����, 1=����, 2=�� ������, 3=�� ������
///��� : �ش� ���� ���� ��.��.�¿� ���������� �β��� ����
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetThickFlangeByShoe(long nRow, long nCol, long nMarkBuje)const
{
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));
	CPlateBasicIndex *pBx = pGir->GetBxByStation(pGir->GetStationOnJijum(nCol));
	return pGir->GetThickJewon(nMarkBuje, pBx, 0, 0, DIM_BASIC);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// �������� ���� (����)
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetHeightGirderByShoe(long nRow, long nCol, BOOL bReal)const
{
	long nG = GetNumberGirderByShoe(nRow, nCol);
	CPlateGirderApp *pGir = GetGirder(nG);

	double dStaCen   = pGir->GetStationOnJijum(nCol);	  // �Ŵ��߽��� Station (o)
	double dDis      = GetDisShoeOnGirderCenter(nRow, nCol);
	CDPoint vAngle   = pGir->GetAngleOnJijum(nCol);

	return pGir->GetHeightGirderByStaAng(dStaCen, dDis, vAngle, bReal);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///��� : ���� ������ �������� �β�
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetThickSlabAndPaveByShoe(long nRow, long nCol)const
{
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));
	CPlateBasicIndex *pBx = pGir->GetBxOnJijum(nCol);
//	double	sta		= pGir->GetStationOnJijum(nCol);
//	CDPoint vAng    = pGir->GetAngleOnJijum(nCol);
//	double  disShoe = GetDisShoeOnGirderCenter(nRow,nCol); // �Ŵ� �߽ɿ��� �̰� �Ÿ�

//	double dThickSlabAndPave = pGir->GetThickSlabPave(sta, disShoe, vAng);

	return m_dThickPave + pGir->GetThickSlab(pBx, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���� ����(�Ŵ������� �ƴ�)�� ��ȹ�� ����
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetElevationPlanElOnCenLineByShoe(long nRow, long nCol)const
{
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));
	double	sta		= pGir->GetStationOnJijum(nCol);
	CDPoint vAng    = pGir->GetAngleOnJijum(nCol);
	double  disShoe = GetDisShoeOnGirderCenter(nRow,nCol); // �Ŵ� �߽ɿ��� �̰� �Ÿ�

	double ele = pGir->GetElevationPlanElOnCenLineAct(sta, disShoe, vAng);

	return ele;
}
///////////////////////////////////////////////////////////////////////////////////////////////
///��� : �� ���(���÷���Ʈ �ϴ�) �������̼�
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetElevationOnShoeTop(long nRow, long nCol) const
{
	CGlobarOption		*pGlopt	= m_pDataManage->GetGlobalOption();
	CPlateGirderApp		*pGir	= GetGirder(GetNumberGirderByShoe(nRow, nCol));
	CPlateBasicIndex	*pBx	= pGir->GetBxOnJijum(nCol);

	double	dGirEL	= pGir->GetElevationGirderForReal(pBx, nRow, nCol);
	double	dHw		=  pGir->GetHeightGirder(pBx, TRUE)
				 + (m_BindConc.m_dHeight - m_BindConc.m_dDis)
				 + (pGlopt->GetDesignHeigtBaseUpper()==0 ? GetThickFlangeByShoe(nRow, nCol, G_F_U) : 0)
				 + GetThickFlangeByShoe(nRow, nCol, G_F_L);
	double	dShoeT	= GetShoeByRowCol(nRow,nCol)->GetThickSolePlate();
	double	dElev	= dGirEL-dHw-dShoeT;

	return dElev;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///��� : �� �ϴ�(Mortar ���) �������̼�
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetElevationOnMortarTop(long nRow, long nCol)const
{
	return GetElevationOnShoeTop(nRow,nCol) - GetShoeByRowCol(nRow,nCol)->GetThickShoe();
}


///////////////////////////////////////////////////////////////////////////////////////////////
///���¸�(Block �ϴ�) �������̼�
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetElevationOnBridgeSeatTop(long nRow, long nCol)const
{
	CShoe *pShoe = GetShoeByRowCol(nRow, nCol);
	return GetElevationOnMortarTop(nRow,nCol) - pShoe->GetThickMortar() - pShoe->GetThickBlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���ؼ��� �߽ɿ��� ���� ��α����� ������(���翡 ����)
///	 ���� ��ΰ� �����߽ɺ��� ������[+]
///   Ȯ���̰� ���������� ��쿡�� (���ؼ��� Ele - ���� ���� Ele) �� ����
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetThickPyungubae(long nRow, long nCol)const
{	
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));
	double	sta		= pGir->GetStationOnJijum(nCol);
	CDPoint vAng    = pGir->GetAngleOnJijum(nCol);
	double  disShoe = GetDisShoeOnGirderCenter(nRow, nCol); // �Ŵ� �߽ɿ��� �̰� �Ÿ�
	
	double dThickPyun = pGir->GetThickPyungubae(sta, disShoe, vAng);
	return dThickPyun;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���ؼ��������� ���� �Ÿ�
///�׻� [+] ���� ������
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetDisLevelActOnShoe(long nRow, long nCol)const
{
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));
	double	sta		= pGir->GetStationOnJijum(nCol);
	CDPoint vAng    = pGir->GetAngleOnJijum(nCol);
	double  disShoe = GetDisShoeOnGirderCenter(nRow, nCol); // �Ŵ� �߽ɿ��� �̰� �Ÿ�
	
	double dDisLevel = pGir->GetDisLevelAct(sta, disShoe, vAng);

	return dDisLevel;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///���ؼ����� �������� ������ ���� ���̸� ����(���� [-], ���� [+])
///Ȯ���� �¿��� ������ ���� �Ŵ� ���� �߽ɿ����� �Ÿ�
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetDisToCenterLineByShoe(long nRow, long nCol)const
{
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));
	double	sta		= pGir->GetStationOnJijum(nCol);
	CDPoint vAng    = pGir->GetAngleOnJijum(nCol);
	double  disShoe = GetDisShoeOnGirderCenter(nRow, nCol); // �Ŵ� �߽ɿ��� �̰� �Ÿ�

	double dDisToCenterLine = pGir->GetDisToCenterLineForExcel(sta, disShoe, vAng);

	return dDisToCenterLine;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// ����ġ�� ����
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetPyungubeCenLineByShoe(long nRow, long nCol)const
{
	CPlateGirderApp	*pGir = GetGirder(GetNumberGirderByShoe(nRow, nCol));

	double	sta	  	   = pGir->GetStationOnJijum(nCol);
	CDPoint vAng       = pGir->GetAngleOnJijum(nCol);
	double  disShoe    = GetDisShoeOnGirderCenter(nRow, nCol); // �Ŵ� �߽ɿ��� �̰� �Ÿ�
	double  dPyungubae = pGir->GetPyungubaeCenLine(sta, disShoe, vAng);

	return dPyungubae;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// ���� ������ �� ���� (����� ����)
///////////////////////////////////////////////////////////////////////////////////////////////
long CPlateBridgeApp::GetQtyShoePrevious(long nRow, long nCol)const
{
	long nShoe = 0;

	for(long j=0; j<nCol; j++) {
		if(j==m_nQtyJigan) break;
		nShoe += GetQtyShoeOnJijum(j);
	}

	return nShoe + nRow;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////////
void CPlateBridgeApp::GetXySss(SLABGIRDERSHOE nTarget, CDPoint* A) const
{
	CPlateGirderApp *pGirCR = GetGirder(-1);
	CPlateGirderApp *pGirLeft = GetGirder(0);
	CPlateGirderApp *pGirRight = GetGirder(GetGirdersu()-1);

	double staLeft = pGirLeft->GetSSS(nTarget);
	CDPoint vAngLeft = pGirLeft->GetAAA(nTarget);
	double staRight = pGirRight->GetSSS(nTarget);
	CDPoint vAngRight = pGirRight->GetAAA(nTarget);

	A[0] = pGirLeft->GetXyGirderDis(staLeft, -m_dWidthSlabLeft, vAngLeft);
	A[3] = pGirRight->GetXyGirderDis(staRight, m_dWidthSlabRight, vAngRight);
	A[1] = A[0];	//���� ���� ��ǥ(Ȯ��)
	A[2] = A[3];	//���� ���� ��ǥ(Ȯ��)

	if(!IsExtLine())	return;
	BOOL bStt = TRUE;
	if(nTarget >= OFF_END_END && nTarget <= OFF_END_HUNCHEND)//������ �� ���
		bStt = FALSE;

	if		(bStt && m_dDisExtLineSttLeft == 0 && m_dDisExtLineSttRight == 0)		return;
	else if	(!bStt && m_dDisExtLineEndLeft == 0 && m_dDisExtLineEndRight == 0)	return;

	double staCR = pGirCR->GetSSS(nTarget);
	CDPoint vAngCR = pGirCR->GetAAA(nTarget);

	CDPoint vAngBangCR = pGirCR->GetLine()->GetAngleAzimuth(staCR);
	CDPoint vAngBangLeft = pGirLeft->GetLine()->GetAngleAzimuth(staLeft);
	CDPoint vAngBangRight = pGirRight->GetLine()->GetAngleAzimuth(staRight);
	CDPoint xyCR = pGirCR->GetXyGirderDis(staCR);
	CDPoint xyLeft = pGirLeft->GetXyGirderDis(staLeft);
	CDPoint xyRight = pGirRight->GetXyGirderDis(staRight);

	if(bStt)
	{
		if(m_dDisExtLineSttLeft != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyLeft, vAngBangLeft.Rotate(vAngLeft), A[1]);
		if(m_dDisExtLineSttRight != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyRight, vAngBangRight.Rotate(vAngRight), A[2]);
	}
	else
	{
		if(m_dDisExtLineEndLeft != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyLeft, vAngBangLeft.Rotate(vAngLeft), A[1]);
		if(m_dDisExtLineEndRight != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyRight, vAngBangRight.Rotate(vAngRight), A[2]);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///�Է� : pBx	���� �Ŵ� ���� Bx(�����߽� �Ŵ��� ���� Bx�� �ƴ�)
///��� : ���� -> Ȯ���� ��� ���
///       �����߽�(Ȯ���� ��� ����) �Ŵ� nIdx�� �������� �� �Ŵ��� Match nIdx���� �Ÿ� ����.
///         ���̾�����(������ ���� ����) ���� ����(��Ÿ�0, ���κ� ���� ����(��Ÿ�)�� ���� ������ ������ ����.
///		 BasicIndex�� ���� ��� dSta 500������ ������ ���� ����
///////////////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetWidthDiap90CrossAng(CPlateBasicIndex *pBx) const
{
	CPlateGirderApp  *pGir     = NULL;
	CPlateBasicIndex *pBxMatch = NULL;

	double dWidth = m_dWidthSlabLeft + m_dWidthSlabRight;
	for(long n = 0; n < GetGirdersu(); n++)
	{
		pGir = GetGirder(n);
		pBxMatch = pBx->GetBxMatchByCrossBeam(pGir);
		if(n < GetGirdersu()-1 && pBxMatch == NULL)
			pBxMatch = pBx->GetBxMatchByVert(pGir);
		dWidth += pGir->GetDisCRSttToCREndReal(pBxMatch);
	}

	return dWidth;
}
/////////////////////////////////////////////////////////////////////////////////////
///�Է� : pBxGir ���� �Ŵ� ���� Bx(�����߽� �Ŵ��� ���� Bx�� �ƴ�)
///��� : ���� -> Ȯ���� �ƴ� ��� ���
///		  ��� -> Ȯ���� �ƴ� ��� ���
///       �����߽�(Ȯ���� ��� ����) �Ŵ� nIdx�� �������� �� �Ŵ��� Match nIdx���� �Ÿ� ����.
///         ���̾�����(������ ���� ����) ���� ����, ���κ� ���� ������ ���� ������ ������ ����.
///		 BasicIndex�� ���� ��� dSta 500������ ������ ���� ����
/////////////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetWidthDiap90Cross90(CPlateBasicIndex *pBxGir) const
{
	CPlateGirderApp  *pGir = NULL;
	CPlateBasicIndex *pBx  = NULL;

	double dWidth = m_dWidthSlabLeft + m_dWidthSlabRight;
	for(long n = 0; n < GetGirdersu(); n++)
	{
		pGir = GetGirder(n);
		pBx = pBxGir->GetBxMatchByCrossBeam(pGir);
		if(n < GetGirdersu()-1 && pBx == NULL)
			pBx = pBxGir->GetBxMatchByVert(pGir);
		dWidth += pGir->GetDisGirderToGirder(pBx);
	}

	return dWidth;
}

///////////////////////////////////////////////////////////////////////
/// ���� �߽ɺ��� ���� ������ ���ܱ����� �̰ݰŸ�
///�Է� : pBxGir	���� �Ŵ� ���� Bx(�����߽� �Ŵ��� ���� Bx�� �ƴ�)
///��� : ���� �߽ɺ��� ���� ������ ���ܱ����� �̰ݰŸ�(���� ������ ������ �����߽� ������ ���� ���[-])
///		 Ȯ���϶� -> ���� ��Ÿ�
///		 Ȯ���ƴҶ�-> ���� �Ÿ�
///////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetDisSlabLeftDom(CPlateBasicIndex *pBxGir) const
{
	CPlateGirderApp *pGir = GetGirder(0);	//�׻� ���� ���� �Ŵ�
	double disSlabLeft = -m_dWidthSlabLeft + pGir->m_dGirderCenter;
	if(!IsExtLine())	return disSlabLeft;	//Ȯ���� �ƴϸ�

	CLineInfo* pLineCR = m_pCRGirder->GetLine();
	double LineDis = 0;

	CDPoint xyResult;
	double staResult = 0;
	CPlateBasicIndex *pBx = pBxGir->GetBxMatchByCrossBeam(pGir);
	if(pBx == NULL)	return 0;
	double sta = pBx->GetStation();
	CDPoint vAng = pBx->GetAngle();
	
	CDPoint p1 = pGir->GetXyGirderDis(sta, -m_dWidthSlabLeft);

	disSlabLeft = pLineCR->GetDisMatchVerticalByXy(p1);
	if(disSlabLeft >= 0)	return disSlabLeft; 
	CDPoint p2 = pGir->GetXyGirderDis(sta);
	BOOL bMatch = pLineCR->GetStationMatchSegment(p1, p2, staResult);
	xyResult = pLineCR->GetXyLine(staResult);
	if(bMatch)//ù��° �Ŵ� ������ ���ܰ� �Ŵ� ���� ������ ���̿� ���� ������ ���� ���
	{
		disSlabLeft = -~(p1-xyResult);
		return disSlabLeft;
	}

	disSlabLeft = -~(p1-p2);
	for(long nG = 0; nG < GetGirdersu(); nG++)
	{
		p1 = pGir->GetXyGirderDis(sta);
		p2 = pGir->GetXyGirderDis(sta);
		bMatch = pLineCR->GetStationMatchSegment(p1, p2, staResult);
		xyResult = pLineCR->GetXyLine(staResult);
		if(bMatch)
		{
			disSlabLeft -= ~(p1-xyResult);
			return disSlabLeft;
		}
		disSlabLeft -= ~(p1-p2);

		if(nG == GetGirdersu()-1)		break;

		p1 = pGir->GetXyGirderDis(sta);
		pGir = GetGirder(nG+1);
		pBx = pBxGir->GetBxMatchByCrossBeam(pGir);
		if(pBx==NULL) return 0;	// �´��� ???????
		sta = pBx->GetStation();
		vAng = pBx->GetAngle();

		p2 = pGir->GetXyGirderDis(sta);
		bMatch = pLineCR->GetStationMatchSegment(p1, p2, staResult);
		xyResult = pLineCR->GetXyLine(staResult);
		if(bMatch)
		{
			disSlabLeft -= ~(p1-xyResult);
			return disSlabLeft;
		}
		disSlabLeft -= ~(p1-p2);
	}

	p1 = pGir->GetXyGirderDis(sta);
	p2 = pGir->GetXyGirderDis(sta, m_dWidthSlabRight);

	bMatch = pLineCR->GetStationMatchSegment(p1, p2, staResult);
	xyResult = pLineCR->GetXyLine(staResult);
	if(bMatch)
	{
		disSlabLeft -= ~(p1-xyResult);
		return disSlabLeft;
	}
	disSlabLeft -= ~(p1-p2);

	LineDis = pLineCR->GetDisMatchAngByTargetLine(pGir->GetLine(), sta);

		//Ȯ���̰� �����߽��� ������ ������ �������
	disSlabLeft -= -LineDis - pGir->m_dGirderCenter - m_dWidthSlabRight;

	return disSlabLeft;
}

///////////////////////////////////////////////////////////////////////
/// ���� �߽ɺ��� ���� ������ ���ܱ����� �̰ݰŸ�
///////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetDisSlabRightDom(CPlateBasicIndex *pBxGir) const
{
	return GetWidthSlabDom(pBxGir) + GetDisSlabLeftDom(pBxGir);
}

///////////////////////////////////////////////////////////////////////
///��� : �����߽� ������ �ִ� ������ �ѱ���(�����갡 �������� �ִٸ� �� �Լ��� ����� �ȵ�.)
///////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetDisSlabLeftLevelDom(CPlateBasicIndex *pBxGir) const
{
	double dDisLevel   = 0;		//�����߽� ������ ���� �� �Ÿ�
	double dWidth      = GetWidthSlabDom(pBxGir);
	double disSlabLeft = GetDisSlabLeftDom(pBxGir);	//�����߽ɿ��� ������ ���������� ����

	if(disSlabLeft >= 0) return disSlabLeft;		//������ ������ �����߽� ������ ������

	double lenTotal = 0;
	for(long n=0; n<GetQtyHDanNode(); n++)
	{
		lenTotal += GetLengthHDanDom(pBxGir, n);
		if(GetTypeLevelHDan(n) == 1)		//Level �̸�
			if((lenTotal <= -disSlabLeft))
				dDisLevel += GetLengthHDanDom(pBxGir, n);
			else if((lenTotal > -disSlabLeft) && (lenTotal - GetLengthHDanDom(pBxGir, n)< -disSlabLeft))
					//���� ������ �����߽ɿ� ������ ��
				dDisLevel += (-disSlabLeft - (lenTotal - GetLengthHDanDom(pBxGir, n)));
	}

	if(-disSlabLeft > dWidth && m_cLevelRoadCenterToSlabEdge == TRUE)	dDisLevel += (-disSlabLeft - dWidth);

	return dDisLevel;
}

///////////////////////////////////////////////////////////////////////
///�Է� : pBxGir	���� �Ŵ� ���� Bx(�����߽� �Ŵ��� ���� Bx�� �ƴ�)
///		 nHDanNode ���� ��� ��ȣ (0, 1....)  => m_dLengthHDan[n]
///��� : pBxGir ������ nHDanNode ������ ���̸� ����
///////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetLengthHDanDom(CPlateBasicIndex *pBxGir, long nHDanNode) const
{
	double len = 0;
	if(GetTypeVariableLengthHDan( nHDanNode) == 1 && pBxGir)	//���� �����϶�
	{
		double	width		= GetWidthSlabDom(pBxGir);
		double	widthInput	= 0;
		long	nVarLineSu	= 0;		//���� ���� ����
		for(long n = 0; n<GetQtyHDanNode(); n++)
		{
			if(GetTypeVariableLengthHDan( n) == 1)		//���� �����϶�(Ȯ������ ���, �⺻ ������ ���� �̻����.)
				nVarLineSu++;
			else
				widthInput += m_dLengthHDan[n];
		}
		len = (width - widthInput)/nVarLineSu;	//���� ������ 2���϶�
	}
	else
	{
		len = m_dLengthHDan[nHDanNode];
	}

	return len;
}

///////////////////////////////////////////////////////////////////////
///�Է� : pBxGir
///       D      �����߽ɿ����� �̰� �Ÿ�
///��� : �����߽ɿ��� D ��ŭ �̰ݵ� �Ÿ��� ������(D��ġ�� ������ [+])
///////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetHeightSlabFromRoadCenterDom(CPlateBasicIndex *pBxGir, double D) const
{
	double ele = 0;	//�����߽� �������̼�(mm)
	double disSlabLeft  = GetDisSlabLeftDom(pBxGir);
	double SlabRightDis = GetWidthSlabDom(pBxGir) + disSlabLeft;

	CDPoint A[HDANLINESU];
	GetXyPaveDom(pBxGir, A);//?
//	if(D <= disSlabLeft)	return A[0].y;
//	if(D >= SlabRightDis)	return A[GetQtyHDanNode()].y;
	if(fabs(D - disSlabLeft)  < 0.0000001 || D < disSlabLeft)	return A[0].y;
	if(fabs(D - SlabRightDis) < 0.0000001 || D > SlabRightDis)	return A[GetQtyHDanNode()].y;
	//����� ���ǿ��� ����� D		->     disSlabLeft < D <  SlabRightDis
	for(long n=1; n<GetQtyHDanDom(pBxGir)+1; n++)//GetQtyHDanNode()+1
	{
		if(D < A[n].x)
		{
			double Dis = D - A[n-1].x;
			double DisEle = 0;
			if(A[n].x - A[n-1].x == 0)		DisEle = 0;
			else DisEle = (A[n].y - A[n-1].y) * Dis / (A[n].x - A[n-1].x);
			ele += A[n-1].y + DisEle;
			break;
		}
	}
		
	return ele;		
}

//��� : �ش� �����̼� �����߽ɿ��� dis��ŭ �̰ݵ� �Ÿ��� ���� ������ �������� �β�
//       ������ �ʺ� ������ ������ 0 �� ����

double CPlateBridgeApp::GetThickSlabAndPaveByDis(CPlateBasicIndex *pBxGir, double D, BOOL bJijumHunchApply)const
{
	CPlateBridgeApp *pBridge = (CPlateBridgeApp *)this;
	
	double width		= GetWidthSlabDom(pBxGir);
	double disSlabLeft	= GetDisSlabLeftDom(pBxGir);
	if(D < disSlabLeft || D > (disSlabLeft + width))	return 0;

	CDPoint cantiA[10], A[10];
	CPlateBasicIndex *pBx = pBxGir->GetBxMatchByCrossBeam(m_pCRGirder);
	double eleSlab = GetElevationPaveActByDis(pBx->GetStation(), D)/*- m_pLine->GetElevationPlanEl(pBx->GetStation())*/;
	CDPoint xySlabDis(D, eleSlab);
	CDPoint vAng(0, 1);	//���� �����߽� �����̼ǰ� �̷�� ���� 90���� ��.
	CDPoint xyResult;

	for(long nG = 0; nG < GetGirdersu(); nG++)	//�ڽ� ��ü�� �Ŵ� ����
	{
		GetGirder(nG)->GetHunchXyDom(pBxGir, A, FALSE);
		if(D >= A[0].x && D <= A[9].x)
		{
			for(long n = 0; n < 10; n++)
			{
				if(GetXyMatchSegAndLine(A[n], A[n+1], xySlabDis, vAng, xyResult))		break;
			}
			return xySlabDis.y - xyResult.y;
		}
	}

	pBridge->GetXyFrontCantilever(pBxGir, cantiA, TRUE);	//������ ���� ĵƿ������ 
	if(D >= cantiA[0].x && D <= cantiA[3].x)
	{
		for(long n = 0; n < 4; n++)
		{
			if(GetXyMatchSegAndLine(cantiA[n], cantiA[n+1], xySlabDis, vAng, xyResult))		break;
		}
		return xySlabDis.y - xyResult.y;
	}

	pBridge->GetXyFrontCantilever(pBxGir, cantiA, FALSE);	//������ ���� ĵƿ������ 
	if(D >= cantiA[3].x && D <= cantiA[0].x)
	{
		for(long n = 0; n < 4; n++)
		{
			if(GetXyMatchSegAndLine(cantiA[n], cantiA[n+1], xySlabDis, vAng, xyResult))		break;
		}
		return xySlabDis.y - xyResult.y;
	}

	ASSERT(FALSE);
	return 0;
}

///////////////////////////////////////////////////////////////////////
///xy���� slope ���⸦ ���� ������ ������ �ϸ�(���-80-250+50)�� ������.
///���� ��鿡�� ������ �����̵��� ������� ������ ����.
///�Ŵ����� �⺻ ������ �β����� ������ ������ ã���� ���
///		*pBxGir : �Ŵ� pBx : �����߽� pBx�� �ƴҼ��� ����.
///		xy      : ������ ���� ���ϴ� ��ġ
///		slope   : ��ǥ���� �̷�� ����(�������� �ö󰡸� => "0")
///		bLowSlab: ������ �Ʒ� ��(�⺻�β� ���ϸ�)���� ������ �������� ����
///////////////////////////////////////////////////////////////////////
CDPoint CPlateBridgeApp::GetMatchPavePointDom(CPlateBasicIndex *pBxGir, CDPoint xy, double slope, BOOL bLowSlab) const
{
	CPlateBridgeApp *pDB   = (CPlateBridgeApp*)this;
	if(pDB->IsTUGir() && m_dThickSlabHunch == 0)	return xy;
	
	CDPoint vAng = CDPoint(1, slope);
	if(slope == 0)	vAng = CDPoint(0, 1);//90��
	CDPoint xyResult(0,0);
	CDPoint A[HDANLINESU];
	GetXyPaveDom(pBxGir, A);//?
	
	double dHTaper = pBxGir->GetGirder()->GetHeightTaper(pBxGir->GetStation());
	double dTHunch = m_dThickSlabHunch;
	if(dHTaper > 0)
		dTHunch = 0;
	if(bLowSlab)
	{
		double SlabT = m_dThickPave + m_dThickSlabBase - dTHunch + dHTaper; //80+300-50 = 330

		for(long n=0; n<= GetQtyHDanDom(pBxGir); n++)//GetQtyHDanNode(); n++)		//������ �������� 1�� ����.
			A[n].y -= SlabT;
	}
	for(long n=0; n<GetQtyHDanDom(pBxGir); n++)//
		if(GetXyMatchSegAndLine(A[n],A[n+1],xy,vAng,xyResult))		break;

	return xyResult;
}

////////////////////////////////////////////////////////////////////////////
///�����߽ɿ��� �����߽�(�������߽�)������ �̰ݰŸ�
////////////////////////////////////////////////////////////////////////////
double CPlateBridgeApp::GetDisRoadCenToSlabCen(CPlateBasicIndex *pBx)
{
	return GetDisSlabLeftDom(pBx) + GetWidthSlabDom(pBx)/2;
}


CPlateBasicIndex *CPlateBridgeApp::GetBxOnNearByPoint(CDPoint xyPoint, __int64 bxFlag, BOOL isHunch)
{
	CVectorArray arrBxPos;
	CVector		 vPos;
	for(long nG=0; nG<GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(bxFlag);
		while(pBx)
		{
			CDPoint xyPos = pGir->GetXyGirderDis(pBx->GetStation());
			vPos.x = nG; 
			vPos.y = pBx->GetStation();
			vPos.z = fabs(~(xyPos-xyPoint));
			
			if(isHunch)// ��ٵ����� ���...
			{
				if(pGir->IsCrossBeamBxHasHunch(pBx))
					arrBxPos.Add(vPos);
			}
			else
				arrBxPos.Add(vPos);
			
			pBx = Finder.GetBxNext();
		}
	}
	
	arrBxPos.Sort(2, TRUE);
	CPlateBasicIndex	*pBx = NULL;
	if(arrBxPos.GetSize()>0)
	{
		CVector vReturn = arrBxPos.GetAt(0);
		pBx = GetGirder((long)vReturn.x)->GetBxByStation(vReturn.y, bxFlag);
	}

	return pBx;
}

double CPlateBridgeApp::GetBoltRad(CString szBoltSize)
{
	szBoltSize.Delete(0);
	double dDia = atof(szBoltSize);
	return 22.5/2 + (dDia-20);
}

// tendon ������ ��ġ���� �� ������ ����
void CPlateBridgeApp::SetCalcJongTendon()
{
	CGlobarOption   *pGlopt    = m_pDataManage->GetGlobalOption();
	CCalcData       *pCalcData = m_pDataManage->GetCalcData();
	CPlateGirderApp *pGir      = GetGirder(CENTERGIRDER);
	
	if(pGir->GetQtyCrossSectionTotal() == 0)	return;

	CPlateBxFinder Finder(pGir);
	
	double dCTC        = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;	// �ٴ��� �߽ɰŸ�
	double dWSttJoint  = pGlopt->GetSttExpansionJointWidth();	// ���������� ���ۺ� ��
	double dWEndJoint  = pGlopt->GetEndExpansionJointWidth();	// ���������� ���� ��
	double dStaSttSlab = pGir->GetSSS(OFF_STT_SLAB);	// ������ ���� �����̼�
	double dStaEndSlab = pGir->GetSSS(OFF_END_SLAB);	// ������ ��   �����̼� 
	double dLengthSlab = fabs(dStaEndSlab-dStaSttSlab); // ������ ����
	double dStaStt     = dStaSttSlab+dLengthSlab/2;		// �������� �߽�
	
	m_arrTendon.RemoveAll();
	m_arrTendonDom.RemoveAll();

	double	dSta = 0;
	long	n	 = 0;
	// �߽ɿ��� ������������....(�߽�����)
	for(n=0; n<3000; n++)
	{
		dSta = dStaStt-dCTC*n;
		if(dSta < dStaSttSlab+dWSttJoint*2)	break;
		
		CDPoint  xyTendon(dSta, 0);
		m_arrTendon.Add(xyTendon);
	}
	// �߽ɿ��� ������������....(�߽ɹ�����)
	for(n=0; n<3000; n++)
	{
		dSta = dStaStt+dCTC*n;
		if(dSta > dStaEndSlab-dWEndJoint*2)	break;

		CDPoint  xyTendon(dSta, 0);
		m_arrTendon.Add(xyTendon);
	}
	m_arrTendon.Sort();
	
	// �ٴ� ������ ��ġ -------------------------
	if(m_nTendonFix==0)	// ������ ����
	{
		for(long nTendon=0; nTendon<m_arrTendon.GetSize(); nTendon++)
		{
			m_arrTendon.GetAt(nTendon).y = 0;
		}
			
	}
	else if(m_nTendonFix==1)	// ������ ����
	{
		for(long nTendon=0; nTendon<m_arrTendon.GetSize(); nTendon++)
		{
			m_arrTendon.GetAt(nTendon).y = 1;
		}
	}
	else if(m_nTendonFix==2)	// �������
	{
		for(long nTendon=0; nTendon<m_arrTendon.GetSize(); nTendon++)
		{
			if(nTendon%2==0)
				m_arrTendon.GetAt(nTendon).y = 0;
			else
				m_arrTendon.GetAt(nTendon).y = 1;
		}

	}

	//���鿡���� ���� ���� ����
	//0 : ������ ���� , 1 : ������ ����, 2: �������
	for(long nTendon = 0; nTendon < m_arrTendon.GetSize(); nTendon++)
	{
		if (m_nTendonDomFix == 2)
		{
			if(nTendon%2==0)
				m_arrTendonDom.Add(0);
			else
				m_arrTendonDom.Add(1);
		}
		else
		{
			m_arrTendonDom.Add(m_nTendonDomFix);
		}
	}
		
}
/////////////////////////////////////////////////////////////////////////////
///Ⱦ�й� ���� ��ȭ �˻�
/////////////////////////////////////////////////////////////////////////////
BOOL CPlateBridgeApp::IsPyunGubaeSame(CPlateBasicIndex *pBxpre, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pDB   = (CPlateBridgeApp*)this;
	CPlateGirderApp *pGir  = pDB->GetGirder(-1);
	CLineInfo       *pLine = pGir->GetLine();

	double TotalSta=0; // ALine ���� ��ü Sta(�������Ե�)

	double dStaPre = pBxpre->GetStation();
	double dSta    = pBx->GetStation();

	double dSlopPreLeft  = 0;
	double dSlopPreRight = 0;

	double dSlopLeft	 = 0;
	double dSlopRight	 = 0;

	long nLeftsu	= pLine->GetLSlopsu();
	long nRightsu	= pLine->GetRSlopsu();
	long i			=	0;
	// dSlopPreLeft
	for(i = 0; i < nLeftsu; i++)
	{
		TotalSta = pLine->m_pSlopLeft[i]->m_dStationPyungubae;
		if(TotalSta < dStaPre) continue;
		dSlopPreLeft = fabs(pLine->m_pSlopLeft[i]->m_dValuePyungubae);
		break;
	}

	// dSlopLeft
	for(i = 0; i < nLeftsu; i++)
	{
		TotalSta = pLine->m_pSlopLeft[i]->m_dStationPyungubae;
		if(TotalSta < dSta) continue;
		dSlopLeft = fabs(pLine->m_pSlopLeft[i]->m_dValuePyungubae);
		break;
	}

	// dSlopPreRight
	for(i = 0; i < nRightsu; i++)
	{
		TotalSta = pLine->m_pSlopRight[i]->m_dStationPyungubae;
		if(TotalSta < dStaPre) continue;
		dSlopPreRight = fabs(pLine->m_pSlopRight[i]->m_dValuePyungubae);
		break;
	}

	// dSlopRight
	for(i = 0; i < nRightsu; i++)
	{
		TotalSta = pLine->m_pSlopRight[i]->m_dStationPyungubae;
		if(TotalSta < dSta) continue;
		dSlopRight = fabs(pLine->m_pSlopRight[i]->m_dValuePyungubae);
		break;
	}

	if((fabs(dSlopLeft - dSlopPreLeft) < 0.0001) && (fabs(dSlopRight - dSlopPreRight) < 0.0001))
		return TRUE;
	else
		return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
/// Ⱦ�й� ������ ��ȭ �˻�
/////////////////////////////////////////////////////////////////////////////
BOOL CPlateBridgeApp::IsWidthSame(CPlateBasicIndex *pBxpre, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp *pDB   = (CPlateBridgeApp*)this;

	double dWidthPre = pDB->GetWidthSlabDom(pBxpre);
	double dWidth    = pDB->GetWidthSlabDom(pBx);

	if(fabs(dWidth - dWidthPre) < 1)	return TRUE;	// 1mm ����
	else								return FALSE;
}

BOOL CPlateBridgeApp::IsByunDanInBridge()
{
	for(long nG=0; nG<GetGirdersu(); nG++)
	{
		for(long n=0; n<m_nQtyJigan; n++)
		{
			if(GetGirder(nG)->IsByunDanMyunInJigan(n))
				return TRUE;
		}	
	}
	return FALSE;
}


// �����߽� ���� ���� �������� ��� ���������� ����.
// pLine->GetTypeLine() =	0 = ����
//							1, 3 = ũ�μ��̵�, 3��������
//							2 = ���
BOOL CPlateBridgeApp::IsStraightLineTotalBridge() const
{
	CPlateGirderApp*	pGir	= GetGirder(-1);	//���� �߽� �Ŵ�
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxStt = Finder.GetBxFirst(BX_STT_JOINT);
	CPlateBasicIndex *pBx	 = Finder.GetBxFirst(BX_LINE_P);
	if(m_pLine->GetTypeLine(pBxStt->GetStation() != 0))	return FALSE;	//������ ������ �ƴ� ���
	if(pBx == NULL)	return TRUE;
	
	return FALSE;	//��� ���ԵǾ� �ִ� ����, ���� ������ ����
}

BOOL CPlateBridgeApp::IsTypeGuardFenceDom(long nHDan) const
{
	if(GetWallGuardTypeHDan(nHDan) == HT_GUARDFENCE || 
			GetWallGuardTypeHDan(nHDan) == HT_CENTERGUARDFENCE)
			return TRUE;
	else
		return FALSE;
}

long CPlateBridgeApp::GetNumHDanByGuardFence(CCentSeparation *pGW) const
{
	if(pGW == NULL)	return -1;
	long nGuard = 0;
	for(long nHDan = 0; nHDan < GetQtyHDanNode(); nHDan++)
	{
		if(IsTypeGuardFenceDom(nHDan))
		{
			CCentSeparation *pFenceMatch = GetGuardWall(nGuard);
			if(pFenceMatch == pGW)
				return nHDan;

			nGuard++;
		}
	}
	return -1;	//��� ���ԵǾ� �ִ� ����, ���� ������ ����
}

long CPlateBridgeApp::GetNumGuardFenceFrLeft(long nHDan) const
{
	if(!IsTypeGuardFenceDom(nHDan))	//��ȣ�� Ÿ���� �ƴ� ���
		return -1;

	long nFence = -1;
	for(long n=0; n <= nHDan; n++)
	{
		if(IsTypeGuardFenceDom(n))	//��ȣ�� Ÿ���̸�
		{
			nFence++;
		}
	}
	return nFence;
}

CDPoint CPlateBridgeApp::GetXyHDanLeftDom(CPlateBasicIndex *pBx, long nHDan) const
{
	CDPoint A[HDANLINESU];
	GetXyPaveDom(pBx, A);
	long nSlabNode = GetNumSlabNodeDom(pBx, nHDan);
	return A[nSlabNode];

}

//�Է� : pBxGir	�ش� �Ŵ�
//		 nHDan	Ⱦ�� ������ �����Է¿����� ��ȣ, �ش� ��ȣ�� ���� ����, ��ȣ�� ����, �������� ���ΰ� ����.
//��� : �Է¿����� ��ȣ�� ������ ��� ������ ��ȣ�� ����.
//		 ���� ������ ����� �׸��ų�, Elevation, ���̰��� ��쿡 ���
long CPlateBridgeApp::GetNumSlabNodeDom(CPlateBasicIndex *pBxGir, long nHDan) const
{
	double width		= GetWidthSlabDom(pBxGir);
	double disSlabLeft	= GetDisSlabLeftDom(pBxGir);
	if(disSlabLeft > 0 || (disSlabLeft + width) < 0)	//�����߽��� ������ ���̿� ���� ���
		return nHDan; 

	double dis = disSlabLeft;
	double disLineToLine = 0;
	/*
	long nLineRail = GetRailLineNumber(0);
	if(IsRailBridgeType() && nLineRail != 0)
	{
		CLineInfo *pLine = ((CLineInfoApp*)m_pLine)->GetLine(nLineRail);
		disLineToLine = m_pLine->GetDisMatchAngByTargetLine(pLine, pBxGir->GetStation(), CDPoint(0,1));
	}
	*/
	for(long nH = 0; nH<nHDan+1; nH++)
	{
		double dLengthHDan = GetLengthHDanDom(pBxGir, nH);	//�����߽� ���� ���̰� ����ȵ� ���� ���, �Է� Ⱦ�ܰ��Ҽ� ���� nH
		if(dis <= disLineToLine && (dis + dLengthHDan) > disLineToLine)
		{
			if(nHDan > nH)
				return nHDan + 1;
		}
		dis += dLengthHDan;
	}
	return nHDan;
}
void CPlateBridgeApp::GetXyGuardFenceDom(CPlateBasicIndex *pBx, long nHDan, 
								CDPoint A[9], BOOL bActual /*= FALSE*/) const
{
	if(!IsTypeGuardFenceDom(nHDan))	//��ȣ�� Ÿ���� �ƴ� ���
		return;

	long nFence = GetNumGuardFenceFrLeft(nHDan);
	CCentSeparation* pGW = GetGuardWall(nFence);
	if(pGW == NULL)	return;
	pGW->GetXyGuardFence(A);
	CDPoint xyLeft = GetXyHDanLeftDom(pBx, nHDan);
	xyLeft.y -= GetThickPave();
	for(long n = 0; n < 9; n++)
		A[n] += xyLeft;

	if(bActual)
	{
//		if(fabs(A[1].x) < fabs(A[8].x))
//		{
//			double width = A[8].x - A[0].x;
//			double slope = GetSlopeRailByLine(nHDan);
//			if(A[0].x < 0)	slope = -slope; 
//			double height = width * slope;
//			for(long n = 0; n < 9; n++)
//				A[n].y += height;
//		}
		CDPoint xyMatch   = GetMatchPavePointDom(pBx, A[1], 0, FALSE);
		if(GetBridgeType()!=TU_GIR)	xyMatch.y -= GetThickPave();
		A[1] = xyMatch;

		int nEnd = 8;
		//if(pGW->IsElecHole()) nEnd = 2;
		xyMatch   = GetMatchPavePointDom(pBx, A[nEnd], 0, FALSE);
		if(GetBridgeType()!=TU_GIR)	xyMatch.y -= GetThickPave();
		A[nEnd] = xyMatch;

	}
	
	return;	
}

//<summary>
//��� : ������ ����� ���� �Է�, ���� ������ ���� x, y ��ǥ�� ���Ѵ�.(�����)
//		 Ȯ���� ���			-> ���̾����� ���� ����, ���κ��� ���� ���� ���� ����
//		 Ȯ���� �ƴ� ���		-> ���̾�����, ���κ��� ���� ���� ���� ����
//</summary>
//<param name="*pBx">���� �Ŵ� ���� Bx(�����߽� �Ŵ��� ���� Bx�� �ƴ�)</param>
//<param name="A[HDANLINESU]">��ȯ ���� ���� ��ǥ��</param>
void CPlateBridgeApp::GetXyPaveDom(CPlateBasicIndex *pBx, CDPoint A[HDANLINESU]) const
{
	GetXyPave(FALSE, A, 0, pBx);
}

void CPlateBridgeApp::GetXyPave(BOOL bActual, CDPoint A[HDANLINESU], double staCR, CPlateBasicIndex *pBx /*= NULL*/) const
{
	//BOOL bRail = IsRailBridgeType();
	BOOL bRail = FALSE;
	GetXyPaveRail(bRail, bActual, A, staCR, pBx);		
	
	if(bActual)
	{
		double planEL = m_pLine->GetElevationPlanEl(staCR);//
		for(long n = 0; n < GetQtyHDanReal(bActual, staCR, pBx)+1; n++)
		{
			A[n].y += planEL;
		}
	}
	return;
}
//<summary>
//��� : �ƽ��� ����� ���� �Է�, ���� ������ ���� x, y ��ǥ�� ���Ѵ�.(�����)
//		 �� �Լ������� ���ؼ��� �߽��� (0,0)�� �ǵ��� �� ���¸� ����.
//		 Ȯ���� ���			-> ���̾����� ���� ����, ���κ��� ���� ���� ���� ����
//		 Ȯ���� �ƴ� ���		-> ���̾�����, ���κ��� ���� ���� ���� ����
//</summary>
//<param name="*pBx">���� �Ŵ� ���� Bx(�����߽� �Ŵ��� ���� Bx�� �ƴ�)</param>
//<param name="A[HDANLINESU]">��ȯ ���� ���� ��ǥ��</param>
void CPlateBridgeApp::GetXyPaveRail(BOOL bRail, BOOL bActual, CDPoint A[HDANLINESU], double staCR, CPlateBasicIndex *pBx /*= NULL*/) const
{
	long nHDan = 0;
	for(nHDan=0; nHDan<HDANLINESU; nHDan++){
		A[nHDan].x = 0;
		A[nHDan].y = 0;
	}
	double sta	= staCR;
	double disSlabLeft	= 0;
	long nQtyHDanReal = 0;

	double disLineToLine = 0;

	if(bActual)
	{
		disSlabLeft	= GetDisSlabLeftAct(sta);
		nQtyHDanReal = GetQtyHDanAct(sta);
	}
	else
	{
		//pBxCen = pBx->GetBxMatchByCrossBeam(m_pCRGirder,1); //GetBxMatchByDiap(m_pCRGirder);
		//if(pBxCen == NULL)	
			//pBxCen = pBx->GetBxMatchByVert(m_pCRGirder);
		sta		= pBx->GetStation();//pBxCen->GetStation();
		disSlabLeft	= GetDisSlabLeftDom(pBx);
		nQtyHDanReal = GetQtyHDanDom(pBx);		
	}

	A[0].x = disSlabLeft;
	A[0].y = 0;

	long nHDanAct = 0;	//���� ��� ������. nHdan �� ���ų� 1�� �� ���� �� ����.(�����߽��� �����곻�� ���� ���)
	double slope = 0;
	double heightFrSlabLeftToCen = 0;	//������ �������ܰ� Ⱦ�� ���ؼ��� �߽ɰ��� ������
	double dLengthHDanAct = 0;
	long nQtyHDan = GetQtyHDanNode();
	for(nHDan = 0; nHDan<nQtyHDan; nHDan++)
	{
		
		if(GetTypeLevelHDan(nHDan) == 0)	
			slope	= m_pCRGirder->GetLine()->GetPyungubae(sta, A[nHDanAct].x);
		else							
			slope	= 0;
		
		if(A[nHDanAct].x - disLineToLine <= -0.0000001)	
			slope = -slope; 		
		
		if(bActual)	
			dLengthHDanAct = GetLengthHDanAct(sta, nHDan);
		else		
			dLengthHDanAct = GetLengthHDanDom(pBx, nHDan);

		if(A[nHDanAct].x <= disLineToLine && (A[nHDanAct].x + dLengthHDanAct) > disLineToLine)	//�����߽��� ���������� Ⱦ���� �����߽ɿ��� �и��Ͽ� �߰�
		{
			double disFrZero = dLengthHDanAct + A[nHDanAct].x;
			A[nHDanAct+1].x = disLineToLine;
			
			A[nHDanAct+1].y = A[nHDanAct].y + fabs(A[nHDanAct].x-disLineToLine)*slope;

			
			if(GetTypeLevelHDan(nHDan) == 0)
				slope	= m_pCRGirder->GetLine()->GetPyungubae(sta, 0);	//"0"�� ���(���ؼ��� �߽��� ���)
				
			heightFrSlabLeftToCen = A[nHDanAct+1].y;
			A[nHDanAct+2].x = disFrZero;
			
			A[nHDanAct+2].y = A[nHDanAct+1].y + fabs(disFrZero-disLineToLine) * slope;

			nHDanAct++;
		}
		else
		{
			A[nHDanAct+1].x = A[nHDanAct].x + dLengthHDanAct;
			A[nHDanAct+1].y = A[nHDanAct].y + dLengthHDanAct*slope;

			if(A[nHDanAct].x + dLengthHDanAct == disLineToLine)
			{			
				double disFrZero = dLengthHDanAct + A[nHDanAct].x;
				heightFrSlabLeftToCen = A[nHDanAct+1].y;
				A[nHDanAct+2].x = disFrZero;
				
				A[nHDanAct+2].y = A[nHDanAct+1].y + fabs(disFrZero-disLineToLine) * slope;
			}
		}
		nHDanAct++;		
	}
		
	//heightFrSlabLeftToCen �� ���ؼ����� ������ �¿��� ���̿� ���� ���(���ؼ����� �����긦 ����� ���� ���)���� ���� ������ ����.
	if(A[0].x >= 0)	//���� ������ ������ �����߽��� ������ ���� ���
	{
		heightFrSlabLeftToCen = A[0].y;	//�׻� "0"��
		if(!m_cLevelRoadCenterToSlabEdge)	//ö������ ��쿡�� �׻� ������ ������.
		{
			double slopeCen		= m_pCRGirder->GetLine()->GetPyungubae(sta, A[0].x);//���� ������ ������ ����
			heightFrSlabLeftToCen += (-A[0].x*slopeCen);	//���� �����곡�ܿ��� ���ؼ����߽ɱ��� �Ÿ� ���簡 �����
		}
	}
	else if(A[nQtyHDanReal].x <= 0)	//���� ������ ������ �����߽��� ������ ���� ���
	{
		heightFrSlabLeftToCen = A[nQtyHDanReal].y;
		if(!m_cLevelRoadCenterToSlabEdge)	//ö������ ��쿡�� �׻� ������ ������.
		{
			double slopeCen		= m_pCRGirder->GetLine()->GetPyungubae(sta, A[nQtyHDanReal].x);//���� ������ ������ ����
			heightFrSlabLeftToCen += (A[nQtyHDanReal].x*slopeCen);	//���� �����곡�ܿ��� ���ؼ����߽ɱ��� �Ÿ� ���簡 �����
		}
	}
	
	//�� ���α����� ������ �������� �������̼��� "0"�� �����Ǿ� ����. �̰��� Ⱦ���� ���ؼ��� �߽��� "0"�� �ǵ��� ��ȯ��.
	for(nHDan=0; nHDan < nQtyHDanReal+1; nHDan++)	//�����߽� ��ġ�� ���̸� "0"�� ������.
		A[nHDan].y -= heightFrSlabLeftToCen;	

}

//�Է� : staCR	�����߽� �����̼�
//��� : ������ ����� �������� ������.(�����߽��� ������ ���̿� ������ ���������� +1 �� ���� ����)
//		 ���� ������ ����� �׸��ų�, Elevation, ���̰��� ��쿡 ���
long CPlateBridgeApp::GetQtyHDanAct(double staCR) const
{
	return GetQtyHDanReal(TRUE, staCR);
}
//�Է� : pBxGir	�ش� �Ŵ�
//��� : ������ ����� �������� ������.(�����߽��� ������ ���̿� ������ ���������� +1 �� ���� ����)
//		 ���� ������ ����� �׸��ų�, Elevation, ���̰��� ��쿡 ���
long CPlateBridgeApp::GetQtyHDanDom(CPlateBasicIndex *pBx) const
{
	return GetQtyHDanReal(FALSE, 0, pBx);
}

long CPlateBridgeApp::GetQtyHDanReal(BOOL bActual, double staCR, CPlateBasicIndex *pBx) const
{
	double width;
	double disSlabLeft;
	if(bActual)
	{
		width		= GetWidthAct(staCR);
		disSlabLeft	= GetDisSlabLeftAct(staCR);
	}
	else
	{
		width		= GetWidthSlabDom(pBx);//?
		disSlabLeft	= GetDisSlabLeftDom(pBx);
	}
	if(disSlabLeft > 0 || (disSlabLeft + width) <= 0)	//�����߽��� ������ ���̿� ���� ���
		return m_nQtyHDanNode; 

	return m_nQtyHDanNode+1;	
}

//�Է� : pBxGir	�ش� �Ŵ�
//		 nHDanDom	Ⱦ�� ������ ��ȣ
//��� : �Է¿����� ��ȣ�� ������ ��� ������ ��ȣ�� ����.
//		 ���� ������ ����� �׸��ų�, Elevation, ���̰��� ��쿡 ���
long CPlateBridgeApp::GetNumInputNodedeDom(CPlateBasicIndex *pBxGir, long nHDanDom) const
{
	double width		= GetWidthSlabDom(pBxGir);
	double disSlabLeft	= GetDisSlabLeftDom(pBxGir);
	if(disSlabLeft > 0 || (disSlabLeft + width) < 0)	//�����߽��� ������ �ۿ� ���� ���
		return nHDanDom; 

	double dis = disSlabLeft;
	for(long nH = 0; nH<nHDanDom; nH++)
	{
		double dLengthHDan = GetLengthHDanDom(pBxGir, nH);	//�����߽� ���� ���̰� ����ȵ� ���� ���, �Է� Ⱦ�ܰ��Ҽ� ���� nH
		if(dis <= 0 && (dis + dLengthHDan) > 0)
		{
			if(nHDanDom > nH)
				return nHDanDom - 1;
		}
		dis += dLengthHDan;
	}
	return nHDanDom;
}

/**	\brief	The CPlateBridgeApp::IsExistLeftGuardFence function
	���� ���� ��ȣ���� �����ϴ°��� �˻� - ���� ��ȣ���� ���� ���ܿ� ��ȣ���� �ִ����� �˻��Ѵ�.	
	\return	BOOL �����ϸ� TRUE �׷��� ������ FALSE
*/
BOOL CPlateBridgeApp::IsExistLeftGuardFence()
{
	if(GetQtyHDanNode()==0) return FALSE;
	if(IsTypeGuardFenceDom(0)) return TRUE;		
	return FALSE;
}

/**	\brief	The CPlateBridgeApp::IsExistRightGuardFence function
	���� ���� ��ȣ���� �����ϴ°��� �˻� - ���� ��ȣ���� ���� ���ܿ� ��ȣ���� �ִ����� �˻��Ѵ�.	
	\return	BOOL �����ϸ� TRUE �׷��� ������ FALSE
*/
BOOL CPlateBridgeApp::IsExistRightGuardFence()
{
	if(GetQtyHDanNode()==0) return FALSE;
	if(IsTypeGuardFenceDom(GetQtyHDanNode()-1)) return TRUE;		
	return FALSE;
}


long CPlateBridgeApp::GetGuardFencesu()
{
	return m_pGuardWall.GetSize();
}


CCentSeparation *CPlateBridgeApp::GetGuardFence(long nGuard)
{
	return m_pGuardWall.GetAt(nGuard);
}


/**	\brief	The *CPlateBridgeApp::GetGuardFencLeft function
	���� ��ȣ����ü�� �����͸� ������
	\return	CCentSeparation ��ȣ���� �������� ������ NULL	
*/
CCentSeparation *CPlateBridgeApp::GetGuardFencLeft()
{
	if(!IsExistLeftGuardFence()) return NULL;
	return m_pGuardWall.GetAt(0);
}

/**	\brief	The *CPlateBridgeApp::GetGuardFencRight function
	���� ��ȣ����ü�� �����͸� ������
	\return	CCentSeparation ��ȣ���� �������� ������ NULL	
*/
CCentSeparation *CPlateBridgeApp::GetGuardFencRight()
{
	if(!IsExistRightGuardFence()) return NULL;
	return m_pGuardWall.GetAt(m_pGuardWall.GetUpperBound());
}

/**	\brief	The CPlateBridgeApp::GetCountOfCenterGuardFence function
	�ߺд��� ������ ������
	\return	long �ߺд��� ����	
*/
long CPlateBridgeApp::GetCountOfCenterGuardFence()
{	
	long nGuardCount = 0;
	for(long nHDan = 1; nHDan < GetQtyHDanNode()-1; nHDan++)
	{
		if(IsTypeGuardFenceDom(nHDan))	++nGuardCount;		
	}
	return nGuardCount;
}

/**	\brief	The CPlateBridgeApp::IsExistRightGuardFence function
	���� �ߺд밡 �����ϴ°��� �˻�
	\return	BOOL �����ϸ� TRUE �׷��� ������ FALSE
*/
BOOL CPlateBridgeApp::IsExistCenterGuardFence()
{
	return (GetCountOfCenterGuardFence()==0 ? FALSE : TRUE);
}

/**	\brief	The *CPlateBridgeApp::GetGuardFencCenter function
	�ߺд��� ��ȣ���� ��ü�� ������
	\param	nIdx	�ߺд밡 �������� ��� �������� 0
	\return	CCentSeparation ��ȣ�� ��ü
*/
CCentSeparation *CPlateBridgeApp::GetGuardFencCenter(long nIdx)
{
	long nStt = IsExistLeftGuardFence() ? 1 : 0;
	return m_pGuardWall.GetAt(nStt + nIdx);
}

// �����߽� ���� ���� ���������� ���� ���� R �� ����(ũ�μ��̵� ����)
//   ������ ��� ���� ������ ��쿡�� R = 0 �� ������.
double CPlateBridgeApp::GetRadiusMinTotalBridge() const
{
	double dStationBridgeEnd = m_dStationBridgeStt + GetLengthBridge();
	double dRadiusMinTotalBridge = m_pLine->GetRadiusMin(m_dStationBridgeStt, dStationBridgeEnd);
	
	return dRadiusMinTotalBridge;
}


void CPlateBridgeApp::GetXySlabAct(double sta, CDPoint A[HDANLINESU]) const
{
	GetXyPaveAct(sta, A);
	double thickPave = GetThickPave();
	for(long n = 0; n < GetQtyHDanAct(sta)+1; n++)
		A[n].y -= thickPave;


	return;
}


void CPlateBridgeApp::GetXyFrontCantilever(CPlateBasicIndex *pBx, CDPoint* A, BOOL bLeft) 
{
	CDPoint APave[HDANLINESU]; // �������� ����Ʈ	
	CDPoint BHunch[10]; // ������ ��ġ����  ����Ʈ
	CDPoint vX(1,0), vY(0,1);
	long	n = 0;
	for(n=0; n<4; n++){
		A[n].x = 0;
		A[n].y = 0;
	}
	
	if(!pBx) return;

	long nG = bLeft ? 0 : GetGirdersu()-2;
	GetXyPaveDom(pBx, APave);//
	GetGirder(nG)->GetHunchXyDom(pBx,BHunch, FALSE, FALSE, IsTUGir());

	//////////////////////////////////////////////////////////////////////////
	CPlateGirderApp	 *pGirL	= GetGirder(0);
	CPlateGirderApp	 *pGirR	= GetGirder(GetQtyGirder()-1);
	CPlateBasicIndex *pBxL	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirL);
	CPlateBasicIndex *pBxR	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirR);
	CPlateBxFinder FinderL(pGirL);
	CPlateBxFinder FinderR(pGirR);
	if(pBx->IsState(BX_STT_SLAB))
	{
		pBxL = FinderL.GetBxFirst(BX_STT_SLAB);
		pBxR = FinderR.GetBxFirst(BX_STT_SLAB);
	}
	if(pBx->IsState(BX_END_SLAB))
	{
		pBxL = FinderL.GetBxFirst(BX_END_SLAB);
		pBxR = FinderR.GetBxFirst(BX_END_SLAB);
	}
	double dHTaper	= pBx->GetGirder()->GetHeightTaper(pBx->GetStation());
	double dHTaperL = pBxL ? pGirL->GetHeightTaper(pBxL->GetStation()) : dHTaper;
	double dHTaperR = pBxR ? pGirR->GetHeightTaper(pBxR->GetStation()) : dHTaperL;
	//////////////////////////////////////////////////////////////////////////	
	
	double dTPave     = GetThickPave();
	double dTSlabLeft = GetSlabThick(CPlateBridgeApp::SLAB_LEFT);
	double dTSlabRigh = GetSlabThick(CPlateBridgeApp::SLAB_RIGHT);
	long   nHDansu    = GetQtyHDanDom(pBx);

	for( n=0; n<HDANLINESU; n++)
		APave[n].y -= dTPave;

	APave[0].y -= (dTSlabLeft + dHTaperL);			// �������� �Ʒ��κ���ġ
	APave[nHDansu].y -=  (dTSlabRigh + dHTaperR);

	if(bLeft)
	{
		if(m_bUserSlabHunch && m_dHunchArray.GetSize()>0)
		{
			// ptLeft : ����ĵƿ���� ��ġ��ġ
			CDPoint ptLeft = CDPoint(BHunch[0].x - m_dHunchArray[0], GetElevationPaveDomByDis(pBx, BHunch[0].x - m_dHunchArray[0]) - m_dThickPave - m_dThickSlabLeftExt - dHTaper);
			A[0] = APave[0];
			A[1] = ptLeft;
			A[2] = BHunch[0];
			A[3] = BHunch[1];
		}
		else
		{
			A[0] = APave[0];
			A[1] = APave[0];
			A[2] = BHunch[0];
			A[3] = BHunch[1];
		}
	}
	else
	{
		if(m_bUserSlabHunch && m_dHunchArray.GetSize()>0)
		{
			// ptRight : ����ĵƿ���� ��ġ��ġ
			CDPoint ptRight = CDPoint(BHunch[9].x + m_dHunchArray.GetAt(GetGirdersu()*2-1), GetElevationPaveDomByDis(pBx, BHunch[9].x + m_dHunchArray[m_dHunchArray.GetUpperBound()]) - m_dThickPave - m_dThickSlabRightExt - dHTaper);			
			A[0] = APave[nHDansu];
			A[1] = ptRight;
			A[2] = BHunch[9];
			A[3] = BHunch[8];
		}
		else
		{
			A[0] = APave[nHDansu];
			A[1] = APave[nHDansu];
			A[2] = BHunch[9];
			A[3] = BHunch[8];
		}
	}
}


CDPoint CPlateBridgeApp::GetMatchRebarPointDom(CPlateBasicIndex *pBxGir, CDPoint xy, double slope, double dSpace, BOOL bLowBySlabHunch) const
{
	return GetMatchRebarPoint(FALSE, bLowBySlabHunch, xy, slope, 0,dSpace, pBxGir);
}


CDPoint CPlateBridgeApp::GetMatchRebarPoint(BOOL bActual, BOOL bLowBySlabHunch, CDPoint xy, double slope, double staCR, double dSpace, CPlateBasicIndex *pBx /*= NULL*/) const
{
	if(bLowBySlabHunch == TRUE && m_dThickSlabHunch == 0)	return xy;
	CDPoint vAng = CDPoint(1, slope);
	if(slope == 0)	vAng = CDPoint(0, 1);//90��
	CDPoint xyResult(0,0);
	CDPoint A[HDANLINESU];
	GetXyPave(bActual, A, staCR, pBx);

	long n = 0;
	for(n=0; n<HDANLINESU; n++) A[n].y -= GetThickPave() + dSpace;
	long nQtyHDanReal = GetQtyHDanReal(bActual, staCR, pBx);
	if(bActual)
	{
		pBx = GetGirder(-1)->GetBxByStation(staCR);
	}
	if(bLowBySlabHunch)
	{
		double slabT = 0;
		if(bActual)	slabT = GetThickPave() + m_dThickSlabBase  + pBx->GetGirder()->GetHeightTaper(staCR) - m_dThickSlabHunch; //80+300-50 = 330
		else		slabT = GetThickPave() + m_dThickSlabBase  + pBx->GetGirder()->GetHeightTaper(pBx->GetStation()) - m_dThickSlabHunch; //80+300-50 = 330
		for(long n=0; n <= nQtyHDanReal; n++)		//������ �������� 1�� ����.
			A[n].y -= slabT;
	}
	for(n = 0; n < nQtyHDanReal; n++)
		if(GetXyMatchSegAndLine(A[n],A[n+1],xy,vAng,xyResult))		break;

	return xyResult;
}


long CPlateBridgeApp::GetsuCrossBeamBxHasHunch()
{
	long nSumCrHunch = 0;
	for (long nG=0; nG<GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{		
			if(pGir->IsCrossBeamBxHasHunch(pBx))
				nSumCrHunch++;

			pBx = Finder.GetBxNext();
		}
	}

	return nSumCrHunch;
}

BOOL CPlateBridgeApp::IsUserSlabHunch(BOOL bLeft, CPlateBasicIndex *pBx)
{
	return FALSE;
	double dUFWidth = GetGirder(bLeft ? 0 : GetGirdersu()-1)->GetWidthFlangeUpper(pBx);
	if(m_bUserSlabHunch)
	{
		if(bLeft && m_dWidthSlabLeft - dUFWidth/2 - m_dWidthHunch - m_dHunchArray.GetAt(0)==0) return FALSE;
		if(!bLeft && m_dWidthSlabRight - dUFWidth/2 - m_dWidthHunch - m_dHunchArray.GetAt(m_dHunchArray.GetUpperBound())==0) return FALSE;
	}
	return m_bUserSlabHunch;
}

void CPlateBridgeApp::SetQtyJijumStiff(long nJijum, long nQtyJijumStiff)
{
	m_nQtyJijumStiff[nJijum] = nQtyJijumStiff;
}

long CPlateBridgeApp::GetQtyJijumStiff(long nJijum)
{
	return m_nQtyJijumStiff[nJijum];
}

void CPlateBridgeApp::SetInsultionVStiff()
{
	if(!IsTUGir())	return;

	long nQty = GetQtyGirder();
	for(long nG=0; nG<nQty; nG++)
	{
		CPlateGirderApp *pGir = GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_VSTIFF|BX_CROSSBEAM_VBRACING);

		while(pBx)
		{
			CVStiff *pVStiff = pBx->GetSection()->GetVStiff();
			if(pVStiff && !pBx->IsJijum())
			{
				pVStiff->m_dH[LEFT][UP]	 = m_pDataManage->GetGlobalOption()->GetInsulationVStiffUF();
				pVStiff->m_dH[RIGHT][UP] = m_pDataManage->GetGlobalOption()->GetInsulationVStiffUF();
			}
			pBx = Finder.GetBxNext();
		}		
	}
}

void CPlateBridgeApp::SetTendonArcType(CPlateBasicIndex *pBx)
{
	double	dDistTotal	=	0.0;
	CDPoint ptCen1,		ptCen2,	ptCen3;
	CDPoint ptCenter,	ptCenter2;
	CDPoint xyMatch1,	xyMatch2;
	CDPoint xyResult1,	xyResult2;
	CDPoint A[HDANLINESU];
	
	GetXyPaveUpperDom(pBx, A);	

	double	dLDist	=	GetSlabLeft()->m_dGirderCenter;
	double	dRDist	=	GetSlabRight()->m_dGirderCenter;
	CDPoint	ptStt	=	CDPoint(A[0].x, GetElevationPaveActByDis(pBx->GetStation(), dLDist) - m_dThickPave - m_dHeightLeftTension);	
	CDPoint ptEnd	=	CDPoint(A[GetQtyHDanNode()].x, GetElevationPaveActByDis(pBx->GetStation(), dRDist) - m_dThickPave - m_dHeightRightTension);	

	for(long n=0; n<m_nSlabTensionSize; n++)
	{
		double	dL		=	m_SlabTension[n].m_dL;
		double	dH		=	m_SlabTension[n].m_dH;

		dDistTotal += dL;

		if(n==0)	ptCen1	=	ptStt;	
		else		ptCen1	=	ptCen2;
		
		ptCen2	=	CDPoint(ptStt.x + dDistTotal, GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal) - m_dThickPave - dH);
		ptCen3	=	CDPoint(ptStt.x + dDistTotal + m_SlabTension[n+1].m_dL, GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal + m_SlabTension[n + 1].m_dL) - m_dThickPave - m_SlabTension[n + 1].m_dH);
		
		if(n == m_nSlabTensionSize - 1)
			ptCen3	=	ptEnd;
		
		CDPoint vDir1	=	(ptCen1 - ptCen2).Unit();
		CDPoint vDir2	=	(ptCen3 - ptCen2).Unit();
		CDPoint vDir3	=	(vDir1 + vDir2).Unit();
		
		if(vDir3.y <0 )
			m_SlabTension[n].m_nType = 0;
		else
			m_SlabTension[n].m_nType = 1; 
	}
}

// ��������� ���� ������ ����
void CPlateBridgeApp::GetRegulationSteelMaterial(long nMaterial, CString szNameSteel, CString &szLeft, CString &szRight)
{
	CString	szFir		= _T("");
	CString	szMid		= _T("");
	CString	szSec		= _T("");
	CString	cs			= _T("");

	if(nMaterial==G_F_U || nMaterial==G_F_L || nMaterial==G_W)
	{
		long nLength = szNameSteel.GetLength();

		for(long i = 0; i < nLength; i++)
		{
			cs = szNameSteel.GetAt(i);
			if(cs == "0" || cs == "1" || cs == "2" || cs == "3" || cs == "4" ||
			   cs == "5" || cs == "6" || cs == "7" || cs == "8" || cs == "9")
			{
				szMid += cs;			
				continue;
			}
			if(szMid.GetLength()>0)	szSec += cs;
			else					szFir += cs;
		}	
	}
	else
	{
		szFir	= GetMaterialNumber(nMaterial, -1);
		szMid	= GetMaterialNumber(nMaterial);
		szSec	= GetMaterialNumber(nMaterial, 1);
	}

	if(szSec.Find("C-H")!=-1 || szSec.Find("-H")!=-1 || szSec.Find("Y")!=-1 || szSec.Find("TMC")!=-1)
	{
		szLeft	= szFir+szMid+szSec;
		szRight	= _T("");
	}
	else if(szSec.Find("YA")!=-1 || szSec.Find("YB")!=-1)
	{
		szLeft	= szFir+szMid+_T("Y");
		if(szSec == _T("YA"))	szRight	= _T("A");
		else					szRight	= _T("B");
	}
	else if(szSec == _T(""))
	{
		szLeft	= szFir+szMid;
		szRight	= _T("");
	}
	else
	{
		szLeft	= szFir+szMid;
		szRight	= szSec;
	}
	
	//DB���� �� ����ó��.
	if(szLeft.Find("SWS400") > -1)
	{
		szLeft = "SM400";
		szRight = "B";
	}
	
	else if(szLeft.Find("SWS490") > -1)
	{
		szLeft	= "SM490";
		szRight = "B";
	}
	else if(szLeft.Find("SWS490Y") > -1 || szLeft.Find("SWS520") > -1)
	{
		szLeft = "SMA490";
		szRight = "B";
	}
	else if(szLeft.Find("SWS570") > -1)
	{
		szLeft = "SMA570";
		szRight = "B";
	}

	if((szLeft == "SM400"  || szLeft == "SM490"		||
		szLeft == "SM490Y" || szLeft == "SMA400"	||
	    szLeft == "SMA490" || szLeft == "SMA570")	&& 
		szRight.GetLength() == 0)
	{
			szRight = "B";
	}
	//
}



void CPlateBridgeApp::GetIndexGirderToCrossBeam(CLongArray &nArrGir, CPlateBasicIndex *pBx, BOOL bPartDim)
{
	CPlateBridgeApp *pDB   = (CPlateBridgeApp*)this;


	nArrGir.RemoveAll();
	BOOL bFirstGir = TRUE;
	long nGirsu = pDB->GetQtyGirder();

	CPlateBasicIndex *pBxGir  = pBx;
	for (long nG = 0; nG < nGirsu; nG++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(nG);
		CPlateGirderApp* pGirR = pDB->GetGirder(nG+1);
		if(pBxGir == NULL) continue;
		if(pGirR == NULL) continue;

		CPlateBasicIndex *pBxGirR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBxGir, pGirR);		
		if(pBxGir &&  pBxGirR == NULL)
		{
			if(bPartDim)
				nArrGir.Add(nG);
			if(nG == 0 && bFirstGir == TRUE)
				bFirstGir = FALSE;

			break;	
		}
		else if(pBxGirR == NULL || pBxGir == NULL)
		{
			if(nG == 0 && bFirstGir == TRUE)
			{
				bFirstGir = FALSE;
				break;
			}

			pBxGir = pBx;
			continue;
		}


		nArrGir.Add(nG);

		pBxGir = pBxGirR;
	}


	if(bFirstGir == FALSE)
	{
		nArrGir.RemoveAll();

		CPlateBasicIndex *pBxGir  = pBx;
		for (long nG = nGirsu-1; nG > -1; nG--)
		{
			CPlateGirderApp* pGir = pDB->GetGirder(nG);
			CPlateGirderApp* pGirR = pDB->GetGirder(nG-1);
			if(pBxGir == NULL) continue;
			if(pGirR == NULL) continue;

			CPlateBasicIndex *pBxGirR = (CPlateBasicIndex *)pGir->GetBxMatchByCrossBeam(pBxGir, pGirR);		
			if(pBxGir &&  pBxGirR == NULL)
			{
				nArrGir.Add(nG);
				break;
			}
			else if(pBxGir == NULL && pBxGirR)
			{
				pBxGir = pBx;
				continue;
			}

			nArrGir.Add(nG);

			pBxGir = pBxGirR;
		}

		nArrGir.Sort();
	}
}