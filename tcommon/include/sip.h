#ifndef _H_SIP_H
#define _H_SIP_H
/*
* The Socket Interface Protocol
* sipʹ��tbus���д��䣬 ��socket�ӿڽ����˰�װ��
*/

#include <stdint.h>

/*sn �ڻػ��в����ظ�*/
typedef uint32_t sip_size_t;
typedef struct sip_cid_s
{
    uint64_t sn;
    uint32_t id;
}sip_cid_t;


/*
 * req =  �̶����ȵİ�ͷ +  ����
 * tbusһ��ֻ����һ��req��server�ô��������򻯴������̡�
 */
typedef enum sip_req_cmd_e
{
	e_sip_req_cmd_connect = 1,
	e_sip_req_cmd_recv = 2,
}sip_req_cmd_t;

typedef struct sip_req_s
{
	sip_req_cmd_t	cmd;            //ָ��
    sip_size_t      size;           //���峤��
	sip_cid_t       cid;            //����id
}sip_req_t;



/*
 * rsp = �������ȵİ�ͷ + ����
 * tbusһ�η��Ͷ��rsp��tconnd�ô�����������ϵͳ���ô�����
 */
typedef enum sip_rsp_cmd_s
{
	e_sip_rsp_cmd_accept = 3,
	e_sip_rsp_cmd_send = 4,
	e_sip_rsp_cmd_close = 5,
}sip_rsp_cmd_t;

#define SIP_BROADCAST_NUM 65536
typedef struct sip_rsp_s
{
	sip_rsp_cmd_t		cmd;                                //ָ��
    sip_size_t          size;                               //���峤��
	uint32_t            cid_list_num;                       //����id����, > 0
	sip_cid_t           cid_list[SIP_BROADCAST_NUM];        //����id����
}sip_rsp_t;
#define SIZEOF_SIP_RSP_T(h) (size_t)((const char*)&(h)->cid_list[(h)->cid_list_num] - (const char*)(h))


#endif//_H_SIP_H
