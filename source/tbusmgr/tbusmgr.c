#include <stdio.h>

#include "tserver/tbus/tbus.h"

void useage() 
{
	printf ( "\n" ) ;
	printf ( "TBus Version %s.\n", TBUS_VERSION);
	printf ( "Usage:\n" ) ;
	printf ( "tbusmgr ([-s size] && [-w id] | [-d id])\n");
	printf ( "-w size -w id\t��������id�� ��СΪsize�Ĺ����ڴ�\n");
	printf ( "-d id\tɾ��idͨ��\n");
}

int main()
{

	useage();
	
	test();
	
	return 0;
}

