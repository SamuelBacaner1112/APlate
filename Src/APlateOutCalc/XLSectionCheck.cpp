// XLSectionCheck.cpp: implementation of the CXLSectionCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateOutCalc.h"
#include "APlateOutCalcStd.h"
#include "XLSectionCheck.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define  maxabs(x,y)  (fabs(x) > fabs(y) ? x : y)

CXLSectionCheck::CXLSectionCheck(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL)
{
	m_nSttRow			= 1;
	m_nPosType			= 0;
	m_pAPlateOutCalcStd	= pStd;
	m_pAPlateOutCalcXL	= pAPlateOutCalcXL;
}

CXLSectionCheck::~CXLSectionCheck()
{

}

long CXLSectionCheck::SectionUserCheck(long nG, double dSta)
{
	CPlateBridgeApp	*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage		*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd	*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcData		*pData			= pDataManage->GetCalcData();
	CFEMManage		*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);

	BOOL	bWind		= pData->m_bWind;
	BOOL	bTUGir		= pDB->IsTUGir();
	CString	szSheet		= _T("");
	BOOL	bPositive	= StressCalc.IsPositive(ELE_MOMENT3, nG, dSta);
	CString	szText			= _T("");
	double	dRatio			= 0;
	long	nElem			= ModelCalc.GetElemNoAndRatio(nG, dSta, dRatio);
	double	dLengthElm		= ModelCalc.GetElemLength(nElem, FALSE);
	double	dLengthFromStt	= dRatio*dLengthElm;
	CElement *pElm	= pFEMManage->GetElement((unsigned short)nElem);

	m_nSttRow	= 2;
	if(bPositive)
	{
		szSheet.Format(_T("�ܸ����(�����Ʈ)"));
		if(bTUGir)	szSheet += _T("_TU");
		if(bWind)	szSheet += _T("_ǳ����");
		else		
		m_pXL->SetActiveSheet(szSheet);
		pOut->SetXL(pXL, "��������",	m_nSttRow, "B", "E", 0);
		szText.Format("���� %d ���� %.3f m", pElm->m_nJStt+1, dLengthFromStt);
		pOut->SetXL(pXL, szText, m_nSttRow, "H", "H", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);
		if(pDB->IsTUGir() && !pDB->IsTUContinuousSpan())
			pXL->DeleteColSell(1, 29, 7, 37);
		PositiveSection_TUGirder(-1, nG, dSta, CXLSectionCheck::POSITIVE_SEC);
	}
	else
	{
		szSheet.Format(_T("�ܸ����(�θ��Ʈ)"));
		if(bTUGir)	szSheet += _T("_TU");
		if(bWind)	szSheet += _T("_ǳ����");
		m_pXL->SetActiveSheet(szSheet);
		pOut->SetXL(pXL, "��������",	m_nSttRow, "B", "E", 0);
		szText.Format("���� %d ���� %.3f m", pElm->m_nJStt+1, dLengthFromStt);
		pOut->SetXL(pXL, szText, m_nSttRow, "H", "H", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);
		if(pDB->IsTUContinuousSpan())
			SetTUContinousNegativeDLL();
		NegativeSection_TUGirder(-1, nG, dSta, CXLSectionCheck::NEGATIVE_SEC);
	}

	return m_nSttRow;
}

void CXLSectionCheck::GetSectionCheckData(CDWordArray &dArrMat, CDWordArray &dArrNG, CDDWordArray &dArrSta, long nPosType, BOOL bPositive)
{
	CGlobarOption	*pOpt	= m_pAPlateOutCalcStd->GetDataManage()->GetGlobalOption();

	if(pOpt->m_nCalcDocSub[3]==0)	GetSectionGirderNotClassify(dArrMat, dArrNG, dArrSta, nPosType, bPositive);
	else							GetSectionGirderClassify(dArrMat, dArrNG, dArrSta, nPosType, bPositive);
}

void CXLSectionCheck::GetSectionGirderClassify(CDWordArray &dArrMat, CDWordArray &dArrNG, CDDWordArray &dArrSta, long nPosType, BOOL bPositive)
{
	CPlateBridgeApp		*pDB            = m_pAPlateOutCalcStd->GetDataManage()->GetBridge();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CFEMManage			*pFEMManage		= m_pAPlateOutCalcStd->GetAPlateCalcStd()->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CCalcData           *pCalcData      = m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();

	BOOL	bHapSung	= pDB->IsHapsung();
	long	nG			= 0;
	long	nNodeIdx	= 0;
	long	nMatSize	= 0;
	double	dSta		= 0;
	BOOL    bPositiveM  = FALSE;

	POSITION pos = pFEMManage->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrm = (CFrameSection *) pFEMManage->m_FrameSectionList.GetNext(pos);
		if(pFrm->m_nFlag==POS_GIRDER)
			nMatSize++;
	}

	//////////////////////////////////////////////////////////////////////
	// ��� �Ŵ����� ã��(�ܸ����)=>�ռ��� s2k�ܸ�
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_SEC || nPosType==NEGATIVE_SEC)
	{
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			for(long nMat=0; nMat<nMatSize; nMat++)
			{
				long nTemp = nG;
				nNodeIdx = pModelCalc->GetMaxForcePositionOfSection(nMat+1, nTemp, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
				if(nNodeIdx < 0)	continue;
				if(nTemp < 0)		continue;

				bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nTemp, dSta);

				if(bHapSung && bPositiveM == bPositive)	
				{
					dArrMat.Add(nMat+1);
					dArrNG.Add(nTemp);
					dArrSta.Add(dSta);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	// �߰��ܸ����
	// �������̴ܸ�
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_SECPLUS || nPosType==NEGATIVE_SECPLUS)
	{
		for(long nG=0; nG<pDB->GetGirdersu(); nG++)
		{
			long nExtSize = pCalcStd->GetSizeByundanInfo(nG);

			for(long nMat = nMatSize; nMat < nExtSize; nMat++)
			{
				long nTemp = nG;
				nNodeIdx = pModelCalc->GetMaxForcePositionOfSection(nMat+1, nTemp, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
				if(nNodeIdx < 0)	continue;
				if(nTemp < 0)		continue;
				bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nTemp, dSta);

				if(bHapSung && bPositiveM == bPositive && nNodeIdx>0)		// �����Ʈ
				{
					dArrMat.Add(nMat+1);
					dArrNG.Add(nTemp);
					dArrSta.Add(dSta);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	// ���ܸ��߰��ܸ����
	// 1. ���ܸ�� ����� �ִ��� ��ġ
	// 2. ���ܸ�� �ּҴܸ� ��ġ
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_BUYNPLUS || nPosType==NEGATIVE_BUYNPLUS)
	{
		CDWordArray  dArrNGExt, dArrNGExt2;
		CDDWordArray dArrStaExt, dArrStaExt2;
		CDWordArray  dArrTypeExt;
		if(pDB->IsByunDanInBridge() && bHapSung)
		{
			// ���ܸ�� ����� �ִ��� ��ġ(�����Ʈ ������ ù�� �Ŵ��� ������ ������� ���� ū ��ġ�� �߰���)
			pStressCalc->GetAddMaxForceSect(dArrNGExt, dArrStaExt);
			pStressCalc->CompareMaterialRemoveDup(dArrNGExt, dArrStaExt, dArrMat);	// ���� Materials �ִ��� ��
			// ���ܸ�� �ּҴܸ� ��ġ
			pStressCalc->GetAddMinSect(dArrNGExt2, dArrStaExt2);
			pStressCalc->CompareMaterialRemoveDup(dArrNGExt2, dArrStaExt2, dArrMat);	// ���� Materials �ִ��� ��
			// �ּҴܸ� �ߺ� �˻�� �߰�
			pStressCalc->CompareHeightAddSect(dArrNGExt, dArrStaExt, dArrNGExt2, dArrStaExt2, dArrTypeExt);
		}
		dArrMat.RemoveAll();
		dArrNG.RemoveAll();
		dArrSta.RemoveAll();

		for(long n=0; n<dArrNGExt.GetSize(); n++)
		{
			bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, dArrNGExt.GetAt(n), dArrStaExt.GetAt(n));

			if(bPositiveM == bPositive)		// �����Ʈ
			{
				dArrMat.Add(n+1);
				dArrNG.Add(dArrNGExt.GetAt(n));
				dArrSta.Add(dArrStaExt.GetAt(n));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	// ����� �߰��ܸ���� ��������
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_USERPLUS || nPosType==NEGATIVE_USERPLUS)
	{
		CDWordArray  dArrMatUser, dArrNGUser;
		CDDWordArray dArrStaUser;
		int nUserSectionSize = pCalcData->m_ArrExtraSecTitle.GetSize();

		for(long i = 0; i < nUserSectionSize; i++)
		{
			int	   nG   = pCalcData->m_ArrExtraSecGir[i];
			double dSta = pCalcData->m_ArrExtraSecSta[i];

			bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);
			if(bPositiveM != bPositive) // ���� ��/�� ���Ʈ ������ �ٸ��ٸ� �׳� �Ѿ��.
				continue;

			// 2015. 05. 22, #APLATE-670, ���ܸ� ������ �̿��� ���� �Ŵ�, Station�� ���� �ܸ� ��ȣ�� �˾Ƴ���.
			long nTermsu = pCalcStd->GetSizeByundanInfo(nG);
			for (long nTerm = 0; nTerm < nTermsu - 1; nTerm++)
			{
				CDanMyunProp *pSectI = &pCalcStd->m_vByunDanInfo[nG][nTerm];
				CDanMyunProp *pSectJ = &pCalcStd->m_vByunDanInfo[nG][nTerm+1];

				if(!pSectI->m_MatRight.m_bUsedCalc && pSectI->m_MatLeft.m_bUsedCalc && nTerm > 0)
				{
					pSectI = &pCalcStd->m_vByunDanInfo[nG][nTerm-1];
					pSectJ = &pCalcStd->m_vByunDanInfo[nG][nTerm];
				}

				// ���� �ܸ鿡 ���� ���� Station�� �� Station�� �˾Ƴ���.
				double dStaStt = pSectI->m_pBx->GetStation();
				double dStaEnd = pSectJ->m_pBx->GetStation();

				// ���� ���� Station�� ���� ���� �ִ� ���
				if(dStaStt <= dSta && dSta <= dStaEnd)
				{
					dArrMatUser.Add(pSectI->m_MatRight.m_nIdx + 1);
					dArrNGUser.Add(nG);
					dArrStaUser.Add(dSta);
					break;
				}
			}
		}

		dArrMat.RemoveAll();
		dArrNG.RemoveAll();
		dArrSta.RemoveAll();

		for(long n = 0; n < dArrMatUser.GetSize(); n++)
		{
			dArrMat.Add(dArrMatUser.GetAt(n));
			dArrNG.Add(dArrNGUser.GetAt(n));
			dArrSta.Add(dArrStaUser.GetAt(n));
		}
	}

	//////////////////////////////////////////////////////////////////////
	// ����������ġ ��������
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_SP || nPosType==NEGATIVE_SP)
	{
		CDWordArray GirderNO, SpliceNO;
		CDDWordArray dArrStaSP;
		double MomentSP, dStaSP, dStaSP2, TempM_SP;

		// �Ŵ��� �������� ������ ���ؼ�, ���� ���� ���������� ���� �Ŵ��� ���Ѵ�.
		long nSpliceCount = 0, nGMaxSp = 0;
		for (nG = 0; nG < pDB->GetGirdersu(); nG++)
		{
			if(pDB->GetGirder(nG)->GetQtySplice() > nSpliceCount)
			{
				nSpliceCount = pDB->GetGirder(nG)->GetQtySplice();
				nGMaxSp = nG;
			}
		}

		// �ش� �Ŵ��� ���ؼ� ������ splice�� Ȯ���� ��/�� ���Ʈ�� Ȯ���Ѵ�.
		for(long i = 0; i < nSpliceCount; i++)
		{
			CPlateSplice *pSpliceMaxGir = pDB->GetGirder(nGMaxSp)->GetSpliceInTotal(i);
			dStaSP   = pSpliceMaxGir->GetBx()->GetStation();
			MomentSP = pStressCalc->GetElementForce(ELE_MOMENT3, 0, nG, dStaSP, 4);

			// ���� �������� ��ȣ(i)�� ���ؼ� ���� ���� moment�� ���� �Ŵ��� ���Ѵ�.
			for(long g = 0; g < pDB->GetGirdersu(); g++)
			{
				CPlateGirderApp *pCurrentGir = pDB->GetGirder(g);
				long nSpliceQty = pCurrentGir->GetQtySplice();
				
				if(i >= nSpliceQty) continue;

				CPlateSplice *pSpliceGir = pCurrentGir->GetSpliceInTotal(i);
				dStaSP2 = pSpliceGir->GetBx()->GetStation();
				TempM_SP = pStressCalc->GetElementForce(ELE_MOMENT3, 0, g, dStaSP2, 4);

				// ���� ���� ���Ʈ�� ��� �ش� �Ŵ��� ��ȣ�� �����Ѵ�.
				if(fabs(TempM_SP) > fabs(MomentSP))
				{
					nG = g;
					MomentSP = TempM_SP;
				}
			}

			double dStaSplice = pDB->GetGirder(nG)->GetBxBySpliceNo(i)->GetStation();
			bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nG, dStaSplice);
			
			if(bPositiveM == bPositive)		// �����Ʈ
			{
				SpliceNO.Add(i+1);
				GirderNO.Add(nG);
				dArrStaSP.Add(dStaSplice);
			}
		}
		dArrMat.RemoveAll();
		dArrNG.RemoveAll();
		dArrSta.RemoveAll();

		for(long n=0; n<SpliceNO.GetSize(); n++)
		{
			dArrMat.Add(SpliceNO.GetAt(n));
			dArrNG.Add(GirderNO.GetAt(n));
			dArrSta.Add(dArrStaSP.GetAt(n));
		}
	}
}

void CXLSectionCheck::GetSectionGirderNotClassify(CDWordArray &dArrMat, CDWordArray &dArrNG, CDDWordArray &dArrSta, long nPosType, BOOL bPositive)
{
	CPlateBridgeApp		*pDB            = m_pAPlateOutCalcStd->GetDataManage()->GetBridge();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CCalcData           *pCalcData      = m_pAPlateOutCalcStd->GetDataManage()->GetCalcData();
	
	BOOL	bHapSung	= pDB->IsHapsung();
	long	nG			= 0;
	long	nNodeIdx	= 0;
	double	dSta		= 0;
	BOOL    bPositiveM  = FALSE;

	//////////////////////////////////////////////////////////////////////
	// ��� �Ŵ����� ã��(�ܸ����)=>�ռ��� s2k�ܸ�
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_SEC || nPosType==NEGATIVE_SEC)
	{
		POSITION pos = pFEMManage->m_FrameSectionList.GetHeadPosition();
		while(pos)
		{
			CFrameSection *pFrame = (CFrameSection*)pFEMManage->m_FrameSectionList.GetNext(pos);
			nG = -1;  

			if(pFrame->m_nFlag == POS_GIRDER)
			{	// ���밪�� �ִ��� ��ġ
				nNodeIdx = pModelCalc->GetMaxForcePositionOfSection(atol(pFrame->m_szName), nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
				if(nNodeIdx < 0)	continue;
				if(nG < 0)			continue;

				bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);
				bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);

				if(bHapSung && bPositiveM == bPositive)	
				{
					dArrMat.Add(atol(pFrame->m_szName));
					dArrNG.Add(nG);
					dArrSta.Add(dSta);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	// �߰��ܸ����
	// �������̴ܸ�
	//////////////////////////////////////////////////////////////////////
	long	nMatSize	= 0;
	POSITION pos = pFEMManage->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		CFrameSection *pFrm = (CFrameSection *) pFEMManage->m_FrameSectionList.GetNext(pos);
		if(pFrm->m_nFlag==POS_GIRDER)
			nMatSize++;
	}

	if(nPosType==POSITIVE_SECPLUS || nPosType==NEGATIVE_SECPLUS)
	{
		for(long n=0; n<pDB->GetGirdersu(); n++)
		{
			for(long nMat=0; nMat<pCalcStd->GetSizeByundanInfo(nG); nMat++)
			{
				long	nMatID	= pCalcStd->GetIndexOnByundanInfo(nG,nMat)+1;
				if(nMatID<nMatSize) continue;

				nNodeIdx = pModelCalc->GetMaxForcePositionOfSection(nMatID, nG, dSta, ELE_MOMENT3, CFemModelingCalc::MAX_ABS, FALSE);
				bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);

				if(bHapSung && bPositiveM == bPositive && nNodeIdx>0)		// �����Ʈ
				{
					BOOL bExist = FALSE;
					for(long k=0; k<dArrMat.GetSize(); k++)
					{
						if(dArrMat.GetAt(k)==nMatID)
							bExist = TRUE;
					}
					if(!bExist)
					{
						dArrMat.Add(nMatID);
						dArrNG.Add(nG);
						dArrSta.Add(dSta);
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	// ���ܸ��߰��ܸ����
	// 1. ���ܸ�� ����� �ִ��� ��ġ
	// 2. ���ܸ�� �ּҴܸ� ��ġ
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_BUYNPLUS || nPosType==NEGATIVE_BUYNPLUS)
	{
		CDWordArray  dArrNGExt, dArrNGExt2;
		CDDWordArray dArrStaExt, dArrStaExt2;
		CDWordArray  dArrTypeExt;
		long nMatNo = 0;
		if(pDB->IsByunDanInBridge() && bHapSung)
		{
			// ���ܸ�� ����� �ִ��� ��ġ(�����Ʈ ������ ù�� �Ŵ��� ������ ������� ���� ū ��ġ�� �߰���)
			pStressCalc->GetAddMaxForceSect(dArrNGExt, dArrStaExt);
			pStressCalc->CompareMaterialRemoveDup(dArrNGExt, dArrStaExt, dArrMat);	// ���� Materials �ִ��� ��
			// ���ܸ�� �ּҴܸ� ��ġ
			pStressCalc->GetAddMinSect(dArrNGExt2, dArrStaExt2);
			pStressCalc->CompareMaterialRemoveDup(dArrNGExt2, dArrStaExt2, dArrMat);	// ���� Materials �ִ��� ��
			// �ּҴܸ� �ߺ� �˻�� �߰�
			pStressCalc->CompareHeightAddSect(dArrNGExt, dArrStaExt, dArrNGExt2, dArrStaExt2, dArrTypeExt);
		}
		dArrMat.RemoveAll();
		dArrNG.RemoveAll();
		dArrSta.RemoveAll();

		for(long n=0; n<dArrNGExt.GetSize(); n++)
		{
			bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, dArrNGExt.GetAt(n), dArrStaExt.GetAt(n));

			if(bPositiveM == bPositive)		// �����Ʈ
			{
				nMatNo = pModelCalc->GetMatNo(dArrNGExt.GetAt(n), dArrStaExt.GetAt(n));
				dArrMat.Add(nMatNo);
				dArrNG.Add(dArrNGExt.GetAt(n));
				dArrSta.Add(dArrStaExt.GetAt(n));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////
	// ����� �߰��ܸ���� ��������
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_USERPLUS || nPosType==NEGATIVE_USERPLUS)
	{
		CDWordArray  dArrMatUser, dArrNGUser;
		CDDWordArray dArrStaUser;
		int nUserSectionSize = pCalcData->m_ArrExtraSecTitle.GetSize();	
		long nMatNo = 0;
		for(long i = 0; i < nUserSectionSize; i++)
		{
			int	   nG   = pCalcData->m_ArrExtraSecGir[i];
			double dSta = pCalcData->m_ArrExtraSecSta[i];

			bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);
			if(bPositiveM == bPositive)		// �����Ʈ
			{
				nMatNo = pModelCalc->GetMatNo(nG, dSta);
				dArrMatUser.Add(nMatNo);
				dArrNGUser.Add(nG);
				dArrStaUser.Add(dSta);
			}
		}
		dArrMat.RemoveAll();
		dArrNG.RemoveAll();
		dArrSta.RemoveAll();

		for(long n=0; n<dArrMatUser.GetSize(); n++)
		{
			dArrMat.Add(dArrMatUser.GetAt(n));
			dArrNG.Add(dArrNGUser.GetAt(n));
			dArrSta.Add(dArrStaUser.GetAt(n));
		}
	}

	//////////////////////////////////////////////////////////////////////
	// ����������ġ ��������
	//////////////////////////////////////////////////////////////////////
	if(nPosType==POSITIVE_SP || nPosType==NEGATIVE_SP)
	{
		CDWordArray GirderNO, SpliceNO;
		CDDWordArray dArrStaSP;
		double MomentSP, dStaSP, dStaSP2, TempM_SP;

		// �Ŵ��� �������� ������ ���ؼ�, ���� ���� ���������� ���� �Ŵ��� ���Ѵ�.
		long nSpliceCount = 0, nGMaxSp = 0;
		for (nG = 0; nG < pDB->GetGirdersu(); nG++)
		{
			if(pDB->GetGirder(nG)->GetQtySplice() > nSpliceCount)
			{
				nSpliceCount = pDB->GetGirder(nG)->GetQtySplice();
				nGMaxSp = nG;
			}
		}

		// �ش� �Ŵ��� ���ؼ� ������ splice�� Ȯ���� ��/�� ���Ʈ�� Ȯ���Ѵ�.
		for(long i = 0; i < nSpliceCount; i++)
		{
			CPlateSplice *pSpliceMaxGir = pDB->GetGirder(nGMaxSp)->GetSpliceInTotal(i);
			dStaSP   = pSpliceMaxGir->GetBx()->GetStation();
			MomentSP = pStressCalc->GetElementForce(ELE_MOMENT3, 0, nG, dStaSP, 4);

			// ���� �������� ��ȣ(i)�� ���ؼ� ���� ���� moment�� ���� �Ŵ��� ���Ѵ�.
			for(long g = 0; g < pDB->GetGirdersu(); g++)
			{
				CPlateGirderApp *pCurrentGir = pDB->GetGirder(g);
				long nSpliceQty = pCurrentGir->GetQtySplice();
				
				if(i >= nSpliceQty) continue;

				CPlateSplice *pSpliceGir = pCurrentGir->GetSpliceInTotal(i);
				dStaSP2 = pSpliceGir->GetBx()->GetStation();
				TempM_SP = pStressCalc->GetElementForce(ELE_MOMENT3, 0, g, dStaSP2, 4);

				// ���� ���� ���Ʈ�� ��� �ش� �Ŵ��� ��ȣ�� �����Ѵ�.
				if(fabs(TempM_SP) > fabs(MomentSP))
				{
					nG = g;
					MomentSP = TempM_SP;
				}
			}

			double dStaSplice = pDB->GetGirder(nG)->GetBxBySpliceNo(i)->GetStation();
			bPositiveM = pStressCalc->IsPositive(ELE_MOMENT3, nG, dStaSplice);

			if(bPositive == bPositiveM)		// �����Ʈ
			{
				SpliceNO.Add(i+1);
				GirderNO.Add(nG);
				dArrStaSP.Add(dStaSplice);
			}
		}
		
		dArrMat.RemoveAll();
		dArrNG.RemoveAll();
		dArrSta.RemoveAll();

		for(long n=0; n<SpliceNO.GetSize(); n++)
		{
			dArrMat.Add(SpliceNO.GetAt(n));
			dArrNG.Add(GirderNO.GetAt(n));
			dArrSta.Add(dArrStaSP.GetAt(n));
		}
	}
}

void CXLSectionCheck::SectionCheck_TUGirder(long nPosType, CStringArray &strArrUsedSheet)
{

	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutExcel		*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CCalcData           *pData          = pDataManage->GetCalcData(); 

	CDWordArray dArrMat, dArrNG;
	CDDWordArray dArrSta;
	BOOL bPositiveM = nPosType%2==0 ? TRUE : FALSE;

	GetSectionCheckData(dArrMat, dArrNG, dArrSta, nPosType, bPositiveM);
	
	if(dArrMat.GetSize()==0)	return;

	CString	szDllSheetName	= _T("");
	CString	szNewSheetName	= _T("");
	CString szText			= _T("");
	CString	szProgress		= _T("");
	BOOL	bWind			= pData->m_bWind;
	BOOL	bExist			= FALSE;
	long	nProgress		= m_pAPlateOutCalcStd->GetAPlateOutExcel()->GetProgress();
	long	nMatsu			= dArrMat.GetSize();

	if(nMatsu > 0)	bExist = TRUE;
	if(!bExist)	return;

	m_nPosType	= nPosType;
	m_nSttRow	= 1;

	switch(nPosType)
	{
	case POSITIVE_SEC:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_SEC:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�θ��Ʈ)");
			break;
		}
	case POSITIVE_SP:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�������� �����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�������� �����Ʈ)");
			break;
		}
	case NEGATIVE_SP:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�������� �θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�������� �θ��Ʈ)");
			break;
		}
	case POSITIVE_SECPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �߰��ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�߰��ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_SECPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �߰��ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�߰��ܸ����(�θ��Ʈ)");
			break;
		}
	case POSITIVE_BUYNPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ���ܸ��߰��ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("���ܸ��߰��ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_BUYNPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ���ܸ��߰��ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("���ܸ��߰��ܸ����(�θ��Ʈ)");
			break;
		}
	case POSITIVE_USERPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ������߰��ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("������߰��ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_USERPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)_TU");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ������߰��ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("������߰��ܸ����(�θ��Ʈ)");
			break;
		}
	}

	if(nPosType%2==0)	// �����Ʈ
	{
		if(pDB->IsTUGir() && !pDB->IsTUContinuousSpan())
			pXL->DeleteColSell(1, 29, 7, 37);
	}
	else
	{
		if(pDB->IsTUContinuousSpan())
			SetTUContinousNegativeDLL();
	}

	BOOL	bFirst	= TRUE;
	for(long nMat = 0; nMat < nMatsu; nMat++)
	{
		long   nMatNo = dArrMat[nMat];
		long   nG     = dArrNG[nMat];
		double dSta   = dArrSta[nMat];

		pOut->ClearCellRef();

		if(pAPlateOutXL->m_bAbort)	return;
		szText.Format("%s - %d", szProgress, nMatNo);
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(szText, nProgress+nMat*2);

		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dSta);
		
		double	dRatio			= 0;
		long	nElem			= pModelCalc->GetElemNoAndRatio(nG, dSta, dRatio);
		double	dLengthElm		= pModelCalc->GetElemLength(nElem, FALSE);
		double	dLengthFromStt	= dRatio*dLengthElm;
		CElement *pElm	= pFEMManage->GetElement((unsigned short)nElem);

		m_nSttRow++;
		if(!bFirst)
		{
			if(bPositiveM)
			{
				if(bWind)	pXL->InsertCopyRowLine(2, 496, m_nSttRow);
				else		pXL->InsertCopyRowLine(2, 419, m_nSttRow);
			}
			else
			{
				if(pDB->IsTUContinuousSpan())
				{
					if(bWind)	pXL->InsertCopyRowLine(2, 442-10, m_nSttRow);
					else		pXL->InsertCopyRowLine(2, 373-10, m_nSttRow);
				}
				else
				{
					if(bWind)	pXL->InsertCopyRowLine(2, 442, m_nSttRow);
					else		pXL->InsertCopyRowLine(2, 373, m_nSttRow);
				}
			}
		}

		// �ܸ��ȣ
		if(nPosType==POSITIVE_SP || nPosType==NEGATIVE_SP)
		{
			CString sCnt = pBx->GetSplice()->GetMarkstring();
			sCnt = sCnt.Mid(2);

			if(pBx->GetSplice()->m_bSpliceType)
				szText = "�������� - ";
			else
				szText = "������� - ";
			
			szText += sCnt;
		}
		else
			szText.Format("< �Ŵ� %d �ܸ� - %d >", nG+1, nMatNo);
		pOut->SetXL(pXL, szText, m_nSttRow, "B", "G", 0, BLACK, 9, TRUE, TA_LEFT, "", 0);

		szText.Format("���� %d ���� %.3f m", pElm->m_nJStt+1, dLengthFromStt);
		pOut->SetXL(pXL, szText, m_nSttRow, "H", "H", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);

		if(bPositiveM)	PositiveSection_TUGirder(nMatNo, nG, dSta, nPosType);
		else			NegativeSection_TUGirder(nMatNo, nG, dSta, nPosType);
		bFirst = FALSE;
	}
}

void CXLSectionCheck::PositiveSection_TUGirder(long nMatNo, long nG, double dSta, long nPosType)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CCalcData           *pData          = pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx			= pGir->GetBxByStation(dSta);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	long	nPosSec	= pBx->GetStation() > dSta ? -1 : 1;
	double	dStaSec	= pModelCalc->GetStationSecJewon(nMatNo, nG, dSta, nPosSec);
	if(nPosType==CXLSectionCheck::POSITIVE_SP||nPosType==CXLSectionCheck::NEGATIVE_SP)
		StressCalc.GetSectionStress(nG, dSta, TRUE);
	else
		StressCalc.GetSectionStress(nG, dSta, TRUE, dStaSec, nPosSec);

	CString	szText		= _T("");
	CString	szLeft		= _T("");
	CString	szRight		= _T("");
	BOOL	bWind		= pData->m_bWind;
	double	dK			= StressCalc.GetMapValueSecStress("K");		//ModelCalc.GetValueOfInfK(nG, dSta);
	double	dEs			= pData->DESIGN_MATERIAL.m_dEst;			// ���� ź�����
	double	dEc			= pData->DESIGN_MATERIAL.m_dBindEc;			// ������ũ��Ʈ�� ź�����
	double	dEd			= pData->DESIGN_MATERIAL.m_dEc;				// �ٴ��� ��ũ��Ʈ�� ź�����
	double	dNc			= (double)pData->DESIGN_MATERIAL.m_nBindNc;	// ���� ������ũ��Ʈ�� ź�������
	double	dNd			= (double)pData->DESIGN_MATERIAL.m_nN;		// ���� �ٴ��� ��ũ��Ʈ�� ź�������
	double	dEpsilon	= pData->DESIGN_MATERIAL.m_dEpsilonS;		// ����������
	double	dAlpha		= pData->DESIGN_MATERIAL.m_dAlpha;			// ����â���
	double	dCreep2		= pData->DESIGN_MATERIAL.m_dCreep2;			// ũ����2
	double	dTemp		= pData->DESIGN_MATERIAL.m_dTemparature;	// ���� ��ũ��Ʈ�� �µ��� 
	double	dL			= pDB->m_dLengthBaseCrossBeam;				// ���κ� ����
	double	dG			= pData->DESIGN_MATERIAL.m_dShearEs;		// ����ź�����
	double	dFckDeck	= pData->DESIGN_MATERIAL.m_dSigmaCK;		// �ٴ��� ��ũ��Ʈ�� �������
	double	dFckConf	= pData->DESIGN_MATERIAL.m_dBindSigmaCK;	// ������ũ��Ʈ�� �������
	double	dLenCalc	= 0;
	double	dLenPositive= pGir->GetLengthPositiveSection(dSta, dLenCalc);
	
	CPlateBasicIndex *pBxSttCr =  pGir->GetBxByStationDir(dSta, -1, BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEndCr = pGir->GetBxByStationDir(dSta, 1, BX_CROSSBEAM_VBRACING);

	if(pBxSttCr && pBxEndCr)
	{
		dL = pBxEndCr->GetStation() - pBxSttCr->GetStation();
	}
	
	if(pDB->IsTUContinuousSpan())
	{
		m_nSttRow--;
		pOut->SetXL(pXL, toM(dLenPositive),	m_nSttRow, "AI", "AK", 1, RED, 9, FALSE, TA_CENTER);
		pOut->SetXL(pXL, toM(dLenCalc),		m_nSttRow, "AI", "AK", 3, RED, 9, FALSE, TA_CENTER);
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("MphT")));
		pOut->SetXL(pXL, szText,			m_nSttRow, "AI", "AK", 1, RED, 9, FALSE, TA_CENTER);
		szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("SphT")));
		pOut->SetXL(pXL, szText,			m_nSttRow, "AI", "AK",-4, RED, 9, FALSE, TA_CENTER);
	}

	pCalcGeneral->GetMaterial(pBx, G_F_U, szLeft, szRight);
	pOut->SetXL(pXL, szLeft,	m_nSttRow, "U", "W", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, szRight,	m_nSttRow, "X", "X", 1, RED, 9, FALSE, TA_CENTER);
	pCalcGeneral->GetMaterial(pBx, G_F_L, szLeft, szRight);
	pOut->SetXL(pXL, szLeft,	m_nSttRow, "U", "W", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, szRight,	m_nSttRow, "X", "X", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Beff"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Td"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Bc"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tc"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Bu"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tu"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tw"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Hw"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Bl"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tl"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Dist"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 4, RED, 9, FALSE, TA_CENTER);

	szText.Format("%.6f", dEs);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dEc);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dEd);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dEpsilon);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 6);
	szText.Format("%.6f", dAlpha);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 6);
	szText.Format("%.6f", dCreep2);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dNc);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dNd);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dTemp);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	if(!pDB->IsTUContinuousSpan())
	{
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph1")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph2")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph3")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 0, RED, 9, FALSE, TA_CENTER, "", 3);
		szText.Format("Mph3  : �Ͽ��� ���߿� ���� ���Ʈ");
		pOut->SetXL(pXL, szText,	m_nSttRow, "B", "P");
	}
	else	m_nSttRow += 3;
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph4")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph5")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph6")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	if(bWind)
	{
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mw")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	if(!pDB->IsTUContinuousSpan())
	{
		szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph1")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
		szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph2")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
		szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph3")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 0, RED, 9, FALSE, TA_CENTER, "", 3);
		szText.Format("Sph3  : �Ͽ��� ���߿� ���� ���ܷ�");
		pOut->SetXL(pXL, szText,	m_nSttRow, "B", "P");
	}
	else
		m_nSttRow += 3;
	szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph4")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph5")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph6")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	if(bWind)
	{
		szText.Format("%.6f", StressCalc.GetMapValueSecStress("Sw"));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mst")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Msct")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mvt")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	if(bWind)
	{
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mwt")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	szText.Format("%.6f", dG);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dL);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	if(bWind)	m_nSttRow += 136;
	else		m_nSttRow += 130;
	szText.Format("%.6f", dK);
	pOut->SetXL(pXL, szText,	m_nSttRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	if(bWind)	m_nSttRow += 114;
	else		m_nSttRow += 113;
	pCalcGeneral->GetMaterial(pBx, G_W, szLeft, szRight);
	pOut->SetXL(pXL, szLeft,	m_nSttRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, szRight,	m_nSttRow, "N", "N", 4, RED, 9, FALSE, TA_CENTER);

	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft + szRight)];
	CHGAllowStress		AllowStress(pSteel);		
	double	dAllowShear		= AllowStress.GetAllowStress(ALLOW_SHEARSTRESS, StressCalc.GetMapValueSecStress("Tw"));
	double	dAllowTension	= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, StressCalc.GetMapValueSecStress("Tl"));	// �������� ��������
	double	dAllowComp		= AllowStress.GetAllowStress(ALLOW_COMPSTRESS, StressCalc.GetMapValueSecStress("Tu"));		// ����������
	pOut->SetXL(pXL, dAllowShear,	m_nSttRow, "P", "R", 4, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dAllowTension, m_nSttRow, "F", "H", 3, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dAllowComp,	m_nSttRow, "V", "X", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	//������������ �����±��� ���� �������
	double	dAllowBuckling = AllowStress.GetAllowStress(ALLOW_BUCKLINGCOEFF, StressCalc.GetMapValueSecStress("Tu"));
	pOut->SetXL(pXL, dAllowBuckling, m_nSttRow, "E", "G", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	if(bWind)	m_nSttRow += 16;
	else		m_nSttRow += 8;
	szText.Format("%.3f", dFckDeck);
	pOut->SetXL(pXL, szText,	m_nSttRow, "S", "T", 3, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.3f", dFckConf);
	pOut->SetXL(pXL, szText,	m_nSttRow, "S", "T", 0, RED, 9, FALSE, TA_CENTER, "", 0);

	m_nSttRow += 89;

	pCalcGeneral->GetMaterial(pBx, G_F_U, szLeft, szRight);
	pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft + szRight)];
	CHGAllowStress	AllowYieldUp(pSteel);
	
	szText.Format("%.1f", AllowYieldUp.GetAllowStress(ALLOW_STDYIELDPOINT_UP,  StressCalc.GetMapValueSecStress("Tu")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "N", "Q", 0, RED, 9, FALSE, TA_CENTER, "CELL_REF_ALLOW", 3);
	
	m_nSttRow += 3;
	
	szText.Format("=%s", pOut->GetCellRef("CELL_REF_ALLOW"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	
	m_nSttRow += 7;
	
	pCalcGeneral->GetMaterial(pBx, G_F_L, szLeft, szRight);
	pSteel	= pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft + szRight)];

	CHGAllowStress	AllowYieldLo(pSteel);
	szText.Format("%.1f", AllowYieldLo.GetAllowStress(ALLOW_STDYIELDPOINT_LO, StressCalc.GetMapValueSecStress("Tu")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "N", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	if(bWind)	m_nSttRow += 185;
	else		m_nSttRow += 12;
}

void CXLSectionCheck::NegativeSection_TUGirder(long nMatNo, long nG, double dSta, long nPosType)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CCalcData           *pData          = pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx			= pGir->GetBxByStation(dSta);
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CFemModelingCalc	ModelCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	long	nPosSec			= pBx->GetStation() > dSta ? -1 : 1;
	double	dStaSec			= pModelCalc->GetStationSecJewon(nMatNo, nG, dSta, nPosSec);
	if(nPosType==CXLSectionCheck::POSITIVE_SP||nPosType==CXLSectionCheck::NEGATIVE_SP)
	{
		ModelCalc.GetSectionCoefficient(nG, dSta);
		StressCalc.GetSectionStress(nG, dSta);
	}
	else
	{
		ModelCalc.GetSectionCoefficient(nG, dSta, FALSE, dStaSec, nPosSec);
		StressCalc.GetSectionStress(nG, dSta, TRUE, dStaSec, nPosSec);
	}

	CString	szText		= _T("");
	CString	szLeft		= _T("");
	CString	szRight		= _T("");
	BOOL	bWind		= pData->m_bWind;
	double	dK			= ModelCalc.GetValueOfInfK(nG, dSta);
	double	dEs			= pData->DESIGN_MATERIAL.m_dEst;			// ���� ź�����
	double	dEc			= pData->DESIGN_MATERIAL.m_dBindEc;			// ������ũ��Ʈ�� ź�����
	double	dEd			= pData->DESIGN_MATERIAL.m_dEc;				// �ٴ��� ��ũ��Ʈ�� ź�����
	double	dNc			= (double)pData->DESIGN_MATERIAL.m_nBindNc;	// ���� ������ũ��Ʈ�� ź�������
	double	dNd			= (double)pData->DESIGN_MATERIAL.m_nN;		// ���� �ٴ��� ��ũ��Ʈ�� ź�������
	double	dEpsilon	= pData->DESIGN_MATERIAL.m_dEpsilonS;		// ����������
	double	dAlpha		= pData->DESIGN_MATERIAL.m_dAlpha;			// ����â���
	double	dCreep2		= pData->DESIGN_MATERIAL.m_dCreep2;			// ũ����2
	double	dTemp		= pData->DESIGN_MATERIAL.m_dTemparature;	// ���� ��ũ��Ʈ�� �µ��� 
	double	dL			= pDB->m_dLengthBaseCrossBeam;				// ���κ� ����
	double	dG			= pData->DESIGN_MATERIAL.m_dShearEs;		// ����ź�����
	double	dCu			= pData->m_dCoverUp;
	double	dCl			= pData->m_dCoverDn;
	double	dFy			= pData->DESIGN_MATERIAL.m_dSigmaY;

	pCalcGeneral->GetMaterial(pBx, G_F_U, szLeft, szRight);
	pOut->SetXL(pXL, szLeft,	m_nSttRow, "U", "W", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, szRight,	m_nSttRow, "X", "X", 1, RED, 9, FALSE, TA_CENTER);
	pCalcGeneral->GetMaterial(pBx, G_F_L, szLeft, szRight);
	pOut->SetXL(pXL, szLeft,	m_nSttRow, "U", "W", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, szRight,	m_nSttRow, "X", "X", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Beff"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Td"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Bc"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tc"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Bu"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tu"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tw"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Hw"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Bl"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Tl"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 1, RED, 9, FALSE, TA_CENTER);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Dist"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "U", "W", 4, RED, 9, FALSE, TA_CENTER);

	szText.Format("%.6f", dEs);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dEc);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dEd);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dEpsilon);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 6);
	szText.Format("%.6f", dAlpha);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 6);
	szText.Format("%.6f", dCreep2);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);	
	szText.Format("%.6f", dNc);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dNd);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dTemp);
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph1")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	if(!pDB->IsTUContinuousSpan())
	{
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph2")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph3")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph4")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mph5")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	if(bWind)
	{
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mw")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph1")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "Sph1", 3);
	if(!pDB->IsTUContinuousSpan())
	{
		szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph2")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph3")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "Sph3", 3);
	szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph4")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "Sph4", 3);	
	szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sph5")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "Sph5", 3);
	if(bWind)
	{
		szText.Format("%.6f", tokN(StressCalc.GetMapValueSecStress("Sw")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mst")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Msct")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mvt")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	if(bWind)
	{
		szText.Format("%.6f", tokNM(StressCalc.GetMapValueSecStress("Mwt")));
		pOut->SetXL(pXL, szText,	m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 3);
	}
	szText.Format("%.6f", dCu);
	pOut->SetXL(pXL, szText,		m_nSttRow, "T", "U", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dCl);
	pOut->SetXL(pXL, szText,		m_nSttRow, "W", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dG);
	pOut->SetXL(pXL, szText,		m_nSttRow, "T", "X", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", dL);
	pOut->SetXL(pXL, szText,		m_nSttRow, "T", "X", 7, RED, 9, FALSE, TA_CENTER, "", 0);

	CString sCellArea_WEB = _T("");
	if(pDB->IsTUContinuousSpan())	sCellArea_WEB.Format("K%d", m_nSttRow - 3);
	
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Aru"));
	pOut->SetXL(pXL, szText,		m_nSttRow, "K", "M", 1, RED, 9, FALSE, TA_CENTER, "", 0);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("Arl"));
	pOut->SetXL(pXL, szText,		m_nSttRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER, "", 0);
	if(bWind)						m_nSttRow += 89;
	else							m_nSttRow += 82;
	if(pDB->IsTUContinuousSpan())	m_nSttRow -= 5;
	szText.Format("%.6f", dK);
	pOut->SetXL(pXL, szText,		m_nSttRow, "M", "O", 0, RED, 9, FALSE, TA_CENTER, "", 3);
	if(bWind)	m_nSttRow += 83;
	else		m_nSttRow += 85;
	pCalcGeneral->GetMaterial(pBx, G_W, szLeft, szRight);
	pOut->SetXL(pXL, szLeft,	m_nSttRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER);
	pOut->SetXL(pXL, szRight,	m_nSttRow, "N", "N", 0, RED, 9, FALSE, TA_CENTER);

	if(pDB->IsTUContinuousSpan())
	{
		long nSttRowTemp = m_nSttRow;

		nSttRowTemp += 2;
		szText.Format("=ABS(%s+%s+%s+%s)*10^3/%s",pOut->GetCellRef("Sph1"), pOut->GetCellRef("Sph3"),pOut->GetCellRef("Sph4"), pOut->GetCellRef("Sph5"), sCellArea_WEB);
		pOut->SetXL(pXL, szText, nSttRowTemp, "V", "Y", 0, BLACK, 9, FALSE, TA_CENTER, "", 0);
	}
	m_nSttRow += 4;
	
	CHGBaseSteel	*pSteel = pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft + szRight)];
	CHGAllowStress		AllowStress(pSteel);		
	double	dAllowShear		= AllowStress.GetAllowStress(ALLOW_SHEARSTRESS, StressCalc.GetMapValueSecStress("Tw"));
	double	dAllowTension	= AllowStress.GetAllowStress(ALLOW_TENSIONSTRESS, StressCalc.GetMapValueSecStress("Tu"));	// �������� ��������
	double	dAllowComp		= AllowStress.GetAllowStress(ALLOW_COMPSTRESS, StressCalc.GetMapValueSecStress("Tl"));		// ����������
	pOut->SetXL(pXL, dAllowShear,	m_nSttRow, "P", "R", 4, RED, 9, FALSE, TA_CENTER, "", 0);
	pOut->SetXL(pXL, dAllowTension, m_nSttRow, "F", "H", 3, RED, 9, FALSE, TA_CENTER, "", 1);
	pOut->SetXL(pXL, dAllowComp,	m_nSttRow, "V", "X", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	//������������ �����±��� ���� �������
	double	dAllowBuckling = AllowStress.GetAllowStress(ALLOW_BUCKLINGCOEFF, StressCalc.GetMapValueSecStress("Tl"));
	pOut->SetXL(pXL, dAllowBuckling, m_nSttRow, "E", "G", 0, RED, 9, FALSE, TA_CENTER, "", 1);

	if(bWind)	m_nSttRow += 17;
	else		m_nSttRow += 9;
	double	dMax_up=0, dMax_lo=0, dMax_upA=0, dMax_loA=0, dTau_up=0, dTau_lo=0, dTau_upA=0, dTau_loA=0;
	StressCalc.GetFlangeStress(nG, dSta, dMax_up, dMax_lo, dMax_upA, dMax_loA, dTau_up, dTau_lo, dTau_upA, dTau_loA, dStaSec, nPosSec);
	szText.Format("%.6f", StressCalc.GetMapValueSecStress("SigmaCA"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "F", "H", 3, RED, 9, FALSE, TA_CENTER, "", 3);

	double	dFta	= 160;
	double	dFca	= 180;
	szText.Format("%.3f", dFta);
	pOut->SetXL(pXL, szText,	m_nSttRow, "E", "G", 1, RED, 9, FALSE, TA_CENTER, "", 1);
	szText.Format("%.3f", dFca);
	pOut->SetXL(pXL, szText,	m_nSttRow, "E", "G", 0, RED, 9, FALSE, TA_CENTER, "", 1);
	
	m_nSttRow += 50;
	szText.Format("%.3f", dFy);
	pOut->SetXL(pXL, szText,	m_nSttRow, "O", "Q", 0, RED, 9, FALSE, TA_CENTER, "CELL_REF_FY", 3);
	
	m_nSttRow += 3;

	szText.Format("=%s",pOut->GetCellRef("CELL_REF_FY"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "O", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);

	m_nSttRow += 7;

	pCalcGeneral->GetMaterial(pBx, G_F_U, szLeft, szRight);
	pSteel =pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft + szRight)];
	CHGAllowStress	AllowYieldUp(pSteel);
	


	szText.Format("%.1f", AllowYieldUp.GetAllowStress(ALLOW_STDYIELDPOINT_UP,  StressCalc.GetMapValueSecStress("Tu")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "N", "Q", 0, RED, 9, FALSE, TA_CENTER, "CELL_REF_ALLOW", 3);

	m_nSttRow += 3;

	szText.Format("=%s", pOut->GetCellRef("CELL_REF_ALLOW"));
	pOut->SetXL(pXL, szText,	m_nSttRow, "N", "Q", 0, BLACK, 9, FALSE, TA_CENTER, "", 3);
	
	m_nSttRow += 7;

	pCalcGeneral->GetMaterial(pBx, G_F_L, szLeft, szRight);
	pSteel =pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(szLeft + szRight)];
	CHGAllowStress	AllowYieldLo(pSteel);

	szText.Format("%.1f", AllowYieldLo.GetAllowStress(ALLOW_STDYIELDPOINT_LO, StressCalc.GetMapValueSecStress("Tu")));
	pOut->SetXL(pXL, szText,	m_nSttRow, "N", "Q", 0, RED, 9, FALSE, TA_CENTER, "", 1);


	if(bWind)						m_nSttRow += 57;
	else							m_nSttRow += 18;
	if(pDB->IsTUContinuousSpan())	m_nSttRow -= 10;
}

void CXLSectionCheck::SectionCheck(long nPosType, CStringArray &strArrUsedSheet)
{
	CPlateBridgeApp		*pDB	= m_pAPlateOutCalcStd->GetAPlateBridgeApp();

	if(pDB->IsTUGir())	SectionCheck_TUGirder(nPosType, strArrUsedSheet);
	else				SectionCheck_GenGirder(nPosType, strArrUsedSheet);
	m_pXL->SetPrintArea(m_pXL->GetCellStr(0, 0), m_pXL->GetCellStr(m_nSttRow, 26));
}
///< (1) �ܸ� ���� - ��.�� ���Ʈ
void CXLSectionCheck::SectionCheck_GenGirder(long nPosType, CStringArray &strArrUsedSheet)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFEMManage			*pFEMManage		= pCalcStd->GetModelingFemData(BEFORE_COMPOSITE_ANALYSIS);
	CAPlateOutExcel		*pAPlateOutXL	= m_pAPlateOutCalcStd->GetAPlateOutExcel();
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CCalcData           *pData          = pDataManage->GetCalcData();
	CGlobarOption       *pGlopt         = pDataManage->GetGlobalOption();

	CDWordArray dArrMat, dArrNG;
	CDDWordArray dArrSta;
	BOOL bPositiveM = FALSE;
	if(nPosType==0 || nPosType==2 || nPosType==4 || nPosType==6 || nPosType==8)
		bPositiveM = TRUE;

	GetSectionCheckData(dArrMat, dArrNG, dArrSta, nPosType, bPositiveM);
	
	if(dArrMat.GetSize()==0)	return;

	CString sText, sLeft, sRight, szProgress, szDllSheetName, szNewSheetName;
	m_nSttRow = 1;
	pOut->ClearCellRef();
	long	nProgress	= m_pAPlateOutCalcStd->GetAPlateOutExcel()->GetProgress();
	long	nMatsu		= dArrMat.GetSize();
	BOOL	bWind		= pData->m_bWind;
	BOOL	bExist		= nMatsu > 0 ? TRUE : FALSE;
	if(!bExist)	return;

	m_nPosType = nPosType;

	switch(nPosType)
	{
	case POSITIVE_SEC:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_SEC:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�θ��Ʈ)");
			break;
		}
	case POSITIVE_SP:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�������� �����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�������� �����Ʈ)");
			break;
		}
	case NEGATIVE_SP:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �ܸ����(�������� �θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�ܸ����(�������� �θ��Ʈ)");
			break;
		}
	case POSITIVE_SECPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �߰��ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�߰��ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_SECPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 �߰��ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("�߰��ܸ����(�θ��Ʈ)");
			break;
		}
	case POSITIVE_BUYNPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ���ܸ��߰��ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("���ܸ��߰��ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_BUYNPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ���ܸ��߰��ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("���ܸ��߰��ܸ����(�θ��Ʈ)");
			break;
		}
	case POSITIVE_USERPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�����Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�����Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ������߰��ܸ����(�����Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("������߰��ܸ����(�����Ʈ)");
			break;
		}
	case NEGATIVE_USERPLUS:
		{
			if(bWind)	szDllSheetName.Format("�ܸ����(�θ��Ʈ)_ǳ����");
			else		szDllSheetName.Format("�ܸ����(�θ��Ʈ)");

			pXL->SetActiveSheet(szDllSheetName);
			szNewSheetName.Format("4.7 ������߰��ܸ����(�θ��Ʈ)");
			pXL->SheetMoveCopy(FALSE, TRUE, szNewSheetName);
			strArrUsedSheet.Add(szNewSheetName);
			pXL->SetActiveSheet(szNewSheetName);
			szProgress.Format("������߰��ܸ����(�θ��Ʈ)");
			break;
		}
	}

	pOut->SetXL(pXL, pGlopt->GetDesignHeigtBaseUpper() == 1 ? "���ǳ���" : "���ǿ���", m_nSttRow, "AB", "AB", 0);

	for(long nMat = 0; nMat < nMatsu; nMat++)
	{
		long	nMatNo		= dArrMat[nMat];
		long	nG			= dArrNG[nMat];
		long	nPosSec		= -1;
		double	dSta		= dArrSta[nMat];
		double	dStaSec		= pModelCalc->GetStationSecJewon(nMatNo, nG, dSta, nPosSec);
		double	dStaCheck	= dStaSec;
		if(nPosType==CXLSectionCheck::POSITIVE_SP||nPosType==CXLSectionCheck::NEGATIVE_SP)
			dStaCheck = dSta;
		
		pOut->ClearCellRef();

		sText.Format("%s - %d", szProgress, nMatNo);
		m_pAPlateOutCalcStd->GetAPlateOutExcel()->SetProgress(sText, nProgress+nMat*2);

		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBasicIndex	*pBx	= pGir->GetBxByStation(dStaCheck);
		
		double	dRatio			= 0;
		long	nElem			= pModelCalc->GetElemNoAndRatio(nG, dSta, dRatio);
		double	dLengthElm		= pModelCalc->GetElemLength(nElem, FALSE);
		double	dLengthFromStt	= dRatio*dLengthElm;
		CElement *pElm	= pFEMManage->GetElement((unsigned short)nElem);

		m_nSttRow++;
		if(nMat!=0)
		{
			if(bPositiveM)	pXL->InsertCopyRowLine(2, bWind ? 391 : 306, m_nSttRow);
			else			pXL->InsertCopyRowLine(2, bWind ? 387 : 309, m_nSttRow);
			// (APLATE-692) ���������� �ڸ������� �������� �θ��Ʈ 2 ��¿��� '�������� ������ ����'�޼�����.
			// Ȯ���� �ȵ����� ������ �ʾ������� �ڵ�� ���� ���̴� ���⿡ Sleep�� �ش�.
			Sleep(1000);
		}

		// �ܸ��ȣ
		if(nPosType==POSITIVE_SP || nPosType==NEGATIVE_SP)
		{
			CString sCnt = pBx->GetSplice()->GetMarkstring();
			sCnt = sCnt.Mid(2);

			if(pBx->GetSplice()->m_bSpliceType)	sText = "Splice - ";
			else								sText = "������� - ";

			sText += sCnt; 			
		}
		else
			sText.Format("< �Ŵ� %d �ܸ� - %d >", nG+1, nMatNo);
		pOut->SetXL(pXL, sText, m_nSttRow, "B", "G", 0, BLACK, 9, TRUE, TA_LEFT, "", 0);
		// ������ȣ
		CString cs;
		cs.Format("���� %d ���� %.3f m", pElm->m_nJStt+1, dLengthFromStt);
		pOut->SetXL(pXL, cs, m_nSttRow, "H", "H", 1, BLACK, 9, FALSE, TA_LEFT, "", 0);

		CCalcGeneral	*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
		pCalcGeneral->GetMaterial(pBx, G_F_U, sLeft, sRight);
		pOut->SetXL(pXL, sLeft,							m_nSttRow, "U", "W", 0, RED,  9, FALSE, TA_CENTER, "STEELTYPEUP_LEFT");
		pOut->SetXL(pXL, sRight,						m_nSttRow, "X", "X", 1, RED,  9, FALSE, TA_CENTER, "STEELTYPEUP_RIGHT");
		pCalcGeneral->GetMaterial(pBx, G_F_L, sLeft, sRight);
		pOut->SetXL(pXL, sLeft,							m_nSttRow, "U", "W", 0, RED,  9, FALSE, TA_CENTER, "STEELTYPELO_LEFT");
		pOut->SetXL(pXL, sRight,						m_nSttRow, "X", "X", 1, RED,  9, FALSE, TA_CENTER, "STEELTYPELO_RIGHT");
		pCalcGeneral->GetMaterial(pBx, G_W, sLeft, sRight);
		pOut->SetXL(pXL, sLeft,							m_nSttRow, "U", "W", 0, RED,  9, FALSE, TA_CENTER, "STEELTYPEWEB_LEFT");
		pOut->SetXL(pXL, sRight,						m_nSttRow, "X", "X", 1, RED,  9, FALSE, TA_CENTER, "STEELTYPEWEB_RIGHT");
		
		SectionResources(nMatNo, nG, dSta, nPosType);			///< �ܸ�����
		
		if(bPositiveM)
		{
			if(pAPlateOutXL->m_bAbort)	return;
			pAPlateOutXL->SetProgress(sText, nProgress+nMat*2+2);
			PositiveSection(nG, dSta, dStaCheck);
		}
		else
		{
			if(pAPlateOutXL->m_bAbort)	return;
			pAPlateOutXL->SetProgress(sText, nProgress+nMat*2+2);
			NegativeSection(nG, dSta, dStaCheck);
		}
	}
	pXL->DeleteRowLineEnd();
}

///< 1) �ܸ鰡�� �� �������� - �����Ʈ
void CXLSectionCheck::SectionResources(long nMatNo, long nG, double dSta, long nPosType)
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CFemStressCalc      *pStressCalc	= m_pAPlateOutCalcStd->GetFemStressCalc();
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CPlateGirderApp		*pGir			= pDB->GetGirder(nG);
	CPlateBxFinder		Finder(pGir);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString	sText;
	BOOL	bWind			= pCalcData->m_bWind;
	long	nPosSec			= 0;
	double	dStaSec			= pModelCalc->GetStationSecJewon(nMatNo, nG, dSta, nPosSec);
	double	dEffectWidth	= pModelCalc->GetEffectWidthSlab(nG, dSta);
	BOOL    bPositiveM      = pStressCalc->IsPositive(ELE_MOMENT3, nG, dSta);
	long	nGSec			= nG;

	if(nPosType==CXLSectionCheck::POSITIVE_SP||nPosType==CXLSectionCheck::NEGATIVE_SP)
	{
		pCalcGeneral->GetCorrectBxIfTaper(nGSec, dSta);
		pModelCalc->GetSectionCoefficient(nGSec, dSta);
		pStressCalc->GetSectionStress(nG, dSta);
	}
	else
	{
		pCalcGeneral->GetCorrectBxIfTaper(nGSec, dStaSec);
		pModelCalc->GetSectionCoefficient(nGSec, dStaSec, FALSE, dSta, nPosSec);
		pStressCalc->GetSectionStress(nG, dSta, TRUE, dStaSec);
	}

	double	dSlabT			= pModelCalc->GetMapValueSecCo("SLAB_THICK");
	double	dHunch			= pModelCalc->GetMapValueSecCo("HUNCH");
	double	dUpFlangeW		= pModelCalc->GetMapValueSecCo("FLANGE_W_UP");
	double	dUpFlangeT		= pModelCalc->GetMapValueSecCo("FLANGE_T_UP");
	double	dWebT			= pModelCalc->GetMapValueSecCo("WEB_T");
	double	dDnFlangeW		= pModelCalc->GetMapValueSecCo("FLANGE_W_DN");
	double	dDnFlangeT		= pModelCalc->GetMapValueSecCo("FLANGE_T_DN");
	double	dWebH			= pModelCalc->GetMapValueSecCo("WEB_H");
	double	dEs				= (pCalcData->DESIGN_MATERIAL.m_dEst);//���� ź�����
	double	dEc				= (pCalcData->DESIGN_MATERIAL.m_dEc);
	double	dEpsilonS		= pCalcData->DESIGN_MATERIAL.m_dEpsilonS;
	double	dAlpha			= pCalcData->DESIGN_MATERIAL.m_dAlpha;
	double	dCreep			= pCalcData->DESIGN_MATERIAL.m_dCreep2;
	long	dN				= pCalcData->DESIGN_MATERIAL.m_nN;
	double	dTemparature	= pCalcData->DESIGN_MATERIAL.m_dTemparature;
	double	dMs				= tokNM(pStressCalc->GetMapValueSecStress("MomentPreDead"));	///< �ռ��� �������߿� ���� ���Ʈ
	double	dMsc			= tokNM(pStressCalc->GetMapValueSecStress("MomentPostDead"));	///< �ռ��� �������߿� ���� ���Ʈ
	double	dMw				= 0;
	if(bWind) dMw=tokNM(pStressCalc->GetMapValueSecStress("MomentWind"));
	double	dMv				= tokNM(pStressCalc->GetMapValueSecStress("MomentLive"));	///< Ȱ���� + ����ħ�Ͽ� ���� ���Ʈ
	double	dSs				= tokN(pStressCalc->GetMapValueSecStress("ShearPreDead"));	///< �ռ��� �������߿� ���� ���ܷ�
	double	dSsc			= tokN(pStressCalc->GetMapValueSecStress("ShearPostDead"));	///< �ռ��� �������߿� ���� ���ܷ�
	double	dSw				= 0;
	if(bWind)	dSw			= tokN(pStressCalc->GetMapValueSecStress("ShearWind"));		///< ǳ���߿� ���� ���ܷ�
	double	dSv				= tokN(pStressCalc->GetMapValueSecStress("ShearLive"));		///< Ȱ���� + ����ħ�Ͽ� ���� ���Ʈ
	double	dMst			= tokNM(pStressCalc->GetMapValueSecStress("TorsionPreDead"));	///< �ռ��� �������߿� ���� ��Ʋ�� ���Ʈ
	double	dMsct			= tokNM(pStressCalc->GetMapValueSecStress("TorsionPostDead"));	///< �ռ��� �������߿� ���� ��Ʋ�� ���Ʈ
	double	dMvt			= tokNM(pStressCalc->GetMapValueSecStress("TorsionLive"));	///< (Ȱ���� + ����ħ��)�� ���� ���Ʈ
	double	dMwt			= 0;
	if(bWind) dMwt=tokNM(pStressCalc->GetMapValueSecStress("TorsionWind"));
	double	dTcu			= pCalcData->m_dCoverUp;
	double	dTcl			= pCalcData->m_dCoverDn;

	///< ���� ġ��
	pOut->SetXL(pXL, dWebH,							m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "WEB_H1",0);
	pOut->SetXL(pXL, dEffectWidth,					m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "SLAB_B1",0);
	pOut->SetXL(pXL, dUpFlangeW,					m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "UP_FLANGE_W1",0);
	pOut->SetXL(pXL, dDnFlangeW,					m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "DN_FLANGE_W1",0);
	pOut->SetXL(pXL, dSlabT,						m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "SLAB_T1",0);
	pOut->SetXL(pXL, dHunch,						m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "HUNCH",0);
	pOut->SetXL(pXL, dUpFlangeT,					m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "UP_FLANGE_T1",0);
	pOut->SetXL(pXL, dDnFlangeT,					m_nSttRow, "U", "W", 1, RED,   9, FALSE, TA_CENTER, "DN_FLANGE_T1",0);
	pOut->SetXL(pXL, dWebT,							m_nSttRow, "U", "W", 4, RED,   9, FALSE, TA_CENTER, "WEB_T1",0);

	///< TABLE
	pOut->SetXL(pXL, dEs,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "ES", 0);
	pOut->SetXL(pXL, dEc,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "EC", 0);
	pOut->SetXL(pXL, dEpsilonS,						m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "EPSILONS", 6);
	pOut->SetXL(pXL, dAlpha,						m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "ALPHA", 6);
	pOut->SetXL(pXL, dCreep,						m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "CREEP", 0);
	pOut->SetXL(pXL, dN,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "N", 0);
	pOut->SetXL(pXL, dTemparature,					m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "TEMPARATURE", 0);
	pOut->SetXL(pXL, dMs,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MS");
	pOut->SetXL(pXL, dMsc,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MSC");
	if(bWind)
		pOut->SetXL(pXL, dMw,						m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MW");	

	pOut->SetXL(pXL, dMv,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MV");
	pOut->SetXL(pXL, dSs,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "SS");
	pOut->SetXL(pXL, dSsc,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "SSC");
	if(bWind)
		pOut->SetXL(pXL, dSw,						m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "SW");
	
	pOut->SetXL(pXL, dSv,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "SV");
	pOut->SetXL(pXL, dMst,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MST");
	pOut->SetXL(pXL, dMsct,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MSCT");
	if(bWind)
		pOut->SetXL(pXL, dMwt,						m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MWT");

	pOut->SetXL(pXL, dMvt,							m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "MVT");
	if(!bPositiveM)
	{
		pOut->SetXL(pXL, dTcu,						m_nSttRow, "T", "U", 0, RED,   9, FALSE, TA_CENTER, "UPPER_PIBOK", 0);
		pOut->SetXL(pXL, dTcl,						m_nSttRow, "W", "X", 1, RED,   9, FALSE, TA_CENTER, "LOWER_PIBOK", 0);
	}
	
	double dCrossDist = pDB->m_dLengthBaseCrossBeam;
	CPlateBasicIndex *pBxSttCr = pGir->GetBxByStationDir(dSta, -1, BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEndCr = pGir->GetBxByStationDir(dSta, 1, BX_CROSSBEAM_VBRACING);

	if(pBxSttCr && pBxEndCr)
	{
		dCrossDist = pBxEndCr->GetStation() - pBxSttCr->GetStation();
	}

	pOut->SetXL(pXL, 81000.0,						m_nSttRow, "T", "X", 1, RED,   9, FALSE, TA_CENTER, "G", 0);
	pOut->SetXL(pXL, dCrossDist,	m_nSttRow, "T", "X", 0, RED,   9, FALSE, TA_CENTER, "CROSSBEAM_TERM", 0);

	if(bPositiveM)
		m_nSttRow+=10;
	else
	{
		double	dAru	= pStressCalc->GetMapValueSecStress("Aru");	///< UPPER ö�� �ܸ���
		double	dArl	= pStressCalc->GetMapValueSecStress("Arl");	///< LOWER ö�� �ܸ���
		m_nSttRow+=7;
		pOut->SetXL(pXL, dAru,	m_nSttRow, "K", "M", 1, RED,   9, FALSE, TA_CENTER, "UP_REBAR_AS", 0);
		pOut->SetXL(pXL, dArl,	m_nSttRow, "K", "M", 5, RED,   9, FALSE, TA_CENTER, "DN_REBAR_AS", 0);
	}
}

///< �ռ��� - �����Ʈ
void CXLSectionCheck::PositiveSection(long nG, double dSta, double dStaCheck)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CPlateBasicIndex	*pBx			= pDB->GetGirder(nG)->GetBxByStation(dSta);
	CPlateBasicIndex	*pBxCheck		= pDB->GetGirder(nG)->GetBxByStation(dStaCheck);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CFemStressCalc		FemStressCalc(pDataManage, pCalcStd);
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CString	sText = _T(""), strLeft = _T(""), strRight = _T(""), strUp = _T(""), strDn = _T(""), strWeb = _T("");

	BOOL	bWind		= pCalcData->m_bWind;
	BOOL	bHapSung	= pDB->IsHapsung();
	BOOL	bHybrid		= pBx->IsHybrid();

	pCalcGeneral->GetMaterial(pBx, G_W, strLeft, strRight);
	CHGBaseSteel	*pSteelL	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	pCalcGeneral->GetMaterial(pBxCheck, G_F_U,	strUp, sText);
	CHGBaseSteel	*pSteelUp	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strUp	+ sText)];
	pCalcGeneral->GetMaterial(pBxCheck, G_F_L,	strDn, sText);
	CHGBaseSteel	*pSteelDn	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strDn + sText)];
	pCalcGeneral->GetMaterial(pBxCheck, G_W,	strWeb, sText);
	CHGBaseSteel	*pSteelWeb	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strWeb + sText)];
	
	CHGAllowStress	AllowShear(pSteelL);	
	CHGAllowStress	AllowTension(pSteelDn);		// �������� ��������
	CHGAllowStress	AllowComp(pSteelUp);		// ����������
	CHGAllowStress	BucklingCoeff(pSteelUp);	// ������������ �����±��� ���� ������� ���
	CHGAllowStress	YieldPointUp(pSteelUp);		// �ǵβ��� ���� �����׺��� - ����
	CHGAllowStress	YieldPointDn(pSteelDn);		// �ǵβ��� ���� �����׺��� - ����
	CHGAllowStress	YieldPointWeb(pSteelWeb);	// �ǵβ��� ���� �����׺��� - ������

	double	dWebT			= atof(pXL->GetXL(pOut->GetCellRef("WEB_T1")));
	double	dAllow			= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, dWebT);
	double	dFck			= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dFlangeUpT		= atof(pXL->GetXL(pOut->GetCellRef("UP_FLANGE_T1")));
	double	dFlangeDnT		= atof(pXL->GetXL(pOut->GetCellRef("DN_FLANGE_T1")));
	double	dAllowTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dFlangeDnT);
	double	dAllowComp		= AllowComp.GetAllowStress(ALLOW_COMPSTRESS, dFlangeUpT);
	double	dAllowBuckling	= BucklingCoeff.GetAllowStress(ALLOW_BUCKLINGCOEFF, dFlangeUpT);
	double	dYieldPointUp	= YieldPointUp.GetAllowStress(ALLOW_STDYIELDPOINT_UP, dFlangeUpT);
	double	dYieldPointDn	= YieldPointDn.GetAllowStress(ALLOW_STDYIELDPOINT_LO, dFlangeDnT);
	double	dYieldPointWeb	= YieldPointWeb.GetAllowStress(ALLOW_STDYIELDPOINT_WEB, dWebT);

	if(bHybrid)
	{
		m_nSttRow+=2;
		pOut->SetXL(pXL, dYieldPointWeb,	m_nSttRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dYieldPointDn,		m_nSttRow, "M", "O",23, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+13);
		m_nSttRow+=25;
	}

	if(bHapSung)	sText.Format("(1) �� �� ��");
	else			sText.Format("(1) 1�� ��������(��������)�� ���� ����");
	pOut->SetXL(pXL, sText,		m_nSttRow, "B", "B", 41);
	if(bWind)	m_nSttRow+=7;

	FemStressCalc.GetSectionStress(nG, dSta);
	double	dK		= FemStressCalc.GetMapValueSecStress("K");	///<  ������
	pOut->SetXL(pXL, dK,				m_nSttRow, "G", "I", 50, RED,  9, FALSE, TA_CENTER, "K");
	
	// �����������
	pOut->SetXL(pXL, strLeft,			m_nSttRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER, "STEELTYPE_WEB_LEFT");
	pOut->SetXL(pXL, strRight,			m_nSttRow, "N", "N",12, RED, 9, FALSE, TA_CENTER, "STEELTYPE_WEB_RIGHT");	
	if(bWind)	m_nSttRow++;

	pOut->SetXL(pXL, dAllow,			m_nSttRow, "U", "V", 6, RED, 9, FALSE, TA_CENTER, "ASHEAR", 0);	
	if(bWind)	m_nSttRow+= 8;
	
	pOut->SetXL(pXL, dAllowTension,		m_nSttRow, "F", "H", 3, RED, 9, FALSE, TA_CENTER, "FTA", 0);
	pOut->SetXL(pXL, dAllowBuckling,	m_nSttRow, "E", "G", 0, RED, 9, FALSE, TA_CENTER, "COEFF", 1);
	pOut->SetXL(pXL, dAllowComp,		m_nSttRow, "V", "X", 8, RED, 9, FALSE, TA_CENTER, "ALLOW_FCA1");
	pOut->SetXL(pXL, dFck,				m_nSttRow, "S", "T", 5, RED, 9, FALSE, TA_CENTER, "FCK", 0);
	
	if(bHybrid)
	{
		m_nSttRow += bWind ? 46 : 34;
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+(bWind ? 45 : 33));
		m_nSttRow += bWind ? 46 : 34;
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+(bWind ? 45 : 33));
		m_nSttRow += bWind ? 92 : 68;
	}

	// �׺��� ���� ������ �˻�
	SafetyTest(TRUE, bWind, bHybrid, dYieldPointUp, dYieldPointDn);
}

void CXLSectionCheck::NegativeSection(long nG, double dSta, double dStaCheck)
{
	CPlateBridgeApp		*pDB			= m_pAPlateOutCalcStd->GetAPlateBridgeApp();	
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CCalcGeneral		*pCalcGeneral	= m_pAPlateOutCalcStd->GetCalcGeneral();
	CFemModelingCalc	*pModelCalc		= m_pAPlateOutCalcStd->GetFemModelingCalc();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData			*pCalcData		= pDataManage->GetCalcData();	
	CPlateGirderApp		*pGir			= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBx			= pGir->GetBxByStation(dSta);
	CPlateBasicIndex	*pBxCheck		= pGir->GetBxByStation(dStaCheck);

	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CSteel				*pSteelCtl		= pDataManage->GetSteel();
	CFemStressCalc		FemStressCalc(pDataManage, pCalcStd);
	CString	sText = _T(""), strLeft = _T(""), strRight = _T(""), strUp = _T(""), strDn = _T(""), strWeb = _T("");

	BOOL	bWind		= pCalcData->m_bWind;
	BOOL	bHapSung	= pDB->IsHapsung();
	BOOL	bHybrid		= pBx->IsHybrid();

	pCalcGeneral->GetMaterial(pBx, G_W, strLeft, strRight);
	CHGBaseSteel	*pSteelL	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strLeft + strRight)];
	pCalcGeneral->GetMaterial(pBxCheck, G_F_U,	strUp, sText);
	CHGBaseSteel	*pSteelUp	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strUp	+ sText)];
	pCalcGeneral->GetMaterial(pBxCheck, G_F_L,	strDn, sText);
	CHGBaseSteel	*pSteelDn	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strDn + sText)];
	pCalcGeneral->GetMaterial(pBxCheck, G_W,	strWeb, sText);
	CHGBaseSteel	*pSteelWeb	=	pSteelCtl->m_SteelArr[pSteelCtl->GetIndex(strWeb + sText)];

	CHGAllowStress	AllowShear(pSteelL);	
	CHGAllowStress	AllowTension(pSteelUp);			// �������� ��������
	CHGAllowStress	AllowComp(pSteelDn);			// ����������
	CHGAllowStress	BucklingCoeff(pSteelDn);		// ������������ �����±��� ���� ������� ���
	CHGAllowStress	YieldPointUp(pSteelUp);			// �ǵβ��� ���� �����׺��� - ����
	CHGAllowStress	YieldPointDn(pSteelDn);			// �ǵβ��� ���� �����׺��� - ����
	CHGAllowStress	YieldPointWeb(pSteelWeb);		// �ǵβ��� ���� �����׺��� - ������
	CHGAllowStress	AllowBendingStress(pSteelDn);
	
	double	dWebT			= atof(pXL->GetXL(pOut->GetCellRef("WEB_T1")));
	double	dAllow			= AllowShear.GetAllowStress(ALLOW_SHEARSTRESS, dWebT);
	double	dFlangeUpT		= atof(pXL->GetXL(pOut->GetCellRef("UP_FLANGE_T1")));
	double	dFlangeDnT		= atof(pXL->GetXL(pOut->GetCellRef("DN_FLANGE_T1")));
	double	dAllowTension	= AllowTension.GetAllowStress(ALLOW_TENSIONSTRESS, dFlangeUpT);
	double	dAllowComp		= AllowComp.GetAllowStress(ALLOW_COMPSTRESS, dFlangeDnT);
	double	dAllowBuckling	= BucklingCoeff.GetAllowStress(ALLOW_BUCKLINGCOEFF, dFlangeDnT);
	double	dFy				= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dYieldPointUp	= YieldPointUp.GetAllowStress(ALLOW_STDYIELDPOINT_UP, dFlangeUpT);
	double	dYieldPointDn	= YieldPointDn.GetAllowStress(ALLOW_STDYIELDPOINT_LO, dFlangeDnT);
	double	dYieldPointWeb	= YieldPointWeb.GetAllowStress(ALLOW_STDYIELDPOINT_WEB, dWebT);
	
	pModelCalc->GetSectionCoefficient(nG, dSta);
	double	dUpFlangeT	= pModelCalc->GetMapValueSecCo("FLANGE_T_UP");
	double	dDnFlangeW	= pModelCalc->GetMapValueSecCo("FLANGE_W_DN");
	double	dDnFlangeT	= pModelCalc->GetMapValueSecCo("FLANGE_T_DN");
	double	dWebH		= pModelCalc->GetMapValueSecCo("WEB_H");
	double	dAw			= (dWebH-dUpFlangeT-dDnFlangeT)*dWebT;
	double	dAc			= dDnFlangeW*dDnFlangeT;
	double	dCrossDist	= pDB->m_dLengthBaseCrossBeam;	

	CPlateBasicIndex *pBxSttCr = pGir->GetBxByStationDir(dSta,-1, BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex *pBxEndCr = pGir->GetBxByStationDir(dSta, 1, BX_CROSSBEAM_VBRACING);
	if(pBxSttCr && pBxEndCr)
	{
		dCrossDist = pBxEndCr->GetStation() - pBxSttCr->GetStation();
	}

	AllowBendingStress.SetAllowBendingStress(dAw, dAc, dCrossDist, dDnFlangeW);

	if(bHybrid)
	{
		m_nSttRow+=2;
		pOut->SetXL(pXL, dYieldPointWeb,	m_nSttRow, "I", "K", 0, RED, 9, FALSE, TA_CENTER, "", 0);
		pOut->SetXL(pXL, dYieldPointDn,		m_nSttRow, "M", "O",23, RED, 9, FALSE, TA_CENTER, "", 0);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+13);
		m_nSttRow+=25;
	}

	if(bHapSung)	sText.Format("(1) �� �� ��");
	else			sText.Format("(1) 1�� ��������(��������)�� ���� ����");
	pOut->SetXL(pXL, sText,		m_nSttRow, "B", "B", 43);
	if(bWind)	m_nSttRow+=7;

	FemStressCalc.GetSectionStress(nG, dSta);
	double	dK		= FemStressCalc.GetMapValueSecStress("K");	///<  ������
	pOut->SetXL(pXL, dK,				m_nSttRow, "G", "I", 43, RED,  9, FALSE, TA_CENTER, "K");
	
	// �����������
	pOut->SetXL(pXL, strLeft,			m_nSttRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER, "STEELTYPE_WEB_LEFT");
	pOut->SetXL(pXL, strRight,			m_nSttRow, "N", "N",12, RED, 9, FALSE, TA_CENTER, "STEELTYPE_WEB_RIGHT");	
	if(bWind)	m_nSttRow++;
	
	pOut->SetXL(pXL, dAllow,			m_nSttRow, "U", "V", 6, RED, 9, FALSE, TA_CENTER, "ASHEAR", 0);	
	if(bWind)	m_nSttRow += 8;
	
	pOut->SetXL(pXL, dAllowTension,		m_nSttRow, "F", "H", 3, RED, 9, FALSE, TA_CENTER, "FTA", 0);
	pOut->SetXL(pXL, dAllowBuckling,	m_nSttRow, "E", "G", 0, RED, 9, FALSE, TA_CENTER, "COEFF", 1);
	pOut->SetXL(pXL, dAllowComp,		m_nSttRow, "V", "X", 9, RED, 9, FALSE, TA_CENTER, "ALLOW_FCA1");

	sText.Format("%.3f", AllowBendingStress.GetAllowStress(ALLOW_BENDINGCOMP_CASE2, dDnFlangeT));
	pOut->SetXL(pXL, sText,				m_nSttRow, "F", "H", 8, RED,   9, FALSE, TA_CENTER, "FCA1");
	
	if(bHybrid)
	{
		m_nSttRow += bWind ? 46 : 34;
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+(bWind ? 45 : 33));
		m_nSttRow += bWind ? 46 : 34;
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow+(bWind ? 45 : 33));
		m_nSttRow += bWind ? 92 : 68;
	}

	// �׺��� ���� ������ �˻�
	SafetyTest(FALSE, bWind, bHybrid, dYieldPointUp, dYieldPointDn, dFy);
}

void CXLSectionCheck::SafetyTest(BOOL bPositive, BOOL bWind, BOOL bHybrid, double dYieldPointUp, double dYieldPointDn, double dFy)
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	CString sText = _T("");

	// �׺��� ���� ������ �˻�
	m_nSttRow += bWind ? 16 : 6;
	long nCellRow = 0;
	if(bHybrid)		nCellRow = m_nSttRow - (bWind?104:70);	// ���̺긮�� �ռ��� Ȱ�������� �����Ŀ� ��ġ ����
	else			nCellRow = m_nSttRow - (bWind? 58:36);	// �Ϲ� �ռ��� Ȱ�������� �����Ŀ� ��ġ ����

	if(bPositive)
	{
		// �ٴ��� ��ũ��Ʈ		
		if(bHybrid)
		{
			sText.Format("=MAX(F%d,I%d)", nCellRow, nCellRow);
			pOut->SetXL(pXL, sText,			m_nSttRow, "I", "J", 4, BLACK,	9, FALSE, TA_CENTER, "", 2);
			if(bWind) m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			sText.Format("=MAX(D%d,I%d)", nCellRow, nCellRow);
			pOut->SetXL(pXL, sText,			m_nSttRow, "I", "J", 3, BLACK,	9, FALSE, TA_CENTER, "", 2);
			if(bWind) m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
		}
	}
	else
	{
		// ��� �ٴ��� REBAR
		if(bHybrid)
		{
			sText.Format("=F%d", nCellRow);
			pOut->SetXL(pXL, sText,			m_nSttRow, "I", "J", 3, BLACK,	9, FALSE, TA_CENTER, "", 2);
			if(bWind)	m_nSttRow++;
			pOut->SetXL(pXL, dFy,			m_nSttRow, "K", "M", 1, RED,	9, FALSE, TA_CENTER);
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			sText.Format("=D%d", nCellRow);
			pOut->SetXL(pXL, sText,			m_nSttRow, "I", "J", 3, BLACK,	9, FALSE, TA_CENTER, "", 2);
			if(bWind) m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
			pOut->SetXL(pXL, dFy,			m_nSttRow, "K", "M", 0, RED,	9, FALSE, TA_CENTER);
		}
	}
	m_nSttRow+=3;
	if(bHybrid)
	{
		m_nSttRow++;
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		m_nSttRow++;
	}
	if(bWind)
	{
		m_nSttRow+=4;
		if(bHybrid)
		{
			m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
		}
		m_nSttRow+=3;
		if(bHybrid)
		{
			m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
		}
	}
	// �������� ����
	m_nSttRow += bWind ? 3 : 2;

	sText.Format("=P%d", nCellRow-1);
	pOut->SetXL(pXL, sText,			m_nSttRow, "J", "L", 2, BLACK,	9, FALSE, TA_CENTER);
	sText.Format("=P%d", nCellRow);
	pOut->SetXL(pXL, sText,			m_nSttRow, "I", "J", 3, BLACK,	9, FALSE, TA_CENTER, "", 2);
	if(bWind)	m_nSttRow++;

	if(bHybrid)
	{	
		pOut->SetXL(pXL, dYieldPointUp, m_nSttRow, "K", "M", 1, RED,	9, FALSE, TA_CENTER);
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		m_nSttRow++;
		pOut->SetXL(pXL, dYieldPointUp, m_nSttRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER);
	}
	m_nSttRow+=3;
	if(bHybrid)
	{
		m_nSttRow++;
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		m_nSttRow++;
	}
	if(bWind)
	{
		m_nSttRow+=4;
		if(bHybrid)
		{
			m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
		}
		m_nSttRow+=3;
		if(bHybrid)
		{
			m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
		}
	}
	// �������� ����
	m_nSttRow += bWind ? 3 : 2;

	sText.Format("=S%d", nCellRow-1);
	pOut->SetXL(pXL, sText,			m_nSttRow, "J", "L", 2, BLACK,	9, FALSE, TA_CENTER);
	sText.Format("=S%d", nCellRow);
	pOut->SetXL(pXL, sText,			m_nSttRow, "I", "J", 3, BLACK,	9, FALSE, TA_CENTER, "", 2);
	if(bWind)	m_nSttRow++;

	if(bHybrid)
	{	
		pOut->SetXL(pXL, dYieldPointDn, m_nSttRow, "K", "M", 1, RED,	9, FALSE, TA_CENTER);
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		m_nSttRow++;
		pOut->SetXL(pXL, dYieldPointDn, m_nSttRow, "K", "M", 0, RED, 9, FALSE, TA_CENTER);
	}
	m_nSttRow+=3;
	if(bHybrid)
	{
		m_nSttRow++;
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
	}
	else
	{
		pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		m_nSttRow++;
	}
	if(bWind)
	{
		m_nSttRow+=4;
		if(bHybrid)
		{
			m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
		}
		m_nSttRow+=3;
		if(bHybrid)
		{
			m_nSttRow++;
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
		}
		else
		{
			pXL->DeleteRowLine(m_nSttRow, m_nSttRow);
			m_nSttRow++;
		}
	}

	m_nSttRow+=6;
	if(bWind)	m_nSttRow+=7;
}
//////////////////////////////////////////////////////////////////////////

void CXLSectionCheck::SectionOutLine()
{
	CPlateBridgeApp		*pDB		= m_pAPlateOutCalcStd->GetAPlateBridgeApp();
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CAPlateCalcStd		*pCalcStd		= m_pAPlateOutCalcStd->GetAPlateCalcStd();
	CFemStressCalc		StressCalc(pDataManage, pCalcStd);
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;

	pXL->SetPageDirection(TRUE);

	m_nSttRow = 1;
	if(pDB->m_nQtyJigan > 1)	pOut->SetXL(pXL, "4.8 �ܸ� ��൵", m_nSttRow, "B", "B", 1);
	else						pOut->SetXL(pXL, "4.7 �ܸ� ��൵", m_nSttRow, "B", "B", 1);

	CString sText;

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		sText.Format("< �Ŵ� %d >", nG+1);
		pOut->SetXL(pXL, sText, m_nSttRow, "C", "F");

		CDomyun	Dom;
		Dom.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom.ClearEtt(TRUE);
		StressCalc.OutputOfMomentDiagram(&Dom, nG, 2.0, 200.0, NULL, NULL, 0, TRUE);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom, m_nSttRow, 0, 10, 40, 40, TRUE, TRUE);	
		SetLines(2);

		CDomyun	Dom2;
		Dom2.SetCWnd(m_pAPlateOutCalcStd->GetCWnd());
		Dom2.ClearEtt(TRUE);
		StressCalc.OutputOfSectionSummary(&Dom2, nG, 1.8, 2.5, 200.0);
		pOut->InsertDomyunByFixWidthAndHeight(pXL, &Dom2, m_nSttRow, 0, 10, 40, 40, TRUE, TRUE);	
		SetLines(2);
	}
}

void CXLSectionCheck::SetLines(int nLine)
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

void CXLSectionCheck::SetTUContinousNegativeDLL()
{
	CAPlateOutCalcXL	*pOut			= m_pAPlateOutCalcXL;
	CXLControl			*pXL			= m_pXL;
	CDataManage			*pDataManage	= m_pAPlateOutCalcStd->GetDataManage();
	CCalcData           *pData          = pDataManage->GetCalcData(); 

	BOOL	bWind	= pData->m_bWind;
	long	nSttRow	= 29;
	CString	szText	= _T("");

	// DeleteRowLine ���� nSttRow++ �� ���, ������� �������� �ϴ� ����߻�
	pXL->DeleteRowLine(nSttRow, nSttRow);	nSttRow++;
	szText.Format("Mph2  : ������ũ��Ʈ ���� + �ٴ��� ���߿� ���� ���Ʈ");
	pOut->SetXL(pXL, szText, nSttRow, "B", "P");	
	szText.Format("Mph3  : 2�� �������߿� ���� ���Ʈ");
	pOut->SetXL(pXL, szText, nSttRow, "B", "P");
	szText.Format("Mph4  : (Ȱ����+����ħ��)�� ���� ���Ʈ");
	pOut->SetXL(pXL, szText, nSttRow, "B", "P", 2);
	if(bWind)	nSttRow++;

	pXL->DeleteRowLine(nSttRow, nSttRow);	nSttRow++;
	szText.Format("Sph2  : ���� ��ũ��Ʈ ���� + �ٴ��� ���߿� ���� ���ܷ�");
	pOut->SetXL(pXL, szText, nSttRow, "B", "P");	
	szText.Format("Sph3 : 2�� �������߿� ���� ���ܷ�");
	pOut->SetXL(pXL, szText, nSttRow, "B", "P");
	szText.Format("Sph4  : (Ȱ����+����ħ��)�� ���� ���ܷ�");
	pOut->SetXL(pXL, szText, nSttRow, "B", "P", 50);
	if(bWind)	nSttRow+=3;

	pXL->DeleteRowLine(nSttRow, nSttRow+4);
	nSttRow+=5;
	szText.Format("(2) Phase 2 - ���� ��ũ��Ʈ+�ٴ��� ��ũ��Ʈ ���߿� ���� ����");

	pXL->SetXL(pXL->GetCellStr(nSttRow-1, 0), "2");
	pOut->SetXL(pXL, szText, nSttRow, "B", "B", 6);

	nSttRow = bWind ? 264 : 248;
	pXL->DeleteRowLine(nSttRow, nSttRow);	nSttRow++;
	if(bWind)
	{
		szText.Format("=E263+V96");
		pOut->SetXL(pXL, szText, nSttRow, "E", "H", 0, 1, 9, FALSE, TA_CENTER);
		szText.Format("=I263+V97");
		pOut->SetXL(pXL, szText, nSttRow, "I", "L", 0, 1, 9, FALSE, TA_CENTER);

		nSttRow--;
		szText.Format("Ca2:Ca1+(2)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca3:Ca2+(3)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca4:Ca3+(4)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca5:Ca4+(6)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca6:Ca5+(7)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca7:Ca6+(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca8:Ca6-(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca9:Ca3+(5)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca10:Ca4+��(5)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca11:Ca10+(6)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca12:Ca11+(7)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca13:Ca12+(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca14:Ca12-(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);

		nSttRow+=6;
		pXL->DeleteRowLine(nSttRow, nSttRow);	nSttRow++;
		szText.Format("������ũ��Ʈ+�ٴ��� ����");
		pOut->SetXL(pXL, szText, nSttRow, "U","AA", 0, 1, 9, FALSE, TA_CENTER);
		
		nSttRow--;
		szText.Format("Ca2:Ca1+(2)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca3:Ca2+(3)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca4:Ca3+(4)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca5:Ca4+(6)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca6:Ca5+(7)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca7:Ca6+(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca8:Ca6-(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca9:Ca3+(5)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca10:Ca4+��(5)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca11:Ca10+(6)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca12:Ca11+(7)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca13:Ca12+(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca14:Ca12-(8)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
	}
	else
	{
		szText.Format("=E247+V93");
		pOut->SetXL(pXL, szText, nSttRow, "E", "H", 0, 1, 9, FALSE, TA_CENTER);
		szText.Format("=I247+V94");
		pOut->SetXL(pXL, szText, nSttRow, "I", "L", 0, 1, 9, FALSE, TA_CENTER);
		szText.Format("������ũ��Ʈ+�ٴ��� ����");
		pOut->SetXL(pXL, szText, nSttRow, "U","AA", 0, 1, 9, FALSE, TA_CENTER);

		nSttRow--;
		szText.Format("Ca2:Ca1+(2)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca3:Ca2+(3)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca4:Ca3+(4)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca5:Ca4+(5)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca6:Ca5+(6)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca7:Ca6+(7)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca8:Ca6-(7)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);

		nSttRow+=6;
		pXL->DeleteRowLine(nSttRow, nSttRow);	nSttRow++;
		szText.Format("������ũ��Ʈ+�ٴ��� ����");
		pOut->SetXL(pXL, szText, nSttRow, "U","AA", 0, 1, 9, FALSE, TA_CENTER);
		
		nSttRow--;
		szText.Format("Ca2:(2)");		pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca3:Ca2+(3)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca4:Ca3+(4)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca5:Ca4+(5)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca6:Ca5+(6)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca7:Ca6+(7)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
		szText.Format("Ca8:Ca6-(7)");	pXL->SetXL(pXL->GetCellStr(nSttRow++, 0), szText);
	}

	pXL->DeleteRowLineEnd();
}