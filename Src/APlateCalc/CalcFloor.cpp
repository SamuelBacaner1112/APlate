// CalcFloor.cpp: implementation of the CCalcFloor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateCalc/APlateCalc.h"
#include "CalcFloor.h"
#include "APlateCalcStd.h"
#include "APlateCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define EPSILON 0.000000001

CCalcFloor::CCalcFloor(CDataManage *pDataManage)
{
	m_pDataManage = pDataManage;
}

CCalcFloor::~CCalcFloor()
{
	
}

//double CCalcFloor::GetMapValuePreStress(CString sz)
//{
//	return m_MapPreStress[sz];
//}

// nSide =  0 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide =  1 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide =  2 : ������ �������ܿ��� ������ �߽ɱ���
// nSide =	3 : ��������
// ���ܿ��� ������ġ�� �ּ� �÷��� �� ��ġ��.
double CCalcFloor::CalcPrestressLength(long nSide)
{
	CPlateBridgeApp		*pBridge    = m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);
	CPlateBasicIndex	*pBx		= pGir->GetCrossBxAtMinFlangeWidth();

	if(pBridge->m_nSlabTensionSize==0) return 0;
	
	double m_dLHeight = pBridge->m_dHeightLeftTension;
	double m_dRHeight = pBridge->m_dHeightRightTension;

	CDPoint A[HDANLINESU];
	typedef struct _TENDON
	{		
		CDPoint xyStt;
		CDPoint xyEnd;
		BOOL    bLine;
		double dR;
		double dRad1;
		double dRad2;
		CDPoint xyCen;
	} TENDON;

	long nTENDTotal = pBridge->m_nSlabTensionSize*2+1;
	TENDON *TEND = new TENDON[nTENDTotal];
	
	pBridge->GetXyPaveUpperDom(pBx, A);	
	double dLDist = pBridge->GetSlabLeft()->m_dGirderCenter;
	double dRDist = pBridge->GetSlabRight()->m_dGirderCenter;
	CDPoint ptStt = CDPoint(A[0].x, pBridge->GetElevationPaveDomByDis(pBx, dLDist) - pBridge->m_dThickPave - m_dLHeight);	
	CDPoint ptEnd = CDPoint(A[pBridge->GetQtyHDanNode()].x,pBridge->GetElevationPaveDomByDis(pBx, dRDist) - pBridge->m_dThickPave - m_dRHeight);	

	CDPoint xyResult1, xyResult2, xyResult3, xyResult4;
	CDPoint pt1ForVec, pt2ForVec, pt1ForVec2, pt2ForVec2;
	CDPoint vec, vec2;
	CDPoint ptNext, ptNext2;
	CDPoint ptCenter, ptCenter2;
	CDPoint xyMatch1, xyMatch2;
	CDPoint ptSttUnit, ptEndUnit;
	double dDistTotal=0;	
	long   nTendon   = 0;
	CDPoint vX(1,0), vY(0,1);
	long n = 0;
	for(n=0; n<pBridge->m_nSlabTensionSize; n++)
	{
		double dL = pBridge->m_SlabTension[n].m_dL;
		double dR = pBridge->m_SlabTension[n].m_dR;
		double dH = pBridge->m_SlabTension[n].m_dH;
		long nT = pBridge->m_SlabTension[n].m_nType;
		dDistTotal += dL;

		pt1ForVec = CDPoint(ptStt.x + dDistTotal-5, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal-5));
		pt2ForVec = CDPoint(ptStt.x + dDistTotal+5, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal+5));
		vec = (pt2ForVec - pt1ForVec).Unit();
		
		ptNext = CDPoint(ptStt.x + dDistTotal, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal) - pBridge->m_dThickPave - dH);		
		ptCenter = ptNext + (nT==0 ? vec.RotateInv90() : vec.Rotate90()) * dR;				

		if(n==0)
		{
			if(GetTangentCircle(ptStt, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;

				double dX1 = pBridge->m_dLengthSecMove;
				double dX2 = xyMatch1.x - ptStt.x;
				double dY2 = xyMatch1.y - ptStt.y;
				double dY1 = (dX1/dX2)*dY2;

				ptSttUnit.x = ptStt.x + dX1;
				ptSttUnit.y = ptStt.y + dY1;

//				pDom->LineTo(ptStt, xyMatch1);
				TEND[nTendon].xyStt = ptStt;
				TEND[nTendon].xyEnd = xyMatch1;
				TEND[nTendon].bLine = TRUE;
				TEND[nTendon].dR    = 0;
				TEND[nTendon].dRad1 = 0;
				TEND[nTendon].dRad2 = 0;
				TEND[nTendon++].xyCen = CDPoint(0,0);
			}
		}
		else
		{
			if(GetTangentCircle(xyMatch2, ptCenter, dR, xyResult1, xyResult2)==2)
			{
				if(nT==0)
					xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				else
					xyMatch1 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
//				pDom->LineTo(xyMatch2, xyMatch1);
				TEND[nTendon].xyStt = xyMatch2;
				TEND[nTendon].xyEnd = xyMatch1;
				TEND[nTendon].bLine = TRUE;
				TEND[nTendon].dR    = 0;
				TEND[nTendon].dRad1 = 0;
				TEND[nTendon].dRad2 = 0;
				TEND[nTendon++].xyCen = CDPoint(0,0);
			}
		}

		if(n!=pBridge->m_nSlabTensionSize-1)
		{
			double dL2 = pBridge->m_SlabTension[n+1].m_dL;
			double dR2 = pBridge->m_SlabTension[n+1].m_dR;
			double dH2 = pBridge->m_SlabTension[n+1].m_dH;
			long nT2 = pBridge->m_SlabTension[n+1].m_nType;

			pt1ForVec2 = CDPoint(ptStt.x + dDistTotal+dL2-5, pBridge->GetElevationPaveDomByDis(pBx, dLDist+dDistTotal+dL2-5));
			pt2ForVec2 = CDPoint(ptStt.x + dDistTotal+dL2+5, pBridge->GetElevationPaveDomByDis(pBx, dLDist+dDistTotal+dL2+5));
			vec2 = (pt2ForVec2 - pt1ForVec2).Unit();
			
			ptNext2 = CDPoint(ptStt.x + dDistTotal+dL2, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal + dL2) - pBridge->m_dThickPave - dH2);			
			ptCenter2 = ptNext2 + (nT2==0 ? vec2.RotateInv90() : vec2.Rotate90()) * dR2;			
			
			if(GetTangentCircleAndCircle(ptCenter, dR, ptCenter2, dR2, xyResult1, xyResult2, xyResult3, xyResult4))
			{		
				xyMatch2 = xyResult1;
				xyMatch2 = ~(xyResult2 - xyMatch1) < ~(xyMatch2 - xyMatch1) ? xyResult2 : xyMatch2;
				// 1 ������ ã�ƾ� �ϹǷ�....
				//xyMatch2 = ~(xyResult3 - xyMatch1) < ~(xyMatch2 - xyMatch1) ? xyResult3 : xyMatch2;
				//xyMatch2 = ~(xyResult4 - xyMatch1) < ~(xyMatch2 - xyMatch1) ? xyResult4 : xyMatch2;				
			}
			else
			{
				//ASSERT(FALSE);
				xyMatch2 = xyMatch1;
			}

			double dAngle1 = (xyMatch1 - ptCenter).Unit().GetAngleDegree();

//			pDom->Arc(ptCenter, dR, dAngle1, dAngle2);		
			TEND[nTendon].xyStt = xyMatch1;
			TEND[nTendon].xyEnd = xyMatch2;
			TEND[nTendon].bLine = FALSE;
			TEND[nTendon].dR    = dR;
			TEND[nTendon].dRad1 = ToRadian(dAngle1);
			TEND[nTendon].dRad2 = ToRadian((xyMatch2 - ptCenter).Unit().GetAngleDegree());
			TEND[nTendon++].xyCen = ptCenter;	
		}
		else
		{
			if(GetTangentCircle(ptEnd, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch2 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;

				double dX1 = pBridge->m_dLengthSecMoveEnd;
				double dX2 = xyMatch2.x - ptEnd.x;
				double dY2 = xyMatch2.y - ptEnd.y;
				double dY1 = (dX1/dX2)*dY2;

				ptEndUnit.x = ptEnd.x - dX1;
				ptEndUnit.y = ptEnd.y - dY1;

//				pDom->LineTo(xyMatch2, ptEnd);
				TEND[nTendon+1].xyStt = xyMatch2;
				TEND[nTendon+1].xyEnd = ptEnd;
				TEND[nTendon+1].bLine = TRUE;
				TEND[nTendon+1].dR    = 0;
				TEND[nTendon+1].dRad1 = 0;
				TEND[nTendon+1].dRad2 = 0;
				TEND[nTendon+1].xyCen = CDPoint(0,0);
			}

			double dAngle1 = (xyMatch1 - ptCenter).Unit().GetAngleDegree();

//			pDom->Arc(ptCenter, dR, dAngle1, dAngle2);						
			TEND[nTendon].xyStt = xyMatch1;
			TEND[nTendon].xyEnd = xyMatch2;
			TEND[nTendon].bLine = FALSE;
			TEND[nTendon].dR    = dR;
			TEND[nTendon].dRad1 = ToRadian(dAngle1);
			TEND[nTendon].dRad2 = ToRadian((xyMatch2 - ptCenter).Unit().GetAngleDegree());
			TEND[nTendon++].xyCen = ptCenter;
		}
	}

	CDPoint xyCutting, xyAnswer;
	double dTGir     = pBridge->GetGirder(0)->GetThickFactByStation(pBx->GetStation(), G_W);
	double dSharpGir = pBridge->GetGirder(0)->GetLengthSharpOnBx(pBx, TRUE);
// nSide = 0 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide = 1 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide = 2 : ������ �������ܿ��� ������ �߽ɱ���
// nSide = 3 : ������ �������ܿ��� �������ܱ���
	xyCutting.y = A[0].y; 
	if(nSide==0)	
		xyCutting.x = A[0].x + pBridge->GetWidthSlabLeft() - dTGir/2 - dSharpGir/2;
	else if(nSide==1)
	{
		dTGir       = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetThickFactByStation(pBx->GetStation(), G_W);
		dSharpGir   = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetLengthSharpOnBx(pBx, TRUE);
		xyCutting.x = A[pBridge->GetQtyHDanNode()].x - pBridge->GetWidthSlabRight() + dTGir/2 + dSharpGir/2;
	}
	else if(nSide==2)
		xyCutting.x = A[0].x + fabs(pBridge->GetSlabRight()->m_dGirderCenter-pBridge->GetSlabLeft()->m_dGirderCenter)/2;
	else if(nSide==3)
	{
		TEND[0].xyStt	= ptSttUnit;
		xyCutting.x		= ptEndUnit.x;
	}
	
	double dTotalL = 0;
	for(n=0; n<nTENDTotal; n++)
	{
		TENDON Tendon = TEND[n];
		if(Tendon.bLine==TRUE)		// ����
		{
			if(GetXyMatchSegAndLine(Tendon.xyStt,Tendon.xyEnd, xyCutting, vY, xyMatch1))		// ������
			{
				xyAnswer = xyMatch1;
				dTotalL += ~(Tendon.xyStt-xyAnswer);
				break;
			}
			else
				dTotalL += ~(Tendon.xyStt-Tendon.xyEnd);
		}
		else						// �
		{
			if(GetXyMatchLineAndArc(xyCutting, vY, Tendon.xyCen, Tendon.dR, min(Tendon.dRad1, Tendon.dRad2), max(Tendon.dRad1, Tendon.dRad2), xyMatch1,xyMatch2) > 0)
			{
				xyAnswer = xyMatch1;
				dTotalL += GetLengthArcBy3Point(Tendon.xyCen, Tendon.xyStt, xyAnswer);
				break;
			}
			else
				dTotalL += GetLengthArcBy3Point(Tendon.xyCen, Tendon.xyStt, Tendon.xyEnd);
		}
	}
	
	delete[] TEND;

	return dTotalL;
}

// nSide =  0 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide =  1 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide =  2 : ������ �������ܿ��� ������ �߽ɱ���
// ���ܿ��� ������ġ�� �ּ� �÷��� �� ��ġ��.
double	CCalcFloor::CalcPrestressTotAngle(long nSide)
{
	CPlateBridgeApp		*pBridge    = m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);
	CPlateBasicIndex	*pBx		= pGir->GetCrossBxAtMinFlangeWidth();

	if(pBridge->m_nSlabTensionSize==0) return 0;
	
	double m_dLHeight = pBridge->m_dHeightLeftTension;
	double m_dRHeight = pBridge->m_dHeightRightTension;

	CDPoint A[HDANLINESU];
	typedef struct _TENDON
	{		
		CDPoint xyStt;
		CDPoint xyEnd;
		BOOL    bLine;
		double dR;
		double dRad1;
		double dRad2;
		CDPoint xyCen;
	} TENDON;

	long nTENDTotal = pBridge->m_nSlabTensionSize*2+1;
	TENDON *TEND = new TENDON[nTENDTotal];
	
	pBridge->GetXyPaveUpperDom(pBx, A);	
	double dLDist = pBridge->GetSlabLeft()->m_dGirderCenter;
	double dRDist = pBridge->GetSlabRight()->m_dGirderCenter;
	CDPoint ptStt = CDPoint(A[0].x, pBridge->GetElevationPaveDomByDis(pBx, dLDist) - pBridge->m_dThickPave - m_dLHeight);	
	CDPoint ptEnd = CDPoint(A[pBridge->GetQtyHDanNode()].x,pBridge->GetElevationPaveDomByDis(pBx, dRDist) - pBridge->m_dThickPave - m_dRHeight);	

	CDPoint xyResult1, xyResult2, xyResult3, xyResult4;
	CDPoint pt1ForVec, pt2ForVec, pt1ForVec2, pt2ForVec2;
	CDPoint vec, vec2;
	CDPoint ptNext, ptNext2;
	CDPoint ptCenter, ptCenter2;
	CDPoint xyMatch1, xyMatch2;
	double dDistTotal=0;	
	long   nTendon   = 0;
	CDPoint vY(0,1);
	long n = 0;
	for(n=0; n<pBridge->m_nSlabTensionSize; n++)
	{
		double dL = pBridge->m_SlabTension[n].m_dL;
		double dR = pBridge->m_SlabTension[n].m_dR;
		double dH = pBridge->m_SlabTension[n].m_dH;
		long nT = pBridge->m_SlabTension[n].m_nType;
		dDistTotal += dL;

		pt1ForVec = CDPoint(ptStt.x + dDistTotal-5, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal-5));
		pt2ForVec = CDPoint(ptStt.x + dDistTotal+5, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal+5));
		vec = (pt2ForVec - pt1ForVec).Unit();
		
		ptNext = CDPoint(ptStt.x + dDistTotal, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal) - pBridge->m_dThickPave - dH);		
		ptCenter = ptNext + (nT==0 ? vec.RotateInv90() : vec.Rotate90()) * dR;				

		if(n==0)
		{
			if(GetTangentCircle(ptStt, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
//				pDom->LineTo(ptStt, xyMatch1);
				TEND[nTendon].xyStt = ptStt;
				TEND[nTendon].xyEnd = xyMatch1;
				TEND[nTendon].bLine = TRUE;
				TEND[nTendon].dR    = 0;
				TEND[nTendon].dRad1 = 0;
				TEND[nTendon].dRad2 = 0;
				TEND[nTendon++].xyCen = CDPoint(0,0);
			}
		}
		else
		{
			if(GetTangentCircle(xyMatch2, ptCenter, dR, xyResult1, xyResult2)==2)
			{
				if(nT==0)
					xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				else
					xyMatch1 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
//				pDom->LineTo(xyMatch2, xyMatch1);
				TEND[nTendon].xyStt = xyMatch2;
				TEND[nTendon].xyEnd = xyMatch1;
				TEND[nTendon].bLine = TRUE;
				TEND[nTendon].dR    = 0;
				TEND[nTendon].dRad1 = 0;
				TEND[nTendon].dRad2 = 0;
				TEND[nTendon++].xyCen = CDPoint(0,0);
			}
		}

		if(n!=pBridge->m_nSlabTensionSize-1)
		{
			double dL2 = pBridge->m_SlabTension[n+1].m_dL;
			double dR2 = pBridge->m_SlabTension[n+1].m_dR;
			double dH2 = pBridge->m_SlabTension[n+1].m_dH;
			long nT2 = pBridge->m_SlabTension[n+1].m_nType;

			pt1ForVec2 = CDPoint(ptStt.x + dDistTotal+dL2-5, pBridge->GetElevationPaveDomByDis(pBx, dLDist+dDistTotal+dL2-5));
			pt2ForVec2 = CDPoint(ptStt.x + dDistTotal+dL2+5, pBridge->GetElevationPaveDomByDis(pBx, dLDist+dDistTotal+dL2+5));
			vec2 = (pt2ForVec2 - pt1ForVec2).Unit();
			
			ptNext2 = CDPoint(ptStt.x + dDistTotal+dL2, pBridge->GetElevationPaveDomByDis(pBx, dLDist + dDistTotal + dL2) - pBridge->m_dThickPave - dH2);			
			ptCenter2 = ptNext2 + (nT2==0 ? vec2.RotateInv90() : vec2.Rotate90()) * dR2;			
			
			if(GetTangentCircleAndCircle(ptCenter, dR, ptCenter2, dR2, xyResult1, xyResult2, xyResult3, xyResult4))
			{		
				xyMatch2 = xyResult1;
				xyMatch2 = ~(xyResult2 - xyMatch1) < ~(xyMatch2 - xyMatch1) ? xyResult2 : xyMatch2;
				// 1 ������ ã�ƾ� �ϹǷ�....
				//xyMatch2 = ~(xyResult3 - xyMatch1) < ~(xyMatch2 - xyMatch1) ? xyResult3 : xyMatch2;
				//xyMatch2 = ~(xyResult4 - xyMatch1) < ~(xyMatch2 - xyMatch1) ? xyResult4 : xyMatch2;				
			}
			else
			{
				//ASSERT(FALSE);
				xyMatch2 = xyMatch1;
			}

			double dAngle1 = (xyMatch1 - ptCenter).Unit().GetAngleDegree();

//			pDom->Arc(ptCenter, dR, dAngle1, dAngle2);		
			TEND[nTendon].xyStt = xyMatch1;
			TEND[nTendon].xyEnd = xyMatch2;
			TEND[nTendon].bLine = FALSE;
			TEND[nTendon].dR    = dR;
			TEND[nTendon].dRad1 = ToRadian(dAngle1);
			TEND[nTendon].dRad2 = ToRadian((xyMatch2 - ptCenter).Unit().GetAngleDegree());
			TEND[nTendon++].xyCen = ptCenter;	
		}
		else
		{
			if(GetTangentCircle(ptEnd, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch2 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
//				pDom->LineTo(xyMatch2, ptEnd);
				TEND[nTendon].xyStt = xyMatch2;
				TEND[nTendon].xyEnd = ptEnd;
				TEND[nTendon].bLine = TRUE;
				TEND[nTendon].dR    = 0;
				TEND[nTendon].dRad1 = 0;
				TEND[nTendon].dRad2 = 0;
				TEND[nTendon++].xyCen = CDPoint(0,0);
			}

			double dAngle1 = (xyMatch1 - ptCenter).Unit().GetAngleDegree();

//			pDom->Arc(ptCenter, dR, dAngle1, dAngle2);							
			TEND[nTendon].xyStt = xyMatch1;
			TEND[nTendon].xyEnd = xyMatch2;
			TEND[nTendon].bLine = FALSE;
			TEND[nTendon].dR    = dR;
			TEND[nTendon].dRad1 = ToRadian(dAngle1);
			TEND[nTendon].dRad2 = ToRadian((xyMatch2 - ptCenter).Unit().GetAngleDegree());
			TEND[nTendon++].xyCen = ptCenter;	
		}
	}

	CDPoint xyCutting, xyAnswer;
	double dTGir     = pBridge->GetGirder(0)->GetThickFactByStation(pBx->GetStation(), G_W);
	double dSharpGir = pBridge->GetGirder(0)->GetLengthSharpOnBx(pBx, TRUE);
// nSide = 0 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide = 1 : ������ �������ܿ��� ����ĵƿ���� ������ġ����
// nSide = 2 : ������ �������ܿ��� ������ �߽ɱ���
	xyCutting.y = A[0].y; 
	if(nSide==0)	
		xyCutting.x = A[0].x + pBridge->GetWidthSlabLeft() - dTGir/2 - dSharpGir/2;
	else if(nSide==1)
	{
		dTGir       = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetThickFactByStation(pBx->GetStation(), G_W);
		dSharpGir   = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetLengthSharpOnBx(pBx, TRUE);
		xyCutting.x = A[pBridge->GetQtyHDanNode()].x - pBridge->GetWidthSlabRight() + dTGir/2 + dSharpGir/2;
	}
	else if(nSide==2)
		xyCutting.x = A[0].x + fabs(pBridge->GetSlabRight()->m_dGirderCenter-pBridge->GetSlabLeft()->m_dGirderCenter)/2;

	double dRadianTotal = 0;
	CDPoint vX(1,0);
	for(n=0; n<nTENDTotal; n++)
	{
		TENDON Tendon = TEND[n];
		if(Tendon.bLine==TRUE)		// ����
		{
			if(GetXyMatchSegAndLine(Tendon.xyStt, Tendon.xyEnd, xyCutting, vY, xyMatch1))		// ������
			{
				CDPoint vAngle = (Tendon.xyEnd-Tendon.xyStt).Unit();
				if(vAngle.y > 0)
					dRadianTotal += vX.BetweenAngle(vAngle).GetAngleRadian();
				else
					dRadianTotal += vAngle.BetweenAngle(vX).GetAngleRadian();
				break;
			}
			else
			{
				CDPoint vAngle = (Tendon.xyEnd-Tendon.xyStt).Unit();
				if(vAngle.y > 0)
					dRadianTotal += vX.BetweenAngle(vAngle).GetAngleRadian()*(n>0 ? 2 : 1);
				else
					dRadianTotal += vAngle.BetweenAngle(vX).GetAngleRadian()*(n>0 ? 2 : 1);
			}
		}
		else						// �
		{
			if(GetXyMatchLineAndArc(xyCutting, vY, Tendon.xyCen, Tendon.dR, min(Tendon.dRad1, Tendon.dRad2), max(Tendon.dRad1, Tendon.dRad2), xyMatch1,xyMatch2) > 0)
				break;
		}
	}
	
	delete[] TEND;

	return dRadianTotal;
}

///< ������Ʈ������ �ս�(���ش��� N mm)
void CCalcFloor::CalcPreStress(CMap <CString, LPCTSTR, double, double>& Map, long nSide)
{
	CPlateBridgeApp		*pDB = m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();

	m_MapPreStress.clear();
	CDPoint	xy(0,0);
	CDPoint xX(1,0), xY(0,1);
	double	dLSlabThick			= pDB->m_dThickSlabBase;//pDB->m_dThickSlabLeft;		// ���� ������ �β�
	double	dRSlabThick			= pDB->m_dThickSlabBase;//pDB->m_dThickSlabRight;		// ���� ������ �β�
	double	dCSlabThick			= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;		///< �߾Ӻ� ������ �β�
	double	dSlabThick			= 0;		///< nSide�� ������ �β�
	double	dI					= 0;
	double	dAreaSlab			= 0;

	double	dLI				= pow(dLSlabThick,3)/12*1000;		// ���� ������ ����
	double	dRI				= pow(dRSlabThick,3)/12*1000;		// ���� ������ ����
	double	dCI				= pow(dCSlabThick,3)/12*1000;		// ���� ������ ����
	double	dAreaInner		= dCSlabThick*1000;					// ���� ������ ����
	double	dLSlabArea		= dLSlabThick*1000;					// ���� ������ ����
	double	dRSlabArea		= dRSlabThick*1000;					// ���� ������ ����
	double	dZInner			= dCSlabThick*dCSlabThick/6*1000;	// ���� ������ ����
	double	dZStt			= dLSlabThick*dLSlabThick/6*1000;	// ���� ������ ����
	double	dZEnd			= dRSlabThick*dRSlabThick/6*1000;	// ���� ������ ����
	double	dZ				= 0;
	double	dHeightSlabToPS	= 0;

	///< ���߰��
	double	dPreDeadLoad	= CalcCantileverPreDeadLoad(nSide);		///< �¿��� �ռ��� ��������
	double	dPostDeadLoad	= CalcCantileverPostDeadLoad(nSide);		///< �¿��� �ռ��� ��������
	double	dLiveMoment		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml;	///< Ȱ����
	double	dWindMoment		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_Mw;	///< ǳ����
	double	dCollMoment		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Mo;	///< �浹����

	if(nSide == FLOOR_LEFT)
	{
		dSlabThick		= dLSlabThick;
		dI				= dLI;
		dAreaSlab		= dLSlabArea;
		dZ				= dZStt;
		dHeightSlabToPS = dSlabThick - (pDB->m_SlabTension[0].m_dH);	// ������ ��ܿ��� �������� �Ÿ�
	}
	else if(nSide == FLOOR_RIGHT)
	{
		dSlabThick		= dRSlabThick;
		dI				= dRI;
		dAreaSlab		= dRSlabArea;
		dZ				= dZEnd;
		dHeightSlabToPS = dSlabThick - (pDB->m_SlabTension[2].m_dH);
	}
	else if(nSide == FLOOR_MID)
	{
		dSlabThick		= dCSlabThick;
		dI				= dCI;
		dAreaSlab		= dAreaInner;
		dLiveMoment		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml;	///< Ȱ����
		dZ				= dZInner;
		dHeightSlabToPS = (pDB->m_SlabTension[1].m_dH);
		dWindMoment		= 0;//ǳ���� ��� ������
	}

	double	dDesignLoad			= dPreDeadLoad + dPostDeadLoad + dLiveMoment + dWindMoment + dCollMoment;///< ��������
	double	dQty				= 1000.0/pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	double	dPrestressEs		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	double	dEc					= pCalcData->DESIGN_MATERIAL.m_dEc;
	double	dTensionStrength	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength;
	double	dYieldStrength		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;
	double	dHr					= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity;
	double	dX					= CalcPrestressLength(nSide);
	double	dAlpha				= CalcPrestressTotAngle(nSide);
	double	dCurvatureFriction	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction;	///< ��� ���� ���
	double	dWobbleFriction		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction;		///< �Ļ� ���� ���
	long	nPSQty				= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	double	dArea				= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea*nPSQty;
	double	dkxua				= toM(dX)*dWobbleFriction + dAlpha*dCurvatureFriction;	
	double	dAnchorageSlip		= (pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip);
	double	dAllowTension		= min(0.8*dTensionStrength, 0.9*dYieldStrength);
	double	dFpf				= dkxua*dAllowTension;
	double	dP					= (dAllowTension*dArea*dQty-(dAllowTension-dFpf)*dArea*dQty)/dX;
	double	dTemp				= dAnchorageSlip*dArea*dQty*dPrestressEs/dP;
	double	dLset				= sqrt(dTemp);
	double	dDeltaP				= 2*dP*dLset;
	double	dPi					= (dAllowTension*dArea*dQty)-dDeltaP;
	double	dFpi				= dPi/(dArea*dQty);
	double	dPi_Cen				= dPi + dP*dX;
	double	dFpi_Cen			= dPi_Cen/(dArea*dQty);
	double	dFan_Cen			= dAllowTension - dFpi;
	double	dPic				= (dPi+dP*(dX));
	double	dFan				= ((dAllowTension-dFpf)*dArea*dQty-dPic)/(dArea*dQty);
	if(dLset<dX)	dFan = 0;
	double	dFpt1				= dAllowTension - dFpf - dFan;
	double	dPt					= dFpt1*dQty*dArea;
	double	dLength				= ABS(dSlabThick/2 - dHeightSlabToPS);	///< ������ �߽ɿ��� PS���������� �Ÿ�
	double	dFci				= dPt/dAreaSlab + dPt*dLength*dLength/dI - dPreDeadLoad/dI*dLength;
	double	dFpel				= 0.5*dPrestressEs/dEc*dFci;
	double	dFpsh				= 0.8*(119-1.05*dHr);
	double	dFcds				= dPostDeadLoad/dZ;
	double	dFpcr				= 12*dFci-7*dFcds;
	double	dFpr				= 35 - 0.07*dFpf - 0.1*dFpel - 0.05*(dFpsh+dFpcr);
	double	dFpt				= dAllowTension-dFpf-dFan-dFpel;	///< PS������ ��ȿ�����
	double	dFpe				= dFpt - dFpsh - dFpcr - dFpr;
	double	dPtl				= dFpt * dArea * dQty;				///< PS������ �����
	double	dMt					= dPtl * dLength;				///< PS���翡���� ���Ʈ
	double	dPtd				= dFpe * dArea * dQty;				///< PS������ �����
	double	dMtd				= dPtd * dLength;					///< PS���翡���� ���Ʈ
	double	dFt_Up				= 0;
	double	dFt_Lo				= 0;
	double	dFtd_Up				= 0;
	double	dFtd_Lo				= 0;
	double	dy					= dSlabThick/2;
	if(nSide == FLOOR_MID)
	{
		dFt_Up	= dPtl/dAreaSlab - dMt/dI*dy + dPreDeadLoad/dI*dy;///< ������Ʈ���� ���Խ� ������
		dFt_Lo	= dPtl/dAreaSlab + dMt/dI*dy - dPreDeadLoad/dI*dy;///< ������Ʈ���� ���Խ� �Ͽ�����
		dFtd_Up	= dPtd/dAreaSlab - dMtd/dI*dy + dDesignLoad/dI*dy;///< �������� �ۿ���� ��ũ��Ʈ �� ����
		dFtd_Lo	= dPtd/dAreaSlab + dMtd/dI*dy - dDesignLoad/dI*dy;///< �������� �ۿ���� ��ũ��Ʈ �Ͽ� ����
	}
	else if(nSide == FLOOR_LEFT || nSide == FLOOR_RIGHT)
	{
		dFt_Up	= dPtl/dAreaSlab + dMt/dI*dy - dPreDeadLoad/dI*dy;///< ������Ʈ���� ���Խ� ������
		dFt_Lo	= dPtl/dAreaSlab - dMt/dI*dy + dPreDeadLoad/dI*dy;///< ������Ʈ���� ���Խ� �Ͽ�����
		dFtd_Up	= dPtd/dAreaSlab + dMtd/dI*dy - dDesignLoad/dI*dy;///< �������� �ۿ���� ��ũ��Ʈ �� ����
		dFtd_Lo	= dPtd/dAreaSlab - dMtd/dI*dy + dDesignLoad/dI*dy;///< �������� �ۿ���� ��ũ��Ʈ �Ͽ� ����
	}

	Map["AllowTension"]			= dAllowTension;
	Map["MomentPreDeadLoad"]	= dPreDeadLoad;
	Map["MomentPostDeadLoad"]	= dPostDeadLoad;
	Map["Fpf"]					= dFpf;
	Map["PSUpperLoad"]			= dFt_Up;
	Map["PSLowerLoad"]			= dFt_Lo;
	Map["PSUpperLoadD"]			= dFtd_Up;
	Map["PSLowerLoadD"]			= dFtd_Lo;
	Map["Mtd"]					= dMtd;
	Map["Ptd"]					= dPtd;
	Map["SlabThick_Inner"]		= dCSlabThick;
	Map["SlabThick_Stt"]		= dLSlabThick;
	Map["SlabThick_End"]		= dRSlabThick;
	Map["LSlab_Area"]			= dLSlabArea;
	Map["RLSlab_Area"]			= dRSlabArea;
	Map["X"]					= dX;
	Map["Alpha"]				= dAlpha;
	Map["KXUA"]					= dkxua;
	Map["QTY"]					= dQty;
	Map["I_Stt"]				= dLI;
	Map["I_End"]				= dRI;
	Map["I_Inner"]				= dCI;
	Map["I"]					= dI;
	Map["P"]					= dP;
	Map["Lset"]					= dLset;
	Map["DeltaP"]				= dDeltaP;
	Map["Pi"]					= dPi;
	Map["Fpi"]					= dFpi;
	Map["Fan"]					= dFan;
	Map["Pi_Cen"]				= dPi_Cen;
	Map["Fpi_Cen"]				= dFpi_Cen;
	Map["Fan_Cen"]				= dFan_Cen;
	Map["Fpt1"]					= dFpt1;
	Map["Pt"]					= dPt;
	Map["Slab_Area"]			= dAreaSlab;
	Map["Fci"]					= dFci;
	Map["Slab_Thick"]			= dSlabThick;
	Map["Length_SlabToPS"]		= dHeightSlabToPS;
	Map["Fpel"]					= dFpel;
	Map["Fpt"]					= dFpt;
	Map["Length"]				= dLength;
	Map["Fpsh"]					= dFpsh;
	Map["Z"]					= dZ;
	Map["Fcds"]					= dFcds;
	Map["Fpcr"]					= dFpcr;
	Map["Fpr"]					= dFpr;
	Map["Fpe"]					= dFpe;
	Map["Ptl"]					= dPtl;
	Map["Mt"]					= dMt;
	Map["DesignLoad"]			= dDesignLoad;
	Map["dTensionStrength"]		= dTensionStrength;
	Map["dYieldStrength"]		= dYieldStrength;
	Map["dArea_p"]				= dArea;
}

long CCalcFloor::GetTangentCircle(CDPoint xyPoint, CDPoint xyCircleCen, double dCircleR, CDPoint& xyResult1, CDPoint& xyResult2)
{
	double d = ~(xyPoint - xyCircleCen);
	if (d < dCircleR)
		return 0;
	double d1 = dCircleR*dCircleR / d;//KSG
	double h  = sqrt(dCircleR*dCircleR - d1*d1);
	
	CDPoint vP = (xyPoint - xyCircleCen).Unit();
	CDPoint N = xyCircleCen + vP * d1;
	xyResult1 = N + vP.RotateInv90() * h;         // ù��° �� ����.
	xyResult2 = N + vP.Rotate90() * h;         // �ι�° �� ����.
	if (h == 0)
		return 1;
	return 2;
}

void CCalcFloor::GetTransPointDistance(CDPointArray& xyArr, CDoubleArray& AngArr)
{
	CPlateBridgeApp	*pBridge	=	m_pDataManage->GetBridge();	
	CPlateBasicIndex *pBx		= pBridge->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	xyArr.RemoveAll();
	AngArr.RemoveAll();

	if(pBridge->m_nSlabTensionSize==0) return;
	double m_dLHeight	=	pBridge->m_dHeightLeftTension;
	double m_dRHeight	=	pBridge->m_dHeightRightTension;
	CDPoint A[HDANLINESU];
	pBridge->GetXyPaveDom(pBx, A);	//?

	double dLDist	=	pBridge->GetSlabLeft()->m_dGirderCenter;
	double dRDist	=	pBridge->GetSlabRight()->m_dGirderCenter;
	double dX1, dX2, dY1, dY2;


	CDPoint ptStt		=	CDPoint(A[0].x, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist) - (pBridge->m_dThickPave + m_dLHeight));
	CDPoint ptSttUnit	=	ptStt;	
	CDPoint ptEnd		=	CDPoint(A[pBridge->GetQtyHDanDom(pBx)].x,pBridge->GetElevationPaveActByDis(pBx->GetStation(), dRDist) - (pBridge->m_dThickPave + m_dRHeight));
	CDPoint ptEndUnit	=	ptEnd;

	CDPoint xyResult1,	xyResult2, xyResult3, xyResult4, xyPoint1, xyPoint2;
	CDPoint ptSttTemp,	ptEndTemp;	
	CDPoint ptCen1,		ptCen2,	ptCen3;
	CDPoint ptCenter,	ptCenter2;
	CDPoint xyMatch1,	xyMatch2;	
	double	dDistTotal	=	0;
	BOOL	bLeftFix	= (pBridge->m_nTendonFix != 1) ? TRUE : FALSE;	// �������� ����&������� : TRUE, ���� : FALSE	
	long	n = 0;
	for (n=0; n<pBridge->m_nSlabTensionSize; n++)
	{
		double	dL	=	pBridge->m_SlabTension[n].m_dL;
		double	dR	=	pBridge->m_SlabTension[n].m_dR;
		double	dH	=	pBridge->m_SlabTension[n].m_dH;
		long	nT	=	pBridge->m_SlabTension[n].m_nType;

		dDistTotal	+=	dL;

		if(n==0)	ptCen1	=	ptStt;	
		else		ptCen1	=	ptCen2;

		ptCen2	=	CDPoint(ptStt.x + dDistTotal, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal) - pBridge->m_dThickPave - dH);
		ptCen3	=	CDPoint(ptStt.x + dDistTotal + pBridge->m_SlabTension[n+1].m_dL, pBridge->GetElevationPaveActByDis(pBx->GetStation(), dLDist + dDistTotal+pBridge->m_SlabTension[n + 1].m_dL) - pBridge->m_dThickPave - pBridge->m_SlabTension[n + 1].m_dH);

		if(n==pBridge->m_nSlabTensionSize - 1) 
			ptCen3	=	ptEnd;

		CDPoint vDir1	=	(ptCen1 - ptCen2).Unit();
		CDPoint vDir2	=	(ptCen3 - ptCen2).Unit();
		CDPoint vDir3	=	(vDir1 + vDir2).Unit();

		//���� ������ ���ϱ� - Start
		CDPoint vDir4	= vDir3.BetweenAngle(vDir1);
		double	dRealV	= (360 - vDir4.GetAngleDegree()) - 90;		
		dRealV = ToRadian(dRealV);	
		double dH1		= tan(dRealV)*dR;
		double dRealR	= sqrt(dH1*dH1 + dR*dR);
		
		//���� ������ ���ϱ� - End
		ptCenter	=	ptCen2 + vDir3 * dRealR;
		
		//������ �����κ�
		if(n==0)
		{	
			if(GetTangentCircle(ptStt, ptCenter, dR, xyResult1, xyResult2)==2)
			{				
				xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;				
				dX1	=	bLeftFix ? pBridge->m_dLengthSecMove : pBridge->m_dLengthSecMoveEnd;
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

				double dLengthDuct = bLeftFix  ? pBridge->m_dLengthDuctOffset : 0;
				dX1	=	dLengthDuct;
				dX2 =	xyMatch1.x - ptSttUnit.x;
				dY2 =	xyMatch1.y - ptSttUnit.y;
				dY1 =	(dX1 / dX2) * dY2;
				
				///////////////////////////////////////////////////////////////////////////////////
				// Postioin Setting
				xyPoint1.x = ptSttTemp.x;
				xyPoint1.y = ptSttTemp.y;
				xyArr.Add(xyPoint1);
				
				
				xyPoint2.x = ptSttTemp.x + dLengthDuct;
				xyPoint2.y = ptSttTemp.y + dY1;
				xyArr.Add(xyPoint2);
				
				xyPoint1 = xyPoint2;
				xyPoint2 = xyMatch1;
				xyArr.Add(xyPoint2);
				///////////////////////////////////////////////////////////////////////////////////
			}
		}
		else
		{
			if(GetTangentCircle(xyMatch2, ptCenter, dR, xyResult1, xyResult2)==2)
			{
				if(nT==0)
					xyMatch1 = (xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
				else
					xyMatch1 = (xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;

				///////////////////////////////////////////////////////////////////////////////////
				// Postioin Setting
				xyPoint1 = xyMatch1;
				xyPoint2 = xyMatch2;
				xyArr.Add(xyPoint1);
				///////////////////////////////////////////////////////////////////////////////////
			}			
		}

		if(n!=pBridge->m_nSlabTensionSize)
		{
			
			//� �׸���
			GetTangentCircle(ptCen3, ptCenter, dR, xyResult1, xyResult2);			
			if(nT == 0)
				xyMatch2	=	(xyResult1.y > xyResult2.y) ? xyResult1 : xyResult2;
			else
				xyMatch2	=	(xyResult1.y < xyResult2.y) ? xyResult1 : xyResult2;
						
			///////////////////////////////////////////////////////////////////////////////////
			// Postioin Setting
			xyPoint1 = xyMatch1;
			xyPoint2 = xyMatch2;
			xyArr.Add(xyPoint2);
			///////////////////////////////////////////////////////////////////////////////////
		}
		
		if(n == pBridge->m_nSlabTensionSize-1)
		{
			dX1	=	bLeftFix ? pBridge->m_dLengthSecMoveEnd : pBridge->m_dLengthSecMove;
			dX2 =	xyMatch2.x - ptEnd.x;
			dY2 =	xyMatch2.y - ptEnd.y;			
			dY1 =	(dX1 / dX2) * dY2;			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;
			
			//������ �κ�
			if(bLeftFix)
				ptEndTemp	=	ptEndUnit;
			else
			{
				ptEndTemp.x	=	ptEndUnit.x - pBridge->m_dLengthDuctOffset;
				ptEndTemp.y	=	ptEndUnit.y + pBridge->m_dLengthDuctOffset * (xyMatch2 - ptEndUnit).Unit().y;
			} 
						
			//140�� ������...				
			dX1	=	bLeftFix ? pBridge->m_dLengthSecMoveEnd + 140 : pBridge->m_dLengthSecMove;
			dX2	=	xyMatch2.x - ptEnd.x;
			dY2	=	xyMatch2.y - ptEnd.y;
			dY1	=	(dX1 / dX2) * dY2;
			
			ptEndUnit.x	=	ptEnd.x - dX1;
			ptEndUnit.y	=	ptEnd.y - dY1;
						
			///////////////////////////////////////////////////////////////////////////////////
			// Postioin Setting
			xyPoint1 = xyMatch2;
			xyPoint2 = ptEndUnit;
			xyArr.Add(xyPoint2);
			///////////////////////////////////////////////////////////////////////////////////			
		}
	}
	
	// Position Angle
	for (n=0; n<xyArr.GetSize()-1; n++)
	{
		CDPoint xx = xyArr.GetAt(n);
		CDPoint xyPos = (xyArr.GetAt(n+1) - xyArr.GetAt(n)).Unit();
		double Ang = xyPos.GetAngleRadian();
		double dPi = ConstPi;
		double dAng = (Ang>dPi/2)? dPi*2-Ang : Ang;
		AngArr.Add(dAng);
	}
}

void CCalcFloor::SeekFlange_Web(double& dFlange, double& dWeb)
{
	dFlange = dWeb = 0;
	
	__int64 bxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE;

	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
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

double CCalcFloor::GetDistance(CDPoint& p1, CDPoint& p2)
{
	double dx = p1.x - p2.x;
	double dy = p1.y - p2.y;
	double dis = sqrt(dx*dx + dy*dy);
	return dis;
}

void CCalcFloor::ReSetTendonDistance(CDPointArray& PosArr, long nPos, CDoubleArray& DisArr)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();

	// ������ ��ġ
	BOOL bLeftFix = (pDB->m_nTendonFix != 1) ? TRUE : FALSE;
	
	// �̰ݰŸ�
	double dSecMoveStt = pDB->m_dLengthSecMove;
	double dSecMoveEnd = pDB->m_dLengthSecMoveEnd;

	// Seek Top Flange Width
	double dFlange		= 0;
	double dWeb			= 0;
	SeekFlange_Web(dFlange, dWeb);

	////////////////////////////////////////////////////////////////////////////////////
	// Seek Girder Position Array
	////////////////////////////////////////////////////////////////////////////////////
	CDPoint xyPos = PosArr.GetAt(0);
	CPlateBasicIndex *pBx = pDB->GetGirder(0)->GetCrossBxAtMaxFlangeWidth();
	CDPointArray xyGirArr;
	CDPointArray xyDis;

	xyGirArr.Add(xyPos);

	double dLen = pDB->GetWidthSlabLeft();

	if (bLeftFix)
	{
		dLen -= dSecMoveStt;
		dLen -= dFlange/2;
	}
	else
	{
		dLen -= dSecMoveEnd;
		dLen -= 140;
		dLen -= dFlange/2;
	}

	xyPos.x += dLen;
	xyGirArr.Add(xyPos);

	dLen = 0;
	for (long nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
 		CPlateBasicIndex *pBxMatch = pBx->GetBxMatchByCrossBeam(pDB->GetGirder(0));
		dLen = pDB->GetGirder(nG)->GetDisGirderToGirder(pBxMatch);

		if (nG==0) dLen += dFlange/2;
		if (nG==pDB->GetGirdersu()-2) dLen += dFlange/2;
		
	}
	xyPos.x += dLen/2;
	xyGirArr.Add(xyPos);
	xyPos.x += dLen/2;
	xyGirArr.Add(xyPos);

	dLen = pDB->GetWidthSlabRight();
	if (bLeftFix)
	{
 		dLen -= dSecMoveEnd;
		dLen -= 140;
		dLen -= dFlange/2;
	}
	else
	{
 		dLen -= dSecMoveStt;
		dLen -= dFlange/2;
	}
	xyPos.x += dLen;
	xyGirArr.Add(xyPos);


	// set tendon distance by position 
	CDPoint xyTarget = xyGirArr.GetAt(1);

	switch (nPos)
	{
	case 0:  xyTarget = xyGirArr.GetAt(1);	break;
	case 1:  xyTarget = xyGirArr.GetAt(2);	break;
	case 2:  xyTarget = xyGirArr.GetAt(3);	break;
	}

	long	nCnt = 0;
	
	for (long n=0; n<PosArr.GetSize(); n++)
	{
		xyPos = PosArr.GetAt(n);
		if (xyPos.x < xyTarget.x) continue;
		nCnt++;
		
		if (nCnt==1)
		{
			CDPoint p1 = PosArr.GetAt(n-1);
			CDPoint p2 = xyPos;
			CDPoint Ang = (p2-p1).Unit();
			CDPoint p3;
			
			GetXyMatchLineAndLine(p1, Ang, xyTarget, CDPoint(0,1), p3);
			xyDis.Add(p3);
		}
		xyDis.Add(xyPos);
	}

	// Seek Distance
	DisArr.RemoveAll();
	for (long k=0; k<xyDis.GetSize()-1; k++)
	{
		double Dis = GetDistance(xyDis.GetAt(k), xyDis.GetAt(k+1));
		DisArr.Add(Dis);
	}
}

void CCalcFloor::GetElementIndex_Cantilever(CFEMManage* pFEM, long nSide, CLongArray& nArr)
{
	//-------------------------------------------------------------------------------------------
	// Seek Flange X-Position
	// Top Flange Left, Top Flange Center, Top Flange Right

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
	else if (nSide==1)	// Right Cantilever
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

double CCalcFloor::GetMemberForce(CFEMManage* pFEM, CString szLoad, long nForceType, long nLoadPos)
{
	double dForceMax = 0;

	long nLoadCase = pFEM->GetLoadCaseIndex(szLoad);
	if (nLoadCase<0)
		return dForceMax;

	double	dForce	= 0;

	for(long nEle = 0; nEle<pFEM->GetElementSize(); nEle++)
	{
		CElement	*pElem		= pFEM->GetElement((unsigned short)nEle);
		CResultForceElement	*pResultForce	= pElem->GetResultForce(nLoadCase);
		CJoint		*pJ_Stt		= pFEM->GetJoint(pElem->m_nJStt);
		CJoint		*pJ_End	= pFEM->GetJoint(pElem->m_nJEnd);

		long	nTypeStt	= pJ_Stt->m_LongAttrib[CFemCantileverCalc::JOINT_SEC];
		long	nTypeEnd	= pJ_End->m_LongAttrib[CFemCantileverCalc::JOINT_SEC];
		long	nSegSize	= pResultForce->m_ResultForceElement.GetSize();
		BOOL bIsPosition	= FALSE;

		if(nLoadPos == CFemCantileverCalc::JOINT_LEFT_CANTILEVER && nTypeEnd == CFemCantileverCalc::JOINT_LEFT_CANTILEVER)
			bIsPosition = TRUE;
		if(nLoadPos == CFemCantileverCalc::JOINT_RIGHT_CANTILEVER && nTypeStt == CFemCantileverCalc::JOINT_LEFT_CANTILEVER)
			bIsPosition = TRUE;
		if(nLoadPos == CFemCantileverCalc::JOINT_SLAB_CENT && nTypeStt == CFemCantileverCalc::JOINT_SLAB_CENT)
			bIsPosition = TRUE;
		if(nLoadPos == CFemCantileverCalc::JOINT_SLAB_DANBU && nTypeStt == CFemCantileverCalc::JOINT_SLAB_DANBU)
			bIsPosition = TRUE;

		if(bIsPosition == FALSE)
			continue;

		for(long nSeg = 0; nSeg<nSegSize; nSeg++)
		{
			dForce	= pResultForce->m_ResultForceElement[nSeg]->m_dForceOnElement[nForceType];
			if (fabs(dForceMax) < fabs(dForce))	
				dForceMax = dForce;
		}
	}

	return dForceMax;

	//CLongArray AddIdx;
	//GetElementIndex_Cantilever(pFEM, nSide, AddIdx);
	//
	//CResultForceElement* pResult = NULL;
	//
	//for (long n=0; n<AddIdx.GetSize(); n++)
	//{
	//	long nElem = AddIdx.GetAt(n);
	//	CElement* pElem = pFEM->GetElement((unsigned short)nElem);
	//	pResult = pElem->GetResultForce(nLoadCase);
	//	if (!pResult) return 0;

	//	long nFirst = 0;
	//	long nLast  = pResult->m_ResultForceElement.GetSize()-1;
	//	
	//	double dForce1 = pResult->m_ResultForceElement[nFirst]->m_dForceOnElement[nType];
	//	double dForce2 = pResult->m_ResultForceElement[nLast]->m_dForceOnElement[nType];

	//	if (nSide==0 || nSide==1)
	//	{
	//		if (fabs(dForce)<fabs(dForce1)) dForce = dForce1;
	//		if (fabs(dForce)<fabs(dForce2)) dForce = dForce2;
	//		
	//	}
	//	else
	//	{
	//		if (!bEnd)
	//		{
	//			if (n%2==0) if (fabs(dForce)<fabs(dForce2)) dForce = dForce2;
	//			if (n%2!=0) if (fabs(dForce)<fabs(dForce1)) dForce = dForce1;
	//		}
	//		else
	//		{
	//			if (n%2==0) if (fabs(dForce)<fabs(dForce1)) dForce = dForce1;
	//			if (n%2!=0) if (fabs(dForce)<fabs(dForce2)) dForce = dForce2;
	//		}
	//	}
	//}

	//return dForce;
}

///<������Ʈ���� �ս�> 2009.07.20 �ں���(���ش��� N mm)
void CCalcFloor::CalcPreStressLoss(CAPlateCalcStd* pCalc, CMap <CString, LPCTSTR, double, double>& Map, long nSide)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	if(pCalc->GetSizeCantileverFemData() == 0)	return;
	CFEMManage			*pFEM		= pCalc->GetCantileverFemData(0);	

	
	//---------------------------------------------------------------------------------------
	// 1. �ܸ����� ����
	// 1.1 Step1 �Ѵܸ�
	double dWidth = 1000;				// ������ ���� �� (1m)
	double dHeight[3] = { 0, 0, 0 };	// ������ �β� (����, ����, �߾�)
	dHeight[0] = pDB->m_dThickSlabBase;
	dHeight[1] = pDB->m_dThickSlabBase;
	dHeight[2] = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;

	double dArea1[3] = { 0, 0, 0 };	// ������ ���� (����, ����, �߾�)
	dArea1[0] = dWidth * dHeight[0];
	dArea1[1] = dWidth * dHeight[1];
	dArea1[2] = dWidth * dHeight[2];

	double dY[3] = { 0, 0, 0 };	// ������ �߽ɰŸ� (����, ����, �߾�)
	dY[0] = dHeight[0] / 2;
	dY[1] = dHeight[1] / 2;
	dY[2] = dHeight[2] / 2;

	double dAy[3] = { 0, 0, 0 };	// �ܸ�1�� ���Ʈ (����, ����, �߾�)
	dAy[0] = dArea1[0] * dY[0];
	dAy[1] = dArea1[1] * dY[1];
	dAy[2] = dArea1[2] * dY[2];

	double dYb1[3] = { 0, 0, 0 };	// ����� ������ �߽ɰŸ�  (����, ����, �߾�)
	dYb1[0] = dAy[0] / dArea1[0];
	dYb1[1] = dAy[1] / dArea1[1];
	dYb1[2] = dAy[2] / dArea1[2];

	double dIx[3] = { 0, 0, 0 };		// �ܸ�2�� ���Ʈ (����, ����, �߾�)
	dIx[0] = dWidth * pow(dHeight[0], 3) / 12;
	dIx[1] = dWidth * pow(dHeight[1], 3) / 12;
	dIx[2] = dWidth * pow(dHeight[2], 3) / 12;

	//---------------------------------------------------------------------------------------
	// 1.2 Step2 ���ܸ�
	long nSheath = 0;					// ������ ����
	double dArea_Sheath = 0;			// ����������

	nSheath =(long) (dWidth / pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC);
	dArea_Sheath = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dSheathH*pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dSheathW;
	dArea_Sheath *= nSheath;

	double dY_Sheath[3] = { 0, 0, 0 };	// ���� �߽ɰŸ�  (����, ����, �߾�)
	dY_Sheath[0] = dHeight[0] - pDB->m_dHeightLeftTension;
	dY_Sheath[1] = dHeight[1] - pDB->m_dHeightRightTension;
	dY_Sheath[2] = dHeight[2] - pDB->GetStdMaxTendonHeight();

	dAy[0] -= dArea_Sheath*dY_Sheath[0];
	dAy[1] -= dArea_Sheath*dY_Sheath[1];
	dAy[2] -= dArea_Sheath*dY_Sheath[2];

	double dArea2[3] = { 0, 0, 0 };	// ������ ���� (����, ����, �߾�)	
	dArea2[0] = dArea1[0] - dArea_Sheath;
	dArea2[1] = dArea1[1] - dArea_Sheath;
	dArea2[2] = dArea1[2] - dArea_Sheath;

	double dYb2[3] = { 0, 0, 0 };	// ����� ������ �߽ɰŸ�  (����, ����, �߾�)
	dYb2[0] = dAy[0] / dArea2[0];
	dYb2[1] = dAy[1] / dArea2[1];
	dYb2[2] = dAy[2] / dArea2[2];

	double dY_e1[3] = { 0, 0, 0 };	// ��ɰŸ�1  (����, ����, �߾�)
	dY_e1[0] = dYb2[0] - dYb1[0]; 
	dY_e1[1] = dYb2[1] - dYb1[1]; 
	dY_e1[2] = dYb2[2] - dYb1[2];

	double dY_e2[3] = { 0, 0, 0 };	// ��ɰŸ�2  (����, ����, �߾�)
	dY_e2[0] = dYb2[0] - dY_Sheath[0];
	dY_e2[1] = dYb2[1] - dY_Sheath[1];
	dY_e2[2] = dYb2[2] - dY_Sheath[2];
	
	double dI_1[3] = { 0, 0, 0 };	// ��ɿ� ���� �ܸ�2�� ���Ʈ 1  (����, ����, �߾�)
	double dI_2[3] = { 0, 0, 0 };	// ��ɿ� ���� �ܸ�2�� ���Ʈ 2  (����, ����, �߾�)
	dI_1[0] = dArea1[0] * dY_e1[0] * dY_e1[0];
	dI_1[1] = dArea1[1] * dY_e1[0] * dY_e1[0];
	dI_1[2] = dArea1[2] * dY_e1[2] * dY_e1[2];

	dI_2[0] = -dArea_Sheath * dY_e2[0] * dY_e2[0];
	dI_2[1] = -dArea_Sheath * dY_e2[1] * dY_e2[1];
	dI_2[2] = -dArea_Sheath * dY_e2[2] * dY_e2[2];

	dIx[0] += dI_1[0] + dI_2[0];
	dIx[1] += dI_1[1] + dI_2[1];
	dIx[2] += dI_1[2] + dI_2[2];	
	
	//---------------------------------------------------------------------------------------
	// 1.3 Step3 PS ������ ȯ��ܸ�
	long	nTendon	= 0;			// ������ ����
	double	dArea_Tendon = 0;		// ������ �ܸ���
	double  dnp	= 0;				// ����ġ ����

	nTendon = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	dArea_Tendon = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea;
	dnp = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs / pCalcData->DESIGN_MATERIAL.m_dEc;
	dArea_Tendon *= nTendon * dnp * nSheath;

	dAy[0] += dArea_Tendon*dY_Sheath[0];
	dAy[1] += dArea_Tendon*dY_Sheath[1];
	dAy[2] += dArea_Tendon*dY_Sheath[2];

	double dArea3[3] = { 0, 0, 0 };	// ������ ���� (����, ����, �߾�)	
	dArea3[0] = dArea2[0] + dArea_Tendon;
	dArea3[1] = dArea2[1] + dArea_Tendon;
	dArea3[2] = dArea2[2] + dArea_Tendon;

	double dYb3[3] = { 0, 0, 0 };	// ����� ������ �߽ɰŸ�  (����, ����, �߾�)
	dYb3[0] = dAy[0] / dArea3[0];
	dYb3[1] = dAy[1] / dArea3[1];
	dYb3[2] = dAy[2] / dArea3[2];

	dY_e1[0] = dYb3[0] - dYb2[0]; 
	dY_e1[1] = dYb3[1] - dYb2[1]; 
	dY_e1[2] = dYb3[2] - dYb2[2];
	
	dY_e2[0] = dYb3[0] - dY_Sheath[0];
	dY_e2[1] = dYb3[1] - dY_Sheath[1];
	dY_e2[2] = dYb3[2] - dY_Sheath[2];

	dI_1[0] = dArea2[0] * dY_e1[0] * dY_e1[0];
	dI_1[1] = dArea2[1] * dY_e1[0] * dY_e1[0];
	dI_1[2] = dArea2[2] * dY_e1[2] * dY_e1[2];
	
	dI_2[0] = dArea_Tendon * dY_e2[0] * dY_e2[0];
	dI_2[1] = dArea_Tendon * dY_e2[1] * dY_e2[1];
	dI_2[2] = dArea_Tendon * dY_e2[2] * dY_e2[2];

	dIx[0] += dI_1[0] + dI_2[0];
	dIx[1] += dI_1[1] + dI_2[1];
	dIx[2] += dI_1[2] + dI_2[2];

	double dZb[3] = { 0, 0, 0 };	// �ܸ�ݰ� (����, ����, �߾�)
	dZb[0] = dIx[0] / dYb3[0];
	dZb[1] = dIx[1] / dYb3[1];
	dZb[2] = dIx[2] / dYb3[2];

	double dep[3] = { 0, 0, 0 };	// �������� ��ɰŸ� (����, ����, �߾�)
	dep[0] = fabs(dYb3[0] - dY_Sheath[0]);
	dep[1] = fabs(dYb3[1] - dY_Sheath[1]);
	dep[2] = fabs(dYb3[2] - dY_Sheath[2]);

	//---------------------------------------------------------------------------------------
	// 2. PS���� ����
	double	dTendon_Dia		= 0;		// ������ ����
	double	dTendon_Area	= 0;		// ������ ����
	long	nTendon_Qty		= 0;		// ������ ����
	
	dTendon_Dia = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dDiameter;
	dTendon_Area= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea;
	nTendon_Qty = nTendon;

	//---------------------------------------------------------------------------------------
	// 3. ������Ʈ���� ���� ���� ����
	double dF_J = 0;	// ��������
	double dL_F = 0;	// �����ս�
	double dL_A = 0;	// Ȱ���ս�
	double dL_E = 0;	// ź���ս�
	double dL_T = 0;	// �Ѽս�
	double dF_T = 0;	// �ѱ�������

	// ��������
	CString szVal = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax;
	
	dF_J = pCalcData->GetJackMaxStrToDouble(szVal);

	// �����ս�
	long nIdx = (nSide==1)? 2: (nSide==2)? 1: 0;
	CDoubleArray dAngArr;
	CDoubleArray ReDisArr;
	CDPointArray xyPosArr;
	double dFactor_Fric	= 0;	// ��� ���� ���
	double dTotal_Alpha = 0;	// �����ܿ��� ������ x������ PS ������ �� �� ��ȭ
	double dTotal_X		= 0;	// �Ļ� ���� ���
	double dLength_End	= 0;	// ��������κ��� ������ x������ �������� ����

	dFactor_Fric	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dCurvatureFriction;
	dTotal_X		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dWobbleFriction;
	GetTransPointDistance(xyPosArr, dAngArr);
	ReSetTendonDistance(xyPosArr, nIdx, ReDisArr);

	long nResize = dAngArr.GetSize() - ReDisArr.GetSize();
	for (long n=0; n<ReDisArr.GetSize(); n++)
	{
		dTotal_Alpha += dAngArr.GetAt(n+nResize);
		dLength_End  += ReDisArr.GetAt(n);
	}
	dL_F = (dTotal_X*dLength_End/1000. + dFactor_Fric*dTotal_Alpha) * dF_J;

	// Ȱ���ս�
	double dSlip	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageSlip;
	if (nSide==0 && pDB->m_nTendonFix<1) dSlip = 0;
	if (nSide==1 && pDB->m_nTendonFix==1) dSlip = 0;

	double dPc		= (dSlip==0)? 0 : dF_J * dTendon_Area * nTendon_Qty * nSheath /1000.;
	double dAp		= dTendon_Area * nTendon_Qty * nSheath;
	double dEp		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dEs;
	double dP_f		= (dSlip==0)? 0 : dL_F * dAp / dLength_End;
	double dL_set	= sqrt( dSlip * dAp * dEp / dP_f );
	double dP_delta = 2 * dP_f * dL_set /1000.;
	double dP_i		= dPc*pow(10.0, 3) - dP_delta*pow(10.0, 3) + dP_f * dLength_End;
	double dS_Act1	= (dSlip==0)? 0 : dF_J;
	double dS_Act2	= (dSlip==0)? 0 : dL_F;
	double dS_Act3	= (dSlip==0)? 0 : dP_i / dAp;

	dL_A = dS_Act1 - dS_Act2 - dS_Act3;

	// ź���ս�
	BOOL	bFem	= pFEM->IsOutputLoaded();
	double	dPi_e	= ( dF_J - dL_F - dL_A ) * dAp;
	double	dMd_b	= (!bFem)? 0 : fabs(GetMemberForce(pFEM, "BEFORE", ELE_MOMENT2, nSide));
	if(nSide == 2)
	{
		double dMd_bTemp = (!bFem)? 0 : fabs(GetMemberForce(pFEM, "BEFORE", ELE_MOMENT2, nSide));
		dMd_b = MAX(dMd_b , dMd_bTemp);
	}
	double  dFac_e	= (dPi_e/dArea3[nSide]) + (dPi_e*dep[nSide]*dep[nSide]/dIx[nSide]) - (dMd_b*pow(10.0,6)*dep[nSide]/dIx[nSide]);
	double  dEci	= pCalcData->DESIGN_MATERIAL.m_dEci;
	
	dL_E = 0.5 * dEp  * dFac_e / dEci;

	// �Ѽս�
	dL_T = dL_F + dL_A + dL_E;

	// �ѱ�������
	dF_T = dF_J - dL_T;

	//---------------------------------------------------------------------------------------
	// 4. ������� ����-1
	double dStrength_Ulti	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength;
	double dStrength_Yield	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;
	double dFactored_Ulit	= dStrength_Ulti * 0.7;
	double dFactored_Yield	= dStrength_Yield * 0.83;

	//---------------------------------------------------------------------------------------
	// 5. �ð��� ���� �� �ս��� ����� ������ ���� ���� �� ������� ����
	double	dTime_FJ	= dF_T;	// ����������
	double	dTime_Sh	= 0;	// ��������
	double	dTime_Cr	= 0;	// ũ����
	double	dTime_Re	= 0;	// �������̼�
	double	dTime_To	= 0;	// �ѼսǷ�
	double  dTime_EFJ	= 0;	// ��ȿ����������
	double	dHumidity	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHumidity;
	double	dMd_a		= fabs(GetMemberForce(pFEM, "AFTER", ELE_MOMENT2, nSide));
	double	dFac_Cr		= dMd_a * pow(10.0, 6) * dep[nSide] / dIx[nSide];

	dTime_Sh = 0.8 * ( 119 - 1.05 * dHumidity);
	dTime_Cr = 12 * dFac_e - 7 *  dFac_Cr;
	dTime_Re = 35 - 0.07 * dL_F - 0.1 * dL_E - 0.05 * (dTime_Sh + dTime_Cr);
	dTime_To = dTime_Cr + dTime_Sh + dTime_Re;
	dTime_EFJ= dTime_FJ - dTime_To;

	//---------------------------------------------------------------------------------------
	// 6. ������� ����-2
	double dStrength_Yield2	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;
	double dFactored_Yield2	= dStrength_Yield * 0.80;


	//---------------------------------------------------------------------------------------
	// 7. ��� ���
	Map["Height_Left"]		= dHeight[0];
	Map["Height_Right"]		= dHeight[1];
	Map["Height_Middle"]	= dHeight[2];
	Map["Area_Left"]		= dArea3[0];
	Map["Area_Right"]		= dArea3[1];
	Map["Area_Middle"]		= dArea3[2];
	Map["Yb_Left"]			= dYb3[0];
	Map["Yb_Right"]			= dYb3[1];
	Map["Yb_Middle"]		= dYb3[2];
	Map["I_Left"]			= dIx[0];
	Map["I_Right"]			= dIx[1];
	Map["I_Middle"]			= dIx[2];
	Map["Zb_Left"]			= dZb[0];
	Map["Zb_Right"]			= dZb[1];
	Map["Zb_Middle"]		= dZb[2];
	Map["Ep_Left"]			= dep[0];
	Map["Ep_Right"]			= dep[1];
	Map["Ep_Middle"]		= dep[2];

	Map["Tendon_Dia"]		= dTendon_Dia;
	Map["Tendon_Area"]		= dTendon_Area;
	Map["Tendon_Qty"]		= nTendon_Qty;

	Map["Stress_Fj"]		= dF_J;
	Map["Loss_Fric"]		= dL_F;
	Map["Loss_Act"]			= dL_A;
	Map["Loss_E"]			= dL_E;
	Map["Loss_Total"]		= dL_T;
	Map["Stress_Total"]		= dF_T;

	Map["Str_Ulimate"]		= dStrength_Ulti;
	Map["Str_Yield"]		= dStrength_Yield;
	Map["Fact_Ulimate"]		= dFactored_Ulit;
	Map["Fact_Yield"]		= dFactored_Yield;

	Map["Time_FJ"]		= dTime_FJ;
	Map["Time_Sh"]		= dTime_Sh;
	Map["Time_Cr"]		= dTime_Cr;
	Map["Time_Re"]		= dTime_Re;
	Map["Time_To"]		= dTime_To;
	Map["Time_EFJ"]		= dTime_EFJ;

	Map["Str_Yield2"]		= dStrength_Yield2;
	Map["Fact_Yield2"]		= dFactored_Yield2;
}

//<�ٴڽ����� üũ> 2009.07.23 �ں���	
void CCalcFloor::CalcPreStressCheck(CAPlateCalcStd* pCalc, CMap <CString, LPCTSTR, double, double>& Map, long nSide)
{
	// nSide 0 : Left, nSide = 1 : Right, nSide = 2 : Middle

	CMap <CString, LPCTSTR, double, double> MapL;	// ������Ʈ���� �ս�
	CalcPreStressLoss(pCalc, MapL, nSide);

	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	if(pCalc->GetSizeCantileverFemData() == 0)	return;
	CFEMManage			*pFEM		= pCalc->GetCantileverFemData(0);

	// 0. Basic Data
	double dTdn_Str[2]	= { 0, 0 };		// ���������� (��������, ���ս���)
	double dEcce		= 0;			// ���������
	double dTdn_Area	= 0;			// ������ܸ���
	double dIx			= 0;			// �ܸ�2�����Ʈ
	double dAxl_For[2]  = { 0, 0 };		// �����翡 ���� ��� (��������, ���ս���)
	double dDgn_Mom[2]	= { 0, 0 };		// ������ ���Ʈ (��������, ���ս���)
	double dRes_Mom[2]	= { 0, 0 };		// �ؼ���� ���Ʈ (��������, ���ս���)
	double dSlab_Area	= 0;			// ������ ����
	double dSlab_Area2	= 0;			// ������ ����
	double dSlab_Yb		= 0;			// ������ ����
	double dSlab_Yb2	= 0;			// ������ ����


	double dRes_Befor1	 = GetMemberForce(pFEM, "BEFORE", ELE_MOMENT2, nSide);				// �¿� ĵƿ, �߾Ӻ�
	double dRes_After1	 = GetMemberForce(pFEM, "AFTER", ELE_MOMENT2, nSide);				// �¿� ĵƿ, �߾Ӻ�
	double dRes_Live_Max1 = GetMemberForce(pFEM, "LIVE MAX", ELE_MOMENT2, nSide);
	double dRes_Live_Min1 = GetMemberForce(pFEM, "LIVE MIN", ELE_MOMENT2, nSide);
	double dRes_Tank_Max1 = GetMemberForce(pFEM, "TK MAX", ELE_MOMENT2, nSide);
	double dRes_Tank_Min1 = GetMemberForce(pFEM, "TK MIN", ELE_MOMENT2, nSide);
	double dRes_Trailer_Max1= GetMemberForce(pFEM, "TR MAX", ELE_MOMENT2, nSide);
	double dRes_Trailer_Min1= GetMemberForce(pFEM, "TR MIN", ELE_MOMENT2, nSide);
	double dRes_Wind_Max1 = GetMemberForce(pFEM, "WIND MAX", ELE_MOMENT2, nSide);
	double dRes_Wind_Min1 = GetMemberForce(pFEM, "WIND MIN", ELE_MOMENT2, nSide);
	double dRes_CF_Max1	 = GetMemberForce(pFEM, "CF MAX", ELE_MOMENT2, nSide);
	double dRes_CF_Min1	 = GetMemberForce(pFEM, "CF MIN", ELE_MOMENT2, nSide);
	double dRes_Crash_Max1= GetMemberForce(pFEM, "CRASH MAX", ELE_MOMENT2, nSide);
	double dRes_Crash_Min1= GetMemberForce(pFEM, "CRASH MIN", ELE_MOMENT2, nSide);

	double dRes_Max = 0;
	double dRes_Min = 0;

	if (nSide!=2)
	{
		dRes_Max = dRes_Befor1 + dRes_After1 + dRes_Wind_Max1 + dRes_CF_Max1 + dRes_Crash_Max1 +  max( dRes_Live_Max1, max(dRes_Tank_Max1, dRes_Trailer_Max1) );
		dRes_Min = dRes_Befor1 + dRes_After1 + dRes_Wind_Min1 + dRes_CF_Min1 + dRes_Crash_Min1 +  min( dRes_Live_Min1 , min(dRes_Tank_Min1, dRes_Trailer_Min1) );
	}
	else
	{
		dRes_Max = dRes_Befor1 + dRes_After1 + dRes_Wind_Max1 + dRes_CF_Max1 + dRes_Crash_Max1 + max( dRes_Live_Max1, max(dRes_Tank_Max1, dRes_Trailer_Max1) );
		dRes_Min = dRes_Befor1 + dRes_After1 + dRes_Wind_Min1 + dRes_CF_Min1 + dRes_Crash_Min1 + min( dRes_Live_Min1, min(dRes_Tank_Min1, dRes_Trailer_Min1) ); 
	}

	long   nSheath		 = 1000 / (long)pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;

	dTdn_Str[0] = MapL["Stress_Total"];
	dTdn_Str[1] = MapL["Time_EFJ"];
	dEcce		= (nSide==0)? MapL["Ep_Left"] : (nSide==1)? MapL["Ep_Right"] : MapL["Ep_Middle"];
	dTdn_Area	= MapL["Tendon_Area"] * MapL["Tendon_Qty"] * nSheath;
	dIx			= (nSide==0)? MapL["I_Left"] : (nSide==1)? MapL["I_Right"] : MapL["I_Middle"];
	dAxl_For[0] = dTdn_Str[0] * dTdn_Area / 1000.;
	dAxl_For[1] = dTdn_Str[1] * dTdn_Area / 1000.;
	dDgn_Mom[0] = dEcce * dAxl_For[0] * pow(10.0, -3);
	dDgn_Mom[1] = dEcce * dAxl_For[1] * pow(10.0, -3);
	dRes_Mom[0] = fabs(dRes_Befor1);
	dRes_Mom[1] = max(fabs(dRes_Max), fabs(dRes_Min));
	dSlab_Area	= (nSide==0)? MapL["Area_Left"] : (nSide==1)? MapL["Area_Right"] : MapL["Area_Middle"];
	dSlab_Area2 = (nSide!=1)? MapL["Area_Left"] : MapL["Area_Right"];
	dSlab_Area *= pow(10.0, -6);
	dSlab_Area2*= pow(10.0, -6);
	dSlab_Yb	= (nSide==0)? MapL["Yb_Left"] : (nSide==1)? MapL["Yb_Right"] : MapL["Yb_Middle"];
	dSlab_Yb   *= pow(10.0, -3);
	dSlab_Yb2	= (nSide!=1)? MapL["Yb_Left"] :  MapL["Yb_Right"];
	dSlab_Yb2  *= pow(10.0, -3);

	// 1. ������Ʈ����
	double dForced_Top[2] = { 0, 0 };	// �� ���� (��������, ���ս���)
	double dForced_Bot[2] = { 0, 0 };	// �Ͽ� ���� (��������, ���ս���)

	double dCondition = (nSide == 2) ? -1 : 1;		// issue 0028191 -  �ٴ��� ������ ���°���� - �߾Ӻδ� + - �� �ݴ�� ��
	dForced_Top[0] = dAxl_For[0]/dSlab_Area + (dDgn_Mom[0]/(dIx*pow(10.0, -12)) * dSlab_Yb) * dCondition - (dRes_Mom[0]/(dIx*pow(10.0, -12)) * dSlab_Yb) * dCondition;
	dForced_Bot[0] = dAxl_For[0]/dSlab_Area - (dDgn_Mom[0]/(dIx*pow(10.0, -12)) * dSlab_Yb) * dCondition + (dRes_Mom[0]/(dIx*pow(10.0, -12)) * dSlab_Yb) * dCondition;

	dForced_Top[1] = dAxl_For[1]/dSlab_Area2 + (dDgn_Mom[1]/(dIx*pow(10.0, -12)) * dSlab_Yb2) * dCondition - (dRes_Mom[1]/(dIx*pow(10.0, -12)) * dSlab_Yb2) * dCondition;
	dForced_Bot[1] = dAxl_For[1]/dSlab_Area2 - (dDgn_Mom[1]/(dIx*pow(10.0, -12)) * dSlab_Yb2) * dCondition + (dRes_Mom[1]/(dIx*pow(10.0, -12)) * dSlab_Yb2) * dCondition;

	
	// 2. ������� 
	double dFci = pCalcData->DESIGN_MATERIAL.m_dSigmaCi;
	double dFck = pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double dAllow_Top[2]= { 0, 0 }; // �������-�� (��������, ���ս���)
	double dAllow_Bot[2]= { 0, 0 }; // �������-�Ͽ� (��������, ���ս���)

	double dPreStressCompress = pCalcData->GetConstPreStressCompress();
	double dPreStressStretch = pCalcData->GetConstPreStressStretch();
	double dFci1 = dPreStressCompress * dFci;
	double dFci2 = dPreStressStretch * dFci;

	dAllow_Top[0] = (dForced_Top[0]<0)? dFci2 : dFci1;
	dAllow_Bot[0] = (dForced_Bot[0]<0)? dFci2 : dFci1;

	double dDesignLoadCompress = pCalcData->GetConstDesignLoadCompress();
	double dDesignLoadStretch = pCalcData->GetConstDesignLoadStretch();
	double dFck1 = dDesignLoadCompress * dFck;
	double dFck2 = dDesignLoadStretch * dFck;

	dAllow_Top[1] = (dForced_Top[1]<0)? dFck2 : dFck1;
	dAllow_Bot[1] = (dForced_Bot[1]<0)? dFck2 : dFck1;

	for (long n=0; n<2; n++)
	{
		dForced_Top[n]	*= pow(10.0, -3);
		dForced_Bot[n]	*= pow(10.0, -3);
	}

	//---------------------------------------------------------------------------------------
	// 3. ��� ���
	Map["Force_Top_Before"] = dForced_Top[0];
	Map["Force_Top_After"]	= dForced_Top[1];
	Map["Force_Bot_Before"] = dForced_Bot[0];
	Map["Force_Bot_After"]	= dForced_Bot[1];

	Map["Allow_Top_Before"] = dAllow_Top[0];
	Map["Allow_Top_After"]	= dAllow_Top[1];
	Map["Allow_Bot_Before"] = dAllow_Bot[0];
	Map["Allow_Bot_After"]	= dAllow_Bot[1];
}

void CCalcFloor::SetCantileverCrack(CMap <CString, LPCTSTR, double, double>& Map,long nSide)
{
	CPlateBridgeApp	*pDB			= m_pDataManage->GetBridge();
	CCalcData		*pCalcData		= m_pDataManage->GetCalcData();
	CGlobarOption	*pOpt			= m_pDataManage->GetGlobalOption();

	SetCantileverRebar(nSide, FLOOR_STRENGTH_DESIGN);
	long nMethod	= pOpt->GetSlabCentDesignMethod();
	if(nMethod==1)
		SetCantileverRebar(nSide, FLOOR_EXPERIENCE_DESIGN);
	m_MapPreStress.clear();
	///< ������ �ϰ��	
	double	dEs			= pCalcData->DESIGN_MATERIAL.m_dEs;	// ö�� ź�����
	double	dEc			= pCalcData->DESIGN_MATERIAL.m_dEc;	// ��ũ��Ʈ ź�����
	double	dAs			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs;
	double	dD			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_d;
	double	dDia		= atof(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CaseH1);
	double	dCtc1		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Ctc1;
	double	dB			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_b;
	double	dMove_Ml	= 0.0;
	double	dCent_Mcf	= 0.0;
	double	dMmax		= 0.0;
	double	dCover		= 0.0;
	double	dPSQty		= 0.0;
	double	dAp			= 0.0;
	
	BOOL	bTank		= pCalcData->m_bTank;
	BOOL	bTrailer	= pCalcData->m_bTrailer;
	if(!pDB->IsPlateGir())
	{
		dAs			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs_1Cy;	//pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTAsuse;
		dCtc1		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTCTC;
		dPSQty		= 1000.0/pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Ctc1;
		dAp			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs_2Cy;	//dPSArea * dPSQty;
		dDia		= atof(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CaseH1);
	}

	double	dMl			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml;
	double	dMl_Tank	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml_Tank;
	double	dMl_Trailer	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml_Trailer;
	double	dMcf		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf;
	double	dMcf_Tank	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Tank;
	double	dMcf_Trailer= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Trailer;
	double	dStop_M		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Stop_M;
	double	dWind_Mw	= nSide == 2 ? 0 : pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_Mw;

	if(bTank && bTrailer)			dMove_Ml	= max(dMl, max(dMl_Tank, dMl_Trailer));
	else if(bTank && !bTrailer)		dMove_Ml	= max(dMl, dMl_Tank);
	else if(!bTank && bTrailer)		dMove_Ml	= max(dMl, dMl_Trailer);
	else							dMove_Ml	= dMl;

	if(bTank && bTrailer)			dCent_Mcf	= max(dMcf, max(dMcf_Tank, dMcf_Trailer));
	else if(bTank && !bTrailer)		dCent_Mcf	= max(dMcf, dMcf_Tank);
	else if(!bTank && bTrailer)		dCent_Mcf	= max(dMcf, dMcf_Trailer);
	else							dCent_Mcf	= dMcf;

	if(nSide == 2)	dCover = pCalcData->m_dCoverDn;
	else			dCover = pCalcData->m_dCoverUp;

	dMmax	= dStop_M + dMove_Ml + dCent_Mcf + 0.3*dWind_Mw;
	long	n		= long(dEs / dEc);
	double	dP		= (dAs+dAp)/(dB * dD);
	double	dnp		= n*dP;
	double	dK		= -dnp + sqrt(pow(dnp,2)+2*dnp);

	double	dJ		= 1 - (dK/3);
	double	dpbjd2	= dP * dB * dJ * dD * dD;
	double	dFsmax	= dMmax / dpbjd2;										// ��ö���� ����
	double	dEA		= dCtc1 == 0 ? dPSQty * 2 : (dB/dCtc1+dPSQty) * 2;				// ��ö�ټ���  b/ctc : �Ҽ������ϰ�� �������� ���� ����
	double	dH		= 2 * dCover;
	double	dA		= (dB * dH) / dEA;										// ��ȿ���� �ܸ���
	double	dDCA	= dH/2.0 * dA;
	double	dZ		= dFsmax * pow(dDCA,1.0/3.0);							// ��ö���� ������ �����ϴ¾�
	double	dHeight	= dD + dH/2;
	double	dR		= (dHeight - (dK * dD))/(dHeight - (dK * dD)-dCover);
	double	dW		= 1.08 * dR * dFsmax * pow(dDCA, 0.333333) * 0.00001;	// �տ���
	double	dC		= dCover - dDia/2.0;									// ö����ũ��Ʈ �ּҵ���
	double	ddWaCoeff	= pOpt->GetdCoeffDesignAllowCrackWidthEnv();
	double	dWa		= ddWaCoeff * dC;										// ���տ���

	Map["CRACK_P"]		= dP;
	Map["CRACK_K"]		= dK;
	Map["CRACK_J"]		= dJ;
	Map["CRACK_MMAX"]	= dMmax;
	Map["CRACK_MSTOP"]	= dStop_M;
	Map["CRACK_MMOVE"]	= dMove_Ml;
	Map["CRACK_MCENT"]	= dCent_Mcf;
	Map["CRACK_MW"]		= dWind_Mw;
	Map["CRACK_PDJD2"]	= dpbjd2;
	Map["CRACK_Fsmax"]	= dFsmax;
	Map["CRACK_Z"]		= dZ;
	Map["CRACK_W"]		= dW;
	Map["CRACK_C"]		= dC;
	Map["CRACK_R"]		= dR;
	Map["CRACK_Wa"]		= dWa;
	Map["CRACK_A"]		= dA;
	Map["CRACK_EA"]		= dEA;
	Map["CRACK_As"]		= dAs;
	Map["CRACK_Ap"]		= dAp;
	Map["CRACK_H"]		= dH;

	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Crack_w	= dW;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Crack_wa	= dWa;
}

long CCalcFloor::GetGuardWallType(long nSide)
{
	CPlateBridgeApp* pBridge = m_pDataManage->GetBridge();	
	m_nTypeHDan = 0;
	long nType=100;
	if(nSide == FLOOR_LEFT)
	{		
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);	

		m_nTypeHDan = pBridge->GetValueTypeHDan(0, 1);

		GetAppConcreteEC(m_nTypeHDan);//��ũ��Ʈ ���...
		if(pSep==NULL)		return 99;

		double Width = pSep->GetWidth();
		double W1 = pSep->m_W1;
		double W2 = pSep->m_W2;
		double W3 = pSep->m_W3;
		double W4 = pSep->m_W4;
		double W5 = pSep->m_W5;
		double H1 = pSep->m_H1;
		double H2 = pSep->m_H2;
		double H3 = pSep->m_H3;

		if(pBridge->IsGuard(0, 1))//��ȣ���̰ų� �ߺд��ϰ�츸...
		{
			if( W1==0 && W2==0 && W3==0 && W4==0 && W5==0)									nType = 0;
			else if( W3>0 && W4>0 && W5>0 && W1==0 && W2==0 && H1>0 && H2>0 && H3>0 )		nType = 1;
			else if( W3>0 && W5==0 && W4>0 && W1==0 && W2==0 && H3>0 && ( H1>0 || H2>0 ))	nType = 2;
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1>0 && H2>0 && H3>0 )		nType = 3;
			else if( Width == W1 )															nType = 4;	
			else if( Width == W2 )															nType = 4;	
			else if( Width == W3 )															nType = 4;	
			else if( Width == W4 )															nType = 4;	
			else if( Width == W5 )															nType = 4;	
			else if( W3>0 && W4>0 && W5>0 && W1>0 && W2>0 && H1>0 && H2>0 && H3>0 )			nType = 5;
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1==0 && H2>0 && H3>0 )		nType = 6;
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1==0 && H2>0 && H3==0 )	nType = 7;
			else if( W3>0 && W4>0 && W5==0 && W1==0 && W2==0 && H1==0 && H2==0 && H3>0 )
			{
				pSep->m_W5 = W4; pSep->m_W4 = 0; pSep->m_H2 = H3; pSep->m_H3 = 0;
				nType = 7;
			}
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1>0 && H2==0 && H3>0 )		nType = 8;						
		}
		else
			nType = 99;
	}
	else if(nSide == FLOOR_RIGHT)
	{				
		CPlateBasicIndex *pBx = pBridge->GetGirder(0)->GetBxOnJijum(0);

		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		m_nTypeHDan = pBridge->GetValueTypeHDan(pBridge->GetQtyHDanNode()-1, 1);

		GetAppConcreteEC(m_nTypeHDan);//��ũ��Ʈ ���...
		if(!pSep)	return 99;

		double Width = pSep->GetWidth();
		double W1 = pSep->m_W5;
		double W2 = pSep->m_W4;
		double W3 = pSep->m_W3;
		double W4 = pSep->m_W2;
		double W5 = pSep->m_W1;
		double H1 = pSep->m_H1;
		double H2 = pSep->m_H2;
		double H3 = pSep->m_H3;

		if(pBridge->IsGuard(pBridge->GetQtyHDanNode()-1, 1))//��ȣ���̰ų� �ߺд��ϰ�츸...
		{
			if( W1==0 && W2==0 && W3==0 && W4==0 && W5==0)									nType = 0;
			else if( W3>0 && W4>0 && W5>0 && W1==0 && W2==0 && H1>0 && H2>0 && H3>0 )		nType = 1;
			else if( W3>0 && W5==0 && W4>0 && W1==0 && W2==0 && H3>0 && ( H1>0 || H2>0 ))	nType = 2;
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1>0 && H2>0 && H3>0 )		nType = 3;
			else if( Width == W1 )															nType = 4;	
			else if( Width == W2 )															nType = 4;	
			else if( Width == W3 )															nType = 4;	
			else if( Width == W4 )															nType = 4;	
			else if( Width == W5 )															nType = 4;	
			else if( W3>0 && W4>0 && W5>0 && W1>0 && W2>0 && H1>0 && H2>0 && H3>0 )			nType = 5;
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1==0 && H2>0 && H3>0 )		nType = 6;
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1==0 && H2>0 && H3==0 )	nType = 7;
			else if( W3>0 && W4>0 && W5==0 && W1==0 && W2==0 && H1==0 && H2==0 && H3>0 )
			{
				pSep->m_W5 = W4; pSep->m_W4 = 0; pSep->m_H2 = H3; pSep->m_H3 = 0;
				nType = 7;
			}
			else if( W3>0 && W4==0 && W5>0 && W1==0 && W2==0 && H1>0 && H2==0 && H3>0 )		nType = 8;			
		}	
		else nType = 99;				
	}

	return nType;
}

// bIsLeft = TRUE  ���������������� ���Ʈ�߽ɱ��� �̰ݰŸ� 
// bIsLeft = FALSE ���������������� ���Ʈ�߽ɱ��� �̰ݰŸ� 
BOOL CCalcFloor::GetCalcGuardRail(CPlateBasicIndex *pBx, double &dDist, double &dForce, BOOL bIsLeft)
{
	CPlateBridgeApp* pBridge = m_pDataManage->GetBridge();
	double	LEN   = 0;
	long	nType = 0;
	
	if(bIsLeft)
	{
		LEN		= pBridge->GetLengthHDanDom(pBx, 0);		
		nType	= GetGuardWallType(FLOOR_LEFT);
	}
	else
	{
		LEN		= pBridge->GetLengthHDanDom(pBx, pBridge->GetQtyHDanNode()-1);
		nType	= GetGuardWallType(FLOOR_RIGHT);
	}

	switch( nType )
	{		
		case	1: GetCalcGuardType1(dDist, dForce, LEN, bIsLeft);	break;
		case	2: GetCalcGuardType2(dDist, dForce, LEN, bIsLeft);	break;
		case	3: GetCalcGuardType3(dDist, dForce, LEN, bIsLeft);	break;
		case	4: GetCalcGuardType4(dDist, dForce, LEN, bIsLeft);	break;
		case	5: GetCalcGuardType5(dDist, dForce, LEN, bIsLeft);	break;
		case	6: GetCalcGuardType6(dDist, dForce, LEN, bIsLeft);	break;
		case	7: GetCalcGuardType7(dDist, dForce, LEN, bIsLeft);	break;
		case	8: GetCalcGuardType8(dDist, dForce, LEN, bIsLeft);	break;
		default:
		dDist = 0.0; dForce = 0.0;
	}
	if(bIsLeft==FALSE)
	{
		dDist += pBridge->GetWidthSlabDom(pBx) - pBridge->m_dWidthSlabRight;
	}

	return TRUE;
}


BOOL CCalcFloor::GetCalcJungBunDe(CPlateBasicIndex *pBx, double &dForce)
{
	CPlateBridgeApp	*pBridge	= m_pDataManage->GetBridge();
	
	// CenterGuardWall
	double	BaseLen		= 1000;

	double	W2		= 0;
	double	W3		= 0;
	double	W4		= 0;
	double	H1		= 0;
	double	H2		= 0;
	double	H3		= 0;
	double	Width	= 0;	
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//���� ������ ��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::CENTER);
		if(!pSep)	return FALSE;
		W2 = pSep->m_W2;
		W3 = pSep->m_W3;
		W4 = pSep->m_W4;
		H1 = pSep->m_H1;		// ���� �β� ����
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double Ton1 =  0;
	double Ton2 =  0;
	double Ton3 =  0;

	Ton1 = (1.0/2.0*(W3 + (W2+W3+W4)) * H3)*m_dAppConcreteEC * BaseLen;
	Ton2 = (1.0/2.0*(W2+W3+W4 + Width)*H2)*m_dAppConcreteEC * BaseLen;
	Ton3 = (Width*H1)*m_dAppConcreteEC * BaseLen;

	dForce = Ton1 +Ton2 +Ton3;
	
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////
///������������ ���߰� ���Ʈ�� ����
/////////////////////////////////////////////////////////////////////////////////////
long CCalcFloor::CalcSlabLeftDeadLoad(CStringArray& arr)
{		
	CPlateBridgeApp  *pBridge = m_pDataManage->GetBridge();			
	CPlateBasicIndex *pBx	  = pBridge->GetGirder(0)->GetBxOnJijum(0);
	double	LEN		= pBridge->GetLengthHDanDom(pBx, 0);
	long	nType	= GetGuardWallType(FLOOR_LEFT);

	switch( nType )
	{
		case   0: CalcGuardLeftType0(pBx, arr, LEN);		break;
		case   1: CalcGuardLeftType1(pBx, arr, LEN);		break;
		case   2: CalcGuardLeftType2(pBx, arr, LEN);		break;
		case   3: CalcGuardLeftType3(pBx, arr, LEN);		break;
		case   4: CalcGuardLeftType4(pBx, arr, LEN);		break;
		case   5: CalcGuardLeftType5(pBx, arr, LEN);		break;
		case   6: CalcGuardLeftType6(pBx, arr, LEN);		break;
		case   7: CalcGuardLeftType7(pBx, arr, LEN);		break;
		case   8: CalcGuardLeftType8(pBx, arr, LEN);		break;
		case  99: CalcGuardLeftType99(pBx, arr, LEN);		break;
		case 100: CalcGuardLeftType100(pBx, arr, LEN);		break;

	}
	
	return nType;
}

///< �����꿡 ���� ���Ʈ�� �ձ��ϱ�
double CCalcFloor::CalcCantileverPreDeadLoad(long nSide)
{

	CStringArray arr;
	if(nSide == FLOOR_LEFT)
	{
		CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();			
		CPlateBasicIndex	*pBx		= pBridge->GetGirder(0)->GetCrossBxAtMinFlangeWidth();
		double	LEN		= pBridge->GetLengthHDanDom(pBx, 0);
		long	nType	= GetGuardWallType(FLOOR_LEFT);

		switch( nType )
		{
			case   0: CalcGuardLeftType0(pBx, arr, LEN);		break;		
			case   1: CalcGuardLeftType1(pBx, arr, LEN);		break;		
			case   2: CalcGuardLeftType2(pBx, arr, LEN);		break;
			case   3: CalcGuardLeftType3(pBx, arr, LEN);		break;
			case   4: CalcGuardLeftType4(pBx, arr, LEN);		break;	
			case   5: CalcGuardLeftType5(pBx, arr, LEN);		break;		
			case   6: CalcGuardLeftType6(pBx, arr, LEN);		break;
			case   7: CalcGuardLeftType7(pBx, arr, LEN);		break;
			case   8: CalcGuardLeftType8(pBx, arr, LEN);		break;
			case  99: CalcGuardLeftType99(pBx, arr, LEN);	break;
			case 100: CalcGuardLeftType100(pBx, arr, LEN);	break;

		}
	}
	else if(nSide == FLOOR_RIGHT)
	{
		CPlateBridgeApp  *pBridge = m_pDataManage->GetBridge();			
		CPlateBasicIndex *pBx     = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetCrossBxAtMinFlangeWidth();	

		double LEN = pBridge->GetLengthHDanDom(pBx, pBridge->GetQtyHDanNode()-1);
		long nType = GetGuardWallType(FLOOR_RIGHT);

		switch( nType )
		{
			case   0: CalcGuardRightType0(pBx, arr, LEN);	break;		
			case   1: CalcGuardRightType1(pBx, arr, LEN);	break;
			case   2: CalcGuardRightType2(pBx, arr, LEN);	break;
			case   3: CalcGuardRightType3(pBx, arr, LEN);	break;
			case   4: CalcGuardRightType4(pBx, arr, LEN);	break;
			case   5: CalcGuardRightType5(pBx, arr, LEN);	break;
			case   6: CalcGuardRightType6(pBx, arr, LEN);	break;
			case   7: CalcGuardRightType7(pBx, arr, LEN);	break;
			case   8: CalcGuardRightType8(pBx, arr, LEN);	break;
			case  99: CalcGuardRightType99(pBx, arr, LEN);	break;
			case 100: CalcGuardRightType100(pBx, arr, LEN);	break;
		}
	}
	else
		CalcSlabCenterDeadLoad(arr);
	return m_dPreDeadLoad[nSide];
}

///< ������ ������ ���Ʈ���� ���ϱ�
double CCalcFloor::CalcCantileverPostDeadLoad(long nSide)
{

	CStringArray arr;
	if(nSide == FLOOR_LEFT)
	{
		CPlateBridgeApp  *pBridge = m_pDataManage->GetBridge();			
		CPlateBasicIndex *pBx	  = pBridge->GetGirder(0)->GetCrossBxAtMinFlangeWidth();
		double	LEN		= pBridge->GetLengthHDanDom(pBx, 0);
		long	nType	= GetGuardWallType(FLOOR_LEFT);

		switch( nType )
		{
			case   0: CalcGuardLeftType0(pBx, arr, LEN);		break;		
			case   1: CalcGuardLeftType1(pBx, arr, LEN);		break;		
			case   2: CalcGuardLeftType2(pBx, arr, LEN);		break;
			case   3: CalcGuardLeftType3(pBx, arr, LEN);		break;
			case   4: CalcGuardLeftType4(pBx, arr, LEN);		break;	
			case   5: CalcGuardLeftType5(pBx, arr, LEN);		break;		
			case   6: CalcGuardLeftType6(pBx, arr, LEN);		break;
			case   7: CalcGuardLeftType7(pBx, arr, LEN);		break;
			case   8: CalcGuardLeftType8(pBx, arr, LEN);		break;
			case  99: CalcGuardLeftType99(pBx, arr, LEN);	break;
			case 100: CalcGuardLeftType100(pBx, arr, LEN);	break;

		}
	}
	else if(nSide == FLOOR_RIGHT)
	{
		CPlateBridgeApp  *pBridge = m_pDataManage->GetBridge();			
		CPlateBasicIndex *pBx     = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetCrossBxAtMinFlangeWidth();	

		double LEN = pBridge->GetLengthHDanDom(pBx, pBridge->GetQtyHDanNode()-1);
		long nType = GetGuardWallType(FLOOR_RIGHT);

		switch( nType )
		{
			case   0: CalcGuardRightType0(pBx, arr, LEN);	break;		
			case   1: CalcGuardRightType1(pBx, arr, LEN);	break;
			case   2: CalcGuardRightType2(pBx, arr, LEN);	break;
			case   3: CalcGuardRightType3(pBx, arr, LEN);	break;
			case   4: CalcGuardRightType4(pBx, arr, LEN);	break;
			case   5: CalcGuardRightType5(pBx, arr, LEN);	break;
			case   6: CalcGuardRightType6(pBx, arr, LEN);	break;
			case   7: CalcGuardRightType7(pBx, arr, LEN);	break;
			case   8: CalcGuardRightType8(pBx, arr, LEN);	break;
			case  99: CalcGuardRightType99(pBx, arr, LEN);	break;
			case 100: CalcGuardRightType100(pBx, arr, LEN);	break;
		}
	}
	else
		CalcSlabCenterDeadLoad(arr);
	return m_dPostDeadLoad[nSide];
}

void CCalcFloor::CalcGuardLeftType0(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();		
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	double PaveT = pBridge->m_dThickPave;		// ���� �β�
	double BaseLen = 1000;						//���� ����(�߷� ���� �� ��� 1m)
	double Term = pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;

	CStringArray ArrSlab;
	double	dTotSlabWeight = 0;
	double  dTotSlabMoment = 0;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton4 = (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton5 = 0;
	double Ton6 = 0;
	double Ton7 = 0;
	double BaseX = Term;

	double Dis4 = (BaseX-LEN)/2;
	double Dis7 = 0;
	double M = 0, MTot = 0;

	m_dPreDeadLoad[0]	= dTotSlabWeight;//Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3;
	m_dPostDeadLoad[0]	= Ton4*Dis4;
	
	for(long n=0; n < ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

// 4	���� ���������̶��
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton4));
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis4));
	arr.Add(cs);
	M = Ton4 * Dis4;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M));
	arr.Add(cs);

	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<pCalcData->m_ELoadTitleArr.GetSize(); n++)
		{
			Ton7 = pCalcData->m_ELoadWeightArr[n];
			Dis7 = toM(Term) - toM(pCalcData->m_ELoadDisArr[n]);
			cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
			arr.Add(cs);
			cs.Format( "%.5f ", tokNPM(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", Dis7);
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", M );
			arr.Add(cs);
		}
	}
	
	MTot += dTotSlabMoment;
	// 5 Total
	cs.Format( "%.5f ", tokN(dTotSlabWeight+Ton4+Ton5+Ton6+Ton7) );	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));					// T*M Total
	arr.Add(cs);	

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Stop_Ton = dTotSlabWeight+Ton4+Ton5+Ton6+Ton7;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Stop_M   = MTot;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_Ton = 0;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_M = 0;
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = 0;
}


void CCalcFloor::GetCalcGuardType1(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1 = 0;
	double D2 = 0;
	double W3 = 0;
	double W4 = 0;
	double W5 = 0;
	double H1 = 0;
	double H2 = 0;
	double H3 = 0;	

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep	= NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;

		if(bIsLeft)
		{		
			D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
			W3 = pSep->m_W3;
			W4 = pSep->m_W4;
			W5 = pSep->m_W5;
		}
		else
		{
			D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
			W3 = pSep->m_W3;
			W4 = pSep->m_W2;
			W5 = pSep->m_W1;
			D2 = LEN - pSep->GetWidth();
		}
		H1 = pSep->m_H1;		// ���� �β� ����
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;
	}
	
	double Ton1, Ton2, Ton3, Ton4, Ton5;

	Ton1 =  W3*H3*m_dAppConcreteEC*BaseLen;
	Ton2 =  W4*H3*m_dAppConcreteEC/2 * BaseLen;
	Ton3 =  (W4+W3)*(H1+H2)*m_dAppConcreteEC * BaseLen;
	Ton4 =  W5*H2*m_dAppConcreteEC/2 * BaseLen;
	Ton5 =  W5*H1*m_dAppConcreteEC * BaseLen;
	
	double Dis1 =  W3/2;
	double Dis2 =  W3+W4/3;
	double Dis3 =  (W3+W4)/2;
	double Dis4 =  W3+W4+(W5/3);
	double Dis5 =  W4+W3+W5/2;

	dForce = (Ton1 + Ton2 + Ton3 + Ton4 + Ton5);
	double dMomentTotal = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3) + (Ton4*Dis4) + (Ton5*Dis5);
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;												// ������ȣ��
	else 
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);				// ������ȣ��
}

void CCalcFloor::GetCalcGuardType2(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1		= 0.0;
	double D2		= 0.0;
	double W3		= 0.0;
	double W4		= 0.0;
	double H1H2		= 0.0;
	double H3		= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep = NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		if(!pSep)	return;

		if(bIsLeft)
		{	
			D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
			W4		= pSep->m_W4;			
		}
		else
		{
			D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
			W4		= pSep->m_W2;			
			D2		= LEN - pSep->GetWidth();
		}
		H1H2	= pSep->m_H1 + pSep->m_H2;		// ���� �β� ����
		H3		= pSep->m_H3;		
	}
	
	double Ton1, Ton2, Ton3;
	
	Ton1 =  (W3*H3) * m_dAppConcreteEC * BaseLen;
	Ton2 =  (W4*H3/2.0) *m_dAppConcreteEC * BaseLen;
	Ton3 =  (W4+W3)*(H1H2) * m_dAppConcreteEC * BaseLen;
	
	double Dis1 =  W3/2;
	double Dis2 =  W3+W4/3;
	double Dis3 =  (W3+W4)/2;
	
	dForce = (Ton1 + Ton2 + Ton3);
	double dMomentTotal = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3);
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;											// ���� ��ȣ��
	else
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);			// ���� ��ȣ��
}

void CCalcFloor::GetCalcGuardType3(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1		= 0.0;
	double D2		= 0.0;
	double W3		= 0.0;
	double W5		= 0.0;
	double H1		= 0.0;
	double H2		= 0.0;
	double H3		= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep = NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		if(!pSep)	return;

		if(bIsLeft)
		{		
			D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
			W5		= pSep->m_W5;
		}
		else
		{
			D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
			W5		= pSep->m_W1;
			D2		= LEN - pSep->GetWidth();
		}
		H1		= pSep->m_H1;
		H2		= pSep->m_H2;		// ���� �β� ����
		H3		= pSep->m_H3;
	}
	
	double Ton1, Ton2, Ton3;	
	Ton1 =  W3*(H3+H2)*m_dAppConcreteEC * BaseLen;
	Ton2 =  W5*H2*m_dAppConcreteEC/2 * BaseLen;
	Ton3 =  (W5+W3)*H1*m_dAppConcreteEC* BaseLen;
	
	double Dis1 =  W3/2;
	double Dis2 =  W3+W5/3;
	double Dis3 =  (W3+W5)/2;
	
	dForce = (Ton1 + Ton2 + Ton3);
	double dMomentTotal = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3);
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;			// ������ȣ��
	else 
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);	// ������ȣ��
}

void CCalcFloor::GetCalcGuardType4(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1		= 0.0;
	double D2		= 0.0;
	double W3		= 0.0;
	double Height	= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep = NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		if(!pSep)	return;

		if(bIsLeft)
		{		
			D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
		}
		else
		{			
			W3		= pSep->m_W3;
			D2		= LEN - pSep->GetWidth();
		}		
		Height	= pSep->m_H1 + pSep->m_H2 + pSep->m_H3;
	}
	
	double Ton1;	

	Ton1 =  W3*Height*m_dAppConcreteEC * BaseLen;
	
	double Dis1 =  W3/2;
	
	dForce = Ton1;
	double dMomentTotal = Ton1*Dis1;
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;			// ������ȣ��
	else 
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);	// ������ȣ��

}

void CCalcFloor::GetCalcGuardType5(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1		= 0.0;
	double D2		= 0.0;
	double W1		= 0.0;
	double W2		= 0.0;
	double W3		= 0.0;
	double W4		= 0.0;
	double W5		= 0.0;
	double H1		= 0.0;
	double H2		= 0.0;
	double H3		= 0.0;
	double Width	= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep = NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		if(!pSep)	return;

		if(bIsLeft)	D1 = pSep->m_D1;			//���� ��ȣ���� ���� �� ����
		else		D2 = LEN - pSep->GetWidth();
		W1		= pSep->m_W1;
		W2		= pSep->m_W2;
		W3		= pSep->m_W3;
		W4		= pSep->m_W4;
		W5		= pSep->m_W5;
		H1		= pSep->m_H1;
		H2		= pSep->m_H2;			// ���� �β� ����
		H3		= pSep->m_H3;
		Width	= pSep->GetWidth();		// ��ȣ��
	}
	
	double Ton1, Ton2, Ton3;	

	Ton1 = (1.0/2.0*(W3 + (W2+W3+W4)) * H3)*m_dAppConcreteEC * BaseLen;
	Ton2 = (1.0/2.0*(W2+W3+W4 + Width)*H2)*m_dAppConcreteEC * BaseLen;
	Ton3 = (Width*H1)*m_dAppConcreteEC * BaseLen;
	
	double Dis1 =  W1+W2+W3/2;
	double Dis2 =  W1+W2+W3/2;
	double Dis3 =  (W1+W2+W3+W4+W5)/2;
	
	dForce = (Ton1 + Ton2 + Ton3);
	double dMomentTotal = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3);
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;			// ������ȣ��
	else 
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);	// ������ȣ��
}

void CCalcFloor::GetCalcGuardType6(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1		= 0.0;
	double D2		= 0.0;
	double W3		= 0.0;
	double W5		= 0.0;
	double H2		= 0.0;
	double H3		= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep = NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		if(!pSep)	return;

		if(bIsLeft)
		{
			D1		= pSep->m_D1;			//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
			W5		= pSep->m_W5;
		}
		else
		{
			D2		= LEN - pSep->GetWidth(); 
			W3		= pSep->m_W3;
			W5		= pSep->m_W1;
		}
		H2		= pSep->m_H2;			// ���� �β� ����
		H3		= pSep->m_H3;
	}
	
	double Ton1, Ton2;

	Ton1 = (W3*(H2+H3))*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H2)*m_dAppConcreteEC/2 * BaseLen;
	
	double Dis1 =  W3/2;
	double Dis2 =  W3+W5/3;
	
	dForce = (Ton1 + Ton2);
	double dMomentTotal = (Ton1*Dis1) + (Ton2*Dis2);
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;			// ������ȣ��
	else 
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);	// ������ȣ��
}

void CCalcFloor::GetCalcGuardType7(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1		= 0.0;
	double D2		= 0.0;
	double W3		= 0.0;
	double W5		= 0.0;
	double H2		= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep = NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		if(!pSep)	return;

		if(bIsLeft)
		{
			D1		= pSep->m_D1;			//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
			W5		= pSep->m_W5;		
		}
		else
		{
			D2		= LEN - pSep->GetWidth(); 
			W3		= pSep->m_W3;
			W5		= pSep->m_W1;
		}
		H2		= pSep->m_H2;			// ���� �β� ����
	}
	
	double Ton1, Ton2;

	Ton1 = (W3*H2)*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H2)*m_dAppConcreteEC/2 * BaseLen;
	
	double Dis1 =  W3/2;
	double Dis2 =  W3+W5/3;
	
	dForce = (Ton1 + Ton2);
	double dMomentTotal = (Ton1*Dis1) + (Ton2*Dis2);
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;			// ������ȣ��
	else 
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);	// ������ȣ��
}

void CCalcFloor::GetCalcGuardType8(double &dDist, double &dForce, double LEN, BOOL bIsLeft)
{
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();

	double BaseLen	= 1000;					//���� ����(�߷� ���� �� ��� 1m)
	double D1		= 0.0;
	double D2		= 0.0;
	double W3		= 0.0;
	double W5		= 0.0;
	double H1		= 0.0;
	double H3		= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation	*pSep = NULL;
		if(bIsLeft)	pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		else		pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);

		if(!pSep)	return;

		if(bIsLeft)
		{
			D1		= pSep->m_D1;			//���� ��ȣ���� ���� �� ����
			W3		= pSep->m_W3;
			W5		= pSep->m_W5;
		}
		else
		{
			D2		= LEN - pSep->GetWidth(); 
			W3		= pSep->m_W3;
			W5		= pSep->m_W1;
		}
		H1		= pSep->m_H1;
		H3		= pSep->m_H3;			// ���� �β� ����
	}
	
	double Ton1, Ton2;

	Ton1 = (W3*(H1+H3))*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H1)*m_dAppConcreteEC * BaseLen;

	
	double Dis1 =  W3/2;
	double Dis2 =  W3+W5/2;
	
	dForce = (Ton1 + Ton2);
	double dMomentTotal = (Ton1*Dis1) + (Ton2*Dis2);
	if(bIsLeft)
		dDist = D1 + dMomentTotal / dForce;			// ������ȣ��
	else 
		dDist = pBridge->m_dWidthSlabRight - (D2 + dMomentTotal / dForce);	// ������ȣ��
}

// ���� : ���� ��ȣ�� (���� Ÿ��) ���� ���(W3, W4, W5 ��� ���� ��)
// �Է� : &arr
// LEN	: ��ȣ�� �ʺ�(mm)
// ��� : &arr
void CCalcFloor::CalcGuardLeftType1(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();		
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1 = 0;
	double	W3 = 0;
	double	W4 = 0;
	double	W5 = 0;
	double	H1 = 0;
	double	H2 = 0;
	double	H3 = 0;
	double	Width = 0;
	
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);

		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3 = pSep->m_W3;
		W4 = pSep->m_W4;
		W5 = pSep->m_W5;
		H1 = pSep->m_H1;		// ���� �β� ����
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double	BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	
	double	Ton1		=  0;
	double	Ton2		=  0;
	double	Ton3		=  0;
	double	Ton4		=  0;
	double	Ton5		=  0;

	
	Ton1 =  W3*H3*m_dAppConcreteEC* BaseLen;
	Ton2 =  W4*H3*m_dAppConcreteEC/2 * BaseLen;
	Ton3 =  (W4+W3)*(H1+H2)*m_dAppConcreteEC * BaseLen;
	Ton4 =  W5*H2*m_dAppConcreteEC/2 * BaseLen;
	Ton5 =  W5*H1*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton9 =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton10 = 0;
	double Ton11 = 0;
	double Ton12 = 0;
	double Ton13 = 0;
	double BaseX = Term;
	double Dis1 =  BaseX-D1-W3/2;	
	double Dis2 =  BaseX-D1-W3-(W4/3);
	double Dis3 =  BaseX-D1-(W3+W4)/2;
	double Dis4 =  BaseX-D1-W3-W4-(W5/3);
	double Dis5 =  BaseX-D1-(W4+W3)-W5/2;
	double Dis9  = (BaseX-LEN)/2;
	double Dis10 = 0;
	double Dis11 = 0;
	double M = 0,	MTot = 0, MTotE = 0;
	
	m_dPreDeadLoad[0]	= dTotSlabMoment;
	m_dPostDeadLoad[0]	= Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton4*Dis4+ Ton5*Dis5 + Ton9*Dis9;
	
	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	
	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(H3), tokNPM3(dUWeigtConc));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W4), toM(H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��	
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W3+W4), toM(H1+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton3) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis3) );
	arr.Add(cs);
	M = Ton3 * Dis3;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2),   tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton4) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis4) );
	arr.Add(cs);
	M = Ton4 * Dis4;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��	
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W5), toM(H1),   tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton5) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis5) );
	arr.Add(cs);
	M = Ton5 * Dis5;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n = 0; n < ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton9));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis9) );
		arr.Add(cs);
		M = Ton9 * Dis9;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
 		double T1	= dWalkLoad1;
		Ton9 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis9 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton9));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis9));
		arr.Add(cs);
		M = Ton9 * Dis9;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	
	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton10 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis10 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton10));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis10));
			arr.Add(cs);
			M = Ton10 * Dis10;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100)
		{
			Ton11 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis11 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton11));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis11));
			arr.Add(cs);
			M = Ton11 * Dis11;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize	= pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];

	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;

	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	
	double dWeightTot = Ton1+Ton2+Ton3+Ton4+Ton5+dTotSlabWeight/*Ton6+Ton7+Ton8*/+Ton9+Ton10+Ton11+Ton12+Ton13;
	MTot+= dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot) );	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );			// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_Ton = dWeightTot;
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_M   = MTot;

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;	
	Dis2 =  W3+W4/3;
	Dis3 =  (W3+W4)/2;
	Dis4 =  W3+W4+(W5/3);
	Dis5 =  W4+W3+W5/2;

	// Slab�� ������ ������ ���		
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3) + (Ton4*Dis4) + (Ton5*Dis5);

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_Ton = tokN(Ton1+Ton2+Ton3+Ton4+Ton5);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_M   = tokNM(MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = (Ton1+Ton2+Ton3+Ton4+Ton5);

}

// ���� : ���� ��ȣ�� (���� Ÿ��) ���� ���(W3, W4 ���� ��)
// �Է� : &arr
//		  LEN	: ��ȣ�� �ʺ�(mm)
// ��� : &arr
void CCalcFloor::CalcGuardLeftType2(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	double PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double D1		= 0.0;
	double W3		= 0.0;
	double W4		= 0.0;
	double H1		= 0.0;
	double H2		= 0.0;
	double H3		= 0.0;
	double Width	= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
		D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3		= pSep->m_W3;
		W4		= pSep->m_W4;
		H1		= pSep->m_H1;
		H2		= pSep->m_H2;
		H3		= pSep->m_H3;
		Width	= pSep->GetWidth();		// ��ȣ��
	}

	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double Term = pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	
	double Ton1	=  0.0;
	double Ton2	=  0.0;
	double Ton3	=  0.0;

	Ton1 =  W3 * H3 * m_dAppConcreteEC * BaseLen;
	Ton2 =  W4 * H3 * m_dAppConcreteEC/2 * BaseLen;
	Ton3 =  (W3 + W4) * (H1+H2) * m_dAppConcreteEC * BaseLen;

	double BaseX =	Term;
	double Dis1  =	BaseX-D1-W3/2;	
	double Dis2  =	BaseX-D1-W3-(W4/3);
	double Dis3  =	BaseX-D1-(W3 + W4)/2;
	double Dis7  =	(BaseX-LEN)/2;
	double Dis8  =	0.0;
	double Dis9  =	0.0;
	double M = 0,	MTot = 0,	MTotE = 0;
	
	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;

	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double	Ton7 =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double	Ton8 =  0.0;
	double	Ton9 =  0.0;

	m_dPreDeadLoad[0]	= dTotSlabMoment;//Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6;
	m_dPostDeadLoad[0]	= Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton7*Dis7;
	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	
	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W4), toM(H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	
	
	// ��	
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W3+W4), toM(H1+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton3) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis3) );
	arr.Add(cs);
	M = Ton3 * Dis3;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n = 0; n < ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7));
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
 		double T1 = dWalkLoad1;
		Ton7 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis7 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7));
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100)
		{
			Ton9 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis9 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton9));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis9));
			arr.Add(cs);
			M = Ton9 * Dis9;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}
	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = Term - pCalcData->m_ELoadDisArr[n];
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	double dWeightTot = Ton1+Ton2+Ton3+dTotSlabWeight/*Ton4+Ton5+Ton6*/+Ton7+Ton8+Ton9;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));											// T*M Total
	arr.Add(cs);

	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Stop_Ton = dWeightTot;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Stop_M   = MTot;

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;	
	Dis2 =  W3+(W4*2/3);
	Dis3 =  (W3+W4)/2;

	// Slab�� ������ ������ ���				
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3);		

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_Ton = (Ton1+Ton2+Ton3);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = (Ton1+Ton2+Ton3);
}

// ���� : ���� ��ȣ�� (���� Ÿ��) ���� ���(W3, W5 ���� ��, W4 = 0)
// �Է� : &arr
//		  LEN	: ��ȣ�� �ʺ�(mm)
// ��� : &arr
void CCalcFloor::CalcGuardLeftType3(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge   = m_pDataManage->GetBridge();		
	CCalcData			*pCalcData = m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	double PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double D1		= 0.0;
	double W3		= 0.0;
	double W5		= 0.0;
	double H1		= 0.0;
	double H2		= 0.0;
	double H3		= 0.0;
	double Width	= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
		D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3		= pSep->m_W3;
		W5		= pSep->m_W5;
		H1		= pSep->m_H1;
		H2		= pSep->m_H2;
		H3		= pSep->m_H3;
		Width	= pSep->GetWidth();		// ��ȣ��		
	}

	double BaseLen	= 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double Term		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	
	double Ton1		= 0.0;
	double Ton2		= 0.0;
	double Ton3		= 0.0;
	
	Ton1 =  W3*(H3+H2)*m_dAppConcreteEC * BaseLen;
	Ton2 =  W5*H2*m_dAppConcreteEC/2 * BaseLen;
	Ton3 =  (W3+W5)*(H1)*m_dAppConcreteEC * BaseLen;

	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CStringArray ArrSlab;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double	Ton7 = (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt*BaseLen;
	double	Ton8 =  0.0;
	double	Ton9 =  0.0;
	double	Ton10 = 0.0;
	double	Ton11 = 0.0;
	double	BaseX	= Term;
	double  Dis1	= BaseX-D1-W3/2;	
	double  Dis2	= BaseX-D1-W3-(W5/3);
	double  Dis3	= BaseX-D1-(W3+W5)/2;
	double	Dis7	= (BaseX-LEN)/2;
	double	Dis8	= 0.0;
	double	Dis9	= 0.0;
	double	M = 0,	MTot = 0,	MTotE = 0;

	m_dPreDeadLoad[0]	= dTotSlabMoment;//Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6;
	m_dPostDeadLoad[0]	= Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton7*Dis7;
///////////////////////////////////////////////////////////////////////////////
	double dUWeigtConc = m_dAppConcreteEC;
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(H3+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	
	
	// ��	
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W3+W5), toM(H1), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton3) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis3) );
	arr.Add(cs);
	M = Ton3 * Dis3;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0; n < ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad) 
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7) );
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
 		double T1 = dWalkLoad1;
		Ton7 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis7 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7));
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100)
		{
			Ton9 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis9 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton9));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis9));
			arr.Add(cs);
			M = Ton9 * Dis9;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];

	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = Ton1+Ton2+Ton3+dTotSlabWeight/*Ton4+Ton5+Ton6*/+Ton7+Ton8+Ton9+Ton10 + Ton11;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);
	
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_Ton = dWeightTot;
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_M   = MTot;

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;	
	Dis2 =  W3+(W5*2/3);
	Dis3 =  (W3+W5)/2;

	// Slab�� ������ ������ ���				
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3);		
	
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_Ton = tokN(Ton1+Ton2+Ton3);
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_M   = MTotE;	
}

// ���� : ���� ��ȣ�� (���� Ÿ��) ���� ���(W3 ���� ��, H1+H2+H3 != 0)
// �Է� : &arr
//		  LEN	: ��ȣ�� �ʺ�(mm)
// ��� : &arr

void CCalcFloor::CalcGuardLeftType4(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs = (_T(""));
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();		
	CCalcData        *pCalcData = m_pDataManage->GetCalcData();
	CPlateGirderApp    *pGir = pBridge->GetGirder(0);
	
	double	PaveT		= pBridge->m_dThickPave;		// ���� �β�	
	double	D1			= 0.0;
	double	W3			= 0.0;
	double	Height		= 0.0;
	double	Width		= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
		D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3		= pSep->m_W3;
		Height	= pSep->m_H1 + pSep->m_H2 + pSep->m_H3;
		Width	= pSep->GetWidth();		// ��ȣ��
	}

	double	BaseLen	= 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	double	Ton1	= 0.0;

	Ton1 = W3*(Height)*m_dAppConcreteEC   * BaseLen;	

	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CStringArray ArrSlab;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double	Ton5	= PaveT * (Term-LEN) * pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt  * BaseLen;
	double	Ton6	= 0.0;
	double	Ton7	= 0.0;
	double	Ton8	= 0.0;
	double	Ton9	= 0.0;
	double	BaseX	= Term;
	double  Dis1	= BaseX-(D1+W3/2);
	double	Dis5	= (BaseX-Width)/2;
	double	Dis6	= 0.0;
	double	Dis7	= 0.0;
	double	M       = 0,	MTot = 0,	MTotE = 0;
	double dUWeigtConc = m_dAppConcreteEC;

	m_dPreDeadLoad[0] = dTotSlabMoment;//Ton2*Dis2 + Ton3*Dis3 + Ton4*Dis4;
	m_dPostDeadLoad[0]	= Ton1*Dis1 + Ton5*Dis5;
	// Area One
	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(Height), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)//�ƽ���Ʈ�����϶�
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton5) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis5) );
		arr.Add(cs);
		M = Ton5 * Dis5;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else//���������϶�
	{
		double dWalkLoad1 = (pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1);
		//double dLengthHDan	= (pBridge->GetLengthHDanDom(pBx, 1));
		double dBodoMomentL = (Term-(D1+Width));//���� ���������� ���Ʈ ���ؼ������� �Ÿ�...
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
		double T1 = dWalkLoad1;
		// double T2 = dWalkLoad1 + dBodoMomentL / dLengthHDan * (dWalkLoad2 - dWalkLoad1);
		Ton5 = (T1 + T2) * 0.5 * dBodoMomentL*1000;
		Dis5 = (T1+T2) == 0 ? 0 : (dBodoMomentL)/3 * (2*T1+T2)/(T1+T2);//��𸮲� ���� ���ϴ� ����..
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(dBodoMomentL));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton5));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis5));
		arr.Add(cs);
		M = Ton5 * Dis5;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	//����
	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton6 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis6 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton6));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis6));
			arr.Add(cs);
			M = Ton6 * Dis6;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100) 
		{
			Ton7 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis7 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis7));
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];

	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	double dWeightTot = Ton1+dTotSlabWeight/*Ton2+Ton3+Ton4*/+Ton5+Ton6+Ton7+Ton8+Ton9;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_M   = (MTot);
	
	// Slab�� ������ ������ ���	
	Dis1 =  Width/2;
	
	MTotE = (Ton1*Dis1);

	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_Ton = (Ton1);
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_M   = MTotE;
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = Ton1;

}

// ���� : ���� ��ȣ�� (���� Ÿ��) ���� ���(W1, W2, W3, W4, W5, H1, H2, H3 ���� ��)
// �Է� : &arr
//		  LEN	: ��ȣ�� �ʺ�(mm)
// ��� : &arr
void CCalcFloor::CalcGuardLeftType5(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);
	
	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double D1 = 0;
	double W1 = 0;
	double W2 = 0;
	double W3 = 0;
	double W4 = 0;
	double W5 = 0;
	double H1 = 0;
	double H2 = 0;
	double H3 = 0;
	double Width = 0;	
	
	if(m_nTypeHDan == HT_GUARDFENCE || m_nTypeHDan == HT_CENTERGUARDFENCE)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W1 = pSep->m_W1;
		W2 = pSep->m_W2;
		W3 = pSep->m_W3;
		W4 = pSep->m_W4;
		W5 = pSep->m_W5;
		H1 = pSep->m_H1;		// ���� �β� ����
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double Term = pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	
	double Ton1 = 0;
	double Ton2 = 0;
	double Ton3 = 0;
	double Ton4 = 0;
	double Ton5 = 0;
	double Ton6 = 0;
	double Ton7 = 0;
	double Ton8  = 0;
	double Ton9  = 0;
	double Ton10 = 0;
	double Ton11 = 0;
	double Ton12 = 0;

	Ton1 = W3 * H3 * m_dAppConcreteEC * BaseLen;
	Ton2 = (W2 * H3)/2 * m_dAppConcreteEC * BaseLen;
	Ton3 = (W4 * H3)/2 * m_dAppConcreteEC * BaseLen;
	Ton4 = (W2 + W3 + W4) * H2 * m_dAppConcreteEC * BaseLen;
	Ton5 = (W1 * H2)/2 * m_dAppConcreteEC * BaseLen;
	Ton6 = (W5 * H2)/2 * m_dAppConcreteEC * BaseLen;
	Ton7 = (W1+W2+W3+W4+W5) * H1 * m_dAppConcreteEC * BaseLen;
	Ton8 = (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	

	double BaseX = Term;
	double Dis1  = BaseX-(D1+W1+W2+W3/2.0);
	double Dis2  = BaseX-(D1+W1+W2/3.0*2);
	double Dis3  = BaseX-(D1+W1+W2+W3+W4/3.0);
	double Dis4  = BaseX-(D1+W1+(W2+W3+W4)/2.0);
	double Dis5  = BaseX-(D1+W1/3.0*2);
	double Dis6  = BaseX-(D1+W1+W2+W3+W4+W5/3.0);
	double Dis7  = BaseX-(D1+(W1+W2+W3+W4+W5)/2);
	double Dis8  = (BaseX-LEN)/2;
	double Dis9  = 0;
	double Dis10 = 0;
	double Dis11 = 0;
	double M = 0,	MTot = 0, MTotE = 0;

	m_dPreDeadLoad[0] = dTotSlabMoment;//Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6;
	m_dPostDeadLoad[0] = Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6 + Ton7*Dis7 + Ton8*Dis8;

	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	if(W2==W4 && W1==W5)
	{
		cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H3), tokNPM3(dUWeigtConc));			arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton1));															arr.Add(cs);
		cs.Format("%.5f ", toM(Dis1));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton1 * Dis1));													arr.Add(cs);
		MTot = Ton1 * Dis1;

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W2), toM(H3), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton2));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis2));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton2 * Dis2));													arr.Add(cs);
		MTot += (Ton2 * Dis2);

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W4), toM(H3), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton3));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis3));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton3 * Dis3));													arr.Add(cs);
		MTot += Ton3 * Dis3;

		cs.Format("  %.3f �� %.3f �� %.3f", toM(W2+W3+W4), toM(H2), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton4));															arr.Add(cs);
		cs.Format("%.5f ", toM(Dis4));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton4 * Dis4));													arr.Add(cs);
		MTot += Ton4 * Dis4;

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W1), toM(H2), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton5));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis5));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton5 * Dis5));													arr.Add(cs);
		MTot += Ton5 * Dis5;

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W5), toM(H2), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton6));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis6));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton6 * Dis6));													arr.Add(cs);
		MTot += Ton6 * Dis6;

		cs.Format("  %.3f �� %.3f �� %.3f", toM(W1+W2+W3+W4+W5), toM(H3), tokNPM3(dUWeigtConc));arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton7));															arr.Add(cs);
		cs.Format("%.5f ", toM(Dis7));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton7 * Dis7));													arr.Add(cs);
		MTot += Ton7 * Dis7;
	}
	else
		AfxMessageBox("���Ʈ �Ÿ��� ���� �� ���� ����� ������ �� �����ϴ�.");
	long n = 0;
	for(n = 0; n < ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton8) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis8) );
		arr.Add(cs);
		M = Ton8 * Dis8;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;	///< N/mm2
// 		double dLengthHDan	= (pBridge->GetLengthHDanDom(pBx, 1));
// 		double dBodoMomentL = (Term-(D1+Width)-dQuater-WebT/2);//���� ���������� ���Ʈ ���ؼ������� �Ÿ�...
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
		double T1 = dWalkLoad1;
		//double T2 = (dWalkLoad1 + (Term-(D1+Width)) / pBridge->GetLengthHDanDom(pBx, 1) * (dWalkLoad2 - dWalkLoad1));
		Ton9 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis9 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton9));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis9));
		arr.Add(cs);
		M = Ton9 * Dis9;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton10 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis10 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton10));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis10));
			arr.Add(cs);
			M = Ton10 * Dis10;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100)
		{
			Ton11 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis11 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton11));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis11));
			arr.Add(cs);
			M = Ton11 * Dis11;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];

	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = Ton1+Ton2+Ton3+Ton4+Ton5+Ton6+Ton7+Ton8+Ton9+Ton10+Ton11+Ton12+dTotSlabWeight;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);

	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  W1 + W2 + W3/2;	
	Dis2 =  W1 + W2 + W3/2;
	Dis3 =  W1 + W2 + W3/2;

	// Slab�� ������ ������ ���		
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3) + (Ton4*Dis4) + (Ton5*Dis5) + (Ton6*Dis6) + (Ton7*Dis7) + (Ton8*Dis8);

	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_Ton = (Ton1+Ton2+Ton3+Ton4+Ton5+Ton6+Ton7);
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = Ton1+Ton2+Ton3+Ton4+Ton5+Ton6+Ton7;
}

//	���� :	���� ��ȣ�� (���� Ÿ��) ���� ���(W3, W5, H2, H3 ���� ��)
//	�Է� :	&arr
//			LEN(��ȣ�� �ʺ�(mm))
//	��� :	&arr
void CCalcFloor::CalcGuardLeftType6(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();		
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1 = 0;
	double	W3 = 0;
	double	W5 = 0;
	double	H2 = 0;
	double	H3 = 0;
	double	Width = 0;
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3 = pSep->m_W3;
		W5 = pSep->m_W5;
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)s
	double Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	
	double Ton1 =  0;
	double Ton2 =  0;
	
	Ton1 = (W3*(H2+H3))*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H2/2.0)*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton6  =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton7  = 0;
	double Ton8  = 0;
	double Ton9  = 0;
	double BaseX = Term;
	double Dis1  = BaseX-(D1+W3/2.0);
	double Dis2  = BaseX-(D1+W3+W5*1/3);
	double Dis6  = (BaseX-LEN)/2;
	double Dis7  = 0;
	double Dis8  = 0;
	double M = 0,	MTot = 0, MTotE = 0;

	m_dPreDeadLoad[0] = dTotSlabMoment;//Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5;
	m_dPostDeadLoad[0] = Ton1*Dis1 + Ton2*Dis2 + Ton6*Dis6;

	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	
	cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H3+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format( "  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2), tokNPM3(dUWeigtConc));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6) );
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;	///< N/mm2
		double T1 = dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
		Ton6 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis6 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6));
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis7));
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100)
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = Ton1+Ton2+dTotSlabWeight/*Ton3+Ton4+Ton5*/+Ton6+Ton7+Ton8+Ton9;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// ��Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );		// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_M   = (MTot);
	
	// Slab�� ������ ������ ���
	Dis1 =  W3/2;
	Dis2 =  W3+(W5*2/3);	

	// Slab�� ������ ������ ���		
	MTotE = (Ton1*Dis1) + (Ton2*Dis2);	
	
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_Ton = (Ton1+Ton2);
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_M   = (MTotE);	
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = Ton1 + Ton2;
}

// ���� : ���� ��ȣ�� (���� Ÿ��) ���� ���(W3, W5, H2 �� �������)
// �Է� : &arr
// LEN	: ��ȣ�� �ʺ�(mm)
// ��� : &arr
void CCalcFloor::CalcGuardLeftType7(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();		
	CCalcData        *pCalcData = m_pDataManage->GetCalcData();
	CPlateGirderApp  *pGir      = pBridge->GetGirder(0);

	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1	    = 0;
	double	W3		= 0;
	double	W5		= 0;
	double	H2		= 0;
	double	Width   = 0;
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3 = pSep->m_W3;
		W5 = pSep->m_W5;
		H2 = pSep->m_H2;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double	BaseLen	= 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	double	Ton1	= 0;
	double	Ton2	= 0;
	
	Ton1 = (W3*H2)*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H2/2.0)*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double	dTotSlabWeight = 0;
	double	dTotSlabMoment = 0;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double	Ton6	= PaveT * (Term-LEN) * pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double	Ton7	= 0;
	double	Ton8	= 0;
	double	Ton9	= 0;
	double	Ton10	= 0;
	double	BaseX	= Term;
	double	Dis1	= BaseX-(D1+W3/2);
	double	Dis2	= BaseX-(D1+W3+W5*1/3);
	double	Dis6	= (BaseX-LEN)/2;
	double	Dis7	= 0;
	double	Dis8	= 0;
	double	M		= 0, MTot = 0, MTotE = 0;
	double  dUWeigtConc = m_dAppConcreteEC;

	m_dPreDeadLoad[0] = dTotSlabWeight;//Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5;
	m_dPostDeadLoad[0] = Ton1*Dis1 + Ton2*Dis2 + Ton6*Dis6;
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format( "  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2), tokNPM3(dUWeigtConc));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6) );
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;	///< N/mm2
		double dBodoMomentL = Term-LEN;//���� ���������� ���Ʈ ���ؼ������� �Ÿ�...
		double T1 = dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
		Ton6 = (T1 + T2) * 0.5 * (dBodoMomentL)*1000;
		Dis6 = (T1+T2) == 0 ? 0 : (dBodoMomentL)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(dBodoMomentL));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6));
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	//����
	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton7 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis7 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis7));
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	double dWeightTot = Ton1+Ton2+dTotSlabWeight/*Ton3+Ton4+Ton5*/+Ton6+Ton7+Ton8+Ton9+Ton10;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));		// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_M   = (MTot);
	// Slab�� ������ ������ ���
	Dis1 =  W3/2;
	Dis2 =  W3+(W5*2/3);	
	// Slab�� ������ ������ ���		
	MTotE = (Ton1*Dis1) + (Ton2*Dis2);	
	
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_Ton = (Ton1+Ton2);			
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = Ton1 + Ton2;
}
// ���� : ���� ��ȣ�� (���� Ÿ��) ���� ���(W3, W5, H1, H3 ���� ��)
// �Է� : &arr
// LEN	: ��ȣ�� �ʺ�(mm)
// ��� : &arr
void CCalcFloor::CalcGuardLeftType8(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();		
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1 = 0;
	double	W3 = 0;
	double	W5 = 0;
	double	H1 = 0;
	double  H3 = 0;
	double	Width = 0;
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3 = pSep->m_W3;
		W5 = pSep->m_W5;
		H1 = pSep->m_H1;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double	BaseLen	= 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	
	double Ton1 =  0;
	double Ton2 =  0;
	
	Ton1 = (W3*(H1+H3))*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H1)*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton6  =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton7  = 0;
	double Ton8  = 0;
	double Ton9  = 0;
	double Ton10 = 0;
	double BaseX = Term;
	double Dis1  = BaseX-(D1+W3/2.0);
	double Dis2  = BaseX-(D1+W3+W5/2.0);
	double Dis6  = (BaseX-LEN)/2;
	double Dis7  = 0;
	double Dis8  = 0;
	double M = 0,	MTot = 0, MTotE = 0;

	m_dPreDeadLoad[0] = dTotSlabMoment;//Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5;
	m_dPostDeadLoad[0] = Ton1*Dis1 + Ton2*Dis2 + Ton6*Dis6;

	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;

	
	cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H1+H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format( "  %.3f �� %.3f �� %.3f", toM(W5), toM(H1), tokNPM3(dUWeigtConc));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	
	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6) );
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoad1;	///< N/mm2
		double T1 = dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dWalkLoadMoment;
		Ton6 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis6 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6));
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", Dis7);
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bNanGan && Width>100)
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	double dWeightTot = Ton1+Ton2+dTotSlabWeight/*Ton3+Ton4+Ton5*/+Ton6+Ton7+Ton8+Ton9+Ton10;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// ��Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);	
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[0].m_Stop_M   = (MTot);
	
	// Slab�� ������ ������ ���
	Dis1 =  W3/2;
	Dis2 =  W3+W5/2;	

	// Slab�� ������ ������ ���		
	MTotE = (Ton1*Dis1) + (Ton2*Dis2);	
	
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_Ton = (Ton1+Ton2);			
	pCalcData->CALC_CANTILEVER_DATA[0].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = Ton1 + Ton2;
}

//0�� Ÿ�԰� ���常 �ٸ�
void CCalcFloor::CalcGuardLeftType99(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();		
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);

	double	PaveT	= pBridge->m_dThickPave;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
		if(!pSep)	return;
	}

	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabLeft(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton4 =  Term*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton5 = 0;
	double Ton6 = 0;
	double Ton7 = 0;
	double BaseX = Term;

	double Dis4  =	BaseX/2;

	m_dPreDeadLoad[0] = dTotSlabMoment;//Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3;
	m_dPostDeadLoad[0] = Ton4*Dis4;

	double M = 0,	MTot = 0;
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	// 4
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton4) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis4) );
		arr.Add(cs);
		M = Ton4 * Dis4;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;

	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			double dDist = Term - pCalcData->m_ELoadDisArr[n];
			if(dDist > 0 && Term > dDist)
			{
				dExtraLoad[n] = pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n] = dDist;
				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = dTotSlabWeight/*Ton1+Ton2+Ton3*/+Ton4+Ton5+Ton6+Ton7;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// 5.Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);

	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Stop_Ton = (dWeightTot + Ton4);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Stop_M   = (MTot);

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_Ton = 0;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_StopE_M   = 0;
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_LEFT].m_dBangHoWeight = 0;
}

void CCalcFloor::CalcGuardLeftType100(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	cs.Format("�� ��ȣ���� �������� �ʴ� ����Դϴ�.");
	arr.Add(cs);
	arr.Add("");
	arr.Add("");
	arr.Add("");
	arr.Add("");
	arr.Add("");
}

long CCalcFloor::CalcSlabRightDeadLoad(CStringArray& arr)
{

	CPlateBridgeApp  *pBridge = m_pDataManage->GetBridge();			
	CPlateBasicIndex *pBx     = pBridge->GetGirder(pBridge->GetGirdersu()-1)->GetCrossBxAtMinFlangeWidth();	

	double LEN = pBridge->GetLengthHDanDom(pBx, pBridge->GetQtyHDanNode()-1);
	long nType = GetGuardWallType(FLOOR_RIGHT);	

	if(nType==99)	LEN = 0;

	switch( nType )
	{
		case   0: CalcGuardRightType0(pBx, arr, LEN);	break;		
		case   1: CalcGuardRightType1(pBx, arr, LEN);	break;
		case   2: CalcGuardRightType2(pBx, arr, LEN);	break;
		case   3: CalcGuardRightType3(pBx, arr, LEN);	break;
		case   4: CalcGuardRightType4(pBx, arr, LEN);	break;
		case   5: CalcGuardRightType5(pBx, arr, LEN);	break;
		case   6: CalcGuardRightType6(pBx, arr, LEN);	break;
		case   7: CalcGuardRightType7(pBx, arr, LEN);	break;
		case   8: CalcGuardRightType8(pBx, arr, LEN);	break;
		case  99: CalcGuardRightType99(pBx, arr, LEN);	break;
		case 100: CalcGuardRightType100(pBx, arr, LEN);	break;
	}
	
	return nType;
}

void CCalcFloor::CalcGuardRightType0(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);
		
	long	nSize=pBridge->GetQtyHDanNode();

	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT = pBridge->m_dThickPave;		// ���� �β�
	double	BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)

	CStringArray ArrSlab;
	double dTotSlabMoment = 0;
	double dTotSlabWeight = 0;

	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton4 = (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton5 = 0;
	double Ton6 = 0;
	double Ton7 = 0;
	double BaseX = Term;

	double Dis4 = (BaseX-LEN)/2;

	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3;
	m_dPostDeadLoad[1] = Ton4*Dis4;
	double M = 0, MTot = 0;
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	
	// 4	���� ���������̶��

	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton4) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis4) );
	arr.Add(cs);
	M = Ton4 * Dis4;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);

	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad)
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	
	double dWeightTot = dTotSlabWeight/*Ton1+Ton2+Ton3*/+Ton4+Ton5+Ton6+Ton7;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];	
	// 5 Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );					// T*M Total
	arr.Add(cs);

	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton = 0;
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M = 0;
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = 0;

}

void CCalcFloor::CalcGuardRightType1(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�
	double	D1 = 0 ;
	double	W3 = 0;
	double	W4 = 0;
	double	W5 = 0;
	double	H1 = 0;
	double	H2 = 0;
	double	H3 = 0;
	double	Width = 0;		// ��ȣ��
	double	D2 = 0 ;

	long nSize=pBridge->GetQtyHDanNode();
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1 = pSep->m_D1;
		W3 = pSep->m_W3;
		W4 = pSep->m_W2;
		W5 = pSep->m_W1;
		H1 = pSep->m_H1;
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;		
		Width = pSep->GetWidth();		// ��ȣ��
		D2 = LEN - Width - D1;
	}

	double BaseLen = 1000;					//���� ����(�߷� ���� �� ��� 1m)

	double Ton1 =  0;
	double Ton2 =  0;
	double Ton3 =  0;
	double Ton4 =  0;
	double Ton5 =  0;

	Ton1 = H3*W3*m_dAppConcreteEC* BaseLen;
	Ton2 = H3*W4*m_dAppConcreteEC/2 * BaseLen;
	Ton3 = (H1+H2)*(W3+W4)*m_dAppConcreteEC * BaseLen;
	Ton4 = H2*W5*m_dAppConcreteEC/2 * BaseLen;
	Ton5 = H1*W5*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton9  =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton10 = 0;
	double Ton11 = 0;
	double Ton12 = 0;
	double Ton13 = 0;
	double BaseX = Term;
	double Dis1  = BaseX-D2-W3/2;
	double Dis2  = BaseX-D2-W3-(W4/3);
	double Dis3  = BaseX-D2-(W3+W4)/2;
	double Dis4  = BaseX-D2-W3-W4-(W5/3);
	double Dis5  = BaseX-D2-(W3+W4)-W5/2;
	double Dis9  = (BaseX-LEN)/2;
	double Dis10 = 0;
	double Dis11 = 0;
	double Dis13 = 0;

	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton6*Dis6 + Ton7*Dis7 + Ton8*Dis8;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5 + Ton9*Dis9 +Ton13*Dis13;

	double M = 0, MTot = 0, MTotE = 0;	

	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;

	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W4), toM(H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W4+W3), toM(H1+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton3) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis3) );
	arr.Add(cs);
	M = Ton3 * Dis3;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2),   tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton4) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis4) );
	arr.Add(cs);
	M = Ton4 * Dis4;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W5), toM(H1),   tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton5) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis5) );
	arr.Add(cs);
	M = Ton5 * Dis5;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n = 0; n < ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton9) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis9) );
		arr.Add(cs);
		M = Ton9 * Dis9;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;		///< N/mm2
		double T1 = dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton9 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis9 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton9));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis9));
		arr.Add(cs);
		M = Ton9 * Dis9;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && Width>100)
		{
			Ton10 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis10 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton10));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis10));
			arr.Add(cs);
			M = Ton10 * Dis10;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100)
		{
			Ton11 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis11 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton11));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis11));
			arr.Add(cs);
			M = Ton11 * Dis11;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
	}

	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	MTot+=dTotSlabMoment;
	double dWeightTot = Ton1+Ton2+Ton3+Ton4+Ton5+dTotSlabWeight/*Ton6+Ton7+Ton8*/+Ton9+Ton10+Ton11+Ton12+ Ton13;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];

	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));		// T*M Total
	arr.Add(cs);

	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;	
	Dis2 =  W3+W4/3;
	Dis3 =  (W3+W4)/2;
	Dis4 =  W3+W4+(W5/3);
	Dis5 =  W3+W4+W5/2;

	// Slab�� ������ ������ ���		
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3) + (Ton4*Dis4) + (Ton5*Dis5);

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_StopE_Ton = (Ton1+Ton2+Ton3+Ton4+Ton5);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = (Ton1+Ton2+Ton3+Ton4+Ton5);

}
  
void CCalcFloor::CalcGuardRightType2(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{

	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1		= 0.0;
	double	W3		= 0.0;
	double	W4		= 0.0;
	double	H1		= 0.0;
	double	H2		= 0.0;
	double	H3		= 0.0;
	double	Width	= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3		= pSep->m_W3;
		W4		= pSep->m_W2;
		H1		= pSep->m_H1;
		H2		= pSep->m_H2;
		H3		= pSep->m_H3;
		Width	= pSep->GetWidth();		// ��ȣ��		
	}

	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double D2 = LEN - Width - D1;
	
	double Ton1	=  0.0;
	double Ton2	=  0.0;
	double Ton3	=  0.0;
	
	Ton1 =  W3 * H3 * m_dAppConcreteEC * BaseLen;
	Ton2 =  W4 * H3 * m_dAppConcreteEC/2 * BaseLen;
	Ton3 =  (W3+W4) * (H1+H2) * m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double	Ton7 =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double	Ton8 =  0.0;
	double	Ton9 =  0.0;
	double	Ton10 = 0.0;
	double  Ton11 = 0.0;
	double	BaseX =	Term;
	double	Dis1  =	BaseX-D2-W3/2;	
	double	Dis2  =	BaseX-D2-W3-(W4/3);
	double	Dis3  =	BaseX-D2-(W3+W4)/2;
	double	Dis7  =	(BaseX-LEN)/2;
	double	Dis8  =	0.0;
	double	Dis9  =	0.0;

	double	M = 0,	MTot = 0,	MTotE = 0;

	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton7*Dis7;

	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	
	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W4), toM(H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);

	// ��	
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W3+W4), toM(H1+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton3) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis3) );
	arr.Add(cs);
	M = Ton3 * Dis3;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7) );
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;	///< N/mm2
		double T1 = dWalkLoad1;
		//double T2 = dWalkLoad1 + (Term-(D2+Width)) / pBridge->GetLengthHDanDom(pBx, nSize-2) * (dWalkLoad2 - dWalkLoad1);
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton7 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis7 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7));
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}
	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100)
		{
			Ton9 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis9 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton9));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis9));
			arr.Add(cs);
			M = Ton9 * Dis9;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = Ton1+Ton2+Ton3+dTotSlabWeight/*Ton4+Ton5+Ton6*/+Ton7+Ton8+Ton9+Ton10+Ton11;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];

	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));		// T*M Total
	arr.Add(cs);

	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;	
	Dis2 =  W3+(W4*2/3);
	Dis3 =  (W3+W4)/2;

	// Slab�� ������ ������ ���				
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3);		
	
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_StopE_Ton = (Ton1+Ton2+Ton3);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = Ton1+Ton2+Ton3;

}

void CCalcFloor::CalcGuardRightType3(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1		= 0.0;
	double	W3		= 0.0;
	double	W5		= 0.0;
	double	H1		= 0.0;
	double	H2		= 0.0;
	double	H3		= 0.0;
	double	Width	= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3		= pSep->m_W3;
		W5		= pSep->m_W1;
		H1		= pSep->m_H1;
		H2		= pSep->m_H2;
		H3		= pSep->m_H3;
		Width	= pSep->GetWidth();		// ��ȣ��		
	}

	double BaseLen	= 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double D2		= LEN - Width - D1;
	
	double Ton1		= 0.0;
	double Ton2		= 0.0;
	double Ton3		= 0.0;

	Ton1 =  W3*(H3+H2)*m_dAppConcreteEC * BaseLen;
	Ton2 =  W5*H2*m_dAppConcreteEC/2 * BaseLen;
	Ton3 =  (W3+W5)*(H1)*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double	Ton7  =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double	Ton8  =  0.0;
	double	Ton9  =  0.0;
	double	Ton10 = 0.0;
	double	Ton11 = 0.0;
	double	BaseX =	Term;
	double  Dis1  = BaseX-D2-W3/2;	
	double  Dis2  = BaseX-D2-W3-(W5/3);
	double  Dis3  = BaseX-D2-(W3+W5)/2;

	double	Dis7	= (BaseX-LEN)/2;
	double	Dis8	= 0.0;
	double	Dis9	= 0.0;
	double	M = 0,	MTot = 0,	MTotE = 0;
	
	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton7*Dis7;

///////////////////////////////////////////////////////////////////////////////
	double dUWeigtConc = m_dAppConcreteEC;
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(H3+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	
	// ��	
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(W3+W5), toM(H1), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton3) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis3) );
	arr.Add(cs);
	M = Ton3 * Dis3;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n = 0; n < ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7) );
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double T1 = dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton7 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis7 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton7));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis7));
		arr.Add(cs);
		M = Ton7 * Dis7;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100)
		{
			Ton9 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis9 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton9));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis9));
			arr.Add(cs);
			M = Ton9 * Dis9;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = Ton1+Ton2+Ton3+dTotSlabWeight/*Ton4+Ton5+Ton6*/+Ton7+Ton8+Ton9+Ton10+Ton11;
	MTot += dTotSlabMoment;

	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;
	Dis2 =  W3+(W5*2/3);
	Dis3 =  W3+W5/2;

	// Slab�� ������ ������ ���		
	
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3);		

	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton = (Ton1+Ton2+Ton3+Ton8);
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = Ton1+Ton2+Ton3;
}

void CCalcFloor::CalcGuardRightType4(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();		
	CCalcData        *pCalcData = m_pDataManage->GetCalcData();
	CPlateGirderApp  *pGir      = pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	PaveT		= pBridge->m_dThickPave;		// ���� �β�	
	double	D1			= 0.0;
	double	W3			= 0.0;
	double	Height		= 0.0;
	double	Width		= 0.0;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1		= pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3		= pSep->m_W3;
		Height	= pSep->m_H1 + pSep->m_H2 + pSep->m_H3;
		Width	= pSep->GetWidth();		// ��ȣ��
	}

	double	BaseLen	= 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double	D2		= LEN - Width - D1;
	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	Ton1	= 0.0;

	Ton1 = W3*(Height)*m_dAppConcreteEC * BaseLen;
	
	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double	Ton5	= PaveT * (Term-LEN) * pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt*BaseLen;
	double	Ton6	= 0.0;
	double	Ton7	= 0.0;
	double	Ton8	= 0.0;
	double	Ton9	= 0.0;
	double	BaseX	= Term;
	double  Dis1	= BaseX-(D2+W3/2);
	double	Dis5	= (BaseX-LEN)/2;
	double	Dis6	= 0.0;
	double	Dis7	= 0.0;

	double	M       = 0,	MTot = 0,	MTotE = 0;
	double dUWeigtConc = m_dAppConcreteEC;

	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton2*Dis2 + Ton3*Dis3 + Ton4*Dis4;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton5*Dis5;
	// Area One
	cs.Format("  %.3f �� %.3f �� %.3f ", toM(W3), toM(Height), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// Area Five
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)//�ƽ���Ʈ�����϶�
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton5) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis5) );
		arr.Add(cs);
		M = Ton5 * Dis5;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else//���������϶�
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;		
		double dBodoMomentL = (Term-LEN);//���� ���������� ���Ʈ ���ؼ������� �Ÿ�...
		//ȯ���ϰڵ�...WebT/2�� �Ǿ� �ִٴ�...
		double T1 = dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton5 = (T1 + T2) * 0.5 * (dBodoMomentL)*1000;
		Dis5 = (T1+T2) == 0 ? 0 : (dBodoMomentL)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(dBodoMomentL));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton5));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis5));
		arr.Add(cs);
		M = Ton5 * Dis5;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton6 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis6 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton6));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis6));
			arr.Add(cs);
			M = Ton6 * Dis6;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100) 
		{
			Ton7 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis7 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis7));
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if((dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	double dWeightTot = Ton1+dTotSlabWeight/*Ton2+Ton3+Ton4*/+Ton5+Ton6+Ton7+Ton8+Ton9;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  Width/2;
	MTotE = (Ton1*Dis1);

	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton = (Ton1);	
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = Ton1;
}

void CCalcFloor::CalcGuardRightType5(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);
	
	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT = pBridge->m_dThickPave;		// ���� �β�	
	double	D1 = 0;
	double	W1 = 0;
	double	W2 = 0;
	double	W3 = 0;
	double	W4 = 0;
	double	W5 = 0;
	double	H1 = 0;
	double	H2 = 0;
	double	H3 = 0;
	double	Width = 0;	
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W1 = pSep->m_W5;
		W2 = pSep->m_W4;
		W3 = pSep->m_W3;
		W4 = pSep->m_W2;
		W5 = pSep->m_W1;
		H1 = pSep->m_H1;		// ���� �β� ����
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	
	double Ton1 = 0;
	double Ton2 = 0;
	double Ton3 = 0;
	double Ton4 = 0;
	double Ton5 = 0;
	double Ton6 = 0;
	double Ton7 = 0;
	double Ton8  = (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton9  = 0;
	double Ton10 = 0;
	double Ton11 = 0;
	Ton1 = W3 * H3 * m_dAppConcreteEC * BaseLen;//(1.0/2.0*(W3 + (W2+W3+W4)) * H3)*m_dAppConcreteEC * BaseLen;
	Ton2 = (W2 * H3)/2 * m_dAppConcreteEC * BaseLen;//(1.0/2.0*(W2+W3+W4 + Width)*H2)*m_dAppConcreteEC * BaseLen;
	Ton3 = (W4 * H3)/2 * m_dAppConcreteEC * BaseLen;//(Width*H1)*m_dAppConcreteEC * BaseLen;
	Ton4 = (W2+W3+W4) * H2 * m_dAppConcreteEC * BaseLen;
	Ton5 = (W1 * H2) / 2 * m_dAppConcreteEC * BaseLen;
	Ton6 = (W5 * H2) / 2 * m_dAppConcreteEC * BaseLen;
	Ton7 = (W1+W2+W3+W4+W5) * H1 * m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);
	
	double BaseX = Term;
	double Dis1  = BaseX-(D1+W1+W2+W3/2.0);
	double Dis2  = BaseX-(D1+W1+W2/3.0*2);
	double Dis3  = BaseX-(D1+W1+W2+W3+W4/3.0);
	double Dis4  = BaseX-(D1+W1+(W2+W3+W4)/2.0);
	double Dis5  = BaseX-(D1+W1/3.0*2);
	double Dis6  = BaseX-(D1+W1+W2+W3+W4+W5/3.0);
	double Dis7  = BaseX-(D1+(W1+W2+W3+W4+W5)/2);
	double Dis8  = (BaseX-LEN)/2;
	double Dis9  = 0;
	double Dis10 = 0;
	double Dis11 = 0;

	double M = 0,	MTot = 0, MTotE = 0;
	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5 + Ton6*Dis6 + Ton7*Dis7 + Ton8*Dis8;
	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	if(W2==W4 && W1==W5)
	{
		cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H3), tokNPM3(dUWeigtConc));			arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton1));															arr.Add(cs);
		cs.Format("%.5f ", toM(Dis1));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton1 * Dis1));													arr.Add(cs);
		MTot = Ton1 * Dis1;

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W2), toM(H3), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton2));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis2));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton2 * Dis2));													arr.Add(cs);
		MTot += (Ton2 * Dis2);

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W4), toM(H3), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton3));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis3));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton3 * Dis3));													arr.Add(cs);
		MTot += Ton3 * Dis3;

		cs.Format("  %.3f �� %.3f �� %.3f", toM(W2+W3+W4), toM(H2), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton4));															arr.Add(cs);
		cs.Format("%.5f ", toM(Dis4));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton4 * Dis4));													arr.Add(cs);
		MTot += Ton4 * Dis4;

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W1), toM(H2), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton5));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis5));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton5 * Dis5));													arr.Add(cs);
		MTot += Ton5 * Dis5;

		cs.Format("  1/2 �� %.3f �� %.3f �� %.3f", toM(W5), toM(H2), tokNPM3(dUWeigtConc));		arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton6));															arr.Add(cs);
		cs.Format("%.5f", toM(Dis6));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton6 * Dis6));													arr.Add(cs);
		MTot += Ton6 * Dis6;

		cs.Format("  %.3f �� %.3f �� %.3f", toM(W1+W2+W3+W4+W5), toM(H3), tokNPM3(dUWeigtConc));arr.Add(cs);
		cs.Format("%.5f ", tokN(Ton7));															arr.Add(cs);
		cs.Format("%.5f ", toM(Dis7));															arr.Add(cs);
		cs.Format("%.5f ", tokNM(Ton7 * Dis7));													arr.Add(cs);
		MTot += Ton7 * Dis7;
	}
	else
		AfxMessageBox("���Ʈ �Ÿ��� ���� �� ���� ����� ������ �� �����ϴ�.");

	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton8) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis8) );
		arr.Add(cs);
		M = Ton8 * Dis8;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double T1 = dWalkLoad1;
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton9 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis9 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton9));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis9));
		arr.Add(cs);
		M = Ton9 * Dis9;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}
	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton10 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis10 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton10));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis10));
			arr.Add(cs);
			M = Ton10 * Dis10;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100)
		{
			Ton11 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis11 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton11));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis11));
			arr.Add(cs);
			M = Ton11 * Dis11;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	double dWeightTot = Ton1+Ton2+Ton3+Ton4+Ton5+Ton6+Ton7+Ton8+Ton9+Ton10+Ton11+dTotSlabWeight;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];

	// �� Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));		// T*M Total
	arr.Add(cs);
	
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);
	
	// Slab�� ������ ������ ���	
	Dis1 =  W1 + W2 + W3/2;	
	Dis2 =  W1 + W2 + W3/2;
	Dis3 =  W1 + W2 + W3/2;

	// Slab�� ������ ������ ���		
	MTotE = (Ton1*Dis1) + (Ton2*Dis2) + (Ton3*Dis3) + (Ton4*Dis4) + Ton5*Dis5 + Ton6*Dis6 + Ton7*Dis7 + Ton8*Dis8;	
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton = (Ton1+Ton2+Ton3+Ton4+Ton5+Ton6+Ton7);
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = Ton1+Ton2+Ton3+Ton4+Ton5+Ton6+Ton7;
}

void CCalcFloor::CalcGuardRightType6(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1 = 0;
	double	W3 = 0;
	double	W5 = 0;
	double	H2 = 0;
	double	H3 = 0;
	double	Width = 0;
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3 = pSep->m_W3;
		W5 = pSep->m_W1;
		H2 = pSep->m_H2;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double D2 = LEN - Width - D1;
	
	double Ton1 =  0;
	double Ton2 =  0;

	Ton1 = (W3*(H2+H3))*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H2/2.0)*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton6 =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton7 = 0;
	double Ton8 = 0;
	double Ton9 = 0;

	double BaseX = Term;
	double Dis1  = BaseX-(D2+W3/2.0);
	double Dis2  = BaseX-(D2+W3+W5*1/3);

	double Dis6  =	(BaseX-LEN)/2;
	double Dis7  = 0;
	double Dis8  = 0;

	double M = 0,	MTot = 0, MTotE = 0;
	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton2*Dis2 + Ton6*Dis6;
	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	
	cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H3+H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.3f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.3f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format( "  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2), tokNPM3(dUWeigtConc));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0; ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6) );
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double T1 = dWalkLoad1;
		double T2 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton6 = (T1 + T2) * 0.5 * (Term-LEN)*1000;
		Dis6 = (T1+T2) == 0 ? 0 : (Term-LEN)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(Term-LEN));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6));
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis7));
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100)
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = Ton1+Ton2+dTotSlabWeight/*Ton3+Ton4+Ton5*/+Ton6+Ton7+Ton8+Ton9;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// ��Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));	// T*M Total
	arr.Add(cs);	
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;
	Dis2 =  W3+(W5*2/3);	

	MTotE = (Ton1*Dis1) + (Ton2*Dis2);		
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton = (Ton1+Ton2);		
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = Ton1 + Ton2;

}

void CCalcFloor::CalcGuardRightType7(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp  *pBridge   = m_pDataManage->GetBridge();		
	CCalcData        *pCalcData = m_pDataManage->GetCalcData();
	CPlateGirderApp  *pGir      = pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1 = 0;
	double	W3 = 0;
	double	W5 = 0;
	double	H2 = 0;
	double	Width = 0;
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3 = pSep->m_W3;
		W5 = pSep->m_W1;
		H2 = pSep->m_H2;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double	BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double	D2    = LEN - Width - D1;
	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	Ton1  = 0;
	double	Ton2  = 0;
	
	Ton1 = (W3*H2)*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H2/2.0)*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton6  = PaveT * (Term-LEN) * pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton7  = 0;
	double Ton8  = 0;
	double Ton9  = 0;
	double Ton10 = 0;
	double BaseX = Term;
	double Dis1  = BaseX-(D2+W3/2);
	double Dis2  = BaseX-(D2+W3+W5*1/3);

	double Dis6  = (BaseX-LEN)/2;
	double Dis7  = 0;
	double Dis8  = 0;

	double M = 0,	MTot = 0, MTotE = 0;
	double dUWeigtConc = m_dAppConcreteEC;
	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton2*Dis2 + Ton6*Dis6;
	// ��
	cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H2), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format( "  %.3f �� %.3f �� %.3f �� 1/2  ", toM(W5), toM(H2), tokNPM3(dUWeigtConc));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	// ��
	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad) 
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6) );
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6) );
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M) );
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double dBodoMomentL = (Term-LEN);//���� ���������� ���Ʈ ���ؼ������� �Ÿ�...
		double T1 = dWalkLoad1;
		//double T2 = dWalkLoad1 + (dBodoMomentL) / pBridge->GetLengthHDanDom(pBx, nSize-2) * (dWalkLoad2 - dWalkLoad1);
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton6 = (T1 + T2) * 0.5 * (dBodoMomentL)*1000;
		Dis6 = (T1+T2) == 0 ? 0 : (dBodoMomentL)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(dBodoMomentL));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6));
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	//����
	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton7 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis7 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis7));
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if((dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}
	double dWeightTot = Ton1+Ton2+dTotSlabWeight/*Ton3+Ton4+Ton5*/+Ton6+Ton7+Ton8+Ton9+Ton10;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// ��Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot));											// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;
	Dis2 =  W3+(W5*2/3);	

	MTotE = (Ton1*Dis1) + (Ton2*Dis2);		
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton            = (Ton1+Ton2);
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M              = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = Ton1 + Ton2;

}

void CCalcFloor::CalcGuardRightType8(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT	= pBridge->m_dThickPave;		// ���� �β�	
	double	D1 = 0;
	double	W3 = 0;
	double	W5 = 0;
	double	H1 = 0;
	double  H3 = 0;
	double	Width = 0;
	
	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
		D1 = pSep->m_D1;		//���� ��ȣ���� ���� �� ����
		W3 = pSep->m_W3;
		W5 = pSep->m_W1;
		H1 = pSep->m_H1;
		H3 = pSep->m_H3;
		Width = pSep->GetWidth();		// ��ȣ��		
	}
	
	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)
	double D2 = LEN - Width - D1;
	
	double Ton1 =  0;
	double Ton2 =  0;
	
	Ton1 = (W3*(H1+H3))*m_dAppConcreteEC * BaseLen;
	Ton2 = (W5*H1)*m_dAppConcreteEC * BaseLen;

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);

	double Ton6  =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton7  = 0;
	double Ton8  = 0;
	double Ton9  = 0;
	double Ton10 = 0;
	double BaseX = Term;
	double Dis1  = BaseX-(D2+W3/2.0);
	double Dis2  = BaseX-(D2+W3+W5/2.0);
	double Dis6  =	(BaseX-LEN)/2;
	double Dis7  = 0;
	double Dis8  = 0;

	double M = 0,	MTot = 0, MTotE = 0;
	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton3*Dis3 + Ton4*Dis4 + Ton5*Dis5;
	m_dPostDeadLoad[1] = Ton1*Dis1 + Ton2*Dis2 + Ton6*Dis6;
	//------------------------------------------------------------------		
	// ��
	double dUWeigtConc = m_dAppConcreteEC;
	
	cs.Format("  %.3f �� %.3f �� %.3f", toM(W3), toM(H1+H3), tokNPM3(dUWeigtConc) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton1) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis1) );
	arr.Add(cs);
	M = Ton1 * Dis1;
	MTot = M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	// ��
	cs.Format( "  %.3f �� %.3f �� %.3f", toM(W5), toM(H1), tokNPM3(dUWeigtConc));
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton2) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis2) );
	arr.Add(cs);
	M = Ton2 * Dis2;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);
	long n = 0;
	for(n=0;n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));

	if(!pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bWalkLoad)
	{
		cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6));
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	} 
	else
	{
		double dWalkLoad1 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoad1;
		double dBodoMomentL = (Term-LEN);//���� ���������� ���Ʈ ���ؼ������� �Ÿ�...
		double T1 = dWalkLoad1;
		//double T2 = dWalkLoad1 + (dBodoMomentL) / pBridge->GetLengthHDanDom(pBx, nSize-2) * (dWalkLoad2 - dWalkLoad1);
		double T2	= pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dWalkLoadMoment;
		Ton6 = (T1 + T2) * 0.5 * (dBodoMomentL)*1000;
		Dis6 = (T1+T2) == 0 ? 0 : (dBodoMomentL)/3 * (2*T1+T2)/(T1+T2);
		cs.Format(" �������� : 1/2 �� (%.3f + %.3f) �� %.3f", tokNPM2(T1), tokNPM2(T2), toM(dBodoMomentL));
		arr.Add(cs);
		cs.Format( "%.5f ", tokN(Ton6));
		arr.Add(cs);
		cs.Format( "%.5f ", toM(Dis6));
		arr.Add(cs);
		M = Ton6 * Dis6;
		MTot += M;
		cs.Format( "%.5f ", tokNM(M));
		arr.Add(cs);
	}

	if(!(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan))
	{
		// ���� �������� �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bBangEm && pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight != 0 && Width>100) 
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangEmDis;
			cs.Format("  (������ : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton7));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis7));
			arr.Add(cs);
			M = Ton7 * Dis7;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}	
		// ���� ������ �ִٸ�....
		if(pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_bNanGan && Width>100)
		{
			Ton8 = pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanWeight;
			Dis8 = Term - pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dNanGanDis;
			cs.Format("  (���� : ������� ���� ���̴� ����)");
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(Ton8));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(Dis8));
			arr.Add(cs);
			M = Ton8 * Dis8;
			MTot += M;
			cs.Format( "%.5f ", tokNM(M));
			arr.Add(cs);
		}
	}

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;
	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = Ton1+Ton2+dTotSlabWeight/*Ton3+Ton4+Ton5*/+Ton6+Ton7+Ton8+Ton9+Ton10;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];

	// ��Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);	
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);

	// Slab�� ������ ������ ���
	Dis1 =  W3/2;
	Dis2 =  W3+W5/2;	

	MTotE = (Ton1*Dis1) + (Ton2*Dis2);		
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton = (Ton1+Ton2);		
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M   = (MTotE);
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = Ton1 + Ton2;
}

//0�� Ÿ�԰� ���常 �ٸ�
void CCalcFloor::CalcGuardRightType99(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);
	
	double	Term	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	PaveT	= pBridge->m_dThickPave;

	if(m_nTypeHDan == 1 || m_nTypeHDan == 2)//��ȣ���̰ų� �ߺд��ϰ�츸...		
	{
		CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
		if(!pSep)	return;
	}

	double BaseLen = 1000;				//���� ����(�߷� ���� �� ��� 1m)

	CStringArray ArrSlab;
	double dTotSlabWeight = 0;
	double dTotSlabMoment = 0;
	CalcSlabRight(pBx, ArrSlab, LEN, dTotSlabWeight, dTotSlabMoment);
	
	double Ton4 =  (Term-LEN)*PaveT*pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt * BaseLen;
	double Ton5 = 0;
	double Ton6 = 0;
	double Ton7 = 0;
	double Ton8 = 0;
	double BaseX = Term;
	double Dis4  =	(BaseX-LEN)/2;

	double M = 0,	MTot = 0;
	m_dPreDeadLoad[1] = dTotSlabMoment;//Ton1*Dis1 + Ton2*Dis2 + Ton3*Dis3;
	m_dPostDeadLoad[1] = Ton4*Dis4;
	long n = 0;
	for(n=0; n<ArrSlab.GetSize(); n++)
		arr.Add(ArrSlab.GetAt(n));
	// 4
	cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-LEN), toM(PaveT), tokNPM3(pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt) );
	arr.Add(cs);
	cs.Format( "%.5f ", tokN(Ton4) );
	arr.Add(cs);
	cs.Format( "%.5f ", toM(Dis4) );
	arr.Add(cs);
	M = Ton4 * Dis4;
	MTot += M;
	cs.Format( "%.5f ", tokNM(M) );
	arr.Add(cs);

	long	nSize = pCalcData->m_ELoadTitleArr.GetSize();
	double	dBridgeWidth = pBridge->m_dBridgeWidth;
	double	dExtraLoad[50];
	double	dExtraLoadDis[50];
	for(n=0; n < 50; n++)	
		dExtraLoad[n]=0.0;

	// ���� �ΰ������� �ִٸ�....
	if(pCalcData->m_bExtraLoad) 
	{
		for(long n=0; n<nSize; n++)
		{
			if( (dBridgeWidth-Term) < pCalcData->m_ELoadDisArr[n])
			{
				dExtraLoad[n]		= pCalcData->m_ELoadWeightArr[n];
				dExtraLoadDis[n]	= pCalcData->m_ELoadDisArr[n] - (dBridgeWidth - Term);

				cs.Format("  (�ΰ����� : %s)", pCalcData->m_ELoadTitleArr[n]);
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dExtraLoad[n]));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dExtraLoadDis[n]));
				arr.Add(cs);
				M = dExtraLoad[n] * dExtraLoadDis[n];
				MTot += M;
				cs.Format( "%.5f ", tokNM(M));
				arr.Add(cs);
			}
		}
	}

	double dWeightTot = dTotSlabWeight/*Ton1+Ton2+Ton3*/+Ton4+Ton5+Ton6+Ton7+Ton8;
	MTot += dTotSlabMoment;
	for(n=0; n<50; n++) 
		dWeightTot += dExtraLoad[n];
	// 5.Total
	cs.Format( "%.5f ", tokN(dWeightTot));	// Weight Total
	arr.Add(cs);
	cs.Format( "%.5f ", tokNM(MTot) );											// T*M Total
	arr.Add(cs);
	// Save Data
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_Ton = (dWeightTot);
	pCalcData->CALC_CANTILEVER_DATA[1].m_Stop_M   = (MTot);
	
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_Ton = 0;
	pCalcData->CALC_CANTILEVER_DATA[1].m_StopE_M   = 0;
	pCalcData->DESIGN_FLOOR_DATA[FLOOR_RIGHT].m_dBangHoWeight = 0;
}

void CCalcFloor::CalcGuardRightType100(CPlateBasicIndex *pBx, CStringArray& arr, double LEN)
{
	CString cs(_T(""));
	cs.Format("�� ��ȣ���� �������� �ʴ� ����Դϴ�.");
	arr.Add(cs);
	arr.Add("");
	arr.Add("");
	arr.Add("");
	arr.Add("");
	arr.Add("");
}

long CCalcFloor::CalcSlabCenterDeadLoad(CStringArray& arr)
{
	CPlateBridgeApp *pDB	   = m_pDataManage->GetBridge();		
	CCalcData       *pCalcData = m_pDataManage->GetCalcData();
	CSteelUWeight   *pUWeight  = m_pDataManage->GetSteelUWeight();	
	CCalcGeneral	CalcGeneral(m_pDataManage);

	CString cs(_T(""));
	double	dHunch		= pDB->m_dThickSlabHunch;
	double	dPaveT		= pDB->m_dThickPave;
	double	dPaveUW		= pCalcData->DESIGN_UNIT_WEIGHT.m_dAsphalt;
	double	dConUW		= pUWeight->m_dUWeightChulgunCon;
	double	dConT		= pDB->m_dThickSlabBase - dHunch;
	double	dBaseLen	= 1000;	
	double	dWeight		= 0;
	double	dL			= CalcGeneral.CalcCentPart_L();
	if(!(pDB->IsTUGir()&&pDB->GetQtyGirder()>2))
		dL += dConT;

	cs.Format("ö����ũ��Ʈ :  %.3f x %.3f  =", toM(dConT), tokNPM3(dConUW));	
	arr.Add(cs);
	cs.Format( "%.5f ", toM(dConT) * tokNPM3(dConUW));
	arr.Add(cs);
	arr.Add(""); // Dist

	cs.Format("");
	arr.Add(cs);
	dWeight += dConT * dConUW * dBaseLen;
	m_dPreDeadLoad[2] =  (dConT*dConUW)*(dL*dL)*dBaseLen/ 10;//(wL^2)/10

	cs.Format("��        �� :  %.3f x %.3f  =", toM(dPaveT), tokNPM3(dPaveUW));
	arr.Add(cs);
	cs.Format( "%.5f ", toM(dPaveT) * tokNPM3(dPaveUW));
	arr.Add(cs);
	arr.Add("");

	cs.Format("");
	arr.Add(cs);
	dWeight += dPaveT * dPaveUW * dBaseLen;
	m_dPostDeadLoad[2] = dPaveT * dPaveUW * dL * dL * dBaseLen / 10;

	double dMd = dWeight * dL * dL / 10;
	arr.Add("��   ��");			///< ����
	cs.Format( "%.5f", tokNPM(dWeight));	///< �߷� �հ�
	arr.Add(cs);
	arr.Add("");			///< �Ÿ�
	arr.Add("");			///< ���Ʈ

	cs.Format(" Md = WL��/ 10 = %.3f �� %.3f��/ 10 = %.3f kN��m", tokNPM(dWeight), toM(dL), tokNM(dMd));
	arr.Add(cs);
	arr.Add("");		///< �߷�
	arr.Add("");		///< �Ÿ�
	cs.Format(" %.5f", tokNM(dMd));
	arr.Add(cs);		///< 

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Stop_Ton = dWeight;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Stop_M   = dMd;
	
	return 0;
}

long CCalcFloor::SetFloorStatus(long nHLL)
{
	CPlateBridgeApp* pBridge = m_pDataManage->GetBridge();	
	CCalcData*       pData   = m_pDataManage->GetCalcData();	
	CPlateBasicIndex *pBx = pBridge->GetGirder(-1)->GetCrossBxAtMinFlangeWidth();

	long nHdanSize= pBridge->GetQtyHDanNode();

//	pData->DESIGN_FLOOR_DATA[0].m_bBangHo = FALSE;//��ȣ��(����)
//	pData->DESIGN_FLOOR_DATA[1].m_bBangHo = FALSE;//��ȣ��(����)
//	pData->DESIGN_FLOOR_DATA[2].m_bBangHo = FALSE;//�ߺй�
//	pData->DESIGN_FLOOR_DATA[0].m_bWalkLoad = FALSE;
//	pData->DESIGN_FLOOR_DATA[1].m_bWalkLoad = FALSE;
//	pData->DESIGN_FLOOR_DATA[0].m_bPeopleLoad = FALSE;
//	pData->DESIGN_FLOOR_DATA[1].m_bPeopleLoad = FALSE;
//	
	double DisStt   = 0;
	double SumDis   = 0;
	long nBanghoSu  = 0;
	long nType      = 0; // 0 = ����, 1 = ��ȣ��, 3 = ����
	
	for(long n = 0; n < nHdanSize; n++)
	{
		nType   = pBridge->GetValueTypeHDan(n, 1/*Ÿ��*/);
		DisStt  = pBridge->GetLengthHDanDom(pBx, n);
		SumDis += DisStt;		
		////////////////////////////////////////////////////////////////////////////////////��ȣ��
		if(pBridge->IsGuard(n, 1) && n == 0) //��ȣ��(����)
		{
			CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);

			nBanghoSu++;
			pData->DESIGN_FLOOR_DATA[0].m_bBangHo				= TRUE;
			pData->DESIGN_FLOOR_DATA[0].m_dBangHoREdge[nHLL]	= toM(SumDis);
			pData->DESIGN_FLOOR_DATA[0].m_dBangHoLEdge[nHLL]	= pSep ? toM(pSep->m_D1) : 0;
			pData->DESIGN_FLOOR_DATA[0].m_dBangHoWidth			= pSep ? toM(pSep->GetWidth()) : 0;
		}
		else if(pBridge->IsGuard(n, 1) && n == nHdanSize-1)//��ȣ��(����)
		{
			CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::RIGHT);
			double  dLen	= pBridge->GetLengthHDanDom(pBx, n); 
			nBanghoSu++;
			pData->DESIGN_FLOOR_DATA[1].m_bBangHo				= TRUE;
			pData->DESIGN_FLOOR_DATA[1].m_dBangHoREdge[nHLL]	= pSep ? toM(SumDis-(dLen - pSep->GetWidth())) : 0;
			pData->DESIGN_FLOOR_DATA[1].m_dBangHoLEdge[nHLL]	= toM(SumDis - DisStt);
			pData->DESIGN_FLOOR_DATA[1].m_dBangHoWidth			= pSep ? toM(pSep->GetWidth()) : 0;
		}
		else if(pBridge->IsGuard(n, 1)
			&& pBridge->GetValueTypeHDan(n-1, 1) != 1 
			&& pBridge->GetValueTypeHDan(n+1, 1) != 1)//�߾Ӻи���
		{
			CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::CENTER);
			nBanghoSu++;
			pData->DESIGN_FLOOR_DATA[2].m_bBangHo				= TRUE;
			pData->DESIGN_FLOOR_DATA[2].m_dBangHoREdge[nHLL]	= toM(SumDis);
			pData->DESIGN_FLOOR_DATA[2].m_dBangHoLEdge[nHLL]	= toM(SumDis - DisStt);
			pData->DESIGN_FLOOR_DATA[2].m_dBangHoWidth			= pSep ? toM(pSep->GetWidth()) : 0;
		}
		////////////////////////////////////////////////////////////////////////////////////����
		if(nType==3 && n == 1)//����(����)
		{
			CCentSeparation* pSep = pBridge->GetCurGuardWall(CCentSeparation::LEFT);
			pData->DESIGN_FLOOR_DATA[0].m_bWalkLoad				= TRUE;
			pData->DESIGN_FLOOR_DATA[0].m_bPeopleLoad			= TRUE;			
			pData->DESIGN_FLOOR_DATA[0].m_dWalkLoadREdge[nHLL]	= toM(SumDis);
			pData->DESIGN_FLOOR_DATA[0].m_dWalkLoadLEdge[nHLL]	= toM(SumDis - DisStt);
			pData->DESIGN_FLOOR_DATA[0].m_dWalkLoadWidth		= toM(DisStt);
			pData->DESIGN_FLOOR_DATA[0].m_dBangHoWidth			= pSep ? toM(pSep->GetWidth()) : 0;
		}
		if(nType==3 && n == nHdanSize-2)//����(����)
		{
			if(nHdanSize-2 == 1)	
			{
				pData->DESIGN_FLOOR_DATA[1].m_bWalkLoad = FALSE;
				pData->DESIGN_FLOOR_DATA[1].m_bPeopleLoad = FALSE;
			}
			else
			{
				pData->DESIGN_FLOOR_DATA[1].m_bWalkLoad = TRUE;
				pData->DESIGN_FLOOR_DATA[1].m_bPeopleLoad = TRUE;
			}
			pData->DESIGN_FLOOR_DATA[1].m_dWalkLoadREdge[nHLL] = toM(SumDis);
			pData->DESIGN_FLOOR_DATA[1].m_dWalkLoadLEdge[nHLL] = toM(SumDis - DisStt);
			pData->DESIGN_FLOOR_DATA[1].m_dWalkLoadWidth = toM(DisStt);
		}
	}
	pData->m_bPassedCalcFloorDlg = TRUE;
	
	return nBanghoSu;
}

void CCalcFloor::CalcSlabLeft(CPlateBasicIndex *pBx, CStringArray& arr, double LEN, double &dTotWeight, double &dTotMoment)
{
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();		
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight	= m_pDataManage->GetSteelUWeight();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(0);
	CCentSeparation		*pSep		= pBridge->GetCurGuardWall(CCentSeparation::LEFT);

	double dWidthBindConc	= pBridge->m_BindConc.m_dWidth;
	BOOL bUserSlab	= pBridge->IsUserSlabHunch(TRUE, pBx);
	double dQuater	= pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double dTerm	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_LEFT].m_Dead_L;
	double dTa		= pBridge->m_dThickSlabLeft;
	double dTc		= pBridge->m_dThickSlabBase;
	double dDis		= pBridge->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;
	double dBaseX	= dTerm;
	double dBaseH	= pBridge->m_dThickSlabLeftExt;
	double dBaseLen = 1000;								//���� ����(�߷� ���� �� ��� 1m)
	double dHunch	= 0;
	if(bUserSlab && pBridge->m_dHunchArray.GetSize() > 0)
		dHunch	= pBridge->m_dHunchArray.GetAt(0);
	double dUWeight	= pUWeight->m_dUWeightChulgunCon;
	double	dMoment		= 0;
	double  dMomentH	= pBridge->IsTUGir() ? dWidthBindConc/4 : 0;
	double	dDeep		= pBridge->m_BindConc.m_dDeep;
	double	dMomentV	= dTc - dDeep;

	///< ���߰��
	double dTon1	= 0;
	double dTon2	= 0;
	double dTon3	= 0;
	double dTon4	= dHunch * ABS(dTc-dBaseH)/2 * dUWeight * dBaseLen;
	double dTon5	= (dTc-dTa) * (dQuater+dDis) * dUWeight * dBaseLen;

	///< �Ÿ����
	double dDis1	= 0;
	double dDis2	= 0;
	double dDis3	= 0;
	double dDis4	= dQuater + dDis + dHunch*2/3;
	double dDis5	= (dQuater + dDis)/2;

	if(!bUserSlab)
	{
		if(!pBridge->IsTUGir())
		{
			dTon1 = dTa * dTerm * dUWeight * dBaseLen;
			dTon2 = (dTc-dTa) * (dTerm - dQuater - dDis)/2 * dUWeight * dBaseLen;
			dTon3 = (dTc-dTa) * (dQuater + dDis)*dUWeight * dBaseLen;
			dDis1 =	dBaseX/2;
			dDis2 =	dQuater+dDis+(dTerm-dDis-dQuater)*1/3;
			dDis3 =	(dQuater+dDis)/2;
		}
		else
		{
			if(dTa==dTc-dDeep)
			{
				dTon1 = dTc * dTerm * dUWeight * dBaseLen;
				dTon2 = dTon3 = dTon4 = dTon5 = 0;
				dDis1 =	dBaseX/2;
				dDis2 =	0;
			}
			else
			{
				dTon1 = (dTa) * (dTerm-dMomentH) * dUWeight * dBaseLen;
				dTon2 = (dTc-dTa) * (dTerm-dMomentH)/2 * dUWeight * dBaseLen;
				dTon3 = dMomentH*dMomentV*dUWeight*dBaseLen;
				dTon4 = dTon5 = 0;
				dDis1 =	(dBaseX-dMomentH)/2+dMomentH;
				dDis2 =	(dBaseX-dMomentH)/3+dMomentH;
				dDis3 = dMomentH/2;
			}
		}
	}

	CString cs;
	if(bUserSlab)
	{
		if(dTon1!=0)
		{
			///< 1
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dTerm), toM(dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon1));
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis1));
			arr.Add(cs);
			dMoment = dTon1 * dDis1;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment) );
			arr.Add(cs);
		}

		if(dTon2 != 0)
		{
			///< 2
			cs.Format("  %.3f �� %.3f �� %.3f �� 1/2", toM(dTerm - dQuater - dDis - dHunch), toM(ABS(dTa-dBaseH)), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon2));
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis2));
			arr.Add(cs);
			dMoment = dTon2 * dDis2;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment) );
			arr.Add(cs);
		}

		if(dTon3 != 0)
		{
			///< 3
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dHunch), toM(ABS(dTa-dBaseH)), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon3));
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis3));
			arr.Add(cs);
			dMoment = dTon3 * dDis3;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment));
			arr.Add(cs);
		}

		if(dTon4 != 0)
		{
			///< 4
			cs.Format("  %.3f �� %.3f �� %.3f �� 1/2", toM(dHunch), toM(ABS(dTc-dBaseH)), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon4));
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis4));
			arr.Add(cs);
			dMoment = dTon4 * dDis4;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment));
			arr.Add(cs);
		}

		if(dTon5 != 0)
		{
			///< 5
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dQuater+dDis), toM(dTc-dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon5));
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis5));
			arr.Add(cs);
			dMoment = dTon5 * dDis5;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment));
			arr.Add(cs);
		}

		dTotMoment = dTotMoment;
		dTotWeight = dTon1+dTon2+dTon3+dTon4+dTon5;
	}
	else
	{
		if(!pBridge->IsTUGir())
		{
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dBaseX), toM(dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon1) );
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis1) );
			arr.Add(cs);
			dMoment = dTon1 * dDis1;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment) );
			arr.Add(cs);
			// ��	
			cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(dBaseX - dDis - dQuater), toM(dTc-dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon2));
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis2));
			arr.Add(cs);
			dMoment = dTon2 * dDis2;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment));
			arr.Add(cs);
			// �� slab
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dQuater+dDis), toM(dTc-dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.6f ", tokN(dTon3) );
			arr.Add(cs);
			cs.Format( "%.6f ", toM(dDis3) );
			arr.Add(cs);
			dMoment = dTon3 * dDis3;
			dTotMoment += dMoment;
			cs.Format( "%.6f ", tokNM(dMoment) );
			arr.Add(cs);

			dTotMoment = dTotMoment;
			dTotWeight = dTon1+dTon2+dTon3;
		}
		else
		{
			if(dTa==dTc-dDeep)
			{
				cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dBaseX), toM(dTc), tokNPM3(dUWeight));
				arr.Add(cs);
				cs.Format( "%.6f ", tokN(dTon1) );
				arr.Add(cs);
				cs.Format( "%.6f ", toM(dDis1) );
				arr.Add(cs);
				dMoment = dTon1 * dDis1;
				dTotMoment += dMoment;
				cs.Format( "%.6f ", tokNM(dMoment) );
				arr.Add(cs);
				
				dTotMoment = dTotMoment;
				dTotWeight = dTon1+dTon2;
			}
			else
			{
				cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dBaseX-dMomentH), toM(dTa), tokNPM3(dUWeight));
				arr.Add(cs);
				cs.Format( "%.6f ", tokN(dTon1) );
				arr.Add(cs);
				cs.Format( "%.6f ", toM(dDis1) );
				arr.Add(cs);
				dMoment = dTon1 * dDis1;
				dTotMoment += dMoment;
				cs.Format( "%.6f ", tokNM(dMoment) );
				arr.Add(cs);
				// ��
				if(dTc!=dTa)
				{
					cs.Format("  %.3f �� %.3f �� %.3f �� 1/2  ", toM(dBaseX-dMomentH), toM(dTc-dTa), tokNPM3(dUWeight));
					arr.Add(cs);
					cs.Format( "%.6f ", tokN(dTon2));
					arr.Add(cs);
					cs.Format( "%.6f ", toM(dDis2));
					arr.Add(cs);
					dMoment = dTon2 * dDis2;
					dTotMoment += dMoment;
					cs.Format( "%.6f ", tokNM(dMoment));
					arr.Add(cs);
				}				

				cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dMomentH), toM(dMomentV), tokNPM3(dUWeight));
				arr.Add(cs);
				cs.Format( "%.6f ", tokN(dTon3));
				arr.Add(cs);
				cs.Format( "%.6f ", toM(dDis3));
				arr.Add(cs);
				dMoment = dTon3 * dDis3;
				dTotMoment += dMoment;
				cs.Format( "%.6f ", tokNM(dMoment));
				arr.Add(cs);
				
				dTotMoment = dTotMoment;
				dTotWeight = dTon1+dTon2+dTon3;
			}
		}
	}
}

void CCalcFloor::CalcSlabRight(CPlateBasicIndex *pBx, CStringArray& arr, double LEN, double &dTotWeight, double &dTotMoment)
{
	CPlateBridgeApp		*pBridge	= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CSteelUWeight *pUWeight	= m_pDataManage->GetSteelUWeight();
	CPlateGirderApp		*pGir		= pBridge->GetGirder(pBridge->GetGirdersu()-1);

	double	dWidthBindConc	= pBridge->m_BindConc.m_dWidth;
	BOOL	bUserSlab		= pBridge->IsUserSlabHunch(FALSE, pBx);
	double	dTa			= pBridge->m_dThickSlabRight;
	double	dTc			= pBridge->m_dThickSlabBase;
	double	dDis		= pBridge->m_dWidthHunch+(pGir->GetMaxWidthFlange(TRUE)-pGir->GetMinWidthFlange(TRUE))/2;
	double	dQuater		= pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double	Term		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_RIGHT].m_Dead_L;
	double	dUWeight	= pUWeight->m_dUWeightChulgunCon;
	double	dHunch		= 0;
	if(bUserSlab && pBridge->m_dHunchArray.GetSize() > 0)
		dHunch = pBridge->m_dHunchArray.GetAt(3);
	double	dBaseH		= pBridge->m_dThickSlabRightExt;
	double	dBaseLen	= 1000;
	double	dBaseX		= Term;

	///< ���߰��
	double dTon1	= 0;//* Term * dUWeight * dBaseLen;
	double dTon2	= 0;//(Term - dQuater - dDis - dHunch) * (ABS(dTa-dBaseH))/2 * dUWeight * dBaseLen;
	double dTon3	= 0;//dHunch * ABS(dTa-dBaseH) * dUWeight * dBaseLen;
	double dTon4	= dHunch * ABS(dTc-dBaseH)/2 * dUWeight * dBaseLen;
	double dTon5	= (dTc-dTa) * (dQuater+dDis) * dUWeight * dBaseLen;

	///< �Ÿ����
	double dDis1	= 0;//dBaseX/2;
	double dDis2	= 0;//dHunch + dQuater + dDis + (Term - dQuater - dDis - dHunch)/3;
	double dDis3	= 0;//dQuater + dDis + dHunch/2;
	double dDis4	= dQuater + dDis + dHunch*2/3;
	double dDis5	= (dQuater + dDis)/2;
	double	dDeep = pBridge->m_BindConc.m_dDeep;
	double dMomentV	= dTc  - dDeep;
	double dMomentH	= pBridge->IsTUGir() ? dWidthBindConc/4 : 0;

	if(!bUserSlab)
	{
		if(!pBridge->IsTUGir())
		{
			dTon1 = dTa * Term * dUWeight * dBaseLen;
			dTon2 = (dTc-dTa) * (Term-dQuater-dDis)/2*dUWeight * dBaseLen;
			dTon3 = (dTc-dTa) * (dQuater+dDis) * dUWeight * dBaseLen;
			dDis1 =	dBaseX/2;
			dDis2 =	dQuater+dDis+(dBaseX-dDis-dQuater)*1/3;
			dDis3 =	(dQuater+dDis)/2;
		}
		else
		{
			if(dTa==dTc-dDeep)
			{
				dTon1 = dTc * (Term-dMomentH) * dUWeight * dBaseLen;
				dTon2 = dTon3 = dTon4 = dTon5 = 0;
				dDis1 =	dBaseX/2;
				dDis2 =	0;
			}
			else
			{
				dTon1 = (dTa) * (Term-dMomentH) * dUWeight * dBaseLen;
				dTon2 = (dTc -dTa) * (Term-dMomentH) * dUWeight/2 * dBaseLen;
				dTon3 = dMomentH*dMomentV*dUWeight*dBaseLen;
				dTon4 = dTon5 = 0;
				dDis1 =(dBaseX-dMomentH)/2+dMomentH;
				dDis2 =	(dBaseX-dMomentH)/2+dMomentH;
				dDis3 = dMomentH/2;
			}
		}
	}
	double dMoment = 0;
	CString cs;
	if(bUserSlab)
	{
		if(dTon1!=0)
		{
			///< 1
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term), toM(dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon1));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis1));
			arr.Add(cs);
			dMoment = dTon1 * dDis1;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment) );
			arr.Add(cs);
		}

		if(dTon2 != 0)
		{
			///< 2
			cs.Format("  %.3f �� %.3f �� %.3f �� 1/2", toM(Term - dQuater - dDis - dHunch), toM(ABS(dTa-dBaseH)), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon2));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis2));
			arr.Add(cs);
			dMoment = dTon2 * dDis2;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment) );
			arr.Add(cs);
		}

		if(dTon3 != 0)
		{
			///< 3
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dHunch), toM(ABS(dTa-dBaseH)), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon3));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis3));
			arr.Add(cs);
			dMoment = dTon3 * dDis3;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment));
			arr.Add(cs);
		}

		if(dTon4 != 0)
		{
			///< 4
			cs.Format("  %.3f �� %.3f �� %.3f �� 1/2", toM(dHunch), toM(ABS(dTc-dBaseH)), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon4));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis4));
			arr.Add(cs);
			dMoment = dTon4 * dDis4;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment));
			arr.Add(cs);
		}

		if(dTon5 != 5)
		{
			///< 5
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dQuater+dDis), toM(dTc-dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon5));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis5));
			arr.Add(cs);
			dMoment = dTon5 * dDis5;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment));
			arr.Add(cs);
		}

		dTotMoment = dTotMoment;
		dTotWeight = dTon1+dTon2+dTon3+dTon4+dTon5;
	}
	else
	{
		if(!pBridge->IsTUGir())
		{
			// slab
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term), toM(dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon1));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis1));
			arr.Add(cs);
			dMoment = dTon1 * dDis1;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment));
			arr.Add(cs);
			// ��
			cs.Format("  %.3f �� %.3f �� %.3f �� 1/2 ", toM(Term-dDis-dQuater),  toM(dTc-dTa), tokNPM3(dUWeight));
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon2));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis2));
			arr.Add(cs);
			dMoment = dTon2 * dDis2;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment));
			arr.Add(cs);
			// ��
			cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dDis+dQuater), toM(dTc-dTa), tokNPM3(dUWeight) );
			arr.Add(cs);
			cs.Format( "%.5f ", tokN(dTon3));
			arr.Add(cs);
			cs.Format( "%.5f ", toM(dDis3));
			arr.Add(cs);
			dMoment = dTon3 * dDis3;
			dTotMoment += dMoment;
			cs.Format( "%.5f ", tokNM(dMoment));
			arr.Add(cs);
			
			dTotMoment = dTotMoment;
			dTotWeight = dTon1+dTon2+dTon3;
		}
		else
		{

			if(dTa==dTc-dDeep)
			{
				cs.Format("  %.3f �� %.3f �� %.3f  ", toM(dBaseX-dMomentH), toM(dTc), tokNPM3(dUWeight));
				arr.Add(cs);
				cs.Format( "%.6f ", tokN(dTon1) );
				arr.Add(cs);
				cs.Format( "%.6f ", toM(dDis1) );
				arr.Add(cs);
				dMoment = dTon1 * dDis1;
				dTotMoment += dMoment;
				cs.Format( "%.6f ", tokNM(dMoment) );
				arr.Add(cs);
				
				dTotMoment = dTotMoment;
				dTotWeight = dTon1+dTon2;
			}
			else
			{
				cs.Format("  %.3f �� %.3f �� %.3f  ", toM(Term-dMomentH), toM(dTa), tokNPM3(dUWeight));
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dTon1));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dDis1));
				arr.Add(cs);
				dMoment = dTon1 * dDis1;
				dTotMoment += dMoment;
				cs.Format( "%.5f ", tokNM(dMoment));
				arr.Add(cs);
				// ��
				if(dTa!=dTc)
				{
					cs.Format("  %.3f �� %.3f �� %.3f �� 1/2 ", toM(Term-dMomentH),  toM(dTc-dTa), tokNPM3(dUWeight));
					arr.Add(cs);
					cs.Format( "%.5f ", tokN(dTon2));
					arr.Add(cs);
					cs.Format( "%.5f ", toM(dDis2));
					arr.Add(cs);
					dMoment = dTon2 * dDis2;
					dTotMoment += dMoment;
					cs.Format( "%.5f ", tokNM(dMoment));
					arr.Add(cs);
				}				

				cs.Format("  %.3f �� %.3f �� %.3f ", toM(dMomentV),  toM(dMomentH), tokNPM3(dUWeight));
				arr.Add(cs);
				cs.Format( "%.5f ", tokN(dTon3));
				arr.Add(cs);
				cs.Format( "%.5f ", toM(dDis3));
				arr.Add(cs);
				dMoment = dTon3 * dDis3;
				dTotMoment += dMoment;
				cs.Format( "%.5f ", tokNM(dMoment));
				arr.Add(cs);
							
				dTotMoment = dTotMoment;
				dTotWeight = dTon1+dTon2+dTon3;
			}
		}
	}
}

double CCalcFloor::GetLengthArcBy3Point(CDPoint xyCen, CDPoint xyStt, CDPoint xyEnd)
{
	double dAngle1 = (xyStt - xyCen).Unit().GetAngleDegree();
	double dBetAngle = fabs((xyEnd - xyCen).Unit().GetAngleDegree() - dAngle1);
	
	return ~(xyCen-xyStt)*2*3.1415926535*(dBetAngle/360);
}

void CCalcFloor::SetSlabMinThick(long nSide)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CCalcGeneral		CalcGeneral(m_pDataManage);
	CCalcFloor			CalcFloor(m_pDataManage);
	CPlateGirderApp		*pGir		= NULL;
	CPlateBasicIndex    *pBx		= NULL;
	CCentSeparation		*pSep		= NULL;

	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad;
	BOOL bUsePreStress	= pDB->m_nSlabTensionSize==0 ? FALSE : TRUE;//������Ʈ���� �������...����...
	double  dLastCheck	= 220;//������ �ٴ����� �ּҵβ��� 220
	if(bUsePreStress)	dLastCheck = 200;//������Ʈ���� �����̸�...(�Ҽ��ְŴ���)

	BOOL	bWall	= 0;	///< ��ȣ�� ����
	long	nQtyGir	= pDB->GetGirdersu();
	double	dLEN	= 0;

	if(nSide==FLOOR_LEFT)
	{
		pGir	= pDB->GetGirder(0);
		pBx		= pGir->GetCrossBxAtMinFlangeWidth();
		pSep	= pDB->GetCurGuardWall(CCentSeparation::LEFT);
		dLEN	= pDB->GetLengthHDanDom(pBx, 0);
		bWall	= pDB->IsGuard(0, 1);
	}
	else
	{
		pGir	= pDB->GetGirder(nQtyGir-1);
		pBx		= pGir->GetCrossBxAtMinFlangeWidth();
		pSep	= pDB->GetCurGuardWall(CCentSeparation::RIGHT);
		dLEN	= pDB->GetLengthHDanDom(pBx, pDB->GetQtyHDanNode()-1);
		bWall	= pDB->IsGuard(pDB->GetQtyHDanNode()-1, 1);
	}

	CMap <CString, LPCTSTR, double, double> Map;
	CalcFloor.CalcPreStress(Map, nSide);
	double	dTireW	= 300;
	double	dL		= 0, dt = 0, dmT = 0.25;
	double	dWidth	= 0;
	double	dTerm	= nSide==FLOOR_LEFT ? pDB->m_dWidthSlabLeft : pDB->m_dWidthSlabRight;
	double	dTWeb	= pGir->GetThickFactByStation(pBx->GetStation(), G_W);
	double	dQuter	= pGir->GetLengthSharpOnBx(pBx, TRUE)/2;
	double	dTslab	= pDB->m_dThickSlabBase;
	double	dWidthBindConc	= pDB->m_BindConc.m_dWidth;

//	for(int n = 0; n < int(pDB->GetQtyHDanNode()/2); n++)
//	{
//		if(pDB->IsGuard(n, 1))
//		{
//			dLEN = pDB->GetLengthHDanDom(pBx, n);
//			if(!pSep)	dLEN	= 0;
//			nWall = 1;
//			break;
//		}
//	}
	
	if(pDB->GetGuardWallSu() <= 0)	return;
	if(pSep)	
	{
		if(bWall)	dWidth = pSep->GetWidth();
	}

	if(nSide==FLOOR_LEFT || nSide==FLOOR_RIGHT)
	{
		if(!bWall)	//��ȣ�� ������
		{
			if(!pDB->IsTUGir())	dL = dTerm - (dTireW+dQuter+dTWeb/2);
			else				dL = dTerm - (dTireW+dWidthBindConc/2);
		}
		else			//��ȣ�� ������
		{
			if(!pDB->IsTUGir())	dL = dTerm - (dLEN+dTireW+dQuter+dTWeb/2);
			else				dL = (dTerm-dWidthBindConc/4)-(dLEN+dTireW);			
		}

		if(bWalkLoad)	dL = 0;

		// �ٴ����� �ּ� �β�		
		double dL_M = toM(dL);

		if (bWalkLoad)//������ �ٴ����� �ּ� �β��� 140 mm�� ǥ������ �Ѵ�.
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Req_T  = 140;
		else if (dL > 0)
		{
			if(dL_M > dmT )	dt = 80*dL_M + 230;			
			else			dt = 280*dL_M + 180;				

			if(bUsePreStress) dt *= 0.9;
			
			dt = MAX(dt, dLastCheck);
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Req_T  = dt;
		}
		else//������ �ٴ����� �ּ� �β��� 
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Req_T  = dLastCheck;				//Check List ���� ���
		
		if(!pDB->IsTUGir())
		{

			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Dead_L = dTerm-dQuter-dTWeb/2;		//�����߿� ���� ����
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_L = IsLaneLoad(nSide) ? dL : 0;						//Ȱ���߿� ���� ����
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Min_T  = dTslab;			
		}
		else
		{
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Dead_L = dTerm-dWidthBindConc/4;	//�����߿� ���� ����
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_L = IsLaneLoad(nSide) ? dL : 0;						//Ȱ���߿� ���� ����
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Min_T  = dTslab;			
		}
	}
	else
	{
		double	dL	= CalcGeneral.CalcCentPart_L();
		double	dt	= 0;
		CString sign(_T(""));
		if(!(pDB->IsTUGir()&&pDB->GetQtyGirder()>2))
			 dL += (pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch);

		if (bWalkLoad)//������ �ٴ����� �ּ� �β��� 140 mm�� ǥ������ �Ѵ�.
			pCalcData->CALC_CANTILEVER_DATA[nSide].m_Req_T  = 140;
		else if (dL > 0)
		{
			if(nQtyGir > 2)	dt = (30 * toM(dL) + 130);
			else			dt = (40 * toM(dL) + 130);

			if(bUsePreStress)	dt = dt*0.9;
			dt = MAX(dt, dLastCheck);		
		}
		else//������ �ٴ����� �ּ� �β��� 
			dt  = dLastCheck;					//Check List ���� ���
		
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Req_T  = dt;
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Dead_L = dL;
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_L = IsLaneLoad(nSide) ? dL : 0;
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Min_T  = dTslab - pDB->m_dThickSlabHunch;
	}
}

void CCalcFloor::SetLiveLoad(long nSide)
{
	BOOL bLeft = nSide==0 ? TRUE : FALSE;

	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcGeneral CalcGeneral(m_pDataManage);
	CPlateGirderApp		*pGir			= pDB->GetGirder(bLeft ? 0 : pDB->GetGirdersu()-1);
	CPlateBasicIndex    *pBx			= pGir->GetCrossBxAtMinFlangeWidth();
	CCalcData       *pCalcData		= m_pDataManage->GetCalcData();

	if(pDB->GetGuardWallSu() <= 0)	return;
	
	CCentSeparation		*pSep = pDB->GetCurGuardWall(bLeft ? CCentSeparation::LEFT : CCentSeparation::RIGHT);

	// Ȱ���� Data	
	double  dLen	= pDB->GetLengthHDanDom(pBx, bLeft ? 0 : pDB->GetQtyHDanNode()-1); 
	if(!pSep)	dLen	= 0;
	double	dSharp	= pGir->GetLengthSharpOnBx(pBx, TRUE);
	double	dTerm	= bLeft ? pDB->m_dWidthSlabLeft : pDB->m_dWidthSlabRight;
	double	dPr		= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dL		= 0;
	double	dMl		= 0;
	double	dMl_	= 0;
	double	dE		= 0;
	double	dP		= 0;
	double	di		= 0;
	double	dii		= 0;
	double	diT		= 0.15;
	long	nFloorIndex = nSide;
	BOOL    bWalkLoad = pCalcData->DESIGN_FLOOR_DATA[nFloorIndex].m_bWalkLoad;
	double	dWebThick	= pGir->GetThickJewon(G_W, pBx);
	double	dDis_Tank		= CalcGeneral.GetDBLOAD_Dis_Tank();
	double	dDis_Trailer	= CalcGeneral.GetDBLOAD_Dis_Trailer();

	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml			= 0;
	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_E			= 0;
	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml			= 0;
	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Tank		= 0;
	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_E3			= 0;
	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Trailer	= 0;

	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Tank_L		= 0;//070409���� �߰�....KB...
	pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Trailer_L		= 0;

	if(bWalkLoad)
	{
		dL  = toM(CalcGeneral.GetWidthBodoDeadLoad(bLeft, pBx));
		dMl = 5 * (dL*dL)/2;

		pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_i  = di;
		pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Pr = frkN(dPr);
		pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml = frkNM(dMl);
	}
	else
	{

		dL = toM(pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_L);
		double dImpact = 15/(40 + dL);
		if(dImpact>0.3)	dImpact = 0.3;

		if(dL > 0)
		{
			CalcGeneral.CalcMovingLoad_I(dL, di, dii);
			dE  = CalcGeneral.CalcMovingLoad_E(dL);	// mm
			dP	= CalcGeneral.CalcMovingLoad_Pr(dPr, di, dE);
			dMl = CalcGeneral.CalcMovingLoad_ML(dP, dL);
			if(dL>1.8)
			{
				CalcGeneral.CalcMovingLoad_I(dL-1.8, di, dii);
				dE  = CalcGeneral.CalcMovingLoad_E(dL-1.8);	// mm
				dP	= CalcGeneral.CalcMovingLoad_Pr(dPr, di, dE);
				dMl_ = CalcGeneral.CalcMovingLoad_ML(dP, dL-1.8);
				dMl += dMl_;
			}

			pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_E  = frM(dE);
			pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml = frkNM(dMl);
			pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_ = frkNM(dMl_);

			if(pCalcData->m_bTank)			// ��ũ����
			{
				double dTankWeight	= pCalcData->m_TankWeight;
				double dL2			= Round(toM(dTerm-(dSharp/2+dWebThick/2)-(frM(dDis_Tank) + dLen)),3);
				double dP2			= dTankWeight / 2 / pCalcData->m_TankEdit1;
				double dMl2			= CalcGeneral.CalcMovingLoad_ML(dP2, dL2) * (1 + diT);

				pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Tank = frkNM(dMl2);
				pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Tank_L = dL2;
			}

			if(pCalcData->m_bTrailer)
			{
				double dTrailer	= max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
				double dTrailer_L = Round(toM(dLen) + dDis_Trailer,3);//�� �ڸ��� ���߱� ����..��...070409...KB...
				double dL3		= Round(toM(dTerm-(dSharp/2+dWebThick/2))- dTrailer_L,3);
				double dE3		= Round(CalcGeneral.CalcMovingLoad_E(dL3),3);
				double dP3		= CalcGeneral.CalcMovingLoad_Pr(dTrailer, diT, dE3);
				double dMl3		= CalcGeneral.CalcMovingLoad_ML(dP3, dL3);

				if(dL3 > 0)
				{
					pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_E3			= (dE3);//frM
					pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Trailer	= frkNM(dMl3);
					pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Trailer_L	= dL3;
				}
				if(dL3<=0)
				{
					pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_E3			= 0;
					pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Trailer	= 0;					
				}
			}
		}
		else 
		{
			dMl = 0;

			pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_E			= frM(1.14);
			pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml			= 0;
			pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Tank		= 0;
			pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Ml_Trailer	= 0;
		}
		pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_i  = dImpact;
		pCalcData->CALC_CANTILEVER_DATA[nFloorIndex].m_Move_Pr = frkN(dPr);
	}
}

void CCalcFloor::SetLiveLoadCenter()
{
	CPlateBridgeApp *pDB		= m_pDataManage->GetBridge();
	CCalcData       *pCalcData		= m_pDataManage->GetCalcData();
	CCalcGeneral CalcGeneral(m_pDataManage);

	double	dL	= toM(CalcGeneral.CalcCentPart_L());
	double	di	= 0, dii = 0, diT = 0.15;
	double	dML	= 0, dML2 = 0, dML3 = 0;
	double	dt	= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dM	= CalcGeneral.CalcMoveCoeff();	//�������϶��� 80%�� ����
	
	
	if(!(pDB->IsTUGir()&&pDB->GetQtyGirder()>2))
		dL += toM(pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch);

	CalcGeneral.CalcMovingLoad_I(dL, di, dii);

	dML = CalcGeneral.CalcCentPart_Ml(dL, di, dt, dM);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml	= frkNM(dML);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_M		= dM;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Dead_L		= frM(dL);

	if(pCalcData->m_bTank)
	{
		double dTankWeight = pCalcData->m_TankWeight;
		dt  = dTankWeight / 2 / pCalcData->m_TankEdit1;
		dML2 = CalcGeneral.CalcCentPart_Ml(dL, diT, dt, dM);								

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Tank = frkNM(dML2);
	}
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Tank = 0;

	if(pCalcData->m_bTrailer)
	{
		dt = max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));
		dML3 = CalcGeneral.CalcCentPart_Ml(dL, diT, dt, dM);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Trailer = frkNM(dML3);
	}
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Trailer = 0;
}

void CCalcFloor::SetWindLoad(long nSide)
{
	CPlateBridgeApp	*pDB			= m_pDataManage->GetBridge();
	CDataManage		*pDataManage	= m_pDataManage;
	CCalcData		*pCalcData		= pDataManage->GetCalcData();
	CCentSeparation	*pSep			= NULL;

	long	nSize			= pDB->GetQtyHDanNode();
	double	dPaveT			= pDB->m_dThickPave;
	double	dWalkLoadH		= pCalcData->DESIGN_FLOOR_DATA[nSide].m_dWalkLoadH;
	double	dBangEmWeight	= pCalcData->DESIGN_FLOOR_DATA[nSide].m_dBangEmWeight;
	double	dBangEmHeight	= pCalcData->DESIGN_FLOOR_DATA[nSide].m_dBangEmHeight;
	double	dH				= 0;
	double	dHH				= 0;
	double	dPw				= 0;
	double	dWindLoadForce	= pCalcData->m_dWindLoadForce;
	double	dMw				= 0;
	BOOL	bWalkLoad		= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad;
	BOOL	bBangEm			= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bBangEm;
	
	if(nSide==FLOOR_LEFT)	pSep = pDB->GetCurGuardWall(CCentSeparation::LEFT);
	else					pSep = pDB->GetCurGuardWall(CCentSeparation::RIGHT);

//	if(pDB->IsGuard(nSide==0?0:nSize-1, 1) || bBangEm || bNangan)//  1 -> ��ȣ��
	if(pDB->IsGuard(nSide==0?0:nSize-1, 1))
	{
		if(pDB->GetGuardWallSu() <= 0)	return;

		double dH1 = pSep ? pSep->m_H1 : 0;
		double dH2 = pSep ? pSep->m_H2 : 0;
		double dH3 = pSep ? pSep->m_H3 : 0;
		
		if(bWalkLoad)	dHH = toM(dH1 + dH2 + dH3 - dWalkLoadH);
		else			dHH = toM(dH1 + dH2 + dH3 - dPaveT);

		if(bBangEm && dBangEmWeight != 0)
		{
			if(dBangEmHeight > dHH)	dHH = toM(dBangEmHeight);
		}	
	}
	else	dHH = 0.0, dH = 0.0;	// ��ȣ���� �ƴϸ�

	dPw =  dWindLoadForce * dHH;

	if(bWalkLoad)	dMw = dPw * (dHH / 2.0 + toM(dWalkLoadH));
	else			dMw = dPw * (dHH / 2.0 + toM(dPaveT));

	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_Pw	= frkNPM(dPw);
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_Mw	= frkNM(dMw);
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_H		= frM(dHH);
}

// ���� �浹����
void CCalcFloor::SetCollisionLoad(long nSide)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CDataManage			*pDataManage	= m_pDataManage;
	CCalcData			*pCalcData		= pDataManage->GetCalcData();
	if(pDB->GetGuardWallSu() <= 0)	return;
	CCentSeparation		*pSep			= NULL;
	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	if(nSide==1)		pGir = pDB->GetGirder(pDB->GetGirdersu()-1);

	double	dMoveL		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_L;
	double	dR			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_R;
	double	dH			= 0;
	double	dH2			= 1.0 + toM(pDB->m_dThickPave);
	double	dHoriForce	= 0;
	double  dVeriForce	= 0;
	double	dMoment		= 0;
	double	dMoment1	= 0;
	double	dMoment2	= 0;
	double	dLEN		= 0;
	double	dSharp		= 0;
	double	dTerm		= 0;
	double	dL			= 0;
	double	dVelocity	= 0;
	BOOL	bNanGan		= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bNanGan;
	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad;

	if(nSide==FLOOR_LEFT)		pSep = pDB->GetCurGuardWall(CCentSeparation::LEFT);
	else if(nSide==FLOOR_RIGHT)	pSep = pDB->GetCurGuardWall(CCentSeparation::RIGHT);

	double	dHeightGuardWall	= pSep ? pSep->GetHeight() : 0;
	double	dW1					= pSep ? pSep->m_W1 : 0;
	double	dW2					= pSep ? pSep->m_W2 : 0;
	double	dW3					= pSep ? pSep->m_W3 : 0;
	double	dW4					= pSep ? pSep->m_W4 : 0;
	double	dW5					= pSep ? pSep->m_W5 : 0;

	if(bNanGan)		dH = toM(dHeightGuardWall + pCalcData->DESIGN_FLOOR_DATA[nSide].m_dNanGanHeight);
	else			dH = toM(dHeightGuardWall);

	if(bWalkLoad)
	{
		CPlateBasicIndex    *pBx = pGir->GetCrossBxAtMinFlangeWidth();

		dSharp		= pGir->GetLengthSharpOnBx(pBx, TRUE);
		dTerm		= nSide ? pDB->m_dWidthSlabRight : pDB->m_dWidthSlabLeft;
		dLEN		= pDB->GetLengthHDanDom(pBx, pDB->GetQtyHDanNode()-1);
		if(!pSep)	dLEN	= 0;
		double	dWebThick	= pGir->GetThickJewon(G_W, pBx);
		
		if(nSide==FLOOR_LEFT)	dL	= toM((dTerm - dSharp/2 - dLEN - dWebThick/2) + dW5 + dW4 + dW3/2);
		else					dL	= toM((dTerm - dSharp/2 - dLEN - dWebThick/2) + dW1 + dW2 + dW3/2);

		dHoriForce	= tokNPM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_HoriF);
		dVeriForce	= tokNPM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_VerF);

		dMoment		= dHoriForce*dH + dVeriForce*dL;

		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Co = frkNPM(dHoriForce);
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Mo = frkNM(dMoment);
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_dH = dH;
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Nangan_dL = dL;
	}
	else
	{
		if(pDB->IsGuard(0, 1))//  1 -> ��ȣ��
		{
			dVelocity	= pCalcData->m_dSpeed;
			dHoriForce	= tokN((dVelocity/60.)*(dVelocity/60.) * 7500.0 + 2500.0);	// kN/m
			dMoment1	= dHoriForce * dH2;
			dMoment2	= 10.0 * dH;

			dMoment = max(dMoment1,dMoment2);
		}

		if(dR <= 200 && dR != 0)
		{
			dMoment *= 2;
		}
	}

	if(pDB->IsGuard(0, 1) == 1)
	{
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Co = frkNPM(dHoriForce);
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Mo = frkNM(dMoment);
	}
}

void CCalcFloor::SetCentriodLoad(long nSide)
{
	CCalcData			*pCalcData		= m_pDataManage->GetCalcData();

	double	dL			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_L;
	BOOL	bWalkLoad	= pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad;

	if(	nSide==0 || nSide==1)
	{
		if(!bWalkLoad && dL>0)	NoneWalkLoad(nSide);
		else					WalkLoad(nSide);
	}
	else
	{
		SetDataInitInnerSlab();
	}
}

void CCalcFloor::NoneWalkLoad(long nSide)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CCalcGeneral CalcGeneral(m_pDataManage);

	long	nQtyGirder	= pDB->GetGirdersu();

	if(pDB->GetGuardWallSu() <= 0)	return;

	CPlateGirderApp		*pGir			= pDB->GetGirder(0);
	if(nSide==FLOOR_RIGHT)		pGir	= pDB->GetGirder(nQtyGirder-1);
	CPlateBxFinder		Finder(pGir);

	double	dPr			= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dE			= toM(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_E);
	double	dR			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_R;
	double	dV			= pCalcData->m_dSpeed;
	double	dCf1=0, dCf2=0, dCf3=0, dPcf1=0, dPcf2=0, dPcf3=0, dMcf1=0, dMcf2=0, dMcf3=0;
	BOOL	bTank		= pCalcData->m_bTank;
	BOOL	bTrailer	= pCalcData->m_bTrailer;

	if(dR==0)	//R�� ���Ѵ��
	{
		dCf1	= 0;
		dPcf1	= 0;
		dMcf1	= 0;
	}
	else
	{
		dCf1	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
		dPcf1	= dPr / dE * dCf1/100.0;
		dMcf1	= dPcf1 * 1.8;
	}

	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf	= frkNM(dMcf1);
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf	= frkNPM(dPcf1);

	if(bTank)
	{
		dPr = pCalcData->m_TankWeight / 2.0 / pCalcData->m_TankEdit1;

		dV		= 40;
		if(dR == 0)	//R�� ���Ѵ��
		{			
			dCf2	= 0;
			dPcf2	= 0;
			dMcf2	= 0;
		}
		else
		{			
			dCf2	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
			dPcf2	= dPr * dCf2 / 100.0;
			dMcf2	= dPcf2 * 1.8;
		}

		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Tank = dMcf2;
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf_Tank = dPcf2;
	}

	if(bTrailer)
	{
		dPr = max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));

		dV		= 40;
		if(dR==0)	//R�� ���Ѵ��
		{			
			dCf3	= 0;
			dPcf3	= 0;
			dMcf3	= 0;
		}
		else
		{	
			//����ü ������ �ȵǳ�..��..070406...KB....070410..�����߾�...��...
			double  dL3		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Trailer_L;
			double  dE3		= CalcGeneral.CalcMovingLoad_E(dL3);

			dCf3	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
			dPcf3	= Round(dPr / dE3 * dCf3/100.0,3);
			dMcf3	= Round(dPcf3 * 1.8,3);
		}

		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Trailer = dMcf3;
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf_Trailer = dPcf3;
	}
}

void CCalcFloor::WalkLoad(long nSide)
{
	CCalcData			*pCalcData		= m_pDataManage->GetCalcData();

	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf			= 0;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf_Tank		= 0;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Pcf_Trailer	= 0;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf			= 0;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Trailer	= 0;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Tank		= 0;
}

void CCalcFloor::SetDataInitInnerSlab()
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData		= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir			= pDB->GetGirder(pDB->GetGirdersu()-1);
	CCalcGeneral CalcGeneral(m_pDataManage);
	if(pDB->GetGuardWallSu() <= 0)	return;

	//��������
	double	dL		= toM(CalcGeneral.CalcCentPart_L() + pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch);
	double	dPr		= tokN(pCalcData->DESIGN_CONDITION.m_dPr);
	double	dV		= pCalcData->m_dSpeed;
	double	dR		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_R;
	double	dtankV	= 40;
	double	dE		= Round(2.4 * dL/(dL+0.6),3);
	double	dCf1	= 0, dCf2=0, dCf3=0, dPcf1=0, dPcf2=0, dPcf3=0, dMcf1=0, dMcf2=0, dMcf3=0;

	if(dR==0)	//R�� ���Ѵ��
	{
		dCf1	= 0;
		dPcf1	= 0;
		dMcf1	= 0;
	}
	else
	{
		dCf1	= CalcGeneral.CalcCentriodLoad_CF(dV, dR);
		dPcf1	= dPr/dE  * dCf1/100.0;
		dMcf1	= dPcf1 * 1.8;
	}

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Pcf = frkNPM(dPcf1);
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf = frkNM(dMcf1);

	if(pCalcData->m_bTank)
	{
		dPr = pCalcData->m_TankWeight / 2 / pCalcData->m_TankEdit1;

		if(dR==0)	//R�� ���Ѵ��
		{
			dtankV	= 40;
			dCf2	= 0;
			dPcf2	= 0;
			dMcf2	= 0;
		}
		else
		{
			dtankV	= 40;
			dCf2	= CalcGeneral.CalcCentriodLoad_CF(dtankV, dR);
			dPcf2	= dPr * dCf2/100.0;
			dMcf2	= dPcf2 * 1.8;
		}

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Pcf_Tank = frkNPM(dPcf2);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Tank = frkNM(dMcf2);

	}

	if(pCalcData->m_bTrailer)
	{
		dPr = max(pCalcData->m_TrailP1, max(pCalcData->m_TrailP2, pCalcData->m_TrailP3));

		if(dR==0)	//R�� ���Ѵ��
		{
			dtankV	= 40;
			dCf3	= 0;
			dPcf3	= 0;
			dMcf3	= 0;
		}
		else
		{
			dtankV	= 40;
			dCf3	= CalcGeneral.CalcCentriodLoad_CF(dtankV, dR);
			dPcf3	= dPr / dE * dCf3/100.0;
			dMcf3	= dPcf3 * 1.8;
		}

		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Pcf_Trailer = frkNPM(dPcf3);
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Trailer = frkNM(dMcf3);
	}
}

void CCalcFloor::SetLoadComposite(long nSide)
{
	if(nSide==FLOOR_MID)	SetLoadCompositeCenter();
	else					SetLoadCompositeCantilever(nSide);
}

void CCalcFloor::SetLoadCompositeCantilever(long nSide)
{
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();

	double dD	= 0;		// ������
	double dCo	= 0;		// �浹����
	double dCF	= 0;		// ��������
	double dCF2	= 0;		// ��������
	double dCF3	= 0;		// ��������
	double dLi	= 0;		// Ȱ����
	double dLi2	= 0;		// Ȱ����
	double dLi3	= 0;		// Ȱ����
	double dW	= 0;		// ǳ����
	double dMu	= 0, dMu_1 = 0, dMu_2 = 0, dMu_3 = 0, dMu_4 = 0, dMu_5 = 0, dMu_6 = 0;

	dD   = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Stop_M;				// ������
	dCo  = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Coll_Mo;			// �浹����
	dCF  = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf;			// ��������
	dCF2 = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Tank;		// ��������(��ũ�˵������϶�)
	dCF3 = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Cent_Mcf_Trailer;	// ��������(��ũƮ���Ϸ������϶�)
	dLi  = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml;			// Ȱ����
	dLi2 = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml_Tank;		// Ȱ����(��ũ�˵������϶�)
	dLi3 = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml_Trailer;	// Ȱ����(��ũƮ���Ϸ������϶�)
	dW   = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Wind_Mw;			// ǳ����

	if(pCalcData->DESIGN_FLOOR_DATA[nSide].m_bWalkLoad)
	{
		dMu_1 = 1.3 * dD + 2.15* dLi;
		dMu = dMu > dMu_1 ? dMu : dMu_1;
		dMu_2 = 1.3 * dD + 1.3 * dLi + 1.3 * dCo;
		dMu = dMu > dMu_2 ? dMu : dMu_2;
		dMu_3 = 1.3 * dD + 1.3 * dLi + 0.65 * dW;
		dMu = dMu > dMu_3 ? dMu : dMu_3;
		dMu_4 = 1.2 * dD + 1.2 * dW + 1.2 * dCo;
		dMu = dMu > dMu_4 ? dMu : dMu_4;
	}
	else
	{
		dMu_1 = 1.3 * dD + 2.15 * dLi + 1.3 * dCF;
		dMu = max(dMu, dMu_1);
		dMu_2 = 1.3 * dD + 1.3 * dLi + 1.3 * dCF + 1.3 * dCo;
		dMu = max(dMu, dMu_2);
		dMu_3 = 1.3 * dD + 1.3 * dLi + 1.3 * dCF + 0.65 * dW;
		dMu = max(dMu, dMu_3);
		dMu_4 = 1.2 * dD + 1.2 * dW + 1.2 * dCo;
		dMu = max(dMu, dMu_4);

		if(pCalcData->m_bTank)
		{
			dMu_5 = 1.3 * dD + 1.3 * dLi2 + 1.3 * dCF2;
			dMu = dMu > dMu_5 ? dMu : dMu_5;
		}

		if(pCalcData->m_bTrailer)
		{
			if(!pCalcData->m_bTank)
			{
				dMu_5 = 1.3 * dD + 1.3 * dLi3 + 1.3 * dCF3;
				dMu = dMu > dMu_5 ? dMu : dMu_5;
			}
			else
			{
				dMu_6 = 1.3 * dD + 1.3 * dLi3 + 1.3 * dCF3;
				dMu = dMu > dMu_6 ? dMu : dMu_6;
			}
		}

	}
 	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Mu = dMu;
}

void CCalcFloor::SetLoadCompositeCenter()
{
	CCalcData		*pCalcData	= m_pDataManage->GetCalcData();

	double	dD		= 0;
	double	dML1	= 0;
	double	dML2	= 0;
	double	dML3	= 0;
	double	dMcf1	= 0;
	double	dMcf2	= 0;
	double	dMcf3	= 0;
	double	dMu_1	= 0;
	double	dMu_2	= 0;
	double	dMu_3	= 0;

	dD		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Stop_M;
	dML1	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml;
	dML2	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Tank;
	dML3	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_Ml_Trailer;
	dMcf1	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf;
	dMcf2	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Tank;
	dMcf3	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Cent_Mcf_Trailer;
	dMu_1	= 1.3 * dD + 2.15 * dML1 + 1.3 * dMcf1;
	dMu_2	= 1.3 * dD +  1.3 * dML2 + 1.3 * dMcf2;
	dMu_3	= 1.3 * dD +  1.3 * dML3 + 1.3 * dMcf3;

	if(pCalcData->m_bTank)
	{
		dMu_1 = dMu_1 > dMu_2 ? dMu_1 : dMu_2;
	}

	if(pCalcData->m_bTrailer)
	{
		dMu_1 = dMu_1 > dMu_3 ? dMu_1 : dMu_3;
	}

	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Mu = dMu_1;
}

// �ٴ��� �߾Ӻ� ������ ���� �� ���ǰ���
void CCalcFloor::SetFloorExperienceDesign()
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CCalcGeneral		CalcGeneral(m_pDataManage);

	CString szText	= _T("");
	CMapStringToString	mapRtn;
	double	dSlabThick				= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;		// �ٴ��� �ּ� �β�
	long	dMainDiaUpper_1Cy		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1;			// ��ö�� ��� Dia Idx 1-Cylce
	long	dMainDiaUpper_2Cy		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo1_2Cy;		// ��ö�� ��� Dia Idx 2-Cycle
	double	dMainCTCUpper			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc1;						// ��ö�� ��� ����
	long	dMainDiaLower_1Cy		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_1Cy;	// ��ö�� �ϸ� Dia Idx 1-Cylce
	long	dMainDiaLower_2Cy		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo1_2Cy;	// ��ö�� �ϸ� Dia Idx 2-Cycle
	double	dMainCTCLower			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc1;					// ��ö�� �ϸ� ����
	long	dBaeRuckDiaUpper		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo3;			// ���ö�� ��� Dia Idx
	double	dBaeRuckCTCUpper		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB;						// ���ö�� ��� ����
	long	dBaeRuckDiaLower		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Combo2;		// ���ö�� �ϸ� Dia Idx
	double	dBaeRuckCTCLower		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Ctc2;					// ���ö�� �ϸ� ����
	long	dEndReinforceDiaMain_1Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2;			// �ܺκ��� ��ö�� Dia Idx 1-Cycle
	long	dEndReinforceDiaMain_2Cy= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy;		// �ܺκ��� ��ö�� Dia Idx 2-Cycle
	double	dEndReinforceCTCMain	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// �ܺκ��� ��ö�� ����
	long	dEndReinforceDiaBeryuk	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo4;			// �ܺκ��� ���ö�� Dia Idx
	double	dEndReinforceCTCBeryuk	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_CtcB1;						// �ܺκ��� ���ö�� ����
	double	dSlabArea				= 1000*dSlabThick;
	double	dB						= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_b = 1000.0;

	// �Ϲݺ� ��ö�� �ϸ� ö��
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs_1Cy		= _dRebarArea[dMainDiaUpper_1Cy] * dB / dMainCTCUpper;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs_2Cy		= _dRebarArea[dMainDiaUpper_2Cy] * dB / dMainCTCUpper;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asuse		= (_dRebarArea[dMainDiaUpper_1Cy]+_dRebarArea[dMainDiaUpper_2Cy]) * dB / dMainCTCUpper;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asreq		= dSlabArea * 0.004;
	
	if(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asuse>pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asreq)
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen1	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen1	= FALSE;
	// �Ϲݺ� ��ö�� ��� ö��
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1_1Cy	= _dRebarArea[dMainDiaLower_1Cy] * dB / dMainCTCLower;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1_2Cy	= _dRebarArea[dMainDiaLower_2Cy] * dB / dMainCTCLower;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1		= (_dRebarArea[dMainDiaLower_1Cy]+_dRebarArea[dMainDiaLower_2Cy]) * dB / dMainCTCLower;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1		= dSlabArea * 0.003;
	if(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse1>pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1)
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheck_Up	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheck_Up	= FALSE;
	// �Ϲݺ� ���ö�� �ϸ� ö��
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsuse		= _dRebarArea[dBaeRuckDiaUpper] * dB / dBaeRuckCTCUpper;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsreq		= dSlabArea * 0.003;
	if(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsuse>pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsreq)
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen2	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckGen2	= FALSE;
	// �Ϲݺ� ���ö�� ��� ö��
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse2		= _dRebarArea[dBaeRuckDiaLower] * dB / dBaeRuckCTCLower;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1		= dSlabArea * 0.003;
	if(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asuse2>pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Up_Asreq1)
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheck_UpB	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheck_UpB	= FALSE;
	// �ܺ� ��ö��
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_1Cy	= _dRebarArea[dEndReinforceDiaMain_1Cy] * dB / dEndReinforceCTCMain;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_2Cy	= _dRebarArea[dEndReinforceDiaMain_2Cy] * dB / dEndReinforceCTCMain;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2		= (_dRebarArea[dEndReinforceDiaMain_1Cy]+_dRebarArea[dEndReinforceDiaMain_2Cy]) * dB / dEndReinforceCTCMain;
	if(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2>pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_Asreq*2)
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan1	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan1	= FALSE;
	// �ܺ� ���ö��
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_BAsuse2		= _dRebarArea[dEndReinforceDiaBeryuk] * dB / dEndReinforceCTCBeryuk;
	if(pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_BAsuse2>pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Lo_BAsreq*2)
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan2	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_bCheckDan2	= FALSE;
}

// nCheckType = 0 : ���������		1 : ������ �����
void CCalcFloor::SetCantileverRebar(long nSide, long nCheckType)
{
	CCalcData		*pCalcData	= m_pDataManage->GetCalcData();
	CGlobarOption	*pOpt		= m_pDataManage->GetGlobalOption();
	if(nCheckType==1)
	{
		SetFloorExperienceDesign();
		return;
	}

	double	dK1		= 0;
	double	dWidthb	= 1000.0;
	double	dMinT	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Min_T;
	double	dCk		= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dY		= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dPib	= pCalcData->m_Pib;   // not
	double	dL		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Dead_L;
	double	dReinMu	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Mu;
	double	dCover	= nSide==2 ? pCalcData->m_dCoverDn : pCalcData->m_dCoverUp;
//	double	dDisD	= dMinT -pDB->m_BindConc.m_dDeep 	- dCover;	// 2010.11.19 - ��꼭���� ������� ����  ��꼭�� ����
	double	dDisD	= dMinT - dCover;	
	double	dAsreq	= 0;
	double	dAsuse	= 0;
	double	dCTCCenM = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Ctc1;
	long	nCombo1_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo1;
	long	nCombo1_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo1_2Cy;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��ö�ٷ� ���� 
	double	dPhiF	= 0.85;
	if(dCk<=28)	dK1 = dPhiF;
	else		dK1 = dPhiF-(dCk-28)*0.007;


	double dA = (dY/(dK1 * dCk * dWidthb) * dPhiF * dY)/2;	//2��������(AX��+BX+C=0) ���� ���� �̿�
	double dB = -dPhiF * dY * dDisD;
	double dC =dReinMu;

	if(dB * dB - 4 * dA * dC > 0)
	{
		double dX1 = (-dB + sqrt(dB * dB - 4 * dA * dC)) / (2 * dA);
		double dX2 = (-dB - sqrt(dB * dB - 4 * dA * dC)) / (2 * dA);
		if(dX1 > dX2 && dX2 > 0)	dAsreq = dX2;
		else						dAsreq = dX1;
	}
	else	dAsreq = 0;

	if(dCTCCenM==0)	dAsuse = 0.000;
	else			dAsuse = (_dRebarArea[nCombo1_1Cy]+_dRebarArea[nCombo1_2Cy]) * dWidthb / dCTCCenM;

	if(pOpt->GetSlabCentDesignMethod() == 2)
		dAsuse += _dRebarArea[pCalcData->m_nLBDeckDiaIdx] * dWidthb / pCalcData->m_dLBDeckCTC;

	double	dRebarA		= dAsuse * dY / (0.85 * dCk * dWidthb);
	double	dPb			= dK1 * dPib * (dCk/dY) * (600.0/(600.0+dY));//(dPib * dK1 * dCk * 0.003) / (dY * (0.003 + dY / dEs));
	double	dRebarPUsed	= dAsuse/(dWidthb * dDisD);
	double	dRebarMd	= dPib * dAsuse * dY * (dDisD-dRebarA/2);

	long	nCombo2_1Cy = pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo2;
	long	nCombo2_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo2_2Cy;
	long	nCombo3		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo3;
	long	nCombo4		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo4;
	double	dCTCDanM	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Ctc2;
	double	dCTCCenB	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB;
	double	dCTCDanB	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB1;
	double	dBRebarPercentUse	= 0;
	double	dBRebarPercent		= 120 / sqrt(toM(dL));
	if(dBRebarPercent > 67)		dBRebarPercentUse = 67;
	else						dBRebarPercentUse = dBRebarPercent;

	double	dAsReqCenB	= dAsreq*dBRebarPercentUse/100;
	double	dAsUseCenB	= dWidthb/dCTCCenB*_dRebarArea[nCombo3];
	double	dAsUseDanM	= 0;
	if(dCTCDanM==0)	 dAsUseDanM = 0.000;
	else			 dAsUseDanM = (_dRebarArea[nCombo2_1Cy]+_dRebarArea[nCombo2_2Cy])*dWidthb/dCTCDanM;
		
	double	dAsUseDanB = dWidthb/dCTCDanB*_dRebarArea[nCombo4];

	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_d			= dDisD;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_a			= dRebarA;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_b			= dWidthb;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Asreq		= dAsreq;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs		= dAsuse;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs_1Cy	= _dRebarArea[nCombo1_1Cy] * dWidthb / dCTCCenM;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs_2Cy	= _dRebarArea[nCombo1_2Cy] * dWidthb / dCTCCenM;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2	= dAsUseDanM;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2_1Cy= _dRebarArea[nCombo2_1Cy] * dWidthb / dCTCDanM;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2_2Cy= _dRebarArea[nCombo2_2Cy] * dWidthb / dCTCDanM;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Pused		= dRebarPUsed;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Md		= dRebarMd;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsreq	= dAsReqCenB;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsuse	= dAsUseCenB;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsuse2	= dAsUseDanB;
	pCalcData->m_dRebarPmax									= 0.75*dPb;

	// �Ϲݺ� ��ö�� ����
	if(dAsuse>dAsreq)			pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen1	= TRUE;
	else						pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen1	= FALSE;
	if(dRebarPUsed<0.75*dPb)	pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen1	= TRUE;
	else						pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen1	= FALSE;
	if(dRebarMd>dReinMu)		pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen1	= TRUE;
	else						pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen1	= FALSE;
	// �ܺ� ��ö�� ����
	if(dAsUseDanM>2*dAsreq)		pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckDan1	= TRUE;
	else						pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckDan1	= FALSE;
	// �Ϲݺ� ���ö�� ����
	if(dAsUseCenB>0.002*dWidthb*dMinT && dAsUseCenB>dAsreq*0.67)
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen2	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckGen2	= FALSE;
	// �ܺ� ���ö�� ����
	if(dAsUseDanB>2*dAsReqCenB)
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckDan2	= TRUE;
	else
		pCalcData->CALC_CANTILEVER_DATA[nSide].m_bCheckDan2	= FALSE;
}

void CCalcFloor::SetInnerJijumRebar()
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CCalcData		*pCalcData	= m_pDataManage->GetCalcData();

	//�ٴ��� ����
	long	nCombo1	= pCalcData->m_ComboInnerJijumSlab1;
	long	nCombo2	= pCalcData->m_ComboInnerJijumSlab2;
	double	dH		= pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;//pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Min_T;
	double	dB		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_b;
	double	dCtc1	= pCalcData->m_Ctc1;
	double	dCtc2	= pCalcData->m_Ctc2;
	double	dAg		= dB * dH;
	double	dAsreq	= dAg * 0.02;
	double	dAsuse1	= _dRebarArea[nCombo1] * dB / dCtc1;
	double	dAsuse2	= _dRebarArea[nCombo2] * dB / dCtc2;
	double	dAsuset	= dAsuse1 + dAsuse2;
	double	dAsEa1	= dB / dCtc1;
	double	dAsEa2	= dB / dCtc2;

	//����� ����
	double	dDia1	= _dRebarDiadim[nCombo1];
	double	dDia2	= _dRebarDiadim[nCombo2];
	double	dCalc2	= 3.141592 * dDia1 * dAsEa1 + 3.141592 * dDia2 * dAsEa2;
	double	dCalc3	= dB * dH;
	double	dJu_ratio = Round(dCalc2 / dCalc3,4);

	//pCalcData->m_CaseH1.Format(".0f", dDia1);
	//pCalcData->m_CaseH2.Format(".0f", dDia2);
	pCalcData->m_AsEa1		= dAsEa1;
	pCalcData->m_AsEa2		= dAsEa2;
	pCalcData->m_Ju_ratio	= dJu_ratio;
	pCalcData->m_Ag			= dAg;
	pCalcData->m_Asreq		= dAsreq;
	pCalcData->m_Asuse1		= dAsuse1;
	pCalcData->m_Asuse2		= dAsuse2;
	pCalcData->m_Asuset		= dAsuset;

	if(dAsuset>dAsreq)	pCalcData->m_bCheckInnerJijum	= TRUE;
	else				pCalcData->m_bCheckInnerJijum	= FALSE;
	
}

double CCalcFloor::GetCantileverDroop(long nSide)
{
	CCalcData	*pCalcData	= m_pDataManage->GetCalcData();

	double	dDeadL	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Dead_L;
	double	dSigmaY	= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dT		= 0.0;
	double	dDef	= 10.0;
	if(nSide == FLOOR_MID)	dDef = 28.0;

	if(dSigmaY == 400.0)	dT	= dDeadL / dDef;
	else					dT	= dDeadL / dDef * (0.43 + dSigmaY/700);

	return dT;
}

void CCalcFloor::SetCantileverUltimateLoad(long nSide, CMap<CString, LPCTSTR, double, double> &Map)
{
	CPlateBridgeApp	*pBridge	= m_pDataManage->GetBridge();
	CCalcData		*pCalcData	= m_pDataManage->GetCalcData();
	CCalcGeneral	CalcGeneral(m_pDataManage);

	Map.RemoveAll();
	CMap <CString, LPCTSTR, double, double> MapStress;
	CalcPreStress(MapStress, nSide);

	// ��ö�� ����
	double	dTensionStrength	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength;
	double	dB					= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_b;
	long	nGenTDiaIdx_1Cy		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_nGenReinTDiaIdx_1Cy;
	long	nGenTDiaIdx_2Cy		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_nGenReinTDiaIdx_2Cy;
	double	dGenTCTC			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTCTC;
	double	dGenTAsuse			= (_dRebarArea[nGenTDiaIdx_1Cy]+_dRebarArea[nGenTDiaIdx_2Cy]) * dB / dGenTCTC;
	long	nGenCDiaIdx_1Cy		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_nGenReinCDiaIdx_1Cy;
	long	nGenCDiaIdx_2Cy		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_nGenReinCDiaIdx_2Cy;
	double	dGenCCTC			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCCTC;
	double	dGenCAsuse			= (_dRebarArea[nGenCDiaIdx_1Cy]+_dRebarArea[nGenCDiaIdx_1Cy]) * dB / dGenCCTC;
	double	dMu					= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Mu;
	double	dMl					= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Move_Ml;
	double	dTc					= pBridge->m_dThickSlabBase;
	if(nSide==FLOOR_MID)	dTc = pBridge->m_dThickSlabBase - pBridge->m_dThickSlabHunch;
	double	dPibokUp		= pCalcData->m_dCoverUp;
	double	dPibokDn		= pCalcData->m_dCoverDn;
	double	dLength			= MapStress["Length"];
	double	dLengthStoPS	= MapStress["Length_SlabToPS"];
	double	dSlabThick		= MapStress["Slab_Thick"];
	double	dZ				= MapStress["Z"];
	double	dI				= MapStress["I"];
	double	dPe				= MapStress["Ptd"];
	double	dMtd			= MapStress["Mtd"];
	double	dSlabArea		= MapStress["Slab_Area"];
	double	dPreDeadLoad	= MapStress["MomentPreDeadLoad"];
	double	dPostDeadLoad	= MapStress["MomentPostDeadLoad"];
	double	dRp				= 0.28;
	double	dFck			= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dFy				= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dB1				= 0.85 - 0.007*(dFck-28.0);
	if(dB1<0.65)			dB1 = 0.65;
	long	nPSQty			= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	double	dArea			= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea*nPSQty;
	double	dQty			= 1000.0/pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	double	dAp				= dArea*dQty;
	double	dDepthPS		= dLengthStoPS;
	double	dDepthTS		= dTc-dPibokDn;
	double	dDepthCS		= dTc-dPibokUp;
	double	dPp				= dAp/(dB*dDepthPS);			// �����
	double	dP				= dGenTAsuse/(dB*dDepthTS);		// ����ö�ٺ�
	double	dPpri			= dGenCAsuse/(dB*dDepthCS);		// ����ö�ٺ�
	double	dFps			= dTensionStrength * (1 - dRp/dB1*(dPp*dTensionStrength/dFck + (dTc-dPibokDn)/dLengthStoPS*(dP*dFy/dFck-dPpri*dFy/dFck)));
	double	dFlangeCheck	= 1.4*dDepthPS*dPp*dFps/dFck;
	double	dBendMn			= 0.85*(dAp*dFps*dDepthPS*(1-0.59*dPp*dFps/dFck));			///< �ڰ���
	double	dQp				= dPp*dFps/dFck;												///< �ִ� �����
	double	dMinQp			= 0.36*dB1;
	double	dFru			= 0.63*sqrt(dFck);
	double	dMcr			= dZ * (dFru + dPe/dSlabArea+dPe*dLength/dZ);					///< �տ� ���Ʈ
	double	dMinSteel		= dBendMn/dMcr;													///< �ּ� ���緮
	double	dDanMu			= 2*dMl+dPreDeadLoad+dPostDeadLoad;
	double	dDanPe			= dPe;
	double	dUpper			= dPe/dSlabArea + dMtd/dI*dLength - dDanMu/dI*dLength;
	double	dLower			= dPe/dSlabArea - dMtd/dI*dLength + dDanMu/dI*dLength;
	double	dX				= ABS(dUpper)*dSlabThick/(ABS(dUpper)+ABS(dLower));
	double	dT				= ABS(dUpper)*dX*0.5;
	double	dfas			= pCalcData->DESIGN_MATERIAL.m_dSigmaSA;
	double	dAs				= dT/dfas;
	double	dDMainCTC		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Ctc2;						// �ܺ� ��ö��
	long	nDanDiaIdx_1Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo2;						// �ܺ� ��ö�� 1 Cycle
	long	nDanDiaIdx_2Cy	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo2_2Cy;					// �ܺ� ��ö�� 2 Cycle
	double	dDanAsuse		= (_dRebarArea[nDanDiaIdx_1Cy]+_dRebarArea[nDanDiaIdx_2Cy]) * dB / dDMainCTC;	// �ܺ� ��ö�� ��� ö�ٷ�
	double	dDanBaeCTC		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB1;
	long	nDanDiaIdx		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo4;
	double	dH				= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Min_T;
	double	dDeadL			= CalcGeneral.CalcCentPart_L();
	
	double	dPer			= min(120.0 / sqrt(toM(dDeadL)), 67.0);
	double	dAsuse			= dGenTAsuse;
	double	dReqAs1			= dAsuse*dPer/100.0;
	double	dReqAs2			= 0.002*dB*dH;
	long	nCombo3			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo3;
	double	dCtcB			= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB;
	double	dBAsuse			= _dRebarArea[nCombo3]*dB/dCtcB;
	// �Ϲݺ� ��ö��
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTAsuse_1Cy	= _dRebarArea[nGenTDiaIdx_1Cy] * dB / dGenTCTC;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTAsuse_2Cy	= _dRebarArea[nGenTDiaIdx_2Cy] * dB / dGenTCTC;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTAsuse		= dGenTAsuse;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCAsuse_1Cy	= _dRebarArea[nGenCDiaIdx_1Cy] * dB / dGenCCTC;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCAsuse_2Cy	= _dRebarArea[nGenCDiaIdx_2Cy] * dB / dGenCCTC;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinCAsuse		= dGenCAsuse;
	// �Ϲݺ� ���ö��
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsuse		= dBAsuse;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsreq		= max(dReqAs1, dReqAs2);
	// �ܺ� ��ö��
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2_1Cy	= _dRebarArea[nDanDiaIdx_1Cy] * dB / dDMainCTC;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2_2Cy	= _dRebarArea[nDanDiaIdx_2Cy] * dB / dDMainCTC;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAs2		= dDanAsuse;
	// �ܺ� ���ö��
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsuse2		= _dRebarArea[nDanDiaIdx] * dB / dDanBaeCTC;
	pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_UseAsBr		= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_BAsreq*2;
	Map["UltimateLoad"]	= dMu;
	Map["Rate_Tension"]	= dP;
	Map["Rate_Compre"]	= dPpri;
	Map["Fps"]			= dFps;
	Map["Bend_Mn"]		= dBendMn;
	Map["MaxSteelRate"]	= dQp;
	Map["MinSteel"]		= dMinSteel;
	Map["FlangeCheck"]	= dFlangeCheck;
	Map["Beta"]			= dB1;
	Map["Mcr"]			= dMcr;
	Map["D_Mu"]			= dDanMu;
	Map["D_PreLoad"]	= dPreDeadLoad;
	Map["D_PostLoad"]	= dPostDeadLoad;
	Map["D_LiveLoad"]	= dMl;
	Map["D_Pe"]			= dDanPe;
	Map["D_Me"]			= dMtd;
	Map["D_Upper"]		= dUpper;
	Map["D_Lower"]		= dLower;
	Map["D_X"]			= dX;
	Map["D_T"]			= dT;
	Map["D_As"]			= dAs;
	Map["Slab_Thick"]	= dSlabThick;
	Map["Depth_PS"]		= dDepthPS;
	Map["Rho_PS"]		= dPp;
	Map["Dead_L"]		= dDeadL;
	Map["Main_Rate"]	= 120.0 / sqrt(toM(dDeadL));
	Map["Use_Rate"]		= dPer;
	Map["MinQp"]		= dMinQp;
}

long GetRebar(long nIdx)
{
	if(nIdx == 0)	return 13;
	if(nIdx == 1)	return 16;
	if(nIdx == 2)	return 19;
	if(nIdx == 3)	return 22;
	if(nIdx == 4)	return 25;
	if(nIdx == 5)	return 29;
	if(nIdx == 6)	return 32;
	if(nIdx == 7)	return 35;
	
	return -1;
}

double GetArea(double nDia)
{
	if (fabs(nDia-13)<EPSILON) return 126.7;
	if (fabs(nDia-16)<EPSILON) return 198.6;
	if (fabs(nDia-19)<EPSILON) return 286.5;
	if (fabs(nDia-22)<EPSILON) return 387.1;
	if (fabs(nDia-25)<EPSILON) return 506.7;
	if (fabs(nDia-29)<EPSILON) return 642.4;
	if (fabs(nDia-32)<EPSILON) return 794.2;
	if (fabs(nDia-35)<EPSILON) return 956.6;

	return -1;
}

double GetDia(double nDia)
{
	if (fabs(nDia-13)<EPSILON) return 12.7;
	if (fabs(nDia-16)<EPSILON) return 15.9;
	if (fabs(nDia-19)<EPSILON) return 19.1;
	if (fabs(nDia-22)<EPSILON) return 22.2;
	if (fabs(nDia-25)<EPSILON) return 25.4;
	if (fabs(nDia-29)<EPSILON) return 28.6;
	if (fabs(nDia-32)<EPSILON) return 31.8;
	if (fabs(nDia-35)<EPSILON) return 34.9;
	
	return -1;
}

// �������߿� ���� �ܸ����
void CCalcFloor::CalcCantileverUltimateLoadCheck(CAPlateCalcStd* pCalc, CMap<CString, LPCTSTR, double, double> &Map, long nSide)
{
	CMap <CString, LPCTSTR, double, double> MapL;	// ������Ʈ���� �ս�
	CalcPreStressLoss(pCalc, MapL, nSide);

	// nSide=0 : Left, nSide=1 : Right, nSide=2 : Middle
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	if(pCalc->GetSizeCantileverFemData() == 0)	return;
	CFEMManage			*pFEM		= pCalc->GetCantileverFemData(0);

	//-------------------------------------------------------------------------------------------------
	// 1. ������ ������ PS ������ ����

	double dBar_Dia1 = GetRebar(pCalcData->CALC_CANTILEVER_DATA[nSide].m_nGenReinTDiaIdx_1Cy);
	double dBar_Spa1 = pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTCTC;
	double dBar_Are1 = GetArea(dBar_Dia1) * 1000 / dBar_Spa1;
	double dBar_Dia2 = GetRebar(pCalcData->CALC_CANTILEVER_DATA[nSide].m_nGenReinTDiaIdx_2Cy);
	double dBar_Spa2 = pCalcData->CALC_CANTILEVER_DATA[nSide].m_dGenReinTCTC;
	double dBar_Are2 = GetArea(dBar_Dia2) * 1000 / dBar_Spa2;
	double dBar_Area = dBar_Are1 + dBar_Are2;
	double dFpu		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionStrength;	// ���Ѱ���
	double dRp		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPSSteelYP;		// PS ������ ������ ���� ���
	double dFck		= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;					// ��ũ��Ʈ ���� ���ذ���
	double dFy		= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double dBeta	= max( 0.85- 0.007*(dFck-28), 0.650);
	long   nSheath	= 1000 / (long)pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	double dAp		= MapL["Tendon_Area"] * MapL["Tendon_Qty"] * nSheath;
	double dAs		= dBar_Area;
	double dSlab_Th	= (nSide==0)? MapL["Height_Left"] : (nSide==1)? MapL["Height_Right"] : MapL["Height_Middle"];
	double dDp		= (nSide==0)? dSlab_Th-pDB->m_dHeightLeftTension : (nSide==1)? dSlab_Th-pDB->m_dHeightRightTension : pDB->GetStdMaxTendonHeight();
	double dDo		= (nSide!=2)? dSlab_Th - 60 : dSlab_Th - 40; 
	double dPp		= dAp / (1000 * dDp);
	double dPo		= dAs / (1000 * dDo);
	double dFps		= dFpu * (1 - (dRp/dBeta)*( dPp*dFpu/dFck + (dDo/dDp)*(dPo*dFy/dFck) ) );	// ������ ������ PS ������ ���� (fps)

	//-------------------------------------------------------------------------------------------------	
	// 2. ���Ѱ��� ����
	// �簢�� �ܸ� �Ǵ� �߸����� �����÷��� ���� �ִ� �÷����� ���� �ܸ鿡 ���� �����ڰ���
	double dPi	  = 0.85;
	double dPiMn1 = ( dAp*dFps*dDp*(1-0.59*(dPp*dFps/dFck + dDo*dPo*dFy/(dDp*dFck))) +
				      dAs*dFy*dDo*(1-0.59*(dDp*dPp*dFps/(dDo*dFck) + dPo*dFy/dFck)) ) * pow(10.0, -6) * dPi;
	// �������� ö���� ��ġ���� �ʾҰų� ��ġ�ߴ��� �� ������ ������ �� �ִ� �ܸ鿡 ���� �����ڰ���
	double dPiMn2 = dPi * dAp*dFps*dDp*(1-0.59*dPp*dFps/dFck) * pow(10.0, -6);

	// �ִ� �����
	double dQp =  dPp*dFps/dFck;

	// �ּ� ���緮
	double dFru		= 0.63*sqrt(dFck);
	double dAxial	= MapL["Time_EFJ"] * dAp;
	double dSlab_A	= dSlab_Th*1000;
	double dZc		= (1000*pow(dSlab_Th,3)/12)/(dSlab_Th/2);
	double dep		= (nSide==0)? MapL["Ep_Left"]	: (nSide==1)? MapL["Ep_Right"] : MapL["Ep_Middle"];
	double dFpcc	= dAxial/dSlab_A + dAxial/dZc*dep;
	double dMcr		= (dFru+dFpcc)*dZc*pow(10.0,-6);

	// ���Ѱ���
	double dBefore	 = GetMemberForce(pFEM, "BEFORE", ELE_MOMENT2, nSide);
	double dAfter	 = GetMemberForce(pFEM, "AFTER", ELE_MOMENT2, nSide);
	double dLive_Max = GetMemberForce(pFEM, "LIVE MAX", ELE_MOMENT2, nSide);
	double dLive_Min = GetMemberForce(pFEM, "LIVE MIN", ELE_MOMENT2, nSide);
	double dTank	 = GetMemberForce(pFEM, "TK MAX", ELE_MOMENT2, nSide);
	//double dTrailer	 = GetMemberForce(pFEM, "TR MAX", ELE_MOMENT2, nSide); // ������ �ʴ� ����
	double dWind_Max = GetMemberForce(pFEM, "WIND MAX", ELE_MOMENT2, nSide);
	double dWind_Min = GetMemberForce(pFEM, "WIND MIN", ELE_MOMENT2, nSide);
	double dCF_Max	 = GetMemberForce(pFEM, "CF MAX", ELE_MOMENT2, nSide);
	double dCF_Min	 = GetMemberForce(pFEM, "CF MIN", ELE_MOMENT2, nSide);
	double dCrash_Max= GetMemberForce(pFEM, "CRASH MAX", ELE_MOMENT2, nSide);
	double dCrash_Min= GetMemberForce(pFEM, "CRASH MIN", ELE_MOMENT2, nSide);

	double dUnitValue[5] = { 0, 0, 0, 0, 0 };
	double dUnitFac[5]	 = { 0, 0, 0, 0, 0 };
	double dResCase[7]	 = { 0, 0, 0, 0, 0, 0, 0 };
	double dMu = 0;
	CDoubleArray dArrRes;

	// seek max value
	dUnitValue[0] = dBefore + dAfter;
	dUnitValue[2] = dWind_Max;
	dUnitValue[3] = dCrash_Max;
	dUnitValue[4] = dCF_Max;
	int i = 0;
	for (i=0; i<7; i++)
	{
		dUnitFac[0] = (i<3)? 1.30 : 1.20;
		dUnitFac[1] = (i==0)? 2.15 : (i==3||i==4)? 0.00 : 1.30;
		dUnitFac[2] = (i==2)? 0.65 : (i==3)? 1.20 : (i==4)? 1.30 : 0.00;
		dUnitFac[3] = (i==1)? 1.30 : (i==3)? 1.20 : 0.00;
		dUnitFac[4] = (i<3)? 1.30 : 0.00;

		dUnitValue[1] = (i<5)? dLive_Max : dTank;

		dResCase[i] = 0;
		for (int j=0; j<5; j++)
		{
			dResCase[i] += ( dUnitFac[j] * dUnitValue[j] );
		}
		// Max Value
		dArrRes.Add(fabs(dResCase[i]));
	}

	// seek min value
	dUnitValue[0] = dBefore + dAfter;
	dUnitValue[2] = dWind_Min;
	dUnitValue[3] = dCrash_Min;
	dUnitValue[4] = dCF_Min;

	for (i=0; i<7; i++)
	{
		dUnitFac[0] = (i<3)? 1.30 : 1.20;
		dUnitFac[1] = (i==0)? 2.15 : (i==3||i==4)? 0.00 : 1.30;
		dUnitFac[2] = (i==2)? 0.65 : (i==3)? 1.20 : (i==4)? 1.30 : 0.00;
		dUnitFac[3] = (i==1)? 1.30 : (i==3)? 1.20 : 0.00;
		dUnitFac[4] = (i<3)? 1.30 : 0.00;

		dUnitValue[1] = (i<5)? dLive_Min : 0;

		dResCase[i] = 0;
		for (int j=0; j<5; j++)
		{
			dResCase[i] += dUnitFac[j] * dUnitValue[j];
		}
		// Min Value
		dArrRes.Add(fabs(dResCase[i]));
	}
	dMu = dArrRes.Max();

	//-------------------------------------------------------------------------------------------------
	// ��±� ö�ٷ� ����

	// seek calculate length
	double dLength  = 0;
	long   nGirSu	= pDB->GetGirdersu();
	double dLeftX   = pDB->GetSlabLeft()->m_dGirderCenter;
	double dRightX  = pDB->GetSlabRight()->m_dGirderCenter;
	double dLen = pDB->GetGirder(0)->m_dGirderCenter - dLeftX;
	CDoubleArray	dSpaceArr;
	dSpaceArr.Add(dLen);

	for (long nG=0; nG<nGirSu-1; nG++)
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
	double dCar			= 300;
	double dFlange		= 0;
	double dWeb			= 0;
	double dGurdWidth	= 0;
	double dSpace		= 0;
	SeekFlange_Web(dFlange, dWeb);

	switch (nSide)
	{
	case 0: // left
		{
			if (pGuardL)	dGurdWidth = pGuardL->GetWidth() + pGuardL->m_D1;
			else			dGurdWidth = 0;

			dSpace	= dSpaceArr.GetAt(0);
			dLength = toM((dSpace-dGurdWidth) - dCar  - dWeb/2 - (dFlange/4 - dWeb/4));
		}
		break;
	case 1:	// right
		{
			if (pGuardR)	dGurdWidth = pGuardR->GetWidth() + pGuardR->m_D1;
			else			dGurdWidth = 0;

			dSpace  = dSpaceArr.GetAt(dSpaceArr.GetSize()-1);
			dLength = toM((dSpace-dGurdWidth) - dCar  - dWeb/2 + (dFlange/4 - dWeb/4));
		}
		break;
	case 2: // middle
		{
			for (long nG=1; nG<dSpaceArr.GetSize()-1; nG++)
			{
				double dDis = dSpaceArr.GetAt(nG);
				dSpace = (dSpace<dDis)? dDis : dSpace;
			}
			if (nGirSu!=2) dSpace -= dWeb - (dFlange-dWeb)/2;
			dLength = toM(dSpace);
		}
		break;
	}

	// ��±� ö�ٷ� - �Ϲݺ�
	double dBar_Sup_Persent = (120 / sqrt(dLength) )/100;						// 1�� ö�ٺ�
	double dBar_Sup_Ratio	= (dBar_Sup_Persent>0.67)? 0.67 : dBar_Sup_Persent;	// �䱸 ö�ٺ�
	double dBar_Sup_Area_Req= dBar_Area* dBar_Sup_Ratio;						// �䱸ö�ٷ�
	double dBar_Sup_Area_Min= 0.002 * 1000 * dSlab_Th;							// �ּ�ö�ٷ�
	double dBar_Sup_Dia1	= GetRebar(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo3);
	double dBar_Sup_Spa1	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB;
	double dBar_Sup_Are1	= GetArea(dBar_Sup_Dia1) * 1000 / dBar_Sup_Spa1;
	double dBar_Sup_Area1	= dBar_Sup_Are1 * 2;
	double dBar_Sup_Area_Req2= max(dBar_Area, dBar_Sup_Area_Min);

	// ��±� ö�ٷ� - �ܺ�
	double dBar_Sup_Area_Req3 = max(dBar_Sup_Area_Req, dBar_Sup_Area_Min);
	double dBar_Sup_Area_Req4 = dBar_Sup_Area_Req3 * 2;
	double dBar_Sup_Dia2	= GetRebar(pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_Combo4);
	double dBar_Sup_Spa2	= pCalcData->CALC_CANTILEVER_DATA[nSide].m_Rein_CtcB1;
	double dBar_Sup_Are2	= GetArea(dBar_Sup_Dia2) * 1000 / dBar_Sup_Spa2;
	double dBar_Sup_Area2	= dBar_Sup_Are2 * 2;

	//-------------------------------------------------------------------------------------------------
	// ��� ������
	Map["Bar_Dia1"] = dBar_Dia1;
	Map["Bar_Spa1"] = dBar_Spa1;
	Map["Bar_Are1"] = dBar_Are1;
	Map["Bar_Dia2"] = dBar_Dia2;
	Map["Bar_Spa2"] = dBar_Spa2;
	Map["Bar_Are2"] = dBar_Are2;
	Map["Bar_Area"] = dBar_Area;
	Map["Fps"]		= dFps;
	Map["Mn1"]		= dPiMn1;
	Map["Mn2"]		= dPiMn2;
	Map["Qp"]		= dQp;
	Map["Beta"]		= dBeta;
	Map["Mcr"]		= dMcr;
	Map["Mu"]		= dMu;

	Map["Length"]	= dLength;
	Map["Slab_Th"]  = dSlab_Th;
	Map["Sup_Rat1"] = dBar_Sup_Persent;
	Map["Sup_Rat2"] = dBar_Sup_Ratio;
	Map["Sup_Req1"] = dBar_Sup_Area_Req;
	Map["Sup_Req2"] = dBar_Sup_Area_Req2;
	Map["Sup_Req3"] = dBar_Sup_Area_Req3;
	Map["Sup_Req4"] = dBar_Sup_Area_Req4;
	Map["Sup_Min"]	= dBar_Sup_Area_Min;
	Map["Sup_Dia1"] = dBar_Sup_Dia1;
	Map["Sup_Spa1"] = dBar_Sup_Spa1;
	Map["Sup_Are1"] = dBar_Sup_Are1;
	Map["Sup_Area1"]= dBar_Sup_Area1;
	Map["Sup_Dia2"] = dBar_Sup_Dia2;
	Map["Sup_Spa2"] = dBar_Sup_Spa2;
	Map["Sup_Are2"] = dBar_Sup_Are2;
	Map["Sup_Area2"]= dBar_Sup_Area2;
}

// ���� ������ ����� ���� (2009.07.27)
void CCalcFloor::CalcCantileverInnerPositionCheck(CMap<CString, LPCTSTR, double, double> &Map)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();

	// 1. ö�ٷ� ����
	double dSlab_Thk = pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;
	double dReq_As = 10 * dSlab_Thk * GetFloorConcAreaCoefficient();
	double dBar_Dia1 = GetRebar(pCalcData->m_ComboInnerJijumSlab1);
	double dBar_Spa1 = pCalcData->m_Ctc1;
	double dBar_Are1 = GetArea(dBar_Dia1) * 1000 / dBar_Spa1;
	double dBar_Dia2 = GetRebar(pCalcData->m_ComboInnerJijumSlab2);
	double dBar_Spa2 = pCalcData->m_Ctc2;
	double dBar_Are2 = GetArea(dBar_Dia2) * 1000 / dBar_Spa2;
	double dBar_Area = dBar_Are1 + dBar_Are2;

	// 2. ����� ����
	double dBar_Use1 = 1000 / dBar_Spa1;
	double dBar_Use2 = 1000 / dBar_Spa2;
	double dBar_n	 = dBar_Use1 + dBar_Use2;
	double dBar_Dia  = GetDia(dBar_Dia1);
//	double dUr		 = ConstPi * dBar_Dia * dBar_n / ( 1000 * dSlab_Thk );
	double dUr		= (ConstPi * GetDia(dBar_Dia1) * dBar_Use1 + ConstPi * GetDia(dBar_Dia2) * dBar_Use2) / ( 1000 * dSlab_Thk );

	//-------------------------------------------------------------------------------------------------
	// ��� ������
	Map["Slab_Thk"] = dSlab_Thk;
	Map["Req_As"]	= dReq_As;
	Map["Bar_Dia1"] = dBar_Dia1;
	Map["Bar_Spa1"] = dBar_Spa1;
	Map["Bar_Are1"] = dBar_Are1;
	Map["Bar_Dia2"] = dBar_Dia2;
	Map["Bar_Spa2"] = dBar_Spa2;
	Map["Bar_Are2"] = dBar_Are2;
	Map["Bar_Area"] = dBar_Area;
	Map["Bar_Use1"] = dBar_Use1;
	Map["Bar_Use2"] = dBar_Use2;
	Map["Bar_n"]    = dBar_n;
	Map["Bar_Dia"]  = dBar_Dia;
	Map["Ur"]		= dUr;
}

// ������ ���� ���� (2009.07.27)
void CCalcFloor::CalcCantileverFixedCheck(CMap<CString, LPCTSTR, double, double> &Map)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();

	// ������ �Է� ����
	double dFix_Qty		= 4;
	double dFix_Width	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageWidth;
	double dFix_Height	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight;
	double dFix_Space	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	double dEff_Space	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	double dSlab_Thk	= (pDB->m_nTendonFix<1)? pDB->m_dThickSlabRight : pDB->m_dThickSlabLeft;
	CString	szFp		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sJackMax;
	double dTdn_Str		= pCalcData->GetJackMaxStrToDouble(szFp);
	double dTdn_Area	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea * pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	double dTdn_Pu		= dTdn_Str * dTdn_Area * pow(10.0,-3);


	// �� Ⱦ���� �Ŀ��¿� ���� ���� ö�ٷ� ����
	double dAH			= dFix_Width;
	double dSH			= dFix_Space;
	double dT2H			= 0.2 * dTdn_Pu * (1 - dAH/dSH);
	double dFsa			= pCalcData->DESIGN_MATERIAL.m_dSigmaSA;
	double dReq_AsT2H	= dT2H * pow(10.0, 3) / dFsa;
	double dDia_T2H		= GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2);
	double dQty_T2H		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2;
	double dUse_AsT2H	= GetArea(dDia_T2H) * dQty_T2H;

	// �� �������� �Ŀ��¿� ���� ����ö�ٷ� ����
	double dAV			= dFix_Height;
	double dSV			= dSlab_Thk;
	double dT2V			= 0.2 * dTdn_Pu * (1 - dAV/dSV);
	double dReq_AsT2V	= dT2V * pow(10.0, 3) / dFsa;
	double dDia_T2V		= GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerRein2);
	double dQty_T2V		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty2;
	double dUse_AsT2V	= GetArea(dDia_T2V) * dQty_T2V;

	// �� �ܺ� ����¿� ���� ����ö�ٷ� ����
	double dT1			= 0.1 * dTdn_Pu * (1 - dAH/dSH);
	double dReq_AsT1	= dT1 * pow(10.0, 3) / dFsa;
	double dDia_T1		= GetRebar(pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein1);
	double dQty_T1		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1;
	double dUse_AsT1	= GetArea(dDia_T1) * dQty_T1;

	// �� ������ �������� ����
	double dFb			= dTdn_Str * dTdn_Area / (dFix_Height*dFix_Width);
	double dFct			= pCalcData->DESIGN_MATERIAL.m_dSigmaCi;
	double dAo			= dSlab_Thk * dEff_Space;
	double dAg			= dFix_Width * dFix_Height;
	double dFba			= 0.7 * 0.8 * dFct * sqrt(dAo/dAg);

	//-------------------------------------------------------------------------------------------------
	// ��� ������
	Map["Fix_Qty"]		= dFix_Qty;
	Map["Fix_Width"]	= dFix_Width;
	Map["Fix_Height"]	= dFix_Height;
	Map["Fix_Space"]	= dFix_Space;
	Map["Eff_Space"]	= dEff_Space;
	Map["Slab_Thk"]		= dSlab_Thk;
	Map["Tdn_Str"]		= dTdn_Str;
	Map["Tdn_Area"]		= dTdn_Area;
	Map["Tdn_Pu"]		= dTdn_Pu;

	Map["dAH"]			= dAH;
	Map["dSH"]			= dSH;
	Map["T2H"]			= dT2H;
	Map["Fsa"]			= dFsa;
	Map["Req_AsT2H"]	= dReq_AsT2H;
	Map["Dia_T2H"]		= dDia_T2H;
	Map["Qty_T2H"]		= dQty_T2H;
	Map["Use_AsT2H"]	= dUse_AsT2H;

	Map["dAV"]			= dAV;
	Map["dSV"]			= dSV;
	Map["T2V"]			= dT2V;
	Map["Req_AsT2V"]	= dReq_AsT2V;
	Map["Dia_T2V"]		= dDia_T2V;
	Map["Qty_T2V"]		= dQty_T2V;
	Map["Use_AsT2V"]	= dUse_AsT2V;

	Map["T1"]			= dT1;
	Map["Req_AsT1"]		= dReq_AsT1;
	Map["Dia_T1"]		= dDia_T1;
	Map["Qty_T1"]		= dQty_T1;
	Map["Use_AsT1"]		= dUse_AsT1;

	Map["Fb"]			= dFb;
	Map["Fct"]			= dFct;
	Map["Ao"]			= dAo;
	Map["Ag"]			= dAg;
	Map["Fba"]			= dFba;
}


// ����ĵƿ������(����������) ���� 3.6.1.4
void CCalcFloor::DesignExpansionJointUltimateLoad(CMap<CString, LPCTSTR, double, double> &Map)
{
	CPlateBridgeApp		*pDB		= m_pDataManage->GetBridge();
	CGlobarOption		*pGlobalOpt	= m_pDataManage->GetGlobalOption();
	CCalcData			*pCalcData	= m_pDataManage->GetCalcData();
	CPlateGirderApp		*pGir		= pDB->GetGirder(0);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBx		= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateCrossBeam		*pCr		= pBx->GetCrossBeam();

	double	dWidthExp		= pGlobalOpt->GetSttExpansionJointWidth();
	double	dHeightExp		= pGlobalOpt->GetSttExpansionJointHeight();
	double	dLengthGirder	= pDB->m_dLengthUganStt+pDB->m_dLengthSlabGirderStt;
	double	dLengthShoe		= dLengthGirder+pDB->m_dLengthGirderShoeStt;
	double	dLengthTaper	= dLengthShoe - pDB->m_dLengthUganStt;
	double	dLengthArm		= dLengthTaper-(pCr->m_T3_CR+pCr->m_uM_CR)/2.0;
//	double	dThickSlab		= pGir->GetThickSlab(pBx, 0);//pDB->GetSlabThick(CPlateBridgeApp::SLAB_LEFT);
	double	dThickSlab		= pDB->m_dThickSlabBase+pDB->m_dHeightTaperStt;
	double	dUnitConc		= pCalcData->DESIGN_UNIT_WEIGHT.m_dChulgun;
	double	dDeadLoad		= dLengthArm * dThickSlab * dUnitConc;
	double	dMd				= dDeadLoad * dLengthArm/2.0;
	double	dLengthMove		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Move_L;
	double	dCollCoeff		= 15/(40+toM(dLengthMove));
	if(dCollCoeff>0.3)	dCollCoeff = 0.3;
	double	dE				= frM(0.35*toM(dLengthArm-100.0) + 0.98);
	double	dPr				= pCalcData->DESIGN_CONDITION.m_dPr;
	double	dMl				= dPr*(dLengthArm-100.0)*(1+dCollCoeff)/dE;
	double	dMu				= 1.3 * dMd + 2.15 * dMl;
	double	dFck			= pCalcData->DESIGN_MATERIAL.m_dSigmaCK;
	double	dFy				= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dWidth			= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_b;
	double	dCover			= pCalcData->m_dCoverUp;
	double	dD				= dThickSlab-dCover-pDB->m_dThickPave;
	double	dPib			= pCalcData->m_Pib;
	double	dPis			= pCalcData->m_Pis;
	double dK1 = 0;
	if(dFck>28)	dK1 = 0.85-(dFck-28)*0.007;
	else		dK1 = 0.85;
	double dA = dFy * dFy/(dK1 * dFck * dWidth*2);	//2��������(AX��+BX+C=0) ���� ���� �̿�
	double dB = -dK1 * dFy * dD;
	double dC = dMu/dK1*1000;
	double	dAsreq			= 0;
	// �ܺ� ��ö�� ���ö�ٷ�
	double	dDMainCTC		= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Ctc2;						// �ܺ� ��ö��
	long	nDanDiaIdx_1Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2;						// �ܺ� ��ö�� 1 Cycle
	long	nDanDiaIdx_2Cy	= pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_Combo2_2Cy;					// �ܺ� ��ö�� 2 Cycle
	double	dAsuse			= (_dRebarArea[nDanDiaIdx_1Cy]+_dRebarArea[nDanDiaIdx_2Cy]) * dWidth / dDMainCTC;	// �ܺ� ��ö�� ��� ö�ٷ�
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_1Cy	= _dRebarArea[nDanDiaIdx_1Cy] * dWidth / dDMainCTC;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2_2Cy	= _dRebarArea[nDanDiaIdx_2Cy] * dWidth / dDMainCTC;
	pCalcData->CALC_CANTILEVER_DATA[FLOOR_MID].m_Rein_UseAs2		= dAsuse;
	// �ܺ� ��ö�� �ʿ�ö�ٷ�
	if(dB * dB - 4 * dA * dC > 0)
	{
		double dX1 = (-dB + sqrt(dB * dB - 4 * dA * dC)) / (2 * dA);
		double dX2 = (-dB - sqrt(dB * dB - 4 * dA * dC)) / (2 * dA);
		if(dX1 > dX2 && dX2 > 0)	dAsreq = dX2;
		else						dAsreq = dX1;
	}
	else	dAsreq = 0;
	// ö�ٺ� ����
	double	dAsmin1		= dPis*sqrt(dFck)*dWidth*dD/dFy;
	double	dAsmin2		= 14.0*dWidth*dD/dFy;
	double	dAsmin3		= dAsreq*4.0/3.0;
	double	dAsminuse	= MIN(MIN(dAsmin1, dAsmin2), dAsmin3);
	dK1			= 0.85;
	double	dPmax		= 0.75*0.85*dK1*dFck/dFy*600.0/(600.0+dFy);
	double	dAsmax		= dPmax*dWidth*dD;
	// �ڿ����� ����
	double	dA1			= dAsuse*dFy/(0.85*dFck*dWidth);
	double	dMn			= dPib*0.85*dAsuse*dFy*(dD-dA1/2);

	Map["WidthExp"]		= dWidthExp;
	Map["HeightExp"]	= dHeightExp;
	Map["ThickSlab"]	= dThickSlab;
	Map["DeadLoad"]		= dDeadLoad;
	Map["Md"]			= dMd;
	Map["LengthArm"]	= dLengthArm;
	Map["CollCoeff"]	= dCollCoeff;
	Map["E"]			= dE;
	Map["Pr"]			= dPr;
	Map["Ml"]			= dMl;
	Map["Mu"]			= dMu;
	Map["B"]			= dWidth;
	Map["D"]			= dD;
	Map["Cover"]		= dCover;
	Map["Asreq"]		= dAsreq;
	Map["Asmin1"]		= dAsmin1;
	Map["Asmin2"]		= dAsmin2;
	Map["Asmin3"]		= dAsmin3;
	Map["Asmin"]		= dAsminuse;
	Map["Pmax"]			= dPmax;
	Map["Asmax"]		= dAsmax;
	Map["A"]			= dA1;
	Map["Mn"]			= dMn;
}

void CCalcFloor::DesignFixingPlate(CMap<CString, LPCTSTR, double, double> &Map)
{
	/*
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CCalcData		*pCalcData	= m_pDataManage->GetCalcData();

	double	dFy				= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double	dSlabThick		= pDB->GetSlabThick(CPlateBridgeApp::SLAB_LEFT);
	double	dFixingLength	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dFixingPlateLength;
	double	dPSUpperLoad	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dTensionLoad;			
	double	dVerPj			= 0.7*dPSUpperLoad;
	double	dVerZ			= 0.3 * dVerPj * (1 - dFixingLength / dSlabThick);
	double	dVerAsreq		= dVerZ*1000/(0.6*dFy);
	double	dHorPj			= 0.7*dPSUpperLoad;
	double	dL				= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPrestressCTC;
	double	dHorT1			= 0.1 * dHorPj;
	double	dHorT2			= 0.2 * dHorPj;
	double	dHorAs1req		= dHorT1*1000/(0.6*dFy);
	double	dHorAs2req		= dHorT2*1000/(0.6*dFy);
	long	nVerRein		= pData->DESIGN_STEELPRESTRESS_DATA.m_nVerRein;
	long	nHorRein1		= pData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein1;
	long	nHorRein2		= pData->DESIGN_STEELPRESTRESS_DATA.m_nHorRein2;
	long	nQtyVerRein		= pData->DESIGN_STEELPRESTRESS_DATA.m_nVerReinQty;
	long	nQtyHorRein1	= pData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1;
	long	nQtyHorRein2	= pData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2;
	pData->DESIGN_STEELPRESTRESS_DATA.m_dVerReinAs		= nQtyVerRein * _dRebarArea[nVerRein];
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs1	= nQtyHorRein1 * _dRebarArea[nHorRein1];
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs2	= nQtyHorRein2 * _dRebarArea[nHorRein2];

	Map["ThickSlab"]	= dSlabThick;
	Map["LengthFixing"]	= dFixingLength;
	*/
}
//������ ���� ����...
void CCalcFloor::SetAnchorageRebar()
{
	CPlateBridgeApp	*pDB		= m_pDataManage->GetBridge();
	CCalcData		*pCalcData	= m_pDataManage->GetCalcData();	
	
	double dFy		= pCalcData->DESIGN_MATERIAL.m_dSigmaY;
	double dFpy		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dArea;
	long dNt		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nPrestressQty;
	double dAt		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dYieldStrength;
	double dB		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dAnchorageHeight;
	double dH		= pDB->m_dThickSlabBase - pDB->m_dThickSlabHunch;		///< �߾Ӻ� ������ �β�
	CString sCaseH1 = pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH1;
	CString sCaseH2	= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_sCaseH2;
	long dQty1		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty1;
	long dQty2		= pCalcData->DESIGN_STEELPRESTRESS_DATA.m_nHorReinQty2;

	long nDia1		= atol((char*)(LPCTSTR)sCaseH1);
	long nDia2		= atol((char*)(LPCTSTR)sCaseH2);	

	double dPj		= 0.9*dFpy*dNt*dAt;//N
	double dT1		= 0.04*dPj; //N
	double dAs1		= dT1/(0.6*dFy);
	double dT2		= 0.3*dPj*(1-(dB/1000)/(dH/1000));
	double dAs2		= dT2 / (0.6*dFy);
	double dReqAs1	= dAs1;
	double dUseAs1	= AreaReturn(nDia1)*2 * dQty1;

	double dReqAs2	= dAs2 + dReqAs1;
	double dUseAs2	= AreaReturn(nDia2)*2 * dQty2;

	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dPj = dPj;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dSpallingForce = dT1;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs1 = dAs1;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dBurstingForce = dT2;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dHorReinAs2 = dAs2;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dReqAs1 = dReqAs1;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dUseAs1 = dUseAs1;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dReqAs2 = dReqAs2;
	pCalcData->DESIGN_STEELPRESTRESS_DATA.m_dUseAs2 = dUseAs2;

}

double CCalcFloor::AreaReturn(long AsNum)
{

	for(int n =0; n < sizeof(_dRebarDia)/sizeof(double); n++)
	{
		if (_dRebarDia[n] == AsNum)
			return _dRebarArea[n];			
	}

	return 0;
}

void CCalcFloor::GetAppConcreteEC(long nTypeHDan)
{
	CCalcData		*pData		= m_pDataManage->GetCalcData();	
	CSteelUWeight	*pUWeight	= m_pDataManage->GetSteelUWeight();

	if (nTypeHDan == 1)// ��ȣ���̸�..
	{
		if(pData->m_bMugun)
			m_dAppConcreteEC	= pUWeight->m_dUWeightMugunCon;
		else
			m_dAppConcreteEC	= pUWeight->m_dUWeightChulgunCon;
	}
	else if (nTypeHDan == 2)// �ߺд��̸�...
	{
		if(pData->m_bJungbunMugun)
			m_dAppConcreteEC	= pUWeight->m_dUWeightMugunCon;
		else
			m_dAppConcreteEC	= pUWeight->m_dUWeightChulgunCon;
	}
	else
		m_dAppConcreteEC	= pUWeight->m_dUWeightChulgunCon;

}

double CCalcFloor::GetFloorConcAreaCoefficient()
{
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();

	long	nDesignStd = pOpt->GetSteelStandardYear();
	if(nDesignStd == APPLYYEAR2008)
		return 2.0;
	else if(nDesignStd == APPLYYEAR2010)
		return 1.5;

	return 0;
}

double CCalcFloor::GetFloorJujangStd()
{
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();

	long	nDesignStd = pOpt->GetSteelStandardYear();
	if(nDesignStd == APPLYYEAR2008)
		return 0.0045;
	else if(nDesignStd == APPLYYEAR2010)
		return 0.0035;

	return 0;
}


CConcBasic * CCalcFloor::MakeConcBasicData( FloorType eType, long nPart )
{
	CPlateBridgeApp	*pDB = m_pDataManage->GetBridge();
	CCalcGeneral CalcGeneral(m_pDataManage);
	CCalcData *pData = m_pDataManage->GetCalcData();
	CGlobarOption *pOpt = m_pDataManage->GetGlobalOption();

	CString strTitle = _T("");

	BOOL bExperience = (eType == eFloor_CenterExperience) ? TRUE : FALSE;
	BOOL bPrecastRebar = FALSE;
	BOOL bUpper = FALSE;

	double dSlabThick = 0.0;
	double dCoverT = 0.0;	// ����� �Ǻ�
	double dCoverC = 0.0;	// ����� �Ǻ�

	long nPos = 0;
	switch(eType)
	{
	case eFloor_LeftCantilever:
		nPos = FLOOR_LEFT;
		bUpper = TRUE;
		dCoverT = pData->m_dCoverUp;
		dCoverC = pData->m_dCoverDn;
		dSlabThick = pDB->m_dThickSlabBase;

		strTitle.Format(_T("���� ĵƿ������"));
		break;

	case eFloor_RightCantilever:
		nPos = FLOOR_RIGHT;
		bUpper = TRUE;
		dCoverT = pData->m_dCoverUp;
		dCoverC = pData->m_dCoverDn;
		dSlabThick = pDB->m_dThickSlabBase;

		strTitle.Format(_T("���� ĵƿ������"));
		break;

	case eFloor_Center:
	case eFloor_CenterExperience:
		nPos = FLOOR_MID;
		bUpper = FALSE;
		dCoverT = pData->m_dCoverDn;
		dCoverC = pData->m_dCoverUp;
		dSlabThick = pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;
		strTitle.Format(_T("�߾Ӻ�"));

		if(pOpt->GetSlabCentDesignMethod() == 2)
		{
			strTitle += _T("(LB-Deck)");
			bPrecastRebar = TRUE;
		}
		break;

	case eFloor_MiddleJijum:
		nPos = FLOOR_MID;
		bUpper = FALSE;
		dCoverT = pData->m_dCoverDn;
		dCoverC = pData->m_dCoverUp;
		dSlabThick = pDB->m_dThickSlabBase-pDB->m_dThickSlabHunch;

		strTitle.Format(_T("�߰�������"));
		break;
	}

	//	CMap<CString, LPCSTR, double, double> MapRebar;

	CConcBasic *pConc = new CConcBasic;
	SetConcBasicDataDesign(pConc);		// �������� �� ����

	CMap<CString, LPCTSTR, double, double>MapCrack;
	SetCantileverCrack(MapCrack, nPos);

	double dBo = 1000;
	double dMoment = tokNM(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Mu);
	double dMomentUse = tokNM(MapCrack["CRACK_MMAX"]);

 	pConc->m_sTitle = strTitle;	
 	pConc->m_Moment = GetValueUnitChange(dMoment, _T("kN.m"), _T("tonf.m")); 
 	pConc->m_Shear;
 	pConc->m_MomentUse = GetValueUnitChange(dMomentUse, _T("kN.m"), _T("tonf.m"));

 	pConc->m_Bo = pData->CALC_CANTILEVER_DATA[nPos].m_Rein_b;	// [�ܸ��� ���] �� (mm)
 	pConc->m_Ho = dSlabThick;									// [�ܸ��� ���] �β� (mm)
 	pConc->m_LnSpan = pData->CALC_CANTILEVER_DATA[nPos].m_Dead_L;

	if(bExperience)
	{
		CPlateGirderApp		*pGirFir	= pDB->GetGirder(0);
		CPlateBxFinder		FinderStt(pGirFir);
		CPlateBasicIndex	*pBxFir		= FinderStt.GetBxFirst(BX_CROSSBEAM_VBRACING);
		CPlateGirderApp		*pGirEnd	= pDB->GetGirder(pDB->GetGirdersu()-1);
		CPlateBxFinder		FinderEnd(pGirEnd);
		CPlateBasicIndex	*pBxEnd		= FinderEnd.GetBxFirst(BX_CROSSBEAM_VBRACING);

		pConc->m_LnSpan = CalcGeneral.CalcCentPart_L();	
		pConc->m_LnSpanLeft	 = pDB->m_dWidthSlabLeft - pGirFir->GetThickJewon(G_W, pBxFir)/2;
		pConc->m_LnSpanRight = pDB->m_dWidthSlabRight - pGirEnd->GetThickJewon(G_W, pBxEnd)/2;

		CCentSeparation	*pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::LEFT);
		if(pCentSeparation)		pConc->m_bLeftGuard = TRUE;
		else					pConc->m_bLeftGuard = FALSE;

		pCentSeparation = pDB->GetCurGuardWall(CCentSeparation::RIGHT);
		if(pCentSeparation)		pConc->m_bRightGuard = TRUE;
		else					pConc->m_bRightGuard = FALSE;
	}

	if(eType == eFloor_Center && nPart == REBAR_PART_END)
	{
		switch(pData->m_nEndPartRebarConditionIdx)
		{
		case 0: 
			pConc->m_dEndPartCondition = 1.0;
			break;

		case 1:
			pConc->m_dEndPartCondition = 2.0;
			break;
		}
	}
	else
	{
		pConc->m_dEndPartCondition = 2.0;
	}

	if(eType == eFloor_Center || eType == eFloor_CenterExperience)	// #APLATE-657
	{
		if(pDB->GetGirdersu() <= 2)
			pConc->m_dDeflectionCoefficient = 20;
		else
			pConc->m_dDeflectionCoefficient = 28;
	}

	// ��ö�� ����
	double	dDia_Main1st_T	= GetRebarDia(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_1, bUpper);
	double	dDia_Main2nd_T	= GetRebarDia(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_2, bUpper);
	double	dCTC_Main1st_T	= GetRebarCTC(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_1, bUpper);
	double	dCTC_Main2st_T	= GetRebarCTC(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_2, bUpper);
	double	dQty_Main1st_T	= dCTC_Main1st_T == 0 ? 0 : dBo / dCTC_Main1st_T;
	double	dQty_Main2nd_T	= dCTC_Main2st_T == 0 ? 0 : dBo / dCTC_Main2st_T;

	// ��ö�� ����
	double	dDia_Main1st_C	= bExperience ? GetRebarDia(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_1, TRUE) : 0;
	double	dDia_Main2nd_C	= bExperience ? GetRebarDia(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_2, TRUE) : 0;
	double	dCTC_Main1st_C	= bExperience ? GetRebarCTC(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_1, TRUE) : 0;
	double	dCTC_Main2st_C	= bExperience ? GetRebarCTC(nPos, nPart, REBAR_TYPE_MAIN, REBAR_CYCLE_2, TRUE) : 0;
	double	dQty_Main1st_C	= bExperience || dCTC_Main1st_C == 0 ? dBo / dCTC_Main1st_C : 0;
	double	dQty_Main2nd_C	= bExperience || dCTC_Main2st_C == 0 ? dBo / dCTC_Main2st_C : 0;

	// ��� ���� 
	double	dDia_Support_T	= GetRebarDia(nPos, nPart, REBAR_TYPE_SUPPORT, REBAR_CYCLE_1, bUpper);
	double	dCTC_Support_T	= GetRebarCTC(nPos, nPart, REBAR_TYPE_SUPPORT, REBAR_CYCLE_2, bUpper);

	// ��� ���� 
	double	dDia_Support_C	= 0;
	double	dCTC_Support_C	= 0;	
	switch(eType)
	{
	case eFloor_CenterExperience :
		dDia_Support_C = GetRebarDia(nPos, nPart, REBAR_TYPE_SUPPORT, REBAR_CYCLE_1, TRUE);
		dCTC_Support_C = GetRebarCTC(nPos, nPart, REBAR_TYPE_SUPPORT, REBAR_CYCLE_2, TRUE);
		break;
	case eFloor_MiddleJijum:
		dDia_Support_C	= GetRebarDia(nPos, nPart, REBAR_TYPE_SUPPORT, REBAR_CYCLE_1, TRUE);
		dCTC_Support_C	= GetRebarCTC(nPos, nPart, REBAR_TYPE_SUPPORT, REBAR_CYCLE_1, TRUE);
		break;
	default:
		dDia_Support_C = 0;
		dCTC_Support_C = 0;
	}

	// ��ö�� ����
	pConc->m_RbT1C_DIA[0]	= dDia_Main1st_T;		// [1Cycle����ö�ٹ�ġ] ö�� ���� (mm)
	pConc->m_RbT1C_dc[0]	= dCoverT;				// [1Cycle����ö�ٹ�ġ] ö�� �߽ɰŸ� (mm) - �Ǻ�
	pConc->m_RbT1C_B[0]		= dQty_Main1st_T;		// [1Cycle����ö�ٹ�ġ] ���ܹ������� ��ġ�� (���ݼ� or (���簢�����϶�)) ö�ٰ���
	pConc->m_RbT2C_DIA[0]	= dDia_Main2nd_T;		// [2Cycle����ö�ٹ�ġ] ö�� ���� (mm)
	pConc->m_RbT2C_dc[0]	= dCoverT;				// [2Cycle����ö�ٹ�ġ] ö�� �߽ɰŸ� (mm) - �Ǻ�
	pConc->m_RbT2C_B[0]		= dQty_Main2nd_T;		// [1Cycle����ö�ٹ�ġ] ���ܹ������� ��ġ�� (���ݼ� or (���簢�����϶�)) ö�ٰ���

	// ��ö�� ����
	pConc->m_RbC1C_DIA[0]	= dDia_Main1st_C;		// [1Cycle����ö�ٹ�ġ] ö�� ���� (mm)
	pConc->m_RbC1C_dc[0]	= dCoverC;				// [1Cycle����ö�ٹ�ġ] ö�� �߽ɰŸ� (mm) - �Ǻ�
	pConc->m_RbC1C_B[0]		= dQty_Main1st_C;		// [1Cycle����ö�ٹ�ġ] ���ܹ������� ��ġ�� (���ݼ� or (���簢�����϶�)) ö�ٰ���
	pConc->m_RbC2C_DIA[0]	= dDia_Main2nd_C;		// [2Cycle����ö�ٹ�ġ] ö�� ���� (mm)
	pConc->m_RbC2C_dc[0]	= dCoverC;				// [2Cycle����ö�ٹ�ġ] ö�� �߽ɰŸ� (mm) - �Ǻ�
	pConc->m_RbC2C_B[0]		= dQty_Main2nd_C;		// [1Cycle����ö�ٹ�ġ] ���ܹ������� ��ġ�� (���ݼ� or (���簢�����϶�)) ö�ٰ���

	if(bPrecastRebar)
	{
		pConc->m_RbT1C_DIA[1]	= GetRebar(pData->m_nLBDeckDiaIdx);	// [LBDeck ö�ٹ�ġ] ö�� ���� (mm)
		pConc->m_RbT1C_dc[1]	= pData->m_dLBDeckCover;			// [LBDeck ö�ٹ�ġ] ö�� �߽ɰŸ� (mm) - �Ǻ�
		pConc->m_RbT1C_B[1]		= dBo/pData->m_dLBDeckCTC;			// [LBDeck ö�ٹ�ġ] ���ܹ������� ��ġ�� (���ݼ� or (���簢�����϶�)) ö�ٰ���
	}
	else
	{
		pConc->m_RbT1C_DIA[1]	= 0;
		pConc->m_RbT1C_dc[1]	= 0;
		pConc->m_RbT1C_B[1]		= 0;
	}

	// ��� ���� 
	pConc->m_CSupportBarDia[0] = dDia_Support_C;	// [���ö��] ö�� ���� (mm)
	pConc->m_CSupportBarCTC[0] = dCTC_Support_C;	// [���ö��] ö�� ���� (mm)
	// ��� ���� 
	pConc->m_TSupportBarDia[0] = dDia_Support_T;	// [���ö��] ö�� ���� (mm)
	pConc->m_TSupportBarCTC[0] = dCTC_Support_T;	// [���ö��] ö�� ���� (mm)

	return pConc;
}

void CCalcFloor::SetConcBasicDataDesign( CConcBasic *pConc )
{
	CCalcData *pData = m_pDataManage->GetCalcData();

	pConc->m_nGirderType = GIRDERTYPE_PLATE;
	pConc->m_eStructureType = CConcBasic::eStructure_Upper;	// ������ Ÿ�� : ���
	pConc->m_nTypeSection = SECTION_SINGLE;					// ��ö�ٺ�
	pConc->m_nTypeUnit = 1;									// ������ (0:CGS, 1:SI)
	pConc->m_nTypeKciCode = m_pDataManage->GetGlobalOption()->GetSteelStandardYear() == APPLYYEAR2010 ? 1 : 0;	// ���뼳����� ((0:KCI2003 (���α�2008, 2004), 1:KCI2007 (���α�2010))

	pConc->m_Ec = GetValueUnitChange(pData->DESIGN_MATERIAL.m_dEc, _T("MPa"), _T("kgf/cm��"));				// ��ũ��Ʈ Ec
	pConc->m_Es = GetValueUnitChange(pData->DESIGN_MATERIAL.m_dEs, _T("MPa"), _T("kgf/cm��"));				// ���� ź�����
	pConc->m_Fck = GetValueUnitChange(pData->DESIGN_MATERIAL.m_dSigmaCK, _T("MPa"), _T("kgf/cm��"));
	pConc->m_Fy = GetValueUnitChange(pData->DESIGN_MATERIAL.m_dSigmaY, _T("MPa"), _T("kgf/cm��"));
	pConc->m_FyShear = pData->DESIGN_MATERIAL.m_dShearEs;
	pConc->m_Pi_C;											// [�������Ұ��] �����
	pConc->m_Pi_F = pData->m_Pib;							// [�������Ұ��] �ڸ��Ʈ
	pConc->m_Pi_V;											// [�������Ұ��] ���ܷ�
	pConc->m_bMinAvCheck = TRUE;
	pConc->m_bCheckCrack = TRUE;
	pConc->m_nEnvTypeCrack;
	pConc->m_nCheckSupportBarRate = SUPPORTBAR_CHECK_SLAB;
	pConc->m_bCheckSuppoerBarAreaRate = TRUE;
	pConc->m_dCheckSuppoerBarAreaRate = 0.002;				// ����ö���� �ܸ�����
	pConc->m_bCheckSupportBarMainRateSpan = TRUE;			// ����ö�� - ��ö���� ���� ������⿡ ������ ���
	pConc->SetStringUnitType();

	pConc->m_nReserved = 1;
}

void CCalcFloor::MakeCalculateOutputFile( FloorType eType )
{
	CConcBasic *pConcGen = MakeConcBasicData(eType, REBAR_PART_GENERAL);
	CConcBasic *pConcEnd = MakeConcBasicData(eType, REBAR_PART_END);
	CConcBasic *pConcMid = MakeConcBasicData(eType, REBAR_PART_MIDDLE);

	CStdioFile DesignFile;
	CStdioFile CrackFile;
	CStdioFile SupportFile;
	CStdioFile DeflectionFile;
	CStdioFile LengthRateFile;
	CStdioFile EndPartFile;
	CStdioFile ExperienceFile;
	CStdioFile ExperienceEndFile;
	CStdioFile ExperienceConditionFile;

	DesignFile.Open (GetFileNameBySubject(eFile_Design), CFile::modeCreate | CFile::modeWrite );
	CrackFile.Open (GetFileNameBySubject(eFile_Crack), CFile::modeCreate | CFile::modeWrite );
	SupportFile.Open (GetFileNameBySubject(eFile_Support), CFile::modeCreate | CFile::modeWrite );
	DeflectionFile.Open (GetFileNameBySubject(eFile_Deflection), CFile::modeCreate | CFile::modeWrite );
	LengthRateFile.Open (GetFileNameBySubject(eFile_LengthRate), CFile::modeCreate | CFile::modeWrite );
	EndPartFile.Open (GetFileNameBySubject(eFile_EndPart), CFile::modeCreate | CFile::modeWrite );
	ExperienceFile.Open (GetFileNameBySubject(eFile_Experience), CFile::modeCreate | CFile::modeWrite );
	ExperienceEndFile.Open (GetFileNameBySubject(eFile_ExperienceEnd), CFile::modeCreate | CFile::modeWrite );
	ExperienceConditionFile.Open (GetFileNameBySubject(eFile_ExperienceCondition), CFile::modeCreate | CFile::modeWrite );

	CRectBeam BeamGen(pConcGen);
	CRectBeam BeamEnd(pConcEnd);
	CRectBeam BeamMID(pConcMid);

	BeamGen.MakeOutputDesignTextFile(&DesignFile);
	BeamGen.MakeOutputSupportCheck(&SupportFile);
	BeamGen.MakeOutputCrackDesign(&CrackFile,TRUE,TRUE);
	BeamGen.MakeOutputExperienceDesignCheck(&ExperienceFile,TRUE,TRUE);
	BeamEnd.MakeOutputEndPartCheck(&EndPartFile);
	BeamEnd.MakeOutputDeflectionCheck(&DeflectionFile);
	BeamEnd.MakeOutputExperienceDesignEndPartCheck(&ExperienceEndFile);
	BeamMID.MakeOutputMainLengthRateCheck(&LengthRateFile);
	BeamGen.MakeOutputExperienceDesignCondition(&ExperienceConditionFile);
	ExperienceConditionFile.Close();

	DesignFile.Close();
	CrackFile.Close();
	SupportFile.Close();
	DeflectionFile.Close();
	LengthRateFile.Close();
	EndPartFile.Close();
	ExperienceFile.Close();
	ExperienceEndFile.Close();

	MakeOutputResultFile(eType);
}

void CCalcFloor::MakeOutputResultFile( FloorType eType )
{
	CCalcGeneral CalcGeneral(m_pDataManage);
	
	CStdioFile ResultFile;
	ResultFile.Open (GetFileNameResult(), CFile::modeCreate | CFile::modeWrite );

	CStdioFile DesignFile;
	CStdioFile CrackFile;
	CStdioFile SupportFile;
	CStdioFile DeflectionFile;
	CStdioFile LengthRateFile;
	CStdioFile EndPartFile;
	CStdioFile ExperienceFile;
	CStdioFile ExperienceEndFile;
	CStdioFile ExperienceConditionFile;

	DesignFile.Open (GetFileNameBySubject(eFile_Design), CFile::modeRead);
	CrackFile.Open (GetFileNameBySubject(eFile_Crack), CFile::modeRead);
	SupportFile.Open (GetFileNameBySubject(eFile_Support), CFile::modeRead);
	DeflectionFile.Open (GetFileNameBySubject(eFile_Deflection), CFile::modeRead);
	LengthRateFile.Open (GetFileNameBySubject(eFile_LengthRate), CFile::modeRead);
	EndPartFile.Open (GetFileNameBySubject(eFile_EndPart), CFile::modeRead);
	ExperienceFile.Open (GetFileNameBySubject(eFile_Experience), CFile::modeRead);
	ExperienceEndFile.Open (GetFileNameBySubject(eFile_ExperienceEnd), CFile::modeRead);
	ExperienceConditionFile.Open (GetFileNameBySubject(eFile_ExperienceCondition), CFile::modeRead);

	CStringArray arrStrReadFile;
	CString strTmp = _T("");

	switch(eType)
	{
	case eFloor_LeftCantilever:
	case eFloor_RightCantilever:
	case eFloor_Center:
		{
			while( DesignFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);

			while( SupportFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);

			while( EndPartFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);

			while( CrackFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);

			while( DeflectionFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);
		}
		break;
	case eFloor_CenterExperience:
		{
			while( ExperienceConditionFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);

			while( ExperienceFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);

			while( ExperienceEndFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);
		}
		break;
	case eFloor_MiddleJijum:
		{
			while( LengthRateFile.ReadString(strTmp) )
				arrStrReadFile.Add(strTmp);
		}
		break;
	}

	for(long ix = 0; ix < arrStrReadFile.GetSize(); ix++)
	{
		strTmp = arrStrReadFile.GetAt(ix);
		strTmp +=  _T("\n");
		ResultFile.WriteString(strTmp);
	}

	DesignFile.Close();
	CrackFile.Close();
	SupportFile.Close();
	DeflectionFile.Close();
	LengthRateFile.Close();
	EndPartFile.Close();
	ExperienceFile.Close();
	ExperienceEndFile.Close();
	ExperienceConditionFile.Close();

	ResultFile.Close();
}

CString CCalcFloor::GetFileNameResult()
{
	CString szPath = AfxGetApp()->GetProfileString(_T("Afx Directory"), _T("Fixed Directory"));
	CString sFileNameResult = szPath + "\\APlate.Result";

	return sFileNameResult;
}

CString CCalcFloor::GetFileNameBySubject( CheckFileName eFile )
{
	CString strPath = AfxGetApp()->GetProfileString(_T("Afx Directory"), _T("Fixed Directory"));
	CString strExt = _T("");
	switch(eFile)
	{
	case eFile_Design:
		strExt.Format("Design");
		break;
	case eFile_Crack:
		strExt.Format("Crack");
		break;
	case eFile_Support:
		strExt.Format("Support");
		break;
	case eFile_Deflection:
		strExt.Format("Deflection");
		break;
	case eFile_LengthRate:
		strExt.Format("LengthRate");
		break;
	case eFile_EndPart:
		strExt.Format("EndPart");
		break;
	case eFile_Experience:
		strExt.Format("Experience");
		break;
	case eFile_ExperienceEnd:
		strExt.Format("ExperienceEnd");
		break;
	case eFile_ExperienceCondition:
		strExt.Format("ExperienceCondition");
		break;
	}

	CString strFileName = strPath + "\\APlate." + strExt;

	return strFileName;
}

double CCalcFloor::GetRebarCTC( long nPos, long nPart, long nRebarType, long nCycle, BOOL bUpper )
{
	CCalcData *pData = m_pDataManage->GetCalcData();

	double dCTC = 0.0;

	switch(nPart)
	{
	case REBAR_PART_GENERAL:
		switch(nRebarType)
		{
		case REBAR_TYPE_MAIN:
			if(nPos == FLOOR_MID && bUpper)
				dCTC = pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Up_Ctc1;
			else
				dCTC = pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Ctc1;
			break;

		case REBAR_TYPE_SUPPORT:
			if(nPos == FLOOR_MID && bUpper)
				dCTC = pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Up_Ctc2;
			else
				dCTC = pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CtcB;
			break;
		}
		break;

	case REBAR_PART_END:
		switch(nRebarType)
		{
		case REBAR_TYPE_MAIN:
				dCTC = pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Ctc2;
			break;

		case REBAR_TYPE_SUPPORT:
				dCTC = pData->CALC_CANTILEVER_DATA[nPos].m_Rein_CtcB1;
			break;
		}
		break;

	case REBAR_PART_MIDDLE:
		if(bUpper)
			dCTC = pData->m_Ctc1;
		else
			dCTC = pData->m_Ctc2;
		break;
	}

	return dCTC;
}

double CCalcFloor::GetRebarDia( long nPos, long nPart, long nRebarType, long nCycle, BOOL bUpper )
{
	CCalcData *pData = m_pDataManage->GetCalcData();

	long nDiaSize = 0;
	double dDia = 0.0;

	switch(nPart)
	{
	case REBAR_PART_GENERAL:
		switch(nRebarType)
		{
		case REBAR_TYPE_MAIN:
			if(nCycle == REBAR_CYCLE_1)
			{
				if(nPos == FLOOR_MID && bUpper)
					nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Up_Combo1_1Cy);
				else
					nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Combo1);
			}
			else
			{
				if(nPos == FLOOR_MID && bUpper)
					nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Up_Combo1_2Cy);
				else
					nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Combo1_2Cy);
			}
			break;

		case REBAR_TYPE_SUPPORT:
			if(nPos == FLOOR_MID && bUpper)	
				nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Up_Combo2);
			else
				nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Combo3);
			break;
		}
		break;

	case REBAR_PART_END:
		switch(nRebarType)
		{
		case REBAR_TYPE_MAIN:
			if(nCycle == REBAR_CYCLE_1)
			{
				nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Combo2);
			}
			else
			{
				nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Combo2_2Cy);
			}
			break;

		case REBAR_TYPE_SUPPORT:
				nDiaSize = GetRebar(pData->CALC_CANTILEVER_DATA[nPos].m_Rein_Combo4);
			break;
		}
		break;

	case REBAR_PART_MIDDLE:
		if(bUpper)
			nDiaSize = GetRebar(pData->m_ComboInnerJijumSlab1);
		else
			nDiaSize = GetRebar(pData->m_ComboInnerJijumSlab2);
		break;

	}

	dDia = (double)nDiaSize; 

	return dDia;
}

/** 
	@Brief
		- ���������� �޴��� ���θ� �ľ� ( ����, ��ȣ��, �ߺд� �ϰ�� FALSE�� return)
	@Param
		- nSide - ���� ĵƿ���� = FLOOR_LEFT, ����ĵƿ���� =  FLOOR_RIGHT,  �߾Ӻ� = FLOOR_MID
		
*/
BOOL CCalcFloor::IsLaneLoad( long nSide )
{
	if(nSide < 0 || nSide > 2)
		return FALSE;

	CPlateBridgeApp *pDB = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir = NULL;
	CPlateBasicIndex *pBx = NULL;
	long nGsu = pDB->GetGirdersu();

	double dSlabLeft = pDB->m_dWidthSlabLeft;
	double dSlabRight = pDB->m_dWidthSlabRight;
	double dSlabCenter = pDB->m_dBridgeWidth - dSlabLeft - dSlabRight;
	double dSectionStt = 0;
	double dSectionEnd = 0;
	
	switch(nSide)
	{
	case FLOOR_LEFT:
		pGir = pDB->GetGirder(0);
		dSectionStt = 0;
		dSectionEnd = dSlabLeft;
		break;
	case FLOOR_RIGHT:
		pGir = pDB->GetGirder(nGsu-1);
		dSectionStt = dSlabLeft + dSlabCenter;
		dSectionEnd = dSlabLeft + dSlabCenter + dSlabRight;
		break;
	case FLOOR_MID:
		pGir = pDB->GetGirder(min(nGsu - 1, 1));
		dSectionStt = dSlabLeft;
		dSectionEnd = dSlabLeft + dSlabCenter;
		break;
	}
	pBx	= pGir->GetCrossBxAtMinFlangeWidth();

	BOOL bIsLaneLoad = FALSE;

	double dLen = 0;
	double dHDanStt = 0;
	double dHDanEnd = 0;
	long nHDanCnt = pDB->GetQtyHDanNode();
	for(long ix = 0; ix < nHDanCnt; ix++)
	{
		dLen = pDB->GetLengthHDanDom(pBx, ix);
		dHDanEnd = dHDanStt + dLen;
		if(dHDanStt < dSectionEnd && dHDanEnd > dSectionStt)
		{
			if(pDB->GetWallGuardTypeHDan(ix) == HT_NONE)
				bIsLaneLoad = TRUE;
		}
		dHDanStt = dHDanEnd;
	}

	return bIsLaneLoad;
}
