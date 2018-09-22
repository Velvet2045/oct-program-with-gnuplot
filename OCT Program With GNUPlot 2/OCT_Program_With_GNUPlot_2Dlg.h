
// OCT Program With GNUPlot 2Dlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp> 
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <string>
#include <conio.h>   
#include "StaticGNUPlot.h"
#include "OCTProgramAbou.h"
#include "OCTIntroBox.h"
#include "OCTDlgOrthogonal.h"

// COCTProgramWithGNUPlot2Dlg ��ȭ ����
class COCTProgramWithGNUPlot2Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	COCTProgramWithGNUPlot2Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OCTPROGRAMWITHGNUPLOT2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_lenX;
	CString m_lenY;
	CString m_lenZ;
	CString m_volume;
	CString m_distance;
	// ��ΰ� �߰��Ǿ����� Ȯ���ϴ� ����
	bool isAddress;
	// ��� ���Ÿ�
	double meanDistance;
	// ���� ���
	CString folderPath;
	CString filePath;
	// �̹��� ���, �̹��� �迭 ��,�� ��
	int imageCount;
	int imgVerticesCols, imgVerticesRows;
	CStaticGNUPlot m_plot;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnOpenVideofile();
	afx_msg void OnImagefileCirrusoct();
	afx_msg void OnImagefileSpectralis();
	afx_msg void OnFileExit();
	afx_msg void OnHelpIntroduction();
	afx_msg void OnHelpAbout();
};
