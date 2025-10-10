#include <core/window.h>

void registerClass(HINSTANCE hInstance, LPCSTR className, ul style, WNDPROC windowProc)
{
    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className;
    wc.hIconSm = NULL;

    wc.style = style;

    RegisterClassEx(&wc);
}

HWND createWindow(
    HINSTANCE hInstance,
    LPCSTR className,
    LPCSTR windowName,
    ul style,
    MathVector position,
    MathVector dimension,
    HWND hParent
)
{
    HWND hWnd = CreateWindowEx(
        0,
        className,
        windowName,
        style,
        position.x,
        position.y,
        dimension.x,
        dimension.y,
        hParent,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hWnd, SW_SHOW);

    return hWnd;
}