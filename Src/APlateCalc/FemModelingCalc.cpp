// FemModelingCalc.cpp: implementation of the CFemModelingCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFemModelingCalc::CFemModelingCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd) : m_pDataManage(pDataManage)
{
	m_pCalcStd = pCalcStd;
}

CFemModelingCalc::~CFemModelingCalc()
{

}

CDPoint CFemModelingCalc::GetStdAngle() 
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	long nStdGirder = -1;	// ���� �Ŵ�,Ȯ���� ���

	CPlateGirderApp *pGir    = pDB->GetGirder(nStdGirder);
	CLineInfo       *pLine   = pGir->GetLine();
	CDPoint			SttPoint = pLine->GetXyLineDis(pGir->GetStationStt(), 0);
	CDPoint			EndPoint = pLine->GetXyLineDis(pGir->GetStationEnd(), 0);

	return (EndPoint - SttPoint).Unit().GetMirrorVert();
}

/////////////////////////////////////////////////////////////////////////
// �������� ���� ����
//   ����(m) : x,y������ǥ��� z: �Ŵ�����÷����� ��ġ
// �ʱ⿡�� ���ܸ��� ����Ͽ� ������ ��ġ�� �����Ͽ����� ���ܸ����� ����
// ��Ұ� ���̴� ���� ������ ���� �Ŵ���鿡�� �����Ŵ������� ������
// ������ ������ġ�� ������
/////////////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeJoint(CFEMManage *pFemData)
{	
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

	CVector xyPos(0,0,0), gxy(0,0);
	long    nIdx   = 0;
	CDPoint SttAng = GetStdAngle();
	double  gz     = 0;
	double  dEle   = 0;	// �� ������ Z��

	// ���𵨸��� ������ �������(��� ������)
	// �Ŵ�����
	long nQtyGirder = pDB->GetGirdersu();
	for(long nG=0; nG<nQtyGirder; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		pGir->SetEleLevel(FALSE);	// ���ܱ��踦 �����
	
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);

		while(pBx)
		{
			if(!pBx->IsJoint())
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			double	dSta	= pBx->GetStation();
			xyPos   = pGir->GetXyGirderDis(dSta);					
			xyPos.x = toM(xyPos.x);
			xyPos.y = toM(xyPos.y);
			dEle    = toM(pGir->GetElevationFlange(dSta) + pGir->GetHeightFlangeUpperByTaper(dSta));

			xyPos = GetXyRotateVector(CDPoint(0,0), xyPos, SttAng);
			if(nIdx==0) { gxy = xyPos; gz = dEle; }  // ù��ǥ�� ���� ��ǥ�� ����
			xyPos -= gxy;
			dEle  -= gz;
		
			CVector vPos(xyPos.x, xyPos.y, dEle);
			CJoint	*pJ	= pFemData->AddJoint((unsigned short)nIdx, vPos);
			//JOINT�� ���¸� �ʱ�ȭ�Ѵ�.(������)
			pJ->m_UX = 0;
			pJ->m_UY = 0;
			pJ->m_UZ = 0;
			pJ->m_LongAttrib[JOINT_GIR]		= nG;
			pJ->m_vPointReal				= xyPos;
			pJ->m_vPointReal.x				= dSta;
			pJ->m_LongAttrib[JOINT_TYPE]	= FRAME_GIRDER_NODE;

			++nIdx;
			pBx = Finder.GetBxNext();
		}
		pGir->SetEleLevel(TRUE);
	}

	for(long nG=0; nG<nQtyGirder; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUM);
		while(pBx)
		{			
			long nJ = GetJointNoByBx(pBx);
			CJoint	*pJ = pFemData->GetJoint((unsigned short)nJ);
			CSection *pSec	= pBx->GetSection();

			long nJijum  = pGir->GetNumberJijumByBx(pBx);
			CDPoint xyGirCenter = pDB->GetAngleShoeOnGirCenter(nG, nJijum);
			double AngRotShoe = xyGirCenter.GetAngleDegree()-90;
	
			pJ->m_dLocalRotate = AngRotShoe;
			// �������� ����
			CShoe *pShoe = pGir->GetShoe(nJijum);
			BYTE  nDir   = pShoe->GetDirectionShoe();
			if(nDir==0)
			{
				pJ->m_UX = 1;
				pJ->m_UY = 1;
				pJ->m_UZ = 1;
			}
			else if(nDir==1)
			{
				pJ->m_UX = 0;
				pJ->m_UY = 0;
				pJ->m_UZ = 1;
			}
			else if(nDir==2)
			{
				pJ->m_UX = 1;
				pJ->m_UY = 0;
				pJ->m_UZ = 1;
			}
			else if(nDir==3)
			{
				pJ->m_UX = 0;
				pJ->m_UY = 1;
				pJ->m_UZ = 1;
			}
			pJ->m_LongAttrib[JOINT_TYPE] = FRAME_GIRDER_NODE | FRAME_SHOE_CENT_NODE;	

			pBx = Finder.GetBxNext();
		}
	}
}


/////////////////////////////////////////////////////////////////////////
/// ��Ҹ� ������ - ��� ������ ���� ����(�Ŵ�, ���κ�)
/////////////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeElement(CFEMManage *pFemData)
{	
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	
	long nEIdx     =0;
	long nJointIdx =0;	
	/////////////////////////////////////////////////////////////////////////
	///<�Ŵ� ���
	long nG = 0;
	for(nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
		while(Finder.GetBxFixNext())
		{	
			if(!pBx->IsJoint())
			{
				pBx = Finder.GetBxNext();
				continue;
			}

			CElement *pEle = pFemData->AddElement((unsigned short)nEIdx, (unsigned short)nJointIdx, (unsigned short)nJointIdx+1);
			pEle->m_LongAttrib[ELEMENT_TYPE]	= FRAME_GIRDER_ELEMENT;
			
			// TU���ӱ��϶��� �����۵��Կ� ����!
			if(pGir->IsShiftSection(pBx))	pEle->m_LongAttrib[ELEMENT_TU_POS_NEG]	= POSITIVE;
			else							pEle->m_LongAttrib[ELEMENT_TU_POS_NEG]	= NEGATIVE;

			if(m_pCalcStd->GetBxByJoint(pFemData->GetJoint((unsigned short)nJointIdx))->IsJijum())
				pEle->m_LongAttrib[ELEMENT_TYPE] = FRAME_GIRDER_ELEMENT | FRAME_SHOE_ELEMENT;

			if(m_pCalcStd->GetBxByJoint(pFemData->GetJoint((unsigned short)nJointIdx+1))->IsDanbuJijumEnd())
				pEle->m_LongAttrib[ELEMENT_TYPE] = FRAME_GIRDER_ELEMENT | FRAME_SHOE_ELEMENT;

			nEIdx++;
			nJointIdx++;
			pBx = Finder.GetBxNext();
		}
		nJointIdx++;
	}
	/////////////////////////////////////////////////////////////////////////
	///< ���κ� ���
	long nJ=0, nJMatch=0;
	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxMatch;
		while(pBx)
		{			
			nJ       = GetJointNoByBx(pBx);
			pBxMatch = pGir->GetBxMatchByCrossBeam(pBx,pGir->m_pRight);

			if(pBxMatch)
			{
				if(pBx->IsJijum() || pDB->IsSafeDiapByCrossBeam(nG*2+3,pBx->GetMatchCol(), TRUE))
				{
					nJMatch = GetJointNoByBx(pBxMatch);
					CElement *pEle = pFemData->AddElement((unsigned short)nEIdx++,(unsigned short)nJ, (unsigned short)nJMatch);
					pEle->m_LongAttrib[ELEMENT_TYPE] = FRAME_CROSS_ELEMENT;
				}
			}			
			pBx = Finder.GetBxNext();
		}				
	}	
	/////////////////////////////////////////////////////////////////////////
	///<���𵨸� ���
//	nJointIdx=0;
//	long nTotalJointGir = 0;
//	for(nJ=0; nJ<pFemData->GetJointSize(); nJ++)
//	{
//		if(pFemData->GetJoint(nJ)->m_nMain == FRAME_GIRDER_NODE)
//			nTotalJointGir++;
//	}
//
//	for(nG=0; nG<pDB->GetGirdersu(); nG++)
//	{
//		CPlateGirderApp *pGir = pDB->GetGirder(nG);
//		CPlateBxFinder Finder(pGir);
//		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_JIJUM);
//		while(pBx)
//		{			
//			nJ = GetJointNoByBx(pBx);
//			CElement *pEle = pFemData->AddElement(nEIdx++, nJ, nTotalJointGir+nJointIdx);
//			pEle->m_bMain = FRAME_SHOE_ELEMENT;
//			
//			pBx = Finder.GetBxNext();
//			nJointIdx++;
//		}
//	}

}
/////////////////////////////////////////////////////////////////////////
///������ ����(�𵨸�)
///
/////////////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeMaterial(CFEMManage *pFemData)
{
	CCalcData	*pCalcData	= m_pDataManage->GetCalcData();
	double	dEst		= tokNPM2(pCalcData->DESIGN_MATERIAL.m_dEst);
	double	dEc			= tokNPM2(pCalcData->DESIGN_MATERIAL.m_dEc);
	double	dDummyEst	= tokNPM2(pCalcData->DESIGN_MATERIAL.m_dDummyEst);
	pFemData->AddMaterial("STEEL", dEst, 0.0, 'C', 0, 0, 0, 0);
	pFemData->AddMaterial("CONC", dEc, 0.0, 'C', 0, 0, 0, 0);
	pFemData->AddMaterial("SHOE", dDummyEst, 0.0, 'C', 0, 0, 0, 0);
}

void CFemModelingCalc::GetGirderFrameSectionArray(vector <CSectionJewon> *pJewonArray, long nAnalysisType)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CFemStressCalc StressCalc(m_pDataManage, m_pCalcStd);

	CSectionJewon vSec;

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	
		while(pBx)
		{
			double	dSta	= pBx->GetStation();
			GetSectionCoefficient(nG, dSta, FALSE, dSta, 0);
			vSec.m_nType	= FRAME_GIRDER_ELEMENT;
			vSec.m_pBx		= pBx;
			vSec.m_dA		= GetSectionCoeffAs(nAnalysisType);
			vSec.m_dI33		= GetSectionCoeffI33(nAnalysisType, pBx);
			vSec.m_dI22		= GetSectionCoeffI22(nAnalysisType);
			vSec.m_dJ		= GetSectionCoeffK(nAnalysisType);
	
			vSec.m_dBu		= m_MapSecCoe["FLANGE_W_UP"];
			vSec.m_dTu		= m_MapSecCoe["FLANGE_T_UP"];
			vSec.m_dBl		= m_MapSecCoe["FLANGE_W_DN"];
			vSec.m_dTl		= m_MapSecCoe["FLANGE_T_DN"];
			vSec.m_dH		= m_MapSecCoe["WEB_H"];
			vSec.m_dTw		= m_MapSecCoe["WEB_T"];

			pJewonArray->push_back(vSec);

			pBx = Finder.GetBxNext();
		}
	}

	// �ӵ������� ���� �Ʒ� �ּ��ڵ忡�� �����ڵ�� �����Ͽ���. - #15918 (17�氣) ����
	vector <CSectionJewon>::iterator iter;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++) iter->m_bNeedDelete = FALSE;

	CDoubleArray dArrI33;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++)	dArrI33.Add(toM4(iter->m_dI33));
	dArrI33.RemoveSameValue(0.0000001);

	for(long n=0; n<dArrI33.GetSize(); n++)
	{
		BOOL bFirst = TRUE;
		for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++)
		{
			if(IsSameI33(frM4(dArrI33[n]), iter->m_dI33))
			{
				iter->m_bNeedDelete = !bFirst;
				bFirst = FALSE;
			}
		}
		pJewonArray->erase(remove_if(pJewonArray->begin(), pJewonArray->end(), IsDelete), pJewonArray->end());		
	}
	
/*	
	vector <CSectionJewon>::iterator iter, iterComp;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++) iter->m_bNeedDelete = FALSE;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++)
	{
		long nByunStt = StressCalc.GetByunDanStyle(iter->m_pBx);
		for(iterComp=iter+1; iterComp!=pJewonArray->end(); iterComp++)
		{
			long nByunEnd = StressCalc.GetByunDanStyle(iterComp->m_pBx);
			if(IsSameI33(iter->m_dI33, iterComp->m_dI33))
				iterComp->m_bNeedDelete = TRUE;
		}
		pJewonArray->erase(remove_if(pJewonArray->begin(), pJewonArray->end(), IsDelete), pJewonArray->end());
	}
	pJewonArray->erase(remove_if(pJewonArray->begin(), pJewonArray->end(), IsDelete), pJewonArray->end());
*/}

void CFemModelingCalc::GetCrossBeamFrameSectionArray(vector <CSectionJewon> *pJewonArray)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();

	CSectionJewon vSec;

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);

		while(pBx)
		{	
			GetSectionCoefficientCR(pBx);

			vSec.m_nType	= FRAME_CROSS_ELEMENT;
			vSec.m_pBx		= pBx;			
			vSec.m_dA		= m_MapSecCoeCR["Sum_As"];
			vSec.m_dI33		= m_MapSecCoeCR["I33"];
			vSec.m_dI22		= m_MapSecCoeCR["I22"];
			vSec.m_dJ		= m_MapSecCoeCR["K"];
			vSec.m_dBu		= m_MapSecCoeCR["up_l"];
			vSec.m_dTu		= m_MapSecCoeCR["up_t"];
			vSec.m_dBl		= m_MapSecCoeCR["lo_l"];
			vSec.m_dTl		= m_MapSecCoeCR["lo_t"];
			vSec.m_dH		= m_MapSecCoeCR["w_l"];
			vSec.m_dTw		= m_MapSecCoeCR["w_t"];
			
			pJewonArray->push_back(vSec);
			
			pBx = Finder.GetBxNext();
		}
	}

	// �ӵ������� ���� �Ʒ� �ּ��ڵ忡�� �����ڵ�� �����Ͽ���. - #15918 (17�氣) ����
	vector <CSectionJewon>::iterator iter;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++) iter->m_bNeedDelete = FALSE;

	CDoubleArray dArrI33;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++)	dArrI33.Add(toM4(iter->m_dI33));
	dArrI33.RemoveSameValue(0.0000001);
	for(long n=0; n<dArrI33.GetSize(); n++)
	{
		BOOL bFirst = TRUE;
		for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++)
		{
			if(IsSameI33(frM4(dArrI33[n]), iter->m_dI33))
			{
				iter->m_bNeedDelete = !bFirst;
				bFirst = FALSE;
			}
		}
		pJewonArray->erase(remove_if(pJewonArray->begin(), pJewonArray->end(), IsDelete), pJewonArray->end());		
	}

/*	
	vector <CSectionJewon>::iterator iter, iterComp;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++) iter->m_bNeedDelete = FALSE;
	for(iter=pJewonArray->begin(); iter!=pJewonArray->end(); iter++)
	{
		for(iterComp=iter+1; iterComp!=pJewonArray->end(); iterComp++)
		{
			if(IsSameI33(iter->m_dI33, iterComp->m_dI33))
				iterComp->m_bNeedDelete = TRUE;
		}
		pJewonArray->erase(remove_if(pJewonArray->begin(), pJewonArray->end(), IsDelete), pJewonArray->end());
	}
	pJewonArray->erase(remove_if(pJewonArray->begin(), pJewonArray->end(), IsDelete), pJewonArray->end());
*/}

/////////////////////////////////////////////////////////////////////////
/// �ܸ�������
///  �ܸ����� ��ҿ� �����ϴ� ���� MakeElementDefineSection()���� ��.
/////////////////////////////////////////////////////////////////////////
long CFemModelingCalc::MakeFrameSection(CFEMManage *pFemData, long nAnalysisType)
{
	CPlateBasicIndex	*pBx	= NULL;
	CCalcGeneral CalcGen(m_pDataManage);

	long	nSection	= 1;
	CString	szSec		= _T("");
	CString	szSecName	= _T("");
	vector <CSectionJewon> arrSecGir;
	vector <CSectionJewon> arrSecCr;
	GetGirderFrameSectionArray(&arrSecGir, nAnalysisType);
	GetCrossBeamFrameSectionArray(&arrSecCr);

	pFemData->m_FrameSectionMap.RemoveAll();
	pFemData->m_FrameSectionList.RemoveAll();

	if(nAnalysisType==BEFORE_COMPOSITE_ANALYSIS)			szSecName = _T("����Ŵ� �ܸ�");
	else if(nAnalysisType==BEFORE_TURN_COMPOSITE_ANALYSIS)	szSecName = _T("������ũ��Ʈ �ռ� �ܸ�");
	else if(nAnalysisType==AFTER_COMPOSITE_ANALYSIS)		szSecName = _T("�Ŵ� �ܸ�");

	long	nSizeGir	= arrSecGir.size();
	long	nSizeCr		= arrSecCr.size();
	CString szUp, szLo, szWeb, szTemp;
	long nSec = 0;
	for(nSec = 0; nSec < nSizeGir; nSec++)
	{
		szSec.Format("%d", nSection);
		CSectionJewon	*pSecJewon	= &arrSecGir[nSec];
		CFrameSection	*pFrmSec = pFemData->AddFrameSection(szSec, "STEEL", toM2(pSecJewon->m_dA), toM4(pSecJewon->m_dI33), toM4(pSecJewon->m_dI22), toM4(pSecJewon->m_dJ), 0, szSecName, POS_GIRDER);
		pFrmSec->m_dJewon[SEC_BU]	= toM(pSecJewon->m_dBu);
		pFrmSec->m_dJewon[SEC_TU]	= toM(pSecJewon->m_dTu);
		pFrmSec->m_dJewon[SEC_BL]	= toM(pSecJewon->m_dBl);
		pFrmSec->m_dJewon[SEC_TL]	= toM(pSecJewon->m_dTl);
		pFrmSec->m_dJewon[SEC_HW]	= toM(pSecJewon->m_dH);
		pFrmSec->m_dJewon[SEC_TW]	= toM(pSecJewon->m_dTw);
		pBx = arrSecGir.at(nSec).m_pBx;
		CalcGen.GetMaterial(pBx, G_F_U, szUp, szTemp);
		CalcGen.GetMaterial(pBx, G_F_L, szLo, szTemp);
		CalcGen.GetMaterial(pBx, G_W,  szWeb, szTemp);
		pFrmSec->m_szJewon[FRM_UPPER]	= szUp;
		pFrmSec->m_szJewon[FRM_LOWER]	= szLo;
		pFrmSec->m_szJewon[FRM_WEB]		= szWeb;
		nSection++;
	}

	for(nSec = 0; nSec < nSizeCr; nSec++)
	{
		szSec.Format("%d", nSection);
		CSectionJewon	*pSecJewon	= &arrSecCr[nSec];
		CFrameSection	*pFrmSec = pFemData->AddFrameSection(szSec, "STEEL", toM2(pSecJewon->m_dA), toM4(pSecJewon->m_dI33), toM4(pSecJewon->m_dI22), toM4(pSecJewon->m_dJ), 0, "���κ� �ܸ�", POS_CROSSBEAM);
		pFrmSec->m_dJewon[SEC_BU]	= toM(pSecJewon->m_dBu);
		pFrmSec->m_dJewon[SEC_TU]	= toM(pSecJewon->m_dTu);
		pFrmSec->m_dJewon[SEC_BL]	= toM(pSecJewon->m_dBl);
		pFrmSec->m_dJewon[SEC_TL]	= toM(pSecJewon->m_dTl);
		pFrmSec->m_dJewon[SEC_HW]	= toM(pSecJewon->m_dH);
		pFrmSec->m_dJewon[SEC_TW]	= toM(pSecJewon->m_dTw);
		pBx = pSecJewon->m_pBx;
		CalcGen.GetMaterial(pBx, G_F_U, szUp, szTemp);
		CalcGen.GetMaterial(pBx, G_F_L, szLo, szTemp);
		CalcGen.GetMaterial(pBx, G_W,  szWeb, szTemp);
		pFrmSec->m_szJewon[FRM_UPPER]	= szUp;
		pFrmSec->m_szJewon[FRM_LOWER]	= szLo;
		pFrmSec->m_szJewon[FRM_WEB]		= szWeb;
		nSection++;
	}

//	szSec.Format("%d", nSection);
//	pFemData->AddFrameSection(szSec, "SHOE", 10.0, 10.0, 10.0, 10.0, 0, "���� �ܸ�", POS_SHOE);
//	nSection++;

	return nSection;
}

/////////////////////////////////////////////////////////////////////////
/// * �ܸ����� ��ҿ� ����(3�ܰ�)
///		1 �ܰ� : ����� ��.������ġ�� �ܸ��� ������ ���
///              ��ҿ� �ܸ��� ������ 
///     2 �ܰ� : �Ŵ���Ҹ� ���������� �˻��ϸ鼭 ��.������ġ �ܸ���
///              ������ ��� ��ȭ�����ܸ��߰���
///     3 �ܰ� : ����� ��.������ġ�� �ܸ��� ������ ���
///              ��ҿ� �ܸ��� ������
/////////////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeElementDefineSection(CFEMManage *pFemData, long nSection, long nAnalysisType)
{
	///********************************************************
	///<1�ܰ�(�Ŵ� �� ��ҳ� �ܸ��� ������ ���, ���κ����)
	///********************************************************
	long nCnt = 0;
	double dSttJointI33=0, dEndJointI33=0, dElmI33=0;

	CFemStressCalc	StressCalc(m_pDataManage, m_pCalcStd);
	CPlateGirderApp	*pGir = m_pCalcStd->m_pDataManage->GetBridge()->GetGirder(1);
	CPlateBxFinder	Finder(pGir);

	POSITION pos = pFemData->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pElm  = (CElement*)pFemData->m_ElementList.GetNext(pos);
		CJoint   *pJStt = pFemData->GetJoint(pElm->m_nJStt);
		CJoint   *pJEnd = pFemData->GetJoint(pElm->m_nJEnd);
		CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pJStt);
		CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pJEnd);
		long	nG		= pBxStt->GetNumberGirder();
		double	dStaStt	= pBxStt->GetStation();
		double	dStaEnd	= pBxEnd->GetStation();

		nCnt ++;

		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)///<�Ŵ��� ���
		{
			GetSectionCoefficient(nG, dStaStt, FALSE, dStaStt, 1);
			dSttJointI33 = GetSectionCoeffI33(nAnalysisType, pBxStt);
			GetSectionCoefficient(nG, dStaEnd, FALSE, dStaEnd, -1);
			dEndJointI33 = GetSectionCoeffI33(nAnalysisType, pBxEnd, FALSE);

			if(IsSameI33(dSttJointI33, dEndJointI33))
			{
				CFrameSection *pFrameSec = GetFrameSectionByI33(pFemData, dSttJointI33);
				if(pFrameSec)
					pElm->m_szFrameSectionName = pFrameSec->m_szName;
			}
			else ///<�ܸ��� ���� �ٸ� ���
				continue;
		}	
		else if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_SHOE_ELEMENT)///< �� 
		{			
			CFrameSection *pFrameSec = (CFrameSection*)pFemData->m_FrameSectionList.GetTail();
			pElm->m_szFrameSectionName = pFrameSec->m_szName;
		}
		else if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)///< ���κ�
		{
			GetSectionCoefficientCR(pBxStt);
			dElmI33 = m_MapSecCoeCR["I33"];
			CFrameSection *pFrameSec = GetFrameSectionByI33(pFemData, dElmI33);
			pElm->m_szFrameSectionName = pFrameSec->m_szName;
		}
	}
	///********************************************************
	///<2�ܰ�
	///<�Ŵ���Ҹ� �� �f��鼭 ��Ұ� ��ȭ�Ǵ� ������ �ܸ��� �߰��Ѵ�.
	///********************************************************
	CStringArray arrSec;///<4�ڸ��� : ��) 0507 [���۴ܸ�5.�����ܸ�7]
	CString szTotal = "";
	POSITION posFr = pFemData->m_ElementList.GetHeadPosition();
	while(posFr)
	{
		CElement *pElm  = (CElement*)pFemData->m_ElementList.GetNext(posFr);
		CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pElm->m_nJStt));
		CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pElm->m_nJEnd));
		long	nG		= pBxStt->GetNumberGirder();
		double	dStaStt	= pBxStt->GetStation();
		double	dStaEnd	= pBxEnd->GetStation();

		GetSectionCoefficient(nG, dStaStt, FALSE, dStaStt, 1);
		dSttJointI33 = GetSectionCoeffI33(nAnalysisType, pBxStt);
		GetSectionCoefficient(nG, dStaEnd, FALSE, dStaEnd, -1);
		dEndJointI33 = GetSectionCoeffI33(nAnalysisType, pBxEnd, FALSE);
		
		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			if(IsSameI33(dSttJointI33, dEndJointI33))
				continue;///<�ܸ��� ������ ���

			CString szSectionStt = GetSectionNameByBx(pFemData, pBxStt, nAnalysisType);
			CString szSectionEnd = GetSectionNameByBx(pFemData, pBxEnd, nAnalysisType);
			szTotal.Format("%2d%2d", atol(szSectionStt), atol(szSectionEnd));
			if(atol(szSectionStt)<10 && atol(szSectionEnd)<10)
				szTotal.Format("0%d0%d", atol(szSectionStt), atol(szSectionEnd));
			else if(atol(szSectionStt)>=10 && atol(szSectionEnd)< 10)
				szTotal.Format("%2d0%d", atol(szSectionStt), atol(szSectionEnd));
			else if(atol(szSectionStt)< 10 && atol(szSectionEnd)>=10)
				szTotal.Format("0%d%2d", atol(szSectionStt), atol(szSectionEnd));

			arrSec.Add(szTotal);
		}
	}
	CStringArray arrSecLast;///<�ߺ��Ǵ� ���ڿ��� �����ϰ� �ٽ� ����
	long n = 0;
	for(n=0; n<arrSec.GetSize(); n++)///<������ �������鼭 �߸�� ���ڴ� ������
	{
		CString szMain = arrSec.GetAt(n);
		long nEqual=0;
		for(long a=n; a<arrSec.GetSize(); a++)
		{
			if(szMain==arrSec.GetAt(a))
				nEqual++;
		}
		if(nEqual==1)
			arrSecLast.Add(arrSec.GetAt(n));
	}
	for(n=0; n<arrSecLast.GetSize(); n++)///<arrSecLast�� �̿��Ͽ� ���ܸ鱸���� ������.
	{
		CString szSecChange = arrSecLast.GetAt(n);
		CString szStt = szSecChange.Left(2);  ///<"01"
		CString szEnd = szSecChange.Right(2); ///<"02"
		///<�ܸ��̸��� arrSecLast�� ���ڿ��� �ٷ� �����
		pFemData->AddFrameSectionOfTaper(Comma(nSection), Comma(atol(szStt)), Comma(atol(szEnd)), 2, 1, "��ȭ �ܸ�", POS_EIVAR);
		nSection++;
	}
	///********************************************************
	///<3�ܰ� : ����� ��.������ġ�� �ܸ��� ������ ���
	///********************************************************
	pos = pFemData->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pElm  = (CElement*)pFemData->m_ElementList.GetNext(pos);
		
		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pElm->m_nJStt));
			CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pElm->m_nJEnd));

			long	nG	= pBxStt->GetNumberGirder();
			double	dStaStt	= pBxStt->GetStation();
			double	dStaEnd	= pBxEnd->GetStation();

			GetSectionCoefficient(nG, dStaStt, FALSE, dStaStt, 1);
			dSttJointI33 = GetSectionCoeffI33(nAnalysisType, pBxStt);
			GetSectionCoefficient(nG, dStaEnd, FALSE, dStaEnd, -1);
			dEndJointI33 = GetSectionCoeffI33(nAnalysisType, pBxEnd, FALSE);

			CString szSec1="", szSec2="";

			if(IsSameI33(dSttJointI33, dEndJointI33))
				continue;///<���ϴܸ��� ���� ������
				
			CFrameSection *pFrameSecStt = GetFrameSectionByI33(pFemData, dSttJointI33);
			CFrameSection *pFrameSecEnd = GetFrameSectionByI33(pFemData, dEndJointI33);

			if(pFrameSecStt==NULL || pFrameSecEnd==NULL) continue;

			szSec1 = pFrameSecStt->m_szName;///<���ۺ��� I33�̿��Ͽ� �ܸ��� ���ؼ� ������
			szSec2 = pFrameSecEnd->m_szName;///<�������� I33�̿��Ͽ� �ܸ��� ���ؼ� ������

			POSITION pos = pFemData->m_FrameSectionList.GetHeadPosition();
			while(pos)
			{
				CFrameSection *pFrameSec =  (CFrameSection*)pFemData->m_FrameSectionList.GetNext(pos);  
				///<����ܸ��� ���۴ܸ�� �����ܸ��� szSec1, szSec2�� ������ ��ҿ� �ܸ��� ������
				if(szSec1==pFrameSec->m_szSecStt && szSec2==pFrameSec->m_szSecEnd)
				{
					pElm->m_szFrameSectionName = pFrameSec->m_szName;
					break;
				}
			}		
		}
	}
}

void CFemModelingCalc::MakeForcePreDead_Turn(CFEMManage *pFemData, long nAnalysisType, long &nLoadCase)
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CCalcData		*pData		= m_pDataManage->GetCalcData();
	CFEMManage		*pManageHLL	= NULL;

	double	dGangjaeUnitWeight	= tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dGangjae);
	CVector vForce(0,0,0);

	double  dCrAs=0, dGirAsPre=0, dGirAsNext=0;
	double	dLoad	= 0;
	double	dLoad1	= 0;
	double	dLoad2	= 0;

	// ���ӱ��̸� �θ��Ʈ�� ���� �ֱ�
	pFemData->MakeElementLoadForce(nLoadCase+1);
	pFemData->m_szLCName.SetAt(nLoadCase, "CONSTEEL");
	pFemData->m_szLCComment.SetAt(nLoadCase, "�ռ��� ������(������ũ��Ʈ ����)");
	long nEle = 0;
	for(nEle = 0; nEle<pFemData->GetElementSize(); nEle++)
	{
		CElement	*pEle	= pFemData->GetElement((unsigned short)nEle);

		if(pEle->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)	break;
		if(pDB->IsTUContinuousSpan() && pEle->m_LongAttrib[ELEMENT_TU_POS_NEG]==NEGATIVE)	continue;
		
		long	nG = GetGirderNoByElemID(nEle);
		long	nHLL = GetHLLNoByElemID(pEle->m_nIndex);
		pManageHLL = m_pCalcStd->GetHLLFemData(nHLL);
		
		vForce     = pManageHLL->GetJoint((unsigned short)nG+1)->GetResultForce(1)->m_Force;
		CVector vStt = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
		CVector vEnd = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
		double	dRate = 1.0;

		CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);	
		if(pDB->IsTUContinuousSpan())
			pLoad->SetLoadForce(0, dRate, CVector(0,0, -vForce.z), CVector(0,0, -vForce.z), CVector(0,0,0), CVector(0,0,0), "TU�Ŵ�", CVector(0,0,0), TRUE); 
		else
			pLoad->SetLoadForce(0, dRate, CVector(0,0, vForce.z), CVector(0,0, vForce.z), CVector(0,0,0), CVector(0,0,0), pDB->IsTUGir()?"TU�Ŵ�":"�Ŵ�", CVector(0,0,0), TRUE); 
	}
	++nLoadCase;
	///<  1. �������� - �ܼ����϶��� 180 �ʹ�
	if(!pDB->IsTUContinuousSpan())
	{
		pFemData->MakeElementLoadForce(nLoadCase+1);
		pFemData->m_szLCName.SetAt(nLoadCase, "TURNING");
		pFemData->m_szLCComment.SetAt(nLoadCase, "180�� TURNING (����Ŵ� ���� + ������ũ��Ʈ ����)");	
		long nEle = 0;
		for(nEle = 0; nEle<pFemData->GetElementSize(); nEle++)
		{
			CElement	*pEle	= pFemData->GetElement((unsigned short)nEle);
			if(pEle->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)	break;
			CPlateBasicIndex *pBx	= m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJStt));
			CPlateBasicIndex *pBxN	= m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJEnd));
			
			long	nG = GetGirderNoByElemID(nEle);
			long	nHLL = GetHLLNoByElemID(pEle->m_nIndex);
			pManageHLL = m_pCalcStd->GetHLLFemData(nHLL);
			
			CVector  vStt  = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
			CVector  vEnd  = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
			double	 dRate = 1.0;
			dLoad1	= pManageHLL->GetJoint((unsigned short)nG+1)->GetResultForce(1)->m_Force.z;
			
			GetSectionCoefficient(pBx);
			dGirAsPre = GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS);
			
			GetSectionCoefficient(pBxN);
			dGirAsNext = GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS);
			
			dLoad2	= toM2((dGirAsPre+dGirAsNext)/2) * dGangjaeUnitWeight * pData->m_BonusRate;
			dLoad	= (dLoad1 + dLoad2) * 2;

			CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);
			pLoad->SetLoadForce(0, dRate, CVector(0,0,-dLoad), CVector(0,0,-dLoad), CVector(0,0,0), CVector(0,0,0), "TU�Ŵ�", CVector(0,0,0), TRUE);
		}
		++nLoadCase;
	}

	pFemData->MakeElementLoadForce(nLoadCase+1);
	pFemData->m_szLCName.SetAt(nLoadCase, "CONCRETE");
	pFemData->m_szLCComment.SetAt(nLoadCase, "�ռ��� ������(������ ����)");

	for(nEle = 0; nEle<pFemData->GetElementSize(); nEle++)
	{
		CElement	*pEle	= pFemData->GetElement((unsigned short)nEle);
		if(pEle->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)	break;
		
		long	nG = GetGirderNoByElemID(nEle);
		long  nHLL = GetHLLNoByElemID(pEle->m_nIndex);
		pManageHLL = m_pCalcStd->GetHLLFemData(nHLL);
		
		vForce     = pManageHLL->GetJoint((unsigned short)nG+1)->GetResultForce(0)->m_Force;
		CVector vStt = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
		CVector vEnd = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
		double  dRate= 1.0;
		
		CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);		
		pLoad->SetLoadForce(0, dRate, CVector(0,0,-vForce.z), CVector(0,0,-vForce.z), CVector(0,0,0), CVector(0,0,0), "������", CVector(0,0,0), TRUE); 
	}

	for(long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		///<      2) ���κ� �� �����극�̽�
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{	
			CElement *pEle = pFemData->GetElement((unsigned short)GetElemByCrossBx(pBx));
			if(pEle)
			{
				CVector vStt   = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
				CVector vEnd   = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
				GetSectionCoefficientCR(pBx);
				dCrAs	= m_MapSecCoeCR["Sum_As"];
				dLoad	= toM2(dCrAs) * dGangjaeUnitWeight * pData->m_BonusRate;

				CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);
				double dRate = 1.0;
				pLoad->SetLoadForce(0, dRate, CVector(0,0,-dLoad), CVector(0,0,-dLoad), CVector(0,0,0), CVector(0,0,0), "���κ�&�����극�̽�", CVector(0,0,0), TRUE); 
			}

			pBx = Finder.GetBxNext();
		}
	}
}

/**	\brief	The CFemModelingCalc::MakeForcePreDead function
	�ռ� �� ������ ������(DeadPre.s2k)	
*/
void CFemModelingCalc::MakeForcePreDead(CFEMManage *pFemData, long nAnalysisType, long &nLoadCase)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CCalcData       *pData	= m_pDataManage->GetCalcData();

	double dGangjaeUnitWeight = tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dGangjae);
	///<  1. ��������
	pFemData->MakeElementLoadForce(nLoadCase+1);
	pFemData->m_szLCName.SetAt(nLoadCase, "STEEL");	
	pFemData->m_szLCComment.SetAt(nLoadCase, "�ռ��� ������(���� ����)");

	double dSelfWeight  = 0;
	double dGirAs=0, dCrAs=0, dGirAsPre=0, dGirAsNext=0;
	
	///<      1) �Ŵ�
	for(long nElm=0; nElm<pFemData->GetElementSize(); nElm++)
	{
		CElement *pEle = pFemData->GetElement((unsigned short)nElm);
		if(pEle->m_LongAttrib[ELEMENT_TYPE] == FRAME_CROSS_ELEMENT)	break;

		CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJStt));
		CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJEnd));
		CVector  vStt  = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
		CVector  vEnd  = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
		
		CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);
		double		dRate = 1.0;		
		
		GetSectionCoefficient(pBxStt);
		dGirAsPre = GetSectionCoeffAs(nAnalysisType);
		
		GetSectionCoefficient(pBxEnd);
		dGirAsNext = GetSectionCoeffAs(nAnalysisType);
		
		dGirAs      = (dGirAsPre+dGirAsNext)/2;
		dSelfWeight = toM2(dGirAs)*dGangjaeUnitWeight*pData->m_BonusRate;
		if(pDB->IsTUGir() && !pDB->IsTUContinuousSpan())
			pLoad->SetLoadForce(0, dRate, CVector(0, 0, dSelfWeight), CVector(0, 0, dSelfWeight), CVector(0,0,0), CVector(0,0,0), "TU�Ŵ�", CVector(0,0,0), TRUE); 
		else
			pLoad->SetLoadForce(0, dRate, CVector(0, 0, -dSelfWeight), CVector(0, 0, -dSelfWeight), CVector(0,0,0), CVector(0,0,0), pDB->IsTUGir()?"TU�Ŵ�":"�Ŵ�", CVector(0,0,0), TRUE);
	}

	if(!pDB->IsTUGir())
	{
		long nElm = 0;
		for(nElm=0; nElm<pFemData->GetElementSize(); nElm++)
		{
			CElement *pEle = pFemData->GetElement((unsigned short)nElm);
			if(pEle->m_LongAttrib[ELEMENT_TYPE] != FRAME_CROSS_ELEMENT)	continue;

			CPlateBasicIndex *pBx = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJStt));
			CVector vStt   = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
			CVector vEnd   = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
			double	dRate  = 1.0;
			GetSectionCoefficientCR(pBx);
			dCrAs       = m_MapSecCoeCR["Sum_As"];
			dSelfWeight = toM2(dCrAs) * dGangjaeUnitWeight * pData->m_BonusRate;
			
			CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);
			pLoad->SetLoadForce(0, dRate, CVector(0,0,-dSelfWeight), CVector(0,0,-dSelfWeight), CVector(0,0,0), CVector(0,0,0), "���κ�&�����극�̽�", CVector(0,0,0), TRUE); 
		}

		++nLoadCase;
		pFemData->MakeElementLoadForce(nLoadCase+1);
		pFemData->m_szLCName.SetAt(nLoadCase, "CONCRETE");
		pFemData->m_szLCComment.SetAt(nLoadCase, "�ռ��� ������(������ ����)");
		
		CFEMManage *pHLL_Stt;
		for(nElm=0; nElm<pFemData->GetElementSize(); nElm++)
		{
			CElement *pEle = pFemData->GetElement((unsigned short)nElm);
			if(pEle->m_LongAttrib[ELEMENT_TYPE] == FRAME_CROSS_ELEMENT)	break;
			
			CPlateBasicIndex *pBxStt = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJStt));
			CPlateBasicIndex *pBxEnd = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJEnd));

			if(m_pCalcStd->GetSizeHLLFemData() == 0 )	return;

			long  nHLL_Stt = GetHLLNoByElemID(pEle->m_nIndex, TRUE);

			pHLL_Stt = m_pCalcStd->GetHLLFemData(nHLL_Stt);
			
			unsigned short usNode = (unsigned short)pBxStt->GetNumberGirder()+1;
			CVector vForceStt = pHLL_Stt->GetJoint(usNode)->GetResultForce(0)->m_Force;
			
			double	dRate	= 1.0;
			if(IsTaperElement(nElm, dRate))
			{
				long  nHLL_End = GetHLLNoByElemID(pEle->m_nIndex, FALSE);
				CFEMManage *pHLL_End = m_pCalcStd->GetHLLFemData(nHLL_End);
				CVector vForceEnd = pHLL_End->GetJoint(usNode)->GetResultForce(0)->m_Force;

				CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);
				pLoad->SetLoadForce(0.0, dRate, CVector(0,0,-vForceStt.z), CVector(0,0,-vForceStt.z), CVector(0,0,0), CVector(0,0,0), "������", CVector(0,0,0), TRUE); 
				pLoad->SetLoadForce(dRate, 1.0, CVector(0,0,-vForceEnd.z), CVector(0,0,-vForceEnd.z), CVector(0,0,0), CVector(0,0,0), "������", CVector(0,0,0), TRUE); 
			}
			else
			{
				CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);
				pLoad->SetLoadForce(0, dRate, CVector(0,0,-vForceStt.z), CVector(0,0,-vForceStt.z), CVector(0,0,0), CVector(0,0,0), "������", CVector(0,0,0), TRUE); 
			}
		}
	}
	else if(pDB->IsTUContinuousSpan())	// TU���ӱ��϶�
	{
		++nLoadCase;
		// ���ӱ��̰� �����Ʈ ������ ���� �ֱ�
		pFemData->MakeElementLoadForce(nLoadCase+1);
		pFemData->m_szLCName.SetAt(nLoadCase, "CONSTEEL");
		pFemData->m_szLCComment.SetAt(nLoadCase, "�ռ��� ������(�����Ʈ ������ũ��Ʈ ����)");
		
		CFEMManage *pManageHLL = NULL;
		CVector vForce(0,0,0);

		for(long nElm=0; nElm<pFemData->GetElementSize(); nElm++)
		{
			CElement *pEle = pFemData->GetElement((unsigned short)nElm);			
			if(pEle->m_LongAttrib[ELEMENT_TYPE] == FRAME_CROSS_ELEMENT)	break;
			if(pEle->m_LongAttrib[ELEMENT_TU_POS_NEG]==POSITIVE)		continue;
			
			CPlateBasicIndex *pBx = m_pCalcStd->GetBxByJoint(pFemData->GetJoint(pEle->m_nJStt));
			long  nHLL = GetHLLNoByElemID(pEle->m_nIndex);
			pManageHLL = m_pCalcStd->GetHLLFemData(nHLL);
			
			vForce     = pManageHLL->GetJoint((unsigned short)pBx->GetNumberGirder()+1)->GetResultForce(1)->m_Force;
			CVector vStt = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
			CVector vEnd = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
			double	dRate = 1.0;

			CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);			
			pLoad->SetLoadForce(0, dRate, CVector(0,0, -vForce.z), CVector(0,0, -vForce.z), CVector(0,0,0), CVector(0,0,0), "TU�Ŵ�", CVector(0,0,0), TRUE); 
		}

		pFemData->AddCombo("TMPSTCON", "STEEL",		CCombo::ADD, CCombo::LOAD, 1.0, FALSE);
		pFemData->AddCombo("TMPSTCON", "CONSTEEL",	CCombo::ADD, CCombo::LOAD, 1.0, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////
///�ռ� �� ������ ������(DeadPost.s2k)
/////////////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeForcePostDead(CFEMManage *pFemData, long &nLoadCase)
{
	CPlateBridgeApp *pDB    = m_pDataManage->GetBridge();
	CCalcData       *pData      = m_pDataManage->GetCalcData();		
	CFEMManage      *pManageHLL = NULL;

	CVector vForce(0,0,0);
	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetBxOnJijum(0);
	// �μӼ�������
	double dExtraForce = tokN(pData->m_AddLoad)*toM(pDB->GetWidthSlabDom(pBx))/pDB->GetGirdersu();
	// �ռ��� ������
	pFemData->MakeElementLoadForce(nLoadCase+1);
	pFemData->m_szLCName.SetAt(nLoadCase, "AFTER");
	if(pData->m_AddLoad == 0)	pFemData->m_szLCComment.SetAt(nLoadCase, "�ռ��� ������");
	else						pFemData->m_szLCComment.SetAt(nLoadCase, "�ռ��� ������ (�μӼ������� ����)");

	unsigned short nIdx=0;
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
		while(Finder.GetBxFixNext())
		{			
			if(!pBx->IsJoint())
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			CElement *pEle = pFemData->GetElement(nIdx);

			long  nHLL     = GetHLLNoByElemID(pEle->m_nIndex);
			
			if(m_pCalcStd->GetSizeHLLFemData() == 0) return;

			pManageHLL     = m_pCalcStd->GetHLLFemData(nHLL);

			long	nLoadCaseHLL	= pManageHLL->GetLoadCaseIndex("AFTER MAX");
			vForce	= pManageHLL->GetJoint((unsigned short)nG+1)->GetResultForce(nLoadCaseHLL)->m_Force;
			CVector vStt   = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
			CVector vEnd   = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;
			double dRate=1.0;
			CLoadForce *pLoad = pEle->GetLoadForce(nLoadCase);

			pLoad->SetLoadForce(0, dRate, CVector(0,0,-vForce.z-dExtraForce), CVector(0,0,-vForce.z-dExtraForce), CVector(0,0,0), CVector(0,0,0), "", CVector(0,0,0), TRUE); 
			pBx = Finder.GetBxNext();
			++nIdx;
		}
	}
}

void CFemModelingCalc::MakeForceLive(CFEMManage *pFemData, long &nLoadCase)
{
	CCalcData       *pData      = m_pDataManage->GetCalcData();	
	
	if(m_pCalcStd->GetSizeHLLFemData() == 0) return;
	CFEMManage      *pManageHLL = m_pCalcStd->GetHLLFemData(pData->m_CurLiveHL);

	CVector vForce(0,0,0);
	CString szLC("");
	CString szCombo("");

	for(long nLC = 0; nLC < pManageHLL->GetLoadCaseSize(); nLC++)
	{
		szLC = pManageHLL->m_szLCName[nLC];
		if(szLC.Find("PEOPLE")!=-1)		
		{
			// �������ߵ� �𵨸��ÿ��� �й��������� ��ҿ� ������
			pFemData->MakeElementLoadForce(nLoadCase+1);
			pFemData->m_szLCName.SetAt(nLoadCase, szLC);	

			szCombo = pManageHLL->m_szLCName[pManageHLL->GetLoadCaseIndex(szLC)];
			pFemData->m_szLCComment.SetAt(nLoadCase, szCombo);

			POSITION pos = pFemData->m_ElementList.GetHeadPosition();
			while(pos)
			{
				CElement *pElm = (CElement *)pFemData->m_ElementList.GetNext(pos);	
				if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
				{
					long nG = GetGirderNoByElemID(pElm->m_nIndex);
					vForce = pManageHLL->GetJoint((unsigned short)nG+1)->GetResultForce(nLC)->m_Force;
					// Ⱦ��ǳ���� ������ǥ��� �����Ӹ𵨸� ��ǥ��� ���� �ٸ��� ������ ������ �ʿ���.
					// x(��) => y(Ⱦ), y(��) => -x(Ⱦ), z(��) => z(��)
					// ���⼭ �ݷ��� �����ӿ� �����ϱ� ������ x,y,z���� *-1�� ����.
					// x => -y, y => x, z => -z
					CLoadForce *pLoad = pElm->GetLoadForce(nLoadCase);
					double dRate = 1.0;
					pLoad->SetLoadForce(0, dRate, CVector(-vForce.y, vForce.x,-vForce.z), CVector(-vForce.y, vForce.x,-vForce.z), CVector(0,0,0), CVector(0,0,0), "", CVector(0,0,0), TRUE);
				}
			}
			nLoadCase++;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
///Ȱ������ Comb Max���� ����
/////////////////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeForceWind(CFEMManage *pFemData, long &nLoadCase)
{
	CPlateBridgeApp *pDB    = m_pDataManage->GetBridge();
	CVector vForce(0,0,0), vForceN(0,0,0);
	// Ȱ����
	unsigned short nElm=0;
	long nCombCase = 0;
	long nQtyGir	= pDB->GetGirdersu();
	
	for(long n=0; n<2; n++)
	{
		///< �������ߵ� �𵨸��ÿ��� �й��������� ��ҿ� ������
		pFemData->MakeElementLoadForce(nLoadCase+1);
		pFemData->m_szLCName.SetAt(nLoadCase, m_pCalcStd->GetWindFemData(0)->m_szLCName[n]);
		pFemData->m_szLCComment.SetAt(nLoadCase, m_pCalcStd->GetWindFemData(0)->m_szLCComment[n]);
		nCombCase = n;//==0 ? 2 : 4;

		nElm = 0;
		for(long nG=0; nG<nQtyGir; nG++) 
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nG);

			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
			long nJijum = 0;
			while(Finder.GetBxFixNext())
			{	
				if(!pBx->IsJoint())
				{
					pBx = Finder.GetBxNext();
					continue;
				}
				CElement		*pEle			= pFemData->GetElement(nElm);
				CFEMManage		*pManageWind	= m_pCalcStd->GetWindFemData(nJijum);
				CFEMManage		*pManageWindN	= NULL;
				if(pDB->IsByunDanInBridge()==FALSE)
					pManageWindN = m_pCalcStd->GetWindFemData(nJijum);
				else
					pManageWindN = m_pCalcStd->GetWindFemData(nJijum+1);

				///< �������� ����Ͽ� ��� ������ ������ ǳ����ũ�⸦ ������.
				///< vForce�� ���� ���� ���� ���� 0ó����
				CVector vForceOr = pManageWind->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force;
				vForce.x  = EQUAL(pManageWind->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.x, 0, 0.00000001) ? 0 : pManageWind->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.x;
				vForce.y  = EQUAL(pManageWind->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.y, 0, 0.00000001) ? 0 : pManageWind->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.y;
				vForce.z  = EQUAL(pManageWind->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.z, 0, 0.00000001) ? 0 : pManageWind->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.z;
				vForceN.x = EQUAL(pManageWindN->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.x, 0, 0.00000001) ? 0 : pManageWindN->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.x;
				vForceN.y = EQUAL(pManageWindN->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.y, 0, 0.00000001) ? 0 : pManageWindN->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.y;
				vForceN.z = EQUAL(pManageWindN->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.z, 0, 0.00000001) ? 0 : pManageWindN->GetJoint((unsigned short)(nQtyGir+2+nG))->GetResultForce(nCombCase)->m_Force.z;
				
				double dLengthJigan = toM(pGir->GetBxOnJijum(nJijum+1)->GetStation() - pGir->GetBxOnJijum(nJijum)->GetStation());
				double dSlopeX = (vForceN.x-vForce.x)/dLengthJigan;
				double dSlopeY = (vForceN.y-vForce.y)/dLengthJigan;
				double dSlopeZ = (vForceN.z-vForce.z)/dLengthJigan;
				double dLJointIFromStt = toM(pFemData->GetJoint(pEle->m_nJStt)->m_vPointReal.x - pGir->GetBxOnJijum(nJijum)->GetStation());
				double dLJointJFromStt = toM(pFemData->GetJoint(pEle->m_nJEnd)->m_vPointReal.x - pGir->GetBxOnJijum(nJijum)->GetStation());

				vForce.x = vForceOr.x+dSlopeX*dLJointIFromStt;
				vForce.y = vForceOr.y+dSlopeY*dLJointIFromStt;
				vForce.z = vForceOr.z+dSlopeZ*dLJointIFromStt;
				vForceN.x = vForceOr.x+dSlopeX*dLJointJFromStt;
				vForceN.y = vForceOr.y+dSlopeY*dLJointJFromStt;
				vForceN.z = vForceOr.z+dSlopeZ*dLJointJFromStt;

				CVector vStt   = pFemData->GetJoint(pEle->m_nJStt)->m_vPoint;
				CVector vEnd   = pFemData->GetJoint(pEle->m_nJEnd)->m_vPoint;

//				CLoadForce *pLoad = NULL;
//				if(n==0)	pLoad = pEle->GetLoadForce(nLoadElm-2);
//				else		pLoad = pEle->GetLoadForce(nLoadElm-1);
				CLoadForce	*pLoad	= pEle->GetLoadForce(nLoadCase);
				
				// Ⱦ��ǳ���� ������ǥ��� �����Ӹ𵨸� ��ǥ��� ���� �ٸ��� ������ ������ �ʿ���.
				// x(��) => y(Ⱦ), y(��) => -x(Ⱦ), z(��) => z(��)
				// ���⼭ �ݷ��� �����ӿ� �����ϱ� ������ x,y,z���� *-1�� ����.
				// x => -y, y => x, z => -z				
				CFemHLLCalc HLLCalc(m_pDataManage, m_pCalcStd);
				double dRatio = 1.0;
				double dRate = 1.0;
				pLoad->SetLoadForce(0, dRate, CVector(-vForce.y*dRatio, vForce.x*dRatio,-vForce.z*dRatio), CVector(-vForce.y*dRatio, vForce.x*dRatio,-vForce.z*dRatio), CVector(0,0,0), CVector(0,0,0), "", CVector(0,0,0), TRUE);
				pBx = Finder.GetBxNext();
				++nElm;

				if(pBx->IsJijum() && pDB->IsByunDanInBridge())
					nJijum++;
			}
		}
		++nLoadCase;
	}
}


///////////////////////////////////////////////////////
/// �� �Ŵ� ���� �� ������ ��
///////////////////////////////////////////////////////
void CFemModelingCalc::MakeLane(CFEMManage *pFemData)
{
	CString szName   = "";
	for(long nEle=0 ; nEle<pFemData->GetElementSize(); nEle++)
	{
		CElement	*pEle = pFemData->GetElement((unsigned short)nEle);

		if(pEle->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			long	nG = GetGirderNoByElemID(nEle);
			szName.Format("LANE%d", nG+1);
			pFemData->AddLane(szName, nEle+1, 0);//��������� �ۿ����.
		}
	}
}

void CFemModelingCalc::MakeVehicleClass(CFEMManage *pFemData)
{
	CPlateBridgeApp	*pDB	= (CPlateBridgeApp*)m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();
	CSteelGrade		*pGrade	= m_pDataManage->GetSteelGrade();

	// ���� ���
	long nCombo = atol(pDB->m_sDeungeub)-1;
	if(nCombo< 0 || nCombo>2) nCombo= 0;

	double Pf	= tokN(pGrade->GetDBPf(nCombo));
	double Pr	= tokN(pGrade->GetDBPr(nCombo));
	double WL	= tokNPM(pGrade->GetDL(nCombo));
	double Pm	= tokN(pGrade->GetDLPm(nCombo));
	double Ps	= tokN(pGrade->GetDLPs(nCombo));
	double TankW = (pData->m_TankWeight / 2) / pData->m_TankEdit1;
	double TankD = pData->m_TankEdit1;

	double TrailP1 = pData->m_TrailP1;
	double TrailP2 = pData->m_TrailP2;
	double TrailP3 = pData->m_TrailP3;
	double TrailD1 = pData->m_TrailEdit1;
	double TrailD2 = pData->m_TrailEdit2;
	double TrailD3 = pData->m_TrailEdit3;
	double TrailD4 = pData->m_TrailEdit4;	

	double DB_L1=4.2;
	double DB_L2=4.2;
	double DB_L3=9.0;

	CString	szDB	= _T("");
	CString	szDL	= _T("");

	if(atol(CString(pDB->m_sDeungeub.GetAt(0)))==1)
	{
		szDB	= _T("DB24");
		szDL	= _T("DL24");
	}
	else if(atol(CString(pDB->m_sDeungeub.GetAt(0)))==2)
	{
		szDB	= _T("DB18");
		szDL	= _T("DL18");
	}
	else
	{
		szDB	= _T("DB13");
		szDL	= _T("DL13");
	}

	pFemData->AddVehicleDB(szDB, 0, Pf, DB_L1, 0);
	pFemData->AddVehicleDB(szDB, 0, Pr, DB_L1, 0);
	pFemData->AddVehicleDB(szDB, 0, Pr, DB_L2, DB_L3);
	pFemData->AddVehicleDL(szDL, 0, WL/3, Pm/3, Ps/3);

	if(pData->m_bTank)
	{
		pFemData->AddVehicleDB("TANK", 0, 0, 0, 0);
		pFemData->AddVehicleDB("TANK", 0, TankW, TankD, 0);
	}
	if(pData->m_bTrailer)
	{
		pFemData->AddVehicleDB("TRAIL", 0, TrailP1, 0, 0);
		pFemData->AddVehicleDB("TRAIL", 0, TrailP2, TrailD1, 0);
		pFemData->AddVehicleDB("TRAIL", 0, TrailP2, TrailD2, 0);
		pFemData->AddVehicleDB("TRAIL", 0, TrailP3, TrailD3, 0);
		pFemData->AddVehicleDB("TRAIL", 0, TrailP3, TrailD4, 0);
	}
}

void CFemModelingCalc::MakeVehicle(CFEMManage *pFemData)
{
	CPlateBridgeApp *pDB   = (CPlateBridgeApp*)m_pDataManage->GetBridge();
	CCalcData       *pData = m_pDataManage->GetCalcData();		

	if(atol(CString(pDB->m_sDeungeub.GetAt(0)))==1)//24-1���
	{
		pFemData->AddVehicleClass("DB24","DB24");
		pFemData->AddVehicleClass("DL24","DL24");
	}
	else if(atol(CString(pDB->m_sDeungeub.GetAt(0)))==2)//18-2���
	{
		pFemData->AddVehicleClass("DB18","DB18");
		pFemData->AddVehicleClass("DL18","DL18");
	}
	else
	{
		pFemData->AddVehicleClass("DB13","DB13");
		pFemData->AddVehicleClass("DL13","DL13");
	}

	if(pData->m_bTank)		pFemData->AddVehicleClass("TANK", "TANK");
	if(pData->m_bTrailer)	pFemData->AddVehicleClass("TRAIL", "TRAIL");

}

/////////////////////////////////////////////////////////////////
/// MOVING LOAD�� ������
/////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeMovingLoad(CFEMManage *pFemData)
{
	CPlateBridgeApp *pDB     = (CPlateBridgeApp*)m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData();		

	if(m_pCalcStd->GetSizeHLLFemData() == 0) return;
	CFEMManage      *pFemHLL = m_pCalcStd->GetHLLFemData(pData->m_CurLiveHL);
	
	CString szLane("");
	CString szLC("");
	CString szLoadStyle("");
	double dSf = 0;
	long nMark = 0;

	if     (atol(CString(pDB->m_sDeungeub.GetAt(0)))==1)	nMark = 24;
	else if(atol(CString(pDB->m_sDeungeub.GetAt(0)))==2)	nMark = 18;
	else													nMark = (long)13.5;	

	double dRf[6] = {1,};
	double nRf = 1;
	for(long nLC = 0; nLC < pFemHLL->GetLoadCaseSize(); nLC++)
	{
		szLC = pFemHLL->m_szLCName.GetAt(nLC);
		if(IsLiveCombMaxLC(szLC))
		{
			if     (szLC.Find("DB") != -1)		szLoadStyle.Format("DB%d", nMark);
			else if(szLC.Find("DL") != -1)		szLoadStyle.Format("DL%d", nMark);
			else if(szLC.Find("TANK") != -1)	szLoadStyle.Format("TANK");
			else if(szLC.Find("TRAIL") != -1)	szLoadStyle.Format("TRAIL");
			else	continue;

			POSITION pos = pFemHLL->m_ElementList.GetHeadPosition();	
			long nPosID = 0;
			while(pos)
			{	
				CElement *pE = (CElement *)pFemHLL->m_ElementList.GetNext(pos);		
				
				if(nPosID >0)	///< ��,���� ������ ��������..
				{
					long nFind = szLC.Find("DB");
					if(nFind==0)
					{
						CString szLane = szLC.Mid(nFind+4);
						if(atol(szLane)==3) dRf[0]=0.9;
						else if(atol(szLane)>3) dRf[0]=0.75;
						else dRf[0]=1.0;
					}
					nFind = szLC.Find("DL");
					if(nFind==0)
					{
						CString szLane = szLC.Mid(nFind+4);
						if(atol(szLane)==3) dRf[0]=0.9;
						else if(atol(szLane)>3) dRf[0]=0.75;
						else dRf[0]=1.0;
					}

					CJoint *pJ_I = pFemHLL->GetJoint(pE->m_nJStt);
					dSf = pJ_I->GetResultForce(nLC)->m_Force.z;
					if(dSf<=0.001) dSf = 0.001;
					
					szLane.Format("LANE%d", nPosID);
					pFemData->AddMovingLoad(szLC, (long)nRf, dRf, szLoadStyle, szLane, Round(dSf,3), 1, 1);
				}
				nPosID++;
			}
		}
	}
}

// ����ħ�Ͽ� ���� Uz���� ������
void CFemModelingCalc::MakeUZPLoad(CFEMManage *pFemData)
{
	CCalcData        *pData = m_pDataManage->GetCalcData();
	CString szLoadName = "";
	long nG = -1;
	long nPier = 1;
	long nLoadCase = 0;
	CPlateBasicIndex *pBxPre	 = NULL;
	if(pData->m_nModelingSettleMethod==0)
	{
		for(long nEle=0; nEle<pFemData->GetElementSize(); nEle++)
		{
			CElement	*pElem = pFemData->GetElement((unsigned short)nEle);

			if(pElem->m_LongAttrib[ELEMENT_TYPE] & FRAME_SHOE_ELEMENT)
			{
				CJoint	*pJStt	= pFemData->GetJoint(pElem->m_nJStt);
				CJoint	*pJEnd	= pFemData->GetJoint(pElem->m_nJEnd);
				CPlateBasicIndex	*pBx	= NULL;

				double	dDist	= 0;
				if(m_pCalcStd->GetBxByJoint(pJStt)->IsJijum())
				{
					pBx		= m_pCalcStd->GetBxByJoint(pJStt);
					dDist	= 0;
				}
				if(m_pCalcStd->GetBxByJoint(pJEnd)->IsJijum())
				{
					pBx		= m_pCalcStd->GetBxByJoint(pJEnd);
					dDist	= 1.0;
				}

				if(pBx == pBxPre)	continue;
				if(!pBx)			continue;

				if(nG!=pBx->GetNumberGirder())	nPier=1;

				if(pBx->IsDanbuJijum())
				{
					if(pBx->IsDanbuJijumStt())
						szLoadName = "A1";
					else
						szLoadName = "A2";
				}
				else
				{
					szLoadName.Format("P%d", nPier);
					nPier++;
				}
				long nLC = pFemData->GetLoadCaseByString(szLoadName);
				if(nLC==-1)
				{
					pFemData->MakeElementLoadForce(nLoadCase+1);
					pFemData->m_szLCName.SetAt(nLoadCase, szLoadName);
					pFemData->m_szLCComment.SetAt(nLoadCase, szLoadName);

					CLoadForce *pLoad = pElem->GetLoadForce(nLoadCase);
					pLoad->SetDiplacementForce(dDist, CVector(0, 0, -toM(pData->m_JijumDownSize)));
					++nLoadCase;
				}
				else
				{
					CLoadForce *pLoad = pElem->GetLoadForce(nLC);
					pLoad->SetDiplacementForce(dDist, CVector(0, 0, -toM(pData->m_JijumDownSize)));
				}

				nG = pBx->GetNumberGirder();
				pBxPre = pBx;
			}
		}
	}
	else
	{
		long nEle = 0;
		for(nEle=0; nEle<pFemData->GetElementSize(); nEle++)
		{
			CElement	*pElem = pFemData->GetElement((unsigned short)nEle);
			
			if(pElem->m_LongAttrib[ELEMENT_TYPE] & FRAME_SHOE_ELEMENT)
			{
				CJoint	*pJStt	= pFemData->GetJoint(pElem->m_nJStt);
				CJoint	*pJEnd	= pFemData->GetJoint(pElem->m_nJEnd);
				CPlateBasicIndex	*pBx	= NULL;
				
				double	dDist	= 0;
				if(m_pCalcStd->GetBxByJoint(pJStt)->IsJijum())
				{
					pBx		= m_pCalcStd->GetBxByJoint(pJStt);
					dDist	= 0;
				}
				if(m_pCalcStd->GetBxByJoint(pJEnd)->IsJijum())
				{
					pBx		= m_pCalcStd->GetBxByJoint(pJEnd);
					dDist	= 1;
				}
				
				if(pBx == pBxPre)	continue;
				if(!pBx)			continue;
				
				if(nG!=pBx->GetNumberGirder())	nPier=1;
				
				if(pBx->IsDanbuJijum())
				{
					if(pBx->IsDanbuJijumStt())	szLoadName = "A1";
					else						szLoadName = "A2";
				}
				else
				{
					szLoadName.Format("P%d", nPier);
					nPier++;
				}

				long nLC = pFemData->GetLoadCaseByString(szLoadName);
				if(nLC==-1)
				{
					pFemData->MakeElementLoadForce(nLoadCase+1);
					pFemData->m_szLCName.SetAt(nLoadCase, szLoadName);
					pFemData->m_szLCComment.SetAt(nLoadCase, szLoadName);
					
					CLoadForce *pLoad = pElem->GetLoadForce(nLoadCase);
					pLoad->SetDiplacementForce(dDist, CVector(0, 0, -toM(pData->m_JijumDownSize)));
					++nLoadCase;
				}
				else
				{
					CLoadForce *pLoad = pElem->GetLoadForce(nLC);
					pLoad->SetDiplacementForce(dDist, CVector(0, 0, -toM(pData->m_JijumDownSize)));
				}
				
				nG = pBx->GetNumberGirder();
				pBxPre = pBx;
			}
		}
		
		long	nCount = 0;
		for(nEle=0; nEle<pFemData->GetElementSize(); nEle++)
		{
			CElement	*pElem = pFemData->GetElement((unsigned short)nEle);
			
			if(pElem->m_LongAttrib[ELEMENT_TYPE] & FRAME_SHOE_ELEMENT)
			{
				CJoint	*pJStt	= pFemData->GetJoint(pElem->m_nJStt);
				CJoint	*pJEnd	= pFemData->GetJoint(pElem->m_nJEnd);
				CPlateBasicIndex	*pBx	= NULL;
				
				double	dDist	= 0;
				if(m_pCalcStd->GetBxByJoint(pJStt)->IsJijum())
				{
					pBx		= m_pCalcStd->GetBxByJoint(pJStt);
					dDist	= 0;
				}
				if(m_pCalcStd->GetBxByJoint(pJEnd)->IsJijum())
				{
					pBx		= m_pCalcStd->GetBxByJoint(pJEnd);
					dDist	= 1;
				}
				
				if(pBx == pBxPre)	continue;
				if(!pBx)			continue;
				
				if(nG!=pBx->GetNumberGirder())	nCount=0;
				
				if(nCount%2==0)	szLoadName = "2N+1";
				else			szLoadName = "2N+2";

				long nLC = pFemData->GetLoadCaseByString(szLoadName);
				if(nLC==-1)
				{
					pFemData->MakeElementLoadForce(nLoadCase+1);
					pFemData->m_szLCName.SetAt(nLoadCase, szLoadName);
					pFemData->m_szLCComment.SetAt(nLoadCase, szLoadName);
					
					CLoadForce *pLoad = pElem->GetLoadForce(nLoadCase);
					pLoad->SetDiplacementForce(dDist, CVector(0, 0, -toM(pData->m_JijumDownSize)));
					++nLoadCase;
				}
				else
				{
					CLoadForce *pLoad = pElem->GetLoadForce(nLC);
					pLoad->SetDiplacementForce(dDist, CVector(0, 0, -toM(pData->m_JijumDownSize)));
				}
// 				
// 				pFemData->m_szLCName.Add(szLoadName);	
// 				pFemData->m_szTitle.Add(szLoadName);	
// 				
// 				long nLoadForce = 0;
// 				if(szLoadName=="2N+1")		nLoadForce = pDB->m_nQtyJigan+1;
// 				else if(szLoadName=="2N+2")	nLoadForce = pDB->m_nQtyJigan+2;
// 				
// 				long nLC = pFemData->GetLoadCaseByString(szLoadName);
// 				CLoadForce *pLoad = pElem->GetLoadForce(nLC);
// 				pLoad->SetDiplacementForce(dDist, CVector(0, 0, -toM(pData->m_JijumDownSize)));
				
				nG = pBx->GetNumberGirder();
				pBxPre = pBx;
				nCount++;
			}
		}
	}
	// m_szLCName �������� ����
	long nSize = pFemData->m_szLCName.GetSize();
	long k = 0;
	for(k=0; k<nSize-1; k++)
	{
		CString sName1 = pFemData->m_szLCName[k];
		for(long l=k+1; l<nSize; l++)
		{
			CString sName2 = pFemData->m_szLCName[l];
			if(sName1 == sName2)
			{
				pFemData->m_szLCName.RemoveAt(l--);
				nSize--;
			}
		}
	}
	// m_szTitle �������� ����
	nSize = pFemData->m_szLCComment.GetSize();
	for(k=0; k<nSize-1; k++)
	{
		CString sName1 = pFemData->m_szLCComment[k];
		for(long l=k+1; l<nSize; l++)
		{
			CString sName2 = pFemData->m_szLCComment[l];
			if(sName1 == sName2)
			{
				pFemData->m_szLCComment.RemoveAt(l--);
				nSize--;
			}
		}
	}
}

void CFemModelingCalc::MakeCombinationIndex(long nCnt, long nTotal, CLongArray& IndexArr)
{
	long* Idx1 = new long[nCnt];
	long* Idx2 = new long[nCnt];
	long n = 0;
	for (n=0; n<nCnt; n++)
	{
		Idx1[n] = n;
		Idx2[n] = nTotal-nCnt+n;
	}

	while(TRUE)
	{
		for (long i=0; i<nCnt; i++) IndexArr.Add(Idx1[i]);

		long nCheck = nCnt-1;
		BOOL bReset = FALSE;

		while(TRUE)
		{
			Idx1[nCheck] += 1;

			if (Idx1[nCheck]>Idx2[nCheck])
			{
				nCheck--;
				bReset = TRUE;
				if (nCheck<0) break;
				continue;
			}
			
			if (bReset)
			{
				for (long j=nCheck+1; j<nCnt; j++)
					Idx1[j]= Idx1[j-1]+1;
			}
			break;
		}

		if (Idx1[0]>Idx2[0])
			break;
	}

	for (n=0; n<nCnt; n++)
	{
		Idx1[n] = NULL;
		Idx2[n] = NULL;
	}
	delete[] Idx1;
	delete[] Idx2;
}


void CFemModelingCalc::MakeUZPCombo(CFEMManage *pFemData)
{
	CPlateBridgeApp* pDB   = (CPlateBridgeApp*)m_pDataManage->GetBridge();
	CCalcData*       pData = m_pDataManage->GetCalcData(); 
	CPlateBasicIndex* pBx = NULL;
	CString szLoadName = "";
	CString szCombName = "";
	
	CPlateGirderApp* pGir = pDB->GetGirder(0);
	CPlateBxFinder Finder(pGir);
	pBx = Finder.GetBxFirst(BX_JIJUM);

	long			nJijum = 0;
	long			nPier=1;
	CStringArray	szLoadNameArr;
	while(pBx)
	{
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		if(pBx->IsJijum())
		{
			if(pBx->IsDanbuJijumStt())		szLoadName = "A1";
			else if(pBx->IsDanbuJijumEnd())	szLoadName = "A2";
			else
			{
				szLoadName.Format("P%d", nPier);
				nPier++;
			}
			if(pData->m_nModelingSettleMethod==1)
				pFemData->AddCombo("SETTLE", szLoadName, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
			szLoadNameArr.Add(szLoadName);
			nJijum++;
		}
		pBx = Finder.GetBxNext();
	}
	
	if(pData->m_nModelingSettleMethod==1)
	{
		pFemData->AddCombo("SETTLE", "2N+1", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		pFemData->AddCombo("SETTLE", "2N+2", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
	}
	else
	{
		CLongArray *pIndex		= new CLongArray[nJijum-2];
		CStringArray *pszComb	= new CStringArray[nJijum-2];

		// create coombination
		// ~ N ~
		long nCase = 0;
		for (nCase=0; nCase<nJijum-2; nCase++)
		{
			MakeCombinationIndex(nCase+2, nJijum, pIndex[nCase]);
			long nCnt = 0;

			for (long i=0; i<pIndex[nCase].GetSize(); i+=nCase+2)
			{
				szCombName.Format("%dN%d", nCase+2, nCnt+1);
				pszComb[nCase].Add(szCombName);
				nCnt++;

				for (long j=0; j<nCase+2; j++)
				{
					long nLoad = pIndex[nCase][i+j];
					szLoadName = szLoadNameArr[nLoad];
					pFemData->AddCombo(szCombName, szLoadName, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
				}		
			}
		}

		// create coombination
		// ~ N
		for (nCase=0; nCase<nJijum-2; nCase++)
		{
			szCombName.Format("%dN", nCase+2);
			long nSize = pszComb[nCase].GetSize();
			for (long i=0; i < nSize; i++)
			{
				szLoadName = pszComb[nCase].GetAt(i);;
				pFemData->AddCombo(szCombName, szLoadName, CCombo::ENEV, CCombo::COMB, 1, FALSE);
			}
		}

		// create coombination
		// settlement
		long nTotal = nJijum + (nJijum-2);
		long nCnt	= 2;
		for (nCase=0; nCase<nTotal; nCase++)
		{
			szCombName = _T("SETTLE");
			if (nCase<szLoadNameArr.GetSize())
			{
				szLoadName = szLoadNameArr.GetAt(nCase);
				pFemData->AddCombo(szCombName, szLoadName, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
			}
			else
			{
				szLoadName.Format("%dN", nCnt++);
				pFemData->AddCombo(szCombName, szLoadName, CCombo::ENEV, CCombo::COMB, 1, FALSE);
			}

		}

		// Array Release
		for (nCase=0; nCase<nJijum-2; nCase++)
		{
			CLongArray arrIndex = pIndex[nCase];
			arrIndex.RemoveAll();

		//	CStringArray arrComb = pszComb[nCase];
		//	arrComb.RemoveAll();
		}
		
		//if(nJijum>2) //Memory leak
		{
			delete[] pIndex;
			delete[] pszComb;	
		}
	}
}

/////////////////////////////////////////////////////////////////////
/// *Live.s2k�� COMBO���� ����
///  pFemHLL(Ⱦ�й�)��    0  = �ռ���������
///	     				  1  = �ռ��Ļ�����
///	 		    		  2~ = Ȱ����
///  �������̴� ù��° ��������
/////////////////////////////////////////////////////////////////////
void CFemModelingCalc::MakeMovingLoadCombo(CFEMManage *pFemData)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData       *pData   = m_pDataManage->GetCalcData(); 
	if(m_pCalcStd->GetSizeHLLFemData() == 0) return;
	CFEMManage      *pFemHLL = m_pCalcStd->GetHLLFemData(pData->m_CurLiveHL);

	CString szLC("");
	double  dJiganL        = m_pDataManage->GetBridge()->m_dLengthJigan[0];
	double  dScaleFactor   = 1+(15/(40+toM(dJiganL)));
	if (dScaleFactor>1.3)	dScaleFactor = 1.3;
	if (dScaleFactor<0)		dScaleFactor = 0;
	
	BOOL bDB=FALSE, bDL=FALSE, bMil=FALSE;
	long nLC = 0;
	for(nLC = 0; nLC < pFemHLL->GetLoadCaseSize(); nLC++)
	{
		szLC = pFemHLL->m_szLCName.GetAt(nLC);
		if(IsLiveCombMaxLC(szLC))		
		{		
			if(szLC.Find("PEOPLE") != -1)
				pFemData->AddCombo(szLC, szLC, CCombo::ENEV, CCombo::LOAD, 1, FALSE);	// ���������϶�
			else if(szLC.Find("DB") != -1)
				pFemData->AddCombo(szLC, szLC, CCombo::ENEV, CCombo::MOVE, dScaleFactor, TRUE);	// DB, DL
			else if(szLC.Find("DL") != -1)
				pFemData->AddCombo(szLC, szLC, CCombo::ENEV, CCombo::MOVE, dScaleFactor, TRUE);	// DB, DL
			else if(szLC.Find("TANK") != -1 || szLC.Find("TRAIL") != -1)
			{	
				if(szLC.Find("TANK") != -1)
					dScaleFactor = pData->m_TankImpact/100+1;
				else 
					dScaleFactor = pData->m_TrailImpact/100+1;
				pFemData->AddCombo(szLC, szLC, CCombo::ENEV, CCombo::MOVE, dScaleFactor, TRUE);
			}
		}
	}

	for(nLC = 0; nLC < pFemHLL->GetLoadCaseSize(); nLC++)
	{
		szLC = pFemHLL->m_szLCName.GetAt(nLC);
		if(IsLiveCombMaxLC(szLC))		
		{
			if(szLC.Find("DB") != -1)
			{
				pFemData->AddCombo("DB", szLC, CCombo::ENEV, CCombo::COMB, 1, FALSE);
				bDB = TRUE;
			}
			else if(szLC.Find("DL") != -1)
			{
				pFemData->AddCombo("DL", szLC, CCombo::ENEV, CCombo::COMB, 1, FALSE);
				bDL = TRUE;
			}
			else if(szLC.Find("TANK") != -1 || szLC.Find("TRAIL") != -1)
			{	
				pFemData->AddCombo("MTY", szLC, CCombo::ENEV, CCombo::COMB, 1, FALSE);
				bMil = TRUE;
			}
		}
	}
	if(bDB)		pFemData->AddCombo("CAR", "DB", CCombo::ENEV, CCombo::COMB, 1, FALSE);
	if(bDL)		pFemData->AddCombo("CAR", "DL", CCombo::ENEV, CCombo::COMB, 1, FALSE);
	if(bMil)	pFemData->AddCombo("CAR", "MTY", CCombo::ENEV, CCombo::COMB, 1, FALSE);

	///< ��������
	CString	szCombo	= _T("");
	CStringArray szArrCombo;
	for(long nHDan=0; nHDan<pDB->GetQtyHDanNode(); nHDan++)
	{
		if(pDB->GetValueTypeHDan(nHDan,1)==HT_PEOPLEROAD)
		{
			szCombo.Format("PEOPLE-%d", szArrCombo.GetSize()+1);
			szArrCombo.Add(szCombo);
			pFemData->AddCombo("LRPEOPLE", szCombo, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		}
	}
	if(szArrCombo.GetSize()>0)
	{
		for(long n=0; n<szArrCombo.GetSize(); n++)
			pFemData->AddCombo("PEOPLE", szArrCombo.GetAt(n), CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		pFemData->AddCombo("PEOPLE",   "LRPEOPLE", CCombo::ENEV, CCombo::COMB, 1, TRUE);

		//PEOPLE and CAR		
		pFemData->AddCombo("PEOCAR", "PEOPLE", CCombo::ADD, CCombo::COMB, 1, FALSE);
		pFemData->AddCombo("PEOCAR", "CAR", CCombo::ADD, CCombo::COMB, 1, TRUE);
	}

	if(pData->m_bWind)
	{
		BOOL bBangEmL = pData->DESIGN_FLOOR_DATA[0].m_bBangEm;
		BOOL bBangEmR = pData->DESIGN_FLOOR_DATA[1].m_bBangEm;

		if(bBangEmL && !bBangEmR) ///< ������������
		{
			pFemData->AddCombo("WIND", "LLWIND", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
			pFemData->AddCombo("WIND", "LRWIND", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		}
		else if(!bBangEmL && bBangEmR) ///< ������������
		{
			pFemData->AddCombo("WIND", "RRWIND", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
			pFemData->AddCombo("WIND", "RLWIND", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		}
		else if(bBangEmL && bBangEmR) ///< ���� ��
		{
			pFemData->AddCombo("WIND", "LLWIND", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
			pFemData->AddCombo("WIND", "RRWIND", CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		}
		// 2006.11.13 �ɵ��� �߰�
		if(szArrCombo.GetSize()>0)
		{
			pFemData->AddCombo("PECAWIN", "PEOPLE", CCombo::ADD, CCombo::COMB, 1, FALSE);
			pFemData->AddCombo("PECAWIN", "CAR", CCombo::ADD, CCombo::COMB, 1, FALSE);
			pFemData->AddCombo("PECAWIN", "WIND", CCombo::ADD, CCombo::COMB, 0.5, FALSE);
		}
		else
		{		
			pFemData->AddCombo("WINCAR", "CAR", CCombo::ADD, CCombo::COMB, 1, FALSE);
			pFemData->AddCombo("WINCAR", "WIND", CCombo::ADD, CCombo::COMB, 0.5, FALSE);
		}
	}

	//DESIGN
	pFemData->AddCombo("DESIGN", "CAR", CCombo::ENEV, CCombo::COMB, 1, FALSE);
	if(szArrCombo.GetSize()>0)
		pFemData->AddCombo("DESIGN", "PEOCAR", CCombo::ENEV, CCombo::COMB, 1, FALSE);
	// 2006.11.13 �ɵ��� ����
	if(pData->m_bWind)
	{
		if(szArrCombo.GetSize()>0)
			pFemData->AddCombo("DESIGN", "PECAWIN", CCombo::ENEV, CCombo::COMB, 1, TRUE);
		else
			pFemData->AddCombo("DESIGN", "WINCAR", CCombo::ENEV, CCombo::COMB, 1, TRUE);
	}
}



/////////////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////////////
long CFemModelingCalc::GetJointNoByBx(CPlateBasicIndex* pBx)
{
	CPlateBridgeApp* pDB   = (CPlateBridgeApp*)m_pDataManage->GetBridge();
	long nR    = 0;
	long nCrNo = pBx->GetSection() ? pBx->GetSection()->GetSelfCount() : 0;
	
	// TU���ӱ��� �ƴϸ� ���κ��ܿ� ������ ������ �����Ƿ� �ӵ��� ���� �ٷ� ó����
	if(!pDB->IsTUContinuousSpan())
	{
		for(long nG=0; nG<pBx->GetNumberGirder(); nG++)	
			nR += pDB->GetGirder(nG)->GetQtyCrossBeamVBracing();	
		return nR+nCrNo;
	}

	for(long nG=0; nG<pBx->GetNumberGirder()+1; nG++)	
	{
		CPlateBxFinder Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBxJoint = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);

		while(pBxJoint)
		{
			if(!pBxJoint->IsJoint())
			{
				pBxJoint = Finder.GetBxNext();
				continue;
			}
			if(pBx == pBxJoint)	return nR;
			nR++;
			pBxJoint = Finder.GetBxNext();
		}
	}
	return nR;
}

//DB����
void CFemModelingCalc::MakeMaxMinForceDB()
{	
	CFEMManage		*pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);

	POSITION pos = pFemData->m_ElementList.GetHeadPosition();
	long nIndex = 0;
	long nElem	= 0;

	while(pos)
	{
		CElement			*pE				= (CElement*)pFemData->m_ElementList.GetNext(pos);
		CElementResult		*pEleResult		= pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("DB MAX");
		if(nIndex==-1 || nIndex>=pE->m_ResultForce.GetSize())		continue;
		//Max
		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dDBMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];
			pEleResult->m_dDBMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dDBMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dDBMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dDBMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dDBMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}

	pos		= pFemData->m_ElementList.GetHeadPosition();
	nIndex	= 0;
	nElem	= 0;
	while(pos)
	{
		//Min
		CElement			*pE				= (CElement*)pFemData->m_ElementList.GetNext(pos);		
		CElementResult		*pEleResult		= pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("DB MIN");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;

		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dDBMin[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dDBMin[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dDBMin[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dDBMin[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dDBMin[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dDBMin[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}

}

//DL����
void CFemModelingCalc::MakeMaxMinForceDL()
{
	CFEMManage	*pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);

	POSITION pos = pFemData->m_ElementList.GetHeadPosition();
	long	nIndex	= 0;
	long	nElem	= 0;
	while(pos)
	{
		CElement			*pE				= (CElement*)pFemData->m_ElementList.GetNext(pos);
		CElementResult		*pEleResult		= (CElementResult*)pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("DL MAX");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;
		//Max
		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dDLMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dDLMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dDLMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dDLMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dDLMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dDLMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}

	pos		= pFemData->m_ElementList.GetHeadPosition();
	nIndex	= 0;
	nElem	= 0;
	while(pos)
	{
		//Min
		CElement			*pE				= (CElement *)pFemData->m_ElementList.GetNext(pos);
		CElementResult		*pEleResult		= (CElementResult*)pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("DL MIN");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;

		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dDLMin[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dDLMin[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dDLMin[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dDLMin[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dDLMin[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dDLMin[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}

}

//MTY����
void CFemModelingCalc::MakeMaxMinForceMTY()
{	
	CFEMManage	*pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
	POSITION pos = pFemData->m_ElementList.GetHeadPosition();

	long	nIndex	= 0;
	long	nElem	= 0;

	while(pos)
	{
		CElement			*pE				= (CElement*)pFemData->m_ElementList.GetNext(pos);
		CElementResult		*pEleResult		= (CElementResult*)pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("MTY MAX");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;
		//Max
		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dMTYMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dMTYMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dMTYMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dMTYMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dMTYMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dMTYMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}

	pos		= pFemData->m_ElementList.GetHeadPosition();
	nIndex	= 0;
	nElem	= 0;
	while(pos)
	{
		//Min
		CElement			*pE				= (CElement *)pFemData->m_ElementList.GetNext(pos);		
		CElementResult		*pEleResult		= pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("MTY MIN");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;

		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dMTYMin[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dMTYMin[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dMTYMin[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dMTYMin[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dMTYMin[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dMTYMin[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}

	}
}

//People����
void CFemModelingCalc::MakeMaxMinForcePeople()
{	
	CFEMManage	*pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
	POSITION pos = pFemData->m_ElementList.GetHeadPosition();

	long nIndex = 0;
	long nElem	= 0;
	while(pos)
	{
		CElement			*pE				= (CElement *)pFemData->m_ElementList.GetNext(pos);
		CElementResult		*pEleResult		= pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("PEOPLE MAX");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;
		//Max
		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dPeopleMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dPeopleMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dPeopleMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dPeopleMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dPeopleMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dPeopleMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}

	pos		= pFemData->m_ElementList.GetHeadPosition();
	nIndex	= 0;
	nElem	= 0;
	while(pos)
	{
		//Min
		CElement			*pE				= (CElement *)pFemData->m_ElementList.GetNext(pos);
		CElementResult		*pEleResult		= pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("PEOPLE MIN");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;

		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dPeopleMin[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dPeopleMin[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dPeopleMin[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dPeopleMin[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dPeopleMin[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dPeopleMin[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}
}


//ǳ����
void CFemModelingCalc::MakeMaxMinForceWind()
{	
	CFEMManage	*pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
	POSITION pos = pFemData->m_ElementList.GetHeadPosition();
	long nIndex = 0;
	long nElem = 0;
	while(pos)
	{
		CElement	*pE			= (CElement *)pFemData->m_ElementList.GetNext(pos);
		CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("WIND MAX");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;
		//Max
		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dWindMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dWindMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dWindMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dWindMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dWindMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dWindMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}

	pos		= pFemData->m_ElementList.GetHeadPosition();
	nIndex	= 0;
	nElem	= 0;
	while(pos)
	{
		//Min
		CElement	*pE			= (CElement *)pFemData->m_ElementList.GetNext(pos);
		CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nElem++);
		nIndex = pFemData->GetLoadCaseIndex("WIND MIN");
		if(nIndex==-1 || nIndex >= pE->m_ResultForce.GetSize())		continue;

		CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
		for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
		{
			pEleResult->m_dWindMin[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
			pEleResult->m_dWindMin[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
			pEleResult->m_dWindMin[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
			pEleResult->m_dWindMin[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
			pEleResult->m_dWindMin[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
			pEleResult->m_dWindMin[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
		}
	}
}

void CFemModelingCalc::MakeResultData()
{
	long nFemSize	= GetFrameAnalysisSize();

	m_pCalcStd->RemoveResultArray();
	m_pCalcStd->MakeResultArray(nFemSize);

	for(long n=0; n<nFemSize; n++)
	{
		long	nIdx = m_pCalcStd->GetAnalysisTypeByIndex(n);
		CFEMManage		*pFemData	= m_pCalcStd->GetModelingFemData(nIdx);
		CResultArray	*pResult	= m_pCalcStd->GetModelingResult(nIdx);

		long	nEleSize	= pFemData->GetElementSize();
		long	nJointSize	= pFemData->GetJointSize();

		AhTPAMake(nEleSize, &pResult->m_EleResultArr, (CElementResult*)0);
		AhTPAMake(nJointSize, &pResult->m_JointResultArr, (CJointResult*)0);
	}
}

void CFemModelingCalc::MakeSummaryOfElementForce()
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CFEMManage		*pFemData	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);

	if(pFemData->GetTotalResultSize(ELM_SIZE) > 0)
	{
		POSITION pos = pFemData->m_ElementList.GetHeadPosition();
		long	nIndex	= 0;//Loadcase
		long	nElem	= 0;
		while(pos)
		{
			nIndex = 0;	//Loadcase-steel
			CElement		*pE			= (CElement *)pFemData->m_ElementList.GetNext(pos);
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nElem++);
			CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
			for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				pEleResult->m_dPreDeadSteel[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];
				pEleResult->m_dPreDeadSteel[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];
				pEleResult->m_dPreDeadSteel[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];
				pEleResult->m_dPreDeadSteel[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];
				pEleResult->m_dPreDeadSteel[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];
				pEleResult->m_dPreDeadSteel[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];
			}

			nIndex = 1;//concreate
			if(!pDB->IsTUGir())
			{
				CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
				for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
				{
					CResultForceElementData *pResultEleData = pResultForce->m_ResultForceElement[a];
					pEleResult->m_dPreDeadSlab[ELE_SHEAR_2][a] = pResultEleData->m_dForceOnElement[0];//GetForce(a, 0);
					pEleResult->m_dPreDeadSlab[ELE_MOMENT3][a] = pResultEleData->m_dForceOnElement[1];//GetForce(a, 1);
					pEleResult->m_dPreDeadSlab[ELE_AXFORCE][a] = pResultEleData->m_dForceOnElement[2];//GetForce(a, 2);
					pEleResult->m_dPreDeadSlab[ELE_SHEAR_3][a] = pResultEleData->m_dForceOnElement[3];//GetForce(a, 3);
					pEleResult->m_dPreDeadSlab[ELE_MOMENT2][a] = pResultEleData->m_dForceOnElement[4];//GetForce(a, 4);
					pEleResult->m_dPreDeadSlab[ELE_AXTORSN][a] = pResultEleData->m_dForceOnElement[5];//GetForce(a, 5);
				}
			}
			if(pDB->IsTUContinuousSpan())	// TU�Ŵ� ���ӱ�
			{
				nIndex = 1;
				pResultForce	= pE->GetResultForce(nIndex);
				long a = 0;
				for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
				{
					pEleResult->m_dPreDeadConf2[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];
					pEleResult->m_dPreDeadConf2[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];
					pEleResult->m_dPreDeadConf2[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];
					pEleResult->m_dPreDeadConf2[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];
					pEleResult->m_dPreDeadConf2[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];
					pEleResult->m_dPreDeadConf2[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];
				}

				nIndex = 2;
				pResultForce	= pE->GetResultForce(nIndex);
				double dSteelConfMax[8][10] = {0,};
				double dSteelConfMin[8][10] = {0,};

				for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
				{
					dSteelConfMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];
					dSteelConfMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];
					dSteelConfMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];
					dSteelConfMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];
					dSteelConfMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];
					dSteelConfMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];
				}				
				nIndex = 3;
				pResultForce	= pE->GetResultForce(nIndex);
				for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
				{
					CResultForceElementData *pResultEleData = pResultForce->m_ResultForceElement[a];
					dSteelConfMin[ELE_SHEAR_2][a] = pResultEleData->m_dForceOnElement[0];
					dSteelConfMin[ELE_MOMENT3][a] = pResultEleData->m_dForceOnElement[1];
					dSteelConfMin[ELE_AXFORCE][a] = pResultEleData->m_dForceOnElement[2];
					dSteelConfMin[ELE_SHEAR_3][a] = pResultEleData->m_dForceOnElement[3];
					dSteelConfMin[ELE_MOMENT2][a] = pResultEleData->m_dForceOnElement[4];
					dSteelConfMin[ELE_AXTORSN][a] = pResultEleData->m_dForceOnElement[5];
				}				

				for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
				{
					pEleResult->m_dPreDeadSteelConf[ELE_SHEAR_2][a] = fabs(dSteelConfMax[ELE_SHEAR_2][a]) > fabs(dSteelConfMin[ELE_SHEAR_2][a]) ? dSteelConfMax[ELE_SHEAR_2][a] : dSteelConfMin[ELE_SHEAR_2][a];
					pEleResult->m_dPreDeadSteelConf[ELE_MOMENT3][a] = fabs(dSteelConfMax[ELE_MOMENT3][a]) > fabs(dSteelConfMin[ELE_MOMENT3][a]) ? dSteelConfMax[ELE_MOMENT3][a] : dSteelConfMin[ELE_MOMENT3][a];
					pEleResult->m_dPreDeadSteelConf[ELE_AXFORCE][a] = fabs(dSteelConfMax[ELE_AXFORCE][a]) > fabs(dSteelConfMin[ELE_AXFORCE][a]) ? dSteelConfMax[ELE_AXFORCE][a] : dSteelConfMin[ELE_AXFORCE][a];
					pEleResult->m_dPreDeadSteelConf[ELE_SHEAR_3][a] = fabs(dSteelConfMax[ELE_SHEAR_3][a]) > fabs(dSteelConfMin[ELE_SHEAR_3][a]) ? dSteelConfMax[ELE_SHEAR_3][a] : dSteelConfMin[ELE_SHEAR_3][a];
					pEleResult->m_dPreDeadSteelConf[ELE_MOMENT2][a] = fabs(dSteelConfMax[ELE_MOMENT2][a]) > fabs(dSteelConfMin[ELE_MOMENT2][a]) ? dSteelConfMax[ELE_MOMENT2][a] : dSteelConfMin[ELE_MOMENT2][a];
					pEleResult->m_dPreDeadSteelConf[ELE_AXTORSN][a] = fabs(dSteelConfMax[ELE_AXTORSN][a]) > fabs(dSteelConfMin[ELE_AXTORSN][a]) ? dSteelConfMax[ELE_AXTORSN][a] : dSteelConfMin[ELE_AXTORSN][a];
				}				
			}
		}
	}
	if(pDB->IsTUGir())
	{
		pFemData	= m_pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);
		pResult		= m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS);

		if(pFemData->GetTotalResultSize(ELM_SIZE) > 0)
		{
			POSITION pos = pFemData->m_ElementList.GetHeadPosition();
			long	nIndex	= 0;//Loadcase
			long	nElem	= 0;
			while(pos)
			{
				nIndex = 0;//Loadcase-steel
				CElement	*pE			= (CElement*)pFemData->m_ElementList.GetNext(pos);
				CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nElem++);
				CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
				long a= 0;
				for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
				{
					pEleResult->m_dPreDeadConf[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
					pEleResult->m_dPreDeadConf[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
					pEleResult->m_dPreDeadConf[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
					pEleResult->m_dPreDeadConf[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
					pEleResult->m_dPreDeadConf[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
					pEleResult->m_dPreDeadConf[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
				}
				
				nIndex = 1;//concreate
				if(pDB->m_nQtyJigan<2)	// ���ӱ����� turning ����
				{
					pResultForce	= pE->GetResultForce(nIndex);
					for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
					{
						CResultForceElementData *pResultEleData = pResultForce->m_ResultForceElement[a];
						pEleResult->m_dPreDeadTurning[ELE_SHEAR_2][a] = pResultEleData->m_dForceOnElement[0];//GetForce(a, 0);
						pEleResult->m_dPreDeadTurning[ELE_MOMENT3][a] = pResultEleData->m_dForceOnElement[1];//GetForce(a, 1);
						pEleResult->m_dPreDeadTurning[ELE_AXFORCE][a] = pResultEleData->m_dForceOnElement[2];//GetForce(a, 2);
						pEleResult->m_dPreDeadTurning[ELE_SHEAR_3][a] = pResultEleData->m_dForceOnElement[3];//GetForce(a, 3);
						pEleResult->m_dPreDeadTurning[ELE_MOMENT2][a] = pResultEleData->m_dForceOnElement[4];//GetForce(a, 4);
						pEleResult->m_dPreDeadTurning[ELE_AXTORSN][a] = pResultEleData->m_dForceOnElement[5];//GetForce(a, 5);
					}

					nIndex++;
				}
				
				pResultForce	= pE->GetResultForce(nIndex);
				for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
				{
					CResultForceElementData *pResultEleData = pResultForce->m_ResultForceElement[a];
					pEleResult->m_dPreDeadSlab[ELE_SHEAR_2][a] = pResultEleData->m_dForceOnElement[0];//GetForce(a, 0);
					pEleResult->m_dPreDeadSlab[ELE_MOMENT3][a] = pResultEleData->m_dForceOnElement[1];//GetForce(a, 1);
					pEleResult->m_dPreDeadSlab[ELE_AXFORCE][a] = pResultEleData->m_dForceOnElement[2];//GetForce(a, 2);
					pEleResult->m_dPreDeadSlab[ELE_SHEAR_3][a] = pResultEleData->m_dForceOnElement[3];//GetForce(a, 3);
					pEleResult->m_dPreDeadSlab[ELE_MOMENT2][a] = pResultEleData->m_dForceOnElement[4];//GetForce(a, 4);
					pEleResult->m_dPreDeadSlab[ELE_AXTORSN][a] = pResultEleData->m_dForceOnElement[5];//GetForce(a, 5);
				}
			}
		}
	}

	//�ռ��� ��������
	pFemData	= m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
	pResult		= m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
	if(pFemData->GetTotalResultSize(ELM_SIZE) > 0)
	{
		POSITION pos = pFemData->m_ElementList.GetHeadPosition();
		long	nIndex	= 0;//Loadcase
		long	nElem	= 0;
		while(pos)
		{
			CElement	*pE			= (CElement*)pFemData->m_ElementList.GetNext(pos);
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nElem++);
			CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);

			for(long a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				pEleResult->m_dPostDead[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
				pEleResult->m_dPostDead[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
				pEleResult->m_dPostDead[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
				pEleResult->m_dPostDead[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
				pEleResult->m_dPostDead[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
				pEleResult->m_dPostDead[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
			}
		}
	}

	//����ħ��
	pFemData	= m_pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS);
	pResult		= m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);
	if(pFemData->GetTotalResultSize(ELM_SIZE) > 0)
	{
		POSITION pos = pFemData->m_ElementList.GetHeadPosition();
		long nElem = 0;
		while(pos)
		{
			long nIndex = pFemData->GetLoadCaseSize()-2;
			CElement	*pE = (CElement *)pFemData->m_ElementList.GetNext(pos);		
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nElem++);
			//Max
			CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
			long a = 0;
			for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				pEleResult->m_dUnSettleMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
				pEleResult->m_dUnSettleMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
				pEleResult->m_dUnSettleMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
				pEleResult->m_dUnSettleMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
				pEleResult->m_dUnSettleMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
				pEleResult->m_dUnSettleMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
			}
			//Min
			nIndex = pFemData->GetLoadCaseSize()-1;
			pResultForce	= pE->GetResultForce(nIndex);
			for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				pEleResult->m_dUnSettleMin[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
				pEleResult->m_dUnSettleMin[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
				pEleResult->m_dUnSettleMin[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
				pEleResult->m_dUnSettleMin[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
				pEleResult->m_dUnSettleMin[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
				pEleResult->m_dUnSettleMin[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
			}
		}
	}

	//Ȱ����
	pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	pResult		= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);

	if(pFemData->GetTotalResultSize(ELM_SIZE) > 0)
	{
		POSITION pos = pFemData->m_ElementList.GetHeadPosition();
		long nElem = 0;
		while(pos)
		{
			CElement	*pE			= (CElement *)pFemData->m_ElementList.GetNext(pos);		
			CElementResult	*pEleResult	= pResult->m_EleResultArr.GetAt(nElem++);

			long nIndex = pE->m_ResultForce.GetSize()-2;
			//Max
			CResultForceElement	*pResultForce	= pE->GetResultForce(nIndex);
			long a = 0;
			for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				pEleResult->m_dLiveMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
				pEleResult->m_dLiveMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
				pEleResult->m_dLiveMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
				pEleResult->m_dLiveMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
				pEleResult->m_dLiveMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
				pEleResult->m_dLiveMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
			}
			//Min
			nIndex = pE->m_ResultForce.GetSize()-1;
			pResultForce	= pE->GetResultForce(nIndex);
			for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				CResultForceElementData *pResultEleData = pResultForce->m_ResultForceElement[a];
				pEleResult->m_dLiveMin[ELE_SHEAR_2][a] = pResultEleData->m_dForceOnElement[0];//GetForce(a, 0);
				pEleResult->m_dLiveMin[ELE_MOMENT3][a] = pResultEleData->m_dForceOnElement[1];//GetForce(a, 1);
				pEleResult->m_dLiveMin[ELE_AXFORCE][a] = pResultEleData->m_dForceOnElement[2];//GetForce(a, 2);
				pEleResult->m_dLiveMin[ELE_SHEAR_3][a] = pResultEleData->m_dForceOnElement[3];//GetForce(a, 3);
				pEleResult->m_dLiveMin[ELE_MOMENT2][a] = pResultEleData->m_dForceOnElement[4];//GetForce(a, 4);
				pEleResult->m_dLiveMin[ELE_AXTORSN][a] = pResultEleData->m_dForceOnElement[5];//GetForce(a, 5);
			}
		}
	}
	//Ȱ����(ǳ���� ����)
	pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	pResult		= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);

	if(pFemData->GetTotalResultSize(ELM_SIZE) > 0)
	{
		long nSIndexMax = pFemData->GetLoadCaseIndex("PEOCAR MAX");
		if(nSIndexMax<0) nSIndexMax = pFemData->GetLoadCaseIndex("CAR MAX");
		long nSIndexMin = pFemData->GetLoadCaseIndex("PEOCAR MIN");
		if(nSIndexMin<0) nSIndexMin = pFemData->GetLoadCaseIndex("CAR MIN");

		POSITION pos = pFemData->m_ElementList.GetHeadPosition();
		long nElem = 0;
		while(pos)
		{
			CElement *pE = (CElement *)pFemData->m_ElementList.GetNext(pos);
			CElementResult *pEleResult = pResult->m_EleResultArr.GetAt(nElem++);
			//Max
			CResultForceElement	*pResultForce	= pE->GetResultForce(nSIndexMax);
			long a = 0;
			for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				pEleResult->m_dOnlyLiveMax[ELE_SHEAR_2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[0];//GetForce(a, 0);
				pEleResult->m_dOnlyLiveMax[ELE_MOMENT3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[1];//GetForce(a, 1);
				pEleResult->m_dOnlyLiveMax[ELE_AXFORCE][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[2];//GetForce(a, 2);
				pEleResult->m_dOnlyLiveMax[ELE_SHEAR_3][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[3];//GetForce(a, 3);
				pEleResult->m_dOnlyLiveMax[ELE_MOMENT2][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[4];//GetForce(a, 4);
				pEleResult->m_dOnlyLiveMax[ELE_AXTORSN][a] = pResultForce->m_ResultForceElement[a]->m_dForceOnElement[5];//GetForce(a, 5);
			}
			//Min			
			pResultForce	= pE->GetResultForce(nSIndexMin);
			for(a=0; a<pResultForce->m_ResultForceElement.GetSize(); a++)
			{
				CResultForceElementData *pResultEleData = pResultForce->m_ResultForceElement[a];
				pEleResult->m_dOnlyLiveMin[ELE_SHEAR_2][a] = pResultEleData->m_dForceOnElement[0];//GetForce(a, 0);
				pEleResult->m_dOnlyLiveMin[ELE_MOMENT3][a] = pResultEleData->m_dForceOnElement[1];//GetForce(a, 1);
				pEleResult->m_dOnlyLiveMin[ELE_AXFORCE][a] = pResultEleData->m_dForceOnElement[2];//GetForce(a, 2);
				pEleResult->m_dOnlyLiveMin[ELE_SHEAR_3][a] = pResultEleData->m_dForceOnElement[3];//GetForce(a, 3);
				pEleResult->m_dOnlyLiveMin[ELE_MOMENT2][a] = pResultEleData->m_dForceOnElement[4];//GetForce(a, 4);
				pEleResult->m_dOnlyLiveMin[ELE_AXTORSN][a] = pResultEleData->m_dForceOnElement[5];//GetForce(a, 5);
			}
		}
	}

	if(pDB->IsTUGir())
	{
		CFEMManage	*pFemPre	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

		CResultArray	*pPreResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
		CResultArray	*pConfResult= m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS);
		CResultArray	*pPosResult	= m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
		CResultArray	*pLivResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
		CResultArray	*pSetResult	= m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);

		for(long nE=0; nE<pFemPre->GetElementSize(); nE++)
		{
			long nIndex = pFemPre->GetLoadCaseSize()-1;
			CElement *pEPre = (CElement *)pFemPre->GetElement((unsigned short)nE);

			CElementResult *pEPreResult = (CElementResult *)pPreResult->m_EleResultArr.GetAt(nE);
			CElementResult *pEConfResult= (CElementResult *)pConfResult->m_EleResultArr.GetAt(nE);
			CElementResult *pEPosResult = (CElementResult *)pPosResult->m_EleResultArr.GetAt(nE);
			CElementResult *pELivResult = (CElementResult *)pLivResult->m_EleResultArr.GetAt(nE);
			CElementResult *pESetResult = (CElementResult *)pSetResult->m_EleResultArr.GetAt(nE);

			if (pEPre->m_ResultForce.GetSize()<1) continue;

			CResultForceElement* pResult = (CResultForceElement*)pEPre->m_ResultForce.GetAt(nIndex);
	
			for(long nSeg=0; nSeg<pEPre->m_ResultForce.GetAt(nIndex)->m_ResultForceElement.GetSize(); nSeg++)
			{
				double	dDeadLoadM3	= pEPreResult->m_dPreDeadSteel[ELE_MOMENT3][nSeg] + pEConfResult->m_dPreDeadConf[ELE_MOMENT3][nSeg] + pEConfResult->m_dPreDeadTurning[ELE_MOMENT3][nSeg] + pEConfResult->m_dPreDeadSlab[ELE_MOMENT3][nSeg] + pEPosResult->m_dPostDead[ELE_MOMENT3][nSeg];
				double	dDeadLoadS3	= pEPreResult->m_dPreDeadSteel[ELE_SHEAR_3][nSeg] + pEConfResult->m_dPreDeadConf[ELE_SHEAR_3][nSeg] + pEConfResult->m_dPreDeadTurning[ELE_SHEAR_3][nSeg] + pEConfResult->m_dPreDeadSlab[ELE_SHEAR_3][nSeg] + pEPosResult->m_dPostDead[ELE_SHEAR_3][nSeg];
				double	dDeadLoadAT	= pEPreResult->m_dPreDeadSteel[ELE_AXTORSN][nSeg] + pEConfResult->m_dPreDeadConf[ELE_AXTORSN][nSeg] + pEConfResult->m_dPreDeadTurning[ELE_AXTORSN][nSeg] + pEConfResult->m_dPreDeadSlab[ELE_AXTORSN][nSeg] + pEPosResult->m_dPostDead[ELE_AXTORSN][nSeg];
				
				pEPreResult->m_dTotalMax[ELE_MOMENT3][nSeg] = dDeadLoadM3 + pELivResult->m_dLiveMax[ELE_MOMENT3][nSeg] + pESetResult->m_dUnSettleMax[ELE_MOMENT3][nSeg];
				pEPreResult->m_dTotalMin[ELE_MOMENT3][nSeg] = dDeadLoadM3 + pELivResult->m_dLiveMin[ELE_MOMENT3][nSeg] + pESetResult->m_dUnSettleMin[ELE_MOMENT3][nSeg];
				pEPreResult->m_dTotalMax[ELE_SHEAR_3][nSeg] = dDeadLoadS3 + pELivResult->m_dLiveMax[ELE_SHEAR_3][nSeg] + pESetResult->m_dUnSettleMax[ELE_SHEAR_3][nSeg];
				pEPreResult->m_dTotalMin[ELE_SHEAR_3][nSeg] = dDeadLoadS3 + pELivResult->m_dLiveMin[ELE_SHEAR_3][nSeg] + pESetResult->m_dUnSettleMin[ELE_SHEAR_3][nSeg];
				pEPreResult->m_dTotalMax[ELE_AXTORSN][nSeg] = dDeadLoadAT + pELivResult->m_dLiveMax[ELE_AXTORSN][nSeg] + pESetResult->m_dUnSettleMax[ELE_AXTORSN][nSeg];
				pEPreResult->m_dTotalMin[ELE_AXTORSN][nSeg] = dDeadLoadAT + pELivResult->m_dLiveMin[ELE_AXTORSN][nSeg] + pESetResult->m_dUnSettleMin[ELE_AXTORSN][nSeg];

				double	dDeadLoadM2	= pEPreResult->m_dPreDeadSteel[ELE_MOMENT2][nSeg] + pEConfResult->m_dPreDeadConf[ELE_MOMENT2][nSeg] + pEConfResult->m_dPreDeadTurning[ELE_MOMENT2][nSeg] + pEConfResult->m_dPreDeadSlab[ELE_MOMENT2][nSeg] + pEPosResult->m_dPostDead[ELE_MOMENT2][nSeg];
				double	dDeadLoadS2	= pEPreResult->m_dPreDeadSteel[ELE_SHEAR_2][nSeg] + pEConfResult->m_dPreDeadConf[ELE_SHEAR_2][nSeg] + pEConfResult->m_dPreDeadTurning[ELE_SHEAR_2][nSeg] + pEConfResult->m_dPreDeadSlab[ELE_SHEAR_2][nSeg] + pEPosResult->m_dPostDead[ELE_SHEAR_2][nSeg];
				double	dDeadLoadAX	= pEPreResult->m_dPreDeadSteel[ELE_AXFORCE][nSeg] + pEConfResult->m_dPreDeadConf[ELE_AXFORCE][nSeg] + pEConfResult->m_dPreDeadTurning[ELE_AXFORCE][nSeg] + pEConfResult->m_dPreDeadSlab[ELE_AXFORCE][nSeg] + pEPosResult->m_dPostDead[ELE_AXFORCE][nSeg];

				pEPreResult->m_dTotalMax[ELE_MOMENT2][nSeg] = dDeadLoadM2 + pELivResult->m_dLiveMax[ELE_MOMENT2][nSeg] + pESetResult->m_dUnSettleMax[ELE_MOMENT2][nSeg];
				pEPreResult->m_dTotalMin[ELE_MOMENT2][nSeg] = dDeadLoadM2 + pELivResult->m_dLiveMin[ELE_MOMENT2][nSeg] + pESetResult->m_dUnSettleMin[ELE_MOMENT2][nSeg];
				pEPreResult->m_dTotalMax[ELE_SHEAR_2][nSeg] = dDeadLoadS2 + pELivResult->m_dLiveMax[ELE_SHEAR_2][nSeg] + pESetResult->m_dUnSettleMax[ELE_SHEAR_2][nSeg];
				pEPreResult->m_dTotalMin[ELE_SHEAR_2][nSeg] = dDeadLoadS2 + pELivResult->m_dLiveMin[ELE_SHEAR_2][nSeg] + pESetResult->m_dUnSettleMin[ELE_SHEAR_2][nSeg];
				pEPreResult->m_dTotalMax[ELE_AXFORCE][nSeg] = dDeadLoadAX + pELivResult->m_dLiveMax[ELE_AXFORCE][nSeg] + pESetResult->m_dUnSettleMax[ELE_AXFORCE][nSeg];
				pEPreResult->m_dTotalMin[ELE_AXFORCE][nSeg] = dDeadLoadAX + pELivResult->m_dLiveMin[ELE_AXFORCE][nSeg] + pESetResult->m_dUnSettleMin[ELE_AXFORCE][nSeg];
			}
		}
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////////////
		// �հ� [�հ迡 ���� �����ʹ� BEFORE_COMPOSITE_ANALYSIS�� ������
		CFEMManage* pFemPre = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

		CResultArray* pPreResult = m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
		CResultArray* pPosResult = m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
		CResultArray* pLivResult = m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
		CResultArray* pSetResult = m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);

		long nSizePreResult = pPreResult->m_EleResultArr.GetSize();
		long nSizePosResult = pPosResult->m_EleResultArr.GetSize();
		long nSizeLivResult = pLivResult->m_EleResultArr.GetSize();
		long nSizeSetResult = pSetResult->m_EleResultArr.GetSize();
		long nSizeEleResult = MIN(nSizePreResult, MIN(nSizePosResult, MIN(nSizeLivResult,nSizeSetResult)));
		for(long nE=0; nE<nSizeEleResult; nE++)
		{
			long nIndex = pFemPre->GetLoadCaseSize()-2;
			CElement *pEPre = (CElement*)pFemPre->GetElement((unsigned short)nE);

			CElementResult *pEPreResult = (CElementResult*)pPreResult->m_EleResultArr.GetAt(nE);
			CElementResult *pEPosResult = (CElementResult*)pPosResult->m_EleResultArr.GetAt(nE);
			CElementResult *pELivResult = (CElementResult*)pLivResult->m_EleResultArr.GetAt(nE);
			CElementResult *pESetResult = (CElementResult*)pSetResult->m_EleResultArr.GetAt(nE);

			long nSize	= pEPre->m_ResultForce.GetSize();
			if (nSize<1) continue;

			for(long nSeg=0; nSeg<pEPre->m_ResultForce.GetAt(nIndex)->m_ResultForceElement.GetSize(); nSeg++)
			{
				pEPreResult->m_dTotalMax[ELE_MOMENT3][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_MOMENT3][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_MOMENT3][nSeg] + pEPosResult->m_dPostDead[ELE_MOMENT3][nSeg] + pELivResult->m_dLiveMax[ELE_MOMENT3][nSeg] + pESetResult->m_dUnSettleMax[ELE_MOMENT3][nSeg];
				pEPreResult->m_dTotalMin[ELE_MOMENT3][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_MOMENT3][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_MOMENT3][nSeg] + pEPosResult->m_dPostDead[ELE_MOMENT3][nSeg] + pELivResult->m_dLiveMin[ELE_MOMENT3][nSeg] + pESetResult->m_dUnSettleMin[ELE_MOMENT3][nSeg];
				pEPreResult->m_dTotalMax[ELE_SHEAR_3][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_SHEAR_3][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_SHEAR_3][nSeg] + pEPosResult->m_dPostDead[ELE_SHEAR_3][nSeg] + pELivResult->m_dLiveMax[ELE_SHEAR_3][nSeg] + pESetResult->m_dUnSettleMax[ELE_SHEAR_3][nSeg];
				pEPreResult->m_dTotalMin[ELE_SHEAR_3][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_SHEAR_3][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_SHEAR_3][nSeg] + pEPosResult->m_dPostDead[ELE_SHEAR_3][nSeg] + pELivResult->m_dLiveMin[ELE_SHEAR_3][nSeg] + pESetResult->m_dUnSettleMin[ELE_SHEAR_3][nSeg];
				pEPreResult->m_dTotalMax[ELE_AXTORSN][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_AXTORSN][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_AXTORSN][nSeg] + pEPosResult->m_dPostDead[ELE_AXTORSN][nSeg] + pELivResult->m_dLiveMax[ELE_AXTORSN][nSeg] + pESetResult->m_dUnSettleMax[ELE_AXTORSN][nSeg];
				pEPreResult->m_dTotalMin[ELE_AXTORSN][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_AXTORSN][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_AXTORSN][nSeg] + pEPosResult->m_dPostDead[ELE_AXTORSN][nSeg] + pELivResult->m_dLiveMin[ELE_AXTORSN][nSeg] + pESetResult->m_dUnSettleMin[ELE_AXTORSN][nSeg];
				pEPreResult->m_dTotalMax[ELE_MOMENT2][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_MOMENT2][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_MOMENT2][nSeg] + pEPosResult->m_dPostDead[ELE_MOMENT2][nSeg] + pELivResult->m_dLiveMax[ELE_MOMENT2][nSeg] + pESetResult->m_dUnSettleMax[ELE_MOMENT2][nSeg];
				pEPreResult->m_dTotalMin[ELE_MOMENT2][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_MOMENT2][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_MOMENT2][nSeg] + pEPosResult->m_dPostDead[ELE_MOMENT2][nSeg] + pELivResult->m_dLiveMin[ELE_MOMENT2][nSeg] + pESetResult->m_dUnSettleMin[ELE_MOMENT2][nSeg];
				pEPreResult->m_dTotalMax[ELE_SHEAR_2][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_SHEAR_2][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_SHEAR_2][nSeg] + pEPosResult->m_dPostDead[ELE_SHEAR_2][nSeg] + pELivResult->m_dLiveMax[ELE_SHEAR_2][nSeg] + pESetResult->m_dUnSettleMax[ELE_SHEAR_2][nSeg];
				pEPreResult->m_dTotalMin[ELE_SHEAR_2][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_SHEAR_2][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_SHEAR_2][nSeg] + pEPosResult->m_dPostDead[ELE_SHEAR_2][nSeg] + pELivResult->m_dLiveMin[ELE_SHEAR_2][nSeg] + pESetResult->m_dUnSettleMin[ELE_SHEAR_2][nSeg];
				pEPreResult->m_dTotalMax[ELE_AXFORCE][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_AXFORCE][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_AXFORCE][nSeg] + pEPosResult->m_dPostDead[ELE_AXFORCE][nSeg] + pELivResult->m_dLiveMax[ELE_AXFORCE][nSeg] + pESetResult->m_dUnSettleMax[ELE_AXFORCE][nSeg];
				pEPreResult->m_dTotalMin[ELE_AXFORCE][nSeg] = pEPreResult->m_dPreDeadSlab[ELE_AXFORCE][nSeg] + pEPreResult->m_dPreDeadSteel[ELE_AXFORCE][nSeg] + pEPosResult->m_dPostDead[ELE_AXFORCE][nSeg] + pELivResult->m_dLiveMin[ELE_AXFORCE][nSeg] + pESetResult->m_dUnSettleMin[ELE_AXFORCE][nSeg];
			}
		}
	}
}

void CFemModelingCalc::MakeSummaryOfJointForce()
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CFEMManage	*pFemData	= NULL;
	CResultArray	*pResult	= NULL;

	long	nJoint	= 0;
	if(pDB->IsTUGir())
	{
		//�ռ��� ��������
		pFemData = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
		pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);

		if(pFemData->GetTotalResultSize(NODE_SIZE) > 0)
		{
			POSITION pos = pFemData->m_JointList.GetHeadPosition();
			long nIndex = 0;//Loadcase
			while(pos)
			{
				nIndex = 0;//Loadcase-steel
				CJoint		*pJoint = (CJoint *)pFemData->m_JointList.GetNext(pos);
				CJointResult	*pJointResult = pResult->m_JointResultArr.GetAt(nJoint++);
				pJointResult->m_vPreDeadSteel[JOINT_REACTION]		= pJoint->GetResultForce(nIndex)->m_Force;
				pJointResult->m_vPreDeadSteel[JOINT_DISPLACEMENT]	= pJoint->GetResultForce(nIndex)->m_Disp;

				if(pDB->IsTUContinuousSpan())
				{
					nIndex++;
					pJointResult->m_vPreDeadConf2[JOINT_REACTION]			= pJoint->GetResultForce(nIndex)->m_Force;
					pJointResult->m_vPreDeadConf2[JOINT_DISPLACEMENT]		= pJoint->GetResultForce(nIndex)->m_Disp;
					nIndex++;
					pJointResult->m_vPreDeadSteelConf[JOINT_REACTION]		= pJoint->GetResultForce(nIndex)->m_Force;
					pJointResult->m_vPreDeadSteelConf[JOINT_DISPLACEMENT]	= pJoint->GetResultForce(nIndex)->m_Disp;
				}
			}	
		}

		pFemData	= m_pCalcStd->GetModelingFemData(BEFORE_TURN_COMPOSITE_ANALYSIS);
		pResult		= m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS);
		if(pFemData->GetTotalResultSize(NODE_SIZE) > 0)
		{
			POSITION pos = pFemData->m_JointList.GetHeadPosition();
			long nIndex = 0;//Loadcase
			nJoint = 0;
			while(pos)
			{
				CJoint			*pJoint			= (CJoint *)pFemData->m_JointList.GetNext(pos);
				CJointResult	*pJointResult	= pResult->m_JointResultArr.GetAt(nJoint++);

				nIndex = 0;//concreate
				pJointResult->m_vPreDeadConf[JOINT_REACTION]		= pJoint->GetResultForce(nIndex)->m_Force;
				pJointResult->m_vPreDeadConf[JOINT_DISPLACEMENT]	= pJoint->GetResultForce(nIndex)->m_Disp;
				nIndex++;

				if(!pDB->IsTUContinuousSpan())
				{
					//Loadcase-turning
					pJointResult->m_vPreDeadTurning[JOINT_REACTION]		= pJoint->GetResultForce(nIndex)->m_Force;
					pJointResult->m_vPreDeadTurning[JOINT_DISPLACEMENT] = pJoint->GetResultForce(nIndex)->m_Disp;
					nIndex++;
				}

				//concreate
				pJointResult->m_vPreDeadSlab[JOINT_REACTION]		= pJoint->m_ResultForce.GetAt(nIndex)->m_Force;
				pJointResult->m_vPreDeadSlab[JOINT_DISPLACEMENT]	= pJoint->m_ResultForce.GetAt(nIndex)->m_Disp;
			}	
		}
	}
	else
	{
		//�ռ��� ��������
		pFemData	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
		pResult		= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
		if(pFemData->GetTotalResultSize(NODE_SIZE) > 0)
		{
			POSITION pos = pFemData->m_JointList.GetHeadPosition();
			long nIndex = 0;//Loadcase
			nJoint = 0;
			while(pos)
			{
				nIndex = 0;//Loadcase-steel
				CJoint		*pJoint			= (CJoint*)pFemData->m_JointList.GetNext(pos);		
				CJointResult	*pJointResult	= pResult->m_JointResultArr.GetAt(nJoint++);
				pJointResult->m_vPreDeadSteel[JOINT_REACTION]		= pJoint->GetResultForce(nIndex)->m_Force;
				pJointResult->m_vPreDeadSteel[JOINT_DISPLACEMENT] = pJoint->GetResultForce(nIndex)->m_Disp;

				nIndex = 1;//concreate
				pJointResult->m_vPreDeadSlab[JOINT_REACTION]		= pJoint->m_ResultForce.GetAt(nIndex)->m_Force;
				pJointResult->m_vPreDeadSlab[JOINT_DISPLACEMENT]  = pJoint->m_ResultForce.GetAt(nIndex)->m_Disp;
			}	
		}
	}
	//�ռ��� ��������
	pFemData	= m_pCalcStd->GetModelingFemData(AFTER_COMPOSITE_ANALYSIS);
	pResult		= m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
	if(pFemData->GetTotalResultSize(NODE_SIZE) > 0)
	{
		POSITION pos = pFemData->m_JointList.GetHeadPosition();
		long nIndex = 0;//Loadcase
		nJoint = 0;
		while(pos)
		{
			CJoint		*pJoint			= (CJoint *)pFemData->m_JointList.GetNext(pos);		
			CJointResult	*pJointResult	= pResult->m_JointResultArr.GetAt(nJoint++);
			pJointResult->m_vPostDead[JOINT_REACTION] = pJoint->m_ResultForce.GetAt(nIndex)->m_Force;
			pJointResult->m_vPostDead[JOINT_DISPLACEMENT]  = pJoint->m_ResultForce.GetAt(nIndex)->m_Disp;
		}
	}
	
	//Ȱ����
	pFemData	= m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	pResult		= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
	if(pFemData->GetTotalResultSize(NODE_SIZE) > 0)
	{
		POSITION pos = pFemData->m_JointList.GetHeadPosition();
		nJoint = 0;
		while(pos)
		{
			long nIndex = pFemData->GetLoadCaseSize()-2;
			CJoint *pJoint = (CJoint *)pFemData->m_JointList.GetNext(pos);
			CJointResult	*pJointResult	= pResult->m_JointResultArr.GetAt(nJoint++);
			//Max
			pJointResult->m_vLiveMax[JOINT_REACTION]		= pJoint->m_ResultForce.GetAt(nIndex)->m_Force;
			pJointResult->m_vLiveMax[JOINT_DISPLACEMENT]	= pJoint->m_ResultForce.GetAt(nIndex)->m_Disp;
			//Min
			nIndex = pFemData->GetLoadCaseSize()-1;
			pJointResult->m_vLiveMin[JOINT_REACTION]		= pJoint->m_ResultForce.GetAt(nIndex)->m_Force;
			pJointResult->m_vLiveMin[JOINT_DISPLACEMENT]	= pJoint->m_ResultForce.GetAt(nIndex)->m_Disp;
		}
	}
	//����ħ��
	pFemData	= m_pCalcStd->GetModelingFemData(SETTLEMENT_ANALYSIS);
	pResult		= m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);
	if(pFemData->GetTotalResultSize(NODE_SIZE) > 0)
	{
		POSITION pos = pFemData->m_JointList.GetHeadPosition();
		nJoint = 0;
		while(pos)
		{
			long nIndex = pFemData->GetLoadCaseSize()-2;
			CJoint *pJoint = (CJoint *)pFemData->m_JointList.GetNext(pos);
			CJointResult	*pJointResult	= pResult->m_JointResultArr.GetAt(nJoint++);
			//Max
			pJointResult->m_vUnSettleMax[JOINT_REACTION]      = pJoint->m_ResultForce.GetAt(nIndex)->m_Force;
			pJointResult->m_vUnSettleMax[JOINT_DISPLACEMENT]  = pJoint->m_ResultForce.GetAt(nIndex)->m_Disp;
			//Min
			nIndex = pFemData->GetLoadCaseSize()-1;
			pJointResult->m_vUnSettleMin[JOINT_REACTION]      = pJoint->m_ResultForce.GetAt(nIndex)->m_Force;
			pJointResult->m_vUnSettleMin[JOINT_DISPLACEMENT]  = pJoint->m_ResultForce.GetAt(nIndex)->m_Disp;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// �հ� [�հ迡 ���� �����ʹ� BEFORE_COMPOSITE_ANALYSIS�� ������
	if(pDB->IsTUGir())
	{
		CFEMManage	*pFemPre	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

		CResultArray	*pPreResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
		CResultArray	*pTurnResult= m_pCalcStd->GetModelingResult(BEFORE_TURN_COMPOSITE_ANALYSIS);
		CResultArray	*pPosResult	= m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
		CResultArray	*pLivResult	= m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
		CResultArray	*pSetResult	= m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);

		for(long nJ=0; nJ<pFemPre->GetJointSize(); nJ++)
		{
			CJointResult	*pJPre	= pPreResult->m_JointResultArr.GetAt(nJ);
			CJointResult	*pJTurn	= pTurnResult->m_JointResultArr.GetAt(nJ);
			CJointResult	*pJPos	= pPosResult->m_JointResultArr.GetAt(nJ);
			CJointResult	*pJLiv	= pLivResult->m_JointResultArr.GetAt(nJ);
			CJointResult	*pJSet	= pSetResult->m_JointResultArr.GetAt(nJ);

			CVector	vReaction	= pJPre->m_vPreDeadSteel[JOINT_REACTION]+pJPre->m_vPreDeadConf2[JOINT_REACTION]+pJPre->m_vPreDeadSteelConf[JOINT_REACTION]
				+pJTurn->m_vPreDeadConf[JOINT_REACTION]+pJTurn->m_vPreDeadTurning[JOINT_REACTION]+pJTurn->m_vPreDeadSlab[JOINT_REACTION]+pJPos->m_vPostDead[JOINT_REACTION];
			CVector	vDisp		= pJPre->m_vPreDeadSteel[JOINT_DISPLACEMENT]+pJPre->m_vPreDeadConf2[JOINT_DISPLACEMENT]+pJPre->m_vPreDeadSteelConf[JOINT_DISPLACEMENT]
				+pJTurn->m_vPreDeadConf[JOINT_DISPLACEMENT]+pJTurn->m_vPreDeadTurning[JOINT_DISPLACEMENT]+pJTurn->m_vPreDeadSlab[JOINT_DISPLACEMENT]+pJPos->m_vPostDead[JOINT_DISPLACEMENT];
			pJPre->m_vTotalMax[JOINT_REACTION] = vReaction+pJLiv->m_vLiveMax[JOINT_REACTION]+pJSet->m_vUnSettleMax[JOINT_REACTION];
			pJPre->m_vTotalMin[JOINT_REACTION] = vReaction+pJLiv->m_vLiveMin[JOINT_REACTION]+pJSet->m_vUnSettleMin[JOINT_REACTION];
			pJPre->m_vTotalMax[JOINT_DISPLACEMENT] = vDisp+pJLiv->m_vLiveMax[JOINT_DISPLACEMENT]+pJSet->m_vUnSettleMax[JOINT_DISPLACEMENT];
			pJPre->m_vTotalMin[JOINT_DISPLACEMENT] = vDisp+pJLiv->m_vLiveMin[JOINT_DISPLACEMENT]+pJSet->m_vUnSettleMin[JOINT_DISPLACEMENT];
		}
	}
	else
	{
		CFEMManage* pFemPre = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

		CResultArray* pPreResult = m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
		CResultArray* pPosResult = m_pCalcStd->GetModelingResult(AFTER_COMPOSITE_ANALYSIS);
		CResultArray* pLivResult = m_pCalcStd->GetModelingResult(LIVE_LOAD_ANALYSIS);
		CResultArray* pSetResult = m_pCalcStd->GetModelingResult(SETTLEMENT_ANALYSIS);

		long nSizePreResult = pPreResult->m_JointResultArr.GetSize();
		long nSizePosResult = pPosResult->m_JointResultArr.GetSize();
		long nSizeLivResult = pLivResult->m_JointResultArr.GetSize();
		long nSizeSetResult = pSetResult->m_JointResultArr.GetSize();
		long nSizeJointResult = MIN(nSizePreResult,MIN(nSizePosResult,MIN(nSizeLivResult,nSizeSetResult)));
		for(long nJ=0; nJ<nSizeJointResult; nJ++)
		{
			CJointResult *pJPre = pPreResult->m_JointResultArr.GetAt(nJ);
			CJointResult *pJPos = pPosResult->m_JointResultArr.GetAt(nJ);
			CJointResult *pJLiv = pLivResult->m_JointResultArr.GetAt(nJ);
			CJointResult *pJSet = pSetResult->m_JointResultArr.GetAt(nJ);

			pJPre->m_vTotalMax[JOINT_REACTION] = pJPre->m_vPreDeadSlab[JOINT_REACTION]+pJPre->m_vPreDeadSteel[JOINT_REACTION]+pJPos->m_vPostDead[JOINT_REACTION]+pJLiv->m_vLiveMax[JOINT_REACTION]+pJSet->m_vUnSettleMax[JOINT_REACTION];
			pJPre->m_vTotalMin[JOINT_REACTION] = pJPre->m_vPreDeadSlab[JOINT_REACTION]+pJPre->m_vPreDeadSteel[JOINT_REACTION]+pJPos->m_vPostDead[JOINT_REACTION]+pJLiv->m_vLiveMin[JOINT_REACTION]+pJSet->m_vUnSettleMin[JOINT_REACTION];
			pJPre->m_vTotalMax[JOINT_DISPLACEMENT] = pJPre->m_vPreDeadSlab[JOINT_DISPLACEMENT]+pJPre->m_vPreDeadSteel[JOINT_DISPLACEMENT]+pJPos->m_vPostDead[JOINT_DISPLACEMENT]+pJLiv->m_vLiveMax[JOINT_DISPLACEMENT]+pJSet->m_vUnSettleMax[JOINT_DISPLACEMENT];
			pJPre->m_vTotalMin[JOINT_DISPLACEMENT] = pJPre->m_vPreDeadSlab[JOINT_DISPLACEMENT]+pJPre->m_vPreDeadSteel[JOINT_DISPLACEMENT]+pJPos->m_vPostDead[JOINT_DISPLACEMENT]+pJLiv->m_vLiveMin[JOINT_DISPLACEMENT]+pJSet->m_vUnSettleMin[JOINT_DISPLACEMENT];
		}
	}
}

double CFemModelingCalc::GetValueOfInfK(long nG, double dSta)
{
	if(m_pCalcStd->GetSizeInfluenceFemData()==0) return 0;
	if(nG==-1) return 0;
	CFEMManage      *pManage = m_pCalcStd->GetInfluenceFemData(nG);
		
	if(pManage->GetElementSize()<=0) return 0;
	double dRatio = 0;
	
	long nElm       = GetElemNoAndRatioAtInfK(nG, dSta, dRatio);
	long nLoadCase  = 0;
	CElement *pElm  = pManage->GetElement((unsigned short)nElm);
	if(pElm==NULL) return 0;

	CResultForceElement *pResult = pElm->GetResultForce(nLoadCase);
	if(pResult==NULL)	return 0;

//	double dMomentStt = pResult->m_ResultForceElement[0]->m_dForceOnElement[ELE_MOMENT3];
//	double dMomentEnd = pResult->m_ResultForceElement[pResult->m_ResultForceElement.GetUpperBound()]->m_dForceOnElement[ELE_MOMENT3];

	double dMomentStt = pResult->m_ResultForceElement[0]->m_dForceOnElement[ELE_MOMENT2];
	double dMomentEnd = pResult->m_ResultForceElement[pResult->m_ResultForceElement.GetUpperBound()]->m_dForceOnElement[ELE_MOMENT2];

	return dMomentStt + dRatio*(dMomentEnd-dMomentStt);
}

double CFemModelingCalc::GetElemByCrossBx(CPlateBasicIndex* pBx)
{
	CFEMManage *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CPlateBasicIndex *pBxComp = NULL;

	for(long nE = 0; nE < pManage->GetElementSize(); nE++)
	{
		CElement *pE = pManage->GetElement((unsigned short)nE);
		if(pE->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)
		{
			pBxComp = m_pCalcStd->GetBxByJoint(pManage->GetJoint(pE->m_nJStt));
			if(pBxComp == pBx)
				return pE->m_nIndex;
		}
	}
	
	return -1;
}
/////////////////////////////////////////////////////////
//��ҳ����� ���׸�Ʈ��ġ������ �����̼�
/////////////////////////////////////////////////////////////
double CFemModelingCalc::GetStationByElmAndRatio(long nElem, double dRatio)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	long             nG      = GetGirderNoByElemID(nElem);
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);

	long nStt=0, nEnd=0;
	GetElemRangeByGirder(nG, nStt, nEnd);
	long nElemPre = nStt - 1;
	long nElemSub = nElem - nElemPre - 1;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	long   n       = 0;
	double dSta    = 0;
	double dLength = 0;
	while(pBx)
	{
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}

		CPlateBxFinder Finder2(pGir);
		CPlateBasicIndex *pBxN = Finder2.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE, Finder.GetBxFixNext());
		while(!pBxN->IsJoint())
			pBxN = Finder2.GetBxNext();

		if(n==nElemSub)
		{
			dSta    = pBx->GetStation();
			dLength = ABS(pBxN->GetStation() - pBx->GetStation());
			break;
		}
		pBx = Finder.GetBxNext();
		n++;
	}

	return dSta + dLength*dRatio;
}
//////////////////////////////////////////////////////
//�ش� JOINT�� �����̸�
//////////////////////////////////////////////////////
BOOL CFemModelingCalc::GetJijumNameOfJoint(long nIdx, CString& szName)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CFEMManage      *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	long nGir   = GetGirderNoByJointID(nIdx);
	long nPier  = 1;

	CPlateBxFinder Finder(pDB->GetGirder(nGir));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while(pBx)
	{
		if(!pBx->IsJoint())
		{
			pBx = Finder.GetBxNext();
			continue;
		}
		if(pBx == m_pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nIdx)))
		{
			if(pBx->IsAbutStt())
				szName = "A1";
			else if(pBx->IsAbutEnd())
				szName = "A2";
			else if(pBx->IsPier())
				szName.Format("P%d", nPier);
			else
				szName = "  ";

			return TRUE;
		}
		if(pBx->IsPier())		nPier++;

		pBx = Finder.GetBxNext();
	}
	
	return FALSE;
}
//joint��ȣ�� �̿��ؼ� ����Ŵ��� ã�´�.
long CFemModelingCalc::GetGirderNoByJointID(long nJoint)
{
	CFEMManage *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	return pManage->GetJoint((unsigned short)nJoint)->m_LongAttrib[JOINT_GIR]; 
}

//Element ��ȣ�� �̿��ؼ� ����Ŵ��� ã�´�.
long CFemModelingCalc::GetGirderNoByElemID(long nElem)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	for(long n=0; n<pDB->GetGirdersu(); n++)
	{
		long nStt = 0; 
		long nEnd = 0;
		GetElemRangeByGirder(n, nStt, nEnd);
		if(nStt <= nElem && nElem <=nEnd)
			return n;
	}
	return -1;
}

//////////////////////////////////////////////////////////
/// nG�Ŵ��� ���� JOINT�� ������ JOINT
//////////////////////////////////////////////////////////
void CFemModelingCalc::GetJointRangeByGirder(long nG, long& nStt, long& nEnd)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	long nSum = 0;
	for(long n=0; n<nG; n++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(n));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
		while(pBx)
		{
			if(!pBx->IsJoint())
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			pBx = Finder.GetBxNext();
			nSum++;
		}
	}
	nStt = nSum;

	CPlateBxFinder Finder2(pDB->GetGirder(nG));
	CPlateBasicIndex *pBx = Finder2.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while(pBx)
	{
		if(!pBx->IsJoint())
		{
			pBx = Finder2.GetBxNext();
			continue;
		}
		pBx = Finder2.GetBxNext();
		nSum++;
	}
	nEnd = nSum-1;
}

//////////////////////////////////////////////////////////
/// nG�Ŵ��� ���� Elem�� ������ Elem
/// ����: �Ŵ����� ��Ҽ� 
//////////////////////////////////////////////////////////
double CFemModelingCalc::GetElemRangeByGirder(long nG, long& nStt, long& nEnd)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	long nSum = 0;
	for(long n=0; n<nG; n++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(n));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
		while(pBx)
		{
			if(!pBx->IsJoint())
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			pBx = Finder.GetBxNext();
			nSum++;
		}
	}
	nStt = nSum-nG;
	nSum = nStt;

	CPlateBxFinder Finder2(pDB->GetGirder(nG));
	CPlateBasicIndex *pBx = Finder2.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	while(pBx)
	{
		if(!pBx->IsJoint())
		{
			pBx = Finder2.GetBxNext();
			continue;
		}
		pBx = Finder2.GetBxNext();
		nSum++;
	}
	nEnd = nSum-2;
	return ABS(nEnd-nStt)+1;
}

double CFemModelingCalc::GetElemLength(long nElm, BOOL bApplyZ)
{
	CFEMManage      *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CElement *pElm  = pManage->GetElement((unsigned short)nElm);
	CJoint   *pJStt = pManage->GetJoint(pElm->m_nJStt);
	CJoint   *pJEnd = pManage->GetJoint(pElm->m_nJEnd);

	double dLengthElm = 0;
	if(bApplyZ)
		dLengthElm = ~(pJStt->m_vPoint-pJEnd->m_vPoint);
	else
	{
		pJStt->m_vPoint.z = 0;
		pJEnd->m_vPoint.z = 0;

		dLengthElm = ~(pJStt->m_vPoint-pJEnd->m_vPoint);
	}

	return dLengthElm;
}

long CFemModelingCalc::GetElemNoAndRatio(long nG, double dSta, double& dRatio)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge(); 
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	long nStt=0, nEnd=0;
	GetElemRangeByGirder(nG, nStt, nEnd);
	long nElm = nStt;
	
	double dStaStt = 0;
	double dStaEnd = 0;
	CPlateBxFinder Finder(pGir);
	CPlateBxFinder Finder2(pGir);
	CPlateBasicIndex *pBxCur	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	CPlateBasicIndex* pBxN		= NULL;
	while(pBxCur)
	{
		if(!pBxCur->IsJoint())
		{
			pBxCur = Finder.GetBxNext();
			continue;
		}
		pBxN = Finder2.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE, Finder.GetBxFixNext());
		while(!pBxN->IsJoint())
			pBxN = Finder2.GetBxNext();
		
		if(pBxCur->GetStation() <= dSta && dSta <= pBxN->GetStation())
		{
			dStaStt = pBxCur->GetStation();
			dStaEnd = pBxN->GetStation();
			break;
		}

		pBxCur = Finder.GetBxNext();
		nElm++;
	}
	if(nElm < 0) nElm = 0;
	
	dRatio = (dSta-dStaStt)/(dStaEnd-dStaStt);

	if (dRatio < 0.0 || dRatio > 1.0)
	{
		dRatio = 0;
		return nElm;
	}

	if(dRatio == 1 && nElm!=nEnd)
	{
		dRatio = 0;
		return nElm+1;
	}

	return nElm;
}

long CFemModelingCalc::GetElemNoAndRatioAtInfK(long nG, double dSta, double& dRatio)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge(); 
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);

	long   nElm    = 0;
	double dStaStt = 0;
	double dStaEnd = 0;
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBxCur	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
	CPlateBasicIndex* pBxN		= Finder.GetBxFixHead();
	while(pBxCur)
	{
		if(!pBxCur->IsJoint())
		{
			pBxCur = Finder.GetBxNext();
			continue;
		}
		CPlateBxFinder Finder2(pGir);
		pBxN = Finder2.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE, Finder.GetBxFixNext());
		while(pBxN && !pBxN->IsJoint())
			pBxN = Finder2.GetBxNext();
		if(!pBxN)	break;

		if(pBxCur->GetStation() <= dSta && dSta <= pBxN->GetStation())
		{
			dStaStt = pBxCur->GetStation();
			dStaEnd = pBxN->GetStation();
			break;
		}

		pBxCur = Finder.GetBxNext();
		nElm++;
	}
	if(nElm < 0) nElm = 0;
	
	dRatio = (dSta-dStaStt)/(dStaEnd-dStaStt);

	return nElm;
}

// �� ������ ��ҹ�ȣ�� �����.
long CFemModelingCalc::GetElemNoShoeByJijumBx(CPlateBasicIndex *pBx)
{
	CFEMManage *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	for(long nE=0; nE<pManage->GetElementSize(); nE++)
	{
		CElement *pE = pManage->GetElement((unsigned short)nE);
		if(pE->m_LongAttrib[ELEMENT_TYPE] & FRAME_SHOE_ELEMENT)
		{
			long nJStt = pE->m_nJStt;
			CPlateBasicIndex *pBxJstt = m_pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nJStt));
			if(pBx==pBxJstt)
			{
				return nE;
			}
		}

	}

	return 0;
}


//�ش� JOINT�� ������ȣ�� ã��(�� �Ŵ�����)
long CFemModelingCalc::GetJijumNoOfJoint(long nIdx)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	long nSum     = 0;
	long nGirder  = 0;
	long n		  = 0 ;
	for(n=0; n<pDB->GetGirdersu(); n++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(n));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_SPLICE);
		while(pBx)
		{
			if(!pBx->IsJoint())
			{
				pBx = Finder.GetBxNext();
				continue;
			}
			if(nSum == nIdx)
			{
				nGirder = n;
			}
			pBx = Finder.GetBxNext();
			nSum++;
		}
	}
	long nJijum = 0;
	long nCr    = 0;
	for(n=0; n<nGirder; n++)
	{
		CPlateBxFinder Finder(pDB->GetGirder(n));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			nCr++;
			pBx = Finder.GetBxNext();
		}
	}
	CPlateBxFinder Finder(pDB->GetGirder(nGirder));
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	long nCrAtGir = 0;
	while(pBx)
	{
		if(pBx->IsJijum() && nCrAtGir <= (nIdx-nCr))
			nJijum++;
		pBx = Finder.GetBxNext();
		nCrAtGir++;
	}
	return nJijum-1;

}

//	����	:	���� - �米�� ��� ������ �������� ��ġ�Ÿ��� ��Ȯ�ϰ� ã�� ���� (������꼭���� ���� ����)
//				����å - ������ ����(bEndPos=TRUE)������ �ּ� ��ġ�Ÿ��� return ��  <- ������꼭������ �����
double CFemModelingCalc::GetUseBasePitchByStation(long nG, double dSta, BOOL bEndPos)
{
	CPlateBridgeApp  *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp	 *pGir	  = pDB->GetGirder(nG);
	CPlateGirderApp	 *pCRGir  = pDB->GetGirder(-1);
	
	double	dUsePitch	= 0;
	double	dSttStation	= pGir->GetSSS(OFF_STT_GIRDER);
	double	dEleStation	= dSta;
	double	dDis		= dEleStation - dSttStation;
	double	dStudDis	= 0;

	double MinDis = pCRGir->m_pSangse->m_StudJong_MinDis;// �ּ� ��ġ�Ÿ�

	if(bEndPos)	return	MinDis;//2006.07.18 Aroad ����... bum.

	for(long n = 1; n < STUD_JONG_SU; n++)
	{
		dStudDis += pGir->m_pSangse->m_StudJong_BaseTerm[n];
		if(dStudDis >= dDis)
		{
			dUsePitch = pGir->m_pSangse->m_StudJong_BaseDis[n];
			break;
		}
	}
	
	return	dUsePitch;
}

long CFemModelingCalc::GetUseStudSuByStation(long nG, double dSta, BOOL bPositive)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);

	double	dSttStation	= pDB->GetStationBridgeStt();
	double	dEleStation	= dSta;
	double	dDis		= dEleStation - dSttStation;
	long	nTotStudsu	= 0;
	long	nStudsu		= 0;
	double	dStudDis	= 0;
	long	n			= 0;

	for(n = 0; n < STUD_JONG_SU; n++)
	{
		nTotStudsu += pGir->m_pSangse->m_StudJong_Studsu[n];
	}
	for(n = 0; n < STUD_JONG_SU; n++)
	{

		dStudDis += pGir->m_pSangse->m_StudJong_StudDis[n] * pGir->m_pSangse->m_StudJong_Studsu[n];
		if(dStudDis >= dDis)
		{
			dStudDis	-= pGir->m_pSangse->m_StudJong_StudDis[n] * pGir->m_pSangse->m_StudJong_Studsu[n];
			nStudsu		+= (int)(ABS(dStudDis - dDis) / pGir->m_pSangse->m_StudJong_StudDis[n]);
			break;
		}
		nStudsu += pGir->m_pSangse->m_StudJong_Studsu[n];
	}

	long nElemStt = 0;
	long nElemEnd = 0;
	long nTotElem = 0;
	double dRatio = 0;
	long nElem    = GetElemNoAndRatio(nG, dSta, dRatio);
	GetElemRangeByGirder(nG, nElemStt, nElemEnd);
	nTotElem = ABS(nElemEnd-nElemStt) + 1;
	if((int)(nTotElem/pDB->m_nQtyJigan) > nElem)
	{
		if(bPositive)	return nStudsu;
		else			return (nTotStudsu - nStudsu);
	}
	else
	{
		if(bPositive)	return (nTotStudsu - nStudsu);
		else			return nStudsu;
	}

}
/////////////////////////////////////////////////////////////////////////////
///nSttGirNum�Ŵ��� nEndGirNum�Ŵ����̿��� ���� ó���� ū ��
/////////////////////////////////////////////////////////////////////////////
double CFemModelingCalc::GetCalcMaxDisplace(long nSttGirNum, long nEndGirNum)
{
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	CFemModelingCalc FemModelingCalc(m_pDataManage, m_pCalcStd);

	double	dMax	= 0;
	double	dReturn = 0;
	long	nStt	= 0; 
	long	nEnd	= 0;
	long	nForce	= JOINT_DISPLACEMENT;

	for(long nG = nSttGirNum; nG < nEndGirNum+1; nG++)
	{
		GetJointRangeByGirder(nG, nStt, nEnd);
		for (long i = nStt; i < nEnd+1; i++)  
		{
			CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(i);
			dMax = max(	ABS(pJResult->m_vTotalMax[nForce].z), ABS(pJResult->m_vTotalMin[nForce].z));
			if(dMax > dReturn)	dReturn = dMax;
		}
	}

	return dReturn;
}


////////////////////////////////////////////////////////////////////////
///<�κ��� ������ ���ϱ�>
///1. ton, M ������ ���
///  �κ��� ������ = �� ���緮/�𵨸��� ���ϵ� ���� ���� 
///  �𵨸����� ���� : ����÷���, �Ϻ��÷���, ������, {���κ�, ���κ�, �������κ��߰��Ұ�}
////////////////////////////////////////////////////////////////////////
double CFemModelingCalc::CalcBonusRate()
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();	
	CPlateGirderApp	*pGir	= pDB->GetGirder(0);
	CBMCreator		*pBMStd	= m_pDataManage->GetBMStandard();
	CTStandard		*pTS	= pBMStd->m_pTStandard;
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	
	double dBonusRate = 0;
	double dTotalTon  = 0;   // �� ���緮
	double dGangjaeUnitWeight = tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dGangjae);

	///<�� ���緮 ���ϱ�
	pTS->SetPaneText(0,"���� �� �߷� �����...",TRUE);
	pBMStd->SetWorkValue(10,"",DIM_BASIC,0x00); // 10 - ��꿡���� ���......
	pBMStd->DoWork();
	pTS->SetPaneText(0,"���� �� �߷� ��� �Ϸ�.",TRUE);

	dTotalTon = pBMStd->GetTotalWeight();
	///<�𵨸��� ���ϵ� ���� ���� ���ϱ�
	///<�Ŵ�
	double dWeightGir=0, dAsStt=0, dAsEnd=0, dLength=0;
	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder	Finder(pDB->GetGirder(nG));
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING); 
		CPlateBasicIndex *pBxSec = NULL;
		while(Finder.GetBxFixNext())
		{
			pBxSec = Finder.GetBxFixNext();
			
			GetSectionCoefficient(pBx);			dAsStt = toM2(GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS));
			GetSectionCoefficient(pBxSec);		dAsEnd = toM2(GetSectionCoeffAs(BEFORE_COMPOSITE_ANALYSIS));
			
			dLength= toM(pGir->GetLengthDimType(pBx->GetStation(), pBxSec->GetStation()));
			dWeightGir += dLength * (dAsStt+dAsEnd)/2 * dGangjaeUnitWeight;

			pBx = Finder.GetBxNext();
		}
	}
	///<���κ�
	double dDis             = 0;
	double dWeightCrossBeam = 0;
	double dCrBeamAs        = 0;

	for(long n=0; n<pDB->GetGirdersu()-1; n++)
	{
		CPlateGirderApp* pGir = pDB->GetGirder(n);

		CPlateBxFinder	Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{			
			GetSectionCoefficientCR(pBx);
			dCrBeamAs = toM2(m_MapSecCoeCR["Sum_As"]);
			dDis      = toM(pGir->GetDisPlanPlateWebToWeb(pBx));
			dWeightCrossBeam += dDis*dCrBeamAs*dGangjaeUnitWeight;
			
			pBx = Finder.GetBxNext();
		}
	}

	///< Data Save
	pData->m_TotalWeight = tokN(dTotalTon*pOpt->GetAccelerationOfGravity());
	pData->m_ModelWeight = dWeightGir+dWeightCrossBeam;

	dBonusRate = pData->m_TotalWeight / pData->m_ModelWeight;
	pData->m_BonusRate = Round(dBonusRate, 6);

	return tokN(dTotalTon);
}


//////////////////////////////////////////////////////////////////////////////////////////////
///�ܸ����� �����
///  nG : �Ŵ���ȣ
///  dSta : �����̼�
///  nType: 0-����  1-����  2-������  3-������ �������� �ռ���,�ռ��� �ܸ��꿡 �ʿ��� ���
///  I33-����, I22-����
///  FrameSection::m_dA-As, m_dIyy-I33(�ռ���), Iv33(�ռ���), m_dIzz-I22(�ռ���), Iv22(�ռ���) 
///                m_dJ-K(�ռ���), Kv(�ռ���)
//////////////////////////////////////////////////////////////////////////////////////////////
// ��Ȯ���� ���� Joint �� Bx�� �����ϴ� ��ġ�϶��� pBx�� ������, �� �ܿ��� dSta���� ���;� �Ѵ�
void CFemModelingCalc::GetSectionCoefficient(CPlateBasicIndex *pBx, BOOL bEffectWidth, double dStaEffect, long nPos)
{
	GetSectionCoefficient(pBx->GetNumberGirder(), pBx->GetStation(), bEffectWidth, dStaEffect, nPos);
}

void CFemModelingCalc::GetSectionCoefficient(long nG, double dSta, BOOL bEffectWidth, double dStaEffect, long nPos)
{
	m_MapSecCoe.RemoveAll();

	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CCalcData			*pData	= m_pDataManage->GetCalcData();
	CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();

	CCalcGeneral	CalcGen(m_pDataManage);
	CFemStressCalc	StressCalc(m_pDataManage, m_pCalcStd);

	double dEst		= pData->DESIGN_MATERIAL.m_dEst;				///< ������ ź�����
	double dN		= pData->DESIGN_MATERIAL.m_nN;                  ///< ź�������(ö��/��ũ��Ʈ)
	double dBindFck	= pData->DESIGN_MATERIAL.m_dBindSigmaCK;		///< ������ũ��Ʈ Fck

//	CalcGen.GetCorrectBxIfTaper(nG, dSta);

	double dWidthUp = GetWidthFlange(nG, dSta, G_F_U, nPos);
	double dWidthDn = GetWidthFlange(nG, dSta, G_F_L, nPos);
	double dThickUp = GetThickFlange(nG, dSta, G_F_U, nPos);
	double dThickDn = GetThickFlange(nG, dSta, G_F_L, nPos);
	double dHeightWeb= GetHeightWeb(pBx, nPos);//pGir->GetHeightGirderByStaAng(dSta);
	double dThickWeb = GetThickFlange(nG, dSta, G_W, nPos);
	double dThickHunch = pDB->m_dThickSlabHunch;
	double dThickSlab  = pDB->m_dThickSlabBase-dThickHunch;
	double dSlabWidth = GetWidthSlab(nG, dStaEffect==0 ? dSta : dStaEffect);	// ������ ����
	if(bEffectWidth)
		dSlabWidth = GetEffectWidthSlab(nG, dStaEffect==0 ? dSta : dStaEffect);	// ������ ��ȿ��
	double	dNc		= pData->DESIGN_MATERIAL.m_nBindNc;
	double	dBc		= pDB->m_BindConc.m_dWidth;		// ������ũ��Ʈ ��
	double	dTc		= pDB->m_BindConc.m_dHeight;	// ������ũ��Ʈ �β�
	double	dDist	= pDB->m_BindConc.m_dDeep;		// ������ũ��Ʈ ���Ա���
	double	dDis	= pDB->m_BindConc.m_dDis;		// ������ũ��Ʈ ���Ա���

	double dAsUp   = dWidthUp*dThickUp;
	double dYUp    = (dHeightWeb+dThickUp)/2;
	double dAsyUp  = dAsUp*dYUp;
	double dAsy2Up = dAsyUp*dYUp;
	double dIyyUp  = dWidthUp*pow(dThickUp, 3)/12;
	double dIzzUp  = pow(dWidthUp, 3)*dThickUp/12;

	double dAsDn   = dWidthDn*dThickDn;
	double dYdn    = -(dHeightWeb+dThickDn)/2;
	double dAsyDn  = dAsDn*dYdn;
	double dAsy2Dn = dAsyDn*dYdn;
	double dIyyDn  = dWidthDn*pow(dThickDn, 3)/12;
	double dIzzDn  = pow(dWidthDn,3)*dThickDn/12;

	double dAsW  = dHeightWeb*dThickWeb;
	double dyW   = 0.0;
	double dAsyW = dAsW*dyW;
	double dAsy2W= dAsyW*dyW;
	double dIyyW = dThickWeb*pow(dHeightWeb, 3)/12;	///<B: �β� H: ���γ��� 
	double dIzzW = dHeightWeb*pow(dThickWeb, 3)/12;	///<B: ���γ��� H: �β� 

	double slab_As  = dSlabWidth*dThickSlab;	
	double slab_y   = (dHeightWeb+dThickSlab)/2+dThickHunch;
	double slab_Asy = slab_As*slab_y;
	double slab_Asy2= slab_Asy*slab_y;
	double slab_Iyy = dSlabWidth*pow(dThickSlab,3)/12;
	double slab_Izz = pow(dSlabWidth,3)*dThickSlab/12;
	//
	double Sum_As  = dAsUp+dAsDn+dAsW;
	double Sum_Asy = dAsyUp+dAsyDn+dAsyW;
	double Sum_Asy2= dAsy2Up+dAsy2Dn+dAsy2W;
	double Sum_Izz = dIzzUp+dIzzDn+dIzzW;
	double Sum_Iyy = dIyyUp+dIyyDn+dIyyW;

	///< �ռ� ��
	double IsApo= Sum_Asy2 + Sum_Iyy;				///<(mm4) Io+Ay^2
	double de	= Sum_Asy / Sum_As;					///<(mm) ��ü���� ������(e) 
	double Ysu	= (dHeightWeb/2 + dThickUp) - de;	///<(mm) �ռ��� ���� ����
	double Ysl	= -(dHeightWeb/2 + dThickDn + de);	///<(mm) �ռ��� ���� ����
	double I33	= IsApo - Sum_As*de*de;				///<(mm4) �ռ��� �ܸ�2�� ���Ʈ Io+Ay^2 - A��e2   
	double I22  = pow(dWidthUp,3)*dThickUp/12 + pow(dWidthDn,3)*dThickDn/12	+ dHeightWeb*pow(dThickWeb,3)/12; 
	double K	= (dWidthUp*pow(dThickUp, 3)+dWidthDn*pow(dThickDn, 3)+dHeightWeb*pow(dThickWeb, 3))/3;///< (mm4)��Ʋ�����

	if(pDB->IsTUGir())
	{
		// ������ũ��Ʈ �ռ��ܸ�
		double	dAc		= dBc * dTc;
		double	dApNc	= dAc / dNc;				// ������ũ��Ʈ�� ȯ��ܸ���
		double	dAvc	= Sum_As + dApNc;
		double	dIc		= dBc * pow(dTc, 3) / 12;
		double	dDc		= dHeightWeb / 2 - de + dThickUp;
		double	dDvc	= dDc * Sum_As / dAvc;
		double	dDvs	= dDc - dDvc;
		double	dIvc33	= I33 + dIc / dNc + Sum_As * pow(dDvs,2) + dAc/dNc * pow(dDvc,2);
		double	dIvc22	= I22 + (dTc * pow(dBc,3)/dNc) / 12;
		double	dKvc	= K + dBc  * pow(dTc, 3) / 3 / dNc;
		// �ٴ��� ��ũ��Ʈ�� �ռ��ܸ�
		double	dAd		= dSlabWidth * dThickSlab;
		double	dApNd	= dAd / dN;					// �ٴ����� ȯ��ܸ���
		double	dAvd	= dAvc + dApNd;
		double	dId		= dSlabWidth * pow(dThickSlab,3) / 12;
		double	dDd		= dDvc +  dTc / 2 - dDist + dThickSlab/2;
		double	dDvd	= dDd * dAvc/dAvd;
		double	dDvsc	= dDd - dDvd;
		double	dIvd33	= dIvc33 + dId/dN + dAvc*pow(dDvsc,2) + dAd/dN*pow(dDvd,2);
		double	dIvd22	= dIvc22 + (dThickSlab*pow(dSlabWidth,3)/dN) / 12;
		double	dKvd	= dKvc + dSlabWidth * pow(dThickSlab, 3) / 3 / dN;
		
		// ������ũ��Ʈ �ռ��ܸ�
		m_MapSecCoe["Acs_SUM"]	= dAc/dN;
		m_MapSecCoe["Ac"]		= dAc;
		m_MapSecCoe["ApNc"]		= dApNc;
		m_MapSecCoe["Avc"]		= dAvc;
		m_MapSecCoe["Ic"]		= dIc;
		m_MapSecCoe["Dc"]		= dDc;
		m_MapSecCoe["Dvc"]		= dDvc;
		m_MapSecCoe["Dvs"]		= dDvs;
		m_MapSecCoe["Ivc33"]	= dIvc33;
		m_MapSecCoe["Ivc22"]	= dIvc22;
		m_MapSecCoe["Kvc"]		= dKvc;
		m_MapSecCoe["BindConWidth"]		= dBc;
		m_MapSecCoe["BindConThick"]		= dTc;
		m_MapSecCoe["BindConDist"]		= dDist;
		m_MapSecCoe["BindConDis"]		= dDis;
		// �ٴ��� ��ũ��Ʈ �ռ��ܸ�
		m_MapSecCoe["Ad"]		= dAd;
		m_MapSecCoe["ApNd"]		= dApNd;
		m_MapSecCoe["Avd"]		= dAvd;
		m_MapSecCoe["Id"]		= dId;
		m_MapSecCoe["Dd"]		= dDd;
		m_MapSecCoe["Dvd"]		= dDvd;
		m_MapSecCoe["Dvsc"]		= dDvsc;
		m_MapSecCoe["Ivd33"]	= dIvd33;
		m_MapSecCoe["Ivd22"]	= dIvd22;
		m_MapSecCoe["Kvd"]		= dKvd;

		double	dConfAs		= dBc * dTc;
		double	dAv2		= Sum_As + dConfAs / dNc;
		double	dD2			= Ysu;
		double	dDc2		= dD2 * Sum_As / dAv2;
		double	dYcu2		= dDc2 + dTc/2;
		double	dAv3		= Sum_As + dConfAs/dNc + slab_As/dN;
		double	dD3			= dYcu2 - dDist + dThickSlab/2;
		double	dDd3		= dD3 * dAv2 / dAv3;
		double	dYdu3		= dDd3 + dThickSlab/2;
		double	dYdl3		= dYdu3 - dThickSlab;
		double	dYcu3		= dYdl3 + dDist;
		double	dYsu3		= dYcu3 - dTc/2;
		double	dYsl3		= dYsu3 - (dHeightWeb + dThickUp + dThickDn);

		m_MapSecCoe["Yvsu"]	= dYsu3;
		m_MapSecCoe["Yvsl"]	= dYsl3;
		m_MapSecCoe["BindFck"] = dBindFck;
	}
	else
	{
		///< �ռ� ��
		BOOL bUpperInner = (pOpt->GetDesignHeigtBaseUpper() == 1); //���ǳ���
		double Acs   = slab_As/dN; 
		double Av    = Sum_As+Acs;
		double dv    = bUpperInner ? (Ysu - dThickUp + dThickHunch + dThickSlab/2) : (Ysu + dThickHunch + dThickSlab/2);
		double dvc   = dv * Sum_As / (Acs + Sum_As);
		double dvs   = dv - dvc;
		double Yvcu  = dvc+dThickSlab/2;
		double Yvcl  = Yvcu-dThickSlab;
		double Yvsu  = bUpperInner ? (Yvcu-(dThickSlab+dThickHunch)+dThickUp) : (Yvcu-(dThickSlab+dThickHunch));
		double Yvsl  = Yvsu-(dHeightWeb+dThickUp+dThickDn);
		double Igc   = dSlabWidth * pow(dThickSlab,3)/12;
		double Igcs  = Igc / dN;
		double IvApo = IsApo + Igcs + Acs * pow(dv,2);
		double Iv33  = I33 + Igcs + Sum_As * pow(dvs,2) + Acs * pow(dvc,2);
		double Iv22  = I22 + pow(dSlabWidth,3) * dThickSlab /12/dN;
		double	Kv	 = K + dSlabWidth*pow(dThickSlab, 3)/3/dN;///<��Ʋ�����
		double dWarpingH  = dHeightWeb+(dThickUp+dThickDn)/2;///< (mm) ����߽ɿ��� �ϴ��÷��� �߽ɱ���
		double dWarpingI1 = dThickUp*dWidthUp*dWidthUp*dWidthUp/12;///< (mm4)
		double dWarpingI2 = dThickDn*dWidthDn*dWidthDn*dWidthDn/12;///< (mm4)
		double dWarpingIw = (dWarpingI1*dWarpingI2)/(dWarpingI1+dWarpingI2)*dWarpingH*dWarpingH;///< (mm6)
		double dLengthJigan = pDB->m_dLengthBaseCrossBeam;// ���κ� ����
		double dWarpingAlpha = dLengthJigan*sqrt(81000*Kv/(dEst*dWarpingIw));					///(������)

		///< �ռ���
		m_MapSecCoe["Acs_SUM"]	= Acs;		///<ȯ�� ������ܸ���
		m_MapSecCoe["Av_SUM"]	= Av;		///<����ܸ���+ȯ�� ������ܸ���
		m_MapSecCoe["dv"]		= dv;		///<
		m_MapSecCoe["dvc"]		= dvc;		///<
		m_MapSecCoe["dvs"]		= dvs;		///<
		m_MapSecCoe["Yvcu"]		= Yvcu;		///<�ռ��� ������ ����(��������)
		m_MapSecCoe["Yvcl"]		= Yvcl;		///<�ռ��� ������ ����(�������Ϻ�)
		m_MapSecCoe["Yvsu"]		= Yvsu;		///<�ռ��� ����   ����
		m_MapSecCoe["Yvsl"]		= Yvsl;		///<�ռ��� ����   ����
		m_MapSecCoe["Igc"]		= Igc;		///<�������� �ܸ�2�����Ʈ
		m_MapSecCoe["Igcs"]		= Igcs;		///<ȯ�� ������ �ܸ�2�����Ʈ
		m_MapSecCoe["IvApo"]	= IvApo;	///<(mm4) Io+Ay^2 (+����)	
		m_MapSecCoe["Iv33"]		= Iv33;		///<(mm4) �ռ��� �ܸ�2�� ���Ʈ Io+Ay^2 - A��e2 (+����)
		m_MapSecCoe["Iv22"]		= Iv22;		///<(mm4) �ܸ�2�� ���Ʈ�� ��(����+������)
		m_MapSecCoe["Kv"]		= Kv;	    ///<(mm4)��Ʋ�� ���
		m_MapSecCoe["WarpingIw"]= dWarpingIw;
		m_MapSecCoe["WarpingAlpha"] = dWarpingAlpha;
	}	
	//////////////////////////////////////////////////////////////////////////
	
	m_MapSecCoe["SlabWidth"]	= dSlabWidth;
	m_MapSecCoe["FLANGE_W_UP"] = dWidthUp;
	m_MapSecCoe["FLANGE_T_UP"] = dThickUp;
	m_MapSecCoe["FLANGE_W_DN"] = dWidthDn;
	m_MapSecCoe["FLANGE_T_DN"] = dThickDn;
	m_MapSecCoe["WEB_T"]	   = dThickWeb;
	m_MapSecCoe["WEB_H"]	   = dHeightWeb;
	m_MapSecCoe["HUNCH"]	   = dThickHunch;
	m_MapSecCoe["SLAB_THICK"]  = dThickSlab;

	m_MapSecCoe["As_UP"]	= dAsUp;	///<(mm2)�ܸ���
	m_MapSecCoe["Y_UP"]		= dYUp;		///<(mm) ���ɰ��� �Ÿ�
	m_MapSecCoe["Asy_UP"]	= dAsyUp;	///<(mm3)�ܸ� 1�����Ʈ(Geometrical Moment)
	m_MapSecCoe["Asy2_UP"]	= dAsy2Up;	///<
	m_MapSecCoe["Iyy_UP"]	= dIyyUp;	///<(mm4)�ܸ� 2�����Ʈ(yy��)
	m_MapSecCoe["Izz_UP"]	= dIzzUp;	///<(mm4)�ܸ� 2�����Ʈ(zz��)-b, h�� �ݴ���

	m_MapSecCoe["As_DN"]	= dAsDn;	///<(mm2)�ܸ���
	m_MapSecCoe["Y_DN"]		= dYdn;		///<(mm)���ɰ��� �Ÿ�
	m_MapSecCoe["Asy_DN"]	= dAsyDn;	///<(mm3)�ܸ� 1�����Ʈ(Geometrical Moment)
	m_MapSecCoe["Asy2_DN"]	= dAsy2Dn;	///<
	m_MapSecCoe["Iyy_DN"]	= dIyyDn;   ///<(mm4)�ܸ� 2�����Ʈ(yy��)
	m_MapSecCoe["Izz_DN"]	= dIzzDn;	///<(mm4)�ܸ� 2�����Ʈ(zz��)-b, h�� �ݴ���

	m_MapSecCoe["As_WEB"]	= dAsW;		///<(mm2)�ܸ���
	m_MapSecCoe["Y_WEB"]	= dyW;		///<(mm)���ɰ��� �Ÿ�
	m_MapSecCoe["Asy_WEB"]	= dAsyW;	///<(mm3)�ܸ� 1�����Ʈ(Geometrical Moment)
	m_MapSecCoe["Asy2_WEB"]	= dAsy2W;	///<
	m_MapSecCoe["Iyy_WEB"]	= dIyyW;    ///<(mm4)�ܸ� 2�����Ʈ(yy��)
	m_MapSecCoe["Izz_WEB"]	= dIzzW;	///<(mm4)�ܸ� 2�����Ʈ(zz��)-b, h�� �ݴ���

	m_MapSecCoe["As_SLAB"]	= slab_As;	///<(mm2)�ܸ���
	m_MapSecCoe["Y_SLAB"]	= slab_y;	///<(mm)���ɰ��� �Ÿ�
	m_MapSecCoe["Asy_SLAB"]	= slab_Asy;	///<(mm3)�ܸ� 1�����Ʈ(Geometrical Moment)
	m_MapSecCoe["Asy2_SLAB"]= slab_Asy2;///<
	m_MapSecCoe["Iyy_SLAB"]	= slab_Iyy; ///<(mm4)�ܸ� 2�����Ʈ(yy��)
	m_MapSecCoe["Izz_SLAB"]	= slab_Izz;	///<(mm4)�ܸ� 2�����Ʈ(zz��)-b, h�� �ݴ���
	///< �ռ��� 
	m_MapSecCoe["As_SUM"]	= Sum_As;	///<(mm2)�ܸ��� ��
	m_MapSecCoe["Asy_SUM"]	= Sum_Asy;	///<(mm3)�ܸ� 1�����Ʈ(Geometrical Moment) ��
	m_MapSecCoe["Asy2_SUM"]	= Sum_Asy2;
	m_MapSecCoe["Iyy_SUM"]	= Sum_Iyy;  ///<(mm4)�ܸ� 2�����Ʈ(yy��) ��
	m_MapSecCoe["Izz_SUM"]	= Sum_Izz;  ///<(mm4)�ܸ� 2�����Ʈ(zz��) �� - b, h�� �ݴ���
	m_MapSecCoe["de"]       = de;		///<������(e)
	m_MapSecCoe["Ysu"]		= Ysu;		///<�ռ��� ���ǵ���
	m_MapSecCoe["Ysl"]		= Ysl;		///<�ռ��� ���ǵ���
	m_MapSecCoe["IsApo"]	= IsApo;	///<(mm4) Io+Ay^2
	m_MapSecCoe["I33"]		= I33;	    ///<(mm4) �ռ��� �ܸ�2�� ���Ʈ Io+Ay^2 - A��e2   
	m_MapSecCoe["I22"]		= I22;	    ///<(mm4) �ܸ�2�� ���Ʈ�� ��
	m_MapSecCoe["K"]		= K;		///<(mm4)��Ʋ�� ���
}

void CFemModelingCalc::GetSectionCoefficientCR(CPlateBasicIndex *pBx)
{	
	m_MapSecCoeCR.RemoveAll();
	CPlateGirderApp  *pGir	= pBx->GetGirder();

	CSection *pSec = pBx->GetSection();
	CPlateCrossBeam *pC  = NULL;
	CVBracing       *pVr = NULL;
	if(pSec->IsenDetType(SECDET_CROSSBEAM))	
		pC = pSec->GetCrossBeam();
	else if(pSec->IsenDetType(SECDET_VBRACING))
		pVr = pSec->GetVBracing(); 

	if(pSec->IsenDetType(SECDET_CROSSBEAM))
	{
		double up_l = 0;
		double up_t = 0;
		double lo_l = 0;
		double lo_t = 0;
		double w_l = 0;
		double w_t = 0;

		if(pC->GetType()==CR_TYPE_HSTEEL)
		{
			CSteelMaterialData	*pMatData	= pC->GetSteelMaterialData();
			up_l = pMatData->m_dB;
			up_t = pMatData->m_dT2;  
			lo_l = pMatData->m_dB;
			lo_t = pMatData->m_dT2; 
			w_l = pMatData->m_dH-(pMatData->m_dT2*2);
			w_t = pMatData->m_dT1;
		}
		else
		{
			up_l = pC->m_uM_CR * 2;
			up_t = pC->m_T2_CR;  
			lo_l = pC->m_dM_CR * 2;
			lo_t = pC->m_T1_CR; 
			w_l = pC->m_H_CR;
			w_t = pC->m_T3_CR;
		}
		m_MapSecCoeCR["up_l"] = up_l;
		m_MapSecCoeCR["up_t"] = up_t;
		m_MapSecCoeCR["lo_l"] = lo_l;
		m_MapSecCoeCR["lo_t"] = lo_t;
		m_MapSecCoeCR["w_l" ] = w_l;
		m_MapSecCoeCR["w_t" ] = w_t;

		double up_As=up_l*up_t;
		double lo_As=lo_l*lo_t;
		double w_As =w_l*w_t;
		m_MapSecCoeCR["up_As"] = up_As;
		m_MapSecCoeCR["lo_As"] = lo_As;
		m_MapSecCoeCR["w_As" ] = w_As;

		double up_y=w_l/2+up_t/2;
		double lo_y=-(w_l/2+lo_t/2);
		double w_y =0;
		m_MapSecCoeCR["up_y"] = up_y;
		m_MapSecCoeCR["lo_y"] = lo_y;
		m_MapSecCoeCR["w_y" ] = w_y;

		double	up_Asy=up_As*up_y;
		double	lo_Asy=lo_As*lo_y;
		double	w_Asy=w_As*w_y;
		m_MapSecCoeCR["up_Asy"] = up_Asy;
		m_MapSecCoeCR["lo_Asy"] = lo_Asy;
		m_MapSecCoeCR["w_Asy" ] = w_Asy;

		double	up_Asy2=up_Asy*up_y;
		double	lo_Asy2=lo_Asy*lo_y;
		double	w_Asy2=w_Asy*w_y;
		m_MapSecCoeCR["up_Asy2"] = up_Asy2;
		m_MapSecCoeCR["lo_Asy2"] = lo_Asy2;
		m_MapSecCoeCR["w_Asy2" ] = w_Asy2;

		double	up_Io=up_l*pow(up_t,3)/12;
		double	lo_Io=lo_l*pow(lo_t,3)/12;
		double	w_Io =w_t *pow(w_l ,3)/12;
		m_MapSecCoeCR["up_Io"] = up_Io;
		m_MapSecCoeCR["lo_Io"] = lo_Io;
		m_MapSecCoeCR["w_Io" ] = w_Io;

		double	up_Ioo=pow(up_l,3)*up_t/12;
		double	lo_Ioo=pow(lo_l,3)*lo_t/12;
		double	w_Ioo =pow(w_t ,3)*w_l /12;
		m_MapSecCoeCR["up_Ioo"] = up_Ioo;
		m_MapSecCoeCR["lo_Ioo"] = lo_Ioo;
		m_MapSecCoeCR["w_Ioo" ] = w_Ioo;

		double	Sum_As=up_As+lo_As+w_As;
		double	Sum_Asy=up_Asy+lo_Asy+w_Asy;
		double	Sum_Asy2=up_Asy2+lo_Asy2+w_Asy2;
		m_MapSecCoeCR["Sum_As"  ] = Sum_As;
		m_MapSecCoeCR["Sum_Asy" ] = Sum_Asy;
		m_MapSecCoeCR["Sum_Asy2"] = Sum_Asy2;

		double	Sum_Io=up_Io+lo_Io+w_Io;
		double	Sum_Ioo=up_Ioo+lo_Ioo+w_Ioo;
		m_MapSecCoeCR["Sum_Io" ] = Sum_Io;
		m_MapSecCoeCR["Sum_Ioo"] = Sum_Ioo;

		double	I33=Sum_Io+Sum_Asy2;
		double	I22=up_t*pow(up_l,3)/12+lo_t*pow(lo_l,3)/12+w_l*pow(w_t,3)/12;
		m_MapSecCoeCR["I33"] = I33;
		m_MapSecCoeCR["I22"] = I22;

		double	K=up_l*pow(up_t,3)/3+lo_l*pow(lo_t,3)/3
				   +w_l*pow(w_t,3)/3;
		m_MapSecCoeCR["K"] = K;
	}
	else if(pSec->IsenDetType(SECDET_VBRACING))
	{
		double Sum_As=0;
		double I33   =0;
		CVBracing      *pVr  = pBx->GetSection()->GetVBracing();

		double dAsUp = frCm2(pVr->GetSteelMaterialData(STEELMAT_UPPER)->m_dSectionArea);
		double dAsDn = frCm2(pVr->GetSteelMaterialData(STEELMAT_LOWER)->m_dSectionArea);
		double dAsSd = frCm2(pVr->GetSteelMaterialData(STEELMAT_DIAGONAL)->m_dSectionArea);
		Sum_As       = dAsUp+dAsDn+dAsSd;
		
		double dH = pGir->GetHeightGirderByStaAng(pBx->GetStation()) - pVr->m_dLH1 - pVr->m_dLH2;
		double dAngRad = 0;
		if(pVr->GetType()==TYPE_VBRACING)	///< V��
		{
			CDPoint xyUpDn  = CDPoint(0, -1);
			CDPoint xyLeft  = pVr->GetVectorFrame(CVBracing::LEFT);
			dAngRad = xyUpDn.BetweenAngle(xyLeft).GetAngleRadian();
		}
		else
		{
			CDPoint xyDnUp  = CDPoint(0, 1);
			CDPoint xyLeft  = pVr->GetVectorFrame(CVBracing::LEFT);
			dAngRad = xyLeft.BetweenAngle(xyDnUp).GetAngleRadian();
		}
		
		double dVal1 = (4*dH*dH*dAsUp)/3;
		double dVal2 = (dAsDn!=0)? dAsUp/dAsDn : 0;
		double dVal3 = (dAsSd!=0 && dAngRad!=0)? dAsUp/(dAsSd*pow(sin(dAngRad),3)) : 0;

		I33 = dVal1 * 1/(2+dVal2+dVal3);

		m_MapSecCoeCR["Sum_As"] = Sum_As;
		m_MapSecCoeCR["I33"] = I33;
	}	
}

////////////////////////////////////////////////////////////////////
//���� : �÷��� ��ȿ�� ���ϱ�
////////////////////////////////////////////////////////////////////
double CFemModelingCalc::GetEffectWidthFlange(long nG, double dSta, BOOL bUpper)
{
	CPlateBridgeApp *pDB  = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx = pGir->GetBxByStation(dSta);
	long nKsu = pDB->m_nQtyJigan;

	double dJiganlen = GetEffectWidthJiganLength(nG, dSta);
	double dB        = bUpper ? pGir->GetWidthFlangeUpper(pBx)/2 : pGir->GetWidthFlangeLower(pBx)/2;

	BOOL bPier = FALSE;
	bPier = IsPierEffectWidthSlab(nG, dSta);
	if(nKsu == 1) bPier = FALSE;
	
	double dLambdaL = GetEffectWidthLambda(dB, dJiganlen, bPier);

	return dLambdaL*2;
}

BOOL CFemModelingCalc::IsPierEffectWidthSlab(long nG, double dSta)
{
	CPlateBridgeApp *pDB  = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);

	// ���� ���� ���ϱ�
	long   Jigansu = pDB->m_nQtyJigan;
	double Distance[KYUNGGANSUMAX];  // L1,L2,L3.....�� ������ �迭
	double Length = 0;
	long	nK = 0;
	for(nK=0; nK<Jigansu; nK++)
	{
		Length = toM(pGir->GetStationOnJijum(nK+1) - pGir->GetStationOnJijum(nK));
		Distance[nK]=Length;
	}

	// �߾Ӻο� ������ ��� ���ϱ�
	double DisK[KYUNGGANSUMAX*2];  // 1���� ���� �� 2�� �� ����
	long count = 0;
	DisK[0] = 0;

	for(nK=1; nK<Jigansu*2; nK++)
	{
		if(nK==1)
		{
			DisK[1] = 0.8 * Distance[0] + DisK[0];
			continue;
		}

		if(nK==Jigansu*2-1)
				DisK[nK] = DisK[nK-1] + 0.8 * Distance[Jigansu-1];

		else if(nK % 2 == 1) // nK�� Ȧ���� ��츸 count 1 ����
		{
			count++;
			DisK[nK] = 0.6 * Distance[count] + DisK[nK-1];
		}
		else 
		{
			DisK[nK] = 0.2 * (Distance[count] + Distance[count+1]) + DisK[nK-1];
		}
	}

	// �߾Ӻ����� ���������� �Ǵ�	
	BOOL bCenter = TRUE;// (�߾Ӻ�)
	double StaMeter=0;       // Station(���Ͱ�)

	StaMeter = toM(dSta - pGir->GetStationOnJijum(0));

	for(nK=1; nK<Jigansu*2; nK++)
	{
		if(nK%2==0) 
		{
			if(StaMeter < DisK[nK]) break;	
		}
		else
		{
			if(StaMeter <= DisK[nK]) break;	
		}
	}

	if(nK%2==0) bCenter = FALSE;
	else        bCenter = TRUE;

	if(bCenter) return FALSE;  // ���� �߾Ӻ�
	else        return TRUE;   // �߰� ������
}

// �������� ��ȿ��
double CFemModelingCalc::GetEffectWidthSlab(long nG, double dSta)
{
	CPlateBridgeApp *pDB     = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx    = pGir->GetBxByStation(dSta);

	BOOL bTuGir	= pDB->IsTUGir();
	long nGsu = pDB->GetGirdersu();
	long nKsu = pDB->m_nQtyJigan;

	double lambdaL1=0, lambdaL2=0, EffectWidthSlab=0;
	double b1=0,b2=0,len=0;
	double dBf = GetEffectWidthFlange(nG, dSta, TRUE);

	BOOL bPier = IsPierEffectWidthSlab(nG, dSta);
	if(nKsu==1) bPier = FALSE;

	//	������ ���� �߾Ӻ����� �Ǵ�
	if(nGsu==1) 
	{
		len = GetEffectWidthJiganLength(nG, dSta);
		lambdaL1 = GetEffectWidthLambda(pDB->m_dWidthSlabLeft, len, bPier);
		lambdaL2 = GetEffectWidthLambda(pDB->m_dWidthSlabRight,len, bPier);
		
		double dLenPlus = 0;
		if(bTuGir)	dLenPlus = pDB->m_BindConc.m_dWidth;
		else		dLenPlus = dBf + pDB->m_dThickSlabHunch*2;
		
		EffectWidthSlab = dLenPlus + lambdaL1 + lambdaL2;
	}
	else if(nG == 0 || nG == -1)
	{
		if(bTuGir)
		{
			b1 = pDB->m_dWidthSlabLeft - pDB->m_BindConc.m_dWidth/2;
			b2 = (pGir->GetDisGirderToGirder(pBx) -  pDB->m_BindConc.m_dWidth)/2;
		}
		else
		{
			b1 = pDB->m_dWidthSlabLeft - dBf/2;
			b2 = (pGir->GetDisGirderToGirder(pBx) - dBf- pDB->m_dThickSlabHunch*2) / 2;
		}

		len = GetEffectWidthJiganLength(nG, dSta);
		lambdaL1 = GetEffectWidthLambda(b1, len, bPier);
		lambdaL2 = GetEffectWidthLambda(b2, len, bPier);

		double dLenPlus = 0;
		if(bTuGir)	dLenPlus = pDB->m_BindConc.m_dWidth;
		else		dLenPlus = dBf + pDB->m_dThickSlabHunch;
		
		EffectWidthSlab = dLenPlus + lambdaL1 + lambdaL2;
	}
	else if(nG==nGsu-1)
	{
		CPlateBasicIndex *pBxPreMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);

		if(bTuGir)
		{
			b1 = (pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch) -  pDB->m_BindConc.m_dWidth)/2;
			b2 = pDB->m_dWidthSlabRight - pDB->m_BindConc.m_dWidth/2;
		}
		else
		{
			b1 = pDB->m_dWidthSlabRight - dBf/2;
			b2 = (pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch) - dBf - pDB->m_dThickSlabHunch*2)/2;
		}

		len = GetEffectWidthJiganLength(nG, dSta);
		lambdaL1 = GetEffectWidthLambda(b1, len, bPier);
		lambdaL2 = GetEffectWidthLambda(b2, len, bPier);

		double dLenPlus = 0;
		if(bTuGir)	dLenPlus = pDB->m_BindConc.m_dWidth;
		else		dLenPlus = dBf + pDB->m_dThickSlabHunch;

		EffectWidthSlab = dLenPlus + lambdaL1 + lambdaL2;
	}
	else
	{
		CPlateBasicIndex *pBxPreMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);

		if(bTuGir)
		{
			b1 = (pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch) - pDB->m_BindConc.m_dWidth)/2;
			b2 = (pGir->GetDisGirderToGirder(pBx) - pDB->m_BindConc.m_dWidth)/2;
		}
		else
		{
			b1 = (pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch) - dBf - pDB->m_dThickSlabHunch*2)/2;
			b2 = (pGir->GetDisGirderToGirder(pBx) - dBf - pDB->m_dThickSlabHunch*2)/2;
		}

		len = GetEffectWidthJiganLength(nG, dSta);
		lambdaL1 = GetEffectWidthLambda(b1, len, bPier);
		lambdaL2 = GetEffectWidthLambda(b2, len, bPier);

		double dLenPlus = 0;
		if(bTuGir)	dLenPlus = pDB->m_BindConc.m_dWidth;
		else		dLenPlus = dBf + pDB->m_dThickSlabHunch*2;

		EffectWidthSlab = dLenPlus + lambdaL1 + lambdaL2;
	}

	return	EffectWidthSlab;
}

//���� : ��ȿ��(�÷���, ������)�� ���ϱ� ���� [���� ������]�� ���ϱ� 1
//�Է� :
double CFemModelingCalc::GetEffectWidthJiganLength(long nG, double dSta)
{
	CPlateBridgeApp *pDB    = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir   = pDB->GetGirder(nG);
	CPlateGirderApp *pGirCR = pDB->GetGirder(-1);
	double EffectLength = 0; // return�� �������

	// ���� ���� ���ϱ�
	long Jigansu = pDB->m_nQtyJigan;
	double Distance[KYUNGGANSUMAX];  // L1,L2,L3.....�� ������ �迭
	double Length=0;
	long nK = 0;
	for(nK = 0; nK < Jigansu; nK++)
	{
		Length = toM(pGir->GetStationOnJijum(nK+1) - pGir->GetStationOnJijum(nK));
		Distance[nK] = Length;
	}

	// �߾Ӻο� ������ ��� ���ϱ�
	double DisK[KYUNGGANSUMAX*2];  // 1���� ���� �� 2�� �� ����
	long count = 0;
	DisK[0] = 0;

	for(nK = 1; nK < Jigansu*2; nK++)
	{
		if(nK == 1)
		{
			DisK[1] = 0.8 * Distance[0] + DisK[0];
			continue;
		}
		if(nK == Jigansu*2-1)	DisK[nK] = DisK[nK-1] + 0.8 * Distance[Jigansu-1];
		else if(nK % 2 == 1) // nK�� Ȧ���� ��츸 count 1 ����
		{
			count++;
			DisK[nK] = 0.6 * Distance[count] + DisK[nK-1];
		}
		else DisK[nK] = 0.2 * (Distance[count] + Distance[count+1]) + DisK[nK-1];
	}

	// �߾Ӻ����� ���������� �Ǵ�	
	BOOL bCenter = TRUE;
	double StaMeter;       // Station(���Ͱ�)

//	StaMeter = toM(pBx->GetStation() - pGir->GetStationOnJijum(0));
	StaMeter = toM(dSta - pGir->GetStationOnJijum(0));

	for(nK = 1; nK < Jigansu*2; nK++)
	{
		if(nK % 2 == 0) 
		{
			bCenter = FALSE;
			if(StaMeter < DisK[nK]) break;	
		}
		else
		{
			bCenter = TRUE;
			if(StaMeter <= DisK[nK]) break;	
		}
	}

	long nJigan;
	if(bCenter) // ���� �߾Ӻ�
	{
		nJigan = (nK+1)/2 - 1;
		EffectLength = GetEffectWidthJiganLength(nG, nJigan, FALSE);
	}
	else        // �߰� ������
	{
		nJigan = nK/2 - 1;
		EffectLength = GetEffectWidthJiganLength(nG, nJigan, TRUE);
	}
	return EffectLength;
}


//���� : ��ȿ��(�÷���, ������)�� ���ϱ� ���� [���� ������]�� ���ϱ� 2
//�Է� :
//			nG  : �Ŵ� Index            |                                            
//			nK	: ���� Index            |        <���� Index ��>                                     
//
//        bJijum: FALSE - ���� �߾Ӻ�   |-->       [0]      [1]       [2]             
//                                      |      |---------|---------|---------|                                    
//				  TRUE	- �߰� ������   |-->            [0]       [1]                          
double CFemModelingCalc::GetEffectWidthJiganLength(long nG, long nK, BOOL bJijum) 
{

	if(nK<0) return -1;
	CPlateBridgeApp *pDB  = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir = pDB->GetGirder(nG);

	long   Jigansu   = pDB->m_nQtyJigan;
	double JiganL    = 0;
	double NextJiganL= 0;
	// ��������
	JiganL = pGir->GetStationOnJijum(nK+1) - pGir->GetStationOnJijum(nK);

	if(Jigansu < 2)			//������ �϶�
		return JiganL;
	
	if(bJijum)  // �߰� ������
	{
		if(nK >= Jigansu-1) return -1;
		if(nK == Jigansu-2)
				NextJiganL=pDB->m_dLengthJigan[nK+1];
		else	NextJiganL=pDB->m_dLengthJigan[nK+1];
		return (JiganL+NextJiganL)*0.2;
	}
	else        // ���� �߾Ӻ�
	{
		if(nK >= Jigansu)  return -1;
		if(nK==0 || nK==Jigansu-1) return JiganL*0.8;
		else                       return JiganL*0.6;
	}
}



//���� : ��ȿ��(�÷���, ������)�� ���ϱ� ���� Lambda ���ϱ�
//�Է� :
//			b			�� �ʺ�
//			l			���� ����(�������� �ƴ�)
//			bPier		����(����,����)�̸� TRUE	
double CFemModelingCalc::GetEffectWidthLambda(double b, double l, BOOL bPier) 
{
	if(bPier)
	{
		if(b/l <= 0.02)		return b;
		else if(b/l >= 0.3)	return 0.15*l;
		else				return (1.06 - 3.2*(b/l) + 4.5*(b/l)*(b/l)) * b; 
	}
	else
	{
		if(b/l <= 0.05)		return b;
		else if(b/l >= 0.3)	return 0.15*l;
		else				return (1.1 - 2*(b/l)) * b; 
	}
}


//////////////////////////////////////////////////////////////////////
/// �� BX�� I33�� ������ ����
//////////////////////////////////////////////////////////////////////
BOOL CFemModelingCalc::IsSameSectionBetweenTwoBx(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, long nAnalysisType)
{
	GetSectionCoefficient(pBxStt);
	double dI33stt = GetSectionCoeffI33(nAnalysisType, pBxStt);
	GetSectionCoefficient(pBxEnd);
	double dI33end = GetSectionCoeffI33(nAnalysisType, pBxEnd);

	if(IsSameI33(dI33stt,dI33end))
		return TRUE;
	else
		return FALSE;
}

BOOL CFemModelingCalc::IsSameI33(double dI331, double dI332, double dError)
{
	double	dI331m4	= toM4(dI331);
	double	dI332m4	= toM4(dI332);

	BOOL	bSame	= FALSE;
	if(fabs(dI331m4-dI332m4)<dError)
		bSame = TRUE;
	return bSame;
}


//////////////////////////////////////////////////////////////////////
/// ���� BX ��ġ�� �ܸ��̸��� ����
//////////////////////////////////////////////////////////////////////
CString CFemModelingCalc::GetSectionNameByBx(CFEMManage *pFemData, CPlateBasicIndex *pBx, long nAnalysisType)
{
	GetSectionCoefficient(pBx);
	double  dI33  = GetSectionCoeffI33(nAnalysisType, pBx);
	CString szSec = "";
	POSITION pos  = pFemData->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrame = (CFrameSection*)pFemData->m_FrameSectionList.GetNext(pos);
		if(IsSameI33(dI33,frM4(pFrame->m_dIx)))
			return pFrame->m_szName;	
	}
	return "SEC_ERROR";
}
//////////////////////////////////////////////////////////////////////
/// �ܸ��̸����� �Ŵ���ȣ�� �����̼��� ã�´�.
/// nType = 0 ; Girder nType = 1; ���κ�
/// dSta : �ܸ��� ��ġ�� �ƴϰ�  GetStationCheckPos()�� ���� ������ �����̼� ��ġ��.
/// nG=-1 : ��ü �Ŵ� �� ���� ���� ������ �ܸ��� ��ġ		(nType==0 �϶��� ����)
/// nG>-1 : �ش� �Ŵ����� ���� ���� ������ �ܸ��� ��ġ		(nType==0 �϶��� ����)
/// return = TRUE : �Ŵ� �������� ���ϴ� �ܸ��� ����
/// return = FALSE : �Ŵ� �������� ���ϴ� �ܸ��� ����
//////////////////////////////////////////////////////////////////////
BOOL CFemModelingCalc::GetPositionBySectionName(CString szSecName, long &nG, double &dSta, long nType)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CFEMManage		*pManage	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS); 
	CFrameSection	*pFrSec		= NULL;
	CFemStressCalc   StressCalc(m_pDataManage, m_pCalcStd);

	POSITION pos	= pManage->m_FrameSectionList.GetHeadPosition();
	long	nSec	= atol(szSecName)-1;
	long	n		= 0;
	double	dStaCur	= 0;

	while(pos)
	{
		pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(pos);  
		if(n==nSec)
			break;
		n++;
	}	
	if(nType == 0)
	{
		double dSecCoe[6] = {0,0,};
		for(long nGComp=0; nGComp<pDB->GetGirdersu(); nGComp++)
		{
			if(nG == -1 || (nG > -1 && nG == nGComp))
			{
				long nSize = m_pCalcStd->m_vByunDanInfo[nGComp].size();
				for(nSec=0; nSec<nSize; nSec++)
				{
					dStaCur = m_pCalcStd->GetStationOnByundanInfo(nGComp,nSec);
					dStaCur = StressCalc.GetStationCheckPos(nGComp, dStaCur);	// ������ �����̼�
					GetSectionCoefficient(nGComp, dStaCur, FALSE, dStaCur, 1);

					dSecCoe[SEC_BU] = toM(m_MapSecCoe["FLANGE_W_UP"]);
					dSecCoe[SEC_TU] = toM(m_MapSecCoe["FLANGE_T_UP"]);
					dSecCoe[SEC_BL] = toM(m_MapSecCoe["FLANGE_W_DN"]);
					dSecCoe[SEC_TL] = toM(m_MapSecCoe["FLANGE_T_DN"]);
					dSecCoe[SEC_HW] = toM(m_MapSecCoe["WEB_H"]);
					dSecCoe[SEC_TW] = toM(m_MapSecCoe["WEB_T"]);

					if(pFrSec->m_dJewon[SEC_BU]==dSecCoe[SEC_BU] && pFrSec->m_dJewon[SEC_TU]==dSecCoe[SEC_TU] &&
					   pFrSec->m_dJewon[SEC_BL]==dSecCoe[SEC_BL] && pFrSec->m_dJewon[SEC_TL]==dSecCoe[SEC_TL] &&
					   pFrSec->m_dJewon[SEC_HW]==dSecCoe[SEC_HW] && pFrSec->m_dJewon[SEC_TW]==dSecCoe[SEC_TW])
					{
						nG		= nGComp;
						dSta	= dStaCur;
						return TRUE;
					}
				}
			}
		}
	}
	else if(nType == 1)
	{
		for(long nGComp = 0; nGComp < pDB->GetGirdersu()-1; nGComp++)
		{
			CPlateGirderApp *pGir = pDB->GetGirder(nGComp);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
			while(pBx)
			{
				GetSectionCoefficientCR(pBx);

				if(Round(frM4(pFrSec->m_dIx), 0) == Round(m_MapSecCoeCR["I33"], 0))
				{
					dSta	= pBx->GetStation();
					nG		= nGComp;
					return TRUE;
				}
				
				pBx = Finder.GetBxNext();
			}
		}
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////
///< I33���� �̿��Ͽ� �ܸ��� ������.I33
/////////////////////////////////////////////////////////////////////////
CFrameSection* CFemModelingCalc::GetFrameSectionByI33(CFEMManage *pFemData, double dSttJointI33)
{
	POSITION pos = pFemData->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec =  (CFrameSection*)pFemData->m_FrameSectionList.GetNext(pos);  
		if(IsSameI33(dSttJointI33, frM4(pFrameSec->m_dIx)))
			return pFrameSec;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////
/// Ⱦ�й踦 ������������ �� ��� ���� ����� ��ġ�� ��� Ⱦ�й豸���� �ִ��� ����
/////////////////////////////////////////////////////////////////////////
long CFemModelingCalc::GetHLLNoByElemID(long nElm)
{
	CCalcData *pData = m_pDataManage->GetCalcData();

	double dStaStt = GetStationByElmAndRatio(nElm, 0);
	double dStaEnd = GetStationByElmAndRatio(nElm, 1);
	
	for(long n=0; n<pData->m_HLsu; n++)
	{
		double dStaHL = pData->m_dStationHLLPos[n];
		if(dStaStt<=dStaHL && dStaHL<=dStaEnd)
			return n;
	}

	return 0;
}

long CFemModelingCalc::GetHLLNoByElemID(long nElm, BOOL bSttNode)
{
	CCalcData *pData = m_pDataManage->GetCalcData();
	if(pData == NULL || pData->m_HLsu == 1) return 0;

	double dStaNode = GetStationByElmAndRatio(nElm, bSttNode ? 0 : 1); // �Ŵ�Station

	if(pData->m_HLType == 0) // ����
	{
		double dStaTaperStt = GetStationTaper(nElm, TRUE);
		double dStaTaperEnd = GetStationTaper(nElm, FALSE);

		if(dStaNode < dStaTaperStt)	return 0;
		if(dStaNode > dStaTaperEnd)	return 2;

		return 1;
	}
	else // ��簡�κ�
	{
		// �����߽� station���� ã��
		for(long nHL=0; nHL<pData->m_HLsu; nHL++)
		{
			double dStaCR = pData->m_dStationHLLPos[nHL]; // �����߽�...	

			if(Compare(dStaNode, dStaCR, _T("="), 500.0))
			{
				return nHL;
			}
		}

		// MatchBx�� ã��
		CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

		long nG = GetGirderNoByElemID(nElm);
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateGirderApp *pGirCR = pDB->GetGirder(-1);

		CPlateBasicIndex *pBx = pGir->GetBxByStation(dStaNode, BX_CROSSBEAM_VBRACING|BX_SPLICE);
		CPlateBasicIndex *pBxMatch = pGir->GetBxMatchByCrossBeam(pBx, pGirCR);
		if(pBxMatch)
		{

			for(long nHL=0; nHL<pData->m_HLsu; nHL++)
			{
				double dStaCR = pData->m_dStationHLLPos[nHL]; // �����߽�...	

				if(Compare(pBxMatch->GetStation(), dStaCR, _T("="), 500.0))
				{
					return nHL;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ��ã�� ��� ���� ����� Ⱦ�й踦 ã�´�... : ��,����(MatchBx�� ã����) ����
		double dDistMin = 1000000.0;
		long nHL_Min = -1;
		for(long nHL=1; nHL<pData->m_HLsu-1; nHL++)
		{
			double dStaCR = pData->m_dStationHLLPos[nHL]; // �����߽�...	

			if(Compare(fabs(dStaNode - dStaCR), dDistMin, _T("<")))
			{
				dDistMin = fabs(dStaNode - dStaCR);
				nHL_Min = nHL;
			}
		}

		if(nHL_Min > -1)
			return nHL_Min;
	}

	return 0;
}

double CFemModelingCalc::GetStationTaper(long nElm, BOOL bStt)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

	long nG = GetGirderNoByElemID(nElm);
	CPlateGirderApp *pGir = pDB->GetGirder(nG);

	double dDis = 0;

	if(bStt)
	{
		double staSttStt = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_HUNCHSTT), dDis, pGir->GetAAA(OFF_STT_HUNCHSTT));//������ ������
		double staSttEnd = pGir->GetStationBySc(pGir->GetSSS(OFF_STT_HUNCHEND), dDis, pGir->GetAAA(OFF_STT_HUNCHEND));//������ ������
		double dStaTaperStt = (staSttStt + staSttEnd) / 2.;

		return dStaTaperStt;
	}
	else
	{
		double staEndEnd = pGir->GetStationBySc(pGir->GetSSS(OFF_END_HUNCHEND), dDis, pGir->GetAAA(OFF_END_HUNCHEND));//������ ������
		double staEndStt = pGir->GetStationBySc(pGir->GetSSS(OFF_END_HUNCHSTT), dDis, pGir->GetAAA(OFF_END_HUNCHSTT));//������ ������
		double dStaTaperEnd = (staEndStt + staEndEnd) / 2.;

		return dStaTaperEnd;
	}
}

BOOL CFemModelingCalc::IsTaperElement(long nElm, double &dRate)
{
	double dStaStt = GetStationByElmAndRatio(nElm, 0);
	double dStaEnd = GetStationByElmAndRatio(nElm, 1);

	double dStaTaperStt = GetStationTaper(nElm, TRUE);
	double dStaTaperEnd = GetStationTaper(nElm, FALSE);

	// ���������ۿ� �ɸ��� ���
	if(dStaStt < dStaTaperStt && dStaEnd > dStaTaperStt)
	{
		dRate = (dStaTaperStt - dStaStt) / (dStaEnd - dStaStt);
		return TRUE;
	}
	// ���������ۿ� �ɸ��� ���
	else if(dStaStt < dStaTaperEnd && dStaEnd > dStaTaperEnd)
	{
		dRate = (dStaTaperEnd - dStaStt) / (dStaEnd - dStaStt);
		return TRUE;
	}
	else
	{
		dRate = 1.0;
		return FALSE;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
/// �Է¹��� �ܸ��� ID�� ������ �ܸ� �����߿��� �ִ밪�� �߻��� ���� ��ġ�� ����.
/// nG == -1 �̸� ��ü �Ŵ�, nG >= 0 �̸� �����Ŵ����� ã��.
/// nSecIndex : �ܸ��� ID(1���� ����)
/// nReturnType=0 : �ִ� �����Ʈ ��ġ
/// nReturnType=1 : �ִ� �θ��Ʈ ��ġ
/// nReturnType=2 : �ִ� ���Ʈ ��ġ
/// nResultType 0,1���� �ش� ���Ʈ�� ���� ���� �����̼��� 0�̵�.
/// nG, dSta�� ���������� ���� �ܸ鱸���� nResultType�� �´� ��ġ
////////////////////////////////////////////////////////////////////////////////////////
long CFemModelingCalc::GetMaxForcePositionOfSection(long nSecIndex, long &nG, double &dSta, int nForceType, long nResultType, BOOL bSectionCheck)
{
	CPlateBridgeApp *pDB   = m_pDataManage->GetBridge();
	CFemStressCalc StressCalc(m_pDataManage, m_pCalcStd);

	long   nTermsu   = 0;
	long   nTempGir  = -1;
	double dForce = 0, dMaxForce = 0, dRatio = 0;
	double dStaStt = 0, dStaEnd = 0;
	// �ܸ鱸���� ����� �ȵǾ� �ִ� ��� ������ ���
	if(m_pCalcStd->GetSizeByundanInfo(0) == 0)
	{
		StressCalc.SetSectionGugan();
		StressCalc.SetSectionSplice();
		for(long n=0; n<pDB->GetGirdersu(); n++)
			StressCalc.MakeDanmyunMomentData(n);
	}
	// ��� �Ŵ� ��ȯ ��ƾ.
	for(long nCurGir=0; nCurGir<pDB->GetGirdersu(); nCurGir++)   
	{
		if (nG != -1 && nG != nCurGir)   // �ش� �Ŵ��� �ƴϸ�, nG�� -1�̸� ��ü�Ŵ� �˻�
			continue;

		dStaStt = pDB->GetGirder(nCurGir)->GetBxOnJijum(0)->GetStation();//����Ŵ��� ���۽����̼�
		nTermsu = m_pCalcStd->GetSizeByundanInfo(nCurGir);				 //����Ŵ��� �ܸ鱸����	

		for (long nTerm = 0; nTerm < nTermsu-1; nTerm++)
		{
			CDanMyunProp *pSectI = &m_pCalcStd->m_vByunDanInfo[nCurGir][nTerm];		// ������ ���۴ܸ�
			CDanMyunProp *pSectJ = &m_pCalcStd->m_vByunDanInfo[nCurGir][nTerm+1];	// ������ ���ܸ�

			dStaEnd	= pSectJ->m_pBx->GetStation();

			BOOL bByun     = m_pCalcStd->IsByunDanMyunOnByundanInfo(nCurGir, nTerm);
			BOOL bNextByun = FALSE;
			if(nTerm+1 < nTermsu-1)
				bNextByun = m_pCalcStd->IsByunDanMyunOnByundanInfo(nCurGir, nTerm+1);

			long nCurSecIndex = m_pCalcStd->GetIndexOnByundanInfo(nCurGir, nTerm);  //����Ŵ� �ܸ鱸���� �ܸ��̸�
			if(!bSectionCheck)	nCurSecIndex	= pSectI->m_MatRight.m_nIdx;

			if(!pSectI->m_MatRight.m_bUsedCalc && pSectI->m_MatLeft.m_bUsedCalc && nTerm>0 && bSectionCheck)
			{
				dStaStt = m_pCalcStd->m_vByunDanInfo[nCurGir][nTerm-1].m_pBx->GetStation();
				dStaEnd = m_pCalcStd->m_vByunDanInfo[nCurGir][nTerm].m_pBx->GetStation();
			}
			if(nCurSecIndex == nSecIndex-1)   //�Է¹��� �ܸ�� ���� �ܸ��� ��ġ�� ���
			{ 
				// ���� ���ܸ��̸�	
				if(bByun || bNextByun)
				{
					if(dRatio == 0)
					{
						// �ܸ� �������� SectionID�� �ٸ��� �������� ��ġ, ������ �������� ��ġ��
						//if(StressCalc.GetSectionIDByJoint(nJointIdx) != nSecIndex)	dStaEnd = dStaStt; 
						//else							 								dStaStt = dStaEnd;
						if(pSectI->m_MatRight.m_nIdx!=pSectJ->m_MatLeft.m_nIdx)			dStaEnd = dStaStt;
						else															dStaStt = dStaEnd;
					}
				}
				//GetElementForceTerm()���� nResultType�� 4�̻��� ���� nLC�� �ǹ̾���.
				if(nResultType==MAX_POSITIVE)
					dForce = StressCalc.GetElementForceTerm(nForceType, 0, nCurGir, dStaStt, dStaEnd, 5);  // �հ��� �����Ʈ��.
				else if(nResultType==MAX_NEGATIVE)
					dForce = StressCalc.GetElementForceTerm(nForceType, 0, nCurGir, dStaStt, dStaEnd, 6);  // �հ��� �θ��Ʈ��.
				else if(nResultType==MAX_ABS)
					dForce = StressCalc.GetElementForceTerm(nForceType, 0, nCurGir, dStaStt, dStaEnd, 4);  // ���� �հ��� �ִ밪.
				// �Ŵ� �������ο��� �̷��� �θ��Ʈ�� ������.
				if(dForce<0 && dForce>-1)
					dForce = 0;
				// nCurGir�� dStaStt�� dStaEnd���̿� �����Ʈ�� ������ dForce=0 �� 
				if( dMaxForce==0 || (nResultType==MAX_ABS	&& fabs(dForce) > fabs(dMaxForce)) || (nResultType==MAX_POSITIVE && dForce > dMaxForce) || (nResultType==MAX_NEGATIVE && dForce < dMaxForce))   // �θ��Ʈ�� ������ ������
				{
					dMaxForce = dForce;
					nTempGir  = nCurGir;
					dSta      = dStaStt;
				}		
				// GetElementForceTerm()���� �ش����ǿ� ���� ������ ���� ��� dForce==0 ���ϵ�
				//if(dForce==0)	dSta = 0;
			}
			dStaStt = dStaEnd;
		}
	}
	if (nTempGir == -1)   // ã�� �ܸ��� ����.
		return -1;

	nG = nTempGir;

	return GetJointNoByStation(nG, dSta, dRatio);
}

long CFemModelingCalc::GetMaxFatigueIndex(long nType, long &nEleIdx, double &dRatio, long nLC)
{
	CPlateBridgeApp *pDB     = m_pDataManage->GetBridge();
	CFEMManage      *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS); 

	CFemStressCalc StressCalc(m_pDataManage, m_pCalcStd);

	long nKsu = pDB->m_nQtyJigan;
	if (nType == 2 && nKsu <= 1)		return -1;
	if (nType == 3 && nKsu <= 2)		return -1;

	long nG = -1, nK, nNode = -1, nMaxNode = -1;
	double dMaxDiffDB = 0.0, dMaxDiffDL = 0.0; 
	double dDBMax = 0.0, dDBMin = 0.0, dDLMax = 0.0, dDLMin = 0.0;
	
	for (long i=0; i<pManage->GetElementSize(); i++)  
	{
		CElement *pElm = pManage->GetElement((unsigned short)i);
		
		if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_GIRDER_ELEMENT)
		{
			if (nG != GetGirderNoByJointID(nNode+1)) // �Ŵ��� ���κκ�
			{  
				nG = GetGirderNoByJointID(nNode+1);
				nK = -1;
				nNode++;
			}

			for (long nPoint = 0; nPoint < 2; nPoint++)   
			{
				BOOL bNodeStt = (nPoint == 0);
				if (bNodeStt == FALSE)
					nNode++;
				double dMaxDB, dMinDB, dMaxDL, dMinDL;

				if (nType == 4)    //����
				{
					if(nLC==1)
					{
						dMaxDB = StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DB_MAX, i, 0);
						dMinDB = StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DB_MIN, i, 0);
					}
					else if(nLC==2)
					{
						dMaxDL = StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DL_MAX, i, 0);
						dMinDL = StressCalc.GetOneEleForce(ELE_SHEAR_2, LC_DL_MIN, i, 0);
					}
				}
				else
				{
					BOOL bValid = FALSE;
					dMaxDB = dMinDB = dMaxDL = dMinDL = 0.0;
					BOOL bFixJijum    = IsRestraintJoint(nNode);
					//BOOL bFixJijumPre = IsRestraintJoint(nNode-1); //APLATE-737 ����� ������ ���� ����
					if(bFixJijum)   // ������
					{
						if (nType == 2)
							bValid = TRUE;
						if (bNodeStt)
							nK++;
					}
					else 
					{
						if (nType == 1 && (nK == 0 || nK == nKsu - 1))
							bValid = TRUE;
						if (nType == 3 && (nK > 0 && nK < nKsu - 1))
							bValid = TRUE;
					}
					if (bValid)  
					{
						if(nLC==1)
						{
							dMaxDB = StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MAX, i, bNodeStt ? 0 : 1);
							dMinDB = StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DB_MIN, i, bNodeStt ? 0 : 1);
						}
						else if(nLC==2)
						{
							dMaxDL = StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DL_MAX, i, bNodeStt ? 0 : 1);
							dMinDL = StressCalc.GetOneEleForce(ELE_MOMENT3, LC_DL_MIN, i, bNodeStt ? 0 : 1);
						}
					}
				}

				if (nLC==1 && dMaxDB - dMinDB > dMaxDiffDB)
				{
					if(nType==1 || nType==3)
					{
						if(dMaxDB > dDBMax)
						{						
							dMaxDiffDB = dMaxDB - dMinDB;
							nEleIdx = i;
							dRatio = bNodeStt ? 0 : 1;
							nMaxNode = nNode;
							dDBMax = dMaxDB;
						}
					}
					else if(nType==2)
					{
						if(dMinDB < dDBMin)
						{						
							dMaxDiffDB = dMaxDB - dMinDB;
							nEleIdx = i;
							dRatio = bNodeStt ? 0 : 1;
							nMaxNode = nNode;
							dDBMin = dMinDB;
						}
					}
					else
					{
						dMaxDiffDB = dMaxDB - dMinDB;
						nEleIdx = i;
						dRatio = bNodeStt ? 0 : 1;
						nMaxNode = nNode;
					}
				}
				if (nLC==2 && dMaxDL - dMinDL > dMaxDiffDL)
				{
					if(nType==1 || nType==3)
					{
						if(dMaxDL > dDLMax)
						{
							dMaxDiffDL = dMaxDL - dMinDL;
							nEleIdx = i;
							dRatio = bNodeStt ? 0 : 1;
							nMaxNode = nNode;
							dDLMax = dMaxDL;
						}
					}
					else if(nType==2)
					{
						if(dMinDL < dDLMin)
						{
							dMaxDiffDL = dMaxDL - dMinDL;
							nEleIdx = i;
							dRatio = bNodeStt ? 0 : 1;
							nMaxNode = nNode;
							dDLMin = dMinDL;
						}
					}
					else
					{
						dMaxDiffDL = dMaxDL - dMinDL;
						nEleIdx = i;
						dRatio = bNodeStt ? 0 : 1;
						nMaxNode = nNode;
					}

				}
			}
		}
	}

	return nMaxNode;
}

///////////////////////////////////////////////////////////////////////
/// dSta�� ��ġ�� ����� ���������̸� ��������, ���������̸� �� ����
///////////////////////////////////////////////////////////////////////
long CFemModelingCalc::GetJointNoByStation(long nG, double dSta, double& dRatio)
{
	CFEMManage *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	long nElm = GetElemNoAndRatio(nG, dSta, dRatio);
	long nNodeIdx = -1;
	if (dRatio <= 0.5)   
	{
		nNodeIdx = pManage->GetElement((unsigned short)nElm)->m_nJStt;
	}
	else   
	{
		nNodeIdx = pManage->GetElement((unsigned short)nElm)->m_nJEnd;
		dRatio -= 1.0;
	}
	return nNodeIdx;
}

///////////////////////////////////////////////////////////////
/// ���� ������ ��ġ�� ������.
///////////////////////////////////////////////////////////////
void CFemModelingCalc::RemoveDupTypedArray(CTypedPtrArray < CObArray, CPlateBasicIndex* > &DArrBx)
{
	if(DArrBx.GetSize()==0) return;

	CTypedPtrArray < CObArray, CPlateBasicIndex* > DArrBxCopy;
	CDWordArray DeleteIdx;
	CPlateBasicIndex *pBx    = NULL;
	CPlateBasicIndex *pBxOrg = DArrBx[0];
	long nG, nGOrg, nSize;
	double dSta, dStaOrg;

	DArrBxCopy.Add(pBxOrg);
	long  i = 0;
	for(i = 1; i < DArrBx.GetSize(); i++)
	{
		pBxOrg = DArrBx[i];
		nGOrg = pBxOrg->GetNumberGirder();
		dStaOrg = pBxOrg->GetStation();

		nSize = DArrBxCopy.GetSize();
		for(long j = 0; j < nSize; j++)
		{
			pBx = DArrBxCopy[j];
			nG = pBx->GetNumberGirder();
			dSta = pBx->GetStation();

			if(nG == nGOrg && fabs(dSta - dStaOrg) <= 0.1)
			{
//				DeleteIdx.Add(i);
				break;
			}
			else if(j == nSize - 1)	DArrBxCopy.Add(pBxOrg);
		}
	}

	DArrBx.RemoveAll();
	for(i = 0; i < DArrBxCopy.GetSize(); i++)
	{
		pBxOrg = DArrBxCopy[i];
		DArrBx.Add(pBxOrg);
	}

}


// �����극�̽� CFrameSection �� pFrSec->m_szName �� pElm->m_szFrameSectionName�� ������ �����극�̽� �� ���Ʈ�� ���� ū ��
void CFemModelingCalc::GetBxArrayOfVBracing(CTypedPtrArray < CObArray, CPlateBasicIndex* > &DArrVBracingBx)
{
	CPlateBasicIndex	*pBx = NULL;
	CPlateBasicIndex	*pBxMax = NULL;
	CSection			*pSec	= NULL;
	CFEMManage			*pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	BOOL bSame = FALSE;
	double dLength = 0, dLengthMax = 0;

	POSITION pos = pManage->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrSec =  (CFrameSection*)pManage->m_FrameSectionList.GetNext(pos);  
		if(pFrSec->m_nFlag==POS_CROSSBEAM)
		{
			dLengthMax = 0;
			pBxMax     = NULL;

			for(long nElm = 0; nElm < pManage->GetElementSize(); nElm++)
			{
				CElement *pElm = pManage->GetElement((unsigned short)nElm);
				
				if(pElm->m_LongAttrib[ELEMENT_TYPE] & FRAME_CROSS_ELEMENT)
				{
					if(pFrSec->m_szName == pElm->m_szFrameSectionName)
						bSame = TRUE;

					if(!bSame)	continue;
					else		bSame = FALSE;
					
					pBx = m_pCalcStd->GetBxByJoint(pManage->GetJoint(pElm->m_nJStt));
					pSec = pBx->GetSection();

					if(pSec->GetenDetType()==SECDET_VBRACING)
					{
						dLength = GetElemLength(nElm);

						if(dLength > dLengthMax)
						{
							dLengthMax = dLength;
							pBxMax = pBx;
						}
					}
				}
			}
			if(pBxMax != NULL)
				DArrVBracingBx.Add(GetBxVBracingMaxMomentByMatType(pBxMax));
		}
	}
}

long CFemModelingCalc::GetShearChainSu(long nG, long nSttNodeNo, long nEndNodeNo)
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir		= pDB->GetGirder(nG);
	CFEMManage	*pManage	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	long	nStudSu		= pGir->m_pSangse->m_StudHori_Qty;
	long	nMaxJointNO	= 0;
	long	nMinJointNO	= 0;
	long	nSumGaeSu	= 0;
	double	dSumSta		= 0;
	GetJointRangeByGirder(nG, nMinJointNO, nMaxJointNO);
	// �Ŵ��� ���� Station
	double	dSttofGir	= pManage->GetJoint((unsigned short)nMinJointNO)->m_vPointReal.x;
	double	dSttSta		= pManage->GetJoint((unsigned short)nSttNodeNo-1)->m_vPointReal.x - dSttofGir;
	double	dEndSta		= pManage->GetJoint((unsigned short)nEndNodeNo-1)->m_vPointReal.x - dSttofGir;

	for(long i=0; i<STUD_JONG_SU; i++)
	{
		double dSta   = pGir->m_pSangse->m_StudJong_StudDis[i];
		double dGaeSu = pGir->m_pSangse->m_StudJong_Studsu[i];
		
		if(dSta<=0 || dGaeSu<=0) break;

		for(long j=0; j<dGaeSu; j++)
		{
			if((dSumSta >= dSttSta) && (dSumSta <= dEndSta))
				++nSumGaeSu;

			if(dSumSta>dEndSta) break;
			
			dSumSta += dSta;
		}
		if(dSumSta>dEndSta) break;
	}
	
	return nSumGaeSu*nStudSu;
}

// Ȱ������ ó�����並 ���� ó���� ����.
// (nLoadCase = -1 : ��� LoadCase�� �ִ밪,  nNodIdx = -1 : ��� ������ �ִ밪)
double CFemModelingCalc::GetLiveLoadDisplace(int nLoadCase, int nNodIdx)
{
	CFEMManage *pManage = m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);
	BOOL bFirst = TRUE;
	double dValue, dMaxValue = 0.0;
	int nNodeSu = pManage->GetJointSize();
	for (int nCurNode = 0; nCurNode < nNodeSu; nCurNode++)   
	{  // �ִ� ó���� ����.
		if (nNodIdx != -1 && nNodIdx != nCurNode)
			continue;
		int nLCSu = pManage->GetLoadCaseSize();
		for (int nCurLC = 0; nCurLC < nLCSu; nCurLC++)
		{
			if (nLoadCase != -1 && nLoadCase != nCurLC)//���� Loadcase�� �ƴѰ��
				continue;
			dValue = pManage->GetJoint(nCurNode)->GetResultForce(nCurLC)->m_Disp.z;
			if (bFirst || fabs (dValue) > fabs (dMaxValue))   
			{
				dMaxValue = dValue;
				bFirst = FALSE;
			}
		}
	}
	return dMaxValue;

}

// ��, ���� �����Ͻ� �ִ�ݷ°��� ����
// Ȱ������ �� LoadCase�� ���� �ݷ� ����.		   
// nLoadType :   1   2   3   4     5        6
//              DB  DL  TK  TR  ��������  ǳ����
//								15 : ������������
//								16 : ������������
// nLoadType > 0 : Max
//           < 0 : Min
// bLeftMax : TRUE(���������Ͻ�), FALSE(���������Ͻ�)
// nLane    : �ش� ������
// nNodeIdx : ����ȣ
// bApplyI  : ��ݰ�� ����
double CFemModelingCalc::GetLiveLoadReaction(long nLoadType, long nLoadGir, long nLane, long nNodIdx, BOOL &bExist)
{	
	CFemStressCalc StressCalc(m_pDataManage, m_pCalcStd);
	CFEMManage *pManage = m_pCalcStd->GetModelingFemData(LIVE_LOAD_ANALYSIS);	
	
	// �ش� LoadCase�� �̸��� ����.
	CString sLCName, sV;
	if(abs(nLoadType) <= 6)
	{
		switch (abs(nLoadType))
		{
		case 1 :	sV = "DB";			break;
		case 2 :	sV = "DL";			break;
		case 3 :	sV = "TANK";		break;
		case 4 :	sV = "TRAIL";		break;
		case 5 :	sV = "PEOPLE";		break;		
		case 6 :	sV = "WIND";		break;
			return 0.0;
		}	
		if(sV == "TANK" || sV == "TRAIL")
			sLCName.Format("G%d%s",nLoadGir + 1, sV);
		else
		{
			if(abs(nLoadType)==5 || abs(nLoadType)==6)
				sLCName = sV;
			else
			{
				if(nLoadGir==0)	sLCName.Format("%sL-%d", sV, nLane+1);
				else			sLCName.Format("%sR-%d", sV, nLane+1);
//				sLCName.Format("G%d%s%d",nLoadGir + 1, sV, nLane+1);
			}
		}
		if(nLoadType > 0)	sLCName += " MAX";
		else				sLCName += " MIN";
	}
	else
	{
		if(nLoadType == 7)			sLCName = "LWIND";
		else if(nLoadType == 8)		sLCName = "RWIND";
		else if(nLoadType == 9)		sLCName = "LPEOPLE";
		else if(nLoadType == 10)	sLCName = "RPEOPLE";
	}

	int nLC = pManage->GetLoadCaseIndex((LPCTSTR) sLCName);
	if (nLC < 0)   
	{
		bExist = FALSE;
		return 0.0;
	}
	bExist = TRUE;
	
	double dValue = StressCalc.GetOneNodForceSub(pManage, NOD_REACTION, nLC, nNodIdx, 'Z');	
	if (nLoadType == 5)
		dValue = max(dValue, 0.0);
	if (nLoadType == -5)
		dValue = min(dValue, 0.0);
	return dValue;
}

double CFemModelingCalc::GetMapValueSecCo(CString sz)
{
	return m_MapSecCoe[sz];
}

double CFemModelingCalc::GetMapValueSecCoCR(CString sz)
{
	return m_MapSecCoeCR[sz];
}


long CFemModelingCalc::GetJiganNoByJointID(long nJoint)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CFEMManage		*pManage	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CPlateBasicIndex	*pBx		= m_pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nJoint));
	CPlateGirderApp		*pGir		= pBx->GetGirder();

	double dSta = pBx->GetStation();

	for(long n=0; n<pDB->m_nQtyJigan; n++)
	{
		double dStaStt = pGir->GetBxOnJijum(n)->GetStation();
		double dStaEnd = pGir->GetBxOnJijum(n+1)->GetStation();
		if(dStaStt <= dSta && dSta <= dStaEnd)
			return n;
	}
	return -1;
}

// nForceCase : �������� (JOINT_REACTION, JOINT_DISPLACEMENT)
// bPier      : ����� �Ǵ� ������ �� ����
// ����� �Ǵ� ������ ��  �����ݷ� �Ǵ� ����ħ�ϰ� ���� ū ���� 
CJoint* CFemModelingCalc::GetJointMaxForceAtJijum(long nForceCase, BOOL bPier)
{
	CFEMManage	*pManage	= m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CResultArray	*pResult	= m_pCalcStd->GetModelingResult(BEFORE_COMPOSITE_ANALYSIS);
	CDPointArray vArr;
	double dReaction = 0;

	for(long nJ=0; nJ<pManage->GetJointSize() ; nJ++)
	{
		CJoint		*pJ			= pManage->GetJoint((unsigned short)nJ);
		CJointResult	*pJResult	= pResult->m_JointResultArr.GetAt(nJ);
		if(pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE)
		{
			CPlateBasicIndex *pBx = m_pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nJ));

			if(bPier && pBx->IsPier())
			{
				dReaction = maxabs(pJResult->m_vTotalMax[nForceCase].z, pJResult->m_vTotalMin[nForceCase].z);
				vArr.Add(CDPoint(nJ, fabs(dReaction)));
			}
			else if(bPier==FALSE && pBx->IsAbut())
			{
				dReaction = maxabs(pJResult->m_vTotalMax[nForceCase].z, pJResult->m_vTotalMin[nForceCase].z);
				vArr.Add(CDPoint(nJ, fabs(dReaction)));
			}
		}		
	}
	vArr.Sort(FALSE, FALSE);

	if(vArr.GetSize()==0)	return NULL;
	return pManage->GetJoint((unsigned short)vArr.GetAt(0).x);
}


long CFemModelingCalc::GetShoeJointNoByBx(CPlateBasicIndex *pBx)
{
	if(pBx->IsJijum()==FALSE)	return -1;

	CFEMManage       *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);    

	for(long nJ=0; nJ<pManage->GetJointSize(); nJ++)
	{
		CJoint *pJ = pManage->GetJoint((unsigned short)nJ);
		if((pJ->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE) && m_pCalcStd->GetBxByJoint(pJ) == pBx)
			return pJ->m_nIdx;
	}

	return -1;
}

// nJoint�� ������ �������� ����� ���������� ���ӻ��¸�
// CJoint::IsRestraint()�Լ��� �̿��Ͽ� �Ǻ� �� ���������� ������
//   ����: ����ġ�� ������ �������̱� ����
// nJoint�� ���������� ���� CJoint::IsRestraint()�� ������.
BOOL CFemModelingCalc::IsRestraintJoint(long nJoint)
{
	if(nJoint<0)
		return FALSE;

	CFEMManage *pManage = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);    

	if(pManage->GetJoint((unsigned short)nJoint)->m_LongAttrib[JOINT_TYPE] & FRAME_SHOE_CENT_NODE)	// ���� ����
	{
		CJoint *pJ = pManage->GetJoint((unsigned short)nJoint);
		return pJ->IsRestraintJoint();
	}
	else						// �Ŵ������ӳ��� ����(��� ��������)
	{
		CPlateBasicIndex *pBx = m_pCalcStd->GetBxByJoint(pManage->GetJoint((unsigned short)nJoint));
		if(pBx->IsJijum())
		{
			long nJShoe = GetShoeJointNoByBx(pBx);
			CJoint *pJ = pManage->GetJoint((unsigned short)nJShoe);
			if(pJ)	return pJ->IsRestraintJoint();
			else	return FALSE;
		}
		else
			return FALSE;

	}
}

long CFemModelingCalc::GetFrameAnalysisSize()
{
	if(m_pDataManage->GetBridge()->IsTUGir())	return 5;
	else										return 4;

	return 0;
}

void CFemModelingCalc::MakeFrameModeling()
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();
	CTStandard		*pTS	= m_pDataManage->GetBMStandard()->m_pTStandard;
	CFemStressCalc StressCalc(m_pDataManage, m_pCalcStd);

	// ���¹�
	CRect rect = CRect(200, 3, 400, 18);
	if(!m_ctlProgress.GetSafeHwnd())
		m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_ctlProgress.SetRange(0, 100);
	m_ctlProgress.ShowWindow(SW_SHOW);	
	pTS->SetPaneText(0, "3D Frame �𵨸� ������...", TRUE);
//	m_ctlProgress.SetPos(0);

	CStringArray	szProject;
	CDWordArray		arrAnalysisType;
	if(pDB->IsTUGir())
	{
		szProject.Add(" BEFORE COMPOSITE SELF-WEIGHT LOAD - 1 ");
		szProject.Add(" BEFORE COMPOSITE SELF-WEIGHT LOAD - 2 ");
		szProject.Add(" AFTER COMPOSITE SELF-WEIGHT LOAD ");
		szProject.Add(" MOVING LOAD ");
		szProject.Add(" UNEQUAL SETTLEMENT ");

		arrAnalysisType.Add(BEFORE_COMPOSITE_ANALYSIS);
		arrAnalysisType.Add(BEFORE_TURN_COMPOSITE_ANALYSIS);
		arrAnalysisType.Add(AFTER_COMPOSITE_ANALYSIS);
		arrAnalysisType.Add(LIVE_LOAD_ANALYSIS);
		arrAnalysisType.Add(SETTLEMENT_ANALYSIS);
	}
	else
	{
		szProject.Add(" BEFORE COMPOSITE SELF-WEIGHT LOAD ");
		szProject.Add(" AFTER COMPOSITE SELF-WEIGHT LOAD ");
		szProject.Add(" MOVING LOAD ");
		szProject.Add(" UNEQUAL SETTLEMENT ");

		arrAnalysisType.Add(BEFORE_COMPOSITE_ANALYSIS);
		arrAnalysisType.Add(AFTER_COMPOSITE_ANALYSIS);
		arrAnalysisType.Add(LIVE_LOAD_ANALYSIS);
		arrAnalysisType.Add(SETTLEMENT_ANALYSIS);
	}

	/////////////////////////////////////////////////////////////////////////
	// ������ ������ ������ �𵨸� ����
	m_pCalcStd->RemoveModelingFemData();
	m_pCalcStd->MakeModelingFemData(GetFrameAnalysisSize());

	for(long i=0; i<GetFrameAnalysisSize(); ++i)
	{
		m_ctlProgress.SetPos((int)(((double)i+1)/((double)GetFrameAnalysisSize())*100));

		long	nAnalysisType	= arrAnalysisType.GetAt(i);
		long	nLoadCase		= 0;

		/////////////////////////////////////////////////////////////////////////
		// ������ �����ڷ� ����
		CFEMManage *pFEM = m_pCalcStd->GetModelingFemData(m_pCalcStd->GetAnalysisTypeByIndex(i));
		pFEM->RemoveAllData();
		pFEM->m_bOutRemarkText	= TRUE;
		pFEM->m_szMidasVersion	= MIDAS_VER_4DOT3;
		pFEM->m_nStructureType	= STRUCTYPE_3D;
		pFEM->SYSTEM_VAR.m_szUnitType = "KN";
		pFEM->SYSTEM_VAR.m_UX = pFEM->SYSTEM_VAR.m_UY = pFEM->SYSTEM_VAR.m_UZ = 0;
		pFEM->SYSTEM_VAR.m_RX = pFEM->SYSTEM_VAR.m_RY = pFEM->SYSTEM_VAR.m_RZ = 0;	
		pFEM->m_nExtraInfo		= nAnalysisType;
		pFEM->m_szFEMName		= szProject.GetAt(i);
		/////////////////////////////////////////////////////////////////////////
		// �𵨸� ����
		MakeJoint(pFEM);
		MakeElement(pFEM);
		MakeMaterial(pFEM);	
		long nSection = MakeFrameSection(pFEM, nAnalysisType);
		MakeElementDefineSection(pFEM, nSection, nAnalysisType);

		switch(nAnalysisType)
		{
		case BEFORE_COMPOSITE_ANALYSIS :
			MakeForcePreDead(pFEM, nAnalysisType, nLoadCase);
			break;
		case BEFORE_TURN_COMPOSITE_ANALYSIS :
			MakeForcePreDead_Turn(pFEM, nAnalysisType, nLoadCase);
			break;
		case AFTER_COMPOSITE_ANALYSIS :
			MakeForcePostDead(pFEM, nLoadCase);
			break;
		case LIVE_LOAD_ANALYSIS :
			{
				MakeForceLive(pFEM, nLoadCase);	
				if(pData->m_bWind && pData->m_bRunCrossWind)
					MakeForceWind(pFEM, nLoadCase);	
				MakeLane(pFEM);
				MakeVehicleClass(pFEM);	
				MakeVehicle(pFEM);
				MakeMovingLoad(pFEM);
				MakeMovingLoadCombo(pFEM);
			}
			break;
		case SETTLEMENT_ANALYSIS :
			{
				if(pData->m_bCheckSettlement)
				{
 					MakeUZPLoad(pFEM);
 					MakeUZPCombo(pFEM);
				}
			}
			break;
		}
	}

	m_ctlProgress.ShowWindow(SW_HIDE);	
	pTS->SetPaneText(0,"",TRUE);
	
	pData->m_bMakeModeling            = TRUE;
	pData->m_bReadFrameModelingOutput = FALSE;
	pData->m_bReadLiveDispOutput      = FALSE;

	AfxGetMainWnd()->SendMessage(WM_USER+15, 1);
}

// szLC�� Ⱦ�й� ��� �� Ȱ���� MAX�� ����ΰ�?
BOOL CFemModelingCalc::IsLiveCombMaxLC(CString szLC)
{
	if(szLC.Find("HAND") != -1)	return FALSE;
	if(szLC.Find("SOUND") != -1) return FALSE;
	if(szLC.Find("DB") != -1 || szLC.Find("DL") != -1 ||  szLC.Find("TRAIL") != -1 || szLC.Find("TANK") != -1)
		return TRUE;

	return FALSE;
}

double CFemModelingCalc::GetHeightWeb(CPlateBasicIndex *pBx, long nPos)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir	= pBx->GetGirder();

	double dSta = pBx->GetStation();
	double dUpperThick = 0;
	double dLowerThick = 0;
	
	CPlateBasicIndex *pBxUpper = pGir->GetBxByStationDir(dSta, -1, BX_UPPERFLANGE);
	CPlateBasicIndex *pBxLower = pGir->GetBxByStationDir(dSta, -1, BX_LOWERFLANGE);

	CPlateBxFinder Finder(pGir);
	if(!pBxUpper) pBxUpper = Finder.GetBxFirst(BX_STT_GIRDER);
	if(!pBxLower) pBxLower = Finder.GetBxFirst(BX_STT_GIRDER);

	if(pBxUpper && pBxUpper->GetFactChain(G_F_U))
	{
		CPlateFactChain *pFactPre	= pBx->GetFactChain(G_F_U, FALSE);
		CPlateFactChain	*pFactNext	= pBx->GetFactChain(G_F_U, TRUE);

		double	dThickPre	= pFactPre ? pFactPre->m_dFactChainThick : 0;
		double	dThickNext	= pFactNext ? pFactNext->m_dFactChainThick : 0;

		if(nPos==-1)		dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? dThickPre : 0;
		else if(nPos==1)	dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? dThickNext : 0;
		else				dUpperThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseUpper() == 0) ? min(dThickPre, dThickNext) : 0;
	}
	
	if(pBxLower && pBxLower->GetFactChain(G_F_L))
	{
		CPlateFactChain *pFactPre	= pBx->GetFactChain(G_F_L, FALSE);
		CPlateFactChain	*pFactNext	= pBx->GetFactChain(G_F_L, TRUE);

		double	dThickPre	= pFactPre ? pFactPre->m_dFactChainThick : 0;
		double	dThickNext	= pFactNext ? pFactNext->m_dFactChainThick : 0;

		if(nPos==-1)		dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? dThickPre : 0;
		else if(nPos==1)	dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? dThickNext : 0;
		else				dLowerThick = (CDataManage::GetGlobalOption()->GetDesignHeigtBaseLower() == 1) ? min(dThickPre, dThickNext) : 0;
	}

	double	dHeightJijum	= pGir->GetHeightFlangeLower(dSta);
	double	dHeightTaper	= pDB->IsTUGir() ? 0 : pGir->GetHeightTaper(dSta);
	double	dHeightWeb		= dHeightJijum - (dUpperThick + dLowerThick + dHeightTaper);

	return dHeightWeb;
}

double CFemModelingCalc::GetThickFlange(long nG, double dSta, long nMarkBuje, long nPos)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);

	return GetThickFlange(pBx, nMarkBuje, nPos);
}

double CFemModelingCalc::GetThickFlange(CPlateBasicIndex *pBx, long nMarkBuje, long nPos)
{
	CPlateFactChain	*pFactPre	= pBx->GetFactChain(nMarkBuje, FALSE);
	CPlateFactChain	*pFactNext	= pBx->GetFactChain(nMarkBuje, TRUE);

	double	dThickPre	= pFactPre ? pFactPre->m_dFactChainThick : 0;
	double	dThickNext	= pFactNext ? pFactNext->m_dFactChainThick : 0;

	if(nPos==-1)		return dThickPre;
	else if(nPos==1)	return dThickNext;
	else			    return min(dThickPre, dThickNext);
}

double CFemModelingCalc::GetWidthFlange(long nG, double dSta, long nMarkBuje, long nPos)
{
	CPlateBridgeApp *pDB     = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	
	BOOL	bUpper	= TRUE;
	if(nMarkBuje==G_F_U)		bUpper = TRUE;
	else if(nMarkBuje==G_F_L)	bUpper = FALSE;
	double dWidthPre = pGir->GetWidthOnStation(dSta, bUpper, FALSE);
	double dWidthNxt = pGir->GetWidthOnStation(dSta, bUpper, TRUE);

	if(nPos==-1)		return dWidthPre;
	else if(nPos==1)	return dWidthNxt;
	else			    return min(dWidthPre, dWidthNxt);
}

double CFemModelingCalc::GetWidthFlange(CPlateBasicIndex *pBx, long nMarkBuje, long nPos)
{
	return GetWidthFlange(pBx->GetNumberGirder(), pBx->GetStation(), nMarkBuje, nPos);	
}

CFrameSection* CFemModelingCalc::GetFrameSectionByPrpty(CFEMManage *pFemData, CDPoint *Prpty, CString *Material)
{
	POSITION pos = pFemData->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrameSec = (CFrameSection*)pFemData->m_FrameSectionList.GetNext(pos);  
		CDPoint xyUpper(frM(pFrameSec->m_dJewon[SEC_BU]), frM(pFrameSec->m_dJewon[SEC_TU]));
		CDPoint xyLower(frM(pFrameSec->m_dJewon[SEC_BL]), frM(pFrameSec->m_dJewon[SEC_TL]));
		CDPoint xyWeb(frM(pFrameSec->m_dJewon[SEC_HW]), frM(pFrameSec->m_dJewon[SEC_TW]));

		if(xyUpper==Prpty[0] && xyLower==Prpty[1] && Round(xyWeb,4) == Round(Prpty[2],4))
		{
 			return pFrameSec;
			// APLATE-734
			// �����ٸ��� ������ ���� ��� �������� �ʰ� ����..
// 			if(!Material)	return pFrameSec;
// 			if(pFrameSec->m_szJewon[0]==Material[0] && pFrameSec->m_szJewon[1]==Material[1] && pFrameSec->m_szJewon[2]==Material[2])
// 				return pFrameSec;
		}
	}
	return NULL;
}

// �������� ����
double CFemModelingCalc::GetWidthSlab(long nG, double dSta)
{
	CPlateBridgeApp *pDB     = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir    = pDB->GetGirder(nG);
	CPlateBasicIndex *pBx    = pGir->GetBxByStation(dSta);

	double	dWidthSlab	= 0;
	//	������ ���� �߾Ӻ����� �Ǵ�
	if(pDB->GetGirdersu()==1) 
	{
		dWidthSlab = pDB->m_dWidthSlabLeft+pDB->m_dWidthSlabRight;
	}
	else if(nG == 0 || nG == -1)
	{
		dWidthSlab = pDB->m_dWidthSlabLeft + pGir->GetDisGirderToGirder(pBx)/2;
	}
	else if(nG==pDB->GetGirdersu()-1)
	{
		CPlateBasicIndex *pBxPreMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);

		dWidthSlab = pDB->m_dWidthSlabRight + pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch)/2;
	}
	else
	{
		CPlateBasicIndex *pBxPreMatch = pBx->GetBxMatchByCrossBeam(pGir->m_pLeft);
		double	dLeftGirderToGirder	= pGir->m_pLeft->GetDisGirderToGirder(pBxPreMatch);
		double	dCurGirderToGirder	= pGir->GetDisGirderToGirder(pBx);

		dWidthSlab = dLeftGirderToGirder/2 + dCurGirderToGirder/2;
	}

	return	dWidthSlab;
}


// pBx : �ܸ� MAT�� ���� ���κ��� ù��° ���κ�BX
// return : pBx�� ������ ���°� ���� ������ �����(ELE_MOMENT3)�� ���� ū ���κ�
CPlateBasicIndex* CFemModelingCalc::GetBxCrossBeamMaxMomentByMatType(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp  *pDB	 = m_pDataManage->GetBridge();
	CPlateCrossBeam  *pCCur  = NULL;
	CPlateBasicIndex *pBxCur = NULL;
	CFEMManage       *pFemData = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CFemStressCalc   StressCalc(m_pDataManage, m_pCalcStd);
	
	CPlateBasicIndex *pBxReturn = NULL;

	double dMoment    = 0;	// ���κ������� ���� ū ���Ʈ�� ���밪
	double dMaxMoment = 0;	// �ִ밪

	long nElm	   = (long)GetElemByCrossBx(pBx);
	long nElmCur   = 0;
	
	for(long nG = 0; nG < pDB->GetGirdersu()-1; nG++)
	{
		long nCrossSu = pDB->GetGirder(nG)->GetQtyCrossBeamVBracing();
		for(long nCross = 0; nCross < nCrossSu; nCross++)
		{
			pBxCur = pDB->GetGirder(nG)->GetBxByCrossNo(nCross);
			if(pBxCur->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			{
				pCCur     = pBxCur->GetCrossBeam();
				nElmCur   = (long)GetElemByCrossBx(pBxCur);

				if(pFemData->GetElement((unsigned short)nElm)->m_szFrameSectionName == pFemData->GetElement((unsigned short)nElmCur)->m_szFrameSectionName)
				{
					dMoment = fabs(StressCalc.GetOneEleMaxForce(ELE_MOMENT3, LC_SUM_MAX, nElm));	
					if(dMaxMoment < dMoment)
					{
						dMaxMoment = dMoment;
						pBxReturn  = pBxCur;
					}
				}
			}
		}
	}

	return pBxReturn;
}

CPlateBasicIndex* CFemModelingCalc::GetBxCrossBeamMaxMoment(BOOL bJijum)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CPlateCrossBeam		*pC			= NULL;
	CBracing			*pBracing	= NULL;
	CPlateBasicIndex	*pBxReturn	= NULL;
	CPlateBasicIndex	*pBxCur		= NULL;

	CFemStressCalc   StressCalc(m_pDataManage, m_pCalcStd);

	double dMaxMoment = 0;	// �ִ밪
	long   nElmCur    = 0;
	double dMoment    = 0;

	for(long nG = 0; nG < pDB->GetGirdersu()-1; nG++)
	{
		long nCrossSu = pDB->GetGirder(nG)->GetQtyCrossBeamVBracing();
		for(long nCross = 0; nCross < nCrossSu; nCross++)
		{
			pBxCur = pDB->GetGirder(nG)->GetBxByCrossNo(nCross);
			if(pBxCur->GetSection()->IsenDetType(SECDET_CROSSBEAM))
			{
				pC = pBxCur->GetCrossBeam();
				if(pBxCur->IsJijum() == bJijum)
				{
					nElmCur   = (long)GetElemByCrossBx(pC->GetBx());
					dMoment = fabs(StressCalc.GetOneEleMaxForce(ELE_MOMENT3, LC_SUM_MAX, nElmCur));	
					if(dMaxMoment < dMoment)
					{
						dMaxMoment = dMoment;
						pBxReturn  = pC->GetBx();
					}
				}
			}
			else if(pBxCur->GetSection()->IsenDetType(SECDET_VBRACING))
			{
				pBracing = pBxCur->GetBracing();
				if(pBxCur->IsJijum() == bJijum)
				{
					nElmCur   = (long)GetElemByCrossBx(pBracing->GetBx());
					dMoment = fabs(StressCalc.GetOneEleMaxForce(ELE_MOMENT3, LC_SUM_MAX, nElmCur));	
					if(dMaxMoment < dMoment)
					{
						dMaxMoment = dMoment;
						pBxReturn  = pBracing->GetBx();
					}
				}
			}
		}
	}

	return pBxReturn;
}

// pBx : �ܸ� MAT�� ���� �����극�̽� �� ù��° �����극�̽� BX
// return : pBx�� ������ ���°� ���� �����극�̽� �� �����(ELE_MOMENT3)�� ���� ū �����극�̽�
CPlateBasicIndex* CFemModelingCalc::GetBxVBracingMaxMomentByMatType(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp  *pDB	 = m_pDataManage->GetBridge();
	CVBracing		 *pVrCur = NULL;
	CPlateBasicIndex *pBxCur = NULL;
	CFEMManage       *pFemData = m_pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);

	CFemStressCalc   StressCalc(m_pDataManage, m_pCalcStd);
	
	CPlateBasicIndex *pBxReturn = NULL;

	double dMoment    = 0;	// �����극�̽� ������ ���� ū ���Ʈ�� ���밪
	double dMaxMoment = 0;	// �ִ밪

	long nElm	   = (long)GetElemByCrossBx(pBx);
	long nElmCur   = 0;
	
	for(long nG = 0; nG < pDB->GetGirdersu()-1; nG++)
	{
		long nCrossSu = pDB->GetGirder(nG)->GetQtyCrossBeamVBracing();
		for(long nCross = 0; nCross < nCrossSu; nCross++)
		{
			pBxCur = pDB->GetGirder(nG)->GetBxByCrossNo(nCross);
			if(pBxCur->GetSection()->IsenDetType(SECDET_VBRACING))
			{
				pVrCur    = pBxCur->GetSection()->GetVBracing();
				nElmCur   = (long)GetElemByCrossBx(pBxCur);

				if(pFemData->GetElement((unsigned short)nElm)->m_szFrameSectionName == pFemData->GetElement((unsigned short)nElmCur)->m_szFrameSectionName)
				{
					dMoment = fabs(StressCalc.GetOneEleMaxForce(ELE_MOMENT3, LC_SUM_MAX, nElm));	
					if(dMaxMoment <= dMoment)
					{
						dMaxMoment = dMoment;
						pBxReturn  = pBxCur;
					}
				}
			}
		}
	}

	return pBxReturn;
}

double CFemModelingCalc::GetMaxSlabWidth(long nG, double dSta)
{
	CPlateBridgeApp		*pDBApp = m_pDataManage->GetBridge();	
	CPlateGirderApp		*pGir		= pDBApp->GetGirder(nG);
	CPlateBasicIndex	*pBx		= pGir->GetBxByStation(dSta);
	long	nGsu = pDBApp->GetGirdersu();
	double	dSttTerm	= 0;
	double	dEndTerm	= 0;
	double	dB1			= 0;
	double	dB2			= 0;
	double	dUp_f_C		= pGir->GetThickJewon(G_F_U,pBx);
	double	dSlabWidth	= 0;

	if(nGsu==1)
	{
		dSttTerm	= pDBApp->m_dWidthSlabLeft;
		dEndTerm	= pDBApp->m_dWidthSlabRight;
		dSlabWidth	= dSttTerm + dUp_f_C + dEndTerm;
	}
	else if(nG==0)
	{
		dSttTerm	= pDBApp->m_dWidthSlabLeft;
		dB2			= pGir->GetDisGirderToGirder(pBx)/ 2.0;
		dSlabWidth	= dSttTerm + dUp_f_C + dB2;
	}

	else if(nG==nGsu-1)
	{
		dEndTerm	= pDBApp->m_dWidthSlabRight;
		dB2			= pGir->GetDisGirderToGirder(pBx)/ 2.0;
		dSlabWidth	= dB2 + dUp_f_C + dEndTerm;
	}
	else
	{
		CPlateGirderApp		*pGirFr = pDBApp->GetGirder(nG-1);
		CPlateBasicIndex	*pBxFr	= pGirFr->GetBxByStation(dSta);

		dB1 = Round(pGirFr->GetDisGirderToGirder(pBxFr) / 2.0,0);
		dB2 = Round(pGir->GetDisGirderToGirder(pBx)  / 2.0, 0);

		dSlabWidth = Round(dB1 + dUp_f_C + dB2,0);
	}

	return	dSlabWidth;
}

double CFemModelingCalc::GetSectionCoeffAs(long nAnalysisType, CPlateBasicIndex *pBx, BOOL bNext)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

	double	dValue	= 0;
	if(nAnalysisType==BEFORE_COMPOSITE_ANALYSIS)	
	{
//Issue Number: 25496
//		if(pBx && pBx->GetGirder()->IsShiftSection(pBx, bNext))	// TU���ӱ� & �����Ʈ��
//			dValue = m_MapSecCoe["Avd"];
//		else
			dValue = m_MapSecCoe["As_SUM"];
	}
	else if(nAnalysisType==BEFORE_TURN_COMPOSITE_ANALYSIS)	dValue = m_MapSecCoe["Avc"];
	else
	{
		if(pDB->IsTUGir())	dValue = m_MapSecCoe["Avd"];
		else				dValue = m_MapSecCoe["Av_SUM"];
	}

	return dValue;
}


double CFemModelingCalc::GetSectionCoeffI33(long nAnalysisType, CPlateBasicIndex *pBx, BOOL bNext)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

	double	dValue	= 0;
	if(nAnalysisType==BEFORE_COMPOSITE_ANALYSIS)
	{
//Issue Number: 25496
//		if(pBx && pBx->GetGirder()->IsShiftSection(pBx, bNext))	// TU���ӱ� & �����Ʈ��
//			dValue = m_MapSecCoe["Ivd33"];
//		else
			dValue = m_MapSecCoe["I33"];
	}
	else if(nAnalysisType==BEFORE_TURN_COMPOSITE_ANALYSIS)
	{
		dValue = m_MapSecCoe["Ivc33"];
	}
	else
	{
		if(pDB->IsTUGir())	dValue = m_MapSecCoe["Ivd33"];
		else				dValue = m_MapSecCoe["Iv33"];
	}

	return dValue;
}

double CFemModelingCalc::GetSectionCoeffI22(long nAnalysisType, CPlateBasicIndex *pBx, BOOL bNext)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

	double	dValue	= 0;
	if(nAnalysisType==BEFORE_COMPOSITE_ANALYSIS)
	{
//Issue Number: 25496
//		if(pBx && pBx->GetGirder()->IsShiftSection(pBx, bNext))	// TU���ӱ� & �����Ʈ��
//			dValue = m_MapSecCoe["Ivd22"];
//		else
			dValue = m_MapSecCoe["I22"];
	}
	else if(nAnalysisType==BEFORE_TURN_COMPOSITE_ANALYSIS)
		dValue = m_MapSecCoe["Ivc22"];
	else
	{
		if(pDB->IsTUGir())	dValue = m_MapSecCoe["Ivd22"];
		else				dValue = m_MapSecCoe["Iv22"];
	}
	return dValue;
}

double CFemModelingCalc::GetSectionCoeffK(long nAnalysisType, CPlateBasicIndex *pBx, BOOL bNext)
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();

	double	dValue	= 0;
	if(nAnalysisType==BEFORE_COMPOSITE_ANALYSIS)
	{
//Issue Number: 25496
//		if(pBx && pBx->GetGirder()->IsShiftSection(pBx, bNext))	// TU���ӱ� & �����Ʈ��
//			dValue = m_MapSecCoe["Kvd"];
//		else
			dValue = m_MapSecCoe["K"];
	}
	else if(nAnalysisType==BEFORE_TURN_COMPOSITE_ANALYSIS)	dValue = m_MapSecCoe["Kvc"];
	else
	{
		if(pDB->IsTUGir())	dValue = m_MapSecCoe["Kvd"];
		else				dValue = m_MapSecCoe["Kv"];
	}
	return dValue;
}

long CFemModelingCalc::GetMatNo(long nG, double dSta)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CCalcGeneral	CalcGen(m_pDataManage);

	long	nSizeSec		= m_pCalcStd->GetSizeByundanInfo(nG);

	for(long nSec=0; nSec<nSizeSec-1; nSec++)
	{
		CDanMyunProp	*pSectI = &m_pCalcStd->m_vByunDanInfo[nG][nSec];
		CDanMyunProp	*pSectJ = &m_pCalcStd->m_vByunDanInfo[nG][nSec+1];

		if(!pSectI->m_MatRight.m_bUsedCalc && pSectI->m_MatLeft.m_bUsedCalc && nSec>0)
		{
			pSectI = &m_pCalcStd->m_vByunDanInfo[nG][nSec-1];	// ������ ���۴ܸ�
			pSectJ = &m_pCalcStd->m_vByunDanInfo[nG][nSec];		// ������ ���ܸ�
		}

		double	dStaStt	= pSectI->m_pBx->GetStation();
		double	dStaEnd	= pSectJ->m_pBx->GetStation();

		if(dStaStt <= dSta && dSta < dStaEnd)
		{
			long nCurIdx = -1;
			if(pSectI->m_MatRight.m_bUsedCalc)
				nCurIdx = pSectI->m_MatRight.m_nIdx;
			else if(pSectJ->m_MatLeft.m_bUsedCalc)
				nCurIdx = pSectJ->m_MatLeft.m_nIdx;
			else
				nCurIdx = m_pCalcStd->GetIndexOnByundanInfo(nG,nSec);
			
			return nCurIdx+1;
		}
		else if(nSec==0 && dSta < dStaStt)
		{
			return -1;
		}
	}

	return m_pCalcStd->GetIndexOnByundanInfo(nG,nSizeSec-1);
}

double CFemModelingCalc::GetStationSecJewon(long nMatNo, long nG, double dSta, long &nPosSec)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx		= pGir->GetBxByStation(dSta);
	CPlateBasicIndex	*pBxSec		= pBx;
	CCalcGeneral	CalcGen(m_pDataManage);

	long	nSizeSec		= m_pCalcStd->GetSizeByundanInfo(nG);
	double	dStaSec			= dSta;

	for(long nSec=0; nSec<nSizeSec-1; nSec++)
	{
		CDanMyunProp	*pSectI = &m_pCalcStd->m_vByunDanInfo[nG][nSec];
		CDanMyunProp	*pSectJ = &m_pCalcStd->m_vByunDanInfo[nG][nSec+1];

		long	nCurIdx	= m_pCalcStd->GetIndexOnByundanInfo(nG,nSec);
		if(!pSectI->m_MatRight.m_bUsedCalc && pSectI->m_MatLeft.m_bUsedCalc && nSec>0)
		{
			pSectI = &m_pCalcStd->m_vByunDanInfo[nG][nSec-1];		// ������ ���۴ܸ�
			pSectJ = &m_pCalcStd->m_vByunDanInfo[nG][nSec];	// ������ ���ܸ�
		}

		double	dStaStt	= pSectI->m_pBx->GetStation();
		double	dStaEnd	= pSectJ->m_pBx->GetStation();

		if(nCurIdx==nMatNo-1)
		{
			GetSectionCoefficient(nG, dStaStt);
			double	dI33Stt	= GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pSectI->m_pBx);
			GetSectionCoefficient(nG, dStaEnd);
			double	dI33End	= GetSectionCoeffI33(BEFORE_COMPOSITE_ANALYSIS, pSectJ->m_pBx);

			if(pSectI->IsByunDanMyun() || pSectJ->IsByunDanMyun())
			{
				pBxSec	= pBx;
				dStaSec = pBx->GetStation();

				if(pBxSec==pSectI->m_pBx)		nPosSec = 1;
				else if(pBxSec==pSectJ->m_pBx)	nPosSec = -1;
			}
			else
			{
				if(dI33Stt<=dI33End)
				{
					pBxSec	= pSectI->m_pBx;
					dStaSec = dStaStt;
					nPosSec	= 1;
				}
				else
				{
					pBxSec	= pSectJ->m_pBx;
					dStaSec = dStaEnd;
					nPosSec	= -1;
				}
			}
			break;
		}
	}

	return dStaSec;
}
