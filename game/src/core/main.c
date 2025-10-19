#include <core/main.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
)
{
    ErrorCode errorCode = {0};

    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    Window window = {0};
    
    errorCode = gr_init(hInstance, "gameName", &window, WindowProc);
    if(errorCode.mainError != 0)
    {
        MessageBeep(MB_ICONHAND);
        MessageBoxEx(window.hWnd, outputError(errorCode), "Error", MB_OK, 0);
        ExitProcess(errorCode.mainError);
    }

    EnableWindow(window.hWnd, TRUE);

    MSG msg = {0};
    while (GetMessage(&msg, window.hWnd, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}