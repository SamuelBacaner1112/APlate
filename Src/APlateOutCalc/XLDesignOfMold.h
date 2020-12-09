// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// XLDesignOfMold.h: interface for the CXLDesignOfMold class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDESIGNOFMOLD_H__BADE3CBC_9E0D_4ADB_800C_9A6D69D372A7__INCLUDED_)
#define AFX_XLDESIGNOFMOLD_H__BADE3CBC_9E0D_4ADB_800C_9A6D69D372A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlateCrossBeam;
class CVBracing;
class CMovingLoad;

class CXLDesignOfMold  : public CAPlateOutCalcXL
{
public:
	CXLDesignOfMold(CAPlateOutCalcStd *pStd, CAPlateOutCalcXL *pAPlateOutCalcXL);
	virtual ~CXLDesignOfMold();

	CAPlateOutCalcStd	*m_pAPlateOutCalcStd;
	void SetXLControl(CXLControl *pXL)	{m_pXL = pXL;};
	void CalcEffectWidth();					///< 4.1 ��ȿ�� ���
	void SectionAssumptionGirder();			///< 4.2 �ܸ鰡�� - �Ŵ�
	void SectionAssumptionGirderSub(long nG, double dSta, CFrameSection *pFrSec);		///< 4.2 �ܸ鰡�� - �Ŵ�
	void SectionAssumptionCR();				///< 4.2 �ܸ鰡�� - ���κ�
	void SectionAssumptionST();				///< 4.2 �ܸ鰡�� - ���κ�
	void ComputeHLoad();					///< 4.3 �ۿ����� ���� - ��) Ⱦ���� ���ߺй�
	void ComputeHLoad_Wind();				///< 4.3 �ۿ����� ���� - ǳ����
	void ComputeHLoad_Collision();			///< 4.3 �ۿ����� ���� - ��ݰ��
	void ComputeHLoad_Mold();				///< 4.3 �ۿ����� ���� - ��) ������ �ۿ�����
	void ComputeHLoad_Mold_Cross();			///<  ��) ������ �ۿ����� - �������� ���κ�
	void ComputeHLoad_Mold_TU();			///<  ��) ������ �ۿ����� - ������ũ��Ʈ ����, 180��TURNING (TU�Ŵ�)
	void Modeling();						///< 4.4 ��) �𵨸�
	void SectionJewon();					///< 4.4 ��) �ܸ�����
	void Load();							///< 4.4 ��) ��������
	void EleForceTotal();					///< 4.4 ��) ����� ����
	void EleForceLoadType();				///< 4.4 ��) �ۿ����ߺ� ����� ����
	void EleForceSummary();					///< 4.4 ��) ����� ���
	void EleForceReaction();				///< 4.4 ��) �� �� �� ��
	void WarpingTorsion();					///< 4.5 ����Ʋ��
	void InfluenceK();						///< 4.6 2������ - ������ K
	CString RetMaxString(CString szStr);
protected:
	long m_nNumber;
	long m_nSttRow;
	CXLControl			*m_pXL;
	CAPlateOutCalcXL	*m_pAPlateOutCalcXL;
	CString				m_strCell[50];
	CStringArray		m_szArrSecJointNum;

	double GetScaceFactor(long nLane);

	void SetLines(int nLine);
	void CalcFlangeEffectWidth();
	void CalcSlabEffectWidth();

	void TypeCrossBeam(CPlateBasicIndex *pBx, CPlateCrossBeam *pCrossBeam);
	void TypeVBracing(CPlateBasicIndex *pBx, CVBracing *pVBracing);

	void EleForceSetTable(long nForceType);

	///< 4.3 �ۿ������� ���� - ��) ������ �ۿ�����
	///< 2) �ռ��� �������� 3) �ռ��� ��������
	void ComputeHLoad_DeadLoad();
	void ComputeHLoad_LiveLoad();
	void ComputeHLoad_PeopleLoad();
	void ComputeHLoad_WindLoad();
	///< ������ �׸�
	void DrawInfK(CDomyun* pDom, long nG);
	void DrawHLoad(CDomyun* pDom, CFEMManage *pFEM, long nLoadCase);

	CString GetJointArrszByI33(double dI33, long nType, long nG = -1);

	void ModelPlanOutput(CDomyun *pDom);
	void CalcNodeCoord(CDPointArray &pNode, double &dShoeLength, double dWidth);
	CDPoint GetCenterOffsetPos(double sta, double dOffset, long nG=-1);
	void WriteLoadForce(CFEMManage *pFEM, long nLoadCase);
private:
	BOOL IsOuterGirder(long nG);
};

#endif // !defined(AFX_XLDESIGNOFMOLD_H__BADE3CBC_9E0D_4ADB_800C_9A6D69D372A7__INCLUDED_)
