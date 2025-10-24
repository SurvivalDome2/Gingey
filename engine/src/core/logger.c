#include <core/logger.h>

static LogQueue logQueue;

SYSTEMTIME systemTime;

uintptr_t hThread;

ErrorCode loggerInit(void)
{
    ErrorCode errorCode = threadInit(loggingThreadProcessor, &logQueue.semaphore, MAX_LOG_QUEUE, &logQueue.criticalSection, &hThread);

    if(errorCode.mainError != 0)
    {
        return errorCode;
    }

    atomic_store(&logQueue.running, TRUE);
    logQueue.oldestMessageIndex = 0;
    logQueue.newestMessageIndex = 0;
    logQueue.totalQueuedMessages = 0;

    return errorCode;
}

unsigned int __stdcall loggingThreadProcessor(void* arg)
{
    (void)arg;

    HANDLE hLogFile = CreateFileW(L"log.txt", FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hLogFile == INVALID_HANDLE_VALUE)
    {
        SYSTEMTIME time;
        GetLocalTime(&time);
        char formattedMessage[1024];
        snprintf(
            formattedMessage, 
            sizeof(formattedMessage),
            "[%02d:%02d:%02d.%03d] FATAL: Failed to create log file",
            time.wHour, time.wMinute, time.wSecond, time.wMilliseconds
        );

        EnterCriticalSection(&logQueue.criticalSection);
        atomic_store(&logQueue.running, FALSE);
        LeaveCriticalSection(&logQueue.criticalSection);

        threadShutdown(&hThread, &logQueue.semaphore, &logQueue.criticalSection);

        return 1;
    }

    while(atomic_load(&logQueue.running))
    {
        WaitForSingleObject(logQueue.semaphore, INFINITE);

        LogEntry logEntry;
        bool hasEntry = FALSE;

        EnterCriticalSection(&logQueue.criticalSection);
        
        if(logQueue.totalQueuedMessages > 0)
        {
            logEntry = logQueue.logEntries[logQueue.oldestMessageIndex];
            logQueue.oldestMessageIndex = (logQueue.oldestMessageIndex + 1) % MAX_LOG_QUEUE;
            logQueue.totalQueuedMessages--;
            hasEntry = TRUE;
        }
        
        LeaveCriticalSection(&logQueue.criticalSection);

        if(hasEntry)
        {
            char* logLevelString;

            switch(logEntry.logLevel)
            {
                case 0:
                    logLevelString = malloc(sizeof("TRACE"));
                    logLevelString = "TRACE";
                    break;
                case 1:
                    logLevelString = malloc(sizeof("INFO"));
                    logLevelString = "INFO";
                    break;
                case 2:
                    logLevelString = malloc(sizeof("WARNING"));
                    logLevelString = "WARNING";
                    break;
                case 3:
                    logLevelString = malloc(sizeof("ERROR"));
                    logLevelString = "ERROR";
                    break;
                case 4:
                    logLevelString = malloc(sizeof("FATAL"));
                    logLevelString = "FATAL";
                    break;
            }

            SYSTEMTIME time;
            GetLocalTime(&time);
            char formattedMessage[2048];
            int len = snprintf(
                formattedMessage, 
                sizeof(formattedMessage),
                "[%02d:%02d:%02d.%03d] %s: %s\n",
                time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
                logLevelString,
                logEntry.message
            );

            DWORD bytesWritten;

            if(!WriteFile(hLogFile, formattedMessage, len, &bytesWritten, NULL))
            {
                DWORD err = GetLastError();
                char message[256];
                snprintf(message, sizeof(message), "WriteFile failed: %lu", err);
            }
        }
    }

    return 0;
}

void logEnqueue(LogLevel logLevel, const char* message, SYSTEMTIME time, int line, const char* file)
{
    EnterCriticalSection(&logQueue.criticalSection);
    
    if(!atomic_load(&logQueue.running))
    {
        return;
    }

    if(logQueue.totalQueuedMessages == MAX_LOG_QUEUE)
    {
        logQueue.oldestMessageIndex = (logQueue.oldestMessageIndex + 1) % MAX_LOG_QUEUE;
        logQueue.totalQueuedMessages--;
    }

    logQueue.logEntries[logQueue.newestMessageIndex].logLevel = logLevel;

    strncpy_s(logQueue.logEntries[logQueue.newestMessageIndex].message, MESSAGE_CHAR_COUNT - 1, message, _TRUNCATE);

    logQueue.logEntries[logQueue.newestMessageIndex].timeOfMessage = time;

    logQueue.logEntries[logQueue.newestMessageIndex].line = line;

    strncpy_s(logQueue.logEntries[logQueue.newestMessageIndex].file, 255, file, _TRUNCATE);

    logQueue.newestMessageIndex = (logQueue.newestMessageIndex + 1) % MAX_LOG_QUEUE;
    logQueue.totalQueuedMessages++;

    bool shouldReleaseSemaphore = (logQueue.totalQueuedMessages < MAX_LOG_QUEUE);

    LeaveCriticalSection(&logQueue.criticalSection);
    
    if(shouldReleaseSemaphore)
    {
        ReleaseSemaphore(logQueue.semaphore, 1, NULL);
    }
}

void loggerShutdown(void)
{
    EnterCriticalSection(&logQueue.criticalSection);
    atomic_store(&logQueue.running, FALSE);
    LeaveCriticalSection(&logQueue.criticalSection);

    int remaining;
    do
    {
        EnterCriticalSection(&logQueue.criticalSection);
        remaining = logQueue.totalQueuedMessages;
        LeaveCriticalSection(&logQueue.criticalSection);
        if (remaining > 0) Sleep(10);
    }
    while (remaining > 0);

    threadShutdown(&hThread, &logQueue.semaphore, &logQueue.criticalSection);
}