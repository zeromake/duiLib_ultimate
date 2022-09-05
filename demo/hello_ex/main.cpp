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
        // _tprintf(_T("notify: %s -%s\n"), msg.sType.GetData(), msg.pSender->GetName().GetData());
        if (msg.sType == DUI_MSGTYPE_CLICK) {
            if (msg.pSender->GetName() == _T("window-min")) {
                SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
            } else if (msg.pSender->GetName() == _T("window-max")) {
                PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
            } else if (msg.pSender->GetName() == _T("window-restore")) {
                PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
            } else if (msg.pSender->GetName() == _T("window-close")) {
                Close(IDCANCEL);
            }
            _tprintf(_T("click: %s\n"), msg.pSender->GetName().GetData());
        } else if(msg.sType == DUI_MSGTYPE_WINDOWSIZE) {
            bool isWindowMax = ::IsZoomed(*this);
            if (isWindowMax != windowMax) {
                windowMax = isWindowMax;
                _tprintf(_T("window change: %d\n"), windowMax);
                if (windowMax) {
                    CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("window-max")));
                    if( pControl ) pControl->SetVisible(false);
                    pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("window-restore")));
                    if( pControl ) pControl->SetVisible(true);
                } else {
                    CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("window-max")));
                    if( pControl ) pControl->SetVisible(true);
                    pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("window-restore")));
                    if( pControl ) pControl->SetVisible(false);
                }
            }
        }
        WindowImplBase::Notify(msg);
    };
 private:
    LPCTSTR m_skinFile;
    CMenuWnd* m_pMenu;
    CStdStringPtrMap m_MenuInfos;
    bool windowMax;
};

#ifndef SKIN_DIR
#define SKIN_DIR ""
#endif

int uimain(HINSTANCE hInstance, TCHAR* argv[], int argc) {
    ::CoInitialize(NULL);
    CPaintManagerUI::SetInstance(hInstance);
    TCHAR *skinDir = _T(SKIN_DIR);
    _tprintf(_T("skin: %s\n"), skinDir);
    CPaintManagerUI::SetResourcePath(skinDir);//CPaintManagerUI::GetInstancePath() + _T("D:\\project\\DuiLib\\skin\\hello\\"));
    std::unique_ptr<CFrameWnd> pFrame(new CFrameWnd(argv[1]));
    pFrame->Create(NULL, _T("Hello Demo Window1"), UI_WNDSTYLE_FRAME, UI_WNDSTYLE_EX_FRAME);
    pFrame->ShowWindow();
    // pFrame->CenterWindow();
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
