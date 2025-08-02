#pragma once

#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "../defines.h"

#define MAX_LOG_QUEUE 128
#define MESSAGE_CHAR_COUNT 512

typedef enum
{
    LOG_LEVEL_TRACE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

typedef struct {
    LogLevel logLevel;
    char message[MESSAGE_CHAR_COUNT];
    SYSTEMTIME timeOfMessage;
    int line;
    char* file;
} LogEntry;

typedef struct {
    LogEntry logEntries[MAX_LOG_QUEUE];
    int oldestMessageIndex;
    int newestMessageIndex;
    int totalQueuedMessages;
    CRITICAL_SECTION lock;
    HANDLE semaphore;
    volatile bool running;
} LogQueue;

bool loggingThreadInit(void);

void loggerShutdown(void);

DWORD WIN32 loggingThreadProcessor(LPVOID lpParameter);

void logEnqueue(LogLevel logLevel, const char* message, SYSTEMTIME time, int line, const char* file);