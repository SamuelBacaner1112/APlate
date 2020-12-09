// XLCheckListUsing.cpp: implementation of the CXLCheckListUsing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplateoutcalc.h"
#include "XLCheckListUsing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXLCheckListUsing::CXLCheckListUsing(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_pAPlateOutCalcStd	= pStd;
}

CXLCheckListUsing::~CXLCheckListUsing()
{

}

void CXLCheckListUsing::Fatigue()
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CXLControl			*pXL			= m_pXL;
	CGeneralBaseStd	*pInStd			= m_pAPlateOutCalcStd->m_pGenInputStd;
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pManange		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CPlateBasicIndex	*pBx			= NULL;

	CString cs1 = (pInStd->GetCalcFatigue()->m_nRoadType == 0) ? "�ְ�������" : "��Ÿ����";
	CString cs2 = (pInStd->GetCalcFatigue()->m_nStruType == 0) ? "�����ϰ��" : "�����ϰ��";

	long nDB = 2;
	
	if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDB = 2;	//�ְ�������, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDB = 0;	//��Ÿ����, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDB = 6;	//�ְ�������, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDB = 4;	//��Ÿ����, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ

	long nDL = 1;

	if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDL = 1;	//�ְ�������, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDL = 0;	//��Ÿ����, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDL = 5;	//�ְ�������, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDL = 4;	//��Ÿ����, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ
	
	// ����
	long nSttRow = 1;
	CString sCell1, sCell2, sText, cs,csV;

	sCell1.Format("A%d", nSttRow);
	pXL->SetXL(sCell1, "������� CHECK LIST");
	pXL->SetFonts(sCell1, sCell1, 12, "����");

	nSttRow++;

	sCell1.Format("A%d", nSttRow);
	pXL->SetXL(sCell1, "11.��뼺����");
	pXL->SetFonts(sCell1, sCell1, 9, "����");

	nSttRow++;

	sCell1.Format("A%d", nSttRow);
	pXL->SetXL(sCell1, " ��) �Ƿΰ���");

	nSttRow++;

	sCell1.Format("A%d", nSttRow);
	pXL->SetXL(sCell1, "  1) �ڸ��Ʈ�� ���� �Ƿΰ���");
  
	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	pXL->SetXL(sCell1, "�ܸ� I");

	nSttRow++;

	sCell1.Format("M%d", nSttRow);
	pXL->SetXL(sCell1, "��f: �����Ƿ����¹��� (�ִ����°� �ּ������� ��) ,  fsr :����Ƿ����� ����");

	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("B%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left

	sCell1.Format("C%d", nSttRow);
	sCell2.Format("C%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 3, 1, 2);//top

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow+1);
	pXL->SetXL(sCell1, "����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow+1);
	pXL->SetXL(sCell1, "��             ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

	sCell1.Format("T%d", nSttRow);
	sCell2.Format("T%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);	
	pXL->SetXL(sCell1, "��f ( MPa ) ");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "��  ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);	
	pXL->SetXL(sCell1, "fsr ( MPa ) ");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow+1);
	pXL->SetXL(sCell1, "�� ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

	sCell1.Format("AP%d", nSttRow);
	sCell2.Format("AP%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

	nSttRow++;

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	pXL->SetXL(sCell1, "DB ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	pXL->SetXL(sCell1, "DL ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "��  ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	pXL->SetXL(sCell1, "DB ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	pXL->SetXL(sCell1, "DL ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow++;


	int nAllow[][8] = {
		{ 442, 260, 168, 168, 351, 203, 168, 168 },
		{ 344, 203, 126, 112, 274, 161, 112, 112 },
		{ 274, 161, 101,  84, 218, 126,  77,  77 },
		{ 250, 147,  91,  70, 196, 112,  70,  63 },
		{ 196, 112,  70,  49, 154,  91,  56,  35 },
		{ 154,  91,  56,  31, 119,  70,  42,  16 },
		{ 112,  64,  40,  18,  84,  49,  28,   9 },
		{ 105,  84,  63,  56,  84,  63,  49,  42 }
	};

	double	dRatio=0, Yvsu=0, Yvsl=0, Iv=0, DB=0, DL=0, dThickFlUP=0, dThickFlDN=0;
	long	nEleIdxDB=0, nNodeIdxDB=0;
	long	nG=0, nGSu = pBridgeApp->GetGirdersu();
	double	y=0, fDB=0, fDL=0;
	long	nHStiffDan=0;

	double	dHeightWStiff=0,	dSta		= 0;
	double	Sigma_DB11	= 0,	Sigma_DB12	= 0;
	double	Sigma_DB13	= 0,	Sigma_DB14	= 0;
	double	Sigma_DL11	= 0,	Sigma_DL12	= 0;
	double	Sigma_DL13	= 0,	Sigma_DL14	= 0;
	double	aDB11		= 0,	aDB12		= 0;		
	double	aDB13		= 0,	aDB14		= 0;
	double	aDL11		= 0,	aDL12		= 0;
	double	aDL13		= 0,	aDL14		= 0;

	long nCheckType = 0;
	// DB�϶� : ����� ���� ��ġ�� ����.
	nNodeIdxDB = pModelCalc->GetMaxFatigueIndex(nCheckType + 1, nEleIdxDB, dRatio, 1);  

	CString sPosType[4];	
	if(nNodeIdxDB >= 0)
	{
		DB = frkNM(pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DB_MAX, nEleIdxDB, dRatio) -
					pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DB_MIN, nEleIdxDB, dRatio));

		DL = frkNM(pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DL_MAX, nEleIdxDB, dRatio) -
					 pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DL_MIN, nEleIdxDB, dRatio));

		nG    = pModelCalc->GetGirderNoByJointID(nNodeIdxDB);
		pBx   = pCalcStd->GetBxByJoint(pManange->GetJoint((unsigned short)nNodeIdxDB));
		dSta  = pBx->GetStation();

		CVStiff			*pVStiff	= pBx->GetSection()->GetVStiff();
		CWebHStiff		*pWStiff	= pBx->GetWebHStiff(); 
		CHBracingGusset *pHGusset	= pBx->GetHBracingGusset();

		pStressCalc->GetSectionStress(nG, dSta);

		Yvsu = pStressCalc->GetMapValueSecStress("Yvsu");
		Yvsl = pStressCalc->GetMapValueSecStress("Yvsl");

		dThickFlUP = pBridgeApp->GetGirder(nG)->GetThickFactByStation(dSta, G_F_U);
		dThickFlDN = pBridgeApp->GetGirder(nG)->GetThickFactByStation(dSta, G_F_L);
		
		if(nCheckType==0 || nCheckType==2)	
			nHStiffDan = 0;
		else					
		{
			nHStiffDan = MAX(pWStiff->GetUpperDan(TRUE)+pWStiff->GetLowerDan(TRUE)
			                                ,pWStiff->GetUpperDan(FALSE)+pWStiff->GetLowerDan(FALSE)) -1;
		}

		dHeightWStiff		 =  pWStiff->GetHeightHStiffBySta(nHStiffDan, TRUE, dSta);

		if(dHeightWStiff==0)	
			pWStiff->GetHeightHStiffBySta(nHStiffDan, FALSE, dSta);

		if(nCheckType==1)
			Iv = pStressCalc->GetMapValueSecStress("Irv");  // �θ��Ʈ
		else
		{
			pModelCalc->GetSectionCoefficient(nG, dSta);  // �����Ʈ
			Iv = pModelCalc->GetMapValueSecCo("Iv33");  // �����Ʈ
		}

		double dHeightGir = pBx->GetHeightGirderByBx();
		
		CPlateGirderApp *pGir = pBridgeApp->GetGirder(nG);
		CSection        *pSec = pBx->GetSection();

		if(pSec == NULL) pSec = pBx->GetSectionLeft();

		if(pSec || nGSu == 1) 
		{						
			long nPosIdx, nDBCol, nDLCol;

			nDBCol  = (pInStd->GetCalcFatigue()->m_nRoadType == 0) ? 2 : 0;
			nDLCol  = (pInStd->GetCalcFatigue()->m_nRoadType == 0) ? 1 : 0;
			nDBCol += (pInStd->GetCalcFatigue()->m_nStruType == 0) ? 0 : 4;
			nDLCol += (pInStd->GetCalcFatigue()->m_nStruType == 0) ? 0 : 4;

			CWebHStiff *pHs		= pBx->GetWebHStiff();
			long		nPosSu	= 4;

			for (long nPos = 0; nPos < nPosSu; nPos++)  
			{				
				if(nCheckType == 0 || nCheckType == 2)
				{
					switch (nPos) 
					{
					case 0 : y = Yvsl+dThickFlDN;	break;			///< �÷����� �������� ���� �ʷ� ������
					case 1 : y = Yvsl+dThickFlDN+MIN(pVStiff->m_dGap[0], pVStiff->m_dGap[1]); break; ///< ������������ ���� ���κ�
					case 2 : y = Yvsu-dThickFlUP-dHeightWStiff;	break;	///< ���򺸰��� ���� ���κ�
					case 3 : y = Yvsl+pHGusset->m_ddH-pHGusset->m_dT;	break;	///< �����ǿ� ������ ���κ� ����� �ż��� ���� ���κ�
					}		
					switch (nPos)  
					{
					case 0 : sPosType[nPos] = "B";	nPosIdx  = 1;	break;
					case 1 : sPosType[nPos] = "C";	nPosIdx  = 3;	break;
					case 2 : sPosType[nPos] = "E";	nPosIdx  = 5;	break;
					case 3 : sPosType[nPos] = "E";	nPosIdx  = 4;	break;
					}
				}
				else
				{
					switch (nPos) 
					{
					case 0 : y = Yvsu-dThickFlUP;	break;					///< �÷����� �������� ���� �ʷ� ������
					case 1 : y = Yvsu-dThickFlUP;	break;					///< �����κ����� ���� ���κ�
					case 2 : y = Yvsu-dThickFlUP-dHeightWStiff;	break;		///< ���򺸰��� ���� ���κ�
					case 3 : y = Yvsu;				break;					///< ���͵忡 ������ ������ �÷�����	
					}
					switch (nPos)  
					{
					case 0 : sPosType[nPos] = "B";	nPosIdx  = 1;	break;
					case 1 : sPosType[nPos] = "C";	nPosIdx  = 3;	break;
					case 2 : sPosType[nPos] = "E";	nPosIdx  = 5;	break;
					case 3 : sPosType[nPos] = "C";	nPosIdx  = 3;	break;
					}
				}				
				fDB = ABS(DB * y / Iv);
				fDL = ABS(DL * y / Iv);

				if(nPos==0)			Sigma_DB11 = fDB, Sigma_DL11 = fDL, aDB11 = nAllow[nPosIdx][nDBCol], aDL11 = nAllow[nPosIdx][nDLCol];
				else if(nPos==1)	Sigma_DB12 = fDB, Sigma_DL12 = fDL, aDB12 = nAllow[nPosIdx][nDBCol], aDL12 = nAllow[nPosIdx][nDLCol];
				else if(nPos==2)	Sigma_DB13 = fDB, Sigma_DL13 = fDL, aDB13 = nAllow[nPosIdx][nDBCol], aDL13 = nAllow[nPosIdx][nDLCol];
				else if(nPos==3)	Sigma_DB14 = fDB, Sigma_DL14 = fDL, aDB14 = nAllow[nPosIdx][nDBCol], aDL14 = nAllow[nPosIdx][nDLCol];
			}			
		}
	}

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow);

	pXL->SetXL(sCell1, "��");

	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow);
	pXL->SetXL(sCell1, "�÷����� �������� ���� �ʷ� ������");
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	sText.Format("%.3f", Sigma_DB11);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	sText.Format("%.3f", Sigma_DL11);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "B");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	sText.Format("%.f", aDB11);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	sText.Format("%.f", aDL11);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
	
	pXL->SetXL(sCell1, sText);

	if(Sigma_DB11>aDB11||Sigma_DL11>aDL11)
		pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);

	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow);
	pXL->SetXL(sCell1, "��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow);
	pXL->SetXL(sCell1, "������ �÷����� ���̾������� �ʷ� ������");
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	sText.Format("%.3f", Sigma_DB12);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	sText.Format("%.3f", Sigma_DL12);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "C");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	sText.Format("%.f", aDB12);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	sText.Format("%.f", aDL12);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
	pXL->SetXL(sCell1, sText);
	
	if(Sigma_DB12>aDB12||Sigma_DL12>aDL12)
		pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);

	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow);
	pXL->SetXL(sCell1, "��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow);
	pXL->SetXL(sCell1, "�����ǿ� ������ ���κ� �ż��� ���� ���κ�");
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	sText.Format("%.3f", Sigma_DB13);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	sText.Format("%.3f", Sigma_DL13);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "E");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	sText.Format("%.f", aDB13);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	sText.Format("%.f", aDL13);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
	pXL->SetXL(sCell1, sText);
	
	if(Sigma_DB13>aDB13||Sigma_DL13>aDL13)
		pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);

	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow);
	pXL->SetXL(sCell1, "��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow);
	pXL->SetXL(sCell1, "���򺸰��� ���� ���κ�");
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	sText.Format("%.3f", Sigma_DB14);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	sText.Format("%.3f", Sigma_DL14);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "E");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	sText.Format("%.f", aDB14);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	sText.Format("%.f", aDL14);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
	pXL->SetXL(sCell1, sText);
	
	if(Sigma_DB14>aDB14||Sigma_DL14>aDL14)
		pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);

	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow += 2;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
//	OnProgress(0, 60);
	
	//�ܸ������ 1��ŭ �� ��
	long nChecksu = min(pBridgeApp->m_nQtyJigan, 3) - 1;

	for(long nDan = 0; nDan < nChecksu; nDan++)
	{
		sCell1.Format("B%d", nSttRow);
		sText = !nDan  ? "�ܸ� II" : "�ܸ� III";

		pXL->SetXL(sCell1, sText);

		nSttRow++;

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("B%d", nSttRow+1);
		pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left

		sCell1.Format("C%d", nSttRow);
		sCell2.Format("C%d", nSttRow+1);
		pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 3, 1, 2);//top

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("C%d", nSttRow+1);
		pXL->SetXL(sCell1, "����");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

		sCell1.Format("D%d", nSttRow);
		sCell2.Format("T%d", nSttRow+1);
		pXL->SetXL(sCell1, "��             ��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

		sCell1.Format("T%d", nSttRow);
		sCell2.Format("T%d", nSttRow+1);
		pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

		sCell1.Format("U%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);	
		pXL->SetXL(sCell1, "��f ( MPa ) ");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AE%d", nSttRow);
		pXL->SetXL(sCell1, "��  ��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AF%d", nSttRow);
		sCell2.Format("AM%d", nSttRow);	
		pXL->SetXL(sCell1, "fsr ( MPa ) ");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		sCell1.Format("AN%d", nSttRow);
		sCell2.Format("AP%d", nSttRow+1);
		pXL->SetXL(sCell1, "�� ��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

		sCell1.Format("AP%d", nSttRow);
		sCell2.Format("AP%d", nSttRow+1);
		pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

		nSttRow++;

		sCell1.Format("U%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		pXL->SetXL(sCell1, "DB ����");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		pXL->SetXL(sCell1, "DL ����");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AE%d", nSttRow);
		pXL->SetXL(sCell1, "��  ��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AF%d", nSttRow);
		sCell2.Format("AI%d", nSttRow);
		pXL->SetXL(sCell1, "DB ����");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AJ%d", nSttRow);
		sCell2.Format("AM%d", nSttRow);
		pXL->SetXL(sCell1, "DL ����");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		nCheckType = nDan + 1;
		nNodeIdxDB = pModelCalc->GetMaxFatigueIndex(nCheckType + 1, nEleIdxDB, dRatio, 1);  
		if(nNodeIdxDB >= 0)
		{
			DB = frkNM(pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DB_MAX, nEleIdxDB, dRatio) -
						pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DB_MIN, nEleIdxDB, dRatio));

			DL = frkNM(pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DL_MAX, nEleIdxDB, dRatio) -
						 pStressCalc->GetOneEleForce(ELE_MOMENT3, LC_DL_MIN, nEleIdxDB, dRatio));

			nG    = pModelCalc->GetGirderNoByJointID(nNodeIdxDB);
		pBx   = pCalcStd->GetBxByJoint(pManange->GetJoint((unsigned short)nNodeIdxDB));
			dSta  = pBx->GetStation();

			CVStiff    *pVStiff = pBx->GetSection()->GetVStiff();
			CWebHStiff *pWStiff = pBx->GetWebHStiff(); 
			CHBracingGusset *pHGusset = pBx->GetHBracingGusset();
			pStressCalc->GetSectionStress(nG, dSta);

			Yvsu = pStressCalc->GetMapValueSecStress("Yvsu");
			Yvsl = pStressCalc->GetMapValueSecStress("Yvsl");
			dThickFlUP = pBridgeApp->GetGirder(nG)->GetThickFactByStation(dSta, G_F_U);
			dThickFlDN = pBridgeApp->GetGirder(nG)->GetThickFactByStation(dSta, G_F_L);
			
			if(nCheckType==0 || nCheckType==2)	nHStiffDan = 0;
			else					nHStiffDan = MAX(pWStiff->GetUpperDan(TRUE)+pWStiff->GetLowerDan(TRUE)
												,pWStiff->GetUpperDan(FALSE)+pWStiff->GetLowerDan(FALSE)) -1;
			dHeightWStiff		 =  pWStiff->GetHeightHStiffBySta(nHStiffDan, TRUE, dSta);
			if(dHeightWStiff==0)	pWStiff->GetHeightHStiffBySta(nHStiffDan, FALSE, dSta);

			if(nCheckType==1)
				Iv = pStressCalc->GetMapValueSecStress("Irv");  // �θ��Ʈ
			else
			{
				pModelCalc->GetSectionCoefficient(nG, dSta);  // �����Ʈ
				Iv = pModelCalc->GetMapValueSecCo("Iv33");  // �����Ʈ
			}

			double dHeightGir = pBx->GetHeightGirderByBx();
			
			CPlateGirderApp *pGir = pBridgeApp->GetGirder(nG);
			CSection        *pSec = pBx->GetSection();
			if(pSec == NULL) pSec = pBx->GetSectionLeft();

			if(pSec || nGSu == 1) 
			{						
				long nPosIdx = 0, nDBCol=0, nDLCol=0;
				nDBCol  = (pInStd->GetCalcFatigue()->m_nRoadType == 0) ? 2 : 0;
				nDLCol  = (pInStd->GetCalcFatigue()->m_nRoadType == 0) ? 1 : 0;
				nDBCol += (pInStd->GetCalcFatigue()->m_nStruType == 0) ? 0 : 4;
				nDLCol += (pInStd->GetCalcFatigue()->m_nStruType == 0) ? 0 : 4;

				CWebHStiff *pHs = pBx->GetWebHStiff();
				long nPosSu = 4;
				for (long nPos = 0; nPos < nPosSu; nPos++)  
				{				
					if(nCheckType==0 || nCheckType==2)
					{
						switch (nPos) 
						{
						case 0 : y = Yvsl+dThickFlDN;	break;			///< �÷����� �������� ���� �ʷ� ������
						case 1 : y = Yvsl+dThickFlDN+MIN(pVStiff->m_dGap[0], pVStiff->m_dGap[1]); break; ///< ������������ ���� ���κ�
						case 2 : y = Yvsu-dThickFlUP-dHeightWStiff;	break;	///< ���򺸰��� ���� ���κ�
						case 3 : y = Yvsl+pHGusset->m_ddH-pHGusset->m_dT;	break;	///< �����ǿ� ������ ���κ� ����� �ż��� ���� ���κ�
						}					
						switch (nPos)  
						{
						case 0 : sPosType[nPos] = "B";	nPosIdx  = 1;	break;
						case 1 : sPosType[nPos] = "C";	nPosIdx  = 3;	break;
						case 2 : sPosType[nPos] = "E";	nPosIdx  = 5;	break;
						case 3 : sPosType[nPos] = "E";	nPosIdx  = 4;	break;
						}

					}
					else
					{
						switch (nPos) 
						{
						case 0 : y = Yvsu-dThickFlUP;	break;					///< �÷����� �������� ���� �ʷ� ������
						case 1 : y = Yvsu-dThickFlUP;	break;					///< �����κ����� ���� ���κ�
						case 2 : y = Yvsu-dThickFlUP-dHeightWStiff;	break;		///< ���򺸰��� ���� ���κ�
						case 3 : y = Yvsu;				break;					///< ���͵忡 ������ ������ �÷�����	
						}
						switch (nPos)  
						{
						case 0 : sPosType[nPos] = "B";	nPosIdx  = 1;	break;
						case 1 : sPosType[nPos] = "C";	nPosIdx  = 3;	break;
						case 2 : sPosType[nPos] = "E";	nPosIdx  = 5;	break;
						case 3 : sPosType[nPos] = "C";	nPosIdx  = 3;	break;
						}
					}				
					fDB = ABS(DB * y / Iv);
					fDL = ABS(DL * y / Iv);

					if(nPos==0)			Sigma_DB11 = fDB, Sigma_DL11 = fDL, aDB11 = nAllow[nPosIdx][nDBCol], aDL11 = nAllow[nPosIdx][nDLCol];
					else if(nPos==1)	Sigma_DB12 = fDB, Sigma_DL12 = fDL, aDB12 = nAllow[nPosIdx][nDBCol], aDL12 = nAllow[nPosIdx][nDLCol];
					else if(nPos==2)	Sigma_DB13 = fDB, Sigma_DL13 = fDL, aDB13 = nAllow[nPosIdx][nDBCol], aDL13 = nAllow[nPosIdx][nDLCol];
					else if(nPos==3)	Sigma_DB14 = fDB, Sigma_DL14 = fDL, aDB14 = nAllow[nPosIdx][nDBCol], aDL14 = nAllow[nPosIdx][nDLCol];
				}			
			}
		}

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("C%d", nSttRow);
		pXL->SetXL(sCell1, "��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("D%d", nSttRow);
		sCell2.Format("T%d", nSttRow);
		pXL->SetXL(sCell1, "�÷����� �������� ���� �ʷ� ������");
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("U%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("%.3f", Sigma_DB11);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_DL11);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AE%d", nSttRow);
		pXL->SetXL(sCell1, "B");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AF%d", nSttRow);
		sCell2.Format("AI%d", nSttRow);
		sText.Format("%.f", aDB11);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AJ%d", nSttRow);
		sCell2.Format("AM%d", nSttRow);
		sText.Format("%.f", aDL11);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AN%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		if(Sigma_DB11>aDB11||Sigma_DL11>aDL11)
			pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("C%d", nSttRow);
		pXL->SetXL(sCell1, "��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("D%d", nSttRow);
		sCell2.Format("T%d", nSttRow);
		pXL->SetXL(sCell1, "������ �÷����� ���̾������� �ʷ� ������");
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("U%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("%.3f", Sigma_DB12);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_DL12);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AE%d", nSttRow);
		pXL->SetXL(sCell1, "C");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AF%d", nSttRow);
		sCell2.Format("AI%d", nSttRow);
		sText.Format("%.f", aDB12);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AJ%d", nSttRow);
		sCell2.Format("AM%d", nSttRow);
		sText.Format("%.f", aDL12);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AN%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		if(Sigma_DB12>aDB12||Sigma_DL12>aDL12)
			pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("C%d", nSttRow);
		pXL->SetXL(sCell1, "��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("D%d", nSttRow);
		sCell2.Format("T%d", nSttRow);
		pXL->SetXL(sCell1, "�����ǿ� ������ ���κ� �ż��� ���� ���κ�");
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("U%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("%.3f", Sigma_DB13);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_DL13);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AE%d", nSttRow);
		pXL->SetXL(sCell1, "E");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AF%d", nSttRow);
		sCell2.Format("AI%d", nSttRow);
		sText.Format("%.f", aDB13);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AJ%d", nSttRow);
		sCell2.Format("AM%d", nSttRow);
		sText.Format("%.f", aDL13);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AN%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		if(Sigma_DB13>aDB13||Sigma_DL13>aDL13)
			pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow++;

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("C%d", nSttRow);
		pXL->SetXL(sCell1, "��");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("D%d", nSttRow);
		sCell2.Format("T%d", nSttRow);
		pXL->SetXL(sCell1, "���򺸰��� ���� ���κ�");
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("U%d", nSttRow);
		sCell2.Format("X%d", nSttRow);
		sText.Format("%.3f", Sigma_DB14);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("Y%d", nSttRow);
		sCell2.Format("AB%d", nSttRow);
		sText.Format("%.3f", Sigma_DL14);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AC%d", nSttRow);
		sCell2.Format("AE%d", nSttRow);
		pXL->SetXL(sCell1, !nDan ? "C" : "E");
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AF%d", nSttRow);
		sCell2.Format("AI%d", nSttRow);
		sText.Format("%.f", aDB14);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AJ%d", nSttRow);
		sCell2.Format("AM%d", nSttRow);
		sText.Format("%.f", aDL14);
		pXL->SetXL(sCell1, sText);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetNumberFormat(sCell1,sCell2, "0");
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("AN%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
		pXL->SetXL(sCell1, sText);
		if(Sigma_DB14>aDB14||Sigma_DL14>aDL14)
			pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);
		pXL->SetMergeCell(sCell1, sCell2);//����
		pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
		pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

		sCell1.Format("B%d", nSttRow);
		sCell2.Format("AP%d", nSttRow);
		pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

		nSttRow += 2;

	}

	OutExcelCheck11_Piro_Shear(nSttRow);
}

void CXLCheckListUsing::OutExcelCheck11_Piro_Shear(long &nSttRow)
{
	CPlateBridgeApp		*pBridgeApp		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CXLControl			*pXL			= m_pXL;
	CGeneralBaseStd	*pInStd			= m_pAPlateOutCalcStd->m_pGenInputStd;
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pManange		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CPlateBasicIndex	*pBx			= NULL;
	long nDB = 2;

	if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDB = 2;	//�ְ�������, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDB = 0;	//��Ÿ����, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDB = 6;	//�ְ�������, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDB = 4;	//��Ÿ����, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ

	long nDL = 1;

	if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDL = 1;	//�ְ�������, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 0))
		nDL = 0;	//��Ÿ����, �����ϰ�ο� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 0)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDL = 5;	//�ְ�������, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ
	else if((pInStd->GetCalcFatigue()->m_nRoadType == 1)&&(pInStd->GetCalcFatigue()->m_nStruType == 1))
		nDL = 4;	//��Ÿ����, �����ϰ�� �ش��ϴ� �Ʒ� �迭��ȣ

	int nAllow[] = { 105,  84,  63,  56,  84,  63,  49,  42 };	// MPa

	double dRatio, Yvsu, Iv, DB, DL;
	long nEleIdxDB, nNodeIdxDB;
	long nG;

	nNodeIdxDB = pModelCalc->GetMaxFatigueIndex(4, nEleIdxDB, dRatio, 1);  
	CPlateBasicIndex *pBxNode = pCalcStd->GetBxByJoint(pManange->GetJoint((unsigned short)nNodeIdxDB));

	BOOL bFixPostiveMoment = FALSE;
	if(pBxNode->IsDanbuJijum())	// �Ŵ� �����ΰ� �θ��Ʈ�� �̼��ϰ� ���� ���� �����ϰ� �����Ʈ�� ó����.
		bFixPostiveMoment = TRUE;

	double Tau_DBu=0, Tau_DBl=0, Tau_DLu=0, Tau_DLl=0, aDBu =0, aDLu=0, aDBl=0, aDLl=0;

	if (nNodeIdxDB >= 0)  
	{
		double Yvsl, tu, tl, tw, Bu, Bl, su, sl;
		DB = frkN(pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, nEleIdxDB, dRatio)
			 -pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, nEleIdxDB, dRatio));

		DL = frkN(pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, nEleIdxDB, dRatio)
			 -pStressCalc->GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, nEleIdxDB, dRatio));

		nG    = pModelCalc->GetGirderNoByJointID(nNodeIdxDB);
		pBx   = pCalcStd->GetBxByJoint(pManange->GetJoint((unsigned short)nNodeIdxDB));

		pModelCalc->GetSectionCoefficient(nG, pBx->GetStation());

		Yvsu = pModelCalc->GetMapValueSecCo("Yvsu");
		Yvsl = -pModelCalc->GetMapValueSecCo("Yvsl");
		double dSta = pBx->GetStation();

		if(pStressCalc->GetElementForce(ELE_MOMENT3, LC_PREV_S, nG, dSta, 0) > 0.0
			|| bFixPostiveMoment)
		{
			pModelCalc->GetSectionCoefficient(nG, dSta);
			Iv = pModelCalc->GetMapValueSecCo("Iv33");  // �����Ʈ
		}
		else
		{
			pStressCalc->GetSectionStress(nG, dSta);
			Iv = pStressCalc->GetMapValueSecStress("Irv");  // �θ��Ʈ
		}
		
		CPlateGirderApp *pGir = pBridgeApp->GetGirder(nG);
		tu = pGir->GetThickFactByStation(dSta, G_F_U);
		tl = pGir->GetThickFactByStation(dSta, G_F_L);
		tw = pGir->GetThickFactByStation(dSta, G_W);
		Bu = pGir->GetWidthFlangeUpper(pBx);
		Bl = pGir->GetWidthFlangeLower(pBx);
		su = max(RoundUp(sqrt(2 * max(tu, tw)), 0), 6.0);
		sl = max(RoundUp(sqrt(2 * max(tl, tw)), 0), 6.0);

		long nDBCol, nDLCol;
		nDBCol  = (pInStd->GetCalcFatigue()->m_nRoadType == 0) ? 2 : 0;
		nDLCol  = (pInStd->GetCalcFatigue()->m_nRoadType == 0) ? 1 : 0;
		nDBCol += (pInStd->GetCalcFatigue()->m_nStruType == 0) ? 0 : 4;
		nDLCol += (pInStd->GetCalcFatigue()->m_nStruType == 0) ? 0 : 4;
		double aDB, aDL;
		aDB = nAllow[nDBCol];
		aDL = nAllow[nDLCol];

		for (long nUpper = 0; nUpper < 2; nUpper++) 
		{
			double t, B, A, Yv, Q, s, Sa, fDB, fDL;
			t  = nUpper ? tu : tl;
			B  = nUpper ? Bu : Bl;
			Yv = nUpper ? Yvsu : Yvsl;
			s  = nUpper ? su : sl;
			A  = t * B;
			Q  = A * (Yv - (t/20));
			Sa = s * 4 / sqrt(2.0);
			fDB = DB * Q / (Iv * Sa);
			fDL = DL * Q / (Iv * Sa);
			if(nUpper!=0) Tau_DBu = fDB, Tau_DLu = fDL, aDBu = aDB, aDLu = aDL;
			else		  Tau_DBl = fDB, Tau_DLl = fDL, aDBl = aDB, aDLl = aDL;
		}
	}

	CString sCell1, sCell2, sText;

	sCell1.Format("A%d", nSttRow);
	pXL->SetXL(sCell1, "  2) ���ܿ� ���� �Ƿΰ��� - �÷����� �������� �ʷ������ο� ���� �����Ƿΰ���");

	nSttRow++;

	sCell1.Format("C%d", nSttRow);
	pXL->SetXL(sCell1, "���� �ܸ��� ���ܷ��� ���� ū");

	sCell1.Format("O%d", nSttRow);
	pXL->SetXL(sCell1, "����");

	sCell1.Format("Q%d", nSttRow);
	sCell2.Format("R%d", nSttRow);
	sText.Format("%d", nNodeIdxDB+1);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->SetNumberFormat(sCell1,sCell1, "0");

	sCell1.Format("S%d", nSttRow);
	pXL->SetXL(sCell1, "�� ���� �Ͽ���");

	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("B%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 1, 1, 2);//Left

	sCell1.Format("C%d", nSttRow);
	sCell2.Format("C%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 3, 1, 2);//top

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow+1);
	pXL->SetXL(sCell1, "����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow+1);
	pXL->SetXL(sCell1, "��             ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

	sCell1.Format("T%d", nSttRow);
	sCell2.Format("T%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);	
	pXL->SetXL(sCell1, "��f ( MPa ) ");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "��  ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);	
	pXL->SetXL(sCell1, "fsr ( MPa ) ");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow+1);
	pXL->SetXL(sCell1, "�� ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);

	sCell1.Format("AP%d", nSttRow);
	sCell2.Format("AP%d", nSttRow+1);
	pXL->CellLine(sCell1, sCell2, 2, 1, 2);//Right

	nSttRow++;

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	pXL->SetXL(sCell1, "DB ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	pXL->SetXL(sCell1, "DL ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "��  ��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	pXL->SetXL(sCell1, "DB ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	pXL->SetXL(sCell1, "DL ����");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow);
	pXL->SetXL(sCell1, "��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow);
	pXL->SetXL(sCell1, "����÷����� �������� �ʷ� ������");
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	sText.Format("%.3f", Tau_DBu);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	sText.Format("%.3f", Tau_DLu);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "F");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	sText.Format("%.f", aDBu);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	sText.Format("%.f", aDLu);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
	if(Tau_DBu>aDBu||Tau_DLu>aDLu)
		pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow++;

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("C%d", nSttRow);
	pXL->SetXL(sCell1, "��");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell1, sCell1, 1, 1, 2);//Left
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("D%d", nSttRow);
	sCell2.Format("T%d", nSttRow);
	pXL->SetXL(sCell1, "�Ϻ��÷����� �������� �ʷ� ������");
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("U%d", nSttRow);
	sCell2.Format("X%d", nSttRow);
	sText.Format("%.3f", Tau_DBl);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("Y%d", nSttRow);
	sCell2.Format("AB%d", nSttRow);
	sText.Format("%.3f", Tau_DLl);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AC%d", nSttRow);
	sCell2.Format("AE%d", nSttRow);
	pXL->SetXL(sCell1, "F");
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AF%d", nSttRow);
	sCell2.Format("AI%d", nSttRow);
	sText.Format("%.f", aDBl);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AJ%d", nSttRow);
	sCell2.Format("AM%d", nSttRow);
	sText.Format("%.f", aDLl);
	pXL->SetXL(sCell1, sText);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetNumberFormat(sCell1,sCell2, "0");
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("AN%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	sText.Format("=IF(AND(U%d<=AF%d,Y%d<=AJ%d),\"O.K.\",\"N.G.\")", nSttRow, nSttRow, nSttRow, nSttRow);
	pXL->SetXL(sCell1, sText);
	if(Tau_DBl>aDBl||Tau_DLl>aDLl)
		pXL->SetFonts(sCell1, sCell1, 9, "����", 3, FALSE);
	pXL->SetMergeCell(sCell1, sCell2);//����
	pXL->SetHoriAlign(sCell1, sCell2, TA_CENTER);
	pXL->CellLine(sCell2, sCell2, 2, 1, 2);//Right

	sCell1.Format("B%d", nSttRow);
	sCell2.Format("AP%d", nSttRow);
	pXL->CellLine(sCell1, sCell2, 4, 1, 2);//bottom

	nSttRow++;
}

void CXLCheckListUsing::OutExcelCheck11_Deflection()
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CAPlateOutExcel		*pOutExcel		= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CFemModelingCalc    ModelCalc(pDataManage, pCalcStd);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFEMManage          *pManage        = pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcStd->GetAPlateOutCalcXL();	
	CXLControl			*pXL			= m_pXL;

	m_nSttRow = 1;
	CString sMark, sText;
	pOut->ClearCellRef();
	pOut->SetColsWidth(pXL, 0, 100, 6);
	pOut->SetXL(pXL, "'11.2 ó�� ����"						, m_nSttRow, "A", "A", 1, BLACK, 9);
	pOutExcel->SetProgress("11.2 ó�� ����", pOutExcel->GetProgress()+10);

	pOut->SetXL(pXL, "    ��������� ������ Ȱ���߿� ���� �� ��ġ�� �ִ� ó���� ( ���� OUTPUT ���� )    [���α� �������]  56 page", m_nSttRow, "A", "A", 1);

	if(!pCalcData->m_bReadLiveDispOutput)
	{
		pOut->SetXL(pXL, "***** ó�������� ���� �ʾҽ��ϴ�.              *****", m_nSttRow, "A", "A", 1, BLACK, 9);			
		return;
	}
	
	char sBuf[40];
	int nRow = 3, nNodeColSu = 10;
	long nLoadCase = pManage->GetLoadCaseIndex("DESIGN MIN");

	// �Ŵ����� ���
	long nSttOutLineRow, nEndOutLineRow;
	for (long nG = 0; nG < pDB->GetGirdersu(); nG++)
	{
		nSttOutLineRow = nRow + 2;
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		long nIdxPrev, nIdxNext;

		ModelCalc.GetJointRangeByGirder(nG, nIdxPrev, nIdxNext);

		sprintf(sBuf, "< Girder %d >", nG + 1);
		pXL->SetXL(nRow, 0, sBuf);
		pXL->SetXL(nRow, nNodeColSu - 1, "( ���� : mm )");
		int nCol = 0;
		int nNode = 0;
		for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++)
		{
			if (nCol == 0 || nCol > nNodeColSu)   
			{   // �� �ѱ�.
				nRow += 2;
				pXL->SetHoriAlign(nRow, 0, nRow + 1, nNodeColSu, TA_CENTER);
				pXL->SetXL(nRow,     0, "����NO.");
				pXL->SetXL(nRow + 1, 0, "ó �� ��");
				nCol = 1;
			}
			pXL->SetXL(nRow, nCol, _ITOT_S(nNode + 1, sBuf, 10));   // ������ȣ
			pXL->SetNumberFormat(nRow, nCol, "0");

			CJoint *pJ = pManage->GetJoint(nNode);
			double dValue = -frM(pJ->GetResultForce(nLoadCase)->m_Disp.z);
//			if(pManage->GetJoint(nNode)->IsRestraint())	dValue = 0.0;
			if(ModelCalc.IsRestraintJoint(nNode))	dValue = 0; 

			sprintf(sBuf, "'%.3f", dValue);
			pXL->SetXL(nRow + 1, nCol, sBuf);   // ó����
			nCol++;
		}
		nEndOutLineRow = nRow + 1;
		pXL->CellOutLineAll(nSttOutLineRow, 0, nEndOutLineRow, nNodeColSu);

		// �׸� �߰�.
		nRow += 4;
		CDomyun Dom;
		Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom.SetScaleDim(500.0);
		m_pAPlateOutCalcStd->GetGeneralInputStd()->GetCalcSlabDisplacement()->DisplacementOutput(&Dom, nG, nLoadCase);

		double dScale = min(480 / pDB->GetLengthBridge(), 0.008);
		Dom.SetCalcExtRect();
		CDRect rect = Dom.GetExtRect();
		double dWidthh = rect.Width();
		double dHeight = rect.Height();
		double dPicRatio = dWidthh / dHeight;
		double dPicScale = dScale * 1.35;				
		CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");				
		
		pXL->AddDomWmfAuto(&Dom,szPath,10,nRow*19.5,dHeight*dPicScale*dPicRatio,dHeight*dPicScale);

		nRow += 4;

		pXL->SetXL(nRow++, 0, "�� ��� ó����");
		int nJigan = 0;
		double dValue, dMaxValue = 0.0;
		for (nNode = nIdxPrev; nNode <= nIdxNext; nNode++) 
		{
			CJoint *pJ = pManage->GetJoint(nNode);
			dValue = -frM(pJ->GetResultForce(nLoadCase)->m_Disp.z);
			dMaxValue = max(dValue, dMaxValue);
//			if (nNode != nIdxPrev && pManage->GetJoint(nNode)->IsRestraint())
			if(nNode != nIdxPrev && ModelCalc.IsRestraintJoint(nNode))
			{
				sprintf(sBuf, "��%d  =", nJigan + 1);
				pXL->SetXL(nRow, 1, sBuf);
				sprintf(sBuf, "%.3f mm", dMaxValue);
				pXL->SetXL(nRow, 2, sBuf);

				double dDa, dLength = toM(pDB->m_dLengthJigan[nJigan]);   // ���� �Ÿ�.
				char sBuf2[20];
				if (dLength > 40)   {
					sprintf (sBuf,  "��a = L/500 =");
					sprintf (sBuf2, "%.1f m  /  500 =", dLength);
					dDa = dLength / 500;
				}
				else if (dLength > 10)   {
					sprintf (sBuf,  "��a = L��/20000 =");
					sprintf (sBuf2, "%.1f�� m / 20000 =", dLength);
					dDa = dLength * dLength / 20000;
				}
				else   {
					sprintf (sBuf,  "��a = L /2000 =");
					sprintf (sBuf2, "%.1f m / 2000 =", dLength);
					dDa = dLength / 2000;
				}
				dDa *= 1000;
				pXL->SetMergeCell(pXL->GetCellStr(nRow, 4), pXL->GetCellStr(nRow, 5));
				pXL->SetMergeCell(pXL->GetCellStr(nRow, 6), pXL->GetCellStr(nRow, 7));
				pXL->SetXL(nRow, 4, sBuf);
				pXL->SetXL(nRow, 6, sBuf2);

				sprintf(sBuf, "%.2f mm", dDa);
				pXL->SetXL(nRow, 8, sBuf);
				if (dDa >= dMaxValue)   {
					pXL->SetXL(nRow, 3, "<");
					pXL->SetXL(nRow, 10, "O.K");
				}
				else   {
					pXL->SetXL(nRow, 3, ">");
					pXL->SetXL(nRow, 10, "N.G");
					pXL->SetFonts(nRow, 10, 9, "����ü", 3, FALSE);
				}
				pXL->SetHoriAlign(nRow, 1, nRow, 10, TA_CENTER);
				pXL->SetHoriAlign(nRow, 4, nRow, 7, TA_LEFT);
				dMaxValue = 0.0;
				nJigan ++;
				nRow ++;
			}
		}
		nRow += 3;
	}

}

void CXLCheckListUsing::ExpansionJoint()
{
	CDesignExpansionJoint	*pDesignExp	= m_pAPlateOutCalcStd->GetAPlateCalcStd()->m_pDesignExpanJoint;	
	CXLControl				*pXL		= m_pXL;
	CExpansionJointData		*pExpData	= NULL;	

	pDesignExp->SetExpansionJoint(0);
	pDesignExp->SetExpansionJoint(1);

	CString szText;
	// ����������ġ�� �԰ݰ���
	pExpData	= pDesignExp->GetExpansionJointData(0);
	szText		= pExpData->m_strName;							pXL->SetXL("C6", szText);
	szText.Format("%.3f", toM(pExpData->m_dLengthGirder[0]));	pXL->SetXL("E6", szText);
	szText.Format("%.3f", toM(pExpData->m_dLengthGirder[1]));	pXL->SetXL("I6", szText);
	szText.Format("%.3f", pExpData->m_dDlt);					pXL->SetXL("M6", szText);
	szText.Format("%.3f", pExpData->m_dDls);					pXL->SetXL("P6", szText);
	szText.Format("%.3f", pExpData->m_dDlc);					pXL->SetXL("S6", szText);
	szText.Format("%.3f", pExpData->m_dDlr);					pXL->SetXL("V6", szText);
	szText.Format("%.3f", pExpData->m_dEstablishExtra);			pXL->SetXL("Y6", szText);
	szText.Format("%.3f", pExpData->m_dAdditionalExtra);		pXL->SetXL("AB6", szText);
	szText.Format("%.3f", pExpData->m_dDli);					pXL->SetXL("AH6", szText);
	szText.Format("%.3f", pExpData->m_dEstablishLayingGap);		pXL->SetXL("AK6", szText);
	szText.Format("%.3f", pExpData->m_dMaxAllowExp);			pXL->SetXL("AN6", szText);

	pExpData	= pDesignExp->GetExpansionJointData(1);
	szText		= pExpData->m_strName;							pXL->SetXL("C7", szText);
	szText.Format("%.3f", toM(pExpData->m_dLengthGirder[0]));	pXL->SetXL("E7", szText);
	szText.Format("%.3f", toM(pExpData->m_dLengthGirder[1]));	pXL->SetXL("I7", szText);
	szText.Format("%.3f", pExpData->m_dDlt);					pXL->SetXL("M7", szText);
	szText.Format("%.3f", pExpData->m_dDls);					pXL->SetXL("P7", szText);
	szText.Format("%.3f", pExpData->m_dDlc);					pXL->SetXL("S7", szText);
	szText.Format("%.3f", pExpData->m_dDlr);					pXL->SetXL("V7", szText);
	szText.Format("%.3f", pExpData->m_dEstablishExtra);			pXL->SetXL("Y7", szText);
	szText.Format("%.3f", pExpData->m_dAdditionalExtra);		pXL->SetXL("AB7", szText);
	szText.Format("%.3f", pExpData->m_dDli);					pXL->SetXL("AH7", szText);
	szText.Format("%.3f", pExpData->m_dEstablishLayingGap);		pXL->SetXL("AK7", szText);
	szText.Format("%.3f", pExpData->m_dMaxAllowExp);			pXL->SetXL("AN7", szText);

	// ����������ġ �� ������ ������ ���� - ������� ����
	pExpData	= pDesignExp->GetExpansionJointData(0);
	szText.Format(pExpData->m_strExpanJoint);					pXL->SetXL("E14", szText);
	szText.Format("%.3f", pExpData->m_dLengthExtension);		pXL->SetXL("I14", szText);
	szText.Format("%.3f", pExpData->m_dMinLayingGap);			pXL->SetXL("M14", szText);
	szText.Format("%.3f", pExpData->m_dMaxLayingGap);			pXL->SetXL("S14", szText);
	szText.Format("%.3f", pExpData->m_dDli);					pXL->SetXL("AM14", szText);
	szText.Format("%.3f", pExpData->m_dLengthContraction);		pXL->SetXL("I20", szText);	// �����

	pExpData	= pDesignExp->GetExpansionJointData(1);
	szText.Format(pExpData->m_strExpanJoint);					pXL->SetXL("E15", szText);
	szText.Format("%.3f", pExpData->m_dLengthExtension);		pXL->SetXL("I15", szText);
	szText.Format("%.3f", pExpData->m_dMinLayingGap);			pXL->SetXL("M15", szText);
	szText.Format("%.3f", pExpData->m_dMaxLayingGap);			pXL->SetXL("S15", szText);
	szText.Format("%.3f", pExpData->m_dDli);					pXL->SetXL("AM15", szText);
	szText.Format("%.3f", pExpData->m_dLengthContraction);		pXL->SetXL("I21", szText);	// �����

	// �����ð� �ƴҶ�
	pExpData	= pDesignExp->GetExpansionJointData(0);
	if(pExpData->m_dDispEarthQuake[0]==0 && pExpData->m_dDispEarthQuake[1]==0)
	{
		pXL->SetXL("AH6", "-");
		pXL->SetXL("AI14", "-");
		pXL->SetXL("AM14", "-");
		pXL->SetXL("AI20", "-");
		pXL->SetXL("AM20", "-");
	}
	pExpData	= pDesignExp->GetExpansionJointData(1);
	if(pExpData->m_dDispEarthQuake[0]==0 && pExpData->m_dDispEarthQuake[1]==0)
	{
		pXL->SetXL("AH7", "-");	
		pXL->SetXL("AI15", "-");
		pXL->SetXL("AM15", "-");	
		pXL->SetXL("AI21", "-");
		pXL->SetXL("AM21", "-");
	}
}
