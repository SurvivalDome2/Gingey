#include <core/main.h>

int main(void)
{
    char* filePath = "test.ini";

    normalizeFilePath(filePath);

    char* buffer = prepareFile(filePath);

    parseFile(buffer);

    return 0;
}