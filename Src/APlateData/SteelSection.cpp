// SteelSection.cpp: implementation of the CSteelSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SteelSection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DATASU	5

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSteelSection::CSteelSection()
{
	m_bSameBuje = TRUE;
	ClearKey();
	DataInit();
}

CSteelSection::~CSteelSection()
{
	RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
//	class	:	CSteelSection
void CSteelSection::RemoveAll()
{
	long size = m_SectionArr.GetSize();
	if(size < 1)	return;
	for(long n = 0 ; n < size ; n++)
		delete m_SectionArr[n];
	m_SectionArr.RemoveAll();
}

void CSteelSection::Serialize(CArchive& ar)
{
	long size	= 0;
//	long nFlag	= 0;
//	long nFlag	= 1;	// FillerPlate 추가 ..090521 lina
//	long nFlag	= 2;
	long nFlag	= 3;	// 가로보 HSteel,각형전단연결재 누락현상 수정

	if(ar.IsStoring()) 
	{
		ar << nFlag;
		size = 	m_SectionArr.GetSize();
		ar << size;
		for(long n = 0 ; n < size ; n++)
			m_SectionArr[n]->Serialize(ar);
		ar << m_bSameBuje;
	}
	else 
	{
		ar >> nFlag;
		RemoveAll();
		ar >> size;

		for(long n = 0 ; n < size ; n++)
		{
			CSteelSectionData *pData = new CSteelSectionData;
			pData->Serialize(ar);

			if(nFlag<3 && (n==CP_STEEL || n==CG_STEEL) && pData->m_strName!="H형강")
			{
				CSteelSectionData *pData2 = new CSteelSectionData;
				pData2->m_strType		= pData->m_strType;
				pData2->m_strName		= "H형강";
				pData2->m_strShape		= "형강";
				pData2->m_strClass		= "부부재";
				pData2->m_strSymbol		= pData->m_strSymbol;
				pData2->m_strSizeType	= "대형부재";
				m_SectionArr.InsertAt(n, pData2);
			}
			if(nFlag<3 && n==GI_SQ && pData->m_strName.Find("각형")==-1)
			{
				CSteelSectionData *pData2 = new CSteelSectionData;
				pData2->m_strType		= pData->m_strType;
				pData2->m_strName		= "전단연결재(각형)";
				pData2->m_strShape		= "플레이트";
				pData2->m_strClass		= "부부재";
				pData2->m_strSymbol		= GetStringData(G_F_U, BDISE_SYMBOL);
				pData2->m_strSizeType	= "소형부재";
				m_SectionArr.InsertAt(n, pData2);
			}
			m_SectionArr.Add(pData);
			if(nFlag<1 && pData->m_strType=="상부 현장이음"		&& pData->m_strName=="하면 이음판") SerializeFlag1(ar, pData);
			if(nFlag<1 && pData->m_strType=="하부 현장이음"		&& pData->m_strName=="상면 이음판") SerializeFlag1(ar, pData);
			if(nFlag<1 && pData->m_strType=="복부판 현장이음"	&& pData->m_strName=="전단 이음판") SerializeFlag1(ar, pData);
		}
		if(nFlag>1)
			ar >> m_bSameBuje;
	}
}

// nFlag = 1 되면서 추가된 부재
void CSteelSection::SerializeFlag1(CArchive& ar, CSteelSectionData *pData)
{
	CSteelSectionData *pDataAdd = new CSteelSectionData;
	pDataAdd->m_strType		= pData->m_strType;
	pDataAdd->m_strName		= "채움판";
	pDataAdd->m_strShape	= "플레이트";
	pDataAdd->m_strClass	= pData->m_strClass;
	pDataAdd->m_strSymbol	= pData->m_strSymbol;
	pDataAdd->m_strSizeType	= pData->m_strSizeType;

	m_SectionArr.Add(pDataAdd);	
}

void CSteelSection::DataInit()
{
	char* szBaseStr[] = {
	"일반 ,상판,플레이트,주부재,SM490B,대형부재",					// G_F_U 0	
	"일반 ,하판,플레이트,주부재,SM490B,대형부재",
	"일반 ,복부판,플레이트,주부재,SM490B,대형부재",
	"일반 ,수평보강재(좌),플레이트,주부재,SM490B,소형부재",
	"일반 ,수평보강재(우),플레이트,주부재,SM490B,소형부재",
	
	"일반 ,수직보강재,플레이트,부부재,SM400B,소형부재",
	"일반 ,지점보강재,플레이트,주부재,SM490B,소형부재",

	"수직브레이싱, 상현재, 형강, 부부재, SM400B,소형부재",
	"수직브레이싱, 하현재, 형강, 부부재, SM400B,소형부재",
	"수직브레이싱, 사재, 형강, 부부재, SM400B,소형부재",
	"수직브레이싱, 이음판, 플레이트, 부부재, SM400B,소형부재",
	"수직브레이싱, 볼트&&너트,볼트,볼트,F10T,소형부재",

	"수평브레이싱, 현재, 형강, 부부재, SM400B,소형부재",
	"수평브레이싱, 이음판, 플레이트, 부부재, SM400B,소형부재",
	"수평브레이싱, 볼트&&너트,볼트,볼트,F10T,소형부재",				// VB_BOLT

	"상부 현장이음,외부 포장면,플레이트,주부재,SM490B,소형부재",
	"상부 현장이음,하면 이음판,플레이트,주부재,SM490B,소형부재",
	"상부 현장이음,채움판,플레이트,주부재,SM490B,소형부재",	// 090521 추가 nFlag=1
	"상부 현장이음,볼트&&너트,볼트,볼트,F10T,소형부재",
	"하부 현장이음,외측 이음판,플레이트,주부재,SM490B,소형부재",
	"하부 현장이음,상면 이음판,플레이트,주부재,SM490B,소형부재",
	"하부 현장이음,채움판,플레이트,주부재,SM490B,소형부재",	// 090521 추가 nFlag=1
	"하부 현장이음,볼트&&너트,볼트,볼트,F10T,소형부재",
	"복부판 현장이음,모멘트 이음판,플레이트,주부재,SM490B,소형부재",
	"복부판 현장이음,전단 이음판,플레이트,주부재,SM490B,소형부재",
	"복부판 현장이음,채움판,플레이트,주부재,SM490B,소형부재",	// 090521 추가 nFlag=1
	"복부판 현장이음,볼트&&너트,볼트,볼트,F10T,소형부재",

	"가로보 지점부,복부판,플레이트,부부재,SM400B,대형부재",
	"가로보 지점부,상판,플레이트,부부재,SM400B,대형부재",
	"가로보 지점부,하판,플레이트,부부재,SM400B,대형부재",
	"가로보 지점부,H형강,형강,부부재,SM490A,대형부재",
	"가로보 지점부,브라켓 복부판,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,브라켓 상판,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,브라켓 하판,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,브라켓 보강재,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,브라켓 보강판,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,수직보강재,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,수평보강재,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,상부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,상부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,하부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,하부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,복부판 이음판 모멘트,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,복부판 이음판 전단,플레이트,부부재,SM400B,소형부재",
	"가로보 지점부,채움판,플레이트,부부재,SM400B,소형부재",			// 추가
	"가로보 지점부,볼트&&너트,볼트,볼트,F10T,소형부재",				// CP_BOLT

	"가로보 일반부,복부판,플레이트,부부재,SM400B,대형부재",
	"가로보 일반부,상판,플레이트,부부재,SM400B,대형부재",
	"가로보 일반부,하판,플레이트,부부재,SM400B,대형부재",
	"가로보 일반부,H형강,형강,부부재,SM490A,대형부재",
	"가로보 일반부,브라켓 복부판,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,브라켓 상판,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,브라켓 하판,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,브라켓 보강재,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,브라켓 보강판,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,수직보강재,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,수평보강재,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,상부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,상부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,하부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,하부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,복부판 이음판 모멘트,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,복부판 이음판 전단,플레이트,부부재,SM400B,소형부재",
	"가로보 일반부,채움판,플레이트,부부재,SM400B,소형부재",			// 추가
	"가로보 일반부,볼트&&너트,볼트,볼트,F10T,소형부재",				// CG_BOLT	

	"세로보,복부판,플레이트,부부재,SM400B,대형부재",
	"세로보,상판,플레이트,부부재,SM400B,대형부재",
	"세로보,하판,플레이트,부부재,SM400B,대형부재",
	"세로보,브라켓 복부판,플레이트,부부재,SM400B,소형부재",
	"세로보,브라켓 상판,플레이트,부부재,SM400B,소형부재",
	"세로보,브라켓 하판,플레이트,부부재,SM400B,소형부재",
	"세로보,브라켓 보강재,플레이트,부부재,SM400B,소형부재",
	"세로보,브라켓 보강판,플레이트,부부재,SM400B,소형부재",
	"세로보,수직보강재,플레이트,부부재,SM400B,소형부재",
	"세로보,수평보강재,플레이트,부부재,SM400B,소형부재",
	"세로보,상부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"세로보,상부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"세로보,하부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"세로보,하부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"세로보,복부판 이음판,플레이트,부부재,SM400B,소형부재",
	"세로보,볼트&&너트,볼트,볼트,F10T,소형부재",					// ST_BOLT
	
	"외측가로보 지점부,복부판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,상판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,하판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,브라켓 복부판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,브라켓 상판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,브라켓 하판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,브라켓 보강재,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,브라켓 보강판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,수직보강재,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,수평보강재,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,상부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,상부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,하부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,하부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,복부판 이음판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 지점부,볼트&&너트,볼트,볼트,F10T,소형부재",			// WI_P_BOLT

	"외측가로보 일반부,복부판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,상판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,하판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,브라켓 복부판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,브라켓 상판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,브라켓 하판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,브라켓 보강재,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,브라켓 보강판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,수직보강재,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,수평보강재,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,상부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,상부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,하부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,하부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,복부판 이음판,플레이트,부부재,SM400B,소형부재",
	"외측가로보 일반부,볼트&&너트,볼트,볼트,F10T,소형부재",			// WI_G_BOLT

	"외측빔 ,복부판,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,상판,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,하판,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,브라켓 복부판,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,브라켓 상판,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,브라켓 하판,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,수직보강재,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,수평보강재,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,상부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"외측빔 ,상부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"외측빔 ,하부 이음판(상면),플레이트,부부재,SM400B,소형부재",
	"외측빔 ,하부 이음판(하면),플레이트,부부재,SM400B,소형부재",
	"외측빔 ,복부판 이음판,플레이트,부부재,SM400B,소형부재",
	"외측빔 ,볼트&&너트,볼트,볼트,F10T,소형부재",
	
	"기타부재,전단연결재,봉강,봉강,SD30,소형부재",
	"기타부재,전단연결재(각형),플레이트,부부재,SM400B,소형부재",
	"기타부재,SLAB ANCHOR,봉강,봉강,SD30,소형부재",
	"기타부재,솔플레이트,플레이트,주부재,SM490B,소형부재",	
	"기타부재,FACE PLATE,플레이트,주부재,SM490B,소형부재",	
	"기타부재,들고리,플레이트,주부재,SM490B,소형부재",
	"기타부재,들고리링,플레이트,주부재,SM490B,소형부재",
	"기타부재,들고리보강재,플레이트,주부재,SM490B,소형부재",
	"기타부재,잭업보강재(좌),플레이트,주부재,SM490B,소형부재",
	"기타부재,잭업보강재(우),플레이트,주부재,SM490B,소형부재",
	"기타부재,잭업 솔플레이트,플레이트,주부재,SM490B,소형부재",
	"기타부재,낙교방지책,플레이트,부부재,SM400B,소형부재",
	"기타부재,유지보수용 동바리,플레이트,부부재,SM400B,소형부재",	// GI_HG
	};

	long nCount = sizeof(szBaseStr) / sizeof(szBaseStr[0]);
	CStringArray SArr;
	RemoveAll();

	for(long n = 0; n < nCount; n++)
	{
		if(DivideStr(SArr, szBaseStr[n], ',' )==FALSE) continue;

		// File에서 입력한 데이터가 적은 경우 처리
		while(SArr.GetSize() < DATASU) SArr.Add(_T(""));

		CSteelSectionData *pData = new CSteelSectionData;

		pData->m_strType	= SArr[0];
		pData->m_strName	= SArr[1];
		pData->m_strShape	= SArr[2];
		pData->m_strClass	= SArr[3];
		pData->m_strSymbol	= SArr[4];
		pData->m_strSizeType= SArr[5];

		m_SectionArr.Add(pData);
	}
}

void	CSteelSection::ClearKey()
{
	m_KeyRec.m_strType   = "";
	m_KeyRec.m_strName   = "";
	m_KeyRec.m_strShape  = "";
	m_KeyRec.m_strClass  = "";
	m_KeyRec.m_strSymbol = "";
	m_KeyRec.m_strSizeType = "";
	

	m_idx	= 0;
	m_Key   = 0;
}

void	CSteelSection::AllocKey(CSteelSectionData* pse, long KeyMode)
{
	ClearKey();
	m_Key = KeyMode;

	if(m_Key & BDISE_TYPE)		m_KeyRec.m_strType		= pse->m_strType;
	if(m_Key & BDISE_NAME)		m_KeyRec.m_strName		= pse->m_strName;
	if(m_Key & BDISE_SHAPE)		m_KeyRec.m_strShape		= pse->m_strShape;
	if(m_Key & BDISE_CLASS)		m_KeyRec.m_strClass		= pse->m_strClass;
	if(m_Key & BDISE_SYMBOL)	m_KeyRec.m_strSymbol	= pse->m_strSymbol;
	if(m_Key & BDISE_SIZETYPE)	m_KeyRec.m_strSizeType	= pse->m_strSizeType;
	
}

BOOL	CSteelSection::StartKey()
{
	long size;
	m_idx = 0;
	
	size = m_SectionArr.GetSize();
	while(m_idx < size) {
		if(m_Key == 0)	return TRUE;

		if(m_Key & BDISE_TYPE) {
			if(m_KeyRec.m_strType != m_SectionArr[m_idx]->m_strType) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_NAME) {
			if(m_KeyRec.m_strName != m_SectionArr[m_idx]->m_strName) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SHAPE) {
			if(m_KeyRec.m_strShape != m_SectionArr[m_idx]->m_strShape) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_CLASS)	{
			if(m_KeyRec.m_strClass != m_SectionArr[m_idx]->m_strClass) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SYMBOL) {
			if(m_KeyRec.m_strSymbol != m_SectionArr[m_idx]->m_strSymbol) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SIZETYPE) {
			if(m_KeyRec.m_strSizeType != m_SectionArr[m_idx]->m_strSizeType) {
				m_idx++;	continue;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL	CSteelSection::NextKey()
{
	long size;
	
	m_idx++;
	size = m_SectionArr.GetSize();
	while(m_idx < size) {
		if(m_Key == 0)	return TRUE;

		if(m_Key & BDISE_TYPE) {
			if(m_KeyRec.m_strType != m_SectionArr[m_idx]->m_strType) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_NAME) {
			if(m_KeyRec.m_strName != m_SectionArr[m_idx]->m_strName) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SHAPE) {
			if(m_KeyRec.m_strShape != m_SectionArr[m_idx]->m_strShape) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_CLASS)	{
			if(m_KeyRec.m_strClass != m_SectionArr[m_idx]->m_strClass) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SYMBOL) {
			if(m_KeyRec.m_strSymbol != m_SectionArr[m_idx]->m_strSymbol) {
				m_idx++;	continue;
			}
		}
		if(m_Key & BDISE_SIZETYPE) {
			if(m_KeyRec.m_strSizeType != m_SectionArr[m_idx]->m_strSizeType) {
				m_idx++;	continue;
			}
		}
		return TRUE;
	}
	return FALSE;
}

CSteelSectionData*	CSteelSection::GetRecord(CSteelSectionData* pBS)
{
	if(m_idx >= m_SectionArr.GetSize())
	{
		pBS->m_strType	= "";
		pBS->m_strName  = "";
		pBS->m_strShape = "";
		pBS->m_strClass = "";
		pBS->m_strSymbol= "";
		pBS->m_strSizeType= "";
		return NULL;
	}

	*pBS = *m_SectionArr[m_idx];
	return pBS;
}

CString CSteelSection::GetStringData(long idx, SECTION_DATA_ID ID) const
{
	CString str(_T(""));
	if(idx >= m_SectionArr.GetSize())	return str;

	switch(ID) {
	case BDISE_TYPE:
		return m_SectionArr[idx]->m_strType;
	case BDISE_NAME:
		return m_SectionArr[idx]->m_strName;
	case BDISE_SHAPE:
		return m_SectionArr[idx]->m_strShape;
	case BDISE_CLASS:
		return m_SectionArr[idx]->m_strClass;
	case BDISE_SYMBOL:
		return m_SectionArr[idx]->m_strSymbol;
	case BDISE_SIZETYPE:
		return m_SectionArr[idx]->m_strSizeType;
	default:
		str = "Out of Range ID";
	}
	return str;
}

long CSteelSection::GetIndex(const CString& strType, const CString& strName) const
{
	long size = m_SectionArr.GetSize();
	for(long i = 0; i < size; i++) 
	{
		CSteelSectionData* pBS = m_SectionArr[i];
		if (strType == pBS->m_strType && strName == pBS->m_strName) 
			return i;
	}

	return -1;
}

CString	CSteelSection::GetType(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strType;	
}

CString	CSteelSection::GetName(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strName;	
}
CString	CSteelSection::GetShape(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strShape;	
}
CString	CSteelSection::GetClass(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strClass;	
}
CString	CSteelSection::GetBujeSizeType(long idx) const
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) 
		return _T("");
	return m_SectionArr[idx]->m_strSizeType;	
}
CString	CSteelSection::GetBujeTypeByMarkBuje(long nMarkBuje) const
{
	if(nMarkBuje < 0 || nMarkBuje > m_SectionArr.GetUpperBound()) 
		return _T("");

	return m_SectionArr[nMarkBuje]->m_strClass;
}
CString	CSteelSection::GetMaterialByMarkBuje(long nMarkBuje) const
{
	if(nMarkBuje < 0 || nMarkBuje > m_SectionArr.GetUpperBound()) 
		return _T("");

	return m_SectionArr[nMarkBuje]->m_strSymbol;
}

void CSteelSection::SetType(long idx,const CString& strType  )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strType = strType;		
}
void CSteelSection::SetName(long idx,const CString& strName )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strName = strName;		
}
void CSteelSection::SetShape(long idx,const CString& strShape )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strShape = strShape;		
}
void CSteelSection::SetClass(long idx,const CString& strClass )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strClass = strClass;		
}
void CSteelSection::SetBujeSizeType(long idx,const CString& strSizeType )
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strSizeType = strSizeType;		
}

void CSteelSection::SetSymbol(long idx,const	CString& strSymbol)
{
	if(idx < 0 || idx > m_SectionArr.GetUpperBound()) return;
	m_SectionArr[idx]->m_strSymbol = strSymbol;	
}
