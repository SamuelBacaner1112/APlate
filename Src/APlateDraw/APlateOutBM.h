// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// APlateOutBM.h: interface for the CAPlateOutBM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APLATEOUTBM_H__2E1327C8_7FCA_40B7_A52C_2BE7B31FC46F__INCLUDED_)
#define AFX_APLATEOUTBM_H__2E1327C8_7FCA_40B7_A52C_2BE7B31FC46F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAPlateOutBM : public CDrawPage
{
public:
	CAPlateOutBM();
	virtual ~CAPlateOutBM();

	void DrawOnePage();
	long GetPageSu();
	double GetScale();
	CString GetTitle();

protected:
	//��������ǥ
	void AddPageBMSang(CDomyun *pDomP, long nSubPage);
	void AddPageBMStandard(CDomyun *pDomP, long nSubPage);
	void AddPageBMCar(CDomyun *pDomP, long nSubPage);
	void AddPageBMSectionType(CDomyun *pDomP, long nSubPage);
	void AddPageBMBuje(CDomyun *pDomP, long nSubPage);
	void AddPageBMBujeTotal(CDomyun *pDomP, long nSubPage);
	void AddPageBMSizeTypeTotal(CDomyun *pDomP, long nSubPage);
	void AddPageBMDeductBuje(CDomyun *pDomP, long nSubPage);
	void AddPageBMDeductSang(CDomyun *pDomP, long nSubPage);
	void AddPageBMEtc(CDomyun *pDomP, long nSubPage);//��Ÿ

	//��������ǥ
	void AddPageBMPaintSang(CDomyun *pDomP, long nSubPage);
	void AddPageBMPaintBuje(CDomyun *pDomP, long nSubPage);
	void AddPageBMPaintEtc(CDomyun *pDomP, long nSubPage);//��Ÿ

	//��������ǥ
	void AddPageWeldSang(CDomyun *pDomP, long nSubPage);
	void AddPageWeldType(CDomyun *pDomP, long nSubPage);
	void AddPageWeldThick(CDomyun *pDomP, long nSubPage);

private:
	// ��������ǥ//////////////////////////////////////
	// ������ǥ
	long m_nPageSang;
	CMatrixStr m_MatrixSang;
	void DrawBMSang(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMSangTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixSang();
	// ǥ������ǥ
	long m_nPageStandard;
	CMatrixStr m_MatrixStandard;
	void DrawBMStandard(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMStandardTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixStandard();
	// �������� ����ǥ
	long m_nPageCar;
	CMatrixStr m_MatrixCar;
	void DrawBMCar(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMCarTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixCar();
	// �ܸ�Ÿ�Ժ� ����ǥ
	long m_nPageSectionType;
	CMatrixStr m_MatrixSectionType;
	void DrawBMSectionType(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMSectionTypeTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixSectionType();
	// ���������Ѱ� ����ǥ//��Ÿ
	CMatrixStr m_MatrixCarTotal;
	void DrawBMCarTotal(CDomyun *pDomP, CDPoint Base);
	void DrawBMCarTotalTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixCarTotal();
	// ������ �β��� ����ǥ//��Ÿ 
	CMatrixStr m_MatrixThick;
	void DrawBMThick(CDomyun *pDomP, CDPoint Base);
	void DrawBMThickTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixThick();
	// ���纰 ����ǥ
	long m_nPageBuje;
	CMatrixStr m_MatrixBuje;
	void DrawBMBuje(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMBujeTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixBuje();
	// ���纰 �Ѱ� ����ǥ
	long m_nPageBujeTotal;
	CMatrixStr m_MatrixBujeTotal;
	void DrawBMBujeTotal(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMBujeTitleTotal(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixBujeTotal();
	// �Ŵ��� �Ѱ� ����ǥ//��Ÿ
	CMatrixStr m_MatrixGirderTotal;
	void DrawBMGirderTotal(CDomyun *pDomP, CDPoint Base);
	void DrawBMGirderTotalTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixGirderTotal();

	// ���������� �Ѱ� ����ǥ
	long m_nPageSizeTypeTotal;
	CMatrixStr m_MatrixSizeTypeTotal;
	void DrawBMSizeTypeTotal(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMSizeTypeTotalTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixSizeTypeTotal();	
	
	// �󼼺����� ����ǥ
	long m_nPageDeductSang;
	CMatrixStr m_MatrixDeductSang;
	void DrawBMDeductSang(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMDeductSangTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixDeductSang();
	// ���纰 ���� ����ǥ
	long m_nPageDeductBuje;
	CMatrixStr m_MatrixDeductBuje;
	void DrawBMDeductBuje(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMDeductBujeTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixDeductBuje();

	long m_nPageEtc;   //���� ��Ÿ ����
	// ���� ���ǥ//////////////////////////////////////
	// �� ���ǥ
	long m_nPagePaintSang;
	CMatrixStr m_MatrixPaintSang;
	void DrawBMPaintSang(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMPaintSangTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixPaintSang();
	// ���纰 ���ǥ
	long m_nPagePaintBuje;
	CMatrixStr m_MatrixPaintBuje;
	void DrawBMPaintBuje(CDomyun *pDomP, long nSttRow, long nEndRow, CDPoint Base);
	void DrawBMPaintBujeTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixPaintBuje();
	// �Ѱ� ���ǥ
	CMatrixStr m_MatrixPaintTotal;
	void DrawBMPaintTotal(CDomyun *pDomP, CDPoint Base);
	void DrawBMPaintTotalTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixPaintTotal();
	// ��Ÿ ���ǥ(�Ѱ�, ��Ʈ)
	CMatrixStr m_MatrixPaintBolt;
	void DrawBMPaintBolt(CDomyun *pDomP,  CDPoint Base);
	void DrawBMPaintBoltTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixPaintBolt();

	long m_nPagePaintEtc; //���� ��Ÿ ����
	// ���� ����ǥ//////////////////////////////////////
	// �� ����ǥ
	long m_nPageWeldSang;
	CMatrixStr m_MatrixWeldSang;
	void DrawWeldSang(CDomyun *pDomP,  long nSttRow, long nEndRow, CDPoint Base);
	void DrawWeldSangTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixWeldSang();
	// Ÿ�Ժ� ����ǥ
	long m_nPageWeldType;
	CMatrixStr m_MatrixWeldType;
	void DrawWeldType(CDomyun *pDomP,  long nSttRow, long nEndRow, CDPoint Base);
	void DrawWeldTypeTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixWeldType();
	// ���� �β��� ����ǥ
	long m_nPageWeldThick;
	CMatrixStr m_MatrixWeldThick;
	void DrawWeldThick(CDomyun *pDomP,  long nSttRow, long nEndRow, CDPoint Base);
	void DrawWeldThickTitle(CDomyun *pDomP, CDPoint Base);
	void MakeMatrixWeldThick();
};

#endif // !defined(AFX_APLATEOUTBM_H__2E1327C8_7FCA_40B7_A52C_2BE7B31FC46F__INCLUDED_)
