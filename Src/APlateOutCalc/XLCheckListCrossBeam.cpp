// XLCheckListCrossBeam.cpp: implementation of the CXLCheckListCrossBeam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplateoutcalc.h"
#include "APlateOutCalcStd.h"
#include "XLCheckListCrossBeam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLCheckListCrossBeam::CXLCheckListCrossBeam(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_pAPlateOutCalcStd	= pStd;
}

CXLCheckListCrossBeam::~CXLCheckListCrossBeam()
{

}


void CXLCheckListCrossBeam::CrossBeam()
{
	CXLControl			*pXL		= m_pXL;
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	

	m_nSttRow			= 1;	
	pXL->SetCellWidth(0, 255, 1);//
	
	pOut->SetXL(pXL, "������� CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "9. ���κ� ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);	


	CrossBeamGeneral(1);//��밭�� �� �ּҵβ� ����
	SetLines(NEXT);

	CrossBeamGeneral(2);//���°���
	SetLines(NEXT);
	CrossBeamGeneral(3);//3) �������� ����
	SetLines(NEXT);
	CrossBeamGeneral(4);//4) ����������
	SetLines(NEXT);
	CrossBeamGeneral(5);//5) ���򺸰���
	SetLines(NEXT);		

}

void CXLCheckListCrossBeam::CrossBeamHorVerBracing()
{
	CXLControl			*pXL		= m_pXL;
	CAPlateOutCalcXL	*pOut		= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	

	m_nSttRow			= 1;	
	pXL->SetCellWidth(0, 255, 1);//
	
	pOut->SetXL(pXL, "������� CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "9. �극�̽� ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);	
	
	CorssBeamBracing(6);//�����극�̽�....
	SetLines(NEXT);
	CorssBeamBracing(7);//�����극�̽�...
	SetLines(NEXT);
	HorBracing(8);//����극�̽�..
	SetLines(NEXT);

}

void CXLCheckListCrossBeam::CrossBeamGeneral(int nSeq)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	BOOL bOnlyFirst = TRUE;
		
	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++)//nG < 1;nG++)// 
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		CString sGubun;

		int nCrossCount = 1;
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();			
			if(pSec)
			{	
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				
				// 0=V�������극�̽�, 1=��V�������극�̽�, 2= ���κ�
				if(pSec->GetenDetType()==SECDET_CROSSBEAM) // ���κ� Ÿ��
				{	
					if (bOnlyFirst)
					{
						CheckListTitle(nSeq);
						bOnlyFirst = FALSE;
					}

					if(nCrossCount < 10)
						sGubun.Format("%dCR0%d", nG+1,nCrossCount);
					else
						sGubun.Format("%dCR%d", nG+1,nCrossCount);

					CrossBeamGeneralDetail(nSeq, pBx, sGubun);					
				}
			}	
			nCrossCount++;
			pBx = Finder.GetBxNext();
		}	
	}

	if(bOnlyFirst && nSeq == 1)
	{
		pOut->SetXL(pXL, "���κ� Ÿ���� �����ϴ�. ", m_nSttRow, "A", "A");		
	}
}

void CXLCheckListCrossBeam::CrossBeamGeneralDetail(int nSeq, CPlateBasicIndex *pBx, CString sGubun)
{
	CAPlateCalcStd			*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl				*pXL			= m_pXL;
	CAPlateOutCalcXL		*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CPlateCrossBeam			*pCrossBeam	= pBx->GetCrossBeam();
	CDesignCrossBeam		CrossBeam(pCalcStd, pBx);
	CDesignEndCrossBeam		*pEndCr			= NULL;
	CDesignCenterCrossBeam	*pCenCr			= NULL;	

	if(pBx->GetSection()->IsenType(SECTION_A))
		pEndCr	= (CDesignEndCrossBeam*)CrossBeam.GetDesignCrossBeam();
	else
		pCenCr	= (CDesignCenterCrossBeam*)CrossBeam.GetDesignCrossBeam();

	CString sText = _T("");
	
	switch(nSeq)
	{
		case 1:
		{		
			CString sMaterial	= _T("");
			long nQtyHorStiff	= 0;
			double dWebMinThick = 0;
			double dUpMinThick	= 0;
			double dLoMinThick	= 0;
			double dWebUseThick	= 0;
			double dUpUseThick	= 0;
			double dLoUseThick	= 0;

			if(pEndCr)
			{
				sMaterial		= pEndCr->m_pJewonFlangeLo->m_strMaterial;
				nQtyHorStiff	= pEndCr->m_nQtyHorStiff;
				dWebUseThick	= pEndCr->m_pJewonWeb->m_dWidth;
				dWebMinThick	= pEndCr->m_strtMinThick.m_dThickWebReq;
				dUpUseThick		= pEndCr->m_pJewonFlangeUp->m_dHeight;
				dUpMinThick		= pEndCr->m_strtMinThick.m_dMinThickFlangeUp;
				dLoUseThick		= pEndCr->m_pJewonFlangeLo->m_dHeight;
				dLoMinThick		= pEndCr->m_strtMinThick.m_dMinThickFlangeLo;						
			}
			if(pCenCr)
			{
				sMaterial		= pCenCr->m_pJewonFlangeLo->m_strMaterial;
				nQtyHorStiff	= pCenCr->m_nQtyHorStiff;
				dWebUseThick	= pCenCr->m_pJewonWeb->m_dWidth;
				dWebMinThick	= pCenCr->m_strtMinThick.m_dThickWebReq;
				dUpUseThick		= pCenCr->m_pJewonFlangeUp->m_dHeight;
				dUpMinThick		= pCenCr->m_strtMinThick.m_dMinThickFlangeUp;
				dLoUseThick		= pCenCr->m_pJewonFlangeLo->m_dHeight;
				dLoMinThick		= pCenCr->m_strtMinThick.m_dMinThickFlangeLo;
			}
			
				
			//���� ������ ����...
			pOut->SetLineBoxText(pXL, sGubun,		m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, sMaterial,	m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, nQtyHorStiff,	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "", 0);					
			pOut->SetLineBoxText(pXL, dWebMinThick, m_nSttRow, "M", "O",   1, BLACK, 9, FALSE, TA_CENTER, "Is1", 1);
			pOut->SetLineBoxText(pXL, dWebUseThick, m_nSttRow, "P", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Is2", 1);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is1"), pOut->GetCellRef("Is2"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dUpMinThick,	m_nSttRow, "U", "W",   1, BLACK, 9, FALSE, TA_CENTER, "Is3", 1);
			pOut->SetLineBoxText(pXL, dUpUseThick,	m_nSttRow, "X", "Z",   1, BLACK, 9, FALSE, TA_CENTER, "Is4", 1);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is3"), pOut->GetCellRef("Is4"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AA", "AB", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dLoMinThick,	m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER, "Is5", 1);
			pOut->SetLineBoxText(pXL, dLoUseThick,	m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "Is6", 1);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is5"), pOut->GetCellRef("Is6"));
			pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);	
			pOut->SetLineBoxText(pXL, "",			m_nSttRow, "AK", "AP", 1, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			break;
		}
		case 2://���°���
		{
			double dCompGen[6];
			double dCompWind[6];
			double dCompEarthQuake[6];
			double dCompFrameLoad[6];
			double dMaxStress[6];
			for (int n=0; n<6; n++)
			{
				dCompGen[n]			= 0;	
				dCompWind[n]		= 0;
				dCompEarthQuake[n]	= 0;
				dCompFrameLoad[n]	= 0;
				dMaxStress[n]		= 0;
			}
			double dMoment				= 0;//�ۿ���Ʈ
			double dShearForce			= 0;//�ۿ����ܷ�
			double dShearStress			= 0;//�����ۿ�����
			double dAllowShearStress	= 0;//�����������
			double dBendStressFu		= 0;//�����»�(�ۿ�)
			double dAllowBendStressFu	= 0;//�����»�(���)
			double dBendStressFl		= 0;//�������Ͽ�(�ۿ�)
			double dAllowBendStressFl	= 0;//�������Ͽ�(���)
			
			if(pEndCr)
			{
				dAllowBendStressFu		= pEndCr->m_strtSection.m_dFca;				
				dAllowBendStressFl		= pEndCr->m_strtSection.m_dSigmaca;
				dAllowShearStress		= pEndCr->m_strtSection.m_dSheara;
				long n = 0;
				for(n=0; n<6;n++)
				{
					dCompGen[n]			= pEndCr->m_strtFrameLoadComp.m_dCompGen[n];
					dCompWind[n]		= pEndCr->m_strtFrameLoadComp.m_dCompWind[n];
					dCompEarthQuake[n]	= pEndCr->m_strtFrameLoadComp.m_dCompEarthQuake[n];
					dCompFrameLoad[n]	= pEndCr->m_strtFrameLoadComp.m_dCompFrameLoad[n];
				}
				
				for (n=0; n<6; n++)
				{
					dMaxStress[n]	= max((max(dCompGen[n],dCompWind[n])),(max(dCompEarthQuake[n],dCompFrameLoad[n])));
				}
				dMoment			= tokNM(dMaxStress[0]);
				dShearForce		= tokN(dMaxStress[1]);
				dBendStressFu	= dMaxStress[3];
				dBendStressFl	= dMaxStress[4];
				dShearStress	= dMaxStress[5];
			}
			if(pCenCr)
			{
				dAllowBendStressFu	= pCenCr->m_strtSection.m_dFca;				
				dAllowShearStress	= pCenCr->m_strtSection.m_dSheara;
				dMoment				= tokNM(max(fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxMoment), 
											  fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinMoment)));
				dShearForce			= tokN(max(fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMaxShear), 
											  fabs(pCenCr->m_pFrameElementLoad->m_strtTotLoad.m_dMinShear)));
				dShearStress		= pCenCr->m_strtSection.m_dShear;
				dAllowShearStress	= pCenCr->m_strtSection.m_dSheara;
				dBendStressFu		= pCenCr->m_strtSection.m_dFu;
				dAllowBendStressFu	= pCenCr->m_strtSection.m_dFca;
				dBendStressFl		= pCenCr->m_strtSection.m_dFl;
				dAllowBendStressFl	= pCenCr->m_strtSection.m_dSigmaca;
				
			}

			pOut->SetLineBoxText(pXL, sGubun,				m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dMoment,				m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dShearForce,			m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER);			
			pOut->SetLineBoxText(pXL, dShearStress,			m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER, "Is1" , 3);
			pOut->SetLineBoxText(pXL, dAllowShearStress,	m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER, "Is2" , 3);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is1"), pOut->GetCellRef("Is2"));
			pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dBendStressFu,		m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER, "Is3" , 3);
			pOut->SetLineBoxText(pXL, dAllowBendStressFu,	m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER, "Is4" , 3);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is3"), pOut->GetCellRef("Is4"));
			pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, dBendStressFl,		m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "Is5" , 3);
			pOut->SetLineBoxText(pXL, dAllowBendStressFl,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "Is6" , 3);
			sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("Is5"), pOut->GetCellRef("Is6"));
			pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		}
		case 3://3) �������� ����
		{					
			CDesignCrossBeamJoint	Joint(pCalcStd, pBx);
			CDesignFlangeJoint		*pUpper		= Joint.m_pUpper;
			CDesignFlangeJoint		*pLower		= Joint.m_pLower;
			CDesignWebJoint			*pWebLeft	= Joint.m_pWebLeft;
			CDesignWebJoint			*pWebRight	= Joint.m_pWebRight;

			//������..���� ���� ��쿡 ����...
			BOOL bIsLeftCon			= TRUE;//����̸� 2 ������ �̸� 3
			BOOL bIsRightCon		= TRUE;

			if (pWebLeft->m_nType == 2)
				bIsLeftCon = TRUE;
			else
				bIsLeftCon = FALSE;

			if (pWebRight->m_nType == 2)
				bIsRightCon = TRUE;
			else
				bIsRightCon = FALSE;

			//�ż������� ����...
			long   nIsGuesset			= pUpper->m_nType;
			double dReqBoltEaFu			= 0;//�ʿ亼Ʈ�� ���
			double dUseBoltEaFu			= 0;//��뺼Ʈ�� 
			double dApplyLoadFu			= 0;//��������
			double dAllowLoadFu			= 0;//�������
			double dReqAreaFu			= 0;//�ʿ�ܸ���
			double dUseAreaFu			= 0;//���ܸ���
			double dJApplyStressFu		= 0;//�������ۿ�����
			double dJAllowStressFu		= 0;//�������������

			double dReqBoltEaFl			= 0;//�ʿ亼Ʈ�� �Ϻ�
			double dUseBoltEaFl			= 0;//��뺼Ʈ�� 
			double dApplyLoadFl			= 0;//��������
			double dAllowLoadFl			= 0;//�������
			double dReqAreaFl			= 0;//�ʿ�ܸ���
			double dUseAreaFl			= 0;//���ܸ���
			double dJApplyStressFl		= 0;//�������ۿ�����
			double dJAllowStressFl		= 0;//�������������

			double dIApplyStressFl		= 0;//����� �ۿ�����
			double dIAllowStressFl		= 0;//����� �������
			
			if(nIsGuesset == 0)//�ż¹���϶�
			{
				if(bIsLeftCon || bIsRightCon)//�������� �¿����� �ϳ��� ������...
				{
					dReqBoltEaFu		= pUpper->m_strtGusset.m_dBoltReq;									//�ʿ亼Ʈ�� ���
					dUseBoltEaFu		= pUpper->m_strtGusset.m_dBoltUse;									//��뺼Ʈ�� 
					dApplyLoadFu		= pUpper->m_strtGusset.m_dAxialLoad/dUseBoltEaFu;					//��������/EA
					dAllowLoadFu		= pUpper->m_strtGusset.m_dAllowBolt;								//�������/EA
					dReqAreaFu			= pUpper->m_strtGusset.m_dAreaFlange;								//�ʿ�ܸ���
					dUseAreaFu			= pUpper->m_strtGusset.m_dSumAs;									//���ܸ���				
					dJApplyStressFu		= pUpper->m_strtGusset.m_dConStress;								//�������ۿ�����
					dJAllowStressFu		= pUpper->m_strtGusset.m_dAllowStress;								//������������� 				

					dReqBoltEaFl		= pLower->m_strtGusset.m_dBoltReq;									//�ʿ亼Ʈ�� �Ϻ�
					dUseBoltEaFl		= pLower->m_strtGusset.m_dBoltUse;									//��뺼Ʈ�� 
					dApplyLoadFl		= pLower->m_strtGusset.m_dAxialLoad/dUseBoltEaFu;					//��������
					dAllowLoadFl		= pLower->m_strtGusset.m_dAllowBolt;								//�������
					dReqAreaFl			= pLower->m_strtGusset.m_dAreaFlange;								//�ʿ�ܸ���
					dUseAreaFl			= pLower->m_strtGusset.m_dSumAs;									//���ܸ���	
					dJApplyStressFl		= pLower->m_strtGusset.m_dConStress;								//�������ۿ�����
					dJAllowStressFl		= pLower->m_strtGusset.m_dAllowStress;								//�������������				

					dIApplyStressFl		= 0;	//����� �ۿ�����
					dIAllowStressFl		= 0;						//����� �������
				}				

				sText.Format("%s (���ؼ��÷���Ʈ���)", sGubun);
				pOut->SetXL(pXL, sText,						m_nSttRow, "B", "B",   1, BLACK, 9, FALSE, TA_LEFT);
			}
			else//��������϶�
			{	
				if(bIsLeftCon || bIsRightCon)//�������� �¿����� �ϳ��� ������...
				{
					dReqBoltEaFu		= pUpper->m_strtPlate.m_dQtyBoltReq;	//�ʿ亼Ʈ�� ���
					dUseBoltEaFu		= pUpper->m_strtPlate.m_dQtyBoltUse;	//��뺼Ʈ�� 
					dApplyLoadFu		= pUpper->m_strtPlate.m_dRho;			//��������
					dAllowLoadFu		= pUpper->m_strtPlate.m_dAllowBolt*2;	//�������
					dReqAreaFu			= pUpper->m_strtPlate.m_dAf;			//�ʿ�ܸ���
					dUseAreaFu			= pUpper->m_strtPlate.m_dAsn;			//���ܸ���				
					dJApplyStressFu		= pUpper->m_strtPlate.m_dDesignStress;	//�������ۿ�����
					dJAllowStressFu		= pUpper->m_strtPlate.m_dAllowStress;	//�������������				

					dReqBoltEaFl		= pLower->m_strtPlate.m_dQtyBoltReq;	//�ʿ亼Ʈ�� �Ϻ�
					dUseBoltEaFl		= pLower->m_strtPlate.m_dQtyBoltUse;	//��뺼Ʈ�� 
					dApplyLoadFl		= pLower->m_strtPlate.m_dRho;			//��������
					dAllowLoadFl		= pLower->m_strtPlate.m_dAllowBolt*2;	//�������
					dReqAreaFl			= pLower->m_strtPlate.m_dAf;			//�ʿ�ܸ���
					dUseAreaFl			= pLower->m_strtPlate.m_dAsn;			//���ܸ���	
					dJApplyStressFl		= pLower->m_strtPlate.m_dDesignStress;	//�������ۿ�����
					dJAllowStressFl		= pLower->m_strtPlate.m_dAllowStress;	//�������������				

					dIApplyStressFl		= pLower->m_strtPlate.m_dStressFlange;	//����� �ۿ�����
					dIAllowStressFl		= dJAllowStressFl;						//����� �������
				}

				pOut->SetXL(pXL, sGubun,					m_nSttRow, "B", "B",   1, BLACK, 9, FALSE, TA_LEFT);		
			}

			pOut->SetLineBoxText(pXL, "�� ��",						m_nSttRow, "B", "H",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "��� �÷���",				m_nSttRow, "I", "N",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�Ϻ� �÷���",				m_nSttRow, "O", "T",   2, BLACK, 9, FALSE, TA_CENTER);
			SetLines(2);
			pOut->SetLineBoxText(pXL, "�� Ʈ",						m_nSttRow, "B", "D",   5, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ʿ亼Ʈ��",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);

			pOut->SetLineBoxText(pXL, GetStrValue(dReqBoltEaFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu1", 1);
			pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dReqBoltEaFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl1", 1);
			pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "��뺼Ʈ��",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseBoltEaFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu2", 1);
			pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseBoltEaFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl2", 1);
			pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "�ۿ�����/EA",				m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dApplyLoadFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu3", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dApplyLoadFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl3", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "�������/EA",				m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dAllowLoadFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "Fu4", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dAllowLoadFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "Fl4", 0);
			pOut->SetLineBoxText(pXL, "N/EA",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "����",						m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);

			if(bIsLeftCon || bIsRightCon)			
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("Fu1"),
				pOut->GetCellRef("Fu2"), pOut->GetCellRef("Fu3"), pOut->GetCellRef("Fu4"));
			else
				sText = "-";			
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);

			if(bIsLeftCon || bIsRightCon)
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("Fl1"),
					pOut->GetCellRef("Fl2"), pOut->GetCellRef("Fl3"), pOut->GetCellRef("Fl4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "O", "T",   1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "������",						m_nSttRow, "B", "D",   5, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ʿ�ܸ���",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dReqAreaFu),		m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu1", 0);
			pOut->SetLineBoxText(pXL, "mm��",						 m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dReqAreaFl),		m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl1", 0);
			pOut->SetLineBoxText(pXL, "mm��",						 m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "���ܸ���",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseAreaFu),		m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu2", 0);
			pOut->SetLineBoxText(pXL, "mm��",						 m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dUseAreaFl),		m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl2", 0);
			pOut->SetLineBoxText(pXL, "mm��",						 m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "�ۿ�����",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJApplyStressFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu3", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJApplyStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl3", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "�������",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJAllowStressFu),	m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER, "JFu4", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "M", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, GetStrValue(dJAllowStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "JFl4", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "����",						m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			if(bIsLeftCon || bIsRightCon)
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("JFu1"),
					pOut->GetCellRef("JFu2"), pOut->GetCellRef("JFu3"), pOut->GetCellRef("JFu4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			if(bIsLeftCon || bIsRightCon)
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")", pOut->GetCellRef("JFl1"),
					pOut->GetCellRef("JFl2"), pOut->GetCellRef("JFl3"), pOut->GetCellRef("JFl4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "O", "T",   1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "",							m_nSttRow, "B", "D",   3, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "�����",								m_nSttRow, "B", "D",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ۿ�����",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "-",							m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetLineBoxText(pXL, GetStrValue(dIApplyStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "IFl3", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetXL(pXL, "��������",							m_nSttRow, "B", "D",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������",					m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "-",							m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetLineBoxText(pXL, GetStrValue(dIAllowStressFl),	m_nSttRow, "O", "R",   1, BLACK, 9, FALSE, TA_CENTER, "IFl4", 0);
			pOut->SetLineBoxText(pXL, "MPa",						m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetXL(pXL, "�� ��",								m_nSttRow, "B", "D",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",						m_nSttRow, "E", "H",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "-",							m_nSttRow, "I", "N",   1, BLACK, 9, FALSE, TA_CENTER);
			if(bIsLeftCon || bIsRightCon && (dIApplyStressFl != 0 && dIAllowStressFl != 0 ))
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")",
					pOut->GetCellRef("IFl3"), pOut->GetCellRef("IFl4"));
			else
				sText = "-";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "O", "T",   1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(-14);
			
			//������..���� ���� ��쿡 ����...

			double dMaxBendShearW	= 0;//�ִ������ܷ� ����������
			double dDirectShearW	= 0;//�������ܷ�
			double dMulShearW		= 0;//�ռ����ܷ�
			double dAllowShearW		= 0;//������ܷ�
			double dReqAreaW		= 0;//�ʿ�ܸ��� ����
			double dUseAreaW		= 0;//���ܸ���
			double dAppBendStressW	= 0;//�ۿ�����������
			double dAllBendStressW	= 0;//���������
			double dAppShearStressW	= 0;//�ۿ���������
			double dAllShearStressW	= 0;//�����������

			double dReqBoltEaW		= 0;//�ʿ亼Ʈ��
			double dUseBoleEaW		= 0;//��뺼Ʈ��
			
			pOut->SetLineBoxText(pXL, "������",			m_nSttRow, "U", "AP",   1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);

			if (bIsLeftCon)//������ �������̸�...
			{		
				dMaxBendShearW		= pWebLeft->m_strtPlate.m_dRhom;				//�ִ������ܷ� ������			
				dDirectShearW		= pWebLeft->m_strtPlate.m_dRhos;				//�������ܷ�
				dMulShearW			= pWebLeft->m_strtPlate.m_dRho;					//�ռ����ܷ�
				dAllowShearW		= pWebLeft->m_strtPlate.m_dAllowBolt*2;			//������ܷ�
				dReqAreaW			= pWebLeft->m_strtPlate.m_dReqArea;				//�ʿ�ܸ��� ����
				dUseAreaW			= pWebLeft->m_strtPlate.m_dSumAs;				//���ܸ���
				dAppBendStressW		= pWebLeft->m_strtPlate.m_dFMax;				//�ۿ�����������
				dAllBendStressW		= pWebLeft->m_strtPlate.m_dAllowTension;		//���������
				dAppShearStressW	= pWebLeft->m_strtPlate.m_dApplyLoad;			//�ۿ���������
				dAllShearStressW	= pWebLeft->m_strtPlate.m_dAllowShear;			//�����������

				pOut->SetLineBoxText(pXL, "���� ������",	m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "�ִ������ܷ�",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMaxBendShearW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "���� ���ܷ�",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dDirectShearW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "�ռ� ���ܷ�",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "��� ���ܷ�",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dReqAreaW,		m_nSttRow, "U", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL5", 1);
				pOut->SetLineBoxText(pXL, "mm��",			 m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dUseAreaW,		m_nSttRow, "U", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL6", 1);
				pOut->SetLineBoxText(pXL, "mm��",			 m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAppBendStressW,	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER, "WL7", 1);
				pOut->SetLineBoxText(pXL, dAppShearStressW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL8", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAllBendStressW,	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER, "WL9", 1);
				pOut->SetLineBoxText(pXL, dAllShearStressW,	m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL10", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL7"), pOut->GetCellRef("WL9"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL8"), pOut->GetCellRef("WL10"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "Z", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "������",			m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, "��������",		m_nSttRow, "Z", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				SetLines(-13);
			}
			else//������ ���� ���
			{
				dReqBoltEaW		= pWebLeft->m_strtGusset.m_dBoltReq;					//�ʿ亼Ʈ��
				dUseBoleEaW		= pWebLeft->m_strtGusset.m_dBoltUse;					//��뺼Ʈ��
				dMulShearW		= pWebLeft->m_strtGusset.m_dApplyLoad / dUseBoleEaW;	//�ռ����ܷ�
				dAllowShearW	= pWebLeft->m_strtGusset.m_dAllowBolt;					//������ܷ�

				pOut->SetLineBoxText(pXL, "���� ������",	m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "�ʿ� ��Ʈ��",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqBoltEaW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "��",				m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "��� ��Ʈ��",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dUseBoleEaW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "��",				m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "�ռ� ���ܷ�",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "��� ���ܷ�",	m_nSttRow, "U", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "Z", "AC",  1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AD", "AE", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "U", "AE",  2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				SetLines(-13);
			}

			if (bIsRightCon)//������ �������̸�...
			{
				dMaxBendShearW		= pWebRight->m_strtPlate.m_dRhom;			//�ִ������ܷ� ������			
				dDirectShearW		= pWebRight->m_strtPlate.m_dRhos;			//�������ܷ�
				dMulShearW			= pWebRight->m_strtPlate.m_dRho;			//�ռ����ܷ�
				dAllowShearW		= pWebRight->m_strtPlate.m_dAllowBolt*2;	//������ܷ�
				dReqAreaW			= pWebRight->m_strtPlate.m_dReqArea;		//�ʿ�ܸ��� ����
				dUseAreaW			= pWebRight->m_strtPlate.m_dSumAs;			//���ܸ���
				dAppBendStressW		= pWebRight->m_strtPlate.m_dFMax;			//�ۿ�����������
				dAllBendStressW		= pWebRight->m_strtPlate.m_dAllowTension;	//���������
				dAppShearStressW	= pWebRight->m_strtPlate.m_dApplyLoad;		//�ۿ���������
				dAllShearStressW	= pWebRight->m_strtPlate.m_dAllowShear;		//�����������

				pOut->SetLineBoxText(pXL, "���� ������",	m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "�ִ������ܷ�",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMaxBendShearW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "���� ���ܷ�",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dDirectShearW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "�ռ� ���ܷ�",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "��� ���ܷ�",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dReqAreaW,		m_nSttRow, "AF", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL5", 1);
				pOut->SetLineBoxText(pXL, "mm��",			 m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dUseAreaW,		m_nSttRow, "AF", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL6", 1);
				pOut->SetLineBoxText(pXL, "mm��",			 m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAppBendStressW,	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "WL7", 1);
				pOut->SetLineBoxText(pXL, dAppShearStressW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL8", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, dAllBendStressW,	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "WL9", 1);
				pOut->SetLineBoxText(pXL, dAllShearStressW,	m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL10", 1);
				pOut->SetLineBoxText(pXL, "MPa",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL7"), pOut->GetCellRef("WL9"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("=IF(AND(%s<=%s,%s<=%s),\"O.K\",\"N.G\")",	pOut->GetCellRef("WL5"), 
					pOut->GetCellRef("WL6"), pOut->GetCellRef("WL8"), pOut->GetCellRef("WL10"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AK", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "������",			m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, "��������",		m_nSttRow, "AK", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(3);						
			}
			else//������ ���� ���
			{
				dReqBoltEaW		= pWebRight->m_strtGusset.m_dBoltReq;					//�ʿ亼Ʈ��
				dUseBoleEaW		= pWebRight->m_strtGusset.m_dBoltUse;					//��뺼Ʈ��
				dMulShearW		= pWebRight->m_strtGusset.m_dApplyLoad / dUseBoleEaW;	//�ռ����ܷ�
				dAllowShearW	= pWebRight->m_strtGusset.m_dAllowBolt;					//������ܷ�

				pOut->SetLineBoxText(pXL, "���� ������",	m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);			
				pOut->SetLineBoxText(pXL, "�ʿ� ��Ʈ��",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqBoltEaW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL1", 1);
				pOut->SetLineBoxText(pXL, "��",				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "��� ��Ʈ��",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dUseBoleEaW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL2", 1);
				pOut->SetLineBoxText(pXL, "��",				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);	
				pOut->SetLineBoxText(pXL, "�ռ� ���ܷ�",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMulShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL3", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "��� ���ܷ�",	m_nSttRow, "AF", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dAllowShearW,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "WL4", 1);
				pOut->SetLineBoxText(pXL, "N/EA",			m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				sText.Format("=IF(AND(%s<=%s,%s<=%s,%s<=%s),\"O.K\",\"N.G\")",
					pOut->GetCellRef("WL1"), pOut->GetCellRef("WL4"), pOut->GetCellRef("WL2"),
					pOut->GetCellRef("WL4"), pOut->GetCellRef("WL3"), pOut->GetCellRef("WL4"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);			
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);						
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);	
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
				SetLines(3);			
			}
			break;
		}
		case 4://����������
		{		
			BOOL bIsVStiff = FALSE;//���������簡 �ʿ����� ����...
			double dMaxGapRateV		= 1.5;//���ݺ� �ִ�
			double dUseGapRateV		= 0;
			double dReqWidthMinV	= 0;
			double dUseWidthMinV	= 0;
			double dReqThickMinV	= 0;
			double dUseThickMinV	= 0;
			double dReqStrengthV	= 0;
			double dUseStrengthV	= 0;

			if(pEndCr)
			{
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(nQtyVerStiff !=0 && pEndCr->m_pVerStiff->m_bIsNeedVerStiff)
				{
					bIsVStiff = TRUE;				
					dUseGapRateV	= pEndCr->m_pVerStiff->m_dRateSpaceHeight;
					dReqWidthMinV	= pEndCr->m_pVerStiff->m_dBvsMin;
					dUseWidthMinV	= pEndCr->m_pVerStiff->m_dBvs;
					dReqThickMinV	= pEndCr->m_pVerStiff->m_dTvsMin;
					dUseThickMinV	= pEndCr->m_pVerStiff->m_dTvs;
					dReqStrengthV	= pEndCr->m_pVerStiff->m_dIReq;
					dUseStrengthV	= pEndCr->m_pVerStiff->m_dIUse;
				}
			}
			if(pCenCr)
			{
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(nQtyVerStiff !=0 && pCenCr->m_pVerStiff->m_bIsNeedVerStiff)
				{
					bIsVStiff = TRUE;				
					dUseGapRateV	= pCenCr->m_pVerStiff->m_dRateSpaceHeight;
					dReqWidthMinV	= pCenCr->m_pVerStiff->m_dBvsMin;
					dUseWidthMinV	= pCenCr->m_pVerStiff->m_dBvs;
					dReqThickMinV	= pCenCr->m_pVerStiff->m_dTvsMin;
					dUseThickMinV	= pCenCr->m_pVerStiff->m_dTvs;
					dReqStrengthV	= pCenCr->m_pVerStiff->m_dIReq;
					dUseStrengthV	= pCenCr->m_pVerStiff->m_dIUse;
				}

			}
			
			if (bIsVStiff)
			{						
				pOut->SetLineBoxText(pXL, sGubun,					m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dMaxGapRateV,				m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER, "V1", 1);
				pOut->SetLineBoxText(pXL, dUseGapRateV,				m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER, "V2", 3);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V1"), pOut->GetCellRef("V2"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqWidthMinV,			m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER, "V3", 1);
				pOut->SetLineBoxText(pXL, dUseWidthMinV,			m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER, "V4", 1);
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V3"), pOut->GetCellRef("V4"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqThickMinV,			m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER, "V5", 1);
				pOut->SetLineBoxText(pXL, dUseThickMinV,			m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER, "V6", 1);
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V5"), pOut->GetCellRef("V6"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqStrengthV,			m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "V7", 0);
				pOut->SetLineBoxText(pXL, dUseStrengthV,			m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "V8", 0);
				sText.Format("=IF(%s<=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("V7"), pOut->GetCellRef("V8"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
				
			}
			else
			{
				
			}
			break;
		}
		case 5://���򺸰���
		{
			BOOL   bIsHStriff		= FALSE;//���򺸰��簡 �ʿ����� ����
			double dReqThickMinH	= 0;
			double dUseThickMinH	= 0;
			double dReqStrengthH	= 0;
			double dUseStrnghthH	= 0;
			double dFitPositionH	= 0;
			double dSetPoistionH	= 0;

			if(pEndCr)
			{			
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(pEndCr->m_nQtyHorStiff > 0 && (nQtyVerStiff !=0 && pEndCr->m_pVerStiff->m_bIsNeedVerStiff))
				{
					bIsHStriff = TRUE;				
					dReqThickMinH	= pEndCr->m_pHorStiff->m_dThsMin;
					dUseThickMinH	= pEndCr->m_pHorStiff->m_dThs;
					dReqStrengthH	= pEndCr->m_pHorStiff->m_dIReq;
					dUseStrnghthH	= pEndCr->m_pHorStiff->m_dIUse;
					dFitPositionH	= 0;
					dSetPoistionH	= 0;				
				}
			}
			if(pCenCr)
			{
				long nQtyVerStiff	= pCrossBeam->m_nVStiffsu_CR;
				if(pCenCr->m_nQtyHorStiff > 0 && (nQtyVerStiff !=0 && pCenCr->m_pVerStiff->m_bIsNeedVerStiff))
				{
					bIsHStriff = TRUE;				
					dReqThickMinH	= pCenCr->m_pHorStiff->m_dThsMin;
					dUseThickMinH	= pCenCr->m_pHorStiff->m_dThs;
					dReqStrengthH	= pCenCr->m_pHorStiff->m_dIReq;
					dUseStrnghthH	= pCenCr->m_pHorStiff->m_dIUse;
					dFitPositionH	= 0;
					dSetPoistionH	= 0;				
				}
			}

			if(bIsHStriff)
			{
				pOut->SetLineBoxText(pXL, sGubun,					m_nSttRow, "B", "D",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqThickMinH,			m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER, "H1", 1);
				pOut->SetLineBoxText(pXL, dUseThickMinH,			m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER, "H2", 1);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("H1"), pOut->GetCellRef("H2"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dReqStrengthH,			m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER, "H3", 0);
				pOut->SetLineBoxText(pXL, dUseStrnghthH,			m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER, "H4", 0);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("H3"), pOut->GetCellRef("H4"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dFitPositionH,			m_nSttRow, "W", "AA",  1, BLACK, 9, FALSE, TA_CENTER, "H5", 0);
				pOut->SetLineBoxText(pXL, dSetPoistionH,			m_nSttRow, "AB", "AF", 1, BLACK, 9, FALSE, TA_CENTER, "H6", 0);
				sText.Format("=IF(%s>=%s,\"O.K\",\"N.G\")", pOut->GetCellRef("H5"), pOut->GetCellRef("H6"));
				pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AG", "AH", 1, BLACK, 9, FALSE, TA_CENTER);	
				pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AI", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}
			break;
		}
	}//end switch
}

void CXLCheckListCrossBeam::CheckListTitle(int nSeq)
{
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	switch(nSeq)
	{
		case 1://��밭�� �� �ּҵβ� ����
			pOut->SetXL(pXL, "��) ���κ� ", m_nSttRow, "A", "A");
			pOut->SetXL(pXL, "1) ��밭�� �� �ּҵβ� ����", m_nSttRow, "B", "B");
				
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "E", "H",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "L",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "������ �β�(mm)",		m_nSttRow, "M", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����÷��� �β�(mm)",	m_nSttRow, "U", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�Ϻ��÷��� �β�(mm)",	m_nSttRow, "AC", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "AK", "AP", 2, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetXL(pXL,"���򺸰���",						m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL,"���ܼ�",							m_nSttRow, "I", "L",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ּ�",					m_nSttRow, "M", "O",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "P", "R",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����", 					m_nSttRow, "S", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ּ�", 					m_nSttRow, "U", "W",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���", 					m_nSttRow, "X", "Z",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����", 					m_nSttRow, "AA", "AB", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ּ�", 					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���", 					m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����", 					m_nSttRow, "AI", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			break;
		case 2://���°���
			pOut->SetXL(pXL, "2) ���� ����", m_nSttRow, "B", "B");

			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "E", "H",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "L",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������� (MPa)", 		m_nSttRow, "M", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� �� �� (MPa)", 		m_nSttRow, "W", "AP",  1, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetXL(pXL, "�ۿ���Ʈ",						m_nSttRow, "E", "H",   0, BLACK, 9, FALSE, TA_CENTER);		
			pOut->SetXL(pXL, "�ۿ����ܷ�",						m_nSttRow, "I", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL,"(kN��m)",					 		 m_nSttRow, "E", "H",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL,"(kN)",								m_nSttRow, "I", "L",   0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ۿ�����",				m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������",				m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "������",				m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������",				m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�Ͽ�����",				m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������",				m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		case 3:
			pOut->SetXL(pXL, "3) �������� ����", m_nSttRow, "B", "B");
			break;
		case 4://����������
			pOut->SetXL(pXL, "4) ����������", m_nSttRow, "B", "B");
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���ݺ�(a/b)",			m_nSttRow, "E", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������� �ּ���(mm)",	m_nSttRow, "M", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������� �ּҵβ�(mm)",	m_nSttRow, "W", "AF",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��(mm��)", 			m_nSttRow, "AG", "AP", 1, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "�ִ�",					m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "W", "Z",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		case 5://���򺸰���
			pOut->SetXL(pXL, "5) ���򺸰���",					m_nSttRow, "B", "B", 0);
			pOut->SetXL(pXL, "���򺸰����� ��ġ�� ���������κ����� �Ÿ� (mm)", 
																m_nSttRow, "W", "W", 1, BLACK, 9, FALSE, TA_LEFT);		
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "D",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������� �ּҵβ�(mm)",	m_nSttRow, "E", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��(mm��)",			m_nSttRow, "M", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "��ġ��ġ�� ������(mm)",	m_nSttRow, "W", "AH",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���", 					m_nSttRow, "AI", "AP", 2, BLACK, 9, FALSE, TA_CENTER);	
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "E", "G",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "H", "J",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "K", "L",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "M", "P",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "������ġ",				m_nSttRow, "W", "AA",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "��ġ��ġ",				m_nSttRow, "AB", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AG", "AH", 1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			break;
		case 6://�����극�̽�...
			pOut->SetXL(pXL, "��) �����극�̽� ", m_nSttRow, "A", "A");
			pOut->SetXL(pXL, "1) ��밭�� �� ���� ����", m_nSttRow, "B", "B");
				
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "F",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "G", "I",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� �� �� ��",			m_nSttRow, "J", "P",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�����(��150)",			 m_nSttRow, "Q", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���� ���� (MPa)",		m_nSttRow, "W", "AH",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ż�Ʈ����(MPa)",		m_nSttRow, "AI", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AO", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "��/��min",				  m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "W", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "Z", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ۿ�",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
		case 7://�ż�Ʈ �÷���Ʈ�� ����
			pOut->SetXL(pXL, "2) �ż�Ʈ �÷���Ʈ�� ����", m_nSttRow, "B", "B");
			
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "I",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ż�Ʈ�� ����(MPa)",		m_nSttRow, "J", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "��Ʈ�� (��)",			m_nSttRow, "U", "AE",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "������ �������� (MPa)",  m_nSttRow, "AF", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "�ۿ�����",				m_nSttRow, "J", "M",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������",				m_nSttRow, "N", "Q",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "R", "T",   1, BLACK, 9, FALSE, TA_CENTER);				
			pOut->SetLineBoxText(pXL, "�ʿ��",					m_nSttRow, "U", "X",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "Y", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ۿ�����",				m_nSttRow, "AF", "AI", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�������",				m_nSttRow, "AJ", "AM", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AN", "AP", 1, BLACK, 9, FALSE, TA_CENTER);		
			SetLines(NEXT);
			break;
		case 8://����극�̽�..
			pOut->SetXL(pXL, "��) ����극�̽� ", m_nSttRow, "A", "A");
			pOut->SetXL(pXL, "1) ��밭�� �� ���� ����", m_nSttRow, "B", "B");
				
			pOut->SetLineBoxText(pXL, "�� ġ",					m_nSttRow, "B", "F",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "G", "I",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� �� �� ��",			m_nSttRow, "J", "P",   2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�����(��150)",			 m_nSttRow, "Q", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���� ���� (MPa)",		m_nSttRow, "W", "AH",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����κ�Ʈ(��)",			m_nSttRow, "AI", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AO", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			pOut->SetLineBoxText(pXL, "��/��min",				  m_nSttRow, "Q", "T",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "U", "V",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "W", "Y",   1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "Z", "AB",  1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�ۿ�",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			break;
	}//end switch	
}

void CXLCheckListCrossBeam::CorssBeamBracing(int nSeq)
{
	CPlateBridgeApp		*pDB = m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	BOOL bOnlyFirst = TRUE;	
		
	for(long nG = 0 ; nG < pDB->GetGirdersu()-1; nG++)//1;nG++)//
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		CString sGubun;

		int nCrossCount = 1;
		
		while(pBx)
		{
			CSection *pSec = pBx->GetSection();			
			if(pSec)
			{	
				if(m_pAPlateOutCalcStd->GetAPlateOutExcel()->m_bAbort)	return;
				
				// 0=V�������극�̽�, 1=��V�������극�̽�, 2= ���κ�
				if(pSec->GetenDetType()==SECDET_VBRACING) // �극�̽� Ÿ��..
				{		
					if (bOnlyFirst)
					{
						CheckListTitle(nSeq);
						bOnlyFirst = FALSE;
					}

					if(nCrossCount < 10)
						sGubun.Format("%dBR0%d", nG+1,nCrossCount);
					else
						sGubun.Format("%dBR%d", nG+1,nCrossCount);
					
					CrossBeamBracingDetail(nSeq, pBx, sGubun);					
				}
			}	
			nCrossCount++;
			pBx = Finder.GetBxNext();
		}	
	}

	if(bOnlyFirst && nSeq == 6)
	{
		pOut->SetXL(pXL, "���� �극�̽� Ÿ���� �����ϴ�. ", m_nSttRow, "A", "A");		
	}

}

void CXLCheckListCrossBeam::CrossBeamBracingDetail(int nSeq, CPlateBasicIndex *pBx, CString sGubun)
{
	CAPlateCalcStd			*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl				*pXL			= m_pXL;
	CAPlateOutCalcXL		*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	
	CDesignCrossBeam		CrossBeam(pCalcStd, pBx);
	CDesignVBracing			*pVBracing		= NULL;
	CDesignInverseVBracing	*pInVBr			= NULL;
	
	CString sText = _T("");

	if(pBx->GetSection()->IsenType(SECTION_A))
		pInVBr		= (CDesignInverseVBracing*)CrossBeam.GetDesignCrossBeam();
	else
		pVBracing	= (CDesignVBracing*)CrossBeam.GetDesignCrossBeam();

	CString sType[3];
	CString sMaterial[3];
	CString sJeWon[3];				//��������
	double  dSlenderness[3];		//�����
	double  dCompressStress[3];		//��������
	double  dComAllowStress[3];		//�������
	double  dTensionStress[3];		//��������
	double  dTenAllowStress[3];		//�������
	double  dGussetStress[3];		//�ż��ۿ�
	double  dGuAllowStress[3];		//�ż����

	double  dReqBoltEA[3];			//�ʿ亼Ʈ��
	double  dUseBoltEa[3];			//��뺼Ʈ��
	double  dWeldStress[3];			//�������ۿ�����
	double  dWeldAllowStress[3];	//�������������
	
	for (int n=0; n<3; n++)			//�ʱ�ȭ
	{
		sMaterial[n]		= "";
		sJeWon[n]			= "";
		dSlenderness[n]		= 0;
		dCompressStress[n]	= 0;	
		dComAllowStress[n]	= 0;	
		dTensionStress[n]	= 0;	
		dTenAllowStress[n]	= 0;	
		dGussetStress[n]	= 0;	
		dGuAllowStress[n]	= 0;
		
		dReqBoltEA[n]		= 0;		
		dUseBoltEa[n]		= 0;		
		dWeldStress[n]		= 0;		
		dWeldAllowStress[n]	= 0;
	}
	
	switch(nSeq)
	{
		case 6://�����극�̽�..��� ������ ���°���.
		{			
			sType[0]="����";
			sType[1]="����";
			sType[2]="����";
			
			if(pVBracing)
			{
				//������
				int n = 0;
				sMaterial[n]		= pVBracing->m_strtChordUp.m_strMaterial;
				sJeWon[n]			= pVBracing->m_strtChordUp.m_sMaterialType;
				dSlenderness[n]		= pVBracing->m_strtChordUp.m_dSlendernessRatio;
				dCompressStress[n]	= pVBracing->m_strtChordUp.m_dFc;
				dComAllowStress[n]	= pVBracing->m_strtChordUp.m_dFcaApo;
				dTensionStress[n]	= 0;
				dTenAllowStress[n]	= 0;	
				dGussetStress[n]	= pVBracing->m_strtChordUp.m_dShear/2;
				dGuAllowStress[n]	= pVBracing->m_strtChordUp.m_dAllowShear;

				//����
				n++;
				sMaterial[n]		= pVBracing->m_strtDiagonal.m_strMaterial;
				sJeWon[n]			= pVBracing->m_strtDiagonal.m_sMaterialType;
				dSlenderness[n]		= pVBracing->m_strtDiagonal.m_dSlendernessRatio;
				dCompressStress[n]	= pVBracing->m_strtDiagonal.m_dFc;
				dComAllowStress[n]	= pVBracing->m_strtDiagonal.m_dFcaApo;
				dTensionStress[n]	= pVBracing->m_strtDiagonal.m_dFt;
				dTenAllowStress[n]	= pVBracing->m_strtDiagonal.m_dFta;
				dGussetStress[n]	= pVBracing->m_strtDiagonal.m_dShear/2;
				dGuAllowStress[n]	= pVBracing->m_strtDiagonal.m_dAllowShear;

				//������
				n++;
				sMaterial[n]		= pVBracing->m_strtChordLo.m_strMaterial;
				sJeWon[n]			= pVBracing->m_strtChordLo.m_sMaterialType;
				dSlenderness[n]		= pVBracing->m_strtChordLo.m_dSlendernessRatio;
				dCompressStress[n]	= pVBracing->m_strtChordLo.m_dFc;
				dComAllowStress[n]	= pVBracing->m_strtChordLo.m_dFcaApo;
				dTensionStress[n]	= 0;
				dTenAllowStress[n]	= 0;	
				dGussetStress[n]	= pVBracing->m_strtChordLo.m_dShear/2;
				dGuAllowStress[n]	= pVBracing->m_strtChordLo.m_dAllowShear;
			}
			if(pInVBr)
			{
				//������
				int n=0;
				sMaterial[n]		= pInVBr->m_strtChordUp.m_strMaterial;
				sJeWon[n]			= pInVBr->m_strtChordUp.m_sMaterialType;
				dSlenderness[n]		= pInVBr->m_strtChordUp.m_dSlendernessRatio;
				dCompressStress[n]	= pInVBr->m_strtChordUp.m_dFc;
				dComAllowStress[n]	= pInVBr->m_strtChordUp.m_dFcaApo;
				dTensionStress[n]	= pInVBr->m_strtChordUp.m_dF;
				dTenAllowStress[n]	= pInVBr->m_strtChordUp.m_dFa;
				dGussetStress[n]	= pInVBr->m_strtChordUp.m_dShear;
				dGuAllowStress[n]	= pInVBr->m_strtChordUp.m_dAllowShear;

				//����
				n++;
				sMaterial[n]		= pInVBr->m_strtDiagonal.m_strMaterial;
				sJeWon[n]			= pInVBr->m_strtDiagonal.m_sMaterialType;
				dSlenderness[n]		= pInVBr->m_strtDiagonal.m_dSlendernessRatio;
				dCompressStress[n]	= pInVBr->m_strtDiagonal.m_dFc;
				dComAllowStress[n]	= pInVBr->m_strtDiagonal.m_dFcaApo;
				dTensionStress[n]	= pInVBr->m_strtDiagonal.m_dFt;
				dTenAllowStress[n]	= pInVBr->m_strtDiagonal.m_dFta;
				dGussetStress[n]	= pInVBr->m_strtDiagonal.m_dShear;
				dGuAllowStress[n]	= pInVBr->m_strtDiagonal.m_dAllowShear;

				//������
				n++;
				sMaterial[n]		= pInVBr->m_strtChordLo.m_strMaterial;
				sJeWon[n]			= pInVBr->m_strtChordLo.m_sMaterialType;
				dSlenderness[n]		= pInVBr->m_strtChordLo.m_dSlendernessRatio;
				dCompressStress[n]	= pInVBr->m_strtChordLo.m_dFc;
				dComAllowStress[n]	= pInVBr->m_strtChordLo.m_dFcaApo;
				dTensionStress[n]	= pInVBr->m_strtChordLo.m_dFt;
				dTenAllowStress[n]	= pInVBr->m_strtChordLo.m_dFta;
				dGussetStress[n]	= pInVBr->m_strtChordLo.m_dShear;
				dGuAllowStress[n]	= pInVBr->m_strtChordLo.m_dAllowShear;
				
			}
			//���� ������ ����...
			pOut->SetLineBoxText(pXL, sGubun,					m_nSttRow, "B", "D",   3, BLACK, 9, FALSE, TA_CENTER);
			for(long n=0; n<3; n++)
			{
				pOut->SetLineBoxText(pXL, sType[n],				m_nSttRow, "E",  "F",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, sMaterial[n],			m_nSttRow, "G",  "I",  1, BLACK, 9, FALSE, TA_CENTER);					
				pOut->SetLineBoxText(pXL, sJeWon[n],			m_nSttRow, "J",  "P",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dSlenderness[n],		m_nSttRow, "Q",  "T",  1, BLACK, 9, FALSE, TA_CENTER, "F1", 1);
				sText.Format("=IF((%s<=150),\"O.K\",\"N.G\")",pOut->GetCellRef("F1"));
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "U",  "V",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dCompressStress[n],	m_nSttRow, "W",  "Y",  1, BLACK, 9, FALSE, TA_CENTER, "F2", 1);
				pOut->SetLineBoxText(pXL, dComAllowStress[n],	m_nSttRow, "Z", "AB", 1, BLACK, 9, FALSE, TA_CENTER, "F3", 1);			
				pOut->SetLineBoxText(pXL, dTensionStress[n],	m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER, "F4", 1);
				pOut->SetLineBoxText(pXL, dTenAllowStress[n],	m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "F5", 1);
				pOut->SetLineBoxText(pXL, dGussetStress[n],		m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER, "F6", 1);
				pOut->SetLineBoxText(pXL, dGuAllowStress[n],	m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "F7", 1);
				sText.Format("=IF(AND((%s<=%s),(%s<=%s),(%s<=%s)),\"O.K\",\"N.G\")", 
					pOut->GetCellRef("F2"),pOut->GetCellRef("F3"),pOut->GetCellRef("F4"),
					pOut->GetCellRef("F5"),pOut->GetCellRef("F6"),pOut->GetCellRef("F7"));
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}

			break;
		}
		case 7://2) �ż�Ʈ �÷���Ʈ�� ����		
		{
			if(pVBracing)
			{
				sType[0]="������/����";
				sType[1]="������";
				sType[2]="����/������";

				//������/����
				int n=0;										
				dGussetStress[n]	= pVBracing->m_strtGussetUp.m_dStress;
				dGuAllowStress[n]	= pVBracing->m_strtGussetUp.m_dAllowStress;
				dReqBoltEA[n]		= pVBracing->m_strtGussetUp.m_dQtyBoltReq;
				dUseBoltEa[n]		= pVBracing->m_strtGussetUp.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;

				//������
				n++;
				dGussetStress[n]	= pVBracing->m_strtGussetLo.m_dStress;
				dGuAllowStress[n]	= pVBracing->m_strtGussetLo.m_dAllowStress;
				dReqBoltEA[n]		= pVBracing->m_strtGussetLo.m_dQtyBoltReq;
				dUseBoltEa[n]		= pVBracing->m_strtGussetLo.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;

				//����/������
				n++;
				dGussetStress[n]	= pVBracing->m_strtGussetDiagonal.m_dShear;
				dGuAllowStress[n]	= pVBracing->m_strtGussetDiagonal.m_dAllowShear;
				dReqBoltEA[n]		= 0;
				dUseBoltEa[n]		= 0;
				dWeldStress[n]		= pVBracing->m_strtGussetDiagonal.m_dShearWeld;
				dWeldAllowStress[n]	= pVBracing->m_strtGussetDiagonal.m_dAllowShearWeld;			
			}
			if(pInVBr)
			{
				sType[0]="������";
				sType[1]="����/������";
				sType[2]="������/����";

				//������
				int n=0;										
				dGussetStress[n]	= pInVBr->m_strtGussetUp.m_dStress;
				dGuAllowStress[n]	= pInVBr->m_strtGussetUp.m_dAllowStress;
				dReqBoltEA[n]		= pInVBr->m_strtGussetUp.m_dQtyBoltReq;
				dUseBoltEa[n]		= pInVBr->m_strtGussetUp.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;

				//����/������
				n++;

				//Guesset Plate�� ���°���� �ۿ������� [����/�������] [��������/�����������] �� 
				//ū���� ǥ���Ѵ�. (060913����...���������� ����)
				double dF_Rate = pInVBr->m_strtGussetDiagonal.m_dAllowStress/pInVBr->m_strtGussetDiagonal.m_dStress;
				double dV_Rate = pInVBr->m_strtGussetDiagonal.m_dAllowShear/pInVBr->m_strtGussetDiagonal.m_dShearGusset; 

				if (dF_Rate > dV_Rate)
				{
					dGussetStress[n]	= pInVBr->m_strtGussetDiagonal.m_dStress;
					dGuAllowStress[n]	= pInVBr->m_strtGussetDiagonal.m_dAllowStress;
				}
				else
				{
					dGussetStress[n]	= pInVBr->m_strtGussetDiagonal.m_dShearGusset;
					dGuAllowStress[n]	= pInVBr->m_strtGussetDiagonal.m_dAllowShear;
				}						
				
				dReqBoltEA[n]		= 0;
				dUseBoltEa[n]		= 0;
				dWeldStress[n]		= pInVBr->m_strtGussetDiagonal.m_dShearWeld;
				dWeldAllowStress[n]	= pInVBr->m_strtGussetDiagonal.m_dAllowShearWeld;	

				//������/����
				n++;
				dGussetStress[n]	= pInVBr->m_strtGussetLo.m_dStress;
				dGuAllowStress[n]	= pInVBr->m_strtGussetLo.m_dAllowStress;
				dReqBoltEA[n]		= pInVBr->m_strtGussetLo.m_dQtyBoltReq;
				dUseBoltEa[n]		= pInVBr->m_strtGussetLo.m_dQtyBoltUse;
				dWeldStress[n]		= 0;
				dWeldAllowStress[n]	= 0;
				
			}
			//���� ������ ����...
			pOut->SetLineBoxText(pXL, sGubun,								m_nSttRow, "B", "D",   3, BLACK, 9, FALSE, TA_CENTER);
			for(long n=0; n<3; n++)
			{
				pOut->SetLineBoxText(pXL, sType[n],							m_nSttRow, "E",  "I",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, dGussetStress[n],					m_nSttRow, "J",  "M",  1, BLACK, 9, FALSE, TA_CENTER, "G1", 3);
				pOut->SetLineBoxText(pXL, dGuAllowStress[n],				m_nSttRow, "N",  "Q",  1, BLACK, 9, FALSE, TA_CENTER, "G2", 3);
				sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")",pOut->GetCellRef("G1"), pOut->GetCellRef("G2"));
				pOut->SetLineBoxText(pXL, sText,							m_nSttRow, "R",  "T",  1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, GetStrValue(dReqBoltEA[n]),		m_nSttRow, "U",  "X",  1, BLACK, 9, FALSE, TA_CENTER, "G3", 1);
				pOut->SetLineBoxText(pXL, GetStrValue(dUseBoltEa[n]),		m_nSttRow, "Y",  "AB", 1, BLACK, 9, FALSE, TA_CENTER, "G4", 1);
				if (dReqBoltEA[n] ==0)
					sText = "-";
				else
					sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")",
						pOut->GetCellRef("G3"), pOut->GetCellRef("G4"));
				pOut->SetLineBoxText(pXL, sText,							m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, GetStrValue(dWeldStress[n]),		m_nSttRow, "AF", "AI", 1, BLACK, 9, FALSE, TA_CENTER, "G5", 3);
				pOut->SetLineBoxText(pXL, GetStrValue(dWeldAllowStress[n]),	m_nSttRow, "AJ", "AM", 1, BLACK, 9, FALSE, TA_CENTER, "G6", 3);
				if (dWeldStress[n] ==0)
					sText = "-";
				else
					sText.Format("=IF(%s<%s,\"O.K\",\"N.G\")",
						pOut->GetCellRef("G5"), pOut->GetCellRef("G6"));
				pOut->SetLineBoxText(pXL, sText,							m_nSttRow, "AN", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}

			break;
		}
	}
}

void CXLCheckListCrossBeam::HorBracing(int nSeq)
{
	CPlateBridgeApp		*pDB = m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	
	BOOL bOnlyFirst = TRUE;
	BOOL bOutValid = FALSE;
	for(long nG = 0 ; nG < pDB->GetGirdersu() - 1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);	
		
		for(long nBr=0; nBr<pGir->m_BracingArr.GetSize() ; nBr++)
		{
			if (bOnlyFirst == TRUE)
			{
				CheckListTitle(nSeq);
				bOnlyFirst = FALSE;
			} 
			
			if(HorBracingDetail(nSeq, nG, nBr) == TRUE)
			{
				bOutValid = TRUE;
			}
		}		
	}

	if(bOutValid == FALSE)
	{
		pXL->DeleteRowLine(m_nSttRow - 6, m_nSttRow - 1);	//���� �극�̽� ���̺� Ÿ��Ʋ�� ������.
		pOut->SetXL(pXL, "���� �극�̽� Ÿ���� �����ϴ�. ", m_nSttRow, "A", "A");		
		pXL->DeleteRowLineEnd();
	}

}

BOOL CXLCheckListCrossBeam::HorBracingDetail(int nSeq, long nG, long nBr)
{
	CPlateBridgeApp			*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd			*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl				*pXL			= m_pXL;
	CAPlateOutCalcXL		*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CBracing				*pHBracing		= pDB->GetGirder(nG)->GetBracingInTotal(nBr);
	long nCur = nBr;
	BOOL bOutValid = FALSE;
	BOOL bContinue = TRUE;

	if(pHBracing == NULL) 
		return bOutValid;

	CPlateBasicIndex *pBx = pHBracing ? pHBracing->GetBx() : NULL;
	
	if(pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2) == NULL) 
		return bOutValid;

	CSteelMaterialData	*pMatData	= pHBracing->GetSteelMaterialData();

	if(pMatData->m_dH == 0 || pMatData->m_dB == 0 || pMatData->m_dT1 == 0)
	{
		while (pBx)
		{
			pHBracing = pDB->GetGirder(nG)->GetBracingInTotal(++nCur);
			if (pHBracing == NULL)
			{
				bContinue=FALSE;
				break;
			}

			pBx = pHBracing->GetBx();
			pMatData	= pHBracing->GetSteelMaterialData();
			if (pMatData->m_dH && pMatData->m_dB && pMatData->m_dT1) 
			{
				bOutValid = TRUE;
				break;
			}
		}

		if (bContinue == FALSE)  
		{
			bOutValid = FALSE;
			return bOutValid;
		}
	}

	CDesignHorBracing	HorBracing(pCalcStd, nG, nCur);
	CString sText = _T("");
	CString sMaterial		= HorBracing.m_strMaterial;
	CString sJeWon			= HorBracing.m_strMaterialType;		//��������
	double  dSlenderness	= HorBracing.m_dSlendernessRatio;	//�����
	double  dCompressStress	= HorBracing.m_dFc;					//��������
	double  dComAllowStress	= HorBracing.m_dFcaApo;				//�������
	double  dTensionStress	= HorBracing.m_dFt;					//��������
	double  dTenAllowStress = HorBracing.m_dFta;				//�������
	double  dReqBoltEA		= HorBracing.m_dQtyBoltReq;			//�ʿ亼Ʈ��
	double  dUseBoltEA		= HorBracing.m_dQtyBoltUse;			//��뺼Ʈ��
	
	sText.Format("G%d-%02d", nG+1, nBr+1);
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "B",   "D", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",				m_nSttRow, "E",   "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, sMaterial,			m_nSttRow, "G",   "I", 1, BLACK, 9, FALSE, TA_CENTER);					
	pOut->SetLineBoxText(pXL, sJeWon,				m_nSttRow, "J",   "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, dSlenderness,			m_nSttRow, "Q",   "T", 1, BLACK, 9, FALSE, TA_CENTER, "F1", 1);
	sText.Format("=IF((%s<=150),\"O.K\",\"N.G\")",pOut->GetCellRef("F1"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "U",   "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, dCompressStress,		m_nSttRow, "W",   "Y", 1, BLACK, 9, FALSE, TA_CENTER, "F2", 1);
	pOut->SetLineBoxText(pXL, dComAllowStress,		m_nSttRow, "Z",  "AB", 1, BLACK, 9, FALSE, TA_CENTER, "F3", 1);			
	pOut->SetLineBoxText(pXL, dTensionStress,		m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER, "F4", 1);
	pOut->SetLineBoxText(pXL, dTenAllowStress,		m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "F5", 1);
	pOut->SetLineBoxText(pXL, dReqBoltEA,			m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER, "F6", 1);
	pOut->SetLineBoxText(pXL, dUseBoltEA,			m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "F7", 1);
	sText.Format("=IF(AND((%s<=%s),(%s<=%s),(%s<=%s)),\"O.K\",\"N.G\")", 
		pOut->GetCellRef("F2"),pOut->GetCellRef("F3"),pOut->GetCellRef("F4"),
		pOut->GetCellRef("F5"),pOut->GetCellRef("F6"),pOut->GetCellRef("F7"));
	pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	
	return bOutValid;
}

void CXLCheckListCrossBeam::CrossBeamJewon()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CGeneralBaseStd		*pInStd			= m_pAPlateOutCalcStd->GetGeneralInputStd();
	
	pOut->SetXL(pXL, "1) ��밭�� �� �ּҵβ� ����", m_nSttRow, "B", "B");

	long nQtyCr, SectionType,nTypeRow;
	CString sTypeName, sTypeName2;
	CString sText;

	for (long nType = 0 ; nType < 3; nType++)
	{
		switch(nType)
		{
		case  0:				
				SectionType = SECTION_A;
				sTypeName =  "A";
				break;

		case  1:				
				SectionType = SECTION_P;
				sTypeName =  "P";
				break;
//		case  2:				
//				SectionType = SECTION_B;
//				sTypeName =  "B";
//				break;
		case  2:				
				SectionType = SECTION_C;
				sTypeName =  "C";
				break;				
		}

		nTypeRow = m_nSttRow;
		sText.Format("<%s Type - ����Ÿ��>",sTypeName);
		pOut->SetXL(pXL, sText,  m_nSttRow, "B", "B");
		pOut->SetLineBoxText(pXL, "�� ��",		m_nSttRow, "B", "E", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��",		m_nSttRow, "F", "I", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",			m_nSttRow, "J", "M", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "������ �β�(mm)", m_nSttRow, "N", "U", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "����÷��� �β�(mm)", m_nSttRow, "V", "AC", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�Ϻ��÷��� �β�(mm)", m_nSttRow, "AD", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��", m_nSttRow, "AL", "AP", 2, BLACK, 9, FALSE, TA_CENTER);
		//SetLines(NEXT);
		pOut->SetXL(pXL,"���򺸰���",		m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"���ܼ�",			m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�ּ�", m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "���", m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "����", m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�ּ�", m_nSttRow, "v", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "���", m_nSttRow, "Y", "AA", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "����", m_nSttRow, "AB", "AC", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�ּ�", m_nSttRow, "AD", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "���", m_nSttRow, "AG", "AI", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "����", m_nSttRow, "AJ", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		
		SetLines(NEXT);
		
		for(long nG=0; nG < pBridgeApp->GetGirdersu()-1; nG++)
		{
			CPlateGirderApp	*pGir	= pBridgeApp->GetGirder(nG);
			
			switch(nType)
			{
			case  0:
					nQtyCr	= pGir->GetQtyCrossAType();
				    break;
			case  1:
					nQtyCr	= pGir->GetQtyCrossPType();
				    break;
//			case  2:
//					nQtyCr	= pGir->GetQtyCrossBType();	   
//				    break;
			case  2:
					nQtyCr	= pGir->GetQtyCrossCType();
				    break;				
			}			

			for(long nCr=0; nCr<nQtyCr; nCr++)
			{
				CPlateBasicIndex	*pBx = pGir->GetBxBySectionType(SectionType, nCr);
				CPlateStressOut	pStressOut(pInStd);
 
				CStringArray Arr;
				CMap <CString, LPCTSTR, double, double> Map;
				pStressOut.SectionCheckOutput(Arr, pBx, Map);
				
				if (nCr == 0)
				{
					sTypeName2 = RetCrossBeamType(Map["MType"], pBx);					
				}				

				sText.Format("G%d-G%d-%s%d", nG+1, nG+2, sTypeName, nCr+1);
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER);

				//Map["JuType"]
				CString sJuType    = pBridgeApp->GetMaterial(CP_F_U, pBx);
				pOut->SetLineBoxText(pXL, sJuType,			m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, Map["HStiff"],	m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER,"",0);
				pOut->SetLineBoxText(pXL, Map["Twmin"],		m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER,"TW");
				pOut->SetLineBoxText(pXL, Map["Tw"],		m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER,"TWMIN");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("TW"), pOut->GetCellRef("TWMIN"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "T", "U", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Tfumin"],	m_nSttRow, "V", "X", 1, BLACK, 9, FALSE, TA_CENTER,"TFU");
				pOut->SetLineBoxText(pXL, Map["Tfu"],		m_nSttRow, "Y", "AA", 1, BLACK, 9, FALSE, TA_CENTER,"TFUMIN");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("TFU"), pOut->GetCellRef("TFUMIN"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AB", "AC", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Tflmin"],	m_nSttRow, "AD", "AF", 1, BLACK, 9, FALSE, TA_CENTER,"TFL");
				pOut->SetLineBoxText(pXL, Map["Tfl"],		m_nSttRow, "AG", "AI", 1, BLACK, 9, FALSE, TA_CENTER,"TFLMIN");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("TFL"), pOut->GetCellRef("TFLMIN"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AJ", "AK", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, "",			m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}			
		}//end for nG
		sText.Format("<%s Type - %s>",sTypeName, sTypeName2);
		pOut->SetXL(pXL, sText,  nTypeRow, "B", "B");
		SetLines(NEXT);
	}//end for nType
	
}

void CXLCheckListCrossBeam::CrossBeamMoment()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CGeneralBaseStd	*pInStd			= m_pAPlateOutCalcStd->GetGeneralInputStd();
	
	pOut->SetXL(pXL, "2) ���� ����", m_nSttRow, "B", "B");

	long nQtyCr, SectionType,nTypeRow;
	CString sTypeName, sTypeName2;
	CString sText;

	for (long nType = 0 ; nType < 3; nType++)
	{
		switch(nType)
		{
		case  0:				
				SectionType = SECTION_A;
				sTypeName =  "A";
				break;

		case  1:				
				SectionType = SECTION_P;
				sTypeName =  "P";
				break;
//		case  2:				
//				SectionType = SECTION_B;
//				sTypeName =  "B";
//				break;
		case  2:				
				SectionType = SECTION_C;
				sTypeName =  "C";
				break;				
		}

		nTypeRow = m_nSttRow;
		sText.Format("<%s Type - ����Ÿ��>",sTypeName);
		pOut->SetXL(pXL, sText,  m_nSttRow, "B", "B");
		pOut->SetLineBoxText(pXL, "�� ��",			m_nSttRow, "B", "E", 3, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "F", "I", 3, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "J", "M", 3, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�������� (MPa)", m_nSttRow, "N", "V", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� �� �� (MPa)", m_nSttRow, "W", "AN", 1, BLACK, 9, FALSE, TA_CENTER);		
		pOut->SetLineBoxText(pXL, "���",			m_nSttRow, "AO", "AP", 3, BLACK, 9, FALSE, TA_CENTER);
		//SetLines(NEXT);
		pOut->SetXL(pXL,"��  ��",					m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"��  ��",					m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"���Ʈ",					m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"���ܷ�",					m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "N", "P", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "Q", "S", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��",			m_nSttRow, "T", "V", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "W", "Y", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "Z", "AB", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��",			m_nSttRow, "AC", "AE", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AF", "AH", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AI", "AK", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��",			m_nSttRow, "AL", "AN", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "Z", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL,"(kN��m)",					 m_nSttRow, "F", "I", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"(kN)",						m_nSttRow, "J", "M", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "Q", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "W", "Y", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "Z", "AB", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,"�� ��",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		//SetLines(NEXT);
		
		for(long nG=0; nG < pBridgeApp->GetGirdersu()-1; nG++)
		{
			CPlateGirderApp	*pGir	= pBridgeApp->GetGirder(nG);
			
			switch(nType)
			{
			case  0:
					nQtyCr	= pGir->GetQtyCrossAType();
				    break;
			case  1:
					nQtyCr	= pGir->GetQtyCrossPType();
				    break;
//			case  2:
//					nQtyCr	= pGir->GetQtyCrossBType();	   
//				    break;
			case  2:
					nQtyCr	= pGir->GetQtyCrossCType();
				    break;				
			}			

			for(long nCr=0; nCr<nQtyCr; nCr++)
			{
				CPlateBasicIndex	*pBx = pGir->GetBxBySectionType(SectionType, nCr);
				CPlateStressOut	pStressOut(pInStd);
 
				CStringArray Arr;
				CMap <CString, LPCTSTR, double, double> Map;
				pStressOut.SectionCheckOutput(Arr, pBx, Map);
				
				if (nCr == 0)
				{
					sTypeName2 = RetCrossBeamType(Map["MType"], pBx);					
				}				

				sText.Format("G%d-G%d-%s%d", nG+1, nG+2, sTypeName, nCr+1);
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "B", "E", 1, BLACK, 9, FALSE, TA_CENTER);

				//Map["JuType"]
				//CString sJuType    = pBridgeApp->GetMaterial(CP_F_U, pBx);
				pOut->SetLineBoxText(pXL, Map["Mmu"],		m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, Map["Msu"],		m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
				pOut->SetLineBoxText(pXL, Map["Mv"],		m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER,"MV");
				pOut->SetLineBoxText(pXL, Map["Mva"],		m_nSttRow, "Q", "S", 1, BLACK, 9, FALSE, TA_CENTER,"MVA");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("MV"), pOut->GetCellRef("MVA"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "T", "V", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Mfu"],		m_nSttRow, "W", "Y", 1, BLACK, 9, FALSE, TA_CENTER,"MFU");
				pOut->SetLineBoxText(pXL, Map["Mfca"],		m_nSttRow, "Z", "AB", 1, BLACK, 9, FALSE, TA_CENTER,"MFCA");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("MFU"), pOut->GetCellRef("MFCA"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AC", "AE", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, Map["Mfl"],		m_nSttRow, "AF", "AH", 1, BLACK, 9, FALSE, TA_CENTER,"MFL");
				pOut->SetLineBoxText(pXL, Map["Mfta"],		m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER,"MFTA");
				sText.Format("=IF(%s <= %s, \"O.K\",\"N.G\")", pOut->GetCellRef("MFL"), pOut->GetCellRef("MFTA"));
				pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);

				pOut->SetLineBoxText(pXL, "",				m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
				SetLines(NEXT);
			}			
		}//end for nG
		sText.Format("<%s Type - %s>",sTypeName, sTypeName2);
		pOut->SetXL(pXL, sText,  nTypeRow, "B", "B");
		SetLines(NEXT);
	}//end for nType
}

CString CXLCheckListCrossBeam::RetCrossBeamType(double sType, CPlateBasicIndex *pBx)
{
	if (sType == 2)
	{
		switch (pBx->GetSection()->GetCrossBeam()->GetType())
		{
			case CR_TYPE_HSTEEL:
				return "����Ÿ��";
			case CR_TYPE_MANUFACTURE:
				return "���� Ÿ��";
			case CR_TYPE_VSTIFF:
				return "���������翬��";
		}
	}
	else
	{
		switch (pBx->GetSection()->GetVBracing()->GetType())
		{
			case TYPE_VBRACING:
				return "�극�̽�(V��)";
			case TYPE_INVVBRACING:
				return "�극�̽�(��V��)";			
		}		
	}
	return "";
}

void CXLCheckListCrossBeam::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}

CString CXLCheckListCrossBeam::GetStrValue(double dValue)
{
	CString str;
	if(dValue == 0)	
		str.Format("'-");
	else 
		str.Format("%.3f", dValue);
	return str;
}