// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutBMBrInfo.h: interface for the CAPlateOutBMBrInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTBMBRINFO_H__2877833C_845B_4337_9B1A_5664688EF622__INCLUDED_)
#define AFX_APLATEOUTBMBRINFO_H__2877833C_845B_4337_9B1A_5664688EF622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SECTION_DANBU_STT	0
#define SECTION_DANBU_END	1
#define SECTION_GEN			2
#define SECTION_JIJUM		3

class CPlateBasicIndex;
//----------class CBrBeam;

class CAPlateOutBMBrInfo  
{
public:
	CAPlateOutBMBrInfo(CAPlateOutBMStd *pAPlateOutBMStd);
	virtual ~CAPlateOutBMBrInfo();

	CAPlateOutBMStd	*m_pAPlateOutBMStd;
	CString	m_szBridgeName;			// �������� �̸�
	CString m_szBridgePath;			// �������� ���
	CString m_szBridgeDir;			// ���� �������
	BOOL	m_bOutTotSlab;			// ����������Ѱ� ��¿���
	BOOL	m_bOutTotRebar;			// ö�ټ����Ѱ� ��¿���
	BOOL	m_bOutRebar;			// ö�� ����ǥ ��¿���
	BOOL	m_bOutSlab;				// ������ ���� ��¿���
	BOOL	m_bOutCalcFoundation;	// ����ٰ�
	long	m_nSpan;				// 

	void GetSlabHunch(CPlateBasicIndex *pBx, CVectorArray &ptArr);
	void GetGirderWidth(long &nGirdersu1, double &dGirderWidth1, long &nGirdersu2, double &dGirderWidth2);
	//----------double GetBrBeamWidth(CBrBeam *pBrBeam);
	double GetCantileverWidth(BOOL bLeft);
	// double GetHeightCrossBeamHunch(BOOL bMaxHeight);
	void GetCrossBeamHunch(double &dMaxLength, double &dMinLength, long &nQtyHunch, 
								double &dMinHeight, double &dMaxHeight, double &dLowWidthdouble);
// 	double GetWidthCrossBeamLow();
// 	long GetQtyCrossBeamHunch();			
	double GetGirderHeight(BOOL bMax);
	void GetSlabVerticalDimensions(CVectorArray &ptArr);
	double GetUpperLengthAver(BOOL bPositive);
	double GetConfinementLengthAver();

	void DrawSlabHunch(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawSlabDongbari(CDomyun *pDom, CPlateBasicIndex *pBx);
	void DrawMainDomyun(CDomyun *pDom, long nSectionPos);
	void DrawSlabHunchGen(CDomyun *pDom);
	void DrawSlabFrontGen(CDomyun *pDom);
	void DrawCrossRebarBindConc(CDomyun *pDom);
	CPlateBasicIndex* GetBxSection(long nSectionPos);
	double GetBridgeLength(long nGuardFence);

	double GetLengthGirder(long nG);
	CDoubleArray GetSizeConnectPart(long nG, BOOL bUpper);		// ������ ���� 0:����� 1:������ 2:�����
	CDoubleArray GetLengthWebButtWelding();						// ������ �´������ ����(�Ŵ���)
	//CDoubleArray GetLengthJongRib(BOOL bUpper);					// ������ ����(�Ŵ���)
	CDoubleArray GetLengthHStiffener();							// ���򺸰��� ����(�Ŵ���)
	//CDoubleArray GetSizeJijumStiffener(long nG);				// ���������� ����(������)
	void GetLengthJijumStiffener(long nG, CDoubleArray &arrLength, CDoubleArray &arrSize);				// �����ΰ��� ����(������)
	//double GetLengthBracketUp(CString szTarget);				// ���κ�(������,�Ϲݺ�), �������κ�(������,�Ϲݺ�), ���κ� ����ϻ��� ����

	//���/�Ϻ�  �÷��� ���� �� ���Ʈ ����.....
	void GetFlangeLengthCompress(BOOL bUpper, CDoubleArray &arrFlangeWidth, 
		CDoubleArray &arrCompress, CDoubleArray &arrSort, double dCompress[3][99]);
};

#endif // !defined(AFX_APLATEOUTBMBRINFO_H__2877833C_845B_4337_9B1A_5664688EF622__INCLUDED_)