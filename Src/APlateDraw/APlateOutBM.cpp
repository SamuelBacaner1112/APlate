// APlateOutBM.cpp: implementation of the CAPlateOutBM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APlateDraw.h"
#include "../PlateGangje\Gangje.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BM_COLUMN_SANG		  11
#define BM_COLUMN_STANDARD	  11
#define BM_COLUMN_CAR		  11
#define BM_COLUMN_CARTOTAL     3
#define BM_COLUMN_SECTION	   9
#define BM_COLUMN_THICK		   6
#define BM_COLUMN_BUJE		   8
#define BM_COLUMN_BUJE_TOTAL   3
#define BM_COLUMN_DEDUCTBUJE   8
#define BM_COLUMN_DEDUCTSANG  11
#define BM_COLUMN_GIRDER_TOTAL 2
#define BM_COLUMN_SIZETYPE_TOTAL 4

#define PAINT_COLUMN_SANG 15
#define PAINT_COLUMN_BUJE 12
#define PAINT_COLUMN_TOTAL 2
#define PAINT_COLUMN_BOLT  6

#define WELD_COLUMN_SANG   16
#define WELD_COLUMN_TYPE    7
#define WELD_COLUMN_THICK   5

#define MPE_BM_SANG		         100
#define MPE_BM_SANG_TITLE	     150
#define MPE_BM_STANDARD          200
#define MPE_BM_STANDARD_TITLE    250
#define MPE_BM_CAR   			 300
#define MPE_BM_CAR_TITLE		 350
#define MPE_BM_CARTOTAL			 400
#define MPE_BM_CARTOTAL_TITLE    450
#define MPE_BM_SECTION			 500
#define MPE_BM_SECTION_TITLE	 550
#define MPE_BM_THICK			 600
#define MPE_BM_THICK_TITLE		 650
#define MPE_BM_BUJE				 700
#define MPE_BM_BUJE_TITLE		 750
#define MPE_BM_BUJETOTAL		 800
#define MPE_BM_BUJETOTAL_TITLE	 850
#define MPE_BM_DEDUCTBUJE		 900
#define MPE_BM_DEDUCTBUJE_TITLE	 950
#define MPE_BM_DEDUCTSANG		1000
#define MPE_BM_DEDUCTSANG_TITLE	1050
#define MPE_BM_GIRDER			1100
#define MPE_BM_GIRDER_TITLE		1150
#define MPE_BM_ILBAN			1200
#define MPE_BM_ILBAN_TITLE		1250
#define MPE_BM_SIZETYPE			1260
#define MPE_BM_SIZETYPE_TITLE	1270

#define MPE_PAINT_SANG			1300
#define MPE_PAINT_SANG_TITLE	1350
#define MPE_PAINT_BUJE			1400
#define MPE_PAINT_BUJE_TITLE	1450
#define MPE_PAINT_TOTAL			1500
#define MPE_PAINT_TOTAL_TITLE	1550
#define MPE_PAINT_BOLT			1600
#define MPE_PAINT_BOLT_TITLE	1650

#define MPE_WELD_SANG       	1700
#define MPE_WELD_SANG_TITLE  	1750
#define MPE_WELD_TYPE       	1800
#define MPE_WELD_TYPE_TITLE 	1850
#define MPE_WELD_THICK      	1900
#define MPE_WELD_THICK_TITLE	1950

CAPlateOutBM::CAPlateOutBM()
{
	//강재 제작표
	m_nPageSang			= 0;
	m_nPageSectionType	= 0;
	m_nPageCar   		= 0;
	m_nPageBuje			= 0;
	m_nPageBujeTotal    = 0;
	m_nPageSizeTypeTotal = 0;
	m_nPageDeductSang	= 0;
	m_nPageDeductBuje	= 0;
	m_nPageEtc			= 0;
	m_nPageStandard     = 0;
	//도장집계표
	m_nPagePaintBuje = 0;
	m_nPagePaintSang = 0;
	m_nPagePaintEtc  = 0;
	//용접집계표
	m_nPageWeldSang  = 0;
	m_nPageWeldType  = 0;
	m_nPageWeldThick = 0;
}

CAPlateOutBM::~CAPlateOutBM()
{
}

void CAPlateOutBM::DrawOnePage()
{
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nPage    = pStd->m_nCurPage;
	long nprePage = m_nSttPage;
	long nSubPage = nPage - nprePage;
	double Scale  = GetScale();
	
	CDomyun *pDomP = pStd->GetDomyun(nPage);
	pDomP->SetScaleDim(Scale);
	CDomyun *pDom  = new CDomyun(pDomP);

	// 도각
	pPageMng->SetProgressData("도각 출력중...",0);
	pStd->GetDogak()->Add(pDom,nPage,Scale,pOpt->m_szDGKName,pOpt->m_sHead_DomyunNumber,pOpt->m_bTitleDogak ? GetTitle() : "",0,IsDxfOut() ? pOpt->m_szXRefPath : "");	
	pStd->GetDogak()->AddTitle(pDom,pDB->m_sNameBridge,GetTitle(), _T(""),FALSE);
	*pDomP << *pDom;
	
	long nPageGangje = m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje+m_nPageBujeTotal+m_nPageSizeTypeTotal+m_nPageDeductSang+m_nPageDeductBuje+m_nPageEtc;
	long nSubPageGangje = m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje+m_nPageBujeTotal+m_nPageSizeTypeTotal+m_nPageDeductSang+m_nPageDeductBuje;
	long nPagePaint = nPageGangje + m_nPagePaintSang + m_nPagePaintBuje + m_nPagePaintEtc;
	long nSubPagePaint = nSubPageGangje + m_nPageEtc + m_nPagePaintSang + m_nPagePaintBuje;	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//강재집계표
	if(nSubPage<=m_nPageSang)
		AddPageBMSang(pDom,nSubPage);  //1 상세
	else if(nSubPage<=m_nPageSang+m_nPageStandard)
		AddPageBMStandard(pDom, nSubPage-m_nPageSang); //2 표준집계표
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar)
		AddPageBMCar(pDom,nSubPage-m_nPageSang-m_nPageStandard);//3 차량
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType)
		AddPageBMSectionType(pDom,nSubPage-m_nPageSang-m_nPageStandard-m_nPageCar);//4 단면별
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje)
		AddPageBMBuje(pDom,nSubPage-m_nPageSang-m_nPageStandard-m_nPageCar-m_nPageSectionType);//5 부재별 
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje+m_nPageBujeTotal)
		AddPageBMBujeTotal(pDom,nSubPage-m_nPageSang-m_nPageStandard-m_nPageCar-m_nPageSectionType-m_nPageBuje);//6 부재별 총괄
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje+m_nPageBujeTotal+m_nPageSizeTypeTotal)
		AddPageBMSizeTypeTotal(pDom,nSubPage-m_nPageSang-m_nPageStandard-m_nPageCar-m_nPageSectionType-m_nPageBuje-m_nPageBujeTotal);//7 부재종류별 총괄
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje+m_nPageBujeTotal+m_nPageSizeTypeTotal+m_nPageDeductSang)
		AddPageBMDeductSang(pDom,nSubPage-m_nPageSang-m_nPageStandard-m_nPageCar-m_nPageSectionType-m_nPageBuje-m_nPageBujeTotal-m_nPageSizeTypeTotal);//8 상세별 공제
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje+m_nPageBujeTotal+m_nPageSizeTypeTotal+m_nPageDeductSang+m_nPageDeductBuje)
		AddPageBMDeductBuje(pDom,nSubPage-m_nPageSang-m_nPageStandard-m_nPageCar-m_nPageSectionType-m_nPageBuje-m_nPageBujeTotal-m_nPageSizeTypeTotal-m_nPageDeductSang);//8부재별 공제
	else if(nSubPage<=m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType+m_nPageBuje+m_nPageBujeTotal+m_nPageSizeTypeTotal+m_nPageDeductSang+m_nPageDeductBuje+m_nPageEtc)
		AddPageBMEtc(pDom,nSubPage-m_nPageSang-m_nPageStandard-m_nPageCar-m_nPageSectionType-m_nPageBuje-m_nPageBujeTotal-m_nPageSizeTypeTotal-m_nPageDeductSang-m_nPageDeductBuje);//9 기타(차량적재총괄+부재별총괄+상세별총괄)
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//도장집계표
	else if(nSubPage <= nPageGangje + m_nPagePaintSang)
		AddPageBMPaintSang(pDom,nSubPage - nSubPageGangje - m_nPageEtc);// 10 도장상세

	else if(nSubPage <= nPageGangje + m_nPagePaintSang + m_nPagePaintBuje)
		AddPageBMPaintBuje(pDom,nSubPage - nSubPageGangje - m_nPageEtc - m_nPagePaintSang);//11 도장 부재별

	else if(nSubPage <= nPageGangje + m_nPagePaintSang + m_nPagePaintBuje + m_nPagePaintEtc)
		AddPageBMPaintEtc(pDom,nSubPage - nSubPageGangje - m_nPageEtc - m_nPagePaintSang - m_nPagePaintBuje); //12 도장 기타
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//용접집계표
	else if(nSubPage <= nPagePaint + m_nPageWeldSang)
		AddPageWeldSang(pDom, nSubPage - nSubPagePaint - m_nPagePaintEtc); //13 용접 상세
	else if(nSubPage <= nPagePaint + m_nPageWeldSang + m_nPageWeldType)
		AddPageWeldType(pDom, nSubPage - nSubPagePaint - m_nPagePaintEtc - m_nPageWeldSang);//14 용접 타입별
	else if(nSubPage <= nPagePaint + m_nPageWeldSang + m_nPageWeldType + m_nPageWeldThick)
		AddPageWeldThick(pDom, nSubPage - nSubPagePaint - m_nPagePaintEtc - m_nPageWeldSang - m_nPageWeldType); //15 용접 두께별
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	pPageMng->SetProgressData("출력완료",100);
	*pDomP << *pDom;
	delete pDom;
}

long CAPlateOutBM::GetPageSu()
{
 	CAPlateDrawStd *pStd= (CAPlateDrawStd*)m_pStd;
	COptionStd	  *pOpt	= pStd->m_pDomOptionStd;
	
	m_nPage    = 0;
	m_nPageEtc = 0;
	m_nPagePaintEtc=0;
	//강재
	m_MatrixSang.SetSize(0,0);
	m_MatrixCar.SetSize(0,0);
	m_MatrixSectionType.SetSize(0,0);
	m_MatrixCarTotal.SetSize(0,0);
	m_MatrixThick.SetSize(0,0);
	m_MatrixBuje.SetSize(0,0);
	m_MatrixBujeTotal.SetSize(0,0);
	m_MatrixDeductSang.SetSize(0,0);
	m_MatrixDeductBuje.SetSize(0,0);
	m_MatrixStandard.SetSize(0,0);
	m_MatrixGirderTotal.SetSize(0,0);
	//도장	
	m_MatrixPaintSang.SetSize(0,0);
	m_MatrixPaintBuje.SetSize(0,0);
	m_MatrixPaintTotal.SetSize(0,0);
	m_MatrixPaintBolt.SetSize(0,0);
	//용접
	m_MatrixWeldSang.SetSize(0,0);
	m_MatrixWeldType.SetSize(0,0);
	m_MatrixWeldThick.SetSize(0,0);

	//강재
	BOOL bSang		= (pOpt->m_BM_OutSelect & MAKE_BM_SANG)			? TRUE : FALSE;
	BOOL bStandard  = (pOpt->m_BM_OutSelect & MAKE_BM_STDANDARD_SANG)	? TRUE : FALSE;
	BOOL bCar		= (pOpt->m_BM_OutSelect & MAKE_BM_CAR)			? TRUE : FALSE;
	BOOL bSection	= (pOpt->m_BM_OutSelect & MAKE_BM_SECTIONTYPE)	? TRUE : FALSE;
	BOOL bCarTotal	= (pOpt->m_BM_OutSelect & MAKE_BM_CARTOTAL)		? TRUE : FALSE;
	BOOL bThick		= (pOpt->m_BM_OutSelect & MAKE_BM_THICK)		? TRUE : FALSE;
	BOOL bBuje		= (pOpt->m_BM_OutSelect & MAKE_BM_BUJE)			? TRUE : FALSE;
	BOOL bBujeTotal	= (pOpt->m_BM_OutSelect & MAKE_BM_BUJE_TOTAL)	? TRUE : FALSE;
	BOOL bGirderTotal= (pOpt->m_BM_OutSelect & MAKE_BM_GIRDER_TOTAL)	? TRUE : FALSE;	
	BOOL bSizeTypeTotal= (pOpt->m_BM_OutSelect & MAKE_BM_BUJEKIND_TOTAL)	? TRUE : FALSE;	
	BOOL bDeductSang= (pOpt->m_BM_OutSelect & MAKE_BM_DEDUCT_SANG)	? TRUE : FALSE;
	BOOL bDeductBuje= (pOpt->m_BM_OutSelect & MAKE_BM_DEDUCT_BUJE)	? TRUE : FALSE;
	//도장	
	BOOL bPaintSang	= (pOpt->m_BM_PaintSelect & PAINTMAKE_SANG)		? TRUE : FALSE;
	BOOL bPaintTotal= (pOpt->m_BM_PaintSelect & PAINTMAKE_SUMMARY)	? TRUE : FALSE;
	BOOL bPaintBolt	= (pOpt->m_BM_PaintSelect & PAINTMAKE_BOLT_TOTAL)		? TRUE : FALSE;
	BOOL bPaintBuje	= (pOpt->m_BM_PaintSelect & PAINTMAKE_BUJE_SANG)		? TRUE : FALSE;
	//용접
	BOOL bWeldSang  = (pOpt->m_BM_WeldSelect & WELDMAKE_SANG)		? TRUE : FALSE;
	BOOL bWeldType	= (pOpt->m_BM_WeldSelect & WELDMAKE_WELDTYPE)	? TRUE : FALSE;
	BOOL bWeldThick	= (pOpt->m_BM_WeldSelect & WELDMAKE_WELDTHICK)	? TRUE : FALSE;

	//강재
	if(bSang)		MakeMatrixSang();
	if(bStandard)   MakeMatrixStandard();
	if(bCar)		MakeMatrixCar();
	if(bSection)	MakeMatrixSectionType();
	if(bBuje)		MakeMatrixBuje();
	if(bBujeTotal)	MakeMatrixBujeTotal();
	if(bGirderTotal) MakeMatrixGirderTotal();
	if(bSizeTypeTotal) MakeMatrixSizeTypeTotal();
	if(bDeductBuje)	MakeMatrixDeductBuje();
	if(bCarTotal)	MakeMatrixCarTotal();
	if(bThick)		MakeMatrixThick();
	if(bDeductSang)	MakeMatrixDeductSang();
	//도장
	if(bPaintSang)	MakeMatrixPaintSang();
	if(bPaintTotal)	MakeMatrixPaintTotal();
	if(bPaintBolt)	MakeMatrixPaintBolt();
	if(bPaintBuje)	MakeMatrixPaintBuje();
	//용접
	if(bWeldSang)	MakeMatrixWeldSang();
	if(bWeldType)	MakeMatrixWeldType();
	if(bWeldThick)	MakeMatrixWeldThick();

	if( // 강재집계표
		m_MatrixSang.GetRows()==0 && 
		m_MatrixStandard.GetRows()==0 && 
		m_MatrixCar.GetRows()==0 && 
		m_MatrixSectionType.GetRows()==0 && 
		m_MatrixCarTotal.GetRows()==0 && 
		m_MatrixThick.GetRows()==0 && 
		m_MatrixBuje.GetRows()==0 && 
		m_MatrixBujeTotal.GetRows()==0 && 
		m_MatrixGirderTotal.GetRows()==0 && 
		m_MatrixDeductSang.GetRows()==0 && 
		m_MatrixDeductBuje.GetRows()==0 && 
		// 도장집계표
	    m_MatrixPaintSang.GetRows()==0 && 
		m_MatrixPaintBuje.GetRows()==0 && 
		m_MatrixPaintTotal.GetRows()==0 && 
		m_MatrixPaintBolt.GetRows()==0 && 
		// 용접집계표
	    m_MatrixWeldSang.GetRows()==0 && 
		m_MatrixWeldType.GetRows()==0 && 
		m_MatrixWeldThick.GetRows()==0
	   )  
		m_nPage = 0;
	else
	{
		/////////////////////////////////////
		long nTableSu  = pOpt->m_BM_TableSu;
		long nColumnSu = pOpt->m_BM_LineSu;
		long nDataSu   = (long)(nTableSu*nColumnSu);
		//강재페이지
		long nBMSize = m_MatrixSang.GetRows();
		m_nPageSang = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageSang++;

		nBMSize = m_MatrixStandard.GetRows();
		m_nPageStandard = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageStandard++;

		nBMSize = m_MatrixCar.GetRows();
		m_nPageCar = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageCar++;

		nBMSize = m_MatrixSectionType.GetRows();
		m_nPageSectionType = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageSectionType++;

		nBMSize = m_MatrixBuje.GetRows();
		m_nPageBuje = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageBuje++;

		nBMSize = m_MatrixBujeTotal.GetRows();
		m_nPageBujeTotal = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageBujeTotal++;

		nBMSize = m_MatrixSizeTypeTotal.GetRows();
		m_nPageSizeTypeTotal = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageSizeTypeTotal++;		

		nBMSize = m_MatrixDeductBuje.GetRows();
		m_nPageDeductBuje = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageDeductBuje++;

		nBMSize = m_MatrixDeductSang.GetRows();
		m_nPageDeductSang = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageDeductSang++;

		/////////////////////////////////////
		nTableSu  = pOpt->m_BM_Paint_TableSu;
		nColumnSu = pOpt->m_BM_Paint_LineSu;
		nDataSu   = (long)(nTableSu*nColumnSu);
		//도장페이지
		nBMSize = m_MatrixPaintSang.GetRows();
		m_nPagePaintSang = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPagePaintSang++;

		nBMSize = m_MatrixPaintBuje.GetRows();
		m_nPagePaintBuje = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPagePaintBuje++;
		
		/////////////////////////////////////
		nTableSu  = pOpt->m_BM_Weld_TableSu;
		nColumnSu = pOpt->m_BM_Weld_LineSu;
		nDataSu   = (long)(nTableSu*nColumnSu);
		//용접페이지
		nBMSize = m_MatrixWeldSang.GetRows();
		m_nPageWeldSang = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageWeldSang++;

		nBMSize = m_MatrixWeldType.GetRows();
		m_nPageWeldType = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageWeldType++;

		nBMSize = m_MatrixWeldThick.GetRows();
		m_nPageWeldThick = nBMSize/(nDataSu);
		if(nBMSize%nDataSu) m_nPageWeldThick++;

		
		//기타페이지
		//강재
		if(m_MatrixCarTotal.GetRows()>0 || m_MatrixThick.GetRows()>0 || m_MatrixGirderTotal.GetRows()>0)
			m_nPageEtc = 1;
		//도장
		if(m_MatrixPaintTotal.GetRows()>0 || m_MatrixPaintBolt.GetRows()>0)
			m_nPagePaintEtc = 1;

		m_nPage = m_nPageSang+m_nPageStandard+m_nPageCar+m_nPageSectionType
			      +m_nPageBuje+m_nPageBujeTotal+m_nPageSizeTypeTotal
			      +m_nPageDeductBuje+m_nPageDeductSang+m_nPageEtc
			      +m_nPagePaintSang+m_nPagePaintBuje+m_nPagePaintEtc
				  +m_nPageWeldSang+m_nPageWeldType+m_nPageWeldThick;
	}

	return m_nPage;
}

CString CAPlateOutBM::GetTitle()
{
 	CAPlateDrawStd	*pStd	= (CAPlateDrawStd*)m_pStd;
	COptionStd		*pOpt	= pStd->m_pDomOptionStd;

	long nPageSu = m_nPage;

	CString szTitle;
	if(nPageSu>1)
		szTitle.Format("강 재 재 료 표 (%d/%d)",pStd->m_nCurPage-m_nSttPage,nPageSu);
	else
		szTitle.Format("강 재 재 료 표");

	return szTitle;
}

double CAPlateOutBM::GetScale()
{
	return 30;
}

void CAPlateOutBM::AddPageBMSang(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixSang.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixSang.GetRows())
			break;
		if(nEndRow >= m_MatrixSang.GetRows())
			nEndRow = m_MatrixSang.GetRows() - 1;
		DrawBMSang(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_SANG+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_SANG+i, "상세집계");
		*pDomP << Dom;

		// TITLE
		if (i ==0)
		{
			CString str;
			str.Format(" 상 세 집 계 표 : %d/%d",nSubPage,m_nPageSang);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_SANG_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_SANG_TITLE, "상세집계타이틀");
			*pDomP << Dom;
		}
	}
}

void CAPlateOutBM::AddPageBMCar(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixCar.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixCar.GetRows())
			break;
		if(nEndRow >= m_MatrixCar.GetRows())
			nEndRow = m_MatrixCar.GetRows() - 1;
		DrawBMCar(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_CAR+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_CAR+i, "차량적재집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 차 량 적 재 집 계 표 : %d/%d",nSubPage,m_nPageCar);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_CAR_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_CAR_TITLE, "차량적재집계타이틀");
			*pDomP << Dom;
		}
	
	}
}

void CAPlateOutBM::MakeMatrixSang()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_Sang;
	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixSang.SetSize(nRows,BM_COLUMN_SANG);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight   = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice && bContinue) 
		{
			m_MatrixSang.InsertRow(i);
			str = pBase.GetStringWeight(SubWeight,nRoundTot);     
			m_MatrixSang.SetTextMatrix(i, 3,"SUB TOTAL");
			m_MatrixSang.SetTextMatrix(i, 9,str);
			SubWeight = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubWeight += pRec->m_dTWeight*dFactorUnit;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		}

		m_MatrixSang.SetTextMatrix(i, 0,pRec->GetStrGirder(pDB->GetNumberStartGirder()));
		m_MatrixSang.SetTextMatrix(i, 1,pRec->GetStrSplice(pDB->GetNumberStartSplice()));
		m_MatrixSang.SetTextMatrix(i, 2,pRec->GetStrEleName());
		m_MatrixSang.SetTextMatrix(i, 3,pRec->GetStrSubEleName());
		m_MatrixSang.SetTextMatrix(i, 4,pRec->GetStrThick());
		m_MatrixSang.SetTextMatrix(i, 5,pRec->GetStrWidth());
		m_MatrixSang.SetTextMatrix(i, 6,pRec->GetStrLength());
		m_MatrixSang.SetTextMatrix(i, 7,pRec->GetStrQty());
		m_MatrixSang.SetTextMatrix(i, 8,pRec->GetStrUWeight(nRoundUnit));
		m_MatrixSang.SetTextMatrix(i, 9,pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixSang.SetTextMatrix(i,10,pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}

	i = m_MatrixSang.GetRows()-1;
	m_MatrixSang.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight, nRoundTot);
	m_MatrixSang.SetTextMatrix(i+1, 3,"SUB TOTAL");
	m_MatrixSang.SetTextMatrix(i+1, 9,str);
	//
	i = m_MatrixSang.GetRows()-1;
	m_MatrixSang.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight, nRoundTot);
	m_MatrixSang.SetTextMatrix(i+1, 3,"GRAND TOTAL");
	m_MatrixSang.SetTextMatrix(i+1, 9,str);
}

void CAPlateOutBM::DrawBMSang(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMSangTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_SANG);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nGirMergeStt = 0;
	long nEleMergeStt = 0;
	CString sNumGir   = _T("");
	CString sEleName  = _T("");
	CString sNextName = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_SANG; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=4 && j<=9)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixSang.GetTextMatrix(i,j), nAlign);
		}

		sNumGir  = m_MatrixSang.GetTextMatrix(i,0);
		if(i==nEndRow || sNumGir.IsEmpty())
		{
			if(nGirMergeStt<nGridRow)
			{
				Dom.SetMergeRow(nGirMergeStt,nGridRow,0);
				Dom.SetMergeRow(nGirMergeStt,nGridRow,1);
			}
			nGirMergeStt = nGridRow+1;
		}

		sEleName  = m_MatrixSang.GetTextMatrix(i+0,2);
		sNextName = (i==nEndRow) ? _T("") : m_MatrixSang.GetTextMatrix(i+1,2);
		if(i==nEndRow || sEleName!=sNextName )
		{
			if(nEleMergeStt<nGridRow)
				Dom.SetMergeRow(nEleMergeStt,nGridRow,2);
			nEleMergeStt = nGridRow+1;
			if(sNextName.IsEmpty()) 
				nEleMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMSangTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_SANG);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();

	Dom.SetTextMatrix( 0,  0, "거더\n번호");
	Dom.SetTextMatrix( 0,  1, "현장\n이음");
	Dom.SetTextMatrix( 0,  2, "종 류");
	Dom.SetTextMatrix( 0,  3, "부재명");
	Dom.SetTextMatrix( 0,  4, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  5, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  6, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  7, "수량\n(개)");
	Dom.SetTextMatrix( 0,  8, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0,  9, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0, 10, "재질");

	Dom.Draw();
	*pDomP << Dom;
}

// 차량적재 집계표
void CAPlateOutBM::DrawBMCar(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMCarTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_CAR);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nGirMergeStt = 0;
	long nEleMergeStt = 0;
	CString sNumGir   = _T("");
	CString sEleName  = _T("");
	CString sNextName = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_CAR; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=4 && j<=9)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixCar.GetTextMatrix(i,j), nAlign);
		}
		sNumGir  = m_MatrixCar.GetTextMatrix(i,0);
		if(i==nEndRow || sNumGir.IsEmpty())
		{
			if(nGirMergeStt<nGridRow)
			{
				Dom.SetMergeRow(nGirMergeStt,nGridRow,0);
				Dom.SetMergeRow(nGirMergeStt,nGridRow,1);
			}
			nGirMergeStt = nGridRow+1;
		}

		sEleName  = m_MatrixCar.GetTextMatrix(i+0,2);
		sNextName = (i==nEndRow) ? _T("") : m_MatrixCar.GetTextMatrix(i+1,2);
		if(i==nEndRow || sEleName!=sNextName )
		{
			if(nEleMergeStt<nGridRow)
				Dom.SetMergeRow(nEleMergeStt,nGridRow,2);
			nEleMergeStt = nGridRow+1;
			if(sNextName.IsEmpty()) 
				nEleMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMCarTitle(CDomyun *pDomP, CDPoint Base)
{
	DrawBMSangTitle(pDomP,Base);
}

void CAPlateOutBM::MakeMatrixCar()
{	
	
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_Car;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixCar.SetSize(nRows,BM_COLUMN_CAR);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice && bContinue) 
		{
			m_MatrixCar.InsertRow(i);
			str = pBase.GetStringWeight(SubWeight,nRoundTot);     
			m_MatrixCar.SetTextMatrix(i, 3,"SUB TOTAL");
			m_MatrixCar.SetTextMatrix(i, 9,str);
			SubWeight = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubWeight += pRec->m_dTWeight*dFactorUnit;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		}

		m_MatrixCar.SetTextMatrix(i, 0,pRec->GetStrGirder(pDB->GetNumberStartGirder()));
		m_MatrixCar.SetTextMatrix(i, 1,pRec->GetStrSplice(pDB->GetNumberStartSplice()));
		m_MatrixCar.SetTextMatrix(i, 2,pRec->GetStrEleName());
		m_MatrixCar.SetTextMatrix(i, 3,pRec->GetStrSubEleName());
		m_MatrixCar.SetTextMatrix(i, 4,pRec->GetStrThick());
		m_MatrixCar.SetTextMatrix(i, 5,pRec->GetStrWidth());
		m_MatrixCar.SetTextMatrix(i, 6,pRec->GetStrLength());
		m_MatrixCar.SetTextMatrix(i, 7,pRec->GetStrQty());
		m_MatrixCar.SetTextMatrix(i, 8,pRec->GetStrUWeight(nRoundUnit));
		m_MatrixCar.SetTextMatrix(i, 9,pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixCar.SetTextMatrix(i,10,pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	
	i = m_MatrixCar.GetRows()-1;
	m_MatrixCar.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixCar.SetTextMatrix(i+1, 3,"SUB TOTAL");
	m_MatrixCar.SetTextMatrix(i+1, 9,str);
	//
	i = m_MatrixCar.GetRows()-1;
	m_MatrixCar.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixCar.SetTextMatrix(i+1, 3,"GRAND TOTAL");
	m_MatrixCar.SetTextMatrix(i+1, 9,str);
}

void CAPlateOutBM::AddPageBMSectionType(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixSectionType.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP); 
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixSectionType.GetRows())
			break;
		if(nEndRow >= m_MatrixSectionType.GetRows())
			nEndRow = m_MatrixSectionType.GetRows() - 1;
		DrawBMSectionType(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_SECTION+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_SECTION+i, "단면타입집계");	
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 단 면 타 입 별 집 계 표 : %d/%d",nSubPage,m_nPageSectionType);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_SECTION_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_SECTION_TITLE, "단면타입집계타이틀");
			*pDomP << Dom;
		}
	}
}

void CAPlateOutBM::DrawBMSectionType(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMSectionTypeTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(35), Dom.Always(18), Dom.Always(18),	
			Dom.Always(18), Dom.Always(18),	Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_SECTION);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow = 0;
	long nEleMergeStt = 0;
	CString sEleName  = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_SECTION; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=2 && j<=7)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixSectionType.GetTextMatrix(i,j), nAlign);
		}
		sEleName = m_MatrixSectionType.GetTextMatrix(i,0);
		if(i==nEndRow || sEleName.IsEmpty())
		{
			if(nEleMergeStt<nGridRow)
				Dom.SetMergeRow(nEleMergeStt,nGridRow,0);
			nEleMergeStt = nGridRow+1;
		}
		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMSectionTypeTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(35), Dom.Always(18), Dom.Always(18),	
			Dom.Always(18), Dom.Always(18),	Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_SECTION);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0, 0, "종 류");
	Dom.SetTextMatrix( 0, 1, "부재명");
	Dom.SetTextMatrix( 0, 2, "두께\n(mm)");
	Dom.SetTextMatrix( 0, 3, "너비\n(mm)");
	Dom.SetTextMatrix( 0, 4, "길이\n(mm)");
	Dom.SetTextMatrix( 0, 5, "수량\n(개)");
	Dom.SetTextMatrix( 0, 6, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0, 7, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0, 8, "재질");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixSectionType()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_SectionType;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixSectionType.SetSize(nRows,BM_COLUMN_SECTION);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && ( pRecPrev->GetStrEleName() != pRec->GetStrEleName()
 			         || ( pRecPrev->GetStrEleName()=="일반" && 
					    pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName()) )   && bContinue ) 
		{
			m_MatrixSectionType.InsertRow(i);
			str = pBase.GetStringWeight(SubWeight,nRoundTot);     
			m_MatrixSectionType.SetTextMatrix(i,1,"SUB TOTAL");
			m_MatrixSectionType.SetTextMatrix(i,7,str);
			SubWeight = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubWeight += pRec->m_dTWeight*dFactorUnit;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		}
		m_MatrixSectionType.SetTextMatrix(i,0,pRec->GetStrEleName());
		m_MatrixSectionType.SetTextMatrix(i,1,pRec->GetStrSubEleName());
		m_MatrixSectionType.SetTextMatrix(i,2,pRec->GetStrThick());
		m_MatrixSectionType.SetTextMatrix(i,3,pRec->GetStrWidth());
		m_MatrixSectionType.SetTextMatrix(i,4,pRec->GetStrLength());
		m_MatrixSectionType.SetTextMatrix(i,5,pRec->GetStrQty());
		m_MatrixSectionType.SetTextMatrix(i,6,pRec->GetStrUWeight(nRoundUnit));
		m_MatrixSectionType.SetTextMatrix(i,7,pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixSectionType.SetTextMatrix(i,8,pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	
	i = m_MatrixSectionType.GetRows()-1;
	m_MatrixSectionType.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixSectionType.SetTextMatrix(i+1, 1,"SUB TOTAL");
	m_MatrixSectionType.SetTextMatrix(i+1, 7,str);
	//
	i = m_MatrixSectionType.GetRows()-1;
	m_MatrixSectionType.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixSectionType.SetTextMatrix(i+1, 1,"GRAND TOTAL");
	m_MatrixSectionType.SetTextMatrix(i+1, 7,str);
}

void CAPlateOutBM::DrawBMCarTotal(CDomyun *pDomP, CDPoint Base)
{
	if(m_MatrixCarTotal.GetRows()==0) return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CGridDomyunEx Dom(pDomP);
	DrawBMCarTotalTitle(&Dom,Base);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = { Dom.Always(10), Dom.Always(40), Dom.Always(30)}; 
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_CARTOTAL);
	Dom.SetRowCount(m_MatrixCarTotal.GetRows());
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<BM_COLUMN_CARTOTAL; i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);
	
	CString strGir = _T("");
	CString strNextGir = _T("");
	long rowmin = 0;
	long nRow = m_MatrixCarTotal.GetRows();
	for(i=0; i<nRow; i++)
	{
		for(long j=0; j<BM_COLUMN_CARTOTAL; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j==2)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(i,j,m_MatrixCarTotal.GetTextMatrix(i,j), nAlign);
		}
		strGir = m_MatrixCarTotal.GetTextMatrix(i,0);
		strNextGir = (i==nRow-1) ? _T("") : m_MatrixCarTotal.GetTextMatrix(i+1,0);
		if(i==nRow || strGir != strNextGir)
		{
			Dom.SetMergeRow(rowmin, i, 0);
			rowmin = i+1;
		}
	}
	Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
	Dom.Draw();
	Dom.Move(pMove->GetPoint(MPE_BM_CARTOTAL));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_CARTOTAL, "차량적재총괄집계");
	*pDomP << Dom;

	// Title
	CString str;
	str.Format(" 차 량 적 재 총 괄 집 계 표");
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
	Dom.Move(pMove->GetPoint(MPE_BM_CARTOTAL_TITLE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_CARTOTAL_TITLE, "차량적재총괄타이틀");
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMCarTotalTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = { Dom.Always(10), Dom.Always(40), Dom.Always(30)}; 
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_CARTOTAL);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "거더\n번호");
	Dom.SetTextMatrix( 0,  1, "현장\n이음");
	Dom.SetTextMatrix( 0,  2, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixCarTotal()
{	
	/*
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	  *pDB	  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CPtrList *pList = &pBMStd->m_BMList_CarTotal;
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();
	
	if(pList->GetCount()==0) return;

	m_MatrixCarTotal.SetSize(nRows,BM_COLUMN_CARTOTAL);

	POSITION pos = pList->GetHeadPosition();
	CBMRecord * pRecPrev = NULL;
	double SubWeight   = 0;
	double TotalWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;
	CString str        = _T("");

	for(long i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		
		if(pRecPrev && pRecPrev->GetStrGirder()!=pRec->GetStrGirder())
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		//
		if(pBMStd->IsCarBuje(pRec))
		{
			str.Format("G%d",pRec->m_nGirder+1+pDB->GetNumberStartGirder());
			m_MatrixCarTotal.SetTextMatrix(i, 0, str);
			str.Format("S%d",pRec->m_nSplice+1+pDB->GetNumberStartSplice());
			m_MatrixCarTotal.SetTextMatrix(i, 1, str);
		}
		else
		{
			m_MatrixCarTotal.SetTextMatrix(i, 1, pRec->GetStrEleName());
		}
		m_MatrixCarTotal.SetTextMatrix(i, 2, pRec->GetStrTWeight(bUnitTon, nRoundTot));
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	i = m_MatrixCarTotal.GetRows()-1;
	m_MatrixCarTotal.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixCarTotal.SetTextMatrix(i+1, 1,"GRAND TOTAL");
	m_MatrixCarTotal.SetTextMatrix(i+1, 2,str);
	*/
}

void CAPlateOutBM::AddPageBMEtc(CDomyun *pDomP, long nSubPage)
{
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	DrawBMCarTotal(&Dom,Base);
	Base.x = pDomP->Always(200);
	DrawBMThick(&Dom,Base);
	Base.x = pDomP->Always(420);
	DrawBMGirderTotal(&Dom,Base);

	*pDomP << Dom;
}


void CAPlateOutBM::DrawBMThick(CDomyun *pDomP, CDPoint Base)
{
	if(m_MatrixThick.GetRows()==0) return;
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));
	CGridDomyunEx Dom(pDomP);
	DrawBMThickTitle(&Dom,Base);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] =  { Dom.Always(30), Dom.Always(30), Dom.Always(18), Dom.Always(18),
									Dom.Always(25),	Dom.Always(30)};
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_THICK);
	Dom.SetRowCount(m_MatrixThick.GetRows());
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	CString sNumMat   = _T("");
	CString sNumMatNext   = _T("");
	long nRow = m_MatrixThick.GetRows();
	long rowmin = 0;
	for(i=0; i<nRow; i++)
	{
		for(long j=0; j<BM_COLUMN_THICK; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=1)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(i,j,m_MatrixThick.GetTextMatrix(i,j), nAlign);
		}
		sNumMat = m_MatrixThick.GetTextMatrix(i,0);
		sNumMatNext = (i==nRow-1) ? _T("") : m_MatrixThick.GetTextMatrix(i+1,0);
		if(sNumMat!=sNumMatNext )
		{

			Dom.SetMergeRow(rowmin, i, 0);
			rowmin = i+1;
		}
	}
	Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
	Dom.Draw();
	Dom.Move(pMove->GetPoint(MPE_BM_THICK));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_THICK, "강종별두께별집계");
	*pDomP << Dom;

	// Title
	CString str;
	str.Format(" 강 종 별 두 께 별 집 계 표");
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	BaseTitle.x = pDomP->Always(200);
	Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
	Dom.Move(pMove->GetPoint(MPE_BM_THICK_TITLE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_THICK_TITLE, "강종별두께별집계타이틀");
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMThickTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] =  { Dom.Always(30), Dom.Always(30), Dom.Always(18), Dom.Always(18),
									Dom.Always(25),	Dom.Always(30)};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_THICK);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "재질");
	Dom.SetTextMatrix( 0,  1, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  2, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  3, "수량\n(개)");
	Dom.SetTextMatrix( 0,  4, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0,  5, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixThick()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_Thick;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixThick.SetSize(nRows,BM_COLUMN_THICK);

	POSITION pos = pList->GetHeadPosition();
	CBMRecord * pRecPrev = NULL;
	BOOL bContinue     = TRUE;
	double SubWeight   = 0;
	double TotalWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;
	CString str        = _T("");
	CString szTemp = "";
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if(pRecPrev && pRecPrev->GetStrMaterial()!=pRec->GetStrMaterial() && bContinue)
		{
			m_MatrixThick.InsertRow(i);
			str = pBase.GetStringWeight(SubWeight,nRoundTot);     
			m_MatrixThick.SetTextMatrix(i, 0,"SUB TOTAL");
			m_MatrixThick.SetTextMatrix(i, 5,str);
			SubWeight = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubWeight += pRec->m_dTWeight*dFactorUnit;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		}

		m_MatrixThick.SetTextMatrix(i, 0,pRec->GetStrMaterial());
		// 두께
		if(pRec->m_nKindName==MATERIAL_KIND_BEAM)
			szTemp.Format("%sX%s", pRec->GetStrWidth(),	pRec->GetStrThick());
		else
			szTemp = pRec->GetStrThick();

		m_MatrixThick.SetTextMatrix(i, 1,szTemp);
		if(pRec->m_nKindName==MATERIAL_KIND_PLATE || pRec->m_nKindName==MATERIAL_KIND_BEAM)
		{
			m_MatrixThick.SetTextMatrix(i, 2, "");		// 길이
			m_MatrixThick.SetTextMatrix(i, 3, "");		// 수량
		}
		else
		{
			m_MatrixThick.SetTextMatrix(i, 2,pRec->GetStrLength());		// 길이
			m_MatrixThick.SetTextMatrix(i, 3,pRec->GetStrQty());		// 수량
		}
//		m_MatrixThick.SetTextMatrix(i, 2,pRec->GetStrLength());
//		m_MatrixThick.SetTextMatrix(i, 3,pRec->GetStrQty());
		m_MatrixThick.SetTextMatrix(i, 4,pRec->GetStrUWeight(nRoundUnit));	
		m_MatrixThick.SetTextMatrix(i, 5,pRec->GetStrTWeight(bUnitTon,nRoundTot));	

		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}

	i = m_MatrixThick.GetRows()-1;
	m_MatrixThick.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);
	m_MatrixThick.SetTextMatrix(i+1, 0,"SUB TOTAL");
	m_MatrixThick.SetTextMatrix(i+1, 5,str);
	//
	i = m_MatrixThick.GetRows()-1;
	m_MatrixThick.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);
	m_MatrixThick.SetTextMatrix(i+1, 0,"GRAND TOTAL");
	m_MatrixThick.SetTextMatrix(i+1, 5,str);
}


void CAPlateOutBM::AddPageBMBuje(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixBuje.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixBuje.GetRows())
			break;
		if(nEndRow >= m_MatrixBuje.GetRows())
			nEndRow = m_MatrixBuje.GetRows() - 1;
		DrawBMBuje(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_BUJE+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_BUJE+i, "부재별집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 부 재 별 집 계 표 : %d/%d",nSubPage,m_nPageBuje);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_BUJE_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_BUJE_TITLE, "부재집계타이틀");
			*pDomP << Dom;
		}

		*pDomP << Dom;
	}
}

void CAPlateOutBM::DrawBMBuje(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMBujeTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(18), Dom.Always(18),	Dom.Always(18), 
		  Dom.Always(18),	Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_BUJE);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nBujeMergeStt = 0;
	CString strBuje   = _T("");
	CString strNextBuje = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_BUJE; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=1 && j<=6)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixBuje.GetTextMatrix(i,j), nAlign);
		}
		strBuje  = m_MatrixBuje.GetTextMatrix(i,0);
		strNextBuje = (i==nEndRow) ? _T("") : m_MatrixBuje.GetTextMatrix(i+1,0);
		if(i==nEndRow || strBuje!=strNextBuje )
		{
			if(nBujeMergeStt<nGridRow)
				Dom.SetMergeRow(nBujeMergeStt,nGridRow,0);
			nBujeMergeStt = nGridRow+1;
			if(strNextBuje.IsEmpty()) 
				nBujeMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMBujeTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(18), Dom.Always(18),	Dom.Always(18), 
		  Dom.Always(18),	Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_BUJE);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "부재명");
	Dom.SetTextMatrix( 0,  1, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  2, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  3, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  4, "수량\n(개)");
	Dom.SetTextMatrix( 0,  5, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0,  6, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0,  7, "재질");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixBuje()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_Buje;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixBuje.SetSize(nRows,BM_COLUMN_BUJE);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	BOOL bContinue	= TRUE;
	long i			= 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if(  pRecPrev && pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName() && bContinue) 
		{
			m_MatrixBuje.InsertRow(i);
			str = pBase.GetStringWeight(SubWeight,nRoundTot);     
			m_MatrixBuje.SetTextMatrix(i, 0,"SUB TOTAL");
			m_MatrixBuje.SetTextMatrix(i, 6,str);
			SubWeight = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubWeight += pRec->m_dTWeight*dFactorUnit;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		}

		m_MatrixBuje.SetTextMatrix(i, 0,pRec->GetStrSubEleName());
		m_MatrixBuje.SetTextMatrix(i, 1,pRec->GetStrThick());
		m_MatrixBuje.SetTextMatrix(i, 2,pRec->GetStrWidth());
		m_MatrixBuje.SetTextMatrix(i, 3,pRec->GetStrLength());
		m_MatrixBuje.SetTextMatrix(i, 4,pRec->GetStrQty());
		m_MatrixBuje.SetTextMatrix(i, 5,pRec->GetStrUWeight(nRoundUnit));
		m_MatrixBuje.SetTextMatrix(i, 6,pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixBuje.SetTextMatrix(i, 7,pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}

	i = m_MatrixBuje.GetRows()-1;
	m_MatrixBuje.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixBuje.SetTextMatrix(i+1, 0,"SUB TOTAL");
	m_MatrixBuje.SetTextMatrix(i+1, 6,str);
	//
	i = m_MatrixBuje.GetRows()-1;
	m_MatrixBuje.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixBuje.SetTextMatrix(i+1, 0,"GRAND TOTAL");
	m_MatrixBuje.SetTextMatrix(i+1, 6,str);
}

//부재총괄집계표
void CAPlateOutBM::AddPageBMBujeTotal(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixBujeTotal.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixBujeTotal.GetRows())
			break;
		if(nEndRow >= m_MatrixBujeTotal.GetRows())
			nEndRow = m_MatrixBujeTotal.GetRows() - 1;
		DrawBMBujeTotal(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_BUJETOTAL+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_BUJETOTAL+i, "부재별총괄집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 부 재 별  총 괄 집 계 표 : %d/%d",nSubPage,m_nPageBujeTotal);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_BUJETOTAL_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_BUJETOTAL_TITLE, "부재별총괄집계타이틀");
			*pDomP << Dom;
		}
	}
}

//부재 종류별 촐괄 집계표
void CAPlateOutBM::AddPageBMSizeTypeTotal(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixSizeTypeTotal.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixSizeTypeTotal.GetRows())
			break;
		if(nEndRow >= m_MatrixSizeTypeTotal.GetRows())
			nEndRow = m_MatrixSizeTypeTotal.GetRows() - 1;
		DrawBMSizeTypeTotal(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_SIZETYPE+i));//ksg
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_SIZETYPE+i, "부재종류별총괄집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 부 재 종 류 별  총 괄 집 계 표 : %d/%d",nSubPage,m_nPageSizeTypeTotal);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_SIZETYPE_TITLE));
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_SIZETYPE_TITLE, "부재종류별총괄집계타이틀");
			*pDomP << Dom;
		}
	}
}

void CAPlateOutBM::DrawBMBujeTotal(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMBujeTitleTotal(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(40), Dom.Always(30), Dom.Always(30)};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_BUJE_TOTAL);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nBujeMergeStt = 0;
	CString strBuje   = _T("");
	CString strNextBuje = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_BUJE_TOTAL; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j==1)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixBujeTotal.GetTextMatrix(i,j), nAlign);
		}
		strBuje  = m_MatrixBujeTotal.GetTextMatrix(i,0);
		strNextBuje = (i==nEndRow) ? _T("") : m_MatrixBujeTotal.GetTextMatrix(i+1,0);
		if(i==nEndRow || strBuje!=strNextBuje )
		{
			if(nBujeMergeStt<nGridRow)
				Dom.SetMergeRow(nBujeMergeStt,nGridRow,0);
			nBujeMergeStt = nGridRow+1;
			if(strNextBuje.IsEmpty()) 
				nBujeMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMBujeTitleTotal(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(40), Dom.Always(30), Dom.Always(30)};
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_BUJE_TOTAL);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "부재명");
	Dom.SetTextMatrix( 0,  1, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0,  2, "재질");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixBujeTotal()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();            
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수 

	CPtrList *pList = &pBMStd->m_BMList_BujeTotal;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixBujeTotal.SetSize(nRows,BM_COLUMN_BUJE_TOTAL);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		
		if(pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice)
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		m_MatrixBujeTotal.SetTextMatrix(i, 0,pRec->GetStrSubEleName());
		m_MatrixBujeTotal.SetTextMatrix(i, 1,pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixBujeTotal.SetTextMatrix(i, 2,pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}

	i = m_MatrixBujeTotal.GetRows()-1;
	m_MatrixBujeTotal.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixBujeTotal.SetTextMatrix(i+1, 0,"SUB TOTAL");
	m_MatrixBujeTotal.SetTextMatrix(i+1, 1,str);
	//
	i = m_MatrixBujeTotal.GetRows()-1;
	m_MatrixBujeTotal.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixBujeTotal.SetTextMatrix(i+1, 0,"GRAND TOTAL");
	m_MatrixBujeTotal.SetTextMatrix(i+1, 1,str);
}


void CAPlateOutBM::DrawBMDeductBuje(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMDeductBujeTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(18), Dom.Always(18),	Dom.Always(18), 
			Dom.Always(18),	Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_DEDUCTBUJE);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nBujeMergeStt = 0;
	CString strBuje   = _T("");
	CString strNextBuje = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_DEDUCTBUJE; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=1 && j<=6)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixDeductBuje.GetTextMatrix(i,j), nAlign);
		}
		strBuje  = m_MatrixDeductBuje.GetTextMatrix(i,0);
		strNextBuje = (i==nEndRow) ? _T("") : m_MatrixDeductBuje.GetTextMatrix(i+1,0);
		if(i==nEndRow || strBuje!=strNextBuje )
		{
			if(nBujeMergeStt<nGridRow)
				Dom.SetMergeRow(nBujeMergeStt,nGridRow,0);
			nBujeMergeStt = nGridRow+1;
			if(strNextBuje.IsEmpty()) 
				nBujeMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMDeductBujeTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(18), Dom.Always(18),	Dom.Always(18), 
			Dom.Always(18),	Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_DEDUCTBUJE);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "부재명");
	Dom.SetTextMatrix( 0,  1, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  2, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  3, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  4, "수량\n(개)");
	Dom.SetTextMatrix( 0,  5, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0,  6, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0,  7, "재질");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixDeductBuje()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_DeductBuje;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixDeductBuje.SetSize(nRows,BM_COLUMN_DEDUCTBUJE);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName() && bContinue) 
		{
			m_MatrixDeductBuje.InsertRow(i);
			str = pBase.GetStringWeight(SubWeight,nRoundTot);     
			m_MatrixDeductBuje.SetTextMatrix(i, 0,"SUB TOTAL");
			m_MatrixDeductBuje.SetTextMatrix(i, 6,str);
			SubWeight = 0;
			bContinue = FALSE;		
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubWeight += pRec->m_dTWeight*dFactorUnit;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		}

		m_MatrixDeductBuje.SetTextMatrix(i, 0,pRec->GetStrSubEleName());
		m_MatrixDeductBuje.SetTextMatrix(i, 1,pRec->GetStrThick());
		m_MatrixDeductBuje.SetTextMatrix(i, 2,pRec->GetStrWidth());
		m_MatrixDeductBuje.SetTextMatrix(i, 3,pRec->GetStrLength());
		m_MatrixDeductBuje.SetTextMatrix(i, 4,pRec->GetStrQty());
		m_MatrixDeductBuje.SetTextMatrix(i, 5,pRec->GetStrUWeight(nRoundUnit));
		m_MatrixDeductBuje.SetTextMatrix(i, 6,pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixDeductBuje.SetTextMatrix(i, 7,pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	
	i = m_MatrixDeductBuje.GetRows()-1;
	m_MatrixDeductBuje.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixDeductBuje.SetTextMatrix(i+1, 0,"SUB TOTAL");
	m_MatrixDeductBuje.SetTextMatrix(i+1, 6,str);
	//
	i = m_MatrixDeductBuje.GetRows()-1;
	m_MatrixDeductBuje.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixDeductBuje.SetTextMatrix(i+1, 0,"GRAND TOTAL");
	m_MatrixDeductBuje.SetTextMatrix(i+1, 6,str);
}

void CAPlateOutBM::AddPageBMDeductBuje(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixDeductBuje.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));
	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixDeductBuje.GetRows())
			break;
		if(nEndRow >= m_MatrixDeductBuje.GetRows())
			nEndRow = m_MatrixDeductBuje.GetRows() - 1;
		DrawBMDeductBuje(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_DEDUCTBUJE+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DEDUCTBUJE+i, "부재별공제집계표");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 부 재 별 공 제  집 계 표 : %d/%d",nSubPage,m_nPageDeductBuje);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_DEDUCTBUJE_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DEDUCTBUJE_TITLE, "부재별공제집계타이틀");
			*pDomP << Dom;
		}
	}
}


void CAPlateOutBM::DrawBMDeductSang(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMDeductSangTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_DEDUCTSANG);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nGirMergeStt = 0;
	long nEleMergeStt = 0;
	CString sNumGir   = _T("");
	CString sEleName  = _T("");
	CString sNextName = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_DEDUCTSANG; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=4 && j<=9)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixDeductSang.GetTextMatrix(i,j), nAlign);
		}

		sNumGir  = m_MatrixDeductSang.GetTextMatrix(i,0);
		if(i==nEndRow || sNumGir.IsEmpty())
		{
			if(nGirMergeStt<nGridRow)
			{
				Dom.SetMergeRow(nGirMergeStt,nGridRow,0);
				Dom.SetMergeRow(nGirMergeStt,nGridRow,1);
			}
			nGirMergeStt = nGridRow+1;
		}

		sEleName  = m_MatrixDeductSang.GetTextMatrix(i+0,2);
		sNextName = (i==nEndRow) ? _T("") : m_MatrixDeductSang.GetTextMatrix(i+1,2);
		if(i==nEndRow || sEleName!=sNextName )
		{
			if(nEleMergeStt<nGridRow)
				Dom.SetMergeRow(nEleMergeStt,nGridRow,2);
			nEleMergeStt = nGridRow+1;
			if(sNextName.IsEmpty()) 
				nEleMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMDeductSangTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(24), Dom.Always(29),	Dom.Always(30)	};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_SANG);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "거더\n번호");
	Dom.SetTextMatrix( 0,  1, "현장\n이음");
	Dom.SetTextMatrix( 0,  2, "종 류");
	Dom.SetTextMatrix( 0,  3, "부재명");
	Dom.SetTextMatrix( 0,  4, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  5, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  6, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  7, "수량\n(개)");
	Dom.SetTextMatrix( 0,  8, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0,  9, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0, 10, "재질");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixDeductSang()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_DeductSang;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixDeductSang.SetSize(nRows,BM_COLUMN_DEDUCTSANG);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		
		if(pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice && bContinue)
		{ 
			m_MatrixDeductSang.InsertRow(i);
			str = pBase.GetStringWeight(SubWeight,nRoundTot);     
			m_MatrixDeductSang.SetTextMatrix(i, 8,"SUB TOTAL");
			m_MatrixDeductSang.SetTextMatrix(i, 9,str);
			SubWeight = 0;
			bContinue = FALSE;		
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubWeight += pRec->m_dTWeight*dFactorUnit;
				i++;
			}	
		}
		else
		{
			bContinue = TRUE;
			SubWeight += pRec->m_dTWeight*dFactorUnit;
		}

		m_MatrixDeductSang.SetTextMatrix(i, 0,pRec->GetStrGirder(pDB->GetNumberStartGirder()));
		m_MatrixDeductSang.SetTextMatrix(i, 1,pRec->GetStrSplice(pDB->GetNumberStartSplice()));
		m_MatrixDeductSang.SetTextMatrix(i, 2,pRec->GetStrEleName());
		m_MatrixDeductSang.SetTextMatrix(i, 3,pRec->GetStrSubEleName());
		m_MatrixDeductSang.SetTextMatrix(i, 4,pRec->GetStrThick());
		m_MatrixDeductSang.SetTextMatrix(i, 5,pRec->GetStrWidth());
		m_MatrixDeductSang.SetTextMatrix(i, 6,pRec->GetStrLength());
		m_MatrixDeductSang.SetTextMatrix(i, 7,pRec->GetStrQty());
		m_MatrixDeductSang.SetTextMatrix(i, 8,pRec->GetStrUWeight(nRoundUnit));
		m_MatrixDeductSang.SetTextMatrix(i, 9,pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixDeductSang.SetTextMatrix(i,10,pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	
	i = m_MatrixDeductSang.GetRows()-1;
	m_MatrixDeductSang.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixDeductSang.SetTextMatrix(i+1, 8,"SUB TOTAL");
	m_MatrixDeductSang.SetTextMatrix(i+1, 9,str);
	//
	i = m_MatrixDeductSang.GetRows()-1;
	m_MatrixDeductSang.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixDeductSang.SetTextMatrix(i+1, 8,"GRAND TOTAL");
	m_MatrixDeductSang.SetTextMatrix(i+1, 9,str);
}

void CAPlateOutBM::AddPageBMDeductSang(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixDeductSang.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixDeductSang.GetRows())
			break;
		if(nEndRow >= m_MatrixDeductSang.GetRows())
			nEndRow = m_MatrixDeductSang.GetRows() - 1;
		DrawBMDeductSang(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_DEDUCTSANG+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DEDUCTSANG+i, "상세별공제집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			Dom.SetCalcExtRect();
			CDRect rect = Dom.GetExtRect();
			CString str;
			str.Format(" 상 세 별 공 제 집 계 표 : %d/%d",nSubPage,m_nPageDeductSang);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_DEDUCTSANG_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_DEDUCTSANG_TITLE, "상세별공제집계타이틀");
			*pDomP << Dom;
		}
	}
}
//표준집계표
void CAPlateOutBM::AddPageBMStandard(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixStandard.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB      = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_TableSu;
	long nLineSu  = pOpt->m_BM_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixStandard.GetRows())
			break;
		if(nEndRow >= m_MatrixStandard.GetRows())
			nEndRow = m_MatrixStandard.GetRows() - 1;
		DrawBMStandard(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_BM_STANDARD+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_STANDARD+i, "표준집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 표 준 집 계 표 : %d/%d",nSubPage,m_nPageStandard);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_BM_STANDARD_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_STANDARD_TITLE, "표준집계타이틀");
			*pDomP << Dom;
		}
	}
}

void CAPlateOutBM::DrawBMStandard(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	DrawBMStandardTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(24), Dom.Always(29),	Dom.Always(30)};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_STANDARD);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nStandardMergeStt = 0;
	long nEleMergeStt = 0;
	CString strStandard   = _T("");
	CString strNextStandard = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_STANDARD; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=4 && j<=9)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixStandard.GetTextMatrix(i,j), nAlign);
		}
		strStandard  = m_MatrixStandard.GetTextMatrix(i,0);
		strNextStandard = (i==nEndRow) ? _T("") : m_MatrixStandard.GetTextMatrix(i+1,0);
		if(i==nEndRow || strNextStandard.IsEmpty())//i==nEndRow || strStandard!=strNextStandard )
		{
			if(nStandardMergeStt<nGridRow) {
				Dom.SetMergeRow(nStandardMergeStt,nGridRow,0);
				Dom.SetMergeRow(nStandardMergeStt,nGridRow,1);
			}
			nStandardMergeStt = nGridRow+1;
			if(strNextStandard.IsEmpty()) 
				nStandardMergeStt++;
		}
		if(strStandard.IsEmpty())
		{
		}

		strStandard  = m_MatrixStandard.GetTextMatrix(i,2);
		strNextStandard = (i==nEndRow) ? _T("") : m_MatrixStandard.GetTextMatrix(i+1,2);
		if(i==nEndRow || strStandard!=strNextStandard )
		{
			if(nEleMergeStt<nGridRow)
				Dom.SetMergeRow(nEleMergeStt,nGridRow,2);
			nEleMergeStt = nGridRow+1;
			if(strNextStandard.IsEmpty()) 
				nEleMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMStandardTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(24), Dom.Always(29),	Dom.Always(30)};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_STANDARD);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "거더\n번호");
	Dom.SetTextMatrix( 0,  1, "현장\n이음");
	Dom.SetTextMatrix( 0,  2, "종 류");
	Dom.SetTextMatrix( 0,  3, "부재명");
	Dom.SetTextMatrix( 0,  4, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  5, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  6, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  7, "수량\n(개)");
	Dom.SetTextMatrix( 0,  8, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0,  9, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0, 10, "재질");

	Dom.Draw();
	*pDomP << Dom;
}

void CAPlateOutBM::MakeMatrixStandard()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_StandardSang;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixStandard.SetSize(nRows,BM_COLUMN_STANDARD);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();

	CString strEleName,strSubName; 

	for(long i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);

		strEleName = pRec->GetStrEleName();
		strSubName = pRec->GetStrSubEleName();
		if(strEleName=="<->") strEleName = _T("");
		if(strSubName=="<->") strSubName = _T("");
		m_MatrixStandard.SetTextMatrix(i, 0, pRec->GetStrGirder(pDB->GetNumberStartGirder()));	// 거더
		m_MatrixStandard.SetTextMatrix(i, 1, pRec->GetStrSplice(pDB->GetNumberStartSplice()));	// 현장이음
		m_MatrixStandard.SetTextMatrix(i, 2, strEleName);							// 종류
		m_MatrixStandard.SetTextMatrix(i, 3, strSubName);							// 부재명
		m_MatrixStandard.SetTextMatrix(i, 4, pRec->GetStrThick());					// 두께
		m_MatrixStandard.SetTextMatrix(i, 5, pRec->GetStrWidth());					// 너비
		m_MatrixStandard.SetTextMatrix(i, 6, pRec->GetStrLength());				// 길이
		m_MatrixStandard.SetTextMatrix(i, 7, pRec->GetStrQty());					// 수량
		m_MatrixStandard.SetTextMatrix(i, 8, pRec->GetStrUWeight(nRoundUnit));				// 단위중량
		m_MatrixStandard.SetTextMatrix(i, 9, pRec->GetStrTWeight(bUnitTon, nRoundTot));			// 총중량
		m_MatrixStandard.SetTextMatrix(i,10, pRec->GetStrMaterial());				// 재질
//		m_MatrixStandard.SetTextMatrix(i,11, _T(""));				// 비고 
	}
}

//거더별 총괄 집계표//기타
void CAPlateOutBM::DrawBMGirderTotal(CDomyun *pDomP, CDPoint Base)
{
	if(m_MatrixGirderTotal.GetRows()==0) return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));
	CGridDomyunEx Dom(pDomP);
	DrawBMGirderTotalTitle(&Dom,Base);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = { Dom.Always(40), Dom.Always(30)}; 
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_GIRDER_TOTAL);
	Dom.SetRowCount(m_MatrixGirderTotal.GetRows());
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<BM_COLUMN_GIRDER_TOTAL; i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);
	
	CString strGir = _T("");
	CString strNextGir = _T("");
	long rowmin = 0;
	long nRow = m_MatrixGirderTotal.GetRows();
	for(i=0; i<nRow; i++)
	{
		for(long j=0; j<BM_COLUMN_GIRDER_TOTAL; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(i>=0 && j>=1)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(i,j,m_MatrixGirderTotal.GetTextMatrix(i,j), nAlign);
		}
		strGir = m_MatrixGirderTotal.GetTextMatrix(i,0);
		strNextGir = (i==nRow-1) ? _T("") : m_MatrixGirderTotal.GetTextMatrix(i+1,0);
		if(i==nRow || strGir != strNextGir)
		{
			Dom.SetMergeRow(rowmin, i, 0);
			rowmin = i+1;
		}
	}
	Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
	Dom.Draw();
	Dom.Move(pMove->GetPoint(MPE_BM_GIRDER));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_GIRDER, "거더별총괄집계");
	*pDomP << Dom;

	// Title
	CString str;
	str.Format(" 거 더 별 총 괄 집 계 표");
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	BaseTitle.x = pDomP->Always(420);
	Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
	Dom.Move(pMove->GetPoint(MPE_BM_GIRDER_TITLE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BM_GIRDER_TITLE, "거더별총괄타이틀");
	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMGirderTotalTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = { Dom.Always(40), Dom.Always(30)}; 
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_GIRDER_TOTAL);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "거더");
	Dom.SetTextMatrix( 0,  1, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixGirderTotal()
{	
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수

	CPtrList *pList = &pBMStd->m_BMList_Girder;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixGirderTotal.SetSize(nRows,BM_COLUMN_GIRDER_TOTAL);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;
	long	i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		
		if(pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice)
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		m_MatrixGirderTotal.SetTextMatrix(i, 0, pRec->GetStrGirder(pDB->GetNumberStartGirder()));
		m_MatrixGirderTotal.SetTextMatrix(i, 1, pRec->GetStrTWeight(bUnitTon, nRoundTot));
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	
	i = m_MatrixGirderTotal.GetRows()-1;
	m_MatrixGirderTotal.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixGirderTotal.SetTextMatrix(i+1, 0,"SUB TOTAL");
	m_MatrixGirderTotal.SetTextMatrix(i+1, 1,str);
	//
	i = m_MatrixGirderTotal.GetRows()-1;
	m_MatrixGirderTotal.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixGirderTotal.SetTextMatrix(i+1, 0,"GRAND TOTAL");
	m_MatrixGirderTotal.SetTextMatrix(i+1, 1,str);
}

//부재종류별 총괄 집계표//기타
void CAPlateOutBM::DrawBMSizeTypeTotal(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{	
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	DrawBMSizeTypeTotalTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1/*HCAD_TBTC*/);

	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(30), Dom.Always(40), Dom.Always(30), Dom.Always(30)};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(BM_COLUMN_SIZETYPE_TOTAL);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nBujeMergeStt = 0;
	CString strBuje   = _T("");
	CString strNextBuje = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<BM_COLUMN_SIZETYPE_TOTAL; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j==2)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixSizeTypeTotal.GetTextMatrix(i,j), nAlign);
		}
		strBuje  = m_MatrixSizeTypeTotal.GetTextMatrix(i,0);
		strNextBuje = (i==nEndRow) ? _T("") : m_MatrixSizeTypeTotal.GetTextMatrix(i+1,0);
		if(i==nEndRow || strBuje!=strNextBuje )
		{
			if(nBujeMergeStt<nGridRow)
				Dom.SetMergeRow(nBujeMergeStt,nGridRow,0);
			nBujeMergeStt = nGridRow+1;
			if(strNextBuje.IsEmpty()) 
				nBujeMergeStt++;
		}

		nGridRow++;
	}

	Dom.SetTextHeight(pStd->m_pOptStd->m_LayerList[HCAD_TBTC]->m_TextHeight);
	Dom.SetTextWidthFactor(pStd->m_pOptStd->m_LayerList[HCAD_TBTC]->m_WidthFactor);	
	Dom.SetFontStyle(pStd->m_pOptStd->m_LayerList[HCAD_TBTC]->m_FontStyle);
	Dom.Draw();

	*pDomP << Dom;
}

void CAPlateOutBM::DrawBMSizeTypeTotalTitle(CDomyun *pDomP, CDPoint Base)
{	
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption   *pGOpt  = pStd->GetDataManage()->GetGlobalOption();	

	CGridDomyunEx Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);

	double UpperTitleWidth[] = { Dom.Always(30), Dom.Always(40), Dom.Always(30), Dom.Always(30)}; 
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(BM_COLUMN_SIZETYPE_TOTAL);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "부재종류");
	Dom.SetTextMatrix( 0,  1, "부재명");
	Dom.SetTextMatrix( 0,  2, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");
	Dom.SetTextMatrix( 0,  3, "재질");

	Dom.SetTextHeight(pStd->m_pOptStd->m_LayerList[HCAD_TBTC]->m_TextHeight);
	Dom.SetTextWidthFactor(pStd->m_pOptStd->m_LayerList[HCAD_TBTC]->m_WidthFactor);	
	Dom.SetFontStyle(pStd->m_pOptStd->m_LayerList[HCAD_TBTC]->m_FontStyle);
	Dom.Draw();

	*pDomP << Dom;	
}
void CAPlateOutBM::MakeMatrixSizeTypeTotal()
{
 	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COptionStd	  *pOpt	  = pStd->m_pDomOptionStd;
	CGlobarOption   *pGOpt  = pStd->GetDataManage()->GetGlobalOption();	
	CBMCreator	  *pBMStd = pStd->GetDataManage()->GetBMStandard();
	CGangjeBase pBase(pDB);                                                                           
	long nRoundTot   = pGOpt->GetPointValueOfGangJae();		// 총중량 강재 반올림 자리수  

	CPtrList *pList = &pBMStd->m_BMList_BujeKindTotal;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	m_MatrixSizeTypeTotal.SetSize(nRows,BM_COLUMN_SIZETYPE_TOTAL);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CBMRecord* pRecPrev = NULL;
	double TotalWeight = 0;
	double SubWeight = 0;
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	double dFactorUnit = bUnitTon ? 0.001: 1.0;
	long	i = 0;
	for(i=0; i<nRows; i++)
	{
		CBMRecord * pRec = (CBMRecord *)pList->GetNext(pos);
		
		if(pRecPrev && pRecPrev->m_nSizeType != pRec->m_nSizeType)
			SubWeight = pRec->m_dTWeight*dFactorUnit;
		else
			SubWeight += pRec->m_dTWeight*dFactorUnit;

		m_MatrixSizeTypeTotal.SetTextMatrix(i, 0, pRec->m_nSizeType==1 ? "대형부재" : pRec->m_nSizeType==2 ? "소형부재" : "기타부재");
		m_MatrixSizeTypeTotal.SetTextMatrix(i, 1, pRec->GetStrSubEleName());
		m_MatrixSizeTypeTotal.SetTextMatrix(i, 2, pRec->GetStrTWeight(bUnitTon, nRoundTot));
		m_MatrixSizeTypeTotal.SetTextMatrix(i, 3, pRec->GetStrMaterial());
		//
		TotalWeight += pRec->m_dTWeight*dFactorUnit;
		pRecPrev = pRec;
	}
	
	i = m_MatrixSizeTypeTotal.GetRows()-1;
	m_MatrixSizeTypeTotal.InsertRow(i);
	str = pBase.GetStringWeight(SubWeight,nRoundTot);     
	m_MatrixSizeTypeTotal.SetTextMatrix(i+1, 1,"SUB TOTAL");
	m_MatrixSizeTypeTotal.SetTextMatrix(i+1, 2,str);
	//
	i = m_MatrixSizeTypeTotal.GetRows()-1;
	m_MatrixSizeTypeTotal.InsertRow(i);
	str = pBase.GetStringWeight(TotalWeight,nRoundTot);     
	m_MatrixSizeTypeTotal.SetTextMatrix(i+1, 1,"GRAND TOTAL");
	m_MatrixSizeTypeTotal.SetTextMatrix(i+1, 2,str);
}

//도장 상세 집계표
void CAPlateOutBM::AddPageBMPaintSang(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixPaintSang. GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_Paint_TableSu;
	long nLineSu  = pOpt->m_BM_Paint_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);

	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixPaintSang.GetRows())
			break;
		if(nEndRow >= m_MatrixPaintSang.GetRows())
			nEndRow = m_MatrixPaintSang.GetRows() - 1;
		DrawBMPaintSang(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_PAINT_SANG+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_SANG+i, "상세집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 상 세 집 계 표 : %d/%d",nSubPage,m_nPagePaintSang);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_PAINT_SANG_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_SANG_TITLE, "상세집계타이틀");
			*pDomP << Dom;
		}
		*pDomP << Dom;
	}
}
void CAPlateOutBM::DrawBMPaintSangTitle(CDomyun *pDomP, CDPoint Base)
{
	CGridDomyunEx Dom(pDomP);
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);

	double UpperTitleWidth[] = 
		  { Dom.Always(15), Dom.Always(15), Dom.Always(35), Dom.Always(35),
		    Dom.Always(20), Dom.Always(20),	Dom.Always(20), Dom.Always(20),
			Dom.Always(30), Dom.Always(30),	Dom.Always(30), Dom.Always(30),
			Dom.Always(30),	Dom.Always(30), Dom.Always(30)};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(PAINT_COLUMN_SANG);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	Dom.SetTextMatrix( 0,  0, "거더\n번호");
	Dom.SetTextMatrix( 0,  1, "현장\n이음");
	Dom.SetTextMatrix( 0,  2, "종 류");
	Dom.SetTextMatrix( 0,  3, "부재명");
	Dom.SetTextMatrix( 0,  4, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  5, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  6, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  7, "수량\n(개)");
	Dom.SetTextMatrix( 0,  8, "교량외부\n(공장)");
	Dom.SetTextMatrix( 0,  9, "교량외부\n(현장)");
	Dom.SetTextMatrix( 0, 10, "교량내부\n(공장)");
	Dom.SetTextMatrix( 0, 11, "연결판\n(공장)");
	Dom.SetTextMatrix( 0, 12, "외부 볼트 및\n연결판(현장)");
	Dom.SetTextMatrix( 0, 13, "내부 볼트 및\n연결판(현장)");
	Dom.SetTextMatrix( 0, 14, "외부포장면\n(공장)");

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::DrawBMPaintSang(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	long i=0, j=0;
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);

	DrawBMPaintSangTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(15), Dom.Always(15), Dom.Always(35), Dom.Always(35),
		    Dom.Always(20), Dom.Always(20),	Dom.Always(20), Dom.Always(20),
			Dom.Always(30), Dom.Always(30),	Dom.Always(30), Dom.Always(30),
			Dom.Always(30),	Dom.Always(30), Dom.Always(30)};
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(PAINT_COLUMN_SANG);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nGirMergeStt = 0;
	long nEleMergeStt = 0;
	CString sNumGir   = _T("");
	CString sEleName  = _T("");
	CString sNextName = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(j=0; j<PAINT_COLUMN_SANG; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=4)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixPaintSang.GetTextMatrix(i,j), nAlign);
		}
		sNumGir  = m_MatrixPaintSang.GetTextMatrix(i,0);
		if(i==nEndRow || sNumGir.IsEmpty())
		{
			if(nGirMergeStt<nGridRow)
			{
				Dom.SetMergeRow(nGirMergeStt,nGridRow,0);
				Dom.SetMergeRow(nGirMergeStt,nGridRow,1);
			}
			nGirMergeStt = nGridRow+1;
		}

		sEleName  = m_MatrixPaintSang.GetTextMatrix(i+0,2);
		sNextName = (i==nEndRow) ? _T("") : m_MatrixPaintSang.GetTextMatrix(i+1,2);
		if(i==nEndRow || sEleName!=sNextName )
		{
			if(nEleMergeStt<nGridRow)
				Dom.SetMergeRow(nEleMergeStt,nGridRow,2);
			nEleMergeStt = nGridRow+1;
			if(sNextName.IsEmpty()) 
				nEleMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixPaintSang()
{	
	CAPlateDrawStd  *pStd   = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB    = pStd->GetDataManage()->GetBridge();
	CGlobarOption   *pGOpt  = pStd->GetDataManage()->GetGlobalOption();	
	COptionStd	    *pOpt	= pStd->m_pDomOptionStd;
	CPaintCreator  *pPaStd = pStd->GetDataManage()->GetPaintStandard();

	CPtrList *pList = &pPaStd->m_ListSang;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	CString cs = _T("");
	long nCol	= 0;
	long nItem  = PAINT_ITEM_MAX;

	m_MatrixPaintSang.SetSize(nRows,PAINT_COLUMN_SANG);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CPaintRecord* pRecPrev = NULL;
	double AreaSub[PAINT_ITEM_MAX], AreaTotal[PAINT_ITEM_MAX];
	long nRound = pGOpt->GetPointValueOfGangJae();// 총중량 강재 반올림 자리수
	long i=0, j=0;;
	for(i=0; i<nItem; i++)	AreaSub[i]   = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	BOOL bContinue = TRUE;
	for(j=0; j<nRows; j++)
	{
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		if( pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice && bContinue) 
		{
			m_MatrixPaintSang.InsertRow(j);
			m_MatrixPaintSang.SetTextMatrix(j, 7,"SUB TOTAL");
			nCol = 8;
			for(i=0; i<nItem; i++)
			{			
				cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
				if(AreaSub[i]==0) cs = _T("");
				m_MatrixPaintSang.SetTextMatrix(j, nCol++,cs);
			}		
			for(i=0; i<nItem; i++)	AreaSub[i] = 0;
		
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
				j++;
		}
		bContinue = TRUE;
		for(i=0; i<nItem; i++)	AreaSub[i] += pRec->m_dPaintArea[i];
	
		m_MatrixPaintSang.SetTextMatrix(j, 0,pRec->GetStrGirder(pDB->GetNumberStartGirder()));
		m_MatrixPaintSang.SetTextMatrix(j, 1,pRec->GetStrSplice(pDB->GetNumberStartSplice()));
		m_MatrixPaintSang.SetTextMatrix(j, 2,pRec->GetStrEleName());
		m_MatrixPaintSang.SetTextMatrix(j, 3,pRec->GetStrSubEleName());
		m_MatrixPaintSang.SetTextMatrix(j, 4,pRec->GetStrThick());
		m_MatrixPaintSang.SetTextMatrix(j, 5,pRec->GetStrWidth());
		m_MatrixPaintSang.SetTextMatrix(j, 6,pRec->GetStrLength());
		m_MatrixPaintSang.SetTextMatrix(j, 7,COMMA(pRec->m_nQty));
		//
		nCol = 8;
		for(i=0; i<nItem; i++)										
		{																		
			if(pRec->m_dPaintArea[i]>0)
				cs.Format("%10.4f",toM2(pRec->m_dPaintArea[i]));
			else
				cs = _T("");
			m_MatrixPaintSang.SetTextMatrix(j, nCol++, cs);					
		}
		for(i=0; i<nItem; i++)	AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPrev = pRec;
	}

	j = m_MatrixPaintSang.GetRows()-1;
	m_MatrixPaintSang.InsertRow(j);
	m_MatrixPaintSang.SetTextMatrix(j+1, 7, "SUB TOTAL");
	nCol = 8;
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
		if(AreaSub[i]==0) cs = _T("");
		m_MatrixPaintSang.SetTextMatrix(j+1, nCol++,cs);
	}		

	j = m_MatrixPaintSang.GetRows()-1;
	m_MatrixPaintSang.InsertRow(j);
	m_MatrixPaintSang.SetTextMatrix(j+1, 7, "총면적");
	nCol = 8;
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_MatrixPaintSang.SetTextMatrix(j+1, nCol++,cs);
	}		
}
//도장 부재별 집계표
void CAPlateOutBM::AddPageBMPaintBuje(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixPaintBuje. GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB      = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_Paint_TableSu;
	long nLineSu  = pOpt->m_BM_Paint_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);
	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));
	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixPaintBuje.GetRows())
			break;
		if(nEndRow >= m_MatrixPaintBuje.GetRows())
			nEndRow = m_MatrixPaintBuje.GetRows() - 1;
		DrawBMPaintBuje(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_PAINT_BUJE+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_BUJE+i, "부재별집계표");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 부 재 별 집 계 표 : %d/%d",nSubPage,m_nPagePaintBuje);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_PAINT_BUJE_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_BUJE_TITLE, "부재별집계타이틀");
			*pDomP << Dom;
		}
	}	
}
void CAPlateOutBM::DrawBMPaintBuje(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base)
{
	long i=0, j=0;
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);

	DrawBMPaintBujeTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(18), Dom.Always(18),	Dom.Always(18), 
			Dom.Always(18),	Dom.Always(30), Dom.Always(30),	Dom.Always(30), 
			Dom.Always(30),	Dom.Always(30),	Dom.Always(30), Dom.Always(30)};
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(PAINT_COLUMN_BUJE);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(j=0; j<PAINT_COLUMN_BUJE; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=1)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixPaintBuje.GetTextMatrix(i,j), nAlign);
		}
		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::DrawBMPaintBujeTitle(CDomyun *pDomP, CDPoint Base)
{
	CGridDomyunEx Dom(pDomP);
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);

	double UpperTitleWidth[] = 
		  { Dom.Always(35), Dom.Always(18), Dom.Always(18),	Dom.Always(18), 
			Dom.Always(18),	Dom.Always(30), Dom.Always(30),	Dom.Always(30), 
			Dom.Always(30),	Dom.Always(30),	Dom.Always(30), Dom.Always(30)};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(PAINT_COLUMN_BUJE);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	Dom.SetTextMatrix( 0,  0, "부재명");
	Dom.SetTextMatrix( 0,  1, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  2, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  3, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  4, "수량\n(개)");
	Dom.SetTextMatrix( 0,  5, "교량외부\n(공장)");
	Dom.SetTextMatrix( 0,  6, "교량외부\n(현장)");
	Dom.SetTextMatrix( 0,  7, "교량내부\n(공장)");
	Dom.SetTextMatrix( 0,  8, "연결판\n(공장)");
	Dom.SetTextMatrix( 0,  9, "외부 볼트 및\n연결판(현장)");
	Dom.SetTextMatrix( 0, 10, "내부 볼트 및\n연결판(현장)");
	Dom.SetTextMatrix( 0, 11, "외부포장면\n(공장)");

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixPaintBuje()
{	
	CAPlateDrawStd *pStd   = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB   = pStd->GetDataManage()->GetBridge();
	CGlobarOption *pGOpt   = pStd->GetDataManage()->GetGlobalOption();	
	COptionStd	  *pOpt	   = pStd->m_pDomOptionStd;
	CPaintCreator *pPaStd = pStd->GetDataManage()->GetPaintStandard();

	CPtrList *pList = &pPaStd->m_ListBujeSang;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	CString cs = _T("");
	long nCol	= 0;
	long nItem  = PAINT_ITEM_MAX;

	m_MatrixPaintBuje.SetSize(nRows,PAINT_COLUMN_BUJE);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CPaintRecord* pRecPrev = NULL;
	double AreaSub[PAINT_ITEM_MAX], AreaTotal[PAINT_ITEM_MAX];
	long nRound = pGOpt->GetPointValueOfGangJae();// 총중량 강재 반올림 자리수
	long i=0, j=0;;
	for(i=0; i<nItem; i++)	AreaSub[i]   = 0;
	for(i=0; i<nItem; i++)	AreaTotal[i] = 0;
	
	BOOL bContinue = TRUE;
	for(j=0; j<nRows; j++)
	{
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		if( pRecPrev && pRecPrev->GetStrSubEleName() != pRec->GetStrSubEleName() && bContinue) 
		{
			m_MatrixPaintBuje.InsertRow(j);
			m_MatrixPaintBuje.SetTextMatrix(j, 4, "SUB TOTAL");
			nCol = 5;
			for(i=0; i<nItem; i++)
			{			
				cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
				if(AreaSub[i]==0) cs = _T("");
				m_MatrixPaintBuje.SetTextMatrix(j, nCol++,cs);
			}		
			for(i=0; i<nItem; i++)	AreaSub[i] = 0;

			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
				j++;
		}
		bContinue = TRUE;
		for(i=0; i<nItem; i++)	AreaSub[i] += pRec->m_dPaintArea[i];

		m_MatrixPaintBuje.SetTextMatrix(j, 0,pRec->GetStrSubEleName());
		m_MatrixPaintBuje.SetTextMatrix(j, 1,pRec->GetStrThick());
		m_MatrixPaintBuje.SetTextMatrix(j, 2,pRec->GetStrWidth());
		m_MatrixPaintBuje.SetTextMatrix(j, 3,pRec->GetStrLength());
		m_MatrixPaintBuje.SetTextMatrix(j, 4,COMMA(pRec->m_nQty));
		//
		nCol = 5;
		for(i=0; i<nItem; i++)										
		{																		
			if(pRec->m_dPaintArea[i]>0)
				cs.Format("%10.4f",toM2(pRec->m_dPaintArea[i]));
			else
				cs = _T("");
			m_MatrixPaintBuje.SetTextMatrix(j, nCol++, cs);					
		}
		for(i=0; i<nItem; i++)	AreaTotal[i] += pRec->m_dPaintArea[i];
		pRecPrev = pRec;
	}

	j = m_MatrixPaintBuje.GetRows()-1;
	m_MatrixPaintBuje.InsertRow(j);
	m_MatrixPaintBuje.SetTextMatrix(j+1, 4, "SUB TOTAL");
	nCol = 5;
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaSub[i]),nRound);
		if(AreaSub[i]==0) cs = _T("");
		m_MatrixPaintBuje.SetTextMatrix(j+1, nCol++,cs);
	}		

	j = m_MatrixPaintBuje.GetRows()-1;
	m_MatrixPaintBuje.InsertRow(j);
	m_MatrixPaintBuje.SetTextMatrix(j+1, 4, "총면적");
	nCol = 5;
	for(i=0; i<nItem; i++)
	{			
		cs = GetStringOutValue(toM2(AreaTotal[i]),nRound);
		if(AreaTotal[i]==0) cs = _T("");
		m_MatrixPaintBuje.SetTextMatrix(j+1, nCol++,cs);
	}	
}


////////////////////////////////////////////
//도장 총괄집계표+도장 볼트 집계표
////////////////////////////////////////////
void CAPlateOutBM::AddPageBMPaintEtc(CDomyun *pDomP, long nSubPage)
{
 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB      = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	CDomyun Dom(pDomP);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);

	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	DrawBMPaintTotal(&Dom,Base);
	Base.x = pDomP->Always(160);
	DrawBMPaintBolt(&Dom,Base);

	*pDomP << Dom;
}

//도장 총괄 집계표
void CAPlateOutBM::DrawBMPaintTotal(CDomyun *pDomP, CDPoint Base)
{
	if(m_MatrixPaintTotal.GetRows()==0) return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));
	CGridDomyunEx Dom(pDomP);
	DrawBMPaintTotalTitle(&Dom,Base);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(40), Dom.Always(30)};
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(PAINT_COLUMN_TOTAL);
	Dom.SetRowCount(m_MatrixPaintTotal.GetRows());
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<PAINT_COLUMN_TOTAL; i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);
	
	CString strGir = _T("");
	CString strNextGir = _T("");
	long rowmin = 0;
//	long nRow = m_MatrixPaintTotal.GetRows();
	long nRow = 7;
	for(i=0; i<nRow; i++)
	{
		for(long j=0; j<PAINT_COLUMN_TOTAL; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=1)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(i,j,m_MatrixPaintTotal.GetTextMatrix(i,j), nAlign);
		}
		strGir = m_MatrixPaintTotal.GetTextMatrix(i,0);
		strNextGir = (i==nRow-1) ? _T("") : m_MatrixPaintTotal.GetTextMatrix(i+1,0);
		if(i==nRow || strGir != strNextGir)
		{
			Dom.SetMergeRow(rowmin, i, 0);
			rowmin = i+1;
		}
	}
	Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
	Dom.Draw();
	Dom.Move(pMove->GetPoint(MPE_PAINT_TOTAL));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_TOTAL, "총괄집계");
	*pDomP << Dom;

	// Title
	CString str;
	str.Format(" 총 괄 집 계 표");
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
	Dom.Move(pMove->GetPoint(MPE_PAINT_TOTAL_TITLE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_TOTAL_TITLE, "총괄타이틀");
	*pDomP << Dom;
}


void CAPlateOutBM::DrawBMPaintTotalTitle(CDomyun *pDomP, CDPoint Base)
{
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(40), Dom.Always(30)};
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(PAINT_COLUMN_TOTAL);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	Dom.SetTextMatrix( 0, 0, "구분");
	Dom.SetTextMatrix( 0, 1, "면적\n(m2)");

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixPaintTotal()
{	
	CAPlateDrawStd *pStd   = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB   = pStd->GetDataManage()->GetBridge();
	CGlobarOption *pGOpt   = pStd->GetDataManage()->GetGlobalOption();
	COptionStd	  *pOpt	   = pStd->m_pDomOptionStd;
	CPaintCreator *pPaStd = pStd->GetDataManage()->GetPaintStandard();

	CPtrList *pList = &pPaStd->m_ListStandard;

	if(pList->GetCount()==0) return;
	
	long nRows = 7;
	CString cs = _T("");

	m_MatrixPaintTotal.SetSize(nRows,PAINT_COLUMN_TOTAL);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	double dSumPaintArea[PAINT_ITEM_MAX];
	for(long i=0; i<PAINT_ITEM_MAX; i++) dSumPaintArea[i]=0;

	while( pos )
	{	
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);
		for(long i=0; i<PAINT_ITEM_MAX; i++)
			dSumPaintArea[i] += pRec->m_dPaintArea[i];
	}
	long nRow = 0;
	long col  = 1;

	m_MatrixPaintTotal.SetTextMatrix(0,0,"교량 외부(공장)");
	m_MatrixPaintTotal.SetTextMatrix(1,0,"교량 외부(현장)");
	m_MatrixPaintTotal.SetTextMatrix(2,0,"교량 내부(공장)");
	m_MatrixPaintTotal.SetTextMatrix(3,0,"연 결 판 (공장)");
	m_MatrixPaintTotal.SetTextMatrix(4,0,"외부 볼트(현장)");
	m_MatrixPaintTotal.SetTextMatrix(5,0,"내부 볼트(현장)");
	m_MatrixPaintTotal.SetTextMatrix(6,0,"외부포장면(공장)");

	str.Format("%10.3f",toM2(dSumPaintArea[OUTSIDE_FACT]));
	m_MatrixPaintTotal.SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[OUTSIDE_FIELD]));
	m_MatrixPaintTotal.SetTextMatrix(nRow++,col,str);
//	str.Format("%10.3f",toM2(dSumPaintArea[INSIDE_FACT]));
//	m_MatrixPaintTotal.SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[SPLICE_FACT]));
	m_MatrixPaintTotal.SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[BOLT_FIELD_OUTSIDE]));
	m_MatrixPaintTotal.SetTextMatrix(nRow++,col,str);
//	str.Format("%10.3f",toM2(dSumPaintArea[BOLT_FIELD_INSIDE]));
//	m_MatrixPaintTotal.SetTextMatrix(nRow++,col,str);
	str.Format("%10.3f",toM2(dSumPaintArea[PAVE_FACT]));
	m_MatrixPaintTotal.SetTextMatrix(nRow++,col,str);
}
//도장 볼트 집계표
void CAPlateOutBM::DrawBMPaintBolt(CDomyun *pDomP, CDPoint Base)
{
	if(m_MatrixPaintBolt.GetRows()==0) return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));
	CGridDomyunEx Dom(pDomP);
	DrawBMPaintBoltTitle(&Dom,Base);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = { Dom.Always(30), Dom.Always(18),  Dom.Always(18),  
		                         Dom.Always(25), Dom.Always(18), Dom.Always(30)};
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(PAINT_COLUMN_BOLT);
	Dom.SetRowCount(m_MatrixPaintBolt.GetRows());
	Dom.SetRowHeightAll(dRowHeight);

	long i = 0;
	for(i=0; i<PAINT_COLUMN_BOLT; i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);
	
	CString strGir = _T("");
	CString strNextGir = _T("");
	long rowmin = 0;
	long nRow = m_MatrixPaintBolt.GetRows();
	for(i=0; i<nRow; i++)
	{
		for(long j=0; j<PAINT_COLUMN_BOLT; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=1)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(i,j,m_MatrixPaintBolt.GetTextMatrix(i,j), nAlign);
		}
		strGir = m_MatrixPaintBolt.GetTextMatrix(i,0);
		strNextGir = (i==nRow-1) ? _T("") : m_MatrixPaintBolt.GetTextMatrix(i+1,0);
		if(i==nRow || strGir != strNextGir)
		{
			Dom.SetMergeRow(rowmin, i, 0);
			rowmin = i+1;
		}
	}
	Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
	Dom.Draw();
	Dom.Move(pMove->GetPoint(MPE_PAINT_BOLT));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_BOLT, "볼트집계");
	*pDomP << Dom;

	// Title
	CString str;
	str.Format(" 볼 트 집 계 표");
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
	Dom.SetTextAlignHorz(TA_LEFT);
	BaseTitle.x = pDomP->Always(160);
	Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
	Dom.Move(pMove->GetPoint(MPE_PAINT_BOLT_TITLE));
	Dom.SetCalcExtRect();
	pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
	pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PAINT_BOLT_TITLE, "볼트집계타이틀");
	*pDomP << Dom;

}
void CAPlateOutBM::DrawBMPaintBoltTitle(CDomyun *pDomP, CDPoint Base)
{
	CAPlateDrawStd *pStd  = (CAPlateDrawStd*)m_pStd;
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = { Dom.Always(30), Dom.Always(18),  Dom.Always(18),  
		                         Dom.Always(25), Dom.Always(18), Dom.Always(30)};
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(PAINT_COLUMN_BOLT);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	BOOL bUnitTon = pGOpt->GetDesignUnitTonfMeter();
	Dom.SetTextMatrix( 0,  0, "재질");
	Dom.SetTextMatrix( 0,  1, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  2, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  3, "단위중량\n(kgf/m3)");
	Dom.SetTextMatrix( 0,  4, "수량\n(개)");
	Dom.SetTextMatrix( 0,  5, bUnitTon ? "총중량\n(Tonf)" : "총중량\n(kgf)");

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixPaintBolt()
{	
	CAPlateDrawStd *pStd   = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB   = pStd->GetDataManage()->GetBridge();
	CGlobarOption *pGOpt   = pStd->GetDataManage()->GetGlobalOption();	
	COptionStd	  *pOpt	   = pStd->m_pDomOptionStd;
	CPaintCreator *pPaStd = pStd->GetDataManage()->GetPaintStandard();

	CPtrList *pList = &pPaStd->m_ListBoltTotal;

	if(pList->GetCount() == 0) return;

	POSITION pos = pList->GetHeadPosition();

	CString cs = _T("");
	long nRows = pList->GetCount();
	long nRoundUnit  = pGOpt->GetPointValueOfUnitGangJae();	// 단위중량 강재 반올림 자리수
	m_MatrixPaintBolt.SetSize(nRows, PAINT_COLUMN_BOLT);

	for(long j=0; j<nRows; j++)
	{
		CPaintRecord *pRec = (CPaintRecord *)pList->GetNext(pos);

		m_MatrixPaintBolt.SetTextMatrix(j, 0, pRec->m_sMaterial);
		m_MatrixPaintBolt.SetTextMatrix(j, 1, pRec->GetStrThick());	
		m_MatrixPaintBolt.SetTextMatrix(j, 2, pRec->GetStrLength());
		cs = GetStringOutValue(pRec->m_dUWeight,nRoundUnit);
		m_MatrixPaintBolt.SetTextMatrix(j, 3, cs);
		cs.Format("%d",pRec->m_nQty);
		m_MatrixPaintBolt.SetTextMatrix(j, 4, cs);
		cs = GetStringOutValue(pRec->m_dTWeight,nRoundUnit);
		m_MatrixPaintBolt.SetTextMatrix(j, 5, cs);
	}
}

// 용접 집계표//////////////////////////////////////
// 상세 집계표
void CAPlateOutBM::AddPageWeldSang(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixWeldSang.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_Weld_TableSu;
	long nLineSu  = pOpt->m_BM_Weld_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);
	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixWeldSang.GetRows())
			break;
		if(nEndRow >= m_MatrixWeldSang.GetRows())
			nEndRow = m_MatrixWeldSang.GetRows() - 1;
		DrawWeldSang(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_WELD_SANG+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WELD_SANG+i, "상세집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 상 세 집 계 표 : %d/%d",nSubPage,m_nPageWeldSang);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_WELD_SANG_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WELD_SANG_TITLE, "상세집계타이틀");
			*pDomP << Dom;
		}
		*pDomP << Dom;
	}
}
void CAPlateOutBM::DrawWeldSang(CDomyun *pDomP,  long nSttRow, long nEndRow, CDPoint Base)
{
	DrawWeldSangTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(25), Dom.Always(20),	Dom.Always(20), Dom.Always(30), 
			Dom.Always(30), Dom.Always(30), Dom.Always(30), Dom.Always(23)};
	
	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(WELD_COLUMN_SANG);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	long nGridRow     = 0;
	long nGirMergeStt = 0;
	long nEleMergeStt = 0;
	CString sNumGir   = _T("");
	CString sEleName  = _T("");
	CString sNextName = _T("");
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<WELD_COLUMN_SANG; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if((j>=4 && j<=7) || j==9 || j==10 || j==15)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixWeldSang.GetTextMatrix(i,j), nAlign);
		}

		sNumGir  = m_MatrixWeldSang.GetTextMatrix(i,0);
		if(i==nEndRow || sNumGir.IsEmpty())
		{
			if(nGirMergeStt<nGridRow)
			{
				Dom.SetMergeRow(nGirMergeStt,nGridRow,0);
				Dom.SetMergeRow(nGirMergeStt,nGridRow,1);
			}
			nGirMergeStt = nGridRow+1;
		}

		sEleName  = m_MatrixWeldSang.GetTextMatrix(i+0,2);
		sNextName = (i==nEndRow) ? _T("") : m_MatrixWeldSang.GetTextMatrix(i+1,2);
		if(i==nEndRow || sEleName!=sNextName )
		{
			if(nEleMergeStt<nGridRow)
				Dom.SetMergeRow(nEleMergeStt,nGridRow,2);
			nEleMergeStt = nGridRow+1;
			if(sNextName.IsEmpty()) 
				nEleMergeStt++;
		}

		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::DrawWeldSangTitle(CDomyun *pDomP, CDPoint Base)
{
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(10), Dom.Always(10), Dom.Always(35), Dom.Always(35),
		    Dom.Always(18), Dom.Always(18),	Dom.Always(18), Dom.Always(18),
			Dom.Always(25), Dom.Always(20),	Dom.Always(20), Dom.Always(30), 
			Dom.Always(30), Dom.Always(30), Dom.Always(30), Dom.Always(23)};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(WELD_COLUMN_SANG);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	Dom.SetTextMatrix( 0,  0, "거더\n번호");
	Dom.SetTextMatrix( 0,  1, "현장\n이음");
	Dom.SetTextMatrix( 0,  2, "종 류");
	Dom.SetTextMatrix( 0,  3, "부재명");
	Dom.SetTextMatrix( 0,  4, "두께\n(mm)");
	Dom.SetTextMatrix( 0,  5, "너비\n(mm)");
	Dom.SetTextMatrix( 0,  6, "길이\n(mm)");
	Dom.SetTextMatrix( 0,  7, "수량\n(개)");
	Dom.SetTextMatrix( 0,  8, "용접형태");
	Dom.SetTextMatrix( 0,  9, "용접두께\n(mm)");
	Dom.SetTextMatrix( 0, 10, "용접장\n(mm)");
	Dom.SetTextMatrix( 0, 11, "개선방법");
	Dom.SetTextMatrix( 0, 12, "적층방법");
	Dom.SetTextMatrix( 0, 13, "접속부재\n종류");
	Dom.SetTextMatrix( 0, 14, "접속부재\n부재명");
	Dom.SetTextMatrix( 0, 15, "접속부재\n두께(mm)");

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixWeldSang()
{	
	CAPlateDrawStd *pStd    = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	 *pDB   = pStd->GetDataManage()->GetBridge();
	CGlobarOption *pGOpt    = pStd->GetDataManage()->GetGlobalOption();
	COptionStd	  *pOpt	    = pStd->m_pDomOptionStd;
	CWeldStandard *pWeldStd = pStd->GetDataManage()->GetWeldStandard();

	CPtrList *pList = &pWeldStd->m_ListSang;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	CString cs = _T("");

	m_MatrixWeldSang.SetSize(nRows, WELD_COLUMN_SANG);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CWeldRecord* pRecPrev = NULL;

	double TotalLength = 0;
	double SubLength   = 0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CWeldRecord * pRec = (CWeldRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_nSplice != pRec->m_nSplice && bContinue) 
		{
			m_MatrixWeldSang.InsertRow(i);
			str.Format("%.0f", SubLength);     
			m_MatrixWeldSang.SetTextMatrix(i,  3,"SUB TOTAL");
			m_MatrixWeldSang.SetTextMatrix(i, 10, str);
		//	SubLength = pRec->m_dLengthWeld;
			SubLength = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubLength += pRec->m_dLengthWeld;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubLength += pRec->m_dLengthWeld;
		}

		m_MatrixWeldSang.SetTextMatrix(i, 0,pRec->GetStrGirder(pDB->GetNumberStartGirder()));
		m_MatrixWeldSang.SetTextMatrix(i, 1,pRec->GetStrSplice(pDB->GetNumberStartSplice()));
		m_MatrixWeldSang.SetTextMatrix(i, 2,pRec->GetStrEleName());
		m_MatrixWeldSang.SetTextMatrix(i, 3,pRec->GetStrSubEleName());
		m_MatrixWeldSang.SetTextMatrix(i, 4,pRec->GetStrThick());
		m_MatrixWeldSang.SetTextMatrix(i, 5,pRec->GetStrWidth());
		m_MatrixWeldSang.SetTextMatrix(i, 6,pRec->GetStrLength());
		m_MatrixWeldSang.SetTextMatrix(i, 7,COMMA(pRec->m_nQty));
		m_MatrixWeldSang.SetTextMatrix(i, 8,pRec->m_sWeldType);
		str.Format("%.0f",pRec->m_dThickWeld);
		m_MatrixWeldSang.SetTextMatrix(i, 9,str);
		str.Format("%.0f",pRec->m_dLengthWeld);
		m_MatrixWeldSang.SetTextMatrix(i,10,str);
		m_MatrixWeldSang.SetTextMatrix(i,11,pRec->m_sImproveType);
		m_MatrixWeldSang.SetTextMatrix(i,12,pRec->m_sLayType);
		m_MatrixWeldSang.SetTextMatrix(i,13,pRec->GetStrEleConnectName());
		m_MatrixWeldSang.SetTextMatrix(i,14,pRec->GetStrSubEleConnectName());
		str.Format("%.0f",pRec->m_dThickConnect);
		m_MatrixWeldSang.SetTextMatrix(i,15,str);
		//
		TotalLength += pRec->m_dLengthWeld;
		pRecPrev = pRec;
	}

	i = m_MatrixWeldSang.GetRows()-1;
	m_MatrixWeldSang.InsertRow(i);
	str.Format("%.0f", SubLength);
	m_MatrixWeldSang.SetTextMatrix(i+1, 3,"SUB TOTAL");
	m_MatrixWeldSang.SetTextMatrix(i+1, 10,str);
	//
	i = m_MatrixWeldSang.GetRows()-1;
	m_MatrixWeldSang.InsertRow(i);
	str.Format("%.0f", TotalLength);
	m_MatrixWeldSang.SetTextMatrix(i+1, 3,"GRAND TOTAL");
	m_MatrixWeldSang.SetTextMatrix(i+1, 10,str);
}
// 타입별 집계표
void CAPlateOutBM::AddPageWeldType(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixWeldType.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_Weld_TableSu;
	long nLineSu  = pOpt->m_BM_Weld_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);
	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixWeldType.GetRows())
			break;
		if(nEndRow >= m_MatrixWeldType.GetRows())
			nEndRow = m_MatrixWeldType.GetRows() - 1;
		DrawWeldType(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_WELD_TYPE+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WELD_TYPE+i, "용접타입별집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 용 접 타 입 별 집 계 표 : %d/%d",nSubPage,m_nPageWeldType);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_WELD_TYPE_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WELD_TYPE_TITLE, "용접타입별집계타이틀");
			*pDomP << Dom;
		}
		*pDomP << Dom;
	}
}
void CAPlateOutBM::DrawWeldType(CDomyun *pDomP,  long nSttRow, long nEndRow, CDPoint Base)
{
	DrawWeldTypeTitle(pDomP,Base);

	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(30), Dom.Always(30), Dom.Always(30), Dom.Always(30),
		  Dom.Always(20), Dom.Always(25),	Dom.Always(30)};

	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(WELD_COLUMN_TYPE);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);
	long nGridRow = 0;
	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<WELD_COLUMN_TYPE; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=3 && j<=4)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixWeldType.GetTextMatrix(i,j), nAlign);
		}
		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::DrawWeldTypeTitle(CDomyun *pDomP, CDPoint Base)
{
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(30), Dom.Always(30), Dom.Always(30), Dom.Always(30),
		  Dom.Always(20), Dom.Always(25),	Dom.Always(30)};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(WELD_COLUMN_TYPE);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	Dom.SetTextMatrix( 0,  0, "용접형태");
	Dom.SetTextMatrix( 0,  1, "개선형태");
	Dom.SetTextMatrix( 0,  2, "적층방법");	
	Dom.SetTextMatrix( 0,  3, "용접장\n(mm)");
	Dom.SetTextMatrix( 0,  4, "용접두께\n(mm)");
	Dom.SetTextMatrix( 0,  5, "부재분류");
	Dom.SetTextMatrix( 0,  6, "부재명");
	
	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixWeldType()
{	
	CAPlateDrawStd *pStd   = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB   = pStd->GetDataManage()->GetBridge();
	CGlobarOption *pGOpt   = pStd->GetDataManage()->GetGlobalOption();
	COptionStd	  *pOpt	   = pStd->m_pDomOptionStd;
	CWeldStandard *pWeldStd= pStd->GetDataManage()->GetWeldStandard();

	CPtrList *pList = &pWeldStd->m_ListWeldType;

	if(pList->GetCount()==0) return;
	
	long nRows = pList->GetCount();

	CString cs = _T("");

	m_MatrixWeldType.SetSize(nRows, WELD_COLUMN_TYPE);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CWeldRecord* pRecPrev = NULL;

	double TotalLength = 0;
	double SubLength   = 0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CWeldRecord * pRec = (CWeldRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_sWeldType != pRec->m_sWeldType && bContinue) 
		{
			m_MatrixWeldType.InsertRow(i);
			str.Format("%.0f", SubLength);     
			m_MatrixWeldType.SetTextMatrix(i,  2,"SUB TOTAL");
			m_MatrixWeldType.SetTextMatrix(i,  3, str);
		//	SubLength = pRec->m_dLengthWeld;
			SubLength = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubLength += pRec->m_dLengthWeld;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubLength += pRec->m_dLengthWeld;
		}
		m_MatrixWeldType.SetTextMatrix(i, 0,pRec->m_sWeldType);
		m_MatrixWeldType.SetTextMatrix(i, 1,pRec->m_sImproveType);
		m_MatrixWeldType.SetTextMatrix(i, 2,pRec->m_sLayType);
		str.Format("%.0f",pRec->m_dLengthWeld);
		m_MatrixWeldType.SetTextMatrix(i, 3,str);
		str.Format("%.0f",pRec->m_dThickWeld);
		m_MatrixWeldType.SetTextMatrix(i, 4,str);
		m_MatrixWeldType.SetTextMatrix(i, 5,pRec->GetStrEleName());
		m_MatrixWeldType.SetTextMatrix(i, 6,pRec->GetStrSubEleName());
		//
		TotalLength += pRec->m_dLengthWeld;
		pRecPrev = pRec;
	}

	i = m_MatrixWeldType.GetRows()-1;
	m_MatrixWeldType.InsertRow(i);
	str.Format("%.0f", SubLength);
	m_MatrixWeldType.SetTextMatrix(i+1, 2,"SUB TOTAL");
	m_MatrixWeldType.SetTextMatrix(i+1, 3,str);
	//
	i = m_MatrixWeldType.GetRows()-1;
	m_MatrixWeldType.InsertRow(i);
	str.Format("%.0f", TotalLength);
	m_MatrixWeldType.SetTextMatrix(i+1, 2,"GRAND TOTAL");
	m_MatrixWeldType.SetTextMatrix(i+1, 3,str);
}
// 용접 두께별 집계표
void CAPlateOutBM::AddPageWeldThick(CDomyun *pDomP, long nSubPage)
{
	if(m_MatrixWeldThick.GetRows() == 0)  return;

 	CAPlateDrawStd	*pStd	  = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp	*pDB	  = pStd->GetDataManage()->GetBridge();
	COutMovePoint	*pMove	  = pStd->GetMovePoint(pStd->m_nCurPage);
	CDrawPageMng	*pPageMng = pStd->GetDrawPageMng();
	COptionStd		*pOpt	  = pStd->m_pDomOptionStd;

	long nTableSu = pOpt->m_BM_Weld_TableSu;
	long nLineSu  = pOpt->m_BM_Weld_LineSu;
	long nDataSu  = (long)(nTableSu*nLineSu);
	long nSttRow = 0;
	long nEndRow = 0;
	CDPoint Base(pDomP->Always(25),pDomP->Always(520));
	CDPoint BaseTitle(pDomP->Always(25),pDomP->Always(520));

	CDomyun Dom(pDomP);
	for(long i=0; i<nTableSu; i++)
	{
		pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);
		nSttRow = nDataSu*(nSubPage-1) + nLineSu*i;
		nEndRow = nSttRow + nLineSu - 1;
		if(nSttRow >= m_MatrixWeldThick.GetRows())
			break;
		if(nEndRow >= m_MatrixWeldThick.GetRows())
			nEndRow = m_MatrixWeldThick.GetRows() - 1;
		DrawWeldThick(&Dom,nSttRow,nEndRow,Base);
		Dom.SetCalcExtRect();
		Base.x += (Dom.GetExtRect().Width() + Dom.Always(10));
		Dom.Move(pMove->GetPoint(MPE_WELD_THICK+i));
		Dom.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WELD_THICK+i, "용접두께별집계");
		*pDomP << Dom;
		// TITLE
		if(i==0)
		{
			CString str;
			str.Format(" 용 접 두 께 별 집 계 표 : %d/%d",nSubPage,m_nPageWeldThick);
			pStd->m_pOptStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.SetTextAlignHorz(TA_LEFT);
			Dom.TextOut(BaseTitle.x , BaseTitle.y + Dom.Always(5),str);
			Dom.Move(pMove->GetPoint(MPE_WELD_THICK_TITLE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom); // HCAD 블럭을 위한 작업
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WELD_THICK_TITLE, "용접두께별집계타이틀");
			*pDomP << Dom;
		}
		*pDomP << Dom;
	}
}
void CAPlateOutBM::DrawWeldThick(CDomyun *pDomP,  long nSttRow, long nEndRow, CDPoint Base)
{
	DrawWeldThickTitle(pDomP,Base);
	long nGridRow = 0;
	CGridDomyunEx Dom(pDomP);
	Base.y -= Dom.Always(16);
	double UpperTitleWidth[] = 
		  { Dom.Always(30), Dom.Always(30), Dom.Always(30), Dom.Always(30),
		  Dom.Always(20)};

	double dRowHeight = Dom.Always(8);
	Dom.SetBasePoint(Base);
	Dom.SetColumnCount(WELD_COLUMN_THICK);
	Dom.SetRowCount(nEndRow-nSttRow+1);
	Dom.SetRowHeightAll(dRowHeight);
	long i  = 0;
	for(i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	for(i=nSttRow; i<=nEndRow; i++)
	{
		for(long j=0; j<WELD_COLUMN_THICK; j++)
		{
			long nAlign = GRID_ALIGN_CENTER|GRID_ALIGN_VCENTER;
			if(j>=3 && j<=4)
				nAlign = GRID_ALIGN_RIGHT|GRID_ALIGN_VCENTER;
			Dom.SetTextMatrix(nGridRow,j,m_MatrixWeldThick.GetTextMatrix(i,j), nAlign);
		}
		nGridRow++;
	}

	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::DrawWeldThickTitle(CDomyun *pDomP, CDPoint Base)
{
	CGridDomyunEx Dom(pDomP);

	double UpperTitleWidth[] = 
		  { Dom.Always(30), Dom.Always(30), Dom.Always(30), Dom.Always(30),
		  Dom.Always(20)};
	
	double dRowHeight = Dom.Always(16);

	Dom.SetBasePoint(Base);
	Dom.SetRowCount(1);
	Dom.SetColumnCount(WELD_COLUMN_THICK);
	Dom.SetRowHeightAll(dRowHeight);
	for(long i=0; i<Dom.GetColumnCount(); i++)
		Dom.SetColumnWidth(i,UpperTitleWidth[i]);
	Dom.SetMargin(3);

	//테이블 제목 쓰기
	Dom.SetTextMatrix( 0,  0, "용접형태");
	Dom.SetTextMatrix( 0,  1, "개선형태");
	Dom.SetTextMatrix( 0,  2, "적층방법");	
	Dom.SetTextMatrix( 0,  3, "용접장\n(mm)");
	Dom.SetTextMatrix( 0,  4, "용접두께\n(mm)");
	
	Dom.Draw();
	*pDomP << Dom;
}
void CAPlateOutBM::MakeMatrixWeldThick()
{	
	CAPlateDrawStd *pStd   = (CAPlateDrawStd*)m_pStd;
	CPlateBridgeApp *pDB   = pStd->GetDataManage()->GetBridge();
	CGlobarOption *pGOpt = pStd->GetDataManage()->GetGlobalOption();
	COptionStd	  *pOpt	   = pStd->m_pDomOptionStd;
	CWeldStandard *pWeldStd = pStd->GetDataManage()->GetWeldStandard();

	CPtrList *pList = &pWeldStd->m_ListWeldThick;

	if(pList->GetCount()==0) return;

	long nRows = pList->GetCount();

	CString cs = _T("");
	m_MatrixWeldThick.SetSize(nRows, WELD_COLUMN_THICK);

	POSITION pos = pList->GetHeadPosition();
	CString str = _T("");

	CWeldRecord* pRecPrev = NULL;

	double TotalLength = 0;
	double SubLength   = 0;

	BOOL bContinue = TRUE;
	long i = 0;
	for(i=0; i<nRows; i++)
	{
		CWeldRecord * pRec = (CWeldRecord *)pList->GetNext(pos);

		if( pRecPrev && pRecPrev->m_dThickWeld != pRec->m_dThickWeld && bContinue) 
		{
			m_MatrixWeldThick.InsertRow(i);
			str.Format("%.0f", SubLength);     
			m_MatrixWeldThick.SetTextMatrix(i,  2,"SUB TOTAL");
			m_MatrixWeldThick.SetTextMatrix(i,  3, str);
		//	SubLength = pRec->m_dLengthWeld;
			SubLength = 0;
			bContinue = FALSE;
			if(pos)	
			{ 
				pList->GetPrev(pos);	
				nRows++; 
				continue;
			}
			else
			{
				SubLength += pRec->m_dLengthWeld;
				i++;
			}
		}
		else
		{
			bContinue = TRUE;
			SubLength += pRec->m_dLengthWeld;
		}

		m_MatrixWeldThick.SetTextMatrix(i, 0,pRec->m_sWeldType);
		m_MatrixWeldThick.SetTextMatrix(i, 1,pRec->m_sImproveType);
		m_MatrixWeldThick.SetTextMatrix(i, 2,pRec->m_sLayType);
		str.Format("%.0f",pRec->m_dLengthWeld);
		m_MatrixWeldThick.SetTextMatrix(i, 3,str);
		str.Format("%.0f",pRec->m_dThickWeld);
		m_MatrixWeldThick.SetTextMatrix(i, 4,str);
		//
		TotalLength += pRec->m_dLengthWeld;
		pRecPrev = pRec;
	}

	i = m_MatrixWeldThick.GetRows()-1;
	m_MatrixWeldThick.InsertRow(i);
	str.Format("%.0f", SubLength);
	m_MatrixWeldThick.SetTextMatrix(i+1, 2,"SUB TOTAL");
	m_MatrixWeldThick.SetTextMatrix(i+1, 3,str);
	//
	i = m_MatrixWeldThick.GetRows()-1;
	m_MatrixWeldThick.InsertRow(i);
	str.Format("%.0f", TotalLength);
	m_MatrixWeldThick.SetTextMatrix(i+1, 2,"GRAND TOTAL");
	m_MatrixWeldThick.SetTextMatrix(i+1, 3,str);
}
