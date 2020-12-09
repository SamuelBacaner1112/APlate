// XLCheckListFloor.cpp: implementation of the CXLCheckListFloor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplateoutcalc.h"
#include "XLCheckListFloor.h"
#include "APlateOutCalcStd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define EPSILON 0.000000001

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLCheckListFloor::CXLCheckListFloor(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_pAPlateOutCalcStd	= pStd;
}

CXLCheckListFloor::~CXLCheckListFloor()
{

}

void CXLCheckListFloor::Floor()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	BOOL bTendon = pBridgeApp->m_bTendonInputComplete;

	if (bTendon) // ������ �ִ� ���
	{
		With_Tendon_Floor();
	}
	else // ������ ���� ���
	{
		Without_Tendon_Floor();	
	}
}

void CXLCheckListFloor::With_Tendon_Floor()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CXLControl			*pXL			= m_pXL;
	CCalcFloor			CalcFloor(pDataManage);
	
	CString	sText;
	CString sTitle;

	// Title
	m_nSttRow = 1;
	pOut->ClearCellRef();	
	pXL->SetCellWidth(0, 255, 1);	
	pOut->SetXL(pXL, "������� CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "1.�ٴ��� ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);
	SetLines(NEXT);

	// ���� ĵƿ������, ���� ĵƿ������, �߾Ӻ� �ٴ��� - ���̺� �ۼ�
	for(int n=0; n<3; n++)
	{
		sTitle = (n==0)? "���� ĵƿ������" : (n==1)? "���� ĵƿ������" : "�߾Ӻ� �ٴ���";			
		pOut->SetXL(pXL, sTitle,					m_nSttRow, "A", "A", 1, BLACK, 9, TRUE, TA_LEFT);

		// Load Calculated Data
		CMap <CString, LPCTSTR, double, double> Map;
		CalcFloor.CalcCantileverUltimateLoadCheck(pCalcStd, Map, n);

		double dPiMn1	= Map["Mn1"];
		double dPiMn2	= Map["Mn2"];
		double dQp		= Map["Qp"];
		double dBeta	= Map["Beta"];
		double dMcr		= Map["Mcr"];
		double dMu		= Map["Mu"];
		double dMn		= min(dPiMn1, dPiMn2);

		pOut->SetLineBoxText(pXL, "�� ��",						m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� �� �� �� �� ��",			m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� �� �� ��",				m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
 
		pOut->SetLineBoxText(pXL, "�ٴ��� �ּҵβ� ����",		m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "������ �ٴ��ǿ� ������Ʈ������ ������ ��� �ּҵβ� ���� ���ʿ�",		
																m_nSttRow, "L", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",							m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
		
		// �������� �ܸ����
		pOut->SetLineBoxText(pXL, "����\n����\n�ܸ�\n����",		m_nSttRow, "B", "D", 4, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "���Ѱ��� ����",				m_nSttRow, "E", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("Mu = %.3f kN.m", dMu);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("��Mn = %.3f kN.m", dPiMn1);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dPiMn1>dMu)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		pOut->SetLineBoxText(pXL, "�����ڰ��� ����",			m_nSttRow, "E", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("Mu = %.3f kN.m", dMu);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("��Mn = %.3f kN.m", dPiMn2);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dPiMn2>dMu)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		pOut->SetLineBoxText(pXL, "�ִ� ����� ����",			m_nSttRow, "E", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("0.36��� = %.3f", 0.36*dBeta);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("qp = %.3f", dQp);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (0.36*dBeta>dQp)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		pOut->SetLineBoxText(pXL, "�ּ� ���緮 ����",			m_nSttRow, "E", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("1.2Mcr = %.3f kN.m", 1.2*dMcr);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("��Mn = %.3f kN.m", dMn);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dMn>1.2*dMcr)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		// Load Calculated Data
		double dBar_Area = Map["Bar_Area"];
		double dSup_Req1 = Map["Sup_Req1"];
		double dSup_Req2 = Map["Sup_Req2"];
		double dSup_Req4 = Map["Sup_Req4"];
		double dSup_Min  = Map["Sup_Min"];	
		double dSup_Area1= Map["Sup_Area1"];
		double dSup_Area2= Map["Sup_Area2"];

		// ���ö�� ö�ٷ� ����
		pOut->SetLineBoxText(pXL, "���ö�� ö�ٷ� ����",		m_nSttRow, "B", "K", 3, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("Ⱦ����ö�� = %.3f mm��", dSup_Req1);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f mm��", dBar_Area);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dBar_Area>dSup_Req1)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		sText.Format("�ּ�ö�ٷ� = %.3f mm��", dSup_Req1);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f mm��", dSup_Min);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dSup_Min>dSup_Req1)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		sText.Format("���ö�ٷ� = %.3f mm��", dSup_Req2);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f mm��", dSup_Area1);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dSup_Area1>dSup_Req2)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		// �ܺ� ���ö�� ö�ٷ� ����
		pOut->SetLineBoxText(pXL, "�ܺ� ���ö�� ö�ٷ� ����",		m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f mm��", dSup_Req4);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f mm��", dSup_Area2);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dSup_Area2>dSup_Req4)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		// ó������
		if (n==0 || n==1)	// ����, ����
		{
			double dDeflect = (n==0)? fabs(GetDeflection(0, "DB MIN")) : fabs(GetDeflection(2, "DB MIN"));
			double dAllowDef= GetAllowDeflection(n); 	

			pOut->SetLineBoxText(pXL, "ó������",					m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dAllowDef);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dDeflect);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			sText = (dAllowDef>dDeflect)? "O.K" : "N.G";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);

		}
		else	// �߾�
		{
			double dDeflect_DB = fabs(GetDeflection(1, "DB MIN"));
			double dDeflect_TK = fabs(GetDeflection(1, "TK MIN"));
			double dDeflect_TR = fabs(GetDeflection(1, "TR MIN"));
			double dAllowDef= GetAllowDeflection(n); 	

			pOut->SetLineBoxText(pXL, "ó��\n����",					m_nSttRow, "B", "D", 3, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "Ȱ �� ��",					m_nSttRow, "E", "K", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dAllowDef);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dDeflect_DB);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			sText = (dAllowDef>dDeflect_DB)? "O.K" : "N.G";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);

			pOut->SetLineBoxText(pXL, "��ũ ����",					m_nSttRow, "E", "K", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dAllowDef);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dDeflect_TK);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			sText = (dAllowDef>dDeflect_TK)? "O.K" : "N.G";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);

			pOut->SetLineBoxText(pXL, "Ʈ���Ϸ� ����",				m_nSttRow, "E", "K", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dAllowDef);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
			sText.Format("%.3f mm", dDeflect_TR);
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			sText = (dAllowDef>dDeflect_TR)? "O.K" : "N.G";
			pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
		}

		// Load Calculated Data
		CMap <CString, LPCTSTR, double, double> Map2;
		CalcFloor.CalcPreStressCheck(pCalcStd, Map2, n);

		double dForced_Top_Before = Map2["Force_Top_Before"];
		double dForced_Top_After  = Map2["Force_Top_After"];
		double dForced_Bot_Before = Map2["Force_Bot_Before"];
		double dForced_Bot_After  = Map2["Force_Bot_After"];
		
		double dAllow_Top_Before  = Map2["Allow_Top_Before"];
		double dAllow_Top_After   = Map2["Allow_Top_After"];
		double dAllow_Bot_Before  = Map2["Allow_Bot_Before"];
		double dAllow_Bot_After   = Map2["Allow_Bot_After"];


		// ������Ʈ���� ���� ������ ���� ����
		pOut->SetLineBoxText(pXL, "������Ʈ���� ���� \n������ ���� ����",					m_nSttRow, "B", "K", 2, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("������ = %.3f MPa", dAllow_Top_Before);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f MPa", dForced_Top_Before);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dAllow_Top_Before>dForced_Top_Before)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		sText.Format("�Ͽ����� = %.3f MPa", dAllow_Bot_Before);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f MPa", dForced_Bot_Before);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dAllow_Bot_Before>dForced_Bot_Before)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
		
		
		// �������� �ۿ���� ���� ����
		pOut->SetLineBoxText(pXL, "�������� �ۿ���� \n���� ����",					m_nSttRow, "B", "K", 2, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("������ = %.3f MPa", dAllow_Top_After);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f MPa", dForced_Top_After);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dAllow_Top_After>dForced_Top_After)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);


		sText.Format("�Ͽ����� = %.3f MPa", dAllow_Bot_After);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("%.3f MPa", dForced_Bot_After);
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		sText = (dAllow_Bot_After>dForced_Bot_After)? "O.K" : "N.G";
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
		SetLines(NEXT);
	}

	// Load Calculated Data
	CMap <CString, LPCTSTR, double, double> Map3;
	CalcFloor.CalcCantileverInnerPositionCheck(Map3);

	double dReq_As	 =	Map3["Req_As"];
	double dBar_Area =	Map3["Bar_Area"];
	double dUr		 =	Map3["Ur"];
	
	// ���� ������ ������� ö�� ����
	pOut->SetXL(pXL, "���� ������ ������� ö�� ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);
	pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� �� �� �� �� ��",		m_nSttRow, "H", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� �� �� ��",			m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);

	pOut->SetLineBoxText(pXL, "ö�ٷ� ����",			m_nSttRow, "B", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("��ũ��Ʈ �ܸ����� %.1f%% �� %.3f mm��", CalcFloor.GetFloorConcAreaCoefficient(), dReq_As);
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "H", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("���ö�ٷ� %.3f mm��", dBar_Area);
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
	sText = (dBar_Area>dReq_As)? "O.K" : "N.G";
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);

	pOut->SetLineBoxText(pXL, "����� ����",			m_nSttRow, "B", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("�ּ� ����� %.4f mm/mm��", CalcFloor.GetFloorJujangStd());
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "H", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("�������� %.4f mm/mm��", dUr);
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
	sText = (dUr>CalcFloor.GetFloorJujangStd())? "O.K" : "N.G";
	pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
}

void CXLCheckListFloor::Without_Tendon_Floor()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CXLControl			*pXL			= m_pXL;
	CCalcFloor			CalcFloor(pDataManage);
	CGlobarOption		*pOpt			= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();
	CCalcGeneral		CalcGeneral(m_pAPlateOutCalcStd->GetDataManage());

	CString	sText;
	CString sTitle;
	long	nPos;
	double	dMinT, dUseT;
	double	Asreq, UseAs, UseAs2;//�Ϲݺ� ��ö�� ...
	CString sDropText	= _T("");

	m_nSttRow = 1;
	pOut->ClearCellRef();

	pXL->SetCellWidth(0, 255, 1);//
	
	pOut->SetXL(pXL, "������� CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "1.�ٴ��� ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);
	SetLines(NEXT);

	for(int n=0; n<3; n++)
	{
		if (n==0)
		{
			nPos = FLOOR_LEFT;
			sTitle = "���� ĵƿ������";			
			dUseT = pBridgeApp->m_dThickSlabBase;
		}
		else if(n==1)
		{
			nPos = FLOOR_RIGHT;
			sTitle = "���� ĵƿ������";			
			dUseT = pBridgeApp->m_dThickSlabBase;			
		}
		else if(n==2)
		{
			nPos = FLOOR_MID;
			sTitle = "�߾Ӻ� �ٴ���";			
			dUseT = pBridgeApp->m_dThickSlabBase - pBridgeApp->m_dThickSlabHunch;
		}
		
		if(n == 2)
		{
			if(pOpt->GetSlabCentDesignMethod() == 1)
			{
				if(CalcGeneral.CheckExperienceDesign(TRUE) || CalcGeneral.CheckExperienceDesign(FALSE))
					continue;
			}
		}

		dMinT = pCalcData->CALC_CANTILEVER_DATA[nPos].m_Req_T;;		// �ٴ��� �ʿ�β�
		Asreq = pCalcData->CALC_CANTILEVER_DATA[nPos].m_Rein_Asreq;	// �ʿ� ö�ٷ�...
		UseAs = pCalcData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs;	// �Ϲݺ� ���ö�ٷ�..
		UseAs2 = pCalcData->CALC_CANTILEVER_DATA[nPos].m_Rein_UseAs2;	// �ܺ� ���ö�ٷ�...

		CString sDropText	= _T("");	
		double	dSigmaY		= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
		double	dCoeff		= nPos == FLOOR_MID ? 28 : 10;	
		double	dT			= CalcFloor.GetCantileverDroop(nPos);

		if(dSigmaY==400.0)//�ٴ����� ���β��� Tmin = L/10 =
		{
			sDropText.Format("�ٴ����� ���β��� Tmin = L/%.0f =", dCoeff);		
		}
		else
		{
			sDropText.Format("���β��� Tmin=L/%.0f��(0.43+%.0f/700)", dCoeff, dSigmaY);
		}

		CMap <CString, LPCTSTR, double, double> Map;
		CalcFloor.SetCantileverCrack(Map, nPos);
			
		double	dW			= Map["CRACK_W"];//��� �տ���.
		double	dWa			= Map["CRACK_Wa"]; // .��� �տ���

		pOut->SetXL(pXL, sTitle,	m_nSttRow, "A", "A", 1, BLACK, 9, FALSE, TA_LEFT);
	
		pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� �� �� �� �� ��",		m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� �� �� ��",			m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);

		pOut->SetLineBoxText(pXL, "�ٴ��� �ּҵβ�����",	m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_LEFT);

		pOut->SetXL(pXL, "�ʿ�β�",						m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, dMinT,								m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_LEFT, "NThick");
		pOut->SetXL(pXL, "mm",								m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, "���β�",						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, dUseT,								m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_LEFT, "UseThick");
		pOut->SetXL(pXL, "mm",								m_nSttRow, "AJ", "AK", 0, BLACK, 9, FALSE, TA_LEFT);
		sText.Format("=IF(%s>220, IF(%s>%s, \"N.G.\", \"O.K.\"), \"N.G.\")", 
			pOut->GetCellRef("UseThick"), pOut->GetCellRef("NThick"), pOut->GetCellRef("UseThick"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetLineBoxText(pXL, "�ٴ��� ö�ٷ� ����",		m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_LEFT);

		pOut->SetXL(pXL, "�ʿ�ö�ٷ�",						m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, Asreq,								m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_LEFT, "NSlab1");
		pOut->SetXL(pXL, "mm��",					 		 m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, "���ö�ٷ�",						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, UseAs,								m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_LEFT, "UseSlab1");
		pOut->SetXL(pXL, "mm��",							 m_nSttRow, "AJ", "AK", 0, BLACK, 9, FALSE, TA_LEFT);		
		sText.Format("=IF(%s>%s, \"N.G.\", \"O.K.\")", 
			pOut->GetCellRef("NSlab1"), pOut->GetCellRef("UseSlab1"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetLineBoxText(pXL, "�ٴ��� �ܺκ��� ����",	m_nSttRow, "B", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_LEFT);

		pOut->SetXL(pXL, "�ʿ�ö�ٷ�",						m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, Asreq*2,							m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_LEFT, "NSlab2");
		pOut->SetXL(pXL, "mm��",					 		 m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, "���ö�ٷ�",						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, UseAs2,							m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_LEFT, "UseSlab2");
		pOut->SetXL(pXL, "mm��",							 m_nSttRow, "AJ", "AK", 0, BLACK, 9, FALSE, TA_LEFT);
		sText.Format("=IF(%s>%s, \"N.G.\", \"O.K.\")", 
			pOut->GetCellRef("NSlab2"), pOut->GetCellRef("UseSlab2"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_LEFT);

		//pOut->SetXL(pXL, "�ʿ��ġ����",					m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_LEFT);
		//pOut->SetXL(pXL, dDead_L,							m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_LEFT, "NSlab3");
		//pOut->SetXL(pXL, "m",					 			m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_LEFT);
		//pOut->SetXL(pXL, "�����ġ����",	 				m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_LEFT);
		//pOut->SetXL(pXL, dDead_L,							m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_LEFT, "UseSlab3");
		//pOut->SetXL(pXL, "m",								m_nSttRow, "AJ", "AK", 0, BLACK, 9, FALSE, TA_LEFT);
		//sText.Format("=IF(%s>%s, \"N.G.\", \"O.K.\")", 
			//pOut->GetCellRef("NSlab3"), pOut->GetCellRef("UseSlab3"));
		//pOut->SetXL(pXL, sText,								m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);


		pOut->SetLineBoxText(pXL, "��뼺 ����",			m_nSttRow, "B", "G", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "ó������",				m_nSttRow, "H", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "AP", 1, BLACK, 9, FALSE, TA_LEFT);
			
		pOut->SetXL(pXL, sDropText,							m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, dT, 				 				m_nSttRow, "Z", "AC", 0, BLACK, 9, FALSE, TA_CENTER, "Tmin");
		pOut->SetXL(pXL, "mm",								m_nSttRow, "AD", "AD", 0, BLACK, 9, FALSE, TA_LEFT);	
		sText.Format("=IF(%s>=%s, \" �����̹Ƿ� ó������ �ʿ�.\",\" �̻��̹Ƿ� ó������ �ʿ����.\")",
			pOut->GetCellRef("Tmin"), pOut->GetCellRef("UseThick"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "AE", "AP", 1, RED, 9, FALSE, TA_LEFT);

		pOut->SetLineBoxText(pXL, "�տ�����",				m_nSttRow, "H", "K", 1, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "L", "X", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_LEFT);

		pOut->SetXL(pXL, "���տ���",						m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, dWa,								m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_LEFT, "NSlab4");
		pOut->SetXL(pXL, "mm",					 			m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, "���տ���",						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_LEFT);
		pOut->SetXL(pXL, dW,								m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_LEFT, "UseSlab4");
		pOut->SetXL(pXL, "mm",								m_nSttRow, "AJ", "AK", 0, BLACK, 9, FALSE, TA_LEFT);
		sText.Format("=IF(%s>=%s, \"N.G.\", \"O.K.\")", 
			pOut->GetCellRef("UseSlab4"), pOut->GetCellRef("NSlab4"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
		SetLines(NEXT);
	}
	
	//������ ����� 
	if(pOpt->GetSlabCentDesignMethod() == 1)
	{
		CString	sRebar					= pCalcData->DESIGN_MATERIAL.m_sSigmaY;	// H
		double	dMainDiaUpper_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy];	// ��ö�� ��� Dia Idx 1-Cylce
		double	dMainDiaUpper_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy];	// ��ö�� ��� Dia Idx 2-Cycle
		double	dMainCTCUpper			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1;					// ��ö�� ��� ����
		double	dMainDiaLower_1Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1];			// ��ö�� �ϸ� Dia Idx 1-Cylce
		double	dMainDiaLower_2Cy		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy];		// ��ö�� �ϸ� Dia Idx 2-Cycle
		double	dMainCTCLower			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1;						// ��ö�� �ϸ� ����
		double	dBaeRuckDiaUpper		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2];		// ���ö�� ��� Dia Idx
		double	dBaeRuckCTCUpper		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2;					// ���ö�� ��� ����
		double	dBaeRuckDiaLower		= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3];			// ���ö�� �ϸ� Dia Idx
		double	dBaeRuckCTCLower		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB;						// ���ö�� �ϸ� ����
		double	dEndReinforceDiaMain_1Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2];			// �ܺκ��� ��ö�� Dia Idx 1-Cycle
		double	dEndReinforceDiaMain_2Cy= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy];		// �ܺκ��� ��ö�� Dia Idx 2-Cycle
		double	dEndReinforceCTCMain	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// �ܺκ��� ��ö�� ����
		double	dEndReinforceDiaBeryuk	= _dRebarDia[pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4];			// �ܺκ��� ���ö�� Dia Idx
		double	dEndReinforceCTCBeryuk	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1;						// �ܺκ��� ���ö�� ����
		
		pOut->SetXL(pXL,	"�߾Ӻ� �ٴ��� (������ �����)", m_nSttRow, "A", "A", 1,	BLACK, 11);

		MakeExperienceTable();

		double	dSlabThick		= pBridgeApp->m_dThickSlabBase - pBridgeApp->m_dThickSlabHunch;					// �ٴ��� �β�
		double	dTempWidth		= 1000;
		
		SetLines(1);
		
		//��ö�� ���ö�� �ʿ�ö�ٷ�
		sText.Format("=%f", dSlabThick * dTempWidth);

		pOut->SetXL(pXL,	sText,	m_nSttRow,	"R",	"T",	0, BLACK,	9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"U",	"U",	0, BLACK,	9, FALSE, TA_CENTER);

		pOut->SetXL(pXL,	"0.3%", m_nSttRow,	"V",	"X",	0, BLACK,	9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);

		sText.Format("=R%d * V%d", m_nSttRow, m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE, TA_CENTER, "", 1);

		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);

		sText.Format("=IF(Z%d+Z%d>Z%d,\"O.K\",\"N.G\")", m_nSttRow + 1, m_nSttRow + 2, m_nSttRow);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"AF",	"AI",	0,	BLUE,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);

		//��ö�� ���ö��
		pOut->SetXL(pXL,	sRebar,				m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dMainDiaUpper_1Cy,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"@",				m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dMainCTCUpper	,	m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		double dAs = GetRebarArea((long)dMainDiaUpper_1Cy) * pow(10.0, 3) / dMainCTCUpper;

		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);		
	
		pOut->SetXL(pXL,	sRebar,				m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dMainDiaUpper_2Cy,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"@",				m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	dMainCTCUpper	,	m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		dAs = (GetRebarArea((long)dMainDiaUpper_2Cy) * pow(10.0, 3)) / dMainCTCUpper;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);

		//��ö�� �ϸ�ö�� �ʿ�ö�ٷ�
		sText.Format("=%f", dSlabThick * dTempWidth);

		pOut->SetXL(pXL,	sText,	m_nSttRow,	"R",	"T",	0, BLACK,	9, FALSE, TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"U",	"U",	0, BLACK,	9, FALSE, TA_CENTER);
		
		pOut->SetXL(pXL,	"0.4%", m_nSttRow,	"V",	"X",	0, BLACK,	9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		sText.Format("=R%d * V%d", m_nSttRow, m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE, TA_CENTER, "MAIN_REQ_AS", 1);
		
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		sText.Format("=IF(Z%d+Z%d>Z%d,\"O.K\",\"N.G\")", m_nSttRow + 1, m_nSttRow + 2, m_nSttRow);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"AF",	"AI",	0,	BLUE,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);

		//��ö�� �ϸ�ö��
		pOut->SetXL(pXL,	sRebar,				m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dMainDiaLower_1Cy,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER, "", 0);
		pOut->SetXL(pXL,	"@",				m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dMainCTCLower	,	m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		dAs = GetRebarArea((long)dMainDiaLower_1Cy) * pow(10.0, 3) / dMainCTCLower;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		SetLines(NEXT);

		pOut->SetXL(pXL,	sRebar,				m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dMainDiaLower_2Cy,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER, "", 0);
		pOut->SetXL(pXL,	"@",				m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dMainCTCLower	,	m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		dAs = GetRebarArea((long)dMainDiaLower_2Cy) * pow(10.0, 3) / dMainCTCLower;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);

		//���ö�� ���ö�� �ʿ�ö�ٷ�
		sText.Format("=%f", dSlabThick * dTempWidth);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"R",	"T",	0, BLACK,	9, FALSE, TA_CENTER,	""	,	0);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"U",	"U",	0, BLACK,	9, FALSE, TA_CENTER);
		
		pOut->SetXL(pXL,	"0.3%", m_nSttRow,	"V",	"X",	0, BLACK,	9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		sText.Format("=R%d * V%d", m_nSttRow, m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE, TA_CENTER,	"",	1);
		
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		sText.Format("=IF(Z%d>Z%d,\"O.K\",\"N.G\")", m_nSttRow + 1, m_nSttRow);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"AF",	"AI",	0,	BLUE,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);

		//���ö�� ���ö��
		pOut->SetXL(pXL,	sRebar,				m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dBaeRuckDiaUpper,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER, "", 0);
		pOut->SetXL(pXL,	"@",				m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dBaeRuckCTCUpper,	m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		dAs = GetRebarArea((long)dBaeRuckDiaUpper) * pow(10.0, 3) / dBaeRuckCTCUpper;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		SetLines(NEXT);

		//���ö�� �ϸ�ö�� �ʿ�ö�ٷ�
		sText.Format("=%f", dSlabThick * dTempWidth);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"R",	"T",	0, BLACK,	9, FALSE, TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"U",	"U",	0, BLACK,	9, FALSE, TA_CENTER);
		
		pOut->SetXL(pXL,	"0.3%", m_nSttRow,	"V",	"X",	0, BLACK,	9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		sText.Format("=R%d * V%d", m_nSttRow, m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE, TA_CENTER, "BAERUCK_REQ_AS", 1);
		
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		sText.Format("=IF(Z%d>Z%d,\"O.K\",\"N.G\")", m_nSttRow + 1, m_nSttRow);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"AF",	"AI",	0,	BLUE,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);

		//���ö�� �ϸ�ö��
		pOut->SetXL(pXL,	sRebar,				m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dBaeRuckDiaLower,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER, "", 0);
		pOut->SetXL(pXL,	"@",				m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dBaeRuckCTCLower,	m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		dAs = GetRebarArea((long)dBaeRuckDiaLower) * pow(10.0, 3) / dBaeRuckCTCLower;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		SetLines(NEXT);

		//�ܺκ���ö�� ��ö�� �ʿ�ö�ٷ�
		sText.Format("=%s", pOut->GetCellRef("MAIN_REQ_AS"));
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"R",	"T",	0, BLACK,	9, FALSE, TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"U",	"U",	0, BLACK,	9, FALSE, TA_CENTER);
		
		pOut->SetXL(pXL,	"2��", m_nSttRow,	"V",	"X",	0, BLACK,	9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		sText.Format("=R%d * 2", m_nSttRow, m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE, TA_CENTER,	"",	1);
		
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		sText.Format("=IF(Z%d+Z%d>Z%d,\"O.K\",\"N.G\")", m_nSttRow + 1, m_nSttRow + 2, m_nSttRow);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"AF",	"AI",	0,	BLUE,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);

		//�ܺκ���ö�� ��ö��
		
		pOut->SetXL(pXL,	sRebar,						m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dEndReinforceDiaMain_1Cy,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER, "" ,0);
		pOut->SetXL(pXL,	"@",						m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dEndReinforceCTCMain,		m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",						m_nSttRow,	"Y",	"Y",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		dAs = GetRebarArea((long)dEndReinforceDiaMain_1Cy) * pow(10.0, 3) / dEndReinforceCTCMain;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		SetLines(NEXT);

		pOut->SetXL(pXL,	sRebar,						m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dEndReinforceDiaMain_2Cy,	m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER, "" , 0);
		pOut->SetXL(pXL,	"@",						m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dEndReinforceCTCMain,		m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",						m_nSttRow,	"Y",	"Y",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		dAs = GetRebarArea((long)dEndReinforceDiaMain_2Cy) * pow(10.0, 3) / dEndReinforceCTCMain;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		SetLines(NEXT);

		//�ܺκ���ö�� ���ö�� �ʿ�ö�ٷ�
		sText.Format("=%s", pOut->GetCellRef("BAERUCK_REQ_AS"));
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"R",	"T",	0, BLACK,	9, FALSE, TA_CENTER,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"U",	"U",	0, BLACK,	9, FALSE, TA_CENTER);
		
		pOut->SetXL(pXL,	"2��", m_nSttRow,	"V",	"X",	0, BLACK,	9, FALSE, TA_CENTER);
		pOut->SetXL(pXL,	"=",	m_nSttRow,	"Y",	"Y",	0, BLACK,	9, FALSE, TA_CENTER);
		
		sText.Format("=R%d * 2", m_nSttRow, m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE, TA_CENTER,	"",	1);
		
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		sText.Format("=IF(Z%d>Z%d,\"O.K\",\"N.G\")", m_nSttRow + 1, m_nSttRow);
		
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"AF",	"AI",	0,	BLUE,	9,	FALSE,	TA_CENTER);

		SetLines(NEXT);
		//�ܺκ���ö�� ���ö��
		pOut->SetXL(pXL,	sRebar,						m_nSttRow,	"R",	"R",	0,	RED,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dEndReinforceDiaBeryuk,		m_nSttRow,	"S",	"T",	0,	RED,	9,	FALSE,	TA_CENTER, "", 0);
		pOut->SetXL(pXL,	"@",						m_nSttRow,	"U",	"U",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dEndReinforceCTCBeryuk,		m_nSttRow,	"V",	"W",	0,	RED,	9,	FALSE,	TA_CENTER,	"",	0);
		pOut->SetXL(pXL,	"=",						m_nSttRow,	"Y",	"Y",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		dAs = GetRebarArea((long)dEndReinforceDiaBeryuk) * pow(10.0, 3) / dEndReinforceCTCBeryuk;
		
		pOut->SetXL(pXL,	dAs,	m_nSttRow,	"Z",	"AB",	0,	BLACK,	9,	FALSE,	TA_LEFT,	"",	1);
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AC",	"AC",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		
		SetLines(NEXT);

		pOut->SetXL(pXL,	"* ���⼭ �ٴ��� ������ �ܸ��� =", m_nSttRow, "B",	"P",	0,	BLACK,	9,	FALSE, TA_RIGHT);

		pOut->SetXL(pXL,	dTempWidth,	m_nSttRow,	"R",	"S",	0,	BLACK,	9,	FALSE,	TA_RIGHT,	"",	0);
		pOut->SetXL(pXL,	"��",		m_nSttRow,	"T",	"T",	0,	BLACK,	9,	FALSE,	TA_CENTER);
		pOut->SetXL(pXL,	dSlabThick,	m_nSttRow,	"U",	"V",	0,	BLACK,	9,	FALSE,	TA_RIGHT,	"",	0);
		pOut->SetXL(pXL,	"=",		m_nSttRow,	"W",	"W",	0,	BLACK,	9,	FALSE,	TA_CENTER);

		sText.Format("=R%d * U%d", m_nSttRow, m_nSttRow);
		pOut->SetXL(pXL,	sText,	m_nSttRow,	"X",	"Z",	0,	BLACK,	9,	FALSE,	TA_CENTER, "",	0);
		
		pOut->SetXL(pXL,	"��",	m_nSttRow,	"AA",	"AA",	0,	BLACK,	9,	FALSE,	TA_CENTER);

		SetLines(2);
	}
	
	double	dAsreq	= pCalcData->m_Asreq;
	double	dAsuset	= pCalcData->m_Asuset;
	double  dJu_ratio = pCalcData->m_Ju_ratio;

	pOut->SetXL(pXL, "���� ������ ������� ö�� ����",	m_nSttRow, "A", "A", 1, BLACK, 9, FALSE, TA_LEFT);
	
	pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "B", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� �� �� �� �� ��",		m_nSttRow, "H", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� �� �� ��",			m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "ö�ٷ� ����",			m_nSttRow, "B", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "H", "X", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_LEFT);

	sText.Format("��ũ��Ʈ�� �ܸ����� %.1f%%  ��", CalcFloor.GetFloorConcAreaCoefficient());
	pOut->SetXL(pXL, sText,		m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, dAsreq,							m_nSttRow, "S", "V", 0, BLACK, 9, FALSE, TA_LEFT, "dAsreq");
	pOut->SetXL(pXL, "mm��",							 m_nSttRow, "W", "X", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "���ö�ٷ�",						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, dAsuset,							m_nSttRow, "AF", "AI", 0, BLACK, 9, FALSE, TA_LEFT, "dAsuset");
	pOut->SetXL(pXL, "mm��",							 m_nSttRow, "AJ", "AK", 0, BLACK, 9, FALSE, TA_LEFT);
	sText.Format("=IF(%s>=%s, \"N.G.\", \"O.K.\")", 
			pOut->GetCellRef("dAsreq"), pOut->GetCellRef("dAsuset"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "����� ����",			m_nSttRow, "B", "G", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "H", "X", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "Y", "AK", 1, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_LEFT);

	pOut->SetXL(pXL, "�ּ� �����",						m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_LEFT);
	sText.Format("%.4f", CalcFloor.GetFloorJujangStd());
	pOut->SetXL(pXL, sText,							m_nSttRow, "S", "U", 0, BLACK, 9, FALSE, TA_LEFT, "dJu_ratio1", 4);
	pOut->SetXL(pXL, "mm/mm��",							 m_nSttRow, "V", "X", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, "��������",						m_nSttRow, "Y", "Y", 0, BLACK, 9, FALSE, TA_LEFT);
	pOut->SetXL(pXL, dJu_ratio,							m_nSttRow, "AF", "AH", 0, BLACK, 9, FALSE, TA_LEFT, "dJu_ratio2", 4);
	pOut->SetXL(pXL, "mm/mm��",							 m_nSttRow, "AI", "AK", 0, BLACK, 9, FALSE, TA_LEFT);
	sText.Format("=IF(%s>=%s, \"N.G.\", \"O.K.\")", 
			pOut->GetCellRef("dJu_ratio1"), pOut->GetCellRef("dJu_ratio2"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "AL", "AP", 1, BLACK, 9, FALSE, TA_CENTER);

}

void CXLCheckListFloor::Reaction()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CXLControl			*pXL			= m_pXL;

	m_nSttRow = 1;
	pOut->ClearCellRef();

	CString	sText, szName;
	long	nForce		= JOINT_REACTION;
	long	nGirdersu	= pBridgeApp->GetGirdersu();
	pXL->SetCellWidth(0, 255, 0.95);//
	
	pOut->SetXL(pXL, "������� CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "2.�ιݷ��� �߻����� �� ��� SHOE�� ������ ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);
	SetLines(NEXT);
	
	for(long nG = 0; nG < nGirdersu; nG++)
	{
		CPlateGirderApp *pGir = pBridgeApp->GetGirder(nG);

		sText.Format("<< �Ŵ� - %d >>", nG+1);
		pOut->SetXL(pXL, sText,								m_nSttRow, "B", "B", 1, BLUE);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "B", "C", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "����",							m_nSttRow, "B", "C", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "����",							m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		
		if(!pDataManage->GetBridge()->IsTUGir())
		{
			pOut->SetLineBoxText(pXL, "�ռ��� ��������",		m_nSttRow, "F", "M", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "N", "Q", 2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "�ռ���",							m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "Ȱ����",					m_nSttRow, "R", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����ħ��",				m_nSttRow, "Z", "AG", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "AH", "AO", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AP", "AQ", 2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "�� ��",							m_nSttRow, "AP", "AQ", 0, BLACK, 9, FALSE, TA_CENTER);
		
		}
		else
		{
			pOut->SetLineBoxText(pXL, "�ռ��� ��������",		m_nSttRow, "F", "V", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "W", "Z", 2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "�ռ���",							m_nSttRow, "W", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "Ȱ����",					m_nSttRow, "AA", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "����ħ��",				m_nSttRow, "AI", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "AQ", "AX", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "",						m_nSttRow, "AY", "AZ", 2, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "�� ��",							m_nSttRow, "AY", "AZ", 0, BLACK, 9, FALSE, TA_CENTER);
		
		}

		SetLines(NEXT);
		pOut->SetXL(pXL, "��ġ",							m_nSttRow, "B", "C", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "��ȣ",							m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "STEEL",					m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER);
		
		if(!pDataManage->GetBridge()->IsTUGir())
		{
			pOut->SetLineBoxText(pXL, "CONCRETE",				m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "��������",						m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MAX",					m_nSttRow, "R", "U", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MIN",					m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MAX",					m_nSttRow, "Z", "AC", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MIN",					m_nSttRow, "AD", "AG", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MAX",					m_nSttRow, "AH", "AK", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MIN",					m_nSttRow, "AL", "AO", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "SHOE",							m_nSttRow, "AP", "AQ", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		else
		{
			pOut->SetLineBoxText(pXL,	"����������",		m_nSttRow,	"J",	"M",	1,	BLACK,	9,	FALSE,	TA_CENTER);
			pOut->SetLineBoxText(pXL,	"180 �ʹ�",			m_nSttRow,	"N",	"Q",	1,	BLACK,	9,	FALSE,	TA_CENTER);
			pOut->SetLineBoxText(pXL,	"���κ��ٴ���",		m_nSttRow,	"R",	"V",	1,	BLACK,	9,	FALSE,	TA_CENTER);
			pOut->SetXL(pXL, "��������",						m_nSttRow, "W", "Z", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MAX",					m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MIN",					m_nSttRow, "AE", "AH", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MAX",					m_nSttRow, "AI", "AL", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MIN",					m_nSttRow, "AM", "AP", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MAX",					m_nSttRow, "AQ", "AT", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetLineBoxText(pXL, "MIN",					m_nSttRow, "AU", "AX", 1, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "SHOE",							m_nSttRow, "AY", "AZ", 0, BLACK, 9, FALSE, TA_CENTER);

		}
		SetLines(NEXT);

		long nJijum = 0;
		for (long nJ = 0; nJ < pFEMManage->GetJointSize(); nJ++)
		{
			CJoint* pJ = pFEMManage->GetJoint((unsigned short)nJ);
			
			if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && pJ->m_LongAttrib[JOINT_GIR] == nG)
			{
				szName = pBridgeApp->m_strJijumName[nJijum];
				CShoe* pShoe =  pGir->GetShoeByJijumNo(nJijum);

				pOut->SetLineBoxText(pXL, szName,				m_nSttRow, "B", "C", 1, BLACK, 9, FALSE, TA_CENTER);
				sText.Format("%d", pJ->m_nIdx+1);
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "D", "E", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);

				sText.Format("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPreDeadSteel[nForce].z);
				pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "F", "I", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
				
				if(!pDataManage->GetBridge()->IsTUGir())
				{
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPreDeadSlab[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%8.3lf", pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPostDead[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "N", "Q", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%8.3lf", pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vLiveMax[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "R", "U", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%8.3lf", pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vLiveMin[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "V", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%8.3lf", pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vUnSettleMax[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "Z", "AC", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%8.3lf", pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vUnSettleMin[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AD", "AG", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMax[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AH", "AK", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%8.3lf",	pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMin[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AL", "AO", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					sText.Format("%3d", pShoe->m_dForceShoe*10);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AP", "AQ", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
				}
				else
				{
					//����������
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPreDeadConf[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "J", "M", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					//180�ʹ�
					sText.Format("%8.3lf",pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPreDeadTurning[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "N", "Q", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					//���κ��ٴ���
					sText.Format("%8.3lf",pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPreDeadSlab[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "R", "V", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
				
					//��������
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vPostDead[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "W", "Z", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					
					//Ȱ����-MAX
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vLiveMax[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AA", "AD", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					
					//Ȱ����-MIN
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vLiveMin[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AE", "AH", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					
					//����ħ��-MAX
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vUnSettleMax[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AI", "AL", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					
					//����ħ��-MIN
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vUnSettleMin[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AM", "AP", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					
					//�հ�-MAX
					sText.Format("%8.3lf", pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMax[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AQ", "AT", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);
					
					//�հ�-MIN
					sText.Format("%8.3lf",	pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS)->m_JointResultArr.GetAt(pJ->m_nIdx)->m_vTotalMin[nForce].z);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AU", "AX", 1, BLACK, 9, FALSE, TA_CENTER, "", 3);

					//SHOE
					sText.Format("%3d", pShoe->m_dForceShoe*10);
					pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AY", "AZ", 1, BLACK, 9, FALSE, TA_CENTER, "", 0);
				}
				SetLines(NEXT);
				nJijum++;
			}
		}
		SetLines(NEXT);
	}
}

void CXLCheckListFloor::Torsion()
{	
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc      StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CXLControl			*pXL			= m_pXL;
	m_nSttRow = 1;
	pOut->ClearCellRef();
	CString	sText;

	///< ��ü �ܸ� �� ������Ʋ�� ����� ���� ���� �ܸ� ã��
	long	nG				= 0;
	double	dSta			= 0;
	double	dEs				= pCalcData->DESIGN_MATERIAL.m_dEs;///< ź�����
	double	dShearEs		= pCalcData->DESIGN_MATERIAL.m_dShearEs;

	pXL->SetCellWidth(0, 255, 1);//

	pOut->SetXL(pXL, "������� CHECK LIST",	m_nSttRow, "A", "A", 1, 1, 12,TRUE);
	pOut->SetXL(pXL, "3.����Ʋ���� ���� ���� ����",	m_nSttRow, "A", "A",1, 1, 9,TRUE);//
	SetLines(NEXT);
	
	pOut->SetXL(pXL, "* ��Ʋ�� �����",	m_nSttRow, "B", "B",1);//* ��Ʋ�� �����
	
	
	pOut->SetXL(pXL, "L  : ������(mm)",						m_nSttRow, "R", "R", 0);
	pOut->SetXL(pXL, "Es : ź�����(MPa)",						m_nSttRow, "AB", "AB", 1);

	pOut->SetXL(pXL, "Iw : ����Ʋ�����(mm6)",					m_nSttRow, "R", "R", 0);
	pOut->SetXL(pXL, "G  : ����ź�����(MPa)",					m_nSttRow, "AB", "AB", 1);
	pOut->SetXL(pXL, "K  : ������Ʋ�����(mm4)",					m_nSttRow, "R", "R", 1);

	SetLines(-2);
	pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "B", "C", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",							m_nSttRow, "D", "D", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "L",							m_nSttRow, "E", "E", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "G", "G", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "(",							m_nSttRow, "H", "H", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "G��K",						m_nSttRow, "I", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "E��Iw",								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-1);
	pOut->SetLineBoxText(pXL, ")",							m_nSttRow, "K", "K", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(2);

	SetLines(NEXT);
	pOut->SetXL(pXL, "�� ������ ��Ʋ�� ����� �᣼ 0.4�� ��� ������Ʋ���� ���� ���°����,", m_nSttRow, "B", "B", 1);
	pOut->SetXL(pXL, "   �᣾10�� ��� ����Ʋ���� ���� ���°���� ���� ������ �� �ִ�.",		m_nSttRow, "B", "B", 1);

	SetLines(NEXT);

	pOut->SetLineBoxText(pXL, "�ܸ�TYPE",					m_nSttRow, "A", "D", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "L(mm)",						m_nSttRow, "E", "H", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Es(MPa)",					m_nSttRow, "I", "M", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Iw(mm^6)",					m_nSttRow, "N", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "G(MPa)",						m_nSttRow, "S", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "K(mm��)",					m_nSttRow, "W", "AA", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "��",							 m_nSttRow, "AB", "AD", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� ��",						m_nSttRow, "AE", "AP", 1, BLACK, 9, FALSE, TA_CENTER);

	SetLines(NEXT);

	long nIdx = 1;

	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		nG		= -1;
		dSta	= 0;
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		long	nMat	= atoi(pFrameSec->m_szName);
		long	nNodeIdx = ModelCalc.GetMaxForcePositionOfSection(nMat, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
		CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
		CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);

		ModelCalc.GetSectionCoefficient(nG, dSta);

		double	dLengthJigan= (pBridgeApp->m_dLengthJigan[pGir->GetNumberJiganByStation(dSta)]);
		double	dWarpingIw	= ModelCalc.GetMapValueSecCo("WarpingIw");
		double	dK			= ModelCalc.GetMapValueSecCo("K");

		sText.Format("�ܸ�-%d", nIdx++);
		pOut->SetLineBoxText(pXL, sText,			m_nSttRow, "A", "D", 1, BLACK,   9, FALSE, TA_CENTER,"", 0);
		pOut->SetLineBoxText(pXL, dLengthJigan,		m_nSttRow, "E", "H", 1, BLACK,   9, FALSE, TA_CENTER,"",1);	
		pOut->SetLineBoxText(pXL, dEs,				m_nSttRow, "I", "M", 1, BLACK,   9, FALSE, TA_CENTER,"",0);		
		
		pOut->SetLineBoxText(pXL, dWarpingIw,		m_nSttRow, "N", "R", 1, BLACK,   9, FALSE, TA_CENTER);
		sText.Format("N%d", m_nSttRow);
		pXL->SetNumberFormat(sText, "0.00000E+00");

		pOut->SetLineBoxText(pXL, dShearEs,			m_nSttRow, "S", "V", 1, BLACK,   9, FALSE, TA_CENTER,"",0);
		pOut->SetLineBoxText(pXL, dK,				m_nSttRow, "W", "AA", 1, BLACK,   9, FALSE, TA_CENTER);

		sText.Format("=E%d*SQRT(S%d*W%d/(I%d*N%d))", m_nSttRow, m_nSttRow, m_nSttRow, m_nSttRow, m_nSttRow);
		pOut->SetLineBoxText(pXL, sText,				m_nSttRow, "AB", "AD", 1, BLACK,   9, FALSE, TA_CENTER);				

		sText.Format("=IF(AB%d>10,\"������Ʋ���� ���ؼ��� ����\",\"������ ����Ʋ���� ���� ������\")", m_nSttRow);
		pOut->SetLineBoxText(pXL, sText,		m_nSttRow, "AE", "AP", 1, BLACK,   9, FALSE, TA_CENTER);

		SetLines(NEXT);
	}
}

void CXLCheckListFloor::SetLines(int nLine)
{
	m_nSttRow += nLine;
	if(nLine == 0) m_nSttRow = 1;
}


double CXLCheckListFloor::GetDeflection(long nPos, CString szLoad)
{
	CAPlateCalcStd		*pCalc		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM		= pCalc->GetCantileverFemData(0);
	double dDisp = 0;
	
	// 0-left, 1-middle, 2-right
	long nLoadCase = pFEM->GetLoadCaseIndex(szLoad);
	if (nLoadCase<0) return dDisp;
	
	// seek member index
	CLongArray IdxElem;
	GetElementIndex_Cantilever(pFEM, nPos, IdxElem);
	
	// seek deflection
	CResultForce* pResult1 = NULL;
	CResultForce* pResult2 = NULL;
	
	for (long n=0; n<IdxElem.GetSize(); n++)
	{		
		long nElem = IdxElem.GetAt(n);
		CElement* pElem = pFEM->GetElement((unsigned short)nElem);
		CJoint*	  pJ1	= pFEM->GetJoint(pElem->m_nJStt);
		CJoint*	  pJ2	= pFEM->GetJoint(pElem->m_nJEnd);
		
		pResult1 = pJ1->GetResultForce(nLoadCase);
		pResult2 = pJ2->GetResultForce(nLoadCase);
		if (!pResult1) continue;
		if (!pResult2) continue;
		
		double dDis1 = pResult1->m_Disp.z;
		double dDis2 = pResult2->m_Disp.z;
		
		if (fabs(dDisp)<fabs(dDis1)) dDisp = dDis1;
		if (fabs(dDisp)<fabs(dDis2)) dDisp = dDis2;
	}
	
	return dDisp*1000;
}

void CXLCheckListFloor::GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr)
{
	//-------------------------------------------------------------------------------------------
	// Seek Flange X-Position
	// 0: Top Flange Left, 1: Top Flange Center, 2: Top Flange Right
	
	CDoubleArray FlangePos;
	BOOL	bFlangeLevel = FALSE;
	double  dFlangeLevel = 0;
	double	dSlabLevel	 = 0;
	
	for (long nJ=0; nJ<pFEM->GetJointSize(); nJ++)
	{
		CJoint* pJoint		= pFEM->GetJoint((unsigned short)nJ);
		double  dJoint_z	= pJoint->m_vPoint.z;
		double  dJoint_x	= pJoint->m_vPoint.x;
		
		if (!bFlangeLevel && dJoint_z !=dSlabLevel)
		{
			bFlangeLevel = TRUE;
			dFlangeLevel = dJoint_z;
		}
		
		if (bFlangeLevel && dFlangeLevel==dJoint_z)
			FlangePos.Add(dJoint_x);
	}
	FlangePos.Sort();
	
	//-------------------------------------------------------------------------------------------
	// Seek Element Index
	if (nSide==0)	// Left Cantilever
	{
		// Target Position
		double dTarget = FlangePos.GetAt(0);
		
		// Seek Element
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			
			if (pJ_S->m_vPoint.x>dTarget) break;
			nArr.Add(nE);
		}
	}
	else if (nSide==2)	// Right Cantilever
	{
		// Target Position
		double dTarget = FlangePos.GetAt(FlangePos.GetUpperBound());
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			CJoint*   pJ_E  = pFEM->GetJoint(pElem->m_nJEnd);
			
			if (pJ_S->m_vPoint.z<dSlabLevel) break;
			if (pJ_E->m_vPoint.x<dTarget) continue;
			nArr.Add(nE);
		}
	}
	else	// Span Middle Member
	{
		// Target Position
		CDoubleArray dTarget;
		for (long nP=2; nP<FlangePos.GetSize()-1; nP+=3)
		{
			double dPos = (FlangePos.GetAt(nP) + FlangePos.GetAt(nP+1)) / 2;
			dTarget.Add(dPos);
		}
		
		for (long nE=0; nE<pFEM->GetElementSize(); nE++)
		{
			CElement* pElem = pFEM->GetElement((unsigned short)nE);
			CJoint*   pJ_S  = pFEM->GetJoint(pElem->m_nJStt);
			CJoint*   pJ_E  = pFEM->GetJoint(pElem->m_nJEnd);
			
			if (pJ_S->m_vPoint.z<dSlabLevel) break;
			
			for (long n=0; n<dTarget.GetSize(); n++)
			{
				double dPos = dTarget.GetAt(n);
				if (fabs(pJ_S->m_vPoint.x-dPos)<EPSILON || fabs(pJ_E->m_vPoint.x-dPos)<EPSILON)
					nArr.Add(nE);
			}
		}		
	}
}

void CXLCheckListFloor::SeekFlange_Web(double& dFlange, double& dWeb)
{
	dFlange = dWeb = 0;
	
	__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;
	CPlateBridgeApp		*pDB = m_pAPlateOutCalcStd->GetAPlateBridgeApp();
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

double CXLCheckListFloor::GetAllowDeflection(long nPos)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	double	dAllow_Def = 0;

	// Seek Girder to Girder Space
	CDoubleArray dSpaceArr;
	long	nGirSu = pDB->GetGirdersu();
	double	dL_Cantilever = pDB->GetSlabLeft()->m_dGirderCenter;
	double	dR_Cantilever = pDB->GetSlabRight()->m_dGirderCenter;
	double	dLen = pDB->GetGirder(0)->m_dGirderCenter - dL_Cantilever;
	dSpaceArr.Add(dLen);

	for (long nG=0; nG<nGirSu-1; nG++)
	{
		double dGirderCenterPrev = pDB->GetGirder(nG)->m_dGirderCenter;
		double dGirderCenterNext = pDB->GetGirder(nG+1)->m_dGirderCenter;
		dLen = dGirderCenterNext - dGirderCenterPrev;
		dSpaceArr.Add(dLen);
	}

	dLen = dR_Cantilever - pDB->GetGirder(pDB->GetGirdersu()-1)->m_dGirderCenter;
	dSpaceArr.Add(dLen);

	// Seek Length for Calculation
	CCentSeparation* pGuardL = pDB->GetGuardFencLeft();
	CCentSeparation* pGuardR = pDB->GetGuardFencRight();

	double dCalc_Len = 0;
	double dCar		 = 300;
	double dFlange	 = 0;
	double dWeb		 = 0;
	SeekFlange_Web(dFlange, dWeb);

	switch (nPos)
	{
	case 0:	// Left
		{
			double dGurdWidth	= (pGuardL)? pGuardL->GetWidth()+pGuardL->m_D1 : 0;
			double dSpace		= dSpaceArr.GetAt(0);
			dCalc_Len	= (dSpace-dGurdWidth) - dCar  - dWeb/2 - (dFlange/4 - dWeb/4);
			dAllow_Def  = dCalc_Len / 300;
		}
		break;
	case 1: // Right
		{
			double dGurdWidth	= (pGuardR)? pGuardR->GetWidth()+pGuardR->m_D1 : 0;
			double dSpace = dSpaceArr.GetAt(dSpaceArr.GetSize()-1);
			dCalc_Len	= (dSpace-dGurdWidth) - dCar  - dWeb/2 + (dFlange/4 - dWeb/4);
			dAllow_Def  = dCalc_Len / 300;
		}
		break;
	case 2: // Center
		{
			for (long nG=1; nG<dSpaceArr.GetSize()-1; nG++)
			{
				double dDis = dSpaceArr.GetAt(nG);
				dCalc_Len = (dCalc_Len<dDis)? dDis : dCalc_Len;
			}

			if (nGirSu!=2)
			{
				dCalc_Len -= dWeb - (dFlange-dWeb)/2;
			}

			dAllow_Def  = dCalc_Len / 800;
		}
		break;
	}
	
	return dAllow_Def;
}

void CXLCheckListFloor::MakeExperienceTable()
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();
	CXLControl			*pXL			= m_pXL;

	long nSttRow_Temp = m_nSttRow;
	
	//nSttRow_Temp++;

	pOut->SetLineBoxText(pXL, "��    ġ",	nSttRow_Temp,	"B",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�� �� ��",	nSttRow_Temp,	"P",	"AE",	1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "��    ��",	nSttRow_Temp,	"AF",	"AI",	1, BLACK, 9, FALSE, TA_CENTER);
	
	
	//������ Value�κ� Line �׸���
	
	long nSttRow_Temp1 = nSttRow_Temp + 1;

	for(long nLine = 0; nLine < 15; nLine++)
	{
		pOut->SetLineBoxText(pXL, "",	 nSttRow_Temp1, "P", "AE",	1, BLACK, 9, FALSE, TA_CENTER);

		nSttRow_Temp1++;
	}

	nSttRow_Temp++;
	
	//��ö��
	pOut->SetLineBoxText(pXL, "�� ö ��",	nSttRow_Temp,	"B",	"F",	6, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetLineBoxText(pXL, "���ö��",	nSttRow_Temp,	"G",	"J",	3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�ö�ٷ�",	nSttRow_Temp,	"K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",	nSttRow_Temp,	 "AF",	"AI",	3,	BLACK, 9, FALSE, TA_CENTER); //����
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL, "���ö�ٷ�",	nSttRow_Temp, "K",	"O",	2, BLACK, 9, FALSE, TA_CENTER);
	
	nSttRow_Temp += 2;
	
	pOut->SetLineBoxText(pXL, "�ϸ�ö��",	nSttRow_Temp,	"G",	"J",	3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�ö�ٷ�",	nSttRow_Temp,	"K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",	nSttRow_Temp,	 "AF",	"AI",	3,	BLACK, 9, FALSE, TA_CENTER);	//����
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL, "���ö�ٷ�",	nSttRow_Temp, "K",	"O",	2, BLACK, 9, FALSE, TA_CENTER);
	
	nSttRow_Temp += 2;
	
	//���ö��
	pOut->SetLineBoxText(pXL, "���ö��",	nSttRow_Temp,	"B",	"F",	4, BLACK, 9, FALSE, TA_CENTER);
	
	pOut->SetLineBoxText(pXL, "���ö��",	nSttRow_Temp,	"G",	"J",	2,	BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�ö�ٷ�",	nSttRow_Temp,	"K",	"O",	1,	BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			nSttRow_Temp,	"AF",	"AI",	2,	BLACK, 9, FALSE, TA_CENTER); //����
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL, "���ö�ٷ�",	nSttRow_Temp, "K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL, "�ϸ�ö��",	nSttRow_Temp,	"G",	"J",	2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�ö�ٷ�",	nSttRow_Temp,	"K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",	nSttRow_Temp,	 "AF",	"AI",	2,	BLACK, 9, FALSE, TA_CENTER);	//����
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL, "���ö�ٷ�",	nSttRow_Temp, "K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	
	
	nSttRow_Temp++;

	//�ܺκ���ö��
	pOut->SetLineBoxText(pXL, "�ܺκ���ö��",	nSttRow_Temp,	"B",	"F",	5, BLACK, 9, FALSE, TA_CENTER);

	pOut->SetLineBoxText(pXL, "��ö��",		nSttRow_Temp,	"G",	"J",	3, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�ö�ٷ�",	nSttRow_Temp,	"K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			nSttRow_Temp,	 "AF",	"AI",	3,	BLACK, 9, FALSE, TA_CENTER);	//����
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL, "���ö�ٷ�",	nSttRow_Temp, "K",	"O",	2, BLACK, 9, FALSE, TA_CENTER);
	
	nSttRow_Temp += 2;
	
	pOut->SetLineBoxText(pXL, "���ö��",	nSttRow_Temp,	"G",	"J",	2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�ʿ�ö�ٷ�",	nSttRow_Temp,	"K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, " ",			nSttRow_Temp,	"AF",	"AI",	2,	BLACK, 9, FALSE, TA_CENTER);	//����
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL, "���ö�ٷ�",	nSttRow_Temp, "K",	"O",	1, BLACK, 9, FALSE, TA_CENTER);
	
	nSttRow_Temp++;
	
	pOut->SetLineBoxText(pXL,	"", nSttRow_Temp, "B",	"AI",	1, BLACK, 9, FALSE, TA_CENTER);

}

double CXLCheckListFloor::GetRebarArea(long nRebarDia)
{
	switch(nRebarDia)
	{
	case 10:
		return 71.33;
	case 13:
		return 126.7;
	case 16:
		return 198.6;
	case 19:
		return 286.5;
	case 22:
		return 387.1;
	case 25:
		return 506.7;
	case 29:
		return 642.4;
	case 32:
		return 794.2;
	case 35:
		return 956.6;
	}
		
	return -1;
}
///////////////////////////////////////////////////////////////////////////////////
//��š񢱢����� ���¥åĥťƥǥ�������ҥ��ɥ��������f ����������
//	���£��������¡áġҡ����� �� �� �� kgf/�� t��m  ����������������
//  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ������������¨èĨ�
//  ��������������������������  �� ��
//  �� �� �� ������ �� ���� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��
///////////////////////////////////////////////////////////////////////////////////// 