#ifndef _H_SIP_H
#define _H_SIP_H
/*
* The Socket Interface Protocol
*/

#include <stdint.h>
#include <unistd.h>
#include "tlibc/core/tlibc_util.h"

#pragma pack(push,1)
typedef uint32_t sip_size_t;
typedef struct _sip_cid_t
{
    uint64_t sn;
    uint32_t id;
}sip_cid_t;


typedef enum _sip_req_cmd_t
{
	e_sip_req_cmd_bein = 1,
	e_sip_req_cmd_connect = 2,
	e_sip_req_cmd_recv = 3,
    e_sip_req_cmd_end = 4,
}sip_req_cmd_t;

/*����=  �̶����ȵİ�ͷ +  ����*/
typedef struct _sip_req_t
{
	sip_req_cmd_t	cmd;            //ָ��
    sip_size_t      size;           //���峤��
	sip_cid_t       cid;            //����id
}sip_req_t;
#define SIP_REQ_SIZE sizeof(sip_req_t)

#define sip_req_t_code(h) \
{\
    tlibc_host16_to_little((h)->cmd)\
    tlibc_host64_to_little((h)->cid.sn)\
    tlibc_host32_to_little((h)->cid.id)\
    tlibc_host32_to_little((h)->size)\
}

#define sip_req_t_decode(h) \
{\
    tlibc_little_to_host16((h)->cmd)\
    tlibc_little_to_host32((h)->size)\
    tlibc_little_to_host64((h)->cid.sn)\
    tlibc_little_to_host32((h)->cid.id)\
}


typedef enum _sip_rsp_cmd_t
{
	e_sip_rsp_cmd_accept = 3,
	e_sip_rsp_cmd_send = 4,
	e_sip_rsp_cmd_close = 5,
}sip_rsp_cmd_t;

/*�ظ�= �������ȵİ�ͷ + ����*/
#define SIP_BROADCAST_NUM 65536
typedef struct _sip_rsp_t
{
	sip_rsp_cmd_t		cmd;                                //ָ��
    sip_size_t          size;                               //���峤��
	uint32_t            cid_list_num;                       //����id����
	sip_cid_t           cid_list[SIP_BROADCAST_NUM];        //����id����
}sip_rsp_t;
#define SIP_RSP_T_CODE_SIZE(h) (sizeof(sip_rsp_t) - sizeof(sip_cid_t) * (SIP_BROADCAST_NUM - (h)->cid_list_num))

#define sip_rsp_t_code(h) \
{\
    size_t i;\
    tlibc_host16_to_little((h)->cmd)\
    tlibc_host32_to_little((h)->size)\
    tlibc_host32_to_little((h)->cid_list_num)\
    for(i = 0; i < (h)->cid_list_num; ++i)\
    {\
        tlibc_host64_to_little((h)->cid_list[i].sn)\
        tlibc_host32_to_little((h)->cid_list[i].id)\
    }\
}

//��������ʾ�ɹ�����ĳ���
//-1��ʾ��������
ssize_t sip_rst_t_decode(char *buff_start, char *buff_limit);



#pragma pack(pop)


#endif//_H_SIP_H
