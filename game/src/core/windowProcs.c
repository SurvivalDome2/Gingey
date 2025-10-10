#include <core/windowProcs.h>

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