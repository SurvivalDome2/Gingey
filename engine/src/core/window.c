#include <core/window.h>

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
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className;
    wc.hIconSm = NULL;

    wc.style = CS_OWNDC;

    RegisterClassEx(&wc);

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

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            SetWindowText(hWnd, "Key press detected");
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}