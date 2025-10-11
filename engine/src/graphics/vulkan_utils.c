#include <graphics/vulkan_utils.h>

void createInfoInstance(const char* name, VkInstanceCreateInfo* createInfo)
{
    if (!name || !createInfo) {
        logEnqueue(LOG_LEVEL_ERROR, "Invalid parameters", systemTime, __LINE__, __FILE__);
        return;
    }

    VkApplicationInfo appInfo = {};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = name;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "Ginger";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    createInfo->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo->pApplicationInfo = &appInfo;

    // Cannot figure out how to free this without causing a crash
    const char** extensions = malloc(sizeof(char*));
    if(extensions == NULL)
    {
        logEnqueue(LOG_LEVEL_FATAL, "Malloc failed when creating Vulkan extensions list", systemTime, __LINE__, __FILE__);
    }
    extensions[0] = (char*)VK_KHR_SURFACE_EXTENSION_NAME;

    createInfo->enabledExtensionCount = 1;
    createInfo->ppEnabledExtensionNames = extensions;

    createInfo->enabledLayerCount = 0;

    return;
}

void createInstance(void)
{
    
}

VkResult initVulkan(const char* gameName, VkInstance* instance)
{
    VkInstanceCreateInfo createInfo = {0};
    createInfoInstance(gameName, &createInfo);

    VkResult result = vkCreateInstance(&createInfo, NULL, instance);

    switch(result)
    {
        case VK_SUCCESS:
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            logEnqueue(LOG_LEVEL_FATAL, "Could not find one or more Vulkan extensions.", systemTime, __LINE__, __FILE__);
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            logEnqueue(LOG_LEVEL_FATAL, "The current driver is incompatible with this engine.", systemTime, __LINE__, __FILE__);
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            logEnqueue(LOG_LEVEL_FATAL, "Vulkan instance initialization failed.", systemTime, __LINE__, __FILE__);
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            logEnqueue(LOG_LEVEL_FATAL, "Could not find any Vulkan layers.", systemTime, __LINE__, __FILE__);
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            logEnqueue(LOG_LEVEL_FATAL, "Device out of memory.", systemTime, __LINE__, __FILE__);
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            logEnqueue(LOG_LEVEL_FATAL, "Out of host memory.", systemTime, __LINE__, __FILE__);
            break;
        default:
            logEnqueue(LOG_LEVEL_FATAL, "Unknown error.", systemTime, __LINE__, __FILE__);
            break;
    }

    return result;
}