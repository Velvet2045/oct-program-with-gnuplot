#pragma once
#include "StaticGNUPlot.h"

// OCTDlgOrthogonal 대화 상자입니다.

class OCTDlgOrthogonal : public CDialogEx
{
	DECLARE_DYNAMIC(OCTDlgOrthogonal)

public:
	OCTDlgOrthogonal(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~OCTDlgOrthogonal();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ORTHOGONAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStaticGNUPlot* m_plot;
	afx_msg void OnBnClickedBtnXAxis();
	afx_msg void OnBnClickedBtnYAxis();
	afx_msg void OnBnClickedBtnZAxis();
};
