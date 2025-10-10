#include <core/main.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    HWND hWnd = init(hInstance, "windowName");

    EnableWindow(hWnd, true);

    MSG msg = {0};
    while (GetMessage(&msg, hWnd, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
 
    loggerShutdown();

    return 0;
}