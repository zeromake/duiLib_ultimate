#include <windows.h>
#include <memory>
#include <tchar.h>
#include <uilib.h>

using namespace DuiLib;

class CFrameWnd : public WindowImplBase {
public:
	CFrameWnd(LPCTSTR skinFile): m_skinFile(skinFile) {};
	LPCTSTR GetWindowClassName() const {
        return _T("Hello");
    };
	CDuiString GetSkinFile() {
        return m_skinFile;
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
 private:
    LPCTSTR m_skinFile;
};

int uimain(HINSTANCE hInstance, TCHAR* argv[], int argc) {
    ::CoInitialize(NULL);
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(_T("D:\\project\\DuiLib\\skin\\hello\\"));//CPaintManagerUI::GetInstancePath() + _T("D:\\project\\DuiLib\\skin\\hello\\"));
    std::unique_ptr<CFrameWnd> pFrame(new CFrameWnd(argv[1]));
    pFrame->Create(NULL, _T("Hello Demo Window1"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->ShowWindow();
    pFrame->CenterWindow();
    CPaintManagerUI::MessageLoop();
    ::CoUninitialize();
    return 0;
}

#ifdef _CONSOLE
#ifdef UNICODE

int wmain(int argc, TCHAR* argv[]) {
    _wsetlocale(0, L".65001");
#else
int main(int argc, TCHAR* argv[]) {
#endif
    printf("argc: %d\n", argc);
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
    auto args = StrSplit(lpCmdLine, _T(" "));
    int argc = args.size();
    TCHAR** argv = new TCHAR*[argc];
    for (int i = 0; i < argc; i++) {
        argv[i] = (TCHAR*)args.at(i);
    }
    return uimain(hInstance, argv, argc);
}
#endif
