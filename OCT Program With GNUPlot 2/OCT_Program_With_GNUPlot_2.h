
// OCT Program With GNUPlot 2.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// COCTProgramWithGNUPlot2App:
// �� Ŭ������ ������ ���ؼ��� OCT Program With GNUPlot 2.cpp�� �����Ͻʽÿ�.
//

class COCTProgramWithGNUPlot2App : public CWinApp
{
public:
	COCTProgramWithGNUPlot2App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern COCTProgramWithGNUPlot2App theApp;
