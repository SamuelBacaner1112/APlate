// Shoe.cpp: implementation of the CShoe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Shoe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CShoe::CShoe(const CShoe& shoe)
{
	*this = shoe;

}
CShoe::CShoe()
{
	m_dDisShoeFrWeb		  = 0;
	m_cDirectionShoe	  = 1;
	m_bLevelSolePlate = 0;
	m_dW_SolePlate = 0;
	m_dW2_SolePlate = 0;
	m_dW3_SolePlate = 0;
	m_dH_SolePlate = 0;
	m_dH2_SolePlate = 0;
	m_dH3_SolePlate = 0;
	m_dDia_SolePlateHole = 0;
	m_bHole_SolePlate = TRUE;
	m_dThick_SolePlate = 0;
	m_dThick_Shoe	=  80;
	m_dThick_Mortar	=  50;
	m_dThick_Block	= 210;
	m_dForceShoe	= 200;

	m_dW1_Mortal = 250;
	m_dW2_Mortal = 250;
	m_dW3_Mortal = 250;

	m_vXyzShoeInput = CVector(0,0,0);
	m_nRefType = -1;
}

CShoe::~CShoe()
{
}

void CShoe::SerializeShoe(CArchive& ar)
{
	long nFlag = 4;
	CVector vVector(0,0,0);
	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << m_dDisShoeFrWeb;
		ar << m_cDirectionShoe;
		ar << m_ZzdAngleShoe;
		ar << m_bLevelSolePlate;
 		ar << m_dW_SolePlate;
		ar << m_dH_SolePlate;
		ar << m_dThick_SolePlate;
		ar << m_dThick_Shoe;
		ar << m_dThick_Mortar;
		ar << m_dThick_Block;
		ar << m_dForceShoe;
		ar << m_dW2_SolePlate;
		ar << m_dW3_SolePlate;
		ar << m_dH2_SolePlate;
		ar << m_dH3_SolePlate;
		ar << m_dDia_SolePlateHole;
		ar << m_bHole_SolePlate;
		ar << m_dW1_Mortal;
		ar << m_dW2_Mortal;
		ar << m_dW3_Mortal;
		ar << m_vXyzShoeInput;
		ar << vVector;  // 여분.
		ar << m_nRefType;
	}
	else
	{
		ar >> nFlag;
		ar >> m_dDisShoeFrWeb;
		ar >> m_cDirectionShoe;
		ar >> m_ZzdAngleShoe;
		ar >> m_bLevelSolePlate;
 		ar >> m_dW_SolePlate; 
		ar >> m_dH_SolePlate;
		ar >> m_dThick_SolePlate;
		ar >> m_dThick_Shoe;
		ar >> m_dThick_Mortar;
		ar >> m_dThick_Block;
		ar >> m_dForceShoe;
		if(nFlag > 0)
		{
			ar >> m_dW2_SolePlate;
			ar >> m_dW3_SolePlate;
			ar >> m_dH2_SolePlate;
			ar >> m_dH3_SolePlate;
			ar >> m_dDia_SolePlateHole;
			ar >> m_bHole_SolePlate;
		}
		if(nFlag > 1)
		{
			ar >> m_dW1_Mortal;
			ar >> m_dW2_Mortal;
			ar >> m_dW3_Mortal;
		}
		if(nFlag > 2)
		{
			ar >> m_vXyzShoeInput;
			ar >> vVector;  // 여분.
		}
		if(nFlag > 3)
			ar >> m_nRefType;	
	}
}

void CShoe::SerializeForOldVersion(CArchive& ar)
{
	BYTE vbyte;
	if(!ar.IsStoring())
	{
		BYTE bFlag;
		ar >> m_cDirectionShoe;
		ar >> bFlag;
		ar >> vbyte;
		ar >> vbyte;
		ar >> m_dDisShoeFrWeb;//m_Pos;
		ar >> m_dW_SolePlate; m_dW_SolePlate -= 200; m_dW2_SolePlate = 100; 
		ar >> m_dH_SolePlate; m_dH_SolePlate -= 200; m_dH2_SolePlate = 100; 
		ar >> m_dThick_SolePlate;
		if( bFlag >= 1 )
			ar >> m_bLevelSolePlate;
		if( bFlag >= 2 )
			ar >> m_ZzdAngleShoe;
		// REV_HA : 2000.6.14
		if( bFlag == 3 ) {
			ar >> m_dForceShoe;
			ar >> m_dThick_SolePlate;//m_H1;
			ar >> m_dThick_Shoe;
			ar >> m_dThick_Mortar;
			ar >> m_dThick_Block;
		}
		m_dDia_SolePlateHole = 50;
		m_dW1_Mortal = 250;		
		m_dW2_Mortal = 250;
		m_dW3_Mortal = 250;
		m_dW3_SolePlate = m_dW_SolePlate - 200;
		m_dH3_SolePlate = m_dH_SolePlate - 200;
	}
}

CShoe& CShoe::operator=(const CShoe& Obj)
{
	m_dDisShoeFrWeb			= Obj.m_dDisShoeFrWeb;	
	m_cDirectionShoe					= Obj.m_cDirectionShoe			;
	m_bLevelSolePlate			= Obj.m_bLevelSolePlate	;
	m_dW_SolePlate			= Obj.m_dW_SolePlate	;
	m_dW2_SolePlate			= Obj.m_dW2_SolePlate;	
	m_dW3_SolePlate			= Obj.m_dW3_SolePlate;	
	m_dH_SolePlate			= Obj.m_dH_SolePlate	;
	m_dH2_SolePlate			= Obj.m_dH2_SolePlate;	
	m_dH3_SolePlate			= Obj.m_dH3_SolePlate;	
	m_dDia_SolePlateHole	= Obj.m_dDia_SolePlateHole;
	m_bHole_SolePlate		= Obj.m_bHole_SolePlate;
	m_dThick_SolePlate			= Obj.m_dThick_SolePlate	;
	m_dThick_Shoe					= Obj.m_dThick_Shoe			;
	m_dThick_Mortar					= Obj.m_dThick_Mortar			;
	m_dThick_Block					= Obj.m_dThick_Block			;
	m_dForceShoe					= Obj.m_dForceShoe			;
	m_dW1_Mortal				= Obj.m_dW1_Mortal	;	
	m_dW2_Mortal				= Obj.m_dW2_Mortal	;	
	m_dW3_Mortal				= Obj.m_dW3_Mortal	;	
	m_nRefType					= Obj.m_nRefType ;
	return *this;
}

BOOL CShoe::operator==( const CShoe& Obj) const
{
	if(m_dDisShoeFrWeb		!= Obj.m_dDisShoeFrWeb)		return FALSE;
	if(m_cDirectionShoe				!= Obj.m_cDirectionShoe)				return FALSE;
	if(m_bLevelSolePlate		!= Obj.m_bLevelSolePlate)		return FALSE;
	if(m_dW_SolePlate		!= Obj.m_dW_SolePlate)		return FALSE;
	if(m_dH_SolePlate		!= Obj.m_dH_SolePlate)		return FALSE;
	if(m_dW2_SolePlate		!= Obj.m_dW2_SolePlate)		return FALSE;
	if(m_dW3_SolePlate		!= Obj.m_dW3_SolePlate)		return FALSE;
	if(m_dH2_SolePlate		!= Obj.m_dH2_SolePlate)		return FALSE;
	if(m_dH3_SolePlate		!= Obj.m_dH3_SolePlate)		return FALSE;
	if(m_dThick_SolePlate		!= Obj.m_dThick_SolePlate)		return FALSE;
	if(m_dDia_SolePlateHole	!= Obj.m_dDia_SolePlateHole)return FALSE;
	if(m_bHole_SolePlate	!= Obj.m_bHole_SolePlate)   return FALSE;
	if(m_dThick_Shoe					!= Obj.m_dThick_Shoe)				return FALSE;
	if(m_dThick_Mortar					!= Obj.m_dThick_Mortar)				return FALSE;
	if(m_dThick_Block					!= Obj.m_dThick_Block)				return FALSE;
	if(m_dForceShoe				!= Obj.m_dForceShoe)				return FALSE;
	if(m_dW1_Mortal			!= Obj.m_dW1_Mortal)			return FALSE;
	if(m_dW2_Mortal			!= Obj.m_dW2_Mortal)			return FALSE;
	if(m_dW3_Mortal			!= Obj.m_dW3_Mortal)			return FALSE;

	return TRUE;
}

double CShoe::GetHeightTotalShoe() const
{
	return (m_dThick_SolePlate + m_dThick_Shoe + m_dThick_Mortar + m_dThick_Block);
}

// 현재는 존재여부를 m_dDisShoeFrWeb가 zero 인경우만 존재하지 않는 경우로 판단
BOOL CShoe::IsExist() const
{
	return m_dDisShoeFrWeb != 0 ? TRUE : FALSE;
}
