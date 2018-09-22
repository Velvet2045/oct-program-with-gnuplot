
// OCT Program With GNUPlot 2Dlg.h : 헤더 파일
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

// COCTProgramWithGNUPlot2Dlg 대화 상자
class COCTProgramWithGNUPlot2Dlg : public CDialogEx
{
// 생성입니다.
public:
	COCTProgramWithGNUPlot2Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OCTPROGRAMWITHGNUPLOT2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
	// 경로가 추가되었는지 확인하는 변수
	bool isAddress;
	// 평균 상대거리
	double meanDistance;
	// 폴더 경로
	CString folderPath;
	CString filePath;
	// 이미지 장수, 이미지 배열 행,열 수
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
