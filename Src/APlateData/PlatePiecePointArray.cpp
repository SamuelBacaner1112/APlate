// PiecePointArray.cpp: implementation of the CPlatePiecePoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlatePiecePoint.h"
//#include "PiecePointArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlatePiecePointArray::CPlatePiecePointArray()
{
	m_dwAttr = 0;
	SetClockWise(TRUE);
	SetSolid(TRUE);
  	m_vNormal = CVector(0, 0, 1);
}

CPlatePiecePointArray::~CPlatePiecePointArray()
{

}

void CPlatePiecePointArray::SetVectorArray(const CVectorArray& vArr)
{	
	SetSize( vArr.GetSize() );	 
	for(long n = 0; n < vArr.GetSize(); n++)	
	{
		CPlatePiecePoint p;
		p = vArr.GetAt(n);
		SetAt( n, p );	
	}
}

void CPlatePiecePointArray::GetVectorArray(CVectorArray& vArr,P_TYPE ptype/*=P_TYPE_OUTLINE*/) const
{	
	vArr.RemoveAll();
	long nSize = GetSize();
  	if (nSize == 0)
  		return;
	CVector pos, posPre;
	long n = 0;
	for (n = nSize-1; n >= 0; n--)
		if (GetAt(n).GetType() & ptype)   {
			posPre = GetAt(n);
			break;
		}

	BOOL bCircle = TRUE;
  	for (n = 0; n < nSize; n++)   {
		if (!(GetAt(n).GetType() & ptype))
			continue;
		pos = GetAt(n);
		if (GetAt(n).GetType() & P_TYPE_ARC)   {

			// ��κ� ó��.
//			double arcUnitAng = 30;
//			double arcUnitLen = 20;
			double arcUnit = 3.5;
			double bulge = GetAt(n).GetLength();
			CVector  cen = GetCenterArc3D(posPre, pos, m_vNormal, bulge);
			double angle = (posPre - cen).GetAng(pos - cen).GetAngleRadian();
			angle = (double)(((posPre - cen) * (pos - cen)) ^ m_vNormal) > 0 ? angle : (ConstPi*2 - angle);
			angle = bulge < 0 ? angle : (angle - ConstPi*2);
//			double nDivide1 = fabs(angle) / ToRadian(arcUnitAng);  // �
//			double nDivide2 = fabs(angle) * ~(pos-cen) / arcUnitLen;  // �����
//			long nDivide = (long)Round((nDivide1 + nDivide2) / 2, 0);
			long nDivide = (long)Round(sqrt(angle * angle * ~(pos-cen)) / arcUnit, 0);
			nDivide = min(max(nDivide, 2), 10);  // �ּ� 2���, �ִ� 10���
			CDPoint vAng(cos(angle / nDivide), sin(angle / nDivide));
			CVector posArc = posPre;
			for (long i = 0; i < nDivide - 1; i++)   {
				posArc = posArc.GetRotateAsAxis(cen, m_vNormal, vAng);
				vArr.Add(posArc);
			}
		}
		vArr.Add(pos);
		posPre = pos;
 		bCircle = FALSE;  // Line�̳� Arc�� ������ Circle�� ó������ ����.
 	}
 	if (bCircle)   {
 		for (n = 0; n < nSize; n++)   {
 			if (!(GetAt(n).GetType() & P_TYPE_CIRCLE))
 				continue;
 			if (!IsBoltHole())   {  // ���� �ܰ���
 				CVector cen = GetAt(n);
 				double arcUnit = 3.5;
 				double radius = GetAt(n).GetLength();
 				double angle = ConstPi * 2;
 				long nDivide = (long)Round(sqrt(angle * angle * radius) / arcUnit, 0);
 				nDivide = min(max(nDivide, 6), 20);  // �ּ� 6���, �ִ� 20���
 				CDPoint vAng(cos(angle / nDivide), sin(angle / nDivide));
 
				CVector vAxis = IsClockWise() ? -m_vNormal : m_vNormal;
				CVector vX(1, 0, 0), vZ(0, 0, 1);
 				CVector vBase = ~(vAxis * vX) > ~(vAxis * vZ) ? vX : vZ;
 				CVector posArc = cen + (vAxis * vBase).Unit() * radius;  // ù ������ ��ġ
 				for (long i = 0; i < nDivide; i++)   {
 					posArc = posArc.GetRotateAsAxis(cen, vAxis, vAng);
 					vArr.Add(posArc);
 				}
 				return;  // ó�� �Ѱ��� ó��.
 			}
 			else  // Bolt Holl��.
 				vArr.Add(GetAt(n));  // �߽���ġ�� ����
 		}
 	}
 }
 
 double CPlatePiecePointArray::GetAreaInfo(CVector &cCenter, BOOL bPolygon) const
{
	double areaSum = 0, area;
	CVector cenSum(0, 0, 0), center;
	CVectorArray vArr;
	BOOL bCircle = TRUE;
	long n = 0;
	if (bPolygon)   {
		// Arc ���� ����. (�����߽� ������ ��������. �����Ƽ�... ����...)
		CVector pos, posPre;
		for (n = GetSize()-1; n >= 0; n--)
			if (GetAt(n).GetType() & P_TYPE_OUTLINE)   {
				posPre = GetAt(n);
				break;
			}
		for (n = 0; n < GetSize(); n++)   {
			if (!(GetAt(n).GetType() & P_TYPE_OUTLINE))
				continue;
			pos = GetAt(n);
			if (GetAt(n).GetType() & P_TYPE_ARC)   {
				double bulge = GetAt(n).GetLength();
				CVector  cen = GetCenterArc3D(posPre, pos, m_vNormal, bulge);
				CVector vCross = (posPre - cen) * (pos - cen);
				double angle = (posPre - cen).GetAng(pos - cen).GetAngleRadian();
				area = pow(~(pos - cen), 2) * angle / 2 - (~vCross) / 2;
				areaSum += ((bulge < 0) ^ (IsClockWise() > 0)) ? area : -area;
			}
			vArr.Add(pos);
			posPre = pos;
			bCircle = FALSE;  // Line�̳� Arc�� ������ Circle�� ó������ ����.
		}
	}
	else   {
		GetVectorArray(vArr);
		bCircle = FALSE;
	}
	if (bCircle)   {
		// Hole �κ�.
		for (n = 0; n < GetSize(); n++)   {
			if (GetAt(n).GetType() & P_TYPE_CIRCLE)   {
				area = ConstPi * pow(GetAt(n).GetLength(), 2);
				center = GetAt(n);
				areaSum += area;
				cenSum += center * area;
			}
		}
	}
	area = vArr.GetAreaInfo(center, bPolygon);
	areaSum += area;
	cenSum += cCenter * area;

	cCenter = cenSum / areaSum;
	if (!IsSolid())
		areaSum = -areaSum;
	return areaSum;
}

CPlatePiecePointArray& CPlatePiecePointArray::RotateAsAxis(const CVector &cBase,const CVector &vAxis,const CDPoint &Angle)
{
	for(long n = 0; n < GetSize(); n++)
		GetAt(n) = GetAt(n).GetRotateAsAxis(cBase,vAxis,Angle);

	m_vNormal = m_vNormal.GetRotateAsAxis(CVector(0, 0, 0), vAxis,Angle);
	return *this;
}

CPlatePiecePointArray& CPlatePiecePointArray::Move(const CVector& vMove)
{
	for(long n = 0; n < GetSize(); n++)
		(*this)[n] += vMove;

	return *this;

}

CPlatePiecePointArray& CPlatePiecePointArray::Transform(const CVector& cBaseS, const CVector& cBaseT, const CVector& vDir1S,
									  const CVector& vDir1T, const CVector& vDir2S, const CVector& vDir2T)
{
	// ������ ����.
	Move(cBaseT - cBaseS);

	// ù��° ���ع��� ����.
	CVector vAxis = (vDir1S * vDir1T).Unit();
	CDPoint Angle = vDir1S.GetAng(vDir1T);
	if (Angle == CDPoint(-1, 0))  // 180�� �ݴ� ����.
		vAxis = (vDir1S * vDir2S).Unit();
	CVector vRoateDir2S = vDir2S;
	if (!vAxis.IsNull())   {
		RotateAsAxis(cBaseT, vAxis, Angle);
		vRoateDir2S = vDir2S.GetRotateAsAxis(CDPoint(0,0), vAxis, Angle);
	}

	// �ι�° ���ع��� ����. (ȸ������ ù��° ���ع�����. �׷��Ƿ� ������ �ƴ� ������ ����.)
	vAxis = vDir1T;
	Angle = (vAxis * vRoateDir2S).GetAng(vAxis * vDir2T);
	if (double ((vRoateDir2S * vDir2T) ^ vAxis) < 0)
		Angle.MirrorVert();
	if (!Angle.IsNull())
		RotateAsAxis(cBaseT, vAxis, Angle);

	return *this;
}

// ��������� ��� ���
CPlatePiecePointArray& CPlatePiecePointArray::TransformForHori(const CVector& cBaseS, const CVector& cBaseT, const CDPoint& vXYDir)
{
	CVector vX(1, 0, 0), vZ(0, 0, 1);
	Transform(cBaseS, cBaseT, vZ, vZ, vX, (CVector)vXYDir);
	return *this;
}

// ���������� ��� ���
CPlatePiecePointArray& CPlatePiecePointArray::TransformForVert(const CVector& cBaseS, const CVector& cBaseT, const CDPoint& vXYDir)
{
	CVector vX(1, 0, 0), vY(0, 1, 0), vZ(0, 0, 1);
	Transform(cBaseS, cBaseT, vY, vZ, vX, (CVector)vXYDir);
	return *this;
}
