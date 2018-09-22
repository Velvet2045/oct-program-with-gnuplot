// OCTIntroBox.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "OCT Program With GNUPlot 2.h"
#include "OCTIntroBox.h"
#include "afxdialogex.h"


// OCTIntroBox ��ȭ �����Դϴ�.

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


// OCTIntroBox �޽��� ó�����Դϴ�.


BOOL OCTIntroBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CStdioFile tip_file;
	// ���� ��ο��� "ReadMe.txt" ������ ����.
	// CFile::modeRead  ->  ������ �б� ���� ����. ( ������ ���ٸ� �����Ѵ�. )
	// CFile::typeText  ->  ������ �Ӽ��� �ؽ�Ʈ �������� �����Ѵ�. ( ���� ����)
	if (tip_file.Open("ReadMe.txt", CFile::modeRead | CFile::typeText)) {

		CString display_str, str;

		// ���Ͽ� ���̻� �ؽ�Ʈ ������ ���������� ���پ� �ݺ��ؼ� �о str ������ �����Ѵ�.
		while (tip_file.ReadString(str)) {

			// �о���� ������ �ؽ�Ʈ�� display_str ������ �߰��Ѵ�.
			display_str += str;

			// ��Ƽ���� �Ӽ��� ���� ����Ʈ ��Ʈ���� ���� ���� "\r\n" �� �־���� �ٹٲ��� ���ش�.
			// ReadString �Լ��� ������ �ؽ�Ʈ�� ������ �ش����� �����ִ� "\n" �� �о ������
			// ������ �ؽ�Ʈ�� ���ٳ��� "\n" �� �ִٰ� ������ �ݵ�� "\n" �� �ٿ��� �մϴ�.
			display_str += "\r\n";
		}

		// display_str ������ ����Ǿ� �ִ� ������ IDC_MY_TEXT ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
		SetDlgItemText(IDC_EDIT1, display_str);

		// �۾��� ��ģ�Ŀ� ������ �ݴ´�.
		tip_file.Close();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void OCTIntroBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_txtScroll.LineScroll(m_txtScroll.GetLineCount(), 0);
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
