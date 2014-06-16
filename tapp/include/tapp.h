#ifndef _H_TAPP
#define _H_TAPP

#ifdef  __cplusplus
extern "C" {
#endif

#include "protocol/tlibc_xml_reader.h"
#include "tlibc_error_code.h"
#include <unistd.h>



#define TAPP_VERSION "0.0.1"
#define TAPP_IDLE_USEC 1000
#define TAPP_IDLE_LIMIT 30


//��ȡ�ṹ��ĺ���ָ�룬 �������Ӧ������TData�����ɵġ�
typedef tlibc_error_code_t (*tapp_xml_reader_t)(tlibc_abstract_reader_t *self, void *data);

/*
*  ��ȡ�����в����� �����ȡʧ�ܻ�exit 1
*/
void tapp_load_config(void *config, int argc, char *argv[], tapp_xml_reader_t reader);


typedef TERROR_CODE (*tapp_func_t)(void *arg);
/*
*  ���Ȼ�ע���źŴ������� Ȼ��ѭ��ִ�����²���
* 1. ����յ�SIGTERM ��SIGINT �źţ���ѭ����break��
* 2. ����յ�SIGUSR1 �źţ�ִ��sigusr1����������򷵻ء�
* 3. ����յ�SIGUSR2 �źţ�ִ��sigusr2����������򷵻ء�
* 4. ����SIGPIPE�ź�
* 5. ִ�����е�process ������ ֱ������һ��NULL��
*      �����process���� ��ôtapp_loop�������������
*      �������process������E_TS_WOULD_BLOCK,  ��ô++idle_count�� �������idle_limit����usleep(usec)��
*      �����һ��process����E_TS_NOERROR�� ��ôidle_count = 0��
* ����:
*     tapp_loop(TAPP_IDLE_USEC, TAPP_IDLE_LIMIT, NULL, NULL, NULL, NULL
                     , process, NULL
                     , NULL, NULL)

*/
TERROR_CODE tapp_loop(useconds_t idle_usec, size_t idle_limit,
                        tapp_func_t sigusr1, void* usr1_arg,
                        tapp_func_t sigusr2, void* usr2_arg,
                        ...);




#ifdef  __cplusplus
}
#endif

#endif//_H_TAPP

