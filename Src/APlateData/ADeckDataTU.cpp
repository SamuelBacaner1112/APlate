// ADeckDataTU.cpp: implementation of the CADeckDataTU class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define REBAR_MIN_SPACETU 100
#define MAKE_RATE m_nSelectRebarAddRate==0 ? ((pRB->m_dDia>=22) ? 1.06 : 1.03) : (pRB->m_bMainRebar ? 1.06 : 1.03)

CADeckDataTU::CADeckDataTU(CDataManage* pManage)
{
	m_pDataManage = pManage;

	m_dCTCAnchor = 200;			// 앵커철근간격
	m_dRangeDanbuBogang = 800;	// 단부보강구간
	m_nQtySabogang = 10;		// 사보강개수
	m_nQtyDivideJong = 8;		// 종방향분할수
	m_nQtyHunch = 9;			// 헌치철근 개수
	m_nQtyAnchor = 2;			// 구속콘 앵커개수(1~3)

	// 피복
	m_dCoverUp = 70;			// 피복(상면)
	m_dCoverDn = 70;			// 피복(하면)
	m_dCoverSd = 70;			// 피복(측면)
	m_dCoverDanbu = 50;			// 피복(단부)
	m_dCoverHunch = 40;			// 피복(헌치)

	// 횡방향철근제원(직경)
	m_dDiaStrup = 13;			// 횡방향철근
	m_dDiaDanbu = 13;			// 단부철근
	m_dDiaAnchor = 13;			// 앵커철근
	m_dDiaSabong = 13;			// 사보강철근

	// 종방향철근제원(직경)
	m_dDiaUpper = 16;			// 상면
	m_dDiaLower = 16;			// 하면
	m_dDiaSide = 16;			// 측면
	m_dDiaEdge = 16;			// 모서리

	// 앵커철근제원
	m_dWidthAnchor = 180;		// 앵커폭
	m_dLengthDolchul = 32;		// 돌출높이
	m_dDisAnchor = 600;			// 앵커간 간격

	m_nAddRebarsuTU = 0;

	// 헌치철근제원
	m_dDiaHunch = 16;			// 헌치보강철근 직경
	m_dOutLength = 460;			// 외측돌출길이
	m_dDisHunch = 100;			// 배치간격	
}


CADeckDataTU::~CADeckDataTU()
{
	AhTPADelete(&m_arrRebarJewonTU,(CRebar*)0);

}


void CADeckDataTU::Serialize(CArchive& ar)
{
//	long nFlag = 2;
//	long nFlag = 3;	// 081110 구속콘 배근수정 변수추가 lina
	long nFlag		= 4;
	long nAddSize	= 0;
	long nAddSize2	= 0;
	long n			= 0;
	long nSize		= 0;
	AhTPASerial(ar, &m_arrRebarJewonTU, (CRebar*)0);
	for (n=0; n<20; n++)
	{
		m_arrTVectorPlan[n].Serialize(ar);
		m_arrTVectorPlanSkewStt[n].Serialize(ar);
		m_arrTVectorPlanSkewEnd[n].Serialize(ar);
		m_arrCTC[n].Serialize(ar);
	}
	
	if(ar.IsStoring())	// m_stStrupShapeTU
	{
		ar << nFlag;
		nSize = m_stBeryukShapeTU.size();
		ar << nSize;
		long nSizeShapeTu;
		for(n=0; n< nSize; n++)
		{
			nSizeShapeTu = m_stBeryukShapeTU.at(n).size();
			ar << nSizeShapeTu;
			for (long m=0; m< nSizeShapeTu; m++)
			{
				ar << m_stBeryukShapeTU[n][m].m_dArrShape;
				ar << m_stBeryukShapeTU[n][m].m_dArrInJoint;
				ar << m_stBeryukShapeTU[n][m].m_dDia;
				ar << m_stBeryukShapeTU[n][m].m_sName;
				ar << m_stBeryukShapeTU[n][m].m_bValid;
				ar << m_stBeryukShapeTU[n][m].m_nCycle;
				ar << m_stBeryukShapeTU[n][m].m_bUpper;
			}
		}

		ar << m_stStrupShapeTU.size();
		for(n=0; n<(long)m_stStrupShapeTU.size(); n++)
		{
			ar << m_stStrupShapeTU.at(n).size();
			for (long m=0; m<(long)m_stStrupShapeTU.at(n).size(); m++)
			{
				ar << m_stStrupShapeTU[n][m].m_dArrShape;
				ar << m_stStrupShapeTU[n][m].m_dArrInJoint;
				ar << m_stStrupShapeTU[n][m].m_dDia;
				ar << m_stStrupShapeTU[n][m].m_sName;
				ar << m_stStrupShapeTU[n][m].m_bValid;
				ar << m_stStrupShapeTU[n][m].m_nCycle;
				ar << m_stStrupShapeTU[n][m].m_bUpper;
			}
		}

		ar << m_dCTCAnchor;
		ar << m_dRangeDanbuBogang;
		ar << m_nQtySabogang;
		ar << m_nQtyDivideJong;
		ar << m_dCoverUp;
		ar << m_dCoverDn;
		ar << m_dCoverSd;
		ar << m_dCoverDanbu;
		ar << m_dDiaStrup;
		ar << m_dDiaDanbu;
		ar << m_dDiaAnchor;
		ar << m_dDiaSabong;
		ar << m_dDiaUpper;
		ar << m_dDiaLower;
		ar << m_dDiaSide;
		ar << m_dDiaEdge;
		ar << m_dWidthAnchor;
		ar << m_dLengthDolchul;
		ar << m_dDisAnchor;
		ar << m_nAddRebarsuTU;		
		// 구속콘크리트 배근수정
		ar << m_nQtyHunch;
		ar << m_nQtyAnchor;
		ar << m_dCoverHunch;
		ar << m_dDiaHunch;
		ar << m_dOutLength;
		ar << m_dDisHunch;

		for(n=0; n<20; n++)
		{
			for(long m=0; m<3; m++)
			{
				m_arrTVectorPlanSkewStt_Cut[n][m].Serialize(ar);
				m_arrTVectorPlanSkewEnd_Cut[n][m].Serialize(ar);
			}
		}

		for(n=0; n<20; n++)
		{
			m_arrTVectorPlanHunchStt[n].Serialize(ar);
			m_arrTVectorPlanHunchEnd[n].Serialize(ar);
		}
	}
	else
	{
		ar >> nFlag;

		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stBeryukShapeTU.push_back(vRebarShape);
		}

		ar >> nAddSize;
		for(n=0; n<nAddSize; n++)
		{
			ar >> nAddSize2;

			vector<REBAR_SHAPE> vRebarShape;
			struct _REBAR_SHAPE stREBAR_SHAPE;
			for (long m=0; m<nAddSize2; m++)
			{
				ar >> stREBAR_SHAPE.m_dArrShape;
				ar >> stREBAR_SHAPE.m_dArrInJoint;
				ar >> stREBAR_SHAPE.m_dDia;
				ar >> stREBAR_SHAPE.m_sName;
				ar >> stREBAR_SHAPE.m_bValid;
				ar >> stREBAR_SHAPE.m_nCycle;
				ar >> stREBAR_SHAPE.m_bUpper;
				// nFlag < 3
				if(stREBAR_SHAPE.m_sName.Find("앵커철근_좌")!=-1 || stREBAR_SHAPE.m_sName.Find("앵커철근_우")!=-1)
				{
					CString szText = stREBAR_SHAPE.m_sName.Right(stREBAR_SHAPE.m_sName.GetLength()-11);
					if(stREBAR_SHAPE.m_sName.Find("앵커철근_좌")!=-1)
						stREBAR_SHAPE.m_sName.Format("앵커철근_%s_1", szText);
					else
						stREBAR_SHAPE.m_sName.Format("앵커철근_%s_2", szText);					
				}

				vRebarShape.push_back(stREBAR_SHAPE);
			}
			m_stStrupShapeTU.push_back(vRebarShape);
		}

		if(nFlag>0)
		{
			ar >> m_dCTCAnchor;
			ar >> m_dRangeDanbuBogang;
			ar >> m_nQtySabogang;
			ar >> m_nQtyDivideJong;
			ar >> m_dCoverUp;
			ar >> m_dCoverDn;
			ar >> m_dCoverSd;
			ar >> m_dCoverDanbu;
			ar >> m_dDiaStrup;
			ar >> m_dDiaDanbu;
			ar >> m_dDiaAnchor;
			ar >> m_dDiaSabong;
			ar >> m_dDiaUpper;
			ar >> m_dDiaLower;
			ar >> m_dDiaSide;
			ar >> m_dDiaEdge;
			ar >> m_dWidthAnchor;
			ar >> m_dLengthDolchul;
			ar >> m_dDisAnchor;
			ar >> m_nAddRebarsuTU;
		}
		if(nFlag>2)
		{
			ar >> m_nQtyHunch;
			ar >> m_nQtyAnchor;
			ar >> m_dCoverHunch;
			ar >> m_dDiaHunch;
			ar >> m_dOutLength;
			ar >> m_dDisHunch;
		}
		else
		{
			m_nQtyHunch		= 0;					// 기존교량에는 헌치철근이 없음
			m_dDisAnchor	*= 2 - m_dWidthAnchor;	// 이격거리가 거더중심-앵커의 중심에서 앵커간 간격으로 변경됨
		}

		if(nFlag>1)
		{
			for (n=0; n<20; n++)
			{
				for(long m=0; m<3; m++)
				{
					m_arrTVectorPlanSkewStt_Cut[n][m].Serialize(ar);
					m_arrTVectorPlanSkewEnd_Cut[n][m].Serialize(ar);
				}
			}
		}

		if(nFlag>3)
		{
			for(n=0; n<20; n++)
			{
				m_arrTVectorPlanHunchStt[n].Serialize(ar);
				m_arrTVectorPlanHunchEnd[n].Serialize(ar);
			}
		}
	}
}


// 구속콘크리트 입력권고안에서 생성된 거더별 선형을 가져옴
CLineInfo* CADeckDataTU::GetLineInfo(long nG)
{
	CLineInfoApp *pLineApp = m_pDataManage->GetLineInfoApp();
	CString sName;
	sName.Format("G%d", nG);

	return pLineApp->GetLine(sName);
}


// 거더별 주철근 CTC를 계산함
void CADeckDataTU::MakeDimArr()
{
	for (long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		MakeDimArrSub(nG);
	}
}


// 주철근 간격을 계산함.
void CADeckDataTU::MakeDimArrSub(long nG)
{
	double dRange = GetRangeMainRebar(nG, FALSE);	// 사각부를 제외한 범위
	double nRebar = (long)(dRange / m_dCTCAnchor);
	double dWidthExtra = dRange - (nRebar*m_dCTCAnchor);
	long nRebarBogang = long(m_dRangeDanbuBogang/m_dCTCAnchor*2);	// 단부 보강범위 개수
	nRebar -= nRebarBogang;

	CDPointArray xyArrDim;
	if(dWidthExtra==0)
	{
		xyArrDim.Add(CDPoint(nRebarBogang, m_dCTCAnchor/2));		
		xyArrDim.Add(CDPoint(nRebar, m_dCTCAnchor));		
		xyArrDim.Add(CDPoint(nRebarBogang, m_dCTCAnchor/2));
	}
	else
	{	
		if(dWidthExtra/2 > REBAR_MIN_SPACETU)
		{
			xyArrDim.Add(CDPoint(1,dWidthExtra/2));
			xyArrDim.Add(CDPoint(nRebarBogang, m_dCTCAnchor/2));		
			xyArrDim.Add(CDPoint(nRebar, m_dCTCAnchor));		
			xyArrDim.Add(CDPoint(nRebarBogang, m_dCTCAnchor/2));	
			xyArrDim.Add(CDPoint(1,dWidthExtra/2));
		}
		else
		{
			xyArrDim.Add(CDPoint(1,(dWidthExtra+m_dCTCAnchor)/2));				
			xyArrDim.Add(CDPoint(nRebarBogang, m_dCTCAnchor/2));		
			xyArrDim.Add(CDPoint(nRebar-1, m_dCTCAnchor));		
			xyArrDim.Add(CDPoint(nRebarBogang, m_dCTCAnchor/2));	
			xyArrDim.Add(CDPoint(1,(dWidthExtra+m_dCTCAnchor)/2));
		}
	}	

	m_arrCTC[nG] = xyArrDim;
}


// 중심선형의 스테이션이 거더의 스테이션에 대응되는 값을 구함.
double CADeckDataTU::GetStaAtGir(double dStaCen, CDPoint vAng, long nG)
{
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);
	CLineInfoApp *pLineAppGir = (CLineInfoApp *)GetLineInfo(nG);
	// dStaCen위치에서 CLineSplit이 분할되는 경우나 매치가 안되는 경우가 있을수 있음.
	// dStaCen이 거더선형의 스테이션이 들어가야함. 그러나 현재 거더스테이션을 알수 있는 방법이 없음.
	CLineSplit* pSplit = pLineAppGir->GetLineSplitByStation(dStaCen);
		
	CDPoint vAngAzimuth = pGirCen->GetLine()->GetAngleAzimuth(dStaCen);
	CDPoint xy = pGirCen->GetLine()->GetXyLineDis(dStaCen, 0);
	double staResult = 0;

	pSplit->GetStationMatchXyAndAngle(xy, vAngAzimuth.Rotate(vAng), staResult);
	
	if(staResult==-0.1)	return dStaCen;
	else				return staResult;
}


// 거더별 주철근범위의 시종점 스테이션
// bIncludeCover=TRUE 피복을 포함한 범위
double CADeckDataTU::GetStaMainRebar(BOOL bStt, long nG, BOOL bIncludeCover)
{
	CPlateGirderApp *pGir = m_pDB->GetGirder(nG);

	double dCover = bIncludeCover ? 0 : m_dCoverDanbu;
	if(bStt)
	{
		CDPoint vAngStt = pGir->GetAAA(OFF_STT_SLAB);
		double	dStaGir = GetStaAtGir(pGir->GetSSS(OFF_STT_GIRDER), vAngStt, nG);
		double	dLengthGap = m_pDB->m_dLengthTUBindConc_Stt - dCover/fabs(vAngStt.y);

		return	dStaGir - dLengthGap;
	}
	else
	{
		CDPoint vAngEnd = pGir->GetAAA(OFF_END_SLAB);
		double	dStaGir = GetStaAtGir(pGir->GetSSS(OFF_END_GIRDER), vAngEnd, nG);
		double	dLengthGap = m_pDB->m_dLengthTUBindConc_End - dCover/fabs(vAngEnd.y);

		return	dStaGir + dLengthGap;
	}
}


// 사각부를 제외한 주철근의 범위
double CADeckDataTU::GetRangeMainRebar(long nG, BOOL bIncludeCover)
{
	return GetStaMainRebar(FALSE, nG, FALSE) - GetStaMainRebar(TRUE, nG, FALSE) - GetLengthSkewTriangle(nG, TRUE) - GetLengthSkewTriangle(nG, FALSE);
}


// 사각범위의 길이 구하기
double CADeckDataTU::GetLengthSkewTriangle(long nG, BOOL bStt)
{
	if(bStt && !m_pDataManage->GetDeckData()->IsSkewStt())
		return 0;
	else if(!bStt && !m_pDataManage->GetDeckData()->IsSkewEnd())
		return 0;
	
	CPlateGirderApp *pGirCen = m_pDB->GetGirder(-1);
	CLineInfo *pLineGir = GetLineInfo(nG);

	double dStaAtGir = GetStaMainRebar(bStt, nG, FALSE);	// 거더의 주철근범위 시작스테이션
	double dDist = -m_pDB->m_BindConc.m_dWidth/2+m_dCoverSd;
	CDPoint vAngSlab = pGirCen->GetAAA(bStt ? OFF_STT_SLAB : OFF_END_SLAB);	// 슬래브시종점각도

	double dStaSkew = pLineGir->GetStationByScInLine(dStaAtGir, dDist, vAngSlab);

	return fabs(dStaSkew-dStaAtGir);
}


// 주철근 구간중 사각부를 제외한 주철근 구간의 시종점 스테이션
double CADeckDataTU::GetStaMainRebarExceptSkew(BOOL bStt, long nG)
{
	double dSta = GetStaMainRebar(bStt, nG, FALSE);
	double dRangeSkewAtGir = GetLengthSkewTriangle(nG, bStt);

	return bStt ? (dSta + dRangeSkewAtGir) : (dSta - dRangeSkewAtGir);
}


// 사각부를 제외한 주철근구간의 평면좌표
CDPoint CADeckDataTU::GetXySkewRange(long nG, BOOL bStt, BOOL bLeft)
{
	CLineInfo *pLineGir = GetLineInfo(nG);

	double dStaSc = GetStaMainRebarExceptSkew(bStt, nG);
	double dDist = bLeft ? -m_pDB->m_BindConc.m_dWidth/2+m_dCoverSd : m_pDB->m_BindConc.m_dWidth/2-m_dCoverSd;

	return pLineGir->GetXyLineDis(dStaSc, dDist);
}


void CADeckDataTU::MakePlanMainRebarTU()
{
	CRebarPlacing RebarPlace;

	CDPoint xyLeft, xyRight;
	double dDistL = 0, dDistR = 0;

	for (long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CLineInfo *pLineGir = GetLineInfo(nG);

		CDoubleArray dArrCtc;
		RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(m_arrCTC[nG], dArrCtc, FALSE);
		
		double dStaStt = GetStaMainRebarExceptSkew(TRUE, nG);
	
		dDistL = -m_pDB->m_BindConc.m_dWidth/2+m_dCoverSd;
		dDistR = m_pDB->m_BindConc.m_dWidth/2-m_dCoverSd;

		xyLeft = pLineGir->GetXyLineDis(dStaStt, dDistL);
		xyRight = pLineGir->GetXyLineDis(dStaStt, dDistR);
		
		CTwinVectorArray tvArr;
		CTwinVector tvLine;
		tvLine.m_v1 = xyLeft;
		tvLine.m_v2 = xyRight;
		tvArr.Add(tvLine);
		for (long n=0; n<dArrCtc.GetSize(); n++)
		{
			dStaStt += dArrCtc[n];

			xyLeft = pLineGir->GetXyLineDis(dStaStt, dDistL);
			xyRight = pLineGir->GetXyLineDis(dStaStt, dDistR);
		
			CTwinVector tvLine;
			tvLine.m_v1 = xyLeft;
			tvLine.m_v2 = xyRight;
			tvArr.Add(tvLine);
		}
		m_arrTVectorPlan[nG] = tvArr;
	}
}


void CADeckDataTU::MakePlanSabogangRebarTU()
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CADeckData		*pDeck	= m_pDataManage->GetDeckData();

	CDPoint xyLeft, xyRight;
	double dDistL = 0, dDistR = 0;

	for (long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CLineInfo *pLineGir = GetLineInfo(nG);
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		for (BOOL bStt=TRUE; bStt>=FALSE; bStt--)
		{
			CDPoint vAng = pGir->GetAAA(bStt?OFF_STT_SLAB:OFF_END_SLAB);
			double dStaSttGir = GetStaMainRebar(bStt, nG, FALSE);

			double	dStndCuttingAngle	= pDB->m_dGirEndStndGuttingAngle;
			BOOL	bCutAngle90		= (vAng.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
			BOOL	bCutAngle90_Inv	= (vAng.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
						
			double sw = bStt?1:-1;
			dDistL = -m_pDB->m_BindConc.m_dWidth/2+m_dCoverSd;
			dDistR =  m_pDB->m_BindConc.m_dWidth/2-m_dCoverSd;

			xyLeft = pLineGir->GetXyLineDisByAng(dStaSttGir, dDistL, vAng);
			xyRight = pLineGir->GetXyLineDisByAng(dStaSttGir, dDistR, vAng);

			CTwinVectorArray tvArr;
			CTwinVector tvLine;
			tvLine.m_v1 = xyLeft;
			tvLine.m_v2 = xyRight;
			tvArr.Add(tvLine);
			for (long n=0; n<m_nQtySabogang-1; n++)
			{
				dStaSttGir += sw*m_dCTCAnchor/2;

				xyLeft = pLineGir->GetXyLineDisByAng(dStaSttGir, dDistL, vAng);
				xyRight = pLineGir->GetXyLineDisByAng(dStaSttGir, dDistR, vAng);
			
				CTwinVector tvLine;
				tvLine.m_v1 = xyLeft;
				tvLine.m_v2 = xyRight;
				tvArr.Add(tvLine);
			}
			
			if(bStt)
			{
				if(pDeck->IsSkewStt())	m_arrTVectorPlanSkewStt[nG] = tvArr;
				else					m_arrTVectorPlanSkewStt[nG].RemoveAll();
			}
			else
			{
				if(pDeck->IsSkewEnd())	m_arrTVectorPlanSkewEnd[nG] = tvArr;
				else					m_arrTVectorPlanSkewEnd[nG].RemoveAll();
			}

			// 거더단부 모따기 적용되었을때 사보강철근 자르기
			if(bCutAngle90 || bCutAngle90_Inv)
			{
				double	dStation		= GetStaMainRebar(bStt, nG, FALSE);
				CDPointArray	arrXy;
				CDPoint	xyResult, xyStt, xyEnd;
				BOOL	bExist	= FALSE;
				arrXy.RemoveAll();
				CDPoint	vAngleLeft	= bStt ? (bCutAngle90_Inv? CDPoint(0,1) : vAng) : (bCutAngle90? CDPoint(0,1) : vAng);
				CDPoint	vAngleRight	= bStt ? (bCutAngle90? CDPoint(0,1) : vAng) : (bCutAngle90_Inv? CDPoint(0,1) : vAng);

				CDPoint	xyLeft_BindConc	= pGir->GetXyGirderDis(dStation, dDistL, vAngleLeft);
				CDPoint	xyMid_BindConc	= pGir->GetXyGirderDis(dStation, 0, vAng);
				CDPoint	xyRight_BindConc= pGir->GetXyGirderDis(dStation, dDistR, vAngleRight);

				if(bCutAngle90)
				{
					if(bStt)
					{
						xyStt	= xyRight_BindConc;
						xyEnd	= xyMid_BindConc;
					}
					else
					{
						xyStt	= xyLeft_BindConc;
						xyEnd	= xyMid_BindConc;
					}
				}
				else
				{
					if(bStt)
					{
						xyStt	= xyLeft_BindConc;
						xyEnd	= xyMid_BindConc;
					}
					else
					{
						xyStt	= xyRight_BindConc;
						xyEnd	= xyMid_BindConc;
					}
				}

				CTwinVectorArray tvArrCut1, tvArrCut2, tvArrCut3;
				for(long n=0; n<tvArr.GetSize(); n++)
				{
					bExist	= GetXyMatchSegAndSeg(tvArr.GetAt(n).m_v1, tvArr.GetAt(n).m_v2, xyStt, xyEnd, xyResult);
					if(bExist)		// 모따기 적용된 철근
					{
						if(bCutAngle90)
						{
							tvLine.m_v1 = bStt ? tvArr.GetAt(n).m_v1 : xyLeft_BindConc;
							tvLine.m_v2 = xyResult;
							tvArrCut2.Add(tvLine);
							tvLine.m_v1 = xyResult;
							tvLine.m_v2 = bStt ? xyRight_BindConc : tvArr.GetAt(n).m_v2;
							tvArrCut2.Add(tvLine);
						}
						else
						{
							tvLine.m_v1 = bStt ? xyLeft_BindConc : tvArr.GetAt(n).m_v1;
							tvLine.m_v2 = xyResult;
							tvArrCut2.Add(tvLine);
							tvLine.m_v1 = xyResult;
							tvLine.m_v2 = bStt ? tvArr.GetAt(n).m_v2 : xyRight_BindConc;
							tvArrCut2.Add(tvLine);
						}
					}
					else	// 기존길이 유지 철근
					{
						tvArrCut1.Add(tvArr.GetAt(n));
					}
				}

				// 단부철근 (Cut Line)
				tvLine.m_v1 = xyStt;
				tvLine.m_v2 = xyEnd;
				tvArrCut3.Add(tvLine);

				if(bStt)
				{
					m_arrTVectorPlanSkewStt_Cut[nG][0]	= tvArrCut1;
					m_arrTVectorPlanSkewStt_Cut[nG][1]	= tvArrCut2;
					m_arrTVectorPlanSkewStt_Cut[nG][2]	= tvArrCut3;
				}
				else
				{
					m_arrTVectorPlanSkewEnd_Cut[nG][0]	= tvArrCut1;
					m_arrTVectorPlanSkewEnd_Cut[nG][1]	= tvArrCut2;
					m_arrTVectorPlanSkewEnd_Cut[nG][2]	= tvArrCut3;
				}
			}
			else
			{
				if(bStt)
				{
					m_arrTVectorPlanSkewStt_Cut[nG][0].RemoveAll();
					m_arrTVectorPlanSkewStt_Cut[nG][1].RemoveAll();
					m_arrTVectorPlanSkewStt_Cut[nG][2].RemoveAll();
				}
				else
				{
					m_arrTVectorPlanSkewEnd_Cut[nG][0].RemoveAll();
					m_arrTVectorPlanSkewEnd_Cut[nG][1].RemoveAll();
					m_arrTVectorPlanSkewEnd_Cut[nG][2].RemoveAll();
				}
			}
		}
	}
}

void CADeckDataTU::MakePlanHunchRebarTU()
{
	CDPoint xyLeft, xyRight, xyCenL, xyCenR;
	double dDistL = 0, dDistR = 0;

	for (long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CLineInfo *pLineGir = GetLineInfo(nG);
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		double dWebT = GetMaxWebT(nG);

		for (BOOL bStt=TRUE; bStt>=FALSE; bStt--)
		{
			CDPoint vAng = pGir->GetAAA(bStt?OFF_STT_SLAB:OFF_END_SLAB);
			double dStaSttGir = GetStaMainRebar(bStt, nG, FALSE);
			
			double sw = bStt?1:-1;
			dDistL = -m_pDB->m_BindConc.m_dWidth/2-m_dOutLength;
			dDistR =  m_pDB->m_BindConc.m_dWidth/2+m_dOutLength;

			xyLeft	= pLineGir->GetXyLineDisByAng(dStaSttGir, dDistL,	vAng);
			xyCenL	= pLineGir->GetXyLineDisByAng(dStaSttGir, -dWebT/2, vAng);
			xyCenR	= pLineGir->GetXyLineDisByAng(dStaSttGir,  dWebT/2, vAng);
			xyRight = pLineGir->GetXyLineDisByAng(dStaSttGir, dDistR,	vAng);			

			CTwinVectorArray tvArr;
			CTwinVector tvLine;
			
			if(m_nQtyHunch>0)
			{
				tvLine.m_v1 = xyLeft;
				tvLine.m_v2 = xyCenL;
				tvArr.Add(tvLine);
				tvLine.m_v1 = xyCenR;
				tvLine.m_v2 = xyRight;
				tvArr.Add(tvLine);
			}
			for (long n=0; n<m_nQtyHunch-1; n++)
			{
				dStaSttGir += sw*m_dDisHunch;

				xyLeft = pLineGir->GetXyLineDisByAng(dStaSttGir, dDistL, vAng);
				xyCenL	= pLineGir->GetXyLineDisByAng(dStaSttGir, -dWebT/2, vAng);
				xyCenR	= pLineGir->GetXyLineDisByAng(dStaSttGir,  dWebT/2, vAng);
				xyRight = pLineGir->GetXyLineDisByAng(dStaSttGir, dDistR, vAng);
			
				tvLine.m_v1 = xyLeft;
				tvLine.m_v2 = xyCenL;
				tvArr.Add(tvLine);
				tvLine.m_v1 = xyCenR;
				tvLine.m_v2 = xyRight;
				tvArr.Add(tvLine);
			}
			
			if(bStt)	m_arrTVectorPlanHunchStt[nG] = tvArr;
			else		m_arrTVectorPlanHunchEnd[nG] = tvArr;
		}
	}
}

void CADeckDataTU::MakeShapeBeryukRebarTU()
{
	m_stBeryukShapeTU.clear();

	for (long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		CLineInfo *pLineGir = GetLineInfo(nG);

		CPlateBxFinder		Finder(pGir);
		CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
		CPlateBasicIndex	*pBxEnd	= Finder.GetBxFirst(BX_END_GIRDER);

		double	dStaSttCen	= pBxStt->GetStation() - m_pDB->m_dLengthTUBindConc_Stt + m_dCoverDanbu;
		double	dStaEndCen	= pBxEnd->GetStation() + m_pDB->m_dLengthTUBindConc_End - m_dCoverDanbu;
		dStaEndCen	= dStaSttCen + pGir->GetLengthDimType(dStaSttCen, dStaEndCen);
		
		CDPoint vAngSlabStt		= pGir->GetAAA(OFF_STT_SLAB);	// 슬래브시점각도
		CDPoint vAngSlabEnd		= pGir->GetAAA(OFF_END_SLAB);	// 슬래브시점각도
		double	dDistLSide		= -m_pDB->m_BindConc.m_dWidth/2+m_dCoverSd;
		double	dDistRSide		=  m_pDB->m_BindConc.m_dWidth/2-m_dCoverSd;
		double	dCTCDivideJong	= (m_pDB->m_BindConc.m_dWidth-m_dCoverSd*2) / m_nQtyDivideJong;
		double	dDistLInner		= dDistLSide + dCTCDivideJong;
		double	dDistRInner		= dDistRSide - dCTCDivideJong;

		double	dStaSttRebarLeftSide	= pLineGir->GetStationByScInLine(dStaSttCen, dDistLSide, vAngSlabStt);
		double	dStaSttRebarRightSide	= pLineGir->GetStationByScInLine(dStaSttCen, dDistRSide, vAngSlabStt);
		double	dStaEndRebarLeftSide	= pLineGir->GetStationByScInLine(dStaEndCen, dDistLSide, vAngSlabEnd);
		double	dStaEndRebarRightSide	= pLineGir->GetStationByScInLine(dStaEndCen, dDistRSide, vAngSlabEnd);
		double	dStaSttRebarCen			= pLineGir->GetStationByScInLine(dStaSttCen, 0, vAngSlabStt);
		double	dStaEndRebarCen			= pLineGir->GetStationByScInLine(dStaEndCen, 0, vAngSlabEnd);

		double	dStaSttRebarLeftInner	= pLineGir->GetStationByScInLine(dStaSttCen, dDistLInner, vAngSlabStt);
		double	dStaSttRebarRightInner	= pLineGir->GetStationByScInLine(dStaSttCen, dDistRInner, vAngSlabStt);
		double	dStaEndRebarLeftInner	= pLineGir->GetStationByScInLine(dStaEndCen, dDistLInner, vAngSlabEnd);
		double	dStaEndRebarRightInner	= pLineGir->GetStationByScInLine(dStaEndCen, dDistRInner, vAngSlabEnd);
		
		double	dYSttRebar	= pGir->GetElevationFlangeCamber(pBxStt->GetStation()) - m_pDB->m_BindConc.m_dDis + m_dCoverDn;
		double	dYEndRebar	= pGir->GetElevationFlangeCamber(pBxEnd->GetStation()) - m_pDB->m_BindConc.m_dDis + m_dCoverDn;

		vector<REBAR_SHAPE> vArrShapeGir;
		CDPointArray arrXY;

		//////////////////////////////////////////////////////////////////////////
		// 모서리(4개-상하좌우)
		arrXY.Add(CDPoint(dStaSttRebarLeftSide, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 좌상(시점)
		arrXY.Add(CDPoint(dStaEndRebarLeftSide, dYEndRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 좌상(종점)
		REBAR_SHAPE shapeLUp;
		shapeLUp.m_dArrShape = arrXY;
		shapeLUp.m_dDia = m_dDiaEdge;
		shapeLUp.m_sName.Format("배력철근_좌상모서리G%d", nG+1);
		vArrShapeGir.push_back(shapeLUp);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarRightSide, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 우상(시점)
		arrXY.Add(CDPoint(dStaEndRebarRightSide, dYEndRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 우상(종점)
		REBAR_SHAPE shapeRUp;
		shapeRUp.m_dArrShape = arrXY;
		shapeRUp.m_dDia = m_dDiaEdge;
		shapeRUp.m_sName.Format("배력철근_우상모서리G%d", nG+1);
		vArrShapeGir.push_back(shapeRUp);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarLeftSide, dYSttRebar));	// 좌하(시점)
		arrXY.Add(CDPoint(dStaEndRebarLeftSide, dYEndRebar));	// 좌하(종점)
		REBAR_SHAPE shapeLLo;
		shapeLLo.m_dArrShape = arrXY;
		shapeLLo.m_dDia = m_dDiaEdge;
		shapeLLo.m_sName.Format("배력철근_좌하모서리G%d", nG+1);
		vArrShapeGir.push_back(shapeLLo);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarRightSide, dYSttRebar));	// 우하(시점)
		arrXY.Add(CDPoint(dStaEndRebarRightSide, dYEndRebar));	// 우하(종점)
		REBAR_SHAPE shapeRLo;
		shapeRLo.m_dArrShape = arrXY;
		shapeRLo.m_dDia = m_dDiaEdge;
		shapeRLo.m_sName.Format("배력철근_우하모서리G%d", nG+1);
		vArrShapeGir.push_back(shapeRLo);

		//////////////////////////////////////////////////////////////////////////
		// 단부측면(2개-시종점)
		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarLeftSide, dYSttRebar - m_dCoverDn + m_pDB->m_BindConc.m_dHeight/2));	// 좌측면(시점)
		arrXY.Add(CDPoint(dStaEndRebarLeftSide, dYEndRebar - m_dCoverDn + m_pDB->m_BindConc.m_dHeight/2));	// 좌측면(종점)
		REBAR_SHAPE shapeLSide;
		shapeLSide.m_dArrShape = arrXY;
		shapeLSide.m_dDia = m_dDiaEdge;
		shapeLSide.m_sName.Format("배력철근_좌측면G%d", nG+1);
		vArrShapeGir.push_back(shapeLSide);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarRightSide, dYSttRebar - m_dCoverDn + m_pDB->m_BindConc.m_dHeight/2));	// 우측면(시점)
		arrXY.Add(CDPoint(dStaEndRebarRightSide, dYEndRebar - m_dCoverDn + m_pDB->m_BindConc.m_dHeight/2));	// 우측면(종점)
		REBAR_SHAPE shapeRSide;
		shapeRSide.m_dArrShape = arrXY;
		shapeRSide.m_dDia = m_dDiaEdge;
		shapeRSide.m_sName.Format("배력철근_우측면G%d", nG+1);
		vArrShapeGir.push_back(shapeRSide);

		//////////////////////////////////////////////////////////////////////////
		// 상부(3개-좌,거더중간,우)
		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarLeftInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 상부 좌측면 인접(시점)
		arrXY.Add(CDPoint(dStaEndRebarLeftInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 상부 좌측면 인접(종점)
		REBAR_SHAPE shapeLUpInner;
		shapeLUpInner.m_dArrShape = arrXY;
		shapeLUpInner.m_dDia = m_dDiaUpper;
		shapeLUpInner.m_sName.Format("배력철근_상부좌G%d", nG+1);
		vArrShapeGir.push_back(shapeLUpInner);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarCen, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));		// 상부 좌측면 인접(시점)
		arrXY.Add(CDPoint(dStaEndRebarCen, dYEndRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));		// 상부 좌측면 인접(종점)
		REBAR_SHAPE shapeUpCenter;
		shapeUpCenter.m_dArrShape = arrXY;
		shapeUpCenter.m_dDia = m_dDiaUpper;
		shapeUpCenter.m_sName.Format("배력철근_상부거더중간G%d", nG+1);
		vArrShapeGir.push_back(shapeUpCenter);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarRightInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 상부 우측면 인접(시점)
		arrXY.Add(CDPoint(dStaEndRebarRightInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 상부 우측면 인접(종점)
		REBAR_SHAPE shapeRUpInner;
		shapeRUpInner.m_dArrShape = arrXY;
		shapeRUpInner.m_dDia = m_dDiaUpper;
		shapeRUpInner.m_sName.Format("배력철근_상부우G%d", nG+1);
		vArrShapeGir.push_back(shapeRUpInner);
	
		//////////////////////////////////////////////////////////////////////////
		// 하부(3개-좌,우,거더중심)
		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarLeftInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 하부 좌측면 인접(시점)
		arrXY.Add(CDPoint(dStaSttRebarLeftInner, dYSttRebar));	// 하부 좌측면 인접(시점)
		arrXY.Add(CDPoint(dStaEndRebarLeftInner, dYSttRebar));	// 하부 좌측면 인접(종점)
		arrXY.Add(CDPoint(dStaEndRebarLeftInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 하부 좌측면 인접(종점)
		REBAR_SHAPE shapeLLoInner;
		shapeLLoInner.m_dArrShape = arrXY;
		shapeLLoInner.m_dDia = m_dDiaLower;
		shapeLLoInner.m_sName.Format("배력철근_하부좌G%d", nG+1);
		vArrShapeGir.push_back(shapeLLoInner);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarCen, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));		// 하부 좌측면 인접(시점)
		arrXY.Add(CDPoint(dStaSttRebarCen, dYSttRebar));		// 하부 좌측면 인접(시점)
		arrXY.Add(CDPoint(dStaEndRebarCen, dYSttRebar));		// 하부 좌측면 인접(종점)
		arrXY.Add(CDPoint(dStaEndRebarCen, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));		// 하부 좌측면 인접(종점)
		REBAR_SHAPE shapeLoCenter;
		shapeLoCenter.m_dArrShape = arrXY;
		shapeLoCenter.m_dDia = m_dDiaLower;
		shapeLoCenter.m_sName.Format("배력철근_하부거더중간G%d", nG+1);
		vArrShapeGir.push_back(shapeLoCenter);

		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dStaSttRebarRightInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 하부 우측면 인접(시점)
		arrXY.Add(CDPoint(dStaSttRebarRightInner, dYSttRebar));	// 하부 우측면 인접(시점)
		arrXY.Add(CDPoint(dStaEndRebarRightInner, dYSttRebar));	// 하부 우측면 인접(종점)
		arrXY.Add(CDPoint(dStaEndRebarRightInner, dYSttRebar+(m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)));	// 하부 우측면 인접(종점)
		REBAR_SHAPE shapeRLoInner;
		shapeRLoInner.m_dArrShape = arrXY;
		shapeRLoInner.m_dDia = m_dDiaLower;
		shapeRLoInner.m_sName.Format("배력철근_하부우G%d", nG+1);
		vArrShapeGir.push_back(shapeRLoInner);

		m_stBeryukShapeTU.push_back(vArrShapeGir);
	}		
}

CDPointArray CADeckDataTU::GetRebarShapeByJongName(long nG, CString szName)		
{
	MakeShapeBeryukRebarTU();

	vector<REBAR_SHAPE>* pRebarShape =  &m_stBeryukShapeTU.at(nG);

	for (long n=0; n<(long)pRebarShape->size(); n++)
	{
		if(pRebarShape->at(n).m_sName == szName)
			return pRebarShape->at(n).m_dArrShape;
	}

	CDPointArray xyArrReturn;
	return xyArrReturn;
}

void CADeckDataTU::MakeShapeStrupRebarTU()
{
	double	dBindW		= m_pDB->m_BindConc.m_dWidth;
	double	dBindH		= m_pDB->m_BindConc.m_dHeight;
	double	dBindS1		= m_pDB->m_BindConc.m_dDeep;
	double	dBindS2		= m_pDB->m_BindConc.m_dDis;
	double	dSlabLowH	= -m_pDB->m_dThickSlabBase;		// 슬래브 하면의 높이

	m_stStrupShapeTU.clear();
	
	for (long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = m_pDB->GetGirder(nG);
		
		double	dWebT			= GetMaxWebT(nG);
		double	dGirderCenter	= pGir->m_dGirderCenter;

		double	dX	= dGirderCenter;
		double	dY	= dSlabLowH + dBindS1;

		vector<REBAR_SHAPE> vArrShapeGir;
		CDPointArray arrXY;
		arrXY.RemoveAll();

		//////////////////////////////////////////////////////////////////////////
		// 스트럽
		arrXY.Add(CDPoint(dX-dWebT/2,				dY-dBindH+m_dCoverDn));		// web 좌측
		arrXY.Add(CDPoint(dX-dBindW/2+m_dCoverSd,	dY-dBindH+m_dCoverDn));		// 좌하
		arrXY.Add(CDPoint(dX-dBindW/2+m_dCoverSd,	dY-m_dCoverUp));			// 좌상
		arrXY.Add(CDPoint(dX+dBindW/2-m_dCoverSd,	dY-m_dCoverUp));			// 우상
		arrXY.Add(CDPoint(dX+dBindW/2-m_dCoverSd,	dY-dBindH+m_dCoverDn));		// 우하
		arrXY.Add(CDPoint(dX+dWebT/2,				dY-dBindH+m_dCoverDn));		// web 우측
		REBAR_SHAPE shapeStrupGen;
		shapeStrupGen.m_dArrShape = arrXY;
		shapeStrupGen.m_dDia = m_dDiaStrup;
		shapeStrupGen.m_sName.Format("스트럽일반_G%d", nG+1);
		vArrShapeGir.push_back(shapeStrupGen);

		// 스트럽단부철근 Shape의 경우 모따기가 고려되지 않은 횡단면도용, 모따기는 철근제원에서 처리함
		arrXY.RemoveAll();
		arrXY.Add(CDPoint(dX-dBindW/2+m_dCoverSd,	dY-dBindH+m_dCoverDn));		// 좌하
		arrXY.Add(CDPoint(dX-dBindW/2+m_dCoverSd,	dY-m_dCoverUp));			// 좌상
		arrXY.Add(CDPoint(dX+dBindW/2-m_dCoverSd,	dY-m_dCoverUp));			// 우상
		arrXY.Add(CDPoint(dX+dBindW/2-m_dCoverSd,	dY-dBindH+m_dCoverDn));		// 우하
		arrXY.Add(CDPoint(dX-dBindW/2+m_dCoverSd,	dY-dBindH+m_dCoverDn));		// 좌하
		REBAR_SHAPE shapeStrupEnd;
		shapeStrupEnd.m_dArrShape = arrXY;
		shapeStrupEnd.m_dDia = m_dDiaStrup;
		shapeStrupEnd.m_sName.Format("스트럽단부_G%d", nG+1);
		vArrShapeGir.push_back(shapeStrupEnd);

		//////////////////////////////////////////////////////////////////////////
		// Anchor철근
		double dSttX = dX-(m_dDisAnchor*(m_nQtyAnchor-1)+m_dWidthAnchor*m_nQtyAnchor)/2;
		for(long n=0; n<m_nQtyAnchor; n++)
		{
			arrXY.RemoveAll();
			arrXY.Add(CDPoint(dSttX,				dY-dBindH+m_dCoverDn));		// 좌하
			arrXY.Add(CDPoint(dSttX,				dY+m_dLengthDolchul));		// 좌상
			arrXY.Add(CDPoint(dSttX+m_dWidthAnchor,	dY+m_dLengthDolchul));		// 우상
			arrXY.Add(CDPoint(dSttX+m_dWidthAnchor,	dY-dBindH+m_dCoverDn));		// 우하
			if(m_nQtyAnchor>1)
				arrXY.Add(CDPoint(dSttX,			dY-dBindH+m_dCoverDn));		// 좌하
			REBAR_SHAPE shapeAnchorL;
			shapeAnchorL.m_dArrShape = arrXY;
			shapeAnchorL.m_dDia = m_dDiaAnchor;
			shapeAnchorL.m_sName.Format("앵커철근_G%d_%d", nG+1, n+1);
			vArrShapeGir.push_back(shapeAnchorL);
			dSttX += m_dDisAnchor+m_dWidthAnchor;
		}

		//////////////////////////////////////////////////////////////////////////
		// 헌치부 철근		

		m_stStrupShapeTU.push_back(vArrShapeGir);
	}		

}

CDPointArray CADeckDataTU::GetRebarShapeByStrupName(long nG, CString szName)		
{
	vector<REBAR_SHAPE>* pRebarShape =  &m_stStrupShapeTU.at(nG);

	for (long n=0; n<(long)pRebarShape->size(); n++)
	{
		if(pRebarShape->at(n).m_sName == szName)
			return pRebarShape->at(n).m_dArrShape;
	}

	CDPointArray xyArrReturn;
	return xyArrReturn;
}

void CADeckDataTU::MakeRebarJewonTU()
{
	CADeckData		*pDeck	= m_pDataManage->GetDeckData();
	CCalcData		*pData	= m_pDataManage->GetCalcData();

	double m_dCK = pDeck->m_SigmaCK;
	pDeck->m_SigmaCK = pData->DESIGN_MATERIAL.m_dBindSigmaCK/10;

	CString	sText = _T(""), sRebar = _T("");
	CDPoint	xyLeft(0,0), xyRight(0,0);
	double	dDistL = 0, dDistR = 0;
	long	nRebarNo	= 0;

	CDPointArray xyArrShapeRebar;

	AhTPADelete(&m_arrRebarJewonTU, (CRebar*)0);

	double	dCTCJongRebar	= (m_pDB->m_BindConc.m_dWidth-m_dCoverSd*2)/m_nQtyDivideJong;
	CDPoint vAngStt = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint vAngEnd = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);
	double	dStndCuttingAngle	= m_pDB->m_dGirEndStndGuttingAngle;
	BOOL	bSttCutAngle90		= (vAngStt.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90		= (vAngEnd.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bSttCutAngle90_Inv	= (vAngStt.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90_Inv	= (vAngEnd.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;

	for (long nG=0; nG<m_pDB->GetGirdersu(); nG++)
	{
		nRebarNo	= 1;
	
		//////////////////////////////////////////////////////////////////////////
		// 일반부 스트럽
		CRebar *pRB = new CRebar;
		xyArrShapeRebar.RemoveAll();
		sText.Format("스트럽일반_G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByStrupName(nG, sText);

		pRB->m_sDescription = sText;
		sText.Format("%d-%d", nRebarNo, nG+1);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 16;
		pRB->m_dDia			= m_dDiaStrup;
		pRB->m_dLengthMax_A	= Round(fabs(xyArrShapeRebar[3].x-xyArrShapeRebar[2].x), 0);
		pRB->m_dLengthMax_B	= Round(fabs(xyArrShapeRebar[2].y-xyArrShapeRebar[1].y), 0);
		pRB->m_dLengthMax_C	= Round(fabs(xyArrShapeRebar[1].x-xyArrShapeRebar[0].x), 0);
		pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2);
		
		pRB->m_nEa = m_arrTVectorPlan[nG].GetSize()-2;
		pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewonTU.Add(pRB);
			nRebarNo++;
		}

		//////////////////////////////////////////////////////////////////////////
		// 단부 스트럽 
		xyArrShapeRebar.RemoveAll();
		sText.Format("스트럽단부_G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByStrupName(nG, sText);
		double	dLen_W	= fabs(xyArrShapeRebar[2].x-xyArrShapeRebar[1].x);
		double	dLen_H	= fabs(xyArrShapeRebar[1].y-xyArrShapeRebar[0].y);

		//=====================================================
		//	시점
		if((bSttCutAngle90 || bSttCutAngle90_Inv))
		{
			// 좌측
			pRB = new CRebar;
			sText.Format("스트럽단부_G%d_시점1", nG+1);
			
			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 17;
			pRB->m_dDia			= m_dDiaStrup;
			pRB->m_dLengthMax_A	= Round(bSttCutAngle90 ? dLen_W/vAngStt.y/2 : dLen_W/2, 0);
			pRB->m_dLengthMax_B	= Round(dLen_H, 0);
			pRB->m_dLengthMax_C	= 100;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2);
			
			pRB->m_nEa = 1;
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
			
			// 우측
			pRB = new CRebar;
			sText.Format("스트럽단부_G%d_시점2", nG+1);
			
			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 17;
			pRB->m_dDia			= m_dDiaStrup;
			pRB->m_dLengthMax_A	= Round(bSttCutAngle90 ? dLen_W/2 : dLen_W/vAngStt.y/2, 0);
			pRB->m_dLengthMax_B	= Round(dLen_H, 0);
			pRB->m_dLengthMax_C	= 100;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2);
			
			pRB->m_nEa = 1;
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
		}
		else
		{
			pRB = new CRebar;
			xyArrShapeRebar.RemoveAll();
			sText.Format("스트럽단부_G%d_시점", nG+1);
			xyArrShapeRebar	= GetRebarShapeByStrupName(nG, sText);

			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 17;
			pRB->m_dDia			= m_dDiaStrup;
			pRB->m_dLengthMax_A	= Round(dLen_W, 0);
			pRB->m_dLengthMax_B	= Round(dLen_H, 0);
			pRB->m_dLengthMax_C	= 100;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2);
			
			pRB->m_nEa = 1;
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
		}
		
		//=====================================================
		//	종점
		if((bEndCutAngle90 || bEndCutAngle90_Inv))
		{
			// 좌측
			pRB = new CRebar;
			sText.Format("스트럽단부_G%d_종점1", nG+1);
			
			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 17;
			pRB->m_dDia			= m_dDiaStrup;
			pRB->m_dLengthMax_A	= Round(bEndCutAngle90 ? dLen_W/2 : dLen_W/vAngStt.y/2, 0);
			pRB->m_dLengthMax_B	= Round(dLen_H, 0);
			pRB->m_dLengthMax_C	= 100;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2);
			
			pRB->m_nEa = 1;
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
			
			// 우측
			pRB = new CRebar;
			sText.Format("스트럽단부_G%d_종점2", nG+1);
			
			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 17;
			pRB->m_dDia			= m_dDiaStrup;
			pRB->m_dLengthMax_A	= Round(bEndCutAngle90 ? dLen_W/vAngStt.y/2 : dLen_W/2, 0);
			pRB->m_dLengthMax_B	= Round(dLen_H, 0);
			pRB->m_dLengthMax_C	= 100;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2);
			
			pRB->m_nEa = 1;
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
		}
		else
		{
			pRB = new CRebar;
			xyArrShapeRebar.RemoveAll();
			sText.Format("스트럽단부_G%d_종점", nG+1);
			xyArrShapeRebar	= GetRebarShapeByStrupName(nG, sText);

			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 17;
			pRB->m_dDia			= m_dDiaStrup;
			pRB->m_dLengthMax_A	= Round(dLen_W, 0);
			pRB->m_dLengthMax_B	= Round(dLen_H, 0);
			pRB->m_dLengthMax_C	= 100;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2);
			
			pRB->m_nEa = 1;
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 종방향 단부측면철근
		pRB = new CRebar;
		
		sText.Format("측면철근단부_G%d", nG+1);
		pRB->m_sDescription = sText;
		sText.Format("%d-%d", nRebarNo, nG+1);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 4;
		pRB->m_dDia			= m_dDiaSide;
		pRB->m_dLengthMax_A	= Round(m_pDB->m_BindConc.m_dWidth-m_dCoverSd*2, 0);
		pRB->m_dLengthMax_B	= Round(m_dRangeDanbuBogang, 0);
		pRB->m_dLengthMax_C	= Round(m_dRangeDanbuBogang, 0);
		pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverSd, (m_pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)/2);
		
		pRB->m_nEa = 2;
		pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewonTU.Add(pRB);
			nRebarNo++;
		}

		//////////////////////////////////////////////////////////////////////////
		// 종방향 측면철근
		pRB = new CRebar;
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_좌측면G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistL	= Round(fabs(xyArrShapeRebar[1].x-xyArrShapeRebar[0].x)-m_dRangeDanbuBogang*2, 0);
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_우측면G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistR	= Round(fabs(xyArrShapeRebar[1].x-xyArrShapeRebar[0].x)-m_dRangeDanbuBogang*2, 0);
		
		sText.Format("측면철근일반_G%d", nG+1);
		pRB->m_sDescription = sText;
		sText.Format("%d-%d", nRebarNo, nG+1);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= m_dDiaSide;
		pRB->m_dLengthMax_A	= MAX(dDistL, dDistR);
		pRB->m_dLengthMin_A	= MIN(dDistL, dDistR);
		pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverSd, dCTCJongRebar/2);
		
		pRB->m_nEa = 2;
		pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewonTU.Add(pRB);
			nRebarNo++;
		}

		//////////////////////////////////////////////////////////////////////////
		// 종방향 하면철근
		pRB = new CRebar;
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_하부좌G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistL	= Round(fabs(xyArrShapeRebar[2].x-xyArrShapeRebar[1].x)-m_dRangeDanbuBogang*2, 0);
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_하부우G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistR	= Round(fabs(xyArrShapeRebar[2].x-xyArrShapeRebar[1].x)-m_dRangeDanbuBogang*2, 0);
		
		sText.Format("하면철근_G%d", nG+1);
		pRB->m_sDescription = sText;
		sText.Format("%d-%d", nRebarNo, nG+1);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 3;
		pRB->m_dDia			= m_dDiaLower;
		pRB->m_dLengthMax_A	= MAX(dDistL, dDistR);
		pRB->m_dLengthMin_A	= MIN(dDistL, dDistR);
		pRB->m_dLengthMax_B	= Round(fabs(xyArrShapeRebar[1].y-xyArrShapeRebar[0].y), 0);
//		pRB->m_dLengthMax_C	= Round(fabs(xyArrShapeRebar[3].y-xyArrShapeRebar[2].y), 0);
		pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, dCTCJongRebar/2);
		
		pRB->m_nEa = m_nQtyDivideJong%2==0 ? m_nQtyDivideJong-2 : m_nQtyDivideJong-1;
		pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewonTU.Add(pRB);
			nRebarNo++;
		}

		//////////////////////////////////////////////////////////////////////////
		// 종방향 모서리철근
		pRB = new CRebar;
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_좌상모서리G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistL	= Round(fabs(xyArrShapeRebar[1].x-xyArrShapeRebar[0].x)-m_dRangeDanbuBogang*2, 0);
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_우상모서리G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistR	= Round(fabs(xyArrShapeRebar[1].x-xyArrShapeRebar[0].x)-m_dRangeDanbuBogang*2, 0);
		
		sText.Format("모서리철근_G%d", nG+1);
		pRB->m_sDescription = sText;
		sText.Format("%d-%d", nRebarNo, nG+1);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= m_dDiaEdge;
		pRB->m_dLengthMax_A	= MAX(dDistL, dDistR);
		pRB->m_dLengthMin_A	= MIN(dDistL, dDistR);
		pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, dCTCJongRebar/2);
		
		pRB->m_nEa = 4;
		pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewonTU.Add(pRB);
			nRebarNo++;
		}

		//////////////////////////////////////////////////////////////////////////
		// 종방향 상면철근
		pRB = new CRebar;
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_상부좌G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistL	= Round(fabs(xyArrShapeRebar[1].x-xyArrShapeRebar[0].x)-m_dRangeDanbuBogang*2, 0);
		xyArrShapeRebar.RemoveAll();
		sText.Format("배력철근_상부우G%d", nG+1);
		xyArrShapeRebar	= GetRebarShapeByJongName(nG, sText);
		dDistR	= Round(fabs(xyArrShapeRebar[1].x-xyArrShapeRebar[0].x)-m_dRangeDanbuBogang*2, 0);
		
		sText.Format("상면철근_G%d", nG+1);
		pRB->m_sDescription = sText;
		sText.Format("%d-%d", nRebarNo, nG+1);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = 1;
		pRB->m_dDia			= m_dDiaUpper;
		pRB->m_dLengthMax_A	= MAX(dDistL, dDistR);
		pRB->m_dLengthMin_A	= MIN(dDistL, dDistR);
		pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverUp, dCTCJongRebar/2);
		
		pRB->m_nEa = m_nQtyDivideJong-1;
		pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewonTU.Add(pRB);
			nRebarNo++;
		}

		//////////////////////////////////////////////////////////////////////////
		// 시점부 사보강 철근
		if((bSttCutAngle90 || bSttCutAngle90_Inv))
		{
			// 모따기가 적용된 철근
			long nQtyRebar	= m_arrTVectorPlanSkewStt_Cut[nG][1].GetSize();
			if(nQtyRebar>0)
			{
				pRB = new CRebar;
				xyArrShapeRebar.RemoveAll();
						
				sText.Format("사보강철근시점1_G%d", nG+1);
				pRB->m_sDescription = sText;
				sText.Format("%d-%d", nRebarNo, nG+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;
				pRB->m_nTypeRebar   = 1;
				pRB->m_dDia			= m_dDiaSabong;
				pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanSkewStt_Cut[nG][1].GetAt(nQtyRebar-1).m_v1-m_arrTVectorPlanSkewStt_Cut[nG][1].GetAt(nQtyRebar-1).m_v2)), 0);
				pRB->m_dLengthMin_A	= Round(fabs(~(m_arrTVectorPlanSkewStt_Cut[nG][1].GetAt(0).m_v1-m_arrTVectorPlanSkewStt_Cut[nG][1].GetAt(0).m_v2)), 0);
				pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2/2);
				
				pRB->m_nEa = nQtyRebar;
				pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewonTU.Add(pRB);
					nRebarNo++;
				}
			}

			// 기존길이 유지된 철근
			nQtyRebar	= m_arrTVectorPlanSkewStt_Cut[nG][0].GetSize();
			if(nQtyRebar>0)
			{
				pRB = new CRebar;
				xyArrShapeRebar.RemoveAll();
						
				sText.Format("사보강철근시점2_G%d", nG+1);
				pRB->m_sDescription = sText;
				sText.Format("%d-%d", nRebarNo, nG+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;
				pRB->m_nTypeRebar   = 1;
				pRB->m_dDia			= m_dDiaSabong;
				pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanSkewStt_Cut[nG][0].GetAt(0).m_v1-m_arrTVectorPlanSkewStt_Cut[nG][0].GetAt(0).m_v2)), 0);
				pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2/2);
				
				pRB->m_nEa = nQtyRebar;
				pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewonTU.Add(pRB);
					nRebarNo++;
				}
			}
		}
		else
		{
			if(m_arrTVectorPlanSkewStt[nG].GetSize()>0)
			{
				pRB = new CRebar;
				xyArrShapeRebar.RemoveAll();
						
				sText.Format("사보강철근시점_G%d", nG+1);
				pRB->m_sDescription = sText;
				sText.Format("%d-%d", nRebarNo, nG+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;
				pRB->m_nTypeRebar   = 1;
				pRB->m_dDia			= m_dDiaSabong;
				pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanSkewStt[nG].GetAt(0).m_v1-m_arrTVectorPlanSkewStt[nG].GetAt(0).m_v2)), 0);
				pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2/2);
				
				pRB->m_nEa = m_arrTVectorPlanSkewStt[nG].GetSize();
				pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewonTU.Add(pRB);
					nRebarNo++;
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 종점부 사보강 철근	
		//	종점
		if((bEndCutAngle90 || bEndCutAngle90_Inv))
		{
			// 모따기가 적용된 철근
			long nQtyRebar	= m_arrTVectorPlanSkewEnd_Cut[nG][1].GetSize();
			if(nQtyRebar>0)
			{
				pRB = new CRebar;
				xyArrShapeRebar.RemoveAll();
						
				sText.Format("사보강철근종점1_G%d", nG+1);
				pRB->m_sDescription = sText;
				sText.Format("%d-%d", nRebarNo, nG+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;
				pRB->m_nTypeRebar   = 1;
				pRB->m_dDia			= m_dDiaSabong;
				pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanSkewEnd_Cut[nG][1].GetAt(nQtyRebar-1).m_v1-m_arrTVectorPlanSkewEnd_Cut[nG][1].GetAt(nQtyRebar-1).m_v2)), 0);
				pRB->m_dLengthMin_A	= Round(fabs(~(m_arrTVectorPlanSkewEnd_Cut[nG][1].GetAt(0).m_v1-m_arrTVectorPlanSkewEnd_Cut[nG][1].GetAt(0).m_v2)), 0);
				pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2/2);
				
				pRB->m_nEa = nQtyRebar;
				pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewonTU.Add(pRB);
					nRebarNo++;
				}
			}

			// 기존길이 유지된 철근
			nQtyRebar	= m_arrTVectorPlanSkewEnd_Cut[nG][0].GetSize();
			if(nQtyRebar>0)
			{
				pRB = new CRebar;
				xyArrShapeRebar.RemoveAll();
						
				sText.Format("사보강철근종점2_G%d", nG+1);
				pRB->m_sDescription = sText;
				sText.Format("%d-%d", nRebarNo, nG+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;
				pRB->m_nTypeRebar   = 1;
				pRB->m_dDia			= m_dDiaSabong;
				pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanSkewEnd_Cut[nG][0].GetAt(0).m_v1-m_arrTVectorPlanSkewEnd_Cut[nG][0].GetAt(0).m_v2)), 0);
				pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2/2);
				
				pRB->m_nEa = nQtyRebar;
				pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewonTU.Add(pRB);
					nRebarNo++;
				}
			}
		}
		else
		{
			if(m_arrTVectorPlanSkewEnd[nG].GetSize()>0)
			{
				pRB = new CRebar;
				xyArrShapeRebar.RemoveAll();
						
				sText.Format("사보강철근종점_G%d", nG+1);
				pRB->m_sDescription = sText;
				sText.Format("%d-%d", nRebarNo, nG+1);	
				pRB->m_sMark		= sText;
				pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
				pRB->m_bMainRebar   = TRUE;
				pRB->m_nTypeRebar   = 1;
				pRB->m_dDia			= m_dDiaSabong;
				pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanSkewEnd[nG].GetAt(0).m_v1-m_arrTVectorPlanSkewEnd[nG].GetAt(0).m_v2)), 0);
				pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
				pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, m_dCTCAnchor/2/2);
				
				pRB->m_nEa = m_arrTVectorPlanSkewEnd[nG].GetSize();
				pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
				if(pRB->m_nEa > 0)
				{
					m_arrRebarJewonTU.Add(pRB);
					nRebarNo++;
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		// Anchor 철근	
		pRB = new CRebar;
		xyArrShapeRebar.RemoveAll();
		sText.Format("앵커철근_G%d_1", nG+1);
		xyArrShapeRebar	= GetRebarShapeByStrupName(nG, sText);
		
		sText.Format("앵커철근_G%d", nG+1);
		pRB->m_sDescription = sText;
		sText.Format("%d-%d", nRebarNo, nG+1);	
		pRB->m_sMark		= sText;
		pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
		pRB->m_bMainRebar   = TRUE;
		pRB->m_nTypeRebar   = m_nQtyAnchor<2 ? 3 : 17;
		pRB->m_dDia			= m_dDiaAnchor;
		pRB->m_dLengthMax_A	= Round(fabs(xyArrShapeRebar[2].x-xyArrShapeRebar[1].x), 0);
		pRB->m_dLengthMax_B	= Round(fabs(xyArrShapeRebar[1].y-xyArrShapeRebar[0].y), 0);
		pRB->m_dLengthMax_C	= 100;
		pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
		pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, m_dCoverDn, (m_dCTCAnchor/m_nQtyAnchor)/2);
		
		pRB->m_nEa = long(GetRangeMainRebar(nG, FALSE)/m_dCTCAnchor)*m_nQtyAnchor;
		pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
		if(pRB->m_nEa > 0)
		{
			m_arrRebarJewonTU.Add(pRB);
			nRebarNo++;
		}

		//////////////////////////////////////////////////////////////////////////
		// 헌치 철근
		if(m_arrTVectorPlanHunchStt[nG].GetSize()>0)
		{
			pRB = new CRebar;
			xyArrShapeRebar.RemoveAll();
			sText.Format("헌치철근시점_G%d", nG+1);		
			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 1;
			pRB->m_dDia			= m_dDiaHunch;
			pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanHunchStt[nG][0].m_v1-m_arrTVectorPlanHunchStt[nG][0].m_v2)), 0);
			pRB->m_dLengthMax_B	= 0;
			pRB->m_dLengthMax_C	= 0;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, 0, 0);
			
			pRB->m_nEa = m_nQtyHunch*2;	// 복부판 좌우
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
		}
		if(m_arrTVectorPlanHunchEnd[nG].GetSize()>0)
		{
			pRB = new CRebar;
			xyArrShapeRebar.RemoveAll();
			sText.Format("헌치철근종점_G%d", nG+1);
			pRB->m_sDescription = sText;
			sText.Format("%d-%d", nRebarNo, nG+1);	
			pRB->m_sMark		= sText;
			pRB->m_Fy			= toKgPCm2(pDeck->m_SigmaY);	
			pRB->m_bMainRebar   = TRUE;
			pRB->m_nTypeRebar   = 1;
			pRB->m_dDia			= m_dDiaHunch;
			pRB->m_dLengthMax_A	= Round(fabs(~(m_arrTVectorPlanHunchEnd[nG][0].m_v1-m_arrTVectorPlanHunchEnd[nG][0].m_v2)), 0);
			pRB->m_dLengthMax_B	= 0;
			pRB->m_dLengthMax_C	= 0;
			pRB->m_dRateAdd		= pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
			pDeck->SetLengthRebar(pRB, JOINT_TENS_NORMAL_B, 0, 0);
			
			pRB->m_nEa = m_nQtyHunch*2;	// 복부판 좌우
			pRB->m_nIdxNumber = 5;	// 구속콘크리트철근
			if(pRB->m_nEa > 0)
			{
				m_arrRebarJewonTU.Add(pRB);
				nRebarNo++;
			}
		}
	}
	pDeck->m_SigmaCK = m_dCK;
}

double CADeckDataTU::GetRebarWeightTotal()
{
	double dTotal = 0;
	double dWeightRebar = 0; 
	double dWeightUnit  = 0; 
	
	for(long n=0; n<m_arrRebarJewonTU.GetSize(); n++)
	{
		CRebar* pRB = m_arrRebarJewonTU[n];
		dWeightUnit  = GetRebarInfo((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dWeightRebar = dWeightUnit * toM(pRB->GetLength()*pRB->m_nEa);
		
		dTotal += dWeightRebar;
	}

	return dTotal;
}

double CADeckDataTU::GetAreaSlab()
{
	CPlateGirderApp		*pGir		= m_pDB->GetGirder(-1);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt		= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd		= Finder.GetBxFirst(BX_END_GIRDER);
	CLineInfo			*pGirLine	= GetLineInfo(-1);

	// 구속콘크리트 시점, 종점점
	CDPoint vAngSlabStt		= pGir->GetAAA(OFF_STT_SLAB);	// 슬래브시점각도
	CDPoint vAngSlabEnd		= pGir->GetAAA(OFF_END_SLAB);	// 슬래브시점각도

	double	dStaSttBindConc	= pGirLine->GetStationByScInLine(pBxStt->GetStation()-m_pDB->m_dLengthTUBindConc_Stt, 0, vAngSlabStt);
	double	dStaEndBindConc	= dStaSttBindConc + pGir->GetLengthDimType(pBxStt->GetStation()-m_pDB->m_dLengthTUBindConc_Stt, pBxEnd->GetStation()+m_pDB->m_dLengthTUBindConc_End);

	double	dAreaSlab	= (dStaEndBindConc-dStaSttBindConc) * m_pDB->m_BindConc.m_dWidth;

	return dAreaSlab;	// mm^2

}

BOOL CADeckDataTU::AddRebar()
{
	CADeckData		*pDeck	= m_pDataManage->GetDeckData();

	m_nAddRebarsuTU++;

	 CRebar* pRB = new CRebar;

	pRB->m_Fy 		  = toKgPCm2(pDeck->m_SigmaY);	
	pRB->m_bMainRebar = TRUE;
	pRB->m_nTypeRebar = 1;
	pRB->m_dDia = 16;
	pRB->m_dRateAdd = pDeck->m_AddRateRebar.GetOneAddRate(pRB->m_dDia, pRB->m_bMainRebar);
	pRB->m_nIdxNumber = 5;
	pRB->m_sDescription = "추가철근";
	pRB->m_sMark.Format("ADD%d", m_nAddRebarsuTU);

	m_arrRebarJewonTU.Add(pRB);

	return TRUE;
}

void CADeckDataTU::MakeRebarMarkPosPlanTU(long nG, double dScale)
{
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CADeckData		*pDeck	= m_pDataManage->GetDeckData();

	CDPointArray xyArrMarkPosStt, xyArrMarkPosEnd;
	
	pair<CString, CDPointArray> vPair;
	CString sText;

	//////////////////////////////////////////////////////////////////////////
	// 스트럽철근일반
	double	dCTC	= (pDB->m_BindConc.m_dWidth - m_dCoverSd*2) / m_nQtyDivideJong;
	CDPointArray xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlan[nG], dCTC*(m_nQtyDivideJong-1+0.5), 0.5, FALSE);
	long	nSize	= xyArrMark.GetSize();
	long	n		= 0;
	for(n=1; n<long(nSize*0.15); n++)
	{
		xyArrMarkPosStt.Add(xyArrMark.GetAt(n));
		xyArrMarkPosEnd.Add(xyArrMark.GetAt(xyArrMark.GetSize()-1-n));
	}
	
	sText.Format("스트럽일반_G%d_시점상", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosStt;
	m_arrMarkPosTU[nG].push_back(vPair);

	sText.Format("스트럽일반_G%d_종점상", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosEnd;
	m_arrMarkPosTU[nG].push_back(vPair);

	xyArrMarkPosStt.RemoveAll();
	xyArrMarkPosEnd.RemoveAll();
	xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlan[nG], dCTC*0.5, 0.5, FALSE);
	
	nSize	= xyArrMark.GetSize();
	for(n=1; n<long(nSize*0.15); n++)
	{
		xyArrMarkPosStt.Add(xyArrMark.GetAt(n));
		xyArrMarkPosEnd.Add(xyArrMark.GetAt(xyArrMark.GetSize()-1-n));
	}
	
	sText.Format("스트럽일반_G%d_시점하", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosStt;
	m_arrMarkPosTU[nG].push_back(vPair);

	sText.Format("스트럽일반_G%d_종점하", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosEnd;
	m_arrMarkPosTU[nG].push_back(vPair);

	//////////////////////////////////////////////////////////////////////////
	// 스트럽철근단부
	xyArrMarkPosStt.RemoveAll();
	xyArrMarkPosEnd.RemoveAll();

	CDPoint vAngStt = m_pDB->GetGirder(-1)->GetAAA(OFF_STT_SLAB);
	CDPoint vAngEnd = m_pDB->GetGirder(-1)->GetAAA(OFF_END_SLAB);
	double	dStndCuttingAngle	= m_pDB->m_dGirEndStndGuttingAngle;
	BOOL	bSttCutAngle90		= (vAngStt.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90		= (vAngEnd.GetAngleDegree()<=dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bSttCutAngle90_Inv	= (vAngStt.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;
	BOOL	bEndCutAngle90_Inv	= (vAngEnd.GetAngleDegree()>=180-dStndCuttingAngle) ? TRUE : FALSE;

	//=====================================================
	//	시점
	if((bSttCutAngle90 || bSttCutAngle90_Inv))
	{
		xyArrMarkPosStt.RemoveAll();
		xyArrMark	= GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewStt_Cut[nG][1], dCTC/2, 0.5, bSttCutAngle90);
		xyArrMarkPosStt.Add(xyArrMark.GetAt(0));
		sText.Format("스트럽단부_G%d_시점1", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMarkPosStt;
		m_arrMarkPosTU[nG].push_back(vPair);

		xyArrMarkPosStt.RemoveAll();
		xyArrMark	= GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewStt_Cut[nG][2], dCTC/2, 0.5, !bSttCutAngle90);
		xyArrMarkPosStt.Add(xyArrMark.GetAt(0));
		sText.Format("스트럽단부_G%d_시점2", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMarkPosStt;
		m_arrMarkPosTU[nG].push_back(vPair);
	}
	else
	{
		if(pDeck->IsSkewStt())
			xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewStt[nG], dCTC/2, 0.5, TRUE);
		else
			xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlan[nG], dCTC/2, 0.5, TRUE);
		xyArrMarkPosStt.Add(xyArrMark.GetAt(0));

		sText.Format("스트럽단부_G%d_시점", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMarkPosStt;
		m_arrMarkPosTU[nG].push_back(vPair);
	}

	//=====================================================
	//	종점
	if((bEndCutAngle90 || bEndCutAngle90_Inv))
	{
		xyArrMarkPosEnd.RemoveAll();
		xyArrMark	= GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewEnd_Cut[nG][1], dCTC/2, 0.5, !bEndCutAngle90);
		xyArrMarkPosEnd.Add(xyArrMark.GetAt(0));
		sText.Format("스트럽단부_G%d_종점1", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMarkPosEnd;
		m_arrMarkPosTU[nG].push_back(vPair);

		xyArrMarkPosEnd.RemoveAll();
		xyArrMark	= GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewEnd_Cut[nG][2], dCTC/2, 0.5, bEndCutAngle90);
		xyArrMarkPosEnd.Add(xyArrMark.GetAt(0));
		sText.Format("스트럽단부_G%d_종점2", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMarkPosEnd;
		m_arrMarkPosTU[nG].push_back(vPair);
	}
	else
	{
		if(pDeck->IsSkewEnd())
		{
			xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewEnd[nG], dCTC/2, 0.5, TRUE);
			xyArrMarkPosEnd.Add(xyArrMark.GetAt(0));
		}
		else
		{
			xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlan[nG], dCTC/2, 0.5, TRUE);
			xyArrMarkPosEnd.Add(xyArrMark.GetAt(xyArrMark.GetSize()-1));
		}
		
		sText.Format("스트럽단부_G%d_종점", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMarkPosEnd;
		m_arrMarkPosTU[nG].push_back(vPair);
	}

	//////////////////////////////////////////////////////////////////////////
	// 하부철근
	long	nQtyRebar	 = (m_nQtyDivideJong%2==0) ? m_nQtyDivideJong/2-1 : (m_nQtyDivideJong+1)/2-1;
	long	nTargetRebar = long(nSize*0.25);
	xyArrMarkPosStt.RemoveAll();
	xyArrMarkPosEnd.RemoveAll();
	
	//	시점
	xyArrMark	= GetXyArrPosByTwinVectorAndDist(m_arrTVectorPlan[nG].GetAt(nTargetRebar), m_arrTVectorPlan[nG].GetAt(nTargetRebar+1), dCTC);
	
	for(n=0; n<nQtyRebar; n++)
		xyArrMarkPosStt.Add(xyArrMark.GetAt(nQtyRebar-n));
	
	sText.Format("하부철근_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosStt;
	m_arrMarkPosTU[nG].push_back(vPair);

	//	종점
	xyArrMark	= GetXyArrPosByTwinVectorAndDist(m_arrTVectorPlan[nG].GetAt(nSize-nTargetRebar-1), m_arrTVectorPlan[nG].GetAt(nSize-nTargetRebar-2), dCTC);
	
	for(n=0; n<nQtyRebar; n++)
		xyArrMarkPosEnd.Add(xyArrMark.GetAt(nQtyRebar-n));
	
	sText.Format("하부철근_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosEnd;
	m_arrMarkPosTU[nG].push_back(vPair);

	//////////////////////////////////////////////////////////////////////////
	// 상부철근
	xyArrMarkPosStt.RemoveAll();
	xyArrMarkPosEnd.RemoveAll();
	
	//	시점
	xyArrMark	= GetXyArrPosByTwinVectorAndDist(m_arrTVectorPlan[nG].GetAt(nTargetRebar-1), m_arrTVectorPlan[nG].GetAt(nTargetRebar), dCTC);
	
	for(n=0; n<nQtyRebar; n++)
		xyArrMarkPosStt.Add(xyArrMark.GetAt(xyArrMark.GetSize()-1-nQtyRebar+n));
	
	sText.Format("상부철근_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosStt;
	m_arrMarkPosTU[nG].push_back(vPair);

	//	종점
	xyArrMark	= GetXyArrPosByTwinVectorAndDist(m_arrTVectorPlan[nG].GetAt(nSize-nTargetRebar), m_arrTVectorPlan[nG].GetAt(nSize-nTargetRebar-1), dCTC);
	
	for(n=0; n<nQtyRebar; n++)
		xyArrMarkPosEnd.Add(xyArrMark.GetAt(xyArrMark.GetSize()-1-nQtyRebar+n));
	
	sText.Format("상부철근_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosEnd;
	m_arrMarkPosTU[nG].push_back(vPair);

	//////////////////////////////////////////////////////////////////////////
	// 모서리철근
	xyArrMarkPosStt.RemoveAll();
	xyArrMarkPosEnd.RemoveAll();
	nTargetRebar = long(nSize*0.07);
	
	//	시점
	xyArrMark	= GetXyArrPosByTwinVectorAndDist(m_arrTVectorPlan[nG].GetAt(nTargetRebar), m_arrTVectorPlan[nG].GetAt(nTargetRebar+1), dCTC);
	
	xyArrMarkPosStt.Add(xyArrMark.GetAt(0));
	xyArrMarkPosStt.Add(xyArrMark.GetAt(xyArrMark.GetSize()-1));
	
	sText.Format("모서리철근_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosStt;
	m_arrMarkPosTU[nG].push_back(vPair);

	//	종점
	xyArrMark	= GetXyArrPosByTwinVectorAndDist(m_arrTVectorPlan[nG].GetAt(nSize-nTargetRebar), m_arrTVectorPlan[nG].GetAt(nSize-nTargetRebar-1), dCTC);
	
	xyArrMarkPosEnd.Add(xyArrMark.GetAt(0));
	xyArrMarkPosEnd.Add(xyArrMark.GetAt(xyArrMark.GetSize()-1));
	
	sText.Format("모서리철근_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMarkPosEnd;
	m_arrMarkPosTU[nG].push_back(vPair);

	//////////////////////////////////////////////////////////////////////////
	// 구속콘크리트 헌치부 철근
	//	시점 
	xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanHunchStt[nG], -m_dOutLength, 2, !bSttCutAngle90);			
	sText.Format("헌치철근시점_G%d", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMark;
	m_arrMarkPosTU[nG].push_back(vPair);

	xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanHunchEnd[nG], -m_dOutLength, 2, !bSttCutAngle90);
	sText.Format("헌치철근종점_G%d", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrMark;
	m_arrMarkPosTU[nG].push_back(vPair);

	//////////////////////////////////////////////////////////////////////////
	// 사보강철근
	//	시점
	if((bSttCutAngle90 || bSttCutAngle90_Inv))
	{
		xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewStt_Cut[nG][1], dCTC*5/2, 0.5, bSttCutAngle90);
			
		sText.Format("사보강철근_G%d_시점1", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMark;
		m_arrMarkPosTU[nG].push_back(vPair);

		xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewStt_Cut[nG][0], dCTC*5/2, 0.5, !bSttCutAngle90);
			
		sText.Format("사보강철근_G%d_시점2", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMark;
		m_arrMarkPosTU[nG].push_back(vPair);
	}
	else
	{
		xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewStt[nG], dCTC*3/2, 0.5, FALSE);
			
		sText.Format("사보강철근_G%d_시점", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMark;
		m_arrMarkPosTU[nG].push_back(vPair);
	}
	//=====================================================
	//	종점
	if((bEndCutAngle90 || bEndCutAngle90_Inv))
	{
		xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewEnd_Cut[nG][1], dCTC*5/2, 0.5, !bEndCutAngle90);
			
		sText.Format("사보강철근_G%d_종점1", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMark;
		m_arrMarkPosTU[nG].push_back(vPair);

		xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewEnd_Cut[nG][0], dCTC*5/2, 0.5, bEndCutAngle90);
			
		sText.Format("사보강철근_G%d_종점2", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMark;
		m_arrMarkPosTU[nG].push_back(vPair);
	}
	else
	{
		xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlanSkewEnd[nG], dCTC*3/2, 0.5, FALSE);
		
		sText.Format("사보강철근_G%d_종점", nG+1);
		vPair.first		= sText;
		vPair.second	= xyArrMark;
		m_arrMarkPosTU[nG].push_back(vPair);
	}
}

// 두개의 TwinVector 사이 중심라인에서 CTC간격의 좌표들을 구함
CDPointArray CADeckDataTU::GetXyArrPosByTwinVectorAndDist(CTwinVector tVector1, CTwinVector tVector2, double dCTC)
{
	CTwinVector tvCen;
	tvCen.m_v1	= (tVector1.m_v1+tVector2.m_v1)/2;
	tvCen.m_v2	= (tVector1.m_v2+tVector2.m_v2)/2;
	CDPoint vTVector = (tvCen.m_v1-tvCen.m_v2).Unit();
	double	dLen = ABS(~(tvCen.m_v1-tvCen.m_v2));
	CDPoint xyPos = tvCen.m_v2;
	CDPointArray	xyArr;
	double	dDist	= 0;

	while(ABS(~(tvCen.m_v2-xyPos))<dLen)
	{
		xyPos = tvCen.m_v2 + dDist*vTVector;
		xyArr.Add(xyPos);
		dDist += dCTC;
	}
	return xyArr;
}


CDPointArray CADeckDataTU::GetMarkPosArrTU(long nG, CTwinVectorArray *tvArrPlane, double dCTC, double dRatio, BOOL bUpper)
{
	CADeckData		*pDeck	= m_pDataManage->GetDeckData();
	CPlateBridgeApp	*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);

	CDPointArray xyReturn;
	if(tvArrPlane->GetSize()==0)	return xyReturn;

	if(bUpper)
	{
		double dDistLeft = pGir->m_dGirderCenter - pDB->m_BindConc.m_dWidth/2 + m_dCoverSd;
		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			xyReturn.Add(pDeck->GetXyPosByTwinVectorAndDist(tVector, dDistLeft+dCTC));
		}
	}
	else
	{
		double dDistRigh = pGir->m_dGirderCenter + pDB->m_BindConc.m_dWidth/2 - m_dCoverSd;
		for(long n=0; n<tvArrPlane->GetSize(); n++)
		{
			CTwinVector tVector = tvArrPlane->GetAt(n);
			xyReturn.Add(pDeck->GetXyPosByTwinVectorAndDist(tVector, dDistRigh-dCTC));
		}
	}

	return xyReturn;
}

CDPointArray CADeckDataTU::GetArrMarkPosByNameTU(long nG, CString szName)
{
	CDPointArray xyArrRebar;
	for(long n=0; n<(long)m_arrMarkPosTU[nG].size(); n++)
	{
		if(szName == m_arrMarkPosTU[nG].at(n).first)
			xyArrRebar = m_arrMarkPosTU[nG].at(n).second;
	}
	return xyArrRebar;
}

CRebar* CADeckDataTU::GetRebarByName(CString szName)
{
	for(long n=0; n<m_arrRebarJewonTU.GetSize(); n++)
	{
		if(szName == m_arrRebarJewonTU[n]->m_sDescription)
			return m_arrRebarJewonTU[n];
	}
	return NULL;
}

void CADeckDataTU::MakeRebarMarkPosJongTU(long nG, double dScale)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir	= pDB->GetGirder(nG);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBxStt	= Finder.GetBxFirst(BX_STT_GIRDER);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxFirst(BX_END_GIRDER);
	CLineInfo			*pGirLine	= GetLineInfo(nG);

	CRebarPlacing RebarPlace;
	CDPointArray xyArrMarkPosStt, xyArrMarkPosEnd;
	
	pair<CString, CDPointArray> vPair;
	CString sText;

	CDPoint vAngSlabStt		= pGir->GetAAA(OFF_STT_SLAB);	// 슬래브시점각도
	CDPoint vAngSlabEnd		= pGir->GetAAA(OFF_END_SLAB);	// 슬래브시점각도

	double	dStaSttBindConc	= pGirLine->GetStationByScInLine(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt, 0, vAngSlabStt);
	double	dStaEndBindConc	= dStaSttBindConc + pGir->GetLengthDimType(pBxStt->GetStation()-pDB->m_dLengthTUBindConc_Stt, pBxEnd->GetStation()+pDB->m_dLengthTUBindConc_End);

	double	dStaSttAnchor	= dStaSttBindConc + m_dCoverDanbu + GetLengthSkewTriangle(nG, TRUE);
	double	dStaEndAnchor	= dStaEndBindConc - m_dCoverDanbu - GetLengthSkewTriangle(nG, FALSE);
	
	double	dXSttAnchor	= dStaSttAnchor;
	double	dYSttAnchor	= pGir->GetElevationFlangeCamber(pBxStt->GetStation()) - pDB->m_BindConc.m_dDis;
	
	double	dX	= dXSttAnchor;
	double	dY	= dYSttAnchor + m_dCoverDn;

	double	dDanbuRebarH	= pDB->m_BindConc.m_dHeight - m_dCoverDn - m_dCoverUp;
	
	double	dCTC	= (pDB->m_BindConc.m_dWidth - m_dCoverSd*2) / m_nQtyDivideJong;
	CDPointArray xyArrMark = GetMarkPosArrTU(nG, &m_arrTVectorPlan[nG], dCTC/2, 0.5, FALSE);
	long	nSize	= xyArrMark.GetSize();
	long	nTargetRebar = long(nSize*0.1);
	
	long	nGugansu		= m_arrCTC[nG].GetSize();
	CDPointArray xyArrTemp, xyArrRebar, xyArrStrup, xyArrHunch;
	CDoubleArray dArrCtc;
	BOOL	bDanbuBogang	= FALSE;
	BOOL	bFirst	= TRUE;
	long	nCount	= 0;
	long	nGugan	= 0;
	CDPoint	xySideGen(0,0), xySideDan(0,0);
	
	// 시점부 철근
	xyArrStrup.RemoveAll();
	for(nGugan=0; nGugan<nGugansu; nGugan++)
	{
		xyArrTemp.RemoveAll();
		xyArrTemp.Add(m_arrCTC[nG].GetAt(nGugan));
		RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrTemp, dArrCtc, FALSE);
		
		bDanbuBogang	= (dArrCtc.GetAt(0)==m_dCTCAnchor/2) ? TRUE : FALSE;
		for(long n=0; n<dArrCtc.GetSize(); n++)
		{
			dX	+= dArrCtc.GetAt(n);
			if(!bFirst && nCount<nTargetRebar)
			{
				xyArrRebar.Add(CDPoint(dX, dY+dDanbuRebarH+m_dCoverUp/2));
			}
			
			if(nGugan==0 && n==0)
			{
				xySideDan	= CDPoint(dX-dArrCtc.GetAt(n), dY+dDanbuRebarH/2);
				bFirst	= FALSE;
			}

			if(nCount>=nTargetRebar*2 && nCount<=nTargetRebar*2+2)
			{
				xyArrStrup.Add(CDPoint(dX, dY-REBAR_SYMBOL_RADIUS));
				xyArrStrup.Add(CDPoint(dX, dY+dDanbuRebarH+REBAR_SYMBOL_RADIUS));
			}

			if(nCount==nTargetRebar*3)
			{
				xySideGen	= CDPoint(dX+m_dCTCAnchor/2, dY+dDanbuRebarH/2);
			}
			
			nCount++;
		}
	}

	dY = pGir->GetElevationDomForExtLine(pBxStt) + m_dCoverHunch;

	long nHunch = 0;
	for(nHunch=0; nHunch<min(m_nQtyHunch,3); nHunch++)
	{
		dX = GetStaMainRebar(TRUE, nG, FALSE)+m_dDisHunch*nHunch;
		xyArrHunch.Add(CDPoint(dX, dY));
	}
	sText.Format("헌치철근종단_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrHunch;
	m_arrMarkPosTU[nG].push_back(vPair);
	
	sText.Format("스트럽일반_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrStrup;
	m_arrMarkPosTU[nG].push_back(vPair);

	sText.Format("앵커철근_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	xyArrTemp.RemoveAll();
	xyArrTemp.Add(xySideGen);
	sText.Format("측면철근일반_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrTemp;
	m_arrMarkPosTU[nG].push_back(vPair);

	xyArrTemp.RemoveAll();
	xyArrTemp.Add(xySideDan);
	sText.Format("측면철근단부_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrTemp;
	m_arrMarkPosTU[nG].push_back(vPair);
	
	// 종점부 철근
	xyArrRebar.RemoveAll();
	xyArrStrup.RemoveAll();
	xyArrHunch.RemoveAll();
	bDanbuBogang	= FALSE;
	bFirst	= FALSE;
	nCount	= 0;
	dX		= dStaEndBindConc - m_dCoverDanbu - GetLengthSkewTriangle(nG, FALSE);
	
	for(nGugan=nGugansu-1; nGugan>=0; nGugan--)
	{
		xyArrTemp.RemoveAll();
		xyArrTemp.Add(m_arrCTC[nG].GetAt(nGugan));
		RebarPlace.ConvertOffsetPointArrayToOffsetDoubleArray(xyArrTemp, dArrCtc, FALSE);
		
		bDanbuBogang	= (dArrCtc.GetAt(0)==m_dCTCAnchor/2) ? TRUE : FALSE;
		for(long n=0; n<dArrCtc.GetSize(); n++)
		{
			dX	-= dArrCtc.GetAt(n);

			if(!bFirst && nCount<nTargetRebar)
			{
				xyArrRebar.Add(CDPoint(dX, dY+dDanbuRebarH+m_dCoverUp/2));
			}

			if(nGugan==nGugansu-1 && n==0)
			{
				xySideDan	= CDPoint(dX+dArrCtc.GetAt(n), dY+dDanbuRebarH/2);
				bFirst	= FALSE;
			}

			if(nCount>=nTargetRebar*2 && nCount<=nTargetRebar*2+2)
			{
				xyArrStrup.Add(CDPoint(dX, dY-REBAR_SYMBOL_RADIUS));
				xyArrStrup.Add(CDPoint(dX, dY+dDanbuRebarH+REBAR_SYMBOL_RADIUS));
			}
			if(nCount==nTargetRebar*3)
			{
				xySideGen	= CDPoint(dX+m_dCTCAnchor/2, dY+dDanbuRebarH/2);
			}
			nCount++;
		}
	}

	dY = pGir->GetElevationDomForExtLine(pBxEnd) + m_dCoverHunch;
	for(nHunch=0; nHunch<min(m_nQtyHunch,3); nHunch++)
	{
		dX = GetStaMainRebar(FALSE, nG, FALSE)-m_dDisHunch*nHunch;
		xyArrHunch.Add(CDPoint(dX, dY));
	}
	sText.Format("헌치철근종단_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrHunch;
	m_arrMarkPosTU[nG].push_back(vPair);
	
	sText.Format("스트럽일반_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrStrup;
	m_arrMarkPosTU[nG].push_back(vPair);

	sText.Format("앵커철근_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	xyArrTemp.RemoveAll();
	xyArrTemp.Add(xySideGen);
	sText.Format("측면철근일반_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrTemp;
	m_arrMarkPosTU[nG].push_back(vPair);
	
	xyArrTemp.RemoveAll();
	xyArrTemp.Add(xySideDan);
	sText.Format("측면철근단부_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrTemp;
	m_arrMarkPosTU[nG].push_back(vPair);

/*	//////////////////////////////////////////////////////////////////////////
	// 단부 스트럽
	sText.Format("배력철근_하부거더중간G%d", nG+1);
	xyArrTemp	= GetRebarShapeByJongName(nG, sText);
	xyArrRebar.RemoveAll();
	xyArrRebar.Add(CDPoint(xyArrTemp.GetAt(1)+CDPoint(0,1)*(pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)/4));
	sText.Format("스트럽단부_G%d_시점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	xyArrRebar.RemoveAll();
	xyArrRebar.Add(CDPoint(xyArrTemp.GetAt(2)+CDPoint(0,1)*(pDB->m_BindConc.m_dHeight-m_dCoverUp-m_dCoverDn)/4));
	sText.Format("스트럽단부_G%d_종점", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);
*/
}

void CADeckDataTU::MakeRebarMarkPosCrossTU(long nG, double dScale)
{
	CPlateBridgeApp		*pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp		*pGir	 = pDB->GetGirder(nG);
	
	double	dBindW		= pDB->m_BindConc.m_dWidth;
	double	dBindH		= pDB->m_BindConc.m_dHeight;
	double	dBindS1		= pDB->m_BindConc.m_dDeep;
	double	dBindS2		= pDB->m_BindConc.m_dDis;
	double	dSlabLowH	= -pDB->m_dThickSlabBase;		// 슬래브 하면의 높이
		
	double	dGirderCenter	= pGir->m_dGirderCenter;

	long	nQtyDivideJong	= m_nQtyDivideJong;	
	double	dCTC	= (dBindW-m_dCoverSd*2)/nQtyDivideJong;

	CDPointArray xyArrTemp, xyArrRebar;
	CDoubleArray dArrCtc;
	CDPoint	xyStrup(0,0), xySideGen(0,0), xySideDan(0,0);
	
	pair<CString, CDPointArray> vPair;
	CString sText;
	
	// 시점부 철근
	sText.Format("스트럽일반_G%d", nG+1);
	xyArrTemp	= GetRebarShapeByStrupName(nG, sText);
	xyArrRebar.RemoveAll();
	xyArrRebar.Add(CDPoint(xyArrTemp.GetAt(1)+CDPoint(1,0)*dCTC/2));
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	// 앵커철근
	sText.Format("앵커철근_G%d_1", nG+1);
	xyArrTemp	= GetRebarShapeByStrupName(nG, sText);
	xyArrRebar.RemoveAll();
	xyArrRebar.Add(CDPoint((xyArrTemp.GetAt(1)+xyArrTemp.GetAt(2))/2));
	sText.Format("앵커철근_G%d", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	// 헌치철근
	sText.Format("헌치철근횡단_G%d", nG+1);	
	xyArrRebar.RemoveAll();
	xyArrRebar.Add(CDPoint(dGirderCenter-dBindW/2-m_dOutLength+m_dCoverSd, dSlabLowH+dBindS1-dBindH+m_dCoverHunch));
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	// 상면철근
	CDPoint	xyPos(dGirderCenter+dBindW/2-m_dCoverSd, dSlabLowH+dBindS1-m_dCoverUp-REBAR_SYMBOL_RADIUS);
	CDPoint	vX(1,0), vY(0,1);
	xyArrRebar.RemoveAll();

	long n = 0;
	for(n=0; n<3; n++)
	{
		xyPos	-= vX*dCTC;
		xyArrRebar.Add(xyPos);
	}
	sText.Format("상면철근_G%d", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	// 하면철근
	xyArrRebar.RemoveAll();
	xyPos	= CDPoint(dGirderCenter+dBindW/2-m_dCoverSd, dSlabLowH+dBindS1-dBindH+m_dCoverDn+REBAR_SYMBOL_RADIUS);
	for(n=0; n<3; n++)
	{
		xyPos	-= vX*dCTC;

		// 짝수구간일 경우 web으로 인해 중앙철근 배근 불가능
		if(nQtyDivideJong%2==0 && n==nQtyDivideJong/2.-1)	continue;
		xyArrRebar.Add(xyPos);
	}
	sText.Format("하면철근_G%d", nG+1);
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);

	// 측면(좌)철근
	sText.Format("측면철근_G%d", nG+1);
	xyArrRebar.RemoveAll();
	xyArrRebar.Add(CDPoint(dGirderCenter-dBindW/2+m_dCoverSd+REBAR_SYMBOL_RADIUS, dSlabLowH+dBindS1-m_dCoverUp-(dBindH-m_dCoverUp-m_dCoverDn)/2));
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);
	
	// 모서리(우)철근
	sText.Format("모서리철근_G%d", nG+1);
	xyArrRebar.RemoveAll();
	xyArrRebar.Add(CDPoint(dGirderCenter+dBindW/2-m_dCoverSd-REBAR_SYMBOL_RADIUS, dSlabLowH+dBindS1-dBindH+m_dCoverDn+REBAR_SYMBOL_RADIUS));
	xyArrRebar.Add(CDPoint(dGirderCenter+dBindW/2-m_dCoverSd+(dBindH-m_dCoverUp-m_dCoverDn)-REBAR_SYMBOL_RADIUS*3, dSlabLowH+dBindS1-dBindH+m_dCoverDn+REBAR_SYMBOL_RADIUS));
	vPair.first		= sText;
	vPair.second	= xyArrRebar;
	m_arrMarkPosTU[nG].push_back(vPair);
}

// 스트럽, 헌치철근을 위한 최대복부판 두께
double CADeckDataTU::GetMaxWebT(long nG)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp *pGir	= pDB->GetGirder(nG);
	
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx	= Finder.GetBxFirst(BX_WEB);

	double dMaxWebT = 0;
	while(pBx)
	{
		dMaxWebT = max(dMaxWebT, pGir->GetThickFactByStation(pBx->GetStation(), G_W));

		pBx = Finder.GetBxNext();
	}

	return dMaxWebT;
}
