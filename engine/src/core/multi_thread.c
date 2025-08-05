#include <core/multi_thread.h>

bool threadInit(_beginthreadex_proc_type startAddress, HANDLE* semaphore, int maxSemaphoreCalls, CRITICAL_SECTION* criticalSection, uintptr_t* hThread)
{
    if(startAddress == 0 || criticalSection == NULL || hThread == NULL)
    {
        return FALSE;
    }
    else
    {
        if (semaphore != NULL && maxSemaphoreCalls != 0)
        {
            *semaphore = CreateSemaphore(NULL, 0, maxSemaphoreCalls, NULL);
            ReleaseSemaphore(*semaphore, 1, NULL);
        }

        InitializeCriticalSection(criticalSection);
        if(criticalSection == NULL)
        {
            return FALSE;
        }

        *hThread = _beginthreadex(NULL, 0, startAddress, NULL, 0, NULL);

        if(*hThread == 0)
        {
            return FALSE;
        }

        return TRUE;
    }
}

void threadShutdown(uintptr_t* hThread, HANDLE* semaphore, CRITICAL_SECTION* criticalSection)
{
    DeleteCriticalSection(criticalSection);
    CloseHandle(*semaphore);
    CloseHandle((HANDLE)*hThread);
}