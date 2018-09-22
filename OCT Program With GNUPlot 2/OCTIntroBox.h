#pragma once
#include "afxwin.h"


// OCTIntroBox 대화 상자입니다.

class OCTIntroBox : public CDialogEx
{
	DECLARE_DYNAMIC(OCTIntroBox)

public:
	OCTIntroBox(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~OCTIntroBox();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_introduction;
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit m_txtScroll;
};
