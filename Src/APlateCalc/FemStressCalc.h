// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// FemStressCalc.h: interface for the CFemStressCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEMSTRESSCALC_H__502BF5DE_6071_40FC_B600_C70A2C3BFDEB__INCLUDED_)
#define AFX_FEMSTRESSCALC_H__502BF5DE_6071_40FC_B600_C70A2C3BFDEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  NOD_DISPLACE   0       // ó��
#define  NOD_ROTATE     1       // ȸ����
#define  NOD_REACTION   2       // �ݷ�
#define  NOD_MOMENT     3       // ���Ʈ

#define  LC_PREV_S				0x00000001  // �ռ��� ������ (Steel)
#define  LC_PREV_CONF			0x00000002  // �ռ��� ������ (������ũ��Ʈ)
#define  LC_PREV_TURN			0x00000004  // �ռ��� ������ (�ʹ�)
#define  LC_PREV_C				0x00000008  // �ռ��� ������ (Concrete)
#define  LC_NEXT				0x00000010  // �ռ��� ������
#define  LC_LIVE_MAX			0x00000020  // Ȱ���� Max
#define  LC_LIVE_MIN			0x00000040  // Ȱ���� Min
#define  LC_ONLYLIVE_MAX		0x00000080  // Ȱ���� Max
#define  LC_ONLYLIVE_MIN		0x00000100  // Ȱ���� Min
#define  LC_DISP_MAX			0x00000200  // ���� ħ�� Max
#define  LC_DISP_MIN			0x00000400  // ���� ħ�� Min
#define  LC_SUM_MAX				0x00000800  // �հ� Max
#define  LC_SUM_MIN				0x00001000  // �հ� Min		///< GetElementForce()�� ��������� ����..
#define  LC_DB_MAX				0x00002000	// DB max
#define  LC_DB_MIN				0x00004000	// DB min
#define  LC_DL_MAX				0x00008000	// DL max
#define  LC_DL_MIN				0x00010000	// DL min
#define  LC_MTY_MAX				0x00020000	// MTY max
#define  LC_MTY_MIN				0x00040000	// MTY min
#define  LC_PEOPLE_MAX			0x00080000	// PEOPLE max
#define  LC_PEOPLE_MIN			0x00100000	// PEOPLE min
#define  LC_WIND_MAX			0x00200000	// WIND max
#define  LC_WIND_MIN			0x00400000	// WIND min
#define  LC_TANK_MAX			0x00800000	// TANK max
#define  LC_TANK_MIN			0x01000000	// TANK min
#define  LC_TRAILER_MAX			0x02000000	// TRAILER max
#define  LC_TRAILER_MIN			0x04000000	// TRAILER min
#define  LC_PREV_CONF2			0x08000000	// �ռ��� ������
#define  LC_PREV_ST_CON			0x10000000	// �ռ��� Steel + ������

// �ռ��� ������
const int FRAME_PRE_DEADLOAD	= LC_PREV_S|LC_PREV_C|LC_PREV_CONF|LC_PREV_TURN;
const int FRAME_PRE_DEADLOAD_2	= LC_PREV_ST_CON|LC_PREV_C|LC_PREV_CONF;
const int FRAME_NEXT_DEADLOAD	= LC_NEXT;


class CFemModelingCalc;
class CPlateBasicIndex;

class AFX_EXT_CLASS CFemStressCalc 
{
public:
	CFemStressCalc(CDataManage *pDataManage, CAPlateCalcStd *pCalcStd);
	virtual ~CFemStressCalc();

public:
	enum { CURRENT, LEFT, RIGHT, BOTH };
	
	CFrameSection* GetFrameSectionByIndex(CFEMManage *pFEM, long nIndex);
	// ���� �ܸ��� I33���� �����ö� �����̼��� ��ġ
	double GetStationCheckPos(CPlateBasicIndex *pBx);
	// ���� �ܸ��� I33���� �����ö� �����̼��� ��ġ
	double GetStationCheckPos(long nG, double dSta);
	// ���� �ܸ��� ����
	double GetAreaOfSectionByStation(long nG, double dSta);

	CDataManage      *m_pDataManage;
	CAPlateCalcStd   *m_pCalcStd;

	map<CString, double> m_MapSecStress;// GetSectionStress
	double GetMapValueSecStress(CString sz);	
	
	double GetOneNodForceSub(CFEMManage *pFEM, int nForceType, long nLC, int nNodIdx, char cAxis);
	double GetOneNodForce(int nForceType, int nLC, int nNodIdx, char cAxis, BOOL &bExist);

	///< nForceType, nLC�� ���ؼ� ���(nEleIdx)�� ���̺���ġ(dRatio)��	������� ���Ѵ�. 
	double GetOneEleForce(int nForceType, int nLC, int nEleIdx, double dRatio);

	///< nForceType, nLC�� ���ؼ� �ش� ���(nEleIdx)�� �ִ������� ���Ѵ�. 
	double GetOneEleMaxForce(int nForceType, int nLC, int nEleIdx);
	
	///< �Ŵ�(nG)�� �����̼�(dSta)�� ��ҷ��� ���Ѵ�.	 
	///<   nResultType ==  0   ������ ��. (Girder�� Basic Index�� �����Ǿ�� ��.)
	///<               ==  1   �ִ밪. (+�� ���� ū��)
	///<               ==  2   �ּҰ�. (-�� ���� ū��)
	///<               ==  3   �ִ� ���밪. (���밪���� ���� ū��. ��, ��ȣ�� �״�� ���ϵ�.)
	///<               ==  4   �հ� Max�� Min�� ���밪�� ū��. (��, ��ȣ�� �״�� ���ϵ�.)
	///<                       (�� ���� LoadCase�� ���õǰ� ������ �հ� Max Min������ ���.)
	double GetElementForce (int nForceType, int nLC, long &nG, double &dSta, int nResultType);

	///< �Ŵ�(nG)�� �����̼�(dStaStt)�� �����̼�(dStaEnd)�� �������� ��ҷ��� ���Ѵ�.	 
	double GetElementForceTerm(int nForceType, int nLC, long nG, double &dStaStt, double dStaEnd, int nResultType);

	///< ��,�Ϻ� ���°� �� ������°� ���ϰ� ���º� ������.
	double GetMaxStressRatio(long nG, double dSta, double &Max_up, double &Max_lo, double &Comp_up, double &Comp_lo, double &Tau, double dStaSec=0, long nPosSec=0);

	///< nG�Ŵ��� dSta��ġ���� ���Ϻ��ִ����°��� �������, �ִ��������°� ������������� ����
	void GetFlangeStress(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec=0, long nPosSec=0);
	void GetFlangeStress_TUGirder(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec=0, long nPosSec=0);
	void GetFlangeStress_GenGirder(long nG, double dSta, double &Max_up, double &Max_lo, double &Max_upA, double &Max_loA, double &Tau_up, double &Tau_lo, double &Tau_upA, double &Tau_loA, double dStaSec=0, long nPosSec=0);

	///< �ܸ� ���並 ���� ���� ���°��� ���ϱ�
	void GetSectionStress(long nG, double dSta, BOOL bHAPSUNG=TRUE, double dStaSec=0, long nPosSec=0);
	void GetSectionStress_TUGirder(long nG, double dSta, BOOL bHAPSUNG=TRUE, double dStaSec=0, long nPosSec=0);
	void GetSectionStress_GenGirder(long nG, double dSta, BOOL bHAPSUNG=TRUE, double dStaSec=0, long nPosSec=0);
	double GetAbsMaxMinEleForce(long nForceType, long nElemIdx, double dRatio);

	///< �ܸ����׸��Ʈ�� ������
	void MakeDanmyunMomentData(long nG);
	void DeleteOnNotUsedExtSection(int nG);
	void SetDanmyunMoment(CDanMyunProp *pPreSect, CDanMyunProp *pCurSect, CDanMyunProp *pNextSect, int nType, double dBound);
	
	BOOL InsertMomentPos(long nG);
	BOOL IsMomentPos(long nG, double dSta);
	///< �ܸ���䱸���� ����.
	void SetSectionGugan();
	
	///< ��������(���κ� ����) ���̿� ���ܸ� ���� ����.
	///< return  0 : ���ܸ� �ƴ�
	///<        -1 : �� �������� ������
	///<         1 : �� �������� ������
	///< pBxTerm = ���ܸ�BX
	//long GetByunDanStyle(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, CPlateBasicIndex *&pBxTerm);
	
	///< ��������(��������, ������ ����) ���̿� ���ܸ� ���� ����.
	long GetByunDanStyle(long nStt, long nEnd, double &dStaByun);
	
	///< pBx��ġ�� ���ܸ� ���� ����
	long GetByunDanStyle(CPlateBasicIndex *pBx);
	
	///< 1. m_ArrSecSta�� �ܸ��� �����ϰ� 
	///< 2. m_dMatMinSpace(�ܸ���� �ּ� ����) ������ �ܸ� ��ȭ�� ���ؼ� ����(����)
	///< 3. ���� �ܸ� ��ȣ ������
	//void SetSectionNameAndRemoveMinSpace();

	///< SetSectionNameAndRemoveMinSpace()���� ������� �ܸ��� ���Ͽ� ���� �ܸ��� ���� �ܸ� ��ȣ��...
	//void SetSameSectionToOne();

	///< nG�Ŵ��� dSttSta, dEndSta���̰� ���ܸ� ��������?
	//int  IsByunDanStyle(long nG, double dSttSta, double dEndSta);

	///< ���� �߾ӰŴ� �����̼��� ��������?
	BOOL IsJijumByStation(double dSta);
	
	///< x1: ���۵�������ġ
	///< y1: ���۵����Ͱ�
	///< x2: ����������ġ
	///< y2: �������Ͱ�
	///< x : ������ ��ġ��
	double Bogan(double x1, double x2, double y1, double y2, double x);
	
	///< CDWordArray���� sort��
	void InsertionSort(CDWordArray &Data);

	///< ������������ ��ġ�� ������.
	void SetSectionSplice();

	// ���κ� ������ġ�� ������.
	void SetSectionCrossBeamPos();
	
	// �����극�̽��� ������ġ�� ������
	void SetSectionVBracingPos();

	// ����극�̽��� ������ġ�� ������
	void SetSectionHBracingPos();

	// ���κ� ���並 ���� ���⼱ ���Ű� �� �հ� ���ϱ�
	double CalcInfluenceLineCross(double Length,double Distance,long nLoadType,long nForceType,CDDWordArray &DArrY);
	
	// ���κ� ���並 ���� ���⼱ ���Ű� �� �հ� ���ϱ�
	double CalcInfluenceLineStg(double Length,double Distance,long nLoadType,long nForceType,CDDWordArray &DArrY);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// �ܸ����� ���̾�α׿� ���� �������(�� �ȵǾ� ���� Ŭ���� ���� ����)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///< �ܸ����׸��Ʈ �� �ܸ鿡 �ۿ��ϴ� ���Ʈ�� �׸�.
	void OutputOfMomentDiagram(CDomyun *pDom, long nG, double dScaleMoment, double Scale = 300, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, long nML = 0, BOOL bXL = FALSE);
	///< ����ܸ��� ��ġ�� ���ܸ��� ���¸� �׸�
	void OutputOfSectionSummary(CDomyun *pDom, long nG, double GFactor, double scaleH, double scaleY = 300, CPlateBasicIndex *pBxStt = NULL, CPlateBasicIndex *pBxEnd = NULL, long nML = 0);
	///< ����, ����, ������, �氣����, �������� ���� ġ������ �׸�
	void OutputOfDimSectionSummary(CDomyun *pDom, double BaseY, long nDan, CString sTitle, CDDWordArray &DimSta, CDDWordArray &LenSta, CDDWordArray &ArrThick);
	void OutputOfFatigueCheckPos(CDomyun *pDom, BOOL bTitle, double TextHeight=1);
	void OutputOfExtraSection(CDomyun *pDom, CDRect rect);
	// ���ܸ�� ����� �ִ��� ��ġ
	void GetAddMaxForceSect(CDWordArray &DArrnG, CDDWordArray &DArrSta);
	// ���ܸ�� ����� �ּ��� ��ġ
	void GetAddMinSect(CDWordArray &DArrnG, CDDWordArray &DArrSta);
	// �ּҴܸ� �ߺ� �˻�� �߰�
	void CompareHeightAddSect(CDWordArray &DArnG, CDDWordArray &DArSta, CDWordArray &DArnG2, CDDWordArray &DArSta2, CDWordArray &DArType);
	void CompareMaterialRemoveDup(CDWordArray &DArnG, CDDWordArray &DArSta, CDWordArray &DArMat);
	// �������̿��� �ּ� ���� ������ ��ġ ���ϱ�
	double GetStationMinHeight(long nG, double dSttSta, double dEndSta);
	// �Ŵ� ����, ����, ���� �β� ���ϱ�
	double GetThickFlangeBySta(long nMarkBuje, long nG, double dSta, BOOL bSmall = TRUE);
	// �����Ʈ�� �����Ǻ�
	BOOL IsPositive(int nForceType, long nG, double dSta);
	// �÷���&������ ��/�� ������ �������� �ٸ��������� �Ǻ�
	BOOL IsSameSteelMaterial(CPlateBasicIndex *pBx);
	BOOL IsSameSteelMaterial(long nG, double dSta);
};

#endif // !defined(AFX_FEMSTRESSCALC_H__502BF5DE_6071_40FC_B600_C70A2C3BFDEB__INCLUDED_)
