#pragma once

#include <windows.h>

#include <defines.h>

enum WindowStylePresets {
    WEMPTY = 0,
    WINDOWED  = WS_BORDER | WS_CAPTION | WS_DISABLED | WS_MAXIMIZE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU,
    BORDERLESS_WINDOWED  = WS_DISABLED | WS_MINIMIZEBOX | WS_SYSMENU,
    FULLSCREEN  = WS_CAPTION | WS_DISABLED | WS_MINIMIZEBOX | WS_SYSMENU,
    IN_GAME_MENU = WS_CHILD
};

enum ClassStylePresets {
    CEMPTY = 0,
    BASE = CS_OWNDC
};

typedef struct {
    HINSTANCE hInstance;
    HWND hWnd;
    const char* name;
} Window;

void registerClass(HINSTANCE hInstance, LPCSTR className, ul style, WNDPROC windowProc);

HWND createWindow(
    HINSTANCE hInstance,
    LPCSTR className,
    LPCSTR windowName,
    ul style,
    MathVector position,
    MathVector dimension,
    HWND hParent
);