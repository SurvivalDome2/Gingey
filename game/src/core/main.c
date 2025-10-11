#include <core/main.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    Window window = tg_init(hInstance, "gameName");
    VkInstance vkInstance = {0};

    {
        VkResult result = initVulkan("gameName", &vkInstance);
        if(result != VK_SUCCESS)
        {
            tg_shutdown(vkInstance);
            PostQuitMessage(result);
        }
    }

    EnableWindow(window.hWnd, TRUE);

    MSG msg = {0};
    while (GetMessage(&msg, window.hWnd, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
 
    tg_shutdown(vkInstance);

    return 0;
}