#pragma once
#include "StaticGNUPlot.h"

// OCTDlgOrthogonal ��ȭ �����Դϴ�.

class OCTDlgOrthogonal : public CDialogEx
{
	DECLARE_DYNAMIC(OCTDlgOrthogonal)

public:
	OCTDlgOrthogonal(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~OCTDlgOrthogonal();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ORTHOGONAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CStaticGNUPlot* m_plot;
	afx_msg void OnBnClickedBtnXAxis();
	afx_msg void OnBnClickedBtnYAxis();
	afx_msg void OnBnClickedBtnZAxis();
};
