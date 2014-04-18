#include "tbusapi.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/uio.h>
#include <errno.h>
#include <string.h>

#include "tlog_print.h"

static void on_recviov(tbusapi_t *self, struct iovec *iov, uint32_t iov_num)
{
	uint32_t i;
	for(i = 0;i < iov_num;++i)
	{
		self->on_recv(self, iov[i].iov_base, iov[i].iov_len);
	}
}

static tbus_atomic_size_t encode(char *dst, size_t dst_len, const char *src, size_t src_len)
{
	if(src_len > dst_len)
	{
		return 0;
	}
	memcpy(dst, src, src_len);
	return (tbus_atomic_size_t)src_len;
}

TERROR_CODE tbusapi_init(tbusapi_t *self, key_t input_tbuskey, uint16_t iov_num, key_t output_tbuskey)
{
	TERROR_CODE ret = E_TS_NOERROR;

	if(input_tbuskey == 0)
	{
		self->itb_id = 0;
		self->itb = NULL;
	}
	else
	{
		self->itb_id = shmget(input_tbuskey, 0, 0666);
		if(self->itb_id == -1)
		{
    		ret = E_TS_ERROR;
		    ERROR_PRINT("shmget reutrn errno %d, %s", errno, strerror(errno));
		    goto done;
		}
		self->itb = shmat(self->itb_id, NULL, 0);
		if((ssize_t)self->itb == -1)
		{
            ret = E_TS_ERROR;
            ERROR_PRINT("shmat reutrn errno %d, %s", errno, strerror(errno));
            goto done;
		}
		
		if(iov_num <= 0)
		{
    		ERROR_PRINT("iov_num must be greater than 0.");
			ret = E_TS_ERROR;
			goto done;
		}
		self->iov_num = iov_num;
	}

	if(output_tbuskey == 0)
	{
		self->otb_id = 0;
		self->otb = NULL;
	}
	else
	{
		self->otb_id = shmget(output_tbuskey, 0, 0666);
		if(self->otb_id == -1)
		{
    		ret = E_TS_ERROR;
		    ERROR_PRINT("shmget reutrn errno %d, %s", errno, strerror(errno));
		    goto done;
		}
		
		self->otb = shmat(self->otb_id, NULL, 0);
		if((ssize_t)self->otb == -1)
		{		
            ret = E_TS_ERROR;
            ERROR_PRINT("shmat reutrn errno %d, %s", errno, strerror(errno));
            goto done;
		}
	}

	self->encode = encode;
	self->on_recviov = on_recviov;
	self->on_recv = NULL;

done:
	return ret;
}

TERROR_CODE tbusapi_process(tbusapi_t *self)
{
	TERROR_CODE ret = E_TS_NOERROR;
	size_t iov_num = self->iov_num;
	tbus_atomic_size_t tbus_head = tbus_read_begin(self->itb, self->iov, &iov_num);
	if(iov_num == 0)
	{
		if(tbus_head != self->itb->head_offset)
		{
			goto read_end;
		}
		else
		{
			ret = E_TS_WOULD_BLOCK;
			goto done;
		}
	}

	if(self->on_recviov)
	{
		self->on_recviov(self, self->iov, (uint32_t)iov_num);
	}

read_end:
	tbus_read_end(self->itb, tbus_head);	
done:
	return ret;
}

void tbusapi_send(tbusapi_t *self, const char *packet, size_t packet_len)
{
	char *buf = NULL;
	tbus_atomic_size_t buf_size;
	tbus_atomic_size_t code_size;

	buf_size = tbus_send_begin(self->otb, &buf);
	code_size = self->encode(buf, buf_size, packet, packet_len);
	if(code_size > 0)
	{
		tbus_send_end(self->otb, code_size);
	}
}

