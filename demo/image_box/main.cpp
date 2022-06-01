#include <windows.h>
#include <memory>
#include <tchar.h>
#include <uilib.h>


using namespace DuiLib;

template <class T = CUIFrameWnd>
class IApp : public CUIApplication
{
public:
#ifdef __MINGW32__
    using __super = CUIApplication;
#endif
	IApp(){};
	~IApp(void){};

	bool InitInstance(HINSTANCE hInstance) {
        if(!__super::InitInstance(hInstance))
            return false;
        //建立主窗口
        m_pMainWnd = new T();
        if( m_pMainWnd == NULL ) return false;
        CPaintManagerUI::SetResourcePath(_T("D:\\project\\DuiLib\\skin\\image_box\\"));
        m_pMainWnd->Create(NULL, GetAppName(), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
        m_pMainWnd->CenterWindow();
        return true;
    };
};

class CMainFrame : public CUIFrameWnd
{
public:
    CMainFrame(void) {};
    ~CMainFrame(void) {};
    LPCTSTR GetWindowClassName() const  { return _T("MainFrame"); }
    CDuiString GetSkinFile() 			{ return _T("main.xml"); }
    void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

    void InitWindow() {};
    bool OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam ) {
        return false;
    };
    bool OnMenuCommand(const MenuCmd *cmd) {
        return false;
    };
    bool OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI) {
        return false;
    };
    void OnNotifyClick(TNotifyUI& msg) {
    };
};

int uimain(HINSTANCE hInstance, TCHAR* argv[], int argc) {
    auto uiApp = IApp<CMainFrame>();
    uiApp.SetAppName(_T("Test"));

	//设置程序为单一执行实例，允许多实例的话，就不需要这行了。
	uiApp.SetSingleApplication(true, _T("CBD4DD1F-3B18-4DA4-A043-0EA9AEC8D570"));
	//设置xml资源载入方式
	// uiApp.SetResType_File(_T(""));
	//初始化duilib, 注册插件，创建主窗口
	if(!uiApp.InitInstance(hInstance))
		return 0;
	//进入消息循环
	uiApp.Run();
	//清理资源。
	uiApp.ExitInstance();
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
    auto args = CDuiStringArray();
    args.SplitString(lpCmdLine, _T(" "));
    int argc = args.GetSize();
    TCHAR** argv = new TCHAR*[argc];
    for (int i = 0; i < argc; i++) {
        argv[i] = (TCHAR*)args.GetAt(i).GetData();
    }
    return uimain(hInstance, argv, argc);
}
#endif
