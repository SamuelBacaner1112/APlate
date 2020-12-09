// XLCheckListStiff.cpp: implementation of the CXLCheckListStiff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplateoutcalc.h"
#include "XLCheckListStiff.h"
#include "XLEtcCheck.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcStd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLCheckListStiff::CXLCheckListStiff(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_pAPlateOutCalcStd	= pStd;
}

CXLCheckListStiff::~CXLCheckListStiff()
{

}

void CXLCheckListStiff::Stiff()
{
	//CPlateBridgeApp		*pDB 		    = m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();

	m_nSttRow			= 1;

	pXL->SetCellWidth(0, 255, 1);//
	
	pOut->SetXL(pXL, "������� CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "8. ������ ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);	

	pOut->SetXL(pXL, "��) WEB ������", m_nSttRow, "A", "A");	
	SetLines(NEXT);
		
	VStiff();
	HStiff();

	pOut->SetXL(pXL, "��) ������ ������ �� ��� ������", m_nSttRow, "A", "A");	
	SetLines(NEXT);
	JijumStiff();	

}

//���� ������
void CXLCheckListStiff::VStiff()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	//CCalcGeneral		CalcGeneral(pDataManage);
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	

	CString sText;
	long	nIndex	= 0;
	long	nG		= 0;
	double	dSta	= 0;

	pOut->SetXL(pXL, "1) ����������",				m_nSttRow, "B", "B");

		
	if(pOpt->GetDesignVStiffInstall()!=0)//0�̸� ��ġ...1�̸� �̼�ġ...
	{
		pOut->SetXL(pXL, " ��ġ�� ���������簡 �����ϴ�.",				m_nSttRow, "B", "B");
		SetLines(NEXT);
		return;
	}

	pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� �� ��(a/b)",			m_nSttRow, "E", "L", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�������� �ּ���(mm)",	m_nSttRow, "M", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�������� �ּҵβ�(mm)",	m_nSttRow, "W", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� ��(mm4)",				m_nSttRow, "AG", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "�ִ�",					m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "M", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "Q", "T", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "W", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		long	nSec	= atoi(pFrameSec->m_szName);
		nG   = -1;
		dSta = 0;
		ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
		
		CPlateGirderApp		*pGir	 = pDB->GetGirder(nG);
		CPlateBasicIndex	*pBx	 = pGir->GetBxByStation(dSta);
		CDesignStiff		DesignStiff(pCalcStd);
		
		DesignStiff.SetVerStiff(pBx);
		
		sText.Format("�ܸ� %d", nIndex+1);
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dMaxA_B,		m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER, "GapMax", 1);//���ݺ��ִ�
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dUseA_B,		m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER, "GapUse", 2);//���ݺ���
		sText.Format("=IF(%s>=%s,\"OK\",\"NG\")", pOut->GetCellRef("GapMax"), pOut->GetCellRef("GapUse"));		
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER);//����
				
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dReqW,	m_nSttRow, "M", "P", 1, BLACK, 9, FALSE, TA_CENTER, "bMin", 1);//���������ʿ�
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dUseW,	m_nSttRow, "Q", "T", 1, BLACK, 9, FALSE, TA_CENTER, "bUse", 1);//�����������
		sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("bMin"), pOut->GetCellRef("bUse"));		
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER);//����	
		
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dReqT,	m_nSttRow, "W", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "TMin", 1);//������β��ʿ�
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dUseT,	m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER, "TUse", 1);//������β����
		sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("TMin"), pOut->GetCellRef("TUse"));		
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "AE", "AF", 1, BLACK, 9, FALSE, TA_CENTER);//����
		
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dReqI,		m_nSttRow, "AG", "AJ", 1, BLACK, 9, FALSE, TA_CENTER, "Ireq", 0);//�����ʿ�
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtVerStiff.m_dUseI,		m_nSttRow, "AK", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "IUsed", 0);//�������
		sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("Ireq"), pOut->GetCellRef("IUsed"));		
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);//����
		
		SetLines(NEXT);


		nIndex++;
	}

	SetLines(NEXT);

}

//���򺸰���
void CXLCheckListStiff::HStiff()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	
	CString sText;
	long	nIndex	= 0;
	long	nG		= 0;
	double	dSta	= 0;
	
	pOut->SetXL(pXL, "2) ���򺸰���",				m_nSttRow, "B", "B",0);
	pOut->SetXL(pXL, "���򺸰����� ��ġ�� ���������κ����� �Ÿ�(mm)",			m_nSttRow, "P", "P", 1, BLACK, 9, FALSE, TA_LEFT);
	
	pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "������ �ּҵβ�(mm)",	m_nSttRow, "E", "L", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� ��(mm4)",				m_nSttRow, "M", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "M", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "Q", "T", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);

	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		nG   = -1;
		dSta = 0;
		long	nSec	= atoi(pFrameSec->m_szName);
		ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);

		CPlateGirderApp		*pGir	 = pDB->GetGirder(nG);
		CPlateBasicIndex	*pBx	 = pGir->GetBxByStation(dSta, BX_PLATEHSTIFF);//���򺸰��簡 �ִ� ��ġ�� ã�ƾ� ��..�ƴϸ� NULL ����...2006.08.09	
		
		sText.Format("�ܸ� %d", nIndex+1);
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
		
		long	nNr		= max(pBx->GetWebHStiff()->GetTotalDansu(TRUE, TRUE),pBx->GetWebHStiff()->GetTotalDansu(TRUE, FALSE));
		if (nNr == 0)
		{
			sText.Format("��ġ�� ���򺸰��簡 �����ϴ�.");
			pOut->SetLineBoxText(pXL, sText,	m_nSttRow, "E", "V", 1, BLACK, 9, FALSE, TA_CENTER);//
			SetLines(NEXT);
			nIndex++;
			continue;
		}
		
		CDesignStiff		DesignStiff(pCalcStd);
		DesignStiff.SetHorStiff(pBx);
		
		sText.Format("%.6f", DesignStiff.m_strtHorStiff.m_dReqWebT);
		double	dHStiff_T	= max(pBx->GetWebHStiff()->m_dThickHStiff[0], pBx->GetWebHStiff()->m_dThickHStiff[1]);
		double	dHStiff_B	= max(pBx->GetWebHStiff()->m_dWidthHStiff[0], pBx->GetWebHStiff()->m_dWidthHStiff[1]);
		double	dtmin		= min(dHStiff_T, dHStiff_B/16);
		
		sText.Format("%.3f", dtmin);
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER, "Req2", 1);//���������ʿ�
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtHorStiff.m_dUseT,		m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER, "Use2", 1);//�����������
		sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("Req2"), pOut->GetCellRef("Use2"));		
		pOut->SetLineBoxText(pXL, sText,										m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER);//����	
		
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtHorStiff.m_dReqI,		m_nSttRow, "M", "P", 1, BLACK, 9, FALSE, TA_CENTER, "Req3", 0);//������β��ʿ�
		pOut->SetLineBoxText(pXL, DesignStiff.m_strtHorStiff.m_dUseI,		m_nSttRow, "Q", "T", 1, BLACK, 9, FALSE, TA_CENTER, "Use3", 0);//������β����
		sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("Req3"), pOut->GetCellRef("Use3"));		
		pOut->SetLineBoxText(pXL, sText,									m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER);//����
	
		SetLines(NEXT);
		
		nIndex++;
	}
	
	SetLines(NEXT);
}


void CXLCheckListStiff::JijumStiff()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CXLControl			*pXL			= m_pXL;
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	//CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	//CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	//CCalcGeneral		CalcGeneral(pDataManage);	
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);


	CString strLeft, strRight;
	CString	sText, sGir;		
	long	nQtyJoint	= pFEMManage->GetJointSize();
	long nStt;
	int nCnt;
	
	
//	pOut->SetXL(pXL, "3) ������ ������ �� ��� ������",				m_nSttRow, "B", "B");	

	pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "J", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "������ ������",			m_nSttRow, "K", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "��� ������",			m_nSttRow, "AA", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "�ּҵβ�(mm)",			m_nSttRow, "K", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����(MPa)",				m_nSttRow, "S", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ּҵβ�(mm)",			m_nSttRow, "AA", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����(MPa)",				m_nSttRow, "AI", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "B", "D", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�Ŵ�",					m_nSttRow, "E", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "K", "M", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ۿ�",					m_nSttRow, "S", "U", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "V", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "Y", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�",					m_nSttRow, "AA", "AC", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AD", "AF", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AG", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ۿ�",					m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "���",					m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",					m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);

	for(long nK = 0; nK <= pDB->m_nQtyJigan; nK++)//������ �Ŵ����� ���ؾ� ��...��...
	{
		//CPlateBasicIndex *pBx = pDB->GetGirder(-1)->GetBxOnJijum(nK);
		CString szName = pDB->m_strJijumName[nK];
		nCnt=0;
		nStt = m_nSttRow;

		long nJijum = 0;
		
		for(long nJoint=0; nJoint<nQtyJoint; nJoint++)//��ü ����Ʈ�� ������...
		{
			CJoint* pJ = pFEMManage->GetJoint((unsigned short)nJoint);
			long nGir = pJ->m_LongAttrib[JOINT_GIR];
			
			if(pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE)//���� �ִ� ����Ʈ�� ����
			{					
				CString szName2 = pDB->m_strJijumName[nJijum];
				
				if (szName == szName2)//������ ������ �Ŵ��� ���ϰ�...
				{
					//������ ������...					
					CDesignJijumStiff   pJStiff(pCalcStd, pCalcStd->GetBxByJoint(pJ), nJoint);
										
					double Ji_ts = pJStiff.m_strJijumStiff.m_dUseT;//��� �β�..ts
					double Ji_dT_req = pJStiff.m_strJijumStiff.m_dReqT;//���� 3.4.2.2					
					double Ji_dApplyF = pJStiff.m_strJijumStiff.m_dFc;//�ۿ� ����
					double Ji_dAllowF = pJStiff.m_strJijumStiff.m_dFca;//��� ����

					double ja_ts = pJStiff.m_strJackupStiff.m_dUseT;
					double ja_dT_req = pJStiff.m_strJackupStiff.m_dReqT;
					double ja_dAlllyF = pJStiff.m_strJackupStiff.m_dFc;
					double ja_dAllowF = pJStiff.m_strJackupStiff.m_dFca;

					//--------------------------------------------------------------------
					sGir.Format("G%d",nGir+1);
					pOut->SetLineBoxText(pXL, sGir,			m_nSttRow, "E", "G", 1);				
					sText.Format("%d", pJ->m_nIdx+1);				
					pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "H", "J", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

					pOut->SetLineBoxText(pXL, Ji_dT_req,	m_nSttRow, "K", "M", 1, BLACK, 9, FALSE, TA_CENTER, "Req1", 1);
					pOut->SetLineBoxText(pXL, Ji_ts,		m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER, "Use1", 1);
					sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("Req1"), pOut->GetCellRef("Use1"));
					pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
					pOut->SetLineBoxText(pXL, Ji_dApplyF,	m_nSttRow, "S", "U", 1, BLACK, 9, FALSE, TA_CENTER, "Req2", 1);
					pOut->SetLineBoxText(pXL, Ji_dAllowF,	m_nSttRow, "V", "X", 1, BLACK, 9, FALSE, TA_CENTER, "Use2", 1);
					sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("Req2"), pOut->GetCellRef("Use2"));
					pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "Y", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

					pOut->SetLineBoxText(pXL, ja_dT_req,	m_nSttRow, "AA", "AC", 1, BLACK, 9, FALSE, TA_CENTER, "Req3", 1);
					pOut->SetLineBoxText(pXL, ja_ts,		m_nSttRow, "AD", "AF", 1, BLACK, 9, FALSE, TA_CENTER, "Use3", 1);
					sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("Req3"), pOut->GetCellRef("Use3"));
					pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AG", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
					pOut->SetLineBoxText(pXL, ja_dAlllyF,	m_nSttRow, "AI", "AK", 1, BLACK, 9, FALSE, TA_CENTER, "Req4", 1);
					pOut->SetLineBoxText(pXL, ja_dAllowF,	m_nSttRow, "AL", "AN", 1, BLACK, 9, FALSE, TA_CENTER, "Use4", 1);
					sText.Format("=IF(%s<%s,\"OK\",\"NG\")", pOut->GetCellRef("Req4"), pOut->GetCellRef("Use4"));
					pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AO", "AP", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);


					SetLines(NEXT);
					nCnt++;
				}
				
				if (nJijum == pDB->m_nQtyJigan)//�ݺ��Ǵ� �Ŵ��� ������ �Ѱ����� ã������...
					nJijum=0;
				else
					nJijum++;

			}
			
		}
		pOut->SetLineBoxText(pXL, szName,	nStt, "B", "D", nCnt);
	}	
		
}



void CXLCheckListStiff::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}

///////////////////////////////////////////////////////////////////////////////////
//��š񢱢����� ���¥åĥťƥǥ�������ҥ��ɥ��������f ����������
//	���£��������¡áġҡ����� �� �� �� kgf/�� t��m  ����������������
//  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ������������¨èĨ�
//  ��������������������������  �� ��
//  �� �� �� ������ �� ���� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
///////////////////////////////////////////////////////////////////////////////////// 