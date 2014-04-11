#include "tconnd_epoll.h"
#include "core/tlibc_list.h"
#include "terrno.h"
#include "tconnd_socket.h"
#include "tconnd.h"
#include "tlog_log.h"
#include "tconnd_listen.h"
#include <unistd.h>
#include <assert.h>
#include <sys/epoll.h>

#include <errno.h>
#include <string.h>

int                         g_epollfd;
static tlibc_list_head_t      readable_list;
tlibc_list_head_t             g_package_socket_list;
tlibc_list_head_t             g_pending_socket_list;


TERROR_CODE tconnd_epoll_init()
{
    TERROR_CODE ret = E_TS_NOERROR;

	tlibc_list_init(&readable_list);
	tlibc_list_init(&g_package_socket_list);	
    tlibc_list_init(&g_pending_socket_list);

    if(g_config.connections > INT_MAX)
    {
        ERROR_LOG("g_config.connections [%u] > INT_MAX.", g_config.connections);
        ret = E_TS_ERROR;
        goto done;
    }

	g_epollfd = epoll_create((int)g_config.connections);
	if(g_epollfd == -1)
	{
        ERROR_LOG("epoll_create errno[%d], %s.", errno, strerror(errno));
	    ret = E_TS_ERRNO;
		goto done;
	}


done:
    return ret;
}

#define TCONND_EPOLL_MAX_EVENTS 1024
TERROR_CODE tconnd_epool_proc()
{
	int i;
	TERROR_CODE ret = E_TS_WOULD_BLOCK;
	tlibc_list_head_t *iter, *next;

	if(tlibc_list_empty(&readable_list))
	{
		struct epoll_event 	events[TCONND_EPOLL_MAX_EVENTS];
		int                 events_num;

		events_num = epoll_wait(g_epollfd, events, TCONND_EPOLL_MAX_EVENTS, 0);
	    if(events_num == -1)
		{
		    //�п��ܱ�ʱ�Ӵ��
		    if(errno == EINTR)
		    {
		        ret = E_TS_NOERROR;
		        goto done;
		    }
		    
	    
            ERROR_LOG("epoll_wait errno[%d], %s.", errno, strerror(errno));
	        ret = E_TS_ERRNO;
			goto done;
	    }

	    for(i = 0; i < events_num; ++i)
	    {
            tconnd_socket_t *socket = events[i].data.ptr;
            if(socket->readable)
            {
                ERROR_LOG("socket [%u, %"PRIu64"] already readable.", socket->id, socket->mempool_entry.sn);
                assert(0);
                ret = E_TS_ERROR;
                goto done;
            }
            socket->readable = TRUE;
            tlibc_list_init(&socket->readable_list);
            tlibc_list_add_tail(&socket->readable_list, &readable_list);
	    }
	}

	if(tlibc_list_empty(&readable_list))
	{
	    ret = E_TS_WOULD_BLOCK;
	    goto done;
	}
	
    for(iter = readable_list.next; iter != &readable_list; iter = next)
    {
        TERROR_CODE r;
        tconnd_socket_t *socket = TLIBC_CONTAINER_OF(iter, tconnd_socket_t, readable_list);
        next = iter->next;

        if(socket == &g_listen)
        {
            r = tconnd_listen();
        }
        else
        {
            r = tconnd_socket_recv(socket);
        }
        
        switch(r)
        {
        case E_TS_NOERROR:
            ret = E_TS_NOERROR;
            break;
        case E_TS_TBUS_NOT_ENOUGH_SPACE:
            ret = E_TS_WOULD_BLOCK;
            goto done;
        case E_TS_TOO_MANY_SOCKET:
            break;
        case E_TS_WOULD_BLOCK:
            break;
        case E_TS_ERRNO:
            switch(errno)
            {
                case EAGAIN:
                    socket->readable = FALSE;
                    tlibc_list_del(iter);
                    break;
                case EINTR:
                    break;
                default:
                    ret = E_TS_ERRNO;
                    goto done;
            }
            break;
        case E_TS_NO_MEMORY:
            {
                tconnd_socket_t *sock = NULL;
                if(tlibc_list_empty(&g_package_socket_list))
                {
                    ret = E_TS_NO_MEMORY;
                    ERROR_LOG("Not enough package buff.");
                    break;
                }

                sock = TLIBC_CONTAINER_OF(g_package_socket_list.next, tconnd_socket_t, g_package_socket_list);
                assert(sock->package_buff != NULL);
                WARN_LOG("close socket [%u, %"PRIu64"] to release package buff.", sock->id, sock->mempool_entry.sn);
                tconnd_socket_delete(sock);
            }
            break;
        case E_TS_CLOSE:
            tconnd_socket_delete(socket);
            break;
        default:
            ret = r;
            goto done;
        }        
    }

done:	
	return ret;
}

void tconnd_epoll_fini()
{
    if(close(g_epollfd) != 0)
    {
        ERROR_LOG("close errno[%d], %s", errno, strerror(errno));
    }
}

