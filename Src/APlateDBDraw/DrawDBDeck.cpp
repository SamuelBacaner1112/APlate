// DrawDBDeck.cpp: implementation of the CDrawDBDeck class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "APlateDBDraw.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawDBDeck::CDrawDBDeck(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{
}

CDrawDBDeck::~CDrawDBDeck()
{

}


void CDrawDBDeck::DrawRebarShapeArr(CDomyun *pDom, vector<REBAR_SHAPE> &vShape, BOOL bInput, BOOL bShowShapeNo, long nCurShape)
{
	CDomyun Dom(pDom);

	for(long nShape=0; nShape<(long)vShape.size(); nShape++)
	{
		REBAR_SHAPE stRebarShape = vShape.at(nShape);
		if(stRebarShape.m_bValid == FALSE)	continue;

		double dMax = 0;
		CDPoint xyStt, xyEnd, xyCen;
		for(long nPoint=0; nPoint<stRebarShape.m_dArrShape.GetSize()-1; nPoint++)
		{
			xyStt = stRebarShape.m_dArrShape[nPoint];
			xyEnd = stRebarShape.m_dArrShape[nPoint+1];
			CDPoint vAng = (xyEnd-xyStt).Unit();
			if(~(xyStt-xyEnd) > dMax)
			{
				dMax = ~(xyStt-xyEnd);
				xyCen = (xyStt+xyEnd)/2 + vAng.Rotate90()*Dom.GetTextHeight();
			}

			Dom.LineTo(xyStt, xyEnd);
		}
#ifdef _DEBUG
		Dom.SetTextHeight(2);
		Dom.TextOut(xyCen, stRebarShape.m_sName);
#endif
	}

	*pDom << Dom;
}

// 자동으로 치수를 기입해주는 함수 (cont 방식)
// arrText에 실제 치수 대신 들어갈 문자를 지정해 줄 수 있다.
// arrText의 문자는 치수와 인덱스를 비교해서 들어가며, 치수 정리시 치수가 사라지면 문자도 사라진다.
// bLeftMargin : 치수가 하나일때 마진이 적어서 삐져나가는 치수를 왼쪽으로 할지 오른쪽으로 할지..
void CDrawDBDeck::AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDoubleArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin)
{
	pDomP->SetDirection(sDirection);

	if(arr.GetSize() == 0) return;

	double dLen			= 0;
	CString sText		= _T("");
	CString sUnderText	= _T("");
	CString str			= _T("");
	BOOL bAssert		= FALSE;
	CDPoint xy			= xyStt;
	double dTotalLen	= 0;
	long nCount			= 1;
	long nSize			= 0;
	CString sEmptyText	= "0";
	BOOL bIsUnder		= FALSE;	// 이전 치수기입시 치수선 아래에 문자를 기입했는가?
	long i = 0;

	// 치수문자가 치수보다 개수가 적으면 0으로 채워준다.
	// 어짜피 치수문자를 뽑아올때 0이면 없는것으로 인식한다.
	if(arrText.GetSize() < arr.GetSize())
	{
		long nCount = arr.GetSize() - arrText.GetSize();
		for(i = 0; i < nCount; i++)
			arrText.Add(sEmptyText);
	}

	// 아래치수문자가 치수보다 개수가 적으면 0으로 채워준다.
	// 어짜피 치수문자를 뽑아올때 0이면 없는것으로 인식한다.
	if(arrUnderText.GetSize() < arr.GetSize())
	{
		long nCount = arr.GetSize() - arrUnderText.GetSize();
		for(i = 0; i < nCount; i++)
			arrUnderText.Add(sEmptyText);
	}


	// 치수가 0인것은 모두 제거한다. (전혀 필요 없음)
	for(i = 0; i < arr.GetSize(); i++)
	{
		if(arr.GetAt(i) == 0) 
		{
			arr.RemoveAt(i, 1);
			arrText.RemoveAt(i, 1);
			arrUnderText.RemoveAt(i, 1);
		}
	}

	
	// 마이너스인것은 인수에 따라 제거한다.
	// 무조건 마이너스인것만 들어오면 방향을 바꾸기 때문에 필요한건지 아닌지 판단하기 힘듬 
	if(bEraseMinus && arr.GetSize() > 0)
	{
		// 시작이 +이면 -인것을 제거하고 
		// 시작이 -이면 +인것을 제거한다.
		BOOL bDeletePlus = arr.GetAt(0) < 0 ? TRUE : FALSE;
		for(long i = 0; i < arr.GetSize(); i++)
		{
			if((bDeletePlus && arr.GetAt(i) > 0) || (!bDeletePlus && arr.GetAt(i) < 0)) 
			{
				arr.RemoveAt(i, 1);
				arrText.RemoveAt(i, 1);
				arrUnderText.RemoveAt(i, 1);
			}
		}
	}

	nSize = arr.GetSize();
	if(nSize == 0) return;

	// 총길이를 미리 구해 놓는다.
	for(i = 0; i < nSize; i++)
		dTotalLen += arr.GetAt(i);

	// 만약 치수중에 방향이 -방향이면, DimLineToOver사용에 주의를 요한다.
	if(arr.GetAt(0) < 0)
		bAssert = TRUE;

	// 주의를 요하는 경우이면 치수를 반대로 기입하기 위해 시작 위치를 옮긴다.
	// 길이 정보들을 모두 뒤집는다.
	if(bAssert) 
	{
		if(sDirection == "TOP" || sDirection == "BOTTOM") xy.x += dTotalLen;
		else xy.y += dTotalLen;

		double *tmp = new double[nSize];
		CString *tmpText	= new CString[nSize];
		CString *tmpUnderText	= new CString[nSize];

		for(i = 0; i < nSize; i++)
		{
			tmp[i] = fabs(arr.GetAt(nSize-1-i));
			tmpText[i] = arrText.GetAt(nSize-1-i);
			tmpUnderText[i] = arrUnderText.GetAt(nSize-1-i);
		}

		arr.RemoveAll();
		arrText.RemoveAll();
		arrUnderText.RemoveAll();
		for(i = 0; i < nSize; i++)
		{
			arr.Add(tmp[i]);
			arrText.Add(tmpText[i]);
			arrUnderText.Add(tmpUnderText[i]);
		}

		delete [] tmp;
		delete [] tmpText;
		delete [] tmpUnderText;
	}


	// 실제로 치수 기입 하는 부분 ******************************************************/
	BOOL bInitMoveTo = FALSE;
	for(i = 0; i < nSize; i++)
	{
		dLen		= arr.GetAt(i);
		sText		= arrText.GetAt(i);
		sUnderText	= arrUnderText.GetAt(i);

		if(dLen == 0) continue;

		// 치수 기입을 위한 텍스트 설정중 /////////////////////////////
		// 반복되는 치수가 있는지검사한다.
		if((i < nSize-1) && bRepetition) 
		{
			if(EQUAL(dLen, arr.GetAt(i+1), 0.001))
			{
				nCount++;
				continue;
			}
		}
		
		if(sText == sEmptyText)
		{
			if(nCount > 1)
			{
				str.Format("%d@%s=%s", nCount, COMMA(dLen), COMMA(dLen*nCount));
				if(sUnderText == sEmptyText && pDomP->GetTextWidth(str) > dLen*nCount)
				{
					str.Format("%d@%s", nCount, COMMA(dLen));
					sUnderText.Format("=%s", COMMA(dLen*nCount));

					if(!bInitMoveTo) 
					{
						str += sUnderText;
						sUnderText = "0";
					}
				}
			}
			else
				str = COMMA(dLen);
		}
		else
		{
			str = sText;
		}

		// 치수 기입 ///////////////////////////////////////////////////
		// 치수가 최소 마진보다 적으면 아래로 내려 쓴다.
		// 최소마진이 -1이면 자동으로 판단한다.
		double dMinWidth = dMinMargin == -1 ? pDomP->GetTextWidth(str) : dMinMargin;
		if(fabs(dLen*nCount) <= dMinWidth)
		{
			if(i == 0)
			{
				if(!bInitMoveTo)
				{
					// 치수가 하나밖에 없을때 마진방향이 오른쪽이라면..
					if(nSize-1 == i && !bLeftMargin)
					{
						pDomP->DimMoveTo(xy.x, xy.y, nDimDan);
						bInitMoveTo = TRUE;

						if(sUnderText == sEmptyText) sUnderText = "";
						pDomP->DimLineToOverExtend(dLen*nCount, nDimDan, str, sUnderText);
					}
					else
					{
						pDomP->DimMoveTo(xy.x, xy.y, nDimDan, str);
						bInitMoveTo = TRUE;

						if(sUnderText == sEmptyText) sUnderText = "";
						pDomP->DimLineTo(dLen*nCount, nDimDan, sUnderText);
					}
				}
				else
				{
					if(sUnderText == sEmptyText)
						pDomP->DimLineTo(dLen*nCount, nDimDan, "");
					else
						pDomP->DimLineToExtend(dLen*nCount, nDimDan, "", sUnderText);

					bIsUnder	= FALSE;
				}
			}
			else if(i == arr.GetSize()-1)
			{
				if(!bInitMoveTo) 
				{
					pDomP->DimMoveTo(xy.x, xy.y, nDimDan, str);
					bInitMoveTo = TRUE;

					if(sUnderText == sEmptyText) sUnderText = "";
					pDomP->DimLineTo(dLen*nCount, nDimDan, sUnderText);
				}
				else
				{
					if(sUnderText == sEmptyText)
						pDomP->DimLineToOver(dLen*nCount, nDimDan, str);
					else
						pDomP->DimLineToOverExtend(dLen*nCount, nDimDan, str, sUnderText);
					bIsUnder	= FALSE;
				}
			}
			else
			{
				if(!bInitMoveTo) 
				{
					pDomP->DimMoveTo(xy.x, xy.y, nDimDan, str);
					bInitMoveTo = TRUE;

					if(sUnderText == sEmptyText) sUnderText = "";
					pDomP->DimLineTo(dLen*nCount, nDimDan, sUnderText);
				}
				else
				{
					if(sUnderText == sEmptyText)
					{
						if(!bIsUnder)
							pDomP->DimLineToExtend(dLen*nCount, nDimDan, "", str);
						else
							pDomP->DimLineTo(dLen*nCount, nDimDan, str);
					}
					else
					{
						pDomP->DimLineToExtend(dLen*nCount, nDimDan, str, sUnderText);
					}
				}

				bIsUnder	= TRUE;
			}
		}
		else
		{
			if(i == 0 || nCount == nSize)
			{
				if(!bInitMoveTo)
				{
					pDomP->DimMoveTo(xy.x, xy.y, nDimDan);
					bInitMoveTo = TRUE;

					if(sUnderText == sEmptyText)
						pDomP->DimLineTo(dLen*nCount, nDimDan, str);
					else
						pDomP->DimLineToExtend(dLen*nCount, nDimDan, str, sUnderText);
				}
				else
				{
					if(sUnderText == sEmptyText)
						pDomP->DimLineTo(dLen*nCount, nDimDan, str);
					else
						pDomP->DimLineToExtend(dLen*nCount, nDimDan, str, sUnderText);

					bIsUnder	= FALSE;
				}
			}
			else
			{
				if(!bInitMoveTo)
				{
					pDomP->DimMoveTo(xy.x, xy.y, nDimDan);
					bInitMoveTo = TRUE;

					if(sUnderText == sEmptyText)
						pDomP->DimLineTo(dLen*nCount, nDimDan, str);
					else
						pDomP->DimLineToExtend(dLen*nCount, nDimDan, str, sUnderText);

				}
				else
				{
					if(sUnderText == sEmptyText)
						pDomP->DimLineTo(dLen*nCount, nDimDan, str);
					else
						pDomP->DimLineToExtend(dLen*nCount, nDimDan, str, sUnderText);
					bIsUnder	= FALSE;
				}
			}
		}

		nCount = 1;
	}


	// 전체 치수 기입하는 경우이면..
	if(bIsTotalDim)
	{
		str = COMMA(fabs(dTotalLen));
			
		if(str != "")
		{
			pDomP->DimMoveTo(xyStt.x, xyStt.y, nDimDan+1);
			pDomP->DimLineTo(dTotalLen, nDimDan+1, str);
		}
	}

	// ****************************************************** 실제로 치수 기입 하는 부분 /
}

// 좌표들을 받아와서.. 자동으로 cont 치수를 기입한다.
// arrText에 실제 치수 대신 들어갈 문자를 지정해 줄 수 있다.
// arrText의 문자는 치수와 인덱스를 비교해서 들어가며, 치수 정리시 치수가 사라지면 문자도 사라진다.
// bAli : 대각선 길이 적용할지? (기본은 수평수직만 됨)
// bInput : 입력창 형태의 치수인지?(이때는 sArrText에는 기호로 들어오고.. sArrUnderText에는 자동으로 길이값이 들어간다.)
// bAutoModifyStt : 시작위치에 대해서 자동으로 보정할지(수평치수일땐 수직위치를 보정하고, 수직치수일땐 수평위치를 보정한다)
void CDrawDBDeck::AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDPointArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin, BOOL bAli, BOOL bInput, BOOL bAutoModifyStt)
{
	// 
	if(arr.GetSize() < 2) return;
	double dLen = 0;
	CDoubleArray arrLen;
	BOOL bHor = sDirection == "TOP" || sDirection == "BOTTOM" ? TRUE : FALSE;
	if(bInput)
	{
		arrUnderText.RemoveAll();
		arrUnderText.Add("0");
	}

	if(bAutoModifyStt)
	{
		if(bHor)
		{
			BOOL bMax	= (sDirection == "TOP") ? TRUE : FALSE;
			long nSize	= arr.GetSize();
			double dStt	= arr.GetAt(0).y;
			for(long i = 1; i < nSize; i++)
			{
				dStt	= bMax ? max(arr.GetAt(i).y, dStt) : min(arr.GetAt(i).y, dStt);
			}
			xyStt.y = dStt;
		}
		else
		{
			BOOL bMax = (sDirection == "RIGHT") ? TRUE : FALSE;
			long nSize	= arr.GetSize();
			double dStt	= arr.GetAt(0).y;
			for(long i = 1; i < nSize; i++)
			{
				dStt	= bMax ? max(arr.GetAt(i).x, dStt) : min(arr.GetAt(i).x, dStt);
			}
			xyStt.x = dStt;
		}
	}

	// 길이정보로 편집.
	for(long i = 0; i < arr.GetSize()-1; i++)
	{
		if(bAli)
		{
			dLen = ~(arr.GetAt(i+1) - arr.GetAt(i));
		}
		else
		{
			if(bHor)
				dLen = arr.GetAt(i+1).x - arr.GetAt(i).x;
			else
				dLen = arr.GetAt(i+1).y - arr.GetAt(i).y;
		}

		arrLen.Add(dLen);
		if(bInput) arrUnderText.Add(COMMA(dLen));
	}

	// 길이 정보로 치수 기입 
	if(arrText.GetSize() > 0) arrText.RemoveAt(0);
	if(arrUnderText.GetSize() > 0) arrUnderText.RemoveAt(0);
	
	AutoDimCont(pDomP, xyStt, arrLen, arrText, arrUnderText, nDimDan, sDirection, dMinMargin, bIsTotalDim, bRepetition, bEraseMinus, bLeftMargin);

	// 실제 길이로 기입할 경우 방향에 따라 회전시켜 줘야 된다.
	if(bAli && arr.GetSize() > 1)
	{
		double dAng = (arr.GetAt(arr.GetSize()-1)-arr.GetAt(0)).GetAngleDegree();
		dAng = bHor ? dAng : dAng - 90;
		CDPoint xyDir = ToDPointFrRadian(ToRadian(dAng));
		pDomP->Rotate(arr.GetAt(0), xyDir);
	}
}

// 라인 배열로 선을 그린다.
void CDrawDBDeck::DrawLineArray(CDomyun *pDomP, CDPointArray &xyArr)
{
	if(xyArr.GetSize() < 2) return;
	if(xyArr.GetSize() % 2 != 0) xyArr.RemoveAt(xyArr.GetSize()-1);

	for(long i = 0; i < xyArr.GetSize()-1; i+=2)
		pDomP->LineTo(xyArr.GetAt(i), xyArr.GetAt(i+1));
}

// 그룹 코드 초기화
void CDrawDBDeck::InitCurGroupCode()
{
	m_nCurGroupCode = 1;
}

void CDrawDBDeck::SettingCurGroupCode(BOOL bVal)
{
	m_bSettingGroupCode = bVal;
}

CObInsert* CDrawDBDeck::InsertHCadBlock(CDomyun *pDomP, const CString &szBlkName, CDPoint xy, double dScale, double dRotDegAng, BOOL bHCADOnly)
{	
//	m_pBlockNameArray = NULL;
	CString szCovBlkName = szBlkName;
//	if(m_pBlockNameArray)
//	{
//		for(long n=0; n<m_pBlockNameArray->GetSize(); n++)
//		{
//			if(m_pBlockNameArray->GetAt(n) == szCovBlkName)
//			{
//				szCovBlkName = m_pBlockUserNameArray->GetAt(n);
//				break;
//			}
//		}
//	}
	// 표준 레이어 생성
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_SYMB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARS);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARX);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTL);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTT);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS1);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS2);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_TXTG);
	m_pARoadOptionStd->SetEnvType(pDomP, HCADEX_GISI);
	m_pARoadOptionStd->SetEnvType(pDomP, HCADEX_GIST);
	CString szImsiCode = m_pARoadOptionStd->m_szDetLayerCode;
	m_pARoadOptionStd->m_szDetLayerCode = "";
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_SYMB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARS);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARX);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTL);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTT);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS1);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS2);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_TXTG);
	m_pARoadOptionStd->SetEnvType(pDomP, HCADEX_GISI);
	m_pARoadOptionStd->SetEnvType(pDomP, HCADEX_GIST);
	m_pARoadOptionStd->m_szDetLayerCode = szImsiCode;

	BOOL bFind = m_HCadSymbolDom.IsExistBlock(szCovBlkName+"-NOATTR");
	if(m_bUseHCadBlock || !bFind || bHCADOnly)
	{
		if(m_HCadSymbolDom.IsExistBlock(szCovBlkName))
		{
			if(!pDomP->IsExistBlock(szCovBlkName))
			{
				CMapStringToPtr* pBlockArray = m_HCadSymbolDom.GetBlockArray();
				CDomyunBase* p;					
				if( pBlockArray->Lookup( szCovBlkName, (void *&)p ) )
				{
					CObInsert* pInsert = new CObInsert, *pInsertFind=NULL;
					CPtrList *pInsertList = m_HCadSymbolDom.GetObjectList(ETT_INSERT);
					POSITION pos = pInsertList->GetHeadPosition();
					while(pos)
					{
						CObInsert* p = (CObInsert*)pInsertList->GetNext(pos);
						if(p->m_sBlockName == szCovBlkName) { pInsertFind = p; break; }
					}				 
					pInsert->SetsLayer(m_pARoadOptionStd->m_szDetLayerCode + ((szCovBlkName == "CSBSEC1") ? "CS-BARX" : "CS-SYMB"));
					pInsert->m_sBlockName = szCovBlkName;
					pInsert->m_vPoint = xy;	
					pInsert->m_vSFactor = CVector(dScale,dScale,dScale);
					pInsert->m_RotationAngle = dRotDegAng;					
					if(pInsertFind)
					{
						AhPtrListCopy(pInsert->m_AttDefArr,pInsertFind->m_AttDefArr,(CObAttdef*)0);														
						pos = pInsert->m_AttDefArr.GetHeadPosition();
						while(pos)
						{
							CObAttdef* p = (CObAttdef*)pInsert->m_AttDefArr.GetNext(pos);						
							p->m_ptPoint = xy + (p->m_ptPoint - pInsertFind->m_vPoint) * dScale;
							p->m_ptPointSecond = xy + (p->m_ptPointSecond - pInsertFind->m_vPoint) * dScale;
							p->m_TextStyle.Height *= dScale;
						}
					}				
					
					CDomyunBase *pBlockDom = new CDomyunBase;					
					*pBlockDom = *p;
					m_pARoadOptionStd->ChangeLayerOnSet(pInsert);
					m_pARoadOptionStd->ChangeDomLastLayerNameForAPier(pBlockDom);					
					pDomP->AddBlockAndInsert(pBlockDom, pInsert);						
					return pInsert;
				}
			}	
			else
			{									
				CObInsert* pInsert = new CObInsert, *pInsertFind=NULL;	
				CPtrList *pInsertList = m_HCadSymbolDom.GetObjectList(ETT_INSERT);
				POSITION pos = pInsertList->GetHeadPosition();
				while(pos)
				{
					CObInsert* p = (CObInsert*)pInsertList->GetNext(pos);
					if(p->m_sBlockName == szCovBlkName) { pInsertFind = p; break; }
				}		
				if(pInsertFind)
				{
					AhPtrListCopy(pInsert->m_AttDefArr,pInsertFind->m_AttDefArr,(CObAttdef*)0);														
					pos = pInsert->m_AttDefArr.GetHeadPosition();
					while(pos)
					{
						CObAttdef* p = (CObAttdef*)pInsert->m_AttDefArr.GetNext(pos);						
						p->m_ptPoint = xy + (p->m_ptPoint - pInsertFind->m_vPoint) * dScale;
						p->m_ptPointSecond = xy + (p->m_ptPointSecond - pInsertFind->m_vPoint) * dScale;
						p->m_TextStyle.Height *= dScale;
					}
				}	
				pInsert->SetsLayer(m_pARoadOptionStd->m_szDetLayerCode + (szCovBlkName == "CSBSEC1" ? "CS-BARX" : "CS-SYMB"));
				pInsert->m_sBlockName = szCovBlkName;
				pInsert->m_vPoint = xy;	
				pInsert->m_vSFactor = CVector(dScale,dScale,dScale);
				m_pARoadOptionStd->ChangeLayerOnSet(pInsert);
				pDomP->Insert(pInsert);				
				return pInsert;
			}
		}
	}
	else
	{
		if(m_HCadSymbolDom.IsExistBlock(szCovBlkName+"-NOATTR"))
		{
			CMapStringToPtr* pBlockArray = m_HCadSymbolDom.GetBlockArray();
			CDomyun Dom(pDomP);
			CDomyunBase* p;					
			if( pBlockArray->Lookup( szCovBlkName+"-NOATTR", (void *&)p ) )
			{				
				m_pARoadOptionStd->ChangeDomLastLayerNameForAPier(p);
				Dom += *p;												
				Dom.RedrawByScale(dScale);
				Dom.Move(xy);
				*pDomP << Dom;				
			}
		}
	}

	return NULL;	
}


BOOL CDrawDBDeck::IsUseHCadBlock()
{
	return m_bUseHCadBlock;
}

void CDrawDBDeck::SetUseHCadBlock(BOOL vUseBlock)
{
	m_bUseHCadBlock = vUseBlock;
}

CDomyun* CDrawDBDeck::GetHCadSymbolDom()
{
	return &m_HCadSymbolDom;
}

// rectCut를 기준으로 절단선 표시 까지해서 tvArr를 그려준다.
void CDrawDBDeck::DrawCuttingTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine)
{
	CRebarPlacing rbPlacing;
	CVectorArray vecArrArea;
	vecArrArea.Add(CVector(rectCut.left, rectCut.top, 0));
	vecArrArea.Add(CVector(rectCut.right, rectCut.top, 0));

	vecArrArea.Add(CVector(rectCut.right, rectCut.top, 0));
	vecArrArea.Add(CVector(rectCut.right, rectCut.bottom, 0));

	vecArrArea.Add(CVector(rectCut.right, rectCut.bottom, 0));
	vecArrArea.Add(CVector(rectCut.left, rectCut.bottom, 0));

	vecArrArea.Add(CVector(rectCut.left, rectCut.bottom, 0));
	vecArrArea.Add(CVector(rectCut.left, rectCut.top, 0));

	// 커팅라인의 요지가 보이는 곳을 체크함.
	BOOL bLeft = FALSE;
	BOOL bRight = FALSE;
	BOOL bTop = FALSE;
	BOOL bBottom = FALSE;
	CDRect rectTvArr = tvArr.GetRect();	// 그릴 그림의 사각좌표
	
	// 검사
	if(rectCut.left >= rectTvArr.left) bLeft = TRUE;
	if(rectCut.right <= rectTvArr.right) bRight = TRUE;
	if(rectCut.top <= rectTvArr.top) bTop = TRUE;
	if(rectCut.bottom >= rectTvArr.bottom) bBottom = TRUE;

	// 원래꺼 손상시키면 곤란하므로...
	CTwinVectorArray tvArrTmp;
	tvArrTmp = tvArr;
	rbPlacing.GetTvArrayByCrossVectorArrayArea(tvArrTmp, vecArrArea, TRUE);

	CDomyun Dom(pDomP);
	DrawTvArray(&Dom, tvArrTmp);

	// 잘린 부분에 대해서 커팅라인 표시 한다.
	// 사방을 다 검사.
	if(tvArrTmp.GetSize() > 0 && bDrawCuttingLine)
	{
		m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);

		BOOL bDir = FALSE;
		DWORD dwArea1 = FALSE;
		DWORD dwArea2 = FALSE;
		BOOL bVer = FALSE;
		double dLenExt = Dom.Always(5);
		CDPointArray xyArr1;
		CDPointArray xyArr2;
		tvArrTmp.GetToPointArray(xyArr1);
		tvArrTmp.GetToPointArray(xyArr2);

		for(long i = 0; i < 4; i++)
		{
			switch(i)
			{
			case 0: bDir = bLeft; dwArea1 = LEFTDOWN_AREA; dwArea2 = LEFTUP_AREA; bVer = TRUE; break;
			case 1: bDir = bRight; dwArea1 = RIGHTDOWN_AREA; dwArea2 = RIGHTUP_AREA; bVer = TRUE; break;
			case 2: bDir = bTop; dwArea1 = UPLEFT_AREA; dwArea2 = UPRIGHT_AREA; bVer = FALSE; break;
			case 3: bDir = bBottom; dwArea1 = DOWNLEFT_AREA; dwArea2 = DOWNRIGHT_AREA; bVer = FALSE; break;
			}

			if(bDir)
			{
				CDPoint xy1 = rbPlacing.GetXyEdgePoint(xyArr1, dwArea1);
				CDPoint xy2 = rbPlacing.GetXyEdgePoint(xyArr2, dwArea2);
				
				// 두 좌표가 같은 경우..
				if(ComparePt(xy1, xy2, "="))
				{
					switch(i)
					{
					case 0: xy1 = CDPoint(rectCut.left, rectCut.bottom); xy2 = CDPoint(rectCut.left, rectCut.top); break;
					case 1: xy1 = CDPoint(rectCut.right, rectCut.bottom); xy2 = CDPoint(rectCut.right, rectCut.top); break;
					case 2: xy1 = CDPoint(rectCut.left, rectCut.top); xy2 = CDPoint(rectCut.right, rectCut.top); break;
					case 3: xy1 = CDPoint(rectCut.left, rectCut.bottom); xy2 = CDPoint(rectCut.right, rectCut.bottom); break;
					}
				}

				CDPoint xyDir = (xy2-xy1).Unit();
				xy1 += xyDir*-dLenExt;
				xy2 += xyDir*dLenExt;

				Dom.CutLightning(xy1.x, xy1.y, xy2.x, xy2.y, bVer, 1);
			}
		}
	}
	
	*pDomP << Dom;
}

void CDrawDBDeck::SetAttrib(CDomyun *pDom, CObInsert *pIns, const CString& szTag, const CString& szText)
{				
	if(m_bUseHCadBlock || ( pIns && pIns->m_AttDefArr.GetCount()>0))
	{		
		if(!pIns) return;
		POSITION pos = pIns->m_AttDefArr.GetHeadPosition();
		while(pos)
		{
			CObAttdef* p = (CObAttdef*)pIns->m_AttDefArr.GetNext(pos);						
			if(p->m_szTag == szTag) 
			{
				p->m_szText = szText;
				break;
			}
		}
	}
	else
	{				
		pDom->ReplaceString(szTag, szText);
	}
}

// rectCut영역만큼 가로로 잘라서 절단표시까지 해줌(기둥같은거 그릴때 유용할것임)
void CDrawDBDeck::DrawCuttingTvArrayHorRange(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine)
{
	CTwinVectorArray tvArrUpper, tvArrLower;	// 위, 아래에 남는 선들
	CRebarPlacing rbPlacing;

	CDomyun Dom(pDomP);
	if(rbPlacing.CuttingHorRange(tvArr, rectCut, tvArrUpper, tvArrLower, TRUE, 100))
	{
		DrawTvArray(pDomP, tvArrUpper);
		DrawTvArray(pDomP, tvArrLower);

		if(bDrawCuttingLine)
		{
			// 둘중 넓은걸 기준으로 컷팅라인을 그리낟.
			CDRect rectUpper = tvArrUpper.GetRect();
			CDRect rectLower = tvArrLower.GetRect();
			double dLeft = min(rectUpper.left, rectLower.left);
			double dRight = max(rectUpper.right, rectLower.right);

			m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);

			Dom.CutLightning(dLeft, rectUpper.bottom, dRight, rectUpper.bottom, FALSE, 1);
			Dom.CutLightning(dLeft, rectLower.top, dRight, rectLower.top, FALSE, 1);
		}
	}
	else
	{
		DrawTvArray(pDomP, tvArr);
	}

	*pDomP << Dom;
}

// rebar cycle info 그리기.
void CDrawDBDeck::DrawRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, long nDrawLeft, CDRect rectLimit)
{
	if(!pRC) return;
	if(Compare(pRC->GetLength(), 0.0, "=")) return;

	CTwinVectorArray tvArr;
	CTwinVector tv;

	if(pRC->m_bUseTvArrRebar)
	{
		tvArr	= pRC->m_tvArrRebar;
	}
	else
	{
		// 라운드 적용할지?(철근이 2개이상인경우..)
		BOOL bApplyRound	= FALSE;//!pBri->m_bBendingDanBuRebar;

		// 기초에서 끝나는 철근은 무조건 라운드 적용 안함.
		if(pRC->m_nPosEnd == REBARINFO_POS_FOOTING_UPPER || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LOWER ||
			pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LEFT || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_RIGHT)
			bApplyRound	= FALSE;
		if(Compare(pRC->m_R_Mid, 0.0, "=")) bApplyRound	= FALSE;

		
		if(pRC->m_nPosStt != pRC->m_nPosEnd)
		{
			tv.m_v1	= pRC->m_xyStt;
			tv.m_v2	= pRC->m_xyMid1;
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tvArr.Add(tv);

			tv.m_v1	= tv.m_v2;
			tv.m_v2	= pRC->m_xyMid2;
			if(bApplyRound)
			{
				tv.m_v1.z	= 1;
				tv.m_v2.z	= fabs(pRC->m_xyMid2.x - pRC->m_xyMid1.x);
				// 아크 그릴때 주의사항 
				// 1. 철근진행 방향이 우측으로 +, 위로 +인경우 아크타입 1에 원 진행 방향을 뒤집어야 됨..
				// 2. 나머지의 경우 아크타입 -1로 그린다.
				// 그래야 정상적으로 그려짐.. ㅋㅋ
				if(!(Compare(pRC->m_xyMid1.x, pRC->m_xyStt.x, ">") && Compare(pRC->m_xyEnd.y, pRC->m_xyMid2.y, ">")))
				{
					GetSwap(tv.m_v1, tv.m_v2);
					GetSwap(tv.m_v1.z, tv.m_v2.z);
				}
			}
			tvArr.Add(tv);

			tv.m_v1	= pRC->m_xyMid2;
			tv.m_v2	= pRC->m_xyEnd;
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tvArr.Add(tv);

			if(pRC->m_bUseEndExt)
			{
				tv.m_v1		= pRC->m_xyEnd;
				tv.m_v2		= pRC->m_xyEndExt;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArr.Add(tv);
			}
		}
		else
		{
			tv.m_v1	= pRC->m_xyStt;
			tv.m_v2	= pRC->m_xyMid1;
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tvArr.Add(tv);
		}
	}
	
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		
		// 영역 안쪽만 남김.
		CRebarPlacing rb;
		CVectorArray vecArrArea;
		tvArrLimit.GetToVectorArray(vecArrArea);
		rb.GetTvArrayByCrossVectorArrayArea(tvArr, vecArrArea, TRUE);
	}

	if(nDrawLeft != 0)
	{
		CRebarPlacing rb;
		CDRect rect	= tvArr.GetRect();
		double dStaCuttingOrg	= GetStaCuttingOrg();
		CDPoint xy1	= CDPoint(dStaCuttingOrg, rect.bottom-100),
				xy2	= CDPoint(dStaCuttingOrg, rect.top+100);

		CVectorArray vecArr;
		tvArr.GetToVectorArray(vecArr);
		if(nDrawLeft == -1)
		{
			rb.TrimVectorArrayByLine(vecArr, xy1, xy2, FALSE, TRUE, FALSE);
		}
		else if(nDrawLeft == 1)
		{
			rb.TrimVectorArrayByLine(vecArr, xy1, xy2, TRUE, TRUE, FALSE);
		}
		
		tvArr.SetFromVectorArray(vecArr);
	}

	DrawTvArray(pDomP, tvArr);
}


// rebarinfocycle array에서 치수기입용 정보를 분석해서 리턴해준다.
// sArrInfo 내용 : 
// 0 : 빈공간
// 1 : 철근이 들어가는 공간(입력받는 철근의 길이)
// 2 : 입력받지 않는 철근이 들어가는 공간(라운드나 모따기 같은거)
// BOOL bApplyJoint : 이음을 고려해서 정보를 분석할지?
// BOOL bApplyEndIs2 : 마지막 철근을 무조건 2로 설정할지?
// BOOL bReverse : 빈공간과 철근이 들어가는 공간을 반대로 찾음.. (2는 그대로)
void CDrawDBDeck::GetDimInfoAtRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bHor, CDPointArray &xyArrDim, CStringArray &sArrInfo, BOOL bMerge, BOOL bApplyJoint, BOOL bApplyEndIs2, BOOL bReverse, double dCoverStt, double dCoverEnd)
{
	if(!bMerge)
	{
		xyArrDim.RemoveAll();
		sArrInfo.RemoveAll();
	}
	

	// 신축이음을 고려하지 않은 함수이다. /////////////////////////////////////
	// 따라서 rc를 신축이음이 있는 두개는 합쳐서 하나로 만든 뒤에 
	// 치수기입 정보를 생성한다.
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> pArrRCTmp;
	long i = 0;
	for(i = 0; i < pArrRC->GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);
		CRebarInfoCycle *pRCNew	= new CRebarInfoCycle;

		*pRCNew = *pRC;

		if(i > 0 && i < pArrRC->GetSize()-1)
		{
			if(pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_SIDE)
			{
				CRebarInfoCycle *pRCNext	= pArrRC->GetAt(i+1);
				pRCNew->m_nPosEnd	= pRCNext->m_nPosEnd;
				if(pRCNext->m_nPosStt == pRCNext->m_nPosEnd)
					pRCNew->m_xyMid1	= pRCNext->m_xyMid1;
				else
					pRCNew->m_xyMid1	= pRCNext->m_xyEnd;
				i++;
			}
		}
		
		pArrRCTmp.Add(pRCNew);
	}
	/////////////////////////////////////////////////////////////////////////////

	



	CDPoint xyLast(0, 0);
	CDPoint xyDir			= bHor ? CDPoint(1, 0) : CDPoint(0, 1);
	CDPoint xyDirOrtho		= bHor ? CDPoint(0, 1) : CDPoint(1, 0);
	double dLengthJointStt	= 0;
	double dLengthJointEnd	= 0;
	long nSize				= pArrRCTmp.GetSize();	
	
	// 시작위치와 철근진행방향 판단.
	if(nSize > 0) 
	{
		CRebarInfoCycle *pRC = pArrRCTmp.GetAt(0);
		CDPoint xyStt(0, 0);
		CDPoint xyEnd(0, 0);
		if(pRC->m_nPosStt != pRC->m_nPosEnd)
		{
			if(!GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), pRC->m_xyEnd, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyLast))
				xyLast	= pRC->m_xyMid1;
			
			xyStt	= pRC->m_xyMid2;
			xyEnd	= pRC->m_xyEnd;
		}
		else
		{
			xyLast	= pRC->m_xyStt;
			xyStt	= pRC->m_xyStt;
			xyEnd	= pRC->m_xyMid1;
		}
	
		xyDir		= (xyEnd-xyStt).Unit();
		xyDirOrtho	= ToDPointFrDegree(xyDir.GetAngleDegree()+90);

		xyArrDim.Add(xyLast);
		sArrInfo.Add("0");
		if(nSize == 1)
		{
			xyArrDim.Add(xyEnd);
			sArrInfo.Add("0");

			// 피복 정보 추가한다.
			// 처음과 끝부분에 피복 정보 추가한다. ////////////////
			if(xyArrDim.GetSize() > 1)
			{
				CDPoint xy1	= xyArrDim.GetAt(0);
				CDPoint xy2	= xyArrDim.GetAt(xyArrDim.GetSize()-1);

				xy1 = xy1 + (xy1-xy2).Unit() * dCoverStt;
				xyArrDim.InsertAt(0, xy1);
				sArrInfo.InsertAt(0, "0");

				xy2	= xy2 + (xy2-xy1).Unit() * dCoverEnd;
				xyArrDim.Add(xy2);
				sArrInfo.Add("0");
			}
			////////////////////////////////////////////////////////

			AhTPADelete(&pArrRCTmp, (CRebarInfoCycle*)0);
			return;	// 1개만 있을 경우 이렇게만 하고 마무리한다. 뒤집고 자시고 없다.
		}
	}
	
	for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC = pArrRCTmp.GetAt(i);

		// 시작, 중간, 끝에 적용 되어야 할 이음 길이..
		if(bApplyJoint)
		{
			if(i == 0)
			{
				dLengthJointStt	= 0;
				dLengthJointEnd	= LENGTH_JOINT/2;
			}
			else if(i == nSize-1)
			{
				dLengthJointStt	= LENGTH_JOINT/2;
				dLengthJointEnd	= 0;
			}
			else 
			{
				dLengthJointStt	= LENGTH_JOINT/2;
				dLengthJointEnd	= LENGTH_JOINT/2;
			}
		}


		// 시작위치와 끝위치가 다르면 모따기(라운드)가 있다.
		if(pRC->m_nPosStt != pRC->m_nPosEnd)
		{
			CDPoint xyStt	= pRC->m_xyStt;

			if(i == 0)
			{
				if(!GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), pRC->m_xyEnd, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyStt))
					xyStt = pRC->m_xyMid1;
			}
			xyStt			= xyStt + (xyDir*dLengthJointStt);
			CDPoint xyMid1	= pRC->m_xyMid1;
			CDPoint xyMid2	= pRC->m_xyMid2;
			CDPoint xyEnd	= pRC->m_xyEnd;
			if(i == nSize-1)
			{
				if(!GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), pRC->m_xyEnd, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyEnd))
					xyEnd	= pRC->m_xyMid2;
			}
			xyEnd			= xyEnd + (-xyDir*dLengthJointEnd);

			if(!ComparePt(xyLast, xyStt, "="))
			{
				xyArrDim.Add(xyStt);
				sArrInfo.Add("0");
			}	

			if(i == 0)
			{
				xyArrDim.Add(xyMid2);
				sArrInfo.Add("2");

				xyArrDim.Add(xyEnd);
				sArrInfo.Add("1");
			}
			else if(i == nSize-1)
			{
				xyArrDim.Add(xyMid1);
				sArrInfo.Add("1");

				xyArrDim.Add(xyEnd);
				sArrInfo.Add("2");
			}

			xyLast = xyEnd + (xyDir * dLengthJointEnd);
		}
		// 같으면 직선
		else 
		{
			CDPoint xyStt	= pRC->m_xyStt + (xyDir*dLengthJointStt);
			CDPoint xyMid1	= pRC->m_xyMid1 + (-xyDir*dLengthJointEnd);
			if(!ComparePt(xyLast, xyStt, "="))
			{
				xyArrDim.Add(xyStt);
				sArrInfo.Add("0");
			}

			xyArrDim.Add(xyMid1);
			sArrInfo.Add("1");


			xyLast = xyMid1 + (xyDir * dLengthJointEnd);
		}
	}


	// reverse이면 1과 0을 뒤바꿈.
	if(bReverse)
	{
		nSize	= sArrInfo.GetSize();
		for(i = 0; i < nSize; i++)
		{
			if(sArrInfo.GetAt(i) == "0") sArrInfo.SetAt(i, "1");
			else if(sArrInfo.GetAt(i) == "1") sArrInfo.SetAt(i, "0");
		}
	}


	// 마지막에 있는 "1"은 무조건 "2"가 되어야 한다.(없으면 고만이고..)
	if(bApplyEndIs2)
	{
		nSize = sArrInfo.GetSize();
		for(i = nSize-1; i >= 0; i--)
		{
			if(sArrInfo.GetAt(i) == "1")
			{
				sArrInfo.SetAt(i, "2");
				break;
			}
		}
	}

	// 피복 정보 추가한다.
	// 처음과 끝부분에 피복 정보 추가한다. ////////////////
	if(xyArrDim.GetSize() > 1)
	{
		CDPoint xy1	= xyArrDim.GetAt(0);
		CDPoint xy2	= xyArrDim.GetAt(xyArrDim.GetSize()-1);

		xy1 = xy1 + (xy1-xy2).Unit() * dCoverStt;
		xyArrDim.InsertAt(0, xy1);
		sArrInfo.InsertAt(0, "0");

		xy2	= xy2 + (xy2-xy1).Unit() * dCoverEnd;
		xyArrDim.Add(xy2);
		sArrInfo.Add("0");
	}
	////////////////////////////////////////////////////////

	AhTPADelete(&pArrRCTmp, (CRebarInfoCycle*)0);
}

// 이음 그리기.
// long nDrawLeft : -1 기준점의 좌측만 그림, 0 고려안하고 다 그림, 1 기준점의 우측만 그림
// double dStaCuttingOrg : 기준 sta
void CDrawDBDeck::DrawJoint(CDomyun *pDomP, CRebarJoint &joint, long nDrawLeft, CDRect rectLimit)
{
	CDPoint xyStt	= joint.m_xyPos + (-joint.m_xyDir * joint.m_dLength/2);
	CDPoint xyEnd	= xyStt + (joint.m_xyDir * joint.m_dLength);

	if(joint.m_bDrawUpper)
		Offset(xyStt, xyEnd, GAP_JOINT);
	else
		Offset(xyStt, xyEnd, -GAP_JOINT);

	CTwinVectorArray tvArr;
	CTwinVector tv;

	if(nDrawLeft == 0)
	{
		tv.m_v1	= xyStt;
		tv.m_v2	= xyEnd;
		tvArr.Add(tv);
	}
	else
	{
		double dStaCuttingOrg	= GetStaCuttingOrg();	// 기준점 찾음.

		CRebarPlacing rb;
		CDPoint xyDir	= (xyEnd-xyStt).Unit();
		// 왼쪽만 그리는 경우
		if(nDrawLeft == -1)
		{
			if(rb.IsParallel(xyDir, CDPoint(0, 1)))
			{
				if(xyStt.x > dStaCuttingOrg)
				{
					xyStt.x = dStaCuttingOrg;
					xyEnd	= xyStt;
				}
			}
			else
			{
				if(xyStt.x > dStaCuttingOrg)
					GetXyMatchLineAndLine(xyStt, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyStt);
				if(xyEnd.x > dStaCuttingOrg)
					GetXyMatchLineAndLine(xyEnd, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyEnd);
			}
		}
		// 오른쪽만 그리는 경우.
		else if(nDrawLeft == 1)
		{
			if(rb.IsParallel(xyDir, CDPoint(0, 1)))
			{
				if(xyStt.x < dStaCuttingOrg)
				{
					xyStt.x = dStaCuttingOrg;
					xyEnd	= xyStt;
				}
			}
			else
			{
				if(xyStt.x < dStaCuttingOrg)
					GetXyMatchLineAndLine(xyStt, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyStt);
				if(xyEnd.x < dStaCuttingOrg)
					GetXyMatchLineAndLine(xyEnd, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyEnd);
			}
		}

		if(!ComparePt(xyStt, xyEnd, "="))
		{
			tv.m_v1	= xyStt;
			tv.m_v2	= xyEnd;
			tvArr.Add(tv);
		}
	}

	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		
		// 영역 안쪽만 남김.
		CRebarPlacing rb;
		CVectorArray vecArrArea;
		tvArrLimit.GetToVectorArray(vecArrArea);
		rb.GetTvArrayByCrossVectorArrayArea(tvArr, vecArrArea, TRUE);
	}

	DrawTvArray(pDomP, tvArr);
}

// 이음 그리기
void CDrawDBDeck::DrawJointByRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC)
{
/*	if(!pArrRC) return;
	CJointArray arrJoint;

	// rebarinfocycle에서 이음 정보 분석
	m_pRcBridgeRebar->GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);

	// 이음 그리기
	DrawJointArray(pDomP, arrJoint);*/
	
}

// rebarinfocycle 배열 그려대기..
void CDrawDBDeck::DrawRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nDrawLeft, CDRect rectLimit)
{
	if(!pArrRC) return;
	long nSize = pArrRC->GetSize();	
	for(long i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC = pArrRC->GetAt(i);
		if(pRC)
		{
			DrawRebarInfoCycle(pDomP, pRC, nDrawLeft, rectLimit);
		}
	}
}

void CDrawDBDeck::DrawJointArray(CDomyun *pDomP, CJointArray &arrJoint, long nDrawLeft, CDRect rectLimit)
{
	// 찾아진 이음 정보로 하나씩 그림.
	long nSize	= arrJoint.GetSize();
	for(long i = 0; i < nSize; i++)
	{
		DrawJoint(pDomP, arrJoint.GetAt(i), nDrawLeft, rectLimit);
	}
}

// rebarinfocycle 철근 마킹하기.
// 마킹 범위를 찾아서 dan에 따라 마킹한다.
// 1단 : 가운데지점에서 오른쪽으로
// 2단 : 왼쪽지점에서 왼쪽으로
// 3단 : 오른쪽지점에서 오른쪽으로
// bUpperAndLeft : 위로나 왼쪽으로 마킹할지?
// bSide : 측면 마킹할지?
// long nTypeStt : 마지막 철근이 시작과 끝이 다른 타입일때의 마킹타입
// long nTypeEnd : 마지막 철근이 시작과 끝이 다른 타입일때의 마킹타입
// 마킹타입 0 - 첫번째 선, 1 - 가운데 선, 2 - 마지막 선
void CDrawDBDeck::MarkRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nType, long nDrawLeft, CDRect rectLimit, double dScalePos)
{
/*	if(!pRC) return;
	if(Compare(pRC->GetLength(), 0.0, "=")) return;
	if(m_sMarkForMarkRebar != "")
	{
		if(m_sMarkForMarkRebar != pRC->m_szMark) return;
	}

	if(!Compare(dHeightMark, 0.0, "=")) dHeightMark += pDomP->Always(MARK_BASE_HEIGHT);
	
	CDomyun Dom(pDomP);
	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CConcSymbol sym(m_pARoadOptionStd);
	CString sMark	= pRC->m_szMark;
	CString sDia	= m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia);
	long nDan		= pRC->m_nRebarDan;
	CDPoint xyStt	= CDPoint(0, 0);
	CDPoint xyEnd	= CDPoint(0, 0);
	CDPoint xyMark	= CDPoint(0, 0);
	BOOL bLeft		= TRUE;
	double dStaCuttingOrg	= GetStaCuttingOrg();

	// tvarr을 사용한 경우 마킹할 위치를 xyStt와 xyEnd에 넣어준다.
	if(pRC->m_bUseTvArrRebar)
	{
		long nSize	= pRC->m_tvArrRebar.GetSize();
		if(nSize > 0)
		{
			CTwinVector tv	= pRC->m_tvArrRebar.GetAt((nSize-1)/2);
			xyStt			= tv.m_v1;
			xyEnd			= tv.m_v2;
		}
	}
	else
	{
		// 마킹할 선 찾음
		if(pRC->m_nPosStt == pRC->m_nPosEnd)
		{
			xyStt	= pRC->m_xyStt;
			xyEnd	= pRC->m_xyMid1;
		}
		else
		{
			if(nType == 0)
			{
				xyStt	= pRC->m_xyStt;
				xyEnd	= pRC->m_xyMid1;
			}
			else if(nType == 1)
			{
				xyStt	= pRC->m_xyMid1;
				xyEnd	= pRC->m_xyMid2;
			}
			else if(nType == 2)
			{
				xyStt	= pRC->m_xyMid2;
				xyEnd	= pRC->m_xyEnd;
			}
		}
	}

	// 마킹할 지점 찾음
	long nDivide	= 5;
	if(nDan == 0)
	{
		double dDist	= ~(xyStt-xyEnd);
		CDPoint xyDir	= (xyEnd-xyStt).Unit();

		xyMark	= xyStt+xyDir*dDist*dScalePos;
		bLeft	= FALSE;
	}
	else if(nDan == 1)
	{
		double dDist	= ~(xyStt-xyEnd);
		CDPoint xyDir	= (xyEnd-xyStt).Unit();

		double dScaleStt	= 1-((1-dScalePos)*2);
		xyMark			= (xyStt+xyDir*dDist*dScaleStt) + (xyDir * dDist*dScalePos/nDivide * nDan+1);
		bLeft			= TRUE;
	}
	else if(nDan == 2)
	{
		double dDist	= ~(xyStt-xyEnd);
		CDPoint xyDir	= (xyEnd-xyStt).Unit();

		double dScaleStt	= 1-((1-dScalePos)*2);
		xyMark			= (xyStt+xyDir*dDist*dScaleStt) + (xyDir * dDist*dScalePos/nDivide * (nDivide-nDan+1));
		bLeft			= FALSE;
	}

	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		tvArrLimit.SetFromRect(rectLimit);

		CVectorArray vecArrLimit;
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		if(!rb.CheckXyByVectorArrayArea(xyMark, vecArrLimit, TRUE)) return;
	}

	
	if(nDrawLeft != 0)
	{
		// 왼쪽만 그림
		if(nDrawLeft == -1)
		{
			if(xyMark.x > dStaCuttingOrg) return;
		}
		// 오른쪽만 그림
		else if(nDrawLeft == 1)
		{
			if(xyMark.x < dStaCuttingOrg) return;
		}
	}

	// 예외적으로 피해야 되는 위치에 대해서 여기서 조정을 할 수 있다. ////////////////////////
	CRcBridgeRebar *pBri	= m_pRcBridgeRebar;
	// 1. 외측벽체 내측치수와 위치가 비슷하면 비켜주자.
	// 단, 상부슬래브 철근만 철근만 적용하되 헌치부 철근은 적용안함
	if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER || 
		pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER)
	{
		// tv를 이용한것은 헌치부 철근이다. 적용하지 않음
		if(!pRC->m_bUseTvArrRebar)
		{
			double dRange	= Dom.GetTextWidth("000000");
			double dOffset	= Dom.Always(Dom.GetDimLevelDis()*2 + Dom.GetDimVLen() + Dom.GetDimExo());
			for(long stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				if(!pBri->IsOutWall(bStt)) continue;

				CTwinVector tvWall	= pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "시점벽체우측면" : "종점벽체좌측면");
				tvWall.m_v1.x	+= bStt ? dOffset : -dOffset;
				if(tvWall.m_v1.x + dRange > xyMark.x && tvWall.m_v1.x - dRange < xyMark.x)
				{
					// 범위에 걸리면 시점측에 걸린거면 종점측으로 아니면 반대로 이동
					double dDist		= bStt ? fabs(xyMark.x - tvWall.m_v1.x) : fabs(xyMark.x - tvWall.m_v1.x) + dRange;
					CDPoint xyDirMove	= bStt ? (xyEnd-xyStt).Unit() : (xyStt-xyEnd).Unit();
					xyMark	= xyMark + xyDirMove * (dRange/2 + dDist);
					if(bStt && xyMark.x > xyEnd.x) xyMark	= xyEnd;
					if(!bStt && xyMark.x < xyStt.x) xyMark	= xyStt;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////



	// 길이 정보
	double dLen	= pRC->GetLength();
	
	if(bSide)
	{
		if(bUpperAndLeft)
		{
			sym.RebarMarkLineSideEx(&Dom, xyMark, sMark, sDia, dLen, 0, TRUE, dHeightMark);
		}
		else
		{
			sym.RebarMarkLineSideEx(&Dom, xyMark, sMark, sDia, dLen, 0, FALSE, dHeightMark);
		}
	}
	else
	{
		if(bUpperAndLeft)
		{
			sym.RebarMarkLineUpperEx(&Dom, xyMark, sMark, sDia, dLen, 0, bLeft, dHeightMark);
		}
		else
		{
			dHeightMark += Dom.Always(MARK_BASE_RADIUS*1.5);
			sym.RebarMarkLineLowerEx(&Dom, xyMark, sMark, sDia, dLen, 0, bLeft, dHeightMark);
		}
	}

	*pDomP << Dom;*/
}

// rebarinfocycle 철근 마킹하기.
// 마킹 범위를 찾아서 dan에 따라 마킹한다.
// 1단 : 가운데지점에서 오른쪽으로
// 2단 : 왼쪽지점에서 왼쪽으로
// 3단 : 오른쪽지점에서 오른쪽으로
// bUpperAndLeft : 위로나 왼쪽으로 마킹할지?
// bSide : 측면 마킹할지?
// long nTypeStt : 마지막 철근이 시작과 끝이 다른 타입일때의 마킹타입
// long nTypeEnd : 마지막 철근이 시작과 끝이 다른 타입일때의 마킹타입
// 마킹타입 0 - 첫번째 선, 1 - 가운데 선, 2 - 마지막 선
void CDrawDBDeck::MarkRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nTypeStt, long nTypeEnd, long nDrawLeft, CDRect rectLimit, double dScalePos)
{
	if(!pArrRC) return;

	long nSize		= pArrRC->GetSize();
	long nType		= 2;
	
	for(long i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);

		if(i == 0) nType = nTypeStt;
		else if(i == nSize-1) nType = nTypeEnd;
		else nType = 2;

		// 슬래브 측면에써 끝날경우 상대선에서 철근마크 해야 됨
		if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_SIDE)
			nType = 2;
		else if(pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_SIDE)
			nType = 0;
		

		MarkRebarInfoCycle(pDomP, pArrRC->GetAt(i), bUpperAndLeft, bSide, dHeightMark, nType, nDrawLeft, rectLimit, dScalePos);
	}
}

// 선형 따라 글려야 되는 선인지...
BOOL CDrawDBDeck::IsDrawByLineInfo(long nPosInRebarInfoCycle)
{
	BOOL bLine	= FALSE;
	if(nPosInRebarInfoCycle == REBARINFO_POS_UPPERSLAB_UPPER || 
		nPosInRebarInfoCycle == REBARINFO_POS_UPPERSLAB_LOWER || 
		nPosInRebarInfoCycle == REBARINFO_POS_LOWERSLAB_UPPER || 
		nPosInRebarInfoCycle == REBARINFO_POS_LOWERSLAB_LOWER)
	{
		bLine	= TRUE;
	}

	return bLine;
}


// PlaceRebarByBlock에서 치수기입용 정보를 빼온다. ㅋㅋ
void CDrawDBDeck::GetDimInfoAtPlaceRebarByBlock(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, BOOL bInput, BOOL bMerge)
{
	if(!bMerge)
	{
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	if(!pPlace) return;

	long nSize	= pPlace->m_xyArr.GetSize();
	CDPoint xyR(0, 0);
	CString sNum	= _T("");
	for(long block = 0; block < pPlace->m_nCountBlock; block++)
	{
		if(nSize-1 < block) break;

		xyR	= pPlace->m_xyArr.GetAt(block);
		
		dArrDim.Add(xyR.x*xyR.y);
		if(bInput)
		{
			sNum.Format("%s%d", sMark, block+1);
			sArrText.Add(sNum);
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrUnderText.Add(sNum);
		}
		else
		{
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrText.Add(sNum);
			sArrUnderText.Add("0");
		}
	}
}

// PlaceRebarByBlock에서 치수기입용 정보를 빼온다. ㅋㅋ
void CDrawDBDeck::GetDimInfoAtPlaceRebarByBlock(CDPointArray &xyArr, CDPointArray &xyArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, CDPoint xyStt, CDPoint xyDir, BOOL bInput, BOOL bMerge)
{
	if(!bMerge)
	{
		xyArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	if(!pPlace) return;

	long nSize	= pPlace->m_xyArr.GetSize();
	if(nSize < 1) return;

	xyArrDim.Add(xyStt);
	sArrText.Add("0");
	sArrUnderText.Add("0");

	CDPoint xyR(0, 0);
	CString sNum	= _T("");
	for(long block = 0; block < pPlace->m_nCountBlock; block++)
	{
		if(nSize-1 < block) break;
		xyR	= pPlace->m_xyArr.GetAt(block);

		xyStt	= xyStt + (xyDir * (xyR.x*xyR.y));
		xyArrDim.Add(xyStt);

		if(bInput)
		{
			sNum.Format("%s%d", sMark, block+1);
			sArrText.Add(sNum);
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrUnderText.Add(sNum);
		}
		else
		{
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrText.Add(sNum);
			sArrUnderText.Add("0");
		}
	}
}


// 옵션에 따라서 EL값을 리턴해준다.
CString CDrawDBDeck::GetStringEL(double dEL)
{
	// nTypeEL -> 0 : EL. 15.000, 1 : EL.:15.000, 2 : EL.=15.000
	long nTypeEL	= 0;//pOpt->m_nTypeEL;	
	CString sEL	= _T("");
	switch(nTypeEL)
	{
		case 0: sEL.Format("EL. %.3f", dEL); break;
		case 1: sEL.Format("EL.:%.3f", dEL); break;
		case 2: sEL.Format("EL.=%.3f", dEL); break;
		default: sEL.Format("EL. %.3f", dEL); break;
	}

	return sEL;
}

// RebarInfoCycle 배열에 치수 넣기(주철근 조립도 용)
// bLeft : 진행방향의 좌측에 치수 넣을지 우측에 치수 넣을지... 
// long nDrawLeft : -1 기준점 좌측에만 치수 기입, 0 기준점 고려 안함, 1 : 기준점 우측에만 치수 기입.
// 수직으로는 아래위로 진행해도 되지만, 수평으로는 반드시 좌에서 우로만 진행해야 제대로 치수가 기입된다.
// BOOL bAli : 실거리로 치수기입 할지...?
void CDrawDBDeck::DimRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, CDPoint xyStt, CDPoint xyEnd, BOOL bLeft, long nDimDan, long nDrawLeft, BOOL bAli)
{
/*	if(!pArrRC) return;
	CJointArray arrJoint;
	// rebarinfocycle에서 이음 정보 분석
	m_pRcBridgeRebar->GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);

	
	CDomyun Dom(pDomP);
	

	// 기준점의 영향을 받아서 치수가 기입되는 방법이 변할 수 잇다
	// -1 : 기입하지 않음, 0 : 정상 기입, 1 : 잘려서 기입됨.(arc에서는 잘려서 기입되는거 고려 안됨)
	long nTypeDim			= 0;	
	double dStaCuttingOrg	= GetStaCuttingOrg();
	BOOL bHor				= FALSE;
	double dLen				= 0;
	CDPoint xyLast			= CDPoint(0, 0);
	CRebarInfoCycle *pRC	= NULL;

	long nSize	= pArrRC->GetSize();
	CDPoint xyDirRotate	= CDPoint(1, 0);
	if(bAli && nSize > 0)
	{
		// 실거리 치수 기입시 회전할 방향 판단.
		// 첫번째 rc로 판단함.
		pRC	= pArrRC->GetAt(nSize-1);

		double dAng	= 0;
		BOOL bHor	= IsDrawByLineInfo(pRC->m_nPosEnd) ? TRUE : FALSE;
		if(pRC->m_nPosStt == pRC->m_nPosEnd)
		{
			dAng	= (pRC->m_xyMid1-pRC->m_xyStt).GetAngleDegree();
		}
		else
		{
			dAng	= (pRC->m_xyEnd-pRC->m_xyMid2).GetAngleDegree();
		}

		dAng	= bHor ? dAng : dAng-90;
		xyDirRotate	= ToDPointFrDegree(dAng);
	}


	for(long rc = 0; rc < nSize; rc++)
	{
		pRC	= pArrRC->GetAt(rc);

		// 라운드 적용할지?(철근이 2개이상인경우..) /////////////////
		BOOL bApplyRound	= FALSE;//!pBri->m_bBendingDanBuRebar;
		
		// 기초에서 끝나는 철근은 무조건 라운드 적용 안함.
		if(pRC->m_nPosEnd == REBARINFO_POS_FOOTING_UPPER || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LOWER ||
			pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LEFT || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_RIGHT)
			bApplyRound	= FALSE;
		/////////////////////////////////////////////////////////////

		bApplyRound	= TRUE;
		nTypeDim	= 0;

		if(rc == 0)
		{
			bHor		= IsDrawByLineInfo(pRC->m_nPosStt) ? TRUE : FALSE;
			nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyStt, pRC->m_xyStt, nDrawLeft, dStaCuttingOrg);
			GetDimInfoByTypeDim(xyStt, pRC->m_xyStt, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);
			dLen	= bAli ? ~(pRC->m_xyStt - xyStt) : (bHor ? pRC->m_xyStt.x - xyStt.x : pRC->m_xyStt.y - xyStt.y);
		
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
			if(dLen > Dom.GetTextWidth(COMMA(fabs(dLen))))
			{
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
					if(bAli)
					{
						Dom.Rotate(xyStt, xyDirRotate);
						*pDomP << Dom;
					}
				}
			}
			else
			{
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, COMMA(fabs(dLen)));
					Dom.DimLineTo(dLen, nDimDan, "");
					if(bAli)
					{
						Dom.Rotate(xyStt, xyDirRotate);
						*pDomP << Dom;
					}
				}
			}
			xyLast	= pRC->m_xyStt;
		}
		
		if(Compare(pRC->GetLength(), 0.0, "=")) continue;

		if(pRC->m_nPosStt == pRC->m_nPosEnd)
		{
 			// 끝날때는 마지막 위치가 이음이 들어가는 위치인지 파악해야 된다. ///////////
			BOOL bJoint		= FALSE;
			CRebarJoint joint;
			CDPoint xySttJoint	= CDPoint(0, 0);
			CDPoint xyEndJoint	= CDPoint(0, 0);
			for(long i = 0; i < arrJoint.GetSize(); i++)
			{
				joint	= arrJoint.GetAt(i);
				xySttJoint	= joint.m_xyPos + (-joint.m_xyDir * joint.m_dLength/2);
				xyEndJoint	= xySttJoint + (joint.m_xyDir * joint.m_dLength);
				
				if(ComparePt(xyEndJoint, pRC->m_xyMid1, "=") || ComparePt(xySttJoint, pRC->m_xyMid1, "="))
				{
					bJoint	= TRUE;
					if(ComparePt(xySttJoint, pRC->m_xyMid1, "=")) GetSwap(xySttJoint, xyEndJoint);
					break;
				}
			}
			////////////////////////////////////////////////////////////////////////////////
			bHor	= IsDrawByLineInfo(pRC->m_nPosStt) ? TRUE : FALSE;
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
			if(bJoint)
			{
				// dStaCuttingOrg 고려해서 좌표 편집들어감.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, xySttJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xyLast, xySttJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				dLen	= bAli ? ~(xySttJoint - xyLast) : (bHor ? (xySttJoint.x-xyLast.x) : (xySttJoint.y-xyLast.y));
				if(!Compare(dLen, 0.0, "="))
				{
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}

					Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
					if(bAli)
					{
						Dom.Rotate(xyLast, xyDirRotate);
						*pDomP << Dom;
					}
				}

				// dStaCuttingOrg 고려해서 좌표 편집들어감.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xySttJoint, xyEndJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xySttJoint, xyEndJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);
				
				dLen	= bAli ? ~(xyEndJoint-xySttJoint) : (bHor ? (xyEndJoint.x-xySttJoint.x) : (xyEndJoint.y-xySttJoint.y));
				if(!Compare(dLen, 0.0, "="))
				{
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
				
					Dom.DimMoveTo(xySttJoint.x, xySttJoint.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
					if(bAli)
					{
						Dom.Rotate(xySttJoint, xyDirRotate);
						*pDomP << Dom;
					}
				}

				xyLast	= xyEndJoint;
			}
			else
			{
				// dStaCuttingOrg 고려해서 좌표 편집들어감.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, pRC->m_xyMid1, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xyLast, pRC->m_xyMid1, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				dLen	= bAli ? ~(pRC->m_xyMid1-xyLast) : (bHor ? (pRC->m_xyMid1.x-xyLast.x) : (pRC->m_xyMid1.y-xyLast.y));
				if(!Compare(dLen, 0.0, "="))
				{
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}

					Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
					if(bAli)
					{
						Dom.Rotate(xyLast, xyDirRotate);
						*pDomP << Dom;
					}
				}

				// 다음 철근 시작까지 치수 기입
				if(rc < nSize-1)
				{
					CRebarInfoCycle *pRCNext	= pArrRC->GetAt(rc+1);

					// dStaCuttingOrg 고려해서 좌표 편집들어감.
					nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid1, pRCNext->m_xyStt, nDrawLeft, dStaCuttingOrg);
					GetDimInfoByTypeDim(pRC->m_xyMid1, pRCNext->m_xyStt, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

					dLen	= bAli ? ~(pRCNext->m_xyStt-pRC->m_xyMid1) : (bHor ? (pRCNext->m_xyStt.x-pRC->m_xyMid1.x) : (pRCNext->m_xyStt.y-pRC->m_xyMid1.y));
					if(!Compare(dLen, 0.0, "="))
					{
						if(!bHor && dLen < 0)
						{
							Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
						}
					
						Dom.DimMoveTo(pRC->m_xyMid1.x, pRC->m_xyMid1.y, nDimDan);
						Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
						if(bAli)
						{
							Dom.Rotate(pRC->m_xyMid1, xyDirRotate);
							*pDomP << Dom;
						}
					}

					xyLast	= pRCNext->m_xyStt;
				}
				else
					xyLast	= pRC->m_xyMid1;
			}
		}
		else
		{
			bHor	= IsDrawByLineInfo(pRC->m_nPosStt) ? TRUE : FALSE;

			// dStaCuttingOrg 고려해서 좌표 편집들어감.
			nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, pRC->m_xyMid1, nDrawLeft, dStaCuttingOrg);
			GetDimInfoByTypeDim(xyLast, pRC->m_xyMid1, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);
		
			dLen	= bAli ? ~(pRC->m_xyMid1-xyLast): (bHor ? (pRC->m_xyMid1.x-xyLast.x) : (pRC->m_xyMid1.y-xyLast.y));
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
			if(!Compare(dLen, 0.0, "="))
			{
				if(!bHor && dLen < 0)
				{
					Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
				}

				Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
				Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
				if(bAli)
				{
					Dom.Rotate(xyLast, xyDirRotate);
					*pDomP << Dom;
				}
			}

			// arc
			if(bApplyRound)
			{
				// dStaCuttingOrg 고려해서 좌표 편집들어감.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid1, pRC->m_xyMid2, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(pRC->m_xyMid1, pRC->m_xyMid2, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				CRebarPlacing rb;
				double dR	= fabs(pRC->m_xyMid2.x - pRC->m_xyMid1.x);
				dLen		= rb.GetArcLength(pRC->m_xyCen, pRC->m_xyMid2, pRC->m_xyMid1);

				// arc 치수 넣을때는 반드시 TOP으로 설정해야 원하는 대로 나온다.
				if(nTypeDim == 0)
				{
					Dom.SetDirection("TOP");
					if(!Compare(dLen, 0.0, "="))
					{
						Dom.DimArcMoveTo(pRC->m_xyCen.x, pRC->m_xyCen.y, dR, pRC->m_xyMid2.x, pRC->m_xyMid2.y, nDimDan);
						Dom.DimArcLineTo(pRC->m_xyMid1.x, pRC->m_xyMid1.y, nDimDan, "R="+COMMA(fabs(dR)));
					}
				}
			}
			else
			{
			}

			// 끝날때는 마지막 위치가 이음이 들어가는 위치인지 파악해야 된다. ///////////
			BOOL bJoint		= FALSE;
			CRebarJoint joint;
			CDPoint xySttJoint	= CDPoint(0, 0);
			CDPoint xyEndJoint	= CDPoint(0, 0);
			for(long i = 0; i < arrJoint.GetSize(); i++)
			{
				joint	= arrJoint.GetAt(i);
				xySttJoint	= joint.m_xyPos + (-joint.m_xyDir * joint.m_dLength/2);
				xyEndJoint	= xySttJoint + (joint.m_xyDir * joint.m_dLength);
				
				if(ComparePt(xyEndJoint, pRC->m_xyEnd, "=") || ComparePt(xySttJoint, pRC->m_xyEnd, "="))
				{
					bJoint	= TRUE;
					if(ComparePt(xySttJoint, pRC->m_xyEnd, "=")) GetSwap(xySttJoint, xyEndJoint);
					break;
				}
			}
			////////////////////////////////////////////////////////////////////////////////

			bHor	= IsDrawByLineInfo(pRC->m_nPosEnd) ? TRUE : FALSE;
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));

			if(bJoint)
			{
				// dStaCuttingOrg 고려해서 좌표 편집들어감.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid2, xySttJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(pRC->m_xyMid2, xySttJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				dLen	= bAli ? ~(xySttJoint-pRC->m_xyMid2) : (bHor ? (xySttJoint.x-pRC->m_xyMid2.x) : (xySttJoint.y-pRC->m_xyMid2.y));

				if(!Compare(dLen, 0.0, "="))
				{
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}	

					Dom.DimMoveTo(pRC->m_xyMid2.x, pRC->m_xyMid2.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
					if(bAli)
					{
						Dom.Rotate(pRC->m_xyMid2, xyDirRotate);
						*pDomP << Dom;
					}
				}

				// dStaCuttingOrg 고려해서 좌표 편집들어감.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xySttJoint, xyEndJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xySttJoint, xyEndJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				dLen	= bAli ? ~(xyEndJoint-xySttJoint) : (bHor ? (xyEndJoint.x-xySttJoint.x) : (xyEndJoint.y-xySttJoint.y));
				if(!Compare(dLen, 0.0, "="))
				{
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					Dom.DimMoveTo(xySttJoint.x, xySttJoint.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
					if(bAli)
					{
						Dom.Rotate(xySttJoint, xyDirRotate);
						*pDomP << Dom;
					}
				}

				xyLast	= xyEndJoint;
			}
			else
			{
				// dStaCuttingOrg 고려해서 좌표 편집들어감.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid2, pRC->m_xyEnd, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(pRC->m_xyMid2, pRC->m_xyEnd, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				dLen	= bAli ? ~(pRC->m_xyEnd-pRC->m_xyMid2) : (bHor ? (pRC->m_xyEnd.x-pRC->m_xyMid2.x) : (pRC->m_xyEnd.y-pRC->m_xyMid2.y));
				if(!Compare(dLen, 0.0, "="))
				{
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					Dom.DimMoveTo(pRC->m_xyMid2.x, pRC->m_xyMid2.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
					if(bAli)
					{
						Dom.Rotate(pRC->m_xyMid2, xyDirRotate);
						*pDomP << Dom;
					}
				}

				// 다음 철근 시작까지 치수 기입
				if(rc < nSize-1)
				{
					CRebarInfoCycle *pRCNext	= pArrRC->GetAt(rc+1);

					// dStaCuttingOrg 고려해서 좌표 편집들어감.
					nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyEnd, pRCNext->m_xyStt, nDrawLeft, dStaCuttingOrg);
					GetDimInfoByTypeDim(pRC->m_xyEnd, pRCNext->m_xyStt, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

					dLen	= bAli ? ~(pRCNext->m_xyStt-pRC->m_xyEnd) : (bHor ? (pRCNext->m_xyStt.x-pRC->m_xyEnd.x) : (pRCNext->m_xyStt.y-pRC->m_xyEnd.y));
					if(!Compare(dLen, 0.0, "="))
					{
						if(!bHor && dLen < 0)
						{
							Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
						}
						Dom.DimMoveTo(pRC->m_xyEnd.x, pRC->m_xyEnd.y, nDimDan);
						Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
						if(bAli)
						{
							Dom.Rotate(pRC->m_xyEnd, xyDirRotate);
							*pDomP << Dom;
						}
					}

					xyLast	= pRCNext->m_xyStt;
				}
				else
					xyLast	= pRC->m_xyEnd;
			}
		}
	}

	// 모든 치수 기입후 마지막 치수 기입
	if(nSize > 0)
	{
		// dStaCuttingOrg 고려해서 좌표 편집들어감.
		nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, xyEnd, nDrawLeft, dStaCuttingOrg);
		GetDimInfoByTypeDim(xyLast, xyEnd, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

		dLen	= bAli ? ~(xyEnd-xyLast) : (bHor ? (xyEnd.x-xyLast.x) : (xyEnd.y-xyLast.y));
		if(!Compare(dLen, 0.0, "="))
		{
			if(!bHor && dLen < 0)
			{
				Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
				Dom.DimMoveTo(xyEnd.x, xyEnd.y, nDimDan, COMMA(fabs(dLen)));
				Dom.DimLineTo(fabs(dLen), nDimDan, "");
				if(bAli)
				{
					Dom.Rotate(xyEnd, xyDirRotate);
					*pDomP << Dom;
				}
			}
			else
			{
				Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
				if(Dom.GetTextWidth(COMMA(fabs(dLen))) > fabs(dLen))
					Dom.DimLineToOver(dLen, nDimDan, COMMA(fabs(dLen)));
				else
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLen)));
				if(bAli)
				{
					Dom.Rotate(xyLast, xyDirRotate);
					*pDomP << Dom;
				}
			}
		}
	}
	else
	{
		// dStaCuttingOrg 고려해서 좌표 편집들어감.
		nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyStt, xyEnd, nDrawLeft, dStaCuttingOrg);
		GetDimInfoByTypeDim(xyStt, xyEnd, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

		dLen	= ~(xyEnd-xyStt);
		if(!Compare(dLen, 0.0, "="))
		{
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyStt.x, xyEnd.y, nDimDan);
			Dom.DimLineTo(dLen, nDimDan, COMMA(dLen));
		
			Dom.Rotate(xyStt, (xyEnd-xyStt).Unit());
			*pDomP << Dom;
		}
	}


	*pDomP << Dom;*/
}

// 좌우측 잘라서 표현할 경우 기준이 되는 sta 리턴
double CDrawDBDeck::GetStaCuttingOrg()
{
	double dSta	= 0;
	// 지간수가 홀수이면 중간 지간을 반으로 나눈 위치,
	// 지간수가 짝수이면 중간 지점 위치.
/*	long nQtyJigan	= m_pRcBridge->m_nQtyJigan;
	if(nQtyJigan%2 == 0)
	{
		long nQtyJijum	= nQtyJigan+1;
		long nMidJijum	= nQtyJijum/2;

		dSta	= pStd->m_pDB->GetStationOnJijum(nMidJijum, 0);	
	}
	else
	{
		long nJijumPrev	= nQtyJigan/2;
		long nJijumNext	= nJijumPrev+1;

		double dStaJijumPrev	= m_pRcBridge->GetStationOnJijum(nJijumPrev, 0);
		double dStaJijumNext	= m_pRcBridge->GetStationOnJijum(nJijumNext, 0);

		dSta	= (dStaJijumPrev+dStaJijumNext)/2;
	}
*/
	return dSta;
}

// rebarinfocycle 치수 기입시 기준점 고려해서 치수 기입 타입을 결정한다.
long CDrawDBDeck::GetTypeDimRebarInfoCycle(BOOL bHor, CDPoint xyStt, CDPoint xyEnd, long nDrawLeft, double dStaCuttingOrg)
{
	long nTypeDim	= 0;

	// dStaCuttingOrg 고려 /////////////////////////////////////////////////
	if(nDrawLeft != 0)
	{
		if(!bHor)
		{
			if(nDrawLeft == -1)
			{
				if(xyStt.x > dStaCuttingOrg) nTypeDim	= -1;
			}
			else if(nDrawLeft == 1)
			{
				if(xyStt.x < dStaCuttingOrg) nTypeDim	= -1;
			}
		}
		else
		{
			if(nDrawLeft == -1)
			{
				if(xyStt.x >= dStaCuttingOrg) nTypeDim	= -1;
				else if(xyEnd.x > dStaCuttingOrg) nTypeDim	= 1;
			}
			else if(nDrawLeft == 1)
			{
				if(xyEnd.x <= dStaCuttingOrg) nTypeDim	= -1;
				else if(xyStt.x < dStaCuttingOrg) nTypeDim	= 1;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return nTypeDim;
}

// typeDim에 따라서 실제로 기입 되는 치수의 위치를 수정한다.
void CDrawDBDeck::GetDimInfoByTypeDim(CDPoint &xyStt, CDPoint &xyEnd, BOOL bHor, long nTypeDim, long nDrawLeft, double dStaCuttingOrg)
{
	// dStaCuttingOrg 고려 안함일때는 리턴
	if(nTypeDim == 0) return;
	if(nDrawLeft == 0) return;

	// 아예 치수 기입 하지 않을때는 다음 위치 고려해서 끝점으로 통일해서 리턴
	if(nTypeDim == -1)
	{
		xyStt	= xyEnd;
		return;
	}


	// 잘려서 치수 기입 될때.
	if(nTypeDim	== 1)
	{
		if(bHor)
		{
			// 왼쪽만 기입하는데 잘려야 되면 마지막 치수를 수정
			if(nDrawLeft == -1)
			{
				xyEnd.x	= dStaCuttingOrg;
			}
			else if(nDrawLeft == 1)
			{
				xyStt.x	= dStaCuttingOrg;
			}
		}
		else
		{
			// 수직일때는 잘려서 기입 되는 일이 없으므로 그냥 리턴.
			return;
		}
	}
}

// 간격배열을 반복치수 기입 정보로 변환
void CDrawDBDeck::GetDimInfoAtDistArray(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CDoubleArray &dArrDist, CString sMark, BOOL bInput, BOOL bMerge, BOOL bHor)
{
	if(!bMerge)
	{
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}


	// 간격을 offset 정보로 변환
	CRebarPlacing rb;
	CDPointArray xyArr;
	rb.ConvertOffsetDoubleArrayToOffsetInfo(dArrDist, xyArr);
	
	long nSize	= xyArr.GetSize();
	CDPoint xyR(0, 0);
	CString sNum	= _T("");
	for(long i = 0; i < nSize; i++)
	{
		if(nSize-1 < i) break;

		xyR	= xyArr.GetAt(i);
		
		dArrDim.Add(xyR.x*xyR.y);
		if(bInput)
		{
			sNum.Format("%s%d", sMark, i+1);
			sArrText.Add(sNum);
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrUnderText.Add(sNum);
		}
		else
		{
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrUnderText.Add("0");
			sArrText.Add(sNum);
		}
	}
}

// rect 두개를 하나의 rect로 머지
CDRect CDrawDBDeck::MergeRectAndRect(CDRect rect1, CDRect rect2)
{
	CDRect rect;
	rect.left	= rect1.left < rect2.left ? rect1.left : rect2.left;
	rect.bottom	= rect1.bottom > rect2.bottom ? rect1.bottom : rect2.bottom;
	rect.top	= rect1.top < rect2.top ? rect1.top : rect2.top;
	rect.right	= rect1.right > rect2.right ? rect1.right : rect2.right;

	return rect;
}

// 사교적용 길이 치수 기입이 필요한지?
// 규칙 : 모든 지점중 사교인 경우가 하나라도 있으면 무조건 사교적용 길이 치수 기입 하고
//        모두 직교인 경우는 무조건 사교적용안한 길이 치수 기입을 한다.
BOOL CDrawDBDeck::IsDimByAngleLen()
{
	BOOL bDimByAngleLen	= FALSE;
/*	for(long i = 0; i < m_pDBStd->m_pDB->m_nQtyJigan+1; i++)
	{
		if(!ComparePt(pBri->GetAngleJijum(i), CDPoint(0, 1), "="))
		{
			bDimByAngleLen	= TRUE;
			break;
		}
	}
*/
	return bDimByAngleLen;
}

// 직선교일경우 괄호안 치수를 없애야 됨.
void CDrawDBDeck::ModifyInfoAutoDimConc(CStringArray &sArrText, CStringArray &sArrUnderText, BOOL bInput)
{
	if(IsDimByAngleLen()) return;

	if(!bInput)
	{
		sArrUnderText.RemoveAll();
		return;
	}

	long nSize1 = sArrText.GetSize();
	long nSize2 = sArrUnderText.GetSize();

	CString str1	= _T("");
	CString str2	= _T("");
	for(long i = 0; i < nSize2; i++)
	{
		str1	= i >= nSize1 ? _T("") : sArrText.GetAt(i);
		str2	= sArrUnderText.GetAt(i);

		if(str2.GetLength() > 2)
		{
			str2.Delete(0);
			str2.Delete(str2.GetLength()-1);
			if(str1 == str2)
			{
				str2	= _T("0");
				sArrUnderText.SetAt(i, str2);
			}
		}
	}
}

// tvArr를 철근 R 적용한 선으로 그림.
// bRoundAtStt : 선의 시작부에 라운드가 있다는 뜻.
// dLenRound : 라운드로 표시할 길이
void CDrawDBDeck::DrawTvArrWidthRound(CDomyun *pDomP, CTwinVectorArray &tvArr, double dLenRound, BOOL bRoundAtStt)
{
	CDomyun Dom(pDomP);
	long lineType	= m_pARoadOptionStd->GetEnvType(&Dom);

	CTwinVector tv, tvSolid, tvHidden;
	long nSize	= tvArr.GetSize();

	for(long i = 0; i < nSize; i++)
	{
		tv	= tvArr.GetAt(i);
		if(bRoundAtStt)
		{
			tvHidden.m_v1	= tv.m_v1;
			tvHidden.m_v2	= tv.m_v1 + (tv.GetXyDir()*dLenRound);
			tvSolid.m_v1	= tvHidden.m_v2;
			tvSolid.m_v2	= tv.m_v2;
		}
		else
		{
			tvHidden.m_v1	= tv.m_v2 + (tv.GetXyDir(TRUE) * dLenRound);
			tvHidden.m_v2	= tv.m_v2;
			tvSolid.m_v1	= tv.m_v1;
			tvSolid.m_v2	= tvHidden.m_v1;
		}
		
		m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		Dom.LineTo(tvSolid.m_v1, tvSolid.m_v2);

		m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
		Dom.LineTo(tvHidden.m_v1, tvHidden.m_v2);
	}

	m_pARoadOptionStd->SetEnvType(&Dom, lineType);

	*pDomP << Dom;
}

// 전단 철근처럼 번갈아가면서 점선으로 그려야 하는 경우..
// bHiddenAtStt가 true이면 시작할때부터 점선..
void CDrawDBDeck::DrawTvArrayWidthDashDot(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bHiddenAtStt)
{
	CDomyun Dom(pDomP);

	CTwinVector tv;
	long nSize	= tvArr.GetSize();

	long nLineTypeFir	= bHiddenAtStt ? LT_DASHDOT_LINE : LT_SOLID;
	long nLineTypeSec	= bHiddenAtStt ? LT_SOLID : LT_DASHDOT_LINE;
	BOOL bFirst			= TRUE;

	for(long i = 0; i < nSize; i++)
	{
		tv	= tvArr.GetAt(i);

		m_pARoadOptionStd->SetEnvLType(&Dom, bFirst ? nLineTypeFir : nLineTypeSec);
		bFirst	= !bFirst;
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	*pDomP << Dom;
}

// CRebar *pRB : 재호의 필살기..
// pRB 가 NULL인 경우 일반적으로 그냥 그린다.
// 그러나!!! pRB가 존재하는 경우, 이미 집계가 된 상황이라 판단하며..
// pRB와 RebarInfoCycle은 이미 기호가  match되어 있다...
// 그러므로 pRB와 RebarInfoCycle 의 기호를 비교해서 같은거만 그린다.
// 이거는 철근 집계 창에서 해당 철근만 보기위해서 그려준다.
void CDrawDBDeck::SetMarkForMarkRebar(CString sMark)
{
	CDrawDBDeck::m_sMarkForMarkRebar	= sMark;
}

// 주철근은 m_sMarkForMarkRebar에 따라서 마킹을 해야 되는 경우가 있고, 안 하는 경우가있다.
// 철근 집계시 철근 종류별로 보여주기위해.
BOOL CDrawDBDeck::IsMarkRebar(CString sMark)
{
	if(CDrawDBDeck::m_sMarkForMarkRebar == "") return TRUE;
	if(CDrawDBDeck::m_sMarkForMarkRebar == sMark) return TRUE;

	return FALSE;
}

void CDrawDBDeck::MarkCenterOfRoad(CDomyun *pDomP, CDPoint xyText, double dLength, CString strSec)
{
	CDomyun Dom(pDomP);

	m_pARoadOptionStd->SetEnvType(&Dom,HCAD_TXTG);

	if(dLength==0)
		xyText.y += Dom.Always(3);

	Dom.SetTextAlignHorz(TA_LEFT);
	double dTextHgt  = Dom.GetTextHeight();
	Dom.TextOut(xyText.x-dTextHgt*0.5, xyText.y, _T("C"));
	Dom.TextOut(xyText.x-Dom.Always(0.4), xyText.y-dTextHgt*0.5, _T("L OF ROAD"));
	if(strSec!=_T(""))
		Dom.TextOut(xyText.x, xyText.y-dTextHgt*2.0, strSec);

	if(dLength>0)
	{
		m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CENT);
		double dTextFactor = strSec==_T("") ? 1.0 : 2.5;
		Dom.LineTo(CDPoint(xyText.x, xyText.y-dTextHgt*dTextFactor), CDPoint(xyText.x, xyText.y-dLength));
	}

	*pDomP << Dom;
}

// 폴리곤 내부 또는 외부를 트림.
// 폴리곤은 반시계방향으로 그려져 있어야 함.
// bOutside:외부를 트림(현재 외브 트림만 지원).
void CDrawDBDeck::GetTvArrTrimByPolygon(CTwinVectorArray &tvArrSource, CTwinVectorArray &tvArrPolygon, BOOL bOutside, CTwinVectorArray &tvArrResult)
{
	CTwinVectorArray tvArr;
	tvArr = tvArrSource;

	CTwinVector tv, tvP, tvIn, tvOut;
	CDPoint xyMatch(0, 0);

	BOOL bLeft1 = FALSE, bLeft2 = FALSE;
	long nSize = tvArr.GetSize();
	long nSizeP = tvArrPolygon.GetSize();

	for(long i=0; i<nSizeP; i++)
	{
		tvP = tvArrPolygon.GetAt(i);
		if(tvP.GetLength()==0) continue;
		for(long j=0; j<nSize; j++)
		{
			tv = tvArr.GetAt(j);
			if(tv.GetLength()==0)
			{
				tvArr.RemoveAt(j);
				nSize = tvArr.GetSize();
				j--;
				continue;
			}
			bLeft1 = GetDirPointByLine(tv.m_v1, tvP.m_v1, tvP.m_v2);
			bLeft2 = GetDirPointByLine(tv.m_v2, tvP.m_v1, tvP.m_v2);
			if(!bLeft1 && !bLeft2)
			{
				tvArr.RemoveAt(j);
				nSize = tvArr.GetSize();
				j--;
				continue;
			}
			tvIn = tv;
			if(GetXyMatchSegAndSeg(tvP.m_v1, tvP.m_v2, tv.m_v1, tv.m_v2, xyMatch))
			{
				if(bLeft1 && !bLeft2)
				{
					tvIn.m_v1 = tv.m_v1;
					tvIn.m_v2 = xyMatch;
				}
				else if(!bLeft1 && bLeft2)
				{
					tvIn.m_v1 = xyMatch;
					tvIn.m_v2 = tv.m_v2;
				}
			}
			if(bOutside)
				tvArr.SetAt(j, tvIn);
		}
	}

	tvArrResult = tvArr;
}

// 도면 치수기입에 필요한 셋팅을 복사한다.
void CDrawDBDeck::ApplyDimSetting(CDomyun *pDomPSource, CDomyun *pDomPDest)
{
	if(m_bApplyDimSettingOfBaseDom)
	{
		pDomPDest->SetScaleDim(pDomPSource->GetScaleDim());
		pDomPDest->SetDimStdTxtHeight(pDomPSource->GetDimStdTxtHeight());
		pDomPDest->SetDimTxt(pDomPSource->GetDimTxt());
		pDomPDest->SetTextHeight(pDomPSource->GetTextHeight(FALSE));
		pDomPDest->SetDimExe(pDomPSource->GetDimExe());
		pDomPDest->SetDimExo(pDomPSource->GetDimExo());
		pDomPDest->SetDimLevelDis(pDomPSource->GetDimLevelDis());
		pDomPDest->SetDimTxt(pDomPSource->GetDimTxt());
		pDomPDest->SetDimVLen(pDomPSource->GetDimVLen());
	}
}

// 수평 시공 이음을 그린다(xy:좌측 기준, dWidth:폭)
void CDrawDBDeck::DrawConstJointHor(CDomyun* pDomP, CDPoint xy, double dWidth)
{
	CDomyun Dom(pDomP);
	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);

	double dJointW = Dom.Always(1);
	double dJointH = Dom.Always(0.5);

	Dom.MoveTo(xy);
	Dom.LineToEx((dWidth-dJointW)/2.0, 0);
	Dom.LineToEx(0, -dJointH);
	Dom.LineToEx(dJointW, 0);
	Dom.LineToEx(0, dJointH);
	Dom.LineToEx((dWidth-dJointW)/2.0, 0);

	*pDomP << Dom;
}

// 수직 시공 이음을 그린다(xy:상단 기준, dHeight:높이)
void CDrawDBDeck::DrawConstJointVer(CDomyun* pDomP, CDPoint xy, double dHeight, BOOL bLeft)
{
	CDomyun Dom(pDomP);
	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);

	double dJointW = Dom.Always(2);
	double lr = bLeft ? -1 : 1;

	Dom.MoveTo(xy);
	Dom.LineToEx(0, -dHeight/2.0);
	Dom.LineToEx(dJointW*lr, 0);
	Dom.LineToEx(0, -dHeight/2.0);

	*pDomP << Dom;
}


// 주철근의 R 마크 해줌
// 현재는 기초부에 들어가는 R 표시만 해주므로 좌우표시만 지원한다.
// 다른방향은 필요하다면 그때 지원하도록 함. 2005.2.4
void CDrawDBDeck::MarkRadiusRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, CDRect rectLimit)
{
	if(!pRC) return;

	if(pRC->m_R_Mid > 0 && pRC->GetLengthMid() > 0)
	{
		BOOL bLeftDir	= pRC->m_xyMid2.x > pRC->m_xyMid1.x;
		CDPoint xyMark	= pRC->m_xyCen;
		CDPoint xyDir	= bLeftDir ? ToDPointFrDegree(-45) : ToDPointFrDegree(225);
		xyMark			= xyMark + xyDir * pRC->m_R_Mid;

		// 해당 영역만 해야 될 경우...
		if(rectLimit.Width() != 0 && rectLimit.Height() != 0)
		{
			BOOL bOK	= FALSE;
			if(xyMark.x >= rectLimit.left && xyMark.x <= rectLimit.right &&
				xyMark.y >= rectLimit.bottom && xyMark.y <= rectLimit.top)
				bOK	= TRUE;

			if(!bOK) return;
		}


		CDomyun Dom(pDomP);
		m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CString sR		= _T("");
		sR.Format("R=%.0f", pRC->m_R_Mid);
		double dWidthText	= Dom.GetTextWidth(sR);
		Dom.GiSiArrowExtend(xyMark.x, xyMark.y, Dom.GetTextHeight()*3, bLeftDir ? -dWidthText : dWidthText, bLeftDir ? 3 : 1, sR, "");

		*pDomP << Dom;
	}
}




//xyMark : 철근의 중앙위치 
//nPos   : 철근의 배근위치(상, 하, 좌, 우)
void CDrawDBDeck::DrawPointRebar(CDomyun *pDom, CDPoint xyMark, long nPos, double dDia)
{
	dDia = REBAR_SYMBOL_RADIUS/2;
	CDomyun Dom(pDom);
	switch(nPos)
	{
	case REBARPOS_UP:
		xyMark.y += dDia;
		break;
	case REBARPOS_DOWN:
		xyMark.y -= dDia;
		break;
	case REBARPOS_LEFT:
		xyMark.x -= dDia;
		break;
	case REBARPOS_RIGHT:
		xyMark.x += dDia;
		break;
	case REBARPOS_DOWN_LEFT:
		xyMark.x -= dDia;
		xyMark.y -= dDia;
		break;
	case REBARPOS_DOWN_RIGHT:
		xyMark.x += dDia;
		xyMark.y -= dDia;
		break;
	case REBARPOS_UP_LEFT:
		xyMark.x -= dDia;
		xyMark.y += dDia;
		break;
	case REBARPOS_UP_RIGHT:
		xyMark.x += dDia;
		xyMark.y += dDia;
		break;
	case REBARPOS_NONE:
		break;
	}

	CHgBaseDrawStd baseDraw;

#ifdef _DEBUG
	Dom.SetLineColor(2);
	Dom.Circle(xyMark, dDia, TRUE);
#endif
#ifndef _DEBUG
	CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xyMark, /*Dom.GetScaleDim()*/20*(dDia/9));
#endif
	*pDom << Dom;
}


long CDrawDBDeck::DrawPointRebarByLine(CDomyun *pDom, CDPoint xyStt, CDPoint xyEnd, double dCTC, long nPos, double dEraseLengthFrStt, CDPoint& xyFirstRebarPos, CDPoint& xyLastRebarPos)
{
	CHgBaseDrawStd baseDraw;

	double dLength = ~(xyStt-xyEnd);
	CDPoint   vDir = (xyEnd-xyStt).Unit();
	long nRebar = long((dLength)/dCTC)+1;
	CDPoint xyOrigin = xyStt;
	BOOL bFirst = FALSE;
	long nRebarReal = 0;
	for(long n=0; n<nRebar; n++)
	{
		if(~(xyStt-xyOrigin)>=dEraseLengthFrStt)
		{
			DrawPointRebar(pDom, xyStt, nPos);
			nRebarReal++;
			
			if(!bFirst)		
			{
				xyFirstRebarPos = xyStt;
				bFirst = TRUE;
			}
			xyLastRebarPos  = xyStt;
		}
		xyStt += vDir*dCTC;
	}

	return nRebarReal;
}


// UINT nTypeDrawOverLapLine : overlap 라인 그리는 방법 
// 0 : 그리지 않음. 1 : 정상대로 그림,
void CDrawDBDeck::DrawTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bDrawInfo, BOOL bDrawDirInfo, UINT nTypeDrawOverLapLine)
{
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(pDomP, tvArr, bDrawInfo, bDrawDirInfo, nTypeDrawOverLapLine);
	return;

	if(tvArr.GetSize() < 1) return;
	CRebarPlacing rbPlacing;
	CTwinVector tv;
		
	CDPoint xyCen(0, 0);	// 원의 중심
	CDPoint xy1(0, 0);		// 선이나 원의 첫번째 좌표
	CDPoint xy2(0, 0);		// 선이나 원의 두번째 좌표
	CDPoint xy3(0, 0);		// 선이나 원의 세번째 좌표
	double dR		= 0;	// 원의 반지름
	double dAngStt	= 0;	// 아크의 시작각
	double dAngEnd	= 0;	// 아크의 끝깍
	
	
	// 그루코드는 기본값이 0 임.
	if(!m_bSettingGroupCode) pDomP->SetCurGroupCode(0);

	for(long i = 0; i < tvArr.GetSize(); i++)
	{
		tv = tvArr[i];

		// 그룹코드를 지정해줘야 하는 선인지 판단해서 그룹 지정.(그럴 필요 없으면 0으로 지정)
		if(m_bSettingGroupCode)
		{
			if(tv.val.valHor || tv.val.valVer)
				pDomP->SetCurGroupCode(m_nCurGroupCode++);
			else
				pDomP->SetCurGroupCode(0);
		}

		// 겹치는 선이 있을 경우 안겹치는 부분만 그려줌.(현재 직선에 대해서만 지원됨)
		if(~(tv.m_vecOverlap1-tv.m_vecOverlap2) > 0 && nTypeDrawOverLapLine == 0) 
		{
			CDPointArray xyArrBaseLine;
			CDPointArray xyArrLine;
			xyArrBaseLine.Add(tv.m_v1); xyArrBaseLine.Add(tv.m_v2);
			xyArrLine.Add(tv.m_vecOverlap1); xyArrLine.Add(tv.m_vecOverlap2);
			
			rbPlacing.CheckOverlapLine(xyArrBaseLine, xyArrLine, FALSE, TRUE);
			DrawLineArray(pDomP, xyArrBaseLine);
			continue;
		}
		
			
		if(tv.m_v1.z == 0)
		{
			pDomP->LineTo(tv.m_v1.x, tv.m_v1.y, tv.m_v2.x, tv.m_v2.y);
			CString str = _T("");
			if(bDrawInfo)
				str = tv.m_sInfo;
				
			if(bDrawDirInfo)
				str += "("+tv.GetHor()+","+tv.GetVer()+")";

			if(str != "")
			{
				// 도면정보 저장
				double dOldAngle = pDomP->GetTextAngle();
				double dOldScale = pDomP->GetScaleDim();

				// 텍스트 기입
				pDomP->SetScaleDim(1);
				pDomP->SetTextHeight(50);
				pDomP->SetTextAngle(tv.GetXyDir().GetAngleDegree());
				pDomP->SetTextAlignHorz(TA_CENTER);
				pDomP->TextOut((tv.m_v1+tv.m_v2)/2, str);

				// 도면정보 복구
				pDomP->SetTextAngle(dOldAngle);
				pDomP->SetScaleDim(dOldScale);
			}
		}
		else if(tv.m_v1.z == 1 || tv.m_v1.z == -1)
		{
			dR = tv.m_v2.z;
			BOOL bLeft = tv.m_v1.z > 0 ? FALSE : TRUE;
			xy1 = tv.m_v1;
			xy2 = tv.m_v2;

			// 원의 중심 찾기
			xyCen = rbPlacing.GetXyCircleCenter(xy1, xy2, dR, bLeft);

			// 원의 중심에서 각 점까지의 각도
			dAngStt = (xy1-xyCen).Unit().GetAngleDegree();
			dAngEnd = (xy2-xyCen).Unit().GetAngleDegree();
			if(dAngEnd < dAngStt) dAngEnd+=360;

			// 아크 그리기
			//double dDist = ~(xy1, xy2);
			//if(~(xy1, xy2) <= dR*2)
			{
				pDomP->Arc(xyCen, dR, dAngStt, dAngEnd-dAngStt);
			}
		}
		else if(tv.m_v1.z == 2)	// 선형의 종단을 그림.. 선형이 있을 경우만 해당
		{
			CLineInfoApp *pLine = m_pDataMng->GetLineInfoApp();
			if(pLine)
			{
				pLine->DrawJongDanLine(pDomP, tv.m_v1.x, tv.m_v2.x, 1, TRUE);
			}
			else 
			{
				pDomP->LineTo(tv.m_v1.x, tv.m_v1.y, tv.m_v2.x, tv.m_v2.y);
			}
		}
	}
}