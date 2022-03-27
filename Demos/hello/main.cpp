#include "stdafx.h"
#include "FrameWnd.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	::CoInitialize(NULL);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\hello"));
	CFrameWnd* pFrame = new CFrameWnd();
	pFrame->Create(NULL, _T("Hello Demo Window1"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowWindow();
	pFrame->CenterWindow();

	CPaintManagerUI::MessageLoop();

	delete pFrame;

	::CoUninitialize();
	return 0;
}
