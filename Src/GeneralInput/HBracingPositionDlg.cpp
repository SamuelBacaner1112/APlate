// HBracingPositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHBracingPositionDlg dialog

CHBracingPositionDlg::CHBracingPositionDlg(CWnd* pParent /*=NULL*/)
	: CGeneralBaseDlg(CHBracingPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHBracingPositionDlg)

	m_nBaseHeightPosHBraceSplice = 0;

	//}}AFX_DATA_INIT
}


void CHBracingPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CGeneralBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHBracingPositionDlg)
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRID2, m_GridStd);
	DDX_Radio(pDX, IDC_RADIO_BASEPOS_UU, m_nBaseHeightPosHBraceSplice);
	DDX_Control(pDX, IDC_EDIT_BASE_HEIGHT, m_sBaseHeightFixPosHBraceSplice);
	DDX_Control(pDX, IDC_EDIT_BASE_RATIO, m_sBaseHeightRatioPosHBraceSplice);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_REMOVEALL, m_btnRemove);
	DDX_Control(pDX, IDC_BUTTON_SAMETYPE, m_btnSameAll);
	DDX_Control(pDX, IDC_BUTTON_ADD_STEEL, m_btnAddSteel);
	DDX_Control(pDX, IDC_BUTTON_DEL_STEEL, m_btnDelSteel);
}


BEGIN_MESSAGE_MAP(CHBracingPositionDlg, CGeneralBaseDlg)
	//{{AFX_MSG_MAP(CHBracingPositionDlg)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEALL, OnButtonRemoveAllHBracing)
	ON_BN_CLICKED(IDC_BUTTON_SAMETYPE, OnButtonSametype)
	ON_BN_CLICKED(ID_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(ID_BUTTON_PREV, OnButtonPrev)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_BASEPOS_UU, OnRadioBaseposUpperGirderLower)
	ON_BN_CLICKED(IDC_RADIO_BASEPOS_DU, OnRadioBaseposLowerGirderUpper)
	ON_BN_CLICKED(IDC_RADIO_HEIGHT_RATIO, OnRadioHeightByRato)
	ON_BN_CLICKED(IDC_BUTTON_ADD_STEEL, OnButtonAddSteel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_STEEL, OnButtonDelSteel)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID2, OnCellChangedDataGridStdSteel)
	ON_EN_KILLFOCUS(IDC_EDIT_BASE_HEIGHT, OnKillfocusEditBaseHeight)
	ON_EN_KILLFOCUS(IDC_EDIT_BASE_RATIO, OnKillfocusEditBaseRatio)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHBracingPositionDlg message handlers
// 모든 횡브레이싱이음판의속성을 초기화함.
void CHBracingPositionDlg::ClearAll()
{
	CPlateBridge *pBridge = m_pStd->GetBridge();	

	for(long nG=-1; nG<pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = (CPlateGirderApp *)pBridge->GetGirder(nG);
		for(long nC=0; nC<pGir->GetQtyHGusset(); nC++)
		{			
			CHBracingGusset *pHBra = pGir->GetHBracingGussetInTotal(nC);
			pHBra->m_nConnect = 0;
			pHBra->m_nMatchCol[CHBracingGusset::LEFTBOTTOM]  = 0;
			pHBra->m_nMatchCol[CHBracingGusset::LEFTTOP]     = 0;
			pHBra->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM] = 0;
			pHBra->m_nMatchCol[CHBracingGusset::RIGHTTOP]    = 0;			
		}
	}
}

void CHBracingPositionDlg::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	
	pDom->ClearEtt(TRUE);		

	CDomyun Dom(pDom);

	CAPlateDrawPyung Draw(m_pStd->GetDataManage());

	Draw.DrawPyung(&Dom, 5);

	*pDom << Dom;	

	CGeneralBaseDlg::DrawInputDomyunView(bZoomAll);
}
// 현재 이 함수는 사용하지 않음.
void CHBracingPositionDlg::DeleteNullColumn()
{
	long nEmptyCount = 0;
	for(long nCol=m_Grid.GetCols()-1; nCol>0; nCol--)
	{
		BOOL bEmpty = TRUE;
		for(long nRow=1; nRow<m_Grid.GetRows(); nRow++)
		{
			long nType = m_Grid.GetUserImage(nRow,nCol);
			if(nType!=CBracing::BRACTYPE_NONE) 
			{
				bEmpty = FALSE;
				break;
			}
		}
		if(bEmpty)	++nEmptyCount;
		else		break;		
	}
	m_Grid.SetColumnCount(m_Grid.GetColumnCount()-nEmptyCount);
}

// 그리드의 컬럼수는 가로보위치의 컬럼수와 동일하다.
// 대신 브레이싱타입이미지는 첫컬럼부터 차레때로 기록됨(중간에 없다가 빈자리로 남는것이 아님)
void CHBracingPositionDlg::SetGridTitle()
{
	UpdateData(TRUE);
	CPlateBridge *pBridge = m_pStd->GetBridge();
	
	long	nRows	= (pBridge->GetGirdersu()-1)*2+1;
	long	nCols	= pBridge->m_CrossAngleGrid.GetCols();

	if(nCols==0) return;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(FALSE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);	
	m_Grid.SetFixedRowCount(1);	
	m_Grid.SetFixedColumnCount(1);
	m_Grid.EnableReturnPass(TRUE);

	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);

	m_Grid.SetRowHeightAll(17);
	m_Grid.SetRowHeight(0, 20);

	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 65);

	m_Grid.SetTextMatrix(0,0,"구 분");

	CString szText;
	for(long nCol=1; nCol<nCols; nCol++)	
	{
		szText.Format("%d",nCol);
		m_Grid.SetTextMatrix(0,nCol,szText);
	}

	long nG=1;
	long nRow = 0;
	for(nRow=1;nRow<nRows-1;nRow+=2)
	{
		szText.Format("G%d-G%d", nG, nG+1);
		m_Grid.SetTextMatrix(nRow, 0, szText);
		m_Grid.SetMergeRow(nRow, nRow+1, 0);
		m_Grid.SetRowHeight(nRow+1, 17);
		nG++;
	}

	for(nRow=1; nRow<nRows-1;nRow+=2)
	{
		for(long nCol=1;nCol<nCols;nCol++)
		{
			m_Grid.SetCellType(nRow,nCol, CT_USERIMAGE);
			m_Grid.SetCellType(nRow+1,nCol, CT_COMBO);
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
	
}

// 정상적인 가로보(우측으로 매치가로보가 있는 경우) 만 수평브레이싱의 타입이미지를 삽입함.
void CHBracingPositionDlg::SetDataInit()
{
	CPlateBridge	*pDB	= m_pStd->GetBridge();
	CSteelMaterial	*pMat	= m_pStd->GetDataManage()->GetSteelMaterial();
	
	m_Grid.DeleteAllItems();

	SetGridTitle();

	long	nRows	= (pDB->GetGirdersu() - 1) * 2 + 1;
	long	nG		= 0;

	for(long nRow = 1; nRow < nRows - 1; nRow += 2)
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxNext;
		CPlateBasicIndex *pBxEnd = Finder.GetBxLast (BX_CROSSBEAM_VBRACING);	
		
		long nCol = 1;

		while(pBx)	// 마지막 전까지
		{
			pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
			if(pBxNext==NULL)	break;
			if(nCol > m_Grid.GetColumnCount() - 1) break;
			m_Grid.SetUserImage(nRow, nCol, pBx->GetBracing()->GetType());

			if(pMat->m_ptrArrStdSteel.GetSize()>0)
			{
				if(pBx->GetBracing()->GetType()==CBracing::BRACTYPE_NONE)
				{
					m_Grid.SetCellType(nRow+1, nCol, CT_EDIT);
					m_Grid.SetTextMatrix(nRow+1, nCol++, "-");
				}
				else
				{
					long nIdx = pBx->GetBracing()->m_nStdSteelIdx;
					if(nIdx>pMat->m_ptrArrStdSteel.GetSize()-1 || nIdx<0)
					{
						pBx->GetBracing()->m_nStdSteelIdx	= 0;
						nIdx = 0;
					}
					CStandardSteel	*pStandardSteel	= pMat->m_ptrArrStdSteel.GetAt(nIdx);
					m_Grid.SetCellType(nRow+1, nCol, CT_COMBO);
					m_Grid.SetTextMatrix(nRow+1, nCol++, pStandardSteel->m_szName);
				}
			}

			if(pBxNext->GetStation() >= pBxEnd->GetStation()) break;
			pBx = pBxNext;
		}
		nG++;
	}
	
	CString sText	= _T("");

	//////////////////////////////////////////////////////////////////////////
	m_nBaseHeightPosHBraceSplice	= pDB->m_nBaseHeightPosHBraceSplice;

	sText.Format("%.0lf",		pDB->m_dBaseHeightFixPosHBraceSplice);
	m_sBaseHeightFixPosHBraceSplice.SetWindowText(sText);

	sText.Format("%.0lf",	pDB->m_dBaseHeightRatioPosHBraceSplice);
	m_sBaseHeightRatioPosHBraceSplice.SetWindowText(sText);

	UpdateData(FALSE);
	
}

// 우측매치가로보가 있는 정상적인 브레이싱 역영의 타입만 설정함.
void CHBracingPositionDlg::SetDataDefault()
{
	CPlateBridgeApp		*pBridge		= m_pStd->GetBridge();		
	CPlateGirderApp		*pGirMatch		= NULL;	
	CPlateBasicIndex	*pBxMatch		= NULL;	
	CPlateBasicIndex	*pBxMatchNext	= NULL;	
	CSteelMaterial		*pMat			= m_pStd->GetDataManage()->GetSteelMaterial();
	
	pMat->StdMaterialRemoveAll();
	CStandardSteel		*pStdSteel	= new CStandardSteel;
	pStdSteel->m_szName			= _T("1");
	pStdSteel->m_nSteelIdx		= CSteelMaterialData::CTSTEEL;
	pStdSteel->m_nMaterialIdx	= 18;
	pMat->AddStandardSteelMaterial(pStdSteel);

	if(!pBridge->IsPlateGir())	//소수주형
	{
		for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
		{
			long nJ=0;
			CPlateGirderApp *pGir = pBridge->GetGirder(nG);
			CPlateBxFinder Finder(pGir);
			CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxNext=NULL;	
			CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);	
			// pBx는 Finder로 검색되는게 아니라 pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2) 함수로 진행.
			while(pBx)
			{			
				// 다음 가로보가 우측에 매치되는 가로보가 없을 경우 우측에 매치되는 가로보가 있을 경우까지 검색			
				pBxNext     = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
				pGirMatch   = pGir->m_pRight;
				pBxMatch    = pBx->GetBxMatchByCrossBeam(pGirMatch);
				pBxMatchNext= pBxNext->GetBxMatchByCrossBeam(pGirMatch);	// 매치되는 가로보가 항상 존재함

				pBx->GetBracing()->SetType(CBracing::BRACTYPE_NONE);
				
				if(pBxNext->GetStation() >= pBxEnd->GetStation())	break;	// 마지막 브레이싱 영역 일경우
				CSteelMaterialData	*pSteelMaterialData	= pBx->GetBracing()->GetSteelMaterialData();
				pSteelMaterialData->m_nType	= CSteelMaterialData::CTSTEEL;
				pMat->SetSteelMaterialDefault(pSteelMaterialData, CSteelMaterialData::CTSTEEL);
				pBx->GetBracing()->m_nStdSteelIdx	= 0;
				pBx = pBxNext;
			}
		}
	}
	else
	{
		for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
		{
			long				nJ		=	0;
			CPlateGirderApp		*pGir	=	pBridge->GetGirder(nG);

			CPlateBxFinder		Finder(pGir);

			CPlateBasicIndex	*pBx		= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxNext=NULL;	
			CPlateBasicIndex	*pBxEnd		= Finder.GetBxLast(BX_CROSSBEAM_VBRACING);	
			double				dStaJiJum	= pGir->GetStationOnJijum(nJ);

			// pBx는 Finder로 검색되는게 아니라 pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2) 함수로 진행.
			while(pBx)
			{			
				// 다음 가로보가 우측에 매치되는 가로보가 없을 경우 우측에 매치되는 가로보가 있을 경우까지 검색			
				pBxNext			= pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
				pGirMatch		= pGir->m_pRight;
				pBxMatch		= pBx->GetBxMatchByCrossBeam(pGirMatch);
				pBxMatchNext	= pBxNext->GetBxMatchByCrossBeam(pGirMatch);	// 매치되는 가로보가 항상 존재함

				if(nG==0)
				{
					if(pBxMatch != pBxMatchNext)		pBx->GetBracing()->SetType(CBracing::BRACTYPE_RIGHTWAVE);
				}
				else if(nG==pBridge->GetGirdersu()-2)
				{
					if(pBxMatch != pBxMatchNext)		pBx->GetBracing()->SetType(CBracing::BRACTYPE_LEFTWAVE);
				}
				else 
				{				
					if(EQUAL(pBx->GetStation(),dStaJiJum,EQUAL_VALUE_PLATE) || 
					  (pBxNext && EQUAL(pBxNext->GetStation(),dStaJiJum,EQUAL_VALUE_PLATE)))
					{
						if(nG%2)	
						{						
							if(pBxMatch != pBxMatchNext)	pBx->GetBracing()->SetType(CBracing::BRACTYPE_LEFTWAVE);					
						}
						else		
						{
							if(pBxMatch != pBxMatchNext)	pBx->GetBracing()->SetType(CBracing::BRACTYPE_RIGHTWAVE);
						}
						if(EQUAL(pBx->GetStation(),dStaJiJum,EQUAL_VALUE_PLATE))
							dStaJiJum = pGir->GetStationOnJijum(++nJ);
					}
				}

				CSteelMaterialData	*pSteelMaterialData	= pBx->GetBracing()->GetSteelMaterialData();

				pSteelMaterialData->m_nType	= CSteelMaterialData::CTSTEEL;

				pMat->SetSteelMaterialDefault(pSteelMaterialData, CSteelMaterialData::CTSTEEL);

				CBracing	*pBr = pBx->GetBracing();

				if(pBx->GetBracing()->GetType()==CBracing::BRACTYPE_NONE)
					pBx->GetBracing()->m_nStdSteelIdx	= -1;
				else
					pBx->GetBracing()->m_nStdSteelIdx	= 0;
				
				if(pBxNext->GetStation() >= pBxEnd->GetStation())	break;	// 마지막 브레이싱 영역 일경우
				
				pBx = pBxNext;
			}
		}
	}

	SetDataBx();
	SetDataInitStdSteel();
}


// BX_HBRACINGGUSSET는 가로보위치+가로보사이 위치임.
void CHBracingPositionDlg::SetDataBx()
{
	CPlateBridgeApp *pBridge = m_pStd->GetBridge();
	CPtrList NList;
	double dStaStt=0, dStaEnd=0, dStaMid=0;

	for(long nG=0; nG<pBridge->GetGirdersu(); nG++)
	{
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);	
		long nCount = 0;

		while(Finder.GetBxFixNext())
		{
			CPlateBasicIndex *pBxNext = Finder.GetBxFixNext();
			dStaStt = pBx->GetStation();
			dStaEnd = pBxNext->GetStation();
			dStaMid = (pBx->GetStation()+pBxNext->GetStation())/2;

			CPlateBasicIndex* p = pGir->newBx();			
			p->SetState(BX_HBRACINGGUSSET, TRUE);
			p->SetStation(dStaStt);
			p->SetAngle(pBx->GetAngle());
			NList.AddTail(p);
			++nCount;

			p = pGir->newBx();
			p->SetState(BX_HBRACINGGUSSET,TRUE);
			p->SetStation(dStaMid);
			p->SetAngle(pBx->GetAngle());
			NList.AddTail(p);
			++nCount;
			
			if(pBxNext->GetBxNext(BX_CROSSBEAM_VBRACING, 0)==NULL)
			{
				p = pGir->newBx();
				p->SetState(BX_HBRACINGGUSSET,TRUE);
				p->SetStation(dStaEnd);
				p->SetAngle(pBxNext->GetAngle());
				NList.AddTail(p);
				++nCount;
			}
			pBx = Finder.GetBxNext();
		}
		pGir->MakeHGusset(nCount);

		pGir->PressBxList(NList, BX_HBRACINGGUSSET);
		AhPtrListDelete(NList,(CPlateBasicIndex*)0);					

		// set LinkToBx
		pGir->SetLKPOS(CPlateBxObject::LK_HBRACING);		
	}
}

// pBx위치에서 횡브레이싱의 속성을 초기화함.
void CHBracingPositionDlg::RemoveHBracing(long nType, CPlateBasicIndex *pBx)
{	
	CPlateGirderApp  *pGir     = pBx->GetGirder();
	CPlateGirderApp  *pGirNext = pGir->m_pRight;
	CPlateBasicIndex *pBxNext  = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
	CHBracingGusset  *pHbGu=NULL, *pHbGuNext=NULL, *pHbGuMatch=NULL, *pHbGuMatchNext=NULL, *pHbGuMid=NULL, *pHbGuMatchMid=NULL;
	CPlateBasicIndex *pBxMatch=NULL, *pBxMatchNext=NULL;
	double dStaMid = 0;

	switch(nType)
	{			
	case CBracing::BRACTYPE_LEFTWAVE:
		if(pGirNext && pBx->GetHBracingGusset())
		{
			pHbGu   = pBx->GetHBracingGusset();
			dStaMid = (pBx->GetStation()+pBxNext->GetStation())/2;
			pHbGuMid= pGir->GetBxByStation(dStaMid, BX_HBRACINGGUSSET)->GetHBracingGusset();
			// 자기 연결 Count 증가
			if(pHbGuMid)
			{
				pHbGuMid->m_nConnect-=2;					
				if(pHbGuMid->m_nConnect < 0) pHbGuMid->m_nConnect  = 0;
				pHbGuMid->m_nMatchCol[CHBracingGusset::LEFTBOTTOM] = 0;
				pHbGuMid->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM]= 0;					
			}
			
			pBxMatch      = pBx->GetBxMatchByCrossBeam(pGirNext);
			pBxMatchNext  = pBxNext->GetBxMatchByCrossBeam(pGirNext);
			pHbGuMatch    = pBxMatch->GetHBracingGusset();
			pHbGuMatchNext= pBxMatchNext->GetHBracingGusset();
			if(pHbGuMatch)					
			{
				--pHbGuMatch->m_nConnect;	
				if(pHbGuMatch->m_nConnect < 0) pHbGuMatch->m_nConnect = 0;
				pHbGuMatch->m_nMatchCol[CHBracingGusset::RIGHTTOP] = 0;					
			}
			if(pHbGuMatchNext)
			{
				--pHbGuMatchNext->m_nConnect;	
				if(pHbGuMatchNext->m_nConnect < 0) pHbGuMatchNext->m_nConnect = 0;
				pHbGuMatchNext->m_nMatchCol[CHBracingGusset::LEFTTOP] = 0;
			}					
		}
		break;
	case CBracing::BRACTYPE_RIGHTWAVE:																
		if(pGirNext && pBx->GetHBracingGusset())
		{
			pHbGu     = pBx->GetHBracingGusset();
			pHbGuNext = pBxNext->GetHBracingGusset();
			// 자기 연결 Count 증가
			if(pHbGu) 
			{
				--pHbGu->m_nConnect;
				if(pHbGu->m_nConnect < 0) pHbGu->m_nConnect = 0;
				pHbGu->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM] = 0;
			}
			if(pHbGuNext)	
			{
				--pHbGuNext->m_nConnect;			
				if(pHbGuNext->m_nConnect < 0) pHbGuNext->m_nConnect = 0;
				pHbGuNext->m_nMatchCol[CHBracingGusset::LEFTBOTTOM] = 0;					
			}
			
			pBxMatch     = pBx->GetBxMatchByCrossBeam(pGirNext);
			pBxMatchNext = pBxNext->GetBxMatchByCrossBeam(pGirNext);
			dStaMid      = (pBxMatch->GetStation()+pBxMatchNext->GetStation())/2;
			pHbGuMatchMid= pGirNext->GetBxByStation(dStaMid, BX_HBRACINGGUSSET)->GetHBracingGusset();
			if(pHbGuMatchMid)					
			{
				pHbGuMatchMid->m_nConnect-=2;
				if(pHbGuMatchMid->m_nConnect < 0) pHbGuMatchMid->m_nConnect = 0;
				pHbGuMatchMid->m_nMatchCol[CHBracingGusset::LEFTTOP] = 0;				
				pHbGuMatchMid->m_nMatchCol[CHBracingGusset::RIGHTTOP] = 0;
			}					
		}
		break;
	case CBracing::BRACTYPE_CROSS:
		{
			pHbGu    = pBx->GetHBracingGusset();
			pHbGuNext= pBxNext->GetHBracingGusset();
			// 자기 연결 Count 증가
			if(pHbGu)					
			{
				--pHbGu->m_nConnect;
				if(pHbGu->m_nConnect < 0) pHbGu->m_nConnect = 0;
				pHbGu->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM]   = 0;
			}
			if(pHbGuNext)
			{
				--pHbGuNext->m_nConnect;					
				if(pHbGuNext->m_nConnect < 0) pHbGuNext->m_nConnect  = 0;
				pHbGuNext->m_nMatchCol[CHBracingGusset::LEFTBOTTOM]  = 0;					
			}

			pHbGuMatch     = pBx->GetBxMatchByCrossBeam(pGirNext)->GetHBracingGusset();
			pHbGuMatchNext = pBxNext->GetBxMatchByCrossBeam(pGirNext)->GetHBracingGusset();
			if(pHbGuMatch)					
			{
				--pHbGuMatch->m_nConnect;
				if(pHbGuMatch->m_nConnect < 0) pHbGuMatch->m_nConnect = 0;
				pHbGuMatch->m_nMatchCol[CHBracingGusset::RIGHTTOP]   = 0;
			}	
			if(pHbGuMatchNext)					
			{
				--pHbGuMatchNext->m_nConnect;
				if(pHbGuMatchNext->m_nConnect < 0) pHbGuMatchNext->m_nConnect = 0;
				pHbGuMatchNext->m_nMatchCol[CHBracingGusset::LEFTTOP]   = 0;
			}	
		}
		break;
	case CBracing::BRACTYPE_SLOPLEFT:
		{
			pHbGu = pBx->GetHBracingGusset();
			// 자기 연결 Count 증가
			if(pHbGu)					
			{
				--pHbGu->m_nConnect;
				if(pHbGu->m_nConnect < 0) pHbGu->m_nConnect = 0;
				pHbGu->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM]   = 0;	
			}
			
			pHbGuMatchNext = pBxNext->GetBxMatchByCrossBeam(pGirNext)->GetHBracingGusset();			
			if(pHbGuMatchNext)					
			{
				--pHbGuMatchNext->m_nConnect;
				if(pHbGuMatchNext->m_nConnect < 0) pHbGuMatchNext->m_nConnect = 0;
				pHbGuMatchNext->m_nMatchCol[CHBracingGusset::LEFTTOP]   = 0;
			}	
		}
		break;
	case CBracing::BRACTYPE_SLOPRIGHT:
		{
			pHbGu    = pBx->GetHBracingGusset();
			pHbGuNext= pBxNext->GetHBracingGusset();
			// 자기 연결 Count 증가
			if(pHbGuNext)
			{
				--pHbGuNext->m_nConnect;	
				if(pHbGuNext->m_nConnect < 0) pHbGuNext->m_nConnect = 0;
				pHbGuNext->m_nMatchCol[CHBracingGusset::LEFTBOTTOM]   = 0;
			}

			pHbGuMatch = pBx->GetBxMatchByCrossBeam(pGirNext)->GetHBracingGusset();
			if(pHbGuMatch)					
			{
				--pHbGuMatch->m_nConnect;
				if(pHbGuMatch->m_nConnect < 0) pHbGuMatch->m_nConnect = 0;
				pHbGuMatch->m_nMatchCol[CHBracingGusset::RIGHTTOP]   = 0;
			}						
		}
		break;					
	}
}

void CHBracingPositionDlg::GetSteelType(CString szTypeName, long &nSteelType, long &nJewonIdx, BOOL bSteel)
{
	CSteelMaterial	*pMat		= m_pStd->GetDataManage()->GetSteelMaterial();
	CString	strSteel	= _T("");
	CString	strSteelMat	= _T("");
	if(bSteel)
	{
		for(long nR=0;nR<m_GridStd.GetRowCount();nR++)
		{
			if(m_GridStd.GetTextMatrix(nR,0)==szTypeName)
			{
				strSteel	= m_GridStd.GetTextMatrix(nR, 1);
				strSteelMat	= m_GridStd.GetTextMatrix(nR, 2);
				break;
			}
		}
		nSteelType	= pMat->GetTypeNameIndex(strSteel);
		nJewonIdx	= pMat->GetJewonIndex(nSteelType, strSteelMat);
	}
	else
	{
		nSteelType	= CSteelMaterialData::CTSTEEL;
		nJewonIdx	= 18;
	}

}


void CHBracingPositionDlg::SetDataSaveStandardSteel()
{
	CSteelMaterial	*pMat			= m_pStd->GetDataManage()->GetSteelMaterial();

	CString szTypeName	= _T("");
	long	nSteelType	= 0;
	long	nJewonIdx	= 0;

	for(long nRow = 1; nRow < m_GridStd.GetRowCount(); nRow++)
	{
		CStandardSteel	*pStandardSteel	= pMat->m_ptrArrStdSteel.GetAt(nRow-1);
		
		szTypeName	= m_GridStd.GetTextMatrix(nRow, 0);

		GetSteelType(szTypeName, nSteelType, nJewonIdx, TRUE);
		
		pStandardSteel->m_szName		= szTypeName;
		pStandardSteel->m_nSteelIdx		= nSteelType;
		pStandardSteel->m_nMaterialIdx	= nJewonIdx;
	}		

}

void CHBracingPositionDlg::SetDataSaveSteelMaterial(CBracing *pBracing, long nRow, long nCol)
{
	CSteelMaterial		*pMat			= m_pStd->GetDataManage()->GetSteelMaterial();
	CSteelMaterialData	*pCurMatData	= pBracing->GetSteelMaterialData();

	CString	szTypeName	= m_Grid.GetTextMatrix(nRow, nCol);
	long	nSteelType	= 0;
	long	nJewonIdx	= 0;
	long	nSecType	= pBracing->GetType();

	if(nSecType==CBracing::BRACTYPE_NONE)
	{
		pBracing->m_nStdSteelIdx			= -1;
		pCurMatData->m_nType				= 0;
		pCurMatData->m_nJewonIdx			= 0;
		pCurMatData->m_dH					= 0;
		pCurMatData->m_dB					= 0;
		pCurMatData->m_dT1					= 0;
		pCurMatData->m_dT2					= 0;
		pCurMatData->m_dR1					= 0;
		pCurMatData->m_dSectionArea			= 0;
		pCurMatData->m_dUnitWeight			= 0;
		pCurMatData->m_d2MomentOfInertia.x	= 0;
		pCurMatData->m_d2MomentOfInertia.y	= 0;
		pCurMatData->m_dRadiusOfGyration.x	= 0;
		pCurMatData->m_dRadiusOfGyration.y	= 0;
		pCurMatData->m_dModulusOfSection.x	= 0;
		pCurMatData->m_dModulusOfSection.y	= 0;
		return;
	}

	GetSteelType(szTypeName, nSteelType, nJewonIdx, TRUE);

	CSteelMaterialData	*pMatData		= pMat->GetMatData(nSteelType, nJewonIdx);
	pBracing->m_nStdSteelIdx			= pMat->GetStandardSteelIdx(szTypeName);
	pCurMatData->m_nType				= nSteelType;
	pCurMatData->m_nJewonIdx			= nJewonIdx;
	pCurMatData->m_dH					= pMatData->m_dH;				
	pCurMatData->m_dB					= pMatData->m_dB;
	pCurMatData->m_dT1					= pMatData->m_dT1;
	pCurMatData->m_dT2					= pMatData->m_dT2;
	pCurMatData->m_dR1					= pMatData->m_dR1;
	pCurMatData->m_dSectionArea			= pMatData->m_dSectionArea;
	pCurMatData->m_dUnitWeight			= pMatData->m_dUnitWeight;
	pCurMatData->m_d2MomentOfInertia.x	= pMatData->m_d2MomentOfInertia.x;
	pCurMatData->m_d2MomentOfInertia.y	= pMatData->m_d2MomentOfInertia.y;
	pCurMatData->m_dRadiusOfGyration.x	= pMatData->m_dRadiusOfGyration.x;
	pCurMatData->m_dRadiusOfGyration.y	= pMatData->m_dRadiusOfGyration.y;
	pCurMatData->m_dModulusOfSection.x	= pMatData->m_dModulusOfSection.x;
	pCurMatData->m_dModulusOfSection.y	= pMatData->m_dModulusOfSection.y;
}

// 우측매치가로보가 있는 정상적인 브레이싱 역영의 타입만 설정함.
void CHBracingPositionDlg::SetDataSave()
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CString			sText	= _T("");

	ClearAll();

	pDB->m_nBaseHeightPosHBraceSplice		= m_nBaseHeightPosHBraceSplice;

	m_sBaseHeightFixPosHBraceSplice.GetWindowText(sText);
	pDB->m_dBaseHeightFixPosHBraceSplice	= atof(sText);

	m_sBaseHeightRatioPosHBraceSplice.GetWindowText(sText);
	pDB->m_dBaseHeightRatioPosHBraceSplice	= atof(sText);

	double dStaMid=0, dStaMidMatch=0;
	CHBracingGusset *pHbGu=NULL, *pHbGuNext=NULL, *pHbGuMatch=NULL, *pHbGuMatchNext=NULL, *pHbGuMid=NULL, *pHbGuMatchMid=NULL;
	CPlateBasicIndex *pBx=NULL, *pBxNext=NULL, *pBxMatch=NULL, *pBxMatchNext=NULL;

	SetDataSaveStandardSteel();
	long nG = 0;
	for(nG=0; nG<pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp *pGir     = pDB->GetGirder(nG);		
		CPlateGirderApp *pGirNext = pGir->m_pRight;

		CPlateBxFinder Finder(pGir);
		pBx = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);		
		CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);
		long nCol = 1;
		long nType= 0;
		// 가로보BX로 진행하는게 아니라 브레이싱영역(매치가로보가 우측에 있는 경우)으로 진행.

		
		while(pBx)	
		{
			pBxNext		  = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
			pBxMatch      = pBx->GetBxMatchByCrossBeam(pGirNext);
			pBxMatchNext  = pBxNext->GetBxMatchByCrossBeam(pGirNext);
			dStaMid       = (pBx->GetStation()+pBxNext->GetStation())/2;
			dStaMidMatch  = (pBxMatch->GetStation()+pBxMatchNext->GetStation())/2;
			
			pHbGu		  = pBx->GetHBracingGusset();
			pHbGuMid      = pGir->GetBxByStation(dStaMid, BX_HBRACINGGUSSET)->GetHBracingGusset();
			pHbGuNext     = pBxNext->GetHBracingGusset();
			pHbGuMatch    = pBxMatch->GetHBracingGusset();
			pHbGuMatchMid = pGirNext->GetBxByStation(dStaMidMatch, BX_HBRACINGGUSSET)->GetHBracingGusset();
			pHbGuMatchNext= pBxMatchNext->GetHBracingGusset();

			RemoveHBracing(pBx->GetBracing()->GetType(), pBx);

			if(nCol > m_Grid.GetCols() - 1)  	nType = 0;
			else								nType = m_Grid.GetUserImage(nG*2+1,nCol);

			pBx->GetBracing()->SetType(nType);

			switch(nType)
			{			
			case CBracing::BRACTYPE_LEFTWAVE:
				if(pGirNext && pHbGu)
				{
					if(pHbGuMid)
					{
						pHbGuMid->m_nConnect+=2;					
						pHbGuMid->m_nMatchCol  [CHBracingGusset::LEFTBOTTOM]  = 1;
						pHbGuMid->m_nMatchCol  [CHBracingGusset::RIGHTBOTTOM] = 1;				
					}
					if(pHbGuMatch)					
					{
						++pHbGuMatch->m_nConnect;			
						pHbGuMatch->m_nMatchCol[CHBracingGusset::RIGHTTOP]   = 1;	
					}
					if(pHbGuMatchNext)
					{
						++pHbGuMatchNext->m_nConnect;					
						pHbGuMatchNext->m_nMatchCol[CHBracingGusset::LEFTTOP]   = 1;
					}					
				}
				break;
			case CBracing::BRACTYPE_RIGHTWAVE:																
				if(pGirNext && pHbGu)
				{
					if(pHbGu)					
					{
						++pHbGu->m_nConnect;
						pHbGu->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM]   = 1;
					}
					if(pHbGuNext)
					{
						++pHbGuNext->m_nConnect;					
						pHbGuNext->m_nMatchCol[CHBracingGusset::LEFTBOTTOM]   = 1;	
					}
					if(pHbGuMatchMid)					
					{
						pHbGuMatchMid->m_nConnect+=2;
						pHbGuMatchMid->m_nMatchCol[CHBracingGusset::LEFTTOP]   = 1;				
						pHbGuMatchMid->m_nMatchCol[CHBracingGusset::RIGHTTOP]  = 1;
					}					
				}
				break;
			case CBracing::BRACTYPE_CROSS:
				{
					if(pHbGu)					
					{
						++pHbGu->m_nConnect;
						pHbGu->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM]   = 1;

					}
					if(pHbGuNext)
					{
						++pHbGuNext->m_nConnect;					
						pHbGuNext->m_nMatchCol[CHBracingGusset::LEFTBOTTOM]   = 1;	
					}
					if(pHbGuMatch)					
					{
						++pHbGuMatch->m_nConnect;
						pHbGuMatch->m_nMatchCol[CHBracingGusset::RIGHTTOP]   = 1;
					}	
					if(pHbGuMatchNext)					
					{
						++pHbGuMatchNext->m_nConnect;
						pHbGuMatchNext->m_nMatchCol[CHBracingGusset::LEFTTOP]   = 1;			
					}	
				}
				break;
			case CBracing::BRACTYPE_SLOPLEFT:
				{
					if(pHbGu)					
					{
						++pHbGu->m_nConnect;
						pHbGu->m_nMatchCol[CHBracingGusset::RIGHTBOTTOM]   = 1;
					}
					if(pHbGuMatchNext)					
					{					
						++pHbGuMatchNext->m_nConnect;
						pHbGuMatchNext->m_nMatchCol[CHBracingGusset::LEFTTOP]   = 1;	
					}	
				}
				break;
			case CBracing::BRACTYPE_SLOPRIGHT:
				{
					if(pHbGuNext)
					{
						++pHbGuNext->m_nConnect;					
						pHbGuNext->m_nMatchCol[CHBracingGusset::LEFTBOTTOM]   = 1;
					}
					if(pHbGuMatch)					
					{						
						++pHbGuMatch->m_nConnect;
						pHbGuMatch->m_nMatchCol[CHBracingGusset::RIGHTTOP]   = 1;
					}						
				}
				break;
			}			

			SetDataSaveSteelMaterial(pBx->GetBracing(), nG * 2 + 2, nCol++);
			if(pBxNext->GetStation() >= pBxEnd->GetStation())
				break;	// 마지막 브레이싱 영역 일경우

			pBx = pBxNext;
		}		
	}	

	for(nG=0; nG<pDB->GetGirdersu(); nG++)		
	{
		CPlateGirderApp *pGir = pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);	
		while(pBx)
		{
			SetDataCalcHBracing(pBx);
			pBx = Finder.GetBxNext();
		}
	}

	
}

void CHBracingPositionDlg::OnPreInitDialog()
{
	if(!m_ImageList.GetSafeHandle())
		m_ImageList.Create(IDB_BITMAP_BRACING_TYPE, 32, 1, RGB(255,255,255));
	m_Grid.SetUserImageList(&m_ImageList);

	SetResize(IDC_BUTTON_REMOVEALL,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAMETYPE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_TITLE1, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_BASEPOS_UU, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_BASEPOS_DU, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_HEIGHT_RATIO, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_BASE_HEIGHT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_BASE_RATIO, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_MM, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_PER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_GRID2, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_BUTTON_ADD_STEEL,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_DEL_STEEL,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetDataInitStdSteel();
}

void CHBracingPositionDlg::OnCellChanged(NMHDR* nmgv,LRESULT*)
{			
	CPlateBridgeApp	*pBridge = m_pStd->GetBridge();

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)nmgv;	
	int nRow = GridView->iRow;
    int nCol = GridView->iColumn;

	if(nRow==0)	return;
			
	CDomyun *pDom = m_pView->GetDomyun();
	long	nG	= (long)(nRow/2.0+0.5)-1;
	CPlateGirderApp *pGir = pBridge->GetGirder(nG);
	// nCol값과 매치하는 정상적인 가로보위치를 찾아서 보여줌(이미지 삽입시 정상적인 가로보만 삽입했기 때문에)
	CPlateBxFinder Finder(pGir);
	CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);		
	CPlateBasicIndex *pBxEnd = Finder.GetBxLast (BX_CROSSBEAM_VBRACING), *pBxNext;
	long nPos=0;
	while(pBx)	
	{
		pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
		if(pBxNext->GetStation() >= pBxEnd->GetStation())	break;	// 마지막 브레이싱 영역 일경우
		
		nPos++;
		if(nPos==nCol)
			break;
		pBx = pBxNext;
	}
	//
	pDom->ClearEtt(FALSE);		
	
	CAPlateDrawPyung Draw(m_pStd->GetDataManage());	
	pDom->FixEtt(TRUE);
	
	CDomyun Dom(pDom);
	Dom.SetLineColor(1);
	Draw.DrawHBracingCLWithenBx(&Dom, pBx, pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2), TRUE);
	Dom.Rotate(CDPoint (0,0), pBridge->GetAngleBridgeCenter().GetMirrorVert());	

	*pDom << Dom;
	
	pDom->FixEtt(FALSE);	
	m_pView->RedrawAll();
	
}

// 횡브레이싱 이음판의 사재의 각도를 계산함.
// pBrGu->xyDir[0~3]
// 0 : 좌상, 1: 우상, 2: 좌하, 3: 우하
#pragma optimize("", off)
void CHBracingPositionDlg::SetDataCalcHBracing(CPlateBasicIndex *pBx)
{
	CPlateGirderApp* pGir    = pBx->GetGirder();
	CPlateGirderApp* pGirL   = pGir->m_pLeft;
	CPlateGirderApp* pGirR   = pGir->m_pRight;
	CHBracingGusset* pBrGu   = pBx->GetHBracingGusset();
	//
	if(!pBrGu->IsReal()) return;	// m_nMatchCol이 모두 0일경우
	CPlatePieceObject pObj1(pGir), pObj2(pGir);

	pBrGu->xyDir[0] = CDPoint(0,0);
	pBrGu->xyDir[1] = CDPoint(0,0);
	pBrGu->xyDir[2] = CDPoint(0,0);
	pBrGu->xyDir[3] = CDPoint(0,0);

	// 횡브레이싱 이음판의 오브젝트를 생성하는 과정에서 사재의 각도가 저장됨
	pBrGu->CreatePieceObj(&pObj1, &pObj2, FALSE, FALSE);
}
#pragma optimize("", on)

void CHBracingPositionDlg::OnButtonRemoveAllHBracing() 
{
	CPlateBridgeApp  *pBridge      = m_pStd->GetBridge();		
	CPlateGirderApp  *pGirMatch    = NULL;	
	CPlateBasicIndex *pBxMatch     = NULL;	
	CPlateBasicIndex *pBxMatchNext = NULL;	

	for(long nG=0; nG<pBridge->GetGirdersu()-1; nG++)
	{
		long nJ=0;
		CPlateGirderApp *pGir = pBridge->GetGirder(nG);
		CPlateBxFinder Finder(pGir);
		CPlateBasicIndex *pBx    = Finder.GetBxFirst(BX_CROSSBEAM_VBRACING), *pBxNext=NULL;	
		CPlateBasicIndex *pBxEnd = Finder.GetBxLast(BX_CROSSBEAM_VBRACING);	
		// pBx는 Finder로 검색되는게 아니라 pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2) 함수로 진행.
		while(pBx)
		{			
			// 다음 가로보가 우측에 매치되는 가로보가 없을 경우 우측에 매치되는 가로보가 있을 경우까지 검색			
			pBxNext     = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
			pGirMatch   = pGir->m_pRight;
			pBxMatch    = pBx->GetBxMatchByCrossBeam(pGirMatch);
			pBxMatchNext= pBxNext->GetBxMatchByCrossBeam(pGirMatch);	// 매치되는 가로보가 항상 존재함
			
			pBx->GetBracing()->SetType(CBracing::BRACTYPE_NONE);
			
			if(pBxNext->GetStation() >= pBxEnd->GetStation())	break;	// 마지막 브레이싱 영역 일경우

			pBx = pBxNext;
		}
	}	

	SetDataInit();
	SetDataSave();

	DrawInputDomyunView(TRUE);
}

BOOL CHBracingPositionDlg::PreTranslateMessage(MSG* pMsg) 
{
	CSteelMaterial	*pMat	= m_pStd->GetDataManage()->GetSteelMaterial();
	CStringArray strCombo;

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		long	nRow	= m_Grid.GetFocusCell().row;
		long	nCol	= m_Grid.GetFocusCell().col;

		strCombo.RemoveAll();
		if(m_Grid.GetCellType(nRow,nCol)==CT_COMBO)
		{
			for(long nCount=0;nCount<pMat->m_ptrArrStdSteel.GetSize();nCount++)
			{
				CStandardSteel	*pStandardSteel	= pMat->m_ptrArrStdSteel.GetAt(nCount);
				strCombo.Add(pStandardSteel->m_szName);
			}
			m_Grid.SetComboString(strCombo);
		}
	}

	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		long	nStdRow	= m_GridStd.GetFocusCell().row;
		long	nStdCol	= m_GridStd.GetFocusCell().col;
		
		if(nStdCol==1)
		{
			for(long nCount=0; nCount<CSteelMaterialData::STEELCOUNT; nCount++)
				strCombo.Add(pMat->GetTypeName(nCount));
			m_GridStd.SetComboString(strCombo);
		}
		
		if(nStdCol==2)
		{
			long nSteelType = pMat->GetTypeNameIndex(m_GridStd.GetTextMatrix(nStdRow, 1));
			
			for(long nCount=0; nCount<pMat->GetSizeJewon(nSteelType); nCount++)
				strCombo.Add(pMat->GetJewonDataString(nSteelType, nCount));
			m_GridStd.SetComboString(strCombo);
		}
	}

	return CGeneralBaseDlg::PreTranslateMessage(pMsg);
}

void CHBracingPositionDlg::OnButtonSametype() 
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CCellID			Cell	= m_Grid.GetFocusCell();

	long	nRow	= Cell.row;
    long	nCol	= Cell.col;
	long	nG		= (long)(nRow/2.0+0.5)-1;
	long	nPos	= 0;

	if(nG<0 || nCol<0)	return;

	CPlateGirderApp		*pGir		= pDB->GetGirder(nG);
	CBracing			*pCurBr		= pGir->GetBracingInTotal(nCol-1);
	CPlateBxFinder		Finder(pGir);
	CPlateBasicIndex	*pBx		= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex	*pBxEnd		= Finder.GetBxLast (BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex	*pBxNext	= NULL;

	while(pBx)
	{
		pBxNext = pBx->GetBxNext(BX_CROSSBEAM_VBRACING, 2);
		if(pBxNext->GetStation() >= pBxEnd->GetStation())
			break;	// 마지막 브레이싱 영역 일경우
		
		nPos++;
		if(nPos==nCol)
			break;
		pBx = pBxNext;
	}

	CTypedPtrArray <CObArray, CBracing*> ptrArrBx;
	long	nCurPos		= GetHBracingPosition(pBx);
	long	nHBrPos		= 0;
	CString	szMessage	= _T("");
	CString	szText		= _T("");
	BOOL	bExist		= FALSE;
	long	nSize		= 0;

	if(nCurPos==POS_STT_END)		szMessage = "< 시종점부 >\n";
	else if(nCurPos==POS_CEN_JIJUM)	szMessage = "< 중앙지점부 >\n";
	else if(nCurPos==POS_GENERAL)	szMessage = "< 일반부 >\n";

	if(nCurPos==POS_STT_END)
	{
		for(long nG=0; nG < pDB->GetGirdersu()-1; nG++)
		{
			pGir	= pDB->GetGirder(nG);
			nSize	= pGir->GetQtyCrossBeamVBracing()-1;

			szText.Format("G%d - G%d : 1\n", nG+1, nG+2);
			szMessage += szText;
			ptrArrBx.Add(pGir->GetBracingInTotal(0));

			szText.Format("G%d - G%d : %d\n", nG+1, nG+2, pGir->GetQtyCrossBeamVBracing()-1);
			szMessage += szText;
			ptrArrBx.Add(pGir->GetBracingInTotal(nSize-1));
		}
	}
	else if(nCurPos==POS_GENERAL)
	{
		for(long nG=0; nG < pDB->GetGirdersu()-1; nG++)
		{
			pGir	= pDB->GetGirder(nG);
			nHBrPos	= 0;
			for(long n=0;n<pGir->GetQtyCrossBeamVBracing()-1; n++)
			{
				nHBrPos++;
				CBracing		*pBr	= pGir->GetBracingInTotal(n);
				if(pBr->GetType()==CBracing::BRACTYPE_NONE)	continue;
				CPlateBasicIndex	*pBx	= pBr->GetBx();
				if(!pBx->IsJijum() && !pBx->GetBxNext(BX_CROSSBEAM_VBRACING)->IsJijum())
				{
					szText.Format("G%d - G%d : %d\n", nG+1, nG+2, nHBrPos+1);
					szMessage += szText;
					ptrArrBx.Add(pBx->GetHBracingGusset()->GetBxBracing()->GetBracing());
				}
			}
		}
	}
	else if(nCurPos==POS_CEN_JIJUM)
	{
		for(long nG=0; nG < pDB->GetGirdersu()-1; nG++)
		{
			pGir	= pDB->GetGirder(nG);
			nHBrPos	= 0;
			for(long n=1;n<pGir->GetQtyCrossBeamVBracing()-2; n++)
			{
				nHBrPos++;
				CBracing		*pBr	= pGir->GetBracingInTotal(n);
				if(pBr->GetType()==CBracing::BRACTYPE_NONE)	continue;
				CPlateBasicIndex	*pBx	= pBr->GetBx();
				if(pBx->IsJijum() || pBx->GetBxNext(BX_CROSSBEAM_VBRACING)->IsJijum())
				{
					szText.Format("G%d - G%d : %d\n", nG+1, nG+2, nHBrPos+1);
					szMessage += szText;
					ptrArrBx.Add(pBx->GetHBracingGusset()->GetBxBracing()->GetBracing());
				}
			}
		}
	}
	if(ptrArrBx.GetSize()>0)	bExist = TRUE;

	if(!bExist)
	{
		szMessage = "동일 구간의 같은 타입이 없습니다.";
		MessageBox(szMessage,"수평브레이싱 배치",MB_ICONINFORMATION );
		return;
	}
	else szMessage += "위 수평브레이싱의 강종을 동일적용합니다.\n계속 하시겠습니까?";
	if( MessageBox(szMessage,"수평브레이싱 배치",MB_ICONQUESTION | MB_YESNO ) == IDNO) return;

	CSteelMaterialData	*pMatData	= pCurBr->GetSteelMaterialData();
	for(long n=0; n<ptrArrBx.GetSize(); n++)
	{
		CBracing	*pBr = ptrArrBx.GetAt(n);
		pBr->SetSteelMaterialData(pMatData);
		pBr->m_nStdSteelIdx	= pCurBr->m_nStdSteelIdx;	
	}
	SetDataInit();
}

long CHBracingPositionDlg::GetHBracingPosition(CPlateBasicIndex *pBx)
{
	long	nPosition = -1;
	long	nG	= pBx->GetNumberGirder();
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
	CPlateBxFinder	Finder(pGir);
	CPlateBasicIndex	*pBxFir	= Finder.GetBxFirst(BX_CROSSBEAM_VBRACING);
	CPlateBasicIndex	*pBxEnd	= Finder.GetBxLast(BX_CROSSBEAM_VBRACING);

	if(pBx==pBxFir || pBx==pBxEnd)
		nPosition	= POS_STT_END;
	else if(pBx->IsJijum() || pBx->GetBxNext(BX_CROSSBEAM_VBRACING)->IsJijum())
		nPosition	= POS_CEN_JIJUM;
	else
		nPosition	= POS_GENERAL;

	return	nPosition;
}

LRESULT CHBracingPositionDlg::OnHBracingInputClose(WPARAM wp,LPARAM lp)
{	
	UpdateData(FALSE);
	SetGridTitle();	
	SetDataInit();
	
	return 0;
}

void CHBracingPositionDlg::OnButtonNext() 
{

	CGeneralBaseDlg::OnButtonNext();	
}

void CHBracingPositionDlg::OnButtonPrev() 
{	
	CGeneralBaseDlg::OnButtonPrev();		
}

void CHBracingPositionDlg::OnDestroy() 
{
	CGeneralBaseDlg::OnDestroy();
}

void CHBracingPositionDlg::OnRadioBaseposUpperGirderLower() 
{
	UpdateData();
}

void CHBracingPositionDlg::OnRadioBaseposLowerGirderUpper() 
{
	UpdateData();	
}

void CHBracingPositionDlg::OnRadioHeightByRato() 
{
	UpdateData();	
}

void CHBracingPositionDlg::SetHBracingEle()
{
	UpdateData();
	
	CString			sText	= _T("");
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();

	pDB->m_nBaseHeightPosHBraceSplice		= m_nBaseHeightPosHBraceSplice;

	m_sBaseHeightFixPosHBraceSplice.GetWindowText(sText);
	pDB->m_dBaseHeightFixPosHBraceSplice	= atof(sText);

	m_sBaseHeightRatioPosHBraceSplice.GetWindowText(sText);
	pDB->m_dBaseHeightRatioPosHBraceSplice	= atof(sText);
	
	for(long nG=0; nG<pDB->GetQtyGirder(); nG++)
	{
		CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
		CPlateBxFinder Finder(pGir);

		CPlateBasicIndex *pBx = Finder.GetBxFirst(BX_HBRACINGGUSSET);
		while(pBx)
		{
			CHBracingGusset *pBrGu = pBx->GetHBracingGusset();

			pBrGu->m_ddH = pBx->GetOffsetFromLowerFlange();
			pBx = Finder.GetBxNext();
		}
	}
}

void CHBracingPositionDlg::OnKillfocusEditBaseHeight() 
{
	SetHBracingEle();
}

void CHBracingPositionDlg::OnKillfocusEditBaseRatio() 
{
	SetHBracingEle();
}

void CHBracingPositionDlg::SetGridTitleStdSteel()
{
	if(!m_pStd)	return;
	
	CSteelMaterial	*pMat	= m_pStd->GetDataManage()->GetSteelMaterial();

	long	nSize	= pMat->m_ptrArrStdSteel.GetSize();
	long	nRows	= nSize+1;
	long	nCols	= 3;

	m_GridStd.SetEditable(TRUE);
	m_GridStd.SetListMode(FALSE);
	m_GridStd.EnableDragAndDrop(TRUE);
	m_GridStd.EnableInsertRow(FALSE);
	m_GridStd.EnableInsertCol(FALSE);	
	m_GridStd.SetFixedRowCount(1);
	m_GridStd.EnableReturnPass(TRUE);
	m_GridStd.SetRowCount(nRows);
	m_GridStd.SetColumnCount(nCols);
	m_GridStd.SetRowHeightAll(17);
	m_GridStd.SetTextMatrix(0, 0, "타입");
	m_GridStd.SetTextMatrix(0, 1, "형 강");
	m_GridStd.SetTextMatrix(0, 2, "제 원");

	m_GridStd.SetColumnWidth(0, 35);
	m_GridStd.SetColumnWidth(1, 120);
	m_GridStd.SetColumnWidth(2,	143);

	for(long n = 1 ; n < nSize+1 ; n++)
	{
		m_GridStd.SetCellType(n, 1, CT_COMBO);
		m_GridStd.SetCellType(n, 2, CT_COMBO);
	}

	m_GridStd.SetRedraw(TRUE, TRUE);

}
void CHBracingPositionDlg::SetDataInitStdSteel()
{
	if(!m_pStd)	return;

	CSteelMaterial	*pMat	= m_pStd->GetDataManage()->GetSteelMaterial();
	
	SetGridTitleStdSteel();

	for(long n=0; n<pMat->m_ptrArrStdSteel.GetSize();n++)
	{
		CStandardSteel	*pStdSteel	= pMat->m_ptrArrStdSteel.GetAt(n);

		m_GridStd.SetTextMatrix(n+1, 0, pStdSteel->m_szName);
		m_GridStd.SetTextMatrix(n+1, 1, pMat->GetTypeName(pStdSteel->m_nSteelIdx));
		m_GridStd.SetTextMatrix(n+1, 2, pMat->GetJewonDataString(pStdSteel->m_nSteelIdx, pStdSteel->m_nMaterialIdx));
	}
}
void CHBracingPositionDlg::OnCellChangedDataGridStdSteel(NMHDR* nmgv,LRESULT*)
{
	NM_GRIDVIEW		*pGrid			= (NM_GRIDVIEW *)nmgv;
	long			nRow			= pGrid->iRow;
	long			nCol			= pGrid->iColumn;
	CSteelMaterial	*pMat			= m_pStd->GetDataManage()->GetSteelMaterial();
	CStandardSteel	*pStandardSteel	= pMat->m_ptrArrStdSteel.GetAt(nRow-1);
	
	if(nCol==0)
	{
		pStandardSteel->m_szName		= m_GridStd.GetTextMatrix(nRow, nCol);
	}
	else if(nCol==1)
	{
		pStandardSteel->m_nSteelIdx		= pMat->GetTypeNameIndex(m_GridStd.GetTextMatrix(nRow, nCol));
		pStandardSteel->m_nMaterialIdx	= 0;
	}
	else if(nCol==2)
	{
		pStandardSteel->m_nMaterialIdx	= pMat->GetJewonIndex(pStandardSteel->m_nSteelIdx, m_GridStd.GetTextMatrix(nRow, nCol));
	}
	
	
	SetDataInitStdSteel();
	SetDataSaveStandardSteel();
	
}


void CHBracingPositionDlg::OnButtonAddSteel() 
{
	CSteelMaterial	*pMat	= m_pStd->GetDataManage()->GetSteelMaterial();
	
	m_GridStd.SetRowCount(m_GridStd.GetRowCount()+1);
	m_GridStd.SetRedraw(TRUE,TRUE);
	
	CStandardSteel		*pStdSteel	= new CStandardSteel;
	
	pStdSteel->m_szName.Format("%d", pMat->m_ptrArrStdSteel.GetSize()+1);
	pStdSteel->m_nSteelIdx		= CSteelMaterialData::CTSTEEL;
	pStdSteel->m_nMaterialIdx	= 18;
	pMat->AddStandardSteelMaterial(pStdSteel);
	
	SetDataInitStdSteel();	
}

void CHBracingPositionDlg::OnButtonDelSteel() 
{
	CPlateBridgeApp	*pDB	= m_pStd->GetBridge();
	CSteelMaterial	*pMat	= m_pStd->GetDataManage()->GetSteelMaterial();
	
	long	nRow	= m_GridStd.GetFocusCell().row;
	long	nCol	= m_GridStd.GetFocusCell().col;
	
	if(nRow==-1 || nCol==-1)	return;
	
	long	nStdIdx		=	pMat->GetStandardSteelIdx(m_GridStd.GetTextMatrix(nRow, 0));
	CString	strCurName	=	m_GridStd.GetTextMatrix(nRow, 0);
	CString	strName		=	_T("");
	BOOL	bExist		=	FALSE;
	
	for(long nG=0; nG < pDB->GetGirdersu()-1; nG++)
	{
		CPlateGirderApp	*pGir	= pDB->GetGirder(nG);
		
		for(long n=0;n<pGir->GetQtyCrossBeamVBracing()-1; n++)
		{
			CBracing	*pBr	= pGir->GetBracingInTotal(n);

			if(pBr->GetType()==CBracing::BRACTYPE_NONE)	continue;
			
			strName = pMat->m_ptrArrStdSteel.GetAt(pBr->m_nStdSteelIdx)->m_szName;
			
			if(strName==strCurName)
			{
				bExist = TRUE;
				break;
			}
		}
	}
	
	if(bExist)
	{
		AfxMessageBox("해당 강재를 사용하고 있습니다. 강재를 변경후 삭제하여 주십시요.");
		return;
	}

	m_GridStd.DeleteRow(nRow);
	m_GridStd.SetRedraw(TRUE,TRUE);
	pMat->m_ptrArrStdSteel.RemoveAt(nRow-1);
		
	SetDataSaveStandardSteel();
	SetDataInitStdSteel();

}
