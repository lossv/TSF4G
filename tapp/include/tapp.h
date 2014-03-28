#ifndef _H_TAPP
#define _H_TAPP

#include "protocol/tlibc_xml_reader.h"
#include "terrno.h"
#include <unistd.h>



#define TAPP_VERSION "0.0.1"

//��ȡ�ṹ��ĺ���ָ�룬 �������Ӧ������TData�����ɵġ�
typedef TLIBC_ERROR_CODE (*tapp_xml_reader_t)(TLIBC_ABSTRACT_READER *self, void *data);

/*
*  ��ȡ�����в����� �����ȡʧ�ܻ�exit 1
*/
void tapp_load_config(void *config, int argc, char *argv[], tapp_xml_reader_t reader);

/*
* ��ѭ������
* E_TS_NOERROR �ɹ�����һ������
* E_TS_WOULD_BLOCK û����Ҫ���������
* ��������ֵΪ�������
*/
typedef TERROR_CODE (*tapp_process_t)();

/*
*  ���Ȼ�ע���źŴ�����
* SIGTERM �����˳�
*
*Ȼ���ѭ��ִ��process
*  ���process ����E_TS_NOERROR������Ͻ�����һ��process
*  ���process ��������E_TS_WOULD_BLOCK ����idle_limit��, ��usleep(usec)��
*  ���process ���� ��ôtapp_loop�����᷵��process����������롣
*/
TERROR_CODE tapp_loop(tapp_process_t process, useconds_t usec, size_t idle_limit);



#endif//_H_TAPP
