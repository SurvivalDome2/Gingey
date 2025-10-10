#include <core/init.h>

HWND init(HINSTANCE hInstance, LPCSTR windowName)
{
    SetCurrentDirectoryW(L".\\bin/game");

    loggerInit();

    LPCSTR className = "main";

    enum WindowStylePresets wStyle = WINDOWED;
    enum ClassStylePresets cStyle = BASE;

    MathVector position  = {0};
    MathVector dimension = {0};

    dimension.x = 100;
    dimension.y = 100;

    registerClass(hInstance, className, cStyle, WindowProc);
    HWND hWnd = createWindow(hInstance, className, windowName, wStyle, position, dimension, NULL);

    return hWnd;
}