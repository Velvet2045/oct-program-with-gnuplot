// OCTDlgOrthogonal.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "OCT Program With GNUPlot 2.h"
#include "OCTDlgOrthogonal.h"
#include "afxdialogex.h"


// OCTDlgOrthogonal ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(OCTDlgOrthogonal, CDialogEx)

OCTDlgOrthogonal::OCTDlgOrthogonal(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ORTHOGONAL, pParent)
{

}

OCTDlgOrthogonal::~OCTDlgOrthogonal()
{
}

void OCTDlgOrthogonal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OCTDlgOrthogonal, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_X_AXIS, &OCTDlgOrthogonal::OnBnClickedBtnXAxis)
	ON_BN_CLICKED(IDC_BTN_Y_AXIS, &OCTDlgOrthogonal::OnBnClickedBtnYAxis)
	ON_BN_CLICKED(IDC_BTN_Z_AXIS, &OCTDlgOrthogonal::OnBnClickedBtnZAxis)
END_MESSAGE_MAP()


// OCTDlgOrthogonal �޽��� ó�����Դϴ�.


void OCTDlgOrthogonal::OnBnClickedBtnXAxis()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	(*m_plot).cmd("set view 90,90,1,1");
	(*m_plot).cmd("replot");
}


void OCTDlgOrthogonal::OnBnClickedBtnYAxis()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	(*m_plot).cmd("set view 90,0,1,1");
	(*m_plot).cmd("replot");
}


void OCTDlgOrthogonal::OnBnClickedBtnZAxis()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	(*m_plot).cmd("set view 180,0,1,1");
	(*m_plot).cmd("replot");
}
