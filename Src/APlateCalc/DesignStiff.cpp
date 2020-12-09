// DesignStiff.cpp: implementation of the CDesignStiff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ������ ������ - Web������(����.����)
CDesignStiff::CDesignStiff(CAPlateCalcStd *pAPlateCalcStd)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
}

CDesignStiff::~CDesignStiff()
{

}

void CDesignStiff::SetVerStiff(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp		*pDB			= m_pAPlateCalcStd->m_pDataManage->GetBridge();	
	CDataManage			*pDataManage	= m_pAPlateCalcStd->m_pDataManage;
	CCalcGeneral		CalcGeneral(pDataManage);

	long nType	= 0;
	CString strLeft, strRight;
	double dSta	= pBx->GetStation();

	if(pBx->IsJijum())
	{
		m_strtVerStiff.m_dMaxA_B = 1.5;								//���ݺ� �ִ�..
		nType = CP_V;
	}
	else
	{
		m_strtVerStiff.m_dMaxA_B = 3.0;								//���ݺ� �ִ�..
		nType = CG_V;
	}

	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateBxFinder		Finder(pGir);

	CPlateBasicIndex	*pBx1			= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING | BX_VSTIFF);
	CPlateBasicIndex	*pBxCross		= pBx1;
	double				dGapJijum		= 0.0;
	double				dTerm			= 0.0;
	
	//������������ ������ ù��° ���κ��� ��ġ�ϴ� ������������ ��ġ�� ��.(������ ���)
	if(pBx->IsJijum())
	{
		while (pBx1 = Finder.GetBxNext())
		{
			if (pBx1->IsState(BX_CROSSBEAM_VBRACING))
			{
				dGapJijum = pBx1->GetStation() - pBxCross->GetStation() - dTerm;
				break;
			}
			else
				dTerm = pBx1->GetStation() - pBxCross->GetStation();
		}
	}
	
	CalcGeneral.GetMaterial(pBx, nType,strLeft,strRight);
	m_strtVerStiff.m_strMaterial = strLeft;							//���������� ����
	m_strtVerStiff.m_dGap	 = pBx->IsJijum() ?  dGapJijum : pDB->m_dLengthBaseVStiff;		//���������� ���� a
													
	CVStiff	*pVStiff = pGir->GetBxByStation(dSta, BX_VSTIFF)->GetSection()->GetVStiff();

	m_strtVerStiff.m_dFlangeGap	= pGir->GetHeightGirderByStaAng(dSta);		//���� ���÷����� ������ b
	m_strtVerStiff.m_dWebThick	= pGir->GetThickJewon(G_W, pBx);	//�������� �β� t

	m_strtVerStiff.m_dUseA_B = m_strtVerStiff.m_dGap/m_strtVerStiff.m_dFlangeGap; //��� ���ݺ�..
	
	m_strtVerStiff.m_dUseW = max(pVStiff->m_dWidth[0], pVStiff->m_dWidth[1]);		//��� ���������� �� bvs
	m_strtVerStiff.m_dReqW = (m_strtVerStiff.m_dFlangeGap/30.0)+50.0;					//�ʿ� ���������� ��
	
	m_strtVerStiff.m_dUseT = max(pVStiff->m_dThick[0], pVStiff->m_dThick[1]);		//��� ���������� �β� tvs
	m_strtVerStiff.m_dReqT = m_strtVerStiff.m_dUseW/13.0;						//�ʿ� ���������� �ּҵβ�

	double dGamma = 8.0*pow(m_strtVerStiff.m_dFlangeGap/m_strtVerStiff.m_dGap,2);//8.0*(b/a)^2
	//�ʿ� ����
	m_strtVerStiff.m_dReqI = (m_strtVerStiff.m_dFlangeGap*pow(m_strtVerStiff.m_dWebThick,3)*dGamma)/10.92;//bt^3/10.92	
	//��� ����
	m_strtVerStiff.m_dUseI = (1.0/3.0)*m_strtVerStiff.m_dUseT*pow(m_strtVerStiff.m_dUseW,3);//1/3*t��vs*bvs^3
}

void CDesignStiff::SetHorStiff(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp		*pDB			= m_pAPlateCalcStd->m_pDataManage->GetBridge();	
	CDataManage			*pDataManage	= m_pAPlateCalcStd->m_pDataManage;
	CCalcGeneral		CalcGeneral(pDataManage);
	CSteel				*pSteelCtl		= m_pAPlateCalcStd->m_pDataManage->GetSteel();
	long nG = pBx->GetNumberGirder();
	long nType=0;
	CString strLeft, strRight;
	double dSta=pBx->GetStation();

	if(pBx->IsJijum())
	{		
		nType = CP_H;
	}
	else
	{		
		nType = CG_H;
	}

	CalcGeneral.GetMaterial(pBx, nType,strLeft,strRight);
	m_strtHorStiff.m_strMaterial = strLeft;					//���򺸰��� ����
	m_strtHorStiff.m_dGap = pDB->m_dLengthBaseVStiff;		//���������� ���� a
	
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CPlateGirderApp	*pGir = pDB->GetGirder(nG);
	CHGAllowStress	AllowMinThick(pSteel);

	m_strtHorStiff.m_dFlangeGap	= pGir->GetHeightGirderByStaAng(dSta);//���Ͼ��÷����� ������ b
	
	m_strtHorStiff.m_dUseW = max(pBx->GetWebHStiff()->m_dWidthHStiff[0], pBx->GetWebHStiff()->m_dWidthHStiff[1]); //��� ���򺸰��� �� bhs
	m_strtHorStiff.m_dUseT = max(pBx->GetWebHStiff()->m_dThickHStiff[0], pBx->GetWebHStiff()->m_dThickHStiff[1]);//��� ���򺸰��� �β� ths		

	m_strtHorStiff.m_dUseWebT = pGir->GetThickJewon(G_W, pBx);//�������� ��� �β� t	
		
	m_strtHorStiff.m_lUse_EA = max(pBx->GetWebHStiff()->GetTotalDansu(TRUE,TRUE), pBx->GetWebHStiff()->GetTotalDansu(TRUE,FALSE));//���򺸰��� ��� �ܼ�

	double dMinThick	= AllowMinThick.GetAllowStress(ALLOW_PLATEBRIDGE_WEBMINTHICK, m_strtHorStiff.m_lUse_EA);	//������ �ּҵβ�		
	m_strtHorStiff.m_dReqWebT = m_strtHorStiff.m_dFlangeGap/dMinThick; //������ �ʿ� �ּҵβ�  = b / ������ �ּҵβ�		

	double dGamma = 30.0*pow(m_strtHorStiff.m_dGap/m_strtHorStiff.m_dFlangeGap,2);//
	m_strtHorStiff.m_dReqI = (m_strtHorStiff.m_dFlangeGap*pow(m_strtHorStiff.m_dUseWebT,3)*dGamma)/10.92;//bt^3/10.92
	m_strtHorStiff.m_dUseI = (1.0/3.0)*m_strtHorStiff.m_dUseT*pow(m_strtHorStiff.m_dUseW,3);//1/3*tvs*bvs^3

	//��ġ��ġ�� ������...
	if(m_strtHorStiff.m_lUse_EA == 1)
		m_strtHorStiff.m_sReqPlace.Format("'%.0f", 0.2*m_strtHorStiff.m_dFlangeGap);
	else
		m_strtHorStiff.m_sReqPlace.Format("'%.0f,%.0f", 0.14*m_strtHorStiff.m_dFlangeGap, 0.36*m_strtHorStiff.m_dFlangeGap);
}

/////////////////////////////////////////////////////////////////
// ������ ������
CDesignJijumStiff::CDesignJijumStiff(CAPlateCalcStd *pAPlateCalcStd, CPlateBasicIndex *pBx, long nJoint)
{
	m_pAPlateCalcStd	= pAPlateCalcStd;
	m_pBx				= pBx;
	m_nJoint			= nJoint;
	SetJijumStiff();
	SetJackupStiff();// JackUp������
}

CDesignJijumStiff::~CDesignJijumStiff()
{
}


void CDesignJijumStiff::SetJijumStiff()
{
	CPlateBridgeApp		*pDB			= m_pAPlateCalcStd->m_pDataManage->GetBridge();	
	CDataManage			*pDataManage	= m_pAPlateCalcStd->m_pDataManage;
	CCalcGeneral		CalcGeneral(pDataManage);
	CFemStressCalc		StressCalc(pDataManage, m_pAPlateCalcStd);	
	CResultArray		*pResult		= m_pAPlateCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	CSteel				*pSteelCtl		= m_pAPlateCalcStd->m_pDataManage->GetSteel();

	long			 nG		= m_pBx->GetNumberGirder();	
	CPlateGirderApp	 *pGir	= pDB->GetGirder(nG);

	CString strLeft, strRight;
	double dSta=m_pBx->GetStation();		
	
	CalcGeneral.GetMaterial(m_pBx, V_J_ST,strLeft,strRight);
	
	CHGBaseSteel		*pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress		AllowBucklingUnConsider(pSteel);	
	CHGAllowStress		AllowBuckling(pSteel);
	CHGAllowStress		AllowTension(pSteel);
	CVStiff	*pVStiff = pGir->GetBxByStation(m_pBx->GetStation(), BX_JIJUMSTIFF)->GetJijumStiff();

	long	nJijum	= pGir->GetNumberJijumByBx(m_pBx);
	long	nJack	= 0;

	
	m_strJijumStiff.m_strMaterial = strLeft;

	double	dReaction	= 0;
	BOOL	bPositive	= StressCalc.IsPositive(ELE_MOMENT3, nG, m_pBx->GetStation());
	CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(m_nJoint);

	if(!bPositive)	dReaction = pJResult->m_vTotalMax[JOINT_REACTION].z;
	else			dReaction = pJResult->m_vTotalMin[JOINT_REACTION].z;
	m_strJijumStiff.m_dReaction = dReaction;//������ �ݷ�

	double dHGir = pGir->GetHeightGirderByStaAng(dSta);
	m_strJijumStiff.m_dWidth = max(pVStiff->m_dWidth[0], pVStiff->m_dWidth[1]);//������ �� b
	m_strJijumStiff.m_dLength = max(pVStiff->GetVStiffHeight(TRUE, dHGir), pVStiff->GetVStiffHeight(FALSE, dHGir)); //���� l
	m_strJijumStiff.m_dUseT = max(pVStiff->m_dThick[0], pVStiff->m_dThick[1]);//��� �β�..ts
	m_strJijumStiff.m_dReqT = m_strJijumStiff.m_dWidth/16.0;//���� 3.4.2.2
	m_strJijumStiff.m_dGap = ABS(pDB->m_dJiJumTerm[nJijum][nJack]);//��ġ����.. d
	m_strJijumStiff.m_dUse_EA = pDB->GetQtyJijumStiff(nJijum);//pGir->GetQtyRowJijumVStiffByBx(m_pBx);//��뿭��
	m_strJijumStiff.m_dWebT	= pGir->GetThickJewon(G_W, m_pBx);//������ �β�..tw

	double b = m_strJijumStiff.m_dWidth;
	double l = m_strJijumStiff.m_dLength;
	double ts = m_strJijumStiff.m_dUseT;
	double tw = m_strJijumStiff.m_dWebT;
	double n = m_strJijumStiff.m_dUse_EA;
	double d = m_strJijumStiff.m_dGap;
	
	//�ܸ���ȿ��
	if((24.0*tw) >= d)
	{
		m_strJijumStiff.m_dDanW_Be = 2.0*12.0*tw + (n-1)*d;
	}
	else
	{
		m_strJijumStiff.m_dDanW_Be =2.0*12.0*tw + (n-1)*24.0*tw;
	}
	
	//�������� �ܸ���..
	m_strJijumStiff.m_dDanArea_As = b*ts*n*2.0; //mm2

	double Be = m_strJijumStiff.m_dDanW_Be;
	double As = m_strJijumStiff.m_dDanArea_As;
	//��ȿ�ܸ���..
	double Ae = Be*tw + As;
	Ae = min(Ae,(1.7*As));
	m_strJijumStiff.m_dReqArea_Ae = Ae;

	//������ �߽ɼ� �ѷ��� �ܸ�2�����Ʈ
	double I =(Be*pow(tw,3)/12.0) + ((ts*pow(b,3)/12.0)+(ts*b*pow((b+tw)/2.0,2)))*n*2;//mm4
	m_strJijumStiff.m_dMom_I = I;
	//�ܸ�2���ݰ�
	double r =sqrt(I/Ae);
	m_strJijumStiff.m_dDan_r = r;

	//��ȿ�±�����
	double le =l/2.0;
	m_strJijumStiff.m_dBuckLing_le = le;

	//�������� ��������
	
	AllowBucklingUnConsider.SetAllowBucklingUnconsider(le, r);	
	double Fcag = AllowBucklingUnConsider.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, ts);
	m_strJijumStiff.m_dFcag = Fcag;
	AllowBuckling.SetAllowBuckling(ts, b);
	double Fcal = AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY,ts);
	m_strJijumStiff.m_dFcal=Fcal;	
	double Fcao = AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, ts);
	m_strJijumStiff.m_dFcao = Fcao;

	double Fca = Fcag*Fcal/Fcao;
	m_strJijumStiff.m_dFca = Fca;

	double Fc = dReaction*1000/Ae;
	m_strJijumStiff.m_dFc = Fc;
}


void CDesignJijumStiff::SetJackupStiff()
{
	CPlateBridgeApp		*pDB			= m_pAPlateCalcStd->m_pDataManage->GetBridge();	
	CDataManage			*pDataManage	= m_pAPlateCalcStd->m_pDataManage;
	CCalcGeneral		CalcGeneral(pDataManage);
	CFemStressCalc		StressCalc(pDataManage, m_pAPlateCalcStd);	
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	
	long			nG		= m_pBx->GetNumberGirder();	
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);

	CString strLeft, strRight;
	
	CalcGeneral.GetMaterial(m_pBx, V_J_ST,strLeft,strRight);

	CHGBaseSteel		*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress		AllowBucklingUnConsider(pSteel);	
	CHGAllowStress		AllowBuckling(pSteel);
	CHGAllowStress		AllowTension(pSteel);

	long	nJijum	= pGir->GetNumberJijumByBx(m_pBx);
	long	nJack	= 0;

	CJackupStiff *pJackUp = pGir->GetJackupByJijum(nJijum, 0);
	if(pJackUp == NULL) return;

	
	m_strJackupStiff.m_strMaterial = strLeft;

	double	dReaction	= m_pAPlateCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(m_nJoint)->m_vPreDeadSteel[JOINT_REACTION].z+
						  m_pAPlateCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(m_nJoint)->m_vPreDeadSlab[JOINT_REACTION].z+
						  m_pAPlateCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(m_nJoint)->m_vPostDead[JOINT_REACTION].z;
	double	dRl			= fabs(m_pAPlateCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(m_nJoint)->m_vLiveMin[JOINT_REACTION].z);
	//CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(m_nJoint);
	//if(bPositive)	dReaction = pJResult->m_vTotalMax[JOINT_REACTION].z;
	//else			dReaction = pJResult->m_vTotalMin[JOINT_REACTION].z;
	m_strJackupStiff.m_dReaction = dReaction;//������ �ݷ�

	m_strJackupStiff.m_dWidthL = pJackUp->GetWidth(TRUE);//������ �� b1
	m_strJackupStiff.m_dWidthR = pJackUp->GetWidth(FALSE);//������ �� b2
	m_strJackupStiff.m_dLength = pJackUp->GetHeight(TRUE); //���� l
	m_strJackupStiff.m_dUseT = pJackUp->GetThick(TRUE); //��� �β�..ts
	m_strJackupStiff.m_dReqT = m_strJackupStiff.m_dWidthR/16.0;//���� 3.4.2.2
	m_strJackupStiff.m_dGap = ABS(pDB->m_dJackUpTerm[nJijum][nJack]);//��ġ����.. d	
	m_strJackupStiff.m_dUse_EA = pGir->GetQtyRowJackupByBx(m_pBx);//��뿭��	
	m_strJackupStiff.m_dWebT	= pGir->GetThickJewon(G_W, m_pBx);//������ �β�..tw
	m_strJackupStiff.m_dQtyOneShoe = GetJackupBuildsuByOneShoe(m_pBx);//SHOE 1���Ҵ� JACK UP��ġ ��ġ����	

	double b1 = m_strJackupStiff.m_dWidthL;
	double b2 = m_strJackupStiff.m_dWidthR;	
	double l = m_strJackupStiff.m_dLength;
	double ts = m_strJackupStiff.m_dUseT;	
	double tw = m_strJackupStiff.m_dWebT;
	double n = m_pBx->IsPier() ? m_strJackupStiff.m_dUse_EA - 1 : m_strJackupStiff.m_dUse_EA;
	double d = m_strJackupStiff.m_dGap;
	double nQty = m_strJackupStiff.m_dQtyOneShoe;
	
	//���� ����...
	double P = (dReaction + 1.5 * dRl) / nQty;
	m_strJackupStiff.m_dApplyLoad_P = P;
	//�ܸ���ȿ��
	if((24.0*tw) >= d)
	{
		m_strJackupStiff.m_dDanW_Be = 2.0*12.0*tw + (n-1)*d;
	}
	else
	{
		m_strJackupStiff.m_dDanW_Be =2.0*12.0*tw + (n-1)*24.0*tw;
	}
	
	//�������� �ܸ���..
	m_strJackupStiff.m_dDanArea_As = b1*ts*n + b2*ts*n; //mm2

	double Be = m_strJackupStiff.m_dDanW_Be;
	double As = m_strJackupStiff.m_dDanArea_As;
	
	//��ȿ�ܸ���..
	double Ae = Be*tw + As;
	Ae = min(Ae,(1.7*As));
	m_strJackupStiff.m_dReqArea_Ae = Ae;
	//�߸����� ��ġ
	double Yo = (n*(b2*ts*b2/2.0+b1*ts*(b2+tw+b1/2.0))+Be*tw*(b2+tw/2.0))/Ae;
	m_strJackupStiff.m_dPos_Yo = Yo;

	//������ �߽ɼ� �ѷ��� �ܸ�2�����Ʈ
	double I =Be*pow(tw,3)/12.0 + Be*tw*pow(ABS(b2+tw/2.0 - Yo),2) + 
			  (ts*pow(b2,3)/12.0 + ts*pow(b2,3)/12.0 + ts*Yo*pow(ABS(ts/2.0-b1),2) +
			  b1*ts*pow(ABS(b2+tw+b1/2.0-Yo),2))*n;//mm4
	m_strJackupStiff.m_dMom_I = I;
	//�ܸ�2���ݰ�
	double r =sqrt(I/Ae);
	m_strJackupStiff.m_dDan_r = r;

	//��ȿ�±�����
	double le =l/2.0;
	m_strJackupStiff.m_dBuckLing_le = le;

	//�������� ��������
	
	AllowBucklingUnConsider.SetAllowBucklingUnconsider(le, r);	
	double Fcag = AllowBucklingUnConsider.GetAllowStress(ALLOW_BUCKLING_UNCONSIDER, ts);
	m_strJackupStiff.m_dFcag = Fcag;
	AllowBuckling.SetAllowBuckling(ts, b1);
	double Fcal = AllowBuckling.GetAllowStress(ALLOW_BUCKLING_FREEJETTY,ts);
	m_strJackupStiff.m_dFcal=Fcal;	
	double Fcao = AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, ts);
	m_strJackupStiff.m_dFcao = Fcao;

	double Fca = Fcag*Fcal/Fcao;
	m_strJackupStiff.m_dFca = Fca;

	double Fc = P*1000/Ae;
	m_strJackupStiff.m_dFc = Fc;
}


///< SHOE 1���Ҵ� JACK UP��ġ ��ġ���Ҽ� ���ϱ�
long CDesignJijumStiff::GetJackupBuildsuByOneShoe(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateCalcStd->m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir			= pBx->GetGirder();

	long	nJijum		= pGir->GetNumberJijumByBx(pBx);
	long	nGaesu		= 0;	
	double	dJackupDis	= 0;
	BOOL	bPlus		= FALSE;
	BOOL	bMinus		= FALSE;

	for(long nJackIdx = 0; nJackIdx < JACKUPSUMAX; nJackIdx++)
	{
		dJackupDis = pBridgeApp->m_dJackUpTerm[nJijum][nJackIdx];
		if(dJackupDis == 0) break;
		if(dJackupDis > 0)  bPlus = TRUE;
		else				bMinus = TRUE;
	}

	if(bPlus)	nGaesu++;
	if(bMinus)	nGaesu++;

	return nGaesu;
}