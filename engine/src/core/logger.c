#include <core/logger.h>

static LogQueue logQueue;

uintptr_t hThread;

bool loggingThreadInit(void)
{
    InitializeCriticalSection(&logQueue.lock);
    logQueue.semaphore = CreateSemaphore(NULL, 0, MAX_LOG_QUEUE, NULL);
    logQueue.running = TRUE;
    logQueue.oldestMessageIndex = 0;
    logQueue.newestMessageIndex = 0;
    logQueue.totalQueuedMessages = 0;

    hThread = _beginthreadex(NULL, 0, loggingThreadProcessor, NULL, 0, NULL);

    ReleaseSemaphore(logQueue.semaphore, 1, NULL);

    return TRUE;
}

unsigned int __stdcall loggingThreadProcessor(void* arg)
{
    (void)arg;

    HANDLE hLogFile = CreateFileA("log.txt", FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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
            char* logLevelString = "";

            switch(logEntry.logLevel)
            {
                case 0:
                    logLevelString = "TRACE";
                    break;
                case 1:
                    logLevelString = "INFO";
                    break;
                case 2:
                    logLevelString = "WARNING";
                    break;
                case 3:
                    logLevelString = "ERROR";
                    break;
                case 4:
                    logLevelString = "FATAL";
                    break;
            }

            SYSTEMTIME time;
            GetSystemTime(&time);
            char formattedMessage[2048];
            int len = snprintf(
                formattedMessage, 
                sizeof(formattedMessage),
                "[%02d:%02d:%02d.%03d] [File: %s] [Line: %d] %s: %s\n",
                time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
                logEntry.file,
                logEntry.line,
                logLevelString,
                logEntry.message
            );

            OutputDebugStringA(formattedMessage);

            printf("%s", formattedMessage);

            DWORD bytesWritten;

            if(!WriteFile(hLogFile, formattedMessage, len, &bytesWritten, NULL))
            {
                DWORD err = GetLastError();
                char message[256];
                snprintf(message, sizeof(message), "WriteFile failed: %lu", err);
                OutputDebugStringA(message);
            }
        }
    }

    return 0;
}

void logEnqueue(LogLevel logLevel, const char* message, SYSTEMTIME time, int line, const char* file)
{
    EnterCriticalSection(&logQueue.lock);
    
    if(!logQueue.running)
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

    LeaveCriticalSection(&logQueue.lock);
    if(shouldReleaseSemaphore)
    {
        ReleaseSemaphore(logQueue.semaphore, 1, NULL);
    }
}

void loggerShutdown(void)
{
    EnterCriticalSection(&logQueue.lock);
    logQueue.running = FALSE;
    LeaveCriticalSection(&logQueue.lock);
    ReleaseSemaphore(logQueue.semaphore, 1, NULL);

    int remaining;
    do
    {
        EnterCriticalSection(&logQueue.lock);
        remaining = logQueue.totalQueuedMessages;
        LeaveCriticalSection(&logQueue.lock);
        if (remaining > 0) Sleep(10);
    } while (remaining > 0);

    DeleteCriticalSection(&logQueue.lock);
    CloseHandle(logQueue.semaphore);
    CloseHandle((HANDLE)hThread);
}