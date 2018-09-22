#pragma once

class CStaticGNUPlot : public CStatic
{
public:
	CStaticGNUPlot();
	virtual ~CStaticGNUPlot();

public:
	void init(const char *gnuplotName);
	void cmd(const char *format, ...);

private:
	friend BOOL CALLBACK cbGetTextWindow(HWND  hwnd, LPARAM  lParam);
	friend BOOL CALLBACK cbGetGraphWindow(HWND  hwnd, LPARAM  lParam);

private:
	HWND		_hWndParent;
	HWND		_hWndText;
	HWND		_hWndGraph;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
