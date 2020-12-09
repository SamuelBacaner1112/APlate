// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateDrawStd.h: interface for the CAPlateDrawStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEDRAWSTD_H__BBD32DEE_244E_4608_8EE6_5F061E1A52E0__INCLUDED_)
#define AFX_APLATEDRAWSTD_H__BBD32DEE_244E_4608_8EE6_5F061E1A52E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

static const int PAGE_GENERAL	= 0;
static const int PAGE_DETAIL	= 1;
static const int PAGE_HBDETAIL	= 2;
static const int PAGE_SPLICE	= 3;
static const int PAGE_HGDETAIL	= 4;
static const int PAGE_VSDETAIL	= 5;
static const int PAGE_ETCDETAIL = 6;
static const int PAGE_BM		= 7;
static const int PAGE_SUMMARY	= 8; 
static const int PAGE_SHOECOORD = 9;
static const int PAGE_CAMBER	= 10;
static const int PAGE_PAINTMAP	= 11;
static const int PAGE_PLANGEN	= 12;
static const int PAGE_LINEINFO	= 13;
static const int PAGE_SLABGEN	= 14;
static const int PAGE_TENDON	= 15;
static const int PAGE_DECKREBAR	= 16;
static const int PAGE_TUGIRDER	= 17;
static const int PAGE_WELDMAP	= 18;

class COptionStd;
class CAPlateCalcStd;
class AFX_EXT_CLASS CAPlateDrawStd : public CDrawStd  
{
public:
	CAPlateDrawStd(CDataManage *pAData);
	virtual ~CAPlateDrawStd();

public:
	CDataManage		*m_pDataManage;	
	COptionStd		*m_pDomOptionStd;
	CPlateBridgeApp *m_pDB;
	CAPlateCalcStd	*m_pCalcStd;

	CDataManage *GetDataManage() const { return m_pDataManage; }
	COptionStd *GetDomOptionStd() const { return m_pDomOptionStd; }

	// 공통함수
public:
	void	AddMaterial(CDomyun * pAdd, double Scale);
	void	AddJubujebubuje(CDomyun* pAdd, double Scale);
	void	ChangeSeperateLayer(CDomyun *pDom);
	void	SetDomyunSize(long nSize);
	long	GetDomyunSize();
	void	SetDomyunTitleArray(CStringArray &strArrTitle);
	void	SetAtDomyunTitle(long nIdx, CString &strTitle);
	CString GetAtDomyunTitle(long nIdx);
private:
	long m_nDomyunSize; 
	CStringArray m_strArrDomyunTitle;
};

#endif // !defined(AFX_APLATEDRAWSTD_H__BBD32DEE_244E_4608_8EE6_5F061E1A52E0__INCLUDED_)
