#pragma once

#include <stdbool.h>
#include <process.h>

#include <windows.h>

#include <defines.h>

ErrorCode threadInit(_beginthreadex_proc_type startAddress, HANDLE* semaphore, int maxSemaphoreCalls, CRITICAL_SECTION* criticalSection, uintptr_t* hThread);

void threadShutdown(uintptr_t* hThread, HANDLE* semaphore, CRITICAL_SECTION* criticalSection);