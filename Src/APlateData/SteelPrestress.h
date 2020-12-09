// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// CSteelPrestress.h: interface for the CSteelPrestress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEELPRESTRESS_H__43BC5D90_46B2_42C3_A716_119B73D46B75__INCLUDED_)
#define AFX_STEELPRESTRESS_H__43BC5D90_46B2_42C3_A716_119B73D46B75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CSteelPrestressDataPlate : public CSteelPrestressData
{
// Constructor
public:
	CSteelPrestressDataPlate();
	virtual ~CSteelPrestressDataPlate();

public:
	void SerializeOld(CArchive& ar);
};

class AFX_EXT_CLASS CSteelPrestressPlate : public CSteelPrestress
{
public:
	CSteelPrestressPlate();
	virtual ~CSteelPrestressPlate();

	void SerializeOld(CArchive& ar);
};

#endif // !defined(AFX_STEELPRESTRESS_H__43BC5D90_46B2_42C3_A716_119B73D46B75__INCLUDED_)
