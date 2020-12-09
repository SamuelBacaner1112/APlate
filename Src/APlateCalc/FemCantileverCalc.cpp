// FemCantileverCalc.cpp: implementation of the CFemCantileverCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateCalc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define EPSILON 0.000001

CFemCantileverCalc::CFemCantileverCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd)
{
	m_pDataManage	= pDataManage;
	m_pCalcStd		= pCalcStd;
}

CFemCantileverCalc::~CFemCantileverCalc()
{

}

BOOL CFemCantileverCalc::IsTaper(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	double	dSta	= pBx->GetStation();
	long	nQtyGir	= pDB->GetQtyGirder();

	if(nQtyGir==pBx->GetNumberGirder()+1)
	{
		if(pBx->GetGirder()->GetHeightTaper(dSta) > 0)
			return TRUE;
		else
			return FALSE;
	}

	for(long nG=pBx->GetNumberGirder()+1; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBasicIndex	*pBxMat	= pBx->GetBxMatchByCrossBeam(pGir);

		if(pBxMat && pGir->GetHeightTaper(pBxMat->GetStation())>0)
			return TRUE;
	}
	return FALSE;
}

CPlateBasicIndex* CFemCantileverCalc::GetModelingPosition()
{
	CPlateBridgeApp		*pDB			= m_pDataManage->GetBridge();

	for(long nG=0; nG<pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBx	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
		while(pBx)
		{
			if(!IsTaper(pBx))
				return pBx;
			pBx = Finder.GetBxNext();
		}
	}

	return NULL;
}

void CFemCantileverCalc::MakeCantileverModeling(long nMakeType)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CAPlateCalcStd		*pCalcStd	= m_pCalcStd;
	CFEMManage			*pFEM		= NULL;
	CPlateBasicIndex	*pBx		= GetModelingPosition();
	
	CRect rect = CRect(200, 3, 400, 18);
	if(!m_ctlProgress.GetSafeHwnd())
		m_ctlProgress.Create(WS_VISIBLE | WS_CHILD, rect, ((CFrameWnd*)AfxGetMainWnd())->GetMessageBar(), 101);
	m_ctlProgress.SetRange(0, 100);
	m_ctlProgress.ShowWindow(SW_SHOW);	
	

	m_ctlProgress.SetPos(10);

	long	nLoadCaseIndex	= 0;
	pCalcStd->RemoveCantileverFemData();
	pCalcStd->MakeCantileverFemData(1);

	pFEM	= m_pCalcStd->GetCantileverFemData(0);
	pFEM->m_szMidasVersion	= MIDAS_VER_7DOT2;
	pFEM->m_bOutRemarkText	= TRUE;
	pFEM->SYSTEM_VAR.m_szUnitType = "KN";
	pFEM->SYSTEM_VAR.m_UX = pFEM->SYSTEM_VAR.m_UY = pFEM->SYSTEM_VAR.m_UZ = 0;
	pFEM->SYSTEM_VAR.m_RX = pFEM->SYSTEM_VAR.m_RY = pFEM->SYSTEM_VAR.m_RZ = 0;
	pFEM->m_szLCName.RemoveAll();
	pFEM->m_szTitle.RemoveAll();
	pFEM->m_szLCComment.RemoveAll();

	MakeJointData(pFEM, pBx);
	MakeElementData(pFEM, pBx);
	MakeMaterialData(pFEM);
	MakeFrameSectionData(pFEM, pBx);
	MakeGroupData(pFEM);
	MakeTDMData(pFEM);
	MakeTDNData(pFEM, pBx);
	MakeSlabSelfLoad(pFEM, pBx, nLoadCaseIndex);
 	MakePostSelfLoad(pFEM, pBx, nLoadCaseIndex);
	MakeLiveLoad(pFEM, pBx, nLoadCaseIndex);
	MakePeopleLoad(pFEM, pBx, nLoadCaseIndex);
	MakeTankLoad(pFEM, pBx, nLoadCaseIndex);
	MakeTrailerLoad(pFEM, pBx, nLoadCaseIndex);
 	MakeWindLoad(pFEM, pBx, nLoadCaseIndex);
 	MakeCrashLoad(pFEM, pBx, nLoadCaseIndex);
 	MakeCentrifugalLoad(pFEM, pBx, nLoadCaseIndex);
	//MakeTendenLoad(pFEM);
	MakeContructionStage(pFEM);
}


double CFemCantileverCalc::GetHDanArea(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	CDPoint A[HDANLINESU];
	CDPoint B[10];
	
	CPlateGirderApp		*pGirL	= pDB->GetGirder(0);
	CPlateGirderApp		*pGirR	= pDB->GetGirder(pDB->GetQtyGirder()-1);
	CPlateBasicIndex	*pBxL	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirL);
	CPlateBasicIndex	*pBxR	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirR);
	
	double dHTaper	= pBx->GetGirder()->GetHeightTaper(pBx->GetStation());
	double dHTaperL = pBxL ? pGirL->GetHeightTaper(pBxL->GetStation()) : dHTaper;
	double dHTaperR = pBxR ? pGirR->GetHeightTaper(pBxR->GetStation()) : dHTaperL;
	
	pBx->GetGirder()->GetHunchXyDom(pBx,B, FALSE, FALSE, pDB->IsTUGir());
	
	pDB->GetXyPaveUpperDom(pBx,A);
	
	CVectorArray vArr;
	long n = 0;
	for(n = 0; n <HDANLINESU; n++)
	{
		if(A[n].IsNull())
			continue;

		vArr.Add(CVector(A[n].x,A[n].y-pDB->m_dThickPave,0));
	
	}
	
	double	dTSlabLeft = pDB->GetSlabThick(CPlateBridgeApp::SLAB_LEFT)  + dHTaperL;
	double	dTSlabRigh = pDB->GetSlabThick(CPlateBridgeApp::SLAB_RIGHT) + dHTaperR;
	
	CDPoint	xyBottomStt(vArr[0].x, vArr[0].y- dTSlabLeft);
	CDPoint	xyBottomEnd(vArr[vArr.GetSize()-1].x,	vArr[vArr.GetSize()-1].y - dTSlabRigh);

	vArr.Add(CVector(0,-pDB->m_dThickPave,0));	
	vArr.Sort(vArr.GetSize(),TRUE);
	vArr.Add(CVector(xyBottomEnd.x,xyBottomEnd.y,0));
	
	for(n = 9; n >= 0; n--)
	{
		CDPoint xyB = B[n];
		if(xyB.IsNull())
			continue;

		vArr.Add(CVector(xyB.x,xyB.y,0));
	}

	vArr.RemoveSameValue();		
	vArr.Add(CVector(xyBottomStt.x, xyBottomStt.y,0));
	vArr.Add(vArr[0]);
	
	CVector cCenter;

	return vArr.GetAreaInfo(cCenter);

}
double CFemCantileverCalc::GetHDanAreaLength(CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	
	CDPoint A[HDANLINESU];
	CDPoint B[10];
	
	CPlateGirderApp		*pGirL	= pDB->GetGirder(0);
	CPlateGirderApp		*pGirR	= pDB->GetGirder(pDB->GetQtyGirder()-1);
	CPlateBasicIndex	*pBxL	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirL);
	CPlateBasicIndex	*pBxR	= pBx->GetGirder()->GetBxMatchByCrossBeam(pBx, pGirR);
	
	double dHTaper	= pBx->GetGirder()->GetHeightTaper(pBx->GetStation());
	double dHTaperL = pBxL ? pGirL->GetHeightTaper(pBxL->GetStation()) : dHTaper;
	double dHTaperR = pBxR ? pGirR->GetHeightTaper(pBxR->GetStation()) : dHTaperL;
	
	pBx->GetGirder()->GetHunchXyDom(pBx,B, FALSE, FALSE, pDB->IsTUGir());
	
	pDB->GetXyPaveUpperDom(pBx,A);
	
	CDPointArray	xyArr;
	long n = 0;
	for(n = 0; n <HDANLINESU; n++)
	{
		if(A[n].IsNull())
			continue;

		xyArr.Add(CDPoint(A[n].x,A[n].y-pDB->m_dThickPave));
		
	}
	
	double	dTSlabLeft = pDB->GetSlabThick(CPlateBridgeApp::SLAB_LEFT)  + dHTaperL;
	double	dTSlabRigh = pDB->GetSlabThick(CPlateBridgeApp::SLAB_RIGHT) + dHTaperR;

	CDPoint	xyBottomStt(xyArr[0].x, xyArr[0].y- dTSlabLeft);
	CDPoint	xyBottomEnd(xyArr[xyArr.GetSize()-1].x,	xyArr[xyArr.GetSize()-1].y - dTSlabRigh);

	xyArr.Add(CDPoint(0,-pDB->m_dThickPave));	
	xyArr.Sort();
	xyArr.Add(CDPoint(xyBottomEnd.x,xyBottomEnd.y));
	
	for(n = 9; n >= 0; n--)
	{
		CDPoint xyB = B[n];
		if(xyB.IsNull())
			continue;

		xyArr.Add(CDPoint(xyB.x,xyB.y));
	}
	
	xyArr.RemoveSameValue();
	xyArr.Add(CDPoint(xyBottomStt.x, xyBottomStt.y));
	xyArr.Add(xyArr[0]);
	
	double dSum = 0.0;

	for(n = 0; n < xyArr.GetSize()-1 ; n++)
	{	
		dSum += ~(xyArr[n+1] -xyArr[n]);
	}

	return dSum;
}

void CFemCantileverCalc::MakeJointData(CFEMManage *pFEM, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CJoint			*pJoint		= NULL;

	CDPoint	A[10];
	for(long n=0; n<10; n++)
		A[n] = CDPoint(0,0);
	pDB->GetXyFrontCantilever(pBx, A, TRUE);

	CWordArray	SlaveArr;
	long	nNode		= 0;
	double	dThickPave	= pDB->GetThickPave();
	double	dFu			= pBx->GetFactChain(G_F_U)->m_dWidth;
	CDPoint	vTendonStt	= GetXyTensionBar(pBx, TRUE);
	CDPoint	vTendonEnd	= GetXyTensionBar(pBx, FALSE);
	CVector	vJoint		= CVector(0,0,0);
	CVector	vMove		= CVector(A[0].x, 0, 0);
	CVector	vTop		= CVector(0, 0, 0);
	CVector	vBot		= CVector(0, 0, 0);

// Slab Joint
	vJoint	= CVector(A[0].x, 0, 0);
	pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB);
	pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[0].x)-dThickPave);

	vJoint	= CVector(vTendonStt.x, 0, 0);
	if (vTendonStt.x!=0)
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB);
	pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, vTendonStt.x)-dThickPave);
	long nG = 0;
	for(nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
		CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
		pGir->GetHunchXyDom(pBxMatch, A);

		vJoint	= CVector(A[1].x, 0, 0);
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB);
		pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[1].x)-dThickPave);

		if(nG==0)
		{
			vJoint	= CVector(A[1].x+dFu/4, 0, 0);
			pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB, -1, JOINT_LEFT_CANTILEVER);
			pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[1].x+dFu/4)-dThickPave);
		}

		SlaveArr.Add((WORD)nNode);
		vJoint	= CVector(A[1].x+dFu/2, 0, 0);
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB);
		pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[1].x+dFu/2)-dThickPave);

		if(nG==pDB->GetQtyGirder()-1)
		{
			vJoint	= CVector(A[2].x-dFu/4, 0, 0);
			pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB, -1, JOINT_RIGHT_CANTILEVER);
			pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[2].x-dFu/4)-dThickPave);
		}

		vJoint	= CVector(A[2].x, 0, 0);
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB, -1, JOINT_SLAB_CENT);
		pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[2].x)-dThickPave);

		if(nG!=pDB->GetQtyGirder()-1)
		{
			vJoint	= CVector(A[4].x, 0, 0);
			pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB, -1, JOINT_SLAB_DANBU);
			pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[4].x)-dThickPave);

			vJoint	= CVector((A[4].x+A[5].x)/2, 0, 0);
			pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB, -1, JOINT_SLAB_DANBU);
			pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, (A[4].x+A[5].x)/2)-dThickPave);

			vJoint	= CVector(A[5].x, 0, 0);
			pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB, -1, JOINT_SLAB_CENT);
			pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[5].x)-dThickPave);
		}
	}

	vJoint	= CVector(vTendonEnd.x, 0, 0);
	if (vTendonEnd.x!=0)
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB);

	pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, vTendonEnd.x)-dThickPave);

	pDB->GetXyFrontCantilever(pBx, A, FALSE);
	vJoint	= CVector(A[0].x, 0, 0);
	pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_SLAB);
	pJoint->m_vPointReal.x	= toM(pDB->GetThickSlabAndPaveByDis(pBx, A[0].x)-dThickPave);

	// 절점정렬
	CDoubleArray dArrPosx;
	long i = 0;
	for (i=0; i<pFEM->m_JointList.GetCount(); i++)
	{
		POSITION pos = pFEM->m_JointList.FindIndex(i);
		CJoint*	 pJ  = (CJoint*)pFEM->m_JointList.GetAt(pos);
		dArrPosx.Add(pJ->m_vPoint.x);
	}
	dArrPosx.Sort();
	for (i=0; i<pFEM->m_JointList.GetCount(); i++)
	{
		POSITION pos = pFEM->m_JointList.FindIndex(i);
		CJoint*	 pJ  = (CJoint*)pFEM->m_JointList.GetAt(pos);
		pJ->m_vPoint.x = dArrPosx.GetAt(i);
	}

 
// Boundary Group
	CBoundaryGroup *pBoundGroup	= pFEM->AddBoundaryGroup("지점");

// Girder Joint
	for(nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
		CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
		pGir->GetHunchXyDom(pBxMatch, A);

		// 상부 플랜지 좌측
		vJoint	= CVector(A[1].x, 0, -pDB->m_dThickSlabBase);
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_GIR_UPPERSIDE, nG);

		// 상부 플랜지 중앙
		CString	szConst	= _T("");
		szConst.Format("Type%d", nG+1);
		pFEM->AddConstraints(szConst, "Type", 1, 1, 1, 1, 1, 1, nNode+1, SlaveArr.GetAt(nG)+1);
		pBoundGroup->m_nNodeRigid.Add((WORD)nNode+1);

		vJoint	= CVector((A[1].x+A[2].x)/2, 0, -pDB->m_dThickSlabBase);
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_GIR_CENT, nG);

		// 상부 플랜지 우측
		vJoint	= CVector(A[2].x, 0, -pDB->m_dThickSlabBase);
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_GIR_UPPERSIDE, nG);


		// 복부
		CPlateCrossBeam	*pC	= pBx->GetSection()->GetCrossBeam();
		if (pC)
		{
			vJoint	= CVector((A[1].x+A[2].x)/2, 0, -pDB->m_dThickSlabBase-pC->m_dLH1-pC->m_H_CR/2);
			pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_CROSS, nG);
		}

		// 하부 플랜지 좌측
		vJoint	= CVector(A[1].x, 0, -pDB->m_dThickSlabBase-pBx->GetHeightGirderByBx());
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_GIR_LOWERSIDE, nG);
		// 하부 플랜지 중앙
		pBoundGroup->m_nNodeConst.Add((WORD)nNode+1);
		vJoint	= CVector((A[1].x+A[2].x)/2, 0, -pDB->m_dThickSlabBase-pBx->GetHeightGirderByBx());
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_GIR_CENT, nG);
		pJoint->m_UX = 1;	pJoint->m_RX = 1;
		pJoint->m_UY = 1;	pJoint->m_RY = 1;
		pJoint->m_UZ = 1;	pJoint->m_RZ = 1;
		// 하부 플랜지 우측
		vJoint	= CVector(A[2].x, 0, -pDB->m_dThickSlabBase-pBx->GetHeightGirderByBx());
		pJoint	= AddJointData(pFEM, nNode, vJoint, vMove, JOINT_GIR_LOWERSIDE, nG);
	}
}

void CFemCantileverCalc::MakeElementData(CFEMManage *pFEM, CPlateBasicIndex *pBx)
{
	CElement		*pElem	= NULL;

	long	nElem		= 0;
	long	nNode		= 0;
// slab element
	for(nNode=0; nNode<pFEM->GetJointSize()-1; nNode++)
	{
		CJoint	*pJ_I	= pFEM->GetJoint((unsigned short)nNode);
		CJoint	*pJ_J	= pFEM->GetJoint((unsigned short)nNode+1);

		if(pJ_I->m_LongAttrib[JOINT_TYPE]==JOINT_SLAB && pJ_J->m_LongAttrib[JOINT_TYPE]==JOINT_SLAB)
		{
			pElem	= pFEM->AddElement((unsigned short)nElem, (unsigned short)nNode, (unsigned short)nNode+1);
			pElem->m_LongAttrib[ELEM_TYPE]	= ELEM_SLAB;
			nElem++;
		}
	}
// flange element
	for(nNode=0; nNode<pFEM->GetJointSize(); nNode++)
	{
		CJoint	*pJ_I	= pFEM->GetJoint((unsigned short)nNode);
		CJoint	*pJ_J	= pFEM->GetJoint((unsigned short)nNode+1);

		long	nType_I	= pJ_I->m_LongAttrib[JOINT_TYPE];
		long	nType_J	= pJ_J ? pJ_J->m_LongAttrib[JOINT_TYPE] : JOINT_GIR_LOWERSIDE;
		long	nG_I	= pJ_I->m_LongAttrib[GIRDER_NUM];
		long	nG_J	= pJ_J ? pJ_J->m_LongAttrib[GIRDER_NUM] : nG_I;

		if(nG_I!=nG_J)			continue;

		if((nType_I==JOINT_GIR_UPPERSIDE && nType_J==JOINT_GIR_CENT) || (nType_I==JOINT_GIR_CENT && nType_J==JOINT_GIR_UPPERSIDE))
		{
			pElem	= pFEM->AddElement((unsigned short)nElem, (unsigned short)nNode, (unsigned short)nNode+1);
			pElem->m_LongAttrib[ELEM_TYPE]	= ELEM_UPPER_FLANGE;
			nElem++;
		}
		else if((nType_I==JOINT_GIR_LOWERSIDE&& nType_J==JOINT_GIR_CENT) || (nType_I==JOINT_GIR_CENT && nType_J==JOINT_GIR_LOWERSIDE))
		{
			pElem	= pFEM->AddElement((unsigned short)nElem, (unsigned short)nNode, (unsigned short)nNode+1);
			pElem->m_LongAttrib[ELEM_TYPE]	= ELEM_LOWER_FLANGE;
			nElem++;
		}
	}
// web element
	long	nStt	= 0;
	CPlateCrossBeam	*pC	= pBx->GetSection()->GetCrossBeam();

	if (pC)
	{
		for(nNode=0; nNode<pFEM->GetJointSize()-2; nNode++)
		{
			CJoint	*pJ_I	= pFEM->GetJoint((unsigned short)nNode);
			CJoint	*pJ_J	= pFEM->GetJoint((unsigned short)nNode+2);
			
			long	nType_I	= pJ_I->m_LongAttrib[JOINT_TYPE];
			long	nType_J	= pJ_J->m_LongAttrib[JOINT_TYPE];
			
			if((nType_I==JOINT_GIR_CENT && nType_J==JOINT_CROSS) || (nType_I==JOINT_CROSS && nType_J==JOINT_GIR_CENT))
			{
				pElem	= pFEM->AddElement((unsigned short)nElem, (unsigned short)nNode, (unsigned short)nNode+2);
				pElem->m_LongAttrib[ELEM_TYPE]	= ELEM_WEB;
				nElem++;
			}
		}
	}
	else
	{
		for (nNode=0; nNode<pFEM->GetJointSize()-3; nNode++)
		{
			CJoint	*pJ_I	= pFEM->GetJoint((unsigned short)nNode);
			CJoint	*pJ_J	= pFEM->GetJoint((unsigned short)nNode+3);
			
			long	nType_I	= pJ_I->m_LongAttrib[JOINT_TYPE];
			long	nType_J	= pJ_J->m_LongAttrib[JOINT_TYPE];

			if (nType_I==JOINT_GIR_CENT && nType_J==JOINT_GIR_CENT)
			{
				pElem = pFEM->AddElement((unsigned short)nElem, (unsigned short)nNode, (unsigned short)nNode+3);
				pElem->m_LongAttrib[ELEM_TYPE] = ELEM_WEB;
				nElem++;
				nNode+=3;
			}
		}
	}

/// crossbeam Element
//	CPlateCrossBeam	*pC	= pBx->GetSection()->GetCrossBeam();
//	if (!pC) return;
	nStt	= 0;
	for(nNode=0; nNode<pFEM->GetJointSize(); nNode++)
	{
		CJoint	*pJ	= pFEM->GetJoint((unsigned short)nNode);
		if(nStt==0 && pJ->m_LongAttrib[JOINT_TYPE]==JOINT_CROSS)
			nStt	= nNode;
		else if(nStt!=0 && pJ->m_LongAttrib[JOINT_TYPE]==JOINT_CROSS)
		{
			pElem = pFEM->AddElement((unsigned short)nElem, (unsigned short)nStt, (unsigned short)nNode);
			pElem->m_LongAttrib[ELEM_TYPE]	= ELEM_CROSS;
			nStt = nNode;
			nElem++;
		}
	}
}

void CFemCantileverCalc::MakeMaterialData(CFEMManage *pFEM)
{
	CCalcData		*pData		= m_pDataManage->GetCalcData();

	double	dEc			= tokNPM2(pData->DESIGN_MATERIAL.m_dEc);			// 콘크리트 탄성계수
	double	dEst		= tokNPM2(pData->DESIGN_MATERIAL.m_dEst);			// 강재 탄성계수
	double	dEs			= tokNPM2(pData->DESIGN_STEELPRESTRESS_DATA.m_dEs);	// 강선 탄성계수
	double	dPc			= 1.0/6.0;							// 콘크리트포아슨비
	double	dPs			= 0.3;								// 강재포아슨비
	double	dThermal	= pData->DESIGN_MATERIAL.m_dAlpha;	// 선팽창계수
	double	dDen		= tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dGangjae);	
	pFEM->AddMaterial("CONC40", MAT_TYPE_CONC, dEc, 0, 'C', 0, 0, dPc, dThermal);
	pFEM->AddMaterial("ST", MAT_TYPE_STEEL, dEst, 0, 'C', 0, 0, dPs, dThermal);
	pFEM->AddMaterial("강선", MAT_TYPE_USER, dEs, dDen, 'C', 0, 0, dPs, dThermal);
}

void CFemCantileverCalc::MakeFrameSectionData(CFEMManage *pFEM, CPlateBasicIndex *pBx)
{
	CFrameSection	*pFrmSec	= NULL;

	CString	szSecName	= _T("");
	long	nIndex		= 0;

	for(long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		CElement		*pElem	= pFEM->GetElement((unsigned short)nElem);
		CJoint			*pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		CJoint			*pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);

		long	nType	= pElem->m_LongAttrib[ELEM_TYPE];
		if(nType==ELEM_SLAB)
		{
			double	dHeightStt	= pJ_I->m_vPointReal.x;
			double	dHeightEnd	= pJ_J->m_vPointReal.x;
			pFrmSec = GetFrameSectionByHeight(pFEM, dHeightStt, dHeightEnd);
			if(pFrmSec)
				pElem->m_szFrameSectionName	= pFrmSec->m_szName;
			else
			{
				szSecName.Format("TAP%d", nIndex+1);
				pElem->m_szFrameSectionName	= szSecName;
				pFrmSec	= pFEM->AddFrameSectionOfTaper(szSecName, "CONC40", _T(""), SOLID_RECTANGLE);
				pFrmSec->m_dJewon[0]	= pJ_I->m_vPointReal.x;
				pFrmSec->m_dJewon[1]	= 1.0;
				pFrmSec->m_dJewon[2]	= pJ_J->m_vPointReal.x;
				pFrmSec->m_dJewon[3]	= 1.0;
				nIndex++;
			}
		}
		else if(nType==ELEM_UPPER_FLANGE)	pElem->m_szFrameSectionName	= _T("UPL");
		else if(nType==ELEM_LOWER_FLANGE)	pElem->m_szFrameSectionName	= _T("LPL");
		else if(nType==ELEM_WEB)			pElem->m_szFrameSectionName	= _T("WEB");
		else if(nType==ELEM_CROSS)			pElem->m_szFrameSectionName	= _T("CROSS");
	}

	CPlateCrossBeam	*pCr	= pBx->GetSection()->GetCrossBeam();
	if (!pCr) return;

	double	dHu	= toM(pBx->GetFactChain(G_F_U)->m_dFactChainThick);
	double	dBu	= 1.0;
	double	dHl	= toM(pBx->GetFactChain(G_F_L)->m_dFactChainThick);
	double	dBl	= 1.0;
	double	dHw	= toM(pBx->GetFactChain(G_W)->m_dFactChainThick);
	double	dBw	= 1.0;
	pFrmSec	= pFEM->AddFrameSectionOfRectangle("UPL", "ST", dBu, dHu);
	pFrmSec	= pFEM->AddFrameSectionOfRectangle("LPL", "ST", dBl, dHl);
	pFrmSec	= pFEM->AddFrameSectionOfRectangle("WEB", "ST", dBw, dHw);
	pFrmSec	= pFEM->AddFrameSection("CROSS", "ST", SECT_H);
	pFrmSec->m_dJewon[0]	= toM(pCr->m_H_CR+pCr->m_T1_CR+pCr->m_T2_CR);	// H
	pFrmSec->m_dJewon[1]	= toM(pCr->m_uM_CR*2);							// B1
	pFrmSec->m_dJewon[2]	= toM(pCr->m_T3_CR);							// tw
	pFrmSec->m_dJewon[3]	= toM(pCr->m_T2_CR);							// tf1
	pFrmSec->m_dJewon[4]	= toM(pCr->m_dM_CR*2);							// B2
	pFrmSec->m_dJewon[5]	= toM(pCr->m_T1_CR);							// tf2
	pFrmSec->m_dJewon[6]	= toM(0.0);										// r1
	pFrmSec->m_dJewon[7]	= toM(0.0);										// r2
	
}

void CFemCantileverCalc::MakeGroupData(CFEMManage *pFEM)
{
	CCalcData	*pData	= m_pDataManage->GetCalcData();
// Structure Group
	for(long nElem=0; nElem<pFEM->GetElementSize(); nElem++)
	{
		CElement	*pElem	= pFEM->GetElement((unsigned short)nElem);
		long	nType	= pElem->m_LongAttrib[ELEM_TYPE];
		switch(nType)
		{
		case ELEM_SLAB :
			pFEM->AddGroupNode("슬래브", pElem->m_nJStt+1);
			pFEM->AddGroupNode("슬래브", pElem->m_nJEnd+1);
			pFEM->AddGroupElem("슬래브", nElem+1);
			break;
		case ELEM_UPPER_FLANGE :
		case ELEM_LOWER_FLANGE :
		case ELEM_WEB :
		case ELEM_CROSS :
			pFEM->AddGroupNode("Girder", pElem->m_nJStt+1);
			pFEM->AddGroupNode("Girder", pElem->m_nJEnd+1);
			pFEM->AddGroupElem("Girder", nElem+1);
			break;
		}
	}
// Load Group
	pFEM->AddLoadGroup("합성전 고정하중");
	pFEM->AddLoadGroup("TENSION");
	pFEM->AddLoadGroup("합성후 고정하중");
	pFEM->AddLoadGroup("활하중");
	if(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm || pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm)
		pFEM->AddLoadGroup("풍하중");
	pFEM->AddLoadGroup("충돌하중");
	if(pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Cent_R>0 || pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Cent_R>0)
		pFEM->AddLoadGroup("원심하중");
	if(pData->m_bTank || pData->m_bTrailer)
		pFEM->AddLoadGroup("특수하중");
// Tendon Group
	pFEM->AddTendonGroup("Tendon");
}

void CFemCantileverCalc::MakeTDMData(CFEMManage *pFEM)
{
	CCalcData	*pData	= m_pDataManage->GetCalcData();

	double	dFck		= tokNPM2(pData->DESIGN_MATERIAL.m_dSigmaCK);
	double	dHum		= 70.0;
	double	dFactorA	= 4.5;
	double	dFactorB	= 0.95;
	pFEM->AddTDMType("크리프&건조수축", "KS", dFck, dHum, 0.275, 1, 1.0);
	pFEM->AddTDMElast("콘크리트강도", "CODE", "Korea Standard", dFck, dFactorA, dFactorB);
	pFEM->AddTDMLink("CONC40", "크리프&건조수축", "콘크리트강도");
}

void CFemCantileverCalc::MakeTDNData(CFEMManage *pFEM, CPlateBasicIndex *pBx)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();

	double	dArea	= toM2(pData->DESIGN_STEELPRESTRESS_DATA.m_dArea);
	double	dDia	= toM(pData->DESIGN_STEELPRESTRESS_DATA.m_dDuctDiameter);
	double	dRM		= 0;
	double	dRC		= 45;
	double	dFF		= pData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction;
	double	dWF		= pData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction;
	double	dUS		= tokNPM2(pData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength);
	double	dYS		= tokNPM2(pData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength);
	double	dABS	= toM(pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	double	dASE	= toM(pData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	double	bBonded	= FALSE;
	pFEM->AddTDNProperty("tendon", CTendonProperty::TYPE_INTERNAL_POST, "강선", dArea, dDia, dRM, dRC, dFF, dWF, dUS, dYS, dABS, dASE, (BOOL)bBonded);

	double	dWidth		= 1000.0;
	double	dCTC		= pData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	long	nElemStt	= 0;
	long	nElemEnd	= 0;
	long	nQtyTDN		= (long)(dWidth/dCTC);
	double	dExt	= (dWidth-(nQtyTDN-1)*dCTC)/2.0;

	GetSlabElemNum(pFEM, nElemStt, nElemEnd);

	for(long nTDN=0; nTDN<nQtyTDN; nTDN++)
	{
		_TDN_PROFILE TDN_PROFILE;
		_TDN_POSITION TDN_POSITION;
		vector<_TDN_POSITION> vTDNPosition;
		TDN_PROFILE.m_szNameProperty	= _T("tendon");
		TDN_PROFILE.m_szNameGroup		= _T("Tendon");
		TDN_PROFILE.m_nArrEleStt.Add(nElemStt+1);
		TDN_PROFILE.m_nArrEleEnd.Add(nElemEnd+1);
		TDN_PROFILE.m_nLengOpt			= CTendonProfile::LENGTH_USER;
		TDN_PROFILE.m_dBegin			= 0;
		TDN_PROFILE.m_dEnd				= 0;
		TDN_PROFILE.m_dBLen				= 0;
		TDN_PROFILE.m_dELen				= 0;
		TDN_PROFILE.m_nCurveType		= CTendonProfile::CURVE_ROUND;
		TDN_PROFILE.m_nInputType		= CTendonProfile::INPUT_3D;
		TDN_PROFILE.m_bTP				= FALSE;
		TDN_PROFILE.m_nTDNNum			= 0;
		TDN_PROFILE.m_nShape			= CTendonProfile::SHAPE_STRAIGHT;
		TDN_PROFILE.m_vIP				= CVector(0,0,0);
		TDN_PROFILE.m_nAxisDir			= CTendonProfile::XDIR_IJ;
		TDN_PROFILE.m_dXRotate			= 0;
		TDN_PROFILE.m_bProjection		= TRUE;
		TDN_PROFILE.m_nGradRot			= CTendonProfile::GRAD_X;
		TDN_PROFILE.m_dGradRot			= 0;

		double	dX		= 0;
		double	dY		= 0;
		double	dZ		= 0;

		if(nTDN==0)	dY	= toM(dWidth/2-dExt);
		else		dY -= toM(dCTC/2);

		TDN_POSITION.m_bFix		= TRUE;
		TDN_POSITION.m_dRy		= 0;
		TDN_POSITION.m_dRz		= 0;
		TDN_POSITION.m_vtPos	= CVector(dX, dY, -toM(pDB->m_SlabTension[0].m_dH));
		TDN_POSITION.m_dRadius	= 0;
		vTDNPosition.push_back(TDN_POSITION);
		for(long nTerm=0; nTerm<pDB->m_nSlabTensionSize; nTerm++)
		{
			dX	+= toM(pDB->m_SlabTension[nTerm].m_dL);
			dZ	= -toM(pDB->m_SlabTension[nTerm].m_dH);
			TDN_POSITION.m_bFix		= FALSE;
			TDN_POSITION.m_dRy		= 0;
			TDN_POSITION.m_dRz		= 0;
			TDN_POSITION.m_vtPos	= CVector(dX, dY, dZ);
			TDN_POSITION.m_dRadius	= toM(pDB->m_SlabTension[nTerm].m_dR);
			vTDNPosition.push_back(TDN_POSITION);
		}
		dX	= toM(pDB->GetWidthAct(pBx->GetStation()));
		TDN_POSITION.m_bFix		= TRUE;
		TDN_POSITION.m_dRy		= 0;
		TDN_POSITION.m_dRz		= 0;
		TDN_POSITION.m_vtPos	= CVector(dX, dY, -toM(pDB->m_SlabTension[pDB->m_nSlabTensionSize-1].m_dH));
		TDN_POSITION.m_dRadius	= 0;
		vTDNPosition.push_back(TDN_POSITION);

		CString szName;
		szName.Format("tendon%d", nTDN+1);
		pFEM->AddTDNProfile(szName, TDN_PROFILE, &vTDNPosition);
	}
}

void CFemCantileverCalc::MakeSlabSelfLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CCalcData	*pData	= m_pDataManage->GetCalcData();
	double	dUnit	= tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dChulgun);

	pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_WEIGHT);
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_WEIGHT);

	for(long nEle=0; nEle<pFEM->GetElementSize(); nEle++)
	{
		CElement	*pElem	= pFEM->GetElement((unsigned short)nEle);
		CLoadForce	*pELoad	= pElem->GetLoadForce(nLoadCaseIndex);
		
		if(pElem->m_LongAttrib[ELEM_TYPE]!=ELEM_SLAB) continue;

		CJoint	*pJ_I	= pFEM->GetJoint(pElem->m_nJStt);
		CJoint	*pJ_J	= pFEM->GetJoint(pElem->m_nJEnd);
		double	dLoad_I	= pJ_I->m_vPointReal.x * dUnit;
		double	dLoad_J	= pJ_J->m_vPointReal.x * dUnit;
		double	dDist_I	= 0.0;
		double	dDist_J	= 1.0;

		pELoad->SetLoadForce(dDist_I, dDist_J, CVector(0,0,-dLoad_I), CVector(0,0,-dLoad_J), CVector(0,0,0), CVector(0,0,0), LOAD_WEIGHT, CVector(0,0,0), TRUE, FALSE, "합성전 고정하중");
	}
	nLoadCaseIndex++;

	pFEM->AddCombo(COMBO_BEFORE, LOAD_WEIGHT, CCombo::ADD, CCombo::LOAD, 1, FALSE);
}

void CFemCantileverCalc::MakePostSelfLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge(); 
	CCalcData		*pData		= m_pDataManage->GetCalcData();
	CSteelUWeight	*pUWeight	= m_pDataManage->GetSteelUWeight();	

	// 방호벽 자중
	BOOL	bFirst		= TRUE;
	long	nGuardIdx	= 0;
	double  dDisStt		= 0;
	double  dDisEnd		= 0;
	double	dUnitWeight	= 0;
	long    nHDanNode   = pDB->GetQtyHDanNode();
	long nH = 0;
	for (nH=0; nH<nHDanNode; nH++)
	{
		long nType = pDB->GetValueTypeHDan(nH, 1);
		if ( nType==HT_NONE || nType==HT_PEOPLEROAD )
		{
			dDisStt  = dDisEnd;
			dDisEnd  = dDisStt + toM(pDB->GetLengthHDanDom(pBx, nH));
			continue;
		}

		CCentSeparation* pGuard = pDB->GetGuardFence(nGuardIdx);

		if (pGuard)
		{
			if     ( nType==HT_GUARDFENCE && pData->m_bMugun  )					dUnitWeight	= pUWeight->m_dUWeightMugunCon;
			else if( nType==HT_GUARDFENCE && !pData->m_bMugun )					dUnitWeight	= pUWeight->m_dUWeightChulgunCon;
			else if( nType==HT_CENTERGUARDFENCE && pData->m_bJungbunMugun )		dUnitWeight	= pUWeight->m_dUWeightMugunCon;
			else if( nType==HT_CENTERGUARDFENCE && !pData->m_bJungbunMugun )	dUnitWeight	= pUWeight->m_dUWeightChulgunCon;

			double dArea	= pGuard->GetArea();
			double dForce	= dArea * dUnitWeight;
			double dDist	= dDisEnd + toM(pGuard->GetDistCentroid());

			if(bFirst)
			{
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_CUBE);
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_CUBE);
				pFEM->AddCombo(COMBO_AFTER, LOAD_CUBE, CCombo::ADD, CCombo::LOAD, 1, FALSE);
				bFirst = FALSE;
			}
			AddPointForceAtLeft(pFEM, dDist, CVector(0,0,-dForce), nLoadCaseIndex, LOAD_CUBE, "합성후 고정하중");
			nGuardIdx++;
		}
		dDisStt  = dDisEnd;
		dDisEnd  = dDisStt + toM(pDB->GetLengthHDanDom(pBx, nH));
	}
	if (!bFirst)
	{
		bFirst = TRUE;
		nLoadCaseIndex++;
	}

	// 방음벽 자중
	// 좌측
	if ( pData->DESIGN_FLOOR_DATA[0].m_bBangEm )
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_SWALL);	
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_SWALL);
		pFEM->AddCombo(COMBO_AFTER, LOAD_SWALL, CCombo::ADD, CCombo::LOAD, 1, FALSE);	
		
		double		dDis    = toM(pData->DESIGN_FLOOR_DATA[0].m_dBangEmDis);	// 재하거리
		long		nElem	= GetElementIndexFromDistance(pFEM, dDis);
		CElement	*pE     = pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pELoad = pE->GetLoadForce(nLoadCaseIndex);
		CJoint		*pJ_1	= pFEM->GetJoint(pE->m_nJStt);
		CJoint		*pJ_2	= pFEM->GetJoint(pE->m_nJEnd);
		double		dDisLen = pJ_2->m_vPoint.x - pJ_1->m_vPoint.x;
		double		dTarget = dDis - pJ_1->m_vPoint.x;
		double		dRatio  = dTarget / dDisLen;		
		double		dForce	= tokN(pData->DESIGN_FLOOR_DATA[0].m_dBangEmWeight);

		pELoad->SetDistForce(dRatio, CVector(0,0,-dForce), LOAD_SWALL, TRUE, "합성후 고정하중");
		if (bFirst)	bFirst = FALSE;
	}

	// 우측
	long nElemApply   = GetLoadApplyMemberCount(pFEM);
	CElement *pEl     = pFEM->GetElement((unsigned short)nElemApply-1);
	CJoint   *pJo	  = pFEM->GetJoint(pEl->m_nJEnd);
	double  dTotalDis = pJo->m_vPoint.x;
	
	if(pData->DESIGN_FLOOR_DATA[1].m_bBangEm)
	{
		if (bFirst)
		{
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_SWALL);	
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_SWALL);
			pFEM->AddCombo(COMBO_AFTER, LOAD_SWALL, CCombo::ADD, CCombo::LOAD, 1, FALSE);
		}

		double	    dDis	= dTotalDis - toM(pData->DESIGN_FLOOR_DATA[1].m_dBangEmDis);
		long		nElem	= GetElementIndexFromDistance(pFEM, dDis);
		CElement	*pE     = pFEM->GetElement((unsigned short)nElem);
		CLoadForce	*pELoad = pE->GetLoadForce(nLoadCaseIndex);		
		CJoint		*pJ_1	= pFEM->GetJoint(pE->m_nJStt);
		CJoint		*pJ_2	= pFEM->GetJoint(pE->m_nJEnd);
		double		dDisLen = pJ_2->m_vPoint.x - pJ_1->m_vPoint.x;
		double		dTarget = dDis - pJ_1->m_vPoint.x;
		double		dRatio  = dTarget / dDisLen;		
		double		dForce  = tokN(pData->DESIGN_FLOOR_DATA[1].m_dBangEmWeight);

		pELoad->SetDistForce(dRatio, CVector(0,0,-dForce), LOAD_SWALL, TRUE, "합성후 고정하중");
		if (bFirst) bFirst = FALSE;
	}
	if (!bFirst)
	{
		bFirst = TRUE;
		nLoadCaseIndex++;
	}

	// 난간 자중
	// 좌측
	if(pData->DESIGN_FLOOR_DATA[0].m_bNanGan)
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_FENCE);	
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_FENCE);
		pFEM->AddCombo(COMBO_AFTER, LOAD_FENCE, CCombo::ADD, CCombo::LOAD, 1, FALSE);

		double		dDist	= toM(pData->DESIGN_FLOOR_DATA[0].m_dNanGanDis);
		long		nElem	= GetElementIndexFromDistance(pFEM, dDist);	
		CElement   *pE		= pFEM->GetElement((unsigned short)nElem);
		CLoadForce *pELoad	= pE->GetLoadForce(nLoadCaseIndex);		
		double		dForce	= tokN(pData->DESIGN_FLOOR_DATA[0].m_dNanGanWeight);
		CJoint		*pJ_1	= pFEM->GetJoint(pE->m_nJStt);
		CJoint		*pJ_2	= pFEM->GetJoint(pE->m_nJEnd);
		double		dDisLen = pJ_2->m_vPoint.x - pJ_1->m_vPoint.x;
		double		dTarget = dDist - pJ_1->m_vPoint.x;
		double		dRatio  = dTarget / dDisLen;

		pELoad->SetDistForce(dRatio, CVector(0,0,-dForce), LOAD_FENCE, TRUE, "합성후 고정하중");
		if (bFirst) bFirst = FALSE;
	}
	// 우측
    if(pData->DESIGN_FLOOR_DATA[1].m_bNanGan)
	{
		if (bFirst)
		{
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_FENCE);	
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_FENCE);
			pFEM->AddCombo(COMBO_AFTER, LOAD_FENCE, CCombo::ADD, CCombo::LOAD, 1, FALSE);
		}
		
		double	    dDist	= dTotalDis - toM(pData->DESIGN_FLOOR_DATA[1].m_dNanGanDis);
		long		nElem	= GetElementIndexFromDistance(pFEM, dDist);
		CElement   *pE		= pFEM->GetElement((unsigned short)nElem);
		CLoadForce *pELoad	= pE->GetLoadForce(nLoadCaseIndex);	
		double		dForce = tokN(pData->DESIGN_FLOOR_DATA[1].m_dNanGanWeight);
		CJoint		*pJ_1	= pFEM->GetJoint(pE->m_nJStt);
		CJoint		*pJ_2	= pFEM->GetJoint(pE->m_nJEnd);
		double		dDisLen = pJ_2->m_vPoint.x - pJ_1->m_vPoint.x;
		double		dTarget = dDist - pJ_1->m_vPoint.x;
		double		dRatio  = dTarget / dDisLen;
		
		pELoad->SetDistForce(dRatio, CVector(0,0,-dForce), LOAD_FENCE, TRUE, "합성후 고정하중");
		if (bFirst) bFirst = FALSE;
	}
	if (!bFirst)
	{
		bFirst = TRUE;
		nLoadCaseIndex++;
	}

	dDisStt = 0;
	dDisEnd = 0;
	// 포장하중
	for (nH=0; nH<nHDanNode; nH++)
	{
		long nType    = pDB->GetValueTypeHDan(nH, 1);
		
		dDisEnd  += toM(pDB->GetLengthHDanDom(pBx, nH));
		
		if ( nType==HT_PEOPLEROAD || nType==HT_GUARDFENCE || nType==HT_CENTERGUARDFENCE)
		{
			dDisStt  = dDisEnd;
			continue;
		}
		if( nH<nHDanNode-1 && nType==HT_NONE && pDB->GetValueTypeHDan(nH+1, 1)==HT_NONE)
			continue;

		if (bFirst)
		{
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_PAVE);	
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_PAVE);
			pFEM->AddCombo(COMBO_AFTER, LOAD_PAVE, CCombo::ADD, CCombo::LOAD, 1, FALSE);
			bFirst = FALSE;
		}

		double	dUnit	= tokNPM3(pData->DESIGN_UNIT_WEIGHT.m_dAsphalt);
		double	dForce	= toM(pDB->m_dThickPave) * dUnit;

		AddTrapForceAtLeft(pFEM, dDisStt, dDisEnd, -dForce, -dForce, nLoadCaseIndex, LOAD_PAVE, "합성후 고정하중", TRUE);
		dDisStt  = dDisEnd;
	}
	if (!bFirst)
	{
		bFirst = TRUE;
		nLoadCaseIndex++;
	}

	// 보도하중
	CDoubleArray DisHDan;
	for (nH=0; nH<nHDanNode; nH++)
	{
		double dDist = pDB->GetLengthHDanDom(pBx, nH);
		if (nH!=0) dDist += DisHDan.GetAt(nH-1);
		DisHDan.Add(dDist);
	}


	for (nH=0; nH<nHDanNode; nH++)
	{
		long nType = pDB->GetValueTypeHDan(nH, 1);
		dDisStt  = (nH==0)? 0 : toM(DisHDan.GetAt(nH-1));
		dDisEnd  = toM(DisHDan.GetAt(nH));

		if (nType!=HT_PEOPLEROAD) continue;

		double dForceWalkLoad1 = 0;
		double dForceWalkLoad2 = 0;
		if(IsLeft(pBx, dDisEnd))
		{
			dForceWalkLoad1 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1);
			dForceWalkLoad2 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad2);
		}
		else
		{
			dForceWalkLoad1 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1);
			dForceWalkLoad2 = tokNPM2(pData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad2);
		}
		if(bFirst)
		{
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, LOAD_WALK);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, LOAD_WALK);
			pFEM->AddCombo(COMBO_AFTER, LOAD_WALK, CCombo::ADD, CCombo::LOAD, 1, FALSE);
			bFirst = FALSE;
		}
		AddTrapForceAtLeft(pFEM, dDisStt, dDisEnd, -dForceWalkLoad1, -dForceWalkLoad2, nLoadCaseIndex, LOAD_WALK, "합성후 고정하중", TRUE);	
	}
	if (!bFirst)
	{
		bFirst = TRUE;
		nLoadCaseIndex++;
	}
}

void CFemCantileverCalc::MakeLiveLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();

	long	nG			= 0;
	long	nQtyLane	= 0;
	long	nHDanType	= 0;
	long	nHDanNode	= pDB->GetQtyHDanNode();
	long	nLaneCount	= 1;
	BOOL	bStt		= FALSE;
	BOOL	bInsertPre	= FALSE;
	double	dSttHDan	= 0;
	double	dEndHDan	= 0;
	double	dSttRoadway	= 0;
	double	dEndRoadway	= 0;
	double	dEffWidth	= 0;
	double	dLenHDanDom	= 0;
	CString	szName		= _T("DB");

	pData->m_DBWidth	= 1.8;               // DB 차폭
	pData->m_DBDis		= 1.2;               // DB 차간   거리
	pData->m_DLDis		= 3.0;               // DL 차선간 거리

	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
	CCalcGeneral		CalGeneral(m_pDataManage);
	CHLLLane HLLLane;
	HLLLane.m_vLaneUnit.clear();

	dSttHDan	= 0;
	dEndHDan	= 0;
	dSttRoadway	= 0;
	dEndRoadway	= 0;
	nLaneCount	= 1;
	bInsertPre	= FALSE;

	long nLaneTemp = 0;
	long nHDan = 0;
	for (nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBxMatch, nHDan));
		dEndHDan	+= dLenHDanDom;

		if (!bStt && nHDanType==HT_NONE)
		{
			dSttRoadway	= dSttHDan;
			bStt		= TRUE;
		}

		long nInsCount  = 0;
		if (bStt && nHDanType != HT_NONE)
		{
			dEndRoadway	= dEndHDan-dLenHDanDom;
			dEffWidth	= fabs(dEndRoadway-dSttRoadway);
			nQtyLane	= CalGeneral.GetQtyLane(dEffWidth);
			bStt		= FALSE;

			for(long nLane=0; nLane<nQtyLane; nLane++)
			{
				CHLLLaneUnit HLLLaneUnit;
				double	dFactor = (nLaneCount<3)? 1 : (nLaneCount<4)? 0.9 : 0.75;

				HLLLaneUnit.m_nLaneType	= 0;
				HLLLaneUnit.m_szLCName.Format("%sL-%d", szName, nLaneCount);
				HLLLaneUnit.m_szDescription.Format("%sL %d", szName, nLaneCount++);
				
				pFEM->m_szLCName.Add(HLLLaneUnit.m_szLCName);
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->AddCombo(COMBO_LIVE, HLLLaneUnit.m_szLCName, CCombo::ENEV, CCombo::LOAD, dFactor, FALSE);
				pFEM->AddCombo(COMBO_DB, HLLLaneUnit.m_szLCName, CCombo::ENEV, CCombo::LOAD, dFactor, FALSE);
				
				if( SetDBLoad(pFEM, pBx, nG, dSttRoadway, dEndRoadway, nLoadCaseIndex, &HLLLaneUnit, nLane+1, HLLLaneUnit.m_szLCName, "활하중") )
				{
					if(bInsertPre)
						InsertPreLane(pFEM, &HLLLane, &HLLLaneUnit, nLaneTemp-1, nLoadCaseIndex, HLLLaneUnit.m_szLCName, "활하중");
					HLLLane.m_vLaneUnit.push_back(HLLLaneUnit);
					
					nLoadCaseIndex++;
					nInsCount++;
				}
			}
			bInsertPre	= TRUE;
			nLaneTemp	+= nInsCount;
		}
		dSttHDan = dEndHDan;
	}

	HLLLane.m_vLaneUnit.clear();
	dSttHDan	= 0;
	dEndHDan	= 0;
	dSttRoadway	= 0;
	dEndRoadway	= 0;
	nLaneCount	= 1;
	bInsertPre	= FALSE;
	nLaneTemp	= 0;
	nG			= pDB->GetQtyGirder()-1;
	
	pGir		= pDB->GetGirder(nG);
	pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
	
	double	dSlabWidth	= pDB->GetWidthSlabDom(pBxMatch);
	dSttHDan	= toM(dSlabWidth);
	dEndHDan	= toM(dSlabWidth);
	bStt = FALSE;
	
	for(nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDanNode-nHDan-1, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBxMatch, nHDanNode-nHDan-1));
		dSttHDan	-= dLenHDanDom;
		dSttHDan = Round(dSttHDan,6);
		if(!bStt && nHDanType==HT_NONE)
		{
			dEndRoadway	= dEndHDan;
			bStt		= TRUE;
		}
		
		long nInsCount  = 0;
		if((bStt && nHDanType!=HT_NONE) || (nHDan==nHDanNode-1 && nHDanType == HT_NONE && bStt))
		{
			dSttRoadway	= dEndHDan;
			dEffWidth	= fabs(dEndRoadway-dSttRoadway);
			nQtyLane	= CalGeneral.GetQtyLane(dEffWidth);
			bStt		= FALSE;
			
			for(long nLane=0; nLane<nQtyLane; nLane++)
			{
				CHLLLaneUnit HLLLaneUnit;
				double	dFactor = (nLaneCount<3)? 1 : (nLaneCount<4)? 0.9 : 0.75;

				HLLLaneUnit.m_nLaneType	= 0;
				HLLLaneUnit.m_szLCName.Format("%sR-%d", szName, nLaneCount);
				HLLLaneUnit.m_szDescription.Format("%sR %d", szName, nLaneCount++);
				
				pFEM->m_szLCName.Add(HLLLaneUnit.m_szLCName);
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->AddCombo(COMBO_LIVE, HLLLaneUnit.m_szLCName, CCombo::ENEV, CCombo::LOAD, dFactor, FALSE);
				pFEM->AddCombo(COMBO_DB, HLLLaneUnit.m_szLCName, CCombo::ENEV, CCombo::LOAD, dFactor, FALSE);
				
				if( SetDBLoad(pFEM, pBx, nG, dSttRoadway, dEndRoadway, nLoadCaseIndex, &HLLLaneUnit, nLane+1, HLLLaneUnit.m_szLCName, "활하중") )
				{
					if(bInsertPre)
						InsertPreLane(pFEM, &HLLLane, &HLLLaneUnit, nLaneTemp-1, nLoadCaseIndex, HLLLaneUnit.m_szLCName, "활하중");
					HLLLane.m_vLaneUnit.push_back(HLLLaneUnit);
					nLoadCaseIndex++;
					nInsCount++;
				}
			}
			bInsertPre	= TRUE;
			nLaneTemp	+= nInsCount;
		}
		dEndHDan = dSttHDan;
	}
}

void CFemCantileverCalc::MakePeopleLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	
	//군중하중
	long	nIdx = 0;
	long	nType= 0;
	long	nHDanNode = pDB->GetQtyHDanNode();
	double	dSumDis=0, dSttGugan=0, dEndGugan=0, dDisStt=0;	
	CString szName("");
	
	for(long nHdan=0; nHdan<nHDanNode; nHdan++)
	{
		nType	= pDB->GetValueTypeHDan(nHdan, 1);
		dDisStt	= toM(pDB->GetLengthHDanDom(pBx, nHdan));
		dSumDis	+= dDisStt;
		dSttGugan = dSumDis-dDisStt;
		dEndGugan = dSumDis;
		
		if(nType==HT_PEOPLEROAD) //보도
		{
			double dL = 3.5;

			szName= (nHdan<=nHDanNode/2)? _T(LOAD_MAN_L) :  _T(LOAD_MAN_R);

			pFEM->m_szLCName.Add(szName);
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			
			AddTrapForceAtLeft(pFEM, dSttGugan, dEndGugan, -dL , -dL, nLoadCaseIndex, szName, "활하중", TRUE);
			nLoadCaseIndex++;
			nIdx++;
		}
	}
}

void CFemCantileverCalc::MakeTankLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();
	
	if(!pData->m_bTank)	return;
	
	CString	szLoadCaseTitle	= _T("");
	CString	szTitle			= _T("");
	double	dDisL			= 0;
	double	dDisR			= 0;
	double	dTankWidth		= pData->m_TankWidth;
	double	dLenHDanDom		= 0;
	double	dSttHDan		= 0;
	double	dEndHDan		= 0;
	double	dSttRoadway		= 0;
	double	dEndRoadway		= 0;
	long	nHDanType		= 0;
	long	nIdx			= 1;
	BOOL	bStt			= FALSE;

	//탱크하중
	for(long nHDan=0; nHDan<pDB->GetQtyHDanNode(); nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBx, nHDan));
		dEndHDan	+= dLenHDanDom;
		if(!bStt && nHDanType==HT_NONE)
		{
			dSttRoadway	= dSttHDan;
			bStt		= TRUE;
		}


		// seek load
		double dLoad[3] = { 0, 0, 0 };
		double dDis[2] = { 0 ,0 };
		CalcCarLoad(dLoad, dDis);

		if(bStt && nHDanType!=HT_NONE)
		{
			dEndRoadway	= dEndHDan-dLenHDanDom;
			bStt		= FALSE;
			if(dEndRoadway-dSttRoadway>dTankWidth)
			{
				dDisL		= (dSttRoadway+dEndRoadway)/2 - dTankWidth/2;
				dDisR		= (dSttRoadway+dEndRoadway)/2 + dTankWidth/2;
				
				szLoadCaseTitle.Format("TANK-%d", nIdx++);
				pFEM->m_szLCName.Add(szLoadCaseTitle);
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				
				double dValue[2] = { 0, 0 };
				dValue[0] = (dDisL<dDis[0])? dLoad[0] : (dDisL<dDis[1])? dLoad[1] : dLoad[2];
				dValue[1] = (dDisR<dDis[0])? dLoad[0] : (dDisR<dDis[1])? dLoad[1] : dLoad[2];
				
				
				AddUnitPointForceAtLeft(pFEM, dDisL, nLoadCaseIndex, szLoadCaseTitle, "특수하중", dValue[0]);
				AddUnitPointForceAtLeft(pFEM, dDisR, nLoadCaseIndex, szLoadCaseTitle, "특수하중", dValue[1]);
				pFEM->AddCombo(COMBO_LIVE, szLoadCaseTitle, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
				pFEM->AddCombo(COMBO_TANK, szLoadCaseTitle, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
				nLoadCaseIndex++;
			}
		}
		dSttHDan = dEndHDan;
	}
}

void CFemCantileverCalc::MakeTrailerLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();
	
	if(!pData->m_bTrailer)	return;
	
	CString	szLoadCaseTitle	= _T("");
	CString	szTitle			= _T("");
	double	dDisL			= 0;
	double	dDisR			= 0;
	double	dTrailerWidth	= pData->m_TrailWidth;
	double	dLenHDanDom		= 0;
	double	dSttHDan		= 0;
	double	dEndHDan		= 0;
	double	dSttRoadway		= 0;
	double	dEndRoadway		= 0;
	long	nHDanType		= 0;
	long	nIndex			= 1;
	BOOL	bStt			= FALSE;

	// seek load
	double dLoad[3] = { 0, 0, 0 };
	double dDis[2] = { 0 ,0 };
	CalcCarLoad(dLoad, dDis);

	for(long nHDan=0; nHDan<pDB->GetQtyHDanNode(); nHDan++)
	{
		nHDanType	= pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDanDom	= toM(pDB->GetLengthHDanDom(pBx, nHDan));
		dEndHDan	+= dLenHDanDom;
		if(!bStt && nHDanType==HT_NONE)
		{
			dSttRoadway	= dSttHDan;
			bStt		= TRUE;
		}
		if(bStt && nHDanType!=HT_NONE)
		{
			dEndRoadway	= dEndHDan-dLenHDanDom;
			bStt		= FALSE;
			dDisL		= (dSttRoadway+dEndRoadway)/2 - dTrailerWidth/2;
			dDisR		= (dSttRoadway+dEndRoadway)/2 + dTrailerWidth/2;

			double dValue[2] = { 0, 0 };
			dValue[0] = (dDisL<dDis[0])? dLoad[0] : (dDisL<dDis[1])? dLoad[1] : dLoad[2];
			dValue[1] = (dDisR<dDis[0])? dLoad[0] : (dDisR<dDis[1])? dLoad[1] : dLoad[2];

			if(dEndRoadway-dSttRoadway>dTrailerWidth)
			{
				szLoadCaseTitle.Format("TRAILER-%d", nIndex++);
				pFEM->m_szLCName.Add(szLoadCaseTitle);
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
								
				AddUnitPointForceAtLeft(pFEM, dDisL, nLoadCaseIndex, szLoadCaseTitle, "특수하중", dValue[0]);
				AddUnitPointForceAtLeft(pFEM, dDisR, nLoadCaseIndex, szLoadCaseTitle, "특수하중", dValue[1]);
				pFEM->AddCombo(COMBO_LIVE, szLoadCaseTitle, CCombo::ENEV, CCombo::LOAD, 1, FALSE);				
				pFEM->AddCombo(COMBO_TRAILER, szLoadCaseTitle, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
				nLoadCaseIndex++;
			}
		}
		dSttHDan = dEndHDan;
	}
}

void CFemCantileverCalc::MakeWindLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CCalcData			*pData	= m_pDataManage->GetCalcData();
	
	SetWindLoadForce(0);
	
	long	nElem		= GetLoadApplyMemberCount(pFEM);
	CString szLoadCaseTitle="";
	CJoint *pJointStt = pFEM->GetJoint(0);
	CJoint *pJointEnd = pFEM->GetJoint((unsigned short)nElem);
	BOOL bBangEmL = pData->DESIGN_FLOOR_DATA[0].m_bBangEm;
	BOOL bBangEmR = pData->DESIGN_FLOOR_DATA[1].m_bBangEm;
	
	if(bBangEmL && !bBangEmR) ///< 좌측방음벽망
	{
		pFEM->m_szLCName.Add(LOAD_WIND_LL);	
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

		CLoadForceJoint	*pLoadForceJoint1	= pJointStt->GetLoadForce(nLoadCaseIndex);
		pLoadForceJoint1->SetLoadForce(LOAD_WIND_LL, CVector(pData->m_dLeftWindFroceFromL[0], 0,0), CVector(0,-pData->m_dLeftWindMomentFromL[0],0), CVector(0,0,0), "풍하중");
		nLoadCaseIndex++;
		
		pFEM->m_szLCName.Add(LOAD_WIND_LR);	
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		CLoadForceJoint	*pLoadForceJoint2	= pJointStt->GetLoadForce(nLoadCaseIndex);
		pLoadForceJoint2->SetLoadForce(LOAD_WIND_LR, CVector(pData->m_dLeftWindFroceFromR[0], 0,0), CVector(0,pData->m_dLeftWindMomentFromR[0],0), CVector(0,0,0), "풍하중");
		nLoadCaseIndex++;

	}
	else if(!bBangEmL && bBangEmR) ///< 우측방음벽망
	{
		pFEM->m_szLCName.Add(LOAD_WIND_RR);	
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		
		CLoadForceJoint *pLoadForceJoint1	= pJointEnd->GetLoadForce(nLoadCaseIndex);
		pLoadForceJoint1->SetLoadForce(LOAD_WIND_RR, CVector(pData->m_dRightWindFroceFromR[0], 0, 0), CVector(0, -pData->m_dRightWindMomentFromR[0], 0), CVector(0,0,0), "풍하중");
		nLoadCaseIndex++;
		
		pFEM->m_szLCName.Add(LOAD_WIND_RL);	
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		CLoadForceJoint *pLoadForceJoint2 = pJointEnd->GetLoadForce(nLoadCaseIndex);
		pLoadForceJoint2->SetLoadForce(LOAD_WIND_RL, CVector(pData->m_dRightWindFroceFromL[0], 0, 0), CVector(0, pData->m_dRightWindMomentFromL[0], 0), CVector(0,0,0), "풍하중");
		nLoadCaseIndex++;
	}
	else if(bBangEmL && bBangEmR) ///< 양쪽 다
	{
		pFEM->m_szLCName.Add(LOAD_WIND_LL);	
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->AddCombo(COMBO_WIND, LOAD_WIND_LL, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		
		CLoadForceJoint *pLoadForceJoint1 = pJointStt->GetLoadForce(nLoadCaseIndex);
		pLoadForceJoint1->SetLoadForce(LOAD_WIND_LL, CVector(pData->m_dLeftWindFroceFromL[0], 0, 0), CVector(0, -pData->m_dLeftWindMomentFromL[0], 0), CVector(0,0,0), "풍하중");
		nLoadCaseIndex++;
		
		pFEM->m_szLCName.Add(LOAD_WIND_RR);	
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->AddCombo(COMBO_WIND, LOAD_WIND_RR, CCombo::ENEV, CCombo::LOAD, 1, FALSE);

		CLoadForceJoint *pLoadForceJoint2 = pJointEnd->GetLoadForce(nLoadCaseIndex);
		pLoadForceJoint2->SetLoadForce(LOAD_WIND_RR, CVector(pData->m_dRightWindFroceFromR[0], 0, 0), CVector(0, -pData->m_dRightWindMomentFromR[0], 0), CVector(0,0,0), "풍하중");
		nLoadCaseIndex++;
	}
}

void CFemCantileverCalc::MakeCrashLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CCalcData		*pData		= m_pDataManage->GetCalcData();
	
	// 방호벽
	CCentSeparation *pGuardL = m_pCalcStd->GetGuardWall(TRUE);
	CCentSeparation *pGuardR = m_pCalcStd->GetGuardWall(FALSE);

	//for (long i=0; i<pDB->GetGuardFencesu(); i++)
	//{
	//	CCentSeparation* pG = pDB->GetGuardFence(i);
	//	
	//	if (pG->m_strName == _T("방호벽(좌)")) pGuardL = pG;
	//	if (pG->m_strName == _T("방호벽(우)")) pGuardR = pG;
	//}

	// 하중위치산정
	long	nElem		= GetLoadApplyMemberCount(pFEM);

	double dR	  = 0;
	double dSpeed = 0;
	double dH	  = 0;
	double dh1	  = 0;
	double dh2	  = 0;
	double dE	  = 0;
	double dM	  = 0;
	double dV	= 0;

	// 좌측에 난간이 있을경우
	if (pData->DESIGN_FLOOR_DATA[0].m_bNanGan)
	{
 		dH = -2.5;
		dV = -1;
		dE = pData->DESIGN_FLOOR_DATA[0].m_dNanGanHeight;
		dE +=  (pGuardL)? pGuardL->GetHeight() : 0;
		dE = toM(dE);
		dM = dE*dH;

		CJoint*		pJoint	= pFEM->GetJoint(0);
		
		pFEM->m_szLCName.Add(LOAD_CRASH_L);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->AddCombo(COMBO_CRASH, LOAD_CRASH_L, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		
		CLoadForceJoint* pLoadJoint = pJoint->GetLoadForce(nLoadCaseIndex);
		pLoadJoint->SetLoadForce( LOAD_CRASH_L, CVector(-dH, -dV, 0), CVector(0, -dM, 0), CVector(0, 0, 0), "충돌하중");
		nLoadCaseIndex++;
	}
	// 좌측에 난간이 없을 경우
	else if (pGuardL)
	{
		dR	  = GetRadius(FLOOR_LEFT);
		dSpeed = pData->m_dSpeed;
		dH	  = (dR<=200)? max(dSpeed/60*dSpeed/60*7.5 + 2.5, 10)*2 : max(dSpeed/60*dSpeed/60*7.5+2.5, 10);
		dh1	  = toM(pDB->m_dThickPave);
		dh2	  = toM(pGuardL->GetHeight()-pDB->m_dThickPave);
		dE	  = dh1 + dh2;
		dM	  = dH * dE;

		CJoint*		pJoint	= pFEM->GetJoint(0);
		
		pFEM->m_szLCName.Add(LOAD_CRASH_L);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->AddCombo(COMBO_CRASH, LOAD_CRASH_L, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		
		CLoadForceJoint* pLoadJoint = pJoint->GetLoadForce(nLoadCaseIndex);
		pLoadJoint->SetLoadForce( LOAD_CRASH_L, CVector(-dH, 0, 0), CVector(0, -dM, 0), CVector(0, 0, 0), "충돌하중");		
		nLoadCaseIndex++;
	}

	// 우측에 난간이 있을경우
	if (pData->DESIGN_FLOOR_DATA[1].m_bNanGan)
	{
		dH = 2.5;
		dV = -1;
		dE = pData->DESIGN_FLOOR_DATA[1].m_dNanGanHeight;
		dE +=  (pGuardR)? pGuardR->GetHeight() : 0;
		dE = toM(dE);
		dM = dE*dH;
		
		CElement*	pElem	= pFEM->GetElement((unsigned short)nElem-1);
		CJoint*		pJoint	= pFEM->GetJoint(pElem->m_nJEnd);
		
		pFEM->m_szLCName.Add(LOAD_CRASH_R);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->AddCombo(COMBO_CRASH, LOAD_CRASH_R, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		
		CLoadForceJoint* pLoadJoint = pJoint->GetLoadForce(nLoadCaseIndex);
		pLoadJoint->SetLoadForce( LOAD_CRASH_R, CVector(dH, dV, 0), CVector(0, dM, 0), CVector(0, 0, 0), "충돌하중" );
		nLoadCaseIndex++;
	}
	else if (pGuardR)
	{
		dR	  = GetRadius(FLOOR_RIGHT);
		dSpeed = pData->m_dSpeed;
		dH	  = (dR<=200)? max(dSpeed/60*dSpeed/60*7.5 + 2.5, 10)*2 : max(dSpeed/60*dSpeed/60*7.5+2.5, 10);
		dh1	  = toM(pDB->m_dThickPave);
		dh2	  = toM(pGuardR->GetHeight()-pDB->m_dThickPave);
		dE	  = dh1 + dh2;
		dM	  = -dH * dE;
		
		CElement*	pElem	= pFEM->GetElement((unsigned short)nElem-1);
		CJoint*		pJoint	= pFEM->GetJoint(pElem->m_nJEnd);
		
		pFEM->m_szLCName.Add(LOAD_CRASH_R);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->AddCombo(COMBO_CRASH, LOAD_CRASH_R, CCombo::ENEV, CCombo::LOAD, 1, FALSE);
		
		CLoadForceJoint* pLoadJoint = pJoint->GetLoadForce(nLoadCaseIndex);
		pLoadJoint->SetLoadForce( LOAD_CRASH_R, CVector(dH, 0, 0), CVector(0, -dM, 0), CVector(0, 0, 0), "충돌하중" );		
		nLoadCaseIndex++;
	}
}

void CFemCantileverCalc::MakeCentrifugalLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long &nLoadCaseIndex)
{
	double dForce[3]	= { 0, 0, 0 };
	double dMoment[3]	= { 0, 0, 0 };
	double dDist[3]		= { 0, 0, 0 };

	GetUnitCentrifugalLoad(dForce, dDist);
	long n = 0;
	for (n=0; n<3; n++)
	{
		dForce[n] *= 2*0.158;
		dMoment[n] = dForce[n]*1.8;
	}

	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CCalcData		*pData	= m_pDataManage->GetCalcData();
	
	long	nG			= 0;
	long	nQtyLane	= 0;
	long	nHDanType	= 0;
	long	nHDanNode	= pDB->GetQtyHDanNode();
	long	nLaneCount	= 1;
	BOOL	bStt		= FALSE;
	BOOL	bInsertPre	= FALSE;
	double	dSttHDan	= 0;
	double	dEndHDan	= 0;
	double	dSttRoadway	= 0;
	double	dEndRoadway	= 0;
	double	dLenHDan	= 0;
	double	dEffWidth	= 0;
	CString	szName		= _T("DB");
	
	pData->m_DBWidth	= 1.8;               // DB 차폭
	pData->m_DBDis		= 1.2;               // DB 차간   거리
	pData->m_DLDis		= 3.0;               // DL 차선간 거리
	
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
	CCalcGeneral		CalGeneral(m_pDataManage);
	CHLLLane HLLLane;
	
	dSttHDan	= 0;
	dEndHDan	= 0;
	dSttRoadway	= 0;
	dEndRoadway	= 0;
	nLaneCount	= 1;
	bInsertPre	= FALSE;

	// 원심하중이 원쪽에 있는지 오른쪽에 있는지 검토
	CLineInfo	*pLine = m_pDataManage->GetLineInfoApp()->GetLine(0);
	BOOL bLeftCentri  = TRUE;
	BOOL bCheck = FALSE; 
	long nIPs = pLine->GetQtyIp();
	
	CDoubleArray xDis;
	CDoubleArray yDis;
	CDoubleArray dRad;

	xDis.Add(pLine->m_xyBp.y);
	yDis.Add(pLine->m_xyBp.x);
	dRad.Add(0);

	for (n=0; n<nIPs; n++)
	{
		CIPRecord* pIp = &pLine->m_pIpRec[n];
		xDis.Add(pIp->m_xy_IpRecord.y);
		yDis.Add(pIp->m_xy_IpRecord.x);
		dRad.Add(pIp->m_dR_IpRecord);
	}
	xDis.Add(pLine->m_xyEp.x);
	yDis.Add(pLine->m_xyEp.y);
	dRad.Add(0);

	for (n=0; n<xDis.GetSize(); n++)
	{
		double x1 = xDis.GetAt(n);
		double y1 = yDis.GetAt(n);
		double r1 = dRad.GetAt(n);

		if (r1<EPSILON) continue;
		if (bCheck) continue;

		double x0 = xDis.GetAt(n-1);
		double y0 = yDis.GetAt(n-1);		
		double x2 = xDis.GetAt(n+1);
		double y2 = yDis.GetAt(n+1);	
		
		CDPoint P_1(x1-x0, y1-y0);
		CDPoint P_2(x2-x0, y2-y0);
		P_1 = P_1.Unit();
		P_2 = P_2.Unit();

		double dCos = P_1*P_2;
		if (dCos>0) bLeftCentri = FALSE;		
		bCheck = TRUE;
	}

	if (!bLeftCentri)
	{
		for (long i=0; i<3; i++)
		{
			dForce[i] *= -1;
			dMoment[i] *= -1;
		}
	}

	long nLaneTemp = 0;
	long nHDan = 0;
	for (nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		if (pData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Cent_R<1) continue;
		nHDanType = pDB->GetValueTypeHDan(nHDan, 1);
		dLenHDan  = toM(pDB->GetLengthHDanDom(pBxMatch, nHDan));
		dEndHDan += dLenHDan;
		
		if (!bStt && nHDanType==HT_NONE)
		{
			dSttRoadway	= dSttHDan;
			bStt		= TRUE;
		}
		
		long nInsCount  = 0;
		if (bStt && nHDanType != HT_NONE)
		{
			dEndRoadway	= dEndHDan-dLenHDan;
			dEffWidth	= fabs(dEndRoadway-dSttRoadway);
			nQtyLane	= CalGeneral.GetQtyLane(dEffWidth);
			bStt		= FALSE;
			
			for(long nLane=0; nLane<nQtyLane; nLane++)
			{
				CHLLLaneUnit HLLLaneUnit;
				double	dFactor = (nLane<2)? 1 : (nLane<3)? 0.9 : 0.75;

				HLLLaneUnit.m_nLaneType	= 0;
				HLLLaneUnit.m_szLCName.Format("LCF-%d", nLaneCount);
				HLLLaneUnit.m_szDescription.Format("%sL %d", szName, nLaneCount++);
				
				pFEM->m_szLCName.Add(HLLLaneUnit.m_szLCName);
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->AddCombo(COMBO_CENTRI, HLLLaneUnit.m_szLCName, CCombo::ENEV, CCombo::LOAD, dFactor, FALSE);

				if( SetCentrifugalLoad(pFEM, pBx, nG, dSttRoadway, dEndRoadway, nLoadCaseIndex, &HLLLaneUnit, nLane+1, dDist, dForce, dMoment, HLLLaneUnit.m_szLCName, "원심하중") )
				{
					if(bInsertPre)
						InsertPreLaneCentri(pFEM, &HLLLane, &HLLLaneUnit, nLaneTemp-1, nLoadCaseIndex, dDist, dForce, dMoment, HLLLaneUnit.m_szLCName, "원심하중");
					HLLLane.m_vLaneUnit.push_back(HLLLaneUnit);					
					nLoadCaseIndex++;
					nInsCount++;
				}
			}		
			bInsertPre	= TRUE;
			nLaneTemp	+= nInsCount;
		}
		dSttHDan = dEndHDan;
	}

	HLLLane.m_vLaneUnit.clear();
	dSttHDan	= 0;
	dEndHDan	= 0;
	dSttRoadway	= 0;
	dEndRoadway	= 0;
	nLaneCount	= 1;
	bInsertPre	= FALSE;
	nLaneTemp	= 0;
	nG			= pDB->GetQtyGirder()-1;
	
	pGir		= pDB->GetGirder(nG);
	pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
	
	double	dSlabWidth	= pDB->GetWidthSlabDom(pBxMatch);
	dSttHDan	= toM(dSlabWidth);
	dEndHDan	= toM(dSlabWidth);
		
	for(nHDan=0; nHDan<nHDanNode; nHDan++)
	{
		if(pData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Cent_R<1) continue;
		nHDanType	= pDB->GetValueTypeHDan(nHDanNode-nHDan-1, 1);
		dLenHDan	= toM(pDB->GetLengthHDanDom(pBxMatch, nHDanNode-nHDan-1));
		dSttHDan	-= dLenHDan;
		if(!bStt && nHDanType==HT_NONE)
		{
			dEndRoadway	= dEndHDan;
			bStt		= TRUE;
		}
		
		long nInsCount  = 0;
		if(bStt && nHDanType!=HT_NONE)
		{
			dSttRoadway	= dEndHDan;
			dEffWidth	= fabs(dEndRoadway-dSttRoadway);
			nQtyLane	= CalGeneral.GetQtyLane(dEffWidth);
			bStt		= FALSE;
			
			for(long nLane=0; nLane<nQtyLane; nLane++)
			{
				CHLLLaneUnit HLLLaneUnit;
				double	dFactor = (nLane<2)? 1 : (nLane<3)? 0.9 : 0.75;

				HLLLaneUnit.m_nLaneType	= 0;
				HLLLaneUnit.m_szLCName.Format("RCF-%d", nLaneCount);
				HLLLaneUnit.m_szDescription.Format("%sR %d", szName, nLaneCount++);
				
				pFEM->m_szLCName.Add(HLLLaneUnit.m_szLCName);
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->AddCombo(COMBO_CENTRI, HLLLaneUnit.m_szLCName, CCombo::ENEV, CCombo::LOAD, dFactor, FALSE);
				
				if( SetCentrifugalLoad(pFEM, pBx, nG, dSttRoadway, dEndRoadway, nLoadCaseIndex, &HLLLaneUnit, 
					nLane+1, dDist, dForce, dMoment, HLLLaneUnit.m_szLCName, "원심하중") )
				{					
					if(bInsertPre)
						InsertPreLaneCentri(pFEM, &HLLLane, &HLLLaneUnit, nLaneTemp-1, nLoadCaseIndex, dDist, dForce, dMoment, HLLLaneUnit.m_szLCName, "원심하중");
					HLLLane.m_vLaneUnit.push_back(HLLLaneUnit);
					nLoadCaseIndex++;
					nInsCount++;
				}
			}
			bInsertPre	= TRUE;
			nLaneTemp	+= nInsCount;
		}
		dEndHDan = dSttHDan;
	}
}

void CFemCantileverCalc::MakeTendenLoad(CFEMManage *pFEM)
{
	long nTDNCnt = pFEM->m_TDNProfileList.GetCount();

	CCalcData *pCalcData = m_pDataManage->GetCalcData();

	CString szVal = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax;
	double	dJack = pCalcData->GetJackMaxStrToDouble(szVal);


	for (long n=0; n<nTDNCnt; n++)
	{
		POSITION pos = pFEM->m_TDNProfileList.FindIndex(n);
		CTendonProperty *pTDN	= (CTendonProperty*)pFEM->m_TDNPropertyList.GetAt(pos);

 		CString szTDNName		= pTDN->m_szName;
		long	nForceStress	= CTendonPreStress::STRESS;
		long	nJacking		= (n==0) ? CTendonPreStress::END : CTendonPreStress::BEGIN;
		double	dBegin			= dJack/1000 * pow( 10.0, 6 );				// 2011.02.18 # 0023067 
//		double	dBegin			= 1.44 * pow( 10.0, 6 );
		double	dEnd			= 0;
		long	nGrouting		= 0;
		CString	szGroup			= _T("TENSION");

		pFEM->AddTDNPrestress(szTDNName, nForceStress, nJacking, dBegin, dEnd, nGrouting, szGroup);
	}
}

void CFemCantileverCalc::MakeContructionStage(CFEMManage *pFEM)
{
	CStage	*pStage = pFEM->AddStage("LAST", 700, TRUE, TRUE);
	pStage->AddStep(7);
	pStage->AddStep(28);
	pStage->AddStep(90);
	pStage->AddStep(365);
	pStage->AddElemActivation("슬래브", 7);
	pStage->AddElemActivation("Girder", 0);
	pStage->AddBoundaryActivation("지점", "ORIGINAL");
	pStage->AddLoadActivation("합성전 고정하중", 7);
	pStage->AddLoadActivation("TENSION", 28);
	pStage->AddLoadActivation("합성후 고정하중", 90);
	pStage->AddLoadActivation("활하중", 365);	
}

void CFemCantileverCalc::AddPointForceAtLeft(CFEMManage *pFEM, double dDist, CVector vForce, long nLoadCase, CString szLoadName, CString szGroupName)
{
	long nApplyElem = GetLoadApplyMemberCount(pFEM);

	for(unsigned short n=0; n<nApplyElem; n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);

		if(dDist >= (pJ_I->m_vPoint.x) && dDist < (pJ_J->m_vPoint.x))
		{			
			double dDistLocal = dDist - (pJ_I->m_vPoint.x);
			double dRatio = dDistLocal / fabs(pJ_J->m_vPoint.x - pJ_I->m_vPoint.x);

			CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
//			pELoad->SetDistForce(dDistLocal, vForce, szLoadName, szGroupName, TRUE);
			pELoad->SetDistForce(dRatio, vForce, szLoadName, TRUE, szGroupName);
			break;
		}
	}
}

BOOL CFemCantileverCalc::IsLeft(CPlateBasicIndex *pBx, double dDist)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	
	double	dWidthBr		= toM(pDB->GetWidthSlabDom(pBx));
	if(dDist>dWidthBr/2)	return FALSE;
	else					return TRUE;
	
	return TRUE;
}

void CFemCantileverCalc::AddTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, double dForceStt, double dForceEnd, long nLoadCase, CString szLoadName, CString szGroupName, BOOL bRatio)
{
	CElement	*pE		= NULL;
	CJoint		*pJ_I	= NULL;
	CJoint		*pJ_J	= NULL;
	CLoadForce	*pELoad	= NULL;
	double dLen	= 0;
	double dStt = 0;
	double dEnd = 0;	

	for(unsigned short n=0; n<pFEM->GetElementSize(); n++)
	{
		pE		= pFEM->GetElement(n);
		pJ_I	= pFEM->GetJoint(pE->m_nJStt);
		pJ_J	= pFEM->GetJoint(pE->m_nJEnd);
		if(dDistStt >= (pJ_I->m_vPoint.x) && dDistStt < (pJ_J->m_vPoint.x))
		{
			double dForceMiddle = dForceStt + (dForceEnd - dForceStt) / (dDistEnd - dDistStt) * min(dDistEnd - dDistStt, (pJ_J->m_vPoint.x) - dDistStt);
			if(dDistEnd <= (pJ_J->m_vPoint.x))
			{
				dLen	= pJ_J->m_vPoint.x - pJ_I->m_vPoint.x;
				dStt	= (dDistStt - pJ_I->m_vPoint.x)/(bRatio?dLen:1);
				dEnd	= (dDistEnd - pJ_I->m_vPoint.x)/(bRatio?dLen:1);
				pELoad	= pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dStt, dEnd, CVector(0,0,dForceStt), CVector(0,0,dForceEnd), CVector(0,0,0), 
									 CVector(0,0,0), szLoadName, CVector(0,0,0), bRatio, FALSE, szGroupName);
				break;
			}		
			else
			{
				dLen	= pJ_J->m_vPoint.x - pJ_I->m_vPoint.x;
				dStt	= (dDistStt - pJ_I->m_vPoint.x)/(bRatio?dLen:1);
				dEnd	= (pJ_J->m_vPoint.x - pJ_I->m_vPoint.x)/(bRatio?dLen:1);
				pELoad	= pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dStt, dEnd, CVector(0,0,dForceStt), CVector(0,0,dForceMiddle), CVector(0,0,0), 
									 CVector(0,0,0), szLoadName, CVector(0,0,0), bRatio, FALSE, szGroupName);
				dForceStt	= dForceMiddle;
				dDistStt	= (pJ_J->m_vPoint.x);			
			}			
		}
	}
}

long CFemCantileverCalc::GetLoadApplyMemberCount(CFEMManage *pFEM)
{
	long nCnt = 0;

	for (long nE=0; nE<pFEM->GetElementSize(); nE++)
	{
		CElement* pElem = pFEM->GetElement((unsigned short)nE);
		CVector vStt    = pFEM->GetJoint(pElem->m_nJStt)->m_vPoint;
		CVector vEnd    = pFEM->GetJoint(pElem->m_nJEnd)->m_vPoint;

		if (vStt.z==0 && vEnd.z==0) nCnt++; 
	}

	return nCnt;
}

long CFemCantileverCalc::GetElementIndexFromDistance(CFEMManage *pFEM, double dDis)
{
	long nElem = -1;
	long nTotal = GetLoadApplyMemberCount(pFEM);

	for (long n=0; n<nTotal; n++)
	{
		CElement* pE = pFEM->GetElement((unsigned short)n);
		CJoint	* pJ = pFEM->GetJoint(pE->m_nJEnd);

		if (dDis<=pJ->m_vPoint.x)
		{
			nElem = n;
			break;
		}
	}


	return nElem;
}

void CFemCantileverCalc::CalcCarLoad(double dValue[3], double dDis[2])
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();

	// 0:Left 1:Middle 2:Right
	dValue[0] = dValue[1] = dValue[2] = 0;
	dDis[0]	  = dDis[1]   = 0;

	// Set Girder Data
	CDoubleArray dSpaceArr;
	long   nGirSu	= pDB->GetGirdersu();
	double dLeftX   = pDB->GetSlabLeft()->m_dGirderCenter;
	double dRightX  = pDB->GetSlabRight()->m_dGirderCenter;
	double dLen		= pDB->GetGirder(0)->m_dGirderCenter - dLeftX;
	dSpaceArr.Add(dLen);
	dDis[0] = dLen;
	long nG = 0;
	for (nG=0; nG<nGirSu-1; nG++)
	{
		double dGirderCenterPrev = pDB->GetGirder(nG)->m_dGirderCenter;
		double dGirderCenterNext = pDB->GetGirder(nG+1)->m_dGirderCenter;
		dLen = dGirderCenterNext - dGirderCenterPrev;
		dSpaceArr.Add(dLen);
	}
	dLen = dRightX - pDB->GetGirder(pDB->GetGirdersu()-1)->m_dGirderCenter;
	dSpaceArr.Add(dLen);

	CCentSeparation* pGuardL = pDB->GetGuardFencLeft();
	CCentSeparation* pGuardR = pDB->GetGuardFencRight();

	// 1. 좌측 캔틸레버부
	double dGurdWidth	= (pGuardL)? pGuardL->GetWidth()+pGuardL->m_D1 : 0;
	double dSpace		= dSpaceArr.GetAt(0);
	double dCar			= 300;
	double dFlange		= 0;
	double dWeb			= 0;
	SeekFlange_Web(dFlange, dWeb);
	double dLengthLeft	= toM((dSpace-dGurdWidth) - dCar  - dWeb/2 - (dFlange/4 - dWeb/4));

	double dFac = (15/(40+dLengthLeft)>0.3) ? 0.3 : 15/(40+dLengthLeft);
	CString szGrade = pDB->m_sDeungeub;
	long	nIdx = szGrade.Find("등급");
	szGrade = szGrade.Left(nIdx);
	long   nGrade = atoi(szGrade);
	double dActive= (nGrade==1)? 96 : 72.54;
	double dLoad = dActive * (1+dFac);
	double dE = 12500/9.*dActive;
	double dB = sqrt( dE*pow(10.0,-6)/2.5 );
	dValue[0] = dLoad * dB;

	// 2. 중앙 캔틸레버부
	dSpace = 0;
	for (long k=0; k<dSpaceArr.GetSize()-1; k++)
		dDis[1] += dSpaceArr.GetAt(k);

	for (nG=1; nG<dSpaceArr.GetSize()-1; nG++)
	{
		double dDis = dSpaceArr.GetAt(nG);
		dSpace = (dSpace<dDis)? dDis : dSpace;
	}
	if (nGirSu!=2)
		dSpace -= dWeb - (dFlange-dWeb)/2;

	double dLengthCen = toM(dSpace);

	dFac = (15/(40+dLengthCen)>0.3) ? 0.3 : 15/(40+dLengthCen);
	dLoad = dActive * (1+dFac);
	dValue[1] = dLoad * dB;

	// 3. 우측 캔틸레버부
	dGurdWidth	= (pGuardR)? pGuardR->GetWidth()+pGuardR->m_D1 : 0;
	dSpace = dSpaceArr.GetAt(dSpaceArr.GetSize()-1);

	double dLengthRight	= toM((dSpace-dGurdWidth) - dCar  - dWeb/2 + (dFlange/4 - dWeb/4));

	dFac = (15/(40+dLengthRight)>0.3) ? 0.3 : 15/(40+dLengthRight);
	dLoad = dActive * (1+dFac);
	dValue[2] = dLoad * dB;

}

BOOL CFemCantileverCalc::SetDBLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nG, double dSttRoadway, double dEndRoadway, 
			long &nLoadCase, CHLLLaneUnit *pHLLLaneUnit, long nQtyLane, CString szLoadName, CString szGroupName)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pData		= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
	CCalcGeneral		CalceGeneral(m_pDataManage);
	
	double	dDBWidth			= pData->m_DBWidth;
	double	dEdge				= pData->m_DBFromEdge;
	double	dLaneWidth			= dEndRoadway-dSttRoadway;
	long	nQtyDesignLane		= CalceGeneral.GetQtyLane(dLaneWidth); 
	double	dDesignLaneWidth	= MIN(Round(dLaneWidth/nQtyDesignLane,3), 3.6);
	
	if (pData->m_nHLiveLoadType==2)
		dDesignLaneWidth = pData->m_dUserLaneWidth;
	
	if(nQtyDesignLane==0)
		return FALSE;
	
	CHLLLaneUnit LaneUnit;
	
	for(long nLane=0; nLane<nQtyLane; nLane++)
	{
		CHLLLaneElement	HLLLaneElem;
		
		if(nLane==0)
		{
			if(nG==0)
			{
				HLLLaneElem.m_dSttHDanPos = dSttRoadway+dEdge;
				HLLLaneElem.m_dEndHDanPos = HLLLaneElem.m_dSttHDanPos+dDBWidth;
			}
			else
			{
				HLLLaneElem.m_dEndHDanPos = dEndRoadway-dEdge;
				HLLLaneElem.m_dSttHDanPos = HLLLaneElem.m_dEndHDanPos-dDBWidth;
			}
		}
		else
		{
			CHLLLaneElement	HLLLaneElemLeft		= LaneUnit.GetElementSide(TRUE);
			CHLLLaneElement	HLLLaneElemRight	= LaneUnit.GetElementSide(FALSE);
			
			if(HLLLaneElemLeft.m_dSttHDanPos-dSttRoadway>dEdge+dDBWidth+(dDesignLaneWidth-dDBWidth))
			{
				HLLLaneElem.m_dEndHDanPos	= HLLLaneElemLeft.m_dSttHDanPos-(dDesignLaneWidth-dDBWidth);
				HLLLaneElem.m_dSttHDanPos	= HLLLaneElem.m_dEndHDanPos-dDBWidth;
			}
			
			if(dEndRoadway-HLLLaneElemRight.m_dEndHDanPos>dEdge+dDBWidth+(dDesignLaneWidth-dDBWidth))
			{
				HLLLaneElem.m_dSttHDanPos	= HLLLaneElemRight.m_dEndHDanPos+(dDesignLaneWidth-dDBWidth);
				HLLLaneElem.m_dEndHDanPos	= HLLLaneElem.m_dSttHDanPos+dDBWidth;
			}
		}
		
		if( HLLLaneElem.m_dEndHDanPos>0 )
			LaneUnit.m_vLaneElem.push_back(HLLLaneElem);
	}

	// seek load
	double dLoad[3] = { 0, 0, 0 };
	double dDis[2] = { 0 ,0 };
	CalcCarLoad(dLoad, dDis);
	
	if(LaneUnit.m_vLaneElem.size()==nQtyLane)
	{
		for(long n=0; n<(long)LaneUnit.m_vLaneElem.size(); n++)
		{
			CHLLLaneElement	HLLLaneElem(LaneUnit.m_vLaneElem.at(n));

			double dValue[2] = { 0, 0 };
			double dStt = HLLLaneElem.m_dSttHDanPos;
			double dEnd = HLLLaneElem.m_dEndHDanPos;
			dValue[0] = (dStt<dDis[0])? dLoad[0] : (dStt<dDis[1])? dLoad[1] : dLoad[2];
			dValue[1] = (dEnd<dDis[0])? dLoad[0] : (dEnd<dDis[1])? dLoad[1] : dLoad[2];
			
			pHLLLaneUnit->m_vLaneElem.push_back(HLLLaneElem);
			AddUnitPointForceAtLeft(pFEM, dStt, nLoadCase, szLoadName, szGroupName, dValue[0]);
			AddUnitPointForceAtLeft(pFEM, dEnd, nLoadCase, szLoadName, szGroupName, dValue[1]);
		}
		
		return TRUE;
	}
	return FALSE;
}

BOOL CFemCantileverCalc::SetCentrifugalLoad(CFEMManage *pFEM, CPlateBasicIndex *pBx, long nG, double dSttRoadway, 
											double dEndRoadway, long &nLoadCase, CHLLLaneUnit *pHLLLaneUnit, long nQtyLane,
											double dCantilDist[3], double dForce[3], double dMoment[3], CString szLoad, CString szGroup)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pData		= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CPlateBasicIndex	*pBxMatch	= pBx->GetBxMatchByCrossBeam(pGir);
	CCalcGeneral		CalceGeneral(m_pDataManage);
	
	double	dDBWidth			= pData->m_DBWidth;
	double	dEdge				= pData->m_DBFromEdge;
	double	dLaneWidth			= dEndRoadway-dSttRoadway;
	long	nQtyDesignLane		= CalceGeneral.GetQtyLane(dLaneWidth); 
	double	dDesignLaneWidth	= MIN(Round(dLaneWidth/nQtyDesignLane,3), 3.6);
	
	if(nQtyDesignLane==0)
		return FALSE;
	
	CHLLLaneUnit LaneUnit;

	
	for(long nLane=0; nLane<nQtyLane; nLane++)
	{
		CHLLLaneElement	HLLLaneElem;
		
		if(nLane==0)
		{
			if(nG==0)
			{
				HLLLaneElem.m_dSttHDanPos = dSttRoadway+dEdge;
				HLLLaneElem.m_dEndHDanPos = HLLLaneElem.m_dSttHDanPos+dDBWidth;
			}
			else
			{
				HLLLaneElem.m_dEndHDanPos = dEndRoadway-dEdge;
				HLLLaneElem.m_dSttHDanPos = HLLLaneElem.m_dEndHDanPos-dDBWidth;
			}
		}
		else
		{
			CHLLLaneElement	HLLLaneElemLeft		= LaneUnit.GetElementSide(TRUE);
			CHLLLaneElement	HLLLaneElemRight	= LaneUnit.GetElementSide(FALSE);
			
			if(HLLLaneElemLeft.m_dSttHDanPos-dSttRoadway>dEdge+dDBWidth+(dDesignLaneWidth-dDBWidth))
			{
				HLLLaneElem.m_dEndHDanPos	= HLLLaneElemLeft.m_dSttHDanPos-(dDesignLaneWidth-dDBWidth);
				HLLLaneElem.m_dSttHDanPos	= HLLLaneElem.m_dEndHDanPos-dDBWidth;
			}
			
			if(dEndRoadway-HLLLaneElemRight.m_dEndHDanPos>dEdge+dDBWidth+(dDesignLaneWidth-dDBWidth))
			{
				HLLLaneElem.m_dSttHDanPos	= HLLLaneElemRight.m_dEndHDanPos+(dDesignLaneWidth-dDBWidth);
				HLLLaneElem.m_dEndHDanPos	= HLLLaneElem.m_dSttHDanPos+dDBWidth;
			}
		}
		
		if(HLLLaneElem.m_dEndHDanPos>0)
			LaneUnit.m_vLaneElem.push_back(HLLLaneElem);
	}
	
	if(LaneUnit.m_vLaneElem.size()==nQtyLane)
	{
		for(long n=0; n<(long)LaneUnit.m_vLaneElem.size(); n++)
		{
			CHLLLaneElement	HLLLaneElem(LaneUnit.m_vLaneElem.at(n));


			pHLLLaneUnit->m_vLaneElem.push_back(HLLLaneElem);
			
			CVector vForce(dForce[0], 0, 0);
			CVector vMoment(0, dMoment[0], 0);
			double  dPos = (HLLLaneElem.m_dSttHDanPos + HLLLaneElem.m_dEndHDanPos)/2;

			if (dPos < dCantilDist[0])
			{
				vForce.x  = dForce[0];
				vMoment.y = dMoment[0];
			}
			else if (dPos < dCantilDist[1])
			{
				vForce.x  = dForce[1];
				vMoment.y = dMoment[1];
			}
			else
			{
				vForce.x  = dForce[2];
				vMoment.y = dMoment[2];
			}

			AddPointForceMomentAtLeft(pFEM, dPos, nLoadCase, vForce, vMoment, szLoad, szGroup);
		}
		
		return TRUE;
	}
	return FALSE;
}

void CFemCantileverCalc::InsertPreLane(CFEMManage *pFEM, CHLLLane *pHLLLane, CHLLLaneUnit *pHLLLaneUnit, long nLane, long nLoadCase, CString szLoadName, CString szGroupName)
{
	if(pHLLLane->m_vLaneUnit.size()==0)	return;
	
	CHLLLaneUnit HLLLaneUnitLast = pHLLLane->m_vLaneUnit.at(nLane);

	// seek load
	double dLoad[3] = { 0, 0, 0 };
	double dDis[2] = { 0 ,0 };
	CalcCarLoad(dLoad, dDis);
	
	for(long n=0; n<(long)HLLLaneUnitLast.m_vLaneElem.size(); n++)
	{
		CHLLLaneElement Elem1	= HLLLaneUnitLast.m_vLaneElem.at(n);
		CHLLLaneElement Elem2(Elem1);
		pHLLLaneUnit->m_vLaneElem.push_back(Elem2);

		double dValue[2] = { 0, 0 };
		double dStt = Elem2.m_dSttHDanPos;
		double dEnd = Elem2.m_dEndHDanPos;
		dValue[0] = (dStt<dDis[0])? dLoad[0] : (dStt<dDis[1])? dLoad[1] : dLoad[2];
		dValue[1] = (dEnd<dDis[0])? dLoad[0] : (dEnd<dDis[1])? dLoad[1] : dLoad[2];


		AddUnitPointForceAtLeft(pFEM, Elem2.m_dSttHDanPos, nLoadCase, szLoadName, szGroupName, dValue[0]);
		AddUnitPointForceAtLeft(pFEM, Elem2.m_dEndHDanPos, nLoadCase, szLoadName, szGroupName, dValue[1]);
	}
}

void CFemCantileverCalc::InsertPreLaneCentri(CFEMManage *pFEM, CHLLLane *pHLLLane, CHLLLaneUnit *pHLLLaneUnit, long nLane, long nLoadCase, double dDis[3], double dForce[3], double dMoment[3], CString szLoadName, CString szGroupName)
{
	if(pHLLLane->m_vLaneUnit.size()==0)	return;
	
	CHLLLaneUnit HLLLaneUnitLast = pHLLLane->m_vLaneUnit.at(nLane);
	
	
	for(long n=0; n<(long)HLLLaneUnitLast.m_vLaneElem.size(); n++)
	{
		CHLLLaneElement Elem1	= HLLLaneUnitLast.m_vLaneElem.at(n);
		CHLLLaneElement Elem2(Elem1);
		pHLLLaneUnit->m_vLaneElem.push_back(Elem2);
		
		CVector vForce(dForce[0], 0, 0);
		CVector vMoment(0, dMoment[0], 0);
		double dPos	= (Elem2.m_dSttHDanPos + Elem2.m_dEndHDanPos)/2;
		
		if (dPos < dDis[0])
		{
			vForce.x  = dForce[0];
			vMoment.y = dMoment[0];
		}
		else if (dPos < dDis[1])
		{
			vForce.x  = dForce[1];
			vMoment.y = dMoment[1];
		}
		else
		{
			vForce.x  = dForce[2];
			vMoment.y = dMoment[2];
		}

		AddPointForceMomentAtLeft(pFEM, dPos, nLoadCase, vForce, vMoment, szLoadName, szGroupName);	
	}
}

BOOL CFemCantileverCalc::AddUnitPointForceAtLeft(CFEMManage *pFEM, double dDist, long nLoadCase, CString szLoadName, CString szGroupName, double dValue)
{
	for(unsigned short n=0; n<pFEM->GetElementSize(); n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);
		if(dDist >= (pJ_I->m_vPoint.x) && dDist < (pJ_J->m_vPoint.x))
		{
			double dLength = fabs(pJ_I->m_vPoint.x - pJ_J->m_vPoint.x);
			double dDistLocal = dDist - (pJ_I->m_vPoint.x);
			double dRatio = dDistLocal / dLength;

			CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
			pELoad->SetDistForce(dRatio, CVector(0,0,-dValue), szLoadName, TRUE, szGroupName);

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CFemCantileverCalc::AddPointForceMomentAtLeft(CFEMManage *pFEM, double dDist, long nLoadCase, CVector vForce, CVector vMoment, CString szLoadName, CString szGroupName)
{
	for(unsigned short n=0; n<pFEM->GetElementSize(); n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);
		if(dDist >= (pJ_I->m_vPoint.x) && dDist < (pJ_J->m_vPoint.x))
		{			
			double dDistLocal = dDist - (pJ_I->m_vPoint.x);
			double dLength = fabs(pJ_I->m_vPoint.x - pJ_J->m_vPoint.x);
			double dRatio = dDistLocal / dLength;

			CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
			pELoad->SetDistForce(dRatio, vForce, szLoadName, TRUE, szGroupName);
			pELoad->SetDistMoment(dRatio, vMoment, szLoadName, TRUE, szGroupName);
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CFemCantileverCalc::AddUnitTrapForceAtLeft(CFEMManage *pFEM, double dDistStt, double dDistEnd, long nLoadCase, CString szLoadName, CString szGroupName)
{
	long nApplyElem = GetLoadApplyMemberCount(pFEM);

	for(unsigned short n=0; n<nApplyElem; n++)
	{
		CElement *pE = pFEM->GetElement(n);
		CJoint *pJ_I = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJ_J = pFEM->GetJoint(pE->m_nJEnd);
		
		if(dDistStt >= (pJ_I->m_vPoint.x) && dDistStt < (pJ_J->m_vPoint.x))
		{			
			if(dDistEnd <= (pJ_J->m_vPoint.x))
			{
				double dStt  = dDistStt - (pJ_I->m_vPoint.x);
				double dEnd  = dDistEnd - (pJ_I->m_vPoint.x);
				double dLength = fabs( pJ_I->m_vPoint.x - pJ_J->m_vPoint.x );
				double dRat1 = dStt / dLength;
				double dRat2 = dEnd / dLength;

				CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dRat1, dRat2, CVector(0,0,-1), CVector(0,0,-1), CVector(0,0,0), CVector(0,0,0), 
					szLoadName, CVector(0,0,0), TRUE, FALSE, szGroupName);
			}		
			else
			{
				double dStt = dDistStt - (pJ_I->m_vPoint.x);
				double dEnd = (pJ_J->m_vPoint.x) - (pJ_I->m_vPoint.x);
				double dLength = fabs( pJ_I->m_vPoint.x - pJ_J->m_vPoint.x );
				double dRat1 = dStt / dLength;
				double dRat2 = dEnd / dLength;

				CLoadForce *pELoad = pE->GetLoadForce(nLoadCase);
				pELoad->SetLoadForce(dRat1, dRat2, CVector(0,0,-1), CVector(0,0,-1), CVector(0,0,0), CVector(0,0,0), 
					szLoadName, CVector(0,0,0), TRUE, FALSE, szGroupName);
				dDistStt = (pJ_J->m_vPoint.x);
			}			
		}
	}
	return FALSE;
}

void CFemCantileverCalc::SetWindLoadForce(long nJijum)
{
	CPlateBridgeApp* pDB= m_pDataManage->GetBridge();
	CPlateGirderApp* pGirCR = pDB->GetGirder(-1);
	CPlateGirderApp* pGirL  = pDB->GetGirder(0);
	CPlateGirderApp* pGirR  = pDB->GetGirder(pDB->GetGirdersu()-1);
	CPlateBasicIndex *pBx   = pDB->GetGirder(-1)->GetBxOnJijum(nJijum);
	CCalcData*       pData  = m_pDataManage->GetCalcData();
	
	BOOL bTwoBangEm = FALSE;
	///< 방음벽이 없을 경우는 리턴함
	if(pData->DESIGN_FLOOR_DATA[0].m_bBangEm==FALSE
		&& pData->DESIGN_FLOOR_DATA[1].m_bBangEm==FALSE) return;
	
	if(pData->DESIGN_FLOOR_DATA[0].m_bBangEm
		&& pData->DESIGN_FLOOR_DATA[1].m_bBangEm) bTwoBangEm = TRUE;
	
	// 방음벽 높이(방호벽 높이 포함)
	double dHBangEmL = toM(pData->DESIGN_FLOOR_DATA[0].m_dBangEmHeight);
	double dHBangEmR = toM(pData->DESIGN_FLOOR_DATA[1].m_dBangEmHeight);
	// 교량 총 폭(나중에 수정요함)
	double dSlabWidth= toM(pDB->GetWidthAct(pBx->GetStation()));
	// 교량 총 높이
	double dTotalHBriL = 0, dTotalHBriR = 0;
	// 슬래브 윗부분 = 방호벽 + 방음벽
	double dUpperHBriL = 0, dUpperHBriR = 0;
	// 풍하중 
	double dForceWindL = 0;
	double dForceWindR = 0;
	// 풍하중 모멘트 팔(거리)
	double dMomentDisL = 0;
	double dMomentDisR = 0;
	//////////////////////////////////////////////////////////
	// 교량 왼쪽
	if(pData->DESIGN_FLOOR_DATA[0].m_bBangEm)  // 벽형 강성 방호책
	{
		dTotalHBriL = toM(pGirL->GetHeightGirderByJijum(nJijum)) + toM(pDB->m_dThickSlabBase) + dHBangEmL;
		dUpperHBriL = dHBangEmL + toM(pDB->m_dThickSlabBase) / 2.0;
		
		dForceWindL = GetWindLoad(dSlabWidth, dTotalHBriL, dTotalHBriL);		
		dMomentDisL = dUpperHBriL - dTotalHBriL/2.0;
		
		pData->m_dLeftWindFroceFromL[nJijum]  = dForceWindL;				// P
		pData->m_dLeftWindMomentFromL[nJijum] = dForceWindL * dMomentDisL; // M
		
		if(!bTwoBangEm)///< 바람이 안쪽에서 불어올때
		{
			dForceWindL = GetWindLoad(dSlabWidth,dTotalHBriL,dHBangEmL);		
			dMomentDisL = dHBangEmL / 2.0  + toM(pDB->m_dThickSlabBase) / 2.0; 
			pData->m_dLeftWindFroceFromR[nJijum]  = -dForceWindL;				// P
			pData->m_dLeftWindMomentFromR[nJijum] = -dForceWindL * dMomentDisL; // M
		}
	}	
	//////////////////////////////////////////////////////////
	if(pData->DESIGN_FLOOR_DATA[1].m_bBangEm)  // 벽형 강성 방호책
	{
		dTotalHBriR = toM(pGirR->GetHeightGirderByJijum(nJijum)) + toM(pDB->m_dThickSlabBase) + dHBangEmR;
		dUpperHBriR = dHBangEmR + toM(pDB->m_dThickSlabBase) / 2.0;
		
		dForceWindR = GetWindLoad(dSlabWidth,dTotalHBriR,dTotalHBriR);		
		dMomentDisR = dUpperHBriR - dTotalHBriR/2.0;
		
		pData->m_dRightWindFroceFromR[nJijum]  = -dForceWindR; // P
		pData->m_dRightWindMomentFromR[nJijum] = -dForceWindR * dMomentDisR; // M
		
		if(!bTwoBangEm)
		{
			dForceWindR = GetWindLoad(dSlabWidth,dTotalHBriR,dHBangEmR);		
			dMomentDisR = dHBangEmR / 2.0 + toM(pDB->m_dThickSlabBase) / 2.0; 
			pData->m_dRightWindFroceFromL[nJijum]  = dForceWindR; // P
			pData->m_dRightWindMomentFromL[nJijum] = dForceWindR * dMomentDisR; // M
		}
	}
}


// 풍하중 크기 구하기
double CFemCantileverCalc::GetWindLoad(double dBridgeWidth, double dBridgeHeight, double dD)
{
	double Ratio = dBridgeWidth / dBridgeHeight;
	double WindLoad = 0;
	
	if(Ratio && Ratio < 8) 
		WindLoad = (400 - 20 * Ratio) * dD;
	
	if(8 <= Ratio)
		WindLoad = 240 * dD;
	
	if(WindLoad < 600) WindLoad = 600;
	
	return tokN(WindLoad);   // kg/m --->  t/m
}

double CFemCantileverCalc::GetElemLength(CFEMManage *pFemMange, long nElm)
{
	CElement *pElm  = pFemMange->GetElement((unsigned short)nElm);
	CJoint   *pJStt = pFemMange->GetJoint(pElm->m_nJStt);
	CJoint   *pJEnd = pFemMange->GetJoint(pElm->m_nJEnd);
	
	double dLengthElm = ~(pJStt->m_vPoint-pJEnd->m_vPoint);
	
	return dLengthElm;
}

double CFemCantileverCalc::GetRadius(long nPosition)
{
	CCalcData	*pData		= m_pDataManage->GetCalcData();

	return pData->CALC_CANTILEVER_DATA[nPosition].m_Cent_R;

	//CPlateBridgeApp	*pBridge	= m_pDataManage->GetBridge();
	//CLineInfoApp	*pLineApp	= m_pDataManage->GetLineInfoApp();
	//CLineInfo		*pLine		= pLineApp->GetLine(0);
	//CLineSplit		*pSplit		= NULL;

	//long		nIPs = pLine->GetQtyIp();
	//long		nRow =	-1;

	//double		dBC	 =	0.0;
	//double		dEC	 =	0.0;	

	//CDoubleArray dArrRadius;

	//for (long nIp=0; nIp<nIPs; nIp++)
	//{
	//	pSplit = pLine->GetLineSplitByIp(nIp);

	//	if(pSplit->m_dR > 0)
	//	{				
	//		pLine->GetStationMatchByXyAndAngle(pSplit->m_xyBc,	CDPoint(0,0),	dBC);
	//		pLine->GetStationMatchByXyAndAngle(pSplit->m_xyEc,	CDPoint(0,0),	dEC);
	//		
	//		nRow++;
	//		
	//		if(dBC <= 0 || dEC <=0 )	continue;
	//		
	//		if((pBridge->GetStationBridgeStt() >= dBC && pBridge->GetStationBridgeStt() <= dEC)	||	//Comment-1
	//			(pBridge->GetStationBridgeEnd() >= dBC && pBridge->GetStationBridgeEnd() <= dEC)	||	//Comment-2
	//			(pBridge->GetStationBridgeStt() <= dBC && pBridge->GetStationBridgeEnd() >= dEC))	//Comment-3
	//		{
	//			CIPRecord* pIpRec = pLine->m_pIpRec + nRow;
	//			
	//			dArrRadius.Add(toM(pIpRec->m_dR_IpRecord));
	//		}	
	//	}
	//}

	//double dRadius = (dArrRadius.GetSize()>0)? dArrRadius.MinIdx() : 0;

	//return dRadius;
}

void CFemCantileverCalc::GetUnitCentrifugalLoad(double dLoad[3], double dDist[3])
{
	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CCalcData	*pData  = m_pDataManage->GetCalcData();
	dLoad[0] = dLoad[1] = dLoad[2] = 0;		// 켄틸레버부에 작용하는 축하중

	double dIfactor[3]   = { 0, 0, 0 };		// 충격계수
	double dApplyLoad[3] = { 0, 0, 0 };		// 작용DB하중
	double dWidth[3]	 = { 0, 0, 0 };		// 윤하중 분포폭


	// Set Girder Data
	CDoubleArray dSpaceArr;
	long   nGirSu	= pDB->GetGirdersu();
	double dLeftX   = pDB->GetSlabLeft()->m_dGirderCenter;
	double dRightX  = pDB->GetSlabRight()->m_dGirderCenter;
	double dLen = pDB->GetGirder(0)->m_dGirderCenter - dLeftX;
	dSpaceArr.Add(dLen);
	dDist[0] = dDist[1] = dDist[2] = dLen;

	long nG = 0;
	for (nG=0; nG<nGirSu-1; nG++)
	{
		double dGirderCenterPrev = pDB->GetGirder(nG)->m_dGirderCenter;
		double dGirderCenterNext = pDB->GetGirder(nG+1)->m_dGirderCenter;
		dLen = dGirderCenterNext - dGirderCenterPrev;
		dSpaceArr.Add(dLen);
		dDist[1] += dLen;
		dDist[2] += dLen;
	}
	dLen = dRightX - pDB->GetGirder(pDB->GetGirdersu()-1)->m_dGirderCenter;
	dSpaceArr.Add(dLen);
	dDist[2] += dLen;

	CCentSeparation* pGuardL = pDB->GetGuardFencLeft();
	CCentSeparation* pGuardR = pDB->GetGuardFencRight();
	double dGurdWidth	= 0;
	double dSpace		= dSpaceArr.GetAt(0);
	double dCar			= 300;
	double dFlange	= 0;
	double dWeb		= 0;
	double dLength	= 0;
	double dPr		= tokN(pData->DESIGN_CONDITION.m_dPr);
	double dE		= 12500/9*dPr;
	SeekFlange_Web(dFlange, dWeb);

	// 좌측켄틸레버부
	dGurdWidth	= (pGuardL)? pGuardL->GetWidth()+pGuardL->m_D1 : 0;
	dLength		= toM((dSpace-dGurdWidth-dFlange/2) - dCar  + dFlange/4 - dWeb/4);
	dIfactor[0] = min( 15/(40+dLength), 0.3 );
	dApplyLoad[0] = dPr*(1+dIfactor[0]);
	dWidth[0] = sqrt(dE*pow(10, -6.25));
	dLoad[0]  = dApplyLoad[0]*dWidth[0];

	// 중앙부
	dSpace = 0;
	for (nG=1; nG<dSpaceArr.GetSize(); nG++)
	{
		double dDis = dSpaceArr.GetAt(nG);
		dSpace = (dSpace<dDis)? dDis : dSpace;
	}

	if (nGirSu==2)
	{
		dLength = (dSpace-dFlange+360 < dSpace)? dSpace-dFlange+360 : dSpace;
		dLength = toM(dLength);
	}
	else
	{
		dLength -= dWeb + (dFlange-dWeb)/2;
		dLength = toM(dLength);
	}
	dIfactor[1] = min( 15/(40+dLength), 0.3 );
	dApplyLoad[1] = dPr*(1+dIfactor[1]);
	dWidth[1] = sqrt(dE*pow(10, -6.25));
	dLoad[1]  = dApplyLoad[1]*dWidth[1];


	// 우측켄틸레버
	dGurdWidth	= (pGuardR)? pGuardR->GetWidth()+pGuardR->m_D1 : 0;
	dSpace = dSpaceArr.GetAt(dSpaceArr.GetSize()-1);
	dLength = toM((dSpace-dGurdWidth-dFlange/2) - dCar  + dFlange/4 - dWeb/4);
	dIfactor[2] = min( 15/(40+dLength), 0.3 );
	dApplyLoad[2] = dPr*(1+dIfactor[2]);
	dWidth[2] = sqrt(dE*pow(10, -6.25));
	dLoad[2]  = dApplyLoad[2]*dWidth[2];
}

void CFemCantileverCalc::SeekFlange_Web(double& dFlange, double& dWeb)
{
	dFlange = dWeb = 0;
	
	__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;
	CPlateBridgeApp		*pDB = m_pDataManage->GetBridge();
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

void CFemCantileverCalc::GetSlabElemNum(CFEMManage *pFEM, long &nStt, long &nEnd)
{
	nStt	= -1;
	nEnd	= -1;
	for(long nEle=0; nEle<pFEM->GetElementSize(); nEle++)
	{
		CElement *pElem = pFEM->GetElement((unsigned short)nEle);
		if(nStt==-1 && pElem->m_LongAttrib[ELEM_TYPE]==ELEM_SLAB)
			nStt = nEle;
		if(nStt!=-1 && pElem->m_LongAttrib[ELEM_TYPE]!=ELEM_SLAB)
		{
			nEnd = nEle-1;
			return;
		}
	}
}

CDPoint CFemCantileverCalc::GetXyTensionBar(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();

	if(pDB->m_nSlabTensionSize==0) return NULL;
	
	double dLHeight	=	pDB->m_dHeightLeftTension;
	double dRHeight	=	pDB->m_dHeightRightTension;

	CDPoint A[HDANLINESU];
	
	pDB->GetXyPaveDom(pBx, A);	//?

	double dLDist	=	pDB->GetSlabLeft()->m_dGirderCenter;
	double dRDist	=	pDB->GetSlabRight()->m_dGirderCenter;

	double dX1, dX2, dY1, dY2;

	CDPoint ptStt		=	CDPoint(A[0].x, pDB->GetElevationPaveActByDis(pBx->GetStation(), dLDist) - (pDB->m_dThickPave + dLHeight));
	CDPoint ptSttUnit	=	ptStt;	
	CDPoint ptEnd		=	CDPoint(A[pDB->GetQtyHDanDom(pBx)].x,pDB->GetElevationPaveActByDis(pBx->GetStation(), dRDist) - (pDB->m_dThickPave + dRHeight));
	CDPoint ptEndUnit	=	ptEnd;
	
	CDPoint xyResult1,	xyResult2, xyResult3, xyResult4;
	CDPoint ptSttTemp,	ptEndTemp;
	
	CDPoint ptCen1,		ptCen2,	ptCen3;
	CDPoint ptCenter,	ptCenter2;
	CDPoint xyMatch1,	xyMatch2;

	double dDistTotal	=	0;

	BOOL bLeftFix = (pDB->m_nTendonFix != 1) ? TRUE : FALSE;	// 고정단이 좌측&지그재그 : TRUE, 우측 : FALSE
	
	for(long n=0; n<pDB->m_nSlabTensionSize; n++)
	{
		double	dL	=	pDB->m_SlabTension[n].m_dL;
		double	dR	=	pDB->m_SlabTension[n].m_dR;
		double	dH	=	pDB->m_SlabTension[n].m_dH;
		long	nT	=	pDB->m_SlabTension[n].m_nType;

		dDistTotal	+=	dL;
		
		if(n==0)	ptCen1	=	ptStt;	
		else		ptCen1	=	ptCen2;

		ptCen2	=	CDPoint(ptStt.x + dDistTotal, pDB->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal) - pDB->m_dThickPave - dH);
		ptCen3	=	CDPoint(ptStt.x + dDistTotal + pDB->m_SlabTension[n+1].m_dL, pDB->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal+pDB->m_SlabTension[n + 1].m_dL) - pDB->m_dThickPave - pDB->m_SlabTension[n + 1].m_dH);
		
		if(n==pDB->m_nSlabTensionSize - 1)
			ptCen3	=	ptEnd;
		
		CDPoint vDir1	=	(ptCen1 - ptCen2).Unit();
		CDPoint vDir2	=	(ptCen3 - ptCen2).Unit();
		CDPoint vDir3	=	(vDir1 + vDir2).Unit();
		
		//실제 반지름 구하기 - Start
		CDPoint vDir4	= vDir3.BetweenAngle(vDir1);
		double	dAng	= vDir4.GetAngleDegree();
		
		dAng = 360 - dAng;

		double dRealV = dAng - 90;

		dRealV = ToRadian(dRealV);

		double dH1		= tan(dRealV)*dR;
		double dRealR	= sqrt(dH1*dH1 + dR*dR);
		
		//실제 반지름 구하기 - End
		ptCenter	=	ptCen2 + vDir3 * dRealR;	
		
		//변곡점 시점부분
		if(n==0)
		{	
			if(GetTangentCircle(ptStt, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				
				dX1	=	bLeftFix ? pDB->m_dLengthSecMove : pDB->m_dLengthSecMoveEnd;
				dX2 =	xyMatch1.x - ptStt.x;
				dY2 =	xyMatch1.y - ptStt.y;

				dY1 = (dX1 / dX2) * dY2;

				ptSttUnit.x	=	ptStt.x + dX1;
				ptSttUnit.y =	ptStt.y + dY1;

				if(bLeftFix)
					ptSttTemp	=	ptSttUnit;
				else
				{
					ptSttTemp.x	=	ptSttUnit.x+140;
					ptSttTemp.y	=	ptSttUnit.y+140*(xyMatch1-ptSttUnit).Unit().y;
				}

				double dLengthDuct = bLeftFix  ? pDB->m_dLengthDuctOffset : 0;

				dX1	=	dLengthDuct;
				dX2 =	xyMatch1.x - ptSttUnit.x;
				dY2 =	xyMatch1.y - ptSttUnit.y;
				dY1 =	(dX1 / dX2) * dY2;
			}
		}
		else
		{
			//직선
			if(GetTangentCircle(xyMatch2, ptCenter, dR, xyResult1, xyResult2)==2)
			{
				if(nT==0)	xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				else		xyMatch1 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
			}			
		}

		if(n!=pDB->m_nSlabTensionSize)
		{
			//곡선
			CDPoint	vAng1	= (xyMatch1-ptCen2).Unit();
			
			vAng1.MirrorHorz();

			GetTangentCircle(ptCen3, ptCenter, dR, xyResult1, xyResult2);

			if(nT == 0)
				xyMatch2 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
			else
				xyMatch2 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
		}

		if(n == pDB->m_nSlabTensionSize-1)
		{
			dX1	=	bLeftFix ? pDB->m_dLengthSecMoveEnd : pDB->m_dLengthSecMove;
			dX2 =	xyMatch2.x - ptEnd.x;
			dY2 =	xyMatch2.y - ptEnd.y;
			
			dY1 =	(dX1 / dX2) * dY2;
			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;

			//마지막 부분
			if(bLeftFix)
				ptEndTemp	=	ptEndUnit;
			else
			{
				ptEndTemp.x	=	ptEndUnit.x - pDB->m_dLengthDuctOffset;
				ptEndTemp.y	=	ptEndUnit.y + pDB->m_dLengthDuctOffset * (xyMatch2 - ptEndUnit).Unit().y;
			}
			
			dX1	=	bLeftFix ? pDB->m_dLengthSecMoveEnd + 140 : pDB->m_dLengthSecMove;
			dX2	=	xyMatch2.x - ptEnd.x;
			dY2	=	xyMatch2.y - ptEnd.y;
			dY1	=	(dX1 / dX2) * dY2;
			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;	
		}
	}

	return bLeft ? ptSttUnit : ptEndUnit;
}

CJoint *CFemCantileverCalc::AddJointData(CFEMManage *pFEM, long &nIndex, CVector vJoint, CVector vMove, long nJointType, long nGirderNum, long nNodeSec)
{
	CJoint	*pJoint	= pFEM->AddJoint((unsigned short)nIndex, toM(vJoint-vMove));

	pJoint->m_LongAttrib[JOINT_TYPE]	= nJointType;
	pJoint->m_LongAttrib[GIRDER_NUM]	= nGirderNum;
	pJoint->m_LongAttrib[JOINT_SEC]	= nNodeSec;
	nIndex++;

	return pJoint;
}

CFrameSection *CFemCantileverCalc::GetFrameSectionByHeight(CFEMManage *pFEM, double dHeightStt, double dHeightEnd)
{
	CFrameSection	*pSec	= NULL;
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();

	while(pos)
	{
		pSec = (CFrameSection*)pFEM->m_FrameSectionList.GetNext(pos);
		if(Round(pSec->m_dJewon[0],6)==Round(dHeightStt,6) && Round(pSec->m_dJewon[2],6)==Round(dHeightEnd,6))
			return pSec;
	}

	return NULL;
}