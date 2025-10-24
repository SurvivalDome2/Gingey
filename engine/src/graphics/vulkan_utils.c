#include <graphics/vulkan_utils.h>

ErrorCode createInfoInstance(const char* name, VkInstanceCreateInfo* createInfo)
{
    ErrorCode errorCode = {0};

    if(name == NULL)
    {
        errorCode.mainError = 25604;
        errorCode.errorDetail = 1;

        logEnqueue(LOG_LEVEL_ERROR, "name param passed into createInfoInstance is NULL (invalid)", systemTime, __LINE__, __FILE__);
        
        return errorCode;
    }
    if(createInfo == NULL)
    {
        errorCode.mainError = 25604;
        errorCode.errorDetail = 2;

        logEnqueue(LOG_LEVEL_FATAL, "createInfo param passed into createInfoInstance is NULL (invalid)", systemTime, __LINE__, __FILE__);

        return errorCode;
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
        errorCode.mainError = 25605;

        logEnqueue(LOG_LEVEL_FATAL, "Failed to allocate memory for the Vulkan extensions list", systemTime, __LINE__, __FILE__);

        return errorCode;
    }
    extensions[0] = (char*)VK_KHR_SURFACE_EXTENSION_NAME;

    createInfo->enabledExtensionCount = 1;
    createInfo->ppEnabledExtensionNames = extensions;

    createInfo->enabledLayerCount = 0;

    return errorCode;
}

// Don't like this function name
b8 checkIfDeviceIsSuitable(VkPhysicalDevice device)
{
    (void)device;
    return true;
}

ErrorCode pickPhysicalDevice(VkInstance instance)
{
    ErrorCode errorCode = {0};

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    VkPhysicalDevice devices[deviceCount];

    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for(uchar i = 0; i < deviceCount; i++)
    {
        if(checkIfDeviceIsSuitable(devices[i]))
        {
            physicalDevice = devices[i];
            break;
        }
        if(i == deviceCount - 1)
        {
            errorCode.mainError = 25606;

            logEnqueue(LOG_LEVEL_FATAL, "Found no suitable GPUs for Vulkan", systemTime, __LINE__, __FILE__);

            return errorCode;
        }
    }

    return errorCode;

    (void)physicalDevice;
}

ErrorCode initVulkan(const char* gameName, VkInstance* instance)
{
    ErrorCode errorCode = {0};

    VkInstanceCreateInfo createInfo = {0};
    errorCode = createInfoInstance(gameName, &createInfo);
    
    if(errorCode.mainError != 0)
    {
        return errorCode;
    }

    VkResult result = vkCreateInstance(&createInfo, NULL, instance);

    errorCode.mainError = result;
    errorCode.errorDetail = -1;

    return errorCode;
}