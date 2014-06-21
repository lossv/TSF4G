#ifndef _H_TLIBC_PLATFORM_WINDOWS
#define _H_TLIBC_PLATFORM_WINDOWS

#ifdef  __cplusplus
extern "C" {
#endif

#include <stddef.h>
//���������stdlib.h strtoll�����
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>

#define strtoll(begin_ptr, end_ptr, radix) _strtoi64(begin_ptr, end_ptr, radix)
#define strtoull(begin_ptr, end_ptr, radix) _strtoui64(begin_ptr, end_ptr, radix)
#define snprintf _snprintf
#define snwprintf _snwprintf

#ifdef _WIN32
#define TLIBC_WORDSIZE 32
#elif _WIN64
#define TLIBC_WORDSIZE 64
#else
#error "unknow wordsize"
#endif


#define PRIi64 "lli"
#define PRIu64 "llu"
#define PRIx64 "llx"


//���ι��
#pragma warning (disable: 4996)

#ifdef  __cplusplus
}
#endif

#endif
