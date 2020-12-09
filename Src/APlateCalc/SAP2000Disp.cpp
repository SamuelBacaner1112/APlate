// SAP2000Disp.cpp: implementation of the CPlateSAP2000Disp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aplatecalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Header ID Type.
#define  HID_DISPLACE	0
#define  HID_REACTION	1
#define  HID_ELEFORCE	2
#define  HID_NOTUSE		3

// Data Type.
#define  LINE_HEADER	0
#define  LINE_DATA		1
#define  LINE_EOF		2
#define  LINE_NODATA	3

#define  POS_LCNAME		11
#define  POS_DATA		23

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlateSAP2000Disp::CPlateSAP2000Disp(CFEMManage *pManage)
{
	m_nHeaderID	= HID_NOTUSE;
	m_nLoadCase	= -1;
	m_nNodeIdx  = -1;
	m_pFEM   = pManage;
}

CPlateSAP2000Disp::~CPlateSAP2000Disp()
{

}


int CPlateSAP2000Disp::LoadSAP2000Disp(CString &sPathName)
{
	// File open
	if (m_fOut.Open (sPathName, CFile::modeRead | CFile::typeText) == NULL)
		return 0;

	int nType, nMaxNode = -1;
	while ((nType = ReadOneLine ()) != LINE_EOF)
	{
		switch (nType)
		{
		case LINE_HEADER :
			if (m_nHeaderID != HID_DISPLACE)
				m_nHeaderID = HID_NOTUSE;

			break;
		case LINE_DATA :
			nMaxNode = max (nMaxNode, m_nNodeIdx);
			if (!ProcessDataLine ())
			{
				return 0;
			}

			break;
		}
	}
	return 1;
}

int CPlateSAP2000Disp::ProcessDataLine()
{
	switch (m_nHeaderID)
	{
	case HID_DISPLACE :
		m_pFEM->SetDisplaceData(m_nLoadCase, m_nNodeIdx, m_dValue);
		break;
	}
	return 1;
}

int CPlateSAP2000Disp::ReadOneLine()
{
	static char *sHeaderID [] = {
		" J O I N T   D I S P L A C E M E N T S",
		" J O I N T   R E A C T I O N S",
		" F R A M E   E L E M E N T"};


	if (!m_fOut.ReadString (m_sOneLine, 100))
		return LINE_EOF;

	// Header Line인지 판별.
	int nHID = 0;
	while (nHID < 3 && strncmp (m_sOneLine, sHeaderID [nHID], strlen (sHeaderID [nHID])))
		nHID++;
	if (nHID < 3)	{
		m_nHeaderID = nHID;
		return LINE_HEADER;
	}
	// 일반 Data Line인지 판별.
	if (strlen (m_sOneLine) >= POS_DATA && GetOneLineNumber (m_sOneLine + POS_DATA, m_dValue) == 6)	{
		m_nNodeIdx  = atoi (m_sOneLine) - 1;
		m_nLoadCase = GetLoadCaseIndex (m_sOneLine + POS_LCNAME);
		if (m_nNodeIdx >= 0 && m_nLoadCase >= 0)
			return LINE_DATA;
	}
	return LINE_NODATA;
}

int CPlateSAP2000Disp::GetOneLineNumber(char *pLine, double *pValue)  // 6개 Data값만 저장.
{
	int nCount = 0;
	char *pNextPos = pLine;
	do	{
		pLine = pNextPos;
		pValue [nCount] = strtod (pLine, &pNextPos);
		nCount++;
	}
	while (pLine != pNextPos);
	return nCount - 1;
}

int CPlateSAP2000Disp::GetLoadCaseIndex(char *pLCName)
{
	char sBuf [101], *pToken, sSeps [] = " ";
	if (!strncmp (pLCName, "Maxima", 6) || !strncmp (pLCName, "Minima", 6))   {
		if (!m_fOut.ReadString (sBuf, 100))   // 다음줄의 Load Case Name 읽어들임.
			return -1;

		LPTSTR pszNextToken = NULL;
		pToken = _STRTOK_S(sBuf, sSeps, &pszNextToken );
		// pToken = strtok (sBuf, sSeps);
		if (!strncmp (pLCName, "Maxima", 6))
			strcat (pToken, " MAX");
		else
			strcat (pToken, " MIN");
		pLCName = pToken;
	}
	else   {
		int i = 14;
		_STRNCPY_S(sBuf, sizeof(sBuf), pLCName, i );
		// strncpy (sBuf, pLCName, i);
		while (sBuf [--i] == ' ');
		sBuf [++i] = NULL;
		pLCName = sBuf;
	}

	return m_pFEM->GetLoadCaseIndex(pLCName);
}

