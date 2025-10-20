#pragma once

#include <stdlib.h>

#include <vulkan/vulkan.h>

#include <core/logger.h>
#include <core/gr_utils.h>

ErrorCode createInfoInstance(const char* name, VkInstanceCreateInfo* createInfo);

void createInstance(void);

ErrorCode initVulkan(const char* gameName, VkInstance* instance);