// CentSeparation.cpp: implementation of the CCentSeparation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CentSeparation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCentSeparation::CCentSeparation()
{
	m_strName = _T("");
	
	m_D1 = 0;
	m_W1 = 0;
	m_W2 = 0;
	m_W3 = 0;
	m_W4 = 0;
	m_W5 = 0;
	m_H1 = 0;
	m_H2 = 0;
	m_H3 = 0;
	m_nDirType = -1;
	m_nSepType = -1;

	m_pRebar = new CGuardFenceRebar;
}


CCentSeparation::~CCentSeparation()
{
	if(m_pRebar)
		delete m_pRebar;
}



void CCentSeparation::Serialize( CArchive& ar )
{
	long nFlag = 2;
	if( ar.IsStoring() )
	{
		ar << nFlag;
		ar << m_D1;
		ar << m_W1;
		ar << m_W2;
		ar << m_W3;
		ar << m_W4;
		ar << m_W5;
		ar << m_H1;
		ar << m_H2;
		ar << m_H3;
		ar << m_strName;
		ar << m_nDirType;
		ar << m_nSepType;
		m_pRebar->Serialize(ar);

	}
	else {
		ar >> nFlag;
		ar >> m_D1;
		ar >> m_W1;
		ar >> m_W2;
		ar >> m_W3;
		ar >> m_W4;
		ar >> m_W5;
		ar >> m_H1;
		ar >> m_H2;
		ar >> m_H3;
		if (nFlag > 0)
		{
			ar >> m_strName;
			ar >> m_nDirType;
			ar >> m_nSepType;
		}
		if(nFlag > 1)
			m_pRebar->Serialize(ar);		
		
	}
}

void CCentSeparation::SerializeOld( CArchive& ar )
{
	long nFlag = 0;
	if( ar.IsStoring() )
	{
		ar << nFlag;
		ar << m_D1;
		ar << m_W1;
		ar << m_W2;
		ar << m_W3;
		ar << m_W4;
		ar << m_W5;
		ar << m_H1;
		ar << m_H2;
		ar << m_H3;
	}
	else {
		ar >> nFlag;
		ar >> m_D1;
		ar >> m_W1;
		ar >> m_W2;
		ar >> m_W3;
		ar >> m_W4;
		ar >> m_W5;
		ar >> m_H1;
		ar >> m_H2;
		ar >> m_H3;
	}
}

CCentSeparation& CCentSeparation::operator=(const CCentSeparation& obj)
{
	m_D1 = obj.m_D1;
	m_W1 = obj.m_W1;
	m_W2 = obj.m_W2;
	m_W3 = obj.m_W3;
	m_W4 = obj.m_W4;
	m_W5 = obj.m_W5;
	m_H1 = obj.m_H1;
	m_H2 = obj.m_H2;
	m_H3 = obj.m_H3;
	m_nDirType = obj.m_nDirType;
	m_nSepType = obj.m_nSepType;
	m_strName = obj.m_strName;
	*m_pRebar = *obj.m_pRebar;

	return *this;
}

inline long CCentSeparation::GetPlace()
{

	if(m_nDirType == -1) return LEFT;
	if(m_nDirType ==  1) return RIGHT;
	return CENTER;
}

inline double CCentSeparation::GetHeight() const
{
	return m_H1+m_H2+m_H3;
}

inline double CCentSeparation::GetWidth() const
{
	return m_W1+m_W2+m_W3+m_W4+m_W5;
}


//void CCentSeparation::SetInitData()
//{
//
//}

void CCentSeparation::SetSeperationType(CString str)
{
	m_strName = str;

	if(m_strName == "방호벽(좌)")
	{
		m_nSepType = SLAB_GUARD_LEFT;
		m_nDirType = -1;
	}
	else if(m_strName == "방호벽(우)")
	{
		m_nSepType = SLAB_GUARD_RIGH;
		m_nDirType = 1;
	}
	else if(m_strName == "중분대(좌)")
	{
		m_nSepType = SLAB_GUARD_CENT;
		m_nDirType = -1;
	}
	else if(m_strName == "중분대(우)")
	{
		m_nSepType = SLAB_GUARD_CENT;
		m_nDirType = 1;
	}
	else
		m_nSepType = -1;
}

//<summary>
//기능 : 방호벽, 난간, 보도 등의 절점 좌표를 구함
//		 <가변구간>은 없다고 결정함. //입력에서 가변구간 설정 받지 않도록 할 것.
//		 사용시에 이전 좌표와 동일한 좌표인지 확인하고 사용할 것.
//</summary>
//<param name="A[9]">반환 받을 절점 좌표들</param>
void CCentSeparation::GetXyGuardFence(CDPoint A[9]) const
{
	for(long n=0; n<9; n++){
		A[n].x = 0;
		A[n].y = 0;
	}
		
	A[0].x = 0;
	A[1].x = m_D1;
	A[2].x = A[1].x;
	A[3].x = m_D1 + m_W1;
	A[4].x = m_D1 + m_W1 + m_W2;
	A[5].x = m_D1 + m_W1 + m_W2 + m_W3;
	A[6].x = m_D1 + m_W1 + m_W2 + m_W3 + m_W4;
	A[7].x = m_D1 + m_W1 + m_W2 + m_W3 + m_W4 + m_W5;
	A[8].x = A[7].x;

	A[0].y = 0;
	A[1].y = 0;
	A[2].y = m_H1;
	A[3].y = m_H1 + m_H2;
	A[4].y = m_H1 + m_H2 + m_H3;
	A[5].y = m_H1 + m_H2 + m_H3;
	A[6].y = m_H1 + m_H2;
	A[7].y = m_H1;
	A[8].y = 0;	
}

//방호벽 권고안...
void CCentSeparation::SetDataDefault1(CCentSeparation *pGuard, long nGuard , long bLeft)
{

	if( pGuard->m_nSepType == 0 || pGuard->m_nSepType == 1 )//방호벽 좌 / 우
	{
		pGuard->m_D1 = bLeft ? 30 : 0;
		
		pGuard->m_W1 = bLeft ? 0 : 120;
		pGuard->m_W2 = bLeft ? 0 : 70;
		pGuard->m_W3 = 230;
		pGuard->m_W4 = bLeft ? 70 : 0;
		pGuard->m_W5 = bLeft ? 120 : 0;
		pGuard->m_H1 = 205;
		pGuard->m_H2 = 175;
		pGuard->m_H3 = 700;
	}
	else if(pGuard->m_nSepType == 2 || pGuard->m_nSepType == 3)//중분대 좌 / 우
	{
		pGuard->m_D1 = 0;

		pGuard->m_W1 = 300;
		pGuard->m_W2 = 0;
		pGuard->m_W3 = 0;
		pGuard->m_W4 = 0;
		pGuard->m_W5 = 0;
		pGuard->m_H1 = 300;
		pGuard->m_H2 = 0;
		pGuard->m_H3 = 0;
	}
	else
	{
		pGuard->m_D1 = 0;

		pGuard->m_W1 = 230;
		pGuard->m_W2 = 110;
		pGuard->m_W3 = 370;
		pGuard->m_W4 = 110;
		pGuard->m_W5 = 230;
		pGuard->m_H1 = 115;
		pGuard->m_H2 = 175;
		pGuard->m_H3 = 560;
	}	
}

// 면적의 계산은 상부 좌측부터
void CCentSeparation::InitCalculate(double dUnitWeight /* = 2.5 */)
{
//	CBaseChief      *pBaseChief = m_pStd->m_pBaseChief;
//	CBaseDesignCtl	*pBaseCtl	= &pBaseChief->m_BaseBoltCtl;
	
	m_dAreaCenter.clear();
	m_strAreaFormula.clear();

	CString strFormula = _T("");
	pair<CString, CDPoint> vPair;
	CDPoint cValue(0, 0);
	CDPoint xyCent(0, 0);	
	double dWidth	= GetWidth();

	if(m_nSepType==SLAB_CURB_WALK_LEFT || m_nSepType==SLAB_CURB_WALK_RIGH)
	{		
		if(m_W4>=m_W1)
		{		
			m_Area[0]		= m_W1 * (m_H1 + m_H2);
			m_DistArea[0]	= m_W1/2;

			if(m_Area[0] > 0)
			{
				cValue.x = m_Area[0];
				cValue.y = m_DistArea[0];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H1 + m_H2), toM(m_W1), dUnitWeight);
				xyCent.x = m_W1/2.0;
				xyCent.y = (m_H1+m_H2)/2.0;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[1]		= (m_W4 - m_W1) * m_H2;
			m_DistArea[1]	= m_W1 + (m_W4 - m_W1)/2;

			if(m_Area[1] > 0)
			{
				cValue.x = m_Area[1];
				cValue.y = m_DistArea[1];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H2), toM(m_W4 - m_W1), dUnitWeight);
				xyCent.x = m_W1 + (m_W4 - m_W1)/2.0;
				xyCent.y = m_H2/2.0;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[2]		= m_W3 * (m_H2 - m_H3 - m_W5);
			m_DistArea[2]	= m_W4 + m_W3/2;

			if(m_Area[2] > 0)
			{
				cValue.x = m_Area[2];
				cValue.y = m_DistArea[2];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H2 - m_H3 - m_W5), toM(m_W3), dUnitWeight);
				xyCent.x = m_W4 + m_W3/2;
				xyCent.y = m_W5 + m_H3 + (m_H2-m_W5-m_H3)/2;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[3]		= m_W3 * m_W5;
			m_DistArea[3]	= m_W4 + m_W3/2;

			if(m_Area[3] > 0)
			{
				cValue.x = m_Area[3];
				cValue.y = m_DistArea[3];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_W5), toM(m_W3), dUnitWeight);
				xyCent.x = m_W4 + m_W3/2;
				xyCent.y = m_W5/2;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[4]		= (m_W1 + m_W2 - m_W3 - m_W4) * m_H2;
			m_DistArea[4]	= m_W4 + m_W3 + (m_W1 + m_W2 - m_W3 - m_W4)/2;

			if(m_Area[4] > 0)
			{
				cValue.x = m_Area[4];
				cValue.y = m_DistArea[4];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H2), toM(m_W1 + m_W2 - m_W3 - m_W4), dUnitWeight);
				xyCent.x = m_W4 + m_W3 + (m_W1 + m_W2 - m_W3 - m_W4)/2;
				xyCent.y = m_H2/2;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}
		}		
		else
		{
			m_Area[0]		= m_W2 * (m_H1 + m_H2);
			m_DistArea[0]	= m_W1 + m_W2/2;

			if(m_Area[0] > 0)
			{
				cValue.x = m_Area[0];
				cValue.y = m_DistArea[0];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H1 + m_H2), toM(m_W2), dUnitWeight);
				xyCent.x = m_W1 + m_W2/2;
				xyCent.y = (m_H1+m_H2)/2.0;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[1]		= (m_W1 - m_W3 - m_W4) * m_H2;
			m_DistArea[1]	= m_W4 + m_W3 + (m_W1 - m_W3 - m_W4)/2;

			if(m_Area[1] > 0)
			{
				cValue.x = m_Area[1];
				cValue.y = m_DistArea[1];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H2), toM(m_W1 - m_W3 - m_W4), dUnitWeight);
				xyCent.x = m_W4 + m_W3 + (m_W1 - m_W3 - m_W4)/2;
				xyCent.y = m_H2/2.0;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[2]		= m_W3 * (m_H2 - m_H3 - m_W5);
			m_DistArea[2]	= m_W4 + m_W3/2;

			if(m_Area[2] > 0)
			{
				cValue.x = m_Area[2];
				cValue.y = m_DistArea[2];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H2 - m_H3 - m_W5), toM(m_W3), dUnitWeight);
				xyCent.x = m_W4 + m_W3/2;
				xyCent.y = m_W5 + m_H3 + (m_H2-m_W5-m_H3)/2;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[3]		= m_W3 * m_W5;
			m_DistArea[3]	= m_W4 + m_W3/2;

			if(m_Area[3] > 0)
			{
				cValue.x = m_Area[3];
				cValue.y = m_DistArea[3];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_W5), toM(m_W3), dUnitWeight);
				xyCent.x = m_W4 + m_W3/2;
				xyCent.y = m_W5/2;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}

			m_Area[4]		= m_W4 * m_H2;
			m_DistArea[4]	= m_W4/2;

			if(m_Area[4] > 0)
			{
				cValue.x = m_Area[4];
				cValue.y = m_DistArea[4];
				strFormula.Format("%.3f x %.3f x %.3f", toM(m_H2), toM(m_W4), dUnitWeight);
				xyCent.x = m_W4/2;
				xyCent.y = m_H2/2;
				m_dAreaCenter.push_back(xyCent);

				vPair.first  = strFormula;
				vPair.second = cValue;
				m_strAreaFormula.push_back(vPair);
			}
		}
	}
	/*
	else if(IsElecHole())
	{
		m_Area[0]		= m_W1 * (m_H1 + m_H2 + m_H3);
		m_DistArea[0]	= m_W1/2;

		if(m_Area[0] > 0)
		{
			cValue.x = m_Area[0];
			cValue.y = m_DistArea[0];
			strFormula.Format("%.3f x %.3f x %.3f", toM(m_H1 + m_H2 + m_H3), toM(m_W2), dUnitWeight);
			xyCent.x = m_W1/2;
			xyCent.y = (m_H1 + m_H2 + m_H3)/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}

		m_Area[1]		= m_W3 * (m_H1 + m_H2 + m_H3);
		m_DistArea[1]	= m_W1 + m_W2 + m_W3/2;

		if(m_Area[1] > 0)
		{
			cValue.x = m_Area[1];
			cValue.y = m_DistArea[1];
			strFormula.Format("%.3f x %.3f x %.3f", toM(m_H1 + m_H2 + m_H3), toM(m_W3), dUnitWeight);
			xyCent.x = m_W1 + m_W2 + m_W3/2;
			xyCent.y = (m_H1 + m_H2 + m_H3)/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}

		m_Area[2]		= m_W2 * m_H1;
		m_DistArea[2]	= m_W1 + m_W2/2;

		if(m_Area[2] > 0)
		{
			cValue.x = m_Area[2];
			cValue.y = m_DistArea[2];
			strFormula.Format("%.3f x %.3f x %.3f", toM(m_H1), toM(m_W2), dUnitWeight);
			xyCent.x = m_W1 + m_W2/2;
			xyCent.y = m_H1/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}

		m_Area[3]		= m_W2 * m_H3;
		m_DistArea[3]	= m_W1 + m_W2/2;

		if(m_Area[3] > 0)
		{
			cValue.x = m_Area[3];
			cValue.y = m_DistArea[3];
			strFormula.Format("%.3f x %.3f x %.3f", toM(m_H3), toM(m_W2), dUnitWeight);
			xyCent.x = m_W1 + m_W2/2;
			xyCent.y = m_H1 + m_H2 + m_H3/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}
	}
	*/
	else
	{	
		m_Area[0]		= m_H3 * m_W3;
		m_DistArea[0]	= m_W1 + m_W2 + m_W3/2.;
		
		if(m_Area[0] > 0)
		{
			cValue.x = m_Area[0];
			cValue.y = m_DistArea[0];
			strFormula.Format("%.3f x %.3f x %.3f", toM(m_H3), toM(m_W3), dUnitWeight);
			xyCent.x = m_W1+m_W2+m_W3/2.0;
			xyCent.y = m_H1+m_H2+m_H3/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}
		
		m_Area[1]		= m_H3 * m_W2 / 2.;
		m_DistArea[1]	= m_W1 + m_W2*2./3.;
			
		if(m_Area[1] > 0)
		{
			cValue.x = m_Area[1];
			cValue.y = m_DistArea[1];
			strFormula.Format("%.3f x %.3f x %.3f x 1/2", toM(m_H3), toM(m_W2), dUnitWeight);
			xyCent.x = m_W1+m_W2/2.0;
			xyCent.y = m_H1+m_H2+m_H3/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}

		m_Area[2]		= m_H3 * m_W4 / 2.;
		m_DistArea[2]	= m_W1 + m_W2 + m_W3 + m_W4/3.;		

		if(m_Area[2] > 0)
		{
			cValue.x = m_Area[2];
			cValue.y = m_DistArea[2];
			strFormula.Format("%.3f x %.3f x %.3f x 1/2", toM(m_H3), toM(m_W4), dUnitWeight);
			xyCent.x = m_W1+m_W2+m_W3+m_W4/2.0;
			xyCent.y = m_H1+m_H2+m_H3/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}		

		m_Area[3]		= m_H2 * (m_W2 + m_W3 + m_W4);
		m_DistArea[3]	= m_W1 + (m_W2 + m_W3 + m_W4)/2.;

		if(m_Area[3] > 0)
		{
			cValue.x = m_Area[3];
			cValue.y = m_DistArea[3];
			strFormula.Format("%.3f x %.3f x %.3f", toM(m_H2), toM(m_W2+m_W3+m_W4), dUnitWeight);
			xyCent.x = m_W1 + (m_W2 + m_W3 + m_W4)/2.0;
			xyCent.y = m_H1+m_H2/2.0;
			m_dAreaCenter.push_back(xyCent);
			
			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}

		m_Area[4]		= m_H2 * m_W1 / 2.;
		m_DistArea[4]	= m_W1*2./3.;
		
		if(m_Area[4] > 0)
		{
			cValue.x = m_Area[4];
			cValue.y = m_DistArea[4];
			strFormula.Format("%.3f x %.3f x %.3f x 1/2", toM(m_H2), toM(m_W1), dUnitWeight);
			xyCent.x = m_W1/2.0;
			xyCent.y = m_H1+m_H2/2.0;
			m_dAreaCenter.push_back(xyCent);

			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}

		m_Area[5]		= m_H2 * m_W5 / 2.;
		m_DistArea[5]	= m_W1 + m_W2 + m_W3 + m_W4 + m_W5/3.;
		
		if(m_Area[5] > 0)
		{
			cValue.x = m_Area[5];
			cValue.y = m_DistArea[5];
			strFormula.Format("%.3f x %.3f x %.3f x 1/2", toM(m_H2), toM(m_W5), dUnitWeight);
			xyCent.x = m_W1+m_W2+m_W3+m_W4+m_W5/2.0;
			xyCent.y = m_H1+m_H2/2.0;
			m_dAreaCenter.push_back(xyCent);
			
			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}

		m_Area[6]		= m_H1 * dWidth;
		m_DistArea[6]	= dWidth / 2.;	
		
		if(m_Area[6] > 0)
		{
			cValue.x = m_Area[6];
			cValue.y = m_DistArea[6];
			strFormula.Format("%.3f x %.3f x %.3f", toM(m_H1), toM(dWidth), dUnitWeight);
			xyCent.x = dWidth/2.0;
			xyCent.y = m_H1/2.0;
			m_dAreaCenter.push_back(xyCent);
			
			vPair.first  = strFormula;
			vPair.second = cValue;
			m_strAreaFormula.push_back(vPair);
		}
	}
}

double CCentSeparation::GetArea()
{
	InitCalculate();

	double Sum_Area = 0;
	for(LONG i=0; i<AREA_NUM; i++)
		Sum_Area += m_Area[i];

	return Sum_Area;

}

// 방호벽 좌측하단에서 방호벽 전체 도심까지의 거리
double CCentSeparation::GetDistCentroid()
{
	InitCalculate();
	
	double Fir_InerM = 0;
	double Sum_Area  = 0;

	for(long i=0; i<AREA_NUM; i++)
	{
		TRACE2("%d : Area : %.3f  ",i+1, m_Area[i]);
		TRACE2("Dist : %.3f => Total : %.3f\n", m_DistArea[i], m_Area[i]*m_DistArea[i]);
		Fir_InerM += m_Area[i]*m_DistArea[i];
		Sum_Area += m_Area[i];
	}

	if(Sum_Area != 0)
		m_DistCentroid = Fir_InerM / Sum_Area;
	else
		m_DistCentroid	= 0;

	return m_DistCentroid;
}



CGuardFenceRebar* CCentSeparation::GetRebar()
{
	return m_pRebar;
}
