#include <core/multi_thread.h>

ErrorCode threadInit(_beginthreadex_proc_type startAddress, HANDLE* semaphore, int maxSemaphoreCalls, CRITICAL_SECTION* criticalSection, uintptr_t* hThread)
{
    ErrorCode errorCode = {0};

    if(startAddress == 0)
    {
        errorCode.mainError = 25601;
        errorCode.errorDetail = 1;

        return errorCode;
    }
    if(criticalSection == NULL)
    {
        errorCode.mainError = 25601;
        errorCode.errorDetail = 2;

        return errorCode;
    }
    if(hThread == NULL)
    {
        errorCode.mainError = 25601;
        errorCode.errorDetail = 3;

        return errorCode;
    }
    
    if (semaphore != NULL && maxSemaphoreCalls != 0)
    {
        *semaphore = CreateSemaphore(NULL, 0, maxSemaphoreCalls, NULL);
        ReleaseSemaphore(*semaphore, 1, NULL);
    }

    InitializeCriticalSection(criticalSection);
    if(criticalSection == NULL)
    {
        errorCode.mainError = 25602;
        errorCode.errorDetail = 1;

        return errorCode;
    }

    *hThread = _beginthreadex(NULL, 0, startAddress, NULL, 0, NULL);

    if(*hThread == 0)
    {
        errorCode.mainError = 25603;
        errorCode.errorDetail = 1;

        return errorCode;
    }

    return errorCode;
}

void threadShutdown(uintptr_t* hThread, HANDLE* semaphore, CRITICAL_SECTION* criticalSection)
{
    DeleteCriticalSection(criticalSection);
    CloseHandle(*semaphore);
    CloseHandle((HANDLE)*hThread);
}