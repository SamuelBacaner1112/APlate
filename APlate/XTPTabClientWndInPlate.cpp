// XTPTabClientWnd.cpp : implementation of the CXTPTabClientWndInPlate class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ?004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTPTabClientWndInPlate.h"
#ifdef _XTP_ACTIVEX
#include "afxctl.h"
#endif
#include "APlateDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_TAB_TEXT		"입력"

#define IDC_TABWORKSPACE			1000

#define DB_LEFT        0x0002 
#define DB_RIGHT       0x0004
#define DB_CROSS       0x0008

#define DB_ENABLED     0x0000
#define DB_DISABLED    0x0100
#define DB_PRESSED     0x1000
#define DB_OVER        0x4000

#define DB_ISENABLED(Style)     (!((Style)&DB_DISABLED))
#define DB_ISPRESSED(Style)     (((Style)&(DB_PRESSED))!=0)
#define DB_ISOVER(Style)        (((Style)&DB_OVER)==DB_OVER)

CXTPTabWorkspaceInPlate::CTabArray* CXTPTabWorkspaceInPlate::CTabArray::pCompare;

CXTPTabWorkspaceInPlate::CPaintManagerInPlate::CPaintManagerInPlate()
{
	m_pWorkspace = NULL;
	RefreshMetrics();
}

void CXTPTabWorkspaceInPlate::CPaintManagerInPlate::RefreshMetrics()
{
	HWND hWnd = 0;
	if (m_pWorkspace)
	{
		CWnd* pWnd = m_pWorkspace->m_pTabClientWnd? m_pWorkspace->m_pTabClientWnd->GetParentFrame(): NULL;
		if (pWnd) hWnd = pWnd->GetSafeHwnd();
	}
	if (hWnd == 0) hWnd = AfxGetMainWnd()? AfxGetMainWnd()->GetSafeHwnd(): 0;

	m_themeTab.OpenTheme(hWnd, L"TAB");
	m_themeToolbar.OpenTheme(hWnd, L"TOOLBAR");

	m_clrBackgroundLight = XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd);
	m_clrBackgroundDark = GetXtremeColor(COLOR_3DFACE);
	m_clrBorder = m_clrBorderSelected = GetSysColor(COLOR_3DSHADOW);
	
	m_clrBorderLeftHighlight = m_clrBorderTopHighlight = GetSysColor(COLOR_3DFACE);
	m_clrHighlight = GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED);
/*
	if (!XTPColorManager()->IsLunaColorsDisabled())
	{	
	
		XTPCurrentSystemTheme systemTheme = XTPDrawHelpers()->GetCurrentSystemTheme();
		

		// 테마 고정
		systemTheme = xtpSystemThemeUnknown;
		
		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
			m_clrBackgroundLight = RGB(252, 253, 254);
			m_clrBackgroundDark = RGB(196, 218, 250);
			m_clrBorderSelected = RGB(59, 97, 156);
			m_clrBorder = RGB(0, 53, 154);	
			m_clrBorderLeftHighlight = RGB(117, 166, 241);
			m_clrBorderTopHighlight = RGB(255, 255, 255);
			m_clrHighlight = RGB(255, 240,	201);			
			break;
			
		case xtpSystemThemeOlive:
			m_clrBackgroundLight = RGB(252, 254, 254);
			m_clrBackgroundDark = RGB(242, 241, 228);
			m_clrBorderSelected = RGB(96, 128, 88);
			m_clrBorder = RGB(96, 119, 107);	
			m_clrBorderLeftHighlight = RGB(176, 194, 140);
			m_clrBorderTopHighlight = RGB(255, 255, 255);
			m_clrHighlight = RGB(255, 240,	199);
			break;
			
		case xtpSystemThemeSilver:
			m_clrBackgroundLight = RGB(252, 254, 254);
			m_clrBackgroundDark = RGB(243, 243, 247);
			m_clrBorderSelected = RGB(124, 124, 148);
			m_clrBorder = RGB(118, 116, 146);	
			m_clrBorderLeftHighlight = RGB(186, 185, 206);
			m_clrBorderTopHighlight = RGB(255, 255, 255);
			m_clrHighlight = RGB(255, 238,	194);
			break;				
		}	
	}
*/
}

//CXTPTabWorkspaceInPlate::CPaintManagerInPlate CXTPTabWorkspaceInPlate::CPaintManagerInPlate::m_managerInstance;

void CXTPTabWorkspaceInPlate::CPaintManagerInPlate::DrawTabControl(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw)
{
	if (!XTPPaintManager())
		return;

	switch (XTPPaintManager()->BaseTheme())
	{		
	case xtpThemeOfficeXP:		
		DrawTabControlOffice(pDC, lpClientRect, pTabCtrl, bDraw); break;
	case xtpThemeOffice2003: 
		DrawTabControlOffice2003(pDC, lpClientRect, pTabCtrl, bDraw); break;
	case xtpThemeNativeWinXP: 
	case xtpThemeOffice2007: 
		DrawTabControlNativeXP(pDC, lpClientRect, pTabCtrl, bDraw); break;
	default: 
		DrawTabControlDefault(pDC, lpClientRect, pTabCtrl, bDraw);
	}
}



COLORREF CXTPTabWorkspaceInPlate::CPaintManagerInPlate::DrawButton(CDC* pDC, CRect rc, COLORREF clrDark, BOOL bFirst, BOOL bFocused, COLORREF clrBorder, COLORREF clrLeftHighlight, COLORREF clrTopHighlight)
{
	const int nTop = rc.top, nBottom = rc.bottom, nLeft = rc.left, nRight = rc.right;

	COLORREF clrReturn = clrDark;
	if (bFocused)
	{
		clrLeftHighlight = clrTopHighlight = clrBorder = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
		clrDark = m_clrHighlight;
	}
	COLORREF clrLight = XTPColorManager()->LightColor(clrDark, RGB(255, 255, 255), 240);

	CRgn rgn;
	if (bFirst)
	{
		CPoint ptRgn[] = {CPoint(nRight + 1, nBottom + 1), CPoint (nRight, nTop + 1),
			CPoint(nLeft + 4, nTop + 1), CPoint(nLeft, nTop + 5), CPoint(nLeft, nBottom + 1) };
		rgn.CreatePolygonRgn(ptRgn, sizeof (ptRgn )/ sizeof (CPoint), ALTERNATE);
	} else
	{
		CPoint ptRgn[] = {
			CPoint(nRight + 1 , nBottom + 1),
			CPoint(nRight + 1, nBottom + 1), CPoint (nRight, nTop + 1),
			CPoint(nLeft + 4, nTop + 1), CPoint(nLeft, nTop + 5), 
			CPoint(nLeft - (nBottom - nTop) + 2 , nBottom + 1) };
		rgn.CreatePolygonRgn(ptRgn, sizeof (ptRgn )/ sizeof (CPoint), ALTERNATE);
	}

	CBrush br;
	br.CreateSolidBrush(RGB(200, 200, 200));

	pDC->SelectClipRgn(&rgn, RGN_AND);
	CRect rcBox;
	rgn.GetRgnBox(rcBox);

	

	XTPPaintManager()->GradientFill(pDC, rcBox, clrLight, clrDark, FALSE);

	pDC->SelectClipRgn(0);

	CXTPPenDC pen(*pDC, clrBorder);
	pDC->MoveTo(nRight, nBottom - 1); pDC->LineTo(nRight, nTop + 2); pDC->LineTo(nRight - 2, nTop);
	pDC->LineTo(nLeft + 7, nTop), pDC->LineTo(nLeft + 2, nTop + 2);
	if (bFirst)
	{
		pDC->LineTo(nLeft, nTop + 5); pDC->LineTo(nLeft, nBottom + 1);
	} 
	else
	{
		pDC->LineTo(nLeft - (nBottom - nTop) + 5, nBottom - 1);		
		pDC->LineTo(nLeft - (nBottom - nTop) + 1, nBottom);		
	}	

	pen.Color(clrLeftHighlight);
	pDC->MoveTo(nRight - 1, nBottom - 1); pDC->LineTo(nRight - 1, nTop + 1); pDC->SetPixel(nRight - 2, nTop + 1, clrLeftHighlight);

	pen.Color(clrTopHighlight);
	pDC->MoveTo(nRight - 3, nTop + 1); pDC->LineTo(nLeft + 6, nTop + 1);
	pDC->LineTo(nLeft + 1, nTop + 4); 
	if (bFirst)
	{	
		pDC->SetPixel(nLeft + 1, nTop + 5, clrTopHighlight);
	} else
	{
		pDC->LineTo(nLeft - (nBottom - nTop) + 5, nBottom); 
	}
	
	return clrReturn;
}

void CXTPTabWorkspaceInPlate::CPaintManagerInPlate::DrawTabControlNativeXP(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw)
{
	if (!m_themeTab.IsAppThemed() || (pTabCtrl->GetStyle() & TCS_BOTTOM))
	{
		DrawTabControlDefault(pDC, lpClientRect, pTabCtrl, bDraw);
		return;
	}
	if (!bDraw)
	{
		if (pTabCtrl->GetTabsArray().GetSize() == 0)
			return;
		CClientDC dc(pTabCtrl);
		CXTPFontDC fnt(&dc, XTPPaintManager()->GetIconFont());

		CSize szRect = dc.GetTextExtent(_T(" "));
		int nOffset = 5;
		int nHeight = max(24, szRect.cy + 9) - nOffset;
		
		lpClientRect->top += min (nHeight, lpClientRect->bottom - lpClientRect->top);
		
		if(lpClientRect->bottom - lpClientRect->top > 2 * nOffset)
		{
			lpClientRect->top += nOffset;
			lpClientRect->bottom -= nOffset;
		}
		else
		{
			lpClientRect->top=lpClientRect->bottom;
		}
		
		if(lpClientRect->right - lpClientRect->left > 2 * nOffset)
		{
			lpClientRect->left += nOffset - 2;
			lpClientRect->right -= nOffset;
		}
		else
		{
			lpClientRect->left=lpClientRect->right;
		}	
		return;
	}

	CRect rcClient(lpClientRect);
	CSize sz = rcClient.Size();
	pDC->FillSolidRect(rcClient, GetXtremeColor(COLOR_3DFACE));
	
	CTabArray& lstTabs = pTabCtrl->GetTabsArray();
	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);
	pDC->SetTextColor(clrText);
	pDC->SetBkMode(TRANSPARENT);
	CXTPFontDC fnt(pDC, XTPPaintManager()->GetIconFont());
	CSize szRect = pDC->GetTextExtent(_T(" "));	
	int nBottom = max(24, szRect.cy + 9) - 3; 
	//XTPPaintManager()->Line(pDC, 1, sz.cy - 3, sz.cx - 1, sz.cy - 3, COLOR_BTNHILIGHT);
	CRect rcPane(0, nBottom, sz.cx, rcClient.bottom);
	m_themeTab.DrawBackground(pDC->GetSafeHdc(), TABP_PANE, 0, &rcPane, 0);

	int nButtonWidth = 49;
	if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) != 0) nButtonWidth -= 14;
	if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) != 0) nButtonWidth -= 14 * 2;

	
	CRgn rgn;
	rgn.CreateRectRgn(2, 0, sz.cx - nButtonWidth, nBottom + 1);
	pDC->SelectClipRgn(&rgn);
	
	int x = pTabCtrl->GetOffset();
	BOOL bEnable = TRUE;
	
	CXTPEmptyRect rcActive;
	CString strActive;
	int i;	
	for (i = 0; i < lstTabs.GetSize(); i++)
	{
		CString strTitle = lstTabs[i].strText;
		
		strTitle = strTitle.GetLength() > 33? strTitle.Left(15) + _T("...") + strTitle.Right(15): strTitle;
		if (strTitle.IsEmpty()) strTitle = _T("   ");
		
		CXTPEmptyRect rcItem;
		pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_CALCRECT | DT_VCENTER);
		
		rcItem.OffsetRect(x, 0);
		rcItem.right += 12;
		
		rcItem.bottom = nBottom + 4;
		lstTabs[i].rcItem = rcItem;
		
		if (rcItem.right > rcClient.right - 49)
			bEnable = FALSE;
		
		x += rcItem.Width();	
	}

	int nFocused = pTabCtrl->GetItemFocused();

	for (i = 0; i < lstTabs.GetSize(); i++)
	{
		CString strTitle = lstTabs[i].strText;
		strTitle = strTitle.GetLength() > 33? strTitle.Left(15) + _T("...") + strTitle.Right(15): strTitle;
		CRect rcItem = lstTabs[i].rcItem;

		if (CRect().IntersectRect(rcClient, rcItem))
		{
			BOOL bActive = pTabCtrl->GetItemTracking() == -1 ? pTabCtrl->GetCurSel() == i : (pTabCtrl->GetItemTracking() == i);

			if (!bActive)
			{
				CRect rcItemBg(rcItem.left, rcItem.top + 3, rcItem.right, rcItem.bottom - 4);
				m_themeTab.DrawBackground(pDC->GetSafeHdc(), TABP_TABITEM, nFocused == i? TIS_FOCUSED: TIS_NORMAL, &rcItemBg, 0);
				pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			} else
			{
				rcActive.SetRect(rcItem.left -2, rcItem.top + 1, rcItem.right + 1, rcItem.bottom - 2);
				strActive = strTitle;
			}
		}
	}

	if (!rcActive.IsRectEmpty())
	{
		m_themeTab.DrawBackground(pDC->GetSafeHdc(), TABP_TABITEM, TIS_SELECTED, &rcActive, 0);
		pDC->DrawText(strActive, rcActive,   DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
	
	
	pDC->SelectClipRgn(NULL);
	rgn.DeleteObject();
	
	
	int nTop = (nBottom + 2 - 15) / 2;
	CRect rcButton(rcClient.right - 19, nTop, rcClient.right - 5, nTop + 15);
	if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) == 0)
	{				
		
		pTabCtrl->m_btnClose.SetRect(rcButton);		
		m_themeToolbar.DrawBackground(pDC->GetSafeHdc(),  TP_BUTTON, 		
			pTabCtrl->m_btnClose.GetStyle() & DB_PRESSED? TS_PRESSED:
		pTabCtrl->m_btnClose.GetStyle() & DB_OVER? TS_HOT: TS_NORMAL, &rcButton, 0);
		pTabCtrl->m_btnClose.Draw(pDC, clrText, FALSE);
		rcButton.OffsetRect(-14, 0);
	} else pTabCtrl->m_btnClose.SetRect(CXTPEmptyRect());
		
	if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) == 0)
	{			
		pTabCtrl->m_btnRight.SetRect(rcButton);
		pTabCtrl->m_btnRight.SetEnable(bEnable);
		m_themeToolbar.DrawBackground(pDC->GetSafeHdc(),  TP_BUTTON, 		
			pTabCtrl->m_btnRight.GetStyle() & DB_PRESSED? TS_PRESSED:
		pTabCtrl->m_btnRight.GetStyle() & DB_OVER? TS_HOT: TS_NORMAL, &rcButton, 0);
		pTabCtrl->m_btnRight.Draw(pDC, clrText, FALSE);
		
		rcButton.OffsetRect(-14, 0);
		pTabCtrl->m_btnLeft.SetRect(rcButton);
		pTabCtrl->m_btnLeft.SetEnable(pTabCtrl->GetOffset() == 6);
		m_themeToolbar.DrawBackground(pDC->GetSafeHdc(),  TP_BUTTON, 		
			pTabCtrl->m_btnLeft.GetStyle() & DB_PRESSED? TS_PRESSED:
		pTabCtrl->m_btnLeft.GetStyle() & DB_OVER? TS_HOT: TS_NORMAL, &rcButton, 0);
		pTabCtrl->m_btnLeft.Draw(pDC, clrText, FALSE);
	}
	else
	{
		pTabCtrl->m_btnRight.SetRect(CXTPEmptyRect());
		pTabCtrl->m_btnLeft.SetRect(CXTPEmptyRect());
	}
}

void CXTPTabWorkspaceInPlate::CPaintManagerInPlate::DrawTabControlOffice2003(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw)
{
	if (pTabCtrl->GetStyle() & TCS_BOTTOM)
	{
		DrawTabControlOffice(pDC, lpClientRect, pTabCtrl, bDraw);
		return;
	}

	if (!bDraw)
	{
		if (pTabCtrl->GetTabsArray().GetSize() == 0)
			return;
		CClientDC dc(pTabCtrl);
		CXTPFontDC fnt(&dc, XTPPaintManager()->GetIconFont());

		CSize szRect = dc.GetTextExtent(_T(" "));
		int nOffset = 2;
		int nHeight = max(29, szRect.cy + 9);
		
		lpClientRect->top += min (nHeight, lpClientRect->bottom - lpClientRect->top);
		
		if(lpClientRect->bottom - lpClientRect->top > 2 * nOffset)
		{
			lpClientRect->top += nOffset;
			lpClientRect->bottom -= nOffset;
		}
		else
		{
			lpClientRect->top=lpClientRect->bottom;
		}
		
		if(lpClientRect->right - lpClientRect->left > 2 * nOffset)
		{
			lpClientRect->left += nOffset;
			lpClientRect->right -= nOffset;
		}
		else
		{
			lpClientRect->left=lpClientRect->right;
		}	
		
	} else
	{
		CRect rcClient(lpClientRect);
		COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);		
		CXTPFontDC fnt(pDC, XTPPaintManager()->GetIconFont());

		CSize szRect = pDC->GetTextExtent(_T(" "));
		int nBottom = max(29, szRect.cy + 9) - 1; 

		CSize sz = rcClient.Size();
		XTPPaintManager()->GradientFill(pDC, CRect(rcClient.left, rcClient.top, rcClient.right, nBottom), m_clrBackgroundDark, m_clrBackgroundLight, FALSE);

		CTabArray& lstTabs = pTabCtrl->GetTabsArray();

		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);

		CXTPPenDC pen(*pDC, GetXtremeColor(COLOR_BTNHILIGHT));
		
		int x = pTabCtrl->GetOffset();
		BOOL bEnable = TRUE;
		
		//int nBottom = sz.cy - 1;
		int nTop = 9;

		int nButtonWidth = 49;
		if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) != 0) nButtonWidth -= 14;
		if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) != 0) nButtonWidth -= 14 * 2;


		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

		CXTPEmptyRect rcActive; int nActive = 0;
		CString strActive;
		int i;
		for (i = 0; i < lstTabs.GetSize(); i++)
		{
			
			CString strTitle = lstTabs[i].strText;
			strTitle = strTitle.GetLength() > 33? strTitle.Left(15) + _T("...") + strTitle.Right(15): strTitle;
			if (strTitle.IsEmpty()) strTitle = _T("   ");
			
			BOOL bActive = pTabCtrl->GetItemTracking() == -1 ? pTabCtrl->GetCurSel() == i : (pTabCtrl->GetItemTracking() == i);

			CXTPEmptyRect rcItem;
			pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_CALCRECT | DT_VCENTER);
			
			rcItem.OffsetRect(x, 0);
			rcItem.right += 36;
			
			rcItem.top = nTop;
			rcItem.bottom = nBottom;
			lstTabs[i].rcItem = rcItem;
			if (rcItem.right > rcClient.right - nButtonWidth)
				bEnable = FALSE;

			if (bActive)
			{
				nActive = i;
				rcActive = rcItem;
				strActive = strTitle;
			}
			x += rcItem.Width();	
		}
		
		int nFocused = pTabCtrl->GetItemFocused();

		for (i = lstTabs.GetSize() - 1; i >= 0; i--)
		{		
			CRect rcItem = lstTabs[i].rcItem;
			CString strTitle = lstTabs[i].strText;
			strTitle = strTitle.GetLength() > 33? strTitle.Left(15) + _T("...") + strTitle.Right(15): strTitle;

			if (CRect().IntersectRect(rcClient, rcItem))
			{
				
				DrawButton(pDC, rcItem, pTabCtrl->GetColor(i), i == 0, i == nFocused, m_clrBorder, m_clrBorderLeftHighlight, m_clrBorderTopHighlight);
				pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}			
		}

		pen.Color(m_clrBorderSelected);
		XTPPaintManager()->Line(pDC, CPoint(0, nBottom), CPoint(sz.cx - 1, nBottom));
		
		COLORREF clrClient = GetXtremeColor(COLOR_3DFACE); 
		if (!rcActive.IsRectEmpty())
		{
			fnt.SetFont(XTPPaintManager()->GetIconBoldFont()); 
			clrClient = DrawButton(pDC, rcActive, pTabCtrl->GetColor(nActive), nActive == 0, nActive == nFocused, m_clrBorderSelected, RGB(255, 255, 255), RGB(255, 255, 255));
			pDC->DrawText(strActive, rcActive,   DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}


		CRect rcButtons(rcClient);
		rcButtons.left = rcButtons.right - nButtonWidth;

		XTPPaintManager()->GradientFill(pDC, CRect(rcClient.right - nButtonWidth, rcClient.top, rcClient.right, nBottom), m_clrBackgroundDark, m_clrBackgroundLight, FALSE);
		XTPPaintManager()->Line(pDC, CPoint(sz.cx - nButtonWidth, nBottom), CPoint(sz.cx, nBottom));
		
		pDC->FillSolidRect(0, nBottom + 1, sz.cx, rcClient.Height() - nBottom, clrClient);
		
		int nTopButton = (nBottom + 4- 15) / 2;
		CRect rcButton(rcClient.right - 19, nTopButton, rcClient.right - 5, nTopButton + 15);
		
		if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) == 0)
		{
			
			pTabCtrl->m_btnClose.SetRect(rcButton);
			//XTPPaintManager()->DrawRectangle(pDC,  rcButton, pTabCtrl->m_btnClose.GetStyle() & DB_OVER, 
				//pTabCtrl->m_btnClose.GetStyle() & DB_PRESSED);
			
			pTabCtrl->m_btnClose.Draw(pDC, clrText, FALSE);
			rcButton.OffsetRect(-14, 0);
		} else pTabCtrl->m_btnClose.SetRect(CXTPEmptyRect());

		if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) == 0)
		{
			pTabCtrl->m_btnRight.SetRect(rcButton);
			pTabCtrl->m_btnRight.SetEnable(bEnable);
			
			//XTPPaintManager()->DrawRectangle(pDC,  rcButton, pTabCtrl->m_btnRight.GetStyle() & DB_OVER, 
				//pTabCtrl->m_btnRight.GetStyle() & DB_PRESSED);			
			pTabCtrl->m_btnRight.Draw(pDC, clrText, FALSE);
			
			rcButton.OffsetRect(-14, 0);				
			pTabCtrl->m_btnLeft.SetRect(rcButton);
			pTabCtrl->m_btnLeft.SetEnable(pTabCtrl->GetOffset() == 6);
			
			//XTPPaintManager()->DrawRectangle(pDC,  rcButton, pTabCtrl->m_btnLeft.GetStyle() & DB_OVER, 
				//pTabCtrl->m_btnLeft.GetStyle() & DB_PRESSED);
			pTabCtrl->m_btnLeft.Draw(pDC, clrText, FALSE);
		} else
		{
			pTabCtrl->m_btnRight.SetRect(CXTPEmptyRect());
			pTabCtrl->m_btnLeft.SetRect(CXTPEmptyRect());
		}
		
}
	}



void CXTPTabWorkspaceInPlate::CPaintManagerInPlate::DrawTabControlDefault(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw)
{
	if (!bDraw)
	{
		DrawTabControlOffice(pDC, lpClientRect, pTabCtrl, bDraw);
 	} 
	else
	{
		CRect rcClient(lpClientRect);
		CSize sz = rcClient.Size();
		pDC->FillSolidRect(rcClient, GetXtremeColor(COLOR_3DFACE));
		CTabArray& lstTabs = pTabCtrl->GetTabsArray();
		COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);
		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);
		CXTPFontDC fnt(pDC, XTPPaintManager()->GetIconFont());
		CSize szRect = pDC->GetTextExtent(_T(" "));

		if (!(pTabCtrl->GetStyle() & TCS_BOTTOM))
		{

			int nBottom = max(24, szRect.cy + 9) - 3 + 3; 

			//XTPPaintManager()->Line(pDC, 1, sz.cy - 3, sz.cx - 1, sz.cy - 3, COLOR_BTNHILIGHT);
			XTPPaintManager()->Draw3dRect(pDC, CRect(0, nBottom -2, sz.cx, rcClient.bottom), COLOR_BTNHILIGHT, COLOR_BTNTEXT);
			XTPPaintManager()->Draw3dRect(pDC, CRect(1, nBottom -1, sz.cx - 1, rcClient.bottom - 1), COLOR_3DFACE, COLOR_3DSHADOW);
		

			int nButtonWidth = 49;
			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) != 0) nButtonWidth -= 14;
			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) != 0) nButtonWidth -= 14 * 2;			
			
			CRgn rgn;
			rgn.CreateRectRgn(2, 0, sz.cx - nButtonWidth, nBottom - 1);
			pDC->SelectClipRgn(&rgn);
			
			int x = pTabCtrl->GetOffset();
			BOOL bEnable = TRUE;

			for (int i = 0; i < lstTabs.GetSize(); i++)
			{

				CString strTitle = lstTabs[i].strText;

				strTitle = strTitle.GetLength() > 33? strTitle.Left(15) + _T("...") + strTitle.Right(15): strTitle;
				if (strTitle.IsEmpty()) strTitle = _T("   ");

				BOOL bActive = pTabCtrl->GetItemTracking() == -1 ? pTabCtrl->GetCurSel() == i: (pTabCtrl->GetItemTracking() == i);

				CXTPEmptyRect rcItem;
				pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_CALCRECT | DT_VCENTER);

				rcItem.OffsetRect(x, 0);
				rcItem.right += 12;

				rcItem.bottom = nBottom + 1;
				lstTabs[i].rcItem = rcItem;

				if (CRect().IntersectRect(rcClient, rcItem))
				{
					int nTop = bActive? 1: 3;
					XTPPaintManager()->Line(pDC, rcItem.left, nBottom - 2, rcItem.left, nTop + 1, COLOR_BTNHILIGHT);
					XTPPaintManager()->Pixel(pDC, rcItem.left + 1 , nTop + 1, COLOR_BTNHILIGHT);
					XTPPaintManager()->Line(pDC, rcItem.left + 2, nTop, rcItem.right - 3,  nTop, COLOR_BTNHILIGHT);
					XTPPaintManager()->Pixel(pDC, rcItem.right - 3 , nTop, COLOR_BTNTEXT);
					XTPPaintManager()->Pixel(pDC, rcItem.right - 2 , nTop + 1, COLOR_BTNTEXT);

					int nDown = bActive? nBottom - 1: nBottom - 2;

					XTPPaintManager()->Line(pDC, rcItem.right - 1, nTop + 2, rcItem.right - 1, nDown, COLOR_BTNTEXT);
					XTPPaintManager()->Line(pDC, rcItem.right - 2, nTop + 2, rcItem.right - 2, nDown, COLOR_3DSHADOW);
					if (bActive)
					{					
						XTPPaintManager()->Line(pDC, rcItem.left + 1, nDown-1 , rcItem.right - 2, nDown -1, COLOR_3DFACE);
						rcItem.bottom--;
					}
					
					pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				}

				if (rcItem.right > rcClient.right - nButtonWidth)
					bEnable = FALSE;

				x += rcItem.Width();	

			}


			pDC->SelectClipRgn(NULL);
			rgn.DeleteObject();

	
			int nTop = (nBottom + 2 - 15) / 2;
			CRect rcButton(rcClient.right - 19, nTop, rcClient.right - 5, nTop + 15);
			
			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) == 0)
			{
				pTabCtrl->m_btnClose.SetRect(CRect(rcClient.right - 19, nTop, rcClient.right - 5, nTop + 15));
				pTabCtrl->m_btnClose.Draw(pDC, clrText, TRUE, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_BTNHIGHLIGHT));
				rcButton.OffsetRect(-14, 0);
			} else pTabCtrl->m_btnClose.SetRect(CXTPEmptyRect());

			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) == 0)
			{				
				pTabCtrl->m_btnRight.SetRect(rcButton);
				pTabCtrl->m_btnRight.SetEnable(bEnable);
				pTabCtrl->m_btnRight.Draw(pDC, clrText, TRUE, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_BTNHIGHLIGHT));
				
				rcButton.OffsetRect(-14, 0);				
				pTabCtrl->m_btnLeft.SetRect(rcButton);
				pTabCtrl->m_btnLeft.SetEnable(pTabCtrl->GetOffset() == 6);
				pTabCtrl->m_btnLeft.Draw(pDC, clrText, TRUE, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_BTNHIGHLIGHT));		
			} else
			{
				pTabCtrl->m_btnRight.SetRect(CXTPEmptyRect());
				pTabCtrl->m_btnLeft.SetRect(CXTPEmptyRect());
			}

		} else
		{

			int nBottom = rcClient.bottom; 
			int nTop = nBottom - (max(24, szRect.cy + 9) - 3 + 1); 

			XTPPaintManager()->Draw3dRect(pDC, CRect(0, 0, sz.cx, nTop), COLOR_BTNHILIGHT, COLOR_BTNTEXT);
			XTPPaintManager()->Draw3dRect(pDC, CRect(1, 1, sz.cx - 1, nTop - 1), COLOR_3DFACE, COLOR_3DSHADOW);

			int i, nMaxWidth  = ::GetSystemMetrics(SM_CXSCREEN) / 3;

			for (i = 0; i < lstTabs.GetSize(); i++)
			{
				CString strTitle = lstTabs[i].strText;
				CXTPEmptyRect rcItem;

				if (!strTitle.IsEmpty())
					pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_CALCRECT | DT_VCENTER);
				else rcItem.right = 10;

				rcItem.right += 28;
				
				if (rcItem.Width() > nMaxWidth)
					rcItem.right = nMaxWidth;
				
				rcItem.bottom = nBottom;
				rcItem.top = nTop;
				lstTabs[i].rcItem = rcItem;
			}

			pTabCtrl->AdjustTabs(sz.cx);

	
			for (i = 0; i < lstTabs.GetSize(); i++)
			{
				CRect rcItem (lstTabs[i].rcItem);
				CString strTitle = lstTabs[i].strText;

				BOOL bActive = pTabCtrl->GetItemTracking() == -1 ? pTabCtrl->GetCurSel() ==i: (pTabCtrl->GetItemTracking() == i);

				if (bActive) pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

				if (CRect().IntersectRect(rcClient, rcItem))
				{
					int nHeight = nBottom - 2;
					int nBtnTop = nTop - 2;
					
					XTPPaintManager()->Line(pDC, rcItem.left, nBtnTop + 1, rcItem.left, nHeight, COLOR_BTNHILIGHT);
					XTPPaintManager()->Line(pDC, rcItem.left + 1, nHeight, rcItem.right - 2, nHeight, COLOR_3DSHADOW);
					XTPPaintManager()->Line(pDC, rcItem.right - 2, nBtnTop+ 1, rcItem.right - 2, nHeight, COLOR_3DSHADOW);
					XTPPaintManager()->Line(pDC, rcItem.right - 1, nBtnTop + 2, rcItem.right - 1, nHeight, COLOR_BTNTEXT);
					XTPPaintManager()->Line(pDC, rcItem.left + 2, nHeight + 1, rcItem.right - 2, nHeight + 1, COLOR_BTNTEXT);					
					XTPPaintManager()->Pixel(pDC, rcItem.left + 1, nHeight - 1, COLOR_3DSHADOW);
					XTPPaintManager()->Pixel(pDC, rcItem.right - 3, nHeight - 1, COLOR_3DSHADOW);
					XTPPaintManager()->Pixel(pDC, rcItem.right - 2, nHeight - 0, COLOR_BTNTEXT);

					if (!bActive)
					{
						XTPPaintManager()->Line(pDC, rcItem.left , nBtnTop + 0, rcItem.right - 2, nBtnTop + 0, COLOR_3DSHADOW);
						XTPPaintManager()->Line(pDC, rcItem.left , nBtnTop + 1, rcItem.right - 1, nBtnTop + 1, COLOR_BTNTEXT);

					} 
					else
					{
						pDC->FillSolidRect(CRect(rcItem.left + 1, nTop - 2, rcItem.right - 2, nTop), GetXtremeColor(COLOR_3DFACE));
					}

					if  (rcItem.Width() >= 20)
					{
						HICON hIcon = pTabCtrl->GetIcon(i);
						if (hIcon)
						{
							CPoint pt(rcItem.left + 3, rcItem.top + (rcItem.Height() - 18) / 2 );
							pDC->DrawState(pt, CSize(16, 16), hIcon, (UINT)DSS_NORMAL, HBRUSH(0));
							//DestroyIcon(hIcon);
						}

						pDC->DrawText(strTitle, CRect(rcItem.left + 16 + 6, rcItem.top, rcItem.right - 6, rcItem.bottom),  
							DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
					}
				}

				
			}
		}

	}
}


void CXTPTabWorkspaceInPlate::CPaintManagerInPlate::DrawTabControlOffice(CDC* pDC, LPRECT lpClientRect, CXTPTabWorkspaceInPlate* pTabCtrl, BOOL bDraw)
{
	if (!bDraw)
	{
		if (pTabCtrl->GetTabsArray().GetSize() == 0)
			return;
		CClientDC dc(pTabCtrl);
		CXTPFontDC fnt(&dc, XTPPaintManager()->GetIconFont());

		CSize szRect = dc.GetTextExtent(_T(" "));
		int nOffset = 2;
		int nHeight = max(24, szRect.cy + 9) - nOffset;
		
		if (pTabCtrl->GetStyle() & TCS_BOTTOM)
			lpClientRect->bottom -= min (nHeight, lpClientRect->bottom - lpClientRect->top);
		else lpClientRect->top += min (nHeight, lpClientRect->bottom - lpClientRect->top);
		
		if(lpClientRect->bottom - lpClientRect->top > 2 * nOffset)
		{
			lpClientRect->top += nOffset;
			lpClientRect->bottom -= nOffset;
		}
		else
		{
			lpClientRect->top=lpClientRect->bottom;
		}
		
		if(lpClientRect->right - lpClientRect->left > 2 * nOffset)
		{
			lpClientRect->left += nOffset;
			lpClientRect->right -= nOffset;
		}
		else
		{
			lpClientRect->left=lpClientRect->right;
		}	

	} 
	else
	{
		COLORREF clr3DFace = GetSysColor(COLOR_3DFACE);
		CRect rcClient(lpClientRect);
		pDC->FillSolidRect(rcClient, clr3DFace);
		pDC->Draw3dRect(rcClient, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));

		COLORREF clrBackground = GetXtremeColor(XPCOLOR_TAB_INACTIVE_BACK)
			, clrText = GetXtremeColor(XPCOLOR_TAB_INACTIVE_TEXT);
		//StyleColorManager()->GetTabControlColors(clrBackground, clrText);
		CTabArray& lstTabs = pTabCtrl->GetTabsArray();
		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);
		CXTPFontDC fnt(pDC, XTPPaintManager()->GetIconFont());
		CSize szRect = pDC->GetTextExtent(_T(" "));
		
		if (!(pTabCtrl->GetStyle() & TCS_BOTTOM))
		{
			int nBottom = max(24, szRect.cy + 9) - 3 + 1;
			CSize sz = rcClient.Size();
			pDC->FillSolidRect(rcClient.left + 2, rcClient.top + 1, rcClient.Width() - 4, nBottom, clrBackground);

			CXTPPenDC pen(*pDC, GetXtremeColor(COLOR_BTNHILIGHT));
			XTPPaintManager()->Line(pDC, CPoint(2, nBottom - 1), CPoint(sz.cx - 2, nBottom - 1));

			int nButtonWidth = 49;
			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) != 0) nButtonWidth -= 14;
			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) != 0) nButtonWidth -= 14 * 2;

			CRgn rgn;
			rgn.CreateRectRgn(2, 0, sz.cx - nButtonWidth, nBottom);
			pDC->SelectClipRgn(&rgn);
			
			int x = pTabCtrl->GetOffset();
			BOOL bEnable = TRUE;

			for (int i = 0; i < lstTabs.GetSize(); i++)
			{

				CString strTitle = lstTabs[i].strText;

				strTitle = strTitle.GetLength() > 33? strTitle.Left(15) + _T("...") + strTitle.Right(15): strTitle;
				if (strTitle.IsEmpty()) strTitle = _T("   ");

				BOOL bActive = pTabCtrl->GetItemTracking() == -1 ? pTabCtrl->GetCurSel() == i: (pTabCtrl->GetItemTracking() == i);

				if (bActive)
				{
					fnt.SetFont(XTPPaintManager()->GetIconBoldFont()); 
					pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
				} 

				CXTPEmptyRect rcItem;
				pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_CALCRECT | DT_VCENTER);

				rcItem.OffsetRect(x, 0);
				rcItem.right += 12;

				rcItem.bottom = nBottom + 4;
				lstTabs[i].rcItem = rcItem;

				if (CRect().IntersectRect(rcClient, rcItem))
				{
					if (!bActive)
					{
						pen.Color(GetXtremeColor(XPCOLOR_3DSHADOW));
						XTPPaintManager()->Line(pDC, CPoint(rcItem.right, 5), CPoint(rcItem.right, nBottom - 1));
					} else
					{
						pDC->FillSolidRect(CRect(rcItem.left, 4, rcItem.right, nBottom), GetXtremeColor(XPCOLOR_3DFACE));
						XTPPaintManager()->Draw3dRect(pDC, CRect(rcItem.left, 3, rcItem.right, nBottom + 1), COLOR_BTNHILIGHT, COLOR_BTNTEXT);
					}

					pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				}

				if (rcItem.right > rcClient.right - nButtonWidth)
					bEnable = FALSE;

				x += rcItem.Width();	

				if (bActive)
				{
					fnt.SetFont(XTPPaintManager()->GetIconFont());
					pDC->SetTextColor(clrText);
				}
			}


			pDC->SelectClipRgn(NULL);
			rgn.DeleteObject();

			pDC->FillSolidRect(CRect(1, nBottom, sz.cx - 1, nBottom + 1) , GetXtremeColor(XPCOLOR_3DFACE));
	
			int nTop = (nBottom + 2 - 15) / 2;
			CRect rcButton(rcClient.right - 19, nTop, rcClient.right - 5, nTop + 15);

			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideClose) == 0)
			{				
				pTabCtrl->m_btnClose.SetRect(rcButton);
				pTabCtrl->m_btnClose.Draw(pDC, clrText);
				rcButton.OffsetRect(-14, 0);
			} else pTabCtrl->m_btnClose.SetRect(CXTPEmptyRect());

			if ((pTabCtrl->m_dwFlags & xtpWorkspaceHideArrows) == 0)
			{			
				pTabCtrl->m_btnRight.SetRect(rcButton);
				pTabCtrl->m_btnRight.SetEnable(bEnable);
				pTabCtrl->m_btnRight.Draw(pDC, clrText);

				rcButton.OffsetRect(-14, 0);				
				pTabCtrl->m_btnLeft.SetRect(rcButton);
				pTabCtrl->m_btnLeft.SetEnable(pTabCtrl->GetOffset() == 6);
				pTabCtrl->m_btnLeft.Draw(pDC, clrText);
			} else
			{
				pTabCtrl->m_btnRight.SetRect(CXTPEmptyRect());
				pTabCtrl->m_btnLeft.SetRect(CXTPEmptyRect());
			}


		} else
		{
			int nBottom = rcClient.bottom - 2;  
			int nTop = nBottom - (max(24, szRect.cy + 9) - 3); 

			CSize sz = rcClient.Size();
			pDC->FillSolidRect(rcClient.left + 2, nTop + 1, rcClient.Width() - 4, nBottom - nTop, clrBackground);

			XTPPaintManager()->Line(pDC, 2, nTop, sz.cx - 2, nTop, COLOR_BTNTEXT);
			int i, nMaxWidth  = ::GetSystemMetrics(SM_CXSCREEN) / 3;

			for (i = 0; i < lstTabs.GetSize(); i++)
			{

				CString strTitle = lstTabs[i].strText;
				CXTPEmptyRect rcItem;

				if (!strTitle.IsEmpty())
					pDC->DrawText(strTitle, rcItem,   DT_SINGLELINE | DT_CALCRECT | DT_VCENTER);
				else rcItem.right = 10;

				rcItem.right += 28;
				
				if (rcItem.Width() > nMaxWidth)
					rcItem.right = nMaxWidth;
				
				rcItem.bottom = nBottom;
				rcItem.top = nTop - 1;
				lstTabs[i].rcItem = rcItem;
				
			}

			pTabCtrl->AdjustTabs(sz.cx);
	
			for (i = 0; i < lstTabs.GetSize(); i++)
			{
				CRect rcItem (lstTabs[i].rcItem);
				CString strTitle = lstTabs[i].strText;

				BOOL bActive = pTabCtrl->GetItemTracking() == -1 ? pTabCtrl->GetCurSel() == i : (pTabCtrl->GetItemTracking() == i);

				if (bActive) pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

				if (CRect().IntersectRect(rcClient, rcItem))
				{
					if (!bActive)
					{
						XTPPaintManager()->Line(pDC, rcItem.right, nTop + 3, rcItem.right, nBottom - 2, COLOR_3DSHADOW);
					} 
					else
					{
						pDC->FillSolidRect(rcItem, GetXtremeColor(XPCOLOR_3DFACE));
						XTPPaintManager()->Draw3dRect(pDC, rcItem, COLOR_BTNHILIGHT, COLOR_BTNTEXT);
					}

					if  (rcItem.Width() >= 20)
					{
						HICON hIcon = pTabCtrl->GetIcon(i);
						if (hIcon)
						{
							CPoint pt(rcItem.left + 3, rcItem.top + (rcItem.Height() - 20) / 2  + 2);
							pDC->DrawState(pt, CSize(16, 16), hIcon, (UINT)DSS_NORMAL, HBRUSH(0));
							//DestroyIcon(hIcon);
						}

						pDC->DrawText(strTitle, CRect(rcItem.left + 16 + 6, rcItem.top, rcItem.right - 6, rcItem.bottom),  
							DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
					}
				}

				if (bActive) pDC->SetTextColor(clrText);
			}
			pDC->FillSolidRect(CRect(2, nTop - 1, sz.cx - 4, nTop), GetXtremeColor(XPCOLOR_3DFACE));
		}
	}
}


void CXTPTabWorkspaceInPlate::CDrawButton::SetEnable(BOOL bEnable)		
{
	if (bEnable) m_wStyle |= DB_DISABLED; else m_wStyle &= ~DB_DISABLED;
}

void CXTPTabWorkspaceInPlate::CDrawButton::Draw (CDC* pDC, COLORREF clrButton, BOOL bFrame, COLORREF clrFrameTopLeft, COLORREF clrFrameBottomRight)
{
	ASSERT (pDC != NULL);
    CRect rc (m_Rect);
	DWORD wStyle = m_wStyle;
	
	if (bFrame && DB_ISENABLED(wStyle))
	{
		if (DB_ISPRESSED(wStyle))
			pDC->Draw3dRect(rc, clrFrameTopLeft, clrFrameBottomRight);		
		else if (DB_ISOVER(wStyle))
			pDC->Draw3dRect(rc, clrFrameBottomRight, clrFrameTopLeft);		
	}
	if (DB_ISPRESSED(wStyle)) 
	{
		rc.OffsetRect(1, 1);
	}
	
	CXTPPenDC pen(*pDC, clrButton);
	

	CBrush brush;
	CBrush* pOldBrush;
    
	if (DB_ISENABLED(wStyle))
	{
		brush.CreateSolidBrush(clrButton);
		pOldBrush = pDC->SelectObject(&brush);    
	} else
	{
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	}

	
    if (wStyle & DB_CROSS)
	{
		pDC->MoveTo(rc.left + 3, rc.top + 3);
		pDC->LineTo(rc.right - 4, rc.bottom - 5);
		pDC->MoveTo(rc.left + 4, rc.top + 3);
		pDC->LineTo(rc.right - 3, rc.bottom - 5);
		
		pDC->MoveTo(rc.left + 3, rc.bottom  - 6);
		pDC->LineTo(rc.right - 4, rc.top + 2);
		pDC->MoveTo(rc.left + 4, rc.bottom  - 6);
		pDC->LineTo(rc.right - 3, rc.top + 2);
	}		
	else if (wStyle & DB_LEFT)
	{
        CPoint pts[] = 
        {
            CPoint(rc.right - 5, rc.top + 2), 
		    CPoint(rc.right - 9, rc.top + 6),
		    CPoint(rc.right - 5, rc.top + 10)
        };
        pDC->Polygon( pts, 3 );
	}
	else if (wStyle & DB_RIGHT)
	{
        CPoint pts[] = 
        {
            CPoint(rc.left + 4, rc.top + 2), 
		    CPoint(rc.left + 8, rc.top + 6),
		    CPoint(rc.left + 4, rc.top + 10)
        };
        pDC->Polygon( pts, 3 );
	}

    pDC->SelectObject(pOldBrush);

}



/////////////////////////////////////////////////////////////////////////////
DWORD CXTPTabWorkspaceInPlate::CDrawButton::Click (CXTPTabWorkspaceInPlate* pWnd, CPoint pt, UINT nIDRepeat)
{
    if ( (::GetCapture() != NULL ) || !DB_ISENABLED(m_wStyle))
		return FALSE;

    pWnd->SetCapture();
    ASSERT(pWnd == CWnd::GetCapture());

    BOOL  bDown = FALSE, bClick = FALSE;
    CRect rcBtn(m_Rect);

	DWORD dwStart = GetTickCount();
    while ( TRUE )
    {
		if (GetTickCount() - dwStart > 20 && nIDRepeat && DB_ISENABLED(m_wStyle))
		{
			dwStart = GetTickCount();
			
			if (m_wStyle & DB_LEFT)
			{
				if (pWnd->m_nOffset < 6) pWnd->m_nOffset += 40;
				if (pWnd->m_nOffset > 6) pWnd->m_nOffset = 6;
			} else
			{
				pWnd->m_nOffset -= 40;
			}
			pWnd->Invalidate(FALSE);
		}
		
        BOOL bNewDown = ::PtInRect (rcBtn, pt) != 0;
        if ( bNewDown != bDown )
        {
            bDown = bNewDown;
			if (bDown) m_wStyle = m_wStyle | DB_PRESSED; else m_wStyle &= ~DB_PRESSED;

			pWnd->InvalidateRect (m_Rect, FALSE);
        }
        MSG msg;
        VERIFY(::GetMessage(&msg, NULL, 0, 0));

        if ( CWnd::GetCapture() != pWnd )
        {
            DispatchMessage (&msg);
            goto ExitLoop;
        }
        switch ( msg.message )
        {
            case WM_MOUSEMOVE:
                pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
                break;

            case WM_LBUTTONUP:
                bClick = bDown;
                goto ExitLoop;

            case WM_KEYDOWN:
                if ( msg.wParam != VK_ESCAPE )
                    break;
            case WM_CANCELMODE:
            case WM_RBUTTONDOWN:
                goto ExitLoop;

			default:
                DispatchMessage (&msg);
                break;
        }
    }
ExitLoop:
    ReleaseCapture();

	m_wStyle &= ~DB_PRESSED;
	
	CheckForMouseOver (pWnd, pt);
	pWnd->InvalidateRect (m_Rect, FALSE);

    return bClick;
}

/////////////////////////////////////////////////////////////////////////////
void CXTPTabWorkspaceInPlate::CDrawButton::CheckForMouseOver (CWnd* pWnd, CPoint pt)
{

	if (!PtInRect (pt) && DB_ISOVER(m_wStyle))
	{
		m_wStyle &= ~DB_OVER;
		pWnd->InvalidateRect (m_Rect, FALSE);
	} 

	if (!DB_ISOVER(m_wStyle) && PtInRect (pt) && DB_ISENABLED(m_wStyle) )
	{
		XTPMouseManager()->TrackMouseLeave(*pWnd);
		m_wStyle |= DB_OVER;

		pWnd->InvalidateRect (m_Rect, FALSE);
	}
}
////////////////////////////

CXTPTabWorkspaceInPlate::CXTPTabWorkspaceInPlate()
	: m_btnClose(DB_CROSS), m_btnLeft(DB_LEFT), m_btnRight(DB_RIGHT)
{
	m_pTabClientWnd=NULL;
	m_nOffset = 6;

	m_nSelected = m_nItemTracking = m_nItemFocused = -1;

	m_paintManager.m_pWorkspace = this;
	
	m_dwFlags = 0;
}


CXTPTabWorkspaceInPlate::~CXTPTabWorkspaceInPlate()
{
}

int CXTPTabWorkspaceInPlate::HitTest(CPoint point)
{
	for (int i = 0; i < m_arrTab.GetSize(); i++)
	{
		if (m_arrTab[i].rcItem.PtInRect(point))
		{	
			return i;
		}
	}
	return -1;
}



BEGIN_MESSAGE_MAP(CXTPTabWorkspaceInPlate, CWnd)
	//{{AFX_MSG_MAP(CXTPTabWorkspaceInPlate)
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SYSCOLORCHANGE()
END_MESSAGE_MAP()



void CXTPTabWorkspaceInPlate::OnDestroy() 
{
	CWnd::OnDestroy();
	m_arrTab.RemoveAll();
}

void CXTPTabWorkspaceInPlate::OnIdleUpdateCmdUI() 
{
	UpdateContents(TRUE);
}

CWnd* CXTPTabWorkspaceInPlate::MDIGetActive()
{
#ifdef _XTP_ACTIVEX
	HWND hWnd = (HWND)m_pTabClientWnd->SendMessage( WM_MDIGETACTIVE);
	return CWnd::FromHandle(hWnd);
#else
	return (m_pTabClientWnd->GetParentFrame()->MDIGetActive());

#endif
}


void CXTPTabWorkspaceInPlate::UpdateContents(BOOL bAddNewWindows/*=FALSE*/)
{
	if (!IsWindowVisible())
		return;
	
	ASSERT(m_pTabClientWnd!=NULL);

    // check MDI windows
    CMDIFrameWnd* pFrameWnd=GetParentFrame();
    if(pFrameWnd==NULL) return;

	BOOL bRecalc=m_pTabClientWnd->m_bForceToRecalc;

	// get pointer to currently active MDIChild
    CWnd* pActiveChildWnd = MDIGetActive();

    CMDIChildWnd* pChildWnd=NULL;
    //int nActiveIndex=-1;

	// start enumerating from currently active MDIChild
    if(pActiveChildWnd!=NULL)
		pChildWnd=(CMDIChildWnd*)pActiveChildWnd->GetWindow(GW_HWNDFIRST);

	if(!pChildWnd) return;
	
	CAPlateDoc *pDoc = (CAPlateDoc *)pChildWnd->GetActiveDocument();
	if(pDoc==NULL)	return;

	long s_nColor = 0;
	long nTabSize = 0;
	CGeneralBaseStd *pStd= pDoc->GetPlateInputStd();
	CGeneralBaseDlg* pBaseDlg = pStd->GetCurruntDlg();
	if(pBaseDlg)	
		nTabSize = pBaseDlg->GetTabCount();

	bRecalc = nTabSize!=m_arrTab.GetSize();		
	
	if(bRecalc)
	{	
		for(long nIndex = m_arrTab.GetSize() - 1; nIndex >= 0; nIndex--)
		{
			RemoveTabItem(m_arrTab[nIndex].hWnd, FALSE);		
		}
		if(pBaseDlg)
		{						
			for(long n=0; n<nTabSize; n++)
			{	
				long nPos=n;
				AddTabItem(nPos, pChildWnd,pBaseDlg->GetTabTitle(n),FALSE);			
			}				
		}
	}
	else
	{	
		CRect rect1, rect2;
		GetClientRect(&rect1);
		m_pTabClientWnd->GetClientRect(&rect2);
		if(m_arrTab.GetSize()>0 && rect1==rect2)
			bRecalc = TRUE;
	}
	if(pBaseDlg)
		m_nSelected = pBaseDlg->m_nTab;

	if(bRecalc)
	{
		// update the size and position of the tab control and MDIClient window
		if(::IsWindow(GetParentFrame()->GetSafeHwnd()))
			GetParentFrame()->RecalcLayout();
		m_pTabClientWnd->m_bForceToRecalc=FALSE;
	}
}

// retrieves pointer to the MDIFrame window
CMDIFrameWnd* CXTPTabWorkspaceInPlate::GetParentFrame() const 
{ 
	ASSERT(m_pTabClientWnd!=NULL);
	ASSERT(m_pTabClientWnd->IsAttached());

	// request MDIClient for the parent frame
	CMDIFrameWnd* pParentFrame=m_pTabClientWnd->GetParentFrame();
	ASSERT(pParentFrame!=NULL);

	return pParentFrame; 
}

// finds the tab item for specified window. Returns -1 if not found
int CXTPTabWorkspaceInPlate::FindTabItem(const HWND hWnd) const
{
	// loop through all tab items
	for(int nIndex=0; nIndex<m_arrTab.GetSize(); nIndex++)
	{
		// check for window handle
		if(m_arrTab[nIndex].hWnd == hWnd)
		{
			return nIndex;
		}
	}

	return -1;
}

// finds the tab item for specified window. Returns -1 if not found
int CXTPTabWorkspaceInPlate::FindTabItem(const CString& szTitle) const
{
	// loop through all tab items
	for(int nIndex=0; nIndex<m_arrTab.GetSize(); nIndex++)
	{
		// check for window handle
		if(m_arrTab[nIndex].strText == szTitle)
		{
			return nIndex;
		}
	}

	return -1;
}


CWnd* CXTPTabWorkspaceInPlate::GetChildWnd(int nIndex)
{
	CWnd* pWnd = CWnd::FromHandle(m_arrTab[nIndex].hWnd);
	ASSERT(pWnd);
#ifndef _XTP_ACTIVEX
	ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));
#endif
	return pWnd;
}

// add new item into the tab control for specified MDIChild. If bRedraw is
// set to TRUE then framework will be redrawn in order to show new item.
BOOL CXTPTabWorkspaceInPlate::AddTabItem(long &s_nColor, const CWnd* pChildWnd, CString szTitle, BOOL bRedraw/*=TRUE*/,
								 BOOL bOnlyVisible/*=TRUE*/)
{	
	ASSERT(pChildWnd!=NULL);
	ASSERT(::IsWindow(pChildWnd->GetSafeHwnd()));

	// make sure we add MDIChild window
	if((pChildWnd->GetExStyle()&WS_EX_MDICHILD)==0)
		return FALSE;

	// make sure it is visible at the moment
	if(bOnlyVisible && (pChildWnd->GetStyle()&WS_VISIBLE)==0)
		return FALSE;

	// set item text to window text
	CString sWindowText=szTitle;//GetTextForTabItem(pChildWnd);

	// save information about new entry
	TAB_ITEM_ENTRY newTabItemEntry;
	newTabItemEntry.strText=sWindowText;
	newTabItemEntry.hWnd = pChildWnd->GetSafeHwnd();
	newTabItemEntry.bFound = TRUE;

	newTabItemEntry.nColor = s_nColor++;

	m_nSelected = m_arrTab.Add(newTabItemEntry);

	// redraw the tab control
	if(!m_arrTab.GetSize() && bRedraw)
		InvalidateRect(NULL);

	// update the size and position of the tab control and MDIClient window
	if(bRedraw)
	{
		ASSERT(::IsWindow(GetParentFrame()->GetSafeHwnd()));
		GetParentFrame()->RecalcLayout();
	}

	return TRUE;
}

// remove item from the tab control that corresponds to specified MDIChild
BOOL CXTPTabWorkspaceInPlate::RemoveTabItem(const HWND hWnd, BOOL bRedraw/*=TRUE*/)
{
	// find the item
	int nTabItem = FindTabItem(hWnd);

	if(nTabItem==-1)
		return FALSE;

	// remove entry from the internal array of created items
	m_arrTab.RemoveAt(nTabItem);

	// update the size and position of the tab control and MDIClient window
	if(bRedraw)
	{
		if(::IsWindow(GetParentFrame()->GetSafeHwnd()))
			GetParentFrame()->RecalcLayout();
	}

	return TRUE;
}


CString CXTPTabWorkspaceInPlate::GetTextForTabItem(const CWnd* pChildWnd) const
{
	ASSERT(pChildWnd!=NULL);

#ifndef _XTP_ACTIVEX
	ASSERT(pChildWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

	CString sWindowText=(LPCTSTR)((CWnd*)pChildWnd)->
		SendMessage(WM_XTP_GETWINDOWTEXT);

	if(sWindowText.IsEmpty())
	{
		CDocument* pDoc=((CMDIChildWnd*)pChildWnd)->GetActiveDocument();
		if(pDoc!=NULL)
			sWindowText=pDoc->GetTitle();

		if(sWindowText.IsEmpty())
			pChildWnd->GetWindowText(sWindowText);
	}
#else
	CString sWindowText;
	pChildWnd->GetWindowText(sWindowText);

#endif

	return sWindowText;
}
void CXTPTabWorkspaceInPlate::OnPaint() 
{
	CPaintDC dcPaint(this); 
	CRect rc;
	GetClientRect(&rc);
	CXTPBufferDC dc(dcPaint, rc);
	
	if (m_pTabClientWnd)
	{
		CRect rectClient;
		m_pTabClientWnd->GetWindowRect(rectClient);
		ScreenToClient(&rectClient);
		dcPaint.ExcludeClipRect(rectClient);

	}
	UpdateContents(TRUE);
	PaintManager()->DrawTabControl(&dc, &rc, this, TRUE);	
}

BOOL CXTPTabWorkspaceInPlate::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CXTPTabWorkspaceInPlate::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (TRUE)//(m_dwStyle & CBRS_ALIGN_TOP)
	{
		if (m_btnClose.PtInRect(point))
		{	
			if (m_btnClose.Click(this, point))
			{
				CWnd* pActiveChildWnd= MDIGetActive();

				if (pActiveChildWnd)
					pActiveChildWnd->SendMessage(WM_CLOSE);
			}
			return;
		}

		if (m_btnLeft.PtInRect(point))
		{		
			m_btnLeft.Click(this, point, TRUE);
			return;
		}

		if (m_btnRight.PtInRect(point))
		{
			m_btnRight.Click(this, point, TRUE);
			return;
		}
	}

	m_nItemTracking = -1;
	m_lstRects.RemoveAll();

	for (int i = 0; i < m_arrTab.GetSize(); i++)
	{
		if (m_arrTab[i].rcItem.PtInRect(point))
		{
			for (int j = 0; j < m_arrTab.GetSize(); j++)
				m_lstRects.Add(m_arrTab[j].rcItem);

			m_nItemTracking = i;
			SetCapture();
			Invalidate(FALSE);
			break;
			
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPTabWorkspaceInPlate::MDIActivate(CWnd* pWnd)
{
	if (!pWnd)
		return;

	#ifndef _XTP_ACTIVEX
		m_pTabClientWnd->GetParentFrame()->MDIActivate(pWnd);
	#else
		m_pTabClientWnd->SendMessage(WM_MDIACTIVATE, (WPARAM)pWnd->m_hWnd, 0);
	#endif
}

void CXTPTabWorkspaceInPlate::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if (m_nItemTracking != -1)
	{
		MDIActivate(GetChildWnd(m_nItemTracking));

		CWnd				*pActiveChildWnd = MDIGetActive();
		CMDIChildWnd		*pChildWnd	= pActiveChildWnd ? (CMDIChildWnd*)pActiveChildWnd->GetWindow(GW_HWNDFIRST) : NULL;
		CAPlateDoc			*pDoc		= pChildWnd ? (CAPlateDoc *)pChildWnd->GetActiveDocument() : NULL;
		CGeneralBaseStd		*pStd		= pDoc ? pDoc->GetPlateInputStd() : NULL;
		CGeneralBaseDlg		*pBaseDlg	= pStd ? pStd->GetCurruntDlg() : NULL;
		if(pBaseDlg) 
		{
			pBaseDlg->m_nTab = m_nItemTracking;
			pBaseDlg->OnSelChangedTab(m_nItemTracking);
			pBaseDlg->DrawInputDomyunView(TRUE);
		}

		m_nItemTracking = -1;
		ReleaseCapture();	

		m_nItemFocused = 0;
		OnIdleUpdateCmdUI();
		Invalidate(FALSE);
	}	

	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPTabWorkspaceInPlate::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nItemTracking != -1)
	{
		for (int i = 0; i < m_lstRects.GetSize(); i++)
		{
			if (i != m_nItemTracking && m_lstRects[i].PtInRect(point))
			{
				
				TAB_ITEM_ENTRY item = m_arrTab[m_nItemTracking];
				m_arrTab[m_nItemTracking] = m_arrTab[i];
				m_arrTab[i] = item;

				m_nItemTracking = i;

				OnIdleUpdateCmdUI();
				Invalidate(FALSE);
				break;
			}
		}

	} else 
	if (TRUE)//if (m_dwStyle & CBRS_ALIGN_TOP)
	{
		m_btnClose.CheckForMouseOver(this, point);
		m_btnLeft.CheckForMouseOver(this, point);
		m_btnRight.CheckForMouseOver(this, point);
	}

	HighlightFocused();

	CWnd::OnMouseMove(nFlags, point);
}

void CXTPTabWorkspaceInPlate::OnCaptureChanged(CWnd *pWnd)
{	
	if (m_nItemTracking != -1)
	{	
		MDIActivate(GetChildWnd(m_nItemTracking));
		m_nItemTracking = -1;
	}

	CWnd::OnCaptureChanged(pWnd);
}

void CXTPTabWorkspaceInPlate::OnMouseLeave()
{
	OnMouseMove(0,CPoint(-1, -1));
}

int CXTPTabWorkspaceInPlate::GetItemFocused() 
{ 
	if (m_nItemTracking != -1)
		return m_nItemTracking;

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	return HitTest(pt);
}

void CXTPTabWorkspaceInPlate::HighlightFocused()
{	
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	int nItemFocused = HitTest(pt);

	if (!XTPMouseManager()->IsTrackedLock() && IsTopParentActive() && nItemFocused != m_nItemFocused)
	{
		m_nItemFocused = nItemFocused;
		Invalidate(FALSE);
		if (m_nItemFocused != -1) 
		{
			TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd};	
			_TrackMouseEvent(&tme);
		}
	}
}
void CXTPTabWorkspaceInPlate::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
}

void CXTPTabWorkspaceInPlate::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	PaintManager()->RefreshMetrics();
}


void CXTPTabWorkspaceInPlate::AdjustTabs(int nWidth)
{
	#define GetRect(nIndex) m_arrTab[nIndex].rcItem

	try
	{
		nWidth -= 9;
		
		int nLength = 0;
		int nSize = (int)m_arrTab.GetSize();
		int i;
		for (i = 0; i < nSize; i++)
		{
			GetRect(i).OffsetRect(-GetRect(i).left, 0);
			nLength += GetRect(i).Width();
		}

		if (nWidth < nLength)
		{
			if (nSize == 1)
				GetRect(0).right = nWidth;
			else
			{
				
				int* nIndex = new int[nSize];
				int i;
				for (i = 0; i < nSize; i++) nIndex[i] = i;
				
				m_arrTab.SortIndexes(nIndex);
				
				
				
				int nDelta = nLength - nWidth;
				
				for (i = 0; i <nSize; i++)
				{
					int k = i;
					int nItemWidth = GetRect(nIndex[i]).Width();
					
					while( k + 1 < nSize && GetRect(nIndex[k + 1]).Width() == nItemWidth) k++;
					
					if (k + 1 < nSize) 
					{
						int nNextWidth = GetRect(nIndex[k + 1]).Width();
						int nCanFree = (k + 1) * (nItemWidth - nNextWidth);
						
						if (nCanFree > nDelta)
						{
							for (int j = 0; j <= k; j++)
								GetRect(nIndex[j]).right-= nDelta/(k + 1);
							break;
							
						} else
						{
							for (int j = 0; j <= k; j++)
							{
								nDelta -= GetRect(nIndex[j]).Width() - nNextWidth;
								GetRect(nIndex[j]).right = nNextWidth;
							}
							
						}
						
					} else
					{
						for (int j = 0; j <nSize; j++)
							GetRect(j).right = nWidth/nSize;
						break;
					}					
					i = k;
				}
				
				delete[] nIndex;
			}
		}
		
		
		
		int x = 4;
		for (i = 0; i < nSize; i++)
		{
			GetRect(i).OffsetRect(x, 0);
			x += GetRect(i).Width();
		}

	}
	catch (...) 
	{
	}
}

HICON CXTPTabWorkspaceInPlate::GetIcon(int nIndex)
{

	HICON hIcon = (HICON)GetChildWnd(nIndex)->SendMessage(WM_XTP_GETTABICON);
	if (hIcon)
		return hIcon;

	HWND hWnd = m_arrTab[nIndex].hWnd;
	hIcon=(HICON)::GetClassLong(hWnd,GCL_HICONSM);

	if(hIcon==NULL)
	{
		hIcon=(HICON)::SendMessage(hWnd, WM_GETICON,ICON_SMALL, 0);
		if(hIcon==NULL)
		{
			hIcon=(HICON)::GetClassLong(hWnd,GCL_HICON);
			if(hIcon==NULL)
				hIcon=(HICON)::SendMessage(hWnd, WM_GETICON,ICON_BIG, 0);
		}
	}
	return hIcon;
}

COLORREF CXTPTabWorkspaceInPlate::GetColor(int nIndex)
{
	const COLORREF clrTable[] = {
		RGB(138, 168, 228), RGB(255, 219, 117), RGB(189, 205, 159), RGB(240, 158, 159),
			RGB(186, 166, 225), RGB(154, 191, 180), RGB(247, 182, 131), RGB(216, 171, 192)
	};
#ifndef _XTP_ACTIVEX
	COLORREF nColor = (COLORREF)GetChildWnd(nIndex)->SendMessage(WM_XTP_GETTABCOLOR);

	if (nColor == 0)
		return clrTable[m_arrTab[nIndex].nColor % (sizeof(clrTable) / sizeof(COLORREF))];

	if (nColor >= XTP_TABCOLOR_BLUE && nColor <= XTP_TABCOLOR_MAGENTA)
		return clrTable[nColor - XTP_TABCOLOR_BLUE];

	return nColor;
	

#else
	return clrTable[m_arrTab[nIndex].nColor % (sizeof(clrTable) / sizeof(COLORREF))];
#endif	

}


BOOL CXTPTabWorkspaceInPlate::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN ||
		pMsg->message == WM_LBUTTONDBLCLK ||
		pMsg->message == WM_RBUTTONDBLCLK)
	{
		if (GetParentFrame()->SendMessage(WM_XTP_PRETRANSLATEMOUSEMSG, (WPARAM)pMsg->message, pMsg->lParam))
			return TRUE;
	}		

	return CWnd::PreTranslateMessage(pMsg);
}

// {F10C6EB2-2F2F-4c05-92CF-6AEB0BFA0E6D}
static const GUID IID_ITabWorkspace = 
{ 0xf10c6eb2, 0x2f2f, 0x4c05, { 0x92, 0xcf, 0x6a, 0xeb, 0xb, 0xfa, 0xe, 0x6d } };

// {F9DE0A11-209A-4655-9316-30879967B6A9}
static const GUID IID_ITabWorkspaceEvents = 
{ 0xf9de0a11, 0x209a, 0x4655, { 0x93, 0x16, 0x30, 0x87, 0x99, 0x67, 0xb6, 0xa9 } };


/////////////////////////////////////////////////////////////////////////////
// CXTPTabClientWndInPlate

CXTPTabClientWndInPlate::CXTPTabClientWndInPlate()
{
	m_pParentFrame=NULL;
	m_bForceToRecalc=FALSE;

	m_tab.m_pTabClientWnd=this;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPTabClientWndInPlate::~CXTPTabClientWndInPlate()
{
}


BEGIN_MESSAGE_MAP(CXTPTabClientWndInPlate, CWnd)
	//{{AFX_MSG_MAP(CXTPTabClientWndInPlate)
	ON_MESSAGE(WM_MDIACTIVATE,OnMDIActivate)
	ON_MESSAGE(WM_MDICREATE,OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY,OnMDIDestroy)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CXTPTabClientWndInPlate::Attach(const CMDIFrameWnd* pParentFrame, 
							 DWORD dwTabCtrlStyle/*=DEFAULT_TABCTRLSTYLE*/)
{
	ASSERT(pParentFrame!=NULL);
	ASSERT(::IsWindow(pParentFrame->GetSafeHwnd()));

	// make sure that m_tab.m_pTabClientWnd is not NULL.
	if (m_tab.m_pTabClientWnd == NULL)
		m_tab.m_pTabClientWnd = this;

	// check if already attached
	if(IsAttached())
	{
		TRACE(_T("CXTPTabClientWndInPlate::Attach: window has already been attached. Call Detach() function before!\n"));
		return FALSE;
	}

	// make sure the specified window is/derived from CMDIFrameWnd class
	if(!pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		TRACE(_T("CXTPTabClientWndInPlate::Attach: specified frame window is not of CMDIFrameWnd class (or derived)!\n"));
		return FALSE;
	}

	// try to sublass MDIClient window
	if(!SubclassWindow(pParentFrame->m_hWndMDIClient))
	{
		TRACE(_T("CXTPTabClientWndInPlate::Attach: failed to subclass MDI Client window\n"));
		return FALSE;
	}

	// save the pointer to parent MDIFrame
	m_pParentFrame=(CMDIFrameWnd*)pParentFrame;


	// create tab control
	//
	
	ASSERT(!::IsWindow(m_tab.GetSafeHwnd()));
	CRect rect(0,0,0,0);

	// make sure WS_POPUP style is not used
	ASSERT((dwTabCtrlStyle&WS_POPUP)==0);
	// make sure the following styles are used 
	dwTabCtrlStyle|=(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_CHILD|WS_VISIBLE);
	
	// try to create the tab control
    if(!m_tab.Create(AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)), _T(""), dwTabCtrlStyle, rect, m_pParentFrame, IDC_TABWORKSPACE))
	{
		TRACE(_T("CXTPTabClientWndInPlate::Attach: failed to create tab control\n"));
		// if failed we detach the parent frame
		Detach();
		return FALSE;
	}

	// update the size and position of the tab control and MDIClient window
	m_pParentFrame->RecalcLayout();

	// populate tab control with MDIChild windows if any exist at the moment
	m_tab.UpdateContents(TRUE);

	PaintManager()->RefreshMetrics();

	return TRUE;
}


BOOL CXTPTabClientWndInPlate::Detach()
{
	// check if any attached
	if(!IsAttached())
	{
		TRACE(_T("CXTPTabClientWndInPlate::Attach: there is nothing to detach! Window hasn't been attached!\n"));
		return FALSE;
	}

	// destroy tab control
	if(::IsWindow(m_tab.GetSafeHwnd()))
		m_tab.DestroyWindow();

	m_tab.m_pTabClientWnd=NULL;

	// unsubclass MDIClient window
	UnsubclassWindow();

	// update the size and position of the MDIClient window
	if(::IsWindow(m_pParentFrame->GetSafeHwnd()))
		m_pParentFrame->RecalcLayout();

	m_pParentFrame=NULL;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CXTPTabClientWndInPlate message handlers


// crucial function that calculate the size of MDIClient window. Called
// by parent MDIFrame window
void CXTPTabClientWndInPlate::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{

    // now do the laying out
    HDWP dwh=BeginDeferWindowPos(2);

    // move tab window
    if(::IsWindow(m_tab.m_hWnd) && (m_tab.GetStyle()&WS_VISIBLE)==WS_VISIBLE)
    {
		{
			CRect rectTab=lpClientRect;

			// move tab control
			dwh=::DeferWindowPos(dwh,m_tab.m_hWnd,NULL,lpClientRect->left,
				lpClientRect->top,
				lpClientRect->right-lpClientRect->left,
				lpClientRect->bottom-lpClientRect->top, SWP_NOZORDER);
			
			m_tab.Invalidate(FALSE);

			PaintManager()->DrawTabControl(0, lpClientRect, &m_tab, FALSE);
		}

		if(lpClientRect->bottom<lpClientRect->top || 
			lpClientRect->right<lpClientRect->left)
			::memset(lpClientRect,0,sizeof(RECT));

		::ShowWindow(m_tab.m_hWnd,SW_SHOWNA);
    }

#ifndef _XTP_ACTIVEX
	// move MDIClient window
    dwh=::DeferWindowPos(dwh,m_hWnd,NULL,lpClientRect->left,lpClientRect->top,
		lpClientRect->right-lpClientRect->left,
		lpClientRect->bottom-lpClientRect->top,
		SWP_NOZORDER);
#endif

    EndDeferWindowPos(dwh);

	CWnd::CalcWindowRect(lpClientRect, nAdjustType);
}

// handler for WM_MDIACTIVATE message. Will select corresponding 
// tab control item
LONG CXTPTabClientWndInPlate::OnMDIActivate(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	LONG lResult=Default();
	
	HWND hActiveWnd=(HWND)wParam;
	// find corresponding tab control item 
	int nTabItem=m_tab.FindTabItem(hActiveWnd);
	if(nTabItem!=-1)
	{
		ASSERT(nTabItem>=0 && nTabItem<m_tab.GetItemCount());
		m_tab.SetCurSel(nTabItem);
	}

	// update the contents of the tab control afterwards
	//m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);
	m_tab.OnIdleUpdateCmdUI();

	return lResult;
}


// handler for WM_MDICREATE message. Will add new item into the 
// tab control
LONG CXTPTabClientWndInPlate::OnMDICreate(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	LONG lResult=Default();

	// lResult is handle to the newd MDIChild window if it was
	// successfully created
	if(lResult!=NULL)
	{
		// there shouldn't be such item in the tab control
		ASSERT(m_tab.FindTabItem((HWND)lResult)==-1);

		// update the tab control contents
		m_tab.SetCurSel(-1);
	    //m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);
		m_tab.OnIdleUpdateCmdUI();
	}

	return lResult;
}


// handler for WM_MDIDESTROY message. Will remove the correspondent item 
// from the tab control
LONG CXTPTabClientWndInPlate::OnMDIDestroy(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// remove the item from the tab control
	m_tab.RemoveTabItem((HWND)wParam);

	LONG lResult=Default();

	// update the contents of the tab control afterwards
	//m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);
	m_tab.OnIdleUpdateCmdUI();

	return lResult;
}

// update the size of the tab control and the MDIClient window
void CXTPTabClientWndInPlate::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	// update the contents of the tab control afterwards
	m_bForceToRecalc=TRUE;
	//m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);
	m_tab.OnIdleUpdateCmdUI();
}


// show/hide tab control if MDIClient is shown/hidden
void CXTPTabClientWndInPlate::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow,nStatus);
	
	// TODO: Add your message handler code here

	if(nStatus==0 && ::IsWindow(m_tab.GetSafeHwnd()))
	{
		if(bShow)
			m_tab.ModifyStyle(NULL,WS_VISIBLE);
		else
			m_tab.ModifyStyle(WS_VISIBLE,NULL);
	}
}


DWORD CXTPTabClientWndInPlate::GetFlags()
{
	return m_tab.m_dwFlags;
}
void CXTPTabClientWndInPlate::SetFlags(DWORD dwFlags)
{
	m_tab.m_dwFlags = dwFlags;
	Invalidate(FALSE);
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTabClientWndInPlate, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPTabClientWndInPlate, "Flags", dispidFlags, GetFlags, SetFlags, VT_I4)
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CXTPTabClientWndInPlate, CCmdTarget)
	INTERFACE_PART(CXTPTabClientWndInPlate, IID_ITabWorkspace, Dispatch)
END_INTERFACE_MAP()



#endif