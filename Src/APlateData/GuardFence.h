// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// GuardFence.h: interface for the CGuardFence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARDFENCE_H__7AA8D4AF_A346_4554_90B0_6377B2C8ED55__INCLUDED_)
#define AFX_GUARDFENCE_H__7AA8D4AF_A346_4554_90B0_6377B2C8ED55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CGuardFence : public CObject  
{
public:
	CGuardFence();
	virtual ~CGuardFence();

	enum POSITION { LEFT, CENTER, RIGHT } nPosition;

	long	GetPosition();
	double	GetWidth()  const;
	double	GetHeight() const;

	double	m_D1;
	double	m_W1;
	double	m_W2;
	double	m_W3;
	double	m_W4;
	double	m_W5;
	double	m_H1;
	double	m_H2;
	double	m_H3;

	long	m_nDirType;

	void Serialize(CArchive& ar);
	CGuardFence& operator = (const CGuardFence& obj);

};

#endif // !defined(AFX_GUARDFENCE_H__7AA8D4AF_A346_4554_90B0_6377B2C8ED55__INCLUDED_)
