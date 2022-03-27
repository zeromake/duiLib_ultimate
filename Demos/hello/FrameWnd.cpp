#include "stdafx.h"
#include "FrameWnd.h"


CFrameWnd::CFrameWnd()
{
}

LPCTSTR CFrameWnd::GetWindowClassName() const
{
	return _T("Hello");
}

CDuiString CFrameWnd::GetSkinFile()
{
	return _T("main.xml");
}


LRESULT CFrameWnd::OnDPIChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
	bHandled = false;
	return 0;
}


LRESULT CFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	if (uMsg == WM_CLOSE) {
		PostQuitMessage(0);
	} else if (uMsg == WM_DPICHANGED) {
		OnDPIChanged(uMsg, wParam, lParam, bHandled);
	}
	bHandled = false;
	return 0;
}


void CFrameWnd::Notify(TNotifyUI& msg) {
	__super::Notify(msg);
}
