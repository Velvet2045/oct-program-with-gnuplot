#pragma once


// OCTProgramAbou ��ȭ �����Դϴ�.

class OCTProgramAbou : public CDialogEx
{
	DECLARE_DYNAMIC(OCTProgramAbou)

public:
	OCTProgramAbou(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~OCTProgramAbou();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
