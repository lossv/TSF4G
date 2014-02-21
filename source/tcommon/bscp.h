#ifndef _H_BSCP_H
#define _H_BSCP_H

/*
* The binary stream cutting protocol.
* �ֽ����и�Э��
*
* Э��= ��ͷ +  ����
* ��ͷ= ΪС�˱������������ ��ʾ����ĳ��ȡ�
*
*/

#include <stdint.h>
#include "tlibc/core/tlibc_util.h"

typedef uint16_t bscp_head_t;

#define bscp_head_t_code(head) {tlibc_host16_to_little(head);}

#define bscp_head_t_decode(head) {tlibc_little_to_host16(head);}


#endif//_H_BSCP_H

