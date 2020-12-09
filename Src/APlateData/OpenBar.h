// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OpenBar.h: interface for the COpenBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENBAR_H__BDA6AEE7_4984_4119_AD9B_1194B3479AB3__INCLUDED_)
#define AFX_OPENBAR_H__BDA6AEE7_4984_4119_AD9B_1194B3479AB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS COpenBar : public CObject
{
protected:
	CDPoint m_ptCen;		//������ �߽���ġ 
	int		m_nTypeOpen;	//������ Ÿ��(0 : ����, 1:�ڽ���)
	double	m_dRadiusOpen;	//����Ÿ���϶� ������ ������ 
	double	m_dWidthOpen;	//�ڽ�Ÿ���϶� ������ ���α���
	double	m_dLengthOpen;	//�ڽ�Ÿ���϶� ������ ���α��� 
	CString m_strOpenType;	//������ ���� �̸� 

	int		m_nCntRebar;	//�� ������ ö�� ����
	double	m_dSpaceRebar;	//ö�ٰ��� 
	double	m_dDiaRebar;	//ö������
	double	m_dLengthRebar;	//ö����ü ����
	double  m_dFy;			//ö���׺����� 

public:
	COpenBar();
	COpenBar(const COpenBar& obj);
	virtual ~COpenBar();
	void Serialize(CArchive& ar);

//Method
public:
	void	SetOpenCenter(double x, double y){m_ptCen.x = x; m_ptCen.y =y; }
	CDPoint GetOpenCenter(){return m_ptCen;}
	void	SetCntRebar(int nCntRebar){m_nCntRebar = nCntRebar;}
	int		GetCntRebar(){return m_nCntRebar;}
	void	SetSpaceRebar(double dSpace){m_dSpaceRebar = dSpace;}
	double	GetSpaceRebar(){return m_dSpaceRebar;}
	void	SetDiaRebar(double dDia){m_dDiaRebar = dDia;}
	double	GetDiaRebar(){return m_dDiaRebar;}
	void	SetLengthRebar(double dLength){m_dLengthRebar = dLength;}
	double	GetLengthRebar(){return m_dLengthRebar;}
	void	SetLengthOpen(double dLength){m_dLengthOpen = dLength;}
	double	GetLengthOpen(){return m_dLengthOpen;}
	void	SetWidthOpen(double dWidth){m_dWidthOpen = dWidth;}
	double	GetWidthOpen(){return m_dWidthOpen;}
	void	SetOpenType(int nType){m_nTypeOpen = nType;}
	int		GetOpenType(){return m_nTypeOpen;}
	void	SetNameOpenType(CString strName){m_strOpenType = strName;}
	CString	GetNameOpenType(){return m_strOpenType;}
	void	SetOpenRadius(double dRadius){m_dRadiusOpen = dRadius;}
	double	GetOpenRadius(){return m_dRadiusOpen;}
	void	SetFy(double dFy){m_dFy = dFy;}
	double	GetFy(){return m_dFy;}

//Draw Method
public:
	CString GetTextDia();
	void DrawOpen(CDomyun* pDom);

};

#endif // !defined(AFX_OPENBAR_H__BDA6AEE7_4984_4119_AD9B_1194B3479AB3__INCLUDED_)
