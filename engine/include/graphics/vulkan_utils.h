#pragma once

#include <stdlib.h>

#include <core/logger.h>

#include <vulkan/vulkan.h>

void createInfoInstance(const char* name, VkInstanceCreateInfo* createInfo);

void createInstance(void);

ErrorCode initVulkan(const char* gameName, VkInstance* instance);