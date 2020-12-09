// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SAP2000Disp.h: interface for the CPlateSAP2000Disp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAP2000DISP_H__B7BB38DD_5F98_4A63_BC4F_022079BDB2DA__INCLUDED_)
#define AFX_SAP2000DISP_H__B7BB38DD_5F98_4A63_BC4F_022079BDB2DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  MAX_TOKEN		10

class CFEMManage;
class AFX_EXT_CLASS CPlateSAP2000Disp
{
public:
	CPlateSAP2000Disp(CFEMManage *pManage);
	virtual ~CPlateSAP2000Disp();

	int LoadSAP2000Disp (CString &sPathName);

protected:
	int GetLoadCaseIndex (char *pLCName);
	int ReadOneLine ();
	int GetOneLineNumber (char *pLine, double *pValue);
	int ProcessDataLine();

	CFEMManage *m_pFEM;
	CStdioFile m_fOut;
	char m_sOneLine [101];

	// 현재 상태 저장 변수.
	int		m_nHeaderID;
	int		m_nLoadCase;
	int		m_nNodeIdx;
	double  m_dValue [MAX_TOKEN];
};

#endif // !defined(AFX_SAP2000DISP_H__B7BB38DD_5F98_4A63_BC4F_022079BDB2DA__INCLUDED_)
