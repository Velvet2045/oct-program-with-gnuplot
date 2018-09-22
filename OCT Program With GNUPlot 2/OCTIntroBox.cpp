// OCTIntroBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "OCT Program With GNUPlot 2.h"
#include "OCTIntroBox.h"
#include "afxdialogex.h"


// OCTIntroBox 대화 상자입니다.

IMPLEMENT_DYNAMIC(OCTIntroBox, CDialogEx)

OCTIntroBox::OCTIntroBox(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_introduction(_T(""))
{

}

OCTIntroBox::~OCTIntroBox()
{
}

void OCTIntroBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_introduction);
	DDV_MaxChars(pDX, m_introduction, 1000);
	DDX_Control(pDX, IDC_EDIT1, m_txtScroll);
}


BEGIN_MESSAGE_MAP(OCTIntroBox, CDialogEx)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// OCTIntroBox 메시지 처리기입니다.


BOOL OCTIntroBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CStdioFile tip_file;
	// 현재 경로에서 "ReadMe.txt" 파일을 연다.
	// CFile::modeRead  ->  파일을 읽기 모드로 연다. ( 파일이 없다면 실패한다. )
	// CFile::typeText  ->  파일의 속성을 텍스트 형식으로 지정한다. ( 생략 가능)
	if (tip_file.Open("ReadMe.txt", CFile::modeRead | CFile::typeText)) {

		CString display_str, str;

		// 파일에 더이상 텍스트 내용이 없을때까지 한줄씩 반복해서 읽어서 str 변수에 저장한다.
		while (tip_file.ReadString(str)) {

			// 읽어들인 한줄의 텍스트를 display_str 변수에 추가한다.
			display_str += str;

			// 멀티라인 속성을 가진 에디트 컨트롤은 줄의 끝에 "\r\n" 가 있어야지 줄바꿈을 해준다.
			// ReadString 함수가 한줄의 텍스트를 읽을때 해당줄의 끝에있는 "\n" 은 읽어서 버리기
			// 때문에 텍스트의 한줄끝에 "\n" 이 있다고 할지라도 반드시 "\n" 을 붙여야 합니다.
			display_str += "\r\n";
		}

		// display_str 변수에 저장되어 있는 내용을 IDC_MY_TEXT 에디트 컨트롤에 출력한다.
		SetDlgItemText(IDC_EDIT1, display_str);

		// 작업을 마친후에 파일을 닫는다.
		tip_file.Close();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void OCTIntroBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_txtScroll.LineScroll(m_txtScroll.GetLineCount(), 0);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
