// OCTDlgOrthogonal.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "OCT Program With GNUPlot 2.h"
#include "OCTDlgOrthogonal.h"
#include "afxdialogex.h"


// OCTDlgOrthogonal 대화 상자입니다.

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


// OCTDlgOrthogonal 메시지 처리기입니다.


void OCTDlgOrthogonal::OnBnClickedBtnXAxis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	(*m_plot).cmd("set view 90,90,1,1");
	(*m_plot).cmd("replot");
}


void OCTDlgOrthogonal::OnBnClickedBtnYAxis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	(*m_plot).cmd("set view 90,0,1,1");
	(*m_plot).cmd("replot");
}


void OCTDlgOrthogonal::OnBnClickedBtnZAxis()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	(*m_plot).cmd("set view 180,0,1,1");
	(*m_plot).cmd("replot");
}
