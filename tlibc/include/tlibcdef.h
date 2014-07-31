#ifndef _H_TLIBCDEF_H
#define _H_TLIBCDEF_H

#include <stdint.h>
#include <stddef.h>

#define TLIBC_MAX_LENGTH_OF_IDENTIFIER 255
#define TLIBC_MAX_PATH_LENGTH 1024


#define TLIBC_UNUSED(x) (void)(x)

#define TLIBC_OFFSET_OF(type, member) ((size_t) &((type *)0)->member)

#define TLIBC_CONTAINER_OF(ptr, type, member) ((type *)((size_t)ptr - TLIBC_OFFSET_OF(type, member)))

typedef size_t (*tlibc_encode_t)(const void *data, char *start, char *limit);

#endif//_H_TLIBCDEF_H
