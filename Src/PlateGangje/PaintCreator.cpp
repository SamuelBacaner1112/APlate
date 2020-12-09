// PaintStandard.cpp: implementation of the CPaintCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "GangjeBase.h"
#include "PaintCreator.h"
#include "PaintRecord.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
  

const double BOTHSIDE = 2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaintCreator::CPaintCreator(CDataManage* pMng)
{
	m_pDataManage = pMng;
	m_pDB = pMng->GetBridge();
	m_nWorkValue = 0;				// 0:생성, 1:tbl파일로드, 2:액셀파일로드
	m_nDimType	 = DIM_BASIC;		// DimType
	m_nMakeFlag  = PAINTMAKE_SANG;	// MakeFlag
	m_dwMakeMarkFlag = 0;

/*	SetMakeMark(CMarkSerial::MARK_BUJE,TRUE);
	SetMakeMark(CMarkSerial::MARK_SECTION,TRUE);
	SetMakeMark(CMarkSerial::MARK_ASSEM,TRUE);
	SetMakeMark(CMarkSerial::MARK_ASSEM_MID,TRUE);
	SetMakeMark(CMarkSerial::MARK_ASSEM_BIG,TRUE);*/
}

CPaintCreator::~CPaintCreator()
{
	ClearAll();

}

#define ABORT(x) {if(m_bAbort == TRUE) return x;}

UINT CPaintCreator::AddAllCreator()
{
	// support progress
	CString sFmt;
	long TRange = 0,nCount = 0;
	for(long n = 0; n < m_pDB->GetGirdersu(); n++)	
		TRange += m_pDB->GetGirder(n)->GetQtySplice() + 1;

	//////////////////////////////////////////////////////////////////////
	// OPTION PAGE 	
	long nGirdersu = m_pDB->GetGirdersu();

 	for(long nG = 0; nG < nGirdersu; nG++)
	{
		CPlateGirderApp* pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER);

		while( Finder.GetBxFixNext())
		{
			OnProgress(0,  (long)((double)++nCount/(double)TRange * 90.0));// support progress

 			sFmt.Format("G%ld SP%ld 플랜지",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddFlange(pBx, Finder.GetBxFixNext());	ABORT(1)
			AddWeb(pBx, Finder.GetBxFixNext());	ABORT(1)
			
			sFmt.Format("G%ld SP%ld 수평보강재",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddHStiff(pBx, Finder.GetBxFixNext());	ABORT(1)

			sFmt.Format("G%ld SP%ld 수직보강재",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddVStiff(pBx, Finder.GetBxFixNext());	ABORT(1)	// 지점부 보강재는 기타부재에서..
			AddCrossBeamGirderVStiff(pBx, Finder.GetBxFixNext());	ABORT(1) // 가로보위치에 있는 거더부착 수직보강재
			AddVBracingGirderVStiff(pBx, Finder.GetBxFixNext());	ABORT(1) // 수직브레이싱위치에 있는 거더부착 수직보강재
				
			sFmt.Format("G%ld SP%ld 현장이음",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddFieldSplice(pBx, Finder.GetBxFixNext(), FALSE, TRUE);	ABORT(1)
			
			sFmt.Format("G%ld SP%ld 가로보",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);

			if(nG != nGirdersu-1)
			{
				AddHBracing(pBx, Finder.GetBxFixNext()); ABORT(1)
				AddVBracing(pBx, Finder.GetBxFixNext()); ABORT(1)
				AddCrossBeam(pBx, Finder.GetBxFixNext());	ABORT(1)
			}
			
			sFmt.Format("G%ld SP%ld 기타부재",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddGitaBuje(pBx, Finder.GetBxFixNext());		ABORT(1)

			pBx = Finder.GetBxNext();
		}
	}
	
	return FALSE;
}
#undef  ABORT

UINT CPaintCreator::DoWork()
{
	UINT nErr = 0;

	// DATA LIST 초기화 : 모두삭제!!!
	ClearAll();

	switch( m_nWorkValue )
	{
		case 0 : 
			nErr = AddAllCreator(); 
			break;
		case 1 : 
			nErr = MakeToLoadFileXL();	
			break;
	}

	if(nErr)	// 사용자 종료, 또는 Error시
	{
		OnProgress(4);
		ClearAll();
		return nErr;
	}

	// 각 재료표별 정리, 생성
	if(m_nMakeFlag & PAINTMAKE_SANG)
	{
		OnProgress(0,90);
		OnProgressText(1, "상세 집계표");
		MakeListSang();
	}
	if(m_nMakeFlag & PAINTMAKE_BUJE_SANG)
	{
		OnProgress(0,91);
		OnProgressText(1, "부재별 상세");
		MakeListBujeSang();
	}
	if(m_nMakeFlag & PAINTMAKE_BUJE_TOTAL)
	{
		OnProgress(0,92);
		OnProgressText(1, "부재별 총괄");
		MakeListBujeTotal();
	}
	if(m_nMakeFlag & PAINTMAKE_ASSEMBLY)
	{
		OnProgress(0,93);
		OnProgressText(1, "소조립별 상세");
		MakeListAssembly();
	}
	if(m_nMakeFlag & PAINTMAKE_ASSEMBLY_TOTAL)
	{
		OnProgress(0,94);
		OnProgressText(1, "소조립별 총괄");
		MakeListAssemblyTotal();
	}
	if(m_nMakeFlag & PAINTMAKE_GRAND_ASSEMBLY)
	{
		OnProgress(0,95);
		OnProgressText(1, "대조립별 상세");
		MakeListGrandAssembly();
	}
	if(m_nMakeFlag & PAINTMAKE_GRAND_ASSEMBLY_TOTAL)
	{
		OnProgress(0,96);
		OnProgressText(1, "대조립별 총괄");
		MakeListGrandAssemblyTotal();
	}
	if(m_nMakeFlag & PAINTMAKE_MAT_THICK)
	{
		OnProgress(0,97);
		OnProgressText(1, "두께별 상세집계표");
		MakeListMatAndThick();
	}

	if(m_nMakeFlag & PAINTMAKE_BOLT_SANG)
	{
		OnProgress(0,98);
		OnProgressText(1, "볼트 상세집계표");
		MakeListBoltSang();
	}
	if(m_nMakeFlag & PAINTMAKE_BOLT_TOTAL)
	{
		OnProgress(0,99);
		OnProgressText(1, "볼트 총괄집계표");
		MakeListBoltTotal();
	}
	if(m_nMakeFlag & PAINTMAKE_SUMMARY)
	{
		OnProgress(0,99);
		OnProgressText(1, "총괄집계표");
	}
	
	OnProgress(0,100);
	OnProgress(4);
	return nErr;
}


void CPaintCreator::SetTStandard(CTStandard *pTStandard)
{
	m_pTStandard = pTStandard;
}

CTStandard* CPaintCreator::GetTStandard()
{
	return m_pTStandard;
}

void CPaintCreator::SetWorkValue(long nWorkValue,const CString sPath,DIMTYPE nDimType,long nMakeFlag)
{
	m_nWorkValue = nWorkValue;	// 0:생성, 1:액셀파일로드
	m_sPath		 = sPath;
	m_nDimType   = nDimType;
	m_nMakeFlag  = nMakeFlag;
}

void CPaintCreator::MakeListSang()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = 0;
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListSang,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListSang.AddTail(pRec);
	}
}


void CPaintCreator::MakeListBujeSang()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x0020|0x0040|0x0080|0x0100)); // 종류,제원
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareBujeSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListBujeSang,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListBujeSang.AddTail(pRec);
	}
}

void CPaintCreator::MakeListBujeTotal()
{
}

void CPaintCreator::MakeListAssembly()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000400|0x010000|0x001000|0x000200
		                                 |0x000040|0x000080|0x000100)); // 소조립마크,부재마크
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareAssembly);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListAssembly,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListAssembly.AddTail(pRec);
	}
}
void CPaintCreator::MakeListAssemblyTotal()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000400|0x010000)); // 소조립마크
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareAssemblyTotal);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListAssemblyTotal,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListAssemblyTotal.AddTail(pRec);
	}
}

void CPaintCreator::MakeListGrandAssembly()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000400|0x010000|0x001000|0x000200
		                                 |0x002000|0x004000//대,소조립마크,부재마크
		                                 |0x000040|0x000080|0x000100)); 
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareGrandAssembly);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListGrandAssembly,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListGrandAssembly.AddTail(pRec);
	}
}

void CPaintCreator::MakeListGrandAssemblyTotal()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x002000|0x004000)); // 대조립마크
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareGrandAssemblyTotal);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListGrandAssemblyTotal,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListGrandAssemblyTotal.AddTail(pRec);
	}
}

void CPaintCreator::MakeListMatAndThick()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x000200|0x001000));	// 부재마크
//										  0x010000|0x000400|	// 소조립마크
//										  0x020000|0x040000|	// 중조립마크
//		                                  0x002000|0x004000));	// 대조립마크
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareMatAndThick);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListMatAndThick,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListMatAndThick.AddTail(pRec);
	}
}

void CPaintCreator::MakeListBoltSang()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x002000|0x004000|0x000200|0x001000| // 대조립,부재마크
		                                   0x80|0x100|0x008000));// 재료,두께,길이
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(!m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareBoltSang);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListBoltSang,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListBoltSang.AddTail(pRec);
	}
}

void CPaintCreator::MakeListBoltTotal()
{
	CPtrList TmpList; // 정렬전 임시 리스트

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x80|0x100|0x008000));// 재료,두께,길이
	while(pos)
	{
		CPaintRecord *pRec = (CPaintRecord*)m_ListStandard.GetNext(pos);
		if(!m_pDB->IsBolt(pRec->m_nMarkBuje)) continue;

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CPaintRecord(pRec));

		if(m_bAbort == TRUE) return;	//사용자 취소 
	}

	// SORT
	ShellSortRecord(&TmpList, &CPaintRecord::CompareBoltTotal);
	if(m_bAbort == TRUE) return;	// 사용자 취소

	// 이전 리스트 삭제
	AhPtrListDelete(m_ListBoltTotal,(CPaintRecord*)0);

	// 리스트 작성
	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CPaintRecord *pRec = (CPaintRecord*)TmpList.GetNext(pos);
		m_ListBoltTotal.AddTail(pRec);
	}
}

void CPaintCreator::ShellSortRecord(CPtrList* pList,int (CPaintRecord::*fcmp)(const CPaintRecord*) const) const
{
	CPtrArray  Arr;
	POSITION pos = pList->GetHeadPosition();
	while( pos ) Arr.Add( pList->GetNext(pos) );

	long nSize = Arr.GetSize();

	long i, j, k, h;
	CPaintRecord Rec;
	for(h = 1; h < nSize; h = 3*h+1);

	for(h /= 3; h > 0; h /=3)
		for(i = 0; i < h; i++)
			for(j = i+h; j < nSize; j += h)
			{
				Rec = *(CPaintRecord*)Arr[j];
				k = j;
				while(k > h - 1 && (((CPaintRecord*)Arr[k-h])->*fcmp)(&Rec) > 0)
				{
					*(CPaintRecord*)Arr[k] = *(CPaintRecord*)Arr[k-h];
					k -= h;
				}
				*(CPaintRecord*)Arr[k] = Rec;
			}

	Arr.RemoveAll();
}
  
void CPaintCreator::AddPaintXLFileLine(const CStringArray& sArr)
{
	if(sArr[0].IsEmpty()) return;

	long nGirder = atoi(sArr[0].Right(sArr[0].GetLength()-1)) - m_pDB->GetNumberStartGirder();
	long nSplice = atoi(sArr[1].Right(sArr[1].GetLength()-1)) - m_pDB->GetNumberStartSplice();

	CPaintRecord * pRec = AddPaintRecord();

	// 거더번호,현장이음번호,종류번호
	pRec->m_nGirder	= nGirder - 1;
	pRec->m_nSplice	= nSplice - 1;
	pRec->SetStrEleName(sArr[2]);
	pRec->SetStrSubEleName(sArr[3]);
	// 기본단면,단면마크,소조립마크,부재마크
	long nCol = 4;
	pRec->m_sMarkSectionBasic	= sArr[nCol++];		
	pRec->m_sMarkSection		= sArr[nCol++];
	pRec->m_sMarkAssem			= sArr[nCol++];
	pRec->m_sMarkBuje			= sArr[nCol];
	// 두께,너비,길이,수량
	nCol = 8;
	pRec->m_vJewon.y	= atof(sArr[nCol++]);
	pRec->m_vJewon.x	= atof(sArr[nCol++]);
	pRec->m_vJewon.z	= atof(sArr[nCol++]);
	pRec->m_nQty		= atoi(sArr[nCol]);

	// 도장
	nCol = 12;
	for(long i=0; i<PAINT_ITEM_MAX; i++)
		pRec->m_dPaintArea[i] = frM2(atof(sArr[nCol+i]));

	//접속부재종류,접속부재부재명,접속부재두께,참조도면,사용용도
	nCol = 12+PAINT_ITEM_MAX;
	pRec->m_sRefDWG			= sArr[nCol++];
	pRec->m_sUsage			= sArr[nCol];
	// 대조립마크,중조립마크(String)
	nCol = 14+PAINT_ITEM_MAX;
	pRec->m_sMarkGrandAssem	= sArr[nCol++];
	pRec->m_sMarkMidAssem	= sArr[nCol];
	// 부재번호,조립번호,중조립번호,대조립번호,기본단면번호,단면번호(long)
	nCol = 16+PAINT_ITEM_MAX;
	pRec->m_nMarkBuje			= atoi(sArr[nCol++]);
	pRec->m_nMarkAssem			= atoi(sArr[nCol++]);
	pRec->m_nMarkMidAssem		= atoi(sArr[nCol++]);
	pRec->m_nMarkGrandAssem		= atoi(sArr[nCol++]);
	pRec->m_nMarkSectionBasic	= atoi(sArr[nCol++]);
	pRec->m_nMarkSection		= atoi(sArr[nCol]);
	// 종류번호,부재번호(접속),종류번호(접속)
	nCol = 22+PAINT_ITEM_MAX;
	pRec->m_nSectionNumber		= atoi(sArr[nCol++]);
}

UINT CPaintCreator::MakeToLoadFileXL()
{
	CXLControl	xl;
	if(xl.OpenXL(m_sPath) == FALSE)	return TRUE;

	CStringArray sArrReadXL;
	CString sLineData = _T("");
	CString sProcess  = _T("");
	long nRow	= 1;
	long nCol	= 0;
	long nColsu = 0;

	// 타이틀을 읽어서 COLUMN수 결정
	for(nCol = 0; nCol < MAX_PAINT_COLUMN; nCol++)
	{
		sLineData = xl.GetXL(0, nCol);
		if(!sLineData.IsEmpty()) nColsu++;
	}

	// DATA READING : 연속두줄이 공백이 아닐 때까지....
	while(1)
	{
		nCol = 0;
		if(xl.GetXL(nRow,nCol).IsEmpty() && xl.GetXL(nRow+1,nCol).IsEmpty()) 
			break;

		sArrReadXL.RemoveAll();
		for(nCol = 0; nCol < nColsu; nCol++)
		{
			sLineData = xl.GetXL(nRow, nCol);
			sLineData.TrimLeft();
			sLineData.TrimRight();
			sArrReadXL.Add(sLineData);
		}
		//
		AddPaintXLFileLine(sArrReadXL);
		sProcess.Format("Excel 파일 읽는중 [%ld열] : %s %s - %s , %s",nRow,sArrReadXL[0]
			             ,sArrReadXL[1],sArrReadXL[2],sArrReadXL[3]);
		OnProgressText(1, sProcess);
		if(m_bAbort == TRUE) 
		{
			xl.QuitXL();
			return TRUE;
		}
		nRow++;
	}

	xl.QuitXL();
	return FALSE;
}

void CPaintCreator::SetTotalPaintStandardList()
{

}




void CPaintCreator::ClearAll()
{
	AhPtrListDelete(m_ListStandard,(CPaintRecord*)0);
	AhPtrListDelete(m_ListSang,(CPaintRecord*)0);
	AhPtrListDelete(m_ListBujeSang,(CPaintRecord*)0);
	AhPtrListDelete(m_ListBujeTotal,(CPaintRecord*)0);
	AhPtrListDelete(m_ListAssembly,(CPaintRecord*)0);
	AhPtrListDelete(m_ListAssemblyTotal,(CPaintRecord*)0);
	AhPtrListDelete(m_ListGrandAssembly,(CPaintRecord*)0);
	AhPtrListDelete(m_ListGrandAssemblyTotal,(CPaintRecord*)0);
	AhPtrListDelete(m_ListMatAndThick,(CPaintRecord*)0);
	AhPtrListDelete(m_ListBoltSang,(CPaintRecord*)0);
	AhPtrListDelete(m_ListBoltTotal,(CPaintRecord*)0);
}


BOOL CPaintCreator::SameAndIncludeRecord(CPtrList* pList, const CPaintRecord* pRec,DWORD exceptionFlag/*=0*/)
{
	BOOL bSameAndInclude = FALSE;
	POSITION pos = pList->GetHeadPosition();
	while( pos )
	{
		CPaintRecord * pSouRec = (CPaintRecord *)pList->GetNext(pos);
		if( pRec->IsSameRecord( pSouRec,exceptionFlag ) )
	 	{
			pSouRec->m_nQty += pRec->m_nQty;
			for(long i=0; i<PAINT_ITEM_MAX; i++)
				pSouRec->m_dPaintArea[i] += pRec->m_dPaintArea[i];
			pSouRec->m_dTWeight += pRec->m_dTWeight;
			bSameAndInclude = TRUE;
			break;
		}
	}
	return bSameAndInclude;
}

void CPaintCreator::AddSpliceUpper(CPlateBasicIndex *pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateSplice    *pSp	= pBx->GetSplice();
	CPlateGirderApp *pGir	= pBx->GetGirder();
	CPlateBridgeApp *pDB	= (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	CGangjeBase pGBase(m_pDB);
 	
	long nMarkBuje = SP_U_U;

	CPaintRecord *pRec = AddPaintRecord();
	SetPaintRecordSplice(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->SetStrEleName("상부 현장이음");
	pRec->SetStrSubEleName("상면 이음판");
	// PAINT SETTING
	double W = pRec->m_vJewon.x;
	double T = pRec->m_vJewon.y;
	double L = pRec->m_vJewon.z;
	//W*L*2 + (W + L) * T * 2; // 연결판도장
	pRec->m_dPaintArea[SPLICE_FACT] = W*L*2 + (W+L)*T*2;

	// 상부-하면 이음판
	for (long nRow = 0; nRow < 2; nRow++)
	{
		pRec = AddPaintRecord(pRec);
		nMarkBuje = SP_U_L;
		SetJewonPaintRecord(pRec,pBx,nMarkBuje,nRow);
		pRec->SetStrSubEleName("하면 이음판");
		// PAINT SETTING
		W = pRec->m_vJewon.x;
		T = pRec->m_vJewon.y;
		L = pRec->m_vJewon.z;
		pRec->m_dPaintArea[SPLICE_FACT] = W*L*2 + (W+L)*T*2;
		if (nRow == 0 || 2) 
		{
			pRec->m_dPaintArea[OUTSIDE_FIELD]	   = -W*L;
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] =  W*L + (W+L)*T*2;
		}
	}

	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_UPPERFLANGE, bNextFiller))
	{
		long nQty = pOpt->GetDesignHeigtBaseUpper() ? 1 : 2;

		pRec = AddPaintRecord(pRec);
		nMarkBuje = SP_U_FILL;
		SetJewonPaintRecord(pRec,pBx,nMarkBuje);
		pRec->SetStrSubEleName("채움판");
		// PAINT SETTING
		W = pRec->m_vJewon.x;
		T = pRec->m_vJewon.y;
		L = pRec->m_vJewon.z;
		pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] = (W+L)*2*T;
		pRec->m_nQty = nQty;
	}

	// 볼트
	nMarkBuje = SP_U_BOLT;
	pRec = AddPaintRecord(pRec);
	SetPaintRecordSplice(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->SetStrSubEleName("볼트-너트");
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("H.T.BOLT");
	long nSpsu = 1;
	pRec->m_nQty = 0;
	for(long n=0; n<nSpsu; n++)
		pRec->m_nQty += (long)( (pSp->m_uE+1)*(pSp->m_uB+1)*4 );
//		pRec->m_nQty += (long)( (pSp->m_uE+1)*(pSp->m_uB+1)*2 );
	if(pBx->GetSplice()->m_uG>0)
		pRec->m_nQty += (long)( (pSp->m_uB+1)*2*2 );
	pRec->m_dTWeight = pRec->m_dUWeight*pRec->m_nQty;

}

void CPaintCreator::AddSpliceLower(CPlateBasicIndex *pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateSplice    *pSp	= pBx->GetSplice();
	CPlateGirderApp *pGir	= pBx->GetGirder();
	CPlateBridgeApp *pDB	= (CPlateBridgeApp*)pGir->GetBridgeApp();
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	CGangjeBase pGBase(m_pDB);
 	
	long nMarkBuje = SP_L_L;

	CPaintRecord *pRec = AddPaintRecord();
	SetPaintRecordSplice(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->SetStrEleName("하부 현장이음");
	pRec->SetStrSubEleName("하면 이음판");
	// PAINT SETTING
	double W = pRec->m_vJewon.x;
	double T = pRec->m_vJewon.y;
	double L = pRec->m_vJewon.z;
	pRec->m_dPaintArea[OUTSIDE_FIELD]	   = -W*L;
	pRec->m_dPaintArea[SPLICE_FACT]		   =  W*L*2 + (W+L)*T*2;
	pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] =  W*L   + (W+L)*T*2;

	// 하부 - 상면이음판
	nMarkBuje = SP_L_U;
	for (long nRow = 0; nRow < 2; nRow++)
	{
		pRec = AddPaintRecord(pRec);
		SetJewonPaintRecord(pRec,pBx,nMarkBuje,nRow);
		// PAINT SETTING
		W = pRec->m_vJewon.x;
		T = pRec->m_vJewon.y;
		L = pRec->m_vJewon.z;
		pRec->SetStrSubEleName("상면 이음판");
		
		pRec->m_dPaintArea[OUTSIDE_FIELD]	   = -W*L;
		pRec->m_dPaintArea[SPLICE_FACT]		   =  W*L*2 + (W+L)*T*2;
		pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] =  W*L   + (W+L)*T*2;
	}

	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_LOWERFLANGE, bNextFiller))
	{
		long nQty = !pOpt->GetDesignHeigtBaseLower() ? 1 : 2;

		pRec = AddPaintRecord(pRec);
		nMarkBuje = SP_L_FILL;
		SetJewonPaintRecord(pRec,pBx,nMarkBuje);
		pRec->SetStrSubEleName("채움판");
		// PAINT SETTING
		W = pRec->m_vJewon.x;
		T = pRec->m_vJewon.y;
		L = pRec->m_vJewon.z;
		pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] = (W+L)*2*T;
		pRec->m_nQty = nQty;
	}

	// 볼트
	nMarkBuje = SP_L_BOLT;
	pRec = AddPaintRecord(pRec);
	SetPaintRecordSplice(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->SetStrSubEleName("볼트-너트");
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("H.T.BOLT");
	long nSpsu = 1;
	pRec->m_nQty = 0;
	for(long n=0; n<nSpsu; n++)
		pRec->m_nQty += (long)( (pSp->m_dE+1)*(pSp->m_dB+1)*4 );
//		pRec->m_nQty += (long)( (pSp->m_dE+1)*(pSp->m_dB+1)*2 );
	if(pBx->GetSplice()->m_dG>0)
		pRec->m_nQty += (long)( (pSp->m_dB+1)*2*2 );
	pRec->m_dTWeight = pRec->m_dUWeight*pRec->m_nQty;
}

void CPaintCreator::AddSpliceWeb(CPlateBasicIndex *pBx)
{
	if(!pBx->IsState(BX_SPLICE)) return;
	if(!pBx->GetSplice()->IsTrue()) return;
	if(!pBx->GetSplice()->m_bSpliceType) return;

	CPlateSplice    *pSp   = pBx->GetSplice();
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CGangjeBase pGBase(m_pDB);
 	
	long nMarkBuje  = SP_W_SH;
	long nPaintType = BOLT_FIELD_OUTSIDE;

	CPaintRecord *pRec = AddPaintRecord();
	SetPaintRecordSplice(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->SetStrEleName("복부판 현장이음");
	pRec->SetStrSubEleName("전단 이음판");
	// PAINT SETTING
	double W = pRec->m_vJewon.x;
	double T = pRec->m_vJewon.y;
	double L = pRec->m_vJewon.z;

	if(pSp->m_nWebType!=SP_WEB_FLMO)	// 통판형,분리형
	{
		pRec->m_dPaintArea[SPLICE_FACT] = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)*2	+ (W + L) * T * 2;
		pRec->m_dPaintArea[nPaintType]  = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)		+ (W + L) * T * 2;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = -pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType);
	}
	else	// 일체형
	{
		double dFL = (pSp->m_sA + pSp->m_sB*pSp->m_sC)*2 + pSp->m_sC1;
		double dMO = (pSp->m_mA + pSp->m_mB*pSp->m_mC)*2 + pSp->m_mC1;
		
		pRec->m_dPaintArea[SPLICE_FACT] = (pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)-(dMO-dFL)*pSp->m_sW)*2+(dMO*2-dFL+W)*2*T;
		pRec->m_dPaintArea[nPaintType]  = (pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)-(dMO-dFL)*pSp->m_sW)  +(dMO*2-dFL+W)*2*T;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = -(pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)-(dMO-dFL)*pSp->m_sW);
	}
	
	if(pSp->m_nWebType==SP_WEB_MO)	// 모멘트형
	{
		nMarkBuje  = SP_W_MO;
		for (long nRow = 0; nRow < 2; nRow++)
		{
			CVector vJewon;
			vJewon.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow),0);
			vJewon.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow),0);
			vJewon.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow),0);

			if(vJewon.x == 0 || vJewon.y == 0 || vJewon.z == 0) continue;

			pRec = AddPaintRecord(pRec);
			SetJewonPaintRecord(pRec,pBx,nMarkBuje,nRow);
			pRec->SetStrSubEleName("모멘트 이음판");
			// PAINT SETTING
			double W = pRec->m_vJewon.x;
			double T = pRec->m_vJewon.y;
			double L = pRec->m_vJewon.z;
			pRec->m_dPaintArea[SPLICE_FACT] = pGBase.GetAreaBuje(nMarkBuje, pBx, nRow,0,m_nDimType)*2 
											+ (W + L) * T * 2;
			pRec->m_dPaintArea[nPaintType]  = pGBase.GetAreaBuje(nMarkBuje, pBx, nRow,0,m_nDimType) 
											+ (W + L) * T * 2;
			
			pRec->m_dPaintArea[OUTSIDE_FIELD] = -pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType);
		}
	}

	nMarkBuje = SP_W_FILL;
	BOOL bNextFiller = TRUE;
	if(pBx->IsFiller(BX_WEB, bNextFiller))
	{
		pRec = AddPaintRecord(pRec);
		SetJewonPaintRecord(pRec,pBx,nMarkBuje,1);
		pRec->SetStrSubEleName("채움판");
		// PAINT SETTING
		double W = pRec->m_vJewon.x;
		double T = pRec->m_vJewon.y;
		double L = pRec->m_vJewon.z;
		pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] = (W+L)*2*T;
		// 모멘트 일체형..모멘트-전단이음 연결부의 도장분량을 더해줌
		if(pSp->m_nWebType==SP_WEB_FLMO)
		{
			double dDis = pGir->GetWidthJewon(SP_W_MO, pBx)-pGir->GetWidthJewon(SP_W_SH, pBx);
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] += dDis*T;
		}

		// 모멘트 이음판
		if(pSp->m_nWebType==SP_WEB_MO)
		{
			for (long nRow = 0; nRow < 3; nRow++)
			{
				if(nRow==1) continue;
				pRec = AddPaintRecord(pRec);
				SetJewonPaintRecord(pRec,pBx,nMarkBuje,nRow);
				pRec->SetStrSubEleName("채움판");
				// PAINT SETTING
				W = pRec->m_vJewon.x;
				T = pRec->m_vJewon.y;
				L = pRec->m_vJewon.z;
				pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE] = (W+L)*2*T;
			}
		}
	}
}

void CPaintCreator::AddFlange(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	__int64 BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_UPPERFLANGE | BX_SPLICE;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd,TRUE,FALSE);
	
	while(pBx)
	{
		AddUpperFlange(pBx);		
		pBx = Finder.GetBxNext();
	}
	BxFlag = BX_STT_GIRDER | BX_END_GIRDER | BX_LOWERFLANGE | BX_SPLICE;
	pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd,TRUE,FALSE);
	while(pBx)
	{
		AddBottomFlange(pBx);
		pBx = Finder.GetBxNext();
	}
}


void CPaintCreator::AddWeb(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	
	__int64 BxFlag = BX_STT_GIRDER|BX_END_GIRDER|BX_WEB|BX_SPLICE;
	CPlateBasicIndex *pBx = Finder.GetBxFirst(BxFlag, pBxStt->GetBxLeft(), pBxEnd->GetBxLeft(),TRUE,FALSE);
	while(pBx)
	{
		AddWebPlate(pBx);
		pBx = Finder.GetBxNext();
	}	
}

void CPaintCreator::AddUpperFlange(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();

	CPaintRecord	*pRec = AddPaintRecord();
	SetPaintRecordFact(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,G_F_U);
	pRec->SetStrSubEleName("상판");

	double WA  = pGir->GetWidthOnStation(pBx->GetStation(),TRUE);
	double WS  = (WA  - pBx->GetFactChain(G_W)->m_dFactChainThick) / 2.0;
	double LA = pGir->GetLengthJewon(G_F_U,pBx);
	double TU = pBx->GetFactChain(G_F_U)->m_dFactChainThick;

	pRec->m_dPaintArea[OUTSIDE_FACT]	= 0;
	pRec->m_dPaintArea[OUTSIDE_FIELD]	= 0;
	pRec->m_dPaintArea[PAVE_FACT]		= 0;

	if(pDB->IsTUGir()) return;

	// PAINT SETTING
	CGlobarOption	*pOpt	= m_pDataManage->GetGlobalOption();
	// APLATE-701
	if(pOpt->GetDesignHeigtBaseUpper() == 0)
	{
		pRec->m_dPaintArea[OUTSIDE_FACT]  = (WS + TU) * LA * 2;	// 외부도장 (공장)
		pRec->m_dPaintArea[OUTSIDE_FIELD] = (WS + TU) * LA * 2;	// 외부도장 (현장)
		pRec->m_dPaintArea[PAVE_FACT]	  = WA * LA;	// 외부 포장면
	}
	else
	{
		pRec->m_dPaintArea[OUTSIDE_FACT]  = WS * LA * 2;	// 외부도장 (공장)
		pRec->m_dPaintArea[OUTSIDE_FIELD] = WS * LA * 2;	// 외부도장 (현장)
		pRec->m_dPaintArea[PAVE_FACT]	  = (WA + TU*2) * LA;	// 외부 포장면
	}
}

void CPaintCreator::AddBottomFlange(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	
	CPaintRecord *pRec = AddPaintRecord();
	SetPaintRecordFact(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,G_F_L);
	pRec->SetStrSubEleName("하판");

	double WA  = pGir->GetWidthOnStation(pBx->GetStation(),FALSE);
	double WS  = (WA  - pBx->GetFactChain(G_W)->m_dFactChainThick) / 2.0;
	double LA = pGir->GetLengthJewon(G_F_L,pBx);
	double TL = pBx->GetFactChain(G_F_L)->m_dFactChainThick;

	// PAINT SETTING
	pRec->m_dPaintArea[OUTSIDE_FACT]  =
	pRec->m_dPaintArea[OUTSIDE_FIELD] = (WA+WS*2+TL*2)*LA;
}

void CPaintCreator::AddWebPlate(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	
	CPaintRecord *pRec = AddPaintRecord();
	SetPaintRecordFact(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,G_W);
	pRec->SetStrSubEleName("복부판");

	// PAINT SETTING
	CGangjeBase pGBase(m_pDB);
	pRec->m_dPaintArea[OUTSIDE_FACT]  = 
	pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(G_W, pBx,0,0,m_nDimType);
//	pRec->m_dPaintArea[INSIDE_FACT]   = pGBase.GetAreaBuje(G_W, pBx,0,0,m_nDimType);


}

void CPaintCreator::AddGitaBujeJackUp(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	long nMarkBuje  = bLeft ? GI_JA_L : GI_JA_R;
	long nCol       = pBx->IsState(BX_JACKUP) ? -1 : 1;
	
	CPaintRecord *pRec = AddPaintRecord();
	//SetPaintRecordFact(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje,0,nCol);
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	pRec->SetStrEleName("일반"/*"기타부재"*/);
	pRec->SetStrSubEleName("잭업보강재");
	pRec->SetStrSectionType("복부판");		
	pRec->SetStrProcessBig("GIRDER");
	pRec->SetStrProcessMid("JACK-UP보강");


	CJackupStiff* pJack = pBx->GetJackupStiff();

	double W = pJack->GetWidth(bLeft);
	double L = pJack->GetHeight(bLeft);
	double T = pJack->GetThick(bLeft);


	// PAINT SETTING
	pRec->m_dPaintArea[OUTSIDE_FACT] =
	pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2+T*L;
}

void CPaintCreator::AddVStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	__int64 BxFlag = BX_VSTIFF;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd,TRUE,FALSE);

	while(pBx)
	{
		CSection* pSec = pBx->GetSection();
		if(pSec)
		{
			AddVStiff(pBx, TRUE);//left
			AddVStiff(pBx, FALSE);//right
		}

		pBx = Finder.GetBxNext();
	}	
}

void CPaintCreator::AddVStiff(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	long nMarkBuje = V_ST;
	
	double W = pGir->GetWidthVStiff(pBx, bLeft);
	double T = pGir->GetThickVStiff(pBx, bLeft);
	double L = pGir->GetLengthVStiff(pBx, bLeft, DIM_BASIC);

	if(W==0 || T==0 || L==0)	return;

	CPaintRecord *pRec = AddPaintRecord();
	SetPaintRecordFact(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje,0,bLeft?0:1);
	pRec->SetStrSubEleName("수직보강재");
	pRec->SetStrProcessBig("GIRDER");
	pRec->SetStrProcessMid("V-STIFF");

 	// PAINT SETTING
	pRec->m_dPaintArea[OUTSIDE_FACT] =
	pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2+L*T;
	//pRec->m_dPaintArea[INSIDE_FACT] = pGBase.GetAreaBuje(nMarkBuje,pBx,row,nCol,m_nDimType)*BOTHSIDE;
}

void CPaintCreator::RemoveTailPaintRecord()
{
	if(m_ListStandard.GetCount() > 0)
	{
		delete (CPaintRecord*)m_ListStandard.GetTail();
		m_ListStandard.RemoveTail();
	}
}


void CPaintCreator::AddHStiff(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	__int64 BxFlag = BX_PLATEHSTIFF;
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BxFlag, pBxStt, pBxEnd,TRUE,FALSE);

	while(pBx)
	{
		CWebHStiff	*pWhs = pBx->GetWebHStiff();
		for (long n = 0; n < WEBHSTIFFSUMAX; n++) 
		{
			if (!pWhs->IsEmpty(n, TRUE))
				AddHStiffSub(pBx, n, TRUE);
			if (!pWhs->IsEmpty(n, FALSE))
				AddHStiffSub(pBx, n, FALSE);
		}
		pBx = Finder.GetBxNext();
	}	
}

void CPaintCreator::AddHStiffSub(CPlateBasicIndex *pBx, long row, BOOL bLeft)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	long nMarkBuje = bLeft ? G_H_L : G_H_R;

	CPaintRecord *pRec = AddPaintRecord();
	SetPaintRecordFact(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje, row);
	pRec->SetStrSubEleName("수평보강재");
	pRec->SetStrProcessBig("GIRDER");
	pRec->SetStrProcessMid("H-STIFF");


	CWebHStiff* pH = pBx->GetWebHStiff();
	double W = pH->m_dWidthHStiff[bLeft ? 0 : 1];// left
	double T = pH->m_dThickHStiff[bLeft ? 0 : 1];// left
	double L = pH->GetLength(row,bLeft,DIM_BASIC);


 	// PAINT SETTING
	pRec->m_dPaintArea[OUTSIDE_FACT] =
	pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2+L*T;
	//pRec->m_dPaintArea[INSIDE_FACT] = pGBase.GetAreaBuje(nMarkBuje,pBx,row,nCol,m_nDimType)*BOTHSIDE;
}
	
//가로보 
void CPaintCreator::AddCrossBeam(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING
		                    , ((CPlateBasicIndex*)pBxStt), ((CPlateBasicIndex*)pBxEnd));
	
	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
		{
			CPlateCrossBeam  *pC = pBx->GetSection()->GetCrossBeam();
			if(pC==NULL) break;


			AddCrossBeamMain(pBx);

			if(pC->m_nLGirSpType)
				AddCrossBeamBrk(pBx,TRUE);
			if(pC->m_nRGirSpType)
				AddCrossBeamBrk(pBx,FALSE);
			if(pC->m_cGasset_CR)
			{
				AddCrossBeamBrkGusset(pBx,TRUE);
				AddCrossBeamBrkGusset(pBx,FALSE);
			}			

			AddCrossBeamSplice(pBx,TRUE);
			AddCrossBeamSplice(pBx,FALSE);
		}
		
		pBx = Finder.GetBxNext();
	}
}


void CPaintCreator::SetPaintAreaVStiff(CPaintRecord* pRec)
{
	//(W*H*2+H*T)
	double W = pRec->m_vJewon.x;
	double T = pRec->m_vJewon.y;
	double H = pRec->m_vJewon.z;
	pRec->m_dPaintArea[OUTSIDE_FACT] = 
	pRec->m_dPaintArea[OUTSIDE_FIELD] = (W*H*2+H*T) * pRec->m_nQty; // 외부도장 (현장)
}

// 가로보 위치(BX_CROSSBEAM_VBRACING)에 있는 거더부착 수직보강재의 물량계산
void CPaintCreator::AddCrossBeamGirderVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp *pGir	= pBxStt->GetGirder();
	CPlateGirderApp *pGirL	= pGir->GetNumberGirder() != 0 ? pGir->m_pLeft : NULL;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	while(pBx)
	{
		if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM) && !pBx->IsJijum())	// 지점일때는 지점부보강재에서
		{
			CPlateCrossBeam *pC = pBx->GetSection()->GetCrossBeam();
			CVStiff *pVStiff = pC ? pC->m_pVStiff : NULL;
			if(pVStiff==NULL) continue;

			CPlateBasicIndex *pBxL	= pGirL ? pGir->GetBxMatchByCrossBeam(pBx, pGirL) : NULL;
			CSection		 *pSecL	= pBxL  ? pBxL->GetSection() : NULL;
			CPlateCrossBeam	 *pCL	= (pSecL && pSecL->IsenDetType(SECDET_CROSSBEAM)) ? pSecL->GetCrossBeam() : NULL;

			long nMarkBuje =V_ST;

			CPaintRecord *pRec;
			CPaintRecord RecMain;
			SetRecStrEleNameCrossBeam(&RecMain,pBx);
			if(pVStiff->m_dWidth[0] > 0)
			{
				if(pVStiff->GetVStiffHeight(TRUE) > 0)	// 거더 좌측
				{
					pRec = AddPaintRecord(&RecMain);
					pRec->SetStrEleName("일반");
					pRec->SetStrSubEleName("수직보강재");
					pRec->SetStrProcessMid("V-STIFF");
					pRec->m_nQty = 1;
				
					SetJewonPaintRecord(pRec, pBx, nMarkBuje, 0, 0);

					if(pCL && pCL->GetType() == CR_TYPE_MANUFACTURE)
					{
						pRec->m_vJewon.z -= (pCL->m_H_CR + pCL->m_T1_CR);
						if(pCL->m_dRH1 > 0)
							pRec->m_vJewon.z -= pCL->m_T2_CR;
					}
					SetPaintAreaVStiff(pRec);
				}
			}
			if(pVStiff->m_dWidth[1] > 0)
			{
				if(pVStiff->GetVStiffHeight(FALSE) > 0)	// 거더 우측
				{
					pRec = AddPaintRecord(&RecMain);
					pRec->SetStrEleName("일반");
					pRec->SetStrSubEleName("수직보강재");
					pRec->SetStrProcessMid("V-STIFF");
					pRec->m_nQty = 1;

					SetJewonPaintRecord(pRec, pBx, nMarkBuje, 0, 1);

					if(pC->GetType() == CR_TYPE_MANUFACTURE)
					{
						pRec->m_vJewon.z -= (pC->m_H_CR + pC->m_T1_CR);
						if(pC->m_dLH1 > 0)
							pRec->m_vJewon.z -= pC->m_T2_CR;
					}
					SetPaintAreaVStiff(pRec);
				}
			}		
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}


void CPaintCreator::AddCrossBeamMain(CPlateBasicIndex *pBx)
{

	if(pBx->GetSection()==NULL) return;

	CPlateGirderApp  *pGir = pBx->GetGirder();
	CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	CGlobarOption    *pGlopt= m_pDB->m_pDataManage->GetGlobalOption();

	if(pBxR == NULL) return;
	
	CPlateCrossBeam  *pC   = pBx->GetSection()->GetCrossBeam();
	
	if(pC==NULL) return;

	double dDistWeb  = pGir->GetDisFrontPlateWebToWeb(pBx);
	double dWVstiffL = pC->m_pVStiff->m_dWidth[1];
	double dWVstiffR = pBxR->GetSection()->GetVStiff()->m_dWidth[0];

	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR*2)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffL-pC->m_dbL3)<1.00)  return;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffR-pC->m_dbR3)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-dWVstiffL-dWVstiffR-pC->m_dbL3-pC->m_dbR3)<1.00)  return;
	}

	if(pC->GetType()==CR_TYPE_HSTEEL)
	{
		AddCrossBeamHSteel(pBx);
	}
	else
	{
		AddCrossBeamWeb(pBx);
		AddCrossBeamFlange(pBx, TRUE);
		AddCrossBeamFlange(pBx, FALSE);
	}

	// 수직보강재
	AddCrossBeamVStiff(pBx);
	// 수평보강재
	AddCrossBeamHStiff(pBx);
	// 슬래브 앵커
	AddCrossBeamAnchor(pBx);
}

void CPaintCreator::AddCrossBeamHSteel(CPlateBasicIndex *pBx)
{
	CPlateGirderApp		*pGir		= pBx->GetGirder();
	CPlateBridgeApp		*pDB		= (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPaintRecord		*pRec		= AddPaintRecord();
	CGlobarOption		*pGloOpt	= m_pDataManage->GetGlobalOption(); 
	CPlateCrossBeam		*pC			= pBx->GetSection()->GetCrossBeam();
	if(!pC)	return;
	CSteelMaterialData	*pMatData	= pC->GetSteelMaterialData();
	CGangjeBase		pGBase(m_pDB);

	CString	szSubName	= _T("");
	long	nRoundUnit	= pGloOpt->GetPointValueOfUnitGangJae();
	long	nMarkBuje	= CP_STEEL;
	long	nRoundTot	= pGloOpt->GetPointValueOfGangJae();
	double	dResult		= 0;

	double	dH		= pMatData->m_dH;
	double	dB		= pMatData->m_dB;
	double	dT1		= pMatData->m_dT1;
	double	dT2		= pMatData->m_dT2;
	double	dL		= Round(pGir->GetLengthJewon(nMarkBuje, pBx),0);

	switch(pMatData->m_nType)
	{
	case CSteelMaterialData::LSTEEL :
		szSubName	= _T("등변 ㄱ 형강");
		dResult		= ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));
		break;
	case CSteelMaterialData::LVALSTEEL :
		szSubName	= _T("부등변 ㄱ 형강");
		dResult		= ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));
		break;
	case CSteelMaterialData::CSTEEL :
		szSubName	= _T("ㄷ 형강");
		dResult		= ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));
		break;
	case CSteelMaterialData::CTSTEEL :
		szSubName	= _T("CT 형강");
		dResult		= ((dB+dH)*dL*2 + (dB*dT2+dH*dT1)*2 - (dT1*dT2*2));
		break;
	case CSteelMaterialData::ISTEEL :
		szSubName	= _T("I 형강");
		dResult		= ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));
		break;
	case CSteelMaterialData::HSTEEL :
		szSubName	= _T("H 형강");
		dResult		= ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));
		break;
	}

	SetRecStrEleNameCrossBeam(pRec, pBx);
	pRec->SetStrSubEleName(szSubName);
	pRec->m_nKindName		= MATERIAL_KIND_BEAM;
	pRec->m_nGirder			= pBx->GetNumberGirder();
	pRec->m_nSplice			= pGir->GetNumberSpliceByBx(pBx);
	pRec->m_nMarkBuje		= nMarkBuje;
	pRec->m_sMaterial		= pDB->GetMaterial(nMarkBuje,pBx);
	pRec->m_nQty			= 1;
	pRec->m_vJewon.x		= dH;
	pRec->m_vJewonAngle.x	= dB;
	pRec->m_vJewon.y		= dT1;
	pRec->m_vJewonAngle.y	= dT2;
	pRec->m_vJewon.z		= dL;
	pRec->m_dTWeight		= Round(pRec->m_dUWeight * toM(pRec->m_vJewon.z), nRoundTot);

 	// PAINT SETTING
	pRec->m_dPaintArea[OUTSIDE_FACT] =  dResult;
	pRec->m_dPaintArea[OUTSIDE_FIELD] = dResult;
}

void CPaintCreator::AddCrossBeamWeb(CPlateBasicIndex *pBx)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CGangjeBase pGBase(m_pDB);

	long nMarkBuje = CP_W;
	CPaintRecord *pRec = AddPaintRecord();
	SetRecStrEleNameCrossBeam(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
 	pRec->SetStrSubEleName("복부판");
	pRec->SetStrProcessMid("WEB");
 
 	// PAINT SETTING
	pRec->m_dPaintArea[OUTSIDE_FACT] =  pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType) * 2; // 외부도장 (공장)
	pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType) * 2; // 외부도장 (현장)
}

void CPaintCreator::AddCrossBeamFlange(CPlateBasicIndex *pBx, BOOL bUpper)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CGangjeBase pGBase(m_pDB);

	long nMarkBuje = bUpper ? CP_F_U : CP_F_L;
	CPaintRecord *pRec = AddPaintRecord();
	SetRecStrEleNameCrossBeam(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->SetStrSubEleName(bUpper ? "상판" : "하판");
	pRec->SetStrProcessMid("FLG");

	double T = pRec->m_vJewon.y;
// 	double L = pGir->GetLengthCrossTaper(pBx, DIM_BASIC)
// 		+ (pRec->m_vJewon.z - pGir->GetLengthCrossTaper(pBx, DIM_BASIC));
	double L = pRec->m_vJewon.z;

	// PAINT SETTING
	if (bUpper) 
	{
		pRec->m_dPaintArea[OUTSIDE_FACT]  = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType);
		pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType);
		pRec->m_dPaintArea[PAVE_FACT]    = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)
											+ L * T *2;
	}
	else 
	{
		pRec->m_dPaintArea[OUTSIDE_FACT]  = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)*2 + L*T*2;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje, pBx,0,0,m_nDimType)*2 + L*T*2;
	}
}

void CPaintCreator::AddCrossBeamHStiff(CPlateBasicIndex *pBx)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CGangjeBase pGBase(m_pDB);

	long nMarkBuje = CP_H;
 
	long HUpperSu = pC->m_nHStiffUppersu_CR;
	long HLowerSu = pC->m_nHStiffLowersu_CR;
	long VStiffSu = pC->m_nVStiffsu_CR;
	
	CGlobarOption *pGlopt= m_pDB->m_pDataManage->GetGlobalOption();

	// 수평보강재가 있으면..
	if(pC->m_nHStiffUppersu_CR + pC->m_nHStiffLowersu_CR > 0)	
	{
		CDPoint Po[18];
		BOOL bIncludeGasset  = pGlopt->GetDesignCrossBeamHStiffAtBracket()==0 ? TRUE : FALSE;
		long nQty = pC->m_cVSide_CR==0 ? 2 : 1;	//양면수직보강재 이면 수평보강재는 2 그렇지 않으면 1
		long nV   = 0;
		BOOL bFront = pC->m_cVSide_CR==2 ? FALSE : TRUE;  
		if (bFront)
			nV = pGir->GetPointVStiffOnCrossBeamPrv(pBx, Po, FALSE, bIncludeGasset);
		else
			nV = pGir->GetPointVStiffOnCrossBeam(pBx, Po, FALSE, bIncludeGasset);

		long nH = pC->m_nHStiffUppersu_CR + pC->m_nHStiffLowersu_CR;
		long nColStt = bIncludeGasset&&pC->m_nLGirSpType  ? 0 : 1;
		long nColEnd = bIncludeGasset&&pC->m_nRGirSpType  ? nV-1 : nV-2;

		for(long nRow=0; nRow<nH; nRow++) 
		{
			for(long nCol=nColStt; nCol<nColEnd; nCol++) 
			{
				CPaintRecord *pRec = AddPaintRecord();
				SetRecStrEleNameCrossBeam(pRec,pBx);
				SetJewonPaintRecord(pRec,pBx,CP_H,nRow,nCol,TRUE);

				pRec->SetStrSubEleName("수평보강재");
				pRec->SetStrProcessMid("H-STIFF");
				pRec->m_nQty = nQty;
	 			// PAINT SETTING
				pRec->m_dPaintArea[OUTSIDE_FACT]  = pGBase.GetAreaBuje(CP_H,pBx,nRow,nCol,m_nDimType,TRUE)*BOTHSIDE;
				pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(CP_H,pBx,nRow,nCol,m_nDimType,TRUE)*BOTHSIDE;
			}
		}
	}
}

void CPaintCreator::AddCrossBeamVStiff(CPlateBasicIndex *pBx)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CGangjeBase pGBase(m_pDB);

	long nMarkBuje	= CP_V;
	long col		= 0;
	
	for (col = 0; col < pC->m_nVStiffsu_CR; col++) 
	{
		if(pC->m_cVSide_CR==2) break;
		CPaintRecord *pRec = AddPaintRecord();
		SetRecStrEleNameCrossBeam(pRec,pBx);
		SetJewonPaintRecord(pRec,pBx,nMarkBuje,0,col,TRUE);

		pRec->SetStrSubEleName("수직보강재");
		pRec->SetStrProcessMid("V-STIFF");
		pRec->m_dPaintArea[OUTSIDE_FACT]  = pGBase.GetAreaBuje(nMarkBuje,pBx,0,col,m_nDimType,TRUE)*BOTHSIDE;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje,pBx,0,col,m_nDimType,TRUE)*BOTHSIDE;
	}
	for (col = 0; col < pC->m_nVStiffsu_CR; col++) 
	{
		if(pC->m_cVSide_CR==1) break;
		CPaintRecord *pRec = AddPaintRecord();
		SetRecStrEleNameCrossBeam(pRec,pBx);
		SetJewonPaintRecord(pRec,pBx,nMarkBuje,1,col,TRUE);

		pRec->SetStrSubEleName("수직보강재");
		pRec->SetStrProcessMid("V-STIFF");
		pRec->m_dPaintArea[OUTSIDE_FACT]  = pGBase.GetAreaBuje(nMarkBuje,pBx,0,col,m_nDimType,FALSE)*BOTHSIDE;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje,pBx,0,col,m_nDimType,FALSE)*BOTHSIDE;
	}
}

void CPaintCreator::AddCrossBeamBrk(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CGangjeBase pGBase(m_pDB);

	long nRow = 0;
	long nCol = bLeft ? 0 : 1;	

	long nMarkBuje = CP_BR_W;
	CPaintRecord *pRec = AddPaintRecord();
	SetRecStrEleNameCrossBeam(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje, nRow, nCol);
	pRec->SetStrSubEleName("브라켓 복부판");
	pRec->SetStrProcessMid("WEB");
	pRec->m_dPaintArea[OUTSIDE_FACT]  = pGBase.GetAreaBuje(nMarkBuje,pBx,nRow,nCol,m_nDimType)*BOTHSIDE;
	pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje,pBx,nRow,nCol,m_nDimType)*BOTHSIDE;
	
	//
	nMarkBuje = CP_BR_F_U;
	pRec = AddPaintRecord(pRec);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje, nRow, nCol);
	pRec->SetStrSubEleName("브라켓 상판");
	pRec->SetStrProcessMid("T-FLG");
	pRec->m_dPaintArea[PAVE_FACT]    = pGBase.GetAreaBuje(nMarkBuje,pBx,0,0,m_nDimType)
		                              + pRec->m_vJewon.z*pRec->m_vJewon.y*2;
	pRec->m_dPaintArea[OUTSIDE_FACT]  = 
	pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje,pBx,0,0,m_nDimType);

	//
	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();
	if(!bLeft)
	{
		CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
		pBrGu = pBxR ? pBxR->GetHBracingGusset() : NULL;
	}

	if(!pBrGu || (bLeft&&!pBrGu->IsReal(1)) || (!bLeft&&!pBrGu->IsReal(0)))
	{
		nMarkBuje = CP_BR_F_L;
		pRec = AddPaintRecord(pRec);
		SetJewonPaintRecord(pRec,pBx,nMarkBuje, nRow, nCol);
		pRec->SetStrSubEleName("브라켓 하판");
		pRec->SetStrProcessMid("B-FLG");
		pRec->m_dPaintArea[OUTSIDE_FACT]  = 
		pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje, pBx, 0,0,m_nDimType)*BOTHSIDE
										  + pRec->m_vJewon.z*pRec->m_vJewon.y*2;
	}	
}

void CPaintCreator::AddCrossBeamBrkGusset(CPlateBasicIndex *pBx, BOOL bLeft)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();
	CGangjeBase pGBase(m_pDB);

	long nRow = 0;
	long nCol = bLeft ? 0 : 1;	

	long nMarkBuje = CP_BR_GA;
	CPaintRecord *pRec = AddPaintRecord();
	SetRecStrEleNameCrossBeam(pRec,pBx);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje, nRow, nCol);
	pRec->SetStrSubEleName("브라켓 보강재");
	pRec->SetStrProcessMid("V-보강");
	pRec->m_dPaintArea[OUTSIDE_FACT]  = 
	pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje,pBx,0,0,m_nDimType)*BOTHSIDE;

	//
	nMarkBuje = CP_BR_GA_FA;
	pRec = AddPaintRecord(pRec);
	SetJewonPaintRecord(pRec,pBx,nMarkBuje, nRow, nCol);
	pRec->SetStrSubEleName("브라켓 보강판");
	pRec->SetStrProcessMid("V-보강FLG");
	//pRec->m_dPaintArea[OUTSIDE_FACT]  = 
	//pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(nMarkBuje,pBx,0,0,m_nDimType)*BOTHSIDE
	//							      + (pRec->m_vJewon.z+pRec->m_vJewon.x)*pRec->m_vJewon.y*BOTHSIDE;

	double W = pRec->m_vJewon.x;
	double T = pRec->m_vJewon.y;
	double L = pRec->m_vJewon.z;
	pRec->m_dPaintArea[OUTSIDE_FACT]  = 
	pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2 + L*T*2;


}


void CPaintCreator::AddCrossBeamSplice(CPlateBasicIndex *pBx, BOOL bLeft)
{
	if(pBx->GetSection()==NULL) return;
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPlateBridgeApp *pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateBasicIndex *pBxR = pBx->GetBxMatchByCrossBeam(pGir->m_pRight);
	if(pBxR==NULL) return;
	CPlateCrossBeam* pC   = pBx->GetSection()->GetCrossBeam();
	if(pC==NULL) return;
	double dDistWeb  = pGir->GetDisFrontPlateWebToWeb(pBx);
	double dWVstiffL = pC->m_pVStiff->m_dWidth[1];
	double dWVstiffR = pBxR->GetSection()->GetVStiff()->m_dWidth[0];

	if(pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR*2)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffL-pC->m_dbL3)<1.00)  return;
	}
	else if(pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-pC->m_P_CR-dWVstiffR-pC->m_dbR3)<1.00)  return;
	}
	else if(!pC->m_nLGirSpType && !pC->m_nRGirSpType)
	{
		if((dDistWeb-dWVstiffL-dWVstiffR-pC->m_dbL3-pC->m_dbR3)<1.00)  return;
	}


	CGangjeBase pGBase(m_pDB);
	long nCol	= bLeft ? 0 : 1;
	long n		= 0;

	CPaintRecord RecMain;
	SetRecStrEleNameCrossBeam(&RecMain,pBx);

	////////////////////////////////////////////////

 	long nMarkBuje = CG_SP_U_U;
	if((pC->m_nLGirSpType && bLeft==TRUE) || (pC->m_nRGirSpType && bLeft==FALSE))
	{
		CPaintRecord *pRec = AddPaintRecord(&RecMain);
		if(pC->GetType()!=CR_TYPE_VSTIFF)
		{
			SetJewonPaintRecord(pRec,pBx,nMarkBuje);
			pRec->SetStrSubEleName("상면 이음판");
			// PAINT SETTING
			double dArea1 = pGBase.GetAreaBuje(nMarkBuje,pBx, 0,0,m_nDimType);
			double dArea2 = pGBase.GetPerimeterBuje(nMarkBuje,pBx, 0,0,m_nDimType)*pRec->m_vJewon.y;
			pRec->m_dPaintArea[SPLICE_FACT]		    = dArea1*2 + dArea2;
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE]  = dArea1 + dArea2;
		}
		else
		{
 			nMarkBuje = CG_SP_U_L;
			for (n = 0; n < 2; n++) 
			{
				if(n==1) pRec = AddPaintRecord(pRec);
				SetJewonPaintRecord(pRec,pBx,nMarkBuje);
				pRec->SetStrSubEleName("상면 이음판");
				// PAINT SETTING
				double dArea1 = pGBase.GetAreaBuje(nMarkBuje,pBx, 0,0,m_nDimType);
				double dArea2 = pGBase.GetPerimeterBuje(nMarkBuje,pBx, 0,0,m_nDimType)*pRec->m_vJewon.y;
				pRec->m_dPaintArea[SPLICE_FACT]		    = dArea1*2 + dArea2;
				pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE]  = dArea1 + dArea2;
			}
		}


		////////////////////////////////////////////////
 		nMarkBuje = CG_SP_U_L;
		for (n = 0; n < 2; n++) 
		{
			pRec = AddPaintRecord(pRec);
			SetJewonPaintRecord(pRec,pBx,nMarkBuje);
			pRec->SetStrSubEleName("하면 이음판");
			// PAINT SETTING
			double dArea1 = pGBase.GetAreaBuje(nMarkBuje,pBx);
			double dArea2 = pGBase.GetPerimeterBuje(nMarkBuje,pBx, 0,0,m_nDimType)*pRec->m_vJewon.y;
			pRec->m_dPaintArea[SPLICE_FACT]			= dArea1 * 2 + dArea2;
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE]  = dArea1 + dArea2;
			pRec->m_dPaintArea[OUTSIDE_FIELD]		= -dArea1;
		}

		////////////////////////////////////////////////
 		nMarkBuje = CG_SP_L_U;
		for (n = 0; n < 2; n++) 
		{
			pRec = AddPaintRecord(pRec);
			SetJewonPaintRecord(pRec,pBx,nMarkBuje);
			pRec->SetStrSubEleName("상면 이음판");
			// PAINT SETTING
			double dArea1 = pGBase.GetAreaBuje(nMarkBuje,pBx, 0,0,m_nDimType);
			double dArea2 = pGBase.GetPerimeterBuje(nMarkBuje,pBx, 0,0,m_nDimType)*pRec->m_vJewon.y;
			pRec->m_dPaintArea[SPLICE_FACT]		    = dArea1*2 + dArea2;
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE]  = dArea1 + dArea2;
			pRec->m_dPaintArea[OUTSIDE_FIELD]		= -dArea1;
		}

		////////////////////////////////////////////////
 		nMarkBuje = CG_SP_L_L;
		for (n = 0; n < 2; n++) 
		{
			if(pC->m_cGasset_CR==0 && n==1)	break;
			if(pC->m_H_CR==pBx->GetHeightGirderByBx() && n==1)	break;
			pRec = AddPaintRecord(pRec);
			SetJewonPaintRecord(pRec,pBx,nMarkBuje);
			pRec->SetStrSubEleName("하면 이음판");
			// PAINT SETTING
			double dArea1 = pGBase.GetAreaBuje(nMarkBuje,pBx, 0,0,m_nDimType);
			double dArea2 = pGBase.GetPerimeterBuje(nMarkBuje,pBx)*pRec->m_vJewon.y;
			pRec->m_dPaintArea[SPLICE_FACT]		    = dArea1*2 + dArea2;
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE]  = dArea1 + dArea2;
			pRec->m_dPaintArea[OUTSIDE_FIELD]		= -dArea1;
		}

	}
	
	////////////////////////////////////////////////
	if(pC->m_mB && pC->m_mC)//	if(pC->m_mW>0 && pC->m_mB>0 && pC->m_mC>0)
	{
		for(n=0; n < 2; n++) 
		{
			nMarkBuje = CG_SP_W_MO;
			CPaintRecord *pRec = AddPaintRecord(&RecMain);
			SetJewonPaintRecord(pRec,pBx,nMarkBuje);
			pRec->SetStrSubEleName("모멘트 이음판");
			double dArea1 = pGBase.GetAreaBuje(nMarkBuje,pBx,n,0,m_nDimType);
			double dArea2 = pGBase.GetPerimeterBuje(nMarkBuje,pBx)*pRec->m_vJewon.y;
			pRec->m_nQty = 2;
			pRec->m_dPaintArea[SPLICE_FACT]		    = (dArea1*2 + dArea2)*pRec->m_nQty;
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE]  = (dArea1 + dArea2)*pRec->m_nQty;
			pRec->m_dPaintArea[OUTSIDE_FIELD]		= -dArea1*pRec->m_nQty;
		}
	}

	////////////////////////////////////////////////
	if(pC->m_sB && pC->m_sC)
	{
		for(n=0; n < 2; n++) 
		{
			nMarkBuje = CG_SP_W_SH;
			CPaintRecord *pRec = AddPaintRecord(&RecMain);
			SetJewonPaintRecord(pRec,pBx,nMarkBuje);
			pRec->SetStrSubEleName("전단 이음판");
			// PAINT SETTING
			double dArea1 = pGBase.GetAreaBuje(nMarkBuje,pBx, 0,0,m_nDimType);
			pRec->m_nQty = 1;
			double dArea2 = pGBase.GetPerimeterBuje(nMarkBuje,pBx,n)*pRec->m_vJewon.y;
			pRec->m_dPaintArea[SPLICE_FACT]		    = (dArea1*2 + dArea2)*pRec->m_nQty;
			pRec->m_dPaintArea[BOLT_FIELD_OUTSIDE]  = (dArea1 + dArea2)*pRec->m_nQty;
			pRec->m_dPaintArea[OUTSIDE_FIELD]		= -dArea1*pRec->m_nQty;
		}
	}




/*
	///채움판 (Filler)
	double dThickFiller;
	int iQty = 1;////한쪽
	if(pC->m_nFillerUseType==1)//양쪽
		iQty = 2;
//	else if(pC->m_nFillerUseType==2)	iQty = 1;
	dThickFiller = pC->GetFillerThick(pBx);
	if((pC->m_nFillerUseType==1 || pC->m_nFillerUseType==2) && dThickFiller >= 2 && pDB->GetBridgeType() == SOSU_GIR)
	{
		nMarkBuje = CP_SP_W_FH;//132
		pRec = AddPaintRecord(pRec);
		pRec->SetStrSubEleName("채움판");
		SetJewonPaintRecord(pRec,pBx,nMarkBuje);
		pRec->m_nQty = iQty;

	}
*/

}

void CPaintCreator::AddCrossBeamAnchor(CPlateBasicIndex *pBx)
{
	CPlateGirderApp	*pGir = pBx->GetGirder();
	CPlateCrossBeam	*pC   = pBx->GetSection()->GetCrossBeam();

	if(pC->m_nAnchorsu_CR>0)
	{
		long nMarkBuje = GI_SA;
		CPaintRecord *pRec = AddPaintRecord();
		SetRecStrEleNameCrossBeam(pRec,pBx);
		SetJewonPaintRecord(pRec,pBx,nMarkBuje);
		pRec->SetStrSubEleName("슬래브 앵커");
		pRec->SetStrProcessBig("BOLT");
		pRec->SetStrProcessMid("SLAB ANCHOR");
		pRec->m_nQty = pC->m_nAnchorsu_CR;


		CPlateBridgeApp* pBridge = (CPlateBridgeApp*)pGir->GetBridgeApp();
		CPlateSangSeDo *pSang  = pBridge->GetGirder(-1)->m_pSangse;
		double H = pSang->m_SlabAnchor_A * 2 + pSang->m_SlabAnchor_B;//앵커바 곡선길이
		double D = pSang->m_SlabAnchor_T;//직경

		// π*D*H+π*(D/2)^2*2
		// PAINT SETTING
		pRec->m_dPaintArea[PAVE_FACT] =  ConstPi*D*H + ConstPi*(D/2)*(D/2) * 2;
	}
}

//기타부재 
void CPaintCreator::AddGitaBuje(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	CPlateGirderApp *pGir = pBxStt->GetGirder();
	CGangjeBase pGBase(m_pDB);

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx = NULL;



	// 지점부보강재
	pBx = Finder.GetBxFirst(BX_JIJUMSTIFF,pBxStt,pBxEnd);
	while(pBx)
	{
		AddGitaBujeJiJumStiff(pBx);		
		pBx = Finder.GetBxNext();
	}

	// 솔플레이트
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING|BX_VSTIFF,pBxStt,pBxEnd);
	while(pBx)
	{
		if(pBx->IsJijum())  AddGitaBujeSolePlate(pBx);		
		pBx = Finder.GetBxNext();
	}

	// 전단연결재 (앵커, 스터드, 각형)
	pBx = Finder.GetBxFirst(BX_STT_GIRDER|BX_SPLICE,pBxStt,pBxEnd,TRUE,FALSE);
	while( pBx )
	{
		AddGitaBujeShareConnect(pBx);
		pBx = Finder.GetBxNext();
	}

	// 동바리고리
	pBx = Finder.GetBxFirst(/*BX_CROSSBEAM_VBRACING|*/BX_VSTIFF,pBxStt,pBxEnd);
	while(pBx)
	{
		AddGitaBujeDongbari(pBx);		
		pBx = Finder.GetBxNext();
	}

	//잭업보강재
	pBx = Finder.GetBxFirst(BX_JACKUP, pBxStt, pBxEnd);
	while(pBx)
	{
		AddGitaBujeJackUp(pBx, TRUE);
		AddGitaBujeJackUp(pBx, FALSE);
		pBx = Finder.GetBxNext();
	}


	//잭업 솔플레이트
	pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	while( pBx )
	{
		if(pBx->IsJijum())
			AddGitaBujeJackUpPlate(pBx);
		pBx = Finder.GetBxNext();
	}


}
void CPaintCreator::AddGitaBujeJackUpPlate(CPlateBasicIndex* pBx)
{
	if(!pBx->IsJijum()) return;
	CPlateBridgeApp	*pBridge = m_pDataManage->GetBridge();
	CPlateGirderApp *pGir = pBx->GetGirder();
	long nJijum = pGir->GetNumberJijumByBx(pBx);
	for(long n=0; n<2; n++)
	{
		double dH = pBridge->m_structJackupJewon[nJijum][n].m_dH;
		double dW = pBridge->m_structJackupJewon[nJijum][n].m_dW;	
		if(dH>0 && dW>0)
		{
			long nMarkBuje  = GI_JA_SA;
			CPaintRecord * pRec = AddPaintRecord();
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
			//pRec->m_nFirstClass = FIRST_GIRDER;
			pRec->SetStrEleName("기타부재");
			pRec->SetStrProcessBig("GIRDER");
			pRec->SetStrProcessMid("JACK-UP보강");
			pRec->SetStrSubEleName("잭업 솔플레이트");
			pRec->SetStrSectionType("솔플레이트");
			pRec->m_nQty = 1;
			SetJewonPaintRecord(pRec,pBx,nMarkBuje,0,n);

				double W = pRec->m_vJewon.x;
				double T = pRec->m_vJewon.y;
				double L = pRec->m_vJewon.z;
				pRec->m_dPaintArea[OUTSIDE_FACT]  = 				  
				pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2+L*T; // 외부도장 (현장)
		}
	}
}

void CPaintCreator::AddGitaBujeJiJumStiff(CPlateBasicIndex* pBx)
{
	CPlateGirderApp* pGir	= pBx->GetGirder();
	CPlateGirderApp *pGirL	= pGir->GetNumberGirder() > 0 ? pGir->m_pLeft : NULL;

		for(long nCol=0; nCol<2; nCol++) //거더기준으로 좌우측.
		{
			CVStiff *pVStiff = pBx->GetJijumStiff();
			if(pVStiff->m_dWidth[nCol]==0 || pVStiff->m_dThick[nCol]==0) continue;

			CPlateBasicIndex* pBxL = pGirL ? pGirL->GetBxMatchByCrossBeam(pBx, pGirL) : NULL;

			long nMarkBuje = V_J_ST;		
			CPaintRecord * pRec = AddPaintRecord();	
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
			pRec->m_nQty = 1;				
			pRec->SetStrEleName("일반"/*"기타부재"*/);
			pRec->SetStrSubEleName("지점부보강재");
			SetJewonPaintRecord(pRec,pBx,nMarkBuje, 0, nCol);

			// 지점부보강재에 제작타입 가로보가 설치되어 있을 경우 - 거더 좌측
			if(nCol == 0 && pBxL && pBxL->IsState(BX_CROSSBEAM_VBRACING))
			{
				CSection *pSec = pBxL->GetSection();
				CPlateCrossBeam *pC = pSec->IsenDetType(SECDET_CROSSBEAM) ? pSec->GetCrossBeam() : NULL;
				if(pC && pC->GetType() == CR_TYPE_MANUFACTURE)
				{
					pRec->m_vJewon.z -= (pC->m_H_CR + pC->m_T1_CR);
					if(pC->m_dRH1 > 0)
						pRec->m_vJewon.z -= pC->m_T2_CR;
				}
			}

			// 지점부보강재에 제작타입 가로보가 설치되어 있을 경우 - 거더 우측
			if(nCol == 1 && pBx->IsState(BX_CROSSBEAM_VBRACING))
			{
				CSection *pSec = pBx->GetSection();
				CPlateCrossBeam *pC = pSec->IsenDetType(SECDET_CROSSBEAM) ? pSec->GetCrossBeam() : NULL;
				if(pC && pC->GetType() == CR_TYPE_MANUFACTURE)
				{
					pRec->m_vJewon.z -= (pC->m_H_CR + pC->m_T1_CR);
					if(pC->m_dLH1 > 0)
						pRec->m_vJewon.z -= pC->m_T2_CR;
				}
			}

			double W = pRec->m_vJewon.x;
			double T = pRec->m_vJewon.y;
			double L = pRec->m_vJewon.z;
			pRec->m_dPaintArea[OUTSIDE_FACT]  = 				  
			pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2+L*T; // 외부도장 (현장)
		}
}
void CPaintCreator::AddGitaBujeShareConnect(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CPaintRecord *pRec = AddPaintRecord();

	long nType = pGir->m_pSangse->m_StudHori_Type;	// 0 앵커, 1 스터드, 2 각형
	long nMarkBuje;
	CString szBuje;
	if(nType == 0)
	{
		nMarkBuje = GI_SA;
		szBuje.Format("슬래브 앵커");
	}
	else if(nType == 1)
	{
		nMarkBuje = GI_SB;
		szBuje.Format("전단연결재");
	}
	else
	{
		nMarkBuje = GI_SQ;
		szBuje.Format("전단연결재 각형");
	}

	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
	pRec->SetStrEleName("일반"/*기타부재*/);
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("STUD BOLT");
	pRec->SetStrSectionType(szBuje);	// Weld Stud
	pRec->SetStrSubEleName(szBuje);	// Weld Stud
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->m_nQty = pBx->GetGirder()->GetStudBoltSu(pRec->m_nSplice);
	if(nType != 1)	// 전단연결재의 숫자 계산으로, 스터드 수만큼 곱해져 있다..
		pRec->m_nQty /= pGir->m_pSangse->m_StudHori_Qty;

	double W = pRec->m_vJewon.x;
	double D = pRec->m_vJewon.y;
	double L = pRec->m_vJewon.z;
	if(nMarkBuje == GI_SB)
		pRec->m_dPaintArea[PAVE_FACT] = ( ConstPi*D*L + ConstPi*(D/2)*(D/2) ) * pRec->m_nQty;
	else if(nMarkBuje == GI_SQ)
		pRec->m_dPaintArea[PAVE_FACT] = (W+2*D)*L * pRec->m_nQty;
	else
		pRec->m_dPaintArea[PAVE_FACT] = ConstPi*D*L * pRec->m_nQty;
}

void CPaintCreator::AddGitaBujeDongbari(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CSection        *pSec = pBx->GetSection();

	if(pSec==NULL)	return;
	if(pSec->GetVStiff()==NULL)	 return;
	
	CVStiff *pVStiff = pSec->GetVStiff();

	BOOL bUpper = pVStiff->m_cCarryUpper;
	BOOL bLower = pVStiff->m_cCarryLower;

	if(!bUpper && !bLower) return;

	BOOL bLeft  = TRUE;
	BOOL bRight = TRUE;
	
	if(pVStiff->m_dWidth[0]>0 && pVStiff->m_dWidth[1]==0)  bLeft	= FALSE;
	if(pVStiff->m_dWidth[0]==0 && pVStiff->m_dWidth[1]>0)  bRight	= FALSE;

	if(!bLeft && !bRight) return;

	long nMarkBuje = GI_HG;

	CPlateBridgeApp* pBri = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CPlateGirderApp* pGirC = pBri->GetGirder(-1);

	long nCount = 0;
	if(bLeft)	nCount++;
	if(bRight)	nCount++;

	for(long n=0; n<nCount; n++)
	{
		CPaintRecord *pRec = AddPaintRecord();
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		//pRec->m_nFirstClass		= FIRST_GIRDER;
		pRec->SetStrEleName("일반"/*"기타부재"*/);
		pRec->SetStrSubEleName("동바리용 고리");
		pRec->SetStrProcessBig("GIRDER");
		pRec->SetStrProcessMid("동바리");
		
		SetJewonPaintRecord(pRec,pBx,nMarkBuje);
		pRec->m_nQty = 0;
		if(bLower)	pRec->m_nQty++;
		if(bUpper)	pRec->m_nQty++;

		double W = pRec->m_vJewon.x;
		double T = pRec->m_vJewon.y;
		double L = pRec->m_vJewon.z;

		if(pGirC->m_pSangse->m_Dongbari_Type)  // 플랜지 접합타입
		{
			pRec->m_dPaintArea[OUTSIDE_FACT]  = 				  
			pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2+(W+L)*T;	// 외부도장 (현장)
		}
		else									// 일반타입
		{
			pRec->m_dPaintArea[OUTSIDE_FACT]  = 				  
			pRec->m_dPaintArea[OUTSIDE_FIELD] = W*L*2+(W*2+L)*T; // 외부도장 (현장)
		}
	}
}

void CPaintCreator::AddGitaBujeSolePlate(CPlateBasicIndex *pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();
	CGangjeBase pGBase(m_pDB);

	long nMarkBuje = GI_SO;
	CPaintRecord *pRec = AddPaintRecord();
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
	SetJewonPaintRecord(pRec,pBx,nMarkBuje,0,pGir->GetNumberJijumByBx(pBx));
	pRec->SetStrEleName("일반"/*"기타부재"*/);
	pRec->SetStrProcessBig("GIRDER");
	pRec->SetStrProcessMid("SOLE");
	pRec->SetStrSubEleName("솔플레이트");
	pRec->SetStrSectionType("솔플레이트");


	
	int k = pBx->GetNumberJijum();
	CShoe * pShoe = pGir->GetShoe(k);
	double WP = pShoe->m_dH_SolePlate + pShoe->m_dH2_SolePlate*2;
	double LP = pShoe->m_dW_SolePlate + pShoe->m_dW2_SolePlate*2;
	double T = pShoe->m_dThick_SolePlate;

	
	// PAINT SETTING
	pRec->m_dPaintArea[OUTSIDE_FACT] = (WP*LP+(WP+LP)*T*2);
}

CPaintRecord* CPaintCreator::AddPaintRecord(CPaintRecord* pRec /*=NULL*/)
{
	pRec = new CPaintRecord(pRec);
	pRec->m_nCountMake = m_ListStandard.GetCount();
	m_ListStandard.AddTail(pRec);
	for(long i=0; i<PAINT_ITEM_MAX; i++)
		pRec->m_dPaintArea[i] = 0;
	return pRec;
}

void CPaintCreator::SetJewonPaintRecord(CPaintRecord *pRec, CPlateBasicIndex *pBx, long nMarkBuje, long nRow, long nCol, BOOL bFront, BOOL bLeftBeam)
{
	CPlateGirderApp	*pGir    = pBx->GetGirder();
	CPlateBridgeApp	*pDB     = (CPlateBridgeApp*)pGir->GetBridgeApp();		
	CGlobarOption   *pGloOpt = m_pDataManage->GetGlobalOption(); 
	
	long nRoundUnit = pGloOpt->GetPointValueOfUnitGangJae();
	// Girder.....
	//pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nMarkBuje = nMarkBuje;
	pRec->m_sMaterial = pDB->GetMaterial(nMarkBuje,pBx);
	// 제원 SETTING
	CVector vJewon;
	vJewon.x = Round(pGir->GetWidthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
	vJewon.y = Round(pGir->GetThickJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
	vJewon.z = Round(pGir->GetLengthJewon(nMarkBuje, pBx, nRow, nCol, m_nDimType, bFront, bLeftBeam),0);
	pRec->m_vJewon = vJewon;
	pRec->m_nQty   = 1;
	if(pDB->IsBolt(pRec->m_nMarkBuje))
		pRec->m_dUWeight  = Round( m_pDB->GetWeightUnitKgM(nMarkBuje,pBx, pRec->m_vJewon),nRoundUnit);
}

void CPaintCreator::SetPaintRecordFact(CPaintRecord *pRec, CPlateBasicIndex* pBx)
{
	CPlateGirderApp*	pGir = pBx->GetGirder();

	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
	if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
	pRec->SetStrEleName("일반");

	pRec->SetStrProcessBig("MAIN P/N");
	pRec->SetStrProcessMid("비연결재");
	//if(pGir->GetFactConnectType(pBx,nMarkBuje)==1) pRec->SetStrProcessMid("끝단재");
	//if(pGir->GetFactConnectType(pBx,nMarkBuje)==2) pRec->SetStrProcessMid("중간재");	
}

void CPaintCreator::SetPaintRecordSplice(CPaintRecord *pRec, CPlateBasicIndex* pBx)
{
	CPlateGirderApp*	pGir = pBx->GetGirder();
	CPlateBridgeApp* pDB = (CPlateBridgeApp*)pGir->GetBridgeApp();

	pRec->m_nGirder		= pBx->GetNumberGirder();
	pRec->m_nSplice		= pGir->GetNumberSpliceByBx(pBx);
	//pRec->m_nFirstClass = FIRST_GIRDER;
//	if(pBx->IsState(BX_SPLICE)) pRec->m_nSplice++;
	pRec->SetStrSectionType("현장이음");
	pRec->SetStrProcessBig("SPLICE");
	pRec->SetStrProcessMid("GIRDER");
}

void CPaintCreator::SetRecStrEleNameCrossBeam(CPaintRecord *pRec, CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir = pBx->GetGirder();

	pRec->m_nGirder     = pBx->GetNumberGirder();
	pRec->m_nSplice     = pGir->GetNumberSpliceByBx(pBx);
//	pRec->m_nFirstClass = FIRST_CROSS;

	if(pBx->GetSection()->IsenDetType(SECDET_CROSSBEAM))
	{
		if(pBx->IsJijum())
			pRec->SetStrEleName("지점부 가로보"); 
		else
			pRec->SetStrEleName("분배 가로보");
	}
	else if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
		pRec->SetStrEleName("수직 브레이싱");
	else
		pRec->SetStrSectionType("가로보");
}

CString CPaintCreator::GetMarkBx(long nMarkType,long nMark,CPlateBasicIndex* pBx,long nRow,long nCol, BOOL bFront) const
{
	if(!IsMakeMark(nMarkType)) return _T("");

/*	switch(nMarkType)
	{
	case CMarkSerial::MARK_BUJE :	
		return m_pDB->GetMarkBuje(nMark,pBx,nRow,nCol,bFront);
	case CMarkSerial::MARK_SECTION :	
		return m_pDB->GetMarkSection(nMark,pBx);
	case CMarkSerial::MARK_ASSEM :		
		return m_pDB->GetMarkAssembly(nMark,pBx,nRow);
	case CMarkSerial::MARK_ASSEM_MID :		
		return m_pDB->GetMarkAssemblyMid(nMark,pBx,nRow);
	case CMarkSerial::MARK_ASSEM_BIG :		
		return m_pDB->GetMarkAssemblyBig(nMark,pBx,nRow);
	}
	*/
	return _T("");
}

void CPaintCreator::SetMakeMark(long nMAKE_MARK,BOOL bApply)
{
	SetBitFlag32(m_dwMakeMarkFlag,nMAKE_MARK,bApply);
}

BOOL CPaintCreator::IsMakeMark(long nMAKE_MARK) const
{
	return GetBitFlag32(m_dwMakeMarkFlag,nMAKE_MARK);
}

void CPaintCreator::AddFacePlate(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd)
{
	if(!m_pDB->m_cFacePlateUse) return;

	CGangjeBase pGBase(m_pDB);
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_STT_GIRDER|BX_END_GIRDER,pBxStt,pBxEnd);

	while( pBx )
	{
		CPlateGirderApp *pGir = pBx->GetGirder();
		CPaintRecord *pRec = AddPaintRecord();
		SetJewonPaintRecord(pRec,pBx,GI_FA);
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);

		pRec->SetStrEleName("기타부재");
		pRec->SetStrSectionType("복부판");
		pRec->SetStrSubEleName("FACE PLATE");
		pRec->SetStrProcessBig("GIRDER");
		pRec->SetStrProcessMid("MAIN WEB RING");
		pRec->m_nQty = 1;
		pRec->m_dPaintArea[OUTSIDE_FACT]  = pGBase.GetAreaBuje(GI_FA,pBx,0,0,m_nDimType,TRUE)*BOTHSIDE;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = pGBase.GetAreaBuje(GI_FA,pBx,0,0,m_nDimType,TRUE)*BOTHSIDE;
		//
		pBx = Finder.GetBxNext();
	}
}

void CPaintCreator::AddFieldSplice(CPlateBasicIndex *pBxStt, CPlateBasicIndex *pBxEnd, BOOL bIncludeStt, BOOL bIncludeEnd)
{

	CPlateGirderApp		*pGir = pBxStt->GetGirder();
	CPlateBridgeApp* pDB  = (CPlateBridgeApp*)pGir->GetBridgeApp();
	CSteelDeduct	*pDed = m_pDataManage->GetSteelDeduct();

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_SPLICE,pBxStt,pBxEnd);


	if(!bIncludeStt && pBx==pBxStt)
		pBx = Finder.GetBxNext();

	while( pBx )
	{
		if(!bIncludeEnd && pBx==pBxEnd)
			break;

			AddSpliceUpper(pBx);
			AddSpliceLower(pBx);
			AddSpliceWeb(pBx);	// 좌측
			AddSpliceWeb(pBx);	// 우측

		pBx = Finder.GetBxNext();
	}

}

CString CGangjeBase::GetStringWeight(double dWeight, long nRound)
{
	CString sResult;

	if(nRound<=0)	sResult.Format("%.0lf" ,dWeight);
	if(nRound==1)	sResult.Format("%.1lf" ,dWeight);
	if(nRound==2)	sResult.Format("%.2lf" ,dWeight);
	if(nRound==3)	sResult.Format("%.3lf" ,dWeight);
	if(nRound==4)	sResult.Format("%.4lf" ,dWeight);
	if(nRound==5)	sResult.Format("%.5lf" ,dWeight);
	if(nRound==6)	sResult.Format("%.6lf" ,dWeight);
	if(nRound==7)	sResult.Format("%.7lf" ,dWeight);
	if(nRound==8)	sResult.Format("%.8lf" ,dWeight);
	if(nRound==9)	sResult.Format("%.9lf" ,dWeight);
	if(nRound>=10)	sResult.Format("%.10lf",dWeight);

	return sResult;
}

////////////////////////// VBracing /////////////////////////////////////
void CPaintCreator::AddHBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBxFinder Finder2(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);
	CPlateBasicIndex* pBx2 = (CPlateBasicIndex*)Finder2.GetBxFirst(BX_HBRACINGGUSSET,pBxStt,pBxEnd);

	while( pBx )
	{
		AddHBracingAngle(pBx);
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}

	while( pBx2 )
	{
		AddHBracingGusset(pBx2);
		AddHBracingBolt(pBx2);		
		pBx2 = (CPlateBasicIndex*)Finder2.GetBxNext();
	}

}

void CPaintCreator::AddHBracingAngle(CPlateBasicIndex* pBx)
{
	CGangjeBase pGBase(m_pDB);
	CGlobarOption   *pGOpt     = m_pDataManage->GetGlobalOption();
	CSteelMaterial  *pSteelMat = m_pDataManage->GetSteelMaterial();
	CPlateGirderApp *pGir      = pBx->GetGirder();
	CSection        *pSec      = pBx->GetSection();
	CBracing        *pBra      = pBx->GetBracing();
	CSteelMaterialData	*pMatData	= pBra->GetSteelMaterialData();

	long nRoundUnit = pGOpt->GetPointValueOfUnitGangJae();
	long nRoundTot  = pGOpt->GetPointValueOfGangJae();
	long nMarkBuje	= 0;
	long nSteelType	= pMatData->m_nType;
	long nJewonIdx	= pMatData->m_nJewonIdx;
	double dResult = 0, dB = 0, dH = 0, dL = 0, dT1 = 0, dT2 = 0;

	if(pBra->GetType() == CBracing::BRACTYPE_LEFTWAVE || 
		pBra->GetType() == CBracing::BRACTYPE_RIGHTWAVE || 
		pBra->GetType() == CBracing::BRACTYPE_CROSS)
	{
		for(long n=0; n<2; n++)
		{		
			nMarkBuje = HB_B;

			CPaintRecord *pRec = AddPaintRecord();			
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);

			// 0 , 1
			pRec->m_nMarkBuje = nMarkBuje;
			pRec->SetStrEleName("수평 브레이싱");

			pRec->m_nQty = 1;
			pRec->m_nKindName  = MATERIAL_KIND_BEAM;			
			pRec->m_dUWeight   = Round(pMatData->m_dUnitWeight*1000, nRoundUnit);		// kg/m		
			pRec->m_sMaterial  = m_pDB->GetMaterial(pRec->m_nMarkBuje,pBx);
			/////////////////////////////////////////////////////////////////////////////////////////////////
			pRec->m_vJewon.x      = pBra->GetBeamJewon(n,FALSE,0);	//dH
			pRec->m_vJewonAngle.x = pBra->GetBeamJewon(n,FALSE,1); //dB
			pRec->m_vJewon.y      = pBra->GetBeamJewon(n,FALSE,2); //dT1
			pRec->m_vJewonAngle.y = pBra->GetBeamJewon(n,FALSE,3); //dT2		
			/////////////////////////////////////////////////////////////////////////////////////////////////

			pRec->m_vJewon.z = pBra->GetBeamJewon(n,FALSE,6);
			pRec->m_dTWeight = Round(pRec->m_dUWeight * toM(pRec->m_vJewon.z), nRoundTot);	// kg
						
			dH = pRec->m_vJewon.x;
			dB = pRec->m_vJewonAngle.x;
			dT1 = pRec->m_vJewon.y;
			dT2 = pRec->m_vJewonAngle.y;
			dL = pRec->m_vJewon.z;

			long nSteelType = pBra->GetSteelMaterialData()->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_LHBEAM);

			if(nSteelType==CSteelMaterialData::LSTEEL)
			{
				pRec->SetStrSubEleName("등변 ㄱ 형강");
				dResult = ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));
			}
			else if(nSteelType==CSteelMaterialData::LVALSTEEL)
			{
				pRec->SetStrSubEleName("부등변 ㄱ 형강");
				dResult = ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));
			}
			else if(nSteelType==CSteelMaterialData::CSTEEL)
			{
				pRec->SetStrSubEleName("ㄷ 형강");
				dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));
			}
			else if(nSteelType==CSteelMaterialData::CTSTEEL)
			{
				pRec->SetStrSubEleName("CT 형강");		
				dResult = ((dB+dH)*dL*2 + (dB*dT2+dH*dT1)*2 - (dT1*dT2*2));
			}
			else if(nSteelType==CSteelMaterialData::ISTEEL)
			{
				pRec->SetStrSubEleName("I 형강");
				dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));
			}
			else if(nSteelType==CSteelMaterialData::HSTEEL)
			{
				pRec->SetStrSubEleName("H 형강");
				dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));
			}


			pRec->m_dPaintArea[OUTSIDE_FACT]  = dResult;
			pRec->m_dPaintArea[OUTSIDE_FIELD] = dResult;
		}
	}
	else if(pBra->GetType() == CBracing::BRACTYPE_SLOPLEFT || 
		pBra->GetType() == CBracing::BRACTYPE_SLOPRIGHT)
	{
		nMarkBuje = HB_B;
		
		CPaintRecord *pRec = AddPaintRecord();	
		//SetJewonPaintRecord(pRec,pBx,nMarkBuje);
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);

		// 0 , 1
		
		pRec->m_nMarkBuje = nMarkBuje;
		pRec->SetStrEleName("수평 브레이싱");

		// 0 - 상부  1 - 하부
		
		//pRec->SetStrProcessMid("HBRACING");

		pRec->m_nQty = 1;
		pRec->m_nKindName  = MATERIAL_KIND_BOLT;
//		pRec->m_dUWeight   = Round(pSteelMat->GetUWeight(pBx, CSteelMaterial::BUJE_LHBEAM)*1000, nRoundUnit);		// kg/m	
		pRec->m_dUWeight   = Round(pMatData->m_dUnitWeight*1000, nRoundUnit);		// kg/m		
		pRec->m_sMaterial  = m_pDB->GetMaterial(pRec->m_nMarkBuje,pBx);
		/////////////////////////////////////////////////////////////////////////////////////////////////
		pRec->m_vJewon.x      = pBra->GetBeamJewon(0,FALSE,0);	//dH
		pRec->m_vJewonAngle.x = pBra->GetBeamJewon(0,FALSE,1); //dB
		pRec->m_vJewon.y      = pBra->GetBeamJewon(0,FALSE,2); //dT1
		pRec->m_vJewonAngle.y = pBra->GetBeamJewon(0,FALSE,3); //dT2		
		/////////////////////////////////////////////////////////////////////////////////////////////////

		pRec->m_vJewon.z = pBra->GetBeamJewon(0,FALSE,6);
		pRec->m_dTWeight = Round(pRec->m_dUWeight * pRec->m_vJewon.z, nRoundTot);
		
		dH = pRec->m_vJewon.x;
		dB = pRec->m_vJewonAngle.x;
		dT1 = pRec->m_vJewon.y;
		dT2 = pRec->m_vJewonAngle.y;
		dL = pRec->m_vJewon.z;

		long nSteelType = pBra->GetSteelMaterialData()->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_LHBEAM);

		if(nSteelType==CSteelMaterialData::LSTEEL){
			pRec->SetStrSubEleName("등변 ㄱ 형강");
			dResult = ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));}
		else if(nSteelType==CSteelMaterialData::LVALSTEEL){
			pRec->SetStrSubEleName("부등변 ㄱ 형강");
			dResult = ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));}
		else if(nSteelType==CSteelMaterialData::CSTEEL){
			pRec->SetStrSubEleName("ㄷ 형강");
			dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));}
		else if(nSteelType==CSteelMaterialData::CTSTEEL){
			pRec->SetStrSubEleName("CT 형강");		
			dResult = ((dB+dH)*dL*2 + (dB*dT2+dH*dT1)*2 - (dT1*dT2*2));}
		else if(nSteelType==CSteelMaterialData::ISTEEL){
			pRec->SetStrSubEleName("I 형강");
			dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));}
		else if(nSteelType==CSteelMaterialData::HSTEEL){
			pRec->SetStrSubEleName("H 형강");
			dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));}


		pRec->m_dPaintArea[OUTSIDE_FACT]  = dResult;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = dResult;
	}
}

void CPaintCreator::AddHBracingGusset(CPlateBasicIndex* pBx)
{
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CHBracingGusset *pBrGu = pBx->GetHBracingGusset();

	for(long nRow=0; nRow<2; nRow++)
	{
		if(pBrGu->IsReal(nRow))
		{
			CPaintRecord * pRec = AddPaintRecord();			
			SetJewonPaintRecord(pRec,pBx,HB_GU, nRow, 0);
			pRec->m_nGirder = pBx->GetNumberGirder();
			pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
					
			pRec->SetStrEleName("수평 브레이싱");
			pRec->SetStrSubEleName("브레이싱 이음판");	
		
			pRec->m_nQty = 1;

			double W = pRec->m_vJewon.x;
			double T = pRec->m_vJewon.y;
			double L = pRec->m_vJewon.z;
				
			pRec->m_dPaintArea[OUTSIDE_FACT]  = 
			pRec->m_dPaintArea[OUTSIDE_FIELD]  = W*L*2 + (W+L)*T*2;
		}
	}
}

void CPaintCreator::AddHBracingBolt(CPlateBasicIndex* pBx)
{
	CGangjeBase pGBase(m_pDB);
	CPlateGirderApp *pGir = pBx->GetGirder();
	CHBracingGusset* pBrGu = pBx->GetHBracingGusset();
	long nMarkBuje = HB_BOLT; // 상부	
	if(pBrGu->IsReal())
	{
		
		CPaintRecord *pRec = AddPaintRecord();
		SetJewonPaintRecord(pRec,pBx,nMarkBuje);
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);	
		pRec->SetStrEleName("수평 브레이싱");
		pRec->SetStrSubEleName("볼트-너트");
		pRec->SetStrProcessBig("BOLT");
		pRec->SetStrProcessMid("H.T.BOLT");
		pRec->m_nQty = pBrGu->GetBoltsu();
		pRec->m_dTWeight = pRec->m_dUWeight*pRec->m_nQty;
	}
}
////////////////////////////////////////////////////////////////////////

////////////////////////// VBracing /////////////////////////////////////
void CPaintCreator::AddVBracing(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	//CGangjeBase pGBase(m_pDB);
	CPlateBxFinder Finder(pBxStt->GetGirder());
	CPlateBasicIndex* pBx = (CPlateBasicIndex*)Finder.GetBxFirst(BX_CROSSBEAM_VBRACING,pBxStt,pBxEnd);

	while( pBx )
	{
		if(pBx->GetSection()->IsenDetType(SECDET_VBRACING))
		{
			CSection *pSec = pBx->GetSection();
			if(pSec->IsenDetType(SECDET_VBRACING)) 		
			{
				AddVBracingAngle(pBx);
				AddVBracingGusset(pBx);
				AddVBracingBolt(pBx);
			}
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
		
	}
}

void CPaintCreator::AddVBracingAngle(CPlateBasicIndex* pBx)
{	
	CGangjeBase pGBase(m_pDB);
	CGlobarOption   *pGOpt = m_pDataManage->GetGlobalOption();
	CSteelMaterial  *pSteelMat = m_pDataManage->GetSteelMaterial();
	CPlateGirderApp *pGir  = pBx->GetGirder();
	CSection        *pSec  = pBx->GetSection();
	CVBracing       *pVBra = pSec->GetVBracing();
	CSteelMaterialData	*pMatData	= NULL;

	long nRoundUnit = pGOpt->GetPointValueOfUnitGangJae();
	long nRoundTot  = pGOpt->GetPointValueOfGangJae();
	long nSteelType = 0;	// 형강종류
	long nJewonIdx	= 0;
	long nBeamPos   = 0;	// 형강위치
	long nMarkBuje;
	double dResult, dB, dH, dL, dT1, dT2;

	for(long n=0; n<4; n++)
	{
		CPaintRecord *pRec = AddPaintRecord();

		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);
		pRec->SetStrEleName("수직 브레이싱");
		// 0 - 상현재, 1 - 하현재, 2 - 사재 좌측, 3 - 사재 우측
		if(n==0)	
		{
			pMatData	= pVBra->GetSteelMaterialData(STEELMAT_UPPER);
			nSteelType	= pMatData->m_nType;//pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_UVBEAM);
			nJewonIdx	= pMatData->m_nJewonIdx;
			nBeamPos	= CSteelMaterial::BUJE_UVBEAM;
			nMarkBuje	= VB_UB;			
		}
		else if(n==1)
		{
			pMatData	= pVBra->GetSteelMaterialData(STEELMAT_LOWER);
			nSteelType	= pMatData->m_nType;//nSteelType = pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_LVBEAM);
			nJewonIdx	= pMatData->m_nJewonIdx;
			nBeamPos	= CSteelMaterial::BUJE_LVBEAM;
			nMarkBuje	= VB_LB;
		}
		else 
		{
			pMatData	= pVBra->GetSteelMaterialData(STEELMAT_DIAGONAL);
			nSteelType	= pMatData->m_nType;//nSteelType = pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_CVBEAM);
			nJewonIdx	= pMatData->m_nJewonIdx;
			nBeamPos	= CSteelMaterial::BUJE_CVBEAM;
			nMarkBuje	= VB_CR;
		}

		pRec->m_nMarkBuje = nMarkBuje;

		pRec->m_nQty = 1;
		pRec->m_nKindName  = MATERIAL_KIND_BEAM;
		
//		pRec->m_dUWeight = Round(pSteelMat->GetUWeight(pBx, nBeamPos)*1000, nRoundUnit);		// kg/m		
		pRec->m_dUWeight = Round(pMatData->m_dUnitWeight*1000, nRoundUnit);		// kg/m		
		pRec->m_sMaterial  = m_pDB->GetMaterial(pRec->m_nMarkBuje,pBx);
		
		pRec->m_vJewon.x      = pSec->GetBeamJewon(n,0); //dH
		pRec->m_vJewonAngle.x = pSec->GetBeamJewon(n,1); //dB
		pRec->m_vJewon.y      = pSec->GetBeamJewon(n,2); //dT1
		pRec->m_vJewonAngle.y = pSec->GetBeamJewon(n,3); //dT2		
		
		pRec->m_vJewon.z = pSec->GetVBracing()->GetLengthBeam(n);
		pRec->m_dTWeight = Round(pRec->m_dUWeight * toM(pRec->m_vJewon.z), nRoundTot);	// kg
		
		dH = pRec->m_vJewon.x;	// 복부판의 높이
		dB = pRec->m_vJewonAngle.x;
		dT1 = pRec->m_vJewon.y;
		dT2 = pRec->m_vJewonAngle.y;
		dL = pRec->m_vJewon.z;

		//long nSteelType = pSteelMat->GetSteelType(pBx, CSteelMaterial::BUJE_LHBEAM);

		if(nSteelType==CSteelMaterialData::LSTEEL){
			pRec->SetStrSubEleName("등변 ㄱ 형강");
			dResult = ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));}
		else if(nSteelType==CSteelMaterialData::LVALSTEEL){
			pRec->SetStrSubEleName("부등변 ㄱ 형강");
			dResult = ((dB+dH)*dL*2 + (dB+dH)*dT1*2 - (dT1*dT1*2));}
		else if(nSteelType==CSteelMaterialData::CSTEEL){
			pRec->SetStrSubEleName("ㄷ 형강");
			dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));}
		else if(nSteelType==CSteelMaterialData::CTSTEEL){
			pRec->SetStrSubEleName("CT 형강");		
			dResult = ((dB+dH)*dL*2 + (dB*dT2+dH*dT1)*2 - (dT1*dT2*2));}
		else if(nSteelType==CSteelMaterialData::ISTEEL){
			pRec->SetStrSubEleName("I 형강");
			dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));}
		else if(nSteelType==CSteelMaterialData::HSTEEL){
			pRec->SetStrSubEleName("H 형강");
			dResult = ((dB+dH)*dL*2 + (dB*dT2*2 + dH*dT1)*2 - (dT1*dT2*4));}


		pRec->m_dPaintArea[OUTSIDE_FACT]  = dResult;
		pRec->m_dPaintArea[OUTSIDE_FIELD] = dResult;
	}
}


void CPaintCreator::AddVBracingGusset(CPlateBasicIndex* pBx)
{	
	CGangjeBase pGBase(m_pDB);
	CPlateGirderApp *pGir = pBx->GetGirder();
	long nMarkBuje = VB_GU;
	
	for(long n=0; n<5; n++)
	{
		CPaintRecord *pRec = AddPaintRecord();
		SetJewonPaintRecord(pRec,pBx,nMarkBuje,0,n);
		pRec->m_nGirder = pBx->GetNumberGirder();
		pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);			
		pRec->SetStrEleName("수직 브레이싱");
		pRec->SetStrSubEleName("브레이싱 이음판");	
		pRec->m_nQty = 1;
		
			double W = pRec->m_vJewon.x;
			double T = pRec->m_vJewon.y;
			double L = pRec->m_vJewon.z;
				
			pRec->m_dPaintArea[OUTSIDE_FACT]  = 
			pRec->m_dPaintArea[OUTSIDE_FIELD]  = W*L*2 + (W+L)*T*2;;
	}
}

void CPaintCreator::AddVBracingBolt(CPlateBasicIndex* pBx)
{
	CGangjeBase pGBase(m_pDB);
	CPlateGirderApp *pGir = pBx->GetGirder();
	CSection *pSec   = pBx->GetSection();
	CVBracing *pB	 = pSec->GetVBracing();

	long nMarkBuje = VB_BOLT; // 상부	
	CPaintRecord *pRec = AddPaintRecord();
	SetJewonPaintRecord(pRec,pBx,nMarkBuje);
	pRec->m_nGirder = pBx->GetNumberGirder();
	pRec->m_nSplice = pGir->GetNumberSpliceByBx(pBx);	
	pRec->SetStrEleName("수직 브레이싱");
	pRec->SetStrSubEleName("볼트-너트");
	pRec->SetStrProcessBig("BOLT");
	pRec->SetStrProcessMid("H.T.BOLT");
	pRec->m_nQty  = (long)( (pB->m_uB + 1) * 2 + (pB->m_dB + 1) * 2);
	pRec->m_dTWeight = pRec->m_dUWeight*pRec->m_nQty;
}
////////////////////////////////////////////////////////////////////////

// 가로보 위치(BX_CROSSBEAM_VBRACING)에 있는 거더부착 수직보강재의 물량계산
void CPaintCreator::AddVBracingGirderVStiff(CPlateBasicIndex* pBxStt, CPlateBasicIndex* pBxEnd)
{
	CPlateBridgeApp *pDB	= m_pDataManage->GetBridge();
	CPlateGirderApp *pGir	= pBxStt->GetGirder();
	CPlateGirderApp *pGirL	= pGir->GetNumberGirder() != 0 ? pGir->m_pLeft : NULL;

	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING, pBxStt, pBxEnd);

	while(pBx)
	{
		double dW=0, dT=0, dH=0, dReault=0;
		if(pBx->GetSection()->IsenDetType(SECDET_VBRACING) && !pBx->IsJijum())	// 지점일때는 지점부보강재에서
		{
			CVBracing *pVr = pBx->GetSection()->GetVBracing();
			CVStiff *pVStiff = pVr ? pVr->m_pVStiff : NULL;
			if(pVStiff==NULL) continue;

			CPlateBasicIndex *pBxL	= pGirL ? pGir->GetBxMatchByCrossBeam(pBx, pGirL) : NULL;
			CSection		 *pSecL	= pBxL  ? pBxL->GetSection() : NULL;
			CPlateCrossBeam	 *pCL	= (pSecL && pSecL->IsenDetType(SECDET_CROSSBEAM)) ? pSecL->GetCrossBeam() : NULL;

			long nMarkBuje =V_ST;

			CPaintRecord *pRec;
			CPaintRecord RecMain;
			SetRecStrEleNameCrossBeam(&RecMain,pBx);
			if(pVStiff->m_dWidth[0] > 0)
			{
				if(pVStiff->GetVStiffHeight(TRUE) > 0)	// 거더 좌측
				{
					pRec = AddPaintRecord(&RecMain);
					pRec->SetStrEleName("일반");
					pRec->SetStrSubEleName("수직보강재");
					pRec->SetStrProcessMid("V-STIFF");
					pRec->m_nQty = 1;
				
					SetJewonPaintRecord(pRec, pBx, nMarkBuje, 0, 0);

					// 수직브레이싱 좌측에 제작타입의 가로보가 설치된 경우 (동일스테이션에 수직브레이싱, 가로보 혼합설치 고려)
					if(pCL && pCL->GetType() == CR_TYPE_MANUFACTURE)
					{
						pRec->m_vJewon.z -= (pCL->m_H_CR + pCL->m_T1_CR);
						if(pCL->m_dRH1 > 0)
							pRec->m_vJewon.z -= pCL->m_T2_CR;
					}

					dW = pRec->m_vJewon.x;
					dT = pRec->m_vJewon.y;
					dH = pRec->m_vJewon.z;

					dReault = (dW*dH*2) + (dH*dT);

					pRec->m_dPaintArea[OUTSIDE_FACT]  = dReault;
					pRec->m_dPaintArea[OUTSIDE_FIELD] = dReault;
				}
			}
			if(pVStiff->m_dWidth[1] > 0)
			{
				if(pVStiff->GetVStiffHeight(FALSE) > 0)	// 거더 우측
				{
					pRec = AddPaintRecord(&RecMain);
					pRec->SetStrEleName("일반");
					pRec->SetStrSubEleName("수직보강재");
					pRec->SetStrProcessMid("V-STIFF");
					pRec->m_nQty = 1;

					SetJewonPaintRecord(pRec, pBx, nMarkBuje, 0, 1);

					
					dW = pRec->m_vJewon.x;
					dT = pRec->m_vJewon.y;
					dH = pRec->m_vJewon.z;

					dReault = (dW*dH*2) + (dH*dT);

					pRec->m_dPaintArea[OUTSIDE_FACT]  = dReault;
					pRec->m_dPaintArea[OUTSIDE_FIELD] = dReault;
				}
			}		
		}
		pBx = (CPlateBasicIndex*)Finder.GetBxNext();
	}
}



 
