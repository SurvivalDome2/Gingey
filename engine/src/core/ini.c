#include <core/ini.h>

LPOFSTRUCT lpOfStruct;

HANDLE hFile;

char* normalizeFilePath(char* filePath)
{
    for(char* c = filePath; *c; c++)
    {
        if(strcmp(c, "\\"))
        {
            c = "/";
        }
    }

    return filePath;
}

char* prepareFile(char* filePath)
{
    SYSTEMTIME time;
    GetLocalTime(&time);
    char* formattedMessage = malloc(57);
    snprintf(
        formattedMessage, 
        strlen(formattedMessage),
        "; File parsed and cached on [%d:%02d:%02d] at [%02d:%02d:%02d]\n\n",
        time.wYear, time.wMonth, time.wDay,
        time.wHour, time.wMinute, time.wSecond
    );

    char* writeBuffer = formattedMessage;

    hFile = CreateFileA(
        filePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    DWORD fileSize = GetFileSize(hFile, NULL);
    
    char* buffer = malloc(fileSize);

    ReadFile(hFile, buffer, fileSize, NULL, NULL);

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    WriteFile(hFile, writeBuffer, strlen(writeBuffer), NULL, NULL);

    buffer[fileSize] = '\0';

    WriteFile(hFile, buffer, strlen(buffer), NULL, NULL);

    buffer = replaceCommentsAndTabsWithSentinelValues(buffer);

    buffer[fileSize] = '\0';

    return buffer;
}

// (FIX) Not sure if the nested for loop is optimal, but I really don't want to work on this any longer than I have to.
char* replaceCommentsAndTabsWithSentinelValues(char string[])
{
    for(int i = 0; string[i]; i++)
    {
        if(string[i] == ';')
        {
            int j = i;

            for(; string[j] != '\n'; j++)
            {
                string[j] = '\x7F';
            }

            i = j - 1;
        }
        else if(string[i] == '\t')
        {
            string[i] = '\x7F';
        }
    }

    return string;
}

void appendToBuffer(char* buffer, char character)
{
    char* c;
    for(c = buffer; *c; c++);

    c[0] = character;
    c[1] = '\0';
}

char* parseFile(char* buffer)
{
    State state = NO_STATE;

    char* currentSection = malloc(64);
    *currentSection = '\0';

    char* currentKey = malloc(64);
    *currentKey = '\0';

    char* currentValue = malloc(64);
    *currentValue = '\0';

    for(char* c = buffer; ; c++)
    {
        if(state == NO_STATE)
        {
            if(*c == '\x7F')
            {
                continue;
            }
            else if(*c == '[')
            {
                state = SECTION;
                appendToBuffer(currentSection, *c);
            }
            else if(*c != ' ' && *c != '\n')
            {
                state = KEY;
                appendToBuffer(currentKey, *c);
            }
            else if(*c == '=')
            {
                state = VALUE;
            }
        }
        else if(state == SECTION)
        {
            if(*c == '\n') state = NO_STATE; // Invalid section
            else if(*c == ']')
            {
                appendToBuffer(currentSection, *c);

                state = NO_STATE;
                printf("Section: %s\n", currentSection);
                memset(currentSection, '\0', 64);
            }
            else
            {
                appendToBuffer(currentSection, *c);
            }
        }
        else if(state == KEY)
        {
            if(*c == '\n') state = NO_STATE; // Invalid key-value pair
            else if(*c == '=')
            {
                state = VALUE;
                printf("Key: %s\n", currentKey);
                memset(currentKey, '\0', 64);
            }
            else
            {
                appendToBuffer(currentKey, *c);
            }
        }
        else if(state == VALUE)
        {
            if(*c == ' ' && *(c - 1) == '=')
            {
                continue;
            }
            else if(*c != '\n' && *c != '\0')
            {
                appendToBuffer(currentValue, *c);
            }
            else if(*c == '\n')
            {
                state = NO_STATE;
                printf("Value: %s\n", currentValue);
                memset(currentValue, '\0', 64);
            }
            else if(*c == '\0')
            {
                printf("Value: %s\n", currentValue);
                memset(currentValue, '\0', 64);
                break;
            }
        }
    }

    CloseHandle(hFile);

    return buffer;
}