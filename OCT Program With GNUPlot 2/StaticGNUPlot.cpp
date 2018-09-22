#include "stdafx.h"
#include "windows.h"
#include "StaticGNUPlot.h"

#pragma warning (disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PARENTCLASS _T("wgnuplot_parent")
#define TEXTCLASS   _T("wgnuplot_text")
#define GRAPHCLASS  _T("wgnuplot_graph")

CStaticGNUPlot::CStaticGNUPlot()
{
	_hWndParent = NULL;
	_hWndText = NULL;
	_hWndGraph = NULL;
}

CStaticGNUPlot::~CStaticGNUPlot()
{
	if (_hWndGraph)	::SendMessage(_hWndGraph, WM_CLOSE, 0, 0);
	if (_hWndText)	::SendMessage(_hWndText, WM_CLOSE, 0, 0);
	if (_hWndParent) ::SendMessage(_hWndParent, WM_CLOSE, 0, 0);
}

BEGIN_MESSAGE_MAP(CStaticGNUPlot, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CALLBACK cbGetTextWindow(HWND hWnd, LPARAM  lParam)
{
	CStaticGNUPlot *pThis = (CStaticGNUPlot *)lParam;

	HWND hWndFinded = FindWindowEx(hWnd, NULL, TEXTCLASS, NULL);
	if (hWnd && hWndFinded) {
		pThis->_hWndParent = hWnd;
		pThis->_hWndText = hWndFinded;
	}
	return TRUE;
}

BOOL CALLBACK cbGetGraphWindow(HWND hWnd, LPARAM  lParam)
{
	CStaticGNUPlot *pThis = (CStaticGNUPlot *)lParam;

	HWND hWndFinded = FindWindowEx(NULL, hWnd, GRAPHCLASS, NULL);
	if (hWndFinded) {
		pThis->_hWndGraph = hWndFinded;
	}
	return TRUE;
}

void CStaticGNUPlot::init(const char *gnuplotName)
{
	if (_hWndGraph)	::SendMessage(_hWndGraph, WM_CLOSE, 0, 0);
	if (_hWndText)	::SendMessage(_hWndText, WM_CLOSE, 0, 0);
	if (_hWndParent) ::SendMessage(_hWndParent, WM_CLOSE, 0, 0);

	_hWndParent = NULL;
	_hWndText = NULL;
	_hWndGraph = NULL;

	STARTUPINFO si = { 0, };
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOWMINIMIZED;
	si.dwFlags = STARTF_USESHOWWINDOW;

	wchar_t wGnuplotName[200];
	mbstowcs(wGnuplotName, gnuplotName, strlen(gnuplotName) + 1);
	LPWSTR ptr = wGnuplotName;

	PROCESS_INFORMATION pi = { 0, };
	BOOL bSuccess = CreateProcess(NULL, (char*)gnuplotName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!bSuccess) return;

	// Text Window가 생성될 때까지 기다린다.
	for (int i = 0; i<100 && !_hWndText; ++i) {
		EnumThreadWindows(pi.dwThreadId, cbGetTextWindow, (LPARAM)this);
		Sleep(10);
	}

	// 비어있는 Graph window 생성
	cmd("clear");

	// Graph Window가 생성될 때까지 기다린다.
	for (int i = 0; i<100 && !_hWndGraph; ++i) {
		EnumThreadWindows(pi.dwThreadId, cbGetGraphWindow, (LPARAM)this);
		Sleep(10);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (_hWndParent) {
		//Parent Window를 숨긴다.
		::ShowWindow(_hWndParent, SW_HIDE);
	}
	if (_hWndGraph) {
		//Graph Window가 Embedding 되도록 스타일을 변경하고 크기 조정
		::SetParent(_hWndGraph, this->m_hWnd);

		long lStyle = ::GetWindowLong(_hWndGraph, GWL_STYLE);
		lStyle &= ~(WS_BORDER | WS_CAPTION | WS_SIZEBOX);
		::SetWindowLong(_hWndGraph, GWL_STYLE, lStyle);

		RECT rc;
		GetClientRect(&rc);

		::MoveWindow(_hWndGraph, rc.left, rc.top, rc.right, rc.bottom, TRUE);
		::InvalidateRect(_hWndGraph, NULL, TRUE);
	}
}

void CStaticGNUPlot::cmd(const char *format, ...)
{
	va_list ap;
	char cmd[1024];

	va_start(ap, format);
	vsprintf(cmd, format, ap);
	va_end(ap);

	if (_hWndText) {
		for (char *p = cmd; *p; ++p) {
			::PostMessage(_hWndText, WM_CHAR, *p, 1);
		}
		::PostMessage(_hWndText, WM_CHAR, '\n', 1);
	}
}

void CStaticGNUPlot::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CStatic::OnPaint() for painting messages

	if (_hWndGraph) {
		::InvalidateRect(_hWndGraph, NULL, TRUE);
	}
}
