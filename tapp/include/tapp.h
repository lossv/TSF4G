#ifndef _H_TAPP
#define _H_TAPP

#include "protocol/tlibc_xml_reader.h"
#include "terrno.h"
#include <unistd.h>



#define TAPP_VERSION "0.0.1"
#define TAPP_IDLE_USEC 1000
#define TAPP_IDLE_LIMIT 30


//��ȡ�ṹ��ĺ���ָ�룬 �������Ӧ������TData�����ɵġ�
typedef TLIBC_ERROR_CODE (*tapp_xml_reader_t)(TLIBC_ABSTRACT_READER *self, void *data);

/*
*  ��ȡ�����в����� �����ȡʧ�ܻ�exit 1
*/
void tapp_load_config(void *config, int argc, char *argv[], tapp_xml_reader_t reader);


typedef TERROR_CODE (*tapp_func_t)();
/*
*  ���Ȼ�ע���źŴ������� Ȼ��ѭ��ִ�����²���
* 1. ����յ�SIGTERM ��SIGINT �źţ���ѭ����break��
* 2. ����յ�SIGUSR1 �źţ�ִ��sigusr1����������򷵻ء�
* 3. ����յ�SIGUSR2 �źţ�ִ��sigusr2����������򷵻ء�
* 4. ����SIGPIPE�ź�
* 5. ִ��process
*      ���process ����E_TS_NOERROR������Ͻ�����һ��process
*      ���process ��������E_TS_WOULD_BLOCK ����idle_limit��, ��usleep(usec)��
*      ���process ���� ��ôtapp_loop�����᷵��process����������롣
*/
TERROR_CODE tapp_loop(tapp_func_t process, useconds_t idle_usec, size_t idle_limit,
                        tapp_func_t sigusr1, tapp_func_t sigusr2);



#endif//_H_TAPP
