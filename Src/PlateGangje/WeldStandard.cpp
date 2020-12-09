// WeldStandard.cpp: implementation of the CWeldStandard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../APlateData/APlateData.h"
#include "WeldCreator.h"
#include "WeldStandard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeldStandard::CWeldStandard(CDataManage* pMng) : CWeldCreator(pMng)
{

}

CWeldStandard::~CWeldStandard()
{

}


UINT CWeldStandard::DoWork()
{
	UINT nErr = 0;

	// DATA LIST �ʱ�ȭ : ��λ���!!!
	ClearListStandard();
	ClearListSang();
	ClearListAssembly();
	ClearListAssemblySang();
	ClearListAssemblyTotal();
	ClearListWeldType();
	ClearListWeldThick();

	switch( m_nWorkValue )
	{
		case 0 : 
			nErr = MakeToCreator();
			break;
		case 1 : 
			nErr = MakeToLoadFileXL();
			break;
	}

	if(nErr)	// ����� ����, �Ǵ� Error��
	{
		OnProgress(4);
		ClearListStandard();
		return nErr;
	}

	// �� ���ǥ�� ����, ����

	if(m_nMakeFlag & WELDMAKE_SANG)
	{
		OnProgress(0,90);
		OnProgressText(1, "�� ����ǥ");
		MakeListSang();
	}

	if(m_nMakeFlag & WELDMAKE_ASSEMBLY_SANG)
	{
		OnProgress(0,92);
		OnProgressText(1, "��������ũ�� ������ǥ");
		MakeListAssemblySang();
	}

	if(m_nMakeFlag & WELDMAKE_ASSEMBLY)
	{
		OnProgress(0,94);
		OnProgressText(1, "��������ũ�� ����ǥ");
		MakeListAssembly();
	}

	if(m_nMakeFlag & WELDMAKE_ASSEMBLY_TOTAL)
	{
		OnProgress(0,96);
		OnProgressText(1, "��������ũ�� �Ѱ�����ǥ");
		MakeListAssemblyTotal();
	}

	if(m_nMakeFlag & WELDMAKE_WELDTYPE)
	{
		OnProgress(0,98);
		OnProgressText(1, "�������º� ����ǥ");
		MakeListWeldType();
	}

	if(m_nMakeFlag & WELDMAKE_WELDTHICK)
	{
		OnProgress(0,99);
		OnProgressText(1, "�����β��� ����ǥ");
		MakeListWeldThick();
	}

	OnProgress(0,100);
	OnProgress(4);
	return nErr;
}

void CWeldStandard::SetTStandard(CTStandard *pTStandard)
{
	m_pTStandard = pTStandard;
}

CTStandard* CWeldStandard::GetTStandard()
{
	return m_pTStandard;
}

void CWeldStandard::SetWorkValue(long nWorkValue,const CString sPath,DIMTYPE nDimType,long nMakeFlag)
{
	m_nWorkValue = nWorkValue;	// 0:����, 2:�׼����Ϸε�
	m_sPath		 = sPath;
	m_nDimType   = nDimType;
	m_nMakeFlag  = nMakeFlag;
}


#define ABORT(x) {if(m_bAbort == TRUE) return x;}

UINT CWeldStandard::MakeToCreator()
{
	// support progress
	CString sFmt;
	long TRange = 0,nCount = 0;
	for(long n = 0; n < m_pDB->GetGirdersu(); n++)	
		TRange += m_pDB->GetGirder(n)->GetQtySplice() + 1;

	//////////////////////////////////////////////////////////////////////
	// OPTION PAGE
	long nGirdersu = m_pDB->GetGirdersu();
	m_bAlertWeld = TRUE;
 	for(long nG = 0; nG < nGirdersu; nG++)
	{
		CPlateGirderApp* pGir = m_pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex* pBx = Finder.GetBxFirst(BX_SPLICE|BX_STT_GIRDER|BX_END_GIRDER);

		while( Finder.GetBxFixNext())
		{
			OnProgress(0,  (long)((double)++nCount/(double)TRange * 90.0));// support progress

			sFmt.Format("G%ld SP%ld ����",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddFlangeUpper(pBx, Finder.GetBxFixNext());		ABORT(1)
			AddFlangeLower(pBx, Finder.GetBxFixNext());		ABORT(1)
			AddFlangeWeb(pBx, Finder.GetBxFixNext());	ABORT(1)			

 			sFmt.Format("G%ld SP%ld ���򺸰���",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddHStiff(pBx, Finder.GetBxFixNext(),TRUE);		ABORT(1)
			AddHStiff(pBx, Finder.GetBxFixNext(),FALSE);	ABORT(1)

			sFmt.Format("G%ld SP%ld ����������",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddVStiff(pBx, Finder.GetBxFixNext());		ABORT(1)	// �Ϲݺ� ����������, ������ ����������
			AddCrossBeamGirderVStiff(pBx, Finder.GetBxFixNext());	ABORT(1) // ���κ���ġ�� �ִ� �Ŵ����� ����������
			AddVBracingGirderVStiff(pBx, Finder.GetBxFixNext());	ABORT(1) // �����극�̽���ġ�� �ִ� �Ŵ����� ����������

			if(nG < nGirdersu-1)		
			{
				sFmt.Format("G%ld SP%ld ���κ�",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
				OnProgressText(1, sFmt);
				AddCrossBeam(pBx, Finder.GetBxFixNext());	ABORT(1)
			}

			if(nG < nGirdersu-1)		
			{
				sFmt.Format("G%ld SP%ld �����극�̽�",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
				OnProgressText(1, sFmt);
				AddVBracing(pBx, Finder.GetBxFixNext());	ABORT(1)
			}

			if(nG < nGirdersu-1)		
			{
				sFmt.Format("G%ld SP%ld ����극�̽�",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
				OnProgressText(1, sFmt);
				AddHBracing(pBx, Finder.GetBxFixNext());	ABORT(1)
			}

			if(pGir->m_pSangse->m_StudHori_Type != 1)
			{
				if(pGir->m_pSangse->m_StudHori_Type == 0)	// ������ ��Ŀ
					sFmt.Format("G%ld SP%ld ������ ��Ŀ",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
				if(pGir->m_pSangse->m_StudHori_Type == 2)	// ���ܿ����� ����
					sFmt.Format("G%ld SP%ld ���ܿ����� ����",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
				OnProgressText(1, sFmt);
				AddStud(pBx, Finder.GetBxFixNext());	ABORT(1)
			}
/*
			sFmt.Format("G%ld SP%ld ���κ�",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddStringer(pBx, Finder.GetBxFixNext());	ABORT(1)

			sFmt.Format("G%ld SP%ld �������κ�",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddWingBeam(pBx, Finder.GetBxFixNext(), TRUE);	ABORT(1)
			AddWingBeam(pBx, Finder.GetBxFixNext(), FALSE);	ABORT(1)

			sFmt.Format("G%ld SP%ld ������",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddEndBeam(pBx, Finder.GetBxFixNext(), TRUE);	ABORT(1)
			AddEndBeam(pBx, Finder.GetBxFixNext(), FALSE);	ABORT(1)
*/

			pBx = Finder.GetBxNext();
		}

		pBx = Finder.GetBxFirst(BX_JACKUP);
		while(pBx)
		{
			sFmt.Format("G%ld SP%ld ���������",nG+1+m_pDB->GetNumberStartGirder(),Finder.GetCurrentCount()+1+m_pDB->GetNumberStartSplice());
			OnProgressText(1, sFmt);
			AddJackUpStiff(pBx);
			pBx = Finder.GetBxNext();
		}
	}
	
	return 0;
}

void CWeldStandard::MakeListSang()
{
	CPtrList TmpList;

	POSITION pos = m_ListStandard.GetHeadPosition();

	DWORD exceptionFlag = 0x00004;
/*	// MARK_SECTION�̾�� �ϳ� ARoad������ SECTION MARK�� ����ϹǷ� �ε����ϰ� MARK_ASSEM���� ���
	if(!IsMakeMark(CMarkSerial::MARK_BUJE))		 exceptionFlag |= (0x004200|0x080000);
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM))	 exceptionFlag |= 0x000400;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM))	 exceptionFlag |= 0x000100;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_MID)) exceptionFlag |= 0x000080;
	if(!IsMakeMark(CMarkSerial::MARK_ASSEM_BIG)) exceptionFlag |= 0x000040;
*/
	while(pos)
	{
		CWeldRecord *pRec = (CWeldRecord*)m_ListStandard.GetNext(pos);

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CWeldRecord(pRec));

		if(m_bAbort == TRUE) return;
	}

	ShellSortRecord(&TmpList, &CWeldRecord::CompareSang);
	if(m_bAbort == TRUE) return;

	AhPtrListDelete(m_ListSang,(CWeldRecord*)0);

	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CWeldRecord *pRec = (CWeldRecord*)TmpList.GetNext(pos);
		m_ListSang.AddTail(pRec);
	}
}

void CWeldStandard::MakeListAssembly()
{
	CPtrList TmpList;

	POSITION pos = m_ListStandard.GetHeadPosition();
	// �Ŵ���ȣ,��������
	DWORD exceptionFlag = 0x00004;
	while(pos)
	{
		CWeldRecord *pRec = (CWeldRecord*)m_ListStandard.GetNext(pos);

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CWeldRecord(pRec));

		if(m_bAbort == TRUE) return;
	}

	ShellSortRecord(&TmpList, &CWeldRecord::CompareAssembly);
	if(m_bAbort == TRUE) return;

	AhPtrListDelete(m_ListAssembly,(CWeldRecord*)0);

	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CWeldRecord *pRec = (CWeldRecord*)TmpList.GetNext(pos);
		m_ListAssembly.AddTail(pRec);
	}
}

void CWeldStandard::MakeListAssemblySang()
{
	CPtrList TmpList;

	POSITION pos = m_ListStandard.GetHeadPosition();
	// �Ŵ���ȣ,��������
	DWORD exceptionFlag = 0x00004;
	while(pos)
	{
		CWeldRecord *pRec = (CWeldRecord*)m_ListStandard.GetNext(pos);

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CWeldRecord(pRec));

		if(m_bAbort == TRUE) return;
	}

	ShellSortRecord(&TmpList, &CWeldRecord::CompareAssemblySang);
	if(m_bAbort == TRUE) return;

	AhPtrListDelete(m_ListAssemblySang,(CWeldRecord*)0);

	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CWeldRecord *pRec = (CWeldRecord*)TmpList.GetNext(pos);
		m_ListAssemblySang.AddTail(pRec);
	}
}


void CWeldStandard::MakeListAssemblyTotal()
{
	CPtrList TmpList;

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x100|0x8000|0x10000|0x20000|0x40000));
	while(pos)
	{
		CWeldRecord *pRec = (CWeldRecord*)m_ListStandard.GetNext(pos);

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CWeldRecord(pRec));

		if(m_bAbort == TRUE) return;
	}

	ShellSortRecord(&TmpList, &CWeldRecord::CompareAssemblyTotal);
	if(m_bAbort == TRUE) return;

	AhPtrListDelete(m_ListAssemblyTotal,(CWeldRecord*)0);

	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CWeldRecord *pRec = (CWeldRecord*)TmpList.GetNext(pos);
		m_ListAssemblyTotal.AddTail(pRec);
	}
}

void CWeldStandard::MakeListWeldType()
{
	CPtrList TmpList;

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x00008|0x00010|0x00020|0x08000|0x10000|0x20000|0x40000));
	while(pos)
	{
		CWeldRecord *pRec = (CWeldRecord*)m_ListStandard.GetNext(pos);

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CWeldRecord(pRec));

		if(m_bAbort == TRUE) return;
	}

	ShellSortRecord(&TmpList, &CWeldRecord::CompareWeldType);
	if(m_bAbort == TRUE) return;

	AhPtrListDelete(m_ListWeldType,(CWeldRecord*)0);

	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CWeldRecord *pRec = (CWeldRecord*)TmpList.GetNext(pos);
		m_ListWeldType.AddTail(pRec);
	}
}

void CWeldStandard::MakeListWeldThick()
{
	CPtrList TmpList;

	POSITION pos = m_ListStandard.GetHeadPosition();
	DWORD exceptionFlag = (0xFFFFFFFF & ~(0x8000|0x10000|0x20000|0x40000));
	while(pos)
	{
		CWeldRecord *pRec = (CWeldRecord*)m_ListStandard.GetNext(pos);

		if(SameAndIncludeRecord(&TmpList,pRec,exceptionFlag) == FALSE)
			TmpList.AddTail(new CWeldRecord(pRec));

		if(m_bAbort == TRUE) return;
	}

	ShellSortRecord(&TmpList, &CWeldRecord::CompareWeldThick);
	if(m_bAbort == TRUE) return;

	AhPtrListDelete(m_ListWeldThick,(CWeldRecord*)0);

	pos = TmpList.GetHeadPosition();	
	while(pos) 
	{
		CWeldRecord *pRec = (CWeldRecord*)TmpList.GetNext(pos);
		m_ListWeldThick.AddTail(pRec);
	}
}


void CWeldStandard::ShellSortRecord(CPtrList* pList,int (CWeldRecord::*fcmp)(const CWeldRecord*) const) const
{
	CPtrArray  Arr;
	POSITION pos = pList->GetHeadPosition();
	while( pos ) Arr.Add( pList->GetNext(pos) );

	long nSize = Arr.GetSize();

	long i, j, k, h;
	CWeldRecord Rec;
	for(h = 1; h < nSize; h = 3*h+1);

	for(h /= 3; h > 0; h /=3)
		for(i = 0; i < h; i++)
			for(j = i+h; j < nSize; j += h)
			{
				Rec = *(CWeldRecord*)Arr[j];
				k = j;
				while(k > h - 1 && (((CWeldRecord*)Arr[k-h])->*fcmp)(&Rec) > 0)
				{
					*(CWeldRecord*)Arr[k] = *(CWeldRecord*)Arr[k-h];
					k -= h;
				}
				*(CWeldRecord*)Arr[k] = Rec;
			}

	Arr.RemoveAll();
}

UINT CWeldStandard::MakeToLoadFileXL()
{
	CXLControl	xl;
	if(xl.OpenXL(m_sPath) == FALSE)	return TRUE;

	CStringArray sArrReadXL;
	CString sLineData = _T("");
	CString sProcess  = _T("");
	long nRow	= 1;
	long nCol	= 0;
	long nColsu = 0;
		
	// Ÿ��Ʋ�� �о COLUMN�� ����
	for(nCol = 0; nCol < MAX_WELD_COLUMN; nCol++)
	{
		sLineData = xl.GetXL(0, nCol);
		if(!sLineData.IsEmpty()) nColsu++;
	}

	// DATA READING : ���ӵ����� ������ �ƴ� ������....
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
		AddWeldXLFileLine(sArrReadXL);
		sProcess.Format("Excel ���� �д��� [%ld��] : %s %s - %s , %s",nRow,sArrReadXL[0]
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


void CWeldStandard::AddWeldXLFileLine(const CStringArray& sArr)
{
	if(sArr[0].IsEmpty()) return;

	long nGirder = atoi(sArr[0].Right(sArr[0].GetLength()-1)) - m_pDB->GetNumberStartGirder();
	long nSplice = atoi(sArr[1].Right(sArr[1].GetLength()-1)) - m_pDB->GetNumberStartSplice();

	CWeldRecord * pRec = AddWeldRecord();

	// �Ŵ���ȣ,����������ȣ,������ȣ
	pRec->m_nGirder	= nGirder - 1;
	pRec->m_nSplice	= nSplice - 1;
	pRec->SetStrEleName(sArr[2]);
	pRec->SetStrSubEleName(sArr[3]);
	// �⺻�ܸ�,�ܸ鸶ũ,��������ũ,���縶ũ
	long nCol = 4;
	pRec->m_sMarkSectionBasic	= sArr[nCol++];		
	pRec->m_sMarkSection		= sArr[nCol++];
	pRec->m_sMarkAssem			= sArr[nCol++];
	pRec->m_sMarkBuje			= sArr[nCol];
	// �β�,�ʺ�,����,����
	nCol = 8;
	pRec->m_vJewon.y	= atof(sArr[nCol++]);
	pRec->m_vJewon.x	= atof(sArr[nCol++]);
	pRec->m_vJewon.z	= atof(sArr[nCol++]);
	pRec->m_nQty		= atoi(sArr[nCol]);

	//���Ӻ�������,���Ӻ�������,���Ӻ���β�,��������,���뵵
	nCol = 17;
	pRec->SetStrEleConnectName(sArr[nCol++]);
	pRec->SetStrSubEleConnectName(sArr[nCol++]);
	pRec->m_dThickConnect	= atof(sArr[nCol++]);
	pRec->m_sRefDWG			= sArr[nCol++];
	pRec->m_sUsage			= sArr[nCol];
	// ��������ũ,��������ũ(String)
	nCol = 22;
	pRec->m_sMarkGrandAssem	= sArr[nCol++];
	pRec->m_sMarkMidAssem	= sArr[nCol];
	// �����ȣ,������ȣ,��������ȣ,��������ȣ,�⺻�ܸ��ȣ,�ܸ��ȣ(long)
	nCol = 24;
	pRec->m_nMarkBuje			= atoi(sArr[nCol++]);
	pRec->m_nMarkAssem			= atoi(sArr[nCol++]);
	pRec->m_nMarkMidAssem		= atoi(sArr[nCol++]);
	pRec->m_nMarkGrandAssem		= atoi(sArr[nCol++]);
	pRec->m_nMarkSectionBasic	= atoi(sArr[nCol++]);
	pRec->m_nMarkSection		= atoi(sArr[nCol]);
	// ������ȣ,�����ȣ(����),������ȣ(����)
	nCol = 30;
// 	pRec->m_nSectionNumber		  = atoi(sArr[nCol++]);
// 	pRec->m_nMarkBujeConnect	  = atoi(sArr[nCol++]);
// 	pRec->m_nSectionNumberConnect = atoi(sArr[nCol]);

	// ��������,�����β�,������,�������
	nCol = 12;
	long nIdxWeld = m_pDB->GetIndexBaseWeld(pRec->m_nMarkBuje,pRec->m_nMarkBujeConnect,NULL);
	CSteelWeld		*pWeld		= m_pDataManage->GetSteelWeld();
	CSteelWeldData *pBaseWeld = pWeld->GetBaseWeldByIndex(nIdxWeld);

	//
	pRec->m_dThickWeld	 = atof(sArr[13]);
	pRec->m_dLengthWeld	 = atof(sArr[14]);
	pRec->m_nWeldType	 = pWeld->GetItemValue(sArr[12]);
	pRec->m_nImproveType = pWeld->GetItemValue(sArr[15]);
	pRec->m_nLayType	 = pWeld->GetItemValue(sArr[16]);

	pRec->m_sWeldType	 = pWeld->GetWeldTypeByIndex(pRec->m_nWeldType);
	pRec->m_sImproveType = pWeld->GetImproveTypeByIndex(pRec->m_nImproveType);
	pRec->m_sLayType	 = pWeld->GetLayTypeByIndex(pRec->m_nLayType);
}
