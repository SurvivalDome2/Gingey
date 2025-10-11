#pragma once

#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <process.h>
#include <stdatomic.h>

#include "../../engine/include/defines.h"

#include <core/multi_thread.h>

#define MAX_LOG_QUEUE 128
#define MESSAGE_CHAR_COUNT 512

extern SYSTEMTIME systemTime;

typedef enum
{
    LOG_LEVEL_TRACE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

typedef struct
{
    LogLevel logLevel;
    char message[MESSAGE_CHAR_COUNT];
    SYSTEMTIME timeOfMessage;
    int line;
    char file[255]; // Maybe make this dynamic? But seems unnecessary for now
} LogEntry;

typedef struct
{
    LogEntry logEntries[MAX_LOG_QUEUE];
    int oldestMessageIndex;
    int newestMessageIndex;
    int totalQueuedMessages;
    CRITICAL_SECTION criticalSection;
    HANDLE semaphore;
    volatile atomic_bool running;
} LogQueue;

bool loggerInit(void);

unsigned int __stdcall loggingThreadProcessor(void* arg); // Requires void* arg, because Windows sucks

void logEnqueue(LogLevel logLevel, const char* message, SYSTEMTIME time, int line, const char* file); // Adds log messages to queue

void loggerShutdown(void);