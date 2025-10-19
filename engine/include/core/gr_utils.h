#pragma once

#include <string.h>

#include <windows.h>

#include <core/logger.h>
#include <core/window.h>

#include <graphics/vulkan_utils.h>

#include <defines.h>

ErrorCode gr_init(HINSTANCE hInstance, LPCSTR gameName, Window* window, WNDPROC WindowProc);

void gr_shutdown(VkInstance vkInstance);

const char* concatString(const char* baseString, const char* string);

char* outputError(ErrorCode errorCode);