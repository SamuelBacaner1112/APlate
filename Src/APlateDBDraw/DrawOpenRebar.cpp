// DrawOpenRebar.cpp: implementation of the CDrawOpenRebar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDBDraw.h"
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define COUNT_BARPOS 7

CDrawOpenRebar::CDrawOpenRebar(CDataManage *pDataMng) : m_pDataMng(pDataMng)
{
	m_nBarType = 0;
}

CDrawOpenRebar::~CDrawOpenRebar()
{

}

void CDrawOpenRebar::DrawRebar(CDomyun *pDom)
{
	if(m_nBarType == 0)  //Round
		DrawRoundRebar(pDom);
	else if(m_nBarType == 1)
		DrawBoxRebar(pDom);
}


void CDrawOpenRebar::DimRebar(CDomyun *pDom, BOOL bInput)
{
	if(m_nBarType == 0)  //Round
		DimRoundRebar(pDom, bInput);
	else if(m_nBarType == 1)
		DimBoxRebar(pDom, bInput);
}


void CDrawOpenRebar::MarkRebar(CDomyun *pDom)
{
	if(m_nBarType == 0)  //Round
		MarkRoundRebar(pDom);
	else if(m_nBarType == 1)
		MarkBoxRebar(pDom);
}


void CDrawOpenRebar::DrawBoxRebar(CDomyun *pDom)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	CBoxOpenBar* pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleBoxType;
	long  i = 0;
	/************************************************************************/
	/* ����                                                                 */
	/************************************************************************/
	for(i = 0; i < 4; i++)
	{
		CDPointArray ptArrLine = pOpen->GetPosStraightBar(i);
		for(int j = 0; j < ptArrLine.GetSize(); j+=2)
		{
			CDPoint ptStt = ptArrLine.GetAt(j);
			CDPoint ptEnd = ptArrLine.GetAt(j+1);
			Dom.LineTo(ptStt, ptEnd);
		}
	}

	/************************************************************************/
	/* �밢��                                                               */
	/************************************************************************/
	for(i = 0; i < 4; i++)
	{
		CDPointArray ptArrLine = pOpen->GetPosDiagonalBar(i);
		for(int j = 0; j < ptArrLine.GetSize(); j+=2)
		{
			CDPoint ptStt = ptArrLine.GetAt(j);
			CDPoint ptEnd = ptArrLine.GetAt(j+1);
			Dom.LineTo(ptStt, ptEnd);
		}
	}

	*pDom << Dom;
}

void CDrawOpenRebar::DimBoxRebar(CDomyun *pDom, BOOL bInput)
{
	DimBoxDiagonalBar(pDom, bInput);
	DimBoxStraightBar(pDom, bInput);
}

void CDrawOpenRebar::DrawRoundRebar(CDomyun *pDom)
{
	CRoundOpenBar* pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleRoundType;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	vector<CDPointArray> vecUpper, vecLower;
	pOpen->GetPosRebar(vecUpper, vecLower);
	DrawPosRoundRebar(&Dom, vecUpper, TRUE);
	DrawPosRoundRebar(&Dom, vecLower, FALSE);

	*pDom << Dom;
}


void CDrawOpenRebar::DimRoundRebar(CDomyun *pDom, BOOL bInput)
{
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);
	
	CRoundOpenBar* pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleRoundType;
	vector<CDPointArray> vecUpper, vecLower;
	pOpen->GetPosRebar(vecUpper, vecLower);
	Dom.SetDirection("TOP");

	int nSize = vecUpper.size();
	CDPointArray ptArr = vecUpper[nSize-1];
	int nSizeEx = ptArr.GetSize();
	if(nSizeEx > 0)
	{
		CDPoint ptDot[COUNT_BARPOS];
		for(int i = 0; i < nSizeEx; i++)
			ptDot[i] = ptArr.GetAt(i);
		
		//���� 1
		double dLen = pOpen->GetLengthLine();
		double dLenSettle = pOpen->GetLengthAnchorage();
		CString str;
		Dom.DimMoveTo(ptDot[0].x, ptDot[0].y, 0);
		str.Format("%s", COMMA(dLenSettle));
		Dom.DimLineToExtend(dLenSettle, 0, "", str);
		str.Format("%s", COMMA(dLen - dLenSettle));
		Dom.DimLineTo(dLen - dLenSettle, 0, str);
		CDPoint ptAng = ToDPointFrDegree(pOpen->GetAngle()/2);
		Dom.Rotate(ptDot[0], ptAng);
		*pDom << Dom;

		//���� 2
		str.Format("%s", COMMA(dLen));
		Dom.DimMoveTo(ptDot[3].x, ptDot[3].y, 0);
		str.Format("%s", COMMA(dLen - dLenSettle));
		Dom.DimLineTo(dLen - dLenSettle, 0, str);
		str.Format("%s", COMMA(dLenSettle));
		Dom.DimLineToExtend(dLenSettle, 0, "", str);
		ptAng = ToDPointFrDegree(-pOpen->GetAngle()/2);
		Dom.Rotate(ptDot[3], ptAng);
		*pDom << Dom;

		//Arc
		CString str2;
		str2.Format("R = %0.0f", pOpen->GetRadius());
		dLen = pOpen->GetLengthArc();
		str.Format("%s", COMMA(dLen));
		Dom.DimArcMoveTo(pOpen->GetOpenCenter().x, pOpen->GetOpenCenter().y + pOpen->GetSpaceForCalc()*(nSize-1), pOpen->GetRadius(), ptDot[3].x, ptDot[3].y, 0);
		Dom.DimArcLineToExtend(ptDot[2].x, ptDot[2].y, 0, str, str2);
		*pDom << Dom;
	}
}


void CDrawOpenRebar::MarkRoundRebar(CDomyun *pDom)
{
	CADeckData* pDeckData = m_pDataMng->GetDeckData();
	CRoundOpenBar* pOpen = &pDeckData->m_WaterHoleRoundType;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	CConcSymbol Sym;
	vector<CDPointArray> vecUpper, vecLower;
	pOpen->GetPosRebar(vecUpper, vecLower);

	int nSize = vecLower.size();
	CRebar *pRB = pDeckData->GetRebarByName("���������ö��");
	if(nSize > 0)
	{
		CDPointArray ptArr = vecLower[nSize-1];
		int nSizeEx = ptArr.GetSize();
		if(nSizeEx > 0)
		{
			CDPoint ptStt = ptArr.GetAt(3);
			CDPoint ptEnd = ptArr.GetAt(5);
			CDPoint ptMark = (ptStt + ptEnd)/2;
			if(pRB)
				Sym.RebarMarkLineLower(&Dom, ptMark, pRB->m_sMark, pRB->GetStringDia(), FALSE);
		}
	}

	*pDom << Dom;
}


void CDrawOpenRebar::DimBoxDiagonalBar(CDomyun *pDom, BOOL bInput)
{
	CBoxOpenBar* pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleBoxType;
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("BOTTOM");

	//���Ͽ� ����� ������ �κп� ġ������ �׸���.
	CDPointArray ptArrLine = pOpen->GetPosDiagonalBar(3);  //���� 
	CDPointArray ptArrDot;
	int nSize = ptArrLine.GetSize();
	CDPoint ptStt = ptArrLine.GetAt(nSize - 2);
	CDPoint ptEnd = ptArrLine.GetAt(nSize - 1);
	ptArrDot.Add(ptStt);
	ptArrDot.Add(ptEnd);

	//������ ������ 
	ptArrLine = pOpen->GetPosDiagonalBar(2);  //����
	nSize = ptArrLine.GetSize();
	int i = 0;
	for(i = 0; i < nSize ; i+=2)
	{
		CDPoint ptSttEx = ptArrLine.GetAt(i);
		CDPoint ptEndEx = ptArrLine.GetAt(i+1);
		CDPoint ptCross;
		GetXyMatchSegAndSeg(ptStt, ptEnd, ptSttEx, ptEndEx, ptCross);
		ptArrDot.Add(ptCross);
	}

	
	//����� ������
	ptArrLine = pOpen->GetPosDiagonalBar(1);  //��� 
	nSize = ptArrLine.GetSize();
	for(i = 0; i < nSize ; i+=2)
	{
		CDPoint ptSttEx = ptArrLine.GetAt(i);
		CDPoint ptEndEx = ptArrLine.GetAt(i+1);
		CDPoint ptCross;
		GetXyMatchSegAndSeg(ptStt, ptEnd, ptSttEx, ptEndEx, ptCross);
		ptArrDot.Add(ptCross);
	}

	ptArrDot.Sort(TRUE);
	
	Dom.DimMoveTo(ptStt.x, ptStt.y, 0);
	for(i = 0; i < ptArrDot.GetSize() - 1; i++)
	{
		CDPoint ptSttEx = ptArrDot.GetAt(i);
		CDPoint ptEndEx = ptArrDot.GetAt(i+1);
		double dDis = ~(ptSttEx - ptEndEx);
		dDis = Round(dDis, 2);
		CString str;
		str.Format("%s", COMMA(dDis));
		if(dDis == pOpen->GetSpaceRebar())
			Dom.DimLineToExtend(dDis, 0, "", str);
		else
			Dom.DimLineTo(dDis, 0, str);
	}

	CDPoint ptAngle = (ptEnd - ptStt).Unit();
	Dom.Rotate(ptStt, ptAngle);

	*pDom << Dom;
}


void CDrawOpenRebar::DimBoxStraightBar(CDomyun *pDom, BOOL bInput)
{
	CBoxOpenBar* pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleBoxType;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("TOP");

	map<double, int> mapDim;
	map<double, int>::iterator pos, pos2;

	CDPointArray ptArrDot, ptArrLine;
	CDPoint ptDot, ptStt, ptEnd;
	int nSize = 0;

	//���� ������ 
	ptArrLine = pOpen->GetPosStraightBar(2);  //����
	nSize = ptArrLine.GetSize();
	int i = 0;
	for(i = 0; i < nSize ; i+=2)
	{
		ptDot = ptArrLine.GetAt(i+1);
		if(i == nSize - 2)
			mapDim.insert(make_pair(ptDot.x, 1));
		else
			mapDim.insert(make_pair(ptDot.x, 0));
	}

	
	//���� ������ 
	ptArrLine = pOpen->GetPosStraightBar(3);  //����
	nSize = ptArrLine.GetSize();
	for(i = 0; i < nSize ; i+=2)
	{
		ptDot = ptArrLine.GetAt(i+1);
		if(i == 0)
			mapDim.insert(make_pair(ptDot.x, 1));
		else
			mapDim.insert(make_pair(ptDot.x, 0));
	}


	//������, ���� 
	ptArrLine = pOpen->GetPosStraightBar(0);  //��� 
	nSize = ptArrLine.GetSize();
	ptStt = ptArrLine.GetAt(nSize - 2);
	ptStt.y = ptDot.y;
	mapDim.insert(make_pair(ptStt.x, 1));
	ptEnd = ptArrLine.GetAt(nSize - 1);
	mapDim.insert(make_pair(ptEnd.x, 1));

	CString str;
	Dom.DimMoveTo(ptStt.x, ptStt.y, 0);
	nSize = mapDim.size();
	pos = mapDim.begin();
	for(i = 0; i < nSize-1 ; i++)
	{
		pos2 = pos;
		pos2++;
		double dDis = pos2->first - pos->first;
		str.Format("%s", COMMA(dDis));
		if(pos2->second == 0)  //�Ϻ� 
			Dom.DimLineToExtend(dDis, 0, "", str);
		else  //��� 
			Dom.DimLineTo(dDis, 0, str);

		pos++;
	}

	/************************************************************************/
	/* �Ǻ�ǥ��                                                             */
	/************************************************************************/
	CDPoint ptCen = pOpen->GetOpenCenter();
	if(pOpen->GetOpenType() == 0) //���� ����� 
		ptStt.x = ptCen.x + pOpen->GetOpenRadius();
	else
		ptStt.x = ptCen.x + pOpen->GetWidthOpen()/2;

	ptStt.y = ptCen.y;
/*	if(bInput == TRUE)
		Dom.DimTextLineExt2(ptStt.x, ptStt.y, pOpen->GetCover(), COMMA(pOpen->GetCover()), "P");
	else
		Dom.DimTextLineExt2(ptStt.x, ptStt.y, pOpen->GetCover(), COMMA(pOpen->GetCover()), "");
*/
	*pDom << Dom;
}


void CDrawOpenRebar::MarkBoxRebar(CDomyun* pDom)
{
	CADeckData  *pDeckData = m_pDataMng->GetDeckData();
	CBoxOpenBar *pOpen	   = &pDeckData->m_WaterHoleBoxType;

	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_DIML);

	CConcSymbol Sym(m_pDataMng->GetOptionStd());
	CDPointArray ptArr = pOpen->GetPosDiagonalBar(3);

	int nSize = ptArr.GetSize();
	CRebar *pRB = pDeckData->GetRebarByName("���������ö��");
	if(nSize > 0)
	{
		CDPoint ptStt = ptArr.GetAt(nSize - 2);
		CDPoint ptEnd = ptArr.GetAt(nSize - 1);
		double dLen = ~(ptEnd - ptStt);
		CDPoint ptMark = ptStt + (ptEnd - ptStt).Unit()*(dLen*0.8);
		if(pRB)	Sym.RebarMarkLineUpper(&Dom, ptMark, pRB->m_sMark, pRB->GetStringDia(), FALSE);
	}

	*pDom << Dom;
}


void CDrawOpenRebar::DrawPosRoundRebar(CDomyun *pDom, vector<CDPointArray>& vecBar, BOOL bUpper)
{
	CRoundOpenBar* pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleRoundType;
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_BARC);

	CDPoint ptDot[COUNT_BARPOS];
	
	for(int i = 0; i < (long)vecBar.size(); i++)
	{
		CDPointArray ptArr = vecBar[i];
		int nSize = ptArr.GetSize();
		if(nSize == COUNT_BARPOS)
		{
			CDPoint ptCen = ptArr[nSize - 1];
			for(int j = 0; j < ptArr.GetSize(); j++)
				ptDot[j] = ptArr[j];

			Dom.LineTo(ptDot[0], ptDot[2]);
			Dom.LineTo(ptDot[3], ptDot[5]);

			if(bUpper == TRUE)
				Dom.Arc(ptCen.x , ptCen.y, pOpen->GetRadius(), 90 - pOpen->GetAngle()/2, pOpen->GetAngle());
			else if(bUpper == FALSE) 
				Dom.Arc(ptCen.x , ptCen.y, pOpen->GetRadius(), 270 - pOpen->GetAngle()/2, pOpen->GetAngle());
		}
	}

	*pDom << Dom;
}


//�����θ� �׸���.
void CDrawOpenRebar::DrawOpen(CDomyun *pDom, BOOL bDrawName)
{
	COpenBar* pOpen = NULL;
	if(m_nBarType == 0)
		pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleRoundType;
	else
		pOpen = &m_pDataMng->GetDeckData()->m_WaterHoleBoxType;
	
	CDomyun Dom(pDom);
	m_pDataMng->GetOptionStd()->SetEnvType(&Dom, HCAD_CONC);

	CDPoint ptCen = pOpen->GetOpenCenter();
	double dWidth = pOpen->GetWidthOpen();
	double dLength = pOpen->GetLengthOpen();

	if(pOpen->GetOpenType() == 0)  //������ Ÿ���� ����
	{
		if(m_nBarType == 0)  //������ ������ ö�� 
		{
			vector<CDPointArray> vecUpper, vecLower;
			m_pDataMng->GetDeckData()->m_WaterHoleRoundType.GetPosRebar(vecUpper, vecLower);
			int nSizeUpper = vecUpper.size();
			int nSizeLower = vecLower.size();
			if(nSizeUpper > 0 && nSizeLower > 0)
			{
				CDPointArray ptArrUpper, ptArrLower;
				ptArrUpper = vecUpper[0];
				ptArrLower = vecLower[nSizeLower-1];
				CDPoint ptStt = ptArrUpper.GetAt(0);
				CDPoint ptEnd = ptArrLower.GetAt(0);
				CDPoint xyCen = (ptStt + ptEnd)/2;
				ptCen.y = xyCen.y;
			}
		}

		Dom.Circle(ptCen, pOpen->GetOpenRadius());		
	}
	else if(pOpen->GetOpenType() == 1)  //�ڽ�Ÿ�� 
	{
		if(m_nBarType == 0)  //������ ������ ö�� 
		{
			vector<CDPointArray> vecUpper, vecLower;
			m_pDataMng->GetDeckData()->m_WaterHoleRoundType.GetPosRebar(vecUpper, vecLower);
			int nSizeUpper = vecUpper.size();
			int nSizeLower = vecLower.size();
			if(nSizeUpper > 0 && nSizeLower > 0)
			{
				CDPointArray ptArrUpper, ptArrLower;
				ptArrUpper = vecUpper[0];
				ptArrLower = vecLower[nSizeLower-1];
				CDPoint ptStt = ptArrUpper.GetAt(0);
				CDPoint ptEnd = ptArrLower.GetAt(0);
				CDPoint xyCen = (ptStt + ptEnd)/2;
				ptCen.y = xyCen.y;
			}
		}

		CDPoint ptLT(ptCen.x - dWidth/2, ptCen.y + dLength/2);
		CDPoint ptRB(ptCen.x + dWidth/2, ptCen.y - dLength/2);
		Dom.Rectangle(ptLT, ptRB);
		
		// X ǥ�� 
		Dom.LineTo(ptLT, ptRB);
		Dom.LineTo(ptLT.x + dWidth, ptLT.y, ptRB.x - dWidth, ptRB.y);
	}

	//���ڿ� ���
	if(pOpen->GetNameOpenType().GetLength() != 0 && bDrawName == TRUE)
	{
		double dWidth = Dom.GetTextWidth(pOpen->GetNameOpenType());
		double dHeight = Dom.GetTextHeight();
		Dom.TextOut(ptCen.x - dWidth/2, ptCen.y - dHeight/2, pOpen->GetNameOpenType());
	}

	*pDom << Dom;
}
