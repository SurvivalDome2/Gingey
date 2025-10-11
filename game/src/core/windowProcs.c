#include <stdio.h>

#include <core/windowProcs.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = {0};
    char* mousePos = (char*)malloc(16 * sizeof(char));

    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_MOUSEMOVE:
            pt = MAKEPOINTS(lParam);
            snprintf(mousePos, 16 * sizeof(char), "x: %hd, y: %hd", pt.x, pt.y);
            SetWindowText(hWnd, mousePos);
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}