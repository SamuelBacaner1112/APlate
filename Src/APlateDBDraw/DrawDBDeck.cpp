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

// �ڵ����� ġ���� �������ִ� �Լ� (cont ���)
// arrText�� ���� ġ�� ��� �� ���ڸ� ������ �� �� �ִ�.
// arrText�� ���ڴ� ġ���� �ε����� ���ؼ� ����, ġ�� ������ ġ���� ������� ���ڵ� �������.
// bLeftMargin : ġ���� �ϳ��϶� ������ ��� ���������� ġ���� �������� ���� ���������� ����..
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
	BOOL bIsUnder		= FALSE;	// ���� ġ�����Խ� ġ���� �Ʒ��� ���ڸ� �����ߴ°�?
	long i = 0;

	// ġ�����ڰ� ġ������ ������ ������ 0���� ä���ش�.
	// ��¥�� ġ�����ڸ� �̾ƿö� 0�̸� ���°����� �ν��Ѵ�.
	if(arrText.GetSize() < arr.GetSize())
	{
		long nCount = arr.GetSize() - arrText.GetSize();
		for(i = 0; i < nCount; i++)
			arrText.Add(sEmptyText);
	}

	// �Ʒ�ġ�����ڰ� ġ������ ������ ������ 0���� ä���ش�.
	// ��¥�� ġ�����ڸ� �̾ƿö� 0�̸� ���°����� �ν��Ѵ�.
	if(arrUnderText.GetSize() < arr.GetSize())
	{
		long nCount = arr.GetSize() - arrUnderText.GetSize();
		for(i = 0; i < nCount; i++)
			arrUnderText.Add(sEmptyText);
	}


	// ġ���� 0�ΰ��� ��� �����Ѵ�. (���� �ʿ� ����)
	for(i = 0; i < arr.GetSize(); i++)
	{
		if(arr.GetAt(i) == 0) 
		{
			arr.RemoveAt(i, 1);
			arrText.RemoveAt(i, 1);
			arrUnderText.RemoveAt(i, 1);
		}
	}

	
	// ���̳ʽ��ΰ��� �μ��� ���� �����Ѵ�.
	// ������ ���̳ʽ��ΰ͸� ������ ������ �ٲٱ� ������ �ʿ��Ѱ��� �ƴ��� �Ǵ��ϱ� ���� 
	if(bEraseMinus && arr.GetSize() > 0)
	{
		// ������ +�̸� -�ΰ��� �����ϰ� 
		// ������ -�̸� +�ΰ��� �����Ѵ�.
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

	// �ѱ��̸� �̸� ���� ���´�.
	for(i = 0; i < nSize; i++)
		dTotalLen += arr.GetAt(i);

	// ���� ġ���߿� ������ -�����̸�, DimLineToOver��뿡 ���Ǹ� ���Ѵ�.
	if(arr.GetAt(0) < 0)
		bAssert = TRUE;

	// ���Ǹ� ���ϴ� ����̸� ġ���� �ݴ�� �����ϱ� ���� ���� ��ġ�� �ű��.
	// ���� �������� ��� �����´�.
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


	// ������ ġ�� ���� �ϴ� �κ� ******************************************************/
	BOOL bInitMoveTo = FALSE;
	for(i = 0; i < nSize; i++)
	{
		dLen		= arr.GetAt(i);
		sText		= arrText.GetAt(i);
		sUnderText	= arrUnderText.GetAt(i);

		if(dLen == 0) continue;

		// ġ�� ������ ���� �ؽ�Ʈ ������ /////////////////////////////
		// �ݺ��Ǵ� ġ���� �ִ����˻��Ѵ�.
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

		// ġ�� ���� ///////////////////////////////////////////////////
		// ġ���� �ּ� �������� ������ �Ʒ��� ���� ����.
		// �ּҸ����� -1�̸� �ڵ����� �Ǵ��Ѵ�.
		double dMinWidth = dMinMargin == -1 ? pDomP->GetTextWidth(str) : dMinMargin;
		if(fabs(dLen*nCount) <= dMinWidth)
		{
			if(i == 0)
			{
				if(!bInitMoveTo)
				{
					// ġ���� �ϳ��ۿ� ������ ���������� �������̶��..
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


	// ��ü ġ�� �����ϴ� ����̸�..
	if(bIsTotalDim)
	{
		str = COMMA(fabs(dTotalLen));
			
		if(str != "")
		{
			pDomP->DimMoveTo(xyStt.x, xyStt.y, nDimDan+1);
			pDomP->DimLineTo(dTotalLen, nDimDan+1, str);
		}
	}

	// ****************************************************** ������ ġ�� ���� �ϴ� �κ� /
}

// ��ǥ���� �޾ƿͼ�.. �ڵ����� cont ġ���� �����Ѵ�.
// arrText�� ���� ġ�� ��� �� ���ڸ� ������ �� �� �ִ�.
// arrText�� ���ڴ� ġ���� �ε����� ���ؼ� ����, ġ�� ������ ġ���� ������� ���ڵ� �������.
// bAli : �밢�� ���� ��������? (�⺻�� ��������� ��)
// bInput : �Է�â ������ ġ������?(�̶��� sArrText���� ��ȣ�� ������.. sArrUnderText���� �ڵ����� ���̰��� ����.)
// bAutoModifyStt : ������ġ�� ���ؼ� �ڵ����� ��������(����ġ���϶� ������ġ�� �����ϰ�, ����ġ���϶� ������ġ�� �����Ѵ�)
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

	// ���������� ����.
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

	// ���� ������ ġ�� ���� 
	if(arrText.GetSize() > 0) arrText.RemoveAt(0);
	if(arrUnderText.GetSize() > 0) arrUnderText.RemoveAt(0);
	
	AutoDimCont(pDomP, xyStt, arrLen, arrText, arrUnderText, nDimDan, sDirection, dMinMargin, bIsTotalDim, bRepetition, bEraseMinus, bLeftMargin);

	// ���� ���̷� ������ ��� ���⿡ ���� ȸ������ ��� �ȴ�.
	if(bAli && arr.GetSize() > 1)
	{
		double dAng = (arr.GetAt(arr.GetSize()-1)-arr.GetAt(0)).GetAngleDegree();
		dAng = bHor ? dAng : dAng - 90;
		CDPoint xyDir = ToDPointFrRadian(ToRadian(dAng));
		pDomP->Rotate(arr.GetAt(0), xyDir);
	}
}

// ���� �迭�� ���� �׸���.
void CDrawDBDeck::DrawLineArray(CDomyun *pDomP, CDPointArray &xyArr)
{
	if(xyArr.GetSize() < 2) return;
	if(xyArr.GetSize() % 2 != 0) xyArr.RemoveAt(xyArr.GetSize()-1);

	for(long i = 0; i < xyArr.GetSize()-1; i+=2)
		pDomP->LineTo(xyArr.GetAt(i), xyArr.GetAt(i+1));
}

// �׷� �ڵ� �ʱ�ȭ
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
	// ǥ�� ���̾� ����
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

// rectCut�� �������� ���ܼ� ǥ�� �����ؼ� tvArr�� �׷��ش�.
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

	// Ŀ�ö����� ������ ���̴� ���� üũ��.
	BOOL bLeft = FALSE;
	BOOL bRight = FALSE;
	BOOL bTop = FALSE;
	BOOL bBottom = FALSE;
	CDRect rectTvArr = tvArr.GetRect();	// �׸� �׸��� �簢��ǥ
	
	// �˻�
	if(rectCut.left >= rectTvArr.left) bLeft = TRUE;
	if(rectCut.right <= rectTvArr.right) bRight = TRUE;
	if(rectCut.top <= rectTvArr.top) bTop = TRUE;
	if(rectCut.bottom >= rectTvArr.bottom) bBottom = TRUE;

	// ������ �ջ��Ű�� ����ϹǷ�...
	CTwinVectorArray tvArrTmp;
	tvArrTmp = tvArr;
	rbPlacing.GetTvArrayByCrossVectorArrayArea(tvArrTmp, vecArrArea, TRUE);

	CDomyun Dom(pDomP);
	DrawTvArray(&Dom, tvArrTmp);

	// �߸� �κп� ���ؼ� Ŀ�ö��� ǥ�� �Ѵ�.
	// ����� �� �˻�.
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
				
				// �� ��ǥ�� ���� ���..
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

// rectCut������ŭ ���η� �߶� ����ǥ�ñ��� ����(��հ����� �׸��� �����Ұ���)
void CDrawDBDeck::DrawCuttingTvArrayHorRange(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine)
{
	CTwinVectorArray tvArrUpper, tvArrLower;	// ��, �Ʒ��� ���� ����
	CRebarPlacing rbPlacing;

	CDomyun Dom(pDomP);
	if(rbPlacing.CuttingHorRange(tvArr, rectCut, tvArrUpper, tvArrLower, TRUE, 100))
	{
		DrawTvArray(pDomP, tvArrUpper);
		DrawTvArray(pDomP, tvArrLower);

		if(bDrawCuttingLine)
		{
			// ���� ������ �������� ���ö����� �׸���.
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

// rebar cycle info �׸���.
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
		// ���� ��������?(ö���� 2���̻��ΰ��..)
		BOOL bApplyRound	= FALSE;//!pBri->m_bBendingDanBuRebar;

		// ���ʿ��� ������ ö���� ������ ���� ���� ����.
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
				// ��ũ �׸��� ���ǻ��� 
				// 1. ö������ ������ �������� +, ���� +�ΰ�� ��ũŸ�� 1�� �� ���� ������ ������� ��..
				// 2. �������� ��� ��ũŸ�� -1�� �׸���.
				// �׷��� ���������� �׷���.. ����
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
		
		// ���� ���ʸ� ����.
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


// rebarinfocycle array���� ġ�����Կ� ������ �м��ؼ� �������ش�.
// sArrInfo ���� : 
// 0 : �����
// 1 : ö���� ���� ����(�Է¹޴� ö���� ����)
// 2 : �Է¹��� �ʴ� ö���� ���� ����(���峪 ����� ������)
// BOOL bApplyJoint : ������ ����ؼ� ������ �м�����?
// BOOL bApplyEndIs2 : ������ ö���� ������ 2�� ��������?
// BOOL bReverse : ������� ö���� ���� ������ �ݴ�� ã��.. (2�� �״��)
void CDrawDBDeck::GetDimInfoAtRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bHor, CDPointArray &xyArrDim, CStringArray &sArrInfo, BOOL bMerge, BOOL bApplyJoint, BOOL bApplyEndIs2, BOOL bReverse, double dCoverStt, double dCoverEnd)
{
	if(!bMerge)
	{
		xyArrDim.RemoveAll();
		sArrInfo.RemoveAll();
	}
	

	// ���������� ������� ���� �Լ��̴�. /////////////////////////////////////
	// ���� rc�� ���������� �ִ� �ΰ��� ���ļ� �ϳ��� ���� �ڿ� 
	// ġ������ ������ �����Ѵ�.
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
	
	// ������ġ�� ö��������� �Ǵ�.
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

			// �Ǻ� ���� �߰��Ѵ�.
			// ó���� ���κп� �Ǻ� ���� �߰��Ѵ�. ////////////////
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
			return;	// 1���� ���� ��� �̷��Ը� �ϰ� �������Ѵ�. ������ �ڽð� ����.
		}
	}
	
	for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC = pArrRCTmp.GetAt(i);

		// ����, �߰�, ���� ���� �Ǿ�� �� ���� ����..
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


		// ������ġ�� ����ġ�� �ٸ��� �����(����)�� �ִ�.
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
		// ������ ����
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


	// reverse�̸� 1�� 0�� �ڹٲ�.
	if(bReverse)
	{
		nSize	= sArrInfo.GetSize();
		for(i = 0; i < nSize; i++)
		{
			if(sArrInfo.GetAt(i) == "0") sArrInfo.SetAt(i, "1");
			else if(sArrInfo.GetAt(i) == "1") sArrInfo.SetAt(i, "0");
		}
	}


	// �������� �ִ� "1"�� ������ "2"�� �Ǿ�� �Ѵ�.(������ ���̰�..)
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

	// �Ǻ� ���� �߰��Ѵ�.
	// ó���� ���κп� �Ǻ� ���� �߰��Ѵ�. ////////////////
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

// ���� �׸���.
// long nDrawLeft : -1 �������� ������ �׸�, 0 ������ϰ� �� �׸�, 1 �������� ������ �׸�
// double dStaCuttingOrg : ���� sta
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
		double dStaCuttingOrg	= GetStaCuttingOrg();	// ������ ã��.

		CRebarPlacing rb;
		CDPoint xyDir	= (xyEnd-xyStt).Unit();
		// ���ʸ� �׸��� ���
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
		// �����ʸ� �׸��� ���.
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
		
		// ���� ���ʸ� ����.
		CRebarPlacing rb;
		CVectorArray vecArrArea;
		tvArrLimit.GetToVectorArray(vecArrArea);
		rb.GetTvArrayByCrossVectorArrayArea(tvArr, vecArrArea, TRUE);
	}

	DrawTvArray(pDomP, tvArr);
}

// ���� �׸���
void CDrawDBDeck::DrawJointByRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC)
{
/*	if(!pArrRC) return;
	CJointArray arrJoint;

	// rebarinfocycle���� ���� ���� �м�
	m_pRcBridgeRebar->GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);

	// ���� �׸���
	DrawJointArray(pDomP, arrJoint);*/
	
}

// rebarinfocycle �迭 �׷����..
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
	// ã���� ���� ������ �ϳ��� �׸�.
	long nSize	= arrJoint.GetSize();
	for(long i = 0; i < nSize; i++)
	{
		DrawJoint(pDomP, arrJoint.GetAt(i), nDrawLeft, rectLimit);
	}
}

// rebarinfocycle ö�� ��ŷ�ϱ�.
// ��ŷ ������ ã�Ƽ� dan�� ���� ��ŷ�Ѵ�.
// 1�� : ����������� ����������
// 2�� : ������������ ��������
// 3�� : �������������� ����������
// bUpperAndLeft : ���γ� �������� ��ŷ����?
// bSide : ���� ��ŷ����?
// long nTypeStt : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// long nTypeEnd : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// ��ŷŸ�� 0 - ù��° ��, 1 - ��� ��, 2 - ������ ��
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

	// tvarr�� ����� ��� ��ŷ�� ��ġ�� xyStt�� xyEnd�� �־��ش�.
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
		// ��ŷ�� �� ã��
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

	// ��ŷ�� ���� ã��
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
		// ���ʸ� �׸�
		if(nDrawLeft == -1)
		{
			if(xyMark.x > dStaCuttingOrg) return;
		}
		// �����ʸ� �׸�
		else if(nDrawLeft == 1)
		{
			if(xyMark.x < dStaCuttingOrg) return;
		}
	}

	// ���������� ���ؾ� �Ǵ� ��ġ�� ���ؼ� ���⼭ ������ �� �� �ִ�. ////////////////////////
	CRcBridgeRebar *pBri	= m_pRcBridgeRebar;
	// 1. ������ü ����ġ���� ��ġ�� ����ϸ� ��������.
	// ��, ��ν����� ö�ٸ� ö�ٸ� �����ϵ� ��ġ�� ö���� �������
	if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER || 
		pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER)
	{
		// tv�� �̿��Ѱ��� ��ġ�� ö���̴�. �������� ����
		if(!pRC->m_bUseTvArrRebar)
		{
			double dRange	= Dom.GetTextWidth("000000");
			double dOffset	= Dom.Always(Dom.GetDimLevelDis()*2 + Dom.GetDimVLen() + Dom.GetDimExo());
			for(long stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				if(!pBri->IsOutWall(bStt)) continue;

				CTwinVector tvWall	= pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "������ü������" : "������ü������");
				tvWall.m_v1.x	+= bStt ? dOffset : -dOffset;
				if(tvWall.m_v1.x + dRange > xyMark.x && tvWall.m_v1.x - dRange < xyMark.x)
				{
					// ������ �ɸ��� �������� �ɸ��Ÿ� ���������� �ƴϸ� �ݴ�� �̵�
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



	// ���� ����
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

// rebarinfocycle ö�� ��ŷ�ϱ�.
// ��ŷ ������ ã�Ƽ� dan�� ���� ��ŷ�Ѵ�.
// 1�� : ����������� ����������
// 2�� : ������������ ��������
// 3�� : �������������� ����������
// bUpperAndLeft : ���γ� �������� ��ŷ����?
// bSide : ���� ��ŷ����?
// long nTypeStt : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// long nTypeEnd : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// ��ŷŸ�� 0 - ù��° ��, 1 - ��� ��, 2 - ������ ��
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

		// ������ ���鿡�� ������� ��뼱���� ö�ٸ�ũ �ؾ� ��
		if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_SIDE)
			nType = 2;
		else if(pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_SIDE)
			nType = 0;
		

		MarkRebarInfoCycle(pDomP, pArrRC->GetAt(i), bUpperAndLeft, bSide, dHeightMark, nType, nDrawLeft, rectLimit, dScalePos);
	}
}

// ���� ���� �۷��� �Ǵ� ������...
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


// PlaceRebarByBlock���� ġ�����Կ� ������ ���´�. ����
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

// PlaceRebarByBlock���� ġ�����Կ� ������ ���´�. ����
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


// �ɼǿ� ���� EL���� �������ش�.
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

// RebarInfoCycle �迭�� ġ�� �ֱ�(��ö�� ������ ��)
// bLeft : ��������� ������ ġ�� ������ ������ ġ�� ������... 
// long nDrawLeft : -1 ������ �������� ġ�� ����, 0 ������ ��� ����, 1 : ������ �������� ġ�� ����.
// �������δ� �Ʒ����� �����ص� ������, �������δ� �ݵ�� �¿��� ��θ� �����ؾ� ����� ġ���� ���Եȴ�.
// BOOL bAli : �ǰŸ��� ġ������ ����...?
void CDrawDBDeck::DimRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, CDPoint xyStt, CDPoint xyEnd, BOOL bLeft, long nDimDan, long nDrawLeft, BOOL bAli)
{
/*	if(!pArrRC) return;
	CJointArray arrJoint;
	// rebarinfocycle���� ���� ���� �м�
	m_pRcBridgeRebar->GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);

	
	CDomyun Dom(pDomP);
	

	// �������� ������ �޾Ƽ� ġ���� ���ԵǴ� ����� ���� �� �մ�
	// -1 : �������� ����, 0 : ���� ����, 1 : �߷��� ���Ե�.(arc������ �߷��� ���ԵǴ°� ��� �ȵ�)
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
		// �ǰŸ� ġ�� ���Խ� ȸ���� ���� �Ǵ�.
		// ù��° rc�� �Ǵ���.
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

		// ���� ��������?(ö���� 2���̻��ΰ��..) /////////////////
		BOOL bApplyRound	= FALSE;//!pBri->m_bBendingDanBuRebar;
		
		// ���ʿ��� ������ ö���� ������ ���� ���� ����.
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
 			// �������� ������ ��ġ�� ������ ���� ��ġ���� �ľ��ؾ� �ȴ�. ///////////
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
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
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

				// dStaCuttingOrg ����ؼ� ��ǥ ������.
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
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
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

				// ���� ö�� ���۱��� ġ�� ����
				if(rc < nSize-1)
				{
					CRebarInfoCycle *pRCNext	= pArrRC->GetAt(rc+1);

					// dStaCuttingOrg ����ؼ� ��ǥ ������.
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

			// dStaCuttingOrg ����ؼ� ��ǥ ������.
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
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid1, pRC->m_xyMid2, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(pRC->m_xyMid1, pRC->m_xyMid2, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				CRebarPlacing rb;
				double dR	= fabs(pRC->m_xyMid2.x - pRC->m_xyMid1.x);
				dLen		= rb.GetArcLength(pRC->m_xyCen, pRC->m_xyMid2, pRC->m_xyMid1);

				// arc ġ�� �������� �ݵ�� TOP���� �����ؾ� ���ϴ� ��� ���´�.
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

			// �������� ������ ��ġ�� ������ ���� ��ġ���� �ľ��ؾ� �ȴ�. ///////////
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
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
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

				// dStaCuttingOrg ����ؼ� ��ǥ ������.
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
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
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

				// ���� ö�� ���۱��� ġ�� ����
				if(rc < nSize-1)
				{
					CRebarInfoCycle *pRCNext	= pArrRC->GetAt(rc+1);

					// dStaCuttingOrg ����ؼ� ��ǥ ������.
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

	// ��� ġ�� ������ ������ ġ�� ����
	if(nSize > 0)
	{
		// dStaCuttingOrg ����ؼ� ��ǥ ������.
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
		// dStaCuttingOrg ����ؼ� ��ǥ ������.
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

// �¿��� �߶� ǥ���� ��� ������ �Ǵ� sta ����
double CDrawDBDeck::GetStaCuttingOrg()
{
	double dSta	= 0;
	// �������� Ȧ���̸� �߰� ������ ������ ���� ��ġ,
	// �������� ¦���̸� �߰� ���� ��ġ.
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

// rebarinfocycle ġ�� ���Խ� ������ ����ؼ� ġ�� ���� Ÿ���� �����Ѵ�.
long CDrawDBDeck::GetTypeDimRebarInfoCycle(BOOL bHor, CDPoint xyStt, CDPoint xyEnd, long nDrawLeft, double dStaCuttingOrg)
{
	long nTypeDim	= 0;

	// dStaCuttingOrg ��� /////////////////////////////////////////////////
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

// typeDim�� ���� ������ ���� �Ǵ� ġ���� ��ġ�� �����Ѵ�.
void CDrawDBDeck::GetDimInfoByTypeDim(CDPoint &xyStt, CDPoint &xyEnd, BOOL bHor, long nTypeDim, long nDrawLeft, double dStaCuttingOrg)
{
	// dStaCuttingOrg ��� �����϶��� ����
	if(nTypeDim == 0) return;
	if(nDrawLeft == 0) return;

	// �ƿ� ġ�� ���� ���� �������� ���� ��ġ ����ؼ� �������� �����ؼ� ����
	if(nTypeDim == -1)
	{
		xyStt	= xyEnd;
		return;
	}


	// �߷��� ġ�� ���� �ɶ�.
	if(nTypeDim	== 1)
	{
		if(bHor)
		{
			// ���ʸ� �����ϴµ� �߷��� �Ǹ� ������ ġ���� ����
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
			// �����϶��� �߷��� ���� �Ǵ� ���� �����Ƿ� �׳� ����.
			return;
		}
	}
}

// ���ݹ迭�� �ݺ�ġ�� ���� ������ ��ȯ
void CDrawDBDeck::GetDimInfoAtDistArray(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CDoubleArray &dArrDist, CString sMark, BOOL bInput, BOOL bMerge, BOOL bHor)
{
	if(!bMerge)
	{
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}


	// ������ offset ������ ��ȯ
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

// rect �ΰ��� �ϳ��� rect�� ����
CDRect CDrawDBDeck::MergeRectAndRect(CDRect rect1, CDRect rect2)
{
	CDRect rect;
	rect.left	= rect1.left < rect2.left ? rect1.left : rect2.left;
	rect.bottom	= rect1.bottom > rect2.bottom ? rect1.bottom : rect2.bottom;
	rect.top	= rect1.top < rect2.top ? rect1.top : rect2.top;
	rect.right	= rect1.right > rect2.right ? rect1.right : rect2.right;

	return rect;
}

// �米���� ���� ġ�� ������ �ʿ�����?
// ��Ģ : ��� ������ �米�� ��찡 �ϳ��� ������ ������ �米���� ���� ġ�� ���� �ϰ�
//        ��� ������ ���� ������ �米������� ���� ġ�� ������ �Ѵ�.
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

// �������ϰ�� ��ȣ�� ġ���� ���־� ��.
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

// tvArr�� ö�� R ������ ������ �׸�.
// bRoundAtStt : ���� ���ۺο� ���尡 �ִٴ� ��.
// dLenRound : ����� ǥ���� ����
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

// ���� ö��ó�� �����ư��鼭 �������� �׷��� �ϴ� ���..
// bHiddenAtStt�� true�̸� �����Ҷ����� ����..
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

// CRebar *pRB : ��ȣ�� �ʻ��..
// pRB �� NULL�� ��� �Ϲ������� �׳� �׸���.
// �׷���!!! pRB�� �����ϴ� ���, �̹� ���谡 �� ��Ȳ�̶� �Ǵ��ϸ�..
// pRB�� RebarInfoCycle�� �̹� ��ȣ��  match�Ǿ� �ִ�...
// �׷��Ƿ� pRB�� RebarInfoCycle �� ��ȣ�� ���ؼ� �����Ÿ� �׸���.
// �̰Ŵ� ö�� ���� â���� �ش� ö�ٸ� �������ؼ� �׷��ش�.
void CDrawDBDeck::SetMarkForMarkRebar(CString sMark)
{
	CDrawDBDeck::m_sMarkForMarkRebar	= sMark;
}

// ��ö���� m_sMarkForMarkRebar�� ���� ��ŷ�� �ؾ� �Ǵ� ��찡 �ְ�, �� �ϴ� ��찡�ִ�.
// ö�� ����� ö�� �������� �����ֱ�����.
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

// ������ ���� �Ǵ� �ܺθ� Ʈ��.
// �������� �ݽð�������� �׷��� �־�� ��.
// bOutside:�ܺθ� Ʈ��(���� �ܺ� Ʈ���� ����).
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

// ���� ġ�����Կ� �ʿ��� ������ �����Ѵ�.
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

// ���� �ð� ������ �׸���(xy:���� ����, dWidth:��)
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

// ���� �ð� ������ �׸���(xy:��� ����, dHeight:����)
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


// ��ö���� R ��ũ ����
// ����� ���ʺο� ���� R ǥ�ø� ���ֹǷ� �¿�ǥ�ø� �����Ѵ�.
// �ٸ������� �ʿ��ϴٸ� �׶� �����ϵ��� ��. 2005.2.4
void CDrawDBDeck::MarkRadiusRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, CDRect rectLimit)
{
	if(!pRC) return;

	if(pRC->m_R_Mid > 0 && pRC->GetLengthMid() > 0)
	{
		BOOL bLeftDir	= pRC->m_xyMid2.x > pRC->m_xyMid1.x;
		CDPoint xyMark	= pRC->m_xyCen;
		CDPoint xyDir	= bLeftDir ? ToDPointFrDegree(-45) : ToDPointFrDegree(225);
		xyMark			= xyMark + xyDir * pRC->m_R_Mid;

		// �ش� ������ �ؾ� �� ���...
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




//xyMark : ö���� �߾���ġ 
//nPos   : ö���� �����ġ(��, ��, ��, ��)
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


// UINT nTypeDrawOverLapLine : overlap ���� �׸��� ��� 
// 0 : �׸��� ����. 1 : ������ �׸�,
void CDrawDBDeck::DrawTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bDrawInfo, BOOL bDrawDirInfo, UINT nTypeDrawOverLapLine)
{
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(pDomP, tvArr, bDrawInfo, bDrawDirInfo, nTypeDrawOverLapLine);
	return;

	if(tvArr.GetSize() < 1) return;
	CRebarPlacing rbPlacing;
	CTwinVector tv;
		
	CDPoint xyCen(0, 0);	// ���� �߽�
	CDPoint xy1(0, 0);		// ���̳� ���� ù��° ��ǥ
	CDPoint xy2(0, 0);		// ���̳� ���� �ι�° ��ǥ
	CDPoint xy3(0, 0);		// ���̳� ���� ����° ��ǥ
	double dR		= 0;	// ���� ������
	double dAngStt	= 0;	// ��ũ�� ���۰�
	double dAngEnd	= 0;	// ��ũ�� ����
	
	
	// �׷��ڵ�� �⺻���� 0 ��.
	if(!m_bSettingGroupCode) pDomP->SetCurGroupCode(0);

	for(long i = 0; i < tvArr.GetSize(); i++)
	{
		tv = tvArr[i];

		// �׷��ڵ带 ��������� �ϴ� ������ �Ǵ��ؼ� �׷� ����.(�׷� �ʿ� ������ 0���� ����)
		if(m_bSettingGroupCode)
		{
			if(tv.val.valHor || tv.val.valVer)
				pDomP->SetCurGroupCode(m_nCurGroupCode++);
			else
				pDomP->SetCurGroupCode(0);
		}

		// ��ġ�� ���� ���� ��� �Ȱ�ġ�� �κи� �׷���.(���� ������ ���ؼ��� ������)
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
				// �������� ����
				double dOldAngle = pDomP->GetTextAngle();
				double dOldScale = pDomP->GetScaleDim();

				// �ؽ�Ʈ ����
				pDomP->SetScaleDim(1);
				pDomP->SetTextHeight(50);
				pDomP->SetTextAngle(tv.GetXyDir().GetAngleDegree());
				pDomP->SetTextAlignHorz(TA_CENTER);
				pDomP->TextOut((tv.m_v1+tv.m_v2)/2, str);

				// �������� ����
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

			// ���� �߽� ã��
			xyCen = rbPlacing.GetXyCircleCenter(xy1, xy2, dR, bLeft);

			// ���� �߽ɿ��� �� �������� ����
			dAngStt = (xy1-xyCen).Unit().GetAngleDegree();
			dAngEnd = (xy2-xyCen).Unit().GetAngleDegree();
			if(dAngEnd < dAngStt) dAngEnd+=360;

			// ��ũ �׸���
			//double dDist = ~(xy1, xy2);
			//if(~(xy1, xy2) <= dR*2)
			{
				pDomP->Arc(xyCen, dR, dAngStt, dAngEnd-dAngStt);
			}
		}
		else if(tv.m_v1.z == 2)	// ������ ������ �׸�.. ������ ���� ��츸 �ش�
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