#ifndef _H_TBUS
#define _H_TBUS

#include "platform/tlibc_platform.h"

#include "terrno.h"

#include <stdint.h>

#define TBUS_VERSION "0.0.1"

typedef uint32_t tbus_atomic_size_t;

typedef enum tbus_cmd_e
{
    e_tbus_cmd_package = 1,
    e_tbus_cmd_ignore = 2,
}tbus_cmd_t;

typedef struct tbus_header_s
{
    tbus_cmd_t cmd;
    tbus_atomic_size_t size;
}tbus_header_t;


typedef struct tbus_s
{
	volatile tbus_atomic_size_t head_offset;
	volatile tbus_atomic_size_t tail_offset;
	tbus_atomic_size_t packet_size;
	tbus_atomic_size_t size;
	char buff[1];
}tbus_t;


TERROR_CODE tbus_init(tbus_t *tb, size_t size, size_t number);

tbus_atomic_size_t tbus_send_begin(tbus_t *tb, TLIBC_OUT char** buf);

void tbus_send_end(tbus_t *tb, tbus_atomic_size_t len);


tbus_atomic_size_t tbus_read_begin(tbus_t *tb, TLIBC_OUT char** buf);

void tbus_read_end(tbus_t *tb, tbus_atomic_size_t len);

typedef tbus_atomic_size_t (*tbus_encode_t)(const void *self, char *start, char *limit);



typedef struct tiovec_s
{
	const void  *iov_base;    /* Starting address */
	size_t iov_len;     /* Number of bytes to transfer */
}tiovec_t;

size_t tbus_peek(tbus_t *tb, tiovec_t *iov, size_t iov_num);
void tbus_peek_over(tbus_t *tb);


#endif//_H_TBUS

