typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;

typedef char bchar;
typedef int bint;

#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

STATIC_ASSERT(sizeof(uchar) == 1, "Expected uchar to be 1 byte.");
STATIC_ASSERT(sizeof(ushort) == 2, "Expected ushort to be 2 bytes.");
STATIC_ASSERT(sizeof(uint) == 4, "Expected uint to be 4 bytes.");
STATIC_ASSERT(sizeof(ulonglong) == 8, "Expected ulong to be 8 bytes.");

#define TRUE 1
#define FALSE 0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) 
#define GY_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "Gingey only supports 64-bit!"
#endif
#endif

#ifdef GY_EXPORT
#ifdef _MSC_VER
#define GY_API __declspec(dllexport)
#else
#define GY_API __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define GY_API __declspec(dllimport)
#else
#define GY_API
#endif
#endif