// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_FONT_H__C02FEF77_5E0D_4032_9FA1_2ED78EF7E4DC__INCLUDED_)
#define AFX_FONT_H__C02FEF77_5E0D_4032_9FA1_2ED78EF7E4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// COleFont wrapper class

class COleFont : public COleDispatchDriver
{
public:
	COleFont() {}		// Calls COleDispatchDriver default constructor
	COleFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COleFont(const COleFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CY GetSize();
	void SetSize(const CY&);
	BOOL GetBold();
	void SetBold(BOOL);
	BOOL GetItalic();
	void SetItalic(BOOL);
	BOOL GetUnderline();
	void SetUnderline(BOOL);
	BOOL GetStrikethrough();
	void SetStrikethrough(BOOL);
	short GetWeight();
	void SetWeight(short);
	short GetCharset();
	void SetCharset(short);

// Operations
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONT_H__C02FEF77_5E0D_4032_9FA1_2ED78EF7E4DC__INCLUDED_)
