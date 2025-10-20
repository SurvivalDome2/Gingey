#include <core/gr_utils.h>

ErrorCode gr_init(HINSTANCE hInstance, LPCSTR gameName, Window* window, WNDPROC WindowProc)
{
    ErrorCode errorCode = {0};

    SetCurrentDirectoryW(L".\\bin/game");

    errorCode = loggerInit();
    if(errorCode.mainError != 0)
    {
        return errorCode;
    }

    enum WindowStylePresets wStyle = WINDOWED;
    enum ClassStylePresets cStyle = BASE;

    MathVector position  = {0};
    MathVector dimension = {0};

    dimension.x = 100;
    dimension.y = 100;

    registerClass(hInstance, "main", cStyle, WindowProc);
    window->hWnd = createWindow(hInstance, "main", gameName, wStyle, position, dimension, NULL);
    window->hInstance = hInstance;
    window->name = gameName;

    VkInstance vkInstance = {0};

    errorCode = initVulkan("gameName", &vkInstance);

    return errorCode;
}

void gr_shutdown(VkInstance vkInstance)
{
    if(vkInstance == NULL)
    {
        logEnqueue(LOG_LEVEL_WARNING, "vkInstance passed into shutdown was NULL", systemTime, __LINE__, __FILE__);
    }
    else
    {
        vkDestroyInstance(vkInstance, NULL);
    }

    // Might be an issue, but can't be bothered to test it
    loggerShutdown();
}

const char* concatString(const char* baseString, const char* string)
{
    size_t totalSize = strlen(baseString) + strlen(string) + 1;

    const char* output = malloc(totalSize);

    strcpy_s(output, totalSize, baseString);
    strcat_s(output, totalSize, string);
    
    return output;
}

// This might just be the ugliest code I've ever written
char* outputError(ErrorCode errorCode)
{
    char* message;
    char* baseMessage;

    switch(errorCode.mainError)
    {
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            baseMessage = malloc(sizeof("Could not find one or more Vulkan extensions") + 1);
            baseMessage = "Could not find one or more Vulkan extensions";

            logEnqueue(LOG_LEVEL_FATAL, "Could not find one or more Vulkan extensions", systemTime, __LINE__, __FILE__);

            return baseMessage;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            baseMessage = malloc(sizeof("The current driver is incompatible with this engine") + 1);
            baseMessage = "The current driver is incompatible with this engine";

            logEnqueue(LOG_LEVEL_FATAL, "The current driver is incompatible with this engine", systemTime, __LINE__, __FILE__);
        case VK_ERROR_INITIALIZATION_FAILED:
            baseMessage = malloc(sizeof("Vulkan instance initialization failed") + 1);
            baseMessage = "Vulkan instance initialization failed";

            logEnqueue(LOG_LEVEL_FATAL, "Vulkan instance initialization failed", systemTime, __LINE__, __FILE__);
        case VK_ERROR_LAYER_NOT_PRESENT:
            baseMessage = malloc(sizeof("Could not find any Vulkan layers") + 1);
            baseMessage = "Could not find any Vulkan layers";

            logEnqueue(LOG_LEVEL_FATAL, "Could not find any Vulkan layers", systemTime, __LINE__, __FILE__);
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            baseMessage = malloc(sizeof("Device out of memory") + 1);
            baseMessage = "Device out of memory";

            logEnqueue(LOG_LEVEL_FATAL, "Device out of memory", systemTime, __LINE__, __FILE__);
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            baseMessage = malloc(sizeof("Out of host memory") + 1);
            baseMessage = "Out of host memory";

            logEnqueue(LOG_LEVEL_FATAL, "Out of host memory", systemTime, __LINE__, __FILE__);
        case 25601:
            baseMessage = malloc(sizeof("Failed to initialize the logging thread due to invalid params:\n") + 1);
            baseMessage = "Failed to initialize the logging thread due to invalid params:\n";
            
            switch(errorCode.errorDetail)
            {
                case 1:
                    message = concatString(baseMessage, "startAddress param is 0 (invalid)");
                    
                    return message;
                case 2:
                    message = concatString(baseMessage, "criticalSection param is a NULL ptr (invalid)");
                    
                    return message;
                case 3:
                    message = concatString(baseMessage, "hThread param is a NULL ptr (invalid)");
                    
                    return message;
                default:
                    message = concatString(baseMessage, "Unknown error");
                    
                    return message;
            }
        case 25602:
            baseMessage = malloc(sizeof("Failed to initialize critical section") + 1);
            baseMessage = "Failed to initialize critical section";
                    
            return baseMessage;
        case 25603:
            baseMessage = malloc(sizeof("Failed to create thread") + 1);
            baseMessage = "Failed to create thread";

            return baseMessage;
        case 25604:
            baseMessage = malloc(sizeof("Failed to create Vulkan infoInstance due to invalid params:\n") + 1);
            baseMessage = "Failed to create Vulkan infoInstance due to invalid params:\n";

            switch(errorCode.errorDetail)
            {
                case 1:
                    message = concatString(baseMessage, "name param is a NULL ptr (invalid)");

                    return message;
                case 2:
                    message = concatString(baseMessage, "createInfo param is a NULL ptr (invalid)");

                    return message;
            }
        case 25605:
            baseMessage = malloc(sizeof("Failed to allocate memory in createInfoInstance:\n") + 1);
            baseMessage = "Failed to allocate memory in createInfoInstance:\n";

            return baseMessage;
        default:
            baseMessage = malloc(sizeof("Unknown error") + 1);
            baseMessage = "Unknown error";

            return baseMessage;
    }
}