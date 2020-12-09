// XLWeldingCheck.cpp: implementation of the CXLWeldingCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcStd.h"
#include "XLWeldingCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

CXLWeldingCheck::CXLWeldingCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_nG				= 0;
	m_nSectionSize		= 0;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
	m_pVStiffBx			= NULL;
}

CXLWeldingCheck::~CXLWeldingCheck()
{

}

void CXLWeldingCheck::WeldingCheck()
{
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CString strText = _T("");

	m_nSttRow = 1;
	m_nSectionSize=0;
	pOut->ClearCellRef();
	pOut->SetXL(pXL, "5.2 �������� ����",					m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "���ֿ������ ������ �����ϴ� �ʷ������� ġ���� ������ ���ѳ��� �ִ� ���� ǥ������ �Ѵ�.",m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "  �ٸ� �ּ� ������ �β��� 8mm�̻��� ���� �ʷ����� ġ���� 6mm�̻����� �Ѵ�.",m_nSttRow, "B", "B", 0);
	
	if(pOpt->GetSteelStandardYear()==APPLYYEAR2010)
		strText = _T("(�� ��.��.�� 2010 3.5.2.4)");
	else
		strText = _T("(�� ��.��.�� 2008 3.5.2.4)");

	pOut->SetXL(pXL, strText, m_nSttRow, "U", "U", 1, BLACK, 9, FALSE, TA_LEFT, "BOOK_REF_3_5_2_4");
	
	long nSttRowTemp = m_nSttRow;
	pOut->SetXL(pXL, "t��:  ������ ������ �β� (mm)",		nSttRowTemp, "L", "L");
	pOut->SetXL(pXL, "t��:  �β����� ������ �β� (mm)",	nSttRowTemp, "L", "L");
	pOut->SetXL(pXL, "S  :  �ʷ����� ġ�� (mm)",			nSttRowTemp, "L", "L");

	m_nSttRow++;

	if(pOpt->GetSteelStandardYear()==APPLYYEAR2010)
	{	
		strText = _T("t��> S �� 6mm(t���� 20mm)");
		pOut->SetXL(pXL, strText, m_nSttRow, "C", "C");
		strText = _T("t��> S �� 8mm(t����20mm)");
		pOut->SetXL(pXL, strText, m_nSttRow, "C", "C", 2);
	}
	else
	{	
		strText = _T("t���� S  �� ��2��t��");
		pOut->SetXL(pXL, strText, m_nSttRow, "B", "B", 2);
	}

	pOut->SetXL(pXL, "��) �÷����� �������� ������ ����",	m_nSttRow, "B", "B");
	FilletWeldMeasureCheck();

	WeldStressCheck();
	pOut->ClearCellRef();
	ReinforceWeldCheck(FALSE);	// �����

	pOut->ClearCellRef();
	ReinforceWeldCheck(TRUE);	// ������

}

///< �ʷ����� ġ�� ����
void CXLWeldingCheck::FilletWeldMeasureCheck()
{

	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEM			= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText, sCellRef_Tu, sCellRef_Tl, sCellRef_Tw, sCellRef_Smin, sCellRef_Smax, sCellRef_Suse;
	long	nIndex	= 0;
	long	nG		= 0;
	double	dSta	= 0;
	pOut->SetXL(pXL, "1) �ʷ����� ġ�� ����",				m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "tu : ��� �÷��� �β�",				m_nSttRow, "C", "C", 0);
	pOut->SetXL(pXL, "tl : �Ϻ� �÷��� �β�",				m_nSttRow, "J", "J", 0);
	pOut->SetXL(pXL, "tw : ������ �β� ",					m_nSttRow, "Q", "Q", 0);
	pOut->SetXL(pXL, "(���� : mm)",							m_nSttRow, "X", "X");

	pOut->SetLineBoxText(pXL, "�� ��",						m_nSttRow, "B", "D", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "������ ���β�",			m_nSttRow, "E", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "��� �÷����� ������",		m_nSttRow, "K", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "�Ϻ� �÷����� ������",		m_nSttRow, "S", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);
	pOut->SetLineBoxText(pXL, "tu",							m_nSttRow, "E", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "tl",							m_nSttRow, "G", "H", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "tw",							m_nSttRow, "I", "J", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smax",						m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smin",						m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Suse",						m_nSttRow, "O", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",						m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smax",						m_nSttRow, "S", "T", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Smin",						m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "Suse",						m_nSttRow, "W", "X", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "����",						m_nSttRow, "Y", "Z", 1, BLACK, 9, FALSE, TA_CENTER);
	SetLines(NEXT);

	long	nPosSec	= -1;
	long	nSec	= 0;
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

		nSec	= atoi(pFrameSec->m_szName);
		nG		= -1;
		dSta	= 0;
		ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_SHEAR_2, CFemModelingCalc::MAX_ABS, FALSE);

		if (nG<0) nG=0;
		double	dStaSec	= ModelCalc.GetStationSecJewon(nSec, nG, dSta, nPosSec);

		CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
		if(!pGir)	continue;

		CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dStaSec);
		if(!pBx)	continue;

		double	dTu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
		double	dTl	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
		double	dTw	= pBx->GetFactChain(G_W)->m_dFactChainThick;
		double	dSu	= pDataManage->GetFilletWeldStandard(dTu, dTw); 
		double	dSl	= pDataManage->GetFilletWeldStandard(dTl, dTw);
		
		sText.Format("�ܸ� - %d", nIndex+1);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "B", "D", 1, RED,   9, FALSE, TA_CENTER);
		///< tu
		sCellRef_Tu.Format("TU_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTu,						m_nSttRow, "E", "F", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tu, 0);
		///< tl
		sCellRef_Tl.Format("TL_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTl,						m_nSttRow, "G", "H", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tl, 0);
		///< tw
		sCellRef_Tw.Format("TW_%d", nIndex);
		pOut->SetLineBoxText(pXL, dTw,						m_nSttRow, "I", "J", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Tw, 0);
		///< Smax - ���
		sText.Format("=MIN(%s,%s)", pOut->GetCellRef(sCellRef_Tu), pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smax.Format("UPPER_S_MAX_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "K", "L", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smax, 1);
		///< Smin - ���
		sText.Format("=IF(%s=\"(�� ��.��.�� 2010 3.5.2.4)\",IF(MAX(E%d,I%d)<=20,6,8),IF(%s<8,SQRT(2*MAX(%s,%s)),MAX(6,SQRT(2*MAX(%s,%s)))))", pOut->GetCellRef("BOOK_REF_3_5_2_4"), m_nSttRow, m_nSttRow,
					pOut->GetCellRef(sCellRef_Smax),pOut->GetCellRef(sCellRef_Tu),pOut->GetCellRef(sCellRef_Tw),pOut->GetCellRef(sCellRef_Tu),pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smin.Format("UPPER_S_MIN_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smin, 1);
		///< Suse - ���
		sCellRef_Suse.Format("UPPER_SUSE_UP_%d", nIndex);
		pOut->SetLineBoxText(pXL, dSu,						m_nSttRow, "O", "P", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Suse, 0);
		///< ����
		sText.Format("=IF(AND(%s<=%s,%s>=%s),\"OK\",\"NG\")", pOut->GetCellRef(sCellRef_Smin), pOut->GetCellRef(sCellRef_Suse),
					pOut->GetCellRef(sCellRef_Smax), pOut->GetCellRef(sCellRef_Suse));
		pOut->SetLineBoxText(pXL, sText,						m_nSttRow, "Q", "R", 1, BLACK, 9, FALSE, TA_CENTER);
		///< Smax - �Ϻ�
		sText.Format("=MIN(%s,%s)", pOut->GetCellRef(sCellRef_Tl), pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smax.Format("LOWER_S_MAX_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "S", "T", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smax, 1);
		///< Smin - �Ϻ�
		sText.Format("=IF(%s=\"(�� ��.��.�� 2010 3.5.2.4)\",IF(MAX(G%d,I%d)<=20,6,8),IF(%s<8,SQRT(2*MAX(%s,%s)),MAX(6,SQRT(2*MAX(%s,%s)))))", pOut->GetCellRef("BOOK_REF_3_5_2_4"), m_nSttRow, m_nSttRow,
					pOut->GetCellRef(sCellRef_Smax),pOut->GetCellRef(sCellRef_Tl),pOut->GetCellRef(sCellRef_Tw),pOut->GetCellRef(sCellRef_Tl),pOut->GetCellRef(sCellRef_Tw));
		sCellRef_Smin.Format("LOWER_S_MIN_%d", nIndex);
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "V", 1, BLACK, 9, FALSE, TA_CENTER, sCellRef_Smin, 1);
		///< Suse - �Ϻ�
		sCellRef_Suse.Format("LOWER_SUSE_UP_%d", nIndex);
		pOut->SetLineBoxText(pXL, dSl,						m_nSttRow, "W", "X", 1, RED,   9, FALSE, TA_CENTER, sCellRef_Suse, 0);
		///< ����
		sText.Format("=IF(AND(%s<=%s,%s>=%s),\"OK\",\"NG\")", pOut->GetCellRef(sCellRef_Smin), pOut->GetCellRef(sCellRef_Suse),
					pOut->GetCellRef(sCellRef_Smax), pOut->GetCellRef(sCellRef_Suse));
		pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "Y", "Z", 1, BLACK, 9, FALSE, TA_CENTER);

		SetLines(NEXT);
		nIndex++;
	}

	SetLines(NEXT);
}
///< �������� ���� ����
void CXLWeldingCheck::WeldStressCheck()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CGlobarOption		*pOpt			= pDataManage->GetGlobalOption();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CSteel				*pSteel			= pDataManage->GetSteel();
	CFEMManage          *pFEM		   = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CSteel				*pSteeCtl		= pDataManage->GetSteel();
	CString	sText;
	pOut->SetXL(pXL, "2) �������� ���� ����",				m_nSttRow, "B", "B", 0);
	pOut->SetXL(pXL, "�����ο� �ۿ��ϴ� ��������",			m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, "[���������α� ǥ�ؽù漭] 26Page",	m_nSttRow, "S", "S", 1);

	///< �̹�������
	pOut->InsertImage(pXL, "���������°���1", m_nSttRow, "B");
	pOut->SetXL(pXL, "tu",		m_nSttRow, "F", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "s",		m_nSttRow, "B", "B", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Yvsu",	m_nSttRow, "H", "I", 2, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Yvsl",	m_nSttRow, "H", "I", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "tl",		m_nSttRow, "B", "B", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "B",		m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);

	SetLines(2);
	pOut->InsertImage(pXL, "���������°���", m_nSttRow, "B");
	SetLines(NEXT);
	pOut->SetXL(pXL, "s",									m_nSttRow, "E", "F", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "s",									m_nSttRow, "B", "B", 0, BLACK, 9, FALSE, TA_CENTER);
	SetLines(-9);

	pOut->SetLineBoxText(pXL, "��",							m_nSttRow, "L", "L", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",							m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "S �� Q",					m_nSttRow, "N", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "<",							m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "��a ",						m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	SetLines(NEXT);
	pOut->SetXL(pXL, "Iv����a",								m_nSttRow, "N", "P", 1, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "��",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "���� ���� (MPa)",				m_nSttRow, "N", "N", 1);
	pOut->SetXL(pXL, "��a",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "��� ���� ����",						m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "(�� ��.��.�� 3.3.2.1) ",			m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "S",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "�� �ۿ����ܷ� (N)",					m_nSttRow, "N", "N", 1);
	pOut->SetXL(pXL, "Q",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "������ ���� �÷����� �Ѵܸ��� �߸��࿡ ����",m_nSttRow, "N", "N");
	pOut->SetXL(pXL, "�ܸ� 1�� ���Ʈ (mm��)",				m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "A(mm��)��Y(mm)",						m_nSttRow, "U", "U");
	pOut->SetXL(pXL, "y",									m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "Yvsu(l) - tu(l)/2",					m_nSttRow, "P", "P");
	pOut->SetXL(pXL, "Iv",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "�Ѵܸ��� �߸��࿡ ���� �ܸ� 2�� ���Ʈ(mm��)",m_nSttRow, "N", "N");
	pOut->SetXL(pXL, "��a",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "�ʷ������� ��β��� �� (cm)",			m_nSttRow, "N", "N", 0);
	pOut->SetXL(pXL, "=",									m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "2EA �� s /  ��2",						m_nSttRow, "V", "V");
	pOut->SetXL(pXL, "s",									m_nSttRow, "L", "L", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, ":",									m_nSttRow, "M", "M", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "�ʷ� ���� ġ�� (mm)",					m_nSttRow, "N", "N", 4);

	pOut->SetXL(pXL, "���� ���� ���",						m_nSttRow, "B", "B");
	CString	sFlange, sSmax, sTu, sB, sA, sYvsu, sQ, sIv, sS, sSigmaA, sNu, sANu, sJuType;
	long	nIndex			= 0;
	long	nMaterial		= 0;
//	long	nSteelIdx		= 0;
	double	dSmax			= 0;
	double	dTu				= 0;
	double	dWf				= 0;
	double	dYvsu			= 0;
	double	dIv				= 0;
	double	dS				= 0;
	double	dTw				= 0;
	double	dANu			= 0;
	
	for(long n=0; n < 2; n++)
	{
		if(n==0)
		{
			pOut->SetXL(pXL, "�� ��� �÷����� ������",				m_nSttRow, "B", "B");
			sFlange = "UPPER";
		}
		else
		{
			sFlange = "LOWER";
			pOut->SetXL(pXL, "�� �Ϻ� �÷����� ������",				m_nSttRow, "B", "B");
		}
		nIndex = 0;
		pOut->SetLineBoxText(pXL, "�� ��",					m_nSttRow, "A", "C", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "D", "E", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "F", "F", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "G", "H", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "I", "J", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "K", "L", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "M", "N", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "O", "Q", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "R", "R", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "S", "T", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "U", "W", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "",						m_nSttRow, "X", "Z", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "���",					m_nSttRow,"AA","AA", 2, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Smax",							m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		if(n==0)
		{
			pOut->SetXL(pXL, "tu",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Bu",								m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "A",								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Yvsu",							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		else
		{
			pOut->SetXL(pXL, "tl",								m_nSttRow, "F", "F", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Bl",								m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "A",								m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
			pOut->SetXL(pXL, "Yvsl",							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		}
		pOut->SetXL(pXL, "Q",								m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "Iv",								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "s",								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "��a",								m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "��",								m_nSttRow, "U", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "��a",								m_nSttRow, "X", "Z", 1, BLACK, 9, FALSE, TA_CENTER);

		pOut->SetXL(pXL, "(kN)",							m_nSttRow, "D", "E", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "F", "F", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "G", "H", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm��)",							m_nSttRow, "I", "J", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "K", "L", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm��)",							m_nSttRow, "M", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm��)",							m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "R", "R", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(mm)",							m_nSttRow, "S", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(MPa)",						m_nSttRow, "U", "W", 0, BLACK, 8, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "(MPa)",						m_nSttRow, "X", "Z", 1, BLACK, 8, FALSE, TA_CENTER);

		double dSmaxTmp=0;
		long	nPosSec = -1;
		long	nSec	= 0;
		long   nG   = 0;
		double dSta = 0;
		double dStaSec = 0;
		POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
		while(pos)
		{
			CFrameSection *pFrameSec = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
			if(pFrameSec->m_nFlag != POS_GIRDER)	continue;

			nSec	= atoi(pFrameSec->m_szName);
			nG   = -1;
			dSta = 0;
			ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_SHEAR_2, CFemModelingCalc::MAX_ABS, FALSE);

			if (nG<0) nG=0;
			dStaSec = ModelCalc.GetStationSecJewon(nSec, nG, dSta, nPosSec);

			CPlateGirderApp		*pGir	= pBridgeApp->GetGirder(nG);
			CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dStaSec);

			dSmax	= fabs(StressCalc.GetElementForce(ELE_SHEAR_2, 0, nG, dSta, 4));	///< �ִ����ܷ�
			dTw		=  pBx->GetFactChain(G_W)->m_dFactChainThick;

			ModelCalc.GetMaxForcePositionOfSection(nSec, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
			ModelCalc.GetSectionCoefficient(nG, dStaSec);
			StressCalc.GetSectionStress(nG, dSta, TRUE, dStaSec);
			BOOL bPositiveM = StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);

			if(n==0)	nMaterial = G_F_U;
			else		nMaterial = G_F_L;
			CString	strLeft		= _T("");
			CString	strRight	= _T("");
			pCalcGeneral->GetMaterial(pBx, nMaterial, strLeft, strRight);

			CHGBaseSteel	*pSteel = pSteeCtl->m_SteelArr[pSteeCtl->GetIndex(strLeft + strRight)];
			CHGAllowStress	AllowShear(pSteel);
			
			if(n==0)
			{
				if(bPositiveM)
					dYvsu = fabs(ModelCalc.GetMapValueSecCo("Yvsu"));
				else
					dYvsu = fabs(StressCalc.GetMapValueSecStress("Yvsu"));
				dTu	= pBx->GetFactChain(G_F_U)->m_dFactChainThick;
				dWf	= pGir->GetWidthOnStation(dSta, TRUE);
				
				if(dSmaxTmp < dSmax)
				{
					dSmaxTmp	= dSmax;
					m_pVStiffBx	= pBx;
					m_nG		= nG;
				}
				m_nSectionSize++;	
			}
			else
			{
				if(bPositiveM)
					dYvsu = fabs(ModelCalc.GetMapValueSecCo("Yvsl"));
				else
					dYvsu = fabs(StressCalc.GetMapValueSecStress("Yvsl"));
				dTu	= pBx->GetFactChain(G_F_L)->m_dFactChainThick;
				dWf	= pGir->GetWidthOnStation(dSta, FALSE);
			
			}
			
			dS = pDataManage->GetFilletWeldStandard(dTu, dTw);
			dANu = AllowShear.GetAllowStress(ALLOW_WELD_SHEARSTRESS, dTu);
			if(pBridgeApp->IsTUGir())
			{
				if(bPositiveM)	dIv = ModelCalc.GetMapValueSecCo("Ivd33");
				else			dIv = StressCalc.GetMapValueSecStress("Irv");
			}
			else
			{
				if(bPositiveM)	dIv = ModelCalc.GetMapValueSecCo("Iv33");
				else			dIv = StressCalc.GetMapValueSecStress("Irv");
			}

			CString	sCell1, sCell2;
			sText.Format("�ܸ� - %d", nIndex+1);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "A", "C", 1, RED,   9, FALSE, TA_CENTER);
			///< Smax
			sSmax.Format("SMAX_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dSmax,					m_nSttRow, "D", "E", 1, RED,   9, FALSE, TA_CENTER, sSmax, 1);
			///< tu
			sTu.Format("TU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dTu,						m_nSttRow, "F", "F", 1, RED,   9, FALSE, TA_CENTER, sTu, 0);
			///< B
			sB.Format("B_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dWf,						m_nSttRow, "G", "H", 1, RED,   9, FALSE, TA_CENTER, sB, 1);
			///< A
			sText.Format("=ROUND(%s*%s,3)", pOut->GetCellRef(sTu), pOut->GetCellRef(sB));
			sA.Format("A_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "I", "J", 1, BLACK, 9, FALSE, TA_CENTER, sA, 1);
			///< Yvsu
			sYvsu.Format("YVSU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dYvsu,					m_nSttRow, "K", "L", 1, RED,   9, FALSE, TA_CENTER, sYvsu, 1);
			///< Q
			sCell1.Format("M%d", m_nSttRow);	sCell2.Format("M%d", m_nSttRow);
			sText.Format("=ROUND(%s*(%s-%s),3)", pOut->GetCellRef(sA), pOut->GetCellRef(sYvsu), pOut->GetCellRef(sTu));
			sQ.Format("Q_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "M", "N", 1, BLACK, 9, FALSE, TA_CENTER, sQ, 0);
			pXL->SetNumberFormat(sCell1, sCell2, "0.0E+00");
			///< Iv
			sCell1.Format("O%d", m_nSttRow);	sCell2.Format("Q%d", m_nSttRow);
			sIv.Format("IV_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dIv,						m_nSttRow, "O", "Q", 1, RED,   9, FALSE, TA_CENTER, sIv, 0);
			pXL->SetNumberFormat(sCell1, sCell2, "0.000E+00");
			///< s
			sS.Format("S_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dS,						m_nSttRow, "R", "R", 1, RED,   9, FALSE, TA_CENTER, sS, 0);
			///< ��a
			sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef(sS));
			sSigmaA.Format("SIGMAA_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "S", "T", 1, BLACK, 9, FALSE, TA_CENTER, sSigmaA, 2);
			///< ��
			sText.Format("=ROUND(%s*10^3*%s/(%s*%s),3)", pOut->GetCellRef(sSmax), pOut->GetCellRef(sQ), pOut->GetCellRef(sIv), pOut->GetCellRef(sSigmaA));
			sNu.Format("NU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow, "U", "W", 1, BLACK, 9, FALSE, TA_CENTER, sNu, 1);
			///< ��a
			sANu.Format("ANU_%s_%d", sFlange, nIndex);
			pOut->SetLineBoxText(pXL, dANu,						m_nSttRow, "X", "Z", 1, RED,   9, FALSE, TA_CENTER, sANu, 0);
			///< ���
			sText.Format("=IF(%s>%s,\"NG\",\"OK\")", pOut->GetCellRef(sNu), pOut->GetCellRef(sANu));
			pOut->SetLineBoxText(pXL, sText,					m_nSttRow,"AA","AA", 1, BLACK, 9, FALSE, TA_CENTER);
			SetLines(NEXT);
			nIndex++;
		}
		SetLines(1);
	}
}
///< ������ ������ ������ ����
void CXLWeldingCheck::ReinforceWeldCheck(BOOL bIsPier)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	CString	sText;

	GetJijumVStiffBxByMaxReaction(bIsPier);
	CPlateBasicIndex	*pBx		= pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)m_nMaxJoint));
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CVStiff				*pVStiff	= pGir->GetBxByStation(pBx->GetStation(), BX_JIJUMSTIFF)
									? pGir->GetBxByStation(pBx->GetStation(), BX_JIJUMSTIFF)->GetJijumStiff() : NULL;
	if(!pVStiff) return;

	long	nQty	= pGir->GetQtyJijumVStiffByBx(pBx);
	double	dHGir	= pGir->GetHeightGirderByStaAng(pBx->GetStation());
	double	dWidth	= pVStiff->m_dWidth[0];
	double	dThick	= pVStiff->m_dThick[0];
	double	dHeight	= pVStiff->GetVStiffHeight(TRUE, dHGir);

	long	nWeldType	= 0;//���� ����..
	long    dsThick     = 0;//�ʷ����� ġ��...

	CString sJuType		= pBridgeApp->GetMaterial(G_F_U);
	
	CString	strLeft		= _T("");
	CString	strRight	= _T("");
	pCalcGeneral->GetMaterial(pBx, G_F_U, strLeft, strRight);
	
	long	nMarkBuje1 = 0;
	long	nMarkBuje2 = 0;

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	CHGAllowStress	AllowShear(pSteel);
	double			dANu	= AllowShear.GetAllowStress(ALLOW_WELD_SHEARSTRESS, dThick);

	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
		{
			if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
			{
				nMarkBuje1 = V_ST; nMarkBuje1 = G_W;
			}
			else
			{
				if(pBx->IsJijum())
				{
					nMarkBuje1 = CP_BR_W;
					nMarkBuje2 = G_W;
				}
				else
				{
					nMarkBuje1 = CG_BR_W;
					nMarkBuje2 = G_W;
				}
			}
		}
		else if(pBx->GetSection())
		{
			nMarkBuje1 = V_ST;
			nMarkBuje2 = G_W;
		}
	}
	long nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje1,nMarkBuje2,pBx);
	if(nIdx==-1)
		nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje2,nMarkBuje1,pBx);
	CSteelWeldData *pWeld = pDataManage->GetSteelWeld()->GetBaseWeldByIndex(nIdx);

	nWeldType	= pWeld->m_nWeldType;//��������
	dsThick		= (long)pGir->GetSizeWeldFillet(pBx, nMarkBuje1, nMarkBuje2);//�ʷ�����ġ��

	if(bIsPier)
	{
		if(m_ArrBxPier.GetSize() == 0) return;
		pOut->SetXL(pXL, "��������",			m_nSttRow, "B", "B");
	}
	else
	{
		if(m_ArrBxAbut.GetSize() == 0) return;
		pOut->SetXL(pXL, "��) ������ ������ ������ ����",		m_nSttRow, "B", "B");
		pOut->SetXL(pXL, "�������",			m_nSttRow, "B", "B");
	}

	if(nWeldType == 0)//�ʷ�����...
	{
		pOut->SetXL(pXL, "Rmax",							m_nSttRow, "B", "C", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, m_dMaxReaction,					m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "MAX_REACTION");
		pOut->SetXL(pXL, "kN",								m_nSttRow, "H", "H", 0);

		pOut->SetXL(pXL, "�ʷ����� ġ�� s = ",				m_nSttRow, "L", "L", 0);
		pOut->SetXL(pXL, dsThick,								m_nSttRow, "Q", "R", 0, RED, 9, FALSE, TA_CENTER, "DSTHICK", 1);
		pOut->SetXL(pXL, "mm",								m_nSttRow, "S", "S", 1);


		pOut->InsertImage(pXL, "�ʷ�����", m_nSttRow, "C");

		pOut->SetXL(pXL, "��뺸����",						m_nSttRow, "L", "L", 0);
		pOut->SetXL(pXL, ":",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, nQty,								m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_QTY", 0);
		pOut->SetXL(pXL, "PL",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dWidth,							m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_W", 0);
		pOut->SetXL(pXL, "��",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dThick,							m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_T", 0);
		pOut->SetXL(pXL, "��",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dHeight,							m_nSttRow, "X", "Y", 1, RED,   9, FALSE, TA_CENTER, "VSTIFF_H", 0);

		///< �׸����� ġ������
		//pOut->SetXL(pXL, 2.0,		m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "PIC_DIM", 0);
		sText.Format("=%s", pOut->GetCellRef("DSTHICK"));
		pOut->SetXL(pXL, sText,		m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_CENTER, "THICK", 0);
		
		pOut->SetXL(pXL, "���� ��β�  a = s / ��2 = ",		m_nSttRow, "L", "L", 0);
		sText.Format("=%s/sqrt(2)",pOut->GetCellRef("DSTHICK"));
		pOut->SetXL(pXL, sText,							m_nSttRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "ATHICK", 1);
		pOut->SetXL(pXL, "mm",								m_nSttRow, "U", "U", 2);

		pOut->SetLineBoxText(pXL, "��",						m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetUnderLineText(pXL, "P",					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

		pOut->SetUnderLineText(pXL, "",						m_nSttRow, "R", "S", 0);
		sText.Format("=%s", pOut->GetCellRef("MAX_REACTION"));
		pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_REACTION_COEFF");
		pOut->SetUnderLineText(pXL, "��",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetUnderLineText(pXL, "10��",					m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_LEFT);
		
		pOut->SetXL(pXL, "��a��l",							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s*2", pOut->GetCellRef("VSTIFF_QTY"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_QTY_COEFF", 0);
		pOut->SetXL(pXL, "EA",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "��",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		//if(pWeld->m_nLayType!=1)	sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
		//else						sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef("THICK"));

		sText.Format("=%s", pOut->GetCellRef("ATHICK"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_T_COEFF", 1);
		pOut->SetXL(pXL, "��",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_H"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_H_COEFF", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=ROUND(%s*10^3/(%s*%s*%s), 3)", pOut->GetCellRef("MAX_REACTION_COEFF"), pOut->GetCellRef("VSTIFF_QTY_COEFF"), pOut->GetCellRef("VSTIFF_T_COEFF"), pOut->GetCellRef("VSTIFF_H_COEFF"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "NU");
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "R", 0);
		pOut->SetXL(pXL, "��a",								m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dANu,								m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "ANU", 0);
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
		sText.Format("=IF(%s>%s, \">\", \"��\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
		sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AA", 0, RED);
		SetLines(3);
	}
	else
	{
		pOut->SetXL(pXL, "Rmax",							m_nSttRow, "B", "C", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, m_dMaxReaction,					m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "MAX_REACTION");
		pOut->SetXL(pXL, "kN",							m_nSttRow, "H", "H", 1);

		pOut->InsertImage(pXL, "��������", m_nSttRow, "C");

		pOut->SetXL(pXL, "��뺸����",						m_nSttRow, "L", "L", 0);
		pOut->SetXL(pXL, ":",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, nQty,								m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_QTY", 0);
		pOut->SetXL(pXL, "PL",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dWidth,							m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_W", 0);
		pOut->SetXL(pXL, "��",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dThick,							m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_T", 0);
		pOut->SetXL(pXL, "��",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dHeight,							m_nSttRow, "X", "Y", 1, RED,   9, FALSE, TA_CENTER, "VSTIFF_H", 0);

		///< �׸����� ġ������
		pOut->SetXL(pXL, 2.0,		m_nSttRow, "I", "I", 0, BLACK, 9, FALSE, TA_LEFT, "PIC_DIM", 0);
		sText.Format("=(%s-%s)/2", pOut->GetCellRef("VSTIFF_T"), pOut->GetCellRef("PIC_DIM"));
		pOut->SetXL(pXL, sText,		m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_RIGHT, "THICK", 0);

		pOut->SetLineBoxText(pXL, "��",						m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
		pOut->SetUnderLineText(pXL, "P",					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

		pOut->SetUnderLineText(pXL, "",						m_nSttRow, "R", "S", 0);
		sText.Format("=%s", pOut->GetCellRef("MAX_REACTION"));
		pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_REACTION_COEFF");
		pOut->SetUnderLineText(pXL, "��",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetUnderLineText(pXL, "10��",					m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_LEFT);
		
		pOut->SetXL(pXL, "��a��l",							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_QTY"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_QTY_COEFF", 0);
		pOut->SetXL(pXL, "EA",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "��",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
//		if(pWeld->m_nLayType!=1)	sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
//		else						sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef("THICK"));
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_T_COEFF", 1);
		pOut->SetXL(pXL, "��",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=%s", pOut->GetCellRef("VSTIFF_H"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_H_COEFF", 0);
		pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
		sText.Format("=ROUND(%s*10^3/(%s*%s*%s), 3)", pOut->GetCellRef("MAX_REACTION_COEFF"), pOut->GetCellRef("VSTIFF_QTY_COEFF"), pOut->GetCellRef("VSTIFF_T_COEFF"), pOut->GetCellRef("VSTIFF_H_COEFF"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "NU");
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "R", 0);
		pOut->SetXL(pXL, "��a",								m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, "=",								m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, dANu,								m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "ANU", 0);
		pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
		sText.Format("=IF(%s>%s, \">\", \"��\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
		sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
		pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AA", 0, RED);
		SetLines(5);
	}

}

void CXLWeldingCheck::WeldVStiffCheck()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	CVStiff				*pVStiff= NULL;
	CSection *pSec = m_pVStiffBx->GetSection();
	if(pSec==NULL)		return;

	if(pSec->IsenDetType(SECDET_CROSSBEAM))
		pVStiff= pSec->GetCrossBeam()->m_pVStiff;
	else if(pSec->IsenDetType(SECDET_VBRACING))
		pVStiff= pSec->GetVBracing()->m_pVStiff;
	else 
		pVStiff= pSec->GetVStiff();
	CPlateBasicIndex *pBx = pVStiff->GetBx();
	long nVStiffsu	= 0;
	long nSide		= -1;
	if(pVStiff->m_dWidth[0]>0)
	{
		nSide = 0;
		nVStiffsu++;
	}
	else if(pVStiff->m_dWidth[1]>0)
	{
		nSide = 1;
		nVStiffsu++;
	}

	CString	sText, sText1, sText2;
	if(!pVStiff || nSide == -1) return;

	long	nQty	= nVStiffsu;
	double	dWidth	= pVStiff->m_dWidth[nSide];
	double	dThick	= pVStiff->m_dThick[nSide];
	double	dHeight	= pVStiff->GetVStiffHeight(nSide==0?TRUE:FALSE);

	CString sJuType		= pBridgeApp->GetMaterial(G_F_U);
	
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(sJuType)];
	CHGAllowStress	AllowStress(pSteel);

	double	dANu		= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, dThick);
	long	nMarkBuje1 = 0;
	long	nMarkBuje2 = 0;
	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
		{
			if(pBx->GetSection()->GetCrossBeam()->GetType()==CR_TYPE_HSTEEL)
			{
				nMarkBuje1 = V_ST;
				nMarkBuje2 = G_W;
			}
			else
			{
				if(pBx->IsJijum())
				{
					nMarkBuje1 = CP_BR_W;
					nMarkBuje2 = G_W;
				}
				else
				{
					nMarkBuje1 = CG_BR_W;
					nMarkBuje2 = G_W;
				}
			}
		}
		else if(pBx->GetSection())
		{
			nMarkBuje1 = V_ST;
			nMarkBuje2 = G_W;
		}
	}

	long nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje1,nMarkBuje2,pBx);
	if(nIdx==-1)
		nIdx = pBridgeApp->GetIndexBaseWeld(nMarkBuje2,nMarkBuje1,pBx);
	CSteelWeldData *pWeld = pDataManage->GetSteelWeld()->GetBaseWeldByIndex(nIdx);

	pOut->SetXL(pXL, "��) ���������� ������ ����",			m_nSttRow, "B", "B");
	pOut->SetXL(pXL, "Rmax",							m_nSttRow, "B", "C", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "D", "D", 0, BLACK, 9, FALSE, TA_CENTER);

	sText1.Format("SMAX_%s_%d", _T("UPPER"), 0);
	sText2.Format("SMAX_%s_%d", _T("UPPER"), m_nSectionSize-1);
	sText.Format("=MAX(%s:%s)", pOut->GetCellRef(sText1), pOut->GetCellRef(sText2));
	pOut->SetXL(pXL, sText,								m_nSttRow, "E", "G", 0, RED,   9, FALSE, TA_CENTER, "MAX_REACTION");
	pOut->SetXL(pXL, "kN",							m_nSttRow, "H", "H", 1);

	pOut->InsertImage(pXL, "��������_�����κ�����", m_nSttRow, "C");

	pOut->SetXL(pXL, "��뺸����",						m_nSttRow, "L", "L", 0);
	pOut->SetXL(pXL, ":",								m_nSttRow, "O", "O", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, nQty,								m_nSttRow, "P", "P", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_QTY", 0);
	pOut->SetXL(pXL, "PL",								m_nSttRow, "Q", "Q", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dWidth,							m_nSttRow, "R", "S", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_W", 0);
	pOut->SetXL(pXL, "��",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dThick,							m_nSttRow, "U", "V", 0, RED,   9, FALSE, TA_CENTER, "VSTIFF_T", 0);
	pOut->SetXL(pXL, "��",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dHeight,							m_nSttRow, "X", "Y", 1, RED,   9, FALSE, TA_CENTER, "VSTIFF_H", 0);

	///< �׸����� ġ������
	pOut->SetXL(pXL, 2.0,		m_nSttRow, "H", "H", 0, BLACK, 9, FALSE, TA_CENTER, "PIC_DIM", 0);
	sText.Format("=(%s-%s)/2", pOut->GetCellRef("VSTIFF_T"), pOut->GetCellRef("PIC_DIM"));
	pOut->SetXL(pXL, sText,		m_nSttRow, "G", "G", 0, BLACK, 9, FALSE, TA_CENTER, "THICK", 0);

	pOut->SetLineBoxText(pXL, "��",						m_nSttRow, "M", "M", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "N", "N", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);
	pOut->SetUnderLineText(pXL, "P",					m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetLineBoxText(pXL, "=",						m_nSttRow, "Q", "Q", 2, BLACK, 9, FALSE, TA_CENTER, "", 0, FALSE);

	pOut->SetUnderLineText(pXL, "",						m_nSttRow, "R", "S", 0);
	sText.Format("=%s", pOut->GetCellRef("MAX_REACTION"));
	pOut->SetUnderLineText(pXL, sText,					m_nSttRow, "T", "V", 0, BLACK, 9, FALSE, TA_CENTER, "MAX_REACTION_COEFF");
	pOut->SetUnderLineText(pXL, "��",					m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetUnderLineText(pXL, "10��",					m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_LEFT);
	
	pOut->SetXL(pXL, "��a��l",							m_nSttRow, "O", "P", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("VSTIFF_QTY"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "R", "R", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_QTY_COEFF", 0);
	pOut->SetXL(pXL, "EA",								m_nSttRow, "S", "S", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "��",								m_nSttRow, "T", "T", 0, BLACK, 9, FALSE, TA_CENTER);
	if(pWeld->m_nLayType!=1)	sText.Format("=%s", pOut->GetCellRef("VSTIFF_T"));
	else						sText.Format("=ROUND(2*%s/SQRT(2),3)", pOut->GetCellRef("THICK"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "U", "V", 0, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_T_COEFF", 1);
	pOut->SetXL(pXL, "��",								m_nSttRow, "W", "W", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=%s", pOut->GetCellRef("VSTIFF_H"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "X", "Y", 1, BLACK, 9, FALSE, TA_CENTER, "VSTIFF_H_COEFF", 0);
	pOut->SetXL(pXL, "=",								m_nSttRow, "N", "N", 0, BLACK, 9, FALSE, TA_CENTER);
	sText.Format("=ROUND(%s*10^3/(%s*%s*%s), 3)", pOut->GetCellRef("MAX_REACTION_COEFF"), pOut->GetCellRef("VSTIFF_QTY_COEFF"), pOut->GetCellRef("VSTIFF_T_COEFF"), pOut->GetCellRef("VSTIFF_H_COEFF"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "NU");
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "R", "R", 0);
	pOut->SetXL(pXL, "��a",								m_nSttRow, "U", "U", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, "=",								m_nSttRow, "V", "V", 0, BLACK, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, dANu,								m_nSttRow, "W", "X", 0, RED,   9, FALSE, TA_CENTER, "ANU", 0);
	pOut->SetXL(pXL, "MPa",						m_nSttRow, "Y", "Y", 0);
	sText.Format("=IF(%s>%s, \">\", \"��\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
	pOut->SetXL(pXL, sText,								m_nSttRow, "T", "T", 0, RED,   9, FALSE, TA_CENTER);
	sText.Format("=IF(%s<%s, \"O.K\", \"N.G\")", pOut->GetCellRef("NU"), pOut->GetCellRef("ANU"));
	pOut->SetXL(pXL, sText,								m_nSttRow,"AA","AA", 0, RED);	

	SetLines(7);
}

// m_nMaxJoint : ������ȣ (0 ���� ����)
void CXLWeldingCheck::GetJijumVStiffBxByMaxReaction(BOOL bPier)
{
	CDataManage		*pDataManage= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd	*pCalcStd	= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage	*pManage    = pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CResultArray	*pResult	= pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);

	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	m_ArrBxAbut.RemoveAll();
	m_ArrBxPier.RemoveAll();

	m_nMaxJoint		= 0;
	m_dMaxReaction	= 0;
	CJoint	*pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, FALSE);

	if(!bPier)
	{
		if(pJ)
		{
			m_ArrBxAbut.Add(pCalcStd->GetBxByJoint(pJ));
			m_nMaxJoint		= pJ->m_nIdx;
			CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(m_nMaxJoint);
			m_dMaxReaction	= maxabs(pJResult->m_vTotalMax[JOINT_REACTION].z, pJResult->m_vTotalMin[JOINT_REACTION].z);
		}
	}
	
	pJ = ModelCalc.GetJointMaxForceAtJijum(JOINT_REACTION, TRUE);

	if(bPier)
	{
		if(pJ)
		{
			m_ArrBxPier.Add(pCalcStd->GetBxByJoint(pJ));
			m_nMaxJoint		= pJ->m_nIdx;
			CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(m_nMaxJoint);
			m_dMaxReaction	= maxabs(pJResult->m_vTotalMax[JOINT_REACTION].z, pJResult->m_vTotalMin[JOINT_REACTION].z);
		}
	}
}

double CXLWeldingCheck::GetMaxReaction(BOOL bPier)
{
	GetJijumVStiffBxByMaxReaction(bPier);
	return m_dMaxReaction;
}

long CXLWeldingCheck::GetMaxJoint(BOOL bPier)
{
	GetJijumVStiffBxByMaxReaction(bPier);
	return m_nMaxJoint;
}

void CXLWeldingCheck::SetLines(int nLine)
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