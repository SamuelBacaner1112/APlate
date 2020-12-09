// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// PlateStressOut.h: interface for the CPlateStressOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATESTRESSOUT_H__3DDF1F3B_A3EB_4F12_9E63_958F5A0AA4D1__INCLUDED_)
#define AFX_PLATESTRESSOUT_H__3DDF1F3B_A3EB_4F12_9E63_958F5A0AA4D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDataManage;
class CPlateBridgeApp;
class CLineInfoApp;
class CAPlateCalcStd;
class CFEMDraw;
class CFTextOutDlg;
class CCrossBeamVerStiff;
class CCrossBeamHorStiff;
class CShRichEdit;

class AFX_EXT_CLASS CPlateStressOut
{
public:

	CPlateStressOut(CGeneralBaseStd *pCalcStd);
	virtual ~CPlateStressOut();

	// Slab�ܸ��ؼ�
	void SlabSectionOutput(CStringArray &Arr);                        
	void SlabRebarOutput(CStringArray &Arr, long nPos);

	///< ������Ʈ������ �ս�
	void SlabSectionPrestress(long nSide, CShRichEdit &RichEdit);
	void SlabPrestresscheck(long nSide, CShRichEdit &RichEdit);		///< �ٴ��� ������ ���°���

	void SlabSectionOutputLeft(CStringArray &Arr);
	void SlabSectionOutputRight(CStringArray &Arr);
	void SlabSectionOutputCenter(CStringArray &Arr);
	void SlabSectionOutputReview(CStringArray &Arr);
	//������
	void Output_DeadLoad(CStringArray &Arr, long nPos);					//nPos=0 : ����ĵƿ����, nPos=1 : ����ĵƿ���� nPos=2 : �߾Ӻν�����
	void Output_MoveLoad(BOOL bLeft, CShRichEdit &RichEdit);			// Ȱ����
	void Output_MoveLoadCenter(CShRichEdit &RichEdit);		            // Ȱ����(�߾Ӻ�)
	void Output_CollisionLoad(long nSide, CShRichEdit &RichEdit);		// �浹����
	void Output_WindLoad(long nSide, CShRichEdit &RichEdit);			// ǳ����
	void Output_CentriodLoad(long nSide, CShRichEdit &RichEdit);		// ��������
	void Output_TotalCombinationLoad(long nPos, CShRichEdit &RichEdit);		// ��������
	void Output_SlabThickness(long nPos, CShRichEdit &RichEdit);		// �������ּҵβ����� nPos=0 : ����ĵƿ����, nPos=1 : ����ĵƿ���� nPos=2 : �߾Ӻν�����
	void Output_SlabThickness_Tendon(CShRichEdit &RichEdit);			// ������ ���� ��� �ٴ��� �ּ� �β� ����
	void Output_RebarQtyCheck(long nPos, CShRichEdit &RichEdit, long nCheckType);           //������ö�ٷ�����
	void Output_DroopReview(long nSide, CShRichEdit &RichEdit);		      //ó������    
	void Output_CrackReview(long nSide, CShRichEdit &RichEdit);		       //�տ�����
	void Output_GiJumReview(CStringArray &Arr);					       //��������
	void OutPut_CentriodLoadCenter(CShRichEdit &RichEdit);						// �������߾Ӻ� ��������
	void Output_InnerFloor(CShRichEdit &RichEdit);
	void Output_RebarQtyExperienceCheck(CShRichEdit &RichEdit);			// �߾Ӻ� ������ ����
	void Output_UltimateLoad(long nSide, CShRichEdit &RichEdit);		// �Ҽ����� �������� ����
	void Output_UltimateLoadCheck(long nSide, CShRichEdit &RichEdit);	// �������߿� ���� �ܸ���� (2009.07.24)
	void Output_FixingPlate(CShRichEdit &RichEdit);						// ������ ���� ����
	void Output_WeldReview(long nType, CShRichEdit &RichEdit, BOOL bJijum); //���� ����
	// ������������
	void SpliceCheckOutput(CStringArray &Arr, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceCheckOutput(CStringArray &Arr, long nG, long nSp, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceFlangeCheckUp(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Sf, double Sa, double Ra, double Dh, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceFlangeCheckLo(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Sf, double Sa, double Ra, double Dh, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void SpliceWebCheck(CStringArray &Arr, long nG, long nSp, double Ss, double Sv, double Ms, double Mv, double Su, double Sl, double Sa, double Ra, CMap<CString, LPCTSTR, double, double> &MapSplice);
	void MakeIoTable (CStringArray &Arr, double pTable [][8], int nCount, int nLMargin = 5);
	// �ܸ����°���
	void StressCheckCoefOutput(CStringArray &Arr, long nG, double dSta, BOOL bDistance);
	// ���κ� ����
	void SectionCheckOutput(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckVBracingGenOutput(CStringArray &Arr, CPlateBasicIndex *pBx); // �����극�̽�(�Ϲ�Ÿ��)
	void StressCheckVBracingJijumOutput(CStringArray &Arr, CPlateBasicIndex *pBx); // �����극�̽�(����Ÿ��)
	void StressCheckEndCrossBeamGeneralType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckEndCrossBeamVBracingType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckCenterCrossBeamGeneralType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);
	void StressCheckCenterCrossBeamVBracingType(CStringArray &Arr, CPlateBasicIndex *pBx, CMap <CString, LPCTSTR, double, double> &Map);

	// Modeling���
	void ElementForceOutput(CStringArray &Arr);                        // ����� ����
	void NodeReactionOutput(CStringArray &Arr);                        // �ݷ� ����
	void NodeDisplaceOutput(CStringArray &Arr);                        // ó�� ����
	void NodeCamberOutput(CStringArray &Arr);						   // ��������	
	void Output_NodeDisplaceCheck(CStringArray &Arr);                  // ó������

	///< ������������ ����
	void CheckVStiff(CStringArray &Arr, CPlateBasicIndex *pBx);
	CGeneralBaseStd *m_pStd;	
	double GetMapValue(CString szMapStr);

	void VerStiff(CStringArray &Arr, CCrossBeamVerStiff *pVerStiff, long nQtyHorStiff);
	void HorStiff(CStringArray &Arr, CCrossBeamHorStiff *pHorStiff);

	void FillOnOKorNG(CShRichEdit &RichEdit, BOOL bIsOK, long nCnt);
	// ��꿡 �ʿ��� �÷��� �β��� �� �β� ����
	void SeekFlange_Web(double& dFlange, double& dWeb);

};

#endif // !defined(AFX_PLATESTRESSOUT_H__3DDF1F3B_A3EB_4F12_9E63_958F5A0AA4D1__INCLUDED_)
