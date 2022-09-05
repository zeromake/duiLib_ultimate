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

    void __InitWindow(){}

    void Notify(TNotifyUI& msg){
        // if (msg.sType == DUI_MSGTYPE_MENU) {
        //     POINT point = msg.ptMouse;
        //     ClientToScreen(m_hWnd, &point);
        //     m_pMenu = CMenuWnd::CreateMenu(NULL, _T("menu.xml"), point, &m_pm, &m_MenuInfos);
        //     m_pMenu->ResizeMenu();
        // }
        WindowImplBase::Notify(msg);
    };
 private:
    LPCTSTR m_skinFile;
    CMenuWnd* m_pMenu;
    CStdStringPtrMap m_MenuInfos;
};

int uimain(HINSTANCE hInstance, TCHAR* argv[], int argc) {
    ::CoInitialize(NULL);
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(_T("D:\\project\\DuiLib\\skin\\hello\\"));//CPaintManagerUI::GetInstancePath() + _T("D:\\project\\DuiLib\\skin\\hello\\"));
    std::unique_ptr<CFrameWnd> pFrame(new CFrameWnd(argv[1]));
    pFrame->Create(NULL, _T("Hello Demo Window1"), UI_WNDSTYLE_FRAME, UI_WNDSTYLE_EX_FRAME);
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
    std::vector<_tstring> args;
    splitString<_tstring>((TCHAR*)lpCmdLine, args, _T(" "));
    argc = args.size();
    TCHAR** argv = new TCHAR*[argc+1];
    for (int i = 0; i < argc; i++) {
        argv[i+1] = (TCHAR*)args.at(i).c_str();
    }
    return uimain(hInstance, argv, argc);
}
#endif
