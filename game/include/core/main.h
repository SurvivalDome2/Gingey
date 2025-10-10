#pragma once

#include <core/logger.h>
#include <core/window.h>

#include <core/init.h>
#include <core/windowProcs.h>

SYSTEMTIME systemTime;

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
);