#pragma once

#include "stdafx.h"

class CFrameWnd : public WindowImplBase {
public:
	explicit CFrameWnd();
	LPCTSTR GetWindowClassName() const;
	CDuiString GetSkinFile();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDPIChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Notify(TNotifyUI& msg);
};
