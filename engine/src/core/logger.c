#include <core/logger.h>

static LogQueue logQueue;

bool loggingThreadInit(void)
{
    InitializeCriticalSection(&logQueue.lock);
    logQueue.semaphore = CreateSemaphore(NULL, 0, MAX_LOG_QUEUE, NULL);
    logQueue.running = TRUE;
    logQueue.oldestMessageIndex = 0;
    logQueue.newestMessageIndex = 0;
    logQueue.totalQueuedMessages = 0;

    // HANDLE hThread = CreateThread(NULL, 0, loggingThreadProcessor, NULL, 0, NULL);

    return TRUE;
}

DWORD WIN32 loggingThreadProcessor(LPVOID lpParameter)
{
    (void)lpParameter; // To convince clang that the lpParameter var is being used

    HANDLE hLogFile = CreateFileA("basicLog", FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hLogFile == INVALID_HANDLE_VALUE)
    {
        OutputDebugStringA("[LOGGER] Failed to open log file\n");
        return 1;
    }

    while(logQueue.running == TRUE)
    {
        WaitForSingleObject(logQueue.semaphore, INFINITE);

        LogEntry logEntry;
        bool hasEntry = FALSE;

        EnterCriticalSection(&logQueue.lock);
        
        if(logQueue.totalQueuedMessages > 0)
        {
            logEntry = logQueue.logEntries[logQueue.oldestMessageIndex];
            logQueue.oldestMessageIndex = (logQueue.oldestMessageIndex + 1) % MAX_LOG_QUEUE;
            logQueue.totalQueuedMessages--;
            hasEntry = TRUE;
        }
        
        LeaveCriticalSection(&logQueue.lock);

        if(hasEntry)
        {
            SYSTEMTIME time;
            GetSystemTime(&time);
            char formattedMessage[1024];
            int len = snprintf(
                formattedMessage, 
                sizeof(formattedMessage),
                "[%02d:%02d:%02d.%03d] [%s] [%d] %s\n",
                time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
                logEntry.file,
                logEntry.line,
                logEntry.message
            );

            OutputDebugStringA(formattedMessage);

            printf("%s", formattedMessage);

            DWORD bytesWritten;
            WriteFile(
                hLogFile,
                formattedMessage,
                len,
                &bytesWritten,
                NULL
            );
        }
    }
    return 0;
}

void logEnqueue(LogLevel logLevel, const char* message, SYSTEMTIME time, int line, const char* file)
{
    EnterCriticalSection(&logQueue.lock);
    
    if(logQueue.totalQueuedMessages == MAX_LOG_QUEUE)
    {
        logQueue.oldestMessageIndex = (logQueue.oldestMessageIndex + 1) % MAX_LOG_QUEUE;
        logQueue.totalQueuedMessages--;
    }

    logQueue.logEntries[logQueue.newestMessageIndex].logLevel = logLevel;

    strncpy(logQueue.logEntries[logQueue.newestMessageIndex].message, message, MESSAGE_CHAR_COUNT - 1);
    logQueue.logEntries[logQueue.newestMessageIndex].message[MESSAGE_CHAR_COUNT - 1] = '\0';

    logQueue.logEntries[logQueue.newestMessageIndex].timeOfMessage = time;

    logQueue.logEntries[logQueue.newestMessageIndex].line = line;

    strncpy(logQueue.logEntries[logQueue.newestMessageIndex].file, file, 255);

    logQueue.newestMessageIndex = (logQueue.newestMessageIndex + 1) % MAX_LOG_QUEUE;
    logQueue.totalQueuedMessages++;

    LeaveCriticalSection(&logQueue.lock);
    ReleaseSemaphore(logQueue.semaphore, 1, NULL);
}

void loggerShutdown(void)
{
    logQueue.running = FALSE;
    ReleaseSemaphore(logQueue.semaphore, 1, NULL);

    while(logQueue.totalQueuedMessages > 0)
    {
        Sleep(10);
    }

    DeleteCriticalSection(&logQueue.lock);
    CloseHandle(logQueue.semaphore);
}