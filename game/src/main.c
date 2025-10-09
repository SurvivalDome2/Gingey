#include <stdio.h>

#include <main.h>

#include <core/logger.h>
#include <core/window.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
)
{
    SetCurrentDirectoryW(L".\\bin/game");

    loggerInit();

    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    enum StylePresets style = WINDOWED;

    MathVector position  = {0};
    MathVector dimension = {0};

    dimension.x = 100;
    dimension.y = 100;

    LPCSTR className  = "className";
    LPCSTR windowName = "windowName";

    HWND hWnd = createWindow(hInstance, className, windowName, style, position, dimension, NULL);

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