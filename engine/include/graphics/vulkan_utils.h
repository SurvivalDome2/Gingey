#pragma once

#include <stdlib.h>
#include <assert.h>

#include <vulkan/vulkan.h>

#include <core/logger.h>
#include <core/gr_utils.h>

ErrorCode createInfoInstance(const char* name, VkInstanceCreateInfo* createInfo);

ErrorCode pickPhysicalDevice(VkInstance instance);

ErrorCode initVulkan(const char* gameName, VkInstance* instance);