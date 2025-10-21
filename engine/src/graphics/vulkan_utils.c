#include <graphics/vulkan_utils.h>

int createInfoInstance(const char* name, VkInstanceCreateInfo* createInfo)
{
    if (!name || !createInfo)
    {
        logEnqueue(LOG_LEVEL_ERROR, "Invalid parameters", systemTime, __LINE__, __FILE__);
        return 0;
    }

    static VkApplicationInfo appInfo = {0};

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

    return 0;
}

void createInstance(void)
{
    
}

ErrorCode initVulkan(const char* gameName, VkInstance* instance)
{
    ErrorCode errorCode = {0};

    VkInstanceCreateInfo createInfo = {0};
    createInfoInstance(gameName, &createInfo);

    if(errorCode.mainError != 0)
    {
        return errorCode;
    }

    VkResult result = vkCreateInstance(&createInfo, NULL, instance);

    errorCode.mainError = result;
    errorCode.errorDetail = -1;

    return errorCode;
}