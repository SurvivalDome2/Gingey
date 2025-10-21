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
        size_t bufferSize = sizeof(errorCode.mainError) + sizeof("Error code: ") + 1;
        char* buffer = malloc(bufferSize);

        snprintf(buffer, bufferSize, "Error code: %d", errorCode.mainError);

        MessageBeep(MB_ICONHAND);
        MessageBoxEx(window.hWnd, outputError(errorCode), buffer, MB_OK, 0);

        gr_shutdown(NULL);

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