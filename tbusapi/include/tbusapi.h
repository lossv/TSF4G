#ifndef _H_TBUSAPI_H
#define _H_TBUSAPI_H

#include "terrno.h"
#include "tbus.h"

#include <sys/uio.h>
#include <stdint.h>

#define TBUSAPI_IOV_NUM 65535
typedef struct tbusapi_s tbusapi_t;
typedef void (*tbusapi_on_recviov_func)(tbusapi_t *self, struct iovec *iov, uint32_t iov_num);
typedef void (*tbusapi_on_recv_func)(tbusapi_t *self, const char *buf, size_t buf_len);
typedef tbus_atomic_size_t (*tbusapi_encode_func)(char *dst, size_t dst_len, const char *src, size_t src_len);
struct tbusapi_s
{
	int itb_id;
	tbus_t *itb;

	int otb_id;
	tbus_t *otb;

	struct iovec iov[TBUSAPI_IOV_NUM];
	uint32_t iov_num;

	tbusapi_on_recviov_func on_recviov;
	tbusapi_on_recv_func on_recv;

	tbusapi_encode_func encode;
};

TERROR_CODE tbusapi_init(tbusapi_t *self, key_t input_tbuskey, uint32_t iov_num, key_t output_tbuskey);

void tbusapi_send(tbusapi_t *self, const char *packet, size_t packet_len);

TERROR_CODE tbusapi_process(tbusapi_t *self);

#endif//_H_TBUSAPI_H

