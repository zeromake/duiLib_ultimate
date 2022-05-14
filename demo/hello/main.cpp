#include <windows.h>
#include <memory>
#include <tchar.h>
#include <uilib.h>

using namespace DuiLib;

class CFrameWnd : public WindowImplBase {
public:
	CFrameWnd(){};
	LPCTSTR GetWindowClassName() const {
        return _T("Hello");
    };
	CDuiString GetSkinFile() {
        return _T("main.xml");
    };
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
        if (uMsg == WM_CLOSE) {
            PostQuitMessage(0);
        } else if (uMsg == WM_DPICHANGED) {
            OnDPIChanged(uMsg, wParam, lParam, bHandled);
        }
        bHandled = false;
        return 0;
    };
	LRESULT OnDPIChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
        bHandled = false;
        return 0;
    };
	void Notify(TNotifyUI& msg){
        WindowImplBase::Notify(msg);
    };
};

int uimain(HINSTANCE hInstance, LPCTSTR argv[], int argc) {
    ::CoInitialize(NULL);
    CPaintManagerUI::InitProcessDPIAware();
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..\\..\\..\\..\\skin\\hello"));
    std::unique_ptr<CFrameWnd> pFrame(new CFrameWnd());
    pFrame->Create(NULL, _T("Hello Demo Window1"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->ShowWindow();
    pFrame->CenterWindow();
    CPaintManagerUI::MessageLoop();
    ::CoUninitialize();
    return 0;
}

#ifdef _CONSOLE
#ifdef UNICODE

int wmain(LPCTSTR argv[], int argc) {
    _wsetlocale(0, L".65001");
#else
int main(LPCTSTR argv[], int argc) {
#endif
    auto hInstance = GetModuleHandle(NULL);
    return uimain(hInstance, argv, argc);
}
#else
int APIENTRY _tWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow) {
    int argc = 0;
    CDuiString s = lpCmdLine;
    auto args = StrSplit(s, _T(" "));
    int argc = args.size();
    LPCTSTR *argv = new LPCTSTR[argc];
    for (int i = 0; i < argc; i++) {
        argv[i] = args.at(i).GetData();
    }
    return uimain(hInstance, argv, argc);
}
#endif
