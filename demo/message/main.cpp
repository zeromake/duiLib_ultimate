#include <windows.h>
#include <tchar.h>
#include <memory>


#ifdef UNICODE
int wmain() {
    _wsetlocale(0, L".65001");
#else
int main() {
#endif
    auto hInstance = GetModuleHandle(NULL);
    return _tWinMain(hInstance, NULL, GetCommandLine(), SW_SHOWNORMAL);
}
int APIENTRY _tWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
int nCmdShow) {
    printf("\n");
    return 0;
}
