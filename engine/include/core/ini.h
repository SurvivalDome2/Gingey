#pragma once

#include <windows.h>
#include <stdio.h>
#include <ctype.h>

#include <defines.h>

typedef enum
{
    NO_STATE,
    SECTION,
    KEY,
    VALUE
} State;

char* normalizeFilePath(char* filePath);

char* replaceCommentsAndTabsWithSentinelValues(char string[]);

char* prepareFile(char* filePath);

char* parseFile(char* filePath);