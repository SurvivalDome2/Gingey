#pragma once

#include <windows.h>

#include <defines.h>

enum StylePresets {
    EMPTY = 0,
    WINDOWED  = WS_BORDER | WS_CAPTION | WS_DISABLED | WS_MAXIMIZE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
    BORDERLESS_WINDOWED  = WS_DISABLED | WS_MINIMIZEBOX | WS_SYSMENU,
    FULLSCREEN  = WS_CAPTION | WS_DISABLED | WS_MINIMIZEBOX | WS_SYSMENU,
    IN_GAME_MENU = WS_CHILD
};

HWND createWindow(
    HINSTANCE hInstance,
    LPCSTR className,
    LPCSTR windowName,
    ul style,
    MathVector position,
    MathVector dimension,
    HWND hParent
);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);