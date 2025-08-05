#pragma once

#include <windows.h>
#include <process.h>
#include <stdbool.h>

#include <defines.h>

bool threadInit(_beginthreadex_proc_type startAddress, HANDLE* semaphore, int maxSemaphoreCalls, CRITICAL_SECTION* criticalSection, uintptr_t* hThread);

void threadShutdown(uintptr_t* hThread, HANDLE* semaphore, CRITICAL_SECTION* criticalSection);