#include <core/main.h>

int main(void)
{
    loggerInit();

    logEnqueue(LOG_LEVEL_TRACE, "Test for LOG_LEVEL_TRACE", systemTime, __LINE__, __FILE__);
    logEnqueue(LOG_LEVEL_INFO, "Test for LOG_LEVEL_INFO", systemTime, __LINE__, __FILE__);
    logEnqueue(LOG_LEVEL_WARNING, "Test for LOG_LEVEL_WARNING", systemTime, __LINE__, __FILE__);
    logEnqueue(LOG_LEVEL_ERROR, "Test for LOG_LEVEL_ERROR", systemTime, __LINE__, __FILE__);
    logEnqueue(LOG_LEVEL_FATAL, "Test for LOG_LEVEL_FATAL", systemTime, __LINE__, __FILE__);

    Sleep(100);

    loggerShutdown();

    return 0;
}