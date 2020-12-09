// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateCalcStd.h: interface for the CAPlateCalcStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATECALCSTD_H__53DBAC70_43A1_425A_B5A6_F719FFE4489D__INCLUDED_)
#define AFX_APLATECALCSTD_H__53DBAC70_43A1_425A_B5A6_F719FFE4489D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataManage;
class CCalcFloor;
class CCalcGeneral;
class CPlateBridgeApp;
class CDesignExpansionJoint;
class CElementResult;
class CJointResult;
class CFEMManage;
class CJoint;
class CCalcData;

#include "ElementResult.h"
#include "JointResult.h"

class AFX_EXT_CLASS CResultArray : public CObject
{
public:
	CResultArray();
	virtual ~CResultArray();

	CTypedPtrArray <CObArray, CElementResult*> m_EleResultArr;
	CTypedPtrArray <CObArray, CJointResult*> m_JointResultArr;
	void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CAPlateCalcStd : public CObject 
{
public:
	CAPlateCalcStd(CDataManage *pDataManage);
	virtual ~CAPlateCalcStd();

	void Remove3DFrameResultData();
	void Serialize(CArchive& ar);

	CDataManage		*m_pDataManage;
	CCalcFloor		*m_pCalcFloor;
	CCalcGeneral    *m_pCalcGeneral;
	CDesignExpansionJoint	*m_pDesignExpanJoint;

	CStringArray	m_szArrModelingFilePath;
	BOOL			m_bMasterLock;
	long			m_nScaleGrade;
	double			m_dAlpha;

	CCentSeparation* GetGuardWall(BOOL bLeft);

	///< 바닥판 모델링	2009.06.30 simdongyoun
	void		MakeCantileverFemData(long nMake);
	void		RemoveCantileverFemData();
	CFEMManage	*GetCantileverFemData(long nIdx)	{ return m_CantileverFemDataArr.GetAt(nIdx);	}
	long		GetSizeCantileverFemData()			{ return m_CantileverFemDataArr.GetSize();		}
	long		GetSizeCantileverFemResult();
	void		GetCantileverElementForceOutput(CStringArray& Arr);
	void		GetCantileverNodeReactionOutput(CStringArray& Arr);

	///< 횡분배관련
	void		MakeHLLFemData(long nMake);
	void        RemoveHLLFemData();
	CFEMManage *GetHLLFemData(long nIdx)	{ return m_HLLFemDataArr.GetAt(nIdx);	}
	long		GetSizeHLLFemData()			  { return m_HLLFemDataArr.GetSize(); }	
	long		GetSizeHLLResult();
	///< 모델링관련
	void		MakeModelingFemData(long nMake);
	void		MakeModelingResult(long nMake);
	void		RemoveModelingFemData();
	CFEMManage *GetModelingFemData(long nAnalysisType);
	CResultArray	*GetModelingResult(long nAnalysisType);
	long		GetSizeModelingFemData()	  { return m_ModelingFemDataArr.GetSize(); }	
	long		GetSizeModelingResult();
	///< 풍하중모델링
	void		MakeWindFemData(long nMake);
	void		RemoveWindFemData();
	CFEMManage *GetWindFemData(long nIdx)	{ return m_WindFemDataArr.GetAt(nIdx);	}
	long		GetSizeWindFemData()		  { return m_WindFemDataArr.GetSize(); }	
	long		GetSizeWindResult();
	///< 영향계수관련
	void		MakeInfluenceFemData(long nMake);
	void		RemoveInfluenceFemData();
	CFEMManage *GetInfluenceFemData(long nIdx)	{ return m_InfluenceFemDataArr.GetAt(nIdx); }
	long		GetSizeInfluenceFemData()	  { return m_InfluenceFemDataArr.GetSize(); }		
	long		GetSizeInfluenceResult();
	// 단면응력 검토
	void		MakeSectionStressData(long nMake);
	void		RemoveSectionStressData();
	CFEMManage	*GetSectionStressData(long nIdx)	{ return m_SectionStressDataArr.GetAt(nIdx);	}
	long		GetSizeSectionStressData()			{ return m_SectionStressDataArr.GetSize();		}
	long		GetSizeSectionStressResult();
	//
	void		MakeResultArray(long nMake);
	void		RemoveResultArray();


	CPlateBridgeApp	*GetBridge()			  { return m_pDataManage->GetBridge(); }
	CCalcData		*GetCalcData()			  { return m_pDataManage->GetCalcData(); }	

	long GetAnalysisTypeByIndex(long nIdx);
	CPlateBasicIndex* GetBxByJoint(CJoint *pJ);

	// 단면응력도 관련
	//void SetExtraDanmyunTotal(CSectionJewon &Mat);
	void SetExtraDanmyun(CSectionJewon &Mat);

private:
	CTypedPtrArray <CObArray, CFEMManage*> m_HLLFemDataArr;
	CTypedPtrArray <CObArray, CFEMManage*> m_ModelingFemDataArr;
	CTypedPtrArray <CObArray, CFEMManage*> m_WindFemDataArr;
	CTypedPtrArray <CObArray, CFEMManage*> m_InfluenceFemDataArr;
	CTypedPtrArray <CObArray, CFEMManage*> m_SectionStressDataArr;
	CTypedPtrArray <CObArray, CFEMManage*> m_CantileverFemDataArr;
	CTypedPtrArray <CObArray, CResultArray*> m_ModelingResultArr;

public:
	// 단면응력도에서 사용.
	double GetStationOnByundanInfo(long nG, long nSec)
	{
		return m_vByunDanInfo[nG][nSec].m_pBx->GetStation();
	}
	BOOL IsByunDanMyunOnByundanInfo(long nG, long nSec)
	{
		return m_vByunDanInfo[nG][nSec].IsByunDanMyun();
	}
	long GetSizeByundanInfo(long nG) { return m_vByunDanInfo[nG].size(); }
	long GetIndexOnByundanInfo(long nG, long nSec)
	{
		return m_vByunDanInfo[nG][nSec].GetIndexUsedMaterial();
	}
	vector<vector<CDanMyunProp> > m_vByunDanInfo;			// 거더별 변화단면 정보	
	vector<CSectionJewon> m_vExtraDanmyun;					// 절점 사이의 추가 단면 정보...
};

#endif // !defined(AFX_APLATECALCSTD_H__53DBAC70_43A1_425A_B5A6_F719FFE4489D__INCLUDED_)
