#pragma once

#include <defines.h>

#include <core/ini.h>
#include <core/logger.h>
#include <core/multi_thread.h>
#include <core/window.h>

#include <graphics/vulkan_utils.h>

#include <core/windowProcs.h>

Window tg_init(HINSTANCE hInstance, LPCSTR gameName);

void tg_shutdown(VkInstance vkInstance);