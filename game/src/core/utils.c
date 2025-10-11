#include <core/utils.h>

Window tg_init(HINSTANCE hInstance, LPCSTR gameName)
{
    SetCurrentDirectoryW(L".\\bin/game");

    loggerInit();

    Window window = {0};

    enum WindowStylePresets wStyle = WINDOWED;
    enum ClassStylePresets cStyle = BASE;

    MathVector position  = {0};
    MathVector dimension = {0};

    dimension.x = 100;
    dimension.y = 100;

    registerClass(hInstance, "main", cStyle, WindowProc);
    window.hWnd = createWindow(hInstance, "main", gameName, wStyle, position, dimension, NULL);
    window.hInstance = hInstance;
    window.name = gameName;

    return window;
}

void tg_shutdown(VkInstance vkInstance)
{
    loggerShutdown();

    vkDestroyInstance(vkInstance, NULL);
}