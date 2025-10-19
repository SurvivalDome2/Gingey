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

    {
        VkResult result = initVulkan("gameName", &vkInstance);
        if(result != VK_SUCCESS)
        {
            gr_shutdown(vkInstance);
            ExitProcess(errorCode.mainError);
        }
    }

    return errorCode;
}

void gr_shutdown(VkInstance vkInstance)
{
    loggerShutdown();

    vkDestroyInstance(vkInstance, NULL);
}

const char* concatString(const char* baseString, const char* string)
{
    size_t totalSize = strlen(baseString) + strlen(string) + 1;

    const char* output = malloc(totalSize);

    strcpy_s(output, totalSize, baseString);
    strcat_s(output, totalSize, string);
    
    return output;
}

char* outputError(ErrorCode errorCode)
{
    char* message;

    switch (errorCode.mainError)
    {
        case 25601:
            char* baseMessage = "Failed to initialize the logging thread due to invalid params:\n";
            
            switch (errorCode.errorDetail)
            {
                case 1:
                    message = concatString(baseMessage, "startAddress param is 0 (invalid)");
                    
                    return message;
                case 2:
                    message = concatString(baseMessage, "criticalSection param is NULL (invalid)");
                    
                    return message;
                case 3:
                    message = concatString(baseMessage, "hThread param is NULL (invalid)");
                    
                    return message;
                default:
                    message = concatString(baseMessage, "Unknown error");
                    
                    return message;
            }
        case 25602:
            baseMessage = "Failed to initialize critical section";
                    
            return baseMessage;
        case 25603:
            baseMessage = "Failed to create thread";

            return baseMessage;
        default:
            baseMessage = "Unknown error";

            return baseMessage;
    }
    return "a";
}