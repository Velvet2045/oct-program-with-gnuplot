#pragma once
#include "afxwin.h"


// OCTIntroBox ��ȭ �����Դϴ�.

class OCTIntroBox : public CDialogEx
{
	DECLARE_DYNAMIC(OCTIntroBox)

public:
	OCTIntroBox(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~OCTIntroBox();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_introduction;
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit m_txtScroll;
};
